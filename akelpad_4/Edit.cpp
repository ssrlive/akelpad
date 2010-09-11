#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0500
#define _WIN32_IE 0x0400
#include <windows.h>
#include <stddef.h>
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <richedit.h>
#include "StackFunc.h"
#include "WideFunc.h"
#include "AkelEdit\StrFunc.h"
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
extern DWORD dwCmdShow;
extern DWORD dwCmdLineOptions;
extern const wchar_t *wpCmdLine;
extern BOOL bMessageBox;

//Identification
extern DWORD dwExeVersion;
extern BOOL bOldWindows;
extern BOOL bOldRichEdit;
extern BOOL bOldComctl32;
extern BOOL bAkelEdit;
extern BOOL bWindowsNT;

//Buffers
extern wchar_t wszCmdLine[COMMANDLINE_SIZE];
extern wchar_t wszCmdArg[COMMANDARG_SIZE];
extern unsigned char pcTranslateBuffer[TRANSLATE_BUFFER_SIZE];
extern char buf[BUFFER_SIZE];
extern wchar_t wbuf[BUFFER_SIZE];
extern char buf2[BUFFER_SIZE];
extern wchar_t wbuf2[BUFFER_SIZE];

//Language
extern HMODULE hLangLib;
extern DWORD dwLangSystem;
extern DWORD dwLangModule;
extern int nLangModuleCount;

//Procedures
extern HSTACK hMainProcStack;
extern HSTACK hMainProcRetStack;
extern HSTACK hEditProcStack;
extern HSTACK hEditProcRetStack;
extern HSTACK hFrameProcStack;
extern HSTACK hFrameProcRetStack;
extern WNDPROC lpfnMainProc;
extern WNDPROCRET lpfnMainProcRet;
extern WNDPROC lpfnFrameProc;
extern WNDPROCRET lpfnFrameProcRet;
extern WNDPROC lpfnEditProc;
extern WNDPROCRET lpfnEditProcRet;

//Plugins
extern HSTACK hPluginsStack;
extern HSTACK hPluginListStack;
extern HSTACK hHandlesStack;
extern RECT rcPluginsInitDialog;
extern BOOL bSavePluginsStackOnExit;
extern WNDPROC OldHotkeyInputProc;

//INI
extern INIFILE hIniFile;
extern wchar_t wszIniFile[MAX_PATH];

//Main Window
extern MAINOPTIONS moInit;
extern MAINOPTIONS moCur;
extern HWND hMainWnd;
extern HWND hDummyWindow;
extern DWORD dwLastMainSize;
extern DWORD dwMouseCapture;
extern HACCEL hGlobalAccel;
extern HACCEL hMainAccel;
extern HICON hMainIcon;
extern HCURSOR hCursorDragMove;
extern HCURSOR hCursorHandOpen;
extern HCURSOR hCursorHandClose;
extern HMENU hMainMenu;
extern HMENU hPopupMenu;
extern HMENU hPopupEdit;
extern HMENU hPopupView;
extern HMENU hPopupCodepage;
extern HMENU hPopupOpenCodepage;
extern HMENU hPopupSaveCodepage;
extern HMENU hPopupHeadline;
extern HMENU hMenuRecentFiles;
extern HMENU hMenuLanguage;
extern HMENU hMenuWindow;
extern BOOL bMenuPopupCodepage;
extern BOOL bMenuRecentFiles;
extern BOOL bMenuLanguage;
extern BOOL bMainOnStart;
extern BOOL bMainOnFinish;
extern BOOL bEditOnFinish;

//Status window
extern STATUSSTATE ssStatus;
extern HWND hStatus;
extern HWND hProgress;
extern int nStatusHeight;
extern int nStatusParts;
extern int nProgressWidth;

//Clones
extern HCURSOR hCursorSizeWE;
extern HCURSOR hCursorSizeNS;
extern HCURSOR hCursorSizeALL;
extern HCURSOR hCursorClone;

//Docks
extern HDOCK hDocksStack;
extern NSIZE nsSize;
extern WNDPROC OldDockProc;
extern WNDPROC OldCloseButtonProc;

//Owner-drawn buttons
extern HSTACK hButtonDrawStack;

//Codepages
extern int *lpCodepageList;
extern int nCodepageListLen;
extern BOOL bCodepageListChanged;
extern int *lpCodepageTable;
extern int nCodepageTableCount;
extern BOOL bDefaultBOM;
extern int nAnsiCodePage;
extern int nOemCodePage;

//Recent files
extern wchar_t (*lpwszRecentNames)[MAX_PATH];
extern DWORD *lpdwRecentPositions;
extern DWORD *lpdwRecentCodepages;

//Open/Save document
extern wchar_t wszFilter[MAX_PATH];
extern int nFilterLen;
extern BOOL bAutodetect;
extern BOOL bSaveDlg;
extern DWORD dwOfnFlags;
extern BOOL bOfnBOM;
extern int nOfnCodePage;
extern POINT ptDocumentPos;
extern WNDPROC OldFilePreviewProc;

//Modeless
extern HWND hDlgModeless;
extern int nModelessType;

//Recode dialog
extern RECT rcRecodeDlg;

//Find/Replace dialog
extern RECT rcFindAndReplaceDlg;
extern wchar_t *wszFindText_orig;
extern wchar_t *wszReplaceText_orig;
extern wchar_t *wszFindText;
extern wchar_t *wszReplaceText;
extern int nFindTextLen;
extern int nReplaceTextLen;
extern WNDPROC OldComboboxEdit;

//Go to line dialog
extern RECT rcGotoDlg;
extern int nGotoType;

//Options dialog
extern HHOOK hPropertyHook;
extern HWND hPropertyTab;
extern int nPropertyStartPage;
extern BOOL bOptionsSave;
extern BOOL bOptionsRestart;

//Font/Color
extern HSTACK hFontsStack;
extern HSTACK hThemesStack;
extern COLORREF crCustColors[16];
extern RECT rcColorsInitDialog;

//Print
extern HWND hWndPreviewEdit;
extern HWND hWndPreviewDlg;
extern HWND hWndZoomEdit;
extern HSTACK hPreviewAllPagesStack;
extern HSTACK hPreviewSelPagesStack;
extern RECT rcPreviewDialog;
extern RECT rcPreviewWindow;
extern RECT rcPreviewPaper;
extern RECT rcPreviewZoomed;
extern POINT ptPreviewScroll;
extern DWORD dwPreviewShowDialog;
extern int lpZoom[];
extern int nPreviewZoomMaxIndex;
extern int nPreviewZoomValue;
extern int nPreviewZoomPercent;
extern int nPreviewCharHeight;
extern int nPreviewAveCharWidth;
extern int nPreviewPageCur;
extern int nPreviewAllPageSum;
extern int nPreviewSelPageSum;
extern BOOL bPreviewSelection;
extern BOOL bPreviewRedrawLock;
extern HHOOK hHookKeys;
extern AEPRINT prn;
extern PRINTINFO prninfo;

//Edit state
extern AECHARRANGE crSel;
extern AECHARINDEX ciCaret;
extern int nLoopCase;
extern DWORD dwWordBreakDefault;
extern BOOL bReopenMsg;
extern WNDPROC OldEditProc;

//Execute
extern char szExeDir[MAX_PATH];
extern wchar_t wszExeDir[MAX_PATH];

//Mdi
extern HSTACK hFramesStack;
extern FRAMEDATA fdInit;
extern FRAMEDATA fdLast;
extern FRAMEDATA *lpFrameCurrent;
extern FRAMEDATA *lpFramePrevious;
extern int nMDI;
extern HWND hMdiClient;
extern BOOL bMdiMaximize;
extern BOOL bMdiNoWindows;
extern HWND hTab;
extern DWORD dwTabOpenTimer;
extern int nTabOpenItem;
extern HSTACK hIconsStack;
extern HIMAGELIST hImageList;
extern HICON hIconEmpty;
extern BOOL bTabPressed;
extern RECT rcMdiListInitDialog;
extern WNDPROC OldMdiClientProc;
extern WNDPROC OldTabProc;
extern FRAMEDATA *lpWndFrame;

//GetProcAddress
extern HMONITOR (WINAPI *MonitorFromPointPtr)(POINT, DWORD);
extern BOOL (WINAPI *GetMonitorInfoAPtr)(HMONITOR, LPMONITORINFO);


//// Init function

HANDLE CreateEditWindow(HWND hWndParent, HWND hWndEditPMDI)
{
  CREATESTRUCTW cs;
  RECT rcRect;
  HANDLE hResult;

  GetClientRect(hWndParent, &rcRect);

  cs.dwExStyle=(moCur.dwPaintOptions & PAINT_STATICEDGE)?WS_EX_STATICEDGE:WS_EX_CLIENTEDGE;
  if (moCur.bRichEditClass)
    cs.lpszClass=bOldWindows?L"RichEdit20A":L"RichEdit20W";
  else
    cs.lpszClass=bOldWindows?L"AkelEditA":L"AkelEditW";
  cs.lpszName=NULL;
  cs.style=WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|
           ((moCur.dwPaintOptions & PAINT_HIDESEL)?0:ES_NOHIDESEL)|
           ((moCur.dwPaintOptions & PAINT_HIDENOSCROLL)?0:ES_DISABLENOSCROLL);
  cs.x=0;
  cs.y=0;
  cs.cx=rcRect.right;
  cs.cy=rcRect.bottom - (moCur.bStatusBar?nStatusHeight:0);
  cs.hwndParent=hWndParent;
  cs.hMenu=(HMENU)ID_EDIT;
  cs.hInstance=hInstance;
  cs.lpCreateParams=NULL;

  if (hWndEditPMDI)
  {
    hResult=(HANDLE)SendMessage(hWndEditPMDI, AEM_CREATEDOCUMENT, 0, (LPARAM)&cs);
  }
  else
  {
    if (hResult=(HANDLE)CreateWindowExWide(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy, cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams))
    {
      OldEditProc=(WNDPROC)GetWindowLongWide((HWND)hResult, GWL_WNDPROC);
      SetWindowLongWide((HWND)hResult, GWL_WNDPROC, (LONG)CommonEditProc);
    }
  }
  return hResult;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SetEditWindowSettings(FRAMEDATA *lpFrame)
{
  DWORD dwOptions;

  //Set settings
  DoViewReadOnly(lpFrame, lpFrame->ei.bReadOnly, TRUE);
  SendMessage(lpFrame->ei.hWndEdit, AEM_SETEVENTMASK, 0, AENM_SCROLL|AENM_PROGRESS|AENM_MODIFY|AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_TEXTINSERT|AENM_TEXTDELETE|AENM_DRAGDROP|AENM_POINT|AENM_LINK);
  SendMessage(lpFrame->ei.hWndEdit, EM_SETEVENTMASK, 0, ENM_SELCHANGE|ENM_CHANGE|ENM_LINK);

  dwOptions=0;
  if (lpFrame->bDetailedUndo)
    dwOptions|=AECO_DETAILEDUNDO;
  if (lpFrame->dwCaretOptions & CO_CARETOUTEDGE)
    dwOptions|=AECO_CARETOUTEDGE;
  if (lpFrame->dwCaretOptions & CO_CARETVERTLINE)
    dwOptions|=AECO_ACTIVECOLUMN;
  if (lpFrame->dwMouseOptions & MO_RICHEDITMOUSE)
    dwOptions|=AECO_LBUTTONUPCONTINUECAPTURE;
  if (!(lpFrame->dwMouseOptions & MO_LEFTMARGINSELECTION))
    dwOptions|=AECO_NOMARGINSEL;
  if (!(lpFrame->dwMouseOptions & MO_MOUSEDRAGGING))
    dwOptions|=AECO_DISABLEDRAG;
  if (moCur.dwPaintOptions & PAINT_PAINTGROUP)
    dwOptions|=AECO_PAINTGROUP;
  if (moCur.dwPaintOptions & PAINT_NONEWLINEDRAW)
    dwOptions|=AECO_NONEWLINEDRAW;
  if (moCur.dwPaintOptions & PAINT_ENTIRENEWLINEDRAW)
    dwOptions|=AECO_ENTIRENEWLINEDRAW;
  SendMessage(lpFrame->ei.hWndEdit, AEM_SETOPTIONS, AECOOP_OR, dwOptions);

  //Font
  SetChosenFont(lpFrame->ei.hWndEdit, &lpFrame->lf);
  UpdateMappedPrintWidth(lpFrame);

  SendMessage(lpFrame->ei.hWndEdit, AEM_SETUNDOLIMIT, (WPARAM)lpFrame->nUndoLimit, 0);
  SendMessage(lpFrame->ei.hWndEdit, AEM_SETCOLORS, 0, (LPARAM)&lpFrame->aec);
  SetMargins(lpFrame->ei.hWndEdit, lpFrame->dwEditMargins, 0);
  SetTabStops(lpFrame->ei.hWndEdit, lpFrame->nTabStopSize, FALSE);
  DoViewWordWrap(lpFrame, lpFrame->ei.bWordWrap, TRUE);
  SetNewLineStatus(lpFrame, lpFrame->ei.nNewLine, AENL_INPUT);

  if (lpFrame->dwMarker)
  {
    SetMarker(lpFrame, lpFrame->dwMarker);
  }
  if (lpFrame->dwLineGap)
  {
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETLINEGAP, lpFrame->dwLineGap, 0);
  }
  if (lpFrame->nCaretWidth != 1)
  {
    POINT pt;

    SendMessage(lpFrame->ei.hWndEdit, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);
    pt.x=lpFrame->nCaretWidth;
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETCARETWIDTH, 0, (LPARAM)&pt);
  }
  if (lpFrame->bShowURL)
  {
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETDETECTURL, lpFrame->bShowURL, 0);

    if (lpFrame->bUrlPrefixesEnable)
      SetUrlPrefixes(lpFrame->ei.hWndEdit, lpFrame->wszUrlPrefixes);
    if (lpFrame->bUrlDelimitersEnable)
    {
      SendMessage(lpFrame->ei.hWndEdit, AEM_SETURLLEFTDELIMITERS, 0, (LPARAM)lpFrame->wszUrlLeftDelimiters);
      SendMessage(lpFrame->ei.hWndEdit, AEM_SETURLRIGHTDELIMITERS, 0, (LPARAM)lpFrame->wszUrlRightDelimiters);
    }
  }
  if (dwWordBreakDefault == (DWORD)-1)
    dwWordBreakDefault=SendMessage(lpFrame->ei.hWndEdit, AEM_GETWORDBREAK, 0, 0);
  if (lpFrame->bWordDelimitersEnable)
  {
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETWORDBREAK, moCur.dwWordBreakCustom, 0);
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)lpFrame->wszWordDelimiters);
  }
  if (lpFrame->bWrapDelimitersEnable)
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)lpFrame->wszWrapDelimiters);

  UpdateStatusUser(lpFrame, CSB_FONTPOINT|CSB_TABSIZE|CSB_MARKER);
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void ResizeEditWindow(FRAMEDATA *lpFrame, DWORD dwFlags)
{
  RECT *lprcEditWindow;

  if (nMDI == WMD_MDI)
    lprcEditWindow=&lpFrame->rcEditWindow;
  else
    lprcEditWindow=&fdInit.rcEditWindow;
  UpdateWindow(hStatus);

  if (lpFrame->ei.hWndMaster)
  {
    RECT rc;

    lpFrame->rcMasterWindow.left=lprcEditWindow->left;
    lpFrame->rcMasterWindow.top=lprcEditWindow->top;

    if (lpFrame->ei.hWndClone1)
    {
      lpFrame->rcMasterWindow.right=max(lpFrame->rcMasterWindow.right, 40);
      lpFrame->rcMasterWindow.right=min(lpFrame->rcMasterWindow.right, lprcEditWindow->right - 40);
      lpFrame->rcMasterWindow.right=max(lpFrame->rcMasterWindow.right, 0);
    }
    else lpFrame->rcMasterWindow.right=lprcEditWindow->right;

    if (lpFrame->ei.hWndClone2)
    {
      lpFrame->rcMasterWindow.bottom=max(lpFrame->rcMasterWindow.bottom, 40);
      lpFrame->rcMasterWindow.bottom=min(lpFrame->rcMasterWindow.bottom, lprcEditWindow->bottom - 40);
      lpFrame->rcMasterWindow.bottom=max(lpFrame->rcMasterWindow.bottom, 0);
    }
    else lpFrame->rcMasterWindow.bottom=lprcEditWindow->bottom;

    if (lpFrame->ei.hWndMaster)
    {
      rc=lpFrame->rcMasterWindow;

      if (!(dwFlags & REW_TEST))
      {
        MoveWindow(lpFrame->ei.hWndMaster, rc.left, rc.top, rc.right, rc.bottom, (dwFlags & REW_NOREDRAW)?FALSE:TRUE);
      }
      else
      {
        ClientToScreenRect(lpFrame->hWndEditParent, &rc);
        DrawMovingRect(&rc);
      }
    }
    if (lpFrame->ei.hWndClone1)
    {
      rc.left=lpFrame->rcMasterWindow.left + lpFrame->rcMasterWindow.right;
      rc.top=lpFrame->rcMasterWindow.top;
      rc.right=lprcEditWindow->right - lpFrame->rcMasterWindow.right;
      rc.bottom=lpFrame->rcMasterWindow.bottom;

      if (!(dwFlags & REW_TEST))
      {
        MoveWindow(lpFrame->ei.hWndClone1, rc.left, rc.top, rc.right, rc.bottom, (dwFlags & REW_NOREDRAW)?FALSE:TRUE);
      }
      else
      {
        ClientToScreenRect(lpFrame->hWndEditParent, &rc);
        DrawMovingRect(&rc);
      }
    }
    if (lpFrame->ei.hWndClone2)
    {
      rc.left=lpFrame->rcMasterWindow.left;
      rc.top=lpFrame->rcMasterWindow.top + lpFrame->rcMasterWindow.bottom;
      rc.right=lpFrame->rcMasterWindow.right;
      rc.bottom=lprcEditWindow->bottom - lpFrame->rcMasterWindow.bottom;

      if (!(dwFlags & REW_TEST))
      {
        MoveWindow(lpFrame->ei.hWndClone2, rc.left, rc.top, rc.right, rc.bottom, (dwFlags & REW_NOREDRAW)?FALSE:TRUE);
      }
      else
      {
        ClientToScreenRect(lpFrame->hWndEditParent, &rc);
        DrawMovingRect(&rc);
      }
    }
    if (lpFrame->ei.hWndClone3)
    {
      rc.left=lpFrame->rcMasterWindow.left + lpFrame->rcMasterWindow.right;
      rc.top=lpFrame->rcMasterWindow.top + lpFrame->rcMasterWindow.bottom;
      rc.right=lprcEditWindow->right - lpFrame->rcMasterWindow.right;
      rc.bottom=lprcEditWindow->bottom - lpFrame->rcMasterWindow.bottom;

      if (!(dwFlags & REW_TEST))
      {
        MoveWindow(lpFrame->ei.hWndClone3, rc.left, rc.top, rc.right, rc.bottom, (dwFlags & REW_NOREDRAW)?FALSE:TRUE);
      }
      else
      {
        ClientToScreenRect(lpFrame->hWndEditParent, &rc);
        DrawMovingRect(&rc);
      }
    }
  }
  else
  {
    if (!(dwFlags & REW_TEST))
    {
      if (lpFrame->ei.hWndEdit)
        MoveWindow(lpFrame->ei.hWndEdit, lprcEditWindow->left, lprcEditWindow->top, lprcEditWindow->right, lprcEditWindow->bottom, (dwFlags & REW_NOREDRAW)?FALSE:TRUE);
    }
  }

  //Remember current frame edit RECT
  if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
    lpFrame->rcEditWindow=fdInit.rcEditWindow;
}

FRAMEDATA* GetFrameDataFromEditWindow(HWND hWndEditCtrl)
{
  if (lpFrameCurrent->ei.hWndEdit)
  {
    if (!hWndEditCtrl || hWndEditCtrl == lpFrameCurrent->ei.hWndEdit)
      return lpFrameCurrent;
    if (lpFrameCurrent->ei.hWndMaster)
    {
      if (hWndEditCtrl == lpFrameCurrent->ei.hWndMaster ||
          hWndEditCtrl == lpFrameCurrent->ei.hWndClone1 ||
          hWndEditCtrl == lpFrameCurrent->ei.hWndClone2 ||
          hWndEditCtrl == lpFrameCurrent->ei.hWndClone3)
        return lpFrameCurrent;
    }
    if (nMDI == WMD_MDI)
    {
      HWND hWndParent;

      if (GetWindowLongWide(hWndEditCtrl, GWL_ID) == ID_EDIT)
      {
        if (hWndParent=GetParent(hWndEditCtrl))
          return (FRAMEDATA *)GetWindowLongWide(hWndParent, GWL_USERDATA);
      }
    }
  }
  return NULL;
}

FRAMEDATA* GetFrameDataFromEditDocument(AEHDOC hDocEditHandle)
{
  if (lpFrameCurrent->ei.hDocEdit)
  {
    if (!hDocEditHandle || hDocEditHandle == lpFrameCurrent->ei.hDocEdit)
      return lpFrameCurrent;
    if (lpFrameCurrent->ei.hDocMaster)
    {
      if (hDocEditHandle == lpFrameCurrent->ei.hDocMaster ||
          hDocEditHandle == lpFrameCurrent->ei.hDocClone1 ||
          hDocEditHandle == lpFrameCurrent->ei.hDocClone2 ||
          hDocEditHandle == lpFrameCurrent->ei.hDocClone3)
        return lpFrameCurrent;
    }
    return StackFrameGetByHandle(&hFramesStack, hDocEditHandle);
  }
  return NULL;
}

FRAMEDATA* CreateFrameData(HWND hWndEditParent, FRAMEDATA *lpFrameSource)
{
  FRAMEDATA *lpFrame;

  if (lpFrame=StackFrameInsert(&hFramesStack))
  {
    //Initialize settings
    CopyFrameData(lpFrame, lpFrameSource);
    lpFrame->hWndEditParent=hWndEditParent;
  }
  return lpFrame;
}

void CopyFrameData(FRAMEDATA *lpFrameTarget, FRAMEDATA *lpFrameSource)
{
  //Copy settings from lpFrameSource
  xmemcpy(((LPBYTE)lpFrameTarget) + sizeof(HSTACK), ((LPBYTE)lpFrameSource) + sizeof(HSTACK), sizeof(FRAMEDATA) - sizeof(HSTACK));

  //Initialize own settings
  lpFrameTarget->hWndEditParent=NULL;
  lpFrameTarget->ei.hWndEdit=NULL;
  lpFrameTarget->ei.hDocEdit=NULL;
  lpFrameTarget->ei.pFile=bOldWindows?(LPBYTE)lpFrameTarget->szFile:(LPBYTE)lpFrameTarget->wszFile;
  lpFrameTarget->ei.szFile=lpFrameTarget->szFile;
  lpFrameTarget->ei.wszFile=lpFrameTarget->wszFile;
  lpFrameTarget->ei.nCodePage=moCur.nDefaultCodePage;
  lpFrameTarget->ei.bBOM=bDefaultBOM;
  lpFrameTarget->ei.nNewLine=moCur.nDefaultNewLine;
  lpFrameTarget->ei.bModified=FALSE;
  lpFrameTarget->ei.bReadOnly=FALSE;
  //lpFrameTarget->ei.bWordWrap=lpFrameSource->ei.bWordWrap;
  lpFrameTarget->ei.bOvertypeMode=FALSE;
  lpFrameTarget->ei.hWndMaster=NULL;
  lpFrameTarget->ei.hDocMaster=NULL;
  lpFrameTarget->ei.hWndClone1=NULL;
  lpFrameTarget->ei.hDocClone1=NULL;
  lpFrameTarget->ei.hWndClone2=NULL;
  lpFrameTarget->ei.hDocClone2=NULL;
  lpFrameTarget->ei.hWndClone3=NULL;
  lpFrameTarget->ei.hDocClone3=NULL;
  lpFrameTarget->szFile[0]='\0';
  lpFrameTarget->wszFile[0]=L'\0';
  lpFrameTarget->nFileLen=0;
  lpFrameTarget->hIcon=hIconEmpty;
  //lpFrameTarget->rcEditWindow=lpFrameSource->rcEditWindow;
  //lpFrameTarget->rcMasterWindow=lpFrameSource->rcMasterWindow;

  lpFrameTarget->lpEditProc=NULL;
  lpFrameTarget->ft.dwLowDateTime=0;
  lpFrameTarget->ft.dwHighDateTime=0;
  lpFrameTarget->dwInputLocale=(DWORD)-1;

  //xmemcpy(&lpFrameTarget->lf, &lpFrameSource->lf, sizeof(LOGFONTW));
  //lpFrameTarget->aec=lpFrameSource->aec;
  //lpFrameTarget->dwEditMargins=lpFrameSource->dwEditMargins;
  //lpFrameTarget->nTabStopSize=lpFrameSource->nTabStopSize;
  //lpFrameTarget->bTabStopAsSpaces=lpFrameSource->bTabStopAsSpaces;
  //lpFrameTarget->nUndoLimit=lpFrameSource->nUndoLimit;
  //lpFrameTarget->bDetailedUndo=lpFrameSource->bDetailedUndo;
  //lpFrameTarget->dwWrapType=lpFrameSource->dwWrapType;
  //lpFrameTarget->dwWrapLimit=lpFrameSource->dwWrapLimit;
  //lpFrameTarget->dwMarker=lpFrameSource->dwMarker;
  //lpFrameTarget->dwMappedPrintWidth=lpFrameSource->dwMappedPrintWidth;
  //lpFrameTarget->dwCaretOptions=lpFrameSource->dwCaretOptions;
  //lpFrameTarget->nCaretWidth=lpFrameSource->nCaretWidth;
  //lpFrameTarget->dwMouseOptions=lpFrameSource->dwMouseOptions;
  //lpFrameTarget->dwLineGap=lpFrameSource->dwLineGap;
  //lpFrameTarget->bShowURL=lpFrameSource->bShowURL;
  //lpFrameTarget->bUrlPrefixesEnable=lpFrameSource->bUrlPrefixesEnable;
  //lpFrameTarget->bUrlDelimitersEnable=lpFrameSource->bUrlDelimitersEnable;
  //lpFrameTarget->bWordDelimitersEnable=lpFrameSource->bWordDelimitersEnable;
  //lpFrameTarget->bWrapDelimitersEnable=lpFrameSource->bWrapDelimitersEnable;
  //xmemcpy(&lpFrameTarget->wszUrlPrefixes, lpFrameSource->wszUrlPrefixes, sizeof(lpFrameTarget->wszUrlPrefixes));
  //xmemcpy(&lpFrameTarget->wszUrlLeftDelimiters, lpFrameSource->wszUrlLeftDelimiters, sizeof(lpFrameTarget->wszUrlLeftDelimiters));
  //xmemcpy(&lpFrameTarget->wszUrlRightDelimiters, lpFrameSource->wszUrlRightDelimiters, sizeof(lpFrameTarget->wszUrlRightDelimiters));
  //xmemcpy(&lpFrameTarget->wszWordDelimiters, lpFrameSource->wszWordDelimiters, sizeof(lpFrameTarget->wszWordDelimiters));
  //xmemcpy(&lpFrameTarget->wszWrapDelimiters, lpFrameSource->wszWrapDelimiters, sizeof(lpFrameTarget->wszWrapDelimiters));
}

void SaveFrameData(FRAMEDATA *lpFrame)
{
  if (nMDI)
  {
    //Remember keyboard layout
    if (moCur.bKeybLayoutMDI)
    {
      lpFrame->dwInputLocale=(DWORD)GetKeyboardLayout(0);
    }
  }
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void RestoreFrameData(FRAMEDATA *lpFrame, DWORD dwFlagsPMDI)
{
  if (nMDI == WMD_PMDI)
  {
    DWORD dwSetDataFlags=0;

    if (dwFlagsPMDI & FWA_NOVISUPDATE)
      dwSetDataFlags|=AESWD_NOALL;
    else if (dwFlagsPMDI & FWA_NOUPDATEEDIT)
      dwSetDataFlags|=AESWD_NOREDRAW;

    if (lpFrame->ei.hDocMaster)
    {
      SendMessage(lpFrame->ei.hWndMaster, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocMaster, dwSetDataFlags);
      if (lpFrame->ei.hDocClone1)
        SendMessage(lpFrame->ei.hWndClone1, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocClone1, dwSetDataFlags);
      if (lpFrame->ei.hDocClone2)
        SendMessage(lpFrame->ei.hWndClone2, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocClone2, dwSetDataFlags);
      if (lpFrame->ei.hDocClone3)
        SendMessage(lpFrame->ei.hWndClone3, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocClone3, dwSetDataFlags);
    }
    else if (lpFrame->ei.hDocEdit)
      SendMessage(lpFrame->ei.hWndEdit, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocEdit, dwSetDataFlags);

    //Edit window focus in other frame can be different
    if (GetFocus() != lpFrame->ei.hWndEdit)
      SetFocus(lpFrame->ei.hWndEdit);

    //If window size has been changed, update virtual window according to current window size
    if (xmemcmp(&lpFrame->rcEditWindow, &fdInit.rcEditWindow, sizeof(RECT)))
    {
      SendMessage(lpFrame->ei.hWndEdit, AEM_UPDATESIZE, 0, 0);
      lpFrame->rcEditWindow=fdInit.rcEditWindow;
    }

    if (!(dwFlagsPMDI & FWA_NOVISUPDATE))
    {
      SplitVisUpdate(lpFrame);
      ResizeEditWindow(lpFrame, (dwFlagsPMDI & FWA_NOUPDATEEDIT)?REW_NOREDRAW:0);
    }
  }
  //Update selection to set valid globals: crSel, ciCaret and nSelSubtract
  SetSelectionStatus(lpFrame->ei.hDocEdit, lpFrame->ei.hWndEdit, NULL, NULL);
  SetCodePageStatus(NULL, lpFrame->ei.nCodePage, lpFrame->ei.bBOM);
  SetNewLineStatus(NULL, lpFrame->ei.nNewLine, 0);
  SetModifyStatus(NULL, lpFrame->ei.bModified);
  SetOvertypeStatus(NULL, lpFrame->ei.bOvertypeMode);

  if (nMDI)
  {
    //Activate keyboard layout
    if (moCur.bKeybLayoutMDI)
    {
      ActivateKeyboard(lpFrame->dwInputLocale);
    }

    //Update tabs
    if (!bTabPressed)
    {
      int nTabItem;

      if ((nTabItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
      {
        SendMessage(hTab, TCM_SETCURSEL, nTabItem, 0);
        UpdateTabs(hTab);
      }
    }
  }
}

BOOL CreateMdiFrameWindow(RECT *rcRectMDI)
{
  BOOL bResult=FALSE;

  if (nMDI == WMD_MDI)
  {
    DWORD dwStyle;

    if (lpFrameCurrent->hWndEditParent) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
    if (bMdiMaximize == -1)
      dwStyle=moCur.dwMdiStyle;
    else
      dwStyle=!bMdiMaximize?0:WS_MAXIMIZE;

    if (lpFrameCurrent == &fdInit)
      lpFramePrevious=NULL;
    else
      lpFramePrevious=lpFrameCurrent;
    CreateMDIWindowWide(APP_MDI_CLASSW, L"", dwStyle, rcRectMDI?rcRectMDI->left:CW_USEDEFAULT, rcRectMDI?rcRectMDI->top:CW_USEDEFAULT, rcRectMDI?rcRectMDI->right:CW_USEDEFAULT, rcRectMDI?rcRectMDI->bottom:CW_USEDEFAULT, hMdiClient, hInstance, 0);
    bResult=TRUE;
  }
  else if (nMDI == WMD_PMDI)
  {
    FRAMEDATA *lpFrame;

    if (lpFrame=CreateFrameData(hMainWnd, lpFrameCurrent))
    {
      //Create virtual document. Procedure and window aren't changed.
      lpFrame->ei.hDocEdit=(AEHDOC)CreateEditWindow(lpFrame->hWndEditParent, fdInit.ei.hWndEdit);
      lpFrame->lpEditProc=fdInit.lpEditProc;
      lpFrame->ei.hWndEdit=fdInit.ei.hWndEdit;

      AddTabItem(hTab, lpFrame->hIcon, (LPARAM)lpFrame);
      ActivateMdiFrameWindow(lpFrame, 0);
      SetEditWindowSettings(lpFrameCurrent);
      SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrameCurrent->ei.hWndEdit, (LPARAM)lpFrameCurrent->ei.hDocEdit);

      bResult=TRUE;
    }
  }
  return bResult;
}

FRAMEDATA* ActivateMdiFrameWindow(FRAMEDATA *lpFrame, DWORD dwFlags)
{
  FRAMEDATA *lpFrameLostFocus=lpFrameCurrent;

  if (lpFrameCurrent != lpFrame)
  {
    if (lpFrameCurrent == &fdInit)
      lpFramePrevious=NULL;
    else
      lpFramePrevious=lpFrameCurrent;

    if (!(dwFlags & FWA_NOUPDATEORDER))
    {
      //Move item to the end of stack, to use access order later.
      StackFrameMove(&hFramesStack, lpFrame, -1);
    }

    if (nMDI == WMD_MDI)
    {
      SendMessage(hMdiClient, WM_MDIACTIVATE, (WPARAM)lpFrame->hWndEditParent, 0);
    }
    else if (nMDI == WMD_PMDI)
    {
      //Save deactivated frame data
      if (lpFrameCurrent->ei.hDocEdit)
        SaveFrameData(lpFrameCurrent);

      //Restore activated frame data
      lpFrameCurrent=lpFrame;
      RestoreFrameData(lpFrameCurrent, dwFlags);

      //Set caption of main window
      if (lpFrameCurrent->wszFile[0])
      {
        //xprintfW(wbuf, L"%s - %s", GetFileName(lpFrameCurrent->wszFile), APP_MAIN_TITLEW);
        xprintfW(wbuf, L"%s - [%s]", APP_MAIN_TITLEW, lpFrameCurrent->wszFile);
        SetWindowTextWide(hMainWnd, wbuf);
      }
      else SetWindowTextWide(hMainWnd, APP_MAIN_TITLEW);

      SendMessage(hMainWnd, AKDN_FRAME_ACTIVATE, (WPARAM)lpFrameCurrent, (LPARAM)NULL);
    }
  }
  return lpFrameLostFocus;
}

FRAMEDATA* NextMdiFrameWindow(FRAMEDATA *lpFrame, BOOL bPrev)
{
  if (moCur.dwTabOptionsMDI & TAB_SWITCH_RIGHTLEFT)
  {
    int nTabItem;
    int nItemCount;

    if ((nTabItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
    {
      nItemCount=SendMessage(hTab, TCM_GETITEMCOUNT, 0, 0) - 1;

      if (bPrev)
      {
        if (nTabItem - 1 < 0)
          SelectTabItem(hTab, nItemCount);
        else
          SelectTabItem(hTab, nTabItem - 1);
      }
      else
      {
        if (nTabItem + 1 > nItemCount)
          SelectTabItem(hTab, 0);
        else
          SelectTabItem(hTab, nTabItem + 1);
      }
    }
  }
  else if (moCur.dwTabOptionsMDI & TAB_SWITCH_NEXTPREV)
  {
    FRAMEDATA *lpFrameNext=StackFrameGetNext(&hFramesStack, lpFrame, bPrev);

    if (lpFrameNext != lpFrame)
      ActivateMdiFrameWindow(lpFrameNext, FWA_NOUPDATEORDER);
  }
  return lpFrameCurrent;
}

int DestroyMdiFrameWindow(FRAMEDATA *lpFrame)
{
  if (lpFrame->ei.hWndEdit)
  {
    if (nMDI == WMD_MDI)
    {
      if (SendMessage(hMdiClient, WM_MDIDESTROY, (WPARAM)lpFrame->hWndEditParent, 0))
        return FWDE_ABORT;
    }
    else if (nMDI == WMD_PMDI)
    {
      FRAMEDATA *lpFrameToActivate;
      int nTabItem;

      //Get previous frame
      if (lpFrame == lpFrameCurrent)
      {
        if (lpFramePrevious)
          lpFrameToActivate=lpFramePrevious;
        else
          lpFrameToActivate=StackFrameGetNext(&hFramesStack, lpFrame, TRUE);
      }
      else lpFrameToActivate=lpFrameCurrent;

      //Activate frame
      ActivateMdiFrameWindow(lpFrame, !lpFrame->ei.bModified?FWA_NOUPDATEEDIT:0);

      if (lpFrame == lpFrameToActivate)
      {
        //Don't destroy last tab
        if (!CloseDocument()) return FWDE_ABORT;
        SendMessage(hMainWnd, AKDN_FRAME_NOWINDOWS, 0, 0);
        return FWDE_LASTTAB;
      }
      else
      {
        //Ask if document unsaved
        if (!SaveChanged()) return FWDE_ABORT;
        RecentFilesSaveCurrentFile();
      }

      //Save closed frame settings
      if (!bMainOnFinish) CopyFrameData(&fdLast, lpFrame);

      if ((nTabItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
      {
        SendMessage(hMainWnd, AKDN_FRAME_DESTROY, (WPARAM)lpFrame, (LPARAM)NULL);

        //Destroy active document
        SplitDestroy(lpFrame, CN_CLONE1|CN_CLONE2|CN_CLONE3);
        SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrame->ei.hWndEdit, (LPARAM)lpFrame->ei.hDocEdit);
        SendMessage(lpFrame->ei.hWndEdit, AEM_SETDOCUMENT, (WPARAM)fdInit.ei.hDocEdit, AESWD_NOREDRAW);
        SendMessage(lpFrame->ei.hWndEdit, AEM_DELETEDOCUMENT, (WPARAM)lpFrame->ei.hDocEdit, 0);
        StackFrameDelete(&hFramesStack, lpFrame);

        //Remove tab item
        DeleteTabItem(hTab, nTabItem);

        //Activate previous window
        if (lpFrameToActivate)
          ActivateMdiFrameWindow(lpFrameToActivate, 0);
      }
    }
    return FWDE_SUCCESS;
  }
  return FWDE_NOWINDOW;
}

BOOL FrameNoWindows()
{
  if (nMDI == WMD_MDI)
  {
    if (!lpFrameCurrent->hWndEditParent)
      return TRUE;
  }
  else
  {
    if (lpFrameCurrent == (FRAMEDATA *)hFramesStack.first &&
        lpFrameCurrent == (FRAMEDATA *)hFramesStack.last &&
        !lpFrameCurrent->ei.bModified &&
        !lpFrameCurrent->ei.wszFile[0])
    {
      return TRUE;
    }
  }
  return FALSE;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SplitCreate(FRAMEDATA *lpFrame, DWORD dwFlags)
{
  if (!lpFrame->ei.hWndMaster)
  {
    lpFrame->ei.hWndMaster=lpFrame->ei.hWndEdit;
    lpFrame->ei.hDocMaster=lpFrame->ei.hDocEdit;
    if (!fdInit.ei.hWndMaster)
    {
      fdInit.ei.hWndMaster=fdInit.ei.hWndEdit;
      fdInit.ei.hDocMaster=fdInit.ei.hDocEdit;
    }

    if (dwFlags & CN_CLONE1)
    {
      if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
      {
        //Create real edit window for clone #1 if not created before
        if (!fdInit.ei.hWndClone1)
        {
          fdInit.ei.hWndClone1=(HWND)CreateEditWindow(hMainWnd, NULL);
          fdInit.ei.hDocClone1=(AEHDOC)SendMessage(fdInit.ei.hWndClone1, AEM_GETDOCUMENT, 0, 0);
        }
        lpFrame->ei.hWndClone1=fdInit.ei.hWndClone1;

        if (nMDI == WMD_SDI)
        {
          lpFrame->ei.hDocClone1=fdInit.ei.hDocClone1;
        }
        else if (nMDI == WMD_PMDI)
        {
          //Create and assign virtual document
          lpFrame->ei.hDocClone1=(AEHDOC)CreateEditWindow(lpFrame->hWndEditParent, fdInit.ei.hWndEdit);
          SendMessage(lpFrame->ei.hWndClone1, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocClone1, AESWD_NOREDRAW);
        }
      }
      else if (nMDI == WMD_MDI)
      {
        lpFrame->ei.hWndClone1=(HWND)CreateEditWindow(lpFrame->hWndEditParent, NULL);
        lpFrame->ei.hDocClone1=(AEHDOC)SendMessage(lpFrame->ei.hWndClone1, AEM_GETDOCUMENT, 0, 0);
      }
      SendMessage(lpFrame->ei.hWndMaster, AEM_ADDCLONE, (WPARAM)lpFrame->ei.hWndClone1, 0);
      SetMargins(lpFrame->ei.hWndClone1, lpFrame->dwEditMargins, 0);
      SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrame->ei.hWndClone1, (LPARAM)lpFrame->ei.hDocClone1);
    }
    if (dwFlags & CN_CLONE2)
    {
      if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
      {
        //Create real edit window for clone #2 if not created before
        if (!fdInit.ei.hWndClone2)
        {
          fdInit.ei.hWndClone2=(HWND)CreateEditWindow(hMainWnd, NULL);
          fdInit.ei.hDocClone2=(AEHDOC)SendMessage(fdInit.ei.hWndClone2, AEM_GETDOCUMENT, 0, 0);
        }
        lpFrame->ei.hWndClone2=fdInit.ei.hWndClone2;

        if (nMDI == WMD_SDI)
        {
          lpFrame->ei.hDocClone2=fdInit.ei.hDocClone2;
        }
        else if (nMDI == WMD_PMDI)
        {
          //Create and assign virtual document
          lpFrame->ei.hDocClone2=(AEHDOC)CreateEditWindow(lpFrame->hWndEditParent, fdInit.ei.hWndEdit);
          SendMessage(lpFrame->ei.hWndClone2, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocClone2, AESWD_NOREDRAW);
        }
      }
      else if (nMDI == WMD_MDI)
      {
        lpFrame->ei.hWndClone2=(HWND)CreateEditWindow(lpFrame->hWndEditParent, NULL);
        lpFrame->ei.hDocClone2=(AEHDOC)SendMessage(lpFrame->ei.hWndClone2, AEM_GETDOCUMENT, 0, 0);
      }
      SendMessage(lpFrame->ei.hWndMaster, AEM_ADDCLONE, (WPARAM)lpFrame->ei.hWndClone2, 0);
      SetMargins(lpFrame->ei.hWndClone2, lpFrame->dwEditMargins, 0);
      SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrame->ei.hWndClone2, (LPARAM)lpFrame->ei.hDocClone2);
    }
    if (dwFlags & CN_CLONE3)
    {
      if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
      {
        //Create real edit window for clone #3 if not created before
        if (!fdInit.ei.hWndClone3)
        {
          fdInit.ei.hWndClone3=(HWND)CreateEditWindow(hMainWnd, NULL);
          fdInit.ei.hDocClone3=(AEHDOC)SendMessage(fdInit.ei.hWndClone3, AEM_GETDOCUMENT, 0, 0);
        }
        lpFrame->ei.hWndClone3=fdInit.ei.hWndClone3;

        if (nMDI == WMD_SDI)
        {
          lpFrame->ei.hDocClone3=fdInit.ei.hDocClone3;
        }
        else if (nMDI == WMD_PMDI)
        {
          //Create and assign virtual document
          lpFrame->ei.hDocClone3=(AEHDOC)CreateEditWindow(lpFrame->hWndEditParent, fdInit.ei.hWndEdit);
          SendMessage(lpFrame->ei.hWndClone3, AEM_SETDOCUMENT, (WPARAM)lpFrame->ei.hDocClone3, AESWD_NOREDRAW);
        }
      }
      else if (nMDI == WMD_MDI)
      {
        lpFrame->ei.hWndClone3=(HWND)CreateEditWindow(lpFrame->hWndEditParent, NULL);
        lpFrame->ei.hDocClone3=(AEHDOC)SendMessage(lpFrame->ei.hWndClone3, AEM_GETDOCUMENT, 0, 0);
      }
      SendMessage(lpFrame->ei.hWndMaster, AEM_ADDCLONE, (WPARAM)lpFrame->ei.hWndClone3, 0);
      SetMargins(lpFrame->ei.hWndClone3, lpFrame->dwEditMargins, 0);
      SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrame->ei.hWndClone3, (LPARAM)lpFrame->ei.hDocClone3);
    }
    SplitVisUpdate(lpFrame);
  }
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SplitDestroy(FRAMEDATA *lpFrame, DWORD dwFlags)
{
  bEditOnFinish=TRUE;

  //AEM_DELCLONE is not necessary cause AkelEdit do all the job.
  if (lpFrame->ei.hWndMaster)
  {
    if (dwFlags & CN_CLONE1)
    {
      if (lpFrame->ei.hWndClone1)
      {
        if (lpFrame != &fdInit)
          SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrame->ei.hWndClone1, (LPARAM)lpFrame->ei.hDocClone1);

        if (nMDI == WMD_MDI || lpFrame == &fdInit)
        {
          DestroyWindow(lpFrame->ei.hWndClone1);
        }
        else if (nMDI == WMD_PMDI)
        {
          if (lpFrame->ei.hDocClone1 == (AEHDOC)SendMessage(lpFrame->ei.hWndClone1, AEM_GETDOCUMENT, 0, 0))
            SendMessage(lpFrame->ei.hWndClone1, AEM_SETDOCUMENT, (WPARAM)fdInit.ei.hDocClone1, AESWD_NOREDRAW);
          SendMessage(lpFrame->ei.hWndClone1, AEM_DELETEDOCUMENT, (WPARAM)lpFrame->ei.hDocClone1, 0);
        }

        if (lpFrame->ei.hWndEdit && lpFrame->ei.hWndClone1 == lpFrame->ei.hWndEdit)
        {
          lpFrame->ei.hWndEdit=lpFrame->ei.hWndMaster;
          lpFrame->ei.hDocEdit=lpFrame->ei.hDocMaster;
        }
        lpFrame->ei.hWndClone1=NULL;
        lpFrame->ei.hDocClone1=NULL;
      }
    }
    if (dwFlags & CN_CLONE2)
    {
      if (lpFrame->ei.hWndClone2)
      {
        if (lpFrame != &fdInit)
          SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrame->ei.hWndClone2, (LPARAM)lpFrame->ei.hDocClone2);

        if (nMDI == WMD_MDI || lpFrame == &fdInit)
        {
          DestroyWindow(lpFrame->ei.hWndClone2);
        }
        else if (nMDI == WMD_PMDI)
        {
          if (lpFrame->ei.hDocClone2 == (AEHDOC)SendMessage(lpFrame->ei.hWndClone2, AEM_GETDOCUMENT, 0, 0))
            SendMessage(lpFrame->ei.hWndClone2, AEM_SETDOCUMENT, (WPARAM)fdInit.ei.hDocClone2, AESWD_NOREDRAW);
          SendMessage(lpFrame->ei.hWndClone2, AEM_DELETEDOCUMENT, (WPARAM)lpFrame->ei.hDocClone2, 0);
        }

        if (lpFrame->ei.hWndEdit && lpFrame->ei.hWndClone2 == lpFrame->ei.hWndEdit)
        {
          lpFrame->ei.hWndEdit=lpFrame->ei.hWndMaster;
          lpFrame->ei.hDocEdit=lpFrame->ei.hDocMaster;
        }
        lpFrame->ei.hWndClone2=NULL;
        lpFrame->ei.hDocClone2=NULL;
      }
    }
    if (dwFlags & CN_CLONE3)
    {
      if (lpFrame->ei.hWndClone3)
      {
        if (lpFrame != &fdInit)
          SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrame->ei.hWndClone3, (LPARAM)lpFrame->ei.hDocClone3);

        if (nMDI == WMD_MDI || lpFrame == &fdInit)
        {
          DestroyWindow(lpFrame->ei.hWndClone3);
        }
        else if (nMDI == WMD_PMDI)
        {
          if (lpFrame->ei.hDocClone3 == (AEHDOC)SendMessage(lpFrame->ei.hWndClone3, AEM_GETDOCUMENT, 0, 0))
            SendMessage(lpFrame->ei.hWndClone3, AEM_SETDOCUMENT, (WPARAM)fdInit.ei.hDocClone3, AESWD_NOREDRAW);
          SendMessage(lpFrame->ei.hWndClone3, AEM_DELETEDOCUMENT, (WPARAM)lpFrame->ei.hDocClone3, 0);
        }

        if (lpFrame->ei.hWndEdit && lpFrame->ei.hWndClone3 == lpFrame->ei.hWndEdit)
        {
          lpFrame->ei.hWndEdit=lpFrame->ei.hWndMaster;
          lpFrame->ei.hDocEdit=lpFrame->ei.hDocMaster;
        }
        lpFrame->ei.hWndClone3=NULL;
        lpFrame->ei.hDocClone3=NULL;
      }
    }
    SplitVisUpdate(lpFrame);
    SetSelectionStatus(lpFrame->ei.hDocEdit, lpFrame->ei.hWndEdit, NULL, NULL);
  }
  bEditOnFinish=FALSE;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SplitVisUpdate(FRAMEDATA *lpFrame)
{
  if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
  {
    if (fdInit.ei.hWndMaster)
    {
      if (fdInit.ei.hWndClone1)
      {
        if (IsWindowVisible(fdInit.ei.hWndClone1) == !lpFrame->ei.hDocClone1)
          ShowWindow(fdInit.ei.hWndClone1, lpFrame->ei.hDocClone1?SW_SHOW:SW_HIDE);
      }
      if (fdInit.ei.hWndClone2)
      {
        if (IsWindowVisible(fdInit.ei.hWndClone2) == !lpFrame->ei.hDocClone2)
          ShowWindow(fdInit.ei.hWndClone2, lpFrame->ei.hDocClone2?SW_SHOW:SW_HIDE);
      }
      if (fdInit.ei.hWndClone3)
      {
        if (IsWindowVisible(fdInit.ei.hWndClone3) == !lpFrame->ei.hDocClone3)
          ShowWindow(fdInit.ei.hWndClone3, lpFrame->ei.hDocClone3?SW_SHOW:SW_HIDE);
      }
    }
  }
}

LRESULT SendToDoc(AEHDOC hDocEdit, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (lpFrameCurrent->ei.hDocEdit == hDocEdit)
    return SendMessage(lpFrameCurrent->ei.hWndEdit, uMsg, wParam, lParam);

  if (nMDI == WMD_PMDI)
  {
    AESENDMESSAGE sm;

    sm.hDoc=hDocEdit;
    sm.uMsg=uMsg;
    sm.wParam=wParam;
    sm.lParam=lParam;
    return SendMessage(fdInit.ei.hWndEdit, AEM_SENDMESSAGE, 0, (LPARAM)&sm);
  }
  else
  {
    HWND hWndEdit;

    hWndEdit=(HWND)SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETDOCUMENTWINDOW, (WPARAM)hDocEdit, 0);
    return SendMessage(hWndEdit, uMsg, wParam, lParam);
  }
}


//// Menu functions

BOOL DoFileNew()
{
  if (nMDI)
    return CreateMdiFrameWindow(NULL);
  else
    return CloseDocument();
}

BOOL CloseDocument()
{
  if (!SaveChanged()) return FALSE;
  RecentFilesSaveCurrentFile();
  SendMessage(hMainWnd, AKDN_EDIT_ONCLOSE, (WPARAM)lpFrameCurrent->ei.hWndEdit, (LPARAM)lpFrameCurrent->ei.hDocEdit);

  SetWindowTextWide(lpFrameCurrent->ei.hWndEdit, L"");
  lpFrameCurrent->szFile[0]='\0';
  lpFrameCurrent->wszFile[0]=L'\0';
  lpFrameCurrent->nFileLen=0;
  SetNewLineStatus(lpFrameCurrent, moCur.nDefaultNewLine, AENL_INPUT);
  SetModifyStatus(lpFrameCurrent, FALSE);
  SetCodePageStatus(lpFrameCurrent, moCur.nDefaultCodePage, bDefaultBOM);
  UpdateTitle(lpFrameCurrent, L"");

  return TRUE;
}

HWND DoFileNewWindow(DWORD dwAddFlags)
{
  STARTUPINFOW siW={0};
  PROCESS_INFORMATION pi={0};
  HWND hWnd=0;

  if (!GetModuleFileNameWide(hInstance, wbuf, MAX_PATH)) return 0;

  siW.cb=sizeof(STARTUPINFOW);
  siW.dwFlags=STARTF_USESHOWWINDOW|dwAddFlags;
  siW.wShowWindow=SW_SHOWNORMAL;

  if (CreateProcessWide(wbuf, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &siW, &pi))
  {
    WaitForInputIdle(pi.hProcess, INFINITE);
    EnumThreadWindows(pi.dwThreadId, EnumThreadWindowsProc, (LPARAM)&hWnd);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return hWnd;
  }
  return 0;
}

BOOL CALLBACK EnumThreadWindowsProc(HWND hwnd, LPARAM lParam)
{
  HWND *hWnd=(HWND *)lParam;

  if (bOldWindows)
  {
    GetClassNameA(hwnd, buf, BUFFER_SIZE);
    if (!lstrcmpA(buf, APP_MAIN_CLASSA))
    {
      *hWnd=hwnd;
      return FALSE;
    }
  }
  else
  {
    GetClassNameW(hwnd, wbuf, BUFFER_SIZE);
    if (!xstrcmpW(wbuf, APP_MAIN_CLASSW))
    {
      *hWnd=hwnd;
      return FALSE;
    }
  }
  return TRUE;
}

BOOL DoFileOpen()
{
  wchar_t *wszFileList;
  DIALOGCODEPAGE dc={-1, -1};
  BOOL bResult=FALSE;

  if (nMDI == WMD_SDI && !SaveChanged()) return FALSE;
  bSaveDlg=FALSE;

  //Open file dialog
  if (wszFileList=AllocWideStr(OPENFILELIST_SIZE))
  {
    OPENFILENAMEW ofnW={0};

    xstrcpynW(wszFileList, lpFrameCurrent->wszFile, MAX_PATH);
    ofnW.lStructSize    =sizeof(OPENFILENAMEW);
    ofnW.lCustData      =(LPARAM)&dc;
    ofnW.hwndOwner      =hMainWnd;
    ofnW.hInstance      =hLangLib;
    ofnW.lpstrFile      =wszFileList;
    ofnW.lpstrFilter    =wszFilter;
    ofnW.nFilterIndex   =2;
    ofnW.nMaxFile       =OPENFILELIST_SIZE;
    ofnW.lpstrInitialDir=FileExistsWide(moCur.wszLastDir)?moCur.wszLastDir:L"";
    ofnW.lpstrDefExt    =NULL;
    ofnW.Flags          =(nMDI?OFN_ALLOWMULTISELECT:0)|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ENABLEHOOK|OFN_ENABLETEMPLATE|OFN_ENABLESIZING|OFN_OVERWRITEPROMPT;
    ofnW.lpfnHook       =(LPOFNHOOKPROC)CodePageDlgProc;
    ofnW.lpTemplateName =MAKEINTRESOURCEW(IDD_OFN);

    bResult=GetOpenFileNameWide(&ofnW);

    if (bResult)
    {
      //GetCurrentDirectoryWide(MAX_PATH, moCur.wszLastDir);
      SetCurrentDirectoryWide(wszExeDir);

      if (!nMDI)
      {
        GetFileDir(wszFileList, moCur.wszLastDir, MAX_PATH);
        if (OpenDocument(NULL, wszFileList, dwOfnFlags, nOfnCodePage, bOfnBOM) < 0)
          bResult=FALSE;
      }
      else
      {
        wchar_t wszFile[MAX_PATH];
        wchar_t wszString[MAX_PATH];
        wchar_t *wpFile=wszFileList + lstrlenW(wszFileList) + 1;
        int nFiles;
        int nFileCount=0;

        if (*wpFile)
        {
          //Multiple files selected
          if (*(wpFile - 2) == '\\') *(wpFile - 2)='\0';
          xstrcpynW(moCur.wszLastDir, wszFileList, MAX_PATH);

          //Get files count
          if (moCur.bStatusBar)
          {
            LoadStringWide(hLangLib, STR_COUNT, wszString, MAX_PATH);
            xarraysizeW(wszFileList, &nFiles);

            //First element in array is directory.
            --nFiles;
          }

          do
          {
            if (IsPathFull(wpFile))
              xstrcpynW(wszFile, wpFile, MAX_PATH);  //.lnk target
            else
              xprintfW(wszFile, L"%s\\%s", wszFileList, wpFile);
            OpenDocument(NULL, wszFile, dwOfnFlags, nOfnCodePage, bOfnBOM);

            //Status update
            if (moCur.bStatusBar)
            {
              MSG msg;

              xprintfW(wbuf, wszString, ++nFileCount, nFiles);
              StatusBar_SetTextWide(hStatus, STATUS_MODIFY, wbuf);

              while (PeekMessageWide(&msg, hStatus, 0, 0, PM_REMOVE))
              {
                TranslateMessage(&msg);
                DispatchMessageWide(&msg);
              }
            }
          }
          while (*(wpFile+=lstrlenW(wpFile) + 1));

          if (moCur.bStatusBar)
            StatusBar_SetTextWide(hStatus, STATUS_MODIFY, L"");
        }
        else
        {
          //One file selected
          GetFileDir(wszFileList, moCur.wszLastDir, MAX_PATH);
          if (OpenDocument(NULL, wszFileList, dwOfnFlags, nOfnCodePage, bOfnBOM) < 0)
            bResult=FALSE;
        }
      }
    }
    FreeWideStr(wszFileList);
  }
  return bResult;
}

int DoFileReopenAs(DWORD dwFlags, int nCodePage, BOOL bBOM)
{
  int nResult=EOD_SUCCESS;
  int nAnswer=0;

  if (!lpFrameCurrent->wszFile[0])
  {
    SetCodePageStatus(lpFrameCurrent, (dwFlags & OD_ADT_DETECT_CODEPAGE)?lpFrameCurrent->ei.nCodePage:nCodePage, (dwFlags & OD_ADT_DETECT_BOM)?lpFrameCurrent->ei.bBOM:bBOM);
    return nResult;
  }

  if (lpFrameCurrent->ei.bModified)
  {
    LoadStringWide(hLangLib, MSG_FILE_WILL_BE_REOPENED, wbuf, BUFFER_SIZE);
    nAnswer=API_MessageBox(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION);
  }
  if (!lpFrameCurrent->ei.bModified || nAnswer == IDOK)
  {
    nResult=OpenDocument(NULL, lpFrameCurrent->wszFile, dwFlags|OD_REOPEN, nCodePage, bBOM);
  }
  return nResult;
}

BOOL DoFileSave()
{
  if (!lpFrameCurrent->ei.bModified && lpFrameCurrent->wszFile[0] && FileExistsWide(lpFrameCurrent->wszFile)) return TRUE;
  if (!lpFrameCurrent->wszFile[0]) return DoFileSaveAs(-1, -1);
  return !SaveDocument(NULL, lpFrameCurrent->wszFile, lpFrameCurrent->ei.nCodePage, lpFrameCurrent->ei.bBOM, SD_UPDATE);
}

BOOL SaveChanged()
{
  int nChoice;

  if (lpFrameCurrent->ei.bModified)
  {
    LoadStringWide(hLangLib, MSG_DOCUMENT_CHANGED, wbuf, BUFFER_SIZE);
    nChoice=API_MessageBox(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_YESNOCANCEL|MB_ICONEXCLAMATION);

    if (nChoice == IDYES)
    {
      if (!DoFileSave()) return FALSE;
    }
    else if (nChoice == IDCANCEL) return FALSE;
  }
  return TRUE;
}

BOOL DoFileSaveAs(int nDialogCodePage, BOOL bDialogBOM)
{
  wchar_t wszSaveFile[MAX_PATH];
  DIALOGCODEPAGE dc={nDialogCodePage, bDialogBOM};
  BOOL bResult;

  bSaveDlg=TRUE;

  //Save file dialog
  {
    OPENFILENAMEW ofnW={0};

    xstrcpynW(wszSaveFile, lpFrameCurrent->wszFile, MAX_PATH);
    ofnW.lStructSize    =sizeof(OPENFILENAMEW);
    ofnW.lCustData      =(LPARAM)&dc;
    ofnW.hwndOwner      =hMainWnd;
    ofnW.hInstance      =hLangLib;
    ofnW.lpstrFile      =wszSaveFile;
    ofnW.lpstrFilter    =wszFilter;
    ofnW.nFilterIndex   =2;
    ofnW.nMaxFile       =MAX_PATH;
    ofnW.lpstrInitialDir=moCur.wszLastDir;
    ofnW.lpstrDefExt    =moCur.wszDefaultSaveExt;
    ofnW.Flags          =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ENABLEHOOK|OFN_ENABLETEMPLATE|OFN_ENABLESIZING|OFN_OVERWRITEPROMPT;
    ofnW.lpfnHook       =(LPOFNHOOKPROC)CodePageDlgProc;
    ofnW.lpTemplateName =MAKEINTRESOURCEW(IDD_OFN);

    bResult=GetSaveFileNameWide(&ofnW);
  }

  if (bResult)
  {
    GetCurrentDirectoryWide(MAX_PATH, moCur.wszLastDir);
    SetCurrentDirectoryWide(wszExeDir);

    if (!SaveDocument(NULL, wszSaveFile, nOfnCodePage, bOfnBOM, SD_UPDATE))
      return TRUE;
  }
  return FALSE;
}

void DoFileSaveAllAs()
{
  if (bOldWindows)
    API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_SAVEALLAS), hMainWnd, (DLGPROC)SaveAllAsDlgProc);
  else
    API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_SAVEALLAS), hMainWnd, (DLGPROC)SaveAllAsDlgProc);
}

BOOL DoFilePageSetup(HWND hWndOwner)
{
  BOOL bResult;

  if (bOldWindows)
  {
    PAGESETUPDLGA psdPageA={0};

    psdPageA.lStructSize        =sizeof(PAGESETUPDLGA);
    psdPageA.hwndOwner          =hWndOwner;
    psdPageA.lpfnPageSetupHook  =PrintPageSetupDlgProc;
    psdPageA.Flags              =prninfo.dwPageFlags|PSD_MARGINS|PSD_ENABLEPAGESETUPHOOK;
    psdPageA.rtMargin           =prninfo.rtMargin;
    psdPageA.hDevMode           =prninfo.hDevMode;
    psdPageA.hDevNames          =prninfo.hDevNames;

    if (bResult=PageSetupDlgA(&psdPageA))
    {
      prninfo.rtMargin=psdPageA.rtMargin;
      moCur.rcPrintMargins=prninfo.rtMargin;
    }
    prninfo.hDevMode=psdPageA.hDevMode;
    prninfo.hDevNames=psdPageA.hDevNames;
  }
  else
  {
    PAGESETUPDLGW psdPageW={0};

    psdPageW.lStructSize        =sizeof(PAGESETUPDLGW);
    psdPageW.hwndOwner          =hWndOwner;
    psdPageW.lpfnPageSetupHook  =PrintPageSetupDlgProc;
    psdPageW.Flags              =prninfo.dwPageFlags|PSD_MARGINS|PSD_ENABLEPAGESETUPHOOK;
    psdPageW.rtMargin           =prninfo.rtMargin;
    psdPageW.hDevMode           =prninfo.hDevMode;
    psdPageW.hDevNames          =prninfo.hDevNames;

    if (bResult=PageSetupDlgW(&psdPageW))
    {
      prninfo.rtMargin=psdPageW.rtMargin;
      moCur.rcPrintMargins=prninfo.rtMargin;
    }
    prninfo.hDevMode=psdPageW.hDevMode;
    prninfo.hDevNames=psdPageW.hDevNames;
  }
  UpdateMappedPrintWidth(lpFrameCurrent);
  UpdateStatusUser(lpFrameCurrent, CSB_MARKER);
  return bResult;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
int DoFilePrint(FRAMEDATA *lpFrame, BOOL bSilent)
{
  int nResult=0;

  //Set print dialog settings
  if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
    prninfo.dwPrintFlags|=PD_NOSELECTION;
  else
    prninfo.dwPrintFlags&=~PD_NOSELECTION;

  //Choose printer
  if (bOldWindows)
  {
    if (!GetPrinterA(hMainWnd, &prninfo, bSilent))
      return 0;
  }
  else
  {
    if (!GetPrinterW(hMainWnd, &prninfo, bSilent))
      return 0;
  }

  if (prninfo.hDC)
  {
    prn.hPrinterDC=prninfo.hDC;
    prn.rcMargins=prninfo.rtMargin;

    nResult=PrintDocument(lpFrame->ei.hWndEdit, &prn, (bOldWindows?PRND_ANSI:0)|PRND_REALPRINT|(prninfo.dwPrintFlags & PD_SELECTION?PRND_SELECTION:PRND_ALLTEXT), 0);

    DeleteDC(prninfo.hDC);
    prninfo.hDC=NULL;
    prninfo.dwPrintFlags&=~PD_SELECTION;
    prninfo.dwPrintFlags&=~PD_PAGENUMS;
    prninfo.dwPrintFlags&=~PD_PRINTTOFILE;
  }
  UpdateMappedPrintWidth(lpFrame);
  UpdateStatusUser(lpFrame, CSB_MARKER);
  return nResult;
}

void DoFilePreview(HWND hWnd)
{
  if (bOldWindows)
  {
    hWndPreviewEdit=hWnd;
    API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_PRINTPREVIEW), hMainWnd, (DLGPROC)PreviewDlgProc);
    hWndPreviewEdit=NULL;
  }
  else
  {
    hWndPreviewEdit=hWnd;
    API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_PRINTPREVIEW), hMainWnd, (DLGPROC)PreviewDlgProc);
    hWndPreviewEdit=NULL;
  }
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
  if (IsReadOnly(hWnd)) return;

  SendMessage(hWnd, AEM_CUT, 0, 0);
}

void DoEditCopy(HWND hWnd)
{
  SendMessage(hWnd, AEM_COPY, 0, 0);
}

BOOL DoEditPaste(HWND hWnd, BOOL bAnsi)
{
  if (IsReadOnly(hWnd)) return FALSE;

  return SendMessage(hWnd, AEM_PASTE, 0, bAnsi);
}

void DoEditClear(HWND hWnd)
{
  if (IsReadOnly(hWnd)) return;

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

void DoEditInsertDate(HWND hWnd)
{
  wchar_t wszTime[128];
  wchar_t wszDate[128];
  wchar_t wszTimeAndDate[MAX_PATH];

  if (IsReadOnly(hWnd)) return;

  if (!moCur.wszDateInsertFormat[0])
  {
    GetTimeFormatWide(LOCALE_USER_DEFAULT, TIME_NOSECONDS, NULL, NULL, wszTime, 128);
    GetDateFormatWide(LOCALE_USER_DEFAULT, 0, NULL, NULL, wszDate, 128);
    xprintfW(wszTimeAndDate, L"%s %s", wszTime, wszDate);
  }
  else
  {
    GetTimeFormatWide(LOCALE_USER_DEFAULT, 0, NULL, moCur.wszDateInsertFormat, wbuf, 128);
    GetDateFormatWide(LOCALE_USER_DEFAULT, 0, NULL, wbuf, wszTimeAndDate, 128);
  }
  ReplaceSelW(hWnd, wszTimeAndDate, -1, -1, NULL, NULL);
}

void DoEditRecode(HWND hWnd)
{
  if (!hDlgModeless)
  {
    nModelessType=MLT_RECODE;

    if (bOldWindows)
      hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_RECODE), hMainWnd, (DLGPROC)RecodeDlgProc);
    else
      hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_RECODE), hMainWnd, (DLGPROC)RecodeDlgProc);

    if (hDlgModeless)
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
  BOOL bColumnSel;
  BOOL bResult=FALSE;

  if (!(nAction & STRSEL_CHECK) && IsReadOnly(hWnd)) return FALSE;

  if (crRange.ciMin.nLine != crRange.ciMax.nLine)
  {
    if (nAction & STRSEL_CHECK) return TRUE;

    if (!(bColumnSel=SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0)))
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEBEGIN, (LPARAM)&crRange.ciMin);
      if (!AEC_IsFirstCharInLine(&crRange.ciMax))
      {
        SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEEND, (LPARAM)&crRange.ciMax);
        //SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTLINE, (LPARAM)&crRange.ciMax);
      }
    }
    crInitialSel=crRange;
    if (!AEC_IndexCompare(&crSel.ciMin, &ciCaret))
      ciInitialCaret=crInitialSel.ciMin;
    else
      ciInitialCaret=crInitialSel.ciMax;

    if (nRangeLen=IndexSubtract(hWnd, &crRange.ciMax, &crRange.ciMin, AELB_ASIS, bColumnSel))
    {
      nStringLen=lstrlenW(wpString);
      nStringBytes=nStringLen * sizeof(wchar_t);

      nFirstLine=SaveLineScroll(hWnd);
      SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
      if (!bColumnSel)
        SetSel(hWnd, &crRange, 0, NULL);

      if (nAction & STRSEL_INSERT)
      {
        nStringLenAll=(crRange.ciMax.nLine - crRange.ciMin.nLine + 1) * nStringLen;
        nBufferLen=nRangeLen + nStringLenAll;

        if (wszRange=AllocWideStr(nBufferLen + 1))
        {
          tr.cr=crRange;
          tr.bColumnSel=bColumnSel;
          tr.pBuffer=wszRange + nStringLenAll;
          tr.dwBufferMax=(DWORD)-1;
          tr.nNewLine=AELB_ASIS;
          tr.bFillSpaces=TRUE;
          SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);
          b=nStringLenAll;

          xmemcpy(wszRange, wpString, nStringBytes);
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

            if (b < nBufferLen || bColumnSel)
            {
              xmemcpy(wszRange + a, wpString, nStringBytes);
              a+=nStringLen;
            }
          }
          wszRange[a]='\0';
          bResult=TRUE;
        }
      }
      else if (nAction & STRSEL_DELETE)
      {
        if (wszRange=AllocWideStr(nRangeLen + 1))
        {
          tr.cr=crRange;
          tr.bColumnSel=bColumnSel;
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
              for (i=0; i < lpFrameCurrent->nTabStopSize && wszRange[b] == ' '; ++i, ++b);
          }
          else if (nAction & STRSEL_SPACE)
          {
            if (wszRange[b] == ' ' || wszRange[b] == '\t')
              ++b;
          }
          else
          {
            if (!xmemcmp(wszRange + b, wpString, nStringBytes))
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
                  for (i=0; i < lpFrameCurrent->nTabStopSize && wszRange[b] == ' '; ++i, ++b);
              }
              else if (nAction & STRSEL_SPACE)
              {
                if (wszRange[b] == ' ' || wszRange[b] == '\t')
                  ++b;
              }
              else
              {
                if (!xmemcmp(wszRange + b, wpString, nStringBytes))
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
        ReplaceSelW(hWnd, wszRange, a, bColumnSel, &crRange.ciMin, &crRange.ciMax);

        //Update selection
        if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
          SetSel(hWnd, &crRange, bColumnSel?AESELT_COLUMNON:0, &crRange.ciMin);
        else
          SetSel(hWnd, &crRange, bColumnSel?AESELT_COLUMNON:0, &crRange.ciMax);
      }
      SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
      InvalidateRect(hWnd, NULL, TRUE);
      RestoreLineScroll(hWnd, nFirstLine);

      FreeWideStr(wszRange);
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

  if (IsReadOnly(hWnd)) return FALSE;

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

    nFirstLine=SaveLineScroll(hWnd);
    SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
    ReplaceSelW(hWnd, wszRange, a, -1, &crRange.ciMin, &crRange.ciMax);

    //Update selection
    if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMin);
    else
      SetSel(hWnd, &crRange, AESELT_COLUMNASIS, &crRange.ciMax);

    SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWnd, NULL, TRUE);
    RestoreLineScroll(hWnd, nFirstLine);

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

  if (IsReadOnly(hWnd)) return FALSE;

  nFirstLine=SaveLineScroll(hWnd);
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
  RestoreLineScroll(hWnd, nFirstLine);

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
  int i;

  if (IsReadOnly(hWnd)) return FALSE;

  nFirstLine=SaveLineScroll(hWnd);
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
      for (i=0; i < nRangeLen; ++i)
        wszRange[i]=WideCharUpper(wszRange[i]);
    }
    else if (nCase == LOWERCASE)
    {
      for (i=0; i < nRangeLen; ++i)
        wszRange[i]=WideCharLower(wszRange[i]);
    }
    else if (nCase == SENTENCECASE)
    {
      while (wpStart < wpEnd)
      {
        while (wpStart < wpEnd && (AKD_wcschr(lpFrameCurrent->wszWordDelimiters, *wpStart) || AKD_wcschr(STR_SENTENCE_DELIMITERSW, *wpStart)))
          ++wpStart;

        if (wpStart < wpEnd) *wpStart++=WideCharUpper(*wpStart);

        while (wpStart < wpEnd && !AKD_wcschr(STR_SENTENCE_DELIMITERSW, *wpStart))
          *wpStart++=WideCharLower(*wpStart);
      }
    }
    else if (nCase == TITLECASE)
    {
      while (wpStart < wpEnd)
      {
        while (wpStart < wpEnd && AKD_wcschr(lpFrameCurrent->wszWordDelimiters, *wpStart))
          ++wpStart;

        if (wpStart < wpEnd) *wpStart++=WideCharUpper(*wpStart);

        while (wpStart < wpEnd && !AKD_wcschr(lpFrameCurrent->wszWordDelimiters, *wpStart))
          *wpStart++=WideCharLower(*wpStart);
      }
    }
    else if (nCase == INVERTCASE)
    {
      while (wpStart < wpEnd)
      {
        if (WideCharLower(*wpStart) == *wpStart)
          *wpStart++=WideCharUpper(*wpStart);
        else
          *wpStart++=WideCharLower(*wpStart);
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
  RestoreLineScroll(hWnd, nFirstLine);

  return bResult;
}

void DoEditFind()
{
  if (!hDlgModeless)
  {
    nModelessType=MLT_FIND;

    if (!nMDI)
      moCur.dwSearchOptions&=~AEFR_ALLFILES;
    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      moCur.dwSearchOptions&=~AEFR_SELECTION;
    else if (moCur.dwSearchOptions & AEFR_CHECKINSELIFSEL)
      moCur.dwSearchOptions=(moCur.dwSearchOptions & ~AEFR_BEGINNING & ~AEFR_ALLFILES) | AEFR_SELECTION;

    if (bOldWindows)
      hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_FIND), hMainWnd, (DLGPROC)FindAndReplaceDlgProc);
    else
      hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_FIND), hMainWnd, (DLGPROC)FindAndReplaceDlgProc);

    if (hDlgModeless)
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditFindNextDown(HWND hWnd)
{
  DWORD dwFlags=(moCur.dwSearchOptions & ~AEFR_UP & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_DOWN;

  if (wszFindText)
    FindTextW(hWnd, dwFlags, wszFindText, nFindTextLen);
  else
    DoEditFind();
}

void DoEditFindNextUp(HWND hWnd)
{
  DWORD dwFlags=(moCur.dwSearchOptions & ~AEFR_DOWN & ~AEFR_BEGINNING & ~AEFR_SELECTION & ~AEFR_ALLFILES) | AEFR_UP;

  if (wszFindText)
    FindTextW(hWnd, dwFlags, wszFindText, nFindTextLen);
  else
    DoEditFind();
}

void DoEditReplace()
{
  if (!hDlgModeless)
  {
    nModelessType=MLT_REPLACE;

    if (!nMDI)
      moCur.dwSearchOptions&=~AEFR_ALLFILES;
    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
      moCur.dwSearchOptions&=~AEFR_SELECTION;
    else if (moCur.dwSearchOptions & AEFR_CHECKINSELIFSEL)
      moCur.dwSearchOptions=(moCur.dwSearchOptions & ~AEFR_BEGINNING & ~AEFR_ALLFILES) | AEFR_SELECTION;

    if (bOldWindows)
      hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_REPLACE), hMainWnd, (DLGPROC)FindAndReplaceDlgProc);
    else
      hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_REPLACE), hMainWnd, (DLGPROC)FindAndReplaceDlgProc);

    if (hDlgModeless)
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

void DoEditGoTo()
{
  if (!hDlgModeless)
  {
    nModelessType=MLT_GOTO;

    if (bOldWindows)
      hDlgModeless=API_CreateDialogA(hLangLib, MAKEINTRESOURCEA(IDD_GOTO), hMainWnd, (DLGPROC)GoToDlgProc);
    else
      hDlgModeless=API_CreateDialogW(hLangLib, MAKEINTRESOURCEW(IDD_GOTO), hMainWnd, (DLGPROC)GoToDlgProc);

    if (hDlgModeless)
    {
      ShowWindow(hDlgModeless, SW_SHOW);
    }
  }
}

BOOL DoViewFont(HWND hWndOwner, LOGFONTW *lfFont)
{
  if (bOldWindows)
  {
    CHOOSEFONTA cfA={0};
    LOGFONTA lfTmpA;

    LogFontWtoA(lfFont, &lfTmpA);
    cfA.lStructSize=sizeof(CHOOSEFONTA);
    cfA.Flags      =CF_FORCEFONTEXIST|CF_INITTOLOGFONTSTRUCT|CF_SCREENFONTS;
    cfA.hwndOwner  =hWndOwner;
    cfA.lpLogFont  =&lfTmpA;
    if (!ChooseFontA(&cfA))
      return FALSE;
    LogFontAtoW(&lfTmpA, lfFont);
  }
  else
  {
    CHOOSEFONTW cfW={0};
    LOGFONTW lfTmpW;

    xmemcpy(&lfTmpW, lfFont, sizeof(LOGFONTW));
    cfW.lStructSize=sizeof(CHOOSEFONTW);
    cfW.Flags      =CF_FORCEFONTEXIST|CF_INITTOLOGFONTSTRUCT|CF_SCREENFONTS;
    cfW.hwndOwner  =hWndOwner;
    cfW.lpLogFont  =&lfTmpW;
    if (!ChooseFontW(&cfW))
      return FALSE;
    xmemcpy(lfFont, &lfTmpW, sizeof(LOGFONTW));
  }
  lfFont->lfHeight=-mod(lfFont->lfHeight);
  return TRUE;
}

void DoViewColors()
{
  if (bOldWindows)
    API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_COLORS), hMainWnd, (DLGPROC)ColorsDlgProc);
  else
    API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_COLORS), hMainWnd, (DLGPROC)ColorsDlgProc);
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void DoViewFontSize(FRAMEDATA *lpFrame, int nAction)
{
  if (nAction == INCREASE_FONT)
  {
    if (lpFrame->lf.lfHeight <= -1)
    {
      lpFrame->lf.lfHeight-=1;
      SetChosenFont(lpFrame->ei.hWndEdit, &lpFrame->lf);
      UpdateMappedPrintWidth(lpFrame);
      UpdateStatusUser(lpFrame, CSB_FONTPOINT|CSB_MARKER);
    }
  }
  else if (nAction == DECREASE_FONT)
  {
    if (lpFrame->lf.lfHeight < -1)
    {
      lpFrame->lf.lfHeight+=1;
      SetChosenFont(lpFrame->ei.hWndEdit, &lpFrame->lf);
      UpdateMappedPrintWidth(lpFrame);
      UpdateStatusUser(lpFrame, CSB_FONTPOINT|CSB_MARKER);
    }
  }
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void DoViewReadOnly(FRAMEDATA *lpFrame, BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_READONLY, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == lpFrame->ei.bReadOnly) return;
  lpFrame->ei.bReadOnly=bState;

  SendMessage(lpFrame->ei.hWndEdit, AEM_SETOPTIONS, lpFrame->ei.bReadOnly?AECOOP_OR:AECOOP_XOR, AECO_READONLY);
  SetModifyStatus(lpFrame, lpFrame->ei.bModified);
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void DoViewWordWrap(FRAMEDATA *lpFrame, BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_WORDWRAP, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == lpFrame->ei.bWordWrap) return;
  lpFrame->ei.bWordWrap=bState;

  if (lpFrame->ei.bWordWrap)
  {
    UpdateShowHScroll(lpFrame);
    SetWordWrap(lpFrame, lpFrame->dwWrapType, lpFrame->dwWrapLimit);
  }
  else
  {
    SetWordWrap(lpFrame, 0, 0);
    UpdateShowHScroll(lpFrame);
  }
}

void DoViewSplitWindow(BOOL bState, WPARAM wParam)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_SPLIT_WINDOW_ALL, bState?MF_CHECKED:MF_UNCHECKED);

  if (bState)
  {
    RECT *lprcEditWindow;

    if (wParam == IDM_VIEW_SPLIT_WINDOW_WE)
    {
      SplitCreate(lpFrameCurrent, CN_CLONE1);
    }
    else if (wParam == IDM_VIEW_SPLIT_WINDOW_NS)
    {
      SplitCreate(lpFrameCurrent, CN_CLONE2);
    }
    if (wParam == IDM_VIEW_SPLIT_WINDOW_ALL)
    {
      SplitCreate(lpFrameCurrent, CN_CLONE1|CN_CLONE2|CN_CLONE3);
    }

    if (nMDI == WMD_MDI)
      lprcEditWindow=&lpFrameCurrent->rcEditWindow;
    else
      lprcEditWindow=&fdInit.rcEditWindow;
    lpFrameCurrent->rcMasterWindow.left=lprcEditWindow->left;
    lpFrameCurrent->rcMasterWindow.top=lprcEditWindow->top;
    lpFrameCurrent->rcMasterWindow.right=lprcEditWindow->right / 2;
    lpFrameCurrent->rcMasterWindow.bottom=lprcEditWindow->bottom / 2;
  }
  else
  {
    //Destroy
    lpFrameCurrent->ei.hWndEdit=lpFrameCurrent->ei.hWndMaster;
    lpFrameCurrent->ei.hDocEdit=lpFrameCurrent->ei.hDocMaster;
    SplitDestroy(lpFrameCurrent, CN_CLONE1|CN_CLONE2|CN_CLONE3);
    lpFrameCurrent->ei.hWndMaster=NULL;
    lpFrameCurrent->ei.hDocMaster=NULL;
    SetFocus(lpFrameCurrent->ei.hWndEdit);
  }

  if (lpFrameCurrent->ei.bWordWrap)
    UpdateShowHScroll(lpFrameCurrent);
  ResizeEditWindow(lpFrameCurrent, 0);
}

void DoViewOnTop(BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_ONTOP, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == moCur.bOnTop) return;
  moCur.bOnTop=bState;

  if (moCur.bOnTop)
    SetWindowPos(hMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
  else
    SetWindowPos(hMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

void DoViewShowStatusBar(BOOL bState, BOOL bFirst)
{
  CheckMenuItem(hMainMenu, IDM_VIEW_SHOW_STATUSBAR, bState?MF_CHECKED:MF_UNCHECKED);
  if (bFirst != TRUE && bState == moCur.bStatusBar) return;
  moCur.bStatusBar=bState;

  ShowWindow(hStatus, moCur.bStatusBar?SW_SHOW:SW_HIDE);
  UpdateSize();
}

BOOL DoSettingsExec()
{
  STARTUPINFOW siW={0};
  PROCESS_INFORMATION pi={0};
  wchar_t *wszCommandExp;
  wchar_t *wszWorkDirExp;
  int nCommandLen;
  int nWorkDirLen;
  BOOL bResult=FALSE;

  nCommandLen=TranslateFileString(moCur.wszCommand, NULL, 0);
  nWorkDirLen=TranslateFileString(moCur.wszWorkDir, NULL, 0);

  if (wszCommandExp=AllocWideStr(nCommandLen + 1))
  {
    if (wszWorkDirExp=AllocWideStr(nWorkDirLen + 1))
    {
      TranslateFileString(moCur.wszCommand, wszCommandExp, nCommandLen + 1);
      TranslateFileString(moCur.wszWorkDir, wszWorkDirExp, nWorkDirLen + 1);

      if (*wszCommandExp)
      {
        siW.cb=sizeof(STARTUPINFOW);
        if (CreateProcessWide(NULL, wszCommandExp, NULL, NULL, FALSE, 0, NULL, *wszWorkDirExp?wszWorkDirExp:NULL, &siW, &pi))
        {
          bResult=TRUE;
          CloseHandle(pi.hProcess);
          CloseHandle(pi.hThread);
        }
        else
        {
          LoadStringWide(hLangLib, MSG_ERROR_RUN, wbuf, BUFFER_SIZE);
          API_MessageBox(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
        }
      }
      FreeWideStr(wszWorkDirExp);
    }
    FreeWideStr(wszCommandExp);
  }
  return bResult;
}

void DoSettingsSaveTime(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_SAVETIME, bState?MF_CHECKED:MF_UNCHECKED);
  moCur.bSaveTime=bState;
}

void DoSettingsKeepSpace(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_KEEPSPACE, bState?MF_CHECKED:MF_UNCHECKED);
  moCur.bKeepSpace=bState;
}

void DoSettingsWatchFile(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_WATCHFILE, bState?MF_CHECKED:MF_UNCHECKED);
  moCur.bWatchFile=bState;
  if (moCur.bWatchFile) SetFocus(hMainWnd);
}

void DoSettingsSingleOpenFile(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_SINGLEOPEN_FILE, bState?MF_CHECKED:MF_UNCHECKED);
  moCur.bSingleOpenFile=bState;
}

void DoSettingsSingleOpenProgram(BOOL bState)
{
  CheckMenuItem(hMainMenu, IDM_OPTIONS_SINGLEOPEN_PROGRAM, bState?MF_CHECKED:MF_UNCHECKED);
  moCur.bSingleOpenProgram=bState;
}

void DoSettingsPlugins(HWND hWnd)
{
  if (bOldWindows)
    API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_PLUGINS), hMainWnd, (DLGPROC)PluginsDlgProc);
  else
    API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_PLUGINS), hMainWnd, (DLGPROC)PluginsDlgProc);
}

void DoSettingsOptions()
{
  hPropertyHook=SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());
  bOptionsSave=FALSE;
  bOptionsRestart=FALSE;

  if (bOldWindows)
  {
    PROPSHEETHEADERA pshA={0};
    PROPSHEETPAGEA pspA[5]={0};

    pspA[0].dwSize      =sizeof(PROPSHEETPAGEA);
    pspA[0].dwFlags     =PSP_DEFAULT;
    pspA[0].hInstance   =hLangLib;
    pspA[0].pszTemplate =MAKEINTRESOURCEA(IDD_OPTIONS_GENERAL);
    pspA[0].pfnDlgProc  =(DLGPROC)OptionsGeneralDlgProc;
    pspA[1].dwSize      =sizeof(PROPSHEETPAGEA);
    pspA[1].dwFlags     =PSP_DEFAULT;
    pspA[1].hInstance   =hLangLib;
    pspA[1].pszTemplate =MAKEINTRESOURCEA(IDD_OPTIONS_REGISTRY);
    pspA[1].pfnDlgProc  =(DLGPROC)OptionsRegistryDlgProc;
    pspA[2].dwSize      =sizeof(PROPSHEETPAGEA);
    pspA[2].dwFlags     =PSP_DEFAULT;
    pspA[2].hInstance   =hLangLib;
    pspA[2].pszTemplate =MAKEINTRESOURCEA(IDD_OPTIONS_EDITOR1);
    pspA[2].pfnDlgProc  =(DLGPROC)OptionsEditor1DlgProc;
    pspA[3].dwSize      =sizeof(PROPSHEETPAGEA);
    pspA[3].dwFlags     =PSP_DEFAULT;
    pspA[3].hInstance   =hLangLib;
    pspA[3].pszTemplate =MAKEINTRESOURCEA(IDD_OPTIONS_EDITOR2);
    pspA[3].pfnDlgProc  =(DLGPROC)OptionsEditor2DlgProc;
    pspA[4].dwSize      =sizeof(PROPSHEETPAGEA);
    pspA[4].dwFlags     =PSP_DEFAULT;
    pspA[4].hInstance   =hLangLib;
    pspA[4].pszTemplate =MAKEINTRESOURCEA(IDD_OPTIONS_ADVANCED);
    pspA[4].pfnDlgProc  =(DLGPROC)OptionsAdvancedDlgProc;

    API_LoadStringA(hLangLib, STR_OPTIONS, buf, BUFFER_SIZE);
    pshA.pszCaption  =buf;
    pshA.dwSize      =(bOldComctl32)?(PROPSHEETHEADERA_V1_SIZE):(sizeof(PROPSHEETHEADERA));
    pshA.dwFlags     =PSH_PROPSHEETPAGE|PSH_NOAPPLYNOW|PSH_USEICONID|PSH_USECALLBACK;
    pshA.hwndParent  =hMainWnd;
    pshA.hInstance   =hLangLib;
    pshA.pszIcon     =MAKEINTRESOURCEA(IDI_ICON_MAIN);
    pshA.nPages      =sizeof(pspA) / sizeof(PROPSHEETPAGEA);
    pshA.nStartPage  =nPropertyStartPage;
    pshA.ppsp        =&pspA[0];
    pshA.pfnCallback =PropSheetProc;

    PropertySheetA(&pshA);
  }
  else
  {
    PROPSHEETHEADERW pshW={0};
    PROPSHEETPAGEW pspW[5]={0};

    pspW[0].dwSize      =sizeof(PROPSHEETPAGEW);
    pspW[0].dwFlags     =PSP_DEFAULT;
    pspW[0].hInstance   =hLangLib;
    pspW[0].pszTemplate =MAKEINTRESOURCEW(IDD_OPTIONS_GENERAL);
    pspW[0].pfnDlgProc  =(DLGPROC)OptionsGeneralDlgProc;
    pspW[1].dwSize      =sizeof(PROPSHEETPAGEW);
    pspW[1].dwFlags     =PSP_DEFAULT;
    pspW[1].hInstance   =hLangLib;
    pspW[1].pszTemplate =MAKEINTRESOURCEW(IDD_OPTIONS_REGISTRY);
    pspW[1].pfnDlgProc  =(DLGPROC)OptionsRegistryDlgProc;
    pspW[2].dwSize      =sizeof(PROPSHEETPAGEW);
    pspW[2].dwFlags     =PSP_DEFAULT;
    pspW[2].hInstance   =hLangLib;
    pspW[2].pszTemplate =MAKEINTRESOURCEW(IDD_OPTIONS_EDITOR1);
    pspW[2].pfnDlgProc  =(DLGPROC)OptionsEditor1DlgProc;
    pspW[3].dwSize      =sizeof(PROPSHEETPAGEW);
    pspW[3].dwFlags     =PSP_DEFAULT;
    pspW[3].hInstance   =hLangLib;
    pspW[3].pszTemplate =MAKEINTRESOURCEW(IDD_OPTIONS_EDITOR2);
    pspW[3].pfnDlgProc  =(DLGPROC)OptionsEditor2DlgProc;
    pspW[4].dwSize      =sizeof(PROPSHEETPAGEW);
    pspW[4].dwFlags     =PSP_DEFAULT;
    pspW[4].hInstance   =hLangLib;
    pspW[4].pszTemplate =MAKEINTRESOURCEW(IDD_OPTIONS_ADVANCED);
    pspW[4].pfnDlgProc  =(DLGPROC)OptionsAdvancedDlgProc;

    LoadStringWide(hLangLib, STR_OPTIONS, wbuf, BUFFER_SIZE);
    pshW.pszCaption=wbuf;
    pshW.dwSize      =sizeof(PROPSHEETHEADERW);
    pshW.dwFlags     =PSH_PROPSHEETPAGE|PSH_NOAPPLYNOW|PSH_USEICONID|PSH_USECALLBACK;
    pshW.hwndParent  =hMainWnd;
    pshW.hInstance   =hLangLib;
    pshW.pszIcon     =MAKEINTRESOURCEW(IDI_ICON_MAIN);
    pshW.nPages      =sizeof(pspW) / sizeof(PROPSHEETPAGEW);
    pshW.nStartPage  =nPropertyStartPage;
    pshW.ppsp        =&pspW[0];
    pshW.pfnCallback =PropSheetProc;

    PropertySheetW(&pshW);
  }

  if (bOptionsSave)
  {
    //Save to INI
    SaveOptions(&moCur, lpFrameCurrent, SS_INI, TRUE);
    SaveThemes(SS_INI);
    StackPluginSave(&hPluginsStack, SS_INI);

    //Save to registry
    SaveOptions(&moCur, lpFrameCurrent, SS_REGISTRY, TRUE);
    SaveThemes(SS_REGISTRY);
    StackPluginSave(&hPluginsStack, SS_REGISTRY);
  }

  if (bOptionsRestart)
  {
    LoadStringWide(hLangLib, MSG_RESTART_PROGRAM, wbuf, BUFFER_SIZE);
    API_MessageBox(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
  }
}

void DoWindowTabView(DWORD dwNewView, BOOL bFirst)
{
  DWORD dwOldView=moCur.dwTabOptionsMDI;
  DWORD dwStyle;
  int nCommand=0;

  if (dwNewView & TAB_VIEW_TOP)
  {
    nCommand=IDM_WINDOW_TABVIEW_TOP;
    dwNewView=TAB_VIEW_TOP;
  }
  else if (dwNewView & TAB_VIEW_BOTTOM)
  {
    nCommand=IDM_WINDOW_TABVIEW_BOTTOM;
    dwNewView=TAB_VIEW_BOTTOM;
  }
  else if (dwNewView & TAB_VIEW_NONE)
  {
    nCommand=IDM_WINDOW_TABVIEW_NONE;
    dwNewView=TAB_VIEW_NONE;
  }
  if (dwOldView & TAB_VIEW_TOP)
    dwOldView=TAB_VIEW_TOP;
  else if (dwOldView & TAB_VIEW_BOTTOM)
    dwOldView=TAB_VIEW_BOTTOM;
  else if (dwOldView & TAB_VIEW_NONE)
    dwOldView=TAB_VIEW_NONE;
  CheckMenuRadioItem(hMainMenu, IDM_WINDOW_TABVIEW_TOP, IDM_WINDOW_TABVIEW_NONE, nCommand, MF_BYCOMMAND);
  EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_STANDARD, !(dwNewView & TAB_VIEW_NONE)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_BUTTONS, !(dwNewView & TAB_VIEW_NONE)?MF_ENABLED:MF_GRAYED);
  if (!bOldComctl32) EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_FLATBUTTONS, !(dwNewView & TAB_VIEW_NONE)?MF_ENABLED:MF_GRAYED);

  if (bFirst != TRUE && dwNewView == dwOldView) return;
  moCur.dwTabOptionsMDI=moCur.dwTabOptionsMDI & ~TAB_VIEW_TOP & ~TAB_VIEW_BOTTOM & ~TAB_VIEW_NONE;
  moCur.dwTabOptionsMDI|=dwNewView;

  if (moCur.dwTabOptionsMDI & TAB_VIEW_TOP)
  {
    dwStyle=GetWindowLongWide(hTab, GWL_STYLE);
    SetWindowLongWide(hTab, GWL_STYLE, dwStyle & ~TCS_BOTTOM);
  }
  else if (moCur.dwTabOptionsMDI & TAB_VIEW_BOTTOM)
  {
    dwStyle=GetWindowLongWide(hTab, GWL_STYLE);
    SetWindowLongWide(hTab, GWL_STYLE, dwStyle|TCS_BOTTOM);
  }
  ShowWindow(hTab, !(moCur.dwTabOptionsMDI & TAB_VIEW_NONE)?SW_SHOW:SW_HIDE);
  UpdateSize();
}

void DoWindowTabType(DWORD dwNewType, BOOL bFirst)
{
  DWORD dwOldType=moCur.dwTabOptionsMDI;
  DWORD dwStyle;
  int nCommand=0;

  if (dwNewType & TAB_TYPE_STANDARD)
  {
    nCommand=IDM_WINDOW_TABTYPE_STANDARD;
    dwNewType=TAB_TYPE_STANDARD;
  }
  else if (dwNewType & TAB_TYPE_BUTTONS)
  {
    nCommand=IDM_WINDOW_TABTYPE_BUTTONS;
    dwNewType=TAB_TYPE_BUTTONS;
  }
  else if (dwNewType & TAB_TYPE_FLATBUTTONS)
  {
    nCommand=IDM_WINDOW_TABTYPE_FLATBUTTONS;
    dwNewType=TAB_TYPE_FLATBUTTONS;
  }
  if (dwOldType & TAB_TYPE_STANDARD)
    dwOldType=TAB_TYPE_STANDARD;
  else if (dwOldType & TAB_TYPE_BUTTONS)
    dwOldType=TAB_TYPE_BUTTONS;
  else if (dwOldType & TAB_TYPE_FLATBUTTONS)
    dwOldType=TAB_TYPE_FLATBUTTONS;
  CheckMenuRadioItem(hMainMenu, IDM_WINDOW_TABTYPE_STANDARD, IDM_WINDOW_TABTYPE_FLATBUTTONS, nCommand, MF_BYCOMMAND);

  if (bFirst != TRUE && dwNewType == dwOldType) return;
  moCur.dwTabOptionsMDI=moCur.dwTabOptionsMDI & ~TAB_TYPE_STANDARD & ~TAB_TYPE_BUTTONS & ~TAB_TYPE_FLATBUTTONS;
  moCur.dwTabOptionsMDI|=dwNewType;

  if (moCur.dwTabOptionsMDI & TAB_TYPE_STANDARD)
  {
    dwStyle=GetWindowLongWide(hTab, GWL_STYLE);
    SetWindowLongWide(hTab, GWL_STYLE, dwStyle & ~TCS_BUTTONS & ~TCS_FLATBUTTONS);
    SendMessage(hTab, TCM_SETITEMSIZE, 0, MAKELPARAM(TAB_WIDTH, TAB_HEIGHT - 4));
  }
  else if (moCur.dwTabOptionsMDI & TAB_TYPE_BUTTONS)
  {
    dwStyle=GetWindowLongWide(hTab, GWL_STYLE);
    SetWindowLongWide(hTab, GWL_STYLE, (dwStyle|TCS_BUTTONS) & ~TCS_FLATBUTTONS);
    SendMessage(hTab, TCM_SETITEMSIZE, 0, MAKELPARAM(TAB_WIDTH, TAB_HEIGHT));
  }
  else if (moCur.dwTabOptionsMDI & TAB_TYPE_FLATBUTTONS)
  {
    dwStyle=GetWindowLongWide(hTab, GWL_STYLE);
    SetWindowLongWide(hTab, GWL_STYLE, dwStyle|TCS_BUTTONS|TCS_FLATBUTTONS);
    SendMessage(hTab, TCM_SETITEMSIZE, 0, MAKELPARAM(TAB_WIDTH, TAB_HEIGHT));
  }
}

void DoWindowSelectWindow()
{
  if (bOldWindows)
    API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_MDILIST), hMainWnd, (DLGPROC)MdiListDlgProc);
  else
    API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_MDILIST), hMainWnd, (DLGPROC)MdiListDlgProc);
}

void DoHelpAbout()
{
  if (bOldWindows)
    API_DialogBoxA(hLangLib, MAKEINTRESOURCEA(IDD_ABOUT), hMainWnd, (DLGPROC)AboutDlgProc);
  else
    API_DialogBoxW(hLangLib, MAKEINTRESOURCEW(IDD_ABOUT), hMainWnd, (DLGPROC)AboutDlgProc);
}

void DoNonMenuDelLine(HWND hWnd)
{
  AECHARRANGE cr=crSel;

  if (IsReadOnly(hWnd)) return;

  cr.ciMin.nCharInLine=0;
  if (!SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTLINE, (LPARAM)&cr.ciMax))
    cr.ciMax.nCharInLine=cr.ciMax.lpLine->nLineLen;
  SetSel(hWnd, &cr, AESELT_LOCKSCROLL, NULL);

  ReplaceSelW(hWnd, L"", -1, FALSE, NULL, NULL);
}


//// INI functions

BOOL OpenIniA(INIFILE *hIniFile, const char *pFile, BOOL bCreate)
{
  HANDLE hFile=INVALID_HANDLE_VALUE;

  if (!bCreate)
  {
    if (FileExistsAnsi(pFile))
      hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  }
  else hFile=API_CreateFileA(pFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (hFile != INVALID_HANDLE_VALUE)
  {
    ReadIni(hIniFile, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
}

BOOL OpenIniW(INIFILE *hIniFile, const wchar_t *wpFile, BOOL bCreate)
{
  HANDLE hFile=INVALID_HANDLE_VALUE;

  if (!bCreate)
  {
    if (FileExistsWide(wpFile))
      hFile=CreateFileWide(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  }
  else hFile=CreateFileWide(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (hFile != INVALID_HANDLE_VALUE)
  {
    ReadIni(hIniFile, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
}

BOOL SaveIniA(INIFILE *hIniFile, const char *pFile)
{
  HANDLE hFile;
  DWORD dwAttr;
  BOOL bResult=FALSE;

  dwAttr=GetFileAttributesA(pFile);

  if (dwAttr == INVALID_FILE_ATTRIBUTES || !(dwAttr & FILE_ATTRIBUTE_READONLY))
  {
    if ((hFile=API_CreateFileA(pFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
    {
      bResult=WriteIni(hIniFile, hFile);
      CloseHandle(hFile);
    }
  }
  return bResult;
}

BOOL SaveIniW(INIFILE *hIniFile, const wchar_t *wpFile)
{
  HANDLE hFile;
  DWORD dwAttr;
  BOOL bResult=FALSE;

  dwAttr=GetFileAttributesWide(wpFile);

  if (dwAttr == INVALID_FILE_ATTRIBUTES || !(dwAttr & FILE_ATTRIBUTE_READONLY))
  {
    if ((hFile=CreateFileWide(wpFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
    {
      bResult=WriteIni(hIniFile, hFile);
      CloseHandle(hFile);
    }
  }
  return bResult;
}

BOOL ReadIni(INIFILE *hIniFile, HANDLE hFile)
{
  INISECTION *lpIniSection=NULL;
  INIKEY *lpIniKey=NULL;
  wchar_t *wszText=NULL;
  wchar_t *wpText;
  wchar_t *wpSection;
  wchar_t *wpKey;
  wchar_t *wpString;
  wchar_t *wpLastChar;
  DWORD dwFileSize;
  DWORD dwBytesRead;
  DWORD dwUnicodeLen;
  int nSectionLen;
  int nKeyLen;
  int nStringLen;

  if ((dwFileSize=GetFileSize(hFile, NULL)) != INVALID_FILE_SIZE)
  {
    if (dwFileSize >= sizeof(wchar_t))
    {
      if (wszText=wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, dwFileSize + sizeof(wchar_t)))
      {
        if (ReadFile(hFile, wpText, dwFileSize, &dwBytesRead, NULL))
        {
          dwUnicodeLen=dwBytesRead / sizeof(wchar_t);
          wpLastChar=wpText + dwUnicodeLen;
          wpText[dwUnicodeLen++]='\0';
          if (wszText[0] == 0xFEFF)
          {
            ++wpText;
            --dwUnicodeLen;
          }

          while (*wpText == ' ' || *wpText == '\t' || *wpText == '\r' || *wpText == '\n')
            ++wpText;

          while (wpText < wpLastChar)
          {
            //Section
            if (*wpText == '[')
            {
              for (wpSection=++wpText; wpText < wpLastChar && *wpText != ']' && *wpText != '\r' && *wpText != '\n'; ++wpText);

              if (*wpText == ']')
              {
                nSectionLen=wpText - wpSection;
                *wpText++='\0';

                if (!StackInsertIndex((stack **)&hIniFile->first, (stack **)&hIniFile->last, (stack **)&lpIniSection, -1, sizeof(INISECTION)))
                {
                  lpIniSection->nSectionBytes=(nSectionLen + 1) * sizeof(wchar_t);
                  if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionBytes))
                    xmemcpy(lpIniSection->wszSection, wpSection, lpIniSection->nSectionBytes);

                  lpIniSection->first=0;
                  lpIniSection->last=0;
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
              if (wpText >= wpLastChar || *wpText == '[') break;

              for (wpKey=wpText; wpText < wpLastChar && *wpText != '=' && *wpText != '\r' && *wpText != '\n'; ++wpText);

              if (*wpText == '=')
              {
                nKeyLen=wpText - wpKey;
                *wpText++='\0';

                for (wpString=wpText; wpText < wpLastChar && *wpText != '\r' && *wpText != '\n'; ++wpText);
                nStringLen=wpText - wpString;
                if (*wpText) *wpText++='\0';

                if (!StackInsertIndex((stack **)&lpIniSection->first, (stack **)&lpIniSection->last, (stack **)&lpIniKey, -1, sizeof(INIKEY)))
                {
                  lpIniKey->nKeyBytes=(nKeyLen + 1) * sizeof(wchar_t);
                  if (lpIniKey->wszKey=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyBytes))
                    xmemcpy(lpIniKey->wszKey, wpKey, lpIniKey->nKeyBytes);

                  lpIniKey->nStringBytes=(nStringLen + 1) * sizeof(wchar_t);
                  if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringBytes))
                    xmemcpy(lpIniKey->wszString, wpString, lpIniKey->nStringBytes);
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

BOOL WriteIni(INIFILE *hIniFile, HANDLE hFile)
{
  INISECTION *lpIniSection=(INISECTION *)hIniFile->first;
  INIKEY *lpIniKey=NULL;
  DWORD dwBytesWritten;

  if (!API_WriteFile(hFile, "\xFF\xFE", 2, &dwBytesWritten, NULL))
    return FALSE;

  while (lpIniSection)
  {
    //Section
    if (!API_WriteFile(hFile, L"[", 2, &dwBytesWritten, NULL))
      return FALSE;
    if (!API_WriteFile(hFile, lpIniSection->wszSection, lpIniSection->nSectionBytes - sizeof(wchar_t), &dwBytesWritten, NULL))
      return FALSE;
    if (!API_WriteFile(hFile, L"]\r\n", 6, &dwBytesWritten, NULL))
      return FALSE;

    //Keys and strings
    lpIniKey=(INIKEY *)lpIniSection->first;

    while (lpIniKey)
    {
      if (!API_WriteFile(hFile, lpIniKey->wszKey, lpIniKey->nKeyBytes - sizeof(wchar_t), &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, L"=", 2, &dwBytesWritten, NULL))
        return FALSE;
      if (!API_WriteFile(hFile, lpIniKey->wszString, lpIniKey->nStringBytes - sizeof(wchar_t), &dwBytesWritten, NULL))
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

INISECTION* StackOpenIniSectionA(INIFILE *hIniFile, char *pSection, int nSectionLen, BOOL bCreate)
{
  INISECTION *lpIniSection=NULL;
  wchar_t *wpSection;

  if (wpSection=(wchar_t *)API_HeapAlloc(hHeap, 0, (nSectionLen + 1) * sizeof(wchar_t)))
  {
    nSectionLen=MultiByteToWideChar(CP_ACP, 0, pSection, nSectionLen + 1, wpSection, nSectionLen + 1);
    lpIniSection=StackOpenIniSectionW(hIniFile, wpSection, nSectionLen - 1, bCreate);
    API_HeapFree(hHeap, 0, (LPVOID)wpSection);
  }
  return lpIniSection;
}

INISECTION* StackOpenIniSectionW(INIFILE *hIniFile, wchar_t *wpSection, int nSectionLen, BOOL bCreate)
{
  INISECTION *lpIniSection=(INISECTION *)hIniFile->first;
  int nSectionBytes=(nSectionLen + 1) * sizeof(wchar_t);

  while (lpIniSection)
  {
    if (lpIniSection->nSectionBytes == nSectionBytes)
    {
      if (!xstrcmpiW(lpIniSection->wszSection, wpSection))
        return lpIniSection;
    }
    lpIniSection=lpIniSection->next;
  }

  if (bCreate)
  {
    if (!StackInsertIndex((stack **)&hIniFile->first, (stack **)&hIniFile->last, (stack **)&lpIniSection, -1, sizeof(INISECTION)))
    {
      lpIniSection->nSectionBytes=nSectionBytes;
      if (lpIniSection->wszSection=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniSection->nSectionBytes))
        xmemcpy(lpIniSection->wszSection, wpSection, lpIniSection->nSectionBytes);

      lpIniSection->first=0;
      lpIniSection->last=0;
    }
  }
  return lpIniSection;
}

INIKEY* StackOpenIniKeyA(INISECTION *lpIniSection, char *pKey, int nKeyLen, BOOL bCreate)
{
  INIKEY *lpIniKey=NULL;
  wchar_t *wpKey;

  if (wpKey=(wchar_t *)API_HeapAlloc(hHeap, 0, (nKeyLen + 1) * sizeof(wchar_t)))
  {
    nKeyLen=MultiByteToWideChar(CP_ACP, 0, pKey, nKeyLen + 1, wpKey, nKeyLen + 1);
    lpIniKey=StackOpenIniKeyW(lpIniSection, wpKey, nKeyLen - 1, bCreate);
    API_HeapFree(hHeap, 0, (LPVOID)wpKey);
  }
  return lpIniKey;
}

INIKEY* StackOpenIniKeyW(INISECTION *lpIniSection, wchar_t *wpKey, int nKeyLen, BOOL bCreate)
{
  INIKEY *lpIniKey=(INIKEY *)lpIniSection->first;
  int nKeyBytes=(nKeyLen + 1) * sizeof(wchar_t);

  while (lpIniKey)
  {
    if (lpIniKey->nKeyBytes == nKeyBytes)
    {
      if (!xstrcmpiW(lpIniKey->wszKey, wpKey))
        return lpIniKey;
    }
    lpIniKey=lpIniKey->next;
  }

  if (bCreate)
  {
    if (!StackInsertIndex((stack **)&lpIniSection->first, (stack **)&lpIniSection->last, (stack **)&lpIniKey, -1, sizeof(INIKEY)))
    {
      lpIniKey->nKeyBytes=nKeyBytes;
      if (lpIniKey->wszKey=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nKeyBytes))
        xmemcpy(lpIniKey->wszKey, wpKey, lpIniKey->nKeyBytes);

      lpIniKey->wszString=NULL;
      lpIniKey->nStringBytes=0;
    }
  }
  return lpIniKey;
}

int StackGetIniData(INIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  DWORD dwStringLen;

  if (nType == INI_DWORD)
  {
    if (lpData)
    {
      DWORD dwNumber;

      dwNumber=(DWORD)xatoiW(lpIniKey->wszString, NULL);
      dwStringLen=min(sizeof(DWORD), dwDataBytes);
      xmemcpy((DWORD *)lpData, &dwNumber, dwStringLen);
      return dwStringLen;
    }
    return sizeof(DWORD);
  }
  else if (nType == INI_BINARY)
  {
    return hex2binW(lpIniKey->wszString, lpData, dwDataBytes);
  }
  else if (nType == INI_STRINGANSI)
  {
    if (lpData)
    {
      dwStringLen=WideCharToMultiByte(CP_ACP, 0, lpIniKey->wszString, lpIniKey->nStringBytes / sizeof(wchar_t), (char *)lpData, dwDataBytes / sizeof(wchar_t), NULL, NULL);
      ((char *)lpData)[dwStringLen - 1]='\0';
      return dwStringLen;
    }
    return WideCharToMultiByte(CP_ACP, 0, lpIniKey->wszString, lpIniKey->nStringBytes / sizeof(wchar_t), NULL, 0, NULL, NULL);
  }
  else if (nType == INI_STRINGUNICODE)
  {
    if (lpData)
    {
      dwStringLen=min((DWORD)lpIniKey->nStringBytes, dwDataBytes);
      dwStringLen=(dwStringLen / sizeof(wchar_t)) * sizeof(wchar_t);
      xmemcpy((wchar_t *)lpData, lpIniKey->wszString, dwStringLen);
      ((wchar_t *)lpData)[dwStringLen / sizeof(wchar_t) - 1]=L'\0';
      return dwStringLen;
    }
    return lpIniKey->nStringBytes;
  }
  return 0;
}

BOOL StackSetIniData(INIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  if (lpIniKey->wszString)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);
    lpIniKey->wszString=NULL;
    lpIniKey->nStringBytes=0;
  }

  if (nType == INI_DWORD)
  {
    wchar_t wszNumber[16];

    lpIniKey->nStringBytes=(xuitoaW(*(DWORD *)lpData, wszNumber) + 1) * sizeof(wchar_t);
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringBytes))
    {
      xmemcpy(lpIniKey->wszString, wszNumber, lpIniKey->nStringBytes);
      return TRUE;
    }
  }
  else if (nType == INI_BINARY)
  {
    lpIniKey->nStringBytes=(dwDataBytes * 2 + 1) * sizeof(wchar_t);
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringBytes))
    {
      bin2hexW(lpData, dwDataBytes, lpIniKey->wszString, lpIniKey->nStringBytes / sizeof(wchar_t), FALSE);
      return TRUE;
    }
  }
  else if (nType == INI_STRINGANSI)
  {
    dwDataBytes=max(dwDataBytes, sizeof(char));

    lpIniKey->nStringBytes=MultiByteToWideChar(CP_ACP, 0, (char *)lpData, dwDataBytes, NULL, 0) * sizeof(wchar_t);
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringBytes))
    {
      MultiByteToWideChar(CP_ACP, 0, (char *)lpData, dwDataBytes, lpIniKey->wszString, lpIniKey->nStringBytes / sizeof(wchar_t));
      if (lpIniKey->wszString[lpIniKey->nStringBytes / sizeof(wchar_t) - 1] != L'\0')
        lpIniKey->wszString[lpIniKey->nStringBytes / sizeof(wchar_t) - 1]=L'\0';
      return TRUE;
    }
  }
  else if (nType == INI_STRINGUNICODE)
  {
    dwDataBytes=max(dwDataBytes, sizeof(wchar_t));

    lpIniKey->nStringBytes=(dwDataBytes / sizeof(wchar_t)) * sizeof(wchar_t);
    if (lpIniKey->wszString=(wchar_t *)API_HeapAlloc(hHeap, 0, lpIniKey->nStringBytes))
    {
      xmemcpy(lpIniKey->wszString, (wchar_t *)lpData, lpIniKey->nStringBytes);
      if (lpIniKey->wszString[lpIniKey->nStringBytes / sizeof(wchar_t) - 1] != L'\0')
        lpIniKey->wszString[lpIniKey->nStringBytes / sizeof(wchar_t) - 1]=L'\0';
      return TRUE;
    }
  }
  return FALSE;
}

int IniGetValueA(INIFILE *hIniFile, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  INISECTION *lpIniSection;
  INIKEY *lpIniKey;

  if (!(lpIniSection=StackOpenIniSectionA(hIniFile, pSection, lstrlenA(pSection), FALSE)))
    return 0;
  if (!(lpIniKey=StackOpenIniKeyA(lpIniSection, pKey, lstrlenA(pKey), FALSE)))
    return 0;

  return StackGetIniData(lpIniKey, nType, lpData, dwDataBytes);
}

int IniGetValueW(INIFILE *hIniFile, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  INISECTION *lpIniSection;
  INIKEY *lpIniKey;

  if (!(lpIniSection=StackOpenIniSectionW(hIniFile, wpSection, lstrlenW(wpSection), FALSE)))
    return 0;
  if (!(lpIniKey=StackOpenIniKeyW(lpIniSection, wpKey, lstrlenW(wpKey), FALSE)))
    return 0;

  return StackGetIniData(lpIniKey, nType, lpData, dwDataBytes);
}

BOOL IniSetValueA(INIFILE *hIniFile, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  INISECTION *lpIniSection;
  INIKEY *lpIniKey;

  if (!(lpIniSection=StackOpenIniSectionA(hIniFile, pSection, lstrlenA(pSection), TRUE)))
    return FALSE;
  if (!(lpIniKey=StackOpenIniKeyA(lpIniSection, pKey, lstrlenA(pKey), TRUE)))
    return FALSE;

  return StackSetIniData(lpIniKey, nType, lpData, dwDataBytes);
}

BOOL IniSetValueW(INIFILE *hIniFile, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes)
{
  INISECTION *lpIniSection;
  INIKEY *lpIniKey;

  if (!(lpIniSection=StackOpenIniSectionW(hIniFile, wpSection, lstrlenW(wpSection), TRUE)))
    return FALSE;
  if (!(lpIniKey=StackOpenIniKeyW(lpIniSection, wpKey, lstrlenW(wpKey), TRUE)))
    return FALSE;

  return StackSetIniData(lpIniKey, nType, lpData, dwDataBytes);
}

void StackDeleteIniKey(INISECTION *lpIniSection, INIKEY *lpIniKey)
{
  if (lpIniKey)
  {
    if (lpIniKey->wszKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszKey);
    if (lpIniKey->wszString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);

    StackDelete((stack **)&lpIniSection->first, (stack **)&lpIniSection->last, (stack *)lpIniKey);
  }
}

void StackDeleteIniSection(INIFILE *hIniFile, INISECTION *lpIniSection, BOOL bOnlyClear)
{
  INIKEY *lpIniKey=(INIKEY *)lpIniSection->first;

  while (lpIniKey)
  {
    if (lpIniKey->wszKey) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszKey);
    if (lpIniKey->wszString) API_HeapFree(hHeap, 0, (LPVOID)lpIniKey->wszString);

    lpIniKey=lpIniKey->next;
  }
  StackClear((stack **)&lpIniSection->first, (stack **)&lpIniSection->last);

  if (!bOnlyClear)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpIniSection->wszSection);
    StackDelete((stack **)&hIniFile->first, (stack **)&hIniFile->last, (stack *)lpIniSection);
  }
}

void StackFreeIni(INIFILE *hIniFile)
{
  INISECTION *lpIniSection=(INISECTION *)hIniFile->first;

  while (lpIniSection)
  {
    API_HeapFree(hHeap, 0, (LPVOID)lpIniSection->wszSection);
    StackDeleteIniSection(hIniFile, lpIniSection, TRUE);

    lpIniSection=lpIniSection->next;
  }
  StackClear((stack **)&hIniFile->first, (stack **)&hIniFile->last);
}


//// Read/Save options

DWORD ReadOption(OPTIONHANDLE *oh, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize)
{
  if (oh->nSaveSettings == SS_REGISTRY)
  {
    if (dwType & MOT_DWORD) dwType=REG_DWORD;
    else if (dwType & MOT_BINARY) dwType=REG_BINARY;
    else if (dwType & MOT_STRING) dwType=REG_SZ;

    if (RegQueryValueExWide((HKEY)oh->hHandle, wpParam, NULL, &dwType, (LPBYTE)lpData, &dwSize) == ERROR_SUCCESS)
      return dwSize;
    else
      return 0;
  }
  else
  {
    INIKEY *lpIniKey;

    if (dwType & MOT_DWORD) dwType=INI_DWORD;
    else if (dwType & MOT_BINARY) dwType=INI_BINARY;
    else if (dwType & MOT_STRING) dwType=INI_STRINGUNICODE;

    if (lpIniKey=StackOpenIniKeyW((INISECTION *)oh->hHandle, wpParam, lstrlenW(wpParam), FALSE))
      return StackGetIniData(lpIniKey, dwType, (LPBYTE)lpData, dwSize);
    else
      return 0;
  }
}

DWORD SaveOption(OPTIONHANDLE *oh, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize)
{
  if (dwType & MOT_MAINOFFSET)
  {
    //lpData is MAINOPTIONS structure offset. Check if specified member in lpData has been changed.
    if (!oh->bForceWrite && !xmemcmp(((LPBYTE)oh->mo) + (DWORD)lpData, ((LPBYTE)&moInit) + (DWORD)lpData, dwSize))
      return dwSize;
    lpData=((LPBYTE)oh->mo) + (DWORD)lpData;
  }
  else if (dwType & MOT_FRAMEOFFSET)
  {
    //lpData is FRAMEDATA structure offset. Check if specified member in lpData has been changed.
    if (!oh->bForceWrite && !xmemcmp(((LPBYTE)oh->fd) + (DWORD)lpData, ((LPBYTE)&fdInit) + (DWORD)lpData, dwSize))
      return dwSize;
    lpData=((LPBYTE)oh->fd) + (DWORD)lpData;
  }
  else if (dwType & MOT_MANUAL)
  {
    //lpData is manual parameter. Save it only if no bForceWrite and not saved before.
    if (!oh->bForceWrite && ReadOption(oh, wpParam, dwType, NULL, 0))
      return dwSize;
  }

  if (oh->nSaveSettings == SS_REGISTRY)
  {
    if (dwType & MOT_DWORD) dwType=REG_DWORD;
    else if (dwType & MOT_BINARY) dwType=REG_BINARY;
    else if (dwType & MOT_STRING) dwType=REG_SZ;

    return !RegSetValueExWide((HKEY)oh->hHandle, wpParam, 0, dwType, (LPBYTE)lpData, dwSize);
  }
  else
  {
    INIKEY *lpIniKey;

    if (dwType & MOT_DWORD) dwType=INI_DWORD;
    else if (dwType & MOT_BINARY) dwType=INI_BINARY;
    else if (dwType & MOT_STRING) dwType=INI_STRINGUNICODE;

    if (lpIniKey=StackOpenIniKeyW((INISECTION *)oh->hHandle, wpParam, lstrlenW(wpParam), TRUE))
      return StackSetIniData(lpIniKey, dwType, (LPBYTE)lpData, dwSize);
    else
      return 0;
  }
}

void ReadOptions(MAINOPTIONS *mo, FRAMEDATA *fd)
{
  OPTIONHANDLE oh;
  DWORD dwSize;

  xprintfW(wszIniFile, L"%s\\AkelPad.ini", wszExeDir);
  if (OpenIniW(&hIniFile, wszIniFile, FALSE))
    IniGetValueW(&hIniFile, L"Options", L"SaveSettings", INI_DWORD, (LPBYTE)&mo->nSaveSettings, sizeof(DWORD));
  oh.mo=mo;
  oh.fd=fd;
  oh.nSaveSettings=mo->nSaveSettings;

  if (oh.nSaveSettings == SS_REGISTRY)
  {
    wchar_t wszRegKey[MAX_PATH];

    xprintfW(wszRegKey, L"%s\\Options", APP_REGHOMEW);
    if (RegOpenKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, KEY_READ, (HKEY *)&oh.hHandle) != ERROR_SUCCESS)
      oh.hHandle=NULL;
  }
  else
  {
    if (!(oh.hHandle=(HANDLE)StackOpenIniSectionW(&hIniFile, L"Options", lstrlenW(L"Options"), FALSE)))
      oh.hHandle=NULL;
  }

  if (oh.hHandle)
  {
    //Manual
    ReadOption(&oh, L"ShowModify", MOT_DWORD, &mo->dwShowModify, sizeof(DWORD));
    ReadOption(&oh, L"StatusPosType", MOT_DWORD, &mo->dwStatusPosType, sizeof(DWORD));
    ReadOption(&oh, L"StatusUserFormat", MOT_STRING, mo->wszStatusUserFormat, sizeof(mo->wszStatusUserFormat));
    ReadOption(&oh, L"WordBreak", MOT_DWORD, &mo->dwWordBreakCustom, sizeof(DWORD));
    ReadOption(&oh, L"PaintOptions", MOT_DWORD, &mo->dwPaintOptions, sizeof(DWORD));
    ReadOption(&oh, L"RichEditClass", MOT_DWORD, &mo->bRichEditClass, sizeof(DWORD));
    ReadOption(&oh, L"DateLogFormat", MOT_STRING, mo->wszDateLogFormat, sizeof(mo->wszDateLogFormat));
    ReadOption(&oh, L"DateInsertFormat", MOT_STRING, mo->wszDateInsertFormat, sizeof(mo->wszDateInsertFormat));

    //Frame data
    ReadOption(&oh, L"WordWrap", MOT_DWORD, &fd->ei.bWordWrap, sizeof(DWORD));
    ReadOption(&oh, L"UndoLimit", MOT_DWORD, &fd->nUndoLimit, sizeof(DWORD));
    ReadOption(&oh, L"DetailedUndo", MOT_DWORD, &fd->bDetailedUndo, sizeof(DWORD));
    ReadOption(&oh, L"WrapType", MOT_DWORD, &fd->dwWrapType, sizeof(DWORD));
    ReadOption(&oh, L"WrapLimit", MOT_DWORD, &fd->dwWrapLimit, sizeof(DWORD));
    ReadOption(&oh, L"Marker", MOT_DWORD, &fd->dwMarker, sizeof(DWORD));
    ReadOption(&oh, L"CaretOptions", MOT_DWORD, &fd->dwCaretOptions, sizeof(DWORD));
    ReadOption(&oh, L"CaretWidth", MOT_DWORD, &fd->nCaretWidth, sizeof(DWORD));
    ReadOption(&oh, L"MouseOptions", MOT_DWORD, &fd->dwMouseOptions, sizeof(DWORD));
    ReadOption(&oh, L"LineGap", MOT_DWORD, &fd->dwLineGap, sizeof(DWORD));
    ReadOption(&oh, L"TabStopSize", MOT_DWORD, &fd->nTabStopSize, sizeof(DWORD));
    ReadOption(&oh, L"TabStopAsSpaces", MOT_DWORD, &fd->bTabStopAsSpaces, sizeof(DWORD));
    ReadOption(&oh, L"MarginsEdit", MOT_DWORD, &fd->dwEditMargins, sizeof(DWORD));
    ReadOption(&oh, L"ShowURL", MOT_DWORD, &fd->bShowURL, sizeof(DWORD));
    ReadOption(&oh, L"ClickURL", MOT_DWORD, &fd->nClickURL, sizeof(DWORD));
    ReadOption(&oh, L"UrlPrefixesEnable", MOT_DWORD, &fd->bUrlPrefixesEnable, sizeof(DWORD));
    ReadOption(&oh, L"UrlPrefixes", MOT_BINARY, fd->wszUrlPrefixes, sizeof(fd->wszUrlPrefixes));
    ReadOption(&oh, L"UrlDelimitersEnable", MOT_DWORD, &fd->bUrlDelimitersEnable, sizeof(DWORD));
    ReadOption(&oh, L"UrlLeftDelimiters", MOT_BINARY, fd->wszUrlLeftDelimiters, sizeof(fd->wszUrlLeftDelimiters));
    ReadOption(&oh, L"UrlRightDelimiters", MOT_BINARY, fd->wszUrlRightDelimiters, sizeof(fd->wszUrlRightDelimiters));
    ReadOption(&oh, L"WordDelimitersEnable", MOT_DWORD, &fd->bWordDelimitersEnable, sizeof(DWORD));
    ReadOption(&oh, L"WordDelimiters", MOT_BINARY, fd->wszWordDelimiters, sizeof(fd->wszWordDelimiters));
    ReadOption(&oh, L"WrapDelimitersEnable", MOT_DWORD, &fd->bWrapDelimitersEnable, sizeof(DWORD));
    ReadOption(&oh, L"WrapDelimiters", MOT_BINARY, fd->wszWrapDelimiters, sizeof(fd->wszWrapDelimiters));
    ReadOption(&oh, L"Font", MOT_BINARY, &fd->lf, offsetof(LOGFONTW, lfFaceName));
    ReadOption(&oh, L"FontFace", MOT_STRING, fd->lf.lfFaceName, sizeof(fd->lf.lfFaceName));
    ReadOption(&oh, L"Colors", MOT_BINARY, &fd->aec, sizeof(AECOLORS));

    //Menu settings
    ReadOption(&oh, L"OnTop", MOT_DWORD, &mo->bOnTop, sizeof(DWORD));
    ReadOption(&oh, L"StatusBar", MOT_DWORD, &mo->bStatusBar, sizeof(DWORD));
    ReadOption(&oh, L"LanguageModule", MOT_STRING, mo->wszLangModule, sizeof(mo->wszLangModule));
    ReadOption(&oh, L"KeepSpace", MOT_DWORD, &mo->bKeepSpace, sizeof(DWORD));
    ReadOption(&oh, L"WatchFile", MOT_DWORD, &mo->bWatchFile, sizeof(DWORD));
    ReadOption(&oh, L"SaveTime", MOT_DWORD, &mo->bSaveTime, sizeof(DWORD));
    ReadOption(&oh, L"SingleOpenFile", MOT_DWORD, &mo->bSingleOpenFile, sizeof(DWORD));
    ReadOption(&oh, L"SingleOpenProgram", MOT_DWORD, &mo->bSingleOpenProgram, sizeof(DWORD));
    ReadOption(&oh, L"MDI", MOT_DWORD, &mo->nMDI, sizeof(DWORD));
    if (mo->nMDI)
    {
      ReadOption(&oh, L"TabOptionsMDI", MOT_DWORD, &mo->dwTabOptionsMDI, sizeof(DWORD));
    }

    //Settings dialog
    if (dwSize=ReadOption(&oh, L"CodepageList", INI_BINARY, NULL, 0))
    {
      if (lpCodepageList=(int *)API_HeapAlloc(hHeap, 0, dwSize))
      {
        ReadOption(&oh, L"CodepageList", MOT_BINARY, lpCodepageList, dwSize);
        nCodepageListLen=CodepageListLen(lpCodepageList);
      }
    }
    ReadOption(&oh, L"ExecuteCommand", MOT_STRING, mo->wszCommand, sizeof(mo->wszCommand));
    ReadOption(&oh, L"ExecuteDirectory", MOT_STRING, mo->wszWorkDir, sizeof(mo->wszWorkDir));
    ReadOption(&oh, L"DefaultCodepage", MOT_DWORD, &mo->nDefaultCodePage, sizeof(DWORD));
    ReadOption(&oh, L"DefaultNewLine", MOT_DWORD, &mo->nDefaultNewLine, sizeof(DWORD));
    ReadOption(&oh, L"CodepageRecognition", MOT_DWORD, &mo->dwLangCodepageRecognition, sizeof(DWORD));
    ReadOption(&oh, L"CodepageRecognitionBuffer", MOT_DWORD, &mo->dwCodepageRecognitionBuffer, sizeof(DWORD));
    ReadOption(&oh, L"SavePositions", MOT_DWORD, &mo->bSavePositions, sizeof(DWORD));
    ReadOption(&oh, L"SaveCodepages", MOT_DWORD, &mo->bSaveCodepages, sizeof(DWORD));
    ReadOption(&oh, L"RecentFiles", MOT_DWORD, &mo->nRecentFiles, sizeof(DWORD));
    ReadOption(&oh, L"SearchStrings", MOT_DWORD, &mo->nSearchStrings, sizeof(DWORD));
    ReadOption(&oh, L"FileTypesOpen", MOT_STRING, mo->wszFileTypesOpen, sizeof(mo->wszFileTypesOpen));
    ReadOption(&oh, L"FileTypesEdit", MOT_STRING, mo->wszFileTypesEdit, sizeof(mo->wszFileTypesEdit));
    ReadOption(&oh, L"FileTypesPrint", MOT_STRING, mo->wszFileTypesPrint, sizeof(mo->wszFileTypesPrint));
    ReadOption(&oh, L"FileTypesAssociated", MOT_DWORD, &mo->dwFileTypesAssociated, sizeof(DWORD));
    if (mo->nMDI)
    {
      ReadOption(&oh, L"KeybLayoutMDI", MOT_DWORD, &mo->bKeybLayoutMDI, sizeof(DWORD));
    }
    ReadOption(&oh, L"DateLog", MOT_DWORD, &mo->bDateLog, sizeof(DWORD));
    ReadOption(&oh, L"SaveInReadOnlyMsg", MOT_DWORD, &mo->bSaveInReadOnlyMsg, sizeof(DWORD));
    ReadOption(&oh, L"DefaultSaveExt", MOT_STRING, mo->wszDefaultSaveExt, sizeof(mo->wszDefaultSaveExt));

    //Search dialog
    ReadOption(&oh, L"SearchOptions", MOT_DWORD, &mo->dwSearchOptions, sizeof(DWORD));

    //Open file dialog
    ReadOption(&oh, L"LastDirectory", MOT_STRING, mo->wszLastDir, sizeof(mo->wszLastDir));

    //Print dialog
    ReadOption(&oh, L"MarginsPrint", MOT_BINARY, &mo->rcPrintMargins, sizeof(RECT));
    ReadOption(&oh, L"PrintColor", MOT_DWORD, &mo->dwPrintColor, sizeof(DWORD));
    ReadOption(&oh, L"PrintHeaderEnable", MOT_DWORD, &mo->bPrintHeaderEnable, sizeof(DWORD));
    ReadOption(&oh, L"PrintHeader", MOT_STRING, mo->wszPrintHeader, sizeof(mo->wszPrintHeader));
    ReadOption(&oh, L"PrintFooterEnable", MOT_DWORD, &mo->bPrintFooterEnable, sizeof(DWORD));
    ReadOption(&oh, L"PrintFooter", MOT_STRING, mo->wszPrintFooter, sizeof(mo->wszPrintFooter));
    ReadOption(&oh, L"PrintFontEnable", MOT_DWORD, &mo->bPrintFontEnable, sizeof(DWORD));
    ReadOption(&oh, L"PrintFont", MOT_BINARY, &mo->lfPrintFont, offsetof(LOGFONTW, lfFaceName));
    ReadOption(&oh, L"PrintFontFace", MOT_STRING, mo->lfPrintFont.lfFaceName, sizeof(mo->lfPrintFont.lfFaceName));

    //Colors dialog
    ReadOption(&oh, L"ColorsDialog", MOT_BINARY, &mo->rcColorsCurrentDialog, sizeof(RECT));

    //Plugin dialog
    ReadOption(&oh, L"PluginsDialog", MOT_BINARY, &mo->rcPluginsCurrentDialog, sizeof(RECT));

    //Mdi list dialog
    if (mo->nMDI)
    {
      ReadOption(&oh, L"MdiListOptions", MOT_DWORD, &mo->dwMdiListOptions, sizeof(DWORD));
      ReadOption(&oh, L"MdiListDialog", MOT_BINARY, &mo->rcMdiListCurrentDialog, sizeof(RECT));
    }

    //Main window
    ReadOption(&oh, L"WindowStyle", MOT_DWORD, &mo->dwMainStyle, sizeof(DWORD));
    ReadOption(&oh, L"WindowPosition", MOT_BINARY, &mo->rcMainWindowRestored, sizeof(RECT));
    if (mo->nMDI)
    {
      ReadOption(&oh, L"WindowStyleMDI", MOT_DWORD, &mo->dwMdiStyle, sizeof(DWORD));
    }

    //Close handle
    if (oh.nSaveSettings == SS_REGISTRY)
    {
      RegCloseKey((HKEY)oh.hHandle);
      oh.hHandle=NULL;
    }
  }

  //Read and register plugins hotkeys
  RegisterPluginsHotkeys(mo);

  //Read theams
  ReadThemes(mo);

  //Free INI
  StackFreeIni(&hIniFile);
}

BOOL SaveOptions(MAINOPTIONS *mo, FRAMEDATA *fd, int nSaveSettings, BOOL bForceWrite)
{
  OPTIONHANDLE oh;
  BOOL bResult=FALSE;

  //Main window style
  mo->dwMainStyle=GetWindowLongWide(hMainWnd, GWL_STYLE);
  mo->dwMainStyle=((mo->dwMainStyle & WS_MAXIMIZE) || ((mo->dwMainStyle & WS_MINIMIZE) && dwLastMainSize == SIZE_MAXIMIZED))?WS_MAXIMIZE:0;

  //MDI frame style
  if (nMDI == WMD_MDI)
  {
    if (lpFrameCurrent->hWndEditParent) SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
    mo->dwMdiStyle=!bMdiMaximize?0:WS_MAXIMIZE;
  }

  if (!bForceWrite)
  {
    if (!xmemcmp(mo, &moInit, sizeof(MAINOPTIONS)) &&
        !xmemcmp(&fd->lf, &fdInit.lf, sizeof(FRAMEDATA) - offsetof(FRAMEDATA, lf)) &&
        fd->ei.bWordWrap == fdInit.ei.bWordWrap &&
        !bCodepageListChanged)
    {
      //Settings unchanged
      return TRUE;
    }
  }
  oh.mo=mo;
  oh.fd=fd;
  oh.nSaveSettings=nSaveSettings;
  oh.bForceWrite=bForceWrite;

  if (oh.nSaveSettings == SS_REGISTRY)
  {
    wchar_t wszRegKey[MAX_PATH];

    xprintfW(wszRegKey, L"%s\\Options", APP_REGHOMEW);
    if (RegCreateKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, (HKEY *)&oh.hHandle, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!OpenIniW(&hIniFile, wszIniFile, TRUE))
      return FALSE;
    if (!(oh.hHandle=(HANDLE)StackOpenIniSectionW(&hIniFile, L"Options", lstrlenW(L"Options"), TRUE)))
      goto Error;

    if (!SaveOption(&oh, L"SaveSettings", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, nSaveSettings), sizeof(DWORD)))
      goto Error;
  }

  //Manual
  if (!SaveOption(&oh, L"ShowModify", MOT_DWORD|MOT_MANUAL, &mo->dwShowModify, sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"StatusPosType", MOT_DWORD|MOT_MANUAL, &mo->dwStatusPosType, sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"StatusUserFormat", MOT_STRING|MOT_MANUAL, mo->wszStatusUserFormat, BytesInString(mo->wszStatusUserFormat)))
    goto Error;
  if (!SaveOption(&oh, L"WordBreak", MOT_DWORD|MOT_MANUAL, &mo->dwWordBreakCustom, sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"PaintOptions", MOT_DWORD|MOT_MANUAL, &mo->dwPaintOptions, sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"RichEditClass", MOT_DWORD|MOT_MANUAL, &mo->bRichEditClass, sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"DateLogFormat", MOT_STRING|MOT_MANUAL, mo->wszDateLogFormat, BytesInString(mo->wszDateLogFormat)))
    goto Error;
  if (!SaveOption(&oh, L"DateInsertFormat", MOT_STRING|MOT_MANUAL, mo->wszDateInsertFormat, BytesInString(mo->wszDateInsertFormat)))
    goto Error;

  //Frame data
  if (!SaveOption(&oh, L"WordWrap", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, ei.bWordWrap), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"UndoLimit", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, nUndoLimit), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"DetailedUndo", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, bDetailedUndo), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"WrapType", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, dwWrapType), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"WrapLimit", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, dwWrapLimit), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"Marker", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, dwMarker), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"CaretOptions", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, dwCaretOptions), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"CaretWidth", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, nCaretWidth), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"MouseOptions", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, dwMouseOptions), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"LineGap", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, dwLineGap), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"TabStopSize", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, nTabStopSize), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"TabStopAsSpaces", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, bTabStopAsSpaces), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"MarginsEdit", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, dwEditMargins), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"ShowURL", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, bShowURL), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"ClickURL", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, nClickURL), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"UrlPrefixesEnable", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, bUrlPrefixesEnable), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"UrlPrefixes", MOT_BINARY|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, wszUrlPrefixes), BytesInString(fd->wszUrlPrefixes)))
    goto Error;
  if (!SaveOption(&oh, L"UrlDelimitersEnable", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, bUrlDelimitersEnable), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"UrlLeftDelimiters", MOT_BINARY|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, wszUrlLeftDelimiters), BytesInString(fd->wszUrlLeftDelimiters)))
    goto Error;
  if (!SaveOption(&oh, L"UrlRightDelimiters", MOT_BINARY|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, wszUrlRightDelimiters), BytesInString(fd->wszUrlRightDelimiters)))
    goto Error;
  if (!SaveOption(&oh, L"WordDelimitersEnable", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, bWordDelimitersEnable), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"WordDelimiters", MOT_BINARY|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, wszWordDelimiters), BytesInString(fd->wszWordDelimiters)))
    goto Error;
  if (!SaveOption(&oh, L"WrapDelimitersEnable", MOT_DWORD|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, bWrapDelimitersEnable), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"WrapDelimiters", MOT_BINARY|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, wszWrapDelimiters), BytesInString(fd->wszWrapDelimiters)))
    goto Error;
  if (!SaveOption(&oh, L"Font", MOT_BINARY|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, lf), offsetof(LOGFONTW, lfFaceName)))
    goto Error;
  if (!SaveOption(&oh, L"FontFace", MOT_STRING|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, lf.lfFaceName), BytesInString(fd->lf.lfFaceName)))
    goto Error;
  if (!SaveOption(&oh, L"Colors", MOT_BINARY|MOT_FRAMEOFFSET, (void *)offsetof(FRAMEDATA, aec), sizeof(AECOLORS)))
    goto Error;

  //Menu settings
  if (!SaveOption(&oh, L"OnTop", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bOnTop), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"StatusBar", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bStatusBar), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"LanguageModule", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszLangModule), BytesInString(mo->wszLangModule)))
    goto Error;
  if (!SaveOption(&oh, L"KeepSpace", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bKeepSpace), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"WatchFile", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bWatchFile), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"SaveTime", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bSaveTime), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"SingleOpenFile", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bSingleOpenFile), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"SingleOpenProgram", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bSingleOpenProgram), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"MDI", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, nMDI), sizeof(DWORD)))
    goto Error;
  if (nMDI)
  {
    if (!SaveOption(&oh, L"TabOptionsMDI", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwTabOptionsMDI), sizeof(DWORD)))
      goto Error;
  }

  //Settings dialog
  if (bForceWrite || bCodepageListChanged)
  {
    bCodepageListChanged=FALSE;
    if (!SaveOption(&oh, L"CodepageList", MOT_BINARY, lpCodepageList, nCodepageListLen * sizeof(int)))
      goto Error;
  }
  if (!SaveOption(&oh, L"ExecuteCommand", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszCommand), BytesInString(mo->wszCommand)))
    goto Error;
  if (!SaveOption(&oh, L"ExecuteDirectory", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszWorkDir), BytesInString(mo->wszWorkDir)))
    goto Error;
  if (!SaveOption(&oh, L"DefaultCodepage", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, nDefaultCodePage), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"DefaultNewLine", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, nDefaultNewLine), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"CodepageRecognition", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwLangCodepageRecognition), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"CodepageRecognitionBuffer", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwCodepageRecognitionBuffer), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"SavePositions", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bSavePositions), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"SaveCodepages", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bSaveCodepages), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"RecentFiles", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, nRecentFiles), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"SearchStrings", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, nSearchStrings), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"FileTypesOpen", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszFileTypesOpen), BytesInString(mo->wszFileTypesOpen)))
    goto Error;
  if (!SaveOption(&oh, L"FileTypesEdit", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszFileTypesEdit), BytesInString(mo->wszFileTypesEdit)))
    goto Error;
  if (!SaveOption(&oh, L"FileTypesPrint", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszFileTypesPrint), BytesInString(mo->wszFileTypesPrint)))
    goto Error;
  if (!SaveOption(&oh, L"FileTypesAssociated", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwFileTypesAssociated), sizeof(DWORD)))
    goto Error;
  if (nMDI)
  {
    if (!SaveOption(&oh, L"KeybLayoutMDI", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bKeybLayoutMDI), sizeof(DWORD)))
      goto Error;
  }
  if (!SaveOption(&oh, L"DateLog", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bDateLog), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"SaveInReadOnlyMsg", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bSaveInReadOnlyMsg), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"DefaultSaveExt", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszDefaultSaveExt), BytesInString(mo->wszDefaultSaveExt)))
    goto Error;

  //Search dialog
  if (!SaveOption(&oh, L"SearchOptions", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwSearchOptions), sizeof(DWORD)))
    goto Error;

  //Open file dialog
  if (!SaveOption(&oh, L"LastDirectory", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszLastDir), BytesInString(mo->wszLastDir)))
    goto Error;

  //Print dialog
  if (!SaveOption(&oh, L"MarginsPrint", MOT_BINARY|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, rcPrintMargins), sizeof(RECT)))
    goto Error;
  if (!SaveOption(&oh, L"PrintColor", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwPrintColor), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"PrintHeaderEnable", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bPrintHeaderEnable), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"PrintHeader", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszPrintHeader), BytesInString(mo->wszPrintHeader)))
    goto Error;
  if (!SaveOption(&oh, L"PrintFooterEnable", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bPrintFooterEnable), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"PrintFooter", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, wszPrintFooter), BytesInString(mo->wszPrintFooter)))
    goto Error;
  if (!SaveOption(&oh, L"PrintFontEnable", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, bPrintFontEnable), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"PrintFont", MOT_BINARY|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, lfPrintFont), offsetof(LOGFONTW, lfFaceName)))
    goto Error;
  if (!SaveOption(&oh, L"PrintFontFace", MOT_STRING|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, lfPrintFont.lfFaceName), BytesInString(mo->lfPrintFont.lfFaceName)))
    goto Error;

  //Colors dialog
  if (!SaveOption(&oh, L"ColorsDialog", MOT_BINARY|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, rcColorsCurrentDialog), sizeof(RECT)))
    goto Error;

  //Plugin dialog
  if (!SaveOption(&oh, L"PluginsDialog", MOT_BINARY|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, rcPluginsCurrentDialog), sizeof(RECT)))
    goto Error;

  //Mdi list dialog
  if (nMDI)
  {
    if (!SaveOption(&oh, L"MdiListOptions", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwMdiListOptions), sizeof(DWORD)))
      goto Error;
    if (!SaveOption(&oh, L"MdiListDialog", MOT_BINARY|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, rcMdiListCurrentDialog), sizeof(RECT)))
      goto Error;
  }

  //Main window
  if (!SaveOption(&oh, L"WindowStyle", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwMainStyle), sizeof(DWORD)))
    goto Error;
  if (!SaveOption(&oh, L"WindowPosition", MOT_BINARY|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, rcMainWindowRestored), sizeof(RECT)))
    goto Error;
  if (nMDI)
  {
    if (!SaveOption(&oh, L"WindowStyleMDI", MOT_DWORD|MOT_MAINOFFSET, (void *)offsetof(MAINOPTIONS, dwMdiStyle), sizeof(DWORD)))
      goto Error;
  }

  if (oh.nSaveSettings == SS_REGISTRY)
    bResult=TRUE;
  else
    bResult=SaveIniW(&hIniFile, wszIniFile);

  if (bResult)
  {
    //Update initial options
    xmemcpy(&moInit, mo, sizeof(MAINOPTIONS));
    xmemcpy(&fdInit.lf, &fd->lf, sizeof(FRAMEDATA) - offsetof(FRAMEDATA, lf));
    fdInit.ei.bWordWrap=fd->ei.bWordWrap;
  }

  Error:
  if (oh.nSaveSettings == SS_REGISTRY)
    RegCloseKey((HKEY)oh.hHandle);
  else
    StackFreeIni(&hIniFile);
  return bResult;
}


//// Open/Save document

int OpenDocument(HWND hWnd, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM)
{
  wchar_t wszFile[MAX_PATH]=L"";
  HANDLE hFile;
  FILESTREAMDATA fsd;
  FRAMEDATA *lpFrame;
  HWND hWndFriend=NULL;
  int nResult=EOD_SUCCESS;
  int nDetect;
  int nFileCmp;
  BOOL bFileExist;

  if (!hWnd)
    hWnd=lpFrameCurrent->ei.hWndEdit;
  if (!hWnd)
  {
    //MDI has no windows
    DoFileNew();
    hWnd=lpFrameCurrent->ei.hWndEdit;
  }
  bFileExist=GetFullName(wpFile, wszFile, MAX_PATH);

  //Notification message
  if (GetWindowLongWide(hWnd, GWL_ID) == ID_EDIT)
  {
    NOPENDOCUMENT nod;
    char szFile[MAX_PATH];

    WideCharToMultiByte(CP_ACP, 0, wszFile, -1, szFile, MAX_PATH, NULL, NULL);
    nod.pFile=bOldWindows?(LPBYTE)szFile:(LPBYTE)wszFile;
    nod.szFile=szFile;
    nod.wszFile=wszFile;
    nod.nCodePage=&nCodePage;
    nod.bBOM=&bBOM;
    nod.dwFlags=&dwFlags;
    nod.bProcess=TRUE;
    SendMessage(hMainWnd, AKDN_OPENDOCUMENT_START, (WPARAM)hWnd, (LPARAM)&nod);
    if (!nod.bProcess)
    {
      nResult=EOD_STOP;
      goto End;
    }
  }

  if (!bFileExist)
  {
    //File doesn't exist
    if (dwCmdLineOptions & CLO_MSGOPENCREATEYES)
    {
    }
    else if (dwCmdLineOptions & CLO_MSGOPENCREATENO)
    {
      nResult=EOD_CANCEL;
      goto End;
    }
    else
    {
      LoadStringWide(hLangLib, MSG_FILE_DOES_NOT_EXIST, wbuf, BUFFER_SIZE);
      xprintfW(wbuf2, wbuf, wszFile);
      if (API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION) == IDCANCEL)
      {
        nResult=EOD_CANCEL;
        goto End;
      }
    }
    nCodePage=moCur.nDefaultCodePage;
    bBOM=bDefaultBOM;
  }
  else
  {
    if (IsEditActive(hWnd))
    {
      //File exists
      if (!(dwFlags & OD_REOPEN) && moCur.bSingleOpenFile)
      {
        if (!nMDI && xstrcmpiW(wszFile, lpFrameCurrent->wszFile))
        {
          if ((hWndFriend=FindWindowExWide(NULL, NULL, APP_SDI_CLASSW, wszFile)) &&
              (hWndFriend=GetParent(hWndFriend)))
          {
            ActivateWindow(hWndFriend);
            OpenDocumentSend(hWndFriend, NULL, wszFile, dwFlags, nCodePage, bBOM, FALSE);
            nResult=EOD_WINDOW_EXIST;
            goto End;
          }
        }
        if (nMDI)
        {
          if (lpFrame=StackFrameGetByName(&hFramesStack, wszFile, -1))
          {
            ActivateMdiFrameWindow(lpFrame, 0);
            hWnd=lpFrameCurrent->ei.hWndEdit;

            if (SaveChanged())
            {
              OpenDocument(hWnd, wszFile, dwFlags|OD_REOPEN, nCodePage, bBOM);
            }
            nResult=EOD_WINDOW_EXIST;
            goto End;
          }
        }
      }
      if (dwFlags & OD_REOPEN)
      {
        SendMessage(hWnd, AEM_GETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);
      }
    }

    //Autodetect code page
    if ((nDetect=AutodetectCodePage(wszFile, moCur.dwCodepageRecognitionBuffer, dwFlags, &nCodePage, &bBOM)) < 0)
    {
      if (!(dwFlags & OD_REOPEN))
      {
        if (nDetect == EDT_BINARY)
        {
          if (dwCmdLineOptions & CLO_MSGOPENBINARYYES)
          {
          }
          else if (dwCmdLineOptions & CLO_MSGOPENBINARYNO)
          {
            nResult=EOD_CANCEL;
            goto End;
          }
          else
          {
            LoadStringWide(hLangLib, MSG_ERROR_BINARY, wbuf, BUFFER_SIZE);
            xprintfW(wbuf2, wbuf, wszFile);
            if (API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDCANCEL)
            {
              nResult=EOD_CANCEL;
              goto End;
            }
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
    LoadStringWide(hLangLib, MSG_CP_UNIMPLEMENTED, wbuf, BUFFER_SIZE);
    xprintfW(wbuf2, wbuf, nCodePage);
    API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    nResult=EOD_CODEPAGE_ERROR;
    goto End;
  }

  //Open file
  if ((hFile=CreateFileWide(wszFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, bFileExist?OPEN_EXISTING:OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
  {
    nResult=EOD_OPEN;
    goto End;
  }

  //Offset BOM
  fsd.nBytesCurrent=0;

  if (bBOM)
  {
    if (nCodePage == CP_UNICODE_UTF16LE ||
        nCodePage == CP_UNICODE_UTF16BE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (nCodePage == CP_UNICODE_UTF32LE ||
             nCodePage == CP_UNICODE_UTF32BE)
    {
      SetFilePointer(hFile, 4, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=4;
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
    RecentFilesSaveCurrentFile();

    //Create edit window if necessary
    if (nMDI && !(dwFlags & OD_REOPEN) && (!lpFrameCurrent->hWndEditParent || lpFrameCurrent->ei.bModified || lpFrameCurrent->wszFile[0]))
    {
      DoFileNew();
      hWnd=lpFrameCurrent->ei.hWndEdit;
    }

    //Get file write time
    GetFileTime(hFile, NULL, NULL, &lpFrameCurrent->ft);
  }

  //Load text
  fsd.hWnd=hWnd;
  fsd.hFile=hFile;
  fsd.nCodePage=nCodePage;
  fsd.dwFlags=dwFlags;
  fsd.nNewLine=moCur.nDefaultNewLine;
  fsd.nBytesMax=-1;
  fsd.bResult=TRUE;
  FileStreamIn(&fsd);
  CloseHandle(hFile);

  if (IsEditActive(hWnd))
  {
    if (fsd.bResult)
    {
      //Compare paths
      nFileCmp=xstrcmpiW(lpFrameCurrent->wszFile, wszFile);

      if (nFileCmp || lpFrameCurrent->ei.nCodePage != nCodePage)
      {
        //Read position of the new document
        if (moCur.nRecentFiles)
        {
          RecentFilesZero();
          RecentFilesRead();
          RecentFilesUpdate(wszFile, -1, nCodePage);
          RecentFilesSave();
          if (nFileCmp) bMenuRecentFiles=TRUE;
        }
      }

      //Update titles
      SetNewLineStatus(lpFrameCurrent, fsd.nNewLine, AENL_INPUT);
      SetModifyStatus(lpFrameCurrent, FALSE);
      SetCodePageStatus(lpFrameCurrent, nCodePage, bBOM);

      if (nFileCmp)
      {
        UpdateTitle(lpFrameCurrent, wszFile);
        xstrcpynW(lpFrameCurrent->wszFile, wszFile, MAX_PATH);
        lpFrameCurrent->nFileLen=lstrlenW(lpFrameCurrent->wszFile);
        WideCharToMultiByte(CP_ACP, 0, lpFrameCurrent->wszFile, lpFrameCurrent->nFileLen + 1, lpFrameCurrent->szFile, MAX_PATH, NULL, NULL);
      }

      //.LOG
      if (moCur.bDateLog && !IsReadOnly(hWnd))
      {
        AECHARINDEX ciChar;
        wchar_t wszDate[128];
        wchar_t wszTime[128];
        wchar_t wszDateAndTime[MAX_PATH];

        if (SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ciChar))
        {
          if (!xstrcmpW(ciChar.lpLine->wpLine, L".LOG"))
          {
            if (!moCur.wszDateLogFormat[0])
            {
              GetDateFormatWide(LOCALE_USER_DEFAULT, 0, NULL, NULL, wszDate, 128);
              GetTimeFormatWide(LOCALE_USER_DEFAULT, 0, NULL, NULL, wszTime, 128);
              xprintfW(wszDateAndTime, L"\r%s %s\r", wszDate, wszTime);
            }
            else
            {
              GetTimeFormatWide(LOCALE_USER_DEFAULT, 0, NULL, moCur.wszDateLogFormat, wbuf, 128);
              GetDateFormatWide(LOCALE_USER_DEFAULT, 0, NULL, wbuf, wbuf2, 128);
              xprintfW(wszDateAndTime, L"\r%s\r", wbuf2);
            }
            SendMessage(hWnd, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
            ReplaceSelW(hWnd, wszDateAndTime, -1, FALSE, NULL, NULL);
            goto GlobalPrint;
          }
        }
      }

      //Update selection
      if (moCur.nRecentFiles && moCur.bSavePositions)
      {
        CHARRANGE cr;

        SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, TRUE);
        cr.cpMin=lpdwRecentPositions[0];
        cr.cpMax=lpdwRecentPositions[0];
        SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
        SendMessage(hWnd, AEM_LOCKSCROLL, SB_BOTH, FALSE);

        if ((dwFlags & OD_REOPEN))
          SendMessage(hWnd, AEM_SETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);
        else
          ScrollCaret(hWnd);
      }

      //Print if "/p" option used in command line
      GlobalPrint:
      if (dwCmdLineOptions & CLO_GLOBALPRINT)
      {
        DoFilePrint(lpFrameCurrent, TRUE);
        dwCmdLineOptions&=~CLO_GLOBALPRINT;

        if (!nMDI)
        {
          PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
        }
        else
        {
          DestroyMdiFrameWindow(lpFrameCurrent);
          //if (FrameNoWindows())
          //  PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
        }
      }
    }
    else nResult=EOD_STREAMIN;
  }

  End:
  if (GetWindowLongWide(hWnd, GWL_ID) == ID_EDIT)
  {
    SendMessage(hMainWnd, AKDN_OPENDOCUMENT_FINISH, (WPARAM)hWnd, nResult);
  }
  return nResult;
}

/*
//Read entire file (AEM_SETTEXTW).
void FileStreamIn(FILESTREAMDATA *lpData)
{
  unsigned char *szBuffer;
  wchar_t *wszBuffer;
  int nBufferBytes;
  DWORD dwFileSize;
  DWORD dwBytesRead;
  DWORD dwCharsConverted;
  DWORD i;

  if ((dwFileSize=GetFileSize(lpData->hFile, NULL)) != INVALID_FILE_SIZE)
  {
    if (lpData->nBytesMax == -1)
      lpData->nBytesMax=dwFileSize;

    if (IsCodePageUnicode(lpData->nCodePage) && lpData->nCodePage != CP_UNICODE_UTF8)
      nBufferBytes=lpData->nBytesMax;
    else
      nBufferBytes=lpData->nBytesMax * sizeof(wchar_t);

    if (wszBuffer=(wchar_t *)API_HeapAlloc(hHeap, 0, nBufferBytes + sizeof(wchar_t)))
    {
      if (IsCodePageUnicode(lpData->nCodePage) && lpData->nCodePage != CP_UNICODE_UTF8)
        szBuffer=(unsigned char *)wszBuffer;
      else
        szBuffer=(unsigned char *)wszBuffer + lpData->nBytesMax;

      //Read data from file
      if (API_ReadFile(lpData->hFile, szBuffer, lpData->nBytesMax, &dwBytesRead, NULL))
      {
        //Translate data to UNICODE
        if (lpData->nCodePage == CP_UNICODE_UTF16LE)
        {
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (lpData->nCodePage == CP_UNICODE_UTF16BE)
        {
          ChangeTwoBytesOrder(szBuffer, dwBytesRead);
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (lpData->nCodePage == CP_UNICODE_UTF32LE)
        {
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szBuffer, dwBytesRead / sizeof(unsigned long), NULL, wszBuffer, nBufferBytes / sizeof(wchar_t));
        }
        else if (lpData->nCodePage == CP_UNICODE_UTF32BE)
        {
          ChangeFourBytesOrder(szBuffer, dwBytesRead);
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szBuffer, dwBytesRead / sizeof(unsigned long), NULL, wszBuffer, nBufferBytes / sizeof(wchar_t));
        }
        else
        {
          if (lpData->nCodePage == CP_UNICODE_UTF8)
            dwCharsConverted=UTF8toUTF16(szBuffer, dwBytesRead, NULL, wszBuffer, nBufferBytes / sizeof(wchar_t));
          else
            dwCharsConverted=MultiByteToWideChar(lpData->nCodePage, 0, (char *)szBuffer, dwBytesRead, wszBuffer, nBufferBytes / sizeof(wchar_t));
        }
        wszBuffer[dwCharsConverted]='\0';

        //Detect new line
        for (i=0; i < dwCharsConverted; ++i)
        {
          if (wszBuffer[i] == '\r')
          {
            if (wszBuffer[i + 1] == '\r' && wszBuffer[i + 2] == '\n')
            {
              //Windows format \r\r\n
              lpData->nNewLine=NEWLINE_WIN;
              break;
            }
            else if (wszBuffer[i + 1] == '\n')
            {
              //Windows format \r\n
              lpData->nNewLine=NEWLINE_WIN;
              break;
            }

            //MacOS format \r
            lpData->nNewLine=NEWLINE_MAC;
            break;
          }
          else if (wszBuffer[i] == '\n')
          {
            //Unix format \n
            lpData->nNewLine=NEWLINE_UNIX;
            break;
          }
        }

        //Send to AkelEdit
        SendMessage(lpData->hWnd, AEM_SETNEWLINE, AENL_INPUT|AENL_OUTPUT, MAKELONG(AELB_ASIS, AELB_ASIS));
        SendMessage(lpData->hWnd, AEM_SETTEXTW, (WPARAM)dwCharsConverted, (LPARAM)wszBuffer);
      }
      else
      {
        lpData->bResult=FALSE;
        PostMessage(hMainWnd, WM_COMMAND, IDM_INTERNAL_ERRORIO_MSG, 0);
      }
      API_HeapFree(hHeap, 0, (LPVOID)wszBuffer);
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

DWORD CALLBACK InputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufBytesSize, DWORD *dwBufBytesDone)
{
  FILESTREAMDATA *lpData=(FILESTREAMDATA *)dwCookie;
  DWORD dwBytesRead;
  DWORD dwCharsConverted=0;
  DWORD dwBytesConverted=0;

  if (lpData->nBytesCurrent <= lpData->nBytesMax)
  {
    if (API_ReadFile(lpData->hFile, pcTranslateBuffer, dwBufBytesSize / sizeof(wchar_t), &dwBytesRead, NULL))
    {
      lpData->nBytesCurrent+=dwBytesRead;

      //Translate data to UNICODE
      if (lpData->nCodePage == CP_UNICODE_UTF16LE)
      {
        xmemcpy(wszBuf, pcTranslateBuffer, dwBytesRead);
        dwCharsConverted=dwBytesRead / sizeof(wchar_t);
      }
      else if (lpData->nCodePage == CP_UNICODE_UTF16BE)
      {
        ChangeTwoBytesOrder(pcTranslateBuffer, dwBytesRead);
        xmemcpy(wszBuf, pcTranslateBuffer, dwBytesRead);
        dwCharsConverted=dwBytesRead / sizeof(wchar_t);
      }
      else if (lpData->nCodePage == CP_UNICODE_UTF32LE)
      {
        dwCharsConverted=UTF32toUTF16((const unsigned long *)pcTranslateBuffer, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszBuf, dwBufBytesSize / sizeof(wchar_t));
      }
      else if (lpData->nCodePage == CP_UNICODE_UTF32BE)
      {
        ChangeFourBytesOrder(pcTranslateBuffer, dwBytesRead);
        dwCharsConverted=UTF32toUTF16((const unsigned long *)pcTranslateBuffer, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszBuf, dwBufBytesSize / sizeof(wchar_t));
      }
      else
      {
        if (lpData->nCodePage == CP_UNICODE_UTF8)
        {
          dwCharsConverted=UTF8toUTF16(pcTranslateBuffer, dwBytesRead, (unsigned int *)&dwBytesConverted, (unsigned short *)wszBuf, dwBufBytesSize / sizeof(wchar_t));

          if (dwBytesRead != dwBytesConverted)
          {
            //UTF-8 char was split
            lpData->nBytesCurrent+=dwBytesConverted - dwBytesRead;
            SetFilePointer(lpData->hFile, lpData->nBytesCurrent, NULL, FILE_BEGIN);
          }
        }
        else
        {
          dwCharsConverted=MultiByteToWideChar(lpData->nCodePage, 0, (char *)pcTranslateBuffer, dwBytesRead, wszBuf, dwBufBytesSize / sizeof(wchar_t));

          if (dwCharsConverted > 0)
          {
            if ((pcTranslateBuffer[dwBytesRead - 1] != '\0' && wszBuf[dwCharsConverted - 1] == '\0') || //Windows 95/98/Me/2000/XP/2003
                (pcTranslateBuffer[dwBytesRead - 1] != '?' && wszBuf[dwCharsConverted - 1] == '?'))     //Windows Vista/7/2008
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
    else PostMessage(hMainWnd, WM_COMMAND, IDM_INTERNAL_ERRORIO_MSG, 0);
  }
  *dwBufBytesDone=dwCharsConverted * sizeof(wchar_t);

  return 0;
}

DWORD ReadFileContent(HANDLE hFile, DWORD dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wpContent)
{
  unsigned char *szBuffer;
  wchar_t *wszBuffer=NULL;
  DWORD dwBufferBytes;
  DWORD dwFileSize;
  DWORD dwBytesRead;
  DWORD dwCharsConverted=0;

  //Offset BOM
  if (bBOM)
  {
    if (SetFilePointer(hFile, 0, NULL, FILE_CURRENT) == 0)
    {
      if (nCodePage == CP_UNICODE_UTF16LE ||
          nCodePage == CP_UNICODE_UTF16BE)
      {
        SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      }
      else if (nCodePage == CP_UNICODE_UTF32LE ||
               nCodePage == CP_UNICODE_UTF32BE)
      {
        SetFilePointer(hFile, 4, NULL, FILE_BEGIN);
      }
      else if (nCodePage == CP_UNICODE_UTF8)
      {
        SetFilePointer(hFile, 3, NULL, FILE_BEGIN);
      }
    }
  }

  if ((dwFileSize=GetFileSize(hFile, NULL)) != INVALID_FILE_SIZE)
  {
    if (dwBytesMax == (DWORD)-1)
      dwBytesMax=dwFileSize;

    if (IsCodePageUnicode(nCodePage) && nCodePage != CP_UNICODE_UTF8)
      dwBufferBytes=dwBytesMax;
    else
      dwBufferBytes=dwBytesMax * sizeof(wchar_t);

    if (wszBuffer=(wchar_t *)API_HeapAlloc(hHeap, 0, dwBufferBytes + sizeof(wchar_t)))
    {
      if (IsCodePageUnicode(nCodePage) && nCodePage != CP_UNICODE_UTF8)
        szBuffer=(unsigned char *)wszBuffer;
      else
        szBuffer=(unsigned char *)wszBuffer + dwBytesMax;

      //Read data from file
      if (API_ReadFile(hFile, szBuffer, dwBytesMax, &dwBytesRead, NULL))
      {
        //Translate data to UNICODE
        if (nCodePage == CP_UNICODE_UTF16LE)
        {
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (nCodePage == CP_UNICODE_UTF16BE)
        {
          ChangeTwoBytesOrder(szBuffer, dwBytesRead);
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (nCodePage == CP_UNICODE_UTF32LE)
        {
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szBuffer, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszBuffer, dwBufferBytes / sizeof(wchar_t));
        }
        else if (nCodePage == CP_UNICODE_UTF32BE)
        {
          ChangeFourBytesOrder(szBuffer, dwBytesRead);
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szBuffer, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszBuffer, dwBufferBytes / sizeof(wchar_t));
        }
        else
        {
          if (nCodePage == CP_UNICODE_UTF8)
            dwCharsConverted=UTF8toUTF16(szBuffer, dwBytesRead, NULL, (unsigned short *)wszBuffer, dwBufferBytes / sizeof(wchar_t));
          else
            dwCharsConverted=MultiByteToWideChar(nCodePage, 0, (char *)szBuffer, dwBytesRead, wszBuffer, dwBufferBytes / sizeof(wchar_t));
        }
        wszBuffer[dwCharsConverted]='\0';
      }
    }
  }
  *wpContent=wszBuffer;
  return dwCharsConverted;
}

BOOL OpenDocumentSend(HWND hWnd, HWND hWndEditCtrl, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess)
{
  OPENDOCUMENTPOSTW odp;
  COPYDATASTRUCT cds;

  odp.dwFlags=dwFlags;
  odp.nCodePage=nCodePage;
  odp.bBOM=bBOM;
  odp.hWnd=hWndEditCtrl;
  xstrcpynW(odp.szFile, wpFile, MAX_PATH);
  if (bOtherProcess)
    GetCurrentDirectoryWide(MAX_PATH, odp.szWorkDir);
  else
    odp.szWorkDir[0]='\0';

  cds.dwData=CD_OPENDOCUMENTW;
  cds.cbData=sizeof(OPENDOCUMENTPOSTW);
  cds.lpData=&odp;
  return SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
}

int SaveDocument(HWND hWnd, const wchar_t *wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags)
{
  wchar_t wszFile[MAX_PATH];
  WIN32_FIND_DATAW wfdW;
  HANDLE hFile;
  FILESTREAMDATA fsd;
  DWORD dwAttr;
  unsigned int nBytesWritten;
  int nResult=ESD_SUCCESS;
  int nWrite=0;
  int nFileCmp;
  int nCodePageCmp;
  int nLine=0;

  if (!wpFile[0])
  {
    SetCodePageStatus(lpFrameCurrent, nCodePage, bBOM);
    return nResult;
  }
  if (!hWnd)
    hWnd=lpFrameCurrent->ei.hWndEdit;
  GetFullName(wpFile, wszFile, MAX_PATH);

  //Notification message
  if (GetWindowLongWide(hWnd, GWL_ID) == ID_EDIT)
  {
    NSAVEDOCUMENT nsd;
    char szFile[MAX_PATH];

    WideCharToMultiByte(CP_ACP, 0, wszFile, -1, szFile, MAX_PATH, NULL, NULL);
    nsd.pFile=bOldWindows?(LPBYTE)szFile:(LPBYTE)wszFile;
    nsd.szFile=szFile;
    nsd.wszFile=wszFile;
    nsd.nCodePage=&nCodePage;
    nsd.bBOM=&bBOM;
    nsd.dwFlags=dwFlags;
    nsd.bProcess=TRUE;
    SendMessage(hMainWnd, AKDN_SAVEDOCUMENT_START, (WPARAM)hWnd, (LPARAM)&nsd);
    if (!nsd.bProcess)
    {
      nResult=ESD_STOP;
      goto End;
    }
  }

  //Check code page
  if (!IsCodePageValid(nCodePage))
  {
    if (!IsEditActive(hWnd))
      SetFocus(hWnd);
    LoadStringWide(hLangLib, MSG_CP_UNIMPLEMENTED, wbuf, BUFFER_SIZE);
    xprintfW(wbuf2, wbuf, nCodePage);
    API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    nResult=ESD_CODEPAGE_ERROR;
    goto End;
  }
  if (!IsCodePageUnicode(nCodePage))
  {
    if (nLine=SendMessage(hWnd, AEM_CHECKCODEPAGE, (WPARAM)nCodePage, 0))
    {
      if (dwCmdLineOptions & CLO_MSGSAVELOSTSYMBOLSYES)
      {
      }
      else if (dwCmdLineOptions & CLO_MSGSAVELOSTSYMBOLSNO)
      {
        nResult=ESD_CODEPAGE_ERROR;
        goto End;
      }
      else
      {
        if (!IsEditActive(hWnd))
          SetFocus(hWnd);
        if (!(moCur.dwStatusPosType & SPT_LINEWRAP) && lpFrameCurrent->ei.bWordWrap)
          nLine=SendMessage(hWnd, AEM_GETUNWRAPLINE, nLine - 1, 0) + 1;
        LoadStringWide(hLangLib, MSG_CP_MISMATCH, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, nLine);
        if (API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDCANCEL)
        {
          nResult=ESD_CODEPAGE_ERROR;
          goto End;
        }
      }
    }
  }

  dwAttr=GetFileAttributesWide(wszFile);

  if (dwAttr != INVALID_FILE_ATTRIBUTES)
  {
    if (moCur.bSaveInReadOnlyMsg && (dwAttr & FILE_ATTRIBUTE_READONLY))
    {
      if (!IsEditActive(hWnd))
        SetFocus(hWnd);
      LoadStringWide(hLangLib, MSG_SAVEIN_READONLY, wbuf, BUFFER_SIZE);
      xprintfW(wbuf2, wbuf, wszFile);
      if (API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_YESNO|MB_ICONEXCLAMATION) == IDNO)
      {
        nResult=ESD_READONLY;
        goto End;
      }
    }
    if ((dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
      SetFileAttributesWide(wszFile, dwAttr & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN & ~FILE_ATTRIBUTE_SYSTEM);
    if (moCur.bSaveTime)
    {
      if ((hFile=FindFirstFileWide(wszFile, &wfdW)) != INVALID_HANDLE_VALUE)
        FindClose(hFile);
    }
  }

  hFile=CreateFileWide(wszFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, (dwAttr != INVALID_FILE_ATTRIBUTES)?TRUNCATE_EXISTING:CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile == INVALID_HANDLE_VALUE)
  {
    nResult=ESD_OPEN;
    goto BackAttr;
  }

  if (bBOM)
  {
    if (IsCodePageUnicode(nCodePage))
    {
      if (nCodePage == CP_UNICODE_UTF16LE)
        nWrite=API_WriteFile(hFile, "\xFF\xFE", 2, (DWORD *)&nBytesWritten, NULL);
      else if (nCodePage == CP_UNICODE_UTF16BE)
        nWrite=API_WriteFile(hFile, "\xFE\xFF", 2, (DWORD *)&nBytesWritten, NULL);
      else if (nCodePage == CP_UNICODE_UTF32LE)
        nWrite=API_WriteFile(hFile, "\xFF\xFE\x00\x00", 4, (DWORD *)&nBytesWritten, NULL);
      else if (nCodePage == CP_UNICODE_UTF32BE)
        nWrite=API_WriteFile(hFile, "\x00\x00\xFE\xFF", 4, (DWORD *)&nBytesWritten, NULL);
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
        SetFileAttributesWide(wszFile, dwAttr|FILE_ATTRIBUTE_ARCHIVE);
      if (moCur.bSaveTime)
      {
        if ((hFile=CreateFileWide(wszFile, FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL)) != INVALID_HANDLE_VALUE)
        {
          SetFileTime(hFile, NULL, NULL, &wfdW.ftLastWriteTime);
          CloseHandle(hFile);
        }
      }
    }

    //Update file info
    if (dwFlags & SD_UPDATE)
    {
      if (IsEditActive(hWnd))
      {
        //Compare
        nFileCmp=xstrcmpiW(lpFrameCurrent->wszFile, wszFile);
        nCodePageCmp=lpFrameCurrent->ei.nCodePage - nCodePage;

        GetFileWriteTimeWide(wszFile, &lpFrameCurrent->ft);
        SetModifyStatus(lpFrameCurrent, FALSE);
        SetCodePageStatus(lpFrameCurrent, nCodePage, bBOM);

        if (nFileCmp)
        {
          UpdateTitle(lpFrameCurrent, wszFile);
          xstrcpynW(lpFrameCurrent->wszFile, wszFile, MAX_PATH);
          lpFrameCurrent->nFileLen=lstrlenW(lpFrameCurrent->wszFile);
          WideCharToMultiByte(CP_ACP, 0, lpFrameCurrent->wszFile, lpFrameCurrent->nFileLen + 1, lpFrameCurrent->szFile, MAX_PATH, NULL, NULL);
        }
        if (nFileCmp || nCodePageCmp)
          RecentFilesSaveCurrentFile();

        if ((dwFlags & SD_SELECTION) || nLine)
        {
          OpenDocument(hWnd, lpFrameCurrent->wszFile, OD_REOPEN, lpFrameCurrent->ei.nCodePage, lpFrameCurrent->ei.bBOM);
        }
      }
      else
      {
        FILETIME ft;
        FRAMEDATA *lpFrame;

        if (lpFrame=GetFrameDataFromEditWindow(hWnd))
        {
          //Compare
          nFileCmp=xstrcmpiW(lpFrame->wszFile, wszFile);

          GetFileWriteTimeWide(wszFile, &ft);
          SetModifyStatus(lpFrame, FALSE);
          lpFrame->ei.nCodePage=nCodePage;
          lpFrame->ei.bBOM=bBOM;
          lpFrame->ft=ft;

          if (nFileCmp)
          {
            UpdateTitle(lpFrame, wszFile);
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
  if (dwAttr != INVALID_FILE_ATTRIBUTES)
  {
    if ((dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
      SetFileAttributesWide(wszFile, dwAttr);
  }

  End:
  if (GetWindowLongWide(hWnd, GWL_ID) == ID_EDIT)
  {
    SendMessage(hMainWnd, AKDN_SAVEDOCUMENT_FINISH, (WPARAM)hWnd, nResult);
  }
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

DWORD CALLBACK OutputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufBytesSize, DWORD *dwBufBytesDone)
{
  FILESTREAMDATA *lpData=(FILESTREAMDATA *)dwCookie;
  unsigned char *pDataToWrite=(unsigned char *)wszBuf;
  DWORD dwBytesToWrite=dwBufBytesSize;

  if (lpData->nCodePage == CP_UNICODE_UTF16LE)
  {
  }
  else if (lpData->nCodePage == CP_UNICODE_UTF16BE)
  {
    ChangeTwoBytesOrder((unsigned char *)wszBuf, dwBytesToWrite);
  }
  else if (lpData->nCodePage == CP_UNICODE_UTF32LE)
  {
    dwBytesToWrite=UTF16toUTF32((const unsigned short *)wszBuf, dwBufBytesSize / sizeof(wchar_t), NULL, (unsigned long *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE / sizeof(unsigned long)) * sizeof(unsigned long);
    pDataToWrite=pcTranslateBuffer;
  }
  else if (lpData->nCodePage == CP_UNICODE_UTF32BE)
  {
    dwBytesToWrite=UTF16toUTF32((const unsigned short *)wszBuf, dwBufBytesSize / sizeof(wchar_t), NULL, (unsigned long *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE / sizeof(unsigned long)) * sizeof(unsigned long);
    ChangeFourBytesOrder(pcTranslateBuffer, dwBytesToWrite);
    pDataToWrite=pcTranslateBuffer;
  }
  else
  {
    if (lpData->nCodePage == CP_UNICODE_UTF8)
      dwBytesToWrite=UTF16toUTF8((const unsigned short *)wszBuf, dwBufBytesSize / sizeof(wchar_t), NULL, pcTranslateBuffer, TRANSLATE_BUFFER_SIZE);
    else
      dwBytesToWrite=WideCharToMultiByte(lpData->nCodePage, 0, wszBuf, dwBufBytesSize / sizeof(wchar_t), (char *)pcTranslateBuffer, TRANSLATE_BUFFER_SIZE, NULL, NULL);
    pDataToWrite=pcTranslateBuffer;
  }
  return !API_WriteFile(lpData->hFile, pDataToWrite, dwBytesToWrite, dwBufBytesDone, NULL);
}

BOOL OpenDirectory(wchar_t *wpPath, BOOL bSubDir)
{
  wchar_t wszName[MAX_PATH];
  WIN32_FIND_DATAW wfdW;
  HANDLE hSearch;

  if (bSubDir)
  {
    xprintfW(wszName, L"%s\\*.*", wpPath);

    if ((hSearch=FindFirstFileWide(wszName, &wfdW)) != INVALID_HANDLE_VALUE)
    {
      do
      {
        if (wfdW.cFileName[0] == '.' && (wfdW.cFileName[1] == '\0' || (wfdW.cFileName[1] == '.' && wfdW.cFileName[2] == '\0'))) continue;

        xprintfW(wszName, L"%s\\%s", wpPath, wfdW.cFileName);

        if (wfdW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
          OpenDirectory(wszName, TRUE);
        else
          OpenDocument(NULL, wszName, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE);
      }
      while (FindNextFileWide(hSearch, &wfdW));

      FindClose(hSearch);
    }
    else return FALSE;
  }
  return TRUE;
}

void DropFiles(HDROP hDrop)
{
  wchar_t wszFile[MAX_PATH];
  wchar_t wszString[MAX_PATH];
  int nDropped;
  int i;

  if (moCur.bStatusBar)
    LoadStringWide(hLangLib, STR_COUNT, wszString, MAX_PATH);

  if (nMDI || SaveChanged())
  {
    nDropped=DragQueryFileWide(hDrop, 0xFFFFFFFF, NULL, 0);

    for (i=0; i < nDropped; ++i)
    {
      DragQueryFileWide(hDrop, i, wszFile, MAX_PATH);
      if (nMDI && IsFile(wszFile) == ERROR_FILE_NOT_FOUND)
        OpenDirectory(wszFile, TRUE);
      else
        OpenDocument(NULL, wszFile, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE);
      if (nMDI == WMD_SDI) break;

      //Status update
      if (moCur.bStatusBar)
      {
        MSG msg;

        xprintfW(wbuf, wszString, i + 1, nDropped);
        StatusBar_SetTextWide(hStatus, STATUS_MODIFY, wbuf);

        while (PeekMessageWide(&msg, hStatus, 0, 0, PM_REMOVE))
        {
          TranslateMessage(&msg);
          DispatchMessageWide(&msg);
        }
      }
    }

    if (moCur.bStatusBar)
      StatusBar_SetTextWide(hStatus, STATUS_MODIFY, L"");
  }
  DragFinish(hDrop);
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void CheckModificationTime(FRAMEDATA *lpFrame)
{
  if (moCur.bWatchFile && lpFrame->wszFile[0] && (lpFrame->ft.dwLowDateTime || lpFrame->ft.dwHighDateTime))
  {
    FILETIME ftTmp;

    if (!FileExistsWide(lpFrame->wszFile))
    {
      xmemset(&lpFrame->ft, 0, sizeof(FILETIME));

      if (GetCapture())
        ReleaseCapture();
      SendMessage(lpFrame->ei.hWndEdit, AEM_DRAGDROP, AEDD_STOPDRAG, 0);
      SendMessage(hMainWnd, WM_COMMAND, IDM_INTERNAL_CANTOPEN_MSG, (LPARAM)lpFrame);
    }
    else if (GetFileWriteTimeWide(lpFrame->wszFile, &ftTmp))
    {
      if (CompareFileTime(&lpFrame->ft, &ftTmp))
      {
        lpFrame->ft=ftTmp;

        if (!bReopenMsg)
        {
          bReopenMsg=TRUE;

          if (GetCapture())
            ReleaseCapture();
          SendMessage(lpFrame->ei.hWndEdit, AEM_DRAGDROP, AEDD_STOPDRAG, 0);
          SendMessage(hMainWnd, WM_COMMAND, IDM_INTERNAL_REOPEN_MSG, (LPARAM)lpFrame);
        }
      }
    }
    else xmemset(&lpFrame->ft, 0, sizeof(FILETIME));
  }
}

BOOL CALLBACK SaveAllAsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCodePageCheck;
  static HWND hWndCodePageList;
  static HWND hWndBOM;
  static HWND hWndNewLineCheck;
  static HWND hWndNewLineWin;
  static HWND hWndNewLineUnix;
  static HWND hWndNewLineMac;
  static HWND hWndOK;
  static BOOL bCodePageEnable=TRUE;
  static BOOL bNewLineEnable=FALSE;
  static int nNewLine=-1;
  static int nCodePage=-1;
  static BOOL bBOM;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndCodePageCheck=GetDlgItem(hDlg, IDC_SAVEALLAS_CODEPAGE_CHECK);
    hWndCodePageList=GetDlgItem(hDlg, IDC_SAVEALLAS_CODEPAGE_LIST);
    hWndBOM=GetDlgItem(hDlg, IDC_SAVEALLAS_BOM);
    hWndNewLineCheck=GetDlgItem(hDlg, IDC_SAVEALLAS_NEWLINE_CHECK);
    hWndNewLineWin=GetDlgItem(hDlg, IDC_SAVEALLAS_NEWLINE_WIN);
    hWndNewLineUnix=GetDlgItem(hDlg, IDC_SAVEALLAS_NEWLINE_UNIX);
    hWndNewLineMac=GetDlgItem(hDlg, IDC_SAVEALLAS_NEWLINE_MAC);
    hWndOK=GetDlgItem(hDlg, IDOK);

    if (nNewLine < 0)
      nNewLine=moCur.nDefaultNewLine;
    if (nCodePage < 0)
      nCodePage=moCur.nDefaultCodePage;

    FillComboboxCodepage(hWndCodePageList, lpCodepageList);
    SelectComboboxCodepage(hWndCodePageList, nCodePage);

    if (IsCodePageUnicode(nCodePage))
    {
      if (bBOM)
        SendMessage(hWndBOM, BM_SETCHECK, BST_CHECKED, 0);
    }
    if (bCodePageEnable)
      SendMessage(hWndCodePageCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (bNewLineEnable)
      SendMessage(hWndNewLineCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (nNewLine == NEWLINE_WIN)
      SendMessage(hWndNewLineWin, BM_SETCHECK, BST_CHECKED, 0);
    else if (nNewLine == NEWLINE_UNIX)
      SendMessage(hWndNewLineUnix, BM_SETCHECK, BST_CHECKED, 0);
    else if (nNewLine == NEWLINE_MAC)
      SendMessage(hWndNewLineMac, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hDlg, WM_COMMAND, IDC_SAVEALLAS_CODEPAGE_CHECK, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SAVEALLAS_CODEPAGE_CHECK ||
        LOWORD(wParam) == IDC_SAVEALLAS_NEWLINE_CHECK)
    {
      bCodePageEnable=SendMessage(hWndCodePageCheck, BM_GETCHECK, 0, 0);
      bNewLineEnable=SendMessage(hWndNewLineCheck, BM_GETCHECK, 0, 0);

      if (!bCodePageEnable && !bNewLineEnable)
        EnableWindow(hWndOK, FALSE);
      else
        EnableWindow(hWndOK, TRUE);
      EnableWindow(hWndCodePageList, bCodePageEnable);
      EnableWindow(hWndBOM, bCodePageEnable);

      if (bCodePageEnable)
      {
        if (!IsCodePageUnicode(nCodePage))
        {
          EnableWindow(hWndBOM, FALSE);
        }
      }
      EnableWindow(hWndNewLineWin, bNewLineEnable);
      EnableWindow(hWndNewLineUnix, bNewLineEnable);
      EnableWindow(hWndNewLineMac, bNewLineEnable);
    }
    else if (LOWORD(wParam) == IDC_SAVEALLAS_CODEPAGE_LIST && HIWORD(wParam) == CBN_SELCHANGE)
    {
      nCodePage=GetDlgItemInt(hDlg, IDC_SAVEALLAS_CODEPAGE_LIST, NULL, FALSE);

      if (!IsCodePageUnicode(nCodePage))
      {
        SendMessage(hWndBOM, BM_SETCHECK, BST_UNCHECKED, 0);
        EnableWindow(hWndBOM, FALSE);
      }
      else
      {
        EnableWindow(hWndBOM, TRUE);
        SendMessage(hWndBOM, BM_SETCHECK, BST_CHECKED, 0);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      bBOM=SendMessage(hWndBOM, BM_GETCHECK, 0, 0);

      if (SendMessage(hWndNewLineWin, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nNewLine=NEWLINE_WIN;
      else if (SendMessage(hWndNewLineUnix, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nNewLine=NEWLINE_UNIX;
      else if (SendMessage(hWndNewLineMac, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nNewLine=NEWLINE_MAC;

      EndDialog(hDlg, 0);

      //Save documents
      if (lpFrameCurrent->hWndEditParent || !nMDI)
      {
        FRAMEDATA *lpFrameInit=lpFrameCurrent;

        do
        {
          if ((bNewLineEnable && (nNewLine != lpFrameCurrent->ei.nNewLine)) ||
              (bCodePageEnable && (nCodePage != lpFrameCurrent->ei.nCodePage || bBOM != lpFrameCurrent->ei.bBOM || !lpFrameCurrent->wszFile[0] || lpFrameCurrent->ei.bModified || !FileExistsWide(lpFrameCurrent->wszFile))))
          {
            if (bNewLineEnable && nNewLine != lpFrameCurrent->ei.nNewLine)
              SetNewLineStatus(lpFrameCurrent, nNewLine, AENL_INPUT|AENL_OUTPUT);

            if (!lpFrameCurrent->wszFile[0])
            {
              if (!DoFileSaveAs(bCodePageEnable?nCodePage:lpFrameCurrent->ei.nCodePage, bCodePageEnable?bBOM:lpFrameCurrent->ei.bBOM))
                break;
            }
            else
            {
              if (SaveDocument(NULL, lpFrameCurrent->wszFile, bCodePageEnable?nCodePage:lpFrameCurrent->ei.nCodePage, bCodePageEnable?bBOM:lpFrameCurrent->ei.bBOM, SD_UPDATE) != ESD_SUCCESS)
                break;
            }
          }
          if (!nMDI) break;

          lpFrameCurrent=NextMdiFrameWindow(lpFrameCurrent, FALSE);
        }
        while (lpFrameCurrent != lpFrameInit);
      }
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


//// Print

unsigned int CALLBACK PrintPageSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFontGroup;
  static HWND hWndFontCheck;
  static HWND hWndFontButton;
  static HWND hWndColorGroup;
  static HWND hWndColorTextCheck;
  static HWND hWndColorBkCheck;
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
  static LOGFONTW lfDialog;
  BOOL bState;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    RECT rcDlg;
    RECT rcControl;
    HWND hWndOK;
    HWND hWndCancel;
    HWND hWndPrinter;
    int nExtend=155;

    if (bOldWindows)
      hBitmapDownArrow=(HBITMAP)API_LoadImageA(hLangLib, MAKEINTRESOURCEA(IDB_BITMAP_DOWNARROW), IMAGE_BITMAP, 0, 0, 0);
    else
      hBitmapDownArrow=(HBITMAP)API_LoadImageW(hLangLib, MAKEINTRESOURCEW(IDB_BITMAP_DOWNARROW), IMAGE_BITMAP, 0, 0, 0);
    xmemcpy(&lfDialog, &moCur.lfPrintFont, sizeof(LOGFONTW));
    hPrintFont=(HFONT)CreateFontIndirectWide(&lfDialog);
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
    hWndFontGroup=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                         12, rcControl.top - 6, 96, 70,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_GROUP,
                         hInstance,
                         NULL);
    //Font checkbox
    hWndFontCheck=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,
                         24, rcControl.top + 26, 16, 16,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_CHECK,
                         hInstance,
                         NULL);
    //Font button
    hWndFontButton=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP,
                         46, rcControl.top + 22, 46, 23,
                         hDlg,
                         (HMENU)IDC_PSD_FONT_BUTTON,
                         hInstance,
                         NULL);
    //Color group box
    hWndColorGroup=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                         120, rcControl.top - 6, 228, 70,
                         hDlg,
                         (HMENU)IDC_PSD_COLOR_GROUP,
                         hInstance,
                         NULL);
    //Color text checkbox
    hWndColorTextCheck=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,
                         136, rcControl.top + 16, 200, 16,
                         hDlg,
                         (HMENU)IDC_PSD_COLOR_TEXT_CHECK,
                         hInstance,
                         NULL);
    //Color background checkbox
    hWndColorBkCheck=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX,
                         136, rcControl.top + 36, 200, 16,
                         hDlg,
                         (HMENU)IDC_PSD_COLOR_BACKGROUND_CHECK,
                         hInstance,
                         NULL);
    //Headline group box
    hWndHeadlineGroup=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                         12, rcControl.top + 71, 336, 70,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_GROUP,
                         hInstance,
                         NULL);
    //Header checkbox
    hWndHeaderCheck=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|BS_RIGHTBUTTON,
                         18, rcControl.top + 93, 76, 16,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_CHECK,
                         hInstance,
                         NULL);
    //Header edit
    hWndHeaderEdit=CreateWindowExWide(WS_EX_CLIENTEDGE,
                         L"EDIT",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
                         102, rcControl.top + 91, 216, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_EDIT,
                         hInstance,
                         NULL);
    //Header help
    hWndHeaderHelp=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_BITMAP,
                         320, rcControl.top + 91, 18, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_HEADER_HELP,
                         hInstance,
                         NULL);
    //Footer checkbox
    hWndFooterCheck=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|BS_RIGHTBUTTON,
                         18, rcControl.top + 113, 76, 16,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_CHECK,
                         hInstance,
                         NULL);
    //Footer edit
    hWndFooterEdit=CreateWindowExWide(WS_EX_CLIENTEDGE,
                         L"EDIT",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_AUTOHSCROLL,
                         102, rcControl.top + 111, 216, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_EDIT,
                         hInstance,
                         NULL);
    //Footer help
    hWndFooterHelp=CreateWindowExWide(0,
                         L"BUTTON",
                         NULL,
                         WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_BITMAP,
                         320, rcControl.top + 111, 18, 18,
                         hDlg,
                         (HMENU)IDC_PSD_HEADLINE_FOOTER_HELP,
                         hInstance,
                         NULL);

    SendMessage(hWndFontGroup, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    LoadStringWide(hLangLib, STR_FONT, wbuf, BUFFER_SIZE);
    SetWindowTextWide(hWndFontGroup, wbuf);
    if (moCur.bPrintFontEnable)
      SendMessage(hWndFontCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndFontButton, FALSE);
    SendMessage(hWndFontButton, WM_SETFONT, (WPARAM)hPrintFont, TRUE);
    SetWindowTextWide(hWndFontButton, L"...");

    SendMessage(hWndColorGroup, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    LoadStringWide(hLangLib, STR_COLOR, wbuf, BUFFER_SIZE);
    SetWindowTextWide(hWndColorGroup, wbuf);

    SendMessage(hWndColorTextCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    LoadStringWide(hLangLib, STR_TEXT, wbuf, BUFFER_SIZE);
    SetWindowTextWide(hWndColorTextCheck, wbuf);
    if (moCur.dwPrintColor & PRNC_TEXT)
      SendMessage(hWndColorTextCheck, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hWndColorBkCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    LoadStringWide(hLangLib, STR_BACKGROUND, wbuf, BUFFER_SIZE);
    SetWindowTextWide(hWndColorBkCheck, wbuf);
    if (moCur.dwPrintColor & PRNC_BACKGROUND)
      SendMessage(hWndColorBkCheck, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hWndHeadlineGroup, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    LoadStringWide(hLangLib, STR_HEADLINE, wbuf, BUFFER_SIZE);
    SetWindowTextWide(hWndHeadlineGroup, wbuf);

    SendMessage(hWndHeaderCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    LoadStringWide(hLangLib, STR_TOP, wbuf, BUFFER_SIZE);
    SetWindowTextWide(hWndHeaderCheck, wbuf);
    if (moCur.bPrintHeaderEnable)
      SendMessage(hWndHeaderCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      EnableWindow(hWndHeaderEdit, FALSE);
      EnableWindow(hWndHeaderHelp, FALSE);
    }
    SendMessage(hWndHeaderEdit, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndHeaderEdit, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    SetWindowTextWide(hWndHeaderEdit, moCur.wszPrintHeader);
    SendMessage(hWndHeaderHelp, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapDownArrow);

    SendMessage(hWndFooterCheck, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    LoadStringWide(hLangLib, STR_BOTTOM, wbuf, BUFFER_SIZE);
    SetWindowTextWide(hWndFooterCheck, wbuf);
    if (moCur.bPrintFooterEnable)
      SendMessage(hWndFooterCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      EnableWindow(hWndFooterEdit, FALSE);
      EnableWindow(hWndFooterHelp, FALSE);
    }
    SendMessage(hWndFooterEdit, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndFooterEdit, WM_SETFONT, (WPARAM)hGuiFont, TRUE);
    SetWindowTextWide(hWndFooterEdit, moCur.wszPrintFooter);
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
        else if (i == IDM_POPUP_HEADLINE_FILENAME)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%f");
        else if (i == IDM_POPUP_HEADLINE_FILEDIR)
          SendMessageA(hWnd, EM_REPLACESEL, TRUE, (LPARAM)"%d");
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
      if (DoViewFont(hDlg, &lfDialog))
      {
        if (hPrintFont) DeleteObject(hPrintFont);
        hPrintFont=(HFONT)CreateFontIndirectWide(&lfDialog);
        SendMessage(hWndFontButton, WM_SETFONT, (WPARAM)hPrintFont, TRUE);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (hBitmapDownArrow) DeleteObject(hBitmapDownArrow);
      if (hPrintFont) DeleteObject(hPrintFont);

      moCur.bPrintFontEnable=SendMessage(hWndFontCheck, BM_GETCHECK, 0, 0);
      xmemcpy(&moCur.lfPrintFont, &lfDialog, sizeof(LOGFONTW));

      moCur.dwPrintColor=0;
      if (SendMessage(hWndColorTextCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.dwPrintColor|=PRNC_TEXT;
      if (SendMessage(hWndColorBkCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.dwPrintColor|=PRNC_BACKGROUND;

      moCur.bPrintHeaderEnable=SendMessage(hWndHeaderCheck, BM_GETCHECK, 0, 0);
      GetWindowTextWide(hWndHeaderEdit, moCur.wszPrintHeader, MAX_PATH);

      moCur.bPrintFooterEnable=SendMessage(hWndFooterCheck, BM_GETCHECK, 0, 0);
      GetWindowTextWide(hWndFooterEdit, moCur.wszPrintFooter, MAX_PATH);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      if (hBitmapDownArrow) DeleteObject(hBitmapDownArrow);
      if (hPrintFont) DeleteObject(hPrintFont);
    }
  }
  return FALSE;
}

BOOL GetPrinterA(HWND hWndOwner, PRINTINFO *prninfo, BOOL bSilent)
{
  PRINTDLGA pdA={0};

  pdA.lStructSize =sizeof(PRINTDLGA);
  pdA.hwndOwner   =hWndOwner;
  pdA.Flags       =prninfo->dwPrintFlags|PD_RETURNDC|PD_USEDEVMODECOPIESANDCOLLATE;
  pdA.nMinPage    =1;
  pdA.nMaxPage    =9999;
  pdA.nFromPage   =prninfo->nFromPage;
  pdA.nToPage     =prninfo->nToPage;
  pdA.nCopies     =1;
  pdA.hDC         =prninfo->hDC;
  pdA.hDevMode    =prninfo->hDevMode;
  pdA.hDevNames   =prninfo->hDevNames;

  if (bSilent)
  {
    DEVMODEA *hDevMode;
    DEVNAMES *hDevNames;

    //If printer already had been choosen
    if (pdA.hDevMode && pdA.hDevNames)
    {
      hDevMode=(DEVMODEA *)GlobalLock(pdA.hDevMode);
      hDevNames=(DEVNAMES *)GlobalLock(pdA.hDevNames);
      pdA.hDC=CreateDCA((char *)hDevNames + hDevNames->wDriverOffset, (char *)hDevNames + hDevNames->wDeviceOffset, NULL, hDevMode);
      GlobalUnlock(pdA.hDevMode);
      GlobalUnlock(pdA.hDevNames);
    }

    //Get default printer
    if (!pdA.hDC)
    {
      pdA.Flags     =PD_RETURNDEFAULT|PD_RETURNDC;
      pdA.hDevMode  =NULL;
      pdA.hDevNames =NULL;

      if (!PrintDlgA(&pdA))
        return FALSE;
    }
  }
  else
  {
    //Display printer dialog
    if (!PrintDlgA(&pdA))
      return FALSE;
    prninfo->dwPrintFlags=pdA.Flags;
    prninfo->nFromPage=pdA.nFromPage;
    prninfo->nToPage=pdA.nToPage;
  }
  prninfo->hDC=pdA.hDC;
  prninfo->hDevMode=pdA.hDevMode;
  prninfo->hDevNames=pdA.hDevNames;
  return TRUE;
}

BOOL GetPrinterW(HWND hWndOwner, PRINTINFO *prninfo, BOOL bSilent)
{
  PRINTDLGW pdW={0};

  pdW.lStructSize =sizeof(PRINTDLGW);
  pdW.hwndOwner   =hWndOwner;
  pdW.Flags       =prninfo->dwPrintFlags|PD_RETURNDC|PD_USEDEVMODECOPIESANDCOLLATE;
  pdW.nMinPage    =1;
  pdW.nMaxPage    =9999;
  pdW.nFromPage   =prninfo->nFromPage;
  pdW.nToPage     =prninfo->nToPage;
  pdW.nCopies     =1;
  pdW.hDC         =prninfo->hDC;
  pdW.hDevMode    =prninfo->hDevMode;
  pdW.hDevNames   =prninfo->hDevNames;

  if (bSilent)
  {
    DEVMODEW *hDevMode;
    DEVNAMES *hDevNames;

    //If printer already had been choosen
    if (pdW.hDevMode && pdW.hDevNames)
    {
      hDevMode=(DEVMODEW *)GlobalLock(pdW.hDevMode);
      hDevNames=(DEVNAMES *)GlobalLock(pdW.hDevNames);
      pdW.hDC=CreateDCW((wchar_t *)hDevNames + hDevNames->wDriverOffset, (wchar_t *)hDevNames + hDevNames->wDeviceOffset, NULL, hDevMode);
      GlobalUnlock(pdW.hDevMode);
      GlobalUnlock(pdW.hDevNames);
    }

    //Get default printer
    if (!pdW.hDC)
    {
      pdW.Flags     =PD_RETURNDEFAULT|PD_RETURNDC;
      pdW.hDevMode  =NULL;
      pdW.hDevNames =NULL;

      if (!PrintDlgW(&pdW))
        return FALSE;
    }
  }
  else
  {
    //Display printer dialog
    if (!PrintDlgW(&pdW))
      return FALSE;
    prninfo->dwPrintFlags=pdW.Flags;
    prninfo->nFromPage=pdW.nFromPage;
    prninfo->nToPage=pdW.nToPage;
  }
  prninfo->hDC=pdW.hDC;
  prninfo->hDevMode=pdW.hDevMode;
  prninfo->hDevNames=pdW.hDevNames;
  return TRUE;
}

DWORD GetMappedPrintWidth(HWND hWnd)
{
  AEPRINT prn={0};
  AEHPRINT hPrintDoc;
  int nAveCharWidth;
  DWORD dwWidth=0;

  if (bOldWindows)
  {
    if (!GetPrinterA(hMainWnd, &prninfo, TRUE))
      return 0;
  }
  else
  {
    if (!GetPrinterW(hMainWnd, &prninfo, TRUE))
      return 0;
  }

  if (prninfo.hDC)
  {
    prn.dwFlags=(prninfo.dwPageFlags & PSD_INHUNDREDTHSOFMILLIMETERS?AEPRN_INHUNDREDTHSOFMILLIMETERS:AEPRN_INTHOUSANDTHSOFINCHES);
    prn.hPrinterDC=prninfo.hDC;
    prn.rcMargins=prninfo.rtMargin;
    prn.hEditFont=(HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0);

    if (hPrintDoc=(AEHPRINT)SendMessage(hWnd, AEM_STARTPRINTDOC, 0, (LPARAM)&prn))
    {
      nAveCharWidth=SendMessage(hWnd, AEM_GETCHARSIZE, AECS_AVEWIDTH, 0);
      dwWidth=MulDiv(nAveCharWidth, prn.rcPageIn.right - prn.rcPageIn.left, prn.nAveCharWidth);
      SendMessage(hWnd, AEM_ENDPRINTDOC, (WPARAM)hPrintDoc, (LPARAM)&prn);
    }
    DeleteDC(prninfo.hDC);
    prninfo.hDC=NULL;
  }
  return dwWidth;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
BOOL UpdateMappedPrintWidth(FRAMEDATA *lpFrame)
{
  if (lpFrame == lpFrameCurrent)
  {
    if (lpFrame->dwMarker == (DWORD)-1 || (lpFrame->ei.bWordWrap && lpFrame->dwWrapLimit == (DWORD)-1))
    {
      lpFrame->dwMappedPrintWidth=GetMappedPrintWidth(lpFrame->ei.hWndEdit);
      if (lpFrame->dwMarker == (DWORD)-1)
        SetMarker(lpFrame, lpFrame->dwMarker);
      if (lpFrame->ei.bWordWrap && lpFrame->dwWrapLimit == (DWORD)-1)
        SetWordWrap(lpFrame, lpFrame->dwWrapType, lpFrame->dwWrapLimit);
      return TRUE;
    }
  }
  return FALSE;
}

int PrintDocument(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage)
{
  DOCINFOW diW={0};
  POINT ptScreenDpi;
  POINT ptPrintDpi;
  PRINTPAGE *lpElement;
  AEHPRINT hPrintDoc;
  HFONT hPrintFontOld;
  HDC hScreenDC;
  RECT rcHeader;
  RECT rcFooter;
  int nPageNumber=nInitPage;
  BOOL bPrintError=FALSE;
  BOOL bPrintStop=FALSE;

  //Set print settings
  prn->dwFlags=((lpFrameCurrent->dwWrapType & AEWW_SYMBOL)?AEPRN_WRAPSYMBOL:AEPRN_WRAPWORD)|
               (prninfo.dwPageFlags & PSD_INHUNDREDTHSOFMILLIMETERS?AEPRN_INHUNDREDTHSOFMILLIMETERS:AEPRN_INTHOUSANDTHSOFINCHES)|
               (dwFlags & PRND_TEST?AEPRN_TEST:0)|
               (dwFlags & PRND_ANSI?AEPRN_ANSI:0)|
               (moCur.dwPrintColor & PRNC_TEXT?AEPRN_COLOREDTEXT:0)|
               (moCur.dwPrintColor & PRNC_BACKGROUND?AEPRN_COLOREDBACKGROUND:0);
  if (!(dwFlags & PRND_RANGE))
  {
    if (dwFlags & PRND_SELECTION)
    {
      if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
        return 0;
      prn->crText=crSel;
    }
    else if (dwFlags & PRND_ALLTEXT)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&prn->crText.ciMin);
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&prn->crText.ciMax);
    }
  }
  if (moCur.bPrintFontEnable)
    prn->hEditFont=CreateFontIndirectWide(&moCur.lfPrintFont);
  else
    prn->hEditFont=CreateFontIndirectWide(&lpFrameCurrent->lf);

  //Document properties
  if (dwFlags & PRND_REALPRINT)
  {
    LoadStringWide(hLangLib, STR_DOCNAME, wbuf, BUFFER_SIZE);
    diW.cbSize=sizeof(DOCINFOW);
    diW.lpszDocName=(lpFrameCurrent->wszFile[0])?lpFrameCurrent->wszFile:wbuf;
    if (prninfo.dwPrintFlags & PD_PRINTTOFILE) diW.lpszOutput=L"FILE:";
  }

  if (!(dwFlags & PRND_REALPRINT) || StartDocWide(prn->hPrinterDC, &diW) > 0)
  {
    if (hPrintDoc=(AEHPRINT)SendMessage(hWnd, AEM_STARTPRINTDOC, 0, (LPARAM)prn))
    {
      if (dwFlags & PRND_TEST)
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

      if (moCur.bPrintHeaderEnable)
      {
        rcHeader=prn->rcPageIn;
        rcHeader.bottom=rcHeader.top + prn->nCharHeight;
        prn->rcPageIn.top+=prn->nCharHeight;
      }
      if (moCur.bPrintFooterEnable)
      {
        rcFooter=prn->rcPageIn;
        rcFooter.top=rcFooter.bottom - prn->nCharHeight;
        prn->rcPageIn.bottom-=prn->nCharHeight;
      }

      while (!bPrintStop && !bPrintError)
      {
        ++nPageNumber;

        if (dwFlags & PRND_REALPRINT)
        {
          if (prninfo.dwPrintFlags & PD_PAGENUMS)
          {
            if (nPageNumber > prninfo.nToPage)
            {
              bPrintStop=TRUE;
              continue;
            }
            else if (nPageNumber < prninfo.nFromPage)
            {
              prn->dwFlags|=AEPRN_TEST;
              if (!SendMessage(hWnd, AEM_PRINTPAGE, (WPARAM)hPrintDoc, (LPARAM)prn))
                bPrintStop=TRUE;
              prn->dwFlags&=~AEPRN_TEST;
              continue;
            }
          }
        }

        //Print page
        if (!(dwFlags & PRND_REALPRINT) || StartPage(prn->hPrinterDC) > 0)
        {
          if (dwFlags & PRND_TEST)
          {
            if (dwFlags & PRND_SELECTION)
            {
              if (lpElement=StackPageInsert(&hPreviewSelPagesStack))
                lpElement->crText=prn->crText;
            }
            else
            {
              if (lpElement=StackPageInsert(&hPreviewAllPagesStack))
                lpElement->crText=prn->crText;
            }
          }

          if (moCur.bPrintHeaderEnable)
          {
            hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);
            if (!PrintHeadline(prn->hPrinterDC, &rcHeader, moCur.wszPrintHeader, nPageNumber))
              bPrintStop=TRUE;
            if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
          }

          if (!SendMessage(hWnd, AEM_PRINTPAGE, (WPARAM)hPrintDoc, (LPARAM)prn))
            bPrintStop=TRUE;

          if (moCur.bPrintFooterEnable)
          {
            hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);
            if (!PrintHeadline(prn->hPrinterDC, &rcFooter, moCur.wszPrintFooter, nPageNumber))
              bPrintStop=TRUE;
            if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
          }

          if (dwFlags & PRND_REALPRINT)
          {
            if (EndPage(prn->hPrinterDC) <= 0)
              bPrintError=TRUE;
          }
          if (dwFlags & PRND_ONEPAGE)
            break;
        }
        else bPrintError=TRUE;
      }
      SendMessage(hWnd, AEM_ENDPRINTDOC, (WPARAM)hPrintDoc, (LPARAM)prn);
    }
    if (dwFlags & PRND_REALPRINT) EndDoc(prn->hPrinterDC);
  }
  else bPrintError=TRUE;

  if (bPrintError)
  {
    LoadStringWide(hLangLib, MSG_ERROR_PRINT, wbuf, BUFFER_SIZE);
    API_MessageBox(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }
  if (prn->hEditFont)
  {
    DeleteObject(prn->hEditFont);
    prn->hEditFont=NULL;
  }
  return nPageNumber;
}

BOOL PrintHeadline(HDC hDC, RECT *rc, wchar_t *wpHeadline, int nPageNumber)
{
  //%% == %, %n[1] == nPageNumber[nPageStart], %f == lpFrameCurrent->wszFile, %d == GetFileDir(lpFrameCurrent->wszFile), %c == DT_CENTER, %l == DT_LEFT, %r == DT_RIGHT
  wchar_t wszFileDir[MAX_PATH];
  wchar_t wszCenter[MAX_PATH];
  wchar_t wszLeft[MAX_PATH];
  wchar_t wszRight[MAX_PATH];
  wchar_t *wpCount=&wszCenter[0];
  wchar_t wszPageNumber[32];
  const wchar_t *wpFile;
  DWORD dwAlign;
  int nCenter=0;
  int nLeft=0;
  int nRight=0;
  int nCount=nCenter;
  int nAlign=DT_CENTER;
  int nPageNumberLen;
  int nPageStart;
  int nFileLen;
  int nFileDirLen;
  int a;
  BOOL bResult=TRUE;

  wpFile=GetFileName(lpFrameCurrent->wszFile);
  nFileLen=lstrlenW(wpFile);
  nFileDirLen=GetFileDir(lpFrameCurrent->wszFile, wszFileDir, MAX_PATH);

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
          nPageStart=xatoiW(wszPageNumber, NULL);
          nPageNumberLen=xprintfW(wszPageNumber, L"%d", nPageNumber + nPageStart - 1);

          if (nCount + nPageNumberLen <= MAX_PATH)
          {
            xstrcpyW(wpCount + nCount, wszPageNumber);
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
          xstrcpyW(wpCount + nCount, wpFile);
          nCount+=nFileLen;
        }
        else break;
      }
      else if (wpHeadline[a] == 'd' || wpHeadline[a] == 'D')
      {
        if (nCount + nFileDirLen <= MAX_PATH)
        {
          xstrcpyW(wpCount + nCount, wszFileDir);
          nCount+=nFileDirLen;
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

  dwAlign=GetTextAlign(hDC);
  if (*wszCenter && bResult)
  {
    SetTextAlign(hDC, TA_CENTER|TA_TOP|TA_NOUPDATECP);
    bResult=ExtTextOutW(hDC, rc->left + (rc->right - rc->left) / 2, rc->top, ETO_CLIPPED, rc, wszCenter, nCenter, NULL);
  }
  if (*wszLeft && bResult)
  {
    SetTextAlign(hDC, TA_LEFT|TA_TOP|TA_NOUPDATECP);
    bResult=ExtTextOutW(hDC, rc->left, rc->top, ETO_CLIPPED, rc, wszLeft, nLeft, NULL);
  }
  if (*wszRight && bResult)
  {
    SetTextAlign(hDC, TA_RIGHT|TA_TOP|TA_NOUPDATECP);
    bResult=ExtTextOutW(hDC, rc->right, rc->top, ETO_CLIPPED, rc, wszRight, nRight, NULL);
  }
  SetTextAlign(hDC, dwAlign);

  return bResult;
}

BOOL CALLBACK PreviewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndPreview;
  static HWND hWndPrevPage;
  static HWND hWndPageCount;
  static HWND hWndNextPage;
  static HWND hWndZoom;
  static HWND hWndSelection;
  int i;

  if (uMsg == WM_INITDIALOG)
  {
    DWORD dwStyle;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndPrevPage=GetDlgItem(hDlg, IDC_PREVIEW_PREVPAGE);
    hWndPageCount=GetDlgItem(hDlg, IDC_PREVIEW_PAGECOUNT);
    hWndNextPage=GetDlgItem(hDlg, IDC_PREVIEW_NEXTPAGE);
    hWndZoom=GetDlgItem(hDlg, IDC_PREVIEW_ZOOM);
    hWndZoomEdit=GetDlgItem(hWndZoom, IDC_COMBOBOXEDIT);
    hWndSelection=GetDlgItem(hDlg, IDC_PREVIEW_SELECTION);

    dwStyle=GetWindowLongWide(hWndZoomEdit, GWL_STYLE);
    SetWindowLongWide(hWndZoomEdit, GWL_STYLE, dwStyle|ES_NUMBER);

    //Positioning dialog
    if (rcPreviewDialog.right && rcPreviewDialog.bottom)
      MoveWindow(hDlg, rcPreviewDialog.left, rcPreviewDialog.top, rcPreviewDialog.right, rcPreviewDialog.bottom, FALSE);

    //Initialize
    hWndPreviewDlg=hDlg;
    hWndPreview=NULL;
    ptPreviewScroll.x=0;
    ptPreviewScroll.y=0;
    nPreviewZoomPercent=nPreviewZoomValue;
    nPreviewPageCur=1;
    bPreviewSelection=FALSE;
    bPreviewRedrawLock=FALSE;

    if (PreviewInit(hWndSelection))
    {
      //Create preview window
      {
        WNDCLASSW wndclass={0};

        wndclass.style        =CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
        wndclass.lpfnWndProc  =PreviewProc;
        wndclass.cbClsExtra   =0;
        wndclass.cbWndExtra   =DLGWINDOWEXTRA;
        wndclass.hInstance    =hInstance;
        wndclass.hIcon        =NULL;
        wndclass.hCursor      =hCursorHandOpen;
        wndclass.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
        wndclass.lpszMenuName =NULL;
        wndclass.lpszClassName=APP_PRINTPREVIEW_CLASSW;
        RegisterClassWide(&wndclass);

        hWndPreview=CreateWindowExWide(0,
                                       APP_PRINTPREVIEW_CLASSW,
                                       NULL,
                                       WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
                                       rcPreviewWindow.left, rcPreviewWindow.top, rcPreviewDialog.right, rcPreviewDialog.bottom,
                                       hDlg,
                                       (HMENU)IDC_PREVIEW_BOX,
                                       hInstance,
                                       NULL);
      }

      //Set caret page
      if (!(nPreviewPageCur=StackPageFind(&hPreviewAllPagesStack, &ciCaret)))
        nPreviewPageCur=1;
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur, 0);

      //Fill zooms
      for (i=0; i <= nPreviewZoomMaxIndex; ++i)
      {
        xprintfW(wbuf, L"%d%%", lpZoom[i]);
        ComboBox_AddStringWide(hWndZoom, wbuf);
      }
      LoadStringWide(hLangLib, STR_PAGEFIT, wbuf, BUFFER_SIZE);
      ComboBox_AddStringWide(hWndZoom, wbuf);
      LoadStringWide(hLangLib, STR_PAGEWIDTH, wbuf, BUFFER_SIZE);
      ComboBox_AddStringWide(hWndZoom, wbuf);

      hHookKeys=SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetCurrentThreadId());
    }
    ShowWindow(hDlg, dwPreviewShowDialog);
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
        GetWindowTextWide(hWndZoomEdit, wbuf, BUFFER_SIZE);
        PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, xatoiW(wbuf, NULL), FALSE);
      }
      else if ((HWND)lParam == hWndPageCount)
      {
        GetWindowTextWide(hWndPageCount, wbuf, BUFFER_SIZE);
        PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, xatoiW(wbuf, NULL), 0);
      }
      else return FALSE;
    }
    else return FALSE;

    SetWindowLongWide(hDlg, DWL_MSGRESULT, 1);
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
    SetWindowLongWide(hDlg, DWL_MSGRESULT, 1);
    return TRUE;
  }
  else if (uMsg == AKDLG_PREVIEWSETZOOM)
  {
    nPreviewZoomValue=wParam;

    if (nPreviewZoomValue == PREVIEWZOOM_FIT ||
        nPreviewZoomValue == PREVIEWZOOM_WIDTH)
    {
    }
    else if (nPreviewZoomValue < lpZoom[0])
    {
      nPreviewZoomValue=lpZoom[0];
    }
    else if (nPreviewZoomValue > lpZoom[nPreviewZoomMaxIndex])
    {
      nPreviewZoomValue=lpZoom[nPreviewZoomMaxIndex];
    }

    if (nPreviewZoomValue != nPreviewZoomPercent)
    {
      nPreviewZoomPercent=nPreviewZoomValue;
      InvalidateRect(hWndPreview, NULL, TRUE);
      if (lParam) UpdateWindow(hWndPreview);
    }
    xprintfW(wbuf, L"%d%%", nPreviewZoomPercent);
    SetWindowTextWide(hWndZoomEdit, wbuf);
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

    xprintfW(wbuf, L"%d / %d", nPreviewPageCur, nPageSum);
    SetWindowTextWide(hWndPageCount, wbuf);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PREVIEW_PRINT)
    {
      if (bPreviewSelection)
      {
        prninfo.dwPrintFlags|=PD_SELECTION;
      }
      SendMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
      PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_PRINT, (LPARAM)hWndPreviewEdit);
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_PAGESETUP)
    {
      //Stop message hooking
      hWndPreviewDlg=NULL;

      if (DoFilePageSetup(hDlg))
      {
        hWndPreviewDlg=hDlg;
        PreviewUninit();
        if (PreviewInit(hWndSelection))
        {
          PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur, 0);
          InvalidateRect(hWndPreview, NULL, TRUE);
        }
      }
      hWndPreviewDlg=hDlg;
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_SELECTION)
    {
      nPreviewPageCur=1;
      bPreviewSelection=SendMessage(hWndSelection, BM_GETCHECK, 0, 0);
      PostMessage(hDlg, AKDLG_PREVIEWSETPAGE, nPreviewPageCur, 0);
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
          PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[nCurSel], FALSE);
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
            PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[i + 1], FALSE);
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
            PostMessage(hDlg, AKDLG_PREVIEWSETZOOM, lpZoom[i - 1], FALSE);
          }
          break;
        }
      }
    }
    else if (LOWORD(wParam) == IDC_PREVIEW_CLOSE ||
             LOWORD(wParam) == IDCANCEL)
    {
      if (GetWindowLongWide(hDlg, GWL_STYLE) & WS_MAXIMIZE)
      {
        dwPreviewShowDialog=SW_MAXIMIZE;
      }
      else
      {
        GetWindowPos(hDlg, NULL, &rcPreviewDialog);
        dwPreviewShowDialog=SW_SHOW;
      }
      hWndPreviewDlg=NULL;
      PreviewUninit();
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

BOOL PreviewInit(HWND hWndSelection)
{
  BOOL bResult=FALSE;

  xmemset(&rcPreviewZoomed, 0, sizeof(RECT));
  nPreviewAllPageSum=0;
  nPreviewSelPageSum=0;

  //Get printer DC
  if (!prninfo.hDC)
  {
    if (bOldWindows)
    {
      if (!GetPrinterA(hMainWnd, &prninfo, TRUE))
        return 0;
    }
    else
    {
      if (!GetPrinterW(hMainWnd, &prninfo, TRUE))
        return 0;
    }
  }

  if (prninfo.hDC)
  {
    //Initialize variables
    prn.hPrinterDC=prninfo.hDC;
    prn.rcMargins=prninfo.rtMargin;

    nPreviewAllPageSum=PrintDocument(hWndPreviewEdit, &prn, PRND_TEST|PRND_ALLTEXT, 0);
    nPreviewSelPageSum=PrintDocument(hWndPreviewEdit, &prn, PRND_TEST|PRND_SELECTION, 0);
    bResult=TRUE;
  }

  if (!nPreviewSelPageSum)
    EnableWindow(hWndSelection, FALSE);
  return bResult;
}

void PreviewUninit()
{
  StackPageFree(&hPreviewAllPagesStack);
  StackPageFree(&hPreviewSelPagesStack);

  if (prninfo.hDC)
  {
    DeleteDC(prninfo.hDC);
    prninfo.hDC=NULL;
  }
}

LRESULT CALLBACK PreviewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

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

        if ((bPreviewSelection == FALSE && (lpElement=StackPageGet(&hPreviewAllPagesStack, nPreviewPageCur))) ||
            (bPreviewSelection == TRUE && (lpElement=StackPageGet(&hPreviewSelPagesStack, nPreviewPageCur))))
        {
          //Create the EMF in memory
          rcMeta.left=0;
          rcMeta.top=0;
          rcMeta.right=GetDeviceCaps(prninfo.hDC, HORZSIZE) * 100;
          rcMeta.bottom=GetDeviceCaps(prninfo.hDC, VERTSIZE) * 100;
          if (bOldWindows)
            hMetaDC=CreateEnhMetaFileA(prninfo.hDC, NULL, &rcMeta, NULL);
          else
            hMetaDC=CreateEnhMetaFileW(prninfo.hDC, NULL, &rcMeta, NULL);

          //Print page on metafile device
          prn.hPrinterDC=hMetaDC;
          prn.crText=lpElement->crText;
          PrintDocument(hWndPreviewEdit, &prn, (bOldWindows?PRND_ANSI:0)|PRND_RANGE|PRND_ONEPAGE, nPreviewPageCur - 1);
          hMetaFile=CloseEnhMetaFile(hMetaDC);

          //Draw page on window
          PreviewPaint(hWnd, ps.hdc, hMetaFile);
          DeleteEnhMetaFile(hMetaFile);

          //Set zoom text
          xprintfW(wbuf, L"%d%%", nPreviewZoomPercent);
          SetWindowTextWide(hWndZoomEdit, wbuf);
        }
      }
      EndPaint(hWnd, &ps);
    }
  }

  if (lResult=PreviewMessages(hWnd, uMsg, wParam, lParam))
    return lResult;

  if (bOldWindows)
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
  else
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK PreviewMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static POINT ptMouseMove;

  if (uMsg == WM_LBUTTONDOWN ||
      uMsg == WM_LBUTTONDBLCLK)
  {
    if (!(dwMouseCapture & MSC_PREVIEWMOVE))
    {
      SetMouseCapture(hWnd, MSC_PREVIEWMOVE);
      SetCursor(hCursorHandClose);
      GetCursorPos(&ptMouseMove);
    }
    if (uMsg == WM_LBUTTONDBLCLK)
    {
      if (nPreviewZoomPercent == 100)
      {
        PostMessage(hWndPreviewDlg, AKDLG_PREVIEWSETZOOM, (WPARAM)PREVIEWZOOM_FIT, FALSE);
      }
      else
      {
        POINT pt;

        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);
        pt.x=MulDiv(100, ptPreviewScroll.x + pt.x, nPreviewZoomPercent) - pt.x;
        pt.y=MulDiv(100, ptPreviewScroll.y + pt.y, nPreviewZoomPercent) - pt.y;

        bPreviewRedrawLock=TRUE;
        SendMessage(hWndPreviewDlg, AKDLG_PREVIEWSETZOOM, 100, TRUE);
        bPreviewRedrawLock=FALSE;
        PreviewHScroll(hWnd, PREVIEWSCROLL_ABSOLUTE, pt.x);
        PreviewVScroll(hWnd, PREVIEWSCROLL_ABSOLUTE, pt.y);
      }
    }
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (dwMouseCapture & MSC_PREVIEWMOVE)
    {
      POINT pt;

      GetCursorPos(&pt);
      PreviewHScroll(hWnd, PREVIEWSCROLL_OFFSET, ptMouseMove.x - pt.x);
      PreviewVScroll(hWnd, PREVIEWSCROLL_OFFSET, ptMouseMove.y - pt.y);
      ptMouseMove=pt;
    }
  }
  else if (uMsg == WM_LBUTTONUP ||
           uMsg == WM_CAPTURECHANGED)
  {
    if (dwMouseCapture & MSC_PREVIEWMOVE)
    {
      ReleaseMouseCapture(MSC_PREVIEWMOVE);
      SetCursor(hCursorHandOpen);
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
    PreviewHScroll(hWnd, LOWORD(wParam), 0);
    return 1;
  }
  else if (uMsg == WM_VSCROLL)
  {
    PreviewVScroll(hWnd, LOWORD(wParam), 0);
    return 1;
  }
  return 0;
}

void PreviewPaint(HWND hWnd, HDC hPaintDC, HENHMETAFILE hMetaFile)
{
  HDC hBufferDC=NULL;
  HBITMAP hBitmap=NULL;
  HBITMAP hBitmapOld=NULL;
  HBRUSH hBrush;
  RECT rcPreviewIn;
  RECT rcPreviewClient;
  RECT rcPaperFrame;
  SIZE sizePreview;

  //Zooming
  if (nPreviewZoomPercent == PREVIEWZOOM_FIT)
  {
    FitInside(RectW(&rcPreviewPaper), RectH(&rcPreviewPaper), RectW(&rcPreviewWindow) - rcPreviewPaper.left * 2, RectH(&rcPreviewWindow) - rcPreviewPaper.top * 2, &sizePreview);
    nPreviewZoomPercent=sizePreview.cx * 100 / RectW(&rcPreviewPaper);
  }
  else if (nPreviewZoomPercent == PREVIEWZOOM_WIDTH)
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

  if (!bPreviewRedrawLock)
  {
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
  }

  //Draw area
  if (moCur.bPrintHeaderEnable)
    prn.rcPageIn.top-=prn.nCharHeight;
  if (moCur.bPrintFooterEnable)
    prn.rcPageIn.bottom+=prn.nCharHeight;
  rcPreviewIn.left=rcPreviewZoomed.left + MulDiv(RectW(&rcPreviewZoomed), prn.rcPageIn.left - prn.rcPageFull.left, RectW(&prn.rcPageFull));
  rcPreviewIn.top=rcPreviewZoomed.top + MulDiv(RectH(&rcPreviewZoomed), prn.rcPageIn.top - prn.rcPageFull.top, RectH(&prn.rcPageFull));
  rcPreviewIn.right=rcPreviewIn.left + MulDiv(RectW(&rcPreviewZoomed), RectW(&prn.rcPageIn), RectW(&prn.rcPageFull));
  rcPreviewIn.bottom=rcPreviewIn.top + MulDiv(RectH(&rcPreviewZoomed), RectH(&prn.rcPageIn), RectH(&prn.rcPageFull));

  //Get character size
  nPreviewAveCharWidth=MulDiv(RectW(&rcPreviewZoomed), prn.nAveCharWidth, RectW(&prn.rcPageFull));
  nPreviewCharHeight=MulDiv(RectW(&rcPreviewZoomed), prn.nCharHeight, RectW(&prn.rcPageFull));

  if (!bPreviewRedrawLock)
  {
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
}

int PreviewHScroll(HWND hWnd, int nAction, int nPos)
{
  SCROLLINFO si;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(hWnd, SB_HORZ, &si);

  if (nAction == PREVIEWSCROLL_OFFSET)
  {
    si.nPos+=nPos;
  }
  else if (nAction == PREVIEWSCROLL_ABSOLUTE)
  {
    si.nPos=nPos;
  }
  else if (nAction == SB_LEFT)
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

int PreviewVScroll(HWND hWnd, int nAction, int nPos)
{
  SCROLLINFO si;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(hWnd, SB_VERT, &si);

  if (nAction == PREVIEWSCROLL_OFFSET)
  {
    si.nPos+=nPos;
  }
  else if (nAction == PREVIEWSCROLL_ABSOLUTE)
  {
    si.nPos=nPos;
  }
  else if (nAction == SB_TOP)
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
    s->cx=s->cy * nWidth / nHeight; //landscape
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

PRINTPAGE* StackPageInsert(HSTACK *hStack)
{
  PRINTPAGE *lpElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(PRINTPAGE)))
    return lpElement;
  return NULL;
}

PRINTPAGE* StackPageGet(HSTACK *hStack, int nPage)
{
  PRINTPAGE *lpElement;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&lpElement, nPage))
    return lpElement;
  return NULL;
}

int StackPageFind(HSTACK *hStack, const AECHARINDEX *ciPos)
{
  PRINTPAGE *lpElement=(PRINTPAGE *)hStack->first;
  int nIndex=0;

  while (lpElement)
  {
    if (AEC_IndexCompare(ciPos, &lpElement->crText.ciMin) < 0)
      return nIndex;

    ++nIndex;
    lpElement=lpElement->next;
  }
  return nIndex;
}

void StackPageFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}


//// Code pages

UINT_PTR CALLBACK CodePageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    DIALOGCODEPAGE *dc;

    if (nMDI) hDlgEdit=GetDlgItem(GetParent(hDlg), IDC_OFN_EDIT);
    hWndCP=GetDlgItem(hDlg, IDC_OFN_CODEPAGE);
    hWndFilePreview=GetDlgItem(hDlg, IDC_OFN_PREVIEW);
    hWndAutodetect=GetDlgItem(hDlg, IDC_OFN_AUTODETECT);

    //Set dialog codepage
    if (bOldWindows)
      dc=(DIALOGCODEPAGE *)((OPENFILENAMEA *)lParam)->lCustData;
    else
      dc=(DIALOGCODEPAGE *)((OPENFILENAMEW *)lParam)->lCustData;
    nCodePage=lpFrameCurrent->ei.nCodePage;
    bBOM=lpFrameCurrent->ei.bBOM;
    if (dc)
    {
      if (dc->nCodePage >= 0) nCodePage=dc->nCodePage;
      if (dc->bBOM >= 0) bBOM=dc->bBOM;
    }
    FillComboboxCodepage(hWndCP, lpCodepageList);
    SelectComboboxCodepage(hWndCP, nCodePage);

    GetWindowPos(hDlg, NULL, &rcDlg);
    GetWindowPos(hWndCP, hDlg, &rcCodePage);
    GetWindowPos(hWndAutodetect, hDlg, &rcAutodetect);
    GetWindowPos(hWndFilePreview, hDlg, &rcFilePreview);

    if (bSaveDlg)
    {
      SetWindowPos(hDlg, 0, 0, 0, rcDlg.right, rcFilePreview.top, SWP_NOMOVE|SWP_NOZORDER);
      SetWindowTextWide(hWndAutodetect, L"&BOM");

      if (!IsCodePageUnicode(nCodePage))
      {
        SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)FALSE, 0);
        EnableWindow(hWndAutodetect, FALSE);
      }
      else SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)bBOM, 0);
    }
    else
    {
      LoadStringWide(hLangLib, STR_AUTODETECT, wbuf, BUFFER_SIZE);
      SetWindowTextWide(hWndAutodetect, wbuf);
      SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)bAutodetect, 0);
      EnableWindow(hWndCP, !bAutodetect);
    }
    SendMessage(hWndFilePreview, AEM_SETCOLORS, 0, (LPARAM)&lpFrameCurrent->aec);
    SetTabStops(hWndFilePreview, lpFrameCurrent->nTabStopSize, FALSE);
    SetChosenFont(hWndFilePreview, &lpFrameCurrent->lf);

    OldFilePreviewProc=(WNDPROC)GetWindowLongWide(hWndFilePreview, GWL_WNDPROC);
    SetWindowLongWide(hWndFilePreview, GWL_WNDPROC, (LONG)NewFilePreviewProc);
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
        LoadStringWide(hLangLib, MSG_CP_UNIMPLEMENTED, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, nOfnCodePage);
        API_MessageBox(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
        SetWindowLongWide(hDlg, DWL_MSGRESULT, 1);
        return TRUE;
      }
    }
    else if (((LPNMHDR)lParam)->code == CDN_SELCHANGE)
    {
      if (!bSaveDlg)
      {
        if (nMDI)
        {
          if (GetWindowTextLengthWide(hDlgEdit) > OPENFILELIST_SIZE)
          {
            LoadStringWide(hLangLib, MSG_LONG_FILELIST, wbuf, BUFFER_SIZE);
            API_MessageBox(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
          }
        }

        if (bOldWindows)
        {
          SendMessageA(((OFNOTIFY *)lParam)->hdr.hwndFrom, CDM_GETFILEPATH, BUFFER_SIZE, (WPARAM)buf);
          MultiByteToWideChar(CP_ACP, 0, buf, -1, wszFile, MAX_PATH);
        }
        else SendMessageW(((OFNOTIFY *)lParam)->hdr.hwndFrom, CDM_GETFILEPATH, MAX_PATH, (WPARAM)wszFile);

        if (FilePreview(hWndFilePreview, wszFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          if (bAutodetect) SelectComboboxCodepage(hWndCP, moCur.nDefaultCodePage);
          SetWindowTextWide(hWndFilePreview, L"");
          return TRUE;
        }
        EnableWindow(hWndFilePreview, TRUE);
        if (bAutodetect) SelectComboboxCodepage(hWndCP, nCodePage);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OFN_CODEPAGE && HIWORD(wParam) == CBN_SELCHANGE)
    {
      nCodePage=GetComboboxCodepage(hWndCP);

      if (bSaveDlg)
      {
        if (!IsCodePageUnicode(nCodePage))
        {
          EnableWindow(hWndAutodetect, FALSE);
          SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)FALSE, 0);
        }
        else
        {
          EnableWindow(hWndAutodetect, TRUE);
          SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)TRUE, 0);
        }
      }
      else
      {
        if (FilePreview(hWndFilePreview, wszFile, PREVIEW_SIZE, OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          SetWindowTextWide(hWndFilePreview, L"");
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
        nCodePage=GetComboboxCodepage(hWndCP);

        if (FilePreview(hWndFilePreview, wszFile, PREVIEW_SIZE, bAutodetect?(OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE):OD_ADT_DETECT_BOM, &nCodePage, &bBOM) < 0)
        {
          EnableWindow(hWndFilePreview, FALSE);
          if (bAutodetect) SelectComboboxCodepage(hWndCP, moCur.nDefaultCodePage);
          SetWindowTextWide(hWndFilePreview, L"");
          return TRUE;
        }
        EnableWindow(hWndFilePreview, TRUE);
        if (bAutodetect) SelectComboboxCodepage(hWndCP, nCodePage);
      }
      return TRUE;
    }
  }
  return FALSE;
}

LRESULT CALLBACK NewFilePreviewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_ESCAPE)
    {
      PostMessage(GetParent(hWnd), WM_KEYDOWN, VK_ESCAPE, 0);
      return TRUE;
    }
  }
  if (bOldWindows)
    return CallWindowProcA(OldFilePreviewProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldFilePreviewProc, hWnd, uMsg, wParam, lParam);
}

void FillComboboxCodepage(HWND hWnd, int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      if (nModelessType == MLT_RECODE &&
          (IsCodePageUnicode(lpCodepageList[i]) && lpCodepageList[i] != CP_UNICODE_UTF8))
      {
        continue;
      }
      GetCodePageName(lpCodepageList[i], wbuf, BUFFER_SIZE);
      ComboBox_AddStringWide(hWnd, wbuf);
    }
  }
}

void FillListBoxCodepage(HWND hWnd, int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      GetCodePageName(lpCodepageList[i], wbuf, BUFFER_SIZE);
      ListBox_AddStringWide(hWnd, wbuf);
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

void ClearListBox(HWND hWnd)
{
  int nCount;

  if ((nCount=SendMessage(hWnd, LB_GETCOUNT, 0, 0)) > 0)
  {
    while (--nCount >= 0) SendMessage(hWnd, LB_DELETESTRING, nCount, 0);
  }
}

int GetComboboxCodepage(HWND hWnd)
{
  int nCodePage=0;
  int nSelection;

  if ((nSelection=SendMessage(hWnd, CB_GETCURSEL, 0, 0)) != CB_ERR)
  {
    ComboBox_GetLBTextWide(hWnd, nSelection, wbuf);
    nCodePage=xatoiW(wbuf, NULL);
  }
  return nCodePage;
}

int GetListBoxCodepage(HWND hWnd)
{
  int nCodePage=0;
  int nSelection;

  if ((nSelection=(int)SendMessage(hWnd, LB_GETCURSEL, 0, 0)) != LB_ERR)
  {
    ListBox_GetTextWide(hWnd, nSelection, wbuf);
    nCodePage=xatoiW(wbuf, NULL);
  }
  return nCodePage;
}

int SelectComboboxCodepage(HWND hWnd, int nCodepage)
{
  int nSelection;

  xprintfW(wbuf, L"%d ", nCodepage);
  nSelection=ComboBox_FindStringWide(hWnd, -1, wbuf);
  SendMessage(hWnd, CB_SETCURSEL, (WPARAM)nSelection, 0);

  return nSelection;
}

int SelectListBoxCodepage(HWND hWnd, int nCodepage)
{
  int nSelection;

  xprintfW(wbuf, L"%d ", nCodepage);
  if ((nSelection=ListBox_FindStringWide(hWnd, -1, wbuf)) != LB_ERR)
    SendMessage(hWnd, LB_SETCURSEL, (WPARAM)nSelection, 0);

  return nSelection;
}

void GetListBoxCodepageList(HWND hWnd, int **lpCodepageList)
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
        ListBox_GetTextWide(hWnd, i, wbuf);
        *lpCodepageListCount++=xatoiW(wbuf, NULL);
      }
      *lpCodepageListCount=0;
      bCodepageListChanged=TRUE;
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

    //CP_ACP
    //CP_OEMCP
    //CP_UNICODE_UTF16LE
    //CP_UNICODE_UTF16BE
    //CP_UNICODE_UTF8
    //CP_UNICODE_UTF7
    //CP_UNICODE_UTF32LE
    //CP_UNICODE_UTF32BE
    //0
    nCodepageTableCount+=9;

    if (*lpCodepageList=lpCodepageListCount=(int *)API_HeapAlloc(hHeap, 0, sizeof(int) * nCodepageTableCount))
    {
      *lpCodepageListCount++=nAnsiCodePage;
      if (nAnsiCodePage != nOemCodePage)
        *lpCodepageListCount++=nOemCodePage;
      *lpCodepageListCount++=CP_UNICODE_UTF16LE;
      *lpCodepageListCount++=CP_UNICODE_UTF16BE;
      *lpCodepageListCount++=CP_UNICODE_UTF8;
      *lpCodepageListCount++=CP_UNICODE_UTF7;
      *lpCodepageListCount++=CP_UNICODE_UTF32LE;
      *lpCodepageListCount++=CP_UNICODE_UTF32BE;

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
        if ((i=xatoiA(buf, NULL)) > 0 && i < 65536 &&
            i != nAnsiCodePage &&
            i != nOemCodePage &&
            i != CP_UNICODE_UTF16LE &&
            i != CP_UNICODE_UTF16BE &&
            i != CP_UNICODE_UTF8 &&
            i != CP_UNICODE_UTF7 &&
            i != CP_UNICODE_UTF32LE &&
            i != CP_UNICODE_UTF32BE)
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

  if ((i=xatoiW(wpCodePage, NULL)) > 0 && i < 65536 &&
      i != nAnsiCodePage &&
      i != nOemCodePage &&
      i != CP_UNICODE_UTF16LE &&
      i != CP_UNICODE_UTF16BE &&
      i != CP_UNICODE_UTF8 &&
      i != CP_UNICODE_UTF7 &&
      i != CP_UNICODE_UTF32LE &&
      i != CP_UNICODE_UTF32BE)
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

void GetCodePageName(int nCodePage, wchar_t *wszCodePage, int nLen)
{
  CPINFOEXW CPInfoExW;

  if (nCodePage)
  {
    if (nCodePage == CP_UNICODE_UTF16LE)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF16LEW, nLen);
    else if (nCodePage == CP_UNICODE_UTF16BE)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF16BEW, nLen);
    else if (nCodePage == CP_UNICODE_UTF8)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF8W, nLen);
    else if (nCodePage == CP_UNICODE_UTF7)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF7W, nLen);
    else if (nCodePage == CP_UNICODE_UTF32LE)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF32LEW, nLen);
    else if (nCodePage == CP_UNICODE_UTF32BE)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF32BEW, nLen);
    else
    {
      if (GetCPInfoExWide(nCodePage, 0, &CPInfoExW) && nCodePage == xatoiW(CPInfoExW.CodePageName, NULL))
        xstrcpynW(wszCodePage, CPInfoExW.CodePageName, nLen);
      else
        xprintfW(wszCodePage, L"%d ", nCodePage);
    }
  }
  else wszCodePage[0]='\0';
}

int FilePreview(HWND hWnd, wchar_t *wpFile, int nPreviewBytes, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
{
  HANDLE hFile;
  FILESTREAMDATA fsd;
  int i;

  if (!(dwFlags & ADT_REG_CODEPAGE) && !(dwFlags & ADT_DETECT_CODEPAGE))
    if (!*nCodePage) return EOD_OPEN;

  if (IsFile(wpFile) != ERROR_SUCCESS)
    return EOD_OPEN;

  if ((i=AutodetectCodePage(wpFile, moCur.dwCodepageRecognitionBuffer, dwFlags, nCodePage, bBOM)) < 0)
    return i;

  if ((hFile=CreateFileWide(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
    return EOD_OPEN;

  //Offset BOM
  fsd.nBytesCurrent=0;

  if (*bBOM)
  {
    if (*nCodePage == CP_UNICODE_UTF16LE ||
        *nCodePage == CP_UNICODE_UTF16BE)
    {
      SetFilePointer(hFile, 2, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=2;
    }
    else if (*nCodePage == CP_UNICODE_UTF32LE ||
             *nCodePage == CP_UNICODE_UTF32BE)
    {
      SetFilePointer(hFile, 4, NULL, FILE_BEGIN);
      fsd.nBytesCurrent=4;
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

int AutodetectCodePage(const wchar_t *wpFile, DWORD dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
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

    if (moCur.nRecentFiles)
    {
      RecentFilesZero();
      RecentFilesRead();
    }
    if (moCur.nRecentFiles && moCur.bSaveCodepages && RecentFilesGet(wpFile, NULL, &nRegCodePage) && nRegCodePage)
    {
      if (nRegCodePage == CP_UNICODE_UTF32LE || nRegCodePage == CP_UNICODE_UTF32BE)
        dwFlags&=~ADT_BINARY_ERROR;
      *nCodePage=nRegCodePage;
      dwFlags|=ADT_DETECT_BOM;
    }
    else
    {
      dwFlags|=ADT_DETECT_CODEPAGE|ADT_DETECT_BOM;
    }
  }

  //Default
  if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=moCur.nDefaultCodePage;
  if (dwFlags & ADT_DETECT_BOM) *bBOM=FALSE;

  //Read file
  if (dwFlags & ADT_BINARY_ERROR || dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if ((hFile=CreateFileWide(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
      return EDT_OPEN;

    if (!(pBuffer=(unsigned char *)API_HeapAlloc(hHeap, 0, dwBytesToCheck + 1)))
    {
      CloseHandle(hFile);
      return EDT_ALLOC;
    }

    if (!API_ReadFile(hFile, pBuffer, dwBytesToCheck, &dwBytesRead, NULL))
    {
      SendMessage(hMainWnd, WM_COMMAND, IDM_INTERNAL_ERRORIO_MSG, 0);
      API_HeapFree(hHeap, 0, (LPVOID)pBuffer);
      CloseHandle(hFile);
      return EDT_READ;
    }
    CloseHandle(hFile);
  }

  //Detect Unicode BOM
  if (dwFlags & ADT_DETECT_CODEPAGE || dwFlags & ADT_DETECT_BOM)
  {
    if (dwBytesRead >= 4)
    {
      if (pBuffer[0] == 0xFF && pBuffer[1] == 0xFE && pBuffer[2] == 0x00 && pBuffer[3] == 0x00)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UTF32LE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF32LE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
      else if (pBuffer[0] == 0x00 && pBuffer[1] == 0x00 && pBuffer[2] == 0xFE && pBuffer[3] == 0xFF)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UTF32BE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF32BE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
    }
    if (dwBytesRead >= 2)
    {
      if (pBuffer[0] == 0xFF && pBuffer[1] == 0xFE)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UTF16LE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF16LE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto Free;
        }
      }
      else if (pBuffer[0] == 0xFE && pBuffer[1] == 0xFF)
      {
        if (!nRegCodePage || nRegCodePage == CP_UNICODE_UTF16BE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF16BE;
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
              if (bUtfCodePage) *nCodePage=moCur.nDefaultCodePage;
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
              *nCodePage=CP_UNICODE_UTF16LE;
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
              *nCodePage=CP_UNICODE_UTF16BE;
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
      if (!AutodetectMultibyte(PRIMARYLANGID(moCur.dwLangCodepageRecognition), pBuffer, dwBytesRead, nCodePage))
      {
        *nCodePage=moCur.nDefaultCodePage;
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
  return EDT_SUCCESS;
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
    xstrcpyA(szANSIwatermark, "\xE0\xE1\xE2\xE5\xE8\xED\xEE\xEF\xF0\xF2\xC0\xC1\xC2\xC5\xC8\xCD\xCE\xCF\xD2");  //‡·‚ÂËÌÓÔÚ¿¡¬≈»ÕŒœ“
    xstrcpyA(szOEMwatermark,  "\xAE\xA5\xA0\xA8\xAD\xE2\x8E\x45\x80\x88\x8D\x92\xB0\xB1\xB2\xB3\xBA\xDB\xCD");  //ÓÂ‡ËÌÚŒE¿»Õ“         Graphic simbols: \xB0\xB1\xB2\xB3\xBA\xDB\xCD
    xstrcpyA(szKOIwatermark,  "\xC1\xC2\xD7\xC5\xC9\xCE\xCF\xD2\xD4\xE1\xE2\xF7\xE5\xE9\xEE\xEF\xF0\xF2\xF4");  //¡¬◊≈…Œœ“‘·‚˜ÂÈÓÔÚÙ
    xstrcpyA(szUTF8watermark, "\xD0\xD1");
  }
  else if (dwLangID == LANG_ENGLISH)
  {
    xstrcpyA(szOEMwatermark, "\xB0\xB1\xB2\xB3\xBA\xDB\xCD");
  }
  else if (dwLangID == LANG_TURKISH)
  {
    xstrcpyA(szANSIwatermark, "\xFC\xFD\xFE");
    xstrcpyA(szOEMwatermark,  "\x81\x87\x8D\xB0\xB1\xB2\xB3\xBA\xDB\xCD");  //Graphic simbols: \xB0\xB1\xB2\xB3\xBA\xDB\xCD
    xstrcpyA(szUTF8watermark, "\xB0\xB1\xBC\xC3\xC4\xC5");
  }
  else if (dwLangID == LANG_CHINESE)
  {
    xstrcpyA(szANSIwatermark, "\xA1\xA2\xA3\xA4\xA5\xA6\xC0\xC1\xC2\xC3\xC4\xC5");
    xstrcpyA(szUTF8watermark, "\xE3\xE4\xE5\xE6\xE7\xE8");
  }
  else if (dwLangID == LANG_JAPANESE)
  {
    xstrcpyA(szANSIwatermark, "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF");
    xstrcpyA(szUTF8watermark, "\xE3");
  }
  else if (dwLangID == LANG_KOREAN)
  {
    xstrcpyA(szANSIwatermark, "\xC0\xC1\xC2\xC3");
    xstrcpyA(szUTF8watermark, "\xEA\xEB\xEC\xED");
  }
  else return FALSE;

  //Zero counter
  xmemset(dwCounter, 0, 0x80 * sizeof(DWORD));

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


unsigned int UTF32toUTF16(const unsigned long *pSource, unsigned int nSourceLen, unsigned int *nSourceDone, unsigned short *szTarget, unsigned int nTargetMax)
{
  const unsigned long *pSrc=pSource;
  const unsigned long *pSrcEnd=pSource + nSourceLen;
  unsigned short *pDst=szTarget;
  unsigned short *pDstEnd=szTarget + nTargetMax;
  unsigned long nChar;

  while (pSrc < pSrcEnd && pDst < pDstEnd)
  {
    nChar=*pSrc;

    if (nChar <= 0xFFFF)
    {
      // UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values
      if (nChar >= 0xD800 && nChar <= 0xDFFF)
        *pDst++=0xFFFD;
      else
        *pDst++=(unsigned short)nChar;
    }
    else if (nChar <= 0x0010FFFF)
    {
      if (pDst + 1 >= pDstEnd) break;
      nChar-=0x0010000UL;
      *pDst++=(unsigned short)((nChar >> 10) + 0xD800);
      *pDst++=(unsigned short)((nChar & 0x3FFUL) + 0xDC00);
    }
    else
    {
      *pDst++=0xFFFD;
    }
    ++pSrc;
  }
  if (nSourceDone)
    *nSourceDone=pSrc - pSource;
  return (pDst - szTarget);
}

unsigned int UTF16toUTF32(const unsigned short *pSource, unsigned int nSourceLen, unsigned int *nSourceDone, unsigned long *szTarget, unsigned int nTargetMax)
{
  const unsigned short *pSrc=pSource;
  const unsigned short *pSrcEnd=pSource + nSourceLen;
  unsigned long *pDst=szTarget;
  unsigned long *pDstEnd=szTarget + nTargetMax;
  unsigned long nChar;

  while (pSrc < pSrcEnd && pDst < pDstEnd)
  {
    nChar=*pSrc;

    //Surrogate pair. High surrogate.
    if (nChar >= 0xD800 && nChar <= 0xDBFF)
    {
      if (++pSrc >= pSrcEnd)
      {
        --pSrc;
        break;
      }

      //Low surrogate
      if (*pSrc >= 0xDC00 && *pSrc <= 0xDFFF)
        nChar=((nChar - 0xD800) << 10) + (*pSrc - 0xDC00) + 0x0010000UL;
      else
        continue;
    }
    *pDst++=nChar;
    ++pSrc;
  }
  if (nSourceDone)
    *nSourceDone=pSrc - pSource;
  return (pDst - szTarget);
}

unsigned int UTF16toUTF8(const unsigned short *pSource, unsigned int nSourceLen, unsigned int *nSourceDone, unsigned char *szTarget, unsigned int nTargetMax)
{
  unsigned int lpFirstByteMark[7]={0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
  const unsigned short *pSrc=pSource;
  const unsigned short *pSrcEnd=pSource + nSourceLen;
  const unsigned short *pSrcDone=pSource;
  unsigned char *pDst=szTarget;
  unsigned char *pDstEnd=szTarget + nTargetMax;
  unsigned long nChar;
  unsigned int nBitesInChar;

  while (pSrc < pSrcEnd && pDst < pDstEnd)
  {
    nChar=*pSrc;

    //Surrogate pair. High surrogate.
    if (nChar >= 0xD800 && nChar <= 0xDBFF)
    {
      if (++pSrc >= pSrcEnd)
      {
        --pSrc;
        break;
      }

      //Low surrogate
      if (*pSrc >= 0xDC00 && *pSrc <= 0xDFFF)
        nChar=0x10000 + ((nChar - 0xD800) << 10) + (*pSrc - 0xDC00);
      else
      {
        pSrcDone=pSource;
        continue;
      }
    }

    if (nChar < 0x110000)
    {
      if (nChar >= 0x10000)
      {
        if (pDst + 3 >= pDstEnd) break;
        nBitesInChar=4;
      }
      else if (nChar >= 0x800)
      {
        if (pDst + 2 >= pDstEnd) break;
        nBitesInChar=3;
      }
      else if (nChar >= 0x80)
      {
        if (pDst + 1 >= pDstEnd) break;
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
          *(pDst + 3)=(unsigned char)(nChar | 0x80) & 0xBF;
          nChar=nChar >> 6;
        }
        case 3:
        {
          *(pDst + 2)=(unsigned char)(nChar | 0x80) & 0xBF;
          nChar=nChar >> 6;
        }
        case 2:
        {
          *(pDst + 1)=(unsigned char)(nChar | 0x80) & 0xBF;
          nChar=nChar >> 6;
        }
        case 1:
        {
          *pDst=(unsigned char)(nChar | lpFirstByteMark[nBitesInChar]);
        }
      }
      pDst+=nBitesInChar;
    }
    pSrcDone=++pSrc;
  }
  if (nSourceDone)
    *nSourceDone=pSrcDone - pSource;
  return (pDst - szTarget);
}

unsigned int UTF8toUTF16(const unsigned char *pSource, unsigned int nSourceLen, unsigned int *nSourceDone, unsigned short *szTarget, unsigned int nTargetMax)
{
  unsigned int lpOffsetsFromUTF8[6]={0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080};
  const unsigned char *pSrc=pSource;
  const unsigned char *pSrcEnd=pSource + nSourceLen;
  const unsigned char *pSrcDone=pSource;
  unsigned short *pDst=szTarget;
  unsigned short *pDstEnd=szTarget + nTargetMax;
  unsigned long nChar;
  unsigned int nTrailing;

  while (pSrc < pSrcEnd && pDst < pDstEnd)
  {
    if (*pSrc < 0x80)
    {
      nTrailing=0;
    }
    else if (*pSrc < 0xC0)
    {
      //Trailing byte in leading position
      pSrcDone=++pSrc;
      continue;
    }
    else if (*pSrc < 0xE0)
    {
      if (pSrc + 1 >= pSrcEnd) break;
      nTrailing=1;
    }
    else if (*pSrc < 0xF0)
    {
      if (pSrc + 2 >= pSrcEnd) break;
      nTrailing=2;
    }
    else if (*pSrc < 0xF8)
    {
      if (pSrc + 3 >= pSrcEnd) break;
      nTrailing=3;
    }
    else
    {
      //No chance for this in UTF-16
      pSrcDone=++pSrc;
      continue;
    }

    //Get unicode char
    nChar=0;

    switch (nTrailing)
    {
      case 3:
      {
        nChar+=*pSrc++;
        nChar=nChar << 6;
      }
      case 2:
      {
        nChar+=*pSrc++;
        nChar=nChar << 6;
      }
      case 1:
      {
        nChar+=*pSrc++;
        nChar=nChar << 6;
      }
      case 0:
      {
        nChar+=*pSrc++;
      }
    }
    nChar-=lpOffsetsFromUTF8[nTrailing];

    //Write unicode char
    if (nChar <= 0xFFFF)
    {
      *pDst++=(unsigned short)nChar;
    }
    else
    {
      //Surrogate pair
      if (pDst + 1 >= pDstEnd) break;
      nChar-=0x10000;
      *pDst++=(unsigned short)((nChar >> 10) + 0xD800);
      *pDst++=(unsigned short)((nChar & 0x3ff) + 0xDC00);
    }
    pSrcDone=pSrc;
  }
  if (nSourceDone)
    *nSourceDone=pSrcDone - pSource;
  return (pDst - szTarget);
}

void ChangeTwoBytesOrder(unsigned char *lpBuffer, unsigned int nBufferLen)
{
  unsigned char *lpBufferEnd=lpBuffer + nBufferLen;
  unsigned char *lpByte=lpBuffer;
  unsigned char ch;

  for (; lpByte + 1 < lpBufferEnd; lpByte+=2)
  {
    ch=*lpByte;
    *lpByte=*(lpByte + 1);
    *(lpByte + 1)=ch;
  }
}

void ChangeFourBytesOrder(unsigned char *lpBuffer, unsigned int nBufferLen)
{
  unsigned char *lpBufferEnd=lpBuffer + nBufferLen;
  unsigned char *lpByte=lpBuffer;
  unsigned char ch;

  for (; lpByte + 3 < lpBufferEnd; lpByte+=4)
  {
    ch=*lpByte;
    *lpByte=*(lpByte + 3);
    *(lpByte + 3)=ch;

    ch=*(lpByte + 1);
    *(lpByte + 1)=*(lpByte + 2);
    *(lpByte + 2)=ch;
  }
}

BOOL IsCodePageUnicode(int nCodePage)
{
  if (nCodePage == CP_UNICODE_UTF16LE ||
      nCodePage == CP_UNICODE_UTF16BE ||
      nCodePage == CP_UNICODE_UTF32LE ||
      nCodePage == CP_UNICODE_UTF32BE ||
      nCodePage == CP_UNICODE_UTF8)
  {
    return TRUE;
  }
  return FALSE;
}

BOOL IsCodePageValid(int nCodePage)
{
  char ch='A';
  wchar_t wch;

  if (IsCodePageUnicode(nCodePage))
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

void RegReadSearch()
{
  wchar_t wszRegKey[MAX_PATH];
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  xprintfW(wszRegKey, L"%s\\Search", APP_REGHOMEW);
  if (RegOpenKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  if (RegQueryValueExWide(hKey, L"find0", NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
  {
    if (wszFindText_orig=wszFindText=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize))
    {
      if (RegQueryValueExWide(hKey, L"find0", NULL, &dwType, (LPBYTE)wszFindText_orig, &dwSize) == ERROR_SUCCESS)
      {
        nFindTextLen=dwSize / sizeof(wchar_t) - 1;

        if (moCur.dwSearchOptions & AEFR_ESCAPESEQ)
        {
          if (wszFindText=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize))
          {
            nFindTextLen=EscapeStringToEscapeDataW(wszFindText_orig, wszFindText, NEWLINE_MAC);
          }
        }
      }
    }
  }
  RegCloseKey(hKey);
}

BOOL CALLBACK FindAndReplaceDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndFind;
  static HWND hWndReplace;
  static HWND hWndMatchCase;
  static HWND hWndWholeWord;
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
  static BOOL bSpecialCheck=FALSE;
  CHARRANGE cr;
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
    hWndWholeWord=GetDlgItem(hDlg, IDC_SEARCH_WHOLEWORD);
    hWndEscapeSeq=GetDlgItem(hDlg, IDC_SEARCH_ESCAPESEQ);
    hWndForward=GetDlgItem(hDlg, IDC_SEARCH_FORWARD);
    hWndBackward=GetDlgItem(hDlg, IDC_SEARCH_BACKWARD);
    hWndBeginning=GetDlgItem(hDlg, IDC_SEARCH_BEGINNING);
    hWndInSelection=GetDlgItem(hDlg, IDC_SEARCH_INSEL);
    if (nModelessType == MLT_REPLACE)
    {
      hWndReplace=GetDlgItem(hDlg, IDC_SEARCH_REPLACE);
      hWndReplaceButton=GetDlgItem(hDlg, IDC_SEARCH_REPLACE_BUTTON);
      hWndReplaceAllButton=GetDlgItem(hDlg, IDC_SEARCH_ALL_BUTTON);
    }
    hWndAllFiles=GetDlgItem(hDlg, IDC_SEARCH_ALLFILES);
    hWndAllFilesGroup=GetDlgItem(hDlg, IDC_SEARCH_ALLFILES_GROUP);

    if (moCur.nSearchStrings)
    {
      FillComboboxSearch(hWndFind, hWndReplace);
    }
    SendMessage(lpFrameCurrent->ei.hWndEdit, EM_EXGETSEL, 0, (LPARAM)&cr);

    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax) || cr.cpMax - cr.cpMin > PUTFIND_MAXSEL || (moCur.dwSearchOptions & AEFR_SELECTION) || SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETCOLUMNSEL, 0, 0))
    {
      SendMessage(hWndFind, CB_SETCURSEL, 0, 0);
    }
    else
    {
      if (ExGetRangeTextW(lpFrameCurrent->ei.hWndEdit, &crSel.ciMin, &crSel.ciMax, FALSE, &wszData, AELB_R, FALSE))
      {
        SetWindowTextWide(hWndFind, wszData);
        FreeText((LPVOID)wszData);
      }
    }
    if (!nMDI)
    {
      ShowWindow(hWndAllFiles, SW_HIDE);
      ShowWindow(hWndAllFilesGroup, SW_HIDE);
    }
    if (nModelessType == MLT_REPLACE)
    {
      SendMessage(hWndReplace, CB_SETCURSEL, 0, 0);
    }

    if (moCur.dwSearchOptions & AEFR_ALLFILES)
      SendMessage(hWndAllFiles, BM_SETCHECK, BST_CHECKED, 0);
    else if (moCur.dwSearchOptions & AEFR_SELECTION)
      SendMessage(hWndInSelection, BM_SETCHECK, BST_CHECKED, 0);
    else if (moCur.dwSearchOptions & AEFR_BEGINNING)
      SendMessage(hWndBeginning, BM_SETCHECK, BST_CHECKED, 0);
    else if (moCur.dwSearchOptions & AEFR_DOWN)
      SendMessage(hWndForward, BM_SETCHECK, BST_CHECKED, 0);
    else if (moCur.dwSearchOptions & AEFR_UP)
      SendMessage(hWndBackward, BM_SETCHECK, BST_CHECKED, 0);

    if (moCur.dwSearchOptions & AEFR_MATCHCASE) SendMessage(hWndMatchCase, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.dwSearchOptions & AEFR_WHOLEWORD) SendMessage(hWndWholeWord, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.dwSearchOptions & AEFR_ESCAPESEQ) SendMessage(hWndEscapeSeq, BM_SETCHECK, BST_CHECKED, 0);

    if (hWndComboboxEdit=GetDlgItem(hWndFind, IDC_COMBOBOXEDIT))
    {
      SendMessage(hWndComboboxEdit, EM_LIMITTEXT, 0, 0);
      OldComboboxEdit=(WNDPROC)GetWindowLongWide(hWndComboboxEdit, GWL_WNDPROC);
      SetWindowLongWide(hWndComboboxEdit, GWL_WNDPROC, (LONG)NewComboboxEditProc);
    }
    if (nModelessType == MLT_REPLACE)
    {
      if (hWndComboboxEdit=GetDlgItem(hWndReplace, IDC_COMBOBOXEDIT))
      {
        SendMessage(hWndComboboxEdit, EM_LIMITTEXT, 0, 0);
        OldComboboxEdit=(WNDPROC)GetWindowLongWide(hWndComboboxEdit, GWL_WNDPROC);
        SetWindowLongWide(hWndComboboxEdit, GWL_WNDPROC, (LONG)NewComboboxEditProc);
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
      if (SendMessage(hWndMatchCase, BM_GETCHECK, 0, 0)) moCur.dwSearchOptions|=AEFR_MATCHCASE;
      else moCur.dwSearchOptions&=~AEFR_MATCHCASE;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_WHOLEWORD)
    {
      if (SendMessage(hWndWholeWord, BM_GETCHECK, 0, 0)) moCur.dwSearchOptions|=AEFR_WHOLEWORD;
      else moCur.dwSearchOptions&=~AEFR_WHOLEWORD;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ESCAPESEQ)
    {
      if (SendMessage(hWndEscapeSeq, BM_GETCHECK, 0, 0)) moCur.dwSearchOptions|=AEFR_ESCAPESEQ;
      else moCur.dwSearchOptions&=~AEFR_ESCAPESEQ;
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_FORWARD)
    {
      moCur.dwSearchOptions&=~AEFR_UP&~AEFR_BEGINNING&~AEFR_SELECTION&~AEFR_ALLFILES;
      moCur.dwSearchOptions|=AEFR_DOWN;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BACKWARD)
    {
      moCur.dwSearchOptions&=~AEFR_DOWN&~AEFR_BEGINNING&~AEFR_SELECTION&~AEFR_ALLFILES;
      moCur.dwSearchOptions|=AEFR_UP;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_BEGINNING)
    {
      moCur.dwSearchOptions&=~AEFR_UP&~AEFR_SELECTION&~AEFR_ALLFILES;
      moCur.dwSearchOptions|=AEFR_DOWN|AEFR_BEGINNING;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_INSEL)
    {
      moCur.dwSearchOptions&=~AEFR_UP&~AEFR_BEGINNING&~AEFR_ALLFILES;
      moCur.dwSearchOptions|=AEFR_DOWN|AEFR_SELECTION;
    }
    else if (LOWORD(wParam) == IDC_SEARCH_ALLFILES)
    {
      moCur.dwSearchOptions&=~AEFR_UP&~AEFR_SELECTION;
      moCur.dwSearchOptions|=AEFR_DOWN|AEFR_BEGINNING|AEFR_ALLFILES;
    }
    else if (LOWORD(wParam) == IDC_SETREADONLY)
    {
      if (nModelessType == MLT_REPLACE)
      {
        EnableWindow(hWndReplaceButton, !lpFrameCurrent->ei.bReadOnly);
        EnableWindow(hWndReplaceAllButton, !lpFrameCurrent->ei.bReadOnly);
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
      if (nModelessType == MLT_REPLACE)
      {
        if (!lpFrameCurrent->ei.bReadOnly)
        {
          EnableWindow(hWndReplaceButton, TRUE);
          EnableWindow(hWndReplaceAllButton, TRUE);
        }
      }

      if (LOWORD(wParam) != IDC_SEARCH_FIND ||
          HIWORD(wParam) == CBN_EDITCHANGE)
      {
        if (bSpecialCheck == TRUE)
        {
          bSpecialCheck=FALSE;
          if (HIWORD(wParam) == CBN_EDITCHANGE)
            moCur.dwSearchOptions|=AEFR_BEGINNING;
          SendMessage(hWndBeginning, BM_SETSTATE, FALSE, 0);
          if (nMDI) SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
        }
      }

      if (HIWORD(wParam) == CBN_SELCHANGE)
        i=ComboBox_GetLBTextLenWide(hWndFind, SendMessage(hWndFind, CB_GETCURSEL, 0, 0));
      else
        i=GetWindowTextLengthWide(hWndFind);

      if (i == 0)
      {
        EnableWindow(hWndFindNextButton, FALSE);
        if (nModelessType == MLT_REPLACE)
        {
          EnableWindow(hWndReplaceButton, FALSE);
          EnableWindow(hWndReplaceAllButton, FALSE);
        }
      }
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
      FreeMemorySearch();

      if ((nFindTextLen=GetComboboxSearchText(hWndFind, &wszFindText_orig, &wszFindText, NEWLINE_MAC)) <= 0)
      {
        FreeMemorySearch();
        return TRUE;
      }
      if (nModelessType == MLT_REPLACE)
      {
        if ((nReplaceTextLen=GetComboboxSearchText(hWndReplace, &wszReplaceText_orig, &wszReplaceText, lpFrameCurrent->ei.nNewLine)) < 0)
        {
          FreeMemorySearch();
          return TRUE;
        }
      }

      hWndFocus=GetFocus();
      if (bReplaceAll)
        bReplaceAllButtonState=EnableWindow(hWndReplaceAllButton, FALSE);

      if (moCur.dwSearchOptions & AEFR_ALLFILES)
      {
        FRAMEDATA *lpFrameInit=lpFrameCurrent;
        int nChanges=0;
        int nChangedFiles=0;

        if (bReplaceAll == TRUE)
        {
          if (bSpecialCheck == TRUE)
          {
            bSpecialCheck=FALSE;
            moCur.dwSearchOptions|=AEFR_BEGINNING;
            SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
          }

          do
          {
            ReplaceTextW(lpFrameCurrent->ei.hWndEdit, moCur.dwSearchOptions, wszFindText, nFindTextLen, wszReplaceText, nReplaceTextLen, TRUE, &nReplaceCount);
            if (!hDlgModeless) break;

            if (nReplaceCount)
            {
              ++nChangedFiles;
              nChanges+=nReplaceCount;
            }
            lpFrameCurrent=NextMdiFrameWindow(lpFrameCurrent, FALSE);
          }
          while (lpFrameCurrent != lpFrameInit);

          if (!(moCur.dwSearchOptions & AEFR_REPLACEALLANDCLOSE))
          {
            LoadStringWide(hLangLib, MSG_REPLACE_COUNT_ALLFILES, wbuf, BUFFER_SIZE);
            xprintfW(wbuf2, wbuf, nChangedFiles, nChanges);
            API_MessageBox(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
          }
          else
          {
            PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
            return TRUE;
          }
        }
        else
        {
          do
          {
            if (bReplace == TRUE)
              nResult=ReplaceTextW(lpFrameCurrent->ei.hWndEdit, moCur.dwSearchOptions, wszFindText, nFindTextLen, wszReplaceText, nReplaceTextLen, FALSE, NULL);
            else
              nResult=FindTextW(lpFrameCurrent->ei.hWndEdit, moCur.dwSearchOptions, wszFindText, nFindTextLen);

            if (nResult == -1)
            {
              if (bSpecialCheck == TRUE)
              {
                bSpecialCheck=FALSE;
                moCur.dwSearchOptions|=AEFR_BEGINNING;
                SendMessage(hWndAllFiles, BM_SETSTATE, FALSE, 0);
              }
              lpFrameCurrent=NextMdiFrameWindow(lpFrameCurrent, FALSE);
            }
            else
            {
              if (bSpecialCheck == FALSE)
              {
                bSpecialCheck=TRUE;
                moCur.dwSearchOptions&=~AEFR_BEGINNING;
                SendMessage(hWndAllFiles, BM_SETSTATE, TRUE, 0);
              }
              break;
            }
          }
          while (lpFrameCurrent != lpFrameInit);

          if (nResult == -1)
          {
            LoadStringWide(hLangLib, MSG_SEARCH_ENDED, wbuf, BUFFER_SIZE);
            API_MessageBox(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
          }
        }
      }
      else
      {
        if (bReplaceAll == TRUE)
        {
          if (bSpecialCheck == TRUE)
          {
            bSpecialCheck=FALSE;
            moCur.dwSearchOptions|=AEFR_BEGINNING;
            SendMessage(hWndBeginning, BM_SETSTATE, FALSE, 0);
          }
        }

        if (bReplace == TRUE || bReplaceAll == TRUE)
          nResult=ReplaceTextW(lpFrameCurrent->ei.hWndEdit, moCur.dwSearchOptions, wszFindText, nFindTextLen, wszReplaceText, nReplaceTextLen, bReplaceAll, &nReplaceCount);
        else
          nResult=FindTextW(lpFrameCurrent->ei.hWndEdit, moCur.dwSearchOptions, wszFindText, nFindTextLen);

        if (nResult == -1)
        {
          if (bSpecialCheck == TRUE)
          {
            bSpecialCheck=FALSE;
            moCur.dwSearchOptions|=AEFR_BEGINNING;
            SendMessage(hWndBeginning, BM_SETSTATE, FALSE, 0);
          }
          if (bReplaceAll == TRUE)
          {
            if (!(moCur.dwSearchOptions & AEFR_REPLACEALLANDCLOSE))
            {
              LoadStringWide(hLangLib, MSG_REPLACE_COUNT, wbuf, BUFFER_SIZE);
              xprintfW(wbuf2, wbuf, nReplaceCount);
              API_MessageBox(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
            }
            else
            {
              PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
              return TRUE;
            }
          }
          else
          {
            LoadStringWide(hLangLib, MSG_SEARCH_ENDED, wbuf, BUFFER_SIZE);
            API_MessageBox(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
          }
        }
        else
        {
          if (moCur.dwSearchOptions & AEFR_BEGINNING)
          {
            bSpecialCheck=TRUE;
            moCur.dwSearchOptions&=~AEFR_BEGINNING;
            SendMessage(hWndBeginning, BM_SETSTATE, TRUE, 0);
          }
        }
      }
      if (bReplaceAll)
        EnableWindow(hWndReplaceAllButton, !bReplaceAllButtonState);
      SetFocus(hWndFocus);

      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      if (bSpecialCheck == TRUE)
      {
        bSpecialCheck=FALSE;
        moCur.dwSearchOptions|=AEFR_BEGINNING;
      }
      if (moCur.nSearchStrings)
        SaveComboboxSearch(hWndFind, hWndReplace);
      GetWindowPos(hDlg, NULL, &rcFindAndReplaceDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      nModelessType=MLT_NONE;
      return TRUE;
    }
  }
  else if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_F3)
    {
      if (GetWindowTextLengthWide(hWndFind))
      {
        FreeMemorySearch();

        if ((nFindTextLen=GetComboboxSearchText(hWndFind, &wszFindText_orig, &wszFindText, NEWLINE_MAC)) <= 0)
        {
          FreeMemorySearch();
          return TRUE;
        }

        if (GetKeyState(VK_SHIFT) >= 0)
          DoEditFindNextDown(lpFrameCurrent->ei.hWndEdit);
        else
          DoEditFindNextUp(lpFrameCurrent->ei.hWndEdit);

        PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
      }
    }
  }
  return FALSE;
}

LRESULT CALLBACK NewComboboxEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_PASTE)
  {
    PasteInEditAsRichEdit(hWnd);
    return TRUE;
  }

  if (bOldWindows)
    return CallWindowProcA(OldComboboxEdit, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldComboboxEdit, hWnd, uMsg, wParam, lParam);
}

void FillComboboxSearch(HWND hWndFind, HWND hWndReplace)
{
  wchar_t wszRegKey[MAX_PATH];
  wchar_t wszRegValue[32];
  wchar_t *wszData;
  HWND hWnd;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int i;

  xprintfW(wszRegKey, L"%s\\Search", APP_REGHOMEW);
  if (RegOpenKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return;

  hWnd=hWndFind;
  xstrcpyW(wbuf, L"find%d");

  ReadW:
  for (i=0; i < moCur.nSearchStrings; ++i)
  {
    xprintfW(wszRegValue, wbuf, i);

    if (RegQueryValueExWide(hKey, wszRegValue, NULL, &dwType, NULL, &dwSize) == ERROR_SUCCESS && dwSize > 0)
    {
      if (wszData=(wchar_t *)API_HeapAlloc(hHeap, 0, dwSize))
      {
        if (RegQueryValueExWide(hKey, wszRegValue, NULL, &dwType, (LPBYTE)wszData, &dwSize) == ERROR_SUCCESS)
        {
          ComboBox_AddStringWide(hWnd, wszData);
          API_HeapFree(hHeap, 0, (LPVOID)wszData);
          continue;
        }
      }
    }
    break;
  }
  if (nModelessType == MLT_REPLACE && hWnd != hWndReplace)
  {
    hWnd=hWndReplace;
    xstrcpyW(wbuf, L"replace%d");
    goto ReadW;
  }
  RegCloseKey(hKey);
}

int GetComboboxSearchText(HWND hWnd, wchar_t **wszText_orig, wchar_t **wszText, int nNewLine)
{
  int nTextLen_orig;
  int nTextLen=-1;
  int nIndex;
  int nItemLen;

  nTextLen_orig=GetWindowTextLengthWide(hWnd) + 1;

  if (*wszText_orig=*wszText=AllocWideStr(nTextLen_orig + 1))
  {
    nTextLen=GetWindowTextWide(hWnd, *wszText_orig, nTextLen_orig);

    if (moCur.nSearchStrings)
    {
      if (**wszText_orig)
      {
        if ((nIndex=ComboBox_FindStringExactWide(hWnd, 0, *wszText_orig)) != CB_ERR)
          SendMessage(hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0);
      }
      else
      {
        for (nIndex=0; (nItemLen=ComboBox_GetLBTextLenWide(hWnd, nIndex)) != CB_ERR && nItemLen; ++nIndex);
        if (!nItemLen) SendMessage(hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0);
      }
      SendMessage(hWnd, CB_DELETESTRING, moCur.nSearchStrings - 1, 0);
      ComboBox_InsertStringWide(hWnd, 0, *wszText_orig);
      SendMessage(hWnd, CB_SETCURSEL, 0, 0);
    }

    if (nTextLen)
    {
      if (moCur.dwSearchOptions & AEFR_ESCAPESEQ)
      {
        if (*wszText=AllocWideStr(nTextLen_orig + 1))
        {
          if (!(nTextLen=EscapeStringToEscapeDataW(*wszText_orig, *wszText, nNewLine)))
          {
            LoadStringWide(hLangLib, MSG_ERROR_SYNTAX, wbuf, BUFFER_SIZE);
            API_MessageBox(GetParent(hWnd), wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
            nTextLen=-1;
          }
        }
        else nTextLen=-1;
      }
    }
  }
  return nTextLen;
}

void SaveComboboxSearch(HWND hWndFind, HWND hWndReplace)
{
  wchar_t wszRegKey[MAX_PATH];
  wchar_t wszRegValue[32];
  wchar_t *wszData;
  HWND hWnd;
  HKEY hKey;
  int nSize;
  int i;

  xprintfW(wszRegKey, L"%s\\Search", APP_REGHOMEW);
  if (RegCreateKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return;

  hWnd=hWndFind;
  xstrcpyW(wbuf, L"find%d");

  SaveW:
  for (i=0; i < moCur.nSearchStrings; ++i)
  {
    xprintfW(wszRegValue, wbuf, i);

    if ((nSize=ComboBox_GetLBTextLenWide(hWnd, i)) != CB_ERR)
    {
      ++nSize;

      if (wszData=AllocWideStr(nSize + 1))
      {
        ComboBox_GetLBTextWide(hWnd, i, wszData);
        RegSetValueExWide(hKey, wszRegValue, 0, REG_SZ, (LPBYTE)wszData, nSize * sizeof(wchar_t));
        FreeWideStr(wszData);
        continue;
      }
    }
    break;
  }
  if (nModelessType == MLT_REPLACE && hWnd != hWndReplace)
  {
    hWnd=hWndReplace;
    xstrcpyW(wbuf, L"replace%d");
    goto SaveW;
  }
  RegCloseKey(hKey);
}

int FindTextW(HWND hWnd, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen)
{
  AEFINDTEXTW ft;
  CHARRANGE cr;

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
  else return -1;

  ft.dwFlags=dwFlags;
  ft.pText=wpFindIt;
  ft.dwTextLen=nFindItLen;
  ft.nNewLine=AELB_R;

  if (SendMessage(hWnd, AEM_FINDTEXTW, 0, (LPARAM)&ft))
  {
    SetSel(hWnd, &ft.crFound, AESELT_LOCKSCROLL, NULL);
    ScrollCaret(hWnd);
    SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
    return cr.cpMin;
  }
  else SendMessage(hMainWnd, AKDN_SEARCH_ENDED, (WPARAM)hDlgModeless, 0);

  return -1;
}

int ReplaceTextW(HWND hWnd, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen, const wchar_t *wpReplaceWith, int nReplaceWithLen, BOOL bAll, int *nReplaceCount)
{
  AECHARRANGE crInitialSel=crSel;
  AECHARRANGE crRange;
  AECHARRANGE crInsert;
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

  if (IsReadOnly(hWnd)) return 0;

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

      if (dwFlags & AEFR_WHOLEWORD)
      {
        if (SendMessage(hWnd, AEM_ISDELIMITER, AEDLM_WORD|AEDLM_PREVCHAR, (LPARAM)&crRange.ciMin))
          dwFlags|=AEFR_WHOLEWORDGOODSTART;
        if (SendMessage(hWnd, AEM_ISDELIMITER, AEDLM_WORD, (LPARAM)&crRange.ciMax))
          dwFlags|=AEFR_WHOLEWORDGOODEND;
      }
    }
    else if (dwFlags & AEFR_BEGINNING)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
      bColumnSel=FALSE;

      if (dwFlags & AEFR_WHOLEWORD)
      {
        dwFlags|=AEFR_WHOLEWORDGOODSTART|AEFR_WHOLEWORDGOODEND;
      }
    }
    else if (dwFlags & AEFR_DOWN)
    {
      crRange.ciMin=crSel.ciMin;
      SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
      bColumnSel=FALSE;

      if (dwFlags & AEFR_WHOLEWORD)
      {
        if (SendMessage(hWnd, AEM_ISDELIMITER, AEDLM_WORD|AEDLM_PREVCHAR, (LPARAM)&crRange.ciMin))
          dwFlags|=AEFR_WHOLEWORDGOODSTART|AEFR_WHOLEWORDGOODEND;
        else
          dwFlags|=AEFR_WHOLEWORDGOODEND;
      }
    }
    else if (dwFlags & AEFR_UP)
    {
      SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
      crRange.ciMax=crSel.ciMax;
      bColumnSel=FALSE;

      if (dwFlags & AEFR_WHOLEWORD)
      {
        if (SendMessage(hWnd, AEM_ISDELIMITER, AEDLM_WORD, (LPARAM)&crRange.ciMax))
          dwFlags|=AEFR_WHOLEWORDGOODSTART|AEFR_WHOLEWORDGOODEND;
        else
          dwFlags|=AEFR_WHOLEWORDGOODSTART;
      }
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
      if (StrReplaceW(wszRangeText, nRangeTextLen, wpFindIt, nFindItLen, wpReplaceWith, nReplaceWithLen, dwFlags, NULL, &nResultTextLen, NULL, NULL, NULL))
      {
        if (wszResultText=AllocWideStr(nResultTextLen + 1))
        {
          //Remember selection
          if (nNewLine == AELB_ASIS)
          {
            crInitialRE.cpMin=-IndexSubtract(hWnd, NULL, &crSel.ciMin, nNewLine, FALSE);
            crInitialRE.cpMax=crInitialRE.cpMin + IndexSubtract(hWnd, &crSel.ciMax, &crSel.ciMin, nNewLine, FALSE);
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
          if (nChanges=StrReplaceW(wszRangeText, nRangeTextLen, wpFindIt, nFindItLen, wpReplaceWith, nReplaceWithLen, dwFlags, wszResultText, NULL, &nMin, &nMax, (nFirstVisible == -0x7FFFFFFF)?NULL:&nFirstVisible))
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
            ReplaceSelW(hWnd, wszResultText, nResultTextLen, bColumnSel, &crInsert.ciMin, &crInsert.ciMax);
            if (nNewLine == AELB_ASIS)
              SendMessage(hWnd, AEM_SETNEWLINE, AENL_INPUT, i);

            //Restore selection
            if (dwFlags & AEFR_SELECTION)
            {
              if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
                SetSel(hWnd, &crInsert, bInitialColumnSel, &crInsert.ciMin);
              else
                SetSel(hWnd, &crInsert, bInitialColumnSel, &crInsert.ciMax);
            }
            else
            {
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
              else if (dwFlags & AEFR_DOWN)
              {
                SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&crInitialSel.ciMin);
                crInitialSel.ciMax=crInitialSel.ciMin;
                IndexOffset(hWnd, &crInitialSel.ciMax, nMax - nMin, nNewLine);
              }
              if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
                SetSel(hWnd, &crInitialSel, bInitialColumnSel, &crInitialSel.ciMin);
              else
                SetSel(hWnd, &crInitialSel, bInitialColumnSel, &crInitialSel.ciMax);
            }

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
              SendMessage(hWnd, AEM_LINESCROLL, AESB_VERT|AESB_ALIGNTOP, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
            }
            else SendMessage(hWnd, AEM_LINESCROLL, AESB_VERT|AESB_ALIGNTOP, ciFirstVisibleBefore.nLine - ciFirstVisibleAfter.nLine);
          }
          FreeWideStr(wszResultText);
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

int StrReplaceW(const wchar_t *wpText, int nTextLen, const wchar_t *wpIt, int nItLen, const wchar_t *wpWith, int nWithLen, DWORD dwFlags, wchar_t *wszResult, int *nResultLen, int *nMin, int *nMax, int *nFirstVisible)
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
    if (dwFlags & AEFR_WHOLEWORD)
    {
      if (nTextCount == 0)
      {
        if (dwFlags & AEFR_WHOLEWORDGOODSTART)
          goto Find;
      }
      else if (AKD_wcschr(lpFrameCurrent->wszWordDelimiters, wpText[nTextCount - 1]))
        goto Find;
      goto Next;
    }

    Find:
    nMatchCount=nTextCount;
    nItCount=0;

    while (((dwFlags & AEFR_MATCHCASE) && wpText[nMatchCount] == wpIt[nItCount]) ||
           (!(dwFlags & AEFR_MATCHCASE) && WideCharLower(wpText[nMatchCount]) == WideCharLower(wpIt[nItCount])))
    {
      if (++nItCount >= nItLen)
      {
        if (dwFlags & AEFR_WHOLEWORD)
        {
          if (nMatchCount + 1 >= nTextLen)
          {
            if (dwFlags & AEFR_WHOLEWORDGOODEND)
              goto Replace;
          }
          else if (AKD_wcschr(lpFrameCurrent->wszWordDelimiters, wpText[nMatchCount + 1]))
            goto Replace;
          goto Next;
        }

        Replace:
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

    Next:
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

int EscapeStringToEscapeDataW(const wchar_t *wpInput, wchar_t *wszOutput, int nNewLine)
{
  const wchar_t *a=wpInput;
  wchar_t *b=wszOutput;
  wchar_t whex[5];
  int nDec;

  for (whex[4]='\0'; *a; ++a, ++b)
  {
    if (*a == '\\')
    {
      if (*++a == '\\') *b='\\';
      else if (*a == 'n')
      {
        if (nNewLine == NEWLINE_MAC) *b='\r';
        else if (nNewLine == NEWLINE_UNIX) *b='\n';
        else if (nNewLine == NEWLINE_WIN)
        {
          *b='\r';
          *++b='\n';
        }
      }
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

void EscapeDataToEscapeStringW(const wchar_t *wpInput, wchar_t *wszOutput)
{
  const wchar_t *a=wpInput;
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

void ReplaceSelA(HWND hWnd, const char *pData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AEREPLACESELA rs;

  rs.pText=pData;
  rs.dwTextLen=(DWORD)nDataLen;
  rs.bColumnSel=bColumnSel;
  rs.ciInsertStart=ciInsertStart;
  rs.ciInsertEnd=ciInsertEnd;
  SendMessage(hWnd, AEM_REPLACESELA, 0, (LPARAM)&rs);
}

void ReplaceSelW(HWND hWnd, const wchar_t *wpData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AEREPLACESELW rs;

  rs.pText=wpData;
  rs.dwTextLen=(DWORD)nDataLen;
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
  GETTEXTLENGTHEX gtl;

  gtl.flags=GTL_PRECISE|GTL_NUMCHARS;
  gtl.codepage=1200;
  return SendMessage(hWnd, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);
}

int GetRangeTextA(HWND hWnd, int nMin, int nMax, char **pText)
{
  TEXTRANGEA txtrngA;
  int nLen;

  if (nMax == -1)
  {
    nMax=GetTextLength(hWnd);
  }
  if (nMin <= nMax)
  {
    nLen=(nMax - nMin);

    if (*pText=(char *)API_HeapAlloc(hHeap, 0, nLen + 1))
    {
      txtrngA.chrg.cpMin=nMin;
      txtrngA.chrg.cpMax=nMax;
      txtrngA.lpstrText=*pText;
      nLen=SendMessage(hWnd, EM_GETTEXTRANGEA, 0, (LPARAM)&txtrngA);

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
  if (nMin <= nMax)
  {
    nLen=(nMax - nMin);

    if (*wpText=(wchar_t *)API_HeapAlloc(hHeap, 0, (nLen + 1) * sizeof(wchar_t)))
    {
      txtrngW.chrg.cpMin=nMin;
      txtrngW.chrg.cpMax=nMax;
      txtrngW.lpstrText=*wpText;
      nLen=SendMessage(hWnd, EM_GETTEXTRANGEW, 0, (LPARAM)&txtrngW);

      return nLen;
    }
  }
  *wpText=NULL;
  return 0;
}

int ExGetRangeTextA(HWND hWnd, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefChar, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, char **pText, int nNewLine, BOOL bFillSpaces)
{
  AETEXTRANGEA tr;
  int nLen;

  tr.cr.ciMin=*ciMin;
  tr.cr.ciMax=*ciMax;
  tr.bColumnSel=bColumnSel;
  tr.pBuffer=NULL;
  tr.dwBufferMax=(DWORD)-1;
  tr.nNewLine=nNewLine;
  tr.nCodePage=nCodePage;
  tr.lpDefaultChar=lpDefaultChar;
  tr.lpUsedDefChar=lpUsedDefChar;
  tr.bFillSpaces=bFillSpaces;

  if (nLen=SendMessage(hWnd, AEM_GETTEXTRANGEA, 0, (LPARAM)&tr))
  {
    if (pText)
    {
      if (tr.pBuffer=(char *)API_HeapAlloc(hHeap, 0, nLen))
      {
        nLen=SendMessage(hWnd, AEM_GETTEXTRANGEA, 0, (LPARAM)&tr);
      }
    }
  }
  if (pText) *pText=tr.pBuffer;
  return nLen;
}

int ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, wchar_t **wpText, int nNewLine, BOOL bFillSpaces)
{
  AETEXTRANGEW tr;
  int nLen;

  tr.cr.ciMin=*ciMin;
  tr.cr.ciMax=*ciMax;
  tr.bColumnSel=bColumnSel;
  tr.pBuffer=NULL;
  tr.dwBufferMax=(DWORD)-1;
  tr.nNewLine=nNewLine;
  tr.bFillSpaces=bFillSpaces;

  if (nLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr))
  {
    if (wpText)
    {
      if (tr.pBuffer=(wchar_t *)API_HeapAlloc(hHeap, 0, nLen * sizeof(wchar_t)))
      {
        nLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);
      }
    }
  }
  if (wpText) *wpText=tr.pBuffer;
  return nLen;
}

BOOL FreeText(LPVOID pText)
{
  if (pText)
    return API_HeapFree(hHeap, 0, (LPVOID)pText);
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

        nTargetLen=lstrlenW(wpSource);

        if (wpTarget=AllocWideStr(nTargetLen + 1))
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
          FreeWideStr(wpTarget);
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

  if (IsReadOnly(hWnd)) return FALSE;

  if (SendMessage(hWnd, AEM_GETCOLUMNSEL, 0, 0))
  {
    if (nLineRange > 0)
    {
      if (OpenClipboard(hWnd))
      {
        if (hData=GetClipboardData(CF_UNICODETEXT))
        {
          if (pData=GlobalLock(hData))
          {
            wchar_t *wpSource=(wchar_t *)pData;
            wchar_t *wpTarget;

            nSourceLen=lstrlenW(wpSource);
            nTargetLen=(nSourceLen + 1) * nLineRange - 1;

            if (wpTarget=AllocWideStr(nTargetLen + 1))
            {
              for (i=0; i < nLineRange; ++i)
              {
                nTargetCount=i * (nSourceLen + 1);
                xmemcpy(wpTarget + nTargetCount, wpSource, nSourceLen * sizeof(wchar_t));
                wpTarget[nTargetCount + nSourceLen]='\r';
              }
              wpTarget[nTargetLen]='\0';

              ReplaceSelW(hWnd, wpTarget, nTargetLen, TRUE, &crRange.ciMin, &crRange.ciMax);
              if (!AEC_IndexCompare(&crInitialSel.ciMin, &ciInitialCaret))
                SetSel(hWnd, &crRange, AESELT_COLUMNON, &crRange.ciMin);
              else
                SetSel(hWnd, &crRange, AESELT_COLUMNON, &crRange.ciMax);
              bResult=FALSE;
              FreeWideStr(wpTarget);
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
                xmemcpy(pTarget + nTargetCount, pSource, nSourceLen);
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
  }
  else
  {
    CHARRANGE cr;

    SendMessage(hWnd, EM_GETSEL, (WPARAM)&cr.cpMin, 0);
    DoEditPaste(hWnd, FALSE);
    SendMessage(hWnd, EM_GETSEL, 0, (LPARAM)&cr.cpMax);
    SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
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

BOOL CALLBACK GoToDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndNumber;
  static HWND hWndLine;
  static HWND hWndOffset;
  AECHARRANGE cr;
  int nLine=0;
  int nColumn=1;
  int nLineCount=0;
  int nNumberLen;
  int a;
  int b;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndNumber=GetDlgItem(hDlg, IDC_GOTO_NUMBER);
    hWndLine=GetDlgItem(hDlg, IDC_GOTO_LINE);
    hWndOffset=GetDlgItem(hDlg, IDC_GOTO_OFFSET);

    if (rcGotoDlg.right && rcGotoDlg.bottom)
      SetWindowPos(hDlg, 0, rcGotoDlg.left, rcGotoDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    if (nGotoType == NT_LINE)
    {
      SendMessage(hWndLine, BM_SETCHECK, BST_CHECKED, 0);
      PostMessage(hDlg, WM_COMMAND, IDC_GOTO_LINE, 0);
    }
    else if (nGotoType == NT_OFFSET)
    {
      SendMessage(hWndOffset, BM_SETCHECK, BST_CHECKED, 0);
      PostMessage(hDlg, WM_COMMAND, IDC_GOTO_OFFSET, 0);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_GOTO_LINE ||
        LOWORD(wParam) == IDC_GOTO_OFFSET)
    {
      if (LOWORD(wParam) == IDC_GOTO_LINE)
        nGotoType=NT_LINE;
      else if (LOWORD(wParam) == IDC_GOTO_OFFSET)
        nGotoType=NT_OFFSET;

      if (nGotoType == NT_LINE)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          if (!(moCur.dwStatusPosType & SPT_LINEWRAP) && lpFrameCurrent->ei.bWordWrap)
            a=SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETUNWRAPLINE, ciCaret.nLine, 0);
          else
            a=ciCaret.nLine;
          nNumberLen=xprintfW(wbuf, L"%d", a + 1);

          if (moCur.dwStatusPosType & SPT_COLUMN)
            b=SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_INDEXTOCOLUMN, MAKELONG(lpFrameCurrent->nTabStopSize, !(moCur.dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);
          else
            b=SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_INDEXTOCOLUMN, MAKELONG(1, !(moCur.dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);

          xprintfW(wbuf, L"%d:%d", a + 1, b + 1);
          SetWindowTextWide(hWndNumber, wbuf);
          SendMessage(hWndNumber, EM_SETSEL, 0, nNumberLen);
        }
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (!SendMessage(hWndNumber, EM_GETMODIFY, 0, 0))
        {
          a=-IndexSubtract(lpFrameCurrent->ei.hWndEdit, NULL, &ciCaret, AELB_ASIS, FALSE);
          SetDlgItemInt(hDlg, IDC_GOTO_NUMBER, a, FALSE);
          SendMessage(hWndNumber, EM_SETSEL, 0, -1);
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (GetWindowTextWide(hWndNumber, wbuf, BUFFER_SIZE))
      {
        //Only numeral
        for (a=0, b=0; wbuf[a]; ++a)
        {
          if (wbuf[a] >= '0' && wbuf[a] <= '9')
            wbuf2[b++]=wbuf[a];
          else if (wbuf[a] == '-' && b == 0)
            wbuf2[b++]=wbuf[a];
          else if (wbuf[a] == ':' && b > 0)
          {
            nColumn=xatoiW(wbuf + a + 1, NULL);
            nColumn=max(1, nColumn);
            break;
          }
          else if (wbuf[a] != ' ' && wbuf[a] != ',' && wbuf[a] != '.' && b > 0)
            break;
        }
        wbuf2[b]='\0';

        nLine=xatoiW(wbuf2, NULL);
      }

      if (nGotoType == NT_LINE)
      {
        nLineCount=SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETLINENUMBER, AEGL_LINECOUNT, 0);
        if (!(moCur.dwStatusPosType & SPT_LINEWRAP) && lpFrameCurrent->ei.bWordWrap)
          nLineCount=SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETUNWRAPLINE, nLineCount - 1, 0) + 1;

        if (!nLine)
        {
          LoadStringWide(hLangLib, MSG_WRONG_STRING, wbuf, BUFFER_SIZE);
          API_MessageBox(hDlg, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
          return FALSE;
        }
        else if (nLine < 0)
        {
          nLine=nLineCount + nLine + 1;
          if (nLine <= 0) nLine=1;
        }
        nLine=min(nLine, nLineCount);

        if (!(moCur.dwStatusPosType & SPT_LINEWRAP) && lpFrameCurrent->ei.bWordWrap)
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETWRAPLINE, nLine - 1, (LPARAM)&cr.ciMin);
        else
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETLINEINDEX, nLine - 1, (LPARAM)&cr.ciMin);

        cr.ciMin.nCharInLine=nColumn - 1;
        if (moCur.dwStatusPosType & SPT_COLUMN)
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_COLUMNTOINDEX, MAKELONG(lpFrameCurrent->nTabStopSize, !(moCur.dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&cr.ciMin);
        else
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_COLUMNTOINDEX, MAKELONG(1, !(moCur.dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&cr.ciMin);
      }
      else if (nGotoType == NT_OFFSET)
      {
        if (nLine >= 0)
        {
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&cr.ciMin);
          IndexOffset(lpFrameCurrent->ei.hWndEdit, &cr.ciMin, nLine, AELB_ASIS);
        }
        else
        {
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&cr.ciMin);
          IndexOffset(lpFrameCurrent->ei.hWndEdit, &cr.ciMin, nLine + 1, AELB_ASIS);
        }
      }

      //Set selection
      cr.ciMax=cr.ciMin;
      SetSel(lpFrameCurrent->ei.hWndEdit, &cr, AESELT_LOCKSCROLL, NULL);
      ScrollCaret(lpFrameCurrent->ei.hWndEdit);
    }
    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDCANCEL)
    {
      GetWindowPos(hDlg, NULL, &rcGotoDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      nModelessType=MLT_NONE;
      return TRUE;
    }
  }
  return FALSE;
}


//// Recent files

BOOL RecentFilesAlloc()
{
  if (!(lpwszRecentNames=(wchar_t (*)[MAX_PATH])API_HeapAlloc(hHeap, 0, moCur.nRecentFiles * MAX_PATH * sizeof(wchar_t))))
    return FALSE;
  if (!(lpdwRecentPositions=(DWORD *)API_HeapAlloc(hHeap, 0, moCur.nRecentFiles * sizeof(DWORD))))
    return FALSE;
  if (!(lpdwRecentCodepages=(DWORD *)API_HeapAlloc(hHeap, 0, moCur.nRecentFiles * sizeof(DWORD))))
    return FALSE;
  return TRUE;
}

void RecentFilesZero()
{
  xmemset(lpwszRecentNames, 0, moCur.nRecentFiles * MAX_PATH * sizeof(wchar_t));
  xmemset(lpdwRecentPositions, 0, moCur.nRecentFiles * sizeof(DWORD));
  xmemset(lpdwRecentCodepages, 0, moCur.nRecentFiles * sizeof(DWORD));
}

int RecentFilesRead()
{
  wchar_t wszRegKey[MAX_PATH];
  wchar_t wszRegValue[32];
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  DWORD dwCodePage;
  int i;

  //Read recent files array
  xprintfW(wszRegKey, L"%s\\Recent", APP_REGHOMEW);
  if (RegOpenKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
    return 0;

  for (i=0; i < moCur.nRecentFiles; ++i)
  {
    xprintfW(wszRegValue, L"nm%d", i);
    dwSize=MAX_PATH * sizeof(wchar_t);
    if (RegQueryValueExWide(hKey, wszRegValue, NULL, &dwType, (LPBYTE)lpwszRecentNames[i], &dwSize) != ERROR_SUCCESS)
      break;
    if (!*lpwszRecentNames[i])
      break;

    xprintfW(wszRegValue, L"ps%d", i);
    dwSize=sizeof(DWORD);
    RegQueryValueExWide(hKey, wszRegValue, NULL, &dwType, (LPBYTE)&lpdwRecentPositions[i], &dwSize);

    xprintfW(wszRegValue, L"cp%d", i);
    dwSize=sizeof(DWORD);
    if (RegQueryValueExWide(hKey, wszRegValue, NULL, &dwType, (LPBYTE)&dwCodePage, &dwSize) == ERROR_SUCCESS)
      lpdwRecentCodepages[i]=LOWORD(dwCodePage);
  }
  RegCloseKey(hKey);
  return i;
}

BOOL RecentFilesGet(const wchar_t *wpFile, int *nPosition, int *nCodePage)
{
  int i;

  if (!*wpFile) return FALSE;

  for (i=0; i < moCur.nRecentFiles && *lpwszRecentNames[i]; ++i)
  {
    if (!xstrcmpiW(lpwszRecentNames[i], wpFile))
    {
      if (nPosition) *nPosition=lpdwRecentPositions[i];
      if (nCodePage) *nCodePage=lpdwRecentCodepages[i];
      return TRUE;
    }
  }
  return FALSE;
}

BOOL RecentFilesUpdate(const wchar_t *wpFile, int nPosition, int nCodePage)
{
  int i;

  if (!*wpFile) return FALSE;

  //Update recent files array - move/add current file to the first place
  for (i=0; i < moCur.nRecentFiles && *lpwszRecentNames[i]; ++i)
  {
    if (!xstrcmpiW(lpwszRecentNames[i], wpFile))
    {
      if (nPosition == -1) nPosition=lpdwRecentPositions[i];
      if (nCodePage == -1) nCodePage=lpdwRecentCodepages[i];
      goto Move;
    }
  }
  if (i >= moCur.nRecentFiles) --i;
  if (nPosition == -1) nPosition=0;
  if (nCodePage == -1) nCodePage=moCur.nDefaultCodePage;

  Move:
  while (i > 0)
  {
    xstrcpynW(lpwszRecentNames[i], lpwszRecentNames[i-1], MAX_PATH);
    lpdwRecentPositions[i]=lpdwRecentPositions[i-1];
    lpdwRecentCodepages[i]=lpdwRecentCodepages[i-1];
    --i;
  }
  xstrcpynW(lpwszRecentNames[0], wpFile, MAX_PATH);
  lpdwRecentPositions[0]=nPosition;
  lpdwRecentCodepages[0]=nCodePage;

  return TRUE;
}

int RecentFilesDeleteOld()
{
  int a;
  int b;
  int i;
  int nDead=0;

  //Delete files from recent files array if they doesn't exist
  for (i=0; i < moCur.nRecentFiles && *lpwszRecentNames[i]; ++i)
  {
    while (*lpwszRecentNames[i] && !FileExistsWide(lpwszRecentNames[i]))
    {
      for (a=i, b=moCur.nRecentFiles - 1; a < b && *lpwszRecentNames[a]; ++a)
      {
        xstrcpynW(lpwszRecentNames[a], lpwszRecentNames[a+1], MAX_PATH);
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

void RecentFilesSave()
{
  wchar_t wszRegKey[MAX_PATH];
  wchar_t wszRegValue[32];
  HKEY hKey;
  int i;

  //Save recent files array
  xprintfW(wszRegKey, L"%s\\Recent", APP_REGHOMEW);
  if (RegCreateKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    return;

  for (i=0; i < moCur.nRecentFiles; ++i)
  {
    xprintfW(wszRegValue, L"nm%d", i);
    RegSetValueExWide(hKey, wszRegValue, 0, REG_SZ, (LPBYTE)lpwszRecentNames[i], BytesInString(lpwszRecentNames[i]));

    xprintfW(wszRegValue, L"ps%d", i);
    RegSetValueExWide(hKey, wszRegValue, 0, REG_DWORD, (LPBYTE)&lpdwRecentPositions[i], sizeof(DWORD));

    xprintfW(wszRegValue, L"cp%d", i);
    RegSetValueExWide(hKey, wszRegValue, 0, REG_DWORD, (LPBYTE)&lpdwRecentCodepages[i], sizeof(DWORD));
  }
  RegCloseKey(hKey);
}

void RecentFilesSaveCurrentFile()
{
  if (moCur.nRecentFiles && lpFrameCurrent->wszFile[0])
  {
    if (!bMainOnFinish || !nMDI || xstrcmpiW(fdLast.wszFile, lpFrameCurrent->wszFile))
    {
      RecentFilesZero();
      RecentFilesRead();
      RecentFilesUpdate(lpFrameCurrent->wszFile, AkelIndexToRichOffset(lpFrameCurrent->ei.hWndEdit, &ciCaret), lpFrameCurrent->ei.nCodePage);
      RecentFilesSave();
      bMenuRecentFiles=TRUE;
    }
  }
}

void RecentFilesMenu()
{
  int i;

  for (i=1; DeleteMenu(hMainMenu, IDM_RECENT_FILES + i, MF_BYCOMMAND); ++i);

  if (moCur.nRecentFiles && *lpwszRecentNames[0])
  {
    LoadStringWide(hLangLib, STR_RECENT_FILES_DELETE_DEAD, wbuf, BUFFER_SIZE);
    ModifyMenuWide(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING, IDM_RECENT_FILES, wbuf);

    for (i=0; i < moCur.nRecentFiles && *lpwszRecentNames[i]; ++i)
    {
      TrimPathW(wbuf2, lpwszRecentNames[i], RECENTFILES_RECORD_LENGTH);
      FixAmpW(wbuf2, wbuf, BUFFER_SIZE);
      InsertMenuWide(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING, IDM_RECENT_FILES + i + 1, wbuf);
    }
    InsertMenuWide(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_SEPARATOR, IDM_RECENT_FILES + i + 1, NULL);
  }
  else
  {
    LoadStringWide(hLangLib, STR_RECENT_FILES_EMPTY, wbuf, BUFFER_SIZE);
    ModifyMenuWide(hMainMenu, IDM_RECENT_FILES, MF_BYCOMMAND|MF_STRING|MF_GRAYED, IDM_RECENT_FILES, wbuf);
  }
}

void FreeMemoryRecentFiles()
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

int TrimPathW(wchar_t *wszResult, const wchar_t *wpPath, int nMax)
{
  int nFileLen;
  int nLen=lstrlenW(wpPath);
  const wchar_t *wpEnd=wpPath + nLen;
  const wchar_t *wpFile=wpEnd - 1;

  if (nLen <= nMax)
  {
    return xstrcpyW(wszResult, wpPath);
  }

  while (wpFile >= wpPath && *wpFile != '\\') --wpFile;

  if (wpFile >= wpPath)
  {
    nFileLen=wpEnd - wpFile;

    if (nFileLen + 3 < nMax)
    {
      return xprintfW(wszResult, L"%.%ds...%s", nMax - nFileLen - 4, wpPath, wpFile);
    }
  }
  return xprintfW(wszResult, L"...%s", wpEnd - nMax + 4);
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

void LanguageMenu()
{
  WIN32_FIND_DATAW wfdW;
  HANDLE hFind;
  int nCommand=0;
  int i;
  nLangModuleCount=0;

  for (i=1; DeleteMenu(hMainMenu, IDM_LANGUAGE + i, MF_BYCOMMAND); ++i);

  xprintfW(wbuf, L"%s\\AkelFiles\\Langs\\*.dll", wszExeDir);

  if ((hFind=FindFirstFileWide(wbuf, &wfdW)) != INVALID_HANDLE_VALUE)
  {
    i=1;

    do
    {
      if (!xstrcmpiW(moCur.wszLangModule, wfdW.cFileName))
        nCommand=IDM_LANGUAGE + i;
      GetBaseName(wfdW.cFileName, wbuf, BUFFER_SIZE);
      InsertMenuWide(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_STRING, IDM_LANGUAGE + i, wbuf);
      ++i;
    }
    while (FindNextFileWide(hFind, &wfdW));

    nLangModuleCount=i - 1;
    InsertMenuWide(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_SEPARATOR, IDM_LANGUAGE + i, NULL);
    FindClose(hFind);
  }

  LoadStringWide(hLangLib, STR_LANGUAGE_NONE, wbuf, BUFFER_SIZE);
  if (!*moCur.wszLangModule)
    nCommand=IDM_LANGUAGE;
  ModifyMenuWide(hMainMenu, IDM_LANGUAGE, MF_BYCOMMAND|MF_STRING, IDM_LANGUAGE, wbuf);

  CheckMenuRadioItem(hMainMenu, nCommand, nCommand, nCommand, MF_BYCOMMAND);
}


//// Code page popup menu

void FillMenuPopupCodepage()
{
  int i;

  if (lpCodepageList)
  {
    while (DeleteMenu(hPopupOpenCodepage, 1, MF_BYPOSITION));
    while (DeleteMenu(hPopupSaveCodepage, 1, MF_BYPOSITION));

    for (i=0; lpCodepageList[i]; ++i)
    {
      GetCodePageName(lpCodepageList[i], wbuf, BUFFER_SIZE);

      if (!i)
      {
        ModifyMenuWide(hPopupOpenCodepage, i, MF_BYPOSITION|MF_STRING, IDM_POPUP_OPENAS, wbuf);
        ModifyMenuWide(hPopupSaveCodepage, i, MF_BYPOSITION|MF_STRING, IDM_POPUP_SAVEAS, wbuf);
      }
      else
      {
        InsertMenuWide(hPopupOpenCodepage, i + 1, MF_BYPOSITION|MF_STRING, IDM_POPUP_OPENAS + i, wbuf);
        InsertMenuWide(hPopupSaveCodepage, i + 1, MF_BYPOSITION|MF_STRING, IDM_POPUP_SAVEAS + i, wbuf);
      }
    }
  }
}

void ShowMenuPopupCodepage(POINT *pt)
{
  NCONTEXTMENU ncm;

  ncm.hWnd=hStatus;
  ncm.uType=NCM_STATUS;
  ncm.pt=*pt;
  ncm.bProcess=TRUE;
  SendMessage(hMainWnd, AKDN_CONTEXTMENU, 0, (LPARAM)&ncm);

  if (ncm.bProcess)
  {
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

  if (IsReadOnly(hWnd)) return;

  nFirstLine=SaveLineScroll(hWnd);
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

      if (wszText=AllocWideStr(nUnicodeLen))
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

        FreeWideStr(wszText);
      }
      if (szText) API_HeapFree(hHeap, 0, (LPVOID)szText);
    }
    FreeText(wszSelText);
  }
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
  RestoreLineScroll(hWnd, nFirstLine);
}

BOOL CALLBACK RecodeDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

    FillComboboxCodepage(hWndCodePageFrom, lpCodepageList);
    FillComboboxCodepage(hWndCodePageTo, lpCodepageList);

    if (!nCodePageFrom || SelectComboboxCodepage(hWndCodePageFrom, nCodePageFrom) == CB_ERR)
      SendMessage(hWndCodePageFrom, CB_SETCURSEL, 0, 0);
    if (!nCodePageTo || SelectComboboxCodepage(hWndCodePageTo, nCodePageTo) == CB_ERR)
      SendMessage(hWndCodePageTo, CB_SETCURSEL, 0, 0);

    if (rcRecodeDlg.right && rcRecodeDlg.bottom)
      SetWindowPos(hDlg, 0, rcRecodeDlg.left, rcRecodeDlg.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

    SendMessage(hDlg, WM_COMMAND, IDC_SETREADONLY, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SETREADONLY)
    {
      EnableWindow(GetDlgItem(hDlg, IDOK), !lpFrameCurrent->ei.bReadOnly);
    }
    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDCANCEL)
    {
      if (LOWORD(wParam) == IDOK)
      {
        nCodePageFrom=GetComboboxCodepage(hWndCodePageFrom);
        nCodePageTo=GetComboboxCodepage(hWndCodePageTo);
      }
      GetWindowPos(hDlg, NULL, &rcRecodeDlg);
      DestroyWindow(hDlg);
      hDlgModeless=NULL;
      nModelessType=MLT_NONE;

      if (LOWORD(wParam) == IDOK)
      {
        RecodeTextW(lpFrameCurrent->ei.hWndEdit, nCodePageFrom, nCodePageTo);
      }
      return TRUE;
    }
  }
  return FALSE;
}


//// Colors

BOOL CALLBACK ColorsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndThemeName;
  static HWND hWndThemeSave;
  static HWND hWndThemeDelete;
  static HWND hWndList;
  static HWND hWndOK;
  static HWND hWndCancel;
  static AECOLORS aecColorsDlg;
  static BOOL bColorsChanged;
  static DIALOGRESIZE drs[]={{&hWndThemeName,   DRS_SIZE|DRS_X, 0},
                             {&hWndThemeSave,   DRS_MOVE|DRS_X, 0},
                             {&hWndThemeDelete, DRS_MOVE|DRS_X, 0},
                             {&hWndList,        DRS_SIZE|DRS_X, 0},
                             {&hWndList,        DRS_SIZE|DRS_Y, 0},
                             {&hWndOK,          DRS_MOVE|DRS_X, 0},
                             {&hWndOK,          DRS_MOVE|DRS_Y, 0},
                             {&hWndCancel,      DRS_MOVE|DRS_X, 0},
                             {&hWndCancel,      DRS_MOVE|DRS_Y, 0},
                             {0, 0, 0}};

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

      LoadStringWide(hLangLib, STR_ELEMENT, wbuf, BUFFER_SIZE);
      lvcW.cx=110;
      lvcW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertColumnWide(hWndList, LVSI_COLOR_ELEMENT, &lvcW);

      LoadStringWide(hLangLib, STR_TEXT, wbuf, BUFFER_SIZE);
      lvcW.cx=70;
      lvcW.iSubItem=LVSI_COLOR_TEXT;
      ListView_InsertColumnWide(hWndList, LVSI_COLOR_TEXT, &lvcW);

      LoadStringWide(hLangLib, STR_BACKGROUND, wbuf, BUFFER_SIZE);
      lvcW.cx=70;
      lvcW.iSubItem=LVSI_COLOR_BACKGROUND;
      ListView_InsertColumnWide(hWndList, LVSI_COLOR_BACKGROUND, &lvcW);

      LoadStringWide(hLangLib, STR_SAMPLE, wbuf, BUFFER_SIZE);
      lvcW.cx=70;
      lvcW.iSubItem=LVSI_COLOR_SAMPLE;
      ListView_InsertColumnWide(hWndList, LVSI_COLOR_SAMPLE, &lvcW);
    }

    //Rows
    {
      LVITEMW lviW;
      int i;

      LoadStringWide(hLangLib, STR_BASIC, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_BASIC;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertItemWide(hWndList, &lviW);

      LoadStringWide(hLangLib, STR_SELECTION, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_SELECTION;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertItemWide(hWndList, &lviW);

      LoadStringWide(hLangLib, STR_ACTIVELINE, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_ACTIVELINE;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertItemWide(hWndList, &lviW);

      LoadStringWide(hLangLib, STR_ACTIVECOLUMN, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_ACTIVECOLUMN;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertItemWide(hWndList, &lviW);

      LoadStringWide(hLangLib, STR_COLUMNMARKER, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_COLUMNMARKER;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertItemWide(hWndList, &lviW);

      LoadStringWide(hLangLib, STR_CARET, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_CARET;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertItemWide(hWndList, &lviW);

      LoadStringWide(hLangLib, STR_URL, wbuf, BUFFER_SIZE);
      lviW.mask=LVIF_TEXT;
      lviW.pszText=wbuf;
      lviW.iItem=LVI_COLOR_URL;
      lviW.iSubItem=LVSI_COLOR_ELEMENT;
      ListView_InsertItemWide(hWndList, &lviW);

      //Set "Sample" text
      LoadStringWide(hLangLib, STR_SAMPLE, wbuf, BUFFER_SIZE);

      for (i=LVI_COLOR_BASIC; i < LVI_COLOR_ACTIVECOLUMN; ++i)
      {
        lviW.mask=LVIF_TEXT;
        lviW.pszText=wbuf;
        lviW.iItem=i;
        lviW.iSubItem=LVSI_COLOR_SAMPLE;
        ListView_SetItemWide(hWndList, &lviW);
      }
    }

    //Combobox
    {
      COLORTHEME *ctElement;
      int nSelection=-1;

      FillComboboxThemes(hWndThemeName);

      aecColorsDlg=lpFrameCurrent->aec;

      if (!aecColorsDlg.dwFlags)
      {
        aecColorsDlg.dwFlags=AECLR_DEFAULT|AECLR_ALL;
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETCOLORS, 0, (LPARAM)&aecColorsDlg);
      }
      if (ctElement=StackThemeGetByColors(&hThemesStack, &aecColorsDlg))
        nSelection=ComboBox_FindStringWide(hWndThemeName, -1, ctElement->wszName);
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

        SetWindowLongWide(hDlg, DWL_MSGRESULT, (LONG)lResult);
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
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crBasicText);
            else if (lvhti.iItem == LVI_COLOR_SELECTION)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crSelText);
            else if (lvhti.iItem == LVI_COLOR_ACTIVELINE)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crActiveLineText);
            else if (lvhti.iItem == LVI_COLOR_ACTIVECOLUMN)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crActiveColumn);
            else if (lvhti.iItem == LVI_COLOR_COLUMNMARKER)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crColumnMarker);
            else if (lvhti.iItem == LVI_COLOR_CARET)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crCaret);
            else if (lvhti.iItem == LVI_COLOR_URL)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crUrlText);
          }
          else if (lvhti.iSubItem == LVSI_COLOR_BACKGROUND)
          {
            if (lvhti.iItem == LVI_COLOR_BASIC)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crBasicBk);
            else if (lvhti.iItem == LVI_COLOR_SELECTION)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crSelBk);
            else if (lvhti.iItem == LVI_COLOR_ACTIVELINE)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crActiveLineBk);
            else if (lvhti.iItem == LVI_COLOR_ACTIVECOLUMN)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crActiveColumn);
            else if (lvhti.iItem == LVI_COLOR_COLUMNMARKER)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crColumnMarker);
            else if (lvhti.iItem == LVI_COLOR_CARET)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crCaret);
            else if (lvhti.iItem == LVI_COLOR_URL)
              bResult=SelectColorDialog(hDlg, &aecColorsDlg.crUrlText);
          }
          if (bResult)
          {
            COLORTHEME *ctElement;
            int nSelection=-1;

            if (ctElement=StackThemeGetByColors(&hThemesStack, &aecColorsDlg))
              nSelection=ComboBox_FindStringWide(hWndThemeName, -1, ctElement->wszName);
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
        COLORTHEME *ctElement;
        int nSelection=-1;
        int nNameLen;

        if (nNameLen=GetWindowTextWide(hWndThemeName, wbuf, BUFFER_SIZE))
        {
          if (ctElement=StackThemeGetByName(&hThemesStack, wbuf))
          {
            nSelection=ComboBox_FindStringWide(hWndThemeName, -1, ctElement->wszName);
          }
          ctElement=StackThemeGetByColors(&hThemesStack, &aecColorsDlg);
        }

        if (!nSelection)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else
        {
          EnableWindow(hWndThemeSave, (nNameLen && (nSelection < 0 || !ctElement))?TRUE:FALSE);
          EnableWindow(hWndThemeDelete, TRUE);
        }
      }
      else if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        COLORTHEME *ctElement;
        int nSelection;

        if ((nSelection=SendMessage(hWndThemeName, CB_GETCURSEL, 0, 0)) != CB_ERR)
        {
          if (ComboBox_GetLBTextWide(hWndThemeName, nSelection, wbuf))
          {
            if (ctElement=StackThemeGetByName(&hThemesStack, wbuf))
            {
              aecColorsDlg=ctElement->aec;
              bColorsChanged=TRUE;
              InvalidateRect(hWndList, NULL, TRUE);
            }
          }
        }

        if (!nSelection)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
        else
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_COLORS_THEME_SAVE)
    {
      COLORTHEME *ctElement;
      wchar_t wszThemeName[MAX_PATH];
      BOOL bProcess=TRUE;

      if (GetWindowTextWide(hWndThemeName, wszThemeName, MAX_PATH))
      {
        if (ctElement=StackThemeGetByName(&hThemesStack, wszThemeName))
        {
          LoadStringWide(hLangLib, MSG_OVERWRITEPROMPT, wbuf, BUFFER_SIZE);
          xprintfW(wbuf2, wbuf, ctElement->wszName);
          if (API_MessageBox(hDlg, wbuf2, APP_MAIN_TITLEW, MB_YESNO|MB_ICONQUESTION) == IDYES)
          {
            xstrcpynW(ctElement->wszName, wszThemeName, MAX_PATH);
            ctElement->aec=aecColorsDlg;
          }
          else bProcess=FALSE;
        }
        else
        {
          StackThemeAdd(&hThemesStack, wszThemeName, &aecColorsDlg);
          ComboBox_AddStringWide(hWndThemeName, wszThemeName);
        }

        if (bProcess)
        {
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_COLORS_THEME_DELETE)
    {
      if (GetWindowTextWide(hWndThemeName, wbuf, BUFFER_SIZE))
      {
        COLORTHEME *ctElement;
        int nSelection;

        if (ctElement=StackThemeGetByName(&hThemesStack, wbuf))
        {
          StackThemeDelete(&hThemesStack, ctElement);
          nSelection=ComboBox_FindStringWide(hWndThemeName, -1, ctElement->wszName);
          SendMessageW(hWndThemeName, CB_DELETESTRING, nSelection, 0);
          SendMessage(hWndThemeName, CB_SETCURSEL, (WPARAM)-1, 0);

          SetFocus(hWndThemeName);
          EnableWindow(hWndThemeSave, FALSE);
          EnableWindow(hWndThemeDelete, FALSE);
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (bColorsChanged)
      {
        lpFrameCurrent->aec=aecColorsDlg;
        lpFrameCurrent->aec.dwFlags=AECLR_ALL;
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETCOLORS, 0, (LPARAM)&lpFrameCurrent->aec);
      }
      SaveThemes(moCur.nSaveSettings);

      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  DialogResizeMessages(&drs[0], &rcColorsInitDialog, &moCur.rcColorsCurrentDialog, DRM_GETMINMAXINFO|DRM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

void FillComboboxThemes(HWND hWnd)
{
  COLORTHEME *ctElement=(COLORTHEME *)hThemesStack.first;

  while (ctElement)
  {
    ComboBox_AddStringWide(hWnd, ctElement->wszName);

    ctElement=ctElement->next;
  }
}

COLORTHEME* StackThemeAdd(HSTACK *hStack, const wchar_t *wpName, AECOLORS *aec)
{
  COLORTHEME *ctElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&ctElement, -1, sizeof(COLORTHEME)))
  {
    xstrcpynW(ctElement->wszName, wpName, MAX_PATH);
    ctElement->nNameLen=lstrlenW(ctElement->wszName);
    if (aec) ctElement->aec=*aec;
  }
  return ctElement;
}

COLORTHEME* StackThemeGetByName(HSTACK *hStack, const wchar_t *wpName)
{
  COLORTHEME *ctElement=(COLORTHEME *)hStack->first;
  int nNameLen=lstrlenW(wpName);

  while (ctElement)
  {
    if (ctElement->nNameLen == nNameLen)
      if (!xstrcmpiW(ctElement->wszName, wpName))
        return ctElement;

    ctElement=ctElement->next;
  }
  return NULL;
}

COLORTHEME* StackThemeGetByColors(HSTACK *hStack, AECOLORS *aec)
{
  COLORTHEME *ctElement=(COLORTHEME *)hStack->first;

  while (ctElement)
  {
    if (!xmemcmp(&ctElement->aec.crCaret, &aec->crCaret, sizeof(AECOLORS) - sizeof(DWORD)))
      return ctElement;

    ctElement=ctElement->next;
  }
  return NULL;
}

void StackThemeDelete(HSTACK *hStack, COLORTHEME *ctElement)
{
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)ctElement);
}

void StackThemeFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void ReadThemes(MAINOPTIONS *mo)
{
  AECOLORS aec;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSizeValue;
  DWORD dwSizeString;
  DWORD dwIndex=0;

  //Standard theme
  LoadStringWide(hLangLib, STR_STANDARDTHEME, wbuf, BUFFER_SIZE);
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
  StackThemeAdd(&hThemesStack, wbuf, &aec);

  if (mo->nSaveSettings == SS_REGISTRY)
  {
    wchar_t wszRegKey[MAX_PATH];

    xprintfW(wszRegKey, L"%s\\Themes", APP_REGHOMEW);
    if (RegOpenKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      while (1)
      {
        dwSizeValue=BUFFER_SIZE;
        dwSizeString=sizeof(AECOLORS);
        if (RegEnumValueWide(hKey, dwIndex, wbuf, &dwSizeValue, NULL, &dwType, (LPBYTE)&aec, &dwSizeString) != ERROR_SUCCESS)
          break;

        StackThemeAdd(&hThemesStack, wbuf, &aec);
        ++dwIndex;
      }
      RegCloseKey(hKey);
    }
  }
  else
  {
    INISECTION *lpIniSection;
    INIKEY *lpIniKey;

    if (lpIniSection=StackOpenIniSectionW(&hIniFile, L"Themes", lstrlenW(L"Themes"), FALSE))
    {
      lpIniKey=(INIKEY *)lpIniSection->first;

      while (lpIniKey)
      {
        StackGetIniData(lpIniKey, INI_BINARY, (LPBYTE)&aec, sizeof(AECOLORS));
        StackThemeAdd(&hThemesStack, lpIniKey->wszKey, &aec);
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
    StackThemeAdd(&hThemesStack, L"Notepad++", &aec);
  }
}

BOOL SaveThemes(int nSaveSettings)
{
  COLORTHEME *ctElement=(COLORTHEME *)hThemesStack.first;
  HKEY hKey;
  INISECTION *lpIniSection;
  BOOL bResult=FALSE;

  if (nSaveSettings == SS_REGISTRY)
  {
    wchar_t wszRegKey[MAX_PATH];

    xprintfW(wszRegKey, L"%s\\Themes", APP_REGHOMEW);
    //Clean old
    RegClearKeyWide(HKEY_CURRENT_USER, wszRegKey);
    if (RegCreateKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!OpenIniW(&hIniFile, wszIniFile, TRUE))
      return FALSE;

    //Clean old
    if (lpIniSection=StackOpenIniSectionW(&hIniFile, L"Themes", lstrlenW(L"Themes"), FALSE))
      StackDeleteIniSection(&hIniFile, lpIniSection, TRUE);
  }

  //Skip "Standard" theme
  if (ctElement) ctElement=ctElement->next;

  while (ctElement)
  {
    if (nSaveSettings == SS_REGISTRY)
    {
      if (RegSetValueExWide(hKey, ctElement->wszName, 0, REG_BINARY, (LPBYTE)&ctElement->aec, sizeof(AECOLORS)) != ERROR_SUCCESS)
        break;
    }
    else
    {
      if (!IniSetValueW(&hIniFile, L"Themes", ctElement->wszName, INI_BINARY, (LPBYTE)&ctElement->aec, sizeof(AECOLORS)))
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
    bResult=SaveIniW(&hIniFile, wszIniFile);
    StackFreeIni(&hIniFile);
  }
  return bResult;
}


//// Plugins stack

void RegisterPluginsHotkeys(MAINOPTIONS *mo)
{
  if (mo->nSaveSettings == SS_REGISTRY)
  {
    wchar_t wszRegKey[MAX_PATH];
    HKEY hKey;
    DWORD dwType;
    DWORD dwSizeValue;
    DWORD dwSizeString;
    DWORD dwIndex=0;
    DWORD dwHotkey=0;

    xprintfW(wszRegKey, L"%s\\Plugs", APP_REGHOMEW);
    if (RegOpenKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      while (1)
      {
        dwSizeValue=BUFFER_SIZE;
        dwSizeString=sizeof(DWORD);
        if (RegEnumValueWide(hKey, dwIndex++, wbuf, &dwSizeValue, NULL, &dwType, (LPBYTE)&dwHotkey, &dwSizeString) != ERROR_SUCCESS)
          break;

        if (dwHotkey)
        {
          StackPluginAdd(&hPluginsStack, wbuf, lstrlenW(wbuf), LOWORD(dwHotkey), HIWORD(dwHotkey), NULL, NULL);
        }
      }
      RegCloseKey(hKey);
    }
  }
  else
  {
    INISECTION *lpIniSection;
    INIKEY *lpIniKey;
    DWORD dwHotkey=0;

    if (lpIniSection=StackOpenIniSectionW(&hIniFile, L"Plugs", lstrlenW(L"Plugs"), FALSE))
    {
      lpIniKey=(INIKEY *)lpIniSection->first;

      while (lpIniKey)
      {
        if (dwHotkey=(DWORD)xatoiW(lpIniKey->wszString, NULL))
        {
          StackPluginAdd(&hPluginsStack, lpIniKey->wszKey, lpIniKey->nKeyBytes / sizeof(wchar_t) - 1, LOWORD(dwHotkey), HIWORD(dwHotkey), NULL, NULL);
        }
        lpIniKey=lpIniKey->next;
      }
    }
  }
}

PLUGINFUNCTION* StackPluginFind(HSTACK *hStack, const wchar_t *wpPluginFunction, int nPluginFunctionLen)
{
  PLUGINFUNCTION *pfElement=(PLUGINFUNCTION *)hStack->first;

  if (!wpPluginFunction) return NULL;
  if (nPluginFunctionLen == -1)
    nPluginFunctionLen=lstrlenW(wpPluginFunction);

  while (pfElement)
  {
    if (pfElement->nFunctionLen == nPluginFunctionLen)
    {
      if (!xstrcmpiW(pfElement->wszFunction, wpPluginFunction))
        break;
    }
    pfElement=pfElement->next;
  }
  return pfElement;
}

PLUGINFUNCTION* StackHotkeyFind(HSTACK *hStack, WORD wHotkey)
{
  PLUGINFUNCTION *pfElement=(PLUGINFUNCTION *)hStack->first;

  while (pfElement)
  {
    if (pfElement->wHotkey == wHotkey)
        break;

    pfElement=pfElement->next;
  }
  return pfElement;
}

PLUGINFUNCTION* StackPluginAdd(HSTACK *hStack, const wchar_t *wpPluginFunction, int nPluginFunctionLen, WORD wHotkey, BOOL bAutoLoad, PLUGINPROC PluginProc, void *lpParameter)
{
  PLUGINFUNCTION *pfElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&pfElement, -1, sizeof(PLUGINFUNCTION)))
  {
    pfElement->wHotkey=wHotkey;
    pfElement->bAutoLoad=bAutoLoad;
    pfElement->bRunning=FALSE;
    pfElement->PluginProc=PluginProc;
    pfElement->lpParameter=lpParameter;

    if (nPluginFunctionLen == -1)
      pfElement->nFunctionLen=lstrlenW(wpPluginFunction);
    else
      pfElement->nFunctionLen=nPluginFunctionLen;
    pfElement->pFunction=bOldWindows?(LPBYTE)pfElement->szFunction:(LPBYTE)pfElement->wszFunction;
    xstrcpynW(pfElement->wszFunction, wpPluginFunction, MAX_PATH);
    WideCharToMultiByte(CP_ACP, 0, pfElement->wszFunction, pfElement->nFunctionLen + 1, pfElement->szFunction, MAX_PATH, NULL, NULL);
  }
  return pfElement;
}

void StackPluginDelete(HSTACK *hStack, void *lpElement)
{
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement);
}

BOOL StackPluginSave(HSTACK *hStack, int nSaveSettings)
{
  PLUGINFUNCTION *pfElement=(PLUGINFUNCTION *)hStack->first;
  HKEY hKey;
  INISECTION *lpIniSection;
  DWORD dwHotkey;
  BOOL bResult=FALSE;

  if (nSaveSettings == SS_REGISTRY)
  {
    wchar_t wszRegKey[MAX_PATH];

    xprintfW(wszRegKey, L"%s\\Plugs", APP_REGHOMEW);
    //Clean old
    RegClearKeyWide(HKEY_CURRENT_USER, wszRegKey);
    if (RegCreateKeyExWide(HKEY_CURRENT_USER, wszRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
      return FALSE;
  }
  else
  {
    if (!OpenIniW(&hIniFile, wszIniFile, TRUE))
      return FALSE;

    //Clean old
    if (lpIniSection=StackOpenIniSectionW(&hIniFile, L"Plugs", lstrlenW(L"Plugs"), FALSE))
      StackDeleteIniSection(&hIniFile, lpIniSection, TRUE);
  }

  while (pfElement)
  {
    if (IsMainFunctionW(pfElement->wszFunction))
    {
      if (pfElement->wHotkey || pfElement->bAutoLoad)
      {
        dwHotkey=MAKELONG(pfElement->wHotkey, pfElement->bAutoLoad);

        if (nSaveSettings == SS_REGISTRY)
        {
          if (RegSetValueExWide(hKey, pfElement->wszFunction, 0, REG_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)) != ERROR_SUCCESS)
            break;
        }
        else
        {
          if (!IniSetValueW(&hIniFile, L"Plugs", pfElement->wszFunction, INI_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD)))
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
    bResult=SaveIniW(&hIniFile, wszIniFile);
    StackFreeIni(&hIniFile);
  }
  return bResult;
}

void StackPluginCleanUp(HSTACK *hStack, BOOL bDeleteNonExistentDLL)
{
  PLUGINFUNCTION *pfElement=(PLUGINFUNCTION *)hStack->first;
  PLUGINFUNCTION *pfNextElement;
  wchar_t wszDLL[MAX_PATH];
  wchar_t wszPlugin[MAX_PATH];

  while (pfElement)
  {
    pfNextElement=pfElement->next;

    if (!pfElement->wHotkey && !pfElement->bAutoLoad && !pfElement->bRunning)
    {
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)pfElement);
      pfElement=NULL;
    }
    if (bDeleteNonExistentDLL && pfElement)
    {
      if (ParsePluginNameW(pfElement->wszFunction, wszPlugin, NULL))
      {
        xprintfW(wszDLL, L"%s\\AkelFiles\\Plugs\\%s.dll", wszExeDir, wszPlugin);

        if (!FileExistsWide(wszDLL))
        {
          StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)pfElement);
          pfElement=NULL;
        }
      }
    }
    pfElement=pfNextElement;
  }
}

void StackPluginFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void CallPluginsOnStart(HSTACK *hStack)
{
  PLUGINFUNCTION *pfElement=(PLUGINFUNCTION *)hStack->first;
  PLUGINFUNCTION *pfNextElement;
  PLUGINCALLSENDW pcs;

  while (pfElement)
  {
    pfNextElement=pfElement->next;

    if (pfElement->bAutoLoad)
    {
      pcs.pFunction=pfElement->wszFunction;
      pcs.lParam=0;
      //pcs.dwSupport=0;
      CallPluginSend(&pfElement, &pcs, DLLCF_ONPROGRAMLOAD);
    }
    pfElement=pfNextElement;
  }
}

int CallPluginSend(PLUGINFUNCTION **ppfElement, PLUGINCALLSENDW *pcs, DWORD dwFlags)
{
  PLUGINFUNCTION *pfElement=NULL;
  int nResult=UD_FAILED;

  if (pcs)
  {
    if (ppfElement)
      pfElement=*ppfElement;

    if (!pfElement)
    {
      if (!(pfElement=StackPluginFind(&hPluginsStack, pcs->pFunction, -1)))
        if (!(pfElement=StackPluginAdd(&hPluginsStack, pcs->pFunction, lstrlenW(pcs->pFunction), 0, FALSE, NULL, NULL)))
          return UD_FAILED;
    }

    if (pfElement && pfElement->PluginProc)
    {
      if ((pfElement->PluginProc)(pfElement->lpParameter))
        nResult=UD_NONUNLOAD_UNCHANGE;
      else
        nResult=UD_NONUNLOAD_UNCHANGE|UD_HOTKEY_DODEFAULT;
    }
    else
    {
      nResult=CallPlugin(pfElement, pcs, dwFlags);

      if (nResult != UD_FAILED)
      {
        if (!(nResult & UD_NONUNLOAD_UNCHANGE))
        {
          if (nResult & UD_NONUNLOAD_ACTIVE)
          {
            if ((dwFlags & DLLCF_SWITCHAUTOLOAD) && !(pcs->dwSupport & PDS_NOAUTOLOAD) && !(dwFlags & DLLCF_GETSUPPORT))
            {
              if (!pfElement->bAutoLoad)
              {
                pfElement->bAutoLoad=TRUE;
                if (dwFlags & DLLCF_SAVENOW)
                  PostMessage(hMainWnd, AKD_DLLSAVE, DLLSF_NOW, 0);
                else if (dwFlags & DLLCF_SAVEONEXIT)
                  PostMessage(hMainWnd, AKD_DLLSAVE, DLLSF_ONEXIT, 0);
              }
            }
            pfElement->bRunning=TRUE;
          }
          else
          {
            if (dwFlags & DLLCF_SWITCHAUTOLOAD)
            {
              if (pfElement->bAutoLoad)
              {
                pfElement->bAutoLoad=FALSE;
                if (dwFlags & DLLCF_SAVENOW)
                  PostMessage(hMainWnd, AKD_DLLSAVE, DLLSF_NOW, 0);
                else if (dwFlags & DLLCF_SAVEONEXIT)
                  PostMessage(hMainWnd, AKD_DLLSAVE, DLLSF_ONEXIT, 0);
              }
            }
            if (pfElement->wHotkey || pfElement->bAutoLoad)
            {
              pfElement->bRunning=FALSE;
            }
            else
            {
              StackPluginDelete(&hPluginsStack, pfElement);
              pfElement=NULL;
            }
          }
        }
      }
      else
      {
        //if (!pfElement->wHotkey && !pfElement->bAutoLoad)
        {
          StackPluginDelete(&hPluginsStack, pfElement);
          pfElement=NULL;
        }
      }
    }

    if (ppfElement)
      *ppfElement=pfElement;
  }
  return nResult;
}

int CallPlugin(PLUGINFUNCTION *lpPluginFunction, PLUGINCALLSENDW *pcs, DWORD dwFlags)
{
  wchar_t wszPlugin[MAX_PATH];
  wchar_t wszFunction[MAX_PATH];
  wchar_t wszDLL[MAX_PATH];
  wchar_t wszPluginWord[MAX_PATH];
  char szFunction[MAX_PATH];
  char szFullName[MAX_PATH];
  HMODULE hModule;
  PLUGINVERSION pv;
  PLUGINDATA pd;
  BOOL bInMemory=TRUE;
  BOOL bCalled=FALSE;
  int nWordLen;
  void (*PluginIDPtr)(PLUGINVERSION *);
  void (*PluginFunctionPtr)(PLUGINDATA *);

  if (pcs->pFunction)
  {
    if (ParsePluginNameW(pcs->pFunction, wszPlugin, wszFunction))
    {
      WideCharToMultiByte(CP_ACP, 0, wszFunction, -1, szFunction, MAX_PATH, NULL, NULL);
      WideCharToMultiByte(CP_ACP, 0, pcs->pFunction, -1, szFullName, MAX_PATH, NULL, NULL);
      xprintfW(wszDLL, L"%s\\AkelFiles\\Plugs\\%s.dll", wszExeDir, wszPlugin);
      nWordLen=LoadStringWide(hLangLib, STR_PLUGIN, wbuf, BUFFER_SIZE);
      wbuf[0]=WideCharLower(wbuf[0]);
      xprintfW(wszPluginWord, L"%s %s", wszPlugin, wbuf);

      if (!(hModule=GetModuleHandleWide(wszDLL)))
      {
        if (hModule=LoadLibraryWide(wszDLL))
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
            if (pv.dwExeMinVersion4x != MAKE_IDENTIFIER(-1, -1, -1, -1))
            {
              if (VersionCompare(pv.dwExeMinVersion4x, dwExeVersion) <= 0)
              {
                if (PluginFunctionPtr=(void (*)(PLUGINDATA *))GetProcAddress(hModule, szFunction))
                {
                  pd.cb=sizeof(PLUGINDATA);
                  pd.dwSupport=(dwFlags & DLLCF_GETSUPPORT)?PDS_GETSUPPORT:PDS_SUPPORTALL;
                  pd.pFunction=bOldWindows?(LPBYTE)szFullName:(LPBYTE)pcs->pFunction;
                  pd.szFunction=szFullName;
                  pd.wszFunction=pcs->pFunction;
                  pd.hInstanceDLL=hModule;
                  pd.lpPluginFunction=lpPluginFunction;
                  pd.nUnload=UD_UNLOAD;
                  pd.bInMemory=bInMemory;
                  pd.bOnStart=(dwFlags & DLLCF_ONPROGRAMLOAD);
                  pd.lParam=pcs->lParam;
                  pd.pAkelDir=bOldWindows?(LPBYTE)szExeDir:(LPBYTE)wszExeDir;
                  pd.szAkelDir=szExeDir;
                  pd.wszAkelDir=wszExeDir;
                  pd.hInstanceEXE=hInstance;
                  pd.hPluginsStack=&hPluginsStack;
                  pd.hMainWnd=hMainWnd;
                  pd.hWndEdit=lpFrameCurrent->ei.hWndEdit;
                  pd.hDocEdit=lpFrameCurrent->ei.hDocEdit;
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
                  pd.nMDI=nMDI;
                  pd.nSaveSettings=moCur.nSaveSettings;
                  pd.pLangModule=bOldWindows?(LPBYTE)moCur.szLangModule:(LPBYTE)moCur.wszLangModule;
                  pd.szLangModule=moCur.szLangModule;
                  pd.wszLangModule=moCur.wszLangModule;
                  pd.wLangSystem=(WORD)dwLangSystem;
                  pd.wLangModule=(WORD)dwLangModule;

                  (*PluginFunctionPtr)(&pd);
                  SendMessage(hMainWnd, AKDN_DLLCALL, 0, (LPARAM)&pd);

                  pcs->dwSupport=pd.dwSupport;
                  if ((dwFlags & DLLCF_GETSUPPORT) && bInMemory)
                    return UD_NONUNLOAD_UNCHANGE;
                  if ((pd.nUnload & UD_NONUNLOAD_ACTIVE) ||
                      (pd.nUnload & UD_NONUNLOAD_NONACTIVE) ||
                      (pd.nUnload & UD_NONUNLOAD_UNCHANGE))
                    return pd.nUnload;
                  bCalled=TRUE;
                }
                else
                {
                  LoadStringWide(hLangLib, MSG_FUNCTION_NOT_FOUND, wbuf, BUFFER_SIZE);
                  xprintfW(wbuf2, wbuf, wszPlugin, wszFunction, wszDLL);
                  API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
                  if (bInMemory) return UD_FAILED;
                }
              }
              else
              {
                LoadStringWide(hLangLib, MSG_UPDATE_PROGRAM, wbuf, BUFFER_SIZE);
                xprintfW(wbuf2, wbuf, LOBYTE(pv.dwExeMinVersion4x), HIBYTE(pv.dwExeMinVersion4x), LOBYTE(HIWORD(pv.dwExeMinVersion4x)), HIBYTE(HIWORD(pv.dwExeMinVersion4x)),
                                      LOBYTE(dwExeVersion), HIBYTE(dwExeVersion), LOBYTE(HIWORD(dwExeVersion)), HIBYTE(HIWORD(dwExeVersion)));
                API_MessageBox(hMainWnd, wbuf2, wszPluginWord, MB_OK|MB_ICONEXCLAMATION);
              }
            }
            else
            {
              LoadStringWide(hLangLib, MSG_PROGRAM_IS_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
              xprintfW(wbuf2, wbuf, wszDLL);
              API_MessageBox(hMainWnd, wbuf2, wszPluginWord, MB_OK|MB_ICONEXCLAMATION);
            }
          }
          else
          {
            wchar_t wszStr[MAX_PATH];

            if (VersionCompare(pv.dwAkelDllVersion, AKELDLL) < 0)
              xstrcpynW(wszStr, wszPluginWord, MAX_PATH);
            else
              xstrcpynW(wszStr, L"AkelPad", MAX_PATH);
            LoadStringWide(hLangLib, MSG_UPDATE_PLUGIN, wbuf, BUFFER_SIZE);
            xprintfW(wbuf2, wbuf, wszStr,
                                   LOBYTE(AKELDLL), HIBYTE(AKELDLL), LOBYTE(HIWORD(AKELDLL)), HIBYTE(HIWORD(AKELDLL)),
                                   wszPlugin,
                                   LOBYTE(pv.dwAkelDllVersion), HIBYTE(pv.dwAkelDllVersion), LOBYTE(HIWORD(pv.dwAkelDllVersion)), HIBYTE(HIWORD(pv.dwAkelDllVersion)));
            API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
          }
        }
        else
        {
          LoadStringWide(hLangLib, MSG_PLUGIN_IS_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
          xprintfW(wbuf2, wbuf, wszDLL);
          API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
        }

        if (FreeLibrary(hModule))
        {
          StackHandleDecrease(&hHandlesStack, hModule);
          if (bCalled) return pd.nUnload; //UD_UNLOAD
        }
      }
      else
      {
        LoadStringWide(hLangLib, MSG_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, wszDLL);
        API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
      }
    }
  }
  return UD_FAILED;
}

BOOL TranslateGlobal(LPMSG lpMsg)
{
  if (TranslateAcceleratorWide(hMainWnd, hGlobalAccel, lpMsg))
    return TRUE;

  if (lpMsg->message == WM_KEYDOWN)
  {
    if (lpMsg->wParam == VK_ESCAPE)
    {
      if (dwMouseCapture)
      {
        ReleaseCapture();
        return TRUE;
      }
    }
  }
  return FALSE;
}

BOOL TranslatePlugin(LPMSG lpMsg)
{
  if (lpMsg->message == AKD_POSTMESSAGE)
  {
    POSTMESSAGE *pm=(POSTMESSAGE *)lpMsg->lParam;

    if (pm)
    {
      SendMessageWide(pm->hWnd, pm->uMsg, pm->wParam, pm->lParam);
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
  else if (lpMsg->message == AKD_DLLCALL ||
           lpMsg->message == AKD_DLLCALLA ||
           lpMsg->message == AKD_DLLCALLW)
  {
    PLUGINCALLPOSTA *lpCallPostA=(PLUGINCALLPOSTA *)lpMsg->lParam;
    PLUGINCALLPOSTW *lpCallPostW=(PLUGINCALLPOSTW *)lpMsg->lParam;
    PLUGINCALLSENDW pcsW;
    wchar_t wszPluginFunction[MAX_PATH];

    if (lpMsg->lParam)
    {
      if (lpMsg->message == AKD_DLLCALLA || (bOldWindows && lpMsg->message == AKD_DLLCALL))
        xprintfW(wszPluginFunction, L"%S", (char *)lpCallPostA->szFunction);
      else
        xprintfW(wszPluginFunction, L"%s", (wchar_t *)lpCallPostW->szFunction);
      pcsW.pFunction=wszPluginFunction;
      pcsW.lParam=lpCallPostW->lParam;
      //pcsW.dwSupport=0;
      CallPluginSend(NULL, &pcsW, lpMsg->wParam);
      GlobalFree((HGLOBAL)lpMsg->lParam);
    }
    return TRUE;
  }
  else if (lpMsg->message == AKD_DLLUNLOAD)
  {
    HMODULE hInstanceDLL=(HMODULE)lpMsg->wParam;
    HANDLE hThread=(HANDLE)lpMsg->lParam;
    char szPluginName[MAX_PATH];
    wchar_t wszPluginName[MAX_PATH];

    if (hInstanceDLL)
    {
      if (hThread)
      {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
      }

      if (GetModuleFileNameWide(hInstanceDLL, wbuf, BUFFER_SIZE))
      {
        GetBaseName(wbuf, wszPluginName, MAX_PATH);
        xprintfW(wszPluginName, L"%s::", wszPluginName);
        WideCharToMultiByte(CP_ACP, 0, wszPluginName, -1, szPluginName, MAX_PATH, NULL, NULL);

        if (FreeLibrary(hInstanceDLL))
        {
          PLUGINFUNCTION *pfElement=(PLUGINFUNCTION *)hPluginsStack.first;
          PLUGINFUNCTION *pfNextElement;
          UNISTRING us;

          StackHandleDecrease(&hHandlesStack, hInstanceDLL);

          //Clean-up plugins stack
          while (pfElement)
          {
            pfNextElement=pfElement->next;

            if (!xstrcmpinW(wszPluginName, pfElement->wszFunction, (DWORD)-1))
            {
              if (pfElement->wHotkey || pfElement->bAutoLoad)
                pfElement->bRunning=FALSE;
              else
                StackPluginDelete(&hPluginsStack, pfElement);
            }
            pfElement=pfNextElement;
          }
          us.pString=bOldWindows?(LPBYTE)szPluginName:(LPBYTE)wszPluginName;
          us.szString=szPluginName;
          us.wszString=wszPluginName;
          SendMessage(hMainWnd, AKDN_DLLUNLOAD, 0, (WPARAM)&us);
        }
      }
    }
    return TRUE;
  }
  return FALSE;
}

int TranslateHotkey(HSTACK *hStack, LPMSG lpMsg)
{
  PLUGINFUNCTION *pfElement=(PLUGINFUNCTION *)hStack->first;
  BYTE nMod=0;
  WORD wHotkey;
  int nResult;

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
        PLUGINCALLSENDW pcs;

        pcs.pFunction=pfElement->wszFunction;
        pcs.lParam=0;
        //pcs.dwSupport=0;
        if (CallPluginSend(&pfElement, &pcs, 0) & UD_HOTKEY_DODEFAULT)
          break;
        return TRUE;
      }
      pfElement=pfElement->next;
    }
    nResult=FALSE;
    SendMessage(hMainWnd, AKDN_HOTKEY, (WPARAM)wHotkey, (LPARAM)&nResult);
    return nResult;
  }
  return FALSE;
}

BOOL ParsePluginNameW(const wchar_t *wpFullName, wchar_t *wszPlugin, wchar_t *wszFunction)
{
  wchar_t *wpFunction;

  if ((wpFunction=AKD_wcschr(wpFullName, ':')) && *(wpFunction + 1) == ':')
  {
    if (wszPlugin) xstrcpynW(wszPlugin, wpFullName, min(wpFunction - wpFullName + 1, MAX_PATH));
    if (wszFunction) xstrcpynW(wszFunction, wpFunction + 2, MAX_PATH);
    return TRUE;
  }
  return FALSE;
}

BOOL IsMainFunctionW(const wchar_t *wpFunction)
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


//// Plugins core

BOOL CALLBACK PluginsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
  static DIALOGRESIZE drs[]={{&hWndList,        DRS_SIZE|DRS_X, 0},
                             {&hWndList,        DRS_SIZE|DRS_Y, 0},
                             {&hWndListInfo,    DRS_MOVE|DRS_Y, 0},
                             {&hWndHotkeyLabel, DRS_MOVE|DRS_X, 0},
                             {&hWndHotkey,      DRS_MOVE|DRS_X, 0},
                             {&hWndAssign,      DRS_MOVE|DRS_X, 0},
                             {&hWndCall,        DRS_MOVE|DRS_Y, 0},
                             {&hWndOK,          DRS_MOVE|DRS_X, 0},
                             {&hWndOK,          DRS_MOVE|DRS_Y, 0},
                             {&hWndCancel,      DRS_MOVE|DRS_X, 0},
                             {&hWndCancel,      DRS_MOVE|DRS_Y, 0},
                             {0, 0, 0}};

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

    LoadStringWide(hLangLib, STR_PLUGIN, wszPlugin, MAX_PATH);
    LoadStringWide(hLangLib, STR_FUNCTION, wszFunction, MAX_PATH);
    xprintfW(wbuf, L"%s::%s", wszPlugin, wszFunction);
    lvcW.cx=209;
    lvcW.iSubItem=LVSI_FUNCTION_NAME;
    ListView_InsertColumnWide(hWndList, LVSI_FUNCTION_NAME, &lvcW);

    LoadStringWide(hLangLib, STR_HOTKEY, wbuf, BUFFER_SIZE);
    lvcW.cx=105;
    lvcW.iSubItem=LVSI_FUNCTION_HOTKEY;
    ListView_InsertColumnWide(hWndList, LVSI_FUNCTION_HOTKEY, &lvcW);

    LoadStringWide(hLangLib, STR_STATUS, wbuf, BUFFER_SIZE);
    lvcW.cx=63;
    lvcW.iSubItem=LVSI_FUNCTION_STATUS;
    ListView_InsertColumnWide(hWndList, LVSI_FUNCTION_STATUS, &lvcW);

    FillPluginList(hWndList);
    nSelItem=-1;
    bListChanged=FALSE;

    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PLUGINS_ASSIGN)
    {
      PLUGINFUNCTION *pfElement=NULL;
      PLUGINLISTITEM *pliElement;
      LVITEMW lviW;
      WORD wHotkey;

      if (wHotkey=(WORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0))
        pfElement=StackHotkeyFind(&hPluginsStack, wHotkey);

      if (!pfElement)
      {
        GetHotkeyString(wHotkey, wbuf);
        lviW.mask=LVIF_TEXT;
        lviW.pszText=wbuf;
        lviW.iItem=nSelItem;
        lviW.iSubItem=LVSI_FUNCTION_HOTKEY;
        ListView_SetItemWide(hWndList, &lviW);

        if (pliElement=GetPluginListItem(&hPluginListStack, nSelItem + 1))
        {
          SendMessage(hWndHotkey, HKM_SETHOTKEY, wHotkey, 0);
          pliElement->pf->wHotkey=wHotkey;
          bListChanged=TRUE;
        }
      }
      else
      {
        LoadStringWide(hLangLib, MSG_HOTKEY_EXISTS, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, pfElement->wszFunction);
        API_MessageBox(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
        SetFocus(hWndHotkey);
      }
    }
    else if (LOWORD(wParam) == IDC_PLUGINS_CALL)
    {
      PLUGINLISTITEM *pliElement;
      PLUGINCALLPOSTW *pcp=NULL;

      if (nSelItem >= 0)
      {
        if (pliElement=GetPluginListItem(&hPluginListStack, nSelItem + 1))
        {
          if (pcp=(PLUGINCALLPOSTW *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW)))
          {
            xstrcpynW(pcp->szFunction, pliElement->pf->wszFunction, MAX_PATH);
            pcp->lParam=0;
          }
        }

        FreePluginList(&hPluginListStack);
        StackPluginCleanUp(&hPluginsStack, bListChanged?TRUE:FALSE);
        if (bListChanged) StackPluginSave(&hPluginsStack, moCur.nSaveSettings);

        EndDialog(hDlg, 0);
        if (pcp) PostMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)pcp);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      FreePluginList(&hPluginListStack);
      StackPluginCleanUp(&hPluginsStack, TRUE);
      StackPluginSave(&hPluginsStack, moCur.nSaveSettings);

      EndDialog(hDlg, 0);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      PLUGINLISTITEM *pliElement=(PLUGINLISTITEM *)hPluginListStack.first;

      while (pliElement)
      {
        pliElement->pf->wHotkey=pliElement->wInitialHotkey;
        pliElement->pf->bAutoLoad=pliElement->bInitialOnStart;

        pliElement=pliElement->next;
      }
      FreePluginList(&hPluginListStack);
      StackPluginCleanUp(&hPluginsStack, FALSE);

      EndDialog(hDlg, 0);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_PLUGINS_LIST)
    {
      if (((NMLISTVIEW *)lParam)->hdr.code == LVN_ITEMCHANGING)
      {
        PLUGINLISTITEM *pliElement;
        BOOL bNewState;
        BOOL bOldState;

        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK)
        {
          bNewState=((((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          bOldState=((((NMLISTVIEW *)lParam)->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            if (pliElement=GetPluginListItem(&hPluginListStack, ((NMLISTVIEW *)lParam)->iItem + 1))
            {
              if (pliElement->pf)
              {
                pliElement->pf->bAutoLoad=bNewState;
                bListChanged=TRUE;

                if (bNewState)
                {
                  if (pliElement->nCallResult == UD_FAILED || pliElement->nAutoLoad == -1)
                  {
                    //Check plugin autoload support
                    PLUGINCALLSENDW pcs;

                    pcs.pFunction=pliElement->pf->wszFunction;
                    pcs.lParam=0;
                    pcs.dwSupport=0;
                    if ((pliElement->nCallResult=CallPluginSend(NULL, &pcs, DLLCF_GETSUPPORT)) != UD_FAILED)
                    {
                      if (pcs.dwSupport & PDS_NOAUTOLOAD)
                        pliElement->nAutoLoad=0;
                      else
                        pliElement->nAutoLoad=1;
                    }
                  }
                  if (pliElement->nCallResult == UD_FAILED || pliElement->nAutoLoad == 0)
                  {
                    if (pliElement->nAutoLoad == 0)
                    {
                      LoadStringWide(hLangLib, MSG_AUTOLOAD_IS_NOT_SUPPORTED, wbuf, BUFFER_SIZE);
                      xprintfW(wbuf2, wbuf, pliElement->pf->wszFunction);
                      API_MessageBox(hDlg, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
                    }
                    pliElement->pf->bAutoLoad=FALSE;
                    SetWindowLongWide(hDlg, DWL_MSGRESULT, TRUE);
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

          if (pliElement=GetPluginListItem(&hPluginListStack, nSelItem + 1))
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
  DialogResizeMessages(&drs[0], &rcPluginsInitDialog, &moCur.rcPluginsCurrentDialog, DRM_GETMINMAXINFO|DRM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    MSG *msg=(MSG *)lParam;

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
  return CallNextHookEx(hHookKeys, code, wParam, lParam);
}

void FillPluginList(HWND hWnd)
{
  WIN32_FIND_DATAW wfdW;
  HANDLE hFind;
  HMODULE hInstance;
  PLUGINLISTDATA pld={0};
  wchar_t wszBaseName[MAX_PATH];
  wchar_t *wpPluginExt=L"dll";

  pld.hWndList=hWnd;
  xprintfW(wbuf, L"%s\\AkelFiles\\Plugs\\*.%s", wszExeDir, wpPluginExt);

  if ((hFind=FindFirstFileWide(wbuf, &wfdW)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      //Avoid FindFirstFile/FindNextFile bug: "*.dll_ANYSYMBOLS" is also matched
      if (xstrcmpiW(wpPluginExt, GetFileExt(wfdW.cFileName)))
        continue;

      xprintfW(wbuf, L"%s\\AkelFiles\\Plugs\\%s", wszExeDir, wfdW.cFileName);

      if (hInstance=LoadLibraryWide(wbuf))
      {
        if (GetProcAddress(hInstance, "DllAkelPadID"))
        {
          GetBaseName(wfdW.cFileName, wszBaseName, MAX_PATH);
          pld.pBaseName=(unsigned char *)wszBaseName;
          GetExportNames(hInstance, FillPluginListProc, (LPARAM)&pld);
        }
        FreeLibrary(hInstance);
      }
    }
    while (FindNextFileWide(hFind, &wfdW));

    FindClose(hFind);
  }
}

BOOL CALLBACK FillPluginListProc(char *pExportName, LPARAM lParam)
{
  wchar_t wszExportName[MAX_PATH];
  wchar_t wszPluginFunction[MAX_PATH];
  PLUGINLISTDATA *pld=(PLUGINLISTDATA *)lParam;
  PLUGINLISTITEM *pliElement;
  PLUGINFUNCTION *pfElement;
  LVITEMW lviW;
  int nIndex;
  int nPluginFunctionLen;

  if (xmemcmp(pExportName, "Dll", 3))
  {
    MultiByteToWideChar(CP_ACP, 0, pExportName, -1, wszExportName, MAX_PATH);
    nPluginFunctionLen=xprintfW(wszPluginFunction, L"%s::%s", (wchar_t *)pld->pBaseName, wszExportName);
    lviW.mask=LVIF_TEXT;
    lviW.pszText=wszPluginFunction;
    lviW.iItem=0;
    lviW.iSubItem=LVSI_FUNCTION_NAME;
    nIndex=ListView_InsertItemWide(pld->hWndList, &lviW);

    if (!StackInsertIndex((stack **)&hPluginListStack.first, (stack **)&hPluginListStack.last, (stack **)&pliElement, nIndex + 1, sizeof(PLUGINLISTITEM)))
    {
      if (pfElement=StackPluginFind(&hPluginsStack, wszPluginFunction, nPluginFunctionLen))
      {
        if (pfElement->bAutoLoad)
        {
          lviW.mask=LVIF_STATE;
          lviW.iItem=nIndex;
          lviW.iSubItem=LVSI_FUNCTION_NAME;
          lviW.state=((pfElement->bAutoLoad + 1) << 12);
          lviW.stateMask=LVIS_STATEIMAGEMASK;
          ListView_SetItemWide(pld->hWndList, &lviW);
        }
        if (pfElement->wHotkey)
        {
          GetHotkeyString(pfElement->wHotkey, wbuf);
          lviW.mask=LVIF_TEXT;
          lviW.pszText=wbuf;
          lviW.iItem=nIndex;
          lviW.iSubItem=LVSI_FUNCTION_HOTKEY;
          ListView_SetItemWide(pld->hWndList, &lviW);
        }
        if (pfElement->bRunning)
        {
          LoadStringWide(hLangLib, STR_RUNNING, wbuf, BUFFER_SIZE);
          lviW.mask=LVIF_TEXT;
          lviW.pszText=wbuf;
          lviW.iItem=nIndex;
          lviW.iSubItem=LVSI_FUNCTION_STATUS;
          ListView_SetItemWide(pld->hWndList, &lviW);
        }
      }
      else pfElement=StackPluginAdd(&hPluginsStack, wszPluginFunction, nPluginFunctionLen, 0, FALSE, NULL, NULL);

      pliElement->pf=pfElement;
      pliElement->wInitialHotkey=pfElement->wHotkey;
      pliElement->bInitialOnStart=pfElement->bAutoLoad;
      pliElement->nAutoLoad=-1;
      pliElement->nCallResult=UD_FAILED;
    }
  }
  return TRUE;
}

PLUGINLISTITEM* GetPluginListItem(HSTACK *hStack, int nIndex)
{
  PLUGINLISTITEM *pliElement;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&pliElement, nIndex))
    return pliElement;
  return NULL;
}

void FreePluginList(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
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

int GetHotkeyString(WORD wHotkey, wchar_t *wszString)
{
  wchar_t wszKeyText[100];
  LONG lScan;
  int nResult=0;

  wszString[0]='\0';

  if (wHotkey)
  {
    if (bOldWindows)
      lScan=MapVirtualKeyA(LOBYTE(wHotkey), 0) << 16;
    else
      lScan=MapVirtualKeyW(LOBYTE(wHotkey), 0) << 16;
    if (HIBYTE(wHotkey) & HOTKEYF_EXT)
      lScan|=0x01000000L;
    GetKeyNameTextWide(lScan, wszKeyText, 100);

    nResult=xprintfW(wszString, L"%s%s%s%s",
                               (HIBYTE(wHotkey) & HOTKEYF_CONTROL)?L"Ctrl + ":L"",
                               (HIBYTE(wHotkey) & HOTKEYF_SHIFT)?L"Shift + ":L"",
                               (HIBYTE(wHotkey) & HOTKEYF_ALT)?L"Alt + ":L"",
                               wszKeyText);
  }
  return nResult;
}


//// Options

LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam)
{
  //Center PropertySheet
  if (iCode == HCBT_ACTIVATE)
  {
    RECT rcEdit;
    RECT rcSheet;

    if (hPropertyHook)
    {
      if (UnhookWindowsHookEx(hPropertyHook))
        hPropertyHook=NULL;
    }
    GetWindowRect(hMainWnd, &rcEdit);
    GetWindowRect((HWND)wParam, &rcSheet);
    rcSheet.left=rcEdit.left + ((rcEdit.right - rcEdit.left) / 2) - ((rcSheet.right - rcSheet.left) / 2);
    rcSheet.top=rcEdit.top + ((rcEdit.bottom - rcEdit.top) / 2) - ((rcSheet.bottom - rcSheet.top) / 2);
    if (rcSheet.left < 0) rcSheet.left=0;
    if (rcSheet.top < 0) rcSheet.top=0;

    SetWindowPos((HWND)wParam, NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
    return 0;
  }
  else return CallNextHookEx(hPropertyHook, iCode, wParam, lParam);
}

int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam)
{
  //Remove "?"
  if (uMsg == PSCB_PRECREATE)
  {
    ((DLGTEMPLATE *)lParam)->style&=~DS_CONTEXTHELP;
  }
  else if (uMsg == PSCB_INITIALIZED)
  {
    hPropertyTab=(HWND)SendMessage(hDlg, PSM_GETTABCONTROL, 0, 0);
  }
  return TRUE;
}

BOOL CALLBACK OptionsGeneralDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

    SetWindowTextWide(hWndCommand, moCur.wszCommand);
    SetWindowTextWide(hWndDirectory, moCur.wszWorkDir);

    LoadStringWide(hLangLib, STR_NONE, wbuf, BUFFER_SIZE);
    ComboBox_AddStringWide(hWndAutodetectCP, wbuf);
    LoadStringWide(hLangLib, STR_AUTODETECT_RUSSIAN, wbuf, BUFFER_SIZE);
    ComboBox_AddStringWide(hWndAutodetectCP, wbuf);
    LoadStringWide(hLangLib, STR_AUTODETECT_ENGLISH, wbuf, BUFFER_SIZE);
    ComboBox_AddStringWide(hWndAutodetectCP, wbuf);
    LoadStringWide(hLangLib, STR_AUTODETECT_TURKISH, wbuf, BUFFER_SIZE);
    ComboBox_AddStringWide(hWndAutodetectCP, wbuf);
    LoadStringWide(hLangLib, STR_AUTODETECT_CHINESE, wbuf, BUFFER_SIZE);
    ComboBox_AddStringWide(hWndAutodetectCP, wbuf);
    LoadStringWide(hLangLib, STR_AUTODETECT_JAPANESE, wbuf, BUFFER_SIZE);
    ComboBox_AddStringWide(hWndAutodetectCP, wbuf);
    LoadStringWide(hLangLib, STR_AUTODETECT_KOREAN, wbuf, BUFFER_SIZE);
    ComboBox_AddStringWide(hWndAutodetectCP, wbuf);

    if (PRIMARYLANGID(moCur.dwLangCodepageRecognition) == LANG_RUSSIAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 1, 0);
    else if (PRIMARYLANGID(moCur.dwLangCodepageRecognition) == LANG_ENGLISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 2, 0);
    else if (PRIMARYLANGID(moCur.dwLangCodepageRecognition) == LANG_TURKISH)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 3, 0);
    else if (PRIMARYLANGID(moCur.dwLangCodepageRecognition) == LANG_CHINESE)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 4, 0);
    else if (PRIMARYLANGID(moCur.dwLangCodepageRecognition) == LANG_JAPANESE)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 5, 0);
    else if (PRIMARYLANGID(moCur.dwLangCodepageRecognition) == LANG_KOREAN)
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 6, 0);
    else
      SendMessage(hWndAutodetectCP, CB_SETCURSEL, 0, 0);
    SetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, moCur.dwCodepageRecognitionBuffer, FALSE);

    FillComboboxCodepage(hWndDefaultCP, lpCodepageList);
    SelectComboboxCodepage(hWndDefaultCP, moCur.nDefaultCodePage);

    if (moCur.nDefaultNewLine == NEWLINE_WIN)
      SendMessage(hWndDefaultNewLineWin, BM_SETCHECK, BST_CHECKED, 0);
    else if (moCur.nDefaultNewLine == NEWLINE_UNIX)
      SendMessage(hWndDefaultNewLineUnix, BM_SETCHECK, BST_CHECKED, 0);
    else if (moCur.nDefaultNewLine == NEWLINE_MAC)
      SendMessage(hWndDefaultNewLineMac, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_EXECCOM_BROWSE)
    {
      wchar_t wszExeFilter[MAX_PATH];
      BOOL bResult;

      //GetOpenFileName dialog file filter
      LoadStringWide(hLangLib, STR_EXECUTABLE_FILTER, wszExeFilter, MAX_PATH);
      for (i=0; wszExeFilter[i]; ++i)
        if (wszExeFilter[i] == '|') wszExeFilter[i]='\0';
      wszExeFilter[++i]='\0';

      //File browse
      {
        OPENFILENAMEW efnW={0};

        wbuf[0]='\0';
        efnW.lStructSize      =sizeof(OPENFILENAMEW);
        efnW.hwndOwner        =hDlg;
        efnW.lpstrFile        =wbuf;
        efnW.nMaxFile         =MAX_PATH;
        efnW.lpstrFilter      =wszExeFilter;
        efnW.nFilterIndex     =1;
        efnW.Flags            =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

        bResult=GetOpenFileNameWide(&efnW);
      }

      if (bResult)
      {
        xprintfW(wbuf2, L"\"%s\"", wbuf);
        SetWindowTextWide(hWndCommand, wbuf2);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_EXECDIR_BROWSE)
    {
      BROWSEINFOW biW={0};
      LPITEMIDLIST pIdList;
      LPMALLOC pMalloc;

      GetWindowTextWide(hWndDirectory, wbuf, BUFFER_SIZE);
      biW.hwndOwner=hDlg;
      biW.pszDisplayName=wbuf;
      biW.ulFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
      biW.lpfn=BrowseCallbackProc;
      biW.lParam=(LPARAM)wbuf;

      if (pIdList=SHBrowseForFolderWide(&biW))
      {
        SHGetPathFromIDListWide(pIdList, wbuf);

        if (SHGetMalloc(&pMalloc))
        {
          pMalloc->Free(pIdList);
          pMalloc->Release();
        }
        SetWindowTextWide(hWndDirectory, wbuf);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGE_FILTER)
    {
      if (bOldWindows)
        API_DialogBoxParamA(hLangLib, MAKEINTRESOURCEA(IDD_OPTIONS_GENERAL_FILTER), hDlg, (DLGPROC)OptionsGeneralFilterDlgProc, (LPARAM)hWndDefaultCP);
      else
        API_DialogBoxParamW(hLangLib, MAKEINTRESOURCEW(IDD_OPTIONS_GENERAL_FILTER), hDlg, (DLGPROC)OptionsGeneralFilterDlgProc, (LPARAM)hWndDefaultCP);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_SETACTIVE)
    {
      nPropertyStartPage=SendMessage(hPropertyTab, TCM_GETCURSEL, 0, 0);
    }
    else if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      //Execute
      GetDlgItemTextWide(hDlg, IDC_OPTIONS_EXECCOM, moCur.wszCommand, BUFFER_SIZE);
      GetDlgItemTextWide(hDlg, IDC_OPTIONS_EXECDIR, moCur.wszWorkDir, MAX_PATH);

      //Autodetect codepage
      i=SendMessage(hWndAutodetectCP, CB_GETCURSEL, 0, 0);

      if (i == 0)
        moCur.dwLangCodepageRecognition=0;
      else if (i == 1)
        moCur.dwLangCodepageRecognition=LANGID_RUSSIAN;
      else if (i == 2)
        moCur.dwLangCodepageRecognition=LANGID_ENGLISH;
      else if (i == 3)
        moCur.dwLangCodepageRecognition=LANGID_TURKISH;
      else if (i == 4)
        moCur.dwLangCodepageRecognition=LANGID_CHINESE;
      else if (i == 5)
        moCur.dwLangCodepageRecognition=LANGID_JAPANESE;
      else if (i == 6)
        moCur.dwLangCodepageRecognition=LANGID_KOREAN;

      //Autodetect codepage buffer
      moCur.dwCodepageRecognitionBuffer=GetDlgItemInt(hDlg, IDC_OPTIONS_CODEPAGE_RECOGNITION_BUFFER, NULL, FALSE);

      //Default codepage
      moCur.nDefaultCodePage=GetComboboxCodepage(hWndDefaultCP);

      //Default newline
      if (SendMessage(hWndDefaultNewLineWin, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.nDefaultNewLine=NEWLINE_WIN;
      else if (SendMessage(hWndDefaultNewLineUnix, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.nDefaultNewLine=NEWLINE_UNIX;
      else if (SendMessage(hWndDefaultNewLineMac, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.nDefaultNewLine=NEWLINE_MAC;
    }
  }
  return FALSE;
}

int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
  {
    if (bOldWindows)
    {
      WideCharToMultiByte(CP_ACP, 0, (wchar_t *)lpData, -1, buf, MAX_PATH, NULL, NULL);
      SendMessage(hWnd, BFFM_SETSELECTIONA, TRUE, (LPARAM)buf);
    }
    else SendMessage(hWnd, BFFM_SETSELECTIONW, TRUE, lpData);
  }
  return 0;
}

BOOL CALLBACK OptionsGeneralFilterDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

    FillListBoxCodepage(hWndCustomList, lpCodepageList);
    EnumCodepageList(&lpFullCodepageList);
    FillListBoxCodepage(hWndSystemList, lpFullCodepageList);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_UP)
    {
      if ((nSelection=SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) > 0)
      {
        ListBox_GetTextWide(hWndCustomList, nSelection, wbuf);
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);
        ListBox_InsertStringWide(hWndCustomList, nSelection - 1, wbuf);
        SendMessage(hWndCustomList, LB_SETCURSEL, nSelection - 1, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DOWN)
    {
      if ((nSelection=(int)SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0)) != LB_ERR &&
          (nCount=(int)SendMessage(hWndCustomList, LB_GETCOUNT, 0, 0)) != LB_ERR &&
           nSelection != nCount - 1)
      {
        ListBox_GetTextWide(hWndCustomList, nSelection, wbuf);
        SendMessage(hWndCustomList, LB_DELETESTRING, nSelection, 0);
        ListBox_InsertStringWide(hWndCustomList, nSelection + 1, wbuf);
        SendMessage(hWndCustomList, LB_SETCURSEL, nSelection + 1, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_ADD)
    {
      if ((nSelection=(int)SendMessage(hWndSystemList, LB_GETCURSEL, 0, 0)) != LB_ERR)
      {
        ListBox_GetTextWide(hWndSystemList, nSelection, wbuf);

        if (ListBox_FindStringWide(hWndCustomList, -1, wbuf) == LB_ERR)
        {
          nSelection=SendMessage(hWndCustomList, LB_GETCURSEL, 0, 0);
          ListBox_InsertStringWide(hWndCustomList, nSelection, wbuf);
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
      ClearListBox(hWndCustomList);
      FillListBoxCodepage(hWndCustomList, lpFullCodepageList);
      EnableWindow(hWndOK, TRUE);
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_CODEPAGES_FILTER_DELETEALL)
    {
      ClearListBox(hWndCustomList);
      EnableWindow(hWndOK, FALSE);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      bMenuPopupCodepage=TRUE;
      CodepageListFree(&lpCodepageList);
      CodepageListFree(&lpFullCodepageList);
      GetListBoxCodepageList(hWndCustomList, &lpCodepageList);
      nCodepageListLen=CodepageListLen(lpCodepageList);
      ClearCombobox(hWndDefaultCP);
      FillComboboxCodepage(hWndDefaultCP, lpCodepageList);
      if (SelectComboboxCodepage(hWndDefaultCP, moCur.nDefaultCodePage) == CB_ERR)
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

BOOL CALLBACK OptionsRegistryDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    SendMessage(hWndRecentFilesSpin, UDM_SETRANGE, 0, MAKELONG(999, 0));
    SendMessage(hWndSearchStringsSpin, UDM_SETBUDDY, (WPARAM)hWndSearchStrings, 0);
    SendMessage(hWndSearchStringsSpin, UDM_SETRANGE, 0, MAKELONG(999, 0));

    if (moCur.dwFileTypesAssociated & AE_OPEN) SendMessage(hWndAssociateOpen, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.dwFileTypesAssociated & AE_EDIT) SendMessage(hWndAssociateEdit, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.dwFileTypesAssociated & AE_PRINT) SendMessage(hWndAssociatePrint, BM_SETCHECK, BST_CHECKED, 0);
    if (!(moCur.dwFileTypesAssociated & AE_OPEN)) EnableWindow(hWndFileTypesOpen, FALSE);
    if (!(moCur.dwFileTypesAssociated & AE_EDIT)) EnableWindow(hWndFileTypesEdit, FALSE);
    if (!(moCur.dwFileTypesAssociated & AE_PRINT)) EnableWindow(hWndFileTypesPrint, FALSE);
    SendMessage(hWndFileTypesOpen, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesEdit, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SendMessage(hWndFileTypesPrint, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);

    SetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, moCur.nRecentFiles, FALSE);
    if (!moCur.nRecentFiles || !*lpwszRecentNames[0]) EnableWindow(hWndRecentFilesClear, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, moCur.nSearchStrings, FALSE);
    if (!moCur.nSearchStrings) EnableWindow(hWndSearchStringsClear, FALSE);

    SetWindowTextWide(hWndFileTypesOpen, moCur.wszFileTypesOpen);
    SetWindowTextWide(hWndFileTypesEdit, moCur.wszFileTypesEdit);
    SetWindowTextWide(hWndFileTypesPrint, moCur.wszFileTypesPrint);

    if (moCur.nSaveSettings == SS_REGISTRY)
      SendMessage(hWndSaveRegistry, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndSaveINI, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.bSavePositions)
      SendMessage(hWndSavePositions, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.bSaveCodepages)
      SendMessage(hWndSaveCodepages, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OPTIONS_RECENTFILES_CLEAR)
    {
      RecentFilesZero();
      RecentFilesSave();
      bMenuRecentFiles=TRUE;
      EnableWindow((HWND)lParam, FALSE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_SEARCHSTRINGS_CLEAR)
    {
      wchar_t wszRegKey[MAX_PATH];

      xprintfW(wszRegKey, L"%s\\Search", APP_REGHOMEW);
      RegClearKeyWide(HKEY_CURRENT_USER, wszRegKey);
      EnableWindow((HWND)lParam, FALSE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_OPEN)
    {
      bState=SendMessage(hWndAssociateOpen, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesOpen, bState);
      if (!bState) SetWindowTextWide(hWndFileTypesOpen, moCur.wszFileTypesOpen);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_EDIT)
    {
      bState=SendMessage(hWndAssociateEdit, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesEdit, bState);
      if (!bState) SetWindowTextWide(hWndFileTypesEdit, moCur.wszFileTypesEdit);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_OPTIONS_ASSOCIATE_PRINT)
    {
      bState=SendMessage(hWndAssociatePrint, BM_GETCHECK, 0, 0);
      EnableWindow(hWndFileTypesPrint, bState);
      if (!bState) SetWindowTextWide(hWndFileTypesPrint, moCur.wszFileTypesPrint);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_SETACTIVE)
    {
      nPropertyStartPage=SendMessage(hPropertyTab, TCM_GETCURSEL, 0, 0);
    }
    else if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      wchar_t wszWindowText[MAX_PATH];
      BOOL bRecentFilesRefresh=FALSE;
      BOOL bShellRefresh=FALSE;
      int a;
      int b;

      //Save settings
      if (SendMessage(hWndSaveRegistry, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=SS_REGISTRY;
      else
        a=SS_INI;
      if (moCur.nSaveSettings != a)
      {
        moCur.nSaveSettings=a;
        bOptionsSave=TRUE;
      }

      //Recent files
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_RECENTFILES_AMOUNT, NULL, FALSE);
      if (moCur.nRecentFiles != a)
      {
        moCur.nRecentFiles=a;
        bRecentFilesRefresh=TRUE;
      }
      a=SendMessage(hWndSavePositions, BM_GETCHECK, 0, 0);
      b=SendMessage(hWndSaveCodepages, BM_GETCHECK, 0, 0);
      if (a != moCur.bSavePositions || b != moCur.bSaveCodepages)
      {
        moCur.bSavePositions=a;
        moCur.bSaveCodepages=b;
        bRecentFilesRefresh=TRUE;
      }

      if (bRecentFilesRefresh)
      {
        FreeMemoryRecentFiles();

        if (moCur.nRecentFiles)
        {
          if (RecentFilesAlloc())
          {
            RecentFilesZero();
            RecentFilesRead();
          }
        }
        bMenuRecentFiles=TRUE;

        //Clean save
        {
          wchar_t wszRegKey[MAX_PATH];

          xprintfW(wszRegKey, L"%s\\Recent", APP_REGHOMEW);
          RegClearKeyWide(HKEY_CURRENT_USER, wszRegKey);
          RecentFilesSave();
        }
      }

      //Search history
      moCur.nSearchStrings=GetDlgItemInt(hDlg, IDC_OPTIONS_SEARCHSTRINGS_AMOUNT, NULL, FALSE);

      //Associations
      bState=SendMessage(hWndAssociateOpen, BM_GETCHECK, 0, 0);
      if (bState && !(moCur.dwFileTypesAssociated & AE_OPEN))
      {
        GetWindowTextWide(hWndFileTypesOpen, moCur.wszFileTypesOpen, MAX_PATH);
        AssociateFileTypesW(hInstance, moCur.wszFileTypesOpen, AE_OPEN|AE_ASSOCIATE);
        moCur.dwFileTypesAssociated|=AE_OPEN;
        bShellRefresh=TRUE;
      }
      else if (bState && (moCur.dwFileTypesAssociated & AE_OPEN))
      {
        GetWindowTextWide(hWndFileTypesOpen, wszWindowText, MAX_PATH);
        if (xstrcmpiW(wszWindowText, moCur.wszFileTypesOpen))
        {
          AssociateFileTypesW(hInstance, moCur.wszFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
          AssociateFileTypesW(hInstance, wszWindowText, AE_OPEN|AE_ASSOCIATE);
          xstrcpynW(moCur.wszFileTypesOpen, wszWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (moCur.dwFileTypesAssociated & AE_OPEN))
      {
        AssociateFileTypesW(hInstance, moCur.wszFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
        moCur.dwFileTypesAssociated&=~AE_OPEN;
        bShellRefresh=TRUE;
      }

      bState=SendMessage(hWndAssociateEdit, BM_GETCHECK, 0, 0);
      if (bState && !(moCur.dwFileTypesAssociated & AE_EDIT))
      {
        GetWindowTextWide(hWndFileTypesEdit, moCur.wszFileTypesEdit, MAX_PATH);
        AssociateFileTypesW(hInstance, moCur.wszFileTypesEdit, AE_EDIT|AE_ASSOCIATE);
        moCur.dwFileTypesAssociated|=AE_EDIT;
        bShellRefresh=TRUE;
      }
      else if (bState && (moCur.dwFileTypesAssociated & AE_EDIT))
      {
        GetWindowTextWide(hWndFileTypesEdit, wszWindowText, MAX_PATH);
        if (xstrcmpiW(wszWindowText, moCur.wszFileTypesEdit))
        {
          AssociateFileTypesW(hInstance, moCur.wszFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
          AssociateFileTypesW(hInstance, wszWindowText, AE_EDIT|AE_ASSOCIATE);
          xstrcpynW(moCur.wszFileTypesEdit, wszWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (moCur.dwFileTypesAssociated & AE_EDIT))
      {
        AssociateFileTypesW(hInstance, moCur.wszFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
        moCur.dwFileTypesAssociated&=~AE_EDIT;
        bShellRefresh=TRUE;
      }

      bState=SendMessage(hWndAssociatePrint, BM_GETCHECK, 0, 0);
      if (bState && !(moCur.dwFileTypesAssociated & AE_PRINT))
      {
        GetWindowTextWide(hWndFileTypesPrint, moCur.wszFileTypesPrint, MAX_PATH);
        AssociateFileTypesW(hInstance, moCur.wszFileTypesPrint, AE_PRINT|AE_ASSOCIATE);
        moCur.dwFileTypesAssociated|=AE_PRINT;
        bShellRefresh=TRUE;
      }
      else if (bState && (moCur.dwFileTypesAssociated & AE_PRINT))
      {
        GetWindowTextWide(hWndFileTypesPrint, wszWindowText, MAX_PATH);
        if (xstrcmpiW(wszWindowText, moCur.wszFileTypesPrint))
        {
          AssociateFileTypesW(hInstance, moCur.wszFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
          AssociateFileTypesW(hInstance, wszWindowText, AE_PRINT|AE_ASSOCIATE);
          xstrcpynW(moCur.wszFileTypesPrint, wszWindowText, MAX_PATH);
        }
        bShellRefresh=TRUE;
      }
      else if (!bState && (moCur.dwFileTypesAssociated & AE_PRINT))
      {
        AssociateFileTypesW(hInstance, moCur.wszFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
        moCur.dwFileTypesAssociated&=~AE_PRINT;
        bShellRefresh=TRUE;
      }
      if (bShellRefresh) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsEditor1DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndMarginLeft;
  static HWND hWndMarginLeftSpin;
  static HWND hWndMarginSelection;
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
  static HWND hWndRichEditMouse;
  static HWND hWndMouseDragging;
  static HWND hWndLineGap;
  static HWND hWndLineGapSpin;

  if (uMsg == WM_INITDIALOG)
  {
    hWndMarginLeft=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT);
    hWndMarginLeftSpin=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT_SPIN);
    hWndMarginSelection=GetDlgItem(hDlg, IDC_OPTIONS_EDITMARGIN_SELECTION);
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
    hWndRichEditMouse=GetDlgItem(hDlg, IDC_OPTIONS_RICHEDITMOUSE);
    hWndMouseDragging=GetDlgItem(hDlg, IDC_OPTIONS_MOUSEDRAGGING);
    hWndLineGap=GetDlgItem(hDlg, IDC_OPTIONS_LINEGAP);
    hWndLineGapSpin=GetDlgItem(hDlg, IDC_OPTIONS_LINEGAP_SPIN);

    SendMessage(hWndMarginLeftSpin, UDM_SETBUDDY, (WPARAM)hWndMarginLeft, 0);
    SendMessage(hWndMarginLeftSpin, UDM_SETRANGE, 0, MAKELONG(999, 0));
    SendMessage(hWndMarginRightSpin, UDM_SETBUDDY, (WPARAM)hWndMarginRight, 0);
    SendMessage(hWndMarginRightSpin, UDM_SETRANGE, 0, MAKELONG(999, 0));
    SendMessage(hWndTabSizeSpin, UDM_SETBUDDY, (WPARAM)hWndTabSize, 0);
    SendMessage(hWndTabSizeSpin, UDM_SETRANGE, 0, MAKELONG(100, 1));
    SendMessage(hWndUndoLimitSpin, UDM_SETBUDDY, (WPARAM)hWndUndoLimit, 0);
    SendMessage(hWndUndoLimitSpin, UDM_SETRANGE, 0, MAKELONG(9999, 0));
    SendMessage(hWndWrapLimitSpin, UDM_SETBUDDY, (WPARAM)hWndWrapLimit, 0);
    SendMessage(hWndWrapLimitSpin, UDM_SETRANGE32, (WPARAM)-1, 9999);
    SendMessage(hWndMarkerSpin, UDM_SETBUDDY, (WPARAM)hWndMarker, 0);
    SendMessage(hWndMarkerSpin, UDM_SETRANGE32, (WPARAM)-1, 9999);
    SendMessage(hWndCaretWidthSpin, UDM_SETBUDDY, (WPARAM)hWndCaretWidth, 0);
    SendMessage(hWndCaretWidthSpin, UDM_SETRANGE, 0, MAKELONG(100, 1));
    SendMessage(hWndLineGapSpin, UDM_SETBUDDY, (WPARAM)hWndLineGap, 0);
    SendMessage(hWndLineGapSpin, UDM_SETRANGE, 0, MAKELONG(100, 0));

    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, LOWORD(lpFrameCurrent->dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, HIWORD(lpFrameCurrent->dwEditMargins), FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, lpFrameCurrent->nTabStopSize, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, lpFrameCurrent->nUndoLimit, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_WRAP_LIMIT, lpFrameCurrent->dwWrapLimit, TRUE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_MARKER, lpFrameCurrent->dwMarker, TRUE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_CARETWIDTH, lpFrameCurrent->nCaretWidth, FALSE);
    SetDlgItemInt(hDlg, IDC_OPTIONS_LINEGAP, lpFrameCurrent->dwLineGap, FALSE);

    if (lpFrameCurrent->bTabStopAsSpaces)
      SendMessage(hWndTabSizeSpaces, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->bDetailedUndo)
      SendMessage(hWndDetailedUndo, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->dwWrapType & AEWW_WORD)
      SendMessage(hWndWrapByWords, BM_SETCHECK, BST_CHECKED, 0);
    else if (lpFrameCurrent->dwWrapType & AEWW_SYMBOL)
      SendMessage(hWndWrapBySymbols, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->dwCaretOptions & CO_CARETOUTEDGE)
      SendMessage(hWndCaretOutEdge, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->dwCaretOptions & CO_CARETVERTLINE)
      SendMessage(hWndCaretVertLine, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->dwMouseOptions & MO_LEFTMARGINSELECTION)
      SendMessage(hWndMarginSelection, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->dwMouseOptions & MO_RICHEDITMOUSE)
      SendMessage(hWndRichEditMouse, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->dwMouseOptions & MO_MOUSEDRAGGING)
      SendMessage(hWndMouseDragging, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_SETACTIVE)
    {
      nPropertyStartPage=SendMessage(hPropertyTab, TCM_GETCURSEL, 0, 0);
    }
    else if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;
      int b;

      //Margins
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_LEFT, NULL, FALSE);
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_EDITMARGIN_RIGHT, NULL, FALSE);
      if (lpFrameCurrent->dwEditMargins != (DWORD)MAKELONG(a, b))
      {
        if (lpFrameCurrent->ei.hWndMaster)
        {
          SetMargins(lpFrameCurrent->ei.hWndMaster, MAKELONG(a, b), lpFrameCurrent->dwEditMargins);
          if (lpFrameCurrent->ei.hWndClone1)
            SetMargins(lpFrameCurrent->ei.hWndClone1, MAKELONG(a, b), lpFrameCurrent->dwEditMargins);
          if (lpFrameCurrent->ei.hWndClone2)
            SetMargins(lpFrameCurrent->ei.hWndClone2, MAKELONG(a, b), lpFrameCurrent->dwEditMargins);
          if (lpFrameCurrent->ei.hWndClone3)
            SetMargins(lpFrameCurrent->ei.hWndClone3, MAKELONG(a, b), lpFrameCurrent->dwEditMargins);
        }
        else SetMargins(lpFrameCurrent->ei.hWndEdit, MAKELONG(a, b), lpFrameCurrent->dwEditMargins);

        lpFrameCurrent->dwEditMargins=MAKELONG(a, b);
        InvalidateRect(lpFrameCurrent->ei.hWndEdit, NULL, TRUE);
      }

      //Tab stops
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_TABSIZE, NULL, FALSE);
      if (lpFrameCurrent->nTabStopSize != a)
      {
        lpFrameCurrent->nTabStopSize=a;
        SetTabStops(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->nTabStopSize, TRUE);
        UpdateStatusUser(lpFrameCurrent, CSB_TABSIZE);
      }
      lpFrameCurrent->bTabStopAsSpaces=SendMessage(hWndTabSizeSpaces, BM_GETCHECK, 0, 0);

      //Undo
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_UNDO_LIMIT, NULL, FALSE);
      if (lpFrameCurrent->nUndoLimit != a)
      {
        lpFrameCurrent->nUndoLimit=a;
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETUNDOLIMIT, (WPARAM)lpFrameCurrent->nUndoLimit, 0);
      }
      lpFrameCurrent->bDetailedUndo=SendMessage(hWndDetailedUndo, BM_GETCHECK, 0, 0);
      SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETOPTIONS, lpFrameCurrent->bDetailedUndo?AECOOP_OR:AECOOP_XOR, AECO_DETAILEDUNDO);

      //Wrap
      if (SendMessage(hWndWrapByWords, BM_GETCHECK, 0, 0) == BST_CHECKED)
        a=AEWW_WORD;
      else
        a=AEWW_SYMBOL;
      b=GetDlgItemInt(hDlg, IDC_OPTIONS_WRAP_LIMIT, NULL, TRUE);

      if ((int)lpFrameCurrent->dwWrapType != a || (int)lpFrameCurrent->dwWrapLimit != b)
      {
        if (lpFrameCurrent->ei.bWordWrap)
          UpdateShowHScroll(lpFrameCurrent);
        lpFrameCurrent->dwWrapType=a;
        lpFrameCurrent->dwWrapLimit=b;

        if (lpFrameCurrent->ei.bWordWrap)
        {
          UpdateShowHScroll(lpFrameCurrent);
          SetWordWrap(lpFrameCurrent, lpFrameCurrent->dwWrapType, lpFrameCurrent->dwWrapLimit);
        }
      }

      //Marker
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_MARKER, NULL, TRUE);
      if ((int)lpFrameCurrent->dwMarker != a)
      {
        lpFrameCurrent->dwMarker=a;
        SetMarker(lpFrameCurrent, lpFrameCurrent->dwMarker);
        UpdateStatusUser(lpFrameCurrent, CSB_MARKER);
      }

      //Caret options
      lpFrameCurrent->dwCaretOptions=0;
      if (SendMessage(hWndCaretOutEdge, BM_GETCHECK, 0, 0) == BST_CHECKED)
        lpFrameCurrent->dwCaretOptions|=CO_CARETOUTEDGE;
      SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETOPTIONS, (lpFrameCurrent->dwCaretOptions & CO_CARETOUTEDGE)?AECOOP_OR:AECOOP_XOR, AECO_CARETOUTEDGE);

      if (SendMessage(hWndCaretVertLine, BM_GETCHECK, 0, 0) == BST_CHECKED)
        lpFrameCurrent->dwCaretOptions|=CO_CARETVERTLINE;
      SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETOPTIONS, (lpFrameCurrent->dwCaretOptions & CO_CARETVERTLINE)?AECOOP_OR:AECOOP_XOR, AECO_ACTIVECOLUMN);

      //Caret width
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_CARETWIDTH, NULL, FALSE);
      if (lpFrameCurrent->nCaretWidth != a)
      {
        lpFrameCurrent->nCaretWidth=a;

        //Update width
        {
          POINT pt;

          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);
          pt.x=lpFrameCurrent->nCaretWidth;
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETCARETWIDTH, 0, (LPARAM)&pt);
        }
      }

      //Mouse options
      lpFrameCurrent->dwMouseOptions=0;
      if (SendMessage(hWndMarginSelection, BM_GETCHECK, 0, 0) == BST_CHECKED)
        lpFrameCurrent->dwMouseOptions|=MO_LEFTMARGINSELECTION;
      SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETOPTIONS, (lpFrameCurrent->dwMouseOptions & MO_LEFTMARGINSELECTION)?AECOOP_XOR:AECOOP_OR, AECO_NOMARGINSEL);

      if (SendMessage(hWndRichEditMouse, BM_GETCHECK, 0, 0) == BST_CHECKED)
        lpFrameCurrent->dwMouseOptions|=MO_RICHEDITMOUSE;
      SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETOPTIONS, (lpFrameCurrent->dwMouseOptions & MO_RICHEDITMOUSE)?AECOOP_OR:AECOOP_XOR, AECO_LBUTTONUPCONTINUECAPTURE);

      if (SendMessage(hWndMouseDragging, BM_GETCHECK, 0, 0) == BST_CHECKED)
        lpFrameCurrent->dwMouseOptions|=MO_MOUSEDRAGGING;
      SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETOPTIONS, !(lpFrameCurrent->dwMouseOptions & MO_MOUSEDRAGGING)?AECOOP_OR:AECOOP_XOR, AECO_DISABLEDRAG);

      //Line gap
      a=GetDlgItemInt(hDlg, IDC_OPTIONS_LINEGAP, NULL, FALSE);
      if ((int)lpFrameCurrent->dwLineGap != a)
      {
        lpFrameCurrent->dwLineGap=a;

        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETLINEGAP, lpFrameCurrent->dwLineGap, 0);
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsEditor2DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndShowURL;
  static HWND hWndSingleClickURL;
  static HWND hWndDoubleClickURL;
  static HWND hWndUrlPrefixesEnable;
  static HWND hWndUrlPrefixes;
  static HWND hWndUrlDelimitersEnable;
  static HWND hWndUrlLeftDelimiters;
  static HWND hWndUrlRightDelimiters;
  static HWND hWndWordDelimitersEnable;
  static HWND hWndWordDelimiters;
  static HWND hWndWrapDelimitersEnable;
  static HWND hWndWrapDelimiters;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndShowURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SHOW);
    hWndSingleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_SINGLECLICK);
    hWndDoubleClickURL=GetDlgItem(hDlg, IDC_OPTIONS_URL_DOUBLECLICK);
    hWndUrlPrefixesEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES_ENABLE);
    hWndUrlPrefixes=GetDlgItem(hDlg, IDC_OPTIONS_URL_PREFIXES);
    hWndUrlDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_URL_DELIMITERS_ENABLE);
    hWndUrlLeftDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_URL_LEFTDELIMITERS);
    hWndUrlRightDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_URL_RIGHTDELIMITERS);
    hWndWordDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS_ENABLE);
    hWndWordDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WORD_DELIMITERS);
    hWndWrapDelimitersEnable=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_DELIMITERS_ENABLE);
    hWndWrapDelimiters=GetDlgItem(hDlg, IDC_OPTIONS_WRAP_DELIMITERS);

    if (lpFrameCurrent->bShowURL)
      SendMessage(hWndShowURL, BM_SETCHECK, BST_CHECKED, 0);
    if (lpFrameCurrent->nClickURL == 1)
      SendMessage(hWndSingleClickURL, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndDoubleClickURL, BM_SETCHECK, BST_CHECKED, 0);

    if (lpFrameCurrent->bUrlPrefixesEnable)
      SendMessage(hWndUrlPrefixesEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndUrlPrefixes, lpFrameCurrent->bUrlPrefixesEnable);
    SetWindowTextWide(hWndUrlPrefixes, lpFrameCurrent->wszUrlPrefixes);
    SendMessage(hWndUrlPrefixes, EM_LIMITTEXT, (WPARAM)URL_PREFIXES_SIZE, 0);

    if (lpFrameCurrent->bUrlDelimitersEnable)
      SendMessage(hWndUrlDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndUrlLeftDelimiters, lpFrameCurrent->bUrlDelimitersEnable);
    EscapeDataToEscapeStringW(lpFrameCurrent->wszUrlLeftDelimiters, wbuf);
    SetWindowTextWide(hWndUrlLeftDelimiters, wbuf);
    SendMessage(hWndUrlLeftDelimiters, EM_LIMITTEXT, (WPARAM)URL_DELIMITERS_SIZE, 0);

    EnableWindow(hWndUrlRightDelimiters, lpFrameCurrent->bUrlDelimitersEnable);
    EscapeDataToEscapeStringW(lpFrameCurrent->wszUrlRightDelimiters, wbuf);
    SetWindowTextWide(hWndUrlRightDelimiters, wbuf);
    SendMessage(hWndUrlRightDelimiters, EM_LIMITTEXT, (WPARAM)URL_DELIMITERS_SIZE, 0);

    if (lpFrameCurrent->bWordDelimitersEnable)
      SendMessage(hWndWordDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWordDelimiters, lpFrameCurrent->bWordDelimitersEnable);
    EscapeDataToEscapeStringW(lpFrameCurrent->wszWordDelimiters, wbuf);
    SetWindowTextWide(hWndWordDelimiters, wbuf);
    SendMessage(hWndWordDelimiters, EM_LIMITTEXT, (WPARAM)WORD_DELIMITERS_SIZE, 0);

    if (lpFrameCurrent->bWrapDelimitersEnable)
      SendMessage(hWndWrapDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    EnableWindow(hWndWrapDelimiters, lpFrameCurrent->bWrapDelimitersEnable);
    EscapeDataToEscapeStringW(lpFrameCurrent->wszWrapDelimiters, wbuf);
    SetWindowTextWide(hWndWrapDelimiters, wbuf);
    SendMessage(hWndWrapDelimiters, EM_LIMITTEXT, (WPARAM)WRAP_DELIMITERS_SIZE, 0);

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
        EnableWindow(hWndUrlLeftDelimiters, FALSE);
        EnableWindow(hWndUrlRightDelimiters, FALSE);
      }
      else
      {
        bState=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
        EnableWindow(hWndUrlPrefixes, bState);
        bState=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
        EnableWindow(hWndUrlLeftDelimiters, bState);
        EnableWindow(hWndUrlRightDelimiters, bState);
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
      EnableWindow(hWndUrlLeftDelimiters, bState);
      EnableWindow(hWndUrlRightDelimiters, bState);
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
      EscapeDataToEscapeStringW(STR_WORD_DELIMITERSW, wbuf);
      SetWindowTextWide(hWndWordDelimiters, wbuf);
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
      EscapeDataToEscapeStringW(STR_WRAP_DELIMITERSW, wbuf);
      SetWindowTextWide(hWndWrapDelimiters, wbuf);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_SETACTIVE)
    {
      nPropertyStartPage=SendMessage(hPropertyTab, TCM_GETCURSEL, 0, 0);
    }
    else if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      int a;

      //HyperLinks
      a=SendMessage(hWndShowURL, BM_GETCHECK, 0, 0);
      if (a != lpFrameCurrent->bShowURL)
      {
        lpFrameCurrent->bShowURL=a;
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETDETECTURL, lpFrameCurrent->bShowURL, 0);
      }
      if (SendMessage(hWndSingleClickURL, BM_GETCHECK, 0, 0) == BST_CHECKED)
        lpFrameCurrent->nClickURL=1;
      else
        lpFrameCurrent->nClickURL=2;

      //Url prefixes
      GetWindowTextWide(hWndUrlPrefixes, lpFrameCurrent->wszUrlPrefixes, URL_PREFIXES_SIZE);

      lpFrameCurrent->bUrlPrefixesEnable=SendMessage(hWndUrlPrefixesEnable, BM_GETCHECK, 0, 0);
      if (lpFrameCurrent->bUrlPrefixesEnable)
        SetUrlPrefixes(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszUrlPrefixes);
      else
        SetUrlPrefixes(lpFrameCurrent->ei.hWndEdit, NULL);

      //Url delimiters
      GetWindowTextWide(hWndUrlLeftDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, lpFrameCurrent->wszUrlLeftDelimiters, NEWLINE_UNIX);

      GetWindowTextWide(hWndUrlRightDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, lpFrameCurrent->wszUrlRightDelimiters, NEWLINE_UNIX);

      lpFrameCurrent->bUrlDelimitersEnable=SendMessage(hWndUrlDelimitersEnable, BM_GETCHECK, 0, 0);
      if (lpFrameCurrent->bUrlDelimitersEnable)
      {
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETURLLEFTDELIMITERS, 0, (LPARAM)lpFrameCurrent->wszUrlLeftDelimiters);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETURLRIGHTDELIMITERS, 0, (LPARAM)lpFrameCurrent->wszUrlRightDelimiters);
      }
      else
      {
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETURLLEFTDELIMITERS, 0, (LPARAM)NULL);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETURLRIGHTDELIMITERS, 0, (LPARAM)NULL);
      }

      //Word delimiters
      GetWindowTextWide(hWndWordDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, lpFrameCurrent->wszWordDelimiters, NEWLINE_UNIX);

      lpFrameCurrent->bWordDelimitersEnable=SendMessage(hWndWordDelimitersEnable, BM_GETCHECK, 0, 0);
      if (lpFrameCurrent->bWordDelimitersEnable)
      {
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETWORDBREAK, moCur.dwWordBreakCustom, 0);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)lpFrameCurrent->wszWordDelimiters);
      }
      else
      {
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETWORDBREAK, dwWordBreakDefault, 0);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)NULL);
      }

      //Wrap delimiters
      GetWindowTextWide(hWndWrapDelimiters, wbuf, BUFFER_SIZE);
      EscapeStringToEscapeDataW(wbuf, lpFrameCurrent->wszWrapDelimiters, NEWLINE_UNIX);

      lpFrameCurrent->bWrapDelimitersEnable=SendMessage(hWndWrapDelimitersEnable, BM_GETCHECK, 0, 0);
      if (lpFrameCurrent->bWrapDelimitersEnable)
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)lpFrameCurrent->wszWrapDelimiters);
      else
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)NULL);
    }
  }
  return FALSE;
}

BOOL CALLBACK OptionsAdvancedDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndDefaultSaveExt;
  static HWND hWndRememberKeybLayout;
  static HWND hWndReplaceAllAndClose;
  static HWND hWndInSelIfSel;
  static HWND hWndSaveInReadOnlyMsg;
  static HWND hWndDateLog;

  if (uMsg == WM_INITDIALOG)
  {
    hWndDefaultSaveExt=GetDlgItem(hDlg, IDC_OPTIONS_DEFAULT_SAVE_EXT);
    hWndRememberKeybLayout=GetDlgItem(hDlg, IDC_OPTIONS_REMEMBER_KEYBLAYOUT);
    hWndReplaceAllAndClose=GetDlgItem(hDlg, IDC_OPTIONS_REPLACEALL_CLOSE);
    hWndInSelIfSel=GetDlgItem(hDlg, IDC_OPTIONS_INSELIFSEL);
    hWndSaveInReadOnlyMsg=GetDlgItem(hDlg, IDC_OPTIONS_SAVEIN_READONLY_MSG);
    hWndDateLog=GetDlgItem(hDlg, IDC_OPTIONS_LOGDATE);

    if (moCur.bKeybLayoutMDI)
      SendMessage(hWndRememberKeybLayout, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.dwSearchOptions & AEFR_REPLACEALLANDCLOSE)
      SendMessage(hWndReplaceAllAndClose, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.dwSearchOptions & AEFR_CHECKINSELIFSEL)
      SendMessage(hWndInSelIfSel, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.bSaveInReadOnlyMsg)
      SendMessage(hWndSaveInReadOnlyMsg, BM_SETCHECK, BST_CHECKED, 0);
    if (moCur.bDateLog)
      SendMessage(hWndDateLog, BM_SETCHECK, BST_CHECKED, 0);

    SetWindowTextWide(hWndDefaultSaveExt, moCur.wszDefaultSaveExt);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if ((int)((NMHDR *)lParam)->code == PSN_SETACTIVE)
    {
      nPropertyStartPage=SendMessage(hPropertyTab, TCM_GETCURSEL, 0, 0);
    }
    else if ((int)((NMHDR *)lParam)->code == PSN_APPLY)
    {
      //Default save extention
      GetWindowTextWide(hWndDefaultSaveExt, moCur.wszDefaultSaveExt, MAX_PATH);

      //Remember keyboard layout for each tab (MDI)
      moCur.bKeybLayoutMDI=SendMessage(hWndRememberKeybLayout, BM_GETCHECK, 0, 0);

      //Replace all and close dialog
      if (SendMessage(hWndReplaceAllAndClose, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.dwSearchOptions|=AEFR_REPLACEALLANDCLOSE;
      else
        moCur.dwSearchOptions&=~AEFR_REPLACEALLANDCLOSE;

      //Check "In selection" if selection not empty
      if (SendMessage(hWndInSelIfSel, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.dwSearchOptions|=AEFR_CHECKINSELIFSEL;
      else
        moCur.dwSearchOptions&=~AEFR_CHECKINSELIFSEL;

      //Save in read only file message
      moCur.bSaveInReadOnlyMsg=SendMessage(hWndSaveInReadOnlyMsg, BM_GETCHECK, 0, 0);

      //.LOG feature
      moCur.bDateLog=SendMessage(hWndDateLog, BM_GETCHECK, 0, 0);
    }
  }
  return FALSE;
}


//// MDI list

BOOL CALLBACK MdiListDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
  static HWND hWndFilesGroup;
  static HWND hWndSave;
  static HWND hWndClose;
  static HWND hWndModified;
  static HWND hWndNames;
  static HWND hWndCancel;
  static HMENU hMenuList;
  static int nModifyFilter;
  static BOOL bListChanged;
  static DIALOGRESIZE drs[]={{&hWndList,         DRS_SIZE|DRS_X, 0},
                             {&hWndList,         DRS_SIZE|DRS_Y, 0},
                             {&hWndStats,        DRS_MOVE|DRS_X, 0},
                             {&hWndStats,        DRS_MOVE|DRS_Y, 0},
                             {&hWndSearch,       DRS_SIZE|DRS_X, 0},
                             {&hWndSearch,       DRS_MOVE|DRS_Y, 0},
                             {&hWndOK,           DRS_MOVE|DRS_X, 0},
                             {&hWndArrangeGroup, DRS_MOVE|DRS_X, 0},
                             {&hWndHorz,         DRS_MOVE|DRS_X, 0},
                             {&hWndVert,         DRS_MOVE|DRS_X, 0},
                             {&hWndTabsGroup,    DRS_MOVE|DRS_X, 0},
                             {&hWndUp,           DRS_MOVE|DRS_X, 0},
                             {&hWndDown,         DRS_MOVE|DRS_X, 0},
                             {&hWndSort,         DRS_MOVE|DRS_X, 0},
                             {&hWndFilesGroup,   DRS_MOVE|DRS_X, 0},
                             {&hWndSave,         DRS_MOVE|DRS_X, 0},
                             {&hWndClose,        DRS_MOVE|DRS_X, 0},
                             {&hWndModified,     DRS_MOVE|DRS_X, 0},
                             {&hWndNames,        DRS_MOVE|DRS_X, 0},
                             {&hWndCancel,       DRS_MOVE|DRS_X, 0},
                             {&hWndCancel,       DRS_MOVE|DRS_Y, 0},
                             {0, 0, 0}};
  static int lpMenuItems[]={IDC_MDILIST_HORZ,
                            IDC_MDILIST_VERT,
                            -1,
                            IDC_MDILIST_UP,
                            IDC_MDILIST_DOWN,
                            IDC_MDILIST_SORT,
                            -1,
                            IDC_MDILIST_SAVE,
                            IDC_MDILIST_CLOSE,
                            -1,
                            IDC_MDILIST_ALL,
                            IDC_MDILIST_ONLYMODIFIED,
                            IDC_MDILIST_ONLYUNMODIFIED,
                            -1,
                            IDC_MDILIST_INVERTSELECTION,
                            0};
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
    hWndFilesGroup=GetDlgItem(hDlg, IDC_MDILIST_FILES_GROUP);
    hWndSave=GetDlgItem(hDlg, IDC_MDILIST_SAVE);
    hWndClose=GetDlgItem(hDlg, IDC_MDILIST_CLOSE);
    hWndModified=GetDlgItem(hDlg, IDC_MDILIST_ONLYMODIFIED);
    hWndNames=GetDlgItem(hDlg, IDC_MDILIST_ONLYNAMES);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SendMessage(hWndSearch, EM_LIMITTEXT, MAX_PATH, 0);
    if (nMDI == WMD_PMDI)
    {
      EnableWindow(hWndArrangeGroup, FALSE);
      EnableWindow(hWndHorz, FALSE);
      EnableWindow(hWndVert, FALSE);
    }
    if (moCur.dwMdiListOptions & MLO_ONLYNAMES)
      SendMessage(hWndNames, BM_SETCHECK, BST_CHECKED, 0);

    nModifyFilter=MLMF_ALL;
    bListChanged=FALSE;
    FillMdiListListBox(hWndList, FALSE, nModifyFilter, (moCur.dwMdiListOptions & MLO_ONLYNAMES));
    if ((nItem=SendMessage(hTab, TCM_GETCURSEL, 0, 0)) != -1)
      SendMessage(hWndList, LB_SETSEL, TRUE, nItem);

    if (hMenuList=CreatePopupMenu())
    {
      for (nItem=0; lpMenuItems[nItem]; ++nItem)
      {
        if (lpMenuItems[nItem] == -1)
        {
          AppendMenuWide(hMenuList, MF_SEPARATOR, (UINT)-1, NULL);
        }
        else
        {
          if (lpMenuItems[nItem] == IDC_MDILIST_ALL)
            LoadStringWide(hLangLib, STR_ALLFILES, wbuf, BUFFER_SIZE);
          else if (lpMenuItems[nItem] == IDC_MDILIST_ONLYMODIFIED)
            LoadStringWide(hLangLib, STR_ONLYMODIFIED, wbuf, BUFFER_SIZE);
          else if (lpMenuItems[nItem] == IDC_MDILIST_ONLYUNMODIFIED)
            LoadStringWide(hLangLib, STR_ONLYUNMODIFIED, wbuf, BUFFER_SIZE);
          else if (lpMenuItems[nItem] == IDC_MDILIST_INVERTSELECTION)
            LoadStringWide(hLangLib, STR_INVERTSELECTION, wbuf, BUFFER_SIZE);
          else
            GetDlgItemTextWide(hDlg, lpMenuItems[nItem], wbuf, BUFFER_SIZE);
          AppendMenuWide(hMenuList, MF_STRING, lpMenuItems[nItem], wbuf);
        }
      }
    }

    PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndList)
    {
      POINT pt;
      HWND hWndControl;
      BOOL bEnable;

      if (lParam == -1)
      {
        pt.x=0;
        pt.y=0;
        ClientToScreen(hWndList, &pt);
      }
      else GetCursorPos(&pt);

      for (nItem=0; lpMenuItems[nItem]; ++nItem)
      {
        if (lpMenuItems[nItem] != -1)
        {
          if (hWndControl=GetDlgItem(hDlg, lpMenuItems[nItem]))
          {
            bEnable=IsWindowEnabled(hWndControl);
            EnableMenuItem(hMenuList, lpMenuItems[nItem], bEnable?MF_ENABLED:MF_GRAYED);
          }
        }
      }
      CheckMenuRadioItem(hMenuList, IDC_MDILIST_ALL, IDC_MDILIST_ONLYUNMODIFIED, IDC_MDILIST_ALL + nModifyFilter, MF_BYCOMMAND);
      TrackPopupMenu(hMenuList, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_MDILIST_SEARCH)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        wchar_t wszSearch[MAX_PATH];

        SendMessage(hWndList, LB_SETSEL, FALSE, -1);

        if (GetWindowTextWide(hWndSearch, wszSearch, MAX_PATH))
        {
          for (nItem=0; 1; ++nItem)
          {
            if (ListBox_GetTextWide(hWndList, nItem, wbuf) == LB_ERR)
              break;
            if (xstrstrW(wbuf, (DWORD)-1, wszSearch, FALSE, NULL, NULL))
              SendMessage(hWndList, LB_SETSEL, TRUE, nItem);
          }
        }
        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
      }
    }
    else if (LOWORD(wParam) == IDC_MDILIST_HORZ)
    {
      ArrangeListBoxSelItems(hWndList, SB_HORZ);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_VERT)
    {
      ArrangeListBoxSelItems(hWndList, SB_VERT);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_UP)
    {
      if (ShiftListBoxSelItems(hWndList, FALSE))
      {
        bListChanged=TRUE;
        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
      }
    }
    else if (LOWORD(wParam) == IDC_MDILIST_DOWN)
    {
      if (ShiftListBoxSelItems(hWndList, TRUE))
      {
        bListChanged=TRUE;
        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
      }
    }
    else if (LOWORD(wParam) == IDC_MDILIST_SORT)
    {
      FillMdiListListBox(hWndList, TRUE, nModifyFilter, (moCur.dwMdiListOptions & MLO_ONLYNAMES));
      bListChanged=TRUE;
      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_SAVE)
    {
      SaveListBoxSelItems(hWndList);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_CLOSE)
    {
      if (CloseListBoxSelItems(hWndList))
        SetFocus(hWndList);
      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_ONLYNAMES)
    {
      FRAMEDATA *lpFrame;
      int nListBoxItem=0;
      BOOL bSelected;

      if (SendMessage(hWndNames, BM_GETCHECK, 0, 0) == BST_CHECKED)
        moCur.dwMdiListOptions|=MLO_ONLYNAMES;
      else
        moCur.dwMdiListOptions&=~MLO_ONLYNAMES;

      while ((int)(lpFrame=(FRAMEDATA *)SendMessage(hWndList, LB_GETITEMDATA, nListBoxItem, 0)) != LB_ERR)
      {
        xprintfW(wbuf, L"%s%s", (moCur.dwMdiListOptions & MLO_ONLYNAMES)?GetFileName(lpFrame->wszFile):lpFrame->wszFile, lpFrame->ei.bModified?L" *":L"");
        bSelected=SendMessage(hWndList, LB_GETSEL, nListBoxItem, 0);
        SendMessage(hWndList, LB_DELETESTRING, nListBoxItem, 0);
        ListBox_InsertStringWide(hWndList, nListBoxItem, wbuf);
        SendMessage(hWndList, LB_SETITEMDATA, nListBoxItem, (LPARAM)lpFrame);
        SendMessage(hWndList, LB_SETSEL, bSelected, nListBoxItem);

        ++nListBoxItem;
      }
      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_MDILIST_ALL ||
             LOWORD(wParam) == IDC_MDILIST_ONLYMODIFIED ||
             LOWORD(wParam) == IDC_MDILIST_ONLYUNMODIFIED)
    {
      if (LOWORD(wParam) - IDC_MDILIST_ALL != nModifyFilter)
      {
        nModifyFilter=LOWORD(wParam) - IDC_MDILIST_ALL;
        FillMdiListListBox(hWndList, FALSE, nModifyFilter, (moCur.dwMdiListOptions & MLO_ONLYNAMES));
        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
      }
    }
    else if (LOWORD(wParam) == IDC_MDILIST_INVERTSELECTION)
    {
      int *lpSelItems;
      int nCount;
      int nSelCount;
      int nItem;
      int nSelItem=0;

      if ((nCount=SendMessage(hWndList, LB_GETCOUNT, 0, 0)) > 0)
      {
        nSelCount=GetListBoxSelItems(hWndList, &lpSelItems);
        SendMessage(hWndList, LB_SETSEL, FALSE, -1);

        for (nItem=0; nItem < nCount; ++nItem)
        {
          while (nSelItem < nSelCount && lpSelItems[nSelItem] < nItem)
            ++nSelItem;

          if (nItem != lpSelItems[nSelItem])
            SendMessage(hWndList, LB_SETSEL, TRUE, nItem);
        }
        FreeListBoxSelItems(&lpSelItems);

        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_MDILIST_LIST, LBN_SELCHANGE), 0);
      }
    }
    else if (LOWORD(wParam) == IDC_MDILIST_LIST)
    {
      if (HIWORD(wParam) == LBN_SELCHANGE)
      {
        int nCount;
        int nSelCount;

        nCount=SendMessage(hWndList, LB_GETCOUNT, 0, 0);
        nSelCount=SendMessage(hWndList, LB_GETSELCOUNT, 0, 0);
        EnableWindow(hWndUp, nSelCount > 0 && !nModifyFilter);
        EnableWindow(hWndDown, nSelCount > 0 && !nModifyFilter);
        EnableWindow(hWndSort, !nModifyFilter);
        EnableWindow(hWndSave, nSelCount > 0);
        EnableWindow(hWndClose, nSelCount > 0);
        EnableWindow(hWndModified, !bListChanged);

        xprintfW(wbuf, L"%d / %d", nSelCount, nCount);
        SetWindowTextWide(hWndStats, wbuf);
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

      if (!nModifyFilter && bListChanged)
      {
        for (nItemNew=0; 1; ++nItemNew)
        {
          if ((nData=SendMessage(hWndList, LB_GETITEMDATA, nItemNew, 0)) == LB_ERR)
            break;
          if ((nItemOld=GetTabItemFromParam(hTab, nData)) != -1)
            MoveTabItem(hTab, nItemOld, nItemNew);
        }
        bListChanged=FALSE;
      }
      if (GetListBoxSelItems(hWndList, &lpSelItems))
      {
        if ((nData=SendMessage(hWndList, LB_GETITEMDATA, lpSelItems[0], 0)) != LB_ERR)
          if ((nItem=GetTabItemFromParam(hTab, nData)) != -1)
            SelectTabItem(hTab, nItem);
        FreeListBoxSelItems(&lpSelItems);
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
  DialogResizeMessages(&drs[0], &rcMdiListInitDialog, &moCur.rcMdiListCurrentDialog, DRM_GETMINMAXINFO|DRM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

void FillMdiListListBox(HWND hWnd, BOOL bSort, int nModifyFilter, BOOL bOnlyNames)
{
  TCITEMW tcItem;
  FRAMEDATA *lpFrame;
  int nTabItem=0;
  int nListBoxItem=0;
  int nNew;

  SendMessage(hWnd, LB_RESETCONTENT, 0, 0);

  for (;;)
  {
    tcItem.mask=TCIF_PARAM;
    if (!SendMessage(hTab, TCM_GETITEMW, nTabItem++, (LPARAM)&tcItem))
      break;
    lpFrame=(FRAMEDATA *)tcItem.lParam;

    if (nModifyFilter == MLMF_ALL ||
        (nModifyFilter == MLMF_ONLYMODIFIED && lpFrame->ei.bModified) ||
        (nModifyFilter == MLMF_ONLYUNMODIFIED && !lpFrame->ei.bModified))
    {
      xprintfW(wbuf, L"%s%s", bOnlyNames?GetFileName(lpFrame->wszFile):lpFrame->wszFile, lpFrame->ei.bModified?L" *":L"");

      if (bSort)
        nNew=ListBox_AddStringWide(hWnd, wbuf);
      else
        nNew=ListBox_InsertStringWide(hWnd, nListBoxItem++, wbuf);

      SendMessage(hWnd, LB_SETITEMDATA, nNew, (LPARAM)lpFrame);
    }
  }
}

int MoveListBoxItem(HWND hWnd, int nOldIndex, int nNewIndex)
{
  wchar_t *wpText;
  int nIndex=LB_ERR;
  int nData;
  int nTextLen;

  if ((nTextLen=SendMessage(hWnd, LB_GETTEXTLEN, nOldIndex, 0)) != LB_ERR)
  {
    if (wpText=AllocWideStr(nTextLen + 1))
    {
      ListBox_GetTextWide(hWnd, nOldIndex, wpText);
      nData=SendMessage(hWnd, LB_GETITEMDATA, nOldIndex, 0);

      SendMessage(hWnd, LB_DELETESTRING, nOldIndex, 0);
      nIndex=ListBox_InsertStringWide(hWnd, nNewIndex, wpText);
      SendMessage(hWnd, LB_SETITEMDATA, nIndex, nData);
      FreeWideStr(wpText);
    }
  }
  return nIndex;
}

BOOL ShiftListBoxSelItems(HWND hWnd, BOOL bMoveDown)
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

  if (nSelCount=GetListBoxSelItems(hWnd, &lpSelItems))
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

          MoveListBoxItem(hWnd, nOldIndex, nNewIndex);
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

          MoveListBoxItem(hWnd, nOldIndex, nNewIndex);
          SendMessage(hWnd, LB_SETSEL, TRUE, nNewIndex);
          bResult=TRUE;
        }
      }
    }
    FreeListBoxSelItems(&lpSelItems);
  }
  return bResult;
}

BOOL SaveListBoxSelItems(HWND hWnd)
{
  FRAMEDATA *lpFrame;
  int *lpSelItems;
  int nSelCount;
  int nItem;
  int i;
  BOOL bResult=TRUE;

  if (nSelCount=GetListBoxSelItems(hWnd, &lpSelItems))
  {
    for (i=nSelCount - 1; i >= 0; --i)
    {
      if ((int)(lpFrame=(FRAMEDATA *)SendMessage(hWnd, LB_GETITEMDATA, lpSelItems[i], 0)) != LB_ERR)
      {
        if ((nItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
        {
          SelectTabItem(hTab, nItem);

          if (!DoFileSave())
          {
            bResult=FALSE;
            break;
          }
          SendMessage(hWnd, LB_DELETESTRING, lpSelItems[i], 0);
          ListBox_InsertStringWide(hWnd, lpSelItems[i], lpFrameCurrent->wszFile);

          SendMessage(hWnd, LB_SETITEMDATA, lpSelItems[i], (LPARAM)lpFrame);
          SendMessage(hWnd, LB_SETSEL, TRUE, lpSelItems[i]);
        }
      }
    }
    FreeListBoxSelItems(&lpSelItems);
  }
  return bResult;
}

void ArrangeListBoxSelItems(HWND hWnd, int nBar)
{
  FRAMEDATA *lpFrame;
  RECT rcClient;
  int *lpSelItems;
  int nHeight;
  int nWidth;
  int nSelCount;
  int nItem;
  int i;

  if (nMDI == WMD_MDI)
  {
    if (nSelCount=GetListBoxSelItems(hWnd, &lpSelItems))
    {
      GetClientRect(hMdiClient, &rcClient);
      if (nBar == SB_HORZ)
        rcClient.top=rcClient.bottom - rcClient.bottom / nSelCount;
      else if (nBar == SB_VERT)
        rcClient.left=rcClient.right - rcClient.right / nSelCount;

      for (i=nSelCount - 1; i >= 0; --i)
      {
        if ((int)(lpFrame=(FRAMEDATA *)SendMessage(hWnd, LB_GETITEMDATA, lpSelItems[i], 0)) != LB_ERR)
        {
          if ((nItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
          {
            SelectTabItem(hTab, nItem);
            if (GetWindowLongWide(lpFrame->hWndEditParent, GWL_STYLE) & WS_MAXIMIZE)
              SendMessage(hMdiClient, WM_MDIRESTORE, (WPARAM)lpFrame->hWndEditParent, 0);

            MoveWindow(lpFrame->hWndEditParent, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);

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
      FreeListBoxSelItems(&lpSelItems);
    }
  }
}

BOOL CloseListBoxSelItems(HWND hWnd)
{
  FRAMEDATA *lpFrame;
  int *lpSelItems;
  int nSelCount;
  int i;
  BOOL bResult=TRUE;

  if (nSelCount=GetListBoxSelItems(hWnd, &lpSelItems))
  {
    for (i=nSelCount - 1; i >= 0; --i)
    {
      if ((int)(lpFrame=(FRAMEDATA *)SendMessage(hWnd, LB_GETITEMDATA, lpSelItems[i], 0)) != LB_ERR)
      {
        if (DestroyMdiFrameWindow(lpFrame) != FWDE_SUCCESS)
        {
          bResult=FALSE;
          break;
        }
        SendMessage(hWnd, LB_DELETESTRING, lpSelItems[i], 0);
      }
    }
    FreeListBoxSelItems(&lpSelItems);
  }
  return bResult;
}

int GetListBoxSelItems(HWND hWnd, int **lpSelItems)
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

void FreeListBoxSelItems(int **lpSelItems)
{
  if (lpSelItems && *lpSelItems)
  {
    API_HeapFree(hHeap, 0, (LPVOID)*lpSelItems);
    *lpSelItems=NULL;
  }
}


//// About

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    SetDlgItemTextWide(hDlg, IDC_ABOUT_VERSION, APP_ABOUT_VERSIONW);
    SetDlgItemTextWide(hDlg, IDC_ABOUT_HOMEPAGE, APP_ABOUT_HOMEPAGEW);
    SetDlgItemTextWide(hDlg, IDC_ABOUT_EMAIL_SHENGALTS, APP_ABOUT_EMAIL_SHENGALTSW);
    SetDlgItemTextWide(hDlg, IDC_ABOUT_EMAIL_KUZNETSOV, APP_ABOUT_EMAIL_KUZNETSOVW);
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
      ShellExecuteWide(GetParent(hDlg), L"open", APP_ABOUT_HOMEPAGEW, NULL, NULL, SW_MAXIMIZE);
      return TRUE;
    }
  }
  return FALSE;
}


//// Dockable windows

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
              MoveWindow(dkElement->hWnd, rcDock.left, ns->rcCurrent.top, rcDock.right, ns->rcCurrent.bottom, FALSE);
              RedrawWindow(dkElement->hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN);
            }
            else if (dkElement->nSide == DKS_TOP ||
                     dkElement->nSide == DKS_BOTTOM)
            {
              if (hDocksStack.nSizingSide) dkElement->rcSize.top=rcDock.top;
              MoveWindow(dkElement->hWnd, ns->rcCurrent.left, rcDock.top, ns->rcCurrent.right, rcDock.bottom, FALSE);
              RedrawWindow(dkElement->hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN);
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

BOOL TranslateDialog(HDOCK *hDocks, LPMSG lpMsg)
{
  if (hDlgModeless && IsDialogMessageWide(hDlgModeless, lpMsg))
  {
    if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
    {
      SendMessageWide(hDlgModeless, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
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
        if (dkElement->hWnd && IsDialogMessageWide(dkElement->hWnd, lpMsg))
        {
          if (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST)
          {
            SendMessageWide(dkElement->hWnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
          }
          return TRUE;
        }
      }
      dkElement=dkElement->next;
    }
  }
  return FALSE;
}


//// Procedures stack

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


//// Handles stack

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


//// MDI windows

FRAMEDATA* StackFrameInsert(HSTACK *hStack)
{
  FRAMEDATA *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(FRAMEDATA));
  return lpElement;
}

FRAMEDATA* StackFrameGetByIndex(HSTACK *hStack, int nIndex)
{
  FRAMEDATA *lpFrame;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&lpFrame, nIndex))
    return lpFrame;
  return NULL;
}

FRAMEDATA* StackFrameGetByHandle(HSTACK *hStack, AEHDOC hDocEdit)
{
  FRAMEDATA *lpFrame=(FRAMEDATA *)hStack->last;

  while (lpFrame)
  {
    if (lpFrame->ei.hDocEdit == hDocEdit)
      return lpFrame;

    lpFrame=lpFrame->prev;
  }
  return NULL;
}

FRAMEDATA* StackFrameGetByName(HSTACK *hStack, const wchar_t *wpFileName, int nFileNameLen)
{
  FRAMEDATA *lpFrame=(FRAMEDATA *)hStack->first;

  if (nFileNameLen == -1)
    nFileNameLen=lstrlenW(wpFileName);

  if (lpFrameCurrent->ei.hWndEdit)
  {
    if (lpFrameCurrent->nFileLen == nFileNameLen)
    {
      if (!xstrcmpiW(lpFrameCurrent->wszFile, wpFileName))
        return lpFrameCurrent;
    }
  }

  while (lpFrame)
  {
    if (lpFrame->nFileLen == nFileNameLen)
    {
      if (!xstrcmpiW(lpFrame->wszFile, wpFileName))
        return lpFrame;
    }
    lpFrame=lpFrame->next;
  }
  return NULL;
}

FRAMEDATA* StackFrameGetNext(HSTACK *hStack, FRAMEDATA *lpFrame, BOOL bPrev)
{
  if (!bPrev)
  {
    if (lpFrame)
    {
      if (!lpFrame->next)
        return (FRAMEDATA *)hStack->first;
      else
        return lpFrame->next;
    }
  }
  else
  {
    if (lpFrame)
    {
      if (!lpFrame->prev)
        return (FRAMEDATA *)hStack->last;
      else
        return lpFrame->prev;
    }
  }
  return NULL;
}

void StackFrameMove(HSTACK *hStack, FRAMEDATA *lpFrame, int nIndex)
{
  StackMoveIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpFrame, nIndex);
}

void StackFrameDelete(HSTACK *hStack, FRAMEDATA *lpFrame)
{
  if (lpFrame == lpFrameCurrent)
    lpFrameCurrent=&fdInit;
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpFrame);
}

void StackFramesFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}


//// Owner-drawn buttons

BUTTONDRAWITEM* StackButtonDrawInsert(HSTACK *hStack)
{
  BUTTONDRAWITEM *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(BUTTONDRAWITEM));
  return lpElement;
}

BUTTONDRAWITEM* StackButtonDrawGet(HSTACK *hStack, HWND hWnd)
{
  BUTTONDRAWITEM *lpButtonDraw=(BUTTONDRAWITEM *)hStack->last;

  while (lpButtonDraw)
  {
    if (lpButtonDraw->hWnd == hWnd)
      return lpButtonDraw;

    lpButtonDraw=lpButtonDraw->prev;
  }
  return NULL;
}

void StackButtonDrawDelete(HSTACK *hStack, BUTTONDRAWITEM *lpButtonDraw)
{
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpButtonDraw);
}

void StackButtonDrawFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}


//// Status bar

void SetSelectionStatus(AEHDOC hDocEdit, HWND hWndEdit, AECHARRANGE *cr, AECHARINDEX *ci)
{
  if (lpFrameCurrent->ei.hDocEdit == hDocEdit)
  {
    wchar_t wszStatus[MAX_PATH];
    int nLine;
    int nColumn;
    BOOL bColumnSel;

    nLoopCase=0;

    if (cr && ci)
    {
      crSel=*cr;
      ciCaret=*ci;
      bColumnSel=SendMessage(hWndEdit, AEM_GETCOLUMNSEL, 0, 0);
    }
    else
    {
      GetSel(hWndEdit, &crSel, &bColumnSel, &ciCaret);
      lpFrameCurrent->nSelSubtract=0;
    }

    if (!(moCur.dwStatusPosType & SPT_LINEWRAP) && lpFrameCurrent->ei.bWordWrap)
      nLine=SendMessage(hWndEdit, AEM_GETUNWRAPLINE, (WPARAM)ciCaret.nLine, 0);
    else
      nLine=ciCaret.nLine;

    if (moCur.dwStatusPosType & SPT_COLUMN)
      nColumn=SendMessage(hWndEdit, AEM_INDEXTOCOLUMN, MAKELONG(lpFrameCurrent->nTabStopSize, !(moCur.dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);
    else
      nColumn=SendMessage(hWndEdit, AEM_INDEXTOCOLUMN, MAKELONG(1, !(moCur.dwStatusPosType & SPT_LINEWRAP)), (LPARAM)&ciCaret);

    if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
    {
      xprintfW(wszStatus, L"%u:%u", nLine + 1, nColumn + 1);
      lpFrameCurrent->nSelSubtract=0;
    }
    else
    {
      if (bColumnSel || !lpFrameCurrent->nSelSubtract || mod(lpFrameCurrent->crPrevSel.ciMin.nLine - crSel.ciMin.nLine) + mod(lpFrameCurrent->crPrevSel.ciMax.nLine - crSel.ciMax.nLine) >= crSel.ciMax.nLine - crSel.ciMin.nLine)
      {
        lpFrameCurrent->nSelSubtract=IndexSubtract(hWndEdit, &crSel.ciMax, &crSel.ciMin, AELB_ASOUTPUT, -1);
      }
      else
      {
        lpFrameCurrent->nSelSubtract+=IndexSubtract(hWndEdit, &lpFrameCurrent->crPrevSel.ciMin, &crSel.ciMin, AELB_ASOUTPUT, -1);
        lpFrameCurrent->nSelSubtract+=IndexSubtract(hWndEdit, &crSel.ciMax, &lpFrameCurrent->crPrevSel.ciMax, AELB_ASOUTPUT, -1);
      }
      xprintfW(wszStatus, L"%u:%u, %u", nLine + 1, nColumn + 1, lpFrameCurrent->nSelSubtract);
      if (bColumnSel) lpFrameCurrent->nSelSubtract=0;
    }
    lpFrameCurrent->crPrevSel=crSel;

    StatusBar_SetTextWide(hStatus, STATUS_POSITION, wszStatus);
    UpdateStatusUser(lpFrameCurrent, CSB_CHARHEX|CSB_CHARDEC|CSB_REALOFFSET|CSB_RICHOFFSET);
  }
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SetModifyStatus(FRAMEDATA *lpFrame, BOOL bState)
{
  if (!lpFrame || lpFrame == lpFrameCurrent)
  {
    //Read only status
    if (lpFrameCurrent->ei.bReadOnly)
    {
      if (!ssStatus.bReadOnly)
      {
        LoadStringWide(hLangLib, STR_READONLY, wbuf, BUFFER_SIZE);
        StatusBar_SetTextWide(hStatus, STATUS_MODIFY, wbuf);
        ssStatus.bReadOnly=TRUE;
      }
    }
    else
    {
      if (ssStatus.bReadOnly)
      {
        if (ssStatus.bModified)
          LoadStringWide(hLangLib, STR_MODIFIED, wbuf, BUFFER_SIZE);
        else
          wbuf[0]='\0';
        StatusBar_SetTextWide(hStatus, STATUS_MODIFY, wbuf);
        ssStatus.bReadOnly=FALSE;
      }
    }

    //Modify status
    if (moCur.dwShowModify & SM_STATUSBAR)
    {
      if (ssStatus.bModified != bState)
      {
        if (!ssStatus.bReadOnly)
        {
          if (bState)
            LoadStringWide(hLangLib, STR_MODIFIED, wbuf, BUFFER_SIZE);
          else
            wbuf[0]='\0';
          StatusBar_SetTextWide(hStatus, STATUS_MODIFY, wbuf);
        }
        ssStatus.bModified=bState;
      }
    }
    if (!lpFrame) return;

    lpFrameCurrent->ei.bModified=bState;
  }
  else lpFrame->ei.bModified=bState;

  //Set modify flag
  SendMessage(lpFrame->ei.hWndEdit, AEM_SETMODIFY, bState, 0);
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SetOvertypeStatus(FRAMEDATA *lpFrame, BOOL bState)
{
  if (!lpFrame || lpFrame == lpFrameCurrent)
  {
    if (ssStatus.bOvertypeMode != bState)
    {
      StatusBar_SetTextWide(hStatus, STATUS_INSERT, bState?L"Ovr":L"Ins");
      ssStatus.bOvertypeMode=bState;
    }
    if (!lpFrame) return;

    lpFrameCurrent->ei.bOvertypeMode=bState;
  }
  else lpFrame->ei.bOvertypeMode=bState;

  //Set overtype mode
  SendMessage(lpFrame->ei.hWndEdit, AEM_SETOVERTYPE, bState, 0);
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SetNewLineStatus(FRAMEDATA *lpFrame, int nState, DWORD dwFlags)
{
  if (!lpFrame || lpFrame == lpFrameCurrent)
  {
    if (ssStatus.nNewLine != nState)
    {
      if (nState == NEWLINE_WIN)
        StatusBar_SetTextWide(hStatus, STATUS_NEWLINE, L"Win");
      else if (nState == NEWLINE_UNIX)
        StatusBar_SetTextWide(hStatus, STATUS_NEWLINE, L"Unix");
      else if (nState == NEWLINE_MAC)
        StatusBar_SetTextWide(hStatus, STATUS_NEWLINE, L"Mac");
      ssStatus.nNewLine=nState;
    }
    if (!lpFrame) return;

    lpFrameCurrent->ei.nNewLine=nState;
  }
  else lpFrame->ei.nNewLine=nState;

  //Set new line
  SendMessage(lpFrame->ei.hWndEdit, AEM_SETNEWLINE, AENL_INPUT|AENL_OUTPUT, MAKELONG(AELB_ASIS, AELB_ASIS));

  if (nState == NEWLINE_WIN)
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_RN, AELB_RN));
  else if (nState == NEWLINE_UNIX)
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_N, AELB_N));
  else if (nState == NEWLINE_MAC)
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETNEWLINE, dwFlags, MAKELONG(AELB_R, AELB_R));

  lpFrame->nSelSubtract=0;
  SendMessage(lpFrame->ei.hWndEdit, AEM_UPDATESEL, AESELT_LOCKSCROLL|AESELT_COLUMNASIS, 0);
}

void SetCodePageStatus(FRAMEDATA *lpFrame, int nCodePage, BOOL bBOM)
{
  if (!lpFrame || lpFrame == lpFrameCurrent)
  {
    if (ssStatus.nCodePage != nCodePage || ssStatus.bBOM != bBOM)
    {
      GetCodePageName(nCodePage, wbuf, BUFFER_SIZE);

      if (IsCodePageUnicode(nCodePage))
      {
        if (!bBOM) xprintfW(wbuf, L"%s%s", wbuf, STR_NOBOMW);
      }
      StatusBar_SetTextWide(hStatus, STATUS_CODEPAGE, wbuf);

      ssStatus.nCodePage=nCodePage;
      ssStatus.bBOM=bBOM;
    }
    if (!lpFrame) return;

    lpFrameCurrent->ei.nCodePage=nCodePage;
    lpFrameCurrent->ei.bBOM=bBOM;
  }
  else
  {
    lpFrame->ei.nCodePage=nCodePage;
    lpFrame->ei.bBOM=bBOM;
  }
}

void UpdateStatusUser(FRAMEDATA *lpFrame, DWORD dwFlags)
{
  if (moCur.wszStatusUserFormat[0])
  {
    if (moCur.dwStatusUserFlags & dwFlags)
    {
      if ((moCur.dwStatusUserFlags & CSB_REALOFFSET) && (dwFlags & CSB_REALOFFSET))
        lpFrame->nCaretRealOffset=-IndexSubtract(lpFrame->ei.hWndEdit, NULL, &ciCaret, AELB_ASIS, FALSE);
      if ((moCur.dwStatusUserFlags & CSB_RICHOFFSET) && (dwFlags & CSB_RICHOFFSET))
        lpFrame->nCaretRichOffset=AkelIndexToRichOffset(lpFrame->ei.hWndEdit, &ciCaret);
      if ((moCur.dwStatusUserFlags & CSB_FONTPOINT) && (dwFlags & CSB_FONTPOINT))
        lpFrame->nFontPoint=GetFontPoint(lpFrame->ei.hWndEdit, &lpFrame->lf);

      if (TranslateStatusUser(lpFrame, moCur.wszStatusUserFormat, wbuf, BUFFER_SIZE))
        StatusBar_SetTextWide(hStatus, STATUS_USER, wbuf);
    }
  }
}

DWORD TranslateStatusUser(FRAMEDATA *lpFrame, const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%ch - hex character code (lowercase),
  //%cH - hex character code (uppercase),
  //%cd - decimal character code,
  //%0 - offset, %f - font size, %t - tab size, %m - marker value,
  //%r - replace count.
  //%% - %
  DWORD dwFlags=0;
  DWORD i;

  for (i=0; *wpString; ++wpString)
  {
    if (*wpString == '%')
    {
      if (*++wpString == '%')
      {
        if (lpFrame && wszBuffer) wszBuffer[i]='%';
        ++i;
      }
      else if (*wpString == 'c' || *wpString == 'C')
      {
        if (*++wpString == 'h' || *wpString == 'H')
        {
          if (lpFrame)
          {
            int nChar=AEC_CharAtIndex(&ciCaret);

            i+=xprintfW(wszBuffer?wszBuffer + i:NULL, (*wpString == 'h')?L"%04x":L"%04X", (nChar == -1)?0xFFFF:nChar);
          }
          else dwFlags|=CSB_CHARHEX;
        }
        else if (*wpString == 'd' || *wpString == 'D')
        {
          if (lpFrame)
          {
            int nChar=AEC_CharAtIndex(&ciCaret);

            i+=xprintfW(wszBuffer?wszBuffer + i:NULL, L"%d", nChar);
          }
          else dwFlags|=CSB_CHARDEC;
        }
      }
      else if (*wpString == 'O')
      {
        if (lpFrame)
          i+=xprintfW(wszBuffer?wszBuffer + i:NULL, L"%d", lpFrame->nCaretRealOffset);
        else
          dwFlags|=CSB_REALOFFSET;
      }
      else if (*wpString == 'o')
      {
        if (lpFrame)
          i+=xprintfW(wszBuffer?wszBuffer + i:NULL, L"%d", lpFrame->nCaretRichOffset);
        else
          dwFlags|=CSB_RICHOFFSET;
      }
      else if (*wpString == 'f' || *wpString == 'F')
      {
        if (lpFrame)
          i+=xprintfW(wszBuffer?wszBuffer + i:NULL, L"%d", lpFrame->nFontPoint);
        else
          dwFlags|=CSB_FONTPOINT;
      }
      else if (*wpString == 't' || *wpString == 'T')
      {
        if (lpFrame)
          i+=xprintfW(wszBuffer?wszBuffer + i:NULL, L"%d", lpFrame->nTabStopSize);
        else
          dwFlags|=CSB_TABSIZE;
      }
      else if (*wpString == 'm' || *wpString == 'M')
      {
        if (lpFrame)
          i+=xprintfW(wszBuffer?wszBuffer + i:NULL, L"%d", lpFrame->dwMarker);
        else
          dwFlags|=CSB_MARKER;
      }
      else break;
    }
    else
    {
      if (lpFrame && wszBuffer) wszBuffer[i]=*wpString;
      ++i;
    }
  }
  if (lpFrame && wszBuffer) wszBuffer[i]='\0';

  if (lpFrame)
    return i;
  else
    return dwFlags;
}


//// Associations

const wchar_t* GetAssociatedIconW(const wchar_t *wpFile, wchar_t *wszIconFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall)
{
  wchar_t wszRoot[MAX_PATH]=L"";
  wchar_t wszKey[MAX_PATH];
  wchar_t wszValue[MAX_PATH];
  wchar_t wszTemp[MAX_PATH];
  wchar_t *wpFileName;
  const wchar_t *wpExt;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int nIndex=0;
  int i;
  int j;

  if (wpExt=GetFileExt(wpFile))
  {
    if (RegOpenKeyExWide(HKEY_CLASSES_ROOT, wpExt - 1, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
      dwSize=BUFFER_SIZE * sizeof(wchar_t);
      RegQueryValueExWide(hKey, L"", NULL, &dwType, (LPBYTE)wszRoot, &dwSize);
      RegCloseKey(hKey);

      if (wszRoot[0])
      {
        //Get DefaultIcon string
        xprintfW(wszKey, L"%s\\DefaultIcon", wszRoot);

        if (RegOpenKeyExWide(HKEY_CLASSES_ROOT, wszKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          RegQueryValueExWide(hKey, L"", NULL, &dwType, (LPBYTE)wszValue, &dwSize);
          RegCloseKey(hKey);

          for (i=0, j=0; wszValue[i]; ++i)
          {
            if (wszValue[i] != '\"')
              wszValue[j++]=wszValue[i];
          }
          wszValue[j]='\0';
        }

        //If no DefaultIcon or if it equal to %1
        if (!hKey || !xstrcmpW(wszValue, L"%1"))
        {
          xprintfW(wszKey, L"%s\\shell\\open\\command", wszRoot);

          if (RegOpenKeyExWide(HKEY_CLASSES_ROOT, wszKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
          {
            dwSize=BUFFER_SIZE * sizeof(wchar_t);
            RegQueryValueExWide(hKey, L"", NULL, &dwType, (LPBYTE)wszValue, &dwSize);
            RegCloseKey(hKey);

            for (i=0, j=0; wszValue[i]; ++i)
            {
              if (wszValue[i] != '\"')
                wszValue[j++]=wszValue[i];
              else
                if (i != 0) break;
            }
            wszValue[j]='\0';
          }

          if (!hKey || !xstrcmpW(wszValue, L"%1"))
          {
            xstrcpynW(wszValue, wpFile, MAX_PATH);
            wpExt=NULL;
          }
        }

        //Extract icons
        for (i=lstrlenW(wszValue) - 1; i > 0; --i)
        {
          if (wszValue[i] == ',')
          {
            wszValue[i]='\0';
            nIndex=xatoiW(wszValue + i + 1, NULL);
            if (nIndex == -1) nIndex=0;
            break;
          }
          else if (wszValue[i] == '-' || (wszValue[i] >= '0' && wszValue[i] <= '9'))
            continue;
          break;
        }
        ExpandEnvironmentStringsWide(wszValue, wszTemp, MAX_PATH);
        if (SearchPathWide(NULL, wszTemp, NULL, MAX_PATH, wszValue, &wpFileName))
        {
          ExtractIconExWide(wszValue, nIndex, phiconLarge, phiconSmall, 1);

          if (wszIconFile) xstrcpynW(wszIconFile, wszValue, MAX_PATH);
          if (nIconIndex) *nIconIndex=nIndex;
          return wpExt;
        }
      }
    }
  }
  if (wszIconFile) wszIconFile[0]='\0';
  if (nIconIndex) *nIconIndex=0;
  if (phiconLarge) *phiconLarge=0;
  if (phiconSmall) *phiconSmall=0;
  return NULL;
}

void AssociateFileTypesW(HINSTANCE hInstance, const wchar_t *wpFileTypes, DWORD dwFlags)
{
  wchar_t wszModule[MAX_PATH];
  wchar_t wszAssocKey[MAX_PATH];
  wchar_t wszTypeKey[16];
  wchar_t wszExt[16];
  const wchar_t *wpExtStart=wpFileTypes;
  const wchar_t *wpExtEnd=wpFileTypes;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;

  GetModuleFileNameWide(hInstance, wszModule, MAX_PATH);

  if (dwFlags & AE_OPEN) xstrcpyW(wszTypeKey, L"Open");
  else if (dwFlags & AE_EDIT) xstrcpyW(wszTypeKey, L"Edit");
  else if (dwFlags & AE_PRINT) xstrcpyW(wszTypeKey, L"Print");

  while (1)
  {
    if (wpExtEnd=AKD_wcschr(wpExtStart, ';'))
      xstrcpynW(wbuf, wpExtStart, wpExtEnd - wpExtStart + 1);
    else
      xstrcpynW(wbuf, wpExtStart, MAX_PATH);
    if (!*wbuf) goto NextExt;
    xprintfW(wszExt, L".%s", wbuf);

    if (RegCreateKeyExWide(HKEY_CLASSES_ROOT, wszExt, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
    {
      dwSize=MAX_PATH * sizeof(wchar_t);
      if (RegQueryValueExWide(hKey, L"", NULL, &dwType, (LPBYTE)wszAssocKey, &dwSize) != ERROR_SUCCESS || !*wszAssocKey)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          xprintfW(wszAssocKey, L"%sfile", wszExt + 1);
          RegSetValueExWide(hKey, L"", 0, REG_SZ, (LPBYTE)wszAssocKey, BytesInString(wszAssocKey));
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
        xprintfW(wbuf2, L"%s\\DefaultIcon", wszAssocKey);

        if (RegCreateKeyExWide(HKEY_CLASSES_ROOT, wbuf2, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExWide(hKey, L"", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) != ERROR_SUCCESS || !*wbuf2)
          {
            if (dwFlags & AE_ASSOCIATE) RegSetValueExWide(hKey, L"", 0, REG_SZ, (LPBYTE)wszModule, BytesInString(wszModule));
          }
          else if (dwFlags & AE_DEASSOCIATE)
          {
            if (!xstrcmpiW(wbuf2, wszModule)) RegDeleteValueWide(hKey, L"");
          }
          RegCloseKey(hKey);
        }
      }

      //Associate command
      xprintfW(wbuf2, L"%s\\Shell\\%s\\Command", wszAssocKey, wszTypeKey);
      xprintfW(wbuf, L"\"%s\"%s \"%%1\"", wszModule, (dwFlags & AE_PRINT)?L" /p":L"");

      if (RegCreateKeyExWide(HKEY_CLASSES_ROOT, wbuf2, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExWide(hKey, L"", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf2 && xstrcmpiW(wbuf, wbuf2))
            {
              if (RegQueryValueExWide(hKey, L"AkelUndo", NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                RegSetValueExWide(hKey, L"AkelUndo", 0, dwType, (LPBYTE)wbuf2, dwSize);
            }
          }
          RegSetValueExWide(hKey, L"", 0, REG_SZ, (LPBYTE)wbuf, BytesInString(wbuf));
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExWide(hKey, L"AkelUndo", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf2) RegSetValueExWide(hKey, L"", 0, dwType, (LPBYTE)wbuf2, dwSize);
            RegDeleteValueWide(hKey, L"AkelUndo");
          }
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExWide(hKey, L"", NULL, &dwType, (LPBYTE)wbuf2, &dwSize) == ERROR_SUCCESS)
          {
            if (!*wbuf2 || !xstrcmpiW(wbuf, wbuf2))
            {
              RegCloseKey(hKey);
              xprintfW(wbuf2, L"%s\\Shell\\%s", wszAssocKey, wszTypeKey);
              if (RegOpenKeyExWide(HKEY_CLASSES_ROOT, wbuf2, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
              {
                RegDeleteKeyWide(hKey, L"Command");
                RegCloseKey(hKey);
                xprintfW(wbuf2, L"%s\\Shell", wszAssocKey);
                if (RegOpenKeyExWide(HKEY_CLASSES_ROOT, wbuf2, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
                {
                  RegDeleteKeyWide(hKey, wszTypeKey);
                  dwSize=BUFFER_SIZE * sizeof(wchar_t);
                  if (RegEnumKeyExW(hKey, 0, wbuf2, &dwSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                  {
                    RegCloseKey(hKey);
                    if (RegOpenKeyExWide(HKEY_CLASSES_ROOT, wszAssocKey, 0, KEY_ENUMERATE_SUB_KEYS, &hKey) == ERROR_SUCCESS)
                      RegDeleteKeyWide(hKey, L"Shell");
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
      xprintfW(wbuf, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s", wszExt);

      if (RegOpenKeyExWide(HKEY_CURRENT_USER, wbuf, 0, KEY_QUERY_VALUE|KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
      {
        if (dwFlags & AE_ASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExWide(hKey, L"Application", NULL, &dwType, (LPBYTE)wbuf, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf) RegSetValueExWide(hKey, L"AkelUndo", 0, dwType, (LPBYTE)wbuf, dwSize);
            RegDeleteValueWide(hKey, L"Application");
          }
        }
        else if (dwFlags & AE_DEASSOCIATE)
        {
          dwSize=BUFFER_SIZE * sizeof(wchar_t);
          if (RegQueryValueExWide(hKey, L"AkelUndo", NULL, &dwType, (LPBYTE)wbuf, &dwSize) == ERROR_SUCCESS)
          {
            if (*wbuf) RegSetValueExWide(hKey, L"Application", 0, dwType, (LPBYTE)wbuf, dwSize);
            RegDeleteValueWide(hKey, L"AkelUndo");
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

ASSOCICON* StackIconInsert(HSTACK *hStack, const wchar_t *wpFile, int nFileLen)
{
  ASSOCICON *lpElement=NULL;

  if (nFileLen == -1)
    nFileLen=lstrlenW(wpFile);

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(ASSOCICON)))
  {
    xstrcpynW(lpElement->wszFile, wpFile, MAX_PATH);
    lpElement->nFileLen=nFileLen;
    lpElement->wpExt=GetAssociatedIconW(lpElement->wszFile, NULL, NULL, NULL, &lpElement->hIcon);

    return lpElement;
  }
  return NULL;
}

ASSOCICON* StackIconGet(HSTACK *hStack, const wchar_t *wpFile, int nFileLen, const wchar_t *wpExt)
{
  ASSOCICON *lpElement=(ASSOCICON *)hStack->first;

  if (nFileLen == -1)
    nFileLen=lstrlenW(wpFile);

  while (lpElement)
  {
    if ((lpElement->wpExt && !xstrcmpiW(lpElement->wpExt, wpExt)) ||
        (!lpElement->wpExt && lpElement->nFileLen == nFileLen &&
                              !xstrcmpiW(lpElement->wszFile, wpFile)))
    {
      return lpElement;
    }
    lpElement=lpElement->next;
  }
  return NULL;
}

void StackIconsFree(HSTACK *hStack)
{
  ASSOCICON *lpElement=(ASSOCICON *)hStack->first;

  while (lpElement)
  {
    if (lpElement->hIcon) DestroyIcon(lpElement->hIcon);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}


//// Fonts

//For WMD_PMDI required: lpFrame == lpFrameCurrent
HFONT SetChosenFont(HWND hWnd, const LOGFONTW *lfFont)
{
  FONTITEM *fi;

  if (!(fi=StackFontItemGet(&hFontsStack, lfFont)))
    fi=StackFontItemInsert(&hFontsStack, lfFont);
  SendMessage(hWnd, WM_SETFONT, (WPARAM)fi->hFont, FALSE);
  return fi->hFont;
}

int GetFontPoint(HWND hWnd, const LOGFONTW *lfFont)
{
  HDC hDC;
  int nPointSize=0;

  if (hDC=GetDC(hWnd))
  {
    nPointSize=-MulDiv(lfFont->lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
    ReleaseDC(hWnd, hDC);
  }
  return nPointSize;
}

FONTITEM* StackFontItemInsert(HSTACK *hStack, const LOGFONTW *lfFont)
{
  FONTITEM *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(FONTITEM)))
  {
    xmemcpy(&lpElement->lfFont, lfFont, sizeof(LOGFONTW));
    lpElement->hFont=(HFONT)CreateFontIndirectWide(&lpElement->lfFont);
    return lpElement;
  }
  return NULL;
}

FONTITEM* StackFontItemGet(HSTACK *hStack, const LOGFONTW *lfFont)
{
  FONTITEM *lpElement=(FONTITEM *)hStack->first;

  while (lpElement)
  {
    if (lpElement->lfFont.lfHeight == lfFont->lfHeight &&
        lpElement->lfFont.lfWeight == lfFont->lfWeight &&
        lpElement->lfFont.lfItalic == lfFont->lfItalic &&
        lpElement->lfFont.lfCharSet == lfFont->lfCharSet)
    {
      if (!xstrcmpiW(lpElement->lfFont.lfFaceName, lfFont->lfFaceName))
        return lpElement;
    }
    lpElement=lpElement->next;
  }
  return NULL;
}

void StackFontItemsFree(HSTACK *hStack)
{
  FONTITEM *lpElement=(FONTITEM *)hStack->first;

  while (lpElement)
  {
    if (lpElement->hFont) DeleteObject(lpElement->hFont);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}


//// Command line functions

wchar_t* GetCommandLineWide(void)
{
  if (bOldWindows)
  {
    AnsiToWide(GetCommandLineA(), -1, wszCmdLine, COMMANDLINE_SIZE);
    return wszCmdLine;
  }
  else return GetCommandLineW();
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
  wchar_t *lpwCmdLine=GetCommandLineWide();

  if (*lpwCmdLine++ == '\"')
    while (*lpwCmdLine != '\"' && *lpwCmdLine != '\0') ++lpwCmdLine;
  else
    while (*lpwCmdLine != ' ' && *lpwCmdLine != '\0') ++lpwCmdLine;
  if (*lpwCmdLine != '\0')
    while (*++lpwCmdLine == ' ');

  return lpwCmdLine;
}

int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg, BOOL bParseAsNotepad)
{
  const wchar_t *wpCount=wpCmdLine;
  wchar_t *wpArg=wszArg;
  wchar_t *wpArgMax=wszArg + nArgMax - 1;
  wchar_t wchFirstChar;
  wchar_t wchStopChar;
  wchar_t wchQuoteChar=0;

  while (*wpCount == ' ') ++wpCount;

  wchFirstChar=*wpCount;
  if (*wpCount == '\"' || *wpCount == '\'' || *wpCount == '`')
  {
    wchStopChar=*wpCount++;
    wchQuoteChar=wchStopChar;
  }
  else wchStopChar=' ';

  if (bParseAsNotepad && wchFirstChar != '/' && wchStopChar == ' ')
  {
    for (; *wpCount != '/' && *wpCount != '\"' && *wpCount != '\0'; ++wpCount)
    {
      if (wpArg < wpArgMax)
      {
        if (wszArg) *wpArg=*wpCount;
        ++wpArg;
      }
    }
    while (wpArg > wszArg && *(wpArg - 1) == ' ')
    {
      --wpArg;
      if (wszArg) *wpArg='\0';
    }
  }
  else
  {
    for (; *wpCount; ++wpCount)
    {
      if (!wchQuoteChar || *wpCount == wchQuoteChar)
      {
        if (*wpCount == wchStopChar)
        {
          if (wchStopChar != wchFirstChar && wchStopChar != ' ')
          {
            if (wpArg < wpArgMax)
            {
              if (wszArg) *wpArg=*wpCount;
              ++wpArg;
            }
          }
          break;
        }
        if (!wchQuoteChar && wchFirstChar == '/')
        {
          if (*wpCount == '\"' || *wpCount == '\'' || *wpCount == '`')
            wchQuoteChar=*wpCount;
          else if (*wpCount == '(')
            wchStopChar=')';
        }
        else wchQuoteChar=0;
      }
      if (wpArg < wpArgMax)
      {
        if (wszArg) *wpArg=*wpCount;
        ++wpArg;
      }
    }
  }
  if (*wpCount == wchStopChar)
    ++wpCount;
  if (wszArg) *wpArg='\0';
  if (wpNextArg)
    for (*wpNextArg=wpCount; **wpNextArg == ' '; ++*wpNextArg);

  return wpArg - wszArg;
}

int ParseCmdLine(const wchar_t **wppCmdLine, BOOL bOnLoad)
{
  const wchar_t *wpCmdLine;
  const wchar_t *wpCmdLineNext;
  wchar_t *wpAction;
  STACKEXTPARAM hParamStack={0};
  DWORD dwAction;
  HWND hWndFriend=NULL;
  BOOL bFileOpenedSDI=FALSE;

  if (wppCmdLine && *wppCmdLine)
  {
    wpCmdLine=*wppCmdLine;
    wpCmdLineNext=*wppCmdLine;

    for (; GetCommandLineArg(wpCmdLine, wszCmdArg, COMMANDARG_SIZE, &wpCmdLineNext, !(dwCmdLineOptions & CLO_NONOTEPADCMD)); wpCmdLine=wpCmdLineNext)
    {
      if (wszCmdArg[0] == '/')
      {
        //On load
        if (!xstrcmpiW(wszCmdArg, L"/REASSOC"))
        {
          if (moCur.dwFileTypesAssociated & AE_OPEN)
            AssociateFileTypesW(hInstance, moCur.wszFileTypesOpen, AE_OPEN|AE_ASSOCIATE);
          else if (moCur.dwFileTypesAssociated & AE_EDIT)
            AssociateFileTypesW(hInstance, moCur.wszFileTypesEdit, AE_EDIT|AE_ASSOCIATE);
          else if (moCur.dwFileTypesAssociated & AE_PRINT)
            AssociateFileTypesW(hInstance, moCur.wszFileTypesPrint, AE_PRINT|AE_ASSOCIATE);
          if (moCur.dwFileTypesAssociated) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/DEASSOC"))
        {
          if (moCur.dwFileTypesAssociated & AE_OPEN)
            AssociateFileTypesW(hInstance, moCur.wszFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
          else if (moCur.dwFileTypesAssociated & AE_EDIT)
            AssociateFileTypesW(hInstance, moCur.wszFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
          else if (moCur.dwFileTypesAssociated & AE_PRINT)
            AssociateFileTypesW(hInstance, moCur.wszFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
          if (moCur.dwFileTypesAssociated) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/QUIT"))
        {
          return PCLE_QUIT;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/END"))
        {
          return PCLE_END;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/P"))
        {
          dwCmdLineOptions|=CLO_GLOBALPRINT;
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/C+"))
        {
          dwCmdLineOptions&=~CLO_MSGOPENCREATENO;
          dwCmdLineOptions|=CLO_MSGOPENCREATEYES;
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/C-"))
        {
          dwCmdLineOptions&=~CLO_MSGOPENCREATEYES;
          dwCmdLineOptions|=CLO_MSGOPENCREATENO;
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/B+"))
        {
          dwCmdLineOptions&=~CLO_MSGOPENBINARYNO;
          dwCmdLineOptions|=CLO_MSGOPENBINARYYES;
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/B-"))
        {
          dwCmdLineOptions&=~CLO_MSGOPENBINARYYES;
          dwCmdLineOptions|=CLO_MSGOPENBINARYNO;
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/L+"))
        {
          dwCmdLineOptions&=~CLO_MSGSAVELOSTSYMBOLSNO;
          dwCmdLineOptions|=CLO_MSGSAVELOSTSYMBOLSYES;
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/L-"))
        {
          dwCmdLineOptions&=~CLO_MSGSAVELOSTSYMBOLSYES;
          dwCmdLineOptions|=CLO_MSGSAVELOSTSYMBOLSNO;
          continue;
        }
        else if (!xstrcmpiW(wszCmdArg, L"/X"))
        {
          dwCmdLineOptions|=CLO_NONOTEPADCMD;
          continue;
        }
        if (bOnLoad) return PCLE_ONLOAD;

        //Process actions
        if (lpFrameCurrent->ei.hWndEdit)
        {
          dwAction=0;

          if (!xstrcmpinW(L"/Command(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_COMMAND;
          }
          else if (!xstrcmpinW(L"/Call(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_CALL;
          }
          else if (!xstrcmpinW(L"/Exec(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_EXEC;
          }
          else if (!xstrcmpinW(L"/OpenFile(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_OPENFILE;
          }
          else if (!xstrcmpinW(L"/SaveFile(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_SAVEFILE;
          }
          else if (!xstrcmpinW(L"/Font(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_FONT;
          }
          else if (!xstrcmpinW(L"/Recode(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_RECODE;
          }
          else if (!xstrcmpinW(L"/Insert(", wszCmdArg, (DWORD)-1))
          {
            dwAction=EXTACT_INSERT;
          }

          if (dwAction)
          {
            for (wpAction=wszCmdArg; *wpAction != '('; ++wpAction);
            GetMethodParameters(&hParamStack, ++wpAction, NULL);

            if (dwAction == EXTACT_COMMAND)
            {
              int nCommand=GetParameterInt(&hParamStack, 1);

              if (nCommand)
              {
                SendMessage(hMainWnd, WM_COMMAND, nCommand, 0);
              }
            }
            else if (dwAction == EXTACT_CALL)
            {
              PLUGINCALLSENDW pcs;
              unsigned char *lpStruct=NULL;

              SetParametersExpChar(&hParamStack, lpFrameCurrent->wszFile, wszExeDir);

              if (CreateParametersStruct(&hParamStack, &lpStruct))
              {
                pcs.pFunction=(wchar_t *)*(int *)lpStruct;
                if (*(int *)(lpStruct + sizeof(int)) > (int)sizeof(int))
                  pcs.lParam=(LPARAM)(lpStruct + sizeof(int));
                else
                  pcs.lParam=0;
                //pcs.dwSupport=0;
                CallPluginSend(NULL, &pcs, 0);
              }
            }
            else if (dwAction == EXTACT_EXEC)
            {
              STARTUPINFOW si={0};
              PROCESS_INFORMATION pi={0};
              wchar_t *wpCmdLine=NULL;
              wchar_t *wpWorkDir=NULL;

              SetParametersExpChar(&hParamStack, lpFrameCurrent->wszFile, wszExeDir);
              wpCmdLine=(wchar_t *)GetParameterExpCharW(&hParamStack, 1);
              wpWorkDir=(wchar_t *)GetParameterExpCharW(&hParamStack, 2);

              if (wpCmdLine)
              {
                si.cb=sizeof(STARTUPINFOW);
                if (CreateProcessWide(NULL, wpCmdLine, NULL, NULL, FALSE, 0, NULL, wpWorkDir, &si, &pi))
                {
                  CloseHandle(pi.hProcess);
                  CloseHandle(pi.hThread);
                }
              }
            }
            else if (dwAction == EXTACT_OPENFILE ||
                     dwAction == EXTACT_SAVEFILE)
            {
              wchar_t *wpFile=NULL;
              int nCodePage=-1;
              BOOL bBOM=-1;

              SetParametersExpChar(&hParamStack, lpFrameCurrent->wszFile, wszExeDir);
              wpFile=(wchar_t *)GetParameterExpCharW(&hParamStack, 1);
              if (hParamStack.nElements >= 2)
                nCodePage=GetParameterInt(&hParamStack, 2);
              if (hParamStack.nElements >= 3)
                bBOM=GetParameterInt(&hParamStack, 3);

              if (dwAction == EXTACT_OPENFILE)
              {
                DWORD dwFlags=OD_ADT_BINARY_ERROR;

                if (nCodePage == -1)
                  dwFlags|=OD_ADT_DETECT_CODEPAGE;
                if (bBOM == -1)
                  dwFlags|=OD_ADT_DETECT_BOM;
                if (OpenDocument(NULL, wpFile, dwFlags, nCodePage, bBOM) != EOD_SUCCESS)
                  return PCLE_END;
              }
              else if (dwAction == EXTACT_SAVEFILE)
              {
                if (nCodePage == -1)
                  nCodePage=lpFrameCurrent->ei.nCodePage;
                if (bBOM == -1)
                  bBOM=lpFrameCurrent->ei.bBOM;
                if (SaveDocument(NULL, wpFile, nCodePage, bBOM, SD_UPDATE) != ESD_SUCCESS)
                  return PCLE_END;
              }
            }
            else if (dwAction == EXTACT_FONT)
            {
              wchar_t *wpFaceName=NULL;
              DWORD dwFontStyle=0;
              int nPointSize=0;
              HDC hDC;

              SetParametersExpChar(&hParamStack, lpFrameCurrent->wszFile, wszExeDir);
              wpFaceName=(wchar_t *)GetParameterExpCharW(&hParamStack, 1);
              dwFontStyle=GetParameterInt(&hParamStack, 2);
              nPointSize=GetParameterInt(&hParamStack, 3);

              if (nPointSize)
              {
                if (hDC=GetDC(lpFrameCurrent->ei.hWndEdit))
                {
                  lpFrameCurrent->lf.lfHeight=-MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
                  ReleaseDC(lpFrameCurrent->ei.hWndEdit, hDC);
                }
              }
              if (dwFontStyle != FS_NONE)
              {
                lpFrameCurrent->lf.lfWeight=(dwFontStyle == FS_FONTBOLD || dwFontStyle == FS_FONTBOLDITALIC)?FW_BOLD:FW_NORMAL;
                lpFrameCurrent->lf.lfItalic=(dwFontStyle == FS_FONTITALIC || dwFontStyle == FS_FONTBOLDITALIC)?TRUE:FALSE;
              }
              if (*wpFaceName != '\0')
              {
                xstrcpynW(lpFrameCurrent->lf.lfFaceName, wpFaceName, LF_FACESIZE);
              }
              SetChosenFont(lpFrameCurrent->ei.hWndEdit, &lpFrameCurrent->lf);
              UpdateMappedPrintWidth(lpFrameCurrent);
              UpdateStatusUser(lpFrameCurrent, CSB_FONTPOINT|CSB_MARKER);
            }
            else if (dwAction == EXTACT_RECODE)
            {
              TEXTRECODE tr;

              tr.nCodePageFrom=GetParameterInt(&hParamStack, 1);
              tr.nCodePageTo=GetParameterInt(&hParamStack, 2);
              RecodeTextW(lpFrameCurrent->ei.hWndEdit, tr.nCodePageFrom, tr.nCodePageTo);
            }
            else if (dwAction == EXTACT_INSERT)
            {
              wchar_t *wpText=NULL;
              wchar_t *wpUnescText=NULL;
              int nTextLen=-1;
              int nUnescTextLen;
              BOOL bEscSequences=FALSE;

              if (!IsReadOnly(lpFrameCurrent->ei.hWndEdit))
              {
                SetParametersExpChar(&hParamStack, lpFrameCurrent->wszFile, wszExeDir);
                wpText=(wchar_t *)GetParameterExpCharW(&hParamStack, 1);
                bEscSequences=GetParameterInt(&hParamStack, 2);

                if (bEscSequences)
                {
                  if (nUnescTextLen=TranslateEscapeString(lpFrameCurrent, wpText, NULL))
                  {
                    if (wpUnescText=(wchar_t *)GlobalAlloc(GPTR, nUnescTextLen * sizeof(wchar_t)))
                    {
                      nTextLen=TranslateEscapeString(lpFrameCurrent, wpText, wpUnescText);
                      wpText=wpUnescText;
                    }
                  }
                }
                if (wpText)
                {
                  ReplaceSelW(lpFrameCurrent->ei.hWndEdit, wpText, nTextLen, -1, NULL, NULL);
                }
                if (wpUnescText) GlobalFree((HGLOBAL)wpUnescText);
              }
            }
            FreeMethodParameters(&hParamStack);
          }
        }
        continue;
      }
      if (!*wszCmdArg) continue;

      //Open file
      if (nMDI == WMD_SDI)
      {
        if (moCur.bSingleOpenFile)
        {
          if (GetFullName(wszCmdArg, wszCmdArg, MAX_PATH))
          {
            if ((hWndFriend=FindWindowExWide(NULL, NULL, APP_SDI_CLASSW, wszCmdArg)) &&
                (hWndFriend=GetParent(hWndFriend)))
            {
              if (hWndFriend != hMainWnd)
              {
                ActivateWindow(hWndFriend);
                SendMessage(hWndFriend, AKD_SETCMDLINEOPTIONS, dwCmdLineOptions, 0);
                SendCmdLine(hWndFriend, wpCmdLine, TRUE);
                return PCLE_QUIT;
              }
            }
          }
        }
        if (bOnLoad) return PCLE_ONLOAD;

        if (!bFileOpenedSDI)
        {
          if (!SaveChanged())
            return PCLE_END;
          if (OpenDocument(NULL, wszCmdArg, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE) != EOD_SUCCESS)
            return PCLE_END;
          bFileOpenedSDI=TRUE;
          continue;
        }
        hWndFriend=DoFileNewWindow(STARTF_NOMUTEX);
        SendMessage(hWndFriend, AKD_SETCMDLINEOPTIONS, dwCmdLineOptions, 0);
        SendCmdLine(hWndFriend, wpCmdLine, TRUE);
        return PCLE_END;
      }
      if (bOnLoad) return PCLE_ONLOAD;

      //nMDI == WMD_MDI || nMDI == WMD_PMDI
      if (OpenDocument(NULL, wszCmdArg, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE) != EOD_SUCCESS)
        return PCLE_END;
    }
  }
  return PCLE_END;
}

void SendCmdLine(HWND hWnd, const wchar_t *wpCmdLine, BOOL bPost)
{
  COPYDATASTRUCT cds;
  PARSECMDLINEPOSTW *pclp;

  if (pclp=(PARSECMDLINEPOSTW *)GlobalAlloc(GMEM_FIXED, sizeof(PARSECMDLINEPOSTW)))
  {
    pclp->bPostMessage=bPost;
    pclp->nCmdLineLen=xstrcpynW(pclp->szCmdLine, wpCmdLine, COMMANDLINE_SIZE);
    pclp->nWorkDirLen=GetCurrentDirectoryWide(MAX_PATH, pclp->szWorkDir);

    cds.dwData=CD_PARSECMDLINEW;
    cds.cbData=sizeof(PARSECMDLINEPOSTW);
    cds.lpData=(PVOID)pclp;
    SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
    GlobalFree((HGLOBAL)pclp);
  }
}

void GetMethodParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpText, const wchar_t **wppText)
{
  EXTPARAM *lpParameter;
  const wchar_t *wpParamBegin=wpText;
  const wchar_t *wpParamEnd;
  wchar_t *wpString;
  wchar_t wchStopChar;
  int nStringLen;

  GetMethodParameter:
  while (*wpParamBegin == ' ' || *wpParamBegin == '\t') ++wpParamBegin;

  if (*wpParamBegin == '\"' || *wpParamBegin == '\'' || *wpParamBegin == '`')
  {
    //String
    wchStopChar=*wpParamBegin++;
    nStringLen=0;

    for (wpParamEnd=wpParamBegin; *wpParamEnd != wchStopChar && *wpParamEnd != '\0'; ++wpParamEnd)
      ++nStringLen;

    if (!StackInsertIndex((stack **)&hParamStack->first, (stack **)&hParamStack->last, (stack **)&lpParameter, -1, sizeof(EXTPARAM)))
    {
      ++hParamStack->nElements;

      if (lpParameter->wpString=(wchar_t *)GlobalAlloc(GPTR, (nStringLen + 1) * sizeof(wchar_t)))
      {
        lpParameter->dwType=EXTPARAM_CHAR;
        wpString=lpParameter->wpString;

        for (wpParamEnd=wpParamBegin; *wpParamEnd != wchStopChar && *wpParamEnd != '\0'; ++wpParamEnd)
          *wpString++=*wpParamEnd;
        *wpString='\0';

        if (bOldWindows)
        {
          nStringLen=WideCharToMultiByte(CP_ACP, 0, lpParameter->wpString, -1, NULL, 0, NULL, NULL);
          if (lpParameter->pString=(char *)GlobalAlloc(GPTR, nStringLen))
            WideCharToMultiByte(CP_ACP, 0, lpParameter->wpString, -1, lpParameter->pString, nStringLen, NULL, NULL);
        }
      }
    }
  }
  else
  {
    //Number
    for (wpParamEnd=wpParamBegin; *wpParamEnd != ',' && *wpParamEnd != ')' && *wpParamEnd != '\0'; ++wpParamEnd);

    if (!StackInsertIndex((stack **)&hParamStack->first, (stack **)&hParamStack->last, (stack **)&lpParameter, -1, sizeof(EXTPARAM)))
    {
      ++hParamStack->nElements;

      lpParameter->dwType=EXTPARAM_INT;
      lpParameter->nNumber=xatoiW(wpParamBegin, NULL);
    }
  }

  while (*wpParamEnd != ',' && *wpParamEnd != ')' && *wpParamEnd != '\0')
    ++wpParamEnd;
  if (*wpParamEnd == ',')
  {
    wpParamBegin=++wpParamEnd;
    goto GetMethodParameter;
  }
  if (*wpParamEnd == ')')
    ++wpParamEnd;
  if (wppText) *wppText=wpParamEnd;
}

void SetParametersExpChar(STACKEXTPARAM *hParamStack, const wchar_t *wpFile, const wchar_t *wpExeDir)
{
  //%f -file, %d -file directory, %a -AkelPad directory, %% -%
  EXTPARAM *lpParameter=(EXTPARAM *)hParamStack->first;
  wchar_t wszFileDir[MAX_PATH];
  wchar_t *wszString;
  wchar_t *wpCount;
  wchar_t *wpExpandStr;
  int nFileLen;
  int nFileDirLen;
  int nExeDirLen;
  int nExpandStrLen;
  int nEnvironmentLen;

  nFileLen=lstrlenW(wpFile);
  nFileDirLen=GetFileDir(wpFile, wszFileDir, MAX_PATH);
  nExeDirLen=lstrlenW(wpExeDir);
  nExpandStrLen=0;

  while (lpParameter)
  {
    if (lpParameter->dwType == EXTPARAM_CHAR)
    {
      nEnvironmentLen=ExpandEnvironmentStringsWide(lpParameter->wpString, NULL, 0);

      if (wszString=(wchar_t *)GlobalAlloc(GPTR, nEnvironmentLen * sizeof(wchar_t)))
      {
        //Expand environment strings
        ExpandEnvironmentStringsWide(lpParameter->wpString, wszString, nEnvironmentLen);

        //Calculate expanded size
        for (wpCount=wszString; *wpCount; ++wpCount)
        {
          if (*wpCount == '%')
          {
            if (*(wpCount + 1) == '%')
            {
              ++wpCount;
              ++nExpandStrLen;
              continue;
            }
            if (*(wpCount + 1) == 'f' || *(wpCount + 1) == 'F')
            {
              ++wpCount;
              nExpandStrLen+=nFileLen;
              continue;
            }
            if (*(wpCount + 1) == 'd' || *(wpCount + 1) == 'D')
            {
              ++wpCount;
              nExpandStrLen+=nFileDirLen;
              continue;
            }
            if (*(wpCount + 1) == 'a' || *(wpCount + 1) == 'A')
            {
              ++wpCount;
              nExpandStrLen+=nExeDirLen;
              continue;
            }
          }
          else ++nExpandStrLen;
        }

        //Expand string
        if (lpParameter->pExpanded)
        {
          GlobalFree((HGLOBAL)lpParameter->pExpanded);
          lpParameter->pExpanded=NULL;
        }
        if (lpParameter->wpExpanded)
        {
          GlobalFree((HGLOBAL)lpParameter->wpExpanded);
          lpParameter->wpExpanded=NULL;
        }

        if (lpParameter->wpExpanded=(wchar_t *)GlobalAlloc(GPTR, (nExpandStrLen + 1) * sizeof(wchar_t)))
        {
          wpExpandStr=lpParameter->wpExpanded;

          for (wpCount=wszString; *wpCount; ++wpCount)
          {
            if (*wpCount == '%')
            {
              if (*(wpCount + 1) == '%')
              {
                ++wpCount;
                *wpExpandStr++='%';
                continue;
              }
              if (*(wpCount + 1) == 'f' || *(wpCount + 1) == 'F')
              {
                ++wpCount;
                xstrcpyW(wpExpandStr, wpFile);
                wpExpandStr+=nFileLen;
                continue;
              }
              if (*(wpCount + 1) == 'd' || *(wpCount + 1) == 'D')
              {
                ++wpCount;
                xstrcpyW(wpExpandStr, wszFileDir);
                wpExpandStr+=nFileDirLen;
                continue;
              }
              if (*(wpCount + 1) == 'a' || *(wpCount + 1) == 'A')
              {
                ++wpCount;
                xstrcpyW(wpExpandStr, wpExeDir);
                wpExpandStr+=nExeDirLen;
                continue;
              }
            }
            else *wpExpandStr++=*wpCount;
          }
          *wpExpandStr='\0';

          if (bOldWindows)
          {
            nExpandStrLen=WideCharToMultiByte(CP_ACP, 0, lpParameter->wpExpanded, -1, NULL, 0, NULL, NULL);
            if (lpParameter->pExpanded=(char *)GlobalAlloc(GPTR, nExpandStrLen))
              WideCharToMultiByte(CP_ACP, 0, lpParameter->wpExpanded, -1, lpParameter->pExpanded, nExpandStrLen, NULL, NULL);
          }
        }
        GlobalFree((HGLOBAL)wszString);
      }
    }
    lpParameter=lpParameter->next;
  }
}

int CreateParametersStruct(STACKEXTPARAM *hParamStack, unsigned char **lppStruct)
{
  EXTPARAM *lpParameter=(EXTPARAM *)hParamStack->first;
  unsigned char *lpStruct=NULL;
  int nStructSize=0;
  int nOffset=0;
  int nIndex=0;

  if (hParamStack->nElements)
  {
    nStructSize=(hParamStack->nElements + 1) * sizeof(int);

    if (lpStruct=(unsigned char *)GlobalAlloc(GPTR, nStructSize))
    {
      while (lpParameter)
      {
        if (nIndex == 1)
        {
          //nIndex == 1 is the size of the call parameters structure
          *((int *)(lpStruct + nOffset))=hParamStack->nElements * sizeof(int);
          nOffset+=sizeof(int);
          ++nIndex;
        }

        //nIndex == 0 is function name, nIndex >= 2 is call parameters
        if (lpParameter->dwType == EXTPARAM_CHAR)
        {
          if (bOldWindows && nIndex > 0)
            *((int *)(lpStruct + nOffset))=(int)lpParameter->pExpanded;
          else
            *((int *)(lpStruct + nOffset))=(int)lpParameter->wpExpanded;
        }
        else *((int *)(lpStruct + nOffset))=lpParameter->nNumber;

        nOffset+=sizeof(int);
        ++nIndex;
        lpParameter=lpParameter->next;
      }
    }
  }
  *lppStruct=lpStruct;
  return nStructSize;
}

int GetParameterInt(STACKEXTPARAM *hParamStack, int nIndex)
{
  EXTPARAM *lpParameter;

  if (!StackGetElement((stack *)hParamStack->first, (stack *)hParamStack->last, (stack **)&lpParameter, nIndex))
  {
    if (lpParameter->dwType == EXTPARAM_INT)
      return lpParameter->nNumber;
  }
  return 0;
}

char* GetParameterCharA(STACKEXTPARAM *hParamStack, int nIndex)
{
  EXTPARAM *lpParameter;

  if (!StackGetElement((stack *)hParamStack->first, (stack *)hParamStack->last, (stack **)&lpParameter, nIndex))
  {
    if (lpParameter->dwType == EXTPARAM_CHAR)
      return lpParameter->pString;
  }
  return NULL;
}

wchar_t* GetParameterCharW(STACKEXTPARAM *hParamStack, int nIndex)
{
  EXTPARAM *lpParameter;

  if (!StackGetElement((stack *)hParamStack->first, (stack *)hParamStack->last, (stack **)&lpParameter, nIndex))
  {
    if (lpParameter->dwType == EXTPARAM_CHAR)
      return lpParameter->wpString;
  }
  return NULL;
}

char* GetParameterExpCharA(STACKEXTPARAM *hParamStack, int nIndex)
{
  EXTPARAM *lpParameter;

  if (!StackGetElement((stack *)hParamStack->first, (stack *)hParamStack->last, (stack **)&lpParameter, nIndex))
  {
    if (lpParameter->dwType == EXTPARAM_CHAR)
      return lpParameter->pExpanded;
  }
  return NULL;
}

wchar_t* GetParameterExpCharW(STACKEXTPARAM *hParamStack, int nIndex)
{
  EXTPARAM *lpParameter;

  if (!StackGetElement((stack *)hParamStack->first, (stack *)hParamStack->last, (stack **)&lpParameter, nIndex))
  {
    if (lpParameter->dwType == EXTPARAM_CHAR)
      return lpParameter->wpExpanded;
  }
  return NULL;
}

int TranslateEscapeString(FRAMEDATA *lpFrame, const wchar_t *wpInput, wchar_t *wszOutput)
{
  const wchar_t *a=wpInput;
  wchar_t *b=wszOutput;
  wchar_t whex[5];
  int nDec;

  for (whex[4]='\0'; *a; ++a)
  {
    if (*a == '\\')
    {
      if (*++a == '\\')
      {
        if (wszOutput) *b='\\';
        ++b;
      }
      else if (*a == 'n')
      {
        if (lpFrame->ei.nNewLine == NEWLINE_MAC)
        {
          if (wszOutput) *b='\r';
          ++b;
        }
        else if (lpFrame->ei.nNewLine == NEWLINE_UNIX)
        {
          if (wszOutput) *b='\n';
          ++b;
        }
        else if (lpFrame->ei.nNewLine == NEWLINE_WIN)
        {
          if (wszOutput) *b='\r';
          ++b;
          if (wszOutput) *b='\n';
          ++b;
        }
      }
      else if (*a == 't')
      {
        if (wszOutput) *b='\t';
        ++b;
      }
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
          nDec=hex2decW(whex);
          if (nDec == -1) goto Error;
          while (*++a == ' ');

          if (wszOutput) *b=nDec;
          ++b;
        }
        while (*a && *a != ']');

        if (!*a) goto Error;
      }
      else if (*a == 's')
      {
        AECHARRANGE cr;
        wchar_t *wszSelText=NULL;
        int nSelTextLen;

        GetSel(lpFrame->ei.hWndEdit, &cr, NULL, NULL);
        if (nSelTextLen=ExGetRangeTextW(lpFrame->ei.hWndEdit, &cr.ciMin, &cr.ciMax, FALSE, wszOutput?&wszSelText:NULL, AELB_ASIS, FALSE))
        {
          if (wszOutput)
          {
            xmemcpy(b, wszSelText, nSelTextLen * sizeof(wchar_t));
            FreeText(wszSelText);
          }
          else --nSelTextLen;

          b+=nSelTextLen;
        }
      }
      else goto Error;
    }
    else
    {
      if (wszOutput) *b=*a;
      ++b;
    }
  }
  if (wszOutput)
    *b='\0';
  else
    ++b;
  return (b - wszOutput);

  Error:
  if (wszOutput) *wszOutput='\0';
  return 0;
}

void FreeMethodParameters(STACKEXTPARAM *hParamStack)
{
  EXTPARAM *lpParameter=(EXTPARAM *)hParamStack->first;

  while (lpParameter)
  {
    if (lpParameter->dwType == EXTPARAM_CHAR)
    {
      if (lpParameter->pString) GlobalFree((HGLOBAL)lpParameter->pString);
      if (lpParameter->wpString) GlobalFree((HGLOBAL)lpParameter->wpString);
      if (lpParameter->pExpanded) GlobalFree((HGLOBAL)lpParameter->pExpanded);
      if (lpParameter->wpExpanded) GlobalFree((HGLOBAL)lpParameter->wpExpanded);
    }
    lpParameter=lpParameter->next;
  }
  StackClear((stack **)&hParamStack->first, (stack **)&hParamStack->last);
  hParamStack->nElements=0;
}


//// Other functions

BOOL GetEditInfo(HWND hWnd, EDITINFO *ei)
{
  if (!hWnd || IsEditActive(hWnd))
  {
    *ei=lpFrameCurrent->ei;
    return TRUE;
  }
  else
  {
    FRAMEDATA *lpFrame;

    if (lpFrame=GetFrameDataFromEditWindow(hWnd))
    {
      *ei=lpFrame->ei;
      return TRUE;
    }
  }
  return FALSE;
}

DWORD IsEditActive(HWND hWnd)
{
  if (!hWnd)
    return 0;
  if (hWnd == lpFrameCurrent->ei.hWndEdit)
    return CN_EDIT;

  if (lpFrameCurrent->ei.hWndMaster)
  {
    if (hWnd == lpFrameCurrent->ei.hWndMaster)
      return CN_MASTER;
    if (lpFrameCurrent->ei.hWndClone1 && hWnd == lpFrameCurrent->ei.hWndClone1)
      return CN_CLONE1;
    if (lpFrameCurrent->ei.hWndClone2 && hWnd == lpFrameCurrent->ei.hWndClone2)
      return CN_CLONE2;
    if (lpFrameCurrent->ei.hWndClone3 && hWnd == lpFrameCurrent->ei.hWndClone3)
      return CN_CLONE3;
  }
  return 0;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void UpdateShowHScroll(FRAMEDATA *lpFrame)
{
  if (!(moCur.dwPaintOptions & PAINT_HIDENOSCROLL))
  {
    BOOL bShowScroll=TRUE;

    if (lpFrame->ei.bWordWrap && !lpFrame->dwWrapLimit && !lpFrame->ei.hWndClone1 && !lpFrame->ei.hWndClone3)
      bShowScroll=FALSE;

    if (lpFrame->ei.hWndMaster)
    {
      SendMessage(lpFrame->ei.hWndMaster, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
      if (lpFrame->ei.hWndClone1)
        SendMessage(lpFrame->ei.hWndClone1, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
      if (lpFrame->ei.hWndClone2)
        SendMessage(lpFrame->ei.hWndClone2, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
      if (lpFrame->ei.hWndClone3)
        SendMessage(lpFrame->ei.hWndClone3, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
    }
    else SendMessage(lpFrame->ei.hWndEdit, AEM_SHOWSCROLLBAR, SB_HORZ, bShowScroll);
  }
}

int SaveLineScroll(HWND hWnd)
{
  return SendMessage(hWnd, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0);
}

void RestoreLineScroll(HWND hWnd, int nBeforeLine)
{
  int nAfterLine;

  nAfterLine=SendMessage(hWnd, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0);
  if (nBeforeLine != nAfterLine)
    SendMessage(hWnd, AEM_LINESCROLL, AESB_VERT|AESB_ALIGNTOP, nBeforeLine - nAfterLine);
}

DWORD ScrollCaret(HWND hWnd)
{
  AESCROLLTOPOINT stp;
  DWORD dwScrollResult;

  //Test scroll to caret
  stp.dwFlags=AESC_TEST|AESC_POINTCARET|AESC_OFFSETCHARX|AESC_OFFSETCHARY;
  stp.nOffsetX=1;
  stp.nOffsetY=0;
  dwScrollResult=SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);

  //Scroll to caret
  stp.dwFlags=AESC_POINTCARET;
  stp.nOffsetX=3;
  stp.nOffsetY=2;
  if (dwScrollResult & AECSE_SCROLLEDX)
    stp.dwFlags|=AESC_OFFSETRECTDIVX;
  if (dwScrollResult & AECSE_SCROLLEDY)
    stp.dwFlags|=AESC_OFFSETRECTDIVY;
  return SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);
}

BOOL SelectColorDialog(HWND hWndOwner, COLORREF *crColor)
{
  COLORREF crReserve=*crColor;

  if (bOldWindows)
  {
    CHOOSECOLORA ccA={0};

    ccA.lStructSize =sizeof(CHOOSECOLORA);
    ccA.lpCustColors=crCustColors;
    ccA.Flags       =CC_FULLOPEN|CC_RGBINIT;
    ccA.hwndOwner   =hWndOwner;
    ccA.rgbResult   =*crColor;

    if (ChooseColorA(&ccA))
    {
      *crColor=ccA.rgbResult;
      return TRUE;
    }
  }
  else
  {
    CHOOSECOLORW ccW={0};

    ccW.lStructSize =sizeof(CHOOSECOLORW);
    ccW.lpCustColors=crCustColors;
    ccW.Flags       =CC_FULLOPEN|CC_RGBINIT;
    ccW.hwndOwner   =hWndOwner;
    ccW.rgbResult   =*crColor;

    if (ChooseColorW(&ccW))
    {
      *crColor=ccW.rgbResult;
      return TRUE;
    }
  }
  *crColor=crReserve;
  return FALSE;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
BOOL GetCharColor(FRAMEDATA *lpFrame, CHARCOLOR *cc)
{
  AECHARRANGE cr;
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciCharIndex;

  GetSel(lpFrame->ei.hWndEdit, &cr, NULL, &ciCaretIndex);
  SendMessage(lpFrame->ei.hWndEdit, AEM_RICHOFFSETTOINDEX, cc->nCharPos, (LPARAM)&ciCharIndex);

  if (AEC_IndexCompare(&ciCharIndex, &cr.ciMin) >= 0 &&
      AEC_IndexCompare(&ciCharIndex, &cr.ciMax) < 0)
  {
    cc->crText=lpFrame->aec.crSelText;
    cc->crBk=lpFrame->aec.crSelBk;
    return TRUE;
  }
  else if (ciCharIndex.nLine == ciCaretIndex.nLine)
  {
    cc->crText=lpFrame->aec.crActiveLineText;
    cc->crBk=lpFrame->aec.crActiveLineBk;
  }
  else
  {
    cc->crText=lpFrame->aec.crBasicText;
    cc->crBk=lpFrame->aec.crBasicBk;
  }
  return FALSE;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SetMarker(FRAMEDATA *lpFrame, DWORD dwPos)
{
  if (dwPos == (DWORD)-1)
  {
    if (!lpFrame->dwMappedPrintWidth)
      lpFrame->dwMappedPrintWidth=GetMappedPrintWidth(lpFrame->ei.hWndEdit);
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETMARKER, AEMT_PIXEL, lpFrame->dwMappedPrintWidth);
  }
  else SendMessage(lpFrame->ei.hWndEdit, AEM_SETMARKER, AEMT_SYMBOL, dwPos);
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void SetWordWrap(FRAMEDATA *lpFrame, DWORD dwType, DWORD dwLimit)
{
  if (dwLimit == (DWORD)-1)
  {
    if (!lpFrame->dwMappedPrintWidth)
      lpFrame->dwMappedPrintWidth=GetMappedPrintWidth(lpFrame->ei.hWndEdit);
    SendMessage(lpFrame->ei.hWndEdit, AEM_SETWORDWRAP, dwType|AEWW_LIMITPIXEL, lpFrame->dwMappedPrintWidth);
  }
  else SendMessage(lpFrame->ei.hWndEdit, AEM_SETWORDWRAP, dwType|AEWW_LIMITSYMBOL, dwLimit);
}

void SetMargins(HWND hWnd, DWORD dwNewMargins, DWORD dwOldMargins)
{
  DWORD dwMargins;
  DWORD dwCurMargins;

  dwCurMargins=SendMessage(hWnd, EM_GETMARGINS, 0, 0);
  dwMargins=MAKELONG(LOWORD(dwCurMargins) - LOWORD(dwOldMargins) + LOWORD(dwNewMargins), HIWORD(dwCurMargins) - HIWORD(dwOldMargins) + HIWORD(dwNewMargins));
  SendMessage(hWnd, EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, dwMargins);
}

void SetTabStops(HWND hWnd, int nTabStops, BOOL bSetRedraw)
{
  SendMessage(hWnd, AEM_SETTABSTOP, nTabStops, bSetRedraw);
}

BOOL InsertTabStop(HWND hWnd)
{
  wchar_t wszSpaces[MAX_PATH];
  int nSpaces;
  int i;

  if (IsReadOnly(hWnd)) return FALSE;

  if (lpFrameCurrent->bTabStopAsSpaces)
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
    nSpaces=lpFrameCurrent->nTabStopSize - (crSel.ciMin.nCharInLine - i) % lpFrameCurrent->nTabStopSize;
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

BOOL IndentTabStop(HWND hWnd, int nAction)
{
  wchar_t wszSpaces[MAX_PATH];
  int nSpaces;
  int i;

  if (IsReadOnly(hWnd)) return FALSE;

  if (lpFrameCurrent->bTabStopAsSpaces)
  {
    nSpaces=min(lpFrameCurrent->nTabStopSize, MAX_PATH - 1);

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

  if (IsReadOnly(hWnd)) return TRUE;

  if (!(moCur.dwStatusPosType & SPT_LINEWRAP) && lpFrameCurrent->ei.bWordWrap)
    SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEBEGIN, (LPARAM)&ciChar);

  //Calculate spaces
  for (ciChar.nCharInLine=0; AEC_IndexCompare(&ciChar, &cr->ciMin) < 0 &&
                             (ciChar.lpLine->wpLine[ciChar.nCharInLine] == ' ' ||
                              ciChar.lpLine->wpLine[ciChar.nCharInLine] == '\t'); ++ciChar.nCharInLine);

  if (ciChar.nCharInLine)
  {
    //Insert spaces
    if (wpText=AllocWideStr(ciChar.nCharInLine + 2))
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
      FreeWideStr(wpText);
      return TRUE;
    }
  }
  return FALSE;
}

int SetUrlPrefixes(HWND hWnd, const wchar_t *wpPrefixes)
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

BOOL IsReadOnly(HWND hWnd)
{
  FRAMEDATA *lpFrame;

  if (lpFrame=GetFrameDataFromEditWindow(hWnd))
    if (lpFrame->ei.bReadOnly)
      return TRUE;
  return FALSE;
}

int IsFile(const wchar_t *wpFile)
{
  DWORD dwAttr;

  dwAttr=GetFileAttributesWide(wpFile);
  if (dwAttr == INVALID_FILE_ATTRIBUTES)
    return ERROR_INVALID_HANDLE;
  if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
    return ERROR_FILE_NOT_FOUND;
  return ERROR_SUCCESS;
}

BOOL IsPathFull(const wchar_t *wpPath)
{
  if (wpPath[0] == '\\' && wpPath[1] == '\\') return TRUE;
  if (wpPath[0] != '\0' && wpPath[1] == ':') return TRUE;
  return FALSE;
}

int GetExeDir(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen)
{
  if (nLen=GetModuleFileNameWide(hInstance, wszExeDir, nLen))
  {
    while (nLen > 0 && wszExeDir[nLen] != '\\') --nLen;
    wszExeDir[nLen]='\0';
  }
  return nLen;
}

int GetFileDir(const wchar_t *wpFile, wchar_t *wszFileDir, int nFileDirLen)
{
  int i;

  if (!nFileDirLen) return 0;
  wszFileDir[0]='\0';

  for (i=lstrlenW(wpFile) - 1; i > 0; --i)
  {
    if (wpFile[i] == '\\')
    {
      i=min(nFileDirLen, i + 1);
      xstrcpynW(wszFileDir, wpFile, i);
      return (i - 1);
    }
  }
  return 0;
}

BOOL GetFullName(const wchar_t *wpFile, wchar_t *wszFileFullName, int nFileMax)
{
  wchar_t wszFileBuf[MAX_PATH];
  wchar_t *wpFileName;

  if (GetFullPathNameWide(wpFile, MAX_PATH, wszFileBuf, &wpFileName))
  {
    if (FileExistsWide(wszFileBuf))
    {
      GetLongPathNameWide(wszFileBuf, wszFileFullName, nFileMax);
      return TRUE;
    }
    xstrcpynW(wszFileFullName, wszFileBuf, nFileMax);
  }
  return FALSE;
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

int GetBaseName(const wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen)
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
  xstrcpynW(wszBaseName, wpFile + i, nBaseNameMaxLen);

  return nBaseNameMaxLen;
}

const wchar_t* GetFileExt(const wchar_t *wpFile)
{
  int i;

  for (i=lstrlenW(wpFile) - 1; i >= 0; --i)
  {
    if (wpFile[i] == '.') return (wchar_t *)(wpFile + i + 1);
    else if (wpFile[i] == '\\') break;
  }
  return NULL;
}

void TrimModifyState(wchar_t *wszFile)
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

BOOL GetFileWriteTimeWide(const wchar_t *wpFile, FILETIME *ft)
{
  HANDLE hFile;

  if ((hFile=CreateFileWide(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    return FALSE;

  GetFileTime(hFile, NULL, NULL, ft);
  CloseHandle(hFile);
  return TRUE;
}

BOOL GetFileVersionA(char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage)
{
  struct LANGANDCODEPAGE {
    WORD wLanguage;
    WORD wCodePage;
  } *lpTranslate;

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
      if (GetFileVersionInfoA(pFile, dwHandle, dwVerSize, pVerBuf))
      {
        if (VerQueryValueA(pVerBuf, "\\", (void **)&pffi, &uLen))
        {
          *nMajor=pffi->dwFileVersionMS / 0x00010000;
          *nMinor=pffi->dwFileVersionMS & 0x0000FFFF;
          *nRelease=pffi->dwFileVersionLS / 0x00010000;
          *nBuild=pffi->dwFileVersionLS & 0x0000FFFF;
          bResult=TRUE;
        }
        if (dwLanguage)
        {
          if (VerQueryValueA(pVerBuf, "\\VarFileInfo\\Translation", (void **)&lpTranslate, &uLen))
          {
            *dwLanguage=lpTranslate->wLanguage;
          }
        }
      }
      API_HeapFree(hHeap, 0, pVerBuf);
    }
  }
  return bResult;
}

BOOL GetFileVersionW(wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage)
{
  struct LANGANDCODEPAGE {
    WORD wLanguage;
    WORD wCodePage;
  } *lpTranslate;

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
      if (GetFileVersionInfoW(wpFile, dwHandle, dwVerSize, pVerBuf))
      {
        if (VerQueryValueW(pVerBuf, L"\\", (void **)&pffi, &uLen))
        {
          *nMajor=pffi->dwFileVersionMS / 0x00010000;
          *nMinor=pffi->dwFileVersionMS & 0x0000FFFF;
          *nRelease=pffi->dwFileVersionLS / 0x00010000;
          *nBuild=pffi->dwFileVersionLS & 0x0000FFFF;
          bResult=TRUE;
        }
        if (dwLanguage)
        {
          if (VerQueryValueW(pVerBuf, L"\\VarFileInfo\\Translation", (void **)&lpTranslate, &uLen))
          {
            *dwLanguage=lpTranslate->wLanguage;
          }
        }
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

int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%f -file, %d -file directory, %a -AkelPad directory, %% -%
  wchar_t wszFileDir[MAX_PATH];
  wchar_t *wpFile=lpFrameCurrent->wszFile;
  wchar_t *wpExeDir=wszExeDir;
  wchar_t *wszExpanded;
  int nFileLen;
  int nFileDirLen;
  int nExeDirLen;
  int nEnvironmentLen;
  int a;
  int b;

  nFileLen=lstrlenW(wpFile);
  nFileDirLen=GetFileDir(wpFile, wszFileDir, MAX_PATH);
  nExeDirLen=lstrlenW(wpExeDir);
  nEnvironmentLen=ExpandEnvironmentStringsWide(wpString, NULL, 0);

  if (wszExpanded=(wchar_t *)GlobalAlloc(GPTR, nEnvironmentLen * sizeof(wchar_t)))
  {
    //Expand environment strings
    ExpandEnvironmentStringsWide(wpString, wszExpanded, nEnvironmentLen);
    wpString=wszExpanded;
  }
  else return 0;

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
          if (wszBuffer) xstrcpyW(wszBuffer + b, wpFile);
          b+=nFileLen - 1;
        }
        else break;
      }
      else if (wpString[a] == 'd' || wpString[a] == 'D')
      {
        if (!wszBuffer || b + nFileDirLen <= nBufferSize)
        {
          if (wszBuffer) xstrcpyW(wszBuffer + b, wszFileDir);
          b+=nFileDirLen - 1;
        }
        else break;
      }
      else if (wpString[a] == 'a' || wpString[a] == 'A')
      {
        if (!wszBuffer || b + nExeDirLen <= nBufferSize)
        {
          if (wszBuffer) xstrcpyW(wszBuffer + b, wpExeDir);
          b+=nExeDirLen - 1;
        }
        else break;
      }
      else break;
    }
    else if (wszBuffer) wszBuffer[b]=wpString[a];
  }
  if (wszBuffer && b < nBufferSize) wszBuffer[b]='\0';

  GlobalFree((HGLOBAL)wszExpanded);
  return b;
}

void SetMouseCapture(HWND hWnd, DWORD dwType)
{
  if (!dwMouseCapture)
    SetCapture(hWnd);
  dwMouseCapture|=dwType;
}

void ReleaseMouseCapture(DWORD dwType)
{
  if (dwMouseCapture & dwType)
  {
    dwMouseCapture&=~dwType;
    if (!dwMouseCapture)
      ReleaseCapture();
  }
}

void ActivateKeyboard(DWORD dwInputLocale)
{
  DWORD dwLangId=LOWORD(dwInputLocale);

  if (dwInputLocale != (DWORD)-1)
  {
    if (bWindowsNT)
    {
      DWORD dwLangIdInit=LOWORD(GetKeyboardLayout(0));
      DWORD dwLangIdCount=dwLangIdInit;

      while (dwLangIdCount != dwLangId)
      {
        ActivateKeyboardLayout((HKL)HKL_NEXT, 0);
        dwLangIdCount=LOWORD(GetKeyboardLayout(0));
        if (dwLangIdCount == dwLangIdInit) break;
      }
    }
    else
    {
      if (LOWORD(GetKeyboardLayout(0)) != dwLangId)
        ActivateKeyboardLayout((HKL)dwLangId, 0);
    }
  }
}

void ActivateWindow(HWND hWnd)
{
  DWORD dwStyle=GetWindowLongWide(hWnd, GWL_STYLE);

  if (dwStyle & WS_VISIBLE)
  {
    if (dwStyle & WS_MINIMIZE)
      ShowWindow(hWnd, SW_RESTORE);
    else
      SetForegroundWindow(hWnd); //BringWindowToTop(hWnd);

    SendMessage(hWnd, WM_COMMAND, IDM_INTERNAL_UPDATEMAINCHILDREN, 0);
  }
  SendMessage(hWnd, AKDN_ACTIVATE, 0, 0);
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
  HWND lpClones[]={lpFrameCurrent->ei.hWndMaster, lpFrameCurrent->ei.hWndClone1, lpFrameCurrent->ei.hWndClone2, lpFrameCurrent->ei.hWndClone3};
  int nCloneCount=sizeof(lpClones) / sizeof(HWND);
  int i;

  if (lpFrameCurrent->ei.hWndMaster && lpFrameCurrent->ei.hWndEdit)
  {
    for (i=0; i < nCloneCount; ++i)
    {
      if (lpFrameCurrent->ei.hWndEdit == lpClones[i])
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

void UpdateSize()
{
  int nTabHeight;
  int nEditHeight;

  if (!hDocksStack.bSizing)
  {
    hDocksStack.bSizing=TRUE;

    GetClientRect(hMainWnd, &nsSize.rcInitial);
    if (moCur.bStatusBar && IsWindowVisible(hStatus))
      nsSize.rcInitial.bottom-=nStatusHeight;
    nsSize.rcCurrent=nsSize.rcInitial;
    SendMessage(hMainWnd, AKDN_SIZE, 0, (LPARAM)&nsSize);

    //Docks
    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_LEFT)?DKS_RIGHT:DKS_LEFT, &nsSize);
    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_LEFT)?DKS_LEFT:DKS_RIGHT, &nsSize);
    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_TOP)?DKS_BOTTOM:DKS_TOP, &nsSize);
    StackDockSize(&hDocksStack, (hDocksStack.nSizingSide == DKS_TOP)?DKS_TOP:DKS_BOTTOM, &nsSize);
    hDocksStack.nSizingSide=0;

    //Edits
    if (!nMDI || (moCur.dwTabOptionsMDI & TAB_VIEW_NONE) || !IsWindowVisible(hTab))
      nTabHeight=0;
    else
      nTabHeight=TAB_HEIGHT;
    nEditHeight=nsSize.rcCurrent.bottom - nTabHeight;

    if (nMDI)
    {
      if (nTabHeight && ((moCur.dwTabOptionsMDI & TAB_VIEW_TOP) || (moCur.dwTabOptionsMDI & TAB_VIEW_BOTTOM)))
      {
        MoveWindow(hTab, nsSize.rcCurrent.left, nsSize.rcCurrent.top + ((moCur.dwTabOptionsMDI & TAB_VIEW_BOTTOM)?nEditHeight:0), nsSize.rcCurrent.right, nTabHeight, FALSE);
        InvalidateRect(hTab, NULL, TRUE);
      }
      if (nMDI == WMD_MDI)
        MoveWindow(hMdiClient, nsSize.rcCurrent.left, nsSize.rcCurrent.top + ((moCur.dwTabOptionsMDI & TAB_VIEW_TOP)?nTabHeight:0), nsSize.rcCurrent.right, nEditHeight, TRUE);
    }
    if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
    {
      fdInit.rcEditWindow.left=nsSize.rcCurrent.left;
      fdInit.rcEditWindow.top=nsSize.rcCurrent.top + ((moCur.dwTabOptionsMDI & TAB_VIEW_TOP)?nTabHeight:0);
      fdInit.rcEditWindow.right=nsSize.rcCurrent.right;
      fdInit.rcEditWindow.bottom=nEditHeight;
      ResizeEditWindow(lpFrameCurrent, 0);
    }
    hDocksStack.bSizing=FALSE;
  }
}

BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcInit, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG)
  {
    RECT rcTemplate;
    RECT rcControl;
    int i;

    GetWindowPos(hDlg, NULL, rcInit);
    rcTemplate=*rcCurrent;
    *rcCurrent=*rcInit;

    for (i=0; drs[i].lpWnd; ++i)
    {
      if (*drs[i].lpWnd)
      {
        GetWindowPos(*drs[i].lpWnd, hDlg, &rcControl);
        if (drs[i].dwType & DRS_SIZE)
        {
          if (drs[i].dwType & DRS_X)
            drs[i].nOffset=rcInit->right - (rcControl.left + rcControl.right);
          else if (drs[i].dwType & DRS_Y)
            drs[i].nOffset=rcInit->bottom - (rcControl.top + rcControl.bottom);
        }
        else if (drs[i].dwType & DRS_MOVE)
        {
          if (drs[i].dwType & DRS_X)
            drs[i].nOffset=rcInit->right - rcControl.left;
          else if (drs[i].dwType & DRS_Y)
            drs[i].nOffset=rcInit->bottom - rcControl.top;
        }
      }
    }

    if (rcTemplate.right && rcTemplate.bottom)
    {
      rcTemplate.left=rcInit->left + (rcInit->right - rcTemplate.right) / 2;
      rcTemplate.top=rcInit->top + (rcInit->bottom - rcTemplate.bottom) / 2;
      SetWindowPos(hDlg, 0, rcTemplate.left, rcTemplate.top, rcTemplate.right, rcTemplate.bottom, SWP_NOZORDER);
    }
  }
  else if (uMsg == WM_GETMINMAXINFO)
  {
    if (dwFlags & DRM_GETMINMAXINFO)
    {
      MINMAXINFO *mmi=(MINMAXINFO *)lParam;

      mmi->ptMinTrackSize.x=rcInit->right;
      mmi->ptMinTrackSize.y=rcInit->bottom;
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      RECT rcControl;
      DWORD dwFlags;
      int i;

      GetWindowPos(hDlg, NULL, rcCurrent);

      for (i=0; drs[i].lpWnd; ++i)
      {
        if (*drs[i].lpWnd)
        {
          dwFlags=0;
          if (drs[i].dwType & DRS_SIZE)
            dwFlags|=SWP_NOMOVE;
          else if (drs[i].dwType & DRS_MOVE)
            dwFlags|=SWP_NOSIZE;
          else
            continue;

          GetWindowPos(*drs[i].lpWnd, hDlg, &rcControl);
          SetWindowPos(*drs[i].lpWnd, 0, (drs[i].dwType & DRS_X)?(rcCurrent->right - drs[i].nOffset):rcControl.left,
                                         (drs[i].dwType & DRS_Y)?(rcCurrent->bottom - drs[i].nOffset):rcControl.top,
                                         (drs[i].dwType & DRS_X)?(rcCurrent->right - rcControl.left - drs[i].nOffset):rcControl.right,
                                         (drs[i].dwType & DRS_Y)?(rcCurrent->bottom - rcControl.top - drs[i].nOffset):rcControl.bottom,
                                          dwFlags|SWP_NOZORDER);
        }
      }
      InvalidateRect(hDlg, NULL, TRUE);
      return TRUE;
    }
  }
  else if (uMsg == WM_PAINT)
  {
    if (dwFlags & DRM_PAINTSIZEGRIP)
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
    PatBlt(hDC, rcClient.left, rcClient.bottom - DOCK_BORDER_1X, rcClient.right - rcClient.left - DOCK_BORDER_1X, DOCK_BORDER_1X, PATINVERT);

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

BOOL IsCursorOnWindow(HWND hWnd)
{
  RECT rc;
  POINT pt;

  GetCursorPos(&pt);
  GetWindowRect(hWnd, &rc);
  return PtInRect(&rc, pt);
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

BOOL EnsureWindowInMonitor(RECT *rcWindow)
{
  MONITORINFO mi={0};
  HMONITOR hMonitor=NULL;
  RECT rcNewWindow;

  if ((DWORD)rcWindow->left == CW_USEDEFAULT)
    return TRUE;

  //Size of the work area on the primary display monitor
  SystemParametersInfo(SPI_GETWORKAREA, 0, &mi.rcWork, 0);

  //Not in primary monitor
  if (!PtInRect(&mi.rcWork, *(POINT *)&rcWindow->left))
  {
    //Get monitor handle
    if (MonitorFromPointPtr)
      hMonitor=(*MonitorFromPointPtr)(*(POINT *)&rcWindow->left, MONITOR_DEFAULTTONEAREST);

    //Get monitor info
    mi.cbSize=sizeof(MONITORINFO);

    if (GetMonitorInfoAPtr)
      (*GetMonitorInfoAPtr)(hMonitor, &mi);
  }
  else return TRUE;

  //Not in any monitor
  if (!PtInRect(&mi.rcWork, *(POINT *)&rcWindow->left))
  {
    //Make corrections
    if (rcWindow->left >= mi.rcWork.left && rcWindow->left < mi.rcWork.right)
      rcNewWindow.left=rcWindow->left;
    else
      rcNewWindow.left=mi.rcWork.left;
    if (rcWindow->top >= mi.rcWork.top && rcWindow->top < mi.rcWork.bottom)
      rcNewWindow.top=rcWindow->top;
    else
      rcNewWindow.top=mi.rcWork.top;
    rcNewWindow.right=mi.rcWork.left + (rcWindow->right - rcWindow->left);
    rcNewWindow.bottom=mi.rcWork.top + (rcWindow->bottom - rcWindow->top);
    *rcWindow=rcNewWindow;
    return FALSE;
  }
  else return TRUE;
}

//For WMD_PMDI required: lpFrame == lpFrameCurrent
void UpdateTitle(FRAMEDATA *lpFrame, const wchar_t *wszFile)
{
  const wchar_t *wpFileName;
  int nFileLen=lstrlenW(wszFile);

  //Get file name without path
  wpFileName=GetFileName(wszFile);

  if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
  {
    if (nMDI == WMD_SDI)
    {
      //Set caption of synchronizing window
      SetWindowTextWide(hDummyWindow, wszFile);
    }

    //Set caption of main window
    if (*wpFileName)
    {
      if (nMDI == WMD_SDI)
      {
        xprintfW(wbuf, L"%s - %s", wpFileName, APP_MAIN_TITLEW);
        SetWindowTextWide(hMainWnd, wbuf);
      }
      else if (nMDI == WMD_PMDI)
      {
        xprintfW(wbuf, L"%s - [%s]", APP_MAIN_TITLEW, wszFile);
        SetWindowTextWide(hMainWnd, wbuf);
      }
    }
    else SetWindowTextWide(hMainWnd, APP_MAIN_TITLEW);
  }

  if (nMDI == WMD_MDI || nMDI == WMD_PMDI)
  {
    ASSOCICON *ai;
    TCITEMW tcItem={0};
    const wchar_t *wpExt;
    HICON hIcon=NULL;
    int nItem;

    if (nMDI == WMD_MDI)
    {
      //Set caption of frame window
      SetWindowTextWide(lpFrame->hWndEditParent, wszFile);
    }

    //Find file icon
    if (wpExt=GetFileExt(wpFileName))
    {
      if (!(ai=StackIconGet(&hIconsStack, wszFile, nFileLen, wpExt)))
        ai=StackIconInsert(&hIconsStack, wszFile, nFileLen);
      hIcon=ai->hIcon;
    }
    if (!hIcon) hIcon=hIconEmpty;

    if ((nItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
    {
      wchar_t wszTabName[MAX_PATH];

      //Replace "&" with "&&"
      FixAmpW(wpFileName, wszTabName, MAX_PATH);

      //Set tab icon
      tcItem.mask=TCIF_IMAGE;
      TabCtrl_GetItemWide(hTab, nItem, &tcItem);
      ImageList_ReplaceIcon(hImageList, tcItem.iImage, hIcon);

      //Set tab text
      tcItem.mask=TCIF_TEXT;
      tcItem.pszText=wszTabName;
      TabCtrl_SetItemWide(hTab, nItem, &tcItem);
    }

    //Set frame info
    xstrcpynW(lpFrame->wszFile, wszFile, MAX_PATH);
    lpFrame->nFileLen=lstrlenW(lpFrame->wszFile);
    WideCharToMultiByte(CP_ACP, 0, lpFrame->wszFile, lpFrame->nFileLen + 1, lpFrame->szFile, MAX_PATH, NULL, NULL);
    lpFrame->hIcon=hIcon;

    if (nMDI == WMD_MDI)
    {
      SendMessage(lpFrame->hWndEditParent, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
      DrawMenuBar(hMainWnd);
    }
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

int AddTabItem(HWND hWnd, HICON hIcon, LPARAM lParam)
{
  TCITEMW tcItem={0};
  int nItemCount;
  int nResult=-1;

  if (hWnd)
  {
    nItemCount=SendMessage(hWnd, TCM_GETITEMCOUNT, 0, 0);
    tcItem.mask=TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM;
    tcItem.pszText=L"";
    tcItem.iImage=ImageList_AddIcon(hImageList, hIcon);
    tcItem.lParam=lParam;
    nResult=TabCtrl_InsertItemWide(hWnd, nItemCount, &tcItem);

    if (!nItemCount) InvalidateRect(hWnd, NULL, TRUE);
  }
  return nResult;
}

LPARAM GetTabParamFromItem(HWND hWnd, int nItem)
{
  TCITEMW tcItem;

  tcItem.mask=TCIF_PARAM;
  if (TabCtrl_GetItemWide(hWnd, nItem, &tcItem))
    return tcItem.lParam;
  return 0;
}

int GetTabItemFromParam(HWND hWnd, LPARAM lParam)
{
  TCITEMW tcItem;
  int nCurSel;
  int i;

  nCurSel=SendMessage(hWnd, TCM_GETCURSEL, 0, 0);
  tcItem.mask=TCIF_PARAM;
  TabCtrl_GetItemWide(hWnd, nCurSel, &tcItem);
  if (tcItem.lParam == lParam)
    return nCurSel;

  for (i=0; TabCtrl_GetItemWide(hWnd, i, &tcItem); ++i)
  {
    if (i != nCurSel)
    {
      if (tcItem.lParam == lParam)
        return i;
    }
  }
  return -1;
}

int GetTabItemFromCursorPos(HWND hWnd)
{
  TCHITTESTINFO thti;

  GetCursorPos(&thti.pt);
  ScreenToClient(hWnd, &thti.pt);
  return SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti);
}

int GetTabItemForDrop(HWND hWnd, POINT *pt)
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

int MoveTabItem(HWND hWnd, int nIndexOld, int nIndexNew)
{
  TCITEMW tcItem={0};
  wchar_t wszItemText[MAX_PATH];

  tcItem.mask=TCIF_TEXT|TCIF_IMAGE|TCIF_PARAM;
  tcItem.pszText=wszItemText;
  tcItem.cchTextMax=MAX_PATH;

  if (TabCtrl_GetItemWide(hWnd, nIndexOld, &tcItem))
  {
    if (nIndexOld == nIndexNew) return nIndexNew;
    SendMessage(hWnd, TCM_DELETEITEM, nIndexOld, 0);
    return TabCtrl_InsertItemWide(hWnd, nIndexNew, &tcItem);
  }
  return -1;
}

BOOL DeleteTabItem(HWND hWnd, int nIndex)
{
  TCITEMW tcItem;

  tcItem.mask=TCIF_IMAGE;
  if (TabCtrl_GetItemWide(hWnd, nIndex, &tcItem))
  {
    SendMessage(hWnd, TCM_REMOVEIMAGE, tcItem.iImage, 0);
    SendMessage(hWnd, TCM_DELETEITEM, nIndex, 0);
    return TRUE;
  }
  return FALSE;
}

void FreeMemorySearch()
{
  if (wszFindText_orig)
  {
    if (wszFindText_orig == wszFindText)
      wszFindText=NULL;
    FreeWideStr(wszFindText_orig);
    wszFindText_orig=NULL;
  }
  if (wszReplaceText_orig)
  {
    if (wszReplaceText_orig == wszReplaceText)
      wszReplaceText=NULL;
    FreeWideStr(wszReplaceText_orig);
    wszReplaceText_orig=NULL;
  }

  if (wszFindText)
  {
    FreeWideStr(wszFindText);
    wszFindText=NULL;
  }
  if (wszReplaceText)
  {
    FreeWideStr(wszReplaceText);
    wszReplaceText=NULL;
  }
}

int BytesInString(const wchar_t *wpString)
{
  return (lstrlenW(wpString) + 1) * sizeof(wchar_t);
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
    while (*s != '\0' && *s != (char)c)
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

  if (!(nResult=LoadStringWide(hLoadInstance, uID, lpBuffer, nBufferMax)))
    if (hLoadInstance != hInstance)
      if (!(nResult=LoadStringWide(hInstance, uID, lpBuffer, nBufferMax)))
        lpBuffer[0]='\0';

  return nResult;
}

int API_MessageBox(HWND hWnd, const wchar_t *lpText, const wchar_t *lpCaption, UINT uType)
{
  int nResult;

  bMessageBox=TRUE;
  nResult=MessageBoxW(hWnd, lpText, lpCaption, uType);
  bMessageBox=FALSE;
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

HANDLE API_CreateFileA(const char *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  HANDLE hResult;

  if ((hResult=CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)) == INVALID_HANDLE_VALUE)
  {
    LoadStringWide(hLangLib, MSG_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
    xprintfW(wbuf2, wbuf, lpFileName);
    API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }

  return hResult;
}

HANDLE API_CreateFileW(const wchar_t *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
  HANDLE hResult;

  if ((hResult=CreateFileWide(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)) == INVALID_HANDLE_VALUE)
  {
    LoadStringWide(hLangLib, MSG_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
    xprintfW(wbuf2, wbuf, lpFileName);
    API_MessageBox(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }

  return hResult;
}

BOOL API_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
  BOOL bResult;

  if (!(bResult=ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped)))
  {
    //...
  }

  return bResult;
}

BOOL API_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
  BOOL bResult;

  if (!(bResult=WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped)))
  {
    LoadStringWide(hLangLib, MSG_ERROR_IO, wbuf, BUFFER_SIZE);
    API_MessageBox(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }

  return bResult;
}

LPVOID API_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
  LPVOID lpResult;

  if (!(lpResult=HeapAlloc(hHeap, dwFlags, dwBytes)))
  {
    LoadStringWide(hLangLib, MSG_ERROR_NOT_ENOUGH_MEMORY, wbuf, BUFFER_SIZE);
    API_MessageBox(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }

  return lpResult;
}

BOOL API_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
  BOOL bResult;

  bResult=HeapFree(hHeap, dwFlags, lpMem);

  return bResult;
}

wchar_t* AllocWideStr(DWORD dwSize)
{
  return (wchar_t *)API_HeapAlloc(hHeap, 0, dwSize * sizeof(wchar_t));
}

BOOL FreeWideStr(wchar_t *wpVar)
{
  if (wpVar)
    return API_HeapFree(hHeap, 0, (LPVOID)wpVar);
  return FALSE;
}
