#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "MethodFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include stack functions
#define StackGetIndex
#define StackGetElement
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackMoveAfter
#define StackMoveBefore
#define StackMoveIndex
#define StackDelete
#define StackClear
#define StackSize
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemset
#define xstrcmpW
#define xstrcmpiW
#define xstrcmpinW
#define xstrlenW
#define xstrcpyW
#define xstrcpynW
#define xstrstrW
#define xatoiA
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define hex2decW
#define xprintfW
#define xstrstrW
#include "StrFunc.h"

//Include wide functions
#define AppendMenuWide
#define CallWindowProcWide
#define ComboBox_AddStringWide
#define CreateProcessWide
#define DialogBoxWide
#define ExpandEnvironmentStringsWide
#define GetKeyNameTextWide
#define GetWindowLongPtrWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Include method functions
#define ALLMETHODFUNC
#include "MethodFunc.h"

//Defines
#define DLLA_HOTKEYS_CHECKHOTKEY   1
#define DLLA_HOTKEYS_STARTSTOP     10

#define STRID_PARSEMSG_UNKNOWNMETHOD       1
#define STRID_PARSEMSG_WRONGPARAMCOUNT     2
#define STRID_PARSEMSG_NOCLOSEPARENTHESIS  3
#define STRID_NAME                         4
#define STRID_NAME_AMP                     5
#define STRID_COMMAND                      6
#define STRID_COMMAND_AMP                  7
#define STRID_HOTKEY                       8
#define STRID_KEYAMP                       9
#define STRID_KEYCODE                      10
#define STRID_CODE                         11
#define STRID_GLOBALKEY                    12
#define STRID_ADD                          13
#define STRID_MODIFY                       14
#define STRID_RENAME                       15
#define STRID_MOVEUP                       16
#define STRID_MOVEDOWN                     17
#define STRID_DELETE                       18
#define STRID_ALLKEYS                      19
#define STRID_ALLKEYS_AMP                  20
#define STRID_MENU_ITEMRENAME              21
#define STRID_MENU_ITEMMOVEUP              22
#define STRID_MENU_ITEMMOVEDOWN            23
#define STRID_MENU_ITEMDELETE              24
#define STRID_ONLYASSIGNED                 25
#define STRID_AUTOLOAD                     26
#define STRID_NAME_EXISTS                  27
#define STRID_HOTKEY_EXISTS                28
#define STRID_LOADFIRST                    29
#define STRID_PLUGIN                       30
#define STRID_OK                           31
#define STRID_CANCEL                       32
#define STRID_CLOSE                        33

#define AKDLL_MSGHOTKEYEXISTS (WM_USER + 100)
#define AKDLL_UPDATEADDMODIFY (WM_USER + 101)
#define AKDLL_UPDATEMOVE      (WM_USER + 102)
#define AKDLL_UPDATESELECTION (WM_USER + 103)

#define OF_LISTTEXT       0x1
#define OF_MAINRECT       0x2
#define OF_ALLKEYSRECT    0x4
#define OF_ALLKEYSOPTIONS 0x8

#define LVIS_LIST_NAME     0
#define LVIS_LIST_COMMAND  1
#define LVIS_LIST_HOTKEY   2

#define EXTACT_COMMAND    1
#define EXTACT_CALL       2
#define EXTACT_EXEC       3
#define EXTACT_OPENFILE   4
#define EXTACT_SAVEFILE   5
#define EXTACT_FONT       6
#define EXTACT_RECODE     7
#define EXTACT_INSERT     8

#define FS_NONE            0
#define FS_FONTNORMAL      1
#define FS_FONTBOLD        2
#define FS_FONTITALIC      3
#define FS_FONTBOLDITALIC  4

//ParseCommand return value
#define PCMD_FAILED   0x0
#define PCMD_SUCCESS  0x1
#define PCMD_ERRORMSG 0x2

#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
  #define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#endif
#ifndef LVS_EX_GRIDLINES
  #define LVS_EX_GRIDLINES 0x00000001
#endif
#ifndef LVS_EX_FULLROWSELECT
  #define LVS_EX_FULLROWSELECT 0x00000020
#endif

#define TXT_DEFAULT_HOTKEYS \
L"\"Notepad\" Exec(\"notepad.exe\") Hotkey(0)\r\
\"Case Loop\" Command(4180) Hotkey(0)\r\
\"Paste Case\" Command(4194) Hotkey(0)\r\
\"Last Script\" Call(\"Scripts::Main\", 1, \"\") Hotkey(0)\r\
\"Hex Convert\" Call(\"HexSel::Main\", 2, 24, \"\\x\", \"\", -2, 1200) Hotkey(0)"

typedef struct _HOTKEYITEM {
  struct _HOTKEYITEM *next;
  struct _HOTKEYITEM *prev;
  wchar_t wszHotkeyName[MAX_PATH];
  wchar_t *wpHotkeyCommand;
  DWORD dwHotkey;
  BOOL bGlobal;
  BOOL bAutoLoad;
  DWORD dwAction;
  STACKEXTPARAM hParamStack;
} HOTKEYITEM;

typedef struct _PLUGINCALL {
  unsigned char *lpStruct;
  BOOL bAutoLoad;
} PLUGINCALL;

typedef struct {
  int nCmd;              //Command identifier
  const char *pCmdName;  //Command name
} CMDLIST;

//Functions prototypes
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void HotkeyListFill(HWND hWnd, HSTACK *hStack, const wchar_t *wpFilter);
int HotkeyListGetIndex(HWND hWnd, HOTKEYITEM *hiElement);
int HotkeyListSetItem(HWND hWnd, int nIndex, HOTKEYITEM *hiElement, BOOL bNew, const wchar_t *wpFilter);
void HotkeyListSelItem(HWND hWnd, int nIndex);
BOOL CALLBACK InputDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AllKeysDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewAllKeysFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillAllKeysList(HWND hWndList, HSTACK *hAllKeysStack, const wchar_t *wpFilter, BOOL bOnlyAssigned);
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CreateHotkeyStack(HSTACK *hStack, const wchar_t *wpText);
HOTKEYITEM* ParseCommand(const wchar_t *wpCount, HSTACK *hStack, HOTKEYITEM *hiElement, const wchar_t **wppCount, DWORD *lpdwParseResult);
void ShowError(int nMessageID, const wchar_t *wpLineBegin, const wchar_t *wpCount);
int CallHotkey(HSTACK *hStack, WORD wHotkey, BOOL bGlobal);
HOTKEYITEM* StackInsertHotkey(HSTACK *hStack);
HOTKEYITEM* StackGetHotkeyByHotkey(HSTACK *hStack, DWORD dwHotkey);
HOTKEYITEM* StackGetHotkeyByName(HSTACK *hStack, wchar_t *wpName);
void StackMoveHotkeyByIndex(HSTACK *hStack, int nOldIndex, int nNewIndex);
void StackDeleteHotkey(HSTACK *hStack, HOTKEYITEM *hiElement);
void StackFreeHotkey(HSTACK *hStack);

int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote);
BOOL NextLine(const wchar_t **wpText);
BOOL SkipComment(const wchar_t **wpText);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
INT_PTR TranslateEscapeString(HWND hWndEdit, const wchar_t *wpInput, wchar_t *wszOutput, DWORD *lpdwCaret);

