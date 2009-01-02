#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0400
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <richedit.h>
#include "ConvFunc.h"
#include "StackFunc.h"
#include "StrFunc.h"
#include "AkelEdit\AkelBuild.h"
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
extern BOOL bOldWindows;
extern BOOL bOldRichEdit;
extern BOOL bOldComctl32;

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
extern HHOOK hHookPlugins;
extern HWND hWndHotkey;
extern BOOL bSavePluginsStackOnExit;

//INI
extern HSTACK hIniStack;
extern char szIniFile[MAX_PATH];
extern wchar_t wszIniFile[MAX_PATH];
extern int nSaveSettings;

//Main Window
extern HWND hMainWnd;
extern HWND hWndEdit;
extern HWND hDummyWindow;
extern HWND hStatus;
extern HWND hProgress;
extern HWND hDlgModeless;
extern RECT rcMainWindowRestored;
extern DWORD dwMainStyle;
extern DWORD dwLastMainSize;
extern int nStatusHeight;
extern int nProgressWidth;
extern BOOL bStatusSelUpdate;
extern HACCEL hGlobalAccel;
extern HACCEL hMainAccel;
extern HICON hMainIcon;
extern HICON hIconEmpty;
extern HCURSOR hCursorDragMove;
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

//Docks
extern HDOCK hDocksStack;
extern NSIZE nsSize;
extern WNDPROC OldDockProc;

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
extern RECT rcGoToLineDlg;

//Options dialog
extern PROPSHEETHEADERA pshA;
extern PROPSHEETHEADERW pshW;
extern PROPSHEETPAGEA pspA[2];
extern PROPSHEETPAGEW pspW[2];
extern HHOOK hHookOptions;
extern BOOL bOptionsRestart;

//Font/Color
extern LOGFONTA lfEditFontA;
extern LOGFONTW lfEditFontW;
extern CHOOSEFONTA cfA;
extern CHOOSEFONTW cfW;
extern CHOOSECOLORA ccA;
extern CHOOSECOLORW ccW;
extern HSTACK hThemesStack;
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

//Edit state
extern AECHARRANGE crSel;
extern AECHARINDEX ciCaret;
extern BOOL bModified;
extern BOOL bInsertState;
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
extern DWORD dwEditMargins;
extern BOOL bShowURL;
extern int nClickURL;
extern wchar_t wszUrlPrefixes[URL_PREFIXES_SIZE];
extern BOOL bUrlPrefixesEnable;
extern wchar_t wszUrlDelimiters[URL_DELIMITERS_SIZE];
extern BOOL bUrlDelimitersEnable;
extern BOOL bCaretOutEdge;
extern FILETIME ftFileTime;
extern WNDPROC OldEditProc;

//Word breaking
extern wchar_t wszWordDelimiters[WORD_DELIMITERS_SIZE];
extern BOOL bWordDelimitersEnable;
extern DWORD dwCustomWordBreak;
extern DWORD dwDefaultWordBreak;

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

//Mdi
extern BOOL bMDI;
extern BOOL bRegMDI;
extern HWND hMdiClient;
extern HWND hWndFrameActive;
extern HWND hWndFrameDestroyed;
extern BOOL bMdiMaximize;
extern BOOL bMdiNoWindows;
extern BOOL bMdiReopen;
extern BOOL bMdiClientRedraw;
extern HWND hTab;
extern int nTabView;
extern int nTabType;
extern int nTabSwitch;
extern HIMAGELIST hImageList;
extern BOOL bTabPressed;
extern BOOL bFileExitError;
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

  GetClientRect(hWndParent, &rcRect);

  hWndEditNew=CreateWindowExA(WS_EX_CLIENTEDGE,
                              "AkelEditA",
                              NULL,
                              WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_DISABLENOSCROLL|ES_NOHIDESEL,
                              0, 0, rcRect.right, rcRect.bottom - (bStatusBar?nStatusHeight:0),
                              hWndParent,
                              (HMENU)ID_EDIT,
                              hInstance,
                              NULL);

  if (!hWndEdit) hWndEdit=hWndEditNew;
  DoViewWordWrap(hWndEditNew, bWordWrap, TRUE);
  DoSettingsReadOnly(hWndEditNew, bReadOnly, TRUE);
  SendMessage(hWndEditNew, AEM_SETEVENTMASK, 0, AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY|AENM_LINK|AENM_PROGRESS|AENM_SCROLL);
  SendMessage(hWndEditNew, EM_SETEVENTMASK, 0, ENM_SELCHANGE|ENM_CHANGE|ENM_LINK);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bDetailedUndo?AECOOP_OR:AECOOP_XOR, AECO_DETAILEDUNDO);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);
  SendMessage(hWndEditNew, AEM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
  SendMessage(hWndEditNew, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
  SendMessage(hWndEditNew, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
  SetTabStops(hWndEditNew, nTabStopSize, FALSE);
  SetChosenFontA(hWndEditNew, &lfEditFontA, TRUE);
  SetNewLineStatusA(hWndEditNew, NEWLINE_WIN, AENL_INPUT, TRUE);

  if (bShowURL)
  {
    SendMessage(hWndEditNew, AEM_SETDETECTURL, bShowURL, 0);

    if (bUrlPrefixesEnable)
      SetUrlPrefixes(hWndEditNew, wszUrlPrefixes);
    if (bUrlDelimitersEnable)
      SendMessage(hWndEditNew, AEM_SETURLDELIMITERS, 0, (LPARAM)wszUrlDelimiters);
  }

  dwDefaultWordBreak=SendMessage(hWndEditNew, AEM_GETWORDBREAK, 0, 0);

  if (bWordDelimitersEnable)
  {
    SendMessage(hWndEditNew, AEM_SETWORDBREAK, dwCustomWordBreak, 0);
    SendMessage(hWndEditNew, AEM_SETWORDDELIMITERS, 0, (LPARAM)wszWordDelimiters);
  }

  OldEditProc=(WNDPROC)GetWindowLongA(hWndEditNew, GWL_WNDPROC);
  SetWindowLongA(hWndEditNew, GWL_WNDPROC, (LONG)CommonEditProcA);
  SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)hWndEditNew, 0);

  return hWndEditNew;
}

HWND CreateEditWindowW(HWND hWndParent)
{
  RECT rcRect;
  HWND hWndEditNew;

  GetClientRect(hWndParent, &rcRect);

  hWndEditNew=CreateWindowExW(WS_EX_CLIENTEDGE,
                              L"AkelEditW",
                              NULL,
                              WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_DISABLENOSCROLL|ES_NOHIDESEL,
                              0, 0, rcRect.right, rcRect.bottom - (bStatusBar?nStatusHeight:0),
                              hWndParent,
                              (HMENU)ID_EDIT,
                              hInstance,
                              NULL);

  if (!hWndEdit) hWndEdit=hWndEditNew;
  DoViewWordWrap(hWndEditNew, bWordWrap, TRUE);
  DoSettingsReadOnly(hWndEditNew, bReadOnly, TRUE);
  SendMessage(hWndEditNew, AEM_SETEVENTMASK, 0, AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY|AENM_LINK|AENM_PROGRESS|AENM_SCROLL);
  SendMessage(hWndEditNew, EM_SETEVENTMASK, 0, ENM_SELCHANGE|ENM_CHANGE|ENM_LINK);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bDetailedUndo?AECOOP_OR:AECOOP_XOR, AECO_DETAILEDUNDO);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);
  SendMessage(hWndEditNew, AEM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
  SendMessage(hWndEditNew, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
  SendMessage(hWndEditNew, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
  SetTabStops(hWndEditNew, nTabStopSize, FALSE);
  SetChosenFontW(hWndEditNew, &lfEditFontW, TRUE);
  SetNewLineStatusW(hWndEditNew, NEWLINE_WIN, AENL_INPUT, TRUE);

  if (bShowURL)
  {
    SendMessage(hWndEditNew, AEM_SETDETECTURL, bShowURL, 0);

    if (bUrlPrefixesEnable)
      SetUrlPrefixes(hWndEditNew, wszUrlPrefixes);
    if (bUrlDelimitersEnable)
      SendMessage(hWndEditNew, AEM_SETURLDELIMITERS, 0, (LPARAM)wszUrlDelimiters);
  }

  dwDefaultWordBreak=SendMessage(hWndEditNew, AEM_GETWORDBREAK, 0, 0);

  if (bWordDelimitersEnable)
  {
    SendMessage(hWndEditNew, AEM_SETWORDBREAK, dwCustomWordBreak, 0);
    SendMessage(hWndEditNew, AEM_SETWORDDELIMITERS, 0, (LPARAM)wszWordDelimiters);
  }

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
    RecentFilesUpdateA(szCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
    RecentFilesSaveA();
  }

  HideCaret(NULL);
  SetWindowTextA(hWndEdit, "");
  ShowCaret(NULL);
  szCurrentFile[0]='\0';
  SetNewLineStatusA(hWndEdit, NEWLINE_WIN, AENL_INPUT, TRUE);
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
    RecentFilesUpdateW(wszCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
    RecentFilesSaveW();
  }

  HideCaret(NULL);
  SetWindowTextW(hWndEdit, L"");
  ShowCaret(NULL);
  wszCurrentFile[0]='\0';
  SetNewLineStatusW(hWndEdit, NEWLINE_WIN, AENL_INPUT, TRUE);
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
    bMdiReopen=TRUE;
    OpenDocumentA(hWndEdit, szCurrentFile, dwFlags, nCodePage, bBOM);
    bMdiReopen=FALSE;
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
    bMdiReopen=TRUE;
    OpenDocumentW(hWndEdit, wszCurrentFile, dwFlags, nCodePage, bBOM);
    bMdiReopen=FALSE;
  }
}

BOOL DoFileSaveA()
{
  if (!bModified && szCurrentFile[0] && FileExistsA(szCurrentFile)) return TRUE;
  if (!szCurrentFile[0]) return DoFileSaveAsA();
  return !SaveDocumentA(hWndEdit, szCurrentFile, nCurrentCodePage, bCurrentBOM, TRUE);
}

BOOL DoFileSaveW()
{
  if (!bModified && wszCurrentFile[0] && FileExistsW(wszCurrentFile)) return TRUE;
  if (!wszCurrentFile[0]) return DoFileSaveAsW();
  return !SaveDocumentW(hWndEdit, wszCurrentFile, nCurrentCodePage, bCurrentBOM, TRUE);
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

    if (!SaveDocumentA(hWndEdit, szFileBuffer, nOfnCodePage, bOfnBOM, TRUE))
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

    if (!SaveDocumentW(hWndEdit, wszFileBuffer, nOfnCodePage, bOfnBOM, TRUE))
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

BOOL DoFilePrintA(BOOL bSilent)
{
  DOCINFOA diA={0};
  HDC hEditDC;
  TEXTMETRICA tmA;
  DRAWTEXTPARAMS dtp;
  AECHARRANGE cr;
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
      cr=crSel;
    }
    else
    {
      SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&cr.ciMin);
      SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&cr.ciMax);
    }

    if (ExGetRangeTextA(hWndEdit, &cr.ciMin, &cr.ciMax, -1, &szBuffer, AELB_RN))
    {
      pText=szBuffer;

      if (bPrintFontEnable)
      {
        hEditDC=GetDC(hWndEdit);
        nPointSize=-MulDiv(lfPrintFontA.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWndEdit, hEditDC);
        memcpy(&lfA, &lfPrintFontA, sizeof(LOGFONTA));
      }
      else
      {
        hEditDC=GetDC(hWndEdit);
        nPointSize=-MulDiv(lfEditFontA.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWndEdit, hEditDC);
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

BOOL DoFilePrintW(BOOL bSilent)
{
  DOCINFOW diW={0};
  HDC hEditDC;
  TEXTMETRICW tmW;
  DRAWTEXTPARAMS dtp;
  AECHARRANGE cr;
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
      cr=crSel;
    }
    else
    {
      SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&cr.ciMin);
      SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&cr.ciMax);
    }

    if (ExGetRangeTextW(hWndEdit, &cr.ciMin, &cr.ciMax, -1, &wszBuffer, AELB_RN))
    {
      wpText=wszBuffer;

      if (bPrintFontEnable)
      {
        hEditDC=GetDC(hWndEdit);
        nPointSize=-MulDiv(lfPrintFontW.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWndEdit, hEditDC);
        memcpy(&lfW, &lfPrintFontW, sizeof(LOGFONTW));
      }
      else
      {
        hEditDC=GetDC(hWndEdit);
        nPointSize=-MulDiv(lfEditFontW.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
        ReleaseDC(hWndEdit, hEditDC);
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
    RecentFilesUpdateA(szCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
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
    RecentFilesUpdateW(wszCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
    RecentFilesSaveW();
  }
  return TRUE;
}

void DoEditUndo(HWND hWnd)
{
  SendMessage(hWnd, AEM_UNDO, 0, 0);
}

void DoEditRedo(HWND hWnd)
{
  SendMessage(hWnd, AEM_REDO, 0, 0);
}

void DoEditCut(HWND hWnd)
{
  if (IsReadOnly()) return;

  SendMessage(hWnd, AEM_CUT, 0, 0);
}

void DoEditCopy(HWND hWnd)
{
  SendMessage(hWnd, AEM_COPY, 0, 0);
}

void DoEditPaste(HWND hWnd, BOOL bAnsi)
{
  if (IsReadOnly()) return;

  SendMessage(hWnd, AEM_PASTE, 0, bAnsi);
}

void DoEditClear(HWND hWnd)
{
  if (IsReadOnly()) return;

  ReplaceSelW(hWnd, L"", -1, -1, NULL, NULL);
}

void DoEditSelectAll(HWND hWnd)
{
  AECHARRANGE cr;

  SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&cr.ciMin);
  SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&cr.ciMax);
  SetSel(hWnd, &cr, FALSE, &cr.ciMax);
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
    ReplaceSelA(hWnd, szTimeAndDate, -1, -1, NULL, NULL);
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
    ReplaceSelW(hWnd, wszTimeAndDate, -1, -1, NULL, NULL);
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
  AECHARRANGE crInitialSel;
  AECHARRANGE crRange=crSel;
  AECHARINDEX ciInitialCaret;
  AETEXTRANGEW tr;
  wchar_t *wszRange=NULL;
  int nRangeLen;
  int nStringLenAll;
  int nStringLen;
  int nStringBytes;
  int nBufferLen;
  int nFirstLine;
  int a=0;
  int b=0;
  int i;
  BOOL bResult=FALSE;

  if (!(nAction & STRSEL_CHECK) && IsReadOnly()) return FALSE;

  if (crRange.ciMin.nLine != crRange.ciMax.nLine)
  {
    if (nAction & STRSEL_CHECK) return TRUE;

    SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEBEGIN, (LPARAM)&crRange.ciMin);
    if (crRange.ciMax.nCharInLine || (crRange.ciMax.lpLine->prev && crRange.ciMax.lpLine->prev->nLineBreak == AELB_WRAP))
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEEND, (LPARAM)&crRange.ciMax);
      SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTLINE, (LPARAM)&crRange.ciMax);
    }

    crInitialSel=crRange;
    if (!AEC_IndexCompare(&crSel.ciMin, &ciCaret))
      ciInitialCaret=crInitialSel.ciMin;
    else
      ciInitialCaret=crInitialSel.ciMax;

    if (nRangeLen=IndexSubtract(hWnd, &crRange.ciMin, &crRange.ciMax, AELB_ASIS, FALSE))
    {
      nStringLen=wcslen(wpString);
      nStringBytes=nStringLen * sizeof(wchar_t);

      SaveLineScroll(hWnd, &nFirstLine);
      SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
      SetSel(hWnd, &crRange, FALSE, NULL);
      if (nAction & STRSEL_INSERT)
      {
        nStringLenAll=(crRange.ciMax.nLine - crRange.ciMin.nLine + 1) * nStringLen;
        nBufferLen=nRangeLen + nStringLenAll;

        if (wszRange=(wchar_t *)API_HeapAlloc(hHeap, 0, nBufferLen * sizeof(wchar_t) + 2))
        {
          tr.cr=crRange;
          tr.bColumnSel=FALSE;
          tr.wpBuffer=wszRange + nStringLenAll;
          tr.dwBufferMax=(DWORD)-1;
          tr.nNewLine=AELB_ASIS;
          SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);
          b=nStringLenAll;

          memcpy(wszRange, wpString, nStringBytes);
          a=nStringLen;

          while (b < nBufferLen)
          {
            if (wszRange[b] == '\r' && wszRange[b + 1] == '\r' && wszRange[b + 2] == '\n')
            {
              wszRange[a++]=wszRange[b++];
              wszRange[a++]=wszRange[b++];
              wszRange[a++]=wszRange[b++];
            }
            else if (wszRange[b] == '\r' && wszRange[b + 1] == '\n')
            {
              wszRange[a++]=wszRange[b++];
              wszRange[a++]=wszRange[b++];
            }
            else if (wszRange[b] == '\r')
            {
              wszRange[a++]=wszRange[b++];
            }
            else if (wszRange[b] == '\n')
            {
              wszRange[a++]=wszRange[b++];
            }
            else
            {
              wszRange[a++]=wszRange[b++];
              continue;
            }

            if (b < nBufferLen)
            {
              memcpy(wszRange + a, wpString, nStringBytes);
              a+=nStringLen;
            }
          }
          wszRange[a]='\0';
          bResult=TRUE;
        }
      }
      else if (nAction & STRSEL_DELETE)
      {
        if (wszRange=(wchar_t *)API_HeapAlloc(hHeap, 0, nRangeLen * sizeof(wchar_t) + 2))
        {
          tr.cr=crRange;
          tr.bColumnSel=FALSE;
          tr.wpBuffer=wszRange;
          tr.dwBufferMax=(DWORD)-1;
          tr.nNewLine=AELB_ASIS;
          SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);

          if (nAction & STRSEL_TAB)
          {
            if (wszRange[b] == '\t')
              ++b;
            else
              for (i=0; i < nTabStopSize && wszRange[b] == ' '; ++i, ++b);
          }
          else if (nAction & STRSEL_SPACE)
          {
            if (wszRange[b] == ' ' || wszRange[b] == '\t')
              ++b;
          }
          else
          {
            if (!memcmp(wszRange + b, wpString, nStringBytes))
              b+=nStringLen;
          }

          while (b < nRangeLen)
          {
            if (wszRange[b] == '\r' && wszRange[b + 1] == '\r' && wszRange[b + 2] == '\n')
            {
              wszRange[a++]=wszRange[b++];
              wszRange[a++]=wszRange[b++];
              wszRange[a++]=wszRange[b++];
            }
            else if (wszRange[b] == '\r' && wszRange[b + 1] == '\n')
            {
              wszRange[a++]=wszRange[b++];
              wszRange[a++]=wszRange[b++];
            }
            else if (wszRange[b] == '\r')
            {
              wszRange[a++]=wszRange[b++];
            }
            else if (wszRange[b] == '\n')
            {
              wszRange[a++]=wszRange[b++];
            }
            else
            {
              wszRange[a++]=wszRange[b++];
              continue;
            }

            if (b < nRangeLen)
            {
              if (nAction & STRSEL_TAB)
              {
                if (wszRange[b] == '\t')
                  ++b;
                else
                  for (i=0; i < nTabStopSize && wszRange[b] == ' '; ++i, ++b);
              }
              else if (nAction & STRSEL_SPACE)
              {
                if (wszRange[b] == ' ' || wszRange[b] == '\t')
                  ++b;
              }
              else
              {
                if (!memcmp(wszRange + b, wpString, nStringBytes))
                  b+=nStringLen;
              }
            }
          }
          wszRange[a]='\0';
          bResult=TRUE;
        }
      }

      if (bResult)
      {
        ReplaceSelW(hWnd, wszRange, a, FALSE, &crRange.ciMin, &crRange.ciMax);

        //Update selection
        if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
          SetSel(hWnd, &crRange, FALSE, &crRange.ciMin);
        else
          SetSel(hWnd, &crRange, FALSE, &crRange.ciMax);
      }
      SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
      InvalidateRect(hWnd, NULL, TRUE);
      RestoreLineScroll(hWnd, &nFirstLine);

      API_HeapFree(hHeap, 0, (LPVOID)wszRange);
      return bResult;
    }
  }
  if (nAction & STRSEL_CHECK) return FALSE;

  return TRUE;
}

BOOL DoEditDeleteFirstCharW(HWND hWnd)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange=crSel;
  AECHARINDEX ciInitialCaret=ciCaret;
  wchar_t *wszRange;
  int nRangeLen;
  int a;
  int b;
  int nFirstLine;
  BOOL bDelete;

  if (IsReadOnly()) return FALSE;

  if (nRangeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszRange, AELB_ASIS))
  {
    bDelete=TRUE;
    a=0, b=0;

    while (b < nRangeLen)
    {
      while (wszRange[b] == '\r' || wszRange[b] == '\n')
      {
        bDelete=TRUE;
        wszRange[a++]=wszRange[b++];
      }
      if (bDelete)
      {
        bDelete=FALSE;
        b++;
      }
      else wszRange[a++]=wszRange[b++];
    }
    wszRange[a]='\0';

    SaveLineScroll(hWnd, &nFirstLine);
    SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
    ReplaceSelW(hWnd, wszRange, a, -1, &crRange.ciMin, &crRange.ciMax);

    //Update selection
    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, -1, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, -1, &crRange.ciMax);

    SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWnd, NULL, TRUE);
    RestoreLineScroll(hWnd, &nFirstLine);

    FreeText(wszRange);
    return TRUE;
  }
  SendMessage(hWnd, WM_KEYDOWN, VK_BACK, 0);
  SendMessage(hWnd, WM_KEYUP, VK_BACK, 0);
  return FALSE;
}

BOOL DoEditDeleteTrailingWhitespacesW(HWND hWnd)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  wchar_t *wszRange;
  int nRangeLen;
  int a;
  int b;
  int nFirstLine;
  BOOL bSelection;
  BOOL bResult=FALSE;

  if (IsReadOnly()) return FALSE;

  SaveLineScroll(hWnd, &nFirstLine);
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
    SetSel(hWnd, &crRange, FALSE, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nRangeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszRange, AELB_ASIS))
  {
    for (a=0, b=0; b < nRangeLen; wszRange[a++]=wszRange[b++])
    {
      if (wszRange[b] == '\r' || wszRange[b] == '\n')
      {
        while (--a >= 0 && (wszRange[a] == '\t' || wszRange[a] == ' '));
        ++a;
      }
    }
    while (--a >= 0 && (wszRange[a] == '\t' || wszRange[a] == ' '));
    ++a;
    wszRange[a]='\0';

    ReplaceSelW(hWnd, wszRange, a, -1, &crRange.ciMin, &crRange.ciMax);

    //Update selection
    if (!bSelection)
    {
      SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crInitialSel.ciMin);
      crRange.ciMin=crInitialSel.ciMin;
      crRange.ciMax=crInitialSel.ciMin;
    }

    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, -1, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, -1, &crRange.ciMax);

    FreeText(wszRange);
    bResult=TRUE;
  }

  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
  RestoreLineScroll(hWnd, &nFirstLine);

  return bResult;
}

BOOL DoEditChangeCaseA(HWND hWnd, int nCase)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  char *szRange;
  char *pStart;
  char *pEnd;
  int nRangeLen;
  int nFirstLine;
  BOOL bSelection;
  BOOL bResult=FALSE;

  if (IsReadOnly()) return FALSE;

  SaveLineScroll(hWnd, &nFirstLine);
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
    SetSel(hWnd, &crRange, FALSE, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nRangeLen=ExGetRangeTextA(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &szRange, AELB_ASIS))
  {
    pStart=szRange;
    pEnd=pStart + nRangeLen;

    if (nCase == UPPERCASE)
    {
      CharUpperBuffA(szRange, nRangeLen);
    }
    else if (nCase == LOWERCASE)
    {
      CharLowerBuffA(szRange, nRangeLen);
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

    ReplaceSelA(hWnd, szRange, -1, -1, NULL, NULL);

    //Update selection
    if (!bSelection)
    {
      SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crInitialSel.ciMin);
      crRange.ciMin=crInitialSel.ciMin;
      crRange.ciMax=crInitialSel.ciMin;
    }
    else
    {
      SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crRange.ciMin);
      SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crRange.ciMax);
    }

    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, -1, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, -1, &crRange.ciMax);

    FreeText(szRange);
    bResult=TRUE;
  }

  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
  RestoreLineScroll(hWnd, &nFirstLine);

  return bResult;
}

BOOL DoEditChangeCaseW(HWND hWnd, int nCase)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  wchar_t *wszRange;
  wchar_t *wpStart;
  wchar_t *wpEnd;
  int nRangeLen;
  int nFirstLine;
  BOOL bSelection;
  BOOL bResult=FALSE;

  if (IsReadOnly()) return FALSE;

  SaveLineScroll(hWnd, &nFirstLine);
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
    SetSel(hWnd, &crRange, FALSE, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nRangeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszRange, AELB_ASIS))
  {
    wpStart=wszRange;
    wpEnd=wpStart + nRangeLen;

    if (nCase == UPPERCASE)
    {
      CharUpperBuffW(wszRange, nRangeLen);
    }
    else if (nCase == LOWERCASE)
    {
      CharLowerBuffW(wszRange, nRangeLen);
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

    ReplaceSelW(hWnd, wszRange, -1, -1, NULL, NULL);

    //Update selection
    if (!bSelection)
    {
      SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crInitialSel.ciMin);
      crRange.ciMin=crInitialSel.ciMin;
      crRange.ciMax=crInitialSel.ciMin;
    }
    else
    {
      SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crRange.ciMin);
      SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crRange.ciMax);
    }

    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, -1, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, -1, &crRange.ciMax);

    FreeText(wszRange);
    bResult=TRUE;
  }

  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
  RestoreLineScroll(hWnd, &nFirstLine);

  return bResult;
}

void DoEditFindA()
{
  if (!hDlgModeless)
  {
    bReplaceDlg=FALSE;
    if (!bMDI && (ftflags & AEFR_ALLFILES)) ftflags&=~AEFR_ALLFILES;

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
    if (!bMDI && (ftflags & AEFR_ALLFILES)) ftflags&=~AEFR_ALLFILES;

    if (hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_FIND), hMainWnd, (DLGPROC)FindAndReplaceDlgProcW))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditFindNextDownA(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_UP & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_DOWN;

  if (szFind)
    FindTextA(hWnd, dwFlags, szFind);
  else
    DoEditFindA();
}

void DoEditFindNextDownW(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_UP & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_DOWN;

  if (wszFind)
    FindTextW(hWnd, dwFlags, wszFind);
  else
    DoEditFindW();
}

void DoEditFindNextUpA(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_DOWN & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_UP;

  if (szFind)
    FindTextA(hWnd, dwFlags, szFind);
  else
    DoEditFindA();
}

void DoEditFindNextUpW(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_DOWN & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_UP;

  if (wszFind)
    FindTextW(hWnd, dwFlags, wszFind);
  else
    DoEditFindW();
}

void DoEditReplaceA()
{
  if (!hDlgModeless)
  {
    bReplaceDlg=TRUE;
    if (!bMDI && (ftflags & AEFR_ALLFILES)) ftflags&=~AEFR_ALLFILES;

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
    if (!bMDI && (ftflags & AEFR_ALLFILES)) ftflags&=~AEFR_ALLFILES;

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

void DoViewColorsA()
{
  API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_COLORS), hMainWnd, (DLGPROC)ColorsDlgProcA);
}

void DoViewColorsW()
{
  API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_COLORS), hMainWnd, (DLGPROC)ColorsDlgProcW);
}

void DoViewFontSizeA(HWND hWnd, int nAction)
{
  if (nAction == INCREASE_FONT)
  {
    if (lfEditFontA.lfHeight <= -1)
    {
      lfEditFontA.lfHeight-=1;
      SetChosenFontA(hWnd, &lfEditFontA, TRUE);
    }
  }
  else if (nAction == DECREASE_FONT)
  {
    if (lfEditFontA.lfHeight < -1)
    {
      lfEditFontA.lfHeight+=1;
      SetChosenFontA(hWnd, &lfEditFontA, TRUE);
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
      SetChosenFontW(hWnd, &lfEditFontW, TRUE);
    }
  }
  else if (nAction == DECREASE_FONT)
  {
    if (lfEditFontW.lfHeight < -1)
    {
      lfEditFontW.lfHeight+=1;
      SetChosenFontW(hWnd, &lfEditFontW, TRUE);
    }
  }
}

void DoViewWordWrap(HWND hWnd, BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_WORDWRAP, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == bWordWrap) return;
  bWordWrap=bState;

  if (bWordWrap)
  {
    SendMessage(hWnd, AEM_SHOWSCROLLBAR, SB_HORZ, FALSE);
    SendMessage(hWnd, AEM_SETWORDWRAP, TRUE, TRUE);
  }
  else
  {
    SendMessage(hWnd, AEM_SETWORDWRAP, FALSE, TRUE);
    SendMessage(hWnd, AEM_SHOWSCROLLBAR, SB_HORZ, TRUE);
  }
}

void DoViewOnTop(BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_ONTOP, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == bOnTop) return;
  bOnTop=bState;

  if (bOnTop)
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

  if (hWnd)
  {
    SendMessage(hWnd, AEM_SETOPTIONS, bReadOnly?AECOOP_OR:AECOOP_XOR, AECO_READONLY);
  }
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
  bOptionsRestart=FALSE;

  PropertySheetA(&pshA);

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
  bOptionsRestart=FALSE;

  PropertySheetW(&pshW);

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

void DoHelpAboutA()
{
  API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_ABOUT), hMainWnd, (DLGPROC)AboutDlgProcA);
}

void DoHelpAboutW()
{
  API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_ABOUT), hMainWnd, (DLGPROC)AboutDlgProcW);
}

void DoNonMenuDelLine(HWND hWnd)
{
  AECHARRANGE cr=crSel;

  if (IsReadOnly()) return;

  cr.ciMin.nCharInLine=0;
  if (!SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTLINE, (LPARAM)&cr.ciMax))
    cr.ciMax.nCharInLine=cr.ciMax.lpLine->nLineLen;
  SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, TRUE);
  SetSel(hWnd, &cr, FALSE, NULL);
  SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, FALSE);

  ReplaceSelW(hWnd, L"", -1, FALSE, NULL, NULL);
}


//// INI functions

