/*****************************************************************
 *                 AkelUpdater NSIS plugin v5.0                  *
 *                                                               *
 * 2014 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *****************************************************************/

#define _WIN32_IE 0x0400
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include "WideFunc.h"
#include "StackFunc.h"
#include "StrFunc.h"
#include "x64Func.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


//Include wide functions
#define ComboBox_AddStringWide
#define ComboBox_GetLBTextWide
#define CreateFileWide
#define CreateProcessWide
#define DialogBoxWide
#define DispatchMessageWide
#define FindFirstFileWide
#define FindNextFileWide
#define GetModuleFileNameWide
#define GetWindowLongPtrWide
#define ListView_GetItemWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetColumnWide
#define ListView_SetItemWide
#define LoadLibraryWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#include "WideFunc.h"

//Include stack functions
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackMoveBefore
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemcmp
#define xmemset
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xstrcpyW
#define xstrcpynA
#define xstrcpynW
#define xstrcmpW
#define xstrcmpnW
#define xstrcmpinW
#define xstrlenA
#define xstrlenW
#define xstrcmpiW
#define xprintfW
#define xstrstrW
#define UTF8toUTF16
#define UTF16toUTF8
#define UTF32toUTF16
#include "StrFunc.h"

//Include x64 functions
#define ReadFile64
#define GetFileSize64
#define SetFilePointer64
#define MultiByteToWideChar64
#define WideCharToMultiByte64
#include "x64Func.h"

//Defines
#define NSIS_MAX_STRLEN 1024

//String IDs
#define STRID_PROGRAM          0
#define STRID_PLUGIN           1
#define STRID_SCRIPT           2
#define STRID_SCRIPTS          3
#define STRID_COPIES           4
#define STRID_LATEST           5
#define STRID_CURRENT          6
#define STRID_DESCRIPTION      7
#define STRID_AUTHOR           8
#define STRID_MIRROR           9
#define STRID_LANGUAGE         10
#define STRID_SELECT           11
#define STRID_ERRORNOTINLIST   12
#define STRID_ERRORNOTPLUGIN   13
#define STRID_ERRORCANTLOAD    14
#define STRID_ERRORCOUNT       15
#define STRID_UPDATE           16
#define STRID_CANCEL           17

#define AKDLL_SHOWWINDOW    (WM_USER + 100)
#define AKDLL_UPDATESTATUS  (WM_USER + 101)

//Plugins list
#define LVSI_NAME         0
#define LVSI_LATEST       1
#define LVSI_CURRENT      2
#define LVSI_DESCRIPTION  3
#define LVSI_AUTHOR       4

#define LT_MAIN    1
#define LT_SCRIPTS 2

#define CR_EQUAL           0
#define CR_INSTALLEDOLDER  1
#define CR_INSTALLEDNEWER  2
#define CR_NOTINSTALLED    3

#ifndef SCS_32BIT_BINARY
  #define SCS_32BIT_BINARY 0
#endif
#ifndef SCS_64BIT_BINARY
  #define SCS_64BIT_BINARY 6
#endif

#define PE_NONE        0
#define PE_NOTPLUGIN   1
#define PE_CANTLOAD    2
#define PE_NOTINLIST   3

typedef struct {
  wchar_t wszName[MAX_PATH];
  DWORD dwError;
} DLLINFO;

//ExDll
typedef struct _stack_t {
  struct _stack_t *next;
  wchar_t text[1];
} stack_t;

typedef struct
{
  int autoclose;
  int all_user_var;
  int exec_error;
  int abort;
  int exec_reboot;
  int reboot_called;
  int XXX_cur_insttype;
  int plugin_api_version;
  int silent;
  int instdir_error;
  int rtl;
  int errlvl;
  int alter_reg_view;
  int status_update;
} exec_flags_t;

typedef struct {
  exec_flags_t *exec_flags;
  int (__stdcall *ExecuteCodeSegment)(int, HWND);
  void (__stdcall *validate_filename)(wchar_t *);
} extra_parameters;

stack_t **g_stacktop;
wchar_t *g_variables;
unsigned int g_stringsize;
extra_parameters *g_pluginParms;
BOOL g_unicode=-1;

//NSIS variables
enum
{
INST_0,         // $0
INST_1,         // $1
INST_2,         // $2
INST_3,         // $3
INST_4,         // $4
INST_5,         // $5
INST_6,         // $6
INST_7,         // $7
INST_8,         // $8
INST_9,         // $9
INST_R0,        // $R0
INST_R1,        // $R1
INST_R2,        // $R2
INST_R3,        // $R3
INST_R4,        // $R4
INST_R5,        // $R5
INST_R6,        // $R6
INST_R7,        // $R7
INST_R8,        // $R8
INST_R9,        // $R9
INST_CMDLINE,   // $CMDLINE
INST_INSTDIR,   // $INSTDIR
INST_OUTDIR,    // $OUTDIR
INST_EXEDIR,    // $EXEDIR
INST_LANG,      // $LANGUAGE
__INST_LAST
};

//FILEITEM.nType
#define FIT_AKELPAD   1
#define FIT_PLUGIN    2
#define FIT_SCRIPT    3
#define FIT_FORSCRIPT 4

//Scripts list columns
#define LVI_SCRIPT        0
#define LVI_HOTKEY        1
#define LVI_STATUS        2
#define LVI_VERSION       3
#define LVI_DESCRIPTION   4
#define LVI_AUTHOR        5
#define LVI_SITE          6

//Scripts list columns flags
#define LCF_VISIBLE    0x1
#define LCF_CONTENT    0x2

//Scripts list columns name
#define LCN_SCRIPT      0x01
#define LCN_HOTKEY      0x02
#define LCN_STATUS      0x04
#define LCN_VERSION     0x08
#define LCN_DESCRIPTION 0x10
#define LCN_AUTHOR      0x20
#define LCN_SITE        0x40

typedef struct {
  int nID;
  int nWidth;
  DWORD dwFlags;
} LISTCOLUMN;

typedef struct _LISTITEM {
  struct _LISTITEM *next;
  struct _LISTITEM *prev;
  wchar_t *wpScript;
  DWORD dwHotkey;
  wchar_t *wpHotkey;
  wchar_t *wpStatus;
  wchar_t *wpVersion;
  wchar_t *wpDescription;
  wchar_t *wpAuthor;
  wchar_t *wpSite;
} LISTITEM;

typedef struct {
  LISTITEM *first;
  LISTITEM *last;
  LISTITEM *lpLastScript;
  int nElements;
} STACKLISTITEM;

typedef struct _FILEITEM {
  struct _FILEITEM *next;
  struct _FILEITEM *prev;
  int nType;                     //See FIT_* defines.
  int nCompare;
  int nChecked;
  DWORD dwError;
  wchar_t wszError[MAX_PATH];
  wchar_t wszPack[MAX_PATH];
  wchar_t wszName[MAX_PATH];
  wchar_t wszCurVer[MAX_PATH];
  wchar_t wszLastVer[MAX_PATH];
  wchar_t wszDescription[MAX_PATH];
  wchar_t wszAuthor[MAX_PATH];
  struct _FILEITEM *mainScript;
  struct _FILEITEM *firstCopy;
  struct _FILEITEM *lastCopy;
} FILEITEM;

typedef struct {
  FILEITEM *first;
  FILEITEM *last;
} STACKFILEITEM;

//Functions prototypes
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ParseLst(HWND hDlg);
void CompareItems();
void FillItems(HWND hDlg, HWND hWndListExe, HWND hWndListDll);
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg);
int GetNextWord(wchar_t *wpStr, INT_PTR nStrLen, wchar_t *wpDelim, int nDelimLen, wchar_t *wszWord, int nWordMax, const wchar_t **wpNextWord);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcInit, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void StackFilesFill(STACKFILEITEM *hStack);
FILEITEM* StackFileInsert(STACKFILEITEM *hStack, const wchar_t *wpPluginName);
FILEITEM* StackFileGet(STACKFILEITEM *hStack, const wchar_t *wpPluginName);
int StackFileSelCount(STACKFILEITEM *hStack);
void StackFilesSort(STACKFILEITEM *hStack);
int StackSort(FILEITEM **first, FILEITEM **last, int nUpDown);
void StackFilesFree(STACKFILEITEM *hStack);
LISTITEM* StackScriptGet(STACKLISTITEM *hStack, const wchar_t *wpScriptFileName);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
int GetBaseName(const wchar_t *wpFile, int nFileLen, wchar_t *wszBaseName, int nBaseNameMax);
const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen);
int VersionCompare(const wchar_t *wpVer1, const wchar_t *wpVer2);
BOOL GetFileVersionA(const char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
BOOL GetFileVersionW(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
BOOL GetFileVersionWide(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
int AutodetectCodePage(const wchar_t *wpFile, HANDLE hFile, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
UINT_PTR ReadFileContent(HANDLE hFile, UINT_PTR dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wppContent);
void ChangeTwoBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst);
void ChangeFourBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst);
BOOL IsCodePageUnicode(int nCodePage);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);

int getuservariable(const int varnum, wchar_t *str, int len);
void setuservariable(const int varnum, const wchar_t *var);
INT_PTR popintegerWide();
void pushintegerWide(INT_PTR integer);
int popstringAnsi(char *str, int len);
int popstringWide(wchar_t *str, int len);
void pushstringAnsi(const char *str);
void pushstringWide(const wchar_t *str);
void Initialize(int string_size, wchar_t *variables, stack_t **stacktop, extra_parameters *extra);

//Global variables
char szBuf[NSIS_MAX_STRLEN];
wchar_t wszBuf[NSIS_MAX_STRLEN];
wchar_t wszBuf2[NSIS_MAX_STRLEN];
wchar_t wszExeFile[MAX_PATH];
int nExeFileLen;
wchar_t wszExeDir[MAX_PATH];
int nExeDirLen;
wchar_t wszAkelDir[MAX_PATH];
wchar_t wszPlugsDir[MAX_PATH];
wchar_t wszInputLanguage[MAX_PATH];
wchar_t wszInputVersion[32];
wchar_t wszNsisTempDir[MAX_PATH];
wchar_t wszInputHelper[MAX_PATH];
wchar_t wszScriptsPack[MAX_PATH]=L"";
HINSTANCE hInstanceDLL=NULL;
HINSTANCE hInstanceEXE=NULL;
BOOL bOldWindows;
WORD wLangSystem;
STACKFILEITEM hFileStack={0};
STACKLISTITEM hScriptsStack={0};
FILEITEM *lpFileItemAkelPad=NULL;
FILEITEM *lpFileItemScripts=NULL;
DLLINFO diGlobal;
HWND hWndDialog=NULL;
int nInputBit=32;
BOOL bScripts=FALSE;
BOOL bInputAuto=FALSE;
BOOL bInputNoCopies=FALSE;
BOOL bSelectAllExe;
BOOL bSelectAllDll;
BOOL bSelectAllJs;
INT_PTR lpDownloadScriptsProc;
RECT rcMainMinMaxDialog={437, 309, 0, 0};
RECT rcMainCurrentDialog={0};

//GetProcAddress
BOOL (WINAPI *GetBinaryTypePtr)(const wchar_t *lpApplicationName, LPDWORD lpBinaryType);
void (*DllAkelUpdaterFill)(HWND hDlg, HWND hWndList, STACKLISTITEM *hScriptsStack, LISTCOLUMN *lpColumns)=NULL;
void (*DllAkelUpdaterFree)(STACKLISTITEM *hScriptsStack)=NULL;

//Plugin extern function
#ifdef __GNUC__
  extern "C"
#endif
void __declspec(dllexport) Init(HWND hwndParent, int string_size, wchar_t *variables, stack_t **stacktop, extra_parameters *extra)
{
  Initialize(string_size, variables, stacktop, extra);
  {
    HMODULE hKernel32;
    DWORD dwBinaryType=SCS_32BIT_BINARY;

    wLangSystem=PRIMARYLANGID(GetUserDefaultLangID());
    hInstanceEXE=GetModuleHandleA(NULL);
    nExeFileLen=GetModuleFileNameWide(hInstanceEXE, wszExeFile, MAX_PATH);
    nExeDirLen=GetFileDir(wszExeFile, nExeFileLen, wszExeDir, MAX_PATH);
    GetFileDir(wszExeDir, nExeDirLen, wszAkelDir, MAX_PATH);
    xprintfW(wszPlugsDir, L"%s\\Plugs", wszExeDir);

    hKernel32=GetModuleHandleA("kernel32.dll");
    if (GetBinaryTypePtr=(BOOL (WINAPI *)(const wchar_t *, LPDWORD))GetProcAddress(hKernel32, "GetBinaryTypeW"))
    {
      xprintfW(wszBuf, L"%s\\..\\AkelPad.exe", wszExeDir);
      if (!GetBinaryTypePtr(wszBuf, &dwBinaryType))
      {
        xprintfW(wszBuf, L"%s\\..\\notepad.exe", wszExeDir);
        GetBinaryTypePtr(wszBuf, &dwBinaryType);
      }
    }
    pushintegerWide(dwBinaryType == SCS_64BIT_BINARY?64:32);
  }
}

#ifdef __GNUC__
  extern "C"
#endif
void __declspec(dllexport) List(HWND hwndParent, int string_size, wchar_t *variables, stack_t **stacktop, extra_parameters *extra)
{
  Initialize(string_size, variables, stacktop, extra);
  {
    INT_PTR nResult;

    popstringWide(wszInputVersion, MAX_PATH);
    popstringWide(wszInputLanguage, MAX_PATH);
    nInputBit=popintegerWide();
    bInputAuto=popintegerWide();
    bInputNoCopies=popintegerWide();
    popstringWide(wszNsisTempDir, MAX_PATH);
    lpDownloadScriptsProc=popintegerWide();
    popstringWide(wszInputHelper, MAX_PATH);

    nResult=DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hwndParent, (DLGPROC)SetupDlgProc);
    pushintegerWide(nResult);
  }
}

