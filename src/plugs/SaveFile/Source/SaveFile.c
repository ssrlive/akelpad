#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <shlobj.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


//Include stack functions
#define StackInsertBefore
#define StackMoveBefore
#define StackClear
#include "StackFunc.h"

/*
//Include string functions
#define WideCharLower
#define xarrlenW
#define xmemcpy
#define xstrlenW
#define xstrcpynW
#define xstrcmpiW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#define xnprintfW
#include "StrFunc.h"

//Include wide functions
#define CallWindowProcWide
#define ComboBox_AddStringWide
#define ComboBox_GetLBTextWide
#define CreateDirectoryWide
#define DeleteFileWide
#define DialogBoxWide
#define ExpandEnvironmentStringsWide
#define FileExistsWide
#define FindFirstFileWide
#define FindNextFileWide
#define GetDateFormatWide
#define GetDlgItemTextWide
#define GetFileAttributesWide
#define MoveFileWide
#define GetTimeFormatWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define ListBox_AddStringWide
#define ListView_GetItemWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define SHBrowseForFolderWide
#define SHFileOperationWide
#define SHGetPathFromIDListWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_SAVEMOMENT           1
#define STRID_SAVEINTERVAL         2
#define STRID_MIN                  3
#define STRID_SEC                  4
#define STRID_SAVEFOCUS            5
#define STRID_SAVEFRAME            6
#define STRID_SAVEMETHOD           7
#define STRID_SAVESIMPLE           8
#define STRID_SAVENEAR             9
#define STRID_SAVEDIR              10
#define STRID_SAVEDIRVARS          11
#define STRID_TMPFILE              12
#define STRID_TMPDELETE            13
#define STRID_TMPTOBIN             14
#define STRID_TMPRECOVER           15
#define STRID_SAVENOBOM            16
#define STRID_FORCENOBOM           17
#define STRID_DLGUNCHECK           18
#define STRID_RECOVER              19
#define STRID_NAME                 20
#define STRID_TIME                 21
#define STRID_FILE                 22
#define STRID_PLUGIN               23
#define STRID_YES                  24
#define STRID_NO                   25
#define STRID_OK                   26
#define STRID_CANCEL               27

#define OF_AUTOSAVE       0x1
#define OF_SAVENOBOM      0x2
#define OF_BACKUPRECT     0x4
#define OF_ALL         (OF_AUTOSAVE  |\
                        OF_SAVENOBOM |\
                        OF_BACKUPRECT)

//Save moment
#define SMOM_TIME    0x1
#define SMOM_FOCUS   0x2
#define SMOM_FRAME   0x4

//Save method
#define SMET_SIMPLE  0x1
#define SMET_NEAR    0x2
#define SMET_DIR     0x4

//Save interval type
#define SIT_MIN    0
#define SIT_SEC    1

//.tmp file
#define REMC_DELETE   0x1
#define REMC_TOBIN    0x2
#define REMC_RECOVER  0x4

//Backup list
#define LVI_NAME         0
#define LVI_TIME         1
#define LVI_FILEBACKUP   2

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
  wchar_t wszNameBackup[MAX_PATH];
  wchar_t wszFileBackup[MAX_PATH];
  wchar_t wszDeferFileBackup[MAX_PATH];
  wchar_t wszDeferFileBackupNoExt[MAX_PATH];
  wchar_t wszFileTime[MAX_PATH];
  __int64 nDiffSec;
  FILETIME ft;
  FRAMEDATA *lpFrame;
  BOOL bCheck;
} BACKUPFILE;

typedef struct {
  BACKUPFILE *first;
  BACKUPFILE *last;
} STACKBACKUPFILE;

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
LPARAM GetItemParam(HWND hWnd, int nIndex);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen);
BOOL GetFileWriteTimeWide(const wchar_t *wpFile, FILETIME *ft);
__int64 FileTimeDiffMS(const FILETIME *ft1, const FILETIME *ft2);

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
DWORD dwSaveFlags=0;
wchar_t wszSaveDir[MAX_PATH]=L"";
wchar_t wszSaveDirExp[MAX_PATH]=L"";
int nSaveDirExpLen;
UINT_PTR dwSaveTimer=0;
DWORD dwSaveMoment=SMOM_TIME;
DWORD dwSaveInterval=5;
DWORD dwSaveIntervalType=SIT_MIN;
DWORD dwSaveMethod=SMET_SIMPLE;
DWORD dwTmpFile=REMC_DELETE|REMC_TOBIN|REMC_RECOVER;
RECT rcBackupMinMaxDialog={410, 153, 0, 0};
RECT rcBackupCurrentDialog={0};
int nColumnWidth1=209;
int nColumnWidth2=120;
int nColumnWidth3=105;
STACKBACKUPFILE hBackupStack={0};
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

    if (bInitAutoSave && (dwTmpFile & REMC_RECOVER) && !(*nod->dwFlags & (OD_NOUPDATE|OD_REOPEN)) &&
        !SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)nod->wszFile))
    {
      wchar_t wszSearch[MAX_PATH];
      wchar_t wszBackupFile[MAX_PATH];
      wchar_t wszFileDir[MAX_PATH];
      const wchar_t *wpName;
      const wchar_t *wpBackupDir=NULL;
      const wchar_t *wpCount;
      const wchar_t *wpID;
      WIN32_FIND_DATAW wfd;
      FILETIME ftFrame;
      HANDLE hSearch;
      int nNameLen;
      int nDirLen;
      int nDeferFileBackup;
      DWORD dwSaveMethodOpen=dwSaveMethod;
      BACKUPFILE *lpBackupFile;
      BACKUPFILE *lpBackupCount;
      __int64 nDiff;

      if (!lpFrame->ft.dwHighDateTime && !lpFrame->ft.dwLowDateTime)
        GetFileWriteTimeWide(nod->wszFile, &ftFrame);
      else
        ftFrame=lpFrame->ft;
      wpName=GetFileName(nod->wszFile, -1);
      nNameLen=(int)xstrlenW(wpName);
      nDirLen=(int)GetFileDir(nod->wszFile, -1, wszFileDir, MAX_PATH);
      wszBackupFile[0]=L'\0';

      while (dwSaveMethodOpen & (SMET_NEAR|SMET_DIR))
      {
        if (dwSaveMethodOpen & SMET_DIR)
          wpBackupDir=wszSaveDirExp;
        else if (dwSaveMethodOpen & SMET_NEAR)
          wpBackupDir=wszFileDir;
        if (xnprintfW(wszSearch, MAX_PATH, L"%s\\%s.*.tmp", wpBackupDir, wpName))
        {
          if ((hSearch=FindFirstFileWide(wszSearch, &wfd)) != INVALID_HANDLE_VALUE)
          {
            do
            {
              //Is backup format
              wpID=wfd.cFileName + nNameLen + 1;
              if (*wpID == L'_') ++wpID;

              for (wpCount=wpID; *wpCount; ++wpCount)
              {
                if (!xstrcmpiW(wpCount, L".tmp"))
                {
                  if (!StackInsertBefore((stack **)&hBackupStack.first, (stack **)&hBackupStack.last, (stack *)NULL, (stack **)&lpBackupFile, sizeof(BACKUPFILE)))
                  {
                    xstrcpynW(lpBackupFile->wszName, wpName, MAX_PATH);
                    xstrcpynW(lpBackupFile->wszDir, wszFileDir, MAX_PATH);
                    xstrcpynW(lpBackupFile->wszFile, nod->wszFile, MAX_PATH);
                    xstrcpynW(lpBackupFile->wszNameBackup, wfd.cFileName, MAX_PATH);
                    xnprintfW(lpBackupFile->wszFileBackup, MAX_PATH, L"%s\\%s", wpBackupDir, wfd.cFileName);
                    if (nDeferFileBackup=(int)xnprintfW(lpBackupFile->wszDeferFileBackup, MAX_PATH, L"%s\\%s._%s", wpBackupDir, wpName, wpID))
                      xstrcpynW(lpBackupFile->wszDeferFileBackupNoExt, lpBackupFile->wszDeferFileBackup, nDeferFileBackup - 3);
                    else
                      lpBackupFile->wszDeferFileBackupNoExt[0]=L'\0';

                    //File time
                    {
                      wchar_t wszTime[128];
                      wchar_t wszDate[128];
                      FILETIME ft;
                      SYSTEMTIME st;

                      lpBackupFile->ft=wfd.ftLastWriteTime;
                      FileTimeToLocalFileTime(&lpBackupFile->ft, &ft);
                      FileTimeToSystemTime(&ft, &st);

                      GetTimeFormatWide(LOCALE_USER_DEFAULT, 0, &st, NULL, wszTime, 128);
                      GetDateFormatWide(LOCALE_USER_DEFAULT, 0, &st, NULL, wszDate, 128);
                      xprintfW(lpBackupFile->wszFileTime, L"%s %s", wszDate, wszTime);

                      lpBackupFile->nDiffSec=FileTimeDiffMS(&lpBackupFile->ft, &ftFrame) / 1000;
                    }
                    lpBackupFile->lpFrame=lpFrame;

                    for (lpBackupCount=hBackupStack.first; lpBackupCount; lpBackupCount=lpBackupCount->next)
                    {
                      if (lpBackupCount == lpBackupFile) continue;

                      if (!xstrcmpiW(lpBackupFile->wszName, lpBackupCount->wszName))
                      {
                        nDiff=FileTimeDiffMS(&lpBackupFile->ft, &lpBackupCount->ft);
                        if (nDiff < 0)
                          break;

                        if (lpBackupCount->bCheck)
                          lpBackupCount->bCheck=FALSE;
                      }
                    }
                    //Two second FAT accuracy
                    if (!lpBackupCount && lpBackupFile->nDiffSec > 2)
                      lpBackupFile->bCheck=TRUE;
                    StackMoveBefore((stack **)&hBackupStack.first, (stack **)&hBackupStack.last, (stack *)lpBackupFile, (stack *)lpBackupCount);
                  }
                  break;
                }
                if (*wpCount != L'_' && (*wpCount < L'0' || *wpCount > L'9'))
                  break;
              }
            }
            while (FindNextFileWide(hSearch, &wfd));

            FindClose(hSearch);
          }
        }
        if (dwSaveMethodOpen & SMET_DIR)
          dwSaveMethodOpen&=~SMET_DIR;
        else if (dwSaveMethodOpen & SMET_NEAR)
          dwSaveMethodOpen&=~SMET_NEAR;
      }
    }
  }
  else if (uMsg == AKDN_POSTDOCUMENT_FINISH)
  {
    if (bInitAutoSave && (dwTmpFile & REMC_RECOVER) && lParam == AKDN_OPENDOCUMENT_FINISH)
    {
      static BOOL bLock;

      //Lock processing AKDN_POSTDOCUMENT_FINISH from RecoverDlgProc
      if (!bLock)
      {
        bLock=TRUE;
        if (hBackupStack.first)
          DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_RECOVER), hMainWnd, (DLGPROC)RecoverDlgProc);
        StackClear((stack **)&hBackupStack.first, (stack **)&hBackupStack.last);
        bLock=FALSE;
      }
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
          RemoveBackupFile(wszFileNameOld, lpFrame->ei.hDocEdit, 0);
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
  static HWND hWndSaveIntervalType;
  static HWND hWndSaveFocusCheck;
  static HWND hWndSaveFrameCheck;
  static HWND hWndSaveSimpleCheck;
  static HWND hWndSaveNearCheck;
  static HWND hWndSaveDirCheck;
  static HWND hWndSaveDir;
  static HWND hWndSaveDirBrowse;
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
  static DWORD dwTmpFileDlg;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndAutoSaveTitle=GetDlgItem(hDlg, IDC_AUTOSAVE_TITLE);
    hWndSaveIntervalCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEINTERVAL_CHECK);
    hWndSaveInterval=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEINTERVAL);
    hWndSaveIntervalType=GetDlgItem(hDlg, IDC_AUTOSAVE_INTERVALTYPE_COMBO);
    hWndSaveFocusCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEFOCUS_CHECK);
    hWndSaveFrameCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEFRAME_CHECK);
    hWndSaveSimpleCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVESIMPLE_CHECK);
    hWndSaveNearCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVENEAR_CHECK);
    hWndSaveDirCheck=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEDIR_CHECK);
    hWndSaveDir=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEDIR);
    hWndSaveDirBrowse=GetDlgItem(hDlg, IDC_AUTOSAVE_SAVEDIR_BROWSE);
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
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEFOCUS_CHECK, GetLangStringW(wLangModule, STRID_SAVEFOCUS));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEFRAME_CHECK, GetLangStringW(wLangModule, STRID_SAVEFRAME));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_METHOD_GROUP, GetLangStringW(wLangModule, STRID_SAVEMETHOD));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVESIMPLE_CHECK, GetLangStringW(wLangModule, STRID_SAVESIMPLE));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVENEAR_CHECK, GetLangStringW(wLangModule, STRID_SAVENEAR));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEDIR_CHECK, GetLangStringW(wLangModule, STRID_SAVEDIR));
    SetDlgItemTextWide(hDlg, IDC_AUTOSAVE_SAVEDIR_NOTE, GetLangStringW(wLangModule, STRID_SAVEDIRVARS));
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

    ComboBox_AddStringWide(hWndSaveIntervalType, GetLangStringW(wLangModule, STRID_MIN));
    ComboBox_AddStringWide(hWndSaveIntervalType, GetLangStringW(wLangModule, STRID_SEC));
    SendMessage(hWndSaveIntervalType, CB_SETCURSEL, (WPARAM)dwSaveIntervalType, 0);

    dwSaveMethodDlg=dwSaveMethod;
    if (dwSaveMethodDlg & SMET_SIMPLE) SendMessage(hWndSaveSimpleCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMethodDlg & SMET_NEAR) SendMessage(hWndSaveNearCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveMethodDlg & SMET_DIR) SendMessage(hWndSaveDirCheck, BM_SETCHECK, BST_CHECKED, 0);

    dwTmpFileDlg=dwTmpFile;
    if (dwTmpFileDlg & REMC_DELETE) SendMessage(hWndTmpDelete, BM_SETCHECK, BST_CHECKED, 0);
    if (dwTmpFileDlg & REMC_TOBIN) SendMessage(hWndTmpToBin, BM_SETCHECK, BST_CHECKED, 0);
    if (dwTmpFileDlg & REMC_RECOVER) SendMessage(hWndTmpRecover, BM_SETCHECK, BST_CHECKED, 0);

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

    if (!dwSaveMethodNew)
      SendMessage(GetDlgItem(hDlg, wCommand), BM_SETCHECK, BST_CHECKED, 0);
    else
      dwSaveMethodDlg=dwSaveMethodNew;

    //Enable windows
    if (wCommand == IDC_AUTOSAVE_SAVEINTERVAL_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVEFOCUS_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVEFRAME_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVESIMPLE_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVENEAR_CHECK ||
        wCommand == IDC_AUTOSAVE_SAVEDIR_CHECK ||
        wCommand == IDC_AUTOSAVE_TMPDELETE_CHECK)
    {
      EnableWindow(hWndSaveSimpleCheck, dwSaveMomentDlg);
      EnableWindow(hWndSaveNearCheck, dwSaveMomentDlg);
      EnableWindow(hWndSaveDirCheck, dwSaveMomentDlg);
      EnableWindow(hWndSaveDir, (dwSaveMomentDlg && (dwSaveMethodDlg & SMET_DIR)));
      EnableWindow(hWndSaveDirBrowse, (dwSaveMomentDlg && (dwSaveMethodDlg & SMET_DIR)));
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
      dwSaveInterval=GetDlgItemInt(hDlg, IDC_AUTOSAVE_SAVEINTERVAL, NULL, FALSE);
      dwSaveIntervalType=(DWORD)SendMessage(hWndSaveIntervalType, CB_GETCURSEL, 0, 0);

      GetWindowTextWide(hWndSaveDir, wszSaveDir, MAX_PATH);
      if (nSaveDirExpLen=TranslateFileString(wszSaveDir, wszSaveDirExp, MAX_PATH))
        if (wszSaveDirExp[nSaveDirExpLen - 1] == L'\\')
          wszSaveDirExp[--nSaveDirExpLen]=L'\0';
      CreateDirectoryWide(wszSaveDirExp, NULL);
      dwSaveFlags|=OF_AUTOSAVE;

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
      dwSaveFlags|=OF_SAVENOBOM;

      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }
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
  static HWND hWndYes;
  static HWND hWndNo;
  static HWND hWndCancel;
  BACKUPFILE *lpBackupFile;
  static RESIZEDIALOG rds[]={{&hWndList,     RDS_SIZE|RDS_X, 0},
                             {&hWndList,     RDS_SIZE|RDS_Y, 0},
                             {&hWndYes,      RDS_MOVE|RDS_X, 0},
                             {&hWndYes,      RDS_MOVE|RDS_Y, 0},
                             {&hWndNo,       RDS_MOVE|RDS_X, 0},
                             {&hWndNo,       RDS_MOVE|RDS_Y, 0},
                             {&hWndCancel,   RDS_MOVE|RDS_X, 0},
                             {&hWndCancel,   RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvc;
    LVITEMW lvi;
    int nIndex;

    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);


    hWndList=GetDlgItem(hDlg, IDC_RECOVER_LIST);
    hWndYes=GetDlgItem(hDlg, IDYES);
    hWndNo=GetDlgItem(hDlg, IDNO);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_RECOVER_LABEL, GetLangStringW(wLangModule, STRID_RECOVER));
    SetDlgItemTextWide(hDlg, IDYES, GetLangStringW(wLangModule, STRID_YES));
    SetDlgItemTextWide(hDlg, IDNO, GetLangStringW(wLangModule, STRID_NO));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES);

    //Columns
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_NAME);
    lvc.cx=nColumnWidth1;
    lvc.iSubItem=LVI_NAME;
    ListView_InsertColumnWide(hWndList, LVI_NAME, &lvc);

    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_TIME);
    lvc.cx=nColumnWidth2;
    lvc.iSubItem=LVI_TIME;
    ListView_InsertColumnWide(hWndList, LVI_TIME, &lvc);

    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_FILE);
    lvc.cx=nColumnWidth3;
    lvc.iSubItem=LVI_FILEBACKUP;
    ListView_InsertColumnWide(hWndList, LVI_FILEBACKUP, &lvc);

    for (lpBackupFile=hBackupStack.first; lpBackupFile; lpBackupFile=lpBackupFile->next)
    {
      lvi.mask=LVIF_TEXT|LVIF_PARAM;
      lvi.pszText=lpBackupFile->wszNameBackup;
      lvi.iItem=0x7FFFFFFF;
      lvi.iSubItem=LVI_NAME;
      lvi.lParam=(LPARAM)lpBackupFile;
      nIndex=ListView_InsertItemWide(hWndList, &lvi);

      if (lpBackupFile->bCheck)
      {
        lvi.mask=LVIF_STATE;
        lvi.iItem=nIndex;
        lvi.iSubItem=LVI_NAME;
        lvi.state=((TRUE + 1) << 12);
        lvi.stateMask=LVIS_STATEIMAGEMASK;
        ListView_SetItemWide(hWndList, &lvi);
      }

      lvi.mask=LVIF_TEXT;
      lvi.pszText=lpBackupFile->wszFileTime;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVI_TIME;
      ListView_SetItemWide(hWndList, &lvi);

      lvi.mask=LVIF_TEXT;
      lvi.pszText=lpBackupFile->wszFileBackup;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVI_FILEBACKUP;
      ListView_SetItemWide(hWndList, &lvi);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    WORD wCommand=LOWORD(wParam);

    if (wCommand == IDYES ||
        wCommand == IDNO ||
        wCommand == IDCANCEL)
    {
      int nWidth;

      nWidth=(int)SendMessage(hWndList, LVM_GETCOLUMNWIDTH, LVI_NAME, 0);
      if (nColumnWidth1 != nWidth)
      {
        nColumnWidth1=nWidth;
        dwSaveFlags|=OF_BACKUPRECT;
      }
      nWidth=(int)SendMessage(hWndList, LVM_GETCOLUMNWIDTH, LVI_TIME, 0);
      if (nColumnWidth2 != nWidth)
      {
        nColumnWidth2=nWidth;
        dwSaveFlags|=OF_BACKUPRECT;
      }
      nWidth=(int)SendMessage(hWndList, LVM_GETCOLUMNWIDTH, LVI_FILEBACKUP, 0);
      if (nColumnWidth3 != nWidth)
      {
        nColumnWidth3=nWidth;
        dwSaveFlags|=OF_BACKUPRECT;
      }
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }
      EndDialog(hDlg, 0);

      if (wCommand == IDYES || wCommand == IDNO)
      {
        BACKUPFILE *lpBackupFile;

        if (wCommand == IDYES)
        {
          for (lpBackupFile=hBackupStack.first; lpBackupFile; lpBackupFile=lpBackupFile->next)
          {
            if (lpBackupFile->bCheck)
            {
              OPENDOCUMENTW od;

              od.pFile=lpBackupFile->wszFileBackup;
              od.pWorkDir=NULL;
              od.dwFlags=OD_ADT_BINARYERROR|OD_NOUPDATE;
              od.nCodePage=lpBackupFile->lpFrame->ei.nCodePage;
              od.bBOM=lpBackupFile->lpFrame->ei.bBOM;
              od.hDoc=lpBackupFile->lpFrame->ei.hDocEdit;
              SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)lpBackupFile->lpFrame->ei.hWndEdit, (LPARAM)&od);
            }
          }
        }
        for (lpBackupFile=hBackupStack.first; lpBackupFile; lpBackupFile=lpBackupFile->next)
        {
          DeleteBackupFile(lpBackupFile->wszFileBackup, dwTmpFile);
        }
      }
      else
      {
        //Rename files
        for (lpBackupFile=hBackupStack.first; lpBackupFile; lpBackupFile=lpBackupFile->next)
        {
          if (xstrcmpiW(lpBackupFile->wszDeferFileBackup, lpBackupFile->wszFileBackup))
          {
            if (!FileExistsWide(lpBackupFile->wszDeferFileBackup))
              MoveFileWide(lpBackupFile->wszFileBackup, lpBackupFile->wszDeferFileBackup);
            else
            {
              wchar_t wszDeferFileBackup[MAX_PATH];
              int i;

              for (i=2; i < 1000; ++i)
              {
                if (xnprintfW(wszDeferFileBackup, MAX_PATH, L"%s_%d.tmp", lpBackupFile->wszDeferFileBackupNoExt, i))
                {
                  if (!FileExistsWide(wszDeferFileBackup))
                  {
                    MoveFileWide(lpBackupFile->wszFileBackup, wszDeferFileBackup);
                    break;
                  }
                }
                else break;
              }
            }
          }
        }
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_RECOVER_LIST)
    {
      NMLISTVIEW *pnmlv=(NMLISTVIEW *)lParam;

      if (pnmlv->hdr.code == LVN_ITEMCHANGED)
      {
        if (pnmlv->uNewState & LVIS_STATEIMAGEMASK)
        {
          BACKUPFILE *lpBackupFile=(BACKUPFILE *)pnmlv->lParam;
          BACKUPFILE *lpBackupItem;
          BOOL bNewState=((pnmlv->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          BOOL bOldState=((pnmlv->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          static BOOL bLock;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            if (!bLock)
            {
              LVITEMW lvi;
              int nIndex;
              int nMaxIndex=(int)SendMessage(pnmlv->hdr.hwndFrom, LVM_GETITEMCOUNT, 0, 0);

              //Allow only one selected item in group
              bLock=TRUE;
              lpBackupFile->bCheck=bNewState;

              for (nIndex=0; nIndex < nMaxIndex; ++nIndex)
              {
                lpBackupItem=(BACKUPFILE *)GetItemParam(pnmlv->hdr.hwndFrom, nIndex);
                if (lpBackupItem == lpBackupFile) continue;

                if (lpBackupItem && lpBackupItem->bCheck && !xstrcmpiW(lpBackupItem->wszFile, lpBackupFile->wszFile))
                {
                  lpBackupItem->bCheck=FALSE;

                  lvi.mask=LVIF_STATE;
                  lvi.iItem=nIndex;
                  lvi.iSubItem=LVI_NAME;
                  lvi.state=((FALSE + 1) << 12);
                  lvi.stateMask=LVIS_STATEIMAGEMASK;
                  ListView_SetItemWide(pnmlv->hdr.hwndFrom, &lvi);
                }
              }
              bLock=FALSE;
            }
          }
        }
      }
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

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcBackupMinMaxDialog, &rcBackupCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      dwSaveFlags|=OF_BACKUPRECT;
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
  const wchar_t *wpExt;
  BOOL bResult=TRUE;

  if (lpFrame->ei.bModified)
  {
    if ((wpExt=GetFileExt(lpFrame->ei.wszFile, -1)) && !xstrcmpiW(L"tmp", wpExt))
      return bResult;

    if (dwSaveMethod & SMET_SIMPLE)
    {
      if (*lpFrame->ei.wszFile)
      {
        if (SendMessage(hMainWnd, AKD_FRAMESTATS, FWS_COUNTFILE, (LPARAM)lpFrame->ei.wszFile) <= 1)
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
    }
    if (dwSaveMethod & SMET_NEAR)
    {
      if (*lpFrame->ei.wszFile)
      {
        if (xnprintfW(wszSaveFile, MAX_PATH, L"%s.%Id.tmp", lpFrame->ei.wszFile, lpFrame->ei.hDocEdit))
        {
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
    if (dwSaveMethod & SMET_DIR)
    {
      if (*wszSaveDirExp)
      {
        if (xnprintfW(wszSaveFile, MAX_PATH, L"%s\\%s.%Id.tmp", wszSaveDirExp, GetFileName(lpFrame->ei.wszFile, -1), lpFrame->ei.hDocEdit))
        {
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
      if (xnprintfW(wszFile, MAX_PATH, L"%s.%Id.tmp", wpEditFile, hDocEdit))
      {
        if (FileExistsWide(wszFile))
          DeleteBackupFile(wszFile, dwTmpFlags);
      }
    }
  }
  if (dwSaveMethod & SMET_DIR)
  {
    if (*wszSaveDirExp)
    {
      if (xnprintfW(wszFile, MAX_PATH, L"%s\\%s.%Id.tmp", wszSaveDirExp, GetFileName(wpEditFile, -1), hDocEdit))
      {
        if (xstrcmpiW(wpEditFile, wszFile))
        {
          if (FileExistsWide(wszFile))
            DeleteBackupFile(wszFile, dwTmpFlags);
        }
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

LPARAM GetItemParam(HWND hWnd, int nIndex)
{
  LVITEMW lvi;

  if (nIndex == -1)
    return 0;
  lvi.mask=LVIF_PARAM;
  lvi.iItem=nIndex;
  lvi.iSubItem=0;
  lvi.lParam=0;
  ListView_GetItemWide(hWnd, &lvi);

  return lvi.lParam;
}

int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%a -AkelPad directory, %% -%
  wchar_t *wpExeDir=wszExeDir;
  wchar_t *wszSource;
  wchar_t *wpSource;
  wchar_t *wpTarget=wszBuffer;
  wchar_t *wpTargetMax=(wszBuffer ? (wszBuffer + nBufferSize) : (wchar_t *)MAXUINT_PTR);
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

BOOL GetFileWriteTimeWide(const wchar_t *wpFile, FILETIME *ft)
{
  WIN32_FIND_DATAW wfd;
  HANDLE hFile;

  if ((hFile=FindFirstFileWide(wpFile, &wfd)) != INVALID_HANDLE_VALUE)
  {
    *ft=wfd.ftLastWriteTime;
    FindClose(hFile);
    return TRUE;
  }
  return FALSE;
}

__int64 FileTimeDiffMS(const FILETIME *ft1, const FILETIME *ft2)
{
  ULARGE_INTEGER ul1;
  ULARGE_INTEGER ul2;

  ul1.HighPart=ft1->dwHighDateTime;
  ul1.LowPart=ft1->dwLowDateTime;
  ul2.HighPart=ft2->dwHighDateTime;
  ul2.LowPart=ft2->dwLowDateTime;
  return (__int64)(ul1.QuadPart - ul2.QuadPart) / 10000;
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
      WideOption(hOptions, L"SaveIntervalType", PO_DWORD, (LPBYTE)&dwSaveIntervalType, sizeof(DWORD));
      WideOption(hOptions, L"SaveMethod", PO_DWORD, (LPBYTE)&dwSaveMethod, sizeof(DWORD));
      WideOption(hOptions, L"SaveDir", PO_STRING, (LPBYTE)wszSaveDir, MAX_PATH * sizeof(wchar_t));
      WideOption(hOptions, L"TmpFile", PO_DWORD, (LPBYTE)&dwTmpFile, sizeof(DWORD));

      //Dialog rectangle
      WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcBackupCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));
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
      WideOption(hOptions, L"SaveIntervalType", PO_DWORD, (LPBYTE)&dwSaveIntervalType, sizeof(DWORD));
      WideOption(hOptions, L"SaveMethod", PO_DWORD, (LPBYTE)&dwSaveMethod, sizeof(DWORD));
      WideOption(hOptions, L"SaveDir", PO_STRING, (LPBYTE)wszSaveDir, (lstrlenW(wszSaveDir) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"TmpFile", PO_DWORD, (LPBYTE)&dwTmpFile, sizeof(DWORD));
    }
    if (dwFlags & OF_BACKUPRECT)
    {
      WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcBackupCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));
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
    if (nStringID == STRID_SEC)
      return L"\x0441\x0435\x043A.";
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
    if (nStringID == STRID_RECOVER)
      return L"\x0412\x043E\x0441\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C:";
    if (nStringID == STRID_NAME)
      return L"\x0418\x043C\x044F";
    if (nStringID == STRID_TIME)
      return L"\x0412\x0440\x0435\x043C\x044F";
    if (nStringID == STRID_FILE)
      return L"\x0424\x0430\x0439\x043B";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_YES)
      return L"\x0414\x0430";
    if (nStringID == STRID_NO)
      return L"\x041D\x0435\x0442";
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
    if (nStringID == STRID_SEC)
      return L"sec.";
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
    if (nStringID == STRID_RECOVER)
      return L"Recover:";
    if (nStringID == STRID_NAME)
      return L"Name";
    if (nStringID == STRID_TIME)
      return L"Time";
    if (nStringID == STRID_FILE)
      return L"File";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_YES)
      return L"Yes";
    if (nStringID == STRID_NO)
      return L"No";
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
    dwSaveTimer=SetTimer(NULL, 0, dwSaveInterval * (dwSaveIntervalType == SIT_SEC ? 1000 : 60000), (TIMERPROC)TimerProc);
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
