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
extern HHOOK hHookPlugins;
extern HWND hWndHotkey;
extern RECT rcPluginsDialog;
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
extern HWND hProgress;
extern HWND hDlgModeless;
extern RECT rcMainWindowRestored;
extern DWORD dwMainStyle;
extern DWORD dwLastMainSize;
extern int nStatusHeight;
extern int nProgressWidth;
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

//Clones
extern BOOL bSplitWindow;
extern HWND hWndMaster;
extern HWND hWndClone1;
extern HWND hWndClone2;
extern HWND hWndClone3;
extern HCURSOR hCursorSizeWE;
extern HCURSOR hCursorSizeNS;
extern HCURSOR hCursorSizeALL;
extern HCURSOR hCursorClone;
extern RECT rcMasterWindow;

//Docks
extern HDOCK hDocksStack;
extern NSIZE nsSize;
extern WNDPROC OldDockProc;

//Codepages
extern RECT rcRecodeDlg;
extern int *lpCodepageList;
extern int nCodepageListLen;
extern int *lpCodepageTable;
extern int nCodepageTableCount;
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

//Open/Save document
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
extern POINT ptDocumentPos;
extern BOOL bDocumentReopen;
extern BOOL bSaveInReadOnlyMsg;
extern WNDPROC OldFilePreviewProc;

//Find/Replace dialog
extern RECT rcFindAndReplaceDlg;
extern DWORD ftflags;
extern BOOL bReplaceDlg;
extern BOOL bReplaceAllAndClose;
extern int nSearchStrings;
extern char *szFindText_orig;
extern wchar_t *wszFindText_orig;
extern char *szReplaceText_orig;
extern wchar_t *wszReplaceText_orig;
extern char *szFindText;
extern wchar_t *wszFindText;
extern char *szReplaceText;
extern wchar_t *wszReplaceText;
extern int nFindTextLen;
extern int nReplaceTextLen;
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
extern HSTACK hThemesStack;
extern AECOLORS aecColors;
extern COLORREF crCustColors[16];
extern BOOL bEditFontChanged;
extern BOOL bColorsChanged;
extern RECT rcColorsDialog;

//Print
extern HWND hWndPreviewEdit;
extern HWND hWndPreviewDlg;
extern HWND hWndZoomEdit;
extern HSTACK hPreviewAllPagesStack;
extern HSTACK hPreviewSelPagesStack;
extern RECT rcPreviewWindow;
extern RECT rcPreviewPaper;
extern RECT rcPreviewZoomed;
extern POINT ptPreviewScroll;
extern int lpZoom[];
extern int nPreviewZoomMaxIndex;
extern int nPreviewZoomPercent;
extern int nPreviewCharHeight;
extern int nPreviewAveCharWidth;
extern int nPreviewPageCur;
extern int nPreviewAllPageSum;
extern int nPreviewSelPageSum;
extern BOOL bPreviewSelection;
extern AEPRINT prn;
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
extern RECT rcEditWindow;
extern AECHARRANGE crSel;
extern AECHARINDEX ciCaret;
extern BOOL bModified;
extern BOOL bInsertState;
extern int nCurrentNewLine;
extern int nDefaultNewLine;
extern BOOL bWordWrap;
extern int nWrapType;
extern DWORD dwWrapLimit;
extern DWORD dwMarker;
extern BOOL bOnTop;
extern BOOL bStatusBar;
extern DWORD dwShowModify;
extern DWORD dwStatusPosType;
extern BOOL bReadOnly;
extern BOOL bSaveTime;
extern BOOL bWatchFile;
extern BOOL bSingleOpenFile;
extern BOOL bSingleOpenProgram;
extern BOOL bKeepSpace;
extern int nLoopCase;
extern DWORD dwEditMargins;
extern int nTabStopSize;
extern BOOL bTabStopAsSpaces;
extern int nUndoLimit;
extern BOOL bDetailedUndo;
extern BOOL bCaretOutEdge;
extern BOOL bCaretVertLine;
extern int nCaretWidth;
extern DWORD dwLineGap;
extern BOOL bShowURL;
extern int nClickURL;
extern BOOL bUrlPrefixesEnable;
extern wchar_t wszUrlPrefixes[URL_PREFIXES_SIZE];
extern BOOL bUrlDelimitersEnable;
extern wchar_t wszUrlDelimiters[URL_DELIMITERS_SIZE];
extern BOOL bWordDelimitersEnable;
extern wchar_t wszWordDelimiters[WORD_DELIMITERS_SIZE];
extern DWORD dwCustomWordBreak;
extern DWORD dwDefaultWordBreak;
extern BOOL bWrapDelimitersEnable;
extern wchar_t wszWrapDelimiters[WRAP_DELIMITERS_SIZE];
extern FILETIME ftFileTime;
extern WNDPROC OldEditProc;

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
  SendMessage(hWndEditNew, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
  SendMessage(hWndEditNew, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bDetailedUndo?AECOOP_OR:AECOOP_XOR, AECO_DETAILEDUNDO);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bCaretVertLine?AECOOP_OR:AECOOP_XOR, AECO_ACTIVECOLUMN);
  SendMessage(hWndEditNew, AEM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
  SetTabStops(hWndEditNew, nTabStopSize, FALSE);
  SetChosenFontA(hWndEditNew, &lfEditFontA, TRUE);
  SetNewLineStatusA(hWndEditNew, nDefaultNewLine, AENL_INPUT, TRUE);

  if (dwMarker)
  {
    SendMessage(hWndEditNew, AEM_SETMARKER, dwMarker, 0);
  }
  if (dwLineGap)
  {
    SendMessage(hWndEditNew, AEM_SETLINEGAP, dwLineGap, 0);
  }
  if (nCaretWidth != 1)
  {
    POINT pt;

    SendMessage(hWndEditNew, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);
    pt.x=nCaretWidth;
    SendMessage(hWndEditNew, AEM_SETCARETWIDTH, 0, (LPARAM)&pt);
  }
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
  if (bWrapDelimitersEnable)
  {
    SendMessage(hWndEditNew, AEM_SETWRAPDELIMITERS, 0, (LPARAM)wszWrapDelimiters);
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
  SendMessage(hWndEditNew, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
  SendMessage(hWndEditNew, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bDetailedUndo?AECOOP_OR:AECOOP_XOR, AECO_DETAILEDUNDO);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);
  SendMessage(hWndEditNew, AEM_SETOPTIONS, bCaretVertLine?AECOOP_OR:AECOOP_XOR, AECO_ACTIVECOLUMN);
  SendMessage(hWndEditNew, AEM_SETUNDOLIMIT, (WPARAM)nUndoLimit, 0);
  SetTabStops(hWndEditNew, nTabStopSize, FALSE);
  SetChosenFontW(hWndEditNew, &lfEditFontW, TRUE);
  SetNewLineStatusW(hWndEditNew, nDefaultNewLine, AENL_INPUT, TRUE);

  if (dwMarker)
  {
    SendMessage(hWndEditNew, AEM_SETMARKER, dwMarker, 0);
  }
  if (dwLineGap)
  {
    SendMessage(hWndEditNew, AEM_SETLINEGAP, dwLineGap, 0);
  }
  if (nCaretWidth != 1)
  {
    POINT pt;

    SendMessage(hWndEditNew, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);
    pt.x=nCaretWidth;
    SendMessage(hWndEditNew, AEM_SETCARETWIDTH, 0, (LPARAM)&pt);
  }
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
  if (bWrapDelimitersEnable)
  {
    SendMessage(hWndEditNew, AEM_SETWRAPDELIMITERS, 0, (LPARAM)wszWrapDelimiters);
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
  SetNewLineStatusA(hWndEdit, nDefaultNewLine, AENL_INPUT, TRUE);
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
  SetNewLineStatusW(hWndEdit, nDefaultNewLine, AENL_INPUT, TRUE);
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

BOOL DoFilePageSetupA(HWND hWndOwner)
{
  BOOL bResult;

  psdPageA.hwndOwner=hWndOwner;
  bResult=PageSetupDlgA(&psdPageA);
  pdA.hDevMode=psdPageA.hDevMode;
  pdA.hDevNames=psdPageA.hDevNames;

  return bResult;
}

BOOL DoFilePageSetupW(HWND hWndOwner)
{
  BOOL bResult;

  psdPageW.hwndOwner=hWndOwner;
  bResult=PageSetupDlgW(&psdPageW);
  pdW.hDevMode=psdPageW.hDevMode;
  pdW.hDevNames=psdPageW.hDevNames;

  return bResult;
}

int DoFilePrintA(HWND hWnd, BOOL bSilent)
{
  int nResult=0;

  //Set print dialog settings
  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
    pdA.Flags|=PD_NOSELECTION;
  else
    pdA.Flags&=~PD_NOSELECTION;

  //Choose printer
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

  if (pdA.hDC)
  {
    prn.hPrinterDC=pdA.hDC;
    nResult=PrintDocumentA(hWnd, &prn, PRN_REALPRINT|(pdA.Flags & PD_SELECTION?PRN_SELECTION:PRN_ALLTEXT), 0);

    pdA.Flags&=~PD_SELECTION;
    pdA.Flags&=~PD_PAGENUMS;
    pdA.Flags&=~PD_PRINTTOFILE;
    DeleteDC(pdA.hDC);
    pdA.hDC=NULL;
  }
  return nResult;
}

int DoFilePrintW(HWND hWnd, BOOL bSilent)
{
  int nResult=0;

  //Set print dialog settings
  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
    pdW.Flags|=PD_NOSELECTION;
  else
    pdW.Flags&=~PD_NOSELECTION;

  //Choose printer
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

  if (pdW.hDC)
  {
    prn.hPrinterDC=pdW.hDC;
    nResult=PrintDocumentW(hWnd, &prn, PRN_REALPRINT|(pdW.Flags & PD_SELECTION?PRN_SELECTION:PRN_ALLTEXT), 0);

    pdW.Flags&=~PD_SELECTION;
    pdW.Flags&=~PD_PAGENUMS;
    pdW.Flags&=~PD_PRINTTOFILE;
    DeleteDC(pdW.hDC);
    pdW.hDC=NULL;
  }
  return nResult;
}

void DoFilePreviewA(HWND hWnd)
{
  hWndPreviewEdit=hWnd;
  API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_PRINTPREVIEW), hMainWnd, (DLGPROC)PreviewDlgProcA);
  hWndPreviewEdit=NULL;
}

void DoFilePreviewW(HWND hWnd)
{
  hWndPreviewEdit=hWnd;
  API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_PRINTPREVIEW), hMainWnd, (DLGPROC)PreviewDlgProcW);
  hWndPreviewEdit=NULL;
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

BOOL DoEditPaste(HWND hWnd, BOOL bAnsi)
{
  if (IsReadOnly()) return FALSE;

  return SendMessage(hWnd, AEM_PASTE, 0, bAnsi);
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
  SetSel(hWnd, &cr, 0, &cr.ciMax);
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
      //SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTLINE, (LPARAM)&crRange.ciMax);
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
      SetSel(hWnd, &crRange, 0, NULL);
      if (nAction & STRSEL_INSERT)
      {
        nStringLenAll=(crRange.ciMax.nLine - crRange.ciMin.nLine + 1) * nStringLen;
        nBufferLen=nRangeLen + nStringLenAll;

        if (wszRange=(wchar_t *)API_HeapAlloc(hHeap, 0, nBufferLen * sizeof(wchar_t) + 2))
        {
          tr.cr=crRange;
          tr.bColumnSel=FALSE;
          tr.pBuffer=wszRange + nStringLenAll;
          tr.dwBufferMax=(DWORD)-1;
          tr.nNewLine=AELB_ASIS;
          tr.bFillSpaces=TRUE;
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
          tr.pBuffer=wszRange;
          tr.dwBufferMax=(DWORD)-1;
          tr.nNewLine=AELB_ASIS;
          tr.bFillSpaces=TRUE;
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
          SetSel(hWnd, &crRange, 0, &crRange.ciMin);
        else
          SetSel(hWnd, &crRange, 0, &crRange.ciMax);
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

  if (nRangeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszRange, AELB_ASIS, TRUE))
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
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMax);

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
    SetSel(hWnd, &crRange, 0, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nRangeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszRange, AELB_ASIS, TRUE))
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
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crInitialSel.ciMin);
      crRange.ciMin=crInitialSel.ciMin;
      crRange.ciMax=crInitialSel.ciMin;
    }

    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMax);

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
  char szWordDelimiters[WORD_DELIMITERS_SIZE];
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
    SetSel(hWnd, &crRange, 0, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nRangeLen=ExGetRangeTextA(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &szRange, AELB_ASIS, TRUE))
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
      WideCharToMultiByte(CP_ACP, 0, wszWordDelimiters, -1, szWordDelimiters, WORD_DELIMITERS_SIZE, NULL, NULL);

      while (pStart < pEnd)
      {
        while (pStart < pEnd && (AKD_strchr(szWordDelimiters, *pStart) || AKD_strchr(SENTENCE_DELIMITERSA, *pStart)))
          ++pStart;

        if (pStart < pEnd) *pStart++=(char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pStart);

        while (pStart < pEnd && !AKD_strchr(SENTENCE_DELIMITERSA, *pStart))
          *pStart++=(char)(WORD)(DWORD)CharLowerA((char *)(DWORD)(WORD)*pStart);
      }
    }
    else if (nCase == TITLECASE)
    {
      WideCharToMultiByte(CP_ACP, 0, wszWordDelimiters, -1, szWordDelimiters, WORD_DELIMITERS_SIZE, NULL, NULL);

      while (pStart < pEnd)
      {
        while (pStart < pEnd && AKD_strchr(szWordDelimiters, *pStart))
          ++pStart;

        if (pStart < pEnd) *pStart++=(char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pStart);

        while (pStart < pEnd && !AKD_strchr(szWordDelimiters, *pStart))
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
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crInitialSel.ciMin);
      crRange.ciMin=crInitialSel.ciMin;
      crRange.ciMax=crInitialSel.ciMin;
    }
    else
    {
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crRange.ciMin);
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crRange.ciMax);
    }

    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMax);

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
    SetSel(hWnd, &crRange, 0, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nRangeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszRange, AELB_ASIS, TRUE))
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
        while (wpStart < wpEnd && (AKD_wcschr(wszWordDelimiters, *wpStart) || AKD_wcschr(SENTENCE_DELIMITERSW, *wpStart)))
          ++wpStart;

        if (wpStart < wpEnd) *wpStart++=(wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpStart);

        while (wpStart < wpEnd && !AKD_wcschr(SENTENCE_DELIMITERSW, *wpStart))
          *wpStart++=(wchar_t)(WORD)(DWORD)CharLowerW((wchar_t *)(DWORD)(WORD)*wpStart);
      }
    }
    else if (nCase == TITLECASE)
    {
      while (wpStart < wpEnd)
      {
        while (wpStart < wpEnd && AKD_wcschr(wszWordDelimiters, *wpStart))
          ++wpStart;

        if (wpStart < wpEnd) *wpStart++=(wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpStart);

        while (wpStart < wpEnd && !AKD_wcschr(wszWordDelimiters, *wpStart))
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
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crInitialSel.ciMin);
      crRange.ciMin=crInitialSel.ciMin;
      crRange.ciMax=crInitialSel.ciMin;
    }
    else
    {
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crRange.ciMin);
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crRange.ciMax);
    }

    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMax);

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
    if ((ftflags & AEFR_ALLFILES) && !bMDI)
      ftflags&=~AEFR_ALLFILES;
    if ((ftflags & AEFR_SELECTION) && !AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      ftflags&=~AEFR_SELECTION;

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
    if ((ftflags & AEFR_ALLFILES) && !bMDI)
      ftflags&=~AEFR_ALLFILES;
    if ((ftflags & AEFR_SELECTION) && !AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      ftflags&=~AEFR_SELECTION;

    if (hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_FIND), hMainWnd, (DLGPROC)FindAndReplaceDlgProcW))
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditFindNextDownA(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_UP & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_DOWN;

  if (szFindText)
    FindTextA(hWnd, dwFlags, szFindText, nFindTextLen);
  else
    DoEditFindA();
}

void DoEditFindNextDownW(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_UP & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_DOWN;

  if (wszFindText)
    FindTextW(hWnd, dwFlags, wszFindText, nFindTextLen);
  else
    DoEditFindW();
}

void DoEditFindNextUpA(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_DOWN & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_UP;

  if (szFindText)
    FindTextA(hWnd, dwFlags, szFindText, nFindTextLen);
  else
    DoEditFindA();
}

