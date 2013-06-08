#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include "AkelEdit.h"
#include "AkelDLL.h"


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 8, 4, 0);
  pv->pPluginName="AkelDLL";
}

//Plugin extern function
void __declspec(dllexport) GetSelTextTest(PLUGINDATA *pd)
{
  GETTEXTRANGE gtr;
  CHARRANGE64 cr;

  //Function doesn't support autoload
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  //Get current text selection
  SendMessage(pd->hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
  gtr.cpMin=cr.cpMin;
  gtr.cpMax=cr.cpMax;

  //Get text from selection
  if (pd->dwSupport & PDS_STRANSI)
  {
    if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGEA, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
    {
      MessageBoxA(pd->hMainWnd, (char *)gtr.pText, "Test", MB_OK);
      SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
    }
    else MessageBoxA(pd->hMainWnd, "Text doesn't selected", "Test", MB_OK);
  }
  else
  {
    if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGEW, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
    {
      MessageBoxW(pd->hMainWnd, (wchar_t *)gtr.pText, L"Test", MB_OK);
      SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
    }
    else MessageBoxW(pd->hMainWnd, L"Text doesn't selected", L"Test", MB_OK);
  }
}

//Plugin extern function
void __declspec(dllexport) OpenDialogTest(PLUGINDATA *pd)
{
  //Function doesn't support autoload
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  SendMessage(pd->hMainWnd, WM_COMMAND, IDM_FILE_OPEN, 0);
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
  }
  return TRUE;
}
