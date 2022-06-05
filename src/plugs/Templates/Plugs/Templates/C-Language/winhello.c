/**CARET**/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

int APIENTRY WinMain(	HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPSTR lpCmdLine,
						int nCmdShow)
{
	/**CARET**/
	MessageBox(GetDesktopWindow(),_T("Hello, World!"),_T("Hello, World!"),MB_OK|MB_ICONINFORMATION);
	return 0;
}