void DoEditFindNextUpW(HWND hWnd)
{
  DWORD dwFlags=(ftflags & ~AEFR_DOWN & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_UP;

  if (wszFindText)
    FindTextW(hWnd, dwFlags, wszFindText, nFindTextLen);
  else
    DoEditFindW();
}

void DoEditReplaceA()
{
  if (!hDlgModeless)
  {
    bReplaceDlg=TRUE;
    if ((ftflags & AEFR_ALLFILES) && !bMDI)
      ftflags&=~AEFR_ALLFILES;
    if ((ftflags & AEFR_SELECTION) && !AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      ftflags&=~AEFR_SELECTION;

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
    if ((ftflags & AEFR_ALLFILES) && !bMDI)
      ftflags&=~AEFR_ALLFILES;
    if ((ftflags & AEFR_SELECTION) && !AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      ftflags&=~AEFR_SELECTION;

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
    UpdateShowHScroll(hWnd);
    SendMessage(hWnd, AEM_SETWORDWRAP, nWrapType, dwWrapLimit);
  }
  else
  {
    SendMessage(hWnd, AEM_SETWORDWRAP, AEWW_NONE, 0);
    UpdateShowHScroll(hWnd);
  }
}

void DoViewSplitWindow(BOOL bState, WPARAM wParam)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_SPLIT_WINDOW_ALL, bState?MF_CHECKED:MF_UNCHECKED);
  bSplitWindow=bState;

  if (bSplitWindow)
  {
    //Create
    hWndMaster=hWndEdit;

    if (wParam == IDM_VIEW_SPLIT_WINDOW_ALL ||
        wParam == IDM_VIEW_SPLIT_WINDOW_WE)
    {
      if (bOldWindows)
        hWndClone1=CreateEditWindowA(GetParent(hWndMaster));
      else
        hWndClone1=CreateEditWindowW(GetParent(hWndMaster));
      if (hWndClone1) SendMessage(hWndMaster, AEM_ADDCLONE, (WPARAM)hWndClone1, 0);
    }
    if (wParam == IDM_VIEW_SPLIT_WINDOW_ALL ||
        wParam == IDM_VIEW_SPLIT_WINDOW_NS)
    {
      if (bOldWindows)
        hWndClone2=CreateEditWindowA(GetParent(hWndMaster));
      else
        hWndClone2=CreateEditWindowW(GetParent(hWndMaster));
      if (hWndClone2) SendMessage(hWndMaster, AEM_ADDCLONE, (WPARAM)hWndClone2, 0);
    }
    if (wParam == IDM_VIEW_SPLIT_WINDOW_ALL)
    {
      if (bOldWindows)
        hWndClone3=CreateEditWindowA(GetParent(hWndMaster));
      else
        hWndClone3=CreateEditWindowW(GetParent(hWndMaster));
      if (hWndClone3) SendMessage(hWndMaster, AEM_ADDCLONE, (WPARAM)hWndClone3, 0);
    }

    if (bWordWrap)
    {
      UpdateShowHScroll(hWndEdit);
    }

    rcMasterWindow.left=0;
    rcMasterWindow.top=0;
    rcMasterWindow.right=rcEditWindow.right / 2;
    rcMasterWindow.bottom=rcEditWindow.bottom / 2;
  }
  else
  {
    //Destroy
    hWndEdit=hWndMaster;
    DestroyEdit(CN_CLONE1|CN_CLONE2|CN_CLONE3, &hWndEdit, &hWndMaster, &hWndClone1, &hWndClone2, &hWndClone3);
    hWndMaster=NULL;

    if (bWordWrap)
    {
      UpdateShowHScroll(hWndEdit);
    }
  }
  ResizeEdit(hWndEdit, hWndMaster, hWndClone1, hWndClone2, hWndClone3, rcEditWindow.left, rcEditWindow.top, rcEditWindow.right, rcEditWindow.bottom, &rcMasterWindow, &rcEditWindow, FALSE);
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
    SaveThemesA(TRUE);
    StackPluginSaveA(&hPluginsStack, TRUE);

    nSaveSettings=SS_INI;
    SaveOptionsA();
    SaveThemesA(TRUE);
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
    SaveThemesW(TRUE);
    StackPluginSaveW(&hPluginsStack, TRUE);

    nSaveSettings=SS_INI;
    SaveOptionsW();
    SaveThemesW(TRUE);
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
  SetSel(hWnd, &cr, AESELT_LOCKSCROLL, NULL);

  ReplaceSelW(hWnd, L"", -1, FALSE, NULL, NULL);
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

  if (dwAttr == INVALID_FILE_ATTRIBUTES || !(dwAttr & FILE_ATTRIBUTE_READONLY))
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

  if (dwAttr == INVALID_FILE_ATTRIBUTES || !(dwAttr & FILE_ATTRIBUTE_READONLY))
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
  ReadOptionA(hHandle, "ShowModify", PO_DWORD, &dwShowModify, sizeof(DWORD));
  ReadOptionA(hHandle, "StatusPosType", PO_DWORD, &dwStatusPosType, sizeof(DWORD));
  ReadOptionA(hHandle, "ReadOnly", PO_DWORD, &bReadOnly, sizeof(DWORD));
  ReadOptionA(hHandle, "SaveTime", PO_DWORD, &bSaveTime, sizeof(DWORD));
  ReadOptionA(hHandle, "KeepSpace", PO_DWORD, &bKeepSpace, sizeof(DWORD));
  ReadOptionA(hHandle, "UndoLimit", PO_DWORD, &nUndoLimit, sizeof(DWORD));
  ReadOptionA(hHandle, "DetailedUndo", PO_DWORD, &bDetailedUndo, sizeof(DWORD));
  ReadOptionA(hHandle, "WrapType", PO_DWORD, &nWrapType, sizeof(DWORD));
  ReadOptionA(hHandle, "WrapLimit", PO_DWORD, &dwWrapLimit, sizeof(DWORD));
  ReadOptionA(hHandle, "Marker", PO_DWORD, &dwMarker, sizeof(DWORD));
  ReadOptionA(hHandle, "CaretOutEdge", PO_DWORD, &bCaretOutEdge, sizeof(DWORD));
  ReadOptionA(hHandle, "CaretVertLine", PO_DWORD, &bCaretVertLine, sizeof(DWORD));
  ReadOptionA(hHandle, "CaretWidth", PO_DWORD, &nCaretWidth, sizeof(DWORD));
  ReadOptionA(hHandle, "LineGap", PO_DWORD, &dwLineGap, sizeof(DWORD));
  ReadOptionA(hHandle, "ReplaceAllAndClose", PO_DWORD, &bReplaceAllAndClose, sizeof(DWORD));
  ReadOptionA(hHandle, "SaveInReadOnlyMsg", PO_DWORD, &bSaveInReadOnlyMsg, sizeof(DWORD));
  ReadOptionA(hHandle, "WatchFile", PO_DWORD, &bWatchFile, sizeof(DWORD));
  ReadOptionA(hHandle, "SingleOpenFile", PO_DWORD, &bSingleOpenFile, sizeof(DWORD));
  ReadOptionA(hHandle, "SingleOpenProgram", PO_DWORD, &bSingleOpenProgram, sizeof(DWORD));
  ReadOptionA(hHandle, "MDI", PO_DWORD, &bRegMDI, sizeof(DWORD));
  ReadOptionA(hHandle, "SavePositions", PO_DWORD, &bSavePositions, sizeof(DWORD));
  ReadOptionA(hHandle, "SaveCodepages", PO_DWORD, &bSaveCodepages, sizeof(DWORD));
  ReadOptionA(hHandle, "DefaultCodepage", PO_DWORD, &nDefaultCodePage, sizeof(DWORD));
  ReadOptionA(hHandle, "DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD));
  ReadOptionA(hHandle, "CodepageRecognition", PO_DWORD, &dwLangCodepageRecognition, sizeof(DWORD));
  ReadOptionA(hHandle, "CodepageRecognitionBuffer", PO_DWORD, &dwCodepageRecognitionBuffer, sizeof(DWORD));
  ReadOptionA(hHandle, "RecentFiles", PO_DWORD, &nRecentFiles, sizeof(DWORD));
  ReadOptionA(hHandle, "SearchStrings", PO_DWORD, &nSearchStrings, sizeof(DWORD));
  ReadOptionA(hHandle, "SearchOptions", PO_DWORD, &ftflags, sizeof(DWORD));
  ReadOptionA(hHandle, "TabStopSize", PO_DWORD, &nTabStopSize, sizeof(DWORD));
  ReadOptionA(hHandle, "TabStopAsSpaces", PO_DWORD, &bTabStopAsSpaces, sizeof(DWORD));
  ReadOptionA(hHandle, "MarginsEdit", PO_DWORD, &dwEditMargins, sizeof(DWORD));
  ReadOptionA(hHandle, "MarginsPrint", PO_BINARY, &psdPageA.rtMargin, sizeof(RECT));
  ReadOptionA(hHandle, "PluginsDialog", PO_BINARY, &rcPluginsDialog, sizeof(RECT));
  ReadOptionA(hHandle, "WindowStyle", PO_DWORD, &dwMainStyle, sizeof(DWORD));
  ReadOptionA(hHandle, "WindowPosition", PO_BINARY, &rcMainWindowRestored, sizeof(RECT));

  if (bRegMDI)
  {
    ReadOptionA(hHandle, "TabViewMDI", PO_DWORD, &nTabView, sizeof(DWORD));
    ReadOptionA(hHandle, "TabTypeMDI", PO_DWORD, &nTabType, sizeof(DWORD));
    ReadOptionA(hHandle, "TabSwitchMDI", PO_DWORD, &nTabSwitch, sizeof(DWORD));
    ReadOptionA(hHandle, "WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD));
  }

  ReadOptionA(hHandle, "ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD));
  ReadOptionA(hHandle, "ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD));
  ReadOptionA(hHandle, "UrlPrefixesEnable", PO_DWORD, &bUrlPrefixesEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "UrlPrefixes", PO_BINARY, wszUrlPrefixes, sizeof(wszUrlPrefixes));
  ReadOptionA(hHandle, "UrlDelimitersEnable", PO_DWORD, &bUrlDelimitersEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "UrlDelimiters", PO_BINARY, wszUrlDelimiters, sizeof(wszUrlDelimiters));
  ReadOptionA(hHandle, "WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "WordDelimiters", PO_BINARY, wszWordDelimiters, sizeof(wszWordDelimiters));
  ReadOptionA(hHandle, "WordBreak", PO_DWORD, &dwCustomWordBreak, sizeof(DWORD));
  ReadOptionA(hHandle, "WrapDelimitersEnable", PO_DWORD, &bWrapDelimitersEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "WrapDelimiters", PO_BINARY, wszWrapDelimiters, sizeof(wszWrapDelimiters));
  ReadOptionA(hHandle, "Font", PO_BINARY, &lfEditFontA, sizeof(LOGFONTA) - LF_FACESIZE);
  ReadOptionA(hHandle, "FontFace", PO_STRING, &lfEditFontA.lfFaceName, LF_FACESIZE);
  ReadOptionA(hHandle, "PrintFontEnable", PO_DWORD, &bPrintFontEnable, sizeof(DWORD));
  ReadOptionA(hHandle, "PrintFont", PO_BINARY, &lfPrintFontA, sizeof(LOGFONTA) - LF_FACESIZE);
  ReadOptionA(hHandle, "PrintFontFace", PO_STRING, &lfPrintFontA.lfFaceName, LF_FACESIZE);
  ReadOptionA(hHandle, "ColorsDialog", PO_BINARY, &rcColorsDialog, sizeof(RECT));
  ReadOptionA(hHandle, "Colors", PO_BINARY, &aecColors, sizeof(AECOLORS));
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
  ReadOptionW(hHandle, L"ShowModify", PO_DWORD, &dwShowModify, sizeof(DWORD));
  ReadOptionW(hHandle, L"StatusPosType", PO_DWORD, &dwStatusPosType, sizeof(DWORD));
  ReadOptionW(hHandle, L"ReadOnly", PO_DWORD, &bReadOnly, sizeof(DWORD));
  ReadOptionW(hHandle, L"SaveTime", PO_DWORD, &bSaveTime, sizeof(DWORD));
  ReadOptionW(hHandle, L"KeepSpace", PO_DWORD, &bKeepSpace, sizeof(DWORD));
  ReadOptionW(hHandle, L"UndoLimit", PO_DWORD, &nUndoLimit, sizeof(DWORD));
  ReadOptionW(hHandle, L"DetailedUndo", PO_DWORD, &bDetailedUndo, sizeof(DWORD));
  ReadOptionW(hHandle, L"WrapType", PO_DWORD, &nWrapType, sizeof(DWORD));
  ReadOptionW(hHandle, L"WrapLimit", PO_DWORD, &dwWrapLimit, sizeof(DWORD));
  ReadOptionW(hHandle, L"Marker", PO_DWORD, &dwMarker, sizeof(DWORD));
  ReadOptionW(hHandle, L"CaretOutEdge", PO_DWORD, &bCaretOutEdge, sizeof(DWORD));
  ReadOptionW(hHandle, L"CaretVertLine", PO_DWORD, &bCaretVertLine, sizeof(DWORD));
  ReadOptionW(hHandle, L"CaretWidth", PO_DWORD, &nCaretWidth, sizeof(DWORD));
  ReadOptionW(hHandle, L"LineGap", PO_DWORD, &dwLineGap, sizeof(DWORD));
  ReadOptionW(hHandle, L"ReplaceAllAndClose", PO_DWORD, &bReplaceAllAndClose, sizeof(DWORD));
  ReadOptionW(hHandle, L"SaveInReadOnlyMsg", PO_DWORD, &bSaveInReadOnlyMsg, sizeof(DWORD));
  ReadOptionW(hHandle, L"WatchFile", PO_DWORD, &bWatchFile, sizeof(DWORD));
  ReadOptionW(hHandle, L"SingleOpenFile", PO_DWORD, &bSingleOpenFile, sizeof(DWORD));
  ReadOptionW(hHandle, L"SingleOpenProgram", PO_DWORD, &bSingleOpenProgram, sizeof(DWORD));
  ReadOptionW(hHandle, L"MDI", PO_DWORD, &bRegMDI, sizeof(DWORD));
  ReadOptionW(hHandle, L"SavePositions", PO_DWORD, &bSavePositions, sizeof(DWORD));
  ReadOptionW(hHandle, L"SaveCodepages", PO_DWORD, &bSaveCodepages, sizeof(DWORD));
  ReadOptionW(hHandle, L"DefaultCodepage", PO_DWORD, &nDefaultCodePage, sizeof(DWORD));
  ReadOptionW(hHandle, L"DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD));
  ReadOptionW(hHandle, L"CodepageRecognition", PO_DWORD, &dwLangCodepageRecognition, sizeof(DWORD));
  ReadOptionW(hHandle, L"CodepageRecognitionBuffer", PO_DWORD, &dwCodepageRecognitionBuffer, sizeof(DWORD));
  ReadOptionW(hHandle, L"RecentFiles", PO_DWORD, &nRecentFiles, sizeof(DWORD));
  ReadOptionW(hHandle, L"SearchStrings", PO_DWORD, &nSearchStrings, sizeof(DWORD));
  ReadOptionW(hHandle, L"SearchOptions", PO_DWORD, &ftflags, sizeof(DWORD));
  ReadOptionW(hHandle, L"TabStopSize", PO_DWORD, &nTabStopSize, sizeof(DWORD));
  ReadOptionW(hHandle, L"TabStopAsSpaces", PO_DWORD, &bTabStopAsSpaces, sizeof(DWORD));
  ReadOptionW(hHandle, L"MarginsEdit", PO_DWORD, &dwEditMargins, sizeof(DWORD));
  ReadOptionW(hHandle, L"MarginsPrint", PO_BINARY, &psdPageW.rtMargin, sizeof(RECT));
  ReadOptionW(hHandle, L"PluginsDialog", PO_BINARY, &rcPluginsDialog, sizeof(RECT));
  ReadOptionW(hHandle, L"WindowStyle", PO_DWORD, &dwMainStyle, sizeof(DWORD));
  ReadOptionW(hHandle, L"WindowPosition", PO_BINARY, &rcMainWindowRestored, sizeof(RECT));

  if (bRegMDI)
  {
    ReadOptionW(hHandle, L"TabViewMDI", PO_DWORD, &nTabView, sizeof(DWORD));
    ReadOptionW(hHandle, L"TabTypeMDI", PO_DWORD, &nTabType, sizeof(DWORD));
    ReadOptionW(hHandle, L"TabSwitchMDI", PO_DWORD, &nTabSwitch, sizeof(DWORD));
    ReadOptionW(hHandle, L"WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD));
  }

  ReadOptionW(hHandle, L"ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD));
  ReadOptionW(hHandle, L"ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD));
  ReadOptionW(hHandle, L"UrlPrefixesEnable", PO_DWORD, &bUrlPrefixesEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"UrlPrefixes", PO_BINARY, wszUrlPrefixes, sizeof(wszUrlPrefixes));
  ReadOptionW(hHandle, L"UrlDelimitersEnable", PO_DWORD, &bUrlDelimitersEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"UrlDelimiters", PO_BINARY, wszUrlDelimiters, sizeof(wszUrlDelimiters));
  ReadOptionW(hHandle, L"WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"WordDelimiters", PO_BINARY, wszWordDelimiters, sizeof(wszWordDelimiters));
  ReadOptionW(hHandle, L"WordBreak", PO_DWORD, &dwCustomWordBreak, sizeof(DWORD));
  ReadOptionW(hHandle, L"WrapDelimitersEnable", PO_DWORD, &bWrapDelimitersEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"WrapDelimiters", PO_BINARY, wszWrapDelimiters, sizeof(wszWrapDelimiters));
  ReadOptionW(hHandle, L"Font", PO_BINARY, &lfEditFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"FontFace", PO_STRING, &lfEditFontW.lfFaceName, LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"PrintFontEnable", PO_DWORD, &bPrintFontEnable, sizeof(DWORD));
  ReadOptionW(hHandle, L"PrintFont", PO_BINARY, &lfPrintFontW, sizeof(LOGFONTW) - LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"PrintFontFace", PO_STRING, &lfPrintFontW.lfFaceName, LF_FACESIZE * sizeof(wchar_t));
  ReadOptionW(hHandle, L"ColorsDialog", PO_BINARY, &rcColorsDialog, sizeof(RECT));
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
    if (szFindText_orig=szFindText=(char *)API_HeapAlloc(hHeap, 0, dwSize + 1))
    {
      if (RegQueryValueExA(hKey, "find0", NULL, &dwType, (LPBYTE)szFindText_orig, &dwSize) == ERROR_SUCCESS)
      {
        nFindTextLen=dwSize - 1;

        if (ftflags & AEFR_ESCAPESEQ)
        {
          if (szFindText=(char *)API_HeapAlloc(hHeap, 0, dwSize + 1))
          {
            nFindTextLen=EscapeStringToEscapeDataA(szFindText_orig, szFindText);
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
    if (wszFindText_orig=wszFindText=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize + 2))
    {
      if (RegQueryValueExW(hKey, L"find0", NULL, &dwType, (LPBYTE)wszFindText_orig, &dwSize) == ERROR_SUCCESS)
      {
        nFindTextLen=dwSize / sizeof(wchar_t) - 1;

        if (ftflags & AEFR_ESCAPESEQ)
        {
          if (wszFindText=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize + 2))
          {
            nFindTextLen=EscapeStringToEscapeDataW(wszFindText_orig, wszFindText);
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
  if (!SaveOptionA(hHandle, "ShowModify", PO_DWORD, &dwShowModify, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "StatusPosType", PO_DWORD, &dwStatusPosType, sizeof(DWORD)))
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
  if (!SaveOptionA(hHandle, "WrapType", PO_DWORD, &nWrapType, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WrapLimit", PO_DWORD, &dwWrapLimit, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "Marker", PO_DWORD, &dwMarker, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "CaretOutEdge", PO_DWORD, &bCaretOutEdge, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "CaretVertLine", PO_DWORD, &bCaretVertLine, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "CaretWidth", PO_DWORD, &nCaretWidth, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "LineGap", PO_DWORD, &dwLineGap, sizeof(DWORD)))
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
  if (!SaveOptionA(hHandle, "DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD)))
    goto Error;
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
  if (!SaveOptionA(hHandle, "PluginsDialog", PO_BINARY, &rcPluginsDialog, sizeof(RECT)))
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
    if (!SaveOptionA(hHandle, "WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD)))
      goto Error;
  }

  if (!SaveOptionA(hHandle, "ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "UrlPrefixesEnable", PO_DWORD, &bUrlPrefixesEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "UrlPrefixes", PO_BINARY, wszUrlPrefixes, wcslen(wszUrlPrefixes) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionA(hHandle, "UrlDelimitersEnable", PO_DWORD, &bUrlDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "UrlDelimiters", PO_BINARY, wszUrlDelimiters, wcslen(wszUrlDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionA(hHandle, "WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WordDelimiters", PO_BINARY, wszWordDelimiters, wcslen(wszWordDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionA(hHandle, "WordBreak", PO_DWORD, &dwCustomWordBreak, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WrapDelimitersEnable", PO_DWORD, &bWrapDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionA(hHandle, "WrapDelimiters", PO_BINARY, wszWrapDelimiters, wcslen(wszWrapDelimiters) * sizeof(wchar_t) + 2))
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
  if (!SaveOptionA(hHandle, "ColorsDialog", PO_BINARY, &rcColorsDialog, sizeof(RECT)))
    goto Error;
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
  if (!SaveOptionW(hHandle, L"ShowModify", PO_DWORD, &dwShowModify, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"StatusPosType", PO_DWORD, &dwStatusPosType, sizeof(DWORD)))
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
  if (!SaveOptionW(hHandle, L"WrapType", PO_DWORD, &nWrapType, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WrapLimit", PO_DWORD, &dwWrapLimit, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"Marker", PO_DWORD, &dwMarker, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"CaretOutEdge", PO_DWORD, &bCaretOutEdge, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"CaretVertLine", PO_DWORD, &bCaretVertLine, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"CaretWidth", PO_DWORD, &nCaretWidth, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"LineGap", PO_DWORD, &dwLineGap, sizeof(DWORD)))
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
  if (!SaveOptionW(hHandle, L"DefaultNewLine", PO_DWORD, &nDefaultNewLine, sizeof(DWORD)))
    goto Error;
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
  if (!SaveOptionW(hHandle, L"PluginsDialog", PO_BINARY, &rcPluginsDialog, sizeof(RECT)))
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
    if (!SaveOptionW(hHandle, L"WindowStyleMDI", PO_DWORD, &dwMdiStyle, sizeof(DWORD)))
      goto Error;
  }

  if (!SaveOptionW(hHandle, L"ShowURL", PO_DWORD, &bShowURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"ClickURL", PO_DWORD, &nClickURL, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"UrlPrefixesEnable", PO_DWORD, &bUrlPrefixesEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"UrlPrefixes", PO_BINARY, wszUrlPrefixes, lstrlenW(wszUrlPrefixes) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"UrlDelimitersEnable", PO_DWORD, &bUrlDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"UrlDelimiters", PO_BINARY, wszUrlDelimiters, lstrlenW(wszUrlDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"WordDelimitersEnable", PO_DWORD, &bWordDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WordDelimiters", PO_BINARY, wszWordDelimiters, lstrlenW(wszWordDelimiters) * sizeof(wchar_t) + 2))
    goto Error;
  if (!SaveOptionW(hHandle, L"WordBreak", PO_DWORD, &dwCustomWordBreak, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WrapDelimitersEnable", PO_DWORD, &bWrapDelimitersEnable, sizeof(DWORD)))
    goto Error;
  if (!SaveOptionW(hHandle, L"WrapDelimiters", PO_BINARY, wszWrapDelimiters, lstrlenW(wszWrapDelimiters) * sizeof(wchar_t) + 2))
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
  if (!SaveOptionW(hHandle, L"ColorsDialog", PO_BINARY, &rcColorsDialog, sizeof(RECT)))
    goto Error;
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
  aec.crActiveColumn=RGB(0x00, 0x00, 0x00);
  aec.crColumnMarker=GetSysColor(COLOR_BTNFACE);
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
      lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

      while (lpIniKey)
      {
        IniGetValueA(&hIniStack, "Themes", lpIniKey->szKey, INI_BINARY, (LPBYTE)&aec, sizeof(AECOLORS));
        StackThemeAddA(&hThemesStack, lpIniKey->szKey, &aec);
        ++dwIndex;

        lpIniKey=lpIniKey->next;
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
    aec.crActiveColumn=RGB(0xE8, 0xE8, 0xFF);
    aec.crColumnMarker=RGB(0xC0, 0xC0, 0xC0);
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
  aec.crActiveColumn=RGB(0x00, 0x00, 0x00);
  aec.crColumnMarker=GetSysColor(COLOR_BTNFACE);
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
      lpIniKey=(HINIKEY *)lpIniSection->hKeysStack.first;

      while (lpIniKey)
      {
        IniGetValueW(&hIniStack, L"Themes", lpIniKey->wszKey, INI_BINARY, (LPBYTE)&aec, sizeof(AECOLORS));
        StackThemeAddW(&hThemesStack, lpIniKey->wszKey, &aec);
        ++dwIndex;

        lpIniKey=lpIniKey->next;
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
    aec.crActiveColumn=RGB(0xE8, 0xE8, 0xFF);
    aec.crColumnMarker=RGB(0xC0, 0xC0, 0xC0);
    StackThemeAddW(&hThemesStack, L"Notepad++", &aec);
  }
}

BOOL SaveThemesA(BOOL bCleanOld)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hThemesStack.first;
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
    if (!OpenIniA(&hIniStack, szIniFile, TRUE))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionA(&hIniStack, "Themes", lstrlenA("Themes")))
        StackDeleteIniSection(&hIniStack, lpIniSection, TRUE);
    }
  }

  //Skip "Standard" theme
  if (ctElement) ctElement=ctElement->next;

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
    ctElement=ctElement->next;
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
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hThemesStack.first;
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
    if (!OpenIniW(&hIniStack, wszIniFile, TRUE))
      return FALSE;

    if (bCleanOld)
    {
      if (lpIniSection=StackGetIniSectionW(&hIniStack, L"Themes", lstrlenW(L"Themes")))
        StackDeleteIniSection(&hIniStack, lpIniSection, TRUE);
    }
  }

  //Skip "Standard" theme
  if (ctElement) ctElement=ctElement->next;

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
    ctElement=ctElement->next;
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
    if (IsEditActive(hWnd))
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

      if (bDocumentReopen)
      {
        SendMessage(hWnd, AEM_GETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);
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

  //Offset BOM
  fsd.nBytesCurrent=0;

  if (bBOM)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (nCodePage == CP_UNICODE_UCS2_BE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (nCodePage == CP_UNICODE_UTF8)
    {
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=3;
    }
  }

  if (IsEditActive(hWnd))
  {
    //Save position of the previous file before load new document
    if (nRecentFiles && szCurrentFile[0])
    {
      RecentFilesZeroA();
      RecentFilesReadA();
      RecentFilesUpdateA(szCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
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

    HideCaret(NULL);
  }

  //Load text
  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.dwFlags=dwFlags;
  fsd.nNewLine=nDefaultNewLine;
  fsd.nBytesMax=-1;
  fsd.bResult=TRUE;
  FileStreamIn(&fsd);
  CloseHandle(hFile);

  if (IsEditActive(hWnd))
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
      SetModifyStatusA(hWndEdit, FALSE, FALSE);
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
        DWORD dwScrollFlags=0;
        DWORD dwScrollResult;

        SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, TRUE);
        cr.cpMin=lpdwRecentPositions[0];
        cr.cpMax=lpdwRecentPositions[0];
        SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
        SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, FALSE);

        if (bDocumentReopen)
          SendMessage(hWnd, AEM_SETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);
        else
        {
          dwScrollResult=SendMessage(hWnd, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(1, 1));
          if (dwScrollResult & AECSE_SCROLLEDX)
            dwScrollFlags|=AESC_UNITRECTDIVX;
          if (dwScrollResult & AECSE_SCROLLEDY)
            dwScrollFlags|=AESC_UNITRECTDIVY;
          SendMessage(hWnd, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
        }
      }

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
    if (IsEditActive(hWnd))
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

      if (bDocumentReopen)
      {
        SendMessage(hWnd, AEM_GETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);
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

  //Offset BOM
  fsd.nBytesCurrent=0;

  if (bBOM)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (nCodePage == CP_UNICODE_UCS2_BE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (nCodePage == CP_UNICODE_UTF8)
    {
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=3;
    }
  }

  if (IsEditActive(hWnd))
  {
    //Save position of the previous file before load new document
    if (nRecentFiles && wszCurrentFile[0])
    {
      RecentFilesZeroW();
      RecentFilesReadW();
      RecentFilesUpdateW(wszCurrentFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCurrentCodePage);
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

    HideCaret(NULL);
  }

  //Load text
  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.dwFlags=dwFlags;
  fsd.nNewLine=nDefaultNewLine;
  fsd.nBytesMax=-1;
  fsd.bResult=TRUE;
  FileStreamIn(&fsd);
  CloseHandle(hFile);

  if (IsEditActive(hWnd))
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
      SetModifyStatusW(hWndEdit, FALSE, FALSE);
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
        DWORD dwScrollFlags=0;
        DWORD dwScrollResult;

        SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, TRUE);
        cr.cpMin=lpdwRecentPositions[0];
        cr.cpMax=lpdwRecentPositions[0];
        SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
        SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, FALSE);

        if (bDocumentReopen)
          SendMessage(hWnd, AEM_SETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);
        else
        {
          dwScrollResult=SendMessage(hWnd, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(1, 1));
          if (dwScrollResult & AECSE_SCROLLEDX)
            dwScrollFlags|=AESC_UNITRECTDIVX;
          if (dwScrollResult & AECSE_SCROLLEDY)
            dwScrollFlags|=AESC_UNITRECTDIVY;
          SendMessage(hWnd, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
        }
      }

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

/*
//Read entire file (AEM_SETTEXTW).
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
        SendMessage(lpData->hWnd, AEM_SETTEXTW, (WPARAM)dwCharsConverted, (LPARAM)wpBuffer);
      }
      else lpData->bResult=FALSE;

      API_HeapFree(hHeap, 0, (LPVOID)wpBuffer);
    }
    else lpData->bResult=FALSE;
  }
  else lpData->bResult=FALSE;
}
*/

//Stream file reading (AEM_STREAMIN).
void FileStreamIn(FILESTREAMDATA *lpData)
{
  AESTREAMIN aesi;

  if (lpData->nBytesMax == -1)
    lpData->nBytesMax=GetFileSize(lpData->hFile, NULL);
  SendMessage(lpData->hWnd, AEM_SETNEWLINE, AENL_INPUT|AENL_OUTPUT, MAKELONG(AELB_ASIS, AELB_ASIS));

  aesi.dwCookie=(DWORD)lpData;
  aesi.lpCallback=InputStreamCallback;
  aesi.nNewLine=AELB_ASIS;
  aesi.dwTextLen=lpData->nBytesMax;
  SendMessage(lpData->hWnd, AEM_STREAMIN, 0, (LPARAM)&aesi);
  lpData->bResult=!aesi.dwError;

  //Detect new line
  if (lpData->bResult)
  {
    if (aesi.nFirstNewLine == AELB_RRN)
      lpData->nNewLine=NEWLINE_WIN;
    else if (aesi.nFirstNewLine == AELB_RN)
      lpData->nNewLine=NEWLINE_WIN;
    else if (aesi.nFirstNewLine == AELB_R)
      lpData->nNewLine=NEWLINE_MAC;
    else if (aesi.nFirstNewLine == AELB_N)
      lpData->nNewLine=NEWLINE_UNIX;
  }
}

DWORD CALLBACK InputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufDone)
{
  FILESTREAMDATA *lpData=(FILESTREAMDATA *)dwCookie;
  DWORD dwBytesRead;
  DWORD dwCharsConverted=0;
  DWORD dwBytesConverted=0;

  if (lpData->nBytesCurrent <= lpData->nBytesMax)
  {
    if (API_ReadFile(lpData->hFile, pcTranslateBuffer, dwBufLen / sizeof(wchar_t), &dwBytesRead, NULL))
    {
      lpData->nBytesCurrent+=dwBytesRead;

      //Translate data to UNICODE
      if (lpData->nCodePage == CP_UNICODE_UCS2_LE)
      {
        memcpy(wszBuf, pcTranslateBuffer, dwBytesRead);
        dwCharsConverted=dwBytesRead / sizeof(wchar_t);
      }
      else if (lpData->nCodePage == CP_UNICODE_UCS2_BE)
      {
        ChangeByteOrder(pcTranslateBuffer, dwBytesRead);
        memcpy(wszBuf, pcTranslateBuffer, dwBytesRead);
        dwCharsConverted=dwBytesRead / sizeof(wchar_t);
      }
      else
      {
        if (lpData->nCodePage == CP_UNICODE_UTF8)
        {
          dwCharsConverted=UTF8toUTF16(pcTranslateBuffer, dwBytesRead, (unsigned int *)&dwBytesConverted, wszBuf, dwBufLen / sizeof(wchar_t));

          if (dwBytesRead != dwBytesConverted)
          {
            //UTF-8 char was split
            lpData->nBytesCurrent+=dwBytesConverted - dwBytesRead;
            SetFilePointer(lpData->hFile, lpData->nBytesCurrent, NULL, FILE_BEGIN);
          }
        }
        else
        {
          dwCharsConverted=MultiByteToWideChar(lpData->nCodePage, 0, (char *)pcTranslateBuffer, dwBytesRead, wszBuf, dwBufLen / sizeof(wchar_t));

          if (dwCharsConverted > 0)
          {
            if (pcTranslateBuffer[dwBytesRead - 1] != '\0' &&
                wszBuf[dwCharsConverted - 1] == '\0')
            {
              //Double-byte char was split
              --lpData->nBytesCurrent;
              SetFilePointer(lpData->hFile, lpData->nBytesCurrent, NULL, FILE_BEGIN);
              --dwCharsConverted;
            }
          }
        }
      }
      wszBuf[dwCharsConverted]='\0';
    }
  }
  *dwBufDone=dwCharsConverted * sizeof(wchar_t);

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
      if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
        nLine=SendMessage(hWnd, AEM_GETUNWRAPLINE, nLine - 1, 0) + 1;
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

  if (dwAttr != INVALID_FILE_ATTRIBUTES)
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

  hFile=API_CreateFileA(szFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, (dwAttr != INVALID_FILE_ATTRIBUTES)?TRUNCATE_EXISTING:CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

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
    if (dwAttr != INVALID_FILE_ATTRIBUTES)
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
          RecentFilesUpdateA(szFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCodePage);
          RecentFilesSaveA();
          if (nFileCmp) bMenuRecentFiles=TRUE;
        }
      }

      if (IsEditActive(hWnd))
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
  if (dwAttr != INVALID_FILE_ATTRIBUTES)
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
      if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
        nLine=SendMessage(hWnd, AEM_GETUNWRAPLINE, nLine - 1, 0) + 1;
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

  if (dwAttr != INVALID_FILE_ATTRIBUTES)
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

  hFile=API_CreateFileW(wszFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, (dwAttr != INVALID_FILE_ATTRIBUTES)?TRUNCATE_EXISTING:CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

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
    if (dwAttr != INVALID_FILE_ATTRIBUTES)
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
          RecentFilesUpdateW(wszFile, AkelIndexToRichOffset(hWndEdit, &ciCaret), nCodePage);
          RecentFilesSaveW();
          if (nFileCmp) bMenuRecentFiles=TRUE;
        }
      }

      if (IsEditActive(hWnd))
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
  if (dwAttr != INVALID_FILE_ATTRIBUTES)
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
  AESTREAMOUT aeso;

  aeso.dwCookie=(DWORD)lpData;
  aeso.lpCallback=OutputStreamCallback;
  aeso.nNewLine=AELB_ASOUTPUT;
  if (lpData->dwFlags & SD_SELECTION)
    aeso.bColumnSel=SendMessage(lpData->hWnd, AEM_GETCOLUMNSEL, 0, 0);
  else
    aeso.bColumnSel=FALSE;
  SendMessage(lpData->hWnd, AEM_STREAMOUT, (lpData->dwFlags & SD_SELECTION)?(AESF_SELECTION|AESF_FILLSPACES):0, (LPARAM)&aeso);
  lpData->bResult=!aeso.dwError;
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
      dwBytesToWrite=UTF16toUTF8(wszBuf, dwBufLen, NULL, (char *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE);
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

int PrintDocumentA(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage)
{
  DOCINFOA diA={0};
  POINT ptScreenDpi;
  POINT ptPrintDpi;
  PRINTPAGE *lpElement;
  HANDLE hPrintDoc;
  HFONT hPrintFontOld;
  HDC hScreenDC;
  RECT rcHeader;
  RECT rcFooter;
  int nPageNumber=nInitPage;
  BOOL bPrintError=FALSE;
  BOOL bPrintStop=FALSE;

  //Set print settings
  prn->dwFlags=(nWrapType == AEWW_SYMBOL?AEPRN_WRAPSYMBOL:AEPRN_WRAPWORD)|
               (psdPageA.Flags & PSD_INHUNDREDTHSOFMILLIMETERS?AEPRN_INHUNDREDTHSOFMILLIMETERS:AEPRN_INTHOUSANDTHSOFINCHES)|
               (dwFlags & PRN_PREVIEW?AEPRN_TEST:0)|
               (dwFlags & PRN_ANSI?AEPRN_ANSI:0);
  if (!(dwFlags & PRN_RANGE))
  {
    if (dwFlags & PRN_SELECTION)
    {
      if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
        return 0;
      prn->crText=crSel;
    }
    else if (dwFlags & PRN_ALLTEXT)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&prn->crText.ciMin);
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&prn->crText.ciMax);
    }
  }
  if (bPrintFontEnable)
    prn->hEditFont=CreateFontIndirectA(&lfPrintFontA);
  else
    prn->hEditFont=CreateFontIndirectA(&lfEditFontA);
  prn->rcMargins=psdPageA.rtMargin;

  //Document properties
  if (dwFlags & PRN_REALPRINT)
  {
    API_LoadStringA(hLangLib, STR_DOCNAME, buf, BUFFER_SIZE);
    diA.cbSize=sizeof(DOCINFOA);
    diA.lpszDocName=(szCurrentFile[0])?szCurrentFile:buf;
    if (pdA.Flags & PD_PRINTTOFILE) diA.lpszOutput="FILE:";
  }

  if (!(dwFlags & PRN_REALPRINT) || StartDocA(prn->hPrinterDC, &diA) > 0)
  {
    if (hPrintDoc=(HANDLE)SendMessage(hWnd, AEM_STARTPRINTDOC, 0, (LPARAM)prn))
    {
      if (dwFlags & PRN_PREVIEW)
      {
        //Get DPI for converting coordinates
        if (hScreenDC=GetDC(hWndPreviewDlg))
        {
          ptScreenDpi.x=GetDeviceCaps(hScreenDC, LOGPIXELSX);
          ptScreenDpi.y=GetDeviceCaps(hScreenDC, LOGPIXELSY);
          ReleaseDC(hWndPreviewDlg, hScreenDC);
        }
        ptPrintDpi.x=GetDeviceCaps(prn->hPrinterDC, LOGPIXELSX);
        ptPrintDpi.y=GetDeviceCaps(prn->hPrinterDC, LOGPIXELSY);

        //Paper in screen coordinates
        rcPreviewPaper.left=2;
        rcPreviewPaper.top=2;
        rcPreviewPaper.right=rcPreviewPaper.left + MulDiv(ptScreenDpi.x, RectW(&prn->rcPageFull), ptPrintDpi.x);
        rcPreviewPaper.bottom=rcPreviewPaper.top + MulDiv(ptScreenDpi.y, RectH(&prn->rcPageFull), ptPrintDpi.y);
      }

      if (bPrintHeaderEnable)
      {
        rcHeader=prn->rcPageIn;
        rcHeader.bottom=rcHeader.top + prn->nCharHeight;
        prn->rcPageIn.top+=prn->nCharHeight;
      }
      if (bPrintFooterEnable)
      {
        rcFooter=prn->rcPageIn;
        rcFooter.top=rcFooter.bottom - prn->nCharHeight;
        prn->rcPageIn.bottom-=prn->nCharHeight;
      }

      while (!bPrintStop && !bPrintError)
      {
        ++nPageNumber;

        if (dwFlags & PRN_REALPRINT)
        {
          if ((pdA.Flags & PD_PAGENUMS) && nPageNumber > pdA.nToPage)
          {
            bPrintStop=TRUE;
            continue;
          }
          else if ((pdA.Flags & PD_PAGENUMS) && nPageNumber < pdA.nFromPage)
          {
            prn->dwFlags|=AEPRN_TEST;
            if (!SendMessage(hWnd, AEM_PRINTPAGE, (WPARAM)hPrintDoc, (LPARAM)prn))
              bPrintStop=TRUE;
            prn->dwFlags&=~AEPRN_TEST;
            continue;
          }
        }

        //Print page
        if (!(dwFlags & PRN_REALPRINT) || StartPage(prn->hPrinterDC) > 0)
        {
          if (dwFlags & PRN_PREVIEW)
          {
            if (dwFlags & PRN_SELECTION)
            {
              if (!StackInsertIndex((stack **)&hPreviewSelPagesStack.first, (stack **)&hPreviewSelPagesStack.last, (stack **)&lpElement, -1, sizeof(PRINTPAGE)))
                lpElement->crText=prn->crText;
            }
            else
            {
              if (!StackInsertIndex((stack **)&hPreviewAllPagesStack.first, (stack **)&hPreviewAllPagesStack.last, (stack **)&lpElement, -1, sizeof(PRINTPAGE)))
                lpElement->crText=prn->crText;
            }
          }

          if (bPrintHeaderEnable)
          {
            hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);
            if (!PrintHeadlineA(prn->hPrinterDC, &rcHeader, szPrintHeader, nPageNumber))
              bPrintStop=TRUE;
            if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
          }

          if (!SendMessage(hWnd, AEM_PRINTPAGE, (WPARAM)hPrintDoc, (LPARAM)prn))
            bPrintStop=TRUE;

          if (bPrintFooterEnable)
          {
            hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);
            if (!PrintHeadlineA(prn->hPrinterDC, &rcFooter, szPrintFooter, nPageNumber))
              bPrintStop=TRUE;
            if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
          }

          if (dwFlags & PRN_REALPRINT)
          {
            if (EndPage(prn->hPrinterDC) <= 0)
              bPrintError=TRUE;
          }
          if (dwFlags & PRN_ONEPAGE)
            break;
        }
        else bPrintError=TRUE;
      }
      SendMessage(hWnd, AEM_ENDPRINTDOC, (WPARAM)hPrintDoc, (LPARAM)prn);
    }
    if (dwFlags & PRN_REALPRINT) EndDoc(prn->hPrinterDC);
  }
  else bPrintError=TRUE;

  if (bPrintError)
  {
    API_LoadStringA(hLangLib, MSG_ERROR_PRINT, buf, BUFFER_SIZE);
    MessageBoxA(hMainWnd, buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
  }
  if (prn->hEditFont)
  {
    DeleteObject(prn->hEditFont);
    prn->hEditFont=NULL;
  }
  return nPageNumber;
}

int PrintDocumentW(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage)
{
  DOCINFOW diW={0};
  POINT ptScreenDpi;
  POINT ptPrintDpi;
  PRINTPAGE *lpElement;
  HANDLE hPrintDoc;
  HFONT hPrintFontOld;
  HDC hScreenDC;
  RECT rcHeader;
  RECT rcFooter;
  int nPageNumber=nInitPage;
  BOOL bPrintError=FALSE;
  BOOL bPrintStop=FALSE;

  //Set print settings
  prn->dwFlags=(nWrapType == AEWW_SYMBOL?AEPRN_WRAPSYMBOL:AEPRN_WRAPWORD)|
               (psdPageW.Flags & PSD_INHUNDREDTHSOFMILLIMETERS?AEPRN_INHUNDREDTHSOFMILLIMETERS:AEPRN_INTHOUSANDTHSOFINCHES)|
               (dwFlags & PRN_PREVIEW?AEPRN_TEST:0)|
               (dwFlags & PRN_ANSI?AEPRN_ANSI:0);
  if (!(dwFlags & PRN_RANGE))
  {
    if (dwFlags & PRN_SELECTION)
    {
      if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
        return 0;
      prn->crText=crSel;
    }
    else if (dwFlags & PRN_ALLTEXT)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&prn->crText.ciMin);
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&prn->crText.ciMax);
    }
  }
  if (bPrintFontEnable)
    prn->hEditFont=CreateFontIndirectW(&lfPrintFontW);
  else
    prn->hEditFont=CreateFontIndirectW(&lfEditFontW);
  prn->rcMargins=psdPageW.rtMargin;

  //Document properties
  if (dwFlags & PRN_REALPRINT)
  {
    API_LoadStringW(hLangLib, STR_DOCNAME, wbuf, BUFFER_SIZE);
    diW.cbSize=sizeof(DOCINFOW);
    diW.lpszDocName=(wszCurrentFile[0])?wszCurrentFile:wbuf;
    if (pdW.Flags & PD_PRINTTOFILE) diW.lpszOutput=L"FILE:";
  }

  if (!(dwFlags & PRN_REALPRINT) || StartDocW(prn->hPrinterDC, &diW) > 0)
  {
    if (hPrintDoc=(HANDLE)SendMessage(hWnd, AEM_STARTPRINTDOC, 0, (LPARAM)prn))
    {
      if (dwFlags & PRN_PREVIEW)
      {
        //Get DPI for converting coordinates
        if (hScreenDC=GetDC(hWndPreviewDlg))
        {
          ptScreenDpi.x=GetDeviceCaps(hScreenDC, LOGPIXELSX);
          ptScreenDpi.y=GetDeviceCaps(hScreenDC, LOGPIXELSY);
          ReleaseDC(hWndPreviewDlg, hScreenDC);
        }
        ptPrintDpi.x=GetDeviceCaps(prn->hPrinterDC, LOGPIXELSX);
        ptPrintDpi.y=GetDeviceCaps(prn->hPrinterDC, LOGPIXELSY);

        //Paper in screen coordinates
        rcPreviewPaper.left=2;
        rcPreviewPaper.top=2;
        rcPreviewPaper.right=rcPreviewPaper.left + MulDiv(ptScreenDpi.x, RectW(&prn->rcPageFull), ptPrintDpi.x);
        rcPreviewPaper.bottom=rcPreviewPaper.top + MulDiv(ptScreenDpi.y, RectH(&prn->rcPageFull), ptPrintDpi.y);
      }

      if (bPrintHeaderEnable)
      {
        rcHeader=prn->rcPageIn;
        rcHeader.bottom=rcHeader.top + prn->nCharHeight;
        prn->rcPageIn.top+=prn->nCharHeight;
      }
      if (bPrintFooterEnable)
      {
        rcFooter=prn->rcPageIn;
        rcFooter.top=rcFooter.bottom - prn->nCharHeight;
        prn->rcPageIn.bottom-=prn->nCharHeight;
      }

      while (!bPrintStop && !bPrintError)
      {
        ++nPageNumber;

        if (dwFlags & PRN_REALPRINT)
        {
          if ((pdW.Flags & PD_PAGENUMS) && nPageNumber > pdW.nToPage)
          {
            bPrintStop=TRUE;
            continue;
          }
          else if ((pdW.Flags & PD_PAGENUMS) && nPageNumber < pdW.nFromPage)
          {
            prn->dwFlags|=AEPRN_TEST;
            if (!SendMessage(hWnd, AEM_PRINTPAGE, (WPARAM)hPrintDoc, (LPARAM)prn))
              bPrintStop=TRUE;
            prn->dwFlags&=~AEPRN_TEST;
            continue;
          }
        }

        //Print page
        if (!(dwFlags & PRN_REALPRINT) || StartPage(prn->hPrinterDC) > 0)
        {
          if (dwFlags & PRN_PREVIEW)
          {
            if (dwFlags & PRN_SELECTION)
            {
              if (!StackInsertIndex((stack **)&hPreviewSelPagesStack.first, (stack **)&hPreviewSelPagesStack.last, (stack **)&lpElement, -1, sizeof(PRINTPAGE)))
                lpElement->crText=prn->crText;
            }
            else
            {
              if (!StackInsertIndex((stack **)&hPreviewAllPagesStack.first, (stack **)&hPreviewAllPagesStack.last, (stack **)&lpElement, -1, sizeof(PRINTPAGE)))
                lpElement->crText=prn->crText;
            }
          }

          if (bPrintHeaderEnable)
          {
            hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);
            if (!PrintHeadlineW(prn->hPrinterDC, &rcHeader, wszPrintHeader, nPageNumber))
              bPrintStop=TRUE;
            if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
          }

          if (!SendMessage(hWnd, AEM_PRINTPAGE, (WPARAM)hPrintDoc, (LPARAM)prn))
            bPrintStop=TRUE;

          if (bPrintFooterEnable)
          {
            hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);
            if (!PrintHeadlineW(prn->hPrinterDC, &rcFooter, wszPrintFooter, nPageNumber))
              bPrintStop=TRUE;
            if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
          }

          if (dwFlags & PRN_REALPRINT)
          {
            if (EndPage(prn->hPrinterDC) <= 0)
              bPrintError=TRUE;
          }
          if (dwFlags & PRN_ONEPAGE)
            break;
        }
        else bPrintError=TRUE;
      }
      SendMessage(hWnd, AEM_ENDPRINTDOC, (WPARAM)hPrintDoc, (LPARAM)prn);
    }
    if (dwFlags & PRN_REALPRINT) EndDoc(prn->hPrinterDC);
  }
  else bPrintError=TRUE;

  if (bPrintError)
  {
    API_LoadStringW(hLangLib, MSG_ERROR_PRINT, wbuf, BUFFER_SIZE);
    MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }
  if (prn->hEditFont)
  {
    DeleteObject(prn->hEditFont);
    prn->hEditFont=NULL;
  }
  return nPageNumber;
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

BOOL CALLBACK PreviewDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndPreview;
  static HWND hWndPrevPage;
  static HWND hWndPageCount;
  static HWND hWndNextPage;
  static HWND hWndZoom;
  static HWND hWndSelection;
  static HHOOK hHookKeys;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndPrevPage=GetDlgItem(hDlg, IDC_PREVIEW_PREVPAGE);
    hWndPageCount=GetDlgItem(hDlg, IDC_PREVIEW_PAGECOUNT);
    hWndNextPage=GetDlgItem(hDlg, IDC_PREVIEW_NEXTPAGE);
    hWndZoom=GetDlgItem(hDlg, IDC_PREVIEW_ZOOM);
    hWndZoomEdit=GetDlgItem(hWndZoom, IDC_COMBOBOXEDIT);
    hWndSelection=GetDlgItem(hDlg, IDC_PREVIEW_SELECTION);

    //Initialize
    hWndPreviewDlg=hDlg;
    hWndPreview=NULL;
    ptPreviewScroll.x=0;
    ptPreviewScroll.y=0;
    nPreviewZoomPercent=PREVIEW_ZOOMFIT;
    nPreviewPageCur=1;
    bPreviewSelection=FALSE;

    if (PreviewInitA(hWndSelection))
    {
      //Create preview window
      {
        WNDCLASSA wndclass={0};

        wndclass.style        =CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
        wndclass.lpfnWndProc  =PreviewProcA;
        wndclass.cbClsExtra   =0;
        wndclass.cbWndExtra   =DLGWINDOWEXTRA;
        wndclass.hInstance    =hInstance;
        wndclass.hIcon        =NULL;
        wndclass.hCursor      =LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
        wndclass.lpszMenuName =NULL;
        wndclass.lpszClassName=APP_PRINTPREVIEW_CLASSA;
        RegisterClassA(&wndclass);

        hWndPreview=CreateWindowA(APP_PRINTPREVIEW_CLASSA,
                                  NULL,
                                  WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                                  0, 0, 0, 0,
                                  hDlg,
                                  (HMENU)IDC_PREVIEW_BOX,
                                  hInstance,
                                  NULL);
      }

      //Fill zooms
      for (i=0; i <= nPreviewZoomMaxIndex; ++i)
      {
        wsprintfA(buf, "%d%%", lpZoom[i]);
        SendMessageA(hWndZoom, CB_ADDSTRING, 0, (LPARAM)buf);
      }
      API_LoadStringA(hLangLib, STR_PAGEFIT, buf, BUFFER_SIZE);
      SendMessageA(hWndZoom, CB_ADDSTRING, 0, (LPARAM)buf);
      API_LoadStringA(hLangLib, STR_PAGEWIDTH, buf, BUFFER_SIZE);
      SendMessageA(hWndZoom, CB_ADDSTRING, 0, (LPARAM)buf);

      hHookKeys=SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetCurrentThreadId());
    }
    ShowWindow(hDlg, SW_MAXIMIZE);
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      if (hWndPreview)
      {
        rcPreviewWindow.right=LOWORD(lParam);
        rcPreviewWindow.bottom=HIWORD(lParam);
        MoveWindow(hWndPreview, rcPreviewWindow.left, rcPreviewWindow.top, RectW(&rcPreviewWindow), RectH(&rcPreviewWindow), TRUE);
      }
    }
    return 0;
  }
  else if (uMsg == AKDLG_PREVIEWKEYDOWN)
  {
    if (wParam == VK_HOME && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_FIRSTPAGE, 0);
    }
    else if (wParam == VK_END && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_LASTPAGE, 0);
    }
    else if (wParam == VK_PRIOR)
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_PREVPAGE, 0);
    }
    else if (wParam == VK_NEXT)
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_NEXTPAGE, 0);
    }
    else if (wParam == VK_ADD && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMIN, 0);
    }
    else if (wParam == VK_SUBTRACT && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMOUT, 0);
    }
    else if (wParam == VK_RETURN)
    {
      if ((HWND)lParam == hWndZoomEdit)
      {
        GetWindowTextA(hWndZoomEdit, buf, BUFFER_SIZE);
        PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, xatoiA(buf), 0);
      }
      else if ((HWND)lParam == hWndPageCount)
      {
        GetWindowTextA(hWndPageCount, buf, BUFFER_SIZE);
        PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, xatoiA(buf), 0);
      }
      else return FALSE;
    }
    else return FALSE;

    SetWindowLongA(hDlg, DWL_MSGRESULT, 1);
    return TRUE;
  }
  else if (uMsg == AKDLG_PREVIEWMOUSEWHEEL)
  {
    if ((short)HIWORD(wParam) < 0)
    {
      if (GetKeyState(VK_CONTROL) & 0x80)
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMOUT, 0);
      else
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_NEXTPAGE, 0);
    }
    else
    {
      if (GetKeyState(VK_CONTROL) & 0x80)
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMIN, 0);
      else
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_PREVPAGE, 0);
    }
    SetWindowLongA(hDlg, DWL_MSGRESULT, 1);
    return TRUE;
  }
  else if (uMsg == AKDLG_PREVIEWSETZOOM)
  {
    int nZoom=wParam;

    if (nZoom == PREVIEW_ZOOMFIT ||
        nZoom == PREVIEW_ZOOMWIDTH)
    {
    }
    else if (nZoom < lpZoom[0])
    {
      nZoom=lpZoom[0];
    }
    else if (nZoom > lpZoom[nPreviewZoomMaxIndex])
    {
      nZoom=lpZoom[nPreviewZoomMaxIndex];
    }

    if (nZoom != nPreviewZoomPercent)
    {
      nPreviewZoomPercent=nZoom;
      InvalidateRect(hWndPreview, NULL, TRUE);
    }
    wsprintfA(buf, "%d%%", nPreviewZoomPercent);
    SetWindowTextA(hWndZoomEdit, buf);
  }
  else if (uMsg == AKDLG_PREVIEWSETPAGE)
  {
    int nPageSum=bPreviewSelection?nPreviewSelPageSum:nPreviewAllPageSum;
    int nPageCur=wParam;

    nPageCur=max(nPageCur, 1);
    nPageCur=min(nPageCur, nPageSum);

    if (nPageCur != nPreviewPageCur)
    {
      nPreviewPageCur=nPageCur;
      InvalidateRect(hWndPreview, NULL, TRUE);
    }
    if (nPreviewPageCur <= 1)
    {
      if (GetFocus() == hWndPrevPage)
        SetFocus(hWndPageCount);
      EnableWindow(hWndPrevPage, FALSE);
    }
    else EnableWindow(hWndPrevPage, TRUE);

    if (nPreviewPageCur >= nPageSum)
    {
      if (GetFocus() == hWndNextPage)
        SetFocus(hWndPageCount);
      EnableWindow(hWndNextPage, FALSE);
    }
    else EnableWindow(hWndNextPage, TRUE);

    wsprintfA(buf, "%d / %d", nPreviewPageCur, nPageSum);
    SetWindowTextA(hWndPageCount, buf);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PREVIEW_PRINT)
    {
      if (bPreviewSelection)
        pdA.Flags|=PD_SELECTION;
      SendMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
      PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_PRINT, (LPARAM)hWndPreviewEdit);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_PAGESETUP)
    {
      //Stop message hooking
      hWndPreviewDlg=NULL;

      if (DoFilePageSetupA(hDlg))
      {
        hWndPreviewDlg=hDlg;
        PreviewUninitA();
        if (PreviewInitA(hWndSelection))
          InvalidateRect(hWndPreview, NULL, TRUE);
      }
      hWndPreviewDlg=hDlg;
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_SELECTION)
    {
      nPreviewPageCur=1;
      bPreviewSelection=SendMessage(hWndSelection, BM_GETCHECK, 0, 0);
      PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur, 0);
      InvalidateRect(hWndPreview, NULL, TRUE);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_FIRSTPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, 1, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_LASTPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, bPreviewSelection?nPreviewSelPageSum:nPreviewAllPageSum, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_PREVPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur - 1, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_NEXTPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur + 1, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_ZOOM)
    {
      int nCurSel;

      if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        if ((nCurSel=SendMessage(hWndZoom, CB_GETCURSEL, 0, 0)) != CB_ERR)
        {
          PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[nCurSel], 0);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_ZOOMIN)
    {
      for (i=nPreviewZoomMaxIndex; i >= 0; --i)
      {
        if (lpZoom[i] <= nPreviewZoomPercent)
        {
          if (i < nPreviewZoomMaxIndex)
          {
            PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[i + 1], 0);
          }
          break;
        }
      }
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_ZOOMOUT)
    {
      for (i=0; i <= nPreviewZoomMaxIndex; ++i)
      {
        if (lpZoom[i] >= nPreviewZoomPercent)
        {
          if (i > 0)
          {
            PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[i - 1], 0);
          }
          break;
        }
      }
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      hWndPreviewDlg=NULL;
      PreviewUninitA();
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    if (hHookKeys)
    {
      if (UnhookWindowsHookEx(hHookKeys))
        hHookKeys=NULL;
    }
  }
  return FALSE;
}

