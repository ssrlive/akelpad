#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlobj.h>
#include <richedit.h>
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "Resources\resource.h"


//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"

//Include stack functions
#define StackGetElement
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackDelete
#define StackMoveAfter
#define StackMoveBefore
#define StackMoveIndex
#define StackCopy
#define StackJoin
#define StackSplit
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemset
#define xmemcmp
#define xarrlenA
#define xarrlenW
#define xstrcmpiW
#define xstrcmpinW
#define xstrcmpnW
#define xstrcpynW
#define xstrlenW
#define xstrcpyW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define AppendMenuWide
#define CallWindowProcWide
#define ComboBox_AddStringWide
#define ComboBox_FindStringExactWide
#define ComboBox_GetLBTextWide
#define ComboBox_InsertStringWide
#define CreateDialogWide
#define CreateDirectoryWide
#define CreateFileWide
#define DefWindowProcWide
#define DeleteFileWide
#define DialogBoxParamWide
#define DialogBoxWide
#define DispatchMessageWide
#define DragQueryFileWide
#define ExpandEnvironmentStringsWide
#define FileExistsWide
#define FindFirstFileWide
#define FindNextFileWide
#define FindWindowExWide
#define GetFileAttributesWide
#define GetMessageWide
#define GetOpenFileNameWide
#define GetWindowLongPtrWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define IsDialogMessageWide
#define ListBox_AddStringWide
#define ListBox_GetTextWide
#define ListBox_InsertStringWide
#define ModifyMenuWide
#define MoveFileWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define TabCtrl_GetItemWide
#define TranslateAcceleratorWide
#define TreeView_GetItemWide
#define TreeView_InsertItemWide
#define TreeView_SetItemWide
#include "WideFunc.h"

//Defines
#define STRID_PARSEMSG_NOOPENBRACKET  1
#define STRID_PARSEMSG_NOCLOSEBRACKET 2
#define STRID_SESSION                 3
#define STRID_OPEN                    4
#define STRID_UPDATE                  5
#define STRID_SAVE                    6
#define STRID_EDIT                    7
#define STRID_COPY                    8
#define STRID_RENAME                  9
#define STRID_DELETE                  10
#define STRID_ADDDIR                  11
#define STRID_ITEMNAME                12
#define STRID_ITEMFILE                13
#define STRID_MENU_OPEN               14
#define STRID_MENU_ACTIVATE           15
#define STRID_MENU_CLOSE              16
#define STRID_MENU_ADDCURFILE         17
#define STRID_MENU_ADDFILE            18
#define STRID_MENU_ADDDIR             19
#define STRID_MENU_RENAME             20
#define STRID_MENU_MOVEDOWN           21
#define STRID_MENU_MOVEUP             22
#define STRID_MENU_DELETE             23
#define STRID_MENU_DELETEOLD          24
#define STRID_NEWNAME                 25
#define STRID_SETTINGS                26
#define STRID_SAVESESSIONS            27
#define STRID_PROGRAMDIR              28
#define STRID_APPDATA                 29
#define STRID_OPENONSTART             30
#define STRID_SAVEONEXIT              31
#define STRID_SHOWPATH                32
#define STRID_SAVERELATIVE            33
#define STRID_SYSTEMFONT              34
#define STRID_CODERTHEME              35
#define STRID_DIALOGTYPE              36
#define STRID_MODALDIALOG             37
#define STRID_MODELESSDIALOG          38
#define STRID_DOCKABLEDIALOG          39
#define STRID_AUTOLOAD                40
#define STRID_SAVEDATA                41
#define STRID_SAVEACTIVE              42
#define STRID_SAVECODEPAGE            43
#define STRID_SAVESELECTION           44
#define STRID_SAVEWORDWRAP            45
#define STRID_SAVEREADONLY            46
#define STRID_SAVEOVERTYPE            47
#define STRID_SAVEBOOKMARKS           48
#define STRID_SAVEALIAS               49
#define STRID_SAVEFOLDS               50
#define STRID_SAVEMARKS               51
#define STRID_PLUGIN                  52
#define STRID_OK                      53
#define STRID_CANCEL                  54
#define STRID_CLOSE                   55
#define STRID_CURRENTSESSION          56
#define STRID_ALREADY_EXIST           57
#define STRID_RENAME_ERROR            58
#define STRID_SESSION_CHANGED         59
#define STRID_CONFIRM_DELETE          60
#define STRID_RESTARTPROGRAM          61
#define STRID_SDI_ISNTSUPPORTED       62
#define STRID_FILTER                  63
#define STRID_DROPTOCURRENT           64

#define DLLA_SESSIONS_OPEN          1
#define DLLA_SESSIONS_SAVE          2
#define DLLA_SESSIONS_CLOSE         3
#define DLLA_SESSIONS_EDIT          4
#define DLLA_SESSIONS_DELETE        5
#define DLLA_SESSIONS_STARTSTOP     10

#define OF_RECT        0x1
#define OF_SETTINGS    0x2

#define BUFFER_SIZE 8192

//Folder flags
#define FLDF_OPEN     0x1
#define FLDF_CLOSE    0x2

//Save directory
#define SDIR_PROGRAM      1
#define SDIR_APPDATA      2

//Save data in session
#define SSD_ACTIVE     0x00000001
#define SSD_CODEPAGE   0x00000002
#define SSD_SELECTION  0x00000004
#define SSD_WORDWRAP   0x00000010
#define SSD_READONLY   0x00000020
#define SSD_OVERTYPE   0x00000040
#define SSD_BOOKMARKS  0x00000080
#define SSD_CODERALIAS 0x00000100
#define SSD_CODERFOLDS 0x00000200
#define SSD_CODERMARKS 0x00000400

//Dialog type
#define DLGT_MODAL    1
#define DLGT_MODELESS 2
#define DLGT_DOCKABLE 3

//DestroyDock type
#define DKT_DEFAULT        0x0
#define DKT_NOUNLOAD       0x1
#define DKT_ONMAINFINISH   0x2
#define DKT_KEEPAUTOLOAD   0x4

//Combobox edit ID
#define IDC_COMBOBOXEDIT       1001

#ifndef INVALID_FILE_ATTRIBUTES
  #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

//Dialog messages
#define AKDLL_SETUP             (WM_USER + 100)
#define AKDLL_UPDATESAVEBUTTON  (WM_USER + 101)
#define AKDLL_GETCARETITEM      (WM_USER + 102)

typedef struct _SESSIONITEM {
  struct _SESSIONITEM *next;
  struct _SESSIONITEM *prev;
  struct _SESSIONITEM *parent;
  struct _SESSIONITEM *firstChild;
  struct _SESSIONITEM *lastChild;
  DWORD dwFolderFlags;
  HTREEITEM hItem;
  DWORD dwState;
  BOOL bMoving;
  BOOL bDeleting;
  wchar_t wszName[MAX_PATH];
  wchar_t wszItemFile[MAX_PATH];
  wchar_t wszItemExpFile[MAX_PATH];
  int nTabActive;
  int nCodePage;
  INT_PTR nSelStart;
  INT_PTR nSelEnd;
  INT_PTR nFirstVisChar;
  BOOL bWordWrap;
  BOOL bReadOnly;
  BOOL bOvertypeMode;
  wchar_t *wszBookmarks;
  wchar_t *wszCoderAlias;
  wchar_t *wszCoderFolds;
  wchar_t *wszCoderMarks;
} SESSIONITEM;

typedef struct {
  SESSIONITEM *first;
  SESSIONITEM *last;
  int nElements;
} STACKSESSIONITEM;

typedef struct _SESSION {
  struct _SESSION *next;
  struct _SESSION *prev;
  wchar_t wszSessionName[MAX_PATH];
  STACKSESSIONITEM hItemsStack;
  BOOL bModified;
} SESSION;

typedef struct {
  SESSION *first;
  SESSION *last;
} STACKSESSION;

//Add session item
#define ASI_FIRST   ((SESSIONITEM *)(INT_PTR)1)
#define ASI_LAST    ((SESSIONITEM *)(INT_PTR)2)

//LineBoard external call
typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  wchar_t *wszBookmarks;
  int *lpnBookmarksLen;
} DLLEXTLINEBOARD;

#define DLLA_LINEBOARD_GETBOOKMARKS  12
#define DLLA_LINEBOARD_SETBOOKMARKS  13
#define DLLA_LINEBOARD_DELBOOKMARKS  14

//Coder structures
typedef struct _MARKTEXT {
  struct _MARKTEXT *next;
  struct _MARKTEXT *prev;
  AEHMARKTEXT hMarkTextHandle;
  DWORD dwMarkID;
} MARKTEXT;

typedef struct {
  MARKTEXT *first;
  MARKTEXT *last;
} STACKMARKTEXT;

typedef struct {
  const wchar_t *wpVarName;
  INT_PTR nVarValue;
  DWORD dwVarFlags;         //See VARF_* defines.
} CODERTHEMEITEM;

//Coder external call
typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  wchar_t *wpAlias;
} DLLEXTCODERSETALIAS;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  wchar_t *wpAlias;
  INT_PTR *lpnAliasLen;
} DLLEXTCODERGETALIAS;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  STACKMARKTEXT *lpMarkTextsStack;
} DLLEXTHIGHLIGHTGETMARKSTACK;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  wchar_t *wpColorText;
  wchar_t *wpColorBk;
  UINT_PTR dwFlags;
  UINT_PTR dwFontStyle;
  UINT_PTR dwMarkID;
  wchar_t *wpMarkText;
  INT_PTR nMarkTextLen;
} DLLEXTHIGHLIGHTMARK;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  CODERTHEMEITEM *cti;
} DLLEXTCODERFILLVARLIST;

#define DLLA_CODER_SETALIAS         6
#define DLLA_CODER_GETALIAS         18
#define DLLA_CODER_FILLVARLIST      23
#define DLLA_HIGHLIGHT_MARK         2
#define DLLA_HIGHLIGHT_GETMARKSTACK 12

#define MARKID_SELECTION  (DWORD)-2

#define MARKFLAG_MATCHCASE 0x1
#define MARKFLAG_REGEXP    0x2
#define MARKFLAG_WHOLEWORD 0x4

//Variable flags
#define VARF_LOWPRIORITY   0x001 //Global variable has low priority.
                                 //Next flags for DLLA_CODER_FILLVARLIST:
#define VARF_EXTSTRING     0x100 //Copy string pointer to (const wchar_t *)CODERTHEMEITEM.nVarValue.
#define VARF_EXTINTCOLOR   0x200 //Copy color integer to (COLORREF)CODERTHEMEITEM.nVarValue or -1 if not color.
#define VARF_EXTLPINTCOLOR 0x400 //Copy color integer to (COLORREF *)CODERTHEMEITEM.nVarValue or -1 if not color.

//Functions prototypes
void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow);
void DestroyDock(HWND hWndDock, DWORD dwType);
LRESULT CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK InputBoxDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ItemEditDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewTreeViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL LoadSessionFile(SESSION *ss);
BOOL RenameSessionFile(const wchar_t *wpOldSessionName, const wchar_t *wpNewSessionName);
void OpenSessionFile(const wchar_t *wpSessionName);
void CloseSessionFile(const wchar_t *wpSessionName);
BOOL EditSessionFile(const wchar_t *wpSessionName);
void SaveSessionFile(SESSION *ss);
void SaveCurrentSession(const wchar_t *wpSessionName);
BOOL DeleteSessionFile(const wchar_t *wpSessionName);
int SaveSessionPrompt();

void LoadSessions(STACKSESSION *hStack);
SESSION* AddEmptySession(STACKSESSION *hStack, const wchar_t *wpSessionName);
SESSION* AddCurrentSession(STACKSESSION *hStack, const wchar_t *wpSessionName);
SESSION* GetSession(STACKSESSION *hStack, const wchar_t *wpSessionName);
void CopySessionLevel(SESSIONITEM *siFirstSource, SESSIONITEM *siLastSource, SESSIONITEM **psiFirstTarget, SESSIONITEM **psiLastTarget, SESSIONITEM *siTargetParent);
void CopySession(SESSION *ssSource, SESSION *ssTarget);
void OpenSession(STACKSESSIONITEM *hStack);
void CloseSession(STACKSESSIONITEM *hStack);
void DeleteSession(STACKSESSION *hStack, SESSION *ss);
void FreeSessions(STACKSESSION *hStack);

SESSIONITEM* StackAddItem(SESSION *ss, SESSIONITEM *siParent, SESSIONITEM *siAfterThis);
int StackItemIndex(SESSION *ss, SESSIONITEM *si);
BOOL StackFindSelRange(SESSION *ss, SESSIONITEM *lpItemInRange, SESSIONITEM **lppStartRangeItem, int *lpnStartRangeIndex, SESSIONITEM **lppEndRangeItem, int *lpnEndRangeIndex);
SESSIONITEM* StackNextItem(SESSIONITEM *siItem, SESSIONITEM *siRoot);
SESSIONITEM* StackNextItemNoChild(SESSIONITEM *si);
SESSIONITEM* StackPrevItem(SESSIONITEM *siItem, SESSIONITEM *siRoot);
SESSIONITEM* StackPrevItemNoChild(SESSIONITEM *si);
void StackDeleteItem(SESSION *ss, SESSIONITEM *siItem);
void StackFreeItems(SESSION *ss);

void OpenItem(SESSIONITEM *si);
BOOL CloseItem(FRAMEDATA *lpFrame, BOOL bSingle);
BOOL SelectItem(HWND hWnd, SESSIONITEM *si, BOOL bSelect);

void ComboFillSessions(STACKSESSION *hStack, HWND hWnd);
int MoveComboBoxItem(HWND hWnd, int nOldIndex, int nNewIndex);
int UpdateComboBoxDropWidth(HWND hWnd);

void TreeFillItemsCurrent(HWND hWndTreeView);
void TreeAddItem(HWND hWndTreeView, SESSIONITEM *si, HTREEITEM hInsertAfter, DWORD dwState);
void TreeAddChild(HWND hWndTreeView, SESSION *ss, SESSIONITEM *si);
BOOL TreeMoveItem(HWND hWndTreeView, HTREEITEM hItemCaret, SESSION *ss, SESSIONITEM *siItem, SESSIONITEM *siParent, SESSIONITEM *siAfterThis);
BOOL TreeDeleteItem(HWND hWndTreeView, HTREEITEM hItem);
void TreeUpdateItem(HWND hWndTreeView, HTREEITEM hItem);
LPARAM TreeItemParam(HWND hWndTreeView, HTREEITEM hItem);
HTREEITEM TreeCursorItem(HWND hWndTreeView);
void ClearTreeView(HWND hWndTreeView, BOOL bRedraw);

void GetCoderColors(HWND hWnd);
void SetListColors(HWND hWndEdit, HWND hWndList);
int GetCollapsedFoldsString(HSTACK *hFoldsStack, wchar_t *wszString);
int SetCollapsedFoldsString(HWND hWnd, HSTACK *hFoldsStack, const wchar_t *wpString);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);
INT_PTR EscapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput);
INT_PTR UnescapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput);
INT_PTR GetEscapeParam(const wchar_t *wpText, const wchar_t **wpParamStart, const wchar_t **wpParamEnd, const wchar_t **wpTextNext);
BOOL NextLine(const wchar_t **wppText);
void SkipSpaces(const wchar_t **wppText);

BOOL IsPathFull(const wchar_t *wpPath);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
int GetBaseName(const wchar_t *wpFile, int nFileLen, wchar_t *wszBaseName, int nBaseNameMax);
BOOL CreateDirectoryRecursive(const wchar_t *wpPath);
void DropFiles(HDROP hDrop, HWND hWndItemsList);
int IsFile(const wchar_t *wpFile);
LRESULT SendToDoc(AEHDOC hDocEdit, HWND hWndEdit, UINT uMsg, WPARAM wParam, LPARAM lParam);
COLORREF AE_ColorBrightness(COLORREF crColor, int nPercent);
BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc);

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

//GetProcAddress
BOOL (WINAPI *SHGetSpecialFolderPathAPtr)(HWND, char *, int, BOOL);
BOOL (WINAPI *SHGetSpecialFolderPathWPtr)(HWND, wchar_t *, int, BOOL);

//Global variables
char szBuffer[BUFFER_SIZE];
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
wchar_t wszExeDir[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hTab;
HICON hMainIcon;
HACCEL hGlobalAccel;
BOOL bOldWindows;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
BOOL bInMemory;
HICON hPluginIcon;
HWND hWndMainDlg=NULL;
HWND hWndItemsList=NULL;
RECT rcMainMinMaxDialog={275, 388, 0, 0};
RECT rcMainCurrentDialog={0};

HWND hWndDockDlg=NULL;
RECT rcSessionsDockRect={0};
int nSessionsDockSide=DKS_LEFT;
DOCK *dkSessionsDlg=NULL;
BOOL bSessionsDockWaitResize=FALSE;

wchar_t wszSessionsDir[MAX_PATH]=L"";
wchar_t wszOpenOnStart[MAX_PATH];
wchar_t wszSaveOnExit[MAX_PATH];
wchar_t wszDlgInputBox[MAX_PATH];
STACKSESSION hSessionStack={0};
SESSION *lpVirtualSession=NULL;
SESSION *lpRealSession=NULL;
int nCurrentSessionIndex=CB_ERR;
int nSaveSessions=SDIR_PROGRAM;
BOOL bOpenOnStart=FALSE;
BOOL bSaveOnExit=FALSE;
int nDialogType=DLGT_MODAL;
int nNewDialogType=0;
BOOL bDockAutoload=TRUE;
COLORREF crListTextColor;
COLORREF crListBkColor;
CODERTHEMEITEM cti[]={{L"Sessions_ListTextColor", (INT_PTR)&crListTextColor, VARF_EXTLPINTCOLOR},
                      {L"Sessions_ListBkColor",   (INT_PTR)&crListBkColor,   VARF_EXTLPINTCOLOR},
                      {0, 0, 0}};
BOOL bCoderTheme=TRUE;
BOOL bListSystemFont=TRUE;
BOOL bShowPath=FALSE;
BOOL bItemOpening=FALSE;
BOOL bSelChanged=FALSE;
BOOL bDragging=FALSE;
int nMouseMove=0;
HCURSOR hCursorDragMove;
SESSIONITEM *siDraggingCursor=NULL;
HIMAGELIST hImageList=NULL;
DWORD dwSaveData=SSD_ACTIVE|SSD_CODEPAGE|SSD_SELECTION|SSD_WORDWRAP|SSD_READONLY|SSD_OVERTYPE|SSD_BOOKMARKS|SSD_CODERALIAS|SSD_CODERFOLDS|SSD_CODERMARKS;
BOOL bSaveRelative=TRUE;
WNDPROC OldTreeViewProc;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewEditProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Sessions";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOSDI;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);
  bInMemory=pd->bInMemory;

  if (nMDI == WMD_SDI)
  {
    if (!pd->bOnStart)
      MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_SDI_ISNTSUPPORTED), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
    return;
  }
  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_SESSIONS_STARTSTOP)
    {
      if (bInitMain)
      {
        if (hWndMainDlg) SendMessage(hWndMainDlg, WM_COMMAND, IDCANCEL, 0);
        UninitMain();

        //Unload plugin
        pd->nUnload=UD_UNLOAD;
      }
      else
      {
        InitMain();

        //Stay in memory, and show as active
        pd->nUnload=UD_NONUNLOAD_ACTIVE;
      }
      return;
    }
    else if (nAction == DLLA_SESSIONS_OPEN ||
             nAction == DLLA_SESSIONS_SAVE ||
             nAction == DLLA_SESSIONS_CLOSE ||
             nAction == DLLA_SESSIONS_EDIT ||
             nAction == DLLA_SESSIONS_DELETE)
    {
      unsigned char *pSession=NULL;
      wchar_t wszSession[MAX_PATH];

      if (IsExtCallParamValid(pd->lParam, 2))
        pSession=(unsigned char *)GetExtCallParam(pd->lParam, 2);

      if (pSession)
      {
        if (pd->dwSupport & PDS_STRANSI)
          MultiByteToWideChar(CP_ACP, 0, (char *)pSession, -1, wszSession, MAX_PATH);
        else
          xstrcpynW(wszSession, (wchar_t *)pSession, MAX_PATH);
        if (*wszSession)
        {
          if (nAction == DLLA_SESSIONS_OPEN)
            OpenSessionFile(wszSession);
          else if (nAction == DLLA_SESSIONS_SAVE)
            SaveCurrentSession(wszSession);
          else if (nAction == DLLA_SESSIONS_CLOSE)
            CloseSessionFile(wszSession);
          else if (nAction == DLLA_SESSIONS_EDIT)
            EditSessionFile(wszSession);
          else if (nAction == DLLA_SESSIONS_DELETE)
            DeleteSessionFile(wszSession);
        }
      }
    }

    //If plugin already loaded, stay in memory and don't change active status
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
    return;
  }

  //Initialize
  if (!bInitMain) InitMain();

  if (nDialogType == DLGT_DOCKABLE)
  {
    if (hWndDockDlg)
    {
      DestroyDock(hWndDockDlg, DKT_DEFAULT);
    }
    else
    {
      if (!pd->bOnStart || bDockAutoload)
      {
        bSessionsDockWaitResize=pd->bOnStart;
        CreateDock(&hWndDockDlg, &dkSessionsDlg, !bSessionsDockWaitResize);
      }
    }
  }
  else
  {
    if (!pd->bOnStart)
    {
      if (!hWndMainDlg)
      {
        if (nDialogType == DLGT_MODAL)
          DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SESSIONS), hMainWnd, (DLGPROC)MainDlgProc);
        else
          hWndMainDlg=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SESSIONS), hMainWnd, (DLGPROC)MainDlgProc);
      }
      else SetActiveWindow(hWndMainDlg);
    }
  }

  //Stay in memory, and show as active
  pd->nUnload=UD_NONUNLOAD_ACTIVE;
}

void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow)
{
  DOCK dk;

  *hWndDock=NULL;

  xmemset(&dk, 0, sizeof(DOCK));
  dk.dwFlags=DKF_DRAGDROP;
  dk.nSide=nSessionsDockSide;
  dk.rcSize=rcSessionsDockRect;

  if (*dkDock=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_ADD, (LPARAM)&dk))
  {
    *hWndDock=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_DOCK), hMainWnd, (DLGPROC)MainDlgProc);

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

