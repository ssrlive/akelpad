#define WIN32_LEAN_AND_MEAN
#include <windows.h>

char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();

char szCmdLine[32768];
wchar_t wszCmdLine[32768];
BOOL bOldWindows;

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

    si.cb=sizeof(STARTUPINFO);
    GetStartupInfo(&si);

    GetSystemDirectory(szSystemDir, MAX_PATH);
    wsprintf(szCmdLine, "\"%s\\notepad.exe\" ", szSystemDir);
    pCmdLineParams=GetCommandLineParamsA();
    lstrcat(szCmdLine, pCmdLineParams);

    if (CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }
  }
  else
  {
    STARTUPINFOW siW={0};
    PROCESS_INFORMATION pi={0};
    wchar_t wszSystemDir[MAX_PATH];
    wchar_t *wpCmdLineParams;

    siW.cb=sizeof(STARTUPINFOW);
    GetStartupInfoW(&siW);

    GetSystemDirectoryW(wszSystemDir, MAX_PATH);
    wsprintfW(wszCmdLine, L"\"%s\\notepad.exe\" ", wszSystemDir);
    wpCmdLineParams=GetCommandLineParamsW();
    lstrcatW(wszCmdLine, wpCmdLineParams);

    if (CreateProcessW(NULL, wszCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &siW, &pi))
    {
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }
  }

  ExitProcess(0);
}

char* GetCommandLineParamsA()
{
  char *lpCmdLine=GetCommandLine();

  if (*lpCmdLine++ == '\"')
    while (*lpCmdLine != '\"' && *lpCmdLine != '\0') ++lpCmdLine;
  else
    while (*lpCmdLine != ' ' && *lpCmdLine != '\0') ++lpCmdLine;
  if (*lpCmdLine != '\0')
    while (*++lpCmdLine == ' ');

  return lpCmdLine;
}

wchar_t* GetCommandLineParamsW()
{
  wchar_t *lpwCmdLine=GetCommandLineW();

  if (*lpwCmdLine++ == '\"')
    while (*lpwCmdLine != '\"' && *lpwCmdLine != '\0') ++lpwCmdLine;
  else
    while (*lpwCmdLine != ' ' && *lpwCmdLine != '\0') ++lpwCmdLine;
  if (*lpwCmdLine != '\0')
    while (*++lpwCmdLine == ' ');

  return lpwCmdLine;
}