BOOL OpenIniA(HSTACK *hIniStack, char *pFile)
{
  HANDLE hFile;

  if (FileExistsA(pFile))
  {
    if ((hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
    {
      ReadIni(hIniStack, hFile);
      CloseHandle(hFile);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL OpenIniW(HSTACK *hIniStack, wchar_t *wpFile)
{
  HANDLE hFile;

  if (FileExistsW(wpFile))
  {
    if ((hFile=API_CreateFileW(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
    {
      ReadIni(hIniStack, hFile);
      CloseHandle(hFile);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CreateIniA(HSTACK *hIniStack, char *pFile)
{
  HANDLE hFile;

  if ((hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
  {
    ReadIni(hIniStack, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
}

BOOL CreateIniW(HSTACK *hIniStack, wchar_t *wpFile)
{
  HANDLE hFile;

  if ((hFile=API_CreateFileW(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
  {
    ReadIni(hIniStack, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
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

                if (!StackInsert((stack **)&hIniStack->first, (stack **)&hIniStack->last, (stack **)&lpIniSection, -1, sizeof(HINISECTION)))
                {
                  lpIniSection->nSectionUnicodeBytes=nSectionLen * sizeof(wchar_t) + 2;
                  if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionUnicodeBytes))
                    memcpy(lpIniSection->wszSection, wpSection, lpIniSection->nSectionUnicodeBytes);
                  lpIniSection->nSectionAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, NULL, 0, NULL, NULL);
                  if (lpIniSection->szSection=(char *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionAnsiBytes))
                    WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, lpIniSection->szSection, lpIniSection->nSectionAnsiBytes, NULL, NULL);
                  lpIniSection->hSectionStack.first=0;
                  lpIniSection->hSectionStack.last=0;
                }
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

                if (!StackInsert((stack **)&lpIniSection->hSectionStack.first, (stack **)&lpIniSection->hSectionStack.last, (stack **)&lpIniKey, -1, sizeof(HINIKEY)))
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

BOOL WriteIni(HSTACK *hIniStack, HANDLE hFile)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->last;
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

    lpIniKey=(HINIKEY *)lpIniSection->hSectionStack.last;

    while (lpIniKey)
    {
      //Keys and strings
      if (!API_WriteFile(hFile, lpIniKey->wszKey, lpIniKey->nKeyUnicodeBytes - 2, &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, L"=", 2, &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, lpIniKey->wszString, lpIniKey->nStringUnicodeBytes - 2, &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, L"\r\n", 4, &dwBytesWritten, NULL))
        return FALSE;
      lpIniKey=lpIniKey->prev;
    }
    if (lpIniSection->prev)
    {
      if (!API_WriteFile(hFile, L"\r\n", 4, &dwBytesWritten, NULL))
        return FALSE;
    }
    lpIniSection=lpIniSection->prev;
  }
  return TRUE;
}

HINISECTION* StackGetIniSectionA(HSTACK *hIniStack, char *pSection, int nSectionLen)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->last;

  nSectionLen=nSectionLen + 1;

  while (lpIniSection)
  {
    if (lpIniSection->nSectionAnsiBytes == nSectionLen)
    {
      if (!lstrcmpiA(lpIniSection->szSection, pSection))
        return lpIniSection;
    }
    lpIniSection=lpIniSection->prev;
  }
  return NULL;
}

HINISECTION* StackGetIniSectionW(HSTACK *hIniStack, wchar_t *wpSection, int nSectionLen)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->last;

  nSectionLen=nSectionLen * sizeof(wchar_t) + 2;

  while (lpIniSection)
  {
    if (lpIniSection->nSectionUnicodeBytes == nSectionLen)
    {
      if (!lstrcmpiW(lpIniSection->wszSection, wpSection))
        return lpIniSection;
    }
    lpIniSection=lpIniSection->prev;
  }
  return NULL;
}

HINIKEY* StackGetIniKeyA(HSTACK *hSectionStack, char *pKey, int nKeyLen)
{
  HINIKEY *lpIniKey=(HINIKEY *)hSectionStack->last;

  nKeyLen=nKeyLen + 1;

  while (lpIniKey)
  {
    if (lpIniKey->nKeyAnsiBytes == nKeyLen)
    {
      if (!lstrcmpiA(lpIniKey->szKey, pKey))
        return lpIniKey;
    }
    lpIniKey=lpIniKey->prev;
  }
  return NULL;
}

HINIKEY* StackGetIniKeyW(HSTACK *hSectionStack, wchar_t *wpKey, int nKeyLen)
{
  HINIKEY *lpIniKey=(HINIKEY *)hSectionStack->last;

  nKeyLen=nKeyLen * sizeof(wchar_t) + 2;

  while (lpIniKey)
  {
    if (lpIniKey->nKeyUnicodeBytes == nKeyLen)
    {
      if (!lstrcmpiW(lpIniKey->wszKey, wpKey))
        return lpIniKey;
    }
    lpIniKey=lpIniKey->prev;
  }
  return NULL;
}

void StackFreeIni(HSTACK *hIniStack)
{
  HINISECTION *lpIniSection=(HINISECTION *)hIniStack->last;

  while (lpIniSection)
  {
    if (lpIniSection->wszSection) API_HeapFree(hHeap, 0, (LPVOID)lpIniSection->wszSection);
    if (lpIniSection->szSection) API_HeapFree(hHeap, 0, (LPVOID)lpIniSection->szSection);
    StackFreeIniSection(&lpIniSection->hSectionStack);
    lpIniSection=lpIniSection->prev;
  }
  StackClear((stack **)&hIniStack->first, (stack **)&hIniStack->last);
}

void StackFreeIniSection(HSTACK *hSectionStack)
{
  HINIKEY *lpIniKey=(HINIKEY *)hSectionStack->last;

  while (lpIniKey)
  {
    if (lpIniKey->wszKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszKey);
    if (lpIniKey->szKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szKey);
    if (lpIniKey->wszString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);
    if (lpIniKey->szString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->szString);
    lpIniKey=lpIniKey->prev;
  }
  StackClear((stack **)&hSectionStack->first, (stack **)&hSectionStack->last);
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
    if (lpIniKey=StackGetIniKeyA(&lpIniSection->hSectionStack, pKey, nKeyLen))
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
    if (lpIniKey=StackGetIniKeyW(&lpIniSection->hSectionStack, wpKey, nKeyLen))
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
    if (!StackInsert((stack **)&hIniStack->first, (stack **)&hIniStack->last, (stack **)&lpIniSection, -1, sizeof(HINISECTION)))
    {
      lpIniSection->nSectionUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, pSection, nSectionLen + 1, NULL, 0) * sizeof(wchar_t);
      if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionUnicodeBytes))
        MultiByteToWideChar(CP_ACP, 0, pSection, nSectionLen + 1, lpIniSection->wszSection, lpIniSection->nSectionUnicodeBytes / sizeof(wchar_t));
      lpIniSection->nSectionAnsiBytes=nSectionLen + 1;
      if (lpIniSection->szSection=(char *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionAnsiBytes))
        memcpy(lpIniSection->szSection, pSection, lpIniSection->nSectionAnsiBytes);
      lpIniSection->hSectionStack.first=0;
      lpIniSection->hSectionStack.last=0;
    }
    else return FALSE;
  }
  if (!(lpIniKey=StackGetIniKeyA(&lpIniSection->hSectionStack, pKey, nKeyLen)))
  {
    if (!StackInsert((stack **)&lpIniSection->hSectionStack.first, (stack **)&lpIniSection->hSectionStack.last, (stack **)&lpIniKey, -1, sizeof(HINIKEY)))
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
    if (!StackInsert((stack **)&hIniStack->first, (stack **)&hIniStack->last, (stack **)&lpIniSection, -1, sizeof(HINISECTION)))
    {
      lpIniSection->nSectionUnicodeBytes=nSectionLen * sizeof(wchar_t) + 2;
      if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionUnicodeBytes))
        memcpy(lpIniSection->wszSection, wpSection, lpIniSection->nSectionUnicodeBytes);
      lpIniSection->nSectionAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, NULL, 0, NULL, NULL);
      if (lpIniSection->szSection=(char *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionAnsiBytes))
        WideCharToMultiByte(CP_ACP, 0, wpSection, nSectionLen + 1, lpIniSection->szSection, lpIniSection->nSectionAnsiBytes, NULL, NULL);
      lpIniSection->hSectionStack.first=0;
      lpIniSection->hSectionStack.last=0;
    }
    else return FALSE;
  }
  if (!(lpIniKey=StackGetIniKeyW(&lpIniSection->hSectionStack, wpKey, nKeyLen)))
  {
    if (!StackInsert((stack **)&lpIniSection->hSectionStack.first, (stack **)&lpIniSection->hSectionStack.last, (stack **)&lpIniKey, -1, sizeof(HINIKEY)))
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

void RegReadOptionsA()
{
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Options", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "WordWrap", NULL, &dwType, (LPBYTE)&bWordWrap, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "OnTop", NULL, &dwType, (LPBYTE)&bOnTop, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "StatusBar", NULL, &dwType, (LPBYTE)&bStatusBar, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "ReadOnly", NULL, &dwType, (LPBYTE)&bReadOnly, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SaveTime", NULL, &dwType, (LPBYTE)&bSaveTime, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "KeepSpace", NULL, &dwType, (LPBYTE)&bKeepSpace, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "UndoLimit", NULL, &dwType, (LPBYTE)&nUndoLimit, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "DetailedUndo", NULL, &dwType, (LPBYTE)&bDetailedUndo, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "CaretOutEdge", NULL, &dwType, (LPBYTE)&bCaretOutEdge, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "ReplaceAllAndClose", NULL, &dwType, (LPBYTE)&bReplaceAllAndClose, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SaveInReadOnlyMsg", NULL, &dwType, (LPBYTE)&bSaveInReadOnlyMsg, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "WatchFile", NULL, &dwType, (LPBYTE)&bWatchFile, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SingleOpenFile", NULL, &dwType, (LPBYTE)&bSingleOpenFile, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SingleOpenProgram", NULL, &dwType, (LPBYTE)&bSingleOpenProgram, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "MDI", NULL, &dwType, (LPBYTE)&bRegMDI, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SavePositions", NULL, &dwType, (LPBYTE)&bSavePositions, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SaveCodepages", NULL, &dwType, (LPBYTE)&bSaveCodepages, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "DefaultCodepage", NULL, &dwType, (LPBYTE)&nDefaultCodePage, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "CodepageRecognition", NULL, &dwType, (LPBYTE)&dwLangCodepageRecognition, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "CodepageRecognitionBuffer", NULL, &dwType, (LPBYTE)&dwCodepageRecognitionBuffer, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "RecentFiles", NULL, &dwType, (LPBYTE)&nRecentFiles, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SearchStrings", NULL, &dwType, (LPBYTE)&nSearchStrings, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "SearchOptions", NULL, &dwType, (LPBYTE)&ftflags, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "TabStopSize", NULL, &dwType, (LPBYTE)&nTabStopSize, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "TabStopAsSpaces", NULL, &dwType, (LPBYTE)&bTabStopAsSpaces, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "MarginsEdit", NULL, &dwType, (LPBYTE)&dwEditMargins, &dwSize);

  dwSize=sizeof(RECT);
  RegQueryValueExA(hKey, "MarginsPrint", NULL, &dwType, (LPBYTE)&psdPageA.rtMargin, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "WindowStyle", NULL, &dwType, (LPBYTE)&dwMainStyle, &dwSize);

  dwSize=sizeof(RECT);
  RegQueryValueExA(hKey, "WindowPosition", NULL, &dwType, (LPBYTE)&rcMainWindowRestored, &dwSize);

  if (bRegMDI)
  {
    dwSize=sizeof(DWORD);
    RegQueryValueExA(hKey, "TabViewMDI", NULL, &dwType, (LPBYTE)&nTabView, &dwSize);

    dwSize=sizeof(DWORD);
    RegQueryValueExA(hKey, "TabTypeMDI", NULL, &dwType, (LPBYTE)&nTabType, &dwSize);

    dwSize=sizeof(DWORD);
    RegQueryValueExA(hKey, "TabSwitchMDI", NULL, &dwType, (LPBYTE)&nTabSwitch, &dwSize);

    dwSize=sizeof(DWORD);
    RegQueryValueExA(hKey, "WindowStyleMDI", NULL, &dwType, (LPBYTE)&dwMdiStyle, &dwSize);
  }

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "WordDelimitersEnable", NULL, &dwType, (LPBYTE)&bWordDelimitersEnable, &dwSize);

  dwSize=WORD_DELIMITERS_SIZE * sizeof(wchar_t);
  RegQueryValueExA(hKey, "WordDelimiters", NULL, &dwType, (LPBYTE)wszWordDelimiters, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "WordBreak", NULL, &dwType, (LPBYTE)&dwCustomWordBreak, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "ShowURL", NULL, &dwType, (LPBYTE)&bShowURL, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "ClickURL", NULL, &dwType, (LPBYTE)&nClickURL, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "UrlPrefixesEnable", NULL, &dwType, (LPBYTE)&bUrlPrefixesEnable, &dwSize);

  dwSize=URL_PREFIXES_SIZE * sizeof(wchar_t);
  RegQueryValueExA(hKey, "UrlPrefixes", NULL, &dwType, (LPBYTE)wszUrlPrefixes, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "UrlDelimitersEnable", NULL, &dwType, (LPBYTE)&bUrlDelimitersEnable, &dwSize);

  dwSize=URL_DELIMITERS_SIZE * sizeof(wchar_t);
  RegQueryValueExA(hKey, "UrlDelimiters", NULL, &dwType, (LPBYTE)wszUrlDelimiters, &dwSize);

  dwSize=sizeof(LOGFONTA) - LF_FACESIZE;
  RegQueryValueExA(hKey, "Font", NULL, &dwType, (LPBYTE)&lfEditFontA, &dwSize);

  dwSize=LF_FACESIZE;
  RegQueryValueExA(hKey, "FontFace", NULL, &dwType, (LPBYTE)&lfEditFontA.lfFaceName, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "PrintFontEnable", NULL, &dwType, (LPBYTE)&bPrintFontEnable, &dwSize);

  dwSize=sizeof(LOGFONTA) - LF_FACESIZE;
  RegQueryValueExA(hKey, "PrintFont", NULL, &dwType, (LPBYTE)&lfPrintFontA, &dwSize);

  dwSize=LF_FACESIZE;
  RegQueryValueExA(hKey, "PrintFontFace", NULL, &dwType, (LPBYTE)&lfPrintFontA.lfFaceName, &dwSize);

  dwSize=sizeof(AECOLORS);
  RegQueryValueExA(hKey, "Colors", NULL, &dwType, (LPBYTE)&aecColors, &dwSize);

  dwSize=MAX_PATH;
  RegQueryValueExA(hKey, "LanguageModule", NULL, &dwType, (LPBYTE)szLangModule, &dwSize);

  dwSize=BUFFER_SIZE;
  RegQueryValueExA(hKey, "ExecuteCommand", NULL, &dwType, (LPBYTE)szCommand, &dwSize);

  dwSize=MAX_PATH;
  RegQueryValueExA(hKey, "ExecuteDirectory", NULL, &dwType, (LPBYTE)szWorkDir, &dwSize);

  dwSize=MAX_PATH;
  RegQueryValueExA(hKey, "FileTypesOpen", NULL, &dwType, (LPBYTE)szFileTypesOpen, &dwSize);

  dwSize=MAX_PATH;
  RegQueryValueExA(hKey, "FileTypesEdit", NULL, &dwType, (LPBYTE)szFileTypesEdit, &dwSize);

  dwSize=MAX_PATH;
  RegQueryValueExA(hKey, "FileTypesPrint", NULL, &dwType, (LPBYTE)szFileTypesPrint, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "FileTypesAssociated", NULL, &dwType, (LPBYTE)&dwFileTypesAssociated, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "PrintHeaderEnable", NULL, &dwType, (LPBYTE)&bPrintHeaderEnable, &dwSize);

  dwSize=MAX_PATH;
  RegQueryValueExA(hKey, "PrintHeader", NULL, &dwType, (LPBYTE)szPrintHeader, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExA(hKey, "PrintFooterEnable", NULL, &dwType, (LPBYTE)&bPrintFooterEnable, &dwSize);

  dwSize=MAX_PATH;
  RegQueryValueExA(hKey, "PrintFooter", NULL, &dwType, (LPBYTE)szPrintFooter, &dwSize);

  if (RegQueryValueExA(hKey, "CodepageList", NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
  {
    if (lpCodepageList=(int *)API_HeapAlloc(hHeap, 0, dwSize))
    {
      RegQueryValueExA(hKey, "CodepageList", NULL, &dwType, (LPBYTE)lpCodepageList, &dwSize);
      nCodepageListLen=CodepageListLen(lpCodepageList);
    }
  }

  RegCloseKey(hKey);
}

void RegReadOptionsW()
{
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Options", 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"WordWrap", NULL, &dwType, (LPBYTE)&bWordWrap, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"OnTop", NULL, &dwType, (LPBYTE)&bOnTop, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"StatusBar", NULL, &dwType, (LPBYTE)&bStatusBar, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"ReadOnly", NULL, &dwType, (LPBYTE)&bReadOnly, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SaveTime", NULL, &dwType, (LPBYTE)&bSaveTime, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"KeepSpace", NULL, &dwType, (LPBYTE)&bKeepSpace, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"UndoLimit", NULL, &dwType, (LPBYTE)&nUndoLimit, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"DetailedUndo", NULL, &dwType, (LPBYTE)&bDetailedUndo, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"CaretOutEdge", NULL, &dwType, (LPBYTE)&bCaretOutEdge, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"ReplaceAllAndClose", NULL, &dwType, (LPBYTE)&bReplaceAllAndClose, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SaveInReadOnlyMsg", NULL, &dwType, (LPBYTE)&bSaveInReadOnlyMsg, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"WatchFile", NULL, &dwType, (LPBYTE)&bWatchFile, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SingleOpenFile", NULL, &dwType, (LPBYTE)&bSingleOpenFile, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SingleOpenProgram", NULL, &dwType, (LPBYTE)&bSingleOpenProgram, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"MDI", NULL, &dwType, (LPBYTE)&bRegMDI, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SavePositions", NULL, &dwType, (LPBYTE)&bSavePositions, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SaveCodepages", NULL, &dwType, (LPBYTE)&bSaveCodepages, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"DefaultCodepage", NULL, &dwType, (LPBYTE)&nDefaultCodePage, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"CodepageRecognition", NULL, &dwType, (LPBYTE)&dwLangCodepageRecognition, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"CodepageRecognitionBuffer", NULL, &dwType, (LPBYTE)&dwCodepageRecognitionBuffer, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"RecentFiles", NULL, &dwType, (LPBYTE)&nRecentFiles, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SearchStrings", NULL, &dwType, (LPBYTE)&nSearchStrings, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"SearchOptions", NULL, &dwType, (LPBYTE)&ftflags, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"TabStopSize", NULL, &dwType, (LPBYTE)&nTabStopSize, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"TabStopAsSpaces", NULL, &dwType, (LPBYTE)&bTabStopAsSpaces, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"MarginsEdit", NULL, &dwType, (LPBYTE)&dwEditMargins, &dwSize);

  dwSize=sizeof(RECT);
  RegQueryValueExW(hKey, L"MarginsPrint", NULL, &dwType, (LPBYTE)&psdPageW.rtMargin, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"WindowStyle", NULL, &dwType, (LPBYTE)&dwMainStyle, &dwSize);

  dwSize=sizeof(RECT);
  RegQueryValueExW(hKey, L"WindowPosition", NULL, &dwType, (LPBYTE)&rcMainWindowRestored, &dwSize);

  if (bRegMDI)
  {
    dwSize=sizeof(DWORD);
    RegQueryValueExW(hKey, L"TabViewMDI", NULL, &dwType, (LPBYTE)&nTabView, &dwSize);

    dwSize=sizeof(DWORD);
    RegQueryValueExW(hKey, L"TabTypeMDI", NULL, &dwType, (LPBYTE)&nTabType, &dwSize);

    dwSize=sizeof(DWORD);
    RegQueryValueExW(hKey, L"TabSwitchMDI", NULL, &dwType, (LPBYTE)&nTabSwitch, &dwSize);

    dwSize=sizeof(DWORD);
    RegQueryValueExW(hKey, L"WindowStyleMDI", NULL, &dwType, (LPBYTE)&dwMdiStyle, &dwSize);
  }

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"WordDelimitersEnable", NULL, &dwType, (LPBYTE)&bWordDelimitersEnable, &dwSize);

  dwSize=WORD_DELIMITERS_SIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"WordDelimiters", NULL, &dwType, (LPBYTE)wszWordDelimiters, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"WordBreak", NULL, &dwType, (LPBYTE)&dwCustomWordBreak, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"ShowURL", NULL, &dwType, (LPBYTE)&bShowURL, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"ClickURL", NULL, &dwType, (LPBYTE)&nClickURL, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"UrlPrefixesEnable", NULL, &dwType, (LPBYTE)&bUrlPrefixesEnable, &dwSize);

  dwSize=URL_PREFIXES_SIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"UrlPrefixes", NULL, &dwType, (LPBYTE)wszUrlPrefixes, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"UrlDelimitersEnable", NULL, &dwType, (LPBYTE)&bUrlDelimitersEnable, &dwSize);

  dwSize=URL_DELIMITERS_SIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"UrlDelimiters", NULL, &dwType, (LPBYTE)wszUrlDelimiters, &dwSize);

  dwSize=sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"Font", NULL, &dwType, (LPBYTE)&lfEditFontW, &dwSize);

  dwSize=LF_FACESIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"FontFace", NULL, &dwType, (LPBYTE)&lfEditFontW.lfFaceName, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"PrintFontEnable", NULL, &dwType, (LPBYTE)&bPrintFontEnable, &dwSize);

  dwSize=sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"PrintFont", NULL, &dwType, (LPBYTE)&lfPrintFontW, &dwSize);

  dwSize=LF_FACESIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"PrintFontFace", NULL, &dwType, (LPBYTE)&lfPrintFontW.lfFaceName, &dwSize);

  dwSize=sizeof(AECOLORS);
  RegQueryValueExW(hKey, L"Colors", NULL, &dwType, (LPBYTE)&aecColors, &dwSize);

  dwSize=MAX_PATH * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"LanguageModule", NULL, &dwType, (LPBYTE)wszLangModule, &dwSize);

  dwSize=BUFFER_SIZE * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"ExecuteCommand", NULL, &dwType, (LPBYTE)wszCommand, &dwSize);

  dwSize=MAX_PATH * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"ExecuteDirectory", NULL, &dwType, (LPBYTE)wszWorkDir, &dwSize);

  dwSize=MAX_PATH * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"FileTypesOpen", NULL, &dwType, (LPBYTE)wszFileTypesOpen, &dwSize);

  dwSize=MAX_PATH * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"FileTypesEdit", NULL, &dwType, (LPBYTE)wszFileTypesEdit, &dwSize);

  dwSize=MAX_PATH * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"FileTypesPrint", NULL, &dwType, (LPBYTE)wszFileTypesPrint, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"FileTypesAssociated", NULL, &dwType, (LPBYTE)&dwFileTypesAssociated, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"PrintHeaderEnable", NULL, &dwType, (LPBYTE)&bPrintHeaderEnable, &dwSize);

  dwSize=MAX_PATH * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"PrintHeader", NULL, &dwType, (LPBYTE)wszPrintHeader, &dwSize);

  dwSize=sizeof(DWORD);
  RegQueryValueExW(hKey, L"PrintFooterEnable", NULL, &dwType, (LPBYTE)&bPrintFooterEnable, &dwSize);

  dwSize=MAX_PATH * sizeof(wchar_t);
  RegQueryValueExW(hKey, L"PrintFooter", NULL, &dwType, (LPBYTE)wszPrintFooter, &dwSize);

  if (RegQueryValueExW(hKey, L"CodepageList", NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
  {
    if (lpCodepageList=(int *)API_HeapAlloc(hHeap, 0, dwSize))
    {
      RegQueryValueExW(hKey, L"CodepageList", NULL, &dwType, (LPBYTE)lpCodepageList, &dwSize);
      nCodepageListLen=CodepageListLen(lpCodepageList);
    }
  }

  RegCloseKey(hKey);
}

void IniReadOptionsA()
{
  DWORD dwSize;

  IniGetValueA(&hIniStack, "Options", "WordWrap", INI_DWORD, (LPBYTE)&bWordWrap, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "OnTop", INI_DWORD, (LPBYTE)&bOnTop, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "StatusBar", INI_DWORD, (LPBYTE)&bStatusBar, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "ReadOnly", INI_DWORD, (LPBYTE)&bReadOnly, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SaveTime", INI_DWORD, (LPBYTE)&bSaveTime, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "KeepSpace", INI_DWORD, (LPBYTE)&bKeepSpace, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "UndoLimit", INI_DWORD, (LPBYTE)&nUndoLimit, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "DetailedUndo", INI_DWORD, (LPBYTE)&bDetailedUndo, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "CaretOutEdge", INI_DWORD, (LPBYTE)&bCaretOutEdge, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "ReplaceAllAndClose", INI_DWORD, (LPBYTE)&bReplaceAllAndClose, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SaveInReadOnlyMsg", INI_DWORD, (LPBYTE)&bSaveInReadOnlyMsg, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "WatchFile", INI_DWORD, (LPBYTE)&bWatchFile, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SingleOpenFile", INI_DWORD, (LPBYTE)&bSingleOpenFile, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SingleOpenProgram", INI_DWORD, (LPBYTE)&bSingleOpenProgram, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "MDI", INI_DWORD, (LPBYTE)&bRegMDI, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SavePositions", INI_DWORD, (LPBYTE)&bSavePositions, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SaveCodepages", INI_DWORD, (LPBYTE)&bSaveCodepages, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "DefaultCodepage", INI_DWORD, (LPBYTE)&nDefaultCodePage, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "CodepageRecognition", INI_DWORD, (LPBYTE)&dwLangCodepageRecognition, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "CodepageRecognitionBuffer", INI_DWORD, (LPBYTE)&dwCodepageRecognitionBuffer, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "RecentFiles", INI_DWORD, (LPBYTE)&nRecentFiles, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SearchStrings", INI_DWORD, (LPBYTE)&nSearchStrings, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "SearchOptions", INI_DWORD, (LPBYTE)&ftflags, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "TabStopSize", INI_DWORD, (LPBYTE)&nTabStopSize, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "TabStopAsSpaces", INI_DWORD, (LPBYTE)&bTabStopAsSpaces, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "MarginsEdit", INI_DWORD, (LPBYTE)&dwEditMargins, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "MarginsPrint", INI_BINARY, (LPBYTE)&psdPageA.rtMargin, sizeof(RECT));
  IniGetValueA(&hIniStack, "Options", "WindowStyle", INI_DWORD, (LPBYTE)&dwMainStyle, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "WindowPosition", INI_BINARY, (LPBYTE)&rcMainWindowRestored, sizeof(RECT));

  if (bRegMDI)
  {
    IniGetValueA(&hIniStack, "Options", "TabViewMDI", INI_DWORD, (LPBYTE)&nTabView, sizeof(DWORD));
    IniGetValueA(&hIniStack, "Options", "TabTypeMDI", INI_DWORD, (LPBYTE)&nTabType, sizeof(DWORD));
    IniGetValueA(&hIniStack, "Options", "TabSwitchMDI", INI_DWORD, (LPBYTE)&nTabSwitch, sizeof(DWORD));
    IniGetValueA(&hIniStack, "Options", "WindowStyleMDI", INI_DWORD, (LPBYTE)&dwMdiStyle, sizeof(DWORD));
  }


  IniGetValueA(&hIniStack, "Options", "WordDelimitersEnable", INI_DWORD, (LPBYTE)&bWordDelimitersEnable, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "WordDelimiters", INI_BINARY, (LPBYTE)wszWordDelimiters, WORD_DELIMITERS_SIZE * sizeof(wchar_t));
  IniGetValueA(&hIniStack, "Options", "WordBreak", INI_DWORD, (LPBYTE)&dwCustomWordBreak, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "ShowURL", INI_DWORD, (LPBYTE)&bShowURL, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "ClickURL", INI_DWORD, (LPBYTE)&nClickURL, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "UrlPrefixesEnable", INI_DWORD, (LPBYTE)&bUrlPrefixesEnable, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "UrlPrefixes", INI_BINARY, (LPBYTE)wszUrlPrefixes, URL_PREFIXES_SIZE * sizeof(wchar_t));
  IniGetValueA(&hIniStack, "Options", "UrlDelimitersEnable", INI_DWORD, (LPBYTE)&bUrlDelimitersEnable, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "UrlDelimiters", INI_BINARY, (LPBYTE)wszUrlDelimiters, URL_DELIMITERS_SIZE * sizeof(wchar_t));
  IniGetValueA(&hIniStack, "Options", "Font", INI_BINARY, (LPBYTE)&lfEditFontA, sizeof(LOGFONTA) - LF_FACESIZE);
  IniGetValueA(&hIniStack, "Options", "FontFace", INI_STRINGANSI, (LPBYTE)&lfEditFontA.lfFaceName, LF_FACESIZE);
  IniGetValueA(&hIniStack, "Options", "PrintFontEnable", INI_DWORD, (LPBYTE)&bPrintFontEnable, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "PrintFont", INI_BINARY, (LPBYTE)&lfPrintFontA, sizeof(LOGFONTA) - LF_FACESIZE);
  IniGetValueA(&hIniStack, "Options", "PrintFontFace", INI_STRINGANSI, (LPBYTE)&lfPrintFontA.lfFaceName, LF_FACESIZE);
  IniGetValueA(&hIniStack, "Options", "Colors", INI_BINARY, (LPBYTE)&aecColors, sizeof(AECOLORS));
  IniGetValueA(&hIniStack, "Options", "LanguageModule", INI_STRINGANSI, (LPBYTE)szLangModule, MAX_PATH);
  IniGetValueA(&hIniStack, "Options", "ExecuteCommand", INI_STRINGANSI, (LPBYTE)szCommand, BUFFER_SIZE);
  IniGetValueA(&hIniStack, "Options", "ExecuteDirectory", INI_STRINGANSI, (LPBYTE)szWorkDir, MAX_PATH);
  IniGetValueA(&hIniStack, "Options", "FileTypesOpen", INI_STRINGANSI, (LPBYTE)szFileTypesOpen, MAX_PATH);
  IniGetValueA(&hIniStack, "Options", "FileTypesEdit", INI_STRINGANSI, (LPBYTE)szFileTypesEdit, MAX_PATH);
  IniGetValueA(&hIniStack, "Options", "FileTypesPrint", INI_STRINGANSI, (LPBYTE)szFileTypesPrint, MAX_PATH);
  IniGetValueA(&hIniStack, "Options", "FileTypesAssociated", INI_DWORD, (LPBYTE)&dwFileTypesAssociated, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "PrintHeaderEnable", INI_DWORD, (LPBYTE)&bPrintHeaderEnable, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "PrintHeader", INI_STRINGANSI, (LPBYTE)szPrintHeader, MAX_PATH);
  IniGetValueA(&hIniStack, "Options", "PrintFooterEnable", INI_DWORD, (LPBYTE)&bPrintFooterEnable, sizeof(DWORD));
  IniGetValueA(&hIniStack, "Options", "PrintFooter", INI_STRINGANSI, (LPBYTE)szPrintFooter, MAX_PATH);

  if (dwSize=IniGetValueA(&hIniStack, "Options", "CodepageList", INI_BINARY, NULL, 0))
  {
    if (lpCodepageList=(int *)API_HeapAlloc(hHeap, 0, dwSize))
    {
      IniGetValueA(&hIniStack, "Options", "CodepageList", INI_BINARY, (LPBYTE)lpCodepageList, dwSize);
      nCodepageListLen=CodepageListLen(lpCodepageList);
    }
  }
}

void IniReadOptionsW()
{
  DWORD dwSize;

  IniGetValueW(&hIniStack, L"Options", L"WordWrap", INI_DWORD, (LPBYTE)&bWordWrap, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"OnTop", INI_DWORD, (LPBYTE)&bOnTop, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"StatusBar", INI_DWORD, (LPBYTE)&bStatusBar, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"ReadOnly", INI_DWORD, (LPBYTE)&bReadOnly, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SaveTime", INI_DWORD, (LPBYTE)&bSaveTime, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"KeepSpace", INI_DWORD, (LPBYTE)&bKeepSpace, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"UndoLimit", INI_DWORD, (LPBYTE)&nUndoLimit, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"DetailedUndo", INI_DWORD, (LPBYTE)&bDetailedUndo, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"CaretOutEdge", INI_DWORD, (LPBYTE)&bCaretOutEdge, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"ReplaceAllAndClose", INI_DWORD, (LPBYTE)&bReplaceAllAndClose, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SaveInReadOnlyMsg", INI_DWORD, (LPBYTE)&bSaveInReadOnlyMsg, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"WatchFile", INI_DWORD, (LPBYTE)&bWatchFile, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SingleOpenFile", INI_DWORD, (LPBYTE)&bSingleOpenFile, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SingleOpenProgram", INI_DWORD, (LPBYTE)&bSingleOpenProgram, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"MDI", INI_DWORD, (LPBYTE)&bRegMDI, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SavePositions", INI_DWORD, (LPBYTE)&bSavePositions, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SaveCodepages", INI_DWORD, (LPBYTE)&bSaveCodepages, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"DefaultCodepage", INI_DWORD, (LPBYTE)&nDefaultCodePage, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"CodepageRecognition", INI_DWORD, (LPBYTE)&dwLangCodepageRecognition, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"CodepageRecognitionBuffer", INI_DWORD, (LPBYTE)&dwCodepageRecognitionBuffer, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"RecentFiles", INI_DWORD, (LPBYTE)&nRecentFiles, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SearchStrings", INI_DWORD, (LPBYTE)&nSearchStrings, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"SearchOptions", INI_DWORD, (LPBYTE)&ftflags, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"TabStopSize", INI_DWORD, (LPBYTE)&nTabStopSize, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"TabStopAsSpaces", INI_DWORD, (LPBYTE)&bTabStopAsSpaces, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"MarginsEdit", INI_DWORD, (LPBYTE)&dwEditMargins, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"MarginsPrint", INI_BINARY, (LPBYTE)&psdPageW.rtMargin, sizeof(RECT));
  IniGetValueW(&hIniStack, L"Options", L"WindowStyle", INI_DWORD, (LPBYTE)&dwMainStyle, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"WindowPosition", INI_BINARY, (LPBYTE)&rcMainWindowRestored, sizeof(RECT));

  if (bRegMDI)
  {
    IniGetValueW(&hIniStack, L"Options", L"TabViewMDI", INI_DWORD, (LPBYTE)&nTabView, sizeof(DWORD));
    IniGetValueW(&hIniStack, L"Options", L"TabTypeMDI", INI_DWORD, (LPBYTE)&nTabType, sizeof(DWORD));
    IniGetValueW(&hIniStack, L"Options", L"TabSwitchMDI", INI_DWORD, (LPBYTE)&nTabSwitch, sizeof(DWORD));
    IniGetValueW(&hIniStack, L"Options", L"WindowStyleMDI", INI_DWORD, (LPBYTE)&dwMdiStyle, sizeof(DWORD));
  }

  IniGetValueW(&hIniStack, L"Options", L"WordDelimitersEnable", INI_DWORD, (LPBYTE)&bWordDelimitersEnable, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"WordDelimiters", INI_BINARY, (LPBYTE)wszWordDelimiters, WORD_DELIMITERS_SIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"WordBreak", INI_DWORD, (LPBYTE)&dwCustomWordBreak, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"ShowURL", INI_DWORD, (LPBYTE)&bShowURL, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"ClickURL", INI_DWORD, (LPBYTE)&nClickURL, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"UrlPrefixesEnable", INI_DWORD, (LPBYTE)&bUrlPrefixesEnable, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"UrlPrefixes", INI_BINARY, (LPBYTE)wszUrlPrefixes, URL_PREFIXES_SIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"UrlDelimitersEnable", INI_DWORD, (LPBYTE)&bUrlDelimitersEnable, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"UrlDelimiters", INI_BINARY, (LPBYTE)wszUrlDelimiters, URL_DELIMITERS_SIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"Font", INI_BINARY, (LPBYTE)&lfEditFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"FontFace", INI_STRINGUNICODE, (LPBYTE)&lfEditFontW.lfFaceName, LF_FACESIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"PrintFontEnable", INI_DWORD, (LPBYTE)&bPrintFontEnable, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"PrintFont", INI_BINARY, (LPBYTE)&lfPrintFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"PrintFontFace", INI_STRINGUNICODE, (LPBYTE)&lfPrintFontW.lfFaceName, LF_FACESIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"Colors", INI_BINARY, (LPBYTE)&aecColors, sizeof(AECOLORS));
  IniGetValueW(&hIniStack, L"Options", L"LanguageModule", INI_STRINGUNICODE, (LPBYTE)wszLangModule, MAX_PATH * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"ExecuteCommand", INI_STRINGUNICODE, (LPBYTE)wszCommand, BUFFER_SIZE * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"ExecuteDirectory", INI_STRINGUNICODE, (LPBYTE)wszWorkDir, MAX_PATH * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"FileTypesOpen", INI_STRINGUNICODE, (LPBYTE)wszFileTypesOpen, MAX_PATH * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"FileTypesEdit", INI_STRINGUNICODE, (LPBYTE)wszFileTypesEdit, MAX_PATH * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"FileTypesPrint", INI_STRINGUNICODE, (LPBYTE)wszFileTypesPrint, MAX_PATH * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"FileTypesAssociated", INI_DWORD, (LPBYTE)&dwFileTypesAssociated, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"PrintHeaderEnable", INI_DWORD, (LPBYTE)&bPrintHeaderEnable, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"PrintHeader", INI_STRINGUNICODE, (LPBYTE)wszPrintHeader, MAX_PATH * sizeof(wchar_t));
  IniGetValueW(&hIniStack, L"Options", L"PrintFooterEnable", INI_DWORD, (LPBYTE)&bPrintFooterEnable, sizeof(DWORD));
  IniGetValueW(&hIniStack, L"Options", L"PrintFooter", INI_STRINGUNICODE, (LPBYTE)wszPrintFooter, MAX_PATH * sizeof(wchar_t));

  if (dwSize=IniGetValueW(&hIniStack, L"Options", L"CodepageList", INI_BINARY, NULL, 0))
  {
    if (lpCodepageList=(int *)API_HeapAlloc(hHeap, 0, dwSize))
    {
      IniGetValueW(&hIniStack, L"Options", L"CodepageList", INI_BINARY, (LPBYTE)lpCodepageList, dwSize);
      nCodepageListLen=CodepageListLen(lpCodepageList);
    }
  }
}

void RegRegisterPluginsHotkeysA()
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

void RegRegisterPluginsHotkeysW()
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

void IniRegisterPluginsHotkeysA()
{
  HINISECTION *lpIniSection;
  HINIKEY *lpIniKey;
  DWORD dwHotkey=0;

  if (lpIniSection=StackGetIniSectionA(&hIniStack, "Plugs", lstrlenA("Plugs")))
  {
    lpIniKey=(HINIKEY *)lpIniSection->hSectionStack.last;

    while (lpIniKey)
    {
      if (dwHotkey=(DWORD)xatoiA(lpIniKey->szString))
      {
        StackPluginAddA(&hPluginsStack, lpIniKey->szKey, lpIniKey->nKeyAnsiBytes - 1, LOWORD(dwHotkey), HIWORD(dwHotkey), FALSE, NULL, NULL);
      }
      lpIniKey=lpIniKey->prev;
    }
  }
}

void IniRegisterPluginsHotkeysW()
{
  HINISECTION *lpIniSection;
  HINIKEY *lpIniKey;
  DWORD dwHotkey=0;

  if (lpIniSection=StackGetIniSectionW(&hIniStack, L"Plugs", lstrlenW(L"Plugs")))
  {
    lpIniKey=(HINIKEY *)lpIniSection->hSectionStack.last;

    while (lpIniKey)
    {
      if (dwHotkey=(DWORD)xatoiW(lpIniKey->wszString))
      {
        StackPluginAddW(&hPluginsStack, lpIniKey->wszKey, (lpIniKey->nKeyUnicodeBytes - 1) / sizeof(wchar_t), LOWORD(dwHotkey), HIWORD(dwHotkey), FALSE, NULL, NULL);
      }
      lpIniKey=lpIniKey->prev;
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
        if (ftflags & AEFR_ESCAPESEQ)
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
        if (ftflags & AEFR_ESCAPESEQ)
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

BOOL RegSaveOptionsA()
{
  HKEY hKey;

  dwMainStyle=GetWindowLongA(hMainWnd, GWL_STYLE);
  dwMainStyle=((dwMainStyle & WS_MAXIMIZE) || ((dwMainStyle & WS_MINIMIZE) && dwLastMainSize == SIZE_MAXIMIZED))?WS_MAXIMIZE:0;
  if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
  dwMdiStyle=(bMdiMaximize == TRUE)?WS_MAXIMIZE:0;

  if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Options", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return FALSE;

  if (RegSetValueExA(hKey, "WordWrap", 0, REG_DWORD, (LPBYTE)&bWordWrap, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "OnTop", 0, REG_DWORD, (LPBYTE)&bOnTop, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "StatusBar", 0, REG_DWORD, (LPBYTE)&bStatusBar, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "ReadOnly", 0, REG_DWORD, (LPBYTE)&bReadOnly, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SaveTime", 0, REG_DWORD, (LPBYTE)&bSaveTime, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "KeepSpace", 0, REG_DWORD, (LPBYTE)&bKeepSpace, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "UndoLimit", 0, REG_DWORD, (LPBYTE)&nUndoLimit, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "DetailedUndo", 0, REG_DWORD, (LPBYTE)&bDetailedUndo, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "CaretOutEdge", 0, REG_DWORD, (LPBYTE)&bCaretOutEdge, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "ReplaceAllAndClose", 0, REG_DWORD, (LPBYTE)&bReplaceAllAndClose, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SaveInReadOnlyMsg", 0, REG_DWORD, (LPBYTE)&bSaveInReadOnlyMsg, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "WatchFile", 0, REG_DWORD, (LPBYTE)&bWatchFile, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SingleOpenFile", 0, REG_DWORD, (LPBYTE)&bSingleOpenFile, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SingleOpenProgram", 0, REG_DWORD, (LPBYTE)&bSingleOpenProgram, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "MDI", 0, REG_DWORD, (LPBYTE)&bRegMDI, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SavePositions", 0, REG_DWORD, (LPBYTE)&bSavePositions, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SaveCodepages", 0, REG_DWORD, (LPBYTE)&bSaveCodepages, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "DefaultCodepage", 0, REG_DWORD, (LPBYTE)&nDefaultCodePage, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "CodepageRecognition", 0, REG_DWORD, (LPBYTE)&dwLangCodepageRecognition, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "CodepageRecognitionBuffer", 0, REG_DWORD, (LPBYTE)&dwCodepageRecognitionBuffer, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "RecentFiles", 0, REG_DWORD, (LPBYTE)&nRecentFiles, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SearchStrings", 0, REG_DWORD, (LPBYTE)&nSearchStrings, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "SearchOptions", 0, REG_DWORD, (LPBYTE)&ftflags, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "TabStopSize", 0, REG_DWORD, (LPBYTE)&nTabStopSize, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "TabStopAsSpaces", 0, REG_DWORD, (LPBYTE)&bTabStopAsSpaces, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "MarginsEdit", 0, REG_DWORD, (LPBYTE)&dwEditMargins, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "MarginsPrint", 0, REG_BINARY, (LPBYTE)&psdPageA.rtMargin, sizeof(RECT)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "WindowStyle", 0, REG_DWORD, (LPBYTE)&dwMainStyle, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "WindowPosition", 0, REG_BINARY, (LPBYTE)&rcMainWindowRestored, sizeof(RECT)) != ERROR_SUCCESS)
    goto Error;

  if (bMDI)
  {
    if (RegSetValueExA(hKey, "TabViewMDI", 0, REG_DWORD, (LPBYTE)&nTabView, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExA(hKey, "TabTypeMDI", 0, REG_DWORD, (LPBYTE)&nTabType, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExA(hKey, "TabSwitchMDI", 0, REG_DWORD, (LPBYTE)&nTabSwitch, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExA(hKey, "WindowStyleMDI", 0, REG_DWORD, (LPBYTE)&dwMdiStyle, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
  }

  if (RegSetValueExA(hKey, "WordDelimitersEnable", 0, REG_DWORD, (LPBYTE)&bWordDelimitersEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "WordDelimiters", 0, REG_BINARY, (LPBYTE)wszWordDelimiters, wcslen(wszWordDelimiters) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "WordBreak", 0, REG_DWORD, (LPBYTE)&dwCustomWordBreak, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "ShowURL", 0, REG_DWORD, (LPBYTE)&bShowURL, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "ClickURL", 0, REG_DWORD, (LPBYTE)&nClickURL, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "UrlPrefixesEnable", 0, REG_DWORD, (LPBYTE)&bUrlPrefixesEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "UrlPrefixes", 0, REG_BINARY, (LPBYTE)wszUrlPrefixes, wcslen(wszUrlPrefixes) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "UrlDelimitersEnable", 0, REG_DWORD, (LPBYTE)&bUrlDelimitersEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "UrlDelimiters", 0, REG_BINARY, (LPBYTE)wszUrlDelimiters, wcslen(wszUrlDelimiters) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;

  if (bEditFontChanged)
  {
    if (RegSetValueExA(hKey, "Font", 0, REG_BINARY, (LPBYTE)&lfEditFontA, sizeof(LOGFONTA) - LF_FACESIZE) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExA(hKey, "FontFace", 0, REG_SZ, (LPBYTE)&lfEditFontA.lfFaceName, lstrlenA(lfEditFontA.lfFaceName) + 1) != ERROR_SUCCESS)
      goto Error;
  }
  if (RegSetValueExA(hKey, "PrintFontEnable", 0, REG_DWORD, (LPBYTE)&bPrintFontEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (bPrintFontChanged)
  {
    if (RegSetValueExA(hKey, "PrintFont", 0, REG_BINARY, (LPBYTE)&lfPrintFontA, sizeof(LOGFONTA) - LF_FACESIZE) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExA(hKey, "PrintFontFace", 0, REG_SZ, (LPBYTE)&lfPrintFontA.lfFaceName, lstrlenA(lfPrintFontA.lfFaceName) + 1) != ERROR_SUCCESS)
      goto Error;
  }
  if (bColorsChanged)
  {
    if (RegSetValueExA(hKey, "Colors", 0, REG_BINARY, (LPBYTE)&aecColors, sizeof(AECOLORS)) != ERROR_SUCCESS)
      goto Error;
  }
  if (RegSetValueExA(hKey, "LanguageModule", 0, REG_SZ, (LPBYTE)szLangModule, lstrlenA(szLangModule) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "ExecuteCommand", 0, REG_SZ, (LPBYTE)szCommand, lstrlenA(szCommand) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "ExecuteDirectory", 0, REG_SZ, (LPBYTE)szWorkDir, lstrlenA(szCommand) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "FileTypesOpen", 0, REG_SZ, (LPBYTE)szFileTypesOpen, lstrlenA(szFileTypesOpen) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "FileTypesEdit", 0, REG_SZ, (LPBYTE)szFileTypesEdit, lstrlenA(szFileTypesEdit) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "FileTypesPrint", 0, REG_SZ, (LPBYTE)szFileTypesPrint, lstrlenA(szFileTypesPrint) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "FileTypesAssociated", 0, REG_DWORD, (LPBYTE)&dwFileTypesAssociated, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "PrintHeaderEnable", 0, REG_DWORD, (LPBYTE)&bPrintHeaderEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "PrintHeader", 0, REG_SZ, (LPBYTE)szPrintHeader, lstrlenA(szPrintHeader) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "PrintFooterEnable", 0, REG_DWORD, (LPBYTE)&bPrintFooterEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "PrintFooter", 0, REG_SZ, (LPBYTE)szPrintFooter, lstrlenA(szPrintFooter) + 1) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExA(hKey, "CodepageList", 0, REG_BINARY, (LPBYTE)lpCodepageList, nCodepageListLen * sizeof(int)) != ERROR_SUCCESS)
    goto Error;

  RegCloseKey(hKey);
  return TRUE;

  Error:
  RegCloseKey(hKey);
  return FALSE;
}

BOOL RegSaveOptionsW()
{
  HKEY hKey;

  dwMainStyle=GetWindowLongW(hMainWnd, GWL_STYLE);
  dwMainStyle=((dwMainStyle & WS_MAXIMIZE) || ((dwMainStyle & WS_MINIMIZE) && dwLastMainSize == SIZE_MAXIMIZED))?WS_MAXIMIZE:0;
  if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
  dwMdiStyle=(bMdiMaximize == TRUE)?WS_MAXIMIZE:0;

  if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Options", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return FALSE;

  if (RegSetValueExW(hKey, L"WordWrap", 0, REG_DWORD, (LPBYTE)&bWordWrap, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"OnTop", 0, REG_DWORD, (LPBYTE)&bOnTop, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"StatusBar", 0, REG_DWORD, (LPBYTE)&bStatusBar, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"ReadOnly", 0, REG_DWORD, (LPBYTE)&bReadOnly, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SaveTime", 0, REG_DWORD, (LPBYTE)&bSaveTime, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"KeepSpace", 0, REG_DWORD, (LPBYTE)&bKeepSpace, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"UndoLimit", 0, REG_DWORD, (LPBYTE)&nUndoLimit, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"DetailedUndo", 0, REG_DWORD, (LPBYTE)&bDetailedUndo, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"CaretOutEdge", 0, REG_DWORD, (LPBYTE)&bCaretOutEdge, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"ReplaceAllAndClose", 0, REG_DWORD, (LPBYTE)&bReplaceAllAndClose, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SaveInReadOnlyMsg", 0, REG_DWORD, (LPBYTE)&bSaveInReadOnlyMsg, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"WatchFile", 0, REG_DWORD, (LPBYTE)&bWatchFile, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SingleOpenFile", 0, REG_DWORD, (LPBYTE)&bSingleOpenFile, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SingleOpenProgram", 0, REG_DWORD, (LPBYTE)&bSingleOpenProgram, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"MDI", 0, REG_DWORD, (LPBYTE)&bRegMDI, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SavePositions", 0, REG_DWORD, (LPBYTE)&bSavePositions, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SaveCodepages", 0, REG_DWORD, (LPBYTE)&bSaveCodepages, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"DefaultCodepage", 0, REG_DWORD, (LPBYTE)&nDefaultCodePage, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"CodepageRecognition", 0, REG_DWORD, (LPBYTE)&dwLangCodepageRecognition, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"CodepageRecognitionBuffer", 0, REG_DWORD, (LPBYTE)&dwCodepageRecognitionBuffer, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"RecentFiles", 0, REG_DWORD, (LPBYTE)&nRecentFiles, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SearchStrings", 0, REG_DWORD, (LPBYTE)&nSearchStrings, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"SearchOptions", 0, REG_DWORD, (LPBYTE)&ftflags, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"TabStopSize", 0, REG_DWORD, (LPBYTE)&nTabStopSize, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"TabStopAsSpaces", 0, REG_DWORD, (LPBYTE)&bTabStopAsSpaces, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"MarginsEdit", 0, REG_DWORD, (LPBYTE)&dwEditMargins, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"MarginsPrint", 0, REG_BINARY, (LPBYTE)&psdPageW.rtMargin, sizeof(RECT)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"WindowStyle", 0, REG_DWORD, (LPBYTE)&dwMainStyle, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"WindowPosition", 0, REG_BINARY, (LPBYTE)&rcMainWindowRestored, sizeof(RECT)) != ERROR_SUCCESS)
    goto Error;

  if (bMDI)
  {
    if (RegSetValueExW(hKey, L"TabViewMDI", 0, REG_DWORD, (LPBYTE)&nTabView, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExW(hKey, L"TabTypeMDI", 0, REG_DWORD, (LPBYTE)&nTabType, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExW(hKey, L"TabSwitchMDI", 0, REG_DWORD, (LPBYTE)&nTabSwitch, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExW(hKey, L"WindowStyleMDI", 0, REG_DWORD, (LPBYTE)&dwMdiStyle, sizeof(DWORD)) != ERROR_SUCCESS)
      goto Error;
  }

  if (RegSetValueExW(hKey, L"WordDelimitersEnable", 0, REG_DWORD, (LPBYTE)&bWordDelimitersEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"WordDelimiters", 0, REG_BINARY, (LPBYTE)wszWordDelimiters, lstrlenW(wszWordDelimiters) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"WordBreak", 0, REG_DWORD, (LPBYTE)&dwCustomWordBreak, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"ShowURL", 0, REG_DWORD, (LPBYTE)&bShowURL, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"ClickURL", 0, REG_DWORD, (LPBYTE)&nClickURL, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"UrlPrefixesEnable", 0, REG_DWORD, (LPBYTE)&bUrlPrefixesEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"UrlPrefixes", 0, REG_BINARY, (LPBYTE)wszUrlPrefixes, lstrlenW(wszUrlPrefixes) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"UrlDelimitersEnable", 0, REG_DWORD, (LPBYTE)&bUrlDelimitersEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"UrlDelimiters", 0, REG_BINARY, (LPBYTE)wszUrlDelimiters, lstrlenW(wszUrlDelimiters) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (bEditFontChanged)
  {
    if (RegSetValueExW(hKey, L"Font", 0, REG_BINARY, (LPBYTE)&lfEditFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExW(hKey, L"FontFace", 0, REG_SZ, (LPBYTE)&lfEditFontW.lfFaceName, lstrlenW(lfEditFontW.lfFaceName) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
      goto Error;
  }
  if (RegSetValueExW(hKey, L"PrintFontEnable", 0, REG_DWORD, (LPBYTE)&bPrintFontEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (bPrintFontChanged)
  {
    if (RegSetValueExW(hKey, L"PrintFont", 0, REG_BINARY, (LPBYTE)&lfPrintFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t)) != ERROR_SUCCESS)
      goto Error;
    if (RegSetValueExW(hKey, L"PrintFontFace", 0, REG_SZ, (LPBYTE)&lfPrintFontW.lfFaceName, lstrlenW(lfPrintFontW.lfFaceName) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
      goto Error;
  }
  if (bColorsChanged)
  {
    if (RegSetValueExW(hKey, L"Colors", 0, REG_BINARY, (LPBYTE)&aecColors, sizeof(AECOLORS)) != ERROR_SUCCESS)
      goto Error;
  }
  if (RegSetValueExW(hKey, L"LanguageModule", 0, REG_SZ, (LPBYTE)wszLangModule, lstrlenW(wszLangModule) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"ExecuteCommand", 0, REG_SZ, (LPBYTE)wszCommand, lstrlenW(wszCommand) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"ExecuteDirectory", 0, REG_SZ, (LPBYTE)wszWorkDir, lstrlenW(wszWorkDir) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"FileTypesOpen", 0, REG_SZ, (LPBYTE)wszFileTypesOpen, lstrlenW(wszFileTypesOpen) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"FileTypesEdit", 0, REG_SZ, (LPBYTE)wszFileTypesEdit, lstrlenW(wszFileTypesEdit) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"FileTypesPrint", 0, REG_SZ, (LPBYTE)wszFileTypesPrint, lstrlenW(wszFileTypesPrint) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"FileTypesAssociated", 0, REG_DWORD, (LPBYTE)&dwFileTypesAssociated, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"PrintHeaderEnable", 0, REG_DWORD, (LPBYTE)&bPrintHeaderEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"PrintHeader", 0, REG_SZ, (LPBYTE)wszPrintHeader, lstrlenW(wszPrintHeader) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"PrintFooterEnable", 0, REG_DWORD, (LPBYTE)&bPrintFooterEnable, sizeof(DWORD)) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"PrintFooter", 0, REG_SZ, (LPBYTE)wszPrintFooter, lstrlenW(wszPrintFooter) * sizeof(wchar_t) + 2) != ERROR_SUCCESS)
    goto Error;
  if (RegSetValueExW(hKey, L"CodepageList", 0, REG_BINARY, (LPBYTE)lpCodepageList, nCodepageListLen * sizeof(int)) != ERROR_SUCCESS)
    goto Error;

  RegCloseKey(hKey);
  return TRUE;

  Error:
  RegCloseKey(hKey);
  return FALSE;
}

BOOL IniSaveOptionsA()
{
  dwMainStyle=GetWindowLongA(hMainWnd, GWL_STYLE);
  dwMainStyle=((dwMainStyle & WS_MAXIMIZE) || ((dwMainStyle & WS_MINIMIZE) && dwLastMainSize == SIZE_MAXIMIZED))?WS_MAXIMIZE:0;
  if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
  dwMdiStyle=(bMdiMaximize == TRUE)?WS_MAXIMIZE:0;

  if (!CreateIniA(&hIniStack, szIniFile))
    return FALSE;

  if (!IniSetValueA(&hIniStack, "Options", "SaveSettings", INI_DWORD, (LPBYTE)&nSaveSettings, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "WordWrap", INI_DWORD, (LPBYTE)&bWordWrap, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "OnTop", INI_DWORD, (LPBYTE)&bOnTop, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "StatusBar", INI_DWORD, (LPBYTE)&bStatusBar, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "ReadOnly", INI_DWORD, (LPBYTE)&bReadOnly, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SaveTime", INI_DWORD, (LPBYTE)&bSaveTime, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "KeepSpace", INI_DWORD, (LPBYTE)&bKeepSpace, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "UndoLimit", INI_DWORD, (LPBYTE)&nUndoLimit, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "DetailedUndo", INI_DWORD, (LPBYTE)&bDetailedUndo, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "CaretOutEdge", INI_DWORD, (LPBYTE)&bCaretOutEdge, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "ReplaceAllAndClose", INI_DWORD, (LPBYTE)&bReplaceAllAndClose, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SaveInReadOnlyMsg", INI_DWORD, (LPBYTE)&bSaveInReadOnlyMsg, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "WatchFile", INI_DWORD, (LPBYTE)&bWatchFile, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SingleOpenFile", INI_DWORD, (LPBYTE)&bSingleOpenFile, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SingleOpenProgram", INI_DWORD, (LPBYTE)&bSingleOpenProgram, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "MDI", INI_DWORD, (LPBYTE)&bRegMDI, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SavePositions", INI_DWORD, (LPBYTE)&bSavePositions, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SaveCodepages", INI_DWORD, (LPBYTE)&bSaveCodepages, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "DefaultCodepage", INI_DWORD, (LPBYTE)&nDefaultCodePage, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "CodepageRecognition", INI_DWORD, (LPBYTE)&dwLangCodepageRecognition, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "CodepageRecognitionBuffer", INI_DWORD, (LPBYTE)&dwCodepageRecognitionBuffer, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "RecentFiles", INI_DWORD, (LPBYTE)&nRecentFiles, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SearchStrings", INI_DWORD, (LPBYTE)&nSearchStrings, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "SearchOptions", INI_DWORD, (LPBYTE)&ftflags, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "TabStopSize", INI_DWORD, (LPBYTE)&nTabStopSize, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "TabStopAsSpaces", INI_DWORD, (LPBYTE)&bTabStopAsSpaces, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "MarginsEdit", INI_DWORD, (LPBYTE)&dwEditMargins, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "MarginsPrint", INI_BINARY, (LPBYTE)&psdPageA.rtMargin, sizeof(RECT)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "WindowStyle", INI_DWORD, (LPBYTE)&dwMainStyle, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "WindowPosition", INI_BINARY, (LPBYTE)&rcMainWindowRestored, sizeof(RECT)))
    goto Error;

  if (bMDI)
  {
    if (!IniSetValueA(&hIniStack, "Options", "TabViewMDI", INI_DWORD, (LPBYTE)&nTabView, sizeof(DWORD)))
      goto Error;
    if (!IniSetValueA(&hIniStack, "Options", "TabTypeMDI", INI_DWORD, (LPBYTE)&nTabType, sizeof(DWORD)))
      goto Error;
    if (!IniSetValueA(&hIniStack, "Options", "TabSwitchMDI", INI_DWORD, (LPBYTE)&nTabSwitch, sizeof(DWORD)))
      goto Error;
    if (!IniSetValueA(&hIniStack, "Options", "WindowStyleMDI", INI_DWORD, (LPBYTE)&dwMdiStyle, sizeof(DWORD)))
      goto Error;
  }

  if (!IniSetValueA(&hIniStack, "Options", "WordDelimitersEnable", INI_DWORD, (LPBYTE)&bWordDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "WordDelimiters", INI_BINARY, (LPBYTE)wszWordDelimiters, wcslen(wszWordDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "WordBreak", INI_DWORD, (LPBYTE)&dwCustomWordBreak, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "ShowURL", INI_DWORD, (LPBYTE)&bShowURL, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "ClickURL", INI_DWORD, (LPBYTE)&nClickURL, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "UrlPrefixesEnable", INI_DWORD, (LPBYTE)&bUrlPrefixesEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "UrlPrefixes", INI_BINARY, (LPBYTE)wszUrlPrefixes, wcslen(wszUrlPrefixes) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "UrlDelimitersEnable", INI_DWORD, (LPBYTE)&bUrlDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "UrlDelimiters", INI_BINARY, (LPBYTE)wszUrlDelimiters, wcslen(wszUrlDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (bEditFontChanged)
  {
    if (!IniSetValueA(&hIniStack, "Options", "Font", INI_BINARY, (LPBYTE)&lfEditFontA, sizeof(LOGFONTA) - LF_FACESIZE))
      goto Error;
    if (!IniSetValueA(&hIniStack, "Options", "FontFace", INI_STRINGANSI, (LPBYTE)&lfEditFontA.lfFaceName, lstrlenA(lfEditFontA.lfFaceName) + 1))
      goto Error;
  }
  if (!IniSetValueA(&hIniStack, "Options", "PrintFontEnable", INI_DWORD, (LPBYTE)&bPrintFontEnable, sizeof(DWORD)))
    goto Error;
  if (bPrintFontChanged)
  {
    if (!IniSetValueA(&hIniStack, "Options", "PrintFont", INI_BINARY, (LPBYTE)&lfPrintFontA, sizeof(LOGFONTA) - LF_FACESIZE))
      goto Error;
    if (!IniSetValueA(&hIniStack, "Options", "PrintFontFace", INI_STRINGANSI, (LPBYTE)&lfPrintFontA.lfFaceName, lstrlenA(lfPrintFontA.lfFaceName) + 1))
      goto Error;
  }
  if (bColorsChanged)
  {
    if (!IniSetValueA(&hIniStack, "Options", "Colors", INI_BINARY, (LPBYTE)&aecColors, sizeof(AECOLORS)))
      goto Error;
  }
  if (!IniSetValueA(&hIniStack, "Options", "LanguageModule", INI_STRINGANSI, (LPBYTE)szLangModule, lstrlenA(szLangModule) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "ExecuteCommand", INI_STRINGANSI, (LPBYTE)szCommand, lstrlenA(szCommand) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "ExecuteDirectory", INI_STRINGANSI, (LPBYTE)szWorkDir, lstrlenA(szWorkDir) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "FileTypesOpen", INI_STRINGANSI, (LPBYTE)szFileTypesOpen, lstrlenA(szFileTypesOpen) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "FileTypesEdit", INI_STRINGANSI, (LPBYTE)szFileTypesEdit, lstrlenA(szFileTypesEdit) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "FileTypesPrint", INI_STRINGANSI, (LPBYTE)szFileTypesPrint, lstrlenA(szFileTypesPrint) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "FileTypesAssociated", INI_DWORD, (LPBYTE)&dwFileTypesAssociated, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "PrintHeaderEnable", INI_DWORD, (LPBYTE)&bPrintHeaderEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "PrintHeader", INI_STRINGANSI, (LPBYTE)szPrintHeader, lstrlenA(szPrintHeader) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "PrintFooterEnable", INI_DWORD, (LPBYTE)&bPrintFooterEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "PrintFooter", INI_STRINGANSI, (LPBYTE)szPrintFooter, lstrlenA(szPrintFooter) + 1))
    goto Error;
  if (!IniSetValueA(&hIniStack, "Options", "CodepageList", INI_BINARY, (LPBYTE)lpCodepageList, nCodepageListLen * sizeof(int)))
    goto Error;

  if (SaveIniA(&hIniStack, szIniFile))
  {
    StackFreeIni(&hIniStack);
    return TRUE;
  }
  Error:
  StackFreeIni(&hIniStack);
  return FALSE;
}

BOOL IniSaveOptionsW()
{
  dwMainStyle=GetWindowLongW(hMainWnd, GWL_STYLE);
  dwMainStyle=((dwMainStyle & WS_MAXIMIZE) || ((dwMainStyle & WS_MINIMIZE) && dwLastMainSize == SIZE_MAXIMIZED))?WS_MAXIMIZE:0;
  if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
  dwMdiStyle=(bMdiMaximize == TRUE)?WS_MAXIMIZE:0;

  if (!CreateIniW(&hIniStack, wszIniFile))
    return FALSE;

  if (!IniSetValueW(&hIniStack, L"Options", L"SaveSettings", INI_DWORD, (LPBYTE)&nSaveSettings, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"WordWrap", INI_DWORD, (LPBYTE)&bWordWrap, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"OnTop", INI_DWORD, (LPBYTE)&bOnTop, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"StatusBar", INI_DWORD, (LPBYTE)&bStatusBar, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"ReadOnly", INI_DWORD, (LPBYTE)&bReadOnly, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SaveTime", INI_DWORD, (LPBYTE)&bSaveTime, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"KeepSpace", INI_DWORD, (LPBYTE)&bKeepSpace, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"UndoLimit", INI_DWORD, (LPBYTE)&nUndoLimit, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"DetailedUndo", INI_DWORD, (LPBYTE)&bDetailedUndo, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"CaretOutEdge", INI_DWORD, (LPBYTE)&bCaretOutEdge, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"ReplaceAllAndClose", INI_DWORD, (LPBYTE)&bReplaceAllAndClose, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SaveInReadOnlyMsg", INI_DWORD, (LPBYTE)&bSaveInReadOnlyMsg, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"WatchFile", INI_DWORD, (LPBYTE)&bWatchFile, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SingleOpenFile", INI_DWORD, (LPBYTE)&bSingleOpenFile, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SingleOpenProgram", INI_DWORD, (LPBYTE)&bSingleOpenProgram, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"MDI", INI_DWORD, (LPBYTE)&bRegMDI, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SavePositions", INI_DWORD, (LPBYTE)&bSavePositions, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SaveCodepages", INI_DWORD, (LPBYTE)&bSaveCodepages, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"DefaultCodepage", INI_DWORD, (LPBYTE)&nDefaultCodePage, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"CodepageRecognition", INI_DWORD, (LPBYTE)&dwLangCodepageRecognition, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"CodepageRecognitionBuffer", INI_DWORD, (LPBYTE)&dwCodepageRecognitionBuffer, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"RecentFiles", INI_DWORD, (LPBYTE)&nRecentFiles, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SearchStrings", INI_DWORD, (LPBYTE)&nSearchStrings, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"SearchOptions", INI_DWORD, (LPBYTE)&ftflags, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"TabStopSize", INI_DWORD, (LPBYTE)&nTabStopSize, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"TabStopAsSpaces", INI_DWORD, (LPBYTE)&bTabStopAsSpaces, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"MarginsEdit", INI_DWORD, (LPBYTE)&dwEditMargins, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"MarginsPrint", INI_BINARY, (LPBYTE)&psdPageW.rtMargin, sizeof(RECT)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"WindowStyle", INI_DWORD, (LPBYTE)&dwMainStyle, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"WindowPosition", INI_BINARY, (LPBYTE)&rcMainWindowRestored, sizeof(RECT)))
    goto Error;

  if (bMDI)
  {
    if (!IniSetValueW(&hIniStack, L"Options", L"TabViewMDI", INI_DWORD, (LPBYTE)&nTabView, sizeof(DWORD)))
      goto Error;
    if (!IniSetValueW(&hIniStack, L"Options", L"TabTypeMDI", INI_DWORD, (LPBYTE)&nTabType, sizeof(DWORD)))
      goto Error;
    if (!IniSetValueW(&hIniStack, L"Options", L"TabSwitchMDI", INI_DWORD, (LPBYTE)&nTabSwitch, sizeof(DWORD)))
      goto Error;
    if (!IniSetValueW(&hIniStack, L"Options", L"WindowStyleMDI", INI_DWORD, (LPBYTE)&dwMdiStyle, sizeof(DWORD)))
      goto Error;
  }

  if (!IniSetValueW(&hIniStack, L"Options", L"WordDelimitersEnable", INI_DWORD, (LPBYTE)&bWordDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"WordDelimiters", INI_BINARY, (LPBYTE)wszWordDelimiters, lstrlenW(wszWordDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"WordBreak", INI_DWORD, (LPBYTE)&dwCustomWordBreak, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"ShowURL", INI_DWORD, (LPBYTE)&bShowURL, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"ClickURL", INI_DWORD, (LPBYTE)&nClickURL, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"UrlPrefixesEnable", INI_DWORD, (LPBYTE)&bUrlPrefixesEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"UrlPrefixes", INI_BINARY, (LPBYTE)wszUrlPrefixes, lstrlenW(wszUrlPrefixes) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"UrlDelimitersEnable", INI_DWORD, (LPBYTE)&bUrlDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"UrlDelimiters", INI_BINARY, (LPBYTE)wszUrlDelimiters, lstrlenW(wszUrlDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (bEditFontChanged)
  {
    if (!IniSetValueW(&hIniStack, L"Options", L"Font", INI_BINARY, (LPBYTE)&lfEditFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t)))
      goto Error;
    if (!IniSetValueW(&hIniStack, L"Options", L"FontFace", INI_STRINGUNICODE, (LPBYTE)&lfEditFontW.lfFaceName, lstrlenW(lfEditFontW.lfFaceName) * sizeof(wchar_t) + 2))
      goto Error;
  }
  if (!IniSetValueW(&hIniStack, L"Options", L"PrintFontEnable", INI_DWORD, (LPBYTE)&bPrintFontEnable, sizeof(DWORD)))
    goto Error;
  if (bPrintFontChanged)
  {
    if (!IniSetValueW(&hIniStack, L"Options", L"PrintFont", INI_BINARY, (LPBYTE)&lfPrintFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t)))
      goto Error;
    if (!IniSetValueW(&hIniStack, L"Options", L"PrintFontFace", INI_STRINGUNICODE, (LPBYTE)&lfPrintFontW.lfFaceName, lstrlenW(lfPrintFontW.lfFaceName) * sizeof(wchar_t) + 2))
      goto Error;
  }
  if (bColorsChanged)
  {
    if (!IniSetValueW(&hIniStack, L"Options", L"Colors", INI_BINARY, (LPBYTE)&aecColors, sizeof(AECOLORS)))
      goto Error;
  }
  if (!IniSetValueW(&hIniStack, L"Options", L"LanguageModule", INI_STRINGUNICODE, (LPBYTE)wszLangModule, lstrlenW(wszLangModule) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"ExecuteCommand", INI_STRINGUNICODE, (LPBYTE)wszCommand, lstrlenW(wszCommand) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"ExecuteDirectory", INI_STRINGUNICODE, (LPBYTE)wszWorkDir, lstrlenW(wszWorkDir) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"FileTypesOpen", INI_STRINGUNICODE, (LPBYTE)wszFileTypesOpen, lstrlenW(wszFileTypesOpen) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"FileTypesEdit", INI_STRINGUNICODE, (LPBYTE)wszFileTypesEdit, lstrlenW(wszFileTypesEdit) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"FileTypesPrint", INI_STRINGUNICODE, (LPBYTE)wszFileTypesPrint, lstrlenW(wszFileTypesPrint) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"FileTypesAssociated", INI_DWORD, (LPBYTE)&dwFileTypesAssociated, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"PrintHeaderEnable", INI_DWORD, (LPBYTE)&bPrintHeaderEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"PrintHeader", INI_STRINGUNICODE, (LPBYTE)wszPrintHeader, lstrlenW(wszPrintHeader) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"PrintFooterEnable", INI_DWORD, (LPBYTE)&bPrintFooterEnable, sizeof(DWORD)))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"PrintFooter", INI_STRINGUNICODE, (LPBYTE)wszPrintFooter, lstrlenW(wszPrintFooter) * sizeof(wchar_t) + 2))
    goto Error;
  if (!IniSetValueW(&hIniStack, L"Options", L"CodepageList", INI_BINARY, (LPBYTE)lpCodepageList, nCodepageListLen * sizeof(int)))
    goto Error;

  if (SaveIniW(&hIniStack, wszIniFile))
  {
    StackFreeIni(&hIniStack);
    return TRUE;
  }
  Error:
  StackFreeIni(&hIniStack);
  return FALSE;
}

void ReadThemesA()
{
  AECOLORS aec;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSizeValue;
  DWORD dwSizeString;
  DWORD dwIndex=0;

  //Standard theme
  API_LoadStringA(hLangLib, STR_STANDARDTHEME, buf, BUFFER_SIZE);
  aec.crCaret=RGB(0x00, 0x00, 0x00);
  aec.crBasicText=GetSysColor(COLOR_WINDOWTEXT);
  aec.crBasicBk=GetSysColor(COLOR_WINDOW);
  aec.crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
  aec.crSelBk=GetSysColor(COLOR_HIGHLIGHT);
  aec.crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
  aec.crActiveLineBk=GetSysColor(COLOR_WINDOW);
  aec.crUrlText=RGB(0x00, 0x00, 0xFF);
  StackThemeAddA(&hThemesStack, buf, &aec);

  if (nSaveSettings == SS_REGISTRY)
  {
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Themes", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      while (1)
      {
        dwSizeValue=BUFFER_SIZE;
        dwSizeString=sizeof(AECOLORS);
        if (RegEnumValueA(hKey, dwIndex, buf, &dwSizeValue, NULL, &dwType, (LPBYTE)&aec, &dwSizeString) != ERROR_SUCCESS)
          break;

        StackThemeAddA(&hThemesStack, buf, &aec);
        ++dwIndex;
      }
      RegCloseKey(hKey);
    }
  }
  else
  {
    HINISECTION *lpIniSection;
    HINIKEY *lpIniKey;

    if (lpIniSection=StackGetIniSectionA(&hIniStack, "Themes", lstrlenA("Themes")))
    {
      lpIniKey=(HINIKEY *)lpIniSection->hSectionStack.last;

      while (lpIniKey)
      {
        IniGetValueA(&hIniStack, "Themes", lpIniKey->szKey, INI_BINARY, (LPBYTE)&aec, sizeof(AECOLORS));
        StackThemeAddA(&hThemesStack, lpIniKey->szKey, &aec);
        ++dwIndex;

        lpIniKey=lpIniKey->prev;
      }
    }
  }

  if (!dwIndex)
  {
    //Notepad++ theme
    aec.crCaret=RGB(0x80, 0x00, 0xFF);
    aec.crBasicText=RGB(0x00, 0x00, 0x00);
    aec.crBasicBk=RGB(0xFF, 0xFF, 0xFF);
    aec.crSelText=RGB(0x00, 0x00, 0x00);
    aec.crSelBk=RGB(0xC0, 0xC0, 0xC0);
    aec.crActiveLineText=RGB(0x00, 0x00, 0x00);
    aec.crActiveLineBk=RGB(0xE8, 0xE8, 0xFF);
    aec.crUrlText=RGB(0x00, 0x00, 0xFF);
    StackThemeAddA(&hThemesStack, "Notepad++", &aec);
  }
}

void ReadThemesW()
{
  AECOLORS aec;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSizeValue;
  DWORD dwSizeString;
  DWORD dwIndex=0;

  //Standard theme
  API_LoadStringW(hLangLib, STR_STANDARDTHEME, wbuf, BUFFER_SIZE);
  aec.crCaret=RGB(0x00, 0x00, 0x00);
  aec.crBasicText=GetSysColor(COLOR_WINDOWTEXT);
  aec.crBasicBk=GetSysColor(COLOR_WINDOW);
  aec.crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
  aec.crSelBk=GetSysColor(COLOR_HIGHLIGHT);
  aec.crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
  aec.crActiveLineBk=GetSysColor(COLOR_WINDOW);
  aec.crUrlText=RGB(0x00, 0x00, 0xFF);
  StackThemeAddW(&hThemesStack, wbuf, &aec);

  if (nSaveSettings == SS_REGISTRY)
  {
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Themes", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      while (1)
      {
        dwSizeValue=BUFFER_SIZE;
        dwSizeString=sizeof(AECOLORS);
        if (RegEnumValueW(hKey, dwIndex, wbuf, &dwSizeValue, NULL, &dwType, (LPBYTE)&aec, &dwSizeString) != ERROR_SUCCESS)
          break;

        StackThemeAddW(&hThemesStack, wbuf, &aec);
        ++dwIndex;
      }
      RegCloseKey(hKey);
    }
  }
  else
  {
    HINISECTION *lpIniSection;
    HINIKEY *lpIniKey;

    if (lpIniSection=StackGetIniSectionW(&hIniStack, L"Themes", lstrlenW(L"Themes")))
    {
      lpIniKey=(HINIKEY *)lpIniSection->hSectionStack.last;

      while (lpIniKey)
      {
        IniGetValueW(&hIniStack, L"Themes", lpIniKey->wszKey, INI_BINARY, (LPBYTE)&aec, sizeof(AECOLORS));
        StackThemeAddW(&hThemesStack, lpIniKey->wszKey, &aec);
        ++dwIndex;

        lpIniKey=lpIniKey->prev;
      }
    }
  }

  if (!dwIndex)
  {
    //Notepad++ theme
    aec.crCaret=RGB(0x80, 0x00, 0xFF);
    aec.crBasicText=RGB(0x00, 0x00, 0x00);
    aec.crBasicBk=RGB(0xFF, 0xFF, 0xFF);
    aec.crSelText=RGB(0x00, 0x00, 0x00);
    aec.crSelBk=RGB(0xC0, 0xC0, 0xC0);
    aec.crActiveLineText=RGB(0x00, 0x00, 0x00);
    aec.crActiveLineBk=RGB(0xE8, 0xE8, 0xFF);
    aec.crUrlText=RGB(0x00, 0x00, 0xFF);
    StackThemeAddW(&hThemesStack, L"Notepad++", &aec);
  }
}

BOOL SaveThemesA(BOOL bCleanOld)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hThemesStack.last;
  HKEY hKey;
  HINISECTION *lpIniSection;
  BOOL bResult=FALSE;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (bCleanOld)
      RegClearKeyA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Themes");

    if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Akelsoft\\AkelPad\\Themes", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!CreateIniA(&hIniStack, szIniFile))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionA(&hIniStack, "Themes", lstrlenA("Themes")))
        StackFreeIniSection(&lpIniSection->hSectionStack);
    }
  }

  //Skip "Standard" theme
  if (ctElement) ctElement=ctElement->prev;

  while (ctElement)
  {
    if (nSaveSettings == SS_REGISTRY)
    {
      if (RegSetValueExA(hKey, ctElement->szName, 0, REG_BINARY, (LPBYTE)&ctElement->aec, sizeof(AECOLORS)) != ERROR_SUCCESS)
        break;
    }
    else
    {
      if (!IniSetValueA(&hIniStack, "Themes", ctElement->szName, INI_BINARY, (LPBYTE)&ctElement->aec, sizeof(AECOLORS)))
        break;
    }
    ctElement=ctElement->prev;
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

BOOL SaveThemesW(BOOL bCleanOld)
{
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hThemesStack.last;
  HKEY hKey;
  HINISECTION *lpIniSection;
  BOOL bResult=FALSE;

  if (nSaveSettings == SS_REGISTRY)
  {
    if (bCleanOld)
      RegClearKeyW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Themes");

    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Akelsoft\\AkelPad\\Themes", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!CreateIniW(&hIniStack, wszIniFile))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionW(&hIniStack, L"Themes", lstrlenW(L"Themes")))
        StackFreeIniSection(&lpIniSection->hSectionStack);
    }
  }

  //Skip "Standard" theme
  if (ctElement) ctElement=ctElement->prev;

  while (ctElement)
  {
    if (nSaveSettings == SS_REGISTRY)
    {
      if (RegSetValueExW(hKey, ctElement->wszName, 0, REG_BINARY, (LPBYTE)&ctElement->aec, sizeof(AECOLORS)) != ERROR_SUCCESS)
        break;
    }
    else
    {
      if (!IniSetValueW(&hIniStack, L"Themes", ctElement->wszName, INI_BINARY, (LPBYTE)&ctElement->aec, sizeof(AECOLORS)))
        break;
    }
    ctElement=ctElement->prev;
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
  nodA.szFile=szFile;
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

  if (!(bFileExist=GetFullNameA(szFile, MAX_PATH)))
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
      if (!bMDI && !bMdiReopen && bSingleOpenFile && lstrcmpiA(szFile, szCurrentFile))
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
      if (bMDI && !bMdiReopen && bSingleOpenFile)
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
            bMdiReopen=TRUE;
            OpenDocumentA(hWnd, szFile, dwFlags, nCodePage, bBOM);
            bMdiReopen=FALSE;
          }
          nResult=EOD_WINDOW_EXIST;
          goto End;
        }
      }
    }

    //Autodetect code page
    if ((nDetect=AutodetectCodePageA(szFile, dwCodepageRecognitionBuffer, dwFlags, &nCodePage, &bBOM)) < 0)
    {
      if (!bMdiReopen)
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
  if (!IsCodePageValid(nCodePage))
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
      RecentFilesUpdateA(szCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
      RecentFilesSaveA();
    }

    if (bMDI && !bMdiReopen && (!hWndFrameActive || bModified || szCurrentFile[0]))
    {
      if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
      CreateMDIWindowA(APP_MDI_CLASSA, "", (bMdiMaximize == TRUE)?WS_MAXIMIZE:0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMdiClient, hInstance, 0);
      hWnd=hWndEdit;
    }

    //Get file write time
    GetFileTime(hFile, NULL, NULL, &ftFileTime);

    HideCaret(NULL);
  }

  //Load text
  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.nNewLine=NEWLINE_WIN;
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
      SetNewLineStatusA(hWndEdit, fsd.nNewLine, AENL_INPUT, TRUE);
      SetModifyStatusA(NULL, FALSE, FALSE);
      SetCodePageStatusA(nCodePage, bBOM, FALSE);

      if (nFileCmp)
      {
        UpdateTitleA(GetParent(hWndEdit), szFile);
        lstrcpynA(szCurrentFile, szFile, MAX_PATH);
      }

      //Update selection
      if (nRecentFiles && bSavePositions)
      {
        CHARRANGE cr;

        cr.cpMin=lpdwRecentPositions[0];
        cr.cpMax=lpdwRecentPositions[0];
        SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
      }

      //Print if "/p" option used in command line
      if (bGlobalPrint)
      {
        DoFilePrintA(TRUE);
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
  nodW.wszFile=wszFile;
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
  if (!(bFileExist=GetFullNameW(wszFile, MAX_PATH)))
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
      if (!bMDI && !bMdiReopen && bSingleOpenFile && lstrcmpiW(wszFile, wszCurrentFile))
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
      if (bMDI && !bMdiReopen && bSingleOpenFile)
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
            bMdiReopen=TRUE;
            OpenDocumentW(hWnd, wszFile, dwFlags, nCodePage, bBOM);
            bMdiReopen=FALSE;
          }
          nResult=EOD_WINDOW_EXIST;
          goto End;
        }
      }
    }

    //Autodetect code page
    if ((nDetect=AutodetectCodePageW(wszFile, dwCodepageRecognitionBuffer, dwFlags, &nCodePage, &bBOM)) < 0)
    {
      if (!bMdiReopen)
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
  if (!IsCodePageValid(nCodePage))
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
      RecentFilesUpdateW(wszCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
      RecentFilesSaveW();
    }

    if (bMDI && !bMdiReopen && (!hWndFrameActive || bModified || wszCurrentFile[0]))
    {
      if (hWndFrameActive) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
      CreateMDIWindowW(APP_MDI_CLASSW, L"", (bMdiMaximize == TRUE)?WS_MAXIMIZE:0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hMdiClient, hInstance, 0);
      hWnd=hWndEdit;
    }

    //Get file write time
    GetFileTime(hFile, NULL, NULL, &ftFileTime);

    HideCaret(NULL);
  }

  //Load text
  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.nNewLine=NEWLINE_WIN;
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
      SetNewLineStatusW(hWndEdit, fsd.nNewLine, AENL_INPUT, TRUE);
      SetModifyStatusW(NULL, FALSE, FALSE);
      SetCodePageStatusW(nCodePage, bBOM, FALSE);

      if (nFileCmp)
      {
        UpdateTitleW(GetParent(hWndEdit), wszFile);
        lstrcpynW(wszCurrentFile, wszFile, MAX_PATH);
      }

      //Update selection
      if (nRecentFiles && bSavePositions)
      {
        CHARRANGE cr;

        cr.cpMin=lpdwRecentPositions[0];
        cr.cpMax=lpdwRecentPositions[0];
        SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
      }

      //Print if "/p" option used in command line
      if (bGlobalPrint)
      {
        DoFilePrintW(TRUE);
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
            dwCharsConverted=UTF8toUTF16((char *)pBuffer, dwBytesRead, NULL, wpBuffer, dwBytesRead);
          else
            dwCharsConverted=MultiByteToWideChar(lpData->nCodePage, 0, (char *)pBuffer, dwBytesRead, wpBuffer, dwBytesRead);
        }
        wpBuffer[dwCharsConverted]='\0';

        //Detect new line
        for (i=0; i < dwCharsConverted; ++i)
        {
          if (wpBuffer[i] == '\r')
          {
            if (wpBuffer[i + 1] == '\r' && wpBuffer[i + 2] == '\n')
            {
              //Windows format \r\r\n
              lpData->nNewLine=NEWLINE_WIN;
              break;
            }
            else if (wpBuffer[i + 1] == '\n')
            {
              //Windows format \r\n
              lpData->nNewLine=NEWLINE_WIN;
              break;
            }

            //MacOS format \r
            lpData->nNewLine=NEWLINE_MAC;
            break;
          }
          else if (wpBuffer[i] == '\n')
          {
            //Unix format \n
            lpData->nNewLine=NEWLINE_UNIX;
            break;
          }
        }

        //Send to AkelEdit
        SendMessage(lpData->hWnd, AEM_SETNEWLINE, AENL_INPUT|AENL_OUTPUT, MAKELONG(AELB_ASIS, AELB_ASIS));
        SendMessage(lpData->hWnd, AEM_SETTEXTW, (LPARAM)dwCharsConverted, (WPARAM)wpBuffer);
      }
      else lpData->bResult=FALSE;

      API_HeapFree(hHeap, 0, (LPVOID)wpBuffer);
    }
    else lpData->bResult=FALSE;
  }
  else lpData->bResult=FALSE;
}

int SaveDocumentA(HWND hWnd, char *szFile, int nCodePage, BOOL bBOM, BOOL bUpdate)
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
  int nLine;

  //Notification message
  nsd.szFile=szFile;
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
  if (!IsCodePageValid(nCodePage))
  {
    API_LoadStringA(hLangLib, MSG_CP_UNIMPLEMENTED, buf, BUFFER_SIZE);
    wsprintfA(buf2, buf, nCodePage);
    MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
    nResult=ESD_CODEPAGE_ERROR;
    goto End;
  }
  if (nCodePage != CP_UNICODE_UCS2_LE && nCodePage != CP_UNICODE_UCS2_BE && nCodePage != CP_UNICODE_UTF8)
  {
    if (nLine=SendMessage(hWnd, AEM_CHECKCODEPAGE, (WPARAM)nCodePage, 0))
    {
      API_LoadStringA(hLangLib, MSG_CP_MISMATCH, buf, BUFFER_SIZE);
      wsprintfA(buf2, buf, nLine);
      if (MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDCANCEL)
      {
        nResult=ESD_CODEPAGE_ERROR;
        goto End;
      }
    }
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
    if (bUpdate)
    {
      //Compare paths
      nFileCmp=lstrcmpiA(szCurrentFile, szFile);

      if (nFileCmp || nCurrentCodePage != nCodePage)
      {
        //Save position of the document
        if (nRecentFiles)
        {
          RecentFilesZeroA();
          RecentFilesReadA();
          RecentFilesUpdateA(szFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCodePage);
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
            SetModifyStatusA(hWnd, FALSE, FALSE);
            wf->ei.nCodePage=nCodePage;
            wf->ei.bBOM=bBOM;
            wf->ft=ft;

            if (lstrcmpiA(wf->szFile, szFile))
            {
              UpdateTitleA(hWndFrame, szFile);
              lstrcpynA(wf->szFile, szFile, MAX_PATH);
            }
          }
        }
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

int SaveDocumentW(HWND hWnd, wchar_t *wszFile, int nCodePage, BOOL bBOM, BOOL bUpdate)
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
  int nLine;

  //Notification message
  nsdW.wszFile=wszFile;
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
  if (!IsCodePageValid(nCodePage))
  {
    API_LoadStringW(hLangLib, MSG_CP_UNIMPLEMENTED, wbuf, BUFFER_SIZE);
    wsprintfW(wbuf2, wbuf, nCodePage);
    MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    nResult=ESD_CODEPAGE_ERROR;
    goto End;
  }
  if (nCodePage != CP_UNICODE_UCS2_LE && nCodePage != CP_UNICODE_UCS2_BE && nCodePage != CP_UNICODE_UTF8)
  {
    if (nLine=SendMessage(hWnd, AEM_CHECKCODEPAGE, (WPARAM)nCodePage, 0))
    {
      API_LoadStringW(hLangLib, MSG_CP_MISMATCH, wbuf, BUFFER_SIZE);
      wsprintfW(wbuf2, wbuf, nLine);
      if (MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDCANCEL)
      {
        nResult=ESD_CODEPAGE_ERROR;
        goto End;
      }
    }
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
    if (bUpdate)
    {
      //Compare paths
      nFileCmp=lstrcmpiW(wszCurrentFile, wszFile);

      if (nFileCmp || nCurrentCodePage != nCodePage)
      {
        //Save position of the document
        if (nRecentFiles)
        {
          RecentFilesZeroW();
          RecentFilesReadW();
          RecentFilesUpdateW(wszFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCodePage);
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
            SetModifyStatusW(hWnd, FALSE, FALSE);
            wf->ei.nCodePage=nCodePage;
            wf->ei.bBOM=bBOM;
            wf->ft=ft;

            if (lstrcmpiW(wf->wszFile, wszFile))
            {
              UpdateTitleW(hWndFrame, wszFile);
              lstrcpynW(wf->wszFile, wszFile, MAX_PATH);
            }
          }
        }
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
  AESTREAM aes;

  aes.dwCookie=(DWORD)lpData;
  aes.lpCallback=OutputStreamCallback;
  aes.bColumnSel=FALSE;
  aes.nNewLine=AELB_ASOUTPUT;
  SendMessage(lpData->hWnd, AEM_STREAMOUT, 0, (LPARAM)&aes);
  lpData->bResult=!aes.dwError;
}

DWORD CALLBACK OutputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufDone)
{
  FILESTREAMDATA *lpData=(FILESTREAMDATA *)dwCookie;
  unsigned char *pDataToWrite=(unsigned char *)wszBuf;
  DWORD dwBytesToWrite=dwBufLen;

  dwBufLen=dwBufLen / sizeof(wchar_t);

  if (lpData->nCodePage == CP_UNICODE_UCS2_LE)
  {
  }
  else if (lpData->nCodePage == CP_UNICODE_UCS2_BE)
  {
    ChangeByteOrder((unsigned char *)wszBuf, dwBytesToWrite);
  }
  else
  {
    if (lpData->nCodePage == CP_UNICODE_UTF8)
      dwBytesToWrite=UTF16toUTF8(wszBuf, dwBufLen, (char *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE);
    else
      dwBytesToWrite=WideCharToMultiByte(lpData->nCodePage, 0, wszBuf, dwBufLen, (char *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE, NULL, NULL);

    pDataToWrite=pcTranslateBuffer;
  }
  return !API_WriteFile(lpData->hFile, pDataToWrite, dwBytesToWrite, dwBufDone, NULL);
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

unsigned int CALLBACK CodePageDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    SendMessage(hWndPreview, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
    SetTabStops(hWndPreview, nTabStopSize, FALSE);
    SetChosenFontA(hWndPreview, &lfEditFontA, FALSE);

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
      AECHARRANGE cr;

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
      GetSel((HWND)wParam, &cr, NULL, NULL);
      EnableMenuItem(hPopupView, IDM_EDIT_COPY, AEC_IndexCompare(&cr.ciMin, &cr.ciMax)?MF_ENABLED:MF_GRAYED);
      i=TrackPopupMenu(hPopupView, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);

      if (i == IDM_EDIT_COPY)
        SendMessage((HWND)wParam, WM_COPY, 0, 0);
      else if (i == IDM_EDIT_SELECTALL)
        DoEditSelectAll((HWND)wParam);
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
      if (!IsCodePageValid(nOfnCodePage))
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
  else if (uMsg == WM_DESTROY)
  {
    HFONT hFont;

    if (hFont=(HFONT)SendMessage(hWndPreview, WM_GETFONT, 0, 0))
      DeleteObject(hFont);
  }

  return FALSE;
}

unsigned int CALLBACK CodePageDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    SendMessage(hWndPreview, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
    SetTabStops(hWndPreview, nTabStopSize, FALSE);
    SetChosenFontW(hWndPreview, &lfEditFontW, FALSE);

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
      AECHARRANGE cr;

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
      GetSel((HWND)wParam, &cr, NULL, NULL);
      EnableMenuItem(hPopupView, IDM_EDIT_COPY, AEC_IndexCompare(&cr.ciMin, &cr.ciMax)?MF_ENABLED:MF_GRAYED);
      i=TrackPopupMenu(hPopupView, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);

      if (i == IDM_EDIT_COPY)
        SendMessage((HWND)wParam, WM_COPY, 0, 0);
      else if (i == IDM_EDIT_SELECTALL)
        DoEditSelectAll((HWND)wParam);
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
      if (!IsCodePageValid(nOfnCodePage))
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
  else if (uMsg == WM_DESTROY)
  {
    HFONT hFont;

    if (hFont=(HFONT)SendMessage(hWndPreview, WM_GETFONT, 0, 0))
      DeleteObject(hFont);
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
      if (!AutodetectMultibyte(dwLangCodepageRecognition, pBuffer, dwBytesRead, nCodePage))
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
      if (!AutodetectMultibyte(dwLangCodepageRecognition, pBuffer, dwBytesRead, nCodePage))
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
  if (dwLangID == LANGID_ENGLISH)
  {
    char szOEMwatermark[]="\xBA\xB3\xB0\xB1\xB2\xDB";
    char szUTF8watermark[]="\xD0\xD1";
    int nANSIrate=5;
    int nOEMrate=0;
    int nUTF8rate=0;
    DWORD dwCounter[0x80];
    DWORD dwMaxIndex=0;
    DWORD dwMaxCount=0;
    DWORD i;
    DWORD j;

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

        if (strchr(szOEMwatermark, dwMaxIndex + 0x80)) nOEMrate+=dwCounter[dwMaxIndex];
        if (strchr(szUTF8watermark, dwMaxIndex + 0x80)) nUTF8rate+=dwCounter[dwMaxIndex];
        dwCounter[dwMaxIndex]=0;
      }

      if (nANSIrate >= nOEMrate && nANSIrate >= nUTF8rate)
        *nCodePage=nAnsiCodePage;
      else if (nOEMrate >= nUTF8rate)
        *nCodePage=nOemCodePage;
      else
        *nCodePage=CP_UNICODE_UTF8;
      return TRUE;
    }
  }
  else if (dwLangID == LANGID_RUSSIAN)
  {
    char szANSIwatermark[]="\xE0\xE1\xE2\xE5\xE8\xED\xEE\xEF\xF0\xF2\xC0\xC1\xC2\xC5\xC8\xCD\xCE\xCF\xD2"; //‡·‚ÂËÌÓÔÚ¿¡¬≈»ÕŒœ“
    char szOEMwatermark[]="\xAE\xA5\xA0\xA8\xAD\xE2\x8E\x45\x80\x88\x8D\x92\xBA\xB3\xB0\xB1\xB2\xDB";      //Character graphics simbols: \xBA\xB3\xB0\xB1\xB2\xDB
    char szKOIwatermark[]="\xC1\xC2\xD7\xC5\xC9\xCE\xCF\xD2\xD4\xE1\xE2\xF7\xE5\xE9\xEE\xEF\xF0\xF2\xF4";  //¡¬◊≈…Œœ“‘·‚˜ÂÈÓÔÚÙ
    char szUTF8watermark[]="\xD0\xD1";
    int nANSIrate=5;
    int nOEMrate=0;
    int nKOIrate=0;
    int nUTF8rate=0;
    DWORD dwCounter[0x80];
    DWORD dwMaxIndex=0;
    DWORD dwMaxCount=0;
    DWORD i;
    DWORD j;

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

      if (nANSIrate >= nOEMrate && nANSIrate >= nKOIrate && nANSIrate >= nUTF8rate)
        *nCodePage=1251;
      else if (nOEMrate >= nKOIrate && nOEMrate >= nUTF8rate)
        *nCodePage=866;
      else if (nKOIrate >= nUTF8rate)
        *nCodePage=CP_KOI8_R;
      else
        *nCodePage=CP_UNICODE_UTF8;
      return TRUE;
    }
  }
  return FALSE;
}

unsigned int UTF8toUTF16(const char *pMultiString, unsigned int nMultiString, unsigned int *nMultiStringRemain,  wchar_t *wszWideString, unsigned int nWideString)
{
  const unsigned char *lpMultiString=(const unsigned char *)pMultiString;
  unsigned int i=0;
  unsigned int ti=0;

  while (i < nMultiString && ti < nWideString)
  {
    if (lpMultiString[i] < 0x80)
    {
      wszWideString[ti]=lpMultiString[i++];
    }
    else if (lpMultiString[i] < 0xE0)
    {
      wszWideString[ti]=(lpMultiString[i++] & 0x1F) << 6;
      if (i >= nMultiString || !lpMultiString[i]) {nMultiString=i - 1; return ti;}
      wszWideString[ti]=wszWideString[ti] + (lpMultiString[i++] & 0x7F);
    }
    else if (lpMultiString[i] < 0xF0)
    {
      wszWideString[ti]=(lpMultiString[i++] & 0xF) << 12;
      if (i >= nMultiString || !lpMultiString[i]) {nMultiString=i - 1; return ti;}
      wszWideString[ti]=wszWideString[ti] + ((lpMultiString[i++] & 0x7F) << 6);
      if (i >= nMultiString || !lpMultiString[i]) {nMultiString=i - 2; return ti;}
      wszWideString[ti]=wszWideString[ti] + (lpMultiString[i++] & 0x7F);
    }
    else {++i; continue;}

    ++ti;
  }
  if (i < nMultiString && ti < nWideString)
    wszWideString[ti++]='\0';
  if (nMultiStringRemain) *nMultiStringRemain=nMultiString - i;
  return ti;
}

unsigned int UTF16toUTF8(const wchar_t *wpWideString, unsigned int nWideString, char *szMultiString, unsigned int nMultiString)
{
  unsigned char *lpMultiString=(unsigned char *)szMultiString;
  unsigned int i=0;
  unsigned int ti=0;

  while (ti < nWideString)
  {
    if (wpWideString[ti] < 0x80)
    {
      if (i + 1 > nMultiString) return i;
      lpMultiString[i++]=(unsigned char)wpWideString[ti];
    }
    else if (wpWideString[ti] < 0x800)
    {
      if (i + 2 > nMultiString) return i;
      lpMultiString[i++]=0xC0|((wpWideString[ti] >> 6) & 0x1F);
      lpMultiString[i++]=0x80|(wpWideString[ti] & 0x3F);
    }
    else
    {
      //wpWideString[ti] < 0x10000
      if (i + 3 > nMultiString) return i;
      lpMultiString[i++]=0xE0|((wpWideString[ti] >> 12) & 0xF);
      lpMultiString[i++]=0x80|((wpWideString[ti] >> 6) & 0x3F);
      lpMultiString[i++]=0x80|(wpWideString[ti] & 0x3F);
    }
    ++ti;
  }
  if (ti < nWideString)
    lpMultiString[i++]='\0';
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

BOOL IsCodePageValid(int nCodePage)
{
  char ch='A';
  wchar_t wch;

  if (nCodePage == CP_UNICODE_UCS2_LE || nCodePage == CP_UNICODE_UCS2_BE || nCodePage == CP_UNICODE_UTF8)
    return TRUE;
  if (MultiByteToWideChar(nCodePage, 0, &ch, 1, &wch, 1))
    return TRUE;

  return FALSE;
}

int IsCodePageMismatch(HWND hWnd, int nCodePage)
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

    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax) || (ftflags & AEFR_SELECTION))
    {
      SendMessage(hWndFind, CB_SETCURSEL, 0, 0);
    }
    else
    {
      if (ExGetRangeTextA(hWndEdit, &crSel.ciMin, &crSel.ciMax, FALSE, &szData, AELB_R))
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

    if (ftflags & AEFR_ALLFILES)
      SendMessage(hWndAllFiles, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_SELECTION)
      SendMessage(hWndInSelection, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_BEGINNING)
      SendMessage(hWndBeginning, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_DOWN)
      SendMessage(hWndForward, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_UP)
      SendMessage(hWndBackward, BM_SETCHECK, BST_CHECKED, 0);

    if (ftflags & AEFR_MATCHCASE) SendMessage(hWndMatchCase, BM_SETCHECK, BST_CHECKED, 0);
    if (ftflags & AEFR_ESCAPESEQ) SendMessage(hWndEscapeSeq, BM_SETCHECK, BST_CHECKED, 0);

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
      if (SendMessage(hWndMatchCase, BM_GETCHECK, 0, 0)) ftflags|=AEFR_MATCHCASE;
      else ftflags&=~AEFR_MATCHCASE;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ESCAPESEQ)
    {
      if (SendMessage(hWndEscapeSeq, BM_GETCHECK, 0, 0)) ftflags|=AEFR_ESCAPESEQ;
      else ftflags&=~AEFR_ESCAPESEQ;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_FORWARD)
    {
      ftflags&=~AEFR_UP&~AEFR_BEGINNING&~AEFR_SELECTION&~AEFR_ALLFILES;
      ftflags|=AEFR_DOWN;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BACKWARD)
    {
      ftflags&=~AEFR_DOWN&~AEFR_BEGINNING&~AEFR_SELECTION&~AEFR_ALLFILES;
      ftflags|=AEFR_UP;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BEGINNING)
    {
      ftflags&=~AEFR_UP&~AEFR_SELECTION&~AEFR_ALLFILES;
      ftflags|=AEFR_DOWN|AEFR_BEGINNING;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_INSEL)
    {
      ftflags&=~AEFR_UP&~AEFR_BEGINNING&~AEFR_ALLFILES;
      ftflags|=AEFR_DOWN|AEFR_SELECTION;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ALLFILES)
    {
      ftflags&=~AEFR_UP&~AEFR_SELECTION;
      ftflags|=AEFR_DOWN|AEFR_BEGINNING|AEFR_ALLFILES;
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
      if (bSpecialCheck == TRUE || (ftflags & AEFR_ALLFILES)) ftflags|=AEFR_BEGINNING;
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

      if (bReplaceAll)
      {
        hWndFocus=GetFocus();
        bReplaceAllButtonState=EnableWindow(hWndReplaceAllButton, FALSE);
      }
      if (ftflags & AEFR_ALLFILES)
      {
        HWND hWndTmp=hWndFrameActive;
        int nChanges=0;
        int nChangedFiles=0;

        if (bReplaceDlg == TRUE && bReplaceAll == TRUE)
        {
          ftflags|=AEFR_BEGINNING;
          if (bSpecialCheck == TRUE)
          {
            bSpecialCheck=FALSE;
            SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
          }

          do
          {
            ReplaceTextA(hWndEdit, ftflags, szFind, szReplace, TRUE, &nReplaceCount);
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
              nResult=ReplaceTextA(hWndEdit, ftflags, szFind, szReplace, FALSE, NULL);
            else
              nResult=FindTextA(hWndEdit, ftflags, szFind);

            if (nResult == -1)
            {
              bCanReplace=FALSE;
              ftflags|=AEFR_BEGINNING;
              SendMessage(hMdiClient, WM_MDINEXT, (WPARAM)hWndFrameActive, FALSE);
            }
            else
            {
              bCanReplace=TRUE;
              ftflags&=~AEFR_BEGINNING;
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
          nResult=ReplaceTextA(hWndEdit, ftflags, szFind, szReplace, bReplaceAll, &nReplaceCount);
        else
          nResult=FindTextA(hWndEdit, ftflags, szFind);

        if (nResult == -1)
        {
          if (bSpecialCheck == TRUE)
          {
            ftflags|=AEFR_BEGINNING;
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
          if (ftflags & AEFR_BEGINNING)
          {
            ftflags&=~AEFR_BEGINNING;
            bSpecialCheck=TRUE;
            SendMessage(hWndBeginning, BM_SETSTATE, TRUE, 0);
          }
        }
      }
      if (bReplaceAll)
      {
        EnableWindow(hWndReplaceAllButton, !bReplaceAllButtonState);
        SetFocus(hWndFocus);
      }
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

    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax) || (ftflags & AEFR_SELECTION))
    {
      SendMessage(hWndFind, CB_SETCURSEL, 0, 0);
    }
    else
    {
      if (ExGetRangeTextW(hWndEdit, &crSel.ciMin, &crSel.ciMax, FALSE, &wszData, AELB_R))
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

    if (ftflags & AEFR_ALLFILES)
      SendMessage(hWndAllFiles, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_SELECTION)
      SendMessage(hWndInSelection, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_BEGINNING)
      SendMessage(hWndBeginning, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_DOWN)
      SendMessage(hWndForward, BM_SETCHECK, BST_CHECKED, 0);
    else if (ftflags & AEFR_UP)
      SendMessage(hWndBackward, BM_SETCHECK, BST_CHECKED, 0);

    if (ftflags & AEFR_MATCHCASE) SendMessage(hWndMatchCase, BM_SETCHECK, BST_CHECKED, 0);
    if (ftflags & AEFR_ESCAPESEQ) SendMessage(hWndEscapeSeq, BM_SETCHECK, BST_CHECKED, 0);

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
      if (SendMessage(hWndMatchCase, BM_GETCHECK, 0, 0)) ftflags|=AEFR_MATCHCASE;
      else ftflags&=~AEFR_MATCHCASE;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ESCAPESEQ)
    {
      if (SendMessage(hWndEscapeSeq, BM_GETCHECK, 0, 0)) ftflags|=AEFR_ESCAPESEQ;
      else ftflags&=~AEFR_ESCAPESEQ;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_FORWARD)
    {
      ftflags&=~AEFR_UP&~AEFR_BEGINNING&~AEFR_SELECTION&~AEFR_ALLFILES;
      ftflags|=AEFR_DOWN;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BACKWARD)
    {
      ftflags&=~AEFR_DOWN&~AEFR_BEGINNING&~AEFR_SELECTION&~AEFR_ALLFILES;
      ftflags|=AEFR_UP;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BEGINNING)
    {
      ftflags&=~AEFR_UP&~AEFR_SELECTION&~AEFR_ALLFILES;
      ftflags|=AEFR_DOWN|AEFR_BEGINNING;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_INSEL)
    {
      ftflags&=~AEFR_UP&~AEFR_BEGINNING&~AEFR_ALLFILES;
      ftflags|=AEFR_DOWN|AEFR_SELECTION;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ALLFILES)
    {
      ftflags&=~AEFR_UP&~AEFR_SELECTION;
      ftflags|=AEFR_DOWN|AEFR_BEGINNING|AEFR_ALLFILES;
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
      if (bSpecialCheck == TRUE || (ftflags & AEFR_ALLFILES)) ftflags|=AEFR_BEGINNING;
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

      if (bReplaceAll)
      {
        hWndFocus=GetFocus();
        bReplaceAllButtonState=EnableWindow(hWndReplaceAllButton, FALSE);
      }
      if (ftflags & AEFR_ALLFILES)
      {
        HWND hWndTmp=hWndFrameActive;
        int nChanges=0;
        int nChangedFiles=0;

        if (bReplaceDlg == TRUE && bReplaceAll == TRUE)
        {
          ftflags|=AEFR_BEGINNING;
          if (bSpecialCheck == TRUE)
          {
            bSpecialCheck=FALSE;
            SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
          }

          do
          {
            ReplaceTextW(hWndEdit, ftflags, wszFind, wszReplace, TRUE, &nReplaceCount);
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
              nResult=ReplaceTextW(hWndEdit, ftflags, wszFind, wszReplace, FALSE, NULL);
            else
              nResult=FindTextW(hWndEdit, ftflags, wszFind);

            if (nResult == -1)
            {
              bCanReplace=FALSE;
              ftflags|=AEFR_BEGINNING;
              SendMessage(hMdiClient, WM_MDINEXT, (WPARAM)hWndFrameActive, FALSE);
            }
            else
            {
              bCanReplace=TRUE;
              ftflags&=~AEFR_BEGINNING;
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
          nResult=ReplaceTextW(hWndEdit, ftflags, wszFind, wszReplace, bReplaceAll, &nReplaceCount);
        else
          nResult=FindTextW(hWndEdit, ftflags, wszFind);

        if (nResult == -1)
        {
          if (bSpecialCheck == TRUE)
          {
            ftflags|=AEFR_BEGINNING;
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
          if (ftflags & AEFR_BEGINNING)
          {
            ftflags&=~AEFR_BEGINNING;
            bSpecialCheck=TRUE;
            SendMessage(hWndBeginning, BM_SETSTATE, TRUE, 0);
          }
        }
      }
      if (bReplaceAll)
      {
        EnableWindow(hWndReplaceAllButton, !bReplaceAllButtonState);
        SetFocus(hWndFocus);
      }
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
  char szRegValue[32];
  char *szData;
  HWND hWnd;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int i;

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
  wchar_t wszRegValue[32];
  wchar_t *wszData;
  HWND hWnd;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int i;

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

    if (ftflags & AEFR_ESCAPESEQ)
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

    if (ftflags & AEFR_ESCAPESEQ)
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
  char szRegValue[32];
  char *szData;
  HWND hWnd;
  HKEY hKey;
  int nSize;
  int i;

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
  wchar_t wszRegValue[32];
  wchar_t *wszData;
  HWND hWnd;
  HKEY hKey;
  int nSize;
  int i;

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

int FindTextA(HWND hWnd, DWORD dwFlags, char *pFindIt)
{
  AEFINDTEXTA ft;
  DWORD dwScrollFlags=0;
  DWORD dwScrollResult;
  BOOL bResult;

  if (dwFlags & AEFR_SELECTION)
  {
    ft.crSearch=crSel;
  }
  else if (dwFlags & AEFR_BEGINNING)
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ft.crSearch.ciMin);
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ft.crSearch.ciMax);
  }
  else if (dwFlags & AEFR_DOWN)
  {
    ft.crSearch.ciMin=crSel.ciMax;
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ft.crSearch.ciMax);
  }
  else if (dwFlags & AEFR_UP)
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ft.crSearch.ciMin);
    ft.crSearch.ciMax=crSel.ciMin;
  }
  else return FALSE;

  ft.dwFlags=dwFlags;
  ft.pText=pFindIt;
  ft.dwTextLen=(DWORD)-1;
  ft.nNewLine=AELB_R;

  if (bResult=SendMessage(hWnd, AEM_FINDTEXTA, 0, (LPARAM)&ft))
  {
    SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, TRUE);
    SetSel(hWnd, &ft.crFound, FALSE, NULL);
    SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, FALSE);

    dwScrollResult=SendMessage(hWnd, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(0, 0));
    if (dwScrollResult & AECSE_SCROLLEDX)
      dwScrollFlags|=AESC_UNITRECTDIVX;
    if (dwScrollResult & AECSE_SCROLLEDY)
      dwScrollFlags|=AESC_UNITRECTDIVY;
    SendMessage(hWnd, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
  }
  else SendMessage(hMainWnd, AKDN_SEARCH_ENDED, (WPARAM)hDlgModeless, 0);

  return bResult?0:-1;
}

int FindTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt)
{
  AEFINDTEXTW ft;
  DWORD dwScrollFlags=0;
  DWORD dwScrollResult;
  BOOL bResult;

  if (dwFlags & AEFR_SELECTION)
  {
    ft.crSearch=crSel;
  }
  else if (dwFlags & AEFR_BEGINNING)
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ft.crSearch.ciMin);
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ft.crSearch.ciMax);
  }
  else if (dwFlags & AEFR_DOWN)
  {
    ft.crSearch.ciMin=crSel.ciMax;
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ft.crSearch.ciMax);
  }
  else if (dwFlags & AEFR_UP)
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ft.crSearch.ciMin);
    ft.crSearch.ciMax=crSel.ciMin;
  }
  else return FALSE;

  ft.dwFlags=dwFlags;
  ft.wpText=wpFindIt;
  ft.dwTextLen=(DWORD)-1;
  ft.nNewLine=AELB_R;

  if (bResult=SendMessage(hWnd, AEM_FINDTEXTW, 0, (LPARAM)&ft))
  {
    SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, TRUE);
    SetSel(hWnd, &ft.crFound, FALSE, NULL);
    SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, FALSE);

    dwScrollResult=SendMessage(hWnd, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(0, 0));
    if (dwScrollResult & AECSE_SCROLLEDX)
      dwScrollFlags|=AESC_UNITRECTDIVX;
    if (dwScrollResult & AECSE_SCROLLEDY)
      dwScrollFlags|=AESC_UNITRECTDIVY;
    SendMessage(hWnd, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
  }
  else SendMessage(hMainWnd, AKDN_SEARCH_ENDED, (WPARAM)hDlgModeless, 0);

  return bResult?0:-1;
}

int ReplaceTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, char *pReplaceWith, BOOL bAll, int *nReplaceCount)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  AECHARINDEX ciFirstVisibleBefore;
  AECHARINDEX ciFirstVisibleAfter;
  CHARRANGE crInitialRE;
  char *szText;
  char *szReplaceText;
  char *pMin;
  char *pMax;
  char *pFirstVisible;
  int nReplaceTextLen;
  int nChanges=0;
  int nResult=-1;
  BOOL bInitialColumnSel;
  BOOL bColumnSel;

  if (bAll)
  {
    bInitialColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);

    if (dwFlags & AEFR_SELECTION)
    {
      crRange=crSel;
      bColumnSel=bInitialColumnSel;
    }
    else if (dwFlags & AEFR_BEGINNING)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
      bColumnSel=FALSE;
    }
    else if (dwFlags & AEFR_DOWN)
    {
      crRange.ciMin=crSel.ciMin;
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
      bColumnSel=FALSE;
    }
    else if (dwFlags & AEFR_UP)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
      crRange.ciMax=crSel.ciMax;
      bColumnSel=FALSE;
    }
    else return FALSE;

    if (ExGetRangeTextA(hWnd, &crRange.ciMin, &crRange.ciMax, bColumnSel, &szText, AELB_R))
    {
      if (StrReplaceA(szText, pFindIt, pReplaceWith, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, NULL, &nReplaceTextLen, NULL, NULL, NULL))
      {
        if (szReplaceText=(char *)API_HeapAlloc(hHeap, 0, nReplaceTextLen + 1))
        {
          //Remember selection
          SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&crInitialRE);

          if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
          {
            pMin=szText + crInitialRE.cpMin;
            pMax=szText + crInitialRE.cpMax;
          }
          else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
          {
            pMin=szText;
            pMax=szText + (crInitialRE.cpMax - crInitialRE.cpMin);
          }

          //Remember scroll
          SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstVisibleBefore);

          if (AEC_IndexCompare(&ciFirstVisibleBefore, &crRange.ciMin) >= 0)
          {
            pFirstVisible=szText + IndexSubtract(hWnd, &ciFirstVisibleBefore, &crRange.ciMin, AELB_R, FALSE);
          }
          else pFirstVisible=NULL;

          //Replace operation
          if (nChanges=StrReplaceA(szText, pFindIt, pReplaceWith, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, szReplaceText, NULL, &pMin, &pMax, &pFirstVisible))
          {
            //Stop redraw
            SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

            if (!(dwFlags & AEFR_SELECTION))
              SetSel(hWnd, &crRange, FALSE, NULL);
            ReplaceSelA(hWnd, szReplaceText, -1, bColumnSel, NULL, NULL);

            //Restore selection
            if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
            {
              RichOffsetToAkelIndex(hWnd, pMin - szText, &crInitialSel.ciMin);
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, pMax - pMin, AELB_R);
            }
            else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
            {
              SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crInitialSel.ciMin);
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, pMax - pMin, AELB_R);
            }
            if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
              SetSel(hWnd, &crInitialSel, bInitialColumnSel, &crInitialSel.ciMin);
            else
              SetSel(hWnd, &crInitialSel, bInitialColumnSel, &crInitialSel.ciMax);

            //Start redraw
            SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
            InvalidateRect(hWnd, NULL, TRUE);

            //Restore scroll
            SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstVisibleAfter);

            if (pFirstVisible)
            {
              ciFirstVisibleBefore=crRange.ciMin;
              SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&ciFirstVisibleBefore);
              IndexOffset(hWnd, &ciFirstVisibleBefore, pFirstVisible - szText, AELB_R);
              SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
            }
            else SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
          }
          API_HeapFree(hHeap, 0, (LPVOID)szReplaceText);
        }
      }
      FreeText(szText);
    }
  }
  else
  {
    if (ExGetRangeTextA(hWnd, &crSel.ciMin, &crSel.ciMax, FALSE, &szText, AELB_R))
    {
      if (((dwFlags & AEFR_MATCHCASE) && !lstrcmpA(pFindIt, szText)) ||
          (!(dwFlags & AEFR_MATCHCASE) && !lstrcmpiA(pFindIt, szText)))
      {
        ReplaceSelA(hWnd, pReplaceWith, -1, FALSE, NULL, NULL);
        nChanges=1;
      }
      FreeText(szText);
    }
    nResult=FindTextA(hWnd, dwFlags, pFindIt);
  }
  if (nReplaceCount) *nReplaceCount=nChanges;
  return nResult;
}

int ReplaceTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, wchar_t *wpReplaceWith, BOOL bAll, int *nReplaceCount)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  AECHARINDEX ciFirstVisibleBefore;
  AECHARINDEX ciFirstVisibleAfter;
  CHARRANGE crInitialRE;
  wchar_t *wszText;
  wchar_t *wszReplaceText;
  wchar_t *wpMin;
  wchar_t *wpMax;
  wchar_t *wpFirstVisible;
  int nReplaceTextLen;
  int nChanges=0;
  int nResult=-1;
  BOOL bInitialColumnSel;
  BOOL bColumnSel;

  if (bAll)
  {
    bInitialColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);

    if (dwFlags & AEFR_SELECTION)
    {
      crRange=crSel;
      bColumnSel=bInitialColumnSel;
    }
    else if (dwFlags & AEFR_BEGINNING)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
      bColumnSel=FALSE;
    }
    else if (dwFlags & AEFR_DOWN)
    {
      crRange.ciMin=crSel.ciMin;
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
      bColumnSel=FALSE;
    }
    else if (dwFlags & AEFR_UP)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
      crRange.ciMax=crSel.ciMax;
      bColumnSel=FALSE;
    }
    else return FALSE;

    if (ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, bColumnSel, &wszText, AELB_R))
    {
      if (StrReplaceW(wszText, wpFindIt, wpReplaceWith, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, NULL, &nReplaceTextLen, NULL, NULL, NULL))
      {
        if (wszReplaceText=(wchar_t *)API_HeapAlloc(hHeap, 0, nReplaceTextLen * sizeof(wchar_t) + 2))
        {
          //Remember selection
          SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&crInitialRE);

          if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
          {
            wpMin=wszText + crInitialRE.cpMin;
            wpMax=wszText + crInitialRE.cpMax;
          }
          else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
          {
            wpMin=wszText;
            wpMax=wszText + (crInitialRE.cpMax - crInitialRE.cpMin);
          }

          //Remember scroll
          SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstVisibleBefore);

          if (AEC_IndexCompare(&ciFirstVisibleBefore, &crRange.ciMin) >= 0)
          {
            wpFirstVisible=wszText + IndexSubtract(hWnd, &ciFirstVisibleBefore, &crRange.ciMin, AELB_R, FALSE);
          }
          else wpFirstVisible=NULL;

          //Replace operation
          if (nChanges=StrReplaceW(wszText, wpFindIt, wpReplaceWith, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, wszReplaceText, NULL, &wpMin, &wpMax, &wpFirstVisible))
          {
            //Stop redraw
            SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

            if (!(dwFlags & AEFR_SELECTION))
              SetSel(hWnd, &crRange, FALSE, NULL);
            ReplaceSelW(hWnd, wszReplaceText, -1, bColumnSel, NULL, NULL);

            //Restore selection
            if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
            {
              RichOffsetToAkelIndex(hWnd, wpMin - wszText, &crInitialSel.ciMin);
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, wpMax - wpMin, AELB_R);
            }
            else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
            {
              SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crInitialSel.ciMin);
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, wpMax - wpMin, AELB_R);
            }
            if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
              SetSel(hWnd, &crInitialSel, bInitialColumnSel, &crInitialSel.ciMin);
            else
              SetSel(hWnd, &crInitialSel, bInitialColumnSel, &crInitialSel.ciMax);

            //Start redraw
            SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
            InvalidateRect(hWnd, NULL, TRUE);

            //Restore scroll
            SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstVisibleAfter);

            if (wpFirstVisible)
            {
              ciFirstVisibleBefore=crRange.ciMin;
              SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&ciFirstVisibleBefore);
              IndexOffset(hWnd, &ciFirstVisibleBefore, wpFirstVisible - wszText, AELB_R);
              SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
            }
            else SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
          }
          API_HeapFree(hHeap, 0, (LPVOID)wszReplaceText);
        }
      }
      FreeText(wszText);
    }
  }
  else
  {
    if (ExGetRangeTextW(hWnd, &crSel.ciMin, &crSel.ciMax, FALSE, &wszText, AELB_R))
    {
      if (((dwFlags & AEFR_MATCHCASE) && !lstrcmpW(wpFindIt, wszText)) ||
          (!(dwFlags & AEFR_MATCHCASE) && !lstrcmpiW(wpFindIt, wszText)))
      {
        ReplaceSelW(hWnd, wpReplaceWith, -1, FALSE, NULL, NULL);
        nChanges=1;
      }
      FreeText(wszText);
    }
    nResult=FindTextW(hWnd, dwFlags, wpFindIt);
  }
  if (nReplaceCount) *nReplaceCount=nChanges;
  return nResult;
}