wchar_t* AllocControlText(HWND hWnd, int *lpnTextLen);
int GetControlText(HWND hWnd, wchar_t *wszText, int nTextMax);
int GetHotkeyString(WORD wHotkey, wchar_t *wszString);
int FindAccelerator(ACCEL *lpMainAccelArray, int nMainAccelCount, int nCmd);
int GetCurFile(wchar_t *wszFile, int nMaxFile);
INT_PTR CopyWideStr(const wchar_t *wpSrc, INT_PTR nSrcLen, wchar_t **wppDst);
BOOL FreeWideStr(wchar_t **wppWideStr);
INT_PTR RemoveLeadTrailSpaces(wchar_t *wszText, INT_PTR nTextLen);

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
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HANDLE hHeap;
HINSTANCE hInstanceEXE;
HINSTANCE hInstanceDLL;
STACKPLUGINFUNCTION *hPluginsStack;
HWND hMainWnd;
HWND hMdiClient;
HMENU hMainMenu;
HMENU hMenuRecentFiles;
HACCEL hGlobalAccel;
HACCEL hMainAccel;
BOOL bOldWindows;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
wchar_t wszExeDir[MAX_PATH];
char *szHotkeyText=NULL;
wchar_t *wszHotkeyText=NULL;
HSTACK hHotkeysStack={0};
HWND hWndMainDlg=NULL;
HWND hWndName=NULL;
HWND hWndCommand=NULL;
HWND hWndHotkey=NULL;
HWND hWndHotkeyLabel=NULL;
HWND hWndMainList=NULL;
HWND hWndAllKeysList=NULL;
WNDPROC lpOldMainFilterProc=NULL;
WNDPROC lpOldAllKeysFilterProc=NULL;
RECT rcMainMinMaxDialog={451, 315, 0, 0};
RECT rcMainCurrentDialog={0};
RECT rcAllKeysMinMaxDialog={301, 163, 0, 0};
RECT rcAllKeysCurrentDialog={0};
wchar_t wszMainFilter[MAX_PATH];
wchar_t wszAllKeysFilter[MAX_PATH];
int nColumnWidth1=145;
int nColumnWidth2=276;
int nColumnWidth3=100;
int nAllKeysColumnWidth1=255;
int nAllKeysColumnWidth2=210;
int nAllKeysColumnWidth3=106;
BOOL bAllKeysOnlyAssigned=FALSE;
BOOL bGlobalState=FALSE;
WNDPROCDATA *NewMainProcData=NULL;
CMDLIST cmdMain[]={{4101,  "IDM_FILE_NEW"},
                   {4102,  "IDM_FILE_CREATENEW"},
                   {4103,  "IDM_FILE_OPEN"},
                   {4104,  "IDM_FILE_REOPEN"},
                   {4105,  "IDM_FILE_SAVE"},
                   {4106,  "IDM_FILE_SAVEAS"},
                   {4107,  "IDM_FILE_PAGESETUP"},
                   {4108,  "IDM_FILE_PRINT"},
                   {4109,  "IDM_FILE_EXIT"},
                   {4110,  "IDM_FILE_SAVEALL"},
                   {4111,  "IDM_FILE_SAVEALLAS"},
                   {4113,  "IDM_FILE_SILENTPRINT"},
                   {4114,  "IDM_FILE_PRINTPREVIEW"},
                   {4121,  "IDM_FILE_REDETECT"},
                   {4122,  "IDM_FILE_REOPENAS_ANSI"},
                   {4123,  "IDM_FILE_REOPENAS_OEM"},
                   {4124,  "IDM_FILE_REOPENAS_KOIR"},
                   {4125,  "IDM_FILE_REOPENAS_UTF16LE"},
                   {4126,  "IDM_FILE_REOPENAS_UTF16BE"},
                   {4127,  "IDM_FILE_REOPENAS_UTF8"},
                   {4131,  "IDM_FILE_SAVEAS_ANSI"},
                   {4132,  "IDM_FILE_SAVEAS_OEM"},
                   {4133,  "IDM_FILE_SAVEAS_KOIR"},
                   {4134,  "IDM_FILE_SAVEAS_UTF16LE"},
                   {4135,  "IDM_FILE_SAVEAS_UTF16BE"},
                   {4136,  "IDM_FILE_SAVEAS_UTF8"},
                   {4137,  "IDM_FILE_SAVEAS_UTF8_NOBOM"},
                   {4140,  "IDM_FILE_CODEPAGEMENU"},
                   {4151,  "IDM_EDIT_UNDO"},
                   {4152,  "IDM_EDIT_REDO"},
                   {4153,  "IDM_EDIT_CUT"},
                   {4154,  "IDM_EDIT_COPY"},
                   {4155,  "IDM_EDIT_PASTE"},
                   {4156,  "IDM_EDIT_CLEAR"},
                   {4157,  "IDM_EDIT_SELECTALL"},
                   {4158,  "IDM_EDIT_FIND"},
                   {4159,  "IDM_EDIT_FINDNEXTDOWN"},
                   {4160,  "IDM_EDIT_FINDNEXTUP"},
                   {4161,  "IDM_EDIT_REPLACE"},
                   {4162,  "IDM_EDIT_GOTO"},
                   {4163,  "IDM_EDIT_INSERTCHAR"},
                   {4164,  "IDM_EDIT_INSERT_TAB"},
                   {4165,  "IDM_EDIT_INSERT_TAB_MENU"},
                   {4166,  "IDM_EDIT_DELETE_TAB"},
                   {4167,  "IDM_EDIT_DELETE_TAB_MENU"},
                   {4168,  "IDM_EDIT_INSERT_SPACE"},
                   {4169,  "IDM_EDIT_INSERT_SPACE_MENU"},
                   {4170,  "IDM_EDIT_DELETE_SPACE"},
                   {4171,  "IDM_EDIT_DELETE_SPACE_MENU"},
                   {4172,  "IDM_EDIT_DELETE_FIRST_CHAR"},
                   {4173,  "IDM_EDIT_DELETE_FIRST_CHAR_MENU"},
                   {4174,  "IDM_EDIT_DELETE_TRAILING_WHITESPACES"},
                   {4175,  "IDM_EDIT_UPPERCASE"},
                   {4176,  "IDM_EDIT_LOWERCASE"},
                   {4177,  "IDM_EDIT_SENTENCECASE"},
                   {4178,  "IDM_EDIT_TITLECASE"},
                   {4179,  "IDM_EDIT_INVERTCASE"},
                   {4180,  "IDM_EDIT_LOOPCASE"},
                   {4182,  "IDM_EDIT_RECODE"},
                   {4183,  "IDM_EDIT_INSERTDATE"},
                   {4184,  "IDM_EDIT_NEWLINE_WIN"},
                   {4185,  "IDM_EDIT_NEWLINE_UNIX"},
                   {4186,  "IDM_EDIT_NEWLINE_MAC"},
                   {4190,  "IDM_EDIT_INSERTMODE"},
                   {4191,  "IDM_EDIT_PASTEANSI"},
                   {4192,  "IDM_EDIT_PASTECOLUMN"},
                   {4193,  "IDM_EDIT_PASTEAFTER"},
                   {4194,  "IDM_EDIT_PASTECASE"},
                   {4195,  "IDM_EDIT_DELETESELWHITESPACES"},
                   {4196,  "IDM_EDIT_AUTOINDENT"},
                   {4197,  "IDM_EDIT_DELLINE"},
                   {4198,  "IDM_EDIT_SELJUMPCARET"},
                   {4199,  "IDM_EDIT_RECENTCARETPREV"},
                   {4200,  "IDM_EDIT_RECENTCARETNEXT"},
                   {4201,  "IDM_VIEW_FONT"},
                   {4202,  "IDM_VIEW_COLORS"},
                   {4204,  "IDM_VIEW_FONTSIZE_INCREASE"},
                   {4205,  "IDM_VIEW_FONTSIZE_DECREASE"},
                   {4206,  "IDM_VIEW_FONTSIZE_RESTORE"},
                   {4209,  "IDM_VIEW_WORDWRAP"},
                   {4210,  "IDM_VIEW_ONTOP"},
                   {4211,  "IDM_VIEW_SHOW_STATUSBAR"},
                   {4212,  "IDM_VIEW_SPLIT_WINDOW_ALL"},
                   {4213,  "IDM_VIEW_SPLIT_WINDOW_WE"},
                   {4214,  "IDM_VIEW_SPLIT_WINDOW_NS"},
                   {4215,  "IDM_VIEW_SPLIT_WINDOW_ONOFF"},
                   {4216,  "IDM_VIEW_READONLY"},
                   {4251,  "IDM_OPTIONS_EXEC"},
                   {4252,  "IDM_OPTIONS_SAVETIME"},
                   {4253,  "IDM_OPTIONS_WATCHFILE"},
                   {4254,  "IDM_OPTIONS_KEEPSPACE"},
                   {4255,  "IDM_OPTIONS_SINGLEOPEN_FILE"},
                   {4256,  "IDM_OPTIONS_SINGLEOPEN_PROGRAM"},
                   {4259,  "IDM_OPTIONS_PLUGINS"},
                   {4260,  "IDM_OPTIONS"},
                   {4261,  "IDM_OPTIONS_SDI"},
                   {4262,  "IDM_OPTIONS_MDI"},
                   {4263,  "IDM_OPTIONS_PMDI"},
                   {4301,  "IDM_WINDOW_TABVIEW_TOP"},
                   {4302,  "IDM_WINDOW_TABVIEW_BOTTOM"},
                   {4303,  "IDM_WINDOW_TABVIEW_NONE"},
                   {4304,  "IDM_WINDOW_TABTYPE_STANDARD"},
                   {4305,  "IDM_WINDOW_TABTYPE_BUTTONS"},
                   {4306,  "IDM_WINDOW_TABTYPE_FLATBUTTONS"},
                   {4307,  "IDM_WINDOW_TILEHORIZONTAL"},
                   {4308,  "IDM_WINDOW_TILEVERTICAL"},
                   {4309,  "IDM_WINDOW_CASCADE"},
                   {4310,  "IDM_WINDOW_TABSWITCH_NEXTPREV"},
                   {4311,  "IDM_WINDOW_TABSWITCH_RIGHTLEFT"},
                   {4316,  "IDM_WINDOW_FRAMENEXT"},
                   {4317,  "IDM_WINDOW_FRAMEPREV"},
                   {4318,  "IDM_WINDOW_FRAMECLOSE"},
                   {4319,  "IDM_WINDOW_FRAMECLOSEALL"},
                   {4320,  "IDM_WINDOW_FRAMECLOSEALL_BUTACTIVE"},
                   {4321,  "IDM_WINDOW_FRAMECLOSEALL_UNMODIFIED"},
                   {4322,  "IDM_WINDOW_FRAMECLONE"},
                   {4323,  "IDM_WINDOW_COPYPATH"},
                   {4324,  "IDM_WINDOW_FILECLOSE"},
                   {4325,  "IDM_WINDOW_FILEEXIT"},
                   {4327,  "IDM_WINDOW_MDILIST"},
                   {4331,  "IDM_WINDOW_CHANGESIZE"},
                   {4332,  "IDM_WINDOW_DLGNEXT"},
                   {4333,  "IDM_WINDOW_DLGPREV"},
                   {4341,  "IDM_WINDOW_CLONENEXT"},
                   {4342,  "IDM_WINDOW_CLONEPREV"},
                   {4351,  "IDM_ABOUT"},
                   {4352,  "IDM_MANUAL"},
                   {5001,  "IDM_RECENT_FILES"},
                   {6001,  "IDM_LANGUAGE"},
                   {7001,  "IDM_POPUP_OPENAS"},
                   {8001,  "IDM_POPUP_SAVEAS"},
                   {10019, "IDM_SELECTWINDOW"},
                   {0, 0}};


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Hotkeys";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_HOTKEYS_STARTSTOP)
    {
      if (bInitMain)
      {
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
    else
    {
      if (bInitMain)
      {
        if (nAction == DLLA_HOTKEYS_CHECKHOTKEY)
        {
          HOTKEYITEM *hiElement=NULL;
          DWORD dwHotkey=0;
          BOOL *lpbExist=NULL;
          unsigned char *szName=NULL;
          int nNameMax=0;

          if (IsExtCallParamValid(pd->lParam, 2))
            dwHotkey=(DWORD)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            lpbExist=(BOOL *)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            szName=(unsigned char *)GetExtCallParam(pd->lParam, 4);
          if (IsExtCallParamValid(pd->lParam, 5))
            nNameMax=(int)GetExtCallParam(pd->lParam, 5);

          if (dwHotkey)
          {
            hiElement=StackGetHotkeyByHotkey(&hHotkeysStack, dwHotkey);
          }
          if (lpbExist)
          {
            if (hiElement)
              *lpbExist=TRUE;
            else
              *lpbExist=FALSE;
          }
          if (szName && nNameMax)
          {
             if (hiElement)
             {
              if (pd->dwSupport & PDS_STRANSI)
                WideCharToMultiByte(CP_ACP, 0, hiElement->wszHotkeyName, -1, (char *)szName, nNameMax, NULL, NULL);
              else
                xstrcpynW((wchar_t *)szName, hiElement->wszHotkeyName, nNameMax);
            }
            else
            {
              szName[0]='\0';
              szName[1]='\0';
            }
          }
        }
      }
      else
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_LOADFIRST), L"Hotkeys::Main");
        MessageBoxW(pd->hMainWnd, wszBuffer, L"Hotkeys::Main", MB_OK|MB_ICONEXCLAMATION);
      }

      //If plugin already loaded, stay in memory and don't change active status
      if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
      return;
    }
  }

  //Initialize
  if (!bInitMain) InitMain();

  if (!pd->bOnStart)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_MAIN), hMainWnd, (DLGPROC)MainDlgProc);
  }

  //Stay in memory, and show as active
  pd->nUnload=UD_NONUNLOAD_ACTIVE;
}

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndNameLabel;
  static HWND hWndCommandLabel;
  static HWND hWndHotkeyCode;
  static HWND hWndButtonGroup;
  static HWND hWndAddModifyHotkey;
  static HWND hWndMoveUpHotkey;
  static HWND hWndMoveDownHotkey;
  static HWND hWndDeleteHotkey;
  static HWND hWndAllKeys;
  static HWND hWndClose;
  static HWND hWndFilter;
  static HMENU hMenuList;
  static HOTKEYITEM *hiLastSelItem=NULL;
  static int nSelItem=-1;
  static BOOL bListChanged=FALSE;
  static RESIZEDIALOG rds[]={{&hWndNameLabel,       0, 0},
                             {&hWndName,            0, 0},
                             {&hWndMainList,        RDS_SIZE|RDS_X, 0},
                             {&hWndMainList,        RDS_SIZE|RDS_Y, 0},
                             {&hWndCommandLabel,    RDS_SIZE|RDS_X, 0},
                             {&hWndCommand,         RDS_SIZE|RDS_X, 0},
                             {&hWndHotkeyLabel,     RDS_MOVE|RDS_X, 0},
                             {&hWndHotkeyCode,      RDS_MOVE|RDS_X, 0},
                             {&hWndHotkey,          RDS_MOVE|RDS_X, 0},
                             {&hWndButtonGroup,     RDS_MOVE|RDS_X, 0},
                             {&hWndAddModifyHotkey, RDS_MOVE|RDS_X, 0},
                             {&hWndMoveUpHotkey,    RDS_MOVE|RDS_X, 0},
                             {&hWndMoveDownHotkey,  RDS_MOVE|RDS_X, 0},
                             {&hWndDeleteHotkey,    RDS_MOVE|RDS_X, 0},
                             {&hWndAllKeys,         RDS_MOVE|RDS_X, 0},
                             {&hWndClose,           RDS_MOVE|RDS_X, 0},
                             {&hWndClose,           RDS_MOVE|RDS_Y, 0},
                             {&hWndFilter,          RDS_MOVE|RDS_Y, 0},
                             {&hWndFilter,          RDS_SIZE|RDS_X, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    hWndMainDlg=hDlg;

    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndMainList=GetDlgItem(hDlg, IDC_LIST);
    hWndNameLabel=GetDlgItem(hDlg, IDC_NAME_LABEL);
    hWndName=GetDlgItem(hDlg, IDC_NAME);
    hWndCommandLabel=GetDlgItem(hDlg, IDC_COMMAND_LABEL);
    hWndCommand=GetDlgItem(hDlg, IDC_COMMAND);
    hWndHotkeyLabel=GetDlgItem(hDlg, IDC_HOTKEY_LABEL);
    hWndHotkeyCode=GetDlgItem(hDlg, IDC_HOTKEYCODE);
    hWndHotkey=GetDlgItem(hDlg, IDC_HOTKEY);
    hWndButtonGroup=GetDlgItem(hDlg, IDC_BUTTONGROUP);
    hWndAddModifyHotkey=GetDlgItem(hDlg, IDC_ADDMODIFYHOTKEY);
    hWndMoveUpHotkey=GetDlgItem(hDlg, IDC_ITEMMOVEUP);
    hWndMoveDownHotkey=GetDlgItem(hDlg, IDC_ITEMMOVEDOWN);
    hWndDeleteHotkey=GetDlgItem(hDlg, IDC_ITEMDELETE);
    hWndAllKeys=GetDlgItem(hDlg, IDC_ALLKEYS);
    hWndClose=GetDlgItem(hDlg, IDC_CLOSE);
    hWndFilter=GetDlgItem(hDlg, IDC_MAIN_FILTER);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_NAME_LABEL, GetLangStringW(wLangModule, STRID_NAME_AMP));
    SetDlgItemTextWide(hDlg, IDC_COMMAND_LABEL, GetLangStringW(wLangModule, STRID_COMMAND_AMP));
    SetDlgItemTextWide(hDlg, IDC_HOTKEY_LABEL, GetLangStringW(wLangModule, STRID_KEYAMP));
    SetDlgItemTextWide(hDlg, IDC_ADDMODIFYHOTKEY, GetLangStringW(wLangModule, STRID_ADD));
    SetDlgItemTextWide(hDlg, IDC_ITEMMOVEUP, GetLangStringW(wLangModule, STRID_MOVEUP));
    SetDlgItemTextWide(hDlg, IDC_ITEMMOVEDOWN, GetLangStringW(wLangModule, STRID_MOVEDOWN));
    SetDlgItemTextWide(hDlg, IDC_ITEMDELETE, GetLangStringW(wLangModule, STRID_DELETE));
    SetDlgItemTextWide(hDlg, IDC_ALLKEYS, GetLangStringW(wLangModule, STRID_ALLKEYS_AMP));
    SetDlgItemTextWide(hDlg, IDC_CLOSE, GetLangStringW(wLangModule, STRID_CLOSE));

    SendMessage(hWndName, EM_LIMITTEXT, MAX_PATH, 0);
    EnableWindow(hWndAddModifyHotkey, FALSE);
    EnableWindow(hWndMoveUpHotkey, FALSE);
    EnableWindow(hWndMoveDownHotkey, FALSE);
    EnableWindow(hWndDeleteHotkey, FALSE);
    SendMessage(hWndMainList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);
    SetWindowTextWide(hWndFilter, wszMainFilter);

    //Popup menu
    if (hMenuList=CreatePopupMenu())
    {
      AppendMenuWide(hMenuList, MF_STRING, IDC_ITEMRENAME, GetLangStringW(wLangModule, STRID_MENU_ITEMRENAME));
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
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_NAME);
      lvc.cx=nColumnWidth1;
      lvc.iSubItem=LVIS_LIST_NAME;
      ListView_InsertColumnWide(hWndMainList, LVIS_LIST_NAME, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_COMMAND);
      lvc.cx=nColumnWidth2;
      lvc.iSubItem=LVIS_LIST_COMMAND;
      ListView_InsertColumnWide(hWndMainList, LVIS_LIST_COMMAND, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_HOTKEY);
      lvc.cx=nColumnWidth3;
      lvc.iSubItem=LVIS_LIST_HOTKEY;
      ListView_InsertColumnWide(hWndMainList, LVIS_LIST_HOTKEY, &lvc);
    }
    ComboBox_AddStringWide(hWndCommand, L"Command(0)");
    ComboBox_AddStringWide(hWndCommand, L"Exec(`Program.exe`)");
    ComboBox_AddStringWide(hWndCommand, L"Call(\"Plugin::Function\")");
    ComboBox_AddStringWide(hWndCommand, L"+Call(\"Plugin::Function\")");
    ComboBox_AddStringWide(hWndCommand, L"Font(\"FaceName\", 0, 10)");
    ComboBox_AddStringWide(hWndCommand, L"Recode(0, 0)");
    ComboBox_AddStringWide(hWndCommand, L"Insert(\"text\")");
    ComboBox_AddStringWide(hWndCommand, L"Call(\"Scripts::Main\", 1, \"EvalCmd.js\", `Command(0);`)");

    HotkeyListFill(hWndMainList, &hHotkeysStack, wszMainFilter);

    lpOldMainFilterProc=(WNDPROC)GetWindowLongPtrWide(hWndFilter, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndFilter, GWLP_WNDPROC, (UINT_PTR)NewMainFilterProc);
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndMainList)
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
          SendMessage(hWndMainList, LVM_GETITEMRECT, (WPARAM)nSelItem, (LPARAM)&rcItem);
          ptScreen.x=rcItem.left;
          ptScreen.y=rcItem.bottom;
        }
        ClientToScreen(hWndMainList, &ptScreen);
      }
      else
      {
        GetCursorPos(&ptScreen);
        lvhti.pt=ptScreen;
        ScreenToClient(hWndMainList, &lvhti.pt);
        SendMessage(hWndMainList, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvhti);

        lvi.stateMask=LVIS_SELECTED;
        lvi.state=LVIS_SELECTED;
        SendMessage(hWndMainList, LVM_SETITEMSTATE, (WPARAM)lvhti.iItem, (LPARAM)&lvi);
      }

      EnableMenuItem(hMenuList, IDC_ITEMRENAME, nSelItem >= 0?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_ITEMMOVEUP, !IsWindowEnabled(hWndMoveUpHotkey));
      EnableMenuItem(hMenuList, IDC_ITEMMOVEDOWN, !IsWindowEnabled(hWndMoveDownHotkey));
      EnableMenuItem(hMenuList, IDC_ITEMDELETE, nSelItem >= 0?MF_ENABLED:MF_GRAYED);
      TrackPopupMenu(hMenuList, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, 0, hDlg, NULL);
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
          hiLastSelItem=(HOTKEYITEM *)((NMLISTVIEW *)lParam)->lParam;
          nSelItem=((NMLISTVIEW *)lParam)->iItem;

          SendMessage(hDlg, AKDLL_UPDATESELECTION, 0, 0);
        }
        if (((NMLISTVIEW *)lParam)->uOldState & LVIS_SELECTED)
        {
          nSelItem=-1;

          SendMessage(hDlg, AKDLL_UPDATESELECTION, 0, 0);
        }
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
      {
        if (nSelItem >= 0)
          SendMessage(hWndMainList, LVM_EDITLABEL, nSelItem, 0);
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

        if (pnkd->wVKey == VK_F2)
        {
          if (!bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_ITEMRENAME, 0);
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
      else if (((NMHDR *)lParam)->code == LVN_ENDLABELEDITA)
      {
        NMLVDISPINFOA *nmlvdi=(NMLVDISPINFOA *)lParam;

        if (nmlvdi->item.pszText && nmlvdi->item.pszText[0])
        {
          MultiByteToWideChar(CP_ACP, 0, nmlvdi->item.pszText, -1, wszBuffer, BUFFER_SIZE);

          if (!StackGetHotkeyByName(&hHotkeysStack, wszBuffer))
          {
            if (hiLastSelItem)
            {
              xstrcpynW(hiLastSelItem->wszHotkeyName, wszBuffer, MAX_PATH);
              bListChanged=TRUE;
              SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, 1);
            }
          }
          else PostMessage(hDlg, AKDLL_MSGHOTKEYEXISTS, 0, 0);
        }
        return TRUE;
      }
      else if (((NMHDR *)lParam)->code == LVN_ENDLABELEDITW)
      {
        NMLVDISPINFOW *nmlvdi=(NMLVDISPINFOW *)lParam;

        if (nmlvdi->item.pszText && nmlvdi->item.pszText[0])
        {
          if (!StackGetHotkeyByName(&hHotkeysStack, nmlvdi->item.pszText))
          {
            if (hiLastSelItem)
            {
              xstrcpynW(hiLastSelItem->wszHotkeyName, nmlvdi->item.pszText, MAX_PATH);
              bListChanged=TRUE;
              SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, 1);
            }
          }
          else PostMessage(hDlg, AKDLL_MSGHOTKEYEXISTS, 0, 0);
        }
        return TRUE;
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_CUSTOMDRAW)
      {
        LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)lParam;
        LRESULT lResult;

        if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
          lResult=CDRF_NOTIFYITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
          lResult=CDRF_NOTIFYSUBITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == (CDDS_SUBITEM|CDDS_ITEMPREPAINT))
        {
          if (lplvcd->iSubItem == LVIS_LIST_HOTKEY)
          {
            HOTKEYITEM *hiElement=(HOTKEYITEM *)lplvcd->nmcd.lItemlParam;

            if (SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, hiElement->dwHotkey))
            {
              lplvcd->clrText=RGB(0xFF, 0x00, 0x00);
              lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
            }
            else if (hiElement->bGlobal)
            {
              lplvcd->clrText=RGB(0x00, 0x00, 0xFF);
              lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
            }
          }
          lResult=CDRF_DODEFAULT;
        }
        else lResult=CDRF_DODEFAULT;

        SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, lResult);
        return TRUE;
      }
    }
  }
  else if (uMsg == AKDLL_MSGHOTKEYEXISTS)
  {
    MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_NAME_EXISTS), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
  }
  else if (uMsg == AKDLL_UPDATEADDMODIFY)
  {
    wchar_t wszHotkeyName[MAX_PATH];
    wchar_t *wszHotkeyCommand;
    HOTKEYITEM *hiElement;
    HWND hWndFocus;
    WORD wHotkey;
    BOOL bEnable=FALSE;

    hWndFocus=GetFocus();
    SetWindowTextWide(hWndAddModifyHotkey, GetLangStringW(wLangModule, STRID_ADD));

    if (GetControlText(hWndName, wszHotkeyName, MAX_PATH))
    {
      if (wszHotkeyCommand=AllocControlText(hWndCommand, NULL))
      {
        bEnable=TRUE;
        RemoveLeadTrailSpaces(wszHotkeyCommand, -1);
        wHotkey=(WORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0);
        xprintfW(wszBuffer, L"%s (%d)", GetLangStringW(wLangModule, STRID_KEYAMP), wHotkey);
        SetWindowTextWide(hWndHotkeyLabel, wszBuffer);

        if (hiElement=StackGetHotkeyByName(&hHotkeysStack, wszHotkeyName))
        {
          SetWindowTextWide(hWndAddModifyHotkey, GetLangStringW(wLangModule, STRID_MODIFY));
          if (!xstrcmpW(wszHotkeyCommand, hiElement->wpHotkeyCommand) && hiElement->dwHotkey == wHotkey && hiElement->bGlobal == bGlobalState)
            bEnable=FALSE;
        }
        GlobalFree((HGLOBAL)wszHotkeyCommand);
      }
    }

    if (!bEnable)
    {
      if (hWndFocus == hWndAddModifyHotkey)
        SetFocus(hWndMainList);
    }
    EnableWindow(hWndAddModifyHotkey, bEnable);
  }
  else if (uMsg == AKDLL_UPDATEMOVE)
  {
    if (nSelItem == 0 || *wszMainFilter)
      EnableWindow(hWndMoveUpHotkey, FALSE);
    else
      EnableWindow(hWndMoveUpHotkey, TRUE);
    if (SendMessage(hWndMainList, LVM_GETITEMCOUNT, 0, 0) == nSelItem + 1 || *wszMainFilter)
      EnableWindow(hWndMoveDownHotkey, FALSE);
    else
      EnableWindow(hWndMoveDownHotkey, TRUE);
  }
  else if (uMsg == AKDLL_UPDATESELECTION)
  {
    if (nSelItem != -1 && hiLastSelItem)
    {
      PLUGINFUNCTION *pfElement;

      SendMessage(hDlg, AKDLL_UPDATEMOVE, 0, 0);
      EnableWindow(hWndDeleteHotkey, TRUE);

      SetWindowTextWide(hWndName, hiLastSelItem->wszHotkeyName);
      SetWindowTextWide(hWndCommand, hiLastSelItem->wpHotkeyCommand);
      SendMessage(hWndHotkey, HKM_SETHOTKEY, hiLastSelItem->dwHotkey, 0);
      bGlobalState=hiLastSelItem->bGlobal;

      if (pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, hiLastSelItem->dwHotkey))
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_HOTKEY_EXISTS), pfElement->wszFunction);
        MessageBoxW(hDlg, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
      }
    }
    else
    {
      EnableWindow(hWndMoveUpHotkey, FALSE);
      EnableWindow(hWndMoveDownHotkey, FALSE);
      EnableWindow(hWndDeleteHotkey, FALSE);
    }
    PostMessage(hDlg, AKDLL_UPDATEADDMODIFY, 0, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_MAIN_FILTER)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        int nIndex;

        GetWindowTextWide(hWndFilter, wszMainFilter, MAX_PATH);
        HotkeyListFill(hWndMainList, &hHotkeysStack, wszMainFilter);
        SendMessage(hDlg, AKDLL_UPDATEMOVE, 0, 0);

        if ((nIndex=HotkeyListGetIndex(hWndMainList, hiLastSelItem)) != -1)
          HotkeyListSelItem(hWndMainList, nIndex);
        else
          nSelItem=-1;
        SendMessage(hDlg, AKDLL_UPDATESELECTION, 0, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_NAME ||
             LOWORD(wParam) == IDC_COMMAND ||
             LOWORD(wParam) == IDC_HOTKEY)
    {
      PostMessage(hDlg, AKDLL_UPDATEADDMODIFY, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_HOTKEYCODE)
    {
      if (DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_INPUTBOX), hDlg, (DLGPROC)InputDlgProc))
        PostMessage(hDlg, AKDLL_UPDATEADDMODIFY, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_ADDMODIFYHOTKEY)
    {
      PLUGINFUNCTION *pfElement=NULL;
      HOTKEYITEM *hiNewElement=NULL;
      HOTKEYITEM *hiOldElement=NULL;
      HOTKEYITEM *hiElement=NULL;
      wchar_t wszHotkeyName[MAX_PATH];
      wchar_t *wszHotkeyCommand;
      DWORD dwParse;
      WORD wHotkey;
      int nNewIndex;
      int nOldIndex=-1;
      int a;
      int b;
      BOOL bAddHotkey=TRUE;

      //Hotkey name
      GetControlText(hWndName, wszHotkeyName, MAX_PATH);
      wHotkey=(WORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0);
      if (hiOldElement=StackGetHotkeyByName(&hHotkeysStack, wszHotkeyName))
      {
        bAddHotkey=FALSE;
        nOldIndex=HotkeyListGetIndex(hWndMainList, hiOldElement);
      }

      //Remove quotes
      for (a=0, b=0; wszHotkeyName[b];)
      {
        if (wszHotkeyName[b] == L'\"')
          ++b;
        else
          wszHotkeyName[a++]=wszHotkeyName[b++];
      }
      wszHotkeyName[a]=L'\0';

      if (wHotkey)
      {
        //Is hotkey already assigned via AKD_DLLADD
        if (pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, wHotkey))
        {
          xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_HOTKEY_EXISTS), pfElement->wszFunction);
          MessageBoxW(hDlg, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
          SetFocus(hWndHotkey);
          return FALSE;
        }

        //Is hotkey already assigned via HotKeys plugin
        if (hiElement=StackGetHotkeyByHotkey(&hHotkeysStack, wHotkey))
        {
          if (!hiOldElement || hiOldElement != hiElement)
          {
            xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_HOTKEY_EXISTS), hiElement->wszHotkeyName);
            MessageBoxW(hDlg, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
            SetFocus(hWndHotkey);
            return FALSE;
          }
        }
      }

      //Hotkey command
      if (wszHotkeyCommand=AllocControlText(hWndCommand, NULL))
      {
        if (hiNewElement=ParseCommand(wszHotkeyCommand, &hHotkeysStack, hiOldElement, NULL, &dwParse))
        {
          xstrcpynW(hiNewElement->wszHotkeyName, wszHotkeyName, MAX_PATH);
          hiNewElement->dwHotkey=wHotkey;
          hiNewElement->bGlobal=bGlobalState;
          SendMessage(hWndHotkey, HKM_SETHOTKEY, hiNewElement->dwHotkey, 0);
        }
        GlobalFree((HGLOBAL)wszHotkeyCommand);

        if (dwParse & PCMD_ERRORMSG)
        {
          SetFocus(hWndCommand);
          return FALSE;
        }
      }
      nNewIndex=HotkeyListSetItem(hWndMainList, nOldIndex, hiNewElement, bAddHotkey, NULL);

      if (nNewIndex >= 0)
      {
        HotkeyListSelItem(hWndMainList, nNewIndex);
        bListChanged=TRUE;
      }
      PostMessage(hDlg, AKDLL_UPDATEADDMODIFY, 0, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_ITEMRENAME)
    {
      if (nSelItem != -1)
        SendMessage(hWndMainList, LVM_EDITLABEL, nSelItem, 0);
    }
    else if (LOWORD(wParam) == IDC_ITEMMOVEUP ||
             LOWORD(wParam) == IDC_ITEMMOVEDOWN)
    {
      if (nSelItem != -1 && hiLastSelItem && !*wszMainFilter)
      {
        HOTKEYITEM *hiOldItem=hiLastSelItem;
        int nOldIndex=nSelItem;
        int nNewIndex;

        //Delete
        SendMessage(hWndMainList, LVM_DELETEITEM, nOldIndex, 0);

        //Add
        if (LOWORD(wParam) == IDC_ITEMMOVEUP)
          nNewIndex=max(nOldIndex - 1, 0);
        else
          nNewIndex=nOldIndex + 1;
        nNewIndex=HotkeyListSetItem(hWndMainList, nNewIndex, hiOldItem, TRUE, NULL);

        //Move item in stack
        StackMoveHotkeyByIndex(&hHotkeysStack, nOldIndex + 1, nNewIndex + 1);

        //Select
        SetFocus(hWndMainList);
        HotkeyListSelItem(hWndMainList, nNewIndex);

        bListChanged=TRUE;
      }
    }
    else if (LOWORD(wParam) == IDC_ITEMDELETE)
    {
      if (nSelItem != -1)
      {
        int nIndex=nSelItem;

        //Delete
        StackDeleteHotkey(&hHotkeysStack, hiLastSelItem);
        hiLastSelItem=NULL;
        SendMessage(hWndMainList, LVM_DELETEITEM, nIndex, 0);
        nIndex=min(nIndex, (int)SendMessage(hWndMainList, LVM_GETITEMCOUNT, 0, 0) - 1);

        //Select
        SetFocus(hWndMainList);
        HotkeyListSelItem(hWndMainList, nIndex);
        bListChanged=TRUE;
      }
    }
    else if (LOWORD(wParam) == IDC_ALLKEYS)
    {

      int nAllKeysColumnWidthOld1=nAllKeysColumnWidth1;
      int nAllKeysColumnWidthOld2=nAllKeysColumnWidth2;
      int nAllKeysColumnWidthOld3=nAllKeysColumnWidth3;
      BOOL bAllKeysOnlyAssignedOld=bAllKeysOnlyAssigned;

      DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_ALLKEYS), hDlg, (DLGPROC)AllKeysDlgProc);

      if (bAllKeysOnlyAssignedOld != bAllKeysOnlyAssigned)
      {
        dwSaveFlags|=OF_ALLKEYSOPTIONS;
      }
      if (nAllKeysColumnWidthOld1 != nAllKeysColumnWidth1 ||
          nAllKeysColumnWidthOld2 != nAllKeysColumnWidth2 ||
          nAllKeysColumnWidthOld3 != nAllKeysColumnWidth3)
      {
        dwSaveFlags|=OF_ALLKEYSRECT;
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (GetFocus() == hWndMainList)
        SendMessage(hWndMainList, LVM_EDITLABEL, nSelItem, 0);
    }
    else if (LOWORD(wParam) == IDC_CLOSE ||
             LOWORD(wParam) == IDCANCEL)
    {
      int nWidth;

      nWidth=(int)SendMessage(hWndMainList, LVM_GETCOLUMNWIDTH, LVIS_LIST_NAME, 0);
      if (nColumnWidth1 != nWidth)
      {
        nColumnWidth1=nWidth;
        dwSaveFlags|=OF_MAINRECT;
      }
      nWidth=(int)SendMessage(hWndMainList, LVM_GETCOLUMNWIDTH, LVIS_LIST_COMMAND, 0);
      if (nColumnWidth2 != nWidth)
      {
        nColumnWidth2=nWidth;
        dwSaveFlags|=OF_MAINRECT;
      }
      nWidth=(int)SendMessage(hWndMainList, LVM_GETCOLUMNWIDTH, LVIS_LIST_HOTKEY, 0);
      if (nColumnWidth3 != nWidth)
      {
        nColumnWidth3=nWidth;
        dwSaveFlags|=OF_MAINRECT;
      }

      if (bListChanged)
      {
        HOTKEYITEM *hiElement;
        DWORD dwSize;

        for (dwSize=0, hiElement=(HOTKEYITEM *)hHotkeysStack.first; hiElement; hiElement=hiElement->next)
        {
          dwSize+=(DWORD)xprintfW(NULL, L"\"%s\" %s Hotkey(%d%s)\r", hiElement->wszHotkeyName, hiElement->wpHotkeyCommand, hiElement->dwHotkey, (hiElement->bGlobal?L", 1":L"")) - 1;
        }
        if (wszHotkeyText=(wchar_t *)HeapAlloc(hHeap, 0, (dwSize + 1) * sizeof(wchar_t)))
        {
          for (dwSize=0, hiElement=(HOTKEYITEM *)hHotkeysStack.first; hiElement; hiElement=hiElement->next)
          {
            dwSize+=(DWORD)xprintfW(wszHotkeyText + dwSize, L"\"%s\" %s Hotkey(%d%s)\r", hiElement->wszHotkeyName, hiElement->wpHotkeyCommand, hiElement->dwHotkey, (hiElement->bGlobal?L", 1":L""));
          }
        }
        dwSaveFlags|=OF_LISTTEXT;
      }
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      if (wszHotkeyText)
      {
        HeapFree(hHeap, 0, wszHotkeyText);
        wszHotkeyText=NULL;
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
    //Destroy resources
    DestroyMenu(hMenuList);
    DestroyIcon(hPluginIcon);
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      dwSaveFlags|=OF_MAINRECT;
  }

  return FALSE;
}

