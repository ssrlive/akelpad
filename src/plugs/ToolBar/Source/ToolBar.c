#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "MethodFunc.h"
#include "IconMenu.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include stack functions
#define StackGetElement
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackMoveBefore
#define StackDelete
#define StackClear
#define StackSize
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemset
#define xmemcmp
#define xstrcmpW
#define xstrcmpiW
#define xstrcmpnW
#define xstrlenW
#define xstrcpyW
#define xstrcpynW
#define xatoiW
#define xitoaA
#define xitoaW
#define xuitoaW
#define dec2hexW
#define hex2decW
#define xprintfW
#define xstrstrW
#include "StrFunc.h"

//Include wide functions
#define CallWindowProcWide
#define CreateDialogWide
#define CreateProcessWide
#define CreateWindowExWide
#define DefWindowProcWide
#define DispatchMessageWide
#define ExpandEnvironmentStringsWide
#define ExtractIconExWide
#define GetMenuStringWide
#define GetMessageWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define IconExtractWide
#define InsertMenuWide
#define IsDialogMessageWide
#define LoadImageWide
#define LoadLibraryExWide
#define RegisterClassWide
#define SearchPathWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define TranslateAcceleratorWide
#define UnregisterClassWide
#include "WideFunc.h"
//*/

//Include method functions
#define ALLMETHODFUNC
#include "MethodFunc.h"

//Include icon menu functions
#define ICONMENU_INCLUDE
#include "IconMenu.h"

//Defines
#define DLLA_TOOLBAR_ROWS      1

#define STRID_SETUP                           1
#define STRID_AUTOLOAD                        2
#define STRID_BIGICONS                        3
#define STRID_FLATBUTTONS                     4
#define STRID_16BIT                           5
#define STRID_32BIT                           6
#define STRID_SIDE                            7
#define STRID_ROWS                            8
#define STRID_PARSEMSG_UNKNOWNSPECIAL         9
#define STRID_PARSEMSG_UNKNOWNMETHOD          10
#define STRID_PARSEMSG_METHODALREADYDEFINED   11
#define STRID_PARSEMSG_NOMETHOD               12
#define STRID_PARSEMSG_WRONGPARAMCOUNT        13
#define STRID_PARSEMSG_NOOPENSET              14
#define STRID_PARSEMSG_NOCOMMA                15
#define STRID_PARSEMSG_NOCLOSEPARENTHESIS     16
#define STRID_PARSEMSG_NOEOL                  17
#define STRID_IF_NOCOMMA                      18
#define STRID_IF_NOCLOSEPARENTHESIS           19
#define STRID_IF_UNKNOWNOPERATOR              20
#define STRID_IF_UNKNOWNMETHOD                21
#define STRID_IF_CALLERROR                    22
#define STRID_IF_NOFALSE                      23
#define STRID_IF_FOCUSCHANGED                 24
#define STRID_IF_WRONGPARAMCOUNT              25
#define STRID_IF_SCRIPTDENIED                 26
#define STRID_PLUGIN                          27
#define STRID_OK                              28
#define STRID_CANCEL                          29
#define STRID_DEFAULTMENU                     30

#define AKDLL_RECREATE        (WM_USER + 100)
#define AKDLL_REFRESH         (WM_USER + 101)
#define AKDLL_SETUP           (WM_USER + 102)
#define AKDLL_SELTEXT         (WM_USER + 103)

#define OF_LISTTEXT       0x1
#define OF_SETTINGS       0x2
#define OF_RECT           0x4

#define EXTACT_COMMAND    1
#define EXTACT_CALL       2
#define EXTACT_EXEC       3
#define EXTACT_OPENFILE   4
#define EXTACT_SAVEFILE   5
#define EXTACT_FONT       6
#define EXTACT_RECODE     7
#define EXTACT_INSERT     8
#define EXTACT_MENU       9

#define EXTPARAM_CHAR     1
#define EXTPARAM_INT      2

#define FS_NONE            0
#define FS_FONTNORMAL      1
#define FS_FONTBOLD        2
#define FS_FONTITALIC      3
#define FS_FONTBOLDITALIC  4

//CreateToolbarData SET() flags
#define CCMS_NOSDI       0x01
#define CCMS_NOMDI       0x02
#define CCMS_NOPMDI      0x04
#define CCMS_NOFILEEXIST 0x20
#define CCMS_SKIPIF      0x40
#define CCMS_CHECKIF     0x80

#define MAX_TOOLBARTEXT_SIZE  64000

//Toolbar side
#define TBSIDE_LEFT    1
#define TBSIDE_RIGHT   2
#define TBSIDE_TOP     3
#define TBSIDE_BOTTOM  4

//Toolbar side priority
#define TSP_TOPBOTTOM  1
#define TSP_LEFTRIGHT  2

//Icon arrow overlay
#define IAO_WHOLEDROPDOWN   -1 //BTNS_WHOLEDROPDOWN style used.
#define IAO_NONE             0 //Don't draw any arrows.
#define IAO_COPYNORMAL       1 //IDI_ICONARROW1 icon used.
#define IAO_COPYWHITEASMASK  2 //IDI_ICONARROW2 icon used.

//Icon size
#define BIS_ICON16          0 //16x16 icons.
#define BIS_ICON32          1 //32x32 icons.
#define BIS_ICON24          2 //24x24 icons.

//Grayed icons
#define GI_SYSTEM          0 //System drawing.
#define GI_PLUGIN          1 //Plugin drawing.

#define IMENU_EDIT     0x00000001
#define IMENU_CHECKS   0x00000004

//If() method states
#define IFS_NORMAL           0x0
#define IFS_CHECKED          0x1
#define IFS_GRAYED           0x2
#define IFS_DISABLED         0x4 //Only for ContextMenu

//AKD_IFEXPRESSION custom errors
#define IEE_FOCUSCHANGED     7

#define TOOLBARBACKGROUNDA   "ToolbarBG"
#define TOOLBARBACKGROUNDW  L"ToolbarBG"

#define ROWSHOW_UNCHANGE -2
#define ROWSHOW_INVERT   -1
#define ROWSHOW_OFF       0
#define ROWSHOW_ON        1

#ifndef BTNS_WHOLEDROPDOWN
  #define BTNS_WHOLEDROPDOWN 0x0080
#endif
#ifndef TBSTYLE_EX_DOUBLEBUFFER
  #define TBSTYLE_EX_DOUBLEBUFFER 0x00000080
#endif

typedef struct _STATEIF {
  struct _STATEIF *next;
  struct _STATEIF *prev;
  DWORD dwFlags;
  STACKEXTPARAM hParamStack;
  INT_PTR nValue;
  BOOL bCalculated;
  int nStopError;
} STATEIF;

typedef struct {
  STATEIF *first;
  STATEIF *last;
} STACKSTATEIF;

typedef struct _TOOLBARITEM {
  struct _TOOLBARITEM *next;
  struct _TOOLBARITEM *prev;
  BOOL bUpdateItem;
  STATEIF *lpStateIf;
  BOOL bAutoLoad;
  DWORD dwAction;
  int nTextOffset;
  TBBUTTON tbb;
  int nButtonWidth;
  wchar_t wszButtonItem[MAX_PATH];
  STACKEXTPARAM hParamStack;
  STACKEXTPARAM hParamMenuName;
} TOOLBARITEM;

typedef struct {
  TOOLBARITEM *first;
  TOOLBARITEM *last;
  HIMAGELIST hImageList;
  HIMAGELIST hDisabledImageList;
  int nRows;
  int nSepRows;
  STACKSTATEIF hStateIfStack;
} STACKTOOLBAR;

typedef struct _ROWITEM {
  struct _ROWITEM *next;
  struct _ROWITEM *prev;
  int nRow;
  TOOLBARITEM *lpFirstToolbarItem;
} ROWITEM;

typedef struct {
  ROWITEM *first;
  ROWITEM *last;
  int nElements;
} STACKROW;

//ContextMenu::Show external call
typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  unsigned char *pPosX;
  unsigned char *pPosY;
  INT_PTR nMenuIndex;
  unsigned char *pMenuName;
  INT_PTR *lpnMenuHeight;
} DLLEXTCONTEXTMENU;

#define DLLA_CONTEXTMENU_SHOWSUBMENU   1

//Functions prototypes
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewEditDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadProc(LPVOID lpParameter);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK NewMainProcRet(CWPRETSTRUCT *cwprs);
LRESULT CALLBACK ToolbarBGProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewToolbarProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK PaintTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

BOOL CreateToolbarWindow();
BOOL CreateToolbarData(STACKTOOLBAR *hStack, const wchar_t *wpText);
DWORD IsFlagOn(DWORD dwSetFlags, DWORD dwCheckFlags);
int ParseRows(STACKROW *lpRowListStack);
ROWITEM* GetRow(STACKROW *lpRowListStack, int nRow);
TOOLBARITEM* GetFirstToolbarItemOfNextRow(ROWITEM *lpRowItem);
void FreeRows(STACKROW *lpRowListStack);
void FreeToolbarData(STACKTOOLBAR *hStack);
void SetToolbarButtons(STACKTOOLBAR *hStack);
void ClearToolbarButtons();
void UpdateToolbar(STACKTOOLBAR *hStack);
void ViewItemCode(TOOLBARITEM *lpButton);
void CallToolbar(STACKTOOLBAR *hStack, int nItem);
void CallContextMenuShow(TOOLBARITEM *lpButton, int nPosX, int nPosY, INT_PTR *lpnMenuHeight);
int GetMenuPosY(TOOLBARITEM *lpButton, RECT *rcButton);
void DestroyToolbarWindow(BOOL bDestroyBG);
TOOLBARITEM* StackInsertBeforeButton(STACKTOOLBAR *hStack, TOOLBARITEM *lpInsertBefore);
TOOLBARITEM* StackGetButtonByID(STACKTOOLBAR *hStack, int nItemID);
TOOLBARITEM* StackGetButtonByIndex(STACKTOOLBAR *hStack, int nIndex);
void StackFreeButton(STACKTOOLBAR *hStack);

int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote);
BOOL NextLine(const wchar_t **wpText);
BOOL SkipComment(const wchar_t **wpText);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
INT_PTR TranslateEscapeString(HWND hWndEdit, const wchar_t *wpInput, wchar_t *wszOutput, DWORD *lpdwCaret);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);

int GetCurFile(wchar_t *wszFile, int nMaxFile);
INT_PTR GetEditText(HWND hWnd, wchar_t **wpText);
INT_PTR CopyWideStr(const wchar_t *wpSrc, INT_PTR nSrcLen, wchar_t **wppDst);
BOOL FreeWideStr(wchar_t **wppWideStr);
void ShowStandardEditMenu(HWND hWnd, HMENU hMenu, BOOL bMouse);
DWORD ScrollCaret(HWND hWnd);

