#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"

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
#define WideCharUpper
#define xmemset
#define xmemcpy
#define xarrlenA
#define xarrlenW
#define xstrlenA
#define xstrlenW
#define xstrcpyW
#define xstrcpynA
#define xstrcpynW
#define xstrcmpiW
#define xprintfA
#define xprintfW
#define xatoiA
#define xatoiW
#define xitoaA
#define xitoaW
#define xuitoaA
#define xuitoaW
#define dec2hexA
#define dec2hexW
#include "StrFunc.h"

//Include wide functions
#define CreateDialogWide
#define CreateDirectoryWide
#define CreateFileWide
#define DeleteFileWide
#define DialogBoxParamWide
#define DialogBoxWide
#define DispatchMessageWide
#define FileExistsWide
#define FindFirstFileWide
#define FindNextFileWide
#define GetFileAttributesWide
#define GetKeyNameTextWide
#define GetMessageWide
#define GetSaveFileNameWide
#define GetWindowLongPtrWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define IsDialogMessageWide
#define ListView_GetItemWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define OpenEventWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#include "WideFunc.h"

//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"
//*/


//Defines
#define DLLA_MACROS_EXEC               1
#define DLLA_MACROS_RECORDSTOPTOOLBAR  2
#define DLLA_MACROS_PLAYTOENDTOOLBAR   3
#define DLLA_MACROS_GETLASTERROR       4

#define STRID_HOTKEY_EXISTS     1
#define STRID_CANNOT_OPENFILE   2
#define STRID_CANNOT_HOOK       3
#define STRID_ALREADY_EXIST     4
#define STRID_CONFIRM_DELETE    5
#define STRID_THREAD_BUSY       6
#define STRID_NORECORD          7
#define STRID_MACRO             8
#define STRID_HOTKEY            9
#define STRID_RECORD_LISTITEM   10
#define STRID_RECORD            11
#define STRID_SAVE              12
#define STRID_DELETE            13
#define STRID_ASSIGN            14
#define STRID_VIEW              15
#define STRID_KEY               16
#define STRID_ACT               17
#define STRID_EXPORT            18
#define STRID_PLUGIN            19
#define STRID_CLOSE             20

#define AKDLL_UPDATE (WM_USER + 100)

#define OF_HOTKEYS     0x1
#define OF_RECT        0x2
#define OF_VIEWRECT    0x4

#define LVI_MACRO_FILE          0
#define LVI_MACRO_HOTKEY        1

#define LVI_VIEW_KEY            0
#define LVI_VIEW_ACT            1

#define MACRO_TIMEOUT           5000

//Ctrl+End
#define CE_RELEASED       0
#define CE_CTRLDOWN       1
#define CE_CTRLENDDOWN    2
#define CE_CTRLENDFINISH  3

//Macro state and MacroPlay return value
#define MS_IDLE           0x0000
#define MS_PLAYING        0x0001
#define MS_NOEDIT         0x0002
#define MS_READONLY       0x0004
#define MS_WAITTIMEOUT    0x0008
#define MS_TOOLBARSTOP    0x0010
#define MS_REPEATLIMIT    0x0020
#define MS_EOFRICHED      0x0040
#define MS_SEARCHENDED    0x0080
#define MS_FRAMENOWINDOWS 0x0100
#define MS_PROGRAMEXIT    0x0200
#define MS_INITIALIZING   0x1000

//Exec macro flags
#define EMF_SCRIPTSNOSYNC 0x1

#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
  #define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#endif
#ifndef LVS_EX_GRIDLINES
  #define LVS_EX_GRIDLINES 0x00000001
#endif
#ifndef LVS_EX_FULLROWSELECT
  #define LVS_EX_FULLROWSELECT 0x00000020
#endif

typedef struct {
  const wchar_t *wpMacro;
  int nRepeat;
  DWORD dwFlags;
  HANDLE hInitMutex;
} EXECMACRO;

typedef struct _KEYACT {
  BYTE bVk;       //Virtual-key code
  DWORD dwFlags;  //KEYEVENTF_*  for keybd_event
} KEYACT;

typedef struct _KEYSTRUCT {
  struct _KEYSTRUCT *next;
  struct _KEYSTRUCT *prev;
  KEYACT ka;
} KEYSTRUCT;

typedef struct {
  KEYSTRUCT *first;
  KEYSTRUCT *last;
} STACKKEY;

typedef struct {
  const wchar_t *wpMacro;
  STACKKEY *lpStack;
} MACROFILE;

//Functions prototypes
DWORD WINAPI ThreadProc(LPVOID lpParameter);
LRESULT CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK StopDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ViewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillMacroList(HWND hWnd);
void FillKeyactList(HWND hWnd, STACKKEY *hStack);
INT_PTR FillSendKeys(STACKKEY *hStack, wchar_t *wszSendKeys);
BOOL IsSingleKey(KEYSTRUCT *lpElement);
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);
BOOL RegisterHotkey(wchar_t *wszMacroName, WORD wHotkey);
BOOL CALLBACK HotkeyProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
DWORD WINAPI ExecThreadProc(LPVOID lpParameter);
KEYSTRUCT* StackHotkeyAdd(STACKKEY *hStack, BYTE bVk, DWORD dwFlags);
void StackHotkeyPress(STACKKEY *hStack, BOOL bToEnd, DWORD dwFlags);
BOOL StackHotkeyRead(STACKKEY *hStack, HANDLE hFile);
BOOL StackHotkeySave(STACKKEY *hStack, HANDLE hFile);
void StackHotkeyFree(STACKKEY *hStack);
BOOL ReadMacroFile(STACKKEY *hKeyStack, wchar_t *wpMacro);
BOOL SaveMacroFile(STACKKEY *hRecordStack, wchar_t *wpMacro);
BOOL DeleteMacroFile(wchar_t *wpMacro);
void MacroRecord();
void MacroStop();
DWORD MacroPlay(STACKKEY *hStack, int nRepeat, DWORD dwFlags);
BOOL WaitIdle();
void ExecMacro(const wchar_t *wpMacro, int nRepeat, DWORD dwFlags);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GetHotkeyString(WORD wHotkey, wchar_t *wszString);
int MoveListViewItem(HWND hWnd, int nOldIndex, int nNewIndex);
BOOL IsCaretAtLastLine();
BOOL IsCaretAtLastEmptyLine();
void PosWindowToCorner(HWND hWndOwner, HWND hWndChild);
int GetBaseName(const wchar_t *wpFile, int nFileLen, wchar_t *wszBaseName, int nBaseNameMax);

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
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
BOOL bOldWindows;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;
char szMutexInitName[]="AkelPad::Macros::MutexInit";
char szMutexExecName[]="AkelPad::Macros::MutexExec";
char szMacrosDir[MAX_PATH];
wchar_t wszMacrosDir[MAX_PATH];
wchar_t wszScriptsExecMutex[MAX_PATH];
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszLastMacro[MAX_PATH]=L"";
STACKKEY hRecordStack={0};
STACKKEY hLastMacroStack={0};
HWND hWndMainDlg=NULL;
HWND hWndStopDlg=NULL;
HWND hWndMacrosList=NULL;
RECT rcMainMinMaxDialog={253, 352, 0, 0};
RECT rcMainCurrentDialog={0};
RECT rcViewMainMinMaxDialog={253, 352, 0, 0};
RECT rcViewMainCurrentDialog={0};
int nColumnWidth1=160;
int nColumnWidth2=109;
int nViewColumnWidth1=160;
int nViewColumnWidth2=109;
HHOOK hHook=NULL;
HANDLE hDialogThread=NULL;
HANDLE hExecThread=NULL;
DWORD dwMainProcessId=0;
DWORD dwMainThreadId=0;
DWORD dwDialogThreadId=0;
DWORD dwExecThreadId=0;
BOOL bRecord=FALSE;
DWORD dwMacroState=MS_IDLE;
int nCtrlEnd=CE_RELEASED;