LRESULT CALLBACK NewMainFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_DOWN || wParam == VK_UP)
    {
      if (GetFocus() != hWndMainList)
        SetFocus(hWndMainList);
      return SendMessage(hWndMainList, uMsg, wParam, lParam);
    }
  }
  return CallWindowProcWide(lpOldMainFilterProc, hWnd, uMsg, wParam, lParam);
}

void HotkeyListFill(HWND hWnd, HSTACK *hStack, const wchar_t *wpFilter)
{
  HOTKEYITEM *hiElement=(HOTKEYITEM *)hStack->first;
  int i=0;

  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SendMessage(hWnd, LVM_DELETEALLITEMS, 0, 0);

  while (hiElement)
  {
    HotkeyListSetItem(hWnd, i++, hiElement, TRUE, wpFilter);

    hiElement=hiElement->next;
  }

  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
}

int HotkeyListGetIndex(HWND hWnd, HOTKEYITEM *hiElement)
{
  LVFINDINFOA lvfi;

  lvfi.flags=LVFI_PARAM;
  lvfi.lParam=(LPARAM)hiElement;
  return (int)SendMessage(hWnd, LVM_FINDITEMA, (WPARAM)-1, (LPARAM)&lvfi);
}

int HotkeyListSetItem(HWND hWnd, int nIndex, HOTKEYITEM *hiElement, BOOL bNew, const wchar_t *wpFilter)
{
  wchar_t wszHotkey[MAX_PATH];
  LVITEMW lvi;

  if (hiElement)
  {
    GetHotkeyString((WORD)hiElement->dwHotkey, wszHotkey);

    if (!wpFilter || !*wpFilter ||
        xstrstrW(hiElement->wszHotkeyName, -1, wpFilter, -1, FALSE, NULL, NULL) ||
        xstrstrW(hiElement->wpHotkeyCommand, -1, wpFilter, -1, FALSE, NULL, NULL) ||
        xstrstrW(wszHotkey, -1, wpFilter, -1, FALSE, NULL, NULL))
    {
      if (bNew)
      {
        lvi.mask=LVIF_PARAM;
        lvi.lParam=(LPARAM)hiElement;
        lvi.iItem=(nIndex > -1)?nIndex:0x7FFFFFFF;
        lvi.iSubItem=LVIS_LIST_NAME;
        nIndex=ListView_InsertItemWide(hWnd, &lvi);
      }

      lvi.mask=LVIF_TEXT;
      lvi.pszText=hiElement->wszHotkeyName;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVIS_LIST_NAME;
      ListView_SetItemWide(hWnd, &lvi);

      lvi.mask=LVIF_TEXT;
      lvi.pszText=hiElement->wpHotkeyCommand;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVIS_LIST_COMMAND;
      ListView_SetItemWide(hWnd, &lvi);

      lvi.mask=LVIF_TEXT;
      lvi.pszText=wszHotkey;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVIS_LIST_HOTKEY;
      ListView_SetItemWide(hWnd, &lvi);

      return nIndex;
    }
  }
  return -1;
}

