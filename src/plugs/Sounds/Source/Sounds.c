#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <mmsystem.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "MethodFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include stack functions
#define StackGetElement
#define StackGetIndex
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
#define xmemset
#define xmemcpy
#define xstrcpyW
#define xstrcpynW
#define xstrlenW
#define xstrcmpW
#define xstrcmpiW
#define xstrrepW
#define xarrlenA
#define xarrlenW
#define xatoiW
#define xitoaW
#define xuitoaW
#define hex2decW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define AppendMenuWide
#define ComboBox_AddStringWide
#define ComboBox_GetLBTextWide
#define DialogBoxParamWide
#define DialogBoxWide
#define ExpandEnvironmentStringsWide
#define GetKeyNameTextWide
#define GetOpenFileNameWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define RegOpenKeyExWide
#define RegQueryValueExWide
#define SetDlgItemTextWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Include method functions
#define ALLMETHODFUNC
#include "MethodFunc.h"


//// Defines

#define STRID_MENU_ITEMADD        1
#define STRID_MENU_ITEMMODIFY     2
#define STRID_MENU_ITEMMOVEUP     3
#define STRID_MENU_ITEMMOVEDOWN   4
#define STRID_MENU_ITEMDELETE     5
#define STRID_ENABLETHEME         6
#define STRID_DISABLEBEEP         7
#define STRID_KEY                 8
#define STRID_FILE                9
#define STRID_INPUTLANG           10
#define STRID_NONE                11
#define STRID_CHARS               12
#define STRID_DIGITS              13
#define STRID_MOVEMENT            14
#define STRID_CLOSE               15
#define STRID_PLUGIN              16
#define STRID_OK                  17
#define STRID_CANCEL              18

#define OF_LISTTEXT    0x1
#define OF_SETTINGS    0x2
#define OF_RECT        0x4

#define LVI_LIST_KEY    0
#define LVI_LIST_FILE   1
#define LVI_LIST_LAYOUT 2

//Key groups
#define KG_CHARS      -1
#define KG_DIGITS     -2
#define KG_MOVEMENT   -3

#define TXT_DEFAULT_SOUNDS \
L"Sound(13, \"%a\\AkelFiles\\Plugs\\Sounds\\Return.wav\", 0)\r\
Sound(2061, \"%a\\AkelFiles\\Plugs\\Sounds\\Return.wav\", 0)\r\
Sound(8, \"%a\\AkelFiles\\Plugs\\Sounds\\Delete.wav\", 0)\r\
Sound(2094, \"%a\\AkelFiles\\Plugs\\Sounds\\Delete.wav\", 0)\r\
Sound(-1, \"%a\\AkelFiles\\Plugs\\Sounds\\Char.wav\", 0)\r\
Sound(-3, \"%a\\AkelFiles\\Plugs\\Sounds\\Movement.wav\", 0)\r"


//// Structures

typedef struct _SOUNDITEM {
  struct _SOUNDITEM *next;
  struct _SOUNDITEM *prev;
  DWORD dwKey;
  wchar_t wszFile[MAX_PATH];
  wchar_t wszFileExpanded[MAX_PATH];
  WORD wLangID;
} SOUNDITEM;

typedef struct {
  SOUNDITEM *first;
  SOUNDITEM *last;
  SOUNDITEM *siChars;
  SOUNDITEM *siDigits;
  SOUNDITEM *siMovement;
} SOUNDSTACK;


//// Functions prototypes

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ItemDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void FillSoundList(HWND hWnd, SOUNDSTACK *hStack);
int SetSoundListItem(HWND hWnd, int nIndex, SOUNDITEM *siElement, BOOL bAdd);
void SelSoundListItem(HWND hWnd, int nIndex);

void CreateSoundsStack(SOUNDSTACK *hStack, const wchar_t *wpText);
SOUNDITEM* StackInsertSound(SOUNDSTACK *hStack);
SOUNDITEM* StackGetSoundItemByIndex(SOUNDSTACK *hStack, int nIndex);
SOUNDITEM* StackGetSoundItemByKeyAndLayout(SOUNDSTACK *hStack, DWORD dwKey, WORD wLangID);
void StackMoveItem(SOUNDSTACK *hStack, int nOldIndex, int nNewIndex);
void StackDeleteItemByIndex(SOUNDSTACK *hStack, int nIndex);
void StackFreeSounds(SOUNDSTACK *hStack);

void FillLayoutList(HWND hWndLayoutList, WORD wSelLangID);
int GetLayoutName(WORD wLangID, wchar_t *wszName, int nNameMax);

BOOL NextLine(const wchar_t **wpText);
BOOL SkipComment(const wchar_t **wpText);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
INT_PTR TranslateEscapeString(HWND hWndEdit, const wchar_t *wpInput, wchar_t *wszOutput, DWORD *lpdwCaret);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);

int GetCurFile(wchar_t *wszFile, int nMaxFile);
int GetHotkeyString(WORD wHotkey, wchar_t *wszString);
BOOL PlaySoundWide(const wchar_t *pszSound, HMODULE hmod, DWORD fdwSound);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();


//// Global variables

wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
wchar_t wszExeDir[MAX_PATH];
HANDLE hHeap;
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HICON hMainIcon;
BOOL bOldWindows;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
SOUNDSTACK hSoundsStack={0};
wchar_t *wszSoundText=NULL;
BOOL bEnableTheme=TRUE;
BOOL bDisableBeep=FALSE;
BOOL bPlaying=FALSE;
RECT rcMainMinMaxDialog={417, 170, 0, 0};
RECT rcMainCurrentDialog={0};
HWND hWndSoundList=NULL;
int nColumnWidth1=120;
int nColumnWidth2=250;
int nColumnWidth3=120;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewEditProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Sounds";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  //Is plugin already loaded?
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
  static HWND hWndEnableTheme;
  static HWND hWndDisableBeep;
  static HWND hWndClose;
  static HMENU hMenuList;
  static int nSelItem=-1;
  static BOOL bListChanged=FALSE;
  static RESIZEDIALOG rds[]={{&hWndEnableTheme, 0, 0},
                             {&hWndSoundList,   RDS_SIZE|RDS_X, 0},
                             {&hWndSoundList,   RDS_SIZE|RDS_Y, 0},
                             {&hWndDisableBeep, RDS_MOVE|RDS_Y, 0},
                             {&hWndClose,       RDS_MOVE|RDS_X, 0},
                             {&hWndClose,       RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndEnableTheme=GetDlgItem(hDlg, IDC_ENABLETHEME);
    hWndSoundList=GetDlgItem(hDlg, IDC_LIST);
    hWndDisableBeep=GetDlgItem(hDlg, IDC_DISABLEBEEP);
    hWndClose=GetDlgItem(hDlg, IDC_CLOSE);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_ENABLETHEME, GetLangStringW(wLangModule, STRID_ENABLETHEME));
    SetDlgItemTextWide(hDlg, IDC_DISABLEBEEP, GetLangStringW(wLangModule, STRID_DISABLEBEEP));
    SetDlgItemTextWide(hDlg, IDC_CLOSE, GetLangStringW(wLangModule, STRID_CLOSE));

    SendMessage(hWndSoundList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
    if (bEnableTheme)
      SendMessage(hWndEnableTheme, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndSoundList, FALSE);
    if (bDisableBeep)
      SendMessage(hWndDisableBeep, BM_SETCHECK, BST_CHECKED, 0);

    //Popup menu
    if (hMenuList=CreatePopupMenu())
    {
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEMADD, GetLangStringW(wLangModule, STRID_MENU_ITEMADD));
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEMMODIFY, GetLangStringW(wLangModule, STRID_MENU_ITEMMODIFY));
      AppendMenuWide(hMenuList, MF_SEPARATOR, (UINT)-1, NULL);
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEMMOVEUP, GetLangStringW(wLangModule, STRID_MENU_ITEMMOVEUP));
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEMMOVEDOWN, GetLangStringW(wLangModule, STRID_MENU_ITEMMOVEDOWN));
      AppendMenuWide(hMenuList, MF_SEPARATOR, (UINT)-1, NULL);
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEMDELETE, GetLangStringW(wLangModule, STRID_MENU_ITEMDELETE));
    }

    //Columns
    {
      LVCOLUMNW lvc;

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_KEY);
      lvc.cx=nColumnWidth1;
      lvc.iSubItem=LVI_LIST_KEY;
      ListView_InsertColumnWide(hWndSoundList, LVI_LIST_KEY, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_FILE);
      lvc.cx=nColumnWidth2;
      lvc.iSubItem=LVI_LIST_FILE;
      ListView_InsertColumnWide(hWndSoundList, LVI_LIST_FILE, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_INPUTLANG);
      lvc.cx=nColumnWidth3;
      lvc.iSubItem=LVI_LIST_LAYOUT;
      ListView_InsertColumnWide(hWndSoundList, LVI_LIST_LAYOUT, &lvc);
    }
    FillSoundList(hWndSoundList, &hSoundsStack);
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndSoundList)
    {
      LVHITTESTINFO lvhti;
      LVITEMW lvi;
      POINT ptScreen={0};
      RECT rcItem;

      if (lParam == -1)
      {
        if ((lvhti.iItem=nSelItem) != -1)
        {
          rcItem.left=LVIR_LABEL;
          SendMessage(hWndSoundList, LVM_GETITEMRECT, (WPARAM)nSelItem, (LPARAM)&rcItem);
          ptScreen.x=rcItem.left;
          ptScreen.y=rcItem.bottom;
        }
        ClientToScreen(hWndSoundList, &ptScreen);
      }
      else
      {
        GetCursorPos(&ptScreen);
        lvhti.pt=ptScreen;
        ScreenToClient(hWndSoundList, &lvhti.pt);
        SendMessage(hWndSoundList, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvhti);

        lvi.stateMask=LVIS_SELECTED;
        lvi.state=LVIS_SELECTED;
        SendMessage(hWndSoundList, LVM_SETITEMSTATE, (WPARAM)lvhti.iItem, (LPARAM)&lvi);
      }

      EnableMenuItem(hMenuList, IDC_ITEMMODIFY, nSelItem >= 0?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_ITEMMOVEUP, !nSelItem?MF_GRAYED:MF_ENABLED);
      if (SendMessage(hWndSoundList, LVM_GETITEMCOUNT, 0, 0) == nSelItem + 1)
        EnableMenuItem(hMenuList, IDC_ITEMMOVEDOWN, MF_GRAYED);
      else
        EnableMenuItem(hMenuList, IDC_ITEMMOVEDOWN, MF_ENABLED);
      EnableMenuItem(hMenuList, IDC_ITEMDELETE, nSelItem >= 0?MF_ENABLED:MF_GRAYED);
      TrackPopupMenu(hMenuList, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, 0, hDlg, NULL);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_ENABLETHEME)
    {
      BOOL bState=(BOOL)SendMessage(hWndEnableTheme, BM_GETCHECK, 0, 0);

      EnableWindow(hWndSoundList, bState);
    }
    else if (LOWORD(wParam) == IDC_ITEMADD ||
             LOWORD(wParam) == IDC_ITEMMODIFY)
    {
      SOUNDITEM *siElement=NULL;
      BOOL bResult;

      if (LOWORD(wParam) == IDC_ITEMMODIFY && nSelItem != -1)
        siElement=StackGetSoundItemByIndex(&hSoundsStack, nSelItem + 1);

      bResult=(BOOL)DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_ITEM), hDlg, (DLGPROC)ItemDlgProc, (LPARAM)siElement);

      if (bResult)
        bListChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDC_ITEMMOVEUP ||
             LOWORD(wParam) == IDC_ITEMMOVEDOWN)
    {
      SOUNDITEM *siElement;
      int nOldIndex=nSelItem;
      int nNewIndex;

      if (nSelItem != -1)
      {
        if (siElement=StackGetSoundItemByIndex(&hSoundsStack, nOldIndex + 1))
        {
          //Delete
          SendMessage(hWndSoundList, LVM_DELETEITEM, nOldIndex, 0);

          //Add
          if (LOWORD(wParam) == IDC_ITEMMOVEUP)
            nNewIndex=max(nOldIndex - 1, 0);
          else
            nNewIndex=nOldIndex + 1;
          nNewIndex=SetSoundListItem(hWndSoundList, nNewIndex, siElement, TRUE);

          //Move item in stack
          StackMoveItem(&hSoundsStack, nOldIndex + 1, nNewIndex + 1);

          //Select
          SetFocus(hWndSoundList);
          SelSoundListItem(hWndSoundList, nNewIndex);

          bListChanged=TRUE;
        }
      }
    }
    else if (LOWORD(wParam) == IDC_ITEMDELETE)
    {
      int nIndex=nSelItem;

      if (nSelItem != -1)
      {
        //Delete
        StackDeleteItemByIndex(&hSoundsStack, nIndex + 1);
        SendMessage(hWndSoundList, LVM_DELETEITEM, nIndex, 0);
        nIndex=min(nIndex, (int)SendMessage(hWndSoundList, LVM_GETITEMCOUNT, 0, 0) - 1);

        //Select
        SetFocus(hWndSoundList);
        SelSoundListItem(hWndSoundList, nIndex);
        bListChanged=TRUE;
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      PostMessage(hDlg, WM_COMMAND, IDC_ITEMMODIFY, 0);
    }
    else if (LOWORD(wParam) == IDC_CLOSE ||
             LOWORD(wParam) == IDCANCEL)
    {
      int nWidth;

      nWidth=(int)SendMessage(hWndSoundList, LVM_GETCOLUMNWIDTH, LVI_LIST_KEY, 0);
      if (nColumnWidth1 != nWidth)
      {
        nColumnWidth1=nWidth;
        dwSaveFlags|=OF_RECT;
      }
      nWidth=(int)SendMessage(hWndSoundList, LVM_GETCOLUMNWIDTH, LVI_LIST_FILE, 0);
      if (nColumnWidth2 != nWidth)
      {
        nColumnWidth2=nWidth;
        dwSaveFlags|=OF_RECT;
      }
      nWidth=(int)SendMessage(hWndSoundList, LVM_GETCOLUMNWIDTH, LVI_LIST_LAYOUT, 0);
      if (nColumnWidth3 != nWidth)
      {
        nColumnWidth3=nWidth;
        dwSaveFlags|=OF_RECT;
      }

      if (SendMessage(hWndEnableTheme, BM_GETCHECK, 0, 0) != bEnableTheme)
      {
        bEnableTheme=!bEnableTheme;
        dwSaveFlags|=OF_SETTINGS;
      }
      if (SendMessage(hWndDisableBeep, BM_GETCHECK, 0, 0) != bDisableBeep)
      {
        bDisableBeep=!bDisableBeep;
        dwSaveFlags|=OF_SETTINGS;
      }

      if (bListChanged)
      {
        SOUNDITEM *siElement;
        DWORD dwSize;

        for (dwSize=0, siElement=(SOUNDITEM *)hSoundsStack.first; siElement; siElement=siElement->next)
        {
          dwSize+=(DWORD)xprintfW(NULL, L"Sound(%d, \"%s\", %d)\r", siElement->dwKey, siElement->wszFile, siElement->wLangID) - 1;
        }
        if (wszSoundText=(wchar_t *)HeapAlloc(hHeap, 0, (dwSize + 1) * sizeof(wchar_t)))
        {
          for (dwSize=0, siElement=(SOUNDITEM *)hSoundsStack.first; siElement; siElement=siElement->next)
          {
            dwSize+=(DWORD)xprintfW(wszSoundText + dwSize, L"Sound(%d, \"%s\", %d)\r", siElement->dwKey, siElement->wszFile, siElement->wLangID);
          }
        }
        dwSaveFlags|=OF_LISTTEXT;
      }
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      if (wszSoundText)
      {
        HeapFree(hHeap, 0, wszSoundText);
        wszSoundText=NULL;
      }
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_LIST)
    {
      if (((NMLISTVIEW *)lParam)->hdr.code == LVN_ITEMCHANGED)
      {
        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_SELECTED)
        {
          nSelItem=((NMLISTVIEW *)lParam)->iItem;
        }
        if (((NMLISTVIEW *)lParam)->uOldState & LVIS_SELECTED)
        {
          nSelItem=-1;
        }
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
      {
        if (nSelItem >= 0)
          PostMessage(hDlg, WM_COMMAND, IDC_ITEMMODIFY, 0);
      }
      else if (((NMHDR *)lParam)->code == (UINT)LVN_KEYDOWN)
      {
        NMLVKEYDOWN *pnkd=(NMLVKEYDOWN *)lParam;
        BOOL bAlt=FALSE;
        BOOL bShift=FALSE;
        BOOL bControl=FALSE;

        if (GetKeyState(VK_MENU) < 0)
          bAlt=TRUE;
        if (GetKeyState(VK_SHIFT) < 0)
          bShift=TRUE;
        if (GetKeyState(VK_CONTROL) < 0)
          bControl=TRUE;

        if (pnkd->wVKey == VK_INSERT)
        {
          if (!bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_ITEMADD, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_F2)
        {
          if (!bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_ITEMMODIFY, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_DELETE)
        {
          if (!bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_ITEMDELETE, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_UP)
        {
          if (bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_ITEMMOVEUP, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_DOWN)
        {
          if (bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_ITEMMOVEDOWN, 0);
            return TRUE;
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
    //Destroy resources
    DestroyMenu(hMenuList);
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

BOOL CALLBACK ItemDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static SOUNDITEM *siElement;
  static HICON hPluginIcon;
  static HICON hPlayIcon;
  static HWND hWndHotkeyInputCheck;
  static HWND hWndHotkeyInput;
  static HWND hWndHotkeyGroupChars;
  static HWND hWndHotkeyGroupDigits;
  static HWND hWndHotkeyGroupMovement;
  static HWND hWndSoundBrowse;
  static HWND hWndSoundEdit;
  static HWND hWndSoundPlay;
  static HWND hWndLayoutList;
  static HWND hWndOK;
  HWND hWndFocus;

  if (uMsg == WM_INITDIALOG)
  {
    siElement=(SOUNDITEM *)lParam;

    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndHotkeyInputCheck=GetDlgItem(hDlg, IDC_KEYINPUT_CHECK);
    hWndHotkeyInput=GetDlgItem(hDlg, IDC_KEYINPUT);
    hWndHotkeyGroupChars=GetDlgItem(hDlg, IDC_KEYGROUP_CHARS);
    hWndHotkeyGroupDigits=GetDlgItem(hDlg, IDC_KEYGROUP_DIGITS);
    hWndHotkeyGroupMovement=GetDlgItem(hDlg, IDC_KEYGROUP_MOVEMENT);
    hWndSoundBrowse=GetDlgItem(hDlg, IDC_FILE_BROWSE);
    hWndSoundEdit=GetDlgItem(hDlg, IDC_FILE_EDIT);
    hWndSoundPlay=GetDlgItem(hDlg, IDC_FILE_PLAY);
    hWndLayoutList=GetDlgItem(hDlg, IDC_LAYOUT_LIST);
    hWndOK=GetDlgItem(hDlg, IDOK);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_KEY_GROUP, GetLangStringW(wLangModule, STRID_KEY));
    SetDlgItemTextWide(hDlg, IDC_KEYGROUP_CHARS, GetLangStringW(wLangModule, STRID_CHARS));
    SetDlgItemTextWide(hDlg, IDC_KEYGROUP_DIGITS, GetLangStringW(wLangModule, STRID_DIGITS));
    SetDlgItemTextWide(hDlg, IDC_KEYGROUP_MOVEMENT, GetLangStringW(wLangModule, STRID_MOVEMENT));
    SetDlgItemTextWide(hDlg, IDC_FILE_GROUP, GetLangStringW(wLangModule, STRID_FILE));
    SetDlgItemTextWide(hDlg, IDC_LAYOUT_GROUP, GetLangStringW(wLangModule, STRID_INPUTLANG));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkeyInput, 0);

    //Set play button
    {
      BUTTONDRAW bd;

      hPlayIcon=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLAY), IMAGE_ICON, 16, 16, 0);
      bd.dwFlags=BIF_ICON|BIF_ETCHED|BIF_ENABLEFOCUS;
      bd.hImage=hPlayIcon;
      bd.nImageWidth=16;
      bd.nImageHeight=16;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndSoundPlay, (LPARAM)&bd);
    }

    if (siElement)
    {
      if (siElement->dwKey == (DWORD)KG_CHARS)
        hWndFocus=hWndHotkeyGroupChars;
      else if (siElement->dwKey == (DWORD)KG_DIGITS)
        hWndFocus=hWndHotkeyGroupDigits;
      else if (siElement->dwKey == (DWORD)KG_MOVEMENT)
        hWndFocus=hWndHotkeyGroupMovement;
      else
      {
        SendMessage(hWndHotkeyInput, HKM_SETHOTKEY, siElement->dwKey, 0);
        hWndFocus=hWndHotkeyInputCheck;
      }
      SendMessage(hWndFocus, BM_SETCHECK, BST_CHECKED, 0);
      SetFocus(hWndFocus);

      SetWindowTextWide(hWndSoundEdit, siElement->wszFile);
    }
    else SendMessage(hWndHotkeyInputCheck, BM_SETCHECK, BST_CHECKED, 0);

    FillLayoutList(hWndLayoutList, (WORD)(siElement?siElement->wLangID:0));
    PostMessage(hDlg, WM_COMMAND, IDC_FILE_EDIT, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_FILE_BROWSE)
    {
      OPENFILENAMEW efnW;
      wchar_t wszFileExpanded[MAX_PATH];

      GetWindowTextWide(hWndSoundEdit, wszBuffer, MAX_PATH);
      TranslateFileString(wszBuffer, wszFileExpanded, MAX_PATH);
      xmemset(&efnW, 0, sizeof(OPENFILENAMEW));
      efnW.lStructSize  =sizeof(OPENFILENAMEW);
      efnW.hwndOwner    =hDlg;
      efnW.lpstrFile    =wszFileExpanded;
      efnW.nMaxFile     =MAX_PATH;
      efnW.lpstrFilter  =L"*.wav\0*.wav\0*.*\0*.*\0\0";
      efnW.nFilterIndex =1;
      efnW.Flags        =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;

      if (GetOpenFileNameWide(&efnW))
      {
        xstrrepW(wszFileExpanded, -1, wszExeDir, -1, L"%a", -1, FALSE, wszFileExpanded, NULL);
        SetWindowTextWide(hWndSoundEdit, wszFileExpanded);
      }
    }
    else if (LOWORD(wParam) == IDC_FILE_EDIT)
    {
      if (GetWindowTextLengthWide(hWndSoundEdit))
      {
        EnableWindow(hWndSoundPlay, TRUE);
        EnableWindow(hWndOK, TRUE);
      }
      else
      {
        EnableWindow(hWndSoundPlay, FALSE);
        EnableWindow(hWndOK, FALSE);
      }
    }
    else if (LOWORD(wParam) == IDC_FILE_PLAY)
    {
      wchar_t wszFileExpanded[MAX_PATH];

      GetWindowTextWide(hWndSoundEdit, wszBuffer, MAX_PATH);
      TranslateFileString(wszBuffer, wszFileExpanded, MAX_PATH);
      PlaySoundWide(wszFileExpanded, NULL, SND_ASYNC|SND_FILENAME|SND_NODEFAULT);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      SOUNDITEM *siNewElement=siElement;
      int nIndex;

      if (!siNewElement)
        siNewElement=StackInsertSound(&hSoundsStack);

      if (siNewElement)
      {
        if (SendMessage(hWndHotkeyGroupChars, BM_GETCHECK, 0, 0) == BST_CHECKED)
          siNewElement->dwKey=(DWORD)KG_CHARS;
        else if (SendMessage(hWndHotkeyGroupDigits, BM_GETCHECK, 0, 0) == BST_CHECKED)
          siNewElement->dwKey=(DWORD)KG_DIGITS;
        else if (SendMessage(hWndHotkeyGroupMovement, BM_GETCHECK, 0, 0) == BST_CHECKED)
          siNewElement->dwKey=(DWORD)KG_MOVEMENT;
        else
          siNewElement->dwKey=(DWORD)SendMessage(hWndHotkeyInput, HKM_GETHOTKEY, 0, 0);

        GetWindowTextWide(hWndSoundEdit, siNewElement->wszFile, MAX_PATH);
        TranslateFileString(siNewElement->wszFile, siNewElement->wszFileExpanded, MAX_PATH);

        if ((nIndex=(int)SendMessage(hWndLayoutList, CB_GETCURSEL, 0, 0)) != CB_ERR)
          siNewElement->wLangID=(WORD)SendMessage(hWndLayoutList, CB_GETITEMDATA, nIndex, 0);

        if (hSoundsStack.siChars == siNewElement)
          hSoundsStack.siChars=NULL;
        if (hSoundsStack.siDigits == siNewElement)
          hSoundsStack.siDigits=NULL;
        if (hSoundsStack.siMovement == siNewElement)
          hSoundsStack.siMovement=NULL;

        if (siNewElement->dwKey == (DWORD)KG_CHARS)
        {
          if (!hSoundsStack.siChars) hSoundsStack.siChars=siNewElement;
        }
        else if (siNewElement->dwKey == (DWORD)KG_DIGITS)
        {
          if (!hSoundsStack.siDigits) hSoundsStack.siDigits=siNewElement;
        }
        else if (siNewElement->dwKey == (DWORD)KG_MOVEMENT)
        {
          if (!hSoundsStack.siMovement) hSoundsStack.siMovement=siNewElement;
        }

        if (siElement)
          nIndex=(int)SendMessage(hWndSoundList, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED);
        else
          nIndex=-1;
        SetSoundListItem(hWndSoundList, nIndex, siNewElement, nIndex == -1?TRUE:FALSE);
      }
      EndDialog(hDlg, TRUE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, FALSE);
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
    //Destroy resources
    DestroyIcon(hPlayIcon);
    DestroyIcon(hPluginIcon);
  }
  return FALSE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDN_EDIT_ONSTART)
  {
    if (bDisableBeep)
      SendMessage((HWND)wParam, AEM_SETOPTIONS, AECOOP_OR, AECO_DISABLEBEEP);
  }
  else if (uMsg == AKDN_HOTKEY)
  {
    if (bEnableTheme)
    {
      SOUNDITEM *lpSoundItem;
      EDITINFO ei;
      HKL dwInputLocale;

      if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
      {
        dwInputLocale=(HKL)SendMessage(ei.hWndEdit, AEM_INPUTLANGUAGE, 0, 0);

        if ((lpSoundItem=StackGetSoundItemByKeyAndLayout(&hSoundsStack, (DWORD)wParam, LOWORD(dwInputLocale))))
        {
          bPlaying=TRUE;
          PlaySoundWide(lpSoundItem->wszFileExpanded, NULL, SND_ASYNC|SND_FILENAME|SND_NODEFAULT);
        }
        else bPlaying=FALSE;
      }
      else bPlaying=FALSE;
    }
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CHAR || uMsg == WM_IME_CHAR)
  {
    if (bEnableTheme)
    {
      if (wParam == VK_TAB || (wParam >= 0x20 && wParam != 0x7F))
      {
        if (hSoundsStack.siChars && !bPlaying)
        {
          EDITINFO ei;
          HKL dwInputLocale;

          if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
          {
            if (!ei.bReadOnly)
            {
              dwInputLocale=(HKL)SendMessage(ei.hWndEdit, AEM_INPUTLANGUAGE, 0, 0);

              if (!hSoundsStack.siChars->wLangID || hSoundsStack.siChars->wLangID == LOWORD(dwInputLocale))
                PlaySoundWide(hSoundsStack.siChars->wszFileExpanded, NULL, SND_ASYNC|SND_FILENAME|SND_NODEFAULT);
            }
          }
        }
      }
    }
  }

  //Call next procedure
  return NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

void FillSoundList(HWND hWnd, SOUNDSTACK *hStack)
{
  SOUNDITEM *siElement;
  int i=0;

  for (siElement=(SOUNDITEM *)hStack->first; siElement; siElement=siElement->next)
  {
    SetSoundListItem(hWnd, i++, siElement, TRUE);
  }
}

int SetSoundListItem(HWND hWnd, int nIndex, SOUNDITEM *siElement, BOOL bAdd)
{
  wchar_t wszKey[MAX_PATH];
  LVITEMW lvi;

  if (siElement)
  {
    if (bAdd)
    {
      lvi.mask=LVIF_PARAM;
      lvi.lParam=(LPARAM)siElement;
      lvi.iItem=(nIndex > -1)?nIndex:0x7FFFFFFF;
      lvi.iSubItem=LVI_LIST_KEY;
      nIndex=ListView_InsertItemWide(hWnd, &lvi);
    }

    if (siElement->dwKey == (DWORD)KG_CHARS)
      xstrcpynW(wszKey, GetLangStringW(wLangModule, STRID_CHARS), MAX_PATH);
    else if (siElement->dwKey == (DWORD)KG_DIGITS)
      xstrcpynW(wszKey, GetLangStringW(wLangModule, STRID_DIGITS), MAX_PATH);
    else if (siElement->dwKey == (DWORD)KG_MOVEMENT)
      xstrcpynW(wszKey, GetLangStringW(wLangModule, STRID_MOVEMENT), MAX_PATH);
    else
      GetHotkeyString((WORD)siElement->dwKey, wszKey);
    lvi.mask=LVIF_TEXT;
    lvi.pszText=wszKey;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVI_LIST_KEY;
    ListView_SetItemWide(hWnd, &lvi);

    lvi.mask=LVIF_TEXT;
    lvi.pszText=siElement->wszFile;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVI_LIST_FILE;
    ListView_SetItemWide(hWnd, &lvi);

    if (!GetLayoutName(siElement->wLangID, wszBuffer, BUFFER_SIZE))
      wszBuffer[0]=L'\0';
    lvi.mask=LVIF_TEXT;
    lvi.pszText=wszBuffer;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVI_LIST_LAYOUT;
    ListView_SetItemWide(hWnd, &lvi);

    return nIndex;
  }
  return -1;
}

void SelSoundListItem(HWND hWnd, int nIndex)
{
  LVITEMA lvi;

  lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
  lvi.state=LVIS_SELECTED|LVIS_FOCUSED;
  SendMessage(hWnd, LVM_SETITEMSTATE, nIndex, (LPARAM)&lvi);
}

void CreateSoundsStack(SOUNDSTACK *hStack, const wchar_t *wpText)
{
  STACKEXTPARAM hParamStack={0};
  EXTPARAM *lpParameter;
  SOUNDITEM *siElement=NULL;
  EXPPARAM ep[]={{L"%f", 2, 0, EXPPARAM_FILE},
                 {L"%d", 2, 0, EXPPARAM_FILEDIR},
                 {0, 0, 0, 0}};

  if (wpText)
  {
    for (; *wpText; NextLine(&wpText))
    {
      if (!SkipComment(&wpText)) break;
      MethodGetName(wpText, wszBuffer, BUFFER_SIZE, &wpText);

      if (!xstrcmpiW(wszBuffer, L"Sound"))
      {
        if (siElement=StackInsertSound(hStack))
        {
          MethodParseParameters(&hParamStack, wpText, &wpText);
          SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&hParamStack, (LPARAM)ep);

          if (lpParameter=MethodGetParameter(&hParamStack, 1))
            siElement->dwKey=(DWORD)lpParameter->nNumber;
          if (lpParameter=MethodGetParameter(&hParamStack, 2))
          {
            xstrcpynW(siElement->wszFile, lpParameter->wpString, MAX_PATH);
            xstrcpynW(siElement->wszFileExpanded, lpParameter->wpExpanded, MAX_PATH);
          }
          if (lpParameter=MethodGetParameter(&hParamStack, 3))
            siElement->wLangID=(WORD)lpParameter->nNumber;
          MethodFreeParameters(&hParamStack);

          if (siElement->dwKey == (DWORD)KG_CHARS)
          {
            if (!hStack->siChars) hStack->siChars=siElement;
          }
          else if (siElement->dwKey == (DWORD)KG_DIGITS)
          {
            if (!hStack->siDigits) hStack->siDigits=siElement;
          }
          else if (siElement->dwKey == (DWORD)KG_MOVEMENT)
          {
            if (!hStack->siMovement) hStack->siMovement=siElement;
          }
        }
        else break;
      }
    }
  }
}

SOUNDITEM* StackInsertSound(SOUNDSTACK *hStack)
{
  SOUNDITEM *siElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&siElement, -1, sizeof(SOUNDITEM));
  return siElement;
}

SOUNDITEM* StackGetSoundItemByIndex(SOUNDSTACK *hStack, int nIndex)
{
  SOUNDITEM *siElement;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&siElement, nIndex))
    return siElement;
  return NULL;
}

