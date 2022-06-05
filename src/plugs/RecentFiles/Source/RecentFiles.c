#define OEMRESOURCE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "IconMenu.h"
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
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xstrlenW
#define xstrcpynW
#define xstrcmpiW
#define xstrstrW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define CreateWindowExWide
#define DialogBoxWide
#define FileExistsWide
#define GetFileAttributesWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define ListBox_AddStringWide
#define ListBox_GetTextWide
#define ListBox_InsertStringWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Include icon menu functions
#define ICONMENU_INCLUDE
#include "IconMenu.h"

//Defines
#define STRID_MENU_OPEN          1
#define STRID_MENU_MOVEUP        2
#define STRID_MENU_MOVEDOWN      3
#define STRID_MENU_SORT          4
#define STRID_MENU_DELETE        5
#define STRID_MENU_DELETEOLD     6
#define STRID_ONLYNAMES          7
#define STRID_PLUGIN             8
#define STRID_OK                 9
#define STRID_CANCEL             10

#define OF_RECT        0x1
#define OF_SETTINGS    0x2

//Functions prototypes
BOOL CALLBACK RecentFilesListDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewListBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int StackImportRecentFiles(STACKRECENTFILE *srfPlugin);
void StackExportRecentFiles(STACKRECENTFILE *srfPlugin);
RECENTFILE* StackGetRecentFile(STACKRECENTFILE *srfPlugin, int nIndex);
int StackSortRecentFiles(STACKRECENTFILE *srfPlugin, int nUpDown);
void StackFreeRecentFiles(STACKRECENTFILE *srfPlugin);
void FillRecentFilesListBox(STACKRECENTFILE *srfPlugin, HWND hWnd, BOOL bOnlyNames);
int MoveListBoxItem(STACKRECENTFILE *srfPlugin, HWND hWnd, int nOldIndex, int nNewIndex);
BOOL ShiftListBoxSelItems(STACKRECENTFILE *srfPlugin, HWND hWnd, BOOL bMoveDown);
int DeleteListBoxSelItems(STACKRECENTFILE *srfPlugin, HWND hWnd);
int DeleteListBoxOldItems(STACKRECENTFILE *srfPlugin, HWND hWnd);
int GetListBoxSelItems(HWND hWnd, int **lpSelItems);
void FreeListBoxSelItems(int **lpSelItems);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
char szBuffer[BUFFER_SIZE];
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HICON hMainIcon;
BOOL bOldWindows;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
RECT rcMainMinMaxDialog={305, 183, 0, 0};
RECT rcMainCurrentDialog={0};
STACKRECENTFILE hRecentFilesStack={0};
HWND hToolbar=NULL;
HIMAGELIST hImageList=NULL;
BOOL bOnlyNames=FALSE;
WNDPROC OldListBoxProc;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="RecentFiles";
}

//Plugin extern function
void __declspec(dllexport) DeleteNonExistent(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (SendMessage(pd->hMainWnd, AKD_RECENTFILES, RF_DELETEOLD, 0))
    SendMessage(pd->hMainWnd, AKD_RECENTFILES, RF_SAVE, (LPARAM)NULL);
}

void __declspec(dllexport) Show(PLUGINDATA *pd)
{
  RECT rc;

  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  GetWindowRect(pd->hWndEdit, &rc);
  TrackPopupMenu(pd->hMenuRecentFiles, TPM_LEFTBUTTON, rc.left, rc.top, 0, pd->hMainWnd, NULL);
}

void __declspec(dllexport) Manage(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  InitMain();
  DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_RECENTFILESLIST), hMainWnd, (DLGPROC)RecentFilesListDlgProc);
  if (bInitMain) UninitMain();
}