int StrReplaceA(char *pText, char *pIt, char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult, char **ppMin, char **ppMax, char **ppFirstVisible)
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

int StrReplaceW(wchar_t *wpText, wchar_t *wpIt, wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult, wchar_t **wppMin, wchar_t **wppMax, wchar_t **wppFirstVisible)
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
    else if (*a == '\n') *b='\\', *++b='n';
    else if (*a == '\\') *b='\\', *++b='\\';
    else *b=*a;
  }
  *b='\0';
}


//// Paste operation

void GetSel(HWND hWnd, AECHARRANGE *crSel, BOOL *bColumnSel, AECHARINDEX *ciCaret)
{
  AESELECTION aes;

  SendMessage(hWnd, AEM_GETSEL, (WPARAM)ciCaret, (LPARAM)&aes);
  if (crSel) *crSel=aes.crSel;
  if (bColumnSel) *bColumnSel=aes.bColumnSel;
}

void SetSel(HWND hWnd, AECHARRANGE *crSel, BOOL bColumnSel, AECHARINDEX *ciCaret)
{
  AESELECTION aes;

  aes.crSel=*crSel;
  if (bColumnSel == -1)
    aes.bColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);
  else
    aes.bColumnSel=bColumnSel;

  SendMessage(hWnd, AEM_SETSEL, (WPARAM)ciCaret, (LPARAM)&aes);
}