void HotkeyListSelItem(HWND hWnd, int nIndex)
{
  LVITEMA lvi;

  lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
  lvi.state=LVIS_SELECTED|LVIS_FOCUSED;
  SendMessage(hWnd, LVM_SETITEMSTATE, nIndex, (LPARAM)&lvi);
}

BOOL CALLBACK InputDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCheck;
  static HICON hPluginIcon;
  DWORD dwHotkey;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndCheck=GetDlgItem(hDlg, IDC_INPUTBOX_CHECK);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_KEYCODE));
    SetDlgItemTextWide(hDlg, IDC_INPUTBOX_LABEL, GetLangStringW(wLangModule, STRID_CODE));
    SetDlgItemTextWide(hDlg, IDC_INPUTBOX_CHECK, GetLangStringW(wLangModule, STRID_GLOBALKEY));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    dwHotkey=(DWORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0);
    SetDlgItemInt(hDlg, IDC_INPUTBOX_EDIT, dwHotkey, FALSE);
    if (bGlobalState) SendMessage(hWndCheck, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      dwHotkey=GetDlgItemInt(hDlg, IDC_INPUTBOX_EDIT, NULL, FALSE);
      SendMessage(hWndHotkey, HKM_SETHOTKEY, dwHotkey, 0);
      bGlobalState=(BOOL)SendMessage(hWndCheck, BM_GETCHECK, 0, 0);
      EndDialog(hDlg, 1);
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
    }
    return TRUE;
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