void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
wchar_t* GetDefaultMenu(int nStringID);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);
BOOL SetExtCallParam(LPARAM lParam, int nIndex, INT_PTR nData);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
char szBuffer[BUFFER_SIZE];
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HANDLE hHeap;
HINSTANCE hInstanceEXE;
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hMdiClient;
HMENU hMainMenu;
HMENU hMenuRecentFiles;
HMENU hPopupEdit;
HICON hMainIcon;
HACCEL hGlobalAccel;
BOOL bOldWindows;
BOOL bNewComctl32;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
char szExeDir[MAX_PATH];
wchar_t wszExeDir[MAX_PATH];
char *szToolBarText=NULL;
wchar_t *wszToolBarText=NULL;
STACKTOOLBAR hStackToolbar={0};
wchar_t wszRowList[MAX_PATH]=L"";
STACKROW hRowListStack={0};
HWND hToolbarBG=NULL;
HWND hToolbar=NULL;
HICON hIconArrowOverlay=NULL;
int nArrowOverlay=IAO_COPYWHITEASMASK;
int nBigIcons=BIS_ICON16;
BOOL bFlatButtons=TRUE;
int nIconsBit=32;
int nGrayedIcons=GI_SYSTEM;
int nToolbarSide=TBSIDE_TOP;
int nSidePriority=TSP_TOPBOTTOM;
SIZE sizeToolbar={0};
SIZE sizeButtons={0};
SIZE sizeIcon={0};
CHARRANGE64 crExtSetSel={0};
UINT_PTR dwPaintTimerId=0;
BOOL bLockRefresh=FALSE;
HWND hWndMainDlg=NULL;
RECT rcMainMinMaxDialog={532, 174, 0, 0};
RECT rcMainCurrentDialog={0};
int nFocusChanged=0;
WNDPROC lpOldToolbarProc=NULL;
WNDPROC lpOldEditDlgProc=NULL;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCRETDATA *NewMainProcRetData=NULL;
WNDPROCRETDATA *NewFrameProcRetData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="ToolBar";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_TOOLBAR_ROWS)
    {
      unsigned char *pRows=NULL;
      wchar_t *wpRows;
      int nCompare;

      if (IsExtCallParamValid(pd->lParam, 2))
        pRows=(unsigned char *)GetExtCallParam(pd->lParam, 2);

      if (pRows)
      {
        if (pd->dwSupport & PDS_STRANSI)
          wpRows=AllocWide((char *)pRows);
        else
          wpRows=(wchar_t *)pRows;
        nCompare=xstrcmpW(wpRows, wszRowList);
        xstrcpynW(wszRowList, wpRows, MAX_PATH);
        if (pd->dwSupport & PDS_STRANSI)
          FreeWide(wpRows);

        if (nCompare)
        {
          dwSaveFlags|=OF_SETTINGS;

          if (bInitMain)
          {
            PostMessage(hToolbarBG, AKDLL_RECREATE, 0, 0);

            //If plugin already loaded, stay in memory and don't change active status
            if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
            return;
          }
        }
      }
    }
  }

  //Is plugin already loaded?
  if (bInitMain)
  {
    UninitMain();

    SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
  }
  else
  {
    InitMain();

    if (!pd->bOnStart)
    {
      CreateToolbarWindow();
      SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
    }

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndToolBarText;
  static HWND hWndBigIconsCheck;
  static HWND hWndFlatButtonsCheck;
  static HWND hWndBit16;
  static HWND hWndBit32;
  static HWND hWndSideLeft;
  static HWND hWndSideTop;
  static HWND hWndSideRight;
  static HWND hWndSideBottom;
  static HWND hWndSideLabel;
  static HWND hWndRowsLabel;
  static HWND hWndRowsEdit;
  static HWND hWndOK;
  static HWND hWndCancel;
  static RESIZEDIALOG rds[]={{&hWndToolBarText,      RDS_SIZE|RDS_X, 0},
                             {&hWndToolBarText,      RDS_SIZE|RDS_Y, 0},
                             {&hWndBigIconsCheck,    RDS_MOVE|RDS_Y, 0},
                             {&hWndFlatButtonsCheck, RDS_MOVE|RDS_Y, 0},
                             {&hWndBit16,            RDS_MOVE|RDS_Y, 0},
                             {&hWndBit32,            RDS_MOVE|RDS_Y, 0},
                             {&hWndSideLeft,         RDS_MOVE|RDS_Y, 0},
                             {&hWndSideTop,          RDS_MOVE|RDS_Y, 0},
                             {&hWndSideRight,        RDS_MOVE|RDS_Y, 0},
                             {&hWndSideBottom,       RDS_MOVE|RDS_Y, 0},
                             {&hWndSideLabel,        RDS_MOVE|RDS_Y, 0},
                             {&hWndRowsLabel,        RDS_MOVE|RDS_Y, 0},
                             {&hWndRowsEdit,         RDS_MOVE|RDS_Y, 0},
                             {&hWndOK,               RDS_MOVE|RDS_X, 0},
                             {&hWndOK,               RDS_MOVE|RDS_Y, 0},
                             {&hWndCancel,           RDS_MOVE|RDS_X, 0},
                             {&hWndCancel,           RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndToolBarText=GetDlgItem(hDlg, IDC_TOOLBARTEXT);
    hWndBigIconsCheck=GetDlgItem(hDlg, IDC_BIGICONS);
    hWndFlatButtonsCheck=GetDlgItem(hDlg, IDC_FLATBUTTONS);
    hWndBit16=GetDlgItem(hDlg, IDC_16BIT);
    hWndBit32=GetDlgItem(hDlg, IDC_32BIT);
    hWndSideLeft=GetDlgItem(hDlg, IDC_SIDELEFT);
    hWndSideTop=GetDlgItem(hDlg, IDC_SIDETOP);
    hWndSideRight=GetDlgItem(hDlg, IDC_SIDERIGHT);
    hWndSideBottom=GetDlgItem(hDlg, IDC_SIDEBOTTOM);
    hWndSideLabel=GetDlgItem(hDlg, IDC_SIDE_LABEL);
    hWndRowsLabel=GetDlgItem(hDlg, IDC_ROWS_LABEL);
    hWndRowsEdit=GetDlgItem(hDlg, IDC_ROWS);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_BIGICONS, GetLangStringW(wLangModule, STRID_BIGICONS));
    SetDlgItemTextWide(hDlg, IDC_FLATBUTTONS, GetLangStringW(wLangModule, STRID_FLATBUTTONS));
    SetDlgItemTextWide(hDlg, IDC_16BIT, GetLangStringW(wLangModule, STRID_16BIT));
    SetDlgItemTextWide(hDlg, IDC_32BIT, GetLangStringW(wLangModule, STRID_32BIT));
    SetDlgItemTextWide(hDlg, IDC_SIDE_LABEL, GetLangStringW(wLangModule, STRID_SIDE));
    SetDlgItemTextWide(hDlg, IDC_ROWS_LABEL, GetLangStringW(wLangModule, STRID_ROWS));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndToolBarText, AEM_SETEVENTMASK, 0, AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_TEXTINSERT|AENM_TEXTDELETE|AENM_POINT);
    SendMessage(hWndToolBarText, EM_SETEVENTMASK, 0, ENM_SELCHANGE|ENM_CHANGE);
    SendMessage(hWndToolBarText, EM_EXLIMITTEXT, 0, MAX_TOOLBARTEXT_SIZE);
    SetWindowTextWide(hWndToolBarText, wszToolBarText);

    if (nBigIcons == BIS_ICON32)
      SendMessage(hWndBigIconsCheck, BM_SETCHECK, BST_CHECKED, 0);
    else if (nBigIcons == BIS_ICON24)
      SendMessage(hWndBigIconsCheck, BM_SETCHECK, BST_INDETERMINATE, 0);
    if (bFlatButtons)
      SendMessage(hWndFlatButtonsCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (nIconsBit == 16)
      SendMessage(hWndBit16, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndBit32, BM_SETCHECK, BST_CHECKED, 0);

    if (nToolbarSide == TBSIDE_LEFT)
      SendMessage(hWndSideLeft, BM_SETCHECK, BST_CHECKED, 0);
    else if (nToolbarSide == TBSIDE_TOP)
      SendMessage(hWndSideTop, BM_SETCHECK, BST_CHECKED, 0);
    else if (nToolbarSide == TBSIDE_RIGHT)
      SendMessage(hWndSideRight, BM_SETCHECK, BST_CHECKED, 0);
    else if (nToolbarSide == TBSIDE_BOTTOM)
      SendMessage(hWndSideBottom, BM_SETCHECK, BST_CHECKED, 0);

    SetWindowTextWide(hWndRowsEdit, wszRowList);

    lpOldEditDlgProc=(WNDPROC)GetWindowLongPtrWide(hWndToolBarText, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndToolBarText, GWLP_WNDPROC, (UINT_PTR)NewEditDlgProc);

    SendMessage(hMainWnd, AKD_SETMODELESS, (WPARAM)hDlg, MLA_ADD);

    //Post AKDLL_SELTEXT because dialog size can be changed after AKD_RESIZEDIALOG
    PostMessage(hDlg, AKDLL_SELTEXT, 0, 0);
  }
  else if (uMsg == AKDLL_SELTEXT)
  {
    if (IsIconic(hDlg))
      ShowWindow(hDlg, SW_RESTORE);

    if (crExtSetSel.cpMin || crExtSetSel.cpMax)
    {
      int nLine;

      if (crExtSetSel.cpMax < crExtSetSel.cpMin)
      {
        nLine=(int)SendMessage(hWndToolBarText, EM_EXLINEFROMCHAR, 0, crExtSetSel.cpMin);
        crExtSetSel.cpMax=SendMessage(hWndToolBarText, EM_LINEINDEX, nLine, 0) + SendMessage(hWndToolBarText, EM_LINELENGTH, crExtSetSel.cpMin, 0);
      }

      SendMessage(hWndToolBarText, AEM_LOCKSCROLL, SB_BOTH, TRUE);
      SendMessage(hWndToolBarText, EM_SETSEL, crExtSetSel.cpMax, crExtSetSel.cpMin);
      SendMessage(hWndToolBarText, AEM_LOCKSCROLL, SB_BOTH, FALSE);
      ScrollCaret(hWndToolBarText);

      crExtSetSel.cpMin=0;
      crExtSetSel.cpMax=0;
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      BOOL bUpdate=FALSE;

      if (LOWORD(wParam) == IDOK)
      {
        STACKTOOLBAR hTestStack;
        wchar_t *wszTest;
        int nValue;

        //Big icons
        if (SendMessage(hWndBigIconsCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
          nValue=BIS_ICON32;
        else if (SendMessage(hWndBigIconsCheck, BM_GETCHECK, 0, 0) == BST_INDETERMINATE)
          nValue=BIS_ICON24;
        else
          nValue=BIS_ICON16;
        if (nValue != nBigIcons)
        {
          nBigIcons=nValue;
          bUpdate=TRUE;
        }

        //Flat buttons
        if (SendMessage(hWndFlatButtonsCheck, BM_GETCHECK, 0, 0) != bFlatButtons)
        {
          bFlatButtons=!bFlatButtons;

          nValue=(int)SendMessage(hToolbar, TB_GETSTYLE, 0, 0);
          if (bFlatButtons)
            SendMessage(hToolbar, TB_SETSTYLE, 0, nValue | TBSTYLE_FLAT);
          else
            SendMessage(hToolbar, TB_SETSTYLE, 0, nValue & ~TBSTYLE_FLAT);

          InvalidateRect(hToolbar, NULL, TRUE);
        }

        //Icons bit
        if (SendMessage(hWndBit16, BM_GETCHECK, 0, 0) == BST_CHECKED)
          nValue=16;
        else
          nValue=32;
        if (nIconsBit != nValue)
        {
          nIconsBit=nValue;
          bUpdate=TRUE;
        }

        //Toolbar side
        if (SendMessage(hWndSideLeft, BM_GETCHECK, 0, 0) == BST_CHECKED)
          nValue=TBSIDE_LEFT;
        else if (SendMessage(hWndSideTop, BM_GETCHECK, 0, 0) == BST_CHECKED)
          nValue=TBSIDE_TOP;
        else if (SendMessage(hWndSideRight, BM_GETCHECK, 0, 0) == BST_CHECKED)
          nValue=TBSIDE_RIGHT;
        else if (SendMessage(hWndSideBottom, BM_GETCHECK, 0, 0) == BST_CHECKED)
          nValue=TBSIDE_BOTTOM;
        if (nToolbarSide != nValue)
        {
          nToolbarSide=nValue;
          bUpdate=TRUE;
        }

        if (SendMessage(hWndRowsEdit, EM_GETMODIFY, 0, 0))
        {
          GetWindowTextWide(hWndRowsEdit, wszRowList, MAX_PATH);
          bUpdate=TRUE;
        }

        if (SendMessage(hWndToolBarText, EM_GETMODIFY, 0, 0))
        {
          GetEditText(hWndToolBarText, &wszTest);

          //Test for errors
          xmemset(&hTestStack, 0, sizeof(STACKTOOLBAR));

          if (!CreateToolbarData(&hTestStack, wszTest))
          {
            FreeToolbarData(&hTestStack);
            FreeWideStr(&wszTest);
            return FALSE;
          }

          //Success
          FreeToolbarData(&hStackToolbar);
          FreeWideStr(&wszToolBarText);
          wszToolBarText=wszTest;
          xmemcpy(&hStackToolbar, &hTestStack, sizeof(STACKTOOLBAR));

          if (!*wszToolBarText)
          {
            FreeToolbarData(&hStackToolbar);
            FreeWideStr(&wszToolBarText);
            wszToolBarText=GetDefaultMenu(STRID_DEFAULTMENU);
            CreateToolbarData(&hStackToolbar, wszToolBarText);
          }
          bUpdate=TRUE;
        }
        dwSaveFlags|=OF_LISTTEXT|OF_SETTINGS;
      }
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      SendMessage(hMainWnd, AKD_SETMODELESS, (WPARAM)hDlg, MLA_DELETE);
      DestroyWindow(hWndMainDlg);
      hWndMainDlg=NULL;
      if (bUpdate)
        PostMessage(hToolbarBG, AKDLL_RECREATE, 0, 0);
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      dwSaveFlags|=OF_RECT;
  }

  return FALSE;
}

LRESULT CALLBACK NewEditDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (uMsg == WM_GETDLGCODE)
  {
    lResult=CallWindowProcWide(lpOldEditDlgProc, hWnd, uMsg, wParam, lParam);

    if (lResult & DLGC_HASSETSEL)
    {
      lResult&=~DLGC_HASSETSEL;
    }
    return lResult;
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    ShowStandardEditMenu((HWND)wParam, hPopupEdit, lParam != -1);
  }

  return CallWindowProcWide(lpOldEditDlgProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKD_IFEXPRESSION)
  {
    LRESULT lResult;

    bLockRefresh=TRUE;
    lResult=NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
    bLockRefresh=FALSE;
    return lResult;
  }
  else if (uMsg == AKDN_MAIN_ONSTART_PRESHOW)
  {
    CreateToolbarWindow();
  }
  else if (uMsg == AKDN_SIZE_ONSTART)
  {
    NSIZE *ns=(NSIZE *)lParam;
    LRESULT lResult=0;
    BOOL bProcess=FALSE;

    if (hToolbarBG && hToolbar && IsWindowVisible(hToolbar))
    {
      if ((nSidePriority == TSP_TOPBOTTOM && (nToolbarSide == TBSIDE_LEFT ||
                                              nToolbarSide == TBSIDE_RIGHT)) ||
          (nSidePriority == TSP_LEFTRIGHT && (nToolbarSide == TBSIDE_TOP ||
                                              nToolbarSide == TBSIDE_BOTTOM)))
      {
        //Give more priority.
        lResult=NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
        bProcess=TRUE;
      }

      if (nToolbarSide == TBSIDE_TOP)
      {
        SetWindowPos(hToolbarBG, 0, ns->rcCurrent.left, ns->rcCurrent.top, ns->rcCurrent.right, sizeToolbar.cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        SetWindowPos(hToolbar, 0, 0, 0, ns->rcCurrent.right, sizeToolbar.cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        ns->rcCurrent.top+=sizeToolbar.cy;
        ns->rcCurrent.bottom-=sizeToolbar.cy;
      }
      else if (nToolbarSide == TBSIDE_BOTTOM)
      {
        SetWindowPos(hToolbarBG, 0, ns->rcCurrent.left, ns->rcCurrent.top + ns->rcCurrent.bottom - sizeToolbar.cy, ns->rcCurrent.right, sizeToolbar.cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        SetWindowPos(hToolbar, 0, 0, 0, ns->rcCurrent.right, sizeToolbar.cy, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        ns->rcCurrent.bottom-=sizeToolbar.cy;
      }
      else if (nToolbarSide == TBSIDE_LEFT)
      {
        SetWindowPos(hToolbarBG, 0, ns->rcCurrent.left, ns->rcCurrent.top, sizeToolbar.cx, ns->rcCurrent.bottom, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        SetWindowPos(hToolbar, 0, 0, 0, sizeToolbar.cx, ns->rcCurrent.bottom, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        ns->rcCurrent.left+=sizeToolbar.cx;
        ns->rcCurrent.right-=sizeToolbar.cx;
      }
      else if (nToolbarSide == TBSIDE_RIGHT)
      {
        SetWindowPos(hToolbarBG, 0, ns->rcCurrent.left + ns->rcCurrent.right - sizeToolbar.cx, ns->rcCurrent.top, sizeToolbar.cx, ns->rcCurrent.bottom, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        SetWindowPos(hToolbar, 0, 0, 0, sizeToolbar.cx, ns->rcCurrent.bottom, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_DEFERERASE);
        ns->rcCurrent.right-=sizeToolbar.cx;
      }
      if (bProcess) return lResult;
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
    if (hWndMainDlg) SendMessage(hWndMainDlg, WM_COMMAND, IDCANCEL, 0);
    UninitMain();
    return FALSE;
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

void CALLBACK NewMainProcRet(CWPRETSTRUCT *cwprs)
{
  if (cwprs->message == AKDN_DLLCALL ||
      cwprs->message == AKDN_DLLUNLOAD ||
      cwprs->message == AKDN_FRAME_NOWINDOWS ||
      (cwprs->message == AKDN_FRAME_ACTIVATE && !(cwprs->wParam & FWA_NOVISUPDATE)))
  {
    if (cwprs->message == AKDN_DLLCALL)
    {
      PLUGINDATA *pd=(PLUGINDATA *)cwprs->lParam;

      if (pd->dwSupport & PDS_GETSUPPORT)
        goto NextProc;
      if (!xstrcmpiW(pd->wszFunction, L"ContextMenu::Show"))
        goto NextProc;
    }
    if (!bLockRefresh)
      PostMessage(hToolbarBG, AKDLL_REFRESH, 0, 0);
  }
  else if (cwprs->message == WM_ACTIVATE)
  {
    if (nFocusChanged == -1)
      nFocusChanged=1;
  }
  else if (cwprs->message == WM_COMMAND)
  {
    if (cwprs->hwnd == hMainWnd)
    {
      if (!bLockRefresh)
        PostMessage(hToolbarBG, AKDLL_REFRESH, 0, 0);
    }
  }
  else if (cwprs->message == WM_NOTIFY)
  {
    if (cwprs->wParam == ID_EDIT)
    {
      if (((NMHDR *)cwprs->lParam)->code == AEN_MODIFY)
      {
        if (!bLockRefresh)
          PostMessage(hToolbarBG, AKDLL_REFRESH, 0, 0);
      }
      else if (((NMHDR *)cwprs->lParam)->code == EN_SELCHANGE)
      {
        if (!bLockRefresh)
          PostMessage(hToolbarBG, AKDLL_REFRESH, 0, 0);
      }
    }
  }

  //Call next procedure
  NextProc:
  if (cwprs->hwnd == hMainWnd)
  {
    if (NewMainProcRetData->NextProc)
      NewMainProcRetData->NextProc(cwprs);
  }
  else
  {
    if (NewFrameProcRetData->NextProc)
      NewFrameProcRetData->NextProc(cwprs);
  }
}

LRESULT CALLBACK ToolbarBGProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDLL_RECREATE)
  {
    DestroyToolbarWindow(FALSE);
    CreateToolbarWindow();
    SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
  }
  else if (uMsg == AKDLL_REFRESH)
  {
    if (hToolbar) UpdateToolbar(&hStackToolbar);
  }
  else if (uMsg == AKDLL_SETUP)
  {
    if (!hWndMainDlg)
      hWndMainDlg=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hMainWnd, (DLGPROC)MainDlgProc);
    else
      SetActiveWindow(hWndMainDlg);
  }
  else if (uMsg == WM_ERASEBKGND)
  {
    return 0;
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == TTN_GETDISPINFOA)
    {
      TOOLBARITEM *lpButton;

      if (lpButton=StackGetButtonByID(&hStackToolbar, (int)((NMHDR *)lParam)->idFrom))
      {
        //WideCharToMultiByte(CP_ACP, 0, lpButton->wszButtonItem, -1, ((NMTTDISPINFOA *)lParam)->szText, 80, NULL, NULL);
        WideCharToMultiByte(CP_ACP, 0, lpButton->wszButtonItem, -1, szBuffer, BUFFER_SIZE, NULL, NULL);
        ((NMTTDISPINFOA *)lParam)->lpszText=szBuffer;
      }
    }
    else if (((NMHDR *)lParam)->code == TTN_GETDISPINFOW)
    {
      TOOLBARITEM *lpButton;

      if (lpButton=StackGetButtonByID(&hStackToolbar, (int)((NMHDR *)lParam)->idFrom))
      {
        //xstrcpynW(((NMTTDISPINFOW *)lParam)->szText, lpButton->wszButtonItem, 80);
        ((NMTTDISPINFOW *)lParam)->lpszText=lpButton->wszButtonItem;
      }
    }
    else if (((NMHDR *)lParam)->code == TBN_DROPDOWN)
    {
      TOOLBARITEM *lpButton;
      RECT rcButton;
      int nPosX;
      int nPosY;

      if (lpButton=StackGetButtonByID(&hStackToolbar, ((NMTOOLBARA *)lParam)->iItem))
      {
        SendMessage(hToolbar, TB_GETRECT, ((NMTOOLBARA *)lParam)->iItem, (LPARAM)&rcButton);
        ClientToScreen(hToolbar, (LPPOINT)&rcButton.left);
        ClientToScreen(hToolbar, (LPPOINT)&rcButton.right);
        nPosX=rcButton.left;
        nPosY=GetMenuPosY(lpButton, &rcButton);

        if (!lpButton->hParamMenuName.first)
        {
          //IDM_FILE_OPEN
          TrackPopupMenu(hMenuRecentFiles, TPM_LEFTBUTTON, nPosX, nPosY, 0, hMainWnd, NULL);
        }
        else
        {
          //ContextMenu::Show
          CallContextMenuShow(lpButton, nPosX, nPosY, NULL);
        }
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if ((HWND)lParam == hToolbar)
    {
      CallToolbar(&hStackToolbar, LOWORD(wParam));
      return TRUE;
    }
  }

  return DefWindowProcWide(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewToolbarProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_PAINT)
  {
    if (bNewComctl32 && hStackToolbar.nSepRows && hRowListStack.nElements != 1)
    {
      //On ComCtl32.dll version 6.10 horizontal separator (TBSTATE_WRAP + TBSTYLE_SEP)
      //erased if window moved outside the screen and then moved back.
      //Use timer for more soft update not often than 100 ms.
      if (!dwPaintTimerId)
      {
        dwPaintTimerId=SetTimer(NULL, 0, 100, (TIMERPROC)PaintTimerProc);
      }
    }
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    POINT pt={LOWORD(lParam), HIWORD(lParam)};

    if (SendMessage(hToolbar, TB_HITTEST, 0, (LPARAM)&pt) < 0)
    {
      PostMessage(hToolbarBG, AKDLL_SETUP, 0, 0);
    }
  }
  else if (uMsg == WM_RBUTTONDOWN)
  {
    //When left and right button pressed, after releasing window is not accept any mouse buttons actions.
    return 0;
  }
  else if (uMsg == WM_RBUTTONUP)
  {
    TOOLBARITEM *lpButton;
    POINT pt;
    int nIndex;

    //Find mouse over item
    GetCursorPos(&pt);
    ScreenToClient(hToolbar, &pt);
    nIndex=(int)SendMessage(hToolbar, TB_HITTEST, 0, (LPARAM)&pt);
    if (nIndex < 0) nIndex=(0 - nIndex) - 1;

    lpButton=StackGetButtonByIndex(&hStackToolbar, nIndex);
    ViewItemCode(lpButton);
    return 0;
  }
  return CallWindowProcWide(lpOldToolbarProc, hWnd, uMsg, wParam, lParam);
}

VOID CALLBACK PaintTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  if (dwPaintTimerId)
  {
    InvalidateRect(hToolbar, NULL, FALSE);
    UpdateWindow(hToolbar);

    KillTimer(NULL, dwPaintTimerId);
    dwPaintTimerId=0;
  }
}

BOOL CreateToolbarWindow()
{
  WNDCLASSW wndclass;
  DWORD dwStyle=0;
  BOOL bResult=TRUE;

  if (!hToolbarBG)
  {
    wndclass.style        =CS_DBLCLKS;
    wndclass.lpfnWndProc  =ToolbarBGProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstanceDLL;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground=(HBRUSH)(UINT_PTR)(COLOR_BTNFACE + 1);
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=TOOLBARBACKGROUNDW;
    if (!RegisterClassWide(&wndclass)) return FALSE;

    hToolbarBG=CreateWindowExWide(0,
                          TOOLBARBACKGROUNDW,
                          NULL,
                          WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,
                          0, 0, 0, 0,
                          hMainWnd,
                          NULL,
                          hInstanceDLL,
                          NULL);
  }

  if (nToolbarSide == TBSIDE_LEFT)
    dwStyle=CCS_VERT|CCS_NODIVIDER|CCS_NOPARENTALIGN|CCS_NORESIZE;
  else if (nToolbarSide == TBSIDE_TOP)
    dwStyle=CCS_TOP|/*CCS_NODIVIDER|*/CCS_NOPARENTALIGN|CCS_NORESIZE;
  else if (nToolbarSide == TBSIDE_RIGHT)
    dwStyle=CCS_VERT|CCS_NODIVIDER|CCS_NOPARENTALIGN|CCS_NORESIZE;
  else if (nToolbarSide == TBSIDE_BOTTOM)
    dwStyle=CCS_BOTTOM|/*CCS_NODIVIDER|*/CCS_NOPARENTALIGN|CCS_NORESIZE;

  hToolbar=CreateWindowExWide(0,
                        L"ToolbarWindow32",
                        NULL,
                        WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS|/*TBSTYLE_TRANSPARENT|TBSTYLE_CUSTOMERASE|*/
                        (bFlatButtons?TBSTYLE_FLAT:0)|dwStyle,
                        0, 0, 0, 0,
                        hToolbarBG,
                        (HMENU)(UINT_PTR)IDC_TOOLBAR,
                        hInstanceDLL,
                        NULL);

  lpOldToolbarProc=(WNDPROC)GetWindowLongPtrWide(hToolbar, GWLP_WNDPROC);
  SetWindowLongPtrWide(hToolbar, GWLP_WNDPROC, (UINT_PTR)NewToolbarProc);

  dwStyle=(DWORD)SendMessage(hToolbar, TB_GETEXTENDEDSTYLE, 0, 0);
  SendMessage(hToolbar, TB_SETEXTENDEDSTYLE, 0, dwStyle|TBSTYLE_EX_DRAWDDARROWS/*|TBSTYLE_EX_DOUBLEBUFFER*/);
  SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

  ParseRows(&hRowListStack);
  if (!CreateToolbarData(&hStackToolbar, wszToolBarText))
    bResult=FALSE;

  SetToolbarButtons(&hStackToolbar);
  UpdateToolbar(&hStackToolbar);
  return bResult;
}

BOOL CreateToolbarData(STACKTOOLBAR *hStack, const wchar_t *wpText)
{
  ROWITEM *lpRowItem=NULL;
  TOOLBARITEM *lpButton;
  TOOLBARITEM *lpLastButton=NULL;
  TOOLBARITEM *lpNextRowItemFirstButton=NULL;
  EXTPARAM *lpParameter;
  STACKEXTPARAM hParamButton={0};
  STACKEXTPARAM hParamMenuName={0};
  STACKEXTPARAM hParamStack={0};
  IFEXPRESSION ie;
  wchar_t wszButtonItem[MAX_PATH];
  wchar_t wszMethodName[MAX_PATH];
  wchar_t wszIconFile[MAX_PATH];
  const wchar_t *wpTextBegin=wpText;
  const wchar_t *wpCount=wpText;
  const wchar_t *wpLineBegin;
  const wchar_t *wpErrorBegin=wpText;
  const wchar_t *wpStrEnd;
  wchar_t *wpFileName;
  HICON hIcon;
  HICON hAdjustIcon;
  DWORD dwAction;
  DWORD dwNewFlags=0;
  DWORD dwSetFlags=0;
  int nFlagCountNoSDI=0;
  int nFlagCountNoMDI=0;
  int nFlagCountNoPMDI=0;
  int nFlagCountNoFileExistAll=0;
  int nFlagCountNoFileExistMore=0;
  int nFlagCountSkipIfAll=0;
  int nFlagCountSkipIfMore=0;
  int nFlagCountStateIf=0;
  STATEIF *lpStateIf=NULL;
  INT_PTR nIfResult;
  DWORD dwStyle;
  int nPlus;
  int nMinus;
  int nFileIconIndex;
  int nItemBitmap=0;
  int nItemCommand=0;
  int nSetCmp;
  int nUnsetCmp;
  int nMessageID;
  BOOL bQuote;
  BOOL bMethod;
  BOOL bPrevSeparator=FALSE;
  BOOL bInRow=TRUE;

  if (wpCount)
  {
    if (nBigIcons == BIS_ICON32)
    {
      sizeIcon.cx=32 /*GetSystemMetrics(SM_CXICON)*/;
      sizeIcon.cy=32 /*GetSystemMetrics(SM_CYICON)*/;
    }
    else if (nBigIcons == BIS_ICON24)
    {
      sizeIcon.cx=24;
      sizeIcon.cy=24;
    }
    else
    {
      sizeIcon.cx=16 /*GetSystemMetrics(SM_CXSMICON)*/;
      sizeIcon.cy=16 /*GetSystemMetrics(SM_CYSMICON)*/;
    }
    hStack->hImageList=ImageList_Create(sizeIcon.cx, sizeIcon.cy, (nIconsBit == 16?ILC_COLOR16:ILC_COLOR32)|ILC_MASK, 0, 0);
    ImageList_SetBkColor(hStack->hImageList, GetSysColor(COLOR_BTNFACE));

    if (nGrayedIcons == GI_PLUGIN)
    {
      hStack->hDisabledImageList=ImageList_Create(sizeIcon.cx, sizeIcon.cy, (nIconsBit == 16?ILC_COLOR16:ILC_COLOR32)|ILC_MASK, 0, 0);
      ImageList_SetBkColor(hStack->hDisabledImageList, GetSysColor(COLOR_BTNFACE));
    }

    //Rows
    hStack->nRows=1;
    hStack->nSepRows=0;

    if (hRowListStack.nElements)
    {
      if (lpRowItem=GetRow(&hRowListStack, hStack->nRows))
      {
        lpNextRowItemFirstButton=GetFirstToolbarItemOfNextRow(lpRowItem);
        bInRow=TRUE;
      }
      else
      {
        lpNextRowItemFirstButton=NULL;
        bInRow=FALSE;
      }
    }

    while (*wpCount)
    {
      bMethod=FALSE;
      lpButton=NULL;
      hIcon=NULL;
      MethodFreeParameters(&hParamMenuName);
      if (!SkipComment(&wpCount)) break;
      wpLineBegin=wpCount;
      wpErrorBegin=wpLineBegin;

      //Set options
      if (!(nSetCmp=xstrcmpnW(L"SET(", wpCount, (UINT_PTR)-1)) ||
          !(nUnsetCmp=xstrcmpnW(L"UNSET(", wpCount, (UINT_PTR)-1)))
      {
        if (!nSetCmp)
        {
          dwNewFlags=(DWORD)xatoiW(wpCount + 4, &wpCount);
          MethodComment(wpCount, &wpCount);

          if (dwNewFlags & CCMS_NOSDI)
            ++nFlagCountNoSDI;
          if (dwNewFlags & CCMS_NOMDI)
            ++nFlagCountNoMDI;
          if (dwNewFlags & CCMS_NOPMDI)
            ++nFlagCountNoPMDI;
          if (dwNewFlags & CCMS_NOFILEEXIST)
          {
            if (*wpCount != L',')
            {
              nMessageID=STRID_PARSEMSG_NOCOMMA;
              goto Error;
            }
            MethodComment(wpCount + 1, &wpCount);

            GetWord(wpCount, wszButtonItem, MAX_PATH, &wpCount, &bQuote);

            if (nFlagCountNoFileExistMore)
              ++nFlagCountNoFileExistMore;
            if (!(dwSetFlags & CCMS_NOFILEEXIST))
            {
              if (*wszButtonItem && bQuote)
              {
                if (TranslateFileString(wszButtonItem, wszBuffer, BUFFER_SIZE))
                {
                  if (SearchPathWide(NULL, wszBuffer, NULL, MAX_PATH, wszButtonItem, &wpFileName))
                    dwNewFlags&=~CCMS_NOFILEEXIST;
                }
              }
              if (dwNewFlags & CCMS_NOFILEEXIST)
                ++nFlagCountNoFileExistMore;
            }
            ++nFlagCountNoFileExistAll;
          }
          if (dwNewFlags & CCMS_SKIPIF)
          {
            if (*wpCount != L',')
            {
              nMessageID=STRID_PARSEMSG_NOCOMMA;
              goto Error;
            }
            MethodComment(wpCount + 1, &wpCount);

            ie.dwFlags=IEF_METHOD;
            nIfResult=SendMessage(hMainWnd, AKD_IFEXPRESSION, (WPARAM)wpCount, (LPARAM)&ie);
            wpCount=ie.wpEnd;
            if (ie.nError)
            {
              nMessageID=(ie.nError - 1) + STRID_IF_NOCOMMA;
              goto Error;
            }

            if (nFlagCountSkipIfMore)
              ++nFlagCountSkipIfMore;
            if (!(dwSetFlags & CCMS_SKIPIF))
            {
              if (nIfResult)
                dwNewFlags&=~CCMS_SKIPIF;
              if (dwNewFlags & CCMS_SKIPIF)
                ++nFlagCountSkipIfMore;
            }
            ++nFlagCountSkipIfAll;
          }
          if (dwNewFlags & CCMS_CHECKIF)
          {
            if (*wpCount != L',')
            {
              nMessageID=STRID_PARSEMSG_NOCOMMA;
              goto Error;
            }
            MethodComment(wpCount + 1, &wpCount);

            ie.dwFlags=IEF_STACKEXTPARAM|IEF_PARSEONLY;
            ie.sep=&hParamStack;
            SendMessage(hMainWnd, AKD_IFEXPRESSION, (WPARAM)wpCount, (LPARAM)&ie);
            wpCount=ie.wpEnd;
            if (!(ie.dwFlags & IEF_IF))
            {
              nMessageID=STRID_IF_UNKNOWNMETHOD;
              goto Error;
            }
            if (ie.nError)
            {
              nMessageID=(ie.nError - 1) + STRID_IF_NOCOMMA;
              goto Error;
            }
            if (hParamStack.nElements != 1 && hParamStack.nElements != 3)
            {
              nMessageID=STRID_IF_WRONGPARAMCOUNT;
              goto Error;
            }
            if (xstrstrW(hParamStack.first->wpString, -1, L"\"Scripts::Main\"", -1, FALSE, NULL, &wpStrEnd) &&
                xstrcmpnW(L", 5", wpStrEnd, (UINT_PTR)-1) && xstrcmpnW(L", 6", wpStrEnd, (UINT_PTR)-1))
            {
              MethodFreeParameters(&hParamStack);
              nMessageID=STRID_IF_SCRIPTDENIED;
              goto Error;
            }
            if (!StackInsertAfter((stack **)&hStack->hStateIfStack.first, (stack **)&hStack->hStateIfStack.last, (stack *)lpStateIf, (stack **)&lpStateIf, sizeof(STATEIF)))
            {
              lpStateIf->dwFlags=IEF_IF;
              lpStateIf->hParamStack=hParamStack;
              xmemset(&hParamStack, 0, sizeof(STACKEXTPARAM));
            }
            ++nFlagCountStateIf;
          }
          dwSetFlags|=dwNewFlags;
        }
        else if (!nUnsetCmp)
        {
          dwNewFlags=(DWORD)xatoiW(wpCount + 6, &wpCount);

          if (dwNewFlags & CCMS_NOSDI)
          {
            if (nFlagCountNoSDI <= 0)
              goto UnsetError;
            if (--nFlagCountNoSDI > 0)
              dwNewFlags&=~CCMS_NOSDI;
          }
          if (dwNewFlags & CCMS_NOMDI)
          {
            if (nFlagCountNoMDI <= 0)
              goto UnsetError;
            if (--nFlagCountNoMDI > 0)
              dwNewFlags&=~CCMS_NOMDI;
          }
          if (dwNewFlags & CCMS_NOPMDI)
          {
            if (nFlagCountNoPMDI <= 0)
              goto UnsetError;
            if (--nFlagCountNoPMDI > 0)
              dwNewFlags&=~CCMS_NOPMDI;
          }
          if (dwNewFlags & CCMS_NOFILEEXIST)
          {
            if (--nFlagCountNoFileExistAll < 0)
              goto UnsetError;
            if (nFlagCountNoFileExistMore && --nFlagCountNoFileExistMore > 0)
              dwNewFlags&=~CCMS_NOFILEEXIST;
          }
          if (dwNewFlags & CCMS_SKIPIF)
          {
            if (--nFlagCountSkipIfAll < 0)
              goto UnsetError;
            if (nFlagCountSkipIfMore && --nFlagCountSkipIfMore > 0)
              dwNewFlags&=~CCMS_SKIPIF;
          }
          if (dwNewFlags & CCMS_CHECKIF)
          {
            if (--nFlagCountStateIf < 0)
              goto UnsetError;
            if (lpStateIf)
              lpStateIf=lpStateIf->prev;
            if (lpStateIf)
              dwNewFlags&=~CCMS_CHECKIF;
          }
          dwSetFlags&=~dwNewFlags;
        }
        MethodComment(wpCount, &wpCount);
        if (*wpCount != L')')
        {
          nMessageID=STRID_PARSEMSG_NOCLOSEPARENTHESIS;
          goto Error;
        }
        MethodComment(wpCount + 1, &wpCount);
        if (*wpCount != L'\r' && *wpCount != L'\0')
        {
          nMessageID=STRID_PARSEMSG_NOEOL;
          goto Error;
        }
        if (!NextLine(&wpCount)) break;
        continue;

        UnsetError:
        nMessageID=STRID_PARSEMSG_NOOPENSET;
        goto Error;
      }
      //Skip line if skip flag set
      if (IsFlagOn(dwSetFlags, CCMS_NOSDI|CCMS_NOMDI|CCMS_NOPMDI|CCMS_NOFILEEXIST|CCMS_SKIPIF))
      {
        if (!NextLine(&wpCount)) break;
        continue;
      }

      if (*wpCount == L'-')
      {
        nMinus=1;
        ++wpCount;
      }
      else nMinus=0;
      GetWord(wpCount, wszButtonItem, MAX_PATH, &wpCount, &bQuote);

      if (!bQuote)
      {
        //Special item
        if (!xstrcmpW(wszButtonItem, L"SEPARATOR") ||
            !xstrcmpW(wszButtonItem, L"SEPARATOR1"))
        {
          if (bInRow && (!bPrevSeparator || !xstrcmpW(wszButtonItem, L"SEPARATOR")))
          {
            if (lpButton=StackInsertBeforeButton(hStack, lpNextRowItemFirstButton))
            {
              lpLastButton=lpButton;
              if (lpRowItem && !lpRowItem->lpFirstToolbarItem)
                lpRowItem->lpFirstToolbarItem=lpButton;
              lpButton->nTextOffset=(int)(wpLineBegin - wpTextBegin);

              lpButton->tbb.iBitmap=-1;
              lpButton->tbb.idCommand=0;
              lpButton->tbb.fsState=0;
              lpButton->tbb.fsStyle=TBSTYLE_SEP;
              lpButton->tbb.dwData=0;
              lpButton->tbb.iString=0;
            }
          }
          bPrevSeparator=TRUE;
          bMethod=TRUE;
        }
        else
        {
          bPrevSeparator=FALSE;

          if (!xstrcmpW(wszButtonItem, L"BREAK"))
          {
            if (lpLastButton)
            {
              lpLastButton->tbb.fsState|=TBSTATE_WRAP;
              if (lpLastButton->tbb.fsStyle & TBSTYLE_SEP)
                ++hStack->nSepRows;
            }
            bMethod=TRUE;
            ++hStack->nRows;

            if (hRowListStack.nElements)
            {
              if (lpRowItem=GetRow(&hRowListStack, hStack->nRows))
              {
                lpNextRowItemFirstButton=GetFirstToolbarItemOfNextRow(lpRowItem);
                bInRow=TRUE;
              }
              else
              {
                lpNextRowItemFirstButton=NULL;
                bInRow=FALSE;
              }
            }
          }
          else
          {
            nMessageID=STRID_PARSEMSG_UNKNOWNSPECIAL;
            goto Error;
          }
        }
      }
      else
      {
        //Normal item
        bPrevSeparator=FALSE;
        while (*wpCount == L' ' || *wpCount == L'\t') ++wpCount;

        if (*wpCount == L'+')
        {
          ++wpCount;
          nPlus=1;
        }
        else nPlus=0;

        //Parse methods
        for (;;)
        {
          while (*wpCount == L' ' || *wpCount == L'\t') ++wpCount;
          wpErrorBegin=wpCount;

          if (!MethodGetName(wpCount, wszMethodName, MAX_PATH, &wpCount))
            break;

          if (!xstrcmpiW(wszMethodName, L"Icon"))
          {
            if (hIcon)
            {
              nMessageID=STRID_PARSEMSG_METHODALREADYDEFINED;
              goto Error;
            }
            MethodGetIcon(wpCount, wszIconFile, MAX_PATH, &nFileIconIndex, &wpCount);
            if (*(wpCount - 1) == L')') --wpCount;

            if (bInRow)
            {
              if (!*wszIconFile)
              {
                hIcon=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(nFileIconIndex + 100), IMAGE_ICON, sizeIcon.cx, sizeIcon.cy, 0);
              }
              else
              {
                wchar_t wszPath[MAX_PATH];
                wchar_t *wpFileName;

                if (TranslateFileString(wszIconFile, wszPath, MAX_PATH))
                {
                  if (SearchPathWide(NULL, wszPath, NULL, MAX_PATH, wszIconFile, &wpFileName))
                  {
                    //if (nBigIcons)
                    //  ExtractIconExWide(wszPath, nFileIconIndex, &hIcon, NULL, 1);
                    //else
                    //  ExtractIconExWide(wszPath, nFileIconIndex, NULL, &hIcon, 1);
                    hIcon=IconExtractWide(wszPath, nFileIconIndex, sizeIcon.cx, sizeIcon.cy);
                  }
                }
              }
              if (hIcon)
              {
                if (nArrowOverlay > 0)
                {
                  //Method "Menu()" without action. Add overlay arrow.
                  if (hParamMenuName.first && !lpButton)
                  {
                    if (!hIconArrowOverlay)
                      hIconArrowOverlay=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(nArrowOverlay == IAO_COPYNORMAL?IDI_ICONARROW1:IDI_ICONARROW2), IMAGE_ICON, sizeIcon.cx, sizeIcon.cy, 0);
                    if (hAdjustIcon=IconMenu_MixIcons(NULL, hIcon, hIconArrowOverlay, nArrowOverlay == IAO_COPYWHITEASMASK))
                    {
                      DestroyIcon(hIcon);
                      hIcon=hAdjustIcon;
                    }
                  }
                }
                ImageList_AddIcon(hStack->hImageList, hIcon);
                if (hStack->hDisabledImageList)
                {
                  if (hAdjustIcon=IconMenu_AdjustIcon(NULL, hIcon, TRUE, 30, 0))
                  {
                    ImageList_AddIcon(hStack->hDisabledImageList, hAdjustIcon);
                    DestroyIcon(hAdjustIcon);
                  }
                }
                DestroyIcon(hIcon);
              }
            }
          }
          else if (!xstrcmpiW(wszMethodName, L"Menu"))
          {
            if (!hParamMenuName.first)
            {
              if (!MethodParseParameters(&hParamMenuName, wpCount, &wpCount))
              {
                nMessageID=STRID_PARSEMSG_WRONGPARAMCOUNT;
                goto Error;
              }
              if (*(wpCount - 1) == L')') --wpCount;
              if (!bInRow) MethodFreeParameters(&hParamMenuName);
              bMethod=TRUE;
            }
            else
            {
              nMessageID=STRID_PARSEMSG_METHODALREADYDEFINED;
              goto Error;
            }
          }
          else
          {
            //Actions
            if (lpButton)
            {
              nMessageID=STRID_PARSEMSG_METHODALREADYDEFINED;
              goto Error;
            }

            if (!xstrcmpiW(wszMethodName, L"Command"))
              dwAction=EXTACT_COMMAND;
            else if (!xstrcmpiW(wszMethodName, L"Call"))
              dwAction=EXTACT_CALL;
            else if (!xstrcmpiW(wszMethodName, L"Exec"))
              dwAction=EXTACT_EXEC;
            else if (!xstrcmpiW(wszMethodName, L"OpenFile"))
              dwAction=EXTACT_OPENFILE;
            else if (!xstrcmpiW(wszMethodName, L"SaveFile"))
              dwAction=EXTACT_SAVEFILE;
            else if (!xstrcmpiW(wszMethodName, L"Font"))
              dwAction=EXTACT_FONT;
            else if (!xstrcmpiW(wszMethodName, L"Recode"))
              dwAction=EXTACT_RECODE;
            else if (!xstrcmpiW(wszMethodName, L"Insert"))
              dwAction=EXTACT_INSERT;
            else
              dwAction=0;

            if (!dwAction)
            {
              nMessageID=STRID_PARSEMSG_UNKNOWNMETHOD;
              goto Error;
            }

            if (bInRow)
            {
              if (lpButton=StackInsertBeforeButton(hStack, lpNextRowItemFirstButton))
              {
                lpLastButton=lpButton;
                if (lpRowItem && !lpRowItem->lpFirstToolbarItem)
                  lpRowItem->lpFirstToolbarItem=lpButton;
                lpButton->bUpdateItem=!nMinus;
                lpButton->lpStateIf=lpStateIf;
                lpButton->bAutoLoad=nPlus;
                lpButton->dwAction=dwAction;
                lpButton->nTextOffset=(int)(wpLineBegin - wpTextBegin);

                xstrcpynW(lpButton->wszButtonItem, wszButtonItem, MAX_PATH);
                lpButton->tbb.iBitmap=-1;
                lpButton->tbb.idCommand=++nItemCommand;
                lpButton->tbb.fsState=TBSTATE_ENABLED;
                lpButton->tbb.fsStyle=TBSTYLE_BUTTON;
                lpButton->tbb.dwData=0;
                lpButton->tbb.iString=0;
                if (!MethodParseParameters(&lpButton->hParamStack, wpCount, &wpCount))
                {
                  nMessageID=STRID_PARSEMSG_WRONGPARAMCOUNT;
                  goto Error;
                }
                if (*(wpCount - 1) == L')') --wpCount;

                if (dwAction == EXTACT_COMMAND)
                {
                  int nCommand=0;

                  if (lpParameter=MethodGetParameter(&lpButton->hParamStack, 1))
                    nCommand=(int)lpParameter->nNumber;

                  if (nCommand == IDM_FILE_OPEN)
                  {
                    lpButton->tbb.fsStyle|=TBSTYLE_DROPDOWN;
                  }
                  if (!*lpButton->wszButtonItem)
                  {
                    GetMenuStringWide(hMainMenu, nCommand, lpButton->wszButtonItem, MAX_PATH, MF_BYCOMMAND);
                  }
                }
              }
            }
            else
            {
              if (!MethodParseParameters(&hParamButton, wpCount, &wpCount))
              {
                nMessageID=STRID_PARSEMSG_WRONGPARAMCOUNT;
                goto Error;
              }
              if (*(wpCount - 1) == L')') --wpCount;
              MethodFreeParameters(&hParamButton);
            }
            bMethod=TRUE;
          }

          MethodComment(wpCount, &wpCount);
          if (*wpCount != L')')
          {
            nMessageID=STRID_PARSEMSG_NOCLOSEPARENTHESIS;
            goto Error;
          }
          ++wpCount;
        }

        if (bMethod)
        {
          if (hParamMenuName.first)
          {
            if (!lpButton)
            {
              //Method "Menu()" without action
              if (lpButton=StackInsertBeforeButton(hStack, lpNextRowItemFirstButton))
              {
                lpLastButton=lpButton;
                if (lpRowItem && !lpRowItem->lpFirstToolbarItem)
                  lpRowItem->lpFirstToolbarItem=lpButton;
                lpButton->dwAction=EXTACT_MENU;
                lpButton->nTextOffset=(int)(wpLineBegin - wpTextBegin);

                xstrcpynW(lpButton->wszButtonItem, wszButtonItem, MAX_PATH);
                lpButton->tbb.iBitmap=-1;
                lpButton->tbb.idCommand=++nItemCommand;
                lpButton->tbb.fsState=TBSTATE_ENABLED;
                lpButton->tbb.fsStyle=TBSTYLE_BUTTON|(nArrowOverlay == IAO_WHOLEDROPDOWN?BTNS_WHOLEDROPDOWN:0);
                lpButton->tbb.dwData=0;
                lpButton->tbb.iString=0;
              }
            }
            else lpButton->tbb.fsStyle|=TBSTYLE_DROPDOWN;

            lpButton->hParamMenuName=hParamMenuName;
            hParamMenuName.first=0;
            hParamMenuName.last=0;
            hParamMenuName.nElements=0;
          }
          if (hIcon)
          {
            lpButton->tbb.iBitmap=nItemBitmap++;
          }
        }
        else
        {
          nMessageID=STRID_PARSEMSG_NOMETHOD;
          goto Error;
        }
      }
    }
  }
  if (hRowListStack.nElements)
  {
    if (lpLastButton)
      lpLastButton->tbb.fsState|=TBSTATE_WRAP;
    if (hStack->last && hRowListStack.last->lpFirstToolbarItem)
      hStack->last->tbb.fsState&=~TBSTATE_WRAP;
  }
  else
  {
    //First button with TBSTATE_WRAP + TBSTYLE_SEP skipped in SetToolbarButtons.
    dwStyle=(DWORD)GetWindowLongPtrWide(hToolbar, GWL_STYLE);

    if (nToolbarSide == TBSIDE_BOTTOM)
    {
      if (hStack->first && (hStack->first->tbb.fsState & TBSTATE_WRAP))
      {
        if (dwStyle & CCS_NODIVIDER)
          SetWindowLongPtrWide(hToolbar, GWL_STYLE, dwStyle & ~CCS_NODIVIDER);
      }
      else
      {
        if (!(dwStyle & CCS_NODIVIDER))
          SetWindowLongPtrWide(hToolbar, GWL_STYLE, dwStyle|CCS_NODIVIDER);
      }
    }
  }
  return TRUE;

  Error:
  MethodFreeParameters(&hParamStack);
  crExtSetSel.cpMin=wpErrorBegin - wpText;
  crExtSetSel.cpMax=wpCount - wpText;

  if (!hWndMainDlg)
    PostMessage(hToolbarBG, AKDLL_SETUP, 0, 0);
  else
    PostMessage(hWndMainDlg, AKDLL_SELTEXT, 0, 0);

  xprintfW(wszBuffer, GetLangStringW(wLangModule, nMessageID), wszButtonItem, wszMethodName);
  MessageBoxW(hWndMainDlg?hWndMainDlg:NULL, wszBuffer, wszPluginTitle, MB_OK|MB_ICONERROR|(hWndMainDlg?0:MB_TOPMOST));
  return FALSE;
}

DWORD IsFlagOn(DWORD dwSetFlags, DWORD dwCheckFlags)
{
  if (!dwSetFlags) return 0;

  if ((dwCheckFlags & CCMS_NOSDI) && (dwSetFlags & CCMS_NOSDI) && nMDI == WMD_SDI)
    return CCMS_NOSDI;
  if ((dwCheckFlags & CCMS_NOMDI) && (dwSetFlags & CCMS_NOMDI) && nMDI == WMD_MDI)
    return CCMS_NOMDI;
  if ((dwCheckFlags & CCMS_NOPMDI) && (dwSetFlags & CCMS_NOPMDI) && nMDI == WMD_PMDI)
    return CCMS_NOPMDI;
  if ((dwCheckFlags & CCMS_NOFILEEXIST) && (dwSetFlags & CCMS_NOFILEEXIST))
    return CCMS_NOFILEEXIST;
  if ((dwCheckFlags & CCMS_SKIPIF) && (dwSetFlags & CCMS_SKIPIF))
    return CCMS_SKIPIF;
  if ((dwCheckFlags & CCMS_CHECKIF) && (dwSetFlags & CCMS_CHECKIF))
    return CCMS_CHECKIF;
  return 0;
}

int ParseRows(STACKROW *lpRowListStack)
{
  STACKROW hCurRowListStack;
  ROWITEM *lpRowItem;
  wchar_t *wpCount=wszRowList;
  int nRow;
  int nShow;
  BOOL bCurExist;

  xmemcpy(&hCurRowListStack, lpRowListStack, sizeof(STACKROW));
  xmemset(lpRowListStack, 0, sizeof(STACKROW));

  if (*wpCount)
  {
    while (nRow=(int)xatoiW(wpCount, (const wchar_t **)&wpCount))
    {
      nShow=ROWSHOW_ON;
      if (*wpCount == L'(')
      {
        nShow=(int)xatoiW(++wpCount, (const wchar_t **)&wpCount);
        if (*wpCount == L')') ++wpCount;
      }

      if (nShow != ROWSHOW_OFF)
      {
        if (!hCurRowListStack.nElements || GetRow(&hCurRowListStack, nRow))
          bCurExist=TRUE;
        else
          bCurExist=FALSE;

        if (nShow == ROWSHOW_ON ||
            (nShow == ROWSHOW_UNCHANGE && bCurExist) ||
            (nShow == ROWSHOW_INVERT && !bCurExist))
        {
          if (!GetRow(lpRowListStack, nRow))
          {
            if (!StackInsertIndex((stack **)&lpRowListStack->first, (stack **)&lpRowListStack->last, (stack **)&lpRowItem, -1, sizeof(ROWITEM)))
            {
              lpRowItem->nRow=nRow;
              ++lpRowListStack->nElements;
            }
          }
        }
      }

      if (*wpCount == L',')
        ++wpCount;
      else
        break;
    }
  }
  FreeRows(&hCurRowListStack);

  //Rebuild rows
  wpCount=wszRowList;
  for (lpRowItem=lpRowListStack->first; lpRowItem; lpRowItem=lpRowItem->next)
  {
    wpCount+=xprintfW(wpCount, L"%s%d", (lpRowItem == lpRowListStack->first?L"":L","), lpRowItem->nRow);
  }

  return lpRowListStack->nElements;
}

ROWITEM* GetRow(STACKROW *lpRowListStack, int nRow)
{
  ROWITEM *lpRowItem;

  for (lpRowItem=lpRowListStack->first; lpRowItem; lpRowItem=lpRowItem->next)
  {
    if (lpRowItem->nRow == nRow)
      break;
  }
  return lpRowItem;
}

TOOLBARITEM* GetFirstToolbarItemOfNextRow(ROWITEM *lpRowItem)
{
  for (lpRowItem=lpRowItem->next; lpRowItem; lpRowItem=lpRowItem->next)
  {
    if (lpRowItem->lpFirstToolbarItem)
      return lpRowItem->lpFirstToolbarItem;
  }
  return NULL;
}

void FreeRows(STACKROW *lpRowListStack)
{
  StackClear((stack **)&lpRowListStack->first, (stack **)&lpRowListStack->last);
  lpRowListStack->nElements=0;
}

void FreeToolbarData(STACKTOOLBAR *hStack)
{
  STATEIF *lpStateIf;

  for (lpStateIf=hStack->hStateIfStack.first; lpStateIf; lpStateIf=lpStateIf->next)
  {
    MethodFreeParameters(&lpStateIf->hParamStack);
  }
  StackClear((stack **)&hStack->hStateIfStack.first, (stack **)&hStack->hStateIfStack.last);

  StackFreeButton(hStack);
  if (hStack->hImageList)
  {
    ImageList_Destroy(hStack->hImageList);
    hStack->hImageList=NULL;
  }
  if (hStack->hDisabledImageList)
  {
    ImageList_Destroy(hStack->hDisabledImageList);
    hStack->hDisabledImageList=NULL;
  }
  if (hIconArrowOverlay)
  {
    DestroyIcon(hIconArrowOverlay);
    hIconArrowOverlay=NULL;
  }
  hStack->nRows=0;
  hStack->nSepRows=0;
}

void SetToolbarButtons(STACKTOOLBAR *hStack)
{
  TOOLBARITEM *lpButton;
  int nArrorWidth;
  int nRowWidth;
  int nMaxRowWidth;

  if (nBigIcons)
    nArrorWidth=10;
  else
    nArrorWidth=9;
  sizeButtons.cx=sizeIcon.cx + 8;
  sizeButtons.cy=sizeIcon.cy + 6;
  sizeToolbar.cx=sizeIcon.cx + 12;
  sizeToolbar.cy=sizeIcon.cy + 12;
  SendMessage(hToolbar, TB_SETBITMAPSIZE, 0, MAKELONG(sizeIcon.cx, sizeIcon.cy));

  nMaxRowWidth=sizeButtons.cx;
  SendMessage(hToolbar, TB_SETBUTTONSIZE, 0, MAKELONG(sizeButtons.cx, sizeButtons.cy));
  SendMessage(hToolbar, TB_SETIMAGELIST, 0, (LPARAM)hStack->hImageList);
  SendMessage(hToolbar, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)hStack->hDisabledImageList);

  for (lpButton=hStack->first; lpButton; lpButton=lpButton->next)
  {
    lpButton->nButtonWidth=sizeButtons.cx;
    if (lpButton->tbb.fsStyle & TBSTYLE_DROPDOWN)
      lpButton->nButtonWidth+=nArrorWidth;
    nRowWidth=lpButton->nButtonWidth;

    ////Calculate vertical row width
    //if (nToolbarSide == TBSIDE_LEFT || nToolbarSide == TBSIDE_RIGHT)
    //{
    //  for (lpTmpButton=lpButton->prev; lpTmpButton; lpTmpButton=lpTmpButton->prev)
    //  {
    //    if (lpTmpButton->tbb.fsState & TBSTATE_WRAP)
    //      break;
    //    nRowWidth+=lpTmpButton->nButtonWidth;
    //  }
    //}

    if (lpButton->tbb.fsState & TBSTATE_WRAP)
    {
      if (nToolbarSide == TBSIDE_TOP || nToolbarSide == TBSIDE_BOTTOM)
      {
        if (lpButton->prev && lpButton->next)
          sizeToolbar.cy+=sizeButtons.cy + ((lpButton->tbb.fsStyle & TBSTYLE_SEP)?8:0);
        else
        {
          sizeToolbar.cy+=(lpButton->tbb.fsStyle & TBSTYLE_SEP)?2:0;
          //First button with TBSTATE_WRAP + TBSTYLE_SEP signal to remove CCS_NODIVIDER
          //style (already done in CreateToolbarData) and don't add this button.
          if (!lpButton->prev) continue;
        }
      }
    }
    else if (nToolbarSide == TBSIDE_LEFT || nToolbarSide == TBSIDE_RIGHT)
      lpButton->tbb.fsState|=TBSTATE_WRAP;

    nMaxRowWidth=max(nRowWidth, nMaxRowWidth);
    SendMessage(hToolbar, TB_ADDBUTTONS, 1, (LPARAM)&lpButton->tbb);
  }

  if (nToolbarSide == TBSIDE_LEFT || nToolbarSide == TBSIDE_RIGHT)
    sizeToolbar.cx=(sizeToolbar.cx - sizeButtons.cx) + nMaxRowWidth;
}

void ClearToolbarButtons()
{
  while (SendMessage(hToolbar, TB_DELETEBUTTON, 0, 0));
}

void UpdateToolbar(STACKTOOLBAR *hStack)
{
  EDITINFO ei;
  STATEIF *lpStateIf;
  TOOLBARITEM *lpButton;
  EXTPARAM *lpParameter;
  BOOL bInitMenu=FALSE;
  static BOOL bUpdating;

  if (!bUpdating)
  {
    bUpdating=TRUE;
    ei.hWndEdit=NULL;
    for (lpStateIf=hStack->hStateIfStack.first; lpStateIf; lpStateIf=lpStateIf->next)
      lpStateIf->bCalculated=FALSE;

    for (lpButton=hStack->first; lpButton; lpButton=lpButton->next)
    {
      if (lpButton->bUpdateItem)
      {
        if (lpButton->lpStateIf)
        {
          if (!lpButton->lpStateIf->nStopError)
          {
            DWORD dwButtonState;

            lpStateIf=lpButton->lpStateIf;

            if (!lpStateIf->bCalculated)
            {
              IFEXPRESSION ie;
              INT_PTR nIfTrue=0;
              INT_PTR nIfFalse=0;
              int nMessageID;
              EXPPARAM ep[]={{L"%f", 2, 0, EXPPARAM_FILE},
                             {L"%d", 2, 0, EXPPARAM_FILEDIR},
                             {L"%m", 2, (INT_PTR)hToolbar, EXPPARAM_INT},
                             {L"%i", 2, lpButton->tbb.idCommand, EXPPARAM_INT},
                             {0, 0, 0, 0}};

              SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpStateIf->hParamStack, (LPARAM)ep);
              if (lpParameter=MethodGetParameter(&lpStateIf->hParamStack, 2))
                nIfTrue=lpParameter->nNumber;
              if (lpParameter=MethodGetParameter(&lpStateIf->hParamStack, 3))
                nIfFalse=lpParameter->nNumber;

              nFocusChanged=-1;
              ie.dwFlags=lpStateIf->dwFlags|IEF_STACKEXTPARAM;
              ie.sep=&lpStateIf->hParamStack;
              lpStateIf->nValue=SendMessage(hMainWnd, AKD_IFEXPRESSION, (WPARAM)NULL, (LPARAM)&ie);
              if (ie.nError == IEE_SUCCESS && nFocusChanged == 1)
                ie.nError=IEE_FOCUSCHANGED;
              nFocusChanged=0;

              if (lpParameter)
              {
                if (lpStateIf->nValue)
                  lpStateIf->nValue=nIfTrue;
                else
                  lpStateIf->nValue=nIfFalse;
              }

              if (ie.nError)
              {
                lpStateIf->nStopError=ie.nError;
                nMessageID=(ie.nError - 1) + STRID_IF_NOCOMMA;
                xprintfW(wszBuffer, GetLangStringW(wLangModule, nMessageID), ie.wpEnd);
                MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONERROR);
                ViewItemCode(lpButton);
                return;
              }
              lpStateIf->bCalculated=TRUE;
            }
            dwButtonState=(DWORD)SendMessage(hToolbar, TB_GETSTATE, lpButton->tbb.idCommand, 0);
            if (!(dwButtonState & TBSTATE_CHECKED) != !(lpStateIf->nValue & IFS_CHECKED))
              SendMessage(hToolbar, TB_CHECKBUTTON, lpButton->tbb.idCommand, (lpStateIf->nValue & TBSTATE_CHECKED)?TRUE:FALSE);
            if (!(dwButtonState & TBSTATE_ENABLED) == !(lpStateIf->nValue & IFS_GRAYED))
              SendMessage(hToolbar, TB_ENABLEBUTTON, lpButton->tbb.idCommand, (lpStateIf->nValue & IFS_GRAYED)?FALSE:TRUE);
          }
        }
        else
        {
          if (lpButton->dwAction == EXTACT_COMMAND)
          {
            DWORD dwMenuState=0;
            DWORD dwButtonState;
            int nCommand=0;

            if (lpParameter=MethodGetParameter(&lpButton->hParamStack, 1))
              nCommand=(int)lpParameter->nNumber;

            if (nCommand)
            {
              if (nCommand == IDM_VIEW_SPLIT_WINDOW_ALL ||
                  nCommand == IDM_VIEW_SPLIT_WINDOW_WE ||
                  nCommand == IDM_VIEW_SPLIT_WINDOW_NS)
              {
                if (!ei.hWndEdit)
                  SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei);
                if (ei.hWndEdit)
                {
                  if ((nCommand == IDM_VIEW_SPLIT_WINDOW_ALL && ei.hWndClone1 && ei.hWndClone2 && ei.hWndClone3) ||
                      (nCommand == IDM_VIEW_SPLIT_WINDOW_WE && ei.hWndClone1 && !ei.hWndClone2 && !ei.hWndClone3) ||
                      (nCommand == IDM_VIEW_SPLIT_WINDOW_NS && !ei.hWndClone1 && ei.hWndClone2 && !ei.hWndClone3))
                  {
                    dwMenuState=MF_CHECKED;
                  }
                }
              }
              else
              {
                if (!bInitMenu)
                {
                  SendMessage(hMainWnd, WM_INITMENU, (WPARAM)hMainMenu, IMENU_EDIT|IMENU_CHECKS);
                  bInitMenu=TRUE;
                }
                dwMenuState=GetMenuState(hMainMenu, nCommand, MF_BYCOMMAND);
              }
              if (dwMenuState != (DWORD)-1)
              {
                dwButtonState=(DWORD)SendMessage(hToolbar, TB_GETSTATE, lpButton->tbb.idCommand, 0);
                if (!(dwMenuState & MF_CHECKED) != !(dwButtonState & TBSTATE_CHECKED))
                  SendMessage(hToolbar, TB_CHECKBUTTON, lpButton->tbb.idCommand, (dwMenuState & MF_CHECKED)?TRUE:FALSE);
                if (!(dwMenuState & MF_GRAYED) == !(dwButtonState & TBSTATE_ENABLED))
                  SendMessage(hToolbar, TB_ENABLEBUTTON, lpButton->tbb.idCommand, (dwMenuState & MF_GRAYED)?FALSE:TRUE);
              }
            }
          }
          else if (lpButton->dwAction == EXTACT_CALL)
          {
            PLUGINFUNCTION *pf;
            wchar_t *wpFunction=NULL;
            int nDllAction=0;
            BOOL bChecked;

            if (lpParameter=MethodGetParameter(&lpButton->hParamStack, 1))
              wpFunction=lpParameter->wpString;
            if (lpParameter=MethodGetParameter(&lpButton->hParamStack, 2))
              nDllAction=(int)lpParameter->nNumber;

            if (wpFunction)
            {
              bChecked=FALSE;
              if (!lpParameter && (pf=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wpFunction, 0)))
                if (pf->bRunning) bChecked=TRUE;
              SendMessage(hToolbar, TB_CHECKBUTTON, lpButton->tbb.idCommand, bChecked);
            }
          }
        }
      }
    }
    bUpdating=FALSE;
  }
}

void ViewItemCode(TOOLBARITEM *lpButton)
{
  if (lpButton)
  {
    crExtSetSel.cpMin=lpButton->nTextOffset;
    crExtSetSel.cpMax=-1;
  }
  if (!hWndMainDlg)
    PostMessage(hToolbarBG, AKDLL_SETUP, 0, 0);
  else
    PostMessage(hWndMainDlg, AKDLL_SELTEXT, 0, 0);
}

void CallToolbar(STACKTOOLBAR *hStack, int nItem)
{
  static RECT rcButton;
  TOOLBARITEM *lpElement;
  EXTPARAM *lpParameter;
  EXPPARAM ep[]={{L"%f", 2, 0, EXPPARAM_FILE},
                 {L"%d", 2, 0, EXPPARAM_FILEDIR},
                 {L"%m", 2, (INT_PTR)hToolbar, EXPPARAM_INT},
                 {L"%i", 2, nItem, EXPPARAM_INT},
                 {L"%bl", 3, (INT_PTR)&rcButton.left, EXPPARAM_LPINT},
                 {L"%bt", 3, (INT_PTR)&rcButton.top, EXPPARAM_LPINT},
                 {L"%br", 3, (INT_PTR)&rcButton.right, EXPPARAM_LPINT},
                 {L"%bb", 3, (INT_PTR)&rcButton.bottom, EXPPARAM_LPINT},
                 {0, 0, 0, 0}};

  if (lpElement=StackGetButtonByID(hStack, nItem))
  {
    if (GetKeyState(VK_CONTROL) & 0x80)
    {
      ViewItemCode(lpElement);
      return;
    }

    SendMessage(hToolbar, TB_GETRECT, nItem, (LPARAM)&rcButton);
    ClientToScreen(hToolbar, (LPPOINT)&rcButton.left);
    ClientToScreen(hToolbar, (LPPOINT)&rcButton.right);

    if (lpElement->dwAction == EXTACT_COMMAND)
    {
      int nCommand=0;
      LPARAM lParam=0;

      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
        nCommand=(int)lpParameter->nNumber;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
        lParam=lpParameter->nNumber;

      if (nCommand)
      {
        SendMessage(hMainWnd, WM_COMMAND, nCommand, lParam);
      }
    }
    else if (lpElement->dwAction == EXTACT_CALL)
    {
      PLUGINCALLSENDW pcs;
      PLUGINCALLPOSTW *pcp;
      wchar_t *wpFunction=lpElement->hParamStack.first->wpString;
      int nStructSize;
      BOOL bCall=FALSE;

      if (lpElement->bAutoLoad)
      {
        pcs.pFunction=wpFunction;
        pcs.lParam=0;
        pcs.dwSupport=PDS_GETSUPPORT;

        if (SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs) != UD_FAILED)
        {
          if (pcs.dwSupport & PDS_NOAUTOLOAD)
          {
            xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_AUTOLOAD), wpFunction);
            MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
          }
          else bCall=TRUE;
        }
      }
      else bCall=TRUE;

      if (bCall)
      {
        SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);

        nStructSize=MethodStructParameters(&lpElement->hParamStack, NULL);
        if (pcp=(PLUGINCALLPOSTW *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW) + nStructSize))
        {
          xstrcpynW(pcp->szFunction, wpFunction, MAX_PATH);
          if (nStructSize > 0)
          {
            pcp->lParam=(LPARAM)((unsigned char *)pcp + sizeof(PLUGINCALLPOSTW));
            MethodStructParameters(&lpElement->hParamStack, (unsigned char *)pcp->lParam);
          }
          else pcp->lParam=0;

          PostMessage(hMainWnd, AKD_DLLCALLW, (lpElement->bAutoLoad?DLLCF_SWITCHAUTOLOAD|DLLCF_SAVEONEXIT:0), (LPARAM)pcp);
        }
      }
    }
    else if (lpElement->dwAction == EXTACT_EXEC)
    {
      STARTUPINFOW si;
      PROCESS_INFORMATION pi;
      wchar_t *wpCmdLine=NULL;
      wchar_t *wpWorkDir=NULL;
      BOOL bWait=FALSE;
      int nShowWindow=-1;

      SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
        wpCmdLine=lpParameter->wpExpanded;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
        wpWorkDir=lpParameter->wpExpanded;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 3))
        bWait=(BOOL)lpParameter->nNumber;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 4))
        nShowWindow=(int)lpParameter->nNumber;

      if (wpCmdLine)
      {
        xmemset(&si, 0, sizeof(STARTUPINFOW));
        si.cb=sizeof(STARTUPINFOW);
        if (nShowWindow >= 0)
        {
          si.dwFlags=STARTF_USESHOWWINDOW;
          si.wShowWindow=(WORD)nShowWindow;
        }
        if (CreateProcessWide(NULL, wpCmdLine, NULL, NULL, FALSE, 0, NULL, (wpWorkDir && *wpWorkDir)?wpWorkDir:NULL, &si, &pi))
        {
          if (bWait)
          {
            WaitForSingleObject(pi.hProcess, INFINITE);
            //GetExitCodeProcess(pi.hProcess, &dwExit);
          }
          CloseHandle(pi.hProcess);
          CloseHandle(pi.hThread);
        }
      }
    }
    else if (lpElement->dwAction == EXTACT_OPENFILE ||
             lpElement->dwAction == EXTACT_SAVEFILE)
    {
      wchar_t *wpFile=NULL;
      int nCodePage=-1;
      BOOL bBOM=-1;

      SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
        wpFile=lpParameter->wpExpanded;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
        nCodePage=(int)lpParameter->nNumber;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 3))
        bBOM=(BOOL)lpParameter->nNumber;

      if (lpElement->dwAction == EXTACT_OPENFILE)
      {
        OPENDOCUMENTW od;

        od.dwFlags=OD_ADT_BINARYERROR;
        if (nCodePage == -1) od.dwFlags|=OD_ADT_DETECTCODEPAGE;
        if (bBOM == -1) od.dwFlags|=OD_ADT_DETECTBOM;

        od.pFile=wpFile;
        od.pWorkDir=NULL;
        od.nCodePage=nCodePage;
        od.bBOM=bBOM;
        od.hDoc=NULL;
        SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);
      }
      else if (lpElement->dwAction == EXTACT_SAVEFILE)
      {
        EDITINFO ei;
        SAVEDOCUMENTW sd;

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
        {
          if (nCodePage == -1)
            nCodePage=ei.nCodePage;
          if (bBOM == -1)
            bBOM=ei.bBOM;
          sd.pFile=wpFile;
          sd.nCodePage=nCodePage;
          sd.bBOM=bBOM;
          sd.dwFlags=SD_UPDATE;
          sd.hDoc=NULL;
          SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)NULL, (LPARAM)&sd);
        }
      }
    }
    else if (lpElement->dwAction == EXTACT_FONT)
    {
      EDITINFO ei;
      LOGFONTW lfNew;
      HDC hDC;
      wchar_t *wpFaceName=NULL;
      DWORD dwFontStyle=0;
      int nPointSize=0;

      SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
        wpFaceName=lpParameter->wpExpanded;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
        dwFontStyle=(DWORD)lpParameter->nNumber;
      if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 3))
        nPointSize=(int)lpParameter->nNumber;

      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
      {
        if (SendMessage(hMainWnd, AKD_GETFONTW, (WPARAM)ei.hWndEdit, (LPARAM)&lfNew))
        {
          if (nPointSize)
          {
            if (hDC=GetDC(ei.hWndEdit))
            {
              lfNew.lfHeight=-MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
              ReleaseDC(ei.hWndEdit, hDC);
            }
          }
          if (dwFontStyle != FS_NONE)
          {
            lfNew.lfWeight=(dwFontStyle == FS_FONTBOLD || dwFontStyle == FS_FONTBOLDITALIC)?FW_BOLD:FW_NORMAL;
            lfNew.lfItalic=(dwFontStyle == FS_FONTITALIC || dwFontStyle == FS_FONTBOLDITALIC)?TRUE:FALSE;
          }
          if (*wpFaceName != L'\0')
          {
            xstrcpynW(lfNew.lfFaceName, wpFaceName, LF_FACESIZE);
          }
          SendMessage(hMainWnd, AKD_SETFONTW, (WPARAM)ei.hWndEdit, (LPARAM)&lfNew);
        }
      }
    }
    else if (lpElement->dwAction == EXTACT_RECODE)
    {
      EDITINFO ei;
      TEXTRECODE tr={0};

      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
      {
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
          tr.nCodePageFrom=(int)lpParameter->nNumber;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
          tr.nCodePageTo=(int)lpParameter->nNumber;

        SendMessage(hMainWnd, AKD_RECODESEL, (WPARAM)ei.hWndEdit, (LPARAM)&tr);
      }
    }
    else if (lpElement->dwAction == EXTACT_INSERT)
    {
      EDITINFO ei;
      wchar_t *wpText=NULL;
      wchar_t *wpUnescText=NULL;
      int nTextLen=-1;
      int nUnescTextLen;
      DWORD dwCaret=(DWORD)-1;
      BOOL bEscSequences=FALSE;

      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
      {
        if (!ei.bReadOnly)
        {
          SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
          if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
            wpText=lpParameter->wpExpanded;
          if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
            bEscSequences=(BOOL)lpParameter->nNumber;

          if (bEscSequences)
          {
            if (nUnescTextLen=(int)TranslateEscapeString(ei.hWndEdit, wpText, NULL, NULL))
            {
              if (wpUnescText=(wchar_t *)GlobalAlloc(GPTR, nUnescTextLen * sizeof(wchar_t)))
              {
                nTextLen=(int)TranslateEscapeString(ei.hWndEdit, wpText, wpUnescText, &dwCaret);
                wpText=wpUnescText;
              }
            }
          }
          if (wpText)
          {
            AEREPLACESELW rs;
            AECHARINDEX ciInsertPos;

            rs.pText=wpText;
            rs.dwTextLen=(UINT_PTR)nTextLen;
            rs.nNewLine=AELB_ASINPUT;
            rs.dwFlags=AEREPT_COLUMNASIS;
            rs.ciInsertStart=&ciInsertPos;
            rs.ciInsertEnd=NULL;
            SendMessage(ei.hWndEdit, AEM_REPLACESELW, 0, (LPARAM)&rs);

            if (dwCaret != (DWORD)-1)
            {
              AEINDEXOFFSET io;

              io.ciCharIn=&ciInsertPos;
              io.ciCharOut=&ciInsertPos;
              io.nOffset=dwCaret;
              io.nNewLine=AELB_ASINPUT;
              SendMessage(ei.hWndEdit, AEM_INDEXOFFSET, 0, (LPARAM)&io);
              SendMessage(ei.hWndEdit, AEM_EXSETSEL, (WPARAM)&ciInsertPos, (LPARAM)&ciInsertPos);
            }
          }
          if (wpUnescText) GlobalFree((HGLOBAL)wpUnescText);
        }
      }
    }
    else if (lpElement->dwAction == EXTACT_MENU)
    {
      int nPosX=rcButton.left;
      int nPosY=GetMenuPosY(lpElement, &rcButton);

      CallContextMenuShow(lpElement, nPosX, nPosY, NULL);
    }
  }
}