#ifdef __GNUC__
  extern "C"
#endif
void __declspec(dllexport) Collapse(HWND hwndParent, int string_size, wchar_t *variables, stack_t **stacktop, extra_parameters *extra)
{
  Initialize(string_size, variables, stacktop, extra);
  {
    HWND hWnd;

    hWnd=(HWND)popintegerWide();
    MoveWindow(hWnd, 0, 0, 0, 0, TRUE);
  }
}

#ifdef __GNUC__
  extern "C"
#endif
void __declspec(dllexport) ParseAndPush(HWND hwndParent, int string_size, wchar_t *variables, stack_t **stacktop, extra_parameters *extra)
{
  Initialize(string_size, variables, stacktop, extra);
  {
    wchar_t *wpArgument;

    popstringWide(wszBuf, NSIS_MAX_STRLEN);
    wpArgument=wszBuf;

    while (GetCommandLineArg(wpArgument, wszBuf2, NSIS_MAX_STRLEN, &wpArgument))
    {
      pushstringWide(wszBuf2);
    }
  }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    hInstanceDLL=hinstDLL;
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

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndGroupExe;
  static HWND hWndListExe;
  static HWND hWndLanguage;
  static HWND hWndMirrorLabel;
  static HWND hWndMirror;
  static HWND hWndListDll;
  static HWND hWndListCheckInfo;
  static HWND hWndListStatusInfo;
  static HWND hWndUpdate;
  static HWND hWndScripts;
  static HWND hWndCancel;
  static LVITEMW lvi;
  static HICON hIconDlg;
  static BOOL bLockUpdateStatus;
  static DIALOGRESIZE drs[]={{&hWndGroupExe,       DRS_SIZE|DRS_X, 0},
                             {&hWndListExe,        DRS_SIZE|DRS_X, 0},
                             {&hWndMirrorLabel,    DRS_MOVE|DRS_X, 0},
                             {&hWndMirror,         DRS_MOVE|DRS_X, 0},
                             {&hWndListDll,        DRS_SIZE|DRS_X, 0},
                             {&hWndListDll,        DRS_SIZE|DRS_Y, 0},
                             {&hWndListCheckInfo,  DRS_MOVE|DRS_Y, 0},
                             {&hWndListStatusInfo, DRS_MOVE|DRS_X, 0},
                             {&hWndListStatusInfo, DRS_MOVE|DRS_Y, 0},
                             {&hWndUpdate,         DRS_MOVE|DRS_Y, 0},
                             {&hWndScripts,        DRS_MOVE|DRS_Y, 0},
                             {&hWndCancel,         DRS_MOVE|DRS_X, 0},
                             {&hWndCancel,         DRS_MOVE|DRS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvc;

    hIconDlg=LoadIconA(hInstanceEXE, MAKEINTRESOURCEA(IDI_ICON));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIconDlg);
    hWndDialog=hDlg;
    hWndGroupExe=GetDlgItem(hDlg, IDC_GROUP_EXE);
    hWndListExe=GetDlgItem(hDlg, IDC_LIST_EXE);
    hWndLanguage=GetDlgItem(hDlg, IDC_LANGUAGE);
    hWndMirrorLabel=GetDlgItem(hDlg, IDC_MIRROR_LABEL);
    hWndMirror=GetDlgItem(hDlg, IDC_MIRROR);
    hWndListDll=GetDlgItem(hDlg, IDC_LIST_DLL);
    hWndListCheckInfo=GetDlgItem(hDlg, IDC_LIST_CHECKINFO);
    hWndListStatusInfo=GetDlgItem(hDlg, IDC_LIST_STATUSINFO);
    hWndUpdate=GetDlgItem(hDlg, IDOK);
    hWndScripts=GetDlgItem(hDlg, IDC_SCRIPTS);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
    SendMessage(hWndListExe, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES);
    SendMessage(hWndListDll, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES);

    xprintfW(wszBuf, L"AkelUpdater %s", wszInputVersion);
    SetWindowTextWide(hDlg, wszBuf);

    SetDlgItemTextWide(hDlg, IDC_MIRROR_LABEL, GetLangStringW(wLangSystem, STRID_MIRROR));
    SetDlgItemTextWide(hDlg, IDC_LANGUAGE_LABEL, GetLangStringW(wLangSystem, STRID_LANGUAGE));
    SetDlgItemTextWide(hDlg, IDC_LIST_CHECKINFO, GetLangStringW(wLangSystem, STRID_SELECT));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangSystem, STRID_UPDATE));
    SetDlgItemTextWide(hDlg, IDC_SCRIPTS, GetLangStringW(wLangSystem, STRID_SCRIPTS));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangSystem, STRID_CANCEL));

    ComboBox_AddStringWide(hWndMirror, L"osdn");
    ComboBox_AddStringWide(hWndMirror, L"heanet");
    ComboBox_AddStringWide(hWndMirror, L"switch");
    ComboBox_AddStringWide(hWndMirror, L"nchc");
    SendMessage(hWndMirror, CB_SETCURSEL, (WPARAM)0, 0);

    ComboBox_AddStringWide(hWndLanguage, L"eng");
    ComboBox_AddStringWide(hWndLanguage, L"rus");
    if (!xstrcmpW(wszInputLanguage, L"rus"))
      SendMessage(hWndLanguage, CB_SETCURSEL, (WPARAM)1, 0);
    else
      SendMessage(hWndLanguage, CB_SETCURSEL, (WPARAM)0, 0);

    if (bScripts)
      SendMessage(hWndScripts, BM_SETCHECK, BST_CHECKED, 0);

    EnableWindow(hWndMirror, FALSE);
    EnableWindow(hWndUpdate, FALSE);

    //Columns EXE
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangSystem, STRID_PROGRAM);
    lvc.cx=188;
    lvc.iSubItem=LVSI_NAME;
    ListView_InsertColumnWide(hWndListExe, LVSI_NAME, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangSystem, STRID_LATEST);
    lvc.cx=105;
    lvc.iSubItem=LVSI_LATEST;
    ListView_InsertColumnWide(hWndListExe, LVSI_LATEST, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangSystem, STRID_CURRENT);
    lvc.cx=63;
    lvc.iSubItem=LVSI_CURRENT;
    ListView_InsertColumnWide(hWndListExe, LVSI_CURRENT, &lvc);

    //Columns DLL
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)L"";
    lvc.cx=210;
    lvc.iSubItem=LVSI_NAME;
    ListView_InsertColumnWide(hWndListDll, LVSI_NAME, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangSystem, STRID_LATEST);
    lvc.cx=83;
    lvc.iSubItem=LVSI_LATEST;
    ListView_InsertColumnWide(hWndListDll, LVSI_LATEST, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangSystem, STRID_CURRENT);
    lvc.cx=83;
    lvc.iSubItem=LVSI_CURRENT;
    ListView_InsertColumnWide(hWndListDll, LVSI_CURRENT, &lvc);

    //Parse "versions.lst"
    ParseLst(hDlg);
    if (lpFileItemScripts && lpFileItemScripts->wszCurVer[0])
      ShowWindow(hWndScripts, SW_SHOW);

    //Compare versions
    CompareItems();

    //Items
    FillItems(hDlg, hWndListExe, hWndListDll);

    if (bInputAuto)
    {
      if (StackFileSelCount(&hFileStack))
        PostMessage(hDlg, WM_COMMAND, IDOK, 0);
      else
        PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    }
    else
    {
      PostMessage(hDlg, AKDLL_SHOWWINDOW, 0, 0);
      PostMessage(hDlg, AKDLL_UPDATESTATUS, 0, 0);
    }
  }
  if (uMsg == AKD_DETECTFILE ||
      uMsg == AKD_DETECTFILEA ||
      uMsg == AKD_DETECTFILEW)
  {
    DETECTFILEW *dc=(DETECTFILEW *)lParam;
    wchar_t *wpFile;
    int nResult;

    if (uMsg == AKD_DETECTFILEA || (bOldWindows && uMsg == AKD_DETECTFILE))
      wpFile=AllocWide((char *)dc->pFile);
    else
      wpFile=(wchar_t *)dc->pFile;

    nResult=AutodetectCodePage(wpFile, NULL, dc->dwFlags, &dc->nCodePage, &dc->bBOM);

    if (uMsg == AKD_DETECTFILEA || (bOldWindows && uMsg == AKD_DETECTFILE))
      FreeWide(wpFile);
    SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, nResult);
    return TRUE;
  }
  if (uMsg == AKD_READFILECONTENT)
  {
    FILECONTENT *fc=(FILECONTENT *)lParam;
    UINT_PTR dwResult;

    dwResult=ReadFileContent(fc->hFile, fc->dwMax, fc->nCodePage, fc->bBOM, &fc->wpContent);
    SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, dwResult);
    return TRUE;
  }
  if (uMsg == AKDLL_SHOWWINDOW)
  {
    ShowWindow(hDlg, SW_SHOW);
  }
  else if (uMsg == AKDLL_UPDATESTATUS)
  {
    if (!bLockUpdateStatus)
    {
      FILEITEM *lpFileItem;
      int nAllItemsCount=0;
      int nSelCount=0;
      int nErrorsCount=0;

      for (lpFileItem=hFileStack.first; lpFileItem; lpFileItem=lpFileItem->next)
      {
        if (lpFileItem->mainScript) continue;

        if (lpFileItem->nCompare == CR_INSTALLEDNEWER ||
            lpFileItem->dwError == PE_NOTPLUGIN ||
            lpFileItem->dwError == PE_CANTLOAD)
        {
          ++nErrorsCount;
        }
        if (lpFileItem->nChecked > 0)
          ++nSelCount;
        ++nAllItemsCount;
      }
      if (nErrorsCount)
        xprintfW(wszBuf2, L"(%s: %d)   ", GetLangStringW(wLangSystem, STRID_ERRORCOUNT), nErrorsCount);
      else
        wszBuf2[0]=L'\0';
      xprintfW(wszBuf, L"%s%d / %d", wszBuf2, nSelCount, nAllItemsCount);
      SetWindowTextWide(hWndListStatusInfo, wszBuf);
      EnableWindow(hWndUpdate, nSelCount);
    }
  }
  else if (uMsg == WM_COPYDATA)
  {
    COPYDATASTRUCT *cds=(COPYDATASTRUCT *)lParam;
    DLLINFO *di=cds->lpData;

    xstrcpynW(diGlobal.wszName, di->wszName, MAX_PATH);
    diGlobal.dwError=di->dwError;
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_LIST_EXE ||
        wParam == IDC_LIST_DLL)
    {
      NMLISTVIEW *pnmlv=(NMLISTVIEW *)lParam;

      if (pnmlv->hdr.code == LVN_COLUMNCLICK)
      {
        if (pnmlv->iSubItem == LVSI_NAME)
        {
          int nIndex;
          int nMaxIndex=(int)SendMessage(pnmlv->hdr.hwndFrom, LVM_GETITEMCOUNT, 0, 0);
          BOOL *lpbSelectAll;

          if (wParam == IDC_LIST_EXE)
            lpbSelectAll=&bSelectAllExe;
          else
          {
            if (bScripts)
              lpbSelectAll=&bSelectAllJs;
            else
              lpbSelectAll=&bSelectAllDll;
          }
          bLockUpdateStatus=TRUE;

          for (nIndex=0; nIndex < nMaxIndex; ++nIndex)
          {
            lvi.mask=LVIF_STATE;
            lvi.iItem=nIndex;
            lvi.iSubItem=LVSI_NAME;
            lvi.state=((*lpbSelectAll + 1) << 12);
            lvi.stateMask=LVIS_STATEIMAGEMASK;
            ListView_SetItemWide(pnmlv->hdr.hwndFrom, &lvi);
          }
          *lpbSelectAll=!*lpbSelectAll;

          bLockUpdateStatus=FALSE;
          PostMessage(hDlg, AKDLL_UPDATESTATUS, 0, 0);
        }
      }
      else if (pnmlv->hdr.code == LVN_ITEMCHANGING)
      {
        if (pnmlv->uNewState & LVIS_STATEIMAGEMASK)
        {
          FILEITEM *lpFileItem=(FILEITEM *)pnmlv->lParam;

          if (lpFileItem && lpFileItem->dwError)
          {
            //Wrong plugin
            SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, 1);
            return TRUE;
          }
        }
      }
      else if (pnmlv->hdr.code == LVN_ITEMCHANGED)
      {
        if (pnmlv->uNewState & LVIS_STATEIMAGEMASK)
        {
          FILEITEM *lpFileItem=(FILEITEM *)pnmlv->lParam;
          FILEITEM *lpFileCount;
          BOOL bNewState=((pnmlv->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          BOOL bOldState=((pnmlv->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            lpFileItem->nChecked=bNewState;

            //Change check of all referenced files
            for (lpFileCount=hFileStack.first; lpFileCount; lpFileCount=lpFileCount->next)
            {
              if (lpFileCount->mainScript == lpFileItem)
              {
                if (bNewState)
                  ++lpFileCount->nChecked;
                else
                  --lpFileCount->nChecked;
              }
            }
            PostMessage(hDlg, AKDLL_UPDATESTATUS, 0, 0);
          }
        }
      }
      else if ((int)((NMHDR *)lParam)->code == NM_CUSTOMDRAW)
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
          FILEITEM *lpFileItem=(FILEITEM *)lplvcd->nmcd.lItemlParam;
          int nCompare=CR_NOTINSTALLED;

          if (wParam == IDC_LIST_EXE)
          {
            if (lpFileItemAkelPad)
              nCompare=lpFileItemAkelPad->nCompare;
          }
          else if (wParam == IDC_LIST_DLL)
          {
            if (lpFileItem)
            {
              if (lpFileItem->dwError == PE_NOTINLIST)
                nCompare=CR_NOTINSTALLED;
              else if (lpFileItem->dwError == PE_NOTPLUGIN ||
                       lpFileItem->dwError == PE_CANTLOAD)
                nCompare=CR_INSTALLEDNEWER;
              else
                nCompare=lpFileItem->nCompare;
            }
          }

          if (lplvcd->iSubItem == LVSI_NAME ||
              lplvcd->iSubItem == LVSI_LATEST)
          {
            if (nCompare == CR_NOTINSTALLED) //Not installed
            {
              lplvcd->clrText=RGB(0xC0, 0xC0, 0xC0);
              lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
            }
            else if (nCompare == CR_INSTALLEDNEWER) //Installed version is higher than on site
            {
              lplvcd->clrText=RGB(0xFF, 0x00, 0x00);
              lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
            }
            else if (nCompare == CR_INSTALLEDOLDER) //New version available on site
            {
              lplvcd->clrText=RGB(0x00, 0x00, 0xFF);
              lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
            }
          }
          else
          {
            lplvcd->clrText=RGB(0x00, 0x00, 0x00);
            lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
          }
          lResult=CDRF_DODEFAULT;
        }
        else lResult=CDRF_DODEFAULT;

        SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, lResult);
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SCRIPTS)
    {
      bScripts=(BOOL)SendMessage(hWndScripts, BM_GETCHECK, 0, 0);

      if (bScripts)
      {
        if (!DllAkelUpdaterFill)
        {
          pushstringWide(L"ini");
          pushstringWide(wszScriptsPack);
          if (lpDownloadScriptsProc >= 0)
            g_pluginParms->ExecuteCodeSegment(lpDownloadScriptsProc - 1, 0);
          ParseLst(hDlg);
          CompareItems();
        }
      }
      FillItems(hDlg, NULL, hWndListDll);
      PostMessage(hDlg, AKDLL_UPDATESTATUS, 0, 0);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      STACKFILEITEM hPackStack={0};
      FILEITEM *lpFileItem;
      FILEITEM *lpCopyItem;
      FILEITEM *lpPackItem;
      int nCountDLL=0;
      int nSelection;
      int nOffset;

      for (lpFileItem=hFileStack.first; lpFileItem; lpFileItem=lpFileItem->next)
      {
        if (lpFileItem->nChecked > 0)
        {
          if (lpFileItem->nType == FIT_SCRIPT)
          {
            //Get checked pack
            if (!(lpPackItem=StackFileGet(&hPackStack, lpFileItem->wszPack)))
              lpPackItem=StackFileInsert(&hPackStack, lpFileItem->wszPack);
          }
          else if (lpFileItem->nType == FIT_PLUGIN)
          {
            ++nCountDLL;
          }

          //"Type|Pack|OrigName"
          nOffset=xprintfW(wszBuf, L"%d|%s|%s", lpFileItem->nType, lpFileItem->wszPack, lpFileItem->wszName);

          //Append copies and push them in format "Type|Pack|OrigName|CopyName1|CopyName2"
          for (lpCopyItem=lpFileItem->firstCopy; lpCopyItem; lpCopyItem=lpCopyItem->next)
          {
            nOffset+=xprintfW(wszBuf + nOffset, L"|%s", lpCopyItem->wszName);
          }

          pushstringWide(wszBuf);
        }
      }

      if (hPackStack.first)
      {
        //Prepare script packs list to download: "KDJ|Infocatcher..."
        wszScriptsPack[0]=L'\0';
        nOffset=0;

        for (lpPackItem=hPackStack.first; lpPackItem; lpPackItem=lpPackItem->next)
        {
          nOffset+=xprintfW(wszScriptsPack + nOffset, L"%s|", lpPackItem->wszName);
        }
        StackFilesFree(&hPackStack);

        //Download script packs
        pushstringWide(L"zip");
        pushstringWide(wszScriptsPack);
        if (lpDownloadScriptsProc >= 0)
          g_pluginParms->ExecuteCodeSegment(lpDownloadScriptsProc - 1, 0);
      }

      xprintfW(wszBuf, L"%s|%d", lpFileItemAkelPad->nChecked > 0?lpFileItemAkelPad->wszLastVer:L"0", nCountDLL);
      setuservariable(INST_0, wszBuf);

      wszBuf[0]=L'\0';
      if ((nSelection=SendMessage(hWndMirror, CB_GETCURSEL, 0, 0)) != CB_ERR)
        ComboBox_GetLBTextWide(hWndMirror, nSelection, wszBuf);
      setuservariable(INST_1, wszBuf);

      wszBuf[0]=L'\0';
      if ((nSelection=SendMessage(hWndLanguage, CB_GETCURSEL, 0, 0)) != CB_ERR)
        ComboBox_GetLBTextWide(hWndLanguage, nSelection, wszBuf);
      setuservariable(INST_2, wszBuf);

      StackFilesFree(&hFileStack);
      EndDialog(hDlg, TRUE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      StackFilesFree(&hFileStack);
      EndDialog(hDlg, FALSE);
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    DestroyIcon(hIconDlg);
  }

  DialogResizeMessages(&drs[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, DRM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

void ParseLst(HWND hDlg)
{
  wchar_t wszNoSupport64Bit[MAX_PATH];
  wchar_t wszString[MAX_PATH];
  wchar_t wszFile[MAX_PATH];
  wchar_t wszPack[MAX_PATH];
  wchar_t wszName[MAX_PATH];
  wchar_t *wpContent;
  wchar_t *wpMaxContent;
  wchar_t *wpCount;
  wchar_t *wpLineEnd;
  FILEITEM *lpFileItem=NULL;
  HANDLE hFile;
  INT_PTR nNameLen;
  UINT_PTR nContentLen;
  int nMajor;
  int nMinor;
  int nRelease;
  int nBuild;
  int nCodePage;
  BOOL bBOM;

  //Parse "versions.lst"
  if (!wszScriptsPack[0])
  {
    wszNoSupport64Bit[0]=L'\0';

    //Add installed
    StackFilesFill(&hFileStack);

    xstrcpynW(wszPack, L"PlugsPack", MAX_PATH);
    xprintfW(wszFile, L"%s\\versions.lst", wszNsisTempDir);
    hFile=CreateFileWide(wszFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
      if (AutodetectCodePage(NULL, hFile, ADT_DETECT_CODEPAGE|ADT_DETECT_BOM, &nCodePage, &bBOM) == EDT_SUCCESS)
      {
        SetFilePointer64(hFile, 0, FILE_BEGIN);
        wpContent=NULL;

        if (nContentLen=ReadFileContent(hFile, (UINT_PTR)-1, nCodePage, bBOM, &wpContent))
        {
          wpMaxContent=wpContent + nContentLen;
          wpCount=wpContent;

          while (wpCount < wpMaxContent)
          {
            wpLineEnd=wpCount;
            while (*wpLineEnd != L'\r' && *wpLineEnd != L'\n' && wpLineEnd < wpMaxContent) ++wpLineEnd;

            if (wpCount < wpLineEnd)
            {
              if ((nNameLen=GetNextWord(wpCount, wpLineEnd - wpCount, L"='", 2, wszName, MAX_PATH, &wpCount)) &&
                  GetNextWord(wpCount, wpLineEnd - wpCount, L"'", 1, wszString, MAX_PATH, &wpCount))
              {
                if (!wszNoSupport64Bit[0] && !xstrcmpiW(wszName, L"$NoSupport64Bit"))
                {
                  xstrcpynW(wszNoSupport64Bit, wszString, MAX_PATH);
                }
                else if (!wszScriptsPack[0] && !xstrcmpiW(wszName, L"$ScriptsPack"))
                {
                  xstrcpynW(wszScriptsPack, wszString, MAX_PATH);
                }
                else
                {
                  //Trim "Ver"
                  if (nNameLen > 3 && !xstrcmpiW(wszName + nNameLen - 3, L"Ver"))
                  {
                    nNameLen-=3;
                    wszName[nNameLen]=L'\0';
                  }
                  //Trim "$"
                  xstrcpynW(wszName, wszName + 1, MAX_PATH);

                  if ((nInputBit == 64 && xstrstrW(wszNoSupport64Bit, -1, wszName, nNameLen, FALSE, NULL, NULL)) ||
                      (lpFileItemAkelPad && !xstrcmpnW(L"AkelPad", wszName, (UINT_PTR)-1)))
                  {
                    //Skip line
                  }
                  else
                  {
                    if (!(lpFileItem=StackFileGet(&hFileStack, wszName)))
                      lpFileItem=StackFileInsert(&hFileStack, wszName);
                    if (lpFileItem)
                    {
                      xstrcpynW(lpFileItem->wszLastVer, wszString, MAX_PATH);

                      if (!lpFileItemAkelPad && !xstrcmpnW(L"AkelPad", wszName, (UINT_PTR)-1))
                      {
                        lpFileItem->nType=FIT_AKELPAD;
                        xprintfW(wszBuf, L"%s\\AkelPad.exe", wszAkelDir);
                        if (GetFileVersionWide(wszBuf, &nMajor, &nMinor, &nRelease, &nBuild, NULL))
                          xprintfW(lpFileItem->wszCurVer, L"%d.%d.%d", nMajor, nMinor, nRelease);
                        else
                        {
                          xprintfW(wszBuf, L"%s\\notepad.exe", wszAkelDir);
                          if (GetFileVersionWide(wszBuf, &nMajor, &nMinor, &nRelease, &nBuild, NULL))
                            xprintfW(lpFileItem->wszCurVer, L"%d.%d.%d", nMajor, nMinor, nRelease);
                        }
                        lpFileItemAkelPad=lpFileItem;
                      }
                      else if (!lpFileItemScripts && !xstrcmpnW(L"Scripts", wszName, (UINT_PTR)-1))
                      {
                        lpFileItem->nType=FIT_PLUGIN;
                        lpFileItemScripts=lpFileItem;
                        xstrcpynW(lpFileItem->wszPack, wszPack, MAX_PATH);
                      }
                      else
                      {
                        lpFileItem->nType=FIT_PLUGIN;
                        xstrcpynW(lpFileItem->wszPack, wszPack, MAX_PATH);
                      }
                    }
                  }
                }
              }
            }
            wpCount=wpLineEnd;
            while (*wpCount == L'\r' || *wpCount == L'\n') ++wpCount;
          }
          GlobalFree((HGLOBAL)wpContent);
        }
      }
      CloseHandle(hFile);
    }
    StackFilesSort(&hFileStack);
  }

  //Parse wszScriptsPack: "KDJ|Infocatcher|Instructor|VladSh"
  if (bScripts && wszScriptsPack[0])
  {
    FILEITEM *lpMainScript;
    LISTITEM *lpScriptItem;
    wchar_t *wpPack=wszScriptsPack;
    wchar_t *wpMaxPack=wszScriptsPack + xstrlenW(wszScriptsPack);
    INT_PTR nVersionLen;
    HMODULE hInstance;
    BOOL bScriptsSection;
    LISTCOLUMN lpColumns[]={{LVI_SCRIPT,      163, LCF_VISIBLE},
                            {LVI_VERSION,     70,  LCF_VISIBLE|LCF_CONTENT},
                            {LVI_DESCRIPTION, 300, LCF_VISIBLE|LCF_CONTENT},
                            {LVI_AUTHOR,      70,  LCF_VISIBLE|LCF_CONTENT},
                            //{LVI_SITE,        70,  LCF_VISIBLE|LCF_CONTENT},
                            {-1, 0, 0}};

    xprintfW(wszFile, L"%s\\Scripts.dll", wszPlugsDir);
    if (hInstance=LoadLibraryWide(wszFile))
    {
      //Fill hScriptsStack with current versions
      if (!DllAkelUpdaterFill)
        DllAkelUpdaterFill=(void (*)(HWND, HWND, STACKLISTITEM *, LISTCOLUMN *))GetProcAddress(hInstance, "DllAkelUpdaterFill");
      if (DllAkelUpdaterFill)
        DllAkelUpdaterFill(hDlg, NULL, &hScriptsStack, lpColumns);

      while (GetNextWord(wpPack, wpMaxPack - wpPack, L"|", 1, wszPack, MAX_PATH, &wpPack))
      {
        xprintfW(wszFile, L"%s\\%s.ini", wszNsisTempDir, wszPack);
        hFile=CreateFileWide(wszFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
          if (AutodetectCodePage(NULL, hFile, ADT_DETECT_CODEPAGE|ADT_DETECT_BOM, &nCodePage, &bBOM) == EDT_SUCCESS)
          {
            SetFilePointer64(hFile, 0, FILE_BEGIN);
            wpContent=NULL;

            if (nContentLen=ReadFileContent(hFile, (UINT_PTR)-1, nCodePage, bBOM, &wpContent))
            {
              wpMaxContent=wpContent + nContentLen;
              wpCount=wpContent;
              bScriptsSection=FALSE;

              while (wpCount < wpMaxContent)
              {
                lpMainScript=NULL;
                wpLineEnd=wpCount;
                while (*wpLineEnd != L'\r' && *wpLineEnd != L'\n' && wpLineEnd < wpMaxContent) ++wpLineEnd;

                if (wpCount < wpLineEnd)
                {
                  if (*wpCount == L'[' && *(wpLineEnd - 1) == L']')
                  {
                    if (!xstrcmpinW(L"[Scripts]", wpCount, (UINT_PTR)-1))
                      bScriptsSection=TRUE;
                    else
                      bScriptsSection=FALSE;
                  }
                  if (bScriptsSection)
                  {
                    if (GetNextWord(wpCount, wpLineEnd - wpCount, L"=", 1, wszName, MAX_PATH, &wpCount))
                    {
                      if (nVersionLen=GetNextWord(wpCount, wpLineEnd - wpCount, L"\"", 1, wszString, MAX_PATH, &wpCount))
                      {
                        //Trim trailing spaces
                        while (nVersionLen > 0 && (wszString[nVersionLen - 1] == L' ' || wszString[nVersionLen - 1] == L'\t'))
                          wszString[--nVersionLen]=L'\0';

                        //Main script
                        if (!(lpMainScript=StackFileGet(&hFileStack, wszName)))
                          lpMainScript=StackFileInsert(&hFileStack, wszName);
                        if (lpMainScript)
                        {
                          lpMainScript->nType=FIT_SCRIPT;
                          xstrcpynW(lpMainScript->wszLastVer, wszString, MAX_PATH);
                          xstrcpynW(lpMainScript->wszPack, wszPack, MAX_PATH);

                          if (lpScriptItem=StackScriptGet(&hScriptsStack, wszName))
                          {
                            xstrcpynW(lpMainScript->wszCurVer, lpScriptItem->wpVersion, MAX_PATH);
                            xstrcpynW(lpMainScript->wszDescription, lpScriptItem->wpDescription, MAX_PATH);
                            xstrcpynW(lpMainScript->wszAuthor, lpScriptItem->wpAuthor, MAX_PATH);
                          }
                        }

                        //Script's additional files
                        if (*(wpCount - 1) == L'\"')
                        {
                          while (GetNextWord(wpCount, wpLineEnd - wpCount, L"\"", 1, wszName, MAX_PATH, &wpCount))
                          {
                            if (!(lpFileItem=StackFileGet(&hFileStack, wszName)))
                              lpFileItem=StackFileInsert(&hFileStack, wszName);
                            if (lpFileItem)
                            {
                              lpFileItem->nType=FIT_FORSCRIPT;
                              lpFileItem->mainScript=lpMainScript;
                              xstrcpynW(lpFileItem->wszPack, wszPack, MAX_PATH);
                            }
                            if (!GetNextWord(wpCount, wpLineEnd - wpCount, L"\"", 1, wszName, MAX_PATH, &wpCount))
                              break;
                          }
                        }
                      }
                    }
                  }
                }
                wpCount=wpLineEnd;
                while (*wpCount == L'\r' || *wpCount == L'\n') ++wpCount;
              }
              GlobalFree((HGLOBAL)wpContent);
            }
          }
          CloseHandle(hFile);
        }
      }

      if (!DllAkelUpdaterFree)
        DllAkelUpdaterFree=(void (*)(STACKLISTITEM *))GetProcAddress(hInstance, "DllAkelUpdaterFree");
      if (DllAkelUpdaterFree)
        DllAkelUpdaterFree(&hScriptsStack);
      FreeLibrary(hInstance);
    }
    StackFilesSort(&hFileStack);
  }
}

void CompareItems()
{
  FILEITEM *lpFileItem;
  int nCompare;

  for (lpFileItem=hFileStack.first; lpFileItem; lpFileItem=lpFileItem->next)
  {
    if (!lpFileItem->wszLastVer[0])
    {
      //Unofficial file
      if (!lpFileItem->dwError)
      {
        xprintfW(lpFileItem->wszError, L"%s %s", lpFileItem->wszName, GetLangStringW(wLangSystem, STRID_ERRORNOTINLIST));
        lpFileItem->dwError=PE_NOTINLIST;
        lpFileItem->nChecked=0;
      }
    }
    else
    {
      //Official file
      if (lpFileItem->wszCurVer[0])
      {
        nCompare=VersionCompare(lpFileItem->wszLastVer, lpFileItem->wszCurVer);
        if (nCompare > 0)
          lpFileItem->nCompare=CR_INSTALLEDOLDER;
        else if (nCompare < 0)
          lpFileItem->nCompare=CR_INSTALLEDNEWER;
        else
          lpFileItem->nCompare=CR_EQUAL;
      }
      else lpFileItem->nCompare=CR_NOTINSTALLED;

      if (lpFileItem->nChecked == -1)
      {
        if (lpFileItem->nCompare == CR_INSTALLEDOLDER)
          lpFileItem->nChecked=1;
        else
          lpFileItem->nChecked=0;
      }
    }
  }
}

void FillItems(HWND hDlg, HWND hWndListExe, HWND hWndListDll)
{
  FILEITEM *lpFileItem;
  FILEITEM *lpCopyItem;
  HWND hWndList;
  LVCOLUMNW lvc;
  LVITEMW lvi;
  int nIndex;
  int nOffset;

  if (hWndListExe)
  {
    SendMessage(hWndListExe, WM_SETREDRAW, FALSE, 0);
    SendMessage(hWndListExe, LVM_DELETEALLITEMS, 0, 0);
    bSelectAllExe=TRUE;
  }
  if (hWndListDll)
  {
    if (bScripts)
    {
      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangSystem, STRID_DESCRIPTION);
      lvc.cx=460;
      lvc.iSubItem=LVSI_DESCRIPTION;
      ListView_InsertColumnWide(hWndListDll, LVSI_DESCRIPTION, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangSystem, STRID_AUTHOR);
      lvc.cx=130;
      lvc.iSubItem=LVSI_AUTHOR;
      ListView_InsertColumnWide(hWndListDll, LVSI_AUTHOR, &lvc);
    }
    else
    {
      SendMessage(hWndListDll, LVM_DELETECOLUMN, LVSI_AUTHOR, 0);
      SendMessage(hWndListDll, LVM_DELETECOLUMN, LVSI_DESCRIPTION, 0);
    }
    xprintfW(wszBuf, L"%s%s", GetLangStringW(wLangSystem, bScripts?STRID_SCRIPT:STRID_PLUGIN), (!bScripts && !bInputNoCopies)?GetLangStringW(wLangSystem, STRID_COPIES):NULL);
    lvc.mask=LVCF_TEXT;
    lvc.pszText=(wchar_t *)wszBuf;
    ListView_SetColumnWide(hWndListDll, LVSI_NAME, &lvc);

    SendMessage(hWndListDll, WM_SETREDRAW, FALSE, 0);
    SendMessage(hWndListDll, LVM_DELETEALLITEMS, 0, 0);
    if (bScripts)
      bSelectAllJs=TRUE;
    else
      bSelectAllDll=TRUE;
  }

  for (lpFileItem=hFileStack.first; lpFileItem; lpFileItem=lpFileItem->next)
  {
    if (lpFileItemAkelPad == lpFileItem)
    {
      if (!hWndListExe) continue;
      hWndList=hWndListExe;
    }
    else
    {
      if (!hWndListDll) continue;
      if (lpFileItem->mainScript) continue;
      if ((lpFileItem->nType == FIT_SCRIPT && !bScripts) ||
          (lpFileItem->nType == FIT_PLUGIN && bScripts))
          continue;
      hWndList=hWndListDll;
    }

    if (hWndList == hWndListExe)
    {
      if (nInputBit == 64)
        xprintfW(wszBuf, L"%s (x64)", lpFileItem->wszName);
      else
        xprintfW(wszBuf, L"%s (x86)", lpFileItem->wszName);
    }
    else if (hWndList == hWndListDll)
    {
      nOffset=xstrcpynW(wszBuf, lpFileItem->wszName, MAX_PATH);

      //Append copies
      if (lpCopyItem=lpFileItem->firstCopy)
      {
        nOffset+=xprintfW(wszBuf + nOffset, L" (%s", lpCopyItem->wszName);

        for (lpCopyItem=lpCopyItem->next; lpCopyItem; lpCopyItem=lpCopyItem->next)
        {
          nOffset+=xprintfW(wszBuf + nOffset, L", %s", lpCopyItem->wszName);
        }
        xprintfW(wszBuf + nOffset, L")");
      }
    }

    lvi.mask=LVIF_TEXT;
    lvi.pszText=lpFileItem->dwError?lpFileItem->wszError:wszBuf;
    lvi.iItem=0x7FFFFFFF;
    lvi.iSubItem=LVSI_NAME;
    nIndex=ListView_InsertItemWide(hWndList, &lvi);

    lvi.mask=LVIF_STATE|LVIF_PARAM;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVSI_NAME;
    lvi.state=((lpFileItem->nChecked + 1) << 12);
    lvi.stateMask=LVIS_STATEIMAGEMASK;
    lvi.lParam=(LPARAM)lpFileItem;
    ListView_SetItemWide(hWndList, &lvi);

    lvi.mask=LVIF_TEXT;
    lvi.pszText=lpFileItem->wszLastVer;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVSI_LATEST;
    ListView_SetItemWide(hWndList, &lvi);

    lvi.mask=LVIF_TEXT;
    lvi.pszText=lpFileItem->wszCurVer;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVSI_CURRENT;
    ListView_SetItemWide(hWndList, &lvi);

    if (bScripts)
    {
      lvi.mask=LVIF_TEXT;
      lvi.pszText=lpFileItem->wszDescription;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVSI_DESCRIPTION;
      ListView_SetItemWide(hWndList, &lvi);

      lvi.mask=LVIF_TEXT;
      lvi.pszText=lpFileItem->wszAuthor;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVSI_AUTHOR;
      ListView_SetItemWide(hWndList, &lvi);
    }

    if (lpFileItem->nCompare == CR_INSTALLEDOLDER)
    {
      if (hWndList == hWndListExe)
        bSelectAllExe=FALSE;
      else if (hWndList == hWndListDll)
      {
        if (bScripts)
          bSelectAllJs=FALSE;
        else
          bSelectAllDll=FALSE;
      }
    }
  }

  if (hWndListExe)
  {
    SendMessage(hWndListExe, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWndListExe, NULL, TRUE);
  }
  if (hWndListDll)
  {
    SendMessage(hWndListDll, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWndListDll, NULL, TRUE);
  }
}

int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg)
{
  const wchar_t *wpCount=wpCmdLine;
  wchar_t *wpArgSet=wszArg;
  wchar_t *wpArgSetMax=wszArg + nArgMax - 1;
  wchar_t wchInitStopChar;
  wchar_t wchCurStopChar;

  while (*wpCount == L' ') ++wpCount;

  if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
    wchInitStopChar=*wpCount++;
  else
    wchInitStopChar=L' ';
  wchCurStopChar=wchInitStopChar;

  for (; *wpCount; ++wpCount)
  {
    if (wchCurStopChar == L' ')
    {
      if (*wpCount == L' ')
        break;
      if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
        wchCurStopChar=*wpCount;
    }
    else if (wchCurStopChar == *wpCount)
    {
      if (wchCurStopChar == wchInitStopChar)
        break;
      wchCurStopChar=L' ';
    }

    if (wpArgSet < wpArgSetMax)
    {
      if (wszArg) *wpArgSet=*wpCount;
      ++wpArgSet;
    }
  }
  if (wszArg) *wpArgSet=L'\0';

  if (wpNextArg)
  {
    if (*wpCount) ++wpCount;
    for (*wpNextArg=wpCount; **wpNextArg == L' '; ++*wpNextArg);
  }
  return (int)(wpArgSet - wszArg);
}

int GetNextWord(wchar_t *wpStr, INT_PTR nStrLen, wchar_t *wpDelim, int nDelimLen, wchar_t *wszWord, int nWordMax, const wchar_t **wpNextWord)
{
  wchar_t *wpMatchBegin;
  wchar_t *wpMatchEnd;

  xstrstrW(wpStr, nStrLen, wpDelim, nDelimLen, TRUE, &wpMatchBegin, &wpMatchEnd);
  if (wpNextWord) *wpNextWord=wpMatchEnd;
  return xstrcpynW(wszWord, wpStr, min(wpMatchBegin - wpStr + 1, nWordMax));
}

BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc)
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

BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcMinMax, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG)
  {
    RECT rcTemplate;
    RECT rcControl;
    DWORD dwFlags=SWP_NOMOVE;
    int i;

    rcTemplate=*rcCurrent;
    GetWindowPos(hDlg, NULL, rcCurrent);

    for (i=0; drs[i].lpWnd; ++i)
    {
      if (*drs[i].lpWnd)
      {
        GetWindowPos(*drs[i].lpWnd, hDlg, &rcControl);
        if (drs[i].dwType & DRS_SIZE)
        {
          if (drs[i].dwType & DRS_X)
            drs[i].nOffset=rcCurrent->right - (rcControl.left + rcControl.right);
          else if (drs[i].dwType & DRS_Y)
            drs[i].nOffset=rcCurrent->bottom - (rcControl.top + rcControl.bottom);
        }
        else if (drs[i].dwType & DRS_MOVE)
        {
          if (drs[i].dwType & DRS_X)
            drs[i].nOffset=rcCurrent->right - rcControl.left;
          else if (drs[i].dwType & DRS_Y)
            drs[i].nOffset=rcCurrent->bottom - rcControl.top;
        }
      }
    }

    if (rcTemplate.right && rcTemplate.bottom)
    {
      if (GetWindowLongPtrWide(hDlg, GWL_STYLE) & DS_CENTER)
      {
        rcTemplate.left=rcCurrent->left + (rcCurrent->right - rcTemplate.right) / 2;
        rcTemplate.top=rcCurrent->top + (rcCurrent->bottom - rcTemplate.bottom) / 2;
        dwFlags&=~SWP_NOMOVE;
      }
      SetWindowPos(hDlg, 0, rcTemplate.left, rcTemplate.top, rcTemplate.right, rcTemplate.bottom, dwFlags|SWP_NOZORDER|SWP_NOACTIVATE);
    }
  }
  else if (uMsg == WM_GETMINMAXINFO)
  {
    if (rcMinMax)
    {
      MINMAXINFO *mmi=(MINMAXINFO *)lParam;

      if (rcMinMax->left)
        mmi->ptMinTrackSize.x=rcMinMax->left;
      if (rcMinMax->top)
        mmi->ptMinTrackSize.y=rcMinMax->top;
      if (rcMinMax->right)
        mmi->ptMaxTrackSize.x=rcMinMax->right;
      if (rcMinMax->bottom)
        mmi->ptMaxTrackSize.y=rcMinMax->bottom;
    }
  }
  else if (uMsg == WM_MOVE)
  {
    if (!(GetWindowLongPtrWide(hDlg, GWL_STYLE) & DS_CENTER))
    {
      RECT rcTemplate;

      GetWindowPos(hDlg, NULL, &rcTemplate);
      rcCurrent->left=rcTemplate.left;
      rcCurrent->top=rcTemplate.top;
      return TRUE;
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      RECT rcControl;
      DWORD dwFlags;
      int i;

      GetWindowPos(hDlg, NULL, rcCurrent);

      for (i=0; drs[i].lpWnd; ++i)
      {
        if (*drs[i].lpWnd)
        {
          dwFlags=0;
          if (drs[i].dwType & DRS_SIZE)
            dwFlags|=SWP_NOMOVE;
          else if (drs[i].dwType & DRS_MOVE)
            dwFlags|=SWP_NOSIZE;
          else
            continue;

          GetWindowPos(*drs[i].lpWnd, hDlg, &rcControl);
          SetWindowPos(*drs[i].lpWnd, 0, (drs[i].dwType & DRS_X)?(rcCurrent->right - drs[i].nOffset):rcControl.left,
                                         (drs[i].dwType & DRS_Y)?(rcCurrent->bottom - drs[i].nOffset):rcControl.top,
                                         (drs[i].dwType & DRS_X)?(rcCurrent->right - rcControl.left - drs[i].nOffset):rcControl.right,
                                         (drs[i].dwType & DRS_Y)?(rcCurrent->bottom - rcControl.top - drs[i].nOffset):rcControl.bottom,
                                          dwFlags|SWP_NOZORDER|SWP_NOACTIVATE);
        }
      }
      InvalidateRect(hDlg, NULL, TRUE);
      return TRUE;
    }
  }
  else if (uMsg == WM_PAINT)
  {
    if (dwFlags & DRM_PAINTSIZEGRIP)
    {
      PAINTSTRUCT ps;
      RECT rcGrip;
      HDC hDC;

      if (hDC=BeginPaint(hDlg, &ps))
      {
        GetClientRect(hDlg, &rcGrip);
        rcGrip.left=rcGrip.right - GetSystemMetrics(SM_CXVSCROLL);
        rcGrip.top=rcGrip.bottom - GetSystemMetrics(SM_CYVSCROLL);
        DrawFrameControl(hDC, &rcGrip, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
        EndPaint(hDlg, &ps);
      }
    }
  }
  return FALSE;
}

