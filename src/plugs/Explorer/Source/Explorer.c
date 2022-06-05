#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <richedit.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include stack functions
#define StackGetElement
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackMoveAfter
#define StackMoveBefore
#define StackMoveIndex
#define StackDelete
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemset
#define xmemcmp
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
#define AppendMenuWide
#define ComboBox_AddStringWide
#define ComboBox_FindStringExactWide
#define ComboBox_GetLBTextWide
#define CreateDialogWide
#define CreateDirectoryWide
#define CreateFileWide
#define DialogBoxParamWide
#define DialogBoxWide
#define FindFirstFileWide
#define FindNextFileWide
#define GetDlgItemTextWide
#define GetFileAttributesWide
#define GetWindowLongPtrWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define SHBrowseForFolderWide
#define SHGetPathFromIDListWide
#define TreeView_GetItemWide
#define TreeView_InsertItemWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_SETUP                1
#define STRID_MENURENAME           2
#define STRID_MENUFINDDOCUMENT     3
#define STRID_MENUREFRESH          4
#define STRID_MENUCREATEDIR        5
#define STRID_MENUCREATEFILE       6
#define STRID_MENUFILTERNONE       7
#define STRID_MENUFILTERINCLUDE    8
#define STRID_MENUFILTEREXCLUDE    9
#define STRID_MENUFILTERITEMADD    10
#define STRID_MENUFILTERITEMUP     11
#define STRID_MENUFILTERITEMDOWN   12
#define STRID_MENUFILTERITEMREMOVE 13
#define STRID_ROOTDIRECTORY        14
#define STRID_MYCOMPUTER           15
#define STRID_SHOWHIDDEN           16
#define STRID_AUTOFIND             17
#define STRID_SINGLECLICK          18
#define STRID_SETSAVELOCATION      19
#define STRID_CREATEDIR            20
#define STRID_CREATEFILE           21
#define STRID_LOADFIRST            22
#define STRID_PLUGIN               23
#define STRID_OK                   24
#define STRID_CANCEL               25
#define STRID_DEFAULTCODER         26
#define STRID_DEFAULTINCLUDE       27
#define STRID_DEFAULTEXCLUDE       28

#define DLLA_EXPLORER_GOTOPATH  1
#define DLLA_EXPLORER_REFRESH   2
#define DLLA_EXPLORER_GETDOCK   3
#define DLLA_EXPLORER_ROOTPATH  4

#define AKDLL_INIT          (WM_USER + 100)
#define AKDLL_SETUP         (WM_USER + 101)
#define AKDLL_RENAME        (WM_USER + 102)
#define AKDLL_FINDDOCUMENT  (WM_USER + 103)
#define AKDLL_REFRESH       (WM_USER + 104)
#define AKDLL_CREATEDIR     (WM_USER + 105)
#define AKDLL_CREATEFILE    (WM_USER + 106)
#define AKDLL_OPENSELECTED  (WM_USER + 107)

#define OF_RECT           0x1
#define OF_SETTINGS       0x2
#define OF_FILTER         0x4

//DestroyDock type
#define DKT_DEFAULT        0x0
#define DKT_NOUNLOAD       0x1
#define DKT_ONMAINFINISH   0x2
#define DKT_KEEPAUTOLOAD   0x4

//Filter
#define FILTER_NONE       0
#define FILTER_INCLUDE    1
#define FILTER_EXCLUDE    2

#ifndef BIF_NEWDIALOGSTYLE
  #define BIF_NEWDIALOGSTYLE 0x0040
#endif
#ifndef SFGAO_STREAM
  #define SFGAO_STREAM 0x00400000
#endif

typedef struct _FILTERGROUP {
  struct _FILTERGROUP *next;
  struct _FILTERGROUP *prev;
  wchar_t wszName[MAX_PATH];
  int nNameLen;
  wchar_t wszGroup[MAX_PATH];
  int nGroupLen;
  wchar_t *lpItems[100];
  int nItemCount;
} FILTERGROUP;

typedef struct {
  FILTERGROUP *first;
  FILTERGROUP *last;
  int nElements;
} HFILTERGROUPSTACK;

typedef struct tagID {
  LPSHELLFOLDER psfParent;
  LPITEMIDLIST pidlFullPath;
  LPITEMIDLIST pidlItem;
} TVITEMDATA, *LPTVITEMDATA;

typedef struct {
  POSTMESSAGE pm;
  OPENDOCUMENTW odp;
  wchar_t szFile[MAX_PATH];
  wchar_t szWorkDir[MAX_PATH];
} PMOPENDOCUMENT;

#ifndef _INC_COMMCTRL
  typedef struct tagTVKEYDOWN {
    NMHDR hdr;
    WORD wVKey;
    UINT flags;
  } NMTVKEYDOWN, FAR *LPNMTVKEYDOWN;
#endif