BOOL CALLBACK PreviewDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndPreview;
  static HWND hWndPrevPage;
  static HWND hWndPageCount;
  static HWND hWndNextPage;
  static HWND hWndZoom;
  static HWND hWndSelection;
  static HHOOK hHookKeys;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndPrevPage=GetDlgItem(hDlg, IDC_PREVIEW_PREVPAGE);
    hWndPageCount=GetDlgItem(hDlg, IDC_PREVIEW_PAGECOUNT);
    hWndNextPage=GetDlgItem(hDlg, IDC_PREVIEW_NEXTPAGE);
    hWndZoom=GetDlgItem(hDlg, IDC_PREVIEW_ZOOM);
    hWndZoomEdit=GetDlgItem(hWndZoom, IDC_COMBOBOXEDIT);
    hWndSelection=GetDlgItem(hDlg, IDC_PREVIEW_SELECTION);

    //Initialize
    hWndPreviewDlg=hDlg;
    hWndPreview=NULL;
    ptPreviewScroll.x=0;
    ptPreviewScroll.y=0;
    nPreviewZoomPercent=PREVIEW_ZOOMFIT;
    nPreviewPageCur=1;
    bPreviewSelection=FALSE;

    if (PreviewInitW(hWndSelection))
    {
      //Create preview window
      {
        WNDCLASSW wndclass={0};

        wndclass.style        =CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
        wndclass.lpfnWndProc  =PreviewProcW;
        wndclass.cbClsExtra   =0;
        wndclass.cbWndExtra   =DLGWINDOWEXTRA;
        wndclass.hInstance    =hInstance;
        wndclass.hIcon        =NULL;
        wndclass.hCursor      =LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
        wndclass.lpszMenuName =NULL;
        wndclass.lpszClassName=APP_PRINTPREVIEW_CLASSW;
        RegisterClassW(&wndclass);

        hWndPreview=CreateWindowW(APP_PRINTPREVIEW_CLASSW,
                                  NULL,
                                  WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                                  0, 0, 0, 0,
                                  hDlg,
                                  (HMENU)IDC_PREVIEW_BOX,
                                  hInstance,
                                  NULL);
      }

      //Fill zooms
      for (i=0; i <= nPreviewZoomMaxIndex; ++i)
      {
        wsprintfW(wbuf, L"%d%%", lpZoom[i]);
        SendMessageW(hWndZoom, CB_ADDSTRING, 0, (LPARAM)wbuf);
      }
      API_LoadStringW(hLangLib, STR_PAGEFIT, wbuf, BUFFER_SIZE);
      SendMessageW(hWndZoom, CB_ADDSTRING, 0, (LPARAM)wbuf);
      API_LoadStringW(hLangLib, STR_PAGEWIDTH, wbuf, BUFFER_SIZE);
      SendMessageW(hWndZoom, CB_ADDSTRING, 0, (LPARAM)wbuf);

      hHookKeys=SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetCurrentThreadId());
    }
    ShowWindow(hDlg, SW_MAXIMIZE);
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      if (hWndPreview)
      {
        rcPreviewWindow.right=LOWORD(lParam);
        rcPreviewWindow.bottom=HIWORD(lParam);
        MoveWindow(hWndPreview, rcPreviewWindow.left, rcPreviewWindow.top, RectW(&rcPreviewWindow), RectH(&rcPreviewWindow), TRUE);
      }
    }
    return 0;
  }
  else if (uMsg == AKDLG_PREVIEWKEYDOWN)
  {
    if (wParam == VK_HOME && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_FIRSTPAGE, 0);
    }
    else if (wParam == VK_END && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_LASTPAGE, 0);
    }
    else if (wParam == VK_PRIOR)
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_PREVPAGE, 0);
    }
    else if (wParam == VK_NEXT)
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_NEXTPAGE, 0);
    }
    else if (wParam == VK_ADD && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMIN, 0);
    }
    else if (wParam == VK_SUBTRACT && (GetKeyState(VK_CONTROL) & 0x80))
    {
      PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMOUT, 0);
    }
    else if (wParam == VK_RETURN)
    {
      if ((HWND)lParam == hWndZoomEdit)
      {
        GetWindowTextW(hWndZoomEdit, wbuf, BUFFER_SIZE);
        PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, xatoiW(wbuf), 0);
      }
      else if ((HWND)lParam == hWndPageCount)
      {
        GetWindowTextW(hWndPageCount, wbuf, BUFFER_SIZE);
        PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, xatoiW(wbuf), 0);
      }
      else return FALSE;
    }
    else return FALSE;

    SetWindowLongW(hDlg, DWL_MSGRESULT, 1);
    return TRUE;
  }
  else if (uMsg == AKDLG_PREVIEWMOUSEWHEEL)
  {
    if ((short)HIWORD(wParam) < 0)
    {
      if (GetKeyState(VK_CONTROL) & 0x80)
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMOUT, 0);
      else
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_NEXTPAGE, 0);
    }
    else
    {
      if (GetKeyState(VK_CONTROL) & 0x80)
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_ZOOMIN, 0);
      else
        PostMessage(hDlg, WM_COMMAND, IDC_PREVIEW_PREVPAGE, 0);
    }
    SetWindowLongW(hDlg, DWL_MSGRESULT, 1);
    return TRUE;
  }
  else if (uMsg == AKDLG_PREVIEWSETZOOM)
  {
    int nZoom=wParam;

    if (nZoom == PREVIEW_ZOOMFIT ||
        nZoom == PREVIEW_ZOOMWIDTH)
    {
    }
    else if (nZoom < lpZoom[0])
    {
      nZoom=lpZoom[0];
    }
    else if (nZoom > lpZoom[nPreviewZoomMaxIndex])
    {
      nZoom=lpZoom[nPreviewZoomMaxIndex];
    }

    if (nZoom != nPreviewZoomPercent)
    {
      nPreviewZoomPercent=nZoom;
      InvalidateRect(hWndPreview, NULL, TRUE);
    }
    wsprintfW(wbuf, L"%d%%", nPreviewZoomPercent);
    SetWindowTextW(hWndZoomEdit, wbuf);
  }
  else if (uMsg == AKDLG_PREVIEWSETPAGE)
  {
    int nPageSum=bPreviewSelection?nPreviewSelPageSum:nPreviewAllPageSum;
    int nPageCur=wParam;

    nPageCur=max(nPageCur, 1);
    nPageCur=min(nPageCur, nPageSum);

    if (nPageCur != nPreviewPageCur)
    {
      nPreviewPageCur=nPageCur;
      InvalidateRect(hWndPreview, NULL, TRUE);
    }
    if (nPreviewPageCur <= 1)
    {
      if (GetFocus() == hWndPrevPage)
        SetFocus(hWndPageCount);
      EnableWindow(hWndPrevPage, FALSE);
    }
    else EnableWindow(hWndPrevPage, TRUE);

    if (nPreviewPageCur >= nPageSum)
    {
      if (GetFocus() == hWndNextPage)
        SetFocus(hWndPageCount);
      EnableWindow(hWndNextPage, FALSE);
    }
    else EnableWindow(hWndNextPage, TRUE);

    wsprintfW(wbuf, L"%d / %d", nPreviewPageCur, nPageSum);
    SetWindowTextW(hWndPageCount, wbuf);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PREVIEW_PRINT)
    {
      if (bPreviewSelection)
        pdW.Flags|=PD_SELECTION;
      SendMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
      PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_PRINT, (LPARAM)hWndPreviewEdit);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_PAGESETUP)
    {
      //Stop message hooking
      hWndPreviewDlg=NULL;

      if (DoFilePageSetupW(hDlg))
      {
        hWndPreviewDlg=hDlg;
        PreviewUninitW();
        if (PreviewInitW(hWndSelection))
          InvalidateRect(hWndPreview, NULL, TRUE);
      }
      hWndPreviewDlg=hDlg;
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_SELECTION)
    {
      nPreviewPageCur=1;
      bPreviewSelection=SendMessage(hWndSelection, BM_GETCHECK, 0, 0);
      PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur, 0);
      InvalidateRect(hWndPreview, NULL, TRUE);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_FIRSTPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, 1, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_LASTPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, bPreviewSelection?nPreviewSelPageSum:nPreviewAllPageSum, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_PREVPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur - 1, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_NEXTPAGE)
    {
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur + 1, 0);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_ZOOM)
    {
      int nCurSel;

      if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        if ((nCurSel=SendMessage(hWndZoom, CB_GETCURSEL, 0, 0)) != CB_ERR)
        {
          PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[nCurSel], 0);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_ZOOMIN)
    {
      for (i=nPreviewZoomMaxIndex; i >= 0; --i)
      {
        if (lpZoom[i] <= nPreviewZoomPercent)
        {
          if (i < nPreviewZoomMaxIndex)
          {
            PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[i + 1], 0);
          }
          break;
        }
      }
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_ZOOMOUT)
    {
      for (i=0; i <= nPreviewZoomMaxIndex; ++i)
      {
        if (lpZoom[i] >= nPreviewZoomPercent)
        {
          if (i > 0)
          {
            PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[i - 1], 0);
          }
          break;
        }
      }
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      hWndPreviewDlg=NULL;
      PreviewUninitW();
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    if (hHookKeys)
    {
      if (UnhookWindowsHookEx(hHookKeys))
        hHookKeys=NULL;
    }
  }
  return FALSE;
}

