#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemset
#define xstrlenW
#define xstrcpynW
#define xstrcmpiW
#define xstrcmpinW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define DialogBoxWide
#define GetClassNameWide
#define GetWindowLongPtrWide
#define PropertySheetWide
#define SetDlgItemTextWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_AUTOSTEP          1
#define STRID_ENABLE            2
#define STRID_STEPTIME          3
#define STRID_STEPWIDTH         4
#define STRID_HSYNCHRONIZATION  5
#define STRID_VSYNCHRONIZATION  6
#define STRID_FRAMEMDI          7
#define STRID_SPLITPANE         8
#define STRID_NOSCROLL          9
#define STRID_UNDO              10
#define STRID_REDO              11
#define STRID_SELECTALL         12
#define STRID_ALIGNCARET        13
#define STRID_OFFSETX           14
#define STRID_OFFSETY           15
#define STRID_AUTOFOCUS         16
#define STRID_FOCUSBACKGROUND   17
#define STRID_MOVESCROLLBAR     18
#define STRID_MOVEWITHSHIFT     19
#define STRID_SWITCHTAB         20
#define STRID_WITHSPIN          21
#define STRID_INVERT            22
#define STRID_PLUGIN            23
#define STRID_OK                24
#define STRID_CANCEL            25

#define DLLA_AUTOSCROLL_TIME     1

#define DLLA_SCROLL_HSCROLL      1
#define DLLA_SCROLL_VSCROLL      2
#define DLLA_SCROLL_LINEHSCROLL  3
#define DLLA_SCROLL_LINEVSCROLL  4
#define DLLA_SCROLL_ENTERSCROLL  5

#define OF_AUTOSCROLL 0x01
#define OF_SYNCHORZ   0x02
#define OF_SYNCVERT   0x04
#define OF_NOSCROLL   0x08
#define OF_ALIGNCARET 0x10
#define OF_AUTOFOCUS  0x20

//Property sheet
#define PAGE_AUTOSCROLL   0
#define PAGE_SYNCHORZ     1
#define PAGE_SYNCVERT     2
#define PAGE_NOSCROLL     3
#define PAGE_ALIGNCARET   4
#define PAGE_AUTOFOCUS    5
#define PAGE_MAX          6

//Move caret
#define MC_CARETNOMOVE      0
#define MC_CARETMOVEINSIDE  1
#define MC_CARETMOVEOUTSIDE 2

//No scroll
#define NS_UNDO      0x1
#define NS_REDO      0x2
#define NS_SELECTALL 0x4

//Synchronization
#define SNC_MDI         0x00000001  //Synchronize scroll of MDI windows.
#define SNC_CLONE       0x00000002  //Synchronize scroll of split windows.

//Auto focus
#define AF_FOCUSBACKGROUND     0x00000001
#define AF_MOVESCROLLBAR       0x00000010
#define AF_MOVESCROLLBARINVERT 0x00000020
#define AF_MOVEWITHSHIFT       0x00000100
#define AF_MOVEWITHSHIFTINVERT 0x00000200
#define AF_SWITCHTAB           0x00001000
#define AF_SWITCHTABINVERT     0x00002000
#define AF_SWITCHTABWITHSPIN   0x00004000

#ifndef IDI_ICON_MAIN
  #define IDI_ICON_MAIN 1001
#endif
#ifndef PSCB_BUTTONPRESSED
  #define PSCB_BUTTONPRESSED 3
#endif
#ifndef WM_MOUSEHWHEEL
  #define WM_MOUSEHWHEEL  0x020E
#endif
#ifndef SPI_GETWHEELSCROLLCHARS
  #define SPI_GETWHEELSCROLLCHARS  0x006C
#endif

//Hook information
#define HT_MAX  10

typedef struct {
  HHOOK hHook;
  DWORD dwThreadId;
  LRESULT (CALLBACK *GetMsgProc)(int, WPARAM, LPARAM);
} HOOKTHREAD;

