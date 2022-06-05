#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"


/*
//Include stack functions
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackDelete
#define StackClear
#include "StackFunc.h"

//Include string functions
#define xmemcpy
#include "StrFunc.h"

//Include wide functions
#define GetWindowLongPtrWide
#define SetWindowLongPtrWide
#include "WideFunc.h"
//*/

//Defines
#ifndef EM_SHOWSCROLLBAR
  #define EM_SHOWSCROLLBAR (WM_USER + 96)
#endif

typedef struct _MDIWINDOW {
  struct _MDIWINDOW *next;
  struct _MDIWINDOW *prev;
  HWND hWnd;
  RECT rc;
  DWORD dwStyle;
} MDIWINDOW;

typedef struct _CHILDWINDOW {
  struct _CHILDWINDOW *next;
  struct _CHILDWINDOW *prev;
  HWND hWndChild;
  BOOL bChildVisible;
} CHILDWINDOW;

//Functions prototypes
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
CHILDWINDOW* StackInsertChildWindow(HSTACK *hStack);
MDIWINDOW* StackInsertFrameWindow(HSTACK *hStack);
void StackDeleteFrameWindow(HSTACK *hStack, HWND hWnd);
void StackGetRectFrameWindow(HSTACK *hStack);
void StackHideAllFrameWindow(HSTACK *hStack);
void StackShowAllFrameWindow(HSTACK *hStack);
void StackFreeWindows(HSTACK *hStack);
void HideFrameWindow(HWND hWnd);
void ShowFrameWindow(HWND hWnd, int x, int y, int cx, int cy);
void ShowEditScrollBars(HWND hWnd, BOOL bShow);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
HWND hMainWnd;
HWND hWndEdit;
HWND hStatus;
HWND hMdiClient;
HWND hTab;
BOOL bOldRichEdit;
int nMDI;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
HMENU hMainMenu;
RECT rcMain;
BOOL bStatusVisible;
BOOL bTabVisible;
BOOL bFrameMaximized;
BOOL bHideEditScrollBars=FALSE;
HSTACK hChildWindowStack={0};
HSTACK hMdiWindowStack={0};
WNDPROCDATA *NewMainProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="FullScreen";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (pd->bOnStart) return;
  if (!bInitCommon) InitCommon(pd);

  if (bInitMain)
  {
    UninitMain();
  }
  else
  {
    InitMain();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDN_SIZE_ONFINISH)
  {
    StackHideAllFrameWindow(&hMdiWindowStack);
  }
  else if (uMsg == AKDN_FRAME_ACTIVATE)
  {
    FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;
    MDIWINDOW *lpElement;
    DWORD dwStyle;

    dwStyle=(DWORD)GetWindowLongPtrWide(lpFrame->hWndEditParent, GWL_STYLE);

    if (dwStyle & WS_OVERLAPPEDWINDOW)
    {
      if (lpElement=StackInsertFrameWindow(&hMdiWindowStack))
      {
        lpElement->hWnd=lpFrame->hWndEditParent;
        GetWindowRect(lpFrame->hWndEditParent, &lpElement->rc);
        lpElement->rc.right-=lpElement->rc.left;
        lpElement->rc.bottom-=lpElement->rc.top;
        ScreenToClient(hMdiClient, (POINT *)&lpElement->rc.left);
      }
      HideFrameWindow(lpFrame->hWndEditParent);
    }
  }
  else if (uMsg == AKDN_FRAME_DESTROY)
  {
    FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;

    StackDeleteFrameWindow(&hMdiWindowStack, lpFrame->hWndEditParent);
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

CHILDWINDOW* StackInsertChildWindow(HSTACK *hStack)
{
  CHILDWINDOW *element=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&element, -1, sizeof(CHILDWINDOW));
  return element;
}

MDIWINDOW* StackInsertFrameWindow(HSTACK *hStack)
{
  MDIWINDOW *element=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&element, -1, sizeof(MDIWINDOW));
  return element;
}

void StackDeleteFrameWindow(HSTACK *hStack, HWND hWnd)
{
  MDIWINDOW *lpElement=(MDIWINDOW *)hStack->first;

  while (lpElement)
  {
    if (lpElement->hWnd == hWnd)
    {
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement);
      return;
    }
    lpElement=lpElement->next;
  }
}

void StackGetRectFrameWindow(HSTACK *hStack)
{
  MDIWINDOW *lpElement=(MDIWINDOW *)hStack->first;

  while (lpElement)
  {
    lpElement->dwStyle=(DWORD)GetWindowLongPtrWide(lpElement->hWnd, GWL_STYLE);
    if (lpElement->dwStyle & WS_MINIMIZE)
      ShowWindow(lpElement->hWnd, SW_SHOWNORMAL);
    GetWindowRect(lpElement->hWnd, &lpElement->rc);
    lpElement->rc.right-=lpElement->rc.left;
    lpElement->rc.bottom-=lpElement->rc.top;
    ScreenToClient(hMdiClient, (POINT *)&lpElement->rc.left);

    lpElement=lpElement->next;
  }
}

