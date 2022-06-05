/*****************************************************************
 *                 AkelUpdater NSIS plugin v6.8                  *
 *                                                               *
 * 2018 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *****************************************************************/

#define _WIN32_IE 0x0400
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "x64Func.h"
#include "ResizeFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
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

//Include wide functions
#define CallWindowProcWide
#define ComboBox_AddStringWide
#define ComboBox_GetLBTextWide
#define CreateFileWide
#define CreateProcessWide
#define DialogBoxWide
#define DirExistsWide
#define DispatchMessageWide
#define FileExistsWide
#define FindFirstFileWide
#define FindNextFileWide
#define GetClassNameWide
#define GetFileAttributesWide
#define GetModuleFileNameWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define ListView_GetColumnWide
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

//Include x64 functions
#define ReadFile64
#define GetFileSize64
#define SetFilePointer64
#define MultiByteToWideChar64
#define WideCharToMultiByte64
#include "x64Func.h"

//Include resize functions
#define ALLRESIZEFUNC
#include "ResizeFunc.h"
//*/

//Defines
#define NSIS_MAX_STRLEN 1024

//String IDs
#define STRID_AVAILABLE        0
#define STRID_PROGRAM          1
#define STRID_PLUGIN           2
#define STRID_SCRIPT           3
#define STRID_SCRIPTS          4
#define STRID_COPIES           5
#define STRID_LATEST           6
#define STRID_CURRENT          7
#define STRID_DESCRIPTION      8
#define STRID_AUTHOR           9
#define STRID_MIRROR           10
#define STRID_LANGUAGE         11
#define STRID_SELECT           12
#define STRID_ERRORNOTINLIST   13
#define STRID_ERRORNOTPLUGIN   14
#define STRID_ERRORCANTLOAD    15
#define STRID_ERRORCOUNT       16
#define STRID_UPDATE           17
#define STRID_CANCEL           18

#define AKDLL_SHOWWINDOW    (WM_USER + 100)
#define AKDLL_UPDATESTATUS  (WM_USER + 101)

//Strings
#define STR_PLUGSPACK    L"PlugsPack"
#define STR_PLUGSPACK64  L"PlugsPack64"

//Plugins list
#define LVI_NAME         0
#define LVI_LATEST       1
#define LVI_CURRENT      2
#define LVI_DESCRIPTION  3
#define LVI_AUTHOR       4

#define LT_MAIN    1
#define LT_SCRIPTS 2

#define CR_EQUAL           0
#define CR_NOTINSTALLED    1
#define CR_INSTALLEDOLDER  2
#define CR_INSTALLEDNEWER  3

#ifndef SCS_32BIT_BINARY
  #define SCS_32BIT_BINARY 0
#endif
#ifndef SCS_64BIT_BINARY
  #define SCS_64BIT_BINARY 6
#endif

//AkelUpdaterHelp action
#define AUH_GETDLLNAME   1

//DLLINFO.dwError
#define PE_NONE        0
#define PE_NOTINLIST   1
#define PE_NOTPLUGIN   2
#define PE_CANTLOAD    3

//FILEITEM.nType
#define FIT_AKELPAD   1
#define FIT_PLUGIN    2
#define FIT_SCRIPT    3
#define FIT_FORSCRIPT 4

typedef struct {
  wchar_t wszName[MAX_PATH];
  DWORD dwError;
} DLLINFO;

typedef struct _FILEREFITEM {
  struct _FILEREFITEM *next;
  struct _FILEREFITEM *prev;
  void *lpFileItem;
} FILEREFITEM;

typedef struct {
  FILEREFITEM *first;
  FILEREFITEM *last;
} STACKFILEREFITEM;

typedef struct _FILEITEM {
  struct _FILEITEM *next;
  struct _FILEITEM *prev;
  int nType;                     //See FIT_* defines.
  BOOL bPath64;
  int nCompare;
  int nChecked;
  DWORD dwError;
  wchar_t wszError[MAX_PATH];
  wchar_t wszPack[MAX_PATH];
  wchar_t wszName[MAX_PATH];
  wchar_t wszNameInList[MAX_PATH];
  wchar_t wszCurVer[MAX_PATH];
  wchar_t wszLastVer[MAX_PATH];
  wchar_t wszDescription[MAX_PATH];
  wchar_t wszAuthor[MAX_PATH];
  struct _FILEREFITEM *firstRefScript;
  struct _FILEREFITEM *lastRefScript;
  struct _FILEITEM *firstCopy;
  struct _FILEITEM *lastCopy;
} FILEITEM;

typedef struct {
  FILEITEM *first;
  FILEITEM *last;
} STACKFILEITEM;


//NSIS
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


//Scripts list columns
#define LVI_SCRIPTS_SCRIPT        0
#define LVI_SCRIPTS_HOTKEY        1
#define LVI_SCRIPTS_STATUS        2
#define LVI_SCRIPTS_VERSION       3
#define LVI_SCRIPTS_DESCRIPTION   4
#define LVI_SCRIPTS_AUTHOR        5
#define LVI_SCRIPTS_SITE          6

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

typedef struct {
  const wchar_t *wpKey;
  INT_PTR nKeyLen;
  wchar_t **wppValue;
  INT_PTR *lpnValueLen;
} CONTENTKEY;

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