void ReplaceSelA(HWND hWnd, char *pData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AEREPLACESELA rs;

  rs.pText=pData;
  rs.dwTextLen=(DWORD)nDataLen;
  if (bColumnSel == -1)
    rs.bColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);
  else
    rs.bColumnSel=bColumnSel;
  rs.ciInsertStart=ciInsertStart;
  rs.ciInsertEnd=ciInsertEnd;
  SendMessage(hWnd, AEM_REPLACESELA, 0, (LPARAM)&rs);
}

void ReplaceSelW(HWND hWnd, wchar_t *wpData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AEREPLACESELW rs;

  rs.wpText=wpData;
  rs.dwTextLen=(DWORD)nDataLen;
  if (bColumnSel == -1)
    rs.bColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);
  else
    rs.bColumnSel=bColumnSel;
  rs.ciInsertStart=ciInsertStart;
  rs.ciInsertEnd=ciInsertEnd;
  SendMessage(hWnd, AEM_REPLACESELW, 0, (LPARAM)&rs);
}

int IndexSubtract(HWND hWnd, AECHARINDEX *ciChar1, AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel)
{
  AEINDEXSUBTRACT aeis;

  aeis.ciChar1=ciChar1;
  aeis.ciChar2=ciChar2;
  aeis.nNewLine=nNewLine;
  if (bColumnSel == -1)
    aeis.bColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);
  else
    aeis.bColumnSel=bColumnSel;
  return SendMessage(hWnd, AEM_INDEXSUBTRACT, 0, (LPARAM)&aeis);
}