//Functions prototypes
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SettingsSheet(int nStartPage);
LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam);
int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam);
BOOL CALLBACK AutoScrollSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SyncHorzSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SyncVertSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK NoScrollSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AlignCaretSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AutoFocusSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SetScrollTimer();
void KillScrollTimer();
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void GetMsgProcCommon(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc1(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc2(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc3(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc4(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc5(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc6(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc7(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc8(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc9(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc10(int code, WPARAM wParam, LPARAM lParam);
void ScrollCaretOptions(HWND hWndEdit, AEHDOC hDocEdit, BOOL bCheckIfSet);
void ScrollCaretOptionsAll(BOOL bRemove);
HWND GetCurEdit();
AEHDOC GetCurDoc();
LRESULT SendToDoc(AEHDOC hDocEdit, HWND hWndEdit, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();
void InitAutoScroll();
void UninitAutoScroll();
void InitSyncHorz();
void UninitSyncHorz();
void InitSyncVert();
void UninitSyncVert();
void InitNoScroll();
void UninitNoScroll();
void InitAlignCaret();
void UninitAlignCaret();
void InitAutoFocus();
void UninitAutoFocus();
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;

//Global variables
wchar_t wszClassName[MAX_PATH];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceDLL;
HINSTANCE hInstanceEXE;
HWND hMainWnd;
HWND hMdiClient;
HICON hMainIcon;
int nMDI;
BOOL bAkelEdit;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
int nInitMain=0;
BOOL bInitAutoScroll=FALSE;
BOOL bInitSyncHorz=FALSE;
BOOL bInitSyncVert=FALSE;
BOOL bInitNoScroll=FALSE;
BOOL bInitAlignCaret=FALSE;
BOOL bInitAutoFocus=FALSE;
DWORD dwSaveFlags=0;
HWND hWndAutoScroll=NULL;
AEHDOC hDocAutoScroll=NULL;
UINT_PTR dwAutoScrollTimer=0;
int nAutoScrollStepTime=50;
int nAutoScrollStepWidth=1;
DWORD dwSyncHorz=SNC_MDI|SNC_CLONE;
DWORD dwSyncVert=SNC_MDI|SNC_CLONE;
DWORD dwNoScroll=NS_UNDO|NS_REDO|NS_SELECTALL;
int nAlignCaretFlags=AESC_OFFSETCHARX|AESC_OFFSETCHARY;
int nAlignCaretSelFlags=0;
int nAlignCaretSelType=AESCT_KEYBOARD;
int nAlignCaretOffsetX=10;
int nAlignCaretOffsetY=5;
DWORD dwAutoFocus=AF_FOCUSBACKGROUND|AF_MOVESCROLLBAR|AF_MOVEWITHSHIFT|AF_SWITCHTAB;
HOOKTHREAD ht[HT_MAX];

//Options dialog
PROPSHEETHEADERW psh={0};
PROPSHEETPAGEW psp[PAGE_MAX]={0};
HWND hWndPropSheet=NULL;
HIMAGELIST hImageList;
HHOOK hHookOptions;

//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Scroll";
}

//Plugin extern function
void __declspec(dllexport) AutoScroll(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (pd->bAkelEdit)
    {
      if (nAction == DLLA_AUTOSCROLL_TIME)
      {
        unsigned char *pStepTime=NULL;
        unsigned char *pStepWidth=NULL;
        wchar_t wszStepTime[32];
        wchar_t wszStepWidth[32];
        wchar_t *wpStepTime=wszStepTime;
        wchar_t *wpStepWidth=wszStepWidth;
        int *lpnStepTime=NULL;
        int *lpnStepWidth=NULL;

        if (IsExtCallParamValid(pd->lParam, 2))
          pStepTime=(unsigned char *)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pStepWidth=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          lpnStepTime=(int *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          lpnStepWidth=(int *)GetExtCallParam(pd->lParam, 5);

        if (pStepTime)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pStepTime, -1, wszStepTime, MAX_PATH);
          else
            wpStepTime=(wchar_t *)pStepTime;
          if (*wpStepTime == L'+')
            nAutoScrollStepTime+=(int)xatoiW(wpStepTime + 1, NULL);
          else if (*wpStepTime == L'-')
            nAutoScrollStepTime-=(int)xatoiW(wpStepTime + 1, NULL);
          else
            nAutoScrollStepTime=(int)xatoiW(wpStepTime, NULL);
          nAutoScrollStepTime=max(nAutoScrollStepTime, 0);
        }
        if (pStepWidth)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pStepWidth, -1, wszStepWidth, MAX_PATH);
          else
            wpStepWidth=(wchar_t *)pStepWidth;
          if (*wpStepWidth == L'+')
            nAutoScrollStepWidth+=(int)xatoiW(wpStepWidth + 1, NULL);
          else if (*wpStepWidth == L'-')
            nAutoScrollStepWidth-=(int)xatoiW(wpStepWidth + 1, NULL);
          else
            nAutoScrollStepWidth=(int)xatoiW(wpStepWidth, NULL);
          nAutoScrollStepWidth=max(nAutoScrollStepWidth, 0);
        }
        if (lpnStepTime)
          *lpnStepTime=nAutoScrollStepTime;
        if (lpnStepWidth)
          *lpnStepWidth=nAutoScrollStepWidth;

        if (pStepTime || pStepWidth)
        {
          UninitAutoScroll();
          if (nAutoScrollStepTime && nAutoScrollStepWidth)
            InitAutoScroll();
          dwSaveFlags|=OF_AUTOSCROLL;
        }
      }
    }

    if (bInitAutoScroll)
    {
      //Stay in memory, and show as active
      pd->nUnload=UD_NONUNLOAD_ACTIVE;
    }
    else
    {
      //If any function still loaded, stay in memory and show as non-active
      if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
    }
    return;
  }

  if (bInitAutoScroll)
  {
    UninitAutoScroll();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitAutoScroll();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) SyncHorz(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (bInitSyncHorz)
  {
    UninitMain();
    UninitSyncHorz();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitSyncHorz();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) SyncVert(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (bInitSyncVert)
  {
    UninitMain();
    UninitSyncVert();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitSyncVert();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) NoScroll(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (bInitNoScroll)
  {
    UninitMain();
    UninitNoScroll();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitNoScroll();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) AlignCaret(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (bInitAlignCaret)
  {
    UninitMain();
    UninitAlignCaret();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitAlignCaret();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) AutoFocus(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (bInitAutoFocus)
  {
    UninitMain();
    UninitAutoFocus();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitAutoFocus();

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

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (pd->bAkelEdit)
    {
      if (nAction == DLLA_SCROLL_HSCROLL ||
          nAction == DLLA_SCROLL_VSCROLL)
      {
        DWORD dwDirection=0;
        int nValue=-1;

        if (IsExtCallParamValid(pd->lParam, 2))
          nValue=(int)GetExtCallParam(pd->lParam, 2);

        if (nValue >= 0)
        {
          if (nAction == DLLA_SCROLL_HSCROLL)
            dwDirection=AESB_HORZ;
          else if (nAction == DLLA_SCROLL_VSCROLL)
            dwDirection=AESB_VERT;
          SendMessage(pd->hWndEdit, AEM_SCROLL, dwDirection, nValue);
        }
      }
      else if (nAction == DLLA_SCROLL_LINEHSCROLL ||
               nAction == DLLA_SCROLL_LINEVSCROLL)
      {
        AESELECTION aes;
        AECHARINDEX ciCaret;
        AESCROLLTOPOINT stp;
        POINT64 ptGlobalCaret;
        DWORD dwAlign=0;
        DWORD dwScrollBefore;
        DWORD dwScrollAfter;
        INT_PTR nPixelScrolled;
        DWORD dwDirection=0;
        int nValue1=0;
        int nValue2=0;

        if (IsExtCallParamValid(pd->lParam, 2))
          nValue1=(int)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          nValue2=(int)GetExtCallParam(pd->lParam, 3);

        if (nValue1)
        {
          if (nAction == DLLA_SCROLL_LINEHSCROLL)
            dwDirection=AESB_HORZ;
          else if (nAction == DLLA_SCROLL_LINEVSCROLL)
          {
            dwDirection=AESB_VERT;
            if (nValue1 > 0)
              dwAlign=AESB_ALIGNTOP;
            else if (nValue1 < 0)
              dwAlign=AESB_ALIGNBOTTOM;
          }

          if (nValue2 == MC_CARETNOMOVE)
          {
            SendMessage(pd->hWndEdit, AEM_LINESCROLL, dwDirection|dwAlign, nValue1);
          }
          else if (nValue2 == MC_CARETMOVEINSIDE || nValue2 == MC_CARETMOVEOUTSIDE)
          {
            stp.dwFlags=AESC_TEST|AESC_POINTCARET|AESC_OFFSETCHARX|AESC_OFFSETCHARY;
            stp.nOffsetX=0;
            stp.nOffsetY=0;

            dwScrollBefore=(DWORD)SendMessage(pd->hWndEdit, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);
            nPixelScrolled=SendMessage(pd->hWndEdit, AEM_LINESCROLL, dwDirection|dwAlign, nValue1);
            dwScrollAfter=(DWORD)SendMessage(pd->hWndEdit, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);

            if ((dwScrollAfter & AECSE_SCROLLEDX) || (dwScrollAfter & AECSE_SCROLLEDY))
            {
              SendMessage(pd->hWndEdit, AEM_GETCARETPOS, (WPARAM)NULL, (LPARAM)&ptGlobalCaret);
              if ((dwScrollBefore & AECSE_SCROLLEDX) || (dwScrollBefore & AECSE_SCROLLEDY))
              {
                POINT64 ptGlobalScroll;
                RECT rcDraw;

                SendMessage(pd->hWndEdit, AEM_GETSCROLLPOS, (WPARAM)NULL, (LPARAM)&ptGlobalScroll);
                SendMessage(pd->hWndEdit, AEM_GETRECT, 0, (LPARAM)&rcDraw);
                ptGlobalCaret.x=max(ptGlobalCaret.x, ptGlobalScroll.x);
                ptGlobalCaret.y=max(ptGlobalCaret.y, ptGlobalScroll.y);
                ptGlobalCaret.x=min(ptGlobalCaret.x, ptGlobalScroll.x + (rcDraw.right - rcDraw.left));
                ptGlobalCaret.y=min(ptGlobalCaret.y, ptGlobalScroll.y + (rcDraw.bottom - rcDraw.top));
              }
              else
              {
                if (dwDirection == AESB_HORZ)
                  ptGlobalCaret.x+=nPixelScrolled;
                else if (dwDirection == AESB_VERT)
                  ptGlobalCaret.y+=nPixelScrolled;
              }
              if (dwDirection == AESB_VERT)
                ptGlobalCaret.x=SendMessage(pd->hWndEdit, AEM_GETCARETHORZINDENT, 0, 0);
              SendMessage(pd->hWndEdit, AEM_CHARFROMGLOBALPOS, (WPARAM)&ptGlobalCaret, (LPARAM)&ciCaret);

              //Set new caret position
              aes.crSel.ciMin=ciCaret;
              aes.crSel.ciMax=ciCaret;
              aes.dwFlags=AESELT_LOCKSCROLL;
              if (dwDirection == AESB_VERT)
                aes.dwFlags|=AESELT_NOCARETHORZINDENT;
              aes.dwType=0;
              SendMessage(pd->hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
            }
          }
        }
      }
      else if (nAction == DLLA_SCROLL_ENTERSCROLL)
      {
        if (pd->hWndEdit != GetFocus())
        {
          pd->nUnload=UD_HOTKEY_DODEFAULT;
          if (pd->bInMemory) pd->nUnload|=UD_NONUNLOAD_NONACTIVE;
          return;
        }
        SendMessage(pd->hWndEdit, AEM_LOCKUPDATE, AELU_CARET, TRUE);
        SendMessage(pd->hWndEdit, WM_CHAR, VK_RETURN, 0);
        SendMessage(pd->hWndEdit, AEM_SCROLL, AESB_VERT, SB_LINEDOWN);
        SendMessage(pd->hWndEdit, AEM_LOCKUPDATE, AELU_CARET, FALSE);
        SendMessage(pd->hWndEdit, AEM_UPDATECARET, 0, 0);
      }
    }
  }
  else
  {
    SettingsSheet(0);
  }

  //If plugin already loaded, stay in memory, but show as non-active
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (uMsg == AKDN_EDIT_ONSTART)
  {
    if (bInitAlignCaret)
    {
      ScrollCaretOptions((HWND)wParam, NULL, FALSE);
    }
  }
  else if (uMsg == AKDN_EDIT_ONFINISH ||
           uMsg == AKDN_EDIT_ONCLOSE)
  {
    if (bInitAutoScroll)
    {
      if (hDocAutoScroll == (AEHDOC)lParam)
      {
        KillScrollTimer();
        hWndAutoScroll=NULL;
        hDocAutoScroll=NULL;
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (bInitNoScroll)
    {
      if (LOWORD(wParam) == IDM_EDIT_SELECTALL && (dwNoScroll & NS_SELECTALL))
      {
        HWND hWndEdit;
        LRESULT lResult;
        int nLockScroll;

        if (bAkelEdit)
        {
          if (hWndEdit=GetCurEdit())
          {
            if ((nLockScroll=(int)SendMessage(hWndEdit, AEM_LOCKSCROLL, (WPARAM)-1, 0)) == -1)
              SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, TRUE);
            lResult=NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
            if (nLockScroll == -1)
              SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, FALSE);
            return lResult;
          }
        }
      }
    }
  }
  if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
    return lResult;

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT)
    {
      if ((bInitSyncHorz && ((NMHDR *)lParam)->code == AEN_HSCROLL && (dwSyncHorz & SNC_MDI)) ||
          (bInitSyncVert && ((NMHDR *)lParam)->code == AEN_VSCROLL && (dwSyncVert & SNC_MDI)))
      {
        AENSCROLL *aens=(AENSCROLL *)lParam;
        HWND hWndChildFrame;
        HWND hWndChildEdit;
        POINT64 ptGlobal;
        static BOOL bLock=FALSE;
        BOOL bMdiMaximize;

        if (nMDI == WMD_MDI && SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize) && !bMdiMaximize)
        {
          if (!bLock)
          {
            bLock=TRUE;
            hWndChildFrame=GetWindow(hMdiClient, GW_CHILD);

            while (hWndChildFrame)
            {
              if (hWndChildFrame != hWnd)
              {
                hWndChildEdit=GetDlgItem(hWndChildFrame, ID_EDIT);

                SendMessage(hWndChildEdit, AEM_GETSCROLLPOS, 0, (LPARAM)&ptGlobal);
                if (((NMHDR *)lParam)->code == AEN_HSCROLL)
                  ptGlobal.x+=aens->nPosNew - aens->nPosOld;
                else
                  ptGlobal.y+=aens->nPosNew - aens->nPosOld;
                SendMessage(hWndChildEdit, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobal);
              }
              hWndChildFrame=GetNextWindow(hWndChildFrame, GW_HWNDNEXT);
            }
            bLock=FALSE;
          }
        }
      }
    }
  }
  if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
    return lResult;

  //Call next procedure
  return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT)
    {
      if ((bInitSyncHorz && ((NMHDR *)lParam)->code == AEN_HSCROLL && (dwSyncHorz & SNC_CLONE)) ||
          (bInitSyncVert && ((NMHDR *)lParam)->code == AEN_VSCROLL && (dwSyncVert & SNC_CLONE)))
      {
        AENSCROLL *aens=(AENSCROLL *)lParam;
        HWND hWndMaster;
        HWND hWndChildEdit;
        POINT64 ptGlobal;
        int nClone=0;
        static BOOL bLock=FALSE;

        if (!bLock)
        {
          bLock=TRUE;

          if (hWndMaster=(HWND)SendMessage(aens->hdr.hwndFrom, AEM_GETMASTER, 0, 0))
          {
            hWndChildEdit=hWndMaster;

            while (hWndChildEdit)
            {
              if (hWndChildEdit != aens->hdr.hwndFrom)
              {
                SendMessage(hWndChildEdit, AEM_GETSCROLLPOS, 0, (LPARAM)&ptGlobal);
                if (((NMHDR *)lParam)->code == AEN_HSCROLL)
                  ptGlobal.x+=aens->nPosNew - aens->nPosOld;
                else
                  ptGlobal.y+=aens->nPosNew - aens->nPosOld;
                SendMessage(hWndChildEdit, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobal);
              }
              hWndChildEdit=(HWND)SendMessage(hWndMaster, AEM_GETCLONE, nClone++, 0);
            }
          }
          bLock=FALSE;
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGING)
      {
        AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;
        DWORD dwOptions;

        if ((bInitNoScroll && (aentc->dwType & AETCT_UNDO) && (dwNoScroll & NS_UNDO)) ||
            (bInitNoScroll && (aentc->dwType & AETCT_REDO) && (dwNoScroll & NS_REDO)))
        {
          dwOptions=(DWORD)SendMessage(aentc->hdr.hwndFrom, AEM_GETOPTIONS, 0, 0);
          if (!(dwOptions & AECO_NOSCROLLDELETEALL))
            SendMessage(aentc->hdr.hwndFrom, AEM_SETOPTIONS, AECOOP_OR, AECO_NOSCROLLDELETEALL);
        }
      }
    }
  }
  return FALSE;
}

//// Settings sheet

void SettingsSheet(int nStartPage)
{
  HICON hIconAutoScroll=NULL;
  HICON hIconSyncHorz=NULL;
  HICON hIconSyncVert=NULL;
  HICON hIconNoScroll=NULL;
  HICON hIconAlignCaret=NULL;
  HICON hIconAutoFocus=NULL;
  POINT ptSmallIcon;

  //Create image list
  ptSmallIcon.x=16 /*GetSystemMetrics(SM_CXSMICON)*/;
  ptSmallIcon.y=16 /*GetSystemMetrics(SM_CYSMICON)*/;

  if (hImageList=ImageList_Create(ptSmallIcon.x, ptSmallIcon.y, ILC_COLOR32|ILC_MASK, 0, 0))
  {
    hIconAutoScroll=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_02), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconSyncHorz=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_00), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconSyncVert=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_01), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconNoScroll=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_03), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconAlignCaret=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_04), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconAutoFocus=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_05), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    ImageList_AddIcon(hImageList, hIconAutoScroll);
    ImageList_AddIcon(hImageList, hIconSyncHorz);
    ImageList_AddIcon(hImageList, hIconSyncVert);
    ImageList_AddIcon(hImageList, hIconNoScroll);
    ImageList_AddIcon(hImageList, hIconAlignCaret);
    ImageList_AddIcon(hImageList, hIconAutoFocus);
  }

  //Show property sheet
  psp[PAGE_AUTOSCROLL].dwSize       =sizeof(PROPSHEETPAGEW);
  psp[PAGE_AUTOSCROLL].dwFlags      =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_AUTOSCROLL].hInstance    =hInstanceDLL;
  psp[PAGE_AUTOSCROLL].pszTemplate  =MAKEINTRESOURCEW(IDD_AUTOSCROLL_SETUP);
  psp[PAGE_AUTOSCROLL].hIcon        =hIconAutoScroll;
  psp[PAGE_AUTOSCROLL].pfnDlgProc   =(DLGPROC)AutoScrollSetupDlgProc;

  psp[PAGE_SYNCHORZ].dwSize       =sizeof(PROPSHEETPAGEW);
  psp[PAGE_SYNCHORZ].dwFlags      =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_SYNCHORZ].hInstance    =hInstanceDLL;
  psp[PAGE_SYNCHORZ].pszTemplate  =MAKEINTRESOURCEW(IDD_SYNCHORZ_SETUP);
  psp[PAGE_SYNCHORZ].hIcon        =hIconSyncHorz;
  psp[PAGE_SYNCHORZ].pfnDlgProc   =(DLGPROC)SyncHorzSetupDlgProc;

  psp[PAGE_SYNCVERT].dwSize      =sizeof(PROPSHEETPAGEW);
  psp[PAGE_SYNCVERT].dwFlags     =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_SYNCVERT].hInstance   =hInstanceDLL;
  psp[PAGE_SYNCVERT].pszTemplate =MAKEINTRESOURCEW(IDD_SYNCVERT_SETUP);
  psp[PAGE_SYNCVERT].hIcon       =hIconSyncVert;
  psp[PAGE_SYNCVERT].pfnDlgProc  =(DLGPROC)SyncVertSetupDlgProc;

  psp[PAGE_NOSCROLL].dwSize       =sizeof(PROPSHEETPAGEW);
  psp[PAGE_NOSCROLL].dwFlags      =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_NOSCROLL].hInstance    =hInstanceDLL;
  psp[PAGE_NOSCROLL].pszTemplate  =MAKEINTRESOURCEW(IDD_NOSCROLL_SETUP);
  psp[PAGE_NOSCROLL].hIcon        =hIconNoScroll;
  psp[PAGE_NOSCROLL].pfnDlgProc   =(DLGPROC)NoScrollSetupDlgProc;

  psp[PAGE_ALIGNCARET].dwSize       =sizeof(PROPSHEETPAGEW);
  psp[PAGE_ALIGNCARET].dwFlags      =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_ALIGNCARET].hInstance    =hInstanceDLL;
  psp[PAGE_ALIGNCARET].pszTemplate  =MAKEINTRESOURCEW(IDD_ALIGNCARET_SETUP);
  psp[PAGE_ALIGNCARET].hIcon        =hIconAlignCaret;
  psp[PAGE_ALIGNCARET].pfnDlgProc   =(DLGPROC)AlignCaretSetupDlgProc;

  psp[PAGE_AUTOFOCUS].dwSize       =sizeof(PROPSHEETPAGEW);
  psp[PAGE_AUTOFOCUS].dwFlags      =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_AUTOFOCUS].hInstance    =hInstanceDLL;
  psp[PAGE_AUTOFOCUS].pszTemplate  =MAKEINTRESOURCEW(IDD_AUTOFOCUS_SETUP);
  psp[PAGE_AUTOFOCUS].hIcon        =hIconAutoFocus;
  psp[PAGE_AUTOFOCUS].pfnDlgProc   =(DLGPROC)AutoFocusSetupDlgProc;

  psh.dwSize      =sizeof(PROPSHEETHEADERW);
  psh.dwFlags     =PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_USECALLBACK|PSH_NOAPPLYNOW;
  psh.hwndParent  =hMainWnd;
  psh.hInstance   =hInstanceEXE;
  psh.pszIcon     =MAKEINTRESOURCEW(IDI_ICON_MAIN);
  psh.pszCaption  =wszPluginTitle;
  psh.nPages      =PAGE_MAX;
  psh.nStartPage  =nStartPage;
  psh.ppsp        =&psp[0];
  psh.pfnCallback =PropSheetProc;

  hHookOptions=SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());
  PropertySheetWide(&psh);

  if (dwSaveFlags)
  {
    if (dwSaveFlags & OF_AUTOSCROLL)
    {
      if (bInitAutoScroll)
      {
        UninitAutoScroll();
        InitAutoScroll();
      }
    }
    if (dwSaveFlags & OF_SYNCHORZ)
    {
      if (bInitSyncHorz)
      {
        UninitSyncHorz();
        InitSyncHorz();
      }
    }
    if (dwSaveFlags & OF_SYNCVERT)
    {
      if (bInitSyncVert)
      {
        UninitSyncVert();
        InitSyncVert();
      }
    }
    if (dwSaveFlags & OF_NOSCROLL)
    {
      if (bInitNoScroll)
      {
        UninitNoScroll();
        InitNoScroll();
      }
    }
    if (dwSaveFlags & OF_ALIGNCARET)
    {
      if (bInitAlignCaret)
      {
        InitAlignCaret();
      }
    }
    SaveOptions(dwSaveFlags);
    dwSaveFlags=0;
  }

  //Destroy image list
  if (hImageList)
  {
    if (ImageList_Destroy(hImageList))
      hImageList=NULL;
  }
}

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

    SetWindowPos((HWND)wParam, NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
  }
  return CallNextHookEx(hHookOptions, iCode, wParam, lParam);
}