BOOL CALLBACK AllKeysDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndFilter;
  static HWND hWndOnlyAssigned;
  static HSTACK hAllKeysStack={0};
  static RESIZEDIALOG rds[]={{&hWndAllKeysList,  RDS_SIZE|RDS_X, 0},
                             {&hWndAllKeysList,  RDS_SIZE|RDS_Y, 0},
                             {&hWndFilter,       RDS_MOVE|RDS_Y, 0},
                             {&hWndOnlyAssigned, RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndAllKeysList=GetDlgItem(hDlg, IDC_ALLKEYS_LIST);
    hWndFilter=GetDlgItem(hDlg, IDC_ALLKEYS_FILTER);
    hWndOnlyAssigned=GetDlgItem(hDlg, IDC_ALLKEYS_ONLYASSIGNED);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_ALLKEYS));
    SetDlgItemTextWide(hDlg, IDC_ALLKEYS_ONLYASSIGNED, GetLangStringW(wLangModule, STRID_ONLYASSIGNED));

    SendMessage(hWndAllKeysList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
    SetWindowTextWide(hWndFilter, wszAllKeysFilter);
    if (bAllKeysOnlyAssigned) SendMessage(hWndOnlyAssigned, BM_SETCHECK, BST_CHECKED, 0);

    //Columns
    {
      LVCOLUMNW lvc;

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_NAME);
      lvc.cx=nAllKeysColumnWidth1;
      lvc.iSubItem=LVIS_LIST_NAME;
      ListView_InsertColumnWide(hWndAllKeysList, LVIS_LIST_NAME, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_COMMAND);
      lvc.cx=nAllKeysColumnWidth2;
      lvc.iSubItem=LVIS_LIST_COMMAND;
      ListView_InsertColumnWide(hWndAllKeysList, LVIS_LIST_COMMAND, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_HOTKEY);
      lvc.cx=nAllKeysColumnWidth3;
      lvc.iSubItem=LVIS_LIST_HOTKEY;
      ListView_InsertColumnWide(hWndAllKeysList, LVIS_LIST_HOTKEY, &lvc);
    }
    FillAllKeysList(hWndAllKeysList, &hAllKeysStack, wszAllKeysFilter, bAllKeysOnlyAssigned);

    lpOldAllKeysFilterProc=(WNDPROC)GetWindowLongPtrWide(hWndFilter, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndFilter, GWLP_WNDPROC, (UINT_PTR)NewAllKeysFilterProc);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_ALLKEYS_LIST)
    {
      if (((NMHDR *)lParam)->code == LVN_COLUMNCLICK)
      {
        NMLISTVIEW *pnmlv=(NMLISTVIEW *)lParam;

        SendMessage(hWndAllKeysList, LVM_SORTITEMS, pnmlv->iSubItem, (LPARAM)CompareFunc);
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
      {
        PostMessage(hDlg, WM_COMMAND, IDOK, 0);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_ALLKEYS_FILTER)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        GetWindowTextWide(hWndFilter, wszAllKeysFilter, MAX_PATH);
        FillAllKeysList(hWndAllKeysList, &hAllKeysStack, wszAllKeysFilter, bAllKeysOnlyAssigned);
      }
    }
    else if (LOWORD(wParam) == IDC_ALLKEYS_ONLYASSIGNED)
    {
      bAllKeysOnlyAssigned=(BOOL)SendMessage(hWndOnlyAssigned, BM_GETCHECK, 0, 0);
      FillAllKeysList(hWndAllKeysList, &hAllKeysStack, wszAllKeysFilter, bAllKeysOnlyAssigned);
    }

    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDCANCEL)
    {
      nAllKeysColumnWidth1=(int)SendMessage(hWndAllKeysList, LVM_GETCOLUMNWIDTH, LVIS_LIST_NAME, 0);
      nAllKeysColumnWidth2=(int)SendMessage(hWndAllKeysList, LVM_GETCOLUMNWIDTH, LVIS_LIST_COMMAND, 0);
      nAllKeysColumnWidth3=(int)SendMessage(hWndAllKeysList, LVM_GETCOLUMNWIDTH, LVIS_LIST_HOTKEY, 0);

      if (LOWORD(wParam) == IDOK)
      {
        LVITEMA lvi;
        HOTKEYITEM *hiElement;

        xmemset(&lvi, 0, sizeof(LVITEMA));
        lvi.mask=LVIF_PARAM;
        lvi.iItem=(int)SendMessage(hWndAllKeysList, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED);
        lvi.iSubItem=LVIS_LIST_NAME;
        SendMessage(hWndAllKeysList, LVM_GETITEMA, 0, (LPARAM)&lvi);
        hiElement=(HOTKEYITEM *)lvi.lParam;

        if (hiElement)
        {
          SetWindowTextWide(hWndName, hiElement->wszHotkeyName);
          SetWindowTextWide(hWndCommand, hiElement->wpHotkeyCommand);
          SendMessage(hWndHotkey, HKM_SETHOTKEY, hiElement->dwHotkey, 0);
          bGlobalState=FALSE;

          PostMessage(hWndMainDlg, AKDLL_UPDATEADDMODIFY, 0, 0);
        }
        StackFreeHotkey(&hAllKeysStack);
        EndDialog(hDlg, 0);
        return TRUE;
      }
      else if (LOWORD(wParam) == IDCANCEL)
      {
        StackFreeHotkey(&hAllKeysStack);
        EndDialog(hDlg, 0);
        return TRUE;
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
    RESIZEDIALOGMSG rdsm={&rds[0], &rcAllKeysMinMaxDialog, &rcAllKeysCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      dwSaveFlags|=OF_ALLKEYSRECT;
  }

  return FALSE;
}

LRESULT CALLBACK NewAllKeysFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_DOWN || wParam == VK_UP)
    {
      if (GetFocus() != hWndAllKeysList)
        SetFocus(hWndAllKeysList);
      return SendMessage(hWndAllKeysList, uMsg, wParam, lParam);
    }
  }
  return CallWindowProcWide(lpOldAllKeysFilterProc, hWnd, uMsg, wParam, lParam);
}