HWND hWndToolbar=NULL;
int nToolBarCmdID=0;
HIMAGELIST hToolBarImageList=NULL;
int nToolBarIndexPlayToEnd=0;
HICON hToolBarIconPlayToEnd=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Macros";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);
  if (!bInitMain) InitMain();

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_MACROS_EXEC)
    {
      unsigned char *pMacro=NULL;
      int nRepeat=1;
      DWORD dwFlags=0;

      if (IsExtCallParamValid(pd->lParam, 2))
        pMacro=(unsigned char *)GetExtCallParam(pd->lParam, 2);
      if (IsExtCallParamValid(pd->lParam, 3))
        nRepeat=(int)GetExtCallParam(pd->lParam, 3);
      if (IsExtCallParamValid(pd->lParam, 4))
        dwFlags=(DWORD)GetExtCallParam(pd->lParam, 4);

      if (pMacro)
      {
        if (!bRecord && !(dwMacroState & MS_PLAYING))
        {
          wchar_t wszMacro[MAX_PATH];

          if (pd->dwSupport & PDS_STRANSI)
            xprintfW(wszMacro, L"%S", pMacro);
          else
            xprintfW(wszMacro, L"%s", pMacro);

          if (wszMacro[0] || hRecordStack.last)
          {
            ExecMacro(wszMacro, nRepeat, dwFlags);
          }
          else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_NORECORD), wszPluginTitle, MB_OK|MB_ICONINFORMATION);
        }
      }
    }
    else if (nAction == DLLA_MACROS_RECORDSTOPTOOLBAR ||
             nAction == DLLA_MACROS_PLAYTOENDTOOLBAR)
    {
      unsigned char *pToolBarHandle=NULL;
      unsigned char *pToolBarItemID=NULL;
      int nToolBarIconWidth;
      int nToolBarIconHeight;

      if (IsExtCallParamValid(pd->lParam, 2))
        pToolBarHandle=(unsigned char *)GetExtCallParam(pd->lParam, 2);
      if (IsExtCallParamValid(pd->lParam, 3))
        pToolBarItemID=(unsigned char *)GetExtCallParam(pd->lParam, 3);

      if (pToolBarHandle && pToolBarItemID)
      {
        if (pd->dwSupport & PDS_STRANSI)
        {
          hWndToolbar=(HWND)xatoiA((char *)pToolBarHandle, NULL);
          nToolBarCmdID=(int)xatoiA((char *)pToolBarItemID, NULL);
        }
        else
        {
          hWndToolbar=(HWND)xatoiW((wchar_t *)pToolBarHandle, NULL);
          nToolBarCmdID=(int)xatoiW((wchar_t *)pToolBarItemID, NULL);
        }
        hToolBarImageList=(HIMAGELIST)SendMessage(hWndToolbar, TB_GETIMAGELIST, 0, 0);
        ImageList_GetIconSize(hToolBarImageList, &nToolBarIconWidth, &nToolBarIconHeight);

        if (nAction == DLLA_MACROS_RECORDSTOPTOOLBAR)
        {
          if (!(dwMacroState & MS_PLAYING))
          {
            static int nButtonIndex;
            static HICON hIconRecord;
            HICON hIconStop;

            if (!bRecord)
            {
              //Change icon from record to stop
              nButtonIndex=(int)SendMessage(hWndToolbar, TB_GETBITMAP, nToolBarCmdID, 0);
              hIconRecord=ImageList_GetIcon(hToolBarImageList, nButtonIndex, ILD_NORMAL);
              hIconStop=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_STOP), IMAGE_ICON, nToolBarIconWidth, nToolBarIconHeight, 0);
              ImageList_ReplaceIcon(hToolBarImageList, nButtonIndex, hIconStop);
              DestroyIcon(hIconStop);

              //Start record
              StackHotkeyFree(&hRecordStack);
              MacroRecord();
              bRecord=TRUE;
            }
            else
            {
              //Stop record
              bRecord=FALSE;
              MacroStop();

              //Change icon from stop to record
              ImageList_ReplaceIcon(hToolBarImageList, nButtonIndex, hIconRecord);
              DestroyIcon(hIconRecord);
            }
          }
        }
        else if (nAction == DLLA_MACROS_PLAYTOENDTOOLBAR)
        {
          if (!bRecord)
          {
            HICON hIconStopToEnd;

            if (!(dwMacroState & MS_PLAYING))
            {
              if (hRecordStack.last)
              {
                //Change icon from play end to stop end
                nToolBarIndexPlayToEnd=(int)SendMessage(hWndToolbar, TB_GETBITMAP, nToolBarCmdID, 0);
                hToolBarIconPlayToEnd=ImageList_GetIcon(hToolBarImageList, nToolBarIndexPlayToEnd, ILD_NORMAL);
                hIconStopToEnd=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_STOPTOEND), IMAGE_ICON, nToolBarIconWidth, nToolBarIconHeight, 0);
                ImageList_ReplaceIcon(hToolBarImageList, nToolBarIndexPlayToEnd, hIconStopToEnd);
                DestroyIcon(hIconStopToEnd);

                ExecMacro(L"", 0, 0);
              }
              else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_NORECORD), wszPluginTitle, MB_OK|MB_ICONINFORMATION);
            }
            else
            {
              //Stop playing
              nCtrlEnd=CE_RELEASED;
              dwMacroState=(dwMacroState & ~MS_PLAYING)|MS_TOOLBARSTOP;

              //Change icon from stop end to play end
              ImageList_ReplaceIcon(hToolBarImageList, nToolBarIndexPlayToEnd, hToolBarIconPlayToEnd);
              DestroyIcon(hToolBarIconPlayToEnd);
              hToolBarIconPlayToEnd=NULL;
            }
          }
        }
      }
    }
    else if (nAction == DLLA_MACROS_GETLASTERROR)
    {
      DWORD *lpdwLastError=NULL;

      if (IsExtCallParamValid(pd->lParam, 2))
        lpdwLastError=(DWORD *)GetExtCallParam(pd->lParam, 2);

      if (lpdwLastError)
      {
        *lpdwLastError=dwMacroState;
      }
    }
  }
  else
  {
    if (!pd->bOnStart)
    {
      if (!hDialogThread)
      {
        hDialogThread=CreateThread(NULL, 0, ThreadProc, NULL, 0, &dwDialogThreadId);
      }
    }
  }

  //Stay in memory, and show as active
  pd->nUnload=UD_NONUNLOAD_ACTIVE;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
  MSG msg;
  BOOL bMsgStatus;

  hWndMainDlg=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_MACROS), hMainWnd, (DLGPROC)MainDlgProc);

  if (hWndMainDlg)
  {
    while ((bMsgStatus=GetMessageWide(&msg, NULL, 0, 0)) && bMsgStatus != -1)
    {
      if (!IsDialogMessageWide(hWndMainDlg, &msg))
      {
        TranslateMessage(&msg);
        DispatchMessageWide(&msg);
      }
    }
  }
  if (hDialogThread)
  {
    CloseHandle(hDialogThread);
    hDialogThread=NULL;
  }
  return 0;
}

