#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <richedit.h>
#include <shlobj.h>
#include <oleidl.h>
#include <richole.h>
#include "ConvFunc.h"
#include "StackFunc.h"
#include "StrFunc.h"
#include "AkelFiles\Langs\Resources\resource.h"
#include "AkelFiles\Langs\Resources\version.h"
#include "AkelFiles\Plugs\AkelDLL\AkelDLL.h"
#include "AkelPad.h"
#include "Edit.h"


//Process
extern HANDLE hHeap;
extern HANDLE hMutex;

//WinMain
extern HINSTANCE hInstance;
extern char *lpCmdLine;
extern wchar_t *wlpCmdLine;
extern DWORD dwCmdShow;
extern STARTUPINFOA lpStartupInfoA;
extern STARTUPINFOW lpStartupInfoW;
extern BOOL bNotepadCommandLine;

//Versions
extern DWORD dwExeVersion;
extern BOOL bOldWindows;
extern BOOL bOldRichEdit;
extern BOOL bOldComctl32;
extern BOOL bAkelEdit;

//Buffers
extern char buf[BUFFER_SIZE];
extern wchar_t wbuf[BUFFER_SIZE];
extern char buf2[BUFFER_SIZE];
extern wchar_t wbuf2[BUFFER_SIZE];
extern unsigned char pcTranslateBuffer[TRANSLATE_BUFFER_SIZE];

//Language
extern char szLangModule[MAX_PATH];
extern wchar_t wszLangModule[MAX_PATH];
extern int nLangModules;
extern HMODULE hLangLib;
extern DWORD dwLangSystem;
extern DWORD dwLangCodepageRecognition;

//Procedures
extern HSTACK hMainProcStack;
extern HSTACK hMainProcRetStack;
extern HSTACK hEditProcStack;
extern HSTACK hEditProcRetStack;
extern HSTACK hFrameProcStack;
extern HSTACK hFrameProcRetStack;
extern WNDPROC lpfnMainProcA;
extern WNDPROC lpfnMainProcW;
extern WNDPROCRET lpfnMainProcRetA;
extern WNDPROCRET lpfnMainProcRetW;
extern WNDPROC lpfnFrameProcA;
extern WNDPROC lpfnFrameProcW;
extern WNDPROCRET lpfnFrameProcRetA;
extern WNDPROCRET lpfnFrameProcRetW;
extern WNDPROC lpfnEditProcA;
extern WNDPROC lpfnEditProcW;
extern WNDPROCRET lpfnEditProcRetA;
extern WNDPROCRET lpfnEditProcRetW;

//Plugins
extern HSTACK hPluginsStack;
extern HSTACK hPluginListStack;
extern HSTACK hHandlesStack;
extern RECT rcPluginsInitDialog;
extern RECT rcPluginsCurrentDialog;
extern BOOL bSavePluginsStackOnExit;

//INI
extern HSTACK hIniStack;
extern char szIniFile[MAX_PATH];
extern wchar_t wszIniFile[MAX_PATH];
extern int nSaveSettings;
extern int nRegSaveSettings;

//Main Window
extern HWND hMainWnd;
extern HWND hWndEdit;
extern HWND hDummyWindow;
extern HWND hStatus;
extern HWND hDlgModeless;
extern RECT rcMainWindowRestored;
extern DWORD dwMainStyle;
extern DWORD dwLastMainSize;
extern int nStatusHeight;
extern HACCEL hGlobalAccel;
extern HACCEL hMainAccel;
extern HICON hMainIcon;
extern HICON hIconEmpty;
extern HCURSOR hCursorDragMove;
extern HBITMAP hBitmapClose;
extern HMENU hMainMenu;
extern HMENU hPopupMenu;
extern HMENU hPopupEdit;
extern HMENU hPopupView;
extern HMENU hPopupCodepage;
extern HMENU hPopupHeadline;
extern HMENU hMenuRecentFiles;
extern HMENU hMenuLanguage;
extern BOOL bMenuPopupCodepage;
extern BOOL bMenuRecentFiles;
extern BOOL bMenuLanguage;
extern BOOL bMainOnStartFinish;
extern BOOL bEditOnFinish;

//Docks
extern HDOCK hDocksStack;
extern NSIZE nsSize;
extern WNDPROC OldDockProc;
extern WNDPROC OldCloseButtonProc;

//Codepages
extern RECT rcRecodeDlg;
extern int *lpCodepageList;
extern int nCodepageListLen;
extern BOOL bDefaultBOM;
extern BOOL bCurrentBOM;
extern int nCurrentCodePage;
extern int nDefaultCodePage;
extern int nAnsiCodePage;
extern int nOemCodePage;
extern DWORD dwCodepageRecognitionBuffer;
extern BOOL bRecodeDlg;

//Recent files
extern char szCurrentFile[MAX_PATH];
extern wchar_t wszCurrentFile[MAX_PATH];
extern char (*lpszRecentNames)[MAX_PATH];
extern wchar_t (*lpwszRecentNames)[MAX_PATH];
extern DWORD *lpdwRecentPositions;
extern DWORD *lpdwRecentCodepages;
extern int nRecentFiles;
extern BOOL bSavePositions;
extern BOOL bSaveCodepages;

//Open/Save dialog
extern OPENFILENAMEA ofnA;
extern OPENFILENAMEW ofnW;
extern char szFileBuffer[FILELIST_BUFFER_SIZE];
extern wchar_t wszFileBuffer[FILELIST_BUFFER_SIZE];
extern char szFilter[MAX_PATH];
extern wchar_t wszFilter[MAX_PATH];
extern char szHomeDir[MAX_PATH];
extern wchar_t wszHomeDir[MAX_PATH];
extern BOOL bAutodetect;
extern BOOL bSaveDlg;
extern DWORD dwOfnFlags;
extern BOOL bOfnBOM;
extern int nOfnCodePage;
extern int nMsgCreate;
extern int nMsgBinary;
extern BOOL bSaveInReadOnlyMsg;
extern WNDPROC OldPreviewProc;

//Find/Replace dialog
extern RECT rcFindAndReplaceDlg;
extern DWORD ftflags;
extern BOOL bReplaceDlg;
extern BOOL bReplaceAllAndClose;
extern int nSearchStrings;
extern char *szFind_orig;
extern wchar_t *wszFind_orig;
extern char *szReplace_orig;
extern wchar_t *wszReplace_orig;
extern char *szFind;
extern wchar_t *wszFind;
extern char *szReplace;
extern wchar_t *wszReplace;
extern WNDPROC OldComboboxEdit;

//Go to line dialog
extern RECT rcGotoLineDlg;
extern int nGotoType;

//Options dialog
extern PROPSHEETHEADERA pshA;
extern PROPSHEETHEADERW pshW;
extern PROPSHEETPAGEA pspA[2];
extern PROPSHEETPAGEW pspW[2];
extern HHOOK hHookOptions;
extern BOOL bOptionsSave;
extern BOOL bOptionsRestart;

//Font/Color
extern LOGFONTA lfEditFontA;
extern LOGFONTW lfEditFontW;
extern CHOOSEFONTA cfA;
extern CHOOSEFONTW cfW;
extern CHOOSECOLORA ccA;
extern CHOOSECOLORW ccW;
extern AECOLORS aecColors;
extern COLORREF crCustColors[16];
extern BOOL bEditFontChanged;
extern BOOL bColorsChanged;

//Print
extern LOGFONTA lfPrintFontA;
extern LOGFONTW lfPrintFontW;
extern PAGESETUPDLGA psdPageA;
extern PAGESETUPDLGW psdPageW;
extern PRINTDLGA pdA;
extern PRINTDLGW pdW;
extern char szPrintHeader[MAX_PATH];
extern wchar_t wszPrintHeader[MAX_PATH];
extern char szPrintFooter[MAX_PATH];
extern wchar_t wszPrintFooter[MAX_PATH];
extern BOOL bPrintFontEnable;
extern BOOL bPrintHeaderEnable;
extern BOOL bPrintFooterEnable;
extern BOOL bGlobalPrint;
extern BOOL bPrintFontChanged;

//Edit position
extern CHARRANGE chrg;
extern int nPosition;
extern int nPositionOld;
extern int nMaxChars;
extern int nMcstep;

//Edit state
extern BOOL bModified;
extern BOOL bOvertypeMode;
extern BOOL bOvertypeModeChange;
extern int nNewLine;
extern BOOL bWordWrap;
extern BOOL bOnTop;
extern BOOL bStatusBar;
extern BOOL bReadOnly;
extern BOOL bSaveTime;
extern BOOL bWatchFile;
extern BOOL bSingleOpenFile;
extern BOOL bSingleOpenProgram;
extern BOOL bKeepSpace;
extern int nLoopCase;
extern int nTabStopSize;
extern BOOL bTabStopAsSpaces;
extern int nUndoLimit;
extern BOOL bDetailedUndo;
extern BOOL bShowURL;
extern int nClickURL;
extern DWORD dwEditMargins;
extern FILETIME ftFileTime;
extern WNDPROC OldEditProc;

//Word breaking
extern wchar_t wszWordDelimiters[DELIMITERS_SIZE];
extern BOOL bWordDelimitersEnable;
extern BOOL bFirstWordBreak;

//Execute
extern char szCommand[BUFFER_SIZE];
extern wchar_t wszCommand[BUFFER_SIZE];
extern char szWorkDir[MAX_PATH];
extern wchar_t wszWorkDir[MAX_PATH];
extern char szExeDir[MAX_PATH];
extern wchar_t wszExeDir[MAX_PATH];

//Associations
extern char szFileTypesOpen[MAX_PATH];
extern wchar_t wszFileTypesOpen[MAX_PATH];
extern char szFileTypesEdit[MAX_PATH];
extern wchar_t wszFileTypesEdit[MAX_PATH];
extern char szFileTypesPrint[MAX_PATH];
extern wchar_t wszFileTypesPrint[MAX_PATH];
extern DWORD dwFileTypesAssociated;

//OLE Drag'n'Drop
extern IRichEditOleCallbackVtbl REOLEVtbl;
extern REOLE rl;

//Mdi
extern BOOL bMDI;
extern BOOL bRegMDI;
extern HWND hMdiClient;
extern HWND hWndFrameActive;
extern HWND hWndFrameDestroyed;
extern BOOL bMdiMaximize;
extern BOOL bMdiNoWindows;
extern BOOL bDocumentReopen;
extern BOOL bMdiClientRedraw;
extern HWND hTab;
extern int nTabView;
extern int nTabType;
extern int nTabSwitch;
extern HIMAGELIST hImageList;
extern BOOL bTabPressed;
extern BOOL bFileExitError;
extern RECT rcMdiListInitDialog;
extern RECT rcMdiListCurrentDialog;
extern DWORD dwMdiStyle;
extern WNDPROC OldMdiClientProc;
extern WNDPROC OldTabProc;
extern WNDFRAMEA *lpWndFrameA;
extern WNDFRAMEW *lpWndFrameW;

//GetProcAddress
extern BOOL (WINAPI *GetCPInfoExAPtr) (UINT, DWORD, LPCPINFOEXA);
extern BOOL (WINAPI *GetCPInfoExWPtr) (UINT, DWORD, LPCPINFOEXW);
extern DWORD (WINAPI *GetLongPathNameAPtr) (char *, char *, DWORD);
extern DWORD (WINAPI *GetLongPathNameWPtr) (wchar_t *, wchar_t *, DWORD);
extern UINT (WINAPI *ExtractIconExAPtr) (char *, int, HICON *, HICON *, UINT);
extern UINT (WINAPI *ExtractIconExWPtr) (wchar_t *, int, HICON *, HICON *, UINT);
extern BOOL (WINAPI *SHGetPathFromIDListAPrt) (LPCITEMIDLIST, char *);
extern BOOL (WINAPI *SHGetPathFromIDListWPrt) (LPCITEMIDLIST, wchar_t *);
extern LPITEMIDLIST (WINAPI *SHBrowseForFolderAPrt) (LPBROWSEINFOA);
extern LPITEMIDLIST (WINAPI *SHBrowseForFolderWPrt) (LPBROWSEINFOW);


//// Init function

HWND CreateEditWindowA(HWND hWndParent)
{
  RECT rcRect;
  HWND hWndEditNew;
  int nLangOptions;
  BOOL bModifiedTmp=bModified;

  GetClientRect(hWndParent, &rcRect);

  hWndEditNew=CreateWindowExA(WS_EX_CLIENTEDGE,
                        "RichEdit20A",
                        NULL,
                        WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|
                          ES_LEFT|ES_MULTILINE|ES_DISABLENOSCROLL|
                          ES_SUNKEN|ES_NOHIDESEL,
                        0, 0, rcRect.right, rcRect.bottom - (bStatusBar?nStatusHeight:0),
                        hWndParent,
                        (HMENU)ID_EDIT,
                        hInstance,
                        NULL);

  if (!hWndEdit) hWndEdit=hWndEditNew;
  SendMessage(hWndEditNew, EM_SETOLECALLBACK, 0, (LPARAM)&rl);
  SendMessage(hWndEditNew, EM_EXLIMITTEXT, 0, nMaxChars);
  nLangOptions=SendMessage(hWndEditNew, EM_GETLANGOPTIONS, 0, 0);
  nLangOptions&=~IMF_AUTOFONT;
  nLangOptions&=~IMF_AUTOFONTSIZEADJUST;
  SendMessage(hWndEditNew, EM_SETLANGOPTIONS, 0, nLangOptions);
  SendMessage(hWndEditNew, EM_SETTEXTMODE, TM_RICHTEXT|TM_MULTILEVELUNDO|TM_MULTICODEPAGE, 0);
  SendMessage(hWndEditNew, EM_SETEVENTMASK, 0, ENM_SELCHANGE|ENM_CHANGE);
  if (bWordDelimitersEnable) SendMessage(hWndEditNew, EM_SETWORDBREAKPROC, 0, (LPARAM)EditWordBreakProc);

  DoViewWordWrap(hWndEditNew, bWordWrap, TRUE);
  DoSettingsReadOnly(hWndEditNew, bReadOnly, TRUE);
  SendMessage(hWndEditNew, EM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
  SendMessage(hWndEditNew, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
  SendMessage(hWndEditNew, EM_SETBKGNDCOLOR, 0, aecColors.crBasicBk);
  SetChosenFontA(hWndEditNew, &lfEditFontA);
  SetChosenFontColorA(hWndEditNew, aecColors.crBasicText);
  ShowURL(hWndEditNew, bShowURL);
  SetWindowTextA(hWndEditNew, "");
  SetModifyStatusA(hWndEditNew, bModifiedTmp, FALSE);

  OldEditProc=(WNDPROC)GetWindowLongA(hWndEditNew, GWL_WNDPROC);
  SetWindowLongA(hWndEditNew, GWL_WNDPROC, (LONG)CommonEditProcA);
  SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)hWndEditNew, 0);

  return hWndEditNew;
}

HWND CreateEditWindowW(HWND hWndParent)
{
  RECT rcRect;
  HWND hWndEditNew;
  int nLangOptions;
  BOOL bModifiedTmp=bModified;

  GetClientRect(hWndParent, &rcRect);

  hWndEditNew=CreateWindowExW(WS_EX_CLIENTEDGE,
                       L"RichEdit20W",
                        NULL,
                        WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|
                          ES_LEFT|ES_MULTILINE|ES_DISABLENOSCROLL|
                          ES_SUNKEN|ES_NOHIDESEL,
                        0, 0, rcRect.right, rcRect.bottom - (bStatusBar?nStatusHeight:0),
                        hWndParent,
                        (HMENU)ID_EDIT,
                        hInstance,
                        NULL);

  if (!hWndEdit) hWndEdit=hWndEditNew;
  SendMessage(hWndEditNew, EM_SETOLECALLBACK, 0, (LPARAM)&rl);
  SendMessage(hWndEditNew, EM_EXLIMITTEXT, 0, nMaxChars);
  nLangOptions=SendMessage(hWndEditNew, EM_GETLANGOPTIONS, 0, 0);
  nLangOptions&=~IMF_AUTOFONT;
  nLangOptions&=~IMF_AUTOFONTSIZEADJUST;
  SendMessage(hWndEditNew, EM_SETLANGOPTIONS, 0, nLangOptions);
  SendMessage(hWndEditNew, EM_SETTEXTMODE, TM_RICHTEXT|TM_MULTILEVELUNDO|TM_MULTICODEPAGE, 0);
  SendMessage(hWndEditNew, EM_SETEVENTMASK, 0, ENM_SELCHANGE|ENM_CHANGE);
  if (bWordDelimitersEnable) SendMessage(hWndEditNew, EM_SETWORDBREAKPROC, 0, (LPARAM)EditWordBreakProc);

  DoViewWordWrap(hWndEditNew, bWordWrap, TRUE);
  DoSettingsReadOnly(hWndEditNew, bReadOnly, TRUE);
  SendMessage(hWndEditNew, EM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
  SendMessage(hWndEditNew, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
  SendMessage(hWndEditNew, EM_SETBKGNDCOLOR, 0, aecColors.crBasicBk);
  SetChosenFontW(hWndEditNew, &lfEditFontW);
  SetChosenFontColorW(hWndEditNew, aecColors.crBasicText);
  ShowURL(hWndEditNew, bShowURL);
  SetWindowTextW(hWndEditNew, L"");
  SetModifyStatusW(hWndEditNew, bModifiedTmp, FALSE);

  OldEditProc=(WNDPROC)GetWindowLongW(hWndEditNew, GWL_WNDPROC);
  SetWindowLongW(hWndEditNew, GWL_WNDPROC, (LONG)CommonEditProcW);
  SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)hWndEditNew, 0);

  return hWndEditNew;
}


//// Menu functions

BOOL DoFileNewA()
{
  if (!bMDI)
  {
    return DoFileCloseA();
  }
  else
  {
    if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
    CreateMDIWindowA(APP_MDI_CLASSA, "", (bMdiMaximize == TRUE)?WS_MAXIMIZE:0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMdiClient, hInstance, 0);
  }
  return TRUE;
}

BOOL DoFileNewW()
{
  if (!bMDI)
  {
    return DoFileCloseW();
  }
  else
  {
    if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
    CreateMDIWindowW(APP_MDI_CLASSW, L"", (bMdiMaximize == TRUE)?WS_MAXIMIZE:0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMdiClient, hInstance, 0);
  }
  return TRUE;
}

BOOL DoFileCloseA()
{
  if (!SaveChangedA()) return FALSE;

  if (nRecentFiles && szCurrentFile[0])
  {
    RecentFilesZeroA();
    RecentFilesReadA();
    RecentFilesUpdateA(szCurrentFile, nPosition, nCurrentCodePage);
    RecentFilesSaveA();
  }

  HideCaret(NULL);
  SetWindowTextA(hWndEdit, "");
  ShowCaret(NULL);
  szCurrentFile[0]='\0';
  SetNewLineStatusA(hWndEdit, NEWLINE_WIN, FALSE);
  SetModifyStatusA(hWndEdit, FALSE, FALSE);
  SetCodePageStatusA(nDefaultCodePage, bDefaultBOM, FALSE);
  UpdateTitleA(GetParent(hWndEdit), "");

  return TRUE;
}

BOOL DoFileCloseW()
{
  if (!SaveChangedW()) return FALSE;

  if (nRecentFiles && wszCurrentFile[0])
  {
    RecentFilesZeroW();
    RecentFilesReadW();
    RecentFilesUpdateW(wszCurrentFile, nPosition, nCurrentCodePage);
    RecentFilesSaveW();
  }

  HideCaret(NULL);
  SetWindowTextW(hWndEdit, L"");
  ShowCaret(NULL);
  wszCurrentFile[0]='\0';
  SetNewLineStatusW(hWndEdit, NEWLINE_WIN, FALSE);
  SetModifyStatusW(hWndEdit, FALSE, FALSE);
  SetCodePageStatusW(nDefaultCodePage, bDefaultBOM, FALSE);
  UpdateTitleW(GetParent(hWndEdit), L"");

  return TRUE;
}

HWND DoFileNewWindowA(DWORD dwAddFlags)
{
  STARTUPINFOA siA={0};
  PROCESS_INFORMATION pi={0};
  HWND hWnd=0;

  if (!GetModuleFileNameA(hInstance, buf, MAX_PATH)) return 0;

  siA.cb=sizeof(STARTUPINFOA);
  siA.dwFlags=STARTF_USESHOWWINDOW|dwAddFlags;
  siA.wShowWindow=SW_SHOWNORMAL;

  if (CreateProcessA(buf, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &siA, &pi))
  {
    WaitForInputIdle(pi.hProcess, INFINITE);
    EnumThreadWindows(pi.dwThreadId, EnumThreadProcA, (LPARAM)&hWnd);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return hWnd;
  }
  return 0;
}

HWND DoFileNewWindowW(DWORD dwAddFlags)
{
  STARTUPINFOW siW={0};
  PROCESS_INFORMATION pi={0};
  HWND hWnd=0;

  if (!GetModuleFileNameW(hInstance, wbuf, MAX_PATH)) return 0;

  siW.cb=sizeof(STARTUPINFOW);
  siW.dwFlags=STARTF_USESHOWWINDOW|dwAddFlags;
  siW.wShowWindow=SW_SHOWNORMAL;

  if (CreateProcessW(wbuf, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &siW, &pi))
  {
    WaitForInputIdle(pi.hProcess, INFINITE);
    EnumThreadWindows(pi.dwThreadId, EnumThreadProcW, (LPARAM)&hWnd);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return hWnd;
  }
  return 0;
}

BOOL CALLBACK EnumThreadProcA(HWND hwnd, LPARAM lParam)
{
  HWND *hWnd=(HWND *)lParam;

  GetClassNameA(hwnd, buf, BUFFER_SIZE);
  if (!lstrcmpA(buf, APP_MAIN_CLASSA))
  {
    *hWnd=hwnd;
    return FALSE;
  }
  return TRUE;
}

BOOL CALLBACK EnumThreadProcW(HWND hwnd, LPARAM lParam)
{
  HWND *hWnd=(HWND *)lParam;

  GetClassNameW(hwnd, wbuf, BUFFER_SIZE);
  if (!lstrcmpW(wbuf, APP_MAIN_CLASSW))
  {
    *hWnd=hwnd;
    return FALSE;
  }
  return TRUE;
}

BOOL DoFileOpenA()
{
  if (!bMDI && !SaveChangedA()) return FALSE;

  bSaveDlg=FALSE;
  ofnA.lStructSize=sizeof(OPENFILENAMEA);
  ofnA.lpstrDefExt=NULL;
  if (!bMDI)
    ofnA.Flags&=~OFN_ALLOWMULTISELECT;
  else
    ofnA.Flags|=OFN_ALLOWMULTISELECT;
  lstrcpynA(szFileBuffer, szCurrentFile, MAX_PATH);

  if (GetOpenFileNameA(&ofnA))
  {
    GetCurrentDirectoryA(MAX_PATH, szHomeDir);
    SetCurrentDirectoryA(szExeDir);

    if (!bMDI)
    {
      if (OpenDocumentA(hWndEdit, szFileBuffer, dwOfnFlags, nOfnCodePage, bOfnBOM) < 0)
        return FALSE;
    }
    else
    {
      char szFile[MAX_PATH];
      char *pFile=szFileBuffer + lstrlenA(szFileBuffer) + 1;

      if (*pFile)
      {
        if (*(pFile - 2) == '\\') *(pFile - 2)='\0';

        do
        {
          if (IsPathFullA(pFile))
            lstrcpynA(szFile, pFile, MAX_PATH);  //.lnk target
          else
            wsprintfA(szFile, "%s\\%s", szFileBuffer, pFile);
          OpenDocumentA(hWndEdit, szFile, dwOfnFlags, nOfnCodePage, bOfnBOM);
        }
        while (*(pFile+=lstrlenA(pFile) + 1));
      }
      else
      {
        if (OpenDocumentA(hWndEdit, szFileBuffer, dwOfnFlags, nOfnCodePage, bOfnBOM) < 0)
          return FALSE;
      }
    }
    return TRUE;
  }
  return FALSE;
}

BOOL DoFileOpenW()
{
  if (!bMDI && !SaveChangedW()) return FALSE;

  bSaveDlg=FALSE;
  ofnW.lStructSize=sizeof(OPENFILENAMEW);
  ofnW.lpstrDefExt=NULL;
  if (!bMDI)
    ofnW.Flags&=~OFN_ALLOWMULTISELECT;
  else
    ofnW.Flags|=OFN_ALLOWMULTISELECT;
  lstrcpynW(wszFileBuffer, wszCurrentFile, MAX_PATH);

  if (GetOpenFileNameW(&ofnW))
  {
    GetCurrentDirectoryW(MAX_PATH, wszHomeDir);
    SetCurrentDirectoryW(wszExeDir);

    if (!bMDI)
    {
      if (OpenDocumentW(hWndEdit, wszFileBuffer, dwOfnFlags, nOfnCodePage, bOfnBOM) < 0)
        return FALSE;
    }
    else
    {
      wchar_t wszFile[MAX_PATH];
      wchar_t *wpFile=wszFileBuffer + lstrlenW(wszFileBuffer) + 1;

      if (*wpFile)
      {
        if (*(wpFile - 2) == '\\') *(wpFile - 2)='\0';

        do
        {
          if (IsPathFullW(wpFile))
            lstrcpynW(wszFile, wpFile, MAX_PATH);  //.lnk target
          else
            wsprintfW(wszFile, L"%s\\%s", wszFileBuffer, wpFile);
          OpenDocumentW(hWndEdit, wszFile, dwOfnFlags, nOfnCodePage, bOfnBOM);
        }
        while (*(wpFile+=lstrlenW(wpFile) + 1));
      }
      else
      {
        if (OpenDocumentW(hWndEdit, wszFileBuffer, dwOfnFlags, nOfnCodePage, bOfnBOM) < 0)
          return FALSE;
      }
    }
    return TRUE;
  }
  return FALSE;
}

void DoFileReopenAsA(DWORD dwFlags, int nCodePage, BOOL bBOM)
{
  int nAnswer=0;

  if (!szCurrentFile[0]) return;

  if (bModified)
  {
    API_LoadStringA(hLangLib, MSG_FILE_WILL_BE_REOPENED, buf, BUFFER_SIZE);
    nAnswer=MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OKCANCEL|MB_ICONEXCLAMATION);
  }
  if (!bModified || nAnswer == IDOK)
  {
    bDocumentReopen=TRUE;
    OpenDocumentA(hWndEdit, szCurrentFile, dwFlags, nCodePage, bBOM);
    bDocumentReopen=FALSE;
  }
}

void DoFileReopenAsW(DWORD dwFlags, int nCodePage, BOOL bBOM)
{
  int nAnswer=0;

  if (!wszCurrentFile[0]) return;

  if (bModified)
  {
    API_LoadStringW(hLangLib, MSG_FILE_WILL_BE_REOPENED, wbuf, BUFFER_SIZE);
    nAnswer=MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION);
  }
  if (!bModified || nAnswer == IDOK)
  {
    bDocumentReopen=TRUE;
    OpenDocumentW(hWndEdit, wszCurrentFile, dwFlags, nCodePage, bBOM);
    bDocumentReopen=FALSE;
  }
}

BOOL DoFileSaveA()
{
  if (!bModified && szCurrentFile[0] && FileExistsA(szCurrentFile)) return TRUE;
  if (!szCurrentFile[0]) return DoFileSaveAsA();
  return !SaveDocumentA(hWndEdit, szCurrentFile, nCurrentCodePage, bCurrentBOM, SD_UPDATE);
}

BOOL DoFileSaveW()
{
  if (!bModified && wszCurrentFile[0] && FileExistsW(wszCurrentFile)) return TRUE;
  if (!wszCurrentFile[0]) return DoFileSaveAsW();
  return !SaveDocumentW(hWndEdit, wszCurrentFile, nCurrentCodePage, bCurrentBOM, SD_UPDATE);
}

BOOL DoFileSaveAsA()
{
  char szDefaultExt[MAX_PATH];

  bSaveDlg=TRUE;
  API_LoadStringA(hLangLib, STR_DEFAULT_SAVE_EXT, szDefaultExt, MAX_PATH);

  ofnA.lStructSize=sizeof(OPENFILENAMEA);
  ofnA.lpstrDefExt=szDefaultExt;
  ofnA.Flags&=~OFN_ALLOWMULTISELECT;
  lstrcpynA(szFileBuffer, szCurrentFile, MAX_PATH);

  if (GetSaveFileNameA(&ofnA))
  {
    GetCurrentDirectoryA(MAX_PATH, szHomeDir);
    SetCurrentDirectoryA(szExeDir);

    if (!SaveDocumentA(hWndEdit, szFileBuffer, nOfnCodePage, bOfnBOM, SD_UPDATE))
      return TRUE;
  }
  return FALSE;
}

BOOL DoFileSaveAsW()
{
  wchar_t wszDefaultExt[MAX_PATH];

  bSaveDlg=TRUE;
  API_LoadStringW(hLangLib, STR_DEFAULT_SAVE_EXT, wszDefaultExt, MAX_PATH);

  ofnW.lStructSize=sizeof(OPENFILENAMEW);
  ofnW.lpstrDefExt=wszDefaultExt;
  ofnW.Flags&=~OFN_ALLOWMULTISELECT;
  lstrcpynW(wszFileBuffer, wszCurrentFile, MAX_PATH);

  if (GetSaveFileNameW(&ofnW))
  {
    GetCurrentDirectoryW(MAX_PATH, wszHomeDir);
    SetCurrentDirectoryW(wszExeDir);

    if (!SaveDocumentW(hWndEdit, wszFileBuffer, nOfnCodePage, bOfnBOM, SD_UPDATE))
      return TRUE;
  }
  return FALSE;
}

BOOL DoFilePageSetupA()
{
  BOOL bResult;

  bResult=PageSetupDlgA(&psdPageA);
  pdA.hDevMode=psdPageA.hDevMode;
  pdA.hDevNames=psdPageA.hDevNames;

  return bResult;
}

BOOL DoFilePageSetupW()
{
  BOOL bResult;

  bResult=PageSetupDlgW(&psdPageW);
  pdW.hDevMode=psdPageW.hDevMode;
  pdW.hDevNames=psdPageW.hDevNames;

  return bResult;
}

BOOL DoFilePrintA(HWND hWnd, BOOL bSilent)
{
  DOCINFOA diA={0};
  HDC hEditDC;
  TEXTMETRICA tmA;
  DRAWTEXTPARAMS dtp;
  CHARRANGE cr;
  RECT rcPage;
  RECT rcDraw;
  RECT rcHeader;
  RECT rcFooter;
  LOGFONTA lfA;
  HFONT hFont=NULL;
  HFONT hFontOld=NULL;
  char *szBuffer;
  char *pText;
  int nPointSize;
  int nPageNumber=0;
  BOOL bError=FALSE;

  if (bSilent)
  {
    GetPrinterDCA(&pdA);
    psdPageA.hDevMode=pdA.hDevMode;
    psdPageA.hDevNames=pdA.hDevNames;
  }
  else
  {
    if (chrg.cpMin == chrg.cpMax)
      pdA.Flags|=PD_NOSELECTION;
    else
      pdA.Flags&=~PD_NOSELECTION;

    if (!PrintDlgA(&pdA)) return FALSE;
    psdPageA.hDevMode=pdA.hDevMode;
    psdPageA.hDevNames=pdA.hDevNames;
  }

  GetPrintPage(pdA.hDC, &psdPageA.rtMargin, (psdPageA.Flags & PSD_INHUNDREDTHSOFMILLIMETERS), &rcPage);

  API_LoadStringA(hLangLib, STR_DOCNAME, buf, BUFFER_SIZE);
  diA.cbSize=sizeof(DOCINFOA);
  diA.lpszDocName=(szCurrentFile[0])?szCurrentFile:buf;
  if (pdA.Flags & PD_PRINTTOFILE) diA.lpszOutput="FILE:";

  if (StartDocA(pdA.hDC, &diA) > 0)
  {
    if (pdA.Flags & PD_SELECTION)
    {
      cr.cpMin=chrg.cpMin;
      cr.cpMax=chrg.cpMax;
    }
    else
    {
      cr.cpMin=0;
      cr.cpMax=-1;
    }

    if (GetRangeTextA(hWnd, cr.cpMin, cr.cpMax, &szBuffer))
    {
      pText=szBuffer;

      if (bPrintFontEnable)
      {
        hEditDC=GetDC(hWnd);
        nPointSize=-MulDiv(lfPrintFontA.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWnd, hEditDC);
        memcpy(&lfA, &lfPrintFontA, sizeof(LOGFONTA));
      }
      else
      {
        hEditDC=GetDC(hWnd);
        nPointSize=-MulDiv(lfEditFontA.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWnd, hEditDC);
        memcpy(&lfA, &lfEditFontA, sizeof(LOGFONTA));
      }
      lfA.lfHeight=-MulDiv(nPointSize, GetDeviceCaps(pdA.hDC, LOGPIXELSY), 72);
      hFont=CreateFontIndirectA(&lfA);
      hFontOld=(HFONT)SelectObject(pdA.hDC, hFont);
      GetTextMetricsA(pdA.hDC, &tmA);

      dtp.cbSize=sizeof(DRAWTEXTPARAMS);
      dtp.iTabLength=nTabStopSize;
      dtp.iLeftMargin=0;
      dtp.iRightMargin=0;
      dtp.uiLengthDrawn=0;

      rcDraw=rcPage;
      if (bPrintHeaderEnable)
      {
        rcDraw.top+=tmA.tmHeight;
        rcHeader=rcPage;
        rcHeader.bottom=rcHeader.top + tmA.tmHeight;
      }
      if (bPrintFooterEnable)
      {
        rcDraw.bottom-=tmA.tmHeight;
        rcFooter=rcPage;
        rcFooter.top=rcFooter.bottom - tmA.tmHeight;
      }

      while (*pText)
      {
        if (StartPage(pdA.hDC) > 0)
        {
          ++nPageNumber;
          SelectObject(pdA.hDC, hFont);

          if (bPrintHeaderEnable)
          {
            if (!PrintHeadlineA(pdA.hDC, &rcHeader, szPrintHeader, nPageNumber))
              bError=TRUE;
          }
          if (!PrintTextA(pdA.hDC, &rcDraw, &tmA, &dtp, pText, &pText))
          {
            bError=TRUE;
          }
          if (bPrintFooterEnable)
          {
            if (!PrintHeadlineA(pdA.hDC, &rcFooter, szPrintFooter, nPageNumber))
              bError=TRUE;
          }

          if (EndPage(pdA.hDC) <= 0)
            bError=TRUE;
        }
        else bError=TRUE;

        if (bError) break;
      }
      if (hFontOld) SelectObject(pdA.hDC, hFontOld);
      DeleteObject(hFont);
      FreeText(szBuffer);
    }
    EndDoc(pdA.hDC);
  }
  else bError=TRUE;

  if (bError)
  {
    API_LoadStringA(hLangLib, MSG_ERROR_PRINT, buf, BUFFER_SIZE);
    MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
  }
  DeleteDC(pdA.hDC);

  return !bError;
}

BOOL DoFilePrintW(HWND hWnd, BOOL bSilent)
{
  DOCINFOW diW={0};
  HDC hEditDC;
  TEXTMETRICW tmW;
  DRAWTEXTPARAMS dtp;
  CHARRANGE cr;
  RECT rcPage;
  RECT rcDraw;
  RECT rcHeader;
  RECT rcFooter;
  LOGFONTW lfW;
  HFONT hFont=NULL;
  HFONT hFontOld=NULL;
  wchar_t *wszBuffer;
  wchar_t *wpText;
  int nPointSize;
  int nPageNumber=0;
  BOOL bError=FALSE;

  if (bSilent)
  {
    GetPrinterDCW(&pdW);
    psdPageW.hDevMode=pdW.hDevMode;
    psdPageW.hDevNames=pdW.hDevNames;
  }
  else
  {
    if (chrg.cpMin == chrg.cpMax)
      pdW.Flags|=PD_NOSELECTION;
    else
      pdW.Flags&=~PD_NOSELECTION;

    if (!PrintDlgW(&pdW)) return FALSE;
    psdPageW.hDevMode=pdW.hDevMode;
    psdPageW.hDevNames=pdW.hDevNames;
  }

  GetPrintPage(pdW.hDC, &psdPageW.rtMargin, (psdPageW.Flags & PSD_INHUNDREDTHSOFMILLIMETERS), &rcPage);

  API_LoadStringW(hLangLib, STR_DOCNAME, wbuf, BUFFER_SIZE);
  diW.cbSize=sizeof(DOCINFOW);
  diW.lpszDocName=(wszCurrentFile[0])?wszCurrentFile:wbuf;
  if (pdW.Flags & PD_PRINTTOFILE) diW.lpszOutput=L"FILE:";

  if (StartDocW(pdW.hDC, &diW) > 0)
  {
    if (pdW.Flags & PD_SELECTION)
    {
      cr.cpMin=chrg.cpMin;
      cr.cpMax=chrg.cpMax;
    }
    else
    {
      cr.cpMin=0;
      cr.cpMax=-1;
    }

    if (GetRangeTextW(hWnd, cr.cpMin, cr.cpMax, &wszBuffer))
    {
      wpText=wszBuffer;

      if (bPrintFontEnable)
      {
        hEditDC=GetDC(hWnd);
        nPointSize=-MulDiv(lfPrintFontW.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWnd, hEditDC);
        memcpy(&lfW, &lfPrintFontW, sizeof(LOGFONTW));
      }
      else
      {
        hEditDC=GetDC(hWnd);
        nPointSize=-MulDiv(lfEditFontW.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWnd, hEditDC);
        memcpy(&lfW, &lfEditFontW, sizeof(LOGFONTW));
      }
      lfW.lfHeight=-MulDiv(nPointSize, GetDeviceCaps(pdW.hDC, LOGPIXELSY), 72);
      hFont=CreateFontIndirectW(&lfW);
      hFontOld=(HFONT)SelectObject(pdW.hDC, hFont);
      GetTextMetricsW(pdW.hDC, &tmW);

      dtp.cbSize=sizeof(DRAWTEXTPARAMS);
      dtp.iTabLength=nTabStopSize;
      dtp.iLeftMargin=0;
      dtp.iRightMargin=0;
      dtp.uiLengthDrawn=0;

      rcDraw=rcPage;
      if (bPrintHeaderEnable)
      {
        rcDraw.top+=tmW.tmHeight;
        rcHeader=rcPage;
        rcHeader.bottom=rcHeader.top + tmW.tmHeight;
      }
      if (bPrintFooterEnable)
      {
        rcDraw.bottom-=tmW.tmHeight;
        rcFooter=rcPage;
        rcFooter.top=rcFooter.bottom - tmW.tmHeight;
      }

      while (*wpText)
      {
        if (StartPage(pdW.hDC) > 0)
        {
          ++nPageNumber;
          SelectObject(pdW.hDC, hFont);

          if (bPrintHeaderEnable)
          {
            if (!PrintHeadlineW(pdW.hDC, &rcHeader, wszPrintHeader, nPageNumber))
              bError=TRUE;
          }
          if (!PrintTextW(pdW.hDC, &rcDraw, &tmW, &dtp, wpText, &wpText))
          {
            bError=TRUE;
          }
          if (bPrintFooterEnable)
          {
            if (!PrintHeadlineW(pdW.hDC, &rcFooter, wszPrintFooter, nPageNumber))
              bError=TRUE;
          }

          if (EndPage(pdW.hDC) <= 0)
            bError=TRUE;
        }
        else bError=TRUE;

        if (bError) break;
      }
      if (hFontOld) SelectObject(pdW.hDC, hFontOld);
      DeleteObject(hFont);
      FreeText(wszBuffer);
    }
    EndDoc(pdW.hDC);
  }
  else bError=TRUE;

  if (bError)
  {
    API_LoadStringW(hLangLib, MSG_ERROR_PRINT, wbuf, BUFFER_SIZE);
    MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }
  DeleteDC(pdW.hDC);

  return !bError;
}

BOOL DoFileExitA()
{
  if (!SaveChangedA()) return FALSE;

  if (nRecentFiles && szCurrentFile[0])
  {
    RecentFilesZeroA();
    RecentFilesReadA();
    RecentFilesUpdateA(szCurrentFile, nPosition, nCurrentCodePage);
    RecentFilesSaveA();
  }
  return TRUE;
}

BOOL DoFileExitW()
{
  if (!SaveChangedW()) return FALSE;

  if (nRecentFiles && wszCurrentFile[0])
  {
    RecentFilesZeroW();
    RecentFilesReadW();
    RecentFilesUpdateW(wszCurrentFile, nPosition, nCurrentCodePage);
    RecentFilesSaveW();
  }
  return TRUE;
}

void DoEditUndo(HWND hWnd)
{
  SendMessage(hWnd, EM_UNDO, 0, 0);
}

void DoEditRedo(HWND hWnd)
{
  SendMessage(hWnd, EM_REDO, 0, 0);
}

void DoEditCutA(HWND hWnd)
{
  if (IsReadOnly()) return;

  DoEditCopy(hWnd);
  DoEditClearA(hWnd);
}

void DoEditCutW(HWND hWnd)
{
  if (IsReadOnly()) return;

  DoEditCopy(hWnd);
  DoEditClearW(hWnd);
}

void DoEditCopy(HWND hWnd)
{
  HGLOBAL hDataTargetA=NULL;
  HGLOBAL hDataTargetW=NULL;
  LPVOID pDataTargetA;
  LPVOID pDataTargetW;
  CHARRANGE cr;
  wchar_t *wszSelection;
  wchar_t *wpSourceCount;
  wchar_t *wpTargetCount;
  int nUnicodeLen;
  int nAnsiLen;
  int nNewLine=0;

  SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);

  if (cr.cpMin < cr.cpMax)
  {
    if (wszSelection=GetSelTextW(hWnd, NULL))
    {
      if (OpenClipboard(hMainWnd))
      {
        EmptyClipboard();

        //Count new lines
        for (wpSourceCount=wszSelection; *wpSourceCount; ++wpSourceCount)
        {
          if (*wpSourceCount == '\r')
            ++nNewLine;
        }
        nUnicodeLen=(cr.cpMax - cr.cpMin) + 1;
        nUnicodeLen+=nNewLine;

        //Unicode
        if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, nUnicodeLen * sizeof(wchar_t)))
        {
          if (pDataTargetW=GlobalLock(hDataTargetW))
          {
            //Copy text with \r\n new lines
            wpTargetCount=(wchar_t *)pDataTargetW;
            wpSourceCount=wszSelection;

            while (*wpSourceCount)
            {
              if (*wpSourceCount == '\r')
              {
                *wpTargetCount++=*wpSourceCount++;
                *wpTargetCount++='\n';
              }
              else *wpTargetCount++=*wpSourceCount++;
            }
            *wpTargetCount='\0';

            //Free selection
            FreeText(wszSelection);
            wszSelection=NULL;

            //ANSI
            nAnsiLen=WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, nUnicodeLen, NULL, 0, NULL, NULL);

            if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, nAnsiLen))
            {
              if (pDataTargetA=GlobalLock(hDataTargetA))
              {
                WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, nUnicodeLen, (char *)pDataTargetA, nAnsiLen, NULL, NULL);
                GlobalUnlock(hDataTargetA);
              }
            }
            GlobalUnlock(hDataTargetW);
          }
        }
        if (hDataTargetW) SetClipboardData(CF_UNICODETEXT, hDataTargetW);
        if (hDataTargetA) SetClipboardData(CF_TEXT, hDataTargetA);
        CloseClipboard();
      }
      if (wszSelection) FreeText(wszSelection);
    }
  }
}

BOOL DoEditPaste(HWND hWnd, BOOL bAnsi)
{
  HGLOBAL hData;
  LPVOID pData;
  BOOL bResult=FALSE;

  if (IsReadOnly()) return FALSE;

  if (OpenClipboard(hWnd))
  {
    if (!bAnsi && (hData=GetClipboardData(CF_UNICODETEXT)))
    {
      if (pData=GlobalLock(hData))
      {
        ReplaceSelW(hWnd, (wchar_t *)pData, -1);
        bResult=TRUE;
        GlobalUnlock(hData);
      }
    }
    else if (hData=GetClipboardData(CF_TEXT))
    {
      if (pData=GlobalLock(hData))
      {
        ReplaceSelA(hWnd, (char *)pData, -1);
        bResult=TRUE;
        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
  return bResult;
}

void DoEditClearA(HWND hWnd)
{
  if (IsReadOnly()) return;

  SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"");
}

void DoEditClearW(HWND hWnd)
{
  if (IsReadOnly()) return;

  SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"");
}

void DoEditSelectAll(HWND hWnd)
{
  SetTextSel(hWnd, 0, -1);
}

void DoEditInsertChar()
{
  ShellExecuteA(hMainWnd, "open", "charmap.exe", NULL, NULL, SW_SHOWDEFAULT);
}

void DoEditInsertDateA(HWND hWnd)
{
  char szTime[128];
  char szDate[128];
  char szTimeAndDate[MAX_PATH];

  if (IsReadOnly()) return;

  if (GetTimeFormatA(LOCALE_USER_DEFAULT, TIME_NOSECONDS, 0, NULL, szTime, 128) &&
      GetDateFormatA(LOCALE_USER_DEFAULT, 0, 0, NULL, szDate, 128))
  {
    wsprintfA(szTimeAndDate, "%s %s", szTime, szDate);
    SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)szTimeAndDate);
  }
}

void DoEditInsertDateW(HWND hWnd)
{
  wchar_t wszTime[128];
  wchar_t wszDate[128];
  wchar_t wszTimeAndDate[MAX_PATH];

  if (IsReadOnly()) return;

  if (GetTimeFormatW(LOCALE_USER_DEFAULT, TIME_NOSECONDS, 0, NULL, wszTime, 128) &&
      GetDateFormatW(LOCALE_USER_DEFAULT, 0, 0, NULL, wszDate, 128))
  {
    wsprintfW(wszTimeAndDate, L"%s %s", wszTime, wszDate);
    SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)wszTimeAndDate);
  }
}

void DoEditRecodeA(HWND hWnd)
{
  if (!hDlgModeless)
  {
    if (hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_RECODE), hMainWnd, (DLGPROC)RecodeDlgProcA))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditRecodeW(HWND hWnd)
{
  if (!hDlgModeless)
  {
    if (hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_RECODE), hMainWnd, (DLGPROC)RecodeDlgProcW))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

BOOL DoEditInsertStringInSelectionW(HWND hWnd, int nAction, wchar_t *wpString)
{
  EDITSTREAM es;
  BUFFERSTREAMDATA bsd;
  CHARRANGE cr;
  RECT rcEdit;
  wchar_t *wszText=NULL;
  wchar_t *wpTextEnd;
  wchar_t *a;
  wchar_t *b;
  int nMinLine;
  int nMaxLine;
  int nMinLineIndex;
  int nMaxLineIndex;
  int nMaxLineLength;
  int nTextSize;
  int nStringsSum;
  int nStringsSize;
  int nStringLength;
  int nStringLengthW;
  int nLen;
  int nFirstLine;
  int i;
  BOOL bEndSel=FALSE;
  BOOL bResult=FALSE;

  if (!(nAction & STRSEL_CHECK) && IsReadOnly()) return FALSE;

  cr.cpMin=chrg.cpMin;
  cr.cpMax=chrg.cpMax;

  if (!bWordWrap && cr.cpMin < cr.cpMax)
  {
    nMinLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, cr.cpMin);
    nMaxLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, cr.cpMax);
    nMinLineIndex=SendMessage(hWnd, EM_LINEINDEX, nMinLine, 0);
    nMaxLineIndex=SendMessage(hWnd, EM_LINEINDEX, nMaxLine, 0);
    nMaxLineLength=SendMessage(hWnd, EM_LINELENGTH, cr.cpMax, 0);

    if (nMinLine != nMaxLine || (nMinLineIndex == cr.cpMin && !nMaxLineLength))
    {
      if (nAction & STRSEL_CHECK) return TRUE;

      if (nMaxLineIndex == cr.cpMax) --nMaxLine;
      else if (nMaxLineLength) cr.cpMax=nMaxLineIndex + nMaxLineLength + 1;
      else bEndSel=TRUE;
      cr.cpMin=nMinLineIndex;

      nStringLength=wcslen(wpString);
      nStringLengthW=nStringLength * sizeof(wchar_t);
      nStringsSum=(nMaxLine - nMinLine) + 1;
      nStringsSize=nStringsSum * nStringLength;
      nTextSize=(cr.cpMax - cr.cpMin) + 1;
      nLen=nTextSize + nStringsSize;

      SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
      SaveLineScroll(hWnd, &rcEdit, &nFirstLine);
      SetTextSel(hWnd, cr.cpMin, cr.cpMax);

      if (nAction & STRSEL_INSERT)
      {
        if (wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nLen * sizeof(wchar_t) + 2))
        {
          a=wszText + nStringLength;
          b=wszText + nStringsSize;
          memcpy(wszText, wpString, nStringLengthW);

          bsd.wpData=b;
          bsd.nDataLen=-1;
          bsd.nCount=0;
          es.dwCookie=(DWORD)&bsd;
          es.pfnCallback=GetSelTextCallbackW;
          SendMessage(hWnd, EM_STREAMOUT, SF_TEXT|SF_UNICODE|SFF_SELECTION, (LPARAM)&es);
          b[bsd.nCount]='\0';

          while (*b)
          {
            if (*b == '\r')
            {
              *a++=*b++;

              if (*b)
              {
                memcpy(a, wpString, nStringLengthW);
                a+=nStringLength;
              }
              else if (bEndSel)
              {
                memcpy(a, wpString, nStringLengthW);
                a+=nStringLength;
                break;
              }
            }
            else *a++=*b++;
          }
          *a='\0';
          bResult=TRUE;
        }
      }
      else if (nAction & STRSEL_DELETE)
      {
        if (wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nTextSize * sizeof(wchar_t) + 2))
        {
          bsd.wpData=wszText;
          bsd.nDataLen=-1;
          bsd.nCount=0;
          es.dwCookie=(DWORD)&bsd;
          es.pfnCallback=GetSelTextCallbackW;
          SendMessage(hWnd, EM_STREAMOUT, SF_TEXT|SF_UNICODE|SFF_SELECTION, (LPARAM)&es);
          wszText[bsd.nCount]='\0';

          wpTextEnd=wszText + nTextSize - 1;
          *wpTextEnd='\0';
          a=wszText;
          b=wszText;

          if (nAction & STRSEL_TAB)
          {
            if (*b == '\t')
              ++b;
            else
              for (i=0; i < nTabStopSize && *b == ' '; ++i, ++b);
          }
          else if (nAction & STRSEL_SPACE)
          {
            if (*b == ' ' || *b == '\t') ++b;
          }
          else
          {
            if (!memcmp(b, wpString, nStringLengthW)) b+=nStringLength;
          }

          while (b <= wpTextEnd)
          {
            if (*b == '\r')
            {
              *a++=*b++;

              if (nAction & STRSEL_TAB)
              {
                if (*b == '\t')
                  ++b;
                else
                  for (i=0; i < nTabStopSize && *b == ' '; ++i, ++b);
              }
              else if (nAction & STRSEL_SPACE)
              {
                if (*b == ' ' || *b == '\t') ++b;
              }
              else
              {
                if (!memcmp(b, wpString, nStringLengthW)) b+=nStringLength;
              }
            }
            else *a++=*b++;
          }
          bResult=TRUE;
        }
      }

      if (bResult)
      {
        ReplaceSelW(hWnd, wszText, -1);
        SetTextSel(hWnd, cr.cpMin, cr.cpMin);
        if (nAction & STRSEL_INSERT)
          SetTextSel(hWnd, cr.cpMin, cr.cpMin + (a - wszText));
        else if (nAction & STRSEL_DELETE)
          SetTextSel(hWnd, cr.cpMin, cr.cpMin + (a - wszText) - 1);
      }
      RestoreLineScroll(hWnd, &rcEdit, &nFirstLine);
      SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
      InvalidateRect(hWnd, NULL, FALSE);

      API_HeapFree(hHeap, 0, (LPVOID)wszText);
      return bResult;
    }
  }
  if (nAction & STRSEL_CHECK) return FALSE;
  return TRUE;
}

BOOL DoEditDeleteFirstCharW(HWND hWnd)
{
  CHARRANGE cr;
  RECT rcEdit;
  wchar_t *wszText;
  int nLen;
  int a;
  int b;
  int nFirstLine;
  BOOL bDelete;

  if (IsReadOnly()) return FALSE;

  cr.cpMin=chrg.cpMin;
  cr.cpMax=chrg.cpMax;
  nLen=(chrg.cpMax - chrg.cpMin) + 1;

  if (cr.cpMin < cr.cpMax)
  {
    if (wszText=GetSelTextW(hWnd, NULL))
    {
      if (wszText[0] != '\0')
      {
        bDelete=TRUE;
        a=0, b=0;

        while (wszText[b])
        {
          while (wszText[b] == '\r')
          {
            bDelete=TRUE;
            wszText[a++]=wszText[b++];
          }
          if (bDelete)
          {
            bDelete=FALSE;
            if (!wszText[b]) break;
            b++;
          }
          else wszText[a++]=wszText[b++];
        }
        wszText[a]='\0';
        if (++b < nLen) ++a;

        SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
        SaveLineScroll(hWnd, &rcEdit, &nFirstLine);
        ReplaceSelW(hWnd, wszText, -1);
        SetTextSel(hWnd, cr.cpMin, cr.cpMin);
        SetTextSel(hWnd, cr.cpMin, cr.cpMin + a);
        RestoreLineScroll(hWnd, &rcEdit, &nFirstLine);
        SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
        InvalidateRect(hWnd, NULL, FALSE);
      }
      FreeText(wszText);
      return TRUE;
    }
    return FALSE;
  }
  SendMessage(hWnd, WM_KEYDOWN, VK_BACK, 0);
  SendMessage(hWnd, WM_KEYUP, VK_BACK, 0);
  return TRUE;
}

BOOL DoEditDeleteTrailingWhitespacesW(HWND hWnd)
{
  CHARRANGE cr;
  RECT rcEdit;
  wchar_t *wszText;
  int nLine=0;
  int nPos=0;
  int nLen;
  int a;
  int b;
  int nFirstLine;
  BOOL bResult=FALSE;

  if (IsReadOnly()) return FALSE;

  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveLineScroll(hWnd, &rcEdit, &nFirstLine);

  cr.cpMin=chrg.cpMin;
  cr.cpMax=chrg.cpMax;
  if (cr.cpMin == cr.cpMax)
  {
    nLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, cr.cpMin);
    nPos=cr.cpMin - SendMessage(hWnd, EM_LINEINDEX, nLine, 0);
    SetTextSel(hWnd, 0, -1);
  }
  nLen=(chrg.cpMax - chrg.cpMin) + 1;

  if (wszText=GetSelTextW(hWnd, NULL))
  {
    for (a=0, b=0; b < nLen; wszText[a++]=wszText[b++])
    {
      if (wszText[b] == '\r' || wszText[b] == '\0')
      {
        while (--a >= 0 && (wszText[a] == '\t' || wszText[a] == ' '));
        ++a;

        if (wszText[b] == '\0')
        {
          wszText[a]='\0';
          if (++b < nLen) ++a;
          break;
        }
      }
    }

    ReplaceSelW(hWnd, wszText, -1);
    if (cr.cpMin == cr.cpMax)
    {
      a=SendMessage(hWnd, EM_LINEINDEX, nLine, 0);
      b=SendMessage(hWnd, EM_LINELENGTH, a, 0);
      if (nPos > b) nPos=b;
      SetTextSel(hWnd, a + nPos, a + nPos);
    }
    else
    {
      SetTextSel(hWnd, cr.cpMin, cr.cpMin);
      SetTextSel(hWnd, cr.cpMin, cr.cpMin + a);
    }

    FreeText(wszText);
    bResult=TRUE;
  }
  RestoreLineScroll(hWnd, &rcEdit, &nFirstLine);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);

  return bResult;
}

BOOL DoEditChangeCaseA(HWND hWnd, int nCase)
{
  CHARRANGE crInitial;
  CHARRANGE crRange;
  RECT rcEdit;
  char *szText;
  char *pStart;
  char *pEnd;
  int nLen;
  int nFirstLine;

  if (IsReadOnly()) return FALSE;

  crInitial.cpMin=crRange.cpMin=chrg.cpMin;
  crInitial.cpMax=crRange.cpMax=chrg.cpMax;

  if (crRange.cpMin == crRange.cpMax)
  {
    crRange.cpMin=0;
    crRange.cpMax=GetTextLength(hWnd);
  }

  if (nLen=GetRangeTextA(hWnd, crRange.cpMin, crRange.cpMax, &szText))
  {
    pStart=szText;
    pEnd=pStart + nLen;

    if (nCase == UPPERCASE)
    {
      CharUpperBuffA(szText, nLen);
    }
    else if (nCase == LOWERCASE)
    {
      CharLowerBuffA(szText, nLen);
    }
    else if (nCase == SENTENCECASE)
    {
      while (pStart < pEnd)
      {
        while (pStart < pEnd && (*pStart == ' ' || *pStart == '\t' || *pStart == '\r' || strchr(SENTENCE_DELIMITERSA, *pStart)))
          ++pStart;

        if (pStart < pEnd) *pStart++=(char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pStart);

        while (pStart < pEnd && !strchr(SENTENCE_DELIMITERSA, *pStart))
          *pStart++=(char)(WORD)(DWORD)CharLowerA((char *)(DWORD)(WORD)*pStart);
      }
    }
    else if (nCase == TITLECASE)
    {
      while (pStart < pEnd)
      {
        while (pStart < pEnd && strchr(TITLE_DELIMITERSA, *pStart))
          ++pStart;

        if (pStart < pEnd) *pStart++=(char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pStart);

        while (pStart < pEnd && !strchr(TITLE_DELIMITERSA, *pStart))
          *pStart++=(char)(WORD)(DWORD)CharLowerA((char *)(DWORD)(WORD)*pStart);
      }
    }
    else if (nCase == INVERTCASE)
    {
      while (pStart < pEnd)
      {
        if (IsCharLowerA(*pStart))
          *pStart++=(char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pStart);
        else
          *pStart++=(char)(WORD)(DWORD)CharLowerA((char *)(DWORD)(WORD)*pStart);
      }
    }
    SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
    SaveLineScroll(hWnd, &rcEdit, &nFirstLine);
    SetTextSel(hWnd, crRange.cpMin, crRange.cpMax);
    ReplaceSelA(hWnd, szText, -1);
    SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin);
    if (crInitial.cpMin != crInitial.cpMax)
      SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin + nLen - 1);
    RestoreLineScroll(hWnd, &rcEdit, &nFirstLine);
    SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWnd, NULL, FALSE);

    FreeText(szText);
    return TRUE;
  }
  return FALSE;
}

BOOL DoEditChangeCaseW(HWND hWnd, int nCase)
{
  CHARRANGE crInitial;
  CHARRANGE crRange;
  RECT rcEdit;
  wchar_t *wszText;
  wchar_t *wpStart;
  wchar_t *wpEnd;
  int nLen;
  int nFirstLine;

  if (IsReadOnly()) return FALSE;

  crInitial.cpMin=crRange.cpMin=chrg.cpMin;
  crInitial.cpMax=crRange.cpMax=chrg.cpMax;

  if (crRange.cpMin == crRange.cpMax)
  {
    crRange.cpMin=0;
    crRange.cpMax=GetTextLength(hWnd);
  }

  if (nLen=GetRangeTextW(hWnd, crRange.cpMin, crRange.cpMax, &wszText))
  {
    wpStart=wszText;
    wpEnd=wpStart + nLen;

    if (nCase == UPPERCASE)
    {
      CharUpperBuffW(wszText, nLen);
    }
    else if (nCase == LOWERCASE)
    {
      CharLowerBuffW(wszText, nLen);
    }
    else if (nCase == SENTENCECASE)
    {
      while (wpStart < wpEnd)
      {
        while (wpStart < wpEnd && (*wpStart == ' ' || *wpStart == '\t' || *wpStart == '\r' || wcschr(SENTENCE_DELIMITERSW, *wpStart)))
          ++wpStart;

        if (wpStart < wpEnd) *wpStart++=(wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpStart);

        while (wpStart < wpEnd && !wcschr(SENTENCE_DELIMITERSW, *wpStart))
          *wpStart++=(wchar_t)(WORD)(DWORD)CharLowerW((wchar_t *)(DWORD)(WORD)*wpStart);
      }
    }
    else if (nCase == TITLECASE)
    {
      while (wpStart < wpEnd)
      {
        while (wpStart < wpEnd && wcschr(TITLE_DELIMITERSW, *wpStart))
          ++wpStart;

        if (wpStart < wpEnd) *wpStart++=(wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpStart);

        while (wpStart < wpEnd && !wcschr(TITLE_DELIMITERSW, *wpStart))
          *wpStart++=(wchar_t)(WORD)(DWORD)CharLowerW((wchar_t *)(DWORD)(WORD)*wpStart);
      }
    }
    else if (nCase == INVERTCASE)
    {
      while (wpStart < wpEnd)
      {
        if (IsCharLowerW(*wpStart))
          *wpStart++=(wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpStart);
        else
          *wpStart++=(wchar_t)(WORD)(DWORD)CharLowerW((wchar_t *)(DWORD)(WORD)*wpStart);
      }
    }
    SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
    SaveLineScroll(hWnd, &rcEdit, &nFirstLine);
    SetTextSel(hWnd, crRange.cpMin, crRange.cpMax);
    ReplaceSelW(hWnd, wszText, -1);
    SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin);
    if (crInitial.cpMin != crInitial.cpMax)
      SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin + nLen - 1);
    RestoreLineScroll(hWnd, &rcEdit, &nFirstLine);
    SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWnd, NULL, FALSE);

    FreeText(wszText);
    return TRUE;
  }
  return FALSE;
}

void DoEditFindA()
{
  if (!hDlgModeless)
  {
    bReplaceDlg=FALSE;
    if ((ftflags & FR_ALLFILES) && !bMDI)
      ftflags&=~FR_ALLFILES;
    if ((ftflags & FR_SELECTION) && chrg.cpMin == chrg.cpMax)
      ftflags&=~FR_SELECTION;

    if (hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_FIND), hMainWnd, (DLGPROC)FindAndReplaceDlgProcA))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditFindW()
{
  if (!hDlgModeless)
  {
    bReplaceDlg=FALSE;
    if ((ftflags & FR_ALLFILES) && !bMDI)
      ftflags&=~FR_ALLFILES;
    if ((ftflags & FR_SELECTION) && chrg.cpMin == chrg.cpMax)
      ftflags&=~FR_SELECTION;

    if (hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_FIND), hMainWnd, (DLGPROC)FindAndReplaceDlgProcW))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditFindNextDownA(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~FR_UP & ~FR_BEGINNING & ~FR_SELECTION & ~FR_ALLFILES) | FR_DOWN;

  if (szFind)
    _FindTextA(hWnd, dwFlags, szFind);
  else
    DoEditFindA();
}

void DoEditFindNextDownW(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~FR_UP & ~FR_BEGINNING & ~FR_SELECTION & ~FR_ALLFILES) | FR_DOWN;

  if (wszFind)
    _FindTextW(hWnd, dwFlags, wszFind);
  else
    DoEditFindW();
}

void DoEditFindNextUpA(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~FR_DOWN & ~FR_BEGINNING & ~FR_SELECTION & ~FR_ALLFILES) | FR_UP;

  if (szFind)
    _FindTextA(hWnd, dwFlags, szFind);
  else
    DoEditFindA();
}

void DoEditFindNextUpW(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~FR_DOWN & ~FR_BEGINNING & ~FR_SELECTION & ~FR_ALLFILES) | FR_UP;

  if (wszFind)
    _FindTextW(hWnd, dwFlags, wszFind);
  else
    DoEditFindW();
}

void DoEditReplaceA()
{
  if (!hDlgModeless)
  {
    bReplaceDlg=TRUE;
    if ((ftflags & FR_ALLFILES) && !bMDI)
      ftflags&=~FR_ALLFILES;
    if ((ftflags & FR_SELECTION) && chrg.cpMin == chrg.cpMax)
      ftflags&=~FR_SELECTION;

    if (hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_REPLACE), hMainWnd, (DLGPROC)FindAndReplaceDlgProcA))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditReplaceW()
{
  if (!hDlgModeless)
  {
    bReplaceDlg=TRUE;
    if ((ftflags & FR_ALLFILES) && !bMDI)
      ftflags&=~FR_ALLFILES;
    if ((ftflags & FR_SELECTION) && chrg.cpMin == chrg.cpMax)
      ftflags&=~FR_SELECTION;

    if (hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_REPLACE), hMainWnd, (DLGPROC)FindAndReplaceDlgProcW))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditGoToLineA()
{
  if (!hDlgModeless)
  {
    if (hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_GOTOLINE), hMainWnd, (DLGPROC)GoToLineDlgProcA))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditGoToLineW()
{
  if (!hDlgModeless)
  {
    if (hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_GOTOLINE), hMainWnd, (DLGPROC)GoToLineDlgProcW))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

BOOL DoViewFontA(HWND hWndOwner, LOGFONTA *lfA)
{
  LOGFONTA lfTmpA;

  memcpy(&lfTmpA, lfA, sizeof(LOGFONTA));
  cfA.hwndOwner=hWndOwner;
  cfA.lpLogFont=lfA;

  if (ChooseFontA(&cfA))
  {
    lfA->lfHeight=-mod(lfA->lfHeight);
    return TRUE;
  }
  memcpy(lfA, &lfTmpA, sizeof(LOGFONTA));
  return FALSE;
}

BOOL DoViewFontW(HWND hWndOwner, LOGFONTW *lfW)
{
  LOGFONTW lfTmpW;

  memcpy(&lfTmpW, lfW, sizeof(LOGFONTW));
  cfW.hwndOwner=hWndOwner;
  cfW.lpLogFont=lfW;

  if (ChooseFontW(&cfW))
  {
    lfW->lfHeight=-mod(lfW->lfHeight);
    return TRUE;
  }
  memcpy(lfW, &lfTmpW, sizeof(LOGFONTW));
  return FALSE;
}

BOOL DoViewColorA(HWND hWndOwner, COLORREF *crColor)
{
  COLORREF crTmp=ccA.rgbResult=*crColor;

  ccA.hwndOwner=hWndOwner;

  if (ChooseColorA(&ccA))
  {
    *crColor=ccA.rgbResult;
    return TRUE;
  }
  *crColor=crTmp;
  return FALSE;
}

BOOL DoViewColorW(HWND hWndOwner, COLORREF *crColor)
{
  COLORREF crTmp=ccW.rgbResult=*crColor;

  ccW.hwndOwner=hWndOwner;

  if (ChooseColorW(&ccW))
  {
    *crColor=ccW.rgbResult;
    return TRUE;
  }
  *crColor=crTmp;
  return FALSE;
}

void DoViewFontSizeA(HWND hWnd, int nAction)
{
  if (nAction == INCREASE_FONT)
  {
    if (lfEditFontA.lfHeight <= -1)
    {
      lfEditFontA.lfHeight-=1;
      SetChosenFontSizeA(hWnd, lfEditFontA.lfHeight);
    }
  }
  else if (nAction == DECREASE_FONT)
  {
    if (lfEditFontA.lfHeight < -1)
    {
      lfEditFontA.lfHeight+=1;
      SetChosenFontSizeA(hWnd, lfEditFontA.lfHeight);
    }
  }
}

void DoViewFontSizeW(HWND hWnd, int nAction)
{
  if (nAction == INCREASE_FONT)
  {
    if (lfEditFontW.lfHeight <= -1)
    {
      lfEditFontW.lfHeight-=1;
      SetChosenFontSizeW(hWnd, lfEditFontW.lfHeight);
    }
  }
  else if (nAction == DECREASE_FONT)
  {
    if (lfEditFontW.lfHeight < -1)
    {
      lfEditFontW.lfHeight+=1;
      SetChosenFontSizeW(hWnd, lfEditFontW.lfHeight);
    }
  }
}

void DoViewAlignmentA(HWND hWnd, WORD wAlignment)
{
  CHARRANGE crInitial;
  PARAFORMAT2 pf;
  RECT rcEdit;
  int nFirstVisibleChar;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  pf.cbSize=sizeof(PARAFORMAT2);
  pf.dwMask=PFM_ALIGNMENT;
  pf.wAlignment=wAlignment;

  crInitial.cpMin=chrg.cpMin;
  crInitial.cpMax=chrg.cpMax;
  if (crInitial.cpMin == crInitial.cpMax)
    SetTextSel(hWnd, 0, -1);
  SendMessage(hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
  SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin);
  if (crInitial.cpMin != crInitial.cpMax)
    SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMax);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusA(hWndEdit, bModifiedTmp, FALSE);
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
}

void DoViewAlignmentW(HWND hWnd, WORD wAlignment)
{
  CHARRANGE crInitial;
  PARAFORMAT2 pf;
  RECT rcEdit;
  int nFirstVisibleChar;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  pf.cbSize=sizeof(PARAFORMAT2);
  pf.dwMask=PFM_ALIGNMENT;
  pf.wAlignment=wAlignment;

  crInitial.cpMin=chrg.cpMin;
  crInitial.cpMax=chrg.cpMax;
  if (crInitial.cpMin == crInitial.cpMax)
    SetTextSel(hWnd, 0, -1);
  SendMessage(hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
  SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin);
  if (crInitial.cpMin != crInitial.cpMax)
    SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMax);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusW(hWndEdit, bModifiedTmp, FALSE);
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
}

void DoViewWordWrap(HWND hWnd, BOOL bState, BOOL bFirst)
{
  NMHDR nmhdr={hWnd, ID_EDIT, EN_SELCHANGE};
  RECT rcEdit;
  int nEventMask;
  int nFirstVisibleChar;

  CheckMenuItem(hMainMenu, IDM_VIEW_WORDWRAP, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == bWordWrap) return;
  if (bFirst == TRUE && bState == FALSE) return;

  //Reserve state
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);

  if (bWordWrap=bState)
  {
    nEventMask=SendMessage(hWnd, EM_GETEVENTMASK, 0, 0);
    SendMessage(hWnd, EM_SETEVENTMASK, 0, nEventMask | ENM_REQUESTRESIZE);
    ShowScrollBar(hWnd, SB_HORZ, FALSE);
    SendMessage(hWnd, EM_SETTARGETDEVICE, (WPARAM)NULL, 0);
  }
  else
  {
    SendMessage(hWnd, EM_SETTARGETDEVICE, (WPARAM)NULL, -1);
    ShowScrollBar(hWnd, SB_HORZ, TRUE);
    nEventMask=SendMessage(hWnd, EM_GETEVENTMASK, 0, 0);
    SendMessage(hWnd, EM_SETEVENTMASK, 0, nEventMask & ~ENM_REQUESTRESIZE);
  }

  //Restore state
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
  SendMessage(hMainWnd, WM_NOTIFY, ID_EDIT, (LPARAM)&nmhdr);
}

void DoViewOnTop(BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_ONTOP, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == bOnTop) return;

  if (bOnTop=bState)
    SetWindowPos(hMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
  else
    SetWindowPos(hMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

void DoViewShowStatusBar(BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_SHOW_STATUSBAR, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == bStatusBar) return;
  bStatusBar=bState;

  ShowWindow(hStatus, bStatusBar?SW_SHOW:SW_HIDE);
  UpdateSize();
}

BOOL DoSettingsExecA()
{
  STARTUPINFOA siA={0};
  PROCESS_INFORMATION pi={0};
  char *szCommandExp;
  char *szWorkDirExp;
  int nCommandLen;
  int nWorkDirLen;
  BOOL bResult=FALSE;

  nCommandLen=TranslateFileStringA(szCommand, NULL, 0);
  nWorkDirLen=TranslateFileStringA(szWorkDir, NULL, 0);

  if (szCommandExp=(char *)API_HeapAlloc(hHeap, 0, nCommandLen + 1))
  {
    if (szWorkDirExp=(char *)API_HeapAlloc(hHeap, 0, nWorkDirLen + 1))
    {
      TranslateFileStringA(szCommand, szCommandExp, nCommandLen + 1);
      TranslateFileStringA(szWorkDir, szWorkDirExp, nWorkDirLen + 1);

      siA.cb=sizeof(STARTUPINFOA);
      if (CreateProcessA(NULL, szCommandExp, NULL, NULL, FALSE, 0, NULL, *szWorkDirExp?szWorkDirExp:NULL, &siA, &pi))
      {
        bResult=TRUE;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      }
      else
      {
        API_LoadStringA(hLangLib, MSG_ERROR_RUN, buf, BUFFER_SIZE);
        MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
      }
      API_HeapFree(hHeap, 0, (LPVOID)szWorkDirExp);
    }
    API_HeapFree(hHeap, 0, (LPVOID)szCommandExp);
  }
  return bResult;
}

BOOL DoSettingsExecW()
{
  STARTUPINFOW siW={0};
  PROCESS_INFORMATION pi={0};
  wchar_t *wszCommandExp;
  wchar_t *wszWorkDirExp;
  int nCommandLen;
  int nWorkDirLen;
  BOOL bResult=FALSE;

  nCommandLen=TranslateFileStringW(wszCommand, NULL, 0);
  nWorkDirLen=TranslateFileStringW(wszWorkDir, NULL, 0);

  if (wszCommandExp=(wchar_t *)API_HeapAlloc(hHeap, 0, nCommandLen * sizeof(wchar_t) + 2))
  {
    if (wszWorkDirExp=(wchar_t *)API_HeapAlloc(hHeap, 0, nWorkDirLen * sizeof(wchar_t) + 2))
    {
      TranslateFileStringW(wszCommand, wszCommandExp, nCommandLen + 1);
      TranslateFileStringW(wszWorkDir, wszWorkDirExp, nWorkDirLen + 1);

      siW.cb=sizeof(STARTUPINFOW);
      if (CreateProcessW(NULL, wszCommandExp, NULL, NULL, FALSE, 0, NULL, *wszWorkDirExp?wszWorkDirExp:NULL, &siW, &pi))
      {
        bResult=TRUE;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
      }
      else
      {
        API_LoadStringW(hLangLib, MSG_ERROR_RUN, wbuf, BUFFER_SIZE);
        MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
      }
      API_HeapFree(hHeap, 0, (LPVOID)wszWorkDirExp);
    }
    API_HeapFree(hHeap, 0, (LPVOID)wszCommandExp);
  }
  return bResult;
}

void DoSettingsReadOnly(HWND hWnd, BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_READONLY, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == bReadOnly) return;
  bReadOnly=bState;

  if (hWnd) SendMessage(hWnd, EM_SETREADONLY, bReadOnly, 0);
}

void DoSettingsSaveTime(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_SAVETIME, bState?MF_CHECKED:MF_UNCHECKED);
  bSaveTime=bState;
}

void DoSettingsKeepSpace(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_KEEPSPACE, bState?MF_CHECKED:MF_UNCHECKED);
  bKeepSpace=bState;
}

void DoSettingsWatchFile(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_WATCHFILE, bState?MF_CHECKED:MF_UNCHECKED);
  bWatchFile=bState;
  if (bWatchFile) SetFocus(hMainWnd);
}

void DoSettingsSingleOpenFile(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_SINGLEOPEN_FILE, bState?MF_CHECKED:MF_UNCHECKED);
  bSingleOpenFile=bState;
}

void DoSettingsSingleOpenProgram(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_SINGLEOPEN_PROGRAM, bState?MF_CHECKED:MF_UNCHECKED);
  bSingleOpenProgram=bState;
}

void DoSettingsPluginsA(HWND hWnd)
{
  API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_PLUGINS), hMainWnd, (DLGPROC)PluginsDlgProcA);
}

void DoSettingsPluginsW(HWND hWnd)
{
  API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_PLUGINS), hMainWnd, (DLGPROC)PluginsDlgProcW);
}

void DoSettingsOptionsA()
{
  hHookOptions=SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());
  API_LoadStringA(hLangLib, STR_OPTIONS, buf, BUFFER_SIZE);
  pshA.pszCaption=buf;
  bOptionsSave=FALSE;
  bOptionsRestart=FALSE;

  PropertySheetA(&pshA);

  if (bOptionsSave)
  {
    int nTmp;

    bEditFontChanged=TRUE;
    bColorsChanged=TRUE;
    bPrintFontChanged=TRUE;

    nTmp=nSaveSettings;
    nSaveSettings=SS_REGISTRY;
    SaveOptionsA();
    StackPluginSaveA(&hPluginsStack, TRUE);

    nSaveSettings=SS_INI;
    SaveOptionsA();
    StackPluginSaveA(&hPluginsStack, TRUE);
    nSaveSettings=nTmp;
  }
  if (bOptionsRestart)
  {
    API_LoadStringA(hLangLib, MSG_RESTART_PROGRAM, buf, BUFFER_SIZE);
    MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
  }
}

void DoSettingsOptionsW()
{
  hHookOptions=SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());
  API_LoadStringW(hLangLib, STR_OPTIONS, wbuf, BUFFER_SIZE);
  pshW.pszCaption=wbuf;
  bOptionsSave=FALSE;
  bOptionsRestart=FALSE;

  PropertySheetW(&pshW);

  if (bOptionsSave)
  {
    int nTmp;

    bEditFontChanged=TRUE;
    bColorsChanged=TRUE;
    bPrintFontChanged=TRUE;

    nTmp=nSaveSettings;
    nSaveSettings=SS_REGISTRY;
    SaveOptionsW();
    StackPluginSaveW(&hPluginsStack, TRUE);

    nSaveSettings=SS_INI;
    SaveOptionsW();
    StackPluginSaveW(&hPluginsStack, TRUE);
    nSaveSettings=nTmp;
  }
  if (bOptionsRestart)
  {
    API_LoadStringW(hLangLib, MSG_RESTART_PROGRAM, wbuf, BUFFER_SIZE);
    MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
  }
}

void DoWindowTabView(int nView, BOOL bFirst)
{
  DWORD dwStyle;
  int nCommand=0;

  if (nView == TAB_VIEW_TOP)
    nCommand=IDM_WINDOW_TABVIEW_TOP;
  else if (nView == TAB_VIEW_BOTTOM)
    nCommand=IDM_WINDOW_TABVIEW_BOTTOM;
  else if (nView == TAB_VIEW_NONE)
    nCommand=IDM_WINDOW_TABVIEW_NONE;
  CheckMenuRadioItem(hMainMenu, IDM_WINDOW_TABVIEW_TOP, IDM_WINDOW_TABVIEW_NONE, nCommand, MF_BYCOMMAND);

  EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_STANDARD, nView?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_BUTTONS, nView?MF_ENABLED:MF_GRAYED);
  if (!bOldComctl32) EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_FLATBUTTONS, nView?MF_ENABLED:MF_GRAYED);
  if (bFirst != TRUE && nView == nTabView) return;
  nTabView=nView;

  if (nTabView == TAB_VIEW_TOP)
  {
    dwStyle=GetWindowLongA(hTab, GWL_STYLE);
    SetWindowLongA(hTab, GWL_STYLE, dwStyle & ~TCS_BOTTOM);
  }
  else if (nTabView == TAB_VIEW_BOTTOM)
  {
    dwStyle=GetWindowLongA(hTab, GWL_STYLE);
    SetWindowLongA(hTab, GWL_STYLE, dwStyle|TCS_BOTTOM);
  }
  ShowWindow(hTab, nTabView?SW_SHOW:SW_HIDE);
  UpdateSize();
}

void DoWindowTabType(int nType, BOOL bFirst)
{
  DWORD dwStyle;
  int nCommand=0;

  if (nType == TAB_TYPE_STANDARD)
    nCommand=IDM_WINDOW_TABTYPE_STANDARD;
  else if (nType == TAB_TYPE_BUTTONS)
    nCommand=IDM_WINDOW_TABTYPE_BUTTONS;
  else if (nType == TAB_TYPE_FLATBUTTONS)
    nCommand=IDM_WINDOW_TABTYPE_FLATBUTTONS;
  CheckMenuRadioItem(hMainMenu, IDM_WINDOW_TABTYPE_STANDARD, IDM_WINDOW_TABTYPE_FLATBUTTONS, nCommand, MF_BYCOMMAND);

  if (bFirst != TRUE && nType == nTabType) return;
  nTabType=nType;

  if (nTabType == TAB_TYPE_STANDARD)
  {
    dwStyle=GetWindowLongA(hTab, GWL_STYLE);
    SetWindowLongA(hTab, GWL_STYLE, dwStyle & ~TCS_BUTTONS & ~TCS_FLATBUTTONS);
    SendMessage(hTab, TCM_SETITEMSIZE, 0, MAKELPARAM(TAB_WIDTH, TAB_HEIGHT - 4));
  }
  else if (nTabType == TAB_TYPE_BUTTONS)
  {
    dwStyle=GetWindowLongA(hTab, GWL_STYLE);
    SetWindowLongA(hTab, GWL_STYLE, (dwStyle|TCS_BUTTONS) & ~TCS_FLATBUTTONS);
    SendMessage(hTab, TCM_SETITEMSIZE, 0, MAKELPARAM(TAB_WIDTH, TAB_HEIGHT));
  }
  else if (nTabType == TAB_TYPE_FLATBUTTONS)
  {
    dwStyle=GetWindowLongA(hTab, GWL_STYLE);
    SetWindowLongA(hTab, GWL_STYLE, dwStyle|TCS_BUTTONS|TCS_FLATBUTTONS);
    SendMessage(hTab, TCM_SETITEMSIZE, 0, MAKELPARAM(TAB_WIDTH, TAB_HEIGHT));
  }
}

void DoWindowSelectWindowA()
{
  API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_MDILIST), hMainWnd, (DLGPROC)MdiListDlgProcA);
}

void DoWindowSelectWindowW()
{
  API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_MDILIST), hMainWnd, (DLGPROC)MdiListDlgProcW);
}

void DoHelpAboutA()
{
  API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_ABOUT), hMainWnd, (DLGPROC)AboutDlgProcA);
}

void DoHelpAboutW()
{
  API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_ABOUT), hMainWnd, (DLGPROC)AboutDlgProcW);
}

void DoNonMenuDelLineA(HWND hWnd)
{
  CHARRANGE cr;
  int nLine;

  if (IsReadOnly()) return;

  nLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nPosition);
  cr.cpMin=SendMessage(hWnd, EM_LINEINDEX, nLine, 0);
  cr.cpMax=cr.cpMin + SendMessage(hWnd, EM_LINELENGTH, nPosition, 0) + 1;

  SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
  SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"");
}

void DoNonMenuDelLineW(HWND hWnd)
{
  CHARRANGE cr;
  int nLine;

  if (IsReadOnly()) return;

  nLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nPosition);
  cr.cpMin=SendMessage(hWnd, EM_LINEINDEX, nLine, 0);
  cr.cpMax=cr.cpMin + SendMessage(hWnd, EM_LINELENGTH, nPosition, 0) + 1;

  SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
  SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"");
}


//// INI functions

BOOL OpenIniA(HSTACK *hIniStack, char *pFile, BOOL bCreate)
{
  HANDLE hFile=INVALID_HANDLE_VALUE;

  if (!bCreate)
  {
    if (FileExistsA(pFile))
      hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  }
  else hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (hFile != INVALID_HANDLE_VALUE)
  {
    ReadIni(hIniStack, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
}

BOOL OpenIniW(HSTACK *hIniStack, wchar_t *wpFile, BOOL bCreate)
{
  HANDLE hFile=INVALID_HANDLE_VALUE;

  if (!bCreate)
  {
    if (FileExistsW(wpFile))
      hFile=API_CreateFileW(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  }
  else hFile=API_CreateFileW(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (hFile != INVALID_HANDLE_VALUE)
  {
    ReadIni(hIniStack, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
}

BOOL SaveIniA(HSTACK *hIniStack, char *pFile)
{
  HANDLE hFile;
  DWORD dwAttr;
  BOOL bResult=FALSE;

  dwAttr=GetFileAttributesA(pFile);

  if (dwAttr == 0xFFFFFFFF || !(dwAttr & FILE_ATTRIBUTE_READONLY))
  {
    if ((hFile=API_CreateFileA(pFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
    {
      bResult=WriteIni(hIniStack, hFile);
      CloseHandle(hFile);
    }
  }
  return bResult;
}

BOOL SaveIniW(HSTACK *hIniStack, wchar_t *wpFile)
{
  HANDLE hFile;
  DWORD dwAttr;
  BOOL bResult=FALSE;

  dwAttr=GetFileAttributesW(wpFile);

  if (dwAttr == 0xFFFFFFFF || !(dwAttr & FILE_ATTRIBUTE_READONLY))
  {
    if ((hFile=API_CreateFileW(wpFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
    {
      bResult=WriteIni(hIniStack, hFile);
      CloseHandle(hFile);
    }
  }
  return bResult;
}

BOOL ReadIni(HSTACK *hIniStack, HANDLE hFile)
{
  HINISECTION *lpIniSection=NULL;
  HINIKEY *lpIniKey=NULL;
  wchar_t *wszText=NULL;
  wchar_t *wpText;
  wchar_t *wpSection;
  wchar_t *wpKey;
  wchar_t *wpString;
  DWORD dwFileSize;
  DWORD dwBytesRead;
  DWORD dwUnicodeLen;
  int nSectionLen;
  int nKeyLen;
  int nStringLen;

  if ((dwFileSize=GetFileSize(hFile, NULL)) != INVALID_FILE_SIZE)
  {
    if (dwFileSize >= 2)
    {
      if (wszText=wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, dwFileSize + 2))
      {
        if (ReadFile(hFile, wpText, dwFileSize, &dwBytesRead, NULL))
        {
          dwUnicodeLen=dwBytesRead / sizeof(wchar_t);
          wpText[dwUnicodeLen++]='\0';
          if (wszText[0] == 0xFEFF)
          {
            ++wpText;
            --dwUnicodeLen;
          }

          while (*wpText == ' ' || *wpText == '\t' || *wpText == '\r' || *wpText == '\n')
            ++wpText;

          while (*wpText)
          {
            //Section
            if (*wpText == '[')
            {
              for (wpSection=++wpText; *wpText && *wpText != ']' && *wpText != '\r' && *wpText != '\n'; ++wpText);

              if (*wpText == ']')
              {
                nSectionLen=wpText - wpSection;
                *wpText++='\0';

                if (!StackInsertIndex((stack **)&hIniStack->first, (stack **)&hIniStack->last, (stack **)&lpIniSection, -1, sizeof(HINISECTION)))
                {
                  lpIniSection->nSectionUnicodeBytes=nSectionLen * sizeof(wchar_t) + 2;
                  if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionUnicodeBytes))
                    memcpy(lpIniSection->wszSection, wpSection, lpIniSection->nSectionUnicodeBytes);
                  lpIniSection->nSectionAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, NULL, 0, NULL, NULL);
                  if (lpIniSection->szSection=(char *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionAnsiBytes))
                    WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, lpIniSection->szSection, lpIniSection->nSectionAnsiBytes, NULL, NULL);
                  lpIniSection->hKeysStack.first=0;
                  lpIniSection->hKeysStack.last=0;
                }
                else goto Error;
              }
              else goto Error;
            }
            else goto Error;

            //Keys and strings
            while (1)
            {
              while (*wpText == ' ' || *wpText == '\t' || *wpText == '\r' || *wpText == '\n')
                ++wpText;
              if (*wpText == '\0' || *wpText == '[') break;

              for (wpKey=wpText; *wpText && *wpText != '=' && *wpText != '\r' && *wpText != '\n'; ++wpText);

              if (*wpText == '=')
              {
                nKeyLen=wpText - wpKey;
                *wpText++='\0';

                for (wpString=wpText; *wpText && *wpText != '\r' && *wpText != '\n'; ++wpText);
                nStringLen=wpText - wpString;
                if (*wpText) *wpText++='\0';

                if (!StackInsertIndex((stack **)&lpIniSection->hKeysStack.first, (stack **)&lpIniSection->hKeysStack.last, (stack **)&lpIniKey, -1, sizeof(HINIKEY)))
                {
                  lpIniKey->nKeyUnicodeBytes=nKeyLen * sizeof(wchar_t) + 2;
                  if (lpIniKey->wszKey=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyUnicodeBytes))
                    memcpy(lpIniKey->wszKey, wpKey, lpIniKey->nKeyUnicodeBytes);
                  lpIniKey->nKeyAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpKey, nKeyLen + 1, NULL, 0, NULL, NULL);
                  if (lpIniKey->szKey=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyAnsiBytes))
                    WideCharToMultiByte(CP_ACP, 0, wpKey, nKeyLen + 1, lpIniKey->szKey, lpIniKey->nKeyAnsiBytes, NULL, NULL);

                  lpIniKey->nStringUnicodeBytes=nStringLen * sizeof(wchar_t) + 2;
                  if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
                    memcpy(lpIniKey->wszString, wpString, lpIniKey->nStringUnicodeBytes);
                  lpIniKey->nStringAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpString, nStringLen + 1, NULL, 0, NULL, NULL);
                  if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
                    WideCharToMultiByte(CP_ACP, 0, wpString, nStringLen + 1, lpIniKey->szString, lpIniKey->nStringAnsiBytes, NULL, NULL);
                }
                else goto Error;
              }
              else goto Error;
            }
          }
          API_HeapFree(hHeap, 0, (LPVOID)wszText);
          return TRUE;
        }
      }
    }
  }
  Error:
  if (wszText) API_HeapFree(hHeap, 0, (LPVOID)wszText);
  return FALSE;
}

BOOL WriteIni(HSTACK *hIniStack, HANDLE hFile)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->first;
  HINIKEY *lpIniKey=NULL;
  DWORD dwBytesWritten;

  if (!API_WriteFile(hFile, "\xFF\xFE", 2, &dwBytesWritten, NULL))
    return FALSE;

  while (lpIniSection)
  {
    //Section
    if (!API_WriteFile(hFile, L"[", 2, &dwBytesWritten, NULL))
      return FALSE;
    if (!API_WriteFile(hFile, lpIniSection->wszSection, lpIniSection->nSectionUnicodeBytes - 2, &dwBytesWritten, NULL))
      return FALSE;
    if (!API_WriteFile(hFile, L"]\r\n", 6, &dwBytesWritten, NULL))
      return FALSE;

    //Keys and strings
    lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

    while (lpIniKey)
    {
      if (!API_WriteFile(hFile, lpIniKey->wszKey, lpIniKey->nKeyUnicodeBytes - 2, &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, L"=", 2, &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, lpIniKey->wszString, lpIniKey->nStringUnicodeBytes - 2, &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, L"\r\n", 4, &dwBytesWritten, NULL))
        return FALSE;
      lpIniKey=lpIniKey->next;
    }
    if (lpIniSection->next)
    {
      if (!API_WriteFile(hFile, L"\r\n", 4, &dwBytesWritten, NULL))
        return FALSE;
    }
    lpIniSection=lpIniSection->next;
  }
  return TRUE;
}

HINISECTION* StackGetIniSectionA(HSTACK *hIniStack, char *pSection, int nSectionLen)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->first;

  nSectionLen=nSectionLen + 1;

  while (lpIniSection)
  {
    if (lpIniSection->nSectionAnsiBytes == nSectionLen)
    {
      if (!lstrcmpiA(lpIniSection->szSection, pSection))
        return lpIniSection;
    }
    lpIniSection=lpIniSection->next;
  }
  return NULL;
}

HINISECTION* StackGetIniSectionW(HSTACK *hIniStack, wchar_t *wpSection, int nSectionLen)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->first;

  nSectionLen=nSectionLen * sizeof(wchar_t) + 2;

  while (lpIniSection)
  {
    if (lpIniSection->nSectionUnicodeBytes == nSectionLen)
    {
      if (!lstrcmpiW(lpIniSection->wszSection, wpSection))
        return lpIniSection;
    }
    lpIniSection=lpIniSection->next;
  }
  return NULL;
}

HINIKEY* StackGetIniKeyA(HINISECTION *lpIniSection, char *pKey, int nKeyLen)
{
  HINIKEY *lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

  nKeyLen=nKeyLen + 1;

  while (lpIniKey)
  {
    if (lpIniKey->nKeyAnsiBytes == nKeyLen)
    {
      if (!lstrcmpiA(lpIniKey->szKey, pKey))
        return lpIniKey;
    }
    lpIniKey=lpIniKey->next;
  }
  return NULL;
}

HINIKEY* StackGetIniKeyW(HINISECTION *lpIniSection, wchar_t *wpKey, int nKeyLen)
{
  HINIKEY *lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

  nKeyLen=nKeyLen * sizeof(wchar_t) + 2;

  while (lpIniKey)
  {
    if (lpIniKey->nKeyUnicodeBytes == nKeyLen)
    {
      if (!lstrcmpiW(lpIniKey->wszKey, wpKey))
        return lpIniKey;
    }
    lpIniKey=lpIniKey->next;
  }
  return NULL;
}

int IniGetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  HINISECTION *lpIniSection=NULL;
  HINIKEY *lpIniKey=NULL;
  int nSectionLen=lstrlenA(pSection);
  int nKeyLen=lstrlenA(pKey);
  DWORD dwStringLen;

  if (lpIniSection=StackGetIniSectionA(hIniStack, pSection, nSectionLen))
  {
    if (lpIniKey=StackGetIniKeyA(lpIniSection, pKey, nKeyLen))
    {
      if (nType == INI_DWORD)
      {
        if (lpData)
        {
          DWORD dwNumber;

          dwNumber=(DWORD)xatoiA(lpIniKey->szString);
          dwStringLen=min(sizeof(DWORD), dwDataBytes);
          memcpy((DWORD *)lpData, &dwNumber, dwStringLen);
          return dwStringLen;
        }
        return sizeof(DWORD);
      }
      else if (nType == INI_BINARY)
      {
        return HexStrToDataA(lpIniKey->szString, lpData, dwDataBytes);
      }
      else if (nType == INI_STRINGANSI)
      {
        if (lpData)
        {
          dwStringLen=min((DWORD)lpIniKey->nStringAnsiBytes, dwDataBytes);
          memcpy((char *)lpData, lpIniKey->szString, dwStringLen);
          return dwStringLen;
        }
        return lpIniKey->nStringAnsiBytes;
      }
      else if (nType == INI_STRINGUNICODE)
      {
        if (lpData)
        {
          dwStringLen=min((DWORD)lpIniKey->nStringUnicodeBytes, dwDataBytes);
          memcpy((wchar_t *)lpData, lpIniKey->wszString, dwStringLen);
          return dwStringLen;
        }
        return lpIniKey->nStringUnicodeBytes;
      }
    }
  }
  return 0;
}

int IniGetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  HINISECTION *lpIniSection=NULL;
  HINIKEY *lpIniKey=NULL;
  int nSectionLen=lstrlenW(wpSection);
  int nKeyLen=lstrlenW(wpKey);
  DWORD dwStringLen;

  if (lpIniSection=StackGetIniSectionW(hIniStack, wpSection, nSectionLen))
  {
    if (lpIniKey=StackGetIniKeyW(lpIniSection, wpKey, nKeyLen))
    {
      if (nType == INI_DWORD)
      {
        if (lpData)
        {
          DWORD dwNumber;

          dwNumber=(DWORD)xatoiW(lpIniKey->wszString);
          dwStringLen=min(sizeof(DWORD), dwDataBytes);
          memcpy((DWORD *)lpData, &dwNumber, dwStringLen);
          return dwStringLen;
        }
        return sizeof(DWORD);
      }
      else if (nType == INI_BINARY)
      {
        return HexStrToDataW(lpIniKey->wszString, lpData, dwDataBytes);
      }
      else if (nType == INI_STRINGANSI)
      {
        if (lpData)
        {
          dwStringLen=min((DWORD)lpIniKey->nStringAnsiBytes, dwDataBytes);
          memcpy((char *)lpData, lpIniKey->szString, dwStringLen);
          return dwStringLen;
        }
        return lpIniKey->nStringAnsiBytes;
      }
      else if (nType == INI_STRINGUNICODE)
      {
        if (lpData)
        {
          dwStringLen=min((DWORD)lpIniKey->nStringUnicodeBytes, dwDataBytes);
          memcpy((wchar_t *)lpData, lpIniKey->wszString, dwStringLen);
          return dwStringLen;
        }
        return lpIniKey->nStringUnicodeBytes;
      }
    }
  }
  return 0;
}

BOOL IniSetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  HINISECTION *lpIniSection=NULL;
  HINIKEY *lpIniKey=NULL;
  char *pString;
  wchar_t *wpString;
  char szNumber[16];
  int nSectionLen=lstrlenA(pSection);
  int nKeyLen=lstrlenA(pKey);
  DWORD dwStringLen;
  DWORD a;
  DWORD b;

  if (!(lpIniSection=StackGetIniSectionA(hIniStack, pSection, nSectionLen)))
  {
    if (!StackInsertIndex((stack **)&hIniStack->first, (stack **)&hIniStack->last, (stack **)&lpIniSection, -1, sizeof(HINISECTION)))
    {
      lpIniSection->nSectionUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, pSection, nSectionLen + 1, NULL, 0) * sizeof(wchar_t);
      if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionUnicodeBytes))
        MultiByteToWideChar(CP_ACP, 0, pSection, nSectionLen + 1, lpIniSection->wszSection, lpIniSection->nSectionUnicodeBytes / sizeof(wchar_t));
      lpIniSection->nSectionAnsiBytes=nSectionLen + 1;
      if (lpIniSection->szSection=(char *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionAnsiBytes))
        memcpy(lpIniSection->szSection, pSection, lpIniSection->nSectionAnsiBytes);
      lpIniSection->hKeysStack.first=0;
      lpIniSection->hKeysStack.last=0;
    }
    else return FALSE;
  }
  if (!(lpIniKey=StackGetIniKeyA(lpIniSection, pKey, nKeyLen)))
  {
    if (!StackInsertIndex((stack **)&lpIniSection->hKeysStack.first, (stack **)&lpIniSection->hKeysStack.last, (stack **)&lpIniKey, -1, sizeof(HINIKEY)))
    {
      lpIniKey->nKeyUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, pKey, nKeyLen + 1, NULL, 0) * sizeof(wchar_t);
      if (lpIniKey->wszKey=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyUnicodeBytes))
        MultiByteToWideChar(CP_ACP, 0, pKey, nKeyLen + 1, lpIniKey->wszKey, lpIniKey->nKeyUnicodeBytes / sizeof(wchar_t));
      lpIniKey->nKeyAnsiBytes=nKeyLen + 1;
      if (lpIniKey->szKey=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyAnsiBytes))
        memcpy(lpIniKey->szKey, pKey, lpIniKey->nKeyAnsiBytes);
    }
    else return FALSE;
  }
  if (lpIniKey->wszString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);
  if (lpIniKey->szString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szString);

  if (nType == INI_DWORD)
  {
    pString=szNumber;
    dwStringLen=wsprintfA(szNumber, "%u", *(DWORD *)lpData);

    lpIniKey->nStringUnicodeBytes=dwStringLen * sizeof(wchar_t) + 2;
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      MultiByteToWideChar(CP_ACP, 0, pString, dwStringLen + 1, lpIniKey->wszString, lpIniKey->nStringUnicodeBytes / sizeof(wchar_t));
    lpIniKey->nStringAnsiBytes=dwStringLen + 1;
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      memcpy(lpIniKey->szString, pString, lpIniKey->nStringAnsiBytes);
  }
  else if (nType == INI_BINARY)
  {
    dwStringLen=dwDataBytes * 2;

    if (pString=(char *)API_HeapAlloc(hHeap, 0, dwStringLen + 1))
    {
      for (a=0, b=0; a < dwDataBytes && b < dwStringLen; ++a)
      {
        b+=wsprintfA(pString + b, "%02X", (unsigned char)lpData[a]);
      }
    }

    lpIniKey->nStringUnicodeBytes=dwStringLen * sizeof(wchar_t) + 2;
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      MultiByteToWideChar(CP_ACP, 0, pString, dwStringLen + 1, lpIniKey->wszString, lpIniKey->nStringUnicodeBytes / sizeof(wchar_t));
    lpIniKey->nStringAnsiBytes=dwStringLen + 1;
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      memcpy(lpIniKey->szString, pString, lpIniKey->nStringAnsiBytes);
    API_HeapFree(hHeap, 0, (LPVOID)pString);
  }
  else if (nType == INI_STRINGANSI)
  {
    pString=(char *)lpData;

    lpIniKey->nStringUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, pString, dwDataBytes, NULL, 0) * sizeof(wchar_t);
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      MultiByteToWideChar(CP_ACP, 0, pString, dwDataBytes, lpIniKey->wszString, lpIniKey->nStringUnicodeBytes / sizeof(wchar_t));
    lpIniKey->nStringAnsiBytes=dwDataBytes;
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      memcpy(lpIniKey->szString, pString, lpIniKey->nStringAnsiBytes);
  }
  else if (nType == INI_STRINGUNICODE)
  {
    wpString=(wchar_t *)lpData;

    lpIniKey->nStringUnicodeBytes=dwDataBytes;
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      memcpy(lpIniKey->wszString, wpString, lpIniKey->nStringUnicodeBytes);
    lpIniKey->nStringAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpString, dwDataBytes / sizeof(wchar_t), NULL, 0, NULL, NULL);
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      WideCharToMultiByte(CP_ACP, 0, wpString, dwDataBytes / sizeof(wchar_t), lpIniKey->szString, lpIniKey->nStringAnsiBytes, NULL, NULL);
  }
  else return FALSE;

  return TRUE;
}

BOOL IniSetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  HINISECTION *lpIniSection=NULL;
  HINIKEY *lpIniKey=NULL;
  char *pString;
  wchar_t *wpString;
  wchar_t wszNumber[16];
  int nSectionLen=lstrlenW(wpSection);
  int nKeyLen=lstrlenW(wpKey);
  DWORD dwStringLen;
  DWORD a;
  DWORD b;

  if (!(lpIniSection=StackGetIniSectionW(hIniStack, wpSection, nSectionLen)))
  {
    if (!StackInsertIndex((stack **)&hIniStack->first, (stack **)&hIniStack->last, (stack **)&lpIniSection, -1, sizeof(HINISECTION)))
    {
      lpIniSection->nSectionUnicodeBytes=nSectionLen * sizeof(wchar_t) + 2;
      if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionUnicodeBytes))
        memcpy(lpIniSection->wszSection, wpSection, lpIniSection->nSectionUnicodeBytes);
      lpIniSection->nSectionAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, NULL, 0, NULL, NULL);
      if (lpIniSection->szSection=(char *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionAnsiBytes))
        WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, lpIniSection->szSection, lpIniSection->nSectionAnsiBytes, NULL, NULL);
      lpIniSection->hKeysStack.first=0;
      lpIniSection->hKeysStack.last=0;
    }
    else return FALSE;
  }
  if (!(lpIniKey=StackGetIniKeyW(lpIniSection, wpKey, nKeyLen)))
  {
    if (!StackInsertIndex((stack **)&lpIniSection->hKeysStack.first, (stack **)&lpIniSection->hKeysStack.last, (stack **)&lpIniKey, -1, sizeof(HINIKEY)))
    {
      lpIniKey->nKeyUnicodeBytes=nKeyLen * sizeof(wchar_t) + 2;
      if (lpIniKey->wszKey=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyUnicodeBytes))
        memcpy(lpIniKey->wszKey, wpKey, lpIniKey->nKeyUnicodeBytes);
      lpIniKey->nKeyAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpKey, nKeyLen + 1, NULL, 0, NULL, NULL);
      if (lpIniKey->szKey=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyAnsiBytes))
        WideCharToMultiByte(CP_ACP, 0, wpKey, nKeyLen + 1, lpIniKey->szKey, lpIniKey->nKeyAnsiBytes, NULL, NULL);
    }
    else return FALSE;
  }
  if (lpIniKey->wszString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);
  if (lpIniKey->szString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szString);

  if (nType == INI_DWORD)
  {
    wpString=wszNumber;
    dwStringLen=wsprintfW(wszNumber, L"%u", *(DWORD *)lpData);

    lpIniKey->nStringUnicodeBytes=dwStringLen * sizeof(wchar_t) + 2;
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      memcpy(lpIniKey->wszString, wpString, lpIniKey->nStringUnicodeBytes);
    lpIniKey->nStringAnsiBytes=dwStringLen + 1;
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      WideCharToMultiByte(CP_ACP, 0, wpString, dwStringLen + 1, lpIniKey->szString, lpIniKey->nStringAnsiBytes, NULL, NULL);
  }
  else if (nType == INI_BINARY)
  {
    dwStringLen=dwDataBytes * 2;

    if (wpString=(wchar_t *)API_HeapAlloc(hHeap, 0, dwStringLen * sizeof(wchar_t) + 2))
    {
      for (a=0, b=0; a < dwDataBytes && b < dwStringLen; ++a)
      {
        b+=wsprintfW(wpString + b, L"%02X", (unsigned char)lpData[a]);
      }
    }

    lpIniKey->nStringUnicodeBytes=dwStringLen * sizeof(wchar_t) + 2;
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      memcpy(lpIniKey->wszString, wpString, lpIniKey->nStringUnicodeBytes);
    lpIniKey->nStringAnsiBytes=dwStringLen + 1;
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      WideCharToMultiByte(CP_ACP, 0, wpString, dwStringLen + 1, lpIniKey->szString, lpIniKey->nStringAnsiBytes, NULL, NULL);
    API_HeapFree(hHeap, 0, (LPVOID)wpString);
  }
  else if (nType == INI_STRINGANSI)
  {
    pString=(char *)lpData;

    lpIniKey->nStringUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, pString, dwDataBytes, NULL, 0) * sizeof(wchar_t);
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      MultiByteToWideChar(CP_ACP, 0, pString, dwDataBytes, lpIniKey->wszString, lpIniKey->nStringUnicodeBytes / sizeof(wchar_t));
    lpIniKey->nStringAnsiBytes=dwDataBytes;
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      memcpy(lpIniKey->szString, pString, lpIniKey->nStringAnsiBytes);
  }
  else if (nType == INI_STRINGUNICODE)
  {
    wpString=(wchar_t *)lpData;

    lpIniKey->nStringUnicodeBytes=dwDataBytes;
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringUnicodeBytes))
      memcpy(lpIniKey->wszString, wpString, lpIniKey->nStringUnicodeBytes);
    lpIniKey->nStringAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpString, dwDataBytes / sizeof(wchar_t), NULL, 0, NULL, NULL);
    if (lpIniKey->szString=(char *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringAnsiBytes))
      WideCharToMultiByte(CP_ACP, 0, wpString, dwDataBytes / sizeof(wchar_t), lpIniKey->szString, lpIniKey->nStringAnsiBytes, NULL, NULL);
  }
  else return FALSE;

  return TRUE;
}

void StackDeleteIniKey(HINISECTION *lpIniSection, HINIKEY *lpIniKey)
{
  if (lpIniKey)
  {
    if (lpIniKey->wszKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszKey);
    if (lpIniKey->szKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szKey);
    if (lpIniKey->wszString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);
    if (lpIniKey->szString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szString);
  }
  StackDelete((stack **)&lpIniSection->hKeysStack.first, (stack **)&lpIniSection->hKeysStack.last, (stack *)lpIniKey);
}

void StackDeleteIniSection(HSTACK *hIniStack, HINISECTION *lpIniSection, BOOL bClear)
{
  HINIKEY *lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

  while (lpIniKey)
  {
    if (lpIniKey->wszKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszKey);
    if (lpIniKey->szKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szKey);
    if (lpIniKey->wszString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);
    if (lpIniKey->szString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szString);
    lpIniKey=lpIniKey->next;
  }
  StackClear((stack **)&lpIniSection->hKeysStack.first, (stack **)&lpIniSection->hKeysStack.last);

  if (!bClear)
    StackDelete((stack **)&hIniStack->first, (stack **)&hIniStack->last, (stack *)lpIniSection);
}

void StackFreeIni(HSTACK *hIniStack)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->first;

  while (lpIniSection)
  {
    if (lpIniSection->wszSection) API_HeapFree(hHeap, 0, (LPVOID)lpIniSection->wszSection);
    if (lpIniSection->szSection) API_HeapFree(hHeap, 0, (LPVOID)lpIniSection->szSection);
    StackDeleteIniSection(hIniStack, lpIniSection, TRUE);
    lpIniSection=lpIniSection->next;
  }
  StackClear((stack **)&hIniStack->first, (stack **)&hIniStack->last);
}

DWORD HexStrToDataA(char *pHexStr, unsigned char *lpData, DWORD dwDataBytes)
{
  int a,b,c;
  DWORD dwLen=0;

  while (*pHexStr && (!lpData || dwLen < dwDataBytes))
  {
    a=*pHexStr;
    if (a >= '0' && a <= '9') a-='0';
    else if (a >= 'a' && a <= 'f') a-='a' - 10;
    else if (a >= 'A' && a <= 'F') a-='A' - 10;
    else break;

    b=*++pHexStr;
    if (b >= '0' && b <= '9') b-='0';
    else if (b >= 'a' && b <= 'f') b-='a' - 10;
    else if (b >= 'A' && b <= 'F') b-='A' - 10;
    else break;

    ++pHexStr;
    c=(a << 4) | b;
    if (lpData) lpData[dwLen]=c;
    ++dwLen;
  }
  return dwLen;
}

DWORD HexStrToDataW(wchar_t *wpHexStr, unsigned char *lpData, DWORD dwDataBytes)
{
  int a,b,c;
  DWORD dwLen=0;

  while (*wpHexStr && (!lpData || dwLen < dwDataBytes))
  {
    a=*wpHexStr;
    if (a >= '0' && a <= '9') a-='0';
    else if (a >= 'a' && a <= 'f') a-='a' - 10;
    else if (a >= 'A' && a <= 'F') a-='A' - 10;
    else break;

    b=*++wpHexStr;
    if (b >= '0' && b <= '9') b-='0';
    else if (b >= 'a' && b <= 'f') b-='a' - 10;
    else if (b >= 'A' && b <= 'F') b-='A' - 10;
    else break;

    ++wpHexStr;
    c=(a << 4) | b;
    if (lpData) lpData[dwLen]=c;
    ++dwLen;
  }
  return dwLen;
}


//// Read/Save options

DWORD ReadOptionA(HANDLE lpHandle, char *pParam, DWORD dwType, void *lpData, DWORD dwSize)
{
  if (nSaveSettings == SS_REGISTRY)
  {
    if (dwType == PO_DWORD) dwType=REG_DWORD;
    else if (dwType == PO_BINARY) dwType=REG_BINARY;
    else if (dwType == PO_STRING) dwType=REG_SZ;

    if (RegQueryValueExA((HKEY)lpHandle, pParam, NULL, &dwType, (LPBYTE)lpData, &dwSize) == ERROR_SUCCESS)
      return dwSize;
    else
      return 0;
  }
  else
  {
    if (dwType == PO_DWORD) dwType=INI_DWORD;
    else if (dwType == PO_BINARY) dwType=INI_BINARY;
    else if (dwType == PO_STRING) dwType=INI_STRINGANSI;

    return IniGetValueA((HSTACK *)lpHandle, "Options", pParam, dwType, (LPBYTE)lpData, dwSize);
  }
}

DWORD ReadOptionW(HANDLE lpHandle, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize)
{
  if (nSaveSettings == SS_REGISTRY)
  {
    if (dwType == PO_DWORD) dwType=REG_DWORD;
    else if (dwType == PO_BINARY) dwType=REG_BINARY;
    else if (dwType == PO_STRING) dwType=REG_SZ;

    if (RegQueryValueExW((HKEY)lpHandle, wpParam, NULL, &dwType, (LPBYTE)lpData, &dwSize) == ERROR_SUCCESS)
      return dwSize;
    else
      return 0;
  }
  else
  {
    if (dwType == PO_DWORD) dwType=INI_DWORD;
    else if (dwType == PO_BINARY) dwType=INI_BINARY;
    else if (dwType == PO_STRING) dwType=INI_STRINGUNICODE;

    return IniGetValueW((HSTACK *)lpHandle, L"Options", wpParam, dwType, (LPBYTE)lpData, dwSize);
  }
}

DWORD SaveOptionA(HANDLE lpHandle, char *pParam, DWORD dwType, void *lpData, DWORD dwSize)
{
  if (nSaveSettings == SS_REGISTRY)
  {
    if (dwType == PO_DWORD) dwType=REG_DWORD;
    else if (dwType == PO_BINARY) dwType=REG_BINARY;
    else if (dwType == PO_STRING) dwType=REG_SZ;

    return !RegSetValueExA((HKEY)lpHandle, pParam, 0, dwType, (LPBYTE)lpData, dwSize);
  }
  else
  {
    if (dwType == PO_DWORD) dwType=INI_DWORD;
    else if (dwType == PO_BINARY) dwType=INI_BINARY;
    else if (dwType == PO_STRING) dwType=INI_STRINGANSI;

    return IniSetValueA((HSTACK *)lpHandle, "Options", pParam, dwType, (LPBYTE)lpData, dwSize);
  }
}

DWORD SaveOptionW(HANDLE lpHandle, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize)
{
  if (nSaveSettings == SS_REGISTRY)
  {
    if (dwType == PO_DWORD) dwType=REG_DWORD;
    else if (dwType == PO_BINARY) dwType=REG_BINARY;
    else if (dwType == PO_STRING) dwType=REG_SZ;

    return !RegSetValueExW((HKEY)lpHandle, wpParam, 0, dwType, (LPBYTE)lpData, dwSize);
  }
  else
  {
    if (dwType == PO_DWORD) dwType=INI_DWORD;
    else if (dwType == PO_BINARY) dwType=INI_BINARY;
    else if (dwType == PO_STRING) dwType=INI_STRINGUNICODE;

    return IniSetValueW((HSTACK *)lpHandle, L"Options", wpParam, dwType, (LPBYTE)lpData, dwSize);
  }
}

void ReadOptionsA()
{
  HANDLE hHandle;
  DWORD dwSize;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Options", 0, KEY_READ, (HKEY *)&hHandle) != ERROR_SUCCESS)
      return;
  }
  else
  {
    hHandle=(HANDLE)&hIniStack;
  }

  ReadOptionA(hHandle, "WordWrap", PO_DWORD, &bWordWrap, sizeof(DWORD));
  ReadOptionA(hHandle, "OnTop", PO_DWORD, &bOnTop, sizeof(DWORD));
  ReadOptionA(hHandle, "StatusBar", PO_DWORD, &bStatusBar, sizeof(DWORD));
  ReadOptionA(hHandle, "ReadOnly", PO_DWORD, &bReadOnly, sizeof(DWORD));
  ReadOptionA(hHandle, "SaveTime", PO_DWORD, &bSaveTime, sizeof(DWORD));
  ReadOptionA(hHandle, "KeepSpace", PO_DWORD, &bKeepSpace, sizeof(DWORD));
  ReadOptionA(hHandle, "UndoLimit", PO_DWORD, &nUndoLimit, sizeof(DWORD));
  ReadOptionA(hHandle, "DetailedUndo", PO_DWORD, &bDetailedUndo, sizeof(DWORD));
  ReadOptionA(hHandle, "ReplaceAllAndClose", PO_DWORD, &bReplaceAllAndClose, sizeof(DWORD));
  ReadOptionA(hHandle, "SaveInReadOnlyMsg", PO_DWORD, &bSaveInReadOnlyMsg, sizeof(DWORD));
  ReadOptionA(hHandle, "WatchFile", PO_DWORD, &bWatchFile, sizeof(DWORD));
  ReadOptionA(hHandle, "SingleOpenFile", PO_DWORD, &bSingleOpenFile, sizeof(DWORD));
  ReadOptionA(hHandle, "SingleOpenProgram", PO_DWORD, &bSingleOpenProgram, sizeof(DWORD));
  ReadOptionA(hHandle, "MDI", PO_DWORD, &bRegMDI, sizeof(DWORD));
  ReadOptionA(hHandle, "SavePositions", PO_DWORD, &bSavePositions, sizeof(DWORD));
  ReadOptionA(hHandle, "SaveCodepages", PO_DWORD, &bSaveCodepages, sizeof(DWORD));
  ReadOptionA(hHandle, "DefaultCodepage", PO_DWORD, &nDefaultCodePage, sizeof(DWORD));
  //ReadOptionA(hHandle, "DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD));
  ReadOptionA(hHandle, "CodepageRecognition", PO_DWORD, &dwLangCodepageRecognition, sizeof(DWORD));
  ReadOptionA(hHandle, "CodepageRecognitionBuffer", PO_DWORD, &dwCodepageRecognitionBuffer, sizeof(DWORD));
  ReadOptionA(hHandle, "RecentFiles", PO_DWORD, &nRecentFiles, sizeof(DWORD));
  ReadOptionA(hHandle, "SearchStrings", PO_DWORD, &nSearchStrings, sizeof(DWORD));
  ReadOptionA(hHandle, "SearchOptions", PO_DWORD, &ftflags, sizeof(DWORD));
  ReadOptionA(hHandle, "TabStopSize", PO_DWORD, &nTabStopSize, sizeof(DWORD));
  ReadOptionA(hHandle, "TabStopAsSpaces", PO_DWORD, &bTabStopAsSpaces, sizeof(DWORD));
  ReadOptionA(hHandle, "MarginsEdit", PO_DWORD, &dwEditMargins, sizeof(DWORD));
  ReadOptionA(hHandle, "MarginsPrint", PO_BINARY, &psdPageA.rtMargin, sizeof(RECT));
  ReadOptionA(hHandle, "PluginsDialog", PO_BINARY, &rcPluginsCurrentDialog, sizeof(RECT));
  ReadOptionA(hHandle, "WindowStyle", PO_DWORD, &dwMainStyle, sizeof(DWORD));
  ReadOptionA(hHandle, "WindowPosition", PO_BINARY, &rcMainWindowRestored, sizeof(RECT));

  if (bRegMDI)
  {
    ReadOptionA(hHandle, "TabViewMDI", PO_DWORD, &nTabView, sizeof(DWORD));
    ReadOptionA(hHandle, "TabTypeMDI", PO_DWORD, &nTabType, sizeof(DWORD));
    ReadOptionA(hHandle, "TabSwitchMDI", PO_DWORD, &nTabSwitch, sizeof(DWORD));
    ReadOptionA(hHandle, "WindowListMDI", PO_BINARY, &rcMdiListCurrentDialog, sizeof(RECT));
    ReadOptionA(hHandle, "WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD));
  }

  ReadOptionA(hHandle, "ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD));
  ReadOptionA(hHandle, "ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD));
  ReadOptionA(hHandle, "WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "WordDelimiters", PO_BINARY, wszWordDelimiters, sizeof(wszWordDelimiters));
  ReadOptionA(hHandle, "Font", PO_BINARY, &lfEditFontA, sizeof(LOGFONTA) - LF_FACESIZE);
  ReadOptionA(hHandle, "FontFace", PO_STRING, &lfEditFontA.lfFaceName, LF_FACESIZE);
  ReadOptionA(hHandle, "PrintFontEnable", PO_DWORD, &bPrintFontEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "PrintFont", PO_BINARY, &lfPrintFontA, sizeof(LOGFONTA) - LF_FACESIZE);
  ReadOptionA(hHandle, "PrintFontFace", PO_STRING, &lfPrintFontA.lfFaceName, LF_FACESIZE);
  ReadOptionA(hHandle, "LanguageModule", PO_STRING, szLangModule, MAX_PATH);
  ReadOptionA(hHandle, "ExecuteCommand", PO_STRING, szCommand, BUFFER_SIZE);
  ReadOptionA(hHandle, "ExecuteDirectory", PO_STRING, szWorkDir, MAX_PATH);
  ReadOptionA(hHandle, "FileTypesOpen", PO_STRING, szFileTypesOpen, MAX_PATH);
  ReadOptionA(hHandle, "FileTypesEdit", PO_STRING, szFileTypesEdit, MAX_PATH);
  ReadOptionA(hHandle, "FileTypesPrint", PO_STRING, szFileTypesPrint, MAX_PATH);
  ReadOptionA(hHandle, "FileTypesAssociated", PO_DWORD, &dwFileTypesAssociated, sizeof(DWORD));
  ReadOptionA(hHandle, "PrintHeaderEnable", PO_DWORD, &bPrintHeaderEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "PrintHeader", PO_STRING, szPrintHeader, MAX_PATH);
  ReadOptionA(hHandle, "PrintFooterEnable", PO_DWORD, &bPrintFooterEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "PrintFooter", PO_STRING, szPrintFooter, MAX_PATH);

  if (dwSize=ReadOptionA(hHandle, "CodepageList", INI_BINARY, NULL, 0))
  {
    if (lpCodepageList=(int *)API_HeapAlloc(hHeap, 0, dwSize))
    {
      ReadOptionA(hHandle, "CodepageList", PO_BINARY, lpCodepageList, dwSize);
      nCodepageListLen=CodepageListLen(lpCodepageList);
    }
  }

  if (nSaveSettings == SS_REGISTRY)
    RegCloseKey((HKEY)hHandle);
}

void ReadOptionsW()
{
  HANDLE hHandle;
  DWORD dwSize;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Options", 0, KEY_READ, (HKEY *)&hHandle) != ERROR_SUCCESS)
      return;
  }
  else
  {
    hHandle=(HANDLE)&hIniStack;
  }

  ReadOptionW(hHandle, L"WordWrap", PO_DWORD, &bWordWrap, sizeof(DWORD));
  ReadOptionW(hHandle, L"OnTop", PO_DWORD, &bOnTop, sizeof(DWORD));
  ReadOptionW(hHandle, L"StatusBar", PO_DWORD, &bStatusBar, sizeof(DWORD));
  ReadOptionW(hHandle, L"ReadOnly", PO_DWORD, &bReadOnly, sizeof(DWORD));
  ReadOptionW(hHandle, L"SaveTime", PO_DWORD, &bSaveTime, sizeof(DWORD));
  ReadOptionW(hHandle, L"KeepSpace", PO_DWORD, &bKeepSpace, sizeof(DWORD));
  ReadOptionW(hHandle, L"UndoLimit", PO_DWORD, &nUndoLimit, sizeof(DWORD));
  ReadOptionW(hHandle, L"DetailedUndo", PO_DWORD, &bDetailedUndo, sizeof(DWORD));
  ReadOptionW(hHandle, L"ReplaceAllAndClose", PO_DWORD, &bReplaceAllAndClose, sizeof(DWORD));
  ReadOptionW(hHandle, L"SaveInReadOnlyMsg", PO_DWORD, &bSaveInReadOnlyMsg, sizeof(DWORD));
  ReadOptionW(hHandle, L"WatchFile", PO_DWORD, &bWatchFile, sizeof(DWORD));
  ReadOptionW(hHandle, L"SingleOpenFile", PO_DWORD, &bSingleOpenFile, sizeof(DWORD));
  ReadOptionW(hHandle, L"SingleOpenProgram", PO_DWORD, &bSingleOpenProgram, sizeof(DWORD));
  ReadOptionW(hHandle, L"MDI", PO_DWORD, &bRegMDI, sizeof(DWORD));
  ReadOptionW(hHandle, L"SavePositions", PO_DWORD, &bSavePositions, sizeof(DWORD));
  ReadOptionW(hHandle, L"SaveCodepages", PO_DWORD, &bSaveCodepages, sizeof(DWORD));
  ReadOptionW(hHandle, L"DefaultCodepage", PO_DWORD, &nDefaultCodePage, sizeof(DWORD));
  //ReadOptionW(hHandle, L"DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD));
  ReadOptionW(hHandle, L"CodepageRecognition", PO_DWORD, &dwLangCodepageRecognition, sizeof(DWORD));
  ReadOptionW(hHandle, L"CodepageRecognitionBuffer", PO_DWORD, &dwCodepageRecognitionBuffer, sizeof(DWORD));
  ReadOptionW(hHandle, L"RecentFiles", PO_DWORD, &nRecentFiles, sizeof(DWORD));
  ReadOptionW(hHandle, L"SearchStrings", PO_DWORD, &nSearchStrings, sizeof(DWORD));
  ReadOptionW(hHandle, L"SearchOptions", PO_DWORD, &ftflags, sizeof(DWORD));
  ReadOptionW(hHandle, L"TabStopSize", PO_DWORD, &nTabStopSize, sizeof(DWORD));
  ReadOptionW(hHandle, L"TabStopAsSpaces", PO_DWORD, &bTabStopAsSpaces, sizeof(DWORD));
  ReadOptionW(hHandle, L"MarginsEdit", PO_DWORD, &dwEditMargins, sizeof(DWORD));
  ReadOptionW(hHandle, L"MarginsPrint", PO_BINARY, &psdPageW.rtMargin, sizeof(RECT));
  ReadOptionW(hHandle, L"PluginsDialog", PO_BINARY, &rcPluginsCurrentDialog, sizeof(RECT));
  ReadOptionW(hHandle, L"WindowStyle", PO_DWORD, &dwMainStyle, sizeof(DWORD));
  ReadOptionW(hHandle, L"WindowPosition", PO_BINARY, &rcMainWindowRestored, sizeof(RECT));

  if (bRegMDI)
  {
    ReadOptionW(hHandle, L"TabViewMDI", PO_DWORD, &nTabView, sizeof(DWORD));
    ReadOptionW(hHandle, L"TabTypeMDI", PO_DWORD, &nTabType, sizeof(DWORD));
    ReadOptionW(hHandle, L"TabSwitchMDI", PO_DWORD, &nTabSwitch, sizeof(DWORD));
    ReadOptionW(hHandle, L"WindowListMDI", PO_BINARY, &rcMdiListCurrentDialog, sizeof(RECT));
    ReadOptionW(hHandle, L"WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD));
  }

  ReadOptionW(hHandle, L"ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD));
  ReadOptionW(hHandle, L"ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD));
  ReadOptionW(hHandle, L"WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"WordDelimiters", PO_BINARY, wszWordDelimiters, sizeof(wszWordDelimiters));
  ReadOptionW(hHandle, L"Font", PO_BINARY, &lfEditFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"FontFace", PO_STRING, &lfEditFontW.lfFaceName, LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"PrintFontEnable", PO_DWORD, &bPrintFontEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"PrintFont", PO_BINARY, &lfPrintFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"PrintFontFace", PO_STRING, &lfPrintFontW.lfFaceName, LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"Colors", PO_BINARY, &aecColors, sizeof(AECOLORS));
  ReadOptionW(hHandle, L"LanguageModule", PO_STRING, wszLangModule, MAX_PATH * sizeof(wchar_t));
  ReadOptionW(hHandle, L"ExecuteCommand", PO_STRING, wszCommand, BUFFER_SIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"ExecuteDirectory", PO_STRING, wszWorkDir, MAX_PATH * sizeof(wchar_t));
  ReadOptionW(hHandle, L"FileTypesOpen", PO_STRING, wszFileTypesOpen, MAX_PATH * sizeof(wchar_t));
  ReadOptionW(hHandle, L"FileTypesEdit", PO_STRING, wszFileTypesEdit, MAX_PATH * sizeof(wchar_t));
  ReadOptionW(hHandle, L"FileTypesPrint", PO_STRING, wszFileTypesPrint, MAX_PATH * sizeof(wchar_t));
  ReadOptionW(hHandle, L"FileTypesAssociated", PO_DWORD, &dwFileTypesAssociated, sizeof(DWORD));
  ReadOptionW(hHandle, L"PrintHeaderEnable", PO_DWORD, &bPrintHeaderEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"PrintHeader", PO_STRING, wszPrintHeader, MAX_PATH * sizeof(wchar_t));
  ReadOptionW(hHandle, L"PrintFooterEnable", PO_DWORD, &bPrintFooterEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"PrintFooter", PO_STRING, wszPrintFooter, MAX_PATH * sizeof(wchar_t));

  if (dwSize=ReadOptionW(hHandle, L"CodepageList", INI_BINARY, NULL, 0))
  {
    if (lpCodepageList=(int *)API_HeapAlloc(hHeap, 0, dwSize))
    {
      ReadOptionW(hHandle, L"CodepageList", PO_BINARY, lpCodepageList, dwSize);
      nCodepageListLen=CodepageListLen(lpCodepageList);
    }
  }

  if (nSaveSettings == SS_REGISTRY)
    RegCloseKey((HKEY)hHandle);
}

void RegisterPluginsHotkeysA()
{
  if (nSaveSettings == SS_REGISTRY)
  {
    HKEY hKey;
    DWORD dwType;
    DWORD dwSizeValue;
    DWORD dwSizeString;
    DWORD dwIndex=0;
    DWORD dwHotkey=0;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Plugs", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      while (1)
      {
        dwSizeValue=BUFFER_SIZE;
        dwSizeString=sizeof(DWORD);
        if (RegEnumValueA(hKey, dwIndex++, buf, &dwSizeValue, NULL, &dwType, (LPBYTE)&dwHotkey, &dwSizeString) != ERROR_SUCCESS)
          break;

        if (dwHotkey)
        {
          StackPluginAddA(&hPluginsStack, buf, lstrlenA(buf), LOWORD(dwHotkey), HIWORD(dwHotkey), FALSE, NULL, NULL);
        }
      }
      RegCloseKey(hKey);
    }
  }
  else
  {
    HINISECTION *lpIniSection;
    HINIKEY *lpIniKey;
    DWORD dwHotkey=0;

    if (lpIniSection=StackGetIniSectionA(&hIniStack, "Plugs", lstrlenA("Plugs")))
    {
      lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

      while (lpIniKey)
      {
        if (dwHotkey=(DWORD)xatoiA(lpIniKey->szString))
        {
          StackPluginAddA(&hPluginsStack, lpIniKey->szKey, lpIniKey->nKeyAnsiBytes - 1, LOWORD(dwHotkey), HIWORD(dwHotkey), FALSE, NULL, NULL);
        }
        lpIniKey=lpIniKey->next;
      }
    }
  }
}

void RegisterPluginsHotkeysW()
{
  if (nSaveSettings == SS_REGISTRY)
  {
    HKEY hKey;
    DWORD dwType;
    DWORD dwSizeValue;
    DWORD dwSizeString;
    DWORD dwIndex=0;
    DWORD dwHotkey=0;

    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Plugs", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      while (1)
      {
        dwSizeValue=BUFFER_SIZE;
        dwSizeString=sizeof(DWORD);
        if (RegEnumValueW(hKey, dwIndex++, wbuf, &dwSizeValue, NULL, &dwType, (LPBYTE)&dwHotkey, &dwSizeString) != ERROR_SUCCESS)
          break;

        if (dwHotkey)
        {
          StackPluginAddW(&hPluginsStack, wbuf, lstrlenW(wbuf), LOWORD(dwHotkey), HIWORD(dwHotkey), FALSE, NULL, NULL);
        }
      }
      RegCloseKey(hKey);
    }
  }
  else
  {
    HINISECTION *lpIniSection;
    HINIKEY *lpIniKey;
    DWORD dwHotkey=0;

    if (lpIniSection=StackGetIniSectionW(&hIniStack, L"Plugs", lstrlenW(L"Plugs")))
    {
      lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

      while (lpIniKey)
      {
        if (dwHotkey=(DWORD)xatoiW(lpIniKey->wszString))
        {
          StackPluginAddW(&hPluginsStack, lpIniKey->wszKey, (lpIniKey->nKeyUnicodeBytes - 1) / sizeof(wchar_t), LOWORD(dwHotkey), HIWORD(dwHotkey), FALSE, NULL, NULL);
        }
        lpIniKey=lpIniKey->next;
      }
    }
  }
}

void RegReadSearchA()
{
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Search", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  if (RegQueryValueExA(hKey, "find0", NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
  {
    if (szFind_orig=szFind=(char *)API_HeapAlloc(hHeap, 0, dwSize + 1))
    {
      if (RegQueryValueExA(hKey, "find0", NULL, &dwType, (LPBYTE)szFind_orig, &dwSize) == ERROR_SUCCESS)
      {
        if (ftflags & FR_ESCAPESEQ)
        {
          if (szFind=(char *)API_HeapAlloc(hHeap, 0, dwSize + 1))
          {
            EscapeStringToEscapeDataA(szFind_orig, szFind);
          }
        }
      }
    }
  }
  RegCloseKey(hKey);
}

void RegReadSearchW()
{
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Search", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  if (RegQueryValueExW(hKey, L"find0", NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
  {
    if (wszFind_orig=wszFind=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize + 2))
    {
      if (RegQueryValueExW(hKey, L"find0", NULL, &dwType, (LPBYTE)wszFind_orig, &dwSize) == ERROR_SUCCESS)
      {
        if (ftflags & FR_ESCAPESEQ)
        {
          if (wszFind=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize + 2))
          {
            EscapeStringToEscapeDataW(wszFind_orig, wszFind);
          }
        }
      }
    }
  }
  RegCloseKey(hKey);
}

BOOL SaveOptionsA()
{
  HANDLE hHandle;
  BOOL bResult=FALSE;

  dwMainStyle=GetWindowLongA(hMainWnd, GWL_STYLE);
  dwMainStyle=((dwMainStyle & WS_MAXIMIZE) || ((dwMainStyle & WS_MINIMIZE) && dwLastMainSize == SIZE_MAXIMIZED))?WS_MAXIMIZE:0;
  if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
  dwMdiStyle=(bMdiMaximize == TRUE)?WS_MAXIMIZE:0;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Options", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, (HKEY *)&hHandle, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!OpenIniA(&hIniStack, szIniFile, TRUE))
      return FALSE;
    hHandle=(HANDLE)&hIniStack;

    if (!SaveOptionA(hHandle, "SaveSettings", PO_DWORD, &nRegSaveSettings, sizeof(DWORD)))
      goto Error;
  }

  if (!SaveOptionA(hHandle, "WordWrap", PO_DWORD, &bWordWrap, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "OnTop", PO_DWORD, &bOnTop, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "StatusBar", PO_DWORD, &bStatusBar, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "ReadOnly", PO_DWORD, &bReadOnly, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SaveTime", PO_DWORD, &bSaveTime, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "KeepSpace", PO_DWORD, &bKeepSpace, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "UndoLimit", PO_DWORD, &nUndoLimit, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "DetailedUndo", PO_DWORD, &bDetailedUndo, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "ReplaceAllAndClose", PO_DWORD, &bReplaceAllAndClose, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SaveInReadOnlyMsg", PO_DWORD, &bSaveInReadOnlyMsg, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WatchFile", PO_DWORD, &bWatchFile, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SingleOpenFile", PO_DWORD, &bSingleOpenFile, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SingleOpenProgram", PO_DWORD, &bSingleOpenProgram, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "MDI", PO_DWORD, &bRegMDI, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SavePositions", PO_DWORD, &bSavePositions, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SaveCodepages", PO_DWORD, &bSaveCodepages, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "DefaultCodepage", PO_DWORD, &nDefaultCodePage, sizeof(DWORD)))
    goto Error;
  //if (!SaveOptionA(hHandle, "DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD)))
  //  goto Error;
  if (!SaveOptionA(hHandle, "CodepageRecognition", PO_DWORD, &dwLangCodepageRecognition, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "CodepageRecognitionBuffer", PO_DWORD, &dwCodepageRecognitionBuffer, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "RecentFiles", PO_DWORD, &nRecentFiles, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SearchStrings", PO_DWORD, &nSearchStrings, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "SearchOptions", PO_DWORD, &ftflags, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "TabStopSize", PO_DWORD, &nTabStopSize, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "TabStopAsSpaces", PO_DWORD, &bTabStopAsSpaces, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "MarginsEdit", PO_DWORD, &dwEditMargins, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "MarginsPrint", PO_BINARY, &psdPageA.rtMargin, sizeof(RECT)))
    goto Error;
  if (!SaveOptionA(hHandle, "PluginsDialog", PO_BINARY, &rcPluginsCurrentDialog, sizeof(RECT)))
    goto Error;
  if (!SaveOptionA(hHandle, "WindowStyle", PO_DWORD, &dwMainStyle, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WindowPosition", PO_BINARY, &rcMainWindowRestored, sizeof(RECT)))
    goto Error;

  if (bMDI)
  {
    if (!SaveOptionA(hHandle, "TabViewMDI", PO_DWORD, &nTabView, sizeof(DWORD)))
      goto Error;
    if (!SaveOptionA(hHandle, "TabTypeMDI", PO_DWORD, &nTabType, sizeof(DWORD)))
      goto Error;
    if (!SaveOptionA(hHandle, "TabSwitchMDI", PO_DWORD, &nTabSwitch, sizeof(DWORD)))
      goto Error;
    if (!SaveOptionA(hHandle, "WindowListMDI", PO_BINARY, &rcMdiListCurrentDialog, sizeof(RECT)))
      goto Error;
    if (!SaveOptionA(hHandle, "WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD)))
      goto Error;
  }

  if (!SaveOptionA(hHandle, "ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WordDelimiters", PO_BINARY, wszWordDelimiters, wcslen(wszWordDelimiters) * sizeof(wchar_t) + 2))
    goto Error;

  if (bEditFontChanged)
  {
    if (!SaveOptionA(hHandle, "Font", PO_BINARY, &lfEditFontA, sizeof(LOGFONTA) - LF_FACESIZE))
      goto Error;
    if (!SaveOptionA(hHandle, "FontFace", PO_STRING, &lfEditFontA.lfFaceName, lstrlenA(lfEditFontA.lfFaceName) + 1))
      goto Error;
  }
  if (!SaveOptionA(hHandle, "PrintFontEnable", PO_DWORD, &bPrintFontEnable, sizeof(DWORD)))
    goto Error;
  if (bPrintFontChanged)
  {
    if (!SaveOptionA(hHandle, "PrintFont", PO_BINARY, &lfPrintFontA, sizeof(LOGFONTA) - LF_FACESIZE))
      goto Error;
    if (!SaveOptionA(hHandle, "PrintFontFace", PO_STRING, &lfPrintFontA.lfFaceName, lstrlenA(lfPrintFontA.lfFaceName) + 1))
      goto Error;
  }
  if (bColorsChanged)
  {
    if (!SaveOptionA(hHandle, "Colors", PO_BINARY, &aecColors, sizeof(AECOLORS)))
      goto Error;
  }
  if (!SaveOptionA(hHandle, "LanguageModule", PO_STRING, szLangModule, lstrlenA(szLangModule) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "ExecuteCommand", PO_STRING, szCommand, lstrlenA(szCommand) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "ExecuteDirectory", PO_STRING, szWorkDir, lstrlenA(szCommand) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "FileTypesOpen", PO_STRING, szFileTypesOpen, lstrlenA(szFileTypesOpen) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "FileTypesEdit", PO_STRING, szFileTypesEdit, lstrlenA(szFileTypesEdit) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "FileTypesPrint", PO_STRING, szFileTypesPrint, lstrlenA(szFileTypesPrint) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "FileTypesAssociated", PO_DWORD, &dwFileTypesAssociated, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "PrintHeaderEnable", PO_DWORD, &bPrintHeaderEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "PrintHeader", PO_STRING, szPrintHeader, lstrlenA(szPrintHeader) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "PrintFooterEnable", PO_DWORD, &bPrintFooterEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "PrintFooter", PO_STRING, szPrintFooter, lstrlenA(szPrintFooter) + 1))
    goto Error;
  if (!SaveOptionA(hHandle, "CodepageList", PO_BINARY, lpCodepageList, nCodepageListLen * sizeof(int)))
    goto Error;

  if (nSaveSettings == SS_REGISTRY)
    bResult=TRUE;
  else
    bResult=SaveIniA((HSTACK *)hHandle, szIniFile);

  Error:
  if (nSaveSettings == SS_REGISTRY)
    RegCloseKey((HKEY)hHandle);
  else
    StackFreeIni((HSTACK *)hHandle);
  return bResult;
}

BOOL SaveOptionsW()
{
  HANDLE hHandle;
  BOOL bResult=FALSE;

  dwMainStyle=GetWindowLongW(hMainWnd, GWL_STYLE);
  dwMainStyle=((dwMainStyle & WS_MAXIMIZE) || ((dwMainStyle & WS_MINIMIZE) && dwLastMainSize == SIZE_MAXIMIZED))?WS_MAXIMIZE:0;
  if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
  dwMdiStyle=(bMdiMaximize == TRUE)?WS_MAXIMIZE:0;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Options", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, (HKEY *)&hHandle, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!OpenIniW(&hIniStack, wszIniFile, TRUE))
      return FALSE;
    hHandle=(HANDLE)&hIniStack;

    if (!SaveOptionW(hHandle, L"SaveSettings", PO_DWORD, &nRegSaveSettings, sizeof(DWORD)))
      goto Error;
  }

  if (!SaveOptionW(hHandle, L"WordWrap", PO_DWORD, &bWordWrap, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"OnTop", PO_DWORD, &bOnTop, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"StatusBar", PO_DWORD, &bStatusBar, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"ReadOnly", PO_DWORD, &bReadOnly, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SaveTime", PO_DWORD, &bSaveTime, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"KeepSpace", PO_DWORD, &bKeepSpace, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"UndoLimit", PO_DWORD, &nUndoLimit, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"DetailedUndo", PO_DWORD, &bDetailedUndo, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"ReplaceAllAndClose", PO_DWORD, &bReplaceAllAndClose, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SaveInReadOnlyMsg", PO_DWORD, &bSaveInReadOnlyMsg, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WatchFile", PO_DWORD, &bWatchFile, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SingleOpenFile", PO_DWORD, &bSingleOpenFile, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SingleOpenProgram", PO_DWORD, &bSingleOpenProgram, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"MDI", PO_DWORD, &bRegMDI, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SavePositions", PO_DWORD, &bSavePositions, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SaveCodepages", PO_DWORD, &bSaveCodepages, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"DefaultCodepage", PO_DWORD, &nDefaultCodePage, sizeof(DWORD)))
    goto Error;
  //if (!SaveOptionW(hHandle, L"DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD)))
  //  goto Error;
  if (!SaveOptionW(hHandle, L"CodepageRecognition", PO_DWORD, &dwLangCodepageRecognition, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"CodepageRecognitionBuffer", PO_DWORD, &dwCodepageRecognitionBuffer, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"RecentFiles", PO_DWORD, &nRecentFiles, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SearchStrings", PO_DWORD, &nSearchStrings, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"SearchOptions", PO_DWORD, &ftflags, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"TabStopSize", PO_DWORD, &nTabStopSize, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"TabStopAsSpaces", PO_DWORD, &bTabStopAsSpaces, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"MarginsEdit", PO_DWORD, &dwEditMargins, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"MarginsPrint", PO_BINARY, &psdPageW.rtMargin, sizeof(RECT)))
    goto Error;
  if (!SaveOptionW(hHandle, L"PluginsDialog", PO_BINARY, &rcPluginsCurrentDialog, sizeof(RECT)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WindowStyle", PO_DWORD, &dwMainStyle, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WindowPosition", PO_BINARY, &rcMainWindowRestored, sizeof(RECT)))
    goto Error;

  if (bMDI)
  {
    if (!SaveOptionW(hHandle, L"TabViewMDI", PO_DWORD, &nTabView, sizeof(DWORD)))
      goto Error;
    if (!SaveOptionW(hHandle, L"TabTypeMDI", PO_DWORD, &nTabType, sizeof(DWORD)))
      goto Error;
    if (!SaveOptionW(hHandle, L"TabSwitchMDI", PO_DWORD, &nTabSwitch, sizeof(DWORD)))
      goto Error;
    if (!SaveOptionW(hHandle, L"WindowListMDI", PO_BINARY, &rcMdiListCurrentDialog, sizeof(RECT)))
      goto Error;
    if (!SaveOptionW(hHandle, L"WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD)))
      goto Error;
  }

  if (!SaveOptionW(hHandle, L"ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WordDelimiters", PO_BINARY, wszWordDelimiters, lstrlenW(wszWordDelimiters) * sizeof(wchar_t) + 2))
    goto Error;

  if (bEditFontChanged)
  {
    if (!SaveOptionW(hHandle, L"Font", PO_BINARY, &lfEditFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t)))
      goto Error;
    if (!SaveOptionW(hHandle, L"FontFace", PO_STRING, &lfEditFontW.lfFaceName, lstrlenW(lfEditFontW.lfFaceName) * sizeof(wchar_t) + 2))
      goto Error;
  }
  if (!SaveOptionW(hHandle, L"PrintFontEnable", PO_DWORD, &bPrintFontEnable, sizeof(DWORD)))
    goto Error;
  if (bPrintFontChanged)
  {
    if (!SaveOptionW(hHandle, L"PrintFont", PO_BINARY, &lfPrintFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t)))
      goto Error;
    if (!SaveOptionW(hHandle, L"PrintFontFace", PO_STRING, &lfPrintFontW.lfFaceName, lstrlenW(lfPrintFontW.lfFaceName) * sizeof(wchar_t) + 2))
      goto Error;
  }
  if (bColorsChanged)
  {
    if (!SaveOptionW(hHandle, L"Colors", PO_BINARY, &aecColors, sizeof(AECOLORS)))
      goto Error;
  }
  if (!SaveOptionW(hHandle, L"LanguageModule", PO_STRING, wszLangModule, lstrlenW(wszLangModule) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"ExecuteCommand", PO_STRING, wszCommand, lstrlenW(wszCommand) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"ExecuteDirectory", PO_STRING, wszWorkDir, lstrlenW(wszWorkDir) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"FileTypesOpen", PO_STRING, wszFileTypesOpen, lstrlenW(wszFileTypesOpen) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"FileTypesEdit", PO_STRING, wszFileTypesEdit, lstrlenW(wszFileTypesEdit) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"FileTypesPrint", PO_STRING, wszFileTypesPrint, lstrlenW(wszFileTypesPrint) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"FileTypesAssociated", PO_DWORD, &dwFileTypesAssociated, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"PrintHeaderEnable", PO_DWORD, &bPrintHeaderEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"PrintHeader", PO_STRING, wszPrintHeader, lstrlenW(wszPrintHeader) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"PrintFooterEnable", PO_DWORD, &bPrintFooterEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"PrintFooter", PO_STRING, wszPrintFooter, lstrlenW(wszPrintFooter) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"CodepageList", PO_BINARY, lpCodepageList, nCodepageListLen * sizeof(int)))
    goto Error;

  if (nSaveSettings == SS_REGISTRY)
    bResult=TRUE;
  else
    bResult=SaveIniW((HSTACK *)hHandle, wszIniFile);

  Error:
  if (nSaveSettings == SS_REGISTRY)
    RegCloseKey((HKEY)hHandle);
  else
    StackFreeIni((HSTACK *)hHandle);
  return bResult;
}


//// Open/Save document

int OpenDocumentA(HWND hWnd, char *szFile, DWORD dwFlags, int nCodePage, BOOL bBOM)
{
  NOPENDOCUMENTA nodA;
  HANDLE hFile;
  FILESTREAMDATA fsd;
  HWND hWndFriend=NULL;
  int nResult=EOD_SUCCESS;
  int nDetect;
  int nFileCmp;
  BOOL bFileExist;

  if (!hWnd) hWnd=hWndEdit;

  //Notification message
  nodA.pFile=szFile;
  nodA.nCodePage=&nCodePage;
  nodA.bBOM=&bBOM;
  nodA.dwFlags=&dwFlags;
  nodA.bProcess=TRUE;
  SendMessage(hMainWnd, AKDN_OPENDOCUMENT_START, (WPARAM)hWnd, (LPARAM)&nodA);
  if (!nodA.bProcess)
  {
    nResult=EOD_STOP;
    goto End;
  }

  if (!(bFileExist=GetFullNameA(szFile)))
  {
    //File doesn't exist
    if (nMsgCreate == AUTOANSWER_ASK)
    {
      API_LoadStringA(hLangLib, MSG_FILE_DOES_NOT_EXIST, buf, BUFFER_SIZE);
      wsprintfA(buf2, buf, szFile);
      if (MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OKCANCEL|MB_ICONEXCLAMATION) == IDCANCEL)
      {
        nResult=EOD_CANCEL;
        goto End;
      }
    }
    else if (nMsgCreate == AUTOANSWER_NO)
    {
      nResult=EOD_CANCEL;
      goto End;
    }

    nCodePage=nDefaultCodePage;
    bBOM=bDefaultBOM;
  }
  else
  {
    if (hWnd == hWndEdit)
    {
      //File exists
      if (!bMDI && !bDocumentReopen && bSingleOpenFile && lstrcmpiA(szFile, szCurrentFile))
      {
        if ((hWndFriend=FindWindowA(APP_SDI_CLASSA, szFile)) &&
            (hWndFriend=GetParent(hWndFriend)))
        {
          SetForegroundWindow(hWndFriend);
          OpenDocumentSendA(hWndFriend, NULL, szFile, dwFlags, nCodePage, bBOM, FALSE);
          nResult=EOD_WINDOW_EXIST;
          goto End;
        }
      }
      if (bMDI && !bDocumentReopen && bSingleOpenFile)
      {
        if (!lstrcmpiA(szFile, szCurrentFile) || (hWndFriend=FindWindowExA(hMdiClient, NULL, APP_MDI_CLASSA, szFile)))
        {
          if (hWndFriend)
          {
            SendMessage(hMdiClient, WM_MDIACTIVATE, (WPARAM)hWndFriend, 0);
            hWnd=hWndEdit;
          }
          if (SaveChangedA())
          {
            bDocumentReopen=TRUE;
            OpenDocumentA(hWnd, szFile, dwFlags, nCodePage, bBOM);
            bDocumentReopen=FALSE;
          }
          nResult=EOD_WINDOW_EXIST;
          goto End;
        }
      }
    }

    //Autodetect code page
    if ((nDetect=AutodetectCodePageA(szFile, dwCodepageRecognitionBuffer, dwFlags, &nCodePage, &bBOM)) < 0)
    {
      if (!bDocumentReopen)
      {
        if (nDetect == EDT_BINARY)
        {
          if (nMsgBinary == AUTOANSWER_ASK)
          {
            API_LoadStringA(hLangLib, MSG_ERROR_BINARY, buf, BUFFER_SIZE);
            wsprintfA(buf2, buf, szFile);
            if (MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDCANCEL)
            {
              nResult=EOD_CANCEL;
              goto End;
            }
          }
          else if (nMsgBinary == AUTOANSWER_NO)
          {
            nResult=EOD_CANCEL;
            goto End;
          }
        }
        else
        {
          nResult=nDetect;
          goto End;
        }
      }
    }
  }

  //Check code page
  if (!CheckCodePage(nCodePage))
  {
    API_LoadStringA(hLangLib, MSG_CP_UNIMPLEMENTED, buf, BUFFER_SIZE);
    wsprintfA(buf2, buf, nCodePage);
    MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
    nResult=EOD_CODEPAGE_ERROR;
    goto End;
  }

  //Open file
  if ((hFile=API_CreateFileA(szFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, bFileExist?OPEN_EXISTING:OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
  {
    nResult=EOD_OPEN;
    goto End;
  }

  if (bBOM)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (nCodePage == CP_UNICODE_UCS2_BE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (nCodePage == CP_UNICODE_UTF8)
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
  }

  if (hWnd == hWndEdit)
  {
    //Save position of the previous file before load new document
    if (nRecentFiles && szCurrentFile[0])
    {
      RecentFilesZeroA();
      RecentFilesReadA();
      RecentFilesUpdateA(szCurrentFile, nPosition, nCurrentCodePage);
      RecentFilesSaveA();
    }

    if (bMDI && !bDocumentReopen && (!hWndFrameActive || bModified || szCurrentFile[0]))
    {
      if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
      CreateMDIWindowA(APP_MDI_CLASSA, "", (bMdiMaximize == TRUE)?WS_MAXIMIZE:0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMdiClient, hInstance, 0);
      hWnd=hWndEdit;
    }

    //Get file write time
    GetFileTime(hFile, NULL, NULL, &ftFileTime);

    nNewLine=NEWLINE_WIN;
    HideCaret(NULL);
  }

  //Load text
  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.dwFlags=dwFlags;
  fsd.nBytesMax=-1;
  fsd.bResult=TRUE;
  FileStreamIn(&fsd);
  CloseHandle(hFile);

  if (hWnd == hWndEdit)
  {
    ShowCaret(NULL);

    if (fsd.bResult)
    {
      //Compare paths
      nFileCmp=lstrcmpiA(szCurrentFile, szFile);

      if (nFileCmp || nCurrentCodePage != nCodePage)
      {
        //Read position of the new document
        if (nRecentFiles)
        {
          RecentFilesZeroA();
          RecentFilesReadA();
          RecentFilesUpdateA(szFile, -1, nCodePage);
          RecentFilesSaveA();
          if (nFileCmp) bMenuRecentFiles=TRUE;
        }
      }

      //Update titles
      SetNewLineStatusA(hWndEdit, nNewLine, TRUE);
      SetModifyStatusA(hWndEdit, FALSE, FALSE);
      SetCodePageStatusA(nCodePage, bBOM, FALSE);

      if (nFileCmp)
      {
        UpdateTitleA(GetParent(hWndEdit), szFile);
        lstrcpynA(szCurrentFile, szFile, MAX_PATH);
      }

      //Update selection
      if (nRecentFiles && bSavePositions)
        SetTextSel(hWnd, lpdwRecentPositions[0], lpdwRecentPositions[0]);

      //Update edit window
      UpdateEdit(hWnd);

      //Print if "/p" option used in command line
      if (bGlobalPrint)
      {
        DoFilePrintA(hWnd, TRUE);
        bGlobalPrint=FALSE;

        if (!bMDI)
        {
          SendMessage(hMainWnd, WM_CLOSE, 0, 0);
        }
        else
        {
          SendMessage(hMdiClient, WM_MDIDESTROY, (WPARAM)hWndFrameActive, 0);
          if (!bSingleOpenProgram && !hWndFrameActive) SendMessage(hMainWnd, WM_CLOSE, 0, 0);
        }
      }
    }
    else nResult=EOD_STREAMIN;
  }

  End:
  SendMessage(hMainWnd, AKDN_OPENDOCUMENT_FINISH, (WPARAM)hWnd, nResult);
  return nResult;
}

int OpenDocumentW(HWND hWnd, wchar_t *wszFile, DWORD dwFlags, int nCodePage, BOOL bBOM)
{
  NOPENDOCUMENTW nodW;
  HANDLE hFile;
  FILESTREAMDATA fsd;
  HWND hWndFriend=NULL;
  int nResult=EOD_SUCCESS;
  int nDetect;
  int nFileCmp;
  BOOL bFileExist;

  if (!hWnd) hWnd=hWndEdit;

  //Notification message
  nodW.pFile=wszFile;
  nodW.nCodePage=&nCodePage;
  nodW.bBOM=&bBOM;
  nodW.dwFlags=&dwFlags;
  nodW.bProcess=TRUE;
  SendMessage(hMainWnd, AKDN_OPENDOCUMENT_START, (WPARAM)hWnd, (LPARAM)&nodW);
  if (!nodW.bProcess)
  {
    nResult=EOD_STOP;
    goto End;
  }
  if (!(bFileExist=GetFullNameW(wszFile)))
  {
    //File doesn't exist
    if (nMsgCreate == AUTOANSWER_ASK)
    {
      API_LoadStringW(hLangLib, MSG_FILE_DOES_NOT_EXIST, wbuf, BUFFER_SIZE);
      wsprintfW(wbuf2, wbuf, wszFile);
      if (MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION) == IDCANCEL)
      {
        nResult=EOD_CANCEL;
        goto End;
      }
    }
    else if (nMsgCreate == AUTOANSWER_NO)
    {
      nResult=EOD_CANCEL;
      goto End;
    }

    nCodePage=nDefaultCodePage;
    bBOM=bDefaultBOM;
  }
  else
  {
    if (hWnd == hWndEdit)
    {
      //File exists
      if (!bMDI && !bDocumentReopen && bSingleOpenFile && lstrcmpiW(wszFile, wszCurrentFile))
      {
        if ((hWndFriend=FindWindowW(APP_SDI_CLASSW, wszFile)) &&
            (hWndFriend=GetParent(hWndFriend)))
        {
          SetForegroundWindow(hWndFriend);
          OpenDocumentSendW(hWndFriend, NULL, wszFile, dwFlags, nCodePage, bBOM, FALSE);
          nResult=EOD_WINDOW_EXIST;
          goto End;
        }
      }
      if (bMDI && !bDocumentReopen && bSingleOpenFile)
      {
        if (!lstrcmpiW(wszFile, wszCurrentFile) || (hWndFriend=FindWindowExW(hMdiClient, NULL, APP_MDI_CLASSW, wszFile)))
        {
          if (hWndFriend)
          {
            SendMessage(hMdiClient, WM_MDIACTIVATE, (WPARAM)hWndFriend, 0);
            hWnd=hWndEdit;
          }
          if (SaveChangedW())
          {
            bDocumentReopen=TRUE;
            OpenDocumentW(hWnd, wszFile, dwFlags, nCodePage, bBOM);
            bDocumentReopen=FALSE;
          }
          nResult=EOD_WINDOW_EXIST;
          goto End;
        }
      }
    }

    //Autodetect code page
    if ((nDetect=AutodetectCodePageW(wszFile, dwCodepageRecognitionBuffer, dwFlags, &nCodePage, &bBOM)) < 0)
    {
      if (!bDocumentReopen)
      {
        if (nDetect == EDT_BINARY)
        {
          if (nMsgBinary == AUTOANSWER_ASK)
          {
            API_LoadStringW(hLangLib, MSG_ERROR_BINARY, wbuf, BUFFER_SIZE);
            wsprintfW(wbuf2, wbuf, wszFile);
            if (MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDCANCEL)
            {
              nResult=EOD_CANCEL;
              goto End;
            }
          }
          else if (nMsgBinary == AUTOANSWER_NO)
          {
            nResult=EOD_CANCEL;
            goto End;
          }
        }
        else
        {
          nResult=nDetect;
          goto End;
        }
      }
    }
  }

  //Check code page
  if (!CheckCodePage(nCodePage))
  {
    API_LoadStringW(hLangLib, MSG_CP_UNIMPLEMENTED, wbuf, BUFFER_SIZE);
    wsprintfW(wbuf2, wbuf, nCodePage);
    MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    nResult=EOD_CODEPAGE_ERROR;
    goto End;
  }

  //Open file
  if ((hFile=API_CreateFileW(wszFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, bFileExist?OPEN_EXISTING:OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
  {
    nResult=EOD_OPEN;
    goto End;
  }

  if (bBOM)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (nCodePage == CP_UNICODE_UCS2_BE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (nCodePage == CP_UNICODE_UTF8)
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
  }

  if (hWnd == hWndEdit)
  {
    //Save position of the previous file before load new document
    if (nRecentFiles && wszCurrentFile[0])
    {
      RecentFilesZeroW();
      RecentFilesReadW();
      RecentFilesUpdateW(wszCurrentFile, nPosition, nCurrentCodePage);
      RecentFilesSaveW();
    }

    if (bMDI && !bDocumentReopen && (!hWndFrameActive || bModified || wszCurrentFile[0]))
    {
      if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
      CreateMDIWindowW(APP_MDI_CLASSW, L"", (bMdiMaximize == TRUE)?WS_MAXIMIZE:0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMdiClient, hInstance, 0);
      hWnd=hWndEdit;
    }

    //Get file write time
    GetFileTime(hFile, NULL, NULL, &ftFileTime);

    nNewLine=NEWLINE_WIN;
    HideCaret(NULL);
  }

  //Load text
  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.dwFlags=dwFlags;
  fsd.nBytesMax=-1;
  fsd.bResult=TRUE;
  FileStreamIn(&fsd);
  CloseHandle(hFile);

  if (hWnd == hWndEdit)
  {
    ShowCaret(NULL);

    if (fsd.bResult)
    {
      //Compare paths
      nFileCmp=lstrcmpiW(wszCurrentFile, wszFile);

      if (nFileCmp || nCurrentCodePage != nCodePage)
      {
        //Read position of the new document
        if (nRecentFiles)
        {
          RecentFilesZeroW();
          RecentFilesReadW();
          RecentFilesUpdateW(wszFile, -1, nCodePage);
          RecentFilesSaveW();
          if (nFileCmp) bMenuRecentFiles=TRUE;
        }
      }

      //Update titles
      SetNewLineStatusW(hWndEdit, nNewLine, TRUE);
      SetModifyStatusW(hWndEdit, FALSE, FALSE);
      SetCodePageStatusW(nCodePage, bBOM, FALSE);

      if (nFileCmp)
      {
        UpdateTitleW(GetParent(hWndEdit), wszFile);
        lstrcpynW(wszCurrentFile, wszFile, MAX_PATH);
      }

      //Update selection
      if (nRecentFiles && bSavePositions)
        SetTextSel(hWnd, lpdwRecentPositions[0], lpdwRecentPositions[0]);

      //Update edit window
      UpdateEdit(hWnd);

      //Print if "/p" option used in command line
      if (bGlobalPrint)
      {
        DoFilePrintW(hWnd, TRUE);
        bGlobalPrint=FALSE;

        if (!bMDI)
        {
          SendMessage(hMainWnd, WM_CLOSE, 0, 0);
        }
        else
        {
          SendMessage(hMdiClient, WM_MDIDESTROY, (WPARAM)hWndFrameActive, 0);
          if (!bSingleOpenProgram && !hWndFrameActive) SendMessage(hMainWnd, WM_CLOSE, 0, 0);
        }
      }
    }
    else nResult=EOD_STREAMIN;
  }

  End:
  SendMessage(hMainWnd, AKDN_OPENDOCUMENT_FINISH, (WPARAM)hWnd, nResult);
  return nResult;
}

void FileStreamIn(FILESTREAMDATA *lpData)
{
  EDITSTREAM es;
  BUFFERSTREAMDATA bsd;
  unsigned char *pBuffer;
  wchar_t *wpBuffer;
  int nBufferLen;
  DWORD dwFileSize;
  DWORD dwBytesRead;
  DWORD dwCharsConverted;
  DWORD i;

  if ((dwFileSize=GetFileSize(lpData->hFile, NULL)) != INVALID_FILE_SIZE)
  {
    if (lpData->nBytesMax == -1)
      lpData->nBytesMax=dwFileSize;

    if (lpData->nCodePage == CP_UNICODE_UCS2_LE || lpData->nCodePage == CP_UNICODE_UCS2_BE)
      nBufferLen=lpData->nBytesMax;
    else
      nBufferLen=lpData->nBytesMax * sizeof(wchar_t);

    if (wpBuffer=(wchar_t *)API_HeapAlloc(hHeap, 0, nBufferLen + 2))
    {
      if (lpData->nCodePage == CP_UNICODE_UCS2_LE || lpData->nCodePage == CP_UNICODE_UCS2_BE)
        pBuffer=(unsigned char *)wpBuffer;
      else
        pBuffer=(unsigned char *)wpBuffer + lpData->nBytesMax;

      //Read data from file
      if (API_ReadFile(lpData->hFile, pBuffer, lpData->nBytesMax, &dwBytesRead, NULL))
      {
        //Translate data to UNICODE
        if (lpData->nCodePage == CP_UNICODE_UCS2_LE)
        {
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (lpData->nCodePage == CP_UNICODE_UCS2_BE)
        {
          ChangeByteOrder(pBuffer, dwBytesRead);
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else
        {
          if (lpData->nCodePage == CP_UNICODE_UTF8)
            dwCharsConverted=UTF8toUTF16(pBuffer, dwBytesRead, NULL, wpBuffer, dwBytesRead);
          else
            dwCharsConverted=MultiByteToWideChar(lpData->nCodePage, 0, (char *)pBuffer, dwBytesRead, wpBuffer, dwBytesRead);
        }
        wpBuffer[dwCharsConverted]='\0';

        //Detect new line
        for (i=0; i < dwCharsConverted; ++i)
        {
          if (wpBuffer[i] == '\r')
          {
            break;
          }
          else if (wpBuffer[i] == '\n')
          {
            SetNewLineStatusW(hWndEdit, NEWLINE_UNIX, FALSE);
            break;
          }
        }

        //Send to RichEdit
        bsd.wpData=wpBuffer;
        bsd.nDataLen=dwCharsConverted;
        bsd.nCount=0;
        es.dwCookie=(DWORD)&bsd;
        es.pfnCallback=InputStreamCallback;
        SendMessage(lpData->hWnd, EM_STREAMIN, SF_TEXT|SF_UNICODE, (LPARAM)&es);

        if (es.dwError) lpData->bResult=FALSE;
      }
      else lpData->bResult=FALSE;

      API_HeapFree(hHeap, 0, (LPVOID)wpBuffer);
    }
    else lpData->bResult=FALSE;
  }
  else lpData->bResult=FALSE;
}

DWORD CALLBACK InputStreamCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  BUFFERSTREAMDATA *lpData=(BUFFERSTREAMDATA *)dwCookie;
  wchar_t *wpSrc=lpData->wpData;
  wchar_t *wpDest=(wchar_t *)pbBuff;
  int i;

  for (i=0, cb=cb / sizeof(wchar_t) - 1; i < cb; ++i, ++lpData->nCount)
  {
    if (lpData->nDataLen == -1)
    {
      if (!wpSrc[lpData->nCount])
        break;
    }
    else
    {
      if (lpData->nCount >= lpData->nDataLen)
        break;
    }

    if (wpSrc[lpData->nCount] == '\r')
    {
      wpDest[i]='\n';

      if (wpSrc[lpData->nCount + 1] == '\r' &&
          wpSrc[lpData->nCount + 2] == '\n')
      {
        lpData->nCount+=2;
      }
      else if (wpSrc[lpData->nCount + 1] == '\n')
      {
        lpData->nCount+=1;
      }
    }
    else wpDest[i]=wpSrc[lpData->nCount];
  }
  *pcb=i * sizeof(wchar_t);

  return 0;
}

int SaveDocumentA(HWND hWnd, char *szFile, int nCodePage, BOOL bBOM, DWORD dwFlags)
{
  NSAVEDOCUMENTA nsd;
  WIN32_FIND_DATAA wfdA;
  HANDLE hFile;
  FILESTREAMDATA fsd;
  DWORD dwAttr;
  unsigned int nBytesWritten;
  int nResult=ESD_SUCCESS;
  int nWrite=0;
  int nFileCmp;
  int nCodePageCmp;

  //Notification message
  nsd.pFile=szFile;
  nsd.nCodePage=&nCodePage;
  nsd.bBOM=&bBOM;
  nsd.bProcess=TRUE;
  SendMessage(hMainWnd, AKDN_SAVEDOCUMENT_START, (WPARAM)hWnd, (LPARAM)&nsd);
  if (!nsd.bProcess)
  {
    nResult=ESD_STOP;
    goto End;
  }

  //Check code page
  if (!CheckCodePage(nCodePage))
  {
    API_LoadStringA(hLangLib, MSG_CP_UNIMPLEMENTED, buf, BUFFER_SIZE);
    wsprintfA(buf2, buf, nCodePage);
    MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
    nResult=ESD_CODEPAGE_ERROR;
    goto End;
  }

  dwAttr=GetFileAttributesA(szFile);
  if (dwAttr != 0xFFFFFFFF)
  {
    if (bSaveInReadOnlyMsg && (dwAttr & FILE_ATTRIBUTE_READONLY))
    {
      API_LoadStringA(hLangLib, MSG_SAVEIN_READONLY, buf, BUFFER_SIZE);
      wsprintfA(buf2, buf, szFile);
      if (MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_YESNO|MB_ICONEXCLAMATION) == IDNO)
      {
        nResult=ESD_READONLY;
        goto End;
      }
    }
    if ((dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
      SetFileAttributesA(szFile, dwAttr & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN & ~FILE_ATTRIBUTE_SYSTEM);
    if (bSaveTime)
    {
      if ((hFile=FindFirstFileA(szFile, &wfdA)) != INVALID_HANDLE_VALUE)
        FindClose(hFile);
    }
  }

  hFile=API_CreateFileA(szFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, (dwAttr != 0xFFFFFFFF)?TRUNCATE_EXISTING:CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    nResult=ESD_OPEN;
    goto BackAttr;
  }

  if (bBOM)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
    {
      if (nCodePage == CP_UNICODE_UCS2_LE)
        nWrite=API_WriteFile(hFile, "\xFF\xFE", 2, (DWORD *)&nBytesWritten, NULL);
      else if (nCodePage == CP_UNICODE_UCS2_BE)
        nWrite=API_WriteFile(hFile, "\xFE\xFF", 2, (DWORD *)&nBytesWritten, NULL);
      else if (nCodePage == CP_UNICODE_UTF8)
        nWrite=API_WriteFile(hFile, "\xEF\xBB\xBF", 3, (DWORD *)&nBytesWritten, NULL);

      if (!nWrite)
      {
        nResult=ESD_WRITE;
        goto BackAttr;
      }
    }
    else bBOM=FALSE;
  }

  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.dwFlags=dwFlags;
  fsd.bResult=TRUE;
  FileStreamOut(&fsd);
  CloseHandle(hFile);

  if (fsd.bResult)
  {
    //Change back file attributes
    if (dwAttr != 0xFFFFFFFF)
    {
      if (!(dwAttr & FILE_ATTRIBUTE_ARCHIVE) || (dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
        SetFileAttributesA(szFile, dwAttr|FILE_ATTRIBUTE_ARCHIVE);
      if (bSaveTime)
      {
        if ((hFile=API_CreateFileA(szFile, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL)) != INVALID_HANDLE_VALUE)
        {
          SetFileTime(hFile, NULL, NULL, &wfdA.ftLastWriteTime);
          CloseHandle(hFile);
        }
      }
    }

    //Update file info
    if (dwFlags & SD_UPDATE)
    {
      //Compare
      nFileCmp=lstrcmpiA(szCurrentFile, szFile);
      nCodePageCmp=nCurrentCodePage - nCodePage;

      if (nFileCmp || nCodePageCmp)
      {
        //Save position of the document
        if (nRecentFiles)
        {
          RecentFilesZeroA();
          RecentFilesReadA();
          RecentFilesUpdateA(szFile, nPosition, nCodePage);
          RecentFilesSaveA();
          if (nFileCmp) bMenuRecentFiles=TRUE;
        }
      }

      if (hWnd == hWndEdit)
      {
        GetFileWriteTimeA(szFile, &ftFileTime);
        SetModifyStatusA(hWndEdit, FALSE, FALSE);
        SetCodePageStatusA(nCodePage, bBOM, FALSE);

        if (nFileCmp)
        {
          UpdateTitleA(GetParent(hWndEdit), szFile);
          lstrcpynA(szCurrentFile, szFile, MAX_PATH);
        }
      }
      else
      {
        FILETIME ft;
        WNDFRAMEA *wf;
        HWND hWndFrame;

        if (hWndFrame=GetParent(hWnd))
        {
          if (wf=(WNDFRAMEA *)GetWindowLongA(hWndFrame, GWL_USERDATA))
          {
            GetFileWriteTimeA(szFile, &ft);
            wf->ei.nCodePage=nCodePage;
            wf->ei.bBOM=bBOM;
            wf->ei.bModified=FALSE;
            wf->ft=ft;

            if (lstrcmpiA(wf->szFile, szFile))
            {
              UpdateTitleA(hWndFrame, szFile);
              lstrcpynA(wf->szFile, szFile, MAX_PATH);
            }
          }
        }
      }
      if ((dwFlags & SD_SELECTION) || nCodePageCmp)
      {
        bDocumentReopen=TRUE;
        OpenDocumentA(hWnd, szCurrentFile, 0, nCurrentCodePage, bCurrentBOM);
        bDocumentReopen=FALSE;
      }
    }
    goto End;
  }
  else
  {
    nResult=ESD_STREAMOUT;
    goto BackAttr;
  }

  BackAttr:
  if (dwAttr != 0xFFFFFFFF)
  {
    if ((dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
      SetFileAttributesA(szFile, dwAttr);
  }

  End:
  SendMessage(hMainWnd, AKDN_SAVEDOCUMENT_FINISH, (WPARAM)hWnd, nResult);
  return nResult;
}

int SaveDocumentW(HWND hWnd, wchar_t *wszFile, int nCodePage, BOOL bBOM, DWORD dwFlags)
{
  NSAVEDOCUMENTW nsdW;
  WIN32_FIND_DATAW wfdW;
  HANDLE hFile;
  FILESTREAMDATA fsd;
  DWORD dwAttr;
  unsigned int nBytesWritten;
  int nResult=ESD_SUCCESS;
  int nWrite=0;
  int nFileCmp;
  int nCodePageCmp;

  //Notification message
  nsdW.pFile=wszFile;
  nsdW.nCodePage=&nCodePage;
  nsdW.bBOM=&bBOM;
  nsdW.bProcess=TRUE;
  SendMessage(hMainWnd, AKDN_SAVEDOCUMENT_START, (WPARAM)hWnd, (LPARAM)&nsdW);
  if (!nsdW.bProcess)
  {
    nResult=ESD_STOP;
    goto End;
  }

  //Check code page
  if (!CheckCodePage(nCodePage))
  {
    API_LoadStringW(hLangLib, MSG_CP_UNIMPLEMENTED, wbuf, BUFFER_SIZE);
    wsprintfW(wbuf2, wbuf, nCodePage);
    MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    nResult=ESD_CODEPAGE_ERROR;
    goto End;
  }

  dwAttr=GetFileAttributesW(wszFile);
  if (dwAttr != 0xFFFFFFFF)
  {
    if (bSaveInReadOnlyMsg && (dwAttr & FILE_ATTRIBUTE_READONLY))
    {
      API_LoadStringW(hLangLib, MSG_SAVEIN_READONLY, wbuf, BUFFER_SIZE);
      wsprintfW(wbuf2, wbuf, wszFile);
      if (MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_YESNO|MB_ICONEXCLAMATION) == IDNO)
      {
        nResult=ESD_READONLY;
        goto End;
      }
    }
    if ((dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
      SetFileAttributesW(wszFile, dwAttr & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN & ~FILE_ATTRIBUTE_SYSTEM);
    if (bSaveTime)
    {
      if ((hFile=FindFirstFileW(wszFile, &wfdW)) != INVALID_HANDLE_VALUE)
        FindClose(hFile);
    }
  }

  hFile=API_CreateFileW(wszFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, (dwAttr != 0xFFFFFFFF)?TRUNCATE_EXISTING:CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    nResult=ESD_OPEN;
    goto BackAttr;
  }

  if (bBOM)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
    {
      if (nCodePage == CP_UNICODE_UCS2_LE)
        nWrite=API_WriteFile(hFile, "\xFF\xFE", 2, (DWORD *)&nBytesWritten, NULL);
      else if (nCodePage == CP_UNICODE_UCS2_BE)
        nWrite=API_WriteFile(hFile, "\xFE\xFF", 2, (DWORD *)&nBytesWritten, NULL);
      else if (nCodePage == CP_UNICODE_UTF8)
        nWrite=API_WriteFile(hFile, "\xEF\xBB\xBF", 3, (DWORD *)&nBytesWritten, NULL);

      if (!nWrite)
      {
        nResult=ESD_WRITE;
        goto BackAttr;
      }
    }
    else bBOM=FALSE;
  }

  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.dwFlags=dwFlags;
  fsd.bResult=TRUE;
  FileStreamOut(&fsd);
  CloseHandle(hFile);

  if (fsd.bResult)
  {
    //Change back file attributes
    if (dwAttr != 0xFFFFFFFF)
    {
      if (!(dwAttr & FILE_ATTRIBUTE_ARCHIVE) || (dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
        SetFileAttributesW(wszFile, dwAttr|FILE_ATTRIBUTE_ARCHIVE);
      if (bSaveTime)
      {
        if ((hFile=API_CreateFileW(wszFile, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL)) != INVALID_HANDLE_VALUE)
        {
          SetFileTime(hFile, NULL, NULL, &wfdW.ftLastWriteTime);
          CloseHandle(hFile);
        }
      }
    }

    //Update file info
    if (dwFlags & SD_UPDATE)
    {
      //Compare
      nFileCmp=lstrcmpiW(wszCurrentFile, wszFile);
      nCodePageCmp=nCurrentCodePage - nCodePage;

      if (nFileCmp || nCodePageCmp)
      {
        //Save position of the document
        if (nRecentFiles)
        {
          RecentFilesZeroW();
          RecentFilesReadW();
          RecentFilesUpdateW(wszFile, nPosition, nCodePage);
          RecentFilesSaveW();
          if (nFileCmp) bMenuRecentFiles=TRUE;
        }
      }

      if (hWnd == hWndEdit)
      {
        GetFileWriteTimeW(wszFile, &ftFileTime);
        SetModifyStatusW(hWndEdit, FALSE, FALSE);
        SetCodePageStatusW(nCodePage, bBOM, FALSE);

        if (nFileCmp)
        {
          UpdateTitleW(GetParent(hWndEdit), wszFile);
          lstrcpynW(wszCurrentFile, wszFile, MAX_PATH);
        }
      }
      else
      {
        FILETIME ft;
        WNDFRAMEW *wf;
        HWND hWndFrame;

        if (hWndFrame=GetParent(hWnd))
        {
          if (wf=(WNDFRAMEW *)GetWindowLongW(hWndFrame, GWL_USERDATA))
          {
            GetFileWriteTimeW(wszFile, &ft);
            wf->ei.nCodePage=nCodePage;
            wf->ei.bBOM=bBOM;
            wf->ei.bModified=FALSE;
            wf->ft=ft;

            if (lstrcmpiW(wf->szFile, wszFile))
            {
              UpdateTitleW(hWndFrame, wszFile);
              lstrcpynW(wf->szFile, wszFile, MAX_PATH);
            }
          }
        }
      }
      if ((dwFlags & SD_SELECTION) || nCodePageCmp)
      {
        bDocumentReopen=TRUE;
        OpenDocumentW(hWnd, wszCurrentFile, 0, nCurrentCodePage, bCurrentBOM);
        bDocumentReopen=FALSE;
      }
    }
    goto End;
  }
  else
  {
    nResult=ESD_STREAMOUT;
    goto BackAttr;
  }

  BackAttr:
  if (dwAttr != 0xFFFFFFFF)
  {
    if ((dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
      SetFileAttributesW(wszFile, dwAttr);
  }

  End:
  SendMessage(hMainWnd, AKDN_SAVEDOCUMENT_FINISH, (WPARAM)hWnd, nResult);
  return nResult;
}

void FileStreamOut(FILESTREAMDATA *lpData)
{
  EDITSTREAM es;

  es.dwCookie=(DWORD)lpData;
  es.pfnCallback=OutputStreamCallback;
  SendMessage(lpData->hWnd, EM_STREAMOUT, SF_TEXT|SF_UNICODE|(lpData->dwFlags & SD_SELECTION?SFF_SELECTION:0), (LPARAM)&es);

  if (es.dwError) lpData->bResult=FALSE;
}

DWORD CALLBACK OutputStreamCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  FILESTREAMDATA *lpData=(FILESTREAMDATA *)dwCookie;
  unsigned char *pDataToWrite=pbBuff;
  DWORD dwCharsConverted;
  DWORD dwBytesToWrite;
  DWORD dwBytesWritten;

  if (nNewLine == NEWLINE_UNIX)
  {
    dwBytesToWrite=TranslateNewLinesToUnixW((wchar_t *)pbBuff, cb / sizeof(wchar_t)) * sizeof(wchar_t);
  }
  else dwBytesToWrite=cb;

  if (lpData->nCodePage == CP_UNICODE_UCS2_LE)
  {
  }
  else if (lpData->nCodePage == CP_UNICODE_UCS2_BE)
  {
    ChangeByteOrder(pbBuff, dwBytesToWrite);
  }
  else
  {
    if (lpData->nCodePage == CP_UNICODE_UTF8)
      dwCharsConverted=UTF16toUTF8((wchar_t *)pbBuff, dwBytesToWrite / sizeof(wchar_t), NULL, (char *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE);
    else
      dwCharsConverted=WideCharToMultiByte(lpData->nCodePage, 0, (wchar_t *)pbBuff, dwBytesToWrite / sizeof(wchar_t), (char *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE, NULL, NULL);

    pDataToWrite=pcTranslateBuffer;
    dwBytesToWrite=dwCharsConverted;
  }

  if (!API_WriteFile(lpData->hFile, pDataToWrite, dwBytesToWrite, &dwBytesWritten, NULL))
    return 0xFFFFFFFF;

  *pcb=cb;

  return 0;
}

BOOL OpenDirectoryA(char *pPath, BOOL bSubDir)
{
  char szName[MAX_PATH];
  WIN32_FIND_DATAA wfdA;
  HANDLE hSearch;

  if (bSubDir)
  {
    wsprintfA(szName, "%s\\*.*", pPath);

    if ((hSearch=FindFirstFileA(szName, &wfdA)) != INVALID_HANDLE_VALUE)
    {
      do
      {
        if (wfdA.cFileName[0] == '.' && (wfdA.cFileName[1] == '\0' || (wfdA.cFileName[1] == '.' && wfdA.cFileName[2] == '\0'))) continue;

        wsprintfA(szName, "%s\\%s", pPath, wfdA.cFileName);

        if (wfdA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
          OpenDirectoryA(szName, TRUE);
        else
          OpenDocumentA(hWndEdit, szName, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE);
      }
      while (FindNextFileA(hSearch, &wfdA));

      FindClose(hSearch);
    }
    else return FALSE;
  }
  return TRUE;
}

BOOL OpenDirectoryW(wchar_t *wpPath, BOOL bSubDir)
{
  wchar_t wszName[MAX_PATH];
  WIN32_FIND_DATAW wfdW;
  HANDLE hSearch;

  if (bSubDir)
  {
    wsprintfW(wszName, L"%s\\*.*", wpPath);

    if ((hSearch=FindFirstFileW(wszName, &wfdW)) != INVALID_HANDLE_VALUE)
    {
      do
      {
        if (wfdW.cFileName[0] == '.' && (wfdW.cFileName[1] == '\0' || (wfdW.cFileName[1] == '.' && wfdW.cFileName[2] == '\0'))) continue;

        wsprintfW(wszName, L"%s\\%s", wpPath, wfdW.cFileName);

        if (wfdW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
          OpenDirectoryW(wszName, TRUE);
        else
          OpenDocumentW(hWndEdit, wszName, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE);
      }
      while (FindNextFileW(hSearch, &wfdW));

      FindClose(hSearch);
    }
    else return FALSE;
  }
  return TRUE;
}


//// Print

unsigned int CALLBACK PrintPageSetupDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFontGroup;
  static HWND hWndFontCheck;
  static HWND hWndFontButton;
  static HWND hWndHeadlineGroup;
  static HWND hWndHeaderCheck;
  static HWND hWndHeaderEdit;
  static HWND hWndHeaderHelp;
  static HWND hWndFooterCheck;
  static HWND hWndFooterEdit;
  static HWND hWndFooterHelp;
  static HFONT hPrintFont;
  static HFONT hGuiFont;
  static HBITMAP hBitmapDownArrow;
  static LOGFONTA lfTmpA;
  static BOOL bFontChanged=FALSE;
  BOOL bState;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    RECT rcDlg;
    RECT rcControl;
    HWND hWndOK;
    HWND hWndCancel;
    HWND hWndPrinter;
    int nExtend=77;

    hBitmapDownArrow=(HBITMAP)API_LoadImageA(hLangLib, MAKEINTRESOURCEA(IDB_BITMAP_DOWNARROW), IMAGE_BITMAP, 0, 0, 0);
    memcpy(&lfTmpA, &lfPrintFontA, sizeof(LOGFONTA));
    hPrintFont=(HFONT)CreateFontIndirectA(&lfTmpA);
    hGuiFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
    hWndPrinter=GetDlgItem(hDlg, IDC_PSD_PRINTER_BUTTON);

    GetWindowRect(hDlg, &rcDlg);
    rcDlg.right-=rcDlg.left;
    rcDlg.bottom-=rcDlg.top;
    if (rcDlg.top - nExtend >= 0)
      rcDlg.top-=nExtend;
    else
      rcDlg.top=0;
    SetWindowPos(hDlg, NULL, rcDlg.left, rcDlg.top, rcDlg.right, rcDlg.bottom + nExtend, SWP_NOZORDER|SWP_NOACTIVATE);

    if (hWndPrinter)
    {
      GetWindowPos(hWndPrinter, hDlg, &rcControl);
      SetWindowPos(hWndPrinter, NULL, rcControl.left, rcControl.top + nExtend, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
    }
    if (hWndOK)
    {
      GetWindowPos(hWndOK, hDlg, &rcControl);
      SetWindowPos(hWndOK, NULL, rcControl.left, rcControl.top + nExtend, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
    }
    if (hWndCancel)
    {
      GetWindowPos(hWndCancel, hDlg, &rcControl);
      SetWindowPos(hWndCancel, NULL, rcControl.left, rcControl.top + nExtend, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
    }

    //Group box
    hWndFontGroup=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                         12, rcControl.top - 6, 96, 70,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_GROUP,
                         hInstance,
                         NULL);
    //Font checkbox
    hWndFontCheck=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,
                         24, rcControl.top + 26, 16, 16,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_CHECK,
                         hInstance,
                         NULL);
    //Font button
    hWndFontButton=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP,
                         46, rcControl.top + 22, 46, 23,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_BUTTON,
                         hInstance,
                         NULL);
    //Group box
    hWndHeadlineGroup=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                         120, rcControl.top - 6, rcDlg.right - 138, 70,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_GROUP,
                         hInstance,
                         NULL);
    //Header checkbox
    hWndHeaderCheck=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|BS_RIGHTBUTTON,
                         126, rcControl.top + 16, 66, 16,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_CHECK,
                         hInstance,
                         NULL);
    //Header edit
    hWndHeaderEdit=CreateWindowExA(WS_EX_CLIENTEDGE,
                         "EDIT",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
                         200, rcControl.top + 14, 118, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_EDIT,
                         hInstance,
                         NULL);
    //Header help
    hWndHeaderHelp=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_BITMAP,
                         320, rcControl.top + 14, 18, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_HELP,
                         hInstance,
                         NULL);
    //Footer checkbox
    hWndFooterCheck=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|BS_RIGHTBUTTON,
                         126, rcControl.top + 36, 66, 16,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_CHECK,
                         hInstance,
                         NULL);
    //Footer edit
    hWndFooterEdit=CreateWindowExA(WS_EX_CLIENTEDGE,
                         "EDIT",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
                         200, rcControl.top + 34, 118, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_EDIT,
                         hInstance,
                         NULL);
    //Footer help
    hWndFooterHelp=CreateWindowExA(0,
                         "BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_BITMAP,
                         320, rcControl.top + 34, 18, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_HELP,
                         hInstance,
                         NULL);

    SendMessage(hWndFontGroup, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringA(hLangLib, STR_FONT, buf, BUFFER_SIZE);
    SetWindowTextA(hWndFontGroup, buf);
    if (bPrintFontEnable)
      SendMessage(hWndFontCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndFontButton, FALSE);
    SendMessage(hWndFontButton, WM_SETFONT, (WPARAM)hPrintFont, TRUE);
    SetWindowTextA(hWndFontButton, "...");

    SendMessage(hWndHeadlineGroup, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringA(hLangLib, STR_HEADLINE, buf, BUFFER_SIZE);
    SetWindowTextA(hWndHeadlineGroup, buf);

    SendMessage(hWndHeaderCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringA(hLangLib, STR_TOP, buf, BUFFER_SIZE);
    SetWindowTextA(hWndHeaderCheck, buf);
    if (bPrintHeaderEnable)
      SendMessage(hWndHeaderCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      EnableWindow(hWndHeaderEdit, FALSE);
      EnableWindow(hWndHeaderHelp, FALSE);
    }
    SendMessage(hWndHeaderEdit, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndHeaderEdit, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    SetWindowTextA(hWndHeaderEdit, szPrintHeader);
    SendMessage(hWndHeaderHelp, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapDownArrow);

    SendMessage(hWndFooterCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringA(hLangLib, STR_BOTTOM, buf, BUFFER_SIZE);
    SetWindowTextA(hWndFooterCheck, buf);
    if (bPrintFooterEnable)
      SendMessage(hWndFooterCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      EnableWindow(hWndFooterEdit, FALSE);
      EnableWindow(hWndFooterHelp, FALSE);
    }
    SendMessage(hWndFooterEdit, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndFooterEdit, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    SetWindowTextA(hWndFooterEdit, szPrintFooter);
    SendMessage(hWndFooterHelp, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapDownArrow);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PSD_FONT_CHECK)
    {
      bState=SendMessage(hWndFontCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFontButton, bState);
    }
    else if (LOWORD(wParam) == IDC_PSD_HEADLINE_HEADER_CHECK)
    {
      bState=SendMessage(hWndHeaderCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndHeaderEdit, bState);
      EnableWindow(hWndHeaderHelp, bState);
    }
    else if (LOWORD(wParam) == IDC_PSD_HEADLINE_FOOTER_CHECK)
    {
      bState=SendMessage(hWndFooterCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFooterEdit, bState);
      EnableWindow(hWndFooterHelp, bState);
    }
    else if (LOWORD(wParam) == IDC_PSD_HEADLINE_HEADER_HELP ||
             LOWORD(wParam) == IDC_PSD_HEADLINE_FOOTER_HELP)
    {
      HWND hWnd=NULL;
      RECT rc;

      if (LOWORD(wParam) == IDC_PSD_HEADLINE_HEADER_HELP)
        hWnd=hWndHeaderEdit;
      if (LOWORD(wParam) == IDC_PSD_HEADLINE_FOOTER_HELP)
        hWnd=hWndFooterEdit;

      GetWindowRect((HWND)lParam, &rc);
      if (i=TrackPopupMenu(hPopupHeadline, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, rc.left, rc.top + (rc.bottom - rc.top), 0, hDlg, NULL))
      {
        if (i == IDM_POPUP_HEADLINE_SYMBOL)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%%");
        else if (i == IDM_POPUP_HEADLINE_PAGE_NUMBER)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%n[1]");
        else if (i == IDM_POPUP_HEADLINE_FILE_NAME)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%f");
        else if (i == IDM_POPUP_HEADLINE_ALIGN_CENTER)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%c");
        else if (i == IDM_POPUP_HEADLINE_ALIGN_LEFT)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%l");
        else if (i == IDM_POPUP_HEADLINE_ALIGN_RIGHT)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%r");
        SetFocus(hWnd);
      }
    }
    else if (LOWORD(wParam) == IDC_PSD_FONT_BUTTON)
    {
      if (DoViewFontA(hDlg, &lfTmpA))
      {
        if (hPrintFont) DeleteObject(hPrintFont);
        hPrintFont=(HFONT)CreateFontIndirectA(&lfTmpA);
        SendMessage(hWndFontButton, WM_SETFONT, (WPARAM)hPrintFont, TRUE);
        bFontChanged=TRUE;
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (hBitmapDownArrow) DeleteObject(hBitmapDownArrow);
      if (hPrintFont) DeleteObject(hPrintFont);

      bPrintFontEnable=SendMessage(hWndFontCheck, BM_GETCHECK, 0, 0);
      memcpy(&lfPrintFontA, &lfTmpA, sizeof(LOGFONTA));
      if (bFontChanged) bPrintFontChanged=TRUE;

      bPrintHeaderEnable=SendMessage(hWndHeaderCheck, BM_GETCHECK, 0, 0);
      GetWindowTextA(hWndHeaderEdit, szPrintHeader, MAX_PATH);

      bPrintFooterEnable=SendMessage(hWndFooterCheck, BM_GETCHECK, 0, 0);
      GetWindowTextA(hWndFooterEdit, szPrintFooter, MAX_PATH);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      if (hBitmapDownArrow) DeleteObject(hBitmapDownArrow);
      if (hPrintFont) DeleteObject(hPrintFont);
    }
  }
  return FALSE;
}

unsigned int CALLBACK PrintPageSetupDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFontGroup;
  static HWND hWndFontCheck;
  static HWND hWndFontButton;
  static HWND hWndHeadlineGroup;
  static HWND hWndHeaderCheck;
  static HWND hWndHeaderEdit;
  static HWND hWndHeaderHelp;
  static HWND hWndFooterCheck;
  static HWND hWndFooterEdit;
  static HWND hWndFooterHelp;
  static HFONT hPrintFont;
  static HFONT hGuiFont;
  static HBITMAP hBitmapDownArrow;
  static LOGFONTW lfTmpW;
  static BOOL bFontChanged=FALSE;
  BOOL bState;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    RECT rcDlg;
    RECT rcControl;
    HWND hWndOK;
    HWND hWndCancel;
    HWND hWndPrinter;
    int nExtend=77;

    hBitmapDownArrow=(HBITMAP)API_LoadImageW(hLangLib, MAKEINTRESOURCEW(IDB_BITMAP_DOWNARROW), IMAGE_BITMAP, 0, 0, 0);
    memcpy(&lfTmpW, &lfPrintFontW, sizeof(LOGFONTW));
    hPrintFont=(HFONT)CreateFontIndirectW(&lfTmpW);
    hGuiFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
    hWndPrinter=GetDlgItem(hDlg, IDC_PSD_PRINTER_BUTTON);

    GetWindowRect(hDlg, &rcDlg);
    rcDlg.right-=rcDlg.left;
    rcDlg.bottom-=rcDlg.top;
    if (rcDlg.top - nExtend >= 0)
      rcDlg.top-=nExtend;
    else
      rcDlg.top=0;
    SetWindowPos(hDlg, NULL, rcDlg.left, rcDlg.top, rcDlg.right, rcDlg.bottom + nExtend, SWP_NOZORDER|SWP_NOACTIVATE);

    if (hWndPrinter)
    {
      GetWindowPos(hWndPrinter, hDlg, &rcControl);
      SetWindowPos(hWndPrinter, NULL, rcControl.left, rcControl.top + nExtend, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
    }
    if (hWndOK)
    {
      GetWindowPos(hWndOK, hDlg, &rcControl);
      SetWindowPos(hWndOK, NULL, rcControl.left, rcControl.top + nExtend, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
    }
    if (hWndCancel)
    {
      GetWindowPos(hWndCancel, hDlg, &rcControl);
      SetWindowPos(hWndCancel, NULL, rcControl.left, rcControl.top + nExtend, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
    }

    //Group box
    hWndFontGroup=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                         12, rcControl.top - 6, 96, 70,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_GROUP,
                         hInstance,
                         NULL);
    //Font checkbox
    hWndFontCheck=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,
                         24, rcControl.top + 26, 16, 16,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_CHECK,
                         hInstance,
                         NULL);
    //Font button
    hWndFontButton=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP,
                         46, rcControl.top + 22, 46, 23,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_BUTTON,
                         hInstance,
                         NULL);
    //Group box
    hWndHeadlineGroup=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                         120, rcControl.top - 6, rcDlg.right - 138, 70,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_GROUP,
                         hInstance,
                         NULL);
    //Header checkbox
    hWndHeaderCheck=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|BS_RIGHTBUTTON,
                         126, rcControl.top + 16, 66, 16,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_CHECK,
                         hInstance,
                         NULL);
    //Header edit
    hWndHeaderEdit=CreateWindowExW(WS_EX_CLIENTEDGE,
                         L"EDIT",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
                         200, rcControl.top + 14, 118, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_EDIT,
                         hInstance,
                         NULL);
    //Header help
    hWndHeaderHelp=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_BITMAP,
                         320, rcControl.top + 14, 18, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_HELP,
                         hInstance,
                         NULL);
    //Footer checkbox
    hWndFooterCheck=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|BS_RIGHTBUTTON,
                         126, rcControl.top + 36, 66, 16,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_CHECK,
                         hInstance,
                         NULL);
    //Footer edit
    hWndFooterEdit=CreateWindowExW(WS_EX_CLIENTEDGE,
                         L"EDIT",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
                         200, rcControl.top + 34, 118, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_EDIT,
                         hInstance,
                         NULL);
    //Footer help
    hWndFooterHelp=CreateWindowExW(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_BITMAP,
                         320, rcControl.top + 34, 18, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_HELP,
                         hInstance,
                         NULL);

    SendMessage(hWndFontGroup, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringW(hLangLib, STR_FONT, wbuf, BUFFER_SIZE);
    SetWindowTextW(hWndFontGroup, wbuf);
    if (bPrintFontEnable)
      SendMessage(hWndFontCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndFontButton, FALSE);
    SendMessage(hWndFontButton, WM_SETFONT, (WPARAM)hPrintFont, TRUE);
    SetWindowTextW(hWndFontButton, L"...");

    SendMessage(hWndHeadlineGroup, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringW(hLangLib, STR_HEADLINE, wbuf, BUFFER_SIZE);
    SetWindowTextW(hWndHeadlineGroup, wbuf);

    SendMessage(hWndHeaderCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringW(hLangLib, STR_TOP, wbuf, BUFFER_SIZE);
    SetWindowTextW(hWndHeaderCheck, wbuf);
    if (bPrintHeaderEnable)
      SendMessage(hWndHeaderCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      EnableWindow(hWndHeaderEdit, FALSE);
      EnableWindow(hWndHeaderHelp, FALSE);
    }
    SendMessage(hWndHeaderEdit, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndHeaderEdit, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    SetWindowTextW(hWndHeaderEdit, wszPrintHeader);
    SendMessage(hWndHeaderHelp, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapDownArrow);

    SendMessage(hWndFooterCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    API_LoadStringW(hLangLib, STR_BOTTOM, wbuf, BUFFER_SIZE);
    SetWindowTextW(hWndFooterCheck, wbuf);
    if (bPrintFooterEnable)
      SendMessage(hWndFooterCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      EnableWindow(hWndFooterEdit, FALSE);
      EnableWindow(hWndFooterHelp, FALSE);
    }
    SendMessage(hWndFooterEdit, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndFooterEdit, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    SetWindowTextW(hWndFooterEdit, wszPrintFooter);
    SendMessage(hWndFooterHelp, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapDownArrow);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PSD_FONT_CHECK)
    {
      bState=SendMessage(hWndFontCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFontButton, bState);
    }
    else if (LOWORD(wParam) == IDC_PSD_HEADLINE_HEADER_CHECK)
    {
      bState=SendMessage(hWndHeaderCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndHeaderEdit, bState);
      EnableWindow(hWndHeaderHelp, bState);
    }
    else if (LOWORD(wParam) == IDC_PSD_HEADLINE_FOOTER_CHECK)
    {
      bState=SendMessage(hWndFooterCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFooterEdit, bState);
      EnableWindow(hWndFooterHelp, bState);
    }
    else if (LOWORD(wParam) == IDC_PSD_HEADLINE_HEADER_HELP ||
             LOWORD(wParam) == IDC_PSD_HEADLINE_FOOTER_HELP)
    {
      HWND hWnd=NULL;
      RECT rc;

      if (LOWORD(wParam) == IDC_PSD_HEADLINE_HEADER_HELP)
        hWnd=hWndHeaderEdit;
      if (LOWORD(wParam) == IDC_PSD_HEADLINE_FOOTER_HELP)
        hWnd=hWndFooterEdit;

      GetWindowRect((HWND)lParam, &rc);
      if (i=TrackPopupMenu(hPopupHeadline, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, rc.left, rc.top + (rc.bottom - rc.top), 0, hDlg, NULL))
      {
        if (i == IDM_POPUP_HEADLINE_SYMBOL)
          SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"%%");
        else if (i == IDM_POPUP_HEADLINE_PAGE_NUMBER)
          SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"%n[1]");
        else if (i == IDM_POPUP_HEADLINE_FILE_NAME)
          SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"%f");
        else if (i == IDM_POPUP_HEADLINE_ALIGN_CENTER)
          SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"%c");
        else if (i == IDM_POPUP_HEADLINE_ALIGN_LEFT)
          SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"%l");
        else if (i == IDM_POPUP_HEADLINE_ALIGN_RIGHT)
          SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"%r");
        SetFocus(hWnd);
      }
    }
    else if (LOWORD(wParam) == IDC_PSD_FONT_BUTTON)
    {
      if (DoViewFontW(hDlg, &lfTmpW))
      {
        if (hPrintFont) DeleteObject(hPrintFont);
        hPrintFont=(HFONT)CreateFontIndirectW(&lfTmpW);
        SendMessage(hWndFontButton, WM_SETFONT, (WPARAM)hPrintFont, TRUE);
        bFontChanged=TRUE;
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (hBitmapDownArrow) DeleteObject(hBitmapDownArrow);
      if (hPrintFont) DeleteObject(hPrintFont);

      bPrintFontEnable=SendMessage(hWndFontCheck, BM_GETCHECK, 0, 0);
      memcpy(&lfPrintFontW, &lfTmpW, sizeof(LOGFONTW));
      if (bFontChanged) bPrintFontChanged=TRUE;

      bPrintHeaderEnable=SendMessage(hWndHeaderCheck, BM_GETCHECK, 0, 0);
      GetWindowTextW(hWndHeaderEdit, wszPrintHeader, MAX_PATH);

      bPrintFooterEnable=SendMessage(hWndFooterCheck, BM_GETCHECK, 0, 0);
      GetWindowTextW(hWndFooterEdit, wszPrintFooter, MAX_PATH);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      if (hBitmapDownArrow) DeleteObject(hBitmapDownArrow);
      if (hPrintFont) DeleteObject(hPrintFont);
    }
  }
  return FALSE;
}

void GetPrintPage(HDC hDC, RECT *rcMargin, BOOL bIsMarginInMillimeters, RECT *rcPage)
{
  RECT rcPhys;
  RECT rcUser;
  RECT rcAdjUser;
  POINT ptDpi;
  POINT ptPage;
  int nExtent;

  nExtent=bIsMarginInMillimeters?2540:1000;

  ptDpi.x=GetDeviceCaps(hDC, LOGPIXELSX);
  ptDpi.y=GetDeviceCaps(hDC, LOGPIXELSY);

  ptPage.x=GetDeviceCaps(hDC, PHYSICALWIDTH);
  ptPage.y=GetDeviceCaps(hDC, PHYSICALHEIGHT);

  rcPhys.left=GetDeviceCaps(hDC, PHYSICALOFFSETX);
  rcPhys.top=GetDeviceCaps(hDC, PHYSICALOFFSETY);
  rcPhys.right=ptPage.x - GetDeviceCaps(hDC, HORZRES) - rcPhys.left;
  rcPhys.bottom=ptPage.y - GetDeviceCaps(hDC, VERTRES) - rcPhys.top;

  if (!rcMargin)
  {
    rcUser.left=0;
    rcUser.top=0;
    rcUser.right=0;
    rcUser.bottom=0;
  }
  else
  {
    rcUser.left=MulDiv(rcMargin->left, ptDpi.x, nExtent);
    rcUser.top=MulDiv(rcMargin->top, ptDpi.y, nExtent);
    rcUser.right=MulDiv(rcMargin->right, ptDpi.x, nExtent);
    rcUser.bottom=MulDiv(rcMargin->bottom, ptDpi.y, nExtent);
  }

  rcAdjUser.left=max(rcPhys.left, rcUser.left);
  rcAdjUser.top=max(rcPhys.top, rcUser.top);
  rcAdjUser.right=max(rcPhys.right, rcUser.right);
  rcAdjUser.bottom=max(rcPhys.bottom, rcUser.bottom);

  rcPage->left=rcAdjUser.left - rcPhys.left;
  rcPage->top=rcAdjUser.top - rcPhys.top;
  rcPage->right=ptPage.x - rcAdjUser.right - rcPhys.left;
  rcPage->bottom=ptPage.y - rcAdjUser.bottom - rcPhys.top;
}

void GetPrinterDCA(PRINTDLGA *pdA)
{
  PRINTDLGA pdTmpA;
  DEVMODEA *hDevMode;
  DEVNAMES *hDevNames;

  pdA->hDC=NULL;

  if (pdA->hDevMode && pdA->hDevNames)
  {
    hDevMode=(DEVMODEA *)GlobalLock(pdA->hDevMode);
    hDevNames=(DEVNAMES *)GlobalLock(pdA->hDevNames);
    pdA->hDC=CreateDCA((char *)hDevNames + hDevNames->wDriverOffset, (char *)hDevNames + hDevNames->wDeviceOffset, NULL, hDevMode);
    GlobalUnlock(pdA->hDevMode);
    GlobalUnlock(pdA->hDevNames);
  }
  if (!pdA->hDC)
  {
    memcpy(&pdTmpA, pdA, sizeof(PRINTDLGA));
    pdTmpA.Flags|=PD_RETURNDEFAULT|PD_RETURNDC;
    pdTmpA.hDevMode=NULL;
    pdTmpA.hDevNames=NULL;

    if (PrintDlgA(&pdTmpA))
    {
     pdA->hDC=pdTmpA.hDC;
     pdA->hDevMode=pdTmpA.hDevMode;
     pdA->hDevNames=pdTmpA.hDevNames;
    }
  }
}

void GetPrinterDCW(PRINTDLGW *pdW)
{
  PRINTDLGW pdTmpW;
  DEVMODEW *hDevMode;
  DEVNAMES *hDevNames;

  pdW->hDC=NULL;

  if (pdW->hDevMode && pdW->hDevNames)
  {
    hDevMode=(DEVMODEW *)GlobalLock(pdW->hDevMode);
    hDevNames=(DEVNAMES *)GlobalLock(pdW->hDevNames);
    pdW->hDC=CreateDCW((wchar_t *)hDevNames + hDevNames->wDriverOffset, (wchar_t *)hDevNames + hDevNames->wDeviceOffset, NULL, hDevMode);
    GlobalUnlock(pdW->hDevMode);
    GlobalUnlock(pdW->hDevNames);
  }
  if (!pdW->hDC)
  {
    memcpy(&pdTmpW, pdW, sizeof(PRINTDLGW));
    pdTmpW.Flags|=PD_RETURNDEFAULT|PD_RETURNDC;
    pdTmpW.hDevMode=NULL;
    pdTmpW.hDevNames=NULL;

    if (PrintDlgW(&pdTmpW))
    {
     pdW->hDC=pdTmpW.hDC;
     pdW->hDevMode=pdTmpW.hDevMode;
     pdW->hDevNames=pdTmpW.hDevNames;
    }
  }
}

BOOL PrintTextA(HDC hDC, RECT *rc, TEXTMETRICA *tmA, DRAWTEXTPARAMS *dtp, char *pText, char **pNextText)
{
  RECT rcDraw=*rc;
  RECT rcCalc;
  BOOL bEndDraw=FALSE;
  BOOL bMultiPart=FALSE;
  BOOL bResult=TRUE;
  DWORD i;

  if (*pText)
  {
    while (1)
    {
      for (i=0; pText[i] && pText[i] != '\f' && i < 8192; ++i);

      if (pText[0] == '\0') break;
      if (pText[i] == '\f')
      {
        if (i == 0) break;
        bEndDraw=TRUE;
      }

      rcCalc=rcDraw;
      if (!DrawTextExA(hDC, pText, i, &rcCalc, DT_CALCRECT|DT_NOPREFIX|DT_EXPANDTABS|DT_TABSTOP|DT_EDITCONTROL|DT_WORDBREAK, dtp))
      {
        bResult=FALSE;
        break;
      }
      if (rcCalc.bottom <= rc->bottom && i == dtp->uiLengthDrawn && i == 8192)
      {
        rcDraw.bottom=rcCalc.bottom - tmA->tmHeight;
        bMultiPart=TRUE;
      }
      if (!DrawTextExA(hDC, pText, i, &rcDraw, DT_NOPREFIX|DT_EXPANDTABS|DT_TABSTOP|DT_EDITCONTROL|DT_WORDBREAK, dtp))
      {
        bResult=FALSE;
        break;
      }

      pText+=dtp->uiLengthDrawn;
      if (!*pText) break;
      if (bEndDraw)
      {
        pText+=2;
        break;
      }
      if (i != dtp->uiLengthDrawn)
      {
        if (bMultiPart)
        {
          rcDraw.top=rcDraw.bottom;
          rcDraw.bottom=rc->bottom;
          bMultiPart=FALSE;
          continue;
        }
        break;
      }
    }
  }
  else bResult=FALSE;

  *pNextText=pText;
  return bResult;
}

BOOL PrintTextW(HDC hDC, RECT *rc, TEXTMETRICW *tmW, DRAWTEXTPARAMS *dtp, wchar_t *wpText, wchar_t **wpNextText)
{
  RECT rcDraw=*rc;
  RECT rcCalc;
  BOOL bEndDraw=FALSE;
  BOOL bMultiPart=FALSE;
  BOOL bResult=TRUE;
  DWORD i;

  if (*wpText)
  {
    while (1)
    {
      for (i=0; wpText[i] && wpText[i] != '\f' && i < 8192; ++i);

      if (wpText[0] == '\0') break;
      if (wpText[i] == '\f')
      {
        if (i == 0) break;
        bEndDraw=TRUE;
      }

      rcCalc=rcDraw;
      if (!DrawTextExW(hDC, wpText, i, &rcCalc, DT_CALCRECT|DT_NOPREFIX|DT_EXPANDTABS|DT_TABSTOP|DT_EDITCONTROL|DT_WORDBREAK, dtp))
      {
        bResult=FALSE;
        break;
      }
      if (rcCalc.bottom <= rc->bottom && i == dtp->uiLengthDrawn && i == 8192)
      {
        rcDraw.bottom=rcCalc.bottom - tmW->tmHeight;
        bMultiPart=TRUE;
      }
      if (!DrawTextExW(hDC, wpText, i, &rcDraw, DT_NOPREFIX|DT_EXPANDTABS|DT_TABSTOP|DT_EDITCONTROL|DT_WORDBREAK, dtp))
      {
        bResult=FALSE;
        break;
      }

      wpText+=dtp->uiLengthDrawn;
      if (!*wpText) break;
      if (bEndDraw)
      {
        wpText+=2;
        break;
      }
      if (i != dtp->uiLengthDrawn)
      {
        if (bMultiPart)
        {
          rcDraw.top=rcDraw.bottom;
          rcDraw.bottom=rc->bottom;
          bMultiPart=FALSE;
          continue;
        }
        break;
      }
    }
  }
  else bResult=FALSE;

  *wpNextText=wpText;
  return bResult;
}

BOOL PrintHeadlineA(HDC hDC, RECT *rc, char *pHeadline, int nPageNumber)
{
  //%% == %, %n[1] == nPageNumber[nPageStart], %f == szCurrentFile, %c == DT_CENTER, %l == DT_LEFT, %r == DT_RIGHT
  char szCenter[MAX_PATH];
  char szLeft[MAX_PATH];
  char szRight[MAX_PATH];
  char *pCount=&szCenter[0];
  char szPageNumber[32];
  char *pFile;
  int nCenter=0;
  int nLeft=0;
  int nRight=0;
  int nCount=nCenter;
  int nAlign=DT_CENTER;
  int nPageNumberLen;
  int nPageStart;
  int nFileLen;
  int a;
  BOOL bResult=TRUE;

  pFile=GetFileNameA(szCurrentFile);
  nFileLen=lstrlenA(pFile);

  for (a=0; pHeadline[a] && nCount < MAX_PATH; ++a)
  {
    if (pHeadline[a] == '%')
    {
      if (pHeadline[++a] == '%')
      {
        pCount[nCount++]='%';
      }
      else if (pHeadline[a] == 'c' || pHeadline[a] == 'C' ||
               pHeadline[a] == 'l' || pHeadline[a] == 'L' ||
               pHeadline[a] == 'r' || pHeadline[a] == 'R')
      {
        if (nAlign == DT_CENTER)
          nCenter=nCount;
        else if (nAlign == DT_LEFT)
          nLeft=nCount;
        else if (nAlign == DT_RIGHT)
          nRight=nCount;

        if (pHeadline[a] == 'c' || pHeadline[a] == 'C')
        {
          nAlign=DT_CENTER;
          pCount=&szCenter[0];
          nCount=nCenter;
        }
        else if (pHeadline[a] == 'l' || pHeadline[a] == 'L')
        {
          nAlign=DT_LEFT;
          pCount=&szLeft[0];
          nCount=nLeft;
        }
        else if (pHeadline[a] == 'r' || pHeadline[a] == 'R')
        {
          nAlign=DT_RIGHT;
          pCount=&szRight[0];
          nCount=nRight;
        }
      }
      else if (pHeadline[a] == 'n' || pHeadline[a] == 'N')
      {
        if (pHeadline[++a] == '[')
        {
          for (++a, nPageNumberLen=0; pHeadline[a] && pHeadline[a] != ']' && nPageNumberLen < 30; ++a, ++nPageNumberLen)
            szPageNumber[nPageNumberLen]=pHeadline[a];
          szPageNumber[nPageNumberLen]='\0';
          nPageStart=xatoiA(szPageNumber);
          nPageNumberLen=wsprintfA(szPageNumber, "%d", nPageNumber + nPageStart - 1);

          if (nCount + nPageNumberLen <= MAX_PATH)
          {
            lstrcpyA(pCount + nCount, szPageNumber);
            nCount+=nPageNumberLen;
          }
          else break;
        }
        else break;
      }
      else if (pHeadline[a] == 'f' || pHeadline[a] == 'F')
      {
        if (nCount + nFileLen <= MAX_PATH)
        {
          lstrcpyA(pCount + nCount, pFile);
          nCount+=nFileLen;
        }
        else break;
      }
      else break;
    }
    else pCount[nCount++]=pHeadline[a];
  }
  if (nAlign == DT_CENTER)
    nCenter=nCount;
  else if (nAlign == DT_LEFT)
    nLeft=nCount;
  else if (nAlign == DT_RIGHT)
    nRight=nCount;

  if (nCenter < MAX_PATH)
    szCenter[nCenter]='\0';
  if (nLeft < MAX_PATH)
    szLeft[nLeft]='\0';
  if (nRight < MAX_PATH)
    szRight[nRight]='\0';

  if (*szCenter)
    bResult=DrawTextA(hDC, szCenter, nCenter, rc, DT_CENTER);
  if (*szLeft)
    bResult=DrawTextA(hDC, szLeft, nLeft, rc, DT_LEFT);
  if (*szRight)
    bResult=DrawTextA(hDC, szRight, nRight, rc, DT_RIGHT);

  return bResult;
}

BOOL PrintHeadlineW(HDC hDC, RECT *rc, wchar_t *wpHeadline, int nPageNumber)
{
  //%% == %, %n[1] == nPageNumber[nPageStart], %f == wszCurrentFile, %c == DT_CENTER, %l == DT_LEFT, %r == DT_RIGHT
  wchar_t wszCenter[MAX_PATH];
  wchar_t wszLeft[MAX_PATH];
  wchar_t wszRight[MAX_PATH];
  wchar_t *wpCount=&wszCenter[0];
  wchar_t wszPageNumber[32];
  wchar_t *wpFile;
  int nCenter=0;
  int nLeft=0;
  int nRight=0;
  int nCount=nCenter;
  int nAlign=DT_CENTER;
  int nPageNumberLen;
  int nPageStart;
  int nFileLen;
  int a;
  BOOL bResult=TRUE;

  wpFile=GetFileNameW(wszCurrentFile);
  nFileLen=lstrlenW(wpFile);

  for (a=0; wpHeadline[a] && nCount < MAX_PATH; ++a)
  {
    if (wpHeadline[a] == '%')
    {
      if (wpHeadline[++a] == '%')
      {
        wpCount[nCount++]='%';
      }
      else if (wpHeadline[a] == 'c' || wpHeadline[a] == 'C' ||
               wpHeadline[a] == 'l' || wpHeadline[a] == 'L' ||
               wpHeadline[a] == 'r' || wpHeadline[a] == 'R')
      {
        if (nAlign == DT_CENTER)
          nCenter=nCount;
        else if (nAlign == DT_LEFT)
          nLeft=nCount;
        else if (nAlign == DT_RIGHT)
          nRight=nCount;

        if (wpHeadline[a] == 'c' || wpHeadline[a] == 'C')
        {
          nAlign=DT_CENTER;
          wpCount=&wszCenter[0];
          nCount=nCenter;
        }
        else if (wpHeadline[a] == 'l' || wpHeadline[a] == 'L')
        {
          nAlign=DT_LEFT;
          wpCount=&wszLeft[0];
          nCount=nLeft;
        }
        else if (wpHeadline[a] == 'r' || wpHeadline[a] == 'R')
        {
          nAlign=DT_RIGHT;
          wpCount=&wszRight[0];
          nCount=nRight;
        }
      }
      else if (wpHeadline[a] == 'n' || wpHeadline[a] == 'N')
      {
        if (wpHeadline[++a] == '[')
        {
          for (++a, nPageNumberLen=0; wpHeadline[a] && wpHeadline[a] != ']' && nPageNumberLen < 30; ++a, ++nPageNumberLen)
            wszPageNumber[nPageNumberLen]=wpHeadline[a];
          wszPageNumber[nPageNumberLen]='\0';
          nPageStart=xatoiW(wszPageNumber);
          nPageNumberLen=wsprintfW(wszPageNumber, L"%d", nPageNumber + nPageStart - 1);

          if (nCount + nPageNumberLen <= MAX_PATH)
          {
            lstrcpyW(wpCount + nCount, wszPageNumber);
            nCount+=nPageNumberLen;
          }
          else break;
        }
        else break;
      }
      else if (wpHeadline[a] == 'f' || wpHeadline[a] == 'F')
      {
        if (nCount + nFileLen <= MAX_PATH)
        {
          lstrcpyW(wpCount + nCount, wpFile);
          nCount+=nFileLen;
        }
        else break;
      }
      else break;
    }
    else wpCount[nCount++]=wpHeadline[a];
  }
  if (nAlign == DT_CENTER)
    nCenter=nCount;
  else if (nAlign == DT_LEFT)
    nLeft=nCount;
  else if (nAlign == DT_RIGHT)
    nRight=nCount;

  if (nCenter < MAX_PATH)
    wszCenter[nCenter]='\0';
  if (nLeft < MAX_PATH)
    wszLeft[nLeft]='\0';
  if (nRight < MAX_PATH)
    wszRight[nRight]='\0';

  if (*wszCenter)
    bResult=DrawTextW(hDC, wszCenter, nCenter, rc, DT_CENTER);
  if (*wszLeft)
    bResult=DrawTextW(hDC, wszLeft, nLeft, rc, DT_LEFT);
  if (*wszRight)
    bResult=DrawTextW(hDC, wszRight, nRight, rc, DT_RIGHT);

  return bResult;
}


//// Code pages

UINT_PTR CALLBACK CodePageDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCP;
  static HWND hWndPreview;
  static HWND hWndAutodetect;
  static HWND hDlgEdit;
  static RECT rcDlg;
  static RECT rcCodePage;
  static RECT rcAutodetect;
  static RECT rcPreview;
  static char szFile[MAX_PATH];
  static int nCodePage;
  static BOOL bBOM;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    if (bMDI) hDlgEdit=GetDlgItem(GetParent(hDlg), IDC_OFN_EDIT);
    hWndCP=GetDlgItem(hDlg, IDC_OFN_CODEPAGE);
    hWndPreview=GetDlgItem(hDlg, IDC_OFN_PREVIEW);
    hWndAutodetect=GetDlgItem(hDlg, IDC_OFN_AUTODETECT);

    nCodePage=nCurrentCodePage;
    FillComboboxCodepageA(hWndCP, lpCodepageList);
    SelectComboboxCodepageA(hWndCP, nCodePage);

    GetWindowPos(hDlg, NULL, &rcDlg);
    GetWindowPos(hWndCP, hDlg, &rcCodePage);
    GetWindowPos(hWndAutodetect, hDlg, &rcAutodetect);
    GetWindowPos(hWndPreview, hDlg, &rcPreview);

    if (bSaveDlg)
    {
      SetWindowPos(hDlg, 0, 0, 0, rcDlg.right, rcPreview.top, SWP_NOMOVE|SWP_NOZORDER);
      SetWindowTextA(hWndAutodetect, "&BOM");

      if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
      {
        SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)bCurrentBOM, 0);
        EnableWindow(hWndAutodetect, TRUE);
      }
      else
      {
        SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)FALSE, 0);
        EnableWindow(hWndAutodetect, FALSE);
      }
    }
    else
    {
      API_LoadStringA(hLangLib, STR_AUTODETECT, buf, BUFFER_SIZE);
      SetWindowTextA(hWndAutodetect, buf);
      SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)bAutodetect, 0);
      EnableWindow(hWndCP, !bAutodetect);
    }
    SendMessage(hWndPreview, EM_SETTEXTMODE, TM_PLAINTEXT|TM_SINGLELEVELUNDO|TM_MULTICODEPAGE, 0);
    SendMessage(hWndPreview, EM_SETBKGNDCOLOR, 0, aecColors.crBasicBk);
    SetChosenFontA(hWndPreview, &lfEditFontA);
    SetChosenFontColorA(hWndPreview, aecColors.crBasicText);

    OldPreviewProc=(WNDPROC)GetWindowLongA(hWndPreview, GWL_WNDPROC);
    SetWindowLongA(hWndPreview, GWL_WNDPROC, (LONG)NewPreviewProcA);
  }
  else if (uMsg == WM_SIZE)
  {
    RECT rcTemplate;
    int x;
    int y;

    GetWindowPos(hWndAutodetect, hDlg, &rcAutodetect);
    GetWindowPos(hDlg, NULL, &rcTemplate);
    x=rcTemplate.right - rcDlg.right;
    y=rcTemplate.bottom - rcDlg.bottom;
    rcDlg=rcTemplate;

    rcCodePage.right+=x;
    rcAutodetect.left+=x;
    rcPreview.right+=x;

    SetWindowPos(hWndCP, 0, 0, 0, rcCodePage.right, rcCodePage.bottom, SWP_NOMOVE|SWP_NOZORDER);
    SetWindowPos(hWndAutodetect, 0, rcAutodetect.left, rcAutodetect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
    SetWindowPos(hWndPreview, 0, 0, 0, rcPreview.right, rcPreview.bottom, SWP_NOMOVE|SWP_NOZORDER);
    return 0;
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndPreview)
    {
      RECT rcRect;
      POINT pt;
      CHARRANGE cr;

      if (lParam == -1)
      {
        GetWindowRect((HWND)wParam, &rcRect);
        pt.x=rcRect.left;
        pt.y=rcRect.top;
      }
      else
      {
        GetCursorPos(&pt);
      }
      SendMessage((HWND)wParam, EM_EXGETSEL, 0, (LPARAM)&cr);
      EnableMenuItem(hPopupView, IDM_EDIT_COPY, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);
      i=TrackPopupMenu(hPopupView, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);

      if (i == IDM_EDIT_COPY)
        SendMessage((HWND)wParam, WM_COPY, 0, 0);
      else if (i == IDM_EDIT_SELECTALL)
        SetTextSel((HWND)wParam, 0, -1);
    }
    return TRUE;
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((LPNMHDR)lParam)->code == CDN_FILEOK)
    {
      if (!bSaveDlg)
      {
        dwOfnFlags=bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):0;
        nOfnCodePage=nCodePage;
        bOfnBOM=bBOM;
      }
      else
      {
        dwOfnFlags=0;
        nOfnCodePage=nCodePage;
        bOfnBOM=(BOOL)SendMessage(hWndAutodetect, BM_GETCHECK, 0, 0);
      }
      if (!CheckCodePage(nOfnCodePage))
      {
        API_LoadStringA(hLangLib, MSG_CP_UNIMPLEMENTED, buf, BUFFER_SIZE);
        wsprintfA(buf2, buf, nOfnCodePage);
        MessageBoxA(hDlg, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
        SetWindowLongA(hDlg, DWL_MSGRESULT, 1);
        return TRUE;
      }
    }
    else if (((LPNMHDR)lParam)->code == CDN_SELCHANGE)
    {
      if (!bSaveDlg)
      {
        if (bMDI)
        {
          if (GetWindowTextLength(hDlgEdit) > FILELIST_BUFFER_SIZE)
          {
            API_LoadStringA(hLangLib, MSG_LONG_FILELIST, buf, BUFFER_SIZE);
            MessageBoxA(hDlg, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
          }
        }

        SendMessageA(((OFNOTIFY*)lParam)->hdr.hwndFrom, CDM_GETFILEPATH, (LPARAM)MAX_PATH, (WPARAM)szFile);

        if (FilePreviewA(hWndPreview, szFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndPreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageA(hWndCP, nDefaultCodePage);
          SetWindowTextA(hWndPreview, "");
          return TRUE;
        }
        EnableWindow(hWndPreview, TRUE);
        if (bAutodetect) SelectComboboxCodepageA(hWndCP, nCodePage);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OFN_CODEPAGE && HIWORD(wParam) == CBN_SELCHANGE)
    {
      nCodePage=GetComboboxCodepageA(hWndCP);

      if (bSaveDlg)
      {
        if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
        {
          EnableWindow(hWndAutodetect, TRUE);
          SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)TRUE, 0);
        }
        else
        {
          EnableWindow(hWndAutodetect, FALSE);
          SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)FALSE, 0);
        }
      }
      else
      {
        if (FilePreviewA(hWndPreview, szFile, PREVIEW_SIZE, OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndPreview, FALSE);
          SetWindowTextA(hWndPreview, "");
        }
        else EnableWindow(hWndPreview, TRUE);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OFN_AUTODETECT)
    {
      if (!bSaveDlg)
      {
        bAutodetect=(BOOL)SendMessage(hWndAutodetect, BM_GETCHECK, 0, 0);
        EnableWindow(hWndCP, !bAutodetect);
        nCodePage=GetComboboxCodepageA(hWndCP);

        if (FilePreviewA(hWndPreview, szFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndPreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageA(hWndCP, nDefaultCodePage);
          SetWindowTextA(hWndPreview, "");
          return TRUE;
        }
        EnableWindow(hWndPreview, TRUE);
        if (bAutodetect) SelectComboboxCodepageA(hWndCP, nCodePage);
      }
      return TRUE;
    }
  }
  return FALSE;
}

UINT_PTR CALLBACK CodePageDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCP;
  static HWND hWndPreview;
  static HWND hWndAutodetect;
  static HWND hDlgEdit;
  static RECT rcDlg;
  static RECT rcCodePage;
  static RECT rcAutodetect;
  static RECT rcPreview;
  static wchar_t wszFile[MAX_PATH];
  static int nCodePage;
  static BOOL bBOM;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    if (bMDI) hDlgEdit=GetDlgItem(GetParent(hDlg), IDC_OFN_EDIT);
    hWndCP=GetDlgItem(hDlg, IDC_OFN_CODEPAGE);
    hWndPreview=GetDlgItem(hDlg, IDC_OFN_PREVIEW);
    hWndAutodetect=GetDlgItem(hDlg, IDC_OFN_AUTODETECT);

    nCodePage=nCurrentCodePage;
    FillComboboxCodepageW(hWndCP, lpCodepageList);
    SelectComboboxCodepageW(hWndCP, nCodePage);

    GetWindowPos(hDlg, NULL, &rcDlg);
    GetWindowPos(hWndCP, hDlg, &rcCodePage);
    GetWindowPos(hWndAutodetect, hDlg, &rcAutodetect);
    GetWindowPos(hWndPreview, hDlg, &rcPreview);

    if (bSaveDlg)
    {
      SetWindowPos(hDlg, 0, 0, 0, rcDlg.right, rcPreview.top, SWP_NOMOVE|SWP_NOZORDER);
      SetWindowTextW(hWndAutodetect, L"&BOM");

      if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
      {
        SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)bCurrentBOM, 0);
        EnableWindow(hWndAutodetect, TRUE);
      }
      else
      {
        SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)FALSE, 0);
        EnableWindow(hWndAutodetect, FALSE);
      }
    }
    else
    {
      API_LoadStringW(hLangLib, STR_AUTODETECT, wbuf, BUFFER_SIZE);
      SetWindowTextW(hWndAutodetect, wbuf);
      SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)bAutodetect, 0);
      EnableWindow(hWndCP, !bAutodetect);
    }
    SendMessage(hWndPreview, EM_SETTEXTMODE, TM_PLAINTEXT|TM_SINGLELEVELUNDO|TM_MULTICODEPAGE, 0);
    SendMessage(hWndPreview, EM_SETBKGNDCOLOR, 0, aecColors.crBasicBk);
    SetChosenFontW(hWndPreview, &lfEditFontW);
    SetChosenFontColorW(hWndPreview, aecColors.crBasicText);

    OldPreviewProc=(WNDPROC)GetWindowLongW(hWndPreview, GWL_WNDPROC);
    SetWindowLongW(hWndPreview, GWL_WNDPROC, (LONG)NewPreviewProcW);
  }
  else if (uMsg == WM_SIZE)
  {
    RECT rcTemplate;
    int x;
    int y;

    GetWindowPos(hWndAutodetect, hDlg, &rcAutodetect);
    GetWindowPos(hDlg, NULL, &rcTemplate);
    x=rcTemplate.right - rcDlg.right;
    y=rcTemplate.bottom - rcDlg.bottom;
    rcDlg=rcTemplate;

    rcCodePage.right+=x;
    rcAutodetect.left+=x;
    rcPreview.right+=x;

    SetWindowPos(hWndCP, 0, 0, 0, rcCodePage.right, rcCodePage.bottom, SWP_NOMOVE|SWP_NOZORDER);
    SetWindowPos(hWndAutodetect, 0, rcAutodetect.left, rcAutodetect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
    SetWindowPos(hWndPreview, 0, 0, 0, rcPreview.right, rcPreview.bottom, SWP_NOMOVE|SWP_NOZORDER);
    return 0;
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndPreview)
    {
      RECT rcRect;
      POINT pt;
      CHARRANGE cr;

      if (lParam == -1)
      {
        GetWindowRect((HWND)wParam, &rcRect);
        pt.x=rcRect.left;
        pt.y=rcRect.top;
      }
      else
      {
        GetCursorPos(&pt);
      }
      SendMessage((HWND)wParam, EM_EXGETSEL, 0, (LPARAM)&cr);
      EnableMenuItem(hPopupView, IDM_EDIT_COPY, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);
      i=TrackPopupMenu(hPopupView, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);

      if (i == IDM_EDIT_COPY)
        SendMessage((HWND)wParam, WM_COPY, 0, 0);
      else if (i == IDM_EDIT_SELECTALL)
        SetTextSel((HWND)wParam, 0, -1);
    }
    return TRUE;
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((LPNMHDR)lParam)->code == CDN_FILEOK)
    {
      if (!bSaveDlg)
      {
        dwOfnFlags=bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):0;
        nOfnCodePage=nCodePage;
        bOfnBOM=bBOM;
      }
      else
      {
        dwOfnFlags=0;
        nOfnCodePage=nCodePage;
        bOfnBOM=(BOOL)SendMessage(hWndAutodetect, BM_GETCHECK, 0, 0);
      }
      if (!CheckCodePage(nOfnCodePage))
      {
        API_LoadStringW(hLangLib, MSG_CP_UNIMPLEMENTED, wbuf, BUFFER_SIZE);
        wsprintfW(wbuf2, wbuf, nOfnCodePage);
        MessageBoxW(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
        SetWindowLongW(hDlg, DWL_MSGRESULT, 1);
        return TRUE;
      }
    }
    else if (((LPNMHDR)lParam)->code == CDN_SELCHANGE)
    {
      if (!bSaveDlg)
      {
        if (bMDI)
        {
          if (GetWindowTextLengthW(hDlgEdit) > FILELIST_BUFFER_SIZE)
          {
            API_LoadStringW(hLangLib, MSG_LONG_FILELIST, wbuf, BUFFER_SIZE);
            MessageBoxW(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
          }
        }

        SendMessageW(((OFNOTIFY*)lParam)->hdr.hwndFrom, CDM_GETFILEPATH, (LPARAM)MAX_PATH, (WPARAM)wszFile);

        if (FilePreviewW(hWndPreview, wszFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndPreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageW(hWndCP, nDefaultCodePage);
          SetWindowTextW(hWndPreview, L"");
          return TRUE;
        }
        EnableWindow(hWndPreview, TRUE);
        if (bAutodetect) SelectComboboxCodepageW(hWndCP, nCodePage);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OFN_CODEPAGE && HIWORD(wParam) == CBN_SELCHANGE)
    {
      nCodePage=GetComboboxCodepageW(hWndCP);

      if (bSaveDlg)
      {
        if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
        {
          EnableWindow(hWndAutodetect, TRUE);
          SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)TRUE, 0);
        }
        else
        {
          EnableWindow(hWndAutodetect, FALSE);
          SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)FALSE, 0);
        }
      }
      else
      {
        if (FilePreviewW(hWndPreview, wszFile, PREVIEW_SIZE, OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndPreview, FALSE);
          SetWindowTextW(hWndPreview, L"");
        }
        else EnableWindow(hWndPreview, TRUE);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OFN_AUTODETECT)
    {
      if (!bSaveDlg)
      {
        bAutodetect=(BOOL)SendMessage(hWndAutodetect, BM_GETCHECK, 0, 0);
        EnableWindow(hWndCP, !bAutodetect);
        nCodePage=GetComboboxCodepageW(hWndCP);

        if (FilePreviewW(hWndPreview, wszFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndPreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageW(hWndCP, nDefaultCodePage);
          SetWindowTextW(hWndPreview, L"");
          return TRUE;
        }
        EnableWindow(hWndPreview, TRUE);
        if (bAutodetect) SelectComboboxCodepageW(hWndCP, nCodePage);
      }
      return TRUE;
    }
  }
  return FALSE;
}

LRESULT CALLBACK NewPreviewProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_ESCAPE)
    {
      PostMessage(GetParent(hWnd), WM_KEYDOWN, VK_ESCAPE, 0);
      return TRUE;
    }
  }
  return CallWindowProcA(OldPreviewProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewPreviewProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_ESCAPE)
    {
      PostMessage(GetParent(hWnd), WM_KEYDOWN, VK_ESCAPE, 0);
      return TRUE;
    }
  }
  return CallWindowProcW(OldPreviewProc, hWnd, uMsg, wParam, lParam);
}

void FillComboboxCodepageA(HWND hWnd, int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      if (bRecodeDlg && (lpCodepageList[i] == CP_UNICODE_UCS2_LE ||
                         lpCodepageList[i] == CP_UNICODE_UCS2_BE))
      {
        continue;
      }
      GetCodePageNameA(lpCodepageList[i], buf, BUFFER_SIZE);
      SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)buf);
    }
  }
}

void FillComboboxCodepageW(HWND hWnd, int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      if (bRecodeDlg && (lpCodepageList[i] == CP_UNICODE_UCS2_LE ||
                         lpCodepageList[i] == CP_UNICODE_UCS2_BE))
      {
        continue;
      }
      GetCodePageNameW(lpCodepageList[i], wbuf, BUFFER_SIZE);
      SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)wbuf);
    }
  }
}

void FillListboxCodepageA(HWND hWnd, int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      GetCodePageNameA(lpCodepageList[i], buf, BUFFER_SIZE);
      SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)buf);
    }
  }
}

void FillListboxCodepageW(HWND hWnd, int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      GetCodePageNameW(lpCodepageList[i], wbuf, BUFFER_SIZE);
      SendMessageW(hWnd, LB_ADDSTRING, 0, (LPARAM)wbuf);
    }
  }
}

void ClearCombobox(HWND hWnd)
{
  int nCount;

  if ((nCount=SendMessage(hWnd, CB_GETCOUNT, 0, 0)) > 0)
  {
    while (--nCount >= 0) SendMessage(hWnd, CB_DELETESTRING, nCount, 0);
  }
}

void ClearListbox(HWND hWnd)
{
  int nCount;

  if ((nCount=SendMessage(hWnd, LB_GETCOUNT, 0, 0)) > 0)
  {
    while (--nCount >= 0) SendMessage(hWnd, LB_DELETESTRING, nCount, 0);
  }
}

int GetComboboxCodepageA(HWND hWnd)
{
  int nCodePage=0;
  int nSelection;

  if ((nSelection=SendMessage(hWnd, CB_GETCURSEL, 0, 0)) != CB_ERR)
  {
    SendMessageA(hWnd, CB_GETLBTEXT, (WPARAM)nSelection, (LPARAM)buf);
    nCodePage=xatoiA(buf);
  }
  return nCodePage;
}

int GetComboboxCodepageW(HWND hWnd)
{
  int nCodePage=0;
  int nSelection;

  if ((nSelection=SendMessage(hWnd, CB_GETCURSEL, 0, 0)) != CB_ERR)
  {
    SendMessageW(hWnd, CB_GETLBTEXT, (WPARAM)nSelection, (LPARAM)wbuf);
    nCodePage=xatoiW(wbuf);
  }
  return nCodePage;
}

int GetListboxCodepageA(HWND hWnd)
{
  int nCodePage=0;
  int nSelection;

  if ((nSelection=(int)SendMessage(hWnd, LB_GETCURSEL, 0, 0)) != LB_ERR)
  {
    SendMessageA(hWnd, LB_GETTEXT, (WPARAM)nSelection, (LPARAM)buf);
    nCodePage=xatoiA(buf);
  }
  return nCodePage;
}

int GetListboxCodepageW(HWND hWnd)
{
  int nCodePage=0;
  int nSelection;

  if ((nSelection=(int)SendMessage(hWnd, LB_GETCURSEL, 0, 0)) != LB_ERR)
  {
    SendMessageW(hWnd, LB_GETTEXT, (WPARAM)nSelection, (LPARAM)wbuf);
    nCodePage=xatoiW(wbuf);
  }
  return nCodePage;
}

int SelectComboboxCodepageA(HWND hWnd, int nCodepage)
{
  int nSelection;

  wsprintfA(buf, "%d ", nCodepage);
  nSelection=SendMessageA(hWnd, CB_FINDSTRING, (WPARAM)-1, (LPARAM)buf);
  SendMessage(hWnd, CB_SETCURSEL, (WPARAM)nSelection, 0);

  return nSelection;
}

int SelectComboboxCodepageW(HWND hWnd, int nCodepage)
{
  int nSelection;

  wsprintfW(wbuf, L"%d ", nCodepage);
  nSelection=SendMessageW(hWnd, CB_FINDSTRING, (WPARAM)-1, (LPARAM)wbuf);
  SendMessage(hWnd, CB_SETCURSEL, (WPARAM)nSelection, 0);

  return nSelection;
}

int SelectListboxCodepageA(HWND hWnd, int nCodepage)
{
  int nSelection;

  wsprintfA(buf, "%d ", nCodepage);

  if ((nSelection=(int)SendMessageA(hWnd, LB_FINDSTRING, (WPARAM)-1, (LPARAM)buf)) != LB_ERR)
  {
    SendMessage(hWnd, LB_SETCURSEL, (WPARAM)nSelection, 0);
  }
  return nSelection;
}

int SelectListboxCodepageW(HWND hWnd, int nCodepage)
{
  int nSelection;

  wsprintfW(wbuf, L"%d ", nCodepage);

  if ((nSelection=(int)SendMessageW(hWnd, LB_FINDSTRING, (WPARAM)-1, (LPARAM)wbuf)) != LB_ERR)
  {
    SendMessage(hWnd, LB_SETCURSEL, (WPARAM)nSelection, 0);
  }
  return nSelection;
}

void GetListboxCodepageListA(HWND hWnd, int **lpCodepageList)
{
  int *lpCodepageListCount;
  int nCount;
  int i;

  if ((nCount=SendMessage(hWnd, LB_GETCOUNT, 0, 0)) > 0)
  {
    if (*lpCodepageList=lpCodepageListCount=(int *)API_HeapAlloc(hHeap, 0, sizeof(int) * (nCount + 1)))
    {
      for (i=0; i < nCount; ++i)
      {
        SendMessageA(hWnd, LB_GETTEXT, (WPARAM)i, (LPARAM)buf);
        *lpCodepageListCount++=xatoiA(buf);
      }
      *lpCodepageListCount=0;
    }
  }
}

void GetListboxCodepageListW(HWND hWnd, int **lpCodepageList)
{
  int *lpCodepageListCount;
  int nCount;
  int i;

  if ((nCount=SendMessage(hWnd, LB_GETCOUNT, 0, 0)) > 0)
  {
    if (*lpCodepageList=lpCodepageListCount=(int *)API_HeapAlloc(hHeap, 0, sizeof(int) * (nCount + 1)))
    {
      for (i=0; i < nCount; ++i)
      {
        SendMessageW(hWnd, LB_GETTEXT, (WPARAM)i, (LPARAM)wbuf);
        *lpCodepageListCount++=xatoiW(wbuf);
      }
      *lpCodepageListCount=0;
    }
  }
}

DWORD EnumCodepageList(int **lpCodepageList)
{
  int *lpCodepageTable;
  int *lpCodepageListCount;
  HKEY hKey;
  DWORD dwSizeValue;
  DWORD dwSizeString;
  DWORD dwType;
  DWORD dwIndex=0;
  DWORD dwCount=0;
  int i;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Nls\\CodePage", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    if (lpCodepageTable=(int *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(int) * 65536))
    {
      while(1)
      {
        dwSizeValue=BUFFER_SIZE;
        dwSizeString=BUFFER_SIZE;
        buf2[0]='\0';

        if (RegEnumValueA(hKey, dwIndex++, buf, &dwSizeValue, NULL, &dwType, (LPBYTE)buf2, &dwSizeString) != ERROR_SUCCESS)
          break;

        if (*buf2 != '\0' && (i=xatoiA(buf)) != 0 && i != nAnsiCodePage && i != nOemCodePage && i < 65536)
        {
          lpCodepageTable[i]=TRUE;
          ++dwCount;
        }
      }
      RegCloseKey(hKey);

      //CP_ACP, CP_OEMCP, CP_UNICODE_UCS2_LE, CP_UNICODE_UCS2_BE, CP_UNICODE_UTF8, CP_UNICODE_UTF7, 0
      dwCount+=7;

      if (*lpCodepageList=lpCodepageListCount=(int *)API_HeapAlloc(hHeap, 0, sizeof(int) * dwCount))
      {
        *lpCodepageListCount++=nAnsiCodePage;
        if (nAnsiCodePage != nOemCodePage)
          *lpCodepageListCount++=nOemCodePage;
        *lpCodepageListCount++=CP_UNICODE_UCS2_LE;
        *lpCodepageListCount++=CP_UNICODE_UCS2_BE;
        *lpCodepageListCount++=CP_UNICODE_UTF8;
        *lpCodepageListCount++=CP_UNICODE_UTF7;

        for (i=0; i <= 65535; ++i)
        {
          if (lpCodepageTable[i])
            *lpCodepageListCount++=i;
        }
        *lpCodepageListCount=0;
      }
      API_HeapFree(hHeap, 0, (LPVOID)lpCodepageTable);
    }
  }
  return dwCount;
}

int CodepageListLen(int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i);
    return ++i;
  }
  return 0;
}

int CodepageListFind(int *lpCodepageList, int nCodePage)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      if (lpCodepageList[i] == nCodePage)
        return i;
    }
  }
  return -1;
}

void CodepageListFree(int **lpCodepageList)
{
  if (lpCodepageList && *lpCodepageList)
  {
    API_HeapFree(hHeap, 0, (LPVOID)*lpCodepageList);
    *lpCodepageList=NULL;
  }
}

void GetCodePageNameA(int nCodePage, char *szCodePage, int nLen)
{
  CPINFOEXA CPInfoExA;
  char szNumber[32];

  if (nCodePage)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE)
      lstrcpynA(szCodePage, STR_UNICODE_UCS2_LEA, nLen);
    else if (nCodePage == CP_UNICODE_UCS2_BE)
      lstrcpynA(szCodePage, STR_UNICODE_UCS2_BEA, nLen);
    else if (nCodePage == CP_UNICODE_UTF8)
      lstrcpynA(szCodePage, STR_UNICODE_UTF8A, nLen);
    else if (nCodePage == CP_UNICODE_UTF7)
      lstrcpynA(szCodePage, STR_UNICODE_UTF7A, nLen);
    else
    {
      if (GetCPInfoExAPtr && (*GetCPInfoExAPtr)(nCodePage, 0, &CPInfoExA) && nCodePage == xatoiA(CPInfoExA.CodePageName))
      {
        lstrcpynA(szCodePage, CPInfoExA.CodePageName, nLen);
      }
      else
      {
        wsprintfA(szNumber, "%d ", nCodePage);
        lstrcpynA(szCodePage, szNumber, nLen);
      }
    }
  }
  else szCodePage[0]='\0';
}

void GetCodePageNameW(int nCodePage, wchar_t *wszCodePage, int nLen)
{
  CPINFOEXW CPInfoExW;
  wchar_t wszNumber[32];

  if (nCodePage)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE)
      lstrcpynW(wszCodePage, STR_UNICODE_UCS2_LEW, nLen);
    else if (nCodePage == CP_UNICODE_UCS2_BE)
      lstrcpynW(wszCodePage, STR_UNICODE_UCS2_BEW, nLen);
    else if (nCodePage == CP_UNICODE_UTF8)
      lstrcpynW(wszCodePage, STR_UNICODE_UTF8W, nLen);
    else if (nCodePage == CP_UNICODE_UTF7)
      lstrcpynW(wszCodePage, STR_UNICODE_UTF7W, nLen);
    else
    {
      if (GetCPInfoExWPtr && (*GetCPInfoExWPtr)(nCodePage, 0, &CPInfoExW) && nCodePage == xatoiW(CPInfoExW.CodePageName))
      {
        lstrcpynW(wszCodePage, CPInfoExW.CodePageName, nLen);
      }
      else
      {
        wsprintfW(wszNumber, L"%d ", nCodePage);
        lstrcpynW(wszCodePage, wszNumber, nLen);
      }
    }
  }
  else wszCodePage[0]='\0';
}

int FilePreviewA(HWND hWnd, char *pFile, int nPreviewBytes, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
{
  HANDLE hFile;
  FILESTREAMDATA fsd;
  int i;

  if (!(dwFlags & ADT_REG_CODEPAGE) && !(dwFlags & ADT_DETECT_CODEPAGE))
    if (!*nCodePage) return EOD_OPEN;

  if (IsFileA(pFile) != ERROR_SUCCESS)
    return EOD_OPEN;

  if ((i=AutodetectCodePageA(pFile, dwCodepageRecognitionBuffer, dwFlags, nCodePage, bBOM)) < 0)
    return i;

  if ((hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
    return EOD_OPEN;

  if (*bBOM)
  {
    if (*nCodePage == CP_UNICODE_UCS2_LE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (*nCodePage == CP_UNICODE_UCS2_BE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (*nCodePage == CP_UNICODE_UTF8)
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
  }

  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=*nCodePage;
  fsd.nBytesMax=nPreviewBytes;
  FileStreamIn(&fsd);
  CloseHandle(hFile);

  return 0;
}

int FilePreviewW(HWND hWnd, wchar_t *wpFile, int nPreviewBytes, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
{
  HANDLE hFile;
  FILESTREAMDATA fsd;
  int i;

  if (!(dwFlags & ADT_REG_CODEPAGE) && !(dwFlags & ADT_DETECT_CODEPAGE))
    if (!*nCodePage) return EOD_OPEN;

  if (IsFileW(wpFile) != ERROR_SUCCESS)
    return EOD_OPEN;

  if ((i=AutodetectCodePageW(wpFile, dwCodepageRecognitionBuffer, dwFlags, nCodePage, bBOM)) < 0)
    return i;

  if ((hFile=API_CreateFileW(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
    return EOD_OPEN;

  if (*bBOM)
  {
    if (*nCodePage == CP_UNICODE_UCS2_LE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (*nCodePage == CP_UNICODE_UCS2_BE)
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
    else if (*nCodePage == CP_UNICODE_UTF8)
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
  }

  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=*nCodePage;
  fsd.nBytesMax=nPreviewBytes;
  FileStreamIn(&fsd);
  CloseHandle(hFile);

  return 0;
}

int AutodetectCodePageA(char *pFile, DWORD dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
{
  HANDLE hFile;
  DWORD dwBytesRead;
  unsigned char *pBuffer=NULL;
  int nRegCodePage=0;
  int a;
  int b;
  BOOL bUtfCodePage=FALSE;
  BOOL bUtfBOM=FALSE;

  //Remembered code page from registry
  if (dwFlags & ADT_REG_CODEPAGE)
  {
    dwFlags&=~ADT_REG_CODEPAGE;
    dwFlags&=~ADT_DETECT_CODEPAGE;
    dwFlags&=~ADT_DETECT_BOM;

    if (nRecentFiles)
    {
      RecentFilesZeroA();
      RecentFilesReadA();
    }
    if (nRecentFiles && bSaveCodepages && RecentFilesGetA(pFile, NULL, &nRegCodePage) && nRegCodePage)
    {
      *nCodePage=nRegCodePage;
      dwFlags|=ADT_DETECT_BOM;
    }
    else
    {
      dwFlags|=ADT_DETECT_CODEPAGE|ADT_DETECT_BOM;
    }
  }

  //Default
  if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=nDefaultCodePage;
  if (dwFlags & ADT_DETECT_BOM) *bBOM=FALSE;

  //Read file
  if (dwFlags & ADT_BINARY_ERROR || dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if ((hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
      return EDT_OPEN;

    if (!(pBuffer=(unsigned char *)API_HeapAlloc(hHeap, 0, dwBytesToCheck + 1)))
    {
      CloseHandle(hFile);
      return EDT_ALLOC;
    }

    if (!API_ReadFile(hFile, pBuffer, dwBytesToCheck, &dwBytesRead, NULL))
    {
      API_HeapFree(hHeap, 0, (LPVOID)pBuffer);
      CloseHandle(hFile);
      return EDT_READ;
    }
    CloseHandle(hFile);
  }

  //Detect Unicode BOM
  if (dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if (dwBytesRead >= 2)
    {
      if (pBuffer[0] == 0xFF && pBuffer[1] == 0xFE)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UCS2_LE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UCS2_LE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
      else if (pBuffer[0] == 0xFE && pBuffer[1] == 0xFF)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UCS2_BE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UCS2_BE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
    }
    if (dwBytesRead >= 3)
    {
      if (pBuffer[0] == 0xEF && pBuffer[1] == 0xBB && pBuffer[2] == 0xBF)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UTF8)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF8;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
    }
  }

  if (dwFlags & ADT_BINARY_ERROR || dwFlags & ADT_DETECT_CODEPAGE)
  {
    if (dwBytesRead >= 2)
    {
      for (a=0, b=dwBytesRead - 1; a < b; ++a)
      {
        if (a % 2 == 0)
        {
          //Detect binary file
          if (dwFlags & ADT_BINARY_ERROR)
          {
            if (pBuffer[a] == 0x00 && pBuffer[a + 1] == 0x00)
            {
              if (bUtfCodePage) *nCodePage=nDefaultCodePage;
              if (bUtfBOM) *bBOM=FALSE;
              API_HeapFree(hHeap, 0, (LPVOID)pBuffer);
              return EDT_BINARY;
            }
          }

          //Detect UTF-16LE, UTF-16BE without BOM
          if (dwFlags & ADT_DETECT_CODEPAGE)
          {
            if (pBuffer[a + 1] == 0x00 && (pBuffer[a] == 0x0D || pBuffer[a] == 0x0A))
            {
              *nCodePage=CP_UNICODE_UCS2_LE;
              dwFlags&=~ADT_DETECT_CODEPAGE;
              bUtfCodePage=TRUE;

              if (dwFlags & ADT_DETECT_BOM)
              {
                *bBOM=FALSE;
                dwFlags&=~ADT_DETECT_BOM;
                bUtfBOM=TRUE;
              }
            }
            else if (pBuffer[a] == 0x00 && (pBuffer[a + 1] == 0x0D || pBuffer[a + 1] == 0x0A))
            {
              *nCodePage=CP_UNICODE_UCS2_BE;
              dwFlags&=~ADT_DETECT_CODEPAGE;
              bUtfCodePage=TRUE;

              if (dwFlags & ADT_DETECT_BOM)
              {
                *bBOM=FALSE;
                dwFlags&=~ADT_DETECT_BOM;
                bUtfBOM=TRUE;
              }
            }
          }
        }
      }
    }
  }

  //Detect non-Unicode
  if (dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if (dwFlags & ADT_DETECT_CODEPAGE)
    {
      if (!AutodetectMultibyte(PRIMARYLANGID(dwLangCodepageRecognition), pBuffer, dwBytesRead, nCodePage))
      {
        *nCodePage=nDefaultCodePage;
        dwFlags&=~ADT_DETECT_CODEPAGE;
      }
    }
    if (dwFlags & ADT_DETECT_BOM)
    {
      *bBOM=FALSE;
      dwFlags&=~ADT_DETECT_BOM;
    }
  }

  //Free buffer
  Free:
  if (pBuffer) API_HeapFree(hHeap, 0, (LPVOID)pBuffer);
  return 0;
}

int AutodetectCodePageW(wchar_t *wpFile, DWORD dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
{
  HANDLE hFile;
  DWORD dwBytesRead;
  unsigned char *pBuffer=NULL;
  int nRegCodePage=0;
  int a;
  int b;
  BOOL bUtfCodePage=FALSE;
  BOOL bUtfBOM=FALSE;

  //Remembered code page from registry
  if (dwFlags & ADT_REG_CODEPAGE)
  {
    dwFlags&=~ADT_REG_CODEPAGE;
    dwFlags&=~ADT_DETECT_CODEPAGE;
    dwFlags&=~ADT_DETECT_BOM;

    if (nRecentFiles)
    {
      RecentFilesZeroW();
      RecentFilesReadW();
    }
    if (nRecentFiles && bSaveCodepages && RecentFilesGetW(wpFile, NULL, &nRegCodePage) && nRegCodePage)
    {
      *nCodePage=nRegCodePage;
      dwFlags|=ADT_DETECT_BOM;
    }
    else
    {
      dwFlags|=ADT_DETECT_CODEPAGE|ADT_DETECT_BOM;
    }
  }

  //Default
  if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=nDefaultCodePage;
  if (dwFlags & ADT_DETECT_BOM) *bBOM=FALSE;

  //Read file
  if (dwFlags & ADT_BINARY_ERROR || dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if ((hFile=API_CreateFileW(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
      return EDT_OPEN;

    if (!(pBuffer=(unsigned char *)API_HeapAlloc(hHeap, 0, dwBytesToCheck + 1)))
    {
      CloseHandle(hFile);
      return EDT_ALLOC;
    }

    if (!API_ReadFile(hFile, pBuffer, dwBytesToCheck, &dwBytesRead, NULL))
    {
      API_HeapFree(hHeap, 0, (LPVOID)pBuffer);
      CloseHandle(hFile);
      return EDT_READ;
    }
    CloseHandle(hFile);
  }

  //Detect Unicode BOM
  if (dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if (dwBytesRead >= 2)
    {
      if (pBuffer[0] == 0xFF && pBuffer[1] == 0xFE)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UCS2_LE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UCS2_LE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
      else if (pBuffer[0] == 0xFE && pBuffer[1] == 0xFF)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UCS2_BE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UCS2_BE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
    }
    if (dwBytesRead >= 3)
    {
      if (pBuffer[0] == 0xEF && pBuffer[1] == 0xBB && pBuffer[2] == 0xBF)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UTF8)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF8;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
    }
  }

  if (dwFlags & ADT_BINARY_ERROR || dwFlags & ADT_DETECT_CODEPAGE)
  {
    if (dwBytesRead >= 2)
    {
      for (a=0, b=dwBytesRead - 1; a < b; ++a)
      {
        if (a % 2 == 0)
        {
          //Detect binary file
          if (dwFlags & ADT_BINARY_ERROR)
          {
            if (pBuffer[a] == 0x00 && pBuffer[a + 1] == 0x00)
            {
              if (bUtfCodePage) *nCodePage=nDefaultCodePage;
              if (bUtfBOM) *bBOM=FALSE;
              API_HeapFree(hHeap, 0, (LPVOID)pBuffer);
              return EDT_BINARY;
            }
          }

          //Detect UTF-16LE, UTF-16BE without BOM
          if (dwFlags & ADT_DETECT_CODEPAGE)
          {
            if (pBuffer[a + 1] == 0x00 && (pBuffer[a] == 0x0D || pBuffer[a] == 0x0A))
            {
              *nCodePage=CP_UNICODE_UCS2_LE;
              dwFlags&=~ADT_DETECT_CODEPAGE;
              bUtfCodePage=TRUE;

              if (dwFlags & ADT_DETECT_BOM)
              {
                *bBOM=FALSE;
                dwFlags&=~ADT_DETECT_BOM;
                bUtfBOM=TRUE;
              }
            }
            else if (pBuffer[a] == 0x00 && (pBuffer[a + 1] == 0x0D || pBuffer[a + 1] == 0x0A))
            {
              *nCodePage=CP_UNICODE_UCS2_BE;
              dwFlags&=~ADT_DETECT_CODEPAGE;
              bUtfCodePage=TRUE;

              if (dwFlags & ADT_DETECT_BOM)
              {
                *bBOM=FALSE;
                dwFlags&=~ADT_DETECT_BOM;
                bUtfBOM=TRUE;
              }
            }
          }
        }
      }
    }
  }

  //Detect non-Unicode
  if (dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if (dwFlags & ADT_DETECT_CODEPAGE)
    {
      if (!AutodetectMultibyte(PRIMARYLANGID(dwLangCodepageRecognition), pBuffer, dwBytesRead, nCodePage))
      {
        *nCodePage=nDefaultCodePage;
        dwFlags&=~ADT_DETECT_CODEPAGE;
      }
    }
    if (dwFlags & ADT_DETECT_BOM)
    {
      *bBOM=FALSE;
      dwFlags&=~ADT_DETECT_BOM;
    }
  }

  //Free buffer
  Free:
  if (pBuffer) API_HeapFree(hHeap, 0, (LPVOID)pBuffer);
  return 0;
}

BOOL AutodetectMultibyte(DWORD dwLangID, unsigned char *pBuffer, DWORD dwBytesToCheck, int *nCodePage)
{
  char szANSIwatermark[128]="";
  char szOEMwatermark[128]="";
  char szKOIwatermark[128]="";
  char szUTF8watermark[128]="";
  int nANSIrate=5;
  int nOEMrate=0;
  int nKOIrate=0;
  int nUTF8rate=0;
  DWORD dwCounter[0x80];
  DWORD dwMaxIndex=0;
  DWORD dwMaxCount=0;
  DWORD i;
  DWORD j;

  //Watermarks
  if (dwLangID == LANG_RUSSIAN)
  {
    lstrcpyA(szANSIwatermark, "\xE0\xE1\xE2\xE5\xE8\xED\xEE\xEF\xF0\xF2\xC0\xC1\xC2\xC5\xC8\xCD\xCE\xCF\xD2");  //абвеинопртАБВЕИНОПТ
    lstrcpyA(szOEMwatermark,  "\xAE\xA5\xA0\xA8\xAD\xE2\x8E\x45\x80\x88\x8D\x92\xB0\xB1\xB2\xB3\xBA\xDB");      //Character graphics simbols: \xB0\xB1\xB2\xB3\xBA\xDB
    lstrcpyA(szKOIwatermark,  "\xC1\xC2\xD7\xC5\xC9\xCE\xCF\xD2\xD4\xE1\xE2\xF7\xE5\xE9\xEE\xEF\xF0\xF2\xF4");  //БВЧЕЙОПТФбвчейопртф
    lstrcpyA(szUTF8watermark, "\xD0\xD1");
  }
  else if (dwLangID == LANG_ENGLISH)
  {
    lstrcpyA(szOEMwatermark, "\xB0\xB1\xB2\xB3\xBA\xDB");
  }
  else if (dwLangID == LANG_TURKISH)
  {
    lstrcpyA(szANSIwatermark, "\xFC\xFD\xFE");
    lstrcpyA(szOEMwatermark,  "\x81\x87\x8D\xB0\xB1\xB2\xB3\xBA\xDB");  //Character graphics simbols: \xB0\xB1\xB2\xB3\xBA\xDB
    lstrcpyA(szUTF8watermark, "\xB0\xB1\xBC\xC3\xC4\xC5");
  }
  else if (dwLangID == LANG_CHINESE)
  {
    lstrcpyA(szANSIwatermark, "\xA1\xA2\xA3\xA4\xA5\xA6\xC0\xC1\xC2\xC3\xC4\xC5");
    lstrcpyA(szUTF8watermark, "\xE3\xE4\xE5\xE6\xE7\xE8");
  }
  else if (dwLangID == LANG_JAPANESE)
  {
    lstrcpyA(szANSIwatermark, "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF");
    lstrcpyA(szUTF8watermark, "\xE3");
  }
  else if (dwLangID == LANG_KOREAN)
  {
    lstrcpyA(szANSIwatermark, "\xC0\xC1\xC2\xC3");
    lstrcpyA(szUTF8watermark, "\xEA\xEB\xEC\xED");
  }
  else return FALSE;

  //Zero counter
  memset(dwCounter, 0, 0x80 * sizeof(DWORD));

  //Count number of each character in input buffer
  for (j=0, i=0; i < dwBytesToCheck; ++i)
  {
    //Char in range 0x80 - 0xFF
    if (pBuffer[i] >= 0x80)
    {
      ++j;
      dwCounter[pBuffer[i] - 0x80]++;
    }
  }

  //Give it up if there's no representative selection
  if (j > 10)
  {
    for (j=0; j < 10; ++j)
    {
      //Get max element
      for (dwMaxCount=0, i=0; i < 0x80; ++i)
      {
        if (dwCounter[i] > dwMaxCount)
        {
          dwMaxCount=dwCounter[i];
          dwMaxIndex=i;
        }
      }
      if (!dwCounter[dwMaxIndex]) break;

      if (strchr(szANSIwatermark, dwMaxIndex + 0x80)) nANSIrate+=dwCounter[dwMaxIndex];
      if (strchr(szOEMwatermark, dwMaxIndex + 0x80)) nOEMrate+=dwCounter[dwMaxIndex];
      if (strchr(szKOIwatermark, dwMaxIndex + 0x80)) nKOIrate+=dwCounter[dwMaxIndex];
      if (strchr(szUTF8watermark, dwMaxIndex + 0x80)) nUTF8rate+=dwCounter[dwMaxIndex];
      dwCounter[dwMaxIndex]=0;
    }

    //Set code page
    if (dwLangID == LANG_RUSSIAN)
    {
      if (nANSIrate >= nOEMrate && nANSIrate >= nKOIrate && nANSIrate >= nUTF8rate)
      {
        *nCodePage=1251;
      }
      else if (nOEMrate >= nKOIrate && nOEMrate >= nUTF8rate)
      {
        *nCodePage=866;
      }
      else if (nKOIrate >= nUTF8rate)
      {
        *nCodePage=CP_KOI8_R;
      }
      else
      {
        *nCodePage=CP_UNICODE_UTF8;
      }
      return TRUE;
    }
    else if (dwLangID == LANG_ENGLISH)
    {
      if (nOEMrate > nANSIrate)
      {
        *nCodePage=nOemCodePage;
        return TRUE;
      }
    }
    else if (dwLangID == LANG_TURKISH)
    {
      if (nANSIrate >= nOEMrate && nANSIrate >= nUTF8rate)
      {
      }
      else if (nOEMrate >= nUTF8rate)
      {
        *nCodePage=857;
        return TRUE;
      }
      else
      {
        *nCodePage=CP_UNICODE_UTF8;
        return TRUE;
      }
    }
    else if (dwLangID == LANG_CHINESE)
    {
      if (nUTF8rate > nANSIrate)
      {
        *nCodePage=CP_UNICODE_UTF8;
        return TRUE;
      }
    }
    else if (dwLangID == LANG_JAPANESE)
    {
      if (nUTF8rate > nANSIrate)
      {
        *nCodePage=CP_UNICODE_UTF8;
        return TRUE;
      }
    }
    else if (dwLangID == LANG_KOREAN)
    {
      if (nUTF8rate > nANSIrate)
      {
        *nCodePage=CP_UNICODE_UTF8;
        return TRUE;
      }
    }
  }
  return FALSE;
}

unsigned int UTF8toUTF16(const unsigned char *pMultiString, unsigned int nMultiStringLen, unsigned int *nMultiStringDone,  wchar_t *wszWideString, unsigned int nWideStringMax)
{
  unsigned int lpOffsetsFromUTF8[6]={0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080};
  unsigned int i=0;
  unsigned int ti=0;
  unsigned int nChar;
  unsigned int nRead=0;
  unsigned int nTrailing;

  while (i < nMultiStringLen && ti < nWideStringMax)
  {
    if (pMultiString[i] < 0x80)
    {
      nTrailing=0;
    }
    else if (pMultiString[i] < 0xC0)
    {
      //Trailing byte in leading position
      nRead=++i;
      continue;
    }
    else if (pMultiString[i] < 0xE0)
    {
      if (i + 1 >= nMultiStringLen) break;
      nTrailing=1;
    }
    else if (pMultiString[i] < 0xF0)
    {
      if (i + 2 >= nMultiStringLen) break;
      nTrailing=2;
    }
    else if (pMultiString[i] < 0xF8)
    {
      if (i + 3 >= nMultiStringLen) break;
      nTrailing=3;
    }
    else
    {
      //No chance for this in UTF-16
      nRead=++i;
      continue;
    }

    //Get unicode char
    nChar=0;

    switch (nTrailing)
    {
      case 3:
      {
        nChar+=pMultiString[i++];
        nChar=nChar << 6;
      }
      case 2:
      {
        nChar+=pMultiString[i++];
        nChar=nChar << 6;
      }
      case 1:
      {
        nChar+=pMultiString[i++];
        nChar=nChar << 6;
      }
      case 0:
      {
        nChar+=pMultiString[i++];
      }
    }
    nChar-=lpOffsetsFromUTF8[nTrailing];

    //Write unicode char
    if (nChar <= 0xFFFF)
    {
      wszWideString[ti]=nChar;
    }
    else
    {
      //Surrogate pair
      if (ti + 1 >= nWideStringMax) break;
      nChar-=0x10000;
      wszWideString[ti++]=(nChar >> 10) + 0xD800;
      wszWideString[ti]=(nChar & 0x3ff) + 0xDC00;
    }
    nRead=i;
    ++ti;
  }
  if (nMultiStringDone)
    *nMultiStringDone=nRead;
  return ti;
}

unsigned int UTF16toUTF8(const wchar_t *wpWideString, unsigned int nWideStringLen, unsigned int *nWideStringDone, char *szMultiString, unsigned int nMultiStringMax)
{
  unsigned int lpFirstByteMark[7]={0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
  unsigned int i=0;
  unsigned int ti=0;
  unsigned int nChar;
  unsigned int nRead=0;
  unsigned int nBitesInChar;

  while (ti < nWideStringLen && i < nMultiStringMax)
  {
    nChar=wpWideString[ti];

    //Surrogate pair. High surrogate.
    if (nChar >= 0xD800 && nChar <= 0xDBFF)
    {
      if (ti + 1 >= nWideStringLen) break;
      ++ti;

      //Low surrogate
      if (wpWideString[ti] >= 0xDC00 && wpWideString[ti] <= 0xDFFF)
      {
        nChar=0x10000 + ((nChar - 0xD800) << 10) + (wpWideString[ti] - 0xDC00);
      }
      else
      {
        nRead=ti;
        continue;
      }
    }

    if (nChar < 0x110000)
    {
      if (nChar >= 0x10000)
      {
        if (i + 3 >= nMultiStringMax) break;
        nBitesInChar=4;
      }
      else if (nChar >= 0x800)
      {
        if (i + 2 >= nMultiStringMax) break;
        nBitesInChar=3;
      }
      else if (nChar >= 0x80)
      {
        if (i + 1 >= nMultiStringMax) break;
        nBitesInChar=2;
      }
      else if (nChar >= 0)
      {
        nBitesInChar=1;
      }

      switch (nBitesInChar)
      {
        case 4:
        {
          szMultiString[i + 3]=(nChar | 0x80) & 0xBF;
          nChar=nChar >> 6;
        }
        case 3:
        {
          szMultiString[i + 2]=(nChar | 0x80) & 0xBF;
          nChar=nChar >> 6;
        }
        case 2:
        {
          szMultiString[i + 1]=(nChar | 0x80) & 0xBF;
          nChar=nChar >> 6;
        }
        case 1:
        {
          szMultiString[i]=nChar | lpFirstByteMark[nBitesInChar];
        }
      }
      i+=nBitesInChar;
    }
    nRead=++ti;
  }
  if (nWideStringDone)
    *nWideStringDone=nRead;
  return i;
}

void ChangeByteOrder(unsigned char *lpBuffer, unsigned int nBufferLen)
{
  unsigned char ch;
  unsigned int a;
  unsigned int b;

  for (a=0, b=1; b < nBufferLen; a+=2, b+=2)
  {
    ch=lpBuffer[a];
    lpBuffer[a]=lpBuffer[b];
    lpBuffer[b]=ch;
  }
}

BOOL CheckCodePage(int nCodePage)
{
  char ch='A';
  wchar_t wch;

  if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
    return TRUE;
  if (MultiByteToWideChar(nCodePage, 0, &ch, 1, &wch, 1))
    return TRUE;

  return FALSE;
}

unsigned int TranslateNewLinesToUnixW(wchar_t *wpWideString, unsigned int nWideString)
{
  unsigned int a;
  unsigned int b;

  for (a=0, b=0; a < nWideString && wpWideString[a]; ++a, ++b)
  {
    if (wpWideString[a] == '\r' && wpWideString[a + 1] == '\n') ++a;
    wpWideString[b]=wpWideString[a];
  }
  return b;
}


//// Find/Replace

BOOL CALLBACK FindAndReplaceDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFind;
  static HWND hWndReplace;
  static HWND hWndMatchCase;
  static HWND hWndEscapeSeq;
  static HWND hWndForward;
  static HWND hWndBackward;
  static HWND hWndBeginning;
  static HWND hWndInSelection;
  static HWND hWndAllFiles;
  static HWND hWndAllFilesGroup;
  static HWND hWndFindNextButton;
  static HWND hWndReplaceButton;
  static HWND hWndReplaceAllButton;
  static HWND hWndCancelButton;
  static BOOL bCanReplace=TRUE;
  static BOOL bSpecialCheck=FALSE;
  char *szData;
  HWND hWndFocus=NULL;
  HWND hWndComboboxEdit;
  BOOL bReplace=FALSE;
  BOOL bReplaceAll=FALSE;
  BOOL bReplaceAllButtonState=FALSE;
  int nReplaceCount;
  int nResult;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndFind=GetDlgItem(hDlg, IDC_SEARCH_FIND);
    hWndFindNextButton=GetDlgItem(hDlg, IDC_SEARCH_FIND_BUTTON);
    hWndCancelButton=GetDlgItem(hDlg, IDCANCEL);
    hWndMatchCase=GetDlgItem(hDlg, IDC_SEARCH_MATCHCASE);
    hWndEscapeSeq=GetDlgItem(hDlg, IDC_SEARCH_ESCAPESEQ);
    hWndForward=GetDlgItem(hDlg, IDC_SEARCH_FORWARD);
    hWndBackward=GetDlgItem(hDlg, IDC_SEARCH_BACKWARD);
    hWndBeginning=GetDlgItem(hDlg, IDC_SEARCH_BEGINNING);
    hWndInSelection=GetDlgItem(hDlg, IDC_SEARCH_INSEL);
    if (bReplaceDlg == TRUE)
    {
      hWndReplace=GetDlgItem(hDlg, IDC_SEARCH_REPLACE);
      hWndReplaceButton=GetDlgItem(hDlg, IDC_SEARCH_REPLACE_BUTTON);
      hWndReplaceAllButton=GetDlgItem(hDlg, IDC_SEARCH_ALL_BUTTON);
    }
    hWndAllFiles=GetDlgItem(hDlg, IDC_SEARCH_ALLFILES);
    hWndAllFilesGroup=GetDlgItem(hDlg, IDC_SEARCH_ALLFILES_GROUP);

    if (nSearchStrings)
    {
      FillComboboxSearchA(hWndFind, hWndReplace);
    }

    if (chrg.cpMin == chrg.cpMax || (ftflags & FR_SELECTION))
    {
      SendMessage(hWndFind, CB_SETCURSEL, 0, 0);
    }
    else
    {
      if (GetRangeTextA(hWndEdit, chrg.cpMin, chrg.cpMax, &szData))
      {
        SetWindowTextA(hWndFind, szData);
        FreeText((LPVOID)szData);
      }
    }
    if (!bMDI)
    {
      ShowWindow(hWndAllFiles, SW_HIDE);
      ShowWindow(hWndAllFilesGroup, SW_HIDE);
    }
    if (bReplaceDlg == TRUE)
    {
      SendMessage(hWndReplace, CB_SETCURSEL, 0, 0);
    }

    if (ftflags & FR_ALLFILES)
      SendMessage(hWndAllFiles, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_SELECTION)
      SendMessage(hWndInSelection, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_BEGINNING)
      SendMessage(hWndBeginning, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_DOWN)
      SendMessage(hWndForward, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_UP)
      SendMessage(hWndBackward, BM_SETCHECK, BST_CHECKED, 0);

    if (ftflags & FR_MATCHCASE) SendMessage(hWndMatchCase, BM_SETCHECK, BST_CHECKED, 0);
    if (ftflags & FR_ESCAPESEQ) SendMessage(hWndEscapeSeq, BM_SETCHECK, BST_CHECKED, 0);

    if (hWndComboboxEdit=GetDlgItem(hWndFind, IDC_COMBOBOX_EDIT))
    {
      SendMessage(hWndComboboxEdit, EM_LIMITTEXT, 0, 0);
      OldComboboxEdit=(WNDPROC)GetWindowLongA(hWndComboboxEdit, GWL_WNDPROC);
      SetWindowLongA(hWndComboboxEdit, GWL_WNDPROC, (LONG)NewComboboxEditProcA);
    }
    if (bReplaceDlg == TRUE)
    {
      if (hWndComboboxEdit=GetDlgItem(hWndReplace, IDC_COMBOBOX_EDIT))
      {
        SendMessage(hWndComboboxEdit, EM_LIMITTEXT, 0, 0);
        OldComboboxEdit=(WNDPROC)GetWindowLongA(hWndComboboxEdit, GWL_WNDPROC);
        SetWindowLongA(hWndComboboxEdit, GWL_WNDPROC, (LONG)NewComboboxEditProcA);
      }
    }
    if (rcFindAndReplaceDlg.right && rcFindAndReplaceDlg.bottom)
      SetWindowPos(hDlg, 0, rcFindAndReplaceDlg.left, rcFindAndReplaceDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    SendMessage(hDlg, WM_COMMAND, IDC_SETREADONLY, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SEARCH_REPLACE)
      return TRUE;
    else if (LOWORD(wParam) == IDC_SEARCH_MATCHCASE)
    {
      if (SendMessage(hWndMatchCase, BM_GETCHECK, 0, 0)) ftflags|=FR_MATCHCASE;
      else ftflags&=~FR_MATCHCASE;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ESCAPESEQ)
    {
      if (SendMessage(hWndEscapeSeq, BM_GETCHECK, 0, 0)) ftflags|=FR_ESCAPESEQ;
      else ftflags&=~FR_ESCAPESEQ;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_FORWARD)
    {
      ftflags&=~FR_UP&~FR_BEGINNING&~FR_SELECTION&~FR_ALLFILES;
      ftflags|=FR_DOWN;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BACKWARD)
    {
      ftflags&=~FR_DOWN&~FR_BEGINNING&~FR_SELECTION&~FR_ALLFILES;
      ftflags|=FR_UP;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BEGINNING)
    {
      ftflags&=~FR_UP&~FR_SELECTION&~FR_ALLFILES;
      ftflags|=FR_DOWN|FR_BEGINNING;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_INSEL)
    {
      ftflags&=~FR_UP&~FR_BEGINNING&~FR_ALLFILES;
      ftflags|=FR_DOWN|FR_SELECTION;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ALLFILES)
    {
      ftflags&=~FR_UP&~FR_SELECTION;
      ftflags|=FR_DOWN|FR_BEGINNING|FR_ALLFILES;
    }
    else if (LOWORD(wParam) == IDC_SETREADONLY)
    {
      if (bReplaceDlg == TRUE)
      {
        EnableWindow(hWndReplaceButton, !bReadOnly);
        EnableWindow(hWndReplaceAllButton, !bReadOnly);
      }
    }

    if (LOWORD(wParam) == IDC_SEARCH_FIND ||
        LOWORD(wParam) == IDC_SEARCH_FORWARD ||
        LOWORD(wParam) == IDC_SEARCH_BACKWARD ||
        LOWORD(wParam) == IDC_SEARCH_BEGINNING ||
        LOWORD(wParam) == IDC_SEARCH_INSEL ||
        LOWORD(wParam) == IDC_SEARCH_ALLFILES)
    {
      EnableWindow(hWndFindNextButton, TRUE);
      if (bReplaceDlg == TRUE)
      {
        if (!bReadOnly)
        {
          EnableWindow(hWndReplaceButton, TRUE);
          EnableWindow(hWndReplaceAllButton, TRUE);
        }
      }
      if (LOWORD(wParam) != IDC_SEARCH_FIND)
      {
        bSpecialCheck=FALSE;
        SendMessage(hWndBeginning, BM_SETSTATE, FALSE, 0);
        if (bMDI) SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
      }

      if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        i=SendMessageA(hWndFind, CB_GETLBTEXTLEN, (WPARAM)SendMessage(hWndFind, CB_GETCURSEL, 0, 0), 0);
      }
      else
      {
        i=GetWindowTextLength(hWndFind);
      }

      if (i == 0)
      {
        EnableWindow(hWndFindNextButton, FALSE);
        if (bReplaceDlg == TRUE)
        {
          EnableWindow(hWndReplaceButton, FALSE);
          EnableWindow(hWndReplaceAllButton, FALSE);
        }
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      if (bSpecialCheck == TRUE || (ftflags & FR_ALLFILES)) ftflags|=FR_BEGINNING;
      if (nSearchStrings) SaveComboboxSearchA(hWndFind, hWndReplace);
      GetWindowPos(hDlg, NULL, &rcFindAndReplaceDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_REPLACE_BUTTON)
    {
      bReplace=TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ALL_BUTTON)
    {
      bReplaceAll=TRUE;
    }
    if (LOWORD(wParam) == IDC_SEARCH_FIND_BUTTON ||
        LOWORD(wParam) == IDC_SEARCH_REPLACE_BUTTON ||
        LOWORD(wParam) == IDC_SEARCH_ALL_BUTTON)
    {
      FreeMemorySearchA();

      if (!GetComboboxSearchTextA(hWndFind, &szFind_orig, &szFind))
      {
        FreeMemorySearchA();
        return TRUE;
      }
      if (bReplaceDlg == TRUE)
      {
        if (!GetComboboxSearchTextA(hWndReplace, &szReplace_orig, &szReplace))
        {
          FreeMemorySearchA();
          return TRUE;
        }
      }

      hWndFocus=GetFocus();
      if (bReplaceAll)
        bReplaceAllButtonState=EnableWindow(hWndReplaceAllButton, FALSE);

      if (ftflags & FR_ALLFILES)
      {
        HWND hWndTmp=hWndFrameActive;
        int nChanges=0;
        int nChangedFiles=0;

        if (bReplaceDlg == TRUE && bReplaceAll == TRUE)
        {
          ftflags|=FR_BEGINNING;
          if (bSpecialCheck == TRUE)
          {
            bSpecialCheck=FALSE;
            SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
          }

          do
          {
            _ReplaceTextA(hWndEdit, ftflags, szFind, szReplace, TRUE, &nReplaceCount);
            if (!hDlgModeless) break;

            if (nReplaceCount)
            {
              ++nChangedFiles;
              nChanges+=nReplaceCount;
            }
            SendMessage(hMdiClient, WM_MDINEXT, (WPARAM)hWndFrameActive, FALSE);
          }
          while (hWndFrameActive != hWndTmp);

          if (!bReplaceAllAndClose)
          {
            API_LoadStringA(hLangLib, MSG_REPLACE_COUNT_ALLFILES, buf, BUFFER_SIZE);
            wsprintfA(buf2, buf, nChangedFiles, nChanges);
            MessageBoxA(hDlg, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONINFORMATION);
          }
          else
          {
            PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
            return TRUE;
          }
        }
        else
        {
          if (bSpecialCheck == FALSE)
          {
            bSpecialCheck=TRUE;
            SendMessage(hWndAllFiles, BM_SETSTATE, TRUE, 0);
          }

          do
          {
            if (bReplace == TRUE && bCanReplace == TRUE)
              nResult=_ReplaceTextA(hWndEdit, ftflags, szFind, szReplace, FALSE, NULL);
            else
              nResult=_FindTextA(hWndEdit, ftflags, szFind);

            if (nResult == -1)
            {
              bCanReplace=FALSE;
              ftflags|=FR_BEGINNING;
              SendMessage(hMdiClient, WM_MDINEXT, (WPARAM)hWndFrameActive, FALSE);
            }
            else
            {
              bCanReplace=TRUE;
              ftflags&=~FR_BEGINNING;
              break;
            }
          }
          while (hWndFrameActive != hWndTmp);

          if (nResult == -1)
          {
            bSpecialCheck=FALSE;
            SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
            API_LoadStringA(hLangLib, MSG_SEARCH_ENDED, buf, BUFFER_SIZE);
            MessageBoxA(hDlg, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONINFORMATION);
          }
        }
      }
      else
      {
        if (bReplaceDlg == TRUE && (bReplace == TRUE || bReplaceAll == TRUE))
          nResult=_ReplaceTextA(hWndEdit, ftflags, szFind, szReplace, bReplaceAll, &nReplaceCount);
        else
          nResult=_FindTextA(hWndEdit, ftflags, szFind);

        if (nResult == -1)
        {
          if (bSpecialCheck == TRUE)
          {
            ftflags|=FR_BEGINNING;
            bSpecialCheck=FALSE;
            SendMessage(hWndBeginning, BM_SETSTATE, FALSE, 0);
          }
          if (bReplaceAll == TRUE)
          {
            if (!bReplaceAllAndClose)
            {
              API_LoadStringA(hLangLib, MSG_REPLACE_COUNT, buf, BUFFER_SIZE);
              wsprintfA(buf2, buf, nReplaceCount);
              MessageBoxA(hDlg, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONINFORMATION);
            }
            else
            {
              PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
              return TRUE;
            }
          }
          else
          {
            API_LoadStringA(hLangLib, MSG_SEARCH_ENDED, buf, BUFFER_SIZE);
            MessageBoxA(hDlg, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONINFORMATION);
          }
        }
        else
        {
          if (ftflags & FR_BEGINNING)
          {
            ftflags&=~FR_BEGINNING;
            bSpecialCheck=TRUE;
            SendMessage(hWndBeginning, BM_SETSTATE, TRUE, 0);
          }
        }
      }
      if (bReplaceAll)
        EnableWindow(hWndReplaceAllButton, !bReplaceAllButtonState);
      SetFocus(hWndFocus);

      return TRUE;
    }
  }
  else if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_F3)
    {
      if (GetWindowTextLength(hWndFind))
      {
        FreeMemorySearchA();

        if (!GetComboboxSearchTextA(hWndFind, &szFind_orig, &szFind))
        {
          FreeMemorySearchA();
          return TRUE;
        }

        if (GetKeyState(VK_SHIFT) >= 0)
          DoEditFindNextDownA(hWndEdit);
        else
          DoEditFindNextUpA(hWndEdit);

        PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK FindAndReplaceDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFind;
  static HWND hWndReplace;
  static HWND hWndMatchCase;
  static HWND hWndEscapeSeq;
  static HWND hWndForward;
  static HWND hWndBackward;
  static HWND hWndBeginning;
  static HWND hWndInSelection;
  static HWND hWndAllFiles;
  static HWND hWndAllFilesGroup;
  static HWND hWndFindNextButton;
  static HWND hWndReplaceButton;
  static HWND hWndReplaceAllButton;
  static HWND hWndCancelButton;
  static BOOL bCanReplace=TRUE;
  static BOOL bSpecialCheck=FALSE;
  wchar_t *wszData;
  HWND hWndFocus=NULL;
  HWND hWndComboboxEdit;
  BOOL bReplace=FALSE;
  BOOL bReplaceAll=FALSE;
  BOOL bReplaceAllButtonState=FALSE;
  int nReplaceCount;
  int nResult;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndFind=GetDlgItem(hDlg, IDC_SEARCH_FIND);
    hWndFindNextButton=GetDlgItem(hDlg, IDC_SEARCH_FIND_BUTTON);
    hWndCancelButton=GetDlgItem(hDlg, IDCANCEL);
    hWndMatchCase=GetDlgItem(hDlg, IDC_SEARCH_MATCHCASE);
    hWndEscapeSeq=GetDlgItem(hDlg, IDC_SEARCH_ESCAPESEQ);
    hWndForward=GetDlgItem(hDlg, IDC_SEARCH_FORWARD);
    hWndBackward=GetDlgItem(hDlg, IDC_SEARCH_BACKWARD);
    hWndBeginning=GetDlgItem(hDlg, IDC_SEARCH_BEGINNING);
    hWndInSelection=GetDlgItem(hDlg, IDC_SEARCH_INSEL);
    if (bReplaceDlg == TRUE)
    {
      hWndReplace=GetDlgItem(hDlg, IDC_SEARCH_REPLACE);
      hWndReplaceButton=GetDlgItem(hDlg, IDC_SEARCH_REPLACE_BUTTON);
      hWndReplaceAllButton=GetDlgItem(hDlg, IDC_SEARCH_ALL_BUTTON);
    }
    hWndAllFiles=GetDlgItem(hDlg, IDC_SEARCH_ALLFILES);
    hWndAllFilesGroup=GetDlgItem(hDlg, IDC_SEARCH_ALLFILES_GROUP);

    if (nSearchStrings)
    {
      FillComboboxSearchW(hWndFind, hWndReplace);
    }

    if (chrg.cpMin == chrg.cpMax || (ftflags & FR_SELECTION))
    {
      SendMessage(hWndFind, CB_SETCURSEL, 0, 0);
    }
    else
    {
      if (GetRangeTextW(hWndEdit, chrg.cpMin, chrg.cpMax, &wszData))
      {
        SetWindowTextW(hWndFind, wszData);
        FreeText((LPVOID)wszData);
      }
    }
    if (!bMDI)
    {
      ShowWindow(hWndAllFiles, SW_HIDE);
      ShowWindow(hWndAllFilesGroup, SW_HIDE);
    }
    if (bReplaceDlg == TRUE)
    {
      SendMessage(hWndReplace, CB_SETCURSEL, 0, 0);
    }

    if (ftflags & FR_ALLFILES)
      SendMessage(hWndAllFiles, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_SELECTION)
      SendMessage(hWndInSelection, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_BEGINNING)
      SendMessage(hWndBeginning, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_DOWN)
      SendMessage(hWndForward, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & FR_UP)
      SendMessage(hWndBackward, BM_SETCHECK, BST_CHECKED, 0);

    if (ftflags & FR_MATCHCASE) SendMessage(hWndMatchCase, BM_SETCHECK, BST_CHECKED, 0);
    if (ftflags & FR_ESCAPESEQ) SendMessage(hWndEscapeSeq, BM_SETCHECK, BST_CHECKED, 0);

    if (hWndComboboxEdit=GetDlgItem(hWndFind, IDC_COMBOBOX_EDIT))
    {
      SendMessage(hWndComboboxEdit, EM_LIMITTEXT, 0, 0);
      OldComboboxEdit=(WNDPROC)GetWindowLongW(hWndComboboxEdit, GWL_WNDPROC);
      SetWindowLongW(hWndComboboxEdit, GWL_WNDPROC, (LONG)NewComboboxEditProcW);
    }
    if (bReplaceDlg == TRUE)
    {
      if (hWndComboboxEdit=GetDlgItem(hWndReplace, IDC_COMBOBOX_EDIT))
      {
        SendMessage(hWndComboboxEdit, EM_LIMITTEXT, 0, 0);
        OldComboboxEdit=(WNDPROC)GetWindowLongW(hWndComboboxEdit, GWL_WNDPROC);
        SetWindowLongW(hWndComboboxEdit, GWL_WNDPROC, (LONG)NewComboboxEditProcW);
      }
    }
    if (rcFindAndReplaceDlg.right && rcFindAndReplaceDlg.bottom)
      SetWindowPos(hDlg, 0, rcFindAndReplaceDlg.left, rcFindAndReplaceDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    SendMessage(hDlg, WM_COMMAND, IDC_SETREADONLY, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SEARCH_REPLACE)
      return TRUE;
    else if (LOWORD(wParam) == IDC_SEARCH_MATCHCASE)
    {
      if (SendMessage(hWndMatchCase, BM_GETCHECK, 0, 0)) ftflags|=FR_MATCHCASE;
      else ftflags&=~FR_MATCHCASE;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ESCAPESEQ)
    {
      if (SendMessage(hWndEscapeSeq, BM_GETCHECK, 0, 0)) ftflags|=FR_ESCAPESEQ;
      else ftflags&=~FR_ESCAPESEQ;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_FORWARD)
    {
      ftflags&=~FR_UP&~FR_BEGINNING&~FR_SELECTION&~FR_ALLFILES;
      ftflags|=FR_DOWN;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BACKWARD)
    {
      ftflags&=~FR_DOWN&~FR_BEGINNING&~FR_SELECTION&~FR_ALLFILES;
      ftflags|=FR_UP;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BEGINNING)
    {
      ftflags&=~FR_UP&~FR_SELECTION&~FR_ALLFILES;
      ftflags|=FR_DOWN|FR_BEGINNING;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_INSEL)
    {
      ftflags&=~FR_UP&~FR_BEGINNING&~FR_ALLFILES;
      ftflags|=FR_DOWN|FR_SELECTION;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ALLFILES)
    {
      ftflags&=~FR_UP&~FR_SELECTION;
      ftflags|=FR_DOWN|FR_BEGINNING|FR_ALLFILES;
    }
    else if (LOWORD(wParam) == IDC_SETREADONLY)
    {
      if (bReplaceDlg == TRUE)
      {
        EnableWindow(hWndReplaceButton, !bReadOnly);
        EnableWindow(hWndReplaceAllButton, !bReadOnly);
      }
    }

    if (LOWORD(wParam) == IDC_SEARCH_FIND ||
        LOWORD(wParam) == IDC_SEARCH_FORWARD ||
        LOWORD(wParam) == IDC_SEARCH_BACKWARD ||
        LOWORD(wParam) == IDC_SEARCH_BEGINNING ||
        LOWORD(wParam) == IDC_SEARCH_INSEL ||
        LOWORD(wParam) == IDC_SEARCH_ALLFILES)
    {
      EnableWindow(hWndFindNextButton, TRUE);
      if (bReplaceDlg == TRUE)
      {
        if (!bReadOnly)
        {
          EnableWindow(hWndReplaceButton, TRUE);
          EnableWindow(hWndReplaceAllButton, TRUE);
        }
      }
      if (LOWORD(wParam) != IDC_SEARCH_FIND)
      {
        bSpecialCheck=FALSE;
        SendMessage(hWndBeginning, BM_SETSTATE, FALSE, 0);
        if (bMDI) SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
      }

      if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        i=SendMessageW(hWndFind, CB_GETLBTEXTLEN, (WPARAM)SendMessage(hWndFind, CB_GETCURSEL, 0, 0), 0);
      }
      else
      {
        i=GetWindowTextLengthW(hWndFind);
      }

      if (i == 0)
      {
        EnableWindow(hWndFindNextButton, FALSE);
        if (bReplaceDlg == TRUE)
        {
          EnableWindow(hWndReplaceButton, FALSE);
          EnableWindow(hWndReplaceAllButton, FALSE);
        }
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      if (bSpecialCheck == TRUE || (ftflags & FR_ALLFILES)) ftflags|=FR_BEGINNING;
      if (nSearchStrings) SaveComboboxSearchW(hWndFind, hWndReplace);
      GetWindowPos(hDlg, NULL, &rcFindAndReplaceDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_REPLACE_BUTTON)
    {
      bReplace=TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ALL_BUTTON)
    {
      bReplaceAll=TRUE;
    }
    if (LOWORD(wParam) == IDC_SEARCH_FIND_BUTTON ||
        LOWORD(wParam) == IDC_SEARCH_REPLACE_BUTTON ||
        LOWORD(wParam) == IDC_SEARCH_ALL_BUTTON)
    {
      FreeMemorySearchW();

      if (!GetComboboxSearchTextW(hWndFind, &wszFind_orig, &wszFind))
      {
        FreeMemorySearchW();
        return TRUE;
      }
      if (bReplaceDlg == TRUE)
      {
        if (!GetComboboxSearchTextW(hWndReplace, &wszReplace_orig, &wszReplace))
        {
          FreeMemorySearchW();
          return TRUE;
        }
      }

      hWndFocus=GetFocus();
      if (bReplaceAll)
        bReplaceAllButtonState=EnableWindow(hWndReplaceAllButton, FALSE);

      if (ftflags & FR_ALLFILES)
      {
        HWND hWndTmp=hWndFrameActive;
        int nChanges=0;
        int nChangedFiles=0;

        if (bReplaceDlg == TRUE && bReplaceAll == TRUE)
        {
          ftflags|=FR_BEGINNING;
          if (bSpecialCheck == TRUE)
          {
            bSpecialCheck=FALSE;
            SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
          }

          do
          {
            _ReplaceTextW(hWndEdit, ftflags, wszFind, wszReplace, TRUE, &nReplaceCount);
            if (!hDlgModeless) break;

            if (nReplaceCount)
            {
              ++nChangedFiles;
              nChanges+=nReplaceCount;
            }
            SendMessage(hMdiClient, WM_MDINEXT, (WPARAM)hWndFrameActive, FALSE);
          }
          while (hWndFrameActive != hWndTmp);

          if (!bReplaceAllAndClose)
          {
            API_LoadStringW(hLangLib, MSG_REPLACE_COUNT_ALLFILES, wbuf, BUFFER_SIZE);
            wsprintfW(wbuf2, wbuf, nChangedFiles, nChanges);
            MessageBoxW(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
          }
          else
          {
            PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
            return TRUE;
          }
        }
        else
        {
          if (bSpecialCheck == FALSE)
          {
            bSpecialCheck=TRUE;
            SendMessage(hWndAllFiles, BM_SETSTATE, TRUE, 0);
          }

          do
          {
            if (bReplace == TRUE && bCanReplace == TRUE)
              nResult=_ReplaceTextW(hWndEdit, ftflags, wszFind, wszReplace, FALSE, NULL);
            else
              nResult=_FindTextW(hWndEdit, ftflags, wszFind);

            if (nResult == -1)
            {
              bCanReplace=FALSE;
              ftflags|=FR_BEGINNING;
              SendMessage(hMdiClient, WM_MDINEXT, (WPARAM)hWndFrameActive, FALSE);
            }
            else
            {
              bCanReplace=TRUE;
              ftflags&=~FR_BEGINNING;
              break;
            }
          }
          while (hWndFrameActive != hWndTmp);

          if (nResult == -1)
          {
            bSpecialCheck=FALSE;
            SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
            API_LoadStringW(hLangLib, MSG_SEARCH_ENDED, wbuf, BUFFER_SIZE);
            MessageBoxW(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
          }
        }
      }
      else
      {
        if (bReplaceDlg == TRUE && (bReplace == TRUE || bReplaceAll == TRUE))
          nResult=_ReplaceTextW(hWndEdit, ftflags, wszFind, wszReplace, bReplaceAll, &nReplaceCount);
        else
          nResult=_FindTextW(hWndEdit, ftflags, wszFind);

        if (nResult == -1)
        {
          if (bSpecialCheck == TRUE)
          {
            ftflags|=FR_BEGINNING;
            bSpecialCheck=FALSE;
            SendMessage(hWndBeginning, BM_SETSTATE, FALSE, 0);
          }
          if (bReplaceAll == TRUE)
          {
            if (!bReplaceAllAndClose)
            {
              API_LoadStringW(hLangLib, MSG_REPLACE_COUNT, wbuf, BUFFER_SIZE);
              wsprintfW(wbuf2, wbuf, nReplaceCount);
              MessageBoxW(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
            }
            else
            {
              PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
              return TRUE;
            }
          }
          else
          {
            API_LoadStringW(hLangLib, MSG_SEARCH_ENDED, wbuf, BUFFER_SIZE);
            MessageBoxW(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
          }
        }
        else
        {
          if (ftflags & FR_BEGINNING)
          {
            ftflags&=~FR_BEGINNING;
            bSpecialCheck=TRUE;
            SendMessage(hWndBeginning, BM_SETSTATE, TRUE, 0);
          }
        }
      }
      if (bReplaceAll)
        EnableWindow(hWndReplaceAllButton, !bReplaceAllButtonState);
      SetFocus(hWndFocus);

      return TRUE;
    }
  }
  else if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_F3)
    {
      if (GetWindowTextLengthW(hWndFind))
      {
        FreeMemorySearchW();

        if (!GetComboboxSearchTextW(hWndFind, &wszFind_orig, &wszFind))
        {
          FreeMemorySearchW();
          return TRUE;
        }

        if (GetKeyState(VK_SHIFT) >= 0)
          DoEditFindNextDownW(hWndEdit);
        else
          DoEditFindNextUpW(hWndEdit);

        PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
      }
    }
  }
  return FALSE;
}

LRESULT CALLBACK NewComboboxEditProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_PASTE)
  {
    PasteInEditAsRichEdit(hWnd);
    return TRUE;
  }
  return CallWindowProcA(OldComboboxEdit, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewComboboxEditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_PASTE)
  {
    PasteInEditAsRichEdit(hWnd);
    return TRUE;
  }
  return CallWindowProcW(OldComboboxEdit, hWnd, uMsg, wParam, lParam);
}

void FillComboboxSearchA(HWND hWndFind, HWND hWndReplace)
{
  HWND hWnd;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int i;
  char szRegValue[32];
  char *szData;

  if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Search", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  hWnd=hWndFind;
  lstrcpyA(buf, "find%d");

  Read:
  for (i=0; i < nSearchStrings; ++i)
  {
    wsprintfA(szRegValue, buf, i);

    if (RegQueryValueExA(hKey, szRegValue, NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
    {
      if (szData=(char *)API_HeapAlloc(hHeap, 0, dwSize + 1))
      {
        if (RegQueryValueExA(hKey, szRegValue, NULL, &dwType, (LPBYTE)szData, &dwSize) == ERROR_SUCCESS)
        {
          SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)szData);
          API_HeapFree(hHeap, 0, (LPVOID)szData);
          continue;
        }
      }
    }
    break;
  }
  if (bReplaceDlg && hWnd != hWndReplace)
  {
    hWnd=hWndReplace;
    lstrcpyA(buf, "replace%d");
    goto Read;
  }
  RegCloseKey(hKey);
}

void FillComboboxSearchW(HWND hWndFind, HWND hWndReplace)
{
  HWND hWnd;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int i;
  wchar_t wszRegValue[32];
  wchar_t *wszData;

  if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Search", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  hWnd=hWndFind;
  lstrcpyW(wbuf, L"find%d");

  ReadW:
  for (i=0; i < nSearchStrings; ++i)
  {
    wsprintfW(wszRegValue, wbuf, i);

    if (RegQueryValueExW(hKey, wszRegValue, NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
    {
      if (wszData=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize + 2))
      {
        if (RegQueryValueExW(hKey, wszRegValue, NULL, &dwType, (LPBYTE)wszData, &dwSize) == ERROR_SUCCESS)
        {
          SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)wszData);
          API_HeapFree(hHeap, 0, (LPVOID)wszData);
          continue;
        }
      }
    }
    break;
  }
  if (bReplaceDlg && hWnd != hWndReplace)
  {
    hWnd=hWndReplace;
    lstrcpyW(wbuf, L"replace%d");
    goto ReadW;
  }
  RegCloseKey(hKey);
}

BOOL GetComboboxSearchTextA(HWND hWnd, char **szText_orig, char **szText)
{
  int nIndex;
  int nTextLen;
  int nItemLen;

  nTextLen=GetWindowTextLength(hWnd) + 1;

  if (*szText_orig=*szText=(char *)API_HeapAlloc(hHeap, 0, nTextLen + 1))
  {
    GetWindowTextA(hWnd, *szText_orig, nTextLen);

    if (nSearchStrings)
    {
      if (**szText_orig)
      {
        if ((nIndex=SendMessageA(hWnd, CB_FINDSTRINGEXACT, 0, (LPARAM)*szText_orig)) != CB_ERR)
          SendMessage(hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0);
      }
      else
      {
        for (nIndex=0; (nItemLen=SendMessageA(hWnd, CB_GETLBTEXTLEN, nIndex, 0)) != CB_ERR && nItemLen; ++nIndex);
        if (!nItemLen) SendMessage(hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0);
      }
      SendMessage(hWnd, CB_DELETESTRING, nSearchStrings - 1, 0);
      SendMessageA(hWnd, CB_INSERTSTRING, 0, (LPARAM)*szText_orig);
      SendMessage(hWnd, CB_SETCURSEL, 0, 0);
    }

    if (ftflags & FR_ESCAPESEQ)
    {
      if (*szText=(char *)API_HeapAlloc(hHeap, 0, nTextLen + 1))
      {
        if (!EscapeStringToEscapeDataA(*szText_orig, *szText))
        {
          API_LoadStringA(hLangLib, MSG_ERROR_SYNTAX, buf, BUFFER_SIZE);
          MessageBoxA(GetParent(hWnd), buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
          return FALSE;
        }
      }
      else return FALSE;
    }
  }
  else return FALSE;

  return TRUE;
}

BOOL GetComboboxSearchTextW(HWND hWnd, wchar_t **wszText_orig, wchar_t **wszText)
{
  int nIndex;
  int nTextLen;
  int nItemLen;

  nTextLen=GetWindowTextLengthW(hWnd) + 1;

  if (*wszText_orig=*wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nTextLen * sizeof(wchar_t) + 2))
  {
    GetWindowTextW(hWnd, *wszText_orig, nTextLen);

    if (nSearchStrings)
    {
      if (**wszText_orig)
      {
        if ((nIndex=SendMessageW(hWnd, CB_FINDSTRINGEXACT, 0, (LPARAM)*wszText_orig)) != CB_ERR)
          SendMessage(hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0);
      }
      else
      {
        for (nIndex=0; (nItemLen=SendMessageW(hWnd, CB_GETLBTEXTLEN, nIndex, 0)) != CB_ERR && nItemLen; ++nIndex);
        if (!nItemLen) SendMessage(hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0);
      }
      SendMessage(hWnd, CB_DELETESTRING, nSearchStrings - 1, 0);
      SendMessageW(hWnd, CB_INSERTSTRING, 0, (LPARAM)*wszText_orig);
      SendMessage(hWnd, CB_SETCURSEL, 0, 0);
    }

    if (ftflags & FR_ESCAPESEQ)
    {
      if (*wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nTextLen * sizeof(wchar_t) + 2))
      {
        if (!EscapeStringToEscapeDataW(*wszText_orig, *wszText))
        {
          API_LoadStringW(hLangLib, MSG_ERROR_SYNTAX, wbuf, BUFFER_SIZE);
          MessageBoxW(GetParent(hWnd), wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
          return FALSE;
        }
      }
      else return FALSE;
    }
  }
  else return FALSE;

  return TRUE;
}

void SaveComboboxSearchA(HWND hWndFind, HWND hWndReplace)
{
  HWND hWnd;
  HKEY hKey;
  int nSize;
  int i;
  char szRegValue[32];
  char *szData;

  if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Search", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return;

  hWnd=hWndFind;
  lstrcpyA(buf, "find%d");

  Save:
  for (i=0; i < nSearchStrings; ++i)
  {
    wsprintfA(szRegValue, buf, i);

    if ((nSize=SendMessageA(hWnd, CB_GETLBTEXTLEN, (WPARAM)i, 0)) != CB_ERR)
    {
      ++nSize;

      if (szData=(char *)API_HeapAlloc(hHeap, 0, nSize + 1))
      {
        SendMessageA(hWnd, CB_GETLBTEXT, (WPARAM)i, (LPARAM)szData);
        RegSetValueExA(hKey, szRegValue, 0, REG_SZ, (LPBYTE)szData, nSize);
        API_HeapFree(hHeap, 0, (LPVOID)szData);
        continue;
      }
    }
    break;
  }
  if (bReplaceDlg && hWnd != hWndReplace)
  {
    hWnd=hWndReplace;
    lstrcpyA(buf, "replace%d");
    goto Save;
  }
  RegCloseKey(hKey);
}

void SaveComboboxSearchW(HWND hWndFind, HWND hWndReplace)
{
  HWND hWnd;
  HKEY hKey;
  int nSize;
  int i;
  wchar_t wszRegValue[32];
  wchar_t *wszData;

  if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Search", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return;

  hWnd=hWndFind;
  lstrcpyW(wbuf, L"find%d");

  SaveW:
  for (i=0; i < nSearchStrings; ++i)
  {
    wsprintfW(wszRegValue, wbuf, i);

    if ((nSize=SendMessageW(hWnd, CB_GETLBTEXTLEN, (WPARAM)i, 0)) != CB_ERR)
    {
      ++nSize;

      if (wszData=(wchar_t *)API_HeapAlloc(hHeap, 0, nSize * sizeof(wchar_t) + 2))
      {
        SendMessageW(hWnd, CB_GETLBTEXT, (WPARAM)i, (LPARAM)wszData);
        RegSetValueExW(hKey, wszRegValue, 0, REG_SZ, (LPBYTE)wszData, nSize * sizeof(wchar_t));
        API_HeapFree(hHeap, 0, (LPVOID)wszData);
        continue;
      }
    }
    break;
  }
  if (bReplaceDlg && hWnd != hWndReplace)
  {
    hWnd=hWndReplace;
    lstrcpyW(wbuf, L"replace%d");
    goto SaveW;
  }
  RegCloseKey(hKey);
}

int _FindTextA(HWND hWnd, DWORD dwFlags, char *pFindIt)
{
  FINDTEXTEXA ftA;
  int nResult;
  int nLayout;

  if (dwFlags & FR_SELECTION)
  {
    ftA.chrg.cpMin=chrg.cpMin;
    ftA.chrg.cpMax=chrg.cpMax;
  }
  else if (dwFlags & FR_BEGINNING)
  {
    ftA.chrg.cpMin=0;
    ftA.chrg.cpMax=-1;
  }
  else if (dwFlags & FR_DOWN)
  {
    ftA.chrg.cpMin=chrg.cpMax;
    ftA.chrg.cpMax=-1;
  }
  else if (dwFlags & FR_UP)
  {
    ftA.chrg.cpMin=chrg.cpMin;
    ftA.chrg.cpMax=0;
  }
  else return -1;

  nLayout=_SetLayout(hWnd);
  ftA.lpstrText=pFindIt;

  if ((nResult=SendMessageA(hWnd, EM_FINDTEXTEX, dwFlags, (LPARAM)&ftA)) != -1)
  {
    //EM_FINDTEXTEX bug workarround
    if (ftA.chrgText.cpMax - ftA.chrgText.cpMin == 1)
    {
      TEXTRANGEA txtrngA;
      char szChar[4];

      txtrngA.chrg.cpMin=ftA.chrgText.cpMin;
      txtrngA.chrg.cpMax=ftA.chrgText.cpMax;
      txtrngA.lpstrText=szChar;
      SendMessageA(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&txtrngA);
      if (*szChar == '\0') nResult=-1;
    }
    if (nResult != -1) SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&ftA.chrgText);
  }
  RestoreLayout(hWnd, nLayout);
  if (nResult == -1) SendMessage(hMainWnd, AKDN_SEARCH_ENDED, (WPARAM)hDlgModeless, 0);
  return nResult;
}

int _FindTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt)
{
  FINDTEXTEXW ftW;
  int nResult;

  if (dwFlags & FR_SELECTION)
  {
    ftW.chrg.cpMin=chrg.cpMin;
    ftW.chrg.cpMax=chrg.cpMax;
  }
  else if (dwFlags & FR_BEGINNING)
  {
    ftW.chrg.cpMin=0;
    ftW.chrg.cpMax=-1;
  }
  else if (dwFlags & FR_DOWN)
  {
    ftW.chrg.cpMin=chrg.cpMax;
    ftW.chrg.cpMax=-1;
  }
  else if (dwFlags & FR_UP)
  {
    ftW.chrg.cpMin=chrg.cpMin;
    ftW.chrg.cpMax=0;
  }
  else return -1;

  ftW.lpstrText=wpFindIt;

  if ((nResult=SendMessageW(hWnd, EM_FINDTEXTEX, dwFlags, (LPARAM)&ftW)) != -1)
  {
    //EM_FINDTEXTEX bug workarround
    if (ftW.chrgText.cpMax - ftW.chrgText.cpMin == 1)
    {
      TEXTRANGEW txtrngW;
      wchar_t wszChar[4];

      txtrngW.chrg.cpMin=ftW.chrgText.cpMin;
      txtrngW.chrg.cpMax=ftW.chrgText.cpMax;
      txtrngW.lpstrText=wszChar;
      SendMessageW(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&txtrngW);
      if (*wszChar == '\0') nResult=-1;
    }
    if (nResult != -1) SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&ftW.chrgText);
  }
  if (nResult == -1) SendMessage(hMainWnd, AKDN_SEARCH_ENDED, (WPARAM)hDlgModeless, 0);
  return nResult;
}

int _ReplaceTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, char *pReplaceWith, BOOL bAll, int *nReplaceCount)
{
  RECT rcEdit;
  CHARRANGE crInitial;
  CHARRANGE crRange;
  char *szText;
  char *szReplaceText;
  char *pMin;
  char *pMax;
  char *pFirstVisible;
  int nFirstVisibleChar;
  int nReplaceTextLen;
  int nResult=-1;
  int nChanges=0;

  if (bAll)
  {
    if (dwFlags & FR_SELECTION)
    {
      crRange.cpMin=chrg.cpMin;
      crRange.cpMax=chrg.cpMax;
    }
    else if (dwFlags & FR_BEGINNING)
    {
      crRange.cpMin=0;
      crRange.cpMax=-1;
    }
    else if (dwFlags & FR_DOWN)
    {
      crRange.cpMin=chrg.cpMin;
      crRange.cpMax=-1;
    }
    else if (dwFlags & FR_UP)
    {
      crRange.cpMin=0;
      crRange.cpMax=chrg.cpMax;
    }
    else return -1;

    if (GetRangeTextA(hWnd, crRange.cpMin, crRange.cpMax, &szText))
    {
      if (_StrReplaceA(szText, pFindIt, pReplaceWith, (dwFlags & FR_MATCHCASE)?TRUE:FALSE, NULL, &nReplaceTextLen, NULL, NULL, NULL))
      {
        if (szReplaceText=(char *)API_HeapAlloc(hHeap, 0, nReplaceTextLen + 1))
        {
          if ((dwFlags & FR_BEGINNING) || (dwFlags & FR_UP))
          {
            pMin=szText + chrg.cpMin;
            pMax=szText + chrg.cpMax;
          }
          else if ((dwFlags & FR_SELECTION) || (dwFlags & FR_DOWN))
          {
            pMin=szText;
            pMax=szText + (chrg.cpMax - chrg.cpMin);
          }
          crInitial.cpMin=chrg.cpMin;
          crInitial.cpMax=chrg.cpMax;

          SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
          if (nFirstVisibleChar < crRange.cpMin)
            pFirstVisible=NULL;
          else
            pFirstVisible=szText + (nFirstVisibleChar - crRange.cpMin);

          if (nChanges=_StrReplaceA(szText, pFindIt, pReplaceWith, (dwFlags & FR_MATCHCASE)?TRUE:FALSE, szReplaceText, NULL, &pMin, &pMax, &pFirstVisible))
          {
            SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
            SetTextSel(hWnd, crRange.cpMin, crRange.cpMax);
            ReplaceSelA(hWnd, szReplaceText, -1);

            if ((dwFlags & FR_BEGINNING) || (dwFlags & FR_UP))
              SetTextSel(hWnd, (pMin - szText), (pMax - szText));
            else if ((dwFlags & FR_SELECTION) || (dwFlags & FR_DOWN))
              SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin + (pMax - szText));

            if (pFirstVisible) nFirstVisibleChar=crRange.cpMin + (pFirstVisible - szText);
            RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
            SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
            InvalidateRect(hWnd, NULL, FALSE);
          }
          API_HeapFree(hHeap, 0, (LPVOID)szReplaceText);
        }
      }
      FreeText(szText);
    }
  }
  else
  {
    if (GetRangeTextA(hWnd, chrg.cpMin, chrg.cpMax, &szText))
    {
      if (((dwFlags & FR_MATCHCASE) && !lstrcmpA(pFindIt, szText)) ||
          (!(dwFlags & FR_MATCHCASE) && !lstrcmpiA(pFindIt, szText)))
      {
        ReplaceSelA(hWnd, pReplaceWith, -1);
        nChanges=1;
      }
      FreeText(szText);
    }
    nResult=_FindTextA(hWnd, dwFlags, pFindIt);
  }
  if (nReplaceCount) *nReplaceCount=nChanges;
  return nResult;
}

int _ReplaceTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, wchar_t *wpReplaceWith, BOOL bAll, int *nReplaceCount)
{
  RECT rcEdit;
  CHARRANGE crInitial;
  CHARRANGE crRange;
  wchar_t *wszText;
  wchar_t *wszReplaceText;
  wchar_t *wpMin;
  wchar_t *wpMax;
  wchar_t *wpFirstVisible;
  int nFirstVisibleChar;
  int nReplaceTextLen;
  int nResult=-1;
  int nChanges=0;

  if (bAll)
  {
    if (dwFlags & FR_SELECTION)
    {
      crRange.cpMin=chrg.cpMin;
      crRange.cpMax=chrg.cpMax;
    }
    else if (dwFlags & FR_BEGINNING)
    {
      crRange.cpMin=0;
      crRange.cpMax=-1;
    }
    else if (dwFlags & FR_DOWN)
    {
      crRange.cpMin=chrg.cpMin;
      crRange.cpMax=-1;
    }
    else if (dwFlags & FR_UP)
    {
      crRange.cpMin=0;
      crRange.cpMax=chrg.cpMax;
    }
    else return -1;

    if (GetRangeTextW(hWnd, crRange.cpMin, crRange.cpMax, &wszText))
    {
      if (_StrReplaceW(wszText, wpFindIt, wpReplaceWith, (dwFlags & FR_MATCHCASE)?TRUE:FALSE, NULL, &nReplaceTextLen, NULL, NULL, NULL))
      {
        if (wszReplaceText=(wchar_t *)API_HeapAlloc(hHeap, 0, nReplaceTextLen * sizeof(wchar_t) + 2))
        {
          if ((dwFlags & FR_BEGINNING) || (dwFlags & FR_UP))
          {
            wpMin=wszText + chrg.cpMin;
            wpMax=wszText + chrg.cpMax;
          }
          else if ((dwFlags & FR_SELECTION) || (dwFlags & FR_DOWN))
          {
            wpMin=wszText;
            wpMax=wszText + (chrg.cpMax - chrg.cpMin);
          }
          crInitial.cpMin=chrg.cpMin;
          crInitial.cpMax=chrg.cpMax;

          SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
          if (nFirstVisibleChar < crRange.cpMin)
            wpFirstVisible=NULL;
          else
            wpFirstVisible=wszText + (nFirstVisibleChar - crRange.cpMin);

          if (nChanges=_StrReplaceW(wszText, wpFindIt, wpReplaceWith, (dwFlags & FR_MATCHCASE)?TRUE:FALSE, wszReplaceText, NULL, &wpMin, &wpMax, &wpFirstVisible))
          {
            SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
            SetTextSel(hWnd, crRange.cpMin, crRange.cpMax);
            ReplaceSelW(hWnd, wszReplaceText, -1);

            if ((dwFlags & FR_BEGINNING) || (dwFlags & FR_UP))
              SetTextSel(hWnd, (wpMin - wszText), (wpMax - wszText));
            else if ((dwFlags & FR_SELECTION) || (dwFlags & FR_DOWN))
              SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin + (wpMax - wszText));

            if (wpFirstVisible) nFirstVisibleChar=crRange.cpMin + (wpFirstVisible - wszText);
            RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
            SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
            InvalidateRect(hWnd, NULL, FALSE);
          }
          API_HeapFree(hHeap, 0, (LPVOID)wszReplaceText);
        }
      }
      FreeText(wszText);
    }
  }
  else
  {
    if (GetRangeTextW(hWnd, chrg.cpMin, chrg.cpMax, &wszText))
    {
      if (((dwFlags & FR_MATCHCASE) && !lstrcmpW(wpFindIt, wszText)) ||
          (!(dwFlags & FR_MATCHCASE) && !lstrcmpiW(wpFindIt, wszText)))
      {
        ReplaceSelW(hWnd, wpReplaceWith, -1);
        nChanges=1;
      }
      FreeText(wszText);
    }
    nResult=_FindTextW(hWnd, dwFlags, wpFindIt);
  }
  if (nReplaceCount) *nReplaceCount=nChanges;
  return nResult;
}

int _StrReplaceA(char *pText, char *pIt, char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult, char **ppMin, char **ppMax, char **ppFirstVisible)
{
  char *pTextCount;
  char *pItCount;
  char *pResult=szResult;
  char *pMin=NULL;
  char *pMax=NULL;
  char *pFirstVisible=NULL;
  int nItLen=lstrlenA(pIt);
  int nWithLen=lstrlenA(pWith);
  int nChanges=0;
  int nDiff=nItLen - nWithLen;
  int i;

  if (ppMin) pMin=*ppMin;
  if (ppMax) pMax=*ppMax;
  if (ppFirstVisible) pFirstVisible=*ppFirstVisible;

  for (pTextCount=pText; *pText; ++pText)
  {
    for (pTextCount=pText, pItCount=pIt;
         (bSensitive == TRUE && *pTextCount == *pItCount) ||
         (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pTextCount) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pItCount));
         ++pTextCount)
    {
      if (!*++pItCount)
      {
        if (szResult)
        {
          if (ppMin && *ppMin)
          {
            if (*ppMin > pTextCount) pMin-=nDiff;
            else if (*ppMin > pText && *ppMin <= pTextCount) pMin=pMin - (*ppMin - pText);
          }
          if (ppMax && *ppMax)
          {
            if (*ppMax > pTextCount) pMax-=nDiff;
            else if (*ppMax > pText && *ppMax <= pTextCount) pMax=pMax - (*ppMax - pText) + nWithLen;
          }
          if (ppFirstVisible && *ppFirstVisible)
          {
            if (*ppFirstVisible > pTextCount) pFirstVisible-=nDiff;
            else if (*ppFirstVisible > pText && *ppFirstVisible <= pTextCount) pFirstVisible=pFirstVisible - (*ppFirstVisible - pText);
          }
          for (i=0; i < nWithLen; ++i)
            *pResult++=pWith[i];
        }
        else pResult+=nWithLen;

        pText=pTextCount + 1;
        pItCount=pIt;
        ++nChanges;
        if (!*pText) goto End;
      }
    }
    if (szResult) *pResult=*pText;
    ++pResult;
  }

  End:
  if (szResult) *pResult=*pText;
  if (nMaxResult) *nMaxResult=(pResult - szResult) + 1;
  if (ppMin && ppMax && ppFirstVisible)
  {
    if (*ppMax) *ppMax=(*ppMin == *ppMax)?pMin:pMax;
    if (*ppMin) *ppMin=pMin;
    if (*ppFirstVisible) *ppFirstVisible=pFirstVisible;
  }
  return nChanges;
}

int _StrReplaceW(wchar_t *wpText, wchar_t *wpIt, wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult, wchar_t **wppMin, wchar_t **wppMax, wchar_t **wppFirstVisible)
{
  wchar_t *wpTextCount;
  wchar_t *wpItCount;
  wchar_t *wpResult=wszResult;
  wchar_t *wpMin=NULL;
  wchar_t *wpMax=NULL;
  wchar_t *wpFirstVisible=NULL;
  int nItLen=lstrlenW(wpIt);
  int nWithLen=lstrlenW(wpWith);
  int nChanges=0;
  int nDiff=nItLen - nWithLen;
  int i;

  if (wppMin) wpMin=*wppMin;
  if (wppMax) wpMax=*wppMax;
  if (wppFirstVisible) wpFirstVisible=*wppFirstVisible;

  for (wpTextCount=wpText; *wpText; ++wpText)
  {
    for (wpTextCount=wpText, wpItCount=wpIt;
         (bSensitive == TRUE && *wpTextCount == *wpItCount) ||
         (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpTextCount) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpItCount));
         ++wpTextCount)
    {
      if (!*++wpItCount)
      {
        if (wszResult)
        {
          if (wppMin && *wppMin)
          {
            if (*wppMin > wpTextCount) wpMin-=nDiff;
            else if (*wppMin > wpText && *wppMin <= wpTextCount) wpMin=wpMin - (*wppMin - wpText);
          }
          if (wppMax && *wppMax)
          {
            if (*wppMax > wpTextCount) wpMax-=nDiff;
            else if (*wppMax > wpText && *wppMax <= wpTextCount) wpMax=wpMax - (*wppMax - wpText) + nWithLen;
          }
          if (wppFirstVisible && *wppFirstVisible)
          {
            if (*wppFirstVisible > wpTextCount) wpFirstVisible-=nDiff;
            else if (*wppFirstVisible > wpText && *wppFirstVisible <= wpTextCount) wpFirstVisible=wpFirstVisible - (*wppFirstVisible - wpText);
          }
          for (i=0; i < nWithLen; ++i)
            *wpResult++=wpWith[i];
        }
        else wpResult+=nWithLen;

        wpText=wpTextCount + 1;
        wpItCount=wpIt;
        ++nChanges;
        if (!*wpText) goto End;
      }
    }
    if (wszResult) *wpResult=*wpText;
    ++wpResult;
  }

  End:
  if (wszResult) *wpResult=*wpText;
  if (nMaxResult) *nMaxResult=(wpResult - wszResult) + 1;
  if (wppMin && wppMax && wppFirstVisible)
  {
    if (*wppMax) *wppMax=(*wppMin == *wppMax)?wpMin:wpMax;
    if (*wppMin) *wppMin=wpMin;
    if (*wppFirstVisible) *wppFirstVisible=wpFirstVisible;
  }
  return nChanges;
}

BOOL EscapeStringToEscapeDataA(char *pInput, char *szOutput)
{
  char *a=pInput;
  char *b=szOutput;
  char hex[3];
  int nDec;

  for (hex[2]='\0'; *a; ++a, ++b)
  {
    if (*a == '\\')
    {
      if (*++a == '\\') *b='\\';
      else if (*a == 'n') *b='\r';
      else if (*a == 't') *b='\t';
      else if (*a == '[')
      {
        while (*++a == ' ');

        do
        {
          hex[0]=*a;
          if (*a == 0) goto Error;
          hex[1]=*++a;
          if (*a == 0) goto Error;
          *b=nDec=hex2decA(hex);
          if (nDec == -1) goto Error;
          while (*++a == ' ');
        }
        while (*a && *a != ']' && ++b);

        if (!*a) goto Error;
      }
      else goto Error;
    }
    else *b=*a;
  }
  *b='\0';
  return TRUE;

  Error:
  *szOutput='\0';
  return FALSE;
}

BOOL EscapeStringToEscapeDataW(wchar_t *wpInput, wchar_t *wszOutput)
{
  wchar_t *a=wpInput;
  wchar_t *b=wszOutput;
  wchar_t whex[5];
  int nDec;

  for (whex[4]='\0'; *a; ++a, ++b)
  {
    if (*a == '\\')
    {
      if (*++a == '\\') *b='\\';
      else if (*a == 'n') *b='\r';
      else if (*a == 't') *b='\t';
      else if (*a == '[')
      {
        while (*++a == ' ');

        do
        {
          whex[0]=*a;
          if (*a == 0) goto Error;
          whex[1]=*++a;
          if (*a == 0) goto Error;
          whex[2]=*++a;
          if (*a == 0) goto Error;
          whex[3]=*++a;
          if (*a == 0) goto Error;
          *b=nDec=hex2decW(whex);
          if (nDec == -1) goto Error;
          while (*++a == ' ');
        }
        while (*a && *a != ']' && ++b);

        if (!*a) goto Error;
      }
      else goto Error;
    }
    else *b=*a;
  }
  *b='\0';
  return TRUE;

  Error:
  *wszOutput='\0';
  return FALSE;
}

void EscapeDataToEscapeStringW(wchar_t *wpInput, wchar_t *wszOutput)
{
  wchar_t *a=wpInput;
  wchar_t *b=wszOutput;

  for (; *a; ++b, ++a)
  {
    if (*a == '\t') *b='\\', *++b='t';
    else if (*a == '\r') *b='\\', *++b='n';
    else if (*a == '\\') *b='\\', *++b='\\';
    else *b=*a;
  }
  *b='\0';
}


//// Paste operation

void ReplaceSelA(HWND hWnd, char *pData, int nDataLen)
{
  wchar_t *wpData;
  int nUnicodeLen;

  nUnicodeLen=MultiByteToWideChar(CP_ACP, 0, pData, nDataLen, NULL, 0);

  if (wpData=(wchar_t *)API_HeapAlloc(hHeap, 0, nUnicodeLen * sizeof(wchar_t)))
  {
    MultiByteToWideChar(CP_ACP, 0, pData, nDataLen, wpData, nUnicodeLen);
    ReplaceSelW(hWnd, wpData, nDataLen);
    API_HeapFree(hHeap, 0, (LPVOID)wpData);
  }
}

void ReplaceSelW(HWND hWnd, wchar_t *wpData, int nDataLen)
{
  EDITSTREAM es;
  BUFFERSTREAMDATA bsd;

  bsd.wpData=wpData;
  bsd.nDataLen=nDataLen;
  bsd.nCount=0;
  es.dwCookie=(DWORD)&bsd;
  es.pfnCallback=InputStreamCallback;
  SendMessage(hWnd, EM_STREAMIN, SF_TEXT|SF_UNICODE|SFF_SELECTION, (LPARAM)&es);
}

wchar_t* GetSelTextW(HWND hWnd, int *nTextLen)
{
  EDITSTREAM es;
  BUFFERSTREAMDATA bsd;
  CHARRANGE cr;
  wchar_t *wszText;
  int nLen;

  SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);

  if (cr.cpMin < cr.cpMax)
  {
    nLen=(cr.cpMax - cr.cpMin) + 1;

    if (wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nLen * sizeof(wchar_t) + 2))
    {
      bsd.wpData=wszText;
      bsd.nDataLen=-1;
      bsd.nCount=0;
      es.dwCookie=(DWORD)&bsd;
      es.pfnCallback=GetSelTextCallbackW;
      SendMessage(hWnd, EM_STREAMOUT, SF_TEXT|SF_UNICODE|SFF_SELECTION, (LPARAM)&es);
      wszText[bsd.nCount]='\0';
      if (nTextLen) *nTextLen=bsd.nCount;
      return wszText;
    }
  }
  if (nTextLen) *nTextLen=0;
  return NULL;
}

DWORD CALLBACK GetSelTextCallbackW(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  BUFFERSTREAMDATA *lpData=(BUFFERSTREAMDATA *)dwCookie;
  wchar_t *wpSrc=(wchar_t *)pbBuff;
  wchar_t *wpDest=lpData->wpData;
  int nSrcLen=cb / sizeof(wchar_t);
  int i;

  for (i=0; i < nSrcLen; ++i)
  {
    if (wpSrc[i] != '\n')
      wpDest[lpData->nCount++]=wpSrc[i];
  }
  *pcb=cb;

  return 0;
}

void SetTextSel(HWND hWnd, int nMin, int nMax)
{
  CHARRANGE cr;

  cr.cpMin=nMin;
  cr.cpMax=nMax;
  SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
}

int GetTextLength(HWND hWnd)
{
  int nLastLineIndex;
  int nLastLineLen;

  nLastLineIndex=SendMessage(hWnd, EM_LINEINDEX, SendMessage(hWnd, EM_GETLINECOUNT, 0, 0) - 1, 0);
  nLastLineLen=SendMessage(hWnd, EM_LINELENGTH, nLastLineIndex, 0);
  return nLastLineIndex + nLastLineLen;
}

int GetRangeTextA(HWND hWnd, int nMin, int nMax, char **pText)
{
  TEXTRANGEA txtrngA;
  int nLen;
  int nLayout;

  if (nMax == -1)
  {
    nMax=GetTextLength(hWnd);
  }
  if (nMin < nMax)
  {
    nLen=(nMax - nMin);

    if (*pText=(char *)API_HeapAlloc(hHeap, 0, nLen + 1))
    {
      nLayout=_SetLayout(hWnd);
      txtrngA.chrg.cpMin=nMin;
      txtrngA.chrg.cpMax=nMax;
      txtrngA.lpstrText=*pText;
      SendMessageA(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&txtrngA);
      RestoreLayout(hWnd, nLayout);

      return nLen;
    }
  }
  *pText=NULL;
  return 0;
}

int GetRangeTextW(HWND hWnd, int nMin, int nMax, wchar_t **wpText)
{
  TEXTRANGEW txtrngW;
  int nLen;

  if (nMax == -1)
  {
    nMax=GetTextLength(hWnd);
  }
  if (nMin < nMax)
  {
    nLen=(nMax - nMin);

    if (*wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, nLen * sizeof(wchar_t) + 2))
    {
      txtrngW.chrg.cpMin=nMin;
      txtrngW.chrg.cpMax=nMax;
      txtrngW.lpstrText=*wpText;
      SendMessageW(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&txtrngW);

      return nLen;
    }
  }
  *wpText=NULL;
  return 0;
}

BOOL FreeText(LPVOID pText)
{
  if (pText) return API_HeapFree(hHeap, 0, pText);
  return FALSE;
}

BOOL PasteInEditAsRichEdit(HWND hWnd)
{
  HGLOBAL hData;
  LPVOID pData;
  BOOL bResult=FALSE;

  if (OpenClipboard(hWnd))
  {
    if (!bOldWindows && (hData=GetClipboardData(CF_UNICODETEXT)))
    {
      if (pData=GlobalLock(hData))
      {
        //Convert \r\r\n->\r, \r\n->\r, \n->\r
        wchar_t *wpSource=(wchar_t *)pData;
        wchar_t *wpTarget;
        wchar_t *wpSourceCount;
        wchar_t *wpTargetCount;
        int nTargetLen;

        nTargetLen=wcslen(wpSource);

        if (wpTarget=(wchar_t *)API_HeapAlloc(hHeap, 0, nTargetLen * sizeof(wchar_t) + 2))
        {
          for (wpTargetCount=wpTarget, wpSourceCount=wpSource; *wpSourceCount; ++wpSourceCount, ++wpTargetCount)
          {
            if (*wpSourceCount == '\r')
            {
              *wpTargetCount='\r';

              if (*(wpSourceCount + 1) == '\r' &&
                  *(wpSourceCount + 2) == '\n')
              {
                wpSourceCount+=2;
              }
              else if (*(wpSourceCount + 1) == '\n')
              {
                wpSourceCount+=1;
              }
            }
            else if (*wpSourceCount == '\n')
            {
              *wpTargetCount='\r';
            }
            else *wpTargetCount=*wpSourceCount;
          }
          *wpTargetCount='\0';

          SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)wpTarget);
          bResult=TRUE;
          API_HeapFree(hHeap, 0, (LPVOID)wpTarget);
        }
        GlobalUnlock(hData);
      }
    }
    else if (hData=GetClipboardData(CF_TEXT))
    {
      if (pData=GlobalLock(hData))
      {
        //Convert \r\r\n->\r, \r\n->\r, \n->\r
        char *pSource=(char *)pData;
        char *pTarget;
        char *pSourceCount;
        char *pTargetCount;
        int nTargetLen;

        nTargetLen=lstrlenA(pSource);

        if (pTarget=(char *)API_HeapAlloc(hHeap, 0, nTargetLen + 1))
        {
          for (pTargetCount=pTarget, pSourceCount=pSource; *pSourceCount; ++pSourceCount, ++pTargetCount)
          {
            if (*pSourceCount == '\r')
            {
              *pTargetCount='\r';

              if (*(pSourceCount + 1) == '\r' &&
                  *(pSourceCount + 2) == '\n')
              {
                pSourceCount+=2;
              }
              else if (*(pSourceCount + 1) == '\n')
              {
                pSourceCount+=1;
              }
            }
            else if (*pSourceCount == '\n')
            {
              *pTargetCount='\r';
            }
            else *pTargetCount=*pSourceCount;
          }
          *pTargetCount='\0';

          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)pTarget);
          bResult=TRUE;
          API_HeapFree(hHeap, 0, (LPVOID)pTarget);
        }
        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
  return bResult;
}

BOOL PasteAfter(HWND hWnd, BOOL bAnsi)
{
  CHARRANGE cr;

  SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
  if (DoEditPaste(hWnd, bAnsi))
  {
    SendMessage(hWnd, EM_SETSEL, cr.cpMin, cr.cpMin);
    return TRUE;
  }
  return FALSE;
}


//// Go to line

BOOL CALLBACK GoToLineDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndNumber;
  static HWND hWndLine;
  static HWND hWndOffset;
  int nNumber=0;
  int nLineCount=0;
  int a;
  int b;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndNumber=GetDlgItem(hDlg, IDC_GOTOLINE_NUMBER);
    hWndLine=GetDlgItem(hDlg, IDC_GOTOLINE_LINE);
    hWndOffset=GetDlgItem(hDlg, IDC_GOTOLINE_OFFSET);

    if (rcGotoLineDlg.right && rcGotoLineDlg.bottom)
      SetWindowPos(hDlg, 0, rcGotoLineDlg.left, rcGotoLineDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    if (nGotoType == NT_LINE)
    {
      SendMessage(hWndLine, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hDlg, WM_COMMAND, IDC_GOTOLINE_LINE, 0);
    }
    else if (nGotoType == NT_OFFSET)
    {
      SendMessage(hWndOffset, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hDlg, WM_COMMAND, IDC_GOTOLINE_OFFSET, 0);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_GOTOLINE_LINE ||
        LOWORD(wParam) == IDC_GOTOLINE_OFFSET)
    {
      if (LOWORD(wParam) == IDC_GOTOLINE_LINE)
        nGotoType=NT_LINE;
      else if (LOWORD(wParam) == IDC_GOTOLINE_OFFSET)
        nGotoType=NT_OFFSET;

      if (nGotoType == NT_LINE)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          a=SendMessage(hWndEdit, EM_EXLINEFROMCHAR, 0, nPosition);
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, a + 1, FALSE);
          SendMessage(hWndNumber, EM_SETSEL, 0, -1);
        }
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, nPosition, FALSE);
          SendMessage(hWndNumber, EM_SETSEL, 0, -1);
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (GetDlgItemTextA(hDlg, IDC_GOTOLINE_NUMBER, buf, BUFFER_SIZE))
      {
        //Only numeral
        for (a=0, b=0; buf[a]; ++a)
        {
          if (buf[a] >= '0' && buf[a] <= '9')
            buf2[b++]=buf[a];
          else if (buf[a] == '-' && b == 0)
            buf2[b++]=buf[a];
        }
        buf2[b]='\0';

        nNumber=xatoiA(buf2);
      }

      if (nGotoType == NT_LINE)
      {
        nLineCount=SendMessage(hWndEdit, EM_GETLINECOUNT, 0, 0);

        if (!nNumber)
        {
          API_LoadStringA(hLangLib, MSG_WRONG_STRING, buf, BUFFER_SIZE);
          MessageBoxA(hDlg, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
          return FALSE;
        }
        else if (nNumber < 0)
        {
          nNumber=nLineCount + nNumber + 1;
          if (nNumber <= 0) nNumber=1;
        }
        nNumber=min(nNumber, nLineCount);

        a=SendMessage(hWndEdit, EM_LINEINDEX, nNumber - 1, 0);
        SetTextSel(hWndEdit, a, a);
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (nNumber >= 0)
        {
          SetTextSel(hWndEdit, nNumber, nNumber);
        }
        else
        {
          a=GetTextLength(hWndEdit);
          SetTextSel(hWndEdit, a + nNumber + 1, a + nNumber + 1);
        }
      }
    }
    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDCANCEL)
    {
      GetWindowPos(hDlg, NULL, &rcGotoLineDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK GoToLineDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndNumber;
  static HWND hWndLine;
  static HWND hWndOffset;
  int nNumber=0;
  int nLineCount=0;
  int a;
  int b;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndNumber=GetDlgItem(hDlg, IDC_GOTOLINE_NUMBER);
    hWndLine=GetDlgItem(hDlg, IDC_GOTOLINE_LINE);
    hWndOffset=GetDlgItem(hDlg, IDC_GOTOLINE_OFFSET);

    if (rcGotoLineDlg.right && rcGotoLineDlg.bottom)
      SetWindowPos(hDlg, 0, rcGotoLineDlg.left, rcGotoLineDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    if (nGotoType == NT_LINE)
    {
      SendMessage(hWndLine, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hDlg, WM_COMMAND, IDC_GOTOLINE_LINE, 0);
    }
    else if (nGotoType == NT_OFFSET)
    {
      SendMessage(hWndOffset, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hDlg, WM_COMMAND, IDC_GOTOLINE_OFFSET, 0);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_GOTOLINE_LINE ||
        LOWORD(wParam) == IDC_GOTOLINE_OFFSET)
    {
      if (LOWORD(wParam) == IDC_GOTOLINE_LINE)
        nGotoType=NT_LINE;
      else if (LOWORD(wParam) == IDC_GOTOLINE_OFFSET)
        nGotoType=NT_OFFSET;

      if (nGotoType == NT_LINE)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          a=SendMessage(hWndEdit, EM_EXLINEFROMCHAR, 0, nPosition);
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, a + 1, FALSE);
          SendMessage(hWndNumber, EM_SETSEL, 0, -1);
        }
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, nPosition, FALSE);
          SendMessage(hWndNumber, EM_SETSEL, 0, -1);
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (GetDlgItemTextW(hDlg, IDC_GOTOLINE_NUMBER, wbuf, BUFFER_SIZE))
      {
        //Only numeral
        for (a=0, b=0; wbuf[a]; ++a)
        {
          if (wbuf[a] >= '0' && wbuf[a] <= '9')
            wbuf2[b++]=wbuf[a];
          else if (wbuf[a] == '-' && b == 0)
            wbuf2[b++]=wbuf[a];
        }
        wbuf2[b]='\0';

        nNumber=xatoiW(wbuf2);
      }

      if (nGotoType == NT_LINE)
      {
        nLineCount=SendMessage(hWndEdit, EM_GETLINECOUNT, 0, 0);

        if (!nNumber)
        {
          API_LoadStringW(hLangLib, MSG_WRONG_STRING, wbuf, BUFFER_SIZE);
          MessageBoxW(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
          return FALSE;
        }
        else if (nNumber < 0)
        {
          nNumber=nLineCount + nNumber + 1;
          if (nNumber <= 0) nNumber=1;
        }
        nNumber=min(nNumber, nLineCount);

        a=SendMessage(hWndEdit, EM_LINEINDEX, nNumber - 1, 0);
        SetTextSel(hWndEdit, a, a);
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (nNumber >= 0)
        {
          SetTextSel(hWndEdit, nNumber, nNumber);
        }
        else
        {
          a=GetTextLength(hWndEdit);
          SetTextSel(hWndEdit, a + nNumber + 1, a + nNumber + 1);
        }
      }
    }
    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDCANCEL)
    {
      GetWindowPos(hDlg, NULL, &rcGotoLineDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      return TRUE;
    }
  }
  return FALSE;
}


//// Recent files

BOOL RecentFilesAllocA()
{
  if (!(lpszRecentNames=(char (*)[MAX_PATH])API_HeapAlloc(hHeap, 0, nRecentFiles * MAX_PATH)))
    return FALSE;
  if (!(lpdwRecentPositions=(DWORD *)API_HeapAlloc(hHeap, 0, nRecentFiles * sizeof(DWORD))))
    return FALSE;
  if (!(lpdwRecentCodepages=(DWORD *)API_HeapAlloc(hHeap, 0, nRecentFiles * sizeof(DWORD))))
    return FALSE;
  return TRUE;
}

BOOL RecentFilesAllocW()
{
  if (!(lpwszRecentNames=(wchar_t (*)[MAX_PATH])API_HeapAlloc(hHeap, 0, nRecentFiles * MAX_PATH * sizeof(wchar_t))))
    return FALSE;
  if (!(lpdwRecentPositions=(DWORD *)API_HeapAlloc(hHeap, 0, nRecentFiles * sizeof(DWORD))))
    return FALSE;
  if (!(lpdwRecentCodepages=(DWORD *)API_HeapAlloc(hHeap, 0, nRecentFiles * sizeof(DWORD))))
    return FALSE;
  return TRUE;
}

void RecentFilesZeroA()
{
  memset(lpszRecentNames, 0, nRecentFiles * MAX_PATH);
  memset(lpdwRecentPositions, 0, nRecentFiles * sizeof(DWORD));
  memset(lpdwRecentCodepages, 0, nRecentFiles * sizeof(DWORD));
}

void RecentFilesZeroW()
{
  memset(lpwszRecentNames, 0, nRecentFiles * MAX_PATH * sizeof(wchar_t));
  memset(lpdwRecentPositions, 0, nRecentFiles * sizeof(DWORD));
  memset(lpdwRecentCodepages, 0, nRecentFiles * sizeof(DWORD));
}

void RecentFilesReadA()
{
  char szRegValue[32];
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  DWORD dwCodePage;
  int i;

  //Read recent files array
  if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Recent", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  for (i=0; i < nRecentFiles; ++i)
  {
    wsprintfA(szRegValue, "nm%d", i);
    dwSize=MAX_PATH;
    if (RegQueryValueExA(hKey, szRegValue, NULL, &dwType, (LPBYTE)lpszRecentNames[i], &dwSize) != ERROR_SUCCESS)
      break;
    if (!*lpszRecentNames[i])
      break;

    wsprintfA(szRegValue, "ps%d", i);
    dwSize=sizeof(DWORD);
    RegQueryValueExA(hKey, szRegValue, NULL, &dwType, (LPBYTE)&lpdwRecentPositions[i], &dwSize);

    wsprintfA(szRegValue, "cp%d", i);
    dwSize=sizeof(DWORD);
    if (RegQueryValueExA(hKey, szRegValue, NULL, &dwType, (LPBYTE)&dwCodePage, &dwSize) == ERROR_SUCCESS)
      lpdwRecentCodepages[i]=LOWORD(dwCodePage);
  }
  RegCloseKey(hKey);
}

void RecentFilesReadW()
{
  wchar_t wszRegValue[32];
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  DWORD dwCodePage;
  int i;

  //Read recent files array
  if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Recent", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  for (i=0; i < nRecentFiles; ++i)
  {
    wsprintfW(wszRegValue, L"nm%d", i);
    dwSize=MAX_PATH * sizeof(wchar_t);
    if (RegQueryValueExW(hKey, wszRegValue, NULL, &dwType, (LPBYTE)lpwszRecentNames[i], &dwSize) != ERROR_SUCCESS)
      break;
    if (!*lpwszRecentNames[i])
      break;

    wsprintfW(wszRegValue, L"ps%d", i);
    dwSize=sizeof(DWORD);
    RegQueryValueExW(hKey, wszRegValue, NULL, &dwType, (LPBYTE)&lpdwRecentPositions[i], &dwSize);

    wsprintfW(wszRegValue, L"cp%d", i);
    dwSize=sizeof(DWORD);
    if (RegQueryValueExW(hKey, wszRegValue, NULL, &dwType, (LPBYTE)&dwCodePage, &dwSize) == ERROR_SUCCESS)
      lpdwRecentCodepages[i]=LOWORD(dwCodePage);
  }
  RegCloseKey(hKey);
}

BOOL RecentFilesGetA(char *pFile, int *nPosition, int *nCodePage)
{
  int i;

  if (!*pFile) return FALSE;

  for (i=0; i < nRecentFiles && *lpszRecentNames[i]; ++i)
  {
    if (!lstrcmpiA(lpszRecentNames[i], pFile))
    {
      if (nPosition) *nPosition=lpdwRecentPositions[i];
      if (nCodePage) *nCodePage=lpdwRecentCodepages[i];
      return TRUE;
    }
  }
  return FALSE;
}

BOOL RecentFilesGetW(wchar_t *wpFile, int *nPosition, int *nCodePage)
{
  int i;

  if (!*wpFile) return FALSE;

  for (i=0; i < nRecentFiles && *lpwszRecentNames[i]; ++i)
  {
    if (!lstrcmpiW(lpwszRecentNames[i], wpFile))
    {
      if (nPosition) *nPosition=lpdwRecentPositions[i];
      if (nCodePage) *nCodePage=lpdwRecentCodepages[i];
      return TRUE;
    }
  }
  return FALSE;
}

BOOL RecentFilesUpdateA(char *pFile, int nPosition, int nCodePage)
{
  int i;

  if (!*pFile) return FALSE;

  //Update recent files array - move/add current file to the first place
  for (i=0; i < nRecentFiles && *lpszRecentNames[i]; ++i)
  {
    if (!lstrcmpiA(lpszRecentNames[i], pFile))
    {
      if (nPosition == -1) nPosition=lpdwRecentPositions[i];
      if (nCodePage == -1) nCodePage=lpdwRecentCodepages[i];
      goto Move;
    }
  }
  if (i >= nRecentFiles) --i;
  if (nPosition == -1) nPosition=0;
  if (nCodePage == -1) nCodePage=nDefaultCodePage;

  Move:
  while (i > 0)
  {
    lstrcpynA(lpszRecentNames[i], lpszRecentNames[i-1], MAX_PATH);
    lpdwRecentPositions[i]=lpdwRecentPositions[i-1];
    lpdwRecentCodepages[i]=lpdwRecentCodepages[i-1];
    --i;
  }
  lstrcpynA(lpszRecentNames[0], pFile, MAX_PATH);
  lpdwRecentPositions[0]=nPosition;
  lpdwRecentCodepages[0]=nCodePage;

  return TRUE;
}

BOOL RecentFilesUpdateW(wchar_t *wpFile, int nPosition, int nCodePage)
{
  int i;

  if (!*wpFile) return FALSE;

  //Update recent files array - move/add current file to the first place
  for (i=0; i < nRecentFiles && *lpwszRecentNames[i]; ++i)
  {
    if (!lstrcmpiW(lpwszRecentNames[i], wpFile))
    {
      if (nPosition == -1) nPosition=lpdwRecentPositions[i];
      if (nCodePage == -1) nCodePage=lpdwRecentCodepages[i];
      goto Move;
    }
  }
  if (i >= nRecentFiles) --i;
  if (nPosition == -1) nPosition=0;
  if (nCodePage == -1) nCodePage=nDefaultCodePage;

  Move:
  while (i > 0)
  {
    lstrcpynW(lpwszRecentNames[i], lpwszRecentNames[i-1], MAX_PATH);
    lpdwRecentPositions[i]=lpdwRecentPositions[i-1];
    lpdwRecentCodepages[i]=lpdwRecentCodepages[i-1];
    --i;
  }
  lstrcpynW(lpwszRecentNames[0], wpFile, MAX_PATH);
  lpdwRecentPositions[0]=nPosition;
  lpdwRecentCodepages[0]=nCodePage;

  return TRUE;
}

int RecentFilesDeleteDeadA()
{
  int a;
  int b;
  int i;
  int nDead=0;

  //Delete files from recent files array if they doesn't exist
  for (i=0; i < nRecentFiles && *lpszRecentNames[i]; ++i)
  {
    while (*lpszRecentNames[i] && !FileExistsA(lpszRecentNames[i]))
    {
      for (a=i, b=nRecentFiles - 1; a < b && *lpszRecentNames[a]; ++a)
      {
        lstrcpynA(lpszRecentNames[a], lpszRecentNames[a+1], MAX_PATH);
        lpdwRecentPositions[a]=lpdwRecentPositions[a+1];
        lpdwRecentCodepages[a]=lpdwRecentCodepages[a+1];
      }
      *lpszRecentNames[a]='\0';
      lpdwRecentPositions[a]=0;
      lpdwRecentCodepages[a]=0;
      ++nDead;
    }
  }
  return nDead;
}

int RecentFilesDeleteDeadW()
{
  int a;
  int b;
  int i;
  int nDead=0;

  //Delete files from recent files array if they doesn't exist
  for (i=0; i < nRecentFiles && *lpwszRecentNames[i]; ++i)
  {
    while (*lpwszRecentNames[i] && !FileExistsW(lpwszRecentNames[i]))
    {
      for (a=i, b=nRecentFiles - 1; a < b && *lpwszRecentNames[a]; ++a)
      {
        lstrcpynW(lpwszRecentNames[a], lpwszRecentNames[a+1], MAX_PATH);
        lpdwRecentPositions[a]=lpdwRecentPositions[a+1];
        lpdwRecentCodepages[a]=lpdwRecentCodepages[a+1];
      }
      *lpwszRecentNames[a]='\0';
      lpdwRecentPositions[a]=0;
      lpdwRecentCodepages[a]=0;
      ++nDead;
    }
  }
  return nDead;
}

void RecentFilesSaveA()
{
  char szRegValue[32];
  HKEY hKey;
  int i;

  //Save recent files array
  if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Recent", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return;

  for (i=0; i < nRecentFiles; ++i)
  {
    wsprintfA(szRegValue, "nm%d", i);
    RegSetValueExA(hKey, szRegValue, 0, REG_SZ, (LPBYTE)lpszRecentNames[i], lstrlenA(lpszRecentNames[i]) + 1);

    wsprintfA(szRegValue, "ps%d", i);
    RegSetValueExA(hKey, szRegValue, 0, REG_DWORD, (LPBYTE)&lpdwRecentPositions[i], sizeof(DWORD));

    wsprintfA(szRegValue, "cp%d", i);
    RegSetValueExA(hKey, szRegValue, 0, REG_DWORD, (LPBYTE)&lpdwRecentCodepages[i], sizeof(DWORD));
  }
  RegCloseKey(hKey);
}

void RecentFilesSaveW()
{
  wchar_t wszRegValue[32];
  HKEY hKey;
  int i;

  //Save recent files array
  if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Recent", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return;

  for (i=0; i < nRecentFiles; ++i)
  {
    wsprintfW(wszRegValue, L"nm%d", i);
    RegSetValueExW(hKey, wszRegValue, 0, REG_SZ, (LPBYTE)lpwszRecentNames[i], lstrlenW(lpwszRecentNames[i]) * sizeof(wchar_t) + 2);

    wsprintfW(wszRegValue, L"ps%d", i);
    RegSetValueExW(hKey, wszRegValue, 0, REG_DWORD, (LPBYTE)&lpdwRecentPositions[i], sizeof(DWORD));

    wsprintfW(wszRegValue, L"cp%d", i);
    RegSetValueExW(hKey, wszRegValue, 0, REG_DWORD, (LPBYTE)&lpdwRecentCodepages[i], sizeof(DWORD));
  }
  RegCloseKey(hKey);
}

void RecentFilesMenuA()
{
  int i;

  for (i=1; DeleteMenu(hMainMenu, IDM_RECENT_FILES + i, MF_BYCOMMAND); ++i);

  if (nRecentFiles && *lpszRecentNames[0])
  {
    API_LoadStringA(hLangLib, STR_RECENT_FILES_DELETE_DEAD, buf, BUFFER_SIZE);
    ModifyMenuA(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING, IDM_RECENT_FILES, buf);

    for (i=0; i < nRecentFiles && *lpszRecentNames[i]; ++i)
    {
      TrimPathA(buf2, lpszRecentNames[i], RECENTFILES_RECORD_LENGTH);
      FixAmpA(buf2, buf, BUFFER_SIZE);
      InsertMenuA(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING, IDM_RECENT_FILES + i + 1, buf);
    }
    InsertMenuA(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_SEPARATOR, IDM_RECENT_FILES + i + 1, NULL);
  }
  else
  {
    API_LoadStringA(hLangLib, STR_RECENT_FILES_EMPTY, buf, BUFFER_SIZE);
    ModifyMenuA(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING|MF_GRAYED, IDM_RECENT_FILES, buf);
  }
}

void RecentFilesMenuW()
{
  int i;

  for (i=1; DeleteMenu(hMainMenu, IDM_RECENT_FILES + i, MF_BYCOMMAND); ++i);

  if (nRecentFiles && *lpwszRecentNames[0])
  {
    API_LoadStringW(hLangLib, STR_RECENT_FILES_DELETE_DEAD, wbuf, BUFFER_SIZE);
    ModifyMenuW(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING, IDM_RECENT_FILES, wbuf);

    for (i=0; i < nRecentFiles && *lpwszRecentNames[i]; ++i)
    {
      TrimPathW(wbuf2, lpwszRecentNames[i], RECENTFILES_RECORD_LENGTH);
      FixAmpW(wbuf2, wbuf, BUFFER_SIZE);
      InsertMenuW(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING, IDM_RECENT_FILES + i + 1, wbuf);
    }
    InsertMenuW(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_SEPARATOR, IDM_RECENT_FILES + i + 1, NULL);
  }
  else
  {
    API_LoadStringW(hLangLib, STR_RECENT_FILES_EMPTY, wbuf, BUFFER_SIZE);
    ModifyMenuW(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING|MF_GRAYED, IDM_RECENT_FILES, wbuf);
  }
}

int TrimPathA(char *szResult, char *pPath, int nMax)
{
  int nFileLen;
  int nLen=lstrlenA(pPath);
  char *pEnd=pPath + nLen;
  char *pFile=pEnd - 1;

  if (nLen <= nMax)
  {
    lstrcpyA(szResult, pPath);
    return nLen;
  }

  while (pFile >= pPath && *pFile != '\\') --pFile;

  if (pFile >= pPath)
  {
    nFileLen=pEnd - pFile;

    if (nMax > nFileLen + 2)
    {
      lstrcpynA(szResult, pPath, nMax - nFileLen - 2);
      lstrcatA(szResult, "...");
      lstrcatA(szResult, pFile);
      return nMax;
    }
  }
  lstrcpyA(szResult, "...");
  lstrcatA(szResult, pEnd - nMax + 3);
  return nMax;
}

int TrimPathW(wchar_t *wszResult, wchar_t *wpPath, int nMax)
{
  int nFileLen;
  int nLen=lstrlenW(wpPath);
  wchar_t *wpEnd=wpPath + nLen;
  wchar_t *wpFile=wpEnd - 1;

  if (nLen <= nMax)
  {
    lstrcpyW(wszResult, wpPath);
    return nLen;
  }

  while (wpFile >= wpPath && *wpFile != '\\') --wpFile;

  if (wpFile >= wpPath)
  {
    nFileLen=wpEnd - wpFile;

    if (nMax > nFileLen + 2)
    {
      lstrcpynW(wszResult, wpPath, nMax - nFileLen - 2);
      lstrcatW(wszResult, L"...");
      lstrcatW(wszResult, wpFile);
      return nMax;
    }
  }
  lstrcpyW(wszResult, L"...");
  lstrcatW(wszResult, wpEnd - nMax + 3);
  return nMax;
}

int FixAmpA(const char *pInput, char *szOutput, int nOutputMax)
{
  int a;
  int b;

  //Replace "&" with "&&"
  nOutputMax-=2;

  if (nOutputMax >= 0)
  {
    for (a=0, b=0; pInput[a] && b < nOutputMax; ++a, ++b)
    {
      szOutput[b]=pInput[a];
      if (pInput[a] == '&')
        szOutput[++b]='&';
    }
    szOutput[b]='\0';
    return b;
  }
  return -1;
}

int FixAmpW(const wchar_t *wpInput, wchar_t *wszOutput, int nOutputMax)
{
  int a;
  int b;

  //Replace "&" with "&&"
  nOutputMax-=2;

  if (nOutputMax >= 0)
  {
    for (a=0, b=0; wpInput[a] && b < nOutputMax; ++a, ++b)
    {
      wszOutput[b]=wpInput[a];
      if (wpInput[a] == '&')
        wszOutput[++b]='&';
    }
    wszOutput[b]='\0';
    return b;
  }
  return -1;
}


//// Language menu

void LanguageMenuA()
{
  WIN32_FIND_DATAA wfdA;
  HANDLE hFind;
  int nCommand=0;
  int i;
  nLangModules=0;

  for (i=1; DeleteMenu(hMainMenu, IDM_LANGUAGE + i, MF_BYCOMMAND); ++i);

  wsprintfA(buf, "%s\\AkelFiles\\Langs\\*.dll", szExeDir);

  if ((hFind=FindFirstFileA(buf, &wfdA)) != INVALID_HANDLE_VALUE)
  {
    i=1;

    do
    {
      if (!lstrcmpiA(szLangModule, wfdA.cFileName))
        nCommand=IDM_LANGUAGE + i;
      GetBaseNameA(wfdA.cFileName, buf, BUFFER_SIZE);
      InsertMenuA(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_STRING, IDM_LANGUAGE + i, buf);
      ++i;
    }
    while (FindNextFileA(hFind, &wfdA));

    nLangModules=i - 1;
    InsertMenuA(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_SEPARATOR, IDM_LANGUAGE + i, NULL);
    FindClose(hFind);
  }

  API_LoadStringA(hLangLib, STR_LANGUAGE_NONE, buf, BUFFER_SIZE);
  if (!*szLangModule)
    nCommand=IDM_LANGUAGE;
  ModifyMenuA(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_STRING, IDM_LANGUAGE, buf);

  CheckMenuRadioItem(hMainMenu, nCommand, nCommand, nCommand, MF_BYCOMMAND);
}

void LanguageMenuW()
{
  WIN32_FIND_DATAW wfdW;
  HANDLE hFind;
  int nCommand=0;
  int i;
  nLangModules=0;

  for (i=1; DeleteMenu(hMainMenu, IDM_LANGUAGE + i, MF_BYCOMMAND); ++i);

  wsprintfW(wbuf, L"%s\\AkelFiles\\Langs\\*.dll", wszExeDir);

  if ((hFind=FindFirstFileW(wbuf, &wfdW)) != INVALID_HANDLE_VALUE)
  {
    i=1;

    do
    {
      if (!lstrcmpiW(wszLangModule, wfdW.cFileName))
        nCommand=IDM_LANGUAGE + i;
      GetBaseNameW(wfdW.cFileName, wbuf, BUFFER_SIZE);
      InsertMenuW(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_STRING, IDM_LANGUAGE + i, wbuf);
      ++i;
    }
    while (FindNextFileW(hFind, &wfdW));

    nLangModules=i - 1;
    InsertMenuW(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_SEPARATOR, IDM_LANGUAGE + i, NULL);
    FindClose(hFind);
  }

  API_LoadStringW(hLangLib, STR_LANGUAGE_NONE, wbuf, BUFFER_SIZE);
  if (!*wszLangModule)
    nCommand=IDM_LANGUAGE;
  ModifyMenuW(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_STRING, IDM_LANGUAGE, wbuf);

  CheckMenuRadioItem(hMainMenu, nCommand, nCommand, nCommand, MF_BYCOMMAND);
}


//// Code page popup menu

void FillMenuPopupCodepageA()
{
  int i;

  if (lpCodepageList)
  {
    for (i=1; DeleteMenu(hPopupCodepage, IDM_POPUP_OPENAS + i, MF_BYCOMMAND); ++i);
    for (i=1; DeleteMenu(hPopupCodepage, IDM_POPUP_SAVEAS + i, MF_BYCOMMAND); ++i);

    for (i=0; lpCodepageList[i]; ++i)
    {
      GetCodePageNameA(lpCodepageList[i], buf, BUFFER_SIZE);

      if (lpCodepageList[i + 1])
      {
        InsertMenuA(hPopupCodepage, IDM_POPUP_OPENAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_OPENAS + i + 1, buf);
        InsertMenuA(hPopupCodepage, IDM_POPUP_SAVEAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_SAVEAS + i + 1, buf);
      }
      else
      {
        ModifyMenuA(hPopupCodepage, IDM_POPUP_OPENAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_OPENAS, buf);
        ModifyMenuA(hPopupCodepage, IDM_POPUP_SAVEAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_SAVEAS, buf);
      }
    }
  }
}

void FillMenuPopupCodepageW()
{
  int i;

  if (lpCodepageList)
  {
    for (i=1; DeleteMenu(hPopupCodepage, IDM_POPUP_OPENAS + i, MF_BYCOMMAND); ++i);
    for (i=1; DeleteMenu(hPopupCodepage, IDM_POPUP_SAVEAS + i, MF_BYCOMMAND); ++i);

    for (i=0; lpCodepageList[i]; ++i)
    {
      GetCodePageNameW(lpCodepageList[i], wbuf, BUFFER_SIZE);

      if (lpCodepageList[i + 1])
      {
        InsertMenuW(hPopupCodepage, IDM_POPUP_OPENAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_OPENAS + i + 1, wbuf);
        InsertMenuW(hPopupCodepage, IDM_POPUP_SAVEAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_SAVEAS + i + 1, wbuf);
      }
      else
      {
        ModifyMenuW(hPopupCodepage, IDM_POPUP_OPENAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_OPENAS, wbuf);
        ModifyMenuW(hPopupCodepage, IDM_POPUP_SAVEAS, MF_BYCOMMAND|MF_STRING, IDM_POPUP_SAVEAS, wbuf);
      }
    }
  }
}

void ShowMenuPopupCodepageA(POINT *pt)
{
  NCONTEXTMENU ncm;
  int nCodePageCur=-1;
  int nCodePageSum;
  int i;

  ncm.hWnd=hStatus;
  ncm.uType=NCM_STATUS;
  ncm.pt=*pt;
  ncm.bProcess=TRUE;
  SendMessage(hMainWnd, AKDN_CONTEXTMENU, 0, (LPARAM)&ncm);
  if (ncm.bProcess)
  {
    if (bMenuPopupCodepage)
    {
      bMenuPopupCodepage=FALSE;
      FillMenuPopupCodepageA();
    }

    for (i=0; lpCodepageList[i]; ++i)
    {
      if (lpCodepageList[i + 1])
      {
        if (lpCodepageList[i] == nCurrentCodePage)
          nCodePageCur=i + 1;
      }
      else
      {
        if (lpCodepageList[i] == nCurrentCodePage)
          nCodePageCur=0;
      }
    }
    nCodePageSum=i - 1;
    CheckMenuRadioItem(hPopupCodepage, IDM_POPUP_OPENAS, IDM_POPUP_OPENAS + nCodePageSum, IDM_POPUP_OPENAS + nCodePageCur, MF_BYCOMMAND);
    CheckMenuRadioItem(hPopupCodepage, IDM_POPUP_SAVEAS, IDM_POPUP_SAVEAS + nCodePageSum, IDM_POPUP_SAVEAS + nCodePageCur, MF_BYCOMMAND);

    TrackPopupMenu(hPopupCodepage, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt->x, pt->y, 0, hMainWnd, NULL);
  }
}

void ShowMenuPopupCodepageW(POINT *pt)
{
  NCONTEXTMENU ncm;
  int nCodePageCur=-1;
  int nCodePageSum;
  int i;

  ncm.hWnd=hStatus;
  ncm.uType=NCM_STATUS;
  ncm.pt=*pt;
  ncm.bProcess=TRUE;
  SendMessage(hMainWnd, AKDN_CONTEXTMENU, 0, (LPARAM)&ncm);
  if (ncm.bProcess)
  {
    if (bMenuPopupCodepage)
    {
      bMenuPopupCodepage=FALSE;
      FillMenuPopupCodepageW();
    }

    for (i=0; lpCodepageList[i]; ++i)
    {
      if (lpCodepageList[i + 1])
      {
        if (lpCodepageList[i] == nCurrentCodePage)
          nCodePageCur=i + 1;
      }
      else
      {
        if (lpCodepageList[i] == nCurrentCodePage)
          nCodePageCur=0;
      }
    }
    nCodePageSum=i - 1;
    CheckMenuRadioItem(hPopupCodepage, IDM_POPUP_OPENAS, IDM_POPUP_OPENAS + nCodePageSum, IDM_POPUP_OPENAS + nCodePageCur, MF_BYCOMMAND);
    CheckMenuRadioItem(hPopupCodepage, IDM_POPUP_SAVEAS, IDM_POPUP_SAVEAS + nCodePageSum, IDM_POPUP_SAVEAS + nCodePageCur, MF_BYCOMMAND);

    TrackPopupMenu(hPopupCodepage, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt->x, pt->y, 0, hMainWnd, NULL);
  }
}


//// Word breaking

BOOL isInDelimiterList(wchar_t c)
{
  if (wcschr(wszWordDelimiters, c) != NULL)
    return TRUE;
  else
    return FALSE;
}

BOOL isInWhitespaceList(wchar_t c)
{
  if (wcschr(WORD_WHITESPACESW, c) != NULL)
    return TRUE;
  else
    return FALSE;
}

int CALLBACK EditWordBreakProc(LPTSTR pchText, int ichCurrent, int cch, int code)
{
  wchar_t *pchTextW=(wchar_t *)pchText;
  int cchW=cch / sizeof(wchar_t);

  if (code == WB_ISDELIMITER)
  {
    return FALSE;
  }
  else if (code == WB_MOVEWORDLEFT)
  {
    if (bFirstWordBreak)
    {
      while (--ichCurrent >= 0 && isInDelimiterList(pchTextW[ichCurrent]));
      if (ichCurrent < 0) return -1;
      bFirstWordBreak=FALSE;
    }
    while (--ichCurrent >= 0 && !isInDelimiterList(pchTextW[ichCurrent]));
    if (ichCurrent < 0) return -1;
    bFirstWordBreak=TRUE;
    return ichCurrent + 1;
  }
  else if (code == WB_MOVEWORDRIGHT)
  {
    if (bFirstWordBreak)
    {
      while (ichCurrent < cchW && isInDelimiterList(pchTextW[ichCurrent])) ++ichCurrent;
      if (ichCurrent >= cchW) return -1;
      bFirstWordBreak=FALSE;
      ++ichCurrent;
    }
    while (ichCurrent < cchW && !isInDelimiterList(pchTextW[ichCurrent])) ++ichCurrent;
    if (ichCurrent >= cchW) return -1;
    bFirstWordBreak=TRUE;
    return ichCurrent;
  }
  else if (code == WB_LEFTBREAK)
  {
    ichCurrent-=2;
    while (ichCurrent >= 0 && !isInWhitespaceList(pchTextW[ichCurrent])) --ichCurrent;
    if (ichCurrent < 0) return -1;
    return ichCurrent + 1;
  }
  else if (code == WB_RIGHTBREAK)
  {
    ++ichCurrent;
    while (ichCurrent < cchW && !isInWhitespaceList(pchTextW[ichCurrent])) ++ichCurrent;
    if (ichCurrent >= cchW) return -1;
    return ichCurrent;
  }
  else if (code == WB_CLASSIFY)
  {
    //FR_WHOLEWORD
    if (!isInDelimiterList(pchTextW[ichCurrent]))
      return WBF_ISWHITE;
    else
      return -1;
  }
  return 0;
}


//// Recode

void RecodeTextW(HWND hWnd, int nCodePageFrom, int nCodePageTo)
{
  CHARRANGE crInitial;
  RECT rcEdit;
  wchar_t *wszSelText;
  char *szText;
  wchar_t *wszText;
  int nFirstLine;
  int nUnicodeLen;
  int nAnsiLen;

  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveLineScroll(hWnd, &rcEdit, &nFirstLine);

  crInitial.cpMin=chrg.cpMin;
  crInitial.cpMax=chrg.cpMax;
  if (chrg.cpMin == chrg.cpMax)
    SetTextSel(hWnd, 0, -1);
  nUnicodeLen=(chrg.cpMax - chrg.cpMin) + 1;

  if (wszSelText=GetSelTextW(hWnd, NULL))
  {
    nAnsiLen=WideCharToMultiByte(nCodePageFrom, 0, wszSelText, nUnicodeLen, NULL, 0, NULL, NULL);

    if (szText=(char *)API_HeapAlloc(hHeap, 0, nAnsiLen))
    {
      WideCharToMultiByte(nCodePageFrom, 0, wszSelText, nUnicodeLen, szText, nAnsiLen, NULL, NULL);
      nUnicodeLen=MultiByteToWideChar(nCodePageTo, 0, szText, nAnsiLen, NULL, 0);

      if (wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nUnicodeLen * sizeof(wchar_t)))
      {
        MultiByteToWideChar(nCodePageTo, 0, szText, nAnsiLen, wszText, nUnicodeLen);

        ReplaceSelW(hWnd, wszText, -1);
        SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin);
        if (crInitial.cpMin != crInitial.cpMax)
          SetTextSel(hWnd, crInitial.cpMin, crInitial.cpMin + nUnicodeLen - 1);

        API_HeapFree(hHeap, 0, (LPVOID)wszText);
      }
      API_HeapFree(hHeap, 0, (LPVOID)szText);
    }
    FreeText(wszSelText);
  }
  RestoreLineScroll(hWnd, &rcEdit, &nFirstLine);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
}

BOOL CALLBACK RecodeDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCodePageFrom;
  static HWND hWndCodePageTo;
  static int nCodePageFrom=0;
  static int nCodePageTo=0;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndCodePageFrom=GetDlgItem(hDlg, IDC_RECODE_CODEPAGE_FROM);
    hWndCodePageTo=GetDlgItem(hDlg, IDC_RECODE_CODEPAGE_TO);

    bRecodeDlg=TRUE;
    FillComboboxCodepageA(hWndCodePageFrom, lpCodepageList);
    FillComboboxCodepageA(hWndCodePageTo, lpCodepageList);
    bRecodeDlg=FALSE;

    if (!nCodePageFrom || SelectComboboxCodepageA(hWndCodePageFrom, nCodePageFrom) == CB_ERR)
      SendMessage(hWndCodePageFrom, CB_SETCURSEL, 0, 0);
    if (!nCodePageTo || SelectComboboxCodepageA(hWndCodePageTo, nCodePageTo) == CB_ERR)
      SendMessage(hWndCodePageTo, CB_SETCURSEL, 0, 0);

    if (rcRecodeDlg.right && rcRecodeDlg.bottom)
      SetWindowPos(hDlg, 0, rcRecodeDlg.left, rcRecodeDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    SendMessage(hDlg, WM_COMMAND, IDC_SETREADONLY, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SETREADONLY)
    {
      EnableWindow(GetDlgItem(hDlg, IDOK), !bReadOnly);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      nCodePageFrom=GetComboboxCodepageA(hWndCodePageFrom);
      nCodePageTo=GetComboboxCodepageA(hWndCodePageTo);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;

      RecodeTextW(hWndEdit, nCodePageFrom, nCodePageTo);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      GetWindowPos(hDlg, NULL, &rcRecodeDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK RecodeDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCodePageFrom;
  static HWND hWndCodePageTo;
  static int nCodePageFrom=0;
  static int nCodePageTo=0;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndCodePageFrom=GetDlgItem(hDlg, IDC_RECODE_CODEPAGE_FROM);
    hWndCodePageTo=GetDlgItem(hDlg, IDC_RECODE_CODEPAGE_TO);

    bRecodeDlg=TRUE;
    FillComboboxCodepageW(hWndCodePageFrom, lpCodepageList);
    FillComboboxCodepageW(hWndCodePageTo, lpCodepageList);
    bRecodeDlg=FALSE;

    if (!nCodePageFrom || SelectComboboxCodepageW(hWndCodePageFrom, nCodePageFrom) == CB_ERR)
      SendMessage(hWndCodePageFrom, CB_SETCURSEL, 0, 0);
    if (!nCodePageTo || SelectComboboxCodepageW(hWndCodePageTo, nCodePageTo) == CB_ERR)
      SendMessage(hWndCodePageTo, CB_SETCURSEL, 0, 0);

    if (rcRecodeDlg.right && rcRecodeDlg.bottom)
      SetWindowPos(hDlg, 0, rcRecodeDlg.left, rcRecodeDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    SendMessage(hDlg, WM_COMMAND, IDC_SETREADONLY, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SETREADONLY)
    {
      EnableWindow(GetDlgItem(hDlg, IDOK), !bReadOnly);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      nCodePageFrom=GetComboboxCodepageW(hWndCodePageFrom);
      nCodePageTo=GetComboboxCodepageW(hWndCodePageTo);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;

      RecodeTextW(hWndEdit, nCodePageFrom, nCodePageTo);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      GetWindowPos(hDlg, NULL, &rcRecodeDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      return TRUE;
    }
  }
  return FALSE;
}


//Plugins

BOOL CALLBACK PluginsDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndListInfo;
  static HWND hWndHotkeyLabel;
  static HWND hWndHotkey;
  static HWND hWndAssign;
  static HWND hWndCall;
  static HWND hWndOK;
  static HWND hWndCancel;
  static int nSelItem;
  static BOOL bListChanged;
  static DIALOGRESIZE drs[]={{&hWndList,        DRS_SIZE|DRS_X|DRS_Y},
                             {&hWndListInfo,    DRS_MOVE|DRS_Y},
                             {&hWndHotkeyLabel, DRS_MOVE|DRS_X},
                             {&hWndHotkey,      DRS_MOVE|DRS_X},
                             {&hWndAssign,      DRS_MOVE|DRS_X},
                             {&hWndCall,        DRS_MOVE|DRS_Y},
                             {&hWndOK,          DRS_MOVE|DRS_X|DRS_Y},
                             {&hWndCancel,      DRS_MOVE|DRS_X|DRS_Y},
                             {0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNA lvcA;
    char szPlugin[MAX_PATH];
    char szFunction[MAX_PATH];

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndList=GetDlgItem(hDlg, IDC_PLUGINS_LIST);
    hWndListInfo=GetDlgItem(hDlg, IDC_PLUGINS_LIST_INFO);
    hWndHotkeyLabel=GetDlgItem(hDlg, IDC_PLUGINS_HOTKEY_LABEL);
    hWndHotkey=GetDlgItem(hDlg, IDC_PLUGINS_HOTKEY);
    hWndAssign=GetDlgItem(hDlg, IDC_PLUGINS_ASSIGN);
    hWndCall=GetDlgItem(hDlg, IDC_PLUGINS_CALL);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    EnableWindow(hWndHotkey, FALSE);
    EnableWindow(hWndAssign, FALSE);
    EnableWindow(hWndCall, FALSE);

    //Columns
    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=buf;

    API_LoadStringA(hLangLib, STR_PLUGIN, szPlugin, BUFFER_SIZE);
    API_LoadStringA(hLangLib, STR_FUNCTION, szFunction, BUFFER_SIZE);
    wsprintfA(buf, "%s::%s", szPlugin, szFunction);
    lvcA.cx=209;
    lvcA.iSubItem=LVSI_FUNCTION_NAME;
    SendMessage(hWndList, LVM_INSERTCOLUMNA, LVSI_FUNCTION_NAME, (LPARAM)&lvcA);

    API_LoadStringA(hLangLib, STR_HOTKEY, buf, BUFFER_SIZE);
    lvcA.cx=105;
    lvcA.iSubItem=LVSI_FUNCTION_HOTKEY;
    SendMessage(hWndList, LVM_INSERTCOLUMNA, LVSI_FUNCTION_HOTKEY, (LPARAM)&lvcA);

    API_LoadStringA(hLangLib, STR_STATUS, buf, BUFFER_SIZE);
    lvcA.cx=63;
    lvcA.iSubItem=LVSI_FUNCTION_STATUS;
    SendMessage(hWndList, LVM_INSERTCOLUMNA, LVSI_FUNCTION_STATUS, (LPARAM)&lvcA);

    FillPluginListA(hWndList);
    nSelItem=-1;
    bListChanged=FALSE;

    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PLUGINS_ASSIGN)
    {
      PLUGINFUNCTIONA *pfElement=NULL;
      PLUGINLISTITEMA *pliElement;
      LVITEMA lviA;
      WORD wHotkey;

      if (wHotkey=(WORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0))
        pfElement=StackHotkeyFindA(&hPluginsStack, wHotkey);

      if (!pfElement)
      {
        GetHotkeyStringA(wHotkey, buf);

        lviA.mask=LVIF_TEXT;
        lviA.pszText=buf;
        lviA.iItem=nSelItem;
        lviA.iSubItem=LVSI_FUNCTION_HOTKEY;
        SendMessage(hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);

        if (pliElement=GetPluginListItemA(&hPluginListStack, nSelItem + 1))
        {
          pliElement->pf->wHotkey=wHotkey;
          bListChanged=TRUE;
        }
      }
      else
      {
        API_LoadStringA(hLangLib, MSG_HOTKEY_EXISTS, buf, BUFFER_SIZE);
        wsprintfA(buf2, buf, pfElement->szFunction);
        MessageBoxA(hDlg, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
        SetFocus(hWndHotkey);
      }
    }
    else if (LOWORD(wParam) == IDC_PLUGINS_CALL)
    {
      PLUGINLISTITEMA *pliElement;
      PLUGINCALLPOSTA *pcp=NULL;

      if (nSelItem >= 0)
      {
        if (pliElement=GetPluginListItemA(&hPluginListStack, nSelItem + 1))
        {
          if (pcp=(PLUGINCALLPOSTA *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTA)))
          {
            lstrcpynA(pcp->szFunction, pliElement->pf->szFunction, MAX_PATH);
            pcp->bOnStart=FALSE;
            pcp->lParam=0;
          }
        }

        FreePluginList(&hPluginListStack);
        StackPluginCleanUpA(&hPluginsStack, bListChanged?TRUE:FALSE);
        if (bListChanged) StackPluginSaveA(&hPluginsStack, TRUE);

        EndDialog(hDlg, 0);
        if (pcp) PostMessage(hMainWnd, AKD_DLLCALL, 0, (LPARAM)pcp);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      FreePluginList(&hPluginListStack);
      StackPluginCleanUpA(&hPluginsStack, TRUE);
      StackPluginSaveA(&hPluginsStack, TRUE);

      EndDialog(hDlg, 0);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      PLUGINLISTITEMA *pliElement=(PLUGINLISTITEMA *)hPluginListStack.first;

      while (pliElement)
      {
        pliElement->pf->wHotkey=pliElement->wInitialHotkey;
        pliElement->pf->bOnStart=pliElement->bInitialOnStart;

        pliElement=pliElement->next;
      }
      FreePluginList(&hPluginListStack);
      StackPluginCleanUpA(&hPluginsStack, FALSE);

      EndDialog(hDlg, 0);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_PLUGINS_LIST)
    {
      if (((NMLISTVIEW *)lParam)->hdr.code == LVN_ITEMCHANGING)
      {
        PLUGINLISTITEMA *pliElement;
        BOOL bNewState;
        BOOL bOldState;

        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK)
        {
          bNewState=((((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          bOldState=((((NMLISTVIEW *)lParam)->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            if (pliElement=GetPluginListItemA(&hPluginListStack, ((NMLISTVIEW *)lParam)->iItem + 1))
            {
              if (pliElement->pf)
              {
                pliElement->pf->bOnStart=bNewState;
                bListChanged=TRUE;

                if (bNewState)
                {
                  if (pliElement->nAutoLoad == -1)
                  {
                    CallPluginA(NULL, pliElement->pf->szFunction, FALSE, 0, &pliElement->nAutoLoad);
                  }
                  if (pliElement->nAutoLoad == 0)
                  {
                    pliElement->pf->bOnStart=FALSE;
                    API_LoadStringA(hLangLib, MSG_AUTOLOAD_IS_NOT_SUPPORTED, buf, BUFFER_SIZE);
                    wsprintfA(buf2, buf, pliElement->pf->szFunction);
                    MessageBoxA(hDlg, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
                    SetWindowLongA(hDlg, DWL_MSGRESULT, TRUE);
                    return TRUE;
                  }
                }
              }
            }
          }
        }
        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_SELECTED)
        {
          nSelItem=((NMLISTVIEW *)lParam)->iItem;

          if (pliElement=GetPluginListItemA(&hPluginListStack, nSelItem + 1))
          {
            SendMessage(hWndHotkey, HKM_SETHOTKEY, pliElement->pf->wHotkey, 0);
            EnableWindow(hWndHotkey, TRUE);
            EnableWindow(hWndAssign, TRUE);
            EnableWindow(hWndCall, TRUE);
          }
        }
        else if (((NMLISTVIEW *)lParam)->uOldState & LVIS_SELECTED)
        {
          nSelItem=-1;

          SendMessage(hWndHotkey, HKM_SETHOTKEY, 0, 0);
          EnableWindow(hWndHotkey, FALSE);
          EnableWindow(hWndAssign, FALSE);
          EnableWindow(hWndCall, FALSE);
        }
      }
      else if ((int)((NMHDR *)lParam)->code == NM_DBLCLK)
      {
        LVHITTESTINFO lvhti={0};

        GetCursorPos(&lvhti.pt);
        ScreenToClient(hWndList, &lvhti.pt);
        SendMessage(hWndList, LVM_HITTEST, 0, (LPARAM)&lvhti);

        if (lvhti.flags & LVHT_ONITEMLABEL)
          PostMessage(hDlg, WM_COMMAND, IDC_PLUGINS_CALL, 0);
      }
    }
  }
  DialogResizeMessages(&drs[0], &rcPluginsInitDialog, &rcPluginsCurrentDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

BOOL CALLBACK PluginsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndListInfo;
  static HWND hWndHotkeyLabel;
  static HWND hWndHotkey;
  static HWND hWndAssign;
  static HWND hWndCall;
  static HWND hWndOK;
  static HWND hWndCancel;
  static int nSelItem;
  static BOOL bListChanged;
  static DIALOGRESIZE drs[]={{&hWndList,        DRS_SIZE|DRS_X|DRS_Y},
                             {&hWndListInfo,    DRS_MOVE|DRS_Y},
                             {&hWndHotkeyLabel, DRS_MOVE|DRS_X},
                             {&hWndHotkey,      DRS_MOVE|DRS_X},
                             {&hWndAssign,      DRS_MOVE|DRS_X},
                             {&hWndCall,        DRS_MOVE|DRS_Y},
                             {&hWndOK,          DRS_MOVE|DRS_X|DRS_Y},
                             {&hWndCancel,      DRS_MOVE|DRS_X|DRS_Y},
                             {0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvcW;
    wchar_t wszPlugin[MAX_PATH];
    wchar_t wszFunction[MAX_PATH];

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndList=GetDlgItem(hDlg, IDC_PLUGINS_LIST);
    hWndListInfo=GetDlgItem(hDlg, IDC_PLUGINS_LIST_INFO);
    hWndHotkeyLabel=GetDlgItem(hDlg, IDC_PLUGINS_HOTKEY_LABEL);
    hWndHotkey=GetDlgItem(hDlg, IDC_PLUGINS_HOTKEY);
    hWndAssign=GetDlgItem(hDlg, IDC_PLUGINS_ASSIGN);
    hWndCall=GetDlgItem(hDlg, IDC_PLUGINS_CALL);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    EnableWindow(hWndHotkey, FALSE);
    EnableWindow(hWndAssign, FALSE);
    EnableWindow(hWndCall, FALSE);

    //Columns
    lvcW.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcW.pszText=wbuf;

    API_LoadStringW(hLangLib, STR_PLUGIN, wszPlugin, BUFFER_SIZE);
    API_LoadStringW(hLangLib, STR_FUNCTION, wszFunction, BUFFER_SIZE);
    wsprintfW(wbuf, L"%s::%s", wszPlugin, wszFunction);
    lvcW.cx=209;
    lvcW.iSubItem=LVSI_FUNCTION_NAME;
    SendMessage(hWndList, LVM_INSERTCOLUMNW, LVSI_FUNCTION_NAME, (LPARAM)&lvcW);

    API_LoadStringW(hLangLib, STR_HOTKEY, wbuf, BUFFER_SIZE);
    lvcW.cx=105;
    lvcW.iSubItem=LVSI_FUNCTION_HOTKEY;
    SendMessage(hWndList, LVM_INSERTCOLUMNW, LVSI_FUNCTION_HOTKEY, (LPARAM)&lvcW);

    API_LoadStringW(hLangLib, STR_STATUS, wbuf, BUFFER_SIZE);
    lvcW.cx=63;
    lvcW.iSubItem=LVSI_FUNCTION_STATUS;
    SendMessage(hWndList, LVM_INSERTCOLUMNW, LVSI_FUNCTION_STATUS, (LPARAM)&lvcW);

    FillPluginListW(hWndList);
    nSelItem=-1;
    bListChanged=FALSE;

    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PLUGINS_ASSIGN)
    {
      PLUGINFUNCTIONW *pfElement=NULL;
      PLUGINLISTITEMW *pliElement;
      LVITEMW lviW;
      WORD wHotkey;

      if (wHotkey=(WORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0))
        pfElement=StackHotkeyFindW(&hPluginsStack, wHotkey);

      if (!pfElement)
      {
        GetHotkeyStringW(wHotkey, wbuf);

        lviW.mask=LVIF_TEXT;
        lviW.pszText=wbuf;
        lviW.iItem=nSelItem;
        lviW.iSubItem=LVSI_FUNCTION_HOTKEY;
        SendMessage(hWndList, LVM_SETITEMW, 0, (LPARAM)&lviW);

        if (pliElement=GetPluginListItemW(&hPluginListStack, nSelItem + 1))
        {
          pliElement->pf->wHotkey=wHotkey;
          bListChanged=TRUE;
        }
      }
      else
      {
        API_LoadStringW(hLangLib, MSG_HOTKEY_EXISTS, wbuf, BUFFER_SIZE);
        wsprintfW(wbuf2, wbuf, pfElement->szFunction);
        MessageBoxW(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
        SetFocus(hWndHotkey);
      }
    }
    else if (LOWORD(wParam) == IDC_PLUGINS_CALL)
    {
      PLUGINLISTITEMW *pliElement;
      PLUGINCALLPOSTW *pcp=NULL;

      if (nSelItem >= 0)
      {
        if (pliElement=GetPluginListItemW(&hPluginListStack, nSelItem + 1))
        {
          if (pcp=(PLUGINCALLPOSTW *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW)))
          {
            lstrcpynW(pcp->szFunction, pliElement->pf->szFunction, MAX_PATH);
            pcp->bOnStart=FALSE;
            pcp->lParam=0;
          }
        }

        FreePluginList(&hPluginListStack);
        StackPluginCleanUpW(&hPluginsStack, bListChanged?TRUE:FALSE);
        if (bListChanged) StackPluginSaveW(&hPluginsStack, TRUE);

        EndDialog(hDlg, 0);
        if (pcp) PostMessage(hMainWnd, AKD_DLLCALL, 0, (LPARAM)pcp);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      FreePluginList(&hPluginListStack);
      StackPluginCleanUpW(&hPluginsStack, TRUE);
      StackPluginSaveW(&hPluginsStack, TRUE);

      EndDialog(hDlg, 0);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      PLUGINLISTITEMW *pliElement=(PLUGINLISTITEMW *)hPluginListStack.first;

      while (pliElement)
      {
        pliElement->pf->wHotkey=pliElement->wInitialHotkey;
        pliElement->pf->bOnStart=pliElement->bInitialOnStart;

        pliElement=pliElement->next;
      }
      FreePluginList(&hPluginListStack);
      StackPluginCleanUpW(&hPluginsStack, FALSE);

      EndDialog(hDlg, 0);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_PLUGINS_LIST)
    {
      if (((NMLISTVIEW *)lParam)->hdr.code == LVN_ITEMCHANGING)
      {
        PLUGINLISTITEMW *pliElement;
        BOOL bNewState;
        BOOL bOldState;

        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK)
        {
          bNewState=((((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          bOldState=((((NMLISTVIEW *)lParam)->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            if (pliElement=GetPluginListItemW(&hPluginListStack, ((NMLISTVIEW *)lParam)->iItem + 1))
            {
              if (pliElement->pf)
              {
                pliElement->pf->bOnStart=bNewState;
                bListChanged=TRUE;

                if (bNewState)
                {
                  if (pliElement->nAutoLoad == -1)
                  {
                    CallPluginW(NULL, pliElement->pf->szFunction, FALSE, 0, &pliElement->nAutoLoad);
                  }
                  if (pliElement->nAutoLoad == 0)
                  {
                    pliElement->pf->bOnStart=FALSE;
                    API_LoadStringW(hLangLib, MSG_AUTOLOAD_IS_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
                    wsprintfW(wbuf2, wbuf, pliElement->pf->szFunction);
                    MessageBoxW(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
                    SetWindowLongW(hDlg, DWL_MSGRESULT, TRUE);
                    return TRUE;
                  }
                }
              }
            }
          }
        }
        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_SELECTED)
        {
          nSelItem=((NMLISTVIEW *)lParam)->iItem;

          if (pliElement=GetPluginListItemW(&hPluginListStack, nSelItem + 1))
          {
            SendMessage(hWndHotkey, HKM_SETHOTKEY, pliElement->pf->wHotkey, 0);
            EnableWindow(hWndHotkey, TRUE);
            EnableWindow(hWndAssign, TRUE);
            EnableWindow(hWndCall, TRUE);
          }
        }
        else if (((NMLISTVIEW *)lParam)->uOldState & LVIS_SELECTED)
        {
          nSelItem=-1;

          SendMessage(hWndHotkey, HKM_SETHOTKEY, 0, 0);
          EnableWindow(hWndHotkey, FALSE);
          EnableWindow(hWndAssign, FALSE);
          EnableWindow(hWndCall, FALSE);
        }
      }
      else if ((int)((NMHDR *)lParam)->code == NM_DBLCLK)
      {
        LVHITTESTINFO lvhti={0};

        GetCursorPos(&lvhti.pt);
        ScreenToClient(hWndList, &lvhti.pt);
        SendMessage(hWndList, LVM_HITTEST, 0, (LPARAM)&lvhti);

        if (lvhti.flags & LVHT_ONITEMLABEL)
          PostMessage(hDlg, WM_COMMAND, IDC_PLUGINS_CALL, 0);
      }
    }
  }
  DialogResizeMessages(&drs[0], &rcPluginsInitDialog, &rcPluginsCurrentDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

void FillPluginListA(HWND hWnd)
{
  WIN32_FIND_DATAA wfdA;
  HANDLE hFind;
  HMODULE hInstance;
  PLUGINLISTDATA pld={0};
  char szBaseName[MAX_PATH];

  pld.hWndList=hWnd;
  wsprintfA(buf, "%s\\AkelFiles\\Plugs\\*.dll", szExeDir);

  if ((hFind=FindFirstFileA(buf, &wfdA)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      wsprintfA(buf, "%s\\AkelFiles\\Plugs\\%s", szExeDir, wfdA.cFileName);

      if (hInstance=LoadLibraryA(buf))
      {
        if (GetProcAddress(hInstance, "DllAkelPadID"))
        {
          GetBaseNameA(wfdA.cFileName, szBaseName, MAX_PATH);
          pld.pBaseName=(unsigned char *)szBaseName;
          GetExportNames(hInstance, FillPluginListProcA, (LPARAM)&pld);
        }
        FreeLibrary(hInstance);
      }
    }
    while (FindNextFileA(hFind, &wfdA));

    FindClose(hFind);
  }
}

void FillPluginListW(HWND hWnd)
{
  WIN32_FIND_DATAW wfdW;
  HANDLE hFind;
  HMODULE hInstance;
  PLUGINLISTDATA pld={0};
  wchar_t wszBaseName[MAX_PATH];

  pld.hWndList=hWnd;
  wsprintfW(wbuf, L"%s\\AkelFiles\\Plugs\\*.dll", wszExeDir);

  if ((hFind=FindFirstFileW(wbuf, &wfdW)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      wsprintfW(wbuf, L"%s\\AkelFiles\\Plugs\\%s", wszExeDir, wfdW.cFileName);

      if (hInstance=LoadLibraryW(wbuf))
      {
        if (GetProcAddress(hInstance, "DllAkelPadID"))
        {
          GetBaseNameW(wfdW.cFileName, wszBaseName, MAX_PATH);
          pld.pBaseName=(unsigned char *)wszBaseName;
          GetExportNames(hInstance, FillPluginListProcW, (LPARAM)&pld);
        }
        FreeLibrary(hInstance);
      }
    }
    while (FindNextFileW(hFind, &wfdW));

    FindClose(hFind);
  }
}

BOOL CALLBACK FillPluginListProcA(char *pExportName, LPARAM lParam)
{
  PLUGINLISTDATA *pld=(PLUGINLISTDATA *)lParam;
  PLUGINLISTITEMA *pliElement;
  PLUGINFUNCTIONA *pfElement;
  LVITEMA lviA;
  int nIndex;

  if (memcmp(pExportName, "Dll", 3))
  {
    wsprintfA(buf, "%s::%s", (char *)pld->pBaseName, pExportName);
    lviA.mask=LVIF_TEXT;
    lviA.pszText=buf;
    lviA.iItem=0;
    lviA.iSubItem=LVSI_FUNCTION_NAME;
    nIndex=SendMessage(pld->hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

    if (!StackInsertIndex((stack **)&hPluginListStack.first, (stack **)&hPluginListStack.last, (stack **)&pliElement, nIndex + 1, sizeof(PLUGINLISTITEMA)))
    {
      if (pfElement=StackPluginFindA(&hPluginsStack, buf))
      {
        if (pfElement->bOnStart)
        {
          lviA.mask=LVIF_STATE;
          lviA.iItem=nIndex;
          lviA.iSubItem=LVSI_FUNCTION_NAME;
          lviA.state=((pfElement->bOnStart + 1) << 12);
          lviA.stateMask=LVIS_STATEIMAGEMASK;
          SendMessage(pld->hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);
        }
        if (pfElement->wHotkey)
        {
          GetHotkeyStringA(pfElement->wHotkey, buf);
          lviA.mask=LVIF_TEXT;
          lviA.pszText=buf;
          lviA.iItem=nIndex;
          lviA.iSubItem=LVSI_FUNCTION_HOTKEY;
          SendMessage(pld->hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);
        }
        if (pfElement->bRunning)
        {
          API_LoadStringA(hLangLib, STR_RUNNING, buf, BUFFER_SIZE);
          lviA.mask=LVIF_TEXT;
          lviA.pszText=buf;
          lviA.iItem=nIndex;
          lviA.iSubItem=LVSI_FUNCTION_STATUS;
          SendMessage(pld->hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);
        }
      }
      else pfElement=StackPluginAddA(&hPluginsStack, buf, lstrlenA(buf), 0, FALSE, FALSE, NULL, NULL);

      pliElement->pf=pfElement;
      pliElement->wInitialHotkey=pfElement->wHotkey;
      pliElement->bInitialOnStart=pfElement->bOnStart;
      pliElement->nAutoLoad=-1;
    }
  }
  return TRUE;
}

BOOL CALLBACK FillPluginListProcW(char *pExportName, LPARAM lParam)
{
  wchar_t wszExportName[MAX_PATH];
  PLUGINLISTDATA *pld=(PLUGINLISTDATA *)lParam;
  PLUGINLISTITEMW *pliElement;
  PLUGINFUNCTIONW *pfElement;
  LVITEMW lviW;
  int nIndex;

  if (memcmp(pExportName, "Dll", 3))
  {
    MultiByteToWideChar(CP_ACP, 0, pExportName, -1, wszExportName, MAX_PATH);
    wsprintfW(wbuf, L"%s::%s", (wchar_t *)pld->pBaseName, wszExportName);
    lviW.mask=LVIF_TEXT;
    lviW.pszText=wbuf;
    lviW.iItem=0;
    lviW.iSubItem=LVSI_FUNCTION_NAME;
    nIndex=SendMessage(pld->hWndList, LVM_INSERTITEMW, 0, (LPARAM)&lviW);

    if (!StackInsertIndex((stack **)&hPluginListStack.first, (stack **)&hPluginListStack.last, (stack **)&pliElement, nIndex + 1, sizeof(PLUGINLISTITEMW)))
    {
      if (pfElement=StackPluginFindW(&hPluginsStack, wbuf))
      {
        if (pfElement->bOnStart)
        {
          lviW.mask=LVIF_STATE;
          lviW.iItem=nIndex;
          lviW.iSubItem=LVSI_FUNCTION_NAME;
          lviW.state=((pfElement->bOnStart + 1) << 12);
          lviW.stateMask=LVIS_STATEIMAGEMASK;
          SendMessage(pld->hWndList, LVM_SETITEMW, 0, (LPARAM)&lviW);
        }
        if (pfElement->wHotkey)
        {
          GetHotkeyStringW(pfElement->wHotkey, wbuf);
          lviW.mask=LVIF_TEXT;
          lviW.pszText=wbuf;
          lviW.iItem=nIndex;
          lviW.iSubItem=LVSI_FUNCTION_HOTKEY;
          SendMessage(pld->hWndList, LVM_SETITEMW, 0, (LPARAM)&lviW);
        }
        if (pfElement->bRunning)
        {
          API_LoadStringW(hLangLib, STR_RUNNING, wbuf, BUFFER_SIZE);
          lviW.mask=LVIF_TEXT;
          lviW.pszText=wbuf;
          lviW.iItem=nIndex;
          lviW.iSubItem=LVSI_FUNCTION_STATUS;
          SendMessage(pld->hWndList, LVM_SETITEMW, 0, (LPARAM)&lviW);
        }
      }
      else pfElement=StackPluginAddW(&hPluginsStack, wbuf, lstrlenW(wbuf), 0, FALSE, FALSE, NULL, NULL);

      pliElement->pf=pfElement;
      pliElement->wInitialHotkey=pfElement->wHotkey;
      pliElement->bInitialOnStart=pfElement->bOnStart;
      pliElement->nAutoLoad=-1;
    }
  }
  return TRUE;
}

PLUGINLISTITEMA* GetPluginListItemA(HSTACK *hStack, int nIndex)
{
  PLUGINLISTITEMA *pliElement;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&pliElement, nIndex))
    return pliElement;
  return NULL;
}

PLUGINLISTITEMW* GetPluginListItemW(HSTACK *hStack, int nIndex)
{
  PLUGINLISTITEMW *pliElement;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&pliElement, nIndex))
    return pliElement;
  return NULL;
}

void FreePluginList(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void GetHotkeyStringA(WORD wHotkey, char *szString)
{
  char szKeyText[100];
  LONG lScan;

  szString[0]='\0';

  if (wHotkey)
  {
    if (HIBYTE(wHotkey) & HOTKEYF_CONTROL)
      lstrcatA(szString, "Ctrl + ");
    if (HIBYTE(wHotkey) & HOTKEYF_SHIFT)
      lstrcatA(szString, "Shift + ");
    if (HIBYTE(wHotkey) & HOTKEYF_ALT)
      lstrcatA(szString, "Alt + ");
    lScan=MapVirtualKey(LOBYTE(wHotkey), 0) << 16;
    if (HIBYTE(wHotkey) & HOTKEYF_EXT) lScan|=0x01000000L;
    GetKeyNameTextA(lScan, szKeyText, 100);
    lstrcatA(szString, szKeyText);
  }
}

void GetHotkeyStringW(WORD wHotkey, wchar_t *wszString)
{
  wchar_t wszKeyText[100];
  LONG lScan;

  wszString[0]='\0';

  if (wHotkey)
  {
    if (HIBYTE(wHotkey) & HOTKEYF_CONTROL)
      lstrcatW(wszString, L"Ctrl + ");
    if (HIBYTE(wHotkey) & HOTKEYF_SHIFT)
      lstrcatW(wszString, L"Shift + ");
    if (HIBYTE(wHotkey) & HOTKEYF_ALT)
      lstrcatW(wszString, L"Alt + ");
    lScan=MapVirtualKeyW(LOBYTE(wHotkey), 0) << 16;
    if (HIBYTE(wHotkey) & HOTKEYF_EXT) lScan|=0x01000000L;
    GetKeyNameTextW(lScan, wszKeyText, 100);
    lstrcatW(wszString, wszKeyText);
  }
}

BOOL ParsePluginNameA(char *pFullName, char *szPlugin, char *szFunction)
{
  char *pFunction;

  if ((pFunction=strchr(pFullName, ':')) && *(pFunction + 1) == ':')
  {
    if (szPlugin) lstrcpynA(szPlugin, pFullName, min(pFunction - pFullName + 1, MAX_PATH));
    if (szFunction) lstrcpynA(szFunction, pFunction + 2, MAX_PATH);
    return TRUE;
  }
  return FALSE;
}

BOOL ParsePluginNameW(wchar_t *wpFullName, wchar_t *wszPlugin, wchar_t *wszFunction)
{
  wchar_t *wpFunction;

  if ((wpFunction=wcschr(wpFullName, ':')) && *(wpFunction + 1) == ':')
  {
    if (wszPlugin) lstrcpynW(wszPlugin, wpFullName, min(wpFunction - wpFullName + 1, MAX_PATH));
    if (wszFunction) lstrcpynW(wszFunction, wpFunction + 2, MAX_PATH);
    return TRUE;
  }
  return FALSE;
}

int CallPluginA(PLUGINFUNCTIONA *lpPluginFunction, char *pFullName, BOOL bOnStart, LPARAM lParam, BOOL *lpbAutoLoad)
{
  char szPlugin[MAX_PATH];
  char szFunction[MAX_PATH];
  char szDLL[MAX_PATH];
  char szPluginWord[MAX_PATH];
  HMODULE hModule;
  PLUGINVERSION pv;
  PLUGINDATA pd;
  BOOL bInMemory=TRUE;
  BOOL bCalled=FALSE;
  int nWordLen;
  void (*PluginIDPtr)(PLUGINVERSION *);
  void (*PluginFunctionPtr)(PLUGINDATA *);

  if (lpbAutoLoad) *lpbAutoLoad=TRUE;

  if (pFullName)
  {
    if (ParsePluginNameA(pFullName, szPlugin, szFunction))
    {
      wsprintfA(szDLL, "%s\\AkelFiles\\Plugs\\%s.dll", szExeDir, szPlugin);
      nWordLen=API_LoadStringA(hLangLib, STR_PLUGIN, buf, BUFFER_SIZE);
      CharLowerBuffA(buf, nWordLen);
      wsprintfA(szPluginWord, "%s %s", szPlugin, buf);

      if (!(hModule=GetModuleHandleA(szDLL)))
      {
        if (hModule=LoadLibraryA(szDLL))
        {
          StackHandleIncrease(&hHandlesStack, hModule);
          bInMemory=FALSE;
        }
      }

      if (hModule)
      {
        if (PluginIDPtr=(void (*)(PLUGINVERSION *))GetProcAddress(hModule, "DllAkelPadID"))
        {
          pv.cb=sizeof(PLUGINVERSION);
          pv.hMainWnd=hMainWnd;
          pv.dwAkelDllVersion=MAKE_IDENTIFIER(-1, -1, -1, -1);
          pv.dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
          pv.dwExeMinVersion4x=MAKE_IDENTIFIER(-1, -1, -1, -1);
          pv.pPluginName=NULL;
          (*PluginIDPtr)(&pv);

          if (LOBYTE(pv.dwAkelDllVersion) == LOBYTE(AKELDLL) &&
              HIBYTE(pv.dwAkelDllVersion) == HIBYTE(AKELDLL))
          {
            if (pv.dwExeMinVersion3x != MAKE_IDENTIFIER(-1, -1, -1, -1))
            {
              if (VersionCompare(pv.dwExeMinVersion3x, dwExeVersion) <= 0)
              {
                if (PluginFunctionPtr=(void (*)(PLUGINDATA *))GetProcAddress(hModule, szFunction))
                {
                  pd.cb=sizeof(PLUGINDATA);
                  pd.pFunction=(unsigned char *)pFullName;
                  pd.hInstanceDLL=hModule;
                  pd.lpPluginFunction=lpPluginFunction;
                  pd.lpbAutoLoad=lpbAutoLoad;
                  pd.nUnload=UD_UNLOAD;
                  pd.bInMemory=bInMemory;
                  pd.bOnStart=bOnStart;
                  pd.lParam=lParam;
                  pd.pAkelDir=(unsigned char *)szExeDir;
                  pd.hInstanceEXE=hInstance;
                  pd.hPluginsStack=&hPluginsStack;
                  pd.hMainWnd=hMainWnd;
                  pd.hWndEdit=hWndEdit;
                  pd.hStatus=hStatus;
                  pd.hMdiClient=hMdiClient;
                  pd.hTab=hTab;
                  pd.hMainMenu=hMainMenu;
                  pd.hMenuRecentFiles=hMenuRecentFiles;
                  pd.hMenuLanguage=hMenuLanguage;
                  pd.hPopupMenu=hPopupMenu;
                  pd.hMainIcon=hMainIcon;
                  pd.hGlobalAccel=hGlobalAccel;
                  pd.hMainAccel=hMainAccel;
                  pd.bOldWindows=bOldWindows;
                  pd.bOldRichEdit=bOldRichEdit;
                  pd.bOldComctl32=bOldComctl32;
                  pd.bAkelEdit=bAkelEdit;
                  pd.bMDI=bMDI;
                  pd.nSaveSettings=nSaveSettings;
                  pd.pLangModule=(unsigned char *)szLangModule;
                  pd.wLangSystem=(WORD)dwLangSystem;

                  (*PluginFunctionPtr)(&pd);
                  SendMessage(hMainWnd, AKDN_DLLCALL, 0, (LPARAM)&pd);

                  if (lpbAutoLoad && bInMemory)
                    return UD_NONUNLOAD_UNCHANGE;
                  if ((pd.nUnload & UD_NONUNLOAD_ACTIVE) ||
                      (pd.nUnload & UD_NONUNLOAD_NONACTIVE) ||
                      (pd.nUnload & UD_NONUNLOAD_UNCHANGE))
                    return pd.nUnload;
                  bCalled=TRUE;
                }
                else
                {
                  API_LoadStringA(hLangLib, MSG_FUNCTION_NOT_FOUND, buf, BUFFER_SIZE);
                  wsprintfA(buf2, buf, szPlugin, szFunction, szDLL);
                  MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
                  if (bInMemory) return UD_FAILED;
                }
              }
              else
              {
                API_LoadStringA(hLangLib, MSG_UPDATE_PROGRAM, buf, BUFFER_SIZE);
                wsprintfA(buf2, buf, LOBYTE(pv.dwExeMinVersion3x), HIBYTE(pv.dwExeMinVersion3x), LOBYTE(HIWORD(pv.dwExeMinVersion3x)), HIBYTE(HIWORD(pv.dwExeMinVersion3x)),
                                     LOBYTE(dwExeVersion), HIBYTE(dwExeVersion), LOBYTE(HIWORD(dwExeVersion)), HIBYTE(HIWORD(dwExeVersion)));
                MessageBoxA(hMainWnd, buf2, szPluginWord, MB_OK|MB_ICONEXCLAMATION);
              }
            }
            else
            {
              API_LoadStringA(hLangLib, MSG_PROGRAM_IS_NOT_SUPPORTED, buf, BUFFER_SIZE);
              wsprintfA(buf2, buf, szDLL);
              MessageBoxA(hMainWnd, buf2, szPluginWord, MB_OK|MB_ICONEXCLAMATION);
            }
          }
          else
          {
            char szStr[MAX_PATH];

            if (VersionCompare(pv.dwAkelDllVersion, AKELDLL) < 0)
              lstrcpynA(szStr, szPluginWord, MAX_PATH);
            else
              lstrcpynA(szStr, "AkelPad", MAX_PATH);
            API_LoadStringA(hLangLib, MSG_UPDATE_PLUGIN, buf, BUFFER_SIZE);
            wsprintfA(buf2, buf, szStr,
                                 LOBYTE(AKELDLL), HIBYTE(AKELDLL), LOBYTE(HIWORD(AKELDLL)), HIBYTE(HIWORD(AKELDLL)),
                                 szPlugin,
                                 LOBYTE(pv.dwAkelDllVersion), HIBYTE(pv.dwAkelDllVersion), LOBYTE(HIWORD(pv.dwAkelDllVersion)), HIBYTE(HIWORD(pv.dwAkelDllVersion)));
            MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
          }
        }
        else
        {
          API_LoadStringA(hLangLib, MSG_PLUGIN_IS_NOT_SUPPORTED, buf, BUFFER_SIZE);
          wsprintfA(buf2, buf, szDLL);
          MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
        }

        if (FreeLibrary(hModule))
        {
          StackHandleDecrease(&hHandlesStack, hModule);
          if (bCalled) return pd.nUnload; //UD_UNLOAD
        }
      }
      else
      {
        API_LoadStringA(hLangLib, MSG_CANNOT_OPEN_FILE, buf, BUFFER_SIZE);
        wsprintfA(buf2, buf, szDLL);
        MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
      }
    }
  }
  return UD_FAILED;
}

int CallPluginW(PLUGINFUNCTIONW *lpPluginFunction, wchar_t *wpFullName, BOOL bOnStart, LPARAM lParam, BOOL *lpbAutoLoad)
{
  wchar_t wszPlugin[MAX_PATH];
  wchar_t wszFunction[MAX_PATH];
  wchar_t wszDLL[MAX_PATH];
  wchar_t wszPluginWord[MAX_PATH];
  char szFunction[MAX_PATH];
  HMODULE hModule;
  PLUGINVERSION pv;
  PLUGINDATA pd;
  BOOL bInMemory=TRUE;
  BOOL bCalled=FALSE;
  int nWordLen;
  void (*PluginIDPtr)(PLUGINVERSION *);
  void (*PluginFunctionPtr)(PLUGINDATA *);

  if (lpbAutoLoad) *lpbAutoLoad=TRUE;

  if (wpFullName)
  {
    if (ParsePluginNameW(wpFullName, wszPlugin, wszFunction))
    {
      WideCharToMultiByte(CP_ACP, 0, wszFunction, -1, szFunction, MAX_PATH, NULL, NULL);
      wsprintfW(wszDLL, L"%s\\AkelFiles\\Plugs\\%s.dll", wszExeDir, wszPlugin);
      nWordLen=API_LoadStringW(hLangLib, STR_PLUGIN, wbuf, BUFFER_SIZE);
      CharLowerBuffW(wbuf, nWordLen);
      wsprintfW(wszPluginWord, L"%s %s", wszPlugin, wbuf);

      if (!(hModule=GetModuleHandleW(wszDLL)))
      {
        if (hModule=LoadLibraryW(wszDLL))
        {
          StackHandleIncrease(&hHandlesStack, hModule);
          bInMemory=FALSE;
        }
      }

      if (hModule)
      {
        if (PluginIDPtr=(void (*)(PLUGINVERSION *))GetProcAddress(hModule, "DllAkelPadID"))
        {
          pv.cb=sizeof(PLUGINVERSION);
          pv.hMainWnd=hMainWnd;
          pv.dwAkelDllVersion=MAKE_IDENTIFIER(-1, -1, -1, -1);
          pv.dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
          pv.dwExeMinVersion4x=MAKE_IDENTIFIER(-1, -1, -1, -1);
          pv.pPluginName=NULL;
          (*PluginIDPtr)(&pv);

          if (LOBYTE(pv.dwAkelDllVersion) == LOBYTE(AKELDLL) &&
              HIBYTE(pv.dwAkelDllVersion) == HIBYTE(AKELDLL))
          {
            if (pv.dwExeMinVersion3x != MAKE_IDENTIFIER(-1, -1, -1, -1))
            {
              if (VersionCompare(pv.dwExeMinVersion3x, dwExeVersion) <= 0)
              {
                if (PluginFunctionPtr=(void (*)(PLUGINDATA *))GetProcAddress(hModule, szFunction))
                {
                  pd.cb=sizeof(PLUGINDATA);
                  pd.pFunction=(unsigned char *)wpFullName;
                  pd.hInstanceDLL=hModule;
                  pd.lpPluginFunction=lpPluginFunction;
                  pd.lpbAutoLoad=lpbAutoLoad;
                  pd.nUnload=UD_UNLOAD;
                  pd.bInMemory=bInMemory;
                  pd.bOnStart=bOnStart;
                  pd.lParam=lParam;
                  pd.pAkelDir=(unsigned char *)wszExeDir;
                  pd.hInstanceEXE=hInstance;
                  pd.hPluginsStack=&hPluginsStack;
                  pd.hMainWnd=hMainWnd;
                  pd.hWndEdit=hWndEdit;
                  pd.hStatus=hStatus;
                  pd.hMdiClient=hMdiClient;
                  pd.hTab=hTab;
                  pd.hMainMenu=hMainMenu;
                  pd.hMenuRecentFiles=hMenuRecentFiles;
                  pd.hMenuLanguage=hMenuLanguage;
                  pd.hPopupMenu=hPopupMenu;
                  pd.hMainIcon=hMainIcon;
                  pd.hGlobalAccel=hGlobalAccel;
                  pd.hMainAccel=hMainAccel;
                  pd.bOldWindows=bOldWindows;
                  pd.bOldRichEdit=bOldRichEdit;
                  pd.bOldComctl32=bOldComctl32;
                  pd.bAkelEdit=bAkelEdit;
                  pd.bMDI=bMDI;
                  pd.nSaveSettings=nSaveSettings;
                  pd.pLangModule=(unsigned char *)wszLangModule;
                  pd.wLangSystem=(WORD)dwLangSystem;

                  (*PluginFunctionPtr)(&pd);
                  SendMessage(hMainWnd, AKDN_DLLCALL, 0, (LPARAM)&pd);

                  if (lpbAutoLoad && bInMemory)
                    return UD_NONUNLOAD_UNCHANGE;
                  if ((pd.nUnload & UD_NONUNLOAD_ACTIVE) ||
                      (pd.nUnload & UD_NONUNLOAD_NONACTIVE) ||
                      (pd.nUnload & UD_NONUNLOAD_UNCHANGE))
                    return pd.nUnload;
                  bCalled=TRUE;
                }
                else
                {
                  API_LoadStringW(hLangLib, MSG_FUNCTION_NOT_FOUND, wbuf, BUFFER_SIZE);
                  wsprintfW(wbuf2, wbuf, wszPlugin, wszFunction, wszDLL);
                  MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
                  if (bInMemory) return UD_FAILED;
                }
              }
              else
              {
                API_LoadStringW(hLangLib, MSG_UPDATE_PROGRAM, wbuf, BUFFER_SIZE);
                wsprintfW(wbuf2, wbuf, LOBYTE(pv.dwExeMinVersion3x), HIBYTE(pv.dwExeMinVersion3x), LOBYTE(HIWORD(pv.dwExeMinVersion3x)), HIBYTE(HIWORD(pv.dwExeMinVersion3x)),
                                       LOBYTE(dwExeVersion), HIBYTE(dwExeVersion), LOBYTE(HIWORD(dwExeVersion)), HIBYTE(HIWORD(dwExeVersion)));
                MessageBoxW(hMainWnd, wbuf2, wszPluginWord, MB_OK|MB_ICONEXCLAMATION);
              }
            }
            else
            {
              API_LoadStringW(hLangLib, MSG_PROGRAM_IS_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
              wsprintfW(wbuf2, wbuf, wszDLL);
              MessageBoxW(hMainWnd, wbuf2, wszPluginWord, MB_OK|MB_ICONEXCLAMATION);
            }
          }
          else
          {
            wchar_t wszStr[MAX_PATH];

            if (VersionCompare(pv.dwAkelDllVersion, AKELDLL) < 0)
              lstrcpynW(wszStr, wszPluginWord, MAX_PATH);
            else
              lstrcpynW(wszStr, L"AkelPad", MAX_PATH);
            API_LoadStringW(hLangLib, MSG_UPDATE_PLUGIN, wbuf, BUFFER_SIZE);
            wsprintfW(wbuf2, wbuf, wszStr,
                                   LOBYTE(AKELDLL), HIBYTE(AKELDLL), LOBYTE(HIWORD(AKELDLL)), HIBYTE(HIWORD(AKELDLL)),
                                   wszPlugin,
                                   LOBYTE(pv.dwAkelDllVersion), HIBYTE(pv.dwAkelDllVersion), LOBYTE(HIWORD(pv.dwAkelDllVersion)), HIBYTE(HIWORD(pv.dwAkelDllVersion)));
            MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
          }
        }
        else
        {
          API_LoadStringW(hLangLib, MSG_PLUGIN_IS_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
          wsprintfW(wbuf2, wbuf, wszDLL);
          MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
        }

        if (FreeLibrary(hModule))
        {
          StackHandleDecrease(&hHandlesStack, hModule);
          if (bCalled) return pd.nUnload; //UD_UNLOAD
        }
      }
      else
      {
        API_LoadStringW(hLangLib, MSG_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
        wsprintfW(wbuf2, wbuf, wszDLL);
        MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
      }
    }
  }
  return UD_FAILED;
}

void CallPluginsOnStartA(HSTACK *hStack)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->first;
  PLUGINFUNCTIONA *pfTmp;
  PLUGINCALLSENDA pcs;

  while (pfElement)
  {
    if (pfElement->bOnStart)
    {
      pcs.pFunction=pfElement->szFunction;
      pcs.bOnStart=TRUE;
      pcs.lParam=0;
      pcs.lpbAutoLoad=NULL;

      if (CallPluginReceiveSendA(&pcs) == UD_FAILED)
      {
        pfTmp=pfElement->next;
        StackPluginDelete(hStack, pfElement);
        pfElement=pfTmp;
        continue;
      }
    }
    pfElement=pfElement->next;
  }
}

void CallPluginsOnStartW(HSTACK *hStack)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->first;
  PLUGINFUNCTIONW *pfTmp;
  PLUGINCALLSENDW pcs;

  while (pfElement)
  {
    if (pfElement->bOnStart)
    {
      pcs.pFunction=pfElement->szFunction;
      pcs.bOnStart=TRUE;
      pcs.lParam=0;
      pcs.lpbAutoLoad=NULL;

      if (CallPluginReceiveSendW(&pcs) == UD_FAILED)
      {
        pfTmp=pfElement->next;
        StackPluginDelete(hStack, pfElement);
        pfElement=pfTmp;
        continue;
      }
    }
    pfElement=pfElement->next;
  }
}

int CallPluginReceiveSendA(PLUGINCALLSENDA *pcs)
{
  PLUGINFUNCTIONA *pfElement;
  int nResult=UD_FAILED;

  if (pcs)
  {
    pfElement=StackPluginFindA(&hPluginsStack, pcs->pFunction);

    if (pfElement && pfElement->PluginProc)
    {
      if ((pfElement->PluginProc)(pfElement->lpParameter))
        nResult=UD_NONUNLOAD_UNCHANGE;
      else
        nResult=UD_NONUNLOAD_UNCHANGE|UD_HOTKEY_DODEFAULT;
    }
    else
    {
      nResult=CallPluginA(pfElement, pcs->pFunction, pcs->bOnStart, pcs->lParam, pcs->lpbAutoLoad);

      if (nResult != UD_FAILED)
      {
        if (!(nResult & UD_NONUNLOAD_UNCHANGE))
        {
          if (nResult & UD_NONUNLOAD_ACTIVE)
          {
            if (pfElement)
              pfElement->bRunning=TRUE;
            else
              StackPluginAddA(&hPluginsStack, pcs->pFunction, lstrlenA(pcs->pFunction), 0, FALSE, TRUE, NULL, NULL);
          }
          else
          {
            if (pfElement)
            {
              if (pfElement->wHotkey || pfElement->bOnStart)
                pfElement->bRunning=FALSE;
              else
                StackPluginDelete(&hPluginsStack, pfElement);
            }
          }
        }
      }
    }
  }
  return nResult;
}

int CallPluginReceiveSendW(PLUGINCALLSENDW *pcs)
{
  PLUGINFUNCTIONW *pfElement;
  int nResult=UD_FAILED;

  if (pcs)
  {
    pfElement=StackPluginFindW(&hPluginsStack, pcs->pFunction);

    if (pfElement && pfElement->PluginProc)
    {
      if ((pfElement->PluginProc)(pfElement->lpParameter))
        nResult=UD_NONUNLOAD_UNCHANGE;
      else
        nResult=UD_NONUNLOAD_UNCHANGE|UD_HOTKEY_DODEFAULT;
    }
    else
    {
      nResult=CallPluginW(pfElement, pcs->pFunction, pcs->bOnStart, pcs->lParam, pcs->lpbAutoLoad);

      if (nResult != UD_FAILED)
      {
        if (!(nResult & UD_NONUNLOAD_UNCHANGE))
        {
          if (nResult & UD_NONUNLOAD_ACTIVE)
          {
            if (pfElement)
              pfElement->bRunning=TRUE;
            else
              StackPluginAddW(&hPluginsStack, pcs->pFunction, lstrlenW(pcs->pFunction), 0, FALSE, TRUE, NULL, NULL);
          }
          else
          {
            if (pfElement)
            {
              if (pfElement->wHotkey || pfElement->bOnStart)
                pfElement->bRunning=FALSE;
              else
                StackPluginDelete(&hPluginsStack, pfElement);
            }
          }
        }
      }
    }
  }
  return nResult;
}

void CallPluginReceivePostA(PLUGINCALLPOSTA *pcp)
{
  PLUGINFUNCTIONA *pfElement;
  int nResult;

  if (pcp)
  {
    pfElement=StackPluginFindA(&hPluginsStack, pcp->szFunction);

    if (pfElement && pfElement->PluginProc)
    {
      (pfElement->PluginProc)(pfElement->lpParameter);
    }
    else
    {
      nResult=CallPluginA(pfElement, pcp->szFunction, pcp->bOnStart, pcp->lParam, NULL);

      if (nResult != UD_FAILED)
      {
        if (!(nResult & UD_NONUNLOAD_UNCHANGE))
        {
          if (nResult & UD_NONUNLOAD_ACTIVE)
          {
            if (pfElement)
              pfElement->bRunning=TRUE;
            else
              StackPluginAddA(&hPluginsStack, pcp->szFunction, lstrlenA(pcp->szFunction), 0, FALSE, TRUE, NULL, NULL);
          }
          else
          {
            if (pfElement)
            {
              if (pfElement->wHotkey || pfElement->bOnStart)
                pfElement->bRunning=FALSE;
              else
                StackPluginDelete(&hPluginsStack, pfElement);
            }
          }
        }
      }
    }
    GlobalFree((HGLOBAL)pcp);
  }
}

void CallPluginReceivePostW(PLUGINCALLPOSTW *pcp)
{
  PLUGINFUNCTIONW *pfElement;
  int nResult;

  if (pcp)
  {
    pfElement=StackPluginFindW(&hPluginsStack, pcp->szFunction);

    if (pfElement && pfElement->PluginProc)
    {
      (pfElement->PluginProc)(pfElement->lpParameter);
    }
    else
    {
      nResult=CallPluginW(pfElement, pcp->szFunction, pcp->bOnStart, pcp->lParam, NULL);

      if (nResult != UD_FAILED)
      {
        if (!(nResult & UD_NONUNLOAD_UNCHANGE))
        {
          if (nResult & UD_NONUNLOAD_ACTIVE)
          {
            if (pfElement)
              pfElement->bRunning=TRUE;
            else
              StackPluginAddW(&hPluginsStack, pcp->szFunction, lstrlenW(pcp->szFunction), 0, FALSE, TRUE, NULL, NULL);
          }
          else
          {
            if (pfElement)
            {
              if (pfElement->wHotkey || pfElement->bOnStart)
                pfElement->bRunning=FALSE;
              else
                StackPluginDelete(&hPluginsStack, pfElement);
            }
          }
        }
      }
    }
    GlobalFree((HGLOBAL)pcp);
  }
}

BOOL GetExportNames(HMODULE hInstance, EXPORTNAMESPROC lpExportNamesProc, LPARAM lParam)
{
  PIMAGE_DOS_HEADER pDOSHead;
  PIMAGE_NT_HEADERS pPEHeader;
  PIMAGE_EXPORT_DIRECTORY pImportDesc;
  unsigned long *pchName;
  char *pName;
  DWORD i;

  pDOSHead=(PIMAGE_DOS_HEADER)hInstance;
  if (pDOSHead->e_magic != IMAGE_DOS_SIGNATURE)
    return FALSE;

  pPEHeader=(PIMAGE_NT_HEADERS)((DWORD)hInstance + pDOSHead->e_lfanew);
  if (pPEHeader->Signature != IMAGE_NT_SIGNATURE)
    return FALSE;

  pImportDesc=(PIMAGE_EXPORT_DIRECTORY)((DWORD)hInstance + pPEHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
  if (!pImportDesc->AddressOfNames)
    return FALSE;

  pchName=(unsigned long *)((DWORD)hInstance + pImportDesc->AddressOfNames);

  for (i=0; i < pImportDesc->NumberOfNames; ++i)
  {
    pName=(char *)((DWORD)hInstance + pchName[i]);
    if ((*lpExportNamesProc)(pName, lParam) == FALSE) return TRUE;
  }
  return TRUE;
}


//// Stack

int StackProcGet(HSTACK *hStack, int nIndex, WNDPROCDATA **ProcData)
{
  return StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)ProcData, nIndex);
}

int StackProcSet(HSTACK *hStack, WNDPROC NewProc, WNDPROCDATA **NewProcData, WNDPROC *FirstProc)
{
  if (!NewProcData || !*NewProcData)
  {
    WNDPROCDATA *lpElement;

    if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, 1, sizeof(WNDPROCDATA)))
    {
      lpElement->CurProc=NewProc;
      if (lpElement->next)
      {
        lpElement->next->PrevProc=NewProc;
        lpElement->NextProc=lpElement->next->CurProc;
      }
      if (NewProcData) *NewProcData=lpElement;
    }
  }
  else
  {
    if (!NewProc)
    {
      if ((*NewProcData)->prev) (*NewProcData)->prev->NextProc=(*NewProcData)->NextProc;
      if ((*NewProcData)->next) (*NewProcData)->next->PrevProc=(*NewProcData)->PrevProc;
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)*NewProcData);
      *NewProcData=NULL;
    }
    else
    {
      (*NewProcData)->CurProc=NewProc;
      if ((*NewProcData)->prev) (*NewProcData)->prev->NextProc=NewProc;
      if ((*NewProcData)->next) (*NewProcData)->next->PrevProc=NewProc;
    }
  }
  if (FirstProc)
  {
    if (hStack->first)
      *FirstProc=((WNDPROCDATA *)(hStack->first))->CurProc;
    else
      *FirstProc=NULL;
  }
  return 0;
}

void StackProcFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

PLUGINHANDLE* StackHandleIncrease(HSTACK *hStack, HMODULE hModule)
{
  PLUGINHANDLE *phElement=(PLUGINHANDLE *)hStack->first;

  while (phElement)
  {
    if (phElement->hModule == hModule)
    {
      phElement->nCount+=1;
      return phElement;
    }
    phElement=phElement->next;
  }

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&phElement, -1, sizeof(PLUGINHANDLE)))
  {
    phElement->hModule=hModule;
    phElement->nCount=1;
  }
  return phElement;
}

PLUGINHANDLE* StackHandleDecrease(HSTACK *hStack, HMODULE hModule)
{
  PLUGINHANDLE *phElement=(PLUGINHANDLE *)hStack->first;

  while (phElement)
  {
    if (phElement->hModule == hModule)
    {
      phElement->nCount-=1;
      if (phElement->nCount <= 0)
      {
        StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)phElement);
        return NULL;
      }
      return phElement;
    }
    phElement=phElement->next;
  }
  return NULL;
}

void StackHandleFree(HSTACK *hStack)
{
  PLUGINHANDLE *phElement=(PLUGINHANDLE *)hStack->first;

  while (phElement)
  {
    while (phElement->nCount > 0 && FreeLibrary(phElement->hModule))
    {
      phElement->nCount-=1;
    }
    phElement=phElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

DOCK* StackDockAdd(HDOCK *hDocks, DOCK *dkData)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.first;
  int nIndex=1;

  while (dkElement)
  {
    if (dkData->nSide == dkElement->nSide)
    {
      if (dkData->nSide == DKS_LEFT)
      {
        if (dkData->rcSize.left < dkElement->rcSize.left)
          break;
      }
      else if (dkData->nSide == DKS_RIGHT)
      {
        if (dkData->rcSize.left > dkElement->rcSize.left)
          break;
      }
      else if (dkData->nSide == DKS_TOP)
      {
        if (dkData->rcSize.top < dkElement->rcSize.top)
          break;
      }
      else if (dkData->nSide == DKS_BOTTOM)
      {
        if (dkData->rcSize.top > dkElement->rcSize.top)
          break;
      }
    }
    else if (dkData->nSide < dkElement->nSide)
      break;

    ++nIndex;

    dkElement=dkElement->next;
  }

  if (!StackInsertIndex((stack **)&hDocks->hStack.first, (stack **)&hDocks->hStack.last, (stack **)&dkElement, nIndex, sizeof(DOCK)))
  {
    dkElement->dwFlags=dkData->dwFlags;
    dkElement->hWnd=dkData->hWnd;
    dkElement->nSide=dkData->nSide;
    dkElement->rcSize=dkData->rcSize;
    dkElement->rcDragDrop=dkData->rcDragDrop;
  }
  return dkElement;
}

int DockSetSide(HDOCK *hDocks, DOCK *dkData, int nSide)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.first;
  int nIndex=1;

  while (dkElement)
  {
    if (nSide == dkElement->nSide)
    {
      if (dkData != dkElement)
      {
        if (nSide == DKS_LEFT)
        {
          if (dkData->rcSize.left <= dkElement->rcSize.left)
            break;
        }
        if (nSide == DKS_RIGHT)
        {
          if (dkData->rcSize.left >= dkElement->rcSize.left)
            break;
        }
        else if (nSide == DKS_TOP)
        {
          if (dkData->rcSize.top <= dkElement->rcSize.top)
            break;
        }
        else if (nSide == DKS_BOTTOM)
        {
          if (dkData->rcSize.top >= dkElement->rcSize.top)
            break;
        }
      }
    }
    else
    {
      if (nSide < dkElement->nSide)
        break;
      if (dkData == dkElement)
        --nIndex;
    }
    ++nIndex;

    dkElement=dkElement->next;
  }
  dkData->nSide=nSide;

  return StackMoveIndex((stack **)&hDocks->hStack.first, (stack **)&hDocks->hStack.last, (stack *)dkData, nIndex);
}

DOCK* StackDockFindWindow(HDOCK *hDocks, HWND hWnd, BOOL bChild)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.first;

  if (hWnd)
  {
    while (dkElement)
    {
      if (dkElement->hWnd)
      {
        if (dkElement->hWnd == hWnd || (bChild && IsChild(dkElement->hWnd, hWnd)))
          return dkElement;
      }
      dkElement=dkElement->next;
    }
  }
  return NULL;
}

HWND StackDockNextWindow(HDOCK *hDocks, DOCK *dkData, BOOL bPrevious)
{
  if (bPrevious)
  {
    if (!dkData)
      dkData=(DOCK *)hDocks->hStack.last;
    else
      dkData=dkData->prev;

    while (dkData)
    {
      if (dkData->hWnd && !(dkData->dwFlags & DKF_HIDDEN))
        return dkData->hWnd;

      dkData=dkData->prev;
    }
  }
  else
  {
    if (!dkData)
      dkData=(DOCK *)hDocks->hStack.first;
    else
      dkData=dkData->next;

    while (dkData)
    {
      if (dkData->hWnd && !(dkData->dwFlags & DKF_HIDDEN))
        return dkData->hWnd;

      dkData=dkData->next;
    }
  }
  return NULL;
}

DOCK* StackDockFromPoint(HDOCK *hDocks, POINT *pt)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.first;
  RECT rc;

  while (dkElement)
  {
    if (dkElement->hWnd)
    {
      if (GetWindowRect(dkElement->hWnd, &rc))
      {
        if (PtInRect(&rc, *pt))
          return dkElement;
      }
    }
    dkElement=dkElement->next;
  }
  return NULL;
}

void StackDockSize(HDOCK *hDocks, int nSide, NSIZE *ns)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.first;
  RECT rcDock;

  while (dkElement)
  {
    if (dkElement->nSide > nSide) break;

    if (dkElement->nSide == nSide)
    {
      if (!(dkElement->dwFlags & DKF_HIDDEN))
      {
        if (dkElement->rcSize.right || dkElement->rcSize.bottom)
        {
          rcDock.left=ns->rcCurrent.left;
          rcDock.top=ns->rcCurrent.top;
          rcDock.right=dkElement->rcSize.right;
          rcDock.bottom=dkElement->rcSize.bottom;

          if (dkElement->nSide == DKS_LEFT)
          {
            if (ns->rcCurrent.right - rcDock.right < DOCK_MAINMIN_X)
              rcDock.right=ns->rcCurrent.right - DOCK_MAINMIN_X;
            else if (ns->rcCurrent.right >= DOCK_MAINMIN_X + DOCK_BORDER_2X)
              rcDock.right=max(rcDock.right, DOCK_BORDER_2X);

            ns->rcCurrent.left+=rcDock.right;
            ns->rcCurrent.right-=rcDock.right;
          }
          else if (dkElement->nSide == DKS_TOP)
          {
            if (ns->rcCurrent.bottom - rcDock.bottom < DOCK_MAINMIN_Y)
              rcDock.bottom=ns->rcCurrent.bottom - DOCK_MAINMIN_Y;
            else if (ns->rcCurrent.bottom >= DOCK_MAINMIN_Y + DOCK_BORDER_2X)
              rcDock.bottom=max(rcDock.bottom, DOCK_BORDER_2X);

            ns->rcCurrent.top+=rcDock.bottom;
            ns->rcCurrent.bottom-=rcDock.bottom;
          }
          else if (dkElement->nSide == DKS_RIGHT)
          {
            if (ns->rcCurrent.right - rcDock.right < DOCK_MAINMIN_X)
              rcDock.right=ns->rcCurrent.right - DOCK_MAINMIN_X;
            else if (ns->rcCurrent.right >= DOCK_MAINMIN_X + DOCK_BORDER_2X)
              rcDock.right=max(rcDock.right, DOCK_BORDER_2X);

            ns->rcCurrent.right-=rcDock.right;
            rcDock.left=ns->rcCurrent.left + ns->rcCurrent.right;
          }
          else if (dkElement->nSide == DKS_BOTTOM)
          {
            if (ns->rcCurrent.bottom - rcDock.bottom < DOCK_MAINMIN_Y)
              rcDock.bottom=ns->rcCurrent.bottom - DOCK_MAINMIN_Y;
            else if (ns->rcCurrent.bottom >= DOCK_MAINMIN_Y + DOCK_BORDER_2X)
              rcDock.bottom=max(rcDock.bottom, DOCK_BORDER_2X);

            ns->rcCurrent.bottom-=rcDock.bottom;
            rcDock.top=ns->rcCurrent.top + ns->rcCurrent.bottom;
          }

          if (dkElement->hWnd)
          {
            if (dkElement->nSide == DKS_LEFT ||
                dkElement->nSide == DKS_RIGHT)
            {
              if (hDocksStack.nSizingSide) dkElement->rcSize.left=rcDock.left;
              MoveWindow(dkElement->hWnd, rcDock.left, ns->rcCurrent.top, rcDock.right, ns->rcCurrent.bottom, TRUE);
            }
            else if (dkElement->nSide == DKS_TOP ||
                     dkElement->nSide == DKS_BOTTOM)
            {
              if (hDocksStack.nSizingSide) dkElement->rcSize.top=rcDock.top;
              MoveWindow(dkElement->hWnd, ns->rcCurrent.left, rcDock.top, ns->rcCurrent.right, rcDock.bottom, TRUE);
            }
          }
        }
      }
    }

    dkElement=dkElement->next;
  }
}

void StackDockDelete(HDOCK *hDocks, DOCK *dkData)
{
  StackDelete((stack **)&hDocks->hStack.first, (stack **)&hDocks->hStack.last, (stack *)dkData);
}

void StackDockFree(HDOCK *hDocks)
{
  StackClear((stack **)&hDocks->hStack.first, (stack **)&hDocks->hStack.last);
}

PLUGINFUNCTIONA* StackPluginFindA(HSTACK *hStack, char *pString)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->first;
  int nStringLen;

  if (!pString || !(nStringLen=lstrlenA(pString)))
    return NULL;

  while (pfElement)
  {
    if (pfElement->nFunctionLen == nStringLen)
    {
      if (!lstrcmpiA(pfElement->szFunction, pString))
        break;
    }
    pfElement=pfElement->next;
  }
  return pfElement;
}

PLUGINFUNCTIONW* StackPluginFindW(HSTACK *hStack, wchar_t *wpString)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->first;
  int nStringLen;

  if (!wpString || !(nStringLen=lstrlenW(wpString)))
    return NULL;

  while (pfElement)
  {
    if (pfElement->nFunctionLen == nStringLen)
    {
      if (!lstrcmpiW(pfElement->szFunction, wpString))
        break;
    }
    pfElement=pfElement->next;
  }
  return pfElement;
}

PLUGINFUNCTIONA* StackHotkeyFindA(HSTACK *hStack, WORD wHotkey)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->first;

  while (pfElement)
  {
    if (pfElement->wHotkey == wHotkey)
        break;
    pfElement=pfElement->next;
  }
  return pfElement;
}

PLUGINFUNCTIONW* StackHotkeyFindW(HSTACK *hStack, WORD wHotkey)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->first;

  while (pfElement)
  {
    if (pfElement->wHotkey == wHotkey)
        break;
    pfElement=pfElement->next;
  }
  return pfElement;
}

PLUGINFUNCTIONA* StackPluginAddA(HSTACK *hStack, char *pString, int nStringLen, WORD wHotkey, BOOL bOnStart, BOOL bRunning, PLUGINPROC PluginProc, void *lpParameter)
{
  PLUGINFUNCTIONA *pfElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&pfElement, -1, sizeof(PLUGINFUNCTIONA)))
  {
    lstrcpynA(pfElement->szFunction, pString, MAX_PATH);
    pfElement->nFunctionLen=nStringLen;
    pfElement->wHotkey=wHotkey;
    pfElement->bOnStart=bOnStart;
    pfElement->bRunning=bRunning;
    pfElement->PluginProc=PluginProc;
    pfElement->lpParameter=lpParameter;
  }
  return pfElement;
}

PLUGINFUNCTIONW* StackPluginAddW(HSTACK *hStack, wchar_t *wpString, int nStringLen, WORD wHotkey, BOOL bOnStart, BOOL bRunning, PLUGINPROC PluginProc, void *lpParameter)
{
  PLUGINFUNCTIONW *pfElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&pfElement, -1, sizeof(PLUGINFUNCTIONW)))
  {
    lstrcpynW(pfElement->szFunction, wpString, MAX_PATH);
    pfElement->nFunctionLen=nStringLen;
    pfElement->wHotkey=wHotkey;
    pfElement->bOnStart=bOnStart;
    pfElement->bRunning=bRunning;
    pfElement->PluginProc=PluginProc;
    pfElement->lpParameter=lpParameter;
  }
  return pfElement;
}

void StackPluginDelete(HSTACK *hStack, void *lpElement)
{
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement);
}

BOOL StackPluginSaveA(HSTACK *hStack, BOOL bCleanOld)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->first;
  HKEY hKey;
  HINISECTION *lpIniSection;
  DWORD dwHotkey;
  BOOL bResult=FALSE;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (bCleanOld)
      RegClearKeyA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Plugs");

    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Plugs", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!OpenIniA(&hIniStack, szIniFile, TRUE))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionA(&hIniStack, "Plugs", lstrlenA("Plugs")))
        StackDeleteIniSection(&hIniStack, lpIniSection, TRUE);
    }
  }

  while (pfElement)
  {
    if (IsMainFunctionA(pfElement->szFunction))
    {
      if (!bCleanOld || (pfElement->wHotkey || pfElement->bOnStart))
      {
        dwHotkey=MAKELONG(pfElement->wHotkey, pfElement->bOnStart);

        if (nSaveSettings == SS_REGISTRY)
        {
          if (RegSetValueExA(hKey, pfElement->szFunction, 0, REG_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)) != ERROR_SUCCESS)
            break;
        }
        else
        {
          if (!IniSetValueA(&hIniStack, "Plugs", pfElement->szFunction, INI_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)))
            break;
        }
      }
    }
    pfElement=pfElement->next;
  }

  if (nSaveSettings == SS_REGISTRY)
  {
    RegCloseKey(hKey);
    bResult=TRUE;
  }
  else
  {
    bResult=SaveIniA(&hIniStack, szIniFile);
    StackFreeIni(&hIniStack);
  }
  return bResult;
}

BOOL StackPluginSaveW(HSTACK *hStack, BOOL bCleanOld)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->first;
  HKEY hKey;
  HINISECTION *lpIniSection;
  DWORD dwHotkey;
  BOOL bResult=FALSE;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (bCleanOld)
      RegClearKeyW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Plugs");

    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Plugs", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!OpenIniW(&hIniStack, wszIniFile, TRUE))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionW(&hIniStack, L"Plugs", lstrlenW(L"Plugs")))
        StackDeleteIniSection(&hIniStack, lpIniSection, TRUE);
    }
  }

  while (pfElement)
  {
    if (IsMainFunctionW(pfElement->szFunction))
    {
      if (!bCleanOld || (pfElement->wHotkey || pfElement->bOnStart))
      {
        dwHotkey=MAKELONG(pfElement->wHotkey, pfElement->bOnStart);

        if (nSaveSettings == SS_REGISTRY)
        {
          if (RegSetValueExW(hKey, pfElement->szFunction, 0, REG_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)) != ERROR_SUCCESS)
            break;
        }
        else
        {
          if (!IniSetValueW(&hIniStack, L"Plugs", pfElement->szFunction, INI_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)))
            break;
        }
      }
    }
    pfElement=pfElement->next;
  }

  if (nSaveSettings == SS_REGISTRY)
  {
    RegCloseKey(hKey);
    bResult=TRUE;
  }
  else
  {
    bResult=SaveIniW(&hIniStack, wszIniFile);
    StackFreeIni(&hIniStack);
  }
  return bResult;
}

void StackPluginCleanUpA(HSTACK *hStack, BOOL bDeleteNonExistentDLL)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->first;
  PLUGINFUNCTIONA *pfElement2;
  char szDLL[MAX_PATH];
  char szPlugin[MAX_PATH];

  while (pfElement)
  {
    pfElement2=pfElement->next;

    if (!pfElement->wHotkey && !pfElement->bOnStart && !pfElement->bRunning)
    {
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)pfElement);
      pfElement=NULL;
    }
    if (bDeleteNonExistentDLL && pfElement)
    {
      if (ParsePluginNameA(pfElement->szFunction, szPlugin, NULL))
      {
        wsprintfA(szDLL, "%s\\AkelFiles\\Plugs\\%s.dll", szExeDir, szPlugin);

        if (!FileExistsA(szDLL))
        {
          StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)pfElement);
          pfElement=NULL;
        }
      }
    }
    pfElement=pfElement2;
  }
}

void StackPluginCleanUpW(HSTACK *hStack, BOOL bDeleteNonExistentDLL)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->first;
  PLUGINFUNCTIONW *pfElement2;
  wchar_t wszDLL[MAX_PATH];
  wchar_t wszPlugin[MAX_PATH];

  while (pfElement)
  {
    pfElement2=pfElement->next;

    if (!pfElement->wHotkey && !pfElement->bOnStart && !pfElement->bRunning)
    {
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)pfElement);
      pfElement=NULL;
    }
    if (bDeleteNonExistentDLL && pfElement)
    {
      if (ParsePluginNameW(pfElement->szFunction, wszPlugin, NULL))
      {
        wsprintfW(wszDLL, L"%s\\AkelFiles\\Plugs\\%s.dll", wszExeDir, wszPlugin);

        if (!FileExistsW(wszDLL))
        {
          StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)pfElement);
          pfElement=NULL;
        }
      }
    }
    pfElement=pfElement2;
  }
}

void StackPluginFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}


BOOL IsMainFunctionA(char *pFunction)
{
  int nCount=0;
  int i;

  for (i=0; pFunction[i]; ++i)
  {
    if (pFunction[i] == ':' && pFunction[i + 1] == ':')
    {
      if (++nCount == 2) return FALSE;
    }
  }
  if (nCount == 1) return TRUE;

  return FALSE;
}

BOOL IsMainFunctionW(wchar_t *wpFunction)
{
  int nCount=0;
  int i;

  for (i=0; wpFunction[i]; ++i)
  {
    if (wpFunction[i] == ':' && wpFunction[i + 1] == ':')
    {
      if (++nCount == 2) return FALSE;
    }
  }
  if (nCount == 1) return TRUE;

  return FALSE;
}

BOOL TranslateDialogA(HDOCK *hDocks, LPMSG lpMsg)
{
  if (hDlgModeless && IsDialogMessageA(hDlgModeless, lpMsg))
  {
    if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
    {
      SendMessageA(hDlgModeless, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
    }
    return TRUE;
  }
  else
  {
    DOCK *dkElement=(DOCK *)hDocks->hStack.first;

    while (dkElement)
    {
      if (!(dkElement->dwFlags & DKF_OWNTHREAD))
      {
        if (dkElement->hWnd && IsDialogMessageA(dkElement->hWnd, lpMsg))
        {
          if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
          {
            SendMessageA(dkElement->hWnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
          }
          return TRUE;
        }
      }
      dkElement=dkElement->next;
    }
  }
  return FALSE;
}

BOOL TranslateDialogW(HDOCK *hDocks, LPMSG lpMsg)
{
  if (hDlgModeless && IsDialogMessageW(hDlgModeless, lpMsg))
  {
    if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
    {
      SendMessageW(hDlgModeless, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
    }
    return TRUE;
  }
  else
  {
    DOCK *dkElement=(DOCK *)hDocks->hStack.first;

    while (dkElement)
    {
      if (!(dkElement->dwFlags & DKF_OWNTHREAD))
      {
        if (dkElement->hWnd && IsDialogMessageW(dkElement->hWnd, lpMsg))
        {
          if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
          {
            SendMessageW(dkElement->hWnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
          }
          return TRUE;
        }
      }
      dkElement=dkElement->next;
    }
  }
  return FALSE;
}

BOOL TranslatePluginA(LPMSG lpMsg)
{
  if (lpMsg->message == AKD_POSTMESSAGE)
  {
    POSTMESSAGE *pm=(POSTMESSAGE *)lpMsg->lParam;

    if (pm)
    {
      SendMessageA(pm->hWnd, pm->uMsg, pm->wParam, pm->lParam);
      GlobalFree((HGLOBAL)pm);
    }
    return TRUE;
  }
  else if (lpMsg->message == AKD_CALLPROC)
  {
    PLUGINPROC lpProcedure=(PLUGINPROC)lpMsg->wParam;
    void *lpParameter=(void *)lpMsg->lParam;

    (lpProcedure)(lpParameter);
    return TRUE;
  }
  else if (lpMsg->message == AKD_DLLCALL)
  {
    PLUGINCALLPOSTA *pcp=(PLUGINCALLPOSTA *)lpMsg->lParam;

    CallPluginReceivePostA(pcp);
    return TRUE;
  }
  else if (lpMsg->message == AKD_DLLUNLOAD)
  {
    HMODULE hInstanceDLL=(HMODULE)lpMsg->wParam;
    HANDLE hThread=(HANDLE)lpMsg->lParam;
    char szPluginName[MAX_PATH];

    if (hInstanceDLL)
    {
      if (hThread)
      {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
      }

      if (GetModuleFileNameA(hInstanceDLL, buf, BUFFER_SIZE))
      {
        GetBaseNameA(buf, szPluginName, MAX_PATH);
        lstrcatA(szPluginName, "::");

        if (FreeLibrary(hInstanceDLL))
        {
          StackHandleDecrease(&hHandlesStack, hInstanceDLL);

          //Clean-up plugins stack
          {
            PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hPluginsStack.first;
            PLUGINFUNCTIONA *pfElement2;

            while (pfElement)
            {
              if (!xstrcmpinA(szPluginName, pfElement->szFunction, (DWORD)-1))
              {
                if (pfElement->wHotkey || pfElement->bOnStart)
                  pfElement->bRunning=FALSE;
                else
                {
                  pfElement2=pfElement->next;
                  StackPluginDelete(&hPluginsStack, pfElement);
                  pfElement=pfElement2;
                  continue;
                }
              }
              pfElement=pfElement->next;
            }
          }
          SendMessage(hMainWnd, AKDN_DLLUNLOAD, 0, (WPARAM)szPluginName);
        }
      }
    }
    return TRUE;
  }
  return FALSE;
}

BOOL TranslatePluginW(LPMSG lpMsg)
{
  if (lpMsg->message == AKD_POSTMESSAGE)
  {
    POSTMESSAGE *pm=(POSTMESSAGE *)lpMsg->lParam;

    if (pm)
    {
      SendMessageW(pm->hWnd, pm->uMsg, pm->wParam, pm->lParam);
      GlobalFree((HGLOBAL)pm);
    }
    return TRUE;
  }
  else if (lpMsg->message == AKD_CALLPROC)
  {
    PLUGINPROC lpProcedure=(PLUGINPROC)lpMsg->wParam;
    void *lpParameter=(void *)lpMsg->lParam;

    (lpProcedure)(lpParameter);
    return TRUE;
  }
  else if (lpMsg->message == AKD_DLLCALL)
  {
    PLUGINCALLPOSTW *pcp=(PLUGINCALLPOSTW *)lpMsg->lParam;

    CallPluginReceivePostW(pcp);
    return TRUE;
  }
  else if (lpMsg->message == AKD_DLLUNLOAD)
  {
    HMODULE hInstanceDLL=(HMODULE)lpMsg->wParam;
    HANDLE hThread=(HANDLE)lpMsg->lParam;
    wchar_t wszPluginName[MAX_PATH];

    if (hInstanceDLL)
    {
      if (hThread)
      {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
      }

      if (GetModuleFileNameW(hInstanceDLL, wbuf, BUFFER_SIZE))
      {
        GetBaseNameW(wbuf, wszPluginName, MAX_PATH);
        lstrcatW(wszPluginName, L"::");

        if (FreeLibrary(hInstanceDLL))
        {
          StackHandleDecrease(&hHandlesStack, hInstanceDLL);

          //Clean-up plugins stack
          {
            PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hPluginsStack.first;
            PLUGINFUNCTIONW *pfElement2;

            while (pfElement)
            {
              if (!xstrcmpinW(wszPluginName, pfElement->szFunction, (DWORD)-1))
              {
                if (pfElement->wHotkey || pfElement->bOnStart)
                  pfElement->bRunning=FALSE;
                else
                {
                  pfElement2=pfElement->next;
                  StackPluginDelete(&hPluginsStack, pfElement);
                  pfElement=pfElement2;
                  continue;
                }
              }
              pfElement=pfElement->next;
            }
          }
          SendMessage(hMainWnd, AKDN_DLLUNLOAD, 0, (WPARAM)wszPluginName);
        }
      }
    }
    return TRUE;
  }
  return FALSE;
}

BOOL TranslateHotkeyA(HSTACK *hStack, LPMSG lpMsg)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->first;
  BYTE nMod=0;
  WORD wHotkey;
  BOOL bResult;

  if (lpMsg->message == WM_SYSKEYDOWN ||  //Alt+..., Shift+Alt+...
      lpMsg->message == WM_KEYDOWN)       //Other combinations
  {
    if ((lpMsg->lParam >> 24) & 1) nMod|=HOTKEYF_EXT;
    if (GetKeyState(VK_CONTROL) & 0x80) nMod|=HOTKEYF_CONTROL;
    if (GetKeyState(VK_MENU) & 0x80) nMod|=HOTKEYF_ALT;
    if (GetKeyState(VK_SHIFT) & 0x80) nMod|=HOTKEYF_SHIFT;
    wHotkey=MAKEWORD(lpMsg->wParam, nMod);

    while (pfElement)
    {
      if (pfElement->wHotkey == wHotkey)
      {
        if (pfElement->PluginProc)
        {
          if (!(pfElement->PluginProc)(pfElement->lpParameter))
            break;
        }
        else
        {
          int nResult;

          nResult=CallPluginA(pfElement, pfElement->szFunction, FALSE, 0, NULL);

          if (nResult != UD_FAILED)
          {
            if (!(nResult & UD_NONUNLOAD_UNCHANGE))
            {
              if (nResult & UD_NONUNLOAD_ACTIVE)
                pfElement->bRunning=TRUE;
              else
                pfElement->bRunning=FALSE;
            }
            if (nResult & UD_HOTKEY_DODEFAULT)
              break;
          }
        }
        return TRUE;
      }
      pfElement=pfElement->next;
    }
    bResult=FALSE;
    SendMessage(hMainWnd, AKDN_HOTKEY, (WPARAM)wHotkey, (LPARAM)&bResult);
    return bResult;
  }
  return FALSE;
}

BOOL TranslateHotkeyW(HSTACK *hStack, LPMSG lpMsg)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->first;
  BYTE nMod=0;
  WORD wHotkey;
  BOOL bResult;

  if (lpMsg->message == WM_SYSKEYDOWN ||  //Alt+..., Shift+Alt+...
      lpMsg->message == WM_KEYDOWN)       //Other combinations
  {
    if ((lpMsg->lParam >> 24) & 1) nMod|=HOTKEYF_EXT;
    if (GetKeyState(VK_CONTROL) & 0x80) nMod|=HOTKEYF_CONTROL;
    if (GetKeyState(VK_MENU) & 0x80) nMod|=HOTKEYF_ALT;
    if (GetKeyState(VK_SHIFT) & 0x80) nMod|=HOTKEYF_SHIFT;
    wHotkey=MAKEWORD(lpMsg->wParam, nMod);

    while (pfElement)
    {
      if (pfElement->wHotkey == wHotkey)
      {
        if (pfElement->PluginProc)
        {
          if (!(pfElement->PluginProc)(pfElement->lpParameter))
            break;
        }
        else
        {
          int nResult;

          nResult=CallPluginW(pfElement, pfElement->szFunction, FALSE, 0, NULL);

          if (nResult != UD_FAILED)
          {
            if (!(nResult & UD_NONUNLOAD_UNCHANGE))
            {
              if (nResult & UD_NONUNLOAD_ACTIVE)
                pfElement->bRunning=TRUE;
              else
                pfElement->bRunning=FALSE;
            }
            if (nResult & UD_HOTKEY_DODEFAULT)
              break;
          }
        }
        return TRUE;
      }
      pfElement=pfElement->next;
    }
    bResult=FALSE;
    SendMessage(hMainWnd, AKDN_HOTKEY, (WPARAM)wHotkey, (LPARAM)&bResult);
    return bResult;
  }
  return FALSE;
}


//// WM_COPYDATA

BOOL OpenDocumentSendA(HWND hWnd, HWND hWndEdit, char *pFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess)
{
  OPENDOCUMENTA od;
  COPYDATASTRUCT cds;

  lstrcpynA(od.szFile, pFile, MAX_PATH);

  if (bOtherProcess)
    GetCurrentDirectoryA(MAX_PATH, od.szWorkDir);
  else
    od.szWorkDir[0]='\0';

  od.hWnd=hWndEdit;
  od.dwFlags=dwFlags;
  od.nCodePage=nCodePage;
  od.bBOM=bBOM;

  cds.dwData=CD_OPENDOCUMENT;
  cds.cbData=sizeof(OPENDOCUMENTA);
  cds.lpData=&od;
  return SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
}

BOOL OpenDocumentSendW(HWND hWnd, HWND hWndEdit, wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess)
{
  OPENDOCUMENTW od;
  COPYDATASTRUCT cds;

  lstrcpynW(od.szFile, wpFile, MAX_PATH);

  if (bOtherProcess)
    GetCurrentDirectoryW(MAX_PATH, od.szWorkDir);
  else
    od.szWorkDir[0]='\0';

  od.hWnd=hWndEdit;
  od.dwFlags=dwFlags;
  od.nCodePage=nCodePage;
  od.bBOM=bBOM;

  cds.dwData=CD_OPENDOCUMENT;
  cds.cbData=sizeof(OPENDOCUMENTW);
  cds.lpData=&od;
  return SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
}


//// Options

LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam)
{
  //Center PropertySheet
  if (iCode == HCBT_ACTIVATE)
  {
    RECT rcEdit;
    RECT rcSheet;

    if (hHookOptions)
    {
      if (UnhookWindowsHookEx(hHookOptions))
        hHookOptions=NULL;
    }
    GetWindowRect(hMainWnd, &rcEdit);
    GetWindowRect((HWND)wParam, &rcSheet);
    rcSheet.left=rcEdit.left + ((rcEdit.right - rcEdit.left) / 2) - ((rcSheet.right - rcSheet.left) / 2);
    rcSheet.top=rcEdit.top + ((rcEdit.bottom - rcEdit.top) / 2) - ((rcSheet.bottom - rcSheet.top) / 2);
    if (rcSheet.left < 0) rcSheet.left=0;
    if (rcSheet.top < 0) rcSheet.top=0;

    SetWindowPos((HWND)wParam, NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
  }
  return 0;
}

int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam)
{
  //Remove "?"
  if (uMsg == PSCB_PRECREATE)
    ((DLGTEMPLATE *)lParam)->style&=~DS_CONTEXTHELP;

  return TRUE;
}

BOOL CALLBACK OptionsGeneralDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndDefaultCP;
  static HWND hWndAutodetectCP;
  static HWND hWndAutodetectCPBuffer;
  HWND hWndRecentFiles;
  HWND hWndRecentFilesSpin;
  HWND hWndRecentFilesClear;
  HWND hWndSearchStrings;
  HWND hWndSearchStringsSpin;
  HWND hWndSearchStringsClear;
  HWND hWndCommand;
  HWND hWndDirectory;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    hWndRecentFiles=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT);
    hWndRecentFilesSpin=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT_SPIN);
    hWndRecentFilesClear=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_CLEAR);
    hWndSearchStrings=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT);
    hWndSearchStringsSpin=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT_SPIN);
    hWndSearchStringsClear=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_CLEAR);
    hWndCommand=GetDlgItem(hDlg, IDC_OPTIONS_EXECCOM);
    hWndDirectory=GetDlgItem(hDlg, IDC_OPTIONS_EXECDIR);
    hWndDefaultCP=GetDlgItem(hDlg, IDC_OPTIONS_DEFAULT_CODEPAGE);
    hWndAutodetectCP=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION);
    hWndAutodetectCPBuffer=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER);

    SendMessage(hWndCommand, EM_LIMITTEXT, (WPARAM)BUFFER_SIZE, 0);
    SendMessage(hWndDirectory, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndRecentFilesSpin, UDM_SETBUDDY, (WPARAM)hWndRecentFiles, 0);
    SendMessage(hWndRecentFilesSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndSearchStringsSpin, UDM_SETBUDDY, (WPARAM)hWndSearchStrings, 0);
    SendMessage(hWndSearchStringsSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));

    SetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, nRecentFiles, FALSE);
    if (!nRecentFiles || !*lpszRecentNames[0]) EnableWindow(hWndRecentFilesClear, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, nSearchStrings, FALSE);
    if (!nSearchStrings) EnableWindow(hWndSearchStringsClear, FALSE);
    SetWindowTextA(hWndCommand, szCommand);
    SetWindowTextA(hWndDirectory, szWorkDir);
    SetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, dwCodepageRecognitionBuffer, FALSE);

    FillComboboxCodepageA(hWndDefaultCP, lpCodepageList);
    SelectComboboxCodepageA(hWndDefaultCP, nDefaultCodePage);

    API_LoadStringA(hLangLib, STR_NONE, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);
    API_LoadStringA(hLangLib, STR_AUTODETECT_RUSSIAN, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);
    API_LoadStringA(hLangLib, STR_AUTODETECT_ENGLISH, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);
    API_LoadStringA(hLangLib, STR_AUTODETECT_TURKISH, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);
    API_LoadStringA(hLangLib, STR_AUTODETECT_CHINESE, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);
    API_LoadStringA(hLangLib, STR_AUTODETECT_JAPANESE, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);
    API_LoadStringA(hLangLib, STR_AUTODETECT_KOREAN, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);

    if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_RUSSIAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 1, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_ENGLISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 2, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_TURKISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 3, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_CHINESE)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 4, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_JAPANESE)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 5, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_KOREAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 6, 0);
    else
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 0, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_RECENTFILES_CLEAR)
    {
      RecentFilesZeroA();
      RecentFilesSaveA();
      bMenuRecentFiles=TRUE;
      EnableWindow((HWND)lParam, FALSE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_SEARCHSTRINGS_CLEAR)
    {
      RegClearKeyA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Search");
      EnableWindow((HWND)lParam, FALSE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_EXECCOM_BROWSE)
    {
      OPENFILENAMEA efnA={0};
      char szExeFilter[MAX_PATH];

      //GetOpenFileName dialog file filter
      API_LoadStringA(hLangLib, STR_EXECUTABLE_FILTER, szExeFilter, MAX_PATH);
      for (i=0; szExeFilter[i]; ++i)
        if (szExeFilter[i] == '|') szExeFilter[i]='\0';
      szExeFilter[++i]='\0';

      buf[0]='\0';
      efnA.lStructSize      =sizeof(OPENFILENAMEA);
      efnA.hwndOwner        =hDlg;
      efnA.lpstrFile        =(char *)&buf;
      efnA.nMaxFile         =MAX_PATH;
      efnA.lpstrFilter      =szExeFilter;
      efnA.nFilterIndex     =1;
      efnA.Flags            =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

      if (GetOpenFileNameA(&efnA))
      {
        wsprintfA(buf2, "\"%s\"", buf);
        SetDlgItemTextA(hDlg, IDC_OPTIONS_EXECCOM, buf2);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_EXECDIR_BROWSE)
    {
      BROWSEINFOA biA={0};
      LPITEMIDLIST pIdList;
      LPMALLOC pMalloc;

      GetDlgItemTextA(hDlg, IDC_OPTIONS_EXECDIR, buf, BUFFER_SIZE);
      biA.hwndOwner=hDlg;
      biA.pszDisplayName=buf;
      biA.ulFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
      biA.lpfn=BrowseCallbackProcA;
      biA.lParam=(LPARAM)buf;

      if (pIdList=(*SHBrowseForFolderAPrt)(&biA))
      {
        (*SHGetPathFromIDListAPrt)(pIdList, buf);

        if (SHGetMalloc(&pMalloc))
        {
          pMalloc->lpVtbl->Free(pMalloc, pIdList);  // pMalloc->Free(pIdList);
          pMalloc->lpVtbl->Release(pMalloc);        // pMalloc->Release();
        }
        SetDlgItemTextA(hDlg, IDC_OPTIONS_EXECDIR, buf);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGE_FILTER)
    {
      API_DialogBoxParamA(hLangLib, MAKEINTRESOURCEA(IDD_OPTIONS_GENERAL_FILTER), hDlg, (DLGPROC)OptionsGeneralFilterDlgProcA, (LPARAM)hWndDefaultCP);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      //Recent files
      i=GetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, NULL, FALSE);
      if (nRecentFiles != i)
      {
        nRecentFiles=i;
        FreeMemoryRecentFilesA();

        if (nRecentFiles)
        {
          if (RecentFilesAllocA())
          {
            RecentFilesZeroA();
            RecentFilesReadA();
          }
        }
        bMenuRecentFiles=TRUE;

        RegClearKeyA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Recent");
        RecentFilesSaveA();
      }

      //Search history
      nSearchStrings=GetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, NULL, FALSE);

      //Execute
      GetDlgItemTextA(hDlg, IDC_OPTIONS_EXECCOM, szCommand, BUFFER_SIZE);
      GetDlgItemTextA(hDlg, IDC_OPTIONS_EXECDIR, szWorkDir, MAX_PATH);

      //Default codepage
      nDefaultCodePage=GetComboboxCodepageA(hWndDefaultCP);

      //Autodetect codepage
      i=SendMessage(hWndAutodetectCP, CB_GETCURSEL, 0, 0);

      if (i == 0)
        dwLangCodepageRecognition=0;
      else if (i == 1)
        dwLangCodepageRecognition=LANGID_RUSSIAN;
      else if (i == 2)
        dwLangCodepageRecognition=LANGID_ENGLISH;
      else if (i == 3)
        dwLangCodepageRecognition=LANGID_TURKISH;
      else if (i == 4)
        dwLangCodepageRecognition=LANGID_CHINESE;
      else if (i == 5)
        dwLangCodepageRecognition=LANGID_JAPANESE;
      else if (i == 6)
        dwLangCodepageRecognition=LANGID_KOREAN;

      //Autodetect codepage buffer
      dwCodepageRecognitionBuffer=GetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, NULL, FALSE);
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsGeneralDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndDefaultCP;
  static HWND hWndAutodetectCP;
  static HWND hWndAutodetectCPBuffer;
  HWND hWndRecentFiles;
  HWND hWndRecentFilesSpin;
  HWND hWndRecentFilesClear;
  HWND hWndSearchStrings;
  HWND hWndSearchStringsSpin;
  HWND hWndSearchStringsClear;
  HWND hWndCommand;
  HWND hWndDirectory;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    hWndRecentFiles=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT);
    hWndRecentFilesSpin=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT_SPIN);
    hWndRecentFilesClear=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_CLEAR);
    hWndSearchStrings=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT);
    hWndSearchStringsSpin=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT_SPIN);
    hWndSearchStringsClear=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_CLEAR);
    hWndCommand=GetDlgItem(hDlg, IDC_OPTIONS_EXECCOM);
    hWndDirectory=GetDlgItem(hDlg, IDC_OPTIONS_EXECDIR);
    hWndDefaultCP=GetDlgItem(hDlg, IDC_OPTIONS_DEFAULT_CODEPAGE);
    hWndAutodetectCP=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION);
    hWndAutodetectCPBuffer=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER);

    SendMessage(hWndCommand, EM_LIMITTEXT, (WPARAM)BUFFER_SIZE, 0);
    SendMessage(hWndDirectory, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndRecentFilesSpin, UDM_SETBUDDY, (WPARAM)hWndRecentFiles, 0);
    SendMessage(hWndRecentFilesSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndSearchStringsSpin, UDM_SETBUDDY, (WPARAM)hWndSearchStrings, 0);
    SendMessage(hWndSearchStringsSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));

    SetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, nRecentFiles, FALSE);
    if (!nRecentFiles || !*lpwszRecentNames[0]) EnableWindow(hWndRecentFilesClear, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, nSearchStrings, FALSE);
    if (!nSearchStrings) EnableWindow(hWndSearchStringsClear, FALSE);
    SetWindowTextW(hWndCommand, wszCommand);
    SetWindowTextW(hWndDirectory, wszWorkDir);
    SetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, dwCodepageRecognitionBuffer, FALSE);

    FillComboboxCodepageW(hWndDefaultCP, lpCodepageList);
    SelectComboboxCodepageW(hWndDefaultCP, nDefaultCodePage);

    API_LoadStringW(hLangLib, STR_NONE, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);
    API_LoadStringW(hLangLib, STR_AUTODETECT_RUSSIAN, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);
    API_LoadStringW(hLangLib, STR_AUTODETECT_ENGLISH, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);
    API_LoadStringW(hLangLib, STR_AUTODETECT_TURKISH, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);
    API_LoadStringW(hLangLib, STR_AUTODETECT_CHINESE, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);
    API_LoadStringW(hLangLib, STR_AUTODETECT_JAPANESE, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);
    API_LoadStringW(hLangLib, STR_AUTODETECT_KOREAN, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);

    if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_RUSSIAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 1, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_ENGLISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 2, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_TURKISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 3, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_CHINESE)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 4, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_JAPANESE)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 5, 0);
    else if (PRIMARYLANGID(dwLangCodepageRecognition) == LANG_KOREAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 6, 0);
    else
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 0, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_RECENTFILES_CLEAR)
    {
      RecentFilesZeroW();
      RecentFilesSaveW();
      bMenuRecentFiles=TRUE;
      EnableWindow((HWND)lParam, FALSE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_SEARCHSTRINGS_CLEAR)
    {
      RegClearKeyW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Search");
      EnableWindow((HWND)lParam, FALSE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_EXECCOM_BROWSE)
    {
      OPENFILENAMEW efnW={0};
      wchar_t wszExeFilter[MAX_PATH];

      //GetOpenFileName dialog file filter
      API_LoadStringW(hLangLib, STR_EXECUTABLE_FILTER, wszExeFilter, MAX_PATH);
      for (i=0; wszExeFilter[i]; ++i)
        if (wszExeFilter[i] == '|') wszExeFilter[i]='\0';
      wszExeFilter[++i]='\0';

      wbuf[0]='\0';
      efnW.lStructSize      =sizeof(OPENFILENAMEW);
      efnW.hwndOwner        =hDlg;
      efnW.lpstrFile        =wbuf;
      efnW.nMaxFile         =MAX_PATH;
      efnW.lpstrFilter      =wszExeFilter;
      efnW.nFilterIndex     =1;
      efnW.Flags            =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

      if (GetOpenFileNameW(&efnW))
      {
        wsprintfW(wbuf2, L"\"%s\"", wbuf);
        SetDlgItemTextW(hDlg, IDC_OPTIONS_EXECCOM, wbuf2);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_EXECDIR_BROWSE)
    {
      BROWSEINFOW biW={0};
      LPITEMIDLIST pIdList;
      LPMALLOC pMalloc;

      GetDlgItemTextW(hDlg, IDC_OPTIONS_EXECDIR, wbuf, BUFFER_SIZE);
      biW.hwndOwner=hDlg;
      biW.pszDisplayName=wbuf;
      biW.ulFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
      biW.lpfn=BrowseCallbackProcW;
      biW.lParam=(LPARAM)wbuf;

      if (pIdList=(*SHBrowseForFolderWPrt)(&biW))
      {
        (*SHGetPathFromIDListWPrt)(pIdList, wbuf);

        if (SHGetMalloc(&pMalloc))
        {
          pMalloc->lpVtbl->Free(pMalloc, pIdList);  // pMalloc->Free(pIdList);
          pMalloc->lpVtbl->Release(pMalloc);        // pMalloc->Release();
        }
        SetDlgItemTextW(hDlg, IDC_OPTIONS_EXECDIR, wbuf);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGE_FILTER)
    {
      API_DialogBoxParamW(hLangLib, MAKEINTRESOURCEW(IDD_OPTIONS_GENERAL_FILTER), hDlg, (DLGPROC)OptionsGeneralFilterDlgProcW, (LPARAM)hWndDefaultCP);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      i=GetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, NULL, FALSE);
      if (nRecentFiles != i)
      {
        nRecentFiles=i;
        FreeMemoryRecentFilesW();

        if (nRecentFiles)
        {
          if (RecentFilesAllocW())
          {
            RecentFilesZeroW();
            RecentFilesReadW();
          }
        }
        bMenuRecentFiles=TRUE;

        RegClearKeyW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Recent");
        RecentFilesSaveW();
      }

      //Search history
      nSearchStrings=GetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, NULL, FALSE);

      //Execute
      GetDlgItemTextW(hDlg, IDC_OPTIONS_EXECCOM, wszCommand, BUFFER_SIZE);
      GetDlgItemTextW(hDlg, IDC_OPTIONS_EXECDIR, wszWorkDir, MAX_PATH);

      //Default codepage
      nDefaultCodePage=GetComboboxCodepageW(hWndDefaultCP);

      //Autodetect codepage
      i=SendMessage(hWndAutodetectCP, CB_GETCURSEL, 0, 0);

      if (i == 0)
        dwLangCodepageRecognition=0;
      else if (i == 1)
        dwLangCodepageRecognition=LANGID_RUSSIAN;
      else if (i == 2)
        dwLangCodepageRecognition=LANGID_ENGLISH;
      else if (i == 3)
        dwLangCodepageRecognition=LANGID_TURKISH;
      else if (i == 4)
        dwLangCodepageRecognition=LANGID_CHINESE;
      else if (i == 5)
        dwLangCodepageRecognition=LANGID_JAPANESE;
      else if (i == 6)
        dwLangCodepageRecognition=LANGID_KOREAN;

      //Autodetect codepage buffer
      dwCodepageRecognitionBuffer=GetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, NULL, FALSE);
    }
  }
  return FALSE;
}

int CALLBACK BrowseCallbackProcA(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
  {
    SendMessage(hWnd, BFFM_SETSELECTIONA, TRUE, lpData);
  }
  return 0;
}

int CALLBACK BrowseCallbackProcW(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
  {
    SendMessage(hWnd, BFFM_SETSELECTIONW, TRUE, lpData);
  }
  return 0;
}

BOOL CALLBACK OptionsGeneralFilterDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCustomList;
  static HWND hWndSystemList;
  static HWND hWndOK;
  static HWND hWndDefaultCP;
  static int *lpFullCodepageList;
  int nSelection;
  int nCount;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndCustomList=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGES_FILTER_CUSTOM);
    hWndSystemList=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGES_FILTER_SYSTEM);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndDefaultCP=(HWND)lParam;

    FillListboxCodepageA(hWndCustomList, lpCodepageList);
    EnumCodepageList(&lpFullCodepageList);
    FillListboxCodepageA(hWndSystemList, lpFullCodepageList);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_UP)
    {
      if ((nSelection=SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) > 0)
      {
        SendMessageA(hWndCustomList, LB_GETTEXT, nSelection, (LPARAM)buf);
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);
        SendMessageA(hWndCustomList, LB_INSERTSTRING, nSelection - 1, (LPARAM)buf);
        SendMessage(hWndCustomList, LB_SETCURSEL, nSelection - 1, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DOWN)
    {
      if ((nSelection=(int)SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) != LB_ERR &&
          (nCount=(int)SendMessage(hWndCustomList, LB_GETCOUNT, 0, 0)) != LB_ERR &&
           nSelection != nCount - 1)
      {
        SendMessageA(hWndCustomList, LB_GETTEXT, nSelection, (LPARAM)buf);
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);
        SendMessageA(hWndCustomList, LB_INSERTSTRING, nSelection + 1, (LPARAM)buf);
        SendMessage(hWndCustomList, LB_SETCURSEL, nSelection + 1, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_ADD)
    {
      if ((nSelection=(int)SendMessage(hWndSystemList, LB_GETCURSEL, 0, 0)) != LB_ERR)
      {
        SendMessageA(hWndSystemList, LB_GETTEXT, nSelection, (LPARAM)buf);

        if ((int)SendMessageA(hWndCustomList, LB_FINDSTRING, (WPARAM)-1, (LPARAM)buf) == LB_ERR)
        {
          nSelection=SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0);
          SendMessageA(hWndCustomList, LB_INSERTSTRING, nSelection, (LPARAM)buf);
          EnableWindow(hWndOK, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DELETE)
    {
      if ((nSelection=(int)SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) != LB_ERR)
      {
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);

        if ((nCount=SendMessage(hWndCustomList, LB_GETCOUNT, 0, 0)) > 0)
        {
          if (nSelection < nCount)
            SendMessage(hWndCustomList, LB_SETCURSEL, nSelection, 0);
          else
            SendMessage(hWndCustomList, LB_SETCURSEL, nCount - 1, 0);
        }
        else EnableWindow(hWndOK, FALSE);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_ADDALL)
    {
      ClearListbox(hWndCustomList);
      FillListboxCodepageA(hWndCustomList, lpFullCodepageList);
      EnableWindow(hWndOK, TRUE);
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DELETEALL)
    {
      ClearListbox(hWndCustomList);
      EnableWindow(hWndOK, FALSE);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      bMenuPopupCodepage=TRUE;
      CodepageListFree(&lpCodepageList);
      CodepageListFree(&lpFullCodepageList);
      GetListboxCodepageListA(hWndCustomList, &lpCodepageList);
      nCodepageListLen=CodepageListLen(lpCodepageList);
      ClearCombobox(hWndDefaultCP);
      FillComboboxCodepageA(hWndDefaultCP, lpCodepageList);
      if (SelectComboboxCodepageA(hWndDefaultCP, nDefaultCodePage) == CB_ERR)
        SendMessage(hWndDefaultCP, CB_SETCURSEL, 0, 0);
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      CodepageListFree(&lpFullCodepageList);
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsGeneralFilterDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCustomList;
  static HWND hWndSystemList;
  static HWND hWndOK;
  static HWND hWndDefaultCP;
  static int *lpFullCodepageList;
  int nSelection;
  int nCount;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndCustomList=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGES_FILTER_CUSTOM);
    hWndSystemList=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGES_FILTER_SYSTEM);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndDefaultCP=(HWND)lParam;

    FillListboxCodepageW(hWndCustomList, lpCodepageList);
    EnumCodepageList(&lpFullCodepageList);
    FillListboxCodepageW(hWndSystemList, lpFullCodepageList);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_UP)
    {
      if ((nSelection=SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) > 0)
      {
        SendMessageW(hWndCustomList, LB_GETTEXT, nSelection, (LPARAM)wbuf);
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);
        SendMessageW(hWndCustomList, LB_INSERTSTRING, nSelection - 1, (LPARAM)wbuf);
        SendMessage(hWndCustomList, LB_SETCURSEL, nSelection - 1, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DOWN)
    {
      if ((nSelection=(int)SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) != LB_ERR &&
          (nCount=(int)SendMessage(hWndCustomList, LB_GETCOUNT, 0, 0)) != LB_ERR &&
           nSelection != nCount - 1)
      {
        SendMessageW(hWndCustomList, LB_GETTEXT, nSelection, (LPARAM)wbuf);
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);
        SendMessageW(hWndCustomList, LB_INSERTSTRING, nSelection + 1, (LPARAM)wbuf);
        SendMessage(hWndCustomList, LB_SETCURSEL, nSelection + 1, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_ADD)
    {
      if ((nSelection=(int)SendMessage(hWndSystemList, LB_GETCURSEL, 0, 0)) != LB_ERR)
      {
        SendMessageW(hWndSystemList, LB_GETTEXT, nSelection, (LPARAM)wbuf);

        if ((int)SendMessageW(hWndCustomList, LB_FINDSTRING, (WPARAM)-1, (LPARAM)wbuf) == LB_ERR)
        {
          nSelection=SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0);
          SendMessageW(hWndCustomList, LB_INSERTSTRING, nSelection, (LPARAM)wbuf);
          EnableWindow(hWndOK, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DELETE)
    {
      if ((nSelection=(int)SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) != LB_ERR)
      {
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);

        if ((nCount=SendMessage(hWndCustomList, LB_GETCOUNT, 0, 0)) > 0)
        {
          if (nSelection < nCount)
            SendMessage(hWndCustomList, LB_SETCURSEL, nSelection, 0);
          else
            SendMessage(hWndCustomList, LB_SETCURSEL, nCount - 1, 0);
        }
        else EnableWindow(hWndOK, FALSE);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_ADDALL)
    {
      ClearListbox(hWndCustomList);
      FillListboxCodepageW(hWndCustomList, lpFullCodepageList);
      EnableWindow(hWndOK, TRUE);
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DELETEALL)
    {
      ClearListbox(hWndCustomList);
      EnableWindow(hWndOK, FALSE);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      bMenuPopupCodepage=TRUE;
      CodepageListFree(&lpCodepageList);
      CodepageListFree(&lpFullCodepageList);
      GetListboxCodepageListW(hWndCustomList, &lpCodepageList);
      nCodepageListLen=CodepageListLen(lpCodepageList);
      ClearCombobox(hWndDefaultCP);
      FillComboboxCodepageW(hWndDefaultCP, lpCodepageList);
      if (SelectComboboxCodepageW(hWndDefaultCP, nDefaultCodePage) == CB_ERR)
        SendMessage(hWndDefaultCP, CB_SETCURSEL, 0, 0);
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      CodepageListFree(&lpFullCodepageList);
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsAdvanced1DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFileTypesOpen;
  static HWND hWndAssociateOpen;
  static HWND hWndFileTypesEdit;
  static HWND hWndAssociateEdit;
  static HWND hWndFileTypesPrint;
  static HWND hWndAssociatePrint;
  static HWND hWndTabSize;
  static HWND hWndTabSizeSpaces;
  static HWND hWndSaveRegistry;
  static HWND hWndSaveINI;
  static HWND hWndUndoLimit;
  static HWND hWndDetailedUndo;
  static HWND hWndDelimitersEnable;
  static HWND hWndDelimiters;
  HWND hWndTabSizeSpin;
  HWND hWndUndoLimitSpin;
  HWND hWndMarginLeft;
  HWND hWndMarginLeftSpin;
  HWND hWndMarginRight;
  HWND hWndMarginRightSpin;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndFileTypesOpen=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_OPEN);
    hWndAssociateOpen=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_OPEN);
    hWndFileTypesEdit=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_EDIT);
    hWndAssociateEdit=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_EDIT);
    hWndFileTypesPrint=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_PRINT);
    hWndAssociatePrint=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_PRINT);
    hWndTabSize=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE);
    hWndTabSizeSpin=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPIN);
    hWndTabSizeSpaces=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPACES);
    hWndSaveRegistry=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_REGISTRY);
    hWndSaveINI=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_INI);
    hWndUndoLimit=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT);
    hWndUndoLimitSpin=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT_SPIN);
    hWndDetailedUndo=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_DETAILED);
    hWndMarginLeft=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT);
    hWndMarginLeftSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT_SPIN);
    hWndMarginRight=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT);
    hWndMarginRightSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT_SPIN);
    hWndDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS_ENABLE);
    hWndDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS);

    if (dwFileTypesAssociated & AE_OPEN) SendMessage(hWndAssociateOpen, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_EDIT) SendMessage(hWndAssociateEdit, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_PRINT) SendMessage(hWndAssociatePrint, BM_SETCHECK, BST_CHECKED, 0);
    if (!(dwFileTypesAssociated & AE_OPEN)) EnableWindow(hWndFileTypesOpen, FALSE);
    if (!(dwFileTypesAssociated & AE_EDIT)) EnableWindow(hWndFileTypesEdit, FALSE);
    if (!(dwFileTypesAssociated & AE_PRINT)) EnableWindow(hWndFileTypesPrint, FALSE);
    SendMessage(hWndFileTypesOpen, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesEdit, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesPrint, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndTabSizeSpin, UDM_SETBUDDY, (WPARAM)hWndTabSize, 0);
    SendMessage(hWndTabSizeSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 1));
    SendMessage(hWndUndoLimitSpin, UDM_SETBUDDY, (WPARAM)hWndUndoLimit, 0);
    SendMessage(hWndUndoLimitSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndMarginLeftSpin, UDM_SETBUDDY, (WPARAM)hWndMarginLeft, 0);
    SendMessage(hWndMarginLeftSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndMarginRightSpin, UDM_SETBUDDY, (WPARAM)hWndMarginRight, 0);
    SendMessage(hWndMarginRightSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndDelimiters, EM_LIMITTEXT, (WPARAM)DELIMITERS_SIZE, 0);

    SetWindowTextA(hWndFileTypesOpen, szFileTypesOpen);
    SetWindowTextA(hWndFileTypesEdit, szFileTypesEdit);
    SetWindowTextA(hWndFileTypesPrint, szFileTypesPrint);

    SetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, nTabStopSize, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, nUndoLimit, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, LOWORD(dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, HIWORD(dwEditMargins), FALSE);

    if (bTabStopAsSpaces)
      SendMessage(hWndTabSizeSpaces, BM_SETCHECK, BST_CHECKED, 0);
    if (bDetailedUndo)
      SendMessage(hWndDetailedUndo, BM_SETCHECK, BST_CHECKED, 0);
    if (nSaveSettings == SS_REGISTRY)
      SendMessage(hWndSaveRegistry, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndSaveINI, BM_SETCHECK, BST_CHECKED, 0);

    if (bWordDelimitersEnable)
      SendMessage(hWndDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndDelimiters, bWordDelimitersEnable);
    EscapeDataToEscapeStringW(wszWordDelimiters, (wchar_t *)buf);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t *)buf, -1, buf2, BUFFER_SIZE, NULL, NULL);
    SetWindowTextA(hWndDelimiters, buf2);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_OPEN)
    {
      bState=SendMessage(hWndAssociateOpen, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesOpen, bState);
      if (!bState) SetWindowTextA(hWndFileTypesOpen, szFileTypesOpen);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_EDIT)
    {
      bState=SendMessage(hWndAssociateEdit, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesEdit, bState);
      if (!bState) SetWindowTextA(hWndFileTypesEdit, szFileTypesEdit);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_PRINT)
    {
      bState=SendMessage(hWndAssociatePrint, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesPrint, bState);
      if (!bState) SetWindowTextA(hWndFileTypesPrint, szFileTypesPrint);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WORD_DELIMITERS_ENABLE)
    {
      bState=SendMessage(hWndDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndDelimiters, bState);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WORD_DELIMITERS_RESET)
    {
      EscapeDataToEscapeStringW(WORD_DELIMITERSW, (wchar_t *)buf);
      WideCharToMultiByte(CP_ACP, 0, (wchar_t *)buf, -1, buf2, BUFFER_SIZE, NULL, NULL);
      SetDlgItemTextA(hDlg, IDC_OPTIONS_WORD_DELIMITERS, buf2);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      char szWindowText[MAX_PATH];
      BOOL bShellRefresh=FALSE;
      int a;
      int b;

      //Associations
      bState=SendMessage(hWndAssociateOpen, BM_GETCHECK, 0, 0);
      if (bState && !(dwFileTypesAssociated & AE_OPEN))
      {
        GetWindowTextA(hWndFileTypesOpen, szFileTypesOpen, MAX_PATH);
        AssociateFileTypesA(hInstance, szFileTypesOpen, AE_OPEN|AE_ASSOCIATE);
        dwFileTypesAssociated|=AE_OPEN;
        bShellRefresh=TRUE;
      }
      else if (bState && (dwFileTypesAssociated & AE_OPEN))
      {
        GetWindowTextA(hWndFileTypesOpen, szWindowText, MAX_PATH);
        if (lstrcmpiA(szWindowText, szFileTypesOpen))
        {
          AssociateFileTypesA(hInstance, szFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
          AssociateFileTypesA(hInstance, szWindowText, AE_OPEN|AE_ASSOCIATE);
          lstrcpynA(szFileTypesOpen, szWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (dwFileTypesAssociated & AE_OPEN))
      {
        AssociateFileTypesA(hInstance, szFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
        dwFileTypesAssociated&=~AE_OPEN;
        bShellRefresh=TRUE;
      }

      bState=SendMessage(hWndAssociateEdit, BM_GETCHECK, 0, 0);
      if (bState && !(dwFileTypesAssociated & AE_EDIT))
      {
        GetWindowTextA(hWndFileTypesEdit, szFileTypesEdit, MAX_PATH);
        AssociateFileTypesA(hInstance, szFileTypesEdit, AE_EDIT|AE_ASSOCIATE);
        dwFileTypesAssociated|=AE_EDIT;
        bShellRefresh=TRUE;
      }
      else if (bState && (dwFileTypesAssociated & AE_EDIT))
      {
        GetWindowTextA(hWndFileTypesEdit, szWindowText, MAX_PATH);
        if (lstrcmpiA(szWindowText, szFileTypesEdit))
        {
          AssociateFileTypesA(hInstance, szFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
          AssociateFileTypesA(hInstance, szWindowText, AE_EDIT|AE_ASSOCIATE);
          lstrcpynA(szFileTypesEdit, szWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (dwFileTypesAssociated & AE_EDIT))
      {
        AssociateFileTypesA(hInstance, szFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
        dwFileTypesAssociated&=~AE_EDIT;
        bShellRefresh=TRUE;
      }

      bState=SendMessage(hWndAssociatePrint, BM_GETCHECK, 0, 0);
      if (bState && !(dwFileTypesAssociated & AE_PRINT))
      {
        GetWindowTextA(hWndFileTypesPrint, szFileTypesPrint, MAX_PATH);
        AssociateFileTypesA(hInstance, szFileTypesPrint, AE_PRINT|AE_ASSOCIATE);
        dwFileTypesAssociated|=AE_PRINT;
        bShellRefresh=TRUE;
      }
      else if (bState && (dwFileTypesAssociated & AE_PRINT))
      {
        GetWindowTextA(hWndFileTypesPrint, szWindowText, MAX_PATH);
        if (lstrcmpiA(szWindowText, szFileTypesPrint))
        {
          AssociateFileTypesA(hInstance, szFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
          AssociateFileTypesA(hInstance, szWindowText, AE_PRINT|AE_ASSOCIATE);
          lstrcpynA(szFileTypesPrint, szWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (dwFileTypesAssociated & AE_PRINT))
      {
        AssociateFileTypesA(hInstance, szFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
        dwFileTypesAssociated&=~AE_PRINT;
        bShellRefresh=TRUE;
      }
      if (bShellRefresh) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);

      //Tab stops
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, NULL, FALSE);
      if (nTabStopSize != a)
      {
        nTabStopSize=a;
        SetTabStopsA(hWndEdit, nTabStopSize, TRUE);
      }
      bTabStopAsSpaces=SendMessage(hWndTabSizeSpaces, BM_GETCHECK, 0, 0);

      //Undo
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, NULL, FALSE);
      if (nUndoLimit != a)
      {
        nUndoLimit=a;
        SendMessage(hWndEdit, EM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
      }
      bDetailedUndo=SendMessage(hWndDetailedUndo, BM_GETCHECK, 0, 0);

      //Margins
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, NULL, FALSE);
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, NULL, FALSE);
      if (dwEditMargins != (DWORD)MAKELONG(a, b))
      {
        dwEditMargins=MAKELONG(a, b);
        SendMessage(hWndEdit, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
        InvalidateRect(hWndEdit, NULL, FALSE);
      }

      //Word delimiters
      a=SendMessage(hWndDelimitersEnable, BM_GETCHECK, 0, 0);
      if (a != bWordDelimitersEnable)
      {
        bWordDelimitersEnable=a;

        if (bWordDelimitersEnable)
        {
          SendMessage(hWndEdit, EM_SETWORDBREAKPROC, 0, (LPARAM)EditWordBreakProc);
        }
        else
        {
          if (bOldRichEdit)
            bOptionsRestart=TRUE;
          else
            SendMessage(hWndEdit, EM_SETWORDBREAKPROC, 0, (LPARAM)NULL);
        }
      }
      a=GetWindowTextA(hWndDelimiters, buf, BUFFER_SIZE);
      MultiByteToWideChar(CP_ACP, 0, buf, a + 1, (wchar_t *)buf2, BUFFER_SIZE / sizeof(wchar_t));
      EscapeStringToEscapeDataW((wchar_t *)buf2, wszWordDelimiters);

      //Save settings
      if (SendMessage(hWndSaveRegistry, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=SS_REGISTRY;
      else
        a=SS_INI;
      if (nSaveSettings != a)
      {
        nSaveSettings=a;
        nRegSaveSettings=a;
        bOptionsSave=TRUE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsAdvanced1DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFileTypesOpen;
  static HWND hWndAssociateOpen;
  static HWND hWndFileTypesEdit;
  static HWND hWndAssociateEdit;
  static HWND hWndFileTypesPrint;
  static HWND hWndAssociatePrint;
  static HWND hWndTabSize;
  static HWND hWndTabSizeSpaces;
  static HWND hWndSaveRegistry;
  static HWND hWndSaveINI;
  static HWND hWndUndoLimit;
  static HWND hWndDetailedUndo;
  static HWND hWndDelimitersEnable;
  static HWND hWndDelimiters;
  HWND hWndTabSizeSpin;
  HWND hWndUndoLimitSpin;
  HWND hWndMarginLeft;
  HWND hWndMarginLeftSpin;
  HWND hWndMarginRight;
  HWND hWndMarginRightSpin;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndFileTypesOpen=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_OPEN);
    hWndAssociateOpen=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_OPEN);
    hWndFileTypesEdit=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_EDIT);
    hWndAssociateEdit=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_EDIT);
    hWndFileTypesPrint=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_PRINT);
    hWndAssociatePrint=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_PRINT);
    hWndTabSize=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE);
    hWndTabSizeSpin=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPIN);
    hWndTabSizeSpaces=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPACES);
    hWndSaveRegistry=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_REGISTRY);
    hWndSaveINI=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_INI);
    hWndUndoLimit=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT);
    hWndUndoLimitSpin=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT_SPIN);
    hWndDetailedUndo=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_DETAILED);
    hWndMarginLeft=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT);
    hWndMarginLeftSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT_SPIN);
    hWndMarginRight=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT);
    hWndMarginRightSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT_SPIN);
    hWndDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS_ENABLE);
    hWndDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS);

    if (dwFileTypesAssociated & AE_OPEN) SendMessage(hWndAssociateOpen, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_EDIT) SendMessage(hWndAssociateEdit, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_PRINT) SendMessage(hWndAssociatePrint, BM_SETCHECK, BST_CHECKED, 0);
    if (!(dwFileTypesAssociated & AE_OPEN)) EnableWindow(hWndFileTypesOpen, FALSE);
    if (!(dwFileTypesAssociated & AE_EDIT)) EnableWindow(hWndFileTypesEdit, FALSE);
    if (!(dwFileTypesAssociated & AE_PRINT)) EnableWindow(hWndFileTypesPrint, FALSE);
    SendMessage(hWndFileTypesOpen, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesEdit, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesPrint, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndTabSizeSpin, UDM_SETBUDDY, (WPARAM)hWndTabSize, 0);
    SendMessage(hWndTabSizeSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 1));
    SendMessage(hWndUndoLimitSpin, UDM_SETBUDDY, (WPARAM)hWndUndoLimit, 0);
    SendMessage(hWndUndoLimitSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndMarginLeftSpin, UDM_SETBUDDY, (WPARAM)hWndMarginLeft, 0);
    SendMessage(hWndMarginLeftSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndMarginRightSpin, UDM_SETBUDDY, (WPARAM)hWndMarginRight, 0);
    SendMessage(hWndMarginRightSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndDelimiters, EM_LIMITTEXT, (WPARAM)DELIMITERS_SIZE, 0);

    SetWindowTextW(hWndFileTypesOpen, wszFileTypesOpen);
    SetWindowTextW(hWndFileTypesEdit, wszFileTypesEdit);
    SetWindowTextW(hWndFileTypesPrint, wszFileTypesPrint);

    SetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, nTabStopSize, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, nUndoLimit, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, LOWORD(dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, HIWORD(dwEditMargins), FALSE);

    if (bTabStopAsSpaces)
      SendMessage(hWndTabSizeSpaces, BM_SETCHECK, BST_CHECKED, 0);
    if (bDetailedUndo)
      SendMessage(hWndDetailedUndo, BM_SETCHECK, BST_CHECKED, 0);
    if (nSaveSettings == SS_REGISTRY)
      SendMessage(hWndSaveRegistry, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndSaveINI, BM_SETCHECK, BST_CHECKED, 0);

    if (bWordDelimitersEnable)
      SendMessage(hWndDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndDelimiters, bWordDelimitersEnable);
    EscapeDataToEscapeStringW(wszWordDelimiters, wbuf);
    SetWindowTextW(hWndDelimiters, wbuf);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_OPEN)
    {
      bState=SendMessage(hWndAssociateOpen, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesOpen, bState);
      if (!bState) SetWindowTextW(hWndFileTypesOpen, wszFileTypesOpen);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_EDIT)
    {
      bState=SendMessage(hWndAssociateEdit, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesEdit, bState);
      if (!bState) SetWindowTextW(hWndFileTypesEdit, wszFileTypesEdit);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_PRINT)
    {
      bState=SendMessage(hWndAssociatePrint, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesPrint, bState);
      if (!bState) SetWindowTextW(hWndFileTypesPrint, wszFileTypesPrint);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WORD_DELIMITERS_ENABLE)
    {
      bState=SendMessage(hWndDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndDelimiters, bState);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WORD_DELIMITERS_RESET)
    {
      EscapeDataToEscapeStringW(WORD_DELIMITERSW, wbuf);
      SetDlgItemTextW(hDlg, IDC_OPTIONS_WORD_DELIMITERS, wbuf);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      wchar_t wszWindowText[MAX_PATH];
      BOOL bShellRefresh=FALSE;
      int a;
      int b;

      //Associations
      bState=SendMessage(hWndAssociateOpen, BM_GETCHECK, 0, 0);
      if (bState && !(dwFileTypesAssociated & AE_OPEN))
      {
        GetWindowTextW(hWndFileTypesOpen, wszFileTypesOpen, MAX_PATH);
        AssociateFileTypesW(hInstance, wszFileTypesOpen, AE_OPEN|AE_ASSOCIATE);
        dwFileTypesAssociated|=AE_OPEN;
        bShellRefresh=TRUE;
      }
      else if (bState && (dwFileTypesAssociated & AE_OPEN))
      {
        GetWindowTextW(hWndFileTypesOpen, wszWindowText, MAX_PATH);
        if (lstrcmpiW(wszWindowText, wszFileTypesOpen))
        {
          AssociateFileTypesW(hInstance, wszFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
          AssociateFileTypesW(hInstance, wszWindowText, AE_OPEN|AE_ASSOCIATE);
          lstrcpynW(wszFileTypesOpen, wszWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (dwFileTypesAssociated & AE_OPEN))
      {
        AssociateFileTypesW(hInstance, wszFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
        dwFileTypesAssociated&=~AE_OPEN;
        bShellRefresh=TRUE;
      }

      bState=SendMessage(hWndAssociateEdit, BM_GETCHECK, 0, 0);
      if (bState && !(dwFileTypesAssociated & AE_EDIT))
      {
        GetWindowTextW(hWndFileTypesEdit, wszFileTypesEdit, MAX_PATH);
        AssociateFileTypesW(hInstance, wszFileTypesEdit, AE_EDIT|AE_ASSOCIATE);
        dwFileTypesAssociated|=AE_EDIT;
        bShellRefresh=TRUE;
      }
      else if (bState && (dwFileTypesAssociated & AE_EDIT))
      {
        GetWindowTextW(hWndFileTypesEdit, wszWindowText, MAX_PATH);
        if (lstrcmpiW(wszWindowText, wszFileTypesEdit))
        {
          AssociateFileTypesW(hInstance, wszFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
          AssociateFileTypesW(hInstance, wszWindowText, AE_EDIT|AE_ASSOCIATE);
          lstrcpynW(wszFileTypesEdit, wszWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (dwFileTypesAssociated & AE_EDIT))
      {
        AssociateFileTypesW(hInstance, wszFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
        dwFileTypesAssociated&=~AE_EDIT;
        bShellRefresh=TRUE;
      }

      bState=SendMessage(hWndAssociatePrint, BM_GETCHECK, 0, 0);
      if (bState && !(dwFileTypesAssociated & AE_PRINT))
      {
        GetWindowTextW(hWndFileTypesPrint, wszFileTypesPrint, MAX_PATH);
        AssociateFileTypesW(hInstance, wszFileTypesPrint, AE_PRINT|AE_ASSOCIATE);
        dwFileTypesAssociated|=AE_PRINT;
        bShellRefresh=TRUE;
      }
      else if (bState && (dwFileTypesAssociated & AE_PRINT))
      {
        GetWindowTextW(hWndFileTypesPrint, wszWindowText, MAX_PATH);
        if (lstrcmpiW(wszWindowText, wszFileTypesPrint))
        {
          AssociateFileTypesW(hInstance, wszFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
          AssociateFileTypesW(hInstance, wszWindowText, AE_PRINT|AE_ASSOCIATE);
          lstrcpynW(wszFileTypesPrint, wszWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (dwFileTypesAssociated & AE_PRINT))
      {
        AssociateFileTypesW(hInstance, wszFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
        dwFileTypesAssociated&=~AE_PRINT;
        bShellRefresh=TRUE;
      }
      if (bShellRefresh) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);

      //Tab stops
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, NULL, FALSE);
      if (nTabStopSize != a)
      {
        nTabStopSize=a;
        SetTabStopsW(hWndEdit, nTabStopSize, TRUE);
      }
      bTabStopAsSpaces=SendMessage(hWndTabSizeSpaces, BM_GETCHECK, 0, 0);

      //Undo
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, NULL, FALSE);
      if (nUndoLimit != a)
      {
        nUndoLimit=a;
        SendMessage(hWndEdit, EM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
      }
      bDetailedUndo=SendMessage(hWndDetailedUndo, BM_GETCHECK, 0, 0);

      //Margins
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, NULL, FALSE);
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, NULL, FALSE);
      if (dwEditMargins != (DWORD)MAKELONG(a, b))
      {
        dwEditMargins=MAKELONG(a, b);
        SendMessage(hWndEdit, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
        InvalidateRect(hWndEdit, NULL, FALSE);
      }

      //Word delimiters
      a=SendMessage(hWndDelimitersEnable, BM_GETCHECK, 0, 0);
      if (a != bWordDelimitersEnable)
      {
        bWordDelimitersEnable=a;

        if (bWordDelimitersEnable)
        {
          SendMessage(hWndEdit, EM_SETWORDBREAKPROC, 0, (LPARAM)EditWordBreakProc);
        }
        else
        {
          if (bOldRichEdit)
            bOptionsRestart=TRUE;
          else
            SendMessage(hWndEdit, EM_SETWORDBREAKPROC, 0, (LPARAM)NULL);
        }
      }
      GetWindowTextW(hWndDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, wszWordDelimiters);

      //Save settings
      if (SendMessage(hWndSaveRegistry, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=SS_REGISTRY;
      else
        a=SS_INI;
      if (nSaveSettings != a)
      {
        nSaveSettings=a;
        nRegSaveSettings=a;
        bOptionsSave=TRUE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsAdvanced2DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndSavePositions;
  static HWND hWndSaveCodepages;
  static HWND hWndShowURL;
  static HWND hWndSingleClickURL;
  static HWND hWndDoubleClickURL;
  static HWND hWndReplaceAllAndClose;
  static HWND hWndSaveInReadOnlyMsg;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndSavePositions=GetDlgItem(hDlg, IDC_OPTIONS_SAVEPOSITIONS);
    hWndSaveCodepages=GetDlgItem(hDlg, IDC_OPTIONS_SAVECODEPAGES);
    hWndShowURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SHOW);
    hWndSingleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SINGLECLICK);
    hWndDoubleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_DOUBLECLICK);
    hWndReplaceAllAndClose=GetDlgItem(hDlg, IDC_OPTIONS_REPLACEALL_CLOSE);
    hWndSaveInReadOnlyMsg=GetDlgItem(hDlg, IDC_OPTIONS_SAVEIN_READONLY_MSG);

    if (bSavePositions)
      SendMessage(hWndSavePositions, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveCodepages)
      SendMessage(hWndSaveCodepages, BM_SETCHECK, BST_CHECKED, 0);
    if (bShowURL)
      SendMessage(hWndShowURL, BM_SETCHECK, BST_CHECKED, 0);
    if (nClickURL == 1)
      SendMessage(hWndSingleClickURL, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndDoubleClickURL, BM_SETCHECK, BST_CHECKED, 0);
    if (bReplaceAllAndClose)
      SendMessage(hWndReplaceAllAndClose, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveInReadOnlyMsg)
      SendMessage(hWndSaveInReadOnlyMsg, BM_SETCHECK, BST_CHECKED, 0);
    PostMessage(hDlg, WM_COMMAND, IDC_OPTIONS_URL_SHOW, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_URL_SHOW)
    {
      bState=SendMessage(hWndShowURL, BM_GETCHECK, 0, 0);
      EnableWindow(hWndSingleClickURL, bState);
      EnableWindow(hWndDoubleClickURL, bState);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;
      int b;

      //Recent files
      a=SendMessage(hWndSavePositions, BM_GETCHECK, 0, 0);
      b=SendMessage(hWndSaveCodepages, BM_GETCHECK, 0, 0);
      if (a != bSavePositions || b != bSaveCodepages)
      {
        bSavePositions=a;
        bSaveCodepages=b;
        FreeMemoryRecentFilesA();

        if (nRecentFiles)
        {
          if (RecentFilesAllocA())
          {
            RecentFilesZeroA();
            RecentFilesReadA();
          }
        }
        bMenuRecentFiles=TRUE;

        RegClearKeyA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Recent");
        RecentFilesSaveA();
      }

      //HyperLinks
      a=SendMessage(hWndShowURL, BM_GETCHECK, 0, 0);
      if (a != bShowURL)
      {
        bShowURL=a;
        ShowURL(hWndEdit, bShowURL);
        bOptionsRestart=TRUE;
      }
      if (SendMessage(hWndSingleClickURL, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nClickURL=1;
      else
        nClickURL=2;

      //ReplaceAll and close dialog
      bReplaceAllAndClose=SendMessage(hWndReplaceAllAndClose, BM_GETCHECK, 0, 0);

      //Save in read only file message
      bSaveInReadOnlyMsg=SendMessage(hWndSaveInReadOnlyMsg, BM_GETCHECK, 0, 0);
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsAdvanced2DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndSavePositions;
  static HWND hWndSaveCodepages;
  static HWND hWndShowURL;
  static HWND hWndSingleClickURL;
  static HWND hWndDoubleClickURL;
  static HWND hWndReplaceAllAndClose;
  static HWND hWndSaveInReadOnlyMsg;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndSavePositions=GetDlgItem(hDlg, IDC_OPTIONS_SAVEPOSITIONS);
    hWndSaveCodepages=GetDlgItem(hDlg, IDC_OPTIONS_SAVECODEPAGES);
    hWndShowURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SHOW);
    hWndSingleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SINGLECLICK);
    hWndDoubleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_DOUBLECLICK);
    hWndReplaceAllAndClose=GetDlgItem(hDlg, IDC_OPTIONS_REPLACEALL_CLOSE);
    hWndSaveInReadOnlyMsg=GetDlgItem(hDlg, IDC_OPTIONS_SAVEIN_READONLY_MSG);

    if (bSavePositions)
      SendMessage(hWndSavePositions, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveCodepages)
      SendMessage(hWndSaveCodepages, BM_SETCHECK, BST_CHECKED, 0);
    if (bShowURL)
      SendMessage(hWndShowURL, BM_SETCHECK, BST_CHECKED, 0);
    if (nClickURL == 1)
      SendMessage(hWndSingleClickURL, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndDoubleClickURL, BM_SETCHECK, BST_CHECKED, 0);
    if (bReplaceAllAndClose)
      SendMessage(hWndReplaceAllAndClose, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveInReadOnlyMsg)
      SendMessage(hWndSaveInReadOnlyMsg, BM_SETCHECK, BST_CHECKED, 0);
    PostMessage(hDlg, WM_COMMAND, IDC_OPTIONS_URL_SHOW, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_URL_SHOW)
    {
      bState=SendMessage(hWndShowURL, BM_GETCHECK, 0, 0);
      EnableWindow(hWndSingleClickURL, bState);
      EnableWindow(hWndDoubleClickURL, bState);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;
      int b;

      //Recent files
      a=SendMessage(hWndSavePositions, BM_GETCHECK, 0, 0);
      b=SendMessage(hWndSaveCodepages, BM_GETCHECK, 0, 0);
      if (a != bSavePositions || b != bSaveCodepages)
      {
        bSavePositions=a;
        bSaveCodepages=b;
        FreeMemoryRecentFilesW();

        if (nRecentFiles)
        {
          if (RecentFilesAllocW())
          {
            RecentFilesZeroW();
            RecentFilesReadW();
          }
        }
        bMenuRecentFiles=TRUE;

        RegClearKeyW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Recent");
        RecentFilesSaveW();
      }

      //HyperLinks
      a=SendMessage(hWndShowURL, BM_GETCHECK, 0, 0);
      if (a != bShowURL)
      {
        bShowURL=a;
        ShowURL(hWndEdit, bShowURL);
        bOptionsRestart=TRUE;
      }
      if (SendMessage(hWndSingleClickURL, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nClickURL=1;
      else
        nClickURL=2;

      //ReplaceAll and close dialog
      bReplaceAllAndClose=SendMessage(hWndReplaceAllAndClose, BM_GETCHECK, 0, 0);

      //Save in read only file message
      bSaveInReadOnlyMsg=SendMessage(hWndSaveInReadOnlyMsg, BM_GETCHECK, 0, 0);
    }
  }
  return FALSE;
}

//// MDI list

BOOL CALLBACK MdiListDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndStats;
  static HWND hWndSearch;
  static HWND hWndOK;
  static HWND hWndArrangeGroup;
  static HWND hWndHorz;
  static HWND hWndVert;
  static HWND hWndTabsGroup;
  static HWND hWndUp;
  static HWND hWndDown;
  static HWND hWndSort;
  static HWND hWndModified;
  static HWND hWndFilesGroup;
  static HWND hWndSave;
  static HWND hWndClose;
  static HWND hWndCancel;
  static BOOL bListChanged;
  static BOOL bOnlyModified;
  static DIALOGRESIZE drs[]={{&hWndList,         DRS_SIZE|DRS_X|DRS_Y},
                             {&hWndStats,        DRS_MOVE|DRS_X|DRS_Y},
                             {&hWndSearch,       DRS_SIZE|DRS_X},
                             {&hWndSearch,       DRS_MOVE|DRS_Y},
                             {&hWndOK,           DRS_MOVE|DRS_X},
                             {&hWndArrangeGroup, DRS_MOVE|DRS_X},
                             {&hWndHorz,         DRS_MOVE|DRS_X},
                             {&hWndVert,         DRS_MOVE|DRS_X},
                             {&hWndTabsGroup,    DRS_MOVE|DRS_X},
                             {&hWndUp,           DRS_MOVE|DRS_X},
                             {&hWndDown,         DRS_MOVE|DRS_X},
                             {&hWndSort,         DRS_MOVE|DRS_X},
                             {&hWndModified,     DRS_MOVE|DRS_X},
                             {&hWndFilesGroup,   DRS_MOVE|DRS_X},
                             {&hWndSave,         DRS_MOVE|DRS_X},
                             {&hWndClose,        DRS_MOVE|DRS_X},
                             {&hWndCancel,       DRS_MOVE|DRS_X|DRS_Y},
                             {0, 0}};
  int nItem;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndList=GetDlgItem(hDlg, IDC_MDILIST_LIST);
    hWndStats=GetDlgItem(hDlg, IDC_MDILIST_STATS);
    hWndSearch=GetDlgItem(hDlg, IDC_MDILIST_SEARCH);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndArrangeGroup=GetDlgItem(hDlg, IDC_MDILIST_ARRANGE_GROUP);
    hWndHorz=GetDlgItem(hDlg, IDC_MDILIST_HORZ);
    hWndVert=GetDlgItem(hDlg, IDC_MDILIST_VERT);
    hWndTabsGroup=GetDlgItem(hDlg, IDC_MDILIST_TABS_GROUP);
    hWndUp=GetDlgItem(hDlg, IDC_MDILIST_UP);
    hWndDown=GetDlgItem(hDlg, IDC_MDILIST_DOWN);
    hWndSort=GetDlgItem(hDlg, IDC_MDILIST_SORT);
    hWndModified=GetDlgItem(hDlg, IDC_MDILIST_ONLYMODIFIED);
    hWndFilesGroup=GetDlgItem(hDlg, IDC_MDILIST_FILES_GROUP);
    hWndSave=GetDlgItem(hDlg, IDC_MDILIST_SAVE);
    hWndClose=GetDlgItem(hDlg, IDC_MDILIST_CLOSE);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SendMessage(hWndSearch, EM_LIMITTEXT, MAX_PATH, 0);

    FillMdiListListboxA(hWndList, FALSE, FALSE);
    if ((nItem=SendMessage(hTab, TCM_GETCURSEL, 0, 0)) != -1)
      SendMessage(hWndList, LB_SETSEL, TRUE, nItem);

    PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_MDILIST_SEARCH)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        char szSearch[MAX_PATH];

        if (GetWindowTextA(hWndSearch, szSearch, MAX_PATH))
        {
          for (nItem=0; 1; ++nItem)
          {
            if (SendMessageA(hWndList, LB_GETTEXT, nItem, (LPARAM)buf) == LB_ERR)
              break;
            if (xstrstrA(buf, (DWORD)-1, szSearch, FALSE, NULL, NULL))
            {
              SendMessage(hWndList, LB_SETSEL, FALSE, -1);
              SendMessage(hWndList, LB_SETSEL, TRUE, nItem);
              break;
            }
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_MDILIST_HORZ)
    {
      ArrangeListboxSelItems(hWndList, SB_HORZ);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_VERT)
    {
      ArrangeListboxSelItems(hWndList, SB_VERT);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_UP)
    {
      if (ShiftListboxSelItems(hWndList, FALSE))
        bListChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDC_MDILIST_DOWN)
    {
      if (ShiftListboxSelItems(hWndList, TRUE))
        bListChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDC_MDILIST_SORT)
    {
      FillMdiListListboxA(hWndList, TRUE, bOnlyModified);
      bListChanged=TRUE;

      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_ONLYMODIFIED)
    {
      bOnlyModified=SendMessage(hWndModified, BM_GETCHECK, 0, 0);
      FillMdiListListboxA(hWndList, FALSE, bOnlyModified);
      bListChanged=FALSE;

      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_SAVE)
    {
      SaveListboxSelItems(hWndList);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_CLOSE)
    {
      if (CloseListboxSelItems(hWndList))
        SetFocus(hWndList);

      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_LIST)
    {
      if (HIWORD(wParam) == LBN_SELCHANGE)
      {
        int nCount;
        int nSelCount;

        nCount=SendMessage(hWndList, LB_GETCOUNT, 0, 0);
        nSelCount=SendMessage(hWndList, LB_GETSELCOUNT, 0, 0);
        //EnableWindow(hWndHorz, nSelCount > 1);
        //EnableWindow(hWndVert, nSelCount > 1);
        EnableWindow(hWndUp, nSelCount > 0 && !bOnlyModified);
        EnableWindow(hWndDown, nSelCount > 0 && !bOnlyModified);
        EnableWindow(hWndSort, !bOnlyModified);
        EnableWindow(hWndSave, nSelCount > 0);
        EnableWindow(hWndClose, nSelCount > 0);

        wsprintfA(buf, "%d / %d", nSelCount, nCount);
        SetWindowTextA(hWndStats, buf);
      }
      else if (HIWORD(wParam) == LBN_DBLCLK)
      {
        PostMessage(hDlg, WM_COMMAND, IDOK, 0);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      int *lpSelItems;
      int nItemNew;
      int nItemOld;
      int nData;

      if (!bOnlyModified && bListChanged)
      {
        for (nItemNew=0; 1; ++nItemNew)
        {
          if ((nData=SendMessage(hWndList, LB_GETITEMDATA, nItemNew, 0)) == LB_ERR)
            break;
          if ((nItemOld=GetTabItemFromParam(hTab, nData)) != -1)
            MoveTabItemA(hTab, nItemOld, nItemNew);
        }
        bListChanged=FALSE;
      }
      if (GetListboxSelItems(hWndList, &lpSelItems))
      {
        if ((nData=SendMessage(hWndList, LB_GETITEMDATA, lpSelItems[0], 0)) != LB_ERR)
          if ((nItem=GetTabItemFromParam(hTab, nData)) != -1)
            SelectTabItem(hTab, nItem);
        FreeListboxSelItems(&lpSelItems);
      }
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  DialogResizeMessages(&drs[0], &rcMdiListInitDialog, &rcMdiListCurrentDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

BOOL CALLBACK MdiListDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndStats;
  static HWND hWndSearch;
  static HWND hWndOK;
  static HWND hWndArrangeGroup;
  static HWND hWndHorz;
  static HWND hWndVert;
  static HWND hWndTabsGroup;
  static HWND hWndUp;
  static HWND hWndDown;
  static HWND hWndSort;
  static HWND hWndModified;
  static HWND hWndFilesGroup;
  static HWND hWndSave;
  static HWND hWndClose;
  static HWND hWndCancel;
  static BOOL bListChanged;
  static BOOL bOnlyModified;
  static DIALOGRESIZE drs[]={{&hWndList,         DRS_SIZE|DRS_X|DRS_Y},
                             {&hWndStats,        DRS_MOVE|DRS_X|DRS_Y},
                             {&hWndSearch,       DRS_SIZE|DRS_X},
                             {&hWndSearch,       DRS_MOVE|DRS_Y},
                             {&hWndOK,           DRS_MOVE|DRS_X},
                             {&hWndArrangeGroup, DRS_MOVE|DRS_X},
                             {&hWndHorz,         DRS_MOVE|DRS_X},
                             {&hWndVert,         DRS_MOVE|DRS_X},
                             {&hWndTabsGroup,    DRS_MOVE|DRS_X},
                             {&hWndUp,           DRS_MOVE|DRS_X},
                             {&hWndDown,         DRS_MOVE|DRS_X},
                             {&hWndSort,         DRS_MOVE|DRS_X},
                             {&hWndModified,     DRS_MOVE|DRS_X},
                             {&hWndFilesGroup,   DRS_MOVE|DRS_X},
                             {&hWndSave,         DRS_MOVE|DRS_X},
                             {&hWndClose,        DRS_MOVE|DRS_X},
                             {&hWndCancel,       DRS_MOVE|DRS_X|DRS_Y},
                             {0, 0}};
  int nItem;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndList=GetDlgItem(hDlg, IDC_MDILIST_LIST);
    hWndStats=GetDlgItem(hDlg, IDC_MDILIST_STATS);
    hWndSearch=GetDlgItem(hDlg, IDC_MDILIST_SEARCH);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndArrangeGroup=GetDlgItem(hDlg, IDC_MDILIST_ARRANGE_GROUP);
    hWndHorz=GetDlgItem(hDlg, IDC_MDILIST_HORZ);
    hWndVert=GetDlgItem(hDlg, IDC_MDILIST_VERT);
    hWndTabsGroup=GetDlgItem(hDlg, IDC_MDILIST_TABS_GROUP);
    hWndUp=GetDlgItem(hDlg, IDC_MDILIST_UP);
    hWndDown=GetDlgItem(hDlg, IDC_MDILIST_DOWN);
    hWndSort=GetDlgItem(hDlg, IDC_MDILIST_SORT);
    hWndModified=GetDlgItem(hDlg, IDC_MDILIST_ONLYMODIFIED);
    hWndFilesGroup=GetDlgItem(hDlg, IDC_MDILIST_FILES_GROUP);
    hWndSave=GetDlgItem(hDlg, IDC_MDILIST_SAVE);
    hWndClose=GetDlgItem(hDlg, IDC_MDILIST_CLOSE);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SendMessage(hWndSearch, EM_LIMITTEXT, MAX_PATH, 0);

    FillMdiListListboxW(hWndList, FALSE, FALSE);
    if ((nItem=SendMessage(hTab, TCM_GETCURSEL, 0, 0)) != -1)
      SendMessage(hWndList, LB_SETSEL, TRUE, nItem);

    PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_MDILIST_SEARCH)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        wchar_t wszSearch[MAX_PATH];

        if (GetWindowTextW(hWndSearch, wszSearch, MAX_PATH))
        {
          for (nItem=0; 1; ++nItem)
          {
            if (SendMessageW(hWndList, LB_GETTEXT, nItem, (LPARAM)wbuf) == LB_ERR)
              break;
            if (xstrstrW(wbuf, (DWORD)-1, wszSearch, FALSE, NULL, NULL))
            {
              SendMessage(hWndList, LB_SETSEL, FALSE, -1);
              SendMessage(hWndList, LB_SETSEL, TRUE, nItem);
              break;
            }
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_MDILIST_HORZ)
    {
      ArrangeListboxSelItems(hWndList, SB_HORZ);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_VERT)
    {
      ArrangeListboxSelItems(hWndList, SB_VERT);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_UP)
    {
      if (ShiftListboxSelItems(hWndList, FALSE))
        bListChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDC_MDILIST_DOWN)
    {
      if (ShiftListboxSelItems(hWndList, TRUE))
        bListChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDC_MDILIST_SORT)
    {
      FillMdiListListboxW(hWndList, TRUE, bOnlyModified);
      bListChanged=TRUE;

      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_ONLYMODIFIED)
    {
      bOnlyModified=SendMessage(hWndModified, BM_GETCHECK, 0, 0);
      FillMdiListListboxW(hWndList, FALSE, bOnlyModified);
      bListChanged=FALSE;

      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_SAVE)
    {
      SaveListboxSelItems(hWndList);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_CLOSE)
    {
      if (CloseListboxSelItems(hWndList))
        SetFocus(hWndList);

      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_LIST)
    {
      if (HIWORD(wParam) == LBN_SELCHANGE)
      {
        int nCount;
        int nSelCount;

        nCount=SendMessage(hWndList, LB_GETCOUNT, 0, 0);
        nSelCount=SendMessage(hWndList, LB_GETSELCOUNT, 0, 0);
        //EnableWindow(hWndHorz, nSelCount > 1);
        //EnableWindow(hWndVert, nSelCount > 1);
        EnableWindow(hWndUp, nSelCount > 0 && !bOnlyModified);
        EnableWindow(hWndDown, nSelCount > 0 && !bOnlyModified);
        EnableWindow(hWndSort, !bOnlyModified);
        EnableWindow(hWndSave, nSelCount > 0);
        EnableWindow(hWndClose, nSelCount > 0);

        wsprintfW(wbuf, L"%d / %d", nSelCount, nCount);
        SetWindowTextW(hWndStats, wbuf);
      }
      else if (HIWORD(wParam) == LBN_DBLCLK)
      {
        PostMessage(hDlg, WM_COMMAND, IDOK, 0);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      int *lpSelItems;
      int nItemNew;
      int nItemOld;
      int nData;

      if (!bOnlyModified && bListChanged)
      {
        for (nItemNew=0; 1; ++nItemNew)
        {
          if ((nData=SendMessage(hWndList, LB_GETITEMDATA, nItemNew, 0)) == LB_ERR)
            break;
          if ((nItemOld=GetTabItemFromParam(hTab, nData)) != -1)
            MoveTabItemW(hTab, nItemOld, nItemNew);
        }
        bListChanged=FALSE;
      }
      if (GetListboxSelItems(hWndList, &lpSelItems))
      {
        if ((nData=SendMessage(hWndList, LB_GETITEMDATA, lpSelItems[0], 0)) != LB_ERR)
          if ((nItem=GetTabItemFromParam(hTab, nData)) != -1)
            SelectTabItem(hTab, nItem);
        FreeListboxSelItems(&lpSelItems);
      }
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  DialogResizeMessages(&drs[0], &rcMdiListInitDialog, &rcMdiListCurrentDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

void FillMdiListListboxA(HWND hWnd, BOOL bSort, BOOL bOnlyModified)
{
  TCITEMA tcItemA;
  WNDFRAMEA *wf;
  int nNew;
  int nTabItem;
  int nListboxItem=0;
  BOOL bFileModified;

  SendMessage(hWnd, LB_RESETCONTENT, 0, 0);

  for (nTabItem=0; 1; ++nTabItem)
  {
    tcItemA.mask=TCIF_PARAM;
    if (!SendMessage(hTab, TCM_GETITEMA, nTabItem, (LPARAM)&tcItemA))
      break;

    GetWindowTextA((HWND)tcItemA.lParam, buf, MAX_PATH);

    if (hWndFrameActive == (HWND)tcItemA.lParam)
      bFileModified=bModified;
    else if (wf=(WNDFRAMEA *)GetWindowLongA((HWND)tcItemA.lParam, GWL_USERDATA))
      bFileModified=wf->ei.bModified;
    else
      bFileModified=FALSE;
    if (bFileModified) lstrcatA(buf, " *");

    if (!bOnlyModified || bFileModified)
    {
      if (bSort)
        nNew=SendMessageA(hWnd, LB_ADDSTRING, 0, (LPARAM)buf);
      else
        nNew=SendMessageA(hWnd, LB_INSERTSTRING, nListboxItem++, (LPARAM)buf);
      SendMessage(hWnd, LB_SETITEMDATA, nNew, tcItemA.lParam);
    }
  }
}

void FillMdiListListboxW(HWND hWnd, BOOL bSort, BOOL bOnlyModified)
{
  TCITEMW tcItemW;
  WNDFRAMEW *wf;
  int nNew;
  int nTabItem;
  int nListboxItem=0;
  BOOL bFileModified;

  SendMessage(hWnd, LB_RESETCONTENT, 0, 0);

  for (nTabItem=0; 1; ++nTabItem)
  {
    tcItemW.mask=TCIF_PARAM;
    if (!SendMessage(hTab, TCM_GETITEMW, nTabItem, (LPARAM)&tcItemW))
      break;

    GetWindowTextW((HWND)tcItemW.lParam, wbuf, MAX_PATH);

    if (hWndFrameActive == (HWND)tcItemW.lParam)
      bFileModified=bModified;
    else if (wf=(WNDFRAMEW *)GetWindowLongW((HWND)tcItemW.lParam, GWL_USERDATA))
      bFileModified=wf->ei.bModified;
    else
      bFileModified=FALSE;
    if (bFileModified) lstrcatW(wbuf, L" *");

    if (!bOnlyModified || bFileModified)
    {
      if (bSort)
        nNew=SendMessageW(hWnd, LB_ADDSTRING, 0, (LPARAM)wbuf);
      else
        nNew=SendMessageW(hWnd, LB_INSERTSTRING, nListboxItem++, (LPARAM)wbuf);
      SendMessage(hWnd, LB_SETITEMDATA, nNew, tcItemW.lParam);
    }
  }
}

int MoveListboxItemA(HWND hWnd, int nOldIndex, int nNewIndex)
{
  char *pText;
  int nIndex=LB_ERR;
  int nData;
  int nTextLen;

  if ((nTextLen=SendMessage(hWnd, LB_GETTEXTLEN, nOldIndex, 0)) != LB_ERR)
  {
    if (pText=(char *)API_HeapAlloc(hHeap, 0, nTextLen + 1))
    {
      SendMessageA(hWnd, LB_GETTEXT, nOldIndex, (LPARAM)pText);
      nData=SendMessage(hWnd, LB_GETITEMDATA, nOldIndex, 0);

      SendMessage(hWnd, LB_DELETESTRING, nOldIndex, 0);
      nIndex=SendMessageA(hWnd, LB_INSERTSTRING, nNewIndex, (LPARAM)pText);
      SendMessage(hWnd, LB_SETITEMDATA, nIndex, nData);
      API_HeapFree(hHeap, 0, (LPVOID)pText);
    }
  }
  return nIndex;
}

int MoveListboxItemW(HWND hWnd, int nOldIndex, int nNewIndex)
{
  wchar_t *wpText;
  int nIndex=LB_ERR;
  int nData;
  int nTextLen;

  if ((nTextLen=SendMessage(hWnd, LB_GETTEXTLEN, nOldIndex, 0)) != LB_ERR)
  {
    if (wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, (nTextLen + 1) * sizeof(wchar_t)))
    {
      SendMessageW(hWnd, LB_GETTEXT, nOldIndex, (LPARAM)wpText);
      nData=SendMessage(hWnd, LB_GETITEMDATA, nOldIndex, 0);

      SendMessage(hWnd, LB_DELETESTRING, nOldIndex, 0);
      nIndex=SendMessageW(hWnd, LB_INSERTSTRING, nNewIndex, (LPARAM)wpText);
      SendMessage(hWnd, LB_SETITEMDATA, nIndex, nData);
      API_HeapFree(hHeap, 0, (LPVOID)wpText);
    }
  }
  return nIndex;
}

BOOL ShiftListboxSelItems(HWND hWnd, BOOL bMoveDown)
{
  int *lpSelItems;
  int nSelCount;
  int nMinIndex;
  int nMaxIndex;
  int nOldIndex=-1;
  int nNewIndex=-1;
  int i;
  BOOL bResult=FALSE;

  nMinIndex=0;
  nMaxIndex=SendMessage(hWnd, LB_GETCOUNT, 0, 0) - 1;

  if (nSelCount=GetListboxSelItems(hWnd, &lpSelItems))
  {
    if (!bMoveDown)
    {
      for (i=0; i < nSelCount; ++i)
      {
        if (lpSelItems[i] > nMinIndex)
        {
          if (nNewIndex == -1 && i > 0)
          {
            if (lpSelItems[i] - 1 <= lpSelItems[i - 1])
              continue;
          }
          nOldIndex=lpSelItems[i];
          nNewIndex=lpSelItems[i] - 1;

          if (bOldWindows)
            MoveListboxItemA(hWnd, nOldIndex, nNewIndex);
          else
            MoveListboxItemW(hWnd, nOldIndex, nNewIndex);
          SendMessage(hWnd, LB_SETSEL, TRUE, nNewIndex);
          bResult=TRUE;
        }
      }
    }
    else
    {
      for (i=--nSelCount; i >= 0; --i)
      {
        if (lpSelItems[i] < nMaxIndex)
        {
          if (nNewIndex == -1 && i < nSelCount)
          {
            if (lpSelItems[i] + 1 >= lpSelItems[i + 1])
              continue;
          }
          nOldIndex=lpSelItems[i];
          nNewIndex=lpSelItems[i] + 1;

          if (bOldWindows)
            MoveListboxItemA(hWnd, nOldIndex, nNewIndex);
          else
            MoveListboxItemW(hWnd, nOldIndex, nNewIndex);
          SendMessage(hWnd, LB_SETSEL, TRUE, nNewIndex);
          bResult=TRUE;
        }
      }
    }
    FreeListboxSelItems(&lpSelItems);
  }
  return bResult;
}

BOOL SaveListboxSelItems(HWND hWnd)
{
  int *lpSelItems;
  int nData;
  int nSelCount;
  int nItem;
  int i;
  BOOL bResult=TRUE;

  if (nSelCount=GetListboxSelItems(hWnd, &lpSelItems))
  {
    for (i=nSelCount - 1; i >= 0; --i)
    {
      if ((nData=SendMessage(hWnd, LB_GETITEMDATA, lpSelItems[i], 0)) != LB_ERR)
      {
        if ((nItem=GetTabItemFromParam(hTab, nData)) != -1)
        {
          SelectTabItem(hTab, nItem);

          if (bOldWindows)
          {
            if (!DoFileSaveA())
            {
              bResult=FALSE;
              break;
            }
            GetWindowTextA((HWND)nData, buf, MAX_PATH);
            SendMessage(hWnd, LB_DELETESTRING, lpSelItems[i], 0);
            SendMessageA(hWnd, LB_INSERTSTRING, lpSelItems[i], (LPARAM)buf);
          }
          else
          {
            if (!DoFileSaveW())
            {
              bResult=FALSE;
              break;
            }
            GetWindowTextW((HWND)nData, wbuf, MAX_PATH);
            SendMessage(hWnd, LB_DELETESTRING, lpSelItems[i], 0);
            SendMessageW(hWnd, LB_INSERTSTRING, lpSelItems[i], (LPARAM)wbuf);
          }
          SendMessage(hWnd, LB_SETITEMDATA, lpSelItems[i], nData);
          SendMessage(hWnd, LB_SETSEL, TRUE, lpSelItems[i]);
        }
      }
    }
    FreeListboxSelItems(&lpSelItems);
  }
  return bResult;
}

void ArrangeListboxSelItems(HWND hWnd, int nBar)
{
  RECT rcClient;
  int *lpSelItems;
  int nHeight;
  int nWidth;
  int nData;
  int nSelCount;
  int nItem;
  int i;

  if (nSelCount=GetListboxSelItems(hWnd, &lpSelItems))
  {
    GetClientRect(hMdiClient, &rcClient);
    if (nBar == SB_HORZ)
      rcClient.top=rcClient.bottom - rcClient.bottom / nSelCount;
    else if (nBar == SB_VERT)
      rcClient.left=rcClient.right - rcClient.right / nSelCount;

    for (i=nSelCount - 1; i >= 0; --i)
    {
      if ((nData=SendMessage(hWnd, LB_GETITEMDATA, lpSelItems[i], 0)) != LB_ERR)
      {
        if ((nItem=GetTabItemFromParam(hTab, nData)) != -1)
        {
          SelectTabItem(hTab, nItem);
          if (GetWindowLongA((HWND)nData, GWL_STYLE) & WS_MAXIMIZE)
            SendMessage(hMdiClient, WM_MDIRESTORE, nData, 0);
          MoveWindow((HWND)nData, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);

          if (nBar == SB_HORZ)
          {
            nHeight=rcClient.bottom - rcClient.top;
            rcClient.top-=nHeight;
            rcClient.bottom-=nHeight;
          }
          else if (nBar == SB_VERT)
          {
            nWidth=rcClient.right - rcClient.left;
            rcClient.left-=nWidth;
            rcClient.right-=nWidth;
          }
        }
      }
    }
    FreeListboxSelItems(&lpSelItems);
  }
}

BOOL CloseListboxSelItems(HWND hWnd)
{
  int *lpSelItems;
  int nData;
  int nSelCount;
  int nItem;
  int i;
  BOOL bResult=TRUE;

  if (nSelCount=GetListboxSelItems(hWnd, &lpSelItems))
  {
    for (i=nSelCount - 1; i >= 0; --i)
    {
      if ((nData=SendMessage(hWnd, LB_GETITEMDATA, lpSelItems[i], 0)) != LB_ERR)
      {
        if ((nItem=GetTabItemFromParam(hTab, nData)) != -1)
        {
          SelectTabItem(hTab, nItem);

          SendMessage(hMdiClient, WM_MDIDESTROY, (WPARAM)hWndFrameActive, 0);
          if (!bFileExitError)
          {
            bResult=FALSE;
            break;
          }
          SendMessage(hWnd, LB_DELETESTRING, lpSelItems[i], 0);
        }
      }
    }
    FreeListboxSelItems(&lpSelItems);
  }
  return bResult;
}

int GetListboxSelItems(HWND hWnd, int **lpSelItems)
{
  int nSelCount;

  if (lpSelItems)
  {
    nSelCount=SendMessage(hWnd, LB_GETSELCOUNT, 0, 0);

    if (*lpSelItems=(int *)API_HeapAlloc(hHeap, 0, nSelCount * sizeof(int)))
    {
      return SendMessage(hWnd, LB_GETSELITEMS, nSelCount, (LPARAM)*lpSelItems);
    }
  }
  return 0;
}

void FreeListboxSelItems(int **lpSelItems)
{
  if (lpSelItems && *lpSelItems)
  {
    API_HeapFree(hHeap, 0, (LPVOID)*lpSelItems);
    *lpSelItems=NULL;
  }
}


//// About

BOOL CALLBACK AboutDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    SetDlgItemTextA(hDlg, IDC_ABOUT_VERSION, APP_ABOUT_VERSIONA);
    SetDlgItemTextA(hDlg, IDC_ABOUT_HOMEPAGE, APP_ABOUT_HOMEPAGEA);
    SetDlgItemTextA(hDlg, IDC_ABOUT_EMAIL_SHENGALTS, APP_ABOUT_EMAIL_SHENGALTSA);
    SetDlgItemTextA(hDlg, IDC_ABOUT_EMAIL_KUZNETSOV, APP_ABOUT_EMAIL_KUZNETSOVA);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_ABOUT_HOMEPAGE)
    {
      ShellExecuteA(GetParent(hDlg), "open", APP_ABOUT_HOMEPAGEA, NULL, NULL, SW_MAXIMIZE);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK AboutDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    SetDlgItemTextW(hDlg, IDC_ABOUT_VERSION, APP_ABOUT_VERSIONW);
    SetDlgItemTextW(hDlg, IDC_ABOUT_HOMEPAGE, APP_ABOUT_HOMEPAGEW);
    SetDlgItemTextW(hDlg, IDC_ABOUT_EMAIL_SHENGALTS, APP_ABOUT_EMAIL_SHENGALTSW);
    SetDlgItemTextW(hDlg, IDC_ABOUT_EMAIL_KUZNETSOV, APP_ABOUT_EMAIL_KUZNETSOVW);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_ABOUT_HOMEPAGE)
    {
      ShellExecuteW(GetParent(hDlg), L"open", APP_ABOUT_HOMEPAGEW, NULL, NULL, SW_MAXIMIZE);
      return TRUE;
    }
  }
  return FALSE;
}


//// Status bar

void SetSelectionStatusA(HWND hWnd, CHARRANGE *cr)
{
  char szStatus[MAX_PATH];
  int nLine;
  int nColumn;
  int nLinebreaks=0;

  if (cr)
  {
    nPositionOld=nPosition;
    nPosition=(cr->cpMin == chrg.cpMin || cr->cpMin == chrg.cpMax)?cr->cpMax:cr->cpMin;
    chrg=*cr;
  }
  else
  {
    SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&chrg);
    nPositionOld=chrg.cpMax;
    nPosition=chrg.cpMax;
  }
  nLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nPosition) + 1;
  nColumn=SendMessage(hWnd, EM_LINEINDEX, nLine - 1, 0);
  nColumn=nPosition - nColumn + 1;

  if (chrg.cpMin == chrg.cpMax)
  {
    wsprintfA(szStatus, "%u:%u", nLine, nColumn);
  }
  else
  {
    if (nNewLine == NEWLINE_WIN) nLinebreaks=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, chrg.cpMax) - SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, chrg.cpMin);
    wsprintfA(szStatus, "%u:%u, %u", nLine, nColumn, chrg.cpMax - chrg.cpMin + nLinebreaks);
  }
  SendMessage(hStatus, SB_SETTEXTA, STATUS_POSITION, (LPARAM)szStatus);
}

void SetSelectionStatusW(HWND hWnd, CHARRANGE *cr)
{
  wchar_t wszStatus[MAX_PATH];
  int nLine;
  int nColumn;
  int nLinebreaks=0;

  if (cr)
  {
    nPositionOld=nPosition;
    nPosition=(cr->cpMin == chrg.cpMin || cr->cpMin == chrg.cpMax)?cr->cpMax:cr->cpMin;
    chrg=*cr;
  }
  else
  {
    SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&chrg);
    nPositionOld=chrg.cpMax;
    nPosition=chrg.cpMax;
  }
  nLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nPosition) + 1;
  nColumn=SendMessage(hWnd, EM_LINEINDEX, nLine - 1, 0);
  nColumn=nPosition - nColumn + 1;

  if (chrg.cpMin == chrg.cpMax)
  {
    wsprintfW(wszStatus, L"%u:%u", nLine, nColumn);
  }
  else
  {
    if (nNewLine == NEWLINE_WIN) nLinebreaks=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, chrg.cpMax) - SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, chrg.cpMin);
    wsprintfW(wszStatus, L"%u:%u, %u", nLine, nColumn, chrg.cpMax - chrg.cpMin + nLinebreaks);
  }
  SendMessage(hStatus, SB_SETTEXTW, STATUS_POSITION, (LPARAM)wszStatus);
}

void SetModifyStatusA(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (!hWnd) hWnd=hWndEdit;

  if (hWnd)
  {
    if (hWnd == hWndEdit)
    {
      if (bFirst != TRUE && bModified == bState) return;
      bModified=bState;

      API_LoadStringA(hLangLib, STR_MODIFIED, buf, BUFFER_SIZE);
      SendMessage(hStatus, SB_SETTEXTA, STATUS_MODIFY, (LPARAM)(bModified?buf:""));
    }
    else
    {
      WNDFRAMEA *wf;
      HWND hWndFrame;

      if (hWndFrame=GetParent(hWnd))
      {
        if (wf=(WNDFRAMEA *)GetWindowLongA(hWndFrame, GWL_USERDATA))
          wf->ei.bModified=bState;
      }
    }
  }
}

void SetModifyStatusW(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (!hWnd) hWnd=hWndEdit;

  if (hWnd)
  {
    if (hWnd == hWndEdit)
    {
      if (bFirst != TRUE && bModified == bState) return;
      bModified=bState;

      API_LoadStringW(hLangLib, STR_MODIFIED, wbuf, BUFFER_SIZE);
      SendMessage(hStatus, SB_SETTEXTW, STATUS_MODIFY, (LPARAM)(bModified?wbuf:L""));
    }
    else
    {
      WNDFRAMEW *wf;
      HWND hWndFrame;

      if (hWndFrame=GetParent(hWnd))
      {
        if (wf=(WNDFRAMEW *)GetWindowLongW(hWndFrame, GWL_USERDATA))
          wf->ei.bModified=bState;
      }
    }
  }
}

void SetNewLineStatusA(HWND hWnd, int nState, BOOL bFirst)
{
  if (!hWnd) hWnd=hWndEdit;

  if (hWnd)
  {
    if (hWnd == hWndEdit)
    {
      NMHDR nmhdr={hWndEdit, ID_EDIT, EN_SELCHANGE};
      HWND hWndParent;

      if (bFirst != TRUE && nNewLine == nState) return;
      nNewLine=nState;

      if (nNewLine == NEWLINE_WIN)
        SendMessage(hStatus, SB_SETTEXTA, STATUS_NEWLINE, (LPARAM)"Win");
      else if (nNewLine == NEWLINE_UNIX)
        SendMessage(hStatus, SB_SETTEXTA, STATUS_NEWLINE, (LPARAM)"Unix");

      if (hWndParent=GetParent(hWnd))
        SendMessage(hWndParent, WM_NOTIFY, ID_EDIT, (LPARAM)&nmhdr);
    }
    else
    {
      WNDFRAMEA *wf;
      HWND hWndFrame;

      if (hWndFrame=GetParent(hWnd))
      {
        if (wf=(WNDFRAMEA *)GetWindowLongA(hWndFrame, GWL_USERDATA))
          wf->ei.nNewLine=nState;
      }
    }
  }
}

void SetNewLineStatusW(HWND hWnd, int nState, BOOL bFirst)
{
  if (!hWnd) hWnd=hWndEdit;

  if (hWnd)
  {
    if (hWnd == hWndEdit)
    {
      NMHDR nmhdr={hWndEdit, ID_EDIT, EN_SELCHANGE};
      HWND hWndParent;

      if (bFirst != TRUE && nNewLine == nState) return;
      nNewLine=nState;

      if (nNewLine == NEWLINE_WIN)
        SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)L"Win");
      else if (nNewLine == NEWLINE_UNIX)
        SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)L"Unix");

      if (hWndParent=GetParent(hWnd))
        SendMessage(hWndParent, WM_NOTIFY, ID_EDIT, (LPARAM)&nmhdr);
    }
    else
    {
      WNDFRAMEW *wf;
      HWND hWndFrame;

      if (hWndFrame=GetParent(hWnd))
      {
        if (wf=(WNDFRAMEW *)GetWindowLongW(hWndFrame, GWL_USERDATA))
          wf->ei.nNewLine=nState;
      }
    }
  }
}

void SetInsertStateStatusA(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (bFirst != TRUE && bOvertypeMode == bState) return;
  bOvertypeMode=bState;

  if (hWnd)
  {
    if (GetKeyState(VK_SHIFT) >= 0 && GetKeyState(VK_CONTROL) >= 0 && GetKeyState(VK_MENU) >= 0)
    {
      bOvertypeModeChange=TRUE;
      SendMessage(hWnd, WM_KEYDOWN, VK_INSERT, 0x00520001);
      SendMessage(hWnd, WM_KEYUP, VK_INSERT, 0xC0520001);
    }
    else return;
  }
  SendMessage(hStatus, SB_SETTEXTA, STATUS_INSERT, (LPARAM)(bOvertypeMode?"Ovr":"Ins"));
}

void SetInsertStateStatusW(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (bFirst != TRUE && bOvertypeMode == bState) return;
  bOvertypeMode=bState;

  if (hWnd)
  {
    if (GetKeyState(VK_SHIFT) >= 0 && GetKeyState(VK_CONTROL) >= 0 && GetKeyState(VK_MENU) >= 0)
    {
      bOvertypeModeChange=TRUE;
      SendMessage(hWnd, WM_KEYDOWN, VK_INSERT, 0x00520001);
      SendMessage(hWnd, WM_KEYUP, VK_INSERT, 0xC0520001);
    }
    else return;
  }
  SendMessage(hStatus, SB_SETTEXTW, STATUS_INSERT, (LPARAM)(bOvertypeMode?L"Ovr":L"Ins"));
}

void SetCodePageStatusA(int nCodePage, BOOL bBOM, BOOL bFirst)
{
  if (bFirst != TRUE && nCurrentCodePage == nCodePage && bCurrentBOM == bBOM) return;
  nCurrentCodePage=nCodePage;
  bCurrentBOM=bBOM;
  GetCodePageNameA(nCurrentCodePage, buf, BUFFER_SIZE);

  if (nCurrentCodePage == CP_UNICODE_UCS2_LE ||
      nCurrentCodePage == CP_UNICODE_UCS2_BE ||
      nCurrentCodePage == CP_UNICODE_UTF8)
  {
    if (!bCurrentBOM)
    {
      lstrcatA(buf, STR_NOBOMA);
    }
  }
  SendMessage(hStatus, SB_SETTEXTA, STATUS_CODEPAGE, (LPARAM)buf);
}

void SetCodePageStatusW(int nCodePage, BOOL bBOM, BOOL bFirst)
{
  if (bFirst != TRUE && nCurrentCodePage == nCodePage && bCurrentBOM == bBOM) return;
  nCurrentCodePage=nCodePage;
  bCurrentBOM=bBOM;
  GetCodePageNameW(nCurrentCodePage, wbuf, BUFFER_SIZE);

  if (nCurrentCodePage == CP_UNICODE_UCS2_LE ||
      nCurrentCodePage == CP_UNICODE_UCS2_BE ||
      nCurrentCodePage == CP_UNICODE_UTF8)
  {
    if (!bCurrentBOM)
    {
      lstrcatW(wbuf, STR_NOBOMW);
    }
  }
  SendMessage(hStatus, SB_SETTEXTW, STATUS_CODEPAGE, (LPARAM)wbuf);
}


//// Other functions

int _SetLayout(HWND hWnd)
{
  HWND hWndOldFocus;
  DWORD dwLayout;

  if (bOldRichEdit)
  {
    dwLayout=LOWORD(GetKeyboardLayout(0));
    if (dwLayout != dwLangSystem && !ActivateKeyboardLayout((HKL)dwLangSystem, 0))
      dwLayout=0;
  }
  else
  {
    hWndOldFocus=GetFocus();
    if (hWndOldFocus != hWnd) SetFocus(hWnd);
    dwLayout=LOWORD(GetKeyboardLayout(0));
    if (dwLayout != dwLangSystem && !ActivateKeyboardLayout((HKL)dwLangSystem, 0))
      dwLayout=0;
    if (hWndOldFocus != hWnd) SetFocus(hWndOldFocus);
  }
  return dwLayout;
}

void RestoreLayout(HWND hWnd, DWORD dwLayout)
{
  HWND hWndOldFocus;

  if (bOldRichEdit)
  {
    if (dwLayout) ActivateKeyboardLayout((HKL)dwLayout, 0);
  }
  else
  {
    hWndOldFocus=GetFocus();
    if (hWndOldFocus != hWnd) SetFocus(hWnd);
    if (dwLayout) ActivateKeyboardLayout((HKL)dwLayout, 0);
    if (hWndOldFocus != hWnd) SetFocus(hWndOldFocus);
  }
}

void SaveCharScroll(HWND hWnd, RECT *rc, int *nBeforeChar)
{
  SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)rc);
  *nBeforeChar=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)rc);
}

void RestoreCharScroll(HWND hWnd, RECT *rc, int *nBeforeChar)
{
  int nAfterChar;
  int nBeforeLine;
  int nAfterLine;

  nAfterChar=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)rc);

  if (*nBeforeChar != nAfterChar)
  {
    nBeforeLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, *nBeforeChar);
    nAfterLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nAfterChar);

    if (nBeforeLine != nAfterLine)
    {
      if (bOldRichEdit)
      {
        SendMessage(hWnd, EM_LINESCROLL, 0, -1);
        SendMessage(hWnd, EM_LINESCROLL, 0, nBeforeLine);
      }
      else SendMessage(hWnd, EM_LINESCROLL, 0, nBeforeLine - nAfterLine);
    }
  }
}

void SaveLineScroll(HWND hWnd, RECT *rc, int *nBeforeLine)
{
  int nBeforeChar;

  SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)rc);
  nBeforeChar=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)rc);
  *nBeforeLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nBeforeChar);
}

void RestoreLineScroll(HWND hWnd, RECT *rc, int *nBeforeLine)
{
  int nAfterChar;
  int nAfterLine;

  nAfterChar=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)rc);
  nAfterLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nAfterChar);

  if (*nBeforeLine != nAfterLine)
  {
    if (bOldRichEdit)
    {
      SendMessage(hWnd, EM_LINESCROLL, 0, -1);
      SendMessage(hWnd, EM_LINESCROLL, 0, *nBeforeLine);
    }
    else SendMessage(hWnd, EM_LINESCROLL, 0, *nBeforeLine - nAfterLine);
  }
}

BOOL SetChosenFontA(HWND hWnd, LOGFONTA *lfA)
{
  CHARFORMATA cfmtA;
  RECT rcEdit;
  int nFirstVisibleChar;
  BOOL bModifiedTmp=FALSE;
  BOOL bResult;

  //Reserve state
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  //Set font
  cfmtA.cbSize=sizeof(CHARFORMATA);
  cfmtA.dwMask=CFM_SIZE|CFM_CHARSET|CFM_FACE|CFM_BOLD|CFM_ITALIC;
  cfmtA.yHeight=mod(PixelsToTwips(hWnd, lfA->lfHeight));
  cfmtA.bCharSet=lfA->lfCharSet;
  cfmtA.bPitchAndFamily=lfA->lfPitchAndFamily;
  lstrcpynA(cfmtA.szFaceName, lfA->lfFaceName, LF_FACESIZE);
  cfmtA.dwEffects=0;
  if (lfA->lfWeight >= FW_BOLD)
    cfmtA.dwEffects|=CFE_BOLD;
  if (lfA->lfItalic)
    cfmtA.dwEffects|=CFE_ITALIC;

  bResult=SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cfmtA);

  //Update tab stops
  SetTabStopsA(hWnd, nTabStopSize, FALSE);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusA(hWndEdit, bModifiedTmp, FALSE);
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
  return bResult;
}

BOOL SetChosenFontW(HWND hWnd, LOGFONTW *lfW)
{
  CHARFORMATW cfmtW;
  RECT rcEdit;
  int nFirstVisibleChar;
  BOOL bModifiedTmp=FALSE;
  BOOL bResult;

  //Reserve state
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  //Set font
  cfmtW.cbSize=sizeof(CHARFORMATW);
  cfmtW.dwMask=CFM_SIZE|CFM_CHARSET|CFM_FACE|CFM_BOLD|CFM_ITALIC;
  cfmtW.yHeight=mod(PixelsToTwips(hWnd, lfW->lfHeight));
  cfmtW.bCharSet=lfW->lfCharSet;
  cfmtW.bPitchAndFamily=lfW->lfPitchAndFamily;
  lstrcpynW(cfmtW.szFaceName, lfW->lfFaceName, LF_FACESIZE);
  cfmtW.dwEffects=0;
  if (lfW->lfWeight >= FW_BOLD)
    cfmtW.dwEffects|=CFE_BOLD;
  if (lfW->lfItalic)
    cfmtW.dwEffects|=CFE_ITALIC;

  bResult=SendMessageW(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cfmtW);

  //Update tab stops
  SetTabStopsW(hWnd, nTabStopSize, FALSE);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusW(hWndEdit, bModifiedTmp, FALSE);
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
  return bResult;
}

void SetChosenFontColorA(HWND hWnd, COLORREF crFont)
{
  CHARFORMATA cfmtA;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  //Set font color
  cfmtA.cbSize=sizeof(CHARFORMATA);
  cfmtA.dwMask=CFM_COLOR;
  cfmtA.dwEffects=0;
  cfmtA.crTextColor=crFont;

  SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cfmtA);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusA(hWndEdit, bModifiedTmp, FALSE);
}

void SetChosenFontColorW(HWND hWnd, COLORREF crFont)
{
  CHARFORMATW cfmtW;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  //Set font color
  cfmtW.cbSize=sizeof(CHARFORMATW);
  cfmtW.dwMask=CFM_COLOR;
  cfmtW.dwEffects=0;
  cfmtW.crTextColor=crFont;

  SendMessageW(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cfmtW);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusW(hWndEdit, bModifiedTmp, FALSE);
}

void SetChosenFontSizeA(HWND hWnd, int nPixelHeight)
{
  CHARFORMATA cfmtA;
  RECT rcEdit;
  int nFirstVisibleChar;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  //Set font size
  cfmtA.cbSize=sizeof(CHARFORMATA);
  cfmtA.dwMask=CFM_SIZE;
  cfmtA.yHeight=mod(PixelsToTwips(hWnd, nPixelHeight));

  SendMessageA(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cfmtA);

  //Update tab stops
  SetTabStopsA(hWnd, nTabStopSize, FALSE);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusA(hWndEdit, bModifiedTmp, FALSE);
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
}

void SetChosenFontSizeW(HWND hWnd, int nPixelHeight)
{
  CHARFORMATW cfmtW;
  RECT rcEdit;
  int nFirstVisibleChar;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  if (hWnd == hWndEdit) bModifiedTmp=bModified;

  //Set font size
  cfmtW.cbSize=sizeof(CHARFORMATW);
  cfmtW.dwMask=CFM_SIZE;
  cfmtW.yHeight=mod(PixelsToTwips(hWnd, nPixelHeight));

  SendMessageW(hWnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cfmtW);

  //Update tab stops
  SetTabStopsW(hWnd, nTabStopSize, FALSE);

  //Restore state
  if (hWnd == hWndEdit) SetModifyStatusW(hWndEdit, bModifiedTmp, FALSE);
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, FALSE);
}

void SetTabStopsA(HWND hWnd, int nTabStops, BOOL bSetRedraw)
{
  PARAFORMAT2 pf;
  CHARRANGE cr;
  RECT rcEdit;
  int nWidthTwips;
  int nFirstVisibleChar;
  int i;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  if (bSetRedraw)
  {
    SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
    SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
    if (hWnd == hWndEdit) bModifiedTmp=bModified;
  }

  if (nWidthTwips=GetCharWidthTwipsA(hWnd) * nTabStops)
  {
    pf.cbSize=sizeof(PARAFORMAT2);
    pf.dwMask=PFM_TABSTOPS;
    pf.cTabCount=MAX_TAB_STOPS;

    for(i=0; i < pf.cTabCount; ++i)
      pf.rgxTabs[i]=(i + 1) * nWidthTwips;

    //From KankowskiEdit
    SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
    SetTextSel(hWnd, 0, -1);
    SendMessage(hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
    SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
  }

  //Restore state
  if (bSetRedraw)
  {
    if (hWnd == hWndEdit) SetModifyStatusA(hWndEdit, bModifiedTmp, FALSE);
    RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
    SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWnd, NULL, FALSE);
  }
}

void SetTabStopsW(HWND hWnd, int nTabStops, BOOL bSetRedraw)
{
  PARAFORMAT2 pf;
  CHARRANGE cr;
  RECT rcEdit;
  int nWidthTwips;
  int nFirstVisibleChar;
  int i;
  BOOL bModifiedTmp=FALSE;

  //Reserve state
  if (bSetRedraw)
  {
    SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
    SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
    if (hWnd == hWndEdit) bModifiedTmp=bModified;
  }

  if (nWidthTwips=GetCharWidthTwipsW(hWnd) * nTabStops)
  {
    pf.cbSize=sizeof(PARAFORMAT2);
    pf.dwMask=PFM_TABSTOPS;
    pf.cTabCount=MAX_TAB_STOPS;

    for(i=0; i < pf.cTabCount; ++i)
      pf.rgxTabs[i]=(i + 1) * nWidthTwips;

    //From KankowskiEdit
    SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
    SetTextSel(hWnd, 0, -1);
    SendMessage(hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
    SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
  }

  //Restore state
  if (bSetRedraw)
  {
    if (hWnd == hWndEdit) SetModifyStatusW(hWndEdit, bModifiedTmp, FALSE);
    RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
    SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWnd, NULL, FALSE);
  }
}

int GetCharWidthTwipsA(HWND hWnd)
{
  HDC hDC;
  HFONT hFont;
  HFONT hFontOld;
  int nWidth=0;

  if (hDC=GetDC(hWnd))
  {
    if (hFont=CreateFontIndirectA(&lfEditFontA))
    {
      hFontOld=(HFONT)SelectObject(hDC, hFont);

      if (GetCharWidth(hDC, 'A', 'A', &nWidth))
        nWidth=MulDiv(nWidth, 1440, GetDeviceCaps(hDC, LOGPIXELSX));

      if (hFontOld) SelectObject(hDC, hFontOld);
      DeleteObject(hFont);
    }
    ReleaseDC(hWnd, hDC);
  }
  return nWidth;
}

int GetCharWidthTwipsW(HWND hWnd)
{
  HDC hDC;
  HFONT hFont;
  HFONT hFontOld;
  int nWidth=0;

  if (hDC=GetDC(hWnd))
  {
    if (hFont=CreateFontIndirectW(&lfEditFontW))
    {
      hFontOld=(HFONT)SelectObject(hDC, hFont);

      if (GetCharWidthW(hDC, 'A', 'A', &nWidth))
        nWidth=MulDiv(nWidth, 1440, GetDeviceCaps(hDC, LOGPIXELSX));

      if (hFontOld) SelectObject(hDC, hFontOld);
      DeleteObject(hFont);
    }
    ReleaseDC(hWnd, hDC);
  }
  return nWidth;
}


BOOL GetCharColor(HWND hWnd, CHARCOLOR *cc)
{
  CHARRANGE cr;

  if (!hWnd) hWnd=hWndEdit;

  SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);

  if (cc->nCharPos >= cr.cpMin && cc->nCharPos < cr.cpMax)
  {
    cc->crText=aecColors.crSelText;
    cc->crBk=aecColors.crSelBk;
    return TRUE;
  }
  else
  {
    cc->crText=aecColors.crBasicText;
    cc->crBk=aecColors.crBasicBk;
  }
  return FALSE;
}

BOOL InsertTabStopW(HWND hWnd)
{
  wchar_t wszSpaces[MAX_PATH];
  int nMaxSpaces;
  int i;

  if (IsReadOnly()) return FALSE;

  if (bTabStopAsSpaces)
  {
    nMaxSpaces=min(nTabStopSize, MAX_PATH - 1);

    for (i=0; i < nMaxSpaces; ++i)
      wszSpaces[i]=' ';
    wszSpaces[i]='\0';
    ReplaceSelW(hWnd, wszSpaces, -1);
  }
  else
    SendMessage(hWnd, WM_CHAR, (WPARAM)'\t', 0);

  return TRUE;
}

BOOL IndentTabStopW(HWND hWnd, int nAction)
{
  wchar_t wszSpaces[MAX_PATH];
  int nMaxSpaces;
  int i;

  if (IsReadOnly()) return FALSE;

  if (bTabStopAsSpaces)
  {
    nMaxSpaces=min(nTabStopSize, MAX_PATH - 1);

    for (i=0; i < nMaxSpaces; ++i)
      wszSpaces[i]=' ';
    wszSpaces[i]='\0';
    DoEditInsertStringInSelectionW(hWnd, nAction, wszSpaces);
  }
  else
    DoEditInsertStringInSelectionW(hWnd, nAction, L"\t");

  return TRUE;
}

BOOL AutoIndentA(HWND hWnd, CHARRANGE *cr)
{
  TEXTRANGEA txtrngA;
  char *pText;
  int nMinLine;
  int nMinLineIndex;
  int nLen;
  int i;

  nMinLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, cr->cpMin);
  nMinLineIndex=SendMessage(hWnd, EM_LINEINDEX, nMinLine, 0);

  if (nMinLineIndex != cr->cpMin)
  {
    nLen=(cr->cpMin - nMinLineIndex) + 1;

    if (pText=(char *)API_HeapAlloc(hHeap, 0, nLen + 1))
    {
      txtrngA.chrg.cpMin=nMinLineIndex;
      txtrngA.chrg.cpMax=cr->cpMin;
      txtrngA.lpstrText=pText + 1;
      SendMessageA(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&txtrngA);

      pText[0]='\n';
      for (i=1; pText[i] == ' ' || pText[i] == '\t'; ++i);
      pText[i]='\0';

      SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)pText);
      API_HeapFree(hHeap, 0, (LPVOID)pText);
      return TRUE;
    }
  }
  SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"\n");
  return FALSE;
}

BOOL AutoIndentW(HWND hWnd, CHARRANGE *cr)
{
  TEXTRANGEW txtrngW;
  wchar_t *wpText;
  int nMinLine;
  int nMinLineIndex;
  int nLen;
  int i;

  nMinLine=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, cr->cpMin);
  nMinLineIndex=SendMessage(hWnd, EM_LINEINDEX, nMinLine, 0);

  if (nMinLineIndex != cr->cpMin)
  {
    nLen=(cr->cpMin - nMinLineIndex) + 1;

    if (wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, nLen * sizeof(wchar_t) + 2))
    {
      txtrngW.chrg.cpMin=nMinLineIndex;
      txtrngW.chrg.cpMax=cr->cpMin;
      txtrngW.lpstrText=wpText + 1;
      SendMessageW(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&txtrngW);

      wpText[0]='\n';
      for (i=1; wpText[i] == ' ' || wpText[i] == '\t'; ++i);
      wpText[i]='\0';

      SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)wpText);
      API_HeapFree(hHeap, 0, (LPVOID)wpText);
      return TRUE;
    }
  }
  SendMessageW(hWnd, EM_REPLACESEL, TRUE, (LPARAM)L"\n");
  return FALSE;
}

int PixelsToTwips(HWND hWnd, int nPixels)
{
  HDC hDC;
  int nTwips=0;

  if (hDC=GetDC(hWnd))
  {
    nTwips=MulDiv(nPixels, 1440, GetDeviceCaps(hDC, LOGPIXELSY));
    ReleaseDC(hWnd, hDC);
  }
  return nTwips;
}

char* GetCommandLineParamsA()
{
  char *lpCmdLine=GetCommandLineA();

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

int GetCommandLineArgA(char *pCmdLine, char *szArgName, int nArgNameLen, char **pArgOption, int *nArgOptionLen, char **pNextArg, BOOL bParseAsNotepad)
{
  char *pCount=pCmdLine;
  char *pArgName=szArgName;
  char *pArgNameMax=szArgName + nArgNameLen - 1;
  char chStopChar;
  BOOL bArgName=TRUE;

  if (pArgOption) *pArgOption=NULL;
  if (nArgOptionLen) *nArgOptionLen=0;
  while (*pCount == ' ') ++pCount;

  if (*pCount == '/')
  {
    for (chStopChar=' '; *pCount != chStopChar && *pCount != '\0'; ++pCount)
    {
      if (bArgName)
      {
        if (*pCount == '=')
        {
          ++pCount;
          if (*pCount == '\"' || *pCount == '\'' || *pCount == '`')
            chStopChar=*pCount;
          if (pArgOption) *pArgOption=pCount;
          --pCount;
          bArgName=FALSE;
        }
        else
        {
          if (pArgName < pArgNameMax)
          {
            if (szArgName) *pArgName=*pCount;
            ++pArgName;
          }
        }
      }
    }
    if (*pCount == '\"' || *pCount == '\'' || *pCount == '`')
      ++pCount;
  }
  else if (*pCount == '\"')
  {
    for (++pCount; *pCount != '\"' && *pCount != '\0'; ++pCount)
    {
      if (pArgName < pArgNameMax)
      {
        if (szArgName) *pArgName=*pCount;
        ++pArgName;
      }
    }
    if (*pCount == '\"')
      ++pCount;
  }
  else
  {
    if (bParseAsNotepad)
    {
      for (; *pCount != '\"' && *pCount != '\0'; ++pCount)
      {
        if (pArgName < pArgNameMax)
        {
          if (szArgName) *pArgName=*pCount;
          ++pArgName;
        }
      }
      if (pArgName < pArgNameMax)
      {
        if (szArgName)
        {
          while (*--pArgName == ' ') *pArgName='\0';
          ++pArgName;
        }
      }
    }
    else
    {
      for (; *pCount != ' ' && *pCount != '\0'; ++pCount)
      {
        if (pArgName < pArgNameMax)
        {
          if (szArgName) *pArgName=*pCount;
          ++pArgName;
        }
      }
    }
  }
  if (szArgName) *pArgName='\0';

  if (pNextArg)
    for (*pNextArg=pCount; **pNextArg == ' '; ++*pNextArg);
  if (pArgOption && *pArgOption && nArgOptionLen)
    *nArgOptionLen=pCount - *pArgOption;

  return pArgName - szArgName;
}

int GetCommandLineArgW(wchar_t *wpCmdLine, wchar_t *wszArgName, int nArgNameLen, wchar_t **wpArgOption, int *nArgOptionLen, wchar_t **wpNextArg, BOOL bParseAsNotepad)
{
  wchar_t *wpCount=wpCmdLine;
  wchar_t *wpArgName=wszArgName;
  wchar_t *wpArgNameMax=wszArgName + nArgNameLen - 1;
  wchar_t wchStopChar;
  BOOL bArgName=TRUE;

  if (wpArgOption) *wpArgOption=NULL;
  if (nArgOptionLen) *nArgOptionLen=0;
  while (*wpCount == ' ') ++wpCount;

  if (*wpCount == '/')
  {
    for (wchStopChar=' '; *wpCount != wchStopChar && *wpCount != '\0'; ++wpCount)
    {
      if (bArgName)
      {
        if (*wpCount == '=')
        {
          ++wpCount;
          if (*wpCount == '\"' || *wpCount == '\'' || *wpCount == '`')
            wchStopChar=*wpCount;
          if (wpArgOption) *wpArgOption=wpCount;
          --wpCount;
          bArgName=FALSE;
        }
        else
        {
          if (wpArgName < wpArgNameMax)
          {
            if (wszArgName) *wpArgName=*wpCount;
            ++wpArgName;
          }
        }
      }
    }
    if (*wpCount == '\"' || *wpCount == '\'' || *wpCount == '`')
      ++wpCount;
  }
  else if (*wpCount == '\"')
  {
    for (++wpCount; *wpCount != '\"' && *wpCount != '\0'; ++wpCount)
    {
      if (wpArgName < wpArgNameMax)
      {
        if (wszArgName) *wpArgName=*wpCount;
        ++wpArgName;
      }
    }
    if (*wpCount == '\"')
      ++wpCount;
  }
  else
  {
    if (bParseAsNotepad)
    {
      for (; *wpCount != '\"' && *wpCount != '\0'; ++wpCount)
      {
        if (wpArgName < wpArgNameMax)
        {
          if (wszArgName) *wpArgName=*wpCount;
          ++wpArgName;
        }
      }
      if (wpArgName < wpArgNameMax)
      {
        if (wszArgName)
        {
          while (*--wpArgName == ' ') *wpArgName='\0';
          ++wpArgName;
        }
      }
    }
    else
    {
      for (; *wpCount != ' ' && *wpCount != '\0'; ++wpCount)
      {
        if (wpArgName < wpArgNameMax)
        {
          if (wszArgName) *wpArgName=*wpCount;
          ++wpArgName;
        }
      }
    }
  }
  if (wszArgName) *wpArgName='\0';

  if (wpNextArg)
    for (*wpNextArg=wpCount; **wpNextArg == ' '; ++*wpNextArg);
  if (wpArgOption && *wpArgOption && nArgOptionLen)
    *nArgOptionLen=wpCount - *wpArgOption;

  return wpArgName - wszArgName;
}

void RegClearKeyA(HKEY hKey, char *pSubKey)
{
  HKEY hOpenKey;
  DWORD dwSize;
  DWORD dwType;

  if (RegOpenKeyExA(hKey, pSubKey, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hOpenKey) == ERROR_SUCCESS)
  {
    while (1)
    {
      dwSize=BUFFER_SIZE;
      if (RegEnumValueA(hOpenKey, 0, buf, &dwSize, NULL, &dwType, NULL, NULL) != ERROR_SUCCESS)
        break;
      if (RegDeleteValueA(hOpenKey, buf) != ERROR_SUCCESS)
        break;
    }
    RegCloseKey(hOpenKey);
  }
}

void RegClearKeyW(HKEY hKey, wchar_t *wpSubKey)
{
  HKEY hOpenKey;
  DWORD dwSize;
  DWORD dwType;

  if (RegOpenKeyExW(hKey, wpSubKey, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hOpenKey) == ERROR_SUCCESS)
  {
    while (1)
    {
      dwSize=BUFFER_SIZE;
      if (RegEnumValueW(hOpenKey, 0, wbuf, &dwSize, NULL, &dwType, NULL, NULL) != ERROR_SUCCESS)
        break;
      if (RegDeleteValueW(hOpenKey, wbuf) != ERROR_SUCCESS)
        break;
    }
    RegCloseKey(hOpenKey);
  }
}

void GetAssociatedIconA(char *pExt, char *szFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall)
{
  char *pFileName;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int nIndex=0;
  int i;
  int j;

  if (pExt=GetFileExtA(pExt))
  {
    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, pExt - 1, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      dwSize=BUFFER_SIZE;
      RegQueryValueExA(hKey, "", NULL, &dwType, (LPBYTE)buf, &dwSize);
      RegCloseKey(hKey);

      wsprintfA(buf2, "%s\\DefaultIcon", buf);

      if (RegOpenKeyExA(HKEY_CLASSES_ROOT, buf2, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
      {
        dwSize=BUFFER_SIZE;
        RegQueryValueExA(hKey, "", NULL, &dwType, (LPBYTE)buf2, &dwSize);
        RegCloseKey(hKey);

        for (i=0, j=0; buf2[i]; ++i)
          if (buf2[i] != '\"') buf[j++]=buf2[i];
        buf[j]='\0';

        for (i=lstrlenA(buf) - 1; i > 0; --i)
        {
          if (buf[i] == ',')
          {
            buf[i]='\0';
            nIndex=xatoiA(buf + i + 1);
            if (nIndex == -1) nIndex=0;
            break;
          }
          else if (buf[i] == '-' || (buf[i] >= '0' && buf[i] <= '9'))
            continue;
          break;
        }
        ExpandEnvironmentStringsA(buf, buf2, BUFFER_SIZE);
        if (SearchPathA(NULL, buf2, NULL, BUFFER_SIZE, buf, &pFileName))
        {
          (*ExtractIconExAPtr)(buf, nIndex, phiconLarge, phiconSmall, 1);
          if (szFile) lstrcpynA(szFile, buf, MAX_PATH);
          if (nIconIndex) *nIconIndex=nIndex;
          return;
        }
      }
    }
  }
  if (szFile) szFile[0]='\0';
  if (nIconIndex) *nIconIndex=0;
  if (phiconLarge) *phiconLarge=0;
  if (phiconSmall) *phiconSmall=0;
}

void GetAssociatedIconW(wchar_t *wpExt, wchar_t *wszFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall)
{
  wchar_t *wpFileName;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int nIndex=0;
  int i;
  int j;

  if (wpExt=GetFileExtW(wpExt))
  {
    if (RegOpenKeyExW(HKEY_CLASSES_ROOT, wpExt - 1, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      dwSize=BUFFER_SIZE * sizeof(wchar_t);
      RegQueryValueExW(hKey, L"", NULL, &dwType, (LPBYTE)wbuf, &dwSize);
      RegCloseKey(hKey);

      wsprintfW(wbuf2, L"%s\\DefaultIcon", wbuf);

      if (RegOpenKeyExW(HKEY_CLASSES_ROOT, wbuf2, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
      {
        dwSize=BUFFER_SIZE * sizeof(wchar_t);
        RegQueryValueExW(hKey, L"", NULL, &dwType, (LPBYTE)wbuf2, &dwSize);
        RegCloseKey(hKey);

        for (i=0, j=0; wbuf2[i]; ++i)
          if (wbuf2[i] != '\"') wbuf[j++]=wbuf2[i];
        wbuf[j]='\0';

        for (i=lstrlenW(wbuf) - 1; i > 0; --i)
        {
          if (wbuf[i] == ',')
          {
            wbuf[i]='\0';
            nIndex=xatoiW(wbuf + i + 1);
            if (nIndex == -1) nIndex=0;
            break;
          }
          else if (wbuf[i] == '-' || (wbuf[i] >= '0' && wbuf[i] <= '9'))
            continue;
          break;
        }
        ExpandEnvironmentStringsW(wbuf, wbuf2, BUFFER_SIZE);
        if (SearchPathW(NULL, wbuf2, NULL, BUFFER_SIZE, wbuf, &wpFileName))
        {
          (*ExtractIconExWPtr)(wbuf, nIndex, phiconLarge, phiconSmall, 1);
          if (wszFile) lstrcpynW(wszFile, wbuf, MAX_PATH);
          if (nIconIndex) *nIconIndex=nIndex;
          return;
        }
      }
    }
  }
  if (wszFile) wszFile[0]='\0';
  if (nIconIndex) *nIconIndex=0;
  if (phiconLarge) *phiconLarge=0;
  if (phiconSmall) *phiconSmall=0;
}

void AssociateFileTypesA(HINSTANCE hInstance, char *pFileTypes, DWORD dwFlags)
{
  char szModule[MAX_PATH];
  char szAssocKey[MAX_PATH];
  char szTypeKey[16];
  char szExt[16];
  char *pExtStart=pFileTypes;
  char *pExtEnd=pFileTypes;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  GetModuleFileNameA(hInstance, szModule, MAX_PATH);

  if (dwFlags & AE_OPEN) lstrcpyA(szTypeKey, "Open");
  else if (dwFlags & AE_EDIT) lstrcpyA(szTypeKey, "Edit");
  else if (dwFlags & AE_PRINT) lstrcpyA(szTypeKey, "Print");

  while (1)
  {
    if (pExtEnd=strchr(pExtStart, ';'))
      lstrcpynA(buf, pExtStart, pExtEnd - pExtStart + 1);
    else
      lstrcpynA(buf, pExtStart, MAX_PATH);
    if (!*buf) goto NextExt;
    wsprintfA(szExt, ".%s", buf);

    if (RegCreateKeyExA(HKEY_CLASSES_ROOT, szExt, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
    {
      dwSize=MAX_PATH;
      if (RegQueryValueExA(hKey, "", NULL, &dwType, (LPBYTE)szAssocKey, &dwSize) != ERROR_SUCCESS || !*szAssocKey)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          wsprintfA(szAssocKey, "%sfile", szExt + 1);
          RegSetValueExA(hKey, "", 0, REG_SZ, (LPBYTE)szAssocKey, lstrlenA(szAssocKey) + 1);
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          RegCloseKey(hKey);
          return;
        }
      }
      RegCloseKey(hKey);

      //Associate icon
      if (dwFlags & AE_OPEN)
      {
        wsprintfA(buf2, "%s\\DefaultIcon", szAssocKey);

        if (RegCreateKeyExA(HKEY_CLASSES_ROOT, buf2, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
        {
          dwSize=BUFFER_SIZE;
          if (RegQueryValueExA(hKey, "", NULL, &dwType, (LPBYTE)buf2, &dwSize) != ERROR_SUCCESS || !*buf2)
          {
            if (dwFlags & AE_ASSOCIATE) RegSetValueExA(hKey, "", 0, REG_SZ, (LPBYTE)szModule, lstrlenA(szModule) + 1);
          }
          else if (dwFlags & AE_DEASSOCIATE)
          {
            if (!lstrcmpiA(buf2, szModule)) RegDeleteValueA(hKey, "");
          }
          RegCloseKey(hKey);
        }
      }

      //Associate command
      wsprintfA(buf2, "%s\\Shell\\%s\\Command", szAssocKey, szTypeKey);
      wsprintfA(buf, "\"%s\"%s \"%%1\"", szModule, (dwFlags & AE_PRINT)?" /p":"");

      if (RegCreateKeyExA(HKEY_CLASSES_ROOT, buf2, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          dwSize=BUFFER_SIZE;
          if (RegQueryValueExA(hKey, "", NULL, &dwType, (LPBYTE)buf2, &dwSize) == ERROR_SUCCESS)
          {
            if (*buf2 && lstrcmpiA(buf, buf2))
            {
              if (RegQueryValueExA(hKey, "AkelUndo", NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                RegSetValueExA(hKey, "AkelUndo", 0, dwType, (LPBYTE)buf2, dwSize);
            }
          }
          RegSetValueExA(hKey, "", 0, REG_SZ, (LPBYTE)buf, lstrlenA(buf) + 1);
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          dwSize=BUFFER_SIZE;
          if (RegQueryValueExA(hKey, "AkelUndo", NULL, &dwType, (LPBYTE)buf2, &dwSize) == ERROR_SUCCESS)
          {
            if (*buf2) RegSetValueExA(hKey, "", 0, dwType, (LPBYTE)buf2, dwSize);
            RegDeleteValueA(hKey, "AkelUndo");
          }
          dwSize=BUFFER_SIZE;
          if (RegQueryValueExA(hKey, "", NULL, &dwType, (LPBYTE)buf2, &dwSize) == ERROR_SUCCESS)
          {
            if (!*buf2 || !lstrcmpiA(buf, buf2))
            {
              RegCloseKey(hKey);
              wsprintfA(buf2, "%s\\Shell\\%s", szAssocKey, szTypeKey);
              if (RegOpenKeyExA(HKEY_CLASSES_ROOT, buf2, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
              {
                RegDeleteKeyA(hKey, "Command");
                RegCloseKey(hKey);
                wsprintfA(buf2, "%s\\Shell", szAssocKey);
                if (RegOpenKeyExA(HKEY_CLASSES_ROOT, buf2, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
                {
                  RegDeleteKeyA(hKey, szTypeKey);
                  dwSize=BUFFER_SIZE;
                  if (RegEnumKeyExA(hKey, 0, buf2, &dwSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                  {
                    RegCloseKey(hKey);
                    if (RegOpenKeyExA(HKEY_CLASSES_ROOT, szAssocKey, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
                      RegDeleteKeyA(hKey, "Shell");
                  }
                }
              }
            }
          }
        }
        RegCloseKey(hKey);
      }
    }

    //Check off "Always open with this program"
    if (dwFlags & AE_OPEN)
    {
      wsprintfA(buf, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s", szExt);

      if (RegOpenKeyExA(HKEY_CURRENT_USER, buf, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          dwSize=BUFFER_SIZE;
          if (RegQueryValueExA(hKey, "Application", NULL, &dwType, (LPBYTE)buf, &dwSize) == ERROR_SUCCESS)
          {
            if (*buf) RegSetValueExA(hKey, "AkelUndo", 0, dwType, (LPBYTE)buf, dwSize);
            RegDeleteValueA(hKey, "Application");
          }
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          dwSize=BUFFER_SIZE;
          if (RegQueryValueExA(hKey, "AkelUndo", NULL, &dwType, (LPBYTE)buf, &dwSize) == ERROR_SUCCESS)
          {
            if (*buf) RegSetValueExA(hKey, "Application", 0, dwType, (LPBYTE)buf, dwSize);
            RegDeleteValueA(hKey, "AkelUndo");
          }
        }
        RegCloseKey(hKey);
      }
    }
    NextExt:
    pExtStart=pExtEnd + 1;
    if (!pExtEnd) break;
    if (!*pExtStart) break;
  }
}

void AssociateFileTypesW(HINSTANCE hInstance, wchar_t *wpFileTypes, DWORD dwFlags)
{
  wchar_t wszModule[MAX_PATH];
  wchar_t wszAssocKey[MAX_PATH];
  wchar_t wszTypeKey[16];
  wchar_t wszExt[16];
  wchar_t *wpExtStart=wpFileTypes;
  wchar_t *wpExtEnd=wpFileTypes;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  GetModuleFileNameW(hInstance, wszModule, MAX_PATH);

  if (dwFlags & AE_OPEN) lstrcpyW(wszTypeKey, L"Open");
  else if (dwFlags & AE_EDIT) lstrcpyW(wszTypeKey, L"Edit");
  else if (dwFlags & AE_PRINT) lstrcpyW(wszTypeKey, L"Print");

  while (1)
  {
    if (wpExtEnd=wcschr(wpExtStart, ';'))
      lstrcpynW(wbuf, wpExtStart, wpExtEnd - wpExtStart + 1);
    else
      lstrcpynW(wbuf, wpExtStart, MAX_PATH);
    if (!*wbuf) goto NextExt;
    wsprintfW(wszExt, L".%s", wbuf);

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, wszExt, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
    {
      dwSize=MAX_PATH * sizeof(wchar_t);
      if (RegQueryValueExW(hKey, L"", NULL, &dwType, (LPBYTE)wszAssocKey, &dwSize) != ERROR_SUCCESS || !*wszAssocKey)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          wsprintfW(wszAssocKey, L"%sfile", wszExt + 1);
          RegSetValueExW(hKey, L"", 0, REG_SZ, (LPBYTE)wszAssocKey, lstrlenW(wszAssocKey) * sizeof(wchar_t) + 2);
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          RegCloseKey(hKey);
          return;
        }
      }
      RegCloseKey(hKey);

      //Associate icon
      if (dwFlags & AE_OPEN)
      {
        wsprintfW(wbuf2, L"%s\\DefaultIcon", wszAssocKey);

        if (RegCreateKeyExW(HKEY_CLASSES_ROOT, wbuf2, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExW(hKey, L"", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) != ERROR_SUCCESS || !*wbuf2)
          {
            if (dwFlags & AE_ASSOCIATE) RegSetValueExW(hKey, L"", 0, REG_SZ, (LPBYTE)wszModule, lstrlenW(wszModule) * sizeof(wchar_t) + 2);
          }
          else if (dwFlags & AE_DEASSOCIATE)
          {
            if (!lstrcmpiW(wbuf2, wszModule)) RegDeleteValueW(hKey, L"");
          }
          RegCloseKey(hKey);
        }
      }

      //Associate command
      wsprintfW(wbuf2, L"%s\\Shell\\%s\\Command", wszAssocKey, wszTypeKey);
      wsprintfW(wbuf, L"\"%s\"%s \"%%1\"", wszModule, (dwFlags & AE_PRINT)?L" /p":L"");

      if (RegCreateKeyExW(HKEY_CLASSES_ROOT, wbuf2, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExW(hKey, L"", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf2 && lstrcmpiW(wbuf, wbuf2))
            {
              if (RegQueryValueExW(hKey, L"AkelUndo", NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                RegSetValueExW(hKey, L"AkelUndo", 0, dwType, (LPBYTE)wbuf2, dwSize);
            }
          }
          RegSetValueExW(hKey, L"", 0, REG_SZ, (LPBYTE)wbuf, lstrlenW(wbuf) * sizeof(wchar_t) + 2);
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExW(hKey, L"AkelUndo", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf2) RegSetValueExW(hKey, L"", 0, dwType, (LPBYTE)wbuf2, dwSize);
            RegDeleteValueW(hKey, L"AkelUndo");
          }
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExW(hKey, L"", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) == ERROR_SUCCESS)
          {
            if (!*wbuf2 || !lstrcmpiW(wbuf, wbuf2))
            {
              RegCloseKey(hKey);
              wsprintfW(wbuf2, L"%s\\Shell\\%s", wszAssocKey, wszTypeKey);
              if (RegOpenKeyExW(HKEY_CLASSES_ROOT, wbuf2, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
              {
                RegDeleteKeyW(hKey, L"Command");
                RegCloseKey(hKey);
                wsprintfW(wbuf2, L"%s\\Shell", wszAssocKey);
                if (RegOpenKeyExW(HKEY_CLASSES_ROOT, wbuf2, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
                {
                  RegDeleteKeyW(hKey, wszTypeKey);
                  dwSize=BUFFER_SIZE * sizeof(wchar_t);
                  if (RegEnumKeyExW(hKey, 0, wbuf2, &dwSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                  {
                    RegCloseKey(hKey);
                    if (RegOpenKeyExW(HKEY_CLASSES_ROOT, wszAssocKey, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
                      RegDeleteKeyW(hKey, L"Shell");
                  }
                }
              }
            }
          }
        }
        RegCloseKey(hKey);
      }
    }

    //Check off "Always open with this program"
    if (dwFlags & AE_OPEN)
    {
      wsprintfW(wbuf, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s", wszExt);

      if (RegOpenKeyExW(HKEY_CURRENT_USER, wbuf, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExW(hKey, L"Application", NULL, &dwType, (LPBYTE)wbuf, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf) RegSetValueExW(hKey, L"AkelUndo", 0, dwType, (LPBYTE)wbuf, dwSize);
            RegDeleteValueW(hKey, L"Application");
          }
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExW(hKey, L"AkelUndo", NULL, &dwType, (LPBYTE)wbuf, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf) RegSetValueExW(hKey, L"Application", 0, dwType, (LPBYTE)wbuf, dwSize);
            RegDeleteValueW(hKey, L"AkelUndo");
          }
        }
        RegCloseKey(hKey);
      }
    }
    NextExt:
    wpExtStart=wpExtEnd + 1;
    if (!wpExtEnd) break;
    if (!*wpExtStart) break;
  }
}

BOOL GetFileWriteTimeA(char *pFile, FILETIME *ft)
{
  HANDLE hFile;

  if ((hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    return FALSE;

  GetFileTime(hFile, NULL, NULL, ft);
  CloseHandle(hFile);
  return TRUE;
}

BOOL GetFileWriteTimeW(wchar_t *wpFile, FILETIME *ft)
{
  HANDLE hFile;

  if ((hFile=API_CreateFileW(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    return FALSE;

  GetFileTime(hFile, NULL, NULL, ft);
  CloseHandle(hFile);
  return TRUE;
}

BOOL IsReadOnly()
{
  if (bReadOnly) MessageBeep(MB_OK);
  return bReadOnly;
}

BOOL SaveChangedA()
{
  int nChoice;

  if (bModified)
  {
    API_LoadStringA(hLangLib, MSG_DOCUMENT_CHANGED, buf, BUFFER_SIZE);
    nChoice=MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_YESNOCANCEL|MB_ICONEXCLAMATION);

    if (nChoice == IDYES)
    {
      if (!DoFileSaveA()) return FALSE;
    }
    else if (nChoice == IDCANCEL) return FALSE;
  }
  return TRUE;
}

BOOL SaveChangedW()
{
  int nChoice;

  if (bModified)
  {
    API_LoadStringW(hLangLib, MSG_DOCUMENT_CHANGED, wbuf, BUFFER_SIZE);
    nChoice=MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_YESNOCANCEL|MB_ICONEXCLAMATION);

    if (nChoice == IDYES)
    {
      if (!DoFileSaveW()) return FALSE;
    }
    else if (nChoice == IDCANCEL) return FALSE;
  }
  return TRUE;
}

void ShowURL(HWND hWnd, BOOL bShow)
{
  int i;

  SendMessage(hWnd, EM_AUTOURLDETECT, bShow, 0);
  i=SendMessage(hWnd, EM_GETEVENTMASK, 0, 0);
  SendMessage(hWnd, EM_SETEVENTMASK, 0, (bShow)?(i|ENM_LINK):(i & ~ENM_LINK));
}

BOOL FileExistsA(char *pFile)
{
  WIN32_FIND_DATAA wfdA;
  HANDLE hFind;

  if ((hFind=FindFirstFileA(pFile, &wfdA)) == INVALID_HANDLE_VALUE)
    return FALSE;

  FindClose(hFind);
  return TRUE;
}

BOOL FileExistsW(wchar_t *wpFile)
{
  WIN32_FIND_DATAW wfdW;
  HANDLE hFind;

  if ((hFind=FindFirstFileW(wpFile, &wfdW)) == INVALID_HANDLE_VALUE)
    return FALSE;

  FindClose(hFind);
  return TRUE;
}

int IsFileA(char *pFile)
{
  WIN32_FIND_DATAA wfdA;
  HANDLE hFind;

  if ((hFind=FindFirstFileA(pFile, &wfdA)) == INVALID_HANDLE_VALUE)
    return ERROR_INVALID_HANDLE;

  FindClose(hFind);

  if (wfdA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    return ERROR_FILE_NOT_FOUND;

  return ERROR_SUCCESS;
}

int IsFileW(wchar_t *wpFile)
{
  WIN32_FIND_DATAW wfdW;
  HANDLE hFind;

  if ((hFind=FindFirstFileW(wpFile, &wfdW)) == INVALID_HANDLE_VALUE)
    return ERROR_INVALID_HANDLE;

  FindClose(hFind);

  if (wfdW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    return ERROR_FILE_NOT_FOUND;

  return ERROR_SUCCESS;
}

BOOL IsPathFullA(char *pPath)
{
  if (pPath[0] == '\\' && pPath[1] == '\\') return TRUE;
  if (pPath[0] != '\0' && pPath[1] == ':') return TRUE;
  return FALSE;
}

BOOL IsPathFullW(wchar_t *wpPath)
{
  if (wpPath[0] == '\\' && wpPath[1] == '\\') return TRUE;
  if (wpPath[0] != '\0' && wpPath[1] == ':') return TRUE;
  return FALSE;
}

int GetExeDirA(HINSTANCE hInstance, char *szExeDir, int nLen)
{
  if (nLen=GetModuleFileNameA(hInstance, szExeDir, nLen))
  {
    while (nLen > 0 && szExeDir[nLen] != '\\') --nLen;
    szExeDir[nLen]='\0';
  }
  return nLen;
}

int GetExeDirW(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen)
{
  if (nLen=GetModuleFileNameW(hInstance, wszExeDir, nLen))
  {
    while (nLen > 0 && wszExeDir[nLen] != '\\') --nLen;
    wszExeDir[nLen]='\0';
  }
  return nLen;
}

int GetFileDirA(char *pFile, char *szFileDir, int nFileDirLen)
{
  int i;

  if (!nFileDirLen) return 0;
  szFileDir[0]='\0';

  for (i=lstrlenA(pFile) - 1; i > 0; --i)
  {
    if (pFile[i] == '\\')
    {
      i=min(nFileDirLen, i + 1);
      lstrcpynA(szFileDir, pFile, i);
      return (i - 1);
    }
  }
  return 0;
}

int GetFileDirW(wchar_t *wpFile, wchar_t *wszFileDir, int nFileDirLen)
{
  int i;

  if (!nFileDirLen) return 0;
  wszFileDir[0]='\0';

  for (i=lstrlenW(wpFile) - 1; i > 0; --i)
  {
    if (wpFile[i] == '\\')
    {
      i=min(nFileDirLen, i + 1);
      lstrcpynW(wszFileDir, wpFile, i);
      return (i - 1);
    }
  }
  return 0;
}

BOOL GetFullNameA(char *szFile)
{
  char *pFileName;

  if (GetFullPathNameA(szFile, BUFFER_SIZE, buf, &pFileName))
  {
    if (FileExistsA(szFile))
    {
      if (GetLongPathNameAPtr)
        (*GetLongPathNameAPtr)(buf, szFile, MAX_PATH);
      else
        lstrcpynA(szFile, buf, MAX_PATH);
      return TRUE;
    }
    lstrcpynA(szFile, buf, MAX_PATH);
  }
  return FALSE;
}

BOOL GetFullNameW(wchar_t *wszFile)
{
  wchar_t *wpFileName;

  if (GetFullPathNameW(wszFile, BUFFER_SIZE, wbuf, &wpFileName))
  {
    if (FileExistsW(wszFile))
    {
      if (GetLongPathNameWPtr)
        (*GetLongPathNameWPtr)(wbuf, wszFile, MAX_PATH);
      else
        lstrcpynW(wszFile, wbuf, MAX_PATH);
      return TRUE;
    }
    lstrcpynW(wszFile, wbuf, MAX_PATH);
  }
  return FALSE;
}

char* GetFileNameA(char *pFile)
{
  int i;

  for (i=lstrlenA(pFile) - 1; i >= 0; --i)
  {
    if (pFile[i] == '\\')
      return (pFile + i + 1);
  }
  return pFile;
}

wchar_t* GetFileNameW(wchar_t *wpFile)
{
  int i;

  for (i=lstrlenW(wpFile) - 1; i >= 0; --i)
  {
    if (wpFile[i] == '\\')
      return (wpFile + i + 1);
  }
  return wpFile;
}

int GetBaseNameA(char *pFile, char *szBaseName, int nBaseNameMaxLen)
{
  int nFileLen=lstrlenA(pFile);
  int nEndOffset=-1;
  int i;

  for (i=nFileLen - 1; i >= 0; --i)
  {
    if (pFile[i] == '\\')
      break;

    if (nEndOffset == -1)
    {
      if (pFile[i] == '.')
        nEndOffset=i;
    }
  }
  ++i;
  if (nEndOffset == -1) nEndOffset=nFileLen;
  nBaseNameMaxLen=min(nEndOffset - i + 1, nBaseNameMaxLen);
  lstrcpynA(szBaseName, pFile + i, nBaseNameMaxLen);

  return nBaseNameMaxLen;
}

int GetBaseNameW(wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen)
{
  int nFileLen=lstrlenW(wpFile);
  int nEndOffset=-1;
  int i;

  for (i=nFileLen - 1; i >= 0; --i)
  {
    if (wpFile[i] == '\\')
      break;

    if (nEndOffset == -1)
    {
      if (wpFile[i] == '.')
        nEndOffset=i;
    }
  }
  ++i;
  if (nEndOffset == -1) nEndOffset=nFileLen;
  nBaseNameMaxLen=min(nEndOffset - i + 1, nBaseNameMaxLen);
  lstrcpynW(wszBaseName, wpFile + i, nBaseNameMaxLen);

  return nBaseNameMaxLen;
}

char* GetFileExtA(char *pFile)
{
  int i;

  for (i=lstrlenA(pFile) - 1; i >= 0; --i)
  {
    if (pFile[i] == '.') return (pFile + i + 1);
    else if (pFile[i] == '\\') break;
  }
  return "";
}

wchar_t* GetFileExtW(wchar_t *wpFile)
{
  int i;

  for (i=lstrlenW(wpFile) - 1; i >= 0; --i)
  {
    if (wpFile[i] == '.') return (wpFile + i + 1);
    else if (wpFile[i] == '\\') break;
  }
  return L"";
}

BOOL GetFileVersionA(char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild)
{
  VS_FIXEDFILEINFO ffi;
  VS_FIXEDFILEINFO *pffi=&ffi;
  void *pVerBuf;
  DWORD dwHandle;
  DWORD dwVerSize;
  UINT uLen;
  BOOL bResult=FALSE;

  *nMajor=*nMinor=*nRelease=*nBuild=0;

  if (dwVerSize=GetFileVersionInfoSizeA(pFile, &dwHandle))
  {
    if (pVerBuf=API_HeapAlloc(hHeap, 0, dwVerSize))
    {
      if (GetFileVersionInfoA(pFile, dwHandle, dwVerSize, pVerBuf) && VerQueryValueA(pVerBuf, "\\", (void **)&pffi, &uLen))
      {
        *nMajor=pffi->dwFileVersionMS / 0x00010000;
        *nMinor=pffi->dwFileVersionMS & 0x0000FFFF;
        *nRelease=pffi->dwFileVersionLS / 0x00010000;
        *nBuild=pffi->dwFileVersionLS & 0x0000FFFF;
        bResult=TRUE;
      }
      API_HeapFree(hHeap, 0, pVerBuf);
    }
  }
  return bResult;
}

BOOL GetFileVersionW(wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild)
{
  VS_FIXEDFILEINFO ffi;
  VS_FIXEDFILEINFO *pffi=&ffi;
  void *pVerBuf;
  DWORD dwHandle;
  DWORD dwVerSize;
  UINT uLen;
  BOOL bResult=FALSE;

  *nMajor=*nMinor=*nRelease=*nBuild=0;

  if (dwVerSize=GetFileVersionInfoSizeW(wpFile, &dwHandle))
  {
    if (pVerBuf=API_HeapAlloc(hHeap, 0, dwVerSize))
    {
      if (GetFileVersionInfoW(wpFile, dwHandle, dwVerSize, pVerBuf) && VerQueryValueW(pVerBuf, L"\\", (void **)&pffi, &uLen))
      {
        *nMajor=pffi->dwFileVersionMS / 0x00010000;
        *nMinor=pffi->dwFileVersionMS & 0x0000FFFF;
        *nRelease=pffi->dwFileVersionLS / 0x00010000;
        *nBuild=pffi->dwFileVersionLS & 0x0000FFFF;
        bResult=TRUE;
      }
      API_HeapFree(hHeap, 0, pVerBuf);
    }
  }
  return bResult;
}

int VersionCompare(DWORD dwVersion1, DWORD dwVersion2)
{
  if (LOBYTE(dwVersion1) != LOBYTE(dwVersion2))
    return LOBYTE(dwVersion1) - LOBYTE(dwVersion2);
  if (HIBYTE(dwVersion1) != HIBYTE(dwVersion2))
    return HIBYTE(dwVersion1) - HIBYTE(dwVersion2);
  if (LOBYTE(HIWORD(dwVersion1)) != LOBYTE(HIWORD(dwVersion2)))
    return LOBYTE(HIWORD(dwVersion1)) - LOBYTE(HIWORD(dwVersion2));
  if (HIBYTE(HIWORD(dwVersion1)) != HIBYTE(HIWORD(dwVersion2)))
    return HIBYTE(HIWORD(dwVersion1)) - HIBYTE(HIWORD(dwVersion2));
  return 0;
}

int TranslateFileStringA(char *pString, char *szBuffer, int nBufferSize)
{
  //%f -file, %d -file directory, %a -AkelPad directory, %% -%
  char szFileDir[MAX_PATH];
  char *pFile=szCurrentFile;
  char *pExeDir=szExeDir;
  int nFileLen;
  int nFileDirLen;
  int nExeDirLen;
  int a;
  int b;

  nFileLen=lstrlenA(pFile);
  nFileDirLen=GetFileDirA(pFile, szFileDir, MAX_PATH);
  nExeDirLen=lstrlenA(pExeDir);

  for (a=0, b=0; pString[a] && (!szBuffer || b < nBufferSize); ++a, ++b)
  {
    if (pString[a] == '%')
    {
      if (pString[++a] == '%')
      {
        if (szBuffer) szBuffer[b]='%';
      }
      else if (pString[a] == 'f' || pString[a] == 'F')
      {
        if (!szBuffer || b + nFileLen <= nBufferSize)
        {
          if (szBuffer) lstrcpyA(szBuffer + b, pFile);
          b+=nFileLen - 1;
        }
        else break;
      }
      else if (pString[a] == 'd' || pString[a] == 'D')
      {
        if (!szBuffer || b + nFileDirLen <= nBufferSize)
        {
          if (szBuffer) lstrcpyA(szBuffer + b, szFileDir);
          b+=nFileDirLen - 1;
        }
        else break;
      }
      else if (pString[a] == 'a' || pString[a] == 'A')
      {
        if (!szBuffer || b + nExeDirLen <= nBufferSize)
        {
          if (szBuffer) lstrcpyA(szBuffer + b, pExeDir);
          b+=nExeDirLen - 1;
        }
        else break;
      }
      else break;
    }
    else if (szBuffer) szBuffer[b]=pString[a];
  }
  if (szBuffer && b < nBufferSize) szBuffer[b]='\0';
  return b;
}

int TranslateFileStringW(wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%f -file, %d -file directory, %a -AkelPad directory, %% -%
  wchar_t wszFileDir[MAX_PATH];
  wchar_t *wpFile=wszCurrentFile;
  wchar_t *wpExeDir=wszExeDir;
  int nFileLen;
  int nFileDirLen;
  int nExeDirLen;
  int a;
  int b;

  nFileLen=lstrlenW(wpFile);
  nFileDirLen=GetFileDirW(wpFile, wszFileDir, MAX_PATH);
  nExeDirLen=lstrlenW(wpExeDir);

  for (a=0, b=0; wpString[a] && (!wszBuffer || b < nBufferSize); ++a, ++b)
  {
    if (wpString[a] == '%')
    {
      if (wpString[++a] == '%')
      {
        if (wszBuffer) wszBuffer[b]='%';
      }
      else if (wpString[a] == 'f' || wpString[a] == 'F')
      {
        if (!wszBuffer || b + nFileLen <= nBufferSize)
        {
          if (wszBuffer) lstrcpyW(wszBuffer + b, wpFile);
          b+=nFileLen - 1;
        }
        else break;
      }
      else if (wpString[a] == 'd' || wpString[a] == 'D')
      {
        if (!wszBuffer || b + nFileDirLen <= nBufferSize)
        {
          if (wszBuffer) lstrcpyW(wszBuffer + b, wszFileDir);
          b+=nFileDirLen - 1;
        }
        else break;
      }
      else if (wpString[a] == 'a' || wpString[a] == 'A')
      {
        if (!wszBuffer || b + nExeDirLen <= nBufferSize)
        {
          if (wszBuffer) lstrcpyW(wszBuffer + b, wpExeDir);
          b+=nExeDirLen - 1;
        }
        else break;
      }
      else break;
    }
    else if (wszBuffer) wszBuffer[b]=wpString[a];
  }
  if (wszBuffer && b < nBufferSize) wszBuffer[b]='\0';
  return b;
}

void ActivateWindow(HWND hWnd)
{
  DWORD dwStyle=GetWindowLongA(hWnd, GWL_STYLE);

  if (dwStyle & WS_VISIBLE)
  {
    if (dwStyle & WS_MINIMIZE)
      ShowWindow(hWnd, SW_RESTORE);
    else
      SetForegroundWindow(hWnd); //BringWindowToTop(hWnd);
  }
}

HWND NextDialog(BOOL bPrevious)
{
  HWND hWndNext=NULL;

  if (hDlgModeless)
  {
    if (GetActiveWindow() == hDlgModeless)
      hWndNext=hMainWnd;
    else
      hWndNext=hDlgModeless;

    SetActiveWindow(hWndNext);
  }
  else
  {
    DOCK *dkElement=NULL;
    HWND hWndFocus=GetFocus();

    if (hWndFocus)
    {
      if (hWndFocus != hWndEdit && hWndFocus != hMdiClient)
        dkElement=StackDockFindWindow(&hDocksStack, hWndFocus, TRUE);

      hWndNext=StackDockNextWindow(&hDocksStack, dkElement, bPrevious);
    }
    if (!hWndNext) hWndNext=hMainWnd;
    SetFocus(hWndNext);
  }
  return hWndNext;
}

void UpdateEdit(HWND hWnd)
{
  RECT rcDraw;

  SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)&rcDraw);
  InvalidateRect(hWnd, &rcDraw, FALSE);
}

void DestroyEdit(HWND *hWndEdit)
{
  bEditOnFinish=TRUE;

  if (hWndEdit && *hWndEdit)
  {
    SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)*hWndEdit, 0);
    DestroyWindow(*hWndEdit);

    *hWndEdit=NULL;
  }

  bEditOnFinish=FALSE;
}

void ResizeEdit(HWND hWnd, int X, int Y, int nWidth, int nHeight)
{
  RECT rcEdit;
  int nFirstVisibleChar;

  SaveCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
  MoveWindow(hWnd, X, Y, nWidth, nHeight, TRUE);
  RestoreCharScroll(hWnd, &rcEdit, &nFirstVisibleChar);
}

void UpdateSize()
{
  int i;

  if (!hDocksStack.bSizing)
  {
    hDocksStack.bSizing=TRUE;

    GetClientRect(hMainWnd, &nsSize.rcInitial);
    if (bStatusBar) nsSize.rcInitial.bottom-=nStatusHeight;
    nsSize.rcCurrent=nsSize.rcInitial;
    SendMessage(hMainWnd, AKDN_SIZE, 0, (LPARAM)&nsSize);

    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_LEFT)?DKS_RIGHT:DKS_LEFT, &nsSize);
    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_LEFT)?DKS_LEFT:DKS_RIGHT, &nsSize);
    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_TOP)?DKS_BOTTOM:DKS_TOP, &nsSize);
    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_TOP)?DKS_TOP:DKS_BOTTOM, &nsSize);
    hDocksStack.nSizingSide=0;

    if (!bMDI)
    {
      ResizeEdit(hWndEdit, nsSize.rcCurrent.left, nsSize.rcCurrent.top, nsSize.rcCurrent.right, nsSize.rcCurrent.bottom);
    }
    else
    {
      i=nsSize.rcCurrent.bottom - (nTabView?TAB_HEIGHT:0);

      MoveWindow(hMdiClient, nsSize.rcCurrent.left, nsSize.rcCurrent.top + ((nTabView == TAB_VIEW_TOP)?TAB_HEIGHT:0), nsSize.rcCurrent.right, i, TRUE);

      if (nTabView == TAB_VIEW_TOP)
        MoveWindow(hTab, nsSize.rcCurrent.left, nsSize.rcCurrent.top, nsSize.rcCurrent.right, TAB_HEIGHT, TRUE);
      else if (nTabView == TAB_VIEW_BOTTOM)
        MoveWindow(hTab, nsSize.rcCurrent.left, nsSize.rcCurrent.top + i, nsSize.rcCurrent.right, TAB_HEIGHT, TRUE);
    }

    hDocksStack.bSizing=FALSE;
  }
}

BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcInit, RECT *rcCurrent, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG)
  {
    RECT rcTemplate;

    GetWindowPos(hDlg, NULL, rcInit);
    rcTemplate=*rcCurrent;
    *rcCurrent=*rcInit;

    if (rcTemplate.right && rcTemplate.bottom)
    {
      rcTemplate.left=rcInit->left + (rcInit->right - rcTemplate.right) / 2;
      rcTemplate.top=rcInit->top + (rcInit->bottom - rcTemplate.bottom) / 2;
      SetWindowPos(hDlg, 0, rcTemplate.left, rcTemplate.top, rcTemplate.right, rcTemplate.bottom, SWP_NOZORDER);
    }
  }
  else if (uMsg == WM_GETMINMAXINFO)
  {
    MINMAXINFO *mmi=(MINMAXINFO *)lParam;

    mmi->ptMinTrackSize.x=rcInit->right;
    mmi->ptMinTrackSize.y=rcInit->bottom;
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      RECT rcTemplate;
      RECT rcControl;
      DWORD dwFlags;
      POINT pt;
      int i;

      GetWindowPos(hDlg, NULL, &rcTemplate);
      pt.x=rcTemplate.right - rcCurrent->right;
      pt.y=rcTemplate.bottom - rcCurrent->bottom;
      *rcCurrent=rcTemplate;

      for (i=0; drs[i].lpWnd; ++i)
      {
        if (*drs[i].lpWnd)
        {
          dwFlags=0;
          if (drs[i].dwType & DRS_SIZE)
            dwFlags|=SWP_NOMOVE;
          else if (drs[i].dwType & DRS_MOVE)
            dwFlags|=SWP_NOSIZE;
          GetWindowPos(*drs[i].lpWnd, hDlg, &rcControl);
          SetWindowPos(*drs[i].lpWnd, 0, rcControl.left + ((drs[i].dwType & DRS_X)?pt.x:0), rcControl.top + ((drs[i].dwType & DRS_Y)?pt.y:0), rcControl.right + ((drs[i].dwType & DRS_X)?pt.x:0), rcControl.bottom + ((drs[i].dwType & DRS_Y)?pt.y:0), dwFlags|SWP_NOZORDER);
        }
      }
      InvalidateRect(hDlg, NULL, TRUE);
      return TRUE;
    }
  }
  else if (uMsg == WM_PAINT)
  {
    PAINTSTRUCT ps;
    RECT rcGrip;
    HDC hDC;

    if (hDC=BeginPaint(hDlg, &ps))
    {
      GetClientRect(hDlg, &rcGrip);
      rcGrip.left=rcGrip.right - GetSystemMetrics(SM_CXVSCROLL);
      rcGrip.top=rcGrip.bottom - GetSystemMetrics(SM_CYVSCROLL);
      DrawFrameControl(hDC, &rcGrip, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
      EndPaint(hDlg, &ps);
    }
  }
  return FALSE;
}

void GetMovingRect(DOCK *dkData, POINT *pt, MINMAXINFO *mmi, RECT *rcScreen)
{
  RECT rcInitial=nsSize.rcInitial;
  RECT rcCurrent=nsSize.rcCurrent;
  HWND hWndParent=GetParent(dkData->hWnd);

  GetWindowRect(dkData->hWnd, rcScreen);
  ClientToScreenRect(hWndParent, &rcInitial);
  ClientToScreenRect(hWndParent, &rcCurrent);

  if (dkData->nSide == DKS_LEFT)
  {
    rcScreen->right=max(rcScreen->left + mmi->ptMinTrackSize.x, pt->x);
    rcScreen->right=min(rcScreen->left + mmi->ptMaxTrackSize.x, rcScreen->right);
    rcScreen->right=min(rcInitial.right, rcScreen->right);
    rcScreen->right=min(rcCurrent.right - DOCK_MAINMIN_X, rcScreen->right);
    if (rcScreen->right - rcScreen->left < DOCK_BORDER_2X)
      rcScreen->right=rcScreen->left + DOCK_BORDER_2X;
  }
  else if (dkData->nSide == DKS_TOP)
  {
    rcScreen->bottom=max(rcScreen->top + mmi->ptMinTrackSize.y, pt->y);
    rcScreen->bottom=min(rcScreen->top + mmi->ptMaxTrackSize.y, rcScreen->bottom);
    rcScreen->bottom=min(rcInitial.bottom, rcScreen->bottom);
    rcScreen->bottom=min(rcCurrent.bottom - DOCK_MAINMIN_Y, rcScreen->bottom);
    if (rcScreen->bottom - rcScreen->top < DOCK_BORDER_2X)
      rcScreen->bottom=rcScreen->top + DOCK_BORDER_2X;
  }
  else if (dkData->nSide == DKS_RIGHT)
  {
    rcScreen->left=min(rcScreen->right - mmi->ptMinTrackSize.x, pt->x);
    rcScreen->left=max(rcScreen->right - mmi->ptMaxTrackSize.x, rcScreen->left);
    rcScreen->left=max(rcInitial.left, rcScreen->left);
    rcScreen->left=max(rcCurrent.left + DOCK_MAINMIN_X, rcScreen->left);
    if (rcScreen->right - rcScreen->left < DOCK_BORDER_2X)
      rcScreen->left=rcScreen->right - DOCK_BORDER_2X;
  }
  else if (dkData->nSide == DKS_BOTTOM)
  {
    rcScreen->top=min(rcScreen->bottom - mmi->ptMinTrackSize.y, pt->y);
    rcScreen->top=max(rcScreen->bottom - mmi->ptMaxTrackSize.y, rcScreen->top);
    rcScreen->top=max(rcInitial.top, rcScreen->top);
    rcScreen->top=max(rcCurrent.top + DOCK_MAINMIN_Y, rcScreen->top);
    if (rcScreen->bottom - rcScreen->top < DOCK_BORDER_2X)
      rcScreen->top=rcScreen->bottom - DOCK_BORDER_2X;
  }
}

void DrawMovingRect(RECT *rcScreen)
{
  const WORD DotPattern[]={0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055};
  HDC hDC;
  HBITMAP hBitmap;
  HBRUSH hBrush;
  HBRUSH hBrushOld;

  if (hDC=GetDC(NULL))
  {
    hBitmap=CreateBitmap(8, 8, 1, 1, DotPattern);
    hBrush=CreatePatternBrush(hBitmap);
    hBrushOld=(HBRUSH)SelectObject(hDC, hBrush);

    PatBlt(hDC, rcScreen->left, rcScreen->top, DOCK_BORDER_1X, rcScreen->bottom - rcScreen->top - DOCK_BORDER_1X, PATINVERT);
    PatBlt(hDC, rcScreen->left + DOCK_BORDER_1X, rcScreen->top, rcScreen->right - rcScreen->left - DOCK_BORDER_1X, DOCK_BORDER_1X, PATINVERT);
    PatBlt(hDC, rcScreen->right - DOCK_BORDER_1X, rcScreen->top + DOCK_BORDER_1X, DOCK_BORDER_1X, rcScreen->bottom - rcScreen->top - DOCK_BORDER_1X, PATINVERT);
    PatBlt(hDC, rcScreen->left, rcScreen->bottom - DOCK_BORDER_1X, rcScreen->right - rcScreen->left - DOCK_BORDER_1X,  DOCK_BORDER_1X, PATINVERT);

    SelectObject(hDC, hBrushOld);
    DeleteObject(hBrush);
    DeleteObject(hBitmap);
    ReleaseDC(NULL, hDC);
  }
}

int GetMouseEdge(HWND hWnd, POINT *pt)
{
  RECT rc;

  GetWindowRect(hWnd, &rc);

  if (PtInRect(&rc, *pt))
  {
    if (pt->x >= rc.left && pt->x <= rc.left + DOCK_BORDER_1X) return DKS_LEFT;
    if (pt->y >= rc.top && pt->y <= rc.top + DOCK_BORDER_1X) return DKS_TOP;
    if (pt->x <= rc.right && pt->x >= rc.right - DOCK_BORDER_1X) return DKS_RIGHT;
    if (pt->y <= rc.bottom && pt->y >= rc.bottom - DOCK_BORDER_1X) return DKS_BOTTOM;
  }
  return 0;
}

void SetEdgeCursor(int nEdge)
{
  static HCURSOR hCursorSizeWE=NULL;
  static HCURSOR hCursorSizeNS=NULL;
  static HCURSOR hCursorSizeALL=NULL;

  if (NULL == hCursorSizeWE) {
      hCursorSizeWE=LoadCursor(NULL, IDC_SIZEWE);
      hCursorSizeNS=LoadCursor(NULL, IDC_SIZENS);
      hCursorSizeALL=LoadCursor(NULL, IDC_SIZEALL);
  }

  if (nEdge == DKS_LEFT || nEdge == DKS_RIGHT)
    SetCursor(hCursorSizeWE);
  else if (nEdge == DKS_TOP || nEdge == DKS_BOTTOM)
    SetCursor(hCursorSizeNS);
  else
    SetCursor(hCursorSizeALL);
}

int GetOppEdge(int nEdge)
{
  if (nEdge == DKS_LEFT) return DKS_RIGHT;
  if (nEdge == DKS_TOP) return DKS_BOTTOM;
  if (nEdge == DKS_RIGHT) return DKS_LEFT;
  if (nEdge == DKS_BOTTOM) return DKS_TOP;
  return 0;
}

BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetWindowRect(hWnd, rc))
  {
    rc->right-=rc->left;
    rc->bottom-=rc->top;

    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

BOOL ScreenToClientRect(HWND hWnd, RECT *rc)
{
  if (!ScreenToClient(hWnd, (POINT *)&rc->left))
    return FALSE;
  if (!ScreenToClient(hWnd, (POINT *)&rc->right))
    return FALSE;

  rc->right-=rc->left;
  rc->bottom-=rc->top;
  return TRUE;
}

BOOL ClientToScreenRect(HWND hWnd, RECT *rc)
{
  rc->right+=rc->left;
  rc->bottom+=rc->top;

  if (!ClientToScreen(hWnd, (POINT *)&rc->left))
    return FALSE;
  if (!ClientToScreen(hWnd, (POINT *)&rc->right))
    return FALSE;
  return TRUE;
}

int RectW(const RECT *rc)
{
  return rc->right - rc->left;
}

int RectH(const RECT *rc)
{
  return rc->bottom - rc->top;
}

void UpdateTitleA(HWND hWndEditParent, char *szFile)
{
  char *pFileName;

  //Get file name without path
  pFileName=GetFileNameA(szFile);

  if (!bMDI)
  {
    //Set caption of synchronizing window
    SetWindowTextA(hDummyWindow, szFile);

    //Set caption of main window
    if (*pFileName)
    {
      wsprintfA(buf, "%s - %s", pFileName, APP_MAIN_TITLEA);
      SetWindowTextA(hWndEditParent, buf);
    }
    else SetWindowTextA(hWndEditParent, APP_MAIN_TITLEA);
  }
  else
  {
    WNDFRAMEA *wf;
    TCITEMA tcItemA;
    HICON hIcon;
    int nItem;

    //Set caption of frame window
    SetWindowTextA(hWndEditParent, szFile);

    //Find file icon
    GetAssociatedIconA(pFileName, NULL, NULL, NULL, &hIcon);
    if (!hIcon) hIcon=hIconEmpty;

    if ((nItem=GetTabItemFromParam(hTab, (LPARAM)hWndEditParent)) != -1)
    {
      char szTabName[MAX_PATH];

      //Replace "&" with "&&"
      FixAmpA(pFileName, szTabName, MAX_PATH);

      //Set tab icon
      tcItemA.mask=TCIF_IMAGE;
      SendMessage(hTab, TCM_GETITEMA, nItem, (LPARAM)&tcItemA);
      ImageList_ReplaceIcon(hImageList, tcItemA.iImage, hIcon);

      //Set tab text
      tcItemA.mask=TCIF_TEXT;
      tcItemA.pszText=szTabName;
      SendMessage(hTab, TCM_SETITEMA, nItem, (LPARAM)&tcItemA);
    }

    //Set frame info
    if (wf=(WNDFRAMEA *)GetWindowLongA(hWndEditParent, GWL_USERDATA))
    {
      lstrcpynA(wf->szFile, szFile, MAX_PATH);
      if (wf->hIcon != hIconEmpty) DestroyIcon(wf->hIcon);
      wf->hIcon=hIcon;
    }
    SendMessage(hWndEditParent, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    DrawMenuBar(hMainWnd);
  }
}

void UpdateTitleW(HWND hWndEditParent, wchar_t *wszFile)
{
  wchar_t *wpFileName;

  //Get file name without path
  wpFileName=GetFileNameW(wszFile);

  if (!bMDI)
  {
    //Set caption of synchronizing window
    SetWindowTextW(hDummyWindow, wszFile);

    //Set caption of main window
    if (*wpFileName)
    {
      wsprintfW(wbuf, L"%s - %s", wpFileName, APP_MAIN_TITLEW);
      SetWindowTextW(hWndEditParent, wbuf);
    }
    else SetWindowTextW(hWndEditParent, APP_MAIN_TITLEW);
  }
  else
  {
    WNDFRAMEW *wf;
    TCITEMW tcItemW;
    HICON hIcon;
    int nItem;

    //Set caption of frame window
    SetWindowTextW(hWndEditParent, wszFile);

    //Find file icon
    GetAssociatedIconW(wpFileName, NULL, NULL, NULL, &hIcon);
    if (!hIcon) hIcon=hIconEmpty;

    if ((nItem=GetTabItemFromParam(hTab, (LPARAM)hWndEditParent)) != -1)
    {
      wchar_t wszTabName[MAX_PATH];

      //Replace "&" with "&&"
      FixAmpW(wpFileName, wszTabName, MAX_PATH);

      //Set tab icon
      tcItemW.mask=TCIF_IMAGE;
      SendMessage(hTab, TCM_GETITEMW, nItem, (LPARAM)&tcItemW);
      ImageList_ReplaceIcon(hImageList, tcItemW.iImage, hIcon);

      //Set tab text
      tcItemW.mask=TCIF_TEXT;
      tcItemW.pszText=wszTabName;
      SendMessage(hTab, TCM_SETITEMW, nItem, (LPARAM)&tcItemW);
    }

    //Set frame info
    if (wf=(WNDFRAMEW *)GetWindowLongW(hWndEditParent, GWL_USERDATA))
    {
      lstrcpynW(wf->szFile, wszFile, MAX_PATH);
      if (wf->hIcon != hIconEmpty) DestroyIcon(wf->hIcon);
      wf->hIcon=hIcon;
    }
    SendMessage(hWndEditParent, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    DrawMenuBar(hMainWnd);
  }
}

void UpdateTabs(HWND hWnd)
{
  HWND hTabSpin;
  int nPos;

  if (hTabSpin=GetDlgItem(hWnd, 1))
  {
    if (nPos=LOWORD(SendMessage(hTabSpin, UDM_GETPOS, 0, 0)))
    {
      SendMessage(hWnd, WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, nPos), 0);
      SendMessage(hWnd, WM_HSCROLL, MAKELONG(SB_ENDSCROLL, 0), 0);
    }
  }
}

int GetTabItemFromParam(HWND hWnd, LPARAM lParam)
{
  TCITEMA tcItemA;
  int nCurSel;
  int i;

  tcItemA.mask=TCIF_PARAM;
  nCurSel=SendMessage(hWnd, TCM_GETCURSEL, 0, 0);
  SendMessage(hWnd, TCM_GETITEMA, nCurSel, (LPARAM)&tcItemA);
  if (tcItemA.lParam == lParam) return nCurSel;

  for (i=0; SendMessage(hWnd, TCM_GETITEMA, i, (LPARAM)&tcItemA); ++i)
  {
    if (i != nCurSel)
    {
      if (tcItemA.lParam == lParam)
        return i;
    }
  }
  return -1;
}

int GetTabItemFromPoint(HWND hWnd, POINT *pt)
{
  TCHITTESTINFO thti;
  RECT rcTab;
  BOOL bMinus=TRUE;
  BOOL bBetween=FALSE;
  int nTabClientX;
  int nCurSel;
  int i=-1;

  GetWindowRect(hWnd, &rcTab);

  if (pt->x > rcTab.left && pt->y > rcTab.top &&
      pt->x < rcTab.right && pt->y < rcTab.bottom)
  {
    nTabClientX=pt->x - rcTab.left;
    rcTab.right-=rcTab.left;
    rcTab.bottom-=rcTab.top;
    thti.pt.x=nTabClientX;
    thti.pt.y=rcTab.bottom / 2;

    if (SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti) == -1)
      bBetween=TRUE;

    while (1)
    {
      if ((i=SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti)) != -1) break;

      if (bMinus)
      {
        if (--thti.pt.x < 0)
        {
          thti.pt.x=nTabClientX + 1;
          bMinus=FALSE;
        }
      }
      else if (++thti.pt.x > rcTab.right) break;
    }

    nCurSel=SendMessage(hWnd, TCM_GETCURSEL, 0, 0);
    if (i < nCurSel && bBetween && bMinus) ++i;
  }
  return i;
}

int SelectTabItem(HWND hWnd, int nIndex)
{
  NMHDR nmhdr={hWnd, ID_TAB, TCN_SELCHANGE};
  int nCurSel;

  nCurSel=SendMessage(hWnd, TCM_GETCURSEL, 0, 0);

  if (nIndex != nCurSel)
  {
    if (SendMessage(hWnd, TCM_SETCURSEL, nIndex, 0) != -1)
    {
      SendMessage(GetParent(hWnd), WM_NOTIFY, ID_TAB, (LPARAM)&nmhdr);
    }
    else return -1;
  }
  return nCurSel;
}

int MoveTabItemA(HWND hWnd, int nIndexOld, int nIndexNew)
{
  TCITEMA tcItemA;
  char szItemText[MAX_PATH];

  tcItemA.mask=TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM;
  tcItemA.pszText=szItemText;
  tcItemA.cchTextMax=MAX_PATH;

  if (SendMessage(hWnd, TCM_GETITEMA, nIndexOld, (LPARAM)&tcItemA))
  {
    if (nIndexOld == nIndexNew) return nIndexNew;
    SendMessage(hWnd, TCM_DELETEITEM, nIndexOld, 0);
    return SendMessage(hWnd, TCM_INSERTITEMA, nIndexNew, (LPARAM)&tcItemA);
  }
  return -1;
}

int MoveTabItemW(HWND hWnd, int nIndexOld, int nIndexNew)
{
  TCITEMW tcItemW;
  wchar_t wszItemText[MAX_PATH];

  tcItemW.mask=TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM;
  tcItemW.pszText=wszItemText;
  tcItemW.cchTextMax=MAX_PATH;

  if (SendMessage(hWnd, TCM_GETITEMW, nIndexOld, (LPARAM)&tcItemW))
  {
    if (nIndexOld == nIndexNew) return nIndexNew;
    SendMessage(hWnd, TCM_DELETEITEM, nIndexOld, 0);
    return SendMessage(hWnd, TCM_INSERTITEMW, nIndexNew, (LPARAM)&tcItemW);
  }
  return -1;
}

BOOL DeleteTabItem(HWND hWnd, int nIndex)
{
  TCITEMA tcItemA;

  tcItemA.mask=TCIF_IMAGE;
  if (SendMessage(hWnd, TCM_GETITEMA, nIndex, (LPARAM)&tcItemA))
  {
    SendMessage(hTab, TCM_REMOVEIMAGE, tcItemA.iImage, 0);
    SendMessage(hWnd, TCM_DELETEITEM, nIndex, 0);
    return TRUE;
  }
  return FALSE;
}

void FreeMemorySearchA()
{
  if (szFind_orig)
  {
    if (szFind_orig == szFind)
      szFind=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)szFind_orig);
    szFind_orig=NULL;
  }
  if (szReplace_orig)
  {
    if (szReplace_orig == szReplace)
      szReplace=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)szReplace_orig);
    szReplace_orig=NULL;
  }

  if (szFind)
  {
    API_HeapFree(hHeap, 0, (LPVOID)szFind);
    szFind=NULL;
  }
  if (szReplace)
  {
    API_HeapFree(hHeap, 0, (LPVOID)szReplace);
    szReplace=NULL;
  }
}

void FreeMemorySearchW()
{
  if (wszFind_orig)
  {
    if (wszFind_orig == wszFind)
      wszFind=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)wszFind_orig);
    wszFind_orig=NULL;
  }
  if (wszReplace_orig)
  {
    if (wszReplace_orig == wszReplace)
      wszReplace=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)wszReplace_orig);
    wszReplace_orig=NULL;
  }

  if (wszFind)
  {
    API_HeapFree(hHeap, 0, (LPVOID)wszFind);
    wszFind=NULL;
  }
  if (wszReplace)
  {
    API_HeapFree(hHeap, 0, (LPVOID)wszReplace);
    wszReplace=NULL;
  }
}

void FreeMemoryRecentFilesA()
{
  if (lpszRecentNames)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpszRecentNames);
    lpszRecentNames=NULL;
  }
  if (lpdwRecentPositions)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpdwRecentPositions);
    lpdwRecentPositions=NULL;
  }
  if (lpdwRecentCodepages)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpdwRecentCodepages);
    lpdwRecentCodepages=NULL;
  }
}

void FreeMemoryRecentFilesW()
{
  if (lpwszRecentNames)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpwszRecentNames);
    lpwszRecentNames=NULL;
  }
  if (lpdwRecentPositions)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpdwRecentPositions);
    lpdwRecentPositions=NULL;
  }
  if (lpdwRecentCodepages)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpdwRecentCodepages);
    lpdwRecentCodepages=NULL;
  }
}


//// OLE Drag'n'Drop

ULONG WINAPI REOLE_AddRef(IRichEditOleCallback *lpTable)
{
  return ++(((REOLE *)lpTable)->uRefCount);
}

ULONG WINAPI REOLE_Release(IRichEditOleCallback * lpTable)
{
  return --(((REOLE *)lpTable)->uRefCount);
}

HRESULT WINAPI REOLE_QueryInterface(IRichEditOleCallback * lpTable, REFIID riid, void **ppvObj)
{
  if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IRichEditOleCallback))
  {
    *ppvObj=lpTable;
    REOLE_AddRef((IRichEditOleCallback *)*ppvObj);
    return S_OK;
  }
  else
  {
    *ppvObj=NULL;
    return E_NOINTERFACE;
  }
}

HRESULT WINAPI REOLE_GetNewStorage(IRichEditOleCallback * lpTable, LPSTORAGE FAR *lplpstg)
{
  return E_NOTIMPL;
}

HRESULT WINAPI REOLE_GetInPlaceContext(IRichEditOleCallback * lpTable, LPOLEINPLACEFRAME FAR *lplpFrame, LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
  return E_NOTIMPL;
}

HRESULT WINAPI REOLE_ShowContainerUI(IRichEditOleCallback * lpTable, BOOL fShow)
{
  return E_NOTIMPL;
}

HRESULT WINAPI REOLE_QueryInsertObject(IRichEditOleCallback * lpTable, LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp)
{
  return E_NOTIMPL;
}

HRESULT WINAPI REOLE_DeleteObject(IRichEditOleCallback * lpTable, LPOLEOBJECT lpoleobj)
{
  return E_NOTIMPL;
}

HRESULT WINAPI REOLE_QueryAcceptData(IRichEditOleCallback * lpTable, LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat, DWORD reco, BOOL fReally, HGLOBAL hMetaPict)
{
  *lpcfFormat=CF_TEXT;

  if (fReally)
  {
    FORMATETC fmtetc;
    STGMEDIUM stgmed;
    LPVOID pData;

    fmtetc.cfFormat=CF_UNICODETEXT;
    fmtetc.ptd=0;
    fmtetc.dwAspect=DVASPECT_CONTENT;
    fmtetc.lindex=-1;
    fmtetc.tymed=TYMED_HGLOBAL;

    if (lpdataobj->lpVtbl->QueryGetData(lpdataobj, &fmtetc) == S_OK)
    {
      if (lpdataobj->lpVtbl->GetData(lpdataobj, &fmtetc, &stgmed) == S_OK)
      {
        if (pData=GlobalLock(stgmed.hGlobal))
        {
          ReplaceSelW(hWndEdit, (wchar_t *)pData, -1);
          GlobalUnlock(stgmed.hGlobal);
        }
        ReleaseStgMedium(&stgmed);
      }
    }
    else
    {
      fmtetc.cfFormat=CF_TEXT;

      if (lpdataobj->lpVtbl->QueryGetData(lpdataobj, &fmtetc) == S_OK)
      {
        if (lpdataobj->lpVtbl->GetData(lpdataobj, &fmtetc, &stgmed) == S_OK)
        {
          if (pData=GlobalLock(stgmed.hGlobal))
          {
            ReplaceSelA(hWndEdit, (char *)pData, -1);
            GlobalUnlock(stgmed.hGlobal);
          }
          ReleaseStgMedium(&stgmed);
        }
      }
    }
    return S_FALSE;
  }
  return S_OK;
}

HRESULT WINAPI REOLE_ContextSensitiveHelp(IRichEditOleCallback * lpTable, BOOL fEnterMode)
{
  return E_NOTIMPL;
}

HRESULT WINAPI REOLE_GetClipboardData(IRichEditOleCallback * lpTable, CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj)
{
  return E_NOTIMPL;
}

HRESULT WINAPI REOLE_GetDragDropEffect(IRichEditOleCallback * lpTable, BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
  if (!fDrag)
  {
    HWND hWndFromPoint;
    HWND hWndOldFocus;
    POINT pt;

    GetCursorPos(&pt);
    if (hWndFromPoint=RealChildWindowFromPoint95(hMdiClient, pt))
    {
      hWndOldFocus=GetFocus();
      if (hWndFromPoint != hWndOldFocus) SetFocus(hWndFromPoint);
    }
  }
  return S_OK;
}

HRESULT WINAPI REOLE_GetContextMenu(IRichEditOleCallback * lpTable, WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg, HMENU FAR *lphmenu)
{
  return E_NOTIMPL;
}

HWND RealChildWindowFromPoint95(HWND hWndParent, POINT pt)
{
  //From MiniSpy by Daniel Godson
  HWND hWndChild=GetWindow(hWndParent, GW_CHILD);

  while (hWndChild)
  {
    if (SendMessage(hWndChild, WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y)) > 0)
      return hWndChild;

    hWndChild=GetNextWindow(hWndChild, GW_HWNDNEXT);
  }
  return NULL;
}


//// API functions replacement

HMENU API_LoadMenuA(HINSTANCE hLoadInstance, char *lpMenuName)
{
  HMENU hResult;

  if (!(hResult=LoadMenuA(hLoadInstance, lpMenuName)))
    if (hLoadInstance != hInstance)
      hResult=LoadMenuA(hInstance, lpMenuName);

  return hResult;
}

HMENU API_LoadMenuW(HINSTANCE hLoadInstance, wchar_t *lpMenuName)
{
  HMENU hResult;

  if (!(hResult=LoadMenuW(hLoadInstance, lpMenuName)))
    if (hLoadInstance != hInstance)
      hResult=LoadMenuW(hInstance, lpMenuName);

  return hResult;
}

HACCEL API_LoadAcceleratorsA(HINSTANCE hLoadInstance, char *lpTableName)
{
  HACCEL hResult;

  if (!(hResult=LoadAcceleratorsA(hLoadInstance, lpTableName)))
    if (hLoadInstance != hInstance)
      hResult=LoadAcceleratorsA(hInstance, lpTableName);

  return hResult;
}

HACCEL API_LoadAcceleratorsW(HINSTANCE hLoadInstance, wchar_t *lpTableName)
{
  HACCEL hResult;

  if (!(hResult=LoadAcceleratorsW(hLoadInstance, lpTableName)))
    if (hLoadInstance != hInstance)
      hResult=LoadAcceleratorsW(hInstance, lpTableName);

  return hResult;
}

HICON API_LoadIconA(HINSTANCE hLoadInstance, char *lpIconName)
{
  HICON hResult;

  if (!(hResult=LoadIconA(hLoadInstance, lpIconName)))
    if (hLoadInstance != hInstance)
      hResult=LoadIconA(hInstance, lpIconName);

  return hResult;
}

HICON API_LoadIconW(HINSTANCE hLoadInstance, wchar_t *lpIconName)
{
  HICON hResult;

  if (!(hResult=LoadIconW(hLoadInstance, lpIconName)))
    if (hLoadInstance != hInstance)
      hResult=LoadIconW(hInstance, lpIconName);

  return hResult;
}

HANDLE API_LoadImageA(HINSTANCE hLoadInstance, char *lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
{
  HANDLE hResult;

  if (!(hResult=LoadImageA(hLoadInstance, lpszName, uType, cxDesired, cyDesired, fuLoad)))
    if (hLoadInstance != hInstance)
      hResult=LoadImageA(hInstance, lpszName, uType, cxDesired, cyDesired, fuLoad);

  return hResult;
}

HANDLE API_LoadImageW(HINSTANCE hLoadInstance, wchar_t *lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad)
{
  HANDLE hResult;

  if (!(hResult=LoadImageW(hLoadInstance, lpszName, uType, cxDesired, cyDesired, fuLoad)))
    if (hLoadInstance != hInstance)
      hResult=LoadImageW(hInstance, lpszName, uType, cxDesired, cyDesired, fuLoad);

  return hResult;
}

int API_LoadStringA(HINSTANCE hLoadInstance, UINT uID, char *lpBuffer, int nBufferMax)
{
  int nResult;

  if (!(nResult=LoadStringA(hLoadInstance, uID, lpBuffer, nBufferMax)))
    if (hLoadInstance != hInstance)
      if (!(nResult=LoadStringA(hInstance, uID, lpBuffer, nBufferMax)))
        lpBuffer[0]='\0';

  return nResult;
}

int API_LoadStringW(HINSTANCE hLoadInstance, UINT uID, wchar_t *lpBuffer, int nBufferMax)
{
  int nResult;

  if (!(nResult=LoadStringW(hLoadInstance, uID, lpBuffer, nBufferMax)))
    if (hLoadInstance != hInstance)
      if (!(nResult=LoadStringW(hInstance, uID, lpBuffer, nBufferMax)))
        lpBuffer[0]='\0';

  return nResult;
}

HWND API_CreateDialogA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc)
{
  HWND hResult;

  if (!(hResult=CreateDialogA(hLoadInstance, lpTemplateName, hWndParent, lpDialogFunc)))
    if (hLoadInstance != hInstance)
      hResult=CreateDialogA(hInstance, lpTemplateName, hWndParent, lpDialogFunc);

  return hResult;
}

HWND API_CreateDialogW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc)
{
  HWND hResult;

  if (!(hResult=CreateDialogW(hLoadInstance, lpTemplateName, hWndParent, lpDialogFunc)))
    if (hLoadInstance != hInstance)
      hResult=CreateDialogW(hInstance, lpTemplateName, hWndParent, lpDialogFunc);

  return hResult;
}

INT_PTR API_DialogBoxA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc)
{
  INT_PTR nResult;

  if ((nResult=DialogBoxA(hLoadInstance, lpTemplateName, hWndParent, lpDialogFunc)) == -1)
    if (hLoadInstance != hInstance)
      nResult=DialogBoxA(hInstance, lpTemplateName, hWndParent, lpDialogFunc);

  return nResult;
}

INT_PTR API_DialogBoxW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc)
{
  INT_PTR nResult;

  if ((nResult=DialogBoxW(hLoadInstance, lpTemplateName, hWndParent, lpDialogFunc)) == -1)
    if (hLoadInstance != hInstance)
      nResult=DialogBoxW(hInstance, lpTemplateName, hWndParent, lpDialogFunc);

  return nResult;
}

INT_PTR API_DialogBoxParamA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
  INT_PTR nResult;

  if ((nResult=DialogBoxParamA(hLoadInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam)) == -1)
    if (hLoadInstance != hInstance)
      nResult=DialogBoxParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);

  return nResult;
}

INT_PTR API_DialogBoxParamW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
  INT_PTR nResult;

  if ((nResult=DialogBoxParamW(hLoadInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam)) == -1)
    if (hLoadInstance != hInstance)
      nResult=DialogBoxParamW(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);

  return nResult;
}

HANDLE API_CreateFileA(char *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  HANDLE hResult;

  if ((hResult=CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)) == INVALID_HANDLE_VALUE)
  {
    API_LoadStringA(hLangLib, MSG_CANNOT_OPEN_FILE, buf, BUFFER_SIZE);
    wsprintfA(buf2, buf, lpFileName);
    MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
  }

  return hResult;
}

HANDLE API_CreateFileW(wchar_t *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  HANDLE hResult;

  if ((hResult=CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)) == INVALID_HANDLE_VALUE)
  {
    API_LoadStringW(hLangLib, MSG_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
    wsprintfW(wbuf2, wbuf, lpFileName);
    MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }

  return hResult;
}

BOOL API_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
  BOOL bResult;

  if (!(bResult=ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped)))
  {
    if (bOldWindows)
    {
      API_LoadStringA(hLangLib, MSG_ERROR_IO, buf, BUFFER_SIZE);
      MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
    }
    else
    {
      API_LoadStringW(hLangLib, MSG_ERROR_IO, wbuf, BUFFER_SIZE);
      MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    }
  }

  return bResult;
}

BOOL API_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
  BOOL bResult;

  if (!(bResult=WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped)))
  {
    if (bOldWindows)
    {
      API_LoadStringA(hLangLib, MSG_ERROR_IO, buf, BUFFER_SIZE);
      MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
    }
    else
    {
      API_LoadStringW(hLangLib, MSG_ERROR_IO, wbuf, BUFFER_SIZE);
      MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    }
  }

  return bResult;
}

LPVOID API_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
  LPVOID lpResult;

  if (!(lpResult=HeapAlloc(hHeap, dwFlags, dwBytes)))
  {
    if (bOldWindows)
    {
      API_LoadStringA(hLangLib, MSG_ERROR_NOT_ENOUGH_MEMORY, buf, BUFFER_SIZE);
      MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
    }
    else
    {
      API_LoadStringW(hLangLib, MSG_ERROR_NOT_ENOUGH_MEMORY, wbuf, BUFFER_SIZE);
      MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    }
  }

  return lpResult;
}

BOOL API_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
  BOOL bResult;

  bResult=HeapFree(hHeap, dwFlags, lpMem);

  return bResult;
}
