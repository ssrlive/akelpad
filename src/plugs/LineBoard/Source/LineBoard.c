#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include stack functions
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackDelete
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharUpper
#define xmemcpy
#define xmemset
#define xstrlenW
#define xstrcpynW
#define xstrcmpW
#define xstrcmpiW
#define xstrcmpnW
#define xstrrepW
#define xatoiA
#define xatoiW
#define xitoaW
#define xuitoaW
#define hex2decW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define AppendMenuWide
#define CallWindowProcWide
#define CreateFontIndirectWide
#define DialogBoxWide
#define GetWindowLongPtrWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#include "WideFunc.h"

//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"
//*/

//Defines
#define DLLA_LINEBOARD_FORCEUPDATE      1
#define DLLA_LINEBOARD_GETRULERHEIGHT   2
#define DLLA_LINEBOARD_SETRULERHEIGHT   3
#define DLLA_LINEBOARD_SHOWBOARD        4
#define DLLA_LINEBOARD_GETBOARDRECT     11
#define DLLA_LINEBOARD_GETBOOKMARKS     12
#define DLLA_LINEBOARD_SETBOOKMARKS     13
#define DLLA_LINEBOARD_DELALLBOOKMARK   14
#define DLLA_LINEBOARD_SETBOOKMARK      15
#define DLLA_LINEBOARD_DELBOOKMARK      16
#define DLLA_LINEBOARD_BOOKMARKLIST     17
#define DLLA_LINEBOARD_NEXTBOOKMARK     18
#define DLLA_LINEBOARD_PREVBOOKMARK     19
#define DLLA_LINEBOARD_ADDWINDOW        50
#define DLLA_LINEBOARD_DELWINDOW        51
#define DLLA_LINEBOARD_GETWINDOW        52

#define STRID_SHOW                    1
#define STRID_PANEL                   2
#define STRID_TEXTCOLOR               3
#define STRID_BACKGROUNDCOLOR         4
#define STRID_BORDERCOLOR             5
#define STRID_BOOKMARKTEXTCOLOR       6
#define STRID_BOOKMARKBACKGROUNDCOLOR 7
#define STRID_BOOKMARKBORDERCOLOR     8
#define STRID_LINEUNSAVEDCOLOR        9
#define STRID_LINESAVEDCOLOR          10
#define STRID_WIDTH                   11
#define STRID_RULERSCALECOLOR         12
#define STRID_RULERCARETCOLOR         13
#define STRID_RULERHEIGHT             14
#define STRID_CARETWIDTH              15
#define STRID_BOOKMARKS               16
#define STRID_SETBOOKMARK             17
#define STRID_DELBOOKMARK             18
#define STRID_DELALLBOOKMARK          19
#define STRID_BOOKMARKLIST            20
#define STRID_NEXTBOOKMARK            21
#define STRID_PREVBOOKMARK            22
#define STRID_SAVEBOOKMARKS           23
#define STRID_BOOKMARKCHARHEIGHT      24
#define STRID_CODERTHEME              25
#define STRID_LOADFIRST               26
#define STRID_SETTINGS                27
#define STRID_PLUGIN                  28
#define STRID_OK                      29
#define STRID_CANCEL                  30

#define AKDLL_FREEMESSAGELOOP (WM_USER + 50)

#define OF_SETTINGS       0x1

//UpdateEdit flags
#define UE_ERASE        0x01
#define UE_ALLRECT      0x02
#define UE_DRAWRECT     0x04
#define UE_FIRSTPIXEL   0x08
#define UE_UPDATEWINDOW 0x10

//Document open flags
#define DOF_NONE        0x0
#define DOF_OPEN        0x1
#define DOF_REOPEN      0x2

//Save bookmarks
#define MBS_NONE       0
#define MBS_ALLTEXT    1
#define MBS_UNDO       2
#define MBS_REPLACESEL 3

//StackGetBoard flags
#define GB_READ   0x0
#define GB_CREATE 0x1

typedef struct _BOOKMARK {
  struct _BOOKMARK *next;
  struct _BOOKMARK *prev;
  int nLine;
  AEPOINT *lpPoint;
} BOOKMARK;

typedef struct {
  BOOKMARK *first;
  BOOKMARK *last;
  int nCount;
} STACKBOOKMARK;

typedef struct _WINDOWBOARD {
  struct _WINDOWBOARD *next;
  struct _WINDOWBOARD *prev;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  int nBoardWidth;
  int nBoardHeight;
  BOOL bUpdateBookmarks;
  wchar_t wszFile[MAX_PATH];
  STACKBOOKMARK hBookmarkStack;

  //User window
  HWND hWndParent;
  WNDPROC lpOldEditProc;
  WNDPROC lpOldParentProc;
} WINDOWBOARD;

//Coder external call
#define DLLA_CODER_FILLVARLIST      23

//Variable flags
#define VARF_LOWPRIORITY   0x001 //Global variable has low priority.
                                 //Next flags for DLLA_CODER_FILLVARLIST:
#define VARF_EXTSTRING     0x100 //Copy string pointer to (const wchar_t *)CODERTHEMEITEM.nVarValue.
#define VARF_EXTINTCOLOR   0x200 //Copy color integer to (COLORREF)CODERTHEMEITEM.nVarValue or -1 if not color.
#define VARF_EXTLPINTCOLOR 0x400 //Copy color integer to (COLORREF *)CODERTHEMEITEM.nVarValue or -1 if not color.

typedef struct {
  const wchar_t *wpVarName;
  INT_PTR nVarValue;
  DWORD dwVarFlags;         //See VARF_* defines.
} CODERTHEMEITEM;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  CODERTHEMEITEM *cti;
} DLLEXTCODERFILLVARLIST;

