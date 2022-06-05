/*****************************************************************
 *                 Resize functions header v1.5                  *
 *                                                               *
 * 2016 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Functions:                                                     *
 * GetWindowPos, GetWindowSize, GetClientPos, GetClientSize      *
 * ResizeDialogUpdateOffsets, ResizeDialogMessages               *
 *****************************************************************/

#ifndef _RESIZEFUNC_H_
#define _RESIZEFUNC_H_

#define ALLRESIZEFUNC

//ResizeDialogMessages flags
#define RDM_PAINTSIZEGRIP 0x2 //Draw resize grid.
#define RDM_ALLCHILDREN   0x4 //Smooth repaint also for all children that not included in RESIZEDIALOG array.

//RESIZEDIALOG type
#define RDS_SIZE  0x1 //Resize control. Can be combined with RDS_X or RDS_Y.
#define RDS_MOVE  0x2 //Move control. Can be combined with RDS_X or RDS_Y.
#define RDS_X     0x4 //X value. Can be combined with RDS_SIZE or RDS_MOVE.
#define RDS_Y     0x8 //Y value. Can be combined with RDS_SIZE or RDS_MOVE.

#ifndef IDC_COMBOBOXEDIT
  #define IDC_COMBOBOXEDIT 1001
#endif
#ifndef BS_TYPEMASK
  #define BS_TYPEMASK 0x0000000FL
#endif

typedef struct {
  HWND *lpWnd;   //Control window.
  DWORD dwType;  //See RDS_* defines.
  int nOffset;   //Control offset, set it to zero.
} RESIZEDIALOG;

typedef struct _RESIZEDIALOGWND {
  struct _RESIZEDIALOGWND *next;
  struct _RESIZEDIALOGWND *prev;
  HWND hWnd;
  RECT rcBeforeWindow;
  RECT rcBeforeClient;
} RESIZEDIALOGWND;

typedef struct {
  RESIZEDIALOGWND *first;
  RESIZEDIALOGWND *last;
} STACKRESIZEDIALOGWND;

BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL GetClientPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL GetClientSize(HWND hWnd, HWND hWndOwner, RECT *rc);
void ResizeDialogUpdateOffsets(RESIZEDIALOG *rds, RECT *rcCurrent, HWND hDlg);
BOOL ResizeDialogMessages(RESIZEDIALOG *rds, const RECT *rcMinMax, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


/********************************************************************
 *
 *  GetWindowPos
 *
 *Get window position.
 *
 *[in] HWND hWnd              Window handle.
 *[in] HWND hWndOwner         Window owner handle, if NULL screen dimensions used.
 *[in] RECT *rc               rc->left and rc->top - window coordinates relative to the upper-left corner of the hWndOwner.
 *                            rc->right and rc->bottom - window coordinates relative to the lower-right corner of the hWndOwner.
 *
 *Returns: TRUE   success.
 *         FALSE  failed.
 ********************************************************************/
#if defined GetWindowPos || defined ALLRESIZEFUNC
#define GetWindowPos_INCLUDED
#undef GetWindowPos
__inline BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetWindowRect(hWnd, rc))
  {
    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->right))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}
#endif

/********************************************************************
 *
 *  GetWindowSize
 *
 *Get window size.
 *
 *[in] HWND hWnd              Window handle.
 *[in] HWND hWndOwner         Window owner handle, if NULL screen dimensions used.
 *[in] RECT *rc               rc->left and rc->top - window coordinates relative to the upper-left corner of the hWndOwner.
 *                            rc->right - width of the window.
 *                            rc->bottom - height of the window.
 *
 *Returns: TRUE   success.
 *         FALSE  failed.
 ********************************************************************/
#if defined GetWindowSize || defined ALLRESIZEFUNC
#define GetWindowSize_INCLUDED
#undef GetWindowSize
__inline BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc)
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
#endif