//Functions prototypes
void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow);
void DestroyDock(HWND hWndDock, DWORD dwType);
BOOL CALLBACK DockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL CALLBACK InputBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ClearTreeView(HWND hWndTreeView, BOOL bRedraw);
void PopulateTree(HWND hWnd, int nRootSpecial, const wchar_t *wpRootDirectory);
void FillTreeView(HWND hWnd, LPSHELLFOLDER psfRoot, LPITEMIDLIST pidlRoot, HTREEITEM hParent);
void SearchTreeView(HWND hWnd, const wchar_t *wpPath);
HTREEITEM GetSelItemTreeView(HWND hWnd, wchar_t *wszPath);
void OpenSelItemTreeView(HWND hWnd);
int CALLBACK TreeViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
BOOL GetItemName(LPSHELLFOLDER psf, LPITEMIDLIST pidl, DWORD dwFlags, wchar_t *wszName, int nMaxName);
int GetItemIcon(LPITEMIDLIST pidl, UINT uFlags);
LPITEMIDLIST CreatePIDL(UINT cbSize);
LPITEMIDLIST NextPIDL(LPCITEMIDLIST pidl);
USHORT CutPIDL(LPITEMIDLIST pidl);
UINT GetSizePIDL(LPCITEMIDLIST pidl);
LPITEMIDLIST CopyPIDL(LPCITEMIDLIST pidl);
LPITEMIDLIST ConcatPIDL(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
BOOL CompareITEMID(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
LPITEMIDLIST ComparePIDL(LPCITEMIDLIST pidlFull, LPCITEMIDLIST pidlPart);
void OpenDocument(const wchar_t *wpFile);
FILTERGROUP* StackInsertGroup(HFILTERGROUPSTACK *hStack);
void StackDeleteGroup(HFILTERGROUPSTACK *hStack, void *lpFilterGroup);
void StackFreeGroup(HFILTERGROUPSTACK *hStack);
void FilterComboboxFill(HWND hWnd);
int FilterStackToString(HFILTERGROUPSTACK *hStack, wchar_t *wszString, int nStringMax);
int FilterGroupToString(FILTERGROUP *lpFilterGroup, wchar_t *wszString, int nStringMax);
void FilterStringToStack(const wchar_t *wpString, HFILTERGROUPSTACK *hStack);
int FilterStringToGroup(const wchar_t *wpString, FILTERGROUP *lpFilterGroup);
BOOL FileMaskCmp(const wchar_t *wpMaskStr, const wchar_t *wpFileStr);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
BOOL IsDirEmpty(const wchar_t *wpDir);
BOOL TrimTrailBackSlash(wchar_t *wszDir);
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
int (WINAPI *SHFileOperationAPtr)(LPSHFILEOPSTRUCTA);
int (WINAPI *SHFileOperationWPtr)(LPSHFILEOPSTRUCTW);
UINT_PTR (WINAPI *SHGetFileInfoAPtr)(const char *, DWORD, SHFILEINFOA *, UINT, UINT);
UINT_PTR (WINAPI *SHGetFileInfoWPtr)(const wchar_t *, DWORD, SHFILEINFOW *, UINT, UINT);

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
PLUGINFUNCTION *pfExplorer;
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hWndEdit;
BOOL bOldWindows;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
HWND hWndDockDlg=NULL;
HWND hWndBrowsePath=NULL;
HWND hWndBrowseTree=NULL;
RECT rcExplorerCurrentDialog={0};
RECT rcExplorerDockRect={0};
int nExplorerDockSide=DKS_LEFT;
DOCK *dkExplorerDlg=NULL;
BOOL bExplorerDockWaitResize=FALSE;
wchar_t wszRootDirectory[MAX_PATH];
wchar_t *wszFilterInclude=NULL;
wchar_t *wszFilterExclude=NULL;
wchar_t wszGotoPath[MAX_PATH];
char szPath[MAX_PATH];
wchar_t wszPath[MAX_PATH];
wchar_t wszInputBox[MAX_PATH];
HFILTERGROUPSTACK hFilterIncludeStack={0};
HFILTERGROUPSTACK hFilterExcludeStack={0};
LPMALLOC lpMalloc=NULL;
int nRootSpecial=CSIDL_DESKTOP;
int nFilterType=FILTER_NONE;
BOOL bShowHidden=FALSE;
BOOL bAutoFind=TRUE;
BOOL bSingleClick=FALSE;
BOOL bSetSaveLocation=FALSE;
BOOL bRenaming=FALSE;
BOOL bOnMainStart=FALSE;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Explorer";
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

    if (nAction == DLLA_EXPLORER_GOTOPATH)
    {
      unsigned char *pPath=NULL;
      EDITINFO ei;

      if (IsExtCallParamValid(pd->lParam, 2))
        pPath=(unsigned char *)GetExtCallParam(pd->lParam, 2);

      if (pPath)
      {
        if (pd->dwSupport & PDS_STRANSI)
          MultiByteToWideChar(CP_ACP, 0, (char *)pPath, -1, wszGotoPath, MAX_PATH);
        else
          xstrcpynW(wszGotoPath, (wchar_t *)pPath, MAX_PATH);
        if (!wszGotoPath[0])
        {
          if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
            xstrcpynW(wszGotoPath, ei.wszFile, MAX_PATH);
        }

        if (bInitMain)
        {
          SearchTreeView(hWndBrowseTree, wszGotoPath);
          wszGotoPath[0]=L'\0';

          //Stay in memory, and show as active
          pd->nUnload=UD_NONUNLOAD_ACTIVE;
          return;
        }
      }
    }
    else if (nAction == DLLA_EXPLORER_ROOTPATH)
    {
      unsigned char *pPath=NULL;

      if (IsExtCallParamValid(pd->lParam, 2))
        pPath=(unsigned char *)GetExtCallParam(pd->lParam, 2);

      if (pPath)
      {
        if (pd->dwSupport & PDS_STRANSI)
          MultiByteToWideChar(CP_ACP, 0, (char *)pPath, -1, wszRootDirectory, MAX_PATH);
        else
          xstrcpynW(wszRootDirectory, (wchar_t *)pPath, MAX_PATH);
        if (xatoiW(wszRootDirectory, NULL) == -1)
        {
          wszRootDirectory[0]=L'\0';
          nRootSpecial=CSIDL_DRIVES;
        }
        else nRootSpecial=CSIDL_DESKTOP;

        dwSaveFlags|=OF_SETTINGS;

        if (bInitMain)
        {
          PostMessage(hWndDockDlg, AKDLL_REFRESH, 0, 0);
          if (bAutoFind)
            PostMessage(hWndDockDlg, AKDLL_FINDDOCUMENT, 0, 0);

          //Stay in memory, and show as active
          pd->nUnload=UD_NONUNLOAD_ACTIVE;
          return;
        }
      }
    }
    else
    {
      if (bInitMain)
      {
        if (nAction == DLLA_EXPLORER_REFRESH)
        {
          if (hWndDockDlg) PostMessage(hWndDockDlg, AKDLL_REFRESH, 0, 0);
        }
        else if (nAction == DLLA_EXPLORER_GETDOCK)
        {
          HWND *lpWndDock=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            lpWndDock=(HWND *)GetExtCallParam(pd->lParam, 2);

          if (lpWndDock)
            *lpWndDock=hWndDockDlg;
        }
      }
      else
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_LOADFIRST), L"Explorer::Main");
        MessageBoxW(pd->hMainWnd, wszBuffer, L"Explorer::Main", MB_OK|MB_ICONEXCLAMATION);
      }

      //If plugin already loaded, stay in memory and don't change active status
      if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
      return;
    }
  }

  //Initialize
  if (bInitMain)
  {
    if (hWndDockDlg) DestroyDock(hWndDockDlg, DKT_KEEPAUTOLOAD);

    //Stay in memory, but show as non-active
    pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();

    pfExplorer=pd->lpPluginFunction;
    bExplorerDockWaitResize=pd->bOnStart;
    CreateDock(&hWndDockDlg, &dkExplorerDlg, !bExplorerDockWaitResize);

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow)
{
  DOCK dk;

  *hWndDock=NULL;

  xmemset(&dk, 0, sizeof(DOCK));
  dk.dwFlags=DKF_DRAGDROP;
  dk.nSide=nExplorerDockSide;
  dk.rcSize=rcExplorerDockRect;

  if (*dkDock=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_ADD, (LPARAM)&dk))
  {
    *hWndDock=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_EXPLORER), hMainWnd, (DLGPROC)DockDlgProc);

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
  static HWND hWndFilterType;
  static HWND hWndFilterCombo;
  static HWND hWndFilterItem;
  static HICON hIconFilterNone;
  static HICON hIconFilterIncl;
  static HICON hIconFilterExcl;
  static HMENU hMenuBrowse;
  static HMENU hMenuFilterType;
  static HMENU hMenuFilterItem;
  static LPCONTEXTMENU2 pExplorerSubMenu2;
  static LPCONTEXTMENU3 pExplorerSubMenu3;
  static BOOL bLabelEditing;
  static RESIZEDIALOG rds[]={{&hWndTitleText,   RDS_SIZE|RDS_X, 0},
                             {&hWndTitleClose,  RDS_MOVE|RDS_X, 0},
                             {&hWndBrowseTree,  RDS_SIZE|RDS_X, 0},
                             {&hWndBrowseTree,  RDS_SIZE|RDS_Y, 0},
                             {&hWndBrowsePath,  RDS_SIZE|RDS_X, 0},
                             {&hWndFilterType,  RDS_MOVE|RDS_Y, 0},
                             {&hWndFilterCombo, RDS_MOVE|RDS_Y, 0},
                             {&hWndFilterCombo, RDS_SIZE|RDS_X, 0},
                             {&hWndFilterItem,  RDS_MOVE|RDS_X, 0},
                             {&hWndFilterItem,  RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    hWndTitleText=GetDlgItem(hDlg, IDC_TITLETEXT);
    hWndTitleClose=GetDlgItem(hDlg, IDC_CLOSE);
    hWndBrowsePath=GetDlgItem(hDlg, IDC_BROWSE_PATH);
    hWndBrowseTree=GetDlgItem(hDlg, IDC_BROWSE_TREE);
    hWndFilterType=GetDlgItem(hDlg, IDC_BROWSE_FILTERTYPE);
    hWndFilterCombo=GetDlgItem(hDlg, IDC_BROWSE_FILTERCOMBO);
    hWndFilterItem=GetDlgItem(hDlg, IDC_BROWSE_FILTERITEM);

    hMenuBrowse=CreatePopupMenu();
    AppendMenuWide(hMenuBrowse, MF_STRING, IDM_RENAME, GetLangStringW(wLangModule, STRID_MENURENAME));
    AppendMenuWide(hMenuBrowse, MF_STRING, IDM_FINDDOCUMENT, GetLangStringW(wLangModule, STRID_MENUFINDDOCUMENT));
    AppendMenuWide(hMenuBrowse, MF_STRING, IDM_REFRESH, GetLangStringW(wLangModule, STRID_MENUREFRESH));
    AppendMenuWide(hMenuBrowse, MF_STRING, IDM_CREATEDIR, GetLangStringW(wLangModule, STRID_MENUCREATEDIR));
    AppendMenuWide(hMenuBrowse, MF_STRING, IDM_CREATEFILE, GetLangStringW(wLangModule, STRID_MENUCREATEFILE));
    AppendMenuWide(hMenuBrowse, MF_SEPARATOR, 0, NULL);
    AppendMenuWide(hMenuBrowse, MF_STRING, IDM_SETUP, GetLangStringW(wLangModule, STRID_SETUP));

    hMenuFilterType=CreatePopupMenu();
    AppendMenuWide(hMenuFilterType, MF_STRING, IDM_FILTERNONE, GetLangStringW(wLangModule, STRID_MENUFILTERNONE));
    AppendMenuWide(hMenuFilterType, MF_STRING, IDM_FILTERINCLUDE, GetLangStringW(wLangModule, STRID_MENUFILTERINCLUDE));
    AppendMenuWide(hMenuFilterType, MF_STRING, IDM_FILTEREXCLUDE, GetLangStringW(wLangModule, STRID_MENUFILTEREXCLUDE));
    CheckMenuRadioItem(hMenuFilterType, IDM_FILTERNONE, IDM_FILTEREXCLUDE, IDM_FILTERNONE + nFilterType, MF_BYCOMMAND);

    hMenuFilterItem=CreatePopupMenu();
    AppendMenuWide(hMenuFilterItem, MF_STRING, IDM_FILTERITEMADD, GetLangStringW(wLangModule, STRID_MENUFILTERITEMADD));
    AppendMenuWide(hMenuFilterItem, MF_STRING, IDM_FILTERITEMMOVEUP, GetLangStringW(wLangModule, STRID_MENUFILTERITEMUP));
    AppendMenuWide(hMenuFilterItem, MF_STRING, IDM_FILTERITEMMOVEDOWN, GetLangStringW(wLangModule, STRID_MENUFILTERITEMDOWN));
    AppendMenuWide(hMenuFilterItem, MF_STRING, IDM_FILTERITEMREMOVE, GetLangStringW(wLangModule, STRID_MENUFILTERITEMREMOVE));

    hIconFilterNone=LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_FILTERNONE), IMAGE_ICON, 0, 0, 0);
    hIconFilterIncl=LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_FILTERINCL), IMAGE_ICON, 0, 0, 0);
    hIconFilterExcl=LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_FILTEREXCL), IMAGE_ICON, 0, 0, 0);

    //Set dock title
    {
      BUTTONDRAW bd={BIF_CROSS|BIF_ETCHED};

      SetWindowTextWide(hWndTitleText, wszPluginName);
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndTitleClose, (LPARAM)&bd);

      bd.dwFlags=BIF_ICON|BIF_ENABLEFOCUS;
      if (nFilterType == FILTER_NONE)
        bd.hImage=hIconFilterNone;
      else if (nFilterType == FILTER_INCLUDE)
        bd.hImage=hIconFilterIncl;
      else if (nFilterType == FILTER_EXCLUDE)
        bd.hImage=hIconFilterExcl;
      bd.nImageWidth=16;
      bd.nImageHeight=16;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndFilterType, (LPARAM)&bd);

      bd.dwFlags=BIF_DOWNARROW|BIF_ENABLEFOCUS;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndFilterItem, (LPARAM)&bd);
    }

    //Fill combobox
    FilterComboboxFill(hWndFilterCombo);
    EnableWindow(hWndFilterCombo, nFilterType);
    EnableWindow(hWndFilterItem, nFilterType && GetWindowTextLengthWide(hWndFilterCombo));
    //After AKD_RESIZEDIALOG control is selected, post deselect message
    PostMessage(hWndFilterCombo, CB_SETEDITSEL, 0, (LPARAM)MAKELONG(0, 0));

    //Set style
    if (bSingleClick)
    {
      DWORD dwStyle;

      dwStyle=(DWORD)GetWindowLongPtrWide(hWndBrowseTree, GWL_STYLE);
      SetWindowLongPtrWide(hWndBrowseTree, GWL_STYLE, dwStyle | TVS_TRACKSELECT);
    }

    if (!bOnMainStart) SendMessage(hDlg, AKDLL_INIT, 0, 0);
  }
  else if (uMsg == AKDLL_INIT)
  {
    HIMAGELIST hImageList;
    SHFILEINFOA sfiA;
    SHFILEINFOW sfiW;

    //Enable images
    if (bOldWindows)
      hImageList=(HIMAGELIST)SHGetFileInfoAPtr("C:\\", 0, &sfiA, sizeof(SHFILEINFOA), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
    else
      hImageList=(HIMAGELIST)SHGetFileInfoWPtr(L"C:\\", 0, &sfiW, sizeof(SHFILEINFOW), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
    if (hImageList) SendMessage(hWndBrowseTree, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)hImageList);

    PopulateTree(hWndBrowseTree, nRootSpecial, wszRootDirectory);

    if (*wszGotoPath)
    {
      SearchTreeView(hWndBrowseTree, wszGotoPath);
      wszGotoPath[0]=L'\0';
    }
    else
    {
      if (bAutoFind)
        PostMessage(hDlg, AKDLL_FINDDOCUMENT, 0, 0);
    }
  }
  else if (uMsg == AKDLL_SETUP)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hDlg, (DLGPROC)SetupDlgProc);
  }
  else if (uMsg == AKDLL_RENAME)
  {
    HTREEITEM hSelItem;

    bRenaming=TRUE;
    if (hSelItem=(HTREEITEM)SendMessage(hWndBrowseTree, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL))
      SendMessage(hWndBrowseTree, TVM_EDITLABEL, 0, (LPARAM)hSelItem);
    bRenaming=FALSE;
  }
  else if (uMsg == AKDLL_FINDDOCUMENT)
  {
    EDITINFO ei;

    if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
    {
      if (*ei.wszFile)
      {
        SearchTreeView(hWndBrowseTree, ei.wszFile);
      }
    }
  }
  else if (uMsg == AKDLL_CREATEDIR ||
           uMsg == AKDLL_CREATEFILE)
  {
    wchar_t wszCreateName[MAX_PATH];

    DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_INPUTBOX), hDlg, (DLGPROC)InputBoxProc, (LPARAM)uMsg);

    if (wszInputBox[0])
    {
      GetSelItemTreeView(hWndBrowseTree, wszPath);
      if (GetFileAttributesWide(wszPath) & FILE_ATTRIBUTE_DIRECTORY)
        xstrcpynW(wszCreateName, wszPath, MAX_PATH);
      else
        GetFileDir(wszPath, -1, wszCreateName, MAX_PATH);
      xprintfW(wszCreateName, L"%s\\%s", wszCreateName, wszInputBox);

      if (uMsg == AKDLL_CREATEDIR)
      {
        if (CreateDirectoryWide(wszCreateName, NULL))
        {
          SearchTreeView(hWndBrowseTree, wszCreateName);
          SetFocus(hWndBrowseTree);
        }
      }
      else if (uMsg == AKDLL_CREATEFILE)
      {
        HANDLE hFile;

        if ((hFile=CreateFileWide(wszCreateName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
        {
          CloseHandle(hFile);

          //Open file
          OpenDocument(wszCreateName);
          if (!bAutoFind) SearchTreeView(hWndBrowseTree, wszCreateName);
        }
      }
    }
    else SetFocus(hWndBrowseTree);
  }
  else if (uMsg == AKDLL_REFRESH)
  {
    wchar_t wszOldPath[MAX_PATH];
    HTREEITEM hSelItem;

    //Remember non-parent item
    if (hSelItem=(HTREEITEM)SendMessage(hWndBrowseTree, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL))
    {
      if (SendMessage(hWndBrowseTree, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hSelItem))
      {
        GetSelItemTreeView(hWndBrowseTree, wszOldPath);
      }
      else hSelItem=NULL;
    }
    PopulateTree(hWndBrowseTree, nRootSpecial, wszRootDirectory);
    SetWindowTextWide(hWndBrowsePath, L"");

    //Restore non-parent item
    if (hSelItem) SearchTreeView(hWndBrowseTree, wszOldPath);
  }
  else if (uMsg == AKDLL_OPENSELECTED)
  {
    OpenSelItemTreeView(hWndBrowseTree);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_BROWSE_TREE)
    {
      if (((NMTREEVIEW *)lParam)->hdr.code == TVN_ITEMEXPANDINGA ||
          ((NMTREEVIEW *)lParam)->hdr.code == TVN_ITEMEXPANDINGW)
      {
        NMTREEVIEW *pnmtv=(NMTREEVIEW *)lParam;
        LPTVITEMDATA lptvid;
        LPSHELLFOLDER psfRoot=NULL;
        TVSORTCB tvscb;
        HRESULT hr;

        if (!(pnmtv->itemNew.state & TVIS_EXPANDEDONCE))
        {
          if (lptvid=(LPTVITEMDATA)pnmtv->itemNew.lParam)
          {
            hr=lptvid->psfParent->lpVtbl->BindToObject(lptvid->psfParent, lptvid->pidlItem, 0, &IID_IShellFolder, (void **)&psfRoot);

            if (SUCCEEDED(hr))
            {
              FillTreeView(hWndBrowseTree, psfRoot, lptvid->pidlFullPath, pnmtv->itemNew.hItem);

              tvscb.hParent=pnmtv->itemNew.hItem;
              tvscb.lParam=0;
              tvscb.lpfnCompare=TreeViewCompareProc;
              SendMessage(hWndBrowseTree, TVM_SORTCHILDRENCB, 0, (LPARAM)&tvscb);

              psfRoot->lpVtbl->Release(psfRoot);
            }
          }
        }
        else SendMessage(hWndBrowseTree, TVM_EXPAND, TVE_COLLAPSE|TVE_COLLAPSERESET, (LPARAM)pnmtv->itemNew.hItem);
      }
      else if (((NMTREEVIEW *)lParam)->hdr.code == TVN_SELCHANGEDA ||
               ((NMTREEVIEW *)lParam)->hdr.code == TVN_SELCHANGEDW)
      {
        NMTREEVIEW *pnmtv=(NMTREEVIEW *)lParam;
        LPTVITEMDATA lptvid;

        wszPath[0]='\0';

        if (lptvid=(LPTVITEMDATA)pnmtv->itemNew.lParam)
        {
          SHGetPathFromIDListWide(lptvid->pidlFullPath, wszPath);
        }
        SetWindowTextWide(hWndBrowsePath, wszPath);
      }
      else if (((NMTREEVIEW *)lParam)->hdr.code == TVN_DELETEITEMA ||
               ((NMTREEVIEW *)lParam)->hdr.code == TVN_DELETEITEMW)
      {
        NMTREEVIEW *pnmtv=(NMTREEVIEW *)lParam;
        LPTVITEMDATA lptvid;

        if (lptvid=(LPTVITEMDATA)pnmtv->itemOld.lParam)
        {
          lptvid->psfParent->lpVtbl->Release(lptvid->psfParent);
          lpMalloc->lpVtbl->Free(lpMalloc, lptvid->pidlFullPath);
          lpMalloc->lpVtbl->Free(lpMalloc, lptvid->pidlItem);
          lpMalloc->lpVtbl->Free(lpMalloc, lptvid);
        }
      }
      else if (((NMTREEVIEW *)lParam)->hdr.code == TVN_BEGINLABELEDITA ||
               ((NMTREEVIEW *)lParam)->hdr.code == TVN_BEGINLABELEDITW)
      {
        if (bSingleClick && !bRenaming)
        {
          SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, 1);
          return TRUE;
        }
        bLabelEditing=TRUE;
        GetSelItemTreeView(hWndBrowseTree, wszPath);
      }
      else if (((NMTREEVIEW *)lParam)->hdr.code == TVN_ENDLABELEDITA ||
               ((NMTREEVIEW *)lParam)->hdr.code == TVN_ENDLABELEDITW)
      {
        NMTVDISPINFOW *pnmtvdi=(NMTVDISPINFOW *)lParam;
        char szRenameTo[MAX_PATH];
        wchar_t wszRenameTo[MAX_PATH];

        if (pnmtvdi->item.pszText)
        {
          SHFILEOPSTRUCTW shfos;
          int nPathLen=(int)xstrlenW(wszPath);
          int nResult;

          GetFileDir(wszPath, nPathLen, wszRenameTo, MAX_PATH);
          if (bOldWindows)
            xprintfW(wszRenameTo, L"%s\\%S", wszRenameTo, pnmtvdi->item.pszText);
          else
            xprintfW(wszRenameTo, L"%s\\%s", wszRenameTo, pnmtvdi->item.pszText);
          wszPath[nPathLen + 1]='\0';

          shfos.hwnd=hDlg;
          shfos.wFunc=FO_RENAME;
          shfos.pFrom=wszPath;
          shfos.pTo=wszRenameTo;
          shfos.fFlags=0;

          if (bOldWindows)
          {
            WideCharToMultiByte(CP_ACP, 0, wszPath, nPathLen + 2, szPath, MAX_PATH, NULL, NULL);
            WideCharToMultiByte(CP_ACP, 0, wszRenameTo, -1, szRenameTo, MAX_PATH, NULL, NULL);
            shfos.pFrom=(wchar_t *)szPath;
            shfos.pTo=(wchar_t *)szRenameTo;
            nResult=SHFileOperationAPtr((SHFILEOPSTRUCTA *)&shfos);
          }
          else nResult=SHFileOperationWPtr(&shfos);

          if (!nResult)
          {
            if (!shfos.fAnyOperationsAborted)
            {
              SearchTreeView(hWndBrowseTree, wszRenameTo);
            }
          }
        }
        bLabelEditing=FALSE;
      }
      else if (((NMTREEVIEW *)lParam)->hdr.code == TVN_KEYDOWN)
      {
        NMTVKEYDOWN *pnmtvkd=(NMTVKEYDOWN *)lParam;

        if (!bLabelEditing)
        {
          if (pnmtvkd->wVKey == VK_DELETE)
          {
            HTREEITEM hSelItem;

            if (hSelItem=GetSelItemTreeView(hWndBrowseTree, wszPath))
            {
              SHFILEOPSTRUCTW shfos;
              int nPathLen=(int)xstrlenW(wszPath);
              int nResult;

              wszPath[nPathLen + 1]='\0';
              shfos.hwnd=hDlg;
              shfos.wFunc=FO_DELETE;
              shfos.pFrom=wszPath;
              shfos.pTo=NULL;
              shfos.fFlags=(GetKeyState(VK_SHIFT) >= 0)?FOF_ALLOWUNDO:0;

              if (bOldWindows)
              {
                WideCharToMultiByte(CP_ACP, 0, wszPath, nPathLen + 2, szPath, MAX_PATH, NULL, NULL);
                shfos.pFrom=(wchar_t *)szPath;
                nResult=SHFileOperationAPtr((SHFILEOPSTRUCTA *)&shfos);
              }
              else nResult=SHFileOperationWPtr(&shfos);

              if (!nResult)
              {
                if (!shfos.fAnyOperationsAborted)
                {
                  SendMessage(hWndBrowseTree, TVM_DELETEITEM, 0, (LPARAM)hSelItem);
                  SetFocus(hWndBrowseTree);
                }
              }
            }
          }
          else if (pnmtvkd->wVKey == VK_F2)
          {
            PostMessage(hDlg, AKDLL_RENAME, 0, 0);
          }
          else if (pnmtvkd->wVKey == VK_F3)
          {
            PostMessage(hDlg, AKDLL_FINDDOCUMENT, 0, 0);
          }
          else if (pnmtvkd->wVKey == VK_F5)
          {
            PostMessage(hDlg, AKDLL_REFRESH, 0, 0);
          }
          else if (pnmtvkd->wVKey == VK_F7)
          {
            PostMessage(hDlg, AKDLL_CREATEDIR, 0, 0);
          }
          else if (pnmtvkd->wVKey == VK_INSERT)
          {
            PostMessage(hDlg, AKDLL_CREATEFILE, 0, 0);
          }
        }
      }
      else if ((!bSingleClick && ((NMHDR *)lParam)->code == (UINT)NM_DBLCLK) ||
               (bSingleClick && ((NMHDR *)lParam)->code == (UINT)NM_CLICK))
      {
        TVHITTESTINFO tvhti;

        GetCursorPos(&tvhti.pt);
        ScreenToClient(hWndBrowseTree, &tvhti.pt);
        SendMessage(hWndBrowseTree, TVM_HITTEST, 0, (LPARAM)&tvhti);

        if ((tvhti.flags & TVHT_ONITEMLABEL) || (tvhti.flags & TVHT_ONITEMICON))
        {
          if (bSingleClick)
          {
            if (!SendMessage(hWndBrowseTree, TVM_EXPAND, TVE_TOGGLE, (LPARAM)tvhti.hItem))
              PostMessage(hDlg, AKDLL_OPENSELECTED, 0, 0);
          }
          else
          {
            if (tvhti.hItem == (HTREEITEM)SendMessage(hWndBrowseTree, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL))
              PostMessage(hDlg, AKDLL_OPENSELECTED, 0, 0);
          }
        }
      }
    }
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

        nCmd=TrackPopupMenu(hMenuBrowse, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWndTitleText, NULL);

        if (nCmd == IDM_RENAME)
          PostMessage(hDlg, AKDLL_RENAME, 0, 0);
        else if (nCmd == IDM_FINDDOCUMENT)
          PostMessage(hDlg, AKDLL_FINDDOCUMENT, 0, 0);
        else if (nCmd == IDM_REFRESH)
          PostMessage(hDlg, AKDLL_REFRESH, 0, 0);
        else if (nCmd == IDM_CREATEDIR)
          PostMessage(hDlg, AKDLL_CREATEDIR, 0, 0);
        else if (nCmd == IDM_CREATEFILE)
          PostMessage(hDlg, AKDLL_CREATEFILE, 0, 0);
        else if (nCmd == IDM_SETUP)
          PostMessage(hDlg, AKDLL_SETUP, 0, 0);
      }
    }
    else if ((HWND)wParam == hWndBrowseTree)
    {
      TVHITTESTINFO tvhti;
      TVITEMW tvi;
      LPTVITEMDATA lptvid;
      POINT ptScreen;
      int nCmd;

      GetCursorPos(&ptScreen);
      tvhti.pt=ptScreen;
      ScreenToClient(hWndBrowseTree, &tvhti.pt);
      SendMessage(hWndBrowseTree, TVM_HITTEST, 0, (LPARAM)&tvhti);

      if ((tvhti.flags & TVHT_ONITEMLABEL) || (tvhti.flags & TVHT_ONITEMICON))
      {
        SendMessage(hWndBrowseTree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)tvhti.hItem);

        tvi.mask=TVIF_PARAM;
        tvi.hItem=tvhti.hItem;

        if (TreeView_GetItemWide(hWndBrowseTree, &tvi))
        {
          if (lptvid=(LPTVITEMDATA)tvi.lParam)
          {
            LPCONTEXTMENU pcm;
            HMENU hMenu;
            HRESULT hr;

            hr=lptvid->psfParent->lpVtbl->GetUIObjectOf(lptvid->psfParent, hDlg, 1, (LPCITEMIDLIST *)&lptvid->pidlItem, &IID_IContextMenu, 0, (void **)&pcm);

            if (SUCCEEDED(hr))
            {
              pcm->lpVtbl->QueryInterface(pcm, &IID_IContextMenu2, (void **)&pExplorerSubMenu2);
              pcm->lpVtbl->QueryInterface(pcm, &IID_IContextMenu3, (void **)&pExplorerSubMenu3);

              if (hMenu=CreatePopupMenu())
              {
                hr=pcm->lpVtbl->QueryContextMenu(pcm, hMenu, 0, 1, 0x7fff, CMF_EXPLORE);

                if (SUCCEEDED(hr))
                {
                  nCmd=TrackPopupMenu(hMenu, TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, 0, hDlg, NULL);

                  if (nCmd)
                  {
                    CMINVOKECOMMANDINFO ici;

                    xmemset(&ici, 0, sizeof(CMINVOKECOMMANDINFO));
                    ici.cbSize=sizeof(CMINVOKECOMMANDINFO);
                    ici.hwnd=hDlg;
                    ici.lpVerb=MAKEINTRESOURCEA(nCmd - 1);
                    ici.nShow=SW_SHOWNORMAL;

                    if (pcm->lpVtbl->InvokeCommand(pcm, &ici) == NOERROR)
                    {
                      if (SHGetPathFromIDListWide(lptvid->pidlFullPath, wszPath))
                      {
                        TrimTrailBackSlash(wszPath);

                        if (GetFileAttributesWide(wszPath) == 0xFFFFFFFF)
                        {
                          SendMessage(hWndBrowseTree, TVM_DELETEITEM, 0, (LPARAM)tvhti.hItem);
                        }
                      }
                    }
                  }
                }
                DestroyMenu(hMenu);
              }
              if (pExplorerSubMenu3) pExplorerSubMenu3=NULL;
              if (pExplorerSubMenu2) pExplorerSubMenu2=NULL;
              pcm->lpVtbl->Release(pcm);
            }
          }
        }
      }
      else
      {
        nCmd=TrackPopupMenu(hMenuBrowse, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, 0, hDlg, NULL);

        if (nCmd == IDM_RENAME)
          PostMessage(hDlg, AKDLL_RENAME, 0, 0);
        else if (nCmd == IDM_FINDDOCUMENT)
          PostMessage(hDlg, AKDLL_FINDDOCUMENT, 0, 0);
        else if (nCmd == IDM_REFRESH)
          PostMessage(hDlg, AKDLL_REFRESH, 0, 0);
        else if (nCmd == IDM_CREATEDIR)
          PostMessage(hDlg, AKDLL_CREATEDIR, 0, 0);
        else if (nCmd == IDM_CREATEFILE)
          PostMessage(hDlg, AKDLL_CREATEFILE, 0, 0);
        else if (nCmd == IDM_SETUP)
          PostMessage(hDlg, AKDLL_SETUP, 0, 0);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_BROWSE_FILTERTYPE)
    {
      BUTTONDRAW bd;
      RECT rc;
      int nCmd;

      GetWindowRect(hWndFilterType, &rc);
      if (nCmd=TrackPopupMenu(hMenuFilterType, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, rc.left, rc.top + (rc.bottom - rc.top), 0, hDlg, NULL))
      {
        //Set icon
        bd.dwFlags=BIF_ICON|BIF_ENABLEFOCUS;
        if (nCmd == IDM_FILTERNONE)
        {
          nFilterType=FILTER_NONE;
          bd.hImage=hIconFilterNone;
        }
        else if (nCmd == IDM_FILTERINCLUDE)
        {
          nFilterType=FILTER_INCLUDE;
          bd.hImage=hIconFilterIncl;
        }
        else if (nCmd == IDM_FILTEREXCLUDE)
        {
          nFilterType=FILTER_EXCLUDE;
          bd.hImage=hIconFilterExcl;
        }
        bd.nImageWidth=16;
        bd.nImageHeight=16;
        SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndFilterType, (LPARAM)&bd);
        InvalidateRect(hWndFilterType, NULL, TRUE);
        UpdateWindow(hWndFilterType);

        //Set radiobutton
        CheckMenuRadioItem(hMenuFilterType, IDM_FILTERNONE, IDM_FILTEREXCLUDE, IDM_FILTERNONE + nFilterType, MF_BYCOMMAND);

        FilterComboboxFill(hWndFilterCombo);
        EnableWindow(hWndFilterCombo, nFilterType);
        EnableWindow(hWndFilterItem, nFilterType && GetWindowTextLengthWide(hWndFilterCombo));

        PostMessage(hWndDockDlg, AKDLL_REFRESH, 0, 0);
        if (bAutoFind)
          PostMessage(hWndDockDlg, AKDLL_FINDDOCUMENT, 0, 0);

        dwSaveFlags|=OF_FILTER;
      }
    }
    else if (LOWORD(wParam) == IDC_BROWSE_FILTERCOMBO)
    {
      if (HIWORD(wParam) == CBN_EDITCHANGE ||
          HIWORD(wParam) == CBN_SELCHANGE)
      {
        FILTERGROUP *lpFilterGroup=NULL;
        int nCurSel;

        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
          nCurSel=(int)SendMessage(hWndFilterCombo, CB_GETCURSEL, 0, 0);
          ComboBox_GetLBTextWide(hWndFilterCombo, nCurSel, wszBuffer);
        }
        else GetWindowTextWide(hWndFilterCombo, wszBuffer, BUFFER_SIZE);

        if (nFilterType == FILTER_INCLUDE)
          lpFilterGroup=hFilterIncludeStack.first;
        else if (nFilterType == FILTER_EXCLUDE)
          lpFilterGroup=hFilterExcludeStack.first;

        FilterStringToGroup(wszBuffer, lpFilterGroup);
        EnableWindow(hWndFilterItem, wszBuffer[0]);

        PostMessage(hWndDockDlg, AKDLL_REFRESH, 0, 0);
        if (bAutoFind)
          PostMessage(hWndDockDlg, AKDLL_FINDDOCUMENT, 0, 0);

        dwSaveFlags|=OF_FILTER;
      }
    }
    else if (LOWORD(wParam) == IDC_BROWSE_FILTERITEM)
    {
      HFILTERGROUPSTACK *hStack=NULL;
      FILTERGROUP *lpFilterGroup=NULL;
      RECT rc;
      int nCmd;
      int nIndex;
      int nStackItem;

      GetWindowRect(hWndFilterItem, &rc);

      if (GetWindowTextWide(hWndFilterCombo, wszBuffer, BUFFER_SIZE))
      {
        if (nFilterType == FILTER_INCLUDE)
          hStack=&hFilterIncludeStack;
        else if (nFilterType == FILTER_EXCLUDE)
          hStack=&hFilterExcludeStack;

        nIndex=(int)ComboBox_FindStringExactWide(hWndFilterCombo, 0, wszBuffer);
        EnableMenuItem(hMenuFilterItem, IDM_FILTERITEMADD, (nIndex == CB_ERR)?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuFilterItem, IDM_FILTERITEMMOVEUP, (nIndex != CB_ERR && nIndex > 0)?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuFilterItem, IDM_FILTERITEMMOVEDOWN, (nIndex != CB_ERR && nIndex < hStack->nElements - 2)?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenuFilterItem, IDM_FILTERITEMREMOVE, (nIndex != CB_ERR)?MF_ENABLED:MF_GRAYED);

        if (nCmd=TrackPopupMenu(hMenuFilterItem, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, rc.left, rc.top + (rc.bottom - rc.top), 0, hDlg, NULL))
        {
          //First item in stack is the current filter
          nStackItem=nIndex + 2;

          if (nCmd == IDM_FILTERITEMADD)
          {
            if (lpFilterGroup=StackInsertGroup(hStack))
            {
              if (!FilterStringToGroup(wszBuffer, lpFilterGroup))
                StackDeleteGroup(hStack, (void *)lpFilterGroup);
            }
          }
          else if (nCmd == IDM_FILTERITEMMOVEUP)
          {
            if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&lpFilterGroup, nStackItem))
              StackMoveIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpFilterGroup, nStackItem - 1);
          }
          else if (nCmd == IDM_FILTERITEMMOVEDOWN)
          {
            if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&lpFilterGroup, nStackItem))
              StackMoveIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpFilterGroup, nStackItem + 1);
          }
          else if (nCmd == IDM_FILTERITEMREMOVE)
          {
            if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&lpFilterGroup, nStackItem))
              StackDeleteGroup(hStack, (void *)lpFilterGroup);
          }
          FilterComboboxFill(hWndFilterCombo);

          dwSaveFlags|=OF_FILTER;
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (!lParam)
      {
        if (bLabelEditing)
        {
          SendMessage(hWndBrowseTree, TVM_ENDEDITLABELNOW, FALSE, 0);
        }
        else
        {
          HWND hWndFocus=GetFocus();

          if (hWndFocus == hWndBrowsePath)
          {
            if (GetWindowTextWide(hWndBrowsePath, wszPath, MAX_PATH))
            {
              SearchTreeView(hWndBrowseTree, wszPath);
            }
          }
          else if (hWndFocus == hWndBrowseTree)
          {
            OpenSelItemTreeView(hWndBrowseTree);
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_CLOSE ||
             LOWORD(wParam) == IDCANCEL)
    {
      if (bLabelEditing && LOWORD(wParam) == IDCANCEL)
      {
        SendMessage(hWndBrowseTree, TVM_ENDEDITLABELNOW, TRUE, 0);
      }
      else
      {
        //Send TVN_DELETEITEM to free items data
        ClearTreeView(hWndBrowseTree, FALSE);

        if (dwSaveFlags)
        {
          SaveOptions(dwSaveFlags);
          dwSaveFlags=0;
        }

        UninitMain();
        SendMessage(hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkExplorerDlg);
        dkExplorerDlg=NULL;

        StackFreeGroup(&hFilterIncludeStack);
        if (wszFilterInclude)
        {
          GlobalFree((HGLOBAL)wszFilterInclude);
          wszFilterInclude=NULL;
        }

        StackFreeGroup(&hFilterExcludeStack);
        if (wszFilterExclude)
        {
          GlobalFree((HGLOBAL)wszFilterExclude);
          wszFilterExclude=NULL;
        }

        DestroyIcon(hIconFilterNone);
        DestroyIcon(hIconFilterIncl);
        DestroyIcon(hIconFilterExcl);
        DestroyMenu(hMenuFilterItem);
        DestroyMenu(hMenuFilterType);
        DestroyMenu(hMenuBrowse);
        DestroyWindow(hWndDockDlg);
        hWndDockDlg=NULL;
        hWndBrowseTree=NULL;

        if (!(lParam & DKT_ONMAINFINISH))
        {
          SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
          SetFocus(hMainWnd);

          if (!(lParam & DKT_KEEPAUTOLOAD))
          {
            pfExplorer->bAutoLoad=FALSE;
            SendMessage(hMainWnd, AKD_DLLSAVE, DLLSF_ONEXIT, 0);
          }
          if (!(lParam & DKT_NOUNLOAD))
            SendMessage(hMainWnd, AKD_DLLUNLOAD, (WPARAM)hInstanceDLL, (LPARAM)NULL);
        }
      }
    }
  }

  //Explorer menu
  if (pExplorerSubMenu3)
  {
    LRESULT lResult;

    if (SUCCEEDED(pExplorerSubMenu3->lpVtbl->HandleMenuMsg2(pExplorerSubMenu3, uMsg, wParam, lParam, &lResult)))
      return (BOOL)lResult;
  }
  else if (pExplorerSubMenu2)
  {
    if (SUCCEEDED(pExplorerSubMenu2->lpVtbl->HandleMenuMsg(pExplorerSubMenu2, uMsg, wParam, lParam)))
      return 0;
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], NULL, &rcExplorerCurrentDialog, 0, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      if (dkExplorerDlg) GetWindowSize(hWndTitleText, hDlg, &dkExplorerDlg->rcDragDrop);
  }

  return FALSE;
}

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndRootDirectoryEdit;
  static HWND hWndRootDirectoryBrowse;
  static HWND hWndRootMyComputer;
  static HWND hWndShowHidden;
  static HWND hWndAutoFind;
  static HWND hWndSingleClick;
  static HWND hWndSetSaveLocation;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndRootDirectoryEdit=GetDlgItem(hDlg, IDC_SETUP_ROOTDIRECTORY_EDIT);
    hWndRootDirectoryBrowse=GetDlgItem(hDlg, IDC_SETUP_ROOTDIRECTORY_BROWSE);
    hWndRootMyComputer=GetDlgItem(hDlg, IDC_SETUP_ROOTMYCOMPUTER);
    hWndShowHidden=GetDlgItem(hDlg, IDC_SETUP_SHOWHIDDEN);
    hWndAutoFind=GetDlgItem(hDlg, IDC_SETUP_AUTOFIND);
    hWndSingleClick=GetDlgItem(hDlg, IDC_SETUP_SINGLECLICK);
    hWndSetSaveLocation=GetDlgItem(hDlg, IDC_SETUP_SETSAVELOCATION);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_SETUP_ROOT_GROUP, GetLangStringW(wLangModule, STRID_ROOTDIRECTORY));
    SetDlgItemTextWide(hDlg, IDC_SETUP_ROOTMYCOMPUTER, GetLangStringW(wLangModule, STRID_MYCOMPUTER));
    SetDlgItemTextWide(hDlg, IDC_SETUP_SHOWHIDDEN, GetLangStringW(wLangModule, STRID_SHOWHIDDEN));
    SetDlgItemTextWide(hDlg, IDC_SETUP_AUTOFIND, GetLangStringW(wLangModule, STRID_AUTOFIND));
    SetDlgItemTextWide(hDlg, IDC_SETUP_SINGLECLICK, GetLangStringW(wLangModule, STRID_SINGLECLICK));
    SetDlgItemTextWide(hDlg, IDC_SETUP_SETSAVELOCATION, GetLangStringW(wLangModule, STRID_SETSAVELOCATION));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndRootDirectoryEdit, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    SetWindowTextWide(hWndRootDirectoryEdit, wszRootDirectory);
    if (nRootSpecial) SendMessage(hWndRootMyComputer, BM_SETCHECK, BST_CHECKED, 0);
    if (bShowHidden) SendMessage(hWndShowHidden, BM_SETCHECK, BST_CHECKED, 0);
    if (bAutoFind) SendMessage(hWndAutoFind, BM_SETCHECK, BST_CHECKED, 0);
    if (bSingleClick) SendMessage(hWndSingleClick, BM_SETCHECK, BST_CHECKED, 0);
    if (bSetSaveLocation) SendMessage(hWndSetSaveLocation, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hDlg, WM_COMMAND, IDC_SETUP_ROOTMYCOMPUTER, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SETUP_ROOTDIRECTORY_BROWSE)
    {
      BROWSEINFOW bi;
      LPITEMIDLIST pIdList;
      LPMALLOC pMalloc;
      wchar_t wszDir[MAX_PATH];

      GetWindowTextWide(hWndRootDirectoryEdit, wszDir, MAX_PATH);
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
        SetWindowTextWide(hWndRootDirectoryEdit, wszDir);
      }
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_SETUP_ROOTMYCOMPUTER)
    {
      bState=(BOOL)SendMessage(hWndRootMyComputer, BM_GETCHECK, 0, 0);
      EnableWindow(hWndRootDirectoryEdit, !bState);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      DWORD dwStyle;

      GetWindowTextWide(hWndRootDirectoryEdit, wszRootDirectory, MAX_PATH);
      if (SendMessage(hWndRootMyComputer, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nRootSpecial=CSIDL_DRIVES;
      else
        nRootSpecial=CSIDL_DESKTOP;

      bShowHidden=(BOOL)SendMessage(hWndShowHidden, BM_GETCHECK, 0, 0);
      bAutoFind=(BOOL)SendMessage(hWndAutoFind, BM_GETCHECK, 0, 0);
      bState=(BOOL)SendMessage(hWndSingleClick, BM_GETCHECK, 0, 0);
      if (bState != bSingleClick)
      {
        bSingleClick=bState;
        dwStyle=(DWORD)GetWindowLongPtrWide(hWndBrowseTree, GWL_STYLE);
        SetWindowLongPtrWide(hWndBrowseTree, GWL_STYLE, bSingleClick?(dwStyle | TVS_TRACKSELECT):(dwStyle & ~TVS_TRACKSELECT));
      }
      bSetSaveLocation=(BOOL)SendMessage(hWndSetSaveLocation, BM_GETCHECK, 0, 0);

      dwSaveFlags|=OF_SETTINGS;

      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      EndDialog(hDlg, 0);
      PostMessage(hWndDockDlg, AKDLL_REFRESH, 0, 0);
      if (bAutoFind)
        PostMessage(hWndDockDlg, AKDLL_FINDDOCUMENT, 0, 0);
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

BOOL CALLBACK InputBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    if (lParam == AKDLL_CREATEDIR)
      SetDlgItemTextWide(hDlg, IDC_INPUTBOX_LABEL, GetLangStringW(wLangModule, STRID_CREATEDIR));
    else if (lParam == AKDLL_CREATEFILE)
      SetDlgItemTextWide(hDlg, IDC_INPUTBOX_LABEL, GetLangStringW(wLangModule, STRID_CREATEFILE));
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      GetDlgItemTextWide(hDlg, IDC_INPUTBOX_EDIT, wszInputBox, MAX_PATH);

      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      wszInputBox[0]=L'\0';
      EndDialog(hDlg, 0);
      return TRUE;
    }
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
  static BOOL bFindDoc;

  if (uMsg == AKDN_MAIN_ONSTART_IDLE)
  {
    if (bOnMainStart) PostMessage(hWndDockDlg, AKDLL_INIT, 0, 0);
  }
  else if (uMsg == AKDN_DOCK_RESIZE)
  {
    if (((DOCK *)wParam)->hWnd == dkExplorerDlg->hWnd)
      dwSaveFlags|=OF_RECT;
  }
  else if (uMsg == AKDN_OPENDOCUMENT_START)
  {
    NOPENDOCUMENT *nod=(NOPENDOCUMENT *)lParam;

    if (!(*nod->dwFlags & OD_NOUPDATE))
      bFindDoc=TRUE;
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_START)
  {
    NSAVEDOCUMENT *nsd=(NSAVEDOCUMENT *)lParam;
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (nsd->dwFlags & SD_UPDATE)
    {
      if (xstrcmpiW(lpFrame->wszFile, nsd->wszFile))
        bFindDoc=TRUE;
    }
  }
  else if (uMsg == AKDN_POSTDOCUMENT_FINISH ||
           (uMsg == AKDN_FRAME_ACTIVATE && !(wParam & FWA_NOVISUPDATE)))
  {
    if (uMsg == AKDN_POSTDOCUMENT_FINISH && !bFindDoc)
    {
      //Don't process
    }
    else
    {
      if (bAutoFind)
        PostMessage(hWndDockDlg, AKDLL_FINDDOCUMENT, 0, 0);
      bFindDoc=FALSE;
    }
  }
  else if (uMsg == AKDN_SIZE_ONSTART)
  {
    if (bExplorerDockWaitResize)
    {
      bExplorerDockWaitResize=FALSE;
      ShowWindow(hWndDockDlg, SW_SHOW);
      dkExplorerDlg->dwFlags&=~DKF_HIDDEN;
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

    if (hWndDockDlg)
    {
      DestroyDock(hWndDockDlg, DKT_ONMAINFINISH);
    }
    return FALSE;
  }
  else if (uMsg == WM_COMMAND)
  {
    if (bSetSaveLocation)
    {
      if ((LOWORD(wParam) == IDM_FILE_SAVE && !SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_FILELEN, (LPARAM)NULL)) ||
          LOWORD(wParam) == IDM_FILE_SAVEAS)
      {
        if (GetWindowTextWide(hWndBrowsePath, wszPath, MAX_PATH))
          SendMessage(hMainWnd, AKD_SETMAININFO, MIS_OFNDIR, (LPARAM)wszPath);
      }
    }
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

void ClearTreeView(HWND hWndTreeView, BOOL bRedraw)
{
  SendMessage(hWndTreeView, WM_SETREDRAW, FALSE, 0);
  SendMessage(hWndTreeView, TVM_DELETEITEM, 0, (LPARAM)NULL);
  SendMessage(hWndTreeView, WM_SETREDRAW, TRUE, 0);
  if (bRedraw) InvalidateRect(hWndTreeView, NULL, TRUE);
}

void PopulateTree(HWND hWnd, int nRootSpecial, const wchar_t *wpRootDirectory)
{
  LPSHELLFOLDER psfDesktopFolder=NULL;
  LPSHELLFOLDER psfFolder=NULL;
  LPSHELLFOLDER psfRoot=NULL;
  LPITEMIDLIST pidlFolder=NULL;
  TVSORTCB tvscb;

  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SendMessage(hWnd, TVM_DELETEITEM, 0, (LPARAM)NULL);

  if (SHGetDesktopFolder(&psfDesktopFolder) == NOERROR)
  {
    if (nRootSpecial)
    {
      if (SHGetSpecialFolderLocation(hWnd, nRootSpecial, &pidlFolder) == S_OK)
      {
        psfDesktopFolder->lpVtbl->BindToObject(psfDesktopFolder, pidlFolder, NULL, &IID_IShellFolder, (void **)&psfFolder);
        psfRoot=psfFolder;
      }
    }
    else if (*wpRootDirectory)
    {
      if (psfDesktopFolder->lpVtbl->ParseDisplayName(psfDesktopFolder, hWnd, NULL, (wchar_t *)wpRootDirectory, NULL, &pidlFolder, NULL) == S_OK)
      {
        psfDesktopFolder->lpVtbl->BindToObject(psfDesktopFolder, pidlFolder, NULL, &IID_IShellFolder, (void **)&psfFolder);
        psfRoot=psfFolder;
      }
    }
    else psfRoot=psfDesktopFolder;

    if (psfRoot)
    {
      FillTreeView(hWnd, psfRoot, pidlFolder, TVI_ROOT);

      tvscb.hParent=TVI_ROOT;
      tvscb.lParam=0;
      tvscb.lpfnCompare=TreeViewCompareProc;
      SendMessage(hWnd, TVM_SORTCHILDRENCB, 0, (LPARAM)&tvscb);

      if (pidlFolder) lpMalloc->lpVtbl->Free(lpMalloc, pidlFolder);
      if (psfFolder) psfFolder->lpVtbl->Release(psfFolder);
    }
    psfDesktopFolder->lpVtbl->Release(psfDesktopFolder);
  }
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
}

void FillTreeView(HWND hWnd, LPSHELLFOLDER psfRoot, LPITEMIDLIST pidlRoot, HTREEITEM hParent)
{
  wchar_t wszName[MAX_PATH];
  wchar_t wszPath[MAX_PATH];
  HWND hWndParent;
  FILTERGROUP *lpFilterGroup=NULL;
  LPENUMIDLIST pedl=NULL;
  LPITEMIDLIST pidlEnum=NULL;
  LPTVITEMDATA lptvid=NULL;
  ULONG ulFetched;
  ULONG ulAttrs;
  ULONG ulState;
  TVINSERTSTRUCTW tvis;
  TVITEMW tvi;
  HTREEITEM hPrev=NULL;
  int nItem;
  BOOL bFound=-1;

  hWndParent=GetParent(hWnd);
  if (nFilterType == FILTER_INCLUDE)
    lpFilterGroup=hFilterIncludeStack.first;
  else if (nFilterType == FILTER_EXCLUDE)
    lpFilterGroup=hFilterExcludeStack.first;

  if (psfRoot->lpVtbl->EnumObjects(psfRoot, hWndParent, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|(bShowHidden?SHCONTF_INCLUDEHIDDEN:0), &pedl) == S_OK)
  {
    while (pedl->lpVtbl->Next(pedl, 1, &pidlEnum, &ulFetched) == S_OK)
    {
      ulAttrs=SFGAO_FOLDER|SFGAO_FILESYSTEM|SFGAO_SHARE|SFGAO_LINK|SFGAO_GHOSTED|SFGAO_STREAM;

      if (psfRoot->lpVtbl->GetAttributesOf(psfRoot, 1, (LPCITEMIDLIST *)&pidlEnum, &ulAttrs) == S_OK)
      {
        if (lptvid=(LPTVITEMDATA)lpMalloc->lpVtbl->Alloc(lpMalloc, sizeof(TVITEMDATA)))
        {
          if (GetItemName(psfRoot, pidlEnum, ((ulAttrs & SFGAO_FOLDER) || !(ulAttrs & SFGAO_FILESYSTEM))?SHGDN_NORMAL:(SHGDN_INFOLDER|SHGDN_FORPARSING), wszName, MAX_PATH))
          {
            if (nFilterType)
            {
              if (!(ulAttrs & SFGAO_FOLDER) ||
                   (ulAttrs & SFGAO_STREAM)) //.zip
              {
                bFound=FALSE;

                if (lpFilterGroup)
                {
                  for (nItem=0; lpFilterGroup->lpItems[nItem]; ++nItem)
                  {
                    if (FileMaskCmp(lpFilterGroup->lpItems[nItem], wszName))
                    {
                      bFound=TRUE;
                      break;
                    }
                  }
                }
              }
              else bFound=-1;
            }

            if (bFound == -1 || (nFilterType == FILTER_INCLUDE && bFound == TRUE) || (nFilterType == FILTER_EXCLUDE && bFound == FALSE))
            {
              psfRoot->lpVtbl->AddRef(psfRoot);
              lptvid->psfParent=psfRoot;
              lptvid->pidlFullPath=ConcatPIDL(pidlRoot, pidlEnum);
              lptvid->pidlItem=CopyPIDL(pidlEnum);

              ulState=0;
              if (ulAttrs & SFGAO_SHARE)
                ulState|=INDEXTOOVERLAYMASK(1);
              if (ulAttrs & SFGAO_LINK)
                ulState|=INDEXTOOVERLAYMASK(2);
              if (ulAttrs & SFGAO_GHOSTED)
                ulState|=TVIS_CUT;

              tvi.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM|TVIF_STATE;
              tvi.state=ulState;
              tvi.stateMask=TVIS_OVERLAYMASK|TVIS_CUT;
              tvi.pszText=wszName;
              tvi.cchTextMax=MAX_PATH;
              tvi.iImage=GetItemIcon(lptvid->pidlFullPath, SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
              tvi.iSelectedImage=GetItemIcon(lptvid->pidlFullPath, SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON);
              tvi.lParam=(LPARAM)lptvid;

              if ((ulAttrs & SFGAO_FOLDER) &&
                 !(ulAttrs & SFGAO_STREAM)) //not .zip
              {
                if (!(ulAttrs & SFGAO_FILESYSTEM))
                {
                  tvi.cChildren=1;
                  tvi.mask|=TVIF_CHILDREN;
                }
                else
                {
                  if (SHGetPathFromIDListWide(lptvid->pidlFullPath, wszPath))
                  {
                    TrimTrailBackSlash(wszPath);

                    if (wszPath[xstrlenW(wszPath) - 1] == ':' || !IsDirEmpty(wszPath))
                    {
                      tvi.cChildren=1;
                      tvi.mask|=TVIF_CHILDREN;
                    }
                  }
                }
              }

              xmemcpy(&tvis.item, &tvi, sizeof(TVITEMW));
              tvis.hInsertAfter=hPrev;
              tvis.hParent=hParent;
              hPrev=(HTREEITEM)TreeView_InsertItemWide(hWnd, &tvis);
            }
          }
        }
        else break;
      }
      lpMalloc->lpVtbl->Free(lpMalloc, pidlEnum);
      pidlEnum=NULL;
    }
    if (pidlEnum) lpMalloc->lpVtbl->Free(lpMalloc, pidlEnum);
    pedl->lpVtbl->Release(pedl);
  }
}

void SearchTreeView(HWND hWnd, const wchar_t *wpPath)
{
  LPSHELLFOLDER pDesktopFolder=NULL;
  LPITEMIDLIST pidlInitial;
  LPITEMIDLIST pidlNext;
  LPITEMIDLIST pidlTemp=NULL;
  LPTVITEMDATA lptvid;
  HTREEITEM hNextItem;
  HTREEITEM hSelItem=NULL;
  HTREEITEM hParentItem;
  HTREEITEM hChildItem;
  TVITEMW tvi;
  USHORT uNextSize=0;
  BOOL bUpdated=FALSE;
  BOOL bRoot=TRUE;

  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

  if (SHGetDesktopFolder(&pDesktopFolder) == NOERROR)
  {
    if (pDesktopFolder->lpVtbl->ParseDisplayName(pDesktopFolder, hWnd, NULL, (wchar_t *)wpPath, NULL, &pidlInitial, NULL) == S_OK)
    {
      pidlNext=pidlInitial;
      hNextItem=(HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_ROOT, (LPARAM)NULL);

      Loop:
      while (pidlNext->mkid.cb && hNextItem)
      {
        tvi.mask=TVIF_PARAM|TVIF_STATE;
        tvi.hItem=hNextItem;
        tvi.stateMask=TVIS_EXPANDEDONCE;

        if (TreeView_GetItemWide(hWnd, &tvi))
        {
          if (lptvid=(LPTVITEMDATA)tvi.lParam)
          {
            if (bRoot ? (INT_PTR)(pidlTemp=ComparePIDL(pidlNext, lptvid->pidlFullPath)) :
                        !HRESULT_CODE(lptvid->psfParent->lpVtbl->CompareIDs(lptvid->psfParent, 0, lptvid->pidlItem, pidlNext)))
            {
              if (!(tvi.state & TVIS_EXPANDEDONCE))
                SendMessage(hWnd, TVM_EXPAND, TVE_EXPAND, (LPARAM)hNextItem);

              if (hChildItem=(HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hNextItem))
              {
                hSelItem=hNextItem;
                hNextItem=hChildItem;

                if (bRoot)
                {
                  bRoot=FALSE;
                  pidlNext=pidlTemp;
                  uNextSize=CutPIDL(pidlNext);
                }
                else
                {
                  //Cut PIDL for CompareIDs to compare only first item
                  pidlNext=NextPIDL(pidlNext);
                  pidlNext->mkid.cb=uNextSize;
                  uNextSize=CutPIDL(pidlNext);
                }
                continue;
              }
              if (!bRoot)
              {
                pidlTemp=NextPIDL(pidlNext);
                pidlTemp->mkid.cb=uNextSize;
                if (!pidlTemp->mkid.cb)
                {
                  pidlNext=pidlTemp;
                  hSelItem=hNextItem;
                }
              }
              else hSelItem=hNextItem;

              break;
            }
          }
        }
        hNextItem=(HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hNextItem);
      }
      if (!bUpdated && !bRoot && pidlNext->mkid.cb)
      {
        //Folder needs updating.
        bUpdated=TRUE;
        SendMessage(hWnd, TVM_EXPAND, TVE_COLLAPSE|TVE_COLLAPSERESET, (LPARAM)hSelItem);
        SendMessage(hWnd, TVM_EXPAND, TVE_EXPAND, (LPARAM)hSelItem);
        if (hNextItem=(HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hSelItem))
        {
          uNextSize=CutPIDL(pidlNext);
          goto Loop;
        }
      }
      if (hSelItem)
      {
        if (hSelItem != (HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0))
        {
          if (hParentItem=(HTREEITEM)SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hSelItem))
            SendMessage(hWnd, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM)hParentItem);
          SendMessage(hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hSelItem);
          SendMessage(hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM)hSelItem);
        }
      }
      if (pidlInitial) lpMalloc->lpVtbl->Free(lpMalloc, pidlInitial);
    }
    pDesktopFolder->lpVtbl->Release(pDesktopFolder);
  }
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
}

HTREEITEM GetSelItemTreeView(HWND hWnd, wchar_t *wszPath)
{
  LPTVITEMDATA lptvid;
  TVITEMW tvi;
  HTREEITEM hSelItem;

  if (hSelItem=(HTREEITEM)SendMessage(hWndBrowseTree, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL))
  {
    tvi.mask=TVIF_PARAM;
    tvi.hItem=hSelItem;

    if (TreeView_GetItemWide(hWndBrowseTree, &tvi))
    {
      if (lptvid=(LPTVITEMDATA)tvi.lParam)
      {
        if (SHGetPathFromIDListWide(lptvid->pidlFullPath, wszPath))
        {
          return hSelItem;
        }
      }
    }
  }
  return NULL;
}

void OpenSelItemTreeView(HWND hWnd)
{
  if (GetSelItemTreeView(hWnd, wszPath))
  {
    TrimTrailBackSlash(wszPath);

    if (!(GetFileAttributesWide(wszPath) & FILE_ATTRIBUTE_DIRECTORY))
    {
      OpenDocument(wszPath);
      SetFocus(hWnd);
    }
  }
}

int CALLBACK TreeViewCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  LPTVITEMDATA lptvid1=(LPTVITEMDATA)lParam1;
  LPTVITEMDATA lptvid2=(LPTVITEMDATA)lParam2;
  HRESULT hr;

  hr=lptvid1->psfParent->lpVtbl->CompareIDs(lptvid1->psfParent, 0, lptvid1->pidlItem, lptvid2->pidlItem);

  return (short)HRESULT_CODE(hr);
}

