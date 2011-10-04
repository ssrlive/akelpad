/***********************************************************************************
 *                      AkelAdmin UAC support for AkelPad                          *
 *                                                                                 *
 * Copyright 2011 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)       *
 *                                                                                 *
 * License: this source is distributed under "BSD license" conditions.             *
 ***********************************************************************************/


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <aclapi.h>
#include "..\AkelEdit\StrFunc.h"

//Include string functions
#define xatoiW
#include "..\AkelEdit\StrFunc.h"


//Defines
#define STRID_ERRORWIN              1
#define STRID_ERRORDIR              2
#define STRID_ERRORCALL             3
#define STRID_ERRORSETPIPESECURITY  4
#define STRID_ERRORGETFILESECURITY  5
#define STRID_ERRORSETFILESECURITY  6

#define BUFFER_SIZE      1024

//AkelAdmin action
#define AAA_CMDINIT          11
#define AAA_EXIT             20  //Exit from pipe server.
#define AAA_SECURITYSAVE     21  //Retrieve file security.
#define AAA_SECURITYEVERYONE 22  //Add all access for the file.
#define AAA_SECURITYRESTORE  23  //Restore saved security for the file.
#define AAA_SECURITYFREE     24  //Free saved security.

#define STR_AKELADMIN L"AkelAdmin"

#ifndef INVALID_FILE_ATTRIBUTES
  #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif
#ifndef DACL_SECURITY_INFORMATION
  #define DACL_SECURITY_INFORMATION  (0x00000004L)
#endif
#ifndef GROUP_SECURITY_INFORMATION
  #define GROUP_SECURITY_INFORMATION (0x00000002L)
#endif
#ifndef LABEL_SECURITY_INFORMATION
  #define LABEL_SECURITY_INFORMATION (0x00000010L)
#endif
#ifndef OWNER_SECURITY_INFORMATION
  #define OWNER_SECURITY_INFORMATION (0x00000001L)
#endif

typedef struct {
  DWORD dwExitCode;
  int nAction;
  wchar_t wszFile[MAX_PATH];
  DWORD dwLangModule;
} ADMINPIPE;


//Functions prototypes
int GetExeDir(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen);
const wchar_t* GetFileName(const wchar_t *wpFile);
int GetCommandLineArgW(wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, wchar_t **wpNextArg);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszAkelAdminPipe[32];
HINSTANCE hInstance;
HWND hPipeAkelAdmin;
LANGID wLangModule=0;

//GetProcAddress
BOOL (WINAPI *GetNamedPipeClientProcessIdPtr)(HANDLE, ULONG *);
DWORD (WINAPI *SetNamedSecurityInfoWPtr)(wchar_t *, SE_OBJECT_TYPE, SECURITY_INFORMATION, PSID, PSID, PACL, PACL);
DWORD (WINAPI *SetSecurityInfoPtr)(HANDLE, SE_OBJECT_TYPE, SECURITY_INFORMATION, PSID, PSID, PACL, PACL);
DWORD (WINAPI *SetEntriesInAclWPtr)(ULONG, PEXPLICIT_ACCESSW, PACL, PACL *);


//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