SOUNDITEM* StackGetSoundItemByKeyAndLayout(SOUNDSTACK *hStack, DWORD dwKey, WORD wLangID)
{
  SOUNDITEM *siElement;

  for (siElement=hStack->first; siElement; siElement=siElement->next)
  {
    if (siElement->dwKey == dwKey && (!siElement->wLangID || siElement->wLangID == wLangID))
      return siElement;
  }

  //Key groups
  {
    BYTE nVk=LOBYTE(dwKey);
    BYTE nMod=HIBYTE(dwKey);

    if (nVk >= VK_PRIOR && nVk <= VK_DOWN)
    {
      if (hStack->siMovement && (!hStack->siMovement->wLangID || hStack->siMovement->wLangID == wLangID))
        return hStack->siMovement;
    }
    if (!(nMod & HOTKEYF_CONTROL) && !(nMod & HOTKEYF_ALT) && !(nMod & HOTKEYF_SHIFT))
    {
      if (nVk >= '0' && nVk <= '9')
      {
        if (hStack->siDigits && (!hStack->siDigits->wLangID || hStack->siDigits->wLangID == wLangID))
          return hStack->siDigits;
      }
    }
  }
  return NULL;
}

void StackMoveItem(SOUNDSTACK *hStack, int nOldIndex, int nNewIndex)
{
  void *siElement=NULL;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&siElement, nOldIndex))
    StackMoveIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack *)siElement, nNewIndex);
}

