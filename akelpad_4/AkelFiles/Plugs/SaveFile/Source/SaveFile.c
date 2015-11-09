#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


//Include stack functions
#define StackInsertBefore
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xarraysizeW
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
#define FindFirstFileWide
#define FindNextFileWide
#define GetDlgItemTextWide
#define GetFileAttributesWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define SHBrowseForFolderWide
#define SHFileOperationWide
#define SHGetPathFromIDListWide
#include "WideFunc.h"

//Defines
#define STRID_SAVEMOMENT           1
#define STRID_SAVEINTERVAL         2
#define STRID_MIN                  3
#define STRID_SAVEFOCUS            4
#define STRID_SAVEFRAME            5
#define STRID_SAVEMETHOD           6
#define STRID_SAVESIMPLE           7
#define STRID_SAVENEAR             8
#define STRID_SAVEDIR              9
#define STRID_SAVEDIRVARS          10
#define STRID_SESSIONS_GROUP       11
#define STRID_SESSIONS_SILENTEXIT  12
#define STRID_SESSIONS_SAVEFILE    13
#define STRID_TMPFILE              14
#define STRID_TMPDELETE            15
#define STRID_TMPTOBIN             16
#define STRID_TMPRECOVER           17
#define STRID_SAVENOBOM            18
#define STRID_FORCENOBOM           19
#define STRID_DLGUNCHECK           20
#define STRID_PLUGIN               21
#define STRID_OK                   22
#define STRID_CANCEL               23

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

//Sessions plugin
#define SES_SILENTEXIT  0x1
#define SES_SAVEFILE    0x2

//.tmp file
#define REMC_DELETE   0x1
#define REMC_TOBIN    0x2
#define REMC_RECOVER  0x4

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

typedef struct _BACKUPFILE {
  struct _BACKUPFILE *next;
  struct _BACKUPFILE *prev;
  wchar_t wszName[MAX_PATH];
  wchar_t wszDir[MAX_PATH];
  wchar_t wszFile[MAX_PATH];
  wchar_t wszFileBackup[MAX_PATH];
  FRAMEDATA *lpFrame;
} BACKUPFILE;

typedef struct {
  BACKUPFILE *first;
  BACKUPFILE *last;
} STACKBACKUPFILE;

//Sessions external call
#define DLLA_SESSIONS_SAVE          2

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  wchar_t *wpSession;
} DLLEXTSESSIONSSAVE;