BOOL PreviewInitA(HWND hWndSelection)
{
  BOOL bResult=FALSE;

  memset(&rcPreviewZoomed, 0, sizeof(RECT));
  nPreviewAllPageSum=0;
  nPreviewSelPageSum=0;

  //Get printer DC
  if (!pdA.hDC)
  {
    GetPrinterDCA(&pdA);
    psdPageA.hDevMode=pdA.hDevMode;
    psdPageA.hDevNames=pdA.hDevNames;
  }

  if (pdA.hDC)
  {
    //Initialize variables
    prn.hPrinterDC=pdA.hDC;
    nPreviewAllPageSum=PrintDocumentA(hWndPreviewEdit, &prn, PRN_PREVIEW|PRN_ALLTEXT, 0);
    nPreviewSelPageSum=PrintDocumentA(hWndPreviewEdit, &prn, PRN_PREVIEW|PRN_SELECTION, 0);
    bResult=TRUE;
  }
  if (!nPreviewSelPageSum)
    EnableWindow(hWndSelection, FALSE);
  PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur, 0);
  return bResult;
}

BOOL PreviewInitW(HWND hWndSelection)
{
  BOOL bResult=FALSE;

  memset(&rcPreviewZoomed, 0, sizeof(RECT));
  nPreviewAllPageSum=0;
  nPreviewSelPageSum=0;

  //Get printer DC
  if (!pdW.hDC)
  {
    GetPrinterDCW(&pdW);
    psdPageW.hDevMode=pdW.hDevMode;
    psdPageW.hDevNames=pdW.hDevNames;
  }

  if (pdW.hDC)
  {
    //Initialize variables
    prn.hPrinterDC=pdW.hDC;
    nPreviewAllPageSum=PrintDocumentW(hWndPreviewEdit, &prn, PRN_PREVIEW|PRN_ALLTEXT, 0);
    nPreviewSelPageSum=PrintDocumentW(hWndPreviewEdit, &prn, PRN_PREVIEW|PRN_SELECTION, 0);
    bResult=TRUE;
  }
  if (!nPreviewSelPageSum)
    EnableWindow(hWndSelection, FALSE);
  PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur, 0);
  return bResult;
}

void PreviewUninitA()
{
  StackClear((stack **)&hPreviewAllPagesStack.first, (stack **)&hPreviewAllPagesStack.last);
  StackClear((stack **)&hPreviewSelPagesStack.first, (stack **)&hPreviewSelPagesStack.last);

  if (pdA.hDC)
  {
    DeleteDC(pdA.hDC);
    pdA.hDC=NULL;
  }
}

void PreviewUninitW()
{
  StackClear((stack **)&hPreviewAllPagesStack.first, (stack **)&hPreviewAllPagesStack.last);
  StackClear((stack **)&hPreviewSelPagesStack.first, (stack **)&hPreviewSelPagesStack.last);

  if (pdW.hDC)
  {
    DeleteDC(pdW.hDC);
    pdW.hDC=NULL;
  }
}

LRESULT CALLBACK PreviewProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_PAINT)
  {
    PAINTSTRUCT ps;

    if (BeginPaint(hWnd, &ps))
    {
      if (ps.rcPaint.right - ps.rcPaint.left > 0 &&
          ps.rcPaint.bottom - ps.rcPaint.top > 0)
      {
        PRINTPAGE *lpElement;
        HENHMETAFILE hMetaFile;
        HDC hMetaDC;
        RECT rcMeta;

        if ((bPreviewSelection == FALSE && !StackGetElement((stack *)hPreviewAllPagesStack.first, (stack *)hPreviewAllPagesStack.last, (stack **)&lpElement, nPreviewPageCur)) ||
            (bPreviewSelection == TRUE && !StackGetElement((stack *)hPreviewSelPagesStack.first, (stack *)hPreviewSelPagesStack.last, (stack **)&lpElement, nPreviewPageCur)))
        {
          //Create the EMF in memory
          rcMeta.left=0;
          rcMeta.top=0;
          rcMeta.right=GetDeviceCaps(pdA.hDC, HORZSIZE) * 100;
          rcMeta.bottom=GetDeviceCaps(pdA.hDC, VERTSIZE) * 100;
          hMetaDC=CreateEnhMetaFileA(pdA.hDC, NULL, &rcMeta, NULL);

          //Print page on metafile device
          prn.hPrinterDC=hMetaDC;
          prn.crText=lpElement->crText;
          PrintDocumentA(hWndPreviewEdit, &prn, PRN_ANSI|PRN_RANGE|PRN_ONEPAGE, nPreviewPageCur - 1);
          hMetaFile=CloseEnhMetaFile(hMetaDC);

          //Draw page on window
          PreviewPaint(hWnd, ps.hdc, hMetaFile);
          DeleteEnhMetaFile(hMetaFile);

          //Set zoom text
          wsprintfA(buf, "%d%%", nPreviewZoomPercent);
          SetWindowTextA(hWndZoomEdit, buf);
        }
      }
      EndPaint(hWnd, &ps);
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      PreviewScrollUpdate(hWnd);
    }
  }
  else if (uMsg == WM_HSCROLL)
  {
    return PreviewHScroll(hWnd, LOWORD(wParam));
  }
  else if (uMsg == WM_VSCROLL)
  {
    return PreviewVScroll(hWnd, LOWORD(wParam));
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    if (nPreviewZoomPercent == 100)
      PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETZOOM, (WPARAM)PREVIEW_ZOOMFIT, 0);
    else
      PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETZOOM, 100, 0);
  }
  return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK PreviewProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_PAINT)
  {
    PAINTSTRUCT ps;

    if (BeginPaint(hWnd, &ps))
    {
      if (ps.rcPaint.right - ps.rcPaint.left > 0 &&
          ps.rcPaint.bottom - ps.rcPaint.top > 0)
      {
        PRINTPAGE *lpElement;
        HENHMETAFILE hMetaFile;
        HDC hMetaDC;
        RECT rcMeta;

        if ((bPreviewSelection == FALSE && !StackGetElement((stack *)hPreviewAllPagesStack.first, (stack *)hPreviewAllPagesStack.last, (stack **)&lpElement, nPreviewPageCur)) ||
            (bPreviewSelection == TRUE && !StackGetElement((stack *)hPreviewSelPagesStack.first, (stack *)hPreviewSelPagesStack.last, (stack **)&lpElement, nPreviewPageCur)))
        {
          //Create the EMF in memory
          rcMeta.left=0;
          rcMeta.top=0;
          rcMeta.right=GetDeviceCaps(pdW.hDC, HORZSIZE) * 100;
          rcMeta.bottom=GetDeviceCaps(pdW.hDC, VERTSIZE) * 100;
          hMetaDC=CreateEnhMetaFileW(pdW.hDC, NULL, &rcMeta, NULL);

          //Print page on metafile device
          prn.hPrinterDC=hMetaDC;
          prn.crText=lpElement->crText;
          PrintDocumentW(hWndPreviewEdit, &prn, PRN_RANGE|PRN_ONEPAGE, nPreviewPageCur - 1);
          hMetaFile=CloseEnhMetaFile(hMetaDC);

          //Draw page on window
          PreviewPaint(hWnd, ps.hdc, hMetaFile);
          DeleteEnhMetaFile(hMetaFile);

          //Set zoom text
          wsprintfW(wbuf, L"%d%%", nPreviewZoomPercent);
          SetWindowTextW(hWndZoomEdit, wbuf);
        }
      }
      EndPaint(hWnd, &ps);
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      PreviewScrollUpdate(hWnd);
    }
  }
  else if (uMsg == WM_HSCROLL)
  {
    return PreviewHScroll(hWnd, LOWORD(wParam));
  }
  else if (uMsg == WM_VSCROLL)
  {
    return PreviewVScroll(hWnd, LOWORD(wParam));
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    if (nPreviewZoomPercent == 100)
      PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETZOOM, (WPARAM)PREVIEW_ZOOMFIT, 0);
    else
      PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETZOOM, 100, 0);
  }
  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

void PreviewPaint(HWND hWnd, HDC hPaintDC, HENHMETAFILE hMetaFile)
{
  HDC hBufferDC;
  HBITMAP hBitmap;
  HBITMAP hBitmapOld=NULL;
  HBRUSH hBrush;
  RECT rcPreviewIn;
  RECT rcPreviewClient;
  RECT rcPaperFrame;
  SIZE sizePreview;

  //Zooming
  if (nPreviewZoomPercent == PREVIEW_ZOOMFIT)
  {
    FitInside(RectW(&rcPreviewPaper), RectH(&rcPreviewPaper), RectW(&rcPreviewWindow) - rcPreviewPaper.left * 2, RectH(&rcPreviewWindow) - rcPreviewPaper.top * 2, &sizePreview);
    nPreviewZoomPercent=sizePreview.cx * 100 / RectW(&rcPreviewPaper);
  }
  else if (nPreviewZoomPercent == PREVIEW_ZOOMWIDTH)
  {
    nPreviewZoomPercent=(RectW(&rcPreviewWindow) - rcPreviewPaper.left * 2 - GetSystemMetrics(SM_CXVSCROLL)) * 100 / RectW(&rcPreviewPaper);
  }
  rcPreviewZoomed.left=rcPreviewPaper.left;
  rcPreviewZoomed.top=rcPreviewPaper.top;
  rcPreviewZoomed.right=rcPreviewZoomed.left + MulDiv(RectW(&rcPreviewPaper), nPreviewZoomPercent, 100);
  rcPreviewZoomed.bottom=rcPreviewZoomed.top + MulDiv(RectH(&rcPreviewPaper), nPreviewZoomPercent, 100);

  //Update scroll
  PreviewScrollUpdate(hWnd);
  PreviewScrollUpdate(hWnd); //Client rect can be changed.
  rcPreviewZoomed.left-=ptPreviewScroll.x;
  rcPreviewZoomed.top-=ptPreviewScroll.y;
  rcPreviewZoomed.right-=ptPreviewScroll.x;
  rcPreviewZoomed.bottom-=ptPreviewScroll.y;

  //Create compatible DC to avoid flicking
  hBufferDC=CreateCompatibleDC(hPaintDC);
  hBitmap=CreateCompatibleBitmap(hPaintDC, RectW(&rcPreviewWindow), RectH(&rcPreviewWindow));
  hBitmapOld=(HBITMAP)SelectObject(hBufferDC, hBitmap);

  //Erase preview window contents
  GetClientRect(hWnd, &rcPreviewClient);
  FillRect(hBufferDC, &rcPreviewClient, (HBRUSH)(COLOR_BTNFACE + 1));

  //Set margins color
  if (hBrush=CreateSolidBrush(RGB(0xE0, 0xE0, 0xE0)))
  {
    FillRect(hBufferDC, &rcPreviewZoomed, hBrush);
    DeleteObject(hBrush);
  }

  //Paper frame
  rcPaperFrame.left=rcPreviewZoomed.left - 1;
  rcPaperFrame.top=rcPreviewZoomed.top - 1;
  rcPaperFrame.right=rcPreviewZoomed.right + 1;
  rcPaperFrame.bottom=rcPreviewZoomed.bottom + 1;
  FrameRect(hBufferDC, &rcPaperFrame, (HBRUSH)GetStockObject(BLACK_BRUSH));

  //Draw area
  if (bPrintHeaderEnable)
    prn.rcPageIn.top-=prn.nCharHeight;
  if (bPrintFooterEnable)
    prn.rcPageIn.bottom+=prn.nCharHeight;
  rcPreviewIn.left=rcPreviewZoomed.left + MulDiv(RectW(&rcPreviewZoomed), prn.rcPageIn.left - prn.rcPageFull.left, RectW(&prn.rcPageFull));
  rcPreviewIn.top=rcPreviewZoomed.top + MulDiv(RectH(&rcPreviewZoomed), prn.rcPageIn.top - prn.rcPageFull.top, RectH(&prn.rcPageFull));
  rcPreviewIn.right=rcPreviewIn.left + MulDiv(RectW(&rcPreviewZoomed), RectW(&prn.rcPageIn), RectW(&prn.rcPageFull));
  rcPreviewIn.bottom=rcPreviewIn.top + MulDiv(RectH(&rcPreviewZoomed), RectH(&prn.rcPageIn), RectH(&prn.rcPageFull));

  //Get character size
  nPreviewAveCharWidth=MulDiv(RectW(&rcPreviewZoomed), prn.nAveCharWidth, RectW(&prn.rcPageFull));
  nPreviewCharHeight=MulDiv(RectW(&rcPreviewZoomed), prn.nCharHeight, RectW(&prn.rcPageFull));

  //Set draw area color
  if (hBrush=CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF)))
  {
    FillRect(hBufferDC, &rcPreviewIn, hBrush);
    DeleteObject(hBrush);
  }

  //Draw text
  PlayEnhMetaFile(hBufferDC, hMetaFile, &rcPreviewZoomed);

  //Transfer image
  BitBlt(hPaintDC, 0, 0, RectW(&rcPreviewWindow), RectH(&rcPreviewWindow), hBufferDC, 0, 0, SRCCOPY);

  //Clean up
  if (hBitmapOld) SelectObject(hBufferDC, hBitmapOld);
  DeleteObject(hBitmap);
  DeleteDC(hBufferDC);
}

int PreviewHScroll(HWND hWnd, int nAction)
{
  SCROLLINFO si;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(hWnd, SB_HORZ, &si);

  if (nAction == SB_LEFT)
  {
    si.nPos=si.nMin;
  }
  else if (nAction == SB_RIGHT)
  {
    si.nPos=si.nMax;
  }
  else if (nAction == SB_LINELEFT)
  {
    si.nPos-=nPreviewAveCharWidth;
  }
  else if (nAction == SB_LINERIGHT)
  {
    si.nPos+=nPreviewAveCharWidth;
  }
  else if (nAction == SB_PAGELEFT)
  {
    si.nPos-=si.nPage;
  }
  else if (nAction == SB_PAGERIGHT)
  {
    si.nPos+=si.nPage;
  }
  else if (nAction == SB_THUMBTRACK)
  {
    si.nPos=si.nTrackPos;
  }
  else if (nAction == SB_THUMBPOSITION)
  {
  }
  si.fMask=SIF_POS;
  SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
  si.fMask=SIF_POS;
  GetScrollInfo(hWnd, SB_HORZ, &si);

  ptPreviewScroll.x=si.nPos;
  InvalidateRect(hWnd, NULL, FALSE);
  return 0;
}

int PreviewVScroll(HWND hWnd, int nAction)
{
  SCROLLINFO si;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(hWnd, SB_VERT, &si);

  if (nAction == SB_TOP)
  {
    si.nPos=si.nMin;
  }
  else if (nAction == SB_BOTTOM)
  {
    si.nPos=si.nMax;
  }
  else if (nAction == SB_LINEUP)
  {
    si.nPos-=nPreviewCharHeight;
  }
  else if (nAction == SB_LINEDOWN)
  {
    si.nPos+=nPreviewCharHeight;
  }
  else if (nAction == SB_PAGEUP)
  {
    si.nPos-=si.nPage;
  }
  else if (nAction == SB_PAGEDOWN)
  {
    si.nPos+=si.nPage;
  }
  else if (nAction == SB_THUMBTRACK)
  {
    si.nPos=si.nTrackPos;
  }
  else if (nAction == SB_THUMBPOSITION)
  {
  }
  si.fMask=SIF_POS;
  SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
  si.fMask=SIF_POS;
  GetScrollInfo(hWnd, SB_VERT, &si);

  ptPreviewScroll.y=si.nPos;
  InvalidateRect(hWnd, NULL, FALSE);
  return 0;
}

void PreviewScrollUpdate(HWND hWnd)
{
  SCROLLINFO si;
  RECT rcPreviewClient;
  int nHorzMax;
  int nVertMax;

  if (rcPreviewZoomed.right && rcPreviewZoomed.bottom)
  {
    nHorzMax=RectW(&rcPreviewZoomed) + rcPreviewPaper.left * 2 - 1;
    nVertMax=RectH(&rcPreviewZoomed) + rcPreviewPaper.top * 2 - 1;
    GetClientRect(hWnd, &rcPreviewClient);

    si.cbSize=sizeof(SCROLLINFO);
    si.fMask=SIF_ALL;
    GetScrollInfo(hWnd, SB_HORZ, &si);

    if (si.nMax != nHorzMax ||
        si.nPage != (DWORD)rcPreviewClient.right)
    {
      si.fMask=SIF_RANGE|SIF_PAGE;
      si.nMin=0;
      si.nMax=nHorzMax;
      si.nPage=rcPreviewClient.right;
      SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

      si.fMask=SIF_POS;
      GetScrollInfo(hWnd, SB_HORZ, &si);

      ptPreviewScroll.x=si.nPos;
    }

    si.cbSize=sizeof(SCROLLINFO);
    si.fMask=SIF_ALL;
    GetScrollInfo(hWnd, SB_VERT, &si);

    if (si.nMax != nVertMax ||
        si.nPage != (DWORD)rcPreviewClient.bottom)
    {
      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_RANGE|SIF_PAGE;
      si.nMin=0;
      si.nMax=nVertMax;
      si.nPage=rcPreviewClient.bottom;
      SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

      si.fMask=SIF_POS;
      GetScrollInfo(hWnd, SB_VERT, &si);

      ptPreviewScroll.y=si.nPos;
    }
  }
}

BOOL FitInside(int nWidth, int nHeight, int nMaxWidth, int nMaxHeight, SIZE *s)
{
  s->cx=nMaxHeight * nWidth / nHeight;
  s->cy=nMaxWidth * nHeight / nWidth;

  if (s->cx <= nMaxWidth)
  {
    s->cy=s->cx * nHeight / nWidth; //portrait
    return TRUE;
  }
  else
  {
    s->cx=s->cy * nWidth / nHeight; //portrait
    return FALSE;
  }
}

int RectW(const RECT *rc)
{
  return rc->right - rc->left;
}

int RectH(const RECT *rc)
{
  return rc->bottom - rc->top;
}


//// Code pages

