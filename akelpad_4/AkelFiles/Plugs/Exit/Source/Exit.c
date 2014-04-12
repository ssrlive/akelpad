#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


//Include string functions
#define xmemcpy
#define xstrlenW
#define xstrcpynW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define DialogBoxWide
#define GetWindowTextWide
#define SetDlgItemTextWide
#define SetWindowTextWide
#include "WideFunc.h"

//Defines
#define STRID_EXIT_MSG                         1
#define STRID_PROMPT_EXIT                      2
#define STRID_ESCAPE                           3
#define STRID_ESC_EXIT                         4
#define STRID_ESC_MINIMIZE                     5
#define STRID_ESC_IGNORE                       6
#define STRID_ESC_CLOSE_TAB                    7
#define STRID_ESC_CLOSE_TAB__ONE_TAB_EXIT      8
#define STRID_ESC_CLOSE_TAB__NO_TAB_EXIT       9
#define STRID_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT  10
#define STRID_PLUGIN                           11
#define STRID_OK                               12
#define STRID_CANCEL                           13

#define EX_PROMT_EXIT                       0x00000001
#define EX_ESC_EXIT                         0x00000002
#define EX_ESC_MINIMIZE                     0x00000004
#define EX_ESC_IGNORE                       0x00000008
#define EX_ESC_CLOSE_TAB                    0x00000010
#define EX_ESC_CLOSE_TAB__ONE_TAB_EXIT      0x00000020
#define EX_ESC_CLOSE_TAB__NO_TAB_EXIT       0x00000040
#define EX_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT  0x00000080

//Functions prototypes
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
wchar_t wszExitMessage[MAX_PATH]=L"";
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hMdiClient;
HWND hTab;
HICON hMainIcon;
int nMDI;
BOOL bOldWindows;
LANGID wLangModule;
DWORD dwSettings=EX_ESC_EXIT|EX_ESC_CLOSE_TAB__ONE_TAB_EXIT;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
WNDPROCDATA *NewMainProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 8, 8, 0);
  pv->pPluginName="Exit";
}

//Plugin extern functions
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

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

