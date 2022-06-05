/******************************************************************
 *                  Wide functions header v3.3                    *
 *                                                                *
 * 2018 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)   *
 *                                                                *
 *  Header provide functions that can be successfully called in   *
 *        all versions of Windows including Win95/98/Me.          *
 *                                                                *
 *                                                                *
 *  WideFunc.h header uses some functions from StrFunc.h header:  *
 *   - Basic requirement is xmemcpy.                              *
 *   - GetLongPathNameWide required xstrcpynW, xstrlenW.          *
 *   - GetOpenFileNameWide required xarrlenA and xarrlenW.        *
 *****************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlobj.h>
#include <stddef.h>
#include <TlHelp32.h>

#ifndef _WIDEFUNC_H_
#define _WIDEFUNC_H_

#define ALLWIDEFUNC 1

//Global variables
extern BOOL WideGlobal_bOldWindows;
extern BOOL WideGlobal_bWinVista;
extern BOOL WideGlobal_bWideInitialized;
extern BOOL (WINAPI *WideGlobal_GetCPInfoExAPtr)(UINT, DWORD, LPCPINFOEXA);
extern BOOL (WINAPI *WideGlobal_GetCPInfoExWPtr)(UINT, DWORD, LPCPINFOEXW);
extern DWORD (WINAPI *WideGlobal_GetLongPathNameAPtr)(const char *, char *, DWORD);
extern DWORD (WINAPI *WideGlobal_GetLongPathNameWPtr)(const wchar_t *, wchar_t *, DWORD);
extern UINT (WINAPI *WideGlobal_ExtractIconExAPtr)(const char *, int, HICON *, HICON *, UINT);
extern UINT (WINAPI *WideGlobal_ExtractIconExWPtr)(const wchar_t *, int, HICON *, HICON *, UINT);
extern BOOL (WINAPI *WideGlobal_SHGetPathFromIDListAPtr)(LPCITEMIDLIST, char *);
extern BOOL (WINAPI *WideGlobal_SHGetPathFromIDListWPtr)(LPCITEMIDLIST, wchar_t *);
extern LPITEMIDLIST (WINAPI *WideGlobal_SHBrowseForFolderAPtr)(LPBROWSEINFOA);
extern LPITEMIDLIST (WINAPI *WideGlobal_SHBrowseForFolderWPtr)(LPBROWSEINFOW);
extern int (WINAPI *WideGlobal_SHFileOperationAPtr)(LPSHFILEOPSTRUCTA);
extern int (WINAPI *WideGlobal_SHFileOperationWPtr)(LPSHFILEOPSTRUCTW);

//Common
void WideInitialize();
void WideNotInitialized();
char* AllocAnsi(const wchar_t *wpWideStr);
char* AllocAnsiLen(const wchar_t *wpWideStr, int nWideStrLen);
void FreeAnsi(char *pAnsiBuf);
wchar_t* AllocWide(const char *pAnsiStr);
wchar_t* AllocWideLen(const char *pAnsiStr, int nAnsiStrLen);
void FreeWide(wchar_t *pWideBuf);
int WideToAnsi(const wchar_t *wpWideStr, int nWideStrLen, char *szAnsiBuf, int nAnsiBufMax);
int AnsiToWide(const char *pAnsiStr, int nAnsiStrLen, wchar_t *wszWideBuf, int nWideBufMax);

//File and directories (FILEWIDEFUNC). Kernel32.lib.
BOOL CreateDirectoryWide(const wchar_t *wpDir, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
BOOL RemoveDirectoryWide(const wchar_t *wpPathName);
HANDLE CreateFileWide(const wchar_t *wpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL CopyFileWide(const wchar_t *wpExistingFileName, const wchar_t *wpNewFileName, BOOL bFailIfExists);
BOOL MoveFileWide(const wchar_t *wpExistingFileName, const wchar_t *wpNewFileName);
BOOL DeleteFileWide(const wchar_t *wpFile);
DWORD GetFileAttributesWide(const wchar_t *wpFile);
BOOL SetFileAttributesWide(const wchar_t *wpFile, DWORD dwFileAttributes);
HANDLE FindFirstFileWide(const wchar_t *wpFile, WIN32_FIND_DATAW *lpFindFileData);
BOOL FindNextFileWide(HANDLE hFindFile, WIN32_FIND_DATAW *lpFindFileData);
DWORD GetCurrentDirectoryWide(int nDirMax, wchar_t *wszDir);
BOOL SetCurrentDirectoryWide(const wchar_t *wszDir);
HMODULE LoadLibraryWide(const wchar_t *wpFileName);
HMODULE LoadLibraryExWide(const wchar_t *wpFileName, HANDLE hFile, DWORD dwFlags);
HMODULE GetModuleHandleWide(const wchar_t *wpModule);
DWORD GetModuleFileNameWide(HMODULE hModule, wchar_t *wszFileName, DWORD nSize);
DWORD GetFullPathNameWide(const wchar_t *wpPath, DWORD nBufferLength, wchar_t *wszBuffer, wchar_t **wpFilePart);
DWORD GetLongPathNameWide(const wchar_t *wpShortPath, wchar_t *wszLongPath, DWORD dwLongPathSize);
DWORD GetTempPathWide(DWORD nBufferLength, wchar_t *wszBuffer);
UINT GetTempFileNameWide(const wchar_t *wpPathName, const wchar_t *wpPrefixString, UINT uUnique, wchar_t *wszTempFileName);
UINT SearchPathWide(const wchar_t *wpPath, const wchar_t *wpFileName, const wchar_t *wpExtension, DWORD nBufferLength, wchar_t *wszBuffer, wchar_t **wpFilePart);
DWORD ExpandEnvironmentStringsWide(const wchar_t *wpSrc, wchar_t *wszDst, DWORD nSize);
BOOL CreateProcessWide(const wchar_t *wpApplicationName, wchar_t *wpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, const wchar_t *wpCurrentDirectory, STARTUPINFOW *lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
//Non-system
BOOL FileExistsAnsi(const char *pFile);
BOOL FileExistsWide(const wchar_t *wpFile);
BOOL DirExistsAnsi(const char *pDir);
BOOL DirExistsWide(const wchar_t *wpDir);

//Shell (SHELLWIDEFUNC). Shell32.lib, Comdlg32.lib and Comctl32.lib.
HINSTANCE ShellExecuteWide(HWND hwnd, const wchar_t *wpOperation, const wchar_t *wpFile, const wchar_t *wpParameters, const wchar_t *wpDirectory, INT nShowCmd);
BOOL SHGetPathFromIDListWide(LPCITEMIDLIST pidl, wchar_t *wszPath);
LPITEMIDLIST SHBrowseForFolderWide(BROWSEINFOW *lpbi);
int SHFileOperationWide(SHFILEOPSTRUCTW *lpfos);
BOOL GetOpenFileNameWide(LPOPENFILENAMEW lpofn);
BOOL GetSaveFileNameWide(LPOPENFILENAMEW lpofn);
BOOL GetOpenOrSaveFileNameWide(LPOPENFILENAMEW lpofn, BOOL bSave);
INT_PTR PropertySheetWide(const PROPSHEETHEADERW *pshW);
UINT DragQueryFileWide(HDROP hDrop, UINT iFile, wchar_t *wszFile, UINT cch);

//Registry (REGWIDEFUNC). Advapi32.lib.
LONG RegCreateKeyExWide(HKEY hKey, const wchar_t *wpSubKey, DWORD dwReserved, wchar_t *wpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
LONG RegOpenKeyExWide(HKEY hKey, const wchar_t *wpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult);
LONG RegEnumKeyExWide(HKEY hKey, DWORD dwIndex, wchar_t *wszKeyName, LPDWORD lpcKeyName, LPDWORD lpReserved, wchar_t *lpClass, LPDWORD lpcClass, PFILETIME lpftLastWriteTime);
LONG RegEnumValueWide(HKEY hKey, DWORD dwIndex, wchar_t *wszValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
LONG RegQueryValueExWide(HKEY hKey, const wchar_t *wpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
LONG RegSetValueExWide(HKEY hKey, const wchar_t *wpValueName, DWORD dwReserved, DWORD dwType, const BYTE *lpData, DWORD cbData);
LONG RegDeleteValueWide(HKEY hKey, const wchar_t *wpValue);
LONG RegDeleteKeyWide(HKEY hKey, const wchar_t *wpSubKey);
//Non-system
void RegClearKeyAnsi(HKEY hKey, char *pSubKey);
void RegClearKeyWide(HKEY hKey, wchar_t *wpSubKey);

//Information (INFOWIDEFUNC). Kernel32.lib and User32.lib.
int GetDateFormatWide(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME *lpDate, const wchar_t *wpFormat, wchar_t *wszDateStr, int cchDate);
int GetTimeFormatWide(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME *lpTime, const wchar_t *wpFormat, wchar_t *wszTimeStr, int cchTime);
BOOL GetCPInfoExWide(UINT CodePage, DWORD dwFlags, LPCPINFOEXW lpCPInfoEx);
int GetKeyNameTextWide(LONG lParam, wchar_t *wpString, int nSize);

//Process (PROCESSWIDEFUNC). Kernel32.lib.
BOOL Process32NextWide(HANDLE hSnapshot, LPPROCESSENTRY32W lppe);
BOOL Process32FirstWide(HANDLE hSnapshot, LPPROCESSENTRY32W lppe);

//Font (FONTWIDEFUNC). Gdi32.lib.
HFONT CreateFontIndirectWide(const LOGFONTW *lfFont);
int AddFontResourceWide(const wchar_t *wpFontName);
BOOL RemoveFontResourceWide(const wchar_t *wpFontName);

//Print (PRINTWIDEFUNC). Gdi32.lib.
int StartDocWide(HDC hdc, const DOCINFOW *lpdi);

//Windows (WINDOWWIDEFUNC). User32.lib.
ATOM RegisterClassWide(WNDCLASSW *lpWndClass);
BOOL UnregisterClassWide(const wchar_t *wpClassName, HINSTANCE hInstance);
HWND CreateWindowExWide(DWORD dwExStyle, const wchar_t *wpClassName, const wchar_t *wpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
HWND CreateMDIWindowWide(const wchar_t *wpClassName, const wchar_t *wpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance, LPARAM lParam);
HWND CreateDialogWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc);
HWND CreateDialogParamWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
INT_PTR DialogBoxWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR DialogBoxParamWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
int GetClassNameWide(HWND hWnd, wchar_t *wszText, int nTextMax);
UINT_PTR GetClassLongPtrWide(HWND hWnd, int nIndex);
UINT_PTR SetClassLongPtrWide(HWND hWnd, int nIndex, UINT_PTR dwNewLong);
UINT_PTR GetWindowLongPtrWide(HWND hWnd, int nIndex);
UINT_PTR SetWindowLongPtrWide(HWND hWnd, int nIndex, UINT_PTR dwNewLong);
LRESULT CallWindowProcWide(WNDPROC lpPrevWndFunc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT DefWindowProcWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT DefDlgProcWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND FindWindowExWide(HWND hWndParent, HWND hWndChildAfter, const wchar_t *wpClassName, const wchar_t *wpWindowName);
int GetWindowTextLengthWide(HWND hWnd);
int GetWindowTextWide(HWND hWnd, wchar_t *wszText, int nTextMax);
BOOL SetWindowTextWide(HWND hWnd, const wchar_t *wpText);
int GetDlgItemTextWide(HWND hDlg, int nIDDlgItem, wchar_t *wszText, int nTextMax);
BOOL SetDlgItemTextWide(HWND hDlg, int nIDDlgItem, const wchar_t *wpText);

//Messages (MESSAGEWIDEFUNC). User32.lib.
BOOL GetMessageWide(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL PeekMessageWide(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
LRESULT DispatchMessageWide(const MSG *lpMsg);
int TranslateAcceleratorWide(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg);
BOOL IsDialogMessageWide(HWND hDlg, LPMSG lpMsg);
LRESULT SendMessageWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT PostMessageWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HANDLE OpenEventWide(DWORD dwDesiredAccess, BOOL bInheritHandle, const wchar_t *wpName);
HANDLE CreateEventWide(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, const wchar_t *wpName);

//Resources (RESOURCEWIDEFUNC). User32.lib.
int LoadStringWide(HINSTANCE hInstance, UINT uID, wchar_t *wszText, int nTextMax);
HANDLE LoadImageWide(HINSTANCE hInstance, const wchar_t *wpName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);
UINT ExtractIconExWide(const wchar_t *wpFile, int nIconIndex, HICON *phiconLarge, HICON *phiconSmall, UINT nIcons);
//Non-system
HICON IconExtractWide(const wchar_t *wpFile, UINT nIconIndex, int cxDesired, int cyDesired);

//Menus (MENUWIDEFUNC). User32.lib.
int GetMenuStringWide(HMENU hMenu, UINT uIDItem, wchar_t *wszText, int nTextMax, UINT uFlag);
BOOL AppendMenuWide(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *wpNewItem);
BOOL InsertMenuWide(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *wpNewItem);
BOOL ModifyMenuWide(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *wpNewItem);

//Controls (CONTROLWIDEFUNC). User32.lib.
BOOL ListView_GetColumnWide(HWND hWnd, int iCol, LVCOLUMNW *lvcW);
int ListView_InsertColumnWide(HWND hWnd, int iCol, const LVCOLUMNW *lvcW);
BOOL ListView_SetColumnWide(HWND hWnd, int iCol, const LVCOLUMNW *lvcW);
BOOL ListView_GetItemWide(HWND hWnd, LVITEMW *lviW);
int ListView_InsertItemWide(HWND hWnd, const LVITEMW *lviW);
BOOL ListView_SetItemWide(HWND hWnd, const LVITEMW *lviW);
BOOL TreeView_GetItemWide(HWND hWnd, TVITEMW *tviW);
HTREEITEM TreeView_InsertItemWide(HWND hWnd, TVINSERTSTRUCTW *tvisW);
BOOL TreeView_SetItemWide(HWND hWnd, const TVITEMW *tviW);
int ComboBox_AddStringWide(HWND hWnd, const wchar_t *wpString);
int ComboBox_InsertStringWide(HWND hWnd, int nIndex, const wchar_t *wpString);
int ComboBox_FindStringExactWide(HWND hWnd, int nIndex, const wchar_t *wpString);
int ComboBox_FindStringWide(HWND hWnd, int nIndex, const wchar_t *wpString);
int ComboBox_GetLBTextLenWide(HWND hWnd, int nIndex);
int ComboBox_GetLBTextWide(HWND hWnd, int nIndex, wchar_t *wszText);
int ListBox_AddStringWide(HWND hWnd, const wchar_t *wpString);
int ListBox_InsertStringWide(HWND hWnd, int nIndex, const wchar_t *wpString);
int ListBox_FindStringExactWide(HWND hWnd, int nIndex, const wchar_t *wpString);
int ListBox_FindStringWide(HWND hWnd, int nIndex, const wchar_t *wpString);
int ListBox_GetTextWide(HWND hWnd, int nIndex, wchar_t *wszText);
BOOL TabCtrl_GetItemWide(HWND hWnd, int nIndex, TCITEMW *tciW);
int TabCtrl_InsertItemWide(HWND hWnd, int nIndex, const TCITEMW *tciW);
BOOL TabCtrl_SetItemWide(HWND hWnd, int nIndex, const TCITEMW *tciW);
DWORD StatusBar_GetTextWide(HWND hWnd, int iPart, wchar_t *wszText);
BOOL StatusBar_SetTextWide(HWND hWnd, int iPart, const wchar_t *wpText);

//GetWindowLongPtrWide/SetWindowLongPtrWide
#ifndef DWLP_MSGRESULT
  #define DWLP_MSGRESULT 0
#endif
#ifndef GWLP_WNDPROC
  #define GWLP_WNDPROC (-4)
#endif
#ifndef GWLP_HINSTANCE
  #define GWLP_HINSTANCE (-6)
#endif
#ifndef GWLP_HWNDPARENT
  #define GWLP_HWNDPARENT (-8)
#endif
#ifndef GWLP_ID
  #define GWLP_ID (-12)
#endif
#ifndef GWLP_USERDATA
  #define GWLP_USERDATA (-21)
#endif

//GetClassLongPtrWide/SetClassLongPtrWide
#ifndef GCLP_MENUNAME
  #define GCLP_MENUNAME (-8)
#endif
#ifndef GCLP_HBRBACKGROUND
  #define GCLP_HBRBACKGROUND (-10)
#endif
#ifndef GCLP_HCURSOR
  #define GCLP_HCURSOR (-12)
#endif
#ifndef GCLP_HICON
  #define GCLP_HICON (-14)
#endif
#ifndef GCLP_HMODULE
  #define GCLP_HMODULE (-16)
#endif
#ifndef GCLP_WNDPROC
  #define GCLP_WNDPROC (-24)
#endif
#ifndef GCLP_HICONSM
  #define GCLP_HICONSM (-34)
#endif

//IconExtractWide
#ifndef LOAD_LIBRARY_AS_IMAGE_RESOURCE
  #define LOAD_LIBRARY_AS_IMAGE_RESOURCE 0x00000020
#endif

//// File and directories
#if defined CreateDirectoryWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define CreateDirectoryWide_INCLUDED
#undef CreateDirectoryWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL CreateDirectoryWide(const wchar_t *wpDir, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateDirectoryW(wpDir, lpSecurityAttributes);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pDir=AllocAnsi(wpDir);
    BOOL bResult;

    bResult=CreateDirectoryA(pDir, lpSecurityAttributes);

    FreeAnsi(pDir);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined RemoveDirectoryWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define RemoveDirectoryWide_INCLUDED
#undef RemoveDirectoryWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL RemoveDirectoryWide(const wchar_t *wpPathName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RemoveDirectoryW(wpPathName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pPathName=AllocAnsi(wpPathName);
    BOOL bResult;

    bResult=RemoveDirectoryA(pPathName);

    FreeAnsi(pPathName);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined CreateFileWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define CreateFileWide_INCLUDED
#undef CreateFileWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HANDLE CreateFileWide(const wchar_t *wpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateFileW(wpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFileName=AllocAnsi(wpFileName);
    HANDLE hResult;

    hResult=CreateFileA(pFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

    FreeAnsi(pFileName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined CopyFileWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define CopyFileWide_INCLUDED
#undef CopyFileWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL CopyFileWide(const wchar_t *wpExistingFileName, const wchar_t *wpNewFileName, BOOL bFailIfExists)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CopyFileW(wpExistingFileName, wpNewFileName, bFailIfExists);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pExistingFileName=AllocAnsi(wpExistingFileName);
    char *pNewFileName=AllocAnsi(wpNewFileName);
    BOOL bResult;

    bResult=CopyFileA(pExistingFileName, pNewFileName, bFailIfExists);

    FreeAnsi(pExistingFileName);
    FreeAnsi(pNewFileName);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined MoveFileWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define MoveFileWide_INCLUDED
#undef MoveFileWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL MoveFileWide(const wchar_t *wpExistingFileName, const wchar_t *wpNewFileName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return MoveFileW(wpExistingFileName, wpNewFileName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pExistingFileName=AllocAnsi(wpExistingFileName);
    char *pNewFileName=AllocAnsi(wpNewFileName);
    BOOL bResult;

    bResult=MoveFileA(pExistingFileName, pNewFileName);

    FreeAnsi(pExistingFileName);
    FreeAnsi(pNewFileName);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined DeleteFileWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define DeleteFileWide_INCLUDED
#undef DeleteFileWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL DeleteFileWide(const wchar_t *wpFile)
{
  if (WideGlobal_bOldWindows == FALSE)
    return DeleteFileW(wpFile);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFile=AllocAnsi(wpFile);
    BOOL bResult;

    bResult=DeleteFileA(pFile);

    FreeAnsi(pFile);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined GetFileAttributesWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetFileAttributesWide_INCLUDED
#undef GetFileAttributesWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline DWORD GetFileAttributesWide(const wchar_t *wpFile)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetFileAttributesW(wpFile);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFile=AllocAnsi(wpFile);
    DWORD dwResult;

    dwResult=GetFileAttributesA(pFile);

    FreeAnsi(pFile);
    return dwResult;
  }

  WideNotInitialized();
  return (DWORD)-1;
}
#endif

#if defined SetFileAttributesWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define SetFileAttributesWide_INCLUDED
#undef SetFileAttributesWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL SetFileAttributesWide(const wchar_t *wpFile, DWORD dwFileAttributes)
{
  if (WideGlobal_bOldWindows == FALSE)
    return SetFileAttributesW(wpFile, dwFileAttributes);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFile=AllocAnsi(wpFile);
    BOOL bResult;

    bResult=SetFileAttributesA(pFile, dwFileAttributes);

    FreeAnsi(pFile);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined FindFirstFileWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define FindFirstFileWide_INCLUDED
#undef FindFirstFileWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HANDLE FindFirstFileWide(const wchar_t *wpFile, WIN32_FIND_DATAW *lpFindFileData)
{
  if (WideGlobal_bOldWindows == FALSE)
    return FindFirstFileW(wpFile, lpFindFileData);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    WIN32_FIND_DATAA wfdA;
    char *pFile=AllocAnsi(wpFile);
    HANDLE hResult;

    if ((hResult=FindFirstFileA(pFile, &wfdA)) != INVALID_HANDLE_VALUE)
    {
      xmemcpy(lpFindFileData, &wfdA, offsetof(WIN32_FIND_DATAA, cFileName));
      AnsiToWide(wfdA.cFileName, -1, lpFindFileData->cFileName, MAX_PATH);
      AnsiToWide(wfdA.cAlternateFileName, -1, lpFindFileData->cAlternateFileName, 14);
    }
    FreeAnsi(pFile);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined FindNextFileWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define FindNextFileWide_INCLUDED
#undef FindNextFileWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL FindNextFileWide(HANDLE hFindFile, WIN32_FIND_DATAW *lpFindFileData)
{
  if (WideGlobal_bOldWindows == FALSE)
    return FindNextFileW(hFindFile, lpFindFileData);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    WIN32_FIND_DATAA wfdA;
    BOOL bResult;

    if (bResult=FindNextFileA(hFindFile, &wfdA))
    {
      xmemcpy(lpFindFileData, &wfdA, offsetof(WIN32_FIND_DATAA, cFileName));
      AnsiToWide(wfdA.cFileName, -1, lpFindFileData->cFileName, MAX_PATH);
      AnsiToWide(wfdA.cAlternateFileName, -1, lpFindFileData->cAlternateFileName, 14);
    }
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined GetCurrentDirectoryWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetCurrentDirectoryWide_INCLUDED
#undef GetCurrentDirectoryWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline DWORD GetCurrentDirectoryWide(int nDirMax, wchar_t *wszDir)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetCurrentDirectoryW(nDirMax, wszDir);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char szDir[MAX_PATH];
    DWORD dwResult;

    GetCurrentDirectoryA(MAX_PATH, szDir);
    if (dwResult=AnsiToWide(szDir, -1, wszDir, nDirMax))
      --dwResult;
    return dwResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined SetCurrentDirectoryWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define SetCurrentDirectoryWide_INCLUDED
#undef SetCurrentDirectoryWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL SetCurrentDirectoryWide(const wchar_t *wpDir)
{
  if (WideGlobal_bOldWindows == FALSE)
    return SetCurrentDirectoryW(wpDir);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pDir=AllocAnsi(wpDir);
    BOOL bResult;

    bResult=SetCurrentDirectoryA(pDir);

    FreeAnsi(pDir);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined LoadLibraryWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define LoadLibraryWide_INCLUDED
#undef LoadLibraryWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HMODULE LoadLibraryWide(const wchar_t *wpFileName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return LoadLibraryW(wpFileName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFileName=AllocAnsi(wpFileName);
    HMODULE hResult;

    hResult=LoadLibraryA(pFileName);

    FreeAnsi(pFileName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined LoadLibraryExWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define LoadLibraryExWide_INCLUDED
#undef LoadLibraryExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HMODULE LoadLibraryExWide(const wchar_t *wpFileName, HANDLE hFile, DWORD dwFlags)
{
  if (WideGlobal_bOldWindows == FALSE)
    return LoadLibraryExW(wpFileName, hFile, dwFlags);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFileName=AllocAnsi(wpFileName);
    HMODULE hResult;

    hResult=LoadLibraryExA(pFileName, hFile, dwFlags);

    FreeAnsi(pFileName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetModuleHandleWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetModuleHandleWide_INCLUDED
#undef GetModuleHandleWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HMODULE GetModuleHandleWide(const wchar_t *wpModule)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetModuleHandleW(wpModule);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pModule=AllocAnsi(wpModule);
    HMODULE hResult;

    hResult=GetModuleHandleA(pModule);

    FreeAnsi(pModule);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetModuleFileNameWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetModuleFileNameWide_INCLUDED
#undef GetModuleFileNameWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline DWORD GetModuleFileNameWide(HMODULE hModule, wchar_t *wszFileName, DWORD nSize)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetModuleFileNameW(hModule, wszFileName, nSize);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char szFileName[MAX_PATH];
    DWORD dwResult;

    if (dwResult=GetModuleFileNameA(hModule, szFileName, MAX_PATH))
    {
      if (dwResult=AnsiToWide(szFileName, -1, wszFileName, nSize))
        --dwResult;
    }
    return dwResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetFullPathNameWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetFullPathNameWide_INCLUDED
#undef GetFullPathNameWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline DWORD GetFullPathNameWide(const wchar_t *wpPath, DWORD nBufferLength, wchar_t *wszBuffer, wchar_t **wpFilePart)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetFullPathNameW(wpPath, nBufferLength, wszBuffer, wpFilePart);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pPath=AllocAnsi(wpPath);
    char szFullPath[MAX_PATH];
    wchar_t *wpCount;
    char *pFilePart;
    DWORD dwResult;

    if (dwResult=GetFullPathNameA(pPath, MAX_PATH, szFullPath, &pFilePart))
    {
      if (dwResult=AnsiToWide(szFullPath, -1, wszBuffer, nBufferLength))
        --dwResult;

      //Get file name
      if (wpFilePart)
      {
        for (wpCount=wszBuffer + dwResult; wpCount >= wszBuffer; --wpCount)
        {
          if (*wpCount == '\\')
            break;
        }
        *wpFilePart=wpCount + 1;
      }
    }
    FreeAnsi(pPath);
    return dwResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetLongPathNameWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetLongPathNameWide_INCLUDED
#undef GetLongPathNameWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
#ifndef PTRWIDEFUNC_INCLUDED
  #define PTRWIDEFUNC_INCLUDED
#endif
__inline DWORD GetLongPathNameWide(const wchar_t *wpShortPath, wchar_t *wszLongPath, DWORD dwLongPathSize)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    if (WideGlobal_GetLongPathNameWPtr)
      return WideGlobal_GetLongPathNameWPtr(wpShortPath, wszLongPath, dwLongPathSize);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (WideGlobal_GetLongPathNameAPtr)
    {
      char *pShortPath=AllocAnsi(wpShortPath);
      char szLongPath[MAX_PATH];
      DWORD dwResult;

      if (dwResult=WideGlobal_GetLongPathNameAPtr(pShortPath, szLongPath, MAX_PATH))
      {
        if (dwResult=AnsiToWide(szLongPath, -1, wszLongPath, dwLongPathSize))
          --dwResult;
      }
      FreeAnsi(pShortPath);
      return dwResult;
    }
  }
  else WideNotInitialized();

  return (DWORD)xstrcpynW(wszLongPath, wpShortPath, dwLongPathSize);
}
#endif

#if defined GetTempPathWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetTempPathWide_INCLUDED
#undef GetTempPathWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline DWORD GetTempPathWide(DWORD nBufferLength, wchar_t *wszBuffer)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetTempPathW(nBufferLength, wszBuffer);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char szBuffer[MAX_PATH];
    DWORD dwResult;

    if (dwResult=GetTempPathA(MAX_PATH, szBuffer))
    {
      if (dwResult=AnsiToWide(szBuffer, -1, wszBuffer, nBufferLength))
        --dwResult;
    }
    return dwResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetTempFileNameWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define GetTempFileNameWide_INCLUDED
#undef GetTempFileNameWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline UINT GetTempFileNameWide(const wchar_t *wpPathName, const wchar_t *wpPrefixString, UINT uUnique, wchar_t *wszTempFileName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetTempFileNameW(wpPathName, wpPrefixString, uUnique, wszTempFileName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pPathName=AllocAnsi(wpPathName);
    char *pPrefixString=AllocAnsi(wpPrefixString);
    char szTempFileName[MAX_PATH];
    UINT uResult;

    if (uResult=GetTempFileNameA(pPathName, pPrefixString, uUnique, szTempFileName))
    {
      if (uResult=AnsiToWide(szTempFileName, -1, wszTempFileName, MAX_PATH))
        --uResult;
    }
    FreeAnsi(pPathName);
    FreeAnsi(pPrefixString);
    return uResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined SearchPathWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define SearchPathWide_INCLUDED
#undef SearchPathWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline UINT SearchPathWide(const wchar_t *wpPath, const wchar_t *wpFileName, const wchar_t *wpExtension, DWORD nBufferLength, wchar_t *wszBuffer, wchar_t **wpFilePart)
{
  if (WideGlobal_bOldWindows == FALSE)
    return SearchPathW(wpPath, wpFileName, wpExtension, nBufferLength, wszBuffer, wpFilePart);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pPath=AllocAnsi(wpPath);
    char *pFileName=AllocAnsi(wpFileName);
    char *pExtension=AllocAnsi(wpExtension);
    char szBuffer[MAX_PATH];
    wchar_t *wpCount;
    char *pFilePart;
    UINT uResult;

    if (uResult=SearchPathA(pPath, pFileName, pExtension, MAX_PATH, szBuffer, &pFilePart))
    {
      if (uResult=AnsiToWide(szBuffer, -1, wszBuffer, nBufferLength))
        --uResult;

      //Get file name
      if (wpFilePart)
      {
        for (wpCount=wszBuffer + uResult; wpCount >= wszBuffer; --wpCount)
        {
          if (*wpCount == '\\')
            break;
        }
        *wpFilePart=wpCount + 1;
      }
    }
    FreeAnsi(pPath);
    FreeAnsi(pFileName);
    FreeAnsi(pExtension);
    return uResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined ExpandEnvironmentStringsWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define ExpandEnvironmentStringsWide_INCLUDED
#undef ExpandEnvironmentStringsWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline DWORD ExpandEnvironmentStringsWide(const wchar_t *wpSrc, wchar_t *wszDst, DWORD nSize)
{
  if (WideGlobal_bOldWindows == FALSE)
    return ExpandEnvironmentStringsW(wpSrc, wszDst, nSize);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pSrc=AllocAnsi(wpSrc);
    char szDst[MAX_PATH];
    DWORD dwResult;

    if (dwResult=ExpandEnvironmentStringsA(pSrc, szDst, MAX_PATH))
      dwResult=AnsiToWide(szDst, -1, wszDst, nSize);

    FreeAnsi(pSrc);
    return dwResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined CreateProcessWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define CreateProcessWide_INCLUDED
#undef CreateProcessWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL CreateProcessWide(const wchar_t *wpApplicationName, wchar_t *wpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, const wchar_t *wpCurrentDirectory, STARTUPINFOW *lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateProcessW(wpApplicationName, wpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, wpCurrentDirectory, lpStartupInfo, lpProcessInformation);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    STARTUPINFOA si;
    char *pApplicationName=AllocAnsi(wpApplicationName);
    char *pCommandLine=AllocAnsi(wpCommandLine);
    char *pCurrentDirectory=AllocAnsi(wpCurrentDirectory);
    char *pDesktop=AllocAnsi(lpStartupInfo->lpDesktop);
    char *pTitle=AllocAnsi(lpStartupInfo->lpTitle);
    BOOL bResult;

    xmemcpy(&si, lpStartupInfo, sizeof(STARTUPINFOA));
    si.lpDesktop=pDesktop;
    si.lpTitle=pTitle;
    bResult=CreateProcessA(pApplicationName, pCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, pCurrentDirectory, &si, lpProcessInformation);

    FreeAnsi(pApplicationName);
    FreeAnsi(pCommandLine);
    FreeAnsi(pCurrentDirectory);
    FreeAnsi(pDesktop);
    FreeAnsi(pTitle);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

//Non-system
#if defined FileExistsAnsi || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define FileExistsAnsi_INCLUDED
#undef FileExistsAnsi
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL FileExistsAnsi(const char *pFile)
{
  DWORD dwAttr=GetFileAttributesA(pFile);
  if (dwAttr == (DWORD)-1 || (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    return FALSE;
  return TRUE;
}
#endif

#if defined FileExistsWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define FileExistsWide_INCLUDED
#undef FileExistsWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL FileExistsWide(const wchar_t *wpFile)
{
  DWORD dwAttr=GetFileAttributesWide(wpFile);
  if (dwAttr == (DWORD)-1 || (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    return FALSE;
  return TRUE;
}
#endif

#if defined DirExistsAnsi || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define DirExistsAnsi_INCLUDED
#undef DirExistsAnsi
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL DirExistsAnsi(const char *pDir)
{
  DWORD dwAttr=GetFileAttributesA(pDir);
  if (dwAttr == (DWORD)-1 || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    return FALSE;
  return TRUE;
}
#endif

#if defined DirExistsWide || defined FILEWIDEFUNC || defined ALLWIDEFUNC
#define DirExistsWide_INCLUDED
#undef DirExistsWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL DirExistsWide(const wchar_t *wpDir)
{
  DWORD dwAttr=GetFileAttributesWide(wpDir);
  if (dwAttr == (DWORD)-1 || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    return FALSE;
  return TRUE;
}
#endif


//// Shell
#if defined ShellExecuteWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define ShellExecuteWide_INCLUDED
#undef ShellExecuteWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HINSTANCE ShellExecuteWide(HWND hwnd, const wchar_t *wpOperation, const wchar_t *wpFile, const wchar_t *wpParameters, const wchar_t *wpDirectory, INT nShowCmd)
{
  if (WideGlobal_bOldWindows == FALSE)
    return ShellExecuteW(hwnd, wpOperation, wpFile, wpParameters, wpDirectory, nShowCmd);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pOperation=AllocAnsi(wpOperation);
    char *pFile=AllocAnsi(wpFile);
    char *pParameters=AllocAnsi(wpParameters);
    char *pDirectory=AllocAnsi(wpDirectory);
    HINSTANCE hResult;

    hResult=ShellExecuteA(hwnd, pOperation, pFile, pParameters, pDirectory, nShowCmd);

    FreeAnsi(pOperation);
    FreeAnsi(pFile);
    FreeAnsi(pParameters);
    FreeAnsi(pDirectory);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined SHGetPathFromIDListWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define SHGetPathFromIDListWide_INCLUDED
#undef SHGetPathFromIDListWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
#ifndef PTRWIDEFUNC_INCLUDED
  #define PTRWIDEFUNC_INCLUDED
#endif
__inline BOOL SHGetPathFromIDListWide(LPCITEMIDLIST pidl, wchar_t *wszPath)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    if (WideGlobal_SHGetPathFromIDListWPtr)
      return WideGlobal_SHGetPathFromIDListWPtr(pidl, wszPath);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (WideGlobal_SHGetPathFromIDListAPtr)
    {
      char szPath[MAX_PATH];
      BOOL bResult;

      if (bResult=WideGlobal_SHGetPathFromIDListAPtr(pidl, szPath))
        AnsiToWide(szPath, -1, wszPath, MAX_PATH);
      return bResult;
    }
  }
  else WideNotInitialized();

  return FALSE;
}
#endif

#if defined SHBrowseForFolderWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define SHBrowseForFolderWide_INCLUDED
#undef SHBrowseForFolderWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
#ifndef PTRWIDEFUNC_INCLUDED
  #define PTRWIDEFUNC_INCLUDED
#endif
__inline LPITEMIDLIST SHBrowseForFolderWide(BROWSEINFOW *lpbi)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    if (WideGlobal_SHBrowseForFolderWPtr)
      return WideGlobal_SHBrowseForFolderWPtr(lpbi);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (WideGlobal_SHBrowseForFolderAPtr)
    {
      BROWSEINFOA biA;
      LPITEMIDLIST lpResult;

      xmemcpy(&biA, lpbi, sizeof(BROWSEINFOA));
      biA.lpszTitle=AllocAnsi(lpbi->lpszTitle);
      if (lpResult=WideGlobal_SHBrowseForFolderAPtr(&biA))
        AnsiToWide(biA.pszDisplayName, -1, lpbi->pszDisplayName, MAX_PATH);

      FreeAnsi((char *)biA.lpszTitle);
      return lpResult;
    }
  }
  else WideNotInitialized();

  return NULL;
}
#endif

#if defined SHFileOperationWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define SHFileOperationWide_INCLUDED
#undef SHFileOperationWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
#ifndef PTRWIDEFUNC_INCLUDED
  #define PTRWIDEFUNC_INCLUDED
#endif
__inline int SHFileOperationWide(SHFILEOPSTRUCTW *lpfos)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    if (WideGlobal_SHFileOperationWPtr)
      return WideGlobal_SHFileOperationWPtr(lpfos);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (WideGlobal_SHFileOperationAPtr)
    {
      SHFILEOPSTRUCTA fosA;
      int nResult;

      fosA.hwnd=lpfos->hwnd;
      fosA.wFunc=lpfos->wFunc;
      fosA.pFrom=AllocAnsiLen(lpfos->pFrom, (int)xarrlenW(lpfos->pFrom, NULL));
      if (lpfos->pTo)
        fosA.pTo=AllocAnsiLen(lpfos->pTo, (int)xarrlenW(lpfos->pTo, NULL));
      else
        fosA.pTo=NULL;
      fosA.fFlags=lpfos->fFlags;
      fosA.fAnyOperationsAborted=lpfos->fAnyOperationsAborted;
      fosA.hNameMappings=lpfos->hNameMappings;
      if (lpfos->fFlags & FOF_WANTMAPPINGHANDLE)
        fosA.lpszProgressTitle=AllocAnsi(lpfos->lpszProgressTitle);
      else
        fosA.lpszProgressTitle=NULL;

      if (nResult=WideGlobal_SHFileOperationAPtr(&fosA))
        lpfos->hNameMappings=fosA.hNameMappings;

      FreeAnsi((char *)fosA.pFrom);
      FreeAnsi((char *)fosA.pTo);
      FreeAnsi((char *)fosA.lpszProgressTitle);
      return nResult;
    }
  }
  else WideNotInitialized();

  return 0;
}
#endif

#if defined GetOpenFileNameWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define GetOpenFileNameWide_INCLUDED
#undef GetOpenFileNameWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL GetOpenFileNameWide(LPOPENFILENAMEW lpofn)
{
  return GetOpenOrSaveFileNameWide(lpofn, FALSE);
}
#endif

#if defined GetSaveFileNameWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define GetSaveFileNameWide_INCLUDED
#undef GetSaveFileNameWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL GetSaveFileNameWide(LPOPENFILENAMEW lpofn)
{
  return GetOpenOrSaveFileNameWide(lpofn, TRUE);
}
#endif

#if defined GetOpenFileNameWide_INCLUDED || defined GetSaveFileNameWide_INCLUDED || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
__inline BOOL GetOpenOrSaveFileNameWide(LPOPENFILENAMEW lpofn, BOOL bSave)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    if (!bSave)
      return GetOpenFileNameW(lpofn);
    else
      return GetSaveFileNameW(lpofn);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    OPENFILENAMEA ofnA;
    BOOL bResult;

    //Make nMaxFile less than 0x7FFF otherwise crash possible
    xmemcpy(&ofnA, lpofn, sizeof(OPENFILENAMEA));
    ofnA.lpstrFilter=AllocAnsiLen(lpofn->lpstrFilter, (int)xarrlenW(lpofn->lpstrFilter, NULL));
    ofnA.lpstrCustomFilter=AllocAnsiLen(lpofn->lpstrCustomFilter, (int)xarrlenW(lpofn->lpstrCustomFilter, NULL));
    ofnA.nMaxFile=min(lpofn->nMaxFile * sizeof(wchar_t), 0x7FFF);
    ofnA.lpstrFile=(char *)GlobalAlloc(GPTR, ofnA.nMaxFile);
    WideToAnsi(lpofn->lpstrFile, -1, ofnA.lpstrFile, ofnA.nMaxFile);
    if (lpofn->lpstrFileTitle)
    {
      ofnA.nMaxFileTitle=min(lpofn->nMaxFileTitle * sizeof(wchar_t), 0x7FFF);
      ofnA.lpstrFileTitle=(char *)GlobalAlloc(GPTR, ofnA.nMaxFileTitle);
    }
    ofnA.lpstrInitialDir=AllocAnsi(lpofn->lpstrInitialDir);
    ofnA.lpstrTitle=AllocAnsi(lpofn->lpstrTitle);
    ofnA.lpstrDefExt=AllocAnsi(lpofn->lpstrDefExt);
    if (lpofn->Flags & OFN_ENABLETEMPLATE)
      if ((UINT_PTR)lpofn->lpTemplateName > MAXUHALF_PTR)
        ofnA.lpTemplateName=AllocAnsi(lpofn->lpTemplateName);

    if (!bSave)
      bResult=GetOpenFileNameA(&ofnA);
    else
      bResult=GetSaveFileNameA(&ofnA);
    if (bResult)
    {
      AnsiToWide(ofnA.lpstrCustomFilter, (int)xarrlenA(ofnA.lpstrCustomFilter, NULL), lpofn->lpstrCustomFilter, lpofn->nMaxCustFilter);
      AnsiToWide(ofnA.lpstrFile, (int)xarrlenA(ofnA.lpstrFile, NULL), lpofn->lpstrFile, lpofn->nMaxFile);
      AnsiToWide(ofnA.lpstrFileTitle, (int)xarrlenA(ofnA.lpstrFileTitle, NULL), lpofn->lpstrFileTitle, lpofn->nMaxFileTitle);
    }

    FreeAnsi((char *)ofnA.lpstrFilter);
    FreeAnsi((char *)ofnA.lpstrCustomFilter);
    GlobalFree((HGLOBAL)ofnA.lpstrFile);
    if (lpofn->lpstrFileTitle)
      GlobalFree((HGLOBAL)ofnA.lpstrFileTitle);
    FreeAnsi((char *)ofnA.lpstrInitialDir);
    FreeAnsi((char *)ofnA.lpstrTitle);
    FreeAnsi((char *)ofnA.lpstrDefExt);
    if (lpofn->Flags & OFN_ENABLETEMPLATE)
      if ((UINT_PTR)lpofn->lpTemplateName > MAXUHALF_PTR)
        FreeAnsi((char *)ofnA.lpTemplateName);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined PropertySheetWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define PropertySheetWide_INCLUDED
#undef PropertySheetWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline INT_PTR PropertySheetWide(const PROPSHEETHEADERW *pshW)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    return PropertySheetW(pshW);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    PROPSHEETHEADERA pshA;
    PROPSHEETPAGEA *pspA;
    PROPSHEETPAGEA *pspCountA;
    const PROPSHEETPAGEW *pspCountW;
    DWORD dwPage;
    INT_PTR nResult=0;

    if (pspA=(PROPSHEETPAGEA *)GlobalAlloc(GPTR, pshW->nPages * pshW->ppsp->dwSize))
    {
      for (pspCountW=pshW->ppsp, pspCountA=pspA, dwPage=0; dwPage < pshW->nPages; ++pspCountW, ++pspCountA, ++dwPage)
      {
        xmemcpy(pspCountA, pspCountW, sizeof(PROPSHEETPAGEA));
        pspCountA->dwSize=sizeof(PROPSHEETPAGEA);
        if (!(pspCountW->dwFlags & PSP_DLGINDIRECT) && (UINT_PTR)pspCountW->pszTemplate > MAXUHALF_PTR)
          pspCountA->pszTemplate=AllocAnsi(pspCountW->pszTemplate);
        if ((pspCountW->dwFlags & PSP_USEICONID) && (UINT_PTR)pspCountW->pszIcon > MAXUHALF_PTR)
          pspCountA->pszIcon=AllocAnsi(pspCountW->pszIcon);
        if (pspCountW->dwFlags & PSP_USETITLE)
          pspCountA->pszTitle=AllocAnsi(pspCountW->pszTitle);
      }

      xmemcpy(&pshA, pshW, 40); //PROPSHEETHEADER_V1_SIZE
      pshA.dwSize=40; //PROPSHEETHEADER_V1_SIZE
      if ((pshW->dwFlags & PSH_USEICONID) && (UINT_PTR)pshW->pszIcon > MAXUHALF_PTR)
        pshA.pszIcon=AllocAnsi(pshW->pszIcon);
      if ((UINT_PTR)pshW->pszCaption > MAXUHALF_PTR)
        pshA.pszCaption=AllocAnsi(pshW->pszCaption);
      if ((pshW->dwFlags & PSH_USEPSTARTPAGE) && (UINT_PTR)pshW->pStartPage > MAXUHALF_PTR)
        pshA.pStartPage=AllocAnsi(pshW->pStartPage);
      pshA.ppsp=pspA;

      nResult=PropertySheetA(&pshA);

      for (pspCountA=pspA, dwPage=0; dwPage < pshW->nPages; ++pspCountA, ++dwPage)
      {
        if (pspCountA->pszTemplate && !(pspCountW->dwFlags & PSP_DLGINDIRECT) && (UINT_PTR)pspCountA->pszTemplate > MAXUHALF_PTR)
          FreeAnsi((char *)pspCountA->pszTemplate);
        if (pspCountA->pszIcon && (pspCountW->dwFlags & PSP_USEICONID) && (UINT_PTR)pspCountA->pszIcon > MAXUHALF_PTR)
          FreeAnsi((char *)pspCountA->pszIcon);
        if (pspCountA->pszTitle)
          FreeAnsi((char *)pspCountA->pszTitle);
      }
      if (pshA.pszIcon && (pshW->dwFlags & PSH_USEICONID) && (UINT_PTR)pshA.pszIcon > MAXUHALF_PTR)
        FreeAnsi((char *)pshA.pszIcon);
      if (pshA.pszCaption && (UINT_PTR)pshA.pszCaption > MAXUHALF_PTR)
        FreeAnsi((char *)pshA.pszCaption);
      if (pshA.pStartPage && (pshW->dwFlags & PSH_USEPSTARTPAGE) && (UINT_PTR)pshA.pStartPage > MAXUHALF_PTR)
        FreeAnsi((char *)pshA.pStartPage);
      GlobalFree((HGLOBAL)pspA);
    }
    return nResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined DragQueryFileWide || defined SHELLWIDEFUNC || defined ALLWIDEFUNC
#define DragQueryFileWide_INCLUDED
#undef DragQueryFileWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline UINT DragQueryFileWide(HDROP hDrop, UINT iFile, wchar_t *wszFile, UINT cch)
{
  if (WideGlobal_bOldWindows == FALSE)
    return DragQueryFileW(hDrop, iFile, wszFile, cch);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char szFile[MAX_PATH];
    int nFileLen;

    if (iFile == 0xFFFFFFFF)
      return DragQueryFileA(hDrop, iFile, NULL, 0);

    nFileLen=DragQueryFileA(hDrop, iFile, szFile, MAX_PATH);
    if (nFileLen=AnsiToWide(szFile, nFileLen + 1, wszFile, cch))
      --nFileLen;
    return nFileLen;
  }

  WideNotInitialized();
  return 0;
}
#endif


//// Registry
#if defined RegCreateKeyExWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegCreateKeyExWide_INCLUDED
#undef RegCreateKeyExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegCreateKeyExWide(HKEY hKey, const wchar_t *wpSubKey, DWORD dwReserved, wchar_t *wpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegCreateKeyExW(hKey, wpSubKey, dwReserved, wpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pSubKey=AllocAnsi(wpSubKey);
    char *pClass=AllocAnsi(wpClass);
    LONG lResult;

    lResult=RegCreateKeyExA(hKey, pSubKey, dwReserved, pClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);

    FreeAnsi(pSubKey);
    FreeAnsi(pClass);
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

#if defined RegOpenKeyExWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegOpenKeyExWide_INCLUDED
#undef RegOpenKeyExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegOpenKeyExWide(HKEY hKey, const wchar_t *wpSubKey, DWORD dwOptions, REGSAM samDesired, PHKEY phkResult)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegOpenKeyExW(hKey, wpSubKey, dwOptions, samDesired, phkResult);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pSubKey=AllocAnsi(wpSubKey);
    LONG lResult;

    lResult=RegOpenKeyExA(hKey, pSubKey, dwOptions, samDesired, phkResult);

    FreeAnsi(pSubKey);
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

#if defined RegEnumKeyExWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegEnumKeyExWide_INCLUDED
#undef RegEnumKeyExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegEnumKeyExWide(HKEY hKey, DWORD dwIndex, wchar_t *wszKeyName, LPDWORD lpcKeyName, LPDWORD lpReserved, wchar_t *lpClass, LPDWORD lpcClass, PFILETIME lpftLastWriteTime)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegEnumKeyExW(hKey, dwIndex, wszKeyName, lpcKeyName, lpReserved, lpClass, lpcClass, lpftLastWriteTime);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char szKeyName[MAX_PATH];
    char szClassName[MAX_PATH];
    DWORD dwKeyChars=MAX_PATH;
    DWORD dwClassChars=MAX_PATH;
    LONG lResult;

    if ((lResult=RegEnumKeyExA(hKey, dwIndex, szKeyName, &dwKeyChars, lpReserved, szClassName, &dwClassChars, lpftLastWriteTime)) == ERROR_SUCCESS)
    {
      if (lpcKeyName)
      {
        if (*lpcKeyName=AnsiToWide(szKeyName, dwKeyChars + 1, wszKeyName, *lpcKeyName))
          --*lpcKeyName;
      }
      if (lpClass)
      {
        if (*lpcClass=AnsiToWide(szClassName, dwClassChars + 1, lpClass, *lpcClass))
          --*lpcClass;
      }
    }
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

#if defined RegEnumValueWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegEnumValueWide_INCLUDED
#undef RegEnumValueWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegEnumValueWide(HKEY hKey, DWORD dwIndex, wchar_t *wszValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegEnumValueW(hKey, dwIndex, wszValueName, lpcValueName, lpReserved, lpType, lpData, lpcbData);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char szValueName[MAX_PATH];
    char *szData;
    DWORD dwValueChars=MAX_PATH;
    DWORD dwDataType;
    DWORD dwDataChars;
    LONG lResult;

    if ((lResult=RegEnumValueA(hKey, dwIndex, szValueName, &dwValueChars, lpReserved, &dwDataType, NULL, &dwDataChars)) == ERROR_SUCCESS)
    {
      if (*lpcValueName=AnsiToWide(szValueName, dwValueChars + 1, wszValueName, *lpcValueName))
        --*lpcValueName;
      dwValueChars=MAX_PATH;

      if (dwDataType == REG_SZ || dwDataType == REG_EXPAND_SZ || dwDataType == REG_MULTI_SZ)
      {
        if (lpData)
        {
          if (szData=(char *)GlobalAlloc(GPTR, dwDataChars))
          {
            lResult=RegEnumValueA(hKey, dwIndex, szValueName, &dwValueChars, lpReserved, lpType, (LPBYTE)szData, &dwDataChars);
            dwDataChars=AnsiToWide(szData, dwDataChars, (wchar_t *)lpData, *lpcbData / sizeof(wchar_t));
            GlobalFree((HGLOBAL)szData);
          }
        }
        if (lpType) *lpType=dwDataType;
        if (lpcbData) *lpcbData=dwDataChars * sizeof(wchar_t);
      }
      else lResult=RegEnumValueA(hKey, dwIndex, szValueName, &dwValueChars, lpReserved, lpType, lpData, lpcbData);
    }
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

#if defined RegQueryValueExWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegQueryValueExWide_INCLUDED
#undef RegQueryValueExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegQueryValueExWide(HKEY hKey, const wchar_t *wpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegQueryValueExW(hKey, wpValueName, lpReserved, lpType, lpData, lpcbData);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pValueName=AllocAnsi(wpValueName);
    char *szData;
    DWORD dwDataType;
    DWORD dwDataChars;
    LONG lResult;

    if ((lResult=RegQueryValueExA(hKey, pValueName, lpReserved, &dwDataType, NULL, &dwDataChars)) == ERROR_SUCCESS)
    {
      if (dwDataType == REG_SZ || dwDataType == REG_EXPAND_SZ || dwDataType == REG_MULTI_SZ)
      {
        if (lpData)
        {
          if (szData=(char *)GlobalAlloc(GPTR, dwDataChars))
          {
            lResult=RegQueryValueExA(hKey, pValueName, lpReserved, lpType, (LPBYTE)szData, &dwDataChars);
            dwDataChars=AnsiToWide(szData, dwDataChars, (wchar_t *)lpData, *lpcbData / sizeof(wchar_t));
            GlobalFree((HGLOBAL)szData);
          }
        }
        if (lpType) *lpType=dwDataType;
        if (lpcbData) *lpcbData=dwDataChars * sizeof(wchar_t);
      }
      else lResult=RegQueryValueExA(hKey, pValueName, lpReserved, lpType, lpData, lpcbData);
    }
    FreeAnsi(pValueName);
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

#if defined RegSetValueExWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegSetValueExWide_INCLUDED
#undef RegSetValueExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegSetValueExWide(HKEY hKey, const wchar_t *wpValueName, DWORD dwReserved, DWORD dwType, const BYTE *lpData, DWORD cbData)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegSetValueExW(hKey, wpValueName, dwReserved, dwType, lpData, cbData);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pValueName=AllocAnsi(wpValueName);
    char *szData;
    LONG lResult=!ERROR_SUCCESS;

    if (dwType == REG_SZ || dwType == REG_EXPAND_SZ || dwType == REG_MULTI_SZ)
    {
      if (szData=(char *)GlobalAlloc(GPTR, cbData))
      {
        cbData=WideToAnsi((wchar_t *)lpData, cbData / sizeof(wchar_t), szData, cbData);
        lResult=RegSetValueExA(hKey, pValueName, dwReserved, dwType, (LPBYTE)szData, cbData);
        GlobalFree((HGLOBAL)szData);
      }
    }
    else lResult=RegSetValueExA(hKey, pValueName, dwReserved, dwType, lpData, cbData);

    FreeAnsi(pValueName);
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

#if defined RegDeleteValueWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegDeleteValueWide_INCLUDED
#undef RegDeleteValueWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegDeleteValueWide(HKEY hKey, const wchar_t *wpValue)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegDeleteValueW(hKey, wpValue);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pValue=AllocAnsi(wpValue);
    LONG lResult;

    lResult=RegDeleteValueA(hKey, pValue);

    FreeAnsi(pValue);
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

#if defined RegDeleteKeyWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegDeleteKeyWide_INCLUDED
#undef RegDeleteKeyWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LONG RegDeleteKeyWide(HKEY hKey, const wchar_t *wpSubKey)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegDeleteKeyW(hKey, wpSubKey);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pSubKey=AllocAnsi(wpSubKey);
    LONG lResult;

    lResult=RegDeleteKeyA(hKey, pSubKey);

    FreeAnsi(pSubKey);
    return lResult;
  }

  WideNotInitialized();
  return !ERROR_SUCCESS;
}
#endif

//Non-system
#if defined RegClearKeyAnsi || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegClearKeyAnsi_INCLUDED
#undef RegClearKeyAnsi
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline void RegClearKeyAnsi(HKEY hKey, char *pSubKey)
{
  char szValue[MAX_PATH];
  HKEY hOpenKey;
  DWORD dwSize;
  DWORD dwType;

  if (RegOpenKeyExA(hKey, pSubKey, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hOpenKey) == ERROR_SUCCESS)
  {
    for (;;)
    {
      dwSize=MAX_PATH;
      if (RegEnumValueA(hOpenKey, 0, szValue, &dwSize, NULL, &dwType, NULL, NULL) != ERROR_SUCCESS)
        break;
      if (RegDeleteValueA(hOpenKey, szValue) != ERROR_SUCCESS)
        break;
    }
    RegCloseKey(hOpenKey);
  }
}
#endif

#if defined RegClearKeyWide || defined REGWIDEFUNC || defined ALLWIDEFUNC
#define RegClearKeyWide_INCLUDED
#undef RegClearKeyWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline void RegClearKeyWide(HKEY hKey, wchar_t *wpSubKey)
{
  wchar_t wszValue[MAX_PATH];
  HKEY hOpenKey;
  DWORD dwSize;
  DWORD dwType;

  if (RegOpenKeyExWide(hKey, wpSubKey, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hOpenKey) == ERROR_SUCCESS)
  {
    for (;;)
    {
      dwSize=MAX_PATH;
      if (RegEnumValueWide(hOpenKey, 0, wszValue, &dwSize, NULL, &dwType, NULL, NULL) != ERROR_SUCCESS)
        break;
      if (RegDeleteValueWide(hOpenKey, wszValue) != ERROR_SUCCESS)
        break;
    }
    RegCloseKey(hOpenKey);
  }
}
#endif


//// Information
#if defined GetDateFormatWide || defined INFOWIDEFUNC || defined ALLWIDEFUNC
#define GetDateFormatWide_INCLUDED
#undef GetDateFormatWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetDateFormatWide(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME *lpDate, const wchar_t *wpFormat, wchar_t *wszDateStr, int cchDate)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetDateFormatW(Locale, dwFlags, lpDate, wpFormat, wszDateStr, cchDate);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFormat=AllocAnsi(wpFormat);
    char szDateStr[MAX_PATH];
    int nResult;

    if (nResult=GetDateFormatA(Locale, dwFlags, lpDate, pFormat, szDateStr, MAX_PATH))
      nResult=AnsiToWide(szDateStr, -1, wszDateStr, cchDate);

    FreeAnsi(pFormat);
    return nResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetTimeFormatWide || defined INFOWIDEFUNC || defined ALLWIDEFUNC
#define GetTimeFormatWide_INCLUDED
#undef GetTimeFormatWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetTimeFormatWide(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME *lpTime, const wchar_t *wpFormat, wchar_t *wszTimeStr, int cchTime)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetTimeFormatW(Locale, dwFlags, lpTime, wpFormat, wszTimeStr, cchTime);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFormat=AllocAnsi(wpFormat);
    char szTimeStr[MAX_PATH];
    int nResult;

    if (nResult=GetTimeFormatA(Locale, dwFlags, lpTime, pFormat, szTimeStr, MAX_PATH))
      nResult=AnsiToWide(szTimeStr, -1, wszTimeStr, cchTime);

    FreeAnsi(pFormat);
    return nResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetCPInfoExWide || defined INFOWIDEFUNC || defined ALLWIDEFUNC
#define GetCPInfoExWide_INCLUDED
#undef GetCPInfoExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
#ifndef PTRWIDEFUNC_INCLUDED
  #define PTRWIDEFUNC_INCLUDED
#endif
__inline BOOL GetCPInfoExWide(UINT CodePage, DWORD dwFlags, LPCPINFOEXW lpCPInfoEx)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    if (WideGlobal_GetCPInfoExWPtr)
      return WideGlobal_GetCPInfoExWPtr(CodePage, dwFlags, lpCPInfoEx);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (WideGlobal_GetCPInfoExAPtr)
    {
      CPINFOEXA cpieA;
      BOOL bResult;

      if (bResult=WideGlobal_GetCPInfoExAPtr(CodePage, dwFlags, &cpieA))
      {
        xmemcpy(lpCPInfoEx, &cpieA, offsetof(CPINFOEXA, CodePageName));
        AnsiToWide(cpieA.CodePageName, -1, lpCPInfoEx->CodePageName, MAX_PATH);
      }
      return bResult;
    }
  }
  else WideNotInitialized();

  return FALSE;
}
#endif

#if defined GetKeyNameTextWide || defined INFOWIDEFUNC || defined ALLWIDEFUNC
#define GetKeyNameTextWide_INCLUDED
#undef GetKeyNameTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetKeyNameTextWide(LONG lParam, wchar_t *wpString, int nSize)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetKeyNameTextW(lParam, wpString, nSize);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char szString[MAX_PATH];
    int nResult;

    if (nResult=GetKeyNameTextA(lParam, szString, MAX_PATH))
    {
      if (nResult=AnsiToWide(szString, -1, wpString, nSize))
       --nResult;
    }
    return nResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined Process32FirstWide || defined PROCESSWIDEFUNC || defined ALLWIDEFUNC
#define Process32FirstWide_INCLUDED
#undef Process32FirstWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL Process32FirstWide(HANDLE hSnapshot, LPPROCESSENTRY32W lppe)
{
  if (WideGlobal_bOldWindows == FALSE)
    return Process32FirstW(hSnapshot, lppe);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    PROCESSENTRY32 peA;
    BOOL bResult;

    peA.dwSize=sizeof(PROCESSENTRY32);
    if (bResult=Process32First(hSnapshot, &peA))
    {
      xmemcpy(lppe, &peA, offsetof(PROCESSENTRY32, szExeFile));
      AnsiToWide(peA.szExeFile, -1, lppe->szExeFile, MAX_PATH);
    }
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined Process32NextWide || defined PROCESSWIDEFUNC || defined ALLWIDEFUNC
#define Process32NextWide_INCLUDED
#undef Process32NextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL Process32NextWide(HANDLE hSnapshot, LPPROCESSENTRY32W lppe)
{
  if (WideGlobal_bOldWindows == FALSE)
    return Process32NextW(hSnapshot, lppe);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    PROCESSENTRY32 peA;
    BOOL bResult;

    peA.dwSize=sizeof(PROCESSENTRY32);
    if (bResult=Process32Next(hSnapshot, &peA))
    {
      xmemcpy(lppe, &peA, offsetof(PROCESSENTRY32, szExeFile));
      AnsiToWide(peA.szExeFile, -1, lppe->szExeFile, MAX_PATH);
    }
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

//// Font
#if defined CreateFontIndirectWide || defined FONTWIDEFUNC || defined ALLWIDEFUNC
#define CreateFontIndirectWide_INCLUDED
#undef CreateFontIndirectWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HFONT CreateFontIndirectWide(const LOGFONTW *lfFont)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateFontIndirectW(lfFont);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    LOGFONTA lfA;

    xmemcpy(&lfA, lfFont, offsetof(LOGFONTW, lfFaceName));
    WideCharToMultiByte(CP_ACP, 0, lfFont->lfFaceName, -1, lfA.lfFaceName, LF_FACESIZE, NULL, NULL);

    return CreateFontIndirectA(&lfA);
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined AddFontResourceWide || defined FONTWIDEFUNC || defined ALLWIDEFUNC
#define AddFontResourceWide_INCLUDED
#undef AddFontResourceWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int AddFontResourceWide(const wchar_t *wpFontName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return AddFontResourceW(wpFontName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFontName=AllocAnsi(wpFontName);
    int nResult;

    nResult=AddFontResourceA(pFontName);

    FreeAnsi(pFontName);
    return nResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined RemoveFontResourceWide || defined FONTWIDEFUNC || defined ALLWIDEFUNC
#define RemoveFontResourceWide_INCLUDED
#undef RemoveFontResourceWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL RemoveFontResourceWide(const wchar_t *wpFontName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RemoveFontResourceW(wpFontName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pFontName=AllocAnsi(wpFontName);
    BOOL bResult;

    bResult=RemoveFontResourceA(pFontName);

    FreeAnsi(pFontName);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif


//// Print
#if defined StartDocWide || defined PRINTWIDEFUNC || defined ALLWIDEFUNC
#define StartDocWide_INCLUDED
#undef StartDocWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int StartDocWide(HDC hdc, const DOCINFOW *lpdi)
{
  if (WideGlobal_bOldWindows == FALSE)
    return StartDocW(hdc, lpdi);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    DOCINFOA diA;
    char *pDocName=AllocAnsi(lpdi->lpszDocName);
    char *pOutput=AllocAnsi(lpdi->lpszOutput);
    char *pDatatype=AllocAnsi(lpdi->lpszDatatype);
    int nResult;

    diA.cbSize=sizeof(DOCINFOA);
    diA.lpszDocName=pDocName;
    diA.lpszOutput=pOutput;
    diA.lpszDatatype=pDatatype;
    diA.fwType=lpdi->fwType;
    nResult=StartDocA(hdc, &diA);

    FreeAnsi(pDocName);
    FreeAnsi(pOutput);
    FreeAnsi(pDatatype);
    return nResult;
  }

  WideNotInitialized();
  return 0;
}
#endif


//// Windows
#if defined RegisterClassWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define RegisterClassWide_INCLUDED
#undef RegisterClassWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline ATOM RegisterClassWide(WNDCLASSW *lpWndClass)
{
  if (WideGlobal_bOldWindows == FALSE)
    return RegisterClassW(lpWndClass);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    WNDCLASSA wcA;
    ATOM wResult;

    xmemcpy(&wcA, lpWndClass, sizeof(WNDCLASSA));
    if ((UINT_PTR)lpWndClass->lpszMenuName > MAXUHALF_PTR)
      wcA.lpszMenuName=AllocAnsi(lpWndClass->lpszMenuName);
    wcA.lpszClassName=AllocAnsi(lpWndClass->lpszClassName);
    wResult=RegisterClassA(&wcA);

    if ((UINT_PTR)lpWndClass->lpszMenuName > MAXUHALF_PTR)
      FreeAnsi((char *)wcA.lpszMenuName);
    FreeAnsi((char *)wcA.lpszClassName);
    return wResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined UnregisterClassWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define UnregisterClassWide_INCLUDED
#undef UnregisterClassWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL UnregisterClassWide(const wchar_t *wpClassName, HINSTANCE hInstance)
{
  if (WideGlobal_bOldWindows == FALSE)
    return UnregisterClassW(wpClassName, hInstance);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pClassName=AllocAnsi(wpClassName);
    BOOL bResult;

    bResult=UnregisterClassA(pClassName, hInstance);

    FreeAnsi(pClassName);
    return bResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined CreateWindowExWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define CreateWindowExWide_INCLUDED
#undef CreateWindowExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HWND CreateWindowExWide(DWORD dwExStyle, const wchar_t *wpClassName, const wchar_t *wpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateWindowExW(dwExStyle, wpClassName, wpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pClassName=AllocAnsi(wpClassName);
    char *pWindowName=AllocAnsi(wpWindowName);
    HWND hResult;

    hResult=CreateWindowExA(dwExStyle, pClassName, pWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

    FreeAnsi(pClassName);
    FreeAnsi(pWindowName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined CreateMDIWindowWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define CreateMDIWindowWide_INCLUDED
#undef CreateMDIWindowWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HWND CreateMDIWindowWide(const wchar_t *wpClassName, const wchar_t *wpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HINSTANCE hInstance, LPARAM lParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateMDIWindowW((wchar_t *)wpClassName, (wchar_t *)wpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hInstance, lParam);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pClassName=AllocAnsi(wpClassName);
    char *pWindowName=AllocAnsi(wpWindowName);
    HWND hResult;

    hResult=CreateMDIWindowA(pClassName, pWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hInstance, lParam);

    FreeAnsi(pClassName);
    FreeAnsi(pWindowName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined CreateDialogWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define CreateDialogWide_INCLUDED
#undef CreateDialogWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HWND CreateDialogWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateDialogW(hInstance, wpTemplate, hWndParent, lpDialogFunc);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pTemplate;
    HWND hResult;

    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      pTemplate=AllocAnsi(wpTemplate);
    else
      pTemplate=(char *)wpTemplate;
    hResult=CreateDialogA(hInstance, pTemplate, hWndParent, lpDialogFunc);
    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      FreeAnsi(pTemplate);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined CreateDialogParamWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define CreateDialogParamWide_INCLUDED
#undef CreateDialogParamWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HWND CreateDialogParamWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateDialogParamW(hInstance, wpTemplate, hWndParent, lpDialogFunc, dwInitParam);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pTemplate;
    HWND hResult;

    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      pTemplate=AllocAnsi(wpTemplate);
    else
      pTemplate=(char *)wpTemplate;
    hResult=CreateDialogParamA(hInstance, pTemplate, hWndParent, lpDialogFunc, dwInitParam);
    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      FreeAnsi(pTemplate);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined DialogBoxWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define DialogBoxWide_INCLUDED
#undef DialogBoxWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline INT_PTR DialogBoxWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc)
{
  if (WideGlobal_bOldWindows == FALSE)
    return DialogBoxW(hInstance, wpTemplate, hWndParent, lpDialogFunc);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pTemplate;
    INT_PTR nResult;

    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      pTemplate=AllocAnsi(wpTemplate);
    else
      pTemplate=(char *)wpTemplate;
    nResult=DialogBoxA(hInstance, pTemplate, hWndParent, lpDialogFunc);
    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      FreeAnsi(pTemplate);
    return nResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined DialogBoxParamWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define DialogBoxParamWide_INCLUDED
#undef DialogBoxParamWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline INT_PTR DialogBoxParamWide(HINSTANCE hInstance, const wchar_t *wpTemplate, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return DialogBoxParamW(hInstance, wpTemplate, hWndParent, lpDialogFunc, dwInitParam);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pTemplate;
    INT_PTR nResult;

    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      pTemplate=AllocAnsi(wpTemplate);
    else
      pTemplate=(char *)wpTemplate;
    nResult=DialogBoxParamA(hInstance, pTemplate, hWndParent, lpDialogFunc, dwInitParam);
    if ((UINT_PTR)wpTemplate > MAXUHALF_PTR)
      FreeAnsi(pTemplate);
    return nResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetClassNameWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define GetClassNameWide_INCLUDED
#undef GetClassNameWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetClassNameWide(HWND hWnd, wchar_t *wszText, int nTextMax)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetClassNameW(hWnd, wszText, nTextMax);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen=0;

    if (szText=(char *)GlobalAlloc(GPTR, nTextMax * sizeof(wchar_t)))
    {
      nTextLen=GetClassNameA(hWnd, szText, nTextMax * sizeof(wchar_t));
      if (nTextLen=AnsiToWide(szText, nTextLen + 1, wszText, nTextMax))
        --nTextLen;
      GlobalFree((HGLOBAL)szText);
    }
    return nTextLen;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetClassLongPtrWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define GetClassLongPtrWide_INCLUDED
#undef GetClassLongPtrWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline UINT_PTR GetClassLongPtrWide(HWND hWnd, int nIndex)
{
  #ifdef _WIN64
    if (WideGlobal_bOldWindows == FALSE)
      return GetClassLongPtrW(hWnd, nIndex);
    else if (WideGlobal_bOldWindows == TRUE)
      return GetClassLongPtrA(hWnd, nIndex);
  #else
    if (WideGlobal_bOldWindows == FALSE)
      return GetClassLongW(hWnd, nIndex);
    else if (WideGlobal_bOldWindows == TRUE)
      return GetClassLongA(hWnd, nIndex);
  #endif

  WideNotInitialized();
  return 0;
}
#endif

#if defined SetClassLongPtrWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define SetClassLongPtrWide_INCLUDED
#undef SetClassLongPtrWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline UINT_PTR SetClassLongPtrWide(HWND hWnd, int nIndex, UINT_PTR dwNewLong)
{
  #ifdef _WIN64
    if (WideGlobal_bOldWindows == FALSE)
      return SetClassLongPtrW(hWnd, nIndex, dwNewLong);
    else if (WideGlobal_bOldWindows == TRUE)
      return SetClassLongPtrA(hWnd, nIndex, dwNewLong);
  #else
    if (WideGlobal_bOldWindows == FALSE)
      return SetClassLongW(hWnd, nIndex, dwNewLong);
    else if (WideGlobal_bOldWindows == TRUE)
      return SetClassLongA(hWnd, nIndex, dwNewLong);
  #endif

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetWindowLongPtrWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define GetWindowLongPtrWide_INCLUDED
#undef GetWindowLongPtrWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline UINT_PTR GetWindowLongPtrWide(HWND hWnd, int nIndex)
{
  #ifdef _WIN64
    if (WideGlobal_bOldWindows == FALSE)
      return GetWindowLongPtrW(hWnd, nIndex);
    else if (WideGlobal_bOldWindows == TRUE)
      return GetWindowLongPtrA(hWnd, nIndex);
  #else
    if (WideGlobal_bOldWindows == FALSE)
      return GetWindowLongW(hWnd, nIndex);
    else if (WideGlobal_bOldWindows == TRUE)
      return GetWindowLongA(hWnd, nIndex);
  #endif

  WideNotInitialized();
  return 0;
}
#endif

#if defined SetWindowLongPtrWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define SetWindowLongPtrWide_INCLUDED
#undef SetWindowLongPtrWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline UINT_PTR SetWindowLongPtrWide(HWND hWnd, int nIndex, UINT_PTR dwNewLong)
{
  #ifdef _WIN64
    if (WideGlobal_bOldWindows == FALSE)
      return SetWindowLongPtrW(hWnd, nIndex, dwNewLong);
    else if (WideGlobal_bOldWindows == TRUE)
      return SetWindowLongPtrA(hWnd, nIndex, dwNewLong);
  #else
    if (WideGlobal_bOldWindows == FALSE)
      return SetWindowLongW(hWnd, nIndex, dwNewLong);
    else if (WideGlobal_bOldWindows == TRUE)
      return SetWindowLongA(hWnd, nIndex, dwNewLong);
  #endif

  WideNotInitialized();
  return 0;
}
#endif

#if defined CallWindowProcWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define CallWindowProcWide_INCLUDED
#undef CallWindowProcWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LRESULT CallWindowProcWide(WNDPROC lpPrevWndFunc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CallWindowProcW(lpPrevWndFunc, hWnd, uMsg, wParam, lParam);
  else if (WideGlobal_bOldWindows == TRUE)
    return CallWindowProcA(lpPrevWndFunc, hWnd, uMsg, wParam, lParam);

  WideNotInitialized();
  return 0;
}
#endif

#if defined DefWindowProcWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define DefWindowProcWide_INCLUDED
#undef DefWindowProcWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LRESULT DefWindowProcWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  else if (WideGlobal_bOldWindows == TRUE)
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);

  WideNotInitialized();
  return 0;
}
#endif

#if defined DefDlgProcWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define DefDlgProcWide_INCLUDED
#undef DefDlgProcWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LRESULT DefDlgProcWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return DefDlgProcW(hWnd, uMsg, wParam, lParam);
  else if (WideGlobal_bOldWindows == TRUE)
    return DefDlgProcA(hWnd, uMsg, wParam, lParam);

  WideNotInitialized();
  return 0;
}
#endif

#if defined FindWindowExWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define FindWindowExWide_INCLUDED
#undef FindWindowExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HWND FindWindowExWide(HWND hWndParent, HWND hWndChildAfter, const wchar_t *wpClassName, const wchar_t *wpWindowName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return FindWindowExW(hWndParent, hWndChildAfter, wpClassName, wpWindowName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pClassName=AllocAnsi(wpClassName);
    char *pWindowName=AllocAnsi(wpWindowName);
    HWND hResult;

    hResult=FindWindowExA(hWndParent, hWndChildAfter, pClassName, pWindowName);

    FreeAnsi(pClassName);
    FreeAnsi(pWindowName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetWindowTextLengthWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define GetWindowTextLengthWide_INCLUDED
#undef GetWindowTextLengthWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetWindowTextLengthWide(HWND hWnd)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetWindowTextLengthW(hWnd);
  else if (WideGlobal_bOldWindows == TRUE)
    return GetWindowTextLengthA(hWnd);

  WideNotInitialized();
  return 0;
}
#endif

#if defined GetWindowTextWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define GetWindowTextWide_INCLUDED
#undef GetWindowTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetWindowTextWide(HWND hWnd, wchar_t *wszText, int nTextMax)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetWindowTextW(hWnd, wszText, nTextMax);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen=0;

    //Make nTextMax less than 0x7FFF otherwise GetWindowTextA failed
    if (nTextMax > 0x7FFF) nTextMax=0x7FFF;

    if (szText=(char *)GlobalAlloc(GPTR, nTextMax * sizeof(wchar_t)))
    {
      nTextLen=GetWindowTextA(hWnd, szText, nTextMax * sizeof(wchar_t));
      if (nTextLen=AnsiToWide(szText, nTextLen + 1, wszText, nTextMax))
        --nTextLen;
      GlobalFree((HGLOBAL)szText);
    }
    return nTextLen;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined SetWindowTextWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define SetWindowTextWide_INCLUDED
#undef SetWindowTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL SetWindowTextWide(HWND hWnd, const wchar_t *wpText)
{
  if (WideGlobal_bOldWindows == FALSE)
    return SetWindowTextW(hWnd, wpText);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pText=AllocAnsi(wpText);
    BOOL bResult;

    bResult=SetWindowTextA(hWnd, pText);

    FreeAnsi(pText);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined GetDlgItemTextWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define GetDlgItemTextWide_INCLUDED
#undef GetDlgItemTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetDlgItemTextWide(HWND hDlg, int nIDDlgItem, wchar_t *wszText, int nTextMax)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetDlgItemTextW(hDlg, nIDDlgItem, wszText, nTextMax);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen=0;

    //Make nTextMax less than 0x7FFF otherwise GetDlgItemTextA failed
    if (nTextMax > 0x7FFF) nTextMax=0x7FFF;

    if (szText=(char *)GlobalAlloc(GPTR, nTextMax * sizeof(wchar_t)))
    {
      nTextLen=GetDlgItemTextA(hDlg, nIDDlgItem, szText, nTextMax * sizeof(wchar_t));
      if (nTextLen=AnsiToWide(szText, nTextLen + 1, wszText, nTextMax))
        --nTextLen;
      GlobalFree((HGLOBAL)szText);
    }
    return nTextLen;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined SetDlgItemTextWide || defined WINDOWWIDEFUNC || defined ALLWIDEFUNC
#define SetDlgItemTextWide_INCLUDED
#undef SetDlgItemTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL SetDlgItemTextWide(HWND hDlg, int nIDDlgItem, const wchar_t *wpText)
{
  if (WideGlobal_bOldWindows == FALSE)
    return SetDlgItemTextW(hDlg, nIDDlgItem, wpText);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pText=AllocAnsi(wpText);
    BOOL bResult;

    bResult=SetDlgItemTextA(hDlg, nIDDlgItem, pText);

    FreeAnsi(pText);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif


//// Messages
#if defined GetMessageWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define GetMessageWide_INCLUDED
#undef GetMessageWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL GetMessageWide(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
  else if (WideGlobal_bOldWindows == TRUE)
    return GetMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);

  WideNotInitialized();
  return 0;
}
#endif

#if defined PeekMessageWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define PeekMessageWide_INCLUDED
#undef PeekMessageWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL PeekMessageWide(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
  if (WideGlobal_bOldWindows == FALSE)
    return PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
  else if (WideGlobal_bOldWindows == TRUE)
    return PeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);

  WideNotInitialized();
  return 0;
}
#endif

#if defined DispatchMessageWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define DispatchMessageWide_INCLUDED
#undef DispatchMessageWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LRESULT DispatchMessageWide(const MSG *lpMsg)
{
  if (WideGlobal_bOldWindows == FALSE)
    return DispatchMessageW(lpMsg);
  else if (WideGlobal_bOldWindows == TRUE)
    return DispatchMessageA(lpMsg);

  WideNotInitialized();
  return 0;
}
#endif

#if defined TranslateAcceleratorWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define TranslateAcceleratorWide_INCLUDED
#undef TranslateAcceleratorWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int TranslateAcceleratorWide(HWND hWnd, HACCEL hAccTable, LPMSG lpMsg)
{
  if (WideGlobal_bOldWindows == FALSE)
    return TranslateAcceleratorW(hWnd, hAccTable, lpMsg);
  else if (WideGlobal_bOldWindows == TRUE)
    return TranslateAcceleratorA(hWnd, hAccTable, lpMsg);

  WideNotInitialized();
  return 0;
}
#endif

#if defined IsDialogMessageWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define IsDialogMessageWide_INCLUDED
#undef IsDialogMessageWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL IsDialogMessageWide(HWND hDlg, LPMSG lpMsg)
{
  if (WideGlobal_bOldWindows == FALSE)
    return IsDialogMessageW(hDlg, lpMsg);
  else if (WideGlobal_bOldWindows == TRUE)
    return IsDialogMessageA(hDlg, lpMsg);

  WideNotInitialized();
  return 0;
}
#endif

#if defined SendMessageWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define SendMessageWide_INCLUDED
#undef SendMessageWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LRESULT SendMessageWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return SendMessageW(hWnd, uMsg, wParam, lParam);
  else if (WideGlobal_bOldWindows == TRUE)
    return SendMessageA(hWnd, uMsg, wParam, lParam);

  WideNotInitialized();
  return 0;
}
#endif

#if defined PostMessageWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define PostMessageWide_INCLUDED
#undef PostMessageWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline LRESULT PostMessageWide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (WideGlobal_bOldWindows == FALSE)
    return PostMessageW(hWnd, uMsg, wParam, lParam);
  else if (WideGlobal_bOldWindows == TRUE)
    return PostMessageA(hWnd, uMsg, wParam, lParam);

  WideNotInitialized();
  return 0;
}
#endif

#if defined OpenEventWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define OpenEventWide_INCLUDED
#undef OpenEventWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HANDLE OpenEventWide(DWORD dwDesiredAccess, BOOL bInheritHandle, const wchar_t *wpName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return OpenEventW(dwDesiredAccess, bInheritHandle, wpName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pName=AllocAnsi(wpName);
    HANDLE hResult;

    hResult=OpenEventA(dwDesiredAccess, bInheritHandle, pName);

    FreeAnsi(pName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined CreateEventWide || defined MESSAGEWIDEFUNC || defined ALLWIDEFUNC
#define CreateEventWide_INCLUDED
#undef CreateEventWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HANDLE CreateEventWide(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, const wchar_t *wpName)
{
  if (WideGlobal_bOldWindows == FALSE)
    return CreateEventW(lpEventAttributes, bManualReset, bInitialState, wpName);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pName=AllocAnsi(wpName);
    HANDLE hResult;

    hResult=CreateEventA(lpEventAttributes, bManualReset, bInitialState, pName);

    FreeAnsi(pName);
    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif


//// Resources
#if defined LoadStringWide || defined RESOURCEWIDEFUNC || defined ALLWIDEFUNC
#define LoadStringWide_INCLUDED
#undef LoadStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int LoadStringWide(HINSTANCE hInstance, UINT uID, wchar_t *wszText, int nTextMax)
{
  if (WideGlobal_bOldWindows == FALSE)
    return LoadStringW(hInstance, uID, wszText, nTextMax);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen=0;

    if (szText=(char *)GlobalAlloc(GPTR, nTextMax * sizeof(wchar_t)))
    {
      nTextLen=LoadStringA(hInstance, uID, szText, nTextMax * sizeof(wchar_t));
      if (nTextLen=AnsiToWide(szText, nTextLen + 1, wszText, nTextMax))
        --nTextLen;
      GlobalFree((HGLOBAL)szText);
    }
    return nTextLen;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined LoadImageWide || defined RESOURCEWIDEFUNC || defined ALLWIDEFUNC
#define LoadImageWide_INCLUDED
#undef LoadImageWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HANDLE LoadImageWide(HINSTANCE hInstance, const wchar_t *wpName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    return LoadImageW(hInstance, wpName, uType, cxDesired, cyDesired, fuLoad);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    HANDLE hResult;

    if ((UINT_PTR)wpName > 0xFFFF)
    {
      char *pName=AllocAnsi(wpName);

      hResult=LoadImageA(hInstance, pName, uType, cxDesired, cyDesired, fuLoad);

      FreeAnsi(pName);
    }
    else hResult=LoadImageA(hInstance, (char *)wpName, uType, cxDesired, cyDesired, fuLoad);

    return hResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined ExtractIconExWide || defined RESOURCEWIDEFUNC || defined ALLWIDEFUNC
#define ExtractIconExWide_INCLUDED
#undef ExtractIconExWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
#ifndef PTRWIDEFUNC_INCLUDED
  #define PTRWIDEFUNC_INCLUDED
#endif
__inline UINT ExtractIconExWide(const wchar_t *wpFile, int nIconIndex, HICON *phiconLarge, HICON *phiconSmall, UINT nIcons)
{
  if (WideGlobal_bOldWindows == FALSE)
  {
    if (WideGlobal_ExtractIconExWPtr)
      return WideGlobal_ExtractIconExWPtr(wpFile, nIconIndex, phiconLarge, phiconSmall, nIcons);
  }
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (WideGlobal_ExtractIconExAPtr)
    {
      char *pFile=AllocAnsi(wpFile);
      UINT uResult;

      uResult=WideGlobal_ExtractIconExAPtr(pFile, nIconIndex, phiconLarge, phiconSmall, nIcons);

      FreeAnsi(pFile);
      return uResult;
    }
  }
  else WideNotInitialized();

  return 0;
}
#endif

#if defined IconExtractWide || defined RESOURCEWIDEFUNC || defined ALLWIDEFUNC
#define IconExtractWide_INCLUDED
#undef IconExtractWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
typedef struct {
  HICON hIcon;
  UINT nIconIndex;
  int cxDesired;
  int cyDesired;
} ICONEXTRACTDATA;

__inline HICON IconExtract_LoadA(HMODULE hModule, char *pName, int cxDesired, int cyDesired)
{
  HRSRC hRsrc;
  HGLOBAL hMemRes;
  void *lpData;
  int nIconID;

  if (!(hRsrc=FindResourceA(hModule, pName, RT_GROUP_ICON)))
    return NULL;
  if (!(hMemRes=LoadResource(hModule, hRsrc)))
    return NULL;
  if (!(lpData=LockResource(hMemRes)))
    return NULL;
  if (!(nIconID=LookupIconIdFromDirectoryEx((LPBYTE)lpData, TRUE, cxDesired, cyDesired, 0)))
    return NULL;
  if (!(hRsrc=FindResourceA(hModule, MAKEINTRESOURCEA(nIconID), RT_ICON)))
    return NULL;
  if (!(hMemRes=LoadResource(hModule, hRsrc)))
    return NULL;
  if (!(lpData=LockResource(hMemRes)))
    return NULL;
  return CreateIconFromResourceEx((PBYTE)lpData, SizeofResource(hModule, hRsrc), TRUE, 0x00030000, cxDesired, cyDesired, 0);
}

__inline BOOL CALLBACK IconExtract_EnumResNameProc(HMODULE hModule, const char *pType, char *pName, LPARAM lParam)
{
  ICONEXTRACTDATA *ied=(ICONEXTRACTDATA *)lParam;

  if (ied->nIconIndex == 0)
  {
    if (WideGlobal_bOldWindows)
      ied->hIcon=IconExtract_LoadA(hModule, pName, ied->cxDesired, ied->cyDesired);
    else
      ied->hIcon=LoadImageA(hModule, pName, IMAGE_ICON, ied->cxDesired, ied->cyDesired, 0);
    return FALSE;
  }
  --ied->nIconIndex;
  return TRUE;
}

__inline HICON IconExtractWide(const wchar_t *wpFile, UINT nIconIndex, int cxDesired, int cyDesired)
{
  ICONEXTRACTDATA ied;
  HMODULE hModule;

  ied.hIcon=NULL;
  if (!nIconIndex)
    ied.hIcon=LoadImageWide(NULL, wpFile, IMAGE_ICON, cxDesired, cyDesired, LR_LOADFROMFILE);

  if (!ied.hIcon)
  {
    //Detect Windows Vista and higher
    if (WideGlobal_bWinVista == -1)
    {
      WideGlobal_bWinVista=FALSE;
      if (!WideGlobal_bOldWindows)
      {
        OSVERSIONINFO ovi;

        ovi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
        GetVersionEx(&ovi);
        if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT)
        {
          if (ovi.dwMajorVersion >= 6)
            WideGlobal_bWinVista=TRUE;
        }
      }
    }

    //Don't use LOAD_LIBRARY_AS_IMAGE_RESOURCE if not supported, because of LoadLibraryEx error.
    if (hModule=LoadLibraryExWide(wpFile, NULL, LOAD_LIBRARY_AS_DATAFILE|(WideGlobal_bWinVista?LOAD_LIBRARY_AS_IMAGE_RESOURCE:0)))
    {
      if ((int)nIconIndex < 0)
      {
        if (WideGlobal_bOldWindows)
          ied.hIcon=IconExtract_LoadA(hModule, MAKEINTRESOURCEA(-(int)nIconIndex), cxDesired, cyDesired);
        else
          ied.hIcon=LoadImageA(hModule, MAKEINTRESOURCEA(-(int)nIconIndex), IMAGE_ICON, cxDesired, cyDesired, 0);
      }
      else
      {
        ied.nIconIndex=nIconIndex;
        ied.cxDesired=cxDesired;
        ied.cyDesired=cyDesired;
        EnumResourceNamesA(hModule, RT_GROUP_ICON, IconExtract_EnumResNameProc, (LPARAM)&ied);
      }
      FreeLibrary(hModule);
    }
  }
  return ied.hIcon;
}
#endif


//// Menus
#if defined GetMenuStringWide || defined MENUWIDEFUNC || defined ALLWIDEFUNC
#define GetMenuStringWide_INCLUDED
#undef GetMenuStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int GetMenuStringWide(HMENU hMenu, UINT uIDItem, wchar_t *wszText, int nTextMax, UINT uFlag)
{
  if (WideGlobal_bOldWindows == FALSE)
    return GetMenuStringW(hMenu, uIDItem, wszText, nTextMax, uFlag);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen=0;

    if (szText=(char *)GlobalAlloc(GPTR, nTextMax * sizeof(wchar_t)))
    {
      nTextLen=GetMenuStringA(hMenu, uIDItem, szText, nTextMax * sizeof(wchar_t), uFlag);
      if (nTextLen=AnsiToWide(szText, nTextLen + 1, wszText, nTextMax))
        --nTextLen;
      GlobalFree((HGLOBAL)szText);
    }
    return nTextLen;
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined AppendMenuWide || defined MENUWIDEFUNC || defined ALLWIDEFUNC
#define AppendMenuWide_INCLUDED
#undef AppendMenuWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL AppendMenuWide(HMENU hMenu, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *wpNewItem)
{
  if (WideGlobal_bOldWindows == FALSE)
    return AppendMenuW(hMenu, uFlags, uIDNewItem, wpNewItem);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if ((uFlags & MF_BITMAP) || (uFlags & MF_OWNERDRAW))
      return AppendMenuA(hMenu, uFlags, uIDNewItem, (char *)wpNewItem);
    else
    {
      char *pNewItem=AllocAnsi(wpNewItem);
      BOOL bResult;

      bResult=AppendMenuA(hMenu, uFlags, uIDNewItem, pNewItem);

      FreeAnsi(pNewItem);
      return bResult;
    }
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined InsertMenuWide || defined MENUWIDEFUNC || defined ALLWIDEFUNC
#define InsertMenuWide_INCLUDED
#undef InsertMenuWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL InsertMenuWide(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *wpNewItem)
{
  if (WideGlobal_bOldWindows == FALSE)
    return InsertMenuW(hMenu, uPosition, uFlags, uIDNewItem, wpNewItem);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if ((uFlags & MF_BITMAP) || (uFlags & MF_OWNERDRAW))
      return InsertMenuA(hMenu, uPosition, uFlags, uIDNewItem, (char *)wpNewItem);
    else
    {
      char *pNewItem=AllocAnsi(wpNewItem);
      BOOL bResult;

      bResult=InsertMenuA(hMenu, uPosition, uFlags, uIDNewItem, pNewItem);

      FreeAnsi(pNewItem);
      return bResult;
    }
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined ModifyMenuWide || defined MENUWIDEFUNC || defined ALLWIDEFUNC
#define ModifyMenuWide_INCLUDED
#undef ModifyMenuWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL ModifyMenuWide(HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *wpNewItem)
{
  if (WideGlobal_bOldWindows == FALSE)
    return ModifyMenuW(hMenu, uPosition, uFlags, uIDNewItem, wpNewItem);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if ((uFlags & MF_BITMAP) || (uFlags & MF_OWNERDRAW))
      return ModifyMenuA(hMenu, uPosition, uFlags, uIDNewItem, (char *)wpNewItem);
    else
    {
      char *pNewItem=AllocAnsi(wpNewItem);
      BOOL bResult;

      bResult=ModifyMenuA(hMenu, uPosition, uFlags, uIDNewItem, pNewItem);

      FreeAnsi(pNewItem);
      return bResult;
    }
  }

  WideNotInitialized();
  return FALSE;
}
#endif


//// Controls
#if defined ListView_GetColumnWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListView_GetColumnWide_INCLUDED
#undef ListView_GetColumnWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL ListView_GetColumnWide(HWND hWnd, int iCol, LVCOLUMNW *lvcW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (BOOL)SendMessageW(hWnd, LVM_GETCOLUMNW, (WPARAM)iCol, (LPARAM)lvcW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (lvcW->mask & LVCF_TEXT)
    {
      wchar_t *wpSaveText=lvcW->pszText;
      int nSaveTextMax=lvcW->cchTextMax;
      BOOL bResult=FALSE;

      lvcW->cchTextMax*=sizeof(wchar_t);
      if (lvcW->pszText=(wchar_t *)GlobalAlloc(GPTR, lvcW->cchTextMax))
      {
        bResult=(BOOL)SendMessageA(hWnd, LVM_GETCOLUMNA, (WPARAM)iCol, (LPARAM)lvcW);
        AnsiToWide((char *)lvcW->pszText, -1, wpSaveText, nSaveTextMax);
        GlobalFree((HGLOBAL)lvcW->pszText);
      }
      lvcW->pszText=wpSaveText;
      lvcW->cchTextMax=nSaveTextMax;
      return bResult;
    }
    return (BOOL)SendMessageA(hWnd, LVM_GETCOLUMNA, (WPARAM)iCol, (LPARAM)lvcW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined ListView_InsertColumnWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListView_InsertColumnWide_INCLUDED
#undef ListView_InsertColumnWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ListView_InsertColumnWide(HWND hWnd, int iCol, const LVCOLUMNW *lvcW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LVM_INSERTCOLUMNW, (WPARAM)iCol, (LPARAM)lvcW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (lvcW->mask & LVCF_TEXT)
    {
      LVCOLUMNA lvcA;
      int nResult;

      xmemcpy(&lvcA, lvcW, sizeof(LVCOLUMNA));
      lvcA.pszText=AllocAnsi(lvcW->pszText);
      nResult=(int)SendMessageA(hWnd, LVM_INSERTCOLUMNA, (WPARAM)iCol, (LPARAM)&lvcA);

      FreeAnsi((char *)lvcA.pszText);
      return nResult;
    }
    return (int)SendMessageA(hWnd, LVM_INSERTCOLUMNA, (WPARAM)iCol, (LPARAM)lvcW);
  }

  WideNotInitialized();
  return -1;
}
#endif

#if defined ListView_SetColumnWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListView_SetColumnWide_INCLUDED
#undef ListView_SetColumnWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL ListView_SetColumnWide(HWND hWnd, int iCol, const LVCOLUMNW *lvcW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LVM_SETCOLUMNW, (WPARAM)iCol, (LPARAM)lvcW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (lvcW->mask & LVCF_TEXT)
    {
      LVCOLUMNA lvcA;
      BOOL bResult;

      xmemcpy(&lvcA, lvcW, sizeof(LVCOLUMNA));
      lvcA.pszText=AllocAnsi(lvcW->pszText);
      bResult=(int)SendMessageA(hWnd, LVM_SETCOLUMNA, (WPARAM)iCol, (LPARAM)&lvcA);

      FreeAnsi((char *)lvcA.pszText);
      return bResult;
    }
    return (int)SendMessageA(hWnd, LVM_SETCOLUMNA, (WPARAM)iCol, (LPARAM)lvcW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined ListView_GetItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListView_GetItemWide_INCLUDED
#undef ListView_GetItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL ListView_GetItemWide(HWND hWnd, LVITEMW *lviW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (BOOL)SendMessageW(hWnd, LVM_GETITEMW, 0, (LPARAM)lviW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (lviW->mask & LVIF_TEXT)
    {
      wchar_t *wpSaveText=lviW->pszText;
      int nSaveTextMax=lviW->cchTextMax;
      BOOL bResult=FALSE;

      lviW->cchTextMax*=sizeof(wchar_t);
      if (lviW->pszText=(wchar_t *)GlobalAlloc(GPTR, lviW->cchTextMax))
      {
        bResult=(BOOL)SendMessageA(hWnd, LVM_GETITEMA, 0, (LPARAM)lviW);
        AnsiToWide((char *)lviW->pszText, -1, wpSaveText, nSaveTextMax);
        GlobalFree((HGLOBAL)lviW->pszText);
      }
      lviW->pszText=wpSaveText;
      lviW->cchTextMax=nSaveTextMax;
      return bResult;
    }
    return (BOOL)SendMessageA(hWnd, LVM_GETITEMA, 0, (LPARAM)lviW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined ListView_InsertItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListView_InsertItemWide_INCLUDED
#undef ListView_InsertItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ListView_InsertItemWide(HWND hWnd, const LVITEMW *lviW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LVM_INSERTITEMW, 0, (LPARAM)lviW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (lviW->mask & LVIF_TEXT)
    {
      LVITEMA lviA;
      int nResult;

      xmemcpy(&lviA, lviW, sizeof(LVITEMA));
      lviA.pszText=AllocAnsi(lviW->pszText);
      nResult=(int)SendMessageA(hWnd, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      FreeAnsi((char *)lviA.pszText);
      return nResult;
    }
    return (int)SendMessageA(hWnd, LVM_INSERTITEMA, 0, (LPARAM)lviW);
  }

  WideNotInitialized();
  return -1;
}
#endif

#if defined ListView_SetItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListView_SetItemWide_INCLUDED
#undef ListView_SetItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL ListView_SetItemWide(HWND hWnd, const LVITEMW *lviW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LVM_SETITEMW, 0, (LPARAM)lviW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (lviW->mask & LVIF_TEXT)
    {
      LVITEMA lviA;
      BOOL bResult;

      xmemcpy(&lviA, lviW, sizeof(LVITEMA));
      lviA.pszText=AllocAnsi(lviW->pszText);
      bResult=(int)SendMessageA(hWnd, LVM_SETITEMA, 0, (LPARAM)&lviA);

      FreeAnsi((char *)lviA.pszText);
      return bResult;
    }
    return (int)SendMessageA(hWnd, LVM_SETITEMA, 0, (LPARAM)lviW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined TreeView_GetItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define TreeView_GetItemWide_INCLUDED
#undef TreeView_GetItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL TreeView_GetItemWide(HWND hWnd, TVITEMW *tviW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (BOOL)SendMessageW(hWnd, TVM_GETITEMW, 0, (LPARAM)tviW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (tviW->mask & TVIF_TEXT)
    {
      wchar_t *wpSaveText=tviW->pszText;
      int nSaveTextMax=tviW->cchTextMax;
      BOOL bResult=FALSE;

      tviW->cchTextMax*=sizeof(wchar_t);
      if (tviW->pszText=(wchar_t *)GlobalAlloc(GPTR, tviW->cchTextMax))
      {
        bResult=(BOOL)SendMessageA(hWnd, TVM_GETITEMA, 0, (LPARAM)tviW);
        AnsiToWide((char *)tviW->pszText, -1, wpSaveText, nSaveTextMax);
        GlobalFree((HGLOBAL)tviW->pszText);
      }
      tviW->pszText=wpSaveText;
      tviW->cchTextMax=nSaveTextMax;
      return bResult;
    }
    return (BOOL)SendMessageA(hWnd, TVM_GETITEMA, 0, (LPARAM)tviW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined TreeView_InsertItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define TreeView_InsertItemWide_INCLUDED
#undef TreeView_InsertItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline HTREEITEM TreeView_InsertItemWide(HWND hWnd, TVINSERTSTRUCTW *tvisW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (HTREEITEM)SendMessageW(hWnd, TVM_INSERTITEMW, 0, (LPARAM)tvisW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (tvisW->item.mask & TVIF_TEXT)
    {
      TVINSERTSTRUCTA tvisA;
      HTREEITEM hResult;

      xmemcpy(&tvisA, tvisW, sizeof(TVINSERTSTRUCTA));
      tvisA.item.pszText=AllocAnsi(tvisW->item.pszText);
      hResult=(HTREEITEM)SendMessageA(hWnd, TVM_INSERTITEMA, 0, (LPARAM)&tvisA);

      FreeAnsi((char *)tvisA.item.pszText);
      return hResult;
    }
    return (HTREEITEM)SendMessageA(hWnd, TVM_INSERTITEMA, 0, (LPARAM)tvisW);
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined TreeView_SetItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define TreeView_SetItemWide_INCLUDED
#undef TreeView_SetItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL TreeView_SetItemWide(HWND hWnd, const TVITEMW *tviW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (BOOL)SendMessageW(hWnd, TVM_SETITEMW, 0, (LPARAM)tviW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (tviW->mask & TVIF_TEXT)
    {
      TVITEMA tviA;
      BOOL bResult;

      xmemcpy(&tviA, tviW, sizeof(TVITEMA));
      tviA.pszText=AllocAnsi(tviW->pszText);
      bResult=(BOOL)SendMessageA(hWnd, TVM_SETITEMA, 0, (LPARAM)&tviA);

      FreeAnsi((char *)tviA.pszText);
      return bResult;
    }
    return (BOOL)SendMessageA(hWnd, TVM_SETITEMA, 0, (LPARAM)tviW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined ComboBox_AddStringWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ComboBox_AddStringWide_INCLUDED
#undef ComboBox_AddStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ComboBox_AddStringWide(HWND hWnd, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return CB_ERR;
}
#endif

#if defined ComboBox_InsertStringWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ComboBox_InsertStringWide_INCLUDED
#undef ComboBox_InsertStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ComboBox_InsertStringWide(HWND hWnd, int nIndex, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, CB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, CB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return CB_ERR;
}
#endif

#if defined ComboBox_FindStringExactWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ComboBox_FindStringExactWide_INCLUDED
#undef ComboBox_FindStringExactWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ComboBox_FindStringExactWide(HWND hWnd, int nIndex, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, CB_FINDSTRINGEXACT, (WPARAM)nIndex, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, CB_FINDSTRINGEXACT, (WPARAM)nIndex, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return CB_ERR;
}
#endif

#if defined ComboBox_FindStringWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ComboBox_FindStringWide_INCLUDED
#undef ComboBox_FindStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ComboBox_FindStringWide(HWND hWnd, int nIndex, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, CB_FINDSTRING, (WPARAM)nIndex, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, CB_FINDSTRING, (WPARAM)nIndex, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return CB_ERR;
}
#endif

#if defined ComboBox_GetLBTextLenWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ComboBox_GetLBTextLenWide_INCLUDED
#undef ComboBox_GetLBTextLenWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ComboBox_GetLBTextLenWide(HWND hWnd, int nIndex)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, CB_GETLBTEXTLEN, (WPARAM)nIndex, 0);
  else if (WideGlobal_bOldWindows == TRUE)
    return (int)SendMessageA(hWnd, CB_GETLBTEXTLEN, (WPARAM)nIndex, 0);

  WideNotInitialized();
  return CB_ERR;
}
#endif

#if defined ComboBox_GetLBTextWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ComboBox_GetLBTextWide_INCLUDED
#undef ComboBox_GetLBTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ComboBox_GetLBTextWide(HWND hWnd, int nIndex, wchar_t *wszText)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)wszText);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen;
    int nResult=CB_ERR;

    if ((nTextLen=(int)SendMessageA(hWnd, CB_GETLBTEXTLEN, (WPARAM)nIndex, 0)) != CB_ERR)
    {
      if (szText=(char *)GlobalAlloc(GPTR, nTextLen + 1))
      {
        if ((nResult=(int)SendMessageA(hWnd, CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)szText)) != CB_ERR)
        {
          if (nResult=AnsiToWide(szText, nResult + 1, wszText, nResult + 1))
            --nResult;
        }
        GlobalFree((HGLOBAL)szText);
      }
    }
    return nResult;
  }

  WideNotInitialized();
  return CB_ERR;
}
#endif

#if defined ListBox_AddStringWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListBox_AddStringWide_INCLUDED
#undef ListBox_AddStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ListBox_AddStringWide(HWND hWnd, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LB_ADDSTRING, 0, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return LB_ERR;
}
#endif

#if defined ListBox_InsertStringWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListBox_InsertStringWide_INCLUDED
#undef ListBox_InsertStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ListBox_InsertStringWide(HWND hWnd, int nIndex, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, LB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return LB_ERR;
}
#endif

#if defined ListBox_FindStringExactWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListBox_FindStringExactWide_INCLUDED
#undef ListBox_FindStringExactWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ListBox_FindStringExactWide(HWND hWnd, int nIndex, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LB_FINDSTRINGEXACT, (WPARAM)nIndex, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, LB_FINDSTRINGEXACT, (WPARAM)nIndex, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return LB_ERR;
}
#endif

#if defined ListBox_FindStringWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListBox_FindStringWide_INCLUDED
#undef ListBox_FindStringWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ListBox_FindStringWide(HWND hWnd, int nIndex, const wchar_t *wpString)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LB_FINDSTRING, (WPARAM)nIndex, (LPARAM)wpString);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pString=AllocAnsi(wpString);
    int nResult;

    nResult=(int)SendMessageA(hWnd, LB_FINDSTRING, (WPARAM)nIndex, (LPARAM)pString);

    FreeAnsi(pString);
    return nResult;
  }

  WideNotInitialized();
  return LB_ERR;
}
#endif

#if defined ListBox_GetTextWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define ListBox_GetTextWide_INCLUDED
#undef ListBox_GetTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int ListBox_GetTextWide(HWND hWnd, int nIndex, wchar_t *wszText)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, LB_GETTEXT, (WPARAM)nIndex, (LPARAM)wszText);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen;
    int nResult=LB_ERR;

    if ((nTextLen=(int)SendMessageA(hWnd, LB_GETTEXTLEN, (WPARAM)nIndex, 0)) != LB_ERR)
    {
      if (szText=(char *)GlobalAlloc(GPTR, nTextLen + 1))
      {
        if ((nResult=(int)SendMessageA(hWnd, LB_GETTEXT, (WPARAM)nIndex, (LPARAM)szText)) != LB_ERR)
        {
          if (nResult=AnsiToWide(szText, nResult + 1, wszText, nResult + 1))
            --nResult;
        }
        GlobalFree((HGLOBAL)szText);
      }
    }
    return nResult;
  }

  WideNotInitialized();
  return LB_ERR;
}
#endif

#if defined TabCtrl_GetItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define TabCtrl_GetItemWide_INCLUDED
#undef TabCtrl_GetItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL TabCtrl_GetItemWide(HWND hWnd, int nIndex, TCITEMW *tciW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (BOOL)SendMessageW(hWnd, TCM_GETITEMW, (WPARAM)nIndex, (LPARAM)tciW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (tciW->mask & TCIF_TEXT)
    {
      wchar_t *wpSaveText=tciW->pszText;
      int nSaveTextMax=tciW->cchTextMax;
      BOOL bResult=FALSE;

      tciW->cchTextMax*=sizeof(wchar_t);
      if (tciW->pszText=(wchar_t *)GlobalAlloc(GPTR, tciW->cchTextMax))
      {
        bResult=(BOOL)SendMessageA(hWnd, TCM_GETITEMA, (WPARAM)nIndex, (LPARAM)tciW);
        AnsiToWide((char *)tciW->pszText, -1, wpSaveText, nSaveTextMax);
        GlobalFree((HGLOBAL)tciW->pszText);
      }
      tciW->pszText=wpSaveText;
      tciW->cchTextMax=nSaveTextMax;
      return bResult;
    }
    return (BOOL)SendMessageA(hWnd, TCM_GETITEMA, (WPARAM)nIndex, (LPARAM)tciW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined TabCtrl_InsertItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define TabCtrl_InsertItemWide_INCLUDED
#undef TabCtrl_InsertItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline int TabCtrl_InsertItemWide(HWND hWnd, int nIndex, const TCITEMW *tciW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (int)SendMessageW(hWnd, TCM_INSERTITEMW, (WPARAM)nIndex, (LPARAM)tciW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (tciW->mask & TCIF_TEXT)
    {
      TCITEMA tciA;
      int nResult;

      xmemcpy(&tciA, tciW, sizeof(TCITEMA));
      tciA.pszText=AllocAnsi(tciW->pszText);
      nResult=(int)SendMessageA(hWnd, TCM_INSERTITEMA, (WPARAM)nIndex, (LPARAM)&tciA);

      FreeAnsi((char *)tciA.pszText);
      return nResult;
    }
    return (int)SendMessageA(hWnd, TCM_INSERTITEMA, (WPARAM)nIndex, (LPARAM)tciW);
  }

  WideNotInitialized();
  return -1;
}
#endif

#if defined TabCtrl_SetItemWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define TabCtrl_SetItemWide_INCLUDED
#undef TabCtrl_SetItemWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL TabCtrl_SetItemWide(HWND hWnd, int nIndex, const TCITEMW *tciW)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (BOOL)SendMessageW(hWnd, TCM_SETITEMW, (WPARAM)nIndex, (LPARAM)tciW);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    if (tciW->mask & TCIF_TEXT)
    {
      TCITEMA tciA;
      BOOL bResult;

      xmemcpy(&tciA, tciW, sizeof(TCITEMA));
      tciA.pszText=AllocAnsi(tciW->pszText);
      bResult=(BOOL)SendMessageA(hWnd, TCM_SETITEMA, (WPARAM)nIndex, (LPARAM)&tciA);

      FreeAnsi((char *)tciA.pszText);
      return bResult;
    }
    return (BOOL)SendMessageA(hWnd, TCM_SETITEMA, (WPARAM)nIndex, (LPARAM)tciW);
  }

  WideNotInitialized();
  return FALSE;
}
#endif

#if defined StatusBar_GetTextWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define StatusBar_GetTextWide_INCLUDED
#undef StatusBar_GetTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline DWORD StatusBar_GetTextWide(HWND hWnd, int iPart, wchar_t *wszText)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (DWORD)SendMessageW(hWnd, SB_GETTEXTW, (WPARAM)iPart, (LPARAM)wszText);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *szText;
    int nTextLen;
    DWORD dwResult=0;

    nTextLen=(int)SendMessageA(hWnd, SB_GETTEXTLENGTH, (WPARAM)iPart, 0);
    nTextLen=LOWORD(nTextLen);

    if (szText=(char *)GlobalAlloc(GPTR, nTextLen + 1))
    {
      dwResult=(DWORD)SendMessageA(hWnd, SB_GETTEXTA, (WPARAM)iPart, (LPARAM)szText);
      AnsiToWide(szText, nTextLen + 1, wszText, nTextLen + 1);
      GlobalFree((HGLOBAL)szText);
    }
    return dwResult;
  }

  WideNotInitialized();
  return 0;
}
#endif

#if defined StatusBar_SetTextWide || defined CONTROLWIDEFUNC || defined ALLWIDEFUNC
#define StatusBar_SetTextWide_INCLUDED
#undef StatusBar_SetTextWide
#ifndef ANYWIDEFUNC_INCLUDED
  #define ANYWIDEFUNC_INCLUDED
#endif
__inline BOOL StatusBar_SetTextWide(HWND hWnd, int iPart, const wchar_t *wpText)
{
  if (WideGlobal_bOldWindows == FALSE)
    return (BOOL)SendMessageW(hWnd, SB_SETTEXTW, (WPARAM)iPart, (LPARAM)wpText);
  else if (WideGlobal_bOldWindows == TRUE)
  {
    char *pText=AllocAnsi(wpText);
    BOOL bResult;

    bResult=(BOOL)SendMessageA(hWnd, SB_SETTEXTA, (WPARAM)iPart, (LPARAM)pText);

    FreeAnsi(pText);
    return bResult;
  }

  WideNotInitialized();
  return FALSE;
}
#endif


#ifdef ANYWIDEFUNC_INCLUDED
#ifndef COMMONWIDEFUNC_INCLUDED
#define COMMONWIDEFUNC_INCLUDED

//// Global variables
__declspec(selectany) BOOL WideGlobal_bOldWindows=-1;
__declspec(selectany) BOOL WideGlobal_bWinVista=-1;
__declspec(selectany) BOOL WideGlobal_bWideInitialized=-1;
#ifdef GetCPInfoExWide_INCLUDED
  __declspec(selectany) BOOL (WINAPI *WideGlobal_GetCPInfoExAPtr)(UINT, DWORD, LPCPINFOEXA)=NULL;
  __declspec(selectany) BOOL (WINAPI *WideGlobal_GetCPInfoExWPtr)(UINT, DWORD, LPCPINFOEXW)=NULL;
#endif
#ifdef GetLongPathNameWide_INCLUDED
  __declspec(selectany) DWORD (WINAPI *WideGlobal_GetLongPathNameAPtr)(const char *, char *, DWORD)=NULL;
  __declspec(selectany) DWORD (WINAPI *WideGlobal_GetLongPathNameWPtr)(const wchar_t *, wchar_t *, DWORD)=NULL;
#endif
#ifdef ExtractIconExWide_INCLUDED
  __declspec(selectany) UINT (WINAPI *WideGlobal_ExtractIconExAPtr)(const char *, int, HICON *, HICON *, UINT)=NULL;
  __declspec(selectany) UINT (WINAPI *WideGlobal_ExtractIconExWPtr)(const wchar_t *, int, HICON *, HICON *, UINT)=NULL;
#endif
#ifdef SHGetPathFromIDListWide_INCLUDED
  __declspec(selectany) BOOL (WINAPI *WideGlobal_SHGetPathFromIDListAPtr)(LPCITEMIDLIST, char *)=NULL;
  __declspec(selectany) BOOL (WINAPI *WideGlobal_SHGetPathFromIDListWPtr)(LPCITEMIDLIST, wchar_t *)=NULL;
#endif
#ifdef SHBrowseForFolderWide_INCLUDED
  __declspec(selectany) LPITEMIDLIST (WINAPI *WideGlobal_SHBrowseForFolderAPtr)(LPBROWSEINFOA)=NULL;
  __declspec(selectany) LPITEMIDLIST (WINAPI *WideGlobal_SHBrowseForFolderWPtr)(LPBROWSEINFOW)=NULL;
#endif
#ifdef SHFileOperationWide_INCLUDED
  __declspec(selectany) int (WINAPI *WideGlobal_SHFileOperationAPtr)(LPSHFILEOPSTRUCTA)=NULL;
  __declspec(selectany) int (WINAPI *WideGlobal_SHFileOperationWPtr)(LPSHFILEOPSTRUCTW)=NULL;
#endif

//// Common
__inline void WideInitialize()
{
  if (WideGlobal_bWideInitialized <= 0)
  {
    WideGlobal_bWideInitialized=TRUE;
    WideGlobal_bOldWindows=!GetWindowsDirectoryW(NULL, 0);

    //Get functions addresses
    #ifdef PTRWIDEFUNC_INCLUDED
    {
      HMODULE hKernel32;
      HMODULE hShell32;

      hKernel32=GetModuleHandleA("kernel32.dll");
      hShell32=GetModuleHandleA("shell32.dll");

      if (WideGlobal_bOldWindows)
      {
        #ifdef GetCPInfoExWide_INCLUDED
          WideGlobal_GetCPInfoExAPtr=(BOOL (WINAPI *)(UINT, DWORD, LPCPINFOEXA))GetProcAddress(hKernel32, "GetCPInfoExA");
        #endif
        #ifdef GetLongPathNameWide_INCLUDED
          WideGlobal_GetLongPathNameAPtr=(DWORD (WINAPI *)(const char *, char *, DWORD))GetProcAddress(hKernel32, "GetLongPathNameA");
        #endif
        #ifdef ExtractIconExWide_INCLUDED
          WideGlobal_ExtractIconExAPtr=(UINT (WINAPI *)(const char *, int, HICON *, HICON *, UINT))GetProcAddress(hShell32, "ExtractIconExA");
        #endif
        #ifdef SHGetPathFromIDListWide_INCLUDED
          WideGlobal_SHGetPathFromIDListAPtr=(BOOL (WINAPI *)(LPCITEMIDLIST, char *))GetProcAddress(hShell32, "SHGetPathFromIDListA");
        #endif
        #ifdef SHBrowseForFolderWide_INCLUDED
          WideGlobal_SHBrowseForFolderAPtr=(LPITEMIDLIST (WINAPI *)(LPBROWSEINFOA))GetProcAddress(hShell32, "SHBrowseForFolderA");
        #endif
        #ifdef SHFileOperationWide_INCLUDED
          WideGlobal_SHFileOperationAPtr=(int (WINAPI *)(LPSHFILEOPSTRUCTA))GetProcAddress(hShell32, "SHFileOperationA");
        #endif
      }
      else
      {
        #ifdef GetCPInfoExWide_INCLUDED
          WideGlobal_GetCPInfoExWPtr=(BOOL (WINAPI *)(UINT, DWORD, LPCPINFOEXW))GetProcAddress(hKernel32, "GetCPInfoExW");
        #endif
        #ifdef GetLongPathNameWide_INCLUDED
          WideGlobal_GetLongPathNameWPtr=(DWORD (WINAPI *)(const wchar_t *, wchar_t *, DWORD))GetProcAddress(hKernel32, "GetLongPathNameW");
        #endif
        #ifdef ExtractIconExWide_INCLUDED
          WideGlobal_ExtractIconExWPtr=(UINT (WINAPI *)(const wchar_t *, int, HICON *, HICON *, UINT))GetProcAddress(hShell32, "ExtractIconExW");
        #endif
        #ifdef SHGetPathFromIDListWide_INCLUDED
          WideGlobal_SHGetPathFromIDListWPtr=(BOOL (WINAPI *)(LPCITEMIDLIST, wchar_t *))GetProcAddress(hShell32, "SHGetPathFromIDListW");
        #endif
        #ifdef SHBrowseForFolderWide_INCLUDED
          WideGlobal_SHBrowseForFolderWPtr=(LPITEMIDLIST (WINAPI *)(LPBROWSEINFOW))GetProcAddress(hShell32, "SHBrowseForFolderW");
        #endif
        #ifdef SHFileOperationWide_INCLUDED
          WideGlobal_SHFileOperationWPtr=(int (WINAPI *)(LPSHFILEOPSTRUCTW))GetProcAddress(hShell32, "SHFileOperationW");
        #endif
      }
    }
    #endif
  }
}

__inline void WideNotInitialized()
{
  if (WideGlobal_bWideInitialized == -1)
  {
    WideGlobal_bWideInitialized=FALSE;
    MessageBoxW(NULL, L"WideInitialize() required.", L"WideFunc.h header", MB_ICONERROR|MB_OK);
  }
}

__inline char* AllocAnsi(const wchar_t *wpWideStr)
{
  return AllocAnsiLen(wpWideStr, -1);
}

__inline char* AllocAnsiLen(const wchar_t *wpWideStr, int nWideStrLen)
{
  char *szAnsiStr;
  int nAnsiChars;

  if (wpWideStr)
  {
    if (nAnsiChars=WideCharToMultiByte(CP_ACP, 0, wpWideStr, nWideStrLen, NULL, 0, NULL, NULL))
    {
      if (szAnsiStr=(char *)GlobalAlloc(GMEM_FIXED, nAnsiChars))
      {
        if (nAnsiChars=WideCharToMultiByte(CP_ACP, 0, wpWideStr, nWideStrLen, szAnsiStr, nAnsiChars, NULL, NULL))
          szAnsiStr[nAnsiChars - 1]='\0';
        return szAnsiStr;
      }
    }
  }
  return NULL;
}

__inline void FreeAnsi(char *pAnsiBuf)
{
  if (pAnsiBuf)
    GlobalFree((HGLOBAL)pAnsiBuf);
}

__inline wchar_t* AllocWide(const char *pAnsiStr)
{
  return AllocWideLen(pAnsiStr, -1);
}

__inline wchar_t* AllocWideLen(const char *pAnsiStr, int nAnsiStrLen)
{
  wchar_t *wszWideStr;
  int nWideChars;

  if (pAnsiStr)
  {
    if (nWideChars=MultiByteToWideChar(CP_ACP, 0, pAnsiStr, nAnsiStrLen, NULL, 0))
    {
      if (wszWideStr=(wchar_t *)GlobalAlloc(GMEM_FIXED, nWideChars * sizeof(wchar_t)))
      {
        if (nWideChars=MultiByteToWideChar(CP_ACP, 0, pAnsiStr, nAnsiStrLen, wszWideStr, nWideChars))
          wszWideStr[nWideChars - 1]=L'\0';
        return wszWideStr;
      }
    }
  }
  return NULL;
}

__inline void FreeWide(wchar_t *pWideBuf)
{
  if (pWideBuf)
    GlobalFree((HGLOBAL)pWideBuf);
}

__inline int WideToAnsi(const wchar_t *wpWideStr, int nWideStrLen, char *szAnsiBuf, int nAnsiBufMax)
{
  int nAnsiChars=0;

  if (wpWideStr)
  {
    if (nAnsiChars=WideCharToMultiByte(CP_ACP, 0, wpWideStr, nWideStrLen, szAnsiBuf, nAnsiBufMax, NULL, NULL))
    {
      if (szAnsiBuf) szAnsiBuf[nAnsiChars - 1]='\0';
    }
  }
  return nAnsiChars;
}

__inline int AnsiToWide(const char *pAnsiStr, int nAnsiStrLen, wchar_t *wszWideBuf, int nWideBufMax)
{
  int nWideChars=0;

  if (pAnsiStr)
  {
    if (nWideChars=MultiByteToWideChar(CP_ACP, 0, pAnsiStr, nAnsiStrLen, wszWideBuf, nWideBufMax))
    {
      if (wszWideBuf) wszWideBuf[nWideChars - 1]=L'\0';
    }
  }
  return nWideChars;
}
#endif //COMMONWIDEFUNC_INCLUDED
#endif //ANYWIDEFUNC_INCLUDED


/********************************************************************
 *                                                                  *
 *                           Example                                *
 *                                                                  *
 ********************************************************************

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "StrFunc.h"
#include "WideFunc.h"

//Include string functions
#define xmemcpy
#include "StrFunc.h"

//Include wide functions
#define GetCurrentDirectoryWide
#define GetDateFormatWide
#include "WideFunc.h"

void main()
{
  wchar_t wszCurDir[MAX_PATH];
  wchar_t wszFormat[MAX_PATH];

  //Initialize WideFunc.h header
  WideInitialize();

  GetCurrentDirectoryWide(MAX_PATH, wszCurDir);
  MessageBoxW(NULL, wszCurDir, NULL, MB_OK);

  GetDateFormatWide(LOCALE_USER_DEFAULT, 0, NULL, NULL, wszFormat, MAX_PATH);
  MessageBoxW(NULL, wszFormat, NULL, MB_OK);
}

*/


#endif  // _WIDEFUNC_H_