BOOL GetItemName(LPSHELLFOLDER psf, LPITEMIDLIST pidl, DWORD dwFlags, wchar_t *wszName, int nMaxName)
{
  STRRET sr;
  BOOL bResult=TRUE;

  if (psf->lpVtbl->GetDisplayNameOf(psf, pidl, dwFlags, &sr) == NOERROR)
  {
    if (sr.uType == STRRET_WSTR)
    {
      xstrcpynW(wszName, sr.pOleStr, nMaxName);
    }
    else if (sr.uType == STRRET_OFFSET)
    {
      MultiByteToWideChar(CP_ACP, 0, (char *)(((unsigned char *)pidl) + sr.uOffset), -1, wszName, nMaxName);
    }
    else if (sr.uType == STRRET_CSTR)
    {
      MultiByteToWideChar(CP_ACP, 0, sr.cStr, -1, wszName, nMaxName);
    }
    else bResult=FALSE;
  }
  else bResult=FALSE;

  return bResult;
}

int GetItemIcon(LPITEMIDLIST pidl, UINT uFlags)
{
  if (bOldWindows)
  {
    SHFILEINFOA sfiA;

    SHGetFileInfoAPtr((char *)pidl, 0, &sfiA, sizeof(SHFILEINFOA), uFlags);
    return sfiA.iIcon;
  }
  else
  {
    SHFILEINFOW sfiW;

    SHGetFileInfoWPtr((wchar_t *)pidl, 0, &sfiW, sizeof(SHFILEINFOW), uFlags);
    return sfiW.iIcon;
  }
}