void StackFilesFill(STACKFILEITEM *hStack)
{
  wchar_t wszSearchDir[MAX_PATH];
  wchar_t wszFile[MAX_PATH];
  wchar_t wszBaseName[MAX_PATH];
  const wchar_t *pDllExt=L"dll";
  WIN32_FIND_DATAW wfd;
  PLUGINVERSION pv;
  FILEITEM *lpFileItem;
  HANDLE hSearch;
  HMODULE hInstance;
  int nMajor;
  int nMinor;
  int nRelease;
  int nBuild;
  void (*DllAkelPadID)(PLUGINVERSION *pv);

  xprintfW(wszSearchDir, L"%s\\*.%s", wszPlugsDir, pDllExt);

  if ((hSearch=FindFirstFileWide(wszSearchDir, &wfd)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      //Avoid FindFirstFile/FindNextFile bug: "*.dll_ANYSYMBOLS" is also matched
      if (xstrcmpiW(pDllExt, GetFileExt(wfd.cFileName, -1)))
        continue;

      //Get real name in diGlobal.wszName
      xprintfW(wszFile, L"%s\\%s", wszPlugsDir, wfd.cFileName);
      diGlobal.wszName[0]=L'\0';
      diGlobal.dwError=PE_CANTLOAD;

      if (bInputNoCopies)
      {
        GetBaseName(wfd.cFileName, -1, diGlobal.wszName, MAX_PATH);
        diGlobal.dwError=PE_NONE;
      }
      else
      {
        if (nInputBit == 64)
        {
          STARTUPINFOW si;
          PROCESS_INFORMATION pi;

          xprintfW(wszBuf, L"\"%s\\%s\" \"%Id\" \"%s\"", wszNsisTempDir, wszInputHelper, hWndDialog, wszFile);
          xmemset(&si, 0, sizeof(STARTUPINFOW));
          si.cb=sizeof(STARTUPINFOW);

          if (CreateProcessWide(NULL, wszBuf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
          {
            //Wait for pi.hProcess and process messages.
            MSG msg;
            BOOL bExitLoop=FALSE;

            for (;;)
            {
              while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
              {
                if (msg.message == WM_QUIT)
                  bExitLoop=TRUE;
                else
                {
                  TranslateMessage(&msg);
                  DispatchMessageWide(&msg);
                }
              }
              if (bExitLoop)
                break;
              if (MsgWaitForMultipleObjects(1, &pi.hProcess, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
                break;
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
          }
        }
        else
        {
          if (hInstance=LoadLibraryWide(wszFile))
          {
            if (DllAkelPadID=(void (*)(PLUGINVERSION *))GetProcAddress(hInstance, "DllAkelPadID"))
            {
              DllAkelPadID(&pv);

              MultiByteToWideChar(CP_ACP, 0, pv.pPluginName, -1, diGlobal.wszName, MAX_PATH);
              diGlobal.dwError=PE_NONE;
            }
            else diGlobal.dwError=PE_NOTPLUGIN;

            FreeLibrary(hInstance);
          }
        }
      }

      //Add to stack
      if (diGlobal.wszName[0])
      {
        if (!(lpFileItem=StackFileGet(hStack, diGlobal.wszName)))
          lpFileItem=StackFileInsert(hStack, diGlobal.wszName);

        if (lpFileItem)
        {
          lpFileItem->nType=FIT_PLUGIN;

          //Is copy?
          GetBaseName(wfd.cFileName, -1, wszBaseName, MAX_PATH);
          if (xstrcmpiW(wszBaseName, diGlobal.wszName))
            StackFileInsert((STACKFILEITEM *)&lpFileItem->firstCopy, wszBaseName);

          //Get version
          if (GetFileVersionWide(wszFile, &nMajor, &nMinor, &nRelease, &nBuild, NULL))
            xprintfW(lpFileItem->wszCurVer, L"%d.%d", nMajor, nMinor);
        }
      }
      if (diGlobal.dwError)
      {
        if (lpFileItem=StackFileInsert(hStack, wfd.cFileName))
        {
          if (diGlobal.dwError == PE_NOTPLUGIN)
            xprintfW(lpFileItem->wszError, L"%s %s", lpFileItem->wszName, GetLangStringW(wLangSystem, STRID_ERRORNOTPLUGIN));
          else if (diGlobal.dwError == PE_CANTLOAD)
            xprintfW(lpFileItem->wszError, L"%s %s", lpFileItem->wszName,  GetLangStringW(wLangSystem, STRID_ERRORCANTLOAD));
          else
            xprintfW(lpFileItem->wszError, L"%s", lpFileItem->wszName);
          lpFileItem->dwError=diGlobal.dwError;
        }
      }
    }
    while (FindNextFileWide(hSearch, &wfd));

    FindClose(hSearch);
  }
}

FILEITEM* StackFileInsert(STACKFILEITEM *hStack, const wchar_t *wpPluginName)
{
  FILEITEM *lpElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(FILEITEM)))
  {
    xstrcpynW(lpElement->wszName, wpPluginName, MAX_PATH);
    lpElement->nChecked=-1;
  }
  return lpElement;
}

FILEITEM* StackFileGet(STACKFILEITEM *hStack, const wchar_t *wpPluginName)
{
  FILEITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(lpElement->wszName, wpPluginName))
      return lpElement;
  }
  return NULL;
}

int StackFileSelCount(STACKFILEITEM *hStack)
{
  FILEITEM *lpElement;
  int nSelCount=0;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->mainScript) continue;

    if (lpElement->nChecked > 0)
      ++nSelCount;
  }
  return nSelCount;
}

