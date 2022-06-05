#include "AnyWindow.h"

BOOL AnyWindow_BringWindowToTop(HWND hWnd)
{
  return BringWindowToTop(hWnd);
}

BOOL AnyWindow_CenterWindow(HWND hWnd, HWND hParentWnd, BOOL bRepaint)
{
  RECT rectParent;
  RECT rect;
  INT  height, width;
  INT  x, y;

  GetWindowRect(hParentWnd, &rectParent);
  GetWindowRect(hWnd, &rect);
  width = rect.right - rect.left;
  height = rect.bottom - rect.top;
  x = ((rectParent.right - rectParent.left) - width) / 2;
  x += rectParent.left;
  y = ((rectParent.bottom - rectParent.top) - height) / 2;
  y += rectParent.top;
  return MoveWindow(hWnd, x, y, width, height, bRepaint);
}

INT AnyWindow_GetWindowText(HWND hWnd, TCHAR* lpTextBuf, INT nTextBufSize)
{
  return GetWindowText(hWnd, lpTextBuf, nTextBufSize);
}

INT AnyWindow_GetWindowTextLength(HWND hWnd)
{
  return GetWindowTextLength(hWnd);
}

BOOL AnyWindow_MoveWindow(HWND hWnd, INT x, INT y, BOOL bRepaint)
{
  RECT rect;
  INT  height, width;

  GetWindowRect(hWnd, &rect);
  width = rect.right - rect.left;
  height = rect.bottom - rect.top;
  return MoveWindow(hWnd, x, y, width, height, bRepaint);
}

BOOL AnyWindow_MoveResizeWindow(HWND hWnd, INT x, INT y, INT width, INT height, BOOL bRepaint)
{
  return MoveWindow(hWnd, x, y, width, height, bRepaint);
}

BOOL AnyWindow_ResizeWindow(HWND hWnd, INT width, INT height, BOOL bRepaint)
{
  RECT rect;

  GetWindowRect(hWnd, &rect);
  return MoveWindow(hWnd, rect.left, rect.top, width, height, bRepaint);
}

BOOL AnyWindow_ShowWindow(HWND hWnd, INT nCmdShow)
{
  return ShowWindow(hWnd, nCmdShow);
}


///////////////////////////////////////////////////////////////////////////
// Additional Unicode functions

#if any_ctrl_enable_w_members

  INT AnyWindow_GetWindowTextW(HWND hWnd, WCHAR* lpTextBuf, INT nTextBufSize)
  {
    return GetWindowTextW(hWnd, lpTextBuf, nTextBufSize);
  }

  INT AnyWindow_GetWindowTextLengthW(HWND hWnd)
  {
    return GetWindowTextLengthW(hWnd);
  }

#endif