void CallContextMenuShow(TOOLBARITEM *lpButton, int nPosX, int nPosY, INT_PTR *lpnMenuHeight)
{
  if (lpButton->hParamMenuName.first)
  {
    PLUGINCALLSENDW pcs;
    DLLEXTCONTEXTMENU decm;
    unsigned char szLeft[32];
    unsigned char szBottom[32];
    unsigned char *pMenuName;

    if (bOldWindows)
    {
      xitoaA(nPosX, (char *)szLeft);
      xitoaA(nPosY, (char *)szBottom);
      pMenuName=(unsigned char *)lpButton->hParamMenuName.first->pString;
    }
    else
    {
      xitoaW(nPosX, (wchar_t *)szLeft);
      xitoaW(nPosY, (wchar_t *)szBottom);
      pMenuName=(unsigned char *)lpButton->hParamMenuName.first->wpString;
    }
    decm.dwStructSize=sizeof(DLLEXTCONTEXTMENU);
    decm.nAction=DLLA_CONTEXTMENU_SHOWSUBMENU;
    decm.pPosX=szLeft;
    decm.pPosY=szBottom;
    decm.nMenuIndex=-1;
    decm.pMenuName=pMenuName;
    decm.lpnMenuHeight=lpnMenuHeight;

    pcs.pFunction=L"ContextMenu::Show";
    pcs.lParam=(LPARAM)&decm;
    pcs.dwSupport=0;
    SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
  }
}

