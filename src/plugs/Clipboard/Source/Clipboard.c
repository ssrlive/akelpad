#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include string functions
#define xmemcpy
#define xmemset
#define xstrlenA
#define xstrlenW
#define xstrcpynA
#define xstrcpynW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define AppendMenuWide
#define CreateDialogWide
#define DialogBoxWide
#define GetWindowTextWide
#define SetDlgItemTextWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_SETUP          1
#define STRID_SEPARATOR      2
#define STRID_LEGEND         3
#define STRID_GLOBALHOTKEYS  4
#define STRID_DELIMSKIP      5
#define STRID_DELIMASTAB     6
#define STRID_DELIMASIS      7
#define STRID_PLUGIN         8
#define STRID_OK             9
#define STRID_CANCEL         10

#define AKDLL_SETUP   (WM_USER + 100)

#define OF_CAPTURE_RECT         0x1
#define OF_CAPTURE_SETTINGS     0x2
#define OF_PASTESERIAL_SETTINGS 0x4

#define OF_CAPTURE     (OF_CAPTURE_RECT |\
                        OF_CAPTURE_SETTINGS)
#define OF_PASTESERIAL (OF_PASTESERIAL_SETTINGS)
#define OF_ALL         (OF_CAPTURE |\
                        OF_PASTESERIAL)

//DestroyDock type
#define DKT_DEFAULT        0x0
#define DKT_NOUNLOAD       0x1
#define DKT_ONMAINFINISH   0x2
#define DKT_KEEPAUTOLOAD   0x4

//Insert serial type
#define IT_DELIM_SKIP  1
#define IT_DELIM_ASTAB 2
#define IT_DELIM_ASIS  3

//Functions prototypes
void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow);
void DestroyDock(HWND hWndDock, DWORD dwType);
BOOL CALLBACK DockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CopySelection(HWND hWnd);
void SaveClipboard(UINT uFormat, wchar_t **wpData, char **pData);
void FreeClipboard(wchar_t **wpData, char **pData);
void SetClipboardFormat(UINT uFormat);
BOOL WaitForReleaseVkKeys(DWORD dwThreadCurrent, DWORD dwThreadTarget, DWORD dwTimeout);
BYTE GetHotkeyMod(DWORD dwHotkey);
BOOL EscapeStringToEscapeData(wchar_t *wpInput, wchar_t *wszOutput);
void EscapeDataToEscapeString(wchar_t *wpInput, wchar_t *wszOutput);
BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc);
void ShowStandardEditMenu(HWND hWnd, HMENU hMenu, BOOL bMouse);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();
void InitCapture();
void UninitCapture();
void InitPasteSerial();
void UninitPasteSerial();
void InitSelAutoCopy();
void UninitSelAutoCopy();

//Global variables
char szBuffer[BUFFER_SIZE];
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
PLUGINFUNCTION *pfCapture;
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hWndEdit;
HMENU hPopupEdit;
BOOL bOldWindows;
BOOL bAkelEdit;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
int nInitMain=0;
DWORD dwSaveFlags=0;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;
WNDPROCDATA *NewEditProcData=NULL;

//Capture
BOOL bInitCapture=FALSE;
wchar_t wszCaptureSeparator[MAX_PATH];
HWND hWndNextViewer=NULL;
HWND hWndCaptureDlg=NULL;
HWND hWndCaptureEdit=NULL;
RECT rcCaptureCurrentDialog={0};
RECT rcCaptureDockRect={0};
int nCaptureDockSide=DKS_BOTTOM;
DOCK *dkCaptureDlg=NULL;
BOOL bCaptureDockWaitResize=FALSE;

//PasteSerial
BOOL bInitPasteSerial=FALSE;
ATOM nHotkeyDelimSkipID=0;
ATOM nHotkeyDelimAsTabID=0;
ATOM nHotkeyDelimAsIsID=0;
DWORD dwHotkeyDelimSkip=MAKEWORD(VK_F9, HOTKEYF_CONTROL);
DWORD dwHotkeyDelimAsTab=MAKEWORD(VK_F10, HOTKEYF_CONTROL);
DWORD dwHotkeyDelimAsIs=MAKEWORD(VK_F11, HOTKEYF_CONTROL);
BOOL bEmulatePress=TRUE;

//SelAutoCopy
BOOL bInitSelAutoCopy=FALSE;
UINT cfSelAutoCopy=0;
char *pClipboard=NULL;
wchar_t *wpClipboard=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Clipboard";
}