//Functions prototypes
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SaveFileProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL CALLBACK RecoverDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void DoAutoSave(FRAMEDATA *lpFrame, int nSaveMoment);
BOOL IsBackupNeeded(FRAMEDATA *lpFrame);
BOOL MakeBackupFile(FRAMEDATA *lpFrame);
void RemoveBackupFile(const wchar_t *wpEditFile, AEHDOC hDocEdit, DWORD dwTmpFlags);
int DeleteBackupFile(const wchar_t *wpFile, DWORD dwTmpFlags);
void UncheckBOM(HWND hDlg);
int GetComboboxCodepage(HWND hWnd);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);

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
int nSaveDirExpLen;
UINT_PTR dwSaveTimer=0;
DWORD dwSaveMoment=SMOM_TIME;
DWORD dwSaveInterval=5;
DWORD dwSaveMethod=SMET_SIMPLE;
DWORD dwSessions=0;
DWORD dwTmpFile=REMC_DELETE|REMC_RECOVER;
int nAppActive=TRUE;
DWORD dwSaveNoBomSettings=NOBOM_UTF8;
WNDPROC lpOldSaveFileProc;
WNDPROCDATA *NewMainProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
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

  if (uMsg == AKDN_INITDIALOGEND)
  {
    if (bInitSaveNoBOM && (dwSaveNoBomSettings & NOBOM_DLGUNCHECK))
    {
      if (wParam == IDT_SAVEFILE)
      {
        NINITDIALOG *nid=(NINITDIALOG *)lParam;

        UncheckBOM(nid->hWnd);
        lpOldSaveFileProc=(WNDPROC)GetWindowLongPtrWide(nid->hWnd, GWLP_WNDPROC);
        SetWindowLongPtrWide(nid->hWnd, GWLP_WNDPROC, (UINT_PTR)SaveFileProc);
      }
    }
  }
  else if (uMsg == AKDN_OPENDOCUMENT_START)
  {
    NOPENDOCUMENT *nod=(NOPENDOCUMENT *)lParam;
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (bInitAutoSave)
    {
      wchar_t wszSearch[MAX_PATH];
      wchar_t wszBackupFile[MAX_PATH];
      wchar_t wszDir[MAX_PATH];
      const wchar_t *wpName;
      WIN32_FIND_DATAW wfd;
      HANDLE hSearch;
      int nNameLen;
      int nDirLen;
      STACKBACKUPFILE hBackupStack={0};
      BACKUPFILE *lpBackupFile;

      wpName=GetFileName(nod->wszFile, -1);
      nNameLen=(int)xstrlenW(wpName);
      nDirLen=(int)GetFileDir(nod->wszFile, -1, wszDir, MAX_PATH);
      wszBackupFile[0]=L'\0';
      xprintfW(wszSearch, L"%s.*.tmp", nod->wszFile);

      if ((hSearch=FindFirstFileWide(wszSearch, &wfd)) != INVALID_HANDLE_VALUE)
      {
        do
        {
          //Is backup format
          for (wpName=wfd.cFileName + nNameLen + 1; *wpName; ++wpName)
          {
            if (!xstrcmpiW(wpName, L".tmp"))
            {
              if (!StackInsertBefore((stack **)&hBackupStack.first, (stack **)&hBackupStack.last, (stack *)NULL, (stack **)&lpBackupFile, sizeof(BACKUPFILE)))
              {
                xstrcpynW(lpBackupFile->wszName, wpName, MAX_PATH);
                xstrcpynW(lpBackupFile->wszDir, wszDir, MAX_PATH);
                xstrcpynW(lpBackupFile->wszFile, nod->wszFile, MAX_PATH);
                xprintfW(lpBackupFile->wszFileBackup, L"%s\\%s", wszDir, wfd.cFileName);
                lpBackupFile->lpFrame=lpFrame;
              }
              break;
            }
            if (*wpName < L'0' || *wpName > L'9')
              break;
          }
        }
        while (FindNextFileWide(hSearch, &wfd));

        FindClose(hSearch);
      }
      if (hBackupStack.first)
      {
        DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_RECOVER), hMainWnd, (DLGPROC)RecoverDlgProc);
      }
      StackClear((stack **)&hBackupStack.first, (stack **)&hBackupStack.last);
    }
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_START)
  {
    NSAVEDOCUMENT *nsd=(NSAVEDOCUMENT *)lParam;
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

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
        xstrcpynW(wszFileNameNew, nsd->wszFile, MAX_PATH);
        xstrcpynW(wszFileNameOld, lpFrame->ei.wszFile, MAX_PATH);
        bRemoveBackupCheck=TRUE;
      }
    }
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_FINISH)
  {
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (bInitAutoSave)
    {
      if (bRemoveBackupCheck)
      {
        if (lParam == ESD_SUCCESS)
        {
          if (xstrcmpiW(wszFileNameNew, wszFileNameOld))
          {
            RemoveBackupFile(wszFileNameOld, lpFrame->ei.hDocEdit, 0);
          }
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
      FRAMEDATA *lpFrame;

      if (lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_BYEDITDOCUMENT, lParam))
      {
        if (lpFrame->ei.bModified && (dwSaveMoment & SMOM_FRAME) && (!(dwTmpFile & REMC_DELETE) || (dwTmpFile & REMC_TOBIN)))
          DoAutoSave(lpFrame, SMOM_FRAME);
        if (!lpFrame->ei.bModified || (dwTmpFile & REMC_DELETE))
          RemoveBackupFile(lpFrame->ei.wszFile, lpFrame->ei.hDocEdit, dwTmpFile);
      }
    }
  }
  else if (uMsg == AKDN_FRAME_DEACTIVATE)
  {
    if (bInitAutoSave)
    {
      if (dwSaveMoment & SMOM_FRAME)
        DoAutoSave((FRAMEDATA *)lParam, SMOM_FRAME);
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    if (bInitAutoSave)
    {
      if (dwSaveMoment & SMOM_FOCUS)
        nAppActive=-1;
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
              DoAutoSave(NULL, SMOM_FOCUS);
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
      LRESULT lResult;

      lResult=CallWindowProcWide(lpOldSaveFileProc, hWnd, uMsg, wParam, lParam);
      UncheckBOM(hWnd);
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
  static HWND hWndSessionsSilentExit;
  static HWND hWndSessionsSaveFile;
  static HWND hWndTmpDelete;
  static HWND hWndTmpToBin;
  static HWND hWndTmpRecover;
  static HWND hWndSaveNoBomTitle;
  static HWND hWndForceRadio;
  static HWND hWndDialogUncheckRadio;
  static HWND hWndUTF8;
  static HWND hWndUTF16LE;
  static HWND hWndUTF16BE;
  static HWND hWndUTF32LE;
  static HWND hWndUTF32BE;
  static DWORD dwSaveMomentDlg;
  static DWORD dwSaveMethodDlg;
  static DWORD dwSessionsDlg;
  static DWORD dwTmpFileDlg;
  static BOOL bSessionsRunning;

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
    hWndSessionsSilentExit=GetDlgItem(hDlg, IDC_AUTOSAVE_SESSIONS_SILENTEXIT_CHECK);
    hWndSessionsSaveFile=GetDlgItem(hDlg, IDC_AUTOSAVE_SESSIONS_SAVEFILE_CHECK);
    hWndTmpDelete=GetDlgItem(hDlg, IDC_AUTOSAVE_TMPDELETE_CHECK);
    hWndTmpToBin=GetDlgItem(hDlg, IDC_AUTOSAVE_TMPTOBIN_CHECK);
    hWndTmpRecover=GetDlgItem(hDlg, IDC_AUTOSAVE_TMPRECOVER_CHECK);
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
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SESSIONS_GROUP, GetLangStringW(wLangModule, STRID_SESSIONS_GROUP));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SESSIONS_SILENTEXIT_CHECK, GetLangStringW(wLangModule, STRID_SESSIONS_SILENTEXIT));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SESSIONS_SAVEFILE_CHECK, GetLangStringW(wLangModule, STRID_SESSIONS_SAVEFILE));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_TMPFILE_GROUP, GetLangStringW(wLangModule, STRID_TMPFILE));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_TMPDELETE_CHECK, GetLangStringW(wLangModule, STRID_TMPDELETE));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_TMPTOBIN_CHECK, GetLangStringW(wLangModule, STRID_TMPTOBIN));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_TMPRECOVER_CHECK, GetLangStringW(wLangModule, STRID_TMPRECOVER));
    SetDlgItemTextWide(hDlg, IDC_SAVENOBOM_GROUP, GetLangStringW(wLangModule, STRID_SAVENOBOM));
    SetDlgItemTextWide(hDlg, IDC_SAVENOBOM_FORCE_RADIO, GetLangStringW(wLangModule, STRID_FORCENOBOM));
    SetDlgItemTextWide(hDlg, IDC_SAVENOBOM_DLGUNCHECK_RADIO, GetLangStringW(wLangModule, STRID_DLGUNCHECK));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    if (!bInitAutoSave) EnableWindow(hWndAutoSaveTitle, FALSE);
    if (!bInitSaveNoBOM) EnableWindow(hWndSaveNoBomTitle, FALSE);

    dwSaveMomentDlg=dwSaveMoment;
    if (dwSaveMomentDlg & SMOM_TIME) SendMessage(hWndSaveIntervalCheck, BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(hWndSaveInterval, EM_LIMITTEXT, 4, 0);
    SetDlgItemInt(hDlg, IDC_AUTOSAVE_SAVEINTERVAL, dwSaveInterval, FALSE);
    if (dwSaveMomentDlg & SMOM_FOCUS) SendMessage(hWndSaveFocusCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMomentDlg & SMOM_FRAME) SendMessage(hWndSaveFrameCheck, BM_SETCHECK, BST_CHECKED, 0);

    dwSaveMethodDlg=dwSaveMethod;
    if (dwSaveMethodDlg & SMET_SIMPLE) SendMessage(hWndSaveSimpleCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMethodDlg & SMET_NEAR) SendMessage(hWndSaveNearCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMethodDlg & SMET_DIR) SendMessage(hWndSaveDirCheck, BM_SETCHECK, BST_CHECKED, 0);

    dwTmpFileDlg=dwTmpFile;
    if (dwTmpFileDlg & REMC_DELETE) SendMessage(hWndTmpDelete, BM_SETCHECK, BST_CHECKED, 0);
    if (dwTmpFileDlg & REMC_TOBIN) SendMessage(hWndTmpToBin, BM_SETCHECK, BST_CHECKED, 0);
    if (dwTmpFileDlg & REMC_RECOVER) SendMessage(hWndTmpRecover, BM_SETCHECK, BST_CHECKED, 0);

    dwSessionsDlg=dwSessions;
    if (dwSessionsDlg & SES_SILENTEXIT) SendMessage(hWndSessionsSilentExit, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSessionsDlg & SES_SAVEFILE) SendMessage(hWndSessionsSaveFile, BM_SETCHECK, BST_CHECKED, 0);

    //Is Sessions plugin running
    {
      PLUGINFUNCTION *pf;

      if (pf=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Sessions::Main", 0))
        bSessionsRunning=pf->bRunning;
      else
        bSessionsRunning=FALSE;
    }

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
    WORD wCommand=LOWORD(wParam);
    DWORD dwSaveMethodNew;
    DWORD dwSessionsNew;

    //Save moment
    if (wCommand == IDC_AUTOSAVE_SAVEINTERVAL_CHECK)
    {
      if (SendMessage(hWndSaveIntervalCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMomentDlg|=SMOM_TIME;
      else
        dwSaveMomentDlg&=~SMOM_TIME;
    }
    else if (wCommand == IDC_AUTOSAVE_SAVEFOCUS_CHECK)
    {
      if (SendMessage(hWndSaveFocusCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMomentDlg|=SMOM_FOCUS;
      else
        dwSaveMomentDlg&=~SMOM_FOCUS;
    }
    else if (wCommand == IDC_AUTOSAVE_SAVEFRAME_CHECK)
    {
      if (SendMessage(hWndSaveFrameCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMomentDlg|=SMOM_FRAME;
      else
        dwSaveMomentDlg&=~SMOM_FRAME;
    }

    //Save method
    dwSaveMethodNew=dwSaveMethodDlg;
    if (wCommand == IDC_AUTOSAVE_SAVESIMPLE_CHECK)
    {
      if (SendMessage(hWndSaveSimpleCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMethodNew|=SMET_SIMPLE;
      else
        dwSaveMethodNew&=~SMET_SIMPLE;
    }
    else if (wCommand == IDC_AUTOSAVE_SAVENEAR_CHECK)
    {
      if (SendMessage(hWndSaveNearCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMethodNew|=SMET_NEAR;
      else
        dwSaveMethodNew&=~SMET_NEAR;
    }
    else if (wCommand == IDC_AUTOSAVE_SAVEDIR_CHECK)
    {
      if (SendMessage(hWndSaveDirCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveMethodNew|=SMET_DIR;
      else
        dwSaveMethodNew&=~SMET_DIR;
    }

    //Sessions plugin
    dwSessionsNew=dwSessionsDlg;
    if (wCommand == IDC_AUTOSAVE_SESSIONS_SILENTEXIT_CHECK)
    {
      if (SendMessage(hWndSessionsSilentExit, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSessionsNew|=SES_SILENTEXIT;
      else
        dwSessionsNew&=~SES_SILENTEXIT;
    }
    else if (wCommand == IDC_AUTOSAVE_SESSIONS_SAVEFILE_CHECK)
    {
      if (SendMessage(hWndSessionsSaveFile, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSessionsNew|=SES_SAVEFILE;
      else
        dwSessionsNew&=~SES_SAVEFILE;
    }

    //.tmp file
    if (wCommand == IDC_AUTOSAVE_TMPDELETE_CHECK)
    {
      if (SendMessage(hWndTmpDelete, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwTmpFileDlg|=REMC_DELETE;
      else
        dwTmpFileDlg&=~REMC_DELETE;
    }
    else if (wCommand == IDC_AUTOSAVE_TMPTOBIN_CHECK)
    {
      if (SendMessage(hWndTmpToBin, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwTmpFileDlg|=REMC_TOBIN;
      else
        dwTmpFileDlg&=~REMC_TOBIN;
    }
    else if (wCommand == IDC_AUTOSAVE_TMPRECOVER_CHECK)
    {
      if (SendMessage(hWndTmpRecover, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwTmpFileDlg|=REMC_RECOVER;
      else
        dwTmpFileDlg&=~REMC_RECOVER;
    }

    if (!dwSaveMethodNew && !(dwSessionsNew & SES_SAVEFILE))
      SendMessage(GetDlgItem(hDlg, wCommand), BM_SETCHECK, BST_CHECKED, 0);
    else
    {
      dwSaveMethodDlg=dwSaveMethodNew;
      dwSessionsDlg=dwSessionsNew;
    }

    //Enable windows
    if (wCommand == IDC_AUTOSAVE_SAVEINTERVAL_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVEFOCUS_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVEFRAME_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVESIMPLE_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVENEAR_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVEDIR_CHECK ||
        wCommand == IDC_AUTOSAVE_SESSIONS_SILENTEXIT_CHECK ||
        wCommand == IDC_AUTOSAVE_TMPDELETE_CHECK)
    {
      EnableWindow(hWndSaveSimpleCheck, dwSaveMomentDlg);
      EnableWindow(hWndSaveNearCheck, dwSaveMomentDlg);
      EnableWindow(hWndSaveDirCheck, dwSaveMomentDlg || (dwSessionsDlg & SES_SILENTEXIT));
      EnableWindow(hWndSaveDir, (dwSaveMomentDlg && (dwSaveMethodDlg & SMET_DIR)) || (dwSessionsDlg & SES_SILENTEXIT));
      EnableWindow(hWndSaveDirBrowse, (dwSaveMomentDlg && (dwSaveMethodDlg & SMET_DIR)) || (dwSessionsDlg & SES_SILENTEXIT));
      EnableWindow(hWndSessionsSilentExit, (dwSaveMethodDlg & SMET_DIR) && bSessionsRunning);
      EnableWindow(hWndSessionsSaveFile, dwSaveMomentDlg && bSessionsRunning);
      EnableWindow(hWndTmpDelete, dwSaveMomentDlg && (dwSaveMethodDlg & (SMET_DIR|SMET_NEAR)));
      EnableWindow(hWndTmpToBin, dwSaveMomentDlg && (dwSaveMethodDlg & (SMET_DIR|SMET_NEAR)) && (dwTmpFileDlg & REMC_DELETE));
      EnableWindow(hWndTmpRecover, dwSaveMomentDlg && (dwSaveMethodDlg & (SMET_DIR|SMET_NEAR)) && (dwTmpFileDlg & REMC_DELETE));
    }
    else if (wCommand == IDC_AUTOSAVE_SAVEDIR_BROWSE)
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
    else if (wCommand == IDOK)
    {
      dwSaveMoment=dwSaveMomentDlg;
      dwSaveMethod=dwSaveMethodDlg;
      dwTmpFile=dwTmpFileDlg;
      dwSessions=dwSessionsDlg;
      dwSaveInterval=GetDlgItemInt(hDlg, IDC_AUTOSAVE_SAVEINTERVAL, NULL, FALSE);

      GetWindowTextWide(hWndSaveDir, wszSaveDir, MAX_PATH);
      if (nSaveDirExpLen=TranslateFileString(wszSaveDir, wszSaveDirExp, MAX_PATH))
        if (wszSaveDirExp[nSaveDirExpLen - 1] == L'\\')
          wszSaveDirExp[--nSaveDirExpLen]=L'\0';
      CreateDirectoryWide(wszSaveDirExp, NULL);

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
    else if (wCommand == IDCANCEL)
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

BOOL CALLBACK RecoverDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndList;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndList=GetDlgItem(hDlg, IDC_RECOVER_LIST);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));
  }
  else if (uMsg == WM_COMMAND)
  {
    WORD wCommand=LOWORD(wParam);

    if (wCommand == IDOK)
    {
      SaveOptions(OF_AUTOSAVE|OF_SAVENOBOM);
      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (wCommand == IDCANCEL)
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

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  DoAutoSave(NULL, SMOM_TIME);
}

void DoAutoSave(FRAMEDATA *lpFrame, int nSaveMoment)
{
  static BOOL bProcessing=FALSE;

  if (!bProcessing)
  {
    bProcessing=TRUE;

    if (dwSaveMethod & (SMET_SIMPLE|SMET_NEAR|SMET_DIR))
    {
      if (!lpFrame) lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0);

      if (nMDI == WMD_SDI || nSaveMoment == SMOM_FRAME)
      {
        MakeBackupFile(lpFrame);
      }
      else if (nMDI == WMD_PMDI || nMDI == WMD_MDI)
      {
        FRAMEDATA *lpFrameInit=lpFrame;
        FRAMEDATA *lpFrameCount=lpFrameInit;

        while (lpFrameCount)
        {
          if (IsBackupNeeded(lpFrameCount))
          {
            if (nMDI == WMD_PMDI)
              SendMessage(hMainWnd, AKD_FRAMEACTIVATE, FWA_NOUPDATEORDER, (LPARAM)lpFrameCount);

            if (!MakeBackupFile(lpFrameCount))
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
    if (dwSessions & SES_SAVEFILE)
    {
      PLUGINFUNCTION *pfSessions;
      PLUGINCALLSENDW pcs;
      DLLEXTSESSIONSSAVE dess;

      if ((pfSessions=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Sessions::Main", 0)) && pfSessions->bRunning)
      {
        dess.dwStructSize=sizeof(DLLEXTSESSIONSSAVE);
        dess.nAction=DLLA_SESSIONS_SAVE;
        dess.wpSession=L"AutoSave";

        pcs.pFunction=L"Sessions::Main";
        pcs.lParam=(LPARAM)&dess;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
      }
    }
    bProcessing=FALSE;
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

BOOL MakeBackupFile(FRAMEDATA *lpFrame)
{
  SAVEDOCUMENTW sd;
  wchar_t wszSaveFile[MAX_PATH];
  BOOL bResult=TRUE;

  if (lpFrame->ei.bModified)
  {
    if (dwSaveMethod & SMET_SIMPLE)
    {
      if (*lpFrame->ei.wszFile)
      {
        xstrcpynW(wszSaveFile, lpFrame->ei.wszFile, MAX_PATH);
        sd.pFile=wszSaveFile;
        sd.nCodePage=lpFrame->ei.nCodePage;
        sd.bBOM=lpFrame->ei.bBOM;
        sd.dwFlags=SD_UPDATE;
        sd.hDoc=NULL;
        if (SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)lpFrame->ei.hWndEdit, (LPARAM)&sd) != ESD_SUCCESS)
          bResult=FALSE;
      }
    }
    if (dwSaveMethod & SMET_NEAR)
    {
      if (*lpFrame->ei.wszFile)
      {
        xprintfW(wszSaveFile, L"%s.%Id.tmp", lpFrame->ei.wszFile, lpFrame->ei.hDocEdit);
        sd.pFile=wszSaveFile;
        sd.nCodePage=lpFrame->ei.nCodePage;
        sd.bBOM=lpFrame->ei.bBOM;
        sd.dwFlags=0;
        sd.hDoc=NULL;
        if (SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)lpFrame->ei.hWndEdit, (LPARAM)&sd) != ESD_SUCCESS)
          bResult=FALSE;
      }
    }
    if (dwSaveMethod & SMET_DIR)
    {
      if (*wszSaveDirExp)
      {
        xprintfW(wszSaveFile, L"%s\\%s.%Id.tmp", wszSaveDirExp, GetFileName(lpFrame->ei.wszFile, -1), lpFrame->ei.hDocEdit);
        sd.pFile=wszSaveFile;
        sd.nCodePage=lpFrame->ei.nCodePage;
        sd.bBOM=lpFrame->ei.bBOM;
        sd.dwFlags=0;
        sd.hDoc=NULL;
        if (SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)lpFrame->ei.hWndEdit, (LPARAM)&sd) != ESD_SUCCESS)
          bResult=FALSE;
      }
    }
  }
  return bResult;
}

void RemoveBackupFile(const wchar_t *wpEditFile, AEHDOC hDocEdit, DWORD dwTmpFlags)
{
  wchar_t wszFile[MAX_PATH];

  if (dwSaveMethod & SMET_NEAR)
  {
    if (*wpEditFile)
    {
      xprintfW(wszFile, L"%s.%Id.tmp", wpEditFile, hDocEdit);
      if (FileExistsWide(wszFile))
        DeleteBackupFile(wszFile, dwTmpFlags);
    }
  }
  if (dwSaveMethod & SMET_DIR)
  {
    if (*wszSaveDirExp)
    {
      xprintfW(wszFile, L"%s\\%s.%Id.tmp", wszSaveDirExp, GetFileName(wpEditFile, -1), hDocEdit);
      if (xstrcmpiW(wpEditFile, wszFile))
      {
        if (FileExistsWide(wszFile))
          DeleteBackupFile(wszFile, dwTmpFlags);
      }
    }
  }
}

int DeleteBackupFile(const wchar_t *wpFile, DWORD dwTmpFlags)
{
  if ((dwTmpFlags & REMC_DELETE) && (dwTmpFlags & REMC_TOBIN))
  {
    SHFILEOPSTRUCTW shfos;
    wchar_t wszFile[MAX_PATH];
    int nPathLen;

    nPathLen=(int)xstrcpynW(wszFile, wpFile, MAX_PATH);
    wszFile[nPathLen + 1]=L'\0';
    shfos.hwnd=hMainWnd;
    shfos.wFunc=FO_DELETE;
    shfos.pFrom=wszFile;
    shfos.pTo=NULL;
    shfos.fFlags=FOF_ALLOWUNDO|FOF_SILENT|FOF_NOCONFIRMATION;
    return SHFileOperationWide(&shfos);
  }
  return DeleteFileWide(wpFile);
}

void UncheckBOM(HWND hDlg)
{
  HWND hWndCodePage=GetDlgItem(hDlg, IDC_OFN_CODEPAGE);
  HWND hWndAutodetect=GetDlgItem(hDlg, IDC_OFN_AUTODETECT);
  int nCodePage=GetComboboxCodepage(hWndCodePage);
  BOOL bBOM=-1;

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

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
      return wpCount + 1;
  }
  return wpFile;
}

int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
    {
      --wpCount;
      break;
    }
  }
  ++wpCount;
  return (int)xstrcpynW(wszFileDir, wpFile, min(nFileDirMax, wpCount - wpFile + 1));
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
      WideOption(hOptions, L"Sessions", PO_DWORD, (LPBYTE)&dwSessions, sizeof(DWORD));
      WideOption(hOptions, L"TmpFile", PO_DWORD, (LPBYTE)&dwTmpFile, sizeof(DWORD));
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
      WideOption(hOptions, L"Sessions", PO_DWORD, (LPBYTE)&dwSessions, sizeof(DWORD));
      WideOption(hOptions, L"TmpFile", PO_DWORD, (LPBYTE)&dwTmpFile, sizeof(DWORD));
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
    if (nStringID == STRID_SESSIONS_GROUP)
      return L"Sessions \x043F\x043B\x0430\x0433\x0438\x043D (AutoSave.session)";
    if (nStringID == STRID_SESSIONS_SILENTEXIT)
      return L"\x041C\x043E\x043B\x0447\x0430\x0020\x0432\x044B\x0445\x043E\x0434\x0438\x0442\x044C\x002F\x0432\x043E\x0441\x0441\x0442\x0430\x043D\x0430\x0432\x043B\x0438\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_SESSIONS_SAVEFILE)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x0444\x0430\x0439\x043B\x0020\x0441\x0435\x0441\x0441\x0438\x0438";
    if (nStringID == STRID_TMPFILE)
      return L"\x0423\x0434\x0430\x043B\x0435\x043D\x0438\x0435\x002F\x0412\x043E\x0441\x0441\x0442\x0430\x043D\x043E\x0432\x043B\x0435\x043D\x0438\x0435\x0020\x002E\x0074\x006D\x0070\x0020\x0444\x0430\x0439\x043B\x0430";
    if (nStringID == STRID_TMPDELETE)
      return L"\x0423\x0434\x0430\x043B\x044F\x0442\x044C\x0020\x002E\x0074\x006D\x0070\x0020\x0444\x0430\x0439\x043B\x002C\x0020\x0435\x0441\x043B\x0438\x0020\x043E\x0440\x0438\x0433\x0438\x043D\x0430\x043B\x0020\x0437\x0430\x043A\x0440\x044B\x0442";
    if (nStringID == STRID_TMPTOBIN)
      return L"\x0423\x0434\x0430\x043B\x044F\x0442\x044C\x0020\x002E\x0074\x006D\x0070\x0020\x0444\x0430\x0439\x043B\x0020\x0432\x0020\x043A\x043E\x0440\x0437\x0438\x043D\x0443";
    if (nStringID == STRID_TMPRECOVER)
      return L"\x0412\x043E\x0441\x0441\x0442\x0430\x043D\x0430\x0432\x043B\x0438\x0432\x0430\x0442\x044C\x0020\x002E\x0074\x006D\x0070\x0020\x0444\x0430\x0439\x043B";
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
    if (nStringID == STRID_SESSIONS_GROUP)
      return L"Sessions plugin (AutoSave.session)";
    if (nStringID == STRID_SESSIONS_SILENTEXIT)
      return L"Silent exit/restore";
    if (nStringID == STRID_SESSIONS_SAVEFILE)
      return L"Save session file";
    if (nStringID == STRID_TMPFILE)
      return L"Delete/Restore .tmp file";
    if (nStringID == STRID_TMPDELETE)
      return L"Delete .tmp file, if original closed";
    if (nStringID == STRID_TMPTOBIN)
      return L"Delete .tmp file to recycle bin";
    if (nStringID == STRID_TMPRECOVER)
      return L"Recover .tmp file";
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

  if (nSaveDirExpLen=TranslateFileString(wszSaveDir, wszSaveDirExp, MAX_PATH))
    if (wszSaveDirExp[nSaveDirExpLen - 1] == L'\\')
      wszSaveDirExp[--nSaveDirExpLen]=L'\0';
  CreateDirectoryWide(wszSaveDirExp, NULL);
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
