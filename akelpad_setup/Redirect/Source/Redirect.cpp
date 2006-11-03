#define WIN32_LEAN_AND_MEAN
#include <windows.h>

char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();

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
    char szSystemDir[MAX_PATH];
    char *pCmdLineParams;

    GetSystemDirectory(szSystemDir, MAX_PATH);
    wsprintf(szCmdLine, "\"%s\\notepad.exe\" ", szSystemDir);
    pCmdLineParams=GetCommandLineParamsA();
    lstrcat(szCmdLine, pCmdLineParams);

    si.cb=sizeof(STARTUPINFO);
    GetStartupInfo(&si);

    if (CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
      MsgWaitForMultipleObjects(1, &pi.hProcess, FALSE, INFINITE, 0);
      CloseHandle(pi.hThread);
      CloseHandle(pi.hProcess);
    }
  }
  else
  {
    STARTUPINFOW siW={0};
    PROCESS_INFORMATION pi={0};
    wchar_t wszSystemDir[MAX_PATH];
    wchar_t *wpCmdLineParams;

    GetSystemDirectoryW(wszSystemDir, MAX_PATH);
    wsprintfW(wszCmdLine, L"\"%s\\notepad.exe\" ", wszSystemDir);
    wpCmdLineParams=GetCommandLineParamsW();
    lstrcatW(wszCmdLine, wpCmdLineParams);

    siW.cb=sizeof(STARTUPINFOW);
    GetStartupInfoW(&siW);

    if (CreateProcessW(NULL, wszCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &siW, &pi))
    {
      MsgWaitForMultipleObjects(1, &pi.hProcess, FALSE, INFINITE, 0);
      CloseHandle(pi.hThread);
      CloseHandle(pi.hProcess);
    }
  }

  ExitProcess(0);
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