//Functions prototypes
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewUserParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewUserEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
BOOL CALLBACK SetBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK DelBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK DelAllBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK BookmarkListProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK NextBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK PrevBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
void UpdateEdit(HWND hWnd, DWORD dwFlags);
void UpdateEditAndClones(HWND hWnd, DWORD dwFlags);
void UpdateEditAll(DWORD dwFlags);
void StackEndBoardAll(HSTACK *hStack);
void StackEndBoard(WINDOWBOARD *lpBoard);
WINDOWBOARD* StackInsertBoard(HSTACK *hStack);
WINDOWBOARD* StackGetBoard(HSTACK *hStack, HWND hWndEdit, AEHDOC hDocEdit, DWORD dwFlags);
WINDOWBOARD* StackGetBoardByUserParent(HSTACK *hStack, HWND hWndParent);
void StackDeleteBoard(HSTACK *hStack, WINDOWBOARD *wb);
void StackFreeBoards(HSTACK *hStack);
BOOKMARK* StackInsertBookmark(WINDOWBOARD *wb, int nLine);
void StackDeleteBookmarkByData(WINDOWBOARD *wb, BOOKMARK *lpBookmark);
BOOL StackDeleteBookmarkByLine(WINDOWBOARD *wb, int nLine);
void StackRemoveDuplicateBookmarks(WINDOWBOARD *wb);
int StackGetBookmark(WINDOWBOARD *wb, int nLine);
int StackGetNextBookmark(WINDOWBOARD *wb, int nLine);
int StackGetPrevBookmark(WINDOWBOARD *wb, int nLine);
void StackGetBookmarkRect(WINDOWBOARD *wb, int nLine, RECT *rc);
void StackSaveMobileBookmarks(WINDOWBOARD *wb);
void StackRestoreMobileBookmarks(WINDOWBOARD *wb, int nBeforeStartLine, int nBeforeEndLine, int nAfterEndLine);
void StackFreeBookmark(WINDOWBOARD *wb);
int ResetBookmarksMovedFlag(WINDOWBOARD *wb);
int GetBookmarksString(WINDOWBOARD *wb, wchar_t *wszString);
int SetBookmarksString(WINDOWBOARD *wb, const wchar_t *wpString);
BOOL SaveRecentFile(WINDOWBOARD *lpBoard);
BOOL RestoreRecentFile(WINDOWBOARD *lpBoard);
void ShowBoookmarksMenu(WINDOWBOARD *lpBoard, int x, int y, BOOL bSettings);
void SetEditRect(AEHDOC hDocEdit, HWND hWndEdit, int nNewWidth, int nOldWidth, int nNewHeight, int nOldHeight);
HWND GetCurEdit();
HWND GetFocusEdit();
void GetCoderColors(HWND hWnd);
int GetMaxNumberWidth(HDC hDC, int *nNumberAverageWidth);
void GetPosFromChar(HWND hWnd, INT_PTR nCharIndex, POINT *pt);
int GetCurrentLine(HWND hWnd);
void SetCurrentLine(HWND hWnd, int nLine);
DWORD ScrollCaret(HWND hWnd);
LRESULT SendToDoc(AEHDOC hDocEdit, HWND hWndEdit, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);
BOOL SetExtCallParam(LPARAM lParam, int nIndex, INT_PTR nData);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszBuffer2[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hMdiClient;
BOOL bOldWindows;
BOOL bOldRichEdit;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
int nInitMain=0;
DWORD dwSaveFlags=0;
DWORD dwDocOpen=DOF_NONE;
BOOL bShowBoard=TRUE;
BOOL bRememberBookmarks=TRUE;
BOOL bBookmarkCharHeight=FALSE;
BOOL bCoderTheme=TRUE;
DWORD dwStatusPosType=0;
HSTACK hWindowStack={0};
FRAMEDATA *lpFrameLostFocus=NULL;
COLORREF crBoardText=RGB(0x00, 0x00, 0x00);
COLORREF crBoardBk=RGB(0xE8, 0xE4, 0xE0);
COLORREF crBoardBorder=RGB(0x80, 0x80, 0x80);
COLORREF crBoardBookmarkText=RGB(0x00, 0x00, 0x00);
COLORREF crBoardBookmarkBk=RGB(0xFF, 0xFF, 0xFF);
COLORREF crBoardBookmarkBorder=RGB(0x00, 0x00, 0x00);
COLORREF crBoardLineUnsaved=RGB(0xF8, 0xFB, 0x25); //RGB(0xFF, 0x00, 0x00);
COLORREF crBoardLineSaved=RGB(0xA5, 0xF1, 0x2E);
COLORREF crBoardRulerScale=RGB(0x80, 0x80, 0x80);
COLORREF crBoardRulerCaret=RGB(0xF8, 0xFB, 0x25);
COLORREF crDrawBoardText;
COLORREF crDrawBoardBk;
COLORREF crDrawBoardBorder;
COLORREF crDrawBoardBookmarkText;
COLORREF crDrawBoardBookmarkBk;
COLORREF crDrawBoardBookmarkBorder;
COLORREF crDrawBoardLineUnsaved;
COLORREF crDrawBoardLineSaved;
COLORREF crDrawBoardRulerScale;
COLORREF crDrawBoardRulerCaret;
CODERTHEMEITEM cti[]={{L"LineBoard_TextColor",           (INT_PTR)&crDrawBoardText,           VARF_EXTLPINTCOLOR},
                      {L"LineBoard_BkColor",             (INT_PTR)&crDrawBoardBk,             VARF_EXTLPINTCOLOR},
                      {L"LineBoard_BorderColor",         (INT_PTR)&crDrawBoardBorder,         VARF_EXTLPINTCOLOR},
                      {L"LineBoard_BookmarkTextColor",   (INT_PTR)&crDrawBoardBookmarkText,   VARF_EXTLPINTCOLOR},
                      {L"LineBoard_BookmarkBkColor",     (INT_PTR)&crDrawBoardBookmarkBk,     VARF_EXTLPINTCOLOR},
                      {L"LineBoard_BookmarkBorderColor", (INT_PTR)&crDrawBoardBookmarkBorder, VARF_EXTLPINTCOLOR},
                      {L"LineBoard_LineUnsavedColor",    (INT_PTR)&crDrawBoardLineUnsaved,    VARF_EXTLPINTCOLOR},
                      {L"LineBoard_LineSavedColor",      (INT_PTR)&crDrawBoardLineSaved,      VARF_EXTLPINTCOLOR},
                      {L"LineBoard_RulerScaleColor",     (INT_PTR)&crDrawBoardRulerScale,     VARF_EXTLPINTCOLOR},
                      {L"LineBoard_RulerCaretColor",     (INT_PTR)&crDrawBoardRulerCaret,     VARF_EXTLPINTCOLOR},
                      {0, 0, 0}};
BOOL bLineUnsavedEnable=TRUE;
BOOL bLineSavedEnable=TRUE;
int nLineModificationWidth=3;
BOOL bRulerEnable=TRUE;
int nRulerHeight=17;
int nRulerCaretWidth=1;
DWORD dwSetBookmark=3117;    //"Alt+Ins"
DWORD dwDelBookmark=3118;    //"Alt+Del"
DWORD dwDelAllBookmark=3374; //"Alt+Shift+Del"
DWORD dwBookmarkList=3108;   //"Alt+Home"
DWORD dwNextBookmark=3106;   //"Alt+PageDown"
DWORD dwPrevBookmark=3105;   //"Alt+PageUp"
PLUGINFUNCTION *pfwSetBookmark=NULL;
PLUGINFUNCTION *pfwDelBookmark=NULL;
PLUGINFUNCTION *pfwDelAllBookmark=NULL;
PLUGINFUNCTION *pfwBookmarkList=NULL;
PLUGINFUNCTION *pfwNextBookmark=NULL;
PLUGINFUNCTION *pfwPrevBookmark=NULL;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;
WNDPROCDATA *NewEditProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="LineBoard";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  BOOL bForceUpdate=FALSE;

  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_LINEBOARD_FORCEUPDATE)
    {
      bForceUpdate=TRUE;
    }
    else
    {
      if (nInitMain)
      {
        if (nAction == DLLA_LINEBOARD_GETRULERHEIGHT)
        {
          int *lpnRulerHeight=NULL;
          BOOL *lpbRulerEnable=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            lpnRulerHeight=(int *)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            lpbRulerEnable=(BOOL *)GetExtCallParam(pd->lParam, 3);

          if (lpnRulerHeight)
          {
            *lpnRulerHeight=nRulerHeight;
            if (lpbRulerEnable)
              *lpbRulerEnable=bRulerEnable;
          }
        }
        else if (nAction == DLLA_LINEBOARD_SETRULERHEIGHT)
        {
          int nNewRulerHeight=-1;
          BOOL bUpdate=FALSE;

          if (IsExtCallParamValid(pd->lParam, 2))
            nNewRulerHeight=(int)GetExtCallParam(pd->lParam, 2);

          if (nNewRulerHeight >= 0)
          {
            if (!nNewRulerHeight || nNewRulerHeight == nRulerHeight)
            {
              if (nRulerHeight)
              {
                //Turn off
                nRulerHeight=0;
                bUpdate=TRUE;
              }
            }
            else
            {
              //Turn on
              nRulerHeight=nNewRulerHeight;
              bUpdate=TRUE;
            }
          }
          else if (nNewRulerHeight == -1)
          {
            bRulerEnable=!bRulerEnable;
            bUpdate=TRUE;
          }

          if (bUpdate)
          {
            dwSaveFlags|=OF_SETTINGS;
            UpdateEditAll(UE_FIRSTPIXEL);
          }
        }
        else if (nAction == DLLA_LINEBOARD_SHOWBOARD)
        {
          BOOL *lpbShowBoard=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            lpbShowBoard=(BOOL *)GetExtCallParam(pd->lParam, 2);

          if (lpbShowBoard)
          {
            *lpbShowBoard=bShowBoard;
          }
          else
          {
            bShowBoard=!bShowBoard;

            dwSaveFlags|=OF_SETTINGS;
            if (bShowBoard)
              UpdateEditAll(UE_FIRSTPIXEL);
            else
              StackEndBoardAll(&hWindowStack);
          }
        }
        else if (nAction == DLLA_LINEBOARD_GETBOARDRECT)
        {
          WINDOWBOARD *lpBoard;
          HWND hWndEdit=NULL;
          AEHDOC hDocEdit=NULL;
          RECT *lpRect=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            hDocEdit=(AEHDOC)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            lpRect=(RECT *)GetExtCallParam(pd->lParam, 4);

          if (lpRect)
          {
            if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_READ))
            {
              GetClientRect(hWndEdit, lpRect);
              lpRect->left=lpBoard->nBoardWidth;
              lpRect->top=lpBoard->nBoardHeight;
            }
          }
        }
        else if (nAction == DLLA_LINEBOARD_GETBOOKMARKS)
        {
          WINDOWBOARD *lpBoard;
          HWND hWndEdit=NULL;
          AEHDOC hDocEdit=NULL;
          wchar_t *wszBookmarks=NULL;
          int *lpnBookmarksLen=NULL;
          int nBookmarksLen=0;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            hDocEdit=(AEHDOC)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            wszBookmarks=(wchar_t *)GetExtCallParam(pd->lParam, 4);
          if (IsExtCallParamValid(pd->lParam, 5))
            lpnBookmarksLen=(int *)GetExtCallParam(pd->lParam, 5);

          if (wszBookmarks || lpnBookmarksLen)
          {
            if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_READ))
              nBookmarksLen=GetBookmarksString(lpBoard, wszBookmarks);
            if (lpnBookmarksLen) *lpnBookmarksLen=nBookmarksLen;
          }
        }
        else if (nAction == DLLA_LINEBOARD_SETBOOKMARKS)
        {
          WINDOWBOARD *lpBoard;
          FRAMEDATA *lpFrame;
          HWND hWndEdit=NULL;
          AEHDOC hDocEdit=NULL;
          unsigned char *pBookmarks=NULL;
          wchar_t *wpBookmarks;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            hDocEdit=(AEHDOC)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            pBookmarks=(unsigned char *)GetExtCallParam(pd->lParam, 4);

          if (pBookmarks)
          {
            if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_CREATE))
            {
              if (pd->dwSupport & PDS_STRANSI)
                wpBookmarks=AllocWide((char *)pBookmarks);
              else
                wpBookmarks=(wchar_t *)pBookmarks;

              if (SetBookmarksString(lpBoard, wpBookmarks))
              {
                if (lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYEDITDOCUMENT, (LPARAM)hDocEdit))
                  xstrcpynW(lpBoard->wszFile, lpFrame->wszFile, MAX_PATH);
                SaveRecentFile(lpBoard);

                if (bShowBoard) UpdateEdit(hWndEdit, UE_FIRSTPIXEL);
              }
              if (pd->dwSupport & PDS_STRANSI)
                FreeWide(wpBookmarks);
            }
          }
        }
        else if (nAction == DLLA_LINEBOARD_DELALLBOOKMARK)
        {
          DelAllBookmarkProc(NULL, pd->lParam, pd->dwSupport);
        }
        else if (nAction == DLLA_LINEBOARD_SETBOOKMARK)
        {
          SetBookmarkProc(NULL, pd->lParam, pd->dwSupport);
        }
        else if (nAction == DLLA_LINEBOARD_DELBOOKMARK)
        {
          DelBookmarkProc(NULL, pd->lParam, pd->dwSupport);
        }
        else if (nAction == DLLA_LINEBOARD_BOOKMARKLIST)
        {
          BookmarkListProc(NULL, pd->lParam, pd->dwSupport);
        }
        else if (nAction == DLLA_LINEBOARD_NEXTBOOKMARK)
        {
          NextBookmarkProc(NULL, pd->lParam, pd->dwSupport);
        }
        else if (nAction == DLLA_LINEBOARD_PREVBOOKMARK)
        {
          PrevBookmarkProc(NULL, pd->lParam, pd->dwSupport);
        }
        else if (nAction == DLLA_LINEBOARD_ADDWINDOW)
        {
          WINDOWBOARD *lpBoard;
          HWND hWndEdit=NULL;
          HWND hWndParent=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);
          hWndParent=GetParent(hWndEdit);

          if (hWndEdit && hWndParent)
          {
            if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, NULL, GB_CREATE))
            {
              lpBoard->hWndParent=hWndParent;
              lpBoard->lpOldEditProc=(WNDPROC)GetWindowLongPtrWide(hWndEdit, GWLP_WNDPROC);
              SetWindowLongPtrWide(hWndEdit, GWLP_WNDPROC, (UINT_PTR)NewUserEditProc);

              lpBoard->lpOldParentProc=(WNDPROC)GetWindowLongPtrWide(hWndParent, GWLP_WNDPROC);
              SetWindowLongPtrWide(hWndParent, GWLP_WNDPROC, (UINT_PTR)NewUserParentProc);
            }
          }
        }
        else if (nAction == DLLA_LINEBOARD_DELWINDOW)
        {
          WINDOWBOARD *lpBoard;
          HWND hWndEdit=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);

          if (hWndEdit)
          {
            if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, NULL, GB_READ))
            {
              StackEndBoard(lpBoard);
              StackDeleteBoard(&hWindowStack, lpBoard);
            }
          }
        }
        else if (nAction == DLLA_LINEBOARD_GETWINDOW)
        {
          HWND hWndEdit=NULL;
          BOOL *lpbFound=NULL;
          BOOL bFound=FALSE;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            lpbFound=(BOOL *)GetExtCallParam(pd->lParam, 3);

          if (hWndEdit && lpbFound)
          {
            if (StackGetBoard(&hWindowStack, hWndEdit, NULL, GB_READ))
              bFound=TRUE;
            *lpbFound=bFound;
          }
        }
      }
      else
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_LOADFIRST), L"LineBoard::Main");
        MessageBoxW(pd->hMainWnd, wszBuffer, L"LineBoard::Main", MB_OK|MB_ICONEXCLAMATION);
      }

      //If plugin already loaded, stay in memory and don't change active status
      if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
      return;
    }
  }

  //Initialize
  if (nInitMain)
  {
    if (bShowBoard) StackEndBoardAll(&hWindowStack);
    StackFreeBoards(&hWindowStack);
    UninitMain();
  }
  else
  {
    InitMain();

    if (!pd->bOnStart)
    {
      if (bShowBoard)
      {
        if (bForceUpdate)
        {
          //Change running state before update window. Because CodeFold received AEN_SETRECT and
          //ask "LineBoard::Main" for RECT only if "LineBoard::Main" running.
          pd->lpPluginFunction->bRunning=TRUE;
          UpdateEditAll(UE_FIRSTPIXEL|UE_UPDATEWINDOW);
        }
        else UpdateEditAll(UE_FIRSTPIXEL);
      }
    }

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) Settings(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hMainWnd, (DLGPROC)SetupDlgProc);

  //If plugin already loaded, stay in memory, but show as non-active
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndShowBoard;
  static HWND hWndBackgroundColorButton;
  static HWND hWndBackgroundColorLabel;
  static HWND hWndTextColorButton;
  static HWND hWndTextColorLabel;
  static HWND hWndBorderColorButton;
  static HWND hWndBorderColorLabel;
  static HWND hWndBookmarkTextColorButton;
  static HWND hWndBookmarkTextColorLabel;
  static HWND hWndBookmarkBackgroundColorButton;
  static HWND hWndBookmarkBackgroundColorLabel;
  static HWND hWndBookmarkBorderColorButton;
  static HWND hWndBookmarkBorderColorLabel;
  static HWND hWndLineUnsavedColorCheck;
  static HWND hWndLineUnsavedColorButton;
  static HWND hWndLineUnsavedColorLabel;
  static HWND hWndLineSavedColorCheck;
  static HWND hWndLineSavedColorButton;
  static HWND hWndLineSavedColorLabel;
  static HWND hWndLineModificationWidthLabel;
  static HWND hWndLineModificationWidthEdit;
  static HWND hWndLineModificationWidthSpin;
  static HWND hWndRulerScaleColorButton;
  static HWND hWndRulerScaleColorLabel;
  static HWND hWndRulerCaretColorButton;
  static HWND hWndRulerCaretColorLabel;
  static HWND hWndRulerEnable;
  static HWND hWndRulerHeightEdit;
  static HWND hWndRulerHeightSpin;
  static HWND hWndRulerCaretWidthLabel;
  static HWND hWndRulerCaretWidthEdit;
  static HWND hWndRulerCaretWidthSpin;
  static HWND hWndSetBookmark;
  static HWND hWndDelBookmark;
  static HWND hWndDelAllBookmark;
  static HWND hWndBookmarkList;
  static HWND hWndNextBookmark;
  static HWND hWndPrevBookmark;
  static HWND hWndRememberBookmark;
  static HWND hWndBookmarkCharHeight;
  static HWND hWndCoderTheme;
  static COLORREF crBoardTextDlg;
  static COLORREF crBoardBkDlg;
  static COLORREF crBoardBorderDlg;
  static COLORREF crBoardBookmarkTextDlg;
  static COLORREF crBoardBookmarkBkDlg;
  static COLORREF crBoardBookmarkBorderDlg;
  static COLORREF crBoardLineUnsavedDlg;
  static COLORREF crBoardLineSavedDlg;
  static COLORREF crBoardRulerScaleDlg;
  static COLORREF crBoardRulerCaretDlg;
  static BOOL bLineUnsavedEnableDlg;
  static BOOL bLineSavedEnableDlg;
  static int nLineModificationWidthDlg;
  static BOOL bRulerEnableDlg;
  static int nRulerHeightDlg;
  static int nRulerCaretWidthDlg;
  static BOOL bShowBoardDlg;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndShowBoard=GetDlgItem(hDlg, IDC_SHOWBOARD);
    hWndBackgroundColorButton=GetDlgItem(hDlg, IDC_BACKGROUNDCOLOR_BUTTON);
    hWndBackgroundColorLabel=GetDlgItem(hDlg, IDC_BACKGROUNDCOLOR_LABEL);
    hWndTextColorButton=GetDlgItem(hDlg, IDC_TEXTCOLOR_BUTTON);
    hWndTextColorLabel=GetDlgItem(hDlg, IDC_TEXTCOLOR_LABEL);
    hWndBorderColorButton=GetDlgItem(hDlg, IDC_BORDERCOLOR_BUTTON);
    hWndBorderColorLabel=GetDlgItem(hDlg, IDC_BORDERCOLOR_LABEL);
    hWndBookmarkTextColorButton=GetDlgItem(hDlg, IDC_BOOKMARKTEXTCOLOR_BUTTON);
    hWndBookmarkTextColorLabel=GetDlgItem(hDlg, IDC_BOOKMARKTEXTCOLOR_LABEL);
    hWndBookmarkBackgroundColorButton=GetDlgItem(hDlg, IDC_BOOKMARKBACKGROUNDCOLOR_BUTTON);
    hWndBookmarkBackgroundColorLabel=GetDlgItem(hDlg, IDC_BOOKMARKBACKGROUNDCOLOR_LABEL);
    hWndBookmarkBorderColorButton=GetDlgItem(hDlg, IDC_BOOKMARKBORDERCOLOR_BUTTON);
    hWndBookmarkBorderColorLabel=GetDlgItem(hDlg, IDC_BOOKMARKBORDERCOLOR_LABEL);
    hWndLineUnsavedColorCheck=GetDlgItem(hDlg, IDC_LINEUNSAVEDCOLOR_CHECK);
    hWndLineUnsavedColorButton=GetDlgItem(hDlg, IDC_LINEUNSAVEDCOLOR_BUTTON);
    hWndLineUnsavedColorLabel=GetDlgItem(hDlg, IDC_LINEUNSAVEDCOLOR_LABEL);
    hWndLineSavedColorCheck=GetDlgItem(hDlg, IDC_LINESAVEDCOLOR_CHECK);
    hWndLineSavedColorButton=GetDlgItem(hDlg, IDC_LINESAVEDCOLOR_BUTTON);
    hWndLineSavedColorLabel=GetDlgItem(hDlg, IDC_LINESAVEDCOLOR_LABEL);
    hWndLineModificationWidthLabel=GetDlgItem(hDlg, IDC_LINEMODIFICATIONWIDTH_LABEL);
    hWndLineModificationWidthEdit=GetDlgItem(hDlg, IDC_LINEMODIFICATIONWIDTH_EDIT);
    hWndLineModificationWidthSpin=GetDlgItem(hDlg, IDC_LINEMODIFICATIONWIDTH_SPIN);
    hWndRulerScaleColorButton=GetDlgItem(hDlg, IDC_RULERSCALECOLOR_BUTTON);
    hWndRulerScaleColorLabel=GetDlgItem(hDlg, IDC_RULERSCALECOLOR_LABEL);
    hWndRulerCaretColorButton=GetDlgItem(hDlg, IDC_RULERCARETCOLOR_BUTTON);
    hWndRulerCaretColorLabel=GetDlgItem(hDlg, IDC_RULERCARETCOLOR_LABEL);
    hWndRulerEnable=GetDlgItem(hDlg, IDC_RULERENABLE);
    hWndRulerHeightEdit=GetDlgItem(hDlg, IDC_RULERHEIGHT_EDIT);
    hWndRulerHeightSpin=GetDlgItem(hDlg, IDC_RULERHEIGHT_SPIN);
    hWndRulerCaretWidthLabel=GetDlgItem(hDlg, IDC_RULERCARETWIDTH_LABEL);
    hWndRulerCaretWidthEdit=GetDlgItem(hDlg, IDC_RULERCARETWIDTH_EDIT);
    hWndRulerCaretWidthSpin=GetDlgItem(hDlg, IDC_RULERCARETWIDTH_SPIN);
    hWndSetBookmark=GetDlgItem(hDlg, IDC_SETBOOKMARK);
    hWndDelBookmark=GetDlgItem(hDlg, IDC_DELBOOKMARK);
    hWndDelAllBookmark=GetDlgItem(hDlg, IDC_DELALLBOOKMARK);
    hWndBookmarkList=GetDlgItem(hDlg, IDC_BOOKMARKLIST);
    hWndNextBookmark=GetDlgItem(hDlg, IDC_NEXTBOOKMARK);
    hWndPrevBookmark=GetDlgItem(hDlg, IDC_PREVBOOKMARK);
    hWndRememberBookmark=GetDlgItem(hDlg, IDC_REMEMBERBOOKMARKS);
    hWndBookmarkCharHeight=GetDlgItem(hDlg, IDC_BOOKMARKCHARHEIGHT);
    hWndCoderTheme=GetDlgItem(hDlg, IDC_CODERTHEME);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_PANEL_GROUP, GetLangStringW(wLangModule, STRID_PANEL));
    SetDlgItemTextWide(hDlg, IDC_SHOWBOARD, GetLangStringW(wLangModule, STRID_SHOW));
    SetDlgItemTextWide(hDlg, IDC_TEXTCOLOR_LABEL, GetLangStringW(wLangModule, STRID_TEXTCOLOR));
    SetDlgItemTextWide(hDlg, IDC_BACKGROUNDCOLOR_LABEL, GetLangStringW(wLangModule, STRID_BACKGROUNDCOLOR));
    SetDlgItemTextWide(hDlg, IDC_BORDERCOLOR_LABEL, GetLangStringW(wLangModule, STRID_BORDERCOLOR));
    SetDlgItemTextWide(hDlg, IDC_BOOKMARKTEXTCOLOR_LABEL, GetLangStringW(wLangModule, STRID_BOOKMARKTEXTCOLOR));
    SetDlgItemTextWide(hDlg, IDC_BOOKMARKBACKGROUNDCOLOR_LABEL, GetLangStringW(wLangModule, STRID_BOOKMARKBACKGROUNDCOLOR));
    SetDlgItemTextWide(hDlg, IDC_BOOKMARKBORDERCOLOR_LABEL, GetLangStringW(wLangModule, STRID_BOOKMARKBORDERCOLOR));
    SetDlgItemTextWide(hDlg, IDC_LINEUNSAVEDCOLOR_LABEL, GetLangStringW(wLangModule, STRID_LINEUNSAVEDCOLOR));
    SetDlgItemTextWide(hDlg, IDC_LINESAVEDCOLOR_LABEL, GetLangStringW(wLangModule, STRID_LINESAVEDCOLOR));
    SetDlgItemTextWide(hDlg, IDC_LINEMODIFICATIONWIDTH_LABEL, GetLangStringW(wLangModule, STRID_WIDTH));
    SetDlgItemTextWide(hDlg, IDC_RULERSCALECOLOR_LABEL, GetLangStringW(wLangModule, STRID_RULERSCALECOLOR));
    SetDlgItemTextWide(hDlg, IDC_RULERCARETCOLOR_LABEL, GetLangStringW(wLangModule, STRID_RULERCARETCOLOR));
    SetDlgItemTextWide(hDlg, IDC_RULERENABLE, GetLangStringW(wLangModule, STRID_RULERHEIGHT));
    SetDlgItemTextWide(hDlg, IDC_RULERCARETWIDTH_LABEL, GetLangStringW(wLangModule, STRID_CARETWIDTH));
    SetDlgItemTextWide(hDlg, IDC_BOOKMARK_GROUP, GetLangStringW(wLangModule, STRID_BOOKMARKS));
    SetDlgItemTextWide(hDlg, IDC_SETBOOKMARK_LABEL, GetLangStringW(wLangModule, STRID_SETBOOKMARK));
    SetDlgItemTextWide(hDlg, IDC_DELBOOKMARK_LABEL, GetLangStringW(wLangModule, STRID_DELBOOKMARK));
    SetDlgItemTextWide(hDlg, IDC_DELALLBOOKMARK_LABEL, GetLangStringW(wLangModule, STRID_DELALLBOOKMARK));
    SetDlgItemTextWide(hDlg, IDC_BOOKMARKLIST_LABEL, GetLangStringW(wLangModule, STRID_BOOKMARKLIST));
    SetDlgItemTextWide(hDlg, IDC_NEXTBOOKMARK_LABEL, GetLangStringW(wLangModule, STRID_NEXTBOOKMARK));
    SetDlgItemTextWide(hDlg, IDC_PREVBOOKMARK_LABEL, GetLangStringW(wLangModule, STRID_PREVBOOKMARK));
    SetDlgItemTextWide(hDlg, IDC_REMEMBERBOOKMARKS, GetLangStringW(wLangModule, STRID_SAVEBOOKMARKS));
    SetDlgItemTextWide(hDlg, IDC_BOOKMARKCHARHEIGHT, GetLangStringW(wLangModule, STRID_BOOKMARKCHARHEIGHT));
    SetDlgItemTextWide(hDlg, IDC_CODERTHEME, GetLangStringW(wLangModule, STRID_CODERTHEME));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndLineModificationWidthSpin, UDM_SETBUDDY, (WPARAM)hWndLineModificationWidthEdit, 0);
    SendMessage(hWndLineModificationWidthSpin, UDM_SETRANGE, 0, MAKELONG(99, 0));
    SetDlgItemInt(hDlg, IDC_LINEMODIFICATIONWIDTH_EDIT, nLineModificationWidth, FALSE);
    SendMessage(hWndRulerHeightSpin, UDM_SETBUDDY, (WPARAM)hWndRulerHeightEdit, 0);
    SendMessage(hWndRulerHeightSpin, UDM_SETRANGE, 0, MAKELONG(99, 0));
    SetDlgItemInt(hDlg, IDC_RULERHEIGHT_EDIT, nRulerHeight, FALSE);
    SendMessage(hWndRulerCaretWidthSpin, UDM_SETBUDDY, (WPARAM)hWndRulerCaretWidthEdit, 0);
    SendMessage(hWndRulerCaretWidthSpin, UDM_SETRANGE, 0, MAKELONG(99, 0));
    SetDlgItemInt(hDlg, IDC_RULERCARETWIDTH_EDIT, nRulerCaretWidth, FALSE);

    if (bShowBoard) SendMessage(hWndShowBoard, BM_SETCHECK, BST_CHECKED, 0);
    if (bLineUnsavedEnable) SendMessage(hWndLineUnsavedColorCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (bLineSavedEnable) SendMessage(hWndLineSavedColorCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (bRulerEnable) SendMessage(hWndRulerEnable, BM_SETCHECK, BST_CHECKED, 0);
    if (bRememberBookmarks) SendMessage(hWndRememberBookmark, BM_SETCHECK, BST_CHECKED, 0);
    if (bBookmarkCharHeight) SendMessage(hWndBookmarkCharHeight, BM_SETCHECK, BST_CHECKED, 0);
    if (bCoderTheme) SendMessage(hWndCoderTheme, BM_SETCHECK, BST_CHECKED, 0);

    InitMain();
    SendMessage(hWndSetBookmark, HKM_SETHOTKEY, dwSetBookmark, 0);
    SendMessage(hWndDelBookmark, HKM_SETHOTKEY, dwDelBookmark, 0);
    SendMessage(hWndDelAllBookmark, HKM_SETHOTKEY, dwDelAllBookmark, 0);
    SendMessage(hWndBookmarkList, HKM_SETHOTKEY, dwBookmarkList, 0);
    SendMessage(hWndNextBookmark, HKM_SETHOTKEY, dwNextBookmark, 0);
    SendMessage(hWndPrevBookmark, HKM_SETHOTKEY, dwPrevBookmark, 0);

    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndSetBookmark, pfwSetBookmark?dwSetBookmark:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndDelBookmark, pfwDelBookmark?dwDelBookmark:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndDelAllBookmark, pfwDelAllBookmark?dwDelAllBookmark:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndBookmarkList, pfwBookmarkList?dwBookmarkList:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndNextBookmark, pfwNextBookmark?dwNextBookmark:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndPrevBookmark, pfwPrevBookmark?dwPrevBookmark:0);
    UninitMain();

    bShowBoardDlg=bShowBoard;
    crBoardTextDlg=crBoardText;
    crBoardBkDlg=crBoardBk;
    crBoardBorderDlg=crBoardBorder;
    crBoardBookmarkTextDlg=crBoardBookmarkText;
    crBoardBookmarkBkDlg=crBoardBookmarkBk;
    crBoardBookmarkBorderDlg=crBoardBookmarkBorder;
    crBoardLineUnsavedDlg=crBoardLineUnsaved;
    crBoardLineSavedDlg=crBoardLineSaved;
    crBoardRulerScaleDlg=crBoardRulerScale;
    crBoardRulerCaretDlg=crBoardRulerCaret;
    bLineUnsavedEnableDlg=bLineUnsavedEnable;
    bLineSavedEnableDlg=bLineSavedEnable;
    nLineModificationWidthDlg=nLineModificationWidth;
    bRulerEnableDlg=bRulerEnable;
    nRulerHeightDlg=nRulerHeight;
    nRulerCaretWidthDlg=nRulerCaretWidth;

    PostMessage(hDlg, WM_COMMAND, IDC_SHOWBOARD, 0);
  }
  else if (uMsg == WM_DRAWITEM)
  {
    DRAWITEMSTRUCT *dis=(DRAWITEMSTRUCT *)lParam;
    HBRUSH hBrush;
    DWORD dwColor=GetSysColor(COLOR_BTNFACE);

    if (wParam == IDC_TEXTCOLOR_BUTTON)
    {
      if (bShowBoardDlg)
        dwColor=crBoardTextDlg;
    }
    else if (wParam == IDC_BACKGROUNDCOLOR_BUTTON)
    {
      if (bShowBoardDlg)
        dwColor=crBoardBkDlg;
    }
    else if (wParam == IDC_BORDERCOLOR_BUTTON)
    {
      if (bShowBoardDlg)
        dwColor=crBoardBorderDlg;
    }
    else if (wParam == IDC_BOOKMARKTEXTCOLOR_BUTTON)
    {
      if (bShowBoardDlg)
        dwColor=crBoardBookmarkTextDlg;
    }
    else if (wParam == IDC_BOOKMARKBACKGROUNDCOLOR_BUTTON)
    {
      if (bShowBoardDlg)
        dwColor=crBoardBookmarkBkDlg;
    }
    else if (wParam == IDC_BOOKMARKBORDERCOLOR_BUTTON)
    {
      if (bShowBoardDlg)
        dwColor=crBoardBookmarkBorderDlg;
    }
    else if (wParam == IDC_LINEUNSAVEDCOLOR_BUTTON)
    {
      if (bShowBoardDlg && bLineUnsavedEnableDlg && nLineModificationWidthDlg)
        dwColor=crBoardLineUnsavedDlg;
    }
    else if (wParam == IDC_LINESAVEDCOLOR_BUTTON)
    {
      if (bShowBoardDlg && bLineSavedEnableDlg && nLineModificationWidthDlg)
        dwColor=crBoardLineSavedDlg;
    }
    else if (wParam == IDC_RULERSCALECOLOR_BUTTON)
    {
      if (bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg)
        dwColor=crBoardRulerScaleDlg;
    }
    else if (wParam == IDC_RULERCARETCOLOR_BUTTON)
    {
      if (bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg && nRulerCaretWidthDlg)
        dwColor=crBoardRulerCaretDlg;
    }

    if (hBrush=CreateSolidBrush(dwColor))
    {
      FillRect(dis->hDC, &dis->rcItem, hBrush);
      if (dis->hwndItem == GetFocus())
        DrawFocusRect(dis->hDC, &dis->rcItem);
      DeleteObject(hBrush);
    }
    return TRUE;
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SHOWBOARD)
    {
      bShowBoardDlg=(BOOL)SendMessage(hWndShowBoard, BM_GETCHECK, 0, 0);
      EnableWindow(hWndTextColorButton, bShowBoardDlg);
      EnableWindow(hWndTextColorLabel, bShowBoardDlg);
      EnableWindow(hWndBackgroundColorButton, bShowBoardDlg);
      EnableWindow(hWndBackgroundColorLabel, bShowBoardDlg);
      EnableWindow(hWndBorderColorButton, bShowBoardDlg);
      EnableWindow(hWndBorderColorLabel, bShowBoardDlg);
      EnableWindow(hWndBookmarkTextColorButton, bShowBoardDlg);
      EnableWindow(hWndBookmarkTextColorLabel, bShowBoardDlg);
      EnableWindow(hWndBookmarkBackgroundColorButton, bShowBoardDlg);
      EnableWindow(hWndBookmarkBackgroundColorLabel, bShowBoardDlg);
      EnableWindow(hWndBookmarkBorderColorButton, bShowBoardDlg);
      EnableWindow(hWndBookmarkBorderColorLabel, bShowBoardDlg);
      EnableWindow(hWndLineUnsavedColorCheck, bShowBoardDlg);
      EnableWindow(hWndLineUnsavedColorButton, (bShowBoardDlg && bLineUnsavedEnableDlg && nLineModificationWidthDlg));
      EnableWindow(hWndLineUnsavedColorLabel, (bShowBoardDlg && bLineUnsavedEnableDlg && nLineModificationWidthDlg));
      EnableWindow(hWndLineSavedColorCheck, bShowBoardDlg);
      EnableWindow(hWndLineSavedColorButton, (bShowBoardDlg && bLineSavedEnableDlg && nLineModificationWidthDlg));
      EnableWindow(hWndLineSavedColorLabel, (bShowBoardDlg && bLineSavedEnableDlg && nLineModificationWidthDlg));
      EnableWindow(hWndLineModificationWidthLabel, bShowBoardDlg);
      EnableWindow(hWndLineModificationWidthEdit, bShowBoardDlg);
      EnableWindow(hWndLineModificationWidthSpin, bShowBoardDlg);
      EnableWindow(hWndRulerScaleColorButton, (bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg));
      EnableWindow(hWndRulerScaleColorLabel, (bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg));
      EnableWindow(hWndRulerCaretColorButton, (bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg && nRulerCaretWidthDlg));
      EnableWindow(hWndRulerCaretColorLabel, (bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg && nRulerCaretWidthDlg));
      EnableWindow(hWndRulerEnable, bShowBoardDlg);
      EnableWindow(hWndRulerHeightEdit, bShowBoardDlg && bRulerEnableDlg);
      EnableWindow(hWndRulerHeightSpin, bShowBoardDlg && bRulerEnableDlg);
      EnableWindow(hWndRulerCaretWidthLabel, bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg);
      EnableWindow(hWndRulerCaretWidthEdit, bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg);
      EnableWindow(hWndRulerCaretWidthSpin, bShowBoardDlg && bRulerEnableDlg && nRulerHeightDlg);
      InvalidateRect(hDlg, NULL, FALSE);
    }
    else if (LOWORD(wParam) == IDC_LINEUNSAVEDCOLOR_CHECK)
    {
      bLineUnsavedEnableDlg=(BOOL)SendMessage(hWndLineUnsavedColorCheck, BM_GETCHECK, 0, 0);
      PostMessage(hDlg, WM_COMMAND, IDC_SHOWBOARD, 0);
    }
    else if (LOWORD(wParam) == IDC_LINESAVEDCOLOR_CHECK)
    {
      bLineSavedEnableDlg=(BOOL)SendMessage(hWndLineSavedColorCheck, BM_GETCHECK, 0, 0);
      PostMessage(hDlg, WM_COMMAND, IDC_SHOWBOARD, 0);
    }
    else if (LOWORD(wParam) == IDC_LINEMODIFICATIONWIDTH_EDIT)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        nLineModificationWidthDlg=GetDlgItemInt(hDlg, IDC_LINEMODIFICATIONWIDTH_EDIT, NULL, FALSE);
        PostMessage(hDlg, WM_COMMAND, IDC_SHOWBOARD, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_RULERENABLE)
    {
      bRulerEnableDlg=(BOOL)SendMessage(hWndRulerEnable, BM_GETCHECK, 0, 0);
      PostMessage(hDlg, WM_COMMAND, IDC_SHOWBOARD, 0);
    }
    else if (LOWORD(wParam) == IDC_RULERHEIGHT_EDIT)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        nRulerHeightDlg=GetDlgItemInt(hDlg, IDC_RULERHEIGHT_EDIT, NULL, FALSE);
        PostMessage(hDlg, WM_COMMAND, IDC_SHOWBOARD, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_RULERCARETWIDTH_EDIT)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        nRulerCaretWidthDlg=GetDlgItemInt(hDlg, IDC_RULERCARETWIDTH_EDIT, NULL, FALSE);
        PostMessage(hDlg, WM_COMMAND, IDC_SHOWBOARD, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_TEXTCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_BACKGROUNDCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_BORDERCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_BOOKMARKTEXTCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_BOOKMARKBACKGROUNDCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_BOOKMARKBORDERCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_RULERSCALECOLOR_BUTTON ||
             LOWORD(wParam) == IDC_RULERCARETCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_LINEUNSAVEDCOLOR_BUTTON ||
             LOWORD(wParam) == IDC_LINESAVEDCOLOR_BUTTON)
    {
      static COLORREF crCustColors[16];
      COLORREF *lpcrColor=NULL;
      HWND hWndButton=NULL;

      if (LOWORD(wParam) == IDC_TEXTCOLOR_BUTTON)
      {
        lpcrColor=&crBoardTextDlg;
        hWndButton=hWndTextColorButton;
      }
      else if (LOWORD(wParam) == IDC_BACKGROUNDCOLOR_BUTTON)
      {
        lpcrColor=&crBoardBkDlg;
        hWndButton=hWndBackgroundColorButton;
      }
      else if (LOWORD(wParam) == IDC_BORDERCOLOR_BUTTON)
      {
        lpcrColor=&crBoardBorderDlg;
        hWndButton=hWndBorderColorButton;
      }
      else if (LOWORD(wParam) == IDC_BOOKMARKTEXTCOLOR_BUTTON)
      {
        lpcrColor=&crBoardBookmarkTextDlg;
        hWndButton=hWndBookmarkTextColorButton;
      }
      else if (LOWORD(wParam) == IDC_BOOKMARKBACKGROUNDCOLOR_BUTTON)
      {
        lpcrColor=&crBoardBookmarkBkDlg;
        hWndButton=hWndBookmarkBackgroundColorButton;
      }
      else if (LOWORD(wParam) == IDC_BOOKMARKBORDERCOLOR_BUTTON)
      {
        lpcrColor=&crBoardBookmarkBorderDlg;
        hWndButton=hWndBookmarkBorderColorButton;
      }
      else if (LOWORD(wParam) == IDC_LINEUNSAVEDCOLOR_BUTTON)
      {
        lpcrColor=&crBoardLineUnsavedDlg;
        hWndButton=hWndLineUnsavedColorButton;
      }
      else if (LOWORD(wParam) == IDC_LINESAVEDCOLOR_BUTTON)
      {
        lpcrColor=&crBoardLineSavedDlg;
        hWndButton=hWndLineSavedColorButton;
      }
      else if (LOWORD(wParam) == IDC_RULERSCALECOLOR_BUTTON)
      {
        lpcrColor=&crBoardRulerScaleDlg;
        hWndButton=hWndRulerScaleColorButton;
      }
      else if (LOWORD(wParam) == IDC_RULERCARETCOLOR_BUTTON)
      {
        lpcrColor=&crBoardRulerCaretDlg;
        hWndButton=hWndRulerCaretColorButton;
      }

      if (bOldWindows)
      {
        CHOOSECOLORA ccA;

        xmemset(&ccA, 0, sizeof(CHOOSECOLORA));
        ccA.lStructSize  =sizeof(CHOOSECOLORA);
        ccA.hwndOwner    =hDlg;
        ccA.lpCustColors =crCustColors;
        ccA.Flags        =CC_FULLOPEN|CC_RGBINIT;
        ccA.rgbResult    =*lpcrColor;

        if (ChooseColorA(&ccA))
        {
          *lpcrColor=ccA.rgbResult;
          InvalidateRect(hWndButton, NULL, FALSE);
        }
      }
      else
      {
        CHOOSECOLORW ccW;

        xmemset(&ccW, 0, sizeof(CHOOSECOLORW));
        ccW.lStructSize  =sizeof(CHOOSECOLORW);
        ccW.hwndOwner    =hDlg;
        ccW.lpCustColors =crCustColors;
        ccW.Flags        =CC_FULLOPEN|CC_RGBINIT;
        ccW.rgbResult    =*lpcrColor;

        if (ChooseColorW(&ccW))
        {
          *lpcrColor=ccW.rgbResult;
          InvalidateRect(hWndButton, NULL, FALSE);
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      crBoardText=crBoardTextDlg;
      crBoardBk=crBoardBkDlg;
      crBoardBorder=crBoardBorderDlg;
      crBoardBookmarkText=crBoardBookmarkTextDlg;
      crBoardBookmarkBk=crBoardBookmarkBkDlg;
      crBoardBookmarkBorder=crBoardBookmarkBorderDlg;
      crBoardLineUnsaved=crBoardLineUnsavedDlg;
      crBoardLineSaved=crBoardLineSavedDlg;
      crBoardRulerScale=crBoardRulerScaleDlg;
      crBoardRulerCaret=crBoardRulerCaretDlg;
      bLineUnsavedEnable=bLineUnsavedEnableDlg;
      bLineSavedEnable=bLineSavedEnableDlg;
      nLineModificationWidth=nLineModificationWidthDlg;
      bRulerEnable=bRulerEnableDlg;
      nRulerHeight=nRulerHeightDlg;
      nRulerCaretWidth=nRulerCaretWidthDlg;

      dwSetBookmark=(WORD)SendMessage(hWndSetBookmark, HKM_GETHOTKEY, 0, 0);
      dwDelBookmark=(WORD)SendMessage(hWndDelBookmark, HKM_GETHOTKEY, 0, 0);
      dwDelAllBookmark=(WORD)SendMessage(hWndDelAllBookmark, HKM_GETHOTKEY, 0, 0);
      dwBookmarkList=(WORD)SendMessage(hWndBookmarkList, HKM_GETHOTKEY, 0, 0);
      dwNextBookmark=(WORD)SendMessage(hWndNextBookmark, HKM_GETHOTKEY, 0, 0);
      dwPrevBookmark=(WORD)SendMessage(hWndPrevBookmark, HKM_GETHOTKEY, 0, 0);

      if (bShowBoardDlg != bShowBoard)
      {
        bShowBoard=bShowBoardDlg;

        if (bShowBoard)
          UpdateEditAll(UE_FIRSTPIXEL);
        else
          StackEndBoardAll(&hWindowStack);
      }
      bRememberBookmarks=(BOOL)SendMessage(hWndRememberBookmark, BM_GETCHECK, 0, 0);
      bBookmarkCharHeight=(BOOL)SendMessage(hWndBookmarkCharHeight, BM_GETCHECK, 0, 0);
      bCoderTheme=(BOOL)SendMessage(hWndCoderTheme, BM_GETCHECK, 0, 0);

      SaveOptions(OF_SETTINGS);
      dwSaveFlags=0;
      EndDialog(hDlg, 0);

      if (nInitMain)
      {
        UninitMain();
        InitMain();

        if (bShowBoard) UpdateEditAll(UE_ALLRECT);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Destroy plugin icon
    DestroyIcon(hPluginIcon);
  }
  return FALSE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult=0;

  if (uMsg == AKDN_CONTEXTMENU)
  {
    NCONTEXTMENU *ncm=(NCONTEXTMENU *)lParam;

    if (ncm->uType == NCM_EDIT)
    {
      WINDOWBOARD *lpBoard;
      POINT ptClient;

      if (lpBoard=StackGetBoard(&hWindowStack, ncm->hWnd, NULL, GB_READ))
      {
        if (lpBoard->nBoardWidth || lpBoard->nBoardHeight)
        {
          ptClient=ncm->pt;
          ScreenToClient(ncm->hWnd, &ptClient);
          if ((ptClient.x >= 0 && ptClient.x < lpBoard->nBoardWidth) ||
              (ptClient.y >= 0 && ptClient.y < lpBoard->nBoardHeight))
          {
            ncm->bProcess=FALSE;
            ShowBoookmarksMenu(lpBoard, ncm->pt.x, ncm->pt.y, TRUE);
          }
        }
      }
    }
  }
  else if (uMsg == AKDN_OPENDOCUMENT_START)
  {
    NOPENDOCUMENT *nod=(NOPENDOCUMENT *)lParam;

    dwDocOpen=DOF_OPEN;
    if (*nod->dwFlags & OD_REOPEN)
      dwDocOpen|=DOF_REOPEN;
  }
  else if (uMsg == AKDN_OPENDOCUMENT_FINISH)
  {
    dwDocOpen=DOF_NONE;
    //if (bShowBoard) UpdateEdit((HWND)wParam, UE_FIRSTPIXEL);
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_FINISH)
  {
    WINDOWBOARD *lpBoard;
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (bRememberBookmarks)
    {
      if (lpBoard=StackGetBoard(&hWindowStack, lpFrame->ei.hWndEdit, lpFrame->ei.hDocEdit, GB_READ))
      {
        if (ResetBookmarksMovedFlag(lpBoard))
          SaveRecentFile(lpBoard);
      }
    }

    if (nLineModificationWidth && (bLineUnsavedEnable || bLineSavedEnable))
    {
      if (bShowBoard) UpdateEditAndClones(lpFrame->ei.hWndEdit, UE_FIRSTPIXEL);
    }
  }
  else if (uMsg == AKDN_EDIT_ONFINISH)
  {
    WINDOWBOARD *lpBoard;

    if (lpBoard=StackGetBoard(&hWindowStack, (HWND)wParam, NULL, GB_READ))
      StackDeleteBoard(&hWindowStack, lpBoard);
  }
  if (ParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
    return lResult;

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult=0;

  if (ParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
    return lResult;

  //Call next procedure
  return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewUserParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  WINDOWBOARD *lpBoard;
  LRESULT lResult;

  if (lpBoard=StackGetBoardByUserParent(&hWindowStack, hWnd))
  {
    lResult=(LRESULT)lpBoard->hWndEdit;
    if (ParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
      return lResult;

    //Call next procedure
    return CallWindowProcWide(lpBoard->lpOldParentProc, hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

BOOL CALLBACK ParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  static int nMobileBookmarksStartLine;
  static int nMobileBookmarksEndLine;
  static int nMobileBookmarksSave;
  static BOOL bUndoRedo;

  if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT || (HWND)*lResult)
    {
      if (((NMHDR *)lParam)->code == AEN_TEXTCHANGING)
      {
        if (dwDocOpen == DOF_NONE || (dwDocOpen & DOF_REOPEN))
        {
          AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;
          WINDOWBOARD *lpBoard;

          nMobileBookmarksSave=MBS_NONE;
          bUndoRedo=FALSE;

          if (((aentc->dwType & AETCT_SETTEXT) || (aentc->dwType & AETCT_STREAMIN)) && (dwDocOpen & DOF_REOPEN))
          {
            nMobileBookmarksStartLine=0;
            nMobileBookmarksEndLine=-1;
            nMobileBookmarksSave=MBS_ALLTEXT;
          }
          else if ((aentc->dwType & AETCT_UNDO) || (aentc->dwType & AETCT_REDO))
          {
            nMobileBookmarksStartLine=-1;
            nMobileBookmarksEndLine=-1;
            nMobileBookmarksSave=MBS_UNDO;
            bUndoRedo=TRUE;
          }
          else if (!aentc->bColumnSel && (aentc->dwType & AETCT_REPLACESEL) && aentc->crSel.ciMin.nLine < aentc->crSel.ciMax.nLine)
          {
            nMobileBookmarksStartLine=aentc->crSel.ciMin.nLine;
            nMobileBookmarksEndLine=aentc->crSel.ciMax.nLine;
            nMobileBookmarksSave=MBS_REPLACESEL;
          }

          if (nMobileBookmarksSave)
          {
            //SaveMobileBookmarks
            HWND hWndMaster;
            HWND hWndEdit;
            int nCloneCount=0;

            if (hWndMaster=(HWND)SendMessage(aentc->hdr.hwndFrom, AEM_GETMASTER, 0, 0))
              hWndEdit=hWndMaster;
            else
              hWndEdit=aentc->hdr.hwndFrom;

            do
            {
              if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, NULL, GB_READ))
              {
                StackSaveMobileBookmarks(lpBoard);
                lpBoard->bUpdateBookmarks=TRUE;
              }
            }
            while (hWndMaster && (hWndEdit=(HWND)SendMessage(hWndMaster, AEM_GETCLONE, nCloneCount++, 0)));
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTDELETEBEGIN ||
               ((NMHDR *)lParam)->code == AEN_TEXTDELETEEND)
      {
        if (bUndoRedo)
        {
          AENTEXTDELETE *aentd=(AENTEXTDELETE *)lParam;

          //Bookmarks not removed after single line modification
          if (aentd->crAkelRange.ciMin.nLine < aentd->crAkelRange.ciMax.nLine)
          {
            if (nMobileBookmarksStartLine == -1)
            {
              nMobileBookmarksStartLine=aentd->crAkelRange.ciMin.nLine;
              nMobileBookmarksEndLine=aentd->crAkelRange.ciMax.nLine;
            }
            else
            {
              if (aentd->crAkelRange.ciMin.nLine < nMobileBookmarksStartLine)
                nMobileBookmarksStartLine=aentd->crAkelRange.ciMin.nLine;
              if (aentd->crAkelRange.ciMax.nLine > nMobileBookmarksEndLine)
                nMobileBookmarksEndLine=aentd->crAkelRange.ciMax.nLine;
            }
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGED)
      {
        AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;

        if (nMobileBookmarksStartLine != -1 &&
            ((nMobileBookmarksSave == MBS_REPLACESEL || nMobileBookmarksSave == MBS_UNDO) ?
                nMobileBookmarksEndLine == aentc->crSel.ciMax.nLine : nMobileBookmarksSave))
        {
          //RestoreMobileBookmarks
          WINDOWBOARD *lpBoard;
          HWND hWndMaster;
          HWND hWndEdit;
          int nCloneCount=0;

          if (hWndMaster=(HWND)SendMessage(aentc->hdr.hwndFrom, AEM_GETMASTER, 0, 0))
            hWndEdit=hWndMaster;
          else
            hWndEdit=aentc->hdr.hwndFrom;

          do
          {
            if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, NULL, GB_READ))
            {
              if (lpBoard->bUpdateBookmarks)
              {
                lpBoard->bUpdateBookmarks=FALSE;
                StackRestoreMobileBookmarks(lpBoard, nMobileBookmarksStartLine, nMobileBookmarksEndLine, aentc->crSel.ciMax.nLine);
              }
            }
          }
          while (hWndMaster && (hWndEdit=(HWND)SendMessage(hWndMaster, AEM_GETCLONE, nCloneCount++, 0)));
        }
      }
    }
  }
  return FALSE;
}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (EditMessages(hWnd, uMsg, wParam, lParam, &lResult))
    return lResult;

  //Call next procedure
  return NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewUserEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  WINDOWBOARD *lpBoard;
  LRESULT lResult;

  if (lpBoard=StackGetBoard(&hWindowStack, hWnd, NULL, GB_READ))
  {
    if (EditMessages(hWnd, uMsg, wParam, lParam, &lResult))
      return lResult;

    if (uMsg == WM_DESTROY)
    {
      StackDeleteBoard(&hWindowStack, lpBoard);
      return 0;
    }

    //Call next procedure
    return CallWindowProcWide(lpBoard->lpOldEditProc, hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

BOOL CALLBACK EditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (uMsg == WM_PAINT)
  {
    char szLineNumber[MAX_PATH];
    WINDOWBOARD *lpBoard;
    HDC hDC;
    HFONT hFont;
    HFONT hFontOld;
    HFONT hFontRuler;
    RECT rcBoard;
    RECT rcClient;
    int nLineCount=0;
    int nLineUnwrapCount=0;
    int nWordWrap=0;
    int nLineNumberLen;
    int nMaxNumberWidth;
    int nNumberAverageWidth;
    int nOldBoardWidth;
    int nOldBoardHeight;
    BOOL bRedrawBoard=FALSE;

    //Get board or create if necessary
    lpBoard=StackGetBoard(&hWindowStack, hWnd, NULL, GB_CREATE);

    if (dwDocOpen == DOF_NONE)
    {
      //Restore bookmarks only when board visible
      if (!bShowBoard || lpBoard->nBoardWidth)
        RestoreRecentFile(lpBoard);
    }
    StackRemoveDuplicateBookmarks(lpBoard);

    if (bShowBoard)
    {
      //if (dwDocOpen == DOF_NONE)
      {
        GetClientRect(hWnd, &rcClient);
        xmemcpy(&rcBoard, &rcClient, sizeof(RECT));
        rcBoard.top=lpBoard->nBoardHeight;
        rcBoard.right=lpBoard->nBoardWidth;

        nLineCount=(int)SendMessage(hWnd, EM_GETLINECOUNT, 0, 0);
        nLineUnwrapCount=nLineCount;
        dwStatusPosType=(DWORD)SendMessage(hMainWnd, AKD_GETMAININFO, MI_STATUSPOSTYPE, 0);
        if (!(dwStatusPosType & SPT_LINEWRAP))
        {
          nWordWrap=(int)SendMessage(hWnd, AEM_GETWORDWRAP, 0, (LPARAM)NULL);
          if (nWordWrap)
            nLineUnwrapCount=(int)SendMessage(hWnd, AEM_GETUNWRAPLINE, nLineCount - 1, 0) + 1;
        }
        bRedrawBoard=TRUE;

        if (hDC=GetDC(hWnd))
        {
          if (!(hFont=(HFONT)SendMessage(hWnd, WM_GETFONT, 0, 0)))
            hFont=(HFONT)GetStockObject(SYSTEM_FONT);
          hFontOld=(HFONT)SelectObject(hDC, hFont);

          //Check board width
          nLineNumberLen=wsprintfA(szLineNumber, "%d", nLineUnwrapCount);
          nMaxNumberWidth=GetMaxNumberWidth(hDC, &nNumberAverageWidth);

          if (nLineNumberLen * nMaxNumberWidth + nNumberAverageWidth != lpBoard->nBoardWidth ||
              (bRulerEnable?nRulerHeight:0) != lpBoard->nBoardHeight)
          {
            nOldBoardWidth=lpBoard->nBoardWidth;
            lpBoard->nBoardWidth=nLineNumberLen * nMaxNumberWidth + nNumberAverageWidth;
            nOldBoardHeight=lpBoard->nBoardHeight;
            lpBoard->nBoardHeight=(bRulerEnable?nRulerHeight:0);
            SetEditRect(NULL, hWnd, lpBoard->nBoardWidth, nOldBoardWidth, lpBoard->nBoardHeight, nOldBoardHeight);
            UpdateWindow(hWnd);
            bRedrawBoard=FALSE;
          }
          if (hFontOld) SelectObject(hDC, hFontOld);
          ReleaseDC(hWnd, hDC);
        }

        //WM_PAINT was sended after UpdateWindow, so don't redraw edit.
        if (!bRedrawBoard)
        {
          *lResult=0;
          return TRUE;
        }
      }

      if (lpBoard->lpOldEditProc)
        *lResult=CallWindowProcWide(lpBoard->lpOldEditProc, hWnd, uMsg, wParam, lParam);
      else
        *lResult=NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);

      if (bRedrawBoard)
      {
        AECHARINDEX ciFirstLine;
        AECHARINDEX ciLastLine;
        CHARRANGE64 crLineRange;
        POINT ptFirstChar;
        POINT64 ptScrollPos;
        POINT ptCaretPos;
        HDC hBufferDC;
        HBITMAP hBitmap;
        HBITMAP hBitmapOld;
        HRGN hRgn;
        HRGN hRgnOld;
        HBRUSH hBrushBoard;
        HBRUSH hBrushBookmark;
        HBRUSH hBrushBookmarkOld;
        HPEN hPenPanelBorder;
        HPEN hPenBookmarkBorder;
        HPEN hPenModifiedUnsaved=NULL;
        HPEN hPenModifiedSaved=NULL;
        HPEN hPenRulerScale=NULL;
        HPEN hPenRulerCaret=NULL;
        HPEN hPenOld;
        RECT rcDraw;
        RECT rcBrush;
        POINT ptTextOut;
        SIZE sizeLineNumber;
        int nCharHeight;
        int nBookmarkHeight;
        int nCharInLine;
        int nUnwrappedLine=0;
        int nLineModified;
        int i;
        BOOL bWrapped;

        if (hDC=GetDC(hWnd))
        {
          GetCoderColors(hWnd);

          //Create buffer DC to avoid flashing
          hBufferDC=CreateCompatibleDC(hDC);
          hBitmap=CreateCompatibleBitmap(hDC, rcClient.right, rcClient.bottom);
          hBitmapOld=(HBITMAP)SelectObject(hBufferDC, hBitmap);

          hFont=(HFONT)SendMessage(hWnd, AEM_GETFONT, AEGF_CURRENT, 0);
          hFontOld=(HFONT)SelectObject(hBufferDC, hFont);
          hBrushBoard=CreateSolidBrush(crDrawBoardBk);
          hBrushBookmark=CreateSolidBrush(crDrawBoardBookmarkBk);
          hPenPanelBorder=CreatePen(PS_SOLID, 0, crDrawBoardBorder);
          hPenBookmarkBorder=CreatePen(PS_SOLID, 0, crDrawBoardBookmarkBorder);
          if (bLineUnsavedEnable && nLineModificationWidth)
            hPenModifiedUnsaved=CreatePen(PS_SOLID, 0, crDrawBoardLineUnsaved);
          if (bLineSavedEnable && nLineModificationWidth)
            hPenModifiedSaved=CreatePen(PS_SOLID, 0, crDrawBoardLineSaved);
          if (bRulerEnable && nRulerHeight)
            hPenRulerScale=CreatePen(PS_SOLID, 0, crDrawBoardRulerScale);
          if (bRulerEnable && nRulerHeight)
            hPenRulerCaret=CreatePen(PS_SOLID, 0, crDrawBoardRulerCaret);
          SetBkColor(hBufferDC, crDrawBoardBk);

          //Some variables could be changed after UpdateWindow
          rcBoard.top=lpBoard->nBoardHeight;
          rcBoard.right=lpBoard->nBoardWidth;
          GetMaxNumberWidth(hBufferDC, &nNumberAverageWidth);

          //Get char height
          nCharHeight=(int)SendMessage(hWnd, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
          if (bBookmarkCharHeight)
            nBookmarkHeight=nCharHeight - (int)SendMessage(hWnd, AEM_GETLINEGAP, 0, 0);
          else
            nBookmarkHeight=nCharHeight;

          //Draw board edge
          hPenOld=(HPEN)SelectObject(hBufferDC, hPenPanelBorder);
          MoveToEx(hBufferDC, rcBoard.right - 1, rcBoard.top, NULL);
          LineTo(hBufferDC, rcBoard.right - 1, rcBoard.bottom);

          //Draw lines
          SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstLine);
          SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTVISIBLELINE, (LPARAM)&ciLastLine);
          ptFirstChar.x=0;
          ptFirstChar.y=(int)SendMessage(hWnd, AEM_VPOSFROMLINE, AECT_CLIENT, ciFirstLine.nLine);

          if (nLineModificationWidth && (hPenModifiedSaved || hPenModifiedUnsaved))
          {
            crLineRange.cpMin=SendMessage(hWnd, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&ciFirstLine);
            crLineRange.cpMax=crLineRange.cpMin + ciFirstLine.lpLine->nLineLen;
            if (ciFirstLine.lpLine->nLineBreak != AELB_WRAP)
              ++crLineRange.cpMax;
          }

          if (rcBoard.top < ptFirstChar.y)
          {
            rcDraw.left=0;
            rcDraw.top=rcBoard.top;
            rcDraw.right=rcBoard.right - 1;
            rcDraw.bottom=ptFirstChar.y;
            FillRect(hBufferDC, &rcDraw, hBrushBoard);
          }
          rcDraw.left=0;
          rcDraw.top=ptFirstChar.y;
          rcDraw.right=rcBoard.right - 1;
          rcDraw.bottom=rcBoard.bottom;

          while (ciFirstLine.nLine <= ciLastLine.nLine && rcDraw.top < rcDraw.bottom)
          {
            if (!SendMessage(hWnd, AEM_ISLINECOLLAPSED, ciFirstLine.nLine, 0))
            {
              if (nWordWrap)
                nUnwrappedLine=(int)SendMessage(hWnd, AEM_GETUNWRAPLINE, ciFirstLine.nLine, 0);
              else
                nUnwrappedLine=ciFirstLine.nLine;
              nLineNumberLen=wsprintfA(szLineNumber, "%d", nUnwrappedLine + 1);
              if (!GetTextExtentPoint32A(hBufferDC, szLineNumber, nLineNumberLen, &sizeLineNumber))
                break;

              ptTextOut.x=rcBoard.right - sizeLineNumber.cx - (nNumberAverageWidth / 2);
              ptTextOut.y=rcDraw.top;
              bWrapped=FALSE;

              if (nWordWrap)
              {
                if (ciFirstLine.lpLine->prev && ciFirstLine.lpLine->prev->nLineBreak == AELB_WRAP)
                {
                  //Erase line space
                  rcBrush.left=0;
                  rcBrush.right=rcDraw.right;
                  rcBrush.top=rcDraw.top;
                  rcBrush.bottom=rcDraw.top + nCharHeight;
                  FillRect(hBufferDC, &rcBrush, hBrushBoard);
                  bWrapped=TRUE;
                }
              }

              if (StackGetBookmark(lpBoard, ciFirstLine.nLine) != -1)
              {
                if (!bWrapped)
                {
                  //Erase line space
                  rcBrush.left=0;
                  rcBrush.right=rcDraw.right;
                  rcBrush.top=rcDraw.top;
                  rcBrush.bottom=rcDraw.top + nCharHeight;
                  FillRect(hBufferDC, &rcBrush, hBrushBoard);
                }

                //Draw bookmark
                SelectObject(hBufferDC, hPenBookmarkBorder);
                hBrushBookmarkOld=(HBRUSH)SelectObject(hBufferDC, hBrushBookmark);
                RoundRect(hBufferDC, 0, rcDraw.top - 1, rcDraw.right, rcDraw.top + nBookmarkHeight, nBookmarkHeight / 3, nBookmarkHeight / 3);
                if (hBrushBookmarkOld) SelectObject(hBufferDC, hBrushBookmarkOld);

                if (!bWrapped)
                {
                  //Draw number
                  SetBkMode(hBufferDC, TRANSPARENT);
                  SetTextColor(hBufferDC, crDrawBoardBookmarkText);
                  TextOutA(hBufferDC, ptTextOut.x, ptTextOut.y, szLineNumber, nLineNumberLen);
                  SetBkMode(hBufferDC, OPAQUE);
                }
              }
              else
              {
                if (!bWrapped)
                {
                  //Erase space before number
                  rcBrush.left=0;
                  rcBrush.right=ptTextOut.x;
                  rcBrush.top=rcDraw.top;
                  rcBrush.bottom=rcDraw.top + nCharHeight;
                  FillRect(hBufferDC, &rcBrush, hBrushBoard);

                  //Draw number
                  SetTextColor(hBufferDC, crDrawBoardText);
                  rcBrush.left=ptTextOut.x;
                  rcBrush.right=ptTextOut.x + sizeLineNumber.cx;
                  ExtTextOutA(hBufferDC, ptTextOut.x, ptTextOut.y, ETO_OPAQUE, &rcBrush, szLineNumber, nLineNumberLen, NULL);

                  //Erase space after number
                  rcBrush.left=ptTextOut.x + sizeLineNumber.cx;
                  rcBrush.right=rcDraw.right;
                  FillRect(hBufferDC, &rcBrush, hBrushBoard);
                }
              }

              if (nLineModificationWidth && (hPenModifiedSaved || hPenModifiedUnsaved))
              {
                nLineModified=(int)SendMessage(hWnd, AEM_ISRANGEMODIFIED, 0, (LPARAM)&crLineRange);

                if ((nLineModified == AEIRM_MODIFIEDSAVED && hPenModifiedSaved) ||
                    (nLineModified == AEIRM_MODIFIEDUNSAVED && hPenModifiedUnsaved))
                {
                  if (nLineModified == AEIRM_MODIFIEDSAVED)
                    SelectObject(hBufferDC, hPenModifiedSaved);
                  else if (nLineModified == AEIRM_MODIFIEDUNSAVED)
                    SelectObject(hBufferDC, hPenModifiedUnsaved);

                  //Draw line modification flag
                  for (i=nLineModificationWidth; i > 0; --i)
                  {
                    MoveToEx(hBufferDC, rcBoard.right - i - 1, rcDraw.top, NULL);
                    LineTo(hBufferDC, rcBoard.right - i - 1, rcDraw.top + nCharHeight);
                  }
                }
              }

              //Next line
              rcDraw.top+=nCharHeight;
              if (ciFirstLine.lpLine=ciFirstLine.lpLine->next)
              {
                if (nLineModificationWidth && (hPenModifiedSaved || hPenModifiedUnsaved))
                {
                  crLineRange.cpMin=crLineRange.cpMax;
                  crLineRange.cpMax=crLineRange.cpMin + ciFirstLine.lpLine->nLineLen;
                  if (ciFirstLine.lpLine->nLineBreak != AELB_WRAP)
                    ++crLineRange.cpMax;
                }
              }
              ++ciFirstLine.nLine;
            }
            else
            {
              if (!SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTUNCOLLAPSEDLINE, (LPARAM)&ciFirstLine))
                break;

              if (nLineModificationWidth && (hPenModifiedSaved || hPenModifiedUnsaved))
              {
                crLineRange.cpMin=SendMessage(hWnd, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&ciFirstLine);
                crLineRange.cpMax=crLineRange.cpMin + ciFirstLine.lpLine->nLineLen;
                if (ciFirstLine.lpLine->nLineBreak != AELB_WRAP)
                  ++crLineRange.cpMax;
              }
            }
          }

          if (rcDraw.top < rcDraw.bottom)
          {
            FillRect(hBufferDC, &rcDraw, hBrushBoard);
          }

          //Copy from buffer DC
          hRgn=CreateRectRgn(rcBoard.left, rcBoard.top, rcBoard.right, rcBoard.bottom);
          hRgnOld=(HRGN)SelectObject(hDC, hRgn);
          BitBlt(hDC, rcBoard.left, rcBoard.top, rcBoard.right - rcBoard.left, rcBoard.bottom - rcBoard.top, hBufferDC, rcBoard.left, rcBoard.top, SRCCOPY);
          if (hRgnOld) SelectObject(hDC, hRgnOld);
          DeleteObject(hRgn);

          if (hPenOld) SelectObject(hBufferDC, hPenOld);
          if (hFontOld) SelectObject(hBufferDC, hFontOld);

          //Draw ruler
          if (bRulerEnable && nRulerHeight && lpBoard->nBoardHeight)
          {
            LOGFONTA lfFont;
            int nPosX;
            int nOffset;
            BOOL bOnCaret=FALSE;

            rcBoard.left=0;
            rcBoard.right=rcClient.right;
            rcBoard.top=0;
            rcBoard.bottom=lpBoard->nBoardHeight;

            //Create ruler font
            GetObjectA(hFont, sizeof(LOGFONTA), &lfFont);
            lfFont.lfHeight=-MulDiv(lpBoard->nBoardHeight, GetDeviceCaps(hBufferDC, LOGPIXELSY), 72) / 2;
            hFontRuler=(HFONT)CreateFontIndirectA(&lfFont);
            SelectObject(hBufferDC, hFontRuler);

            //Erase ruler space
            FillRect(hBufferDC, &rcBoard, hBrushBoard);

            //Draw ruler edge
            hPenOld=(HPEN)SelectObject(hBufferDC, hPenPanelBorder);
            MoveToEx(hBufferDC, lpBoard->nBoardWidth - 1, rcBoard.bottom - 1, NULL);
            LineTo(hBufferDC, rcBoard.right, rcBoard.bottom - 1);

            SelectObject(hBufferDC, hPenRulerScale);
            SendMessage(hWnd, AEM_GETRECT, 0, (LPARAM)&rcDraw);
            SendMessage(hWnd, AEM_GETSCROLLPOS, 0, (LPARAM)&ptScrollPos);
            SendMessage(hWnd, AEM_GETCARETPOS, (WPARAM)&ptCaretPos, 0);
            nCharInLine=(int)(ptScrollPos.x / nNumberAverageWidth);

            for (nPosX=rcDraw.left - (int)(ptScrollPos.x % nNumberAverageWidth); nPosX < rcDraw.right; nPosX+=nNumberAverageWidth)
            {
              if (nRulerCaretWidth && ptCaretPos.x >= nPosX - nNumberAverageWidth / 2 && ptCaretPos.x <= nPosX + nNumberAverageWidth / 2)
              {
                bOnCaret=TRUE;
                SelectObject(hBufferDC, hPenRulerCaret);

                for (i=1; i < nRulerCaretWidth; ++i)
                {
                  if (i % 2)
                    nOffset=-(i + 1) / 2;
                  else
                    nOffset=i / 2;
                  MoveToEx(hBufferDC, nPosX + nOffset, rcBoard.bottom - 2, NULL);
                  if (nCharInLine % 10 == 0)
                    LineTo(hBufferDC, nPosX + nOffset, rcBoard.top);
                  else if (nCharInLine % 5 == 0)
                    LineTo(hBufferDC, nPosX + nOffset, (rcBoard.bottom - 2) - lpBoard->nBoardHeight / 2);
                  else
                    LineTo(hBufferDC, nPosX + nOffset, (rcBoard.bottom - 2) - lpBoard->nBoardHeight / 3);
                }
              }

              MoveToEx(hBufferDC, nPosX, rcBoard.bottom - 2, NULL);
              if (nCharInLine % 10 == 0)
              {
                LineTo(hBufferDC, nPosX, rcBoard.top);

                //Draw number
                nLineNumberLen=wsprintfA(szLineNumber, "%d", nCharInLine);
                SetBkMode(hBufferDC, TRANSPARENT);
                SetTextColor(hBufferDC, crDrawBoardText);
                TextOutA(hBufferDC, nPosX + 1, rcBoard.top - min(lpBoard->nBoardHeight / 10, 1), szLineNumber, nLineNumberLen);
                SetBkMode(hBufferDC, OPAQUE);
              }
              else if (nCharInLine % 5 == 0)
                LineTo(hBufferDC, nPosX, (rcBoard.bottom - 2) - lpBoard->nBoardHeight / 2);
              else
                LineTo(hBufferDC, nPosX, (rcBoard.bottom - 2) - lpBoard->nBoardHeight / 3);

              if (bOnCaret)
              {
                SelectObject(hBufferDC, hPenRulerScale);
                bOnCaret=FALSE;
              }
              ++nCharInLine;
            }

            //Copy from buffer DC
            hRgn=CreateRectRgn(rcBoard.left, rcBoard.top, rcBoard.right, rcBoard.bottom);
            hRgnOld=(HRGN)SelectObject(hDC, hRgn);
            BitBlt(hDC, rcBoard.left, rcBoard.top, rcBoard.right - rcBoard.left, rcBoard.bottom - rcBoard.top, hBufferDC, rcBoard.left, rcBoard.top, SRCCOPY);
            if (hRgnOld) SelectObject(hBufferDC, hRgnOld);
            DeleteObject(hRgn);

            if (hPenOld) SelectObject(hBufferDC, hPenOld);
            if (hFontOld) SelectObject(hBufferDC, hFontOld);
            DeleteObject(hFontRuler);
          }

          DeleteObject(hPenBookmarkBorder);
          DeleteObject(hPenPanelBorder);
          if (hPenModifiedUnsaved) DeleteObject(hPenModifiedUnsaved);
          if (hPenModifiedSaved) DeleteObject(hPenModifiedSaved);
          if (hPenRulerScale) DeleteObject(hPenRulerScale);
          if (hPenRulerCaret) DeleteObject(hPenRulerCaret);
          if (hBitmapOld) SelectObject(hBufferDC, hBitmapOld);
          DeleteObject(hBrushBookmark);
          DeleteObject(hBrushBoard);
          DeleteObject(hBitmap);
          DeleteDC(hBufferDC);
          ReleaseDC(hWnd, hDC);
        }
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    WINDOWBOARD *lpBoard;
    POINT pt;

    if (lpBoard=StackGetBoard(&hWindowStack, hWnd, NULL, GB_READ))
    {
      if (bShowBoard)
      {
        pt.x=LOWORD(lParam);
        pt.y=HIWORD(lParam);

        if (pt.x < lpBoard->nBoardWidth)
        {
          BOOKMARK *lpBookmark;
          RECT rcBookmark;
          int nClickChar;
          int nClickLine;

          nClickChar=(int)SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&pt);
          nClickLine=(int)SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nClickChar);

          if (StackGetBookmark(lpBoard, nClickLine) >= 0)
          {
            if (StackDeleteBookmarkByLine(lpBoard, nClickLine))
            {
              SaveRecentFile(lpBoard);
              StackGetBookmarkRect(lpBoard, nClickLine, &rcBookmark);
              InvalidateRect(hWnd, &rcBookmark, FALSE);
            }
          }
          else
          {
            if (lpBookmark=StackInsertBookmark(lpBoard, nClickLine))
            {
              SaveRecentFile(lpBoard);
              UpdateEdit(hWnd, UE_FIRSTPIXEL);
            }
          }
          *lResult=0;
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK SetBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  WINDOWBOARD *lpBoard;
  HWND hWndEdit=NULL;
  AEHDOC hDocEdit=NULL;
  BOOKMARK *lpBookmark;
  int nCurrentLine;

  if (lParam)
  {
    if (IsExtCallParamValid(lParam, 2))
      hWndEdit=(HWND)GetExtCallParam(lParam, 2);
    if (IsExtCallParamValid(lParam, 3))
      hDocEdit=(AEHDOC)GetExtCallParam(lParam, 3);
  }
  if (!hWndEdit)
    hWndEdit=GetFocusEdit();

  if (hWndEdit)
  {
    if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_READ))
    {
      nCurrentLine=GetCurrentLine(lpBoard->hWndEdit);

      //Toggle bookmark
      if (!dwDelBookmark)
      {
        if (StackGetBookmark(lpBoard, nCurrentLine) >= 0)
        {
          DelBookmarkProc(NULL, lParam, dwSupport);
          return TRUE;
        }
      }

      //Set bookmark
      if (lpBookmark=StackInsertBookmark(lpBoard, nCurrentLine))
      {
        SaveRecentFile(lpBoard);
        if (bShowBoard) UpdateEdit(lpBoard->hWndEdit, UE_FIRSTPIXEL);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK DelBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  WINDOWBOARD *lpBoard;
  HWND hWndEdit=NULL;
  AEHDOC hDocEdit=NULL;
  RECT rcBookmark;
  int nCurrentLine;

  if (lParam)
  {
    if (IsExtCallParamValid(lParam, 2))
      hWndEdit=(HWND)GetExtCallParam(lParam, 2);
    if (IsExtCallParamValid(lParam, 3))
      hDocEdit=(AEHDOC)GetExtCallParam(lParam, 3);
  }
  if (!hWndEdit)
    hWndEdit=GetFocusEdit();

  if (hWndEdit)
  {
    if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_READ))
    {
      nCurrentLine=GetCurrentLine(lpBoard->hWndEdit);

      if (StackDeleteBookmarkByLine(lpBoard, nCurrentLine))
      {
        SaveRecentFile(lpBoard);
        StackGetBookmarkRect(lpBoard, nCurrentLine, &rcBookmark);
        if (bShowBoard) InvalidateRect(lpBoard->hWndEdit, &rcBookmark, FALSE);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK DelAllBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  WINDOWBOARD *lpBoard;
  HWND hWndEdit=NULL;
  AEHDOC hDocEdit=NULL;
  RECT rcBoard;

  if (lParam)
  {
    if (IsExtCallParamValid(lParam, 2))
      hWndEdit=(HWND)GetExtCallParam(lParam, 2);
    if (IsExtCallParamValid(lParam, 3))
      hDocEdit=(AEHDOC)GetExtCallParam(lParam, 3);
  }
  if (!hWndEdit)
    hWndEdit=GetFocusEdit();

  if (hWndEdit)
  {
    if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_READ))
    {
      StackFreeBookmark(lpBoard);
      SaveRecentFile(lpBoard);

      if (bShowBoard)
      {
        GetClientRect(lpBoard->hWndEdit, &rcBoard);
        rcBoard.right=lpBoard->nBoardWidth;
        InvalidateRect(lpBoard->hWndEdit, &rcBoard, FALSE);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK BookmarkListProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  ShowBoookmarksMenu(NULL, -1, -1, FALSE);
  return TRUE;
}

BOOL CALLBACK NextBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  WINDOWBOARD *lpBoard;
  HWND hWndEdit=NULL;
  AEHDOC hDocEdit=NULL;
  int nBookmarkLine;

  if (lParam)
  {
    if (IsExtCallParamValid(lParam, 2))
      hWndEdit=(HWND)GetExtCallParam(lParam, 2);
    if (IsExtCallParamValid(lParam, 3))
      hDocEdit=(AEHDOC)GetExtCallParam(lParam, 3);
  }
  if (!hWndEdit)
    hWndEdit=GetFocusEdit();

  if (hWndEdit)
  {
    if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_READ))
    {
      if ((nBookmarkLine=StackGetNextBookmark(lpBoard, GetCurrentLine(lpBoard->hWndEdit))) >= 0)
      {
        SetCurrentLine(lpBoard->hWndEdit, nBookmarkLine);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK PrevBookmarkProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  WINDOWBOARD *lpBoard;
  HWND hWndEdit=NULL;
  AEHDOC hDocEdit=NULL;
  int nBookmarkLine;

  if (lParam)
  {
    if (IsExtCallParamValid(lParam, 2))
      hWndEdit=(HWND)GetExtCallParam(lParam, 2);
    if (IsExtCallParamValid(lParam, 3))
      hDocEdit=(AEHDOC)GetExtCallParam(lParam, 3);
  }
  if (!hWndEdit)
    hWndEdit=GetFocusEdit();

  if (hWndEdit)
  {
    if (lpBoard=StackGetBoard(&hWindowStack, hWndEdit, hDocEdit, GB_READ))
    {
      if ((nBookmarkLine=StackGetPrevBookmark(lpBoard, GetCurrentLine(lpBoard->hWndEdit))) >= 0)
      {
        SetCurrentLine(lpBoard->hWndEdit, nBookmarkLine);
      }
    }
  }
  return TRUE;
}

void UpdateEdit(HWND hWnd, DWORD dwFlags)
{
  RECT rc;
  BOOL bErase=FALSE;

  if (dwFlags & UE_ERASE)
    bErase=TRUE;

  if (dwFlags & UE_ALLRECT)
  {
    InvalidateRect(hWnd, NULL, bErase);
  }
  else if (dwFlags & UE_DRAWRECT)
  {
    SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)&rc);
    InvalidateRect(hWnd, &rc, bErase);
  }
  else if (dwFlags & UE_FIRSTPIXEL)
  {
    rc.left=0;
    rc.top=0;
    rc.right=1;
    rc.bottom=1;
    InvalidateRect(hWnd, &rc, bErase);
  }
  if (dwFlags & UE_UPDATEWINDOW)
    UpdateWindow(hWnd);
}


void UpdateEditAndClones(HWND hWnd, DWORD dwFlags)
{
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
  {
    if (ei.hWndMaster)
    {
      UpdateEdit(ei.hWndMaster, dwFlags);
      if (ei.hWndClone1)
        UpdateEdit(ei.hWndClone1, dwFlags);
      if (ei.hWndClone2)
        UpdateEdit(ei.hWndClone2, dwFlags);
      if (ei.hWndClone3)
        UpdateEdit(ei.hWndClone3, dwFlags);
    }
    else UpdateEdit(ei.hWndEdit, dwFlags);
  }
}

void UpdateEditAll(DWORD dwFlags)
{
  EDITINFO ei;
  BOOL bMaximized=TRUE;

  if (nMDI == WMD_MDI)
    SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);

  if (bMaximized)
  {
    if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
    {
      if (ei.hWndMaster)
      {
        UpdateEdit(ei.hWndMaster, dwFlags);
        if (ei.hWndClone1)
          UpdateEdit(ei.hWndClone1, dwFlags);
        if (ei.hWndClone2)
          UpdateEdit(ei.hWndClone2, dwFlags);
        if (ei.hWndClone3)
          UpdateEdit(ei.hWndClone3, dwFlags);
      }
      else UpdateEdit(ei.hWndEdit, dwFlags);
    }
  }
  else
  {
    //nMDI == WMD_MDI in non-maximized state
    FRAMEDATA *lpFrameCount=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0);
    FRAMEDATA *lpFrameInit=lpFrameCount;

    while (lpFrameCount)
    {
      if (lpFrameCount->ei.hWndMaster)
      {
        UpdateEdit(lpFrameCount->ei.hWndMaster, dwFlags);
        if (lpFrameCount->ei.hWndClone1)
          UpdateEdit(lpFrameCount->ei.hWndClone1, dwFlags);
        if (lpFrameCount->ei.hWndClone2)
          UpdateEdit(lpFrameCount->ei.hWndClone2, dwFlags);
        if (lpFrameCount->ei.hWndClone3)
          UpdateEdit(lpFrameCount->ei.hWndClone3, dwFlags);
      }
      else UpdateEdit(lpFrameCount->ei.hWndEdit, dwFlags);

      if (lpFrameInit == (lpFrameCount=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_NEXT, (LPARAM)lpFrameCount)))
        break;
    }
  }
}

void StackEndBoardAll(HSTACK *hStack)
{
  WINDOWBOARD *lpCurrent=StackGetBoard(&hWindowStack, GetCurEdit(), NULL, GB_READ);
  WINDOWBOARD *lpBoard;

  for (lpBoard=(WINDOWBOARD *)hStack->first; lpBoard; lpBoard=lpBoard->next)
  {
    //Process non-current boards
    if (lpBoard != lpCurrent) StackEndBoard(lpBoard);
  }

  //Process current board at the end for valid scrollbars and caret position
  if (lpCurrent) StackEndBoard(lpCurrent);
}

void StackEndBoard(WINDOWBOARD *lpBoard)
{
  int nOldBoardWidth;
  int nOldBoardHeight;

  if (lpBoard->nBoardWidth && IsWindow(lpBoard->hWndEdit))
  {
    nOldBoardWidth=lpBoard->nBoardWidth;
    lpBoard->nBoardWidth=0;
    nOldBoardHeight=lpBoard->nBoardHeight;
    lpBoard->nBoardHeight=0;
    SetEditRect(lpBoard->hDocEdit, lpBoard->hWndEdit, lpBoard->nBoardWidth, nOldBoardWidth, lpBoard->nBoardHeight, nOldBoardHeight);
  }
}

WINDOWBOARD* StackInsertBoard(HSTACK *hStack)
{
  WINDOWBOARD *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(WINDOWBOARD));
  return lpElement;
}

WINDOWBOARD* StackGetBoard(HSTACK *hStack, HWND hWndEdit, AEHDOC hDocEdit, DWORD dwFlags)
{
  WINDOWBOARD *lpBoard=NULL;

  if (hWndEdit || (nMDI == WMD_PMDI && hDocEdit))
  {
    for (lpBoard=(WINDOWBOARD *)hStack->first; lpBoard; lpBoard=lpBoard->next)
    {
      if (lpBoard->hWndEdit == hWndEdit)
      {
        if (lpBoard->hWndParent)
          return lpBoard;
        break;
      }
    }

    if (lpBoard && nMDI == WMD_PMDI)
    {
      if (!hDocEdit)
        hDocEdit=(AEHDOC)SendMessage(hWndEdit, AEM_GETDOCUMENT, 0, 0);

      for (lpBoard=(WINDOWBOARD *)hStack->first; lpBoard; lpBoard=lpBoard->next)
      {
        if (lpBoard->hDocEdit == hDocEdit)
          return lpBoard;
      }
    }

    if (!lpBoard && (dwFlags & GB_CREATE))
    {
      if (lpBoard=StackInsertBoard(hStack))
      {
        if (nMDI == WMD_PMDI)
        {
          if (hDocEdit)
            lpBoard->hDocEdit=hDocEdit;
          else
            lpBoard->hDocEdit=(AEHDOC)SendMessage(hWndEdit, AEM_GETDOCUMENT, 0, 0);
        }
        lpBoard->hWndEdit=hWndEdit;
      }
    }
  }
  return lpBoard;
}

WINDOWBOARD* StackGetBoardByUserParent(HSTACK *hStack, HWND hWndParent)
{
  WINDOWBOARD *lpBoard;

  for (lpBoard=(WINDOWBOARD *)hStack->first; lpBoard; lpBoard=lpBoard->next)
  {
    if (lpBoard->hWndParent == hWndParent)
      return lpBoard;
  }
  return NULL;
}

void StackDeleteBoard(HSTACK *hStack, WINDOWBOARD *wb)
{
  if (wb->hWndParent)
  {
    SetWindowLongPtrWide(wb->hWndEdit, GWLP_WNDPROC, (UINT_PTR)wb->lpOldEditProc);
    wb->lpOldEditProc=NULL;
    SetWindowLongPtrWide(wb->hWndParent, GWLP_WNDPROC, (UINT_PTR)wb->lpOldParentProc);
    wb->lpOldParentProc=NULL;
  }
  StackFreeBookmark(wb);
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)wb);
}

void StackFreeBoards(HSTACK *hStack)
{
  WINDOWBOARD *lpBoard;

  for (lpBoard=(WINDOWBOARD *)hStack->first; lpBoard; lpBoard=lpBoard->next)
  {
    if (lpBoard->hWndParent)
    {
      SetWindowLongPtrWide(lpBoard->hWndEdit, GWLP_WNDPROC, (UINT_PTR)lpBoard->lpOldEditProc);
      lpBoard->lpOldEditProc=NULL;
      SetWindowLongPtrWide(lpBoard->hWndParent, GWLP_WNDPROC, (UINT_PTR)lpBoard->lpOldParentProc);
      lpBoard->lpOldParentProc=NULL;

      //Till dll in memory make sure no messages in hWndParent thread
      SendMessage(lpBoard->hWndParent, AKDLL_FREEMESSAGELOOP, 0, 0);
    }
    StackFreeBookmark(lpBoard);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

BOOKMARK* StackInsertBookmark(WINDOWBOARD *wb, int nLine)
{
  BOOKMARK *lpBookmark;
  BOOKMARK *lpElement=NULL;
  AEPOINT point;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    if (lpBookmark->lpPoint->ciPoint.nLine == nLine)
      return NULL;
    if (lpBookmark->lpPoint->ciPoint.nLine > nLine)
      break;
  }

  if (!StackInsertBefore((stack **)&wb->hBookmarkStack.first, (stack **)&wb->hBookmarkStack.last, (stack *)lpBookmark, (stack **)&lpElement, sizeof(BOOKMARK)))
  {
    ++wb->hBookmarkStack.nCount;
    lpElement->nLine=nLine;

    if (SendMessage(wb->hWndEdit, AEM_GETLINEINDEX, (WPARAM)nLine, (LPARAM)&point.ciPoint))
    {
      point.nPointOffset=AEPTO_IGNORE;
      point.nPointLen=0;
      point.dwFlags=0;
      point.dwUserData=0;
      if (point.ciPoint.lpLine->prev && point.ciPoint.lpLine->prev->nLineBreak == AELB_WRAP)
        point.ciPoint.nCharInLine=min(point.ciPoint.nCharInLine + 1, point.ciPoint.lpLine->nLineLen);
      lpElement->lpPoint=(AEPOINT *)SendMessage(wb->hWndEdit, AEM_ADDPOINT, 0, (LPARAM)&point);
    }
    if (!lpElement->lpPoint)
    {
      StackDelete((stack **)&wb->hBookmarkStack.first, (stack **)&wb->hBookmarkStack.last, (stack *)lpElement);
      --wb->hBookmarkStack.nCount;
      lpElement=NULL;
    }
  }
  return lpElement;
}

void StackDeleteBookmarkByData(WINDOWBOARD *wb, BOOKMARK *lpBookmark)
{
  SendMessage(wb->hWndEdit, AEM_DELPOINT, (WPARAM)lpBookmark->lpPoint, 0);
  StackDelete((stack **)&wb->hBookmarkStack.first, (stack **)&wb->hBookmarkStack.last, (stack *)lpBookmark);
  --wb->hBookmarkStack.nCount;
}

BOOL StackDeleteBookmarkByLine(WINDOWBOARD *wb, int nLine)
{
  BOOKMARK *lpBookmark;
  BOOKMARK *lpNextBookmark;
  BOOL bResult=FALSE;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpNextBookmark)
  {
    lpNextBookmark=lpBookmark->next;

    if (lpBookmark->lpPoint->ciPoint.nLine >= nLine)
    {
      if (lpBookmark->lpPoint->ciPoint.nLine == nLine)
      {
        StackDeleteBookmarkByData(wb, lpBookmark);
        bResult=TRUE;
      }
      else break;
    }
  }
  return bResult;
}

void StackRemoveDuplicateBookmarks(WINDOWBOARD *wb)
{
  BOOKMARK *lpBookmark;
  BOOKMARK *lpNextBookmark;
  BOOKMARK *lpDeleteBookmark;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpNextBookmark)
  {
    lpNextBookmark=lpBookmark->next;

    while (lpNextBookmark)
    {
      if (lpBookmark->lpPoint->ciPoint.nLine == lpNextBookmark->lpPoint->ciPoint.nLine)
      {
        lpDeleteBookmark=lpNextBookmark;
        lpNextBookmark=lpNextBookmark->next;

        StackDeleteBookmarkByData(wb, lpDeleteBookmark);
      }
      else break;
    }
  }
}

int StackGetBookmark(WINDOWBOARD *wb, int nLine)
{
  BOOKMARK *lpBookmark;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    if (lpBookmark->lpPoint->ciPoint.nLine > nLine)
      break;
    if (lpBookmark->lpPoint->ciPoint.nLine == nLine)
      return lpBookmark->lpPoint->ciPoint.nLine;
  }
  return -1;
}