LRESULT CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndPlayButton;
  static HWND hWndPlayX;
  static HWND hWndRepeat;
  static HWND hWndRepeatSpin;
  static HWND hWndActionsGroup;
  static HWND hWndRecordButton;
  static HWND hWndName;
  static HWND hWndSaveButton;
  static HWND hWndDeleteButton;
  static HWND hWndHotkey;
  static HWND hWndAssignButton;
  static HWND hWndViewButton;
  static HWND hWndCloseButton;
  static HICON hPlayIcon;
  static int nSelItem=-1;
  static BOOL bListChanged=FALSE;
  static RESIZEDIALOG rds[]={{&hWndMacrosList,   RDS_SIZE|RDS_X, 0},
                             {&hWndMacrosList,   RDS_SIZE|RDS_Y, 0},
                             {&hWndPlayButton,   RDS_MOVE|RDS_X, 0},
                             {&hWndPlayX,        RDS_MOVE|RDS_X, 0},
                             {&hWndRepeat,       RDS_MOVE|RDS_X, 0},
                             {&hWndRepeatSpin,   RDS_MOVE|RDS_X, 0},
                             {&hWndActionsGroup, RDS_MOVE|RDS_X, 0},
                             {&hWndRecordButton, RDS_MOVE|RDS_X, 0},
                             {&hWndName,         RDS_MOVE|RDS_X, 0},
                             {&hWndSaveButton,   RDS_MOVE|RDS_X, 0},
                             {&hWndDeleteButton, RDS_MOVE|RDS_X, 0},
                             {&hWndHotkey,       RDS_MOVE|RDS_X, 0},
                             {&hWndAssignButton, RDS_MOVE|RDS_X, 0},
                             {&hWndViewButton,   RDS_MOVE|RDS_X, 0},
                             {&hWndCloseButton,  RDS_MOVE|RDS_X, 0},
                             {&hWndCloseButton,  RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvc;

    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hPluginIcon);

    hWndMacrosList=GetDlgItem(hDlg, IDC_MACROS_LIST);
    hWndPlayButton=GetDlgItem(hDlg, IDC_PLAY);
    hWndPlayX=GetDlgItem(hDlg, IDC_PLAY_X);
    hWndRepeat=GetDlgItem(hDlg, IDC_REPEAT);
    hWndRepeatSpin=GetDlgItem(hDlg, IDC_REPEAT_SPIN);
    hWndActionsGroup=GetDlgItem(hDlg, IDC_ACTIONS_GROUP);
    hWndRecordButton=GetDlgItem(hDlg, IDC_RECORD);
    hWndName=GetDlgItem(hDlg, IDC_NAME);
    hWndSaveButton=GetDlgItem(hDlg, IDC_SAVE);
    hWndDeleteButton=GetDlgItem(hDlg, IDC_DELETE);
    hWndHotkey=GetDlgItem(hDlg, IDC_HOTKEY);
    hWndAssignButton=GetDlgItem(hDlg, IDC_ASSIGN);
    hWndViewButton=GetDlgItem(hDlg, IDC_VIEW);
    hWndCloseButton=GetDlgItem(hDlg, IDC_CLOSE);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_RECORD, GetLangStringW(wLangModule, STRID_RECORD));
    SetDlgItemTextWide(hDlg, IDC_SAVE, GetLangStringW(wLangModule, STRID_SAVE));
    SetDlgItemTextWide(hDlg, IDC_DELETE, GetLangStringW(wLangModule, STRID_DELETE));
    SetDlgItemTextWide(hDlg, IDC_ASSIGN, GetLangStringW(wLangModule, STRID_ASSIGN));
    SetDlgItemTextWide(hDlg, IDC_VIEW, GetLangStringW(wLangModule, STRID_VIEW));
    SetDlgItemTextWide(hDlg, IDC_CLOSE, GetLangStringW(wLangModule, STRID_CLOSE));

    //Set play button icon
    {
      BUTTONDRAW bd;

      hPlayIcon=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLAY), IMAGE_ICON, 16, 16, 0);
      bd.dwFlags=BIF_ICON|BIF_ETCHED|BIF_ENABLEFOCUS;
      bd.hImage=hPlayIcon;
      bd.nImageWidth=16;
      bd.nImageHeight=16;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndPlayButton, (LPARAM)&bd);
    }

    SendMessage(hWndRepeatSpin, UDM_SETBUDDY, (WPARAM)hWndRepeat, 0);
    SendMessage(hWndRepeatSpin, UDM_SETRANGE, 0, MAKELONG(999, 0));
    SetDlgItemInt(hDlg, IDC_REPEAT, 1, FALSE);
    SendMessage(hWndName, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndMacrosList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);

    //Columns
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_MACRO);
    lvc.cx=nColumnWidth1;
    lvc.iSubItem=LVI_MACRO_FILE;
    ListView_InsertColumnWide(hWndMacrosList, LVI_MACRO_FILE, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_HOTKEY);
    lvc.cx=nColumnWidth2;
    lvc.iSubItem=LVI_MACRO_HOTKEY;
    ListView_InsertColumnWide(hWndMacrosList, LVI_MACRO_HOTKEY, &lvc);

    FillMacroList(hWndMacrosList);
    SendMessage(hDlg, AKDLL_UPDATE, 0, 0);
  }
  else if (uMsg == AKDLL_UPDATE)
  {
    BOOL bEnable;

    if (nSelItem == 0)
    {
      if (hRecordStack.last)
      {
        bEnable=TRUE;
        if (GetWindowTextLengthWide(hWndName))
          EnableWindow(hWndSaveButton, TRUE);
        else
          EnableWindow(hWndSaveButton, FALSE);
      }
      else
      {
        bEnable=FALSE;
        EnableWindow(hWndSaveButton, FALSE);
      }
      EnableWindow(hWndPlayButton, bEnable);
      EnableWindow(hWndRepeat, bEnable);
      EnableWindow(hWndRepeatSpin, bEnable);
      EnableWindow(hWndName, bEnable);
      EnableWindow(hWndViewButton, bEnable);
      EnableWindow(hWndRecordButton, TRUE);
      EnableWindow(hWndDeleteButton, FALSE);
      EnableWindow(hWndHotkey, FALSE);
      EnableWindow(hWndAssignButton, FALSE);
    }
    else
    {
      if (nSelItem > 0)
        bEnable=TRUE;
      else
        bEnable=FALSE;

      EnableWindow(hWndPlayButton, bEnable);
      EnableWindow(hWndRepeat, bEnable);
      EnableWindow(hWndRepeatSpin, bEnable);
      EnableWindow(hWndRecordButton, FALSE);
      EnableWindow(hWndName, FALSE);
      EnableWindow(hWndSaveButton, FALSE);
      EnableWindow(hWndDeleteButton, bEnable);
      EnableWindow(hWndHotkey, bEnable);
      EnableWindow(hWndAssignButton, bEnable);
      EnableWindow(hWndViewButton, bEnable);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_MACROS_LIST)
    {
      if (((NMLISTVIEW *)lParam)->hdr.code == LVN_ITEMCHANGED)
      {
        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK)
        {
          BOOL bNewState;
          BOOL bOldState;

          bNewState=((((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          bOldState=((((NMLISTVIEW *)lParam)->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
            bListChanged=TRUE;
        }
        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_SELECTED)
        {
          SendMessage(hWndHotkey, HKM_SETHOTKEY, ((NMLISTVIEW *)lParam)->lParam, 0);
          nSelItem=((NMLISTVIEW *)lParam)->iItem;
        }
        if (((NMLISTVIEW *)lParam)->uOldState & LVIS_SELECTED)
        {
          SendMessage(hWndHotkey, HKM_SETHOTKEY, 0, 0);
          nSelItem=-1;
        }
        PostMessage(hDlg, AKDLL_UPDATE, 0, 0);
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
        PostMessage(hDlg, WM_COMMAND, IDC_PLAY, 0);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PLAY)
    {
      wchar_t wszName[MAX_PATH];
      LVITEMW lvi;
      int nRepeat;

      if (nSelItem == 0)
      {
        EnableWindow(hWndPlayButton, FALSE);
        SetFocus(hMainWnd);
        nRepeat=GetDlgItemInt(hDlg, IDC_REPEAT, NULL, FALSE);
        MacroPlay(&hRecordStack, nRepeat, 0);
        EnableWindow(hWndPlayButton, TRUE);
      }
      else
      {
        //Get macro name
        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszName;
        lvi.cchTextMax=MAX_PATH;
        lvi.iItem=nSelItem;
        lvi.iSubItem=LVI_MACRO_FILE;
        ListView_GetItemWide(hWndMacrosList, &lvi);

        if (xstrcmpiW(wszName, wszLastMacro))
        {
          StackHotkeyFree(&hLastMacroStack);
          wszLastMacro[0]='\0';

          if (ReadMacroFile(&hLastMacroStack, wszName))
            xstrcpynW(wszLastMacro, wszName, MAX_PATH);
        }
        EnableWindow(hWndPlayButton, FALSE);
        SetFocus(hMainWnd);
        nRepeat=GetDlgItemInt(hDlg, IDC_REPEAT, NULL, FALSE);
        MacroPlay(&hLastMacroStack, nRepeat, 0);
        EnableWindow(hWndPlayButton, TRUE);
      }
    }
    else if (LOWORD(wParam) == IDC_RECORD)
    {
      DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_STOP), hDlg, (DLGPROC)StopDlgProc);
      PostMessage(hDlg, AKDLL_UPDATE, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_NAME)
    {
      PostMessage(hDlg, AKDLL_UPDATE, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_SAVE)
    {
      wchar_t wszName[MAX_PATH];
      LVITEMW lvi;
      int nIndex;

      if (GetWindowTextWide(hWndName, wszName, MAX_PATH))
      {
        xprintfW(wszBuffer, L"%s\\%s.macro", wszMacrosDir, wszName);

        if (!FileExistsWide(wszBuffer))
        {
          if (SaveMacroFile(&hRecordStack, wszName))
          {
            xstrcpynW(wszLastMacro, wszName, MAX_PATH);
            xmemcpy(&hLastMacroStack, &hRecordStack, sizeof(STACKKEY));
            hRecordStack.first=0;
            hRecordStack.last=0;

            lvi.mask=LVIF_TEXT;
            lvi.pszText=wszName;
            lvi.iItem=(int)SendMessage(hWndMacrosList, LVM_GETITEMCOUNT, 0, 0);
            lvi.iSubItem=LVI_MACRO_FILE;
            nIndex=ListView_InsertItemWide(hWndMacrosList, &lvi);

            if (!nIndex) nIndex=MoveListViewItem(hWndMacrosList, nIndex, nIndex + 1);

            SetFocus(hWndMacrosList);
            lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
            lvi.state=LVIS_SELECTED|LVIS_FOCUSED;
            SendMessage(hWndMacrosList, LVM_SETITEMSTATE, nIndex, (LPARAM)&lvi);
          }
          else MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_CANNOT_OPENFILE), wszPluginTitle, MB_OK|MB_ICONERROR);
        }
        else MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_ALREADY_EXIST), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
      }
    }
    else if (LOWORD(wParam) == IDC_DELETE)
    {
      wchar_t wszName[MAX_PATH];
      LVITEMW lvi;

      if (nSelItem > 0)
      {
        if (MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_CONFIRM_DELETE), wszPluginTitle, MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
          //Get macro name
          lvi.mask=LVIF_TEXT;
          lvi.pszText=wszName;
          lvi.cchTextMax=MAX_PATH;
          lvi.iItem=nSelItem;
          lvi.iSubItem=LVI_MACRO_FILE;
          ListView_GetItemWide(hWndMacrosList, &lvi);

          if (DeleteMacroFile(wszName))
          {
            if (!xstrcmpiW(wszName, wszLastMacro))
            {
              StackHotkeyFree(&hLastMacroStack);
              wszLastMacro[0]='\0';
            }
            SendMessage(hWndMacrosList, LVM_DELETEITEM, nSelItem, 0);
            RegisterHotkey(wszName, 0);
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_ASSIGN)
    {
      LVITEMW lvi;
      WORD wHotkey;
      wchar_t wszFile[MAX_PATH];
      wchar_t wszHotkey[MAX_PATH];

      //Get macro name
      lvi.mask=LVIF_TEXT;
      lvi.pszText=wszFile;
      lvi.cchTextMax=MAX_PATH;
      lvi.iItem=nSelItem;
      lvi.iSubItem=LVI_MACRO_FILE;
      ListView_GetItemWide(hWndMacrosList, &lvi);

      //Get macro hotkey
      wHotkey=(WORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0);

      //Register hotkey
      if (RegisterHotkey(wszFile, wHotkey))
      {
        GetHotkeyString(wHotkey, wszHotkey);

        lvi.mask=LVIF_PARAM;
        lvi.iItem=nSelItem;
        lvi.iSubItem=LVI_MACRO_FILE;
        lvi.lParam=wHotkey;
        ListView_SetItemWide(hWndMacrosList, &lvi);

        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszHotkey;
        lvi.iItem=nSelItem;
        lvi.iSubItem=LVI_MACRO_HOTKEY;
        ListView_SetItemWide(hWndMacrosList, &lvi);

        SendMessage(hWndHotkey, HKM_SETHOTKEY, wHotkey, 0);
        bListChanged=TRUE;
      }
      else SetFocus(hWndHotkey);
    }
    else if (LOWORD(wParam) == IDC_VIEW)
    {
      STACKKEY *lpCurStack=NULL;
      STACKKEY hFileStack={0};
      wchar_t wszName[MAX_PATH];
      LVITEMW lvi;

      if (nSelItem == 0)
      {
        wszName[0]=L'\0';
        lpCurStack=&hRecordStack;
      }
      else
      {
        //Get macro name
        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszName;
        lvi.cchTextMax=MAX_PATH;
        lvi.iItem=nSelItem;
        lvi.iSubItem=LVI_MACRO_FILE;
        ListView_GetItemWide(hWndMacrosList, &lvi);

        if (ReadMacroFile(&hFileStack, wszName))
          lpCurStack=&hFileStack;
      }

      if (lpCurStack)
      {
        MACROFILE mf;

        mf.wpMacro=wszName;
        mf.lpStack=lpCurStack;
        DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_VIEW), hDlg, (DLGPROC)ViewDlgProc, (LPARAM)&mf);
        if (nSelItem != 0)
          StackHotkeyFree(lpCurStack);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (nSelItem == 0)
        PostMessage(hDlg, WM_COMMAND, IDC_RECORD, 0);
      else if (nSelItem > 0)
        PostMessage(hDlg, WM_COMMAND, IDC_PLAY, 0);
    }
    else if (LOWORD(wParam) == IDC_CLOSE ||
             LOWORD(wParam) == IDCANCEL)
    {
      int nWidth;

      nWidth=(int)SendMessage(hWndMacrosList, LVM_GETCOLUMNWIDTH, LVI_MACRO_FILE, 0);
      if (nColumnWidth1 != nWidth)
      {
        nColumnWidth1=nWidth;
        dwSaveFlags|=OF_RECT;
      }
      nWidth=(int)SendMessage(hWndMacrosList, LVM_GETCOLUMNWIDTH, LVI_MACRO_HOTKEY, 0);
      if (nColumnWidth2 != nWidth)
      {
        nColumnWidth2=nWidth;
        dwSaveFlags|=OF_RECT;
      }

      if (bListChanged || LOWORD(wParam) == IDC_CLOSE)
      {
        dwSaveFlags|=OF_RECT|OF_HOTKEYS;
      }
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      DestroyWindow(hWndMainDlg);
      hWndMainDlg=NULL;
      CloseHandle(hDialogThread);
      hDialogThread=NULL;
      ExitThread(0);
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Destroy resources
    DestroyIcon(hPlayIcon);
    DestroyIcon(hPluginIcon);
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      dwSaveFlags|=OF_RECT;
  }

  return FALSE;
}

BOOL CALLBACK StopDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hIconStop;
  static HWND hWndStopButton;

  if (uMsg == WM_INITDIALOG)
  {
    hWndStopDlg=hDlg;
    hWndStopButton=GetDlgItem(hDlg, IDC_STOP);

    //Set stop button icon
    {
      BUTTONDRAW bd;

      hIconStop=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_STOP), IMAGE_ICON, 16, 16, 0);
      bd.dwFlags=BIF_ICON|BIF_ETCHED;
      bd.hImage=hIconStop;
      bd.nImageWidth=16;
      bd.nImageHeight=16;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndStopButton, (LPARAM)&bd);
    }

    //Set dialog position
    {
      EDITINFO ei;

      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
        PosWindowToCorner(ei.hWndEdit, hDlg);
    }

    ShowWindow(hWndMainDlg, SW_HIDE);
    PostMessage(hDlg, WM_COMMAND, IDC_RECORD, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_RECORD)
    {
      //Start record
      StackHotkeyFree(&hRecordStack);
      SetFocus(hMainWnd);
      MacroRecord();
      bRecord=TRUE;
    }
    else if (LOWORD(wParam) == IDC_STOP ||
             LOWORD(wParam) == IDCANCEL)
    {
      //Stop record
      bRecord=FALSE;
      MacroStop();

      ShowWindow(hWndMainDlg, SW_SHOW);
      EndDialog(hDlg, 0);
      DestroyIcon(hIconStop);
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
    hWndStopDlg=NULL;
  }
  return FALSE;
}

