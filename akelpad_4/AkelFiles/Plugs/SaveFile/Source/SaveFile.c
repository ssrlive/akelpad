#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


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
#define CallWindowProcWide
#define ComboBox_GetLBTextWide
#define CreateDirectoryWide
#define DeleteFileWide
#define DialogBoxWide
#define ExpandEnvironmentStringsWide
#define FileExistsWide
#define GetDlgItemTextWide
#define GetFileAttributesWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define SHBrowseForFolderWide
#define SHGetPathFromIDListWide
#include "WideFunc.h"

//Defines
#define STRID_SAVEMOMENT   1
#define STRID_SAVEINTERVAL 2
#define STRID_MIN          3
#define STRID_SAVEFOCUS    4
#define STRID_SAVEFRAME    5
#define STRID_SAVEMETHOD   6
#define STRID_SAVESIMPLE   7
#define STRID_SAVENEAR     8
#define STRID_SAVEDIR      9
#define STRID_SAVEDIRVARS  10
#define STRID_SAVENOBOM    11
#define STRID_FORCENOBOM   12
#define STRID_DLGUNCHECK   13
#define STRID_PLUGIN       14
#define STRID_OK           15
#define STRID_CANCEL       16

#define OF_AUTOSAVE       0x1
#define OF_SAVENOBOM      0x2
#define OF_ALL         (OF_AUTOSAVE |\
                        OF_SAVENOBOM)

#define BUFFER_SIZE      1024

//Save moment
#define SMOM_TIME    0x1
#define SMOM_FOCUS   0x2
#define SMOM_FRAME   0x4

//Save method
#define SMET_SIMPLE  0x1
#define SMET_NEAR    0x2
#define SMET_DIR     0x4

#ifndef BIF_NEWDIALOGSTYLE
  #define BIF_NEWDIALOGSTYLE 0x0040
#endif
#ifndef IDC_OFN_AUTODETECT
  #define IDC_OFN_AUTODETECT 2501
#endif
#ifndef IDC_OFN_CODEPAGE
  #define IDC_OFN_CODEPAGE 2502
#endif

#define CP_UNICODE_UTF16LE  1200
#define CP_UNICODE_UTF16BE  1201
#define CP_UNICODE_UTF32LE  12000
#define CP_UNICODE_UTF32BE  12001
#define CP_UNICODE_UTF7     65000
#define CP_UNICODE_UTF8     65001

#define NOBOM_UTF16LE    0x00000001
#define NOBOM_UTF16BE    0x00000002
#define NOBOM_UTF8       0x00000004
#define NOBOM_UTF32LE    0x00000008
#define NOBOM_UTF32BE    0x00000010
#define NOBOM_DLGUNCHECK 0x00000100

//Functions prototypes
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SaveFileProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void DoAutoSave(int nSaveMoment);
BOOL IsBackupNeeded(FRAMEDATA *lpFrame);
BOOL MakeBackupFile(HWND hWndEdit);
void RemoveBackupFile(HWND hWndEdit, const wchar_t *wpEditFile);
int GetComboboxCodepage(HWND hWnd);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();
void InitAutoSave();
void UninitAutoSave();
void InitSaveNoBOM();
void UninitSaveNoBOM();

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
wchar_t wszExeDir[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hMdiClient;
BOOL bOldWindows;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
int nInitMain=0;
BOOL bInitAutoSave=FALSE;
BOOL bInitSaveNoBOM=FALSE;
wchar_t wszSaveDir[MAX_PATH]=L"";
wchar_t wszSaveDirExp[MAX_PATH]=L"";
UINT_PTR dwSaveTimer=0;
DWORD dwSaveMoment=SMOM_TIME;
DWORD dwSaveInterval=5;
DWORD dwSaveMethod=SMET_SIMPLE;
int nAppActive=TRUE;
DWORD dwSaveNoBomSettings=NOBOM_UTF8;
WNDPROC lpOldSaveFileProc;
WNDPROCDATA *NewMainProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 3, 0);
  pv->pPluginName="SaveFile";
}

