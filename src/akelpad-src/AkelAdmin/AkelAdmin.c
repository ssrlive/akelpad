/***********************************************************************************
 *                      AkelAdmin UAC support for AkelPad                          *
 *                                                                                 *
 * Copyright 2015 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)       *
 *                                                                                 *
 * License: this source is distributed under "BSD license" conditions.             *
 ***********************************************************************************/


#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <windows.h>
#include <tchar.h>
#include <aclapi.h>
#include "..\AkelEdit\StrFunc.h"

/*
//Include string functions
#define xstrlenW
#define xatoiW
#include "..\AkelEdit\StrFunc.h"
//*/

//Defines
#define STRID_ERRORWIN              1
#define STRID_ERRORDIR              2
#define STRID_ERRORCALL             3
#define STRID_ERRORCREATEFILE       4
#define STRID_ERRORSETPIPESECURITY  5
#define STRID_ERRORGETFILESECURITY  6
#define STRID_ERRORSETFILESECURITY  7
#define STRID_ERRORGETPRIVILEGE     8

#define BUFFER_SIZE      1024

//AkelAdmin action
#define AAA_CMDINIT             11
#define AAA_EXIT                20  //Exit from pipe server.
#define AAA_SECURITYGET         21  //Retrieve file security.
#define AAA_SECURITYSETEVERYONE 22  //Add all access for the file.
#define AAA_SECURITYRESTORE     23  //Restore saved security for the file.
#define AAA_SECURITYFREE        24  //Free saved security.
#define AAA_CREATEFILE          25  //Create new file.

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
#ifndef SDDL_REVISION_1
  #define SDDL_REVISION_1 1
#endif
#ifndef LOW_INTEGRITY_SDDL_SACL_W
  #define LOW_INTEGRITY_SDDL_SACL_W L"S:(ML;;NW;;;LW)"
#endif
#ifndef MEDIUM_INTEGRITY_SDDL_SACL_W
  #define MEDIUM_INTEGRITY_SDDL_SACL_W L"S:(ML;;NW;;;ME)"
#endif

typedef struct {
  DWORD dwExitCode;
  int nAction;
  wchar_t wszFile[MAX_PATH];
  DWORD dwLangModule;
} ADMINPIPE;