BOOL CALLBACK ViewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static MACROFILE *mf;
  static HICON hPluginIcon;
  static HWND hWndSendKeysEdit;
  static HWND hWndExportButton;
  static HWND hWndKeyactList;
  static HWND hWndCloseButton;
  static RESIZEDIALOG rds[]={{&hWndSendKeysEdit, RDS_SIZE|RDS_X, 0},
                             {&hWndExportButton, RDS_MOVE|RDS_X, 0},
                             {&hWndKeyactList,   RDS_SIZE|RDS_X, 0},
                             {&hWndKeyactList,   RDS_SIZE|RDS_Y, 0},
                             {&hWndCloseButton,  RDS_MOVE|RDS_X, 0},
                             {&hWndCloseButton,  RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvc;

    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hPluginIcon);

    mf=(MACROFILE *)lParam;
    hWndSendKeysEdit=GetDlgItem(hDlg, IDC_SENDKEYS);
    hWndExportButton=GetDlgItem(hDlg, IDC_EXPORT);
    hWndKeyactList=GetDlgItem(hDlg, IDC_KEYACT_LIST);
    hWndCloseButton=GetDlgItem(hDlg, IDC_CLOSE);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_VIEW));
    SetDlgItemTextWide(hDlg, IDC_EXPORT, GetLangStringW(wLangModule, STRID_EXPORT));
    SetDlgItemTextWide(hDlg, IDC_CLOSE, GetLangStringW(wLangModule, STRID_CLOSE));

    SendMessage(hWndKeyactList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

    //Columns
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_KEY);
    lvc.cx=nViewColumnWidth1;
    lvc.iSubItem=LVI_VIEW_KEY;
    ListView_InsertColumnWide(hWndKeyactList, LVI_VIEW_KEY, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_ACT);
    lvc.cx=nViewColumnWidth2;
    lvc.iSubItem=LVI_VIEW_ACT;
    ListView_InsertColumnWide(hWndKeyactList, LVI_VIEW_ACT, &lvc);

    FillKeyactList(hWndKeyactList, mf->lpStack);

    //Fill SendKeys string
    {
      wchar_t *wszSendKeys;
      INT_PTR nSendKeysLen;

      nSendKeysLen=FillSendKeys(mf->lpStack, NULL);
      if (wszSendKeys=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSendKeysLen * sizeof(wchar_t)))
      {
        FillSendKeys(mf->lpStack, wszSendKeys);
        SetWindowTextWide(hWndSendKeysEdit, wszSendKeys);
        GlobalFree((HGLOBAL)wszSendKeys);
      }
      if (!nSendKeysLen)
        EnableWindow(hWndExportButton, FALSE);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_EXPORT)
    {
      OPENFILENAMEW efn;
      wchar_t wszScriptFile[MAX_PATH];
      wchar_t *wszSendKeys;
      char *szText;
      char *pText;
      HANDLE hFile;
      int nText;
      int nSendKeysLen;
      DWORD dwBytesWritten;

      xstrcpynW(wszScriptFile, mf->wpMacro, MAX_PATH);
      xmemset(&efn, 0, sizeof(OPENFILENAMEW));
      efn.lStructSize  =sizeof(OPENFILENAMEW);
      efn.hwndOwner    =hDlg;
      efn.lpstrFile    =wszScriptFile;
      efn.nMaxFile     =MAX_PATH;
      efn.lpstrFilter  =L"*.js\0*.js\0*.*\0*.*\0\0";
      efn.nFilterIndex =1;
      efn.Flags        =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_ENABLESIZING|OFN_OVERWRITEPROMPT;
      efn.lpstrDefExt  =L"js";

      GetSaveFileNameWide(&efn);

      if (*wszScriptFile)
      {
        if ((hFile=CreateFileWide(wszScriptFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
        {
          nSendKeysLen=GetWindowTextLengthWide(hWndSendKeysEdit);
          if (wszSendKeys=(wchar_t *)GlobalAlloc(GPTR, (nSendKeysLen + 1) * sizeof(wchar_t)))
          {
            GetWindowTextWide(hWndSendKeysEdit, wszSendKeys, nSendKeysLen + 1);
            pText="var WshShell=new ActiveXObject(\"WScript.shell\");\r\n\r\nWshShell.SendKeys(\"%S\");\r\n";
            nText=(int)xprintfA(NULL, pText, wszSendKeys);
            if (szText=(char *)GlobalAlloc(GPTR, nText + 1))
            {
              nText=(int)xprintfA(szText, pText, wszSendKeys);
              WriteFile(hFile, szText, nText, &dwBytesWritten, NULL);
              GlobalFree((HGLOBAL)szText);
            }
            GlobalFree((HGLOBAL)wszSendKeys);
          }
          CloseHandle(hFile);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_CLOSE ||
             LOWORD(wParam) == IDOK ||
             LOWORD(wParam) == IDCANCEL)
    {
      int nWidth;

      nWidth=(int)SendMessage(hWndKeyactList, LVM_GETCOLUMNWIDTH, LVI_VIEW_ACT, 0);
      if (nViewColumnWidth1 != nWidth)
      {
        nViewColumnWidth1=nWidth;
        dwSaveFlags|=OF_VIEWRECT;
      }
      nWidth=(int)SendMessage(hWndKeyactList, LVM_GETCOLUMNWIDTH, LVI_VIEW_KEY, 0);
      if (nViewColumnWidth2 != nWidth)
      {
        nViewColumnWidth2=nWidth;
        dwSaveFlags|=OF_VIEWRECT;
      }

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

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcViewMainMinMaxDialog, &rcViewMainCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      dwSaveFlags|=OF_VIEWRECT;
  }

  return FALSE;
}

BOOL RegisterHotkey(wchar_t *wszMacroName, WORD wHotkey)
{
  PLUGINFUNCTION *pfElement=NULL;
  wchar_t wszPrefix[MAX_PATH];
  wchar_t wszFunction[MAX_PATH];
  wchar_t wszHotkeyOwner[MAX_PATH];
  int nOwner;

  xstrcpyW(wszPrefix, L"Macros::Main::");
  xprintfW(wszFunction, L"%s%s", wszPrefix, wszMacroName);
  nOwner=(int)SendMessage(hMainWnd, AKD_CHECKHOTKEY, wHotkey, (LPARAM)wszHotkeyOwner);

  if (!nOwner)
  {
    if (pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wszFunction, 0))
    {
      pfElement->wHotkey=wHotkey;

      if (!pfElement->wHotkey && !pfElement->bRunning && !pfElement->bAutoLoad)
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfElement);
    }
    else
    {
      if (wHotkey)
      {
        PLUGINADDW pa={0};

        pa.pFunction=wszFunction;
        pa.wHotkey=wHotkey;
        pa.PluginProc=HotkeyProc;

        if (pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa))
        {
          pfElement->lpParameter=pfElement->wszFunction + lstrlenW(wszPrefix);
        }
      }
    }
    return TRUE;
  }
  else
  {
    xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_HOTKEY_EXISTS), wszFunction, wszHotkeyOwner);
    MessageBoxW(hWndMainDlg, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
    return FALSE;
  }
}