void __declspec(dllexport) Settings(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hMainWnd, (DLGPROC)SetupDlgProc);

  //If plugin already loaded, stay in memory, but show as non-active
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndPromptExit;
  static HWND hWndPromptExitMsg;
  static HWND hWndEscExit;
  static HWND hWndEscMinimize;
  static HWND hWndEscIgnore;
  static HWND hWndEscCloseTab;
  static HWND hWndEscCloseOneTabExit;
  static HWND hWndEscCloseNoTabExit;
  static HWND hWndEscCloseNoTabDontExit;
  static DWORD dwDialogSettings;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndPromptExit=GetDlgItem(hDlg, IDC_PROMPT_EXIT);
    hWndPromptExitMsg=GetDlgItem(hDlg, IDC_PROMPT_EXIT_MSG);
    hWndEscExit=GetDlgItem(hDlg, IDC_ESC_EXIT);
    hWndEscMinimize=GetDlgItem(hDlg, IDC_ESC_MINIMIZE);
    hWndEscIgnore=GetDlgItem(hDlg, IDC_ESC_IGNORE);
    hWndEscCloseTab=GetDlgItem(hDlg,  IDC_ESC_CLOSE_TAB);
    hWndEscCloseOneTabExit=GetDlgItem(hDlg, IDC_ESC_CLOSE_TAB__ONE_TAB_EXIT);
    hWndEscCloseNoTabExit=GetDlgItem(hDlg, IDC_ESC_CLOSE_TAB__NO_TAB_EXIT);
    hWndEscCloseNoTabDontExit=GetDlgItem(hDlg, IDC_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetWindowTextWide(hWndPromptExit, GetLangStringW(wLangModule, STRID_PROMPT_EXIT));
    SetDlgItemTextWide(hDlg, IDC_ESCAPE, GetLangStringW(wLangModule, STRID_ESCAPE));
    SetWindowTextWide(hWndEscExit, GetLangStringW(wLangModule, STRID_ESC_EXIT));
    SetWindowTextWide(hWndEscMinimize, GetLangStringW(wLangModule, STRID_ESC_MINIMIZE));
    SetWindowTextWide(hWndEscIgnore, GetLangStringW(wLangModule, STRID_ESC_IGNORE));
    SetWindowTextWide(hWndEscCloseTab, GetLangStringW(wLangModule, STRID_ESC_CLOSE_TAB));
    SetWindowTextWide(hWndEscCloseOneTabExit, GetLangStringW(wLangModule, STRID_ESC_CLOSE_TAB__ONE_TAB_EXIT));
    SetWindowTextWide(hWndEscCloseNoTabExit, GetLangStringW(wLangModule, STRID_ESC_CLOSE_TAB__NO_TAB_EXIT));
    SetWindowTextWide(hWndEscCloseNoTabDontExit, GetLangStringW(wLangModule, STRID_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndPromptExitMsg, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SetWindowTextWide(hWndPromptExitMsg, wszExitMessage);
    EnableWindow(hWndPromptExitMsg, FALSE);
    EnableWindow(hWndEscCloseOneTabExit, FALSE);
    EnableWindow(hWndEscCloseNoTabExit, FALSE);
    EnableWindow(hWndEscCloseNoTabDontExit, FALSE);

    dwDialogSettings=dwSettings;

    if (dwDialogSettings & EX_PROMT_EXIT)
    {
      EnableWindow(hWndPromptExitMsg, TRUE);
      SendMessage(hWndPromptExit, BM_SETCHECK, BST_CHECKED, 0);
    }
    if (dwDialogSettings & EX_ESC_EXIT)
      SendMessage(hWndEscExit, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwDialogSettings & EX_ESC_MINIMIZE)
      SendMessage(hWndEscMinimize, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwDialogSettings & EX_ESC_IGNORE)
      SendMessage(hWndEscIgnore, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwDialogSettings & EX_ESC_CLOSE_TAB)
    {
      EnableWindow(hWndEscCloseOneTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabDontExit, TRUE);
      SendMessage(hWndEscCloseTab, BM_SETCHECK, BST_CHECKED, 0);

      if (dwDialogSettings & EX_ESC_CLOSE_TAB__ONE_TAB_EXIT)
        SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_CHECKED, 0);
      else if (dwDialogSettings & EX_ESC_CLOSE_TAB__NO_TAB_EXIT)
        SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_CHECKED, 0);
      else if (dwDialogSettings & EX_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT)
        SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_CHECKED, 0);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PROMPT_EXIT)
    {
      if (SendMessage(hWndPromptExit, BM_GETCHECK, 0, 0) == BST_CHECKED)
        EnableWindow(hWndPromptExitMsg, TRUE);
      else
        EnableWindow(hWndPromptExitMsg, FALSE);
    }
    else if (LOWORD(wParam) == IDC_ESC_EXIT ||
             LOWORD(wParam) == IDC_ESC_MINIMIZE ||
             LOWORD(wParam) == IDC_ESC_IGNORE)
    {
      EnableWindow(hWndEscCloseOneTabExit, FALSE);
      EnableWindow(hWndEscCloseNoTabExit, FALSE);
      EnableWindow(hWndEscCloseNoTabDontExit, FALSE);
    }
    else if (LOWORD(wParam) == IDC_ESC_CLOSE_TAB)
    {
      EnableWindow(hWndEscCloseOneTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabDontExit, TRUE);

      if (dwDialogSettings & EX_ESC_CLOSE_TAB__ONE_TAB_EXIT)
        SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_CHECKED, 0);
      else if (dwDialogSettings & EX_ESC_CLOSE_TAB__NO_TAB_EXIT)
        SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_CHECKED, 0);
      else if (dwDialogSettings & EX_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT)
        SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_CHECKED, 0);
    }
    else if (LOWORD(wParam) == IDC_ESC_CLOSE_TAB__ONE_TAB_EXIT)
    {
      SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_UNCHECKED, 0);

      dwDialogSettings|=EX_ESC_CLOSE_TAB__ONE_TAB_EXIT;
      dwDialogSettings&=~EX_ESC_CLOSE_TAB__NO_TAB_EXIT & ~EX_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT;
    }
    else if (LOWORD(wParam) == IDC_ESC_CLOSE_TAB__NO_TAB_EXIT)
    {
      SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_UNCHECKED, 0);

      dwDialogSettings|=EX_ESC_CLOSE_TAB__NO_TAB_EXIT;
      dwDialogSettings&=~EX_ESC_CLOSE_TAB__ONE_TAB_EXIT & ~EX_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT;
    }
    else if (LOWORD(wParam) == IDC_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT)
    {
      SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_CHECKED, 0);

      dwDialogSettings|=EX_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT;
      dwDialogSettings&=~EX_ESC_CLOSE_TAB__ONE_TAB_EXIT & ~EX_ESC_CLOSE_TAB__NO_TAB_EXIT;
    }
    else if (LOWORD(wParam) == IDOK)
    {
      dwDialogSettings&=~EX_PROMT_EXIT & ~EX_ESC_EXIT & ~EX_ESC_MINIMIZE & ~EX_ESC_IGNORE & ~EX_ESC_CLOSE_TAB;

      if (SendMessage(hWndPromptExit, BM_GETCHECK, 0, 0) == BST_CHECKED)
      {
        GetWindowTextWide(hWndPromptExitMsg, wszExitMessage, MAX_PATH);
        dwDialogSettings|=EX_PROMT_EXIT;
      }
      if (SendMessage(hWndEscExit, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDialogSettings|=EX_ESC_EXIT;
      if (SendMessage(hWndEscMinimize, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDialogSettings|=EX_ESC_MINIMIZE;
      if (SendMessage(hWndEscIgnore, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDialogSettings|=EX_ESC_IGNORE;
      if (SendMessage(hWndEscCloseTab, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDialogSettings|=EX_ESC_CLOSE_TAB;
      dwSettings=dwDialogSettings;

      SaveOptions(0);
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Destroy plugin icon
    DestroyIcon(hPluginIcon);
  }
  return FALSE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDM_FILE_EXIT && HIWORD(wParam) == 1)
    {
      if (dwSettings & EX_ESC_MINIMIZE)
      {
        PostMessage(hMainWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
        return FALSE;
      }
      if (dwSettings & EX_ESC_IGNORE)
        return FALSE;

      if (nMDI == WMD_MDI || nMDI == WMD_PMDI)
      {
        if (dwSettings & EX_ESC_CLOSE_TAB)
        {
          if (dwSettings & EX_ESC_CLOSE_TAB__ONE_TAB_EXIT)
          {
            if (SendMessage(hTab, TCM_GETITEMCOUNT, 0, 0) > 1)
            {
              PostMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSE, 0);
              return FALSE;
            }
          }
          else if (dwSettings & EX_ESC_CLOSE_TAB__NO_TAB_EXIT)
          {
            if (!SendMessage(hMainWnd, AKD_FRAMENOWINDOWS, 0, 0))
            {
              PostMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSE, 0);
              return FALSE;
            }
          }
          else if (dwSettings & EX_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT)
          {
            if (!SendMessage(hMainWnd, AKD_FRAMENOWINDOWS, 0, 0))
            {
              PostMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSE, 0);
            }
            return FALSE;
          }
        }
      }
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    if (dwSettings & EX_PROMT_EXIT)
    {
      if (MessageBoxW(hMainWnd, wszExitMessage, wszPluginTitle, MB_YESNO|MB_ICONQUESTION) == IDNO)
        return FALSE;
    }
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}


//// Options

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData)
{
  PLUGINOPTIONW po;

  po.pOptionName=pOptionName;
  po.dwType=dwType;
  po.lpData=lpData;
  po.dwData=dwData;
  return SendMessage(hMainWnd, AKD_OPTIONW, (WPARAM)hOptions, (LPARAM)&po);
}

void ReadOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"Settings", PO_DWORD, (LPBYTE)&dwSettings, sizeof(DWORD));
    WideOption(hOptions, L"ExitMessage", PO_BINARY, (LPBYTE)wszExitMessage, MAX_PATH * sizeof(wchar_t));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
  if (!*wszExitMessage) xstrcpynW(wszExitMessage, GetLangStringW(wLangModule, STRID_EXIT_MSG), MAX_PATH);
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"Settings", PO_DWORD, (LPBYTE)&dwSettings, sizeof(DWORD));
    WideOption(hOptions, L"ExitMessage", PO_BINARY, (LPBYTE)wszExitMessage, (lstrlenW(wszExitMessage) + 1) * sizeof(wchar_t));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
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
    if (nStringID == STRID_EXIT_MSG)
      return L"\x0412\x044B\x0020\x0434\x0435\x0439\x0441\x0442\x0432\x0438\x0442\x0435\x043B\x044C\x043D\x043E\x0020\x0445\x043E\x0442\x0438\x0442\x0435\x0020\x0432\x044B\x0439\x0442\x0438\x003F";
    if (nStringID == STRID_PROMPT_EXIT)
      return L"\x041F\x043E\x0434\x0442\x0432\x0435\x0440\x0436\x0434\x0435\x043D\x0438\x0435\x0020\x0432\x044B\x0445\x043E\x0434\x0430";
    if (nStringID == STRID_ESCAPE)
      return L"\x0045\x0073\x0063\x0061\x0070\x0065";
    if (nStringID == STRID_ESC_EXIT)
      return L"\x0412\x044B\x0439\x0442\x0438\x0020\x0438\x0437\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x044B";
    if (nStringID == STRID_ESC_MINIMIZE)
      return L"\x0421\x0432\x0435\x0440\x043D\x0443\x0442\x044C\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x0443";
    if (nStringID == STRID_ESC_IGNORE)
      return L"\x0418\x0433\x043D\x043E\x0440\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_ESC_CLOSE_TAB)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C\x0020\x0430\x043A\x0442\x0438\x0432\x043D\x0443\x044E\x0020\x0432\x043A\x043B\x0430\x0434\x043A\x0443\x0020\x0028\x004D\x0044\x0049\x0029";
    if (nStringID == STRID_ESC_CLOSE_TAB__ONE_TAB_EXIT)
      return L"\x0412\x044B\x0439\x0442\x0438\x002C\x0020\x0435\x0441\x043B\x0438\x0020\x043E\x0434\x043D\x0430\x0020\x0432\x043A\x043B\x0430\x0434\x043A\x0430";
    if (nStringID == STRID_ESC_CLOSE_TAB__NO_TAB_EXIT)
      return L"\x0412\x044B\x0439\x0442\x0438\x002C\x0020\x0435\x0441\x043B\x0438\x0020\x043D\x0435\x0442\x0020\x0432\x043A\x043B\x0430\x0434\x043E\x043A";
    if (nStringID == STRID_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT)
      return L"\x041D\x0435\x0020\x0432\x044B\x0445\x043E\x0434\x0438\x0442\x044C\x002C\x0020\x0435\x0441\x043B\x0438\x0020\x043D\x0435\x0442\x0020\x0432\x043A\x043B\x0430\x0434\x043E\x043A";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_EXIT_MSG)
      return L"Are you sure you want to quit?";
    if (nStringID == STRID_PROMPT_EXIT)
      return L"Prompt exit";
    if (nStringID == STRID_ESCAPE)
      return L"Escape";
    if (nStringID == STRID_ESC_EXIT)
      return L"Close program";
    if (nStringID == STRID_ESC_MINIMIZE)
      return L"Minimize program";
    if (nStringID == STRID_ESC_IGNORE)
      return L"Ignore";
    if (nStringID == STRID_ESC_CLOSE_TAB)
      return L"Close active tab (MDI)";
    if (nStringID == STRID_ESC_CLOSE_TAB__ONE_TAB_EXIT)
      return L"Exit if one tab to close";
    if (nStringID == STRID_ESC_CLOSE_TAB__NO_TAB_EXIT)
      return L"Exit if no tab to close";
    if (nStringID == STRID_ESC_CLOSE_TAB__NO_TAB_DONT_EXIT)
      return L"Do not exit if no tab to close";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
  }
  return L"";
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hInstanceDLL=pd->hInstanceDLL;
  hMainWnd=pd->hMainWnd;
  hMainIcon=pd->hMainIcon;
  hMdiClient=pd->hMdiClient;
  hTab=pd->hTab;
  nMDI=pd->nMDI;
  bOldWindows=pd->bOldWindows;
  wLangModule=PRIMARYLANGID(pd->wLangModule);

  //Initialize WideFunc.h header
  WideInitialize();

  //Plugin name
  {
    int i;

    for (i=0; pd->wszFunction[i] != L':'; ++i)
      wszPluginName[i]=pd->wszFunction[i];
    wszPluginName[i]=L'\0';
  }
  xprintfW(wszPluginTitle, GetLangStringW(wLangModule, STRID_PLUGIN), wszPluginName);
  ReadOptions(0);
}

void InitMain()
{
  bInitMain=TRUE;

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);
}

void UninitMain()
{
  bInitMain=FALSE;

  //Remove subclass
  if (NewMainProcData)
  {
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
    NewMainProcData=NULL;
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