LPITEMIDLIST CreatePIDL(UINT cbSize)
{
  LPITEMIDLIST pidl=NULL;

  if (cbSize)
  {
    if (pidl=(LPITEMIDLIST)lpMalloc->lpVtbl->Alloc(lpMalloc, cbSize))
    {
      xmemset(pidl, 0, cbSize);
    }
  }
  return pidl;
}

LPITEMIDLIST NextPIDL(LPCITEMIDLIST pidl)
{
  return (LPITEMIDLIST)((unsigned char *)pidl + pidl->mkid.cb);
}

USHORT CutPIDL(LPITEMIDLIST pidl)
{
  USHORT uNextSize;

  pidl=NextPIDL(pidl);
  uNextSize=pidl->mkid.cb;
  pidl->mkid.cb=0;
  return uNextSize;
}

UINT GetSizePIDL(LPCITEMIDLIST pidl)
{
  UINT cbTotal=0;

  if (pidl)
  {
    while (pidl->mkid.cb)
    {
      cbTotal+=pidl->mkid.cb;
      pidl=NextPIDL(pidl);
    }
    cbTotal+=sizeof(USHORT);
  }
  return cbTotal;
}

LPITEMIDLIST CopyPIDL(LPCITEMIDLIST pidl)
{
  LPITEMIDLIST pidlNew;
  UINT cb;

  cb=GetSizePIDL(pidl);

  if (pidlNew=CreatePIDL(cb))
  {
    xmemcpy(pidlNew, pidl, cb);
  }
  return pidlNew;
}