int GetMenuPosY(TOOLBARITEM *lpButton, RECT *rcButton)
{
  int nMaxY;
  INT_PTR nPosY;
  INT_PTR nMenuHeight=0;

  nPosY=rcButton->bottom;

  if (nToolbarSide != TBSIDE_TOP)
  {
    //Get menu height
    if (!lpButton->hParamMenuName.first)
    {
      //IDM_FILE_OPEN
      STACKRECENTFILE *rfs;

      if (SendMessage(hMainWnd, AKD_RECENTFILES, RF_GET, (LPARAM)&rfs))
        nMenuHeight=rfs->nElements * GetSystemMetrics(SM_CYMENU);
    }
    else
    {
      //ContextMenu::Show
      CallContextMenuShow(lpButton, 0, 0, &nMenuHeight);
    }

    //Correct y-position
    nMaxY=GetSystemMetrics(SM_CYSCREEN);

    if (nPosY + nMenuHeight > nMaxY)
    {
      nPosY=rcButton->top;
      if (nPosY + nMenuHeight <= nMaxY)
        nPosY=rcButton->top - nMenuHeight;
    }
  }
  return (int)nPosY;
}

void DestroyToolbarWindow(BOOL bDestroyBG)
{
  DestroyWindow(hToolbar);
  hToolbar=NULL;
  FreeToolbarData(&hStackToolbar);

  if (bDestroyBG)
  {
    DestroyWindow(hToolbarBG);
    hToolbarBG=NULL;
    UnregisterClassWide(TOOLBARBACKGROUNDW, hInstanceDLL);
  }
}