//Functions prototypes
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
void ParseLst(HWND hDlg);
void CompareItems();
void FillItems(HWND hDlg, HWND hWndListExe, HWND hWndListDll, const wchar_t *wpFilter);
LPARAM GetItemParam(HWND hWnd, int nIndex);
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg);
int GetNextWord(const wchar_t *wpStr, INT_PTR nStrLen, const wchar_t *wpDelim, int nDelimLen, wchar_t *wszWord, int nWordMax, const wchar_t **wpNextWord);
void StackFilesFill(STACKFILEITEM *hStack, const wchar_t *wpPlugsDir, BOOL bPath64);
FILEREFITEM* StackFileRefInsert(STACKFILEREFITEM *hStack, FILEITEM *lpFileItem);
FILEITEM* StackFileInsert(STACKFILEITEM *hStack, const wchar_t *wpName);
FILEITEM* StackFileGet(STACKFILEITEM *hStack, const wchar_t *wpName, BOOL bPath64);
int StackFileSelCount(STACKFILEITEM *hStack);
void StackFilesSort(STACKFILEITEM *hStack);
int StackSort(FILEITEM **first, FILEITEM **last, int nUpDown);
void StackFilesFree(STACKFILEITEM *hStack);
LISTITEM* StackScriptGet(STACKLISTITEM *hStack, const wchar_t *wpScriptFileName);
void AkelUpdaterHelpExec(int nAction, const wchar_t *wpFile);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
int GetBaseName(const wchar_t *wpFile, int nFileLen, wchar_t *wszBaseName, int nBaseNameMax);
const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen);
int VersionCompare(const wchar_t *wpVer1, const wchar_t *wpVer2);
BOOL GetFileVersionA(const char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
BOOL GetFileVersionW(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
BOOL GetFileVersionWide(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
INT_PTR DetectAndReadFile(HANDLE *lphFile, const wchar_t *wpFile, DWORD dwFlags, int *lpnCodePage, BOOL *lpbBOM, wchar_t **wppContent, UINT_PTR dwBytesMax);
int AutodetectCodePage(HANDLE hFile, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
UINT_PTR ReadFileContent(HANDLE hFile, UINT_PTR dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wppContent);
void ChangeTwoBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst);
void ChangeFourBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst);
BOOL IsCodePageUnicode(int nCodePage);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);

void StackFillListItem(STACKLISTITEM *hStack, LISTCOLUMN *lpColumns);
LISTITEM* StackInsertListItem(STACKLISTITEM *hStack);
void StackFreeListItem(STACKLISTITEM *hStack);
int LangMatchRate(LANGID wCompareIt, LANGID wCompareWith);
DWORD GetColumnFlags(LISTCOLUMN *lpColumns);
INT_PTR CopyWideStr(const wchar_t *wpSrc, INT_PTR nSrcLen, wchar_t **wppDst);
BOOL FreeWideStr(wchar_t *wpWideStr);

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
wchar_t wszBuffer[NSIS_MAX_STRLEN];
wchar_t wszBuffer2[NSIS_MAX_STRLEN];
wchar_t wszExeFile[MAX_PATH];
int nExeFileLen;
wchar_t wszExeDir[MAX_PATH];
int nExeDirLen;
wchar_t wszAkelExe[MAX_PATH];
wchar_t wszAkelExe64[MAX_PATH];
wchar_t wszAkelDir[MAX_PATH];
wchar_t wszPlugsDir[MAX_PATH];
wchar_t wszPlugsDir64[MAX_PATH];
wchar_t wszScriptsDir[MAX_PATH];
wchar_t wszInputLanguage[MAX_PATH];
wchar_t wszInputVersion[32];
wchar_t wszNsisTempDir[MAX_PATH];
wchar_t wszInputHelper[MAX_PATH];
wchar_t wszAkelUpdaterVer[MAX_PATH]=L"";
wchar_t wszScriptsPack[MAX_PATH]=L"";
wchar_t wszFilter[MAX_PATH]=L"";
HINSTANCE hInstanceDLL=NULL;
HINSTANCE hInstanceEXE=NULL;
BOOL bOldWindows;
LANGID wLangModule;
STACKFILEITEM hFileStack={0};
STACKLISTITEM hScriptsStack={0};
FILEITEM *lpFileItemAkelPad=NULL;
FILEITEM *lpFileItemAkelPad64=NULL;
FILEITEM *lpFileItemScripts=NULL;
DLLINFO diGlobal;
HWND hWndDialog=NULL;
HWND hWndListDll=NULL;
WNDPROC lpOldFilterProc=NULL;
int nInputBit=32;
BOOL bScripts=FALSE;
BOOL bScriptsLstLoaded=FALSE;
BOOL bInputOnTop=FALSE;
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

    wLangModule=GetUserDefaultLangID();
    hInstanceEXE=GetModuleHandleA(NULL);
    nExeFileLen=GetModuleFileNameWide(hInstanceEXE, wszExeFile, MAX_PATH);
    nExeDirLen=GetFileDir(wszExeFile, nExeFileLen, wszExeDir, MAX_PATH);
    GetFileDir(wszExeDir, nExeDirLen, wszAkelDir, MAX_PATH);
    xprintfW(wszPlugsDir, L"%s\\Plugs", wszExeDir);
    xprintfW(wszScriptsDir, L"%s\\Scripts", wszPlugsDir);

    xprintfW(wszAkelExe, L"%s\\AkelPad.exe", wszAkelDir);
    if (!FileExistsWide(wszAkelExe))
    {
      xprintfW(wszAkelExe, L"%s\\notepad.exe", wszAkelDir);
      if (!FileExistsWide(wszAkelExe))
        wszAkelExe[0]=L'\0';
    }
    xprintfW(wszAkelExe64, L"%s\\AkelPad64.exe", wszAkelDir);
    if (!FileExistsWide(wszAkelExe64))
      wszAkelExe64[0]=L'\0';

    xprintfW(wszPlugsDir64, L"%s\\Plugs64", wszExeDir);
    if (!DirExistsWide(wszPlugsDir64))
      wszPlugsDir64[0]=L'\0';

    hKernel32=GetModuleHandleA("kernel32.dll");
    if (GetBinaryTypePtr=(BOOL (WINAPI *)(const wchar_t *, LPDWORD))GetProcAddress(hKernel32, "GetBinaryTypeW"))
      GetBinaryTypePtr(wszAkelExe, &dwBinaryType);
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
    bInputOnTop=popintegerWide();
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
    const wchar_t *wpArgument;

    popstringWide(wszBuffer, NSIS_MAX_STRLEN);
    wpArgument=wszBuffer;

    while (GetCommandLineArg(wpArgument, wszBuffer2, NSIS_MAX_STRLEN, &wpArgument))
    {
      pushstringWide(wszBuffer2);
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
  static HWND hWndLanguageLabel;
  static HWND hWndLanguage;
  static HWND hWndMirrorLabel;
  static HWND hWndMirror;
  static HWND hWndListCheckInfo;
  static HWND hWndListStatusInfo;
  static HWND hWndUpdate;
  static HWND hWndScripts;
  static HWND hWndScriptsFilter;
  static HWND hWndCancel;
  static LVITEMW lvi;
  static HICON hIconDlg;
  static BOOL bLockUpdateStatus;
  static RESIZEDIALOG rds[]={{&hWndLanguageLabel,  0, 0},
                             {&hWndLanguage,       0, 0},
                             {&hWndGroupExe,       RDS_SIZE|RDS_X, 0},
                             {&hWndListExe,        RDS_SIZE|RDS_X, 0},
                             {&hWndMirrorLabel,    RDS_MOVE|RDS_X, 0},
                             {&hWndMirror,         RDS_MOVE|RDS_X, 0},
                             {&hWndListDll,        RDS_SIZE|RDS_X, 0},
                             {&hWndListDll,        RDS_SIZE|RDS_Y, 0},
                             {&hWndListCheckInfo,  RDS_MOVE|RDS_Y, 0},
                             {&hWndListStatusInfo, RDS_MOVE|RDS_X, 0},
                             {&hWndListStatusInfo, RDS_MOVE|RDS_Y, 0},
                             {&hWndUpdate,         RDS_MOVE|RDS_Y, 0},
                             {&hWndScripts,        RDS_MOVE|RDS_Y, 0},
                             {&hWndScriptsFilter,  RDS_SIZE|RDS_X, 0},
                             {&hWndScriptsFilter,  RDS_MOVE|RDS_Y, 0},
                             {&hWndCancel,         RDS_MOVE|RDS_X, 0},
                             {&hWndCancel,         RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvc;

    hIconDlg=LoadIconA(hInstanceEXE, MAKEINTRESOURCEA(IDI_ICON));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIconDlg);
    hWndDialog=hDlg;
    hWndGroupExe=GetDlgItem(hDlg, IDC_GROUP_EXE);
    hWndListExe=GetDlgItem(hDlg, IDC_LIST_EXE);
    hWndLanguageLabel=GetDlgItem(hDlg, IDC_LANGUAGE_LABEL);
    hWndLanguage=GetDlgItem(hDlg, IDC_LANGUAGE);
    hWndMirrorLabel=GetDlgItem(hDlg, IDC_MIRROR_LABEL);
    hWndMirror=GetDlgItem(hDlg, IDC_MIRROR);
    hWndListDll=GetDlgItem(hDlg, IDC_LIST_DLL);
    hWndListCheckInfo=GetDlgItem(hDlg, IDC_LIST_CHECKINFO);
    hWndListStatusInfo=GetDlgItem(hDlg, IDC_LIST_STATUSINFO);
    hWndUpdate=GetDlgItem(hDlg, IDOK);
    hWndScripts=GetDlgItem(hDlg, IDC_SCRIPTS);
    hWndScriptsFilter=GetDlgItem(hDlg, IDC_SCRIPTS_FILTER);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
    SendMessage(hWndListExe, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES);
    SendMessage(hWndListDll, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_CHECKBOXES);

    xprintfW(wszBuffer, L"AkelUpdater %s", wszInputVersion);
    SetWindowTextWide(hDlg, wszBuffer);

    SetDlgItemTextWide(hDlg, IDC_MIRROR_LABEL, GetLangStringW(wLangModule, STRID_MIRROR));
    SetDlgItemTextWide(hDlg, IDC_LANGUAGE_LABEL, GetLangStringW(wLangModule, STRID_LANGUAGE));
    SetDlgItemTextWide(hDlg, IDC_LIST_CHECKINFO, GetLangStringW(wLangModule, STRID_SELECT));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_UPDATE));
    SetDlgItemTextWide(hDlg, IDC_SCRIPTS, GetLangStringW(wLangModule, STRID_SCRIPTS));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

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

    EnableWindow(hWndMirror, FALSE);
    EnableWindow(hWndUpdate, FALSE);

    //Columns EXE
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_PROGRAM);
    lvc.cx=188;
    lvc.iSubItem=LVI_NAME;
    ListView_InsertColumnWide(hWndListExe, LVI_NAME, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_LATEST);
    lvc.cx=105;
    lvc.iSubItem=LVI_LATEST;
    ListView_InsertColumnWide(hWndListExe, LVI_LATEST, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_CURRENT);
    lvc.cx=63;
    lvc.iSubItem=LVI_CURRENT;
    ListView_InsertColumnWide(hWndListExe, LVI_CURRENT, &lvc);

    //Columns DLL
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)L"";
    lvc.cx=210;
    lvc.iSubItem=LVI_NAME;
    ListView_InsertColumnWide(hWndListDll, LVI_NAME, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_LATEST);
    lvc.cx=83;
    lvc.iSubItem=LVI_LATEST;
    ListView_InsertColumnWide(hWndListDll, LVI_LATEST, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_CURRENT);
    lvc.cx=83;
    lvc.iSubItem=LVI_CURRENT;
    ListView_InsertColumnWide(hWndListDll, LVI_CURRENT, &lvc);

    //Parse "versions.lst"
    ParseLst(hDlg);
    if (lpFileItemScripts && lpFileItemScripts->wszCurVer[0])
      ShowWindow(hWndScripts, SW_SHOW);

    //Compare versions
    CompareItems();

    //Items
    FillItems(hDlg, hWndListExe, hWndListDll, wszFilter);

    if (bInputAuto)
    {
      if (StackFileSelCount(&hFileStack))
        PostMessage(hDlg, WM_COMMAND, IDOK, 0);
      else
        PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    }
    else
    {
      lpOldFilterProc=(WNDPROC)GetWindowLongPtrWide(hWndScriptsFilter, GWLP_WNDPROC);
      SetWindowLongPtrWide(hWndScriptsFilter, GWLP_WNDPROC, (UINT_PTR)NewFilterProc);

      PostMessage(hDlg, AKDLL_SHOWWINDOW, 0, 0);
      PostMessage(hDlg, AKDLL_UPDATESTATUS, 0, 0);
    }
    if (bInputOnTop)
    {
      SetWindowPos(hDlg, (HWND)(UINT_PTR)-1/*HWND_TOPMOST*/, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
    }
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
      int nSelProgram=0;
      int nErrorsCount=0;

      for (lpFileItem=hFileStack.first; lpFileItem; lpFileItem=lpFileItem->next)
      {
        if (lpFileItem->nType == FIT_FORSCRIPT) continue;

        if (lpFileItem->nCompare == CR_INSTALLEDNEWER ||
            lpFileItem->dwError == PE_NOTPLUGIN ||
            lpFileItem->dwError == PE_CANTLOAD)
        {
          ++nErrorsCount;
        }
        if (lpFileItem->nChecked > 0)
        {
          ++nSelCount;
          if (lpFileItem->nType == FIT_AKELPAD)
            ++nSelProgram;
        }
        ++nAllItemsCount;
      }
      if (nErrorsCount)
        xprintfW(wszBuffer2, L"(%s: %d)   ", GetLangStringW(wLangModule, STRID_ERRORCOUNT), nErrorsCount);
      else
        wszBuffer2[0]=L'\0';
      xprintfW(wszBuffer, L"%s%d / %d", wszBuffer2, nSelCount, nAllItemsCount);
      SetWindowTextWide(hWndListStatusInfo, wszBuffer);
      EnableWindow(hWndUpdate, nSelCount);
      EnableWindow(hWndMirror, nSelProgram);
    }
  }
  else if (uMsg == WM_COPYDATA)
  {
    COPYDATASTRUCT *cds=(COPYDATASTRUCT *)lParam;
    DLLINFO *di=(DLLINFO *)cds->lpData;

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
        if (pnmlv->iSubItem == LVI_NAME)
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
            lvi.iSubItem=LVI_NAME;
            lvi.state=((*lpbSelectAll + 1) << 12);
            lvi.stateMask=LVIS_STATEIMAGEMASK;
            ListView_SetItemWide(pnmlv->hdr.hwndFrom, &lvi);
          }
          *lpbSelectAll=!*lpbSelectAll;

          bLockUpdateStatus=FALSE;
          PostMessage(hDlg, AKDLL_UPDATESTATUS, 0, 0);
        }
        else
        {
          SendMessage(pnmlv->hdr.hwndFrom, LVM_SORTITEMS, (LPARAM)pnmlv->iSubItem, (LPARAM)CompareFunc);
        }
      }
      else if (pnmlv->hdr.code == (UINT)NM_DBLCLK)
      {
        NMITEMACTIVATE *pnmia=(NMITEMACTIVATE *)lParam;
        FILEITEM *lpFileItem;
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;

        //pnmia->lParam undefined so get it manually
        lpFileItem=(FILEITEM *)GetItemParam(pnmlv->hdr.hwndFrom, pnmia->iItem);

        if (lpFileItem && lpFileItem->nType == FIT_SCRIPT)
        {
          xmemset(&si, 0, sizeof(STARTUPINFOW));
          si.cb=sizeof(STARTUPINFOW);

          xprintfW(wszBuffer, L"\"%s\" \"%s\\%s\"", wszAkelExe, wszScriptsDir, lpFileItem->wszName);
          if (CreateProcessWide(NULL, wszBuffer, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
          {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
          }
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
          FILEREFITEM *lpRefCount;
          BOOL bNewState=((pnmlv->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          BOOL bOldState=((pnmlv->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            lpFileItem->nChecked=bNewState;

            //Change check of all referenced files
            for (lpFileCount=hFileStack.first; lpFileCount; lpFileCount=lpFileCount->next)
            {
              for (lpRefCount=lpFileCount->firstRefScript; lpRefCount; lpRefCount=lpRefCount->next)
              {
                if (lpRefCount->lpFileItem == lpFileItem)
                {
                  if (bNewState)
                    ++lpFileCount->nChecked;
                  else
                    --lpFileCount->nChecked;
                  break;
                }
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
            if (lpFileItem)
              nCompare=lpFileItem->nCompare;
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

          if (lplvcd->iSubItem == LVI_NAME ||
              lplvcd->iSubItem == LVI_LATEST)
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
        if (!bScriptsLstLoaded)
        {
          bScriptsLstLoaded=TRUE;
          pushstringWide(L"ini");
          pushstringWide(wszScriptsPack);
          if (lpDownloadScriptsProc >= 0)
            g_pluginParms->ExecuteCodeSegment(lpDownloadScriptsProc - 1, 0);
          ParseLst(hDlg);
          CompareItems();
        }
      }
      ShowWindow(hWndScriptsFilter, bScripts);
      FillItems(hDlg, NULL, hWndListDll, wszFilter);
      PostMessage(hDlg, AKDLL_UPDATESTATUS, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_SCRIPTS_FILTER)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        GetWindowTextWide(hWndScriptsFilter, wszFilter, MAX_PATH);
        FillItems(hDlg, NULL, hWndListDll, wszFilter);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      STACKFILEITEM hPackStack={0};
      FILEITEM *lpFileItem;
      FILEITEM *lpCopyItem;
      FILEITEM *lpPackItem;
      int nCountScript=0;
      int nCountDLL=0;
      int nCountDLL64=0;
      int nSelection;
      int nOffset;

      EnableWindow(hWndUpdate, FALSE);
      EnableWindow(hDlg, FALSE);

      for (lpFileItem=hFileStack.last; lpFileItem; lpFileItem=lpFileItem->prev)
      {
        if (lpFileItem->nChecked > 0)
        {
          if (lpFileItem->nType == FIT_SCRIPT || lpFileItem->nType == FIT_FORSCRIPT)
          {
            //Get checked pack
            if (!(lpPackItem=StackFileGet(&hPackStack, lpFileItem->wszPack, -1)))
              lpPackItem=StackFileInsert(&hPackStack, lpFileItem->wszPack);
            ++nCountScript;
          }
          else if (lpFileItem->nType == FIT_PLUGIN)
          {
            if (lpFileItem->bPath64)
              ++nCountDLL64;
            else
              ++nCountDLL;
          }

          //"Type|Pack|Path64|OrigName"
          nOffset=xprintfW(wszBuffer, L"%d|%s|%d|%s", lpFileItem->nType, lpFileItem->wszPack, lpFileItem->bPath64, lpFileItem->wszName);

          //Append copies and push them in format "Type|Pack|Path64|OrigName|CopyName1|CopyName2"
          for (lpCopyItem=lpFileItem->firstCopy; lpCopyItem; lpCopyItem=lpCopyItem->next)
          {
            nOffset+=xprintfW(wszBuffer + nOffset, L"|%s", lpCopyItem->wszName);
          }

          pushstringWide(wszBuffer);
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

      xprintfW(wszBuffer, L"%s|%s|%d|%d|%d", (lpFileItemAkelPad->nChecked > 0) ? lpFileItemAkelPad->wszLastVer : L"0",
                                             (lpFileItemAkelPad64 && lpFileItemAkelPad64->nChecked > 0) ? lpFileItemAkelPad64->wszLastVer : L"0",
                                             nCountDLL,
                                             nCountDLL64,
                                             nCountScript);
      setuservariable(INST_0, wszBuffer);

      wszBuffer[0]=L'\0';
      if ((nSelection=SendMessage(hWndMirror, CB_GETCURSEL, 0, 0)) != CB_ERR)
        ComboBox_GetLBTextWide(hWndMirror, nSelection, wszBuffer);
      setuservariable(INST_1, wszBuffer);

      wszBuffer[0]=L'\0';
      if ((nSelection=SendMessage(hWndLanguage, CB_GETCURSEL, 0, 0)) != CB_ERR)
        ComboBox_GetLBTextWide(hWndLanguage, nSelection, wszBuffer);
      setuservariable(INST_2, wszBuffer);

      EndDialog(hDlg, TRUE);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, FALSE);
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    StackFilesFree(&hFileStack);
    DestroyIcon(hIconDlg);
  }

  ResizeDialogMessages(&rds[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

LRESULT CALLBACK NewFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_DOWN || wParam == VK_UP)
    {
      if (GetFocus() != hWndListDll)
        SetFocus(hWndListDll);
      return SendMessage(hWndListDll, uMsg, wParam, lParam);
    }
  }
  return CallWindowProcWide(lpOldFilterProc, hWnd, uMsg, wParam, lParam);
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  FILEITEM *lpFileItem1=(FILEITEM *)lParam1;
  FILEITEM *lpFileItem2=(FILEITEM *)lParam2;

  if (lParamSort == LVI_NAME)
  {
    return xstrcmpiW(lpFileItem1->wszName, lpFileItem2->wszName);
  }
  if (lParamSort == LVI_LATEST)
  {
    return xstrcmpiW(lpFileItem1->wszLastVer, lpFileItem2->wszLastVer);
  }
  if (lParamSort == LVI_CURRENT)
  {
    return xstrcmpiW(lpFileItem1->wszCurVer, lpFileItem2->wszCurVer);
  }
  if (lParamSort == LVI_DESCRIPTION)
  {
    return xstrcmpiW(lpFileItem1->wszDescription, lpFileItem2->wszDescription);
  }
  if (lParamSort == LVI_AUTHOR)
  {
    return xstrcmpiW(lpFileItem1->wszAuthor, lpFileItem2->wszAuthor);
  }
  return 0;
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
  const wchar_t *wpCount;
  const wchar_t *wpLineEnd;
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
    wszAkelUpdaterVer[0]=L'\0';

    xprintfW(wszFile, L"%s\\versions.lst", wszNsisTempDir);
    wpContent=NULL;

    if (nContentLen=DetectAndReadFile(&hFile, wszFile, ADT_DETECTCODEPAGE|ADT_DETECTBOM, &nCodePage, &bBOM, &wpContent, (UINT_PTR)-1))
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
            else if (!wszAkelUpdaterVer[0] && !xstrcmpiW(wszName, L"$AkelUpdaterVer"))
            {
              xstrcpynW(wszAkelUpdaterVer, wszString, MAX_PATH);
              if (xstrcmpW(wszInputVersion, wszAkelUpdaterVer))
              {
                xprintfW(wszBuffer, L"AkelUpdater %s (%s %s)", wszInputVersion, GetLangStringW(wLangModule, STRID_AVAILABLE), wszAkelUpdaterVer);
                SetWindowTextWide(hDlg, wszBuffer);
              }
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

                //Trim "$"
                xstrcpynW(wszName, wszName + 1, MAX_PATH);

                if ((lpFileItemAkelPad && !xstrcmpnW(L"AkelPad", wszName, (UINT_PTR)-1)) ||
                    (nInputBit == 64 && xstrstrW(wszNoSupport64Bit, -1, wszName, nNameLen, FALSE, NULL, NULL)))
                {
                  //Skip line
                }
                else
                {
                  if (!(lpFileItem=StackFileGet(&hFileStack, wszName, -1)))
                    lpFileItem=StackFileInsert(&hFileStack, wszName);
                  if (lpFileItem)
                  {
                    xstrcpynW(lpFileItem->wszLastVer, wszString, MAX_PATH);

                    if (!lpFileItemAkelPad && !xstrcmpnW(L"AkelPad", wszName, (UINT_PTR)-1))
                    {
                      lpFileItem->nType=FIT_AKELPAD;
                      if (GetFileVersionWide(wszAkelExe, &nMajor, &nMinor, &nRelease, &nBuild, NULL))
                        xprintfW(lpFileItem->wszCurVer, L"%d.%d.%d", nMajor, nMinor, nRelease);
                      lpFileItemAkelPad=lpFileItem;

                      if (nMajor != 3 && wszAkelExe64[0])
                      {
                        if (lpFileItem=StackFileInsert(&hFileStack, wszName))
                        {
                          lpFileItem->nType=FIT_AKELPAD;
                          xstrcpynW(lpFileItem->wszLastVer, wszString, MAX_PATH);
                          xprintfW(lpFileItem->wszNameInList, L"%s[64]", wszName);
                          if (GetFileVersionWide(wszAkelExe64, &nMajor, &nMinor, &nRelease, &nBuild, NULL))
                            xprintfW(lpFileItem->wszCurVer, L"%d.%d.%d", nMajor, nMinor, nRelease);
                          lpFileItemAkelPad64=lpFileItem;
                        }
                      }
                    }
                    else
                    {
                      lpFileItem->nType=FIT_PLUGIN;
                      xstrcpynW(lpFileItem->wszPack, (nInputBit == 64 ? STR_PLUGSPACK64 : STR_PLUGSPACK), MAX_PATH);

                      if (wszPlugsDir64[0] && !xstrstrW(wszNoSupport64Bit, -1, wszName, nNameLen, FALSE, NULL, NULL))
                      {
                        if (lpFileItem=StackFileInsert(&hFileStack, wszName))
                        {
                          lpFileItem->nType=FIT_PLUGIN;
                          lpFileItem->bPath64=TRUE;
                          xstrcpynW(lpFileItem->wszLastVer, wszString, MAX_PATH);
                          xstrcpynW(lpFileItem->wszPack, STR_PLUGSPACK64, MAX_PATH);
                          xprintfW(lpFileItem->wszNameInList, L"%s[64]", wszName);
                        }
                      }
                      if (!lpFileItemScripts && !xstrcmpnW(L"Scripts", wszName, (UINT_PTR)-1))
                        lpFileItemScripts=lpFileItem;
                    }
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
    if (hFile) CloseHandle(hFile);

    //Add installed
    StackFilesFill(&hFileStack, wszPlugsDir, FALSE);
    if (wszPlugsDir64[0])
      StackFilesFill(&hFileStack, wszPlugsDir64, TRUE);
    StackFilesSort(&hFileStack);
  }

  //Parse wszScriptsPack: "KDJ|Infocatcher|Instructor|VladSh"
  if (bScripts && wszScriptsPack[0])
  {
    FILEITEM *lpMainScript;
    LISTITEM *lpScriptItem;
    const wchar_t *wpPack=wszScriptsPack;
    const wchar_t *wpMaxPack=wszScriptsPack + xstrlenW(wszScriptsPack);
    INT_PTR nVersionLen;
    BOOL bScriptsSection;
    LISTCOLUMN lpColumns[]={{LVI_SCRIPTS_SCRIPT,      163, LCF_VISIBLE},
                            {LVI_SCRIPTS_VERSION,     70,  LCF_VISIBLE|LCF_CONTENT},
                            {LVI_SCRIPTS_DESCRIPTION, 300, LCF_VISIBLE|LCF_CONTENT},
                            {LVI_SCRIPTS_AUTHOR,      70,  LCF_VISIBLE|LCF_CONTENT},
                            //{LVI_SCRIPTS_SITE,        70,  LCF_VISIBLE|LCF_CONTENT},
                            {-1, 0, 0}};

    //Fill hScriptsStack with current versions
    StackFillListItem(&hScriptsStack, lpColumns);

    while (GetNextWord(wpPack, wpMaxPack - wpPack, L"|", 1, wszPack, MAX_PATH, &wpPack))
    {
      xprintfW(wszFile, L"%s\\%s.ini", wszNsisTempDir, wszPack);
      wpContent=NULL;

      if (nContentLen=DetectAndReadFile(&hFile, wszFile, ADT_DETECTCODEPAGE|ADT_DETECTBOM, &nCodePage, &bBOM, &wpContent, (UINT_PTR)-1))
      {
        wpMaxContent=wpContent + nContentLen;
        wpCount=wpContent;
        bScriptsSection=FALSE;

        while (wpCount < wpMaxContent)
        {
          lpMainScript=NULL;
          wpLineEnd=wpCount;
          while (*wpLineEnd != L'\r' && *wpLineEnd != L'\n' && wpLineEnd < wpMaxContent) ++wpLineEnd;

          if (wpCount < wpLineEnd && *wpCount != L';')
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
                  if (!(lpMainScript=StackFileGet(&hFileStack, wszName, -1)))
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
                      if (!(lpFileItem=StackFileGet(&hFileStack, wszName, -1)))
                        lpFileItem=StackFileInsert(&hFileStack, wszName);
                      if (lpFileItem)
                      {
                        lpFileItem->nType=FIT_FORSCRIPT;
                        StackFileRefInsert((STACKFILEREFITEM *)&lpFileItem->firstRefScript, lpMainScript);
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
      if (hFile) CloseHandle(hFile);
    }
    StackFreeListItem(&hScriptsStack);

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
        xprintfW(lpFileItem->wszError, L"%s %s", lpFileItem->wszNameInList, GetLangStringW(wLangModule, STRID_ERRORNOTINLIST));
        lpFileItem->dwError=PE_NOTINLIST;
      }
      lpFileItem->nChecked=0;
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
  StackFilesSort(&hFileStack);
}

void FillItems(HWND hDlg, HWND hWndListExe, HWND hWndListDll, const wchar_t *wpFilter)
{
  FILEITEM *lpFileItem;
  FILEITEM *lpCopyItem;
  HWND hWndList;
  LVCOLUMNW lvc;
  LVITEMW lvi;
  int nIndex;
  int nOffset;
  const wchar_t *wpName;

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
      lvc.mask=LVCF_WIDTH;
      if (!ListView_GetColumnWide(hWndListDll, LVI_DESCRIPTION, &lvc))
      {
        lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
        lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_DESCRIPTION);
        lvc.cx=460;
        lvc.iSubItem=LVI_DESCRIPTION;
        ListView_InsertColumnWide(hWndListDll, LVI_DESCRIPTION, &lvc);
      }

      lvc.mask=LVCF_WIDTH;
      if (!ListView_GetColumnWide(hWndListDll, LVI_AUTHOR, &lvc))
      {
        lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
        lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_AUTHOR);
        lvc.cx=130;
        lvc.iSubItem=LVI_AUTHOR;
        ListView_InsertColumnWide(hWndListDll, LVI_AUTHOR, &lvc);
      }
    }
    else
    {
      SendMessage(hWndListDll, LVM_DELETECOLUMN, LVI_AUTHOR, 0);
      SendMessage(hWndListDll, LVM_DELETECOLUMN, LVI_DESCRIPTION, 0);
    }
    xprintfW(wszBuffer, L"%s%s", GetLangStringW(wLangModule, bScripts?STRID_SCRIPT:STRID_PLUGIN), (!bScripts && !bInputNoCopies)?GetLangStringW(wLangModule, STRID_COPIES):NULL);
    lvc.mask=LVCF_TEXT;
    lvc.pszText=(wchar_t *)wszBuffer;
    ListView_SetColumnWide(hWndListDll, LVI_NAME, &lvc);

    SendMessage(hWndListDll, WM_SETREDRAW, FALSE, 0);
    SendMessage(hWndListDll, LVM_DELETEALLITEMS, 0, 0);
    if (bScripts)
      bSelectAllJs=TRUE;
    else
      bSelectAllDll=TRUE;
  }

  for (lpFileItem=hFileStack.first; lpFileItem; lpFileItem=lpFileItem->next)
  {
    if (lpFileItem == lpFileItemAkelPad ||
        lpFileItem == lpFileItemAkelPad64)
    {
      if (!hWndListExe) continue;
      hWndList=hWndListExe;
    }
    else
    {
      if (!hWndListDll) continue;
      if (lpFileItem->nType == FIT_FORSCRIPT) continue;
      if ((lpFileItem->nType == FIT_SCRIPT && !bScripts) ||
          (lpFileItem->nType == FIT_PLUGIN && bScripts))
          continue;
      hWndList=hWndListDll;
    }

    if (lpFileItem->wszNameInList[0])
      wpName=lpFileItem->wszNameInList;
    else
      wpName=lpFileItem->wszName;

    if (hWndList == hWndListExe)
    {
      if (nInputBit == 64 || lpFileItem == lpFileItemAkelPad64)
        xprintfW(wszBuffer, L"%s (x64)", wpName);
      else
        xprintfW(wszBuffer, L"%s (x86)", wpName);
    }
    else if (hWndList == hWndListDll)
    {
      //Filter check (with contents)
      if (bScripts && wpFilter && *wpFilter)
      {
        if (!xstrstrW(lpFileItem->wszName, -1, wpFilter, -1, FALSE, NULL, NULL) &&
            !xstrstrW(lpFileItem->wszCurVer, -1, wpFilter, -1, FALSE, NULL, NULL) &&
            !xstrstrW(lpFileItem->wszLastVer, -1, wpFilter, -1, FALSE, NULL, NULL) &&
            !xstrstrW(lpFileItem->wszDescription, -1, wpFilter, -1, FALSE, NULL, NULL) &&
            !xstrstrW(lpFileItem->wszAuthor, -1, wpFilter, -1, FALSE, NULL, NULL))
        {
          continue;
        }
      }

      nOffset=xstrcpynW(wszBuffer, wpName, MAX_PATH);

      //Append copies
      if (lpCopyItem=lpFileItem->firstCopy)
      {
        nOffset+=xprintfW(wszBuffer + nOffset, L" (%s", lpCopyItem->wszName);

        for (lpCopyItem=lpCopyItem->next; lpCopyItem; lpCopyItem=lpCopyItem->next)
        {
          nOffset+=xprintfW(wszBuffer + nOffset, L", %s", lpCopyItem->wszName);
        }
        xprintfW(wszBuffer + nOffset, L")");
      }
    }

    lvi.mask=LVIF_TEXT|LVIF_PARAM;
    lvi.pszText=lpFileItem->dwError?lpFileItem->wszError:wszBuffer;
    lvi.iItem=0x7FFFFFFF;
    lvi.iSubItem=LVI_NAME;
    lvi.lParam=(LPARAM)lpFileItem;
    nIndex=ListView_InsertItemWide(hWndList, &lvi);

    lvi.mask=LVIF_STATE;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVI_NAME;
    lvi.state=((lpFileItem->nChecked + 1) << 12);
    lvi.stateMask=LVIS_STATEIMAGEMASK;
    ListView_SetItemWide(hWndList, &lvi);

    lvi.mask=LVIF_TEXT;
    lvi.pszText=lpFileItem->wszLastVer;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVI_LATEST;
    ListView_SetItemWide(hWndList, &lvi);

    lvi.mask=LVIF_TEXT;
    lvi.pszText=lpFileItem->wszCurVer;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVI_CURRENT;
    ListView_SetItemWide(hWndList, &lvi);

    if (bScripts)
    {
      lvi.mask=LVIF_TEXT;
      lvi.pszText=lpFileItem->wszDescription;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVI_DESCRIPTION;
      ListView_SetItemWide(hWndList, &lvi);

      lvi.mask=LVIF_TEXT;
      lvi.pszText=lpFileItem->wszAuthor;
      lvi.iItem=nIndex;
      lvi.iSubItem=LVI_AUTHOR;
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

int GetNextWord(const wchar_t *wpStr, INT_PTR nStrLen, const wchar_t *wpDelim, int nDelimLen, wchar_t *wszWord, int nWordMax, const wchar_t **wpNextWord)
{
  const wchar_t *wpMatchBegin;
  const wchar_t *wpMatchEnd;

  xstrstrW(wpStr, nStrLen, wpDelim, nDelimLen, TRUE, &wpMatchBegin, &wpMatchEnd);
  if (wpNextWord) *wpNextWord=wpMatchEnd;
  return xstrcpynW(wszWord, wpStr, min(wpMatchBegin - wpStr + 1, nWordMax));
}

void StackFilesFill(STACKFILEITEM *hStack, const wchar_t *wpPlugsDir, BOOL bPath64)
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

  xprintfW(wszSearchDir, L"%s\\*.%s", wpPlugsDir, pDllExt);

  if ((hSearch=FindFirstFileWide(wszSearchDir, &wfd)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      //Avoid FindFirstFile/FindNextFile bug: "*.dll_ANYSYMBOLS" is also matched
      if (xstrcmpiW(pDllExt, GetFileExt(wfd.cFileName, -1)))
        continue;

      //Get real name in diGlobal.wszName
      xprintfW(wszFile, L"%s\\%s", wpPlugsDir, wfd.cFileName);
      diGlobal.wszName[0]=L'\0';
      diGlobal.dwError=PE_CANTLOAD;

      if (bInputNoCopies)
      {
        GetBaseName(wfd.cFileName, -1, diGlobal.wszName, MAX_PATH);
        diGlobal.dwError=PE_NONE;
      }
      else
      {
        if (nInputBit == 64 || bPath64)
        {
          //Receive diGlobal via WM_COPYDATA of hWndDialog
          AkelUpdaterHelpExec(AUH_GETDLLNAME, wszFile);
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
        if (!(lpFileItem=StackFileGet(hStack, diGlobal.wszName, bPath64)))
        {
          //Non-standard plugin
          if (lpFileItem=StackFileInsert(hStack, diGlobal.wszName))
          {
            lpFileItem->nType=FIT_PLUGIN;
            lpFileItem->bPath64=bPath64;
            //xstrcpynW(lpFileItem->wszPack, (nInputBit == 64 ? STR_PLUGSPACK64 : STR_PLUGSPACK), MAX_PATH);
            if (bPath64)
              xprintfW(lpFileItem->wszNameInList, L"%s[64]", diGlobal.wszName);
            else
              xprintfW(lpFileItem->wszNameInList, L"%s", diGlobal.wszName);
          }
        }

        if (lpFileItem)
        {
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
          if (bPath64)
            xprintfW(lpFileItem->wszNameInList, L"%s[64]", lpFileItem->wszName);
          else
            xprintfW(lpFileItem->wszNameInList, L"%s", lpFileItem->wszName);

          if (diGlobal.dwError == PE_NOTPLUGIN)
            xprintfW(lpFileItem->wszError, L"%s %s", lpFileItem->wszNameInList, GetLangStringW(wLangModule, STRID_ERRORNOTPLUGIN));
          else if (diGlobal.dwError == PE_CANTLOAD)
            xprintfW(lpFileItem->wszError, L"%s %s", lpFileItem->wszNameInList,  GetLangStringW(wLangModule, STRID_ERRORCANTLOAD));
          else
            xprintfW(lpFileItem->wszError, L"%s", lpFileItem->wszNameInList);
          lpFileItem->nType=FIT_PLUGIN;
          lpFileItem->dwError=diGlobal.dwError;
        }
      }
    }
    while (FindNextFileWide(hSearch, &wfd));

    FindClose(hSearch);
  }
}

FILEREFITEM* StackFileRefInsert(STACKFILEREFITEM *hStack, FILEITEM *lpFileItem)
{
  FILEREFITEM *lpElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(FILEREFITEM)))
  {
    lpElement->lpFileItem=lpFileItem;
  }
  return lpElement;
}

FILEITEM* StackFileInsert(STACKFILEITEM *hStack, const wchar_t *wpName)
{
  FILEITEM *lpElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(FILEITEM)))
  {
    xstrcpynW(lpElement->wszName, wpName, MAX_PATH);
    lpElement->nChecked=-1;
  }
  return lpElement;
}

FILEITEM* StackFileGet(STACKFILEITEM *hStack, const wchar_t *wpName, BOOL bPath64)
{
  FILEITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(lpElement->wszName, wpName) &&
        (bPath64 == -1 || lpElement->bPath64 == bPath64))
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
    if (lpElement->nType == FIT_FORSCRIPT) continue;

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
      if (tmp2->dwError == tmp1->dwError)
      {
        if (tmp2->nCompare == tmp1->nCompare)
          i=xstrcmpiW(tmp2->wszName, tmp1->wszName);
        else
          i=tmp2->nCompare > tmp1->nCompare?-1:1;
      }
      else i=tmp2->dwError > tmp1->dwError?-1:1;

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
    StackClear((stack **)&lpElement->firstRefScript, (stack **)&lpElement->lastRefScript);
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

void AkelUpdaterHelpExec(int nAction, const wchar_t *wpFile)
{
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;

  xprintfW(wszBuffer, L"\"%s\\%s\" %d \"%Id\" \"%s\"", wszNsisTempDir, wszInputHelper, nAction, hWndDialog, wpFile);
  xmemset(&si, 0, sizeof(STARTUPINFOW));
  si.cb=sizeof(STARTUPINFOW);

  if (CreateProcessWide(NULL, wszBuffer, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
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

INT_PTR DetectAndReadFile(HANDLE *lphFile, const wchar_t *wpFile, DWORD dwFlags, int *lpnCodePage, BOOL *lpbBOM, wchar_t **wppContent, UINT_PTR dwBytesMax)
{
  HANDLE hFile;
  INT_PTR nContentLen=0;

  if (lphFile && !wpFile)
    hFile=*lphFile;
  else
    hFile=NULL;
  if (!hFile)
    hFile=CreateFileWide(wpFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (hFile && hFile != INVALID_HANDLE_VALUE)
  {
    if (dwFlags & (ADT_DETECTCODEPAGE|ADT_DETECTBOM))
    {
      if (AutodetectCodePage(hFile, dwFlags, lpnCodePage, lpbBOM) == EDT_SUCCESS)
        SetFilePointer64(hFile, 0, FILE_BEGIN);
    }
    nContentLen=ReadFileContent(hFile, dwBytesMax, *lpnCodePage, *lpbBOM, wppContent);
    if (!lphFile) CloseHandle(hFile);
  }
  if (lphFile) *lphFile=hFile;
  return nContentLen;
}

int AutodetectCodePage(HANDLE hFile, DWORD dwFlags, int *nCodePage, BOOL *bBOM)
{
  BYTE wszFirstBytes[4];
  UINT_PTR dwBytesRead=0;
  int nResult=EDT_SUCCESS;

  if (hFile && hFile != INVALID_HANDLE_VALUE)
  {
    if (ReadFile64(hFile, wszFirstBytes, sizeof(wszFirstBytes), &dwBytesRead, NULL))
    {
      if (dwBytesRead >= 4)
      {
        if (wszFirstBytes[0] == 0xFF && wszFirstBytes[1] == 0xFE && wszFirstBytes[2] == 0x00 && wszFirstBytes[3] == 0x00)
        {
          if (dwFlags & ADT_DETECTCODEPAGE) *nCodePage=CP_UNICODE_UTF32LE;
          if (dwFlags & ADT_DETECTBOM) *bBOM=TRUE;
          goto End;
        }
        else if (wszFirstBytes[0] == 0x00 && wszFirstBytes[1] == 0x00 && wszFirstBytes[2] == 0xFE && wszFirstBytes[3] == 0xFF)
        {
          if (dwFlags & ADT_DETECTCODEPAGE) *nCodePage=CP_UNICODE_UTF32BE;
          if (dwFlags & ADT_DETECTBOM) *bBOM=TRUE;
          goto End;
        }
      }
      if (dwBytesRead >= 2)
      {
        if (wszFirstBytes[0] == 0xFF && wszFirstBytes[1] == 0xFE)
        {
          if (dwFlags & ADT_DETECTCODEPAGE) *nCodePage=CP_UNICODE_UTF16LE;
          if (dwFlags & ADT_DETECTBOM) *bBOM=TRUE;
          goto End;
        }
        else if (wszFirstBytes[0] == 0xFE && wszFirstBytes[1] == 0xFF)
        {
          if (dwFlags & ADT_DETECTCODEPAGE) *nCodePage=CP_UNICODE_UTF16BE;
          if (dwFlags & ADT_DETECTBOM) *bBOM=TRUE;
          goto End;
        }
      }
      if (dwBytesRead >= 3)
      {
        if (wszFirstBytes[0] == 0xEF && wszFirstBytes[1] == 0xBB && wszFirstBytes[2] == 0xBF)
        {
          if (dwFlags & ADT_DETECTCODEPAGE) *nCodePage=CP_UNICODE_UTF8;
          if (dwFlags & ADT_DETECTBOM) *bBOM=TRUE;
          goto End;
        }
      }
    }
    else nResult=EDT_READ;
  }
  else nResult=EDT_OPEN;

  //Default
  if (dwFlags & ADT_DETECTCODEPAGE) *nCodePage=GetACP();
  if (dwFlags & ADT_DETECTBOM) *bBOM=FALSE;

  End:
  return nResult;
}

UINT_PTR ReadFileContent(HANDLE hFile, UINT_PTR dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wppContent)
{
  unsigned char *szContent;
  wchar_t *wszContent=*wppContent;
  UINT_PTR dwContentBytes;
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
      dwContentBytes=dwBytesMax;
    else
      dwContentBytes=dwBytesMax * sizeof(wchar_t);

    if (!wszContent)
      wszContent=(wchar_t *)GlobalAlloc(GMEM_FIXED, dwContentBytes + sizeof(wchar_t));
    if (wszContent)
    {
      if (IsCodePageUnicode(nCodePage) && nCodePage != CP_UNICODE_UTF8)
        szContent=(unsigned char *)wszContent;
      else
        szContent=(unsigned char *)wszContent + dwBytesMax;

      //Read data from file
      if (ReadFile64(hFile, szContent, dwBytesMax, &dwBytesRead, NULL) && dwBytesRead)
      {
        //Translate data to UNICODE
        if (nCodePage == CP_UNICODE_UTF16LE)
        {
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (nCodePage == CP_UNICODE_UTF16BE)
        {
          ChangeTwoBytesOrder(szContent, dwBytesRead, NULL);
          dwCharsConverted=dwBytesRead / sizeof(wchar_t);
        }
        else if (nCodePage == CP_UNICODE_UTF32LE)
        {
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szContent, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszContent, dwContentBytes / sizeof(wchar_t));
        }
        else if (nCodePage == CP_UNICODE_UTF32BE)
        {
          ChangeFourBytesOrder(szContent, dwBytesRead, NULL);
          dwCharsConverted=UTF32toUTF16((const unsigned long *)szContent, dwBytesRead / sizeof(unsigned long), NULL, (unsigned short *)wszContent, dwContentBytes / sizeof(wchar_t));
        }
        else
        {
          if (nCodePage == CP_UNICODE_UTF8)
            dwCharsConverted=UTF8toUTF16(szContent, dwBytesRead, NULL, (unsigned short *)wszContent, dwContentBytes / sizeof(wchar_t));
          else
            dwCharsConverted=MultiByteToWideChar64(nCodePage, 0, (char *)szContent, dwBytesRead, wszContent, dwContentBytes / sizeof(wchar_t));
        }
      }
      wszContent[dwCharsConverted]=L'\0';
    }
  }
  *wppContent=wszContent;
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
  wLangID=PRIMARYLANGID(wLangID);

  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_AVAILABLE)
      return L"\x0434\x043E\x0441\x0442\x0443\x043F\x043D\x0430\x0020\x0432\x0435\x0440\x0441\x0438\x044F";
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
    if (nStringID == STRID_AVAILABLE)
      return L"available version";
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


//Adaptation from Scripts.c
void StackFillListItem(STACKLISTITEM *hStack, LISTCOLUMN *lpColumns)
{
  LISTITEM *lpListItem;
  WIN32_FIND_DATAW wfd;
  HANDLE hFind;
  HANDLE hFile;
  wchar_t wszFile[MAX_PATH];
  const wchar_t *wpExt;
  wchar_t *wpCount;
  wchar_t *wpMaxCount;
  wchar_t *wpValue;
  wchar_t *wpValueEnd;
  wchar_t *wpLineEnd;
  wchar_t *wszContentBuffer=NULL;
  wchar_t *wpContent=NULL;
  wchar_t *wpMaxContent=NULL;
  static wchar_t *wpVersion;
  static wchar_t *wpDescription;
  static wchar_t *wpAuthor;
  static wchar_t *wpSite;
  const wchar_t *wpComment;
  INT_PTR nContentLen;
  INT_PTR nBytesToRead;
  static INT_PTR nVersionLen;
  static INT_PTR nDescriptionLen;
  static INT_PTR nAuthorLen;
  static INT_PTR nSiteLen;
  DWORD dwAllColumns;
  DWORD dwContentColumns;
  int nCommentLen;
  LANGID wLang;
  LANGID wDescriptionLang;
  int nCodePage;
  BOOL bBOM;
  CONTENTKEY *ckCount;
  CONTENTKEY ck[]={{NULL, 0, &wpDescription, &nDescriptionLen},
                   {L"***", 3, &wpDescription, &nDescriptionLen},
                   {L"Description(", 12, &wpDescription, &nDescriptionLen},
                   {L"Version:", 8, &wpVersion, &nVersionLen},
                   {L"Author:", 7, &wpAuthor, &nAuthorLen},
                   {L"http://akelpad.sourceforge.net/", 31, &wpSite, &nSiteLen},
                   {0,0,0}};

  //Clear stack
  StackFreeListItem(hStack);

  //Find columns
  dwAllColumns=GetColumnFlags(lpColumns);
  dwContentColumns=dwAllColumns & (LCN_VERSION|LCN_DESCRIPTION|LCN_AUTHOR|LCN_SITE);

  //Content buffer
  if (dwContentColumns)
  {
    wszContentBuffer=(wchar_t *)GlobalAlloc(GPTR, (65536 + 1) * sizeof(wchar_t));
    wpMaxContent=wszContentBuffer + 65536;
    nBytesToRead=512;
  }

  //Script files
  xprintfW(wszFile, L"%s\\*.*", wszScriptsDir);

  if ((hFind=FindFirstFileWide(wszFile, &wfd)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      if (wfd.cFileName[0] == '.' && (wfd.cFileName[1] == '\0' || (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0'))) continue;

      if (!(wpExt=GetFileExt(wfd.cFileName, -1)))
        continue;
      if (!xstrcmpiW(wpExt, L"js"))
      {
        wpComment=L"//";
        nCommentLen=2;
      }
      else if (!xstrcmpiW(wpExt, L"vbs"))
      {
        wpComment=L"'";
        nCommentLen=1;
      }
      else continue;

      //Script content
      hFile=NULL;
      nCodePage=0;
      bBOM=FALSE;
      ck[0].wpKey=wpComment;
      ck[0].nKeyLen=nCommentLen;
      wpContent=wszContentBuffer;
      wpVersion=NULL;
      wpDescription=NULL;
      wpAuthor=NULL;
      wpSite=NULL;
      wDescriptionLang=0;

      if (dwContentColumns)
      {
        xprintfW(wszFile, L"%s\\%s", wszScriptsDir, wfd.cFileName);

        if (nContentLen=DetectAndReadFile(&hFile, wszFile, ADT_DETECTCODEPAGE|ADT_DETECTBOM, &nCodePage, &bBOM, &wpContent, (UINT_PTR)nBytesToRead))
        {
          wpCount=wpContent;
          wpMaxCount=wpContent + nContentLen;

          while (nBytesToRead == -1 ? wpCount < wpMaxCount : nContentLen)
          {
            wpLineEnd=wpCount;

            for (;;)
            {
              //Make sure we have all line in buffer
              while (*wpLineEnd != L'\r' && *wpLineEnd != L'\n' && *wpLineEnd != L'\0') ++wpLineEnd;
              if (!*wpLineEnd && nBytesToRead != -1)
              {
                wpContent=wpMaxCount;
                if (wpContent + nBytesToRead <= wpMaxContent)
                {
                  //Read next block to wszContentBuffer
                  if (nContentLen=DetectAndReadFile(&hFile, NULL, 0, &nCodePage, &bBOM, &wpContent, (UINT_PTR)nBytesToRead))
                  {
                    //Now wpCount could point to "\r|\n"
                    while (*wpCount == L'\r' || *wpCount == L'\n') ++wpCount;
                    wpMaxCount=wpContent + nContentLen;
                    continue;
                  }
                }
              }
              break;
            }

            //Skip spaces at line start
            while (*wpCount == L' ' || *wpCount == L'\t') ++wpCount;

            //Line must start with comment
            if (xstrcmpinW(wpComment, wpCount, (UINT_PTR)-1))
              break;
            wpCount+=nCommentLen;

            //Skip spaces
            while (*wpCount == L' ' || *wpCount == L'\t') ++wpCount;

            if (*wpCount != L'\r' && *wpCount != L'\n')
            {
              for (ckCount=ck; ckCount->wpKey; ++ckCount)
              {
                wpValue=*ckCount->wppValue;

                if ((!wpValue || (ckCount->wppValue == &wpDescription && wDescriptionLang != wLangModule)) &&
                    !xstrcmpinW(ckCount->wpKey, wpCount, (UINT_PTR)-1))
                {
                  if (ckCount->wppValue != &wpSite)
                  {
                    wpCount+=ckCount->nKeyLen;
                    while (*wpCount == L' ' || *wpCount == L'\t') ++wpCount;
                  }
                  wpValue=wpCount;
                  wpValueEnd=wpLineEnd;
                  wpCount=wpLineEnd;

                  //Trim trailing spaces
                  while (wpValueEnd > wpValue && (*(wpValueEnd - 1) == L' ' || *(wpValueEnd - 1) == L'\t'))
                    --wpValueEnd;
                  if (ckCount->wppValue == &wpDescription)
                  {
                    if (ckCount->wpKey[ckCount->nKeyLen - 1] == L'(')
                    {
                      wLang=(WORD)xatoiW(wpValue, (const wchar_t **)&wpValue);
                      if (LangMatchRate(wLang, wLangModule) <= LangMatchRate(wDescriptionLang, wLangModule))
                        break;
                      wDescriptionLang=wLang;

                      if (!xstrcmpinW(L"):", wpValue, (UINT_PTR)-1))
                        wpValue+=2;
                      while (*wpValue == L' ' || *wpValue == L'\t') ++wpValue;
                    }
                    else
                    {
                      if (wDescriptionLang) break;
                      wDescriptionLang=wLangModule;
                    }

                    //*** Description *** -> Description
                    if (wpValueEnd - ckCount->nKeyLen >= wpValue && !xstrcmpinW(ckCount->wpKey, wpValueEnd - ckCount->nKeyLen, (UINT_PTR)-1))
                      wpValueEnd-=ckCount->nKeyLen;
                    //Trim trailing spaces
                    while (wpValueEnd > wpValue && (*(wpValueEnd - 1) == L' ' || *(wpValueEnd - 1) == L'\t'))
                      --wpValueEnd;
                  }
                  *ckCount->wppValue=wpValue;
                  *ckCount->lpnValueLen=wpValueEnd - wpValue;
                  break;
                }
              }
              wpCount=wpLineEnd;
            }
            while (*wpCount == L'\r' || *wpCount == L'\n') ++wpCount;

            //Break if all info is received
            if ((!(dwContentColumns & LCN_VERSION) || wpVersion) &&
                (!(dwContentColumns & LCN_DESCRIPTION) || (wpDescription && wDescriptionLang == wLangModule)) &&
                (!(dwContentColumns & LCN_AUTHOR) || wpAuthor) &&
                (!(dwContentColumns & LCN_SITE) || wpSite))
              break;
          }
        }
        if (hFile) CloseHandle(hFile);
      }

      if (lpListItem=StackInsertListItem(hStack))
      {
        if (dwAllColumns & LCN_SCRIPT)
          CopyWideStr(wfd.cFileName, -1, &lpListItem->wpScript);
        if (wpVersion)
          CopyWideStr(wpVersion, nVersionLen, &lpListItem->wpVersion);
        if (wpDescription)
          CopyWideStr(wpDescription, nDescriptionLen, &lpListItem->wpDescription);
        if (wpAuthor)
          CopyWideStr(wpAuthor, nAuthorLen, &lpListItem->wpAuthor);
        if (wpSite)
          CopyWideStr(wpSite, nSiteLen, &lpListItem->wpSite);
      }
    }
    while (FindNextFileWide(hFind, &wfd));

    FindClose(hFind);
  }
  if (wszContentBuffer) GlobalFree((HGLOBAL)wszContentBuffer);
}

LISTITEM* StackInsertListItem(STACKLISTITEM *hStack)
{
  LISTITEM *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(LISTITEM)))
    ++hStack->nElements;
  return lpElement;
}

void StackFreeListItem(STACKLISTITEM *hStack)
{
  LISTITEM *lpListItem;

  for (lpListItem=hStack->first; lpListItem; lpListItem=lpListItem->next)
  {
    FreeWideStr(lpListItem->wpScript);
    FreeWideStr(lpListItem->wpHotkey);
    FreeWideStr(lpListItem->wpStatus);
    FreeWideStr(lpListItem->wpVersion);
    FreeWideStr(lpListItem->wpDescription);
    FreeWideStr(lpListItem->wpAuthor);
    FreeWideStr(lpListItem->wpSite);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->lpLastScript=NULL;
  hStack->nElements=0;
}

int LangMatchRate(LANGID wCompareIt, LANGID wCompareWith)
{
  int nRate=0;

  if (wCompareIt == wCompareWith)
    nRate=3;
  else if (PRIMARYLANGID(wCompareIt) == PRIMARYLANGID(wCompareWith))
    nRate=2;
  else if (PRIMARYLANGID(wCompareIt) == LANG_ENGLISH)
    nRate=1;
  return nRate;
}

DWORD GetColumnFlags(LISTCOLUMN *lpColumns)
{
  LISTCOLUMN *lpColumnCount;
  DWORD dwColumns=0;

  for (lpColumnCount=lpColumns; lpColumnCount->nID >= 0; ++lpColumnCount)
  {
    if (lpColumnCount->dwFlags & LCF_VISIBLE)
    {
      if (lpColumnCount->nID == LVI_SCRIPTS_SCRIPT)
        dwColumns|=LCN_SCRIPT;
      else if (lpColumnCount->nID == LVI_SCRIPTS_HOTKEY)
        dwColumns|=LCN_HOTKEY;
      else if (lpColumnCount->nID == LVI_SCRIPTS_STATUS)
        dwColumns|=LCN_STATUS;
      else if (lpColumnCount->nID == LVI_SCRIPTS_VERSION)
        dwColumns|=LCN_VERSION;
      else if (lpColumnCount->nID == LVI_SCRIPTS_DESCRIPTION)
        dwColumns|=LCN_DESCRIPTION;
      else if (lpColumnCount->nID == LVI_SCRIPTS_AUTHOR)
        dwColumns|=LCN_AUTHOR;
      else if (lpColumnCount->nID == LVI_SCRIPTS_SITE)
        dwColumns|=LCN_SITE;
    }
  }
  return dwColumns;
}

INT_PTR CopyWideStr(const wchar_t *wpSrc, INT_PTR nSrcLen, wchar_t **wppDst)
{
  wchar_t *wszDst=*wppDst;

  if (nSrcLen == -1)
    nSrcLen=xstrlenW(wpSrc);
  if (wszDst)
    FreeWideStr(wszDst);
  if (wszDst=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nSrcLen + 1) * sizeof(wchar_t)))
    xstrcpynW(wszDst, wpSrc, nSrcLen + 1);
  *wppDst=wszDst;
  return nSrcLen;
}

BOOL FreeWideStr(wchar_t *wpWideStr)
{
  if (wpWideStr && GlobalFree((HGLOBAL)wpWideStr))
    return TRUE;
  return FALSE;
}

//NSIS
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