void FillMacroList(HWND hWnd)
{
  WIN32_FIND_DATAW wfd;
  HANDLE hFind;
  wchar_t wszFindFiles[MAX_PATH];
  wchar_t wszBaseName[MAX_PATH];
  wchar_t wszHotkey[MAX_PATH];
  LVITEMW lvi;
  DWORD dwStyle;
  int nIndexToSelect=-1;
  int nIndex;

  //Macro files
  xprintfW(wszFindFiles, L"%s\\*.macro", wszMacrosDir);

  if ((hFind=FindFirstFileWide(wszFindFiles, &wfd)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      GetBaseName(wfd.cFileName, -1, wszBaseName, MAX_PATH);

      //Find hotkey
      {
        PLUGINFUNCTION *pfElement=NULL;

        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszBaseName;
        lvi.iItem=0x7FFFFFFF;
        lvi.iSubItem=LVI_MACRO_FILE;
        nIndex=ListView_InsertItemWide(hWnd, &lvi);

        if (nIndexToSelect < 0)
        {
          if (!xstrcmpiW(wszBaseName, wszLastMacro))
            nIndexToSelect=nIndex;
        }
        else if (nIndexToSelect >= nIndex)
          ++nIndexToSelect;

        xprintfW(wszBuffer, L"Macros::Main::%s", wszBaseName);
        pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wszBuffer, 0);

        if (pfElement)
        {
          lvi.mask=LVIF_PARAM;
          lvi.iItem=nIndex;
          lvi.iSubItem=LVI_MACRO_FILE;
          lvi.lParam=pfElement->wHotkey;
          ListView_SetItemWide(hWnd, &lvi);

          GetHotkeyString(pfElement->wHotkey, wszHotkey);
          lvi.mask=LVIF_TEXT;
          lvi.pszText=wszHotkey;
          lvi.iItem=nIndex;
          lvi.iSubItem=LVI_MACRO_HOTKEY;
          ListView_SetItemWide(hWnd, &lvi);
        }
      }
    }
    while (FindNextFileWide(hFind, &wfd));

    FindClose(hFind);
  }

  dwStyle=(DWORD)GetWindowLongPtrWide(hWnd, GWL_STYLE);
  SetWindowLongPtrWide(hWnd, GWL_STYLE, dwStyle & ~LVS_SORTASCENDING);
  lvi.mask=LVIF_TEXT;
  lvi.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_RECORD_LISTITEM);
  lvi.iItem=0;
  lvi.iSubItem=LVI_MACRO_FILE;
  nIndex=ListView_InsertItemWide(hWnd, &lvi);
  SetWindowLongPtrWide(hWnd, GWL_STYLE, dwStyle);

  lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
  lvi.state=LVIS_SELECTED|LVIS_FOCUSED;
  SendMessage(hWnd, LVM_SETITEMSTATE, (WPARAM)max(nIndexToSelect + 1, 0), (LPARAM)&lvi);
  SendMessage(hWnd, LVM_ENSUREVISIBLE, (WPARAM)max(nIndexToSelect + 1, 0), TRUE);
}

void FillKeyactList(HWND hWnd, STACKKEY *hStack)
{
  KEYSTRUCT *lpElement;
  wchar_t wszHotkey[MAX_PATH];
  LVITEMW lvi;
  int nIndex;
  WORD wHotkey;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    wHotkey=MAKEWORD(lpElement->ka.bVk, (lpElement->ka.dwFlags & KEYEVENTF_EXTENDEDKEY)?HOTKEYF_EXT:0);
    GetHotkeyString(wHotkey, wszHotkey);
    lvi.mask=LVIF_TEXT;
    lvi.pszText=wszHotkey;
    lvi.iItem=0x7FFFFFFF;
    lvi.iSubItem=LVI_VIEW_KEY;
    nIndex=ListView_InsertItemWide(hWnd, &lvi);

    lvi.mask=LVIF_TEXT;
    lvi.pszText=(lpElement->ka.dwFlags & KEYEVENTF_KEYUP)?L"Up":L"Down";
    lvi.iItem=nIndex;
    lvi.iSubItem=LVI_VIEW_ACT;
    ListView_SetItemWide(hWnd, &lvi);
  }
}

INT_PTR FillSendKeys(STACKKEY *hStack, wchar_t *wszSendKeys)
{
  KEYSTRUCT *lpElement;
  wchar_t *wpCount=wszSendKeys;
  wchar_t wchChar;
  HKL hklEnglish;
  INT_PTR nKeyLen;
  BYTE nMod=0;

  //Use English layout cause SendKeys understand only English
  if (bOldWindows)
    hklEnglish=LoadKeyboardLayoutA("00000409", 0);
  else
    hklEnglish=LoadKeyboardLayoutW(L"00000409", 0);

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->ka.bVk == VK_SHIFT ||
        lpElement->ka.bVk == VK_CONTROL ||
        lpElement->ka.bVk == VK_MENU)
    {
      if (lpElement->ka.dwFlags & KEYEVENTF_KEYUP)
      {
        if (lpElement->ka.bVk == VK_SHIFT)
          nMod&=~HOTKEYF_SHIFT;
        else if (lpElement->ka.bVk == VK_CONTROL)
          nMod&=~HOTKEYF_CONTROL;
        else if (lpElement->ka.bVk == VK_MENU)
          nMod&=~HOTKEYF_ALT;

        if (nMod & HOTKEYF_EXT)
        {
          if (wszSendKeys) *wpCount=L')';
          ++wpCount;
          nMod&=~HOTKEYF_EXT;
        }
      }
      else
      {
        if (lpElement->ka.bVk == VK_SHIFT)
          nMod|=HOTKEYF_SHIFT;
        else if (lpElement->ka.bVk == VK_CONTROL)
          nMod|=HOTKEYF_CONTROL;
        else if (lpElement->ka.bVk == VK_MENU)
          nMod|=HOTKEYF_ALT;
      }
    }
    else if (!(lpElement->ka.dwFlags & KEYEVENTF_KEYUP))
    {
      if (nMod && !(nMod & HOTKEYF_EXT))
      {
        if (nMod & HOTKEYF_SHIFT)
        {
          if (wszSendKeys) *wpCount=L'+';
          ++wpCount;
        }
        if (nMod & HOTKEYF_CONTROL)
        {
          if (wszSendKeys) *wpCount=L'^';
          ++wpCount;
        }
        if (nMod & HOTKEYF_ALT)
        {
          if (wszSendKeys) *wpCount=L'%';
          ++wpCount;
        }

        if (!IsSingleKey(lpElement))
        {
          if (wszSendKeys) *wpCount=L'(';
          ++wpCount;
          nMod|=HOTKEYF_EXT;
        }
      }
      if (lpElement->ka.bVk == VK_BACK)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{BS}");       //{BACKSPACE}, {BKSP}
      else if (lpElement->ka.bVk == VK_PAUSE)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{BREAK}");
      else if (lpElement->ka.bVk == VK_CAPITAL)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{CAPSLOCK}");
      else if (lpElement->ka.bVk == VK_DELETE)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{DEL}");      //{DELETE}
      else if (lpElement->ka.bVk == VK_DOWN)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{DOWN}");
      else if (lpElement->ka.bVk == VK_END)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{END}");
      else if (lpElement->ka.bVk == VK_RETURN)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{ENTER}");    //~
      else if (lpElement->ka.bVk == VK_ESCAPE)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{ESC}");
      else if (lpElement->ka.bVk == VK_HELP)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{HELP}");
      else if (lpElement->ka.bVk == VK_HOME)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{HOME}");
      else if (lpElement->ka.bVk == VK_INSERT)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{INS}");      //{INSERT}
      else if (lpElement->ka.bVk == VK_LEFT)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{LEFT}");
      else if (lpElement->ka.bVk == VK_NUMLOCK)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{NUMLOCK}");
      else if (lpElement->ka.bVk == VK_NEXT)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{PGDN}");
      else if (lpElement->ka.bVk == VK_PRIOR)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{PGUP}");
      else if (lpElement->ka.bVk == VK_PRINT)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{PRTSC}");
      else if (lpElement->ka.bVk == VK_RIGHT)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{RIGHT}");
      else if (lpElement->ka.bVk == VK_SCROLL)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{SCROLLLOCK}");
      else if (lpElement->ka.bVk == VK_TAB)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{TAB}");
      else if (lpElement->ka.bVk == VK_UP)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{UP}");
      else if (lpElement->ka.bVk == VK_F1)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F1}");
      else if (lpElement->ka.bVk == VK_F2)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F2}");
      else if (lpElement->ka.bVk == VK_F3)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F3}");
      else if (lpElement->ka.bVk == VK_F4)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F4}");
      else if (lpElement->ka.bVk == VK_F5)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F5}");
      else if (lpElement->ka.bVk == VK_F6)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F6}");
      else if (lpElement->ka.bVk == VK_F7)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F7}");
      else if (lpElement->ka.bVk == VK_F8)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F8}");
      else if (lpElement->ka.bVk == VK_F9)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F9}");
      else if (lpElement->ka.bVk == VK_F10)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F10}");
      else if (lpElement->ka.bVk == VK_F11)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F11}");
      else if (lpElement->ka.bVk == VK_F12)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F12}");
      else if (lpElement->ka.bVk == VK_F13)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F13}");
      else if (lpElement->ka.bVk == VK_F14)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F14}");
      else if (lpElement->ka.bVk == VK_F15)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F15}");
      else if (lpElement->ka.bVk == VK_F16)
        nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{F16}");
      else
      {
        if (hklEnglish)
        {
          if (bOldWindows)
            wchChar=LOWORD(MapVirtualKeyExA(lpElement->ka.bVk, 2, hklEnglish));
          else
            wchChar=LOWORD(MapVirtualKeyExW(lpElement->ka.bVk, 2, hklEnglish));
        }
        else
        {
          if (bOldWindows)
            wchChar=LOWORD(MapVirtualKeyA(lpElement->ka.bVk, 2));
          else
            wchChar=LOWORD(MapVirtualKeyW(lpElement->ka.bVk, 2));
        }

        if (wchChar == L'+')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{+}");
        else if (wchChar == L'^')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{^}");
        else if (wchChar == L'%')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{%}");
        else if (wchChar == L'~')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{~}");
        else if (wchChar == L'{')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{{}");
        else if (wchChar == L'}')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{}}");
        else if (wchChar == L'[')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{[}");
        else if (wchChar == L']')
          nKeyLen=xstrcpyW(wszSendKeys?wpCount:NULL, L"{]}");
        else
          nKeyLen=xprintfW(wszSendKeys?wpCount:NULL, L"%c", (nMod & HOTKEYF_SHIFT) ? WideCharUpper(wchChar) : WideCharLower(wchChar));
      }
      if (!wszSendKeys) --nKeyLen;
      wpCount+=nKeyLen;
    }
  }
  if (wszSendKeys)
    *wpCount=L'\0';
  else
    ++wpCount;

  if (hklEnglish)
    UnloadKeyboardLayout(hklEnglish);
  return wpCount - wszSendKeys;
}