TOOLBARITEM* StackInsertBeforeButton(STACKTOOLBAR *hStack, TOOLBARITEM *lpInsertBefore)
{
  TOOLBARITEM *lpElement=NULL;

  StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpInsertBefore, (stack **)&lpElement, sizeof(TOOLBARITEM));
  return lpElement;
}

TOOLBARITEM* StackGetButtonByID(STACKTOOLBAR *hStack, int nItemID)
{
  TOOLBARITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->tbb.idCommand == nItemID)
      return lpElement;
  }
  return NULL;
}

TOOLBARITEM* StackGetButtonByIndex(STACKTOOLBAR *hStack, int nIndex)
{
  TOOLBARITEM *lpElement;
  int nCount=0;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (nCount++ == nIndex)
      return lpElement;
  }
  return NULL;
}

void StackFreeButton(STACKTOOLBAR *hStack)
{
  TOOLBARITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    MethodFreeParameters(&lpElement->hParamStack);
    MethodFreeParameters(&lpElement->hParamMenuName);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote)
{
  const wchar_t *wpCount;
  wchar_t wchStopChar;

  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  if (*wpText == L'\"' || *wpText == L'\'' || *wpText == L'`')
  {
    if (lpbQuote) *lpbQuote=TRUE;
    wchStopChar=*wpText;
    wpCount=++wpText;

    //Parse: "param" or 'param' or `param`
    while (*wpCount != wchStopChar /*&& *wpCount != L'\r'*/ && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
    {
      *wppNextWord=wpCount;
      if (*wpCount == wchStopChar)
        ++*wppNextWord;
    }
  }
  else
  {
    if (lpbQuote) *lpbQuote=FALSE;
    wpCount=wpText;

    //Parse: param1 param2 param3
    while (*wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
      *wppNextWord=wpCount;
  }
  return (int)xstrcpynW(wszWord, wpText, min(nWordMax, wpCount - wpText + 1));
}

BOOL NextLine(const wchar_t **wpText)
{
  while (**wpText != L'\r' && **wpText != L'\n' && **wpText != L'\0') ++*wpText;
  if (**wpText == L'\0') return FALSE;
  if (**wpText == L'\r') ++*wpText;
  if (**wpText == L'\n') ++*wpText;
  return TRUE;
}

BOOL SkipComment(const wchar_t **wpText)
{
  for (;;)
  {
    while (**wpText == L' ' || **wpText == L'\t' || **wpText == L'\r' || **wpText == L'\n') ++*wpText;

    if (**wpText == L';' || **wpText == L'#')
    {
      if (!NextLine(wpText))
        return FALSE;
    }
    else break;
  }
  if (**wpText == L'\0')
    return FALSE;
  return TRUE;
}

int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
    {
      --wpCount;
      break;
    }
  }
  ++wpCount;
  return (int)xstrcpynW(wszFileDir, wpFile, min(nFileDirMax, wpCount - wpFile + 1));
}