BOOL CALLBACK RecentFilesListDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndItemsList;
  static HWND hWndStats;
  static HWND hWndSearch;
  static HWND hWndOnlyNames;
  static HWND hWndOK;
  static HWND hWndCancel;
  static HICONMENU hIconMenuList;
  static HMENU hMenuList;
  static BOOL bListChanged;
  static BOOL bOnlyNamesDlg;
  static RESIZEDIALOG rds[]={{&hWndItemsList,  RDS_SIZE|RDS_X, 0},
                             {&hWndItemsList,  RDS_SIZE|RDS_Y, 0},
                             {&hToolbar,       RDS_SIZE|RDS_X, 0},
                             {&hToolbar,       RDS_MOVE|RDS_Y, 0},
                             {&hWndStats,      RDS_MOVE|RDS_X, 0},
                             {&hWndStats,      RDS_MOVE|RDS_Y, 0},
                             {&hWndSearch,     RDS_SIZE|RDS_X, 0},
                             {&hWndSearch,     RDS_MOVE|RDS_Y, 0},
                             {&hWndOnlyNames,  RDS_MOVE|RDS_Y, 0},
                             {&hWndOK,         RDS_MOVE|RDS_X, 0},
                             {&hWndOK,         RDS_MOVE|RDS_Y, 0},
                             {&hWndCancel,     RDS_MOVE|RDS_X, 0},
                             {&hWndCancel,     RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};
  int nItem;
  LRESULT lResult;

  if (lResult=IconMenu_Messages(hDlg, uMsg, wParam, lParam))
  {
    SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, lResult);
    return TRUE;
  }

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndItemsList=GetDlgItem(hDlg, IDC_ITEM_LIST);
    hWndStats=GetDlgItem(hDlg, IDC_ITEM_STATS);
    hWndSearch=GetDlgItem(hDlg, IDC_SEARCH);
    hWndOnlyNames=GetDlgItem(hDlg, IDC_ONLYNAMES);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDC_ONLYNAMES, GetLangStringW(wLangModule, STRID_ONLYNAMES));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));
    SendMessage(hWndSearch, EM_LIMITTEXT, MAX_PATH, 0);
    if (bOnlyNames) SendMessage(hWndOnlyNames, BM_SETCHECK, BST_CHECKED, 0);
    bOnlyNamesDlg=bOnlyNames;

    //Create toolbar
    {
      TBBUTTON tbb;
      RECT rcList;
      RECT rcStats;
      HICON hIcon;
      int nIcon;
      int nCommand;

      hImageList=ImageList_Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 0);
      ImageList_SetBkColor(hImageList, GetSysColor(COLOR_MENU));

      for (nIcon=IDI_ICON_OPEN; nIcon <= IDI_ICON_DELETEOLD; ++nIcon)
      {
        hIcon=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(nIcon), IMAGE_ICON, 16, 16, 0);
        ImageList_AddIcon(hImageList, hIcon);
        DestroyIcon(hIcon);
      }

      GetWindowSize(hWndItemsList, hDlg, &rcList);
      GetWindowSize(hWndStats, hDlg, &rcStats);
      hToolbar=CreateWindowExWide(0,
                            L"ToolbarWindow32",
                            NULL,
                            WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS|TBSTYLE_FLAT|CCS_NORESIZE|CCS_NOPARENTALIGN|CCS_NODIVIDER,
                            rcList.left, rcList.top + rcList.bottom + 1, rcStats.left - rcList.left, 28,
                            hDlg,
                            (HMENU)(UINT_PTR)IDC_TOOLBAR,
                            hInstanceDLL,
                            NULL);

      SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
      SendMessage(hToolbar, TB_SETBITMAPSIZE, 0, MAKELONG(16, 16));
      SendMessage(hToolbar, TB_SETBUTTONSIZE, 0, MAKELONG(24, 22));
      SendMessage(hToolbar, TB_SETIMAGELIST, 0, (LPARAM)hImageList);

      for (nCommand=IDC_ITEM_OPEN; nCommand <= IDC_ITEM_DELETEOLD; ++nCommand)
      {
        tbb.iBitmap=nCommand - IDC_ITEM_OPEN;
        tbb.idCommand=nCommand;
        tbb.fsState=TBSTATE_ENABLED;
        tbb.fsStyle=TBSTYLE_BUTTON;
        tbb.dwData=0;
        tbb.iString=0;
        SendMessage(hToolbar, TB_ADDBUTTONS, 1, (LPARAM)&tbb);

        if (nCommand == IDC_ITEM_OPEN || nCommand == IDC_ITEM_SORT)
        {
          tbb.iBitmap=-1;
          tbb.idCommand=0;
          tbb.fsState=0;
          tbb.fsStyle=TBSTYLE_SEP;
          tbb.dwData=0;
          tbb.iString=0;
          SendMessage(hToolbar, TB_ADDBUTTONS, 1, (LPARAM)&tbb);
        }
      }
    }

    //Popup menu
    if (hMenuList=CreatePopupMenu())
    {
      if (hIconMenuList=IconMenu_Alloc(hDlg))
      {
        IconMenu_AddItemW(hIconMenuList, hImageList, 0, 16, 16, hMenuList, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_OPEN, GetLangStringW(wLangModule, STRID_MENU_OPEN));
        IconMenu_AddItemW(hIconMenuList, hImageList, -1, 0, 0, hMenuList, -1, MF_BYPOSITION|MF_SEPARATOR, (UINT_PTR)-1, NULL);
        IconMenu_AddItemW(hIconMenuList, hImageList, 1, 16, 16, hMenuList, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_MOVEUP, GetLangStringW(wLangModule, STRID_MENU_MOVEUP));
        IconMenu_AddItemW(hIconMenuList, hImageList, 2, 16, 16, hMenuList, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_MOVEDOWN, GetLangStringW(wLangModule, STRID_MENU_MOVEDOWN));
        IconMenu_AddItemW(hIconMenuList, hImageList, 3, 16, 16, hMenuList, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_SORT, GetLangStringW(wLangModule, STRID_MENU_SORT));
        IconMenu_AddItemW(hIconMenuList, hImageList, -1, 0, 0, hMenuList, -1, MF_BYPOSITION|MF_SEPARATOR, (UINT_PTR)-1, NULL);
        IconMenu_AddItemW(hIconMenuList, hImageList, 4, 16, 16, hMenuList, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_DELETE, GetLangStringW(wLangModule, STRID_MENU_DELETE));
        IconMenu_AddItemW(hIconMenuList, hImageList, 5, 16, 16, hMenuList, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_DELETEOLD, GetLangStringW(wLangModule, STRID_MENU_DELETEOLD));
      }
    }
    bListChanged=FALSE;
    nItem=StackImportRecentFiles(&hRecentFilesStack);
    FillRecentFilesListBox(&hRecentFilesStack, hWndItemsList, bOnlyNamesDlg);
    if (nItem >= 0)
      SendMessage(hWndItemsList, LB_SETSEL, TRUE, nItem);
    PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_ITEM_LIST, LBN_SELCHANGE), 0);

    //SubClass listbox
    OldListBoxProc=(WNDPROC)GetWindowLongPtrWide(hWndItemsList, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndItemsList, GWLP_WNDPROC, (UINT_PTR)NewListBoxProc);
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndItemsList)
    {
      POINT pt;
      int nCount;
      int nSelCount;

      if (lParam == -1)
      {
        pt.x=0;
        pt.y=0;
        ClientToScreen(hWndItemsList, &pt);
      }
      else
      {
        GetCursorPos(&pt);
      }

      nCount=(int)SendMessage(hWndItemsList, LB_GETCOUNT, 0, 0);
      nSelCount=(int)SendMessage(hWndItemsList, LB_GETSELCOUNT, 0, 0);
      EnableMenuItem(hMenuList, IDC_ITEM_OPEN, nSelCount?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_ITEM_MOVEUP, nSelCount?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_ITEM_MOVEDOWN, nSelCount?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_ITEM_SORT, nCount?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_ITEM_DELETE, nSelCount?MF_ENABLED:MF_GRAYED);
      TrackPopupMenu(hMenuList, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == TTN_GETDISPINFOA)
    {
      int nStrID=((int)((NMHDR *)lParam)->idFrom - IDC_ITEM_OPEN) + STRID_MENU_OPEN;

      WideCharToMultiByte(CP_ACP, 0, GetLangStringW(wLangModule, nStrID), -1, ((NMTTDISPINFOA *)lParam)->szText, 80, NULL, NULL);
    }
    else if (((NMHDR *)lParam)->code == TTN_GETDISPINFOW)
    {
      int nStrID=((int)((NMHDR *)lParam)->idFrom - IDC_ITEM_OPEN) + STRID_MENU_OPEN;

      xstrcpynW(((NMTTDISPINFOW *)lParam)->szText, GetLangStringW(wLangModule, nStrID), 80);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_ONLYNAMES)
    {
      int *lpSelItems=NULL;
      int nSelCount;
      int i;

      bOnlyNamesDlg=(BOOL)SendMessage(hWndOnlyNames, BM_GETCHECK, 0, 0);

      //Fill listbox
      nSelCount=GetListBoxSelItems(hWndItemsList, &lpSelItems);
      FillRecentFilesListBox(&hRecentFilesStack, hWndItemsList, bOnlyNamesDlg);
      for (i=0; i < nSelCount; ++i)
        SendMessage(hWndItemsList, LB_SETSEL, TRUE, lpSelItems[i]);
      if (lpSelItems) FreeListBoxSelItems(&lpSelItems);
    }
    else if (LOWORD(wParam) == IDC_SEARCH)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        wchar_t wszSearch[MAX_PATH];

        SendMessage(hWndItemsList, LB_SETSEL, FALSE, -1);

        if (GetWindowTextWide(hWndSearch, wszSearch, MAX_PATH))
        {
          for (nItem=0;; ++nItem)
          {
            if (ListBox_GetTextWide(hWndItemsList, nItem, wszBuffer) == LB_ERR)
              break;
            if (xstrstrW(wszBuffer, -1, wszSearch, -1, FALSE, NULL, NULL))
              SendMessage(hWndItemsList, LB_SETSEL, TRUE, nItem);
          }
        }
        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_ITEM_LIST, LBN_SELCHANGE), 0);
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_MOVEUP)
    {
      if (ShiftListBoxSelItems(&hRecentFilesStack, hWndItemsList, FALSE))
        bListChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDC_ITEM_MOVEDOWN)
    {
      if (ShiftListBoxSelItems(&hRecentFilesStack, hWndItemsList, TRUE))
        bListChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDC_ITEM_SORT)
    {
      StackSortRecentFiles(&hRecentFilesStack, 1);
      FillRecentFilesListBox(&hRecentFilesStack, hWndItemsList, bOnlyNamesDlg);
      bListChanged=TRUE;
      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_ITEM_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_ITEM_DELETE)
    {
      if (DeleteListBoxSelItems(&hRecentFilesStack, hWndItemsList))
        bListChanged=TRUE;
      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_ITEM_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_ITEM_DELETEOLD)
    {
      if (DeleteListBoxOldItems(&hRecentFilesStack, hWndItemsList))
        bListChanged=TRUE;
      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_ITEM_LIST, LBN_SELCHANGE), 0);
    }
    else if (LOWORD(wParam) == IDC_ITEM_LIST)
    {
      if (HIWORD(wParam) == LBN_SELCHANGE)
      {
        int nCount;
        int nSelCount;

        nCount=(int)SendMessage(hWndItemsList, LB_GETCOUNT, 0, 0);
        nSelCount=(int)SendMessage(hWndItemsList, LB_GETSELCOUNT, 0, 0);

        xprintfW(wszBuffer, L"%d / %d", nSelCount, nCount);
        SetWindowTextWide(hWndStats, wszBuffer);
      }
      else if (HIWORD(wParam) == LBN_DBLCLK)
      {
        PostMessage(hDlg, WM_COMMAND, IDC_ITEM_OPEN, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_ITEM_OPEN ||
             LOWORD(wParam) == IDOK ||
             LOWORD(wParam) == IDCANCEL)
    {
      if (LOWORD(wParam) == IDC_ITEM_OPEN ||
          (LOWORD(wParam) == IDOK && !bListChanged))
      {
        RECENTFILE *lpElement;
        OPENDOCUMENTW od;
        int *lpSelItems;
        int nSelCount;
        int i;

        if (nSelCount=GetListBoxSelItems(hWndItemsList, &lpSelItems))
        {
          //Close dialog
          EndDialog(hDlg, 0);
          hDlg=NULL;

          for (i=0; i < nSelCount; ++i)
          {
            if (lpElement=StackGetRecentFile(&hRecentFilesStack, lpSelItems[i] + 1))
            {
              od.pFile=lpElement->wszFile;
              od.pWorkDir=NULL;
              od.dwFlags=OD_ADT_BINARYERROR|OD_ADT_REGCODEPAGE;
              od.nCodePage=0;
              od.bBOM=0;
              od.hDoc=NULL;
              SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);
            }
            else break;

            if (nMDI == WMD_SDI) break;
          }
          FreeListBoxSelItems(&lpSelItems);
        }
      }
      if (LOWORD(wParam) == IDC_ITEM_OPEN ||
          LOWORD(wParam) == IDOK)
      {
        if (bListChanged)
          StackExportRecentFiles(&hRecentFilesStack);

        if (bOnlyNamesDlg != bOnlyNames)
        {
          bOnlyNames=bOnlyNamesDlg;
          dwSaveFlags|=OF_SETTINGS;
        }
      }
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      ImageList_Destroy(hImageList);
      DestroyWindow(hToolbar);
      DestroyMenu(hMenuList);
      IconMenu_Free(hIconMenuList, NULL);
      StackFreeRecentFiles(&hRecentFilesStack);
      if (hDlg) EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    //Destroy plugin icon
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

LRESULT CALLBACK NewListBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_GETDLGCODE)
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

  if (uMsg == WM_KEYDOWN ||
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
    else if (wParam == VK_DELETE)
    {
      if (!bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_DELETE, 0);
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
    else if (wParam == VK_UP)
    {
      if (bAlt && !bShift && !bControl)
      {
        PostMessage(GetParent(hWnd), WM_COMMAND, IDC_ITEM_MOVEUP, 0);
        return TRUE;
      }
    }
  }

  if (bOldWindows)
    return CallWindowProcA(OldListBoxProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldListBoxProc, hWnd, uMsg, wParam, lParam);
}

int StackImportRecentFiles(STACKRECENTFILE *srfPlugin)
{
  EDITINFO ei;
  int nCurItem=-1;

  SendMessage(hMainWnd, AKD_RECENTFILES, RF_CLEAR, (LPARAM)srfPlugin);
  SendMessage(hMainWnd, AKD_RECENTFILES, RF_READ, (LPARAM)srfPlugin);
  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
    nCurItem=(int)SendMessage(hMainWnd, AKD_RECENTFILES, RF_FINDINDEX, (LPARAM)ei.wszFile);
  return nCurItem;
}

void StackExportRecentFiles(STACKRECENTFILE *srfPlugin)
{
  STACKRECENTFILE *srfProgram;

  if (SendMessage(hMainWnd, AKD_RECENTFILES, RF_GET, (LPARAM)&srfProgram))
  {
    SendMessage(hMainWnd, AKD_RECENTFILES, RF_CLEAR, (LPARAM)NULL);
    srfProgram->first=srfPlugin->first;
    srfProgram->last=srfPlugin->last;
    srfPlugin->first=0;
    srfPlugin->last=0;
    SendMessage(hMainWnd, AKD_RECENTFILES, RF_SAVE, (LPARAM)NULL);
  }
}

RECENTFILE* StackGetRecentFile(STACKRECENTFILE *srfPlugin, int nIndex)
{
  RECENTFILE *lpElement=NULL;

  StackGetElement((stack *)srfPlugin->first, (stack *)srfPlugin->last, (stack **)&lpElement, nIndex);
  return lpElement;
}

int StackSortRecentFiles(STACKRECENTFILE *srfPlugin, int nUpDown)
{
  RECENTFILE *tmp1;
  RECENTFILE *tmp2;
  RECENTFILE *tmpNext;
  int i;

  if (nUpDown != 1 && nUpDown != -1) return 1;

  for (tmp1=srfPlugin->first; tmp1; tmp1=tmpNext)
  {
    tmpNext=tmp1->next;

    for (tmp2=srfPlugin->first; tmp2 != tmp1; tmp2=tmp2->next)
    {
      i=xstrcmpiW(tmp2->wszFile, tmp1->wszFile);

      if (i == 0 || i == nUpDown)
      {
        StackMoveBefore((stack **)&srfPlugin->first, (stack **)&srfPlugin->last, (stack *)tmp1, (stack *)tmp2);
        break;
      }
    }
  }
  return 0;
}

void StackFreeRecentFiles(STACKRECENTFILE *srfPlugin)
{
  SendMessage(hMainWnd, AKD_RECENTFILES, RF_CLEAR, (LPARAM)srfPlugin);
}

void FillRecentFilesListBox(STACKRECENTFILE *srfPlugin, HWND hWnd, BOOL bOnlyNames)
{
  RECENTFILE *lpElement;
  int nItem=0;

  SendMessage(hWnd, LB_RESETCONTENT, 0, 0);

  for (lpElement=srfPlugin->first; lpElement; lpElement=lpElement->next)
  {
    if (bOnlyNames)
      ListBox_InsertStringWide(hWnd, nItem++, GetFileName(lpElement->wszFile, lpElement->nFileLen));
    else
      ListBox_InsertStringWide(hWnd, nItem++, lpElement->wszFile);
  }
}

int MoveListBoxItem(STACKRECENTFILE *srfPlugin, HWND hWnd, int nOldIndex, int nNewIndex)
{
  RECENTFILE *lpElement;
  wchar_t *wpText;
  int nIndex=LB_ERR;
  int nTextLen;

  if ((nTextLen=(int)SendMessage(hWnd, LB_GETTEXTLEN, nOldIndex, 0)) != LB_ERR)
  {
    if (wpText=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nTextLen + 1) * sizeof(wchar_t)))
    {
      if (!StackGetElement((stack *)srfPlugin->first, (stack *)srfPlugin->last, (stack **)&lpElement, nOldIndex + 1))
        StackMoveIndex((stack **)&srfPlugin->first, (stack **)&srfPlugin->last, (stack *)lpElement, nNewIndex + 1);
      ListBox_GetTextWide(hWnd, nOldIndex, wpText);
      SendMessage(hWnd, LB_DELETESTRING, nOldIndex, 0);
      nIndex=ListBox_InsertStringWide(hWnd, nNewIndex, wpText);
      GlobalFree((HGLOBAL)wpText);
    }
  }
  return nIndex;
}