//Plugin extern function
void __declspec(dllexport) Capture(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon)
    InitCommon(pd);
  else if (!bInitCapture)
    ReadOptions(OF_CAPTURE);

  if (bInitCapture)
  {
    DestroyDock(hWndCaptureDlg, DKT_KEEPAUTOLOAD);

    //Stay in memory and show as non-active
    pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitCapture();

    pfCapture=pd->lpPluginFunction;
    bCaptureDockWaitResize=pd->bOnStart;
    CreateDock(&hWndCaptureDlg, &dkCaptureDlg, !bCaptureDockWaitResize);

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) PasteSerial(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon)
    InitCommon(pd);
  else if (!bInitPasteSerial)
    ReadOptions(OF_PASTESERIAL);

  if (bInitPasteSerial)
  {
    UninitMain();
    UninitPasteSerial();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitPasteSerial();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) SelAutoCopy(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (bInitSelAutoCopy)
  {
    UninitMain();
    UninitSelAutoCopy();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitSelAutoCopy();

    if (!pd->bOnStart)
    {
      if (SendMessage(pd->hWndEdit, EM_SELECTIONTYPE, 0, 0) != SEL_EMPTY)
      {
        CopySelection(pd->hWndEdit);
      }
    }

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) Paste(PLUGINDATA *pd)
{
  EDITINFO ei;

  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)pd->hWndEdit, (LPARAM)&ei))
  {
    if (!ei.bReadOnly)
    {
      if (bInitSelAutoCopy && IsClipboardFormatAvailable(cfSelAutoCopy))
      {
        if (wpClipboard)
        {
          SendMessage(pd->hMainWnd, AKD_REPLACESELW, (WPARAM)pd->hWndEdit, (LPARAM)wpClipboard);
        }
        else if (pClipboard)
        {
          SendMessage(pd->hMainWnd, AKD_REPLACESELA, (WPARAM)pd->hWndEdit, (LPARAM)pClipboard);
        }
      }
      else SendMessage(pd->hMainWnd, AKD_PASTE, (WPARAM)pd->hWndEdit, 0);
    }
  }

  //If plugin already loaded, stay in memory
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
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

void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow)
{
  DOCK dk;

  *hWndDock=NULL;

  xmemset(&dk, 0, sizeof(DOCK));
  dk.dwFlags=DKF_DRAGDROP;
  dk.nSide=nCaptureDockSide;
  dk.rcSize=rcCaptureDockRect;

  if (*dkDock=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_ADD, (LPARAM)&dk))
  {
    *hWndDock=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_CAPTURE), hMainWnd, (DLGPROC)DockDlgProc);

    if (*hWndDock)
    {
      if (!(*dkDock)->rcSize.right || !(*dkDock)->rcSize.bottom)
      {
        GetWindowRect(*hWndDock, &(*dkDock)->rcSize);
        (*dkDock)->rcSize.right-=(*dkDock)->rcSize.left;
        (*dkDock)->rcSize.bottom-=(*dkDock)->rcSize.top;
        (*dkDock)->rcSize.left=0;
        (*dkDock)->rcSize.top=0;
      }
      (*dkDock)->hWnd=*hWndDock;
      SendMessage(hMainWnd, AKD_DOCK, DK_SUBCLASS, (LPARAM)*dkDock);

      if (bShow)
        SendMessage(hMainWnd, AKD_DOCK, DK_SHOW, (LPARAM)*dkDock);
      else
        (*dkDock)->dwFlags|=DKF_HIDDEN;
    }
  }
}

void DestroyDock(HWND hWndDock, DWORD dwType)
{
  SendMessage(hWndDock, WM_COMMAND, IDCANCEL, dwType);
}