void StackFilesSort(STACKFILEITEM *hStack)
{
  FILEITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    StackSort(&lpElement->firstCopy, &lpElement->lastCopy, 1);
  }
  StackSort(&hStack->first, &hStack->last, 1);
}

int StackSort(FILEITEM **first, FILEITEM **last, int nUpDown)
{
  FILEITEM *tmp1;
  FILEITEM *tmp2;
  FILEITEM *tmpNext;
  int i;

  if (nUpDown != 1 && nUpDown != -1) return 1;

  for (tmp1=*first; tmp1; tmp1=tmpNext)
  {
    tmpNext=tmp1->next;

    for (tmp2=*first; tmp2 != tmp1; tmp2=tmp2->next)
    {
      i=xstrcmpiW(tmp2->wszName, tmp1->wszName);

      if (i == 0 || i == nUpDown)
      {
        StackMoveBefore((stack **)first, (stack **)last, (stack *)tmp1, (stack *)tmp2);
        break;
      }
    }
  }
  return 0;
}

void StackFilesFree(STACKFILEITEM *hStack)
{
  FILEITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    StackClear((stack **)&lpElement->firstCopy, (stack **)&lpElement->lastCopy);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

LISTITEM* StackScriptGet(STACKLISTITEM *hStack, const wchar_t *wpScriptFileName)
{
  LISTITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(lpElement->wpScript, wpScriptFileName))
      return lpElement;
  }
  return NULL;
}