int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam)
{
  static HWND hWndPropTab;

  if (uMsg == PSCB_PRECREATE)
  {
    //Remove "?"
    ((DLGTEMPLATE *)lParam)->style&=~DS_CONTEXTHELP;
  }
  else if (uMsg == PSCB_INITIALIZED)
  {
    HIMAGELIST hImageListOld;

    hWndPropSheet=hDlg;

    //Set 32-bit hImageList
    if (hWndPropTab=(HWND)SendMessage(hDlg, PSM_GETTABCONTROL, 0, 0))
    {
      hImageListOld=(HIMAGELIST)SendMessage(hWndPropTab, TCM_SETIMAGELIST, 0, (LPARAM)hImageList);
      if (hImageListOld) ImageList_Destroy(hImageListOld);
    }
  }
  else if (uMsg == PSCB_BUTTONPRESSED)
  {
    if (lParam == PSBTN_OK ||
        lParam == PSBTN_CANCEL ||
        lParam == PSBTN_FINISH)
    {
      //Detach hImageList otherwise ImageList_Destroy failed
      SendMessage(hWndPropTab, TCM_SETIMAGELIST, 0, (LPARAM)NULL);
    }
  }
  return TRUE;
}

BOOL CALLBACK AutoScrollSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndAutoScrollTitle;
  static HWND hWndStepTimeEdit;
  static HWND hWndStepTimeSpin;
  static HWND hWndStepWidthEdit;
  static HWND hWndStepWidthSpin;

  if (uMsg == WM_INITDIALOG)
  {
    hWndAutoScrollTitle=GetDlgItem(hDlg, IDC_AUTOSCROLL_TITLE);
    hWndStepTimeEdit=GetDlgItem(hDlg, IDC_AUTOSCROLL_STEPTIME_EDIT);
    hWndStepTimeSpin=GetDlgItem(hDlg, IDC_AUTOSCROLL_STEPTIME_SPIN);
    hWndStepWidthEdit=GetDlgItem(hDlg, IDC_AUTOSCROLL_STEPWIDTH_EDIT);
    hWndStepWidthSpin=GetDlgItem(hDlg, IDC_AUTOSCROLL_STEPWIDTH_SPIN);

    SetDlgItemTextWide(hDlg, IDC_AUTOSCROLL_AUTOSTEP_GROUP, GetLangStringW(wLangModule, STRID_AUTOSTEP));
    SetDlgItemTextWide(hDlg, IDC_AUTOSCROLL_STEPTIME_LABEL, GetLangStringW(wLangModule, STRID_STEPTIME));
    SetDlgItemTextWide(hDlg, IDC_AUTOSCROLL_STEPWIDTH_LABEL, GetLangStringW(wLangModule, STRID_STEPWIDTH));

    SendMessage(hWndStepTimeSpin, UDM_SETRANGE, 0, MAKELONG(9999, 0));
    SendMessage(hWndStepTimeSpin, UDM_SETBUDDY, (WPARAM)hWndStepTimeEdit, 0);
    SendMessage(hWndStepTimeEdit, EM_LIMITTEXT, 4, 0);
    SetDlgItemInt(hDlg, IDC_AUTOSCROLL_STEPTIME_EDIT, nAutoScrollStepTime, TRUE);

    SendMessage(hWndStepWidthSpin, UDM_SETRANGE, 0, MAKELONG(9999, 0));
    SendMessage(hWndStepWidthSpin, UDM_SETBUDDY, (WPARAM)hWndStepWidthEdit, 0);
    SendMessage(hWndStepWidthEdit, EM_LIMITTEXT, 4, 0);
    SetDlgItemInt(hDlg, IDC_AUTOSCROLL_STEPWIDTH_EDIT, nAutoScrollStepWidth, TRUE);

    if (!bInitAutoScroll) EnableWindow(hWndAutoScrollTitle, FALSE);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      nAutoScrollStepTime=GetDlgItemInt(hDlg, IDC_AUTOSCROLL_STEPTIME_EDIT, NULL, TRUE);
      nAutoScrollStepWidth=GetDlgItemInt(hDlg, IDC_AUTOSCROLL_STEPWIDTH_EDIT, NULL, TRUE);

      dwSaveFlags|=OF_AUTOSCROLL;
    }
  }
  return FALSE;
}