/********************************************************************
 *
 *  GetClientPos
 *
 *Get client position.
 *
 *[in] HWND hWnd              Window handle.
 *[in] HWND hWndOwner         Window owner handle, if NULL screen dimensions used.
 *[in] RECT *rc               rc->left and rc->top - client coordinates relative to the upper-left corner of the hWndOwner.
 *                            rc->right and rc->bottom - client coordinates relative to the lower-right corner of the hWndOwner.
 *
 *Returns: TRUE   success.
 *         FALSE  failed.
 ********************************************************************/
#if defined GetClientPos || defined ALLRESIZEFUNC
#define GetClientPos_INCLUDED
#undef GetClientPos
__inline BOOL GetClientPos(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetClientRect(hWnd, rc))
  {
    if (!ClientToScreen(hWnd, (POINT *)&rc->left))
      return FALSE;
    if (!ClientToScreen(hWnd, (POINT *)&rc->right))
      return FALSE;

    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->right))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}
#endif

/********************************************************************
 *
 *  GetClientSize
 *
 *Get client position.
 *
 *[in] HWND hWnd              Window handle.
 *[in] HWND hWndOwner         Window owner handle, if NULL screen dimensions used.
 *[in] RECT *rc               rc->left and rc->top - client coordinates relative to the upper-left corner of the hWndOwner.
 *                            rc->right - width of the window.
 *                            rc->bottom - height of the window.
 *
 *Returns: TRUE   success.
 *         FALSE  failed.
 ********************************************************************/
#if defined GetClientSize || defined ALLRESIZEFUNC
#define GetClientSize_INCLUDED
#undef GetClientSize
__inline BOOL GetClientSize(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetClientRect(hWnd, rc))
  {
    rc->right-=rc->left;
    rc->bottom-=rc->top;
    if (!ClientToScreen(hWnd, (POINT *)&rc->left))
      return FALSE;

    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}
#endif

/********************************************************************
 *
 *  ResizeDialogUpdateOffsets
 *
 *Update offsets (RESIZEDIALOG.nOffset). Required to call if control size was changed outside ResizeDialogMessages.
 *
 *[in,out] RESIZEDIALOG *rds  Pointer to a first RESIZEDIALOG element in array. Last element specified as NULL in RESIZEDIALOG.lpWnd.
 *[in] RECT *rcCurrent        Pointer to a current dialog rectangle.
 *[in] HWND hDlg              Dialog handle.
 *
 *Returns: void.
 ********************************************************************/
#if defined ResizeDialogUpdateOffsets || defined ALLRESIZEFUNC
#define ResizeDialogUpdateOffsets_INCLUDED
#undef ResizeDialogUpdateOffsets
__inline void ResizeDialogUpdateOffsets(RESIZEDIALOG *rds, RECT *rcCurrent, HWND hDlg)
{
  RESIZEDIALOG *rdsControl;
  RECT rcControl;

  for (rdsControl=rds; rdsControl->lpWnd; ++rdsControl)
  {
    if (!*rdsControl->lpWnd)
      continue;

    GetWindowSize(*rdsControl->lpWnd, hDlg, &rcControl);
    if (rdsControl->dwType & RDS_SIZE)
    {
      if (rdsControl->dwType & RDS_X)
        rdsControl->nOffset=rcCurrent->right - (rcControl.left + rcControl.right);
      else if (rdsControl->dwType & RDS_Y)
        rdsControl->nOffset=rcCurrent->bottom - (rcControl.top + rcControl.bottom);
    }
    else if (rdsControl->dwType & RDS_MOVE)
    {
      if (rdsControl->dwType & RDS_X)
        rdsControl->nOffset=rcCurrent->right - rcControl.left;
      else if (rdsControl->dwType & RDS_Y)
        rdsControl->nOffset=rcCurrent->bottom - rcControl.top;
    }
  }
}
#endif