BOOL CALLBACK DockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndTitleText;
  static HWND hWndTitleClose;
  static HMENU hMenuLabel;
  static RESIZEDIALOG rds[]={{&hWndTitleText,    RDS_SIZE|RDS_X, 0},
                             {&hWndTitleClose,   RDS_MOVE|RDS_X, 0},
                             {&hWndCaptureEdit,  RDS_SIZE|RDS_X, 0},
                             {&hWndCaptureEdit,  RDS_SIZE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    HFONT hFontEdit;

    hWndCaptureEdit=GetDlgItem(hDlg, IDC_CAPTURE);
    hWndTitleText=GetDlgItem(hDlg, IDC_TITLETEXT);
    hWndTitleClose=GetDlgItem(hDlg, IDC_CAPTURE_CLOSE);

    //SendMessage(hWndCaptureEdit, EM_SETUNDOLIMIT, 0, 0);
    hFontEdit=(HFONT)SendMessage(hMainWnd, AKD_GETFONT, (WPARAM)NULL, (LPARAM)NULL);
    SendMessage(hWndCaptureEdit, WM_SETFONT, (WPARAM)hFontEdit, FALSE);

    hMenuLabel=CreatePopupMenu();
    AppendMenuWide(hMenuLabel, MF_STRING, IDM_SETUP, GetLangStringW(wLangModule, STRID_SETUP));

    //Set dock title
    {
      BUTTONDRAW bd={BIF_CROSS|BIF_ETCHED};

      SetWindowTextWide(hWndTitleText, L"Clipboard::Capture");
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndTitleClose, (LPARAM)&bd);
    }

    //View clipboard
    hWndNextViewer=SetClipboardViewer(hMainWnd);
  }
  else if (uMsg == AKDLL_SETUP)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hDlg, (DLGPROC)SetupDlgProc);
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    POINT pt;
    RECT rc;

    GetCursorPos(&pt);
    GetWindowRect(hWndTitleText, &rc);

    if (PtInRect(&rc, pt))
    {
      PostMessage(hDlg, AKDLL_SETUP, 0, 0);
    }
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hDlg)
    {
      POINT pt;
      RECT rc;

      GetCursorPos(&pt);
      GetWindowRect(hWndTitleText, &rc);

      if (PtInRect(&rc, pt))
      {
        int nCmd;

        nCmd=TrackPopupMenu(hMenuLabel, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWndTitleText, NULL);

        if (nCmd == IDM_SETUP)
          PostMessage(hDlg, AKDLL_SETUP, 0, 0);
      }
    }
    else if ((HWND)wParam == hWndCaptureEdit)
    {
      ShowStandardEditMenu((HWND)wParam, hPopupEdit, lParam != -1);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_CAPTURE_CLOSE ||
        LOWORD(wParam) == IDCANCEL)
    {
      //Stop view clipboard
      ChangeClipboardChain(hMainWnd, hWndNextViewer);
      hWndNextViewer=NULL;

      //Save OF_CAPTURE_RECT
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      UninitMain();
      UninitCapture();
      SendMessage(hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkCaptureDlg);
      dkCaptureDlg=NULL;

      DestroyMenu(hMenuLabel);
      DestroyWindow(hWndCaptureDlg);
      hWndCaptureDlg=NULL;

      if (!(lParam & DKT_ONMAINFINISH))
      {
        SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
        SetFocus(hMainWnd);

        if (!(lParam & DKT_KEEPAUTOLOAD))
        {
          pfCapture->bAutoLoad=FALSE;
          SendMessage(hMainWnd, AKD_DLLSAVE, DLLSF_ONEXIT, 0);
        }
        if ((lParam & DKT_NOUNLOAD) || nInitMain)
        {
          pfCapture->bRunning=FALSE;
          SendMessage(hMainWnd, WM_COMMAND, 0, 0);
        }
        else SendMessage(hMainWnd, AKD_DLLUNLOAD, (WPARAM)hInstanceDLL, (LPARAM)NULL);
      }
    }
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], NULL, &rcCaptureCurrentDialog, 0, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      if (dkCaptureDlg) GetWindowSize(hWndTitleText, hDlg, &dkCaptureDlg->rcDragDrop);
  }

  return FALSE;
}

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndCaptureTitle;
  static HWND hWndSeparator;
  static HWND hWndPasteSerialTitle;
  static HWND hWndHotkeyDelimSkip;
  static HWND hWndHotkeyDelimAsTab;
  static HWND hWndHotkeyDelimAsIs;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndCaptureTitle=GetDlgItem(hDlg, IDC_SETUP_CAPTURE_TITLE);
    hWndSeparator=GetDlgItem(hDlg, IDC_SETUP_CAPTURE_SEPARATOR);
    hWndPasteSerialTitle=GetDlgItem(hDlg, IDC_SETUP_PASTESERIAL_TITLE);
    hWndHotkeyDelimSkip=GetDlgItem(hDlg, IDC_SETUP_PASTESERIAL_DELIMSKIP);
    hWndHotkeyDelimAsTab=GetDlgItem(hDlg, IDC_SETUP_PASTESERIAL_DELIMASTAB);
    hWndHotkeyDelimAsIs=GetDlgItem(hDlg, IDC_SETUP_PASTESERIAL_DELIMASIS);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_SETUP_CAPTURE_SEPARATOR_LABEL, GetLangStringW(wLangModule, STRID_SEPARATOR));
    SetDlgItemTextWide(hDlg, IDC_SETUP_CAPTURE_SEPARATOR_LEGEND, GetLangStringW(wLangModule, STRID_LEGEND));
    SetDlgItemTextWide(hDlg, IDC_SETUP_PASTESERIAL_GROUP, GetLangStringW(wLangModule, STRID_GLOBALHOTKEYS));
    SetDlgItemTextWide(hDlg, IDC_SETUP_PASTESERIAL_DELIMSKIP_LABEL, GetLangStringW(wLangModule, STRID_DELIMSKIP));
    SetDlgItemTextWide(hDlg, IDC_SETUP_PASTESERIAL_DELIMASTAB_LABEL, GetLangStringW(wLangModule, STRID_DELIMASTAB));
    SetDlgItemTextWide(hDlg, IDC_SETUP_PASTESERIAL_DELIMASIS_LABEL, GetLangStringW(wLangModule, STRID_DELIMASIS));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    if (!bInitCapture) EnableWindow(hWndCaptureTitle, FALSE);
    if (!bInitPasteSerial) EnableWindow(hWndPasteSerialTitle, FALSE);
    SendMessage(hWndSeparator, EM_LIMITTEXT, MAX_PATH, 0);
    EscapeDataToEscapeString(wszCaptureSeparator, wszBuffer);
    SetWindowTextWide(hWndSeparator, wszBuffer);

    SendMessage(hWndHotkeyDelimSkip, HKM_SETHOTKEY, dwHotkeyDelimSkip, 0);
    SendMessage(hWndHotkeyDelimAsTab, HKM_SETHOTKEY, dwHotkeyDelimAsTab, 0);
    SendMessage(hWndHotkeyDelimAsIs, HKM_SETHOTKEY, dwHotkeyDelimAsIs, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      GetWindowTextWide(hWndSeparator, wszBuffer, MAX_PATH);
      EscapeStringToEscapeData(wszBuffer, wszCaptureSeparator);

      dwHotkeyDelimSkip=(DWORD)SendMessage(hWndHotkeyDelimSkip, HKM_GETHOTKEY, 0, 0);
      dwHotkeyDelimAsTab=(DWORD)SendMessage(hWndHotkeyDelimAsTab, HKM_GETHOTKEY, 0, 0);
      dwHotkeyDelimAsIs=(DWORD)SendMessage(hWndHotkeyDelimAsIs, HKM_GETHOTKEY, 0, 0);

      if (nInitMain)
      {
        if (bInitCapture)
        {
          UninitCapture();
          InitCapture();
        }
        if (bInitPasteSerial)
        {
          UninitPasteSerial();
          InitPasteSerial();
        }
        if (bInitSelAutoCopy)
        {
          UninitSelAutoCopy();
          InitSelAutoCopy();
        }
      }
      dwSaveFlags|=OF_CAPTURE_SETTINGS|OF_PASTESERIAL_SETTINGS;

      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

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
  //Capture
  if (uMsg == WM_CHANGECBCHAIN)
  {
    if (bInitCapture)
    {
      if (hWndNextViewer == (HWND)wParam)
        hWndNextViewer=(HWND)lParam;
      else if (hWndNextViewer && IsWindow(hWndNextViewer))
        SendMessage(hWndNextViewer, uMsg, wParam, lParam);
    }
  }
  else if (uMsg == WM_DRAWCLIPBOARD)
  {
    if (bInitCapture)
    {
      if (hWndCaptureDlg)
      {
        if (GetFocus() != hWndCaptureEdit)
        {
          if (!cfSelAutoCopy || !IsClipboardFormatAvailable(cfSelAutoCopy))
          {
            SendMessage(hWndCaptureEdit, EM_SETSEL, (WPARAM)0x7FFFFFFF, (LPARAM)0x7FFFFFFF);
            SendMessage(hMainWnd, AKD_PASTE, (WPARAM)hWndCaptureEdit, 0);
            SendMessage(hWndCaptureEdit, EM_SETSEL, (WPARAM)0x7FFFFFFF, (LPARAM)0x7FFFFFFF);
            SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndCaptureEdit, (LPARAM)wszCaptureSeparator);
          }
        }
      }
      if (hWndNextViewer && IsWindow(hWndNextViewer))
        SendMessage(hWndNextViewer, uMsg, wParam, lParam);
    }
  }

  //PasteSerial
  else if (uMsg == WM_HOTKEY)
  {
    if (bInitPasteSerial)
    {
      char *szSerial=NULL;
      char *pSerial=NULL;
      HWND hWndTargetForeground;
      HWND hWndTargetFocus;
      DWORD dwTargetForeground;
      DWORD dwThreadCurrent;
      WORD wVk;
      int nInsertType=0;

      if (GetHotkeyMod(dwHotkeyDelimSkip) == LOWORD(lParam) && LOBYTE(dwHotkeyDelimSkip) == HIWORD(lParam))
        nInsertType=IT_DELIM_SKIP;
      else if (GetHotkeyMod(dwHotkeyDelimAsTab) == LOWORD(lParam) && LOBYTE(dwHotkeyDelimAsTab) == HIWORD(lParam))
        nInsertType=IT_DELIM_ASTAB;
      else if (GetHotkeyMod(dwHotkeyDelimAsIs) == LOWORD(lParam) && LOBYTE(dwHotkeyDelimAsIs) == HIWORD(lParam))
        nInsertType=IT_DELIM_ASIS;

      if (nInsertType)
      {
        hWndTargetForeground=GetForegroundWindow();
        dwTargetForeground=GetWindowThreadProcessId(hWndTargetForeground, NULL);
        dwThreadCurrent=GetCurrentThreadId();

        if (dwThreadCurrent != dwTargetForeground)
        {
          if (AttachThreadInput(dwThreadCurrent, dwTargetForeground, TRUE))
          {
            if (WaitForReleaseVkKeys(dwThreadCurrent, dwTargetForeground, 5000))
            {
              if (hWndTargetFocus=GetFocus())
              {
                //Get clipboard text
                {
                  HGLOBAL hData;
                  LPVOID pData;
                  INT_PTR nDataLen;
  
                  if (OpenClipboard(NULL))
                  {
                    if (hData=GetClipboardData(CF_TEXT))
                    {
                      if (pData=GlobalLock(hData))
                      {
                        nDataLen=xstrlenA((char *)pData) + 1;
                        if (szSerial=(char *)GlobalAlloc(GPTR, nDataLen))
                          xstrcpynA(szSerial, (char *)pData, nDataLen);
                        GlobalUnlock(hData);
                      }
                    }
                    CloseClipboard();
                  }
                }
  
                //Post serial
                if (szSerial)
                {
                  pSerial=szSerial;
  
                  while (*pSerial && *pSerial != '\n')
                  {
                    if (nInsertType != IT_DELIM_ASIS)
                    {
                      if (*pSerial == '-' || *pSerial == ' ' || *pSerial == '\t')
                      {
                        if (nInsertType == IT_DELIM_ASTAB)
                        {
                          if (GetFocus() == hWndTargetFocus)
                          {
                            if (bEmulatePress)
                            {
                              //WaitForReleaseVkKeys(dwThreadCurrent, dwTargetForeground, INFINITE);
                              keybd_event(VK_TAB, 0, KEYEVENTF_EXTENDEDKEY, 0);
                              keybd_event(VK_TAB, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
                              //WaitForReleaseVkKeys(dwThreadCurrent, dwTargetForeground, INFINITE);
                            }
                            else SendMessage(hWndTargetFocus, WM_CHAR, '\t', 1);
                          }
                        }
                        while (*++pSerial == '-' || *pSerial == ' ' || *pSerial == '\t');
                        hWndTargetFocus=GetFocus();
                        continue;
                      }
                    }
  
                    if (bEmulatePress)
                    {
                      //WaitForReleaseVkKeys(dwThreadCurrent, dwTargetForeground, INFINITE);
                      wVk=VkKeyScan(*pSerial);
                      if (HIBYTE(wVk) & 1)
                        keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY, 0);
                      if (HIBYTE(wVk) & 2)
                        keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY, 0);
                      if (HIBYTE(wVk) & 4)
                        keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY, 0);
                      keybd_event(LOBYTE(wVk), 0, 0, 0);
                      keybd_event(LOBYTE(wVk), 0, KEYEVENTF_KEYUP, 0);
                      if (HIBYTE(wVk) & 4)
                        keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
                      if (HIBYTE(wVk) & 2)
                        keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
                      if (HIBYTE(wVk) & 1)
                        keybd_event(VK_SHIFT, 0, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP, 0);
                      //WaitForReleaseVkKeys(dwThreadCurrent, dwTargetForeground, INFINITE);
                    }
                    else SendMessage(hWndTargetFocus, WM_CHAR, *pSerial, 1);
  
                    ++pSerial;
                  }
                  GlobalFree((HGLOBAL)szSerial);
                  szSerial=NULL;
                }
              }
            }
            AttachThreadInput(dwThreadCurrent, dwTargetForeground, FALSE);
          }
        }
      }
    }
  }

  //Notifications
  else if (uMsg == AKDN_DOCK_RESIZE)
  {
    if (bInitCapture)
    {
      if (((DOCK *)wParam)->hWnd == dkCaptureDlg->hWnd)
        dwSaveFlags|=OF_CAPTURE_RECT;
    }
  }
  else if (uMsg == AKDN_SIZE_ONSTART)
  {
    if (bInitCapture)
    {
      if (bCaptureDockWaitResize)
      {
        bCaptureDockWaitResize=FALSE;
        ShowWindow(hWndCaptureDlg, SW_SHOW);
        dkCaptureDlg->dwFlags&=~DKF_HIDDEN;
      }
    }
  }
  if (uMsg == AKDN_MAIN_ONSTART_FINISH)
  {
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

    if (hWndCaptureDlg)
    {
      DestroyDock(hWndCaptureDlg, DKT_ONMAINFINISH);
    }
    if (bInitPasteSerial)
    {
      UninitMain();
      UninitPasteSerial();
    }
    if (bInitSelAutoCopy)
    {
      UninitMain();
      UninitSelAutoCopy();
    }
    return FALSE;
  }

  //Special messages
  {
    LRESULT lResult;

    if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
      return lResult;
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //Special messages
  {
    LRESULT lResult;

    if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
      return lResult;
  }

  //Call next procedure
  return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_LBUTTONUP)
  {
    if (bInitSelAutoCopy)
    {
      if (SendMessage(hWnd, EM_SELECTIONTYPE, 0, 0) != SEL_EMPTY)
      {
        if (SendMessage(hWnd, AEM_GETMOUSESTATE, AEMS_CAPTURE, 0) & AEMSC_MOUSEMOVE)
        {
          CopySelection(hWnd);
        }
      }
    }
  }

  //Call next procedure
  return NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT)
    {
      if (((NMHDR *)lParam)->code == EN_SELCHANGE)
      {
        if (bInitSelAutoCopy)
        {
          SELCHANGE64 *sc=(SELCHANGE64 *)lParam;

          if (sc->chrg64.cpMin != sc->chrg64.cpMax)
          {
            if (GetCapture() != sc->nmhdr.hwndFrom)
            {
              CopySelection(sc->nmhdr.hwndFrom);
            }
          }
        }
      }
    }
  }
  return 0;
}