BOOL IsSingleKey(KEYSTRUCT *lpElement)
{
  while (lpElement=lpElement->next)
  {
    if (lpElement->ka.bVk == VK_SHIFT ||
        lpElement->ka.bVk == VK_CONTROL ||
        lpElement->ka.bVk == VK_MENU)
    {
      break;
    }
    else if (!(lpElement->ka.dwFlags & KEYEVENTF_KEYUP))
      return FALSE;
  }
  return TRUE;
}

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    if (wParam == PM_REMOVE)
    {
      MSG *msg=(MSG *)lParam;
      DWORD dwFlags=0;

      if (msg->message == WM_KEYDOWN || msg->message == WM_SYSKEYDOWN ||
          msg->message == WM_KEYUP || msg->message == WM_SYSKEYUP)
      {
        if ((msg->lParam >> 24) & 1)
          dwFlags|=KEYEVENTF_EXTENDEDKEY;
        if (msg->message == WM_KEYUP || msg->message == WM_SYSKEYUP)
          dwFlags|=KEYEVENTF_KEYUP;
        else if (msg->message == WM_KEYDOWN || msg->message == WM_SYSKEYDOWN)
        {
          if ((msg->lParam >> 30) & 1)
            StackHotkeyAdd(&hRecordStack, (BYTE)msg->wParam, dwFlags|KEYEVENTF_KEYUP);
        }
        StackHotkeyAdd(&hRecordStack, (BYTE)msg->wParam, dwFlags);
      }
    }
  }
  return CallNextHookEx(hHook, code, wParam, lParam);
}

BOOL CALLBACK HotkeyProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  if (!(dwMacroState & MS_PLAYING))
  {
    wchar_t *wpName=(wchar_t *)lpParameter;

    ExecMacro(wpName, 1, 0);
  }
  return TRUE;
}

DWORD WINAPI ExecThreadProc(LPVOID lpParameter)
{
  EXECMACRO *em=(EXECMACRO *)lpParameter;
  HANDLE hInitMutex=em->hInitMutex;
  HANDLE hExecMutex;
  HANDLE hThread=hExecThread;
  wchar_t wszMacro[MAX_PATH];
  int nRepeat;
  DWORD dwFlags;

  dwMacroState=MS_INITIALIZING;

  //Copy to thread memory
  xstrcpynW(wszMacro, em->wpMacro, MAX_PATH);
  nRepeat=em->nRepeat;
  dwFlags=em->dwFlags;

  //Protect from double execution
  hExecMutex=CreateEventA(NULL, FALSE, FALSE, szMutexExecName);

  //Thread is initialized now unlock main thread
  SetEvent(hInitMutex);

  if (hExecMutex)
  {
    if (GetLastError() == ERROR_ALREADY_EXISTS)
      WaitForSingleObject(hExecMutex, INFINITE);

    //Wait for release all virtual keys
    while (SendMessage(hMainWnd, AKD_WAITKEYBOARD, TRUE, 0))
      Sleep(100);

    if (!WaitIdle())
    {
      //Play macro
      if (wszMacro[0])
      {
        if (xstrcmpiW(wszMacro, wszLastMacro))
        {
          StackHotkeyFree(&hLastMacroStack);
          wszLastMacro[0]='\0';

          if (ReadMacroFile(&hLastMacroStack, wszMacro))
            xstrcpynW(wszLastMacro, wszMacro, MAX_PATH);
        }
        MacroPlay(&hLastMacroStack, nRepeat, dwFlags);
      }
      else MacroPlay(&hRecordStack, nRepeat, dwFlags);
    }
    SetEvent(hExecMutex);
    CloseHandle(hExecMutex);
  }
  dwMacroState&=~MS_INITIALIZING;

  //Change icon from stop end to play end
  if (hToolBarIconPlayToEnd)
  {
    RECT rcButton;

    ImageList_ReplaceIcon(hToolBarImageList, nToolBarIndexPlayToEnd, hToolBarIconPlayToEnd);
    DestroyIcon(hToolBarIconPlayToEnd);
    hToolBarIconPlayToEnd=NULL;
    SendMessage(hWndToolbar, TB_GETRECT, nToolBarCmdID, (LPARAM)&rcButton);
    InvalidateRect(hWndToolbar, &rcButton, TRUE);
  }

  //Free thread handle
  if (hThread)
  {
    CloseHandle(hThread);
    hThread=NULL;
  }
  return 0;
}

KEYSTRUCT* StackHotkeyAdd(STACKKEY *hStack, BYTE bVk, DWORD dwFlags)
{
  KEYSTRUCT *lpElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(KEYSTRUCT)))
  {
    lpElement->ka.bVk=bVk;
    lpElement->ka.dwFlags=dwFlags;
  }
  return lpElement;
}

void StackHotkeyPress(STACKKEY *hStack, BOOL bToEnd, DWORD dwFlags)
{
  KEYSTRUCT *lpElement=hStack->first;
  HANDLE hScriptsExecMutex;
  BOOL bBusy=FALSE;

  if (IsCaretAtLastLine())
    nCtrlEnd=CE_CTRLENDFINISH;
  else
    nCtrlEnd=CE_RELEASED;

  while (lpElement)
  {
    if (dwMacroState & MS_PLAYING)
    {
      //Press key
      keybd_event(lpElement->ka.bVk, 0, lpElement->ka.dwFlags, 0);

      //Ctrl+End
      if (lpElement->ka.bVk == VK_CONTROL)
      {
        if (nCtrlEnd == CE_RELEASED)
          nCtrlEnd=CE_CTRLDOWN;
      }
      else if (lpElement->ka.bVk == VK_END)
      {
        if (nCtrlEnd == CE_CTRLDOWN)
          nCtrlEnd=CE_CTRLENDDOWN;
      }
    }
    else
    {
      //Release key
      if (lpElement->ka.dwFlags & KEYEVENTF_KEYUP)
      {
        if (GetKeyState(lpElement->ka.bVk) < 0)
        {
          keybd_event(lpElement->ka.bVk, 0, lpElement->ka.dwFlags, 0);

          //Ctrl+End
          if (lpElement->ka.bVk == VK_CONTROL)
          {
            if (nCtrlEnd == CE_CTRLDOWN)
              nCtrlEnd=CE_RELEASED;
          }
        }
      }
    }
    if (bBusy=WaitIdle())
      dwMacroState=(dwMacroState & ~MS_PLAYING)|MS_WAITTIMEOUT;

    //Scripts plugin synchronization
    if (!(dwFlags & EMF_SCRIPTSNOSYNC))
    {
      while (hScriptsExecMutex=OpenEventWide(EVENT_ALL_ACCESS, FALSE, wszScriptsExecMutex))
      {
        WaitForSingleObject(hScriptsExecMutex, INFINITE);

        if (bBusy=WaitIdle())
          dwMacroState=(dwMacroState & ~MS_PLAYING)|MS_WAITTIMEOUT;

        SetEvent(hScriptsExecMutex);
        CloseHandle(hScriptsExecMutex);
      }
    }

    //Check is end of the file reached
    if (bToEnd && (dwMacroState & MS_PLAYING))
    {
      if (IsCaretAtLastLine())
      {
        if (nCtrlEnd == CE_RELEASED)
          dwMacroState|=MS_EOFRICHED;
        else
          nCtrlEnd=CE_CTRLENDFINISH;
      }
    }
    lpElement=lpElement->next;
  }

  if (bBusy)
    MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_THREAD_BUSY), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
}

BOOL StackHotkeyRead(STACKKEY *hStack, HANDLE hFile)
{
  KEYSTRUCT *lpElement;
  KEYACT ka;
  DWORD dwBytesRead;

  for (;;)
  {
    if (!ReadFile(hFile, &ka, sizeof(KEYACT), &dwBytesRead, NULL))
      return FALSE;
    if (!dwBytesRead)
      break;
    if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(KEYSTRUCT)))
      xmemcpy(&lpElement->ka, &ka, sizeof(KEYACT));
    else
      return FALSE;
  }
  return TRUE;
}