LPITEMIDLIST ConcatPIDL(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
  LPITEMIDLIST pidlNew=NULL;
  UINT cb1;
  UINT cb2;

  cb1=pidl1?GetSizePIDL(pidl1) - sizeof(USHORT):0;
  cb2=pidl2?GetSizePIDL(pidl2) - sizeof(USHORT):0;

  if (cb1 + cb2)
  {
    if (pidlNew=CreatePIDL(cb1 + cb2 + sizeof(USHORT)))
    {
      if (pidl1) xmemcpy(pidlNew, pidl1, cb1);
      if (pidl2) xmemcpy((unsigned char *)pidlNew + cb1, pidl2, cb2);
    }
  }
  return pidlNew;
}

BOOL CompareITEMID(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
  if (pidl1->mkid.cb == pidl2->mkid.cb)
  {
    if (!xmemcmp(pidl1, pidl2, pidl1->mkid.cb))
    {
      return TRUE;
    }
  }
  return FALSE;
}

LPITEMIDLIST ComparePIDL(LPCITEMIDLIST pidlFull, LPCITEMIDLIST pidlPart)
{
  LPITEMIDLIST pidl1=(LPITEMIDLIST)pidlFull;
  LPITEMIDLIST pidl2=(LPITEMIDLIST)pidlPart;

  while (pidl1->mkid.cb && pidl2->mkid.cb)
  {
    if (CompareITEMID(pidl1, pidl2))
    {
      pidl1=NextPIDL(pidl1);
      pidl2=NextPIDL(pidl2);
    }
    else return NULL;
  }
  if (pidl2->mkid.cb)
    return NULL;
  return pidl1;
}