/********************************************************************
 *
 *  ResizeDialogMessages
 *
 *Resize dialog that has style WS_THICKFRAME.
 *
 *[in,out] RESIZEDIALOG *rds  Pointer to a first RESIZEDIALOG element in array. Last element specified as NULL in RESIZEDIALOG.lpWnd.
 *[in] const RECT *rcMinMax   Pointer to a min/max sizes. Each member is valid if not equal to zero. Can be NULL.
 *                             RECT->rcMinMax.left, RECT->rcMinMax.top specifies minimum dialog size.
 *                             RECT->rcMinMax.right, RECT->rcMinMax.bottom specifies maximum dialog size.
 *[in] RECT *rcCurrent        Pointer to a current dialog rectangle. Set all members of RECT to zero at first call.
 *[in] DWORD dwFlags          See RDM_* defines.
 *[in] HWND hDlg              Dialog handle.
 *[in] UINT uMsg              Dialog message.
 *[in] WPARAM wParam          Dialog message WPARAM.
 *[in] LPARAM lParam          Dialog message LPARAM.
 *
 *Returns: TRUE   dialog size or position is changed.
 *         FALSE  dialog size and position aren't changed.
 ********************************************************************/
#if defined ResizeDialogMessages || defined ALLRESIZEFUNC
#define ResizeDialogMessages_INCLUDED
#undef ResizeDialogMessages
__inline BOOL ResizeDialogMessages(RESIZEDIALOG *rds, const RECT *rcMinMax, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_CREATE:
    case WM_INITDIALOG:
    {
      RECT rcTemplate;
      DWORD dwSwpFlags=SWP_NOMOVE;

      rcTemplate=*rcCurrent;
      GetWindowSize(hDlg, NULL, rcCurrent);
      ResizeDialogUpdateOffsets(rds, rcCurrent, hDlg);

      if (rcTemplate.right && rcTemplate.bottom)
      {
        if (GetWindowLongPtrWide(hDlg, GWL_STYLE) & DS_CENTER)
        {
          rcTemplate.left=rcCurrent->left + (rcCurrent->right - rcTemplate.right) / 2;
          rcTemplate.top=rcCurrent->top + (rcCurrent->bottom - rcTemplate.bottom) / 2;
          dwSwpFlags&=~SWP_NOMOVE;
        }
        SetWindowPos(hDlg, 0, rcTemplate.left, rcTemplate.top, rcTemplate.right, rcTemplate.bottom, dwSwpFlags|SWP_NOZORDER|SWP_NOACTIVATE);
      }
      break;
    }
    case WM_GETMINMAXINFO:
    {
      if (rcMinMax)
      {
        MINMAXINFO *mmi=(MINMAXINFO *)lParam;

        if (rcMinMax->left)
          mmi->ptMinTrackSize.x=rcMinMax->left;
        if (rcMinMax->top)
          mmi->ptMinTrackSize.y=rcMinMax->top;
        if (rcMinMax->right)
          mmi->ptMaxTrackSize.x=rcMinMax->right;
        if (rcMinMax->bottom)
          mmi->ptMaxTrackSize.y=rcMinMax->bottom;
      }
      break;
    }
    case WM_WINDOWPOSCHANGING:
    {
      WINDOWPOS *lpWindowPos=(WINDOWPOS *)lParam;
      RECT rcTemplate;

      if (lpWindowPos && !(lpWindowPos->flags & SWP_NOCOPYBITS))
      {
        if (GetWindowLongPtrWide(hDlg, GWL_STYLE) & WS_CHILD)
          GetWindowSize(hDlg, GetParent(hDlg), &rcTemplate);
        else
          GetWindowSize(hDlg, NULL, &rcTemplate);

        if ((!(lpWindowPos->flags & SWP_NOMOVE) && (lpWindowPos->x != rcTemplate.left || lpWindowPos->y != rcTemplate.top)) &&
            (!(lpWindowPos->flags & SWP_NOSIZE) && (lpWindowPos->cx != rcTemplate.right || lpWindowPos->cy != rcTemplate.bottom)))
        {
          //Size and position changed don't copy bits to avoid blinking.
          lpWindowPos->flags|=SWP_NOCOPYBITS;
        }
      }
      break;
    }
    case WM_MOVE:
    {
      if (!(GetWindowLongPtrWide(hDlg, GWL_STYLE) & DS_CENTER))
      {
        RECT rcTemplate;

        GetWindowSize(hDlg, NULL, &rcTemplate);
        rcCurrent->left=rcTemplate.left;
        rcCurrent->top=rcTemplate.top;
        return TRUE;
      }
      break;
    }
    case WM_SIZE:
    {
      if (lParam)
      {
        STACKRESIZEDIALOGWND hDRWStack={0};
        RESIZEDIALOGWND *lpDRW;
        RESIZEDIALOGWND *lpIntersectDRW;
        RESIZEDIALOG *rdsControl=rds;
        RESIZEDIALOG *rdsCount;
        wchar_t wszClassName[MAX_PATH];
        RECT rcControl;
        RECT rcNew;
        RECT rcAfterWindow;
        RECT rcIntersect;
        DWORD dwSwpFlags;
        HWND hWndControl=NULL;
        HWND hWndComboboxEdit;
        HRGN hRgnChanged=NULL;
        HRGN hRgnControl=NULL;
        HRGN hRgnToErase=NULL;
        HRGN hRgnToDrawBefore=NULL;
        HRGN hRgnToDrawAfter=NULL;
        HRGN hRgnAllChild=NULL;
        int nChanged=0;
        BOOL bClipChildren=FALSE;

        hRgnChanged=CreateRectRgn(0, 0, 0, 0);
        hRgnControl=CreateRectRgn(0, 0, 0, 0);
        hRgnToErase=CreateRectRgn(0, 0, 0, 0);
        hRgnToDrawBefore=CreateRectRgn(0, 0, 0, 0);
        hRgnToDrawAfter=CreateRectRgn(0, 0, 0, 0);
        hRgnAllChild=CreateRectRgn(0, 0, 0, 0);

        GetWindowSize(hDlg, NULL, rcCurrent);
        if (GetWindowLongPtrWide(hDlg, GWL_STYLE) & WS_CLIPCHILDREN)
          bClipChildren=TRUE;

        //First cycle throw the rds array, next throw the all other children
        for (;;)
        {
          if (rdsControl->lpWnd)
            hWndControl=*rdsControl->lpWnd;

          if (hWndControl)
          {
            GetClassNameWide(hWndControl, wszClassName, MAX_PATH);
            GetWindowSize(hWndControl, hDlg, &rcControl);

            if (!bClipChildren)
            {
              if (!xstrcmpiW(wszClassName, L"BUTTON") && (GetWindowLongPtrWide(hWndControl, GWL_STYLE) & BS_TYPEMASK) == BS_GROUPBOX)
              {
                //Exclude GroupBox from smooth draw - it will be erased later.
              }
              else
              {
                //Remember control sizes
                for (lpDRW=hDRWStack.first; lpDRW; lpDRW=lpDRW->next)
                {
                  if (lpDRW->hWnd == hWndControl)
                    break;
                }
                if (!lpDRW)
                {
                  if (!StackInsertBefore((stack **)&hDRWStack.first, (stack **)&hDRWStack.last, NULL, (stack **)&lpDRW, sizeof(RESIZEDIALOGWND)))
                  {
                    lpDRW->hWnd=hWndControl;
                    GetWindowPos(lpDRW->hWnd, hDlg, &lpDRW->rcBeforeWindow);

                    if (!xstrcmpiW(wszClassName, L"COMBOBOX"))
                    {
                      if (hWndComboboxEdit=GetDlgItem(hWndControl, IDC_COMBOBOXEDIT))
                        GetClientPos(hWndComboboxEdit, hDlg, &lpDRW->rcBeforeClient);
                    }
                    else
                    {
                      GetClientPos(hWndControl, hDlg, &lpDRW->rcBeforeClient);
                      if (!xstrcmpiW(wszClassName, L"EDIT"))
                        lpDRW->rcBeforeClient.right=max(lpDRW->rcBeforeClient.right - GetSystemMetrics(SM_CXVSCROLL), lpDRW->rcBeforeClient.left);
                      else if (!xstrcmpiW(wszClassName, L"STATIC"))
                      {
                        if ((GetWindowLongPtrWide(hWndControl, GWL_STYLE) & SS_SUNKEN) || (GetWindowLongPtrWide(hWndControl, GWL_EXSTYLE) & WS_EX_CLIENTEDGE))
                          lpDRW->rcBeforeClient.right=max(lpDRW->rcBeforeClient.right - GetSystemMetrics(SM_CXEDGE) - 1, lpDRW->rcBeforeClient.left);
                      }
                    }
                  }
                }
              }
            }
            if (rdsControl->lpWnd)
            {
              rcNew.left=((rdsControl->dwType & RDS_MOVE) && (rdsControl->dwType & RDS_X))?(rcCurrent->right - rdsControl->nOffset):rcControl.left;
              rcNew.top=((rdsControl->dwType & RDS_MOVE) && (rdsControl->dwType & RDS_Y))?(rcCurrent->bottom - rdsControl->nOffset):rcControl.top;
              rcNew.right=((rdsControl->dwType & RDS_SIZE) && (rdsControl->dwType & RDS_X))?(rcCurrent->right - rcControl.left - rdsControl->nOffset):rcControl.right;
              rcNew.bottom=((rdsControl->dwType & RDS_SIZE) && (rdsControl->dwType & RDS_Y))?(rcCurrent->bottom - rcControl.top - rdsControl->nOffset):rcControl.bottom;

              if (xmemcmp(&rcNew, &rcControl, sizeof(RECT)))
              {
                dwSwpFlags=0;
                if (rdsControl->dwType & RDS_SIZE)
                  dwSwpFlags|=SWP_NOMOVE;
                else if (rdsControl->dwType & RDS_MOVE)
                  dwSwpFlags|=SWP_NOSIZE;
                else
                  continue;

                if (SetWindowPos(hWndControl, 0, rcNew.left, rcNew.top, rcNew.right, rcNew.bottom, dwSwpFlags|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOCOPYBITS|SWP_DEFERERASE))
                  ++nChanged;
              }
            }
          }

          if (rdsControl->lpWnd)
          {
            ++rdsControl;
            hWndControl=NULL;
          }
          if (!rdsControl->lpWnd)
          {
            //Find child that not exist in rds array
            if (dwFlags & RDM_ALLCHILDREN)
            {
              for (;;)
              {
                if (!hWndControl)
                  hWndControl=GetWindow(hDlg, GW_CHILD);
                else
                  hWndControl=GetWindow(hWndControl, GW_HWNDNEXT);
                if (!hWndControl) break;

                for (rdsCount=rds; rdsCount->lpWnd; ++rdsCount)
                {
                  if (*rdsCount->lpWnd == hWndControl)
                    break;
                }
                if (!rdsCount->lpWnd) break;
              }
            }
            if (!hWndControl) break;
          }
        }
        if (nChanged)
        {
          if (bClipChildren)
          {
            RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN);
          }
          else
          {
            //Collect changed rectangles
            for (lpDRW=hDRWStack.first; lpDRW; lpDRW=lpDRW->next)
            {
              //Check is this window was intersected before resize
              for (lpIntersectDRW=hDRWStack.first; lpIntersectDRW; lpIntersectDRW=lpIntersectDRW->next)
              {
                if (lpIntersectDRW != lpDRW)
                  if (IntersectRect(&rcIntersect, &lpIntersectDRW->rcBeforeWindow, &lpDRW->rcBeforeWindow))
                    break;
              }
              GetWindowPos(lpDRW->hWnd, hDlg, &rcAfterWindow);
              SetRectRgn(hRgnControl, rcAfterWindow.left, rcAfterWindow.top, rcAfterWindow.right, rcAfterWindow.bottom);
              CombineRgn(hRgnAllChild, hRgnAllChild, hRgnControl, RGN_OR);

              if (lpIntersectDRW || lpDRW->rcBeforeWindow.left != rcAfterWindow.left || lpDRW->rcBeforeWindow.top != rcAfterWindow.top)
              {
                //All control
                CombineRgn(hRgnChanged, hRgnControl, NULL, RGN_COPY);
              }
              else
              {
                //Only changed parts
                SetRectRgn(hRgnChanged, lpDRW->rcBeforeClient.left, lpDRW->rcBeforeClient.top, lpDRW->rcBeforeClient.right, lpDRW->rcBeforeClient.bottom);
                CombineRgn(hRgnChanged, hRgnControl, hRgnChanged, RGN_DIFF);
              }
              CombineRgn(hRgnToDrawAfter, hRgnToDrawAfter, hRgnChanged, RGN_OR);
            }
            GetUpdateRgn(hDlg, hRgnToDrawBefore, FALSE);

            //Erase parent window background without children
            GetClientRect(hDlg, &rcControl);
            SetRectRgn(hRgnControl, rcControl.left, rcControl.top, rcControl.right, rcControl.bottom);
            CombineRgn(hRgnToErase, hRgnControl, hRgnAllChild, RGN_DIFF);
            InvalidateRgn(hDlg, hRgnToErase, TRUE);
            UpdateWindow(hDlg);

            //{
            //  //Region debug
            //  HBRUSH hBrush;
            //  HDC hDC;
            //
            //  if (hDC=GetDC(hDlg))
            //  {
            //    if (hBrush=CreateSolidBrush(RGB(0xFF, 0x00, 0x00)))
            //    {
            //      FillRgn(hDC, hRgnToErase, hBrush);
            //      DeleteObject(hBrush);
            //    }
            //    ReleaseDC(hDlg, hDC);
            //  }
            //}

            //Remove erased region and draw children
            CombineRgn(hRgnToDrawBefore, hRgnToDrawBefore, hRgnToErase, RGN_DIFF);
            CombineRgn(hRgnToDrawAfter, hRgnToDrawAfter, hRgnToErase, RGN_DIFF);
            CombineRgn(hRgnToDrawAfter, hRgnToDrawAfter, hRgnToDrawBefore, RGN_OR);
            InvalidateRgn(hDlg, hRgnToDrawAfter, FALSE);
            UpdateWindow(hDlg);
          }
        }
        DeleteObject(hRgnChanged);
        DeleteObject(hRgnControl);
        DeleteObject(hRgnToErase);
        DeleteObject(hRgnToDrawBefore);
        DeleteObject(hRgnToDrawAfter);
        DeleteObject(hRgnAllChild);
        StackClear((stack **)&hDRWStack.first, (stack **)&hDRWStack.last);
        return TRUE;
      }
      break;
    }
    case WM_PAINT:
    {
      if (dwFlags & RDM_PAINTSIZEGRIP)
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
      break;
    }
  }
  return FALSE;
}
#endif