int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; wpCount >= wpFile; --wpCount)
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

int GetBaseName(const wchar_t *wpFile, int nFileLen, wchar_t *wszBaseName, int nBaseNameMax)
{
  const wchar_t *wpCount;
  const wchar_t *wpExt=NULL;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; wpCount >= wpFile; --wpCount)
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

const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; wpCount >= wpFile; --wpCount)
  {
    if (*wpCount == L'.') return wpCount + 1;
    if (*wpCount == L'\\') break;
  }
  return NULL;
}

int VersionCompare(const wchar_t *wpVer1, const wchar_t *wpVer2)
{
  const wchar_t *v1;
  const wchar_t *v2;
  const wchar_t *c1=wpVer1;
  const wchar_t *c2=wpVer2;
  int nCompare;

  while (*c1 || *c2)
  {
    //Compare until numbers found
    for (; (*c1 < L'0' || *c1 > L'9') || (*c2 < L'0' || *c2 > L'9'); ++c1, ++c2)
    {
      if (nCompare=WideCharLower(*c1) - WideCharLower(*c2))
        return nCompare > 0?1:-1;
      if (!*c1 && !*c2)
        return 0;
    }

    //Ignore leading zeros
    for (v1=c1; *v1 == L'0'; ++v1);
    for (v2=c2; *v2 == L'0'; ++v2);

    //Compare numbers
    for (c1=v1; *c1 >= L'0' && *c1 <= L'9'; ++c1);
    for (c2=v2; *c2 >= L'0' && *c2 <= L'9'; ++c2);
    if (c1 - v1 > c2 - v2)
      return 1;
    if (c1 - v1 < c2 - v2)
      return -1;
    if (nCompare=xstrcmpnW(v1, v2, c1 - v1))
      return nCompare;
  }
  return 0;
}