LRESULT CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HMENU hMenuLabel;
  static HWND hWndTitleText;
  static HWND hWndTitleClose;
  static HWND hWndSessionAction;
  static HWND hWndSessionList;
  static HWND hWndGroupSession;
  static HWND hWndOpenButton;
  static HWND hWndCloseButton;
  static HWND hWndSaveButton;
  static HWND hWndEditButton;
  static HWND hWndCopyButton;
  static HWND hWndRenameButton;
  static HWND hWndDeleteButton;
  static HWND hWndSettingsButton;
  static HWND hWndOKButton;
  static HMENU hMenuItems;
  static HTREEITEM hItemCaretPrev;
  static SESSION ssCurrentTmp;
  static RESIZEDIALOG rds[]={{&hWndTitleText,      RDS_SIZE|RDS_X, 0},
                             {&hWndTitleClose,     RDS_MOVE|RDS_X, 0},
                             {&hWndSessionList,    RDS_SIZE|RDS_X, 0},
                             {&hWndItemsList,      RDS_SIZE|RDS_X, 0},
                             {&hWndItemsList,      RDS_SIZE|RDS_Y, 0},
                             {&hWndGroupSession,   RDS_MOVE|RDS_X, 0},
                             {&hWndOpenButton,     RDS_MOVE|RDS_X, 0},
                             {&hWndCloseButton,    RDS_MOVE|RDS_X, 0},
                             {&hWndSaveButton,     RDS_MOVE|RDS_X, 0},
                             {&hWndEditButton,     RDS_MOVE|RDS_X, 0},
                             {&hWndCopyButton,     RDS_MOVE|RDS_X, 0},
                             {&hWndRenameButton,   RDS_MOVE|RDS_X, 0},
                             {&hWndDeleteButton,   RDS_MOVE|RDS_X, 0},
                             {&hWndSettingsButton, RDS_MOVE|RDS_X, 0},
                             {&hWndOKButton,       RDS_MOVE|RDS_X, 0},
                             {&hWndOKButton,       RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    hWndMainDlg=hDlg;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndSessionList=GetDlgItem(hDlg, IDC_SESSION_LIST);
    hWndItemsList=GetDlgItem(hDlg, IDC_ITEMS_LIST);
    if (nDialogType == DLGT_DOCKABLE)
    {
      hWndTitleText=GetDlgItem(hDlg, IDC_TITLETEXT);
      hWndTitleClose=GetDlgItem(hDlg, IDC_CLOSE);
      hWndSessionAction=GetDlgItem(hDlg, IDC_SESSION_ACTION);
    }
    else
    {
      hWndGroupSession=GetDlgItem(hDlg, IDC_GROUP_SESSION);
      hWndOpenButton=GetDlgItem(hDlg, IDC_SESSION_OPEN);
      hWndCloseButton=GetDlgItem(hDlg, IDC_SESSION_CLOSE);
      hWndSaveButton=GetDlgItem(hDlg, IDC_SESSION_SAVE);
      hWndEditButton=GetDlgItem(hDlg, IDC_SESSION_EDIT);
      hWndCopyButton=GetDlgItem(hDlg, IDC_SESSION_COPY);
      hWndRenameButton=GetDlgItem(hDlg, IDC_SESSION_RENAME);
      hWndDeleteButton=GetDlgItem(hDlg, IDC_SESSION_DELETE);
      hWndSettingsButton=GetDlgItem(hDlg, IDC_SETTINGS);
      hWndOKButton=GetDlgItem(hDlg, IDOK);
    }
    if (nDialogType != DLGT_DOCKABLE)
    {
      SetWindowTextWide(hDlg, wszPluginTitle);
      SetDlgItemTextWide(hDlg, IDC_GROUP_SESSION, GetLangStringW(wLangModule, STRID_SESSION));
      SetDlgItemTextWide(hDlg, IDC_SESSION_OPEN, GetLangStringW(wLangModule, STRID_OPEN));
      SetDlgItemTextWide(hDlg, IDC_SESSION_CLOSE, GetLangStringW(wLangModule, STRID_CLOSE));
      SetDlgItemTextWide(hDlg, IDC_SESSION_SAVE, GetLangStringW(wLangModule, STRID_SAVE));
      SetDlgItemTextWide(hDlg, IDC_SESSION_EDIT, GetLangStringW(wLangModule, STRID_EDIT));
      SetDlgItemTextWide(hDlg, IDC_SESSION_COPY, GetLangStringW(wLangModule, STRID_COPY));
      SetDlgItemTextWide(hDlg, IDC_SESSION_RENAME, GetLangStringW(wLangModule, STRID_RENAME));
      SetDlgItemTextWide(hDlg, IDC_SESSION_DELETE, GetLangStringW(wLangModule, STRID_DELETE));
      SetDlgItemTextWide(hDlg, IDC_SETTINGS, GetLangStringW(wLangModule, STRID_SETTINGS));
      SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    }

    LoadSessions(&hSessionStack);
    ComboFillSessions(&hSessionStack, hWndSessionList);
    ComboBox_InsertStringWide(hWndSessionList, 0, GetLangStringW(wLangModule, STRID_CURRENTSESSION));
    SendMessage(hWndSessionList, CB_SETCURSEL, 0, 0);
    PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_SESSION_LIST, CBN_SELCHANGE), (LPARAM)hWndSessionList);

    //Popup menu
    if (hMenuItems=CreatePopupMenu())
    {
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_OPEN, GetLangStringW(wLangModule, STRID_MENU_OPEN));
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_CLOSE, GetLangStringW(wLangModule, STRID_MENU_CLOSE));
      AppendMenuWide(hMenuItems, MF_SEPARATOR, (UINT)-1, NULL);
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_ADDCURFILE, GetLangStringW(wLangModule, STRID_MENU_ADDCURFILE));
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_ADDFILES, GetLangStringW(wLangModule, STRID_MENU_ADDFILE));
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_ADDDIR, GetLangStringW(wLangModule, STRID_MENU_ADDDIR));
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_RENAME, GetLangStringW(wLangModule, STRID_MENU_RENAME));
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_MOVEUP, GetLangStringW(wLangModule, STRID_MENU_MOVEUP));
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_MOVEDOWN, GetLangStringW(wLangModule, STRID_MENU_MOVEDOWN));
      AppendMenuWide(hMenuItems, MF_SEPARATOR, (UINT)-1, NULL);
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_DELETE, GetLangStringW(wLangModule, STRID_MENU_DELETE));
      AppendMenuWide(hMenuItems, MF_STRING, IDC_ITEM_DELETEOLD, GetLangStringW(wLangModule, STRID_MENU_DELETEOLD));
    }

    //Set dock title
    if (nDialogType == DLGT_DOCKABLE)
    {
      BUTTONDRAW bd={BIF_CROSS|BIF_ETCHED};

      SetWindowTextWide(hWndTitleText, wszPluginName);
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndTitleClose, (LPARAM)&bd);

      bd.dwFlags=BIF_DOWNARROW|BIF_ENABLEFOCUS;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndSessionAction, (LPARAM)&bd);

      if (hMenuLabel=CreatePopupMenu())
      {
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SESSION_OPEN, GetLangStringW(wLangModule, STRID_OPEN));
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SESSION_CLOSE, GetLangStringW(wLangModule, STRID_CLOSE));
        AppendMenuWide(hMenuLabel, MF_SEPARATOR, (UINT)-1, NULL);
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SESSION_SAVE, GetLangStringW(wLangModule, STRID_SAVE));
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SESSION_EDIT, GetLangStringW(wLangModule, STRID_EDIT));
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SESSION_COPY, GetLangStringW(wLangModule, STRID_COPY));
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SESSION_RENAME, GetLangStringW(wLangModule, STRID_RENAME));
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SESSION_DELETE, GetLangStringW(wLangModule, STRID_DELETE));
        AppendMenuWide(hMenuLabel, MF_SEPARATOR, (UINT)-1, NULL);
        AppendMenuWide(hMenuLabel, MF_STRING, IDC_SETTINGS, GetLangStringW(wLangModule, STRID_SETTINGS));
      }
    }

    if (!bListSystemFont)
    {
      HFONT hFontEdit;

      hFontEdit=(HFONT)SendMessage(hMainWnd, AKD_GETFONT, (WPARAM)NULL, (LPARAM)NULL);
      SendMessage(hWndItemsList, WM_SETFONT, (WPARAM)hFontEdit, FALSE);
    }
    SetListColors(NULL, hWndItemsList);

    ImageList_SetBkColor(hImageList, (COLORREF)SendMessage(hWndItemsList, TVM_GETBKCOLOR, 0, 0));
    SendMessage(hWndItemsList, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImageList);

    //Items list can accept Drag'n'Drop
    DragAcceptFiles(hWndItemsList, TRUE);

    //SubClass listbox
    OldTreeViewProc=(WNDPROC)GetWindowLongPtrWide(hWndItemsList, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndItemsList, GWLP_WNDPROC, (UINT_PTR)NewTreeViewProc);

    if (nDialogType == DLGT_MODELESS)
      SendMessage(hMainWnd, AKD_SETMODELESS, (WPARAM)hDlg, MLA_ADD);
  }
  else if (uMsg == AKDLL_SETUP)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETTINGS), hDlg, (DLGPROC)SettingsDlgProc);
  }
  else if (uMsg == AKDLL_UPDATESAVEBUTTON)
  {
    if (nDialogType != DLGT_DOCKABLE)
    {
      BOOL bEnable=FALSE;

      if (!lpVirtualSession || lpVirtualSession->bModified)
        bEnable=TRUE;
      if (!bEnable)
      {
        if (GetFocus() == hWndSaveButton)
          SetFocus(hWndSessionList);
      }
      EnableWindow(hWndSaveButton, bEnable);
      EnableWindow(hWndCopyButton, !bEnable);
    }
  }
  else if (uMsg == AKDLL_GETCARETITEM)
  {
    hItemCaretPrev=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndItemsList)
    {
      RECT rcItem;
      POINT pt;

      if (lParam == -1)
      {
        *(HTREEITEM *)&rcItem=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
        SendMessage(hWndItemsList, TVM_GETITEMRECT, TRUE, (LPARAM)&rcItem);
        pt.x=rcItem.left;
        pt.y=rcItem.bottom;
        ClientToScreen(hWndItemsList, &pt);
      }
      else GetCursorPos(&pt);

      if (!lpVirtualSession)
      {
        HTREEITEM hItemCaret=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);

        ModifyMenuWide(hMenuItems, IDC_ITEM_OPEN, MF_BYCOMMAND|MF_STRING, IDC_ITEM_OPEN, GetLangStringW(wLangModule, STRID_MENU_ACTIVATE));
        EnableMenuItem(hMenuItems, IDC_ITEM_OPEN, hItemCaret?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_CLOSE, hItemCaret?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_ADDCURFILE, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_ADDFILES, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_ADDDIR, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_RENAME, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_MOVEUP, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_MOVEDOWN, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_DELETE, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_DELETEOLD, MF_GRAYED);
      }
      else
      {
        SESSIONITEM *lpCount;
        wchar_t *wpFile;
        int nSelCount=0;
        int nSelFilesCount=0;

        for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
        {
          if (lpCount->dwState & TVIS_SELECTED)
          {
            if (!lpCount->dwFolderFlags)
              ++nSelFilesCount;
            ++nSelCount;
          }
        }
        ModifyMenuWide(hMenuItems, IDC_ITEM_OPEN, MF_BYCOMMAND|MF_STRING, IDC_ITEM_OPEN, GetLangStringW(wLangModule, STRID_MENU_OPEN));
        EnableMenuItem(hMenuItems, IDC_ITEM_OPEN, nSelFilesCount?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_CLOSE, nSelFilesCount?MF_ENABLED:MF_GRAYED);
        if ((wpFile=(wchar_t *)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_FILEW, (LPARAM)NULL)) && *wpFile)
          EnableMenuItem(hMenuItems, IDC_ITEM_ADDCURFILE, MF_ENABLED);
        else
          EnableMenuItem(hMenuItems, IDC_ITEM_ADDCURFILE, MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_ADDFILES, MF_ENABLED);
        EnableMenuItem(hMenuItems, IDC_ITEM_ADDDIR, MF_ENABLED);
        EnableMenuItem(hMenuItems, IDC_ITEM_RENAME, nSelCount?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_MOVEUP, nSelCount?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_MOVEDOWN, nSelCount?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_DELETE, nSelCount?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuItems, IDC_ITEM_DELETEOLD, MF_ENABLED);
      }
      TrackPopupMenu(hMenuItems, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
    }
    else if ((HWND)wParam == hWndDockDlg)
    {
      POINT pt;
      RECT rc;

      GetCursorPos(&pt);
      GetWindowRect(hWndTitleText, &rc);

      if (PtInRect(&rc, pt))
        PostMessage(hDlg, WM_COMMAND, IDC_SESSION_ACTION, -1);
    }
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    POINT pt;
    RECT rc;

    GetCursorPos(&pt);
    GetWindowRect(hWndTitleText, &rc);

    if (PtInRect(&rc, pt))
      PostMessage(hDlg, AKDLL_SETUP, 0, 0);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_ITEMS_LIST)
    {
      if (((NMHDR *)lParam)->code == (UINT)NM_CUSTOMDRAW)
      {
        NMTVCUSTOMDRAW *lptvcd=(NMTVCUSTOMDRAW *)lParam;
        LRESULT lResult;

        if (lptvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
          lResult=CDRF_NOTIFYITEMDRAW;
        }
        else if (lptvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
          if (bDragging)
          {
            if (siDraggingCursor && siDraggingCursor == (SESSIONITEM *)lptvcd->nmcd.lItemlParam)
            {
              lptvcd->clrTextBk=AE_ColorBrightness(GetSysColor(COLOR_HIGHLIGHT), 80);
            }
          }
          lResult=CDRF_DODEFAULT;
        }
        else lResult=CDRF_DODEFAULT;

        SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, lResult);
        return TRUE;
      }
      else if (((NMTREEVIEWW *)lParam)->hdr.code == TVN_SELCHANGEDA ||
               ((NMTREEVIEWW *)lParam)->hdr.code == TVN_SELCHANGEDW)
      {
        if (!bDragging)
        {
          NMTREEVIEWW *pnmtv=(NMTREEVIEWW *)lParam;

          if (lpVirtualSession)
          {
            //Multi-selection
            SESSIONITEM *lpOldItem=(SESSIONITEM *)pnmtv->itemOld.lParam;
            SESSIONITEM *lpNewItem=(SESSIONITEM *)pnmtv->itemNew.lParam;
            SESSIONITEM *lpCount;

            if (lpOldItem && GetKeyState(VK_CONTROL) < 0)
            {
              if (lpOldItem->dwState & TVIS_SELECTED)
                SelectItem(hWndItemsList, lpOldItem, TRUE);
              if (lpNewItem)
              {
                if (lpNewItem->dwState & TVIS_SELECTED)
                  SelectItem(hWndItemsList, lpNewItem, FALSE);
                else
                  SelectItem(hWndItemsList, lpNewItem, TRUE);
              }
            }
            else if (lpOldItem && GetKeyState(VK_SHIFT) < 0)
            {
              SESSIONITEM *lpStartItem;
              SESSIONITEM *lpEndItem;
              SESSIONITEM *lpSwitchItem=NULL;
              SESSIONITEM *lpFirstSelItem;
              SESSIONITEM *lpLastSelItem;
              int nOldIndex;
              int nNewIndex;
              int nFirstSelIndex;
              int nLastSelIndex;
              BOOL bSelect;

              if ((nOldIndex=StackItemIndex(lpVirtualSession, lpOldItem)) &&
                  (nNewIndex=StackItemIndex(lpVirtualSession, lpNewItem)))
              {
                if (StackFindSelRange(lpVirtualSession, lpOldItem, &lpFirstSelItem, &nFirstSelIndex, &lpLastSelItem, &nLastSelIndex))
                {
                  if (nNewIndex < nOldIndex && nNewIndex < nFirstSelIndex)
                    lpStartItem=lpNewItem;
                  else if (nFirstSelIndex < nOldIndex)
                    lpStartItem=lpFirstSelItem;
                  else
                    lpStartItem=lpOldItem;

                  if (nNewIndex >= nOldIndex && nNewIndex >= nLastSelIndex)
                    lpEndItem=lpNewItem;
                  else if (nLastSelIndex >= nOldIndex)
                    lpEndItem=lpLastSelItem;
                  else
                    lpEndItem=lpOldItem;

                  if (lpFirstSelItem == lpLastSelItem ?
                      nNewIndex > nOldIndex :
                      hItemCaretPrev == lpLastSelItem->hItem)
                  {
                    if (nOldIndex < nNewIndex)
                      bSelect=TRUE;
                    else
                    {
                      if (nFirstSelIndex <= nNewIndex)
                        lpSwitchItem=lpNewItem;
                      else
                        lpSwitchItem=lpFirstSelItem;
                      bSelect=TRUE;
                    }
                  }
                  else
                  {
                    if (nOldIndex >= nNewIndex)
                      bSelect=TRUE;
                    else
                    {
                      if (nLastSelIndex >= nNewIndex)
                        lpSwitchItem=lpNewItem;
                      else
                        lpSwitchItem=lpLastSelItem;
                      bSelect=FALSE;
                    }
                  }

                  //Select range
                  for (lpCount=lpStartItem; lpCount; lpCount=StackNextItem(lpCount, NULL))
                  {
                    if (lpCount == lpSwitchItem && !bSelect)
                    {
                      lpSwitchItem=NULL;
                      bSelect=!bSelect;
                    }
                    SelectItem(hWndItemsList, lpCount, bSelect);
                    if (lpCount == lpSwitchItem && bSelect)
                    {
                      lpSwitchItem=NULL;
                      bSelect=!bSelect;
                    }
                    if (lpCount == lpEndItem)
                      break;
                  }
                }
              }
            }
            else
            {
              //Reset selection
              for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
              {
                if ((lpCount->dwState & TVIS_SELECTED) && lpCount != lpNewItem)
                  SelectItem(hWndItemsList, lpCount, FALSE);
              }
              if (lpNewItem)
                SelectItem(hWndItemsList, lpNewItem, TRUE);
            }

            //Update selection for Win7
            for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
            {
              if ((lpCount->dwState & TVIS_SELECTED) && lpCount != lpNewItem)
                SelectItem(hWndItemsList, lpCount, TRUE);
            }
            PostMessage(hDlg, AKDLL_GETCARETITEM, 0, 0);
          }
          bSelChanged=TRUE;
        }
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
      {
        if (TreeCursorItem(hWndItemsList))
          PostMessage(hDlg, WM_COMMAND, IDC_ITEM_OPEN, 0);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SESSION_ACTION)
    {
      POINT pt;
      RECT rcButton;

      if (lParam == -1)
      {
        GetCursorPos(&pt);
      }
      else
      {
        GetWindowRect(hWndSessionAction, &rcButton);
        pt.x=rcButton.left;
        pt.y=rcButton.top + (rcButton.bottom - rcButton.top);
      }

      if (!nCurrentSessionIndex)
      {
        ModifyMenuWide(hMenuLabel, IDC_SESSION_OPEN, MF_BYCOMMAND|MF_STRING|MF_ENABLED, IDC_SESSION_OPEN, GetLangStringW(wLangModule, STRID_UPDATE));
        if (SendMessage(hWndItemsList, TVM_GETCOUNT, 0, 0) > 0)
          EnableMenuItem(hMenuLabel, IDC_SESSION_CLOSE, MF_ENABLED);
        else
          EnableMenuItem(hMenuLabel, IDC_SESSION_CLOSE, MF_GRAYED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_EDIT, MF_GRAYED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_COPY, MF_GRAYED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_RENAME, MF_GRAYED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_DELETE, MF_GRAYED);
      }
      else
      {
        if (SendMessage(hWndItemsList, TVM_GETCOUNT, 0, 0) > 0)
        {
          ModifyMenuWide(hMenuLabel, IDC_SESSION_OPEN, MF_BYCOMMAND|MF_STRING|MF_ENABLED, IDC_SESSION_OPEN, GetLangStringW(wLangModule, STRID_OPEN));
          EnableMenuItem(hMenuLabel, IDC_SESSION_CLOSE, MF_ENABLED);
        }
        else
        {
          ModifyMenuWide(hMenuLabel, IDC_SESSION_OPEN, MF_BYCOMMAND|MF_STRING|MF_GRAYED, IDC_SESSION_OPEN, GetLangStringW(wLangModule, STRID_OPEN));
          EnableMenuItem(hMenuLabel, IDC_SESSION_CLOSE, MF_GRAYED);
        }
        EnableMenuItem(hMenuLabel, IDC_SESSION_EDIT, MF_ENABLED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_COPY, MF_ENABLED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_RENAME, MF_ENABLED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_DELETE, MF_ENABLED);
      }
      if (!lpVirtualSession || lpVirtualSession->bModified)
      {
        EnableMenuItem(hMenuLabel, IDC_SESSION_SAVE, MF_ENABLED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_COPY, MF_GRAYED);
      }
      else
      {
        EnableMenuItem(hMenuLabel, IDC_SESSION_SAVE, MF_GRAYED);
        EnableMenuItem(hMenuLabel, IDC_SESSION_COPY, MF_ENABLED);
      }
      TrackPopupMenu(hMenuLabel, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
    }
    else if (LOWORD(wParam) == IDC_SESSION_LIST)
    {
      if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        if (SaveSessionPrompt() == IDCANCEL)
        {
          SendMessage(hWndSessionList, CB_SETCURSEL, nCurrentSessionIndex, 0);
          return 0;
        }
        if (lpVirtualSession)
          StackFreeItems(lpVirtualSession);
        lpVirtualSession=NULL;
        lpRealSession=NULL;

        if (!(nCurrentSessionIndex=(int)SendMessage(hWndSessionList, CB_GETCURSEL, 0, 0)))
        {
          ClearTreeView(hWndItemsList, FALSE);
          TreeFillItemsCurrent(hWndItemsList);

          if (nDialogType != DLGT_DOCKABLE)
          {
            SetWindowTextWide(hWndOpenButton, GetLangStringW(wLangModule, STRID_UPDATE));
            EnableWindow(hWndOpenButton, TRUE);
            if (SendMessage(hWndItemsList, TVM_GETCOUNT, 0, 0) > 0)
              EnableWindow(hWndCloseButton, TRUE);
            else
              EnableWindow(hWndCloseButton, FALSE);
            EnableWindow(hWndEditButton, FALSE);
            EnableWindow(hWndCopyButton, FALSE);
            EnableWindow(hWndRenameButton, FALSE);
            EnableWindow(hWndDeleteButton, FALSE);
          }
        }
        else
        {
          ClearTreeView(hWndItemsList, FALSE);
          ComboBox_GetLBTextWide(hWndSessionList, nCurrentSessionIndex, wszBuffer);

          if (lpRealSession=GetSession(&hSessionStack, wszBuffer))
          {
            CopySession(lpRealSession, &ssCurrentTmp);
            lpVirtualSession=&ssCurrentTmp;
            TreeAddChild(hWndItemsList, lpVirtualSession, NULL);
          }

          if (nDialogType != DLGT_DOCKABLE)
          {
            SetWindowTextWide(hWndOpenButton, GetLangStringW(wLangModule, STRID_OPEN));
            if (SendMessage(hWndItemsList, TVM_GETCOUNT, 0, 0) > 0)
            {
              EnableWindow(hWndOpenButton, TRUE);
              EnableWindow(hWndCloseButton, TRUE);
            }
            else
            {
              EnableWindow(hWndOpenButton, FALSE);
              EnableWindow(hWndCloseButton, FALSE);
            }
            EnableWindow(hWndEditButton, TRUE);
            EnableWindow(hWndCopyButton, TRUE);
            EnableWindow(hWndRenameButton, TRUE);
            EnableWindow(hWndDeleteButton, TRUE);
          }
        }
        PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_SESSION_OPEN)
    {
      if (!lpVirtualSession)
        SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_SESSION_LIST, CBN_SELCHANGE), (LPARAM)hWndSessionList);
      else
        OpenSession(&lpVirtualSession->hItemsStack);
    }
    else if (LOWORD(wParam) == IDC_SESSION_CLOSE)
    {
      if (!lpVirtualSession)
      {
        SendMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSEALL, 0);
        SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_SESSION_LIST, CBN_SELCHANGE), (LPARAM)hWndSessionList);
      }
      else CloseSession(&lpVirtualSession->hItemsStack);
    }
    else if (LOWORD(wParam) == IDC_SESSION_SAVE ||
             LOWORD(wParam) == IDC_SESSION_COPY)
    {
      SESSION *lpNewElement;
      SESSION *lpOldElement=NULL;
      wchar_t wszNewSession[MAX_PATH];
      int nIndex;
      BOOL bSave=TRUE;

      if (lpVirtualSession && LOWORD(wParam) == IDC_SESSION_SAVE)
      {
        StackFreeItems(lpRealSession);
        lpVirtualSession->bModified=FALSE;
        CopySession(lpVirtualSession, lpRealSession);
        SaveSessionFile(lpRealSession);
        PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
        return 0;
      }

      if (!DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_INPUTBOX), hDlg, (DLGPROC)InputBoxDlgProc, LOWORD(wParam)))
        return 0;
      xstrcpynW(wszNewSession, wszDlgInputBox, MAX_PATH);

      if (lpOldElement=GetSession(&hSessionStack, wszNewSession))
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ALREADY_EXIST), wszNewSession);
        if (MessageBoxW(hDlg, wszBuffer, wszPluginTitle, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == IDNO)
          bSave=FALSE;
      }

      if (bSave)
      {
        if (!lpVirtualSession)
        {
          lpNewElement=AddCurrentSession(&hSessionStack, wszNewSession);
        }
        else
        {
          if (lpNewElement=AddEmptySession(&hSessionStack, wszNewSession))
          {
            xstrcpynW(lpVirtualSession->wszSessionName, wszNewSession, MAX_PATH);
            lpVirtualSession->bModified=FALSE;
            CopySession(lpVirtualSession, lpNewElement);
          }
        }
        SaveSessionFile(lpNewElement);

        if (!lpOldElement)
        {
          if ((nIndex=ComboBox_AddStringWide(hWndSessionList, wszNewSession)) != CB_ERR)
          {
            if (!nIndex)
              MoveComboBoxItem(hWndSessionList, nIndex, ++nIndex);
          }
        }
        else
        {
          DeleteSession(&hSessionStack, lpOldElement);
          nIndex=ComboBox_FindStringExactWide(hWndSessionList, -1, wszNewSession);
        }
        SetFocus(hWndSessionList);
        SendMessage(hWndSessionList, CB_SETCURSEL, nIndex, 0);
        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_SESSION_LIST, CBN_SELCHANGE), (LPARAM)hWndSessionList);
      }
    }
    else if (LOWORD(wParam) == IDC_SESSION_EDIT)
    {
      if (lpVirtualSession)
      {
        if (EditSessionFile(lpVirtualSession->wszSessionName))
        {
          if (nDialogType != DLGT_DOCKABLE)
            PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
          return TRUE;
        }
      }
    }
    else if (LOWORD(wParam) == IDC_SESSION_RENAME)
    {
      INT_PTR nResult;

      nResult=DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_INPUTBOX), hDlg, (DLGPROC)InputBoxDlgProc, IDC_SESSION_RENAME);

      if (nResult)
      {
        if (lpRealSession && lpVirtualSession)
        {
          if (RenameSessionFile(lpRealSession->wszSessionName, wszDlgInputBox))
          {
            xstrcpynW(lpVirtualSession->wszSessionName, wszDlgInputBox, MAX_PATH);
            xstrcpynW(lpRealSession->wszSessionName, wszDlgInputBox, MAX_PATH);

            SendMessage(hWndSessionList, CB_DELETESTRING, nCurrentSessionIndex, 0);
            ComboBox_InsertStringWide(hWndSessionList, nCurrentSessionIndex, lpVirtualSession->wszSessionName);
            SetFocus(hWndSessionList);
            SendMessage(hWndSessionList, CB_SETCURSEL, nCurrentSessionIndex, 0);
          }
          else MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_RENAME_ERROR), wszPluginTitle, MB_OK|MB_ICONERROR);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_SESSION_DELETE)
    {
      if (lpVirtualSession)
      {
        if (MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_CONFIRM_DELETE), wszPluginTitle, MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
          if (DeleteSessionFile(lpVirtualSession->wszSessionName))
          {
            //Delete session from stack
            DeleteSession(&hSessionStack, lpRealSession);
            SendMessage(hWndSessionList, CB_DELETESTRING, nCurrentSessionIndex, 0);
            lpRealSession=NULL;

            //Delete temp session
            StackFreeItems(lpVirtualSession);
            lpVirtualSession=NULL;

            SetFocus(hWndSessionList);
            SendMessage(hWndSessionList, CB_SETCURSEL, 0, 0);
            PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_SESSION_LIST, CBN_SELCHANGE), (LPARAM)hWndSessionList);
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_OPEN)
    {
      if (!lpVirtualSession)
      {
        FRAMEDATA *lpFrame;

        if (lpFrame=(FRAMEDATA *)TreeItemParam(hWndItemsList, NULL))
          SendMessage(hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrame);
      }
      else
      {
        SESSIONITEM *lpCount;

        for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
        {
          if ((lpCount->dwState & TVIS_SELECTED) && !lpCount->dwFolderFlags)
            OpenItem(lpCount);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_CLOSE)
    {
      if (!lpVirtualSession)
      {
        TVITEMW tvi;

        tvi.mask=TVIF_PARAM;
        tvi.hItem=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
        if (TreeView_GetItemWide(hWndItemsList, &tvi))
        {
          if (CloseItem((FRAMEDATA *)tvi.lParam, TRUE))
            TreeDeleteItem(hWndItemsList, tvi.hItem);
        }
      }
      else
      {
        SESSIONITEM *lpCount;
        FRAMEDATA *lpFrame;

        for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
        {
          if (lpCount->dwState & TVIS_SELECTED)
          {
            if (lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)lpCount->wszItemExpFile))
            {
              if (!CloseItem(lpFrame, TRUE))
                break;
            }
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_ADDCURFILE ||
             LOWORD(wParam) == IDC_ITEM_ADDFILES)
    {
      if (lpVirtualSession)
      {
        HTREEITEM hItemCaret;
        BOOL bAddFiles=TRUE;

        //Get caret here. Because, if no caret in list and dialog close, caret will return.
        hItemCaret=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
        wszBuffer[0]=L'\0';
        wszBuffer[1]=L'\0';

        if (LOWORD(wParam) == IDC_ITEM_ADDCURFILE)
        {
          EDITINFO ei;

          if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
            xstrcpynW(wszBuffer, ei.wszFile, MAX_PATH);
          else
            bAddFiles=FALSE;
        }
        else if (LOWORD(wParam) == IDC_ITEM_ADDFILES)
        {
          OPENFILENAMEW ofn;

          xmemset(&ofn, 0, sizeof(OPENFILENAMEW));
          ofn.lStructSize  =sizeof(OPENFILENAMEW);
          ofn.hwndOwner    =hDlg;
          ofn.hInstance    =hInstanceDLL;
          ofn.lpstrFile    =wszBuffer;
          ofn.lpstrFilter  =GetLangStringW(wLangModule, STRID_FILTER);
          ofn.nFilterIndex =2;
          ofn.nMaxFile     =BUFFER_SIZE;
          ofn.Flags        =OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ENABLESIZING;
          bAddFiles=GetOpenFileNameWide(&ofn);
        }

        if (bAddFiles && *wszBuffer)
        {
          wchar_t wszFile[MAX_PATH];
          wchar_t *wpFile=wszBuffer + xstrlenW(wszBuffer) + 1;
          HTREEITEM hItemAfterThis=TVI_LAST;
          SESSIONITEM *siNew=NULL;
          SESSIONITEM *siParent=NULL;
          SESSIONITEM *siAfterThis;
          BOOL bFirstFile=TRUE;

          if (hItemCaret && (siAfterThis=(SESSIONITEM *)TreeItemParam(hWndItemsList, hItemCaret)))
          {
            if (!siAfterThis->dwFolderFlags)
            {
              siParent=siAfterThis->parent;
              hItemAfterThis=siAfterThis->hItem;
            }
            else
            {
              siParent=siAfterThis;
              siAfterThis=ASI_FIRST;
              hItemAfterThis=TVI_FIRST;
            }
          }
          else siAfterThis=ASI_LAST;

          if (*wpFile)
          {
            //Multiple files
            if (*(wpFile - 2) == L'\\') *(wpFile - 2)=L'\0';

            do
            {
              if (IsPathFull(wpFile))
                xstrcpyW(wszFile, wpFile);  //.lnk target
              else
                xprintfW(wszFile, L"%s\\%s", wszBuffer, wpFile);

              if (siNew=StackAddItem(lpVirtualSession, siParent, siAfterThis))
              {
                xstrcpynW(siNew->wszName, GetFileName(wszFile, -1), MAX_PATH);
                xstrcpynW(siNew->wszItemFile, wszFile, MAX_PATH);
                xstrcpynW(siNew->wszItemExpFile, wszFile, MAX_PATH);
                siNew->dwState=TVIS_SELECTED;
                TreeAddItem(hWndItemsList, siNew, hItemAfterThis, siNew->dwState);
                siAfterThis=siNew;
                hItemAfterThis=siNew->hItem;
                lpVirtualSession->bModified=TRUE;
                if (bFirstFile)
                {
                  //Reset selection
                  SendMessage(hWndItemsList, TVM_SELECTITEM, TVGN_CARET, (LPARAM)siNew->hItem);
                  bFirstFile=FALSE;
                }
              }
            }
            while (*(wpFile+=xstrlenW(wpFile) + 1));
          }
          else
          {
            //Single file
            if (siNew=StackAddItem(lpVirtualSession, siParent, siAfterThis))
            {
              xstrcpynW(siNew->wszName, GetFileName(wszBuffer, -1), MAX_PATH);
              xstrcpynW(siNew->wszItemFile, wszBuffer, MAX_PATH);
              xstrcpynW(siNew->wszItemExpFile, wszBuffer, MAX_PATH);
              siNew->dwState=TVIS_SELECTED;
              TreeAddItem(hWndItemsList, siNew, hItemAfterThis, siNew->dwState);
              lpVirtualSession->bModified=TRUE;
              SendMessage(hWndItemsList, TVM_SELECTITEM, TVGN_CARET, (LPARAM)siNew->hItem);
            }
          }
          PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_ADDDIR)
    {
      if (lpVirtualSession)
      {
        INT_PTR nResult;
        HTREEITEM hItemCaret;

        //Get caret here. Because, if no caret in list and dialog close, caret will return.
        hItemCaret=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);

        nResult=DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_INPUTBOX), hDlg, (DLGPROC)InputBoxDlgProc, IDC_ITEM_ADDDIR);

        if (nResult && wszDlgInputBox[0])
        {
          HTREEITEM hItemAfterThis=TVI_LAST;
          SESSIONITEM *siNew=NULL;
          SESSIONITEM *siParent=NULL;
          SESSIONITEM *siAfterThis;

          if (hItemCaret && (siAfterThis=(SESSIONITEM *)TreeItemParam(hWndItemsList, hItemCaret)))
          {
            if (!siAfterThis->dwFolderFlags)
            {
              siParent=siAfterThis->parent;
              hItemAfterThis=siAfterThis->hItem;
            }
            else
            {
              siParent=siAfterThis;
              siAfterThis=ASI_FIRST;
              hItemAfterThis=TVI_FIRST;
            }
          }
          else siAfterThis=ASI_LAST;

          if (siNew=StackAddItem(lpVirtualSession, siParent, siAfterThis))
          {
            xstrcpynW(siNew->wszName, wszDlgInputBox, MAX_PATH);
            siNew->wszItemFile[0]=L'\0';
            siNew->wszItemExpFile[0]=L'\0';
            siNew->dwState=0;
            siNew->dwFolderFlags=FLDF_CLOSE;
            TreeAddItem(hWndItemsList, siNew, hItemAfterThis, siNew->dwState);
            lpVirtualSession->bModified=TRUE;
            SendMessage(hWndItemsList, TVM_SELECTITEM, TVGN_CARET, (LPARAM)siNew->hItem);
          }
        }
        PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_RENAME)
    {
      if (lpVirtualSession)
      {
        TVITEMW tvi;
        SESSIONITEM *siCaret=NULL;
        HTREEITEM hItemCaret=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
        INT_PTR nResult;

        if (hItemCaret && (siCaret=(SESSIONITEM *)TreeItemParam(hWndItemsList, hItemCaret)))
        {
          nResult=DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_ITEMEDIT), hDlg, (DLGPROC)ItemEditDlgProc, (LPARAM)siCaret);

          if (nResult)
          {
            if (!siCaret->dwFolderFlags)
              TranslateFileString(siCaret->wszItemFile, siCaret->wszItemExpFile, MAX_PATH);
            tvi.mask=TVIF_TEXT;
            tvi.pszText=(wchar_t *)siCaret->wszName;
            tvi.cchTextMax=MAX_PATH;
            tvi.hItem=siCaret->hItem;
            TreeView_SetItemWide(hWndItemsList, &tvi);

            lpVirtualSession->bModified=TRUE;
          }
          PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_MOVEUP)
    {
      if (lpVirtualSession)
      {
        SESSIONITEM *lpCount=lpVirtualSession->hItemsStack.first;
        HTREEITEM hItemCaret=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);

        while (lpCount)
        {
          if ((lpCount->dwState & TVIS_SELECTED) && lpCount->prev && !(lpCount->prev->dwState & TVIS_SELECTED))
          {
            TreeMoveItem(hWndItemsList, hItemCaret, lpVirtualSession, lpCount, lpCount->parent, lpCount->prev->prev);
            lpCount=StackNextItemNoChild(lpCount->next);
            lpVirtualSession->bModified=TRUE;
            continue;
          }
          lpCount=StackNextItem(lpCount, NULL);
        }
        PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_MOVEDOWN)
    {
      if (lpVirtualSession)
      {
        SESSIONITEM *lpCount=lpVirtualSession->hItemsStack.last;
        HTREEITEM hItemCaret=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);

        while (lpCount)
        {
          if ((lpCount->dwState & TVIS_SELECTED) && lpCount->next && !(lpCount->next->dwState & TVIS_SELECTED))
          {
            TreeMoveItem(hWndItemsList, hItemCaret, lpVirtualSession, lpCount, lpCount->parent, lpCount->next);
            lpCount=StackPrevItemNoChild(lpCount->prev);
            lpVirtualSession->bModified=TRUE;
            continue;
          }
          lpCount=StackPrevItem(lpCount, NULL);
        }
        PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_DELETE ||
             LOWORD(wParam) == IDC_ITEM_DELETEOLD)
    {
      if (lpVirtualSession)
      {
        SESSIONITEM *lpCount;
        SESSIONITEM *lpPrevCount=NULL;
        SESSIONITEM *lpNewCaret=NULL;
        SESSIONITEM *lpParentCaret=NULL;
        HTREEITEM hItemCaret=(HTREEITEM)SendMessage(hWndItemsList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);

        //Mark for deletion
        if (LOWORD(wParam) == IDC_ITEM_DELETE)
        {
          for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
          {
            if (lpCount->dwState & TVIS_SELECTED)
              lpCount->bDeleting=TRUE;
          }
        }
        else if (LOWORD(wParam) == IDC_ITEM_DELETEOLD)
        {
          for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
          {
            if (!lpCount->dwFolderFlags && !FileExistsWide(lpCount->wszItemExpFile))
              lpCount->bDeleting=TRUE;
          }
        }

        //Deletion
        for (lpCount=lpVirtualSession->hItemsStack.first; lpCount;)
        {
          if (lpCount->bDeleting)
          {
            if (!lpNewCaret && hItemCaret == lpCount->hItem)
            {
              //Change caret
              for (lpNewCaret=lpCount; lpNewCaret;)
              {
                if (!lpNewCaret->bDeleting)
                {
                  for (lpParentCaret=lpNewCaret; lpParentCaret; lpParentCaret=lpParentCaret->parent)
                  {
                    //Check that root items not marked for deletion
                    if (lpParentCaret->bDeleting)
                    {
                      lpNewCaret=lpParentCaret;
                      goto NextCaret;
                    }
                  }
                  SendMessage(hWndItemsList, TVM_SELECTITEM, TVGN_CARET, (LPARAM)lpNewCaret->hItem);
                  break;
                }
                NextCaret:
                if (lpNewCaret->next)
                  lpNewCaret=lpNewCaret->next;
                else
                  lpNewCaret=lpNewCaret->parent;
              }
            }
            TreeDeleteItem(hWndItemsList, lpCount->hItem);
            StackDeleteItem(lpVirtualSession, lpCount);
            if (!lpPrevCount)
              lpCount=lpVirtualSession->hItemsStack.first;
            else
              lpCount=StackNextItem(lpPrevCount, NULL);
            lpVirtualSession->bModified=TRUE;
            continue;
          }
          lpPrevCount=lpCount;
          lpCount=StackNextItem(lpCount, NULL);
        }
        PostMessage(hDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_SETTINGS)
    {
      PostMessage(hDlg, AKDLL_SETUP, 0, 0);
    }
    else if (LOWORD(wParam) == IDOK ||
             LOWORD(wParam) == IDCANCEL ||
             LOWORD(wParam) == IDC_CLOSE)
    {
      int nCurDialogType=nDialogType;

      if (dwSaveFlags)
      {
        if (dkSessionsDlg)
        {
          nSessionsDockSide=dkSessionsDlg->nSide;
          rcSessionsDockRect=dkSessionsDlg->rcSize;
        }
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }
      if (!(lParam & DKT_ONMAINFINISH))
      {
        if (SaveSessionPrompt() == IDCANCEL)
          return 0;
      }

      //Change dialog type
      if ((nCurDialogType == DLGT_MODAL || nCurDialogType == DLGT_MODELESS) &&
          (nNewDialogType == DLGT_MODAL || nNewDialogType == DLGT_MODELESS))
      {
        nDialogType=nNewDialogType;
        nNewDialogType=0;
        xmemset(&rcMainCurrentDialog, 0, sizeof(RECT));
      }
      if (lpVirtualSession)
        StackFreeItems(lpVirtualSession);
      lpVirtualSession=NULL;
      lpRealSession=NULL;
      FreeSessions(&hSessionStack);

      //Destroy resources
      DestroyMenu(hMenuItems);

      if (nCurDialogType == DLGT_DOCKABLE)
      {
        SendMessage(hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkSessionsDlg);
        dkSessionsDlg=NULL;

        DestroyMenu(hMenuLabel);
        DestroyWindow(hWndDockDlg);
        hWndDockDlg=NULL;
        hWndMainDlg=NULL;

        if (!(lParam & DKT_ONMAINFINISH))
        {
          SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
          SetFocus(hMainWnd);
        }
      }
      else
      {
        //Close dialog
        if (nCurDialogType == DLGT_MODAL)
          EndDialog(hWndMainDlg, 0);
        else
        {
          SendMessage(hMainWnd, AKD_SETMODELESS, (WPARAM)hDlg, MLA_DELETE);
          DestroyWindow(hWndMainDlg);
        }
        hWndMainDlg=NULL;
      }
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, (nDialogType == DLGT_DOCKABLE?0:RDM_PAINTSIZEGRIP), hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
    {
      if (dkSessionsDlg)
        GetWindowSize(hWndTitleText, hDlg, &dkSessionsDlg->rcDragDrop);
      else
        dwSaveFlags|=OF_RECT;
    }
  }

  return FALSE;
}

BOOL CALLBACK InputBoxDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndInputBox;
  static HWND hWndInputBoxEdit;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndInputBox=GetDlgItem(hDlg, IDC_INPUTBOX_EDIT);
    hWndInputBoxEdit=GetDlgItem(hWndInputBox, IDC_COMBOBOXEDIT);

    if (lParam == IDC_SESSION_SAVE)
      SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_SAVE));
    else if (lParam == IDC_SESSION_COPY)
      SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_COPY));
    else if (lParam == IDC_SESSION_RENAME)
      SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_RENAME));
    else if (lParam == IDC_ITEM_ADDDIR)
      SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_ADDDIR));
    SetDlgItemTextWide(hDlg, IDC_INPUTBOX_LABEL, GetLangStringW(wLangModule, STRID_NEWNAME));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndInputBoxEdit, EM_LIMITTEXT, MAX_PATH, 0);
    if (lParam != IDC_ITEM_ADDDIR)
      ComboFillSessions(&hSessionStack, hWndInputBox);
    if (lpVirtualSession)
    {
      if (lParam == IDC_SESSION_COPY)
      {
        wchar_t wszSessionName[MAX_PATH];
        int nNumber=2;
        int i;

        xstrcpynW(wszSessionName, lpVirtualSession->wszSessionName, MAX_PATH);
        i=(int)xstrlenW(wszSessionName) - 1;

        if (wszSessionName[i] == L')')
        {
          while (--i >= 0 && wszSessionName[i] >= L'0' && wszSessionName[i] <= L'9');
          if (wszSessionName[i] == L'(')
          {
            nNumber=(int)xatoiW(wszSessionName + i + 1, NULL) + 1;
            wszSessionName[i]=L'\0';
          }
        }
        for (i=nNumber; ; ++i)
        {
          xprintfW(wszBuffer, L"%s(%d)", wszSessionName, i);
          if (!GetSession(&hSessionStack, wszBuffer))
          {
            SetWindowTextWide(hWndInputBox, wszBuffer);
            break;
          }
        }
      }
      else if (lParam == IDC_ITEM_ADDDIR)
      {
      }
      else SetWindowTextWide(hWndInputBox, lpVirtualSession->wszSessionName);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_INPUTBOX_EDIT)
    {
      if (HIWORD(wParam) == CBN_DROPDOWN)
      {
        UpdateComboBoxDropWidth(hWndInputBox);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      GetWindowTextWide(hWndInputBox, wszBuffer, MAX_PATH);
      xstrcpynW(wszDlgInputBox, wszBuffer, MAX_PATH);

      EndDialog(hDlg, 1);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }
  return FALSE;
}