void CopySelection(HWND hWnd)
{
  if (!bAkelEdit)
  {
    /*RichEdit compatibility dropped.
    TEXTRANGE64A txtrng;
    char szChar[3];

    SendMessage(hWnd, EM_EXSETSEL64, 0, (LPARAM)&txtrng.chrg);

    if (txtrng.chrg.cpMax - txtrng.chrg.cpMin == 1)
    {
      txtrng.lpstrText=szChar;

      if (SendMessageA(hWnd, EM_GETTEXTRANGE64A, 0, (LPARAM)&txtrng))
      {
        if (szChar[0] != '\0')
        {
          SaveClipboard(cfSelAutoCopy, &wpClipboard, &pClipboard);
          SendMessage(hMainWnd, AKD_COPY, (WPARAM)hWnd, 0);
          SetClipboardFormat(cfSelAutoCopy);
        }
      }
    }
    */
  }
  else
  {
    SaveClipboard(cfSelAutoCopy, &wpClipboard, &pClipboard);
    SendMessage(hMainWnd, AKD_COPY, (WPARAM)hWnd, 0);
    SetClipboardFormat(cfSelAutoCopy);
  }
}

void SaveClipboard(UINT uFormat, wchar_t **wpData, char **pData)
{
  HGLOBAL hDataSource;
  LPVOID pDataSource;
  INT_PTR nLen;

  if (OpenClipboard(NULL))
  {
    if (!IsClipboardFormatAvailable(uFormat))
    {
      if (*wpData)
      {
        GlobalFree((HGLOBAL)*wpData);
        *wpData=NULL;
      }
      if (*pData)
      {
        GlobalFree((HGLOBAL)*pData);
        *pData=NULL;
      }
      if (hDataSource=GetClipboardData(CF_UNICODETEXT))
      {
        if (pDataSource=GlobalLock(hDataSource))
        {
          nLen=xstrlenW((wchar_t *)pDataSource) + 1;
          if (*wpData=(wchar_t *)GlobalAlloc(GMEM_FIXED, nLen * sizeof(wchar_t)))
            xstrcpynW(*wpData, pDataSource, nLen);
          GlobalUnlock(hDataSource);
        }
      }
      else if (hDataSource=GetClipboardData(CF_TEXT))
      {
        if (pDataSource=GlobalLock(hDataSource))
        {
          nLen=xstrlenA((char *)pDataSource) + 1;
          if (*pData=(char *)GlobalAlloc(GMEM_FIXED, nLen))
            xstrcpynA(*pData, pDataSource, nLen);
          GlobalUnlock(hDataSource);
        }
      }
    }
    CloseClipboard();
  }
}