BOOL CALLBACK SyncHorzSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndSyncHorzTitle;
  static HWND hWndSyncHorzMDI;
  static HWND hWndSyncHorzClone;

  if (uMsg == WM_INITDIALOG)
  {
    hWndSyncHorzTitle=GetDlgItem(hDlg, IDC_SYNCHORZ_TITLE);
    hWndSyncHorzMDI=GetDlgItem(hDlg, IDC_SYNCHORZ_MDI);
    hWndSyncHorzClone=GetDlgItem(hDlg, IDC_SYNCHORZ_CLONE);

    SetDlgItemTextWide(hDlg, IDC_SYNCHORZ_GROUP, GetLangStringW(wLangModule, STRID_HSYNCHRONIZATION));
    SetDlgItemTextWide(hDlg, IDC_SYNCHORZ_MDI, GetLangStringW(wLangModule, STRID_FRAMEMDI));
    SetDlgItemTextWide(hDlg, IDC_SYNCHORZ_CLONE, GetLangStringW(wLangModule, STRID_SPLITPANE));

    if (!bInitSyncHorz) EnableWindow(hWndSyncHorzTitle, FALSE);

    if (dwSyncHorz & SNC_MDI)
      SendMessage(hWndSyncHorzMDI, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSyncHorz & SNC_CLONE)
      SendMessage(hWndSyncHorzClone, BM_SETCHECK, BST_CHECKED, 0);

    if (!bAkelEdit || nMDI == WMD_SDI || nMDI == WMD_PMDI)
      EnableWindow(hWndSyncHorzMDI, FALSE);
    if (!bAkelEdit)
      EnableWindow(hWndSyncHorzClone, FALSE);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      dwSyncHorz=0;
      if (SendMessage(hWndSyncHorzMDI, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSyncHorz|=SNC_MDI;
      if (SendMessage(hWndSyncHorzClone, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSyncHorz|=SNC_CLONE;

      dwSaveFlags|=OF_SYNCHORZ;
    }
  }
  return FALSE;
}

BOOL CALLBACK SyncVertSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndSyncVertTitle;
  static HWND hWndSyncVertMDI;
  static HWND hWndSyncVertClone;

  if (uMsg == WM_INITDIALOG)
  {
    hWndSyncVertTitle=GetDlgItem(hDlg, IDC_SYNCVERT_TITLE);
    hWndSyncVertMDI=GetDlgItem(hDlg, IDC_SYNCVERT_MDI);
    hWndSyncVertClone=GetDlgItem(hDlg, IDC_SYNCVERT_CLONE);

    SetDlgItemTextWide(hDlg, IDC_SYNCVERT_GROUP, GetLangStringW(wLangModule, STRID_VSYNCHRONIZATION));
    SetDlgItemTextWide(hDlg, IDC_SYNCVERT_MDI, GetLangStringW(wLangModule, STRID_FRAMEMDI));
    SetDlgItemTextWide(hDlg, IDC_SYNCVERT_CLONE, GetLangStringW(wLangModule, STRID_SPLITPANE));

    if (!bInitSyncVert) EnableWindow(hWndSyncVertTitle, FALSE);

    if (dwSyncVert & SNC_MDI)
      SendMessage(hWndSyncVertMDI, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSyncVert & SNC_CLONE)
      SendMessage(hWndSyncVertClone, BM_SETCHECK, BST_CHECKED, 0);

    if (!bAkelEdit || nMDI == WMD_SDI || nMDI == WMD_PMDI)
      EnableWindow(hWndSyncVertMDI, FALSE);
    if (!bAkelEdit)
      EnableWindow(hWndSyncVertClone, FALSE);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      dwSyncVert=0;
      if (SendMessage(hWndSyncVertMDI, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSyncVert|=SNC_MDI;
      if (SendMessage(hWndSyncVertClone, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSyncVert|=SNC_CLONE;

      dwSaveFlags|=OF_SYNCVERT;
    }
  }
  return FALSE;
}

BOOL CALLBACK NoScrollSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndNoScrollTitle;
  static HWND hWndUndo;
  static HWND hWndRedo;
  static HWND hWndSelectAll;

  if (uMsg == WM_INITDIALOG)
  {
    hWndNoScrollTitle=GetDlgItem(hDlg, IDC_NOSCROLL_TITLE);
    hWndUndo=GetDlgItem(hDlg, IDC_NOSCROLL_UNDO);
    hWndRedo=GetDlgItem(hDlg, IDC_NOSCROLL_REDO);
    hWndSelectAll=GetDlgItem(hDlg, IDC_NOSCROLL_SELECTALL);

    SetDlgItemTextWide(hDlg, IDC_NOSCROLL_GROUP, GetLangStringW(wLangModule, STRID_NOSCROLL));
    SetDlgItemTextWide(hDlg, IDC_NOSCROLL_UNDO, GetLangStringW(wLangModule, STRID_UNDO));
    SetDlgItemTextWide(hDlg, IDC_NOSCROLL_REDO, GetLangStringW(wLangModule, STRID_REDO));
    SetDlgItemTextWide(hDlg, IDC_NOSCROLL_SELECTALL, GetLangStringW(wLangModule, STRID_SELECTALL));

    if (!bInitNoScroll) EnableWindow(hWndNoScrollTitle, FALSE);

    if (dwNoScroll & NS_UNDO)
      SendMessage(hWndUndo, BM_SETCHECK, BST_CHECKED, 0);
    if (dwNoScroll & NS_REDO)
      SendMessage(hWndRedo, BM_SETCHECK, BST_CHECKED, 0);
    if (dwNoScroll & NS_SELECTALL)
      SendMessage(hWndSelectAll, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      dwNoScroll=0;
      if (SendMessage(hWndUndo, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwNoScroll|=NS_UNDO;
      if (SendMessage(hWndRedo, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwNoScroll|=NS_REDO;
      if (SendMessage(hWndSelectAll, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwNoScroll|=NS_SELECTALL;

      dwSaveFlags|=OF_NOSCROLL;
    }
  }
  return FALSE;
}

BOOL CALLBACK AlignCaretSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndAlignCaretTitle;
  static HWND hWndOffsetXEdit;
  static HWND hWndOffsetXSpin;
  static HWND hWndOffsetYEdit;
  static HWND hWndOffsetYSpin;

  if (uMsg == WM_INITDIALOG)
  {
    hWndAlignCaretTitle=GetDlgItem(hDlg, IDC_ALIGNCARET_TITLE);
    hWndOffsetXEdit=GetDlgItem(hDlg, IDC_ALIGNCARET_OFFSETX_EDIT);
    hWndOffsetXSpin=GetDlgItem(hDlg, IDC_ALIGNCARET_OFFSETX_SPIN);
    hWndOffsetYEdit=GetDlgItem(hDlg, IDC_ALIGNCARET_OFFSETY_EDIT);
    hWndOffsetYSpin=GetDlgItem(hDlg, IDC_ALIGNCARET_OFFSETY_SPIN);

    SetDlgItemTextWide(hDlg, IDC_ALIGNCARET_GROUP, GetLangStringW(wLangModule, STRID_ALIGNCARET));
    SetDlgItemTextWide(hDlg, IDC_ALIGNCARET_OFFSETX_LABEL, GetLangStringW(wLangModule, STRID_OFFSETX));
    SetDlgItemTextWide(hDlg, IDC_ALIGNCARET_OFFSETY_LABEL, GetLangStringW(wLangModule, STRID_OFFSETY));

    SendMessage(hWndOffsetXSpin, UDM_SETRANGE, 0, MAKELONG(9999, 0));
    SendMessage(hWndOffsetXSpin, UDM_SETBUDDY, (WPARAM)hWndOffsetXEdit, 0);
    SendMessage(hWndOffsetXEdit, EM_LIMITTEXT, 4, 0);
    SetDlgItemInt(hDlg, IDC_ALIGNCARET_OFFSETX_EDIT, nAlignCaretOffsetX, TRUE);

    SendMessage(hWndOffsetYSpin, UDM_SETRANGE, 0, MAKELONG(9999, 0));
    SendMessage(hWndOffsetYSpin, UDM_SETBUDDY, (WPARAM)hWndOffsetYEdit, 0);
    SendMessage(hWndOffsetYEdit, EM_LIMITTEXT, 4, 0);
    SetDlgItemInt(hDlg, IDC_ALIGNCARET_OFFSETY_EDIT, nAlignCaretOffsetY, TRUE);

    if (!bInitAlignCaret) EnableWindow(hWndAlignCaretTitle, FALSE);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      nAlignCaretOffsetX=GetDlgItemInt(hDlg, IDC_ALIGNCARET_OFFSETX_EDIT, NULL, TRUE);
      nAlignCaretOffsetY=GetDlgItemInt(hDlg, IDC_ALIGNCARET_OFFSETY_EDIT, NULL, TRUE);

      dwSaveFlags|=OF_ALIGNCARET;
    }
  }
  return FALSE;
}

BOOL CALLBACK AutoFocusSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndAutoFocusTitle;
  static HWND hWndFocusBackground;
  static HWND hWndMoveScrollbar;
  static HWND hWndMoveScrollbarInvert;
  static HWND hWndMoveWithShift;
  static HWND hWndMoveWithShiftInvert;
  static HWND hWndSwitchTab;
  static HWND hWndSwitchTabInvert;
  static HWND hWndSwitchTabWithSpin;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    hWndAutoFocusTitle=GetDlgItem(hDlg, IDC_AUTOFOCUS_TITLE);
    hWndFocusBackground=GetDlgItem(hDlg, IDC_AUTOFOCUS_FOCUSBACKGROUND);
    hWndMoveScrollbar=GetDlgItem(hDlg, IDC_AUTOFOCUS_MOVESCROLLBAR);
    hWndMoveScrollbarInvert=GetDlgItem(hDlg, IDC_AUTOFOCUS_MOVESCROLLBAR_INVERT);
    hWndMoveWithShift=GetDlgItem(hDlg, IDC_AUTOFOCUS_MOVEWITHSHIFT);
    hWndMoveWithShiftInvert=GetDlgItem(hDlg, IDC_AUTOFOCUS_MOVEWITHSHIFT_INVERT);
    hWndSwitchTab=GetDlgItem(hDlg, IDC_AUTOFOCUS_SWITCHTAB);
    hWndSwitchTabInvert=GetDlgItem(hDlg, IDC_AUTOFOCUS_SWITCHTAB_INVERT);
    hWndSwitchTabWithSpin=GetDlgItem(hDlg, IDC_AUTOFOCUS_SWITCHTAB_WITHSPIN);

    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_GROUP, GetLangStringW(wLangModule, STRID_AUTOFOCUS));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_FOCUSBACKGROUND, GetLangStringW(wLangModule, STRID_FOCUSBACKGROUND));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_MOVESCROLLBAR, GetLangStringW(wLangModule, STRID_MOVESCROLLBAR));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_MOVESCROLLBAR_INVERT, GetLangStringW(wLangModule, STRID_INVERT));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_MOVEWITHSHIFT, GetLangStringW(wLangModule, STRID_MOVEWITHSHIFT));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_MOVEWITHSHIFT_INVERT, GetLangStringW(wLangModule, STRID_INVERT));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_SWITCHTAB, GetLangStringW(wLangModule, STRID_SWITCHTAB));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_SWITCHTAB_INVERT, GetLangStringW(wLangModule, STRID_INVERT));
    SetDlgItemTextWide(hDlg, IDC_AUTOFOCUS_SWITCHTAB_WITHSPIN, GetLangStringW(wLangModule, STRID_WITHSPIN));

    if (!bInitAutoFocus) EnableWindow(hWndAutoFocusTitle, FALSE);

    if (dwAutoFocus & AF_FOCUSBACKGROUND)
      SendMessage(hWndFocusBackground, BM_SETCHECK, BST_CHECKED, 0);
    if (dwAutoFocus & AF_MOVESCROLLBAR)
      SendMessage(hWndMoveScrollbar, BM_SETCHECK, BST_CHECKED, 0);
    if (dwAutoFocus & AF_MOVESCROLLBARINVERT)
      SendMessage(hWndMoveScrollbarInvert, BM_SETCHECK, BST_CHECKED, 0);
    if (dwAutoFocus & AF_MOVEWITHSHIFT)
      SendMessage(hWndMoveWithShift, BM_SETCHECK, BST_CHECKED, 0);
    if (dwAutoFocus & AF_MOVEWITHSHIFTINVERT)
      SendMessage(hWndMoveWithShiftInvert, BM_SETCHECK, BST_CHECKED, 0);
    if (dwAutoFocus & AF_SWITCHTAB)
      SendMessage(hWndSwitchTab, BM_SETCHECK, BST_CHECKED, 0);
    if (dwAutoFocus & AF_SWITCHTABINVERT)
      SendMessage(hWndSwitchTabInvert, BM_SETCHECK, BST_CHECKED, 0);
    if (dwAutoFocus & AF_SWITCHTABWITHSPIN)
      SendMessage(hWndSwitchTabWithSpin, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hDlg, WM_COMMAND, IDC_AUTOFOCUS_MOVESCROLLBAR, 0);
    SendMessage(hDlg, WM_COMMAND, IDC_AUTOFOCUS_MOVEWITHSHIFT, 0);
    SendMessage(hDlg, WM_COMMAND, IDC_AUTOFOCUS_SWITCHTAB, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_AUTOFOCUS_MOVESCROLLBAR)
    {
      bState=(BOOL)SendMessage(hWndMoveScrollbar, BM_GETCHECK, 0, 0);
      EnableWindow(hWndMoveScrollbarInvert, bState);
    }
    else if (LOWORD(wParam) == IDC_AUTOFOCUS_MOVEWITHSHIFT)
    {
      bState=(BOOL)SendMessage(hWndMoveWithShift, BM_GETCHECK, 0, 0);
      EnableWindow(hWndMoveWithShiftInvert, bState);
    }
    else if (LOWORD(wParam) == IDC_AUTOFOCUS_SWITCHTAB)
    {
      bState=(BOOL)SendMessage(hWndSwitchTab, BM_GETCHECK, 0, 0);
      EnableWindow(hWndSwitchTabInvert, bState);
      EnableWindow(hWndSwitchTabWithSpin, bState);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      dwAutoFocus=0;
      if (SendMessage(hWndFocusBackground, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_FOCUSBACKGROUND;
      if (SendMessage(hWndMoveScrollbar, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_MOVESCROLLBAR;
      if (SendMessage(hWndMoveScrollbarInvert, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_MOVESCROLLBARINVERT;
      if (SendMessage(hWndMoveWithShift, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_MOVEWITHSHIFT;
      if (SendMessage(hWndMoveWithShiftInvert, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_MOVEWITHSHIFTINVERT;
      if (SendMessage(hWndSwitchTab, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_SWITCHTAB;
      if (SendMessage(hWndSwitchTabInvert, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_SWITCHTABINVERT;
      if (SendMessage(hWndSwitchTabWithSpin, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwAutoFocus|=AF_SWITCHTABWITHSPIN;

      dwSaveFlags|=OF_AUTOFOCUS;
    }
  }
  return FALSE;
}

void SetScrollTimer()
{
  if (!dwAutoScrollTimer)
  {
    //if (hWndAutoScroll) HideCaret(hWndAutoScroll);
    dwAutoScrollTimer=SetTimer(NULL, 0, nAutoScrollStepTime, (TIMERPROC)TimerProc);
  }
}

void KillScrollTimer()
{
  if (dwAutoScrollTimer)
  {
    KillTimer(NULL, dwAutoScrollTimer);
    dwAutoScrollTimer=0;
    //if (hWndAutoScroll) ShowCaret(hWndAutoScroll);
  }
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  if (hDocAutoScroll)
  {
    if (hDocAutoScroll == GetCurDoc())
    {
      SCROLLINFO si;
      POINT64 ptGlobal;

      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_PAGE|SIF_POS|SIF_RANGE;
      GetScrollInfo(hWndAutoScroll, SB_VERT, &si);

      if (si.nPage > 0 && si.nPos + (int)si.nPage < si.nMax)
      {
        SendMessage(hWndAutoScroll, AEM_GETSCROLLPOS, 0, (LPARAM)&ptGlobal);
        ptGlobal.y+=nAutoScrollStepWidth;
        SendMessage(hWndAutoScroll, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobal);
      }
    }
  }
  else
  {
    hWndAutoScroll=GetCurEdit();
    hDocAutoScroll=GetCurDoc();
  }
}

void GetMsgProcCommon(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    MSG *msg=(MSG *)lParam;

    if (msg->message == WM_MOUSEWHEEL ||
        msg->message == WM_MOUSEHWHEEL)
    {
      HWND hWndPoint;
      POINT ptPos;
      POINT ptClient;

      GetCursorPos(&ptPos);

      if (hWndPoint=WindowFromPoint(ptPos))
      {
        if (hWndPoint == hMainWnd)
        {
          ptClient=ptPos;
          ScreenToClient(hMainWnd, &ptClient);
          hWndPoint=ChildWindowFromPoint(hMainWnd, ptClient);
        }
        if (hWndPoint && GetClassNameWide(hWndPoint, wszClassName, MAX_PATH))
        {
          if (!xstrcmpiW(wszClassName, L"SysTabControl32"))
          {
            if (dwAutoFocus & AF_SWITCHTAB)
            {
              HWND hTabSpin=NULL;
              NMHDR nmhdr;
              int nTabCurIndex;
              int nTabNewIndex;

              if (!(dwAutoFocus & AF_SWITCHTABWITHSPIN) || ((hTabSpin=GetWindow(hWndPoint, GW_CHILD)) && IsWindowVisible(hTabSpin)))
              {
                if (dwAutoFocus & AF_SWITCHTABWITHSPIN)
                {
                  nTabCurIndex=LOWORD(SendMessage(hTabSpin, UDM_GETPOS, 0, 0));
                  nTabNewIndex=nTabCurIndex;
                }
                else
                {
                  nmhdr.hwndFrom=hWndPoint;
                  nmhdr.idFrom=GetDlgCtrlID(hWndPoint);
                  nmhdr.code=TCN_SELCHANGE;

                  nTabCurIndex=(int)SendMessage(hWndPoint, TCM_GETCURSEL, 0, 0);
                  nTabNewIndex=nTabCurIndex;
                }

                if ((!(dwAutoFocus & AF_SWITCHTABINVERT) && (short)HIWORD(msg->wParam) < 0) ||
                    ((dwAutoFocus & AF_SWITCHTABINVERT) && (short)HIWORD(msg->wParam) >= 0))
                {
                  if (nTabCurIndex > 0)
                    --nTabNewIndex;
                }
                else
                {
                  if (nTabCurIndex < (int)(SendMessage(hWndPoint, TCM_GETITEMCOUNT, 0, 0) - 1))
                    ++nTabNewIndex;
                }
                if (nTabNewIndex != nTabCurIndex)
                {
                  if (dwAutoFocus & AF_SWITCHTABWITHSPIN)
                  {
                    SendMessage(hWndPoint, WM_HSCROLL, MAKELONG(SB_THUMBPOSITION, nTabNewIndex), 0);
                    SendMessage(hWndPoint, WM_HSCROLL, MAKELONG(SB_ENDSCROLL, 0), 0);
                  }
                  else
                  {
                    if (SendMessage(hWndPoint, TCM_SETCURSEL, (WPARAM)nTabNewIndex, 0) != -1)
                      SendMessage(GetParent(hWndPoint), WM_NOTIFY, (WPARAM)nmhdr.idFrom, (LPARAM)&nmhdr);
                  }
                }
                msg->message=WM_NULL;
              }
            }
          }
          else if (!xstrcmpinW(L"AkelEdit", wszClassName, (UINT_PTR)-1) ||
                   !xstrcmpinW(L"RichEdit20", wszClassName, (UINT_PTR)-1) ||
                   !xstrcmpiW(wszClassName, L"SysListView32") ||
                   !xstrcmpiW(wszClassName, L"SysTreeView32") ||
                   !xstrcmpiW(wszClassName, L"ListBox") ||
                   !xstrcmpiW(wszClassName, L"ComboBox") ||
                   !xstrcmpiW(wszClassName, L"Edit"))
          {
            UINT uMsg=msg->message;
            DWORD dwChars=0;
            int nHitTest=0;
            BOOL bInvert=FALSE;

            if (dwAutoFocus & AF_FOCUSBACKGROUND)
            {
              if (hWndPoint != msg->hwnd)
              {
                SetFocus(hWndPoint);
                msg->message=WM_NULL;
              }
            }

            if (msg->message == WM_MOUSEWHEEL && (((dwAutoFocus & AF_MOVESCROLLBAR) && (nHitTest=(int)SendMessage(hWndPoint, WM_NCHITTEST, 0, MAKELONG(ptPos.x, ptPos.y))) == HTHSCROLL) ||
                                                  ((dwAutoFocus & AF_MOVEWITHSHIFT) && (GetKeyState(VK_SHIFT) & 0x80))))
            {
              SystemParametersInfoA(SPI_GETWHEELSCROLLCHARS, 0, &dwChars, 0);
              if (!dwChars) dwChars=3;

              if (nHitTest == HTHSCROLL)
              {
                if (dwAutoFocus & AF_MOVESCROLLBARINVERT)
                  bInvert=TRUE;
              }
              else
              {
                if (dwAutoFocus & AF_MOVEWITHSHIFTINVERT)
                  bInvert=TRUE;
              }

              if ((!bInvert && (short)HIWORD(msg->wParam) < 0) ||
                  (bInvert && (short)HIWORD(msg->wParam) >= 0))
              {
                while (dwChars--) SendMessage(hWndPoint, WM_HSCROLL, SB_LINELEFT, (LPARAM)NULL);
              }
              else
              {
                while (dwChars--) SendMessage(hWndPoint, WM_HSCROLL, SB_LINERIGHT, (LPARAM)NULL);
              }
              msg->message=WM_NULL;
            }
            else if (!(dwAutoFocus & AF_FOCUSBACKGROUND))
            {
              if (hWndPoint != msg->hwnd)
              {
                SendMessage(hWndPoint, uMsg, msg->wParam, msg->lParam);
                msg->message=WM_NULL;
              }
            }
          }
        }
      }
    }
  }
}

LRESULT CALLBACK GetMsgProc1(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[0].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc2(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[1].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc3(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[2].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc4(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[3].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc5(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[4].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc6(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[5].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc7(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[6].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc8(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[7].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc9(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[8].hHook, code, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc10(int code, WPARAM wParam, LPARAM lParam)
{
  GetMsgProcCommon(code, wParam, lParam);
  return CallNextHookEx(ht[9].hHook, code, wParam, lParam);
}

void ScrollCaretOptions(HWND hWndEdit, AEHDOC hDocEdit, BOOL bCheckIfSet)
{
  AESCROLLCARETOPTIONS sco;

  sco.dwFlags=0;
  if (bCheckIfSet)
  {
    if (hDocEdit)
      SendToDoc(hDocEdit, hWndEdit, AEM_SCROLLCARETOPTIONS, FALSE, (LPARAM)&sco);
    else
      SendMessage(hWndEdit, AEM_SCROLLCARETOPTIONS, FALSE, (LPARAM)&sco);
  }

  if (!sco.dwFlags)
  {
    sco.dwFlags=nAlignCaretFlags;
    sco.dwSelFlags=nAlignCaretSelFlags;
    sco.dwSelType=nAlignCaretSelType;
    sco.nOffsetX=nAlignCaretOffsetX;
    sco.nOffsetY=nAlignCaretOffsetY;
    if (hDocEdit)
      SendToDoc(hDocEdit, hWndEdit, AEM_SCROLLCARETOPTIONS, TRUE, (LPARAM)&sco);
    else
      SendMessage(hWndEdit, AEM_SCROLLCARETOPTIONS, TRUE, (LPARAM)&sco);
  }
}

void ScrollCaretOptionsAll(BOOL bRemove)
{
  FRAMEDATA *lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYINDEX, 1);

  while (lpFrame)
  {
    if (bRemove)
    {
      if (nMDI == WMD_PMDI)
        SendToDoc(lpFrame->ei.hDocEdit, lpFrame->ei.hWndEdit, AEM_SCROLLCARETOPTIONS, TRUE, (LPARAM)NULL);
      else
        SendMessage(lpFrame->ei.hWndEdit, AEM_SCROLLCARETOPTIONS, TRUE, (LPARAM)NULL);
    }
    else ScrollCaretOptions(lpFrame->ei.hWndEdit, (nMDI == WMD_PMDI?lpFrame->ei.hDocEdit:NULL), FALSE);

    lpFrame=lpFrame->next;
  }
}

HWND GetCurEdit()
{
  return (HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL);
}

AEHDOC GetCurDoc()
{
  return (AEHDOC)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_DOCEDIT, (LPARAM)NULL);
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
    WideOption(hOptions, L"StepTime", PO_DWORD, (LPBYTE)&nAutoScrollStepTime, sizeof(DWORD));
    WideOption(hOptions, L"StepWidth", PO_DWORD, (LPBYTE)&nAutoScrollStepWidth, sizeof(DWORD));
    WideOption(hOptions, L"SyncHorz", PO_DWORD, (LPBYTE)&dwSyncHorz, sizeof(DWORD));
    WideOption(hOptions, L"SyncVert", PO_DWORD, (LPBYTE)&dwSyncVert, sizeof(DWORD));
    WideOption(hOptions, L"AlignCaretFlags", PO_DWORD, (LPBYTE)&nAlignCaretFlags, sizeof(DWORD));
    WideOption(hOptions, L"AlignCaretSelFlags", PO_DWORD, (LPBYTE)&nAlignCaretSelFlags, sizeof(DWORD));
    WideOption(hOptions, L"AlignCaretSelType", PO_DWORD, (LPBYTE)&nAlignCaretSelType, sizeof(DWORD));
    WideOption(hOptions, L"AlignCaretOffsetX", PO_DWORD, (LPBYTE)&nAlignCaretOffsetX, sizeof(DWORD));
    WideOption(hOptions, L"AlignCaretOffsetY", PO_DWORD, (LPBYTE)&nAlignCaretOffsetY, sizeof(DWORD));
    WideOption(hOptions, L"NoScroll", PO_DWORD, (LPBYTE)&dwNoScroll, sizeof(DWORD));
    WideOption(hOptions, L"AutoFocus", PO_DWORD, (LPBYTE)&dwAutoFocus, sizeof(DWORD));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_AUTOSCROLL)
    {
      WideOption(hOptions, L"StepTime", PO_DWORD, (LPBYTE)&nAutoScrollStepTime, sizeof(DWORD));
      WideOption(hOptions, L"StepWidth", PO_DWORD, (LPBYTE)&nAutoScrollStepWidth, sizeof(DWORD));
    }
    if (dwFlags & OF_SYNCHORZ)
    {
      WideOption(hOptions, L"SyncHorz", PO_DWORD, (LPBYTE)&dwSyncHorz, sizeof(DWORD));
    }
    if (dwFlags & OF_SYNCVERT)
    {
      WideOption(hOptions, L"SyncVert", PO_DWORD, (LPBYTE)&dwSyncVert, sizeof(DWORD));
    }
    if (dwFlags & OF_NOSCROLL)
    {
      WideOption(hOptions, L"NoScroll", PO_DWORD, (LPBYTE)&dwNoScroll, sizeof(DWORD));
    }
    if (dwFlags & OF_ALIGNCARET)
    {
      WideOption(hOptions, L"AlignCaretFlags", PO_DWORD, (LPBYTE)&nAlignCaretFlags, sizeof(DWORD));
      WideOption(hOptions, L"AlignCaretSelFlags", PO_DWORD, (LPBYTE)&nAlignCaretSelFlags, sizeof(DWORD));
      WideOption(hOptions, L"AlignCaretSelType", PO_DWORD, (LPBYTE)&nAlignCaretSelType, sizeof(DWORD));
      WideOption(hOptions, L"AlignCaretOffsetX", PO_DWORD, (LPBYTE)&nAlignCaretOffsetX, sizeof(DWORD));
      WideOption(hOptions, L"AlignCaretOffsetY", PO_DWORD, (LPBYTE)&nAlignCaretOffsetY, sizeof(DWORD));
    }
    if (dwFlags & OF_AUTOFOCUS)
    {
      WideOption(hOptions, L"AutoFocus", PO_DWORD, (LPBYTE)&dwAutoFocus, sizeof(DWORD));
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
    if (nStringID == STRID_AUTOSTEP)
      return L"\x0410\x0432\x0442\x043E\x043C\x0430\x0442\x0438\x0447\x0435\x0441\x043A\x0430\x044F\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0442\x043A\x0430";
    if (nStringID == STRID_ENABLE)
      return L"\x0412\x043A\x043B\x044E\x0447\x0435\x043D\x043E";
    if (nStringID == STRID_STEPTIME)
      return L"\x0412\x0440\x0435\x043C\x044F\x0020\x0448\x0430\x0433\x0430";
    if (nStringID == STRID_STEPWIDTH)
      return L"\x0428\x0438\x0440\x0438\x043D\x0430\x0020\x0448\x0430\x0433\x0430";
    if (nStringID == STRID_HSYNCHRONIZATION)
      return L"\x0413\x043E\x0440\x0438\x0437\x043E\x043D\x0442\x0430\x043B\x044C\x043D\x0430\x044F\x0020\x0441\x0438\x043D\x0445\x0440\x043E\x043D\x0438\x0437\x0430\x0446\x0438\x044F";
    if (nStringID == STRID_VSYNCHRONIZATION)
      return L"\x0412\x0435\x0440\x0442\x0438\x043A\x0430\x043B\x044C\x043D\x0430\x044F\x0020\x0441\x0438\x043D\x0445\x0440\x043E\x043D\x0438\x0437\x0430\x0446\x0438\x044F";
    if (nStringID == STRID_FRAMEMDI)
      return L"\x0412\x043A\x043B\x0430\x0434\x043A\x0438\x0020\x0028\x004D\x0044\x0049\x0029";
    if (nStringID == STRID_SPLITPANE)
      return L"\x0420\x0430\x0437\x0434\x0435\x043B\x0435\x043D\x043D\x044B\x0435\x0020\x043E\x043A\x043D\x0430";
    if (nStringID == STRID_NOSCROLL)
      return L"\x041D\x0435\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0447\x0438\x0432\x0430\x0442\x044C\x0020\x043E\x043F\x0435\x0440\x0430\x0446\x0438\x0438";
    if (nStringID == STRID_UNDO)
      return L"\x041E\x0442\x043C\x0435\x043D\x0438\x0442\x044C\x0020\x0028\x0432\x0435\x0441\x044C\x0020\x0442\x0435\x043A\x0441\x0442\x0029";
    if (nStringID == STRID_REDO)
      return L"\x041F\x043E\x0432\x0442\x043E\x0440\x0438\x0442\x044C\x0020\x0028\x0432\x0435\x0441\x044C\x0020\x0442\x0435\x043A\x0441\x0442\x0029";
    if (nStringID == STRID_SELECTALL)
      return L"\x0412\x044B\x0434\x0435\x043B\x0438\x0442\x044C\x0020\x0432\x0441\x0435";
    if (nStringID == STRID_ALIGNCARET)
      return L"\x0412\x044B\x0440\x0430\x0432\x043D\x0438\x0432\x0430\x043D\x0438\x0435\x0020\x043A\x0430\x0440\x0435\x0442\x043A\x0438";
    if (nStringID == STRID_OFFSETX)
      return L"X";
    if (nStringID == STRID_OFFSETY)
      return L"Y";
    if (nStringID == STRID_AUTOFOCUS)
      return L"\x0410\x0432\x0442\x043E\x043C\x0430\x0442\x0438\x0447\x0435\x0441\x043A\x0438\x0439\x0020\x0444\x043E\x043A\x0443\x0441";
    if (nStringID == STRID_FOCUSBACKGROUND)
      return L"\x041F\x0435\x0440\x0435\x0434\x0430\x0447\x0430\x0020\x0444\x043E\x043A\x0443\x0441\x0430";
    if (nStringID == STRID_MOVESCROLLBAR)
      return L"\x041F\x0440\x043E\x043A\x0440\x0443\x0442\x043A\x0430\x0020\x043F\x043E\x043B\x0437\x0443\x043D\x043A\x043E\x043C";
    if (nStringID == STRID_MOVEWITHSHIFT)
      return L"\x0413\x043E\x0440\x0438\x0437\x043E\x043D\x0442\x0430\x043B\x044C\x043D\x0430\x044F\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0442\x043A\x0430\x0020\x0441\x0020\x0053\x0068\x0069\x0066\x0074\x0027\x043E\x043C";
    if (nStringID == STRID_SWITCHTAB)
      return L"\x041F\x0435\x0440\x0435\x043A\x043B\x044E\x0447\x0435\x043D\x0438\x0435\x0020\x043C\x0435\x0436\x0434\x0443\x0020\x0432\x043A\x043B\x0430\x0434\x043A\x0430\x043C\x0438";
    if (nStringID == STRID_WITHSPIN)
      return L"\x041A\x043E\x0433\x0434\x0430\x0020\x043A\x043D\x043E\x043F\x043A\x0438\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0442\x043A\x0438\x0020\x0432\x0438\x0434\x0438\x043C\x044B";
    if (nStringID == STRID_INVERT)
      return L"\x0418\x043D\x0432\x0435\x0440\x0442\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_AUTOSTEP)
      return L"Automatic scrolling";
    if (nStringID == STRID_ENABLE)
      return L"Enable";
    if (nStringID == STRID_STEPTIME)
      return L"Step time";
    if (nStringID == STRID_STEPWIDTH)
      return L"Step width";
    if (nStringID == STRID_HSYNCHRONIZATION)
      return L"Horizontal synchronization";
    if (nStringID == STRID_VSYNCHRONIZATION)
      return L"Vertical synchronization";
    if (nStringID == STRID_FRAMEMDI)
      return L"Tabs (MDI)";
    if (nStringID == STRID_SPLITPANE)
      return L"Split panes";
    if (nStringID == STRID_NOSCROLL)
      return L"No scroll operations";
    if (nStringID == STRID_UNDO)
      return L"Undo (all text)";
    if (nStringID == STRID_REDO)
      return L"Redo (all text)";
    if (nStringID == STRID_SELECTALL)
      return L"Select all";
    if (nStringID == STRID_ALIGNCARET)
      return L"Align caret";
    if (nStringID == STRID_OFFSETX)
      return L"X";
    if (nStringID == STRID_OFFSETY)
      return L"Y";
    if (nStringID == STRID_AUTOFOCUS)
      return L"Automatic focus";
    if (nStringID == STRID_FOCUSBACKGROUND)
      return L"Transfer focus";
    if (nStringID == STRID_MOVESCROLLBAR)
      return L"Scrolling with slider";
    if (nStringID == STRID_MOVEWITHSHIFT)
      return L"Horizontal scroll with Shift";
    if (nStringID == STRID_SWITCHTAB)
      return L"Switching between tabs";
    if (nStringID == STRID_WITHSPIN)
      return L"When scroll buttons visible";
    if (nStringID == STRID_INVERT)
      return L"Invert";
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

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hInstanceDLL=pd->hInstanceDLL;
  hInstanceEXE=pd->hInstanceEXE;
  hMainWnd=pd->hMainWnd;
  hMdiClient=pd->hMdiClient;
  hMainIcon=pd->hMainIcon;
  nMDI=pd->nMDI;
  bAkelEdit=pd->bAkelEdit;
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
    //Subclass
    NewMainProcData=NULL;
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

    if (nMDI == WMD_MDI)
    {
      NewFrameProcData=NULL;
      SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewFrameProc, (LPARAM)&NewFrameProcData);
    }
  }
}

void UninitMain()
{
  if (!--nInitMain)
  {
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
  }
}

void InitAutoScroll()
{
  bInitAutoScroll=TRUE;

  hWndAutoScroll=GetCurEdit();
  hDocAutoScroll=GetCurDoc();
  if (nAutoScrollStepTime && nAutoScrollStepWidth)
    SetScrollTimer();
}

void UninitAutoScroll()
{
  bInitAutoScroll=FALSE;

  KillScrollTimer();
  hWndAutoScroll=NULL;
  hDocAutoScroll=NULL;
}

void InitSyncHorz()
{
  bInitSyncHorz=TRUE;
}

void UninitSyncHorz()
{
  bInitSyncHorz=FALSE;
}

void InitSyncVert()
{
  bInitSyncVert=TRUE;
}

void UninitSyncVert()
{
  bInitSyncVert=FALSE;
}

void InitNoScroll()
{
  bInitNoScroll=TRUE;
}

void UninitNoScroll()
{
  bInitNoScroll=FALSE;
}

void InitAlignCaret()
{
  bInitAlignCaret=TRUE;

  ScrollCaretOptionsAll(FALSE);
}

void UninitAlignCaret()
{
  bInitAlignCaret=FALSE;

  ScrollCaretOptionsAll(TRUE);
}

void InitAutoFocus()
{
  bInitAutoFocus=TRUE;

  xmemset(&ht, 0, sizeof(ht));
  ht[0].GetMsgProc=GetMsgProc1;
  ht[1].GetMsgProc=GetMsgProc2;
  ht[2].GetMsgProc=GetMsgProc3;
  ht[3].GetMsgProc=GetMsgProc4;
  ht[4].GetMsgProc=GetMsgProc5;
  ht[5].GetMsgProc=GetMsgProc6;
  ht[6].GetMsgProc=GetMsgProc7;
  ht[7].GetMsgProc=GetMsgProc8;
  ht[8].GetMsgProc=GetMsgProc9;
  ht[9].GetMsgProc=GetMsgProc10;

  ht[0].dwThreadId=GetCurrentThreadId();
  ht[0].hHook=SetWindowsHookExA(WH_GETMESSAGE, ht[0].GetMsgProc, 0, ht[0].dwThreadId);
}

void UninitAutoFocus()
{
  int i;

  bInitAutoFocus=FALSE;

  //Free all threads
  for (i=0; i < HT_MAX; ++i)
  {
    if (ht[i].dwThreadId)
    {
      UnhookWindowsHookEx(ht[i].hHook);
      ht[i].hHook=NULL;
      ht[i].dwThreadId=0;
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
    //Hook current thread
    if (bInitAutoFocus)
    {
      DWORD dwThreadId=GetCurrentThreadId();
      int i;

      for (i=0; i < HT_MAX; ++i)
      {
        if (dwThreadId == ht[i].dwThreadId)
          return TRUE;
      }

      for (i=0; i < HT_MAX; ++i)
      {
        if (!ht[i].dwThreadId)
        {
          ht[i].dwThreadId=dwThreadId;
          ht[i].hHook=SetWindowsHookExA(WH_GETMESSAGE, ht[i].GetMsgProc, 0, ht[i].dwThreadId);
          break;
        }
      }
    }
  }
  else if (fdwReason == DLL_THREAD_DETACH)
  {
    //Unhook current thread
    if (bInitAutoFocus)
    {
      DWORD dwThreadId=GetCurrentThreadId();
      int i;

      for (i=0; i < HT_MAX; ++i)
      {
        if (ht[i].dwThreadId)
        {
          if (ht[i].dwThreadId == dwThreadId)
          {
            UnhookWindowsHookEx(ht[i].hHook);
            ht[i].hHook=NULL;
            ht[i].dwThreadId=0;
            break;
          }
        }
      }
    }
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
    if (bInitAutoScroll)
    {
      UninitAutoScroll();
    }
    if (bInitSyncHorz)
    {
      UninitMain();
      UninitSyncHorz();
    }
    if (bInitSyncVert)
    {
      UninitMain();
      UninitSyncVert();
    }
    if (bInitNoScroll)
    {
      UninitMain();
      UninitNoScroll();
    }
    if (bInitAlignCaret)
    {
      UninitMain();
      UninitAlignCaret();
    }
    if (bInitAutoFocus)
    {
      UninitMain();
      UninitAutoFocus();
    }
  }
  return TRUE;
}