BOOL ShiftListBoxSelItems(STACKRECENTFILE *srfPlugin, HWND hWnd, BOOL bMoveDown)
{
  int *lpSelItems;
  int nSelCount;
  int nMinIndex;
  int nMaxIndex;
  int nOldIndex=-1;
  int nNewIndex=-1;
  int i;
  BOOL bResult=FALSE;

  nMinIndex=0;
  nMaxIndex=(int)SendMessage(hWnd, LB_GETCOUNT, 0, 0) - 1;

  if (nSelCount=GetListBoxSelItems(hWnd, &lpSelItems))
  {
    if (!bMoveDown)
    {
      for (i=0; i < nSelCount; ++i)
      {
        if (lpSelItems[i] > nMinIndex)
        {
          if (nNewIndex == -1 && i > 0)
          {
            if (lpSelItems[i] - 1 <= lpSelItems[i - 1])
              continue;
          }
          nOldIndex=lpSelItems[i];
          nNewIndex=lpSelItems[i] - 1;

          MoveListBoxItem(srfPlugin, hWnd, nOldIndex, nNewIndex);
          SendMessage(hWnd, LB_SETSEL, TRUE, nNewIndex);
          bResult=TRUE;
        }
      }
    }
    else
    {
      for (i=--nSelCount; i >= 0; --i)
      {
        if (lpSelItems[i] < nMaxIndex)
        {
          if (nNewIndex == -1 && i < nSelCount)
          {
            if (lpSelItems[i] + 1 >= lpSelItems[i + 1])
              continue;
          }
          nOldIndex=lpSelItems[i];
          nNewIndex=lpSelItems[i] + 1;

          MoveListBoxItem(srfPlugin, hWnd, nOldIndex, nNewIndex);
          SendMessage(hWnd, LB_SETSEL, TRUE, nNewIndex);
          bResult=TRUE;
        }
      }
    }
    FreeListBoxSelItems(&lpSelItems);
  }
  return bResult;
}