void FreeClipboard(wchar_t **wpData, char **pData)
{
  //Free memory
  if (*wpData)
  {
    GlobalFree((HGLOBAL)*wpData);
    *wpData=NULL;
  }
  if (*pData)
  {
    GlobalFree((HGLOBAL)*pData);
    *pData=NULL;
  }
}

void SetClipboardFormat(UINT uFormat)
{
  if (!IsClipboardFormatAvailable(uFormat))
  {
    if (OpenClipboard(NULL))
    {
      SetClipboardData(uFormat, NULL);
      CloseClipboard();
    }
  }
}

BOOL WaitForReleaseVkKeys(DWORD dwThreadCurrent, DWORD dwThreadTarget, DWORD dwTimeout)
{
  DWORD dwStart=GetTickCount();
  BYTE lpKeyState[256];
  int i;

  Loop:
  if (dwTimeout == INFINITE || GetTickCount() - dwStart < dwTimeout)
  {
    AttachThreadInput(dwThreadCurrent, dwThreadTarget, FALSE);

    if (AttachThreadInput(dwThreadCurrent, dwThreadTarget, TRUE))
    {
      Sleep(0);

      if (GetKeyboardState(lpKeyState))
      {
        for (i=0; i < 256; ++i)
        {
          if (lpKeyState[i] & 0x80)
            goto Loop;
        }
      }
    }
    return TRUE;
  }
  return FALSE;
}