INT_PTR TranslateEscapeString(HWND hWndEdit, const wchar_t *wpInput, wchar_t *wszOutput, DWORD *lpdwCaret)
{
  EDITINFO ei;
  const wchar_t *a=wpInput;
  wchar_t *b=wszOutput;
  wchar_t whex[5];
  int nDec;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWndEdit, (LPARAM)&ei))
  {
    for (whex[4]=L'\0'; *a; ++a)
    {
      if (*a == L'\\')
      {
        if (*++a == L'\\')
        {
          if (wszOutput) *b=L'\\';
          ++b;
        }
        else if (*a == L'n')
        {
          if (ei.nNewLine == NEWLINE_MAC)
          {
            if (wszOutput) *b=L'\r';
            ++b;
          }
          else if (ei.nNewLine == NEWLINE_UNIX)
          {
            if (wszOutput) *b=L'\n';
            ++b;
          }
          else if (ei.nNewLine == NEWLINE_WIN)
          {
            if (wszOutput) *b=L'\r';
            ++b;
            if (wszOutput) *b=L'\n';
            ++b;
          }
        }
        else if (*a == L't')
        {
          if (wszOutput) *b=L'\t';
          ++b;
        }
        else if (*a == L'[')
        {
          while (*++a == L' ');

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
            nDec=(int)hex2decW(whex, 4, NULL);
            if (nDec == -1) goto Error;
            while (*++a == L' ');

            if (wszOutput) *b=(wchar_t)nDec;
            ++b;
          }
          while (*a && *a != L']');

          if (!*a) goto Error;
        }
        else if (*a == L's')
        {
          CHARRANGE64 cr;

          SendMessage(hWndEdit, EM_EXGETSEL64, 0, (WPARAM)&cr);
          if (cr.cpMin != cr.cpMax)
          {
            if (wszOutput)
            {
              wchar_t *wpText;
              INT_PTR nTextLen=0;

              if (wpText=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)hWndEdit, (LPARAM)&nTextLen))
              {
                xmemcpy(b, wpText, nTextLen * sizeof(wchar_t));
                b+=nTextLen;
                SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpText);
              }
            }
            else b+=cr.cpMax - cr.cpMin;
          }
        }
        else if (*a == L'|')
        {
          if (lpdwCaret) *lpdwCaret=(DWORD)(b - wszOutput);
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
      *b=L'\0';
    else
      ++b;
    return (b - wszOutput);
  }

  Error:
  if (wszOutput) *wszOutput=L'\0';
  return 0;
}

int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%a -AkelPad directory, %% -%
  wchar_t *wpExeDir=wszExeDir;
  wchar_t *wszSource;
  wchar_t *wpSource;
  wchar_t *wpTarget=wszBuffer;
  wchar_t *wpTargetMax=(wszBuffer ? (wszBuffer + nBufferSize) : (wchar_t *)MAXUINT_PTR);
  int nStringLen;
  BOOL bStringStart=TRUE;

  //Expand environment strings
  nStringLen=ExpandEnvironmentStringsWide(wpString, NULL, 0);

  if (wszSource=(wchar_t *)GlobalAlloc(GPTR, nStringLen * sizeof(wchar_t)))
  {
    ExpandEnvironmentStringsWide(wpString, wszSource, nStringLen);

    //Expand plugin variables
    for (wpSource=wszSource; *wpSource && wpTarget < wpTargetMax;)
    {
      if (bStringStart && *wpSource == L'%')
      {
        if (*++wpSource == L'%')
        {
          ++wpSource;
          if (wszBuffer) *wpTarget=L'%';
          ++wpTarget;
        }
        else if (*wpSource == L'a' || *wpSource == L'A')
        {
          ++wpSource;
          wpTarget+=xstrcpynW(wszBuffer?wpTarget:NULL, wpExeDir, wpTargetMax - wpTarget) - !wszBuffer;
        }
      }
      else
      {
        if (*wpSource != L'\"' && *wpSource != L'\'' && *wpSource != L'`')
          bStringStart=FALSE;
        if (wszBuffer) *wpTarget=*wpSource;
        ++wpTarget;
        ++wpSource;
      }
    }
    if (wpTarget < wpTargetMax)
    {
      if (wszBuffer)
        *wpTarget=L'\0';
      else
        ++wpTarget;
    }
    GlobalFree((HGLOBAL)wszSource);
  }
  return (int)(wpTarget - wszBuffer);
}

int GetCurFile(wchar_t *wszFile, int nMaxFile)
{
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    return (int)xstrcpynW(wszFile, ei.wszFile, nMaxFile) + 1;
  }
  wszFile[0]=L'\0';
  return 0;
}

INT_PTR GetEditText(HWND hWnd, wchar_t **wpText)
{
  GETTEXTRANGE gtr;
  INT_PTR nTextLen;

  gtr.cpMin=0;
  gtr.cpMax=-1;

  if (nTextLen=SendMessage(hMainWnd, AKD_GETTEXTRANGEW, (WPARAM)hWnd, (LPARAM)&gtr))
    *wpText=(wchar_t *)gtr.pText;
  else
    CopyWideStr(L"", -1, wpText);
  return nTextLen;
}

INT_PTR CopyWideStr(const wchar_t *wpSrc, INT_PTR nSrcLen, wchar_t **wppDst)
{
  wchar_t *wszDst=*wppDst;

  if (nSrcLen == -1)
    nSrcLen=xstrlenW(wpSrc);
  if (wszDst)
    FreeWideStr(&wszDst);
  if (wszDst=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nSrcLen + 1) * sizeof(wchar_t)))
    xstrcpynW(wszDst, wpSrc, nSrcLen + 1);
  *wppDst=wszDst;
  return nSrcLen;
}

BOOL FreeWideStr(wchar_t **wppWideStr)
{
  if (wppWideStr && *wppWideStr && !GlobalFree((HGLOBAL)*wppWideStr))
  {
    *wppWideStr=NULL;
    return TRUE;
  }
  return FALSE;
}

void ShowStandardEditMenu(HWND hWnd, HMENU hMenu, BOOL bMouse)
{
  POINT pt;
  CHARRANGE64 cr;
  int nCmd;

  if (!bMouse && SendMessage(hWnd, AEM_GETCARETPOS, (WPARAM)&pt, 0))
  {
    pt.y+=(int)SendMessage(hWnd, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
    ClientToScreen(hWnd, &pt);
  }
  else GetCursorPos(&pt);

  SendMessage(hWnd, EM_EXGETSEL64, 0, (LPARAM)&cr);
  EnableMenuItem(hMenu, IDM_EDIT_UNDO, SendMessage(hWnd, EM_CANUNDO, 0, 0)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_REDO, SendMessage(hWnd, EM_CANREDO, 0, 0)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_PASTE, SendMessage(hWnd, EM_CANPASTE, 0, 0)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_CUT, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_CLEAR, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_COPY, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);

  nCmd=TrackPopupMenu(hMenu, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

  if (nCmd == IDM_EDIT_UNDO)
    SendMessage(hWnd, EM_UNDO, 0, 0);
  else if (nCmd == IDM_EDIT_REDO)
    SendMessage(hWnd, EM_REDO, 0, 0);
  else if (nCmd == IDM_EDIT_CUT)
    SendMessage(hWnd, WM_CUT, 0, 0);
  else if (nCmd == IDM_EDIT_COPY)
    SendMessage(hWnd, WM_COPY, 0, 0);
  else if (nCmd == IDM_EDIT_PASTE)
    SendMessage(hWnd, WM_PASTE, 0, 0);
  else if (nCmd == IDM_EDIT_CLEAR)
    SendMessage(hWnd, WM_CLEAR, 0, 0);
  else if (nCmd == IDM_EDIT_SELECTALL)
    SendMessage(hWnd, EM_SETSEL, 0, -1);
}

DWORD ScrollCaret(HWND hWnd)
{
  AESCROLLTOPOINT stp;
  DWORD dwScrollResult;

  //Test scroll to caret
  stp.dwFlags=AESC_TEST|AESC_POINTCARET|AESC_OFFSETCHARX|AESC_OFFSETCHARY;
  stp.nOffsetX=1;
  stp.nOffsetY=0;
  dwScrollResult=(DWORD)SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);

  //Scroll to caret
  stp.dwFlags=AESC_POINTCARET;
  stp.nOffsetX=3;
  stp.nOffsetY=2;
  if (dwScrollResult & AECSE_SCROLLEDX)
    stp.dwFlags|=AESC_OFFSETRECTDIVX;
  if (dwScrollResult & AECSE_SCROLLEDY)
    stp.dwFlags|=AESC_OFFSETRECTDIVY;
  return (DWORD)SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);
}


//// Options

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData)
{
  PLUGINOPTIONW po;

  po.pOptionName=pOptionName;
  po.dwType=dwType;
  po.lpData=lpData;
  po.dwData=dwData;
  return SendMessage(hMainWnd, AKD_OPTIONW, (WPARAM)hOptions, (LPARAM)&po);
}

