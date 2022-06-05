#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
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
//*/

//Defines
#define STRID_EXITMSG                   1
#define STRID_PROMPTEXIT                2
#define STRID_CMDLINE                   3
#define STRID_ESCAPE                    4
#define STRID_ESCEXIT                   5
#define STRID_ESCMINIMIZE               6
#define STRID_ESCIGNORE                 7
#define STRID_ESCCLOSETAB               8
#define STRID_ESCCLOSETAB_ONETABEXIT    9
#define STRID_ESCCLOSETAB_NOTABEXIT     10
#define STRID_ESCCLOSETAB_NOTABDONTEXIT 11
#define STRID_PLUGIN                    12
#define STRID_OK                        13
#define STRID_CANCEL                    14

#define DLLA_EXIT_EMULATEESC   1

#define EX_PROMPTEXIT                   0x00000001
#define EX_ESCEXIT                      0x00000002
#define EX_ESCMINIMIZE                  0x00000004
#define EX_ESCIGNORE                    0x00000008
#define EX_ESCCLOSETAB_ONETABEXIT       0x00000020
#define EX_ESCCLOSETAB_NOTABEXIT        0x00000040
#define EX_ESCCLOSETAB_NOTABDONTEXIT    0x00000080
#define EX_CMDLINE                      0x00001000

//IDM_FILE_SAVEALL
#define TXT_DEFAULT_CMDLINE L"/Command(4110)"

//Functions prototypes
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL ProcessEsc(DWORD dwSettings);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
wchar_t wszExitMessage[MAX_PATH]=L"";
wchar_t wszExitCmdLine[COMMANDLINE_SIZE];
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
DWORD dwSettings=EX_ESCEXIT;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
WNDPROCDATA *NewMainProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Exit";
}

