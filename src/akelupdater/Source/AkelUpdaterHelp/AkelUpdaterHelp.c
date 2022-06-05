#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include "StrFunc.h"
#include "AkelDLL.h"

/*
//Include string functions
#define xatoiW
#define xstrcpynW
#define xstrlenW
#include "..\StrFunc.h"
//*/

//Defines
#define PE_NONE        0
#define PE_NOTINLIST   1
#define PE_NOTPLUGIN   2
#define PE_CANTLOAD    3

//AkelUpdaterHelp action
#define AUH_GETDLLNAME   1

typedef struct {
  wchar_t wszName[MAX_PATH];
  DWORD dwError;
} DLLINFO;

//Functions prototypes
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg);

//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

//Entry point
int CALLBACK _tWinMain(HINSTANCE hApp, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  PLUGINVERSION pv;
  DLLINFO di;
  HMODULE hInstance;
  HWND hWndSend=NULL;
  wchar_t *wpArguments=GetCommandLineW();
  wchar_t wszWindow[MAX_PATH];
  wchar_t wszFile[MAX_PATH];
  int nAction;
  //GetProcAddress
  void (*DllAkelPadID)(PLUGINVERSION *pv);

  di.dwError=PE_CANTLOAD;

  //Skip executable
  GetCommandLineArg(wpArguments, NULL, 0, &wpArguments);

  //First argument is action
  nAction=(int)xatoiW(wpArguments, &wpArguments);

  //Second argument is window handle
  if (GetCommandLineArg(wpArguments, wszWindow, MAX_PATH, &wpArguments))
  {
    hWndSend=(HWND)xatoiW(wszWindow, NULL);

    //Third argument is dll file to check
    if (GetCommandLineArg(wpArguments, wszFile, MAX_PATH, &wpArguments))
    {
      if (hInstance=LoadLibraryW(wszFile))
      {
        if (nAction == AUH_GETDLLNAME)
        {
          if (DllAkelPadID=(void (*)(PLUGINVERSION *))GetProcAddress(hInstance, "DllAkelPadID"))
          {
            DllAkelPadID(&pv);

            MultiByteToWideChar(CP_ACP, 0, pv.pPluginName, -1, di.wszName, MAX_PATH);
            di.dwError=PE_NONE;
          }
          else di.dwError=PE_NOTPLUGIN;
        }
        FreeLibrary(hInstance);
      }
    }
  }

  //Send result to caller
  if (hWndSend)
  {
    COPYDATASTRUCT cds;

    cds.dwData=0;
    cds.cbData=sizeof(di);
    cds.lpData=(PVOID)&di;
    SendMessage(hWndSend, WM_COPYDATA, (WPARAM)hWndSend, (LPARAM)&cds);
  }

  ExitProcess(0);
}

int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg)
{
  const wchar_t *wpCount=wpCmdLine;
  wchar_t *wpArgSet=wszArg;
  wchar_t *wpArgSetMax=wszArg + nArgMax - 1;
  wchar_t wchInitStopChar;
  wchar_t wchCurStopChar;

  while (*wpCount == L' ') ++wpCount;

  if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
    wchInitStopChar=*wpCount++;
  else
    wchInitStopChar=L' ';
  wchCurStopChar=wchInitStopChar;

  for (; *wpCount; ++wpCount)
  {
    if (wchCurStopChar == L' ')
    {
      if (*wpCount == L' ')
        break;
      if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
        wchCurStopChar=*wpCount;
    }
    else if (wchCurStopChar == *wpCount)
    {
      if (wchCurStopChar == wchInitStopChar)
        break;
      wchCurStopChar=L' ';
    }

    if (wpArgSet < wpArgSetMax)
    {
      if (wszArg) *wpArgSet=*wpCount;
      ++wpArgSet;
    }
  }
  if (wszArg) *wpArgSet=L'\0';

  if (wpNextArg)
  {
    if (*wpCount) ++wpCount;
    for (*wpNextArg=wpCount; **wpNextArg == L' '; ++*wpNextArg);
  }
  return (int)(wpArgSet - wszArg);
}