void FillAllKeysList(HWND hWndList, HSTACK *hAllKeysStack, const wchar_t *wpFilter, BOOL bOnlyAssigned)
{
  HOTKEYITEM *hiItem;
  wchar_t wszHotkeyCommand[MAX_PATH];
  int nInsertItem=0;

  SendMessage(hWndList, WM_SETREDRAW, FALSE, 0);
  SendMessage(hWndList, LVM_DELETEALLITEMS, 0, 0);
  StackFreeHotkey(hAllKeysStack);

  //Accelerators
  {
    ACCEL *lpGlobalAccelArray;
    ACCEL *lpMainAccelArray;
    ACCEL *lpAccelItem;
    int nAccelGlobalFind;
    int nAccelMainFind;
    int nAccelGlobalIndex;
    int nAccelMainIndex;
    int nGlobalAccelCount;
    int nMainAccelCount;
    BYTE nMod;
    WORD wVirtKey;
    int i;

    //Get table size
    nGlobalAccelCount=CopyAcceleratorTable(hGlobalAccel, NULL, 0);
    lpGlobalAccelArray=(ACCEL *)GlobalAlloc(GPTR, nGlobalAccelCount * sizeof(ACCEL));

    nMainAccelCount=CopyAcceleratorTable(hMainAccel, NULL, 0);
    lpMainAccelArray=(ACCEL *)GlobalAlloc(GPTR, nMainAccelCount * sizeof(ACCEL));

    if (lpGlobalAccelArray && lpMainAccelArray)
    {
      //Get table array
      CopyAcceleratorTable(hGlobalAccel, lpGlobalAccelArray, nGlobalAccelCount);
      CopyAcceleratorTable(hMainAccel, lpMainAccelArray, nMainAccelCount);

      for (i=0; cmdMain[i].nCmd; ++i)
      {
        nAccelGlobalFind=0;
        nAccelMainFind=0;
        nAccelGlobalIndex=-1;
        nAccelMainIndex=-1;
        lpAccelItem=NULL;

        for (;;)
        {
          if (nAccelGlobalFind != -1 && (nAccelGlobalFind=FindAccelerator(lpGlobalAccelArray + (nAccelGlobalIndex + 1), nGlobalAccelCount - (nAccelGlobalIndex + 1), cmdMain[i].nCmd)) != -1)
          {
            nAccelGlobalIndex+=nAccelGlobalFind + 1;
            lpAccelItem=lpGlobalAccelArray + nAccelGlobalIndex;
          }
          else if (nAccelMainFind != -1 && (nAccelMainFind=FindAccelerator(lpMainAccelArray + (nAccelMainIndex + 1), nMainAccelCount - (nAccelMainIndex + 1), cmdMain[i].nCmd)) != -1)
          {
            nAccelMainIndex+=nAccelMainFind + 1;
            lpAccelItem=lpMainAccelArray + nAccelMainIndex;
          }
          else
          {
            if (lpAccelItem || bOnlyAssigned)
              break;
          }

          if (lpAccelItem || !bOnlyAssigned)
          {
            if (hiItem=StackInsertHotkey(hAllKeysStack))
            {
              //hiItem->wszHotkeyName
              MultiByteToWideChar(CP_ACP, 0, cmdMain[i].pCmdName, -1, hiItem->wszHotkeyName, MAX_PATH);

              //hiItem->wpHotkeyCommand
              xprintfW(wszHotkeyCommand, L"Command(%d)", cmdMain[i].nCmd);
              CopyWideStr(wszHotkeyCommand, -1, &hiItem->wpHotkeyCommand);

              //hiItem->dwHotkey
              if (lpAccelItem)
              {
                nMod=0;
                if (lpAccelItem->fVirt & FSHIFT)
                  nMod|=HOTKEYF_SHIFT;
                if (lpAccelItem->fVirt & FCONTROL)
                  nMod|=HOTKEYF_CONTROL;
                if (lpAccelItem->fVirt & FALT)
                  nMod|=HOTKEYF_ALT;
                if (lpAccelItem->fVirt & FVIRTKEY)
                {
                  wVirtKey=lpAccelItem->key;

                  if (wVirtKey == VK_INSERT ||
                      wVirtKey == VK_DELETE ||
                      wVirtKey == VK_HOME ||
                      wVirtKey == VK_END ||
                      wVirtKey == VK_NEXT ||
                      wVirtKey == VK_PRIOR ||
                      wVirtKey == VK_LEFT ||
                      wVirtKey == VK_RIGHT ||
                      wVirtKey == VK_UP ||
                      wVirtKey == VK_DOWN)
                  {
                    nMod|=HOTKEYF_EXT;
                  }
                }
                else
                {
                  //lpAccelItem->key is character code, convert it to virtual key.
                  if (bOldWindows)
                    wVirtKey=VkKeyScanA((char)lpAccelItem->key);
                  else
                    wVirtKey=VkKeyScanW((wchar_t)lpAccelItem->key);
                  wVirtKey=LOBYTE(wVirtKey);
                }
                hiItem->dwHotkey=MAKEWORD(wVirtKey, nMod);
              }
              else hiItem->dwHotkey=0;

              if (HotkeyListSetItem(hWndList, -1, hiItem, TRUE, wszAllKeysFilter) == -1)
                StackDeleteHotkey(hAllKeysStack, hiItem);
            }

            if (!lpAccelItem && !bOnlyAssigned)
              break;
          }
        }
      }
      SendMessage(hWndList, LVM_SORTITEMS, LVIS_LIST_COMMAND, (LPARAM)CompareFunc);
    }
    if (lpGlobalAccelArray) GlobalFree((HGLOBAL)lpGlobalAccelArray);
    if (lpMainAccelArray) GlobalFree((HGLOBAL)lpMainAccelArray);
  }

  //All plugins
  {
    PLUGINFUNCTION *pfElement;
    DWORD dwHotkey;
    const wchar_t *wpStrBegin;
    const wchar_t *wpStrEnd;
    int nFound;

    for (pfElement=hPluginsStack->first; pfElement; pfElement=pfElement->next)
    {
      dwHotkey=pfElement->wHotkey;

      if (dwHotkey)
      {
        if (hiItem=StackInsertHotkey(hAllKeysStack))
        {
          //hiItem->wszHotkeyName
          xstrcpynW(hiItem->wszHotkeyName, pfElement->wszFunction, MAX_PATH);

          //hiItem->wpHotkeyCommand
          nFound=0;
          if (nFound+=xstrstrW(hiItem->wszHotkeyName, -1, L"::", 2, TRUE, &wpStrBegin, &wpStrEnd))
            nFound+=xstrstrW(wpStrEnd, -1, L"::", 2, TRUE, &wpStrBegin, &wpStrEnd);

          if (nFound > 0)
          {
            if (nFound == 2 && !xstrcmpinW(L"Scripts::Main", hiItem->wszHotkeyName, (UINT_PTR)-1))
              xprintfW(wszHotkeyCommand, L"Call(\"Scripts::Main\", 1, \"%s\")", wpStrEnd);
            else if (nFound == 2 && !xstrcmpinW(L"Macros::Main", hiItem->wszHotkeyName, (UINT_PTR)-1))
              xprintfW(wszHotkeyCommand, L"Call(\"Macros::Main\", 1, \"%s\")", wpStrEnd);
            else
              xprintfW(wszHotkeyCommand, L"Call(\"%s\")", hiItem->wszHotkeyName);
          }
          else wszHotkeyCommand[0]=L'\0';

          CopyWideStr(wszHotkeyCommand, -1, &hiItem->wpHotkeyCommand);

          //hiItem->dwHotkey
          hiItem->dwHotkey=dwHotkey;

          if (HotkeyListSetItem(hWndList, nInsertItem++, hiItem, TRUE, wszAllKeysFilter) == -1)
            StackDeleteHotkey(hAllKeysStack, hiItem);
        }
      }
    }
  }

  //Hotkeys plugin
  {
    HOTKEYITEM *hiElement;

    for (hiElement=(HOTKEYITEM *)hHotkeysStack.first; hiElement; hiElement=hiElement->next)
    {
      if (hiElement->dwHotkey)
      {
        if (hiItem=StackInsertHotkey(hAllKeysStack))
        {
          //hiItem->wszHotkeyName
          xprintfW(hiItem->wszHotkeyName, L"Hotkeys::%s", hiElement->wszHotkeyName);

          //hiItem->wpHotkeyCommand
          CopyWideStr(hiElement->wpHotkeyCommand, -1, &hiItem->wpHotkeyCommand);

          //hiItem->dwHotkey
          hiItem->dwHotkey=hiElement->dwHotkey;

          if (HotkeyListSetItem(hWndList, nInsertItem++, hiItem, TRUE, wszAllKeysFilter) == -1)
            StackDeleteHotkey(hAllKeysStack, hiItem);
        }
      }
    }
  }

  SendMessage(hWndList, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWndList, NULL, TRUE);
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  HOTKEYITEM *hiElement1=(HOTKEYITEM *)lParam1;
  HOTKEYITEM *hiElement2=(HOTKEYITEM *)lParam2;

  if (lParamSort == LVIS_LIST_NAME)
  {
    return xstrcmpiW(hiElement1->wszHotkeyName, hiElement2->wszHotkeyName);
  }
  else if (lParamSort == LVIS_LIST_COMMAND)
  {
    return xstrcmpiW(hiElement1->wpHotkeyCommand, hiElement2->wpHotkeyCommand);
  }
  else if (lParamSort == LVIS_LIST_HOTKEY)
  {
    wchar_t wszHotkeyName1[MAX_PATH];
    wchar_t wszHotkeyName2[MAX_PATH];

    GetHotkeyString((WORD)hiElement1->dwHotkey, wszHotkeyName1);
    GetHotkeyString((WORD)hiElement2->dwHotkey, wszHotkeyName2);
    return xstrcmpiW(wszHotkeyName1, wszHotkeyName2);
  }
  return 0;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDN_HOTKEY || uMsg == AKDN_HOTKEYGLOBAL)
  {
    int *nProcess=(int *)lParam;

    *nProcess=CallHotkey(&hHotkeysStack, (WORD)wParam, (uMsg == AKDN_HOTKEYGLOBAL));
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

void CreateHotkeyStack(HSTACK *hStack, const wchar_t *wpText)
{
  HOTKEYITEM *hiElement=NULL;
  wchar_t wszMethodName[MAX_PATH];
  wchar_t wszHotkeyName[MAX_PATH];
  const wchar_t *wpCount=wpText;
  const wchar_t *wpLineBegin;

  if (wpCount)
  {
    while (*wpCount)
    {
      if (!SkipComment(&wpCount)) break;
      wpLineBegin=wpCount;

      //Name
      GetWord(wpCount, wszHotkeyName, MAX_PATH, &wpCount, NULL);

      //Command
      if (hiElement=ParseCommand(wpCount, hStack, NULL, &wpCount, NULL))
      {
        xstrcpynW(hiElement->wszHotkeyName, wszHotkeyName, MAX_PATH);

        //Hotkey
        MethodGetName(wpCount, wszMethodName, MAX_PATH, &wpCount);

        if (!xstrcmpiW(wszMethodName, L"Hotkey"))
        {
          MethodComment(wpCount, &wpCount);
          hiElement->dwHotkey=(DWORD)xatoiW(wpCount, &wpCount);
          MethodComment(wpCount, &wpCount);
          if (*wpCount == L',')
          {
            MethodComment(wpCount + 1, &wpCount);
            hiElement->bGlobal=(BOOL)xatoiW(wpCount, &wpCount);
            MethodComment(wpCount, &wpCount);
          }

          if (*wpCount != L')')
            ShowError(STRID_PARSEMSG_NOCLOSEPARENTHESIS, wpLineBegin, wpCount);
          else
            ++wpCount;
        }
        else ShowError(STRID_PARSEMSG_UNKNOWNMETHOD, wpLineBegin, wpCount);
      }
      if (!NextLine(&wpCount)) break;
    }
  }
}

HOTKEYITEM* ParseCommand(const wchar_t *wpCount, HSTACK *hStack, HOTKEYITEM *hiElement, const wchar_t **wppCount, DWORD *lpdwParseResult)
{
  STACKEXTPARAM hParamStack={0};
  wchar_t wszMethodName[MAX_PATH];
  const wchar_t *wpLineBegin;
  const wchar_t *wpCommand;
  DWORD dwAction;
  int nPlus;
  DWORD dwResult=PCMD_FAILED;

  while (*wpCount == L' ' || *wpCount == L'\t') ++wpCount;
  wpLineBegin=wpCount;
  wpCommand=wpCount;

  if (*wpCount == L'+')
  {
    ++wpCount;
    nPlus=1;
  }
  else nPlus=0;

  //Command
  if (!MethodGetName(wpCount, wszMethodName, MAX_PATH, &wpCount))
    goto End;

  if (!xstrcmpiW(wszMethodName, L"Command"))
    dwAction=EXTACT_COMMAND;
  else if (!xstrcmpiW(wszMethodName, L"Call"))
    dwAction=EXTACT_CALL;
  else if (!xstrcmpiW(wszMethodName, L"Exec"))
    dwAction=EXTACT_EXEC;
  else if (!xstrcmpiW(wszMethodName, L"OpenFile"))
    dwAction=EXTACT_OPENFILE;
  else if (!xstrcmpiW(wszMethodName, L"SaveFile"))
    dwAction=EXTACT_SAVEFILE;
  else if (!xstrcmpiW(wszMethodName, L"Font"))
    dwAction=EXTACT_FONT;
  else if (!xstrcmpiW(wszMethodName, L"Recode"))
    dwAction=EXTACT_RECODE;
  else if (!xstrcmpiW(wszMethodName, L"Insert"))
    dwAction=EXTACT_INSERT;
  else
    dwAction=0;

  if (dwAction)
  {
    if (!MethodParseParameters(&hParamStack, wpCount, &wpCount))
    {
      ShowError(STRID_PARSEMSG_WRONGPARAMCOUNT, wpLineBegin, wpCount);
      dwResult|=PCMD_ERRORMSG;
    }
    if (*(wpCount - 1) == L')') --wpCount;

    MethodComment(wpCount, &wpCount);
    if (*wpCount != L')')
    {
      ShowError(STRID_PARSEMSG_NOCLOSEPARENTHESIS, wpLineBegin, wpCount);
      dwResult|=PCMD_ERRORMSG;
    }
    else ++wpCount;

    if (!(dwResult & PCMD_ERRORMSG) || !lpdwParseResult)
    {
      if (!hiElement)
        hiElement=StackInsertHotkey(hStack);

      if (hiElement)
      {
        MethodFreeParameters(&hiElement->hParamStack);
        hiElement->hParamStack=hParamStack;
        xmemset(&hParamStack, 0, sizeof(STACKEXTPARAM));

        hiElement->bAutoLoad=nPlus;
        hiElement->dwAction=dwAction;
        CopyWideStr(wpCommand, wpCount - wpCommand, &hiElement->wpHotkeyCommand);
        dwResult|=PCMD_SUCCESS;
      }
    }
    MethodFreeParameters(&hParamStack);
  }
  else
  {
    ShowError(STRID_PARSEMSG_UNKNOWNMETHOD, wpLineBegin, wpCount);
    dwResult|=PCMD_ERRORMSG;
  }

  End:
  if (wppCount) *wppCount=wpCount;
  if (lpdwParseResult) *lpdwParseResult=dwResult;
  return hiElement;
}

void ShowError(int nMessageID, const wchar_t *wpLineBegin, const wchar_t *wpCount)
{
  xprintfW(wszBuffer, GetLangStringW(wLangModule, nMessageID), min(wpCount - wpLineBegin, BUFFER_SIZE - 1), wpLineBegin);
  MessageBoxW(hWndMainDlg?hWndMainDlg:hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONERROR);
}

int CallHotkey(HSTACK *hStack, WORD wHotkey, BOOL bGlobal)
{
  HOTKEYITEM *lpElement;
  EXTPARAM *lpParameter;
  EXPPARAM ep[]={{L"%f", 2, 0, EXPPARAM_FILE},
                 {L"%d", 2, 0, EXPPARAM_FILEDIR},
                 {0, 0, 0, 0}};

  if (lpElement=StackGetHotkeyByHotkey(hStack, wHotkey))
  {
    if (lpElement->bGlobal == bGlobal)
    {
      if (lpElement->dwAction == EXTACT_COMMAND)
      {
        int nCommand=0;
        LPARAM lParam=0;

        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
          nCommand=(int)lpParameter->nNumber;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
          lParam=lpParameter->nNumber;

        if (nCommand > 0)
        {
          SendMessage(hMainWnd, WM_COMMAND, nCommand, lParam);
        }
        else if (nCommand == -1)
        {
          //Skip accelerators
          return -1;
        }
      }
      else if (lpElement->dwAction == EXTACT_CALL)
      {
        PLUGINCALLSENDW pcs;
        wchar_t *wpFunction=lpElement->hParamStack.first->wpString;
        int nStructSize;
        BOOL bCall=FALSE;
        INT_PTR nCallResult;

        if (lpElement->bAutoLoad)
        {
          pcs.pFunction=wpFunction;
          pcs.lParam=0;
          pcs.dwSupport=PDS_GETSUPPORT;

          if (SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs) != UD_FAILED)
          {
            if (pcs.dwSupport & PDS_NOAUTOLOAD)
            {
              xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_AUTOLOAD), wpFunction);
              MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
            }
            else bCall=TRUE;
          }
        }
        else bCall=TRUE;

        if (bCall)
        {
          PLUGINFUNCTION *pf;
          PLUGINCALLPOSTW *pcp;

          SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);

          if ((pf=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wpFunction, 0)) && pf->bRunning)
          {
            //Function is running therefore call plugin through PostMessage.
            //Because called function can unload itself with unsublass (AKD_SETMAINPROC) and AKDN_HOTKEY can go to unloaded memory.
            nStructSize=MethodStructParameters(&lpElement->hParamStack, NULL);
            if (pcp=(PLUGINCALLPOSTW *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW) + nStructSize))
            {
              xstrcpynW(pcp->szFunction, wpFunction, MAX_PATH);
              if (nStructSize > 0)
              {
                pcp->lParam=(LPARAM)((unsigned char *)pcp + sizeof(PLUGINCALLPOSTW));
                MethodStructParameters(&lpElement->hParamStack, (unsigned char *)pcp->lParam);
              }
              else pcp->lParam=0;

              PostMessage(hMainWnd, AKD_DLLCALLW, (lpElement->bAutoLoad?DLLCF_SWITCHAUTOLOAD|DLLCF_SAVEONEXIT:0), (LPARAM)pcp);
            }
          }
          else
          {
            //Function is not running therefore call plugin through SendMessage and check UD_HOTKEY_DODEFAULT flag.
            if (nStructSize=MethodStructParameters(&lpElement->hParamStack, NULL))
            {
              if (pcs.lParam=(LPARAM)GlobalAlloc(GPTR, nStructSize))
                MethodStructParameters(&lpElement->hParamStack, (unsigned char *)pcs.lParam);
            }
            else pcs.lParam=0;

            pcs.pFunction=wpFunction;
            pcs.dwSupport=0;
            nCallResult=SendMessage(hMainWnd, AKD_DLLCALLW, (lpElement->bAutoLoad?DLLCF_SWITCHAUTOLOAD|DLLCF_SAVEONEXIT:0), (LPARAM)&pcs);
            if (nCallResult > 0 && (nCallResult & UD_HOTKEY_DODEFAULT))
              return 0;
          }
        }
      }
      else if (lpElement->dwAction == EXTACT_EXEC)
      {
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        wchar_t *wpCmdLine=NULL;
        wchar_t *wpWorkDir=NULL;
        BOOL bWait=FALSE;
        int nShowWindow=-1;

        SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
          wpCmdLine=lpParameter->wpExpanded;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
          wpWorkDir=lpParameter->wpExpanded;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 3))
          bWait=(BOOL)lpParameter->nNumber;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 4))
          nShowWindow=(int)lpParameter->nNumber;

        if (wpCmdLine)
        {
          xmemset(&si, 0, sizeof(STARTUPINFOW));
          si.cb=sizeof(STARTUPINFOW);
          if (nShowWindow >= 0)
          {
            si.dwFlags=STARTF_USESHOWWINDOW;
            si.wShowWindow=(WORD)nShowWindow;
          }
          if (CreateProcessWide(NULL, wpCmdLine, NULL, NULL, FALSE, 0, NULL, (wpWorkDir && *wpWorkDir)?wpWorkDir:NULL, &si, &pi))
          {
            if (bWait)
            {
              WaitForSingleObject(pi.hProcess, INFINITE);
              //GetExitCodeProcess(pi.hProcess, &dwExit);
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
          }
        }
      }
      else if (lpElement->dwAction == EXTACT_OPENFILE ||
               lpElement->dwAction == EXTACT_SAVEFILE)
      {
        wchar_t *wpFile=NULL;
        int nCodePage=-1;
        BOOL bBOM=-1;

        SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
          wpFile=lpParameter->wpExpanded;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
          nCodePage=(int)lpParameter->nNumber;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 3))
          bBOM=(BOOL)lpParameter->nNumber;

        if (lpElement->dwAction == EXTACT_OPENFILE)
        {
          OPENDOCUMENTW od;

          od.dwFlags=OD_ADT_BINARYERROR;
          if (nCodePage == -1) od.dwFlags|=OD_ADT_DETECTCODEPAGE;
          if (bBOM == -1) od.dwFlags|=OD_ADT_DETECTBOM;

          od.pFile=wpFile;
          od.pWorkDir=NULL;
          od.nCodePage=nCodePage;
          od.bBOM=bBOM;
          od.hDoc=NULL;
          SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);
        }
        else if (lpElement->dwAction == EXTACT_SAVEFILE)
        {
          EDITINFO ei;
          SAVEDOCUMENTW sd;

          if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
          {
            if (nCodePage == -1)
              nCodePage=ei.nCodePage;
            if (bBOM == -1)
              bBOM=ei.bBOM;
            sd.pFile=wpFile;
            sd.nCodePage=nCodePage;
            sd.bBOM=bBOM;
            sd.dwFlags=SD_UPDATE;
            sd.hDoc=NULL;
            SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)NULL, (LPARAM)&sd);
          }
        }
      }
      else if (lpElement->dwAction == EXTACT_FONT)
      {
        EDITINFO ei;
        LOGFONTW lfNew;
        HDC hDC;
        wchar_t *wpFaceName=NULL;
        DWORD dwFontStyle=0;
        int nPointSize=0;

        SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
          wpFaceName=lpParameter->wpExpanded;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
          dwFontStyle=(DWORD)lpParameter->nNumber;
        if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 3))
          nPointSize=(int)lpParameter->nNumber;

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
        {
          if (SendMessage(hMainWnd, AKD_GETFONTW, (WPARAM)ei.hWndEdit, (LPARAM)&lfNew))
          {
            if (nPointSize)
            {
              if (hDC=GetDC(ei.hWndEdit))
              {
                lfNew.lfHeight=-MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
                ReleaseDC(ei.hWndEdit, hDC);
              }
            }
            if (dwFontStyle != FS_NONE)
            {
              lfNew.lfWeight=(dwFontStyle == FS_FONTBOLD || dwFontStyle == FS_FONTBOLDITALIC)?FW_BOLD:FW_NORMAL;
              lfNew.lfItalic=(dwFontStyle == FS_FONTITALIC || dwFontStyle == FS_FONTBOLDITALIC)?TRUE:FALSE;
            }
            if (*wpFaceName != L'\0')
            {
              xstrcpynW(lfNew.lfFaceName, wpFaceName, LF_FACESIZE);
            }
            SendMessage(hMainWnd, AKD_SETFONTW, (WPARAM)ei.hWndEdit, (LPARAM)&lfNew);
          }
        }
      }
      else if (lpElement->dwAction == EXTACT_RECODE)
      {
        EDITINFO ei;
        TEXTRECODE tr={0};

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
        {
          if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
            tr.nCodePageFrom=(int)lpParameter->nNumber;
          if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
            tr.nCodePageTo=(int)lpParameter->nNumber;

          SendMessage(hMainWnd, AKD_RECODESEL, (WPARAM)ei.hWndEdit, (LPARAM)&tr);
        }
      }
      else if (lpElement->dwAction == EXTACT_INSERT)
      {
        EDITINFO ei;
        wchar_t *wpText=NULL;
        wchar_t *wpUnescText=NULL;
        int nTextLen=-1;
        int nUnescTextLen;
        DWORD dwCaret=(DWORD)-1;
        BOOL bEscSequences=FALSE;

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
        {
          if (!ei.bReadOnly)
          {
            SendMessage(hMainWnd, AKD_METHODEXPANDPARAMETERS, (WPARAM)&lpElement->hParamStack, (LPARAM)ep);
            if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 1))
              wpText=lpParameter->wpExpanded;
            if (lpParameter=MethodGetParameter(&lpElement->hParamStack, 2))
              bEscSequences=(BOOL)lpParameter->nNumber;

            if (bEscSequences)
            {
              if (nUnescTextLen=(int)TranslateEscapeString(ei.hWndEdit, wpText, NULL, NULL))
              {
                if (wpUnescText=(wchar_t *)GlobalAlloc(GPTR, nUnescTextLen * sizeof(wchar_t)))
                {
                  nTextLen=(int)TranslateEscapeString(ei.hWndEdit, wpText, wpUnescText, &dwCaret);
                  wpText=wpUnescText;
                }
              }
            }
            if (wpText)
            {
              AEREPLACESELW rs;
              AECHARINDEX ciInsertPos;

              rs.pText=wpText;
              rs.dwTextLen=(UINT_PTR)nTextLen;
              rs.nNewLine=AELB_ASINPUT;
              rs.dwFlags=AEREPT_COLUMNASIS;
              rs.ciInsertStart=&ciInsertPos;
              rs.ciInsertEnd=NULL;
              SendMessage(ei.hWndEdit, AEM_REPLACESELW, 0, (LPARAM)&rs);

              if (dwCaret != (DWORD)-1)
              {
                AEINDEXOFFSET io;

                io.ciCharIn=&ciInsertPos;
                io.ciCharOut=&ciInsertPos;
                io.nOffset=dwCaret;
                io.nNewLine=AELB_ASINPUT;
                SendMessage(ei.hWndEdit, AEM_INDEXOFFSET, 0, (LPARAM)&io);
                SendMessage(ei.hWndEdit, AEM_EXSETSEL, (WPARAM)&ciInsertPos, (LPARAM)&ciInsertPos);
              }
            }
            if (wpUnescText) GlobalFree((HGLOBAL)wpUnescText);
          }
        }
      }
      return 1;
    }
  }
  return 0;
}