int IndexOffset(HWND hWnd, AECHARINDEX *ciChar, int nOffset, int nNewLine)
{
  AEINDEXOFFSET aeio;

  aeio.ciCharIn=ciChar;
  aeio.ciCharOut=ciChar;
  aeio.nOffset=nOffset;
  aeio.nNewLine=nNewLine;
  return SendMessage(hWnd, AEM_INDEXOFFSET, 0, (LPARAM)&aeio);
}

int AkelIndexToRichOffset(HWND hWnd, AECHARINDEX *ciChar)
{
  return SendMessage(hWnd, AEM_INDEXTORICHOFFSET, 0, (LPARAM)ciChar);
}

void RichOffsetToAkelIndex(HWND hWnd, int nOffset, AECHARINDEX *ciChar)
{
  SendMessage(hWnd, AEM_RICHOFFSETTOINDEX, nOffset, (LPARAM)ciChar);
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

  if (nMax == -1)
  {
    nMax=GetTextLength(hWnd);
  }
  if (nMin < nMax)
  {
    nLen=(nMax - nMin) + 1;

    if (*pText=(char *)API_HeapAlloc(hHeap, 0, nLen + 1))
    {
      txtrngA.chrg.cpMin=nMin;
      txtrngA.chrg.cpMax=nMax;
      txtrngA.lpstrText=*pText;
      SendMessageA(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&txtrngA);

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
    nLen=(nMax - nMin) + 1;

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

int ExGetRangeTextA(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, char **pText, int nNewLine)
{
  AETEXTRANGEA tr;
  int nLen;

  tr.cr.ciMin=*ciMin;
  tr.cr.ciMax=*ciMax;
  if (bColumnSel == -1)
    tr.bColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);
  else
    tr.bColumnSel=bColumnSel;
  tr.pBuffer=NULL;
  tr.dwBufferMax=(DWORD)-1;
  tr.nNewLine=nNewLine;

  if (nLen=SendMessage(hWnd, AEM_GETTEXTRANGEA, 0, (LPARAM)&tr))
  {
    if (tr.pBuffer=(char *)API_HeapAlloc(hHeap, 0, nLen))
    {
      nLen=SendMessage(hWnd, AEM_GETTEXTRANGEA, 0, (LPARAM)&tr);
    }
  }
  *pText=tr.pBuffer;
  return nLen;
}

int ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, wchar_t **wpText, int nNewLine)
{
  AETEXTRANGEW tr;
  int nLen;

  tr.cr.ciMin=*ciMin;
  tr.cr.ciMax=*ciMax;
  if (bColumnSel == -1)
    tr.bColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0);
  else
    tr.bColumnSel=bColumnSel;
  tr.wpBuffer=NULL;
  tr.dwBufferMax=(DWORD)-1;
  tr.nNewLine=nNewLine;

  if (nLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr))
  {
    if (tr.wpBuffer=(wchar_t *)API_HeapAlloc(hHeap, 0, nLen * sizeof(wchar_t)))
    {
      nLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);
    }
  }
  *wpText=tr.wpBuffer;
  return nLen;
}

