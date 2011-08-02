#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "StrFunc.h"
#include "AkelDLL.h"


//Include string functions
#define xatoiA
#define xstrcpynA
#define xstrlenA
#include "StrFunc.h"

//Defines
#define PE_NONE              0
#define PE_NOTINLIST         1
#define PE_NOTPLUGIN         2
#define PE_CANTLOAD          3

typedef struct {
  char szPluginName[MAX_PATH];
  DWORD dwError;
} DLLINFO;

//Functions prototypes
int GetCommandLineArgA(char *pCmdLine, char *szArg, int nArgMax, char **pNextArg);

//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

//Entry point
void _WinMain()
{
  PLUGINVERSION pv;
  DLLINFO di;
  HMODULE hInstance;
  HWND hWndSend=NULL;
  char *pCmdLine=GetCommandLineA();
  char *pArguments=pCmdLine;
  char szWindow[MAX_PATH];
  char szFile[MAX_PATH];
  void (*DllAkelPadID)(PLUGINVERSION *pv);

  di.dwError=PE_CANTLOAD;

  //Skip executable
  GetCommandLineArgA(pArguments, NULL, 0, &pArguments);

  //First argument is window handle
  if (GetCommandLineArgA(pArguments, szWindow, MAX_PATH, &pArguments))
  {
    hWndSend=(HWND)xatoiA(szWindow, NULL);

    //Second argument is dll file to check
    if (GetCommandLineArgA(pArguments, szFile, MAX_PATH, &pArguments))
    {
      if (hInstance=LoadLibraryA(szFile))
      {
        if (DllAkelPadID=(void (*)(PLUGINVERSION *))GetProcAddress(hInstance, "DllAkelPadID"))
        {
          DllAkelPadID(&pv);

          xstrcpynA(di.szPluginName, pv.pPluginName, MAX_PATH);
          di.dwError=PE_NONE;
        }
        else di.dwError=PE_NOTPLUGIN;

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

int GetCommandLineArgA(char *pCmdLine, char *szArg, int nArgMax, char **pNextArg)
{
  char *pCount=pCmdLine;
  char *pArgSet=szArg;
  char *pArgSetMax=szArg + nArgMax - 1;
  char chInitStopChar;
  char chCurStopChar;

  while (*pCount == ' ') ++pCount;

  if (*pCount == '\"' || *pCount == '\'' || *pCount == '`')
    chInitStopChar=*pCount++;
  else
    chInitStopChar=' ';
  chCurStopChar=chInitStopChar;

  for (; *pCount; ++pCount)
  {
    if (chCurStopChar == ' ')
    {
      if (*pCount == ' ')
        break;
      if (*pCount == '\"' || *pCount == '\'' || *pCount == '`')
        chCurStopChar=*pCount;
    }
    else if (chCurStopChar == *pCount)
    {
      if (chCurStopChar == chInitStopChar)
        break;
      chCurStopChar=' ';
    }

    if (pArgSet < pArgSetMax)
    {
      if (szArg) *pArgSet=*pCount;
      ++pArgSet;
    }
  }
  if (szArg) *pArgSet='\0';

  if (pNextArg)
  {
    if (*pCount) ++pCount;
    for (*pNextArg=pCount; **pNextArg == ' '; ++*pNextArg);
  }
  return (int)(pArgSet - szArg);
}
