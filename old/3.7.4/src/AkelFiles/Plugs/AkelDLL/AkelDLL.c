#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include "AkelDLL.h"


//Identification
EXTERN_C void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(3, 7, 4, 0);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 4, 0, 0);
  pv->pPluginName="AkelDLL";
}

//Plugin extern function
EXTERN_C void __declspec(dllexport) GetSelTextTest(PLUGINDATA *pd)
{
  GETTEXTRANGE gtr;
  CHARRANGE cr;
  int nLen;

  //Function doesn't support autoload
  if (pd->lpbAutoLoad)
  {
    *pd->lpbAutoLoad=FALSE;
    return;
  }

  //If function load on start (autoload), then exit
  if (pd->bOnStart) return;

  //Get current text selection
  SendMessage(pd->hWndEdit, EM_EXGETSEL, 0, (LPARAM)&cr);
  gtr.cpMin=cr.cpMin;
  gtr.cpMax=cr.cpMax;

  //Get text from selection
  if (nLen=SendMessage(pd->hMainWnd, AKD_GETTEXTRANGE, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
  {
    //Show result
    if (pd->bOldWindows)
      MessageBoxA(pd->hMainWnd, (char *)gtr.pText, "Test", MB_OK);
    else
      MessageBoxW(pd->hMainWnd, (wchar_t *)gtr.pText, L"Test", MB_OK);

    //Free text buffer allocated with AKD_GETTEXTRANGE
    SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
  }
  else
  {
    //Show result
    if (pd->bOldWindows)
      MessageBoxA(pd->hMainWnd, "Text doesn't selected", "Test", MB_OK);
    else
      MessageBoxW(pd->hMainWnd, L"Text doesn't selected", L"Test", MB_OK);
  }
}

//Plugin extern function
EXTERN_C void __declspec(dllexport) OpenDialogTest(PLUGINDATA *pd)
{
  //Function doesn't support autoload
  if (pd->lpbAutoLoad)
  {
    *pd->lpbAutoLoad=FALSE;
    return;
  }

  //If function load on start (autoload), then exit
  if (pd->bOnStart) return;

  SendMessage(pd->hMainWnd, WM_COMMAND, IDM_FILE_OPEN, 0);
}

//Entry point
EXTERN_C BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
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
  }
  return TRUE;
}