void ReadOptions(DWORD dwFlags)
{
  HANDLE hOptions;
  int nSize;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    if ((nSize=(int)WideOption(hOptions, L"ToolBarText", PO_BINARY, NULL, 0)) > 0)
    {
      if (wszToolBarText=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize))
      {
        WideOption(hOptions, L"ToolBarText", PO_BINARY, (LPBYTE)wszToolBarText, nSize);
      }
    }

    WideOption(hOptions, L"BigIcons", PO_DWORD, (LPBYTE)&nBigIcons, sizeof(DWORD));
    WideOption(hOptions, L"FlatButtons", PO_DWORD, (LPBYTE)&bFlatButtons, sizeof(DWORD));
    WideOption(hOptions, L"IconsBit", PO_DWORD, (LPBYTE)&nIconsBit, sizeof(DWORD));
    WideOption(hOptions, L"GrayedIcons", PO_DWORD, (LPBYTE)&nGrayedIcons, sizeof(DWORD));
    WideOption(hOptions, L"ToolbarSide", PO_DWORD, (LPBYTE)&nToolbarSide, sizeof(DWORD));
    WideOption(hOptions, L"SidePriority", PO_DWORD, (LPBYTE)&nSidePriority, sizeof(DWORD));
    WideOption(hOptions, L"ArrowOverlay", PO_DWORD, (LPBYTE)&nArrowOverlay, sizeof(DWORD));
    WideOption(hOptions, L"RowList", PO_STRING, (LPBYTE)wszRowList, MAX_PATH * sizeof(wchar_t));
    WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }

  //Default menus
  if (!wszToolBarText) wszToolBarText=GetDefaultMenu(STRID_DEFAULTMENU);
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_LISTTEXT)
    {
      WideOption(hOptions, L"ToolBarText", PO_BINARY, (LPBYTE)wszToolBarText, ((int)xstrlenW(wszToolBarText) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_SETTINGS)
    {
      WideOption(hOptions, L"BigIcons", PO_DWORD, (LPBYTE)&nBigIcons, sizeof(DWORD));
      WideOption(hOptions, L"FlatButtons", PO_DWORD, (LPBYTE)&bFlatButtons, sizeof(DWORD));
      WideOption(hOptions, L"IconsBit", PO_DWORD, (LPBYTE)&nIconsBit, sizeof(DWORD));
      WideOption(hOptions, L"GrayedIcons", PO_DWORD, (LPBYTE)&nGrayedIcons, sizeof(DWORD));
      WideOption(hOptions, L"ToolbarSide", PO_DWORD, (LPBYTE)&nToolbarSide, sizeof(DWORD));
      WideOption(hOptions, L"SidePriority", PO_DWORD, (LPBYTE)&nSidePriority, sizeof(DWORD));
      WideOption(hOptions, L"ArrowOverlay", PO_DWORD, (LPBYTE)&nArrowOverlay, sizeof(DWORD));
      WideOption(hOptions, L"RowList", PO_STRING, (LPBYTE)wszRowList, ((int)xstrlenW(wszRowList) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_RECT)
    {
      WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
    }

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

wchar_t* GetDefaultMenu(int nStringID)
{
  wchar_t *wszMenuText;
  DWORD dwSize;

  dwSize=(DWORD)xprintfW(NULL, L"%s", GetLangStringW(wLangModule, nStringID));
  if (wszMenuText=(wchar_t *)GlobalAlloc(GMEM_FIXED, dwSize * sizeof(wchar_t)))
    xprintfW(wszMenuText, L"%s", GetLangStringW(wLangModule, nStringID));
  return wszMenuText;
}

const char* GetLangStringA(LANGID wLangID, int nStringID)
{
  static char szStringBuf[MAX_PATH];

  WideCharToMultiByte(CP_ACP, 0, GetLangStringW(wLangID, nStringID), -1, szStringBuf, MAX_PATH, NULL, NULL);
  return szStringBuf;
}

const wchar_t* GetLangStringW(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_SETUP)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0438\x0442\x044C\x002E\x002E\x002E";
    if (nStringID == STRID_AUTOLOAD)
      return L"\x0022\x0025\x0073\x0022\x0020\x043D\x0435\x0020\x043F\x043E\x0434\x0434\x0435\x0440\x0436\x0438\x0432\x0430\x0435\x0442\x0020\x0430\x0432\x0442\x043E\x0437\x0430\x0433\x0440\x0443\x0437\x043A\x0443\x0020\x0028\x0022\x002B\x0022\x0020\x043F\x0435\x0440\x0435\x0434\x0020\x0022\x0043\x0061\x006C\x006C\x0022\x0029\x002E";
    if (nStringID == STRID_BIGICONS)
      return L"\x0411\x043E\x043B\x044C\x0448\x0438\x0435\x0020\x0438\x043A\x043E\x043D\x043A\x0438";
    if (nStringID == STRID_FLATBUTTONS)
      return L"\x041F\x043B\x043E\x0441\x043A\x0438\x0435\x0020\x043A\x043D\x043E\x043F\x043A\x0438";
    if (nStringID == STRID_16BIT)
      return L"\x0031\x0036\x002D\x0431\x0438\x0442";
    if (nStringID == STRID_32BIT)
      return L"\x0033\x0032\x002D\x0431\x0438\x0442\x0430";
    if (nStringID == STRID_SIDE)
      return L"\x0421\x0442\x043E\x0440\x043E\x043D\x0430";
    if (nStringID == STRID_ROWS)
      return L"\x0420\x044F\x0434\x044B (1,2...)";
    if (nStringID == STRID_PARSEMSG_UNKNOWNSPECIAL)
      return L"\x041D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x044B\x0439\x0020\x0441\x043F\x0435\x0446\x0438\x0430\x043B\x044C\x043D\x044B\x0439\x0020\x043F\x0443\x043D\x043A\x0442 \"%s\".";
    if (nStringID == STRID_PARSEMSG_UNKNOWNMETHOD)
      return L"\x041D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x044B\x0439\x0020\x043C\x0435\x0442\x043E\x0434 \"%0.s%s\".";
    if (nStringID == STRID_PARSEMSG_METHODALREADYDEFINED)
      return L"\x041C\x0435\x0442\x043E\x0434\x0020\x0443\x0436\x0435\x0020\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x002E";
    if (nStringID == STRID_PARSEMSG_NOMETHOD)
      return L"\x042D\x043B\x0435\x043C\x0435\x043D\x0442\x0020\x043D\x0435\x0020\x0438\x0441\x043F\x043E\x043B\x044C\x0437\x0443\x0435\x0442\x0020\x043C\x0435\x0442\x043E\x0434\x0430\x0020\x0434\x043B\x044F\x0020\x0432\x044B\x043F\x043E\x043B\x043D\x0435\x043D\x0438\x044F\x002E";
    if (nStringID == STRID_PARSEMSG_WRONGPARAMCOUNT)
      return L"\x041D\x0435\x0432\x0435\x0440\x043D\x043E\x0435\x0020\x043A\x043E\x043B\x0438\x0447\x0435\x0441\x0442\x0432\x043E\x0020\x043F\x0430\x0440\x0430\x043C\x0435\x0442\x0440\x043E\x0432\x002E";
    if (nStringID == STRID_PARSEMSG_NOOPENSET)
      return L"\x041D\x0435\x0442\x0020\x043E\x0442\x043A\x0440\x044B\x0432\x0430\x044E\x0449\x0435\x0433\x043E SET().";
    if (nStringID == STRID_PARSEMSG_NOCOMMA)
      return L"\x041D\x0435\x0442\x0020\x0437\x0430\x043F\x044F\x0442\x043E\x0439 \",\".";
    if (nStringID == STRID_PARSEMSG_NOCLOSEPARENTHESIS)
      return L"\x041D\x0435\x0442\x0020\x0437\x0430\x043A\x0440\x044B\x0432\x0430\x044E\x0449\x0435\x0439\x0020\x0441\x043A\x043E\x0431\x043A\x0438 \")\".";
    if (nStringID == STRID_PARSEMSG_NOEOL)
      return L"\x041E\x0436\x0438\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x043A\x043E\x043D\x0435\x0446\x0020\x0441\x0442\x0440\x043E\x043A\x0438.";
    if (nStringID == STRID_IF_NOCOMMA)
      return L"If: \x043D\x0435\x0442\x0020\x0437\x0430\x043F\x044F\x0442\x043E\x0439 \",\".";
    if (nStringID == STRID_IF_NOCLOSEPARENTHESIS)
      return L"If: \x043D\x0435\x0442\x0020\x0437\x0430\x043A\x0440\x044B\x0432\x0430\x044E\x0449\x0435\x0439\x0020\x0441\x043A\x043E\x0431\x043A\x0438 \")\".";
    if (nStringID == STRID_IF_UNKNOWNOPERATOR)
      return L"If: \x043D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x044B\x0439\x0020\x043E\x043F\x0435\x0440\x0430\x0442\x043E\x0440";
    if (nStringID == STRID_IF_UNKNOWNMETHOD)
      return L"If: \x043D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x044B\x0439\x0020\x043C\x0435\x0442\x043E\x0434.";
    if (nStringID == STRID_IF_CALLERROR)
      return L"If: \x043E\x0448\x0438\x0431\x043A\x0430\x0020\x0432\x044B\x0437\x043E\x0432\x0430.";
    if (nStringID == STRID_IF_NOFALSE)
      return L"If: \x043E\x0442\x0441\x0443\x0442\x0441\x0442\x0432\x0443\x0435\x0442 \":\".";
    if (nStringID == STRID_IF_FOCUSCHANGED)
      return L"If: SET(128) \x0438\x0437\x043C\x0435\x043D\x0438\x043B\x0020\x0444\x043E\x043A\x0443\x0441\x0020\x043E\x043A\x043D\x0430.";
    if (nStringID == STRID_IF_WRONGPARAMCOUNT)
      return L"If: \x043D\x0435\x0432\x0435\x0440\x043D\x043E\x0435\x0020\x043A\x043E\x043B\x0438\x0447\x0435\x0441\x0442\x0432\x043E\x0020\x043F\x0430\x0440\x0430\x043C\x0435\x0442\x0440\x043E\x0432.";
    if (nStringID == STRID_IF_SCRIPTDENIED)
      return L"If: \x0432\x044B\x0437\x043E\x0432\x0020\x0441\x043A\x0440\x0438\x043F\x0442\x0430\x0020\x0437\x0430\x043F\x0440\x0435\x0449\x0451\x043D\x0020\x0432 SET(128, If(...)).";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";

    //File menu
    if (nStringID == STRID_DEFAULTMENU)
      return L"\
\"\" Command(4101) Icon(0)\r\
\"\" Command(4102) Icon(1)\r\
\"\" Command(4103) Icon(2)\r\
\"\" Command(4104) Icon(3)\r\
SEPARATOR1\r\
\"\" Command(4105) Icon(4)\r\
\"\" Command(4106) Icon(5)\r\
SET(1)\r\
    # MDI/PMDI\r\
    \"\" Command(4110) Icon(32)\r\
    \"\" Command(4111) Icon(33)\r\
UNSET(1)\r\
SEPARATOR1\r\
\"\" Command(4108) Icon(6)\r\
\"\" Command(4114) Icon(21)\r\
SEPARATOR1\r\
" L"\
\"\" Command(4153) Icon(7)\r\
\"\" Command(4154) Icon(8)\r\
\"\" Command(4155) Icon(9)\r\
\"\" Command(4156) Icon(25)\r\
SEPARATOR1\r\
\"\" Command(4151) Icon(10)\r\
\"\" Command(4152) Icon(11)\r\
SEPARATOR1\r\
\"\" Command(4158) Icon(12)\r\
\"\" Command(4161) Icon(13)\r\
\"\" Command(4163) Icon(14)\r\
\"\" Command(4183) Icon(26)\r\
SEPARATOR1\r\
" L"\
\"\" Command(4201) Icon(27)\r\
-\"\x0423\x0432\x0435\x043B\x0438\x0447\x0438\x0442\x044C\x0020\x0448\x0440\x0438\x0444\x0442\" Command(4204) Icon(28)\r\
-\"\x0423\x043C\x0435\x043D\x044C\x0448\x0438\x0442\x044C\x0020\x0448\x0440\x0438\x0444\x0442\" Command(4205) Icon(29)\r\
\"\" Command(4202) Icon(30)\r\
SEPARATOR1\r\
\"\" Command(4216) Icon(20)\r\
\"\" Command(4209) Icon(16)\r\
\"\x0420\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x044C\x0020\x043D\x0430\x0020\x0034\x0020\x0447\x0430\x0441\x0442\x0438\" Command(4212) Icon(22)\r\
\"\x0420\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x044C\x0020\x0432\x0435\x0440\x0442\x0438\x043A\x0430\x043B\x044C\x043D\x043E\" Command(4213) Icon(23)\r\
\"\x0420\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x044C\x0020\x0433\x043E\x0440\x0438\x0437\x043E\x043D\x0442\x0430\x043B\x044C\x043D\x043E\" Command(4214) Icon(24)\r\
\"\" Command(4210) Icon(15)\r\
SEPARATOR1\r\
" L"\
\"\" Command(4251) Icon(17)\r\
\"\" Command(4259) Icon(18)\r\
\"\" Command(4260) Icon(19)\r\
SEPARATOR1\r\
" L"\
\"\x0417\x0430\x043F\x0443\x0441\x0442\x0438\x0442\x044C\x0020\x0431\x043B\x043E\x043A\x043D\x043E\x0442\" Exec(\"notepad.exe\") Icon(\"notepad.exe\")\r\
SET(32, \"%a\\AkelFiles\\AkelUpdater.exe\")\r\
    \"\x0417\x0430\x043F\x0443\x0441\x0442\x0438\x0442\x044C\x0020\x043E\x0431\x043D\x043E\x0432\x043B\x0435\x043D\x0438\x0435\" Exec(\"%a\\AkelFiles\\AkelUpdater.exe\") Icon(\"%a\\AkelFiles\\AkelUpdater.exe\")\r\
UNSET(32)\r\
" L"\
\r\
SEPARATOR1\r\
BREAK\r\
\"\x0413\x043B\x0430\x0432\x043D\x043E\x0435\x0020\x043C\x0435\x043D\x044E\" Call(\"ContextMenu::Show\", 2, \"%bl\", \"%bb\") Icon(38)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Coder.dll\")\r\
    \"\x041F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x0438\x0440\x043E\x0432\x0430\x043D\x0438\x0435\" Menu(\"CODER\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 12)\r\
    \"\x041E\x0442\x043C\x0435\x0442\x0438\x0442\x044C\" Menu(\"MARK\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 13)\r\
    \"\x0421\x0438\x043D\x0442\x0430\x043A\x0441\x0438\x0447\x0435\x0441\x043A\x0430\x044F\x0020\x0442\x0435\x043C\x0430\" Menu(\"SYNTAXTHEME\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 4)\r\
    \"\x0426\x0432\x0435\x0442\x043E\x0432\x0430\x044F\x0020\x0442\x0435\x043C\x0430\" Menu(\"COLORTHEME\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 5)\r\
    \"\x041F\x0430\x043D\x0435\x043B\x044C CodeFold\" Call(\"Coder::CodeFold\", 1) Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 3)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\XBrackets.dll\")\r\
    \"\x041F\x0430\x0440\x043D\x044B\x0435\x0020\x0441\x043A\x043E\x0431\x043A\x0438\" +Call(\"XBrackets::Main\") Menu(\"XBRACKETS\") Icon(\"%a\\AkelFiles\\Plugs\\XBrackets.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\SpellCheck.dll\")\r\
    \"\x041F\x0440\x043E\x0432\x0435\x0440\x043A\x0430\x0020\x043E\x0440\x0444\x043E\x0433\x0440\x0430\x0444\x0438\x0438\" +Call(\"SpellCheck::Background\") Menu(\"SPELLCHECK\") Icon(35)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\SpecialChar.dll\")\r\
    \"\x0421\x043F\x0435\x0446\x0438\x0430\x043B\x044C\x043D\x044B\x0435\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x044B\" +Call(\"SpecialChar::Main\") Menu(\"SPECIALCHAR\") Icon(\"%a\\AkelFiles\\Plugs\\SpecialChar.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\LineBoard.dll\")\r\
    \"\x041D\x043E\x043C\x0435\x0440\x0430\x0020\x0441\x0442\x0440\x043E\x043A\x002C\x0020\x0437\x0430\x043A\x043B\x0430\x0434\x043A\x0438\" +Call(\"LineBoard::Main\") Menu(\"LINEBOARD\") Icon(\"%a\\AkelFiles\\Plugs\\LineBoard.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Clipboard.dll\")\r\
    \"\x0411\x0443\x0444\x0435\x0440\x0020\x043E\x0431\x043C\x0435\x043D\x0430\" Menu(\"CLIPBOARD\") Icon(\"%a\\AkelFiles\\Plugs\\Clipboard.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\SaveFile.dll\")\r\
    \"\x0421\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x0438\x0435\x0020\x0444\x0430\x0439\x043B\x0430\" Menu(\"SAVEFILE\") Icon(\"%a\\AkelFiles\\Plugs\\SaveFile.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Log.dll\")\r\
    \"\x041F\x0440\x043E\x0441\x043C\x043E\x0442\x0440\x0020\x043B\x043E\x0433\x0430\" Call(\"Log::Watch\") Menu(\"LOG\") Icon(\"%a\\AkelFiles\\Plugs\\Log.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Explorer.dll\")\r\
    \"\x041F\x0430\x043D\x0435\x043B\x044C\x0020\x043F\x0440\x043E\x0432\x043E\x0434\x043D\x0438\x043A\x0430\" +Call(\"Explorer::Main\") Menu(\"EXPLORE\") Icon(\"%a\\AkelFiles\\Plugs\\Explorer.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\QSearch.dll\")\r\
    \"\x041F\x0430\x043D\x0435\x043B\x044C\x0020\x043F\x043E\x0438\x0441\x043A\x0430\" +Call(\"QSearch::QSearch\") Menu(\"QSEARCH\") Icon(\"%a\\AkelFiles\\Plugs\\QSearch.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Macros.dll\")\r\
    -\"\x041C\x0430\x043A\x0440\x043E\x0441\x044B...\" Call(\"Macros::Main\") Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 0)\r\
    \"\x0417\x0430\x043F\x0438\x0441\x0430\x0442\x044C\" Call(\"Macros::Main\", 2, \"%m\", \"%i\") Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 1)\r\
    \"\x0412\x043E\x0441\x043F\x0440\x043E\x0438\x0437\x0432\x0435\x0441\x0442\x0438\x0020\x043E\x0434\x0438\x043D\x0020\x0440\x0430\x0437\" Call(\"Macros::Main\", 1, \"\", 1) Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 3)\r\
    \"\x0412\x043E\x0441\x043F\x0440\x043E\x0438\x0437\x0432\x0435\x0441\x0442\x0438\x0020\x0434\x043E\x0020\x043A\x043E\x043D\x0446\x0430\" Call(\"Macros::Main\", 3, \"%m\", \"%i\") Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 4)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Scripts.dll\")\r\
    -\"\x0421\x043A\x0440\x0438\x043F\x0442\x044B...\" +Call(\"Scripts::Main\") Menu(\"SCRIPTS\") Icon(\"%a\\AkelFiles\\Plugs\\Scripts.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\RecentFiles.dll\")\r\
    -\"\x041F\x043E\x0441\x043B\x0435\x0434\x043D\x0438\x0435\x0020\x0444\x0430\x0439\x043B\x044B...\" Call(\"RecentFiles::Manage\") Icon(\"%a\\AkelFiles\\Plugs\\RecentFiles.dll\", 0)\r\
UNSET(32)\r\
SET(1)\r\
    # MDI/PMDI\r\
    SET(32, \"%a\\AkelFiles\\Plugs\\Sessions.dll\")\r\
        -\"\x0421\x0435\x0441\x0441\x0438\x0438...\" Call(\"Sessions::Main\") Menu(\"SESSIONS\") Icon(\"%a\\AkelFiles\\Plugs\\Sessions.dll\", 0)\r\
    UNSET(32)\r\
UNSET(1)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Templates.dll\")\r\
    -\"\x0428\x0430\x0431\x043B\x043E\x043D\x044B...\" Call(\"Templates::Open\") Menu(\"TEMPLATES\") Icon(37)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Format.dll\")\r\
    \"\x0421\x043E\x0440\x0442\x0438\x0440\x043E\x0432\x0430\x0442\x044C\x0020\x0441\x0442\x0440\x043E\x043A\x0438\x0020\x043F\x043E\x0020\x0432\x043E\x0437\x0440\x0430\x0441\x0442\x0430\x043D\x0438\x044E\" Call(\"Format::LineSortStrAsc\") Menu(\"FORMAT\") Icon(\"%a\\AkelFiles\\Plugs\\Format.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Scroll.dll\")\r\
    \"\x0412\x0435\x0440\x0442\x0438\x043A\x0430\x043B\x044C\x043D\x0430\x044F\x0020\x0441\x0438\x043D\x0445\x0440\x043E\x043D\x0438\x0437\x0430\x0446\x0438\x044F\" Call(\"Scroll::SyncVert\") Menu(\"SCROLL\") Icon(\"%a\\AkelFiles\\Plugs\\Scroll.dll\", 1)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\HexSel.dll\")\r\
    \"Hex \x043A\x043E\x0434\" +Call(\"HexSel::Main\") Icon(\"%a\\AkelFiles\\Plugs\\HexSel.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Hotkeys.dll\")\r\
    -\"\x0413\x043E\x0440\x044F\x0447\x0438\x0435\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0438...\" +Call(\"Hotkeys::Main\") Menu(\"HOTKEYS\") Icon(\"%a\\AkelFiles\\Plugs\\Hotkeys.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\MinimizeToTray.dll\")\r\
    -\"\x0421\x0432\x0435\x0440\x043D\x0443\x0442\x044C\x0020\x0432\x0020\x0442\x0440\x0435\x0439\" Call(\"MinimizeToTray::Now\") Menu(\"MINIMIZETOTRAY\") Icon(\"%a\\AkelFiles\\Plugs\\MinimizeToTray.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Sounds.dll\")\r\
    \"\x0417\x0432\x0443\x043A\x043E\x0432\x043E\x0439\x0020\x043D\x0430\x0431\x043E\x0440\x0020\x0442\x0435\x043A\x0441\x0442\x0430\" +Call(\"Sounds::Main\") Menu(\"SOUNDS\") Icon(\"%a\\AkelFiles\\Plugs\\Sounds.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Speech.dll\")\r\
    \"\x041C\x0430\x0448\x0438\x043D\x043D\x043E\x0435\x0020\x0447\x0442\x0435\x043D\x0438\x0435\x0020\x0442\x0435\x043A\x0441\x0442\x0430\" +Call(\"Speech::Main\") Icon(\"%a\\AkelFiles\\Plugs\\Speech.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r";

  }
  else
  {
    if (nStringID == STRID_SETUP)
      return L"Customize...";
    if (nStringID == STRID_AUTOLOAD)
      return L"\"%s\" doesn't support autoload (\"+\" before \"Call\").";
    if (nStringID == STRID_BIGICONS)
      return L"Big icons";
    if (nStringID == STRID_FLATBUTTONS)
      return L"Flat buttons";
    if (nStringID == STRID_16BIT)
      return L"16-bit";
    if (nStringID == STRID_32BIT)
      return L"32-bit";
    if (nStringID == STRID_SIDE)
      return L"Side";
    if (nStringID == STRID_ROWS)
      return L"Rows (1,2...)";
    if (nStringID == STRID_PARSEMSG_UNKNOWNSPECIAL)
      return L"Unknown special item \"%s\".";
    if (nStringID == STRID_PARSEMSG_UNKNOWNMETHOD)
      return L"Unknown method \"%0.s%s\".";
    if (nStringID == STRID_PARSEMSG_METHODALREADYDEFINED)
      return L"Method already defined.";
    if (nStringID == STRID_PARSEMSG_NOMETHOD)
      return L"The element does not use method for execution.";
    if (nStringID == STRID_PARSEMSG_WRONGPARAMCOUNT)
      return L"Wrong number of parameters.";
    if (nStringID == STRID_PARSEMSG_NOOPENSET)
      return L"No opening SET().";
    if (nStringID == STRID_PARSEMSG_NOCOMMA)
      return L"No comma \",\".";
    if (nStringID == STRID_PARSEMSG_NOCLOSEPARENTHESIS)
      return L"No close parenthesis \")\".";
    if (nStringID == STRID_PARSEMSG_NOEOL)
      return L"End of line expected.";
    if (nStringID == STRID_IF_NOCOMMA)
      return L"If: no comma \",\".";
    if (nStringID == STRID_IF_NOCLOSEPARENTHESIS)
      return L"If: no close parenthesis \")\".";
    if (nStringID == STRID_IF_UNKNOWNOPERATOR)
      return L"If: unknown operator.";
    if (nStringID == STRID_IF_UNKNOWNMETHOD)
      return L"If: unknown method.";
    if (nStringID == STRID_IF_CALLERROR)
      return L"If: call error.";
    if (nStringID == STRID_IF_NOFALSE)
      return L"If: missing \":\".";
    if (nStringID == STRID_IF_FOCUSCHANGED)
      return L"If: SET(128) changed window focus.";
    if (nStringID == STRID_IF_WRONGPARAMCOUNT)
      return L"If: wrong number of parameters.";
    if (nStringID == STRID_IF_SCRIPTDENIED)
      return L"If: script execution in SET(128, If(...)) is denied.";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";

    //File menu
    if (nStringID == STRID_DEFAULTMENU)
      return L"\
\"\" Command(4101) Icon(0)\r\
\"\" Command(4102) Icon(1)\r\
\"\" Command(4103) Icon(2)\r\
\"\" Command(4104) Icon(3)\r\
SEPARATOR1\r\
\"\" Command(4105) Icon(4)\r\
\"\" Command(4106) Icon(5)\r\
SET(1)\r\
    # MDI/PMDI\r\
    \"\" Command(4110) Icon(32)\r\
    \"\" Command(4111) Icon(33)\r\
UNSET(1)\r\
SEPARATOR1\r\
\"\" Command(4108) Icon(6)\r\
\"\" Command(4114) Icon(21)\r\
SEPARATOR1\r\
" L"\
\"\" Command(4153) Icon(7)\r\
\"\" Command(4154) Icon(8)\r\
\"\" Command(4155) Icon(9)\r\
\"\" Command(4156) Icon(25)\r\
SEPARATOR1\r\
\"\" Command(4151) Icon(10)\r\
\"\" Command(4152) Icon(11)\r\
SEPARATOR1\r\
\"\" Command(4158) Icon(12)\r\
\"\" Command(4161) Icon(13)\r\
\"\" Command(4163) Icon(14)\r\
\"\" Command(4183) Icon(26)\r\
SEPARATOR1\r\
" L"\
\"\" Command(4201) Icon(27)\r\
-\"Increase font\" Command(4204) Icon(28)\r\
-\"Decrease font\" Command(4205) Icon(29)\r\
\"\" Command(4202) Icon(30)\r\
SEPARATOR1\r\
\"\" Command(4216) Icon(20)\r\
\"\" Command(4209) Icon(16)\r\
\"Split into four panes\" Command(4212) Icon(22)\r\
\"Split vertically\" Command(4213) Icon(23)\r\
\"Split horizontally\" Command(4214) Icon(24)\r\
\"\" Command(4210) Icon(15)\r\
SEPARATOR1\r\
" L"\
\"\" Command(4251) Icon(17)\r\
\"\" Command(4259) Icon(18)\r\
\"\" Command(4260) Icon(19)\r\
SEPARATOR1\r\
" L"\
\"Run Notepad\" Exec(\"notepad.exe\") Icon(\"notepad.exe\")\r\
SET(32, \"%a\\AkelFiles\\AkelUpdater.exe\")\r\
    \"Run AkelUpdater\" Exec(\"%a\\AkelFiles\\AkelUpdater.exe\") Icon(\"%a\\AkelFiles\\AkelUpdater.exe\")\r\
UNSET(32)\r\
" L"\
\r\
SEPARATOR1\r\
BREAK\r\
\"Main menu\" Call(\"ContextMenu::Show\", 2, \"%bl\", \"%bb\") Icon(38)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Coder.dll\")\r\
    \"Programming\" Menu(\"CODER\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 12)\r\
    \"Mark\" Menu(\"MARK\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 13)\r\
    \"Syntax theme\" Menu(\"SYNTAXTHEME\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 4)\r\
    \"Color theme\" Menu(\"COLORTHEME\") Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 5)\r\
    \"CodeFold panel\" Call(\"Coder::CodeFold\", 1) Icon(\"%a\\AkelFiles\\Plugs\\Coder.dll\", 3)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\XBrackets.dll\")\r\
    \"Brackets\" +Call(\"XBrackets::Main\") Menu(\"XBRACKETS\") Icon(\"%a\\AkelFiles\\Plugs\\XBrackets.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\SpellCheck.dll\")\r\
    \"Spell check\" +Call(\"SpellCheck::Background\") Menu(\"SPELLCHECK\") Icon(35)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\SpecialChar.dll\")\r\
    \"Special characters\" +Call(\"SpecialChar::Main\") Menu(\"SPECIALCHAR\") Icon(\"%a\\AkelFiles\\Plugs\\SpecialChar.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\LineBoard.dll\")\r\
    \"Line numbers, bookmarks\" +Call(\"LineBoard::Main\") Menu(\"LINEBOARD\") Icon(\"%a\\AkelFiles\\Plugs\\LineBoard.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Clipboard.dll\")\r\
    \"Clipboard\" Menu(\"CLIPBOARD\") Icon(\"%a\\AkelFiles\\Plugs\\Clipboard.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\SaveFile.dll\")\r\
    \"File saving\" Menu(\"SAVEFILE\") Icon(\"%a\\AkelFiles\\Plugs\\SaveFile.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Log.dll\")\r\
    \"Log view\" Call(\"Log::Watch\") Menu(\"LOG\") Icon(\"%a\\AkelFiles\\Plugs\\Log.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Explorer.dll\")\r\
    \"Explorer panel\" +Call(\"Explorer::Main\") Menu(\"EXPLORE\") Icon(\"%a\\AkelFiles\\Plugs\\Explorer.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\QSearch.dll\")\r\
    \"Search panel\" +Call(\"QSearch::QSearch\") Menu(\"QSEARCH\") Icon(\"%a\\AkelFiles\\Plugs\\QSearch.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Macros.dll\")\r\
    -\"Macros...\" Call(\"Macros::Main\") Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 0)\r\
    \"Record\" Call(\"Macros::Main\", 2, \"%m\", \"%i\") Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 1)\r\
    \"Play once\" Call(\"Macros::Main\", 1, \"\", 1) Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 3)\r\
    \"Play to the end\" Call(\"Macros::Main\", 3, \"%m\", \"%i\") Icon(\"%a\\AkelFiles\\Plugs\\Macros.dll\", 4)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Scripts.dll\")\r\
    -\"Scripts...\" +Call(\"Scripts::Main\") Menu(\"SCRIPTS\") Icon(\"%a\\AkelFiles\\Plugs\\Scripts.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\RecentFiles.dll\")\r\
   -\"Recent files...\" Call(\"RecentFiles::Manage\") Icon(\"%a\\AkelFiles\\Plugs\\RecentFiles.dll\", 0)\r\
UNSET(32)\r\
SET(1)\r\
    # MDI/PMDI\r\
    SET(32, \"%a\\AkelFiles\\Plugs\\Sessions.dll\")\r\
        -\"Sessions...\" Call(\"Sessions::Main\") Menu(\"SESSIONS\") Icon(\"%a\\AkelFiles\\Plugs\\Sessions.dll\", 0)\r\
    UNSET(32)\r\
UNSET(1)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Templates.dll\")\r\
    -\"Templates...\" Call(\"Templates::Open\") Menu(\"TEMPLATES\") Icon(37)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Format.dll\")\r\
    \"Sort lines by string ascending\" Call(\"Format::LineSortStrAsc\") Menu(\"FORMAT\") Icon(\"%a\\AkelFiles\\Plugs\\Format.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Scroll.dll\")\r\
    \"Vertical synchronization\" Call(\"Scroll::SyncVert\") Menu(\"SCROLL\") Icon(\"%a\\AkelFiles\\Plugs\\Scroll.dll\", 1)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\HexSel.dll\")\r\
    \"Hex code\" +Call(\"HexSel::Main\") Icon(\"%a\\AkelFiles\\Plugs\\HexSel.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Hotkeys.dll\")\r\
    -\"Hotkeys...\" +Call(\"Hotkeys::Main\") Menu(\"HOTKEYS\") Icon(\"%a\\AkelFiles\\Plugs\\Hotkeys.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\MinimizeToTray.dll\")\r\
    -\"Minimize to tray\" Call(\"MinimizeToTray::Now\") Menu(\"MINIMIZETOTRAY\") Icon(\"%a\\AkelFiles\\Plugs\\MinimizeToTray.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r\
" L"\
SET(32, \"%a\\AkelFiles\\Plugs\\Sounds.dll\")\r\
    \"Sound typing\" +Call(\"Sounds::Main\") Menu(\"SOUNDS\") Icon(\"%a\\AkelFiles\\Plugs\\Sounds.dll\", 0)\r\
UNSET(32)\r\
SET(32, \"%a\\AkelFiles\\Plugs\\Speech.dll\")\r\
    \"Machine reading\" +Call(\"Speech::Main\") Icon(\"%a\\AkelFiles\\Plugs\\Speech.dll\", 0)\r\
UNSET(32)\r\
SEPARATOR1\r";

  }
  return L"";
}

BOOL IsExtCallParamValid(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return TRUE;
  return FALSE;
}

INT_PTR GetExtCallParam(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return *(((INT_PTR *)lParam) + nIndex);
  return 0;
}

BOOL SetExtCallParam(LPARAM lParam, int nIndex, INT_PTR nData)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
  {
    *(((INT_PTR *)lParam) + nIndex)=nData;
    return TRUE;
  }
  return FALSE;
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hInstanceDLL=pd->hInstanceDLL;
  hInstanceEXE=pd->hInstanceEXE;
  hMainWnd=pd->hMainWnd;
  hMdiClient=pd->hMdiClient;
  hMainMenu=pd->hMainMenu;
  hMenuRecentFiles=pd->hMenuRecentFiles;
  hPopupEdit=GetSubMenu(pd->hPopupMenu, MENU_POPUP_EDIT);
  hMainIcon=pd->hMainIcon;
  hGlobalAccel=pd->hGlobalAccel;
  bOldWindows=pd->bOldWindows;
  nMDI=pd->nMDI;
  wLangModule=PRIMARYLANGID(pd->wLangModule);
  hHeap=GetProcessHeap();

  //Initialize WideFunc.h header
  WideInitialize();

  //Is Comctl32.dll 6.10 or higher
  if (LOWORD(pd->dwVerComctl32) > 6 || (LOWORD(pd->dwVerComctl32) == 6 && HIWORD(pd->dwVerComctl32) >= 10))
    bNewComctl32=TRUE;
  else
    bNewComctl32=FALSE;

  //Plugin name
  {
    int i;

    for (i=0; pd->wszFunction[i] != L':'; ++i)
      wszPluginName[i]=pd->wszFunction[i];
    wszPluginName[i]=L'\0';
  }
  xprintfW(wszPluginTitle, GetLangStringW(wLangModule, STRID_PLUGIN), wszPluginName);
  xstrcpynW(wszExeDir, pd->wszAkelDir, MAX_PATH);
  ReadOptions(0);
}

void InitMain()
{
  bInitMain=TRUE;

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

  NewMainProcRetData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROCRET, (WPARAM)NewMainProcRet, (LPARAM)&NewMainProcRetData);

  if (nMDI == WMD_MDI)
  {
    NewFrameProcRetData=NULL;
    SendMessage(hMainWnd, AKD_SETFRAMEPROCRET, (WPARAM)NewMainProcRet, (LPARAM)&NewFrameProcRetData);
  }
}

void UninitMain()
{
  bInitMain=FALSE;

  //Save options
  if (dwSaveFlags)
  {
    SaveOptions(dwSaveFlags);
    dwSaveFlags=0;
  }

  //Remove subclass
  if (NewMainProcData)
  {
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
    NewMainProcData=NULL;
  }
  if (NewMainProcRetData)
  {
    SendMessage(hMainWnd, AKD_SETMAINPROCRET, (WPARAM)NULL, (LPARAM)&NewMainProcRetData);
    NewMainProcRetData=NULL;
  }
  if (NewFrameProcRetData)
  {
    SendMessage(hMainWnd, AKD_SETFRAMEPROCRET, (WPARAM)NULL, (LPARAM)&NewFrameProcRetData);
    NewFrameProcRetData=NULL;
  }

  //Destroy toolbar
  DestroyToolbarWindow(TRUE);
  FreeRows(&hRowListStack);
  FreeWideStr(&wszToolBarText);
}

//Entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_DETACH)
  {
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
  }
  return TRUE;
}
