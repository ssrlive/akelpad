#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include "Resources\resource.h"


//Defines
#define APP_TEST_CLASSW    L"LibraryTest"
#define APP_TEST_TITLEW    L"LibraryTest"

//Functions prototypes
LRESULT CALLBACK TestProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Global variables
HINSTANCE hInstance;
HMODULE hLibAkelPad=NULL;
HWND hTestWnd=NULL;
HWND hWndAkelPad=NULL;
WNDPROC lpOldMainProc;

//GetProcAddress
HWND (*AkelPadCreatePtr)(unsigned char *pCmdLine, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu);
DWORD (*AkelPadTranslatePtr)(MSG *lpMsg);
BOOL (*AkelPadClosePtr)(void);


//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

void _WinMain()
{
  MSG msg;

  InitCommonControls();
  hInstance=GetModuleHandle(NULL);

  //Load AkelPad.dll
  if (!(hLibAkelPad=LoadLibraryA("AkelPad.dll")))
  {
    MessageBoxW(NULL, L"LoadLibrary", APP_TEST_TITLEW, MB_OK|MB_ICONERROR);
    goto Quit;
  }

  //Get extern functions
  AkelPadCreatePtr=(HWND (*)(unsigned char *, DWORD, int, int, int, int, HWND, HMENU))GetProcAddress(hLibAkelPad, "Create");
  AkelPadTranslatePtr=(DWORD (*)(MSG *))GetProcAddress(hLibAkelPad, "Translate");
  AkelPadClosePtr=(BOOL (*)(void))GetProcAddress(hLibAkelPad, "Close");

  //Test window create
  {
    WNDCLASSW wndclass={0};

    wndclass.style        =CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wndclass.lpfnWndProc  =TestProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstance;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=APP_TEST_CLASSW;
    if (!RegisterClassW(&wndclass)) goto Quit;

    hTestWnd=CreateWindowW(APP_TEST_CLASSW, APP_TEST_TITLEW, WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    if (!hTestWnd)
    {
      MessageBoxW(NULL, L"CreateWindow", APP_TEST_TITLEW, MB_OK|MB_ICONERROR);
      goto Quit;
    }
  }

  //AkelPad window create
  {
    RECT rcRect;

    GetClientRect(hTestWnd, &rcRect);
    hWndAkelPad=AkelPadCreatePtr((BYTE *)L"", WS_CHILD, 0, 0, rcRect.right, rcRect.bottom, hTestWnd, (HMENU)NULL);
    if (!hWndAkelPad)
    {
      MessageBoxW(NULL, L"CreateWindow", APP_TEST_TITLEW, MB_OK|MB_ICONERROR);
      goto Quit;
    }

    //Subclass AkelPad window to catch escape key
    #ifdef _WIN64
      lpOldMainProc=(WNDPROC)GetWindowLongPtrW(hWndAkelPad, GWLP_WNDPROC);
      SetWindowLongPtrW(hWndAkelPad, GWLP_WNDPROC, (UINT_PTR)MainProc);
    #else
      lpOldMainProc=(WNDPROC)GetWindowLongW(hWndAkelPad, GWL_WNDPROC);
      SetWindowLongW(hWndAkelPad, GWL_WNDPROC, (UINT_PTR)MainProc);
    #endif
  }

  //Message loop
  while (GetMessageW(&msg, NULL, 0, 0) > 0)
  {
    //Translate AkelPad messages
    if (!AkelPadTranslatePtr(&msg))
    {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }
  }

  Quit:
  if (hLibAkelPad) FreeLibrary(hLibAkelPad);
  ExitProcess(0);
}

LRESULT CALLBACK TestProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CREATE)
  {
  }
  else if (uMsg == WM_SETFOCUS)
  {
    if (hWndAkelPad)
    {
      SetFocus(hWndAkelPad);
      return 0;
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (hWndAkelPad)
      MoveWindow(hWndAkelPad, 0, 0, LOWORD(lParam), HIWORD(lParam), FALSE);
  }
  else if (uMsg == WM_CLOSE)
  {
    //Quit, if user doesn't abort quiting
    if (AkelPadClosePtr())
      DestroyWindow(hTestWnd);
    return 0;
  }
  else if (uMsg == WM_DESTROY)
  {
    PostQuitMessage(0);
  }
  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == 4109 /*IDM_FILE_EXIT*/)
    {
      PostMessage(hTestWnd, WM_CLOSE, 0, 0);
    }
  }
  return CallWindowProcW(lpOldMainProc, hWnd, uMsg, wParam, lParam);
}