int StackGetNextBookmark(WINDOWBOARD *wb, int nLine)
{
  BOOKMARK *lpBookmark;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    if (lpBookmark->lpPoint->ciPoint.nLine > nLine)
      return lpBookmark->lpPoint->ciPoint.nLine;
  }
  return -1;
}

int StackGetPrevBookmark(WINDOWBOARD *wb, int nLine)
{
  BOOKMARK *lpBookmark;
  BOOKMARK *prev=NULL;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    if (lpBookmark->lpPoint->ciPoint.nLine < nLine)
      prev=lpBookmark;
    else
      break;
  }

  if (prev)
    return prev->lpPoint->ciPoint.nLine;
  return -1;
}

void StackGetBookmarkRect(WINDOWBOARD *wb, int nLine, RECT *rc)
{
  RECT rcEdit;
  POINT ptTop;
  int nLineIndex;
  int nCharHeight;

  GetClientRect(wb->hWndEdit, &rcEdit);

  if ((nLineIndex=(int)SendMessage(wb->hWndEdit, EM_LINEINDEX, nLine, 0)) >= 0)
  {
    GetPosFromChar(wb->hWndEdit, nLineIndex, &ptTop);
    nCharHeight=(int)SendMessage(wb->hWndEdit, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
    rc->left=rcEdit.left;
    rc->top=ptTop.y;
    rc->right=wb->nBoardWidth;
    rc->bottom=min(ptTop.y + nCharHeight, rcEdit.bottom);
  }
  else
  {
    rc->left=rcEdit.left;
    rc->top=rcEdit.top;
    rc->right=wb->nBoardWidth;
    rc->bottom=rcEdit.bottom;
  }
}

void StackSaveMobileBookmarks(WINDOWBOARD *wb)
{
  BOOKMARK *lpBookmark;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    lpBookmark->nLine=lpBookmark->lpPoint->ciPoint.nLine;
  }
}