void StackDeleteItemByIndex(SOUNDSTACK *hStack, int nIndex)
{
  SOUNDITEM *siElement=NULL;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&siElement, nIndex))
  {
    StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)siElement);
  }
}

void StackFreeSounds(SOUNDSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void FillLayoutList(HWND hWndLayoutList, WORD wSelLangID)
{
  wchar_t wszLangName[MAX_PATH];
  HKL *lpList;
  WORD wLangID;
  int nListCount;
  int nIndex;
  int nSelIndex=0;
  int i;

  ComboBox_AddStringWide(hWndLayoutList, GetLangStringW(wLangModule, STRID_NONE));

  if ((nListCount=GetKeyboardLayoutList(0, NULL)) > 1)
  {
    if (lpList=(HKL *)GlobalAlloc(GPTR, nListCount * sizeof(UINT_PTR)))
    {
      GetKeyboardLayoutList(nListCount, lpList);

      for (i=0; i < nListCount; ++i)
      {
        wLangID=LOWORD((UINT_PTR)lpList[i]);
        GetLayoutName(wLangID, wszLangName, MAX_PATH);

        xprintfW(wszBuffer, L"0x%04x %s", wLangID, wszLangName);
        if ((nIndex=ComboBox_AddStringWide(hWndLayoutList, wszBuffer)) != CB_ERR)
          SendMessage(hWndLayoutList, CB_SETITEMDATA, nIndex, (LPARAM)wLangID);

        if (wSelLangID == wLangID)
          nSelIndex=i + 1;
      }
      GlobalFree((HGLOBAL)lpList);
    }
  }
  SendMessage(hWndLayoutList, CB_SETCURSEL, nSelIndex, 0);
}

int GetLayoutName(WORD wLangID, wchar_t *wszName, int nNameMax)
{
  wchar_t wszLangID[32];
  wchar_t wszLangName[MAX_PATH];
  wchar_t *wpLangNameCount;
  HKEY hKey;
  DWORD dwType;
  DWORD dwSize;
  int nResult=0;

  if (!wLangID) return 0;

  if (RegOpenKeyExWide(HKEY_CLASSES_ROOT, L"MIME\\Database\\Rfc1766", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    //Input language ID
    xprintfW(wszLangID, L"%04x", wLangID);

    //Input language name
    dwSize=MAX_PATH * sizeof(wchar_t);
    if (RegQueryValueExWide(hKey, wszLangID, NULL, &dwType, (LPBYTE)wszLangName, &dwSize) == ERROR_SUCCESS)
    {
      for (wpLangNameCount=wszLangName; *wpLangNameCount; ++wpLangNameCount)
      {
        if (*wpLangNameCount == L';')
        {
          ++wpLangNameCount;
          break;
        }
      }
      nResult=(int)xstrcpynW(wszName, wpLangNameCount, nNameMax);
    }
    RegCloseKey(hKey);
  }
  return nResult;
}

BOOL NextLine(const wchar_t **wpText)
{
  while (**wpText != L'\r' && **wpText != L'\n' && **wpText != L'\0') ++*wpText;
  if (**wpText == L'\0') return FALSE;
  if (**wpText == L'\r') ++*wpText;
  if (**wpText == L'\n') ++*wpText;
  return TRUE;
}

BOOL SkipComment(const wchar_t **wpText)
{
  for (;;)
  {
    while (**wpText == L' ' || **wpText == L'\t' || **wpText == L'\r' || **wpText == L'\n') ++*wpText;

    if (**wpText == L';' || **wpText == L'#')
    {
      if (!NextLine(wpText))
        return FALSE;
    }
    else break;
  }
  if (**wpText == L'\0')
    return FALSE;
  return TRUE;
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

INT_PTR TranslateEscapeString(HWND hWndEdit, const wchar_t *wpInput, wchar_t *wszOutput, DWORD *lpdwCaret)
{
  EDITINFO ei;
  const wchar_t *a=wpInput;
  wchar_t *b=wszOutput;
  wchar_t whex[5];
  int nDec;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWndEdit, (LPARAM)&ei))
  {
    for (whex[4]='\0'; *a; ++a)
    {
      if (*a == '\\')
      {
        if (*++a == '\\')
        {
          if (wszOutput) *b='\\';
          ++b;
        }
        else if (*a == 'n')
        {
          if (ei.nNewLine == NEWLINE_MAC)
          {
            if (wszOutput) *b='\r';
            ++b;
          }
          else if (ei.nNewLine == NEWLINE_UNIX)
          {
            if (wszOutput) *b='\n';
            ++b;
          }
          else if (ei.nNewLine == NEWLINE_WIN)
          {
            if (wszOutput) *b='\r';
            ++b;
            if (wszOutput) *b='\n';
            ++b;
          }
        }
        else if (*a == 't')
        {
          if (wszOutput) *b='\t';
          ++b;
        }
        else if (*a == '[')
        {
          while (*++a == ' ');

          do
          {
            whex[0]=*a;
            if (!*a) goto Error;
            whex[1]=*++a;
            if (!*a) goto Error;
            whex[2]=*++a;
            if (!*a) goto Error;
            whex[3]=*++a;
            if (!*a) goto Error;
            nDec=(int)hex2decW(whex, 4, NULL);
            if (nDec == -1) goto Error;
            while (*++a == ' ');

            if (wszOutput) *b=(wchar_t)nDec;
            ++b;
          }
          while (*a && *a != ']');

          if (!*a) goto Error;
        }
        else if (*a == 's')
        {
          CHARRANGE64 cr;

          SendMessage(hWndEdit, EM_EXGETSEL64, 0, (WPARAM)&cr);
          if (cr.cpMin != cr.cpMax)
          {
            if (wszOutput)
            {
              wchar_t *wpText;
              INT_PTR nTextLen=0;

              if (wpText=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)hWndEdit, (LPARAM)&nTextLen))
              {
                xmemcpy(b, wpText, nTextLen * sizeof(wchar_t));
                b+=nTextLen;
                SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpText);
              }
            }
            else b+=cr.cpMax - cr.cpMin;
          }
        }
        else if (*a == '|')
        {
          if (lpdwCaret) *lpdwCaret=(DWORD)(b - wszOutput);
        }
        else goto Error;
      }
      else
      {
        if (wszOutput) *b=*a;
        ++b;
      }
    }
    if (wszOutput)
      *b='\0';
    else
      ++b;
    return (b - wszOutput);
  }

  Error:
  if (wszOutput) *wszOutput='\0';
  return 0;
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