//Plugin extern functions
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_EXIT_EMULATEESC)
    {
      DWORD dwProcessEsc=(DWORD)-1;

      if (IsExtCallParamValid(pd->lParam, 2))
        dwProcessEsc=(DWORD)GetExtCallParam(pd->lParam, 2);

      if (dwProcessEsc == (DWORD)-1)
        dwProcessEsc=dwSettings;
      if (!ProcessEsc(dwProcessEsc))
        PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
    }

    //If plugin already loaded, stay in memory and don't change active status
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
    return;
  }

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
  static HWND hWndPromptExitEnable;
  static HWND hWndPromptExit;
  static HWND hWndCmdLineEnable;
  static HWND hWndCmdLine;
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

    hWndPromptExitEnable=GetDlgItem(hDlg, IDC_PROMPTEXIT_ENABLE);
    hWndPromptExit=GetDlgItem(hDlg, IDC_PROMPTEXIT);
    hWndCmdLineEnable=GetDlgItem(hDlg, IDC_CMDLINE_ENABLE);
    hWndCmdLine=GetDlgItem(hDlg, IDC_CMDLINE);
    hWndEscExit=GetDlgItem(hDlg, IDC_ESCEXIT);
    hWndEscMinimize=GetDlgItem(hDlg, IDC_ESCMINIMIZE);
    hWndEscIgnore=GetDlgItem(hDlg, IDC_ESCIGNORE);
    hWndEscCloseTab=GetDlgItem(hDlg,  IDC_ESCCLOSETAB);
    hWndEscCloseOneTabExit=GetDlgItem(hDlg, IDC_ESCCLOSETAB_ONETABEXIT);
    hWndEscCloseNoTabExit=GetDlgItem(hDlg, IDC_ESCCLOSETAB_NOTABEXIT);
    hWndEscCloseNoTabDontExit=GetDlgItem(hDlg, IDC_ESCCLOSETAB_NOTABDONTEXIT);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetWindowTextWide(hWndPromptExitEnable, GetLangStringW(wLangModule, STRID_PROMPTEXIT));
    SetWindowTextWide(hWndCmdLineEnable, GetLangStringW(wLangModule, STRID_CMDLINE));
    SetDlgItemTextWide(hDlg, IDC_ESCAPE_GROUP, GetLangStringW(wLangModule, STRID_ESCAPE));
    SetWindowTextWide(hWndEscExit, GetLangStringW(wLangModule, STRID_ESCEXIT));
    SetWindowTextWide(hWndEscMinimize, GetLangStringW(wLangModule, STRID_ESCMINIMIZE));
    SetWindowTextWide(hWndEscIgnore, GetLangStringW(wLangModule, STRID_ESCIGNORE));
    SetWindowTextWide(hWndEscCloseTab, GetLangStringW(wLangModule, STRID_ESCCLOSETAB));
    SetWindowTextWide(hWndEscCloseOneTabExit, GetLangStringW(wLangModule, STRID_ESCCLOSETAB_ONETABEXIT));
    SetWindowTextWide(hWndEscCloseNoTabExit, GetLangStringW(wLangModule, STRID_ESCCLOSETAB_NOTABEXIT));
    SetWindowTextWide(hWndEscCloseNoTabDontExit, GetLangStringW(wLangModule, STRID_ESCCLOSETAB_NOTABDONTEXIT));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndPromptExit, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SetWindowTextWide(hWndPromptExit, wszExitMessage);
    SendMessage(hWndCmdLine, EM_LIMITTEXT, (WPARAM)COMMANDLINE_SIZE, 0);
    SetWindowTextWide(hWndCmdLine, wszExitCmdLine);

    dwDialogSettings=dwSettings;
    EnableWindow(hWndEscCloseOneTabExit, FALSE);
    EnableWindow(hWndEscCloseNoTabExit, FALSE);
    EnableWindow(hWndEscCloseNoTabDontExit, FALSE);

    if (dwDialogSettings & EX_PROMPTEXIT)
    {
      EnableWindow(hWndPromptExit, TRUE);
      SendMessage(hWndPromptExitEnable, BM_SETCHECK, BST_CHECKED, 0);
    }
    else EnableWindow(hWndPromptExit, FALSE);

    if (dwDialogSettings & EX_CMDLINE)
    {
      EnableWindow(hWndCmdLine, TRUE);
      SendMessage(hWndCmdLineEnable, BM_SETCHECK, BST_CHECKED, 0);
    }
    else EnableWindow(hWndCmdLine, FALSE);

    if (dwDialogSettings & EX_ESCEXIT)
      SendMessage(hWndEscExit, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwDialogSettings & EX_ESCMINIMIZE)
      SendMessage(hWndEscMinimize, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwDialogSettings & EX_ESCIGNORE)
      SendMessage(hWndEscIgnore, BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      EnableWindow(hWndEscCloseOneTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabDontExit, TRUE);
      SendMessage(hWndEscCloseTab, BM_SETCHECK, BST_CHECKED, 0);

      if (dwDialogSettings & EX_ESCCLOSETAB_NOTABEXIT)
        SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_CHECKED, 0);
      else if (dwDialogSettings & EX_ESCCLOSETAB_NOTABDONTEXIT)
        SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_CHECKED, 0);
      else //EX_ESCCLOSETAB_ONETABEXIT
        SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_CHECKED, 0);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PROMPTEXIT_ENABLE)
    {
      if (SendMessage(hWndPromptExitEnable, BM_GETCHECK, 0, 0) == BST_CHECKED)
        EnableWindow(hWndPromptExit, TRUE);
      else
        EnableWindow(hWndPromptExit, FALSE);
    }
    else if (LOWORD(wParam) == IDC_CMDLINE_ENABLE)
    {
      if (SendMessage(hWndCmdLineEnable, BM_GETCHECK, 0, 0) == BST_CHECKED)
        EnableWindow(hWndCmdLine, TRUE);
      else
        EnableWindow(hWndCmdLine, FALSE);
    }
    else if (LOWORD(wParam) == IDC_ESCEXIT ||
             LOWORD(wParam) == IDC_ESCMINIMIZE ||
             LOWORD(wParam) == IDC_ESCIGNORE)
    {
      EnableWindow(hWndEscCloseOneTabExit, FALSE);
      EnableWindow(hWndEscCloseNoTabExit, FALSE);
      EnableWindow(hWndEscCloseNoTabDontExit, FALSE);
    }
    else if (LOWORD(wParam) == IDC_ESCCLOSETAB)
    {
      EnableWindow(hWndEscCloseOneTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabExit, TRUE);
      EnableWindow(hWndEscCloseNoTabDontExit, TRUE);

      if (dwDialogSettings & EX_ESCCLOSETAB_NOTABEXIT)
        SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_CHECKED, 0);
      else if (dwDialogSettings & EX_ESCCLOSETAB_NOTABDONTEXIT)
        SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_CHECKED, 0);
      else //EX_ESCCLOSETAB_ONETABEXIT
        SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_CHECKED, 0);
    }
    else if (LOWORD(wParam) == IDC_ESCCLOSETAB_ONETABEXIT)
    {
      SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_UNCHECKED, 0);

      dwDialogSettings|=EX_ESCCLOSETAB_ONETABEXIT;
      dwDialogSettings&=~EX_ESCCLOSETAB_NOTABEXIT & ~EX_ESCCLOSETAB_NOTABDONTEXIT;
    }
    else if (LOWORD(wParam) == IDC_ESCCLOSETAB_NOTABEXIT)
    {
      SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_CHECKED, 0);
      SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_UNCHECKED, 0);

      dwDialogSettings|=EX_ESCCLOSETAB_NOTABEXIT;
      dwDialogSettings&=~EX_ESCCLOSETAB_ONETABEXIT & ~EX_ESCCLOSETAB_NOTABDONTEXIT;
    }
    else if (LOWORD(wParam) == IDC_ESCCLOSETAB_NOTABDONTEXIT)
    {
      SendMessage(hWndEscCloseOneTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabExit, BM_SETCHECK, BST_UNCHECKED, 0);
      SendMessage(hWndEscCloseNoTabDontExit, BM_SETCHECK, BST_CHECKED, 0);

      dwDialogSettings|=EX_ESCCLOSETAB_NOTABDONTEXIT;
      dwDialogSettings&=~EX_ESCCLOSETAB_ONETABEXIT & ~EX_ESCCLOSETAB_NOTABEXIT;
    }
    else if (LOWORD(wParam) == IDOK)
    {
      dwSettings=0;

      if (SendMessage(hWndPromptExitEnable, BM_GETCHECK, 0, 0) == BST_CHECKED)
      {
        GetWindowTextWide(hWndPromptExit, wszExitMessage, MAX_PATH);
        dwSettings|=EX_PROMPTEXIT;
      }
      if (SendMessage(hWndCmdLineEnable, BM_GETCHECK, 0, 0) == BST_CHECKED)
      {
        GetWindowTextWide(hWndCmdLine, wszExitCmdLine, COMMANDLINE_SIZE);
        dwSettings|=EX_CMDLINE;
      }
      if (SendMessage(hWndEscExit, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSettings|=EX_ESCEXIT;
      if (SendMessage(hWndEscMinimize, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSettings|=EX_ESCMINIMIZE;
      if (SendMessage(hWndEscIgnore, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSettings|=EX_ESCIGNORE;
      if (SendMessage(hWndEscCloseTab, BM_GETCHECK, 0, 0) == BST_CHECKED)
      {
        if (SendMessage(hWndEscCloseNoTabExit, BM_GETCHECK, 0, 0) == BST_CHECKED)
          dwSettings|=EX_ESCCLOSETAB_NOTABEXIT;
        else if (SendMessage(hWndEscCloseNoTabDontExit, BM_GETCHECK, 0, 0) == BST_CHECKED)
          dwSettings|=EX_ESCCLOSETAB_NOTABDONTEXIT;
        else //hWndEscCloseOneTabExit
          dwSettings|=EX_ESCCLOSETAB_ONETABEXIT;
      }

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
      if (ProcessEsc(dwSettings))
        return FALSE;
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    if (dwSettings & EX_PROMPTEXIT)
    {
      if (MessageBoxW(hMainWnd, wszExitMessage, wszPluginTitle, MB_YESNO|MB_ICONQUESTION) == IDNO)
        return FALSE;
    }
    if (dwSettings & EX_CMDLINE)
    {
      PARSECMDLINESENDW pcls;

      pcls.pCmdLine=wszExitCmdLine;
      pcls.pWorkDir=L"";
      SendMessage(hMainWnd, AKD_PARSECMDLINEW, 0, (LPARAM)&pcls);
    }
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

BOOL ProcessEsc(DWORD dwSettings)
{
  if (dwSettings & EX_ESCMINIMIZE)
  {
    PostMessage(hMainWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    return TRUE;
  }
  if (dwSettings & EX_ESCIGNORE)
    return TRUE;

  if (nMDI == WMD_MDI || nMDI == WMD_PMDI)
  {
    if (dwSettings & EX_ESCCLOSETAB_ONETABEXIT)
    {
      if (!SendMessage(hMainWnd, AKD_FRAMENOWINDOWS, 0, 0))
      {
        int nFrameCount=(int)SendMessage(hMainWnd, AKD_FRAMESTATS, FWS_COUNTALL, 0);

        SendMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSE, 0);
        if (nFrameCount > 1)
          return TRUE;
      }
    }
    else if (dwSettings & EX_ESCCLOSETAB_NOTABEXIT)
    {
      if (!SendMessage(hMainWnd, AKD_FRAMENOWINDOWS, 0, 0))
      {
        SendMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSE, 0);
        return TRUE;
      }
    }
    else if (dwSettings & EX_ESCCLOSETAB_NOTABDONTEXIT)
    {
      if (!SendMessage(hMainWnd, AKD_FRAMENOWINDOWS, 0, 0))
      {
        SendMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSE, 0);
      }
      return TRUE;
    }
  }
  return FALSE;
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
    WideOption(hOptions, L"ExitMessage", PO_BINARY, (LPBYTE)wszExitMessage, sizeof(wszExitMessage));
    WideOption(hOptions, L"ExitCmdLine", PO_STRING, (LPBYTE)wszExitCmdLine, sizeof(wszExitCmdLine));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
  if (!*wszExitMessage) xstrcpynW(wszExitMessage, GetLangStringW(wLangModule, STRID_EXITMSG), MAX_PATH);
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"Settings", PO_DWORD, (LPBYTE)&dwSettings, sizeof(DWORD));
    WideOption(hOptions, L"ExitMessage", PO_BINARY, (LPBYTE)wszExitMessage, ((int)xstrlenW(wszExitMessage) + 1) * sizeof(wchar_t));
    WideOption(hOptions, L"ExitCmdLine", PO_STRING, (LPBYTE)wszExitCmdLine, ((int)xstrlenW(wszExitCmdLine) + 1) * sizeof(wchar_t));

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
    if (nStringID == STRID_EXITMSG)
      return L"\x0412\x044B\x0020\x0434\x0435\x0439\x0441\x0442\x0432\x0438\x0442\x0435\x043B\x044C\x043D\x043E\x0020\x0445\x043E\x0442\x0438\x0442\x0435\x0020\x0432\x044B\x0439\x0442\x0438\x003F";
    if (nStringID == STRID_PROMPTEXIT)
      return L"\x041F\x043E\x0434\x0442\x0432\x0435\x0440\x0436\x0434\x0435\x043D\x0438\x0435\x0020\x0432\x044B\x0445\x043E\x0434\x0430";
    if (nStringID == STRID_CMDLINE)
      return L"\x041A\x043E\x043C\x0430\x043D\x0434\x043D\x0430\x044F\x0020\x0441\x0442\x0440\x043E\x043A\x0430\x0020\x0432\x044B\x0445\x043E\x0434\x0430";
    if (nStringID == STRID_ESCAPE)
      return L"\x0045\x0073\x0063\x0061\x0070\x0065";
    if (nStringID == STRID_ESCEXIT)
      return L"\x0412\x044B\x0439\x0442\x0438\x0020\x0438\x0437\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x044B";
    if (nStringID == STRID_ESCMINIMIZE)
      return L"\x0421\x0432\x0435\x0440\x043D\x0443\x0442\x044C\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x0443";
    if (nStringID == STRID_ESCIGNORE)
      return L"\x0418\x0433\x043D\x043E\x0440\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_ESCCLOSETAB)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C\x0020\x0430\x043A\x0442\x0438\x0432\x043D\x0443\x044E\x0020\x0432\x043A\x043B\x0430\x0434\x043A\x0443\x0020\x0028\x004D\x0044\x0049\x0029";
    if (nStringID == STRID_ESCCLOSETAB_ONETABEXIT)
      return L"\x0412\x044B\x0439\x0442\x0438\x002C\x0020\x0435\x0441\x043B\x0438\x0020\x043E\x0434\x043D\x0430\x0020\x0432\x043A\x043B\x0430\x0434\x043A\x0430";
    if (nStringID == STRID_ESCCLOSETAB_NOTABEXIT)
      return L"\x0412\x044B\x0439\x0442\x0438\x002C\x0020\x0435\x0441\x043B\x0438\x0020\x043D\x0435\x0442\x0020\x0432\x043A\x043B\x0430\x0434\x043E\x043A";
    if (nStringID == STRID_ESCCLOSETAB_NOTABDONTEXIT)
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
    if (nStringID == STRID_EXITMSG)
      return L"Are you sure you want to quit?";
    if (nStringID == STRID_PROMPTEXIT)
      return L"Prompt exit";
    if (nStringID == STRID_CMDLINE)
      return L"Exit command line";
    if (nStringID == STRID_ESCAPE)
      return L"Escape";
    if (nStringID == STRID_ESCEXIT)
      return L"Close program";
    if (nStringID == STRID_ESCMINIMIZE)
      return L"Minimize program";
    if (nStringID == STRID_ESCIGNORE)
      return L"Ignore";
    if (nStringID == STRID_ESCCLOSETAB)
      return L"Close active tab (MDI)";
    if (nStringID == STRID_ESCCLOSETAB_ONETABEXIT)
      return L"Exit if one tab to close";
    if (nStringID == STRID_ESCCLOSETAB_NOTABEXIT)
      return L"Exit if no tab to close";
    if (nStringID == STRID_ESCCLOSETAB_NOTABDONTEXIT)
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

BOOL IsExtCallParamValid(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return TRUE;
  return FALSE;
}

INT_PTR GetExtCallParam(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return *(((INT_PTR *)lParam) + nIndex);
  return 0;
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
  xstrcpynW(wszExitCmdLine, TXT_DEFAULT_CMDLINE, COMMANDLINE_SIZE);
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