void StackRestoreMobileBookmarks(WINDOWBOARD *wb, int nBeforeStartLine, int nBeforeEndLine, int nAfterEndLine)
{
  BOOKMARK *lpBookmark;
  BOOKMARK *lpPrevBookmark;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpPrevBookmark)
  {
    lpPrevBookmark=lpBookmark->next;

    if (nBeforeEndLine == -1 || (lpBookmark->nLine >= nBeforeStartLine && lpBookmark->nLine <= nBeforeEndLine))
    {
      if (nBeforeEndLine == -1 || lpBookmark->nLine <= nAfterEndLine)
      {
        lpBookmark->lpPoint->ciPoint.nLine=lpBookmark->nLine;
        SendMessage(wb->hWndEdit, AEM_INDEXUPDATE, 0, (LPARAM)&lpBookmark->lpPoint->ciPoint);
        lpBookmark->lpPoint->ciPoint.nCharInLine=min(lpBookmark->lpPoint->ciPoint.nCharInLine, lpBookmark->lpPoint->ciPoint.lpLine->nLineLen);
      }
      else
      {
        StackDeleteBookmarkByData(wb, lpBookmark);
        continue;
      }
    }
  }
}

void StackFreeBookmark(WINDOWBOARD *wb)
{
  BOOKMARK *lpBookmark;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    if (nMDI == WMD_PMDI)
      SendToDoc(wb->hDocEdit, wb->hWndEdit, AEM_DELPOINT, (WPARAM)lpBookmark->lpPoint, 0);
    else
      SendMessage(wb->hWndEdit, AEM_DELPOINT, (WPARAM)lpBookmark->lpPoint, 0);
  }

  StackClear((stack **)&wb->hBookmarkStack.first, (stack **)&wb->hBookmarkStack.last);
  wb->hBookmarkStack.nCount=0;
}