void StackHideAllFrameWindow(HSTACK *hStack)
{
  MDIWINDOW *lpElement=(MDIWINDOW *)hStack->first;

  while (lpElement)
  {
    HideFrameWindow(lpElement->hWnd);

    lpElement=lpElement->next;
  }
}

void StackShowAllFrameWindow(HSTACK *hStack)
{
  MDIWINDOW *lpElement=(MDIWINDOW *)hStack->first;

  while (lpElement)
  {
    ShowFrameWindow(lpElement->hWnd, lpElement->rc.left, lpElement->rc.top, lpElement->rc.right, lpElement->rc.bottom);
    if (lpElement->dwStyle & WS_MINIMIZE)
      ShowWindow(lpElement->hWnd, SW_MINIMIZE);

    lpElement=lpElement->next;
  }
}

void StackFreeWindows(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void HideFrameWindow(HWND hWnd)
{
  HWND hWndFrameEdit;
  RECT rcClient;
  DWORD dwStyle;

  dwStyle=(DWORD)GetWindowLongPtrWide(hWnd, GWL_STYLE);
  SetWindowLongPtrWide(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
  GetClientRect(hMdiClient, &rcClient);
  MoveWindow(hWnd, rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, TRUE);

  if (bHideEditScrollBars)
  {
    if (!bOldRichEdit)
    {
      hWndFrameEdit=GetDlgItem(hWnd, ID_EDIT);
      ShowEditScrollBars(hWndFrameEdit, FALSE);
    }
  }
}

void ShowFrameWindow(HWND hWnd, int x, int y, int cx, int cy)
{
  HWND hWndFrameEdit;
  DWORD dwStyle;

  dwStyle=(DWORD)GetWindowLongPtrWide(hWnd, GWL_STYLE);
  SetWindowLongPtrWide(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
  MoveWindow(hWnd, x, y, cx, cy, TRUE);

  if (bHideEditScrollBars)
  {
    if (!bOldRichEdit)
    {
      hWndFrameEdit=GetDlgItem(hWnd, ID_EDIT);
      ShowEditScrollBars(hWndFrameEdit, TRUE);
    }
  }
}

void ShowEditScrollBars(HWND hWnd, BOOL bShow)
{
  EDITINFO ei;

  if (!bOldRichEdit)
  {
    if (bShow)
    {
      //Vertical
      SendMessage(hWnd, EM_SHOWSCROLLBAR, SB_VERT, TRUE);

      //Horizontal
      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
      {
        if (!ei.bWordWrap)
          SendMessage(hWnd, EM_SHOWSCROLLBAR, SB_HORZ, TRUE);
      }
    }
    else
    {
      //Vertical
      SendMessage(hWnd, EM_SHOWSCROLLBAR, SB_VERT, FALSE);

      //Horizontal
      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
      {
        if (!ei.bWordWrap)
          SendMessage(hWnd, EM_SHOWSCROLLBAR, SB_HORZ, FALSE);
      }
    }
  }
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hMainWnd=pd->hMainWnd;
  hWndEdit=pd->hWndEdit;
  hStatus=pd->hStatus;
  hMdiClient=pd->hMdiClient;
  hTab=pd->hTab;
  bOldRichEdit=pd->bOldRichEdit;
  nMDI=pd->nMDI;

  //Initialize WideFunc.h header
  WideInitialize();
}

void InitMain()
{
  CHILDWINDOW *lpChildWindow;
  MDIWINDOW *lpMdiWindow;
  DOCK *lpDockWindow;
  DOCK dk;
  HWND hWndFrameActive=NULL;
  HWND hWndChild;
  DWORD dwStyle;
  int nScreenX;
  int nScreenY;

  bInitMain=TRUE;

  //Hide main window children
  hWndChild=GetWindow(hMainWnd, GW_CHILD);

  while (hWndChild)
  {
    if (((nMDI == WMD_SDI || nMDI == WMD_PMDI) && GetWindowLongPtrWide(hWndChild, GWLP_ID) != ID_EDIT) ||
        (nMDI == WMD_MDI && hWndChild != hMdiClient))
    {
      if (lpChildWindow=StackInsertChildWindow(&hChildWindowStack))
      {
        lpChildWindow->hWndChild=hWndChild;
        if (lpChildWindow->bChildVisible=IsWindowVisible(hWndChild))
        {
          dk.hWnd=hWndChild;
          if (lpDockWindow=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_FINDDOCK, (LPARAM)&dk))
            SendMessage(hMainWnd, AKD_DOCK, DK_HIDE, (LPARAM)lpDockWindow);
          else
            ShowWindow(hWndChild, SW_HIDE);
        }
      }
    }
    hWndChild=GetNextWindow(hWndChild, GW_HWNDNEXT);
  }

  //Screen metrics
  nScreenX=GetSystemMetrics(SM_CXSCREEN);
  nScreenY=GetSystemMetrics(SM_CYSCREEN);

  //Change main window caption and size
  dwStyle=(DWORD)GetWindowLongPtrWide(hMainWnd, GWL_STYLE);
  if (!(dwStyle & WS_MAXIMIZE))
  {
    GetWindowRect(hMainWnd, &rcMain);
    rcMain.right-=rcMain.left;
    rcMain.bottom-=rcMain.top;
  }
  SetWindowLongPtrWide(hMainWnd, GWL_STYLE, (dwStyle & ~WS_OVERLAPPEDWINDOW) | WS_POPUP);
  MoveWindow(hMainWnd, 0, 0, nScreenX, nScreenY, TRUE);

  if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
  {
    //Hide main window menu
    hMainMenu=GetMenu(hMainWnd);
    SetMenu(hMainWnd, NULL);

    //Hide edit window scrollbars
    if (bHideEditScrollBars)
    {
      ShowEditScrollBars(hWndEdit, FALSE);
    }
  }
  else
  {
    //Remember MDI frame windows
    hWndChild=GetWindow(hMdiClient, GW_CHILD);

    while (hWndChild)
    {
      if (lpMdiWindow=StackInsertFrameWindow(&hMdiWindowStack))
        lpMdiWindow->hWnd=hWndChild;
      hWndChild=GetNextWindow(hWndChild, GW_HWNDNEXT);
    }

    //Remember MDI frame sizes
    SendMessage(hMdiClient, WM_SETREDRAW, FALSE, 0);
    if (hWndFrameActive=(HWND)SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bFrameMaximized))
    {
      if (bFrameMaximized) SendMessage(hMdiClient, WM_MDIRESTORE, (WPARAM)hWndFrameActive, 0);
      StackGetRectFrameWindow(&hMdiWindowStack);
      SetFocus(hWndFrameActive);
    }
    SendMessage(hMdiClient, WM_SETREDRAW, TRUE, 0);

    //Hide main window menu
    hMainMenu=GetMenu(hMainWnd);
    SetMenu(hMainWnd, NULL);

    //Change MDI client window size
    MoveWindow(hMdiClient, 0, 0, nScreenX, nScreenY, TRUE);

    //Resize MDI frame windows
    SendMessage(hMdiClient, WM_SETREDRAW, FALSE, 0);
    StackHideAllFrameWindow(&hMdiWindowStack);
    SendMessage(hMdiClient, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWndFrameActive, NULL, FALSE);

    //SubClass
    NewMainProcData=NULL;
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);
  }
}

