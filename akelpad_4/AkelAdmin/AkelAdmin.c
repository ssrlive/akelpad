/***********************************************************************************
 *                      AkelAdmin UAC support for AkelPad                          *
 *                                                                                 *
 * Copyright 2011 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)       *
 *                                                                                 *
 * License: this source is distributed under "BSD license" conditions.             *
 ***********************************************************************************/


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Aclapi.h>
#include "..\AkelEdit\StrFunc.h"

//Include string functions
#define xmemset
#define xatoiW
#include "..\AkelEdit\StrFunc.h"


//Defines
#define STRID_ERRORDIR           1
#define STRID_ERRORCALL          2
#define STRID_ERRORFILESECURITY  3
#define STRID_ERRORPIPESECURITY  4
#define STRID_ERRORDELETE        5
#define STRID_ERRORMOVE          6

#define BUFFER_SIZE      1024

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

#define STR_AKELADMIN L"AkelAdmin"

typedef struct {
  int nAction;
  DWORD dwExitCode;
  wchar_t wszSourceFile[MAX_PATH];
  wchar_t wszTargetFile[MAX_PATH];
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

  if (hInstance)
  {
    //Get program directory
    GetExeDir(hInstance, wszBuffer, BUFFER_SIZE);

    //Get default language ID
    wLangModule=PRIMARYLANGID(GetUserDefaultLangID());

    if (!lstrcmpiW(L"AkelFiles", GetFileName(wszBuffer)))
    {
      //Skip executable
      GetCommandLineArgW(pArguments, NULL, 0, &pArguments);

      //First argument is action number (currently only one action).
      if (GetCommandLineArgW(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
      {
        nAction=(int)xatoiW(wszBuffer, NULL);

        //Initialize pipe server
        if (nAction == 10)
        {
          //Second argument is caller process id.
          if (GetCommandLineArgW(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
          {
            HANDLE hPipeAkelAdmin;
            HANDLE hMutex;
            HANDLE hKernel32;
            DWORD dwInitProcessId;
            DWORD dwClientProcessId;

            dwInitProcessId=(DWORD)xatoiW(wszBuffer, NULL);

            if (hMutex=OpenEventW(EVENT_MODIFY_STATE, FALSE, STR_AKELADMIN))
            {
              wsprintfW(wszAkelAdminPipe, L"\\\\.\\pipe\\%s-%d", STR_AKELADMIN, dwInitProcessId);

              //Get functions addresses
              hKernel32=GetModuleHandleW(L"kernel32.dll");
              GetNamedPipeClientProcessIdPtr=(BOOL (WINAPI *)(HANDLE, ULONG *))GetProcAddress(hKernel32, "GetNamedPipeClientProcessId");

              if ((hPipeAkelAdmin=CreateNamedPipeW(wszAkelAdminPipe, PIPE_ACCESS_DUPLEX|PIPE_WAIT|WRITE_DAC, 0, 1, sizeof(ADMINPIPE), sizeof(ADMINPIPE), 0, NULL)) != INVALID_HANDLE_VALUE)
              {
                ACL *pNewACL=NULL;
                EXPLICIT_ACCESSW eal[1];
                SID_IDENTIFIER_AUTHORITY SIDAuthWorld=SECURITY_WORLD_SID_AUTHORITY;
                SID *pSIDEveryone=NULL;
                BOOL bChangeAccessResult=FALSE;

                // Specify the DACL to use. Create a SID for the Everyone group.
                if (AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, (void **)&pSIDEveryone))
                {
                  xmemset(&eal[0], 0, sizeof(EXPLICIT_ACCESS));
                  eal[0].grfAccessPermissions=GENERIC_ALL;
                  eal[0].grfAccessMode=SET_ACCESS;
                  eal[0].grfInheritance=NO_INHERITANCE;
                  eal[0].Trustee.TrusteeForm=TRUSTEE_IS_SID;
                  eal[0].Trustee.TrusteeType=TRUSTEE_IS_WELL_KNOWN_GROUP;
                  eal[0].Trustee.ptstrName=(wchar_t *)pSIDEveryone;
                  eal[0].Trustee.MultipleTrusteeOperation=NO_MULTIPLE_TRUSTEE;
                  eal[0].Trustee.pMultipleTrustee=NULL;

                  if (SetEntriesInAclW(1, eal, NULL, &pNewACL) == ERROR_SUCCESS)
                  {
                    if (SetSecurityInfo(hPipeAkelAdmin, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewACL, NULL) == ERROR_SUCCESS)
                      bChangeAccessResult=TRUE;
                    LocalFree(pNewACL);
                  }
                  FreeSid(pSIDEveryone);
                }
                if (!bChangeAccessResult)
                  MessageBoxW(NULL, GetLangStringW(wLangModule, STRID_ERRORPIPESECURITY), STR_AKELADMIN, MB_ICONERROR);

                //Signal mutex
                SetEvent(hMutex);

                //Wait for client connect with CreateFile
                while (ConnectNamedPipe(hPipeAkelAdmin, NULL))
                {
                  ADMINPIPE apipe;
                  DWORD dwSourceAttr;
                  DWORD dwTargetAttr;
                  DWORD dwBytesRead;
                  DWORD dwBytesWritten;
                  BOOL bBreak=FALSE;

                  //Accept connection only from initial caller that runs this process
                  if (!GetNamedPipeClientProcessIdPtr || ((*GetNamedPipeClientProcessIdPtr)(hPipeAkelAdmin, &dwClientProcessId) && dwClientProcessId == dwInitProcessId))
                  {
                    //Wait for client WriteFile
                    if (ReadFile(hPipeAkelAdmin, &apipe, sizeof(apipe), &dwBytesRead, NULL))
                    {
                      if (apipe.nAction == 0)
                      {
                        //Unload process
                        apipe.dwExitCode=0;
                        bBreak=TRUE;
                      }
                      else if (apipe.nAction == 1)
                      {
                        //Move file action
                        apipe.dwExitCode=1;
                        wLangModule=PRIMARYLANGID(apipe.dwLangModule);

                        if ((dwSourceAttr=GetFileAttributesW(apipe.wszSourceFile)) != INVALID_FILE_ATTRIBUTES &&
                            (dwTargetAttr=GetFileAttributesW(apipe.wszTargetFile)) != INVALID_FILE_ATTRIBUTES)
                        {
                          SECURITY_INFORMATION ssi=DACL_SECURITY_INFORMATION|
                                                   GROUP_SECURITY_INFORMATION|
                                                   LABEL_SECURITY_INFORMATION|
                                                   OWNER_SECURITY_INFORMATION;
                          SECURITY_DESCRIPTOR *psd=NULL;
                          DWORD dwSize=0;

                          //Retrive file security
                          GetFileSecurityW(apipe.wszTargetFile, ssi, NULL, 0, &dwSize);

                          if (dwSize)
                          {
                            if (psd=(SECURITY_DESCRIPTOR *)GlobalAlloc(GMEM_FIXED, dwSize))
                            {
                              if (!GetFileSecurityW(apipe.wszTargetFile, ssi, psd, dwSize, &dwSize))
                              {
                                GlobalFree(psd);
                                psd=NULL;
                              }
                            }
                          }

                          //Replace protected file
                          if (DeleteFileW(apipe.wszTargetFile))
                          {
                            if (MoveFileW(apipe.wszSourceFile, apipe.wszTargetFile))
                            {
                              //Copy attributes and security
                              SetFileAttributesW(apipe.wszTargetFile, dwTargetAttr);

                              if (psd)
                              {
                                if (!SetFileSecurityW(apipe.wszTargetFile, ssi, psd))
                                {
                                  wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORFILESECURITY), apipe.wszTargetFile);
                                  MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                                }
                              }
                              apipe.dwExitCode=0;
                            }
                            else
                            {
                              wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORMOVE), apipe.wszSourceFile, apipe.wszTargetFile);
                              MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                            }
                          }
                          else
                          {
                            wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORDELETE), apipe.wszTargetFile);
                            MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                          }

                          //Free security buffer
                          if (psd)
                          {
                            GlobalFree(psd);
                            psd=NULL;
                          }
                        }
                      }
                      WriteFile(hPipeAkelAdmin, &apipe, sizeof(apipe), &dwBytesWritten, NULL);
                      FlushFileBuffers(hPipeAkelAdmin);
                    }
                  }
                  DisconnectNamedPipe(hPipeAkelAdmin);
                  if (bBreak) break;
                }
                dwExitCode=0;
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
    if (nStringID == STRID_ERRORDIR)
      return L"AkelAdmin \x0434\x043E\x043B\x0436\x0435\x043D\x0020\x043D\x0430\x0445\x043E\x0434\x0438\x0442\x044C\x0441\x044F\x0020\x0432\x0020\x043F\x0430\x043F\x043A\x0435 AkelFiles.";
    if (nStringID == STRID_ERRORCALL)
      return L"\x0414\x0430\x043D\x043D\x0430\x044F\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x0430\x0020\x043F\x0440\x0435\x0434\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x0430\x0020\x0434\x043B\x044F\x0020\x0432\x043D\x0443\x0442\x0440\x0435\x043D\x043D\x0435\x0433\x043E\x0020\x0438\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x043D\x0438\x044F AkelPad'\x043E\x043C. \x041D\x0435\x0020\x0432\x044B\x0437\x044B\x0432\x0430\x0439\x0442\x0435\x0020\x0435\x0435\x0020\x043D\x0430\x043F\x0440\x044F\x043C\x0443\x044E.";
    if (nStringID == STRID_ERRORPIPESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0434\x043B\x044F\x0020\x043A\x0430\x043D\x0430\x043B\x0430.";
    if (nStringID == STRID_ERRORFILESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0434\x043B\x044F\x0020\x0444\x0430\x0439\x043B\x0430 \"%s\"";
    if (nStringID == STRID_ERRORDELETE)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x0444\x0430\x0439\x043B \"%s\"";
    if (nStringID == STRID_ERRORMOVE)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0444\x0430\x0439\x043B.\n\x0418\x0437:\"%s\"\n\x0432: \"%s\"";
  }
  else
  {
    if (nStringID == STRID_ERRORDIR)
      return L"AkelAdmin should be in AkelFiles folder.";
    if (nStringID == STRID_ERRORCALL)
      return L"This program is internal for AkelPad. Don't call it directly.";
    if (nStringID == STRID_ERRORPIPESECURITY)
      return L"Can't set security options for pipe.";
    if (nStringID == STRID_ERRORFILESECURITY)
      return L"Can't set security options for file \"%s\"";
    if (nStringID == STRID_ERRORDELETE)
      return L"Can't delete file \"%s\"";
    if (nStringID == STRID_ERRORMOVE)
      return L"Can't move file.\nFrom: \"%s\"\nto: \"%s\"";
  }
  return L"";
}