BOOL FreeText(LPVOID pText)
{
  if (pText) return API_HeapFree(hHeap, 0, pText);
  return FALSE;
}

void PasteInEditAsRichEdit(HWND hWnd)
{
  HGLOBAL hData;
  LPVOID pData;

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
          API_HeapFree(hHeap, 0, (LPVOID)pTarget);
        }
        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
}


//// Go to line

BOOL CALLBACK GoToLineDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AECHARRANGE cr;
  int nLineNumber=0;
  int nLineCount=0;
  int a;
  int b;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    SetDlgItemInt(hDlg, IDC_GOTOLINE, ciCaret.nLine + 1, FALSE);

    if (rcGoToLineDlg.right && rcGoToLineDlg.bottom)
      SetWindowPos(hDlg, 0, rcGoToLineDlg.left, rcGoToLineDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      if (GetDlgItemTextA(hDlg, IDC_GOTOLINE, buf, BUFFER_SIZE))
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

        nLineNumber=xatoiA(buf2);
      }
      nLineCount=SendMessage(hWndEdit, AEM_GETLINECOUNT, 0, 0);

      if (!nLineNumber)
      {
        API_LoadStringA(hLangLib, MSG_WRONG_STRING, buf, BUFFER_SIZE);
        MessageBoxA(hDlg, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
        return FALSE;
      }
      else if (nLineNumber < 0)
      {
        nLineNumber=nLineCount + nLineNumber + 1;
        if (nLineNumber <= 0) nLineNumber=1;
      }
      nLineNumber=min(nLineNumber, nLineCount);

      if (SendMessage(hWndEdit, AEM_GETLINEINDEX, nLineNumber - 1, (LPARAM)&cr.ciMin))
      {
        DWORD dwScrollResult=0;
        DWORD dwScrollFlags=0;

        SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, TRUE);
        cr.ciMax=cr.ciMin;
        SetSel(hWndEdit, &cr, FALSE, NULL);
        SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, FALSE);

        dwScrollResult=SendMessage(hWndEdit, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(0, 0));
        if (dwScrollResult & AECSE_SCROLLEDX)
          dwScrollFlags|=AESC_UNITRECTDIVX;
        if (dwScrollResult & AECSE_SCROLLEDY)
          dwScrollFlags|=AESC_UNITRECTDIVY;
        SendMessage(hWndEdit, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
      }
    }
    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDCANCEL)
    {
      GetWindowPos(hDlg, NULL, &rcGoToLineDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK GoToLineDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AECHARRANGE cr;
  int nLineNumber=0;
  int nLineCount=0;
  int a;
  int b;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    SetDlgItemInt(hDlg, IDC_GOTOLINE, ciCaret.nLine + 1, FALSE);

    if (rcGoToLineDlg.right && rcGoToLineDlg.bottom)
      SetWindowPos(hDlg, 0, rcGoToLineDlg.left, rcGoToLineDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      if (GetDlgItemTextW(hDlg, IDC_GOTOLINE, wbuf, BUFFER_SIZE))
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

        nLineNumber=xatoiW(wbuf2);
      }
      nLineCount=SendMessage(hWndEdit, AEM_GETLINECOUNT, 0, 0);

      if (!nLineNumber)
      {
        API_LoadStringW(hLangLib, MSG_WRONG_STRING, wbuf, BUFFER_SIZE);
        MessageBoxW(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
        return FALSE;
      }
      else if (nLineNumber < 0)
      {
        nLineNumber=nLineCount + nLineNumber + 1;
        if (nLineNumber <= 0) nLineNumber=1;
      }
      nLineNumber=min(nLineNumber, nLineCount);

      if (SendMessage(hWndEdit, AEM_GETLINEINDEX, nLineNumber - 1, (LPARAM)&cr.ciMin))
      {
        DWORD dwScrollResult=0;
        DWORD dwScrollFlags=0;

        SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, TRUE);
        cr.ciMax=cr.ciMin;
        SetSel(hWndEdit, &cr, FALSE, NULL);
        SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, FALSE);

        dwScrollResult=SendMessage(hWndEdit, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(0, 0));
        if (dwScrollResult & AECSE_SCROLLEDX)
          dwScrollFlags|=AESC_UNITRECTDIVX;
        if (dwScrollResult & AECSE_SCROLLEDY)
          dwScrollFlags|=AESC_UNITRECTDIVY;
        SendMessage(hWndEdit, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
      }
    }
    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDCANCEL)
    {
      GetWindowPos(hDlg, NULL, &rcGoToLineDlg);
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
      TrimPathA(buf, lpszRecentNames[i], RECENTFILES_RECORD_LENGTH);
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
      TrimPathW(wbuf, lpwszRecentNames[i], RECENTFILES_RECORD_LENGTH);
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


//// Recode

void RecodeTextW(HWND hWnd, int nCodePageFrom, int nCodePageTo)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  wchar_t *wszSelText;
  char *szText;
  wchar_t *wszText;
  int nFirstLine;
  int nUnicodeLen;
  int nAnsiLen;
  BOOL bSelection;

  SaveLineScroll(hWnd, &nFirstLine);
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
    SetSel(hWnd, &crRange, FALSE, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nUnicodeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszSelText, AELB_ASIS))
  {
    nAnsiLen=WideCharToMultiByte(nCodePageFrom, 0, wszSelText, nUnicodeLen + 1, NULL, 0, NULL, NULL);

    if (szText=(char *)API_HeapAlloc(hHeap, 0, nAnsiLen))
    {
      WideCharToMultiByte(nCodePageFrom, 0, wszSelText, nUnicodeLen + 1, szText, nAnsiLen, NULL, NULL);
      nUnicodeLen=MultiByteToWideChar(nCodePageTo, 0, szText, nAnsiLen, NULL, 0);

      if (wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nUnicodeLen * sizeof(wchar_t)))
      {
        MultiByteToWideChar(nCodePageTo, 0, szText, nAnsiLen, wszText, nUnicodeLen);

        ReplaceSelW(hWnd, wszText, nUnicodeLen - 1, -1, &crRange.ciMin, &crRange.ciMax);

        //Update selection
        if (!bSelection)
        {
          SendMessage(hWnd, AEM_UPDATEINDEX, 0, (LPARAM)&crInitialSel.ciMin);
          crRange.ciMin=crInitialSel.ciMin;
          crRange.ciMax=crInitialSel.ciMin;
        }

        if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
          SetSel(hWnd, &crRange, -1, &crRange.ciMin);
        else
          SetSel(hWnd, &crRange, -1, &crRange.ciMax);

        API_HeapFree(hHeap, 0, (LPVOID)wszText);
      }
      API_HeapFree(hHeap, 0, (LPVOID)szText);
    }
    FreeText(wszSelText);
  }
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
  RestoreLineScroll(hWnd, &nFirstLine);
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


//// Colors

BOOL CALLBACK ColorsDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndThemeName;
  static HWND hWndThemeSave;
  static HWND hWndThemeDelete;
  static HWND hWndList;
  static AECOLORS aecColorsDlg;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndThemeName=GetDlgItem(hDlg, IDC_COLORS_THEME_NAME);
    hWndThemeSave=GetDlgItem(hDlg, IDC_COLORS_THEME_SAVE);
    hWndThemeDelete=GetDlgItem(hDlg, IDC_COLORS_THEME_DELETE);
    hWndList=GetDlgItem(hDlg, IDC_COLORS_LIST);
    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);

    //Columns
    {
      LVCOLUMNA lvcA;

      lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvcA.pszText=buf;

      API_LoadStringA(hLangLib, STR_ELEMENT, buf, BUFFER_SIZE);
      lvcA.cx=110;
      lvcA.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTCOLUMNA, LVSI_COLOR_ELEMENT, (LPARAM)&lvcA);

      API_LoadStringA(hLangLib, STR_TEXT, buf, BUFFER_SIZE);
      lvcA.cx=70;
      lvcA.iSubItem=LVSI_COLOR_TEXT;
      SendMessage(hWndList, LVM_INSERTCOLUMNA, LVSI_COLOR_TEXT, (LPARAM)&lvcA);

      API_LoadStringA(hLangLib, STR_BACKGROUND, buf, BUFFER_SIZE);
      lvcA.cx=70;
      lvcA.iSubItem=LVSI_COLOR_BACKGROUND;
      SendMessage(hWndList, LVM_INSERTCOLUMNA, LVSI_COLOR_BACKGROUND, (LPARAM)&lvcA);

      API_LoadStringA(hLangLib, STR_SAMPLE, buf, BUFFER_SIZE);
      lvcA.cx=70;
      lvcA.iSubItem=LVSI_COLOR_SAMPLE;
      SendMessage(hWndList, LVM_INSERTCOLUMNA, LVSI_COLOR_SAMPLE, (LPARAM)&lvcA);
    }

    //Rows
    {
      LVITEMA lviA;
      int i;

      API_LoadStringA(hLangLib, STR_BASIC, buf, BUFFER_SIZE);
      lviA.mask=LVIF_TEXT;
      lviA.pszText=buf;
      lviA.iItem=LVI_COLOR_BASIC;
      lviA.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      API_LoadStringA(hLangLib, STR_SELECTION, buf, BUFFER_SIZE);
      lviA.mask=LVIF_TEXT;
      lviA.pszText=buf;
      lviA.iItem=LVI_COLOR_SELECTION;
      lviA.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      API_LoadStringA(hLangLib, STR_ACTIVELINE, buf, BUFFER_SIZE);
      lviA.mask=LVIF_TEXT;
      lviA.pszText=buf;
      lviA.iItem=LVI_COLOR_ACTIVELINE;
      lviA.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      API_LoadStringA(hLangLib, STR_CARET, buf, BUFFER_SIZE);
      lviA.mask=LVIF_TEXT;
      lviA.pszText=buf;
      lviA.iItem=LVI_COLOR_CARET;
      lviA.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      API_LoadStringA(hLangLib, STR_URL, buf, BUFFER_SIZE);
      lviA.mask=LVIF_TEXT;
      lviA.pszText=buf;
      lviA.iItem=LVI_COLOR_URL;
      lviA.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      //Set "Sample" text
      API_LoadStringA(hLangLib, STR_SAMPLE, buf, BUFFER_SIZE);

      for (i=LVI_COLOR_BASIC; i < LVI_COLOR_CARET; ++i)
      {
        lviA.mask=LVIF_TEXT;
        lviA.pszText=buf;
        lviA.iItem=i;
        lviA.iSubItem=LVSI_COLOR_SAMPLE;
        SendMessage(hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);
      }
    }

    //Combobox
    {
      COLORTHEMEA *ctElement;
      int nSelection=-1;

      FillComboboxThemesA(hWndThemeName);

      aecColorsDlg=aecColors;

      if (!aecColorsDlg.dwFlags)
      {
        aecColorsDlg.dwFlags=AECLR_DEFAULT|AECLR_ALL;
        SendMessage(hWndEdit, AEM_GETCOLORS, 0, (LPARAM)&aecColorsDlg);
      }
      if (ctElement=StackThemeGetByColorsA(&hThemesStack, &aecColorsDlg))
        nSelection=SendMessageA(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->szName);
      SendMessage(hWndThemeName, CB_SETCURSEL, (WPARAM)nSelection, 0);

      if (nSelection <= 0)
      {
        EnableWindow(hWndThemeSave, FALSE);
        EnableWindow(hWndThemeDelete, FALSE);
      }
      else if (nSelection > 0)
      {
        EnableWindow(hWndThemeSave, FALSE);
        EnableWindow(hWndThemeDelete, TRUE);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_COLORS_LIST)
    {
      if ((int)((NMHDR *)lParam)->code == NM_CUSTOMDRAW)
      {
        LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)lParam;
        LRESULT lResult;

        if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
          lResult=CDRF_NOTIFYITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
          lResult=CDRF_NOTIFYSUBITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == (CDDS_SUBITEM|CDDS_ITEMPREPAINT))
        {
          if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_BASIC)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crBasicText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crBasicBk;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrText=aecColorsDlg.crBasicText;
              lplvcd->clrTextBk=aecColorsDlg.crBasicBk;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_SELECTION)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crSelText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crSelBk;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrText=aecColorsDlg.crSelText;
              lplvcd->clrTextBk=aecColorsDlg.crSelBk;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_ACTIVELINE)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveLineText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveLineBk;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrText=aecColorsDlg.crActiveLineText;
              lplvcd->clrTextBk=aecColorsDlg.crActiveLineBk;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_CARET)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crCaret;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crCaret;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crCaret;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_URL)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crUrlText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crUrlText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crUrlText;
            }
          }
          lResult=CDRF_DODEFAULT;
        }
        else lResult=CDRF_DODEFAULT;

        SetWindowLongA(hDlg, DWL_MSGRESULT, (LONG)lResult);
        return TRUE;
      }
      else if ((int)((NMHDR *)lParam)->code == NM_CLICK)
      {
        LVHITTESTINFO lvhti={0};
        BOOL bResult=FALSE;

        GetCursorPos(&lvhti.pt);
        ScreenToClient(hWndList, &lvhti.pt);
        SendMessage(hWndList, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvhti);

        if (lvhti.iItem >= 0)
        {
          if (lvhti.iSubItem == LVSI_COLOR_TEXT)
          {
            if (lvhti.iItem == LVI_COLOR_BASIC)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crBasicText);
            else if (lvhti.iItem == LVI_COLOR_SELECTION)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crSelText);
            else if (lvhti.iItem == LVI_COLOR_ACTIVELINE)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crActiveLineText);
            else if (lvhti.iItem == LVI_COLOR_CARET)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crCaret);
            else if (lvhti.iItem == LVI_COLOR_URL)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crUrlText);
          }
          else if (lvhti.iSubItem == LVSI_COLOR_BACKGROUND)
          {
            if (lvhti.iItem == LVI_COLOR_BASIC)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crBasicBk);
            else if (lvhti.iItem == LVI_COLOR_SELECTION)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crSelBk);
            else if (lvhti.iItem == LVI_COLOR_ACTIVELINE)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crActiveLineBk);
            else if (lvhti.iItem == LVI_COLOR_CARET)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crCaret);
            else if (lvhti.iItem == LVI_COLOR_URL)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crUrlText);
          }
          if (bResult)
          {
            COLORTHEMEA *ctElement;
            int nSelection=-1;

            if (ctElement=StackThemeGetByColorsA(&hThemesStack, &aecColorsDlg))
              nSelection=SendMessageA(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->szName);
            SendMessage(hWndThemeName, CB_SETCURSEL, (WPARAM)nSelection, 0);
            if (nSelection > 0)
              EnableWindow(hWndThemeDelete, TRUE);
            else
              EnableWindow(hWndThemeDelete, FALSE);

            bColorsChanged=TRUE;
            InvalidateRect(hWndList, NULL, TRUE);
          }
        }
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_COLORS_THEME_NAME)
    {
      if (HIWORD(wParam) == CBN_EDITCHANGE)
      {
        COLORTHEMEA *ctElement;
        int nSelection=-1;
        int nNameLen;

        if (nNameLen=GetWindowTextA(hWndThemeName, buf, BUFFER_SIZE))
        {
          if (ctElement=StackThemeGetByNameA(&hThemesStack, buf))
          {
            nSelection=SendMessageA(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->szName);
          }
        }

        if (nSelection < 0)
        {
          EnableWindow(hWndThemeSave, nNameLen?TRUE:FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection == 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection > 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, TRUE);
        }
      }
      else if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        COLORTHEMEA *ctElement;
        int nSelection;

        if ((nSelection=SendMessage(hWndThemeName, CB_GETCURSEL, 0, 0)) != CB_ERR)
        {
          if (SendMessageA(hWndThemeName, CB_GETLBTEXT, (WPARAM)nSelection, (LPARAM)buf))
          {
            if (ctElement=StackThemeGetByNameA(&hThemesStack, buf))
            {
              aecColorsDlg=ctElement->aec;
              bColorsChanged=TRUE;
              InvalidateRect(hWndList, NULL, TRUE);
            }
          }
        }

        if (nSelection < 0)
        {
          EnableWindow(hWndThemeSave, TRUE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection == 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection > 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_COLORS_THEME_SAVE)
    {
      if (GetWindowTextA(hWndThemeName, buf, BUFFER_SIZE))
      {
        StackThemeAddA(&hThemesStack, buf, &aecColorsDlg);
        SendMessageA(hWndThemeName, CB_ADDSTRING, 0, (LPARAM)buf);

        EnableWindow(hWndThemeSave, FALSE);
        EnableWindow(hWndThemeDelete, TRUE);
      }
    }
    else if (LOWORD(wParam) == IDC_COLORS_THEME_DELETE)
    {
      if (GetWindowTextA(hWndThemeName, buf, BUFFER_SIZE))
      {
        COLORTHEMEA *ctElement;
        int nSelection;

        if (ctElement=StackThemeGetByNameA(&hThemesStack, buf))
        {
          StackThemeDeleteA(&hThemesStack, ctElement);
          nSelection=SendMessageA(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->szName);
          SendMessageA(hWndThemeName, CB_DELETESTRING, nSelection, 0);
          SendMessage(hWndThemeName, CB_SETCURSEL, (WPARAM)-1, 0);

          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (bColorsChanged)
      {
        aecColors=aecColorsDlg;
        aecColors.dwFlags=AECLR_ALL;
        SendMessage(hWndEdit, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
      }
      SaveThemesA(TRUE);

      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK ColorsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndThemeName;
  static HWND hWndThemeSave;
  static HWND hWndThemeDelete;
  static HWND hWndList;
  static AECOLORS aecColorsDlg;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndThemeName=GetDlgItem(hDlg, IDC_COLORS_THEME_NAME);
    hWndThemeSave=GetDlgItem(hDlg, IDC_COLORS_THEME_SAVE);
    hWndThemeDelete=GetDlgItem(hDlg, IDC_COLORS_THEME_DELETE);
    hWndList=GetDlgItem(hDlg, IDC_COLORS_LIST);
    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_GRIDLINES, LVS_EX_GRIDLINES);

    //Columns
    {
      LVCOLUMNW lvcW;

      lvcW.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvcW.pszText=wbuf;

      API_LoadStringW(hLangLib, STR_ELEMENT, wbuf, BUFFER_SIZE);
      lvcW.cx=110;
      lvcW.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTCOLUMNW, LVSI_COLOR_ELEMENT, (LPARAM)&lvcW);

      API_LoadStringW(hLangLib, STR_TEXT, wbuf, BUFFER_SIZE);
      lvcW.cx=70;
      lvcW.iSubItem=LVSI_COLOR_TEXT;
      SendMessage(hWndList, LVM_INSERTCOLUMNW, LVSI_COLOR_TEXT, (LPARAM)&lvcW);

      API_LoadStringW(hLangLib, STR_BACKGROUND, wbuf, BUFFER_SIZE);
      lvcW.cx=70;
      lvcW.iSubItem=LVSI_COLOR_BACKGROUND;
      SendMessage(hWndList, LVM_INSERTCOLUMNW, LVSI_COLOR_BACKGROUND, (LPARAM)&lvcW);

      API_LoadStringW(hLangLib, STR_SAMPLE, wbuf, BUFFER_SIZE);
      lvcW.cx=70;
      lvcW.iSubItem=LVSI_COLOR_SAMPLE;
      SendMessage(hWndList, LVM_INSERTCOLUMNW, LVSI_COLOR_SAMPLE, (LPARAM)&lvcW);
    }

    //Rows
    {
      LVITEMW lviW;
      int i;

      API_LoadStringW(hLangLib, STR_BASIC, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_BASIC;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&lviW);

      API_LoadStringW(hLangLib, STR_SELECTION, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_SELECTION;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&lviW);

      API_LoadStringW(hLangLib, STR_ACTIVELINE, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_ACTIVELINE;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&lviW);

      API_LoadStringW(hLangLib, STR_CARET, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_CARET;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&lviW);

      API_LoadStringW(hLangLib, STR_URL, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_URL;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&lviW);

      //Set "Sample" text
      API_LoadStringW(hLangLib, STR_SAMPLE, wbuf, BUFFER_SIZE);

      for (i=LVI_COLOR_BASIC; i < LVI_COLOR_CARET; ++i)
      {
        lviW.mask=LVIF_TEXT;
        lviW.pszText=wbuf;
        lviW.iItem=i;
        lviW.iSubItem=LVSI_COLOR_SAMPLE;
        SendMessage(hWndList, LVM_SETITEMW, 0, (LPARAM)&lviW);
      }
    }

    //Combobox
    {
      COLORTHEMEW *ctElement;
      int nSelection=-1;

      FillComboboxThemesW(hWndThemeName);

      aecColorsDlg=aecColors;

      if (!aecColorsDlg.dwFlags)
      {
        aecColorsDlg.dwFlags=AECLR_DEFAULT|AECLR_ALL;
        SendMessage(hWndEdit, AEM_GETCOLORS, 0, (LPARAM)&aecColorsDlg);
      }
      if (ctElement=StackThemeGetByColorsW(&hThemesStack, &aecColorsDlg))
        nSelection=SendMessageW(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->wszName);
      SendMessage(hWndThemeName, CB_SETCURSEL, (WPARAM)nSelection, 0);

      if (nSelection <= 0)
      {
        EnableWindow(hWndThemeSave, FALSE);
        EnableWindow(hWndThemeDelete, FALSE);
      }
      else if (nSelection > 0)
      {
        EnableWindow(hWndThemeSave, FALSE);
        EnableWindow(hWndThemeDelete, TRUE);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_COLORS_LIST)
    {
      if ((int)((NMHDR *)lParam)->code == NM_CUSTOMDRAW)
      {
        LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)lParam;
        LRESULT lResult;

        if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
          lResult=CDRF_NOTIFYITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
          lResult=CDRF_NOTIFYSUBITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == (CDDS_SUBITEM|CDDS_ITEMPREPAINT))
        {
          if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_BASIC)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crBasicText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crBasicBk;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrText=aecColorsDlg.crBasicText;
              lplvcd->clrTextBk=aecColorsDlg.crBasicBk;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_SELECTION)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crSelText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crSelBk;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrText=aecColorsDlg.crSelText;
              lplvcd->clrTextBk=aecColorsDlg.crSelBk;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_ACTIVELINE)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveLineText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveLineBk;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrText=aecColorsDlg.crActiveLineText;
              lplvcd->clrTextBk=aecColorsDlg.crActiveLineBk;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_CARET)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crCaret;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crCaret;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crCaret;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_URL)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crUrlText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crUrlText;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crUrlText;
            }
          }
          lResult=CDRF_DODEFAULT;
        }
        else lResult=CDRF_DODEFAULT;

        SetWindowLongW(hDlg, DWL_MSGRESULT, (LONG)lResult);
        return TRUE;
      }
      else if ((int)((NMHDR *)lParam)->code == NM_CLICK)
      {
        LVHITTESTINFO lvhti={0};
        BOOL bResult=FALSE;

        GetCursorPos(&lvhti.pt);
        ScreenToClient(hWndList, &lvhti.pt);
        SendMessage(hWndList, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvhti);

        if (lvhti.iItem >= 0)
        {
          if (lvhti.iSubItem == LVSI_COLOR_TEXT)
          {
            if (lvhti.iItem == LVI_COLOR_BASIC)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crBasicText);
            else if (lvhti.iItem == LVI_COLOR_SELECTION)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crSelText);
            else if (lvhti.iItem == LVI_COLOR_ACTIVELINE)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crActiveLineText);
            else if (lvhti.iItem == LVI_COLOR_CARET)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crCaret);
            else if (lvhti.iItem == LVI_COLOR_URL)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crUrlText);
          }
          else if (lvhti.iSubItem == LVSI_COLOR_BACKGROUND)
          {
            if (lvhti.iItem == LVI_COLOR_BASIC)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crBasicBk);
            else if (lvhti.iItem == LVI_COLOR_SELECTION)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crSelBk);
            else if (lvhti.iItem == LVI_COLOR_ACTIVELINE)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crActiveLineBk);
            else if (lvhti.iItem == LVI_COLOR_CARET)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crCaret);
            else if (lvhti.iItem == LVI_COLOR_URL)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crUrlText);
          }
          if (bResult)
          {
            COLORTHEMEW *ctElement;
            int nSelection=-1;

            if (ctElement=StackThemeGetByColorsW(&hThemesStack, &aecColorsDlg))
              nSelection=SendMessageW(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->wszName);
            SendMessage(hWndThemeName, CB_SETCURSEL, (WPARAM)nSelection, 0);
            if (nSelection > 0)
              EnableWindow(hWndThemeDelete, TRUE);
            else
              EnableWindow(hWndThemeDelete, FALSE);

            bColorsChanged=TRUE;
            InvalidateRect(hWndList, NULL, TRUE);
          }
        }
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_COLORS_THEME_NAME)
    {
      if (HIWORD(wParam) == CBN_EDITCHANGE)
      {
        COLORTHEMEW *ctElement;
        int nSelection=-1;
        int nNameLen;

        if (nNameLen=GetWindowTextW(hWndThemeName, wbuf, BUFFER_SIZE))
        {
          if (ctElement=StackThemeGetByNameW(&hThemesStack, wbuf))
          {
            nSelection=SendMessageW(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->wszName);
          }
        }

        if (nSelection < 0)
        {
          EnableWindow(hWndThemeSave, nNameLen?TRUE:FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection == 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection > 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, TRUE);
        }
      }
      else if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        COLORTHEMEW *ctElement;
        int nSelection;

        if ((nSelection=SendMessage(hWndThemeName, CB_GETCURSEL, 0, 0)) != CB_ERR)
        {
          if (SendMessageW(hWndThemeName, CB_GETLBTEXT, (WPARAM)nSelection, (LPARAM)wbuf))
          {
            if (ctElement=StackThemeGetByNameW(&hThemesStack, wbuf))
            {
              aecColorsDlg=ctElement->aec;
              bColorsChanged=TRUE;
              InvalidateRect(hWndList, NULL, TRUE);
            }
          }
        }

        if (nSelection < 0)
        {
          EnableWindow(hWndThemeSave, TRUE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection == 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else if (nSelection > 0)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_COLORS_THEME_SAVE)
    {
      if (GetWindowTextW(hWndThemeName, wbuf, BUFFER_SIZE))
      {
        StackThemeAddW(&hThemesStack, wbuf, &aecColorsDlg);
        SendMessageW(hWndThemeName, CB_ADDSTRING, 0, (LPARAM)wbuf);

        EnableWindow(hWndThemeSave, FALSE);
        EnableWindow(hWndThemeDelete, TRUE);
      }
    }
    else if (LOWORD(wParam) == IDC_COLORS_THEME_DELETE)
    {
      if (GetWindowTextW(hWndThemeName, wbuf, BUFFER_SIZE))
      {
        COLORTHEMEW *ctElement;
        int nSelection;

        if (ctElement=StackThemeGetByNameW(&hThemesStack, wbuf))
        {
          StackThemeDeleteW(&hThemesStack, ctElement);
          nSelection=SendMessageW(hWndThemeName, CB_FINDSTRING, (WPARAM)-1, (LPARAM)ctElement->wszName);
          SendMessageW(hWndThemeName, CB_DELETESTRING, nSelection, 0);
          SendMessage(hWndThemeName, CB_SETCURSEL, (WPARAM)-1, 0);

          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (bColorsChanged)
      {
        aecColors=aecColorsDlg;
        aecColors.dwFlags=AECLR_ALL;
        SendMessage(hWndEdit, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
      }
      SaveThemesW(TRUE);

      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  return FALSE;
}

void FillComboboxThemesA(HWND hWnd)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hThemesStack.last;

  while (ctElement)
  {
    SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)ctElement->szName);

    ctElement=ctElement->prev;
  }
}

void FillComboboxThemesW(HWND hWnd)
{
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hThemesStack.last;

  while (ctElement)
  {
    SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)ctElement->wszName);

    ctElement=ctElement->prev;
  }
}


//// Plugins