//Entry point
void _WinMain()
{
  wchar_t *wpCmdLine=GetCommandLineW();
  wchar_t *pArguments=wpCmdLine;
  int nAction=0;
  DWORD dwExitCode=1;

  //Get program HINSTANCE
  hInstance=GetModuleHandleW(NULL);

  //Get default language ID
  wLangModule=PRIMARYLANGID(GetUserDefaultLangID());

  if (hInstance)
  {
    //Get program directory
    GetExeDir(hInstance, wszBuffer, BUFFER_SIZE);

    if (!lstrcmpiW(L"AkelFiles", GetFileName(wszBuffer)))
    {
      //Skip executable
      GetCommandLineArgW(pArguments, NULL, 0, &pArguments);

      //First argument is action number (currently only one action).
      if (GetCommandLineArgW(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
      {
        nAction=(int)xatoiW(wszBuffer, NULL);

        //Initialize pipe server
        if (nAction == AAA_CMDINIT)
        {
          //Second argument is caller process id.
          if (GetCommandLineArgW(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
          {
            HANDLE hPipeAkelAdmin;
            HANDLE hMutex;
            HMODULE hKernel32;
            HMODULE hAdvApi32;
            DWORD dwInitProcessId;
            DWORD dwClientProcessId;

            dwInitProcessId=(DWORD)xatoiW(wszBuffer, NULL);

            if (hMutex=OpenEventW(EVENT_MODIFY_STATE, FALSE, STR_AKELADMIN))
            {
              wsprintfW(wszAkelAdminPipe, L"\\\\.\\pipe\\%s-%d", STR_AKELADMIN, dwInitProcessId);

              //Get functions addresses
              hKernel32=GetModuleHandleW(L"kernel32.dll");
              GetNamedPipeClientProcessIdPtr=(BOOL (WINAPI *)(HANDLE, ULONG *))GetProcAddress(hKernel32, "GetNamedPipeClientProcessId");

              hAdvApi32=GetModuleHandleW(L"advapi32.dll");
              SetNamedSecurityInfoWPtr=(DWORD (WINAPI *)(wchar_t *, SE_OBJECT_TYPE, SECURITY_INFORMATION, PSID, PSID, PACL, PACL))GetProcAddress(hAdvApi32, "SetNamedSecurityInfoW");
              SetSecurityInfoPtr=(DWORD (WINAPI *)(HANDLE, SE_OBJECT_TYPE, SECURITY_INFORMATION, PSID, PSID, PACL, PACL))GetProcAddress(hAdvApi32, "SetSecurityInfo");
              SetEntriesInAclWPtr=(DWORD (WINAPI *)(ULONG, PEXPLICIT_ACCESSW, PACL, PACL *))GetProcAddress(hAdvApi32, "SetEntriesInAclW");

              if ((hPipeAkelAdmin=CreateNamedPipeW(wszAkelAdminPipe, PIPE_ACCESS_DUPLEX|PIPE_WAIT|WRITE_DAC, 0, PIPE_UNLIMITED_INSTANCES, sizeof(ADMINPIPE), sizeof(ADMINPIPE), 0, NULL)) != INVALID_HANDLE_VALUE)
              {
                ACL *pEveryoneACL=NULL;
                EXPLICIT_ACCESSW eal[1];
                SID_IDENTIFIER_AUTHORITY SIDAuthWorld=SECURITY_WORLD_SID_AUTHORITY;
                SID *pSIDEveryone=NULL;
                SECURITY_DESCRIPTOR *psdCurrentFile=NULL;
                wchar_t wszCurrentFile[MAX_PATH];
                BOOL bChangeAccessResult=FALSE;

                //Specify the DACL to use. Create a SID for the Everyone group.
                if (AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, (void **)&pSIDEveryone))
                {
                  eal[0].grfAccessPermissions=GENERIC_ALL;
                  eal[0].grfAccessMode=SET_ACCESS;
                  eal[0].grfInheritance=NO_INHERITANCE;
                  eal[0].Trustee.TrusteeForm=TRUSTEE_IS_SID;
                  eal[0].Trustee.TrusteeType=TRUSTEE_IS_WELL_KNOWN_GROUP;
                  eal[0].Trustee.ptstrName=(wchar_t *)pSIDEveryone;
                  eal[0].Trustee.MultipleTrusteeOperation=NO_MULTIPLE_TRUSTEE;
                  eal[0].Trustee.pMultipleTrustee=NULL;

                  if (SetEntriesInAclWPtr(1, eal, NULL, &pEveryoneACL) == ERROR_SUCCESS)
                  {
                    if (SetSecurityInfoPtr(hPipeAkelAdmin, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pEveryoneACL, NULL) == ERROR_SUCCESS)
                      bChangeAccessResult=TRUE;
                  }
                }
                if (!bChangeAccessResult)
                  MessageBoxW(NULL, GetLangStringW(wLangModule, STRID_ERRORSETPIPESECURITY), STR_AKELADMIN, MB_ICONERROR);

                //Signal mutex
                SetEvent(hMutex);

                //Wait for client connect with CreateFile
                for (;;)
                {
                  ADMINPIPE apipe;
                  DWORD dwBytesRead;
                  DWORD dwBytesWritten;
                  BOOL bBreak=FALSE;

                  if (ConnectNamedPipe(hPipeAkelAdmin, NULL))
                  {
                    //Accept connection only from initial caller that runs this process
                    if (!GetNamedPipeClientProcessIdPtr || (GetNamedPipeClientProcessIdPtr(hPipeAkelAdmin, &dwClientProcessId) && dwClientProcessId == dwInitProcessId))
                    {
                      //Wait for client WriteFile
                      if (ReadFile(hPipeAkelAdmin, &apipe, sizeof(ADMINPIPE), &dwBytesRead, NULL))
                      {
                        apipe.dwExitCode=0;

                        if (apipe.nAction == AAA_EXIT)
                        {
                          //Unload process
                          bBreak=TRUE;
                        }
                        else if (apipe.nAction == AAA_SECURITYSAVE)
                        {
                          SECURITY_INFORMATION ssi=DACL_SECURITY_INFORMATION|
                                                   GROUP_SECURITY_INFORMATION|
                                                   LABEL_SECURITY_INFORMATION|
                                                   OWNER_SECURITY_INFORMATION;
                          DWORD dwSize=0;

                          wLangModule=PRIMARYLANGID(apipe.dwLangModule);

                          //Retrieve file security
                          if (!psdCurrentFile)
                          {
                            GetFileSecurityW(apipe.wszFile, ssi, NULL, 0, &dwSize);

                            if (dwSize)
                            {
                              if (psdCurrentFile=(SECURITY_DESCRIPTOR *)GlobalAlloc(GMEM_FIXED, dwSize))
                              {
                                if (!GetFileSecurityW(apipe.wszFile, ssi, psdCurrentFile, dwSize, &dwSize))
                                {
                                  wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORGETFILESECURITY), apipe.wszFile);
                                  MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                                  apipe.dwExitCode=1;
                                }
                              }
                            }

                            if (apipe.dwExitCode)
                            {
                              GlobalFree(psdCurrentFile);
                              psdCurrentFile=NULL;
                            }
                            else lstrcpynW(wszCurrentFile, apipe.wszFile, MAX_PATH);
                          }
                          else apipe.dwExitCode=1;
                        }
                        else if (apipe.nAction == AAA_SECURITYEVERYONE)
                        {
                          wLangModule=PRIMARYLANGID(apipe.dwLangModule);

                          //Decrease file security
                          if (psdCurrentFile)
                          {
                            if (SetNamedSecurityInfoWPtr(apipe.wszFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pEveryoneACL, NULL) != ERROR_SUCCESS)
                            {
                              wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORSETFILESECURITY), apipe.wszFile);
                              MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                              apipe.dwExitCode=1;
                            }
                          }
                        }
                        else if (apipe.nAction == AAA_SECURITYRESTORE)
                        {
                          SECURITY_INFORMATION ssi=DACL_SECURITY_INFORMATION|
                                                   GROUP_SECURITY_INFORMATION|
                                                   LABEL_SECURITY_INFORMATION|
                                                   OWNER_SECURITY_INFORMATION;

                          wLangModule=PRIMARYLANGID(apipe.dwLangModule);

                          //Restore file security
                          if (psdCurrentFile)
                          {
                            if (!SetFileSecurityW(wszCurrentFile, ssi, psdCurrentFile))
                            {
                              wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORSETFILESECURITY), wszCurrentFile);
                              MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                              apipe.dwExitCode=1;
                            }
                          }
                          else apipe.dwExitCode=1;
                        }
                        else if (apipe.nAction == AAA_SECURITYFREE)
                        {
                          //Free security buffer
                          if (psdCurrentFile)
                          {
                            GlobalFree(psdCurrentFile);
                            psdCurrentFile=NULL;
                          }
                          else apipe.dwExitCode=1;
                        }
                        WriteFile(hPipeAkelAdmin, &apipe, sizeof(ADMINPIPE), &dwBytesWritten, NULL);
                        FlushFileBuffers(hPipeAkelAdmin);
                      }
                    }
                    DisconnectNamedPipe(hPipeAkelAdmin);
                    if (bBreak) break;
                  }
                }
                dwExitCode=0;

                if (psdCurrentFile) GlobalFree(psdCurrentFile);
                if (pEveryoneACL) LocalFree(pEveryoneACL);
                if (pSIDEveryone) FreeSid(pSIDEveryone);
                CloseHandle(hPipeAkelAdmin);
              }
            }
          }
        }
      }
      if (nAction == 0)
        MessageBoxW(NULL, GetLangStringW(wLangModule, STRID_ERRORCALL), STR_AKELADMIN, MB_ICONEXCLAMATION);
    }
    else MessageBoxW(NULL, GetLangStringW(wLangModule, STRID_ERRORDIR), STR_AKELADMIN, MB_ICONEXCLAMATION);
  }
  else MessageBoxW(NULL, GetLangStringW(wLangModule, STRID_ERRORWIN), STR_AKELADMIN, MB_ICONEXCLAMATION);

  ExitProcess(dwExitCode);
}