int DeleteListBoxSelItems(STACKRECENTFILE *srfPlugin, HWND hWnd)
{
  RECENTFILE *lpElement;
  int *lpSelItems;
  int nCount;
  int nSelCount;
  int i;

  if (nSelCount=GetListBoxSelItems(hWnd, &lpSelItems))
  {
    for (i=nSelCount - 1; i >= 0; --i)
    {
      if (!StackGetElement((stack *)srfPlugin->first, (stack *)srfPlugin->last, (stack **)&lpElement, lpSelItems[i] + 1))
        StackDelete((stack **)&srfPlugin->first, (stack **)&srfPlugin->last, (stack *)lpElement);
      SendMessage(hWnd, LB_DELETESTRING, lpSelItems[i], 0);
    }
    if (nSelCount == 1)
    {
      nCount=(int)SendMessage(hWnd, LB_GETCOUNT, 0, 0);
      if (nCount == lpSelItems[0])
        SendMessage(hWnd, LB_SETSEL, TRUE, lpSelItems[0] - 1);
      else
        SendMessage(hWnd, LB_SETSEL, TRUE, lpSelItems[0]);
    }
    else SetFocus(hWnd);

    FreeListBoxSelItems(&lpSelItems);
  }
  return nSelCount;
}

int DeleteListBoxOldItems(STACKRECENTFILE *srfPlugin, HWND hWnd)
{
  RECENTFILE *lpElement;
  RECENTFILE *lpElementNext;
  int nItem=0;
  int nDelCount=0;

  for (lpElement=srfPlugin->first; lpElement; lpElement=lpElementNext)
  {
    lpElementNext=lpElement->next;

    if (!FileExistsWide(lpElement->wszFile))
    {
      StackDelete((stack **)&srfPlugin->first, (stack **)&srfPlugin->last, (stack *)lpElement);
      SendMessage(hWnd, LB_DELETESTRING, nItem, 0);
      ++nDelCount;
    }
    ++nItem;
  }
  return nDelCount;
}

