#ifndef __AKELPAD_H__
#define __AKELPAD_H__


//// Defines

#define TAB_HEIGHT           24
#define TAB_WIDTH            90

//Timer IDs
#define TIMERID_TABOPEN     1

//Times
#define TIME_TABOPEN        300

#define DOCK_MAINMIN_X       30
#define DOCK_MAINMIN_Y       30
#define DOCK_BORDER_1X        3
#define DOCK_BORDER_2X       (DOCK_BORDER_1X * 2)


//// Functions prototype

void _WinMain();
void WinMainCleanUp();
LRESULT CALLBACK CommonMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK CommonFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CloneDragAndDropMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK NewMdiClientProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewTabProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DockProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DockMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK NewButtonDrawProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewHotkeyInputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DummyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