int GetExeDir(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen)
{
  if (nLen=GetModuleFileNameW(hInstance, wszExeDir, nLen))
  {
    while (nLen > 0 && wszExeDir[nLen] != '\\') --nLen;
    wszExeDir[nLen]='\0';
  }
  return nLen;
}

const wchar_t* GetFileName(const wchar_t *wpFile)
{
  int i;

  for (i=lstrlenW(wpFile) - 1; i >= 0; --i)
  {
    if (wpFile[i] == '\\')
      return (wpFile + i + 1);
  }
  return wpFile;
}

int GetCommandLineArgW(wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, wchar_t **wpNextArg)
{
  wchar_t *wpCount=wpCmdLine;
  wchar_t *wpArgSet=wszArg;
  wchar_t *wpArgSetMax=wszArg + nArgMax - 1;
  wchar_t wchInitStopChar;
  wchar_t wchCurStopChar;

  while (*wpCount == L' ') ++wpCount;

  if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
    wchInitStopChar=*wpCount++;
  else
    wchInitStopChar=L' ';
  wchCurStopChar=wchInitStopChar;

  for (; *wpCount; ++wpCount)
  {
    if (wchCurStopChar == L' ')
    {
      if (*wpCount == L' ')
        break;
      if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
        wchCurStopChar=*wpCount;
    }
    else if (wchCurStopChar == *wpCount)
    {
      if (wchCurStopChar == wchInitStopChar)
        break;
      wchCurStopChar=L' ';
    }

    if (wpArgSet < wpArgSetMax)
    {
      if (wszArg) *wpArgSet=*wpCount;
      ++wpArgSet;
    }
  }
  if (wszArg) *wpArgSet=L'\0';

  if (wpNextArg)
  {
    if (*wpCount) ++wpCount;
    for (*wpNextArg=wpCount; **wpNextArg == L' '; ++*wpNextArg);
  }
  return (int)(wpArgSet - wszArg);
}