BYTE GetHotkeyMod(DWORD dwHotkey)
{
  //Convert modifier from HKM_GETHOTKEY to WM_HOTKEY.
  BYTE nHotkeyMod=HIBYTE(dwHotkey);
  BYTE nResultMod=0;

  if (nHotkeyMod & HOTKEYF_SHIFT)
    nResultMod|=MOD_SHIFT;
  if (nHotkeyMod & HOTKEYF_CONTROL)
    nResultMod|=MOD_CONTROL;
  if (nHotkeyMod & HOTKEYF_ALT)
    nResultMod|=MOD_ALT;
  return nResultMod;
}

BOOL EscapeStringToEscapeData(wchar_t *wpInput, wchar_t *wszOutput)
{
  wchar_t *a=wpInput;
  wchar_t *b=wszOutput;

  for (; *a; ++a, ++b)
  {
    if (*a == '\\')
    {
      if (*++a == '\\') *b='\\';
      else if (*a == 'r') *b='\r';
      else if (*a == 'n') *b='\n';
      else if (*a == 't') *b='\t';
      else
      {
        *b='\0';
        return FALSE;
      }
    }
    else *b=*a;
  }
  *b='\0';
  return TRUE;
}

void EscapeDataToEscapeString(wchar_t *wpInput, wchar_t *wszOutput)
{
  wchar_t *a=wpInput;
  wchar_t *b=wszOutput;

  for (; *a; ++a, ++b)
  {
    if (*a == '\t') *b='\\', *++b='t';
    else if (*a == '\r') *b='\\', *++b='r';
    else if (*a == '\n') *b='\\', *++b='n';
    else if (*a == '\\') *b='\\', *++b='\\';
    else *b=*a;
  }
  *b='\0';
}

BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetWindowRect(hWnd, rc))
  {
    rc->right-=rc->left;
    rc->bottom-=rc->top;

    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

void ShowStandardEditMenu(HWND hWnd, HMENU hMenu, BOOL bMouse)
{
  POINT pt;
  CHARRANGE64 cr;
  int nCmd;

  if (!bMouse && SendMessage(hWnd, AEM_GETCARETPOS, (WPARAM)&pt, 0))
  {
    pt.y+=(int)SendMessage(hWnd, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
    ClientToScreen(hWnd, &pt);
  }
  else GetCursorPos(&pt);

  SendMessage(hWnd, EM_EXGETSEL64, 0, (LPARAM)&cr);
  EnableMenuItem(hMenu, IDM_EDIT_UNDO, SendMessage(hWnd, EM_CANUNDO, 0, 0)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_REDO, SendMessage(hWnd, EM_CANREDO, 0, 0)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_PASTE, SendMessage(hWnd, EM_CANPASTE, 0, 0)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_CUT, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_CLEAR, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);
  EnableMenuItem(hMenu, IDM_EDIT_COPY, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);

  nCmd=TrackPopupMenu(hMenu, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

  if (nCmd == IDM_EDIT_UNDO)
    SendMessage(hWnd, EM_UNDO, 0, 0);
  else if (nCmd == IDM_EDIT_REDO)
    SendMessage(hWnd, EM_REDO, 0, 0);
  else if (nCmd == IDM_EDIT_CUT)
    SendMessage(hWnd, WM_CUT, 0, 0);
  else if (nCmd == IDM_EDIT_COPY)
    SendMessage(hWnd, WM_COPY, 0, 0);
  else if (nCmd == IDM_EDIT_PASTE)
    SendMessage(hWnd, WM_PASTE, 0, 0);
  else if (nCmd == IDM_EDIT_CLEAR)
    SendMessage(hWnd, WM_CLEAR, 0, 0);
  else if (nCmd == IDM_EDIT_SELECTALL)
    SendMessage(hWnd, EM_SETSEL, 0, -1);
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
    if (dwFlags & OF_CAPTURE_RECT)
    {
      WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&nCaptureDockSide, sizeof(DWORD));
      WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&rcCaptureDockRect, sizeof(RECT));
    }
    if (dwFlags & OF_CAPTURE_SETTINGS)
    {
      WideOption(hOptions, L"Separator", PO_BINARY, (LPBYTE)wszCaptureSeparator, MAX_PATH * sizeof(wchar_t));
    }
    if (dwFlags & OF_PASTESERIAL_SETTINGS)
    {
      WideOption(hOptions, L"HotkeyDelimSkip", PO_DWORD, (LPBYTE)&dwHotkeyDelimSkip, sizeof(DWORD));
      WideOption(hOptions, L"HotkeyDelimAsTab", PO_DWORD, (LPBYTE)&dwHotkeyDelimAsTab, sizeof(DWORD));
      WideOption(hOptions, L"HotkeyDelimAsIs", PO_DWORD, (LPBYTE)&dwHotkeyDelimAsIs, sizeof(DWORD));
    }

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_CAPTURE_RECT)
    {
      WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&dkCaptureDlg->nSide, sizeof(DWORD));
      WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&dkCaptureDlg->rcSize, sizeof(RECT));
    }
    if (dwFlags & OF_CAPTURE_SETTINGS)
    {
      WideOption(hOptions, L"Separator", PO_BINARY, (LPBYTE)wszCaptureSeparator, ((int)xstrlenW(wszCaptureSeparator) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_PASTESERIAL_SETTINGS)
    {
      WideOption(hOptions, L"HotkeyDelimSkip", PO_DWORD, (LPBYTE)&dwHotkeyDelimSkip, sizeof(DWORD));
      WideOption(hOptions, L"HotkeyDelimAsTab", PO_DWORD, (LPBYTE)&dwHotkeyDelimAsTab, sizeof(DWORD));
      WideOption(hOptions, L"HotkeyDelimAsIs", PO_DWORD, (LPBYTE)&dwHotkeyDelimAsIs, sizeof(DWORD));
    }

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
    if (nStringID == STRID_SETUP)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_SEPARATOR)
      return L"\x0420\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x0435\x043B\x044C\x003A";
    if (nStringID == STRID_LEGEND)
      return L" \\\\ - \x043E\x0431\x0440\x0430\x0442\x043D\x044B\x0439\x0020\x0441\x043B\x044D\x0448\n \\n - \x043A\x043E\x043D\x0435\x0446\x0020\x0441\x0442\x0440\x043E\x043A\x0438\n \\t - \x0437\x043D\x0430\x043A\x0020\x0442\x0430\x0431\x0443\x043B\x044F\x0446\x0438\x0438";
    if (nStringID == STRID_GLOBALHOTKEYS)
      return L"\x0413\x043B\x043E\x0431\x0430\x043B\x044C\x043D\x044B\x0435\x0020\x0433\x043E\x0440\x044F\x0447\x0438\x0435\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0438";
    if (nStringID == STRID_DELIMSKIP)
      return L"\x041F\x0440\x043E\x043F\x0443\x0441\x043A\x0430\x0442\x044C\x0020\x0440\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x0435\x043B\x0438";
    if (nStringID == STRID_DELIMASTAB)
      return L"\x0417\x0430\x043C\x0435\x043D\x044F\x0442\x044C\x0020\x0440\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x0435\x043B\x0438\x0020\x0442\x0430\x0431\x043E\x043C";
    if (nStringID == STRID_DELIMASIS)
      return L"\x0412\x0441\x0442\x0430\x0432\x0438\x0442\x044C\x0020\x043A\x0430\x043A\x0020\x0435\x0441\x0442\x044C";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_SETUP)
      return L"Settings...";
    if (nStringID == STRID_SEPARATOR)
      return L"Separator:";
    if (nStringID == STRID_LEGEND)
      return L" \\\\ - backslash\n \\n - line feed\n \\t - tabulation";
    if (nStringID == STRID_GLOBALHOTKEYS)
      return L"Global hotkeys";
    if (nStringID == STRID_DELIMSKIP)
      return L"Skip delimiters";
    if (nStringID == STRID_DELIMASTAB)
      return L"Replace delimiter with tab";
    if (nStringID == STRID_DELIMASIS)
      return L"Paste as is";
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
  hWndEdit=pd->hWndEdit;
  bOldWindows=pd->bOldWindows;
  bAkelEdit=pd->bAkelEdit;
  nMDI=pd->nMDI;
  wLangModule=PRIMARYLANGID(pd->wLangModule);
  hPopupEdit=GetSubMenu(pd->hPopupMenu, MENU_POPUP_EDIT);

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
  xstrcpynW(wszCaptureSeparator, L"\n------------------------------------------------------------\n", MAX_PATH);
  ReadOptions(OF_ALL);
}