BOOL CALLBACK PluginsDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndAssign;
  static HWND hWndCall;
  static int nSelItem;
  static BOOL bListChanged;

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNA lvcA;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndList=GetDlgItem(hDlg, IDC_PLUGINS_LIST);
    hWndHotkey=GetDlgItem(hDlg, IDC_PLUGINS_HOTKEY);
    hWndAssign=GetDlgItem(hDlg, IDC_PLUGINS_ASSIGN);
    hWndCall=GetDlgItem(hDlg, IDC_PLUGINS_CALL);
    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    EnableWindow(hWndHotkey, FALSE);
    EnableWindow(hWndAssign, FALSE);
    EnableWindow(hWndCall, FALSE);

    //Columns
    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=buf;

    API_LoadStringA(hLangLib, STR_PLUGIN_FUNCTION, buf, BUFFER_SIZE);
    lvcA.cx=210;
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

    hHookPlugins=SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetCurrentThreadId());
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
      PLUGINLISTITEMA *pliElement=(PLUGINLISTITEMA *)hPluginListStack.last;

      while (pliElement)
      {
        pliElement->pf->wHotkey=pliElement->wInitialHotkey;
        pliElement->pf->bOnStart=pliElement->bInitialOnStart;

        pliElement=pliElement->prev;
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
                    API_LoadStringA(hLangLib, MSG_AUTOLOAD_DOES_NOT_SUPPORTED, buf, BUFFER_SIZE);
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
  else if (uMsg == WM_DESTROY)
  {
    if (hHookPlugins)
    {
      if (UnhookWindowsHookEx(hHookPlugins))
        hHookPlugins=NULL;
    }
    hWndHotkey=NULL;
  }
  return FALSE;
}

BOOL CALLBACK PluginsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndAssign;
  static HWND hWndCall;
  static int nSelItem;
  static BOOL bListChanged;

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvcW;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndList=GetDlgItem(hDlg, IDC_PLUGINS_LIST);
    hWndHotkey=GetDlgItem(hDlg, IDC_PLUGINS_HOTKEY);
    hWndAssign=GetDlgItem(hDlg, IDC_PLUGINS_ASSIGN);
    hWndCall=GetDlgItem(hDlg, IDC_PLUGINS_CALL);
    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    EnableWindow(hWndHotkey, FALSE);
    EnableWindow(hWndAssign, FALSE);
    EnableWindow(hWndCall, FALSE);

    //Columns
    lvcW.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcW.pszText=wbuf;

    API_LoadStringW(hLangLib, STR_PLUGIN_FUNCTION, wbuf, BUFFER_SIZE);
    lvcW.cx=210;
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

    hHookPlugins=SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetCurrentThreadId());
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
        wsprintfW(wbuf2, wbuf, pfElement->wszFunction);
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
            lstrcpynW(pcp->wszFunction, pliElement->pf->wszFunction, MAX_PATH);
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
      PLUGINLISTITEMW *pliElement=(PLUGINLISTITEMW *)hPluginListStack.last;

      while (pliElement)
      {
        pliElement->pf->wHotkey=pliElement->wInitialHotkey;
        pliElement->pf->bOnStart=pliElement->bInitialOnStart;

        pliElement=pliElement->prev;
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
                    CallPluginW(NULL, pliElement->pf->wszFunction, FALSE, 0, &pliElement->nAutoLoad);
                  }
                  if (pliElement->nAutoLoad == 0)
                  {
                    pliElement->pf->bOnStart=FALSE;
                    API_LoadStringW(hLangLib, MSG_AUTOLOAD_DOES_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
                    wsprintfW(wbuf2, wbuf, pliElement->pf->wszFunction);
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
  else if (uMsg == WM_DESTROY)
  {
    if (hHookPlugins)
    {
      if (UnhookWindowsHookEx(hHookPlugins))
        hHookPlugins=NULL;
    }
    hWndHotkey=NULL;
  }
  return FALSE;
}

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    MSG *msg=(MSG *)lParam;

    if (hWndHotkey && hWndHotkey == msg->hwnd)
    {
      if (msg->message >= WM_KEYFIRST && msg->message <= WM_KEYLAST)
      {
        if (msg->wParam == VK_SPACE ||
            msg->wParam == VK_RETURN ||
            msg->wParam == VK_BACK ||
            msg->wParam == VK_ESCAPE)
        {
          if (msg->message == WM_KEYDOWN ||
              msg->message == WM_SYSKEYDOWN)
          {
            BYTE nMod=0;

            if ((msg->lParam >> 24) & 1) nMod|=HOTKEYF_EXT;
            if (GetKeyState(VK_CONTROL) & 0x80) nMod|=HOTKEYF_CONTROL;
            if (GetKeyState(VK_MENU) & 0x80) nMod|=HOTKEYF_ALT;
            if (GetKeyState(VK_SHIFT) & 0x80) nMod|=HOTKEYF_SHIFT;
            SendMessage(hWndHotkey, HKM_SETHOTKEY, MAKEWORD(msg->wParam, nMod), 0);
          }
          msg->message=WM_NULL;
        }
      }
    }
  }
  return CallNextHookEx(hHookPlugins, code, wParam, lParam);
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
        GetBaseNameA(wfdA.cFileName, szBaseName, MAX_PATH);
        pld.pBaseName=(unsigned char *)szBaseName;
        GetExportNames(hInstance, FillPluginListProcA, (LPARAM)&pld);
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
        GetBaseNameW(wfdW.cFileName, wszBaseName, MAX_PATH);
        pld.pBaseName=(unsigned char *)wszBaseName;
        GetExportNames(hInstance, FillPluginListProcW, (LPARAM)&pld);
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

    if (!StackInsert((stack **)&hPluginListStack.first, (stack **)&hPluginListStack.last, (stack **)&pliElement, nIndex + 1, sizeof(PLUGINLISTITEMA)))
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

    if (!StackInsert((stack **)&hPluginListStack.first, (stack **)&hPluginListStack.last, (stack **)&pliElement, nIndex + 1, sizeof(PLUGINLISTITEMW)))
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

  if (!StackElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&pliElement, nIndex))
    return pliElement;
  return NULL;
}

PLUGINLISTITEMW* GetPluginListItemW(HSTACK *hStack, int nIndex)
{
  PLUGINLISTITEMW *pliElement;

  if (!StackElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&pliElement, nIndex))
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
  HMODULE hModule;
  PLUGINDATA pd;
  BOOL bActive=TRUE;
  BOOL bCalled=FALSE;
  void (*PluginFunctionPtr)(PLUGINDATA *);

  if (lpbAutoLoad) *lpbAutoLoad=TRUE;

  if (ParsePluginNameA(pFullName, szPlugin, szFunction))
  {
    wsprintfA(szDLL, "%s\\AkelFiles\\Plugs\\%s.dll", szExeDir, szPlugin);

    if (!(hModule=GetModuleHandleA(szDLL)))
    {
      if (hModule=LoadLibraryA(szDLL))
      {
        StackHandleIncrease(&hHandlesStack, hModule);
        bActive=FALSE;
      }
    }

    if (hModule)
    {
      if (PluginFunctionPtr=(void (*)(PLUGINDATA *))GetProcAddress(hModule, szFunction))
      {
        pd.cb=sizeof(PLUGINDATA);
        pd.pFunction=(unsigned char *)pFullName;
        pd.hInstanceDLL=hModule;
        pd.lpPluginFunction=lpPluginFunction;
        pd.lpbAutoLoad=lpbAutoLoad;
        pd.nUnload=UD_UNLOAD;
        pd.bActive=bActive;
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
        pd.lpReserved=NULL;
        pd.bOldWindows=bOldWindows;
        pd.bOldRichEdit=bOldRichEdit;
        pd.bOldComctl32=bOldComctl32;
        pd.bAkelEdit=TRUE;
        pd.bMDI=bMDI;
        pd.nSaveSettings=nSaveSettings;
        pd.pLangModule=(unsigned char *)szLangModule;
        pd.wLangSystem=(WORD)dwLangSystem;
        pd.hGlobalAccel=hGlobalAccel;
        pd.hMainAccel=hMainAccel;

        (*PluginFunctionPtr)(&pd);
        SendMessage(hMainWnd, AKDN_DLLCALL, 0, (LPARAM)&pd);

        if (lpbAutoLoad && bActive) return EDL_NONUNLOADED;
        if (pd.nUnload == UD_NONUNLOAD) return EDL_NONUNLOADED;
        if (pd.nUnload == UD_NONUNLOAD_ACTIVE) return EDL_NONUNLOADED_ACTIVE;
        if (pd.nUnload == UD_NONUNLOAD_NONACTIVE) return EDL_NONUNLOADED_NONACTIVE;
        bCalled=TRUE;
      }
      else
      {
        API_LoadStringA(hLangLib, MSG_ERROR_FUNCTION_NOT_FOUND, buf, BUFFER_SIZE);
        wsprintfA(buf2, buf, szPlugin, szFunction, szDLL);
        MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
        if (bActive) return EDL_FAILED;
      }

      if (FreeLibrary(hModule))
      {
        StackHandleDecrease(&hHandlesStack, hModule);
        if (bCalled) return EDL_UNLOADED;
      }
    }
    else
    {
      API_LoadStringA(hLangLib, MSG_ERROR_CANNOT_OPEN_FILE, buf, BUFFER_SIZE);
      wsprintfA(buf2, buf, szDLL);
      MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
    }
  }
  return EDL_FAILED;
}

int CallPluginW(PLUGINFUNCTIONW *lpPluginFunction, wchar_t *wpFullName, BOOL bOnStart, LPARAM lParam, BOOL *lpbAutoLoad)
{
  wchar_t wszPlugin[MAX_PATH];
  wchar_t wszFunction[MAX_PATH];
  wchar_t wszDLL[MAX_PATH];
  char szFunction[MAX_PATH];
  HMODULE hModule;
  PLUGINDATA pd;
  BOOL bActive=TRUE;
  BOOL bCalled=FALSE;
  void (*PluginFunctionPtr)(PLUGINDATA *);

  if (lpbAutoLoad) *lpbAutoLoad=TRUE;

  if (ParsePluginNameW(wpFullName, wszPlugin, wszFunction))
  {
    WideCharToMultiByte(CP_ACP, 0, wszFunction, -1, szFunction, MAX_PATH, NULL, NULL);
    wsprintfW(wszDLL, L"%s\\AkelFiles\\Plugs\\%s.dll", wszExeDir, wszPlugin);

    if (!(hModule=GetModuleHandleW(wszDLL)))
    {
      if (hModule=LoadLibraryW(wszDLL))
      {
        StackHandleIncrease(&hHandlesStack, hModule);
        bActive=FALSE;
      }
    }

    if (hModule)
    {
      if (PluginFunctionPtr=(void (*)(PLUGINDATA *))GetProcAddress(hModule, szFunction))
      {
        pd.cb=sizeof(PLUGINDATA);
        pd.pFunction=(unsigned char *)wpFullName;
        pd.hInstanceDLL=hModule;
        pd.lpPluginFunction=lpPluginFunction;
        pd.lpbAutoLoad=lpbAutoLoad;
        pd.nUnload=UD_UNLOAD;
        pd.bActive=bActive;
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
        pd.lpReserved=NULL;
        pd.bOldWindows=bOldWindows;
        pd.bOldRichEdit=bOldRichEdit;
        pd.bOldComctl32=bOldComctl32;
        pd.bAkelEdit=TRUE;
        pd.bMDI=bMDI;
        pd.nSaveSettings=nSaveSettings;
        pd.pLangModule=(unsigned char *)wszLangModule;
        pd.wLangSystem=(WORD)dwLangSystem;
        pd.hGlobalAccel=hGlobalAccel;
        pd.hMainAccel=hMainAccel;

        (*PluginFunctionPtr)(&pd);
        SendMessage(hMainWnd, AKDN_DLLCALL, 0, (LPARAM)&pd);

        if (lpbAutoLoad && bActive) return EDL_NONUNLOADED;
        if (pd.nUnload == UD_NONUNLOAD) return EDL_NONUNLOADED;
        if (pd.nUnload == UD_NONUNLOAD_ACTIVE) return EDL_NONUNLOADED_ACTIVE;
        if (pd.nUnload == UD_NONUNLOAD_NONACTIVE) return EDL_NONUNLOADED_NONACTIVE;
        bCalled=TRUE;
      }
      else
      {
        API_LoadStringW(hLangLib, MSG_ERROR_FUNCTION_NOT_FOUND, wbuf, BUFFER_SIZE);
        wsprintfW(wbuf2, wbuf, wszPlugin, wszFunction, wszDLL);
        MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
        if (bActive) return EDL_FAILED;
      }

      if (FreeLibrary(hModule))
      {
        StackHandleDecrease(&hHandlesStack, hModule);
        if (bCalled) return EDL_UNLOADED;
      }
    }
    else
    {
      API_LoadStringW(hLangLib, MSG_ERROR_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
      wsprintfW(wbuf2, wbuf, wszDLL);
      MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
    }
  }
  return EDL_FAILED;
}

void CallPluginsOnStartA(HSTACK *hStack)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->last;
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

      if (CallPluginReceiveSendA(&pcs) == EDL_FAILED)
      {
        pfTmp=pfElement->prev;
        StackPluginDelete(hStack, pfElement);
        pfElement=pfTmp;
        continue;
      }
    }
    pfElement=pfElement->prev;
  }
}

void CallPluginsOnStartW(HSTACK *hStack)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->last;
  PLUGINFUNCTIONW *pfTmp;
  PLUGINCALLSENDW pcs;

  while (pfElement)
  {
    if (pfElement->bOnStart)
    {
      pcs.wpFunction=pfElement->wszFunction;
      pcs.bOnStart=TRUE;
      pcs.lParam=0;
      pcs.lpbAutoLoad=NULL;

      if (CallPluginReceiveSendW(&pcs) == EDL_FAILED)
      {
        pfTmp=pfElement->prev;
        StackPluginDelete(hStack, pfElement);
        pfElement=pfTmp;
        continue;
      }
    }
    pfElement=pfElement->prev;
  }
}

int CallPluginReceiveSendA(PLUGINCALLSENDA *pcs)
{
  PLUGINFUNCTIONA *pfElement;
  int nResult=EDL_FAILED;

  if (pcs)
  {
    pfElement=StackPluginFindA(&hPluginsStack, pcs->pFunction);

    if (pfElement && pfElement->PluginProc)
    {
      (pfElement->PluginProc)(pfElement->lpParameter);
    }
    else
    {
      nResult=CallPluginA(pfElement, pcs->pFunction, pcs->bOnStart, pcs->lParam, pcs->lpbAutoLoad);

      if (nResult == EDL_NONUNLOADED_ACTIVE)
      {
        if (pfElement)
          pfElement->bRunning=TRUE;
        else
          StackPluginAddA(&hPluginsStack, pcs->pFunction, lstrlenA(pcs->pFunction), 0, FALSE, TRUE, NULL, NULL);
      }
      else if (nResult != EDL_NONUNLOADED)
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
  return nResult;
}

int CallPluginReceiveSendW(PLUGINCALLSENDW *pcs)
{
  PLUGINFUNCTIONW *pfElement;
  int nResult=EDL_FAILED;

  if (pcs)
  {
    pfElement=StackPluginFindW(&hPluginsStack, pcs->wpFunction);

    if (pfElement && pfElement->PluginProc)
    {
      (pfElement->PluginProc)(pfElement->lpParameter);
    }
    else
    {
      nResult=CallPluginW(pfElement, pcs->wpFunction, pcs->bOnStart, pcs->lParam, pcs->lpbAutoLoad);

      if (nResult == EDL_NONUNLOADED_ACTIVE)
      {
        if (pfElement)
          pfElement->bRunning=TRUE;
        else
          StackPluginAddW(&hPluginsStack, pcs->wpFunction, lstrlenW(pcs->wpFunction), 0, FALSE, TRUE, NULL, NULL);
      }
      else if (nResult != EDL_NONUNLOADED)
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
  return nResult;
}

void CallPluginReceivePostA(PLUGINCALLPOSTA *pcp)
{
  PLUGINFUNCTIONA *pfElement;
  int nResult=EDL_FAILED;

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

      if (nResult == EDL_NONUNLOADED_ACTIVE)
      {
        if (pfElement)
          pfElement->bRunning=TRUE;
        else
          StackPluginAddA(&hPluginsStack, pcp->szFunction, lstrlenA(pcp->szFunction), 0, FALSE, TRUE, NULL, NULL);
      }
      else if (nResult != EDL_NONUNLOADED)
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
    GlobalFree((HGLOBAL)pcp);
  }
}

void CallPluginReceivePostW(PLUGINCALLPOSTW *pcp)
{
  PLUGINFUNCTIONW *pfElement;
  int nResult=EDL_FAILED;

  if (pcp)
  {
    pfElement=StackPluginFindW(&hPluginsStack, pcp->wszFunction);

    if (pfElement && pfElement->PluginProc)
    {
      (pfElement->PluginProc)(pfElement->lpParameter);
    }
    else
    {
      nResult=CallPluginW(pfElement, pcp->wszFunction, pcp->bOnStart, pcp->lParam, NULL);

      if (nResult == EDL_NONUNLOADED_ACTIVE)
      {
        if (pfElement)
          pfElement->bRunning=TRUE;
        else
          StackPluginAddW(&hPluginsStack, pcp->wszFunction, lstrlenW(pcp->wszFunction), 0, FALSE, TRUE, NULL, NULL);
      }
      else if (nResult != EDL_NONUNLOADED)
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
  return StackElement((stack *)hStack->first, (stack *)hStack->last, (stack **)ProcData, nIndex);
}

int StackProcSet(HSTACK *hStack, WNDPROC NewProc, WNDPROCDATA **NewProcData, WNDPROC *FirstProc)
{
  if (!NewProcData || !*NewProcData)
  {
    WNDPROCDATA *lpElement;

    if (!StackInsert((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, 1, sizeof(WNDPROCDATA)))
    {
      lpElement->CurProc=NewProc;
      if (lpElement->prev)
      {
        lpElement->prev->PrevProc=NewProc;
        lpElement->NextProc=lpElement->prev->CurProc;
      }
      if (NewProcData) *NewProcData=lpElement;
    }
  }
  else
  {
    if (!NewProc)
    {
      if ((*NewProcData)->next) (*NewProcData)->next->NextProc=(*NewProcData)->NextProc;
      if ((*NewProcData)->prev) (*NewProcData)->prev->PrevProc=(*NewProcData)->PrevProc;
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)*NewProcData);
      *NewProcData=NULL;
    }
    else
    {
      (*NewProcData)->CurProc=NewProc;
      if ((*NewProcData)->next) (*NewProcData)->next->NextProc=NewProc;
      if ((*NewProcData)->prev) (*NewProcData)->prev->PrevProc=NewProc;
    }
  }
  if (FirstProc)
  {
    if (hStack->last)
      *FirstProc=((WNDPROCDATA *)(hStack->last))->CurProc;
    else
      *FirstProc=NULL;
  }
  return 0;
}

void StackProcFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

COLORTHEMEA* StackThemeAddA(HSTACK *hStack, char *pName, AECOLORS *aec)
{
  COLORTHEMEA *ctElement;

  if (!StackInsert((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&ctElement, -1, sizeof(COLORTHEMEA)))
  {
    lstrcpynA(ctElement->szName, pName, MAX_PATH);
    ctElement->nNameLen=lstrlenA(ctElement->szName);
    if (aec) ctElement->aec=*aec;
  }
  return ctElement;
}

COLORTHEMEW* StackThemeAddW(HSTACK *hStack, wchar_t *wpName, AECOLORS *aec)
{
  COLORTHEMEW *ctElement;

  if (!StackInsert((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&ctElement, -1, sizeof(COLORTHEMEW)))
  {
    lstrcpynW(ctElement->wszName, wpName, MAX_PATH);
    ctElement->nNameLen=lstrlenW(ctElement->wszName);
    if (aec) ctElement->aec=*aec;
  }
  return ctElement;
}

COLORTHEMEA* StackThemeGetByNameA(HSTACK *hStack, char *pName)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hStack->last;
  int nNameLen=lstrlenA(pName);

  while (ctElement)
  {
    if (ctElement->nNameLen == nNameLen)
      if (!lstrcmpiA(ctElement->szName, pName))
        return ctElement;

    ctElement=ctElement->prev;
  }
  return NULL;
}

COLORTHEMEW* StackThemeGetByNameW(HSTACK *hStack, wchar_t *wpName)
{
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hStack->last;
  int nNameLen=lstrlenW(wpName);

  while (ctElement)
  {
    if (ctElement->nNameLen == nNameLen)
      if (!lstrcmpiW(ctElement->wszName, wpName))
        return ctElement;

    ctElement=ctElement->prev;
  }
  return NULL;
}

COLORTHEMEA* StackThemeGetByColorsA(HSTACK *hStack, AECOLORS *aec)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hStack->last;

  while (ctElement)
  {
    if (!memcmp(&ctElement->aec.crCaret, &aec->crCaret, sizeof(AECOLORS) - sizeof(DWORD)))
      return ctElement;

    ctElement=ctElement->prev;
  }
  return NULL;
}

COLORTHEMEW* StackThemeGetByColorsW(HSTACK *hStack, AECOLORS *aec)
{
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hStack->last;

  while (ctElement)
  {
    if (!memcmp(&ctElement->aec.crCaret, &aec->crCaret, sizeof(AECOLORS) - sizeof(DWORD)))
      return ctElement;

    ctElement=ctElement->prev;
  }
  return NULL;
}

void StackThemeDeleteA(HSTACK *hStack, COLORTHEMEA *ctElement)
{
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)ctElement);
}

void StackThemeDeleteW(HSTACK *hStack, COLORTHEMEW *ctElement)
{
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)ctElement);
}

void StackThemeFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

PLUGINHANDLE* StackHandleIncrease(HSTACK *hStack, HMODULE hModule)
{
  PLUGINHANDLE *phElement=(PLUGINHANDLE *)hStack->last;

  while (phElement)
  {
    if (phElement->hModule == hModule)
    {
      phElement->nCount+=1;
      return phElement;
    }
    phElement=phElement->prev;
  }

  if (!StackInsert((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&phElement, 1, sizeof(PLUGINHANDLE)))
  {
    phElement->hModule=hModule;
    phElement->nCount=1;
  }
  return phElement;
}

PLUGINHANDLE* StackHandleDecrease(HSTACK *hStack, HMODULE hModule)
{
  PLUGINHANDLE *phElement=(PLUGINHANDLE *)hStack->last;

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
    phElement=phElement->prev;
  }
  return NULL;
}

void StackHandleFree(HSTACK *hStack)
{
  PLUGINHANDLE *phElement=(PLUGINHANDLE *)hStack->last;

  while (phElement)
  {
    while (phElement->nCount > 0 && FreeLibrary(phElement->hModule))
    {
      phElement->nCount-=1;
    }
    phElement=phElement->prev;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

DOCK* StackDockAdd(HDOCK *hDocks, DOCK *dkData)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.last;
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

    dkElement=dkElement->prev;
  }

  if (!StackInsert((stack **)&hDocks->hStack.first, (stack **)&hDocks->hStack.last, (stack **)&dkElement, nIndex, sizeof(DOCK)))
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
  DOCK *dkElement=(DOCK *)hDocks->hStack.last;
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

    dkElement=dkElement->prev;
  }
  dkData->nSide=nSide;

  return StackMove((stack **)&hDocks->hStack.first, (stack **)&hDocks->hStack.last, (stack *)dkData, nIndex);
}

DOCK* StackDockFindWindow(HDOCK *hDocks, HWND hWnd, BOOL bChild)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.last;

  if (hWnd)
  {
    while (dkElement)
    {
      if (dkElement->hWnd)
      {
        if (dkElement->hWnd == hWnd || (bChild && IsChild(dkElement->hWnd, hWnd)))
          return dkElement;
      }
      dkElement=dkElement->prev;
    }
  }
  return NULL;
}

HWND StackDockNextWindow(HDOCK *hDocks, DOCK *dkData, BOOL bPrevious)
{
  if (!bPrevious)
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
  DOCK *dkElement=(DOCK *)hDocks->hStack.last;
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
    dkElement=dkElement->prev;
  }
  return NULL;
}

void StackDockSize(HDOCK *hDocks, int nSide, NSIZE *ns)
{
  DOCK *dkElement=(DOCK *)hDocks->hStack.last;
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

    dkElement=dkElement->prev;
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
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->last;
  int nStringLen;

  if (!(nStringLen=lstrlenA(pString)))
    return NULL;

  while (pfElement)
  {
    if (pfElement->nFunctionLen == nStringLen)
    {
      if (!lstrcmpiA(pfElement->szFunction, pString))
        break;
    }
    pfElement=pfElement->prev;
  }
  return pfElement;
}

PLUGINFUNCTIONW* StackPluginFindW(HSTACK *hStack, wchar_t *wpString)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->last;
  int nStringLen;

  if (!(nStringLen=lstrlenW(wpString)))
    return NULL;

  while (pfElement)
  {
    if (pfElement->nFunctionLen == nStringLen)
    {
      if (!lstrcmpiW(pfElement->wszFunction, wpString))
        break;
    }
    pfElement=pfElement->prev;
  }
  return pfElement;
}

PLUGINFUNCTIONA* StackHotkeyFindA(HSTACK *hStack, WORD wHotkey)
{
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->last;

  while (pfElement)
  {
    if (pfElement->wHotkey == wHotkey)
        break;
    pfElement=pfElement->prev;
  }
  return pfElement;
}

PLUGINFUNCTIONW* StackHotkeyFindW(HSTACK *hStack, WORD wHotkey)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->last;

  while (pfElement)
  {
    if (pfElement->wHotkey == wHotkey)
        break;
    pfElement=pfElement->prev;
  }
  return pfElement;
}

PLUGINFUNCTIONA* StackPluginAddA(HSTACK *hStack, char *pString, int nStringLen, WORD wHotkey, BOOL bOnStart, BOOL bRunning, PLUGINPROC PluginProc, void *lpParameter)
{
  PLUGINFUNCTIONA *pfElement;

  if (!StackInsert((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&pfElement, -1, sizeof(PLUGINFUNCTIONA)))
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

  if (!StackInsert((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&pfElement, -1, sizeof(PLUGINFUNCTIONW)))
  {
    lstrcpynW(pfElement->wszFunction, wpString, MAX_PATH);
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
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->last;
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
    if (!CreateIniA(&hIniStack, szIniFile))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionA(&hIniStack, "Plugs", lstrlenA("Plugs")))
        StackFreeIniSection(&lpIniSection->hSectionStack);
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
    pfElement=pfElement->prev;
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
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->last;
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
    if (!CreateIniW(&hIniStack, wszIniFile))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionW(&hIniStack, L"Plugs", lstrlenW(L"Plugs")))
        StackFreeIniSection(&lpIniSection->hSectionStack);
    }
  }

  while (pfElement)
  {
    if (IsMainFunctionW(pfElement->wszFunction))
    {
      if (!bCleanOld || (pfElement->wHotkey || pfElement->bOnStart))
      {
        dwHotkey=MAKELONG(pfElement->wHotkey, pfElement->bOnStart);

        if (nSaveSettings == SS_REGISTRY)
        {
          if (RegSetValueExW(hKey, pfElement->wszFunction, 0, REG_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)) != ERROR_SUCCESS)
            break;
        }
        else
        {
          if (!IniSetValueW(&hIniStack, L"Plugs", pfElement->wszFunction, INI_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)))
            break;
        }
      }
    }
    pfElement=pfElement->prev;
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
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->last;
  PLUGINFUNCTIONA *pfElement2;
  char szDLL[MAX_PATH];
  char szPlugin[MAX_PATH];

  while (pfElement)
  {
    pfElement2=pfElement->prev;

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
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->last;
  PLUGINFUNCTIONW *pfElement2;
  wchar_t wszDLL[MAX_PATH];
  wchar_t wszPlugin[MAX_PATH];

  while (pfElement)
  {
    pfElement2=pfElement->prev;

    if (!pfElement->wHotkey && !pfElement->bOnStart && !pfElement->bRunning)
    {
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)pfElement);
      pfElement=NULL;
    }
    if (bDeleteNonExistentDLL && pfElement)
    {
      if (ParsePluginNameW(pfElement->wszFunction, wszPlugin, NULL))
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
    DOCK *dkElement=(DOCK *)hDocks->hStack.last;

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
      dkElement=dkElement->prev;
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
    DOCK *dkElement=(DOCK *)hDocks->hStack.last;

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
      dkElement=dkElement->prev;
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
            PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hPluginsStack.last;
            PLUGINFUNCTIONA *pfElement2;

            while (pfElement)
            {
              if (!xstrcmpnA(szPluginName, pfElement->szFunction, (DWORD)-1, FALSE))
              {
                if (pfElement->wHotkey || pfElement->bOnStart)
                  pfElement->bRunning=FALSE;
                else
                {
                  pfElement2=pfElement->prev;
                  StackPluginDelete(&hPluginsStack, pfElement);
                  pfElement=pfElement2;
                  continue;
                }
              }
              pfElement=pfElement->prev;
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
            PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hPluginsStack.last;
            PLUGINFUNCTIONW *pfElement2;

            while (pfElement)
            {
              if (!xstrcmpnW(wszPluginName, pfElement->wszFunction, (DWORD)-1, FALSE))
              {
                if (pfElement->wHotkey || pfElement->bOnStart)
                  pfElement->bRunning=FALSE;
                else
                {
                  pfElement2=pfElement->prev;
                  StackPluginDelete(&hPluginsStack, pfElement);
                  pfElement=pfElement2;
                  continue;
                }
              }
              pfElement=pfElement->prev;
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
  PLUGINFUNCTIONA *pfElement=(PLUGINFUNCTIONA *)hStack->last;
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
          (pfElement->PluginProc)(pfElement->lpParameter);
        }
        else
        {
          int nResult=EDL_FAILED;

          nResult=CallPluginA(pfElement, pfElement->szFunction, FALSE, 0, NULL);

          if (nResult == EDL_NONUNLOADED_ACTIVE)
            pfElement->bRunning=TRUE;
          else if (nResult != EDL_NONUNLOADED)
            pfElement->bRunning=FALSE;
        }
        return TRUE;
      }
      pfElement=pfElement->prev;
    }
    bResult=FALSE;
    SendMessage(hMainWnd, AKDN_HOTKEY, (WPARAM)wHotkey, (LPARAM)&bResult);
    return bResult;
  }
  return FALSE;
}