void OpenDocument(const wchar_t *wpFile)
{
/*
  PMOPENDOCUMENT *pmod;

  if (*wpFile)
  {
    if (pmod=(PMOPENDOCUMENT *)GlobalAlloc(GPTR, sizeof(PMOPENDOCUMENT)))
    {
      xstrcpynW(pmod->szFile, wpFile, MAX_PATH);
      pmod->szWorkDir[0]='\0';

      pmod->odp.pFile=pmod->szFile;
      pmod->odp.pWorkDir=pmod->szWorkDir;
      pmod->odp.dwFlags=OD_ADT_BINARYERROR|OD_ADT_REGCODEPAGE;
      pmod->odp.nCodePage=0;
      pmod->odp.bBOM=0;
      pmod->odp.hDoc=NULL;

      //Post message
      pmod->pm.hWnd=hMainWnd;
      pmod->pm.uMsg=AKD_OPENDOCUMENTW;
      pmod->pm.wParam=(WPARAM)NULL;
      pmod->pm.lParam=(LPARAM)&pmod->odp;
      PostMessage(hMainWnd, AKD_POSTMESSAGE, 0, (LPARAM)pmod);
    }
  }
*/
  OPENDOCUMENTW od;

  od.pFile=wpFile;
  od.pWorkDir=NULL;
  od.dwFlags=OD_ADT_BINARYERROR|OD_ADT_REGCODEPAGE;
  od.nCodePage=0;
  od.bBOM=0;
  od.hDoc=NULL;
  SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);
}

FILTERGROUP* StackInsertGroup(HFILTERGROUPSTACK *hStack)
{
  FILTERGROUP *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(FILTERGROUP)))
  {
    ++hStack->nElements;
  }
  return lpElement;
}