void InitMain()
{
  if (!nInitMain++)
  {
    //SubClass
    NewMainProcData=NULL;
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

    if (nMDI == WMD_MDI)
    {
      NewFrameProcData=NULL;
      SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewFrameProc, (LPARAM)&NewFrameProcData);
    }

    NewEditProcData=NULL;
    SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NewEditProc, (LPARAM)&NewEditProcData);
  }
}

void UninitMain()
{
  if (!--nInitMain)
  {
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
    if (NewEditProcData)
    {
      SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&NewEditProcData);
      NewEditProcData=NULL;
    }
  }
}

void InitCapture()
{
  bInitCapture=TRUE;
}

void UninitCapture()
{
  bInitCapture=FALSE;
}

void InitPasteSerial()
{
  bInitPasteSerial=TRUE;

  if (nHotkeyDelimSkipID=GlobalAddAtomA("AkelPad::PasteSerial::DelimSkip"))
    RegisterHotKey(hMainWnd, nHotkeyDelimSkipID, GetHotkeyMod(dwHotkeyDelimSkip), LOBYTE(dwHotkeyDelimSkip));
  if (nHotkeyDelimAsTabID=GlobalAddAtomA("AkelPad::PasteSerial::DelimAsTab"))
    RegisterHotKey(hMainWnd, nHotkeyDelimAsTabID, GetHotkeyMod(dwHotkeyDelimAsTab), LOBYTE(dwHotkeyDelimAsTab));
  if (nHotkeyDelimAsIsID=GlobalAddAtomA("AkelPad::PasteSerial::DelimAsIs"))
    RegisterHotKey(hMainWnd, nHotkeyDelimAsIsID, GetHotkeyMod(dwHotkeyDelimAsIs), LOBYTE(dwHotkeyDelimAsIs));
}

void UninitPasteSerial()
{
  bInitPasteSerial=FALSE;

  if (nHotkeyDelimSkipID)
  {
    if (UnregisterHotKey(hMainWnd, nHotkeyDelimSkipID))
    {
      GlobalDeleteAtom(nHotkeyDelimSkipID);
      nHotkeyDelimSkipID=0;
    }
  }
  if (nHotkeyDelimAsTabID)
  {
    if (UnregisterHotKey(hMainWnd, nHotkeyDelimAsTabID))
    {
      GlobalDeleteAtom(nHotkeyDelimAsTabID);
      nHotkeyDelimAsTabID=0;
    }
  }
  if (nHotkeyDelimAsIsID)
  {
    if (UnregisterHotKey(hMainWnd, nHotkeyDelimAsIsID))
    {
      GlobalDeleteAtom(nHotkeyDelimAsIsID);
      nHotkeyDelimAsIsID=0;
    }
  }
}

void InitSelAutoCopy()
{
  bInitSelAutoCopy=TRUE;

  //Register clipboard format
  cfSelAutoCopy=RegisterClipboardFormatA("AkelPad::SelAutoCopy");
}

void UninitSelAutoCopy()
{
  bInitSelAutoCopy=FALSE;

  FreeClipboard(&wpClipboard, &pClipboard);
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