//Plugin extern function
void __declspec(dllexport) AutoSave(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon)
    InitCommon(pd);
  else if (!bInitAutoSave)
    ReadOptions(OF_AUTOSAVE);

  if (bInitAutoSave)
  {
    UninitMain();
    UninitAutoSave();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitAutoSave();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) SaveNoBOM(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon)
    InitCommon(pd);
  else if (!bInitSaveNoBOM)
    ReadOptions(OF_SAVENOBOM);

  if (bInitSaveNoBOM)
  {
    UninitMain();
    UninitSaveNoBOM();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitSaveNoBOM();

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

  DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETTINGS), hMainWnd, (DLGPROC)SettingsDlgProc);

  //If plugin already loaded, stay in memory, but show as non-active
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static wchar_t wszFileNameNew[MAX_PATH];
  static wchar_t wszFileNameOld[MAX_PATH];
  static BOOL bRemoveBackupCheck;

  if (uMsg == AKDN_INITDIALOGBEGIN)
  {
    if (bInitSaveNoBOM && (dwSaveNoBomSettings & NOBOM_DLGUNCHECK))
    {
      if (wParam == IDT_SAVEFILE)
      {
        NINITDIALOG *nid=(NINITDIALOG *)lParam;

        lpOldSaveFileProc=(WNDPROC)GetWindowLongPtrWide(nid->hWnd, GWLP_WNDPROC);
        SetWindowLongPtrWide(nid->hWnd, GWLP_WNDPROC, (UINT_PTR)SaveFileProc);
      }
    }
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_START)
  {
    NSAVEDOCUMENT *nsd=(NSAVEDOCUMENT *)lParam;

    if (bInitSaveNoBOM && !(dwSaveNoBomSettings & NOBOM_DLGUNCHECK))
    {
      if ((*nsd->nCodePage == CP_UNICODE_UTF8 && (dwSaveNoBomSettings & NOBOM_UTF8)) ||
          (*nsd->nCodePage == CP_UNICODE_UTF16LE && (dwSaveNoBomSettings & NOBOM_UTF16LE)) ||
          (*nsd->nCodePage == CP_UNICODE_UTF16BE && (dwSaveNoBomSettings & NOBOM_UTF16BE)) ||
          (*nsd->nCodePage == CP_UNICODE_UTF32LE && (dwSaveNoBomSettings & NOBOM_UTF32LE)) ||
          (*nsd->nCodePage == CP_UNICODE_UTF32BE && (dwSaveNoBomSettings & NOBOM_UTF32BE)))
      {
        *nsd->bBOM=FALSE;
      }
    }

    if (bInitAutoSave)
    {
      if (nsd->dwFlags & SD_UPDATE)
      {
        EDITINFO ei;

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, wParam, (LPARAM)&ei))
        {
          xstrcpynW(wszFileNameNew, nsd->wszFile, MAX_PATH);
          xstrcpynW(wszFileNameOld, ei.wszFile, MAX_PATH);
          bRemoveBackupCheck=TRUE;
        }
      }
    }
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_FINISH)
  {
    if (bInitAutoSave)
    {
      if (bRemoveBackupCheck)
      {
        if (lParam == ESD_SUCCESS)
        {
          if (xstrcmpiW(wszFileNameNew, wszFileNameOld))
            RemoveBackupFile((HWND)wParam, wszFileNameOld);
        }
        bRemoveBackupCheck=FALSE;
      }
    }
  }
  else if (uMsg == AKDN_EDIT_ONFINISH ||
           uMsg == AKDN_EDIT_ONCLOSE)
  {
    if (bInitAutoSave)
    {
      EDITINFO ei;

      if (SendMessage(hMainWnd, AKD_GETEDITINFO, wParam, (LPARAM)&ei))
      {
        if (!ei.bModified)
          RemoveBackupFile((HWND)wParam, ei.wszFile);
      }
    }
  }
  else if (uMsg == AKDN_FRAME_DEACTIVATE)
  {
    if (bInitAutoSave)
    {
      if (dwSaveMoment & SMOM_FRAME)
      {
        if (lParam)
        {
          DoAutoSave(SMOM_FRAME);
        }
      }
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    if (bInitAutoSave)
    {
      if (dwSaveMoment & SMOM_FOCUS)
      {
        nAppActive=-1;
      }
    }
  }
  else if (uMsg == WM_ACTIVATEAPP)
  {
    if (bInitAutoSave)
    {
      if (dwSaveMoment & SMOM_FOCUS)
      {
        if (nAppActive != -1)
        {
          if (wParam == TRUE)
          {
            if (!nAppActive)
              nAppActive=TRUE;
          }
          else
          {
            if (nAppActive)
            {
              nAppActive=FALSE;
              DoAutoSave(SMOM_FOCUS);
            }
          }
        }
      }
    }
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK SaveFileProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OFN_CODEPAGE && HIWORD(wParam) == CBN_SELCHANGE)
    {
      HWND hWndCodePage=GetDlgItem(hWnd, IDC_OFN_CODEPAGE);
      HWND hWndAutodetect=GetDlgItem(hWnd, IDC_OFN_AUTODETECT);
      int nCodePage=GetComboboxCodepage(hWndCodePage);
      BOOL bBOM=-1;
      LRESULT lResult;

      lResult=CallWindowProcWide(lpOldSaveFileProc, hWnd, uMsg, wParam, lParam);

      if ((nCodePage == CP_UNICODE_UTF8 && (dwSaveNoBomSettings & NOBOM_UTF8)) ||
          (nCodePage == CP_UNICODE_UTF16LE && (dwSaveNoBomSettings & NOBOM_UTF16LE)) ||
          (nCodePage == CP_UNICODE_UTF16BE && (dwSaveNoBomSettings & NOBOM_UTF16BE)) ||
          (nCodePage == CP_UNICODE_UTF32LE && (dwSaveNoBomSettings & NOBOM_UTF32LE)) ||
          (nCodePage == CP_UNICODE_UTF32BE && (dwSaveNoBomSettings & NOBOM_UTF32BE)))
      {
        bBOM=FALSE;
      }
      if (bBOM != -1)
        SendMessage(hWndAutodetect, BM_SETCHECK, (WPARAM)bBOM, 0);
      return lResult;
    }
  }
  return CallWindowProcWide(lpOldSaveFileProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndAutoSaveTitle;
  static HWND hWndSaveIntervalCheck;
  static HWND hWndSaveInterval;
  static HWND hWndSaveFocusCheck;
  static HWND hWndSaveFrameCheck;
  static HWND hWndSaveSimpleCheck;
  static HWND hWndSaveNearCheck;
  static HWND hWndSaveDirCheck;
  static HWND hWndSaveDir;
  static HWND hWndSaveDirBrowse;
  static HWND hWndSaveNoBomTitle;
  static HWND hWndForceRadio;
  static HWND hWndDialogUncheckRadio;
  static HWND hWndUTF8;
  static HWND hWndUTF16LE;
  static HWND hWndUTF16BE;
  static HWND hWndUTF32LE;
  static HWND hWndUTF32BE;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndAutoSaveTitle=GetDlgItem(hDlg, IDC_AUTOSAVE_TITLE);
    hWndSaveIntervalCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEINTERVAL_CHECK);
    hWndSaveInterval=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEINTERVAL);
    hWndSaveFocusCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEFOCUS_CHECK);
    hWndSaveFrameCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEFRAME_CHECK);
    hWndSaveSimpleCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVESIMPLE_CHECK);
    hWndSaveNearCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVENEAR_CHECK);
    hWndSaveDirCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEDIR_CHECK);
    hWndSaveDir=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEDIR);
    hWndSaveDirBrowse=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEDIR_BROWSE);
    hWndSaveNoBomTitle=GetDlgItem(hDlg, IDC_SAVENOBOM_TITLE);
    hWndForceRadio=GetDlgItem(hDlg, IDC_SAVENOBOM_FORCE_RADIO);
    hWndDialogUncheckRadio=GetDlgItem(hDlg, IDC_SAVENOBOM_DLGUNCHECK_RADIO);
    hWndUTF8=GetDlgItem(hDlg, IDC_SAVENOBOM_UTF8);
    hWndUTF16LE=GetDlgItem(hDlg, IDC_SAVENOBOM_UTF16LE);
    hWndUTF16BE=GetDlgItem(hDlg, IDC_SAVENOBOM_UTF16BE);
    hWndUTF32LE=GetDlgItem(hDlg, IDC_SAVENOBOM_UTF32LE);
    hWndUTF32BE=GetDlgItem(hDlg, IDC_SAVENOBOM_UTF32BE);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_MOMENT_GROUP, GetLangStringW(wLangModule, STRID_SAVEMOMENT));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEINTERVAL_CHECK, GetLangStringW(wLangModule, STRID_SAVEINTERVAL));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_MIN_LABEL, GetLangStringW(wLangModule, STRID_MIN));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEFOCUS_CHECK, GetLangStringW(wLangModule, STRID_SAVEFOCUS));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEFRAME_CHECK, GetLangStringW(wLangModule, STRID_SAVEFRAME));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_METHOD_GROUP, GetLangStringW(wLangModule, STRID_SAVEMETHOD));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVESIMPLE_CHECK, GetLangStringW(wLangModule, STRID_SAVESIMPLE));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVENEAR_CHECK, GetLangStringW(wLangModule, STRID_SAVENEAR));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEDIR_CHECK, GetLangStringW(wLangModule, STRID_SAVEDIR));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEDIR_NOTE, GetLangStringW(wLangModule, STRID_SAVEDIRVARS));
    SetDlgItemTextWide(hDlg, IDC_SAVENOBOM_GROUP, GetLangStringW(wLangModule, STRID_SAVENOBOM));
    SetDlgItemTextWide(hDlg, IDC_SAVENOBOM_FORCE_RADIO, GetLangStringW(wLangModule, STRID_FORCENOBOM));
    SetDlgItemTextWide(hDlg, IDC_SAVENOBOM_DLGUNCHECK_RADIO, GetLangStringW(wLangModule, STRID_DLGUNCHECK));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    if (!bInitAutoSave) EnableWindow(hWndAutoSaveTitle, FALSE);
    if (!bInitSaveNoBOM) EnableWindow(hWndSaveNoBomTitle, FALSE);
    if (dwSaveMoment & SMOM_TIME) SendMessage(hWndSaveIntervalCheck, BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(hWndSaveInterval, EM_LIMITTEXT, 4, 0);
    SetDlgItemInt(hDlg, IDC_AUTOSAVE_SAVEINTERVAL, dwSaveInterval, FALSE);
    if (dwSaveMoment & SMOM_FOCUS) SendMessage(hWndSaveFocusCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMoment & SMOM_FRAME) SendMessage(hWndSaveFrameCheck, BM_SETCHECK, BST_CHECKED, 0);

    if (dwSaveMethod & SMET_SIMPLE) SendMessage(hWndSaveSimpleCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMethod & SMET_NEAR) SendMessage(hWndSaveNearCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMethod & SMET_DIR) SendMessage(hWndSaveDirCheck, BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(hWndSaveDir, EM_LIMITTEXT, MAX_PATH, 0);
    SetWindowTextWide(hWndSaveDir, wszSaveDir);

    if (dwSaveNoBomSettings & NOBOM_DLGUNCHECK)
      SendMessage(hWndDialogUncheckRadio, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndForceRadio, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveNoBomSettings & NOBOM_UTF8)
      SendMessage(hWndUTF8, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveNoBomSettings & NOBOM_UTF16LE)
      SendMessage(hWndUTF16LE, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveNoBomSettings & NOBOM_UTF16BE)
      SendMessage(hWndUTF16BE, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveNoBomSettings & NOBOM_UTF32LE)
      SendMessage(hWndUTF32LE, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveNoBomSettings & NOBOM_UTF32BE)
      SendMessage(hWndUTF32BE, BM_SETCHECK, BST_CHECKED, 0);

    PostMessage(hDlg, WM_COMMAND, IDC_AUTOSAVE_SAVEINTERVAL_CHECK, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    DWORD dwState;

    //Save moment
    if (LOWORD(wParam) == IDC_AUTOSAVE_SAVEINTERVAL_CHECK)
    {
      if (SendMessage(hWndSaveIntervalCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMoment|=SMOM_TIME;
      else
        dwSaveMoment&=~SMOM_TIME;
    }
    else if (LOWORD(wParam) == IDC_AUTOSAVE_SAVEFOCUS_CHECK)
    {
      if (SendMessage(hWndSaveFocusCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMoment|=SMOM_FOCUS;
      else
        dwSaveMoment&=~SMOM_FOCUS;
    }
    else if (LOWORD(wParam) == IDC_AUTOSAVE_SAVEFRAME_CHECK)
    {
      if (SendMessage(hWndSaveFrameCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMoment|=SMOM_FRAME;
      else
        dwSaveMoment&=~SMOM_FRAME;
    }

    //Save method
    dwState=dwSaveMethod;
    if (LOWORD(wParam) == IDC_AUTOSAVE_SAVESIMPLE_CHECK)
    {
      if (SendMessage(hWndSaveSimpleCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwState|=SMET_SIMPLE;
      else
        dwState&=~SMET_SIMPLE;
    }
    else if (LOWORD(wParam) == IDC_AUTOSAVE_SAVENEAR_CHECK)
    {
      if (SendMessage(hWndSaveNearCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwState|=SMET_NEAR;
      else
        dwState&=~SMET_NEAR;
    }
    else if (LOWORD(wParam) == IDC_AUTOSAVE_SAVEDIR_CHECK)
    {
      if (SendMessage(hWndSaveDirCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwState|=SMET_DIR;
      else
        dwState&=~SMET_DIR;
    }
    if (!dwState)
      SendMessage(GetDlgItem(hDlg, LOWORD(wParam)), BM_SETCHECK, BST_CHECKED, 0);
    else
      dwSaveMethod=dwState;

    //Enable windows
    if (LOWORD(wParam) == IDC_AUTOSAVE_SAVEINTERVAL_CHECK ||
        LOWORD(wParam) == IDC_AUTOSAVE_SAVEFOCUS_CHECK ||
        LOWORD(wParam) == IDC_AUTOSAVE_SAVEFRAME_CHECK ||
        LOWORD(wParam) == IDC_AUTOSAVE_SAVESIMPLE_CHECK ||
        LOWORD(wParam) == IDC_AUTOSAVE_SAVENEAR_CHECK ||
        LOWORD(wParam) == IDC_AUTOSAVE_SAVEDIR_CHECK)
    {
      if (!dwSaveMoment)
      {
        EnableWindow(hWndSaveSimpleCheck, FALSE);
        EnableWindow(hWndSaveNearCheck, FALSE);
        EnableWindow(hWndSaveDirCheck, FALSE);
        EnableWindow(hWndSaveDir, FALSE);
        EnableWindow(hWndSaveDirBrowse, FALSE);
      }
      else
      {
        EnableWindow(hWndSaveSimpleCheck, TRUE);
        EnableWindow(hWndSaveNearCheck, TRUE);
        EnableWindow(hWndSaveDirCheck, TRUE);

        if (SendMessage(hWndSaveDirCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        {
          EnableWindow(hWndSaveDir, TRUE);
          EnableWindow(hWndSaveDirBrowse, TRUE);
        }
        else
        {
          EnableWindow(hWndSaveDir, FALSE);
          EnableWindow(hWndSaveDirBrowse, FALSE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_AUTOSAVE_SAVEDIR_BROWSE)
    {
      BROWSEINFOW bi;
      LPITEMIDLIST pIdList;
      LPMALLOC pMalloc;
      wchar_t wszDir[MAX_PATH];

      GetWindowTextWide(hWndSaveDir, wszDir, MAX_PATH);
      bi.hwndOwner=hDlg;
      bi.pidlRoot=NULL;
      bi.pszDisplayName=wszDir;
      bi.lpszTitle=NULL;
      bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE|BIF_EDITBOX;
      bi.lpfn=BrowseCallbackProc;
      bi.lParam=(LPARAM)wszDir;
      bi.iImage=0;

      if (pIdList=SHBrowseForFolderWide(&bi))
      {
        SHGetPathFromIDListWide(pIdList, wszDir);

        if (SHGetMalloc(&pMalloc))
        {
          pMalloc->lpVtbl->Free(pMalloc, pIdList);
          pMalloc->lpVtbl->Release(pMalloc);
        }
        SetWindowTextWide(hWndSaveDir, wszDir);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDOK)
    {
      dwSaveInterval=GetDlgItemInt(hDlg, IDC_AUTOSAVE_SAVEINTERVAL, NULL, FALSE);

      GetWindowTextWide(hWndSaveDir, wszSaveDir, MAX_PATH);
      TranslateFileString(wszSaveDir, wszSaveDirExp, MAX_PATH);

      dwSaveNoBomSettings=0;
      if (SendMessage(hWndDialogUncheckRadio, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveNoBomSettings|=NOBOM_DLGUNCHECK;
      if (SendMessage(hWndUTF8, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveNoBomSettings|=NOBOM_UTF8;
      if (SendMessage(hWndUTF16LE, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveNoBomSettings|=NOBOM_UTF16LE;
      if (SendMessage(hWndUTF16BE, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveNoBomSettings|=NOBOM_UTF16BE;
      if (SendMessage(hWndUTF32LE, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveNoBomSettings|=NOBOM_UTF32LE;
      if (SendMessage(hWndUTF32BE, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveNoBomSettings|=NOBOM_UTF32BE;

      SaveOptions(OF_AUTOSAVE|OF_SAVENOBOM);
      EndDialog(hDlg, 0);

      if (nInitMain)
      {
        if (bInitAutoSave)
        {
          UninitAutoSave();
          InitAutoSave();
        }
        if (bInitSaveNoBOM)
        {
          UninitSaveNoBOM();
          InitSaveNoBOM();
        }
      }
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

int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
  {
    if (bOldWindows)
    {
      char szPath[MAX_PATH];

      WideCharToMultiByte(CP_ACP, 0, (wchar_t *)lpData, -1, szPath, MAX_PATH, NULL, NULL);
      SendMessage(hWnd, BFFM_SETSELECTIONA, TRUE, (LPARAM)szPath);
    }
    else SendMessage(hWnd, BFFM_SETSELECTIONW, TRUE, lpData);
  }
  return 0;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  DoAutoSave(SMOM_TIME);
}

void DoAutoSave(int nSaveMoment)
{
  if (nMDI == WMD_SDI || nSaveMoment == SMOM_FRAME)
  {
    MakeBackupFile(NULL);
  }
  else if (nMDI == WMD_PMDI || nMDI == WMD_MDI)
  {
    FRAMEDATA *lpFrameInit=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0);
    FRAMEDATA *lpFrameCount=lpFrameInit;

    while (lpFrameCount)
    {
      if (IsBackupNeeded(lpFrameCount))
      {
        if (nMDI == WMD_PMDI)
          SendMessage(hMainWnd, AKD_FRAMEACTIVATE, FWA_NOUPDATEORDER, (LPARAM)lpFrameCount);

        if (!MakeBackupFile(lpFrameCount->ei.hWndEdit))
          break;
      }

      if (lpFrameInit == (lpFrameCount=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_NEXT, (LPARAM)lpFrameCount)))
      {
        if (nMDI == WMD_PMDI)
          SendMessage(hMainWnd, AKD_FRAMEACTIVATE, FWA_NOUPDATEORDER, (LPARAM)lpFrameInit);
        break;
      }
    }
  }
}

BOOL IsBackupNeeded(FRAMEDATA *lpFrame)
{
  if (lpFrame)
  {
    if (lpFrame->ei.bModified)
    {
      if (dwSaveMethod & SMET_SIMPLE)
      {
        if (*lpFrame->ei.wszFile)
          return TRUE;
      }
      if (dwSaveMethod & SMET_NEAR)
      {
        if (*lpFrame->ei.wszFile)
          return TRUE;
      }
      if (dwSaveMethod & SMET_DIR)
      {
        if (*wszSaveDirExp)
          return TRUE;
      }
    }
  }
  return FALSE;
}

BOOL MakeBackupFile(HWND hWndEdit)
{
  static BOOL bProcessing=FALSE;
  SAVEDOCUMENTW sd;
  EDITINFO ei;
  wchar_t wszSaveFile[MAX_PATH];
  BOOL bResult=TRUE;

  if (!bProcessing)
  {
    bProcessing=TRUE;

    if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWndEdit, (LPARAM)&ei))
    {
      if (ei.bModified)
      {
        if (dwSaveMethod & SMET_SIMPLE)
        {
          if (*ei.wszFile)
          {
            xstrcpynW(wszSaveFile, ei.wszFile, MAX_PATH);
            sd.pFile=wszSaveFile;
            sd.nCodePage=ei.nCodePage;
            sd.bBOM=ei.bBOM;
            sd.dwFlags=SD_UPDATE;
            if (SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)ei.hWndEdit, (LPARAM)&sd) != ESD_SUCCESS)
              bResult=FALSE;
          }
        }
        if (dwSaveMethod & SMET_NEAR)
        {
          if (*ei.wszFile)
          {
            xprintfW(wszSaveFile, L"%s.tmp", ei.wszFile);
            sd.pFile=wszSaveFile;
            sd.nCodePage=ei.nCodePage;
            sd.bBOM=ei.bBOM;
            sd.dwFlags=0;
            if (SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)ei.hWndEdit, (LPARAM)&sd) != ESD_SUCCESS)
              bResult=FALSE;
          }
        }
        if (dwSaveMethod & SMET_DIR)
        {
          if (*wszSaveDirExp)
          {
            CreateDirectoryWide(wszSaveDirExp, NULL);

            if (*ei.wszFile)
              xprintfW(wszSaveFile, L"%s\\%s", wszSaveDirExp, GetFileName(ei.wszFile, -1));
            else
              xprintfW(wszSaveFile, L"%s\\%Id.tmp", wszSaveDirExp, ei.hWndEdit);
            sd.pFile=wszSaveFile;
            sd.nCodePage=ei.nCodePage;
            sd.bBOM=ei.bBOM;
            sd.dwFlags=0;
            if (SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)ei.hWndEdit, (LPARAM)&sd) != ESD_SUCCESS)
              bResult=FALSE;
          }
        }
      }
    }
    bProcessing=FALSE;
  }
  return bResult;
}

void RemoveBackupFile(HWND hWndEdit, const wchar_t *wpEditFile)
{
  wchar_t wszFile[MAX_PATH];

  if (dwSaveMethod & SMET_NEAR)
  {
    if (*wpEditFile)
    {
      xprintfW(wszFile, L"%s.tmp", wpEditFile);
      if (FileExistsWide(wszFile))
        DeleteFileWide(wszFile);
    }
  }
  if (dwSaveMethod & SMET_DIR)
  {
    if (*wszSaveDirExp)
    {
      if (*wpEditFile)
        xprintfW(wszFile, L"%s\\%s", wszSaveDirExp, GetFileName(wpEditFile, -1));
      else
        xprintfW(wszFile, L"%s\\%Id.tmp", wszSaveDirExp, hWndEdit);

      if (xstrcmpiW(wpEditFile, wszFile))
      {
        if (FileExistsWide(wszFile))
          DeleteFileWide(wszFile);
      }
    }
  }
}

int GetComboboxCodepage(HWND hWnd)
{
  int nCodePage=0;
  int nSelection;

  if ((nSelection=(int)SendMessage(hWnd, CB_GETCURSEL, 0, 0)) != CB_ERR)
  {
    ComboBox_GetLBTextWide(hWnd, nSelection, wszBuffer);
    nCodePage=(int)xatoiW(wszBuffer, NULL);
  }
  return nCodePage;
}

int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%a -AkelPad directory, %% -%
  wchar_t *wpExeDir=wszExeDir;
  wchar_t *wszSource;
  wchar_t *wpSource;
  wchar_t *wpTarget=wszBuffer;
  wchar_t *wpTargetMax=wszBuffer + (wszBuffer?nBufferSize:0x7FFFFFFF);
  int nStringLen;
  BOOL bStringStart=TRUE;

  //Expand environment strings
  nStringLen=ExpandEnvironmentStringsWide(wpString, NULL, 0);

  if (wszSource=(wchar_t *)GlobalAlloc(GPTR, nStringLen * sizeof(wchar_t)))
  {
    ExpandEnvironmentStringsWide(wpString, wszSource, nStringLen);

    //Expand plugin variables
    for (wpSource=wszSource; *wpSource && wpTarget < wpTargetMax;)
    {
      if (bStringStart && *wpSource == L'%')
      {
        if (*++wpSource == L'%')
        {
          ++wpSource;
          if (wszBuffer) *wpTarget=L'%';
          ++wpTarget;
        }
        else if (*wpSource == L'a' || *wpSource == L'A')
        {
          ++wpSource;
          wpTarget+=xstrcpynW(wszBuffer?wpTarget:NULL, wpExeDir, wpTargetMax - wpTarget) - !wszBuffer;
        }
      }
      else
      {
        if (*wpSource != L'\"' && *wpSource != L'\'' && *wpSource != L'`')
          bStringStart=FALSE;
        if (wszBuffer) *wpTarget=*wpSource;
        ++wpTarget;
        ++wpSource;
      }
    }
    if (wpTarget < wpTargetMax)
    {
      if (wszBuffer)
        *wpTarget=L'\0';
      else
        ++wpTarget;
    }
    GlobalFree((HGLOBAL)wszSource);
  }
  return (int)(wpTarget - wszBuffer);
}

const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; wpCount >= wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
      return wpCount + 1;
  }
  return wpFile;
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
    if (dwFlags & OF_AUTOSAVE)
    {
      WideOption(hOptions, L"SaveMoment", PO_DWORD, (LPBYTE)&dwSaveMoment, sizeof(DWORD));
      WideOption(hOptions, L"SaveInterval", PO_DWORD, (LPBYTE)&dwSaveInterval, sizeof(DWORD));
      WideOption(hOptions, L"SaveMethod", PO_DWORD, (LPBYTE)&dwSaveMethod, sizeof(DWORD));
      WideOption(hOptions, L"SaveDir", PO_STRING, (LPBYTE)wszSaveDir, MAX_PATH * sizeof(wchar_t));
    }
    if (dwFlags & OF_SAVENOBOM)
    {
      WideOption(hOptions, L"SaveNoBomSettings", PO_DWORD, (LPBYTE)&dwSaveNoBomSettings, sizeof(DWORD));
    }

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_AUTOSAVE)
    {
      WideOption(hOptions, L"SaveMoment", PO_DWORD, (LPBYTE)&dwSaveMoment, sizeof(DWORD));
      WideOption(hOptions, L"SaveInterval", PO_DWORD, (LPBYTE)&dwSaveInterval, sizeof(DWORD));
      WideOption(hOptions, L"SaveMethod", PO_DWORD, (LPBYTE)&dwSaveMethod, sizeof(DWORD));
      WideOption(hOptions, L"SaveDir", PO_STRING, (LPBYTE)wszSaveDir, (lstrlenW(wszSaveDir) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_SAVENOBOM)
    {
      WideOption(hOptions, L"SaveNoBomSettings", PO_DWORD, (LPBYTE)&dwSaveNoBomSettings, sizeof(DWORD));
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
    if (nStringID == STRID_SAVEMOMENT)
      return L"\x041C\x043E\x043C\x0435\x043D\x0442\x0020\x0441\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x0438\x044F";
    if (nStringID == STRID_SAVEINTERVAL)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x043A\x0430\x0436\x0434\x044B\x0435";
    if (nStringID == STRID_MIN)
      return L"\x043C\x0438\x043D\x002E";
    if (nStringID == STRID_SAVEFOCUS)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x043F\x0440\x0438\x0020\x043F\x043E\x0442\x0435\x0440\x0435\x0020\x0444\x043E\x043A\x0443\x0441\x0430";
    if (nStringID == STRID_SAVEFRAME)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x043F\x0440\x0438\x0020\x043F\x0435\x0440\x0435\x043A\x043B\x044E\x0447\x0435\x043D\x0438\x0438\x0020\x0432\x043A\x043B\x0430\x0434\x043E\x043A";
    if (nStringID == STRID_SAVEMETHOD)
      return L"\x041C\x0435\x0442\x043E\x0434\x0020\x0441\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x0438\x044F";
    if (nStringID == STRID_SAVESIMPLE)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x043A\x0430\x043A\x0020\x043E\x0431\x044B\x0447\x043D\x043E";
    if (nStringID == STRID_SAVENEAR)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x043A\x043E\x043F\x0438\x044E\x0020\x0440\x044F\x0434\x043E\x043C\x0020\x043A\x0430\x043A\x0020\x002A\x002E\x0074\x006D\x0070\x0020\x0444\x0430\x0439\x043B";
    if (nStringID == STRID_SAVEDIR)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x043A\x043E\x043F\x0438\x044E\x0020\x0432\x0020\x0434\x0438\x0440\x0435\x043A\x0442\x043E\x0440\x0438\x0438\x003A";
    if (nStringID == STRID_SAVEDIRVARS)
      return L"\x0025\x0061\x0020\x002D\x0020\x0434\x0438\x0440\x0435\x043A\x0442\x043E\x0440\x0438\x044F\x0020\x0041\x006B\x0065\x006C\x0050\x0061\x0064\x0027\x0430";
    if (nStringID == STRID_SAVENOBOM)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x0431\x0435\x0437\x0020\x0042\x004F\x004D";
    if (nStringID == STRID_FORCENOBOM)
      return L"\x0424\x043E\x0440\x0441\x0438\x0440\x043E\x0432\x0430\x0442\x044C\x0020\x0441\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x0438\x0435\x0020\x0431\x0435\x0437\x0020\x0042\x004F\x004D";
    if (nStringID == STRID_DLGUNCHECK)
      return L"\x0421\x043D\x0438\x043C\x0430\x0442\x044C\x0020\x0433\x0430\x043B\x043A\x0443\x0020\x0042\x004F\x004D\x0020\x0432\x0020\x0434\x0438\x0430\x043B\x043E\x0433\x0435\x0020\x0441\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x0438\x044F";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_SAVEMOMENT)
      return L"Save moment";
    if (nStringID == STRID_SAVEINTERVAL)
      return L"Save every";
    if (nStringID == STRID_MIN)
      return L"min.";
    if (nStringID == STRID_SAVEFOCUS)
      return L"Save on lose focus";
    if (nStringID == STRID_SAVEFRAME)
      return L"Save on deactivate frame";
    if (nStringID == STRID_SAVEMETHOD)
      return L"Save method";
    if (nStringID == STRID_SAVESIMPLE)
      return L"Save as usual";
    if (nStringID == STRID_SAVENEAR)
      return L"Save a copy as *.tmp file";
    if (nStringID == STRID_SAVEDIR)
      return L"Save a copy in directory:";
    if (nStringID == STRID_SAVEDIRVARS)
      return L"%a - AkelPad directory";
    if (nStringID == STRID_SAVENOBOM)
      return L"Save without BOM";
    if (nStringID == STRID_FORCENOBOM)
      return L"Force saving without BOM";
    if (nStringID == STRID_DLGUNCHECK)
      return L"Uncheck BOM in save dialog";
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
  hMdiClient=pd->hMdiClient;
  bOldWindows=pd->bOldWindows;
  nMDI=pd->nMDI;
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
  xstrcpynW(wszExeDir, pd->wszAkelDir, MAX_PATH);
  ReadOptions(OF_ALL);

  TranslateFileString(wszSaveDir, wszSaveDirExp, MAX_PATH);
}

void InitMain()
{
  if (!nInitMain++)
  {
    //SubClass
    NewMainProcData=NULL;
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);
  }
}

void UninitMain()
{
  if (!--nInitMain)
  {
    if (NewMainProcData)
    {
      SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
      NewMainProcData=NULL;
    }
  }
}

void InitAutoSave()
{
  bInitAutoSave=TRUE;

  if (dwSaveMoment & SMOM_TIME)
  {
    dwSaveTimer=SetTimer(NULL, 0, dwSaveInterval * 60000, (TIMERPROC)TimerProc);
  }
}

void UninitAutoSave()
{
  bInitAutoSave=FALSE;

  if (dwSaveTimer)
  {
    KillTimer(NULL, dwSaveTimer);
    dwSaveTimer=0;
  }
}

void InitSaveNoBOM()
{
  bInitSaveNoBOM=TRUE;
}

void UninitSaveNoBOM()
{
  bInitSaveNoBOM=FALSE;
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
    if (bInitAutoSave)
    {
      UninitMain();
      UninitAutoSave();
    }
    if (bInitSaveNoBOM)
    {
      UninitMain();
      UninitSaveNoBOM();
    }
  }
  return TRUE;
}