//Functions prototypes
DWORD WINAPI ThreadProc(LPVOID lpParameter);
BOOL EnablePrivilege(const wchar_t *wpName, BOOL bEnable);
int GetExeDir(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszAkelAdminPipe[32];
HINSTANCE hInstance;
HANDLE hThread=NULL;
DWORD dwThreadId;
DWORD dwExitCode=1;
DWORD dwInitProcessId;
HWND hPipeAkelAdmin;
LANGID wLangModule=0;

//GetProcAddress
BOOL (WINAPI *GetNamedPipeClientProcessIdPtr)(HANDLE, ULONG *);
DWORD (WINAPI *SetNamedSecurityInfoWPtr)(wchar_t *, SE_OBJECT_TYPE, SECURITY_INFORMATION, PSID, PSID, PACL, PACL);
DWORD (WINAPI *SetSecurityInfoPtr)(HANDLE, SE_OBJECT_TYPE, SECURITY_INFORMATION, PSID, PSID, PACL, PACL);
DWORD (WINAPI *SetEntriesInAclWPtr)(ULONG, PEXPLICIT_ACCESSW, PACL, PACL *);
BOOL (WINAPI *ConvertStringSecurityDescriptorToSecurityDescriptorWPtr)(wchar_t *, DWORD, SECURITY_DESCRIPTOR **, ULONG *);
BOOL (WINAPI *GetSecurityDescriptorSaclPtr)(SECURITY_DESCRIPTOR *, BOOL *, PACL *, BOOL *);


//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

//Entry point
int CALLBACK _tWinMain(HINSTANCE hApp, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
  wchar_t *wpCmdLine=GetCommandLineW();
  const wchar_t *pArguments=wpCmdLine;
  int nAction=0;
  int nExeDirLen;

  //Get program HINSTANCE
  hInstance=GetModuleHandleW(NULL);

  //Get default language ID
  wLangModule=PRIMARYLANGID(GetUserDefaultLangID());

  if (hInstance)
  {
    //Get program directory
    nExeDirLen=GetExeDir(hInstance, wszBuffer, BUFFER_SIZE);

    if (!lstrcmpiW(L"AkelFiles", GetFileName(wszBuffer, nExeDirLen)))
    {
      //Skip executable
      GetCommandLineArg(pArguments, NULL, 0, &pArguments);

      //First argument is action number (currently only one action).
      if (GetCommandLineArg(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
      {
        nAction=(int)xatoiW(wszBuffer, NULL);

        //Initialize pipe server
        if (nAction == AAA_CMDINIT)
        {
          //Second argument is caller process id.
          if (GetCommandLineArg(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
          {
            dwInitProcessId=(DWORD)xatoiW(wszBuffer, NULL);

            if (hThread=CreateThread(NULL, 0, ThreadProc, NULL, 0, &dwThreadId))
            {
              //Wait for hThread and process messages to avoid busy cursor.
              MSG msg;

              while (MsgWaitForMultipleObjects(1, &hThread, FALSE, INFINITE, QS_ALLINPUT) != WAIT_OBJECT_0)
              {
                while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
                {
                  TranslateMessage(&msg);
                  DispatchMessageW(&msg);
                }
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

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
  HANDLE hPipeAkelAdmin;
  HANDLE hMutex;
  HMODULE hKernel32;
  HMODULE hAdvApi32;
  DWORD dwClientProcessId;

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
    ConvertStringSecurityDescriptorToSecurityDescriptorWPtr=(BOOL (WINAPI *)(wchar_t *, DWORD, SECURITY_DESCRIPTOR **, ULONG *))GetProcAddress(hAdvApi32, "ConvertStringSecurityDescriptorToSecurityDescriptorW");
    GetSecurityDescriptorSaclPtr=(BOOL (WINAPI *)(SECURITY_DESCRIPTOR *, BOOL *, PACL *, BOOL *))GetProcAddress(hAdvApi32, "GetSecurityDescriptorSacl");

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

        //If a client connects before the ConnectNamedPipe is called, the function returns zero and GetLastError returns ERROR_PIPE_CONNECTED
        if (ConnectNamedPipe(hPipeAkelAdmin, NULL) || GetLastError() == ERROR_PIPE_CONNECTED)
        {
          //Wait for client WriteFile
          if (ReadFile(hPipeAkelAdmin, &apipe, sizeof(ADMINPIPE), &dwBytesRead, NULL))
          {
            apipe.dwExitCode=1;

            //Accept connection only from initial caller that runs this process
            if (!GetNamedPipeClientProcessIdPtr || (GetNamedPipeClientProcessIdPtr(hPipeAkelAdmin, &dwClientProcessId) && dwClientProcessId == dwInitProcessId))
            {
              if (apipe.nAction == AAA_EXIT)
              {
                //Unload process
                bBreak=TRUE;
                apipe.dwExitCode=0;
              }
              else if (apipe.nAction == AAA_CREATEFILE)
              {
                HANDLE hFile;

                wLangModule=PRIMARYLANGID(apipe.dwLangModule);

                //Create new file
                if ((hFile=CreateFileW(apipe.wszFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
                {
                  CloseHandle(hFile);
                  apipe.dwExitCode=0;
                }
                else
                {
                  wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORCREATEFILE), apipe.wszFile);
                  MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                }
              }
              else if (apipe.nAction == AAA_SECURITYGET)
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
                      if (GetFileSecurityW(apipe.wszFile, ssi, psdCurrentFile, dwSize, &dwSize))
                      {
                        lstrcpynW(wszCurrentFile, apipe.wszFile, MAX_PATH);
                        apipe.dwExitCode=0;
                      }
                      else
                      {
                        wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORGETFILESECURITY), apipe.wszFile);
                        MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                      }
                    }
                  }

                  if (apipe.dwExitCode)
                  {
                    GlobalFree(psdCurrentFile);
                    psdCurrentFile=NULL;
                  }
                }
              }
              else if (apipe.nAction == AAA_SECURITYSETEVERYONE)
              {
                wLangModule=PRIMARYLANGID(apipe.dwLangModule);

                //Decrease file security
                if (psdCurrentFile)
                {
                  SECURITY_DESCRIPTOR *psdLowIntegrity=NULL;
                  PACL pSacl=NULL;
                  BOOL bSaclPresent=FALSE;
                  BOOL bSaclDefaulted=FALSE;

                  //Get medium integrity to allow write in file on disk root.
                  if (ConvertStringSecurityDescriptorToSecurityDescriptorWPtr(MEDIUM_INTEGRITY_SDDL_SACL_W, SDDL_REVISION_1, &psdLowIntegrity, NULL))
                    GetSecurityDescriptorSaclPtr(psdLowIntegrity, &bSaclPresent, &pSacl, &bSaclDefaulted);

                  if (SetNamedSecurityInfoWPtr(apipe.wszFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION|LABEL_SECURITY_INFORMATION, NULL, NULL, pEveryoneACL, pSacl) == ERROR_SUCCESS)
                  {
                    apipe.dwExitCode=0;
                  }
                  else
                  {
                    wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORSETFILESECURITY), apipe.wszFile);
                    MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                  }
                  if (psdLowIntegrity)
                    LocalFree(psdLowIntegrity);
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
                  if (EnablePrivilege(SE_RESTORE_NAME, TRUE))
                  {
                   if (psdCurrentFile->Control & SE_DACL_AUTO_INHERITED)
                     psdCurrentFile->Control|=SE_DACL_AUTO_INHERIT_REQ;
                   if (psdCurrentFile->Control & SE_SACL_AUTO_INHERITED)
                     psdCurrentFile->Control|=SE_SACL_AUTO_INHERIT_REQ;

                    if (SetFileSecurityW(wszCurrentFile, ssi, psdCurrentFile))
                    {
                      apipe.dwExitCode=0;
                    }
                    else
                    {
                      wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORSETFILESECURITY), wszCurrentFile);
                      MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                    }
                    EnablePrivilege(SE_RESTORE_NAME, FALSE);
                  }
                  else
                  {
                    wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORGETPRIVILEGE), wszCurrentFile);
                    MessageBoxW(NULL, wszBuffer, STR_AKELADMIN, MB_ICONERROR);
                  }
                }
              }
              else if (apipe.nAction == AAA_SECURITYFREE)
              {
                //Free security buffer
                if (psdCurrentFile)
                {
                  GlobalFree(psdCurrentFile);
                  psdCurrentFile=NULL;
                  apipe.dwExitCode=0;
                }
              }
            }
            WriteFile(hPipeAkelAdmin, &apipe, sizeof(ADMINPIPE), &dwBytesWritten, NULL);
            FlushFileBuffers(hPipeAkelAdmin);
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

  if (hThread)
  {
    CloseHandle(hThread);
    hThread=NULL;
  }
  return 0;
}

BOOL EnablePrivilege(const wchar_t *wpName, BOOL bEnable)
{
  TOKEN_PRIVILEGES tp;
  HANDLE hToken;
  BOOL bResult=FALSE;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
  {
    if (LookupPrivilegeValueW(NULL, wpName, &tp.Privileges[0].Luid))
    {
      tp.PrivilegeCount=1;
      tp.Privileges[0].Attributes=(bEnable?SE_PRIVILEGE_ENABLED:0);

      if (AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
        bResult=TRUE;
    }
    CloseHandle(hToken);
  }
  return bResult;
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

const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
      return wpCount + 1;
  }
  return wpFile;
}

int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg)
{
  const wchar_t *wpCount=wpCmdLine;
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
    if (nStringID == STRID_ERRORCREATEFILE)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0441\x043E\x0437\x0434\x0430\x0442\x044C\x0020\x0444\x0430\x0439\x043B \"%s\"";
    if (nStringID == STRID_ERRORSETPIPESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0434\x043B\x044F\x0020\x043A\x0430\x043D\x0430\x043B\x0430.";
    if (nStringID == STRID_ERRORGETFILESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x043F\x043E\x043B\x0443\x0447\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0444\x0430\x0439\x043B\x0430 \"%s\"";
    if (nStringID == STRID_ERRORSETFILESECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0434\x043B\x044F\x0020\x0444\x0430\x0439\x043B\x0430 \"%s\"";
    if (nStringID == STRID_ERRORGETPRIVILEGE)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x043F\x043E\x043B\x0443\x0447\x0438\x0442\x044C\x0020\x043F\x0440\x0438\x0432\x0438\x043B\x0435\x0433\x0438\x0438\x0020\x0434\x043B\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x043A\x0438\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0435\x043A\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0444\x0430\x0439\x043B\x0430 \"%s\"";
  }
  else
  {
    if (nStringID == STRID_ERRORWIN)
      return L"Designed for NT systems.";
    if (nStringID == STRID_ERRORDIR)
      return L"AkelAdmin should be in AkelFiles folder.";
    if (nStringID == STRID_ERRORCALL)
      return L"This program is internal for AkelPad. Don't call it directly.";
    if (nStringID == STRID_ERRORCREATEFILE)
      return L"Can't create file \"%s\"";
    if (nStringID == STRID_ERRORSETPIPESECURITY)
      return L"Can't set security options for pipe.";
    if (nStringID == STRID_ERRORGETFILESECURITY)
      return L"Can't get security options of file \"%s\"";
    if (nStringID == STRID_ERRORSETFILESECURITY)
      return L"Can't set security options for file \"%s\"";
    if (nStringID == STRID_ERRORGETPRIVILEGE)
      return L"Can't get privilege to set security options of file \"%s\"";
  }
  return L"";
}