int GetCurFile(wchar_t *wszFile, int nMaxFile)
{
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    return (int)xstrcpynW(wszFile, ei.wszFile, nMaxFile) + 1;
  }
  wszFile[0]='\0';
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

BOOL PlaySoundWide(const wchar_t *pszSound, HMODULE hmod, DWORD fdwSound)
{
  if (bOldWindows == TRUE)
  {
    BOOL bResult;

    if ((UINT_PTR)pszSound > MAXUHALF_PTR)
      pszSound=(wchar_t *)AllocAnsi(pszSound);
    bResult=PlaySoundA((const char *)pszSound, hInstanceDLL, SND_ASYNC|SND_RESOURCE);
    if ((UINT_PTR)pszSound > MAXUHALF_PTR)
      FreeAnsi((char *)pszSound);
    return bResult;
  }
  return PlaySoundW(pszSound, hmod, fdwSound);
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
    if ((nSize=(DWORD)WideOption(hOptions, L"SoundText", PO_BINARY, NULL, 0)) > 0)
    {
      if (wszSoundText=(wchar_t *)HeapAlloc(hHeap, 0, nSize + sizeof(wchar_t)))
      {
        WideOption(hOptions, L"SoundText", PO_BINARY, (LPBYTE)wszSoundText, nSize);
        wszSoundText[nSize / sizeof(wchar_t)]=L'\0';
      }
    }

    WideOption(hOptions, L"EnableTheme", PO_DWORD, (LPBYTE)&bEnableTheme, sizeof(DWORD));
    WideOption(hOptions, L"DisableBeep", PO_DWORD, (LPBYTE)&bDisableBeep, sizeof(DWORD));

    //Window rectangle
    WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
    WideOption(hOptions, L"ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
    WideOption(hOptions, L"ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
    WideOption(hOptions, L"ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }

  if (!wszSoundText)
  {
    if (wszSoundText=(wchar_t *)GlobalAlloc(GPTR, sizeof(TXT_DEFAULT_SOUNDS)))
    {
      xmemcpy((unsigned char *)wszSoundText, TXT_DEFAULT_SOUNDS, sizeof(TXT_DEFAULT_SOUNDS));
    }
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_LISTTEXT)
    {
      if (wszSoundText)
        WideOption(hOptions, L"SoundText", PO_BINARY, (LPBYTE)wszSoundText, (lstrlenW(wszSoundText) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_SETTINGS)
    {
      WideOption(hOptions, L"EnableTheme", PO_DWORD, (LPBYTE)&bEnableTheme, sizeof(DWORD));
      WideOption(hOptions, L"DisableBeep", PO_DWORD, (LPBYTE)&bDisableBeep, sizeof(DWORD));
    }
    if (dwFlags & OF_RECT)
    {
      WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));
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
    if (nStringID == STRID_MENU_ITEMADD)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C...\tInsert";
    if (nStringID == STRID_MENU_ITEMMODIFY)
      return L"\x0418\x0437\x043C\x0435\x043D\x0438\x0442\x044C...\tF2";
    if (nStringID == STRID_MENU_ITEMMOVEUP)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x0432\x0435\x0440\x0445\tAlt+Up";
    if (nStringID == STRID_MENU_ITEMMOVEDOWN)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x043D\x0438\x0437\tAlt+Down";
    if (nStringID == STRID_MENU_ITEMDELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\tDelete";
    if (nStringID == STRID_ENABLETHEME)
      return L"\x0412\x043A\x043B\x044E\x0447\x0435\x043D\x043E";
    if (nStringID == STRID_DISABLEBEEP)
      return L"\x041E\x0442\x043A\x043B\x044E\x0447\x0438\x0442\x044C\x0020\x0437\x0432\x0443\x043A\x043E\x0432\x044B\x0435\x0020\x0441\x0438\x0433\x043D\x0430\x043B\x044B\x0020\x0432\x0020\x043E\x043A\x043D\x0435\x0020\x0440\x0435\x0434\x0430\x043A\x0442\x0438\x0440\x043E\x0432\x0430\x043D\x0438\x044F";

    if (nStringID == STRID_KEY)
      return L"\x041A\x043B\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_FILE)
      return L"\x0424\x0430\x0439\x043B";
    if (nStringID == STRID_INPUTLANG)
      return L"\x042F\x0437\x044B\x043A\x0020\x0432\x0432\x043E\x0434\x0430";
    if (nStringID == STRID_NONE)
      return L"\x041D\x0435\x0442";
    if (nStringID == STRID_CHARS)
      return L"\x0413\x0440\x0443\x043F\x043F\x0430\x003A\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x044B";
    if (nStringID == STRID_DIGITS)
      return L"\x0413\x0440\x0443\x043F\x043F\x0430\x003A\x0020\x0447\x0438\x0441\x043B\x0430";
    if (nStringID == STRID_MOVEMENT)
      return L"\x0413\x0440\x0443\x043F\x043F\x0430\x003A\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x0449\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_CLOSE)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_MENU_ITEMADD)
      return L"Add...\tInsert";
    if (nStringID == STRID_MENU_ITEMMODIFY)
      return L"Modify...\tF2";
    if (nStringID == STRID_MENU_ITEMMOVEUP)
      return L"Move up\tAlt+Up";
    if (nStringID == STRID_MENU_ITEMMOVEDOWN)
      return L"Move down\tAlt+Down";
    if (nStringID == STRID_MENU_ITEMDELETE)
      return L"Delete\tDelete";
    if (nStringID == STRID_ENABLETHEME)
      return L"Enable";
    if (nStringID == STRID_DISABLEBEEP)
      return L"Disable beep in edit window";

    if (nStringID == STRID_KEY)
      return L"Key";
    if (nStringID == STRID_FILE)
      return L"File";
    if (nStringID == STRID_INPUTLANG)
      return L"Input language";
    if (nStringID == STRID_NONE)
      return L"None";
    if (nStringID == STRID_CHARS)
      return L"Group: symbols";
    if (nStringID == STRID_DIGITS)
      return L"Group: digits";
    if (nStringID == STRID_MOVEMENT)
      return L"Group: movement";
    if (nStringID == STRID_CLOSE)
      return L"Close";
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
  wLangModule=PRIMARYLANGID(pd->wLangModule);
  hHeap=GetProcessHeap();

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
  ReadOptions(0);
  CreateSoundsStack(&hSoundsStack, wszSoundText);
  HeapFree(hHeap, 0, wszSoundText);
  wszSoundText=NULL;
}

void InitMain()
{
  bInitMain=TRUE;

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

  NewEditProcData=NULL;
  SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NewEditProc, (LPARAM)&NewEditProcData);
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
    StackFreeSounds(&hSoundsStack);
    if (bInitMain) UninitMain();
  }
  return TRUE;
}
