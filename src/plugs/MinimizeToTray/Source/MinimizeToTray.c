#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include "AkelDLL.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "Resources\resource.h"

/*
//Include string functions
#define xmemcpy
#define xstrcpynW
#define xstrlenW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define AppendMenuWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_MENU_RESTORE  1
#define STRID_MENU_EXIT     2

//Functions prototypes
LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Hide();
void Show();
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
HWND hMainWnd;
BOOL bOldWindows;
int nMDI;
HICON hMainIcon;
HICON hMainSmallIcon=NULL;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;
UINT nMinimizeToTrayMessage=0;
HMENU hMenuList=NULL;
DWORD dwMainStyle;
DWORD dwCmdShow;
BOOL bMinimizeToTrayAlways=FALSE;
BOOL bMinimizeToTrayNowOnStart=FALSE;
BOOL bCmdShow=FALSE;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="MinimizeToTray";
}

//Plugin extern function
void __declspec(dllexport) Always(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);
  if (!bInitMain) InitMain();

  if (bMinimizeToTrayAlways)
  {
    bMinimizeToTrayAlways=FALSE;

    //Stay in memory, but show as non-active
    pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    bMinimizeToTrayAlways=TRUE;

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) Now(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (pd->bOnStart) bMinimizeToTrayNowOnStart=TRUE;
  if (!bInitCommon) InitCommon(pd);
  if (!bInitMain) InitMain();

  Hide();

  //Stay in memory, but show as non-active
  pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}

LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDN_MAIN_ONSTART_PRESHOW)
  {
    if (bMinimizeToTrayNowOnStart)
    {
      NMAINSHOW *nms=(NMAINSHOW *)lParam;

      dwMainStyle=*nms->dwStyle;
      dwCmdShow=*nms->dwShow;
      nms->bProcess=FALSE;
      bCmdShow=TRUE;
    }
  }
  else if (uMsg == AKDN_MAIN_ONSTART_FINISH)
  {
    bMinimizeToTrayNowOnStart=FALSE;
  }
  else if (uMsg == WM_SYSCOMMAND)
  {
    if (hWnd == hMainWnd)
    {
      if (wParam == SC_MINIMIZE)
      {
        if (bMinimizeToTrayAlways)
        {
          Hide();
          return TRUE;
        }
      }
    }
  }
  else if (uMsg == nMinimizeToTrayMessage)
  {
    if (lParam == WM_RBUTTONUP)
    {
      POINT pt;
      int nCmd;

      GetCursorPos(&pt);
      SetForegroundWindow(hMainWnd);

      if (nCmd=TrackPopupMenu(hMenuList, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hMainWnd, NULL))
      {
        if (nCmd == IDC_ITEM_RESTORE)
        {
          Show();
        }
        else if (nCmd == IDC_ITEM_EXIT)
        {
          NOTIFYICONDATAA nid;

          //Delete notify icon
          nid.cbSize=sizeof(nid);
          nid.hWnd=hMainWnd;
          nid.uID=0;
          Shell_NotifyIconA(NIM_DELETE, &nid);

          SendMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
        }
      }
      return TRUE;
    }
    else if (lParam == WM_LBUTTONUP)
    {
      Show();
      return TRUE;
    }
  }
  else if (uMsg == AKDN_ACTIVATE ||
           uMsg == AKDN_MESSAGEBOXBEGIN)
  {
    if (!bMinimizeToTrayNowOnStart)
    {
      if (!IsWindowVisible(hMainWnd))
        Show();
    }
  }

  //Call next procedure
  if (hWnd == hMainWnd)
    return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
  else
    return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

void Hide()
{
  EDITINFO ei;
  const wchar_t *wpFileName=NULL;

  //Get tip
  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    if (*ei.wszFile)
      wpFileName=GetFileName(ei.wszFile, -1);
  }
  xprintfW(wszBuffer, L"%s%s%s", wpFileName, wpFileName?L" - ":NULL, L"AkelPad");

  if (bOldWindows)
  {
    NOTIFYICONDATAA nid;

    //Set tip
    WideCharToMultiByte(CP_ACP, 0, wszBuffer, -1, nid.szTip, 64, NULL, NULL);
    nid.szTip[63]='\0';

    //Add notify icon
    nid.cbSize=sizeof(nid);
    nid.hWnd=hMainWnd;
    nid.uID=0;
    nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
    nid.uCallbackMessage=nMinimizeToTrayMessage;
    nid.hIcon=(HICON)hMainSmallIcon;
    Shell_NotifyIconA(NIM_ADD, &nid);
  }
  else
  {
    NOTIFYICONDATAW nid;

    //Set tip
    xstrcpynW(nid.szTip, wszBuffer, 64);

    //Add notify icon
    nid.cbSize=sizeof(nid);
    nid.hWnd=hMainWnd;
    nid.uID=0;
    nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
    nid.uCallbackMessage=nMinimizeToTrayMessage;
    nid.hIcon=(HICON)hMainSmallIcon;
    Shell_NotifyIconW(NIM_ADD, &nid);
  }

  if (!bCmdShow)
  {
    //Hide main window
    ShowWindow(hMainWnd, SW_MINIMIZE);
    ShowWindow(hMainWnd, SW_HIDE);
  }
}

void Show()
{
  NOTIFYICONDATA nid;

  //Delete notify icon
  nid.cbSize=sizeof(nid);
  nid.hWnd=hMainWnd;
  nid.uID=0;
  Shell_NotifyIconA(NIM_DELETE, &nid);

  //Show main window
  if (bCmdShow)
  {
    ShowWindow(hMainWnd, (dwMainStyle == WS_MAXIMIZE)?SW_SHOWMAXIMIZED:SW_SHOW);
    if (dwCmdShow != SW_SHOWNORMAL) ShowWindow(hMainWnd, dwCmdShow);
    SetForegroundWindow(hMainWnd);
    bCmdShow=FALSE;
  }
  else
  {
    ShowWindow(hMainWnd, SW_SHOW);
    ShowWindow(hMainWnd, SW_RESTORE);
  }
}

const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
      return wpCount + 1;
  }
  return wpFile;
}

const char* GetLangStringA(LANGID wLangID, int nStringID)
{
  static char szStringBuf[MAX_PATH];

  WideCharToMultiByte(CP_ACP, 0, GetLangStringW(wLangID, nStringID), -1, szStringBuf, MAX_PATH, NULL, NULL);
  return szStringBuf;
}

const wchar_t* GetLangStringW(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_MENU_RESTORE)
      return L"\x0412\x043E\x0441\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_MENU_EXIT)
      return L"\x0412\x044B\x0439\x0442\x0438";
  }
  else
  {
    if (nStringID == STRID_MENU_RESTORE)
      return L"Restore";
    if (nStringID == STRID_MENU_EXIT)
      return L"Exit";
  }
  return L"";
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hMainWnd=pd->hMainWnd;
  bOldWindows=pd->bOldWindows;
  nMDI=pd->nMDI;
  hMainIcon=pd->hMainIcon;
  wLangModule=PRIMARYLANGID(pd->wLangModule);

  //Initialize WideFunc.h header
  WideInitialize();
}

void InitMain()
{
  bInitMain=TRUE;

  //Get 16x16 icon
  hMainSmallIcon=(HICON)CopyImage((HANDLE)hMainIcon, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_COPYFROMRESOURCE);

  //Register unique message
  if (!nMinimizeToTrayMessage)
    nMinimizeToTrayMessage=RegisterWindowMessageA("MinimizeToTray");

  //Popup menu
  if (!hMenuList)
  {
    if (hMenuList=CreatePopupMenu())
    {
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEM_RESTORE, GetLangStringW(wLangModule, STRID_MENU_RESTORE));
      AppendMenuWide(hMenuList, MF_SEPARATOR, (UINT)-1, NULL);
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEM_EXIT, GetLangStringW(wLangModule, STRID_MENU_EXIT));
    }
  }

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewEditParentProc, (LPARAM)&NewMainProcData);

  if (nMDI == WMD_MDI)
  {
    NewFrameProcData=NULL;
    SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewEditParentProc, (LPARAM)&NewFrameProcData);
  }
}

void UninitMain()
{
  bInitMain=FALSE;

  //Destroy 16x16 icon
  if (hMainSmallIcon)
  {
    DestroyIcon(hMainSmallIcon);
    hMainSmallIcon=NULL;
  }

  //Destroy menu
  if (hMenuList)
  {
    DestroyMenu(hMenuList);
    hMenuList=NULL;
  }

  //Remove subclass
  if (NewMainProcData)
  {
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
    NewMainProcData=NULL;
  }
  if (NewFrameProcData)
  {
    SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&NewFrameProcData);
    NewFrameProcData=NULL;
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
