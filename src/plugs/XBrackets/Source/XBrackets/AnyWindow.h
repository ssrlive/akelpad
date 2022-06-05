#ifndef _winapi_c_any_window_h_
#define _winapi_c_any_window_h_
//---------------------------------------------------------------------------

// "base.h" must be the first header file
#include "base.h"

BOOL    AnyWindow_BringWindowToTop(HWND hWnd);
BOOL    AnyWindow_CenterWindow(HWND hWnd, HWND hParentWnd, BOOL bRepaint /* = FALSE */);
INT     AnyWindow_GetWindowText(HWND hWnd, TCHAR* lpTextBuf, INT nTextBufSize);
INT     AnyWindow_GetWindowTextLength(HWND hWnd);
BOOL    AnyWindow_MoveWindow(HWND hWnd, INT x, INT y, BOOL bRepaint /* = FALSE */);
BOOL    AnyWindow_MoveResizeWindow(HWND hWnd, INT x, INT y, INT width, INT height, BOOL bRepaint /* = FALSE */);
BOOL    AnyWindow_ResizeWindow(HWND hWnd, INT width, INT height, BOOL bRepaint /* = FALSE */);
BOOL    AnyWindow_ShowWindow(HWND hWnd, INT nCmdShow /* = SW_SHOWNORMAL */);

#if any_ctrl_enable_w_members
INT     AnyWindow_GetWindowTextW(HWND hWnd, WCHAR* lpTextBuf, INT nTextBufSize);
INT     AnyWindow_GetWindowTextLengthW(HWND hWnd);
#endif

//---------------------------------------------------------------------------
#endif