void StackDeleteGroup(HFILTERGROUPSTACK *hStack, void *lpFilterGroup)
{
  if (!StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpFilterGroup))
  {
    --hStack->nElements;
  }
}

void StackFreeGroup(HFILTERGROUPSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}

void FilterComboboxFill(HWND hWnd)
{
  SetWindowTextWide(hWnd, L"");
  SendMessage(hWnd, CB_RESETCONTENT, 0, 0);

  if (nFilterType != FILTER_NONE)
  {
    HFILTERGROUPSTACK *hStack=NULL;
    FILTERGROUP *lpFilterGroup;

    if (nFilterType == FILTER_INCLUDE)
      hStack=&hFilterIncludeStack;
    else if (nFilterType == FILTER_EXCLUDE)
      hStack=&hFilterExcludeStack;

    for (lpFilterGroup=hStack->first; lpFilterGroup; lpFilterGroup=lpFilterGroup->next)
    {
      if (FilterGroupToString(lpFilterGroup, wszBuffer, BUFFER_SIZE))
      {
        if (lpFilterGroup != hStack->first)
          ComboBox_AddStringWide(hWnd, wszBuffer);
        else
          SetWindowTextWide(hWnd, wszBuffer);
      }
    }
  }
}

int FilterStackToString(HFILTERGROUPSTACK *hStack, wchar_t *wszString, int nStringMax)
{
  FILTERGROUP *lpFilterGroup;
  int nStringLen=0;

  for (lpFilterGroup=hStack->first; lpFilterGroup; lpFilterGroup=lpFilterGroup->next)
  {
    nStringLen+=FilterGroupToString(lpFilterGroup, wszString?wszString + nStringLen:NULL, nStringMax - nStringLen);
    if (wszString) wszString[nStringLen++]=L'|';
  }
  if (wszString)
  {
    if (nStringLen) --nStringLen;
    wszString[nStringLen]=L'\0';
  }
  return nStringLen;
}

int FilterGroupToString(FILTERGROUP *lpFilterGroup, wchar_t *wszString, int nStringMax)
{
  const wchar_t *wpSource;
  wchar_t *wpTarget;
  wchar_t *wpTargetMax;

  if (!wszString)
    return (lpFilterGroup->nNameLen?lpFilterGroup->nNameLen + 2:0) + lpFilterGroup->nGroupLen + 1;
  wpTarget=wszString;
  wpTargetMax=wszString + nStringMax;

  if (lpFilterGroup->wszName[0])
  {
    wpTarget+=xstrcpynW(wpTarget, lpFilterGroup->wszName, nStringMax);
    if (wpTarget < wpTargetMax) *wpTarget++=L'(';
  }
  wpSource=lpFilterGroup->wszGroup;

  while (wpTarget < wpTargetMax)
  {
    if (!*wpSource)
    {
      if (!*(wpSource + 1))
        break;
      *wpTarget++=L';';
      ++wpSource;
    }
    else *wpTarget++=*wpSource++;
  }
  if (lpFilterGroup->wszName[0])
  {
    if (wpTarget < wpTargetMax) *wpTarget++=L')';
  }
  *wpTarget=L'\0';
  return (int)(wpTarget - wszString);
}

void FilterStringToStack(const wchar_t *wpString, HFILTERGROUPSTACK *hStack)
{
  FILTERGROUP *lpFilterGroup;

  do
  {
    if (lpFilterGroup=StackInsertGroup(hStack))
    {
      wpString+=FilterStringToGroup(wpString, lpFilterGroup);

      //Allow empty string only for the first element
      if (hStack->nElements > 1 && lpFilterGroup->nItemCount == 0)
        StackDeleteGroup(hStack, (void *)lpFilterGroup);
    }
  }
  while (*wpString);
}

int FilterStringToGroup(const wchar_t *wpString, FILTERGROUP *lpFilterGroup)
{
  const wchar_t *wpGroupStart=wpString;
  const wchar_t *wpGroupEnd;
  const wchar_t *wpGroupName;
  wchar_t *wpTarget;
  BOOL bItemStart=TRUE;

  //Ignore ';' at the beginning
  while (*wpGroupStart == L';') ++wpGroupStart;

  //Find group end
  for (wpGroupEnd=wpGroupStart; *wpGroupEnd != L'|' && *wpGroupEnd; ++wpGroupEnd);

  //Each mask of wszGroup is separated with '\0' and last mask is terminated with '\0\0'
  lpFilterGroup->wszName[0]=L'\0';
  lpFilterGroup->nNameLen=0;
  lpFilterGroup->wszGroup[0]=L'\0';
  lpFilterGroup->wszGroup[1]=L'\0';
  lpFilterGroup->nGroupLen=0;
  lpFilterGroup->nItemCount=0;

  //Name of the group
  if (wpGroupStart != wpGroupEnd)
  {
    if (*(wpGroupEnd - 1) == L')')
    {
      --wpGroupEnd;
      wpGroupName=wpGroupStart;

      while (wpGroupStart < wpGroupEnd)
      {
        if (*wpGroupStart == L'(')
        {
          lpFilterGroup->nNameLen=(int)xstrcpynW(lpFilterGroup->wszName, wpGroupName, wpGroupStart - wpGroupName + 1);
          ++wpGroupStart;
          break;
        }
        ++wpGroupStart;
      }
    }
  }

  //Group elements
  if (wpGroupStart != wpGroupEnd)
  {
    wpTarget=lpFilterGroup->wszGroup;

    //Replace ';' with '\0'
    for (;;)
    {
      if (*wpGroupStart == L';')
      {
        //Ignore mulitple ';' and spaces at the beginning of mask
        while (*++wpGroupStart == L';' || *wpGroupStart == L' ' || *wpGroupStart == L'\t');
        *wpTarget++=L'\0';
        bItemStart=TRUE;
      }
      else
      {
        if (bItemStart)
        {
          lpFilterGroup->lpItems[lpFilterGroup->nItemCount++]=wpTarget;
          bItemStart=FALSE;
        }
        *wpTarget++=*wpGroupStart++;
      }
      if (wpGroupStart >= wpGroupEnd)
      {
        *wpTarget++=L'\0';
        *wpTarget=L'\0';
        lpFilterGroup->nGroupLen=(int)(wpTarget - lpFilterGroup->wszGroup);
        break;
      }
    }
  }
  lpFilterGroup->lpItems[lpFilterGroup->nItemCount]=0;

  if (*wpGroupStart == L')') ++wpGroupStart;
  if (*wpGroupStart == L'|') ++wpGroupStart;
  return (int)(wpGroupStart - wpString);
}

