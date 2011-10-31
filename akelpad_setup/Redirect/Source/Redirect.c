#define WIN32_LEAN_AND_MEAN
#define WINBASE_DECLARE_GET_SYSTEM_WOW64_DIRECTORY
#include <windows.h>
#include <commdlg.h>
#include "StrFunc.h"
#include "Resources\resource.h"


//Include string functions
#define xmemset
#include "StrFunc.h"

//Defines
#define APP_NAMEA  "AkelPad redirector"
#define APP_NAMEW L"AkelPad redirector"

//Functions prototypes
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();

//Global variables
char szCmdLine[32768];
wchar_t wszCmdLine[32768];
BOOL bOldWindows;

//GCC
int main()
{
  return 0;
}

//Entry point
void _WinMain()
{
  if (GetWindowsDirectoryW(NULL, 0))
    bOldWindows=FALSE;
  else
    bOldWindows=TRUE;

  if (bOldWindows)
  {
    STARTUPINFOA siA;
    PROCESS_INFORMATION pi;
    char szModule[MAX_PATH];
    char szPath[MAX_PATH];
    char *pCmdLineParams;
    HKEY hKey;
    DWORD dwType;
    DWORD dwSize;
    DWORD dwResult;

    if ((dwResult=RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Akelsoft\\AkelPad", 0, KEY_READ, &hKey)) == ERROR_SUCCESS)
    {
      dwSize=MAX_PATH;
      dwResult=RegQueryValueExA(hKey, "Path", NULL, &dwType, (LPBYTE)szPath, &dwSize);
      RegCloseKey(hKey);
    }
    if (dwResult != ERROR_SUCCESS)
    {
      //GetSystemWow64DirectoryA(szCmdLine, MAX_PATH);
      GetSystemDirectoryA(szCmdLine, MAX_PATH);
      wsprintfA(szPath, "%s\\notepad.exe", szCmdLine);
    }
    GetModuleFileNameA(NULL, szModule, MAX_PATH);

    if (!lstrcmpiA(szModule, szPath))
    {
      OPENFILENAMEA efnA;

      szPath[0]='\0';
      xmemset(&efnA, 0, sizeof(OPENFILENAMEA));
      efnA.lStructSize      =sizeof(OPENFILENAMEA);
      efnA.hwndOwner        =NULL;
      efnA.lpstrFile        =szPath;
      efnA.nMaxFile         =MAX_PATH;
      efnA.lpstrFilter      ="AkelPad.exe\0AkelPad.exe\0*.*\0*.*\0\0";
      efnA.nFilterIndex     =1;
      efnA.Flags            =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

      if (GetOpenFileNameA(&efnA))
      {
        if ((dwResult=RegCreateKeyExA(HKEY_LOCAL_MACHINE, "Software\\Akelsoft\\AkelPad", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)) == ERROR_SUCCESS)
        {
          dwResult=RegSetValueExA(hKey, "Path", 0, REG_SZ, (LPBYTE)szPath, lstrlenA(szPath) + 1);
          RegCloseKey(hKey);
        }
        if (dwResult != ERROR_SUCCESS)
        {
          MessageBox(NULL, "Can't write: \"HKEY_LOCAL_MACHINE\\Software\\Akelsoft\\AkelPad\"", APP_NAMEA, MB_ICONERROR);
          goto Quit;
        }
      }
      else goto Quit;
    }

    wsprintfA(szCmdLine, "\"%s\" ", szPath);
    pCmdLineParams=GetCommandLineParamsA();
    lstrcatA(szCmdLine, pCmdLineParams);

    xmemset(&siA, 0, sizeof(STARTUPINFOA));
    siA.cb=sizeof(STARTUPINFOA);
    GetStartupInfoA(&siA);

    if (CreateProcessA(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &siA, &pi))
    {
      MsgWaitForMultipleObjects(1, &pi.hProcess, FALSE, INFINITE, 0);
      CloseHandle(pi.hThread);
      CloseHandle(pi.hProcess);
    }
  }
  else
  {
    STARTUPINFOW siW;
    PROCESS_INFORMATION pi;
    wchar_t wszModule[MAX_PATH];
    wchar_t wszPath[MAX_PATH];
    wchar_t *wpCmdLineParams;
    HKEY hKey;
    DWORD dwType;
    DWORD dwSize;
    DWORD dwResult;

    if ((dwResult=RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Akelsoft\\AkelPad", 0, KEY_READ, &hKey)) == ERROR_SUCCESS)
    {
      dwSize=MAX_PATH * sizeof(wchar_t);
      dwResult=RegQueryValueExW(hKey, L"Path", NULL, &dwType, (LPBYTE)wszPath, &dwSize);
      RegCloseKey(hKey);
    }
    if (dwResult != ERROR_SUCCESS)
    {
      //GetSystemWow64DirectoryW(wszCmdLine, MAX_PATH);
      GetSystemDirectoryW(wszCmdLine, MAX_PATH);
      wsprintfW(wszPath, L"%s\\notepad.exe", wszCmdLine);
    }
    GetModuleFileNameW(NULL, wszModule, MAX_PATH);

    if (!lstrcmpiW(wszModule, wszPath))
    {
      OPENFILENAMEW efnW;

      wszPath[0]='\0';
      xmemset(&efnW, 0, sizeof(OPENFILENAMEW));
      efnW.lStructSize      =sizeof(OPENFILENAMEW);
      efnW.hwndOwner        =NULL;
      efnW.lpstrFile        =wszPath;
      efnW.nMaxFile         =MAX_PATH;
      efnW.lpstrFilter      =L"AkelPad.exe\0AkelPad.exe\0*.*\0*.*\0\0";
      efnW.nFilterIndex     =1;
      efnW.Flags            =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

      if (GetOpenFileNameW(&efnW))
      {
        if ((dwResult=RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Akelsoft\\AkelPad", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL)) == ERROR_SUCCESS)
        {
          dwResult=RegSetValueExW(hKey, L"Path", 0, REG_SZ, (LPBYTE)wszPath, lstrlenW(wszPath) * sizeof(wchar_t) + 2);
          RegCloseKey(hKey);
        }
        if (dwResult != ERROR_SUCCESS)
        {
          MessageBoxW(NULL, L"Can't write: \"HKEY_LOCAL_MACHINE\\Software\\Akelsoft\\AkelPad\"", APP_NAMEW, MB_ICONERROR);
          goto Quit;
        }
      }
      else goto Quit;
    }

    wsprintfW(wszCmdLine, L"\"%s\" ", wszPath);
    wpCmdLineParams=GetCommandLineParamsW();
    lstrcatW(wszCmdLine, wpCmdLineParams);

    xmemset(&siW, 0, sizeof(STARTUPINFOA));
    siW.cb=sizeof(STARTUPINFOW);
    GetStartupInfoW(&siW);

    if (CreateProcessW(NULL, wszCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &siW, &pi))
    {
      MsgWaitForMultipleObjects(1, &pi.hProcess, FALSE, INFINITE, 0);
      CloseHandle(pi.hThread);
      CloseHandle(pi.hProcess);
    }
  }

  Quit:
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