HOTKEYITEM* StackInsertHotkey(HSTACK *hStack)
{
  HOTKEYITEM *hiElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&hiElement, -1, sizeof(HOTKEYITEM));
  return hiElement;
}

HOTKEYITEM* StackGetHotkeyByHotkey(HSTACK *hStack, DWORD dwHotkey)
{
  HOTKEYITEM *hiElement;

  for (hiElement=(HOTKEYITEM *)hStack->first; hiElement; hiElement=hiElement->next)
  {
    if (hiElement->dwHotkey == dwHotkey)
      return hiElement;
  }
  return NULL;
}

HOTKEYITEM* StackGetHotkeyByName(HSTACK *hStack, wchar_t *wpName)
{
  HOTKEYITEM *hiElement;

  for (hiElement=(HOTKEYITEM *)hStack->first; hiElement; hiElement=hiElement->next)
  {
    if (!xstrcmpW(hiElement->wszHotkeyName,  wpName))
      return hiElement;
  }
  return NULL;
}

void StackMoveHotkeyByIndex(HSTACK *hStack, int nOldIndex, int nNewIndex)
{
  void *hiElement=NULL;

  if (!StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&hiElement, nOldIndex))
    StackMoveIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack *)hiElement, nNewIndex);
}

void StackDeleteHotkey(HSTACK *hStack, HOTKEYITEM *hiElement)
{
  FreeWideStr(&hiElement->wpHotkeyCommand);
  MethodFreeParameters(&hiElement->hParamStack);
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)hiElement);
}

void StackFreeHotkey(HSTACK *hStack)
{
  HOTKEYITEM *hiElement;

  for (hiElement=(HOTKEYITEM *)hStack->first; hiElement; hiElement=hiElement->next)
  {
    FreeWideStr(&hiElement->wpHotkeyCommand);
    MethodFreeParameters(&hiElement->hParamStack);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote)
{
  const wchar_t *wpCount;
  wchar_t wchStopChar;

  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  if (*wpText == L'\"' || *wpText == L'\'' || *wpText == L'`')
  {
    if (lpbQuote) *lpbQuote=TRUE;
    wchStopChar=*wpText;
    wpCount=++wpText;

    //Parse: "param" or 'param' or `param`
    while (*wpCount != wchStopChar && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
    {
      *wppNextWord=wpCount;
      if (*wpCount == wchStopChar)
        ++*wppNextWord;
    }
  }
  else
  {
    if (lpbQuote) *lpbQuote=FALSE;
    wpCount=wpText;

    //Parse: param1 param2 param3
    while (*wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
      *wppNextWord=wpCount;
  }
  return (int)xstrcpynW(wszWord, wpText, min(nWordMax, wpCount - wpText + 1));
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
    for (whex[4]=L'\0'; *a; ++a)
    {
      if (*a == L'\\')
      {
        if (*++a == L'\\')
        {
          if (wszOutput) *b=L'\\';
          ++b;
        }
        else if (*a == L'n')
        {
          if (ei.nNewLine == NEWLINE_MAC)
          {
            if (wszOutput) *b=L'\r';
            ++b;
          }
          else if (ei.nNewLine == NEWLINE_UNIX)
          {
            if (wszOutput) *b=L'\n';
            ++b;
          }
          else if (ei.nNewLine == NEWLINE_WIN)
          {
            if (wszOutput) *b=L'\r';
            ++b;
            if (wszOutput) *b=L'\n';
            ++b;
          }
        }
        else if (*a == L't')
        {
          if (wszOutput) *b=L'\t';
          ++b;
        }
        else if (*a == L'[')
        {
          while (*++a == L' ');

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
            while (*++a == L' ');

            if (wszOutput) *b=(wchar_t)nDec;
            ++b;
          }
          while (*a && *a != L']');

          if (!*a) goto Error;
        }
        else if (*a == L's')
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
        else if (*a == L'|')
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
      *b=L'\0';
    else
      ++b;
    return (b - wszOutput);
  }

  Error:
  if (wszOutput) *wszOutput=L'\0';
  return 0;
}

wchar_t* AllocControlText(HWND hWnd, int *lpnTextLen)
{
  wchar_t *wszText;
  int nTextLen;

  nTextLen=GetWindowTextLengthWide(hWnd);
  if (wszText=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nTextLen + 1) * sizeof(wchar_t)))
    GetWindowTextWide(hWnd, wszText, nTextLen + 1);
  if (lpnTextLen) *lpnTextLen=nTextLen;
  return wszText;
}

int GetControlText(HWND hWnd, wchar_t *wszText, int nTextMax)
{
  INT_PTR nTextLen;

  nTextLen=GetWindowTextWide(hWnd, wszText, nTextMax);
  if (wszText) nTextLen=RemoveLeadTrailSpaces(wszText, nTextLen);
  return (int)nTextLen;
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

int FindAccelerator(ACCEL *lpMainAccelArray, int nMainAccelCount, int nCmd)
{
  int i;

  for (i=0; i < nMainAccelCount; ++i)
  {
    if (lpMainAccelArray[i].cmd == nCmd)
      return i;
  }
  return -1;
}

int GetCurFile(wchar_t *wszFile, int nMaxFile)
{
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    return (int)xstrcpynW(wszFile, ei.wszFile, nMaxFile) + 1;
  }
  wszFile[0]=L'\0';
  return 0;
}

INT_PTR CopyWideStr(const wchar_t *wpSrc, INT_PTR nSrcLen, wchar_t **wppDst)
{
  wchar_t *wszDst=*wppDst;

  if (nSrcLen == -1)
    nSrcLen=xstrlenW(wpSrc);
  if (wszDst)
    FreeWideStr(&wszDst);
  if (wszDst=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nSrcLen + 1) * sizeof(wchar_t)))
    xstrcpynW(wszDst, wpSrc, nSrcLen + 1);
  *wppDst=wszDst;
  return nSrcLen;
}