const wchar_t* GetLangStringW(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_ERRORWIN)
      return L"\x041F\x0440\x0435\x0434\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x043E\x0020\x0434\x043B\x044F\x0020\x004E\x0054\x0020\x0441\x0438\x0441\x0442\x0435\x043C\x002E";
    if (nStringID == STRID_ERRORDIR)
      return L"AkelAdmin \x0434\x043E\x043B\x0436\x0435\x043D\x0020\x043D\x0430\x0445\x043E\x0434\x0438\x0442\x044C\x0441\x044F\x0020\x0432\x0020\x043F\x0430\x043F\x043A\x0435 AkelFiles.";
    if (nStringID == STRID_ERRORCALL)
      return L"\x0414\x0430\x043D\x043D\x0430\x044F\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x0430\x0020\x043F\x0440\x0435\x0434\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x0430\x0020\x0434\x043B\x044F\x0020\x0432\x043D\x0443\x0442\x0440\x0435\x043D\x043D\x0435\x0433\x043E\x0020\x0438\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x043D\x0438\x044F AkelPad'\x043E\x043C. \x041D\x0435\x0020\x0432\x044B\x0437\x044B\x0432\x0430\x0439\x0442\x0435\x0020\x0435\x0435\x0020\x043D\x0430\x043F\x0440\x044F\x043C\x0443\x044E.";
    if (nStringID == STRID_ERRORSETPIPESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0434\x043B\x044F\x0020\x043A\x0430\x043D\x0430\x043B\x0430.";
    if (nStringID == STRID_ERRORGETFILESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x043F\x043E\x043B\x0443\x0447\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0444\x0430\x0439\x043B\x0430 \"%s\"";
    if (nStringID == STRID_ERRORSETFILESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0434\x043B\x044F\x0020\x0444\x0430\x0439\x043B\x0430 \"%s\"";
  }
  else
  {
    if (nStringID == STRID_ERRORWIN)
      return L"Designed for NT systems.";
    if (nStringID == STRID_ERRORDIR)
      return L"AkelAdmin should be in AkelFiles folder.";
    if (nStringID == STRID_ERRORCALL)
      return L"This program is internal for AkelPad. Don't call it directly.";
    if (nStringID == STRID_ERRORSETPIPESECURITY)
      return L"Can't set security options for pipe.";
    if (nStringID == STRID_ERRORGETFILESECURITY)
      return L"Can't get security options of file \"%s\"";
    if (nStringID == STRID_ERRORSETFILESECURITY)
      return L"Can't set security options for file \"%s\"";
  }
  return L"";
}