BOOL CALLBACK ItemEditDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndName;
  static HWND hWndFileLabel;
  static HWND hWndFile;
  static HWND hWndOKButton;
  static SESSIONITEM *si;

  if (uMsg == WM_INITDIALOG)
  {
    si=(SESSIONITEM *)lParam;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);
    hWndName=GetDlgItem(hDlg, IDC_ITEMNAME);
    hWndFileLabel=GetDlgItem(hDlg, IDC_ITEMFILE_LABEL);
    hWndFile=GetDlgItem(hDlg, IDC_ITEMFILE);
    hWndOKButton=GetDlgItem(hDlg, IDOK);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_RENAME));
    SetDlgItemTextWide(hDlg, IDC_ITEMNAME_LABEL, GetLangStringW(wLangModule, STRID_ITEMNAME));
    SetDlgItemTextWide(hDlg, IDC_ITEMFILE_LABEL, GetLangStringW(wLangModule, STRID_ITEMFILE));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));
    SetWindowTextWide(hWndName, si->wszName);
    if (!si->dwFolderFlags)
    {
      SetWindowTextWide(hWndFile, si->wszItemFile);
      if (bShowPath)
      {
        SetFocus(hWndFile);
        SendMessage(hWndFile, EM_SETSEL, 0, (LPARAM)-1);
      }
    }
    else
    {
      EnableWindow(hWndFileLabel, FALSE);
      EnableWindow(hWndFile, FALSE);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_ITEMNAME)
    {
      if (GetWindowTextLengthWide(hWndName))
        EnableWindow(hWndOKButton, TRUE);
      else
        EnableWindow(hWndOKButton, FALSE);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      GetWindowTextWide(hWndName, si->wszName, MAX_PATH);
      if (!si->dwFolderFlags)
        GetWindowTextWide(hWndFile, si->wszItemFile, MAX_PATH);

      EndDialog(hDlg, 1);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndProgramDir;
  static HWND hWndAppDataDir;
  static HWND hWndOpenOnStart;
  static HWND hWndOpenName;
  static HWND hWndSaveOnExit;
  static HWND hWndSaveName;
  static HWND hWndShowPath;
  static HWND hWndSaveRelative;
  static HWND hWndDlgSystemFont;
  static HWND hWndDlgCoderTheme;
  static HWND hWndDlgTypeModal;
  static HWND hWndDlgTypeModeless;
  static HWND hWndDlgTypeDockable;
  static HWND hWndDlgDockAutoload;
  static HWND hWndSaveActive;
  static HWND hWndSaveCodepage;
  static HWND hWndSaveSelection;
  static HWND hWndSaveWordWrap;
  static HWND hWndSaveReadOnly;
  static HWND hWndSaveOvertype;
  static HWND hWndSaveBookmarks;
  static HWND hWndSaveAlias;
  static HWND hWndSaveFolds;
  static HWND hWndSaveMarks;
  int nState;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndProgramDir=GetDlgItem(hDlg, IDC_SETTINGS_SAVESESSIONS_PROGRAMDIR);
    hWndAppDataDir=GetDlgItem(hDlg, IDC_SETTINGS_SAVESESSIONS_APPDATADIR);
    hWndOpenOnStart=GetDlgItem(hDlg, IDC_SETTINGS_OPENONSTART);
    hWndOpenName=GetDlgItem(hDlg, IDC_SETTINGS_OPENNAME);
    hWndSaveOnExit=GetDlgItem(hDlg, IDC_SETTINGS_SAVEONEXIT);
    hWndSaveName=GetDlgItem(hDlg, IDC_SETTINGS_SAVENAME);
    hWndShowPath=GetDlgItem(hDlg, IDC_SETTINGS_SHOWPATH);
    hWndSaveRelative=GetDlgItem(hDlg, IDC_SETTINGS_SAVERELATIVE);
    hWndDlgSystemFont=GetDlgItem(hDlg, IDC_SETTINGS_SYSTEMFONT);
    hWndDlgCoderTheme=GetDlgItem(hDlg, IDC_SETTINGS_CODERTHEME);
    hWndDlgTypeModal=GetDlgItem(hDlg, IDC_SETTINGS_DLGTYPE_MODAL);
    hWndDlgTypeModeless=GetDlgItem(hDlg, IDC_SETTINGS_DLGTYPE_MODELESS);
    hWndDlgTypeDockable=GetDlgItem(hDlg, IDC_SETTINGS_DLGTYPE_DOCKABLE);
    hWndDlgDockAutoload=GetDlgItem(hDlg, IDC_SETTINGS_DOCKAUTOLOAD);
    hWndSaveActive=GetDlgItem(hDlg, IDC_SETTINGS_SAVEACTIVE);
    hWndSaveCodepage=GetDlgItem(hDlg, IDC_SETTINGS_SAVECODEPAGE);
    hWndSaveSelection=GetDlgItem(hDlg, IDC_SETTINGS_SAVESELECTION);
    hWndSaveWordWrap=GetDlgItem(hDlg, IDC_SETTINGS_SAVEWORDWRAP);
    hWndSaveReadOnly=GetDlgItem(hDlg, IDC_SETTINGS_SAVEREADONLY);
    hWndSaveOvertype=GetDlgItem(hDlg, IDC_SETTINGS_SAVEOVERTYPE);
    hWndSaveBookmarks=GetDlgItem(hDlg, IDC_SETTINGS_SAVEBOOKMARKS);
    hWndSaveAlias=GetDlgItem(hDlg, IDC_SETTINGS_SAVEALIAS);
    hWndSaveFolds=GetDlgItem(hDlg, IDC_SETTINGS_SAVEFOLDS);
    hWndSaveMarks=GetDlgItem(hDlg, IDC_SETTINGS_SAVEMARKS);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_SETTINGS));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVESESSIONS_GROUP, GetLangStringW(wLangModule, STRID_SAVESESSIONS));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVESESSIONS_PROGRAMDIR, GetLangStringW(wLangModule, STRID_PROGRAMDIR));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVESESSIONS_APPDATADIR, GetLangStringW(wLangModule, STRID_APPDATA));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SESSION_GROUP, GetLangStringW(wLangModule, STRID_SESSION));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_OPENONSTART, GetLangStringW(wLangModule, STRID_OPENONSTART));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEONEXIT, GetLangStringW(wLangModule, STRID_SAVEONEXIT));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SHOWPATH, GetLangStringW(wLangModule, STRID_SHOWPATH));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVERELATIVE, GetLangStringW(wLangModule, STRID_SAVERELATIVE));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SYSTEMFONT, GetLangStringW(wLangModule, STRID_SYSTEMFONT));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_CODERTHEME, GetLangStringW(wLangModule, STRID_CODERTHEME));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_DLGTYPE_GROUP, GetLangStringW(wLangModule, STRID_DIALOGTYPE));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_DLGTYPE_MODAL, GetLangStringW(wLangModule, STRID_MODALDIALOG));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_DLGTYPE_MODELESS, GetLangStringW(wLangModule, STRID_MODELESSDIALOG));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_DLGTYPE_DOCKABLE, GetLangStringW(wLangModule, STRID_DOCKABLEDIALOG));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_DOCKAUTOLOAD, GetLangStringW(wLangModule, STRID_AUTOLOAD));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEDATA_GROUP, GetLangStringW(wLangModule, STRID_SAVEDATA));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEACTIVE, GetLangStringW(wLangModule, STRID_SAVEACTIVE));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVECODEPAGE, GetLangStringW(wLangModule, STRID_SAVECODEPAGE));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVESELECTION, GetLangStringW(wLangModule, STRID_SAVESELECTION));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEWORDWRAP, GetLangStringW(wLangModule, STRID_SAVEWORDWRAP));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEREADONLY, GetLangStringW(wLangModule, STRID_SAVEREADONLY));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEOVERTYPE, GetLangStringW(wLangModule, STRID_SAVEOVERTYPE));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEBOOKMARKS, GetLangStringW(wLangModule, STRID_SAVEBOOKMARKS));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEALIAS, GetLangStringW(wLangModule, STRID_SAVEALIAS));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEFOLDS, GetLangStringW(wLangModule, STRID_SAVEFOLDS));
    SetDlgItemTextWide(hDlg, IDC_SETTINGS_SAVEMARKS, GetLangStringW(wLangModule, STRID_SAVEMARKS));

    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndOpenName, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndSaveName, EM_LIMITTEXT, MAX_PATH, 0);
    SetWindowTextWide(hWndOpenName, wszOpenOnStart);
    SetWindowTextWide(hWndSaveName, wszSaveOnExit);

    if (nSaveSessions == SDIR_PROGRAM)
      SendMessage(hWndProgramDir, BM_SETCHECK, BST_CHECKED, 0);
    else if (nSaveSessions == SDIR_APPDATA)
      SendMessage(hWndAppDataDir, BM_SETCHECK, BST_CHECKED, 0);
    if (bOpenOnStart) SendMessage(hWndOpenOnStart, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveOnExit) SendMessage(hWndSaveOnExit, BM_SETCHECK, BST_CHECKED, 0);
    if (bShowPath) SendMessage(hWndShowPath, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveRelative) SendMessage(hWndSaveRelative, BM_SETCHECK, BST_CHECKED, 0);
    if (bListSystemFont) SendMessage(hWndDlgSystemFont, BM_SETCHECK, BST_CHECKED, 0);
    if (bCoderTheme) SendMessage(hWndDlgCoderTheme, BM_SETCHECK, BST_CHECKED, 0);

    if ((nNewDialogType? nNewDialogType : nDialogType) == DLGT_MODAL)
    {
      SendMessage(hWndDlgTypeModal, BM_SETCHECK, BST_CHECKED, 0);
      EnableWindow(hWndDlgDockAutoload, FALSE);
    }
    else if ((nNewDialogType? nNewDialogType : nDialogType) == DLGT_MODELESS)
    {
      SendMessage(hWndDlgTypeModeless, BM_SETCHECK, BST_CHECKED, 0);
      EnableWindow(hWndDlgDockAutoload, FALSE);
    }
    else if ((nNewDialogType? nNewDialogType : nDialogType) == DLGT_DOCKABLE)
    {
      SendMessage(hWndDlgTypeDockable, BM_SETCHECK, BST_CHECKED, 0);
      EnableWindow(hWndDlgDockAutoload, TRUE);
    }
    if (bDockAutoload) SendMessage(hWndDlgDockAutoload, BM_SETCHECK, BST_CHECKED, 0);

    if (dwSaveData & SSD_ACTIVE) SendMessage(hWndSaveActive, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_CODEPAGE) SendMessage(hWndSaveCodepage, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_SELECTION) SendMessage(hWndSaveSelection, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_WORDWRAP) SendMessage(hWndSaveWordWrap, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_READONLY) SendMessage(hWndSaveReadOnly, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_OVERTYPE) SendMessage(hWndSaveOvertype, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_BOOKMARKS) SendMessage(hWndSaveBookmarks, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_CODERALIAS) SendMessage(hWndSaveAlias, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_CODERFOLDS) SendMessage(hWndSaveFolds, BM_SETCHECK, BST_CHECKED, 0);
    if (dwSaveData & SSD_CODERMARKS) SendMessage(hWndSaveMarks, BM_SETCHECK, BST_CHECKED, 0);

    PostMessage(hDlg, WM_COMMAND, IDC_SETTINGS_OPENONSTART, 0);
    PostMessage(hDlg, WM_COMMAND, IDC_SETTINGS_SAVEONEXIT, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SETTINGS_DLGTYPE_MODAL ||
        LOWORD(wParam) == IDC_SETTINGS_DLGTYPE_MODELESS ||
        LOWORD(wParam) == IDC_SETTINGS_DLGTYPE_DOCKABLE)
    {
      EnableWindow(hWndDlgDockAutoload, (LOWORD(wParam) == IDC_SETTINGS_DLGTYPE_DOCKABLE));
    }
    else if (LOWORD(wParam) == IDC_SETTINGS_OPENONSTART)
    {
      nState=(int)SendMessage(hWndOpenOnStart, BM_GETCHECK, 0, 0);
      EnableWindow(hWndOpenName, nState);
    }
    else if (LOWORD(wParam) == IDC_SETTINGS_SAVEONEXIT)
    {
      nState=(int)SendMessage(hWndSaveOnExit, BM_GETCHECK, 0, 0);
      EnableWindow(hWndSaveName, nState);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      BOOL bRestart=FALSE;

      //Save place
      nState=0;
      if (SendMessage(hWndProgramDir, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nState=SDIR_PROGRAM;
      else if (SendMessage(hWndAppDataDir, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nState=SDIR_APPDATA;
      if (nState != nSaveSessions)
      {
        nSaveSessions=nState;
        bRestart=TRUE;
      }

      nState=(BOOL)SendMessage(hWndShowPath, BM_GETCHECK, 0, 0);
      if (bShowPath != nState)
      {
        bShowPath=nState;
        PostMessage(hWndMainDlg, WM_COMMAND, MAKELONG(IDC_SESSION_LIST, CBN_SELCHANGE), (LPARAM)0);
      }
      bSaveRelative=(BOOL)SendMessage(hWndSaveRelative, BM_GETCHECK, 0, 0);

      nState=(BOOL)SendMessage(hWndDlgSystemFont, BM_GETCHECK, 0, 0);
      if (nState != bListSystemFont)
      {
        bListSystemFont=nState;
        if (!bListSystemFont)
        {
          HFONT hFontEdit;

          hFontEdit=(HFONT)SendMessage(hMainWnd, AKD_GETFONT, (WPARAM)NULL, (LPARAM)NULL);
          SendMessage(hWndItemsList, WM_SETFONT, (WPARAM)hFontEdit, FALSE);
        }
        else SendMessage(hWndItemsList, WM_SETFONT, (WPARAM)NULL, FALSE);
      }
      nState=(BOOL)SendMessage(hWndDlgCoderTheme, BM_GETCHECK, 0, 0);
      if (nState != bCoderTheme)
      {
        bCoderTheme=nState;
        if (!bCoderTheme)
        {
          SendMessage(hWndItemsList, TVM_SETTEXTCOLOR, 0, (LPARAM)(DWORD)-1);
          SendMessage(hWndItemsList, TVM_SETBKCOLOR, 0, (LPARAM)(DWORD)-1);
        }
        else SetListColors(NULL, hWndItemsList);
      }

      //Dialog type
      nState=0;
      if (SendMessage(hWndDlgTypeModal, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nState=DLGT_MODAL;
      else if (SendMessage(hWndDlgTypeModeless, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nState=DLGT_MODELESS;
      else if (SendMessage(hWndDlgTypeDockable, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nState=DLGT_DOCKABLE;
      if (nState != nDialogType)
      {
        if (nState == DLGT_DOCKABLE || nDialogType == DLGT_DOCKABLE)
        {
          bRestart=TRUE;
          nNewDialogType=nState;
        }
        else if (!hWndDockDlg)
        {
          nNewDialogType=nState;
        }
      }
      bDockAutoload=(BOOL)SendMessage(hWndDlgDockAutoload, BM_GETCHECK, 0, 0);

      //Save data
      dwSaveData=0;
      if (SendMessage(hWndSaveActive, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_ACTIVE;
      if (SendMessage(hWndSaveCodepage, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_CODEPAGE;
      if (SendMessage(hWndSaveSelection, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_SELECTION;
      if (SendMessage(hWndSaveWordWrap, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_WORDWRAP;
      if (SendMessage(hWndSaveReadOnly, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_READONLY;
      if (SendMessage(hWndSaveOvertype, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_OVERTYPE;
      if (SendMessage(hWndSaveBookmarks, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_BOOKMARKS;
      if (SendMessage(hWndSaveAlias, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_CODERALIAS;
      if (SendMessage(hWndSaveFolds, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_CODERFOLDS;
      if (SendMessage(hWndSaveMarks, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwSaveData|=SSD_CODERMARKS;

      //Restart message
      if (bRestart)
        MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_RESTARTPROGRAM), wszPluginTitle, MB_OK|MB_ICONINFORMATION);

      GetWindowTextWide(hWndOpenName, wszOpenOnStart, MAX_PATH);
      GetWindowTextWide(hWndSaveName, wszSaveOnExit, MAX_PATH);
      bOpenOnStart=(BOOL)SendMessage(hWndOpenOnStart, BM_GETCHECK, 0, 0);
      bSaveOnExit=(BOOL)SendMessage(hWndSaveOnExit, BM_GETCHECK, 0, 0);

      dwSaveFlags|=OF_SETTINGS;
      EndDialog(hDlg, 0);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }
  return FALSE;
}

LRESULT CALLBACK NewTreeViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_SETFOCUS ||
      uMsg == WM_KILLFOCUS)
  {
    //Multi-selection
    InvalidateRect(hWnd, NULL, FALSE);
  }
  else if (uMsg == WM_DROPFILES)
  {
    if (lpVirtualSession)
      DropFiles((HDROP)wParam, hWnd);
    else
      MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_DROPTOCURRENT), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
    return 0;
  }
  else if (uMsg == WM_RBUTTONDOWN)
  {
    return DefWindowProcWide(hWnd, uMsg, wParam, lParam);
  }
  else if (uMsg == WM_LBUTTONDOWN)
  {
    if (lpVirtualSession)
    {
      //Multi-selection
      TVHITTESTINFO tvhti;
      TVITEMW tvi;
      HTREEITEM hItemCaret=(HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
      HTREEITEM hItemCursor=NULL;
      SESSIONITEM *siCursor;

      if (GetFocus() != hWnd)
        SetFocus(hWnd);

      //Get cursor item
      GetCursorPos(&tvhti.pt);
      ScreenToClient(hWnd, &tvhti.pt);
      SendMessage(hWnd, TVM_HITTEST, 0, (LPARAM)&tvhti);
      if (tvhti.hItem)
      {
        //Collapse/Expand item
        if (tvhti.flags & TVHT_ONITEMBUTTON)
        {
          SendMessage(hWnd, TVM_EXPAND, TVE_TOGGLE, (LPARAM)tvhti.hItem);

          tvi.mask=TVIF_STATE|TVIF_PARAM;
          tvi.stateMask=TVIS_EXPANDED;
          tvi.hItem=tvhti.hItem;
          if (TreeView_GetItemWide(hWnd, &tvi))
          {
            siCursor=(SESSIONITEM *)tvi.lParam;

            if (tvi.state & TVIS_EXPANDED)
            {
              if (siCursor->dwFolderFlags & FLDF_CLOSE)
              {
                siCursor->dwFolderFlags&=~FLDF_CLOSE;
                siCursor->dwFolderFlags|=FLDF_OPEN;
                lpVirtualSession->bModified=TRUE;
              }
            }
            else
            {
              if (siCursor->dwFolderFlags & FLDF_OPEN)
              {
                siCursor->dwFolderFlags&=~FLDF_OPEN;
                siCursor->dwFolderFlags|=FLDF_CLOSE;
                lpVirtualSession->bModified=TRUE;
              }
            }

            //TreeView control sets caret to collapsed item without
            //notifications, so change data in stack accordingly.
            if (siCursor->dwFolderFlags & FLDF_CLOSE)
            {
              SESSIONITEM *lpCount;
              int nResetCount=0;

              //Reset selection in childrens
              for (lpCount=siCursor->firstChild; lpCount; lpCount=StackNextItem(lpCount, siCursor))
              {
                if (lpCount->dwState & TVIS_SELECTED)
                {
                  SelectItem(hWnd, lpCount, FALSE);
                  ++nResetCount;
                }
              }
              if (nResetCount)
                SelectItem(hWnd, siCursor, TRUE);
            }
            PostMessage(hWndMainDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
          }
          return TRUE;
        }
        if (tvhti.flags & (TVHT_ONITEMLABEL|TVHT_ONITEMICON))
          hItemCursor=tvhti.hItem;
      }

      if (hItemCursor && (siCursor=(SESSIONITEM *)TreeItemParam(hWnd, hItemCursor)) && (siCursor->dwState & TVIS_SELECTED) && GetKeyState(VK_SHIFT) >= 0 && GetKeyState(VK_CONTROL) >= 0)
      {
        //Cursor item is selected, so don't process click without Ctrl and Shift, because we can start dragging.
      }
      else
      {
        if (hItemCaret == hItemCursor)
        {
          NMTREEVIEWW nmtv;

          nmtv.hdr.hwndFrom=hWnd;
          nmtv.hdr.idFrom=IDC_ITEMS_LIST;
          nmtv.hdr.code=TVN_SELCHANGEDW;
          nmtv.action=TVC_BYMOUSE;

          xmemset(&nmtv.itemOld, 0, sizeof(TVITEMW));
          nmtv.itemOld.mask=TVIF_STATE|TVIF_PARAM;
          nmtv.itemOld.stateMask=TVIS_SELECTED;
          nmtv.itemOld.hItem=hItemCursor;
          TreeView_GetItemWide(hWnd, &nmtv.itemOld);
          xmemcpy(&nmtv.itemNew, &nmtv.itemOld, sizeof(TVITEMW));
          SendMessage(GetParent(hWnd), WM_NOTIFY, IDC_ITEMS_LIST, (LPARAM)&nmtv);
        }
        else SendMessage(hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItemCursor);
      }
      nMouseMove=4;
      return TRUE;
    }
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (!bDragging)
    {
      if (nMouseMove > 0)
      {
        if (--nMouseMove == 0)
          bDragging=TRUE;
      }
    }
    if (bDragging)
    {
      SESSIONITEM *siNewCursor=NULL;
      HTREEITEM hItemCursor;

      SetCursor(hCursorDragMove);
      if (hItemCursor=TreeCursorItem(hWnd))
        siNewCursor=(SESSIONITEM *)TreeItemParam(hWnd, hItemCursor);
      if (siNewCursor != siDraggingCursor && siDraggingCursor)
      {
        //Erase old item background
        TreeUpdateItem(hWnd, siDraggingCursor->hItem);
      }

      if (siNewCursor)
      {
        //Draw current item background
        siDraggingCursor=siNewCursor;
        TreeUpdateItem(hWnd, siDraggingCursor->hItem);
      }
      else siDraggingCursor=NULL;
    }
  }
  else if (uMsg == WM_LBUTTONUP)
  {
    nMouseMove=0;

    if (bDragging)
    {
      //Drop
      HTREEITEM hItemAfterThis=TVI_LAST;
      SESSIONITEM *siParent=NULL;
      SESSIONITEM *siAfterThis=siDraggingCursor;

      if (siAfterThis)
      {
        if (!siAfterThis->dwFolderFlags)
        {
          siParent=siAfterThis->parent;
          hItemAfterThis=siAfterThis->hItem;
        }
        else
        {
          siParent=siAfterThis;
          siAfterThis=ASI_FIRST;
          hItemAfterThis=TVI_FIRST;
        }
      }
      else siAfterThis=ASI_LAST;

      if (lpVirtualSession)
      {
        SESSIONITEM *lpCount=lpVirtualSession->hItemsStack.first;
        SESSIONITEM *lpPrevCount=NULL;
        HTREEITEM hItemCaret=(HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);

        while (lpCount)
        {
          if ((lpCount->dwState & TVIS_SELECTED) && !lpCount->bMoving)
          {
            lpCount->bMoving=TRUE;

            if (TreeMoveItem(hWnd, hItemCaret, lpVirtualSession, lpCount, siParent, siAfterThis))
            {
              siAfterThis=lpCount;
              if (!lpPrevCount)
                lpCount=lpVirtualSession->hItemsStack.first;
              else
              {
                if (lpPrevCount->bMoving)
                  lpCount=StackNextItemNoChild(lpPrevCount);
                else
                  lpCount=StackNextItem(lpPrevCount, NULL);
              }
              lpVirtualSession->bModified=TRUE;
              continue;
            }
            else siAfterThis=lpCount;
          }
          lpPrevCount=lpCount;
          if (lpCount->bMoving)
            lpCount=StackNextItemNoChild(lpCount);
          else
            lpCount=StackNextItem(lpCount, NULL);
        }
        for (lpCount=lpVirtualSession->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
        {
          if (lpCount->bMoving)
            lpCount->bMoving=FALSE;
        }
        PostMessage(hWndMainDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
      }
      bDragging=FALSE;
      siDraggingCursor=NULL;
      InvalidateRect(hWnd, NULL, FALSE);
    }
  }
  else if (uMsg == WM_GETDLGCODE)
  {
    MSG *msg=(MSG *)lParam;

    if (msg)
    {
      if (msg->message == WM_KEYDOWN)
      {
        if (msg->wParam == VK_RETURN)
        {
          return DLGC_WANTALLKEYS;
        }
      }
    }
  }
  else if (uMsg == WM_KEYDOWN ||
           uMsg == WM_SYSKEYDOWN)
  {
    BOOL bAlt=FALSE;
    BOOL bShift=FALSE;
    BOOL bControl=FALSE;

    if (GetKeyState(VK_MENU) < 0)
      bAlt=TRUE;
    if (GetKeyState(VK_SHIFT) < 0)
      bShift=TRUE;
    if (GetKeyState(VK_CONTROL) < 0)
      bControl=TRUE;

    if (wParam == VK_RETURN)
    {
      if (!bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_OPEN, 0);
        return TRUE;
      }
    }
    else if (wParam == L'W')
    {
      if (!bAlt && !bShift && bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_CLOSE, 0);
        return TRUE;
      }
    }
    else if (wParam == VK_INSERT)
    {
      if (!bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_ADDFILES, 0);
        return TRUE;
      }
    }
    else if (wParam == VK_F2)
    {
      if (!bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_RENAME, 0);
        return TRUE;
      }
    }
    else if (wParam == VK_F7)
    {
      if (!bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_ADDDIR, 0);
        return TRUE;
      }
    }
    else if (wParam == VK_DELETE)
    {
      if (!bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_DELETE, 0);
        return TRUE;
      }
    }
    if (wParam == VK_UP)
    {
      if (bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_MOVEUP, 0);
        return TRUE;
      }
    }
    else if (wParam == VK_DOWN)
    {
      if (bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_MOVEDOWN, 0);
        return TRUE;
      }
    }
  }

  return CallWindowProcWide(OldTreeViewProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDN_MAIN_ONSTART_SHOW)
  {
    if (bOpenOnStart)
    {
      OpenSessionFile(wszOpenOnStart);
    }
  }
  else if (uMsg == AKDN_DOCK_RESIZE)
  {
    if (dkSessionsDlg)
    {
      if (((DOCK *)wParam)->hWnd == dkSessionsDlg->hWnd)
        dwSaveFlags|=OF_RECT;
    }
  }
  else if (uMsg == AKDN_SIZE_ONSTART)
  {
    if (bSessionsDockWaitResize)
    {
      bSessionsDockWaitResize=FALSE;
      ShowWindow(hWndDockDlg, SW_SHOW);
      dkSessionsDlg->dwFlags&=~DKF_HIDDEN;
    }
  }
  else if (uMsg == AKDN_DLLCODER)
  {
    NCODERUPDATE *ncu=(NCODERUPDATE *)lParam;

    if (ncu->dwFlags & SAE_RESETLIST)
      SetListColors(NULL, hWndItemsList);
  }
  else if (uMsg == WM_CLOSE ||
           uMsg == WM_QUERYENDSESSION)
  {
    if (bSaveOnExit)
    {
      if (*wszSaveOnExit)
        SaveCurrentSession(wszSaveOnExit);
    }
    if (hWndMainDlg)
    {
      if (SaveSessionPrompt() == IDCANCEL)
        return FALSE;
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
    if (hWndMainDlg)
    {
      if (nDialogType == DLGT_DOCKABLE)
        DestroyDock(hWndDockDlg, DKT_ONMAINFINISH);
      else
        SendMessage(hWndMainDlg, WM_COMMAND, IDCANCEL, 0);
    }
    return FALSE;
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_SETFONT)
  {
    if (hWndMainDlg && !bListSystemFont)
    {
      SendMessage(hWndItemsList, WM_SETFONT, wParam, FALSE);
    }
  }

  //Call next procedure
  return NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

BOOL LoadSessionFile(SESSION *ss)
{
  SESSIONITEM *siNew;
  SESSIONITEM *siParent=NULL;
  wchar_t wszName[MAX_PATH];
  wchar_t wszSessionFile[MAX_PATH];
  wchar_t wszDocumentFile[MAX_PATH];
  wchar_t *wszText;
  const wchar_t *wpText;
  const wchar_t *wpTextBegin;
  const wchar_t *wpFolderStart;
  const wchar_t *wpName;
  const wchar_t *wpBookmark;
  const wchar_t *wpAlias;
  const wchar_t *wpFold;
  const wchar_t *wpMark;
  HANDLE hFile;
  CHARRANGE64 cr;
  DWORD dwFileSize;
  DWORD dwBytesRead;
  DWORD dwUnicodeLen;
  INT_PTR nFirstVisChar;
  DWORD dwFolderFlags;
  BOOL bWordWrap;
  BOOL bReadOnly;
  BOOL bOvertypeMode;
  wchar_t *wszBookmarks;
  wchar_t *wszCoderAlias;
  wchar_t *wszCoderFolds;
  wchar_t *wszCoderMarks;
  int nCodePage;
  int nTabActive;
  int nMessageID;
  int i;

  xprintfW(wszSessionFile, L"%s\\%s.session", wszSessionsDir, ss->wszSessionName);
  if ((hFile=CreateFileWide(wszSessionFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
    return FALSE;
  if ((dwFileSize=GetFileSize(hFile, NULL)) != INVALID_FILE_SIZE)
  {
    if (dwFileSize >= 2)
    {
      if (wszText=(wchar_t *)GlobalAlloc(GPTR, dwFileSize + 2))
      {
        if (ReadFile(hFile, wszText, dwFileSize, &dwBytesRead, NULL))
        {
          wpText=(const wchar_t *)wszText;
          dwUnicodeLen=dwBytesRead / sizeof(wchar_t);
          wszText[dwUnicodeLen++]=L'\0';
          if (wszText[0] == 0xFEFF)
          {
            ++wpText;
            --dwUnicodeLen;
          }
          wpTextBegin=wpText;

          while (*wpText)
          {
            SkipSpaces(&wpText);

            //Folder end
            if (*wpText == L'}')
            {
              if (!siParent)
              {
                nMessageID=STRID_PARSEMSG_NOOPENBRACKET;
                goto Error;
              }
              ++wpText;
              siParent=siParent->parent;
              continue;
            }

            //Folder begin
            if (*wpText == L'\"' || (*wpText == L'-' && *(wpText + 1) == L'\"'))
            {
              dwFolderFlags=FLDF_CLOSE;
              if (*wpText == L'-')
              {
                dwFolderFlags&=~FLDF_CLOSE;
                dwFolderFlags|=FLDF_OPEN;
                wpText+=2;
              }
              else ++wpText;

              for (wpFolderStart=wpText; *wpText != L'\r' && *wpText != L'\0'; ++wpText)
              {
                if (*wpText == L'\"')
                {
                  if (siParent=StackAddItem(ss, siParent, ASI_LAST))
                  {
                    xstrcpynW(siParent->wszName, wpFolderStart, (wpText - wpFolderStart) + 1);
                    siParent->dwFolderFlags=dwFolderFlags;
                  }
                  ++wpText;
                  break;
                }
              }
              if (!NextLine(&wpText)) goto FreeText;
              SkipSpaces(&wpText);

              if (*wpText != L'{')
              {
                nMessageID=STRID_PARSEMSG_NOOPENBRACKET;
                goto Error;
              }
              ++wpText;
              SkipSpaces(&wpText);
              continue;
            }

            //File name
            for (i=0; i < MAX_PATH && *wpText != L'\r' && *wpText != L'\0'; wszDocumentFile[i++]=*wpText++);
            wszDocumentFile[i]=L'\0';
            if (!NextLine(&wpText)) goto FreeText;
            SkipSpaces(&wpText);

            //File parameters
            wszName[0]=L'\0';
            nTabActive=0;
            nCodePage=0;
            cr.cpMin=0;
            cr.cpMax=0;
            nFirstVisChar=0;
            bWordWrap=FALSE;
            bReadOnly=FALSE;
            bOvertypeMode=FALSE;
            wszBookmarks=NULL;
            wszCoderAlias=NULL;
            wszCoderFolds=NULL;
            wszCoderMarks=NULL;

            while (*wpText != L'\r' && *wpText != L'\0')
            {
              if (*wpText == L'/')
              {
                ++wpText;

                if (!xstrcmpnW(L"Name=", wpText, (UINT_PTR)-1))
                {
                  wpText+=5;
                  if (*wpText == L'\"')
                  {
                    for (wpName=++wpText; *wpText != L'\"' && *wpText != L'\0'; ++wpText);

                    if (*wpText == L'\"')
                    {
                      xstrcpynW(wszName, wpName, min(wpText - wpName + 1, MAX_PATH));
                      ++wpText;
                    }
                  }
                }
                else if (!xstrcmpnW(L"Active=", wpText, (UINT_PTR)-1))
                {
                  wpText+=7;
                  if (xatoiW(wpText, &wpText) == 1)
                    nTabActive=1;
                }
                else if (!xstrcmpnW(L"Codepage=", wpText, (UINT_PTR)-1))
                {
                  wpText+=9;
                  nCodePage=(int)xatoiW(wpText, &wpText);
                }
                else if (!xstrcmpnW(L"Selection=", wpText, (UINT_PTR)-1))
                {
                  wpText+=10;
                  cr.cpMin=cr.cpMax=xatoiW(wpText, &wpText);
                  if (*wpText == L'-')
                    cr.cpMax=xatoiW(++wpText, &wpText);
                }
                else if (!xstrcmpnW(L"FirstVisChar=", wpText, (UINT_PTR)-1))
                {
                  wpText+=13;
                  nFirstVisChar=xatoiW(wpText, &wpText);
                }
                else if (!xstrcmpnW(L"WordWrap=", wpText, (UINT_PTR)-1))
                {
                  wpText+=9;
                  bWordWrap=(BOOL)xatoiW(wpText, &wpText);
                }
                else if (!xstrcmpnW(L"ReadOnly=", wpText, (UINT_PTR)-1))
                {
                  wpText+=9;
                  bReadOnly=(BOOL)xatoiW(wpText, &wpText);
                }
                else if (!xstrcmpnW(L"Overtype=", wpText, (UINT_PTR)-1))
                {
                  wpText+=9;
                  bOvertypeMode=(BOOL)xatoiW(wpText, &wpText);
                }
                else if (!xstrcmpnW(L"Bookmarks=", wpText, (UINT_PTR)-1))
                {
                  wpText+=10;
                  wpBookmark=wpText;

                  while (*wpText >= L'0' && *wpText <= L'9')
                  {
                    xatoiW(wpText, &wpText);
                    if (*wpText == L',')
                      ++wpText;
                  }
                  if (wszBookmarks=(wchar_t *)GlobalAlloc(GPTR, (wpText - wpBookmark + 1) * sizeof(wchar_t)))
                  {
                    xstrcpynW(wszBookmarks, wpBookmark, wpText - wpBookmark + 1);
                  }
                }
                else if (!xstrcmpnW(L"CoderAlias=", wpText, (UINT_PTR)-1))
                {
                  wpText+=11;
                  if (*wpText == L'\"')
                  {
                    for (wpAlias=++wpText; *wpText != L'\"' && *wpText != L'\0'; ++wpText);

                    if (*wpText == L'\"')
                    {
                      if (wszCoderAlias=(wchar_t *)GlobalAlloc(GPTR, (wpText - wpAlias + 1) * sizeof(wchar_t)))
                      {
                        xstrcpynW(wszCoderAlias, wpAlias, wpText - wpAlias + 1);
                      }
                      ++wpText;
                    }
                  }
                }
                else if (!xstrcmpnW(L"Folds=", wpText, (UINT_PTR)-1))
                {
                  wpText+=6;
                  wpFold=wpText;

                  while (*wpText >= L'0' && *wpText <= L'9')
                  {
                    xatoiW(wpText, &wpText);
                    if (*wpText == L',')
                      ++wpText;
                  }
                  if (wszCoderFolds=(wchar_t *)GlobalAlloc(GPTR, (wpText - wpFold + 1) * sizeof(wchar_t)))
                  {
                    xstrcpynW(wszCoderFolds, wpFold, wpText - wpFold + 1);
                  }
                }
                else if (!xstrcmpnW(L"Marks=", wpText, (UINT_PTR)-1))
                {
                  wpText+=6;
                  if (*wpText == L'(')
                  {
                    wpMark=wpText;

                    NextMark:
                    while (*++wpText != L')' && *wpText != L'\0')
                    {
                      if (*wpText == L'\"')
                      {
                        while (*++wpText != L'\"')
                        {
                          if (*wpText == L'\\') ++wpText;
                        }
                      }
                    }
                    if (*wpText == L')' && *++wpText == L',' && *++wpText == L'(')
                      goto NextMark;
                    if (wszCoderMarks=(wchar_t *)GlobalAlloc(GPTR, (wpText - wpMark + 1) * sizeof(wchar_t)))
                    {
                      xstrcpynW(wszCoderMarks, wpMark, wpText - wpMark + 1);
                    }
                  }
                }
                continue;
              }
              ++wpText;
            }
            if (wszDocumentFile[0])
            {
              if (siNew=StackAddItem(ss, siParent, ASI_LAST))
              {
                if (wszName[0])
                  xstrcpynW(siNew->wszName, wszName, MAX_PATH);
                else
                  xstrcpynW(siNew->wszName, GetFileName(wszDocumentFile, -1), MAX_PATH);
                xstrcpynW(siNew->wszItemFile, wszDocumentFile, MAX_PATH);
                TranslateFileString(wszDocumentFile, siNew->wszItemExpFile, MAX_PATH);
                siNew->nTabActive=nTabActive;
                siNew->nCodePage=nCodePage;
                siNew->nSelStart=cr.cpMin;
                siNew->nSelEnd=cr.cpMax;
                siNew->nFirstVisChar=nFirstVisChar;
                siNew->bWordWrap=bWordWrap;
                siNew->bReadOnly=bReadOnly;
                siNew->bOvertypeMode=bOvertypeMode;
                siNew->wszBookmarks=wszBookmarks;
                siNew->wszCoderAlias=wszCoderAlias;
                siNew->wszCoderFolds=wszCoderFolds;
                siNew->wszCoderMarks=wszCoderMarks;
              }
            }
          }
          if (siParent)
          {
            nMessageID=STRID_PARSEMSG_NOCLOSEBRACKET;
            goto Error;
          }
        }
        goto FreeText;

        Error:
        if (EditSessionFile(ss->wszSessionName))
        {
          CHARRANGE64 cr;
          HWND hWndEdit=(HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL);

          cr.cpMin=wpText - wpTextBegin;
          cr.cpMax=cr.cpMin + 1;
          SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
        }
        MessageBoxW(hMainWnd, GetLangStringW(wLangModule, nMessageID), wszPluginTitle, MB_OK|MB_ICONERROR);

        FreeText:
        GlobalFree((HGLOBAL)wszText);
      }
    }
  }
  CloseHandle(hFile);
  return TRUE;
}

BOOL RenameSessionFile(const wchar_t *wpOldSessionName, const wchar_t *wpNewSessionName)
{
  wchar_t wszOldSession[MAX_PATH];
  wchar_t wszNewSession[MAX_PATH];

  xprintfW(wszOldSession, L"%s\\%s.session", wszSessionsDir, wpOldSessionName);
  xprintfW(wszNewSession, L"%s\\%s.session", wszSessionsDir, wpNewSessionName);
  return MoveFileWide(wszOldSession, wszNewSession);
}

void OpenSessionFile(const wchar_t *wpSessionName)
{
  SESSION ss;

  xmemset(&ss, 0, sizeof(SESSION));
  xstrcpynW(ss.wszSessionName, wpSessionName, MAX_PATH);

  if (LoadSessionFile(&ss))
  {
    OpenSession(&ss.hItemsStack);
    StackFreeItems(&ss);
  }
}

void CloseSessionFile(const wchar_t *wpSessionName)
{
  SESSION ss;

  xmemset(&ss, 0, sizeof(SESSION));
  xstrcpynW(ss.wszSessionName, wpSessionName, MAX_PATH);

  if (LoadSessionFile(&ss))
  {
    CloseSession(&ss.hItemsStack);
    StackFreeItems(&ss);
  }
}

BOOL EditSessionFile(const wchar_t *wpSessionName)
{
  wchar_t wszSessionFile[MAX_PATH];
  OPENDOCUMENTW od;

  xprintfW(wszSessionFile, L"%s\\%s.session", wszSessionsDir, wpSessionName);
  od.pFile=wszSessionFile;
  od.pWorkDir=NULL;
  od.dwFlags=OD_ADT_DETECTBOM|OD_ADT_DETECTCODEPAGE;
  od.nCodePage=0;
  od.bBOM=0;
  od.hDoc=NULL;
  return !SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);
}

void SaveSessionFile(SESSION *ss)
{
  SESSIONITEM *si=ss->hItemsStack.first;
  wchar_t wszSessionFile[MAX_PATH];
  wchar_t wszCoderAlias[MAX_PATH];
  wchar_t *wszData;
  wchar_t *wpCount;
  wchar_t *wpMaxCount;
  wchar_t *wszSpacesIndent=NULL;
  INT_PTR nDataSize;
  HANDLE hFile;
  DWORD dwBytesWritten;
  int nLevel=0;
  int nMaxLevel=-1;

  if (nSaveSessions == SDIR_PROGRAM)
    CreateDirectoryWide(wszSessionsDir, NULL);
  else if (nSaveSessions == SDIR_APPDATA)
    CreateDirectoryRecursive(wszSessionsDir);

  xprintfW(wszSessionFile, L"%s\\%s.session", wszSessionsDir, ss->wszSessionName);
  if ((hFile=CreateFileWide(wszSessionFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
    return;
  if (WriteFile(hFile, "\xFF\xFE", 2, &dwBytesWritten, NULL))
  {
    while (si)
    {
      if (nLevel > nMaxLevel)
      {
        if (wszSpacesIndent)
        {
          GlobalFree((HGLOBAL)wszSpacesIndent);
          wszSpacesIndent=NULL;
        }
        if (wszSpacesIndent=(wchar_t *)GlobalAlloc(GPTR, (nLevel * 2 + 1) * sizeof(wchar_t)))
        {
          for (wpCount=wszSpacesIndent, wpMaxCount=wszSpacesIndent + (nLevel * 2); wpCount < wpMaxCount; ++wpCount)
            *wpCount=L' ';
        }
        nMaxLevel=nLevel;
      }

      if (si->wszCoderAlias)
        xprintfW(wszCoderAlias, L"\"%s\"", si->wszCoderAlias);
      else
        wszCoderAlias[0]=L'\0';

      //Write data
      dwBytesWritten=0;
      wszData=NULL;

      for (;;)
      {
        if (si->dwFolderFlags)
        {
          //Is empty folder
          if (!si->firstChild)
            nDataSize=xprintfW(wszData, L"%s%.%ds%s\"%s\"\r\n%.%ds{\r\n%.%ds}\r\n",
                                        (si->prev && !si->prev->dwFolderFlags)?L"\r\n":L"", nLevel * 2, wszSpacesIndent, (si->dwFolderFlags & FLDF_OPEN)?L"-":L"", si->wszName, nLevel * 2, wszSpacesIndent, nLevel * 2, wszSpacesIndent);
          else
            nDataSize=xprintfW(wszData, L"%s%.%ds%s\"%s\"\r\n%.%ds{\r\n",
                                        (si->prev && !si->prev->dwFolderFlags)?L"\r\n":L"", nLevel * 2, wszSpacesIndent, (si->dwFolderFlags & FLDF_OPEN)?L"-":L"", si->wszName, nLevel * 2, wszSpacesIndent);
        }
        else
        {
          //File
          if (bSaveRelative)
          {
            if (!xstrcmpinW(wszExeDir, si->wszItemFile, (UINT_PTR)-1))
              xprintfW(si->wszItemFile, L"%%a%s", si->wszItemFile + xstrlenW(wszExeDir));
          }
          nDataSize=xprintfW(wszData, L"%s%.%ds%s\r\n%.%ds/Name=\"%s\" /Active=%d /Codepage=%d /Selection=%Id-%Id /FirstVisChar=%Id /WordWrap=%d /ReadOnly=%d /Overtype=%d /Bookmarks=%s /CoderAlias=%s /Folds=%s /Marks=%s\r\n",
                                      (si->prev && !si->prev->dwFolderFlags)?L"\r\n":L"", nLevel * 2, wszSpacesIndent, si->wszItemFile, nLevel * 2, wszSpacesIndent, si->wszName, si->nTabActive, si->nCodePage, si->nSelStart, si->nSelEnd, si->nFirstVisChar, si->bWordWrap, si->bReadOnly, si->bOvertypeMode, si->wszBookmarks, wszCoderAlias, si->wszCoderFolds, si->wszCoderMarks);
        }

        if (!wszData)
        {
          if (wszData=(wchar_t *)GlobalAlloc(GPTR, nDataSize * sizeof(wchar_t)))
            continue;
          else
            break;
        }
        WriteFile(hFile, wszData, (DWORD)nDataSize * sizeof(wchar_t), &dwBytesWritten, NULL);
        GlobalFree((HGLOBAL)wszData);
        break;
      }
      if (!dwBytesWritten) break;

      //Next item
      if (si->firstChild)
      {
        si=si->firstChild;
        ++nLevel;
        continue;
      }

      do
      {
        if (si->next)
        {
          si=si->next;
          break;
        }
        si=si->parent;
        --nLevel;

        if (si)
        {
          WriteFile(hFile, wszSpacesIndent, (nLevel * 2) * sizeof(wchar_t), &dwBytesWritten, NULL);
          WriteFile(hFile, L"}\r\n", 3 * sizeof(wchar_t), &dwBytesWritten, NULL);
        }
      }
      while (si);
    }
  }
  if (wszSpacesIndent) GlobalFree((HGLOBAL)wszSpacesIndent);
  CloseHandle(hFile);
}

void SaveCurrentSession(const wchar_t *wpSessionName)
{
  STACKSESSION hCloseStack={0};
  SESSION ssDummy;
  SESSION *ss;

  if (ss=AddCurrentSession(&hCloseStack, wpSessionName))
  {
    SaveSessionFile(ss);
    DeleteSession(&hCloseStack, ss);
  }
  else
  {
    //Save empty session
    xmemset(&ssDummy, 0, sizeof(SESSION));
    xstrcpynW(ssDummy.wszSessionName, wpSessionName, MAX_PATH);
    SaveSessionFile(&ssDummy);
  }
}

BOOL DeleteSessionFile(const wchar_t *wpSessionName)
{
  wchar_t wszSession[MAX_PATH];

  xprintfW(wszSession, L"%s\\%s.session", wszSessionsDir, wpSessionName);
  return DeleteFileWide(wszSession);
}

int SaveSessionPrompt()
{
  int nChoice=0;

  if (lpVirtualSession && lpVirtualSession->bModified)
  {
    xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_SESSION_CHANGED), lpVirtualSession->wszSessionName);
    nChoice=MessageBoxW(hWndMainDlg, wszBuffer, wszPluginTitle, MB_YESNOCANCEL|MB_ICONEXCLAMATION);

    if (nChoice == IDYES)
    {
      StackFreeItems(lpRealSession);
      lpVirtualSession->bModified=FALSE;
      CopySession(lpVirtualSession, lpRealSession);
      SaveSessionFile(lpRealSession);
    }
  }
  return nChoice;
}

void LoadSessions(STACKSESSION *hStack)
{
  SESSION *ss;
  WIN32_FIND_DATAW wfd;
  HANDLE hFind;
  wchar_t wszFindFiles[MAX_PATH];
  wchar_t wszBaseName[MAX_PATH];

  //Sessions files
  xprintfW(wszFindFiles, L"%s\\*.session", wszSessionsDir);

  if ((hFind=FindFirstFileWide(wszFindFiles, &wfd)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      if (wfd.cFileName[0] == L'.' && (wfd.cFileName[1] == L'\0' || (wfd.cFileName[1] == L'.' && wfd.cFileName[2] == L'\0'))) continue;

      GetBaseName(wfd.cFileName, -1, wszBaseName, MAX_PATH);

      if (ss=AddEmptySession(hStack, wszBaseName))
      {
        LoadSessionFile(ss);
      }
    }
    while (FindNextFileWide(hFind, &wfd));

    FindClose(hFind);
  }
}

SESSION* AddEmptySession(STACKSESSION *hStack, const wchar_t *wpSessionName)
{
  SESSION *ss=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&ss, -1, sizeof(SESSION)))
  {
    xstrcpynW(ss->wszSessionName, wpSessionName, MAX_PATH);
  }
  return ss;
}

SESSION* AddCurrentSession(STACKSESSION *hStack, const wchar_t *wpSessionName)
{
  FRAMEDATA *lpFrameActive;
  FRAMEDATA *lpFrame;
  SESSION *lpSessionElement=NULL;
  SESSIONITEM *siNew=NULL;
  PLUGINFUNCTION *pfLineBoard=NULL;
  PLUGINFUNCTION *pfCoder=NULL;
  TCITEMW tcItem;
  CHARRANGE64 cr;
  HSTACK *hFoldsStack;
  wchar_t *wszBookmarks;
  wchar_t *wszCoderAlias;
  wchar_t *wszCoderFolds;
  wchar_t *wszCoderMarks;
  int nSize;
  int nTabActive;
  INT_PTR nFirstVisChar;
  int nItem=0;
  BOOL bLineBoardRunning=-1;
  BOOL bHighLightRunning=-1;
  BOOL bCodeFoldRunning=-1;
  BOOL bAutoCompleteRunning=-1;

  if (lpFrameActive=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0))
  {
    if (lpSessionElement=AddEmptySession(hStack, wpSessionName))
    {
      for (;;)
      {
        tcItem.mask=TCIF_PARAM;
        if (!TabCtrl_GetItemWide(hTab, nItem++, &tcItem))
          break;
        lpFrame=(FRAMEDATA *)tcItem.lParam;

        if (*lpFrame->ei.wszFile)
        {
          if (lpFrameActive == lpFrame)
            nTabActive=1;
          else
            nTabActive=0;

          if (nMDI == WMD_PMDI)
          {
            SendToDoc(lpFrame->ei.hDocEdit, lpFrame->ei.hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
            nFirstVisChar=SendToDoc(lpFrame->ei.hDocEdit, lpFrame->ei.hWndEdit, AEM_GETRICHOFFSET, AEGI_FIRSTVISIBLECHAR, 0);

          }
          else
          {
            SendMessage(lpFrame->ei.hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
            nFirstVisChar=SendMessage(lpFrame->ei.hWndEdit, AEM_GETRICHOFFSET, AEGI_FIRSTVISIBLECHAR, 0);
          }

          //Get LineBoard plugin bookmarks
          wszBookmarks=NULL;

          if (dwSaveData & SSD_BOOKMARKS)
          {
            if (bLineBoardRunning == -1)
            {
              if (pfLineBoard=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"LineBoard::Main", 0))
                bLineBoardRunning=pfLineBoard->bRunning;
              else
                bLineBoardRunning=FALSE;
            }
            if (bLineBoardRunning)
            {
              PLUGINCALLSENDW pcs;
              DLLEXTLINEBOARD delb;
              int nBookmarksLen=0;

              delb.dwStructSize=sizeof(DLLEXTLINEBOARD);
              delb.nAction=DLLA_LINEBOARD_GETBOOKMARKS;
              delb.hWndEdit=lpFrame->ei.hWndEdit;
              delb.hDocEdit=lpFrame->ei.hDocEdit;
              delb.wszBookmarks=NULL;
              delb.lpnBookmarksLen=&nBookmarksLen;

              //Calculate bookmarks size
              pcs.pFunction=L"LineBoard::Main";
              pcs.lParam=(LPARAM)&delb;
              pcs.dwSupport=0;
              SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

              //Get bookmarks
              if (nBookmarksLen)
              {
                if (delb.wszBookmarks=(wchar_t *)GlobalAlloc(GPTR, nBookmarksLen * sizeof(wchar_t)))
                  SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
                wszBookmarks=delb.wszBookmarks;
              }
            }
          }

          //Get Coder plugin alias
          wszCoderAlias=NULL;

          if (dwSaveData & SSD_CODERALIAS)
          {
            if (bHighLightRunning == -1)
            {
              if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfCoder->bRunning)
                bHighLightRunning=TRUE;
              else
                bHighLightRunning=FALSE;
            }
            if (bCodeFoldRunning == -1)
            {
              if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::CodeFold", 0)) && pfCoder->bRunning)
                bCodeFoldRunning=TRUE;
              else
                bCodeFoldRunning=FALSE;
            }
            if (bAutoCompleteRunning == -1)
            {
              if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::AutoComplete", 0)) && pfCoder->bRunning)
                bAutoCompleteRunning=TRUE;
              else
                bAutoCompleteRunning=FALSE;
            }
            if (bHighLightRunning || bCodeFoldRunning || bAutoCompleteRunning)
            {
              if (wszCoderAlias=(wchar_t *)GlobalAlloc(GPTR, MAX_PATH * sizeof(wchar_t)))
              {
                PLUGINCALLSENDW pcs;
                DLLEXTCODERGETALIAS decga;
                INT_PTR nAliasLen=0;

                decga.dwStructSize=sizeof(DLLEXTCODERGETALIAS);
                decga.nAction=DLLA_CODER_GETALIAS;
                decga.hWndEdit=lpFrame->ei.hWndEdit;
                decga.hDocEdit=lpFrame->ei.hDocEdit;
                decga.wpAlias=wszCoderAlias;
                decga.lpnAliasLen=&nAliasLen;

                pcs.pFunction=L"Coder::Settings";
                pcs.lParam=(LPARAM)&decga;
                pcs.dwSupport=PDS_STRWIDE;
                SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

                if (!nAliasLen)
                {
                  GlobalFree((HGLOBAL)wszCoderAlias);
                  wszCoderAlias=NULL;
                }
              }
            }
          }

          //Get Coder plugin folds
          wszCoderFolds=NULL;

          if (dwSaveData & SSD_CODERFOLDS)
          {
            if (hFoldsStack=(HSTACK *)SendToDoc(lpFrame->ei.hDocEdit, lpFrame->ei.hWndEdit, AEM_GETFOLDSTACK, 0, 0))
            {
              if (hFoldsStack->first)
              {
                if (nSize=GetCollapsedFoldsString(hFoldsStack, NULL))
                {
                  if (wszCoderFolds=(wchar_t *)GlobalAlloc(GPTR, nSize * sizeof(wchar_t)))
                    nSize=GetCollapsedFoldsString(hFoldsStack, wszCoderFolds);
                }
              }
            }
          }

          //Get Coder plugin marks
          wszCoderMarks=NULL;

          if (dwSaveData & SSD_CODERMARKS)
          {
            if (bHighLightRunning == -1)
            {
              if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfCoder->bRunning)
                bHighLightRunning=TRUE;
              else
                bHighLightRunning=FALSE;
            }
            if (bHighLightRunning)
            {
              PLUGINCALLSENDW pcs;
              DLLEXTHIGHLIGHTGETMARKSTACK dehgms;
              STACKMARKTEXT hMarkTextsStack={0};
              MARKTEXT *lpMarkText;
              AEMARKTEXTITEMW *lpMarkItem;
              wchar_t wszColorText[MAX_PATH];
              wchar_t wszColorBk[MAX_PATH];
              INT_PTR nTextLen;

              dehgms.dwStructSize=sizeof(DLLEXTHIGHLIGHTGETMARKSTACK);
              dehgms.nAction=DLLA_HIGHLIGHT_GETMARKSTACK;
              dehgms.hWndEdit=lpFrame->ei.hWndEdit;
              dehgms.hDocEdit=lpFrame->ei.hDocEdit;
              dehgms.lpMarkTextsStack=&hMarkTextsStack;

              pcs.pFunction=L"Coder::HighLight";
              pcs.lParam=(LPARAM)&dehgms;
              pcs.dwSupport=0;
              SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

              //Get text
              for (;;)
              {
                nTextLen=0;

                for (lpMarkText=hMarkTextsStack.first; lpMarkText; lpMarkText=lpMarkText->next)
                {
                  if (lpMarkText->dwMarkID != MARKID_SELECTION)
                  {
                    lpMarkItem=(AEMARKTEXTITEMW *)lpMarkText->hMarkTextHandle;

                    if (lpMarkItem->crText == (DWORD)-1)
                      xprintfW(wszColorText, L"0");
                    else
                      xprintfW(wszColorText, L"#%02X%02X%02X", GetRValue(lpMarkItem->crText), GetGValue(lpMarkItem->crText), GetBValue(lpMarkItem->crText));

                    if (lpMarkItem->crBk == (DWORD)-1)
                      xprintfW(wszColorBk, L"0");
                    else
                      xprintfW(wszColorBk, L"#%02X%02X%02X", GetRValue(lpMarkItem->crBk), GetGValue(lpMarkItem->crBk), GetBValue(lpMarkItem->crBk));

                    if (nTextLen) nTextLen+=xprintfW(wszCoderMarks?wszCoderMarks + nTextLen:NULL, L",");
                    nTextLen+=xprintfW(wszCoderMarks?wszCoderMarks + nTextLen:NULL, L"(%s,%s,%u,%d,%d,\"", wszColorText, wszColorBk, lpMarkItem->dwFlags, lpMarkItem->dwFontStyle, lpMarkText->dwMarkID);
                    nTextLen+=EscapeString(lpMarkItem->pMarkText, lpMarkItem->nMarkTextLen, wszCoderMarks?wszCoderMarks + nTextLen:NULL);
                    nTextLen+=xprintfW(wszCoderMarks?wszCoderMarks + nTextLen:NULL, L"\")");
                  }
                }
                if (!wszCoderMarks)
                {
                  if (wszCoderMarks=(wchar_t *)GlobalAlloc(GPTR, (nTextLen + 1) * sizeof(wchar_t)))
                    continue;
                  else
                    break;
                }
                else break;
              }
            }
          }

          if (siNew=StackAddItem(lpSessionElement, NULL, ASI_LAST))
          {
            xstrcpynW(siNew->wszName, GetFileName(lpFrame->ei.wszFile, -1), MAX_PATH);
            xstrcpynW(siNew->wszItemFile, lpFrame->ei.wszFile, MAX_PATH);
            xstrcpynW(siNew->wszItemExpFile, lpFrame->ei.wszFile, MAX_PATH);
            siNew->nTabActive=nTabActive;
            siNew->nCodePage=lpFrame->ei.nCodePage;
            siNew->nSelStart=cr.cpMin;
            siNew->nSelEnd=cr.cpMax;
            siNew->nFirstVisChar=nFirstVisChar;
            siNew->bWordWrap=lpFrame->ei.bWordWrap;
            siNew->bReadOnly=lpFrame->ei.bReadOnly;
            siNew->bOvertypeMode=lpFrame->ei.bOvertypeMode;
            siNew->wszBookmarks=wszBookmarks;
            siNew->wszCoderAlias=wszCoderAlias;
            siNew->wszCoderFolds=wszCoderFolds;
            siNew->wszCoderMarks=wszCoderMarks;
          }
        }
      }
    }
  }
  return lpSessionElement;
}

SESSION* GetSession(STACKSESSION *hStack, const wchar_t *wpSessionName)
{
  SESSION *ss;

  for (ss=hStack->first; ss; ss=ss->next)
  {
    if (!xstrcmpiW(ss->wszSessionName, wpSessionName))
      return ss;
  }
  return NULL;
}

void CopySessionLevel(SESSIONITEM *siFirstSource, SESSIONITEM *siLastSource, SESSIONITEM **psiFirstTarget, SESSIONITEM **psiLastTarget, SESSIONITEM *siTargetParent)
{
  SESSIONITEM *siFirstTarget;
  SIZE_T nSize;

  *psiFirstTarget=NULL;
  *psiLastTarget=NULL;
  StackCopy((stack *)siFirstSource, (stack *)siLastSource, (stack **)psiFirstTarget, (stack **)psiLastTarget, sizeof(SESSIONITEM));
  siFirstTarget=*psiFirstTarget;

  //Realloc strings
  while (siFirstSource)
  {
    siFirstTarget->parent=siTargetParent;
    siFirstTarget->firstChild=NULL;
    siFirstTarget->lastChild=NULL;

    if (siFirstSource->wszBookmarks)
    {
      nSize=GlobalSize((HGLOBAL)siFirstSource->wszBookmarks);
      if (siFirstTarget->wszBookmarks=(wchar_t *)GlobalAlloc(GPTR, nSize))
        xmemcpy(siFirstTarget->wszBookmarks, siFirstSource->wszBookmarks, nSize);
    }
    if (siFirstSource->wszCoderAlias)
    {
      nSize=GlobalSize((HGLOBAL)siFirstSource->wszCoderAlias);
      if (siFirstTarget->wszCoderAlias=(wchar_t *)GlobalAlloc(GPTR, nSize))
        xmemcpy(siFirstTarget->wszCoderAlias, siFirstSource->wszCoderAlias, nSize);
    }
    if (siFirstSource->wszCoderFolds)
    {
      nSize=GlobalSize((HGLOBAL)siFirstSource->wszCoderFolds);
      if (siFirstTarget->wszCoderFolds=(wchar_t *)GlobalAlloc(GPTR, nSize))
        xmemcpy(siFirstTarget->wszCoderFolds, siFirstSource->wszCoderFolds, nSize);
    }
    if (siFirstSource->wszCoderMarks)
    {
      nSize=GlobalSize((HGLOBAL)siFirstSource->wszCoderMarks);
      if (siFirstTarget->wszCoderMarks=(wchar_t *)GlobalAlloc(GPTR, nSize))
        xmemcpy(siFirstTarget->wszCoderMarks, siFirstSource->wszCoderMarks, nSize);
    }
    siFirstSource=siFirstSource->next;
    siFirstTarget=siFirstTarget->next;
  }
}

void CopySession(SESSION *ssSource, SESSION *ssTarget)
{
  SESSIONITEM *lpSourceParent=NULL;
  SESSIONITEM *lpTargetParent=NULL;
  SESSIONITEM *lpSourceSubling;
  SESSIONITEM *lpTargetSubling;

  xstrcpynW(ssTarget->wszSessionName, ssSource->wszSessionName, MAX_PATH);
  ssTarget->bModified=ssSource->bModified;
  CopySessionLevel(ssSource->hItemsStack.first, ssSource->hItemsStack.last, &ssTarget->hItemsStack.first, &ssTarget->hItemsStack.last, NULL);
  ssTarget->hItemsStack.nElements=ssSource->hItemsStack.nElements;
  lpSourceSubling=ssSource->hItemsStack.first;
  lpTargetSubling=ssTarget->hItemsStack.first;

  NextParent:
  while (lpSourceSubling)
  {
    if (lpSourceSubling->firstChild)
    {
      CopySessionLevel(lpSourceSubling->firstChild, lpSourceSubling->lastChild, &lpTargetSubling->firstChild, &lpTargetSubling->lastChild, lpTargetSubling);
      lpSourceSubling=lpSourceSubling->firstChild;
      lpTargetSubling=lpTargetSubling->firstChild;
      continue;
    }

    //Group doesn't have childrens
    lpSourceParent=lpSourceSubling->parent;
    lpTargetParent=lpTargetSubling->parent;
    lpSourceSubling=lpSourceSubling->next;
    lpTargetSubling=lpTargetSubling->next;
  }
  if (lpSourceParent)
  {
    lpSourceSubling=lpSourceParent->next;
    lpTargetSubling=lpTargetParent->next;
    lpSourceParent=lpSourceParent->parent;
    lpTargetParent=lpTargetParent->parent;
    goto NextParent;
  }
}

void OpenSession(STACKSESSIONITEM *hStack)
{
  SESSIONITEM *lpCount;
  FRAMEDATA *lpFrameActivate=NULL;

  for (lpCount=hStack->first; lpCount; lpCount=StackNextItem(lpCount, NULL))
  {
    if (!lpCount->dwFolderFlags && FileExistsWide(lpCount->wszItemExpFile))
    {
      OpenItem(lpCount);

      if (lpCount->nTabActive)
      {
        lpFrameActivate=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0);
      }
    }
  }

  if (lpFrameActivate) SendMessage(hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrameActivate);
}

void CloseSession(STACKSESSIONITEM *hStack)
{
  SESSIONITEM *lpCount;
  FRAMEDATA *lpFrame;

  for (lpCount=hStack->first; lpCount; lpCount=StackNextItem(lpCount, NULL))
  {
    if (!lpCount->dwFolderFlags && (lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)lpCount->wszItemExpFile)))
    {
      if (!CloseItem(lpFrame, FALSE))
        break;
    }
  }
}

void DeleteSession(STACKSESSION *hStack, SESSION *ss)
{
  StackFreeItems(ss);
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)ss);
}

void FreeSessions(STACKSESSION *hStack)
{
  SESSION *ss;

  for (ss=hStack->first; ss; ss=ss->next)
  {
    StackFreeItems(ss);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

SESSIONITEM* StackAddItem(SESSION *ss, SESSIONITEM *siParent, SESSIONITEM *siAfterThis)
{
  SESSIONITEM *siNew=NULL;
  SESSIONITEM **siFirst;
  SESSIONITEM **siLast;

  if (!siParent)
  {
    siFirst=&ss->hItemsStack.first;
    siLast=&ss->hItemsStack.last;
  }
  else
  {
    siFirst=&siParent->firstChild;
    siLast=&siParent->lastChild;
  }
  if (siAfterThis == ASI_FIRST)
    siAfterThis=NULL;
  else if (siAfterThis == ASI_LAST)
    siAfterThis=*siLast;

  if (!StackInsertAfter((stack **)siFirst, (stack **)siLast, (stack *)siAfterThis, (stack **)&siNew, sizeof(SESSIONITEM)))
  {
    siNew->parent=siParent;
    ++ss->hItemsStack.nElements;
  }
  return siNew;
}

int StackItemIndex(SESSION *ss, SESSIONITEM *si)
{
  SESSIONITEM *lpCount;
  int nIndex=1;

  for (lpCount=ss->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
  {
    if (lpCount == si)
      return nIndex;
    ++nIndex;
  }
  return 0;
}

BOOL StackFindSelRange(SESSION *ss, SESSIONITEM *lpItemInRange, SESSIONITEM **lppStartRangeItem, int *lpnStartRangeIndex, SESSIONITEM **lppEndRangeItem, int *lpnEndRangeIndex)
{
  SESSIONITEM *lpStartRangeItem=NULL;
  SESSIONITEM *lpEndRangeItem=NULL;
  SESSIONITEM *lpCount;
  int nStartRangeIndex=0;
  int nEndRangeIndex=0;
  int nIndex=1;
  BOOL bFound=FALSE;

  for (lpCount=ss->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
  {
    if (lpCount->dwState & TVIS_SELECTED)
    {
      if (!lpStartRangeItem)
      {
        lpStartRangeItem=lpCount;
        nStartRangeIndex=nIndex;
      }
      lpEndRangeItem=lpCount;
      nEndRangeIndex=nIndex;
      if (lpItemInRange == lpCount)
        bFound=TRUE;
    }
    else
    {
      if (bFound) break;
      lpStartRangeItem=NULL;
    }
    ++nIndex;
  }
  if (bFound)
  {
    if (lppStartRangeItem) *lppStartRangeItem=lpStartRangeItem;
    if (lpnStartRangeIndex) *lpnStartRangeIndex=nStartRangeIndex;
    if (lppEndRangeItem) *lppEndRangeItem=lpEndRangeItem;
    if (lpnEndRangeIndex) *lpnEndRangeIndex=nEndRangeIndex;
  }
  return bFound;
}

SESSIONITEM* StackEndSelRange(SESSION *ss, int *lpnIndex)
{
  SESSIONITEM *lpCount;
  SESSIONITEM *lpLastSelItem=NULL;
  int nLastSelIndex=0;
  int nIndex=1;

  for (lpCount=ss->hItemsStack.first; lpCount; lpCount=StackNextItem(lpCount, NULL))
  {
    if (lpCount->dwState & TVIS_SELECTED)
    {
      lpLastSelItem=lpCount;
      nLastSelIndex=nIndex;
    }
    ++nIndex;
  }
  if (lpnIndex) *lpnIndex=nLastSelIndex;
  return lpLastSelItem;
}

SESSIONITEM* StackNextItem(SESSIONITEM *siItem, SESSIONITEM *siRoot)
{
  if (siItem->firstChild)
    return siItem->firstChild;

  do
  {
    if (siItem->next)
      return siItem->next;
    siItem=siItem->parent;
  }
  while (siItem != siRoot);

  return NULL;
}

SESSIONITEM* StackNextItemNoChild(SESSIONITEM *si)
{
  do
  {
    if (si->next)
      return si->next;
  }
  while (si=si->parent);

  return si;
}

SESSIONITEM* StackPrevItem(SESSIONITEM *siItem, SESSIONITEM *siRoot)
{
  if (siItem->lastChild)
    return siItem->lastChild;

  do
  {
    if (siItem->prev)
      return siItem->prev;
    siItem=siItem->parent;
  }
  while (siItem != siRoot);

  return NULL;
}

SESSIONITEM* StackPrevItemNoChild(SESSIONITEM *si)
{
  do
  {
    if (si->prev)
      return si->prev;
  }
  while (si=si->parent);

  return si;
}

void StackDeleteItem(SESSION *ss, SESSIONITEM *siItem)
{
  SESSIONITEM *lpParent=siItem;
  SESSIONITEM *lpSubling;
  SESSIONITEM *lpNextSubling;

  if (siItem)
    lpSubling=siItem->firstChild;
  else
    lpSubling=ss->hItemsStack.first;

  while (lpSubling)
  {
    NextParent:
    if (lpSubling->firstChild)
    {
      lpSubling=lpSubling->firstChild;
      continue;
    }

    //Group doesn't have childrens
    lpParent=lpSubling->parent;
    lpNextSubling=lpSubling->next;

    //Delete group
    if (lpSubling->wszBookmarks)
      GlobalFree((HGLOBAL)lpSubling->wszBookmarks);
    if (lpSubling->wszCoderAlias)
      GlobalFree((HGLOBAL)lpSubling->wszCoderAlias);
    if (lpSubling->wszCoderFolds)
      GlobalFree((HGLOBAL)lpSubling->wszCoderFolds);
    if (lpSubling->wszCoderMarks)
      GlobalFree((HGLOBAL)lpSubling->wszCoderMarks);

    if (!lpParent)
      StackDelete((stack **)&ss->hItemsStack.first, (stack **)&ss->hItemsStack.last, (stack *)lpSubling);
    else
      StackDelete((stack **)&lpParent->firstChild, (stack **)&lpParent->lastChild, (stack *)lpSubling);
    --ss->hItemsStack.nElements;

    lpSubling=lpNextSubling;
  }
  if (lpParent != siItem)
  {
    lpSubling=lpParent;
    goto NextParent;
  }

  if (siItem)
  {
    if (!siItem->parent)
      StackDelete((stack **)&ss->hItemsStack.first, (stack **)&ss->hItemsStack.last, (stack *)siItem);
    else
      StackDelete((stack **)&siItem->parent->firstChild, (stack **)&siItem->parent->lastChild, (stack *)siItem);
    --ss->hItemsStack.nElements;
  }
}

void StackFreeItems(SESSION *ss)
{
  StackDeleteItem(ss, NULL);
}

void OpenItem(SESSIONITEM *si)
{
  FRAMEDATA *lpFrameActivate=NULL;
  EDITINFO ei;
  OPENDOCUMENTW od;
  CHARRANGE64 cr;
  AECHARINDEX ciChar;
  POINT64 ptGlobal;
  DWORD dwCmdLineOptions;
  int nLockScroll;

  bItemOpening=TRUE;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    if (SendMessage(hMainWnd, AKD_GETMAININFO, MI_SINGLEOPENFILE, 0))
    {
      if (lpFrameActivate=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)si->wszItemExpFile))
      {
        if (lpFrameActivate)
        {
          SendMessage(hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrameActivate);
          //if (lpFrameActivate->ei.bModified)
          //  goto End;
          goto End;
        }
      }
    }

    //Create empty document
    if (!lpFrameActivate)
    {
      if (!ei.hWndEdit || ei.bModified || *ei.wszFile)
        SendMessage(hMainWnd, WM_COMMAND, IDM_FILE_NEW, TRUE);
    }
  }
  else SendMessage(hMainWnd, WM_COMMAND, IDM_FILE_NEW, TRUE);

  //Set edit parameters
  SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei);

  if ((dwSaveData & SSD_WORDWRAP) && si->bWordWrap != ei.bWordWrap)
    SendMessage(hMainWnd, WM_COMMAND, IDM_VIEW_WORDWRAP, 0);
  if ((dwSaveData & SSD_READONLY) && si->bReadOnly != ei.bReadOnly)
    SendMessage(hMainWnd, WM_COMMAND, IDM_VIEW_READONLY, 0);
  if ((dwSaveData & SSD_OVERTYPE) && si->bOvertypeMode != ei.bOvertypeMode)
    SendMessage(hMainWnd, WM_COMMAND, IDM_EDIT_INSERTMODE, 0);

  if (!lpFrameActivate)
  {
    //Open file
    dwCmdLineOptions=(DWORD)SendMessage(hMainWnd, AKD_GETCMDLINEOPTIONS, 0, 0);
    SendMessage(hMainWnd, AKD_SETCMDLINEOPTIONS, CLO_MSGOPENCREATENO|CLO_MSGOPENBINARYNO, 0);

    od.pFile=si->wszItemExpFile;
    od.pWorkDir=NULL;
    od.dwFlags=OD_ADT_DETECTBOM|((!si->nCodePage || !(dwSaveData & SSD_CODEPAGE))?OD_ADT_DETECTCODEPAGE:0)|(dwSaveData & SSD_SELECTION?OD_NOSCROLL:0);
    od.nCodePage=si->nCodePage;
    od.bBOM=0;
    od.hDoc=NULL;
    SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);

    SendMessage(hMainWnd, AKD_SETCMDLINEOPTIONS, dwCmdLineOptions, 0);
  }

  //Set LineBoard plugin bookmarks
  if ((dwSaveData & SSD_BOOKMARKS) && si->wszBookmarks && *si->wszBookmarks)
  {
    PLUGINFUNCTION *pfLineBoard;

    if (pfLineBoard=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"LineBoard::Main", 0))
    {
      if (pfLineBoard->bRunning)
      {
        PLUGINCALLSENDW pcs;
        DLLEXTLINEBOARD delb;

        //Free all bookmarks already set
        delb.dwStructSize=sizeof(DLLEXTLINEBOARD);
        delb.nAction=DLLA_LINEBOARD_DELBOOKMARKS;
        delb.hWndEdit=ei.hWndEdit;
        delb.hDocEdit=ei.hDocEdit;

        pcs.pFunction=L"LineBoard::Main";
        pcs.lParam=(LPARAM)&delb;
        pcs.dwSupport=0;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

        //Set bookmarks
        delb.dwStructSize=sizeof(DLLEXTLINEBOARD);
        delb.nAction=DLLA_LINEBOARD_SETBOOKMARKS;
        delb.hWndEdit=ei.hWndEdit;
        delb.hDocEdit=ei.hDocEdit;
        delb.wszBookmarks=si->wszBookmarks;

        pcs.pFunction=L"LineBoard::Main";
        pcs.lParam=(LPARAM)&delb;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
      }
    }
  }

  //Set Coder plugin alias
  if ((dwSaveData & SSD_CODERALIAS) && si->wszCoderAlias && *si->wszCoderAlias)
  {
    PLUGINFUNCTION *pfCoder;

    if (((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfCoder->bRunning) ||
        ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::CodeFold", 0)) && pfCoder->bRunning) ||
        ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::AutoComplete", 0)) && pfCoder->bRunning))
    {
      PLUGINCALLSENDW pcs;
      DLLEXTCODERSETALIAS decsa;

      decsa.dwStructSize=sizeof(DLLEXTCODERSETALIAS);
      decsa.nAction=DLLA_CODER_SETALIAS;
      decsa.wpAlias=si->wszCoderAlias;

      pcs.pFunction=L"Coder::Settings";
      pcs.lParam=(LPARAM)&decsa;
      pcs.dwSupport=PDS_STRWIDE;
      SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
    }
  }

  //Set Coder plugin folds
  if ((dwSaveData & SSD_CODERFOLDS) && si->wszCoderFolds && *si->wszCoderFolds)
  {
    HSTACK *hFoldsStack;

    if (hFoldsStack=(HSTACK *)SendMessage(ei.hWndEdit, AEM_GETFOLDSTACK, 0, 0))
    {
      if (hFoldsStack->first)
      {
        SetCollapsedFoldsString(ei.hWndEdit, hFoldsStack, si->wszCoderFolds);
      }
    }
  }

  //Set Coder plugin marks
  if ((dwSaveData & SSD_CODERMARKS) && si->wszCoderMarks && *si->wszCoderMarks)
  {
    PLUGINFUNCTION *pfCoder;

    if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfCoder->bRunning)
    {
      PLUGINCALLSENDW pcs;
      DLLEXTHIGHLIGHTMARK dehm;
      const wchar_t *wpText=si->wszCoderMarks;
      const wchar_t *wpParamStart;
      const wchar_t *wpParamEnd;
      wchar_t wszColorText[MAX_PATH];
      wchar_t wszColorBk[MAX_PATH];
      DWORD dwHLFlags;

      //Force send WM_PAINT otherwise StackGetHighLightWindow returns NULL.
      UpdateWindow(ei.hWndEdit);

      dehm.dwStructSize=sizeof(DLLEXTHIGHLIGHTMARK);
      dehm.nAction=DLLA_HIGHLIGHT_MARK;
      dehm.wpColorText=wszColorText;
      dehm.wpColorBk=wszColorBk;

      for (;;)
      {
        if (!GetEscapeParam(wpText, &wpParamStart, &wpParamEnd, &wpText))
          break;
        xstrcpynW(wszColorText, wpParamStart, wpParamEnd - wpParamStart + 1);

        if (!GetEscapeParam(wpText, &wpParamStart, &wpParamEnd, &wpText))
          break;
        xstrcpynW(wszColorBk, wpParamStart, wpParamEnd - wpParamStart + 1);

        if (!GetEscapeParam(wpText, &wpParamStart, &wpParamEnd, &wpText))
          break;
        dwHLFlags=(DWORD)xatoiW(wpParamStart, NULL);
        dehm.dwFlags=0;
        if (dwHLFlags & AEHLF_MATCHCASE)
          dehm.dwFlags|=MARKFLAG_MATCHCASE;
        if (dwHLFlags & AEHLF_REGEXP)
          dehm.dwFlags|=MARKFLAG_REGEXP;
        if (dwHLFlags & AEHLF_WHOLEWORD)
          dehm.dwFlags|=MARKFLAG_WHOLEWORD;

        if (!GetEscapeParam(wpText, &wpParamStart, &wpParamEnd, &wpText))
          break;
        dehm.dwFontStyle=(DWORD)xatoiW(wpParamStart, NULL);

        if (!GetEscapeParam(wpText, &wpParamStart, &wpParamEnd, &wpText))
          break;
        dehm.dwMarkID=(DWORD)xatoiW(wpParamStart, NULL);

        if (!GetEscapeParam(wpText, &wpParamStart, &wpParamEnd, &wpText))
          break;
        if (*wpParamStart == L'\"') ++wpParamStart;
        if (*(wpParamEnd - 1) == L'\"') --wpParamEnd;

        if (dehm.wpMarkText=(wchar_t *)GlobalAlloc(GPTR, (wpParamEnd - wpParamStart + 1) * sizeof(wchar_t)))
        {
          dehm.nMarkTextLen=UnescapeString(wpParamStart, wpParamEnd - wpParamStart, dehm.wpMarkText);
        }

        pcs.pFunction=L"Coder::HighLight";
        pcs.lParam=(LPARAM)&dehm;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

        if (dehm.wpMarkText)
          GlobalFree((HGLOBAL)dehm.wpMarkText);
        if (*wpText == L')' && *++wpText == L',' && *++wpText == L'(')
          continue;
        break;
      }
    }
  }

  //Set selection
  if (!lpFrameActivate)
  {
    if (dwSaveData & SSD_SELECTION)
    {
      if ((nLockScroll=(int)SendMessage(ei.hWndEdit, AEM_LOCKSCROLL, (WPARAM)-1, 0)) == -1)
        SendMessage(ei.hWndEdit, AEM_LOCKSCROLL, SB_BOTH, TRUE);
      cr.cpMin=si->nSelStart;
      cr.cpMax=si->nSelEnd;
      SendMessage(ei.hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
      if (nLockScroll == -1)
        SendMessage(ei.hWndEdit, AEM_LOCKSCROLL, SB_BOTH, FALSE);

      SendMessage(ei.hWndEdit, AEM_RICHOFFSETTOINDEX, (WPARAM)si->nFirstVisChar, (LPARAM)&ciChar);
      SendMessage(ei.hWndEdit, AEM_GLOBALPOSFROMCHAR, (WPARAM)&ptGlobal, (LPARAM)&ciChar);
      SendMessage(ei.hWndEdit, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobal);
    }
  }

  End:
  bItemOpening=FALSE;
}

BOOL CloseItem(FRAMEDATA *lpFrame, BOOL bSingle)
{
  BOOL bResult=TRUE;

  if (lpFrame)
  {
    SendMessage(hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrame);
    bResult=(BOOL)SendMessage(hMainWnd, WM_COMMAND, IDM_WINDOW_FRAMECLOSE, 0);
  }
  return bResult;
}

BOOL SelectItem(HWND hWnd, SESSIONITEM *si, BOOL bSelect)
{
  TVITEMW tvi;
  BOOL bResult;

  tvi.mask=TVIF_STATE;
  tvi.state=bSelect?TVIS_SELECTED:0;
  tvi.stateMask=TVIS_SELECTED;
  tvi.hItem=si->hItem;
  bResult=TreeView_SetItemWide(hWnd, &tvi);

  if (bSelect)
    si->dwState|=TVIS_SELECTED;
  else
    si->dwState&=~TVIS_SELECTED;
  return bResult;
}

void ComboFillSessions(STACKSESSION *hStack, HWND hWnd)
{
  SESSION *ss;

  for (ss=hStack->first; ss; ss=ss->next)
  {
    ComboBox_AddStringWide(hWnd, ss->wszSessionName);
  }
}

int MoveComboBoxItem(HWND hWnd, int nOldIndex, int nNewIndex)
{
  wchar_t *wpText;
  int nIndex=CB_ERR;
  int nTextLen;

  if ((nTextLen=(int)SendMessage(hWnd, CB_GETLBTEXTLEN, nOldIndex, 0)) != CB_ERR)
  {
    if (wpText=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nTextLen + 1) * sizeof(wchar_t)))
    {
      ComboBox_GetLBTextWide(hWnd, nOldIndex, wpText);
      SendMessage(hWnd, CB_DELETESTRING, nOldIndex, 0);
      nIndex=ComboBox_InsertStringWide(hWnd, nNewIndex, wpText);
      GlobalFree((HGLOBAL)wpText);
    }
  }
  return nIndex;
}

int UpdateComboBoxDropWidth(HWND hWnd)
{
  wchar_t *wpText;
  HDC hDC;
  HFONT hFont;
  HFONT hOldFont;
  SIZE size;
  int nTextLen;
  int nMaxWidth=0;
  int nIndex=0;

  if (hDC=GetDC(hWnd))
  {
    hFont=(HFONT)SendMessage(hWnd, WM_GETFONT, 0,0);
    hOldFont=(HFONT)SelectObject(hDC, hFont);

    for (;;)
    {
      if ((nTextLen=(int)SendMessage(hWnd, CB_GETLBTEXTLEN, nIndex, 0)) != CB_ERR)
      {
        if (wpText=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nTextLen + 1) * sizeof(wchar_t)))
        {
          ComboBox_GetLBTextWide(hWnd, nIndex, wpText);
          if (GetTextExtentPoint32W(hDC, wpText, nTextLen, &size))
            nMaxWidth=max(size.cx, nMaxWidth);
          GlobalFree((HGLOBAL)wpText);
        }
        else break;
      }
      else break;

      ++nIndex;
    }

    if (hOldFont) SelectObject(hDC, hOldFont);
    ReleaseDC(hWnd, hDC);
  }
  SendMessage(hWnd, CB_SETDROPPEDWIDTH, nMaxWidth + 10, 0);
  return nMaxWidth;
}

void TreeFillItemsCurrent(HWND hWndTreeView)
{
  TVINSERTSTRUCTW tvis;
  TVITEMW tvi;
  TCITEMW tcItem;
  FRAMEDATA *lpFrame;
  int nTabItem=0;

  for (;;)
  {
    tcItem.mask=TCIF_PARAM;
    if (!TabCtrl_GetItemWide(hTab, nTabItem++, &tcItem))
      break;
    lpFrame=(FRAMEDATA *)tcItem.lParam;

    if (*lpFrame->ei.wszFile)
    {
      tvi.mask=TVIF_TEXT|TVIF_PARAM;
      tvi.pszText=(wchar_t *)(bShowPath?lpFrame->ei.wszFile:GetFileName(lpFrame->ei.wszFile, -1));
      tvi.cchTextMax=MAX_PATH;
      tvi.lParam=(LPARAM)lpFrame;

      xmemcpy(&tvis.item, &tvi, sizeof(TVITEMW));
      tvis.hParent=NULL;
      tvis.hInsertAfter=TVI_LAST;
      TreeView_InsertItemWide(hWndTreeView, &tvis);
    }
  }
}

void TreeAddItem(HWND hWndTreeView, SESSIONITEM *si, HTREEITEM hInsertAfter, DWORD dwState)
{
  TVINSERTSTRUCTW tvis;

  tvis.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_STATE|TVIF_PARAM;
  tvis.item.hItem=NULL;
  tvis.item.state=dwState;
  tvis.item.stateMask=dwState;
  if (bShowPath && !si->dwFolderFlags)
    tvis.item.pszText=si->wszItemFile;
  else
    tvis.item.pszText=si->wszName;
  tvis.item.cchTextMax=MAX_PATH;
  if (!si->dwFolderFlags)
  {
    tvis.item.iImage=0;
    tvis.item.iSelectedImage=0;
  }
  else
  {
    tvis.item.iImage=1;
    tvis.item.iSelectedImage=1;
  }
  tvis.item.cChildren=0;
  tvis.item.lParam=(LPARAM)si;

  tvis.hParent=si->parent?si->parent->hItem:NULL;
  tvis.hInsertAfter=hInsertAfter;
  si->hItem=(HTREEITEM)TreeView_InsertItemWide(hWndTreeView, &tvis);
}

void TreeAddChild(HWND hWndTreeView, SESSION *ss, SESSIONITEM *si)
{
  SESSIONITEM *siCount;
  SESSIONITEM *siFirst;

  if (!si)
    siFirst=ss->hItemsStack.first;
  else
    siFirst=si->firstChild;

  for (siCount=siFirst; siCount; siCount=StackNextItem(siCount, si))
  {
    TreeAddItem(hWndTreeView, siCount, TVI_LAST, siCount->dwState);
  }
  for (siCount=siFirst; siCount; siCount=StackNextItem(siCount, si))
  {
    if ((siCount->dwFolderFlags & FLDF_OPEN) && siCount->firstChild)
      SendMessage(hWndTreeView, TVM_EXPAND, TVE_EXPAND, (LPARAM)siCount->hItem);
  }
}

BOOL TreeMoveItem(HWND hWndTreeView, HTREEITEM hItemCaret, SESSION *ss, SESSIONITEM *siItem, SESSIONITEM *siParent, SESSIONITEM *siAfterThis)
{
  HTREEITEM hItemDelete=siItem->hItem;
  SESSIONITEM *siBeforeThis;
  SESSIONITEM **siFirstSrc;
  SESSIONITEM **siLastSrc;
  SESSIONITEM **siFirstDst;
  SESSIONITEM **siLastDst;
  SESSIONITEM *siCount;

  if (siItem == siParent)
    return FALSE;
  if (!siItem->parent)
  {
    siFirstSrc=&ss->hItemsStack.first;
    siLastSrc=&ss->hItemsStack.last;
  }
  else
  {
    siFirstSrc=&siItem->parent->firstChild;
    siLastSrc=&siItem->parent->lastChild;
  }
  if (!siParent)
  {
    siFirstDst=&ss->hItemsStack.first;
    siLastDst=&ss->hItemsStack.last;
  }
  else
  {
    siFirstDst=&siParent->firstChild;
    siLastDst=&siParent->lastChild;
  }
  if (siAfterThis == NULL || siAfterThis == ASI_FIRST)
  {
    siAfterThis=NULL;
    siBeforeThis=*siFirstDst;
  }
  else if (siAfterThis == ASI_LAST)
  {
    siAfterThis=*siLastDst;
    siBeforeThis=NULL;
  }
  else siBeforeThis=siAfterThis->next;

  //Item need to be moved?
  if (*siFirstSrc == *siFirstDst && *siLastSrc == *siLastDst &&
      (siBeforeThis == siItem || siBeforeThis == siItem->next))
    return FALSE;

  //Check that siAfterThis not child for siItem
  for (siCount=siAfterThis; siCount; siCount=siCount->parent)
  {
    if (siCount == siItem)
      return FALSE;
  }

  //Move in stacks
  StackSplit((stack **)siFirstSrc, (stack **)siLastSrc, (stack *)siItem, (stack *)siItem);
  StackJoin((stack **)siFirstDst, (stack **)siLastDst, (stack *)siBeforeThis, (stack *)siItem, (stack *)siItem);
  siItem->parent=siParent;

  //Move in control
  TreeAddItem(hWndTreeView, siItem, siAfterThis?siAfterThis->hItem:TVI_FIRST, siItem->dwState);
  TreeAddChild(hWndTreeView, ss, siItem);
  if ((siItem->dwFolderFlags & FLDF_OPEN) && siItem->firstChild)
    SendMessage(hWndTreeView, TVM_EXPAND, TVE_EXPAND, (LPARAM)siItem->hItem);
  if (hItemCaret == hItemDelete)
    SendMessage(hWndTreeView, TVM_SELECTITEM, TVGN_CARET, (LPARAM)siItem->hItem);
  TreeDeleteItem(hWndTreeView, hItemDelete);
  return TRUE;
}

BOOL TreeDeleteItem(HWND hWndTreeView, HTREEITEM hItem)
{
  BOOL bResult;

  //bDeleting=TRUE;
  bResult=(BOOL)SendMessage(hWndTreeView, TVM_DELETEITEM, 0, (LPARAM)hItem);
  //bDeleting=FALSE;
  return bResult;
}

void TreeUpdateItem(HWND hWndTreeView, HTREEITEM hItem)
{
  RECT rcItem;

  *(HTREEITEM *)&rcItem=hItem;
  SendMessage(hWndTreeView, TVM_GETITEMRECT, TRUE, (LPARAM)&rcItem);
  InvalidateRect(hWndTreeView, &rcItem, FALSE);
}

LPARAM TreeItemParam(HWND hWndTreeView, HTREEITEM hItem)
{
  TVITEMW tvi;

  tvi.mask=TVIF_PARAM;
  if (!hItem)
    tvi.hItem=(HTREEITEM)SendMessage(hWndTreeView, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
  else
    tvi.hItem=hItem;
  if (TreeView_GetItemWide(hWndTreeView, &tvi))
    return tvi.lParam;
  return 0;
}

HTREEITEM TreeCursorItem(HWND hWndTreeView)
{
  TVHITTESTINFO tvhti;

  GetCursorPos(&tvhti.pt);
  ScreenToClient(hWndTreeView, &tvhti.pt);
  SendMessage(hWndTreeView, TVM_HITTEST, 0, (LPARAM)&tvhti);
  if (tvhti.hItem)
  {
    if (tvhti.flags & (TVHT_ONITEMLABEL|TVHT_ONITEMICON))
      return tvhti.hItem;
  }
  return NULL;
}

void ClearTreeView(HWND hWndTreeView, BOOL bRedraw)
{
  SendMessage(hWndTreeView, WM_SETREDRAW, FALSE, 0);
  TreeDeleteItem(hWndTreeView, NULL);
  SendMessage(hWndTreeView, WM_SETREDRAW, TRUE, 0);
  if (bRedraw) InvalidateRect(hWndTreeView, NULL, TRUE);
}

void GetCoderColors(HWND hWnd)
{
  int i;

  //Default colors
  for (i=0; cti[i].wpVarName; ++i)
  {
    *(COLORREF *)cti[i].nVarValue=(COLORREF)-1;
  }

  if (bCoderTheme)
  {
    PLUGINFUNCTION *pfCoder;
    PLUGINCALLSENDW pcs;
    DLLEXTCODERFILLVARLIST decfvl;

    if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfCoder->bRunning)
    {
      decfvl.dwStructSize=sizeof(DLLEXTCODERFILLVARLIST);
      decfvl.nAction=DLLA_CODER_FILLVARLIST;
      decfvl.hWndEdit=hWnd;
      decfvl.hDocEdit=NULL;
      decfvl.cti=cti;

      pcs.pFunction=L"Coder::Settings";
      pcs.lParam=(LPARAM)&decfvl;
      pcs.dwSupport=PDS_STRWIDE;
      SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
    }
  }
}

void SetListColors(HWND hWndEdit, HWND hWndList)
{
  if (bCoderTheme && hWndList)
  {
    if (!hWndEdit) hWndEdit=(HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, 0);
    GetCoderColors(hWndEdit);
    if ((DWORD)SendMessage(hWndList, TVM_GETTEXTCOLOR, 0, 0) != crListTextColor)
      SendMessage(hWndList, TVM_SETTEXTCOLOR, 0, (LPARAM)crListTextColor);
    if ((DWORD)SendMessage(hWndList, TVM_GETBKCOLOR, 0, 0) != crListBkColor)
      SendMessage(hWndList, TVM_SETBKCOLOR, 0, (LPARAM)crListBkColor);
  }
}

int GetCollapsedFoldsString(HSTACK *hFoldsStack, wchar_t *wszString)
{
  AEFOLD *lpFold;
  int nFoldCount=0;
  int nSize=0;

  for (lpFold=(AEFOLD *)hFoldsStack->first; lpFold; lpFold=AEC_NextFold(lpFold, !(lpFold->dwFlags & AEFOLDF_COLLAPSED)))
  {
    ++nFoldCount;

    if (lpFold->dwFlags & AEFOLDF_COLLAPSED)
    {
      nSize+=xitoaW(nFoldCount, wszString?wszString + nSize:NULL);

      if (wszString) *(wszString + nSize)=L',';
      ++nSize;
    }
  }
  if (nSize) --nSize;

  if (wszString)
  {
    *(wszString + nSize)=L'\0';
    return nSize;
  }
  return nSize + 1;
}

int SetCollapsedFoldsString(HWND hWnd, HSTACK *hFoldsStack, const wchar_t *wpString)
{
  AEFOLD *lpFold;
  const wchar_t *wpCount=wpString;
  int nFoldCount=0;
  int nFoldNumber=0;
  int nCollapsed=0;

  for (lpFold=(AEFOLD *)hFoldsStack->first; lpFold; lpFold=AEC_NextFold(lpFold, !(lpFold->dwFlags & AEFOLDF_COLLAPSED)))
  {
    ++nFoldCount;

    if (nFoldNumber < nFoldCount)
    {
      if (*wpCount >= L'0' && *wpCount <= L'9')
      {
        nFoldNumber=(int)xatoiW(wpCount, &wpCount);
        if (*wpCount == L',')
          ++wpCount;
      }
      else break;
    }
    if (nFoldNumber == nFoldCount)
    {
      SendMessage(hWnd, AEM_COLLAPSEFOLD, (WPARAM)lpFold, AECF_COLLAPSE|AECF_NOUPDATE|AECF_NOCARETCORRECT);
      ++nCollapsed;
    }
  }
  if (nCollapsed)
    SendMessage(hWnd, AEM_UPDATEFOLD, 0, (LPARAM)-1);
  return (int)(wpCount - wpString);
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

INT_PTR EscapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput)
{
  //Escape: \ -> \\ and " -> \"
  const wchar_t *wpInputMax=wpInput + nInputLen;
  wchar_t *wpOutput=wszOutput;

  for (; wpInput < wpInputMax; ++wpInput)
  {
    if (*wpInput == L'\\')
    {
      if (wszOutput)
      {
        *wpOutput++=L'\\';
        *wpOutput++=L'\\';
      }
      else wpOutput+=2;
    }
    else if (*wpInput == L'\"')
    {
      if (wszOutput)
      {
        *wpOutput++=L'\\';
        *wpOutput++=L'\"';
      }
      else wpOutput+=2;
    }
    else
    {
      if (wszOutput)
        *wpOutput++=*wpInput;
      else
        wpOutput+=1;
    }
  }
  return (wpOutput - wszOutput);
}

INT_PTR UnescapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput)
{
  //Unescape: \\ -> \ and \" -> "
  const wchar_t *wpInputMax=wpInput + nInputLen;
  wchar_t *wpOutput=wszOutput;

  for (; wpInput < wpInputMax; ++wpInput)
  {
    if (*wpInput == L'\\')
      ++wpInput;

    if (wszOutput)
      *wpOutput++=*wpInput;
    else
      wpOutput+=1;
  }
  return (wpOutput - wszOutput);
}

INT_PTR GetEscapeParam(const wchar_t *wpText, const wchar_t **wpParamStart, const wchar_t **wpParamEnd, const wchar_t **wpTextNext)
{
  if (*wpText == L'(')
    ++wpText;
  *wpParamStart=wpText;

  for (; *wpText != L')' && *wpText; ++wpText)
  {
    if (*wpText == L'\"')
    {
      while (*++wpText != L'\"')
      {
        if (*wpText == L'\\') ++wpText;
      }
    }
    else if (*wpText == L',')
      break;
  }
  *wpParamEnd=wpText;

  if (*wpText == L',')
    *wpTextNext=wpText + 1;
  else
    *wpTextNext=wpText;
  return *wpParamEnd - *wpParamStart;
}

BOOL NextLine(const wchar_t **wpText)
{
  while (**wpText != L'\r' && **wpText != L'\n' && **wpText != L'\0') ++*wpText;
  if (**wpText == L'\0') return FALSE;
  if (**wpText == L'\r') ++*wpText;
  if (**wpText == L'\n') ++*wpText;
  return TRUE;
}

void SkipSpaces(const wchar_t **wppText)
{
  while (**wppText == L' ' || **wppText == L'\t' || **wppText == L'\r' || **wppText == L'\n') ++*wppText;
}

BOOL IsPathFull(const wchar_t *wpPath)
{
  if (wpPath[0] == L'\\' && wpPath[1] == L'\\') return TRUE;
  if (wpPath[0] != L'\0' && wpPath[1] == L':') return TRUE;
  return FALSE;
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

BOOL CreateDirectoryRecursive(const wchar_t *wpPath)
{
  wchar_t wszPath[MAX_PATH];
  wchar_t *wpPathEnd;
  wchar_t *wpCount;

  xstrcpynW(wszPath, wpPath, MAX_PATH);
  wpPathEnd=wszPath + xstrlenW(wszPath);
  wpCount=wpPathEnd;

  Create:
  if (!CreateDirectoryWide(wszPath, NULL))
  {
    if (GetLastError() == ERROR_PATH_NOT_FOUND)
    {
      for (; wpCount > wszPath; --wpCount)
      {
        if (*wpCount == L'\\')
        {
          *wpCount=L'\0';
          goto Create;
        }
      }
    }
    else if (GetLastError() == ERROR_ALREADY_EXISTS)
      return TRUE;
    return FALSE;
  }
  while (wpCount < wpPathEnd)
  {
    for (*wpCount=L'\\'; wpCount < wpPathEnd; ++wpCount)
    {
      if (*wpCount == L'\0')
        break;
    }
    if (!CreateDirectoryWide(wszPath, NULL))
      return FALSE;
  }
  return TRUE;
}

void DropFiles(HDROP hDrop, HWND hWndItemsList)
{
  HTREEITEM hItemCursor;
  HTREEITEM hItemAfterThis=TVI_LAST;
  SESSIONITEM *siNew=NULL;
  SESSIONITEM *siParent=NULL;
  SESSIONITEM *siAfterThis;
  wchar_t wszFile[MAX_PATH];
  BOOL bFirstFile=TRUE;
  int nDropped;
  int i;

  hItemCursor=TreeCursorItem(hWndItemsList);

  if (siAfterThis=(SESSIONITEM *)TreeItemParam(hWndItemsList, hItemCursor))
  {
    if (!siAfterThis->dwFolderFlags)
    {
      siParent=siAfterThis->parent;
      hItemAfterThis=siAfterThis->hItem;
    }
    else
    {
      siParent=siAfterThis;
      siAfterThis=ASI_FIRST;
      hItemAfterThis=TVI_FIRST;
    }
  }
  else siAfterThis=ASI_LAST;

  nDropped=DragQueryFileWide(hDrop, 0xFFFFFFFF, NULL, 0);

  for (i=0; i < nDropped; ++i)
  {
    DragQueryFileWide(hDrop, i, wszFile, MAX_PATH);

    if (IsFile(wszFile) == ERROR_SUCCESS)
    {
      if (lpVirtualSession)
      {
        if (siNew=StackAddItem(lpVirtualSession, siParent, siAfterThis))
        {
          xstrcpynW(siNew->wszName, GetFileName(wszFile, -1), MAX_PATH);
          xstrcpynW(siNew->wszItemFile, wszFile, MAX_PATH);
          xstrcpynW(siNew->wszItemExpFile, wszFile, MAX_PATH);
          siNew->dwState=TVIS_SELECTED;
          TreeAddItem(hWndItemsList, siNew, hItemAfterThis, siNew->dwState);
          siAfterThis=siNew;
          hItemAfterThis=siNew->hItem;
          lpVirtualSession->bModified=TRUE;
          if (bFirstFile)
          {
            //Reset selection
            SendMessage(hWndItemsList, TVM_SELECTITEM, TVGN_CARET, (LPARAM)siNew->hItem);
            bFirstFile=FALSE;
          }
        }
      }
    }
    if (nMDI == WMD_SDI) break;
  }
  DragFinish(hDrop);

  PostMessage(hWndMainDlg, AKDLL_UPDATESAVEBUTTON, 0, 0);
}

int IsFile(const wchar_t *wpFile)
{
  DWORD dwAttr;

  dwAttr=GetFileAttributesWide(wpFile);
  if (dwAttr == INVALID_FILE_ATTRIBUTES)
    return ERROR_INVALID_HANDLE;
  if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
    return ERROR_DIRECTORY;
  return ERROR_SUCCESS;
}

LRESULT SendToDoc(AEHDOC hDocEdit, HWND hWndEdit, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AESENDMESSAGE sm;

  sm.hDoc=hDocEdit;
  sm.uMsg=uMsg;
  sm.wParam=wParam;
  sm.lParam=lParam;
  if (SendMessage(hWndEdit, AEM_SENDMESSAGE, 0, (LPARAM)&sm))
    return sm.lResult;
  return 0;
}

COLORREF AE_ColorBrightness(COLORREF crColor, int nPercent)
{
  //From -100% to 0% - to dark, from 0% to 100% - to light
  BYTE r=GetRValue(crColor);
  BYTE g=GetGValue(crColor);
  BYTE b=GetBValue(crColor);

  if  (nPercent > 0 && nPercent <= 100)
  {
    r=(BYTE)(r + (nPercent * (255 - r) / 100));
    g=(BYTE)(g + (nPercent * (255 - g) / 100));
    b=(BYTE)(b + (nPercent * (255 - b) / 100));
  }
  else if (nPercent >= -100 && nPercent < 0)
  {
    r=(BYTE)(r + (nPercent * r / 100));
    g=(BYTE)(g + (nPercent * g / 100));
    b=(BYTE)(b + (nPercent * b / 100));
  }
  return RGB(r, g, b);
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
    WideOption(hOptions, L"SaveSessions", PO_DWORD, (LPBYTE)&nSaveSessions, sizeof(DWORD));
    WideOption(hOptions, L"OpenOnStartEnable", PO_DWORD, (LPBYTE)&bOpenOnStart, sizeof(DWORD));
    WideOption(hOptions, L"OpenOnStartSession", PO_STRING, (LPBYTE)wszOpenOnStart, sizeof(wszOpenOnStart));
    WideOption(hOptions, L"SaveOnExitEnable", PO_DWORD, (LPBYTE)&bSaveOnExit, sizeof(DWORD));
    WideOption(hOptions, L"SaveOnExitSession", PO_STRING, (LPBYTE)wszSaveOnExit, sizeof(wszSaveOnExit));
    WideOption(hOptions, L"ShowPath", PO_DWORD, (LPBYTE)&bShowPath, sizeof(DWORD));
    WideOption(hOptions, L"SaveRelative", PO_DWORD, (LPBYTE)&bSaveRelative, sizeof(DWORD));
    WideOption(hOptions, L"ListSystemFont", PO_DWORD, (LPBYTE)&bListSystemFont, sizeof(DWORD));
    WideOption(hOptions, L"CoderTheme", PO_DWORD, (LPBYTE)&bCoderTheme, sizeof(DWORD));
    WideOption(hOptions, L"SaveData", PO_DWORD, (LPBYTE)&dwSaveData, sizeof(DWORD));
    WideOption(hOptions, L"DockAutoload", PO_DWORD, (LPBYTE)&bDockAutoload, sizeof(DWORD));
    WideOption(hOptions, L"DialogType", PO_DWORD, (LPBYTE)&nDialogType, sizeof(DWORD));

    if (nDialogType == DLGT_DOCKABLE)
    {
      WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&nSessionsDockSide, sizeof(DWORD));
      WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&rcSessionsDockRect, sizeof(RECT));
    }
    else WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_SETTINGS)
    {
      WideOption(hOptions, L"SaveSessions", PO_DWORD, (LPBYTE)&nSaveSessions, sizeof(DWORD));
      WideOption(hOptions, L"OpenOnStartEnable", PO_DWORD, (LPBYTE)&bOpenOnStart, sizeof(DWORD));
      WideOption(hOptions, L"OpenOnStartSession", PO_STRING, (LPBYTE)wszOpenOnStart, ((int)xstrlenW(wszOpenOnStart) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"SaveOnExitEnable", PO_DWORD, (LPBYTE)&bSaveOnExit, sizeof(DWORD));
      WideOption(hOptions, L"SaveOnExitSession", PO_STRING, (LPBYTE)wszSaveOnExit, ((int)xstrlenW(wszSaveOnExit) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"ShowPath", PO_DWORD, (LPBYTE)&bShowPath, sizeof(DWORD));
      WideOption(hOptions, L"SaveRelative", PO_DWORD, (LPBYTE)&bSaveRelative, sizeof(DWORD));
      WideOption(hOptions, L"ListSystemFont", PO_DWORD, (LPBYTE)&bListSystemFont, sizeof(DWORD));
      WideOption(hOptions, L"CoderTheme", PO_DWORD, (LPBYTE)&bCoderTheme, sizeof(DWORD));
      WideOption(hOptions, L"SaveData", PO_DWORD, (LPBYTE)&dwSaveData, sizeof(DWORD));
      WideOption(hOptions, L"DockAutoload", PO_DWORD, (LPBYTE)&bDockAutoload, sizeof(DWORD));
      if (nNewDialogType)
        WideOption(hOptions, L"DialogType", PO_DWORD, (LPBYTE)&nNewDialogType, sizeof(DWORD));
    }

    if (dwFlags & OF_RECT)
    {
      if (nDialogType == DLGT_DOCKABLE)
      {
        WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&dkSessionsDlg->nSide, sizeof(DWORD));
        WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&dkSessionsDlg->rcSize, sizeof(RECT));
      }
      else WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
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
    if (nStringID == STRID_PARSEMSG_NOOPENBRACKET)
      return L"\x041D\x0435\x0442\x0020\x043E\x0442\x043A\x0440\x044B\x0432\x0430\x044E\x0449\x0435\x0439\x0020\x0441\x043A\x043E\x0431\x043A\x0438\x002E";
    if (nStringID == STRID_PARSEMSG_NOCLOSEBRACKET)
      return L"\x041D\x0435\x0442\x0020\x0437\x0430\x043A\x0440\x044B\x0432\x0430\x044E\x0449\x0435\x0439\x0020\x0441\x043A\x043E\x0431\x043A\x0438\x002E";
    if (nStringID == STRID_SESSION)
      return L"\x0421\x0435\x0441\x0441\x0438\x044F";
    if (nStringID == STRID_OPEN)
      return L"\x041E\x0442\x043A\x0440\x044B\x0442\x044C";
    if (nStringID == STRID_UPDATE)
      return L"\x041E\x0431\x043D\x043E\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_SAVE)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x0438\x0442\x044C";
    if (nStringID == STRID_EDIT)
      return L"\x0420\x0435\x0434\x0430\x043A\x0442\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_COPY)
      return L"\x0421\x043A\x043E\x043F\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_RENAME)
      return L"\x041F\x0435\x0440\x0435\x0438\x043C\x0435\x043D\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_DELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_ADDDIR)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C\x0020\x043F\x0430\x043F\x043A\x0443";
    if (nStringID == STRID_ITEMNAME)
      return L"\x0418\x043C\x044F:";
    if (nStringID == STRID_ITEMFILE)
      return L"\x0424\x0430\x0439\x043B:";
    if (nStringID == STRID_MENU_OPEN)
      return L"\x041E\x0442\x043A\x0440\x044B\x0442\x044C\tEnter";
    if (nStringID == STRID_MENU_CLOSE)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C\tCtrl+W";
    if (nStringID == STRID_MENU_ACTIVATE)
      return L"\x0410\x043A\x0442\x0438\x0432\x0438\x0440\x043E\x0432\x0430\x0442\x044C\tEnter";
    if (nStringID == STRID_MENU_ADDCURFILE)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C\x0020\x0442\x0435\x043A\x0443\x0449\x0438\x0439\x0020\x0444\x0430\x0439\x043B";
    if (nStringID == STRID_MENU_ADDFILE)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C\x0020\x0444\x0430\x0439\x043B\x044B...\tInsert";
    if (nStringID == STRID_MENU_ADDDIR)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C\x0020\x043F\x0430\x043F\x043A\x0443...\tF7";
    if (nStringID == STRID_MENU_RENAME)
      return L"\x041F\x0435\x0440\x0435\x0438\x043C\x0435\x043D\x043E\x0432\x0430\x0442\x044C...\tF2";
    if (nStringID == STRID_MENU_MOVEDOWN)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x043D\x0438\x0437\tAlt+Down";
    if (nStringID == STRID_MENU_MOVEUP)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x0432\x0435\x0440\x0445\tAlt+Up";
    if (nStringID == STRID_MENU_DELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\tDelete";
    if (nStringID == STRID_MENU_DELETEOLD)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x043D\x0435\x0441\x0443\x0449\x0435\x0441\x0442\x0432\x0443\x044E\x0449\x0438\x0435";
    if (nStringID == STRID_NEWNAME)
      return L"\x041D\x043E\x0432\x043E\x0435\x0020\x0438\x043C\x044F\x003A";
    if (nStringID == STRID_SETTINGS)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_SAVESESSIONS)
      return L"\x0425\x0440\x0430\x043D\x0435\x043D\x0438\x0435\x0020\x0441\x0435\x0441\x0441\x0438\x0439";
    if (nStringID == STRID_PROGRAMDIR)
      return L"\x0414\x0438\x0440\x0435\x043A\x0442\x043E\x0440\x0438\x044F\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x044B";
    if (nStringID == STRID_APPDATA)
      return L"\x0414\x0438\x0440\x0435\x043A\x0442\x043E\x0440\x0438\x044F\x0020\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x0435\x043B\x044F";
    if (nStringID == STRID_OPENONSTART)
      return L"\x041E\x0442\x043A\x0440\x044B\x0442\x044C\x0020\x043F\x0440\x0438\x0020\x0437\x0430\x043F\x0443\x0441\x043A\x0435\x003A";
    if (nStringID == STRID_SAVEONEXIT)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x0438\x0442\x044C\x0020\x043D\x0430\x0020\x0432\x044B\x0445\x043E\x0434\x0435\x003A";
    if (nStringID == STRID_SHOWPATH)
      return L"\x041F\x043E\x043A\x0430\x0437\x044B\x0432\x0430\x0442\x044C\x0020\x043F\x0443\x0442\x044C\x0020\x0444\x0430\x0439\x043B\x043E\x0432";
    if (nStringID == STRID_SAVERELATIVE)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x043F\x0443\x0442\x044C\x0020\x0041\x006B\x0065\x006C\x0050\x0061\x0064\x0027\x0430\x0020\x043A\x0430\x043A\x0020\x0025\x0061";
    if (nStringID == STRID_SYSTEMFONT)
      return L"\x0421\x0438\x0441\x0442\x0435\x043C\x043D\x044B\x0439\x0020\x0448\x0440\x0438\x0444\x0442\x0020\x0432\x0020\x0441\x043F\x0438\x0441\x043A\x0435";
    if (nStringID == STRID_CODERTHEME)
      return L"\x0418\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x044C\x0020\x0442\x0435\x043C\x044B Coder";
    if (nStringID == STRID_DIALOGTYPE)
      return L"\x0422\x0438\x043F\x0020\x0434\x0438\x0430\x043B\x043E\x0433\x0430";
    if (nStringID == STRID_MODALDIALOG)
      return L"\x041C\x043E\x0434\x0430\x043B\x044C\x043D\x044B\x0439\x0020\x0434\x0438\x0430\x043B\x043E\x0433";
    if (nStringID == STRID_MODELESSDIALOG)
      return L"\x041D\x0435\x043C\x043E\x0434\x0430\x043B\x044C\x043D\x044B\x0439\x0020\x0434\x0438\x0430\x043B\x043E\x0433";
    if (nStringID == STRID_DOCKABLEDIALOG)
      return L"\x041F\x0440\x0438\x043A\x0440\x0435\x043F\x043B\x0435\x043D\x043D\x044B\x0439\x0020\x0434\x0438\x0430\x043B\x043E\x0433";
    if (nStringID == STRID_AUTOLOAD)
      return L"\x0410\x0432\x0442\x043E\x0437\x0430\x0433\x0440\x0443\x0437\x043A\x0430";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_SAVEDATA)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x0434\x0430\x043D\x043D\x044B\x0435";
    if (nStringID == STRID_SAVEACTIVE)
      return L"\x0410\x043A\x0442\x0438\x0432\x043D\x044B\x0439\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442";
    if (nStringID == STRID_SAVECODEPAGE)
      return L"\x041A\x043E\x0434\x0438\x0440\x043E\x0432\x043A\x0430";
    if (nStringID == STRID_SAVESELECTION)
      return L"\x0412\x044B\x0434\x0435\x043B\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_SAVEWORDWRAP)
      return L"\x041F\x0435\x0440\x0435\x043D\x043E\x0441\x0020\x043F\x043E\x0020\x0441\x043B\x043E\x0432\x0430\x043C";
    if (nStringID == STRID_SAVEREADONLY)
      return L"\x0422\x043E\x043B\x044C\x043A\x043E\x0020\x0447\x0442\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_SAVEOVERTYPE)
      return L"\x0420\x0435\x0436\x0438\x043C\x0020\x0432\x0441\x0442\x0430\x0432\x043A\x0438";
    if (nStringID == STRID_SAVEBOOKMARKS)
      return L"\x0417\x0430\x043A\x043B\x0430\x0434\x043A\x0438 LineBoard";
    if (nStringID == STRID_SAVEALIAS)
      return L"\x041F\x0441\x0435\x0432\x0434\x043E\x043D\x0438\x043C\x0020\x0444\x0430\x0439\x043B\x0430 Coder";
    if (nStringID == STRID_SAVEFOLDS)
      return L"\x0421\x0432\x0435\x0440\x043D\x0443\x0442\x044B\x0435\x0020\x0431\x043B\x043E\x043A\x0438 Coder";
    if (nStringID == STRID_SAVEMARKS)
      return L"\x041E\x0442\x043C\x0435\x0442\x043A\x0438\x0020\x0442\x0435\x043A\x0441\x0442\x0430 Coder";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
    if (nStringID == STRID_CLOSE)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C";
    if (nStringID == STRID_CURRENTSESSION)
      return L"\x003C\x0422\x0435\x043A\x0443\x0449\x0430\x044F\x003E";
    if (nStringID == STRID_ALREADY_EXIST)
      return L"\x0421\x0435\x0441\x0441\x0438\x044F\x0020\x0022\x0025\x0073\x0022\x0020\x0443\x0436\x0435\x0020\x0441\x0443\x0449\x0435\x0441\x0442\x0432\x0443\x0435\x0442\x002E\x0020\x041F\x0435\x0440\x0435\x0437\x0430\x043F\x0438\x0441\x0430\x0442\x044C\x0020\x0435\x0451\x003F";
    if (nStringID == STRID_RENAME_ERROR)
      return L"\x041E\x0448\x0438\x0431\x043A\x0430\x0020\x043F\x0440\x0438\x0020\x043F\x0435\x0440\x0435\x0438\x043C\x0435\x043D\x043E\x0432\x0430\x043D\x0438\x0438\x0020\x0441\x0435\x0441\x0441\x0438\x0438\x002E";
    if (nStringID == STRID_SESSION_CHANGED)
      return L"\x0421\x0435\x0441\x0441\x0438\x044F\x0020\x0022\x0025\x0073\x0022\x0020\x0431\x044B\x043B\x0430\x0020\x0438\x0437\x043C\x0435\x043D\x0451\x043D\x0430\x002E\x0020\x0421\x043E\x0445\x0440\x0430\x043D\x0438\x0442\x044C\x003F";
    if (nStringID == STRID_CONFIRM_DELETE)
      return L"\x0412\x044B\x0020\x0443\x0432\x0435\x0440\x0435\x043D\x044B\x002C\x0020\x0447\x0442\x043E\x0020\x0445\x043E\x0442\x0438\x0442\x0435\x0020\x0443\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x0441\x0435\x0441\x0441\x0438\x044E\x003F";
    if (nStringID == STRID_RESTARTPROGRAM)
      return L"\x0418\x0437\x043C\x0435\x043D\x0435\x043D\x0438\x044F\x0020\x0432\x0441\x0442\x0443\x043F\x044F\x0442\x0020\x0432\x0020\x0441\x0438\x043B\x0443\x0020\x043F\x043E\x0441\x043B\x0435\x0020\x043F\x0435\x0440\x0435\x0437\x0430\x043F\x0443\x0441\x043A\x0430\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x044B\x002E";
    if (nStringID == STRID_SDI_ISNTSUPPORTED)
      return L"\x0053\x0044\x0049\x0020\x0440\x0435\x0436\x0438\x043C\x0020\x043D\x0435\x0020\x043F\x043E\x0434\x0434\x0435\x0440\x0436\x0438\x0432\x0430\x0435\x0442\x0441\x044F\x002E";
    if (nStringID == STRID_FILTER)
      return L"\x0422\x0435\x043A\x0441\x0442\x043E\x0432\x044B\x0435\x0020\x0444\x0430\x0439\x043B\x044B (*.txt)\0*.txt\0\x0412\x0441\x0435\x0020\x0444\x0430\x0439\x043B\x044B (*.*)\0*.*\0\0";
    if (nStringID == STRID_DROPTOCURRENT)
      return L"\x0414\x043B\x044F\x0020\x0442\x0435\x043A\x0443\x0449\x0435\x0439\x0020\x0441\x0435\x0441\x0441\x0438\x0438\x0020\x0444\x0430\x0439\x043B\x044B\x0020\x043D\x0435\x0020\x043F\x0440\x0438\x043D\x0438\x043C\x0430\x044E\x0442\x0441\x044F.";
  }
  else
  {
    if (nStringID == STRID_PARSEMSG_NOOPENBRACKET)
      return L"No opening bracket.";
    if (nStringID == STRID_PARSEMSG_NOCLOSEBRACKET)
      return L"No closing bracket.";
    if (nStringID == STRID_SESSION)
      return L"Session";
    if (nStringID == STRID_OPEN)
      return L"Open";
    if (nStringID == STRID_UPDATE)
      return L"Update";
    if (nStringID == STRID_SAVE)
      return L"Save";
    if (nStringID == STRID_EDIT)
      return L"Edit";
    if (nStringID == STRID_COPY)
      return L"Copy";
    if (nStringID == STRID_RENAME)
      return L"Rename";
    if (nStringID == STRID_DELETE)
      return L"Delete";
    if (nStringID == STRID_ADDDIR)
      return L"Add folder";
    if (nStringID == STRID_ITEMNAME)
      return L"Name:";
    if (nStringID == STRID_ITEMFILE)
      return L"File:";
    if (nStringID == STRID_MENU_OPEN)
      return L"Open\tEnter";
    if (nStringID == STRID_MENU_CLOSE)
      return L"Close\tCtrl+W";
    if (nStringID == STRID_MENU_ACTIVATE)
      return L"Activate\tEnter";
    if (nStringID == STRID_MENU_ADDCURFILE)
      return L"Add current file";
    if (nStringID == STRID_MENU_ADDFILE)
      return L"Add files...\tInsert";
    if (nStringID == STRID_MENU_ADDDIR)
      return L"Add folder...\tF7";
    if (nStringID == STRID_MENU_RENAME)
      return L"Rename...\tF2";
    if (nStringID == STRID_MENU_MOVEDOWN)
      return L"Move down\tAlt+Down";
    if (nStringID == STRID_MENU_MOVEUP)
      return L"Move up\tAlt+Up";
    if (nStringID == STRID_MENU_DELETE)
      return L"Delete\tDelete";
    if (nStringID == STRID_MENU_DELETEOLD)
      return L"Delete non-existent";
    if (nStringID == STRID_NEWNAME)
      return L"New name";
    if (nStringID == STRID_SETTINGS)
      return L"Settings...";
    if (nStringID == STRID_SAVESESSIONS)
      return L"Save sessions";
    if (nStringID == STRID_PROGRAMDIR)
      return L"Program directory";
    if (nStringID == STRID_APPDATA)
      return L"Application data directory";
    if (nStringID == STRID_OPENONSTART)
      return L"Open on start:";
    if (nStringID == STRID_SAVEONEXIT)
      return L"Save on exit:";
    if (nStringID == STRID_SHOWPATH)
      return L"Show files path";
    if (nStringID == STRID_SAVERELATIVE)
      return L"Save AkelPad path as %a";
    if (nStringID == STRID_SYSTEMFONT)
      return L"System font in list";
    if (nStringID == STRID_CODERTHEME)
      return L"Use Coder themes";
    if (nStringID == STRID_DIALOGTYPE)
      return L"Dialog type";
    if (nStringID == STRID_MODALDIALOG)
      return L"Modal dialog";
    if (nStringID == STRID_MODELESSDIALOG)
      return L"Modeless dialog";
    if (nStringID == STRID_DOCKABLEDIALOG)
      return L"Dockable dialog";
    if (nStringID == STRID_AUTOLOAD)
      return L"Autoload";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_SAVEDATA)
      return L"Save data";
    if (nStringID == STRID_SAVEACTIVE)
      return L"Active document";
    if (nStringID == STRID_SAVECODEPAGE)
      return L"Codepage";
    if (nStringID == STRID_SAVESELECTION)
      return L"Selection";
    if (nStringID == STRID_SAVEWORDWRAP)
      return L"Word wrap";
    if (nStringID == STRID_SAVEREADONLY)
      return L"Read only";
    if (nStringID == STRID_SAVEOVERTYPE)
      return L"Overtype mode";
    if (nStringID == STRID_SAVEBOOKMARKS)
      return L"LineBoard bookmarks";
    if (nStringID == STRID_SAVEALIAS)
      return L"Coder file alias";
    if (nStringID == STRID_SAVEFOLDS)
      return L"Coder collapsed folds";
    if (nStringID == STRID_SAVEMARKS)
      return L"Coder text marks";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
    if (nStringID == STRID_CLOSE)
      return L"Close";
    if (nStringID == STRID_CURRENTSESSION)
      return L"<Current>";
    if (nStringID == STRID_ALREADY_EXIST)
      return L"Session \"%s\" already exists. Overwrite it?";
    if (nStringID == STRID_RENAME_ERROR)
      return L"Can't rename session.";
    if (nStringID == STRID_SESSION_CHANGED)
      return L"Session \"%s\" was modified. Do you want to save it?";
    if (nStringID == STRID_CONFIRM_DELETE)
      return L"Are you sure you want to delete session?";
    if (nStringID == STRID_RESTARTPROGRAM)
      return L"Changes will be reflected upon restarting the program.";
    if (nStringID == STRID_SDI_ISNTSUPPORTED)
      return L"SDI mode is not supported.";
    if (nStringID == STRID_FILTER)
      return L"Text files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
    if (nStringID == STRID_DROPTOCURRENT)
      return L"Drop allowed only for non-current session.";
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
  hTab=pd->hTab;
  hMainIcon=pd->hMainIcon;
  hGlobalAccel=pd->hGlobalAccel;
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
  xstrcpynW(wszOpenOnStart, L"OnExit", MAX_PATH);
  xstrcpynW(wszSaveOnExit, L"OnExit", MAX_PATH);
  ReadOptions(0);

  if (nSaveSessions == SDIR_PROGRAM)
  {
    xprintfW(wszSessionsDir, L"%s\\AkelFiles\\Plugs\\Sessions", pd->wszAkelDir);
  }
  else if (nSaveSessions == SDIR_APPDATA)
  {
    HMODULE hShell32=GetModuleHandleA("shell32.dll");

    if (bOldWindows)
    {
      SHGetSpecialFolderPathAPtr=(BOOL (WINAPI *)(HWND, char *, int, BOOL))GetProcAddress(hShell32, "SHGetSpecialFolderPathA");
      if (SHGetSpecialFolderPathAPtr(hMainWnd, szBuffer, CSIDL_APPDATA, FALSE))
        xprintfW(wszSessionsDir, L"%S\\AkelPad\\Sessions", szBuffer);
    }
    else
    {
      SHGetSpecialFolderPathWPtr=(BOOL (WINAPI *)(HWND, wchar_t *, int, BOOL))GetProcAddress(hShell32, "SHGetSpecialFolderPathW");
      if (SHGetSpecialFolderPathWPtr(hMainWnd, wszBuffer, CSIDL_APPDATA, FALSE))
        xprintfW(wszSessionsDir, L"%s\\AkelPad\\Sessions", wszBuffer);
    }
  }
}

void InitMain()
{
  bInitMain=TRUE;

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

  if (nDialogType == DLGT_DOCKABLE)
  {
    NewEditProcData=NULL;
    SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NewEditProc, (LPARAM)&NewEditProcData);
  }

  //Create image list
  {
    HICON hIcon;

    hImageList=ImageList_Create(16, 16, ILC_COLOR16|ILC_MASK, 0, 0);

    if (hIcon=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_FILE), IMAGE_ICON, 16, 16, 0))
    {
      ImageList_AddIcon(hImageList, hIcon);
      DestroyIcon(hIcon);
    }
    if (hIcon=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_FOLDER), IMAGE_ICON, 16, 16, 0))
    {
      ImageList_AddIcon(hImageList, hIcon);
      DestroyIcon(hIcon);
    }
  }
  hCursorDragMove=(HCURSOR)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDC_CURSOR_DRAGMOVE), IMAGE_CURSOR, 0, 0, 0);
  hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
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
  if (NewEditProcData)
  {
    SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&NewEditProcData);
    NewEditProcData=NULL;
  }

  FreeSessions(&hSessionStack);

  //Destroy resources
  ImageList_Destroy(hImageList);
  DestroyCursor(hCursorDragMove);
  DestroyIcon(hPluginIcon);
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