BOOL StackHotkeySave(STACKKEY *hStack, HANDLE hFile)
{
  KEYSTRUCT *lpElement;
  DWORD dwBytesWritten;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!WriteFile(hFile, &lpElement->ka, sizeof(KEYACT), &dwBytesWritten, NULL))
      return FALSE;
  }
  return TRUE;
}

void StackHotkeyFree(STACKKEY *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

BOOL ReadMacroFile(STACKKEY *hKeyStack, wchar_t *wpMacro)
{
  HANDLE hFile;
  wchar_t wszMacro[MAX_PATH];

  xprintfW(wszMacro, L"%s\\%s.macro", wszMacrosDir, wpMacro);

  if ((hFile=CreateFileWide(wszMacro, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
  {
    StackHotkeyRead(hKeyStack, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
}

BOOL SaveMacroFile(STACKKEY *hRecordStack, wchar_t *wpMacro)
{
  HANDLE hFile;
  wchar_t wszMacro[MAX_PATH];

  xprintfW(wszMacro, L"%s\\%s.macro", wszMacrosDir, wpMacro);
  CreateDirectoryWide(wszMacrosDir, NULL);

  if ((hFile=CreateFileWide(wszMacro, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
  {
    StackHotkeySave(hRecordStack, hFile);
    CloseHandle(hFile);
    return TRUE;
  }
  return FALSE;
}

BOOL DeleteMacroFile(wchar_t *wpMacro)
{
  wchar_t wszMacro[MAX_PATH];

  xprintfW(wszMacro, L"%s\\%s.macro", wszMacrosDir, wpMacro);
  return DeleteFileWide(wszMacro);
}

void MacroRecord()
{
  hHook=SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, hInstanceDLL, 0);
  if (!hHook) MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_CANNOT_HOOK), wszPluginTitle, MB_OK|MB_ICONERROR);
}

void MacroStop()
{
  if (hHook)
  {
    if (UnhookWindowsHookEx(hHook))
      hHook=NULL;
  }
}

DWORD MacroPlay(STACKKEY *hStack, int nRepeat, DWORD dwFlags)
{
  EDITINFO ei;
  DWORD dwWindowProcessId;
  DWORD dwWindowThreadId;
  int nCount=0;
  BOOL bBreak=FALSE;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    if (!ei.bReadOnly)
    {
      //Play
      dwMacroState=MS_PLAYING;
      if (nRepeat == 0 && IsCaretAtLastLine())
      {
        dwMacroState|=MS_EOFRICHED;
        bBreak=TRUE;
      }

      do
      {
        dwWindowThreadId=GetWindowThreadProcessId(GetForegroundWindow(), &dwWindowProcessId);
        if (dwWindowProcessId != dwMainProcessId) break;
        if (hDialogThread && dwWindowThreadId == dwDialogThreadId) break;
        StackHotkeyPress(hStack, !nRepeat, dwFlags);
        if (!(dwMacroState & MS_PLAYING) || bBreak) break;
        if (dwMacroState & MS_EOFRICHED)
        {
          if (IsCaretAtLastEmptyLine()) break;
          bBreak=TRUE;
        }
      }
      while (nRepeat == 0 || ++nCount < nRepeat);

      if (nRepeat > 0 && nCount == nRepeat)
        dwMacroState|=MS_REPEATLIMIT;
      dwMacroState&=~MS_PLAYING;
    }
    else
    {
      MessageBeep(MB_OK);
      dwMacroState=MS_READONLY;
    }
  }
  else dwMacroState=MS_NOEDIT;

  return dwMacroState;
}

BOOL WaitIdle()
{
  MSG msg;
  DWORD dwTypes;
  DWORD dwStartWait;

  //Process plugin thread messages
  if (hDialogThread)
  {
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessageA(&msg);
    }
  }

  //Wait for process main thread messages
  dwStartWait=GetTickCount();

  for (;;)
  {
    if (GetTickCount() - dwStartWait < MACRO_TIMEOUT)
    {
      Sleep(0);
      dwTypes=(DWORD)SendMessage(hMainWnd, AKD_GETQUEUE, //QS_KEY|
                                                           QS_MOUSEMOVE|
                                                           QS_MOUSEBUTTON|
                                                           QS_POSTMESSAGE|
                                                           QS_TIMER|
                                                           QS_PAINT|
                                                           QS_SENDMESSAGE|
                                                           QS_HOTKEY, 0);
      if (dwTypes == 0) break;
    }
    else return TRUE;
  }
  return FALSE;
}

void ExecMacro(const wchar_t *wpMacro, int nRepeat, DWORD dwFlags)
{
  EXECMACRO em;
  HANDLE hInitMutex;

  if (!hDialogThread)
  {
    if (hInitMutex=CreateEventA(NULL, FALSE, FALSE, szMutexInitName))
    {
      em.wpMacro=wpMacro;
      em.nRepeat=nRepeat;
      em.dwFlags=dwFlags;
      em.hInitMutex=hInitMutex;

      if (hExecThread=CreateThread(NULL, 0, ExecThreadProc, &em, 0, &dwExecThreadId))
        WaitForSingleObject(hInitMutex, INFINITE);

      CloseHandle(hInitMutex);
    }
  }
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_MOVE || uMsg == WM_SIZE)
  {
    //Set dialog position
    if (hWndStopDlg)
    {
      EDITINFO ei;
      LRESULT lResult;

      //Call next procedure
      lResult=NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
        PosWindowToCorner(ei.hWndEdit, hWndStopDlg);
      return lResult;
    }
  }
  else if (uMsg == AKDN_SEARCH_ENDED)
  {
    nCtrlEnd=CE_RELEASED;
    dwMacroState=(dwMacroState & ~MS_PLAYING)|MS_SEARCHENDED;
  }
  else if (uMsg == AKDN_FRAME_NOWINDOWS)
  {
    nCtrlEnd=CE_RELEASED;
    dwMacroState=(dwMacroState & ~MS_PLAYING)|MS_FRAMENOWINDOWS;
  }
  else if (uMsg == AKDN_EDIT_ONFINISH)
  {
    //nCtrlEnd=CE_RELEASED;
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    nCtrlEnd=CE_RELEASED;
    dwMacroState=(dwMacroState & ~MS_PLAYING)|MS_PROGRAMEXIT;
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
    if (hWndMainDlg) SendMessage(hWndMainDlg, WM_COMMAND, IDCANCEL, 0);
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

LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT)
    {
      if (((NMHDR *)lParam)->code == EN_SELCHANGE)
      {
        if (nCtrlEnd == CE_CTRLENDFINISH)
          nCtrlEnd=CE_RELEASED;
      }
    }
  }
  return 0;
}

int GetHotkeyString(WORD wHotkey, wchar_t *wszString)
{
  wchar_t wszKeyText[100];
  LONG lScan;
  int nResult=0;

  wszString[0]=L'\0';
  wszKeyText[0]=L'\0';

  if (wHotkey)
  {
    if (bOldWindows)
      lScan=MapVirtualKeyA(LOBYTE(wHotkey), 0) << 16;
    else
      lScan=MapVirtualKeyW(LOBYTE(wHotkey), 0) << 16;
    if (HIBYTE(wHotkey) & HOTKEYF_EXT)
      lScan|=0x01000000L;
    GetKeyNameTextWide(lScan, wszKeyText, 100);

    nResult=(int)xprintfW(wszString, L"%s%s%s%s",
                                     (HIBYTE(wHotkey) & HOTKEYF_CONTROL)?L"Ctrl + ":L"",
                                     (HIBYTE(wHotkey) & HOTKEYF_SHIFT)?L"Shift + ":L"",
                                     (HIBYTE(wHotkey) & HOTKEYF_ALT)?L"Alt + ":L"",
                                     wszKeyText);
  }
  return nResult;
}

int MoveListViewItem(HWND hWnd, int nOldIndex, int nNewIndex)
{
  wchar_t wszText[MAX_PATH];
  LVITEMW lvi;
  DWORD dwStyle;
  int nIndex;

  //Get item text
  lvi.mask=LVIF_TEXT;
  lvi.pszText=wszText;
  lvi.cchTextMax=MAX_PATH;
  lvi.iItem=nOldIndex;
  lvi.iSubItem=0;
  ListView_GetItemWide(hWnd, &lvi);

  //Delete item
  SendMessage(hWnd, LVM_DELETEITEM, nOldIndex, 0);

  //Insert item
  dwStyle=(DWORD)GetWindowLongPtrWide(hWnd, GWL_STYLE);
  SetWindowLongPtrWide(hWnd, GWL_STYLE, dwStyle & ~LVS_SORTASCENDING);

  lvi.mask=LVIF_TEXT;
  lvi.pszText=wszText;
  lvi.iItem=nNewIndex;
  lvi.iSubItem=0;
  nIndex=ListView_InsertItemWide(hWnd, &lvi);

  SetWindowLongPtrWide(hWnd, GWL_STYLE, dwStyle);

  return nIndex;
}

BOOL IsCaretAtLastLine()
{
  EDITINFO ei;
  AECHARINDEX ciCaret;
  AECHARINDEX ciLastChar;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    SendMessage(ei.hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
    SendMessage(ei.hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ciLastChar);
    if (ciCaret.nLine >= ciLastChar.nLine)
      return TRUE;
  }
  return FALSE;
}

BOOL IsCaretAtLastEmptyLine()
{
  EDITINFO ei;
  AECHARINDEX ciCaret;
  AECHARINDEX ciLastChar;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    SendMessage(ei.hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
    SendMessage(ei.hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ciLastChar);
    if (AEC_IndexCompare(&ciCaret, &ciLastChar) >= 0 && !ciLastChar.lpLine->nLineLen)
      return TRUE;
  }
  return FALSE;
}

void PosWindowToCorner(HWND hWndOwner, HWND hWndChild)
{
  RECT rcChild;
  RECT rcOwner;

  GetClientRect(hWndOwner, &rcOwner);
  ClientToScreen(hWndOwner, (POINT *)&rcOwner.left);
  ClientToScreen(hWndOwner, (POINT *)&rcOwner.right);
  GetWindowRect(hWndChild, &rcChild);
  SetWindowPos(hWndChild, NULL, rcOwner.right - (rcChild.right - rcChild.left), rcOwner.top, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
}

int GetBaseName(const wchar_t *wpFile, int nFileLen, wchar_t *wszBaseName, int nBaseNameMax)
{
  const wchar_t *wpCount;
  const wchar_t *wpExt=NULL;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
      break;
    if (!wpExt && *wpCount == L'.')
      wpExt=wpCount;
  }
  ++wpCount;
  if (!wpExt) wpExt=wpFile + nFileLen;
  return (int)xstrcpynW(wszBaseName, wpCount, min(nBaseNameMax, wpExt - wpCount + 1));
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
    WideOption(hOptions, L"/WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
    WideOption(hOptions, L"/ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
    WideOption(hOptions, L"/ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));

    WideOption(hOptions, L"/ViewWindowRect", PO_BINARY, (LPBYTE)&rcViewMainCurrentDialog, sizeof(RECT));
    WideOption(hOptions, L"/ViewColumnWidth1", PO_DWORD, (LPBYTE)&nViewColumnWidth1, sizeof(DWORD));
    WideOption(hOptions, L"/ViewColumnWidth2", PO_DWORD, (LPBYTE)&nViewColumnWidth2, sizeof(DWORD));

    //Macro hotkeys
    {
      WIN32_FIND_DATAW wfd;
      HANDLE hFind;
      wchar_t wszFindFiles[MAX_PATH];
      wchar_t wszBaseName[MAX_PATH];

      xprintfW(wszFindFiles, L"%s\\*.macro", wszMacrosDir);

      if ((hFind=FindFirstFileWide(wszFindFiles, &wfd)) != INVALID_HANDLE_VALUE)
      {
        do
        {
          GetBaseName(wfd.cFileName, -1, wszBaseName, MAX_PATH);

          //Read and register hotkey
          {
            DWORD dwHotkey=0;

            if (hOptions)
              WideOption(hOptions, wszBaseName, PO_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD));
            if (dwHotkey)
              RegisterHotkey(wszBaseName, LOWORD(dwHotkey));
          }
        }
        while (FindNextFileWide(hFind, &wfd));

        FindClose(hFind);
      }
    }
    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE|(dwFlags & OF_HOTKEYS?POB_CLEAR:0), (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_RECT)
    {
      WideOption(hOptions, L"/WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"/ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"/ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
    }
    if (dwFlags & OF_VIEWRECT)
    {
      WideOption(hOptions, L"/ViewWindowRect", PO_BINARY, (LPBYTE)&rcViewMainCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"/ViewColumnWidth1", PO_DWORD, (LPBYTE)&nViewColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"/ViewColumnWidth2", PO_DWORD, (LPBYTE)&nViewColumnWidth2, sizeof(DWORD));
    }
    if (dwFlags & OF_HOTKEYS)
    {
      wchar_t wszFile[MAX_PATH];
      LVITEMW lvi;
      int i=1;

      for (;;)
      {
        //Get macro name
        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszFile;
        lvi.cchTextMax=MAX_PATH;
        lvi.iItem=i;
        lvi.iSubItem=LVI_MACRO_FILE;
        if (!ListView_GetItemWide(hWndMacrosList, &lvi)) break;

        //Get macro hotkey
        lvi.mask=LVIF_PARAM;
        lvi.iItem=i;
        lvi.iSubItem=LVI_MACRO_FILE;
        if (!ListView_GetItemWide(hWndMacrosList, &lvi)) break;

        //Save hotkey
        if (lvi.lParam)
        {
          WideOption(hOptions, wszFile, PO_DWORD, (LPBYTE)&lvi.lParam, sizeof(DWORD));
        }

        ++i;
      }
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
    if (nStringID == STRID_HOTKEY_EXISTS)
      return L"%s\n\n\x0413\x043E\x0440\x044F\x0447\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430\x0020\x0443\x0436\x0435\x0020\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x0430\x0020\x0434\x043B\x044F\x0020\x0022\x0025\x0073\x0022\x002E";
    if (nStringID == STRID_CANNOT_OPENFILE)
      return L"\x041D\x0435\x0432\x043E\x0437\x043C\x043E\x0436\x043D\x043E\x0020\x043E\x0442\x043A\x0440\x044B\x0442\x044C\x0020\x0444\x0430\x0439\x043B\x002E";
    if (nStringID == STRID_CANNOT_HOOK)
      return L"\x041D\x0435\x0432\x043E\x0437\x043C\x043E\x0436\x043D\x043E\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x0445\x0443\x043A\x002E";
    if (nStringID == STRID_ALREADY_EXIST)
      return L"\x041C\x0430\x043A\x0440\x043E\x0441\x0020\x0443\x0436\x0435\x0020\x0441\x0443\x0449\x0435\x0441\x0442\x0432\x0443\x0435\x0442\x002E";
    if (nStringID == STRID_CONFIRM_DELETE)
      return L"\x0412\x044B\x0020\x0443\x0432\x0435\x0440\x0435\x043D\x044B\x002C\x0020\x0447\x0442\x043E\x0020\x0445\x043E\x0442\x0438\x0442\x0435\x0020\x0443\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x043C\x0430\x043A\x0440\x043E\x0441\x003F";
    if (nStringID == STRID_THREAD_BUSY)
      return L"\x0413\x043B\x0430\x0432\x043D\x044B\x0439\x0020\x043F\x043E\x0442\x043E\x043A\x0020\x0437\x0430\x043D\x044F\x0442\x002E";
    if (nStringID == STRID_NORECORD)
      return L"\x041D\x0435\x0442\x0020\x043C\x0430\x043A\x0440\x043E\x0441\x0430\x0020\x0434\x043B\x044F\x0020\x0432\x043E\x0441\x043F\x0440\x043E\x0438\x0437\x0432\x0435\x0434\x0435\x043D\x0438\x044F\x002E";
    if (nStringID == STRID_MACRO)
      return L"\x041C\x0430\x043A\x0440\x043E\x0441";
    if (nStringID == STRID_HOTKEY)
      return L"\x0413\x043E\x0440\x044F\x0447\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_RECORD_LISTITEM)
      return L"\x003C\x0417\x0430\x043F\x0438\x0441\x044C\x003E";
    if (nStringID == STRID_RECORD)
      return L"\x0417\x0430\x043F\x0438\x0441\x044C";
    if (nStringID == STRID_SAVE)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x0438\x0442\x044C";
    if (nStringID == STRID_DELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_ASSIGN)
      return L"\x041D\x0430\x0437\x043D\x0430\x0447\x0438\x0442\x044C";
    if (nStringID == STRID_VIEW)
      return L"\x041F\x0440\x043E\x0441\x043C\x043E\x0442\x0440...";
    if (nStringID == STRID_KEY)
      return L"\x041A\x043B\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_ACT)
      return L"\x0414\x0435\x0439\x0441\x0442\x0432\x0438\x0435";
    if (nStringID == STRID_EXPORT)
      return L"\x042D\x043A\x0441\x043F\x043E\x0440\x0442";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_CLOSE)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C";
  }
  else
  {
    if (nStringID == STRID_HOTKEY_EXISTS)
      return L"%s\n\nHotkey already assigned to \"%s\".";
    if (nStringID == STRID_CANNOT_OPENFILE)
      return L"Cannot open file.";
    if (nStringID == STRID_CANNOT_HOOK)
      return L"Cannot install hook.";
    if (nStringID == STRID_ALREADY_EXIST)
      return L"Macro already exists.";
    if (nStringID == STRID_CONFIRM_DELETE)
      return L"Are you sure, you want to delete macro?";
    if (nStringID == STRID_THREAD_BUSY)
      return L"Main thread is busy.";
    if (nStringID == STRID_NORECORD)
      return L"No macro for playing.";
    if (nStringID == STRID_MACRO)
      return L"Macros";
    if (nStringID == STRID_HOTKEY)
      return L"Hotkey";
    if (nStringID == STRID_RECORD_LISTITEM)
      return L"<Record>";
    if (nStringID == STRID_RECORD)
      return L"Record";
    if (nStringID == STRID_SAVE)
      return L"Save";
    if (nStringID == STRID_DELETE)
      return L"Delete";
    if (nStringID == STRID_ASSIGN)
      return L"Assign";
    if (nStringID == STRID_VIEW)
      return L"View...";
    if (nStringID == STRID_KEY)
      return L"Key";
    if (nStringID == STRID_ACT)
      return L"Action";
    if (nStringID == STRID_EXPORT)
      return L"Export";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_CLOSE)
      return L"Close";
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
  xprintfW(wszMacrosDir, L"%s\\AkelFiles\\Plugs\\Macros", pd->wszAkelDir);
  xprintfW(wszScriptsExecMutex, L"AkelPad::Scripts::MutexExec::%d", GetCurrentProcessId());
  ReadOptions(0);

  dwMainProcessId=GetCurrentProcessId();
  dwMainThreadId=GetCurrentThreadId();
}

void InitMain()
{
  bInitMain=TRUE;

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

  if (nMDI == WMD_MDI)
  {
    NewFrameProcData=NULL;
    SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewFrameProc, (LPARAM)&NewFrameProcData);
  }
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
  if (NewFrameProcData)
  {
    SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&NewFrameProcData);
    NewFrameProcData=NULL;
  }

  StackHotkeyFree(&hRecordStack);
  StackHotkeyFree(&hLastMacroStack);
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