unsigned int CALLBACK CodePageDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCP;
  static HWND hWndFilePreview;
  static HWND hWndAutodetect;
  static HWND hDlgEdit;
  static RECT rcDlg;
  static RECT rcCodePage;
  static RECT rcAutodetect;
  static RECT rcFilePreview;
  static char szFile[MAX_PATH];
  static int nCodePage;
  static BOOL bBOM;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    if (bMDI) hDlgEdit=GetDlgItem(GetParent(hDlg), IDC_OFN_EDIT);
    hWndCP=GetDlgItem(hDlg, IDC_OFN_CODEPAGE);
    hWndFilePreview=GetDlgItem(hDlg, IDC_OFN_PREVIEW);
    hWndAutodetect=GetDlgItem(hDlg, IDC_OFN_AUTODETECT);

    nCodePage=nCurrentCodePage;
    FillComboboxCodepageA(hWndCP, lpCodepageList);
    SelectComboboxCodepageA(hWndCP, nCodePage);

    GetWindowPos(hDlg, NULL, &rcDlg);
    GetWindowPos(hWndCP, hDlg, &rcCodePage);
    GetWindowPos(hWndAutodetect, hDlg, &rcAutodetect);
    GetWindowPos(hWndFilePreview, hDlg, &rcFilePreview);

    if (bSaveDlg)
    {
      SetWindowPos(hDlg, 0, 0, 0, rcDlg.right, rcFilePreview.top, SWP_NOMOVE|SWP_NOZORDER);
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
    SendMessage(hWndFilePreview, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
    SetTabStops(hWndFilePreview, nTabStopSize, FALSE);
    SetChosenFontA(hWndFilePreview, &lfEditFontA, FALSE);

    OldFilePreviewProc=(WNDPROC)GetWindowLongA(hWndFilePreview, GWL_WNDPROC);
    SetWindowLongA(hWndFilePreview, GWL_WNDPROC, (LONG)NewFilePreviewProcA);
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
    rcFilePreview.right+=x;

    SetWindowPos(hWndCP, 0, 0, 0, rcCodePage.right, rcCodePage.bottom, SWP_NOMOVE|SWP_NOZORDER);
    SetWindowPos(hWndAutodetect, 0, rcAutodetect.left, rcAutodetect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
    SetWindowPos(hWndFilePreview, 0, 0, 0, rcFilePreview.right, rcFilePreview.bottom, SWP_NOMOVE|SWP_NOZORDER);
    return 0;
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndFilePreview)
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

        if (FilePreviewA(hWndFilePreview, szFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageA(hWndCP, nDefaultCodePage);
          SetWindowTextA(hWndFilePreview, "");
          return TRUE;
        }
        EnableWindow(hWndFilePreview, TRUE);
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
        if (FilePreviewA(hWndFilePreview, szFile, PREVIEW_SIZE, OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          SetWindowTextA(hWndFilePreview, "");
        }
        else EnableWindow(hWndFilePreview, TRUE);
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

        if (FilePreviewA(hWndFilePreview, szFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageA(hWndCP, nDefaultCodePage);
          SetWindowTextA(hWndFilePreview, "");
          return TRUE;
        }
        EnableWindow(hWndFilePreview, TRUE);
        if (bAutodetect) SelectComboboxCodepageA(hWndCP, nCodePage);
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    HFONT hFont;

    if (hFont=(HFONT)SendMessage(hWndFilePreview, WM_GETFONT, 0, 0))
      DeleteObject(hFont);
  }
  return FALSE;
}

unsigned int CALLBACK CodePageDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCP;
  static HWND hWndFilePreview;
  static HWND hWndAutodetect;
  static HWND hDlgEdit;
  static RECT rcDlg;
  static RECT rcCodePage;
  static RECT rcAutodetect;
  static RECT rcFilePreview;
  static wchar_t wszFile[MAX_PATH];
  static int nCodePage;
  static BOOL bBOM;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    if (bMDI) hDlgEdit=GetDlgItem(GetParent(hDlg), IDC_OFN_EDIT);
    hWndCP=GetDlgItem(hDlg, IDC_OFN_CODEPAGE);
    hWndFilePreview=GetDlgItem(hDlg, IDC_OFN_PREVIEW);
    hWndAutodetect=GetDlgItem(hDlg, IDC_OFN_AUTODETECT);

    nCodePage=nCurrentCodePage;
    FillComboboxCodepageW(hWndCP, lpCodepageList);
    SelectComboboxCodepageW(hWndCP, nCodePage);

    GetWindowPos(hDlg, NULL, &rcDlg);
    GetWindowPos(hWndCP, hDlg, &rcCodePage);
    GetWindowPos(hWndAutodetect, hDlg, &rcAutodetect);
    GetWindowPos(hWndFilePreview, hDlg, &rcFilePreview);

    if (bSaveDlg)
    {
      SetWindowPos(hDlg, 0, 0, 0, rcDlg.right, rcFilePreview.top, SWP_NOMOVE|SWP_NOZORDER);
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
    SendMessage(hWndFilePreview, AEM_SETCOLORS, 0, (LPARAM)&aecColors);
    SetTabStops(hWndFilePreview, nTabStopSize, FALSE);
    SetChosenFontW(hWndFilePreview, &lfEditFontW, FALSE);

    OldFilePreviewProc=(WNDPROC)GetWindowLongW(hWndFilePreview, GWL_WNDPROC);
    SetWindowLongW(hWndFilePreview, GWL_WNDPROC, (LONG)NewFilePreviewProcW);
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
    rcFilePreview.right+=x;

    SetWindowPos(hWndCP, 0, 0, 0, rcCodePage.right, rcCodePage.bottom, SWP_NOMOVE|SWP_NOZORDER);
    SetWindowPos(hWndAutodetect, 0, rcAutodetect.left, rcAutodetect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
    SetWindowPos(hWndFilePreview, 0, 0, 0, rcFilePreview.right, rcFilePreview.bottom, SWP_NOMOVE|SWP_NOZORDER);
    return 0;
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndFilePreview)
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

        if (FilePreviewW(hWndFilePreview, wszFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageW(hWndCP, nDefaultCodePage);
          SetWindowTextW(hWndFilePreview, L"");
          return TRUE;
        }
        EnableWindow(hWndFilePreview, TRUE);
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
        if (FilePreviewW(hWndFilePreview, wszFile, PREVIEW_SIZE, OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          SetWindowTextW(hWndFilePreview, L"");
        }
        else EnableWindow(hWndFilePreview, TRUE);
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

        if (FilePreviewW(hWndFilePreview, wszFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          if (bAutodetect) SelectComboboxCodepageW(hWndCP, nDefaultCodePage);
          SetWindowTextW(hWndFilePreview, L"");
          return TRUE;
        }
        EnableWindow(hWndFilePreview, TRUE);
        if (bAutodetect) SelectComboboxCodepageW(hWndCP, nCodePage);
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    HFONT hFont;

    if (hFont=(HFONT)SendMessage(hWndFilePreview, WM_GETFONT, 0, 0))
      DeleteObject(hFont);
  }
  return FALSE;
}

LRESULT CALLBACK NewFilePreviewProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_ESCAPE)
    {
      PostMessage(GetParent(hWnd), WM_KEYDOWN, VK_ESCAPE, 0);
      return TRUE;
    }
  }
  return CallWindowProcA(OldFilePreviewProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewFilePreviewProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_ESCAPE)
    {
      PostMessage(GetParent(hWnd), WM_KEYDOWN, VK_ESCAPE, 0);
      return TRUE;
    }
  }
  return CallWindowProcW(OldFilePreviewProc, hWnd, uMsg, wParam, lParam);
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

int EnumCodepageList(int **lpCodepageList)
{
  int *lpCodepageListCount;
  int i;

  lpCodepageTable=NULL;
  nCodepageTableCount=0;

  if (lpCodepageTable=(int *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(int) * 65536))
  {
    if (bOldWindows)
      RegEnumSystemCodePagesA();
    else
      EnumSystemCodePagesW(EnumCodePagesProc, CP_INSTALLED);

    //CP_ACP, CP_OEMCP, CP_UNICODE_UCS2_LE, CP_UNICODE_UCS2_BE, CP_UNICODE_UTF8, CP_UNICODE_UTF7, 0
    nCodepageTableCount+=7;

    if (*lpCodepageList=lpCodepageListCount=(int *)API_HeapAlloc(hHeap, 0, sizeof(int) * nCodepageTableCount))
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
    lpCodepageTable=NULL;
  }
  return nCodepageTableCount;
}

void RegEnumSystemCodePagesA()
{
  HKEY hKey;
  DWORD dwSizeValue;
  DWORD dwSizeString;
  DWORD dwType;
  DWORD dwIndex=0;
  int i;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Nls\\CodePage", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    while(1)
    {
      dwSizeValue=BUFFER_SIZE;
      dwSizeString=BUFFER_SIZE;
      buf2[0]='\0';

      if (RegEnumValueA(hKey, dwIndex++, buf, &dwSizeValue, NULL, &dwType, (LPBYTE)buf2, &dwSizeString) != ERROR_SUCCESS)
        break;

      if (*buf2)
      {
        if ((i=xatoiA(buf)) > 0 && i < 65536 &&
            i != nAnsiCodePage &&
            i != nOemCodePage &&
            i != CP_UNICODE_UCS2_LE &&
            i != CP_UNICODE_UCS2_BE &&
            i != CP_UNICODE_UTF8 &&
            i != CP_UNICODE_UTF7)
        {
          lpCodepageTable[i]=TRUE;
          ++nCodepageTableCount;
        }
      }
    }
    RegCloseKey(hKey);
  }
}

BOOL CALLBACK EnumCodePagesProc(wchar_t *wpCodePage)
{
  int i;

  if ((i=xatoiW(wpCodePage)) > 0 && i < 65536 &&
      i != nAnsiCodePage &&
      i != nOemCodePage &&
      i != CP_UNICODE_UCS2_LE &&
      i != CP_UNICODE_UCS2_BE &&
      i != CP_UNICODE_UTF8 &&
      i != CP_UNICODE_UTF7)
  {
    lpCodepageTable[i]=TRUE;
    ++nCodepageTableCount;
  }
  return TRUE;
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

  //Offset BOM
  fsd.nBytesCurrent=0;

  if (*bBOM)
  {
    if (*nCodePage == CP_UNICODE_UCS2_LE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (*nCodePage == CP_UNICODE_UCS2_BE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (*nCodePage == CP_UNICODE_UTF8)
    {
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=3;
    }
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

  //Offset BOM
  fsd.nBytesCurrent=0;

  if (*bBOM)
  {
    if (*nCodePage == CP_UNICODE_UCS2_LE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (*nCodePage == CP_UNICODE_UCS2_BE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (*nCodePage == CP_UNICODE_UTF8)
    {
      SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=3;
    }
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
    lstrcpyA(szANSIwatermark, "\xE0\xE1\xE2\xE5\xE8\xED\xEE\xEF\xF0\xF2\xC0\xC1\xC2\xC5\xC8\xCD\xCE\xCF\xD2");  //‡·‚ÂËÌÓÔÚ¿¡¬≈»ÕŒœ“
    lstrcpyA(szOEMwatermark,  "\xAE\xA5\xA0\xA8\xAD\xE2\x8E\x45\x80\x88\x8D\x92\xB0\xB1\xB2\xB3\xBA\xDB");      //Character graphics simbols: \xB0\xB1\xB2\xB3\xBA\xDB
    lstrcpyA(szKOIwatermark,  "\xC1\xC2\xD7\xC5\xC9\xCE\xCF\xD2\xD4\xE1\xE2\xF7\xE5\xE9\xEE\xEF\xF0\xF2\xF4");  //¡¬◊≈…Œœ“‘·‚˜ÂÈÓÔÚÙ
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

      if (AKD_strchr(szANSIwatermark, dwMaxIndex + 0x80)) nANSIrate+=dwCounter[dwMaxIndex];
      if (AKD_strchr(szOEMwatermark, dwMaxIndex + 0x80)) nOEMrate+=dwCounter[dwMaxIndex];
      if (AKD_strchr(szKOIwatermark, dwMaxIndex + 0x80)) nKOIrate+=dwCounter[dwMaxIndex];
      if (AKD_strchr(szUTF8watermark, dwMaxIndex + 0x80)) nUTF8rate+=dwCounter[dwMaxIndex];
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

unsigned int TranslateNewLinesToUnixW(wchar_t *wszWideString, unsigned int nWideStringLen)
{
  unsigned int a;
  unsigned int b;

  for (a=0, b=0; a < nWideStringLen && wszWideString[a]; ++a, ++b)
  {
    if (wszWideString[a] == '\r' && wszWideString[a + 1] == '\n') ++a;
    wszWideString[b]=wszWideString[a];
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

    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax) || (ftflags & AEFR_SELECTION) || SendMessage(hWndEdit, AEM_GETCOLUMNSEL, 0, 0))
    {
      SendMessage(hWndFind, CB_SETCURSEL, 0, 0);
    }
    else
    {
      if (ExGetRangeTextA(hWndEdit, &crSel.ciMin, &crSel.ciMax, FALSE, &szData, AELB_R, FALSE))
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

      if ((nFindTextLen=GetComboboxSearchTextA(hWndFind, &szFindText_orig, &szFindText)) <= 0)
      {
        FreeMemorySearchA();
        return TRUE;
      }
      if (bReplaceDlg == TRUE)
      {
        if ((nReplaceTextLen=GetComboboxSearchTextA(hWndReplace, &szReplaceText_orig, &szReplaceText)) < 0)
        {
          FreeMemorySearchA();
          return TRUE;
        }
      }

      hWndFocus=GetFocus();
      if (bReplaceAll)
        bReplaceAllButtonState=EnableWindow(hWndReplaceAllButton, FALSE);

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
            ReplaceTextA(hWndEdit, ftflags, szFindText, nFindTextLen, szReplaceText, nReplaceTextLen, TRUE, &nReplaceCount);
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
              nResult=ReplaceTextA(hWndEdit, ftflags, szFindText, nFindTextLen, szReplaceText, nReplaceTextLen, FALSE, NULL);
            else
              nResult=FindTextA(hWndEdit, ftflags, szFindText, nFindTextLen);

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
          nResult=ReplaceTextA(hWndEdit, ftflags, szFindText, nFindTextLen, szReplaceText, nReplaceTextLen, bReplaceAll, &nReplaceCount);
        else
          nResult=FindTextA(hWndEdit, ftflags, szFindText, nFindTextLen);

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

        if ((nFindTextLen=GetComboboxSearchTextA(hWndFind, &szFindText_orig, &szFindText)) <= 0)
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

    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax) || (ftflags & AEFR_SELECTION) || SendMessage(hWndEdit, AEM_GETCOLUMNSEL, 0, 0))
    {
      SendMessage(hWndFind, CB_SETCURSEL, 0, 0);
    }
    else
    {
      if (ExGetRangeTextW(hWndEdit, &crSel.ciMin, &crSel.ciMax, FALSE, &wszData, AELB_R, FALSE))
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

      if ((nFindTextLen=GetComboboxSearchTextW(hWndFind, &wszFindText_orig, &wszFindText)) <= 0)
      {
        FreeMemorySearchW();
        return TRUE;
      }
      if (bReplaceDlg == TRUE)
      {
        if ((nReplaceTextLen=GetComboboxSearchTextW(hWndReplace, &wszReplaceText_orig, &wszReplaceText)) < 0)
        {
          FreeMemorySearchW();
          return TRUE;
        }
      }

      hWndFocus=GetFocus();
      if (bReplaceAll)
        bReplaceAllButtonState=EnableWindow(hWndReplaceAllButton, FALSE);

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
            ReplaceTextW(hWndEdit, ftflags, wszFindText, nFindTextLen, wszReplaceText, nReplaceTextLen, TRUE, &nReplaceCount);
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
              nResult=ReplaceTextW(hWndEdit, ftflags, wszFindText, nFindTextLen, wszReplaceText, nReplaceTextLen, FALSE, NULL);
            else
              nResult=FindTextW(hWndEdit, ftflags, wszFindText, nFindTextLen);

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
          nResult=ReplaceTextW(hWndEdit, ftflags, wszFindText, nFindTextLen, wszReplaceText, nReplaceTextLen, bReplaceAll, &nReplaceCount);
        else
          nResult=FindTextW(hWndEdit, ftflags, wszFindText, nFindTextLen);

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

        if ((nFindTextLen=GetComboboxSearchTextW(hWndFind, &wszFindText_orig, &wszFindText)) <= 0)
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

int GetComboboxSearchTextA(HWND hWnd, char **szText_orig, char **szText)
{
  int nTextLen_orig;
  int nTextLen=-1;
  int nIndex;
  int nItemLen;

  nTextLen_orig=GetWindowTextLength(hWnd) + 1;

  if (*szText_orig=*szText=(char *)API_HeapAlloc(hHeap, 0, nTextLen_orig + 1))
  {
    nTextLen=GetWindowTextA(hWnd, *szText_orig, nTextLen_orig);

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

    if (nTextLen)
    {
      if (ftflags & AEFR_ESCAPESEQ)
      {
        if (*szText=(char *)API_HeapAlloc(hHeap, 0, nTextLen_orig + 1))
        {
          if (!(nTextLen=EscapeStringToEscapeDataA(*szText_orig, *szText)))
          {
            API_LoadStringA(hLangLib, MSG_ERROR_SYNTAX, buf, BUFFER_SIZE);
            MessageBoxA(GetParent(hWnd), buf, APP_MAIN_TITLEA, MB_OK|MB_ICONERROR);
            nTextLen=-1;
          }
        }
        else nTextLen=-1;
      }
    }
  }
  return nTextLen;
}

int GetComboboxSearchTextW(HWND hWnd, wchar_t **wszText_orig, wchar_t **wszText)
{
  int nTextLen_orig;
  int nTextLen=-1;
  int nIndex;
  int nItemLen;

  nTextLen_orig=GetWindowTextLengthW(hWnd) + 1;

  if (*wszText_orig=*wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nTextLen_orig * sizeof(wchar_t) + 2))
  {
    nTextLen=GetWindowTextW(hWnd, *wszText_orig, nTextLen_orig);

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

    if (nTextLen)
    {
      if (ftflags & AEFR_ESCAPESEQ)
      {
        if (*wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nTextLen_orig * sizeof(wchar_t) + 2))
        {
          if (!(nTextLen=EscapeStringToEscapeDataW(*wszText_orig, *wszText)))
          {
            API_LoadStringW(hLangLib, MSG_ERROR_SYNTAX, wbuf, BUFFER_SIZE);
            MessageBoxW(GetParent(hWnd), wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
            nTextLen=-1;
          }
        }
        else nTextLen=-1;
      }
    }
  }
  return nTextLen;
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

int FindTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, int nFindItLen)
{
  AEFINDTEXTA ft;
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
  ft.dwTextLen=nFindItLen;
  ft.nNewLine=AELB_R;

  if (bResult=SendMessage(hWnd, AEM_FINDTEXTA, 0, (LPARAM)&ft))
  {
    DWORD dwScrollFlags=0;
    DWORD dwScrollResult;

    SetSel(hWnd, &ft.crFound, AESELT_LOCKSCROLL, NULL);

    dwScrollResult=SendMessage(hWnd, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(1, 1));
    if (dwScrollResult & AECSE_SCROLLEDX)
      dwScrollFlags|=AESC_UNITRECTDIVX;
    if (dwScrollResult & AECSE_SCROLLEDY)
      dwScrollFlags|=AESC_UNITRECTDIVY;
    SendMessage(hWnd, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
  }
  else SendMessage(hMainWnd, AKDN_SEARCH_ENDED, (WPARAM)hDlgModeless, 0);

  return bResult?0:-1;
}

int FindTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, int nFindItLen)
{
  AEFINDTEXTW ft;
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
  ft.pText=wpFindIt;
  ft.dwTextLen=nFindItLen;
  ft.nNewLine=AELB_R;

  if (bResult=SendMessage(hWnd, AEM_FINDTEXTW, 0, (LPARAM)&ft))
  {
    DWORD dwScrollFlags=0;
    DWORD dwScrollResult;

    SetSel(hWnd, &ft.crFound, AESELT_LOCKSCROLL, NULL);

    dwScrollResult=SendMessage(hWnd, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(1, 1));
    if (dwScrollResult & AECSE_SCROLLEDX)
      dwScrollFlags|=AESC_UNITRECTDIVX;
    if (dwScrollResult & AECSE_SCROLLEDY)
      dwScrollFlags|=AESC_UNITRECTDIVY;
    SendMessage(hWnd, AEM_SCROLLCARET, dwScrollFlags, MAKELONG(3, 2));
  }
  else SendMessage(hMainWnd, AKDN_SEARCH_ENDED, (WPARAM)hDlgModeless, 0);

  return bResult?0:-1;
}

int ReplaceTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, int nFindItLen, char *pReplaceWith, int nReplaceWithLen, BOOL bAll, int *nReplaceCount)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  AECHARINDEX ciFirstVisibleBefore;
  AECHARINDEX ciFirstVisibleAfter;
  CHARRANGE crInitialRE;
  char *szRangeText;
  char *szResultText;
  int nMin;
  int nMax;
  int nNewLine=AELB_ASIS;
  int nFirstVisible;
  int nRangeTextLen;
  int nResultTextLen;
  int nChanges=0;
  int nResult=-1;
  int i;
  BOOL bInitialColumnSel;
  BOOL bColumnSel;

  if (nFindItLen == -1)
    nFindItLen=lstrlenA(pFindIt);
  if (nReplaceWithLen == -1)
    nReplaceWithLen=lstrlenA(pReplaceWith);

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

    for (i=0; i < nFindItLen; ++i)
    {
      if (pFindIt[i] == '\r')
      {
        nNewLine=AELB_R;
        break;
      }
    }

    if (nRangeTextLen=ExGetRangeTextA(hWnd, &crRange.ciMin, &crRange.ciMax, bColumnSel, &szRangeText, nNewLine, TRUE))
    {
      if (StrReplaceA(szRangeText, nRangeTextLen, pFindIt, nFindItLen, pReplaceWith, nReplaceWithLen, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, NULL, &nResultTextLen, NULL, NULL, NULL))
      {
        if (szResultText=(char *)API_HeapAlloc(hHeap, 0, nResultTextLen + 1))
        {
          //Remember selection
          if (nNewLine == AELB_ASIS)
          {
            crInitialRE.cpMin=IndexSubtract(hWnd, NULL, &crSel.ciMin, nNewLine, FALSE);
            crInitialRE.cpMax=crInitialRE.cpMin + IndexSubtract(hWnd, &crSel.ciMin, &crSel.ciMax, nNewLine, FALSE);
          }
          else SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&crInitialRE);

          if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
          {
            nMin=crInitialRE.cpMin;
            nMax=crInitialRE.cpMax;
          }
          else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
          {
            nMin=0;
            nMax=crInitialRE.cpMax - crInitialRE.cpMin;
          }

          //Remember scroll
          SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstVisibleBefore);

          if (AEC_IndexCompare(&ciFirstVisibleBefore, &crRange.ciMin) >= 0)
            nFirstVisible=IndexSubtract(hWnd, &ciFirstVisibleBefore, &crRange.ciMin, nNewLine, FALSE);
          else
            nFirstVisible=-0x7FFFFFFF;

          //Replace operation
          if (nChanges=StrReplaceA(szRangeText, nRangeTextLen, pFindIt, nFindItLen, pReplaceWith, nReplaceWithLen, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, szResultText, NULL, &nMin, &nMax, (nFirstVisible == -0x7FFFFFFF)?NULL:&nFirstVisible))
          {
            FreeText(szRangeText);
            szRangeText=NULL;

            //Stop redraw
            SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

            if (!(dwFlags & AEFR_SELECTION))
              SetSel(hWnd, &crRange, 0, NULL);

            if (nNewLine == AELB_ASIS)
            {
              i=SendMessage(hWnd, AEM_GETNEWLINE, 0, 0);
              SendMessage(hWnd, AEM_SETNEWLINE, AENL_INPUT, AELB_ASIS);
            }
            ReplaceSelA(hWnd, szResultText, nResultTextLen, bColumnSel, NULL, NULL);
            if (nNewLine == AELB_ASIS)
              SendMessage(hWnd, AEM_SETNEWLINE, AENL_INPUT, i);

            //Restore selection
            if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
            {
              if (nNewLine == AELB_ASIS)
              {
                SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crInitialSel.ciMin);
                IndexOffset(hWnd, &crInitialSel.ciMin, nMin, nNewLine);
              }
              else if (nNewLine == AELB_R)
              {
                RichOffsetToAkelIndex(hWnd, nMin, &crInitialSel.ciMin);
              }
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, nMax - nMin, nNewLine);
            }
            else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
            {
              SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crInitialSel.ciMin);
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, nMax - nMin, nNewLine);
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

            if (nFirstVisible != -0x7FFFFFFF)
            {
              ciFirstVisibleBefore=crRange.ciMin;
              SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&ciFirstVisibleBefore);
              IndexOffset(hWnd, &ciFirstVisibleBefore, nFirstVisible, nNewLine);
              SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
            }
            else SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
          }
          API_HeapFree(hHeap, 0, (LPVOID)szResultText);
        }
      }
      if (szRangeText) FreeText(szRangeText);
    }
  }
  else
  {
    AEFINDTEXTA ft;

    ft.dwFlags=dwFlags;
    ft.pText=pFindIt;
    ft.dwTextLen=nFindItLen;
    ft.nNewLine=AELB_R;

    if (SendMessage(hWnd, AEM_ISMATCHA, (WPARAM)&crSel.ciMin, (LPARAM)&ft))
    {
      if (!AEC_IndexCompare(&crSel.ciMax, &ft.crFound.ciMax))
      {
        ReplaceSelA(hWnd, pReplaceWith, nReplaceWithLen, FALSE, NULL, NULL);
        nChanges=1;
      }
    }
    nResult=FindTextA(hWnd, dwFlags, pFindIt, nFindItLen);
  }
  if (nReplaceCount) *nReplaceCount=nChanges;
  return nResult;
}

int ReplaceTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, int nFindItLen, wchar_t *wpReplaceWith, int nReplaceWithLen, BOOL bAll, int *nReplaceCount)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  AECHARINDEX ciFirstVisibleBefore;
  AECHARINDEX ciFirstVisibleAfter;
  CHARRANGE crInitialRE;
  wchar_t *wszRangeText;
  wchar_t *wszResultText;
  int nMin;
  int nMax;
  int nNewLine=AELB_ASIS;
  int nFirstVisible;
  int nRangeTextLen;
  int nResultTextLen;
  int nChanges=0;
  int nResult=-1;
  int i;
  BOOL bInitialColumnSel;
  BOOL bColumnSel;

  if (nFindItLen == -1)
    nFindItLen=lstrlenW(wpFindIt);
  if (nReplaceWithLen == -1)
    nReplaceWithLen=lstrlenW(wpReplaceWith);

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

    for (i=0; i < nFindItLen; ++i)
    {
      if (wpFindIt[i] == '\r')
      {
        nNewLine=AELB_R;
        break;
      }
    }

    if (nRangeTextLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, bColumnSel, &wszRangeText, nNewLine, TRUE))
    {
      if (StrReplaceW(wszRangeText, nRangeTextLen, wpFindIt, nFindItLen, wpReplaceWith, nReplaceWithLen, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, NULL, &nResultTextLen, NULL, NULL, NULL))
      {
        if (wszResultText=(wchar_t *)API_HeapAlloc(hHeap, 0, nResultTextLen * sizeof(wchar_t) + 2))
        {
          //Remember selection
          if (nNewLine == AELB_ASIS)
          {
            crInitialRE.cpMin=IndexSubtract(hWnd, NULL, &crSel.ciMin, nNewLine, FALSE);
            crInitialRE.cpMax=crInitialRE.cpMin + IndexSubtract(hWnd, &crSel.ciMin, &crSel.ciMax, nNewLine, FALSE);
          }
          else SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&crInitialRE);

          if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
          {
            nMin=crInitialRE.cpMin;
            nMax=crInitialRE.cpMax;
          }
          else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
          {
            nMin=0;
            nMax=crInitialRE.cpMax - crInitialRE.cpMin;
          }

          //Remember scroll
          SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstVisibleBefore);

          if (AEC_IndexCompare(&ciFirstVisibleBefore, &crRange.ciMin) >= 0)
            nFirstVisible=IndexSubtract(hWnd, &ciFirstVisibleBefore, &crRange.ciMin, nNewLine, FALSE);
          else
            nFirstVisible=-0x7FFFFFFF;

          //Replace operation
          if (nChanges=StrReplaceW(wszRangeText, nRangeTextLen, wpFindIt, nFindItLen, wpReplaceWith, nReplaceWithLen, (dwFlags & AEFR_MATCHCASE)?TRUE:FALSE, wszResultText, NULL, &nMin, &nMax, (nFirstVisible == -0x7FFFFFFF)?NULL:&nFirstVisible))
          {
            FreeText(wszRangeText);
            wszRangeText=NULL;

            //Stop redraw
            SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

            if (!(dwFlags & AEFR_SELECTION))
              SetSel(hWnd, &crRange, 0, NULL);

            if (nNewLine == AELB_ASIS)
            {
              i=SendMessage(hWnd, AEM_GETNEWLINE, 0, 0);
              SendMessage(hWnd, AEM_SETNEWLINE, AENL_INPUT, AELB_ASIS);
            }
            ReplaceSelW(hWnd, wszResultText, nResultTextLen, bColumnSel, NULL, NULL);
            if (nNewLine == AELB_ASIS)
              SendMessage(hWnd, AEM_SETNEWLINE, AENL_INPUT, i);

            //Restore selection
            if ((dwFlags & AEFR_BEGINNING) || (dwFlags & AEFR_UP))
            {
              if (nNewLine == AELB_ASIS)
              {
                SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crInitialSel.ciMin);
                IndexOffset(hWnd, &crInitialSel.ciMin, nMin, nNewLine);
              }
              else if (nNewLine == AELB_R)
              {
                RichOffsetToAkelIndex(hWnd, nMin, &crInitialSel.ciMin);
              }
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, nMax - nMin, nNewLine);
            }
            else if ((dwFlags & AEFR_SELECTION) || (dwFlags & AEFR_DOWN))
            {
              SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crInitialSel.ciMin);
              crInitialSel.ciMax=crInitialSel.ciMin;
              IndexOffset(hWnd, &crInitialSel.ciMax, nMax - nMin, nNewLine);
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

            if (nFirstVisible != -0x7FFFFFFF)
            {
              ciFirstVisibleBefore=crRange.ciMin;
              SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&ciFirstVisibleBefore);
              IndexOffset(hWnd, &ciFirstVisibleBefore, nFirstVisible, nNewLine);
              SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
            }
            else SendMessage(hWnd, AEM_LINESCROLL, SB_VERT, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
          }
          API_HeapFree(hHeap, 0, (LPVOID)wszResultText);
        }
      }
      if (wszRangeText) FreeText(wszRangeText);
    }
  }
  else
  {
    AEFINDTEXTW ft;

    ft.dwFlags=dwFlags;
    ft.pText=wpFindIt;
    ft.dwTextLen=nFindItLen;
    ft.nNewLine=AELB_R;

    if (SendMessage(hWnd, AEM_ISMATCHW, (WPARAM)&crSel.ciMin, (LPARAM)&ft))
    {
      if (!AEC_IndexCompare(&crSel.ciMax, &ft.crFound.ciMax))
      {
        ReplaceSelW(hWnd, wpReplaceWith, nReplaceWithLen, FALSE, NULL, NULL);
        nChanges=1;
      }
    }
    nResult=FindTextW(hWnd, dwFlags, wpFindIt, nFindItLen);
  }
  if (nReplaceCount) *nReplaceCount=nChanges;
  return nResult;
}

int StrReplaceA(char *pText, int nTextLen, char *pIt, int nItLen, char *pWith, int nWithLen, BOOL bSensitive, char *szResult, int *nResultLen, int *nMin, int *nMax, int *nFirstVisible)
{
  int nMinOffset=0;
  int nMaxOffset=0;
  int nFirstVisibleOffset=0;
  int nTextCount;
  int nMatchCount;
  int nItCount;
  int nWithCount;
  int nResultCount=0;
  int nDiff;
  int nChanges=0;

  if (nItLen == -1)
    nItLen=lstrlenA(pIt);
  if (nWithLen == -1)
    nWithLen=lstrlenA(pWith);
  nDiff=nItLen - nWithLen;

  if (nMin) nMinOffset=*nMin;
  if (nMax) nMaxOffset=*nMax;
  if (nFirstVisible) nFirstVisibleOffset=*nFirstVisible;

  for (nTextCount=0; nTextCount < nTextLen; ++nTextCount)
  {
    nMatchCount=nTextCount;
    nItCount=0;

    while ((bSensitive == TRUE && pText[nMatchCount] == pIt[nItCount]) ||
           (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)pText[nMatchCount]) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)pIt[nItCount])))
    {
      if (++nItCount >= nItLen)
      {
        if (szResult)
        {
          if (nMin)
          {
            if (*nMin > nMatchCount) nMinOffset-=nDiff;
            else if (*nMin > nTextCount && *nMin <= nMatchCount) nMinOffset-=(*nMin - nTextCount);
          }
          if (nMax)
          {
            if (*nMax > nMatchCount) nMaxOffset-=nDiff;
            else if (*nMax > nTextCount && *nMax <= nMatchCount) nMaxOffset-=(*nMax - nTextCount) + nWithLen;
          }
          if (nFirstVisible)
          {
            if (*nFirstVisible > nMatchCount) nFirstVisibleOffset-=nDiff;
            else if (*nFirstVisible > nTextCount && *nFirstVisible <= nMatchCount) nFirstVisibleOffset-=(*nFirstVisible - nTextCount);
          }
          for (nWithCount=0; nWithCount < nWithLen; ++nWithCount)
            szResult[nResultCount++]=pWith[nWithCount];
        }
        else nResultCount+=nWithLen;

        nTextCount=nMatchCount + 1;
        nItCount=0;
        ++nChanges;
        if (nTextCount >= nTextLen) goto End;
      }
      if (++nMatchCount >= nTextLen) break;
    }
    if (szResult) szResult[nResultCount]=pText[nTextCount];
    ++nResultCount;
  }

  End:
  if (nResultLen) *nResultLen=nResultCount;
  if (nMax) *nMax=(*nMin == *nMax)?nMinOffset:nMaxOffset;
  if (nMin) *nMin=nMinOffset;
  if (nFirstVisible) *nFirstVisible=nFirstVisibleOffset;
  return nChanges;
}

int StrReplaceW(wchar_t *wpText, int nTextLen, wchar_t *wpIt, int nItLen, wchar_t *wpWith, int nWithLen, BOOL bSensitive, wchar_t *wszResult, int *nResultLen, int *nMin, int *nMax, int *nFirstVisible)
{
  int nMinOffset=0;
  int nMaxOffset=0;
  int nFirstVisibleOffset=0;
  int nTextCount;
  int nMatchCount;
  int nItCount;
  int nWithCount;
  int nResultCount=0;
  int nDiff;
  int nChanges=0;

  if (nItLen == -1)
    nItLen=lstrlenW(wpIt);
  if (nWithLen == -1)
    nWithLen=lstrlenW(wpWith);
  nDiff=nItLen - nWithLen;

  if (nMin) nMinOffset=*nMin;
  if (nMax) nMaxOffset=*nMax;
  if (nFirstVisible) nFirstVisibleOffset=*nFirstVisible;

  for (nTextCount=0; nTextCount < nTextLen; ++nTextCount)
  {
    nMatchCount=nTextCount;
    nItCount=0;

    while ((bSensitive == TRUE && wpText[nMatchCount] == wpIt[nItCount]) ||
           (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)wpText[nMatchCount]) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)wpIt[nItCount])))
    {
      if (++nItCount >= nItLen)
      {
        if (wszResult)
        {
          if (nMin)
          {
            if (*nMin > nMatchCount) nMinOffset-=nDiff;
            else if (*nMin > nTextCount && *nMin <= nMatchCount) nMinOffset-=(*nMin - nTextCount);
          }
          if (nMax)
          {
            if (*nMax > nMatchCount) nMaxOffset-=nDiff;
            else if (*nMax > nTextCount && *nMax <= nMatchCount) nMaxOffset-=(*nMax - nTextCount) + nWithLen;
          }
          if (nFirstVisible)
          {
            if (*nFirstVisible > nMatchCount) nFirstVisibleOffset-=nDiff;
            else if (*nFirstVisible > nTextCount && *nFirstVisible <= nMatchCount) nFirstVisibleOffset-=(*nFirstVisible - nTextCount);
          }
          for (nWithCount=0; nWithCount < nWithLen; ++nWithCount)
            wszResult[nResultCount++]=wpWith[nWithCount];
        }
        else nResultCount+=nWithLen;

        nTextCount=nMatchCount + 1;
        nItCount=0;
        ++nChanges;
        if (nTextCount >= nTextLen) goto End;
      }
      if (++nMatchCount >= nTextLen) break;
    }
    if (wszResult) wszResult[nResultCount]=wpText[nTextCount];
    ++nResultCount;
  }

  End:
  if (nResultLen) *nResultLen=nResultCount;
  if (nMax) *nMax=(*nMin == *nMax)?nMinOffset:nMaxOffset;
  if (nMin) *nMin=nMinOffset;
  if (nFirstVisible) *nFirstVisible=nFirstVisibleOffset;
  return nChanges;
}

int EscapeStringToEscapeDataA(char *pInput, char *szOutput)
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
          if (!*a) goto Error;
          hex[1]=*++a;
          if (!*a) goto Error;
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
  return (b - szOutput);

  Error:
  *szOutput='\0';
  return 0;
}

