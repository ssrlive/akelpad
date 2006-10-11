#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int GetExeDirA(HINSTANCE hInstance, char *szExeDir, int nLen);
int GetExeDirW(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen);
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();

HINSTANCE hInstance;
BOOL bOldWindows;
char szCmdLine[32768];
wchar_t wszCmdLine[32768];

void _WinMain()
{
  if (GetWindowsDirectoryW(NULL, 0))
    bOldWindows=FALSE;
  else
    bOldWindows=TRUE;

  if (bOldWindows)
  {
    STARTUPINFO si={0};
    PROCESS_INFORMATION pi={0};
    char szExeDir[MAX_PATH];
    char szSystemDir[MAX_PATH];
    char *pCmdLineParams;

    hInstance=GetModuleHandle(NULL);
    GetExeDirA(hInstance, szExeDir, MAX_PATH);
    GetSystemDirectory(szSystemDir, MAX_PATH);

    if (lstrcmpi(szExeDir, szSystemDir))
    {
      si.cb=sizeof(STARTUPINFO);
      GetStartupInfo(&si);

      wsprintf(szCmdLine, "\"%s\\notepad.exe\" ", szSystemDir);
      pCmdLineParams=GetCommandLineParamsA();
      lstrcat(szCmdLine, pCmdLineParams);

      if (CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
      {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      }
    }
  }
  else
  {
    STARTUPINFOW siW={0};
    PROCESS_INFORMATION pi={0};
    wchar_t wszExeDir[MAX_PATH];
    wchar_t wszSystemDir[MAX_PATH];
    wchar_t *wpCmdLineParams;

    hInstance=GetModuleHandle(NULL);
    GetExeDirW(hInstance, wszExeDir, MAX_PATH);
    GetSystemDirectoryW(wszSystemDir, MAX_PATH);

    if (lstrcmpiW(wszExeDir, wszSystemDir))
    {
      siW.cb=sizeof(STARTUPINFOW);
      GetStartupInfoW(&siW);

      wsprintfW(wszCmdLine, L"\"%s\\notepad.exe\" ", wszSystemDir);
      wpCmdLineParams=GetCommandLineParamsW();
      lstrcatW(wszCmdLine, wpCmdLineParams);

      if (CreateProcessW(NULL, wszCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &siW, &pi))
      {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      }
    }
  }

  ExitProcess(0);
}

int GetExeDirA(HINSTANCE hInstance, char *szExeDir, int nLen)
{
  nLen=GetModuleFileName(hInstance, szExeDir, nLen);
  while (nLen > 0 && szExeDir[nLen] != '\\') --nLen;
  szExeDir[nLen]='\0';

  return nLen;
}

int GetExeDirW(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen)
{
  nLen=GetModuleFileNameW(hInstance, wszExeDir, nLen);
  while (nLen > 0 && wszExeDir[nLen] != '\\') --nLen;
  wszExeDir[nLen]='\0';

  return nLen;
}

char* GetCommandLineParamsA()
{
  char *pCmdLine=GetCommandLine();

  if (*pCmdLine++ == '\"')
    while (*pCmdLine != '\"' && *pCmdLine != '\0') ++pCmdLine;
  else
    while (*pCmdLine != ' ' && *pCmdLine != '\0') ++pCmdLine;
  if (*pCmdLine != '\0')
    while (*++pCmdLine == ' ');

  return pCmdLine;
}

wchar_t* GetCommandLineParamsW()
{
  wchar_t *wpCmdLine=GetCommandLineW();

  if (*wpCmdLine++ == '\"')
    while (*wpCmdLine != '\"' && *wpCmdLine != '\0') ++wpCmdLine;
  else
    while (*wpCmdLine != ' ' && *wpCmdLine != '\0') ++wpCmdLine;
  if (*wpCmdLine != '\0')
    while (*++wpCmdLine == ' ');

  return wpCmdLine;
}