int ResetBookmarksMovedFlag(WINDOWBOARD *wb)
{
  BOOKMARK *lpBookmark;
  int nResult=0;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    if (lpBookmark->lpPoint->dwFlags & AEPTF_MOVELINE)
    {
      lpBookmark->lpPoint->dwFlags&=~AEPTF_MOVELINE;
      ++nResult;
    }
  }
  return nResult;
}

int GetBookmarksString(WINDOWBOARD *wb, wchar_t *wszString)
{
  BOOKMARK *lpBookmark;
  int nUnwrappedLine;
  int nSize=0;

  for (lpBookmark=wb->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
  {
    nUnwrappedLine=(int)SendToDoc(wb->hDocEdit, wb->hWndEdit, AEM_GETUNWRAPLINE, lpBookmark->lpPoint->ciPoint.nLine, 0);
    if (wszString)
      nSize+=xitoaW(nUnwrappedLine, wszString + nSize);
    else
      nSize+=xitoaW(nUnwrappedLine, NULL) - 1;

    if (wszString) *(wszString + nSize)=L',';
    ++nSize;
  }
  if (nSize) --nSize;

  if (wszString)
  {
    *(wszString + nSize)=L'\0';
    return nSize;
  }
  return nSize + 1;
}

int SetBookmarksString(WINDOWBOARD *wb, const wchar_t *wpString)
{
  const wchar_t *wpCount=wpString;
  int nWrappedLine;
  int nCount=0;

  while (*wpCount >= L'0' && *wpCount <= L'9')
  {
    nWrappedLine=(int)SendToDoc(wb->hDocEdit, wb->hWndEdit, AEM_GETWRAPLINE, xatoiW(wpCount, &wpCount), (LPARAM)NULL);
    StackInsertBookmark(wb, nWrappedLine);
    ++nCount;
    if (*wpCount == L',')
      ++wpCount;
    else
      break;
  }
  return nCount;
}

BOOL SaveRecentFile(WINDOWBOARD *lpBoard)
{
  EDITINFO ei;
  RECENTFILEPARAM rfp;
  RECENTFILE *rfReal;
  RECENTFILEPARAM *rfpReal;
  BOOL bResult=FALSE;

  if (bRememberBookmarks)
  {
    if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)lpBoard->hWndEdit, (LPARAM)&ei))
    {
      if (*ei.wszFile)
      {
        if (rfReal=(RECENTFILE *)SendMessage(hMainWnd, AKD_RECENTFILES, RF_FINDITEMBYNAME, (LPARAM)ei.wszFile))
        {
          wchar_t *wszBookmarks=NULL;
          int nSize;

          if ((nSize=GetBookmarksString(lpBoard, NULL)) > 1)
          {
            if (wszBookmarks=(wchar_t *)GlobalAlloc(GPTR, nSize * sizeof(wchar_t)))
            {
              GetBookmarksString(lpBoard, wszBookmarks);

              //Set
              rfp.file=rfReal;
              rfp.pParamName=L"Bookmarks";
              rfp.pParamValue=wszBookmarks;
              SendMessage(hMainWnd, AKD_RECENTFILES, RF_SETPARAM, (LPARAM)&rfp);
              bResult=TRUE;

              GlobalFree((HGLOBAL)wszBookmarks);
            }
          }
          else
          {
            //Delete
            rfp.file=rfReal;
            rfp.pParamName=L"Bookmarks";
            if (rfpReal=(RECENTFILEPARAM *)SendMessage(hMainWnd, AKD_RECENTFILES, RF_GETPARAM, (LPARAM)&rfp))
            {
              SendMessage(hMainWnd, AKD_RECENTFILES, RF_DELETEPARAM, (LPARAM)rfpReal);
              bResult=TRUE;
            }
          }
        }
      }
    }
  }
  return bResult;
}