int EscapeStringToEscapeDataW(wchar_t *wpInput, wchar_t *wszOutput)
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
          if (!*a) goto Error;
          whex[1]=*++a;
          if (!*a) goto Error;
          whex[2]=*++a;
          if (!*a) goto Error;
          whex[3]=*++a;
          if (!*a) goto Error;
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
  return (b - wszOutput);

  Error:
  *wszOutput='\0';
  return 0;
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
  if (bColumnSel) *bColumnSel=(aes.dwFlags & AESELT_COLUMNON);
}

void SetSel(HWND hWnd, AECHARRANGE *crSel, DWORD dwFlags, AECHARINDEX *ciCaret)
{
  AESELECTION aes;

  aes.crSel=*crSel;
  aes.dwFlags=dwFlags;
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

  rs.pText=wpData;
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
    nLen=(nMax - nMin);

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

int ExGetRangeTextA(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, char **pText, int nNewLine, BOOL bFillSpaces)
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
  tr.bFillSpaces=bFillSpaces;

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

int ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, wchar_t **wpText, int nNewLine, BOOL bFillSpaces)
{
  AETEXTRANGEW tr;
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
  tr.bFillSpaces=bFillSpaces;

  if (nLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr))
  {
    if (tr.pBuffer=(wchar_t *)API_HeapAlloc(hHeap, 0, nLen * sizeof(wchar_t)))
    {
      nLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);
    }
  }
  *wpText=tr.pBuffer;
  return nLen;
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