void UninitMain()
{
  CHILDWINDOW *lpChildWindow;
  DOCK *lpDockWindow;
  DOCK dk;
  HWND hWndFrameActive;
  DWORD dwStyle;

  bInitMain=FALSE;

  //Show hidden main window children
  for (lpChildWindow=(CHILDWINDOW *)hChildWindowStack.first; lpChildWindow; lpChildWindow=lpChildWindow->next)
  {
    if (lpChildWindow->bChildVisible)
    {
      dk.hWnd=lpChildWindow->hWndChild;
      if (lpDockWindow=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_FINDDOCK, (LPARAM)&dk))
        SendMessage(hMainWnd, AKD_DOCK, DK_SHOW, (LPARAM)lpDockWindow);
      else
        ShowWindow(lpChildWindow->hWndChild, SW_SHOW);
    }
  }

  //Restore main window caption and size
  dwStyle=(DWORD)GetWindowLongPtrWide(hMainWnd, GWL_STYLE);
  SetWindowLongPtrWide(hMainWnd, GWL_STYLE, (dwStyle & ~WS_POPUP) | WS_OVERLAPPEDWINDOW);
  if (!(dwStyle & WS_MAXIMIZE))
    MoveWindow(hMainWnd, rcMain.left, rcMain.top, rcMain.right, rcMain.bottom, TRUE);

  //Show main window menu
  SetMenu(hMainWnd, hMainMenu);

  if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
  {
    //Show edit window scrollbars
    if (bHideEditScrollBars)
    {
      ShowEditScrollBars(hWndEdit, TRUE);
    }
  }
  else
  {
    //Remove subclass
    if (NewMainProcData)
    {
      SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
      NewMainProcData=NULL;
    }

    //Restore MDI frame windows
    if (hWndFrameActive=(HWND)SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)NULL))
    {
      if (bFrameMaximized)
      {
        SendMessage(hMdiClient, WM_SETREDRAW, FALSE, 0);
        StackShowAllFrameWindow(&hMdiWindowStack);
        SendMessage(hMdiClient, WM_MDIMAXIMIZE, (WPARAM)hWndFrameActive, 0);
        SendMessage(hMdiClient, WM_SETREDRAW, TRUE, 0);
        InvalidateRect(hWndFrameActive, NULL, FALSE);
      }
      else
      {
        StackShowAllFrameWindow(&hMdiWindowStack);
        SendMessage(hMdiClient, WM_MDIACTIVATE, (WPARAM)hWndFrameActive, 0);
      }
    }
  }

  //Focus on main window
  SetFocus(hMainWnd);

  StackFreeWindows(&hChildWindowStack);
  StackFreeWindows(&hMdiWindowStack);
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
    if (bInitMain) UninitMain();
  }
  return TRUE;
}
