#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"


/*
//Include stack functions
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackDelete
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xstrlenW
#define xstrcpynW
#define xstrcmpiW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define AddFontResourceWide
#define FindFirstFileWide
#define FindNextFileWide
#define RemoveFontResourceWide
#include "WideFunc.h"
//*/

//Defines
typedef struct _FONTNAME {
  struct _FONTNAME *next;
  struct _FONTNAME *prev;
  wchar_t wszFont[MAX_PATH];
} FONTNAME;

//Functions prototypes
const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
HWND hMainWnd;
BOOL bOldWindows;
BOOL bWindowsNT4=FALSE;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
char szFontsDir[MAX_PATH];
wchar_t wszFontsDir[MAX_PATH];
char szBuffer[BUFFER_SIZE];
wchar_t wszBuffer[BUFFER_SIZE];
HSTACK hFontsStack={0};
BOOL bFontsAdded=FALSE;

//GetProcAddress
int (WINAPI *AddFontResourceExWPtr)(const wchar_t *, DWORD, VOID *);
BOOL (WINAPI *RemoveFontResourceExWPtr)(const wchar_t *, DWORD, VOID *);


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Fonts";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  //Is plugin already loaded?
  if (bInitMain)
  {
    UninitMain();
  }
  else
  {
    InitMain();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'.') return wpCount + 1;
    if (*wpCount == L'\\') break;
  }
  return NULL;
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hMainWnd=pd->hMainWnd;
  bOldWindows=pd->bOldWindows;

  //Initialize WideFunc.h header
  WideInitialize();

  //Get functions addresses
  {
    HMODULE hGdi32;

    hGdi32=GetModuleHandleA("gdi32.dll");
    AddFontResourceExWPtr=(int (WINAPI *)(const wchar_t *, DWORD, VOID *))GetProcAddress(hGdi32, "AddFontResourceExW");
    RemoveFontResourceExWPtr=(BOOL (WINAPI *)(const wchar_t *, DWORD, VOID *))GetProcAddress(hGdi32, "RemoveFontResourceExW");
  }

  //Is Windows NT4?
  {
    OSVERSIONINFO ovi;

    ovi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
    GetVersionEx(&ovi);
    if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
      if (ovi.dwMajorVersion == 4 && ovi.dwMinorVersion == 0)
        bWindowsNT4=TRUE;
    }
  }

  xprintfW(wszFontsDir, L"%s\\AkelFiles\\Plugs\\Fonts", pd->wszAkelDir);
}

void InitMain()
{
  bInitMain=TRUE;

  //Find font files
  {
    FONTNAME *fn;
    WIN32_FIND_DATAW wfd;
    HANDLE hFind;
    wchar_t wszFindFiles[MAX_PATH];
    const wchar_t *wpExt;

    xprintfW(wszFindFiles, L"%s\\*.*", wszFontsDir);

    if ((hFind=FindFirstFileWide(wszFindFiles, &wfd)) != INVALID_HANDLE_VALUE)
    {
      do
      {
        if (wfd.cFileName[0] == '.' && (wfd.cFileName[1] == '\0' || (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0'))) continue;

        if (wpExt=GetFileExt(wfd.cFileName, -1))
        {
          if (!xstrcmpiW(wpExt, L"fon") ||
              !xstrcmpiW(wpExt, L"fnt") ||
              !xstrcmpiW(wpExt, L"ttf") ||
              !xstrcmpiW(wpExt, L"ttc") ||
              !xstrcmpiW(wpExt, L"fot") ||
              !xstrcmpiW(wpExt, L"otf") ||
              !xstrcmpiW(wpExt, L"mmm") ||
              !xstrcmpiW(wpExt, L"pfb") ||
              !xstrcmpiW(wpExt, L"pfm"))
          {
            xprintfW(wszBuffer, L"%s\\%s", wszFontsDir, wfd.cFileName);
            if (bOldWindows || bWindowsNT4)
              AddFontResourceWide(wszBuffer);
            else
              AddFontResourceExWPtr(wszBuffer, FR_PRIVATE, 0);
            bFontsAdded=TRUE;

            if (!StackInsertIndex((stack **)&hFontsStack.first, (stack **)&hFontsStack.last, (stack **)&fn, -1, sizeof(FONTNAME)))
            {
              xstrcpynW(fn->wszFont, wszBuffer, MAX_PATH);
            }
          }
        }
      }
      while (FindNextFileWide(hFind, &wfd));

      FindClose(hFind);
    }
  }

  if (bFontsAdded)
  {
    //SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
  }
}

void UninitMain()
{
  bInitMain=FALSE;

  //Detach fonts
  {
    FONTNAME *lpElement=(FONTNAME *)hFontsStack.first;

    while (lpElement)
    {
      if (bOldWindows || bWindowsNT4)
        RemoveFontResourceWide(lpElement->wszFont);
      else
        RemoveFontResourceExWPtr(lpElement->wszFont, FR_PRIVATE, 0);

      lpElement=lpElement->next;
    }
  }
  StackClear((stack **)&hFontsStack.first, (stack **)&hFontsStack.last);

  if (bFontsAdded)
  {
    //SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
    bFontsAdded=FALSE;
  }
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
    if (bInitMain) UninitMain();
  }
  return TRUE;
}