BOOL ColumnPaste(HWND hWnd)
{
  HGLOBAL hData;
  LPVOID pData;
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARINDEX ciInitialCaret=ciCaret;
  int nLineRange=(crSel.ciMax.nLine - crSel.ciMin.nLine) + 1;
  int nSourceLen;
  int nTargetLen;
  int nTargetCount;
  int i;
  BOOL bResult=FALSE;

  if (SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0) && nLineRange > 0)
  {
     if (OpenClipboard(hWnd))
     {
       if (hData=GetClipboardData(CF_UNICODETEXT))
       {
         if (pData=GlobalLock(hData))
         {
           wchar_t *wpSource=(wchar_t *)pData;
           wchar_t *wpTarget;

           nSourceLen=wcslen(wpSource);
           nTargetLen=(nSourceLen + 1) * nLineRange - 1;

           if (wpTarget=(wchar_t *)API_HeapAlloc(hHeap, 0, nTargetLen * sizeof(wchar_t) + 2))
           {
             for (i=0; i < nLineRange; ++i)
             {
               nTargetCount=i * (nSourceLen + 1);
               memcpy(wpTarget + nTargetCount, wpSource, nSourceLen * sizeof(wchar_t));
               wpTarget[nTargetCount + nSourceLen]='\r';
             }
             wpTarget[nTargetLen]='\0';

             ReplaceSelW(hWnd, wpTarget, nTargetLen, TRUE, &crRange.ciMin, &crRange.ciMax);
             if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
               SetSel(hWnd, &crRange, AESELT_COLUMNON, &crRange.ciMin);
             else
               SetSel(hWnd, &crRange, AESELT_COLUMNON, &crRange.ciMax);
             bResult=FALSE;
             API_HeapFree(hHeap, 0, (LPVOID)wpTarget);
           }
           GlobalUnlock(hData);
         }
       }
       else if (hData=GetClipboardData(CF_TEXT))
       {
         if (pData=GlobalLock(hData))
         {
           char *pSource=(char *)pData;
           char *pTarget;

           nSourceLen=lstrlenA(pSource);
           nTargetLen=(nSourceLen + 1) * nLineRange - 1;

           if (pTarget=(char *)API_HeapAlloc(hHeap, 0, nTargetLen + 1))
           {
             for (i=0; i < nLineRange; ++i)
             {
               nTargetCount=i * (nSourceLen + 1);
               memcpy(pTarget + nTargetCount, pSource, nSourceLen);
               pTarget[nTargetCount + nSourceLen]='\r';
             }
             pTarget[nTargetLen]='\0';

             ReplaceSelA(hWnd, pTarget, nTargetLen, TRUE, NULL, NULL);
             if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
               SetSel(hWnd, &crRange, AESELT_COLUMNON, &crRange.ciMin);
             else
               SetSel(hWnd, &crRange, AESELT_COLUMNON, &crRange.ciMax);
             bResult=FALSE;
             API_HeapFree(hHeap, 0, (LPVOID)pTarget);
           }
           GlobalUnlock(hData);
         }
       }
      CloseClipboard();
    }
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
  AECHARRANGE cr;
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
          if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
            a=SendMessage(hWndEdit, AEM_GETUNWRAPLINE, ciCaret.nLine, 0);
          else
            a=ciCaret.nLine;
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, a + 1, FALSE);
          SendMessage(hWndNumber, EM_SETSEL, 0, -1);
        }
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          a=IndexSubtract(hWndEdit, NULL, &ciCaret, AELB_ASIS, FALSE);
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, a, FALSE);
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
        nLineCount=SendMessage(hWndEdit, AEM_GETLINECOUNT, 0, 0);
        if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
          nLineCount=SendMessage(hWndEdit, AEM_GETUNWRAPLINE, nLineCount - 1, 0) + 1;

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

        if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
          SendMessage(hWndEdit, AEM_GETWRAPLINE, nNumber - 1, (LPARAM)&cr.ciMin);
        else
          SendMessage(hWndEdit, AEM_GETLINEINDEX, nNumber - 1, (LPARAM)&cr.ciMin);
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (nNumber >= 0)
          SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&cr.ciMin);
        else
          SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&cr.ciMin);
        IndexOffset(hWndEdit, &cr.ciMin, nNumber, AELB_ASIS);
      }

      //Set selection
      {
        DWORD dwScrollFlags=0;
        DWORD dwScrollResult;

        cr.ciMax=cr.ciMin;
        SetSel(hWndEdit, &cr, AESELT_LOCKSCROLL, NULL);

        dwScrollResult=SendMessage(hWndEdit, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(1, 1));
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
  AECHARRANGE cr;
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
          if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
            a=SendMessage(hWndEdit, AEM_GETUNWRAPLINE, ciCaret.nLine, 0);
          else
            a=ciCaret.nLine;
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, a + 1, FALSE);
          SendMessage(hWndNumber, EM_SETSEL, 0, -1);
        }
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          a=IndexSubtract(hWndEdit, NULL, &ciCaret, AELB_ASIS, FALSE);
          SetDlgItemInt(hDlg, IDC_GOTOLINE_NUMBER, a, FALSE);
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
        nLineCount=SendMessage(hWndEdit, AEM_GETLINECOUNT, 0, 0);
        if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
          nLineCount=SendMessage(hWndEdit, AEM_GETUNWRAPLINE, nLineCount - 1, 0) + 1;

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

        if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
          SendMessage(hWndEdit, AEM_GETWRAPLINE, nNumber - 1, (LPARAM)&cr.ciMin);
        else
          SendMessage(hWndEdit, AEM_GETLINEINDEX, nNumber - 1, (LPARAM)&cr.ciMin);
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (nNumber >= 0)
          SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&cr.ciMin);
        else
          SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&cr.ciMin);
        IndexOffset(hWndEdit, &cr.ciMin, nNumber, AELB_ASIS);
      }

      //Set selection
      {
        DWORD dwScrollFlags=0;
        DWORD dwScrollResult;

        cr.ciMax=cr.ciMin;
        SetSel(hWndEdit, &cr, AESELT_LOCKSCROLL, NULL);

        dwScrollResult=SendMessage(hWndEdit, AEM_SCROLLCARETTEST, AESC_UNITCHARX|AESC_UNITCHARY, MAKELONG(1, 1));
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
    SetSel(hWnd, &crRange, 0, &crRange.ciMax);
    bSelection=FALSE;
  }
  else
  {
    crRange=crSel;
    bSelection=TRUE;
  }

  if (nUnicodeLen=ExGetRangeTextW(hWnd, &crRange.ciMin, &crRange.ciMax, -1, &wszSelText, AELB_ASIS, TRUE))
  {
    nAnsiLen=WideCharToMultiByte(nCodePageFrom, 0, wszSelText, nUnicodeLen + 1, NULL, 0, NULL, NULL);

    if (szText=(char *)API_HeapAlloc(hHeap, 0, nAnsiLen))
    {
      WideCharToMultiByte(nCodePageFrom, 0, wszSelText, nUnicodeLen + 1, szText, nAnsiLen, NULL, NULL);
      nUnicodeLen=MultiByteToWideChar(nCodePageTo, 0, szText, nAnsiLen, NULL, 0);

      if (wszText=(wchar_t *)API_HeapAlloc(hHeap, 0, nUnicodeLen * sizeof(wchar_t)))
      {
        MultiByteToWideChar(nCodePageTo, 0, szText, nAnsiLen, wszText, nUnicodeLen);
        API_HeapFree(hHeap, 0, (LPVOID)szText);
        szText=NULL;

        ReplaceSelW(hWnd, wszText, nUnicodeLen - 1, -1, &crRange.ciMin, &crRange.ciMax);

        //Update selection
        if (!bSelection)
        {
          SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crInitialSel.ciMin);
          crRange.ciMin=crInitialSel.ciMin;
          crRange.ciMax=crInitialSel.ciMin;
        }

        if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
          SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMin);
        else
          SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMax);

        API_HeapFree(hHeap, 0, (LPVOID)wszText);
      }
      if (szText) API_HeapFree(hHeap, 0, (LPVOID)szText);
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
  static HWND hWndOK;
  static HWND hWndCancel;
  static AECOLORS aecColorsDlg;
  static DIALOGRESIZE drs[]={{&hWndThemeName,   DRS_SIZE|DRS_X},
                             {&hWndThemeSave,   DRS_MOVE|DRS_X},
                             {&hWndThemeDelete, DRS_MOVE|DRS_X},
                             {&hWndList,        DRS_SIZE|DRS_X|DRS_Y},
                             {&hWndOK,          DRS_MOVE|DRS_X|DRS_Y},
                             {&hWndCancel,      DRS_MOVE|DRS_X|DRS_Y},
                             {0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndThemeName=GetDlgItem(hDlg, IDC_COLORS_THEME_NAME);
    hWndThemeSave=GetDlgItem(hDlg, IDC_COLORS_THEME_SAVE);
    hWndThemeDelete=GetDlgItem(hDlg, IDC_COLORS_THEME_DELETE);
    hWndList=GetDlgItem(hDlg, IDC_COLORS_LIST);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
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

      API_LoadStringA(hLangLib, STR_ACTIVECOLUMN, buf, BUFFER_SIZE);
      lviA.mask=LVIF_TEXT;
      lviA.pszText=buf;
      lviA.iItem=LVI_COLOR_ACTIVECOLUMN;
      lviA.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      API_LoadStringA(hLangLib, STR_COLUMNMARKER, buf, BUFFER_SIZE);
      lviA.mask=LVIF_TEXT;
      lviA.pszText=buf;
      lviA.iItem=LVI_COLOR_COLUMNMARKER;
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

      for (i=LVI_COLOR_BASIC; i < LVI_COLOR_ACTIVECOLUMN; ++i)
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
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_ACTIVECOLUMN)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveColumn;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveColumn;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveColumn;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_COLUMNMARKER)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crColumnMarker;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crColumnMarker;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crColumnMarker;
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
            else if (lvhti.iItem == LVI_COLOR_ACTIVECOLUMN)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crActiveColumn);
            else if (lvhti.iItem == LVI_COLOR_COLUMNMARKER)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crColumnMarker);
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
            else if (lvhti.iItem == LVI_COLOR_ACTIVECOLUMN)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crActiveColumn);
            else if (lvhti.iItem == LVI_COLOR_COLUMNMARKER)
              bResult=SelectColorDialogA(hDlg, &aecColorsDlg.crColumnMarker);
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
  DialogResizeMessages(&drs[0], &rcColorsDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

BOOL CALLBACK ColorsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndThemeName;
  static HWND hWndThemeSave;
  static HWND hWndThemeDelete;
  static HWND hWndList;
  static HWND hWndOK;
  static HWND hWndCancel;
  static AECOLORS aecColorsDlg;
  static DIALOGRESIZE drs[]={{&hWndThemeName,   DRS_SIZE|DRS_X},
                             {&hWndThemeSave,   DRS_MOVE|DRS_X},
                             {&hWndThemeDelete, DRS_MOVE|DRS_X},
                             {&hWndList,        DRS_SIZE|DRS_X|DRS_Y},
                             {&hWndOK,          DRS_MOVE|DRS_X|DRS_Y},
                             {&hWndCancel,      DRS_MOVE|DRS_X|DRS_Y},
                             {0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndThemeName=GetDlgItem(hDlg, IDC_COLORS_THEME_NAME);
    hWndThemeSave=GetDlgItem(hDlg, IDC_COLORS_THEME_SAVE);
    hWndThemeDelete=GetDlgItem(hDlg, IDC_COLORS_THEME_DELETE);
    hWndList=GetDlgItem(hDlg, IDC_COLORS_LIST);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
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

      API_LoadStringW(hLangLib, STR_ACTIVECOLUMN, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_ACTIVECOLUMN;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      SendMessage(hWndList, LVM_INSERTITEMW, 0, (LPARAM)&lviW);

      API_LoadStringW(hLangLib, STR_COLUMNMARKER, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_COLUMNMARKER;
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

      for (i=LVI_COLOR_BASIC; i < LVI_COLOR_ACTIVECOLUMN; ++i)
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
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_ACTIVECOLUMN)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveColumn;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveColumn;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crActiveColumn;
            }
          }
          else if (lplvcd->nmcd.dwItemSpec == LVI_COLOR_COLUMNMARKER)
          {
            if (lplvcd->iSubItem == LVSI_COLOR_TEXT)
            {
              lplvcd->clrTextBk=aecColorsDlg.crColumnMarker;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_BACKGROUND)
            {
              lplvcd->clrTextBk=aecColorsDlg.crColumnMarker;
            }
            else if (lplvcd->iSubItem == LVSI_COLOR_SAMPLE)
            {
              lplvcd->clrTextBk=aecColorsDlg.crColumnMarker;
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
            else if (lvhti.iItem == LVI_COLOR_ACTIVECOLUMN)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crActiveColumn);
            else if (lvhti.iItem == LVI_COLOR_COLUMNMARKER)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crColumnMarker);
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
            else if (lvhti.iItem == LVI_COLOR_ACTIVECOLUMN)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crActiveColumn);
            else if (lvhti.iItem == LVI_COLOR_COLUMNMARKER)
              bResult=SelectColorDialogW(hDlg, &aecColorsDlg.crColumnMarker);
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
  DialogResizeMessages(&drs[0], &rcColorsDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

void FillComboboxThemesA(HWND hWnd)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hThemesStack.first;

  while (ctElement)
  {
    SendMessageA(hWnd, CB_ADDSTRING, 0, (LPARAM)ctElement->szName);

    ctElement=ctElement->next;
  }
}

void FillComboboxThemesW(HWND hWnd)
{
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hThemesStack.first;

  while (ctElement)
  {
    SendMessageW(hWnd, CB_ADDSTRING, 0, (LPARAM)ctElement->wszName);

    ctElement=ctElement->next;
  }
}


//// Plugins

BOOL CALLBACK PluginsDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndListInfo;
  static HWND hWndHotkeyLabel;
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
  else if (uMsg == WM_DESTROY)
  {
    if (hHookPlugins)
    {
      if (UnhookWindowsHookEx(hHookPlugins))
        hHookPlugins=NULL;
    }
    hWndHotkey=NULL;
  }
  DialogResizeMessages(&drs[0], &rcPluginsDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

BOOL CALLBACK PluginsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndListInfo;
  static HWND hWndHotkeyLabel;
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
                    CallPluginW(NULL, pliElement->pf->wszFunction, FALSE, 0, &pliElement->nAutoLoad);
                  }
                  if (pliElement->nAutoLoad == 0)
                  {
                    pliElement->pf->bOnStart=FALSE;
                    API_LoadStringW(hLangLib, MSG_AUTOLOAD_IS_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
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
  DialogResizeMessages(&drs[0], &rcPluginsDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    MSG *msg=(MSG *)lParam;

    if (msg->message >= WM_KEYFIRST && msg->message <= WM_KEYLAST)
    {
      if (hWndHotkey && hWndHotkey == msg->hwnd)
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
    if (hWndPreviewDlg)
    {
      if (msg->message == WM_KEYDOWN)
      {
        if (SendMessage(hWndPreviewDlg, AKDLG_PREVIEWKEYDOWN, msg->wParam, (LPARAM)msg->hwnd))
          msg->message=WM_NULL;
      }
      else if (msg->message == WM_MOUSEWHEEL)
      {
        if (SendMessage(hWndPreviewDlg, AKDLG_PREVIEWMOUSEWHEEL, msg->wParam, (LPARAM)msg->hwnd))
          msg->message=WM_NULL;
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

  if ((pFunction=AKD_strchr(pFullName, ':')) && *(pFunction + 1) == ':')
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

  if ((wpFunction=AKD_wcschr(wpFullName, ':')) && *(wpFunction + 1) == ':')
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
  BOOL bActive=TRUE;
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
          bActive=FALSE;
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
          (*PluginIDPtr)(&pv);

          if (LOBYTE(pv.dwAkelDllVersion) == LOBYTE(AKELDLL) &&
              HIBYTE(pv.dwAkelDllVersion) == HIBYTE(AKELDLL))
          {
            if (pv.dwExeMinVersion4x != MAKE_IDENTIFIER(-1, -1, -1, -1))
            {
              if (VersionCompare(pv.dwExeMinVersion4x, dwExeVersion) <= 0)
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

                  if (lpbAutoLoad && bActive) return EDL_NONUNLOADED;
                  if (pd.nUnload == UD_NONUNLOAD) return EDL_NONUNLOADED;
                  if (pd.nUnload == UD_NONUNLOAD_ACTIVE) return EDL_NONUNLOADED_ACTIVE;
                  if (pd.nUnload == UD_NONUNLOAD_NONACTIVE) return EDL_NONUNLOADED_NONACTIVE;
                  bCalled=TRUE;
                }
                else
                {
                  API_LoadStringA(hLangLib, MSG_FUNCTION_NOT_FOUND, buf, BUFFER_SIZE);
                  wsprintfA(buf2, buf, szPlugin, szFunction, szDLL);
                  MessageBoxA(hMainWnd, buf2, APP_MAIN_TITLEA, MB_OK|MB_ICONEXCLAMATION);
                  if (bActive) return EDL_FAILED;
                }
              }
              else
              {
                API_LoadStringA(hLangLib, MSG_UPDATE_PROGRAM, buf, BUFFER_SIZE);
                wsprintfA(buf2, buf, LOBYTE(pv.dwExeMinVersion4x), HIBYTE(pv.dwExeMinVersion4x), LOBYTE(HIWORD(pv.dwExeMinVersion4x)), HIBYTE(HIWORD(pv.dwExeMinVersion4x)),
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
          if (bCalled) return EDL_UNLOADED;
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
  return EDL_FAILED;
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
  BOOL bActive=TRUE;
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
          bActive=FALSE;
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
          (*PluginIDPtr)(&pv);

          if (LOBYTE(pv.dwAkelDllVersion) == LOBYTE(AKELDLL) &&
              HIBYTE(pv.dwAkelDllVersion) == HIBYTE(AKELDLL))
          {
            if (pv.dwExeMinVersion4x != MAKE_IDENTIFIER(-1, -1, -1, -1))
            {
              if (VersionCompare(pv.dwExeMinVersion4x, dwExeVersion) <= 0)
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

                  if (lpbAutoLoad && bActive) return EDL_NONUNLOADED;
                  if (pd.nUnload == UD_NONUNLOAD) return EDL_NONUNLOADED;
                  if (pd.nUnload == UD_NONUNLOAD_ACTIVE) return EDL_NONUNLOADED_ACTIVE;
                  if (pd.nUnload == UD_NONUNLOAD_NONACTIVE) return EDL_NONUNLOADED_NONACTIVE;
                  bCalled=TRUE;
                }
                else
                {
                  API_LoadStringW(hLangLib, MSG_FUNCTION_NOT_FOUND, wbuf, BUFFER_SIZE);
                  wsprintfW(wbuf2, wbuf, wszPlugin, wszFunction, wszDLL);
                  MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
                  if (bActive) return EDL_FAILED;
                }
              }
              else
              {
                API_LoadStringW(hLangLib, MSG_UPDATE_PROGRAM, wbuf, BUFFER_SIZE);
                wsprintfW(wbuf2, wbuf, LOBYTE(pv.dwExeMinVersion4x), HIBYTE(pv.dwExeMinVersion4x), LOBYTE(HIWORD(pv.dwExeMinVersion4x)), HIBYTE(HIWORD(pv.dwExeMinVersion4x)),
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
          if (bCalled) return EDL_UNLOADED;
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
  return EDL_FAILED;
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

      if (CallPluginReceiveSendA(&pcs) == EDL_FAILED)
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
      pcs.wpFunction=pfElement->wszFunction;
      pcs.bOnStart=TRUE;
      pcs.lParam=0;
      pcs.lpbAutoLoad=NULL;

      if (CallPluginReceiveSendW(&pcs) == EDL_FAILED)
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

COLORTHEMEA* StackThemeAddA(HSTACK *hStack, char *pName, AECOLORS *aec)
{
  COLORTHEMEA *ctElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&ctElement, -1, sizeof(COLORTHEMEA)))
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

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&ctElement, -1, sizeof(COLORTHEMEW)))
  {
    lstrcpynW(ctElement->wszName, wpName, MAX_PATH);
    ctElement->nNameLen=lstrlenW(ctElement->wszName);
    if (aec) ctElement->aec=*aec;
  }
  return ctElement;
}

COLORTHEMEA* StackThemeGetByNameA(HSTACK *hStack, char *pName)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hStack->first;
  int nNameLen=lstrlenA(pName);

  while (ctElement)
  {
    if (ctElement->nNameLen == nNameLen)
      if (!lstrcmpiA(ctElement->szName, pName))
        return ctElement;

    ctElement=ctElement->next;
  }
  return NULL;
}

COLORTHEMEW* StackThemeGetByNameW(HSTACK *hStack, wchar_t *wpName)
{
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hStack->first;
  int nNameLen=lstrlenW(wpName);

  while (ctElement)
  {
    if (ctElement->nNameLen == nNameLen)
      if (!lstrcmpiW(ctElement->wszName, wpName))
        return ctElement;

    ctElement=ctElement->next;
  }
  return NULL;
}

COLORTHEMEA* StackThemeGetByColorsA(HSTACK *hStack, AECOLORS *aec)
{
  COLORTHEMEA *ctElement=(COLORTHEMEA *)hStack->first;

  while (ctElement)
  {
    if (!memcmp(&ctElement->aec.crCaret, &aec->crCaret, sizeof(AECOLORS) - sizeof(DWORD)))
      return ctElement;

    ctElement=ctElement->next;
  }
  return NULL;
}

COLORTHEMEW* StackThemeGetByColorsW(HSTACK *hStack, AECOLORS *aec)
{
  COLORTHEMEW *ctElement=(COLORTHEMEW *)hStack->first;

  while (ctElement)
  {
    if (!memcmp(&ctElement->aec.crCaret, &aec->crCaret, sizeof(AECOLORS) - sizeof(DWORD)))
      return ctElement;

    ctElement=ctElement->next;
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
      if (!lstrcmpiW(pfElement->wszFunction, wpString))
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
              if (!xstrcmpnA(szPluginName, pfElement->szFunction, (DWORD)-1, FALSE))
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
              if (!xstrcmpnW(wszPluginName, pfElement->wszFunction, (DWORD)-1, FALSE))
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
  static HWND hWndCommand;
  static HWND hWndDirectory;
  static HWND hWndAutodetectCP;
  static HWND hWndAutodetectCPBuffer;
  static HWND hWndDefaultCP;
  static HWND hWndDefaultNewLineWin;
  static HWND hWndDefaultNewLineUnix;
  static HWND hWndDefaultNewLineMac;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    hWndCommand=GetDlgItem(hDlg, IDC_OPTIONS_EXECCOM);
    hWndDirectory=GetDlgItem(hDlg, IDC_OPTIONS_EXECDIR);
    hWndAutodetectCP=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION);
    hWndAutodetectCPBuffer=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER);
    hWndDefaultCP=GetDlgItem(hDlg, IDC_OPTIONS_DEFAULT_CODEPAGE);
    hWndDefaultNewLineWin=GetDlgItem(hDlg, IDC_OPTIONS_NEWLINE_WIN);
    hWndDefaultNewLineUnix=GetDlgItem(hDlg, IDC_OPTIONS_NEWLINE_UNIX);
    hWndDefaultNewLineMac=GetDlgItem(hDlg, IDC_OPTIONS_NEWLINE_MAC);

    SendMessage(hWndCommand, EM_LIMITTEXT, (WPARAM)BUFFER_SIZE, 0);
    SendMessage(hWndDirectory, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);

    SetWindowTextA(hWndCommand, szCommand);
    SetWindowTextA(hWndDirectory, szWorkDir);

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
    SetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, dwCodepageRecognitionBuffer, FALSE);

    FillComboboxCodepageA(hWndDefaultCP, lpCodepageList);
    SelectComboboxCodepageA(hWndDefaultCP, nDefaultCodePage);

    if (nDefaultNewLine == NEWLINE_WIN)
      SendMessage(hWndDefaultNewLineWin, BM_SETCHECK, BST_CHECKED, 0);
    else if (nDefaultNewLine == NEWLINE_UNIX)
      SendMessage(hWndDefaultNewLineUnix, BM_SETCHECK, BST_CHECKED, 0);
    else if (nDefaultNewLine == NEWLINE_MAC)
      SendMessage(hWndDefaultNewLineMac, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_EXECCOM_BROWSE)
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
      //Execute
      GetDlgItemTextA(hDlg, IDC_OPTIONS_EXECCOM, szCommand, BUFFER_SIZE);
      GetDlgItemTextA(hDlg, IDC_OPTIONS_EXECDIR, szWorkDir, MAX_PATH);

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

      //Default codepage
      nDefaultCodePage=GetComboboxCodepageA(hWndDefaultCP);

      //Default newline
      if (SendMessage(hWndDefaultNewLineWin, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDefaultNewLine=NEWLINE_WIN;
      else if (SendMessage(hWndDefaultNewLineUnix, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDefaultNewLine=NEWLINE_UNIX;
      else if (SendMessage(hWndDefaultNewLineMac, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDefaultNewLine=NEWLINE_MAC;
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsGeneralDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCommand;
  static HWND hWndDirectory;
  static HWND hWndAutodetectCP;
  static HWND hWndAutodetectCPBuffer;
  static HWND hWndDefaultCP;
  static HWND hWndDefaultNewLineWin;
  static HWND hWndDefaultNewLineUnix;
  static HWND hWndDefaultNewLineMac;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    hWndCommand=GetDlgItem(hDlg, IDC_OPTIONS_EXECCOM);
    hWndDirectory=GetDlgItem(hDlg, IDC_OPTIONS_EXECDIR);
    hWndAutodetectCP=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION);
    hWndAutodetectCPBuffer=GetDlgItem(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER);
    hWndDefaultCP=GetDlgItem(hDlg, IDC_OPTIONS_DEFAULT_CODEPAGE);
    hWndDefaultNewLineWin=GetDlgItem(hDlg, IDC_OPTIONS_NEWLINE_WIN);
    hWndDefaultNewLineUnix=GetDlgItem(hDlg, IDC_OPTIONS_NEWLINE_UNIX);
    hWndDefaultNewLineMac=GetDlgItem(hDlg, IDC_OPTIONS_NEWLINE_MAC);

    SendMessage(hWndCommand, EM_LIMITTEXT, (WPARAM)BUFFER_SIZE, 0);
    SendMessage(hWndDirectory, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);

    SetWindowTextW(hWndCommand, wszCommand);
    SetWindowTextW(hWndDirectory, wszWorkDir);

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
    SetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, dwCodepageRecognitionBuffer, FALSE);

    FillComboboxCodepageW(hWndDefaultCP, lpCodepageList);
    SelectComboboxCodepageW(hWndDefaultCP, nDefaultCodePage);

    if (nDefaultNewLine == NEWLINE_WIN)
      SendMessage(hWndDefaultNewLineWin, BM_SETCHECK, BST_CHECKED, 0);
    else if (nDefaultNewLine == NEWLINE_UNIX)
      SendMessage(hWndDefaultNewLineUnix, BM_SETCHECK, BST_CHECKED, 0);
    else if (nDefaultNewLine == NEWLINE_MAC)
      SendMessage(hWndDefaultNewLineMac, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_EXECCOM_BROWSE)
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
      //Execute
      GetDlgItemTextW(hDlg, IDC_OPTIONS_EXECCOM, wszCommand, BUFFER_SIZE);
      GetDlgItemTextW(hDlg, IDC_OPTIONS_EXECDIR, wszWorkDir, MAX_PATH);

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

      //Default codepage
      nDefaultCodePage=GetComboboxCodepageW(hWndDefaultCP);

      //Default newline
      if (SendMessage(hWndDefaultNewLineWin, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDefaultNewLine=NEWLINE_WIN;
      else if (SendMessage(hWndDefaultNewLineUnix, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDefaultNewLine=NEWLINE_UNIX;
      else if (SendMessage(hWndDefaultNewLineMac, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDefaultNewLine=NEWLINE_MAC;
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

BOOL CALLBACK OptionsRegistryDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndSaveRegistry;
  static HWND hWndSaveINI;
  static HWND hWndSavePositions;
  static HWND hWndSaveCodepages;
  static HWND hWndRecentFiles;
  static HWND hWndRecentFilesSpin;
  static HWND hWndRecentFilesClear;
  static HWND hWndSearchStrings;
  static HWND hWndSearchStringsSpin;
  static HWND hWndSearchStringsClear;
  static HWND hWndFileTypesOpen;
  static HWND hWndAssociateOpen;
  static HWND hWndFileTypesEdit;
  static HWND hWndAssociateEdit;
  static HWND hWndFileTypesPrint;
  static HWND hWndAssociatePrint;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndSaveRegistry=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_REGISTRY);
    hWndSaveINI=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_INI);
    hWndSavePositions=GetDlgItem(hDlg, IDC_OPTIONS_SAVEPOSITIONS);
    hWndSaveCodepages=GetDlgItem(hDlg, IDC_OPTIONS_SAVECODEPAGES);
    hWndRecentFiles=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT);
    hWndRecentFilesSpin=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT_SPIN);
    hWndRecentFilesClear=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_CLEAR);
    hWndSearchStrings=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT);
    hWndSearchStringsSpin=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT_SPIN);
    hWndSearchStringsClear=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_CLEAR);
    hWndFileTypesOpen=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_OPEN);
    hWndAssociateOpen=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_OPEN);
    hWndFileTypesEdit=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_EDIT);
    hWndAssociateEdit=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_EDIT);
    hWndFileTypesPrint=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_PRINT);
    hWndAssociatePrint=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_PRINT);

    SendMessage(hWndRecentFilesSpin, UDM_SETBUDDY, (WPARAM)hWndRecentFiles, 0);
    SendMessage(hWndRecentFilesSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndSearchStringsSpin, UDM_SETBUDDY, (WPARAM)hWndSearchStrings, 0);
    SendMessage(hWndSearchStringsSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));

    if (dwFileTypesAssociated & AE_OPEN) SendMessage(hWndAssociateOpen, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_EDIT) SendMessage(hWndAssociateEdit, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_PRINT) SendMessage(hWndAssociatePrint, BM_SETCHECK, BST_CHECKED, 0);
    if (!(dwFileTypesAssociated & AE_OPEN)) EnableWindow(hWndFileTypesOpen, FALSE);
    if (!(dwFileTypesAssociated & AE_EDIT)) EnableWindow(hWndFileTypesEdit, FALSE);
    if (!(dwFileTypesAssociated & AE_PRINT)) EnableWindow(hWndFileTypesPrint, FALSE);
    SendMessage(hWndFileTypesOpen, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesEdit, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesPrint, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);

    SetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, nRecentFiles, FALSE);
    if (!nRecentFiles || !*lpszRecentNames[0]) EnableWindow(hWndRecentFilesClear, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, nSearchStrings, FALSE);
    if (!nSearchStrings) EnableWindow(hWndSearchStringsClear, FALSE);

    SetWindowTextA(hWndFileTypesOpen, szFileTypesOpen);
    SetWindowTextA(hWndFileTypesEdit, szFileTypesEdit);
    SetWindowTextA(hWndFileTypesPrint, szFileTypesPrint);

    if (nSaveSettings == SS_REGISTRY)
      SendMessage(hWndSaveRegistry, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndSaveINI, BM_SETCHECK, BST_CHECKED, 0);
    if (bSavePositions)
      SendMessage(hWndSavePositions, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveCodepages)
      SendMessage(hWndSaveCodepages, BM_SETCHECK, BST_CHECKED, 0);
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
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_OPEN)
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
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      char szWindowText[MAX_PATH];
      BOOL bShellRefresh=FALSE;
      int a;
      int b;

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

      //Recent files
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, NULL, FALSE);
      if (nRecentFiles != a)
      {
        nRecentFiles=a;
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

      //Search history
      nSearchStrings=GetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, NULL, FALSE);

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
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsRegistryDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndSaveRegistry;
  static HWND hWndSaveINI;
  static HWND hWndSavePositions;
  static HWND hWndSaveCodepages;
  static HWND hWndRecentFiles;
  static HWND hWndRecentFilesSpin;
  static HWND hWndRecentFilesClear;
  static HWND hWndSearchStrings;
  static HWND hWndSearchStringsSpin;
  static HWND hWndSearchStringsClear;
  static HWND hWndFileTypesOpen;
  static HWND hWndAssociateOpen;
  static HWND hWndFileTypesEdit;
  static HWND hWndAssociateEdit;
  static HWND hWndFileTypesPrint;
  static HWND hWndAssociatePrint;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndSaveRegistry=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_REGISTRY);
    hWndSaveINI=GetDlgItem(hDlg, IDC_OPTIONS_SAVESETTINGS_INI);
    hWndSavePositions=GetDlgItem(hDlg, IDC_OPTIONS_SAVEPOSITIONS);
    hWndSaveCodepages=GetDlgItem(hDlg, IDC_OPTIONS_SAVECODEPAGES);
    hWndRecentFiles=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT);
    hWndRecentFilesSpin=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT_SPIN);
    hWndRecentFilesClear=GetDlgItem(hDlg, IDC_OPTIONS_RECENTFILES_CLEAR);
    hWndSearchStrings=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT);
    hWndSearchStringsSpin=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT_SPIN);
    hWndSearchStringsClear=GetDlgItem(hDlg, IDC_OPTIONS_SEARCHSTRINGS_CLEAR);
    hWndFileTypesOpen=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_OPEN);
    hWndAssociateOpen=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_OPEN);
    hWndFileTypesEdit=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_EDIT);
    hWndAssociateEdit=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_EDIT);
    hWndFileTypesPrint=GetDlgItem(hDlg, IDC_OPTIONS_FILETYPES_PRINT);
    hWndAssociatePrint=GetDlgItem(hDlg, IDC_OPTIONS_ASSOCIATE_PRINT);

    SendMessage(hWndRecentFilesSpin, UDM_SETBUDDY, (WPARAM)hWndRecentFiles, 0);
    SendMessage(hWndRecentFilesSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndSearchStringsSpin, UDM_SETBUDDY, (WPARAM)hWndSearchStrings, 0);
    SendMessage(hWndSearchStringsSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));

    if (dwFileTypesAssociated & AE_OPEN) SendMessage(hWndAssociateOpen, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_EDIT) SendMessage(hWndAssociateEdit, BM_SETCHECK, BST_CHECKED, 0);
    if (dwFileTypesAssociated & AE_PRINT) SendMessage(hWndAssociatePrint, BM_SETCHECK, BST_CHECKED, 0);
    if (!(dwFileTypesAssociated & AE_OPEN)) EnableWindow(hWndFileTypesOpen, FALSE);
    if (!(dwFileTypesAssociated & AE_EDIT)) EnableWindow(hWndFileTypesEdit, FALSE);
    if (!(dwFileTypesAssociated & AE_PRINT)) EnableWindow(hWndFileTypesPrint, FALSE);
    SendMessage(hWndFileTypesOpen, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesEdit, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesPrint, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);

    SetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, nRecentFiles, FALSE);
    if (!nRecentFiles || !*lpwszRecentNames[0]) EnableWindow(hWndRecentFilesClear, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, nSearchStrings, FALSE);
    if (!nSearchStrings) EnableWindow(hWndSearchStringsClear, FALSE);

    SetWindowTextW(hWndFileTypesOpen, wszFileTypesOpen);
    SetWindowTextW(hWndFileTypesEdit, wszFileTypesEdit);
    SetWindowTextW(hWndFileTypesPrint, wszFileTypesPrint);

    if (nSaveSettings == SS_REGISTRY)
      SendMessage(hWndSaveRegistry, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndSaveINI, BM_SETCHECK, BST_CHECKED, 0);
    if (bSavePositions)
      SendMessage(hWndSavePositions, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveCodepages)
      SendMessage(hWndSaveCodepages, BM_SETCHECK, BST_CHECKED, 0);
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
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_OPEN)
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
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      wchar_t wszWindowText[MAX_PATH];
      BOOL bShellRefresh=FALSE;
      int a;
      int b;

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

      //Recent files
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, NULL, FALSE);
      if (nRecentFiles != a)
      {
        nRecentFiles=a;
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

      //Search history
      nSearchStrings=GetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, NULL, FALSE);

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
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsEditorDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndMarginLeft;
  static HWND hWndMarginLeftSpin;
  static HWND hWndMarginRight;
  static HWND hWndMarginRightSpin;
  static HWND hWndTabSize;
  static HWND hWndTabSizeSpin;
  static HWND hWndTabSizeSpaces;
  static HWND hWndUndoLimit;
  static HWND hWndUndoLimitSpin;
  static HWND hWndDetailedUndo;
  static HWND hWndWrapByWords;
  static HWND hWndWrapBySymbols;
  static HWND hWndWrapLimit;
  static HWND hWndWrapLimitSpin;
  static HWND hWndMarker;
  static HWND hWndMarkerSpin;
  static HWND hWndCaretOutEdge;
  static HWND hWndCaretVertLine;
  static HWND hWndCaretWidth;
  static HWND hWndCaretWidthSpin;
  static HWND hWndLineGap;
  static HWND hWndLineGapSpin;

  if (uMsg == WM_INITDIALOG)
  {
    hWndMarginLeft=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT);
    hWndMarginLeftSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT_SPIN);
    hWndMarginRight=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT);
    hWndMarginRightSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT_SPIN);
    hWndTabSize=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE);
    hWndTabSizeSpin=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPIN);
    hWndTabSizeSpaces=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPACES);
    hWndUndoLimit=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT);
    hWndUndoLimitSpin=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT_SPIN);
    hWndDetailedUndo=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_DETAILED);
    hWndWrapByWords=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_BY_WORDS);
    hWndWrapBySymbols=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_BY_SYMBOLS);
    hWndWrapLimit=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_LIMIT);
    hWndWrapLimitSpin=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_LIMIT_SPIN);
    hWndMarker=GetDlgItem(hDlg, IDC_OPTIONS_MARKER);
    hWndMarkerSpin=GetDlgItem(hDlg, IDC_OPTIONS_MARKER_SPIN);
    hWndCaretOutEdge=GetDlgItem(hDlg, IDC_OPTIONS_CARETOUTEDGE);
    hWndCaretVertLine=GetDlgItem(hDlg, IDC_OPTIONS_CARETVERTLINE);
    hWndCaretWidth=GetDlgItem(hDlg, IDC_OPTIONS_CARETWIDTH);
    hWndCaretWidthSpin=GetDlgItem(hDlg, IDC_OPTIONS_CARETWIDTH_SPIN);
    hWndLineGap=GetDlgItem(hDlg, IDC_OPTIONS_LINEGAP);
    hWndLineGapSpin=GetDlgItem(hDlg, IDC_OPTIONS_LINEGAP_SPIN);

    SendMessage(hWndMarginLeftSpin, UDM_SETBUDDY, (WPARAM)hWndMarginLeft, 0);
    SendMessage(hWndMarginLeftSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndMarginRightSpin, UDM_SETBUDDY, (WPARAM)hWndMarginRight, 0);
    SendMessage(hWndMarginRightSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndTabSizeSpin, UDM_SETBUDDY, (WPARAM)hWndTabSize, 0);
    SendMessage(hWndTabSizeSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 1));
    SendMessage(hWndUndoLimitSpin, UDM_SETBUDDY, (WPARAM)hWndUndoLimit, 0);
    SendMessage(hWndUndoLimitSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndWrapLimitSpin, UDM_SETBUDDY, (WPARAM)hWndWrapLimit, 0);
    SendMessage(hWndWrapLimitSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndMarkerSpin, UDM_SETBUDDY, (WPARAM)hWndMarker, 0);
    SendMessage(hWndMarkerSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndCaretWidthSpin, UDM_SETBUDDY, (WPARAM)hWndCaretWidth, 0);
    SendMessage(hWndCaretWidthSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 1));
    SendMessage(hWndLineGapSpin, UDM_SETBUDDY, (WPARAM)hWndLineGap, 0);
    SendMessage(hWndLineGapSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 0));

    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, LOWORD(dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, HIWORD(dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, nTabStopSize, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, nUndoLimit, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_WRAP_LIMIT, dwWrapLimit, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_MARKER, dwMarker, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_CARETWIDTH, nCaretWidth, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_LINEGAP, dwLineGap, FALSE);

    if (bTabStopAsSpaces)
      SendMessage(hWndTabSizeSpaces, BM_SETCHECK, BST_CHECKED, 0);
    if (bDetailedUndo)
      SendMessage(hWndDetailedUndo, BM_SETCHECK, BST_CHECKED, 0);
    if (nWrapType == AEWW_WORD)
      SendMessage(hWndWrapByWords, BM_SETCHECK, BST_CHECKED, 0);
    else if (nWrapType == AEWW_SYMBOL)
      SendMessage(hWndWrapBySymbols, BM_SETCHECK, BST_CHECKED, 0);
    if (bCaretOutEdge)
      SendMessage(hWndCaretOutEdge, BM_SETCHECK, BST_CHECKED, 0);
    if (bCaretVertLine)
      SendMessage(hWndCaretVertLine, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;
      int b;

      //Margins
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, NULL, FALSE);
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, NULL, FALSE);
      if (dwEditMargins != (DWORD)MAKELONG(a, b))
      {
        dwEditMargins=MAKELONG(a, b);
        SendMessage(hWndEdit, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
        InvalidateRect(hWndEdit, NULL, TRUE);
      }

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

      //Wrap
      if (SendMessage(hWndWrapByWords, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=AEWW_WORD;
      else
        a=AEWW_SYMBOL;
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_WRAP_LIMIT, NULL, FALSE);

      if (nWrapType != a || (int)dwWrapLimit != b)
      {
        if (bWordWrap)
        {
          UpdateShowHScroll(hWndEdit);
        }
        nWrapType=a;
        dwWrapLimit=b;

        if (bWordWrap)
        {
          UpdateShowHScroll(hWndEdit);
          SendMessage(hWndEdit, AEM_SETWORDWRAP, nWrapType, dwWrapLimit);
        }
      }

      //Marker
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_MARKER, NULL, FALSE);
      if ((int)dwMarker != a)
      {
        dwMarker=a;
        SendMessage(hWndEdit, AEM_SETMARKER, dwMarker, 0);
      }

      //Allow caret moving out of the line edge
      bCaretOutEdge=SendMessage(hWndCaretOutEdge, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);

      //Draw caret vertical line
      bCaretVertLine=SendMessage(hWndCaretVertLine, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bCaretVertLine?AECOOP_OR:AECOOP_XOR, AECO_ACTIVECOLUMN);

      //Caret width
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_CARETWIDTH, NULL, FALSE);
      if (nCaretWidth != a)
      {
        nCaretWidth=a;

        //Update width
        {
          POINT pt;

          SendMessage(hWndEdit, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);
          pt.x=nCaretWidth;
          SendMessage(hWndEdit, AEM_SETCARETWIDTH, 0, (LPARAM)&pt);
        }
      }

      //Line gap
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_LINEGAP, NULL, FALSE);
      if ((int)dwLineGap != a)
      {
        dwLineGap=a;

        SendMessage(hWndEdit, AEM_SETLINEGAP, dwLineGap, 0);
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsEditorDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndMarginLeft;
  static HWND hWndMarginLeftSpin;
  static HWND hWndMarginRight;
  static HWND hWndMarginRightSpin;
  static HWND hWndTabSize;
  static HWND hWndTabSizeSpin;
  static HWND hWndTabSizeSpaces;
  static HWND hWndUndoLimit;
  static HWND hWndUndoLimitSpin;
  static HWND hWndDetailedUndo;
  static HWND hWndWrapByWords;
  static HWND hWndWrapBySymbols;
  static HWND hWndWrapLimit;
  static HWND hWndWrapLimitSpin;
  static HWND hWndMarker;
  static HWND hWndMarkerSpin;
  static HWND hWndCaretOutEdge;
  static HWND hWndCaretVertLine;
  static HWND hWndCaretWidth;
  static HWND hWndCaretWidthSpin;
  static HWND hWndLineGap;
  static HWND hWndLineGapSpin;

  if (uMsg == WM_INITDIALOG)
  {
    hWndMarginLeft=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT);
    hWndMarginLeftSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT_SPIN);
    hWndMarginRight=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT);
    hWndMarginRightSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT_SPIN);
    hWndTabSize=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE);
    hWndTabSizeSpin=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPIN);
    hWndTabSizeSpaces=GetDlgItem(hDlg, IDC_OPTIONS_TABSIZE_SPACES);
    hWndUndoLimit=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT);
    hWndUndoLimitSpin=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_LIMIT_SPIN);
    hWndDetailedUndo=GetDlgItem(hDlg, IDC_OPTIONS_UNDO_DETAILED);
    hWndWrapByWords=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_BY_WORDS);
    hWndWrapBySymbols=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_BY_SYMBOLS);
    hWndWrapLimit=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_LIMIT);
    hWndWrapLimitSpin=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_LIMIT_SPIN);
    hWndMarker=GetDlgItem(hDlg, IDC_OPTIONS_MARKER);
    hWndMarkerSpin=GetDlgItem(hDlg, IDC_OPTIONS_MARKER_SPIN);
    hWndCaretOutEdge=GetDlgItem(hDlg, IDC_OPTIONS_CARETOUTEDGE);
    hWndCaretVertLine=GetDlgItem(hDlg, IDC_OPTIONS_CARETVERTLINE);
    hWndCaretWidth=GetDlgItem(hDlg, IDC_OPTIONS_CARETWIDTH);
    hWndCaretWidthSpin=GetDlgItem(hDlg, IDC_OPTIONS_CARETWIDTH_SPIN);
    hWndLineGap=GetDlgItem(hDlg, IDC_OPTIONS_LINEGAP);
    hWndLineGapSpin=GetDlgItem(hDlg, IDC_OPTIONS_LINEGAP_SPIN);

    SendMessage(hWndMarginLeftSpin, UDM_SETBUDDY, (WPARAM)hWndMarginLeft, 0);
    SendMessage(hWndMarginLeftSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndMarginRightSpin, UDM_SETBUDDY, (WPARAM)hWndMarginRight, 0);
    SendMessage(hWndMarginRightSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(999, 0));
    SendMessage(hWndTabSizeSpin, UDM_SETBUDDY, (WPARAM)hWndTabSize, 0);
    SendMessage(hWndTabSizeSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 1));
    SendMessage(hWndUndoLimitSpin, UDM_SETBUDDY, (WPARAM)hWndUndoLimit, 0);
    SendMessage(hWndUndoLimitSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndWrapLimitSpin, UDM_SETBUDDY, (WPARAM)hWndWrapLimit, 0);
    SendMessage(hWndWrapLimitSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndMarkerSpin, UDM_SETBUDDY, (WPARAM)hWndMarker, 0);
    SendMessage(hWndMarkerSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(9999, 0));
    SendMessage(hWndCaretWidthSpin, UDM_SETBUDDY, (WPARAM)hWndCaretWidth, 0);
    SendMessage(hWndCaretWidthSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 1));
    SendMessage(hWndLineGapSpin, UDM_SETBUDDY, (WPARAM)hWndLineGap, 0);
    SendMessage(hWndLineGapSpin, UDM_SETRANGE, 0, (LPARAM)MAKELONG(100, 0));

    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, LOWORD(dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, HIWORD(dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, nTabStopSize, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, nUndoLimit, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_WRAP_LIMIT, dwWrapLimit, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_MARKER, dwMarker, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_CARETWIDTH, nCaretWidth, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_LINEGAP, dwLineGap, FALSE);

    if (bTabStopAsSpaces)
      SendMessage(hWndTabSizeSpaces, BM_SETCHECK, BST_CHECKED, 0);
    if (bDetailedUndo)
      SendMessage(hWndDetailedUndo, BM_SETCHECK, BST_CHECKED, 0);
    if (nWrapType == AEWW_WORD)
      SendMessage(hWndWrapByWords, BM_SETCHECK, BST_CHECKED, 0);
    else if (nWrapType == AEWW_SYMBOL)
      SendMessage(hWndWrapBySymbols, BM_SETCHECK, BST_CHECKED, 0);
    if (bCaretOutEdge)
      SendMessage(hWndCaretOutEdge, BM_SETCHECK, BST_CHECKED, 0);
    if (bCaretVertLine)
      SendMessage(hWndCaretVertLine, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;
      int b;

      //Margins
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, NULL, FALSE);
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, NULL, FALSE);
      if (dwEditMargins != (DWORD)MAKELONG(a, b))
      {
        dwEditMargins=MAKELONG(a, b);
        SendMessage(hWndEdit, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwEditMargins);
        InvalidateRect(hWndEdit, NULL, TRUE);
      }

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

      //Wrap
      if (SendMessage(hWndWrapByWords, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=AEWW_WORD;
      else
        a=AEWW_SYMBOL;
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_WRAP_LIMIT, NULL, FALSE);

      if (nWrapType != a || (int)dwWrapLimit != b)
      {
        if (bWordWrap)
        {
          UpdateShowHScroll(hWndEdit);
        }
        nWrapType=a;
        dwWrapLimit=b;

        if (bWordWrap)
        {
          UpdateShowHScroll(hWndEdit);
          SendMessage(hWndEdit, AEM_SETWORDWRAP, nWrapType, dwWrapLimit);
        }
      }

      //Marker
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_MARKER, NULL, FALSE);
      if ((int)dwMarker != a)
      {
        dwMarker=a;
        SendMessage(hWndEdit, AEM_SETMARKER, dwMarker, 0);
      }

      //Allow caret moving out of the line edge
      bCaretOutEdge=SendMessage(hWndCaretOutEdge, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bCaretOutEdge?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);

      //Draw caret vertical line
      bCaretVertLine=SendMessage(hWndCaretVertLine, BM_GETCHECK, 0, 0);
      SendMessage(hWndEdit, AEM_SETOPTIONS, bCaretVertLine?AECOOP_OR:AECOOP_XOR, AECO_ACTIVECOLUMN);

      //Caret width
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_CARETWIDTH, NULL, FALSE);
      if (nCaretWidth != a)
      {
        nCaretWidth=a;

        //Update width
        {
          POINT pt;

          SendMessage(hWndEdit, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);
          pt.x=nCaretWidth;
          SendMessage(hWndEdit, AEM_SETCARETWIDTH, 0, (LPARAM)&pt);
        }
      }

      //Line gap
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_LINEGAP, NULL, FALSE);
      if ((int)dwLineGap != a)
      {
        dwLineGap=a;

        SendMessage(hWndEdit, AEM_SETLINEGAP, dwLineGap, 0);
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsAdvancedDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndShowURL;
  static HWND hWndSingleClickURL;
  static HWND hWndDoubleClickURL;
  static HWND hWndUrlPrefixesEnable;
  static HWND hWndUrlPrefixes;
  static HWND hWndUrlDelimitersEnable;
  static HWND hWndUrlDelimiters;
  static HWND hWndWordDelimitersEnable;
  static HWND hWndWordDelimiters;
  static HWND hWndWrapDelimitersEnable;
  static HWND hWndWrapDelimiters;
  static HWND hWndReplaceAllAndClose;
  static HWND hWndSaveInReadOnlyMsg;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndShowURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SHOW);
    hWndSingleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SINGLECLICK);
    hWndDoubleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_DOUBLECLICK);
    hWndUrlPrefixesEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES_ENABLE);
    hWndUrlPrefixes=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES);
    hWndUrlDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS_ENABLE);
    hWndUrlDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS);
    hWndWordDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS_ENABLE);
    hWndWordDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS);
    hWndWrapDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_DELIMITERS_ENABLE);
    hWndWrapDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_DELIMITERS);
    hWndReplaceAllAndClose=GetDlgItem(hDlg, IDC_OPTIONS_REPLACEALL_CLOSE);
    hWndSaveInReadOnlyMsg=GetDlgItem(hDlg, IDC_OPTIONS_SAVEIN_READONLY_MSG);

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

    if (bWordDelimitersEnable)
      SendMessage(hWndWordDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWordDelimiters, bWordDelimitersEnable);
    EscapeDataToEscapeStringW(wszWordDelimiters, (wchar_t *)buf);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t *)buf, -1, buf2, BUFFER_SIZE, NULL, NULL);
    SetWindowTextA(hWndWordDelimiters, buf2);
    SendMessage(hWndWordDelimiters, EM_LIMITTEXT, (WPARAM)WORD_DELIMITERS_SIZE, 0);

    if (bWrapDelimitersEnable)
      SendMessage(hWndWrapDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWrapDelimiters, bWrapDelimitersEnable);
    EscapeDataToEscapeStringW(wszWrapDelimiters, (wchar_t *)buf);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t *)buf, -1, buf2, BUFFER_SIZE, NULL, NULL);
    SetWindowTextA(hWndWrapDelimiters, buf2);
    SendMessage(hWndWrapDelimiters, EM_LIMITTEXT, (WPARAM)WRAP_DELIMITERS_SIZE, 0);

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
      SetWindowTextA(hWndWordDelimiters, buf2);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WRAP_DELIMITERS_ENABLE)
    {
      bState=SendMessage(hWndWrapDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndWrapDelimiters, bState);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WRAP_DELIMITERS_RESET)
    {
      EscapeDataToEscapeStringW(WRAP_DELIMITERSW, (wchar_t *)buf);
      WideCharToMultiByte(CP_ACP, 0, (wchar_t *)buf, -1, buf2, BUFFER_SIZE, NULL, NULL);
      SetWindowTextA(hWndWrapDelimiters, buf2);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;

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

      //Wrap delimiters
      a=GetWindowTextA(hWndWrapDelimiters, buf, BUFFER_SIZE);
      MultiByteToWideChar(CP_ACP, 0, buf, a + 1, (wchar_t *)buf2, BUFFER_SIZE / sizeof(wchar_t));
      EscapeStringToEscapeDataW((wchar_t *)buf2, wszWrapDelimiters);

      bWrapDelimitersEnable=SendMessage(hWndWrapDelimitersEnable, BM_GETCHECK, 0, 0);
      if (bWrapDelimitersEnable)
        SendMessage(hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)wszWrapDelimiters);
      else
        SendMessage(hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)NULL);

      //ReplaceAll and close dialog
      bReplaceAllAndClose=SendMessage(hWndReplaceAllAndClose, BM_GETCHECK, 0, 0);

      //Save in read only file message
      bSaveInReadOnlyMsg=SendMessage(hWndSaveInReadOnlyMsg, BM_GETCHECK, 0, 0);
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsAdvancedDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndShowURL;
  static HWND hWndSingleClickURL;
  static HWND hWndDoubleClickURL;
  static HWND hWndUrlPrefixesEnable;
  static HWND hWndUrlPrefixes;
  static HWND hWndUrlDelimitersEnable;
  static HWND hWndUrlDelimiters;
  static HWND hWndWordDelimitersEnable;
  static HWND hWndWordDelimiters;
  static HWND hWndWrapDelimitersEnable;
  static HWND hWndWrapDelimiters;
  static HWND hWndReplaceAllAndClose;
  static HWND hWndSaveInReadOnlyMsg;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndShowURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SHOW);
    hWndSingleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SINGLECLICK);
    hWndDoubleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_DOUBLECLICK);
    hWndUrlPrefixesEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES_ENABLE);
    hWndUrlPrefixes=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES);
    hWndUrlDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS_ENABLE);
    hWndUrlDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS);
    hWndWordDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS_ENABLE);
    hWndWordDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS);
    hWndWrapDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_DELIMITERS_ENABLE);
    hWndWrapDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_DELIMITERS);
    hWndReplaceAllAndClose=GetDlgItem(hDlg, IDC_OPTIONS_REPLACEALL_CLOSE);
    hWndSaveInReadOnlyMsg=GetDlgItem(hDlg, IDC_OPTIONS_SAVEIN_READONLY_MSG);

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

    if (bWordDelimitersEnable)
      SendMessage(hWndWordDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWordDelimiters, bWordDelimitersEnable);
    EscapeDataToEscapeStringW(wszWordDelimiters, wbuf);
    SetWindowTextW(hWndWordDelimiters, wbuf);
    SendMessage(hWndWordDelimiters, EM_LIMITTEXT, (WPARAM)WORD_DELIMITERS_SIZE, 0);

    if (bWrapDelimitersEnable)
      SendMessage(hWndWrapDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWrapDelimiters, bWrapDelimitersEnable);
    EscapeDataToEscapeStringW(wszWrapDelimiters, wbuf);
    SetWindowTextW(hWndWrapDelimiters, wbuf);
    SendMessage(hWndWrapDelimiters, EM_LIMITTEXT, (WPARAM)WRAP_DELIMITERS_SIZE, 0);

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
    else if (LOWORD(wParam) == IDC_OPTIONS_WORD_DELIMITERS_ENABLE)
    {
      bState=SendMessage(hWndWordDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndWordDelimiters, bState);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WORD_DELIMITERS_RESET)
    {
      EscapeDataToEscapeStringW(WORD_DELIMITERSW, wbuf);
      SetWindowTextW(hWndWordDelimiters, wbuf);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WRAP_DELIMITERS_ENABLE)
    {
      bState=SendMessage(hWndWrapDelimitersEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndWrapDelimiters, bState);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_WRAP_DELIMITERS_RESET)
    {
      EscapeDataToEscapeStringW(WRAP_DELIMITERSW, wbuf);
      SetWindowTextW(hWndWrapDelimiters, wbuf);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;

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

      //Wrap delimiters
      GetWindowTextW(hWndWrapDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, wszWrapDelimiters);

      bWrapDelimitersEnable=SendMessage(hWndWrapDelimitersEnable, BM_GETCHECK, 0, 0);
      if (bWrapDelimitersEnable)
        SendMessage(hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)wszWrapDelimiters);
      else
        SendMessage(hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)NULL);

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
  int nLine;
  int nColumn;

  if (cr && ci)
  {
    crSel=*cr;
    ciCaret=*ci;
  }
  else
  {
    GetSel(hWnd, &crSel, NULL, &ciCaret);
  }

  if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
    nLine=SendMessage(hWnd, AEM_GETUNWRAPLINE, (WPARAM)ciCaret.nLine, 0);
  else
    nLine=ciCaret.nLine;

  if (dwStatusPosType & SPT_COLUMN)
    nColumn=SendMessage(hWnd, AEM_GETINDEXCOLUMN, MAKELONG(nTabStopSize, !(dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);
  else
    nColumn=SendMessage(hWnd, AEM_GETINDEXCOLUMN, MAKELONG(1, !(dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);

  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
    wsprintfA(szStatus, "%u:%u", nLine + 1, nColumn);
  else
    wsprintfA(szStatus, "%u:%u, %u", nLine + 1, nColumn, IndexSubtract(hWnd, &crSel.ciMin, &crSel.ciMax, AELB_ASOUTPUT, -1));

  SendMessage(hStatus, SB_SETTEXTA, STATUS_POSITION, (LPARAM)szStatus);
}

void SetSelectionStatusW(HWND hWnd, AECHARRANGE *cr, AECHARINDEX *ci)
{
  wchar_t wszStatus[MAX_PATH];
  int nLine;
  int nColumn;

  if (cr && ci)
  {
    crSel=*cr;
    ciCaret=*ci;
  }
  else
  {
    GetSel(hWnd, &crSel, NULL, &ciCaret);
  }

  if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
    nLine=SendMessage(hWnd, AEM_GETUNWRAPLINE, (WPARAM)ciCaret.nLine, 0);
  else
    nLine=ciCaret.nLine;

  if (dwStatusPosType & SPT_COLUMN)
    nColumn=SendMessage(hWnd, AEM_GETINDEXCOLUMN, MAKELONG(nTabStopSize, !(dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);
  else
    nColumn=SendMessage(hWnd, AEM_GETINDEXCOLUMN, MAKELONG(1, !(dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);

  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
    wsprintfW(wszStatus, L"%u:%u", nLine + 1, nColumn);
  else
    wsprintfW(wszStatus, L"%u:%u, %u", nLine + 1, nColumn, IndexSubtract(hWnd, &crSel.ciMin, &crSel.ciMax, AELB_ASOUTPUT, -1));

  SendMessage(hStatus, SB_SETTEXTW, STATUS_POSITION, (LPARAM)wszStatus);
}

void SetModifyStatusA(HWND hWnd, BOOL bState, BOOL bFirst)
{
  if (hWnd) SendMessage(hWnd, AEM_SETMODIFY, bState, 0);

  if (!hWnd || IsEditActive(hWnd))
  {
    if (bFirst != TRUE && bModified == bState) return;
    bModified=bState;

    if (dwShowModify & SM_STATUSBAR)
    {
      API_LoadStringA(hLangLib, STR_MODIFIED, buf, BUFFER_SIZE);
      SendMessage(hStatus, SB_SETTEXTA, STATUS_MODIFY, (LPARAM)(bModified?buf:""));
    }
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

  if (!hWnd || IsEditActive(hWnd))
  {
    if (bFirst != TRUE && bModified == bState) return;
    bModified=bState;

    if (dwShowModify & SM_STATUSBAR)
    {
      API_LoadStringW(hLangLib, STR_MODIFIED, wbuf, BUFFER_SIZE);
      SendMessage(hStatus, SB_SETTEXTW, STATUS_MODIFY, (LPARAM)(bModified?wbuf:L""));
    }
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

    SendMessage(hWnd, AEM_UPDATESEL, AESELT_LOCKSCROLL|AESELT_COLUMNASIS, 0);
  }

  if (!hWnd || IsEditActive(hWnd))
  {
    if (bFirst != TRUE && nCurrentNewLine == nState) return;
    nCurrentNewLine=nState;

    if (nCurrentNewLine == NEWLINE_WIN)
      SendMessage(hStatus, SB_SETTEXTA, STATUS_NEWLINE, (LPARAM)"Win");
    else if (nCurrentNewLine == NEWLINE_UNIX)
      SendMessage(hStatus, SB_SETTEXTA, STATUS_NEWLINE, (LPARAM)"Unix");
    else if (nCurrentNewLine == NEWLINE_MAC)
      SendMessage(hStatus, SB_SETTEXTA, STATUS_NEWLINE, (LPARAM)"Mac");
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

    SendMessage(hWnd, AEM_UPDATESEL, AESELT_LOCKSCROLL|AESELT_COLUMNASIS, 0);
  }

  if (!hWnd || IsEditActive(hWnd))
  {
    if (bFirst != TRUE && nCurrentNewLine == nState) return;
    nCurrentNewLine=nState;

    if (nCurrentNewLine == NEWLINE_WIN)
      SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)L"Win");
    else if (nCurrentNewLine == NEWLINE_UNIX)
      SendMessage(hStatus, SB_SETTEXTW, STATUS_NEWLINE, (LPARAM)L"Unix");
    else if (nCurrentNewLine == NEWLINE_MAC)
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

BOOL GetEditInfoA(HWND hWnd, EDITINFO *ei)
{
  if (!hWnd || IsEditActive(hWnd))
  {
    if (hWndEdit)
    {
      ei->hWndEdit=hWndEdit;
      ei->pFile=(unsigned char *)szCurrentFile;
      ei->nCodePage=nCurrentCodePage;
      ei->bBOM=bCurrentBOM;
      ei->nNewLine=nCurrentNewLine;
      ei->bModified=bModified;
      ei->bReadOnly=bReadOnly;
      ei->bWordWrap=bWordWrap;
      ei->bInsertState=bInsertState;
      return TRUE;
    }
    return FALSE;
  }
  if (bMDI)
  {
    WNDFRAMEA *wf;
    HWND hWndFrame;

    if (hWndFrame=GetParent(hWnd))
    {
      if (wf=(WNDFRAMEA *)GetWindowLongA(hWndFrame, GWL_USERDATA))
      {
        *ei=wf->ei;
        return TRUE;
      }
    }
  }
  return FALSE;
}

BOOL GetEditInfoW(HWND hWnd, EDITINFO *ei)
{
  if (!hWnd || IsEditActive(hWnd))
  {
    if (hWndEdit)
    {
      ei->hWndEdit=hWndEdit;
      ei->pFile=(unsigned char *)wszCurrentFile;
      ei->nCodePage=nCurrentCodePage;
      ei->bBOM=bCurrentBOM;
      ei->nNewLine=nCurrentNewLine;
      ei->bModified=bModified;
      ei->bReadOnly=bReadOnly;
      ei->bWordWrap=bWordWrap;
      ei->bInsertState=bInsertState;
      return TRUE;
    }
    return FALSE;
  }
  if (bMDI)
  {
    WNDFRAMEW *wf;
    HWND hWndFrame;

    if (hWndFrame=GetParent(hWnd))
    {
      if (wf=(WNDFRAMEW *)GetWindowLongW(hWndFrame, GWL_USERDATA))
      {
        *ei=wf->ei;
        return TRUE;
      }
    }
  }
  return FALSE;
}

DWORD IsEditActive(HWND hWnd)
{
  if (hWnd == hWndEdit)
    return CN_EDIT;

  if (hWndMaster)
  {
    if (hWnd == hWndMaster)
      return CN_MASTER;
    if (hWndClone1 && hWnd == hWndClone1)
      return CN_CLONE1;
    if (hWndClone2 && hWnd == hWndClone2)
      return CN_CLONE2;
    if (hWndClone3 && hWnd == hWndClone3)
      return CN_CLONE3;
  }
  return 0;
}

void UpdateShowHScroll(HWND hWnd)
{
  BOOL bShowScroll=TRUE;

  if (bWordWrap && !dwWrapLimit && !hWndClone1 && !hWndClone3)
    bShowScroll=FALSE;

  if (!bSplitWindow)
  {
    SendMessage(hWnd, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
  }
  else
  {
    if (hWndMaster) SendMessage(hWndMaster, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
    if (hWndClone1) SendMessage(hWndClone1, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
    if (hWndClone2) SendMessage(hWndClone2, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
    if (hWndClone3) SendMessage(hWndClone3, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
  }
}

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
  AECHARINDEX ciChar=cr->ciMin;
  wchar_t *wpText;

  if (!(dwStatusPosType & SPT_LINEWRAP) && bWordWrap)
    SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEBEGIN, (LPARAM)&ciChar);

  //Calculate spaces
  for (ciChar.nCharInLine=0; AEC_IndexCompare(&ciChar, &cr->ciMin) < 0 &&
                             (ciChar.lpLine->wpLine[ciChar.nCharInLine] == ' ' ||
                              ciChar.lpLine->wpLine[ciChar.nCharInLine] == '\t'); ++ciChar.nCharInLine);

  if (ciChar.nCharInLine)
  {
    //Insert spaces
    if (wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, (ciChar.nCharInLine + 2) * sizeof(wchar_t)))
    {
      wpText[0]='\n';

      for (ciChar.nCharInLine=0; AEC_IndexCompare(&ciChar, &cr->ciMin) < 0 &&
                                 (ciChar.lpLine->wpLine[ciChar.nCharInLine] == ' ' ||
                                  ciChar.lpLine->wpLine[ciChar.nCharInLine] == '\t'); ++ciChar.nCharInLine)
      {
        wpText[ciChar.nCharInLine + 1]=ciChar.lpLine->wpLine[ciChar.nCharInLine];
      }
      wpText[ciChar.nCharInLine + 1]='\0';

      ReplaceSelW(hWnd, wpText, -1, FALSE, NULL, NULL);
      API_HeapFree(hHeap, 0, (LPVOID)wpText);
      return TRUE;
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
    if (pExtEnd=AKD_strchr(pExtStart, ';'))
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
    if (wpExtEnd=AKD_wcschr(wpExtStart, ';'))
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
/*
  WIN32_FIND_DATAA wfdA;
  HANDLE hFind;

  if ((hFind=FindFirstFileA(pFile, &wfdA)) == INVALID_HANDLE_VALUE)
    return FALSE;

  FindClose(hFind);
  return TRUE;
*/
  if (GetFileAttributesA(pFile) == INVALID_FILE_ATTRIBUTES)
    return FALSE;
  return TRUE;
}

BOOL FileExistsW(wchar_t *wpFile)
{
/*
  WIN32_FIND_DATAW wfdW;
  HANDLE hFind;

  if ((hFind=FindFirstFileW(wpFile, &wfdW)) == INVALID_HANDLE_VALUE)
    return FALSE;

  FindClose(hFind);
  return TRUE;
*/
  if (GetFileAttributesW(wpFile) == INVALID_FILE_ATTRIBUTES)
    return FALSE;
  return TRUE;
}

int IsFileA(char *pFile)
{
  DWORD dwAttr;

  dwAttr=GetFileAttributesA(pFile);
  if (dwAttr == INVALID_FILE_ATTRIBUTES)
    return ERROR_INVALID_HANDLE;
  if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
    return ERROR_FILE_NOT_FOUND;
  return ERROR_SUCCESS;
}

int IsFileW(wchar_t *wpFile)
{
  DWORD dwAttr;

  dwAttr=GetFileAttributesW(wpFile);
  if (dwAttr == INVALID_FILE_ATTRIBUTES)
    return ERROR_INVALID_HANDLE;
  if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
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
    if (FileExistsA(buf))
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
    if (FileExistsW(wbuf))
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

void TrimModifyStateA(char *szFile)
{
  int i;

  for (i=lstrlenA(szFile) - 1; i >= 0; --i)
  {
    if (szFile[i] == '*' || szFile[i] == ' ')
      szFile[i]='\0';
    else
      break;
  }
}

void TrimModifyStateW(wchar_t *wszFile)
{
  int i;

  for (i=lstrlenW(wszFile) - 1; i >= 0; --i)
  {
    if (wszFile[i] == '*' || wszFile[i] == ' ')
      wszFile[i]='\0';
    else
      break;
  }
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
      if (!IsEditActive(hWndFocus) && hWndFocus != hMdiClient)
        dkElement=StackDockFindWindow(&hDocksStack, hWndFocus, TRUE);

      hWndNext=StackDockNextWindow(&hDocksStack, dkElement, bPrevious);
    }
    if (!hWndNext) hWndNext=hMainWnd;
    SetFocus(hWndNext);
  }
  return hWndNext;
}

HWND NextClone(BOOL bPrevious)
{
  HWND lpClones[]={hWndMaster, hWndClone1, hWndClone2, hWndClone3};
  int nCloneCount=sizeof(lpClones) / sizeof(HWND);
  int i;

  if (hWndMaster && hWndEdit)
  {
    for (i=0; i < nCloneCount; ++i)
    {
      if (hWndEdit == lpClones[i])
        break;
    }

    while (i < nCloneCount)
    {
      if (bPrevious)
      {
        if (--i < 0)
          i=nCloneCount - 1;
      }
      else
      {
        if (++i >= nCloneCount)
          i=0;
      }

      if (lpClones[i])
      {
        SetFocus(lpClones[i]);
        return lpClones[i];
      }
    }
  }
  return NULL;
}

void DestroyEdit(DWORD dwFlags, HWND *hWndEdit, HWND *hWndMaster, HWND *hWndClone1, HWND *hWndClone2, HWND *hWndClone3)
{
  if (hWndMaster && *hWndMaster)
  {
    if (dwFlags & CN_CLONE1)
    {
      if (hWndClone1 && *hWndClone1)
      {
        SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)*hWndClone1, 0);
        SendMessage(*hWndMaster, AEM_DELCLONE, (WPARAM)*hWndClone1, 0);
        DestroyWindow(*hWndClone1);

        if (hWndEdit && *hWndEdit && *hWndClone1 == *hWndEdit)
          *hWndEdit=*hWndMaster;
        *hWndClone1=NULL;
      }
    }
    if (dwFlags & CN_CLONE2)
    {
      if (hWndClone2 && *hWndClone2)
      {
        SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)*hWndClone2, 0);
        SendMessage(*hWndMaster, AEM_DELCLONE, (WPARAM)*hWndClone2, 0);
        DestroyWindow(*hWndClone2);

        if (hWndEdit && *hWndEdit && *hWndClone2 == *hWndEdit)
          *hWndEdit=*hWndMaster;
        *hWndClone2=NULL;
      }
    }
    if (dwFlags & CN_CLONE3)
    {
      if (hWndClone3 && *hWndClone3)
      {
        SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)*hWndClone3, 0);
        SendMessage(*hWndMaster, AEM_DELCLONE, (WPARAM)*hWndClone3, 0);
        DestroyWindow(*hWndClone3);

        if (hWndEdit && *hWndEdit && *hWndClone3 == *hWndEdit)
          *hWndEdit=*hWndMaster;
        *hWndClone3=NULL;
      }
    }
    if (dwFlags & CN_MASTER)
    {
      if (hWndMaster && *hWndMaster)
      {
        SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)*hWndMaster, 0);
        DestroyWindow(*hWndMaster);

        if (hWndEdit && *hWndEdit && *hWndMaster == *hWndEdit)
          *hWndEdit=NULL;
        *hWndMaster=NULL;
      }
    }
  }
  if (dwFlags & CN_EDIT)
  {
    if (hWndEdit && *hWndEdit)
    {
      SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)*hWndEdit, 0);
      DestroyWindow(*hWndEdit);

      *hWndEdit=NULL;
    }
  }
}

void ResizeEdit(HWND hWndEdit, HWND hWndMaster, HWND hWndClone1, HWND hWndClone2, HWND hWndClone3, int X, int Y, int nWidth, int nHeight, RECT *rcMasterWindow, RECT *rcEditWindow, BOOL bTest)
{
  UpdateWindow(hStatus);

  if (hWndMaster)
  {
    HWND hWndParent=GetParent(hWndMaster);
    RECT rc;

    rcMasterWindow->left=X;
    rcMasterWindow->top=Y;

    if (hWndClone1)
    {
      rcMasterWindow->right=max(rcMasterWindow->right, 40);
      rcMasterWindow->right=min(rcMasterWindow->right, nWidth - 40);
      rcMasterWindow->right=max(rcMasterWindow->right, 0);
    }
    else rcMasterWindow->right=nWidth;

    if (hWndClone2)
    {
      rcMasterWindow->bottom=max(rcMasterWindow->bottom, 40);
      rcMasterWindow->bottom=min(rcMasterWindow->bottom, nHeight - 40);
      rcMasterWindow->bottom=max(rcMasterWindow->bottom, 0);
    }
    else rcMasterWindow->bottom=nHeight;

    if (hWndMaster)
    {
      rc.left=rcMasterWindow->left;
      rc.top=rcMasterWindow->top;
      rc.right=rcMasterWindow->right;
      rc.bottom=rcMasterWindow->bottom;

      if (!bTest)
      {
        MoveWindow(hWndMaster, rc.left, rc.top, rc.right, rc.bottom, TRUE);
      }
      else
      {
        ClientToScreenRect(hWndParent, &rc);
        DrawMovingRect(&rc);
      }
    }
    if (hWndClone1)
    {
      rc.left=rcMasterWindow->left + rcMasterWindow->right;
      rc.top=rcMasterWindow->top;
      rc.right=nWidth - rcMasterWindow->right;
      rc.bottom=rcMasterWindow->bottom;

      if (!bTest)
      {
        MoveWindow(hWndClone1, rc.left, rc.top, rc.right, rc.bottom, TRUE);
      }
      else
      {
        ClientToScreenRect(hWndParent, &rc);
        DrawMovingRect(&rc);
      }
    }
    if (hWndClone2)
    {
      rc.left=rcMasterWindow->left;
      rc.top=rcMasterWindow->top + rcMasterWindow->bottom;
      rc.right=rcMasterWindow->right;
      rc.bottom=nHeight - rcMasterWindow->bottom;

      if (!bTest)
      {
        MoveWindow(hWndClone2, rc.left, rc.top, rc.right, rc.bottom, TRUE);
      }
      else
      {
        ClientToScreenRect(hWndParent, &rc);
        DrawMovingRect(&rc);
      }
    }
    if (hWndClone3)
    {
      rc.left=rcMasterWindow->left + rcMasterWindow->right;
      rc.top=rcMasterWindow->top + rcMasterWindow->bottom;
      rc.right=nWidth - rcMasterWindow->right;
      rc.bottom=nHeight - rcMasterWindow->bottom;

      if (!bTest)
      {
        MoveWindow(hWndClone3, rc.left, rc.top, rc.right, rc.bottom, TRUE);
      }
      else
      {
        ClientToScreenRect(hWndParent, &rc);
        DrawMovingRect(&rc);
      }
    }
  }
  else
  {
    if (!bTest)
    {
      MoveWindow(hWndEdit, X, Y, nWidth, nHeight, TRUE);
    }
  }

  rcEditWindow->left=X;
  rcEditWindow->top=Y;
  rcEditWindow->right=nWidth;
  rcEditWindow->bottom=nHeight;
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
      ResizeEdit(hWndEdit, hWndMaster, hWndClone1, hWndClone2, hWndClone3, nsSize.rcCurrent.left, nsSize.rcCurrent.top, nsSize.rcCurrent.right, nsSize.rcCurrent.bottom, &rcMasterWindow, &rcEditWindow, FALSE);
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

BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcDialog, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static RECT rcInitDialog;
  static RECT rcTempDialog;
  static BOOL bRectChanged;

  if (!rcDialog) rcDialog=&rcTempDialog;

  if (uMsg == WM_INITDIALOG)
  {
    RECT rcTemplate;

    GetWindowPos(hDlg, NULL, &rcInitDialog);
    rcTemplate=*rcDialog;
    *rcDialog=rcInitDialog;

    if (rcTemplate.right && rcTemplate.bottom)
    {
      rcTemplate.left=rcInitDialog.left + (rcInitDialog.right - rcTemplate.right) / 2;
      rcTemplate.top=rcInitDialog.top + (rcInitDialog.bottom - rcTemplate.bottom) / 2;
      SetWindowPos(hDlg, 0, rcTemplate.left, rcTemplate.top, rcTemplate.right, rcTemplate.bottom, SWP_NOZORDER);
    }
    bRectChanged=FALSE;
  }
  else if (uMsg == WM_GETMINMAXINFO)
  {
    MINMAXINFO *mmi=(MINMAXINFO *)lParam;

    mmi->ptMinTrackSize.x=rcInitDialog.right;
    mmi->ptMinTrackSize.y=rcInitDialog.bottom;
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
      pt.x=rcTemplate.right - rcDialog->right;
      pt.y=rcTemplate.bottom - rcDialog->bottom;
      *rcDialog=rcTemplate;

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
      bRectChanged=TRUE;
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
  return bRectChanged;
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
  RECT rcClient=*rcScreen;
  HDC hDC;
  HBITMAP hBitmap;
  HBRUSH hBrush;
  HBRUSH hBrushOld;

  if (!ScreenToClient(hMainWnd, (POINT *)&rcClient.left))
    return;
  if (!ScreenToClient(hMainWnd, (POINT *)&rcClient.right))
    return;

  if (hDC=GetDC(hMainWnd))
  {
    hBitmap=CreateBitmap(8, 8, 1, 1, DotPattern);
    hBrush=CreatePatternBrush(hBitmap);
    hBrushOld=(HBRUSH)SelectObject(hDC, hBrush);

    PatBlt(hDC, rcClient.left, rcClient.top, DOCK_BORDER_1X, rcClient.bottom - rcClient.top - DOCK_BORDER_1X, PATINVERT);
    PatBlt(hDC, rcClient.left + DOCK_BORDER_1X, rcClient.top, rcClient.right - rcClient.left - DOCK_BORDER_1X, DOCK_BORDER_1X, PATINVERT);
    PatBlt(hDC, rcClient.right - DOCK_BORDER_1X, rcClient.top + DOCK_BORDER_1X, DOCK_BORDER_1X, rcClient.bottom - rcClient.top - DOCK_BORDER_1X, PATINVERT);
    PatBlt(hDC, rcClient.left, rcClient.bottom - DOCK_BORDER_1X, rcClient.right - rcClient.left - DOCK_BORDER_1X,  DOCK_BORDER_1X, PATINVERT);

    SelectObject(hDC, hBrushOld);
    DeleteObject(hBrush);
    DeleteObject(hBitmap);
    ReleaseDC(hMainWnd, hDC);
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

BOOL DeleteTabItem(HWND hWnd, int nIndex)
{
  TCITEMA tcItemA;

  tcItemA.mask=TCIF_IMAGE;
  if (SendMessage(hWnd, TCM_GETITEMA, nIndex, (LPARAM)&tcItemA))
  {
    SendMessage(hWnd, TCM_REMOVEIMAGE, tcItemA.iImage, 0);
    SendMessage(hWnd, TCM_DELETEITEM, nIndex, 0);
    return TRUE;
  }
  return FALSE;
}

void FreeMemorySearchA()
{
  if (szFindText_orig)
  {
    if (szFindText_orig == szFindText)
      szFindText=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)szFindText_orig);
    szFindText_orig=NULL;
  }
  if (szReplaceText_orig)
  {
    if (szReplaceText_orig == szReplaceText)
      szReplaceText=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)szReplaceText_orig);
    szReplaceText_orig=NULL;
  }

  if (szFindText)
  {
    API_HeapFree(hHeap, 0, (LPVOID)szFindText);
    szFindText=NULL;
  }
  if (szReplaceText)
  {
    API_HeapFree(hHeap, 0, (LPVOID)szReplaceText);
    szReplaceText=NULL;
  }
}