BOOL FreeWideStr(wchar_t **wppWideStr)
{
  if (wppWideStr && *wppWideStr && !GlobalFree((HGLOBAL)*wppWideStr))
  {
    *wppWideStr=NULL;
    return TRUE;
  }
  return FALSE;
}

INT_PTR RemoveLeadTrailSpaces(wchar_t *wszText, INT_PTR nTextLen)
{
  INT_PTR i;

  if (nTextLen == -1) nTextLen=xstrlenW(wszText);

  //Remove trailing spaces
  while (nTextLen > 0 && (wszText[nTextLen - 1] == L' ' || wszText[nTextLen - 1] == L'\t'))
  {
    wszText[--nTextLen]=L'\0';
  }

  //Count leading spaces
  for (i=0; i < nTextLen; ++i)
  {
    if (wszText[i] != L' ' && wszText[i] != L'\t')
      break;
  }

  if (i > 0)
    nTextLen=xstrcpynW(wszText, wszText + i, (nTextLen - i) + 1);
  return nTextLen;
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
    //Hotkeys data
    if ((nSize=(int)WideOption(hOptions, L"HotkeyText", PO_BINARY, NULL, 0)) > 0)
    {
      if (wszHotkeyText=(wchar_t *)HeapAlloc(hHeap, 0, nSize + sizeof(wchar_t)))
      {
        WideOption(hOptions, L"HotkeyText", PO_BINARY, (LPBYTE)wszHotkeyText, nSize);
        wszHotkeyText[nSize / sizeof(wchar_t)]=L'\0';
      }
    }

    //Dialog rectangle
    WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
    WideOption(hOptions, L"ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
    WideOption(hOptions, L"ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
    WideOption(hOptions, L"ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));

    //All keys dialog
    WideOption(hOptions, L"AllKeysRect", PO_BINARY, (LPBYTE)&rcAllKeysCurrentDialog, sizeof(RECT));
    WideOption(hOptions, L"AllKeysColumnWidth1", PO_DWORD, (LPBYTE)&nAllKeysColumnWidth1, sizeof(DWORD));
    WideOption(hOptions, L"AllKeysColumnWidth2", PO_DWORD, (LPBYTE)&nAllKeysColumnWidth2, sizeof(DWORD));
    WideOption(hOptions, L"AllKeysColumnWidth3", PO_DWORD, (LPBYTE)&nAllKeysColumnWidth3, sizeof(DWORD));
    WideOption(hOptions, L"AllKeysOnlyAssigned", PO_DWORD, (LPBYTE)&bAllKeysOnlyAssigned, sizeof(DWORD));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }

  if (!wszHotkeyText)
  {
    if (wszHotkeyText=(wchar_t *)GlobalAlloc(GPTR, sizeof(TXT_DEFAULT_HOTKEYS)))
    {
      xmemcpy((unsigned char *)wszHotkeyText, TXT_DEFAULT_HOTKEYS, sizeof(TXT_DEFAULT_HOTKEYS));
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
      if (wszHotkeyText)
        WideOption(hOptions, L"HotkeyText", PO_BINARY, (LPBYTE)wszHotkeyText, (lstrlenW(wszHotkeyText) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_MAINRECT)
    {
      WideOption(hOptions, L"WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
      WideOption(hOptions, L"ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));
    }
    if (dwFlags & OF_ALLKEYSRECT)
    {
      WideOption(hOptions, L"AllKeysRect", PO_BINARY, (LPBYTE)&rcAllKeysCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"AllKeysColumnWidth1", PO_DWORD, (LPBYTE)&nAllKeysColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"AllKeysColumnWidth2", PO_DWORD, (LPBYTE)&nAllKeysColumnWidth2, sizeof(DWORD));
      WideOption(hOptions, L"AllKeysColumnWidth3", PO_DWORD, (LPBYTE)&nAllKeysColumnWidth3, sizeof(DWORD));
    }
    if (dwFlags & OF_ALLKEYSOPTIONS)
    {
      WideOption(hOptions, L"AllKeysOnlyAssigned", PO_DWORD, (LPBYTE)&bAllKeysOnlyAssigned, sizeof(DWORD));
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
    if (nStringID == STRID_PARSEMSG_UNKNOWNMETHOD)
      return L"\x041D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x044B\x0439\x0020\x043C\x0435\x0442\x043E\x0434.\n\n%.%ds";
    if (nStringID == STRID_PARSEMSG_WRONGPARAMCOUNT)
      return L"\x041D\x0435\x0432\x0435\x0440\x043D\x043E\x0435\x0020\x043A\x043E\x043B\x0438\x0447\x0435\x0441\x0442\x0432\x043E\x0020\x043F\x0430\x0440\x0430\x043C\x0435\x0442\x0440\x043E\x0432\x002E";
    if (nStringID == STRID_PARSEMSG_NOCLOSEPARENTHESIS)
      return L"\x041D\x0435\x0442\x0020\x0437\x0430\x043A\x0440\x044B\x0432\x0430\x044E\x0449\x0435\x0439\x0020\x0441\x043A\x043E\x0431\x043A\x0438 \")\".\n\n%.%ds";
    if (nStringID == STRID_NAME)
      return L"\x0418\x043C\x044F";
    if (nStringID == STRID_NAME_AMP)
      return L"&\x0418\x043C\x044F";
    if (nStringID == STRID_COMMAND)
      return L"\x041A\x043E\x043C\x0430\x043D\x0434\x0430";
    if (nStringID == STRID_COMMAND_AMP)
      return L"&\x041A\x043E\x043C\x0430\x043D\x0434\x0430";
    if (nStringID == STRID_HOTKEY)
      return L"\x0413\x043E\x0440\x044F\x0447\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_KEYAMP)
      return L"\x041A\x043B&\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_KEYCODE)
      return L"\x041A\x043E\x0434\x0020\x0433\x043E\x0440\x044F\x0447\x0435\x0439\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0438";
    if (nStringID == STRID_CODE)
      return L"\x041A\x043E\x0434:";
    if (nStringID == STRID_GLOBALKEY)
      return L"\x0413\x043B\x043E\x0431\x0430\x043B\x044C\x043D\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_ADD)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_MODIFY)
      return L"\x0418\x0437\x043C\x0435\x043D\x0438\x0442\x044C";
    if (nStringID == STRID_MOVEUP)
      return L"\x0412\x0432\x0435\x0440\x0445";
    if (nStringID == STRID_MOVEDOWN)
      return L"\x0412\x043D\x0438\x0437";
    if (nStringID == STRID_DELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_ALLKEYS)
      return L"\x0412\x0441\x0435\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_ALLKEYS_AMP)
      return L"&\x0412\x0441\x0435\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_MENU_ITEMRENAME)
      return L"\x041F\x0435\x0440\x0435\x0438\x043C\x0435\x043D\x043E\x0432\x0430\x0442\x044C\tF2";
    if (nStringID == STRID_MENU_ITEMMOVEUP)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x0432\x0435\x0440\x0445\tAlt+Up";
    if (nStringID == STRID_MENU_ITEMMOVEDOWN)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x043D\x0438\x0437\tAlt+Down";
    if (nStringID == STRID_MENU_ITEMDELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\tDelete";
    if (nStringID == STRID_ONLYASSIGNED)
      return L"\x041F\x043E\x043A\x0430\x0437\x044B\x0432\x0430\x0442\x044C\x0020\x0442\x043E\x043B\x044C\x043A\x043E\x0020\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x043D\x044B\x0435\x0020\x0049\x0044\x004D\x005F\x0020\x043A\x043E\x043C\x0430\x043D\x0434\x044B";
    if (nStringID == STRID_AUTOLOAD)
      return L"\x0022\x0025\x0073\x0022\x0020\x043D\x0435\x0020\x043F\x043E\x0434\x0434\x0435\x0440\x0436\x0438\x0432\x0430\x0435\x0442\x0020\x0430\x0432\x0442\x043E\x0437\x0430\x0433\x0440\x0443\x0437\x043A\x0443\x0020\x0028\x0022\x002B\x0022\x0020\x043F\x0435\x0440\x0435\x0434\x0020\x0022\x0043\x0061\x006C\x006C\x0022\x0029\x002E";
    if (nStringID == STRID_NAME_EXISTS)
      return L"\x0418\x043C\x044F\x0020\x0443\x0436\x0435\x0020\x0441\x0443\x0449\x0435\x0441\x0442\x0432\x0443\x0435\x0442\x002E";
    if (nStringID == STRID_HOTKEY_EXISTS)
      return L"\x0413\x043E\x0440\x044F\x0447\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430\x0020\x0443\x0436\x0435\x0020\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x0430\x0020\x0434\x043B\x044F\x0020\x0022\x0025\x0073\x0022\x002E";
    if (nStringID == STRID_LOADFIRST)
      return L"\x0417\x0430\x0433\x0440\x0443\x0437\x0438\x0442\x0435\x0020\x0441\x043F\x0435\x0440\x0432\x0430 %s.";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
    if (nStringID == STRID_CLOSE)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C";
  }
  else
  {
    if (nStringID == STRID_PARSEMSG_UNKNOWNMETHOD)
      return L"Unknown method.\n\n%.%ds";
    if (nStringID == STRID_PARSEMSG_WRONGPARAMCOUNT)
      return L"Wrong number of parameters.";
    if (nStringID == STRID_PARSEMSG_NOCLOSEPARENTHESIS)
      return L"No close parenthesis \")\".\n\n%.%ds";
    if (nStringID == STRID_NAME)
      return L"Name";
    if (nStringID == STRID_NAME_AMP)
      return L"&Name";
    if (nStringID == STRID_COMMAND)
      return L"Command";
    if (nStringID == STRID_COMMAND_AMP)
      return L"&Command";
    if (nStringID == STRID_HOTKEY)
      return L"Hotkey";
    if (nStringID == STRID_KEYAMP)
      return L"&Hotkey";
    if (nStringID == STRID_KEYCODE)
      return L"Hotkey code";
    if (nStringID == STRID_CODE)
      return L"Code:";
    if (nStringID == STRID_GLOBALKEY)
      return L"Global key";
    if (nStringID == STRID_ADD)
      return L"Add";
    if (nStringID == STRID_MODIFY)
      return L"Modify";
    if (nStringID == STRID_MOVEUP)
      return L"Up";
    if (nStringID == STRID_MOVEDOWN)
      return L"Down";
    if (nStringID == STRID_DELETE)
      return L"Delete";
    if (nStringID == STRID_ALLKEYS)
      return L"All keys...";
    if (nStringID == STRID_ALLKEYS_AMP)
      return L"&All keys...";
    if (nStringID == STRID_MENU_ITEMRENAME)
      return L"Rename\tF2";
    if (nStringID == STRID_MENU_ITEMMOVEUP)
      return L"Move up\tAlt+Up";
    if (nStringID == STRID_MENU_ITEMMOVEDOWN)
      return L"Move down\tAlt+Down";
    if (nStringID == STRID_MENU_ITEMDELETE)
      return L"Delete\tDelete";
    if (nStringID == STRID_ONLYASSIGNED)
      return L"Show only assigned IDM_ commands";
    if (nStringID == STRID_AUTOLOAD)
      return L"\"%s\" doesn't support autoload (\"+\" before \"Call\").";
    if (nStringID == STRID_NAME_EXISTS)
      return L"\"%s\" name already exits.";
    if (nStringID == STRID_HOTKEY_EXISTS)
      return L"Hotkey already assigned to \"%s\".";
    if (nStringID == STRID_LOADFIRST)
      return L"Load %s first.";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
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
  hInstanceEXE=pd->hInstanceEXE;
  hPluginsStack=pd->hPluginsStack;
  hMainWnd=pd->hMainWnd;
  hMdiClient=pd->hMdiClient;
  hMainMenu=pd->hMainMenu;
  hMenuRecentFiles=pd->hMenuRecentFiles;
  hGlobalAccel=pd->hGlobalAccel;
  hMainAccel=pd->hMainAccel;
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
  wszMainFilter[0]=L'\0';
  wszAllKeysFilter[0]=L'\0';
  CreateHotkeyStack(&hHotkeysStack, wszHotkeyText);
  HeapFree(hHeap, 0, wszHotkeyText);
  wszHotkeyText=NULL;
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

  StackFreeHotkey(&hHotkeysStack);

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