BOOL GetFileVersionA(const char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage)
{
  struct LANGANDCODEPAGE {
    WORD wLanguage;
    WORD wCodePage;
  } *lpTranslate;

  VS_FIXEDFILEINFO ffi;
  VS_FIXEDFILEINFO *pffi=&ffi;
  void *pVerBuf;
  DWORD dwHandle;
  DWORD dwVerSize;
  UINT uLen;
  BOOL bResult=FALSE;

  *nMajor=*nMinor=*nRelease=*nBuild=0;

  if (dwVerSize=GetFileVersionInfoSizeA((char *)pFile, &dwHandle))
  {
    if (pVerBuf=GlobalAlloc(GMEM_FIXED, dwVerSize))
    {
      if (GetFileVersionInfoA((char *)pFile, dwHandle, dwVerSize, pVerBuf))
      {
        if (VerQueryValueA(pVerBuf, "\\", (void **)&pffi, &uLen))
        {
          *nMajor=pffi->dwFileVersionMS / 0x00010000;
          *nMinor=pffi->dwFileVersionMS & 0x0000FFFF;
          *nRelease=pffi->dwFileVersionLS / 0x00010000;
          *nBuild=pffi->dwFileVersionLS & 0x0000FFFF;
          bResult=TRUE;
        }
        if (dwLanguage)
        {
          if (VerQueryValueA(pVerBuf, "\\VarFileInfo\\Translation", (void **)&lpTranslate, &uLen))
          {
            *dwLanguage=lpTranslate->wLanguage;
          }
        }
      }
      GlobalFree((HGLOBAL)pVerBuf);
    }
  }
  return bResult;
}

BOOL GetFileVersionW(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage)
{
  struct LANGANDCODEPAGE {
    WORD wLanguage;
    WORD wCodePage;
  } *lpTranslate;

  VS_FIXEDFILEINFO ffi;
  VS_FIXEDFILEINFO *pffi=&ffi;
  void *pVerBuf;
  DWORD dwHandle;
  DWORD dwVerSize;
  UINT uLen;
  BOOL bResult=FALSE;

  *nMajor=*nMinor=*nRelease=*nBuild=0;

  if (dwVerSize=GetFileVersionInfoSizeW((wchar_t *)wpFile, &dwHandle))
  {
    if (pVerBuf=GlobalAlloc(GMEM_FIXED, dwVerSize))
    {
      if (GetFileVersionInfoW((wchar_t *)wpFile, dwHandle, dwVerSize, pVerBuf))
      {
        if (VerQueryValueW(pVerBuf, L"\\", (void **)&pffi, &uLen))
        {
          *nMajor=pffi->dwFileVersionMS / 0x00010000;
          *nMinor=pffi->dwFileVersionMS & 0x0000FFFF;
          *nRelease=pffi->dwFileVersionLS / 0x00010000;
          *nBuild=pffi->dwFileVersionLS & 0x0000FFFF;
          bResult=TRUE;
        }
        if (dwLanguage)
        {
          if (VerQueryValueW(pVerBuf, L"\\VarFileInfo\\Translation", (void **)&lpTranslate, &uLen))
          {
            *dwLanguage=lpTranslate->wLanguage;
          }
        }
      }
      GlobalFree((HGLOBAL)pVerBuf);
    }
  }
  return bResult;
}

BOOL GetFileVersionWide(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage)
{
  if (!bOldWindows)
    return GetFileVersionW(wpFile, nMajor, nMinor, nRelease, nBuild, NULL);
  else
  {
    char *pFile=AllocAnsi(wpFile);
    BOOL bResult;

    bResult=GetFileVersionA(pFile, nMajor, nMinor, nRelease, nBuild, NULL);

    FreeAnsi(pFile);
    return bResult;
  }
}

int AutodetectCodePage(const wchar_t *wpFile, HANDLE hFile, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
{
  HANDLE hFileInput=hFile;
  BYTE wszFirstBytes[4];
  UINT_PTR dwBytesRead=0;
  int nResult=EDT_SUCCESS;

  if (!hFile)
    hFile=CreateFileWide(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (hFile != INVALID_HANDLE_VALUE)
  {
    if (ReadFile64(hFile, wszFirstBytes, sizeof(wszFirstBytes), &dwBytesRead, NULL))
    {
      if (dwBytesRead >= 4)
      {
        if (wszFirstBytes[0] == 0xFF && wszFirstBytes[1] == 0xFE && wszFirstBytes[2] == 0x00 && wszFirstBytes[3] == 0x00)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF32LE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto End;
        }
        else if (wszFirstBytes[0] == 0x00 && wszFirstBytes[1] == 0x00 && wszFirstBytes[2] == 0xFE && wszFirstBytes[3] == 0xFF)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF32BE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto End;
        }
      }
      if (dwBytesRead >= 2)
      {
        if (wszFirstBytes[0] == 0xFF && wszFirstBytes[1] == 0xFE)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF16LE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto End;
        }
        else if (wszFirstBytes[0] == 0xFE && wszFirstBytes[1] == 0xFF)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF16BE;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto End;
        }
      }
      if (dwBytesRead >= 3)
      {
        if (wszFirstBytes[0] == 0xEF && wszFirstBytes[1] == 0xBB && wszFirstBytes[2] == 0xBF)
        {
          if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=CP_UNICODE_UTF8;
          if (dwFlags & ADT_DETECT_BOM) *bBOM=TRUE;
          goto End;
        }
      }
    }
    else nResult=EDT_READ;
  }
  else nResult=EDT_OPEN;

  //Default
  if (dwFlags & ADT_DETECT_CODEPAGE) *nCodePage=GetACP();
  if (dwFlags & ADT_DETECT_BOM) *bBOM=FALSE;

  End:
  if (hFileInput != hFile && hFile != INVALID_HANDLE_VALUE)
    CloseHandle(hFile);
  return nResult;
}

UINT_PTR ReadFileContent(HANDLE hFile, UINT_PTR dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wppContent)
{
  unsigned char *szBuffer;
  wchar_t *wszBuffer=*wppContent;
  UINT_PTR dwBufferBytes;
  UINT_PTR dwBytesRead;
  UINT_PTR dwCharsConverted=0;

  //Offset BOM
  if (bBOM)
  {
    if (SetFilePointer64(hFile, 0, FILE_CURRENT) == 0)
    {
      if (nCodePage == CP_UNICODE_UTF16LE ||
          nCodePage == CP_UNICODE_UTF16BE)
      {
        SetFilePointer64(hFile, 2, FILE_BEGIN);
      }
      else if (nCodePage == CP_UNICODE_UTF32LE ||
               nCodePage == CP_UNICODE_UTF32BE)
      {
        SetFilePointer64(hFile, 4, FILE_BEGIN);
      }
      else if (nCodePage == CP_UNICODE_UTF8)
      {
        SetFilePointer64(hFile, 3, FILE_BEGIN);
      }
    }
  }
  if (dwBytesMax == (UINT_PTR)-1)
    dwBytesMax=GetFileSize64(hFile);

  if (dwBytesMax != (UINT_PTR)-1)
  {
    if (IsCodePageUnicode(nCodePage) && nCodePage != CP_UNICODE_UTF8)
      dwBufferBytes=dwBytesMax;
    else
      dwBufferBytes=dwBytesMax * sizeof(wchar_t);

    if (!wszBuffer)
      wszBuffer=(wchar_t *)GlobalAlloc(GMEM_FIXED, dwBufferBytes + sizeof(wchar_t));
    if (wszBuffer)
    {
      if (IsCodePageUnicode(nCodePage) && nCodePage != CP_UNICODE_UTF8)
        szBuffer=(unsigned char *)wszBuffer;
      else
        szBuffer=(unsigned char *)wszBuffer + dwBytesMax;

      //Read data from file
      if (ReadFile64(hFile, szBuffer, dwBytesMax, &dwBytesRead, NULL))
      {
        //Translate data to UNICODE
        if (nCodePage == CP_UNICODE_UTF16LE)
        {
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (nCodePage == CP_UNICODE_UTF16BE)
        {
          ChangeTwoBytesOrder(szBuffer, dwBytesRead, NULL);
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (nCodePage == CP_UNICODE_UTF32LE)
        {
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szBuffer, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszBuffer, dwBufferBytes / sizeof(wchar_t));
        }
        else if (nCodePage == CP_UNICODE_UTF32BE)
        {
          ChangeFourBytesOrder(szBuffer, dwBytesRead, NULL);
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szBuffer, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszBuffer, dwBufferBytes / sizeof(wchar_t));
        }
        else
        {
          if (nCodePage == CP_UNICODE_UTF8)
            dwCharsConverted=UTF8toUTF16(szBuffer, dwBytesRead, NULL, (unsigned short *)wszBuffer, dwBufferBytes / sizeof(wchar_t));
          else
            dwCharsConverted=MultiByteToWideChar64(nCodePage, 0, (char *)szBuffer, dwBytesRead, wszBuffer, dwBufferBytes / sizeof(wchar_t));
        }
        wszBuffer[dwCharsConverted]=L'\0';
      }
    }
  }
  *wppContent=wszBuffer;
  return dwCharsConverted;
}

void ChangeTwoBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst)
{
  unsigned char *pLast=pSrc + dwSrcSize - 2;
  unsigned char *pSrcByte=pSrc;
  unsigned char *pDstByte=pDst?pDst:pSrc;
  unsigned char ch2;

  while (pSrcByte <= pLast)
  {
    ch2=*pSrcByte++;
    *pDstByte++=*pSrcByte++;
    *pDstByte++=ch2;
  }
}

void ChangeFourBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst)
{
  unsigned char *pLast=pSrc + dwSrcSize - 4;
  unsigned char *pSrcByte=pSrc;
  unsigned char *pDstByte=pDst?pDst:pSrc;
  unsigned char ch2;
  unsigned char ch3;
  unsigned char ch4;

  while (pSrcByte <= pLast)
  {
    ch4=*pSrcByte++;
    ch3=*pSrcByte++;
    ch2=*pSrcByte++;
    *pDstByte++=*pSrcByte++;
    *pDstByte++=ch2;
    *pDstByte++=ch3;
    *pDstByte++=ch4;
  }
}

BOOL IsCodePageUnicode(int nCodePage)
{
  if (nCodePage == CP_UNICODE_UTF16LE ||
      nCodePage == CP_UNICODE_UTF16BE ||
      nCodePage == CP_UNICODE_UTF32LE ||
      nCodePage == CP_UNICODE_UTF32BE ||
      nCodePage == CP_UNICODE_UTF8)
  {
    return TRUE;
  }
  return FALSE;
}

const wchar_t* GetLangStringW(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_PROGRAM)
      return L"\x041F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x0430";
    if (nStringID == STRID_PLUGIN)
      return L"\x041F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_SCRIPT)
      return L"\x0421\x043A\x0440\x0438\x043F\x0442";
    if (nStringID == STRID_SCRIPTS)
      return L"\x0421\x043A\x0440\x0438\x043F\x0442\x044B";
    if (nStringID == STRID_COPIES)
      return L" (\x043A\x043E\x043F\x0438\x0438)";
    if (nStringID == STRID_LATEST)
      return L"\x041F\x043E\x0441\x043B\x0435\x0434\x043D\x044F\x044F";
    if (nStringID == STRID_CURRENT)
      return L"\x0422\x0435\x043A\x0443\x0449\x0430\x044F";
    if (nStringID == STRID_DESCRIPTION)
      return L"\x041E\x043F\x0438\x0441\x0430\x043D\x0438\x0435";
    if (nStringID == STRID_AUTHOR)
      return L"\x0410\x0432\x0442\x043E\x0440";
    if (nStringID == STRID_MIRROR)
      return L"\x0417\x0435\x0440\x043A\x0430\x043B\x043E";
    if (nStringID == STRID_LANGUAGE)
      return L"\x042F\x0437\x044B\x043A";
    if (nStringID == STRID_SELECT)
      return L"\x0423\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x0435\x0020\x0433\x0430\x043B\x043E\x0447\x043A\x0443\x0020\x0434\x043B\x044F\x0020\x0437\x0430\x0433\x0440\x0443\x0437\x043A\x0438\x002E";
    if (nStringID == STRID_ERRORNOTINLIST)
      return L"[\x043D\x0435\x0442\x0020\x0432\x0020\x0441\x043F\x0438\x0441\x043A\x0435]";
    if (nStringID == STRID_ERRORNOTPLUGIN)
      return L"[\x041D\x0415\x0020\x042F\x0412\x041B\x002E\x0020\x041F\x041B\x0410\x0413\x0418\x041D\x041E\x041C]";
    if (nStringID == STRID_ERRORCANTLOAD)
      return L"[\x041D\x0415\x0020\x0417\x0410\x0413\x0420\x0423\x0416\x0410\x0415\x0422\x0421\x042F]";
    if (nStringID == STRID_ERRORCOUNT)
      return L"\x041E\x0448\x0438\x0431\x043E\x043A";
    if (nStringID == STRID_UPDATE)
      return L"\x041E\x0431\x043D\x043E\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_PROGRAM)
      return L"Program";
    if (nStringID == STRID_PLUGIN)
      return L"Plugin";
    if (nStringID == STRID_SCRIPT)
      return L"Script";
    if (nStringID == STRID_SCRIPTS)
      return L"Scripts";
    if (nStringID == STRID_COPIES)
      return L" (copies)";
    if (nStringID == STRID_LATEST)
      return L"Latest";
    if (nStringID == STRID_CURRENT)
      return L"Current";
    if (nStringID == STRID_DESCRIPTION)
      return L"Description";
    if (nStringID == STRID_AUTHOR)
      return L"Author";
    if (nStringID == STRID_MIRROR)
      return L"Mirror";
    if (nStringID == STRID_LANGUAGE)
      return L"Language";
    if (nStringID == STRID_SELECT)
      return L"Select checkbox for download.";
    if (nStringID == STRID_ERRORCOUNT)
      return L"Errors";
    if (nStringID == STRID_ERRORNOTINLIST)
      return L"[not in list]";
    if (nStringID == STRID_ERRORNOTPLUGIN)
      return L"[NOT PLUGIN]";
    if (nStringID == STRID_ERRORCANTLOAD)
      return L"[CAN'T LOAD]";
    if (nStringID == STRID_UPDATE)
      return L"Update";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
  }
  return L"";
}

int getuservariable(const int varnum, wchar_t *str, int len)
{
  if (varnum >= 0 && varnum < __INST_LAST)
  {
    if (g_unicode)
    {
      len=xstrcpynW(str, g_variables + varnum*g_stringsize, len);
    }
    else
    {
      if (len=MultiByteToWideChar(CP_ACP, 0, ((char *)g_variables) + varnum*g_stringsize, -1, str, len))
        str[--len]=L'\0';
    }
    return len;
  }
  return -1;
}

void setuservariable(const int varnum, const wchar_t *var)
{
  if (var != NULL && varnum >= 0 && varnum < __INST_LAST)
  {
    if (g_unicode)
      xstrcpynW(g_variables + varnum*g_stringsize, var, NSIS_MAX_STRLEN);
    else
      WideCharToMultiByte(CP_ACP, 0, var, -1, ((char *)g_variables) + varnum*g_stringsize, NSIS_MAX_STRLEN, NULL, NULL);
  }
}

INT_PTR popintegerWide()
{
  wchar_t wszInt[32];

  wszInt[0]=L'\0';
  popstringWide(wszInt, 31);
  return xatoiW(wszInt, NULL);
}

void pushintegerWide(INT_PTR integer)
{
  wchar_t wszInt[32];

  xitoaW(integer, wszInt);
  pushstringWide(wszInt);
}

int popstringAnsi(char *str, int len)
{
  stack_t *th;

  if (g_stacktop && *g_stacktop)
  {
    th=(*g_stacktop);
    if (g_unicode)
    {
      if (len=WideCharToMultiByte(CP_ACP, 0, (const wchar_t *)th->text, -1, str, len, NULL, NULL))
        str[--len]='\0';
    }
    else
    {
      len=xstrcpynA(str, (const char *)th->text, len);
    }
    *g_stacktop=th->next;
    GlobalFree((HGLOBAL)th);
    return len;
  }
  return -1;
}

int popstringWide(wchar_t *str, int len)
{
  stack_t *th;

  if (g_stacktop && *g_stacktop)
  {
    th=(*g_stacktop);
    if (g_unicode)
    {
      len=xstrcpynW(str, th->text, len);
    }
    else
    {
      if (len=MultiByteToWideChar(CP_ACP, 0, (const char *)th->text, -1, str, len))
        str[--len]=L'\0';
    }
    *g_stacktop=th->next;
    GlobalFree((HGLOBAL)th);
    return len;
  }
  return -1;
}

void pushstringAnsi(const char *str)
{
  stack_t *th;

  if (g_stacktop)
  {
    if (g_unicode)
    {
      th=(stack_t *)GlobalAlloc(GPTR, sizeof(stack_t) + NSIS_MAX_STRLEN * sizeof(wchar_t));
      MultiByteToWideChar(CP_ACP, 0, str, -1, (wchar_t *)th->text, NSIS_MAX_STRLEN);
    }
    else
    {
      th=(stack_t *)GlobalAlloc(GPTR, sizeof(stack_t) + NSIS_MAX_STRLEN);
      xstrcpynA((char *)th->text, str, NSIS_MAX_STRLEN);
    }
    th->next=*g_stacktop;
    *g_stacktop=th;
  }
}

void pushstringWide(const wchar_t *str)
{
  stack_t *th;

  if (g_stacktop)
  {
    if (g_unicode)
    {
      th=(stack_t *)GlobalAlloc(GPTR, sizeof(stack_t) + NSIS_MAX_STRLEN * sizeof(wchar_t));
      xstrcpynW(th->text, str, NSIS_MAX_STRLEN);
    }
    else
    {
      th=(stack_t *)GlobalAlloc(GPTR, sizeof(stack_t) + NSIS_MAX_STRLEN);
      WideCharToMultiByte(CP_ACP, 0, str, -1, (char *)th->text, NSIS_MAX_STRLEN, NULL, NULL);
    }
    th->next=*g_stacktop;
    *g_stacktop=th;
  }
}

void Initialize(int string_size, wchar_t *variables, stack_t **stacktop, extra_parameters *extra)
{
  g_stacktop=stacktop;
  g_variables=variables;
  g_stringsize=string_size;
  g_pluginParms=extra;

  if (g_unicode == -1)
  {
    wchar_t wszCurPath[]=L"C:\\";

    g_pluginParms->validate_filename(wszCurPath);
    g_unicode=(wszCurPath[2] == L'\\')?FALSE:TRUE;

    //Initialize WideFunc.h header
    WideInitialize();
    WideGlobal_bOldWindows=!g_unicode;
  }
}