BOOL RestoreRecentFile(WINDOWBOARD *lpBoard)
{
  EDITINFO ei;
  RECENTFILEPARAM rfp;
  RECENTFILE *rfReal;
  RECENTFILEPARAM *rfpReal;
  BOOL bResult=FALSE;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)lpBoard->hWndEdit, (LPARAM)&ei))
  {
    if (xstrcmpiW(lpBoard->wszFile, ei.wszFile))
    {
      StackFreeBookmark(lpBoard);
      xstrcpynW(lpBoard->wszFile, ei.wszFile, MAX_PATH);

      if (bRememberBookmarks && *lpBoard->wszFile)
      {
        if (rfReal=(RECENTFILE *)SendMessage(hMainWnd, AKD_RECENTFILES, RF_FINDITEMBYNAME, (LPARAM)lpBoard->wszFile))
        {
          rfp.file=rfReal;
          rfp.pParamName=L"Bookmarks";

          if (rfpReal=(RECENTFILEPARAM *)SendMessage(hMainWnd, AKD_RECENTFILES, RF_GETPARAM, (LPARAM)&rfp))
          {
            if (SetBookmarksString(lpBoard, rfpReal->pParamValue))
            {
              bResult=TRUE;
            }
          }
        }
      }
    }
  }
  return bResult;
}

