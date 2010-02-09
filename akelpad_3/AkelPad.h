#ifndef __AKELPAD_H__
#define __AKELPAD_H__


//// Defines

#define TAB_HEIGHT           24
#define TAB_WIDTH            90

#define TAB_VIEW_NONE         0
#define TAB_VIEW_TOP          1
#define TAB_VIEW_BOTTOM       2

#define TAB_TYPE_STANDARD     0
#define TAB_TYPE_BUTTONS      1
#define TAB_TYPE_FLATBUTTONS  2

#define TAB_SWITCH_NEXTPREV   0
#define TAB_SWITCH_RIGHTLEFT  1

#define STATUS_POSITION       0
#define STATUS_MODIFY         1
#define STATUS_INSERT         2
#define STATUS_NEWLINE        3
#define STATUS_CODEPAGE       4
#define STATUS_PARTS          5

#define DOCK_MAINMIN_X       30
#define DOCK_MAINMIN_Y       30
#define DOCK_BORDER_1X        3
#define DOCK_BORDER_2X       (DOCK_BORDER_1X * 2)

#define DOCK_SIZING           1
#define DOCK_DRAGDROP         2


//// Functions prototype

LRESULT CALLBACK CommonMainProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonMainProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessagesA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessagesW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonFrameProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonFrameProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonEditProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonEditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMdiClientProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMdiClientProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewTabProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewTabProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DockProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DockProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DockMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewCloseButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewHotkeyInputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DummyProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DummyProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