/********************************************************************
 *                                                                  *
 *                           Example                                *
 *                                                                  *
 ********************************************************************

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "ResizeFunc.h"

//Include stack functions
#define StackInsertBefore
#define StackClear
#include "StackFunc.h"

//Include string functions
#define xmemcmp
#define xstrcmpiW
#include "StrFunc.h"

//Include wide functions
#define GetWindowLongPtrWide
#define GetClassNameWide
#include "WideFunc.h"

//Include resize functions
#define ALLRESIZEFUNC
#include "ResizeFunc.h"

...

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndList;
  static HWND hWndScriptsFilter;
  static HWND hWndCancel;
  static RESIZEDIALOG rds[]={{&hWndList,           RDS_SIZE|RDS_X, 0},
                             {&hWndList,           RDS_SIZE|RDS_Y, 0},
                             {&hWndScriptsFilter,  RDS_SIZE|RDS_X, 0},
                             {&hWndScriptsFilter,  RDS_MOVE|RDS_Y, 0},
                             {&hWndCancel,         RDS_MOVE|RDS_X, 0},
                             {&hWndCancel,         RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    hWndList=GetDlgItem(hDlg, IDC_LIST);
    hWndScriptsFilter=GetDlgItem(hDlg, IDC_SCRIPTS_FILTER);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
  }

  ResizeDialogMessages(&rds[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

*/

#endif //_RESIZEFUNC_H_