void ShowBoookmarksMenu(WINDOWBOARD *lpBoard, int x, int y, BOOL bSettings)
{
  BOOKMARK *lpBookmark;
  HWND hWndEdit;
  HMENU hMenuBookmarks;
  POINT pt;
  wchar_t wszFormat[64];
  wchar_t *wpSpaces=L"    ";
  wchar_t *wpLine;
  INT_PTR nLineLen;
  int nLine=0;
  int nLineDigits;
  int nIndex;
  int nCount=0;
  int i;

  if (lpBoard)
  {
    hWndEdit=lpBoard->hWndEdit;
  }
  else
  {
    if (hWndEdit=GetFocusEdit())
      lpBoard=StackGetBoard(&hWindowStack, hWndEdit, NULL, GB_READ);
  }

  if (hWndEdit)
  {
    if (x < 0 && y < 0)
    {
      //TrackPopupMenu locks WM_KEYUP processing by AkelEdit and cause next WM_CHAR ignoring, so send WM_KEYUP directly.
      if (HIBYTE(dwBookmarkList) & HOTKEYF_ALT)
        SendMessage(hWndEdit, WM_KEYUP, VK_MENU, 0);
    }

    if (lpBoard)
    {
      if (hMenuBookmarks=CreatePopupMenu())
      {
         //Get max line
         if (!(dwStatusPosType & SPT_LINEWRAP))
           nLine=(int)SendMessage(lpBoard->hWndEdit, AEM_GETLINENUMBER, AEGL_LINEUNWRAPCOUNT, 0);
         else
           nLine=(int)SendMessage(lpBoard->hWndEdit, AEM_GETLINENUMBER, AEGL_LINECOUNT, 0);
         nLineDigits=(int)xprintfW(NULL, L"%d", nLine) - 1;
         xprintfW(wszFormat, L"%%0%dd:%s%%.%%ds", nLineDigits, wpSpaces);
         //Format is now something like: L"%02d:    %.%ds"

         //Fill menu
         for (lpBookmark=lpBoard->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
         {
           wpLine=lpBookmark->lpPoint->ciPoint.lpLine->wpLine;
           nLine=lpBookmark->lpPoint->ciPoint.nLine;
           if (!(dwStatusPosType & SPT_LINEWRAP))
             nLine=(int)SendMessage(lpBoard->hWndEdit, AEM_GETUNWRAPLINE, nLine, 0);

           //Skip leading spaces
           for (i=0; wpLine[i] == ' ' || wpLine[i] == '\t'; ++i);

           //Replace tabulation with spaces
           nLineLen=min(lpBookmark->lpPoint->ciPoint.lpLine->nLineLen - i, 128);
           xstrrepW(wpLine + i, nLineLen, L"\t", 1, wpSpaces, -1, TRUE, wszBuffer, &nLineLen);

           //Add menu string
           xprintfW(wszBuffer2, wszFormat, nLine + 1, nLineLen, wszBuffer);
           AppendMenuWide(hMenuBookmarks, MF_STRING, ++nCount, wszBuffer2);
         }

         if (bSettings)
         {
           if (nCount)
             AppendMenuWide(hMenuBookmarks, MF_SEPARATOR, (UINT)-1, NULL);
           AppendMenuWide(hMenuBookmarks, MF_STRING, ++nCount, GetLangStringW(wLangModule, STRID_SETTINGS));
         }

         //Show menu
         if (x < 0 && y < 0 && SendMessage(lpBoard->hWndEdit, AEM_GETCARETPOS, (WPARAM)&pt, 0))
         {
           pt.y+=(int)SendMessage(lpBoard->hWndEdit, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
           ClientToScreen(lpBoard->hWndEdit, &pt);
         }
         else
         {
           pt.x=x;
           pt.y=y;
         }

         if (lpBoard->hWndParent)
           SetFocus(hMainWnd);
         nIndex=TrackPopupMenu(hMenuBookmarks, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hMainWnd, NULL);
         if (lpBoard->hWndParent)
           SetFocus(lpBoard->hWndParent);

         if (bSettings && nCount == nIndex)
         {
           DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hMainWnd, (DLGPROC)SetupDlgProc);
         }
         else
         {
           //Go to bookmark
           if (nIndex)
           {
             for (lpBookmark=lpBoard->hBookmarkStack.first; lpBookmark; lpBookmark=lpBookmark->next)
             {
               if (!--nIndex)
               {
                 SetCurrentLine(lpBoard->hWndEdit, lpBookmark->lpPoint->ciPoint.nLine);
                 break;
               }
             }
           }
         }
         DestroyMenu(hMenuBookmarks);
      }
    }
  }
}

void SetEditRect(AEHDOC hDocEdit, HWND hWndEdit, int nNewWidth, int nOldWidth, int nNewHeight, int nOldHeight)
{
  RECT rcErase;
  RECT rcDraw;
  DWORD dwFlags=AERC_NORIGHT|AERC_NOBOTTOM;

  if (nMDI == WMD_PMDI)
  {
    SendToDoc(hDocEdit, hWndEdit, AEM_GETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendToDoc(hDocEdit, hWndEdit, AEM_GETRECT, dwFlags, (LPARAM)&rcDraw);
  }
  else
  {
    SendMessage(hWndEdit, AEM_GETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendMessage(hWndEdit, AEM_GETRECT, dwFlags, (LPARAM)&rcDraw);
  }

  rcDraw.left+=nNewWidth - nOldWidth;
  rcDraw.top+=nNewHeight - nOldHeight;
  rcErase.left+=nNewWidth - nOldWidth;
  rcErase.top+=nNewHeight - nOldHeight;

  if (nMDI == WMD_PMDI)
  {
    SendToDoc(hDocEdit, hWndEdit, AEM_SETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendToDoc(hDocEdit, hWndEdit, AEM_SETRECT, dwFlags|AERC_UPDATE, (LPARAM)&rcDraw);
  }
  else
  {
    SendMessage(hWndEdit, AEM_SETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendMessage(hWndEdit, AEM_SETRECT, dwFlags|AERC_UPDATE, (LPARAM)&rcDraw);
  }
}

HWND GetCurEdit()
{
 EDITINFO ei;

 if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
   return ei.hWndEdit;
 return NULL;
}

HWND GetFocusEdit()
{
  WINDOWBOARD *lpBoard;
  HWND hWndFocus=GetFocus();

  if (hWndFocus)
  {
    if (lpBoard=StackGetBoard(&hWindowStack, hWndFocus, NULL, GB_READ))
      return lpBoard->hWndEdit;
  }
  return GetCurEdit();
}

void GetCoderColors(HWND hWnd)
{
  //Default colors
  crDrawBoardText=crBoardText;
  crDrawBoardBk=crBoardBk;
  crDrawBoardBorder=crBoardBorder;
  crDrawBoardBookmarkText=crBoardBookmarkText;
  crDrawBoardBookmarkBk=crBoardBookmarkBk;
  crDrawBoardBookmarkBorder=crBoardBookmarkBorder;
  crDrawBoardLineUnsaved=crBoardLineUnsaved;
  crDrawBoardLineSaved=crBoardLineSaved;
  crDrawBoardRulerCaret=crBoardRulerCaret;
  crDrawBoardRulerScale=crBoardRulerScale;

  if (bCoderTheme)
  {
    PLUGINFUNCTION *pfCoder;
    PLUGINCALLSENDW pcs;
    DLLEXTCODERFILLVARLIST decfvl;

    if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfCoder->bRunning)
    {
      decfvl.dwStructSize=sizeof(DLLEXTCODERFILLVARLIST);
      decfvl.nAction=DLLA_CODER_FILLVARLIST;
      decfvl.hWndEdit=hWnd;
      decfvl.hDocEdit=NULL;
      decfvl.cti=cti;

      pcs.pFunction=L"Coder::Settings";
      pcs.lParam=(LPARAM)&decfvl;
      pcs.dwSupport=PDS_STRWIDE;
      SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
    }
  }
}

int GetMaxNumberWidth(HDC hDC, int *nNumberAverageWidth)
{
  char szNumber[32];
  SIZE sizeNumber;
  int nAllNumberWidth=0;
  int nMaxNumberWidth=0;
  int nNumber;

  for (nNumber=0; nNumber <= 9; ++nNumber)
  {
    wsprintfA(szNumber, "%d", nNumber);
    GetTextExtentPoint32A(hDC, szNumber, 1, &sizeNumber);
    if (sizeNumber.cx > nMaxNumberWidth)
      nMaxNumberWidth=sizeNumber.cx;
    nAllNumberWidth+=sizeNumber.cx;
  }
  if (nNumberAverageWidth)
    *nNumberAverageWidth=nAllNumberWidth / 10;
  return nMaxNumberWidth;
}

void GetPosFromChar(HWND hWnd, INT_PTR nCharIndex, POINT *pt)
{
  DWORD dwPosition;

  if (bOldRichEdit)
  {
    dwPosition=(DWORD)SendMessage(hWnd, EM_POSFROMCHAR, nCharIndex, (LPARAM)NULL);
    pt->x=LOWORD(dwPosition);
    pt->y=HIWORD(dwPosition);
  }
  else
  {
    SendMessage(hWnd, EM_POSFROMCHAR, (WPARAM)pt, nCharIndex);
  }
}

int GetCurrentLine(HWND hWnd)
{
  INT_PTR nLineIndex;

  nLineIndex=SendMessage(hWnd, EM_LINEINDEX, (WPARAM)-1, 0);
  return (int)SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nLineIndex);
}

void SetCurrentLine(HWND hWnd, int nLine)
{
  AESELECTION aes;

  aes.crSel.ciMin.nLine=nLine;
  aes.crSel.ciMin.lpLine=NULL;
  aes.crSel.ciMin.nCharInLine=0;

  if (SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&aes.crSel.ciMin))
  {
    aes.crSel.ciMax=aes.crSel.ciMin;
    aes.dwFlags=AESELT_LOCKSCROLL;
    aes.dwType=0;
    SendMessage(hWnd, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);

    ScrollCaret(hWnd);
  }
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

LRESULT SendToDoc(AEHDOC hDocEdit, HWND hWndEdit, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AESENDMESSAGE sm;

  sm.hDoc=hDocEdit;
  sm.uMsg=uMsg;
  sm.wParam=wParam;
  sm.lParam=lParam;
  if (SendMessage(hWndEdit, AEM_SENDMESSAGE, 0, (LPARAM)&sm))
    return sm.lResult;
  return 0;
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

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"ShowBoard", PO_DWORD, (LPBYTE)&bShowBoard, sizeof(DWORD));
    WideOption(hOptions, L"TextColor", PO_DWORD, (LPBYTE)&crBoardText, sizeof(DWORD));
    WideOption(hOptions, L"BackgroundColor", PO_DWORD, (LPBYTE)&crBoardBk, sizeof(DWORD));
    WideOption(hOptions, L"BorderColor", PO_DWORD, (LPBYTE)&crBoardBorder, sizeof(DWORD));
    WideOption(hOptions, L"BookmarkTextColor", PO_DWORD, (LPBYTE)&crBoardBookmarkText, sizeof(DWORD));
    WideOption(hOptions, L"BookmarkBackgroundColor", PO_DWORD, (LPBYTE)&crBoardBookmarkBk, sizeof(DWORD));
    WideOption(hOptions, L"BookmarkBorderColor", PO_DWORD, (LPBYTE)&crBoardBookmarkBorder, sizeof(DWORD));
    WideOption(hOptions, L"LineUnsavedEnable", PO_DWORD, (LPBYTE)&bLineUnsavedEnable, sizeof(DWORD));
    WideOption(hOptions, L"LineUnsavedColor", PO_DWORD, (LPBYTE)&crBoardLineUnsaved, sizeof(DWORD));
    WideOption(hOptions, L"LineSavedEnable", PO_DWORD, (LPBYTE)&bLineSavedEnable, sizeof(DWORD));
    WideOption(hOptions, L"LineSavedColor", PO_DWORD, (LPBYTE)&crBoardLineSaved, sizeof(DWORD));
    WideOption(hOptions, L"LineModificationWidth", PO_DWORD, (LPBYTE)&nLineModificationWidth, sizeof(DWORD));
    WideOption(hOptions, L"RulerScaleColor", PO_DWORD, (LPBYTE)&crBoardRulerScale, sizeof(DWORD));
    WideOption(hOptions, L"RulerCaretColor", PO_DWORD, (LPBYTE)&crBoardRulerCaret, sizeof(DWORD));
    WideOption(hOptions, L"RulerEnable", PO_DWORD, (LPBYTE)&bRulerEnable, sizeof(DWORD));
    WideOption(hOptions, L"RulerHeight", PO_DWORD, (LPBYTE)&nRulerHeight, sizeof(DWORD));
    WideOption(hOptions, L"RulerCaretWidth", PO_DWORD, (LPBYTE)&nRulerCaretWidth, sizeof(DWORD));
    WideOption(hOptions, L"SetBookmark", PO_DWORD, (LPBYTE)&dwSetBookmark, sizeof(DWORD));
    WideOption(hOptions, L"DelBookmark", PO_DWORD, (LPBYTE)&dwDelBookmark, sizeof(DWORD));
    WideOption(hOptions, L"DelAllBookmark", PO_DWORD, (LPBYTE)&dwDelAllBookmark, sizeof(DWORD));
    WideOption(hOptions, L"BookmarkList", PO_DWORD, (LPBYTE)&dwBookmarkList, sizeof(DWORD));
    WideOption(hOptions, L"NextBookmark", PO_DWORD, (LPBYTE)&dwNextBookmark, sizeof(DWORD));
    WideOption(hOptions, L"PrevBookmark", PO_DWORD, (LPBYTE)&dwPrevBookmark, sizeof(DWORD));
    WideOption(hOptions, L"RememberBookmarks", PO_DWORD, (LPBYTE)&bRememberBookmarks, sizeof(DWORD));
    WideOption(hOptions, L"BookmarkCharHeight", PO_DWORD, (LPBYTE)&bBookmarkCharHeight, sizeof(DWORD));
    WideOption(hOptions, L"CoderTheme", PO_DWORD, (LPBYTE)&bCoderTheme, sizeof(DWORD));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_SETTINGS)
    {
      WideOption(hOptions, L"ShowBoard", PO_DWORD, (LPBYTE)&bShowBoard, sizeof(DWORD));
      WideOption(hOptions, L"TextColor", PO_DWORD, (LPBYTE)&crBoardText, sizeof(DWORD));
      WideOption(hOptions, L"BackgroundColor", PO_DWORD, (LPBYTE)&crBoardBk, sizeof(DWORD));
      WideOption(hOptions, L"BorderColor", PO_DWORD, (LPBYTE)&crBoardBorder, sizeof(DWORD));
      WideOption(hOptions, L"BookmarkTextColor", PO_DWORD, (LPBYTE)&crBoardBookmarkText, sizeof(DWORD));
      WideOption(hOptions, L"BookmarkBackgroundColor", PO_DWORD, (LPBYTE)&crBoardBookmarkBk, sizeof(DWORD));
      WideOption(hOptions, L"BookmarkBorderColor", PO_DWORD, (LPBYTE)&crBoardBookmarkBorder, sizeof(DWORD));
      WideOption(hOptions, L"LineUnsavedEnable", PO_DWORD, (LPBYTE)&bLineUnsavedEnable, sizeof(DWORD));
      WideOption(hOptions, L"LineUnsavedColor", PO_DWORD, (LPBYTE)&crBoardLineUnsaved, sizeof(DWORD));
      WideOption(hOptions, L"LineSavedEnable", PO_DWORD, (LPBYTE)&bLineSavedEnable, sizeof(DWORD));
      WideOption(hOptions, L"LineSavedColor", PO_DWORD, (LPBYTE)&crBoardLineSaved, sizeof(DWORD));
      WideOption(hOptions, L"LineModificationWidth", PO_DWORD, (LPBYTE)&nLineModificationWidth, sizeof(DWORD));
      WideOption(hOptions, L"RulerScaleColor", PO_DWORD, (LPBYTE)&crBoardRulerScale, sizeof(DWORD));
      WideOption(hOptions, L"RulerCaretColor", PO_DWORD, (LPBYTE)&crBoardRulerCaret, sizeof(DWORD));
      WideOption(hOptions, L"RulerEnable", PO_DWORD, (LPBYTE)&bRulerEnable, sizeof(DWORD));
      WideOption(hOptions, L"RulerHeight", PO_DWORD, (LPBYTE)&nRulerHeight, sizeof(DWORD));
      WideOption(hOptions, L"RulerCaretWidth", PO_DWORD, (LPBYTE)&nRulerCaretWidth, sizeof(DWORD));
      WideOption(hOptions, L"SetBookmark", PO_DWORD, (LPBYTE)&dwSetBookmark, sizeof(DWORD));
      WideOption(hOptions, L"DelBookmark", PO_DWORD, (LPBYTE)&dwDelBookmark, sizeof(DWORD));
      WideOption(hOptions, L"DelAllBookmark", PO_DWORD, (LPBYTE)&dwDelAllBookmark, sizeof(DWORD));
      WideOption(hOptions, L"BookmarkList", PO_DWORD, (LPBYTE)&dwBookmarkList, sizeof(DWORD));
      WideOption(hOptions, L"NextBookmark", PO_DWORD, (LPBYTE)&dwNextBookmark, sizeof(DWORD));
      WideOption(hOptions, L"PrevBookmark", PO_DWORD, (LPBYTE)&dwPrevBookmark, sizeof(DWORD));
      WideOption(hOptions, L"RememberBookmarks", PO_DWORD, (LPBYTE)&bRememberBookmarks, sizeof(DWORD));
      WideOption(hOptions, L"BookmarkCharHeight", PO_DWORD, (LPBYTE)&bBookmarkCharHeight, sizeof(DWORD));
      WideOption(hOptions, L"CoderTheme", PO_DWORD, (LPBYTE)&bCoderTheme, sizeof(DWORD));
    }

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
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
    if (nStringID == STRID_SHOW)
      return L"\x041F\x043E\x043A\x0430\x0437\x0430\x0442\x044C";
    if (nStringID == STRID_PANEL)
      return L"\x041F\x0430\x043D\x0435\x043B\x044C";
    if (nStringID == STRID_TEXTCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0442\x0435\x043A\x0441\x0442\x0430";
    if (nStringID == STRID_BACKGROUNDCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0444\x043E\x043D\x0430";
    if (nStringID == STRID_BORDERCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0433\x0440\x0430\x043D\x0438\x0446\x044B";
    if (nStringID == STRID_BOOKMARKTEXTCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0442\x0435\x043A\x0441\x0442\x0430\x0020\x0437\x0430\x043A\x043B\x0430\x0434\x043A\x0438";
    if (nStringID == STRID_BOOKMARKBACKGROUNDCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0444\x043E\x043D\x0430\x0020\x0437\x0430\x043A\x043B\x0430\x0434\x043A\x0438";
    if (nStringID == STRID_BOOKMARKBORDERCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0433\x0440\x0430\x043D\x0438\x0446\x044B\x0020\x0437\x0430\x043A\x043B\x0430\x0434\x043A\x0438";
    if (nStringID == STRID_LINEUNSAVEDCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x043D\x0435\x0441\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x043D\x043E\x0439\x0020\x0441\x0442\x0440\x043E\x043A\x0438";
    if (nStringID == STRID_LINESAVEDCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0441\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x043D\x043E\x0439\x0020\x0441\x0442\x0440\x043E\x043A\x0438";
    if (nStringID == STRID_WIDTH)
      return L"\x0428\x0438\x0440\x0438\x043D\x0430:";
    if (nStringID == STRID_RULERSCALECOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0448\x043A\x0430\x043B\x044B\x0020\x043D\x0430\x0020\x043B\x0438\x043D\x0435\x0439\x043A\x0435";
    if (nStringID == STRID_RULERCARETCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x043A\x0430\x0440\x0435\x0442\x043A\x0438\x0020\x043D\x0430\x0020\x043B\x0438\x043D\x0435\x0439\x043A\x0435";
    if (nStringID == STRID_RULERHEIGHT)
      return L"\x0412\x044B\x0441\x043E\x0442\x0430\x0020\x043B\x0438\x043D\x0435\x0439\x043A\x0438:";
    if (nStringID == STRID_CARETWIDTH)
      return L"\x0428\x0438\x0440\x0438\x043D\x0430\x0020\x043A\x0430\x0440\x0435\x0442\x043A\x0438:";
    if (nStringID == STRID_BOOKMARKS)
      return L"\x0417\x0430\x043A\x043B\x0430\x0434\x043A\x0438";
    if (nStringID == STRID_SETBOOKMARK)
      return L"\x0423\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_DELBOOKMARK)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_DELALLBOOKMARK)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x0432\x0441\x0435";
    if (nStringID == STRID_BOOKMARKLIST)
      return L"\x0421\x043F\x0438\x0441\x043E\x043A";
    if (nStringID == STRID_NEXTBOOKMARK)
      return L"\x0421\x043B\x0435\x0434\x0443\x044E\x0449\x0430\x044F";
    if (nStringID == STRID_PREVBOOKMARK)
      return L"\x041F\x0440\x0435\x0434\x044B\x0434\x0443\x0449\x0430\x044F";
    if (nStringID == STRID_SAVEBOOKMARKS)
      return L"\x0417\x0430\x043F\x043E\x043C\x0438\x043D\x0430\x0442\x044C\x0020\x0437\x0430\x043A\x043B\x0430\x0434\x043A\x0438";
    if (nStringID == STRID_BOOKMARKCHARHEIGHT)
      return L"\x0417\x0430\x043A\x043B\x0430\x0434\x043A\x0430\x0020\x043D\x0430\x0020\x0432\x044B\x0441\x043E\x0442\x0443\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x0430";
    if (nStringID == STRID_CODERTHEME)
      return L"\x0418\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x044C\x0020\x0442\x0435\x043C\x044B Coder";
    if (nStringID == STRID_LOADFIRST)
      return L"\x0417\x0430\x0433\x0440\x0443\x0437\x0438\x0442\x0435\x0020\x0441\x043F\x0435\x0440\x0432\x0430 %s.";
    if (nStringID == STRID_SETTINGS)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438...";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_SHOW)
      return L"Show board";
    if (nStringID == STRID_PANEL)
      return L"Panel";
    if (nStringID == STRID_TEXTCOLOR)
      return L"Text color";
    if (nStringID == STRID_BACKGROUNDCOLOR)
      return L"Background color";
    if (nStringID == STRID_BORDERCOLOR)
      return L"Border color";
    if (nStringID == STRID_BOOKMARKTEXTCOLOR)
      return L"Bookmark text color";
    if (nStringID == STRID_BOOKMARKBACKGROUNDCOLOR)
      return L"Bookmark background color";
    if (nStringID == STRID_BOOKMARKBORDERCOLOR)
      return L"Bookmark border color";
    if (nStringID == STRID_LINEUNSAVEDCOLOR)
      return L"Unsaved line color";
    if (nStringID == STRID_LINESAVEDCOLOR)
      return L"Saved line color";
    if (nStringID == STRID_WIDTH)
      return L"Width:";
    if (nStringID == STRID_RULERSCALECOLOR)
      return L"Ruler scale color";
    if (nStringID == STRID_RULERCARETCOLOR)
      return L"Ruler caret color";
    if (nStringID == STRID_RULERHEIGHT)
      return L"Ruler height:";
    if (nStringID == STRID_CARETWIDTH)
      return L"Caret width:";
    if (nStringID == STRID_BOOKMARKS)
      return L"Bookmarks";
    if (nStringID == STRID_SETBOOKMARK)
      return L"Set";
    if (nStringID == STRID_DELBOOKMARK)
      return L"Delete";
    if (nStringID == STRID_DELALLBOOKMARK)
      return L"Delete all";
    if (nStringID == STRID_BOOKMARKLIST)
      return L"List";
    if (nStringID == STRID_NEXTBOOKMARK)
      return L"Next";
    if (nStringID == STRID_PREVBOOKMARK)
      return L"Previous";
    if (nStringID == STRID_SAVEBOOKMARKS)
      return L"Remember bookmarks";
    if (nStringID == STRID_BOOKMARKCHARHEIGHT)
      return L"Bookmark character height";
    if (nStringID == STRID_CODERTHEME)
      return L"Use Coder themes";
    if (nStringID == STRID_LOADFIRST)
      return L"Load %s first.";
    if (nStringID == STRID_SETTINGS)
      return L"Settings...";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
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
  hMainWnd=pd->hMainWnd;
  hMdiClient=pd->hMdiClient;
  bOldWindows=pd->bOldWindows;
  bOldRichEdit=pd->bOldRichEdit;
  nMDI=pd->nMDI;
  wLangModule=PRIMARYLANGID(pd->wLangModule);

  //Initialize WideFunc.h header
  WideInitialize();

  //Plugin name
  {
    int i;

    for (i=0; pd->wszFunction[i] != L':'; ++i)
      wszPluginName[i]=pd->wszFunction[i];
    wszPluginName[i]=L'\0';
  }
  xprintfW(wszPluginTitle, GetLangStringW(wLangModule, STRID_PLUGIN), wszPluginName);

  ReadOptions(0);
}

void InitMain()
{
  if (!nInitMain++)
  {
    //Add hotkeys
    {
      PLUGINADDW pa={0};

      if (dwSetBookmark && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwSetBookmark))
      {
        pa.pFunction=L"LineBoard::Main::SetBookmark";
        pa.wHotkey=(WORD)dwSetBookmark;
        pa.PluginProc=SetBookmarkProc;
        pfwSetBookmark=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwDelBookmark && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwDelBookmark))
      {
        pa.pFunction=L"LineBoard::Main::DelBookmark";
        pa.wHotkey=(WORD)dwDelBookmark;
        pa.PluginProc=DelBookmarkProc;
        pfwDelBookmark=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwDelAllBookmark && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwDelAllBookmark))
      {
        pa.pFunction=L"LineBoard::Main::DelAllBookmark";
        pa.wHotkey=(WORD)dwDelAllBookmark;
        pa.PluginProc=DelAllBookmarkProc;
        pfwDelAllBookmark=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwBookmarkList && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwBookmarkList))
      {
        pa.pFunction=L"LineBoard::Main::BookmarkList";
        pa.wHotkey=(WORD)dwBookmarkList;
        pa.PluginProc=BookmarkListProc;
        pfwBookmarkList=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwNextBookmark && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwNextBookmark))
      {
        pa.pFunction=L"LineBoard::Main::NextBookmark";
        pa.wHotkey=(WORD)dwNextBookmark;
        pa.PluginProc=NextBookmarkProc;
        pfwNextBookmark=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwPrevBookmark && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwPrevBookmark))
      {
        pa.pFunction=L"LineBoard::Main::PrevBookmark";
        pa.wHotkey=(WORD)dwPrevBookmark;
        pa.PluginProc=PrevBookmarkProc;
        pfwPrevBookmark=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
    }

    //Subclass
    NewMainProcData=NULL;
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

    if (nMDI == WMD_MDI)
    {
      NewFrameProcData=NULL;
      SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewFrameProc, (LPARAM)&NewFrameProcData);
    }

    NewEditProcData=NULL;
    SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NewEditProc, (LPARAM)&NewEditProcData);
  }
}

void UninitMain()
{
  if (!--nInitMain)
  {
    //Save options
    if (dwSaveFlags)
    {
      SaveOptions(dwSaveFlags);
      dwSaveFlags=0;
    }

    //Remove hotkeys
    if (pfwSetBookmark)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwSetBookmark);
      pfwSetBookmark=NULL;
    }
    if (pfwDelBookmark)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwDelBookmark);
      pfwDelBookmark=NULL;
    }
    if (pfwDelAllBookmark)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwDelAllBookmark);
      pfwDelAllBookmark=NULL;
    }
    if (pfwBookmarkList)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwBookmarkList);
      pfwBookmarkList=NULL;
    }
    if (pfwNextBookmark)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwNextBookmark);
      pfwNextBookmark=NULL;
    }
    if (pfwPrevBookmark)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwPrevBookmark);
      pfwPrevBookmark=NULL;
    }

    //Remove subclass
    if (NewMainProcData)
    {
      SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
      NewMainProcData=NULL;
    }
    if (NewFrameProcData)
    {
      SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&NewFrameProcData);
      NewFrameProcData=NULL;
    }
    if (NewEditProcData)
    {
      SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&NewEditProcData);
      NewEditProcData=NULL;
    }
  }
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
    if (nInitMain)
    {
      //if (bShowBoard) StackEndBoardAll(&hWindowStack);
      StackFreeBoards(&hWindowStack);
      UninitMain();
    }
  }
  return TRUE;
}