void FreeMemorySearchW()
{
  if (wszFindText_orig)
  {
    if (wszFindText_orig == wszFindText)
      wszFindText=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)wszFindText_orig);
    wszFindText_orig=NULL;
  }
  if (wszReplaceText_orig)
  {
    if (wszReplaceText_orig == wszReplaceText)
      wszReplaceText=NULL;
    API_HeapFree(hHeap, 0, (LPVOID)wszReplaceText_orig);
    wszReplaceText_orig=NULL;
  }

  if (wszFindText)
  {
    API_HeapFree(hHeap, 0, (LPVOID)wszFindText);
    wszFindText=NULL;
  }
  if (wszReplaceText)
  {
    API_HeapFree(hHeap, 0, (LPVOID)wszReplaceText);
    wszReplaceText=NULL;
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

char* AKD_strchr(const char *s, int c)
{
  if (c == '\r' || c == '\n')
  {
    while (*s != '\0' && *s != '\r' && *s != '\n')
      ++s;
    if (*s != '\0')
      return ((char *)s);
    return NULL;
  }
  else
  {
    while (*s != '\0' && *s != c)
      ++s;
    if (*s != '\0')
      return ((char *)s);
    return NULL;
  }
}

wchar_t* AKD_wcschr(const wchar_t *s, wchar_t c)
{
  if (c == L'\r' || c == L'\n')
  {
    while (*s != L'\0' && *s != L'\r' && *s != L'\n')
      ++s;
    if (*s != L'\0')
      return ((wchar_t *)s);
    return NULL;
  }
  else
  {
    while (*s != L'\0' && *s != c)
      ++s;
    if (*s != L'\0')
      return ((wchar_t *)s);
    return NULL;
  }
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