BOOL FileMaskCmp(const wchar_t *wpMaskStr, const wchar_t *wpFileStr)
{
  const wchar_t *wpMemMask=NULL;
  const wchar_t *wpMemFile=NULL;

  while (*wpFileStr)
  {
    if (*wpMaskStr == L'*')
    {
      do
      {
        if (!*++wpMaskStr) return TRUE;
      }
      while (*wpMaskStr == L'*' || *wpMaskStr == L'?');

      while (WideCharLower(*wpMaskStr) != WideCharLower(*wpFileStr))
      {
        if (!*++wpFileStr) goto End;
      }
      if (*wpMaskStr == L'.' && *(wpMaskStr + 1) == L'\0')
        return !*(wpFileStr + 1);
      wpMemMask=wpMaskStr++;
      wpMemFile=wpFileStr++;
    }
    else if (*wpMaskStr == L'?' || WideCharLower(*wpMaskStr) == WideCharLower(*wpFileStr))
    {
      ++wpMaskStr;
      ++wpFileStr;
    }
    else
    {
      if (!wpMemMask) return FALSE;
      wpMaskStr=wpMemMask;
      wpFileStr=++wpMemFile;
    }
  }

  End:
  if ((*wpMaskStr == L'.' && *(wpMaskStr + 1) == L'\0') ||
      (*wpMaskStr == L'*' && *(wpMaskStr + 1) == L'\0') ||
      (*wpMaskStr == L'*' && *(wpMaskStr + 1) == L'.' && *(wpMaskStr + 2) == L'\0'))
    return TRUE;
  return !*wpMaskStr;
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

BOOL IsDirEmpty(const wchar_t *wpDir)
{
  wchar_t wszMask[MAX_PATH];
  WIN32_FIND_DATAW wfd;
  HANDLE hSearch;
  BOOL bResult=TRUE;

  xprintfW(wszMask, L"%s\\*.*", wpDir);

  if ((hSearch=FindFirstFileWide(wszMask, &wfd)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      if (wfd.cFileName[0] == '.' && (wfd.cFileName[1] == '\0' || (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0'))) continue;

      bResult=FALSE;
      break;
    }
    while (FindNextFileWide(hSearch, &wfd));

    FindClose(hSearch);
  }
  return bResult;
}

BOOL TrimTrailBackSlash(wchar_t *wszDir)
{
  int nLen;
  int i;

  nLen=(int)xstrlenW(wszDir);

  for (i=nLen - 1; i > 0 && wszDir[i] == '\\'; --i)
  {
    wszDir[i]=L'\0';
  }
  if (i < nLen - 1) return TRUE;
  return FALSE;
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
  int nSize;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&nExplorerDockSide, sizeof(DWORD));
    WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&rcExplorerDockRect, sizeof(RECT));
    WideOption(hOptions, L"RootDirectory", PO_BINARY, (LPBYTE)wszRootDirectory, MAX_PATH * sizeof(wchar_t));
    WideOption(hOptions, L"RootSpecial", PO_DWORD, (LPBYTE)&nRootSpecial, sizeof(DWORD));
    WideOption(hOptions, L"ShowHidden", PO_DWORD, (LPBYTE)&bShowHidden, sizeof(DWORD));
    WideOption(hOptions, L"AutoFind", PO_DWORD, (LPBYTE)&bAutoFind, sizeof(DWORD));
    WideOption(hOptions, L"SingleClick", PO_DWORD, (LPBYTE)&bSingleClick, sizeof(DWORD));
    WideOption(hOptions, L"SetSaveLocation", PO_DWORD, (LPBYTE)&bSetSaveLocation, sizeof(DWORD));
    WideOption(hOptions, L"FilterOptions", PO_DWORD, (LPBYTE)&nFilterType, sizeof(DWORD));

    //Include filter
    if ((nSize=(int)WideOption(hOptions, L"FilterInclude", PO_STRING, NULL, 0)) > 0)
    {
      if (wszFilterInclude=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize))
      {
        WideOption(hOptions, L"FilterInclude", PO_STRING, (LPBYTE)wszFilterInclude, nSize);
      }
    }

    //Exclude filter
    if ((nSize=(int)WideOption(hOptions, L"FilterExclude", PO_STRING, NULL, 0)) > 0)
    {
      if (wszFilterExclude=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize))
      {
        WideOption(hOptions, L"FilterExclude", PO_STRING, (LPBYTE)wszFilterExclude, nSize);
      }
    }

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }

  //Default include filter
  if (!wszFilterInclude)
  {
    nSize=(int)xprintfW(NULL, L"%s%s", GetLangStringW(wLangModule, STRID_DEFAULTINCLUDE),
                                       GetLangStringW(wLangModule, STRID_DEFAULTCODER));

    if (wszFilterInclude=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize * sizeof(wchar_t)))
    {
      xprintfW(wszFilterInclude, L"%s%s", GetLangStringW(wLangModule, STRID_DEFAULTINCLUDE),
                                          GetLangStringW(wLangModule, STRID_DEFAULTCODER));
    }
  }

  //Default exclude filter
  if (!wszFilterExclude)
  {
    nSize=(int)xprintfW(NULL, L"%s", GetLangStringW(wLangModule, STRID_DEFAULTEXCLUDE));

    if (wszFilterExclude=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize * sizeof(wchar_t)))
    {
      xprintfW(wszFilterExclude, L"%s", GetLangStringW(wLangModule, STRID_DEFAULTEXCLUDE));
    }
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;
  DWORD dwSize;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_RECT)
    {
      WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&dkExplorerDlg->nSide, sizeof(DWORD));
      WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&dkExplorerDlg->rcSize, sizeof(RECT));
    }
    if (dwFlags & OF_SETTINGS)
    {
      WideOption(hOptions, L"RootDirectory", PO_BINARY, (LPBYTE)wszRootDirectory, ((int)xstrlenW(wszRootDirectory) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"RootSpecial", PO_DWORD, (LPBYTE)&nRootSpecial, sizeof(DWORD));
      WideOption(hOptions, L"ShowHidden", PO_DWORD, (LPBYTE)&bShowHidden, sizeof(DWORD));
      WideOption(hOptions, L"AutoFind", PO_DWORD, (LPBYTE)&bAutoFind, sizeof(DWORD));
      WideOption(hOptions, L"SingleClick", PO_DWORD, (LPBYTE)&bSingleClick, sizeof(DWORD));
      WideOption(hOptions, L"SetSaveLocation", PO_DWORD, (LPBYTE)&bSetSaveLocation, sizeof(DWORD));
    }
    if (dwFlags & OF_FILTER)
    {
      if (dwSize=FilterStackToString(&hFilterIncludeStack, NULL, 0))
      {
        if (wszFilterInclude=(wchar_t *)GlobalAlloc(GMEM_FIXED, dwSize * sizeof(wchar_t)))
        {
          FilterStackToString(&hFilterIncludeStack, wszFilterInclude, dwSize);
          WideOption(hOptions, L"FilterInclude", PO_STRING, (LPBYTE)wszFilterInclude, ((int)xstrlenW(wszFilterInclude) + 1) * sizeof(wchar_t));
        }
      }
      if (dwSize=FilterStackToString(&hFilterExcludeStack, NULL, 0))
      {
        if (wszFilterExclude=(wchar_t *)GlobalAlloc(GMEM_FIXED, dwSize * sizeof(wchar_t)))
        {
          FilterStackToString(&hFilterExcludeStack, wszFilterExclude, dwSize);
          WideOption(hOptions, L"FilterExclude", PO_STRING, (LPBYTE)wszFilterExclude, ((int)xstrlenW(wszFilterExclude) + 1) * sizeof(wchar_t));
        }
      }
      WideOption(hOptions, L"FilterOptions", PO_DWORD, (LPBYTE)&nFilterType, sizeof(DWORD));
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
  if (nStringID == STRID_DEFAULTCODER)
    return L"|\
Assembler (*.asm)|\
AutoIt (*.au3)|\
Batch (*.bat;*.cmd)|\
C++ (*.java;*.cpp;*.cxx;*.hpp;*.hxx;*.c++;*.idl;*.tlh;*.tli;*.hh;*.cc;*.cp;*.ii;*.c;*.h;*.i)|\
Sharp (*.cs)|\
CSS (*.css)|\
HTML (*.shtml;*.xhtml;*.html;*.htm;*.hta;*.asp)|\
Ini (*.properties;*.ini;*.inf;*.reg;*.url)|\
Inno (*.iss)|\
JScript (*.js)|\
Lua (*.lua)|\
NSIS (*.nsi;*.nsh)|\
Pascal (*.dpr;*.pas)|\
Perl (*.plx;*.prl;*.pl;*.pm)|\
PHP (*.php3;*.php)|\
Python (*.py)|\
Resource (*.rc)|\
SQL (*.sql)|\
VBScript (*.bas;*.vbs;*.vb5;*.wbt;*.frm;*.vb)|\
XML (*.manifest;*.vcproj;*.csproj;*.vbproj;*.vdproj;*.wixobj;*.wixout;*.wixlib;*.wixpdb;*.wixmsp;*.wixmst;*.user;*.wsdl;*.xaml;*.xslt;*.xml;*.axl;*.dxl;*.fb2;*.kml;*.msc;*.rdf;*.rss;*.svg;*.xsd;*.xsl;*.xul;*.wxi;*.wxl;*.wxs)";

  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_SETUP)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_MENURENAME)
      return L"\x041F\x0435\x0440\x0435\x0438\x043C\x0435\x043D\x043E\x0432\x0430\x0442\x044C\tF2";
    if (nStringID == STRID_MENUFINDDOCUMENT)
      return L"\x041D\x0430\x0439\x0442\x0438\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442\tF3";
    if (nStringID == STRID_MENUCREATEDIR)
      return L"\x0421\x043E\x0437\x0434\x0430\x0442\x044C\x0020\x043F\x0430\x043F\x043A\x0443\tF7";
    if (nStringID == STRID_MENUCREATEFILE)
      return L"\x0421\x043E\x0437\x0434\x0430\x0442\x044C\x0020\x0444\x0430\x0439\x043B\tInsert";
    if (nStringID == STRID_MENUREFRESH)
      return L"\x041E\x0431\x043D\x043E\x0432\x0438\x0442\x044C\tF5";
    if (nStringID == STRID_MENUFILTERNONE)
      return L"\x0411\x0435\x0437\x0020\x0444\x0438\x043B\x044C\x0442\x0440\x0430";
    if (nStringID == STRID_MENUFILTERINCLUDE)
      return L"\x0422\x043E\x043B\x044C\x043A\x043E\x0020\x0443\x043A\x0430\x0437\x0430\x043D\x043D\x044B\x0435\x0020\x0442\x0438\x043F\x044B";
    if (nStringID == STRID_MENUFILTEREXCLUDE)
      return L"\x0418\x0441\x043A\x043B\x044E\x0447\x0438\x0442\x044C\x0020\x0443\x043A\x0430\x0437\x0430\x043D\x043D\x044B\x0435\x0020\x0442\x0438\x043F\x044B";
    if (nStringID == STRID_MENUFILTERITEMADD)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_MENUFILTERITEMUP)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x0432\x0435\x0440\x0445";
    if (nStringID == STRID_MENUFILTERITEMDOWN)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x043D\x0438\x0437";
    if (nStringID == STRID_MENUFILTERITEMREMOVE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_ROOTDIRECTORY)
      return L"\x041A\x043E\x0440\x043D\x0435\x0432\x043E\x0439\x0020\x043A\x0430\x0442\x0430\x043B\x043E\x0433";
    if (nStringID == STRID_MYCOMPUTER)
      return L"\x041C\x043E\x0439\x0020\x043A\x043E\x043C\x043F\x044C\x044E\x0442\x0435\x0440";
    if (nStringID == STRID_SHOWHIDDEN)
      return L"\x041F\x043E\x043A\x0430\x0437\x044B\x0432\x0430\x0442\x044C\x0020\x0441\x043A\x0440\x044B\x0442\x044B\x0435\x0020\x044D\x043B\x0435\x043C\x0435\x043D\x0442\x044B";
    if (nStringID == STRID_AUTOFIND)
      return L"\x0410\x0432\x0442\x043E\x043C\x0430\x0442\x0438\x0447\x0435\x0441\x043A\x0438\x0020\x043D\x0430\x0445\x043E\x0434\x0438\x0442\x044C\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442\x044B";
    if (nStringID == STRID_SINGLECLICK)
      return L"\x041E\x0434\x0438\x043D\x0430\x0440\x043D\x044B\x0439\x0020\x043A\x043B\x0438\x043A";
    if (nStringID == STRID_SETSAVELOCATION)
      return L"\x041F\x0435\x0440\x0435\x0434\x0430\x0442\x044C\x0020\x043F\x0430\x043F\x043A\x0443\x0020\x0434\x043B\x044F\x0020\x0441\x043E\x0445\x0440\x0430\x043D\x0435\x043D\x0438\x044F";
    if (nStringID == STRID_CREATEDIR)
      return L"\x0421\x043E\x0437\x0434\x0430\x0442\x044C\x0020\x043F\x0430\x043F\x043A\x0443:";
    if (nStringID == STRID_CREATEFILE)
      return L"\x0421\x043E\x0437\x0434\x0430\x0442\x044C\x0020\x0444\x0430\x0439\x043B:";
    if (nStringID == STRID_LOADFIRST)
      return L"\x0417\x0430\x0433\x0440\x0443\x0437\x0438\x0442\x0435\x0020\x0441\x043F\x0435\x0440\x0432\x0430 %s.";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";

    if (nStringID == STRID_DEFAULTINCLUDE)
      return L"\
\x0412\x0441\x0435\x0020\x0444\x0430\x0439\x043B\x044B (*)|\
\x0412\x0441\x0435\x0020\x0444\x0430\x0439\x043B\x044B (*)|\
\x0422\x0435\x043A\x0441\x0442 (*.txt;*.log;*.nfo;*.diz)";

    if (nStringID == STRID_DEFAULTEXCLUDE)
      return L"\
\x041F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x044B (*.lnk;*.exe;*.dll;*.sys;*.ocx)|\
\x041F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x044B (*.lnk;*.exe;*.dll;*.sys;*.ocx)";

  }
  else
  {
    if (nStringID == STRID_SETUP)
      return L"Settings...";
    if (nStringID == STRID_MENURENAME)
      return L"Rename\tF2";
    if (nStringID == STRID_MENUFINDDOCUMENT)
      return L"Find document\tF3";
    if (nStringID == STRID_MENUREFRESH)
      return L"Refresh\tF5";
    if (nStringID == STRID_MENUCREATEDIR)
      return L"Create folder\tF7";
    if (nStringID == STRID_MENUCREATEFILE)
      return L"Create file\tInsert";
    if (nStringID == STRID_MENUFILTERNONE)
      return L"No filter";
    if (nStringID == STRID_MENUFILTERINCLUDE)
      return L"Only specified types";
    if (nStringID == STRID_MENUFILTEREXCLUDE)
      return L"Without specified types";
    if (nStringID == STRID_MENUFILTERITEMADD)
      return L"Add";
    if (nStringID == STRID_MENUFILTERITEMUP)
      return L"Move up";
    if (nStringID == STRID_MENUFILTERITEMDOWN)
      return L"Move down";
    if (nStringID == STRID_MENUFILTERITEMREMOVE)
      return L"Remove";
    if (nStringID == STRID_ROOTDIRECTORY)
      return L"Root directory";
    if (nStringID == STRID_MYCOMPUTER)
      return L"My Computer";
    if (nStringID == STRID_SHOWHIDDEN)
      return L"Show hidden items";
    if (nStringID == STRID_AUTOFIND)
      return L"Auto find documents";
    if (nStringID == STRID_SINGLECLICK)
      return L"Single click";
    if (nStringID == STRID_SETSAVELOCATION)
      return L"Set save location";
    if (nStringID == STRID_CREATEDIR)
      return L"Create folder:";
    if (nStringID == STRID_CREATEFILE)
      return L"Create file:";
    if (nStringID == STRID_LOADFIRST)
      return L"Load %s first.";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";

    if (nStringID == STRID_DEFAULTINCLUDE)
      return L"\
All files (*)|\
All files (*)|\
Text (*.txt;*.log;*.nfo;*.diz)";

    if (nStringID == STRID_DEFAULTEXCLUDE)
      return L"\
Programs (*.lnk;*.exe;*.dll;*.sys;*.ocx)|\
Programs (*.lnk;*.exe;*.dll;*.sys;*.ocx)";

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
  bOnMainStart=pd->bOnStart;
  hMainWnd=pd->hMainWnd;
  hWndEdit=pd->hWndEdit;
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

  if (bOldWindows)
  {
    HMODULE hShell32;

    hShell32=GetModuleHandleA("shell32.dll");
    SHFileOperationAPtr=(int (WINAPI *)(LPSHFILEOPSTRUCTA))GetProcAddress(hShell32, "SHFileOperationA");
    SHGetFileInfoAPtr=(UINT_PTR (WINAPI *)(const char *, DWORD, SHFILEINFOA *, UINT, UINT))GetProcAddress(hShell32, "SHGetFileInfoA");
  }
  else
  {
    HMODULE hShell32;

    hShell32=GetModuleHandleW(L"shell32.dll");
    SHFileOperationWPtr=(int (WINAPI *)(LPSHFILEOPSTRUCTW))GetProcAddress(hShell32, "SHFileOperationW");
    SHGetFileInfoWPtr=(UINT_PTR (WINAPI *)(const wchar_t *, DWORD, SHFILEINFOW *, UINT, UINT))GetProcAddress(hShell32, "SHGetFileInfoW");
  }

  //Initialize text variables
  wszRootDirectory[0]=L'\0';
  wszGotoPath[0]=L'\0';

  ReadOptions(0);

  //Get filter array
  FilterStringToStack(wszFilterInclude, &hFilterIncludeStack);
  if (wszFilterInclude)
  {
    GlobalFree((HGLOBAL)wszFilterInclude);
    wszFilterInclude=NULL;
  }

  FilterStringToStack(wszFilterExclude, &hFilterExcludeStack);
  if (wszFilterExclude)
  {
    GlobalFree((HGLOBAL)wszFilterExclude);
    wszFilterExclude=NULL;
  }
}

void InitMain()
{
  bInitMain=TRUE;

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

  SHGetMalloc(&lpMalloc);
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

  if (lpMalloc)
  {
    lpMalloc->lpVtbl->Release(lpMalloc);
    lpMalloc=NULL;
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
  }
  return TRUE;
}