int GetListBoxSelItems(HWND hWnd, int **lpSelItems)
{
  int nSelCount;

  if (lpSelItems)
  {
    nSelCount=(int)SendMessage(hWnd, LB_GETSELCOUNT, 0, 0);

    if (*lpSelItems=(int *)GlobalAlloc(GPTR, nSelCount * sizeof(int)))
    {
      return (int)SendMessage(hWnd, LB_GETSELITEMS, nSelCount, (LPARAM)*lpSelItems);
    }
  }
  return 0;
}

void FreeListBoxSelItems(int **lpSelItems)
{
  if (lpSelItems && *lpSelItems)
  {
    GlobalFree((HGLOBAL)*lpSelItems);
    *lpSelItems=NULL;
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
    WideOption(hOptions, L"OnlyNames", PO_DWORD, (LPBYTE)&bOnlyNames, sizeof(DWORD));
    WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));

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
      WideOption(hOptions, L"OnlyNames", PO_DWORD, (LPBYTE)&bOnlyNames, sizeof(DWORD));
    }
    if (dwFlags & OF_RECT)
    {
      WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
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
    if (nStringID == STRID_MENU_OPEN)
      return L"\x041E\x0442\x043A\x0440\x044B\x0442\x044C\tEnter";
    if (nStringID == STRID_MENU_MOVEUP)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x0432\x0435\x0440\x0445\tAlt+Up";
    if (nStringID == STRID_MENU_MOVEDOWN)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x043D\x0438\x0437\tAlt+Down";
    if (nStringID == STRID_MENU_SORT)
      return L"\x0421\x043E\x0440\x0442\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_MENU_DELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\tDelete";
    if (nStringID == STRID_MENU_DELETEOLD)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x043D\x0435\x0441\x0443\x0449\x0435\x0441\x0442\x0432\x0443\x044E\x0449\x0438\x0435";
    if (nStringID == STRID_ONLYNAMES)
      return L"\x0422\x043E\x043B\x044C\x043A\x043E\x0020\x0438\x043C\x0435\x043D\x0430";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_MENU_OPEN)
      return L"Open\tEnter";
    if (nStringID == STRID_MENU_MOVEUP)
      return L"Move up\tAlt+Up";
    if (nStringID == STRID_MENU_MOVEDOWN)
      return L"Move down\tAlt+Down";
    if (nStringID == STRID_MENU_SORT)
      return L"Sort";
    if (nStringID == STRID_MENU_DELETE)
      return L"Delete\tDelete";
    if (nStringID == STRID_MENU_DELETEOLD)
      return L"Delete non-existent";
    if (nStringID == STRID_ONLYNAMES)
      return L"Only names";
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
  ReadOptions(0);
}

void InitMain()
{
  bInitMain=TRUE;
}

void UninitMain()
{
  bInitMain=FALSE;
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