BOOL TranslateHotkeyW(HSTACK *hStack, LPMSG lpMsg)
{
  PLUGINFUNCTIONW *pfElement=(PLUGINFUNCTIONW *)hStack->last;
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
          (pfElement->PluginProc)(pfElement->lpParameter);
        }
        else
        {
          int nResult=EDL_FAILED;

          nResult=CallPluginW(pfElement, pfElement->wszFunction, FALSE, 0, NULL);

          if (nResult == EDL_NONUNLOADED_ACTIVE)
            pfElement->bRunning=TRUE;
          else if (nResult != EDL_NONUNLOADED)
            pfElement->bRunning=FALSE;
        }
        return TRUE;
      }
      pfElement=pfElement->prev;
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

  lstrcpynW(od.wszFile, wpFile, MAX_PATH);

  if (bOtherProcess)
    GetCurrentDirectoryW(MAX_PATH, od.wszWorkDir);
  else
    od.wszWorkDir[0]='\0';

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
    API_LoadStringA(hLangLib, STR_AUTODETECT_ENGLISH, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);
    API_LoadStringA(hLangLib, STR_AUTODETECT_RUSSIAN, buf, BUFFER_SIZE);
    SendMessageA(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)buf);

    if (dwLangCodepageRecognition == LANGID_ENGLISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 1, 0);
    else if (dwLangCodepageRecognition == LANGID_RUSSIAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 2, 0);
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
          pMalloc->Free(pIdList);
          pMalloc->Release();
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
        dwLangCodepageRecognition=LANGID_ENGLISH;
      else if (i == 2)
        dwLangCodepageRecognition=LANGID_RUSSIAN;

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
    API_LoadStringW(hLangLib, STR_AUTODETECT_ENGLISH, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);
    API_LoadStringW(hLangLib, STR_AUTODETECT_RUSSIAN, wbuf, BUFFER_SIZE);
    SendMessageW(hWndAutodetectCP, CB_ADDSTRING, 0, (LPARAM)wbuf);

    if (dwLangCodepageRecognition == LANGID_ENGLISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 1, 0);
    else if (dwLangCodepageRecognition == LANGID_RUSSIAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 2, 0);
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
          pMalloc->Free(pIdList);
          pMalloc->Release();
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
        dwLangCodepageRecognition=LANGID_ENGLISH;
      else if (i == 2)
        dwLangCodepageRecognition=LANGID_RUSSIAN;

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
  static HWND hWndWordDelimitersEnable;
  static HWND hWndWordDelimiters;
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
    hWndWordDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS_ENABLE);
    hWndWordDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS);

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
    SendMessage(hWndWordDelimiters, EM_LIMITTEXT, (WPARAM)WORD_DELIMITERS_SIZE, 0);

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
      SendMessage(hWndWordDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWordDelimiters, bWordDelimitersEnable);
    EscapeDataToEscapeStringW(wszWordDelimiters, (wchar_t *)buf);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t *)buf, -1, buf2, BUFFER_SIZE, NULL, NULL);
    SetWindowTextA(hWndWordDelimiters, buf2);
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
      bState=SendMessage(hWndWordDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndWordDelimiters, bState);
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
        SetTabStops(hWndEdit, nTabStopSize, TRUE);
      }
      bTabStopAsSpaces=SendMessage(hWndTabSizeSpaces, BM_GETCHECK, 0, 0);

      //Undo
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, NULL, FALSE);
      if (nUndoLimit != a)
      {
        nUndoLimit=a;
        SendMessage(hWndEdit, AEM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
      }
      bDetailedUndo=SendMessage(hWndDetailedUndo, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bDetailedUndo?AECOOP_OR:AECOOP_XOR, AECO_DETAILEDUNDO);

      //Margins
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, NULL, FALSE);
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, NULL, FALSE);
      if (dwEditMargins != (DWORD)MAKELONG(a, b))
      {
        dwEditMargins=MAKELONG(a, b);
        SendMessage(hWndEdit, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
        InvalidateRect(hWndEdit, NULL, TRUE);
      }

      //Word delimiters
      a=GetWindowTextA(hWndWordDelimiters, buf, BUFFER_SIZE);
      MultiByteToWideChar(CP_ACP, 0, buf, a + 1, (wchar_t *)buf2, BUFFER_SIZE / sizeof(wchar_t));
      EscapeStringToEscapeDataW((wchar_t *)buf2, wszWordDelimiters);

      bWordDelimitersEnable=SendMessage(hWndWordDelimitersEnable, BM_GETCHECK, 0, 0);
      if (bWordDelimitersEnable)
      {
        SendMessage(hWndEdit, AEM_SETWORDBREAK, dwCustomWordBreak, 0);
        SendMessage(hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)wszWordDelimiters);
      }
      else
      {
        SendMessage(hWndEdit, AEM_SETWORDBREAK, dwDefaultWordBreak, 0);
        SendMessage(hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)NULL);
      }

      //Save settings
      if (SendMessage(hWndSaveRegistry, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=SS_REGISTRY;
      else
        a=SS_INI;
      if (nSaveSettings != a)
      {
        nSaveSettings=a;

        bEditFontChanged=TRUE;
        bColorsChanged=TRUE;
        bPrintFontChanged=TRUE;
        IniSaveOptionsA();
        RegSaveOptionsA();
        SaveThemesA(TRUE);
        StackPluginSaveA(&hPluginsStack, TRUE);
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
  static HWND hWndWordDelimitersEnable;
  static HWND hWndWordDelimiters;
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
    hWndWordDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS_ENABLE);
    hWndWordDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS);

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
    SendMessage(hWndWordDelimiters, EM_LIMITTEXT, (WPARAM)WORD_DELIMITERS_SIZE, 0);

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
      SendMessage(hWndWordDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWordDelimiters, bWordDelimitersEnable);
    EscapeDataToEscapeStringW(wszWordDelimiters, wbuf);
    SetWindowTextW(hWndWordDelimiters, wbuf);
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
      bState=SendMessage(hWndWordDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndWordDelimiters, bState);
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
        SetTabStops(hWndEdit, nTabStopSize, TRUE);
      }
      bTabStopAsSpaces=SendMessage(hWndTabSizeSpaces, BM_GETCHECK, 0, 0);

      //Undo
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, NULL, FALSE);
      if (nUndoLimit != a)
      {
        nUndoLimit=a;
        SendMessage(hWndEdit, AEM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
      }
      bDetailedUndo=SendMessage(hWndDetailedUndo, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bDetailedUndo?AECOOP_OR:AECOOP_XOR, AECO_DETAILEDUNDO);

      //Margins
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, NULL, FALSE);
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, NULL, FALSE);
      if (dwEditMargins != (DWORD)MAKELONG(a, b))
      {
        dwEditMargins=MAKELONG(a, b);
        SendMessage(hWndEdit, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
        InvalidateRect(hWndEdit, NULL, TRUE);
      }

      //Word delimiters
      GetWindowTextW(hWndWordDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, wszWordDelimiters);

      bWordDelimitersEnable=SendMessage(hWndWordDelimitersEnable, BM_GETCHECK, 0, 0);
      if (bWordDelimitersEnable)
      {
        SendMessage(hWndEdit, AEM_SETWORDBREAK, dwCustomWordBreak, 0);
        SendMessage(hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)wszWordDelimiters);
      }
      else
      {
        SendMessage(hWndEdit, AEM_SETWORDBREAK, dwDefaultWordBreak, 0);
        SendMessage(hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)NULL);
      }

      //Save settings
      if (SendMessage(hWndSaveRegistry, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=SS_REGISTRY;
      else
        a=SS_INI;
      if (nSaveSettings != a)
      {
        nSaveSettings=a;

        bEditFontChanged=TRUE;
        bColorsChanged=TRUE;
        bPrintFontChanged=TRUE;
        IniSaveOptionsW();
        RegSaveOptionsW();
        SaveThemesW(TRUE);
        StackPluginSaveW(&hPluginsStack, TRUE);
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
  static HWND hWndUrlPrefixesEnable;
  static HWND hWndUrlPrefixes;
  static HWND hWndUrlDelimitersEnable;
  static HWND hWndUrlDelimiters;
  static HWND hWndCaretOutEdge;
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
    hWndUrlPrefixesEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES_ENABLE);
    hWndUrlPrefixes=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES);
    hWndUrlDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS_ENABLE);
    hWndUrlDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS);
    hWndCaretOutEdge=GetDlgItem(hDlg, IDC_OPTIONS_CARETOUTEDGE);
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

    if (bUrlPrefixesEnable)
      SendMessage(hWndUrlPrefixesEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndUrlPrefixes, bUrlPrefixesEnable);
    WideCharToMultiByte(CP_ACP, 0, wszUrlPrefixes, -1, buf2, BUFFER_SIZE, NULL, NULL);
    SetWindowTextA(hWndUrlPrefixes, buf2);
    SendMessage(hWndUrlPrefixes, EM_LIMITTEXT, (WPARAM)URL_PREFIXES_SIZE, 0);

    if (bUrlDelimitersEnable)
      SendMessage(hWndUrlDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndUrlDelimiters, bUrlDelimitersEnable);
    EscapeDataToEscapeStringW(wszUrlDelimiters, (wchar_t *)buf);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t *)buf, -1, buf2, BUFFER_SIZE, NULL, NULL);
    SetWindowTextA(hWndUrlDelimiters, buf2);
    SendMessage(hWndUrlDelimiters, EM_LIMITTEXT, (WPARAM)URL_DELIMITERS_SIZE, 0);

    if (bCaretOutEdge)
      SendMessage(hWndCaretOutEdge, BM_SETCHECK, BST_CHECKED, 0);
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
      EnableWindow(hWndUrlPrefixesEnable, bState);
      EnableWindow(hWndUrlDelimitersEnable, bState);

      if (!bState)
      {
        EnableWindow(hWndUrlPrefixes, FALSE);
        EnableWindow(hWndUrlDelimiters, FALSE);
      }
      else
      {
        bState=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
        EnableWindow(hWndUrlPrefixes, bState);
        bState=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
        EnableWindow(hWndUrlDelimiters, bState);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_URL_PREFIXES_ENABLE)
    {
      bState=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndUrlPrefixes, bState);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_URL_DELIMITERS_ENABLE)
    {
      bState=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndUrlDelimiters, bState);
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
        SendMessage(hWndEdit, AEM_SETDETECTURL, bShowURL, 0);
      }
      if (SendMessage(hWndSingleClickURL, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nClickURL=1;
      else
        nClickURL=2;

      //Url prefixes
      a=GetWindowTextA(hWndUrlPrefixes, buf, BUFFER_SIZE);
      MultiByteToWideChar(CP_ACP, 0, buf, a + 1, wszUrlPrefixes, URL_PREFIXES_SIZE);

      bUrlPrefixesEnable=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
      if (bUrlPrefixesEnable)
        SetUrlPrefixes(hWndEdit, wszUrlPrefixes);
      else
        SetUrlPrefixes(hWndEdit, NULL);

      //Url delimiters
      a=GetWindowTextA(hWndUrlDelimiters, buf, BUFFER_SIZE);
      MultiByteToWideChar(CP_ACP, 0, buf, a + 1, (wchar_t *)buf2, BUFFER_SIZE / sizeof(wchar_t));
      EscapeStringToEscapeDataW((wchar_t *)buf2, wszUrlDelimiters);

      bUrlDelimitersEnable=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
      if (bUrlDelimitersEnable)
        SendMessage(hWndEdit, AEM_SETURLDELIMITERS, 0, (LPARAM)wszUrlDelimiters);
      else
        SendMessage(hWndEdit, AEM_SETURLDELIMITERS, 0, (LPARAM)NULL);

      //Allow caret moving out of the line edge
      bCaretOutEdge=SendMessage(hWndCaretOutEdge, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);
      SendMessage(hWndEdit, AEM_UPDATESEL, AESELT_LOCKSCROLL, 0);

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
  static HWND hWndUrlPrefixesEnable;
  static HWND hWndUrlPrefixes;
  static HWND hWndUrlDelimitersEnable;
  static HWND hWndUrlDelimiters;
  static HWND hWndCaretOutEdge;
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
    hWndUrlPrefixesEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES_ENABLE);
    hWndUrlPrefixes=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES);
    hWndUrlDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS_ENABLE);
    hWndUrlDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS);
    hWndCaretOutEdge=GetDlgItem(hDlg, IDC_OPTIONS_CARETOUTEDGE);
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

    if (bUrlPrefixesEnable)
      SendMessage(hWndUrlPrefixesEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndUrlPrefixes, bUrlPrefixesEnable);
    SetWindowTextW(hWndUrlPrefixes, wszUrlPrefixes);
    SendMessage(hWndUrlPrefixes, EM_LIMITTEXT, (WPARAM)URL_PREFIXES_SIZE, 0);

    if (bUrlDelimitersEnable)
      SendMessage(hWndUrlDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndUrlDelimiters, bUrlDelimitersEnable);
    EscapeDataToEscapeStringW(wszUrlDelimiters, wbuf);
    SetWindowTextW(hWndUrlDelimiters, wbuf);
    SendMessage(hWndUrlDelimiters, EM_LIMITTEXT, (WPARAM)URL_DELIMITERS_SIZE, 0);

    if (bCaretOutEdge)
      SendMessage(hWndCaretOutEdge, BM_SETCHECK, BST_CHECKED, 0);
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
      EnableWindow(hWndUrlPrefixesEnable, bState);
      EnableWindow(hWndUrlDelimitersEnable, bState);

      if (!bState)
      {
        EnableWindow(hWndUrlPrefixes, FALSE);
        EnableWindow(hWndUrlDelimiters, FALSE);
      }
      else
      {
        bState=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
        EnableWindow(hWndUrlPrefixes, bState);
        bState=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
        EnableWindow(hWndUrlDelimiters, bState);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_URL_PREFIXES_ENABLE)
    {
      bState=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndUrlPrefixes, bState);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_URL_DELIMITERS_ENABLE)
    {
      bState=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndUrlDelimiters, bState);
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
        SendMessage(hWndEdit, AEM_SETDETECTURL, bShowURL, 0);
      }
      if (SendMessage(hWndSingleClickURL, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nClickURL=1;
      else
        nClickURL=2;

      //Url prefixes
      GetWindowTextW(hWndUrlPrefixes, wszUrlPrefixes, URL_PREFIXES_SIZE);

      bUrlPrefixesEnable=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
      if (bUrlPrefixesEnable)
        SetUrlPrefixes(hWndEdit, wszUrlPrefixes);
      else
        SetUrlPrefixes(hWndEdit, NULL);

      //Url delimiters
      GetWindowTextW(hWndUrlDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, wszUrlDelimiters);

      bUrlDelimitersEnable=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
      if (bUrlDelimitersEnable)
        SendMessage(hWndEdit, AEM_SETURLDELIMITERS, 0, (LPARAM)wszUrlDelimiters);
      else
        SendMessage(hWndEdit, AEM_SETURLDELIMITERS, 0, (LPARAM)NULL);

      //Allow caret moving out of the line edge
      bCaretOutEdge=SendMessage(hWndCaretOutEdge, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);
      SendMessage(hWndEdit, AEM_UPDATESEL, AESELT_LOCKSCROLL, 0);

      //ReplaceAll and close dialog
      bReplaceAllAndClose=SendMessage(hWndReplaceAllAndClose, BM_GETCHECK, 0, 0);

      //Save in read only file message
      bSaveInReadOnlyMsg=SendMessage(hWndSaveInReadOnlyMsg, BM_GETCHECK, 0, 0);
    }
  }
  return FALSE;
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

void SetSelectionStatusA(HWND hWnd, AECHARRANGE *cr, AECHARINDEX *ci)
{
  char szStatus[MAX_PATH];

  if (cr && ci)
  {
    crSel=*cr;
    ciCaret=*ci;
  }
  else
  {
    GetSel(hWnd, &crSel, NULL, &ciCaret);
  }

  if (bStatusSelUpdate)
  {
    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      wsprintfA(szStatus, "%u:%u", ciCaret.nLine + 1, ciCaret.nCharInLine + 1);
    else
      wsprintfA(szStatus, "%u:%u, %u", ciCaret.nLine + 1, ciCaret.nCharInLine + 1, IndexSubtract(hWnd, &crSel.ciMin, &crSel.ciMax, AELB_ASOUTPUT, -1));

    SendMessage(hStatus, SB_SETTEXTA, STATUS_POSITION, (LPARAM)szStatus);
  }
}

void SetSelectionStatusW(HWND hWnd, AECHARRANGE *cr, AECHARINDEX *ci)
{
  wchar_t wszStatus[MAX_PATH];

  if (cr && ci)
  {
    crSel=*cr;
    ciCaret=*ci;
  }
  else
  {
    GetSel(hWnd, &crSel, NULL, &ciCaret);
  }

  if (bStatusSelUpdate)
  {
    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      wsprintfW(wszStatus, L"%u:%u", ciCaret.nLine + 1, ciCaret.nCharInLine + 1);
    else
      wsprintfW(wszStatus, L"%u:%u, %u", ciCaret.nLine + 1, ciCaret.nCharInLine + 1, IndexSubtract(hWnd, &crSel.ciMin, &crSel.ciMax, AELB_ASOUTPUT, -1));

    SendMessage(hStatus, SB_SETTEXTW, STATUS_POSITION, (LPARAM)wszStatus);
  }
}

void SetModifyStatusA(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (hWnd) SendMessage(hWnd, AEM_SETMODIFY, bState, 0);

  if (!hWnd || hWnd == hWndEdit)
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

void SetModifyStatusW(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (hWnd) SendMessage(hWnd, AEM_SETMODIFY, bState, 0);

  if (!hWnd || hWnd == hWndEdit)
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

void SetNewLineStatusA(HWND hWnd, int nState, DWORD dwFlags, BOOL bFirst)
{
  if (hWnd)
  {
    SendMessage(hWnd, AEM_SETNEWLINE, AENL_INPUT|AENL_OUTPUT, MAKELONG(AELB_ASIS, AELB_ASIS));

    if (nState == NEWLINE_WIN)
      SendMessage(hWnd, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_RN, AELB_RN));
    else if (nState == NEWLINE_UNIX)
      SendMessage(hWnd, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_N, AELB_N));
    else if (nState == NEWLINE_MAC)
      SendMessage(hWnd, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_R, AELB_R));

    SendMessage(hWnd, AEM_UPDATESEL, AESELT_LOCKSCROLL, 0);
  }

  if (!hWnd || hWnd == hWndEdit)
  {
    if (bFirst != TRUE && nNewLine == nState) return;
    nNewLine=nState;

    if (nNewLine == NEWLINE_WIN)
      SendMessage(hStatus, SB_SETTEXTA, STATUS_NEWLINE, (LPARAM)"Win");
    else if (nNewLine == NEWLINE_UNIX)
      SendMessage(hStatus, SB_SETTEXTA, STATUS_NEWLINE, (LPARAM)"Unix");
    else if (nNewLine == NEWLINE_MAC)
      SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)"Mac");
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

void SetNewLineStatusW(HWND hWnd, int nState, DWORD dwFlags, BOOL bFirst)
{
  if (hWnd)
  {
    SendMessage(hWnd, AEM_SETNEWLINE, AENL_INPUT|AENL_OUTPUT, MAKELONG(AELB_ASIS, AELB_ASIS));

    if (nState == NEWLINE_WIN)
      SendMessage(hWnd, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_RN, AELB_RN));
    else if (nState == NEWLINE_UNIX)
      SendMessage(hWnd, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_N, AELB_N));
    else if (nState == NEWLINE_MAC)
      SendMessage(hWnd, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_R, AELB_R));

    SendMessage(hWnd, AEM_UPDATESEL, AESELT_LOCKSCROLL, 0);
  }

  if (!hWnd || hWnd == hWndEdit)
  {
    if (bFirst != TRUE && nNewLine == nState) return;
    nNewLine=nState;

    if (nNewLine == NEWLINE_WIN)
      SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)L"Win");
    else if (nNewLine == NEWLINE_UNIX)
      SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)L"Unix");
    else if (nNewLine == NEWLINE_MAC)
      SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)L"Mac");
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

void SetInsertStateStatusA(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (bFirst != TRUE && bInsertState == bState) return;
  bInsertState=bState;

  if (hWnd) SendMessage(hWnd, AEM_SETOVERTYPE, bInsertState, 0);

  SendMessage(hStatus, SB_SETTEXTA, STATUS_INSERT, (LPARAM)(bInsertState?"Ovr":"Ins"));
}

void SetInsertStateStatusW(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (bFirst != TRUE && bInsertState == bState) return;
  bInsertState=bState;

  if (hWnd) SendMessage(hWnd, AEM_SETOVERTYPE, bInsertState, 0);

  SendMessage(hStatus, SB_SETTEXTW, STATUS_INSERT, (LPARAM)(bInsertState?L"Ovr":L"Ins"));
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

void SaveLineScroll(HWND hWnd, int *nBeforeLine)
{
  AECHARINDEX ciCharIndex;

  SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciCharIndex);
  *nBeforeLine=ciCharIndex.nLine;
}

void RestoreLineScroll(HWND hWnd, int *nBeforeLine)
{
  AECHARINDEX ciCharIndex;

  SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciCharIndex);

  if (*nBeforeLine != ciCharIndex.nLine)
  {
    SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, *nBeforeLine - ciCharIndex.nLine);
  }
}

BOOL SelectColorDialogA(HWND hWndOwner, COLORREF *crColor)
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

BOOL SelectColorDialogW(HWND hWndOwner, COLORREF *crColor)
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

BOOL GetCharColor(HWND hWnd, CHARCOLOR *cc)
{
  AECHARRANGE cr;
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciCharIndex;

  if (!hWnd) hWnd=hWndEdit;

  GetSel(hWnd, &cr, NULL, &ciCaretIndex);
  SendMessage(hWnd, AEM_RICHOFFSETTOINDEX, cc->nCharPos, (LPARAM)&ciCharIndex);

  if (AEC_IndexCompare(&ciCharIndex, &cr.ciMin) >= 0 &&
      AEC_IndexCompare(&ciCharIndex, &cr.ciMax) < 0)
  {
    cc->crText=aecColors.crSelText;
    cc->crBk=aecColors.crSelBk;
    return TRUE;
  }
  else if (ciCharIndex.nLine == ciCaretIndex.nLine)
  {
    cc->crText=aecColors.crActiveLineText;
    cc->crBk=aecColors.crActiveLineBk;
  }
  else
  {
    cc->crText=aecColors.crBasicText;
    cc->crBk=aecColors.crBasicBk;
  }
  return FALSE;
}

HFONT SetChosenFontA(HWND hWnd, LOGFONTA *lfA, BOOL bDeleteOld)
{
  HFONT hOldFont=NULL;
  HFONT hNewFont=NULL;

  if (lfA)
  {
    if (hNewFont=(HFONT)CreateFontIndirectA(lfA))
    {
      if (bDeleteOld)
      {
        if (hOldFont=(HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0))
        {
          DeleteObject(hOldFont);
          hOldFont=NULL;
        }
      }
      SendMessage(hWnd, WM_SETFONT, (WPARAM)hNewFont, FALSE);
    }
  }
  return hNewFont;
}

HFONT SetChosenFontW(HWND hWnd, LOGFONTW *lfW, BOOL bDeleteOld)
{
  HFONT hOldFont=NULL;
  HFONT hNewFont=NULL;

  if (lfW)
  {
    if (hNewFont=(HFONT)CreateFontIndirectW(lfW))
    {
      if (bDeleteOld)
      {
        if (hOldFont=(HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0))
        {
          DeleteObject(hOldFont);
          hOldFont=NULL;
        }
      }
      SendMessage(hWnd, WM_SETFONT, (WPARAM)hNewFont, FALSE);
    }
  }
  return hNewFont;
}

void SetTabStops(HWND hWnd, int nTabStops, BOOL bSetRedraw)
{
  SendMessage(hWnd, AEM_SETTABSTOP, nTabStops, bSetRedraw);
}

BOOL InsertTabStopW(HWND hWnd)
{
  wchar_t wszSpaces[MAX_PATH];
  int nSpaces;
  int i;

  if (IsReadOnly()) return FALSE;

  if (bTabStopAsSpaces)
  {
    for (i=min(crSel.ciMin.lpLine->nLineLen, crSel.ciMin.nCharInLine) - 1; i > 0; --i)
    {
      if (crSel.ciMin.lpLine->wpLine[i] == L'\t')
      {
        ++i;
        break;
      }
    }
    i=max(i, 0);
    nSpaces=nTabStopSize - (crSel.ciMin.nCharInLine - i) % nTabStopSize;
    nSpaces=min(nSpaces, MAX_PATH - 1);

    for (i=0; i < nSpaces; ++i)
      wszSpaces[i]=' ';
    wszSpaces[i]='\0';
    ReplaceSelW(hWnd, wszSpaces, -1, -1, NULL, NULL);
  }
  else
    SendMessage(hWnd, WM_CHAR, (WPARAM)'\t', 0);

  return TRUE;
}

BOOL IndentTabStopW(HWND hWnd, int nAction)
{
  wchar_t wszSpaces[MAX_PATH];
  int nSpaces;
  int i;

  if (IsReadOnly()) return FALSE;

  if (bTabStopAsSpaces)
  {
    nSpaces=min(nTabStopSize, MAX_PATH - 1);

    for (i=0; i < nSpaces; ++i)
      wszSpaces[i]=' ';
    wszSpaces[i]='\0';
    DoEditInsertStringInSelectionW(hWnd, nAction, wszSpaces);
  }
  else
    DoEditInsertStringInSelectionW(hWnd, nAction, L"\t");

  return TRUE;
}

BOOL AutoIndent(HWND hWnd, AECHARRANGE *cr)
{
  wchar_t *wpText;
  int i;

  if (cr->ciMin.nCharInLine)
  {
    //Calculate spaces
    for (i=0; cr->ciMin.lpLine->wpLine[i] == ' ' || cr->ciMin.lpLine->wpLine[i] == '\t'; ++i);

    if (i)
    {
      //Insert spaces
      if (wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, (i + 2) * sizeof(wchar_t)))
      {
        wpText[0]='\n';

        for (i=0; cr->ciMin.lpLine->wpLine[i] == ' ' || cr->ciMin.lpLine->wpLine[i] == '\t'; ++i)
        {
          wpText[i + 1]=cr->ciMin.lpLine->wpLine[i];
        }
        wpText[i + 1]='\0';

        ReplaceSelW(hWnd, wpText, -1, FALSE, NULL, NULL);
        API_HeapFree(hHeap, 0, (LPVOID)wpText);
        return TRUE;
      }
    }
  }
  return FALSE;
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

int SetUrlPrefixes(HWND hWnd, wchar_t *wpPrefixes)
{
  wchar_t wszBuffer[URL_PREFIXES_SIZE];
  int nUrlPrefixMax=URL_PREFIXES_SIZE - 2;
  int a=0;
  int b=0;

  if (wpPrefixes)
  {
    while (wpPrefixes[a] == ' ') ++a;

    for (; b < nUrlPrefixMax; ++a, ++b)
    {
      if (wpPrefixes[a])
      {
        if (wpPrefixes[a] == ' ')
        {
          while (wpPrefixes[a + 1] == ' ') ++a;
          wszBuffer[b]='\0';
        }
        else wszBuffer[b]=wpPrefixes[a];
      }
      else break;
    }
    wszBuffer[b]='\0';
    wszBuffer[++b]='\0';

    return SendMessage(hWnd, AEM_SETURLPREFIXES, 0, (LPARAM)wszBuffer);
  }
  else
  {
    return SendMessage(hWnd, AEM_SETURLPREFIXES, 0, (LPARAM)NULL);
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

BOOL GetFullNameA(char *szFile, int nFileLen)
{
  char *pFileName;

  if (GetFullPathNameA(szFile, BUFFER_SIZE, buf, &pFileName))
  {
    if (FileExistsA(szFile))
    {
      if (GetLongPathNameAPtr)
        (*GetLongPathNameAPtr)(buf, szFile, nFileLen);
      else
        lstrcpynA(szFile, buf, nFileLen);
      return TRUE;
    }
    lstrcpynA(szFile, buf, nFileLen);
  }
  return FALSE;
}

BOOL GetFullNameW(wchar_t *wszFile, int nFileLen)
{
  wchar_t *wpFileName;

  if (GetFullPathNameW(wszFile, BUFFER_SIZE, wbuf, &wpFileName))
  {
    if (FileExistsW(wszFile))
    {
      if (GetLongPathNameWPtr)
        (*GetLongPathNameWPtr)(wbuf, wszFile, nFileLen);
      else
        lstrcpynW(wszFile, wbuf, nFileLen);
      return TRUE;
    }
    lstrcpynW(wszFile, wbuf, nFileLen);
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

void ResizeEdit(HWND hWnd, int X, int Y, int nWidth, int nHeight)
{
  UpdateWindow(hStatus);
  MoveWindow(hWnd, X, Y, nWidth, nHeight, TRUE);
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

      if (nTabView == TAB_VIEW_TOP)
      {
        MoveWindow(hTab, nsSize.rcCurrent.left, nsSize.rcCurrent.top, nsSize.rcCurrent.right, TAB_HEIGHT, TRUE);
        UpdateWindow(hTab);
      }
      else if (nTabView == TAB_VIEW_BOTTOM)
      {
        MoveWindow(hTab, nsSize.rcCurrent.left, nsSize.rcCurrent.top + i, nsSize.rcCurrent.right, TAB_HEIGHT, TRUE);
        UpdateWindow(hTab);
      }
      MoveWindow(hMdiClient, nsSize.rcCurrent.left, nsSize.rcCurrent.top + ((nTabView == TAB_VIEW_TOP)?TAB_HEIGHT:0), nsSize.rcCurrent.right, i, TRUE);
    }

    hDocksStack.bSizing=FALSE;
  }
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
  static HCURSOR hCursorSizeWE=LoadCursor(NULL, IDC_SIZEWE);
  static HCURSOR hCursorSizeNS=LoadCursor(NULL, IDC_SIZENS);
  static HCURSOR hCursorSizeALL=LoadCursor(NULL, IDC_SIZEALL);

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
      //Set tab icon
      tcItemA.mask=TCIF_IMAGE;
      SendMessage(hTab, TCM_GETITEMA, nItem, (LPARAM)&tcItemA);
      ImageList_ReplaceIcon(hImageList, tcItemA.iImage, hIcon);

      //Set tab text
      tcItemA.mask=TCIF_TEXT;
      tcItemA.pszText=pFileName;
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
      //Set tab icon
      tcItemW.mask=TCIF_IMAGE;
      SendMessage(hTab, TCM_GETITEMW, nItem, (LPARAM)&tcItemW);
      ImageList_ReplaceIcon(hImageList, tcItemW.iImage, hIcon);

      //Set tab text
      tcItemW.mask=TCIF_TEXT;
      tcItemW.pszText=wpFileName;
      SendMessage(hTab, TCM_SETITEMW, nItem, (LPARAM)&tcItemW);
    }

    //Set frame info
    if (wf=(WNDFRAMEW *)GetWindowLongW(hWndEditParent, GWL_USERDATA))
    {
      lstrcpynW(wf->wszFile, wszFile, MAX_PATH);
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
  int i=-1;

  tcItemA.mask=TCIF_PARAM;

  for (i=0; SendMessage(hWnd, TCM_GETITEMA, i, (LPARAM)&tcItemA); ++i)
  {
    if (tcItemA.lParam == lParam) break;
  }
  return i;
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


//// AkelEdit control functions

int AEC_IndexCompare(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2)
{
  if (ciChar1->nLine == ciChar2->nLine &&
      ciChar1->nCharInLine == ciChar2->nCharInLine)
  {
    return 0;
  }
  if ((ciChar1->nLine < ciChar2->nLine) ||
      (ciChar1->nLine == ciChar2->nLine &&
       ciChar1->nCharInLine < ciChar2->nCharInLine))
  {
    return -1;
  }
  return 1;
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
    API_LoadStringA(hLangLib, MSG_ERROR_CANNOT_OPEN_FILE, buf, BUFFER_SIZE);
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
    API_LoadStringW(hLangLib, MSG_ERROR_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
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
