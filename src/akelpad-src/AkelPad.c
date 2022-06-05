#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0500
#define _WIN32_IE 0x0500
#include <windows.h>
#include <tchar.h>
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <aclapi.h>
#include <richedit.h>
#include "AkelEdit\StackFunc.h"
#include "AkelEdit\StrFunc.h"
#include "AkelEdit\x64Func.h"
#include "AkelEdit\AkelBuild.h"
#include "AkelEdit\RegExpFunc.h"
#include "WideFunc.h"
#include "ResizeFunc.h"
#include "MethodFunc.h"
#include "AkelFiles\Langs\Resources\resource.h"
#include "AkelFiles\Langs\Resources\version.h"
#include "AkelFiles\Plugs\AkelDLL\AkelDLL.h"
#include "AkelPad.h"
#include "Edit.h"


/*
//Include stack functions
#ifndef AKELEDIT_STATICBUILD
  #define StackInsertBefore
  #define StackDelete
  #define StackJoin
#else
  #define StackInsertBefore_INCLUDED
  #define StackDelete_INCLUDED
  #define StackJoin_INCLUDED
#endif
#define StackGetElement
#define StackInsertAfter
#define StackInsertIndex
#define StackMoveAfter
#define StackMoveBefore
#define StackMoveIndex
#define StackClear
#include "AkelEdit\StackFunc.h"

//Include AEC functions
#ifndef AKELEDIT_STATICBUILD
  #define AEC_FUNCTIONS
  #include "AkelEdit\AkelEdit.h"
#endif

//Include string functions
#ifndef AKELEDIT_STATICBUILD
  #ifdef AKELPAD_LINGUISTICCASING
    #define WideCharLower_LINGUISTICCASING
    #define WideCharUpper_LINGUISTICCASING
  #endif
  #define WideCharLower
  #define WideCharUpper
  #define xmemcpy
  #define xmemcmp
  #define xmemset
  #define xarrlenW
  #define xarrcpynW
  #define xstrlenA
  #define xstrlenW
  #define xstrcmpW
  #define xstrcmpiA
  #define xstrcmpiW
  #define xstrcmpnW
  #define xstrcmpinW
  #define xstrcpyW
  #define xstrcpynA
  #define xstrcpynW
  #define xatoiW
  #define hex2decW
  #define UTF8toUTF16
  #define UTF16toUTF8
#else
  #define WideCharLower_INCLUDED
  #define WideCharUpper_INCLUDED
  #define xmemcpy_INCLUDED
  #define xmemcmp_INCLUDED
  #define xmemset_INCLUDED
  #define xarrlenW_INCLUDED
  #define xarrcpynW_INCLUDED
  #define xstrlenA_INCLUDED
  #define xstrlenW_INCLUDED
  #define xstrcmpW_INCLUDED
  #define xstrcmpiA_INCLUDED
  #define xstrcmpiW_INCLUDED
  #define xstrcmpnW_INCLUDED
  #define xstrcmpinW_INCLUDED
  #define xstrcpyW_INCLUDED
  #define xstrcpynA_INCLUDED
  #define xstrcpynW_INCLUDED
  #define xatoiW_INCLUDED
  #define hex2decW_INCLUDED
  #define UTF8toUTF16_INCLUDED
  #define UTF16toUTF8_INCLUDED
#endif
#define xarrlenA
#define xstrcpyA
#define xstrstrW
#define xatoiA
#define xuitoaW
#define xitoaW
#define dec2hexW
#define hex2binW
#define bin2hexW
#define xprintfW
#define UTF32toUTF16
#define UTF16toUTF32
#include "AkelEdit\StrFunc.h"

//Include RegExp functions
#ifndef AKELEDIT_STATICBUILD
  #define RE_FUNCTIONS
  #include "AkelEdit\RegExpFunc.h"
#endif

//Include x64 functions
#ifndef AKELEDIT_STATICBUILD
  #define MultiByteToWideChar64
  #define WideCharToMultiByte64
#else
  #define MultiByteToWideChar64_INCLUDED
  #define WideCharToMultiByte64_INCLUDED
#endif
#define GetFileSize64
#define SetFilePointer64
#define ReadFile64
#define WriteFile64
#include "AkelEdit\x64Func.h"

//Include wide functions
#define AppendMenuWide
#define CallWindowProcWide
#define ComboBox_AddStringWide
#define ComboBox_FindStringExactWide
#define ComboBox_FindStringWide
#define ComboBox_GetLBTextLenWide
#define ComboBox_GetLBTextWide
#define ComboBox_InsertStringWide
#define CreateDialogWide
#define CreateDirectoryWide
#define CreateEventWide
#define CreateFileWide
#define CreateFontIndirectWide
#define CreateMDIWindowWide
#define CreateProcessWide
#define CreateWindowExWide
#define DefWindowProcWide
#define DeleteFileWide
#define DialogBoxParamWide
#define DialogBoxWide
#define DirExistsWide
#define DispatchMessageWide
#define DragQueryFileWide
#define ExpandEnvironmentStringsWide
#define FileExistsAnsi
#define FileExistsWide
#define FindFirstFileWide
#define FindNextFileWide
#define FindWindowExWide
#define GetClassLongPtrWide
#define GetClassNameWide
#define GetCPInfoExWide
#define GetCurrentDirectoryWide
#define GetDateFormatWide
#define GetDlgItemTextWide
#define GetFileAttributesWide
#define GetFullPathNameWide
#define GetKeyNameTextWide
#define GetLongPathNameWide
#define GetMenuStringWide
#define GetMessageWide
#define GetModuleFileNameWide
#define GetModuleHandleWide
#define GetOpenFileNameWide
#define GetSaveFileNameWide
#define GetTimeFormatWide
#define GetWindowLongPtrWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define IconExtractWide
#define InsertMenuWide
#define IsDialogMessageWide
#define ListBox_AddStringWide
#define ListBox_FindStringWide
#define ListBox_GetTextWide
#define ListBox_InsertStringWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define LoadImageWide
#define LoadLibraryExWide
#define LoadLibraryWide
#define LoadStringWide
#define ModifyMenuWide
#define PeekMessageWide
#define PropertySheetWide
#define RegClearKeyAnsi
#define RegClearKeyWide
#define RegCreateKeyExWide
#define RegDeleteKeyWide
#define RegDeleteValueWide
#define RegEnumValueWide
#define RegisterClassWide
#define RegOpenKeyExWide
#define RegQueryValueExWide
#define RegSetValueExWide
#define SearchPathWide
#define SendMessageWide
#define SetClassLongPtrWide
#define SetCurrentDirectoryWide
#define SetDlgItemTextWide
#define SetFileAttributesWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define SHBrowseForFolderWide
#define ShellExecuteWide
#define SHGetPathFromIDListWide
#define StartDocWide
#define StatusBar_SetTextWide
#define TabCtrl_GetItemWide
#define TabCtrl_InsertItemWide
#define TabCtrl_SetItemWide
#define TranslateAcceleratorWide
#include "WideFunc.h"
//*/

//Include resize functions
#define ALLRESIZEFUNC
#include "ResizeFunc.h"

//Include method functions
#define ALLMETHODFUNC
#include "MethodFunc.h"

//Process
HANDLE hHeap;
HANDLE hMutex=0;
DWORD dwProcessId;

//WinMain
MAINCREATE mc;
HINSTANCE hInstance;
DWORD dwCmdShow=SW_SHOWNORMAL;
#ifndef AKELEDIT_STATICBUILD
  HMODULE hAkelLib=NULL;
#endif
STARTUPINFOA lpStartupInfoA;
wchar_t wszMainClass[MAX_PATH];
BOOL bNewInstance=FALSE;

//Identification
DWORD dwExeVersion=0;
BOOL bOldWindows;
BOOL bOldRichEdit=FALSE;
BOOL bOldComctl32=FALSE;
BOOL dwVerComctl32=0;
BOOL bAkelEdit=TRUE;
BOOL bWindowsNT4=FALSE;
BOOL bWindows7=FALSE;
INT_PTR nUniqueID=9;

//Buffers
unsigned char pcTranslateBuffer[TRANSLATE_BUFFER_SIZE];
wchar_t wszMsg[BUFFER_SIZE];
char buf[BUFFER_SIZE];
wchar_t wbuf[BUFFER_SIZE];
char buf2[BUFFER_SIZE];
wchar_t wbuf2[BUFFER_SIZE];

//Command line
wchar_t wszCmdLine[COMMANDLINE_SIZE];
wchar_t wszCmdArg[COMMANDARG_SIZE];
wchar_t *wpCmdLineBegin=NULL;
int nCmdLineBeginLen=0;
wchar_t *wpCmdLineEnd=NULL;
int nCmdLineEndLen=0;
BOOL bCmdLineChanged=FALSE;
const wchar_t *wpCmdLine=NULL;
wchar_t *wpCmdParamsStart=NULL;
wchar_t *wpCmdParamsEnd=NULL;
DWORD dwCmdLineOptions=0;
BOOL bCmdLineQuitAsEnd=FALSE;
int nParseCmdLineOnLoad=0;

//Language
HMODULE hLangModule;
DWORD dwLangSystem;
DWORD dwLangModule;
int nLangModuleCount=0;

//Procedures
HSTACK hMainProcStack={0};
HSTACK hMainProcRetStack={0};
HSTACK hEditProcStack={0};
HSTACK hEditProcRetStack={0};
HSTACK hFrameProcStack={0};
HSTACK hFrameProcRetStack={0};
WNDPROC lpfnMainProc;
WNDPROCRET lpfnMainProcRet;
WNDPROC lpfnFrameProc;
WNDPROCRET lpfnFrameProcRet;
WNDPROC lpfnEditProc;
WNDPROCRET lpfnEditProcRet;

//Plugins
STACKPLUGINFUNCTION hPluginsStack={0};
STACKPLUGINLIST hPluginListStack={0};
STACKPLUGINHANDLE hHandlesStack={0};
RECT rcPluginsMinMaxDialog={274, 192, 0, 0};
BOOL bSavePluginsStackOnExit=FALSE;
WNDPROC lpOldHotkeyInputProc=NULL;
wchar_t wszLastFunction[MAX_PATH]=L"";
int nLastFunctionIndex;

//INI
INIFILE hAkelPadIni={0};
wchar_t wszAkelPadIni[MAX_PATH];

//Main settings
HANDLE hReadOptions;
MAINOPTIONS moInit;
MAINOPTIONS moCur;
BOOL bSaveManual=FALSE;

//Main Window
HWND hMainWnd=NULL;
HWND hDummyWindow;
DWORD dwLastMainSizeType=SIZE_RESTORED;
DWORD dwMouseCapture=0;
HACCEL hGlobalAccel;
HACCEL hMainAccel;
HICON hMainIcon;
HCURSOR hCursorDragMove;
HCURSOR hCursorHandOpen;
HCURSOR hCursorHandClose;
HMENU hMainMenu;
HMENU hPopupMenu;
HMENU hPopupEdit;
HMENU hPopupView;
HMENU hPopupCodepage;
HMENU hPopupOpenCodepage;
HMENU hPopupSaveCodepage;
HMENU hPopupHeadline;
HMENU hMenuRecentFiles=NULL;
HMENU hMenuLanguage=NULL;
HMENU hMenuWindow=NULL;
BOOL bMenuPopupCodepage=TRUE;
BOOL bMenuRecentFiles=FALSE;
BOOL bMenuLanguage=FALSE;
BOOL bEnterMenuLoop=FALSE;
BOOL bMainOnStart=FALSE;
BOOL bMainCheckIdle=FALSE;
int nMainOnFinish=MOF_NONE;
BOOL bEditOnFinish=FALSE;
BOOL bFirstTabOnFinish=FALSE;

//Status window
STATUSSTATE ssStatus;
STACKSTATUSPART hStatusStack={0};
HWND hStatus;
HWND hProgress;
int nStatusHeight=0;
int nProgressWidth=0;

//Clones
HCURSOR hCursorSizeWE;
HCURSOR hCursorSizeNS;
HCURSOR hCursorSizeALL;
HCURSOR hCursorClone=NULL;
int nLastSplit=0;

//Docks
STACKDOCK hDocksStack={0};
NSIZE nsSize;
WNDPROC lpOldCloseButtonProc=NULL;

//Owner-drawn buttons
STACKBUTTONDRAW hButtonDrawStack={0};

//Codepages
int *lpCodepageList=NULL;
int nCodepageListLen=0;
BOOL bCodepageListChanged=FALSE;
int *lpCodepageTable;
int nCodepageTableCount;
int nAnsiCodePage;
int nOemCodePage;

//History
STACKRECENTFILE hRecentFilesStack={0};
STACKSEARCH hFindStack={0};
STACKSEARCH hReplaceStack={0};

//Open/Save document
OPENFILENAME_2000W *ofnStruct;
wchar_t *wszOfnFileList;
wchar_t wszOfnStream[MAX_PATH];
wchar_t wszFileFilter[MAX_PATH];
int nFileFilterLen;
BOOL bAutodetect=TRUE;
BOOL bSaveDlg;
DWORD dwOfnFlags;
BOOL bOfnBOM=FALSE;
int nOfnCodePage;
int nOfnStreamOffset;
POINT64 ptDocumentPos;
FILESTREAMDATA *lpStreamInData=NULL;
HWND hOfnDlgCombo;
HWND hOfnDlgEdit;
WNDPROC lpOldFilePreviewProc;
WNDPROC lpOldFileParentProc;

//AkelAdmin
wchar_t wszAkelAdminExe[MAX_PATH];
wchar_t wszAkelAdminPipe[32];
BOOL bPipeInitAkelAdmin=FALSE;
BOOL bSetSecurity=FALSE;
HICON hIconShieldAkelAdmin=NULL;

//MessageBox dialog
HWND hDlgMsgBox=NULL;
UINT dwLoadStringLastID=0;

//Modeless dialog
HWND hDlgModeless=NULL;
int nModelessType=MLT_NONE;
STACKMODELESS hModelessStack={0};
STACKMODELESS hEnumModelessStack={0};

//Recode dialog
RECT rcRecodeMinMaxDialog={246, 264, 0, 0};

//Find/Replace dialog
wchar_t *wszFindText=NULL;
wchar_t *wszReplaceText=NULL;
int nFindTextLen=0;
int nReplaceTextLen=0;
BOOL bNoSearchFinishMsg=FALSE;
BOOL bLockSearchSetTextCatch=FALSE;
WORD wLastReplaceButtonID=IDC_SEARCH_FIND_BUTTON;
WNDPROC lpOldComboboxEdit;

//Options dialog
HHOOK hHookPropertySheet;
HWND hWndPropertySheet;
HWND hWndPropTab;
HWND hWndPropOK;
HWND hWndPropCancel;
HWND hWndPropGeneral;
HWND hWndPropRegistry;
HWND hWndPropEditor1;
HWND hWndPropEditor2;
HWND hWndPropAdvanced;
int nPropertyStartPage=0;
BOOL bSaveSettingsSwitch;
BOOL bSaveHistorySwitch;
BOOL bOptionsRestart;
RECT rcPropMinMaxDialog={358, 470, 0, 0};
BOOL bPropResize;
WNDPROC lpOldPropProc;
RESIZEDIALOG rdsProp[]={{&hWndPropTab,      RDS_SIZE|RDS_X, 0},
                        {&hWndPropTab,      RDS_SIZE|RDS_Y, 0},
                        {&hWndPropOK,       RDS_MOVE|RDS_X, 0},
                        {&hWndPropOK,       RDS_MOVE|RDS_Y, 0},
                        {&hWndPropCancel,   RDS_MOVE|RDS_X, 0},
                        {&hWndPropCancel,   RDS_MOVE|RDS_Y, 0},
                        {&hWndPropGeneral,  RDS_SIZE|RDS_X, 0},
                        {&hWndPropGeneral,  RDS_SIZE|RDS_Y, 0},
                        {&hWndPropRegistry, RDS_SIZE|RDS_X, 0},
                        {&hWndPropRegistry, RDS_SIZE|RDS_Y, 0},
                        {&hWndPropEditor1,  RDS_SIZE|RDS_X, 0},
                        {&hWndPropEditor1,  RDS_SIZE|RDS_Y, 0},
                        {&hWndPropEditor2,  RDS_SIZE|RDS_X, 0},
                        {&hWndPropEditor2,  RDS_SIZE|RDS_Y, 0},
                        {&hWndPropAdvanced, RDS_SIZE|RDS_X, 0},
                        {&hWndPropAdvanced, RDS_SIZE|RDS_Y, 0},
                        {0, 0, 0}};

//Font/Color
STACKFONT hFontsStack={0};
STACKCOLORTHEME hThemesStack={0};
STACKBKIMAGE hBkImagesStack={0};
COLORREF crCustColors[16]={0};
RECT rcColorsMinMaxDialog={362, 333, 0, 0};
AECOLORS aecDefault;

//Print
HWND hWndPreviewEdit=NULL;
HWND hWndPreviewDlg=NULL;
HWND hWndZoomEdit;
STACKPRINTPAGE hPreviewAllPagesStack={0};
STACKPRINTPAGE hPreviewSelPagesStack={0};
RECT rcPreviewDialog={0};
RECT rcPreviewWindow={0};
RECT rcPreviewPaper={0};
RECT rcPreviewZoomed={0};
POINT ptPreviewScroll;
DWORD dwPreviewShowDialog=SW_MAXIMIZE;
int lpZoom[]={25, 50, 75, 100, 150, 200, 300, 400, 500, PREVIEWZOOM_FIT, PREVIEWZOOM_WIDTH};
int nPreviewZoomMaxIndex=8;
int nPreviewZoomValue=PREVIEWZOOM_FIT;
int nPreviewZoomPercent;
int nPreviewCharHeight;
int nPreviewAveCharWidth;
int nPreviewPageCur;
int nPreviewAllPageSum;
int nPreviewSelPageSum;
BOOL bPreviewSelection;
BOOL bPreviewRedrawLock;
HHOOK hHookKeys=NULL;
AEPRINT prn;
PRINTINFO prninfo={0};

//Zooming factor
POINT ptUnitCur={0};
POINT ptUnit96={0};

//Edit state
AECHARRANGE crCurSel={0};
AECHARINDEX ciCurCaret={0};
int nLoopCase=0;
DWORD dwWordBreakDefault=(DWORD)-1;
BOOL bRecentCaretMsg=FALSE;
BOOL bCheckingWriteTime=FALSE;
WNDPROC lpOldEditProc;

//Execute
char szExeFile[MAX_PATH];
wchar_t wszExeFile[MAX_PATH];
int nExeFileLen;
char szExeDir[MAX_PATH];
wchar_t wszExeDir[MAX_PATH];
int nExeDirLen;
wchar_t wszLangsDll[16];
wchar_t wszPlugsDll[16];
wchar_t wszAkelUpdaterExe[MAX_PATH];

//Mdi
STACKFRAMEDATA hFramesStack={0};
FRAMEDATA fdInit;
FRAMEDATA fdDefault;
FRAMEDATA *lpFrameCurrent=&fdDefault;
int nMDI=WMD_SDI;
HWND hMdiClient=NULL;
BOOL bMdiMaximize=-1;
BOOL bMdiNoWindows=FALSE;
HWND hTab=NULL;
UINT_PTR dwTabOpenTimer=0;
int nTabOpenItem=-1;
int nDocumentsCount=0;
int nDocumentsModified=0;
int nDocumentIndex=0;
DWORD dwChangedPrompt=0;
STACKASSOCICON hIconsStack={0};
HIMAGELIST hImageList;
HICON hIconEmpty=NULL;
BOOL bTabPressing=FALSE;
BOOL bFrameActivating=FALSE;
DWORD dwMdiFrameActivating=0;
RECT rcMdiListMinMaxDialog={221, 463, 0, 0};
WNDPROC lpOldMdiClientProc;
WNDPROC lpOldTabProc;
FRAMEDATA *lpFrame;

//GetProcAddress
HMONITOR (WINAPI *MonitorFromPointPtr)(POINT, DWORD);
BOOL (WINAPI *GetMonitorInfoAPtr)(HMONITOR, LPMONITORINFO);
DWORD (WINAPI *SetSecurityInfoPtr)(HANDLE, SE_OBJECT_TYPE, SECURITY_INFORMATION, PSID, PSID, PACL, PACL)=NULL;
DWORD (WINAPI *SetEntriesInAclWPtr)(ULONG, PEXPLICIT_ACCESSW, PACL, PACL *)=NULL;
BOOL (WINAPI *ShellExecuteExWPtr)(LPSHELLEXECUTEINFOW)=NULL;

//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

//DLL build
#ifdef AKELPAD_DLLBUILD
HWND __declspec(dllexport) Create(unsigned char *pCmdLine, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
{
  mc.pCmdLine=pCmdLine;
  mc.dwStyle=dwStyle;
  mc.x=x;
  mc.y=y;
  mc.nWidth=nWidth;
  mc.nHeight=nHeight;
  mc.hWndParent=hWndParent;
  mc.hMenu=hMenu;

  _WinMain();

  return hMainWnd;
}

DWORD __declspec(dllexport) Translate(MSG *lpMsg)
{
  DWORD dwResult=TranslateMessageAll(TMSG_ALL, lpMsg);

  if (bMainCheckIdle)
  {
    if (GetQueueStatus(QS_ALLINPUT) == 0)
    {
      bMainCheckIdle=FALSE;
      SendMessage(hMainWnd, AKDN_MAIN_ONSTART_IDLE, 0, 0);
    }
  }
  return dwResult;
}

BOOL __declspec(dllexport) Close()
{
  BOOL bResult=FALSE;

  if (hMainWnd)
  {
    //Send exit command
    SendMessage(hMainWnd, WM_CLOSE, 0, 0);

    if (nMainOnFinish)
    {
      bResult=TRUE;

      //Destroy main window
      SendMessage(hMainWnd, AKDN_MAIN_ONFINISH, 0, 0);
      WinMainCleanUp();
    }
  }
  else bResult=TRUE;

  return bResult;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    hInstance=hinstDLL;
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
#endif

//Entry point
//void _WinMain()
EXTERN_C int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPTSTR lpCmdLine, int nShowCmd)
{
  WNDCLASSW wndclassW;
  HMODULE hUser32;
  int nMajor;
  int nMinor;
  int nRelease;
  int nBuild;

  //Process
  hHeap=GetProcessHeap();
  dwProcessId=GetCurrentProcessId();

  //Initialize WideFunc.h header
  WideInitialize();

  //Is unicode Windows
  bOldWindows=WideGlobal_bOldWindows;

  //Is Windows NT4?
  if (!bOldWindows)
  {
    OSVERSIONINFO ovi;

    ovi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
    GetVersionEx(&ovi);
    if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
      if (ovi.dwMajorVersion == 4 && ovi.dwMinorVersion == 0)
        bWindowsNT4=TRUE;
      else if (ovi.dwMajorVersion >= 6)
        bWindows7=TRUE;
    }
  }

  //Get program version
  {
    DWORD ver[4]={RC_AKELPADID};

    dwExeVersion=MAKE_IDENTIFIER(ver[0], ver[1], ver[2], ver[3]);
  }

  //Fill MAINCREATE if not DLL build
  #ifndef AKELPAD_DLLBUILD
    if (bOldWindows)
      mc.pCmdLine=(BYTE *)GetCommandLineParamsA();
    else
      mc.pCmdLine=(BYTE *)GetCommandLineParamsW();
    mc.hWndParent=NULL;
    mc.dwStyle=WS_OVERLAPPEDWINDOW;
  #endif

  //Get startup info
  #ifndef AKELPAD_DLLBUILD
    lpStartupInfoA.cb=sizeof(STARTUPINFOA);
    GetStartupInfoA(&lpStartupInfoA);
    if ((lpStartupInfoA.dwFlags & STARTF_USESHOWWINDOW) && lpStartupInfoA.wShowWindow != SW_SHOWDEFAULT)
      dwCmdShow=lpStartupInfoA.wShowWindow;
  #endif

  //Get program HINSTANCE
  #ifndef AKELPAD_DLLBUILD
    hInstance=GetModuleHandleWide(NULL);
  #endif

  //Get program directory
  nExeFileLen=GetModuleFileNameWide(hInstance, wszExeFile, MAX_PATH);
  WideCharToMultiByte(CP_ACP, 0, wszExeFile, nExeFileLen + 1, szExeFile, MAX_PATH, NULL, NULL);

  nExeDirLen=GetFileDir(wszExeFile, nExeFileLen, wszExeDir, MAX_PATH);
  WideCharToMultiByte(CP_ACP, 0, wszExeDir, nExeDirLen + 1, szExeDir, MAX_PATH, NULL, NULL);

  //Main window class
  xstrcpynW(wszMainClass, APP_MAIN_CLASSW, MAX_PATH);

  //Zero AkelAdmin variables
  wszAkelAdminPipe[0]=L'\0';
  wszAkelAdminExe[0]=L'\0';

  //INI
  xprintfW(wszAkelPadIni, L"%s\\AkelPad.ini", wszExeDir);

  //System default codepages
  nAnsiCodePage=GetACP();
  nOemCodePage=GetOEMCP();
  dwLangSystem=GetUserDefaultLangID();

  //System default print metrics
  if (GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, buf, BUFFER_SIZE))
  {
    if (*buf == '0')
    {
      prninfo.dwPageFlags=PSD_INHUNDREDTHSOFMILLIMETERS;
      prninfo.rtMargin.left=1000;
      prninfo.rtMargin.right=1000;
      prninfo.rtMargin.top=1000;
      prninfo.rtMargin.bottom=1000;
    }
    else
    {
      prninfo.dwPageFlags=PSD_INTHOUSANDTHSOFINCHES;
      prninfo.rtMargin.left=500;
      prninfo.rtMargin.right=500;
      prninfo.rtMargin.top=500;
      prninfo.rtMargin.bottom=500;
    }
    prninfo.hDC=NULL;
    prninfo.dwPrintFlags=PD_ALLPAGES|PD_COLLATE;
    prninfo.nFromPage=1;
    prninfo.nToPage=1;
    prninfo.nCopies=1;
  }

  //Set default options before reading from registry/ini
  xmemset(&moInit, 0, sizeof(MAINOPTIONS));
  xmemset(&fdInit, 0, sizeof(FRAMEDATA));
  xmemset(&aecDefault, 0, sizeof(AECOLORS));

  //Codepage
  moInit.nDefaultCodePage=nAnsiCodePage;
  moInit.bDefaultBOM=FALSE;
  moInit.nNewFileCodePage=nAnsiCodePage;
  moInit.bNewFileBOM=FALSE;
  moInit.nNewFileNewLine=NEWLINE_WIN;

  //--Edit state external--
  //fdInit.hWndEditParent=NULL;
  //fdInit.ei.hWndEdit=NULL;
  //fdInit.ei.hDocEdit=NULL;
  fdInit.ei.pFile=bOldWindows?(LPBYTE)fdInit.szFile:(LPBYTE)fdInit.wszFile;
  fdInit.ei.szFile=fdInit.szFile;
  fdInit.ei.wszFile=fdInit.wszFile;
  fdInit.ei.nCodePage=moInit.nNewFileCodePage;
  fdInit.ei.bBOM=moInit.bNewFileBOM;
  fdInit.ei.nNewLine=moInit.nNewFileNewLine;
  //fdInit.ei.bModified=FALSE;
  //fdInit.ei.bReadOnly=FALSE;
  //fdInit.ei.bWordWrap=FALSE;
  //fdInit.ei.bOvertypeMode=FALSE;
  //fdInit.ei.hWndMaster=NULL;
  //fdInit.ei.hDocMaster=NULL;
  //fdInit.ei.hWndClone1=NULL;
  //fdInit.ei.hDocClone1=NULL;
  //fdInit.ei.hWndClone2=NULL;
  //fdInit.ei.hDocClone2=NULL;
  //fdInit.ei.hWndClone3=NULL;
  //fdInit.ei.hDocClone3=NULL;
  //fdInit.szFile[0]='\0';
  //fdInit.wszFile[0]=L'\0';
  //fdInit.nFileLen=0;
  fdInit.hIcon=hIconEmpty;
  fdInit.nIconIndex=0;
  //fdInit.rcEditWindow.left=0;
  //fdInit.rcEditWindow.top=0;
  //fdInit.rcEditWindow.right=0;
  //fdInit.rcEditWindow.bottom=0;
  //fdInit.rcMasterWindow.left=0;
  //fdInit.rcMasterWindow.top=0;
  //fdInit.rcMasterWindow.right=0;
  //fdInit.rcMasterWindow.bottom=0;

  //--Edit state internal--
  //fdInit.lpEditProc=NULL;
  //fdInit.ft.dwLowDateTime=0;
  //fdInit.ft.dwHighDateTime=0;
  fdInit.dwInputLocale=(HKL)(UINT_PTR)-1;

  //--Edit settings--
  fdInit.rcEditMargins.left=4;
  fdInit.rcEditMargins.top=1;
  fdInit.rcEditMargins.right=4;
  fdInit.rcEditMargins.bottom=0;
  fdInit.nTabStopSize=EDIT_TABSTOPS;
  fdInit.bTabStopAsSpaces=FALSE;
  fdInit.nUndoLimit=EDIT_UNDOLIMIT;
  fdInit.bDetailedUndo=FALSE;
  fdInit.dwWrapType=AEWW_WORD;
  //fdInit.dwWrapLimit=0;
  //fdInit.dwMarker=0;
  //fdInit.dwMappedPrintWidth=0;
  fdInit.dwCaretOptions=CO_CARETACTIVELINE|CO_CARETACTIVELINEBORDER;
  fdInit.nCaretWidth=1;
  fdInit.dwMouseOptions=MO_LEFTMARGINSELECTION|MO_RICHEDITMOUSE|MO_MOUSEDRAGGING;
  fdInit.dwLineGap=1;
  fdInit.bShowURL=TRUE;
  fdInit.nClickURL=2;
  //fdInit.bUrlPrefixesEnable=FALSE;
  //fdInit.bUrlDelimitersEnable=FALSE;
  fdInit.bWordDelimitersEnable=TRUE;
  //fdInit.bWrapDelimitersEnable=FALSE;

  fdInit.nUrlPrefixesLen=(int)xstrcpynW(fdInit.wszUrlPrefixes, STR_URL_PREFIXESW, URL_PREFIXES_SIZE);
  fdInit.nUrlLeftDelimitersLen=(int)xarrcpynW(fdInit.wszUrlLeftDelimiters, STR_URL_LEFTDELIMITERSW, URL_DELIMITERS_SIZE) - 2;
  fdInit.nUrlRightDelimitersLen=(int)xarrcpynW(fdInit.wszUrlRightDelimiters, STR_URL_RIGHTDELIMITERSW, URL_DELIMITERS_SIZE) - 2;
  fdInit.nWordDelimitersLen=(int)xarrcpynW(fdInit.wszWordDelimiters, STR_WORD_DELIMITERSW, WORD_DELIMITERS_SIZE) - 2;
  fdInit.nWrapDelimitersLen=(int)xarrcpynW(fdInit.wszWrapDelimiters, STR_WRAP_DELIMITERSW, WRAP_DELIMITERS_SIZE) - 2;
  //moInit.wszBkImageFileDlg[0]=L'\0';
  fdInit.nBkImageAlpha=EDIT_BKIMAGEALPHA;

  //--Save place--
  moInit.nSaveSettings=SS_REGISTRY;
  moInit.nSaveHistory=SS_REGISTRY;

  //--Manual--
  moInit.dwShowModify=SM_STATUSBAR|SM_TABTITLE_MDI;
  //moInit.dwStatusPosType=0;
  //moInit.wszStatusUserFormat[0]=L'\0';
  //moInit.nStatusUserFormatLen=0;
  moInit.dwWordBreakCustom=AEWB_LEFTWORDSTART|AEWB_RIGHTWORDEND;
  //moInit.dwPaintOptions=0;
  //moInit.nFixedCharWidth=0;
  //moInit.dwEditStyle=0;
  //moInit.bRichEditClass=FALSE;
  moInit.bAkelAdminResident=TRUE;
  //moInit.wszDateLogFormat[0]=L'\0';
  //moInit.wszDateInsertFormat[0]=L'\0';
  //moInit.wszAkelUpdaterOptions[0]=L'\0';
  //moInit.wszUrlCommand[0]=L'\0';
  //moInit.wszTabNameFind[0]=L'\0';
  //moInit.wszTabNameRep[0]=L'\0';

  //--Menu settings--
  //moInit.bOnTop=FALSE;
  moInit.bStatusBar=TRUE;
  //moInit.szLangModule[0]='\0';
  //moInit.wszLangModule[0]=L'\0';
  //moInit.bKeepSpace=FALSE;
  moInit.bWatchFile=TRUE;
  //moInit.bSaveTime=FALSE;
  //moInit.bSingleOpenFile=FALSE;
  moInit.dwSingleOpenProgram=SOP_ON;
  moInit.nMDI=WMD_SDI;
  moInit.dwTabOptionsMDI=TAB_VIEW_TOP|TAB_TYPE_STANDARD|TAB_SWITCH_RIGHTLEFT;

  //--Settings dialog--
  //xmemset(&moInit.rcPropCurrentDialog, 0, sizeof(RECT));
  //moInit.wszExecuteCommand[0]=L'\0';
  //moInit.wszExecuteDirectory[0]=L'\0';
  //lpCodepageList=NULL;
  moInit.dwLangCodepageRecognition=dwLangSystem;
  moInit.dwCodepageRecognitionBuffer=DETECT_CODEPAGE_SIZE;
  moInit.bSavePositions=TRUE;
  moInit.bSaveCodepages=TRUE;
  moInit.nRecentFiles=RECENTFILES_AMOUNT;
  moInit.nSearchStrings=SEARCHSTRINGS_AMOUNT;
  xstrcpyW(moInit.wszFileTypesOpen, STR_ASSOCIATE_OPENW);
  xstrcpyW(moInit.wszFileTypesEdit, STR_ASSOCIATE_EDITW);
  xstrcpyW(moInit.wszFileTypesPrint, STR_ASSOCIATE_PRINTW);
  //moInit.dwFileTypesAssociated=0;
  //moInit.dwKeybLayoutOptions=0;
  //moInit.bSilentCloseEmptyMDI=FALSE;
  //moInit.bDateLog=FALSE;
  //moInit.bSaveInReadOnlyMsg=FALSE;
  xstrcpyW(moInit.wszDefaultSaveExt, STR_DEFAULTSAVEEXTW);

  //--Open file dialog--
  //moInit.wszOfnDir[0]=L'\0';
  //moInit.wszLastDir[0]=L'\0';
  moInit.bShowPlacesBar=TRUE;

  //--Print dialog--
  moInit.rcPrintMargins=prninfo.rtMargin;
  //moInit.dwPrintColor=0;
  //moInit.bPrintHeaderEnable=FALSE;
  xstrcpyW(moInit.wszPrintHeader, STR_PRINT_HEADERW);
  //moInit.bPrintFooterEnable=FALSE;
  xstrcpyW(moInit.wszPrintFooter, STR_PRINT_FOOTERW);
  //moInit.bPrintFontEnable=FALSE;

  //--Modeless dialog--
  //moInit.bModelessSavePos=FALSE;

  //--Recode dialog--
  //xmemset(&moInit.rcRecodeCurrentDialog, 0, sizeof(RECT));

  //--Search dialog--
  //xmemset(&moInit.rcSearchCurrentDialog, 0, sizeof(RECT));
  moInit.dwSearchOptions=FRF_DOWN;

  //--Go to line dialog--
  //xmemset(&moInit.rcGotoCurrentDialog, 0, sizeof(RECT));
  moInit.dwGotoOptions=GT_LINE;

  //--Colors dialog--
  //xmemset(&moInit.rcColorsCurrentDialog, 0, sizeof(RECT));

  //--Plugin dialog--
  //xmemset(&moInit.rcPluginsCurrentDialog, 0, sizeof(RECT));

  //--Mdi list dialog--
  //moInit.dwMdiListOptions=0;
  //xmemset(&moInit.rcMdiListCurrentDialog, 0, sizeof(RECT));

  //--Main window--
  //moInit.dwMainStyle=0;
  moInit.rcMainWindowRestored.left=CW_USEDEFAULT;
  moInit.rcMainWindowRestored.top=CW_USEDEFAULT;
  moInit.rcMainWindowRestored.right=CW_USEDEFAULT;
  moInit.rcMainWindowRestored.bottom=CW_USEDEFAULT;
  moInit.dwMdiStyle=WS_MAXIMIZE;

  //Read only few options
  hReadOptions=ReadOptions(&moInit, &fdInit, PCL_ONLOAD, NULL);
  xmemcpy(&moCur, &moInit, sizeof(MAINOPTIONS));
  nMDI=moInit.nMDI;

  //Command line
  wpCmdLine=GetCommandLineParamsWide(mc.pCmdLine, &wpCmdParamsStart, &wpCmdParamsEnd);

  //Parse commmand line on load
  if (wpCmdLine)
    nParseCmdLineOnLoad=ParseCmdLine(&wpCmdLine, PCL_ONLOAD);

  //Mutex
  #ifndef AKELPAD_DLLBUILD
    if (!(lpStartupInfoA.dwFlags & STARTF_NOMUTEX))
    {
      if (hMutex=CreateEventWide(NULL, FALSE, FALSE, wszMainClass))
      {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
          WaitForSingleObject(hMutex, INFINITE);
      }
    }

    if ((nMDI == WMD_MDI || nMDI == WMD_PMDI) && (moCur.dwSingleOpenProgram & SOP_ON) && !bNewInstance)
    {
      HWND hWndFriend;
      DWORD dwAtom;

      //Pass command line to opened instance
      if (hWndFriend=FindAkelCopy())
      {
        dwAtom=(DWORD)GetClassLongPtrWide(hWndFriend, GCW_ATOM);
        if (dwCmdShow != SW_HIDE && dwCmdShow != SW_SHOWMINNOACTIVE && dwCmdShow != SW_SHOWNA && dwCmdShow != SW_SHOWNOACTIVATE)
          ActivateWindow(hWndFriend);

        //Wait until we can send PostMessage.
        while (!IsWindowEnabled(hWndFriend) || SendMessage(hWndFriend, AKD_GETQUEUE, QS_ALLEVENTS, 0))
        {
          Sleep(100);

          //Is window still exist?
          if (GetClassLongPtrWide(hWndFriend, GCW_ATOM) != dwAtom)
            goto Quit;
        }

        //Send command line parameters without CmdLineBegin and CmdLineEnd
        *wpCmdParamsEnd=L'\0';
        SendCmdLine(hWndFriend, wpCmdParamsStart, TRUE, TRUE);
        goto Quit;
      }
    }
  #endif

  if (wpCmdLine)
  {
    if (nParseCmdLineOnLoad == PCLE_QUIT)
      goto Quit;
    else if (nParseCmdLineOnLoad != PCLE_ONLOAD)
      wpCmdLine=NULL;
  }

  //Read all options
  ReadOptions(&moInit, &fdInit, PCL_ONSHOW, hReadOptions);

  if (!fdInit.lf.lfHeight)
  {
    //Get edit font
    HDC hDC;

    if (hDC=GetDC(NULL))
    {
      fdInit.lf.lfHeight=-MulDiv(10, GetDeviceCaps(hDC, LOGPIXELSY), 72);
      fdInit.lf.lfWidth=0;
      fdInit.lf.lfEscapement=0;
      fdInit.lf.lfOrientation=0;
      fdInit.lf.lfWeight=FW_NORMAL;
      fdInit.lf.lfItalic=FALSE;
      fdInit.lf.lfUnderline=FALSE;
      fdInit.lf.lfStrikeOut=FALSE;
      fdInit.lf.lfCharSet=DEFAULT_CHARSET;
      fdInit.lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
      fdInit.lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
      fdInit.lf.lfQuality=DEFAULT_QUALITY;
      fdInit.lf.lfPitchAndFamily=DEFAULT_PITCH;
      xstrcpynW(fdInit.lf.lfFaceName, L"Courier New", LF_FACESIZE);
      ReleaseDC(NULL, hDC);
    }
  }
  if (!moInit.lfPrintFont.lfHeight)
  {
    //Get print font
    xmemcpy(&moInit.lfPrintFont, &fdInit.lf, sizeof(LOGFONTW));
  }
  StackFontItemInsert(&hFontsStack, &fdInit.lf);

  if (IsCodePageUnicode(moInit.nDefaultCodePage))
    moInit.bDefaultBOM=TRUE;
  fdInit.ei.nCodePage=moInit.nNewFileCodePage;
  fdInit.ei.bBOM=moInit.bNewFileBOM;
  prninfo.rtMargin=moInit.rcPrintMargins;
  if (!lpCodepageList) nCodepageListLen=EnumCodepageList(&lpCodepageList);
  if (!wpCmdLineBegin) wpCmdLineBegin=(wchar_t *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(wchar_t));
  if (!wpCmdLineEnd) wpCmdLineEnd=(wchar_t *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(wchar_t));
  xmemcpy(&fdDefault, &fdInit, sizeof(FRAMEDATA));

  //Normalize search flags
  moInit.dwSearchOptions=(moInit.dwSearchOptions & FRF_DOWN) |
          (moInit.dwSearchOptions & FRF_WHOLEWORD) |
          (moInit.dwSearchOptions & FRF_MATCHCASE) |
          (moInit.dwSearchOptions & FRF_UP) |
          (moInit.dwSearchOptions & FRF_BEGINNING) |
          (moInit.dwSearchOptions & FRF_SELECTION) |
          (moInit.dwSearchOptions & FRF_ESCAPESEQ) |
          (moInit.dwSearchOptions & FRF_REGEXP) |
          (moInit.dwSearchOptions & FRF_REGEXPNONEWLINEDOT) |
          (moInit.dwSearchOptions & FRF_ALLFILES) |
          (moInit.dwSearchOptions & FRF_REPLACEALLANDCLOSE) |
          (moInit.dwSearchOptions & FRF_REPLACEALLNOMSG) |
          (moInit.dwSearchOptions & FRF_CHECKINSELIFSEL) |
          (moInit.dwSearchOptions & FRF_CYCLESEARCH) |
          (moInit.dwSearchOptions & FRF_CYCLESEARCHPROMPT);

  //Normalize tab flags
  if (!(moInit.dwTabOptionsMDI & TAB_VIEW_NONE) && !(moInit.dwTabOptionsMDI & TAB_VIEW_TOP) && !(moInit.dwTabOptionsMDI & TAB_VIEW_BOTTOM))
    moInit.dwTabOptionsMDI|=TAB_VIEW_TOP;
  if (!(moInit.dwTabOptionsMDI & TAB_TYPE_STANDARD) && !(moInit.dwTabOptionsMDI & TAB_TYPE_BUTTONS) && !(moInit.dwTabOptionsMDI & TAB_TYPE_FLATBUTTONS))
    moInit.dwTabOptionsMDI|=TAB_TYPE_STANDARD;
  if (!(moInit.dwTabOptionsMDI & TAB_SWITCH_NEXTPREV) && !(moInit.dwTabOptionsMDI & TAB_SWITCH_RIGHTLEFT))
    moInit.dwTabOptionsMDI|=TAB_SWITCH_RIGHTLEFT;

  //Get status bar user flags
  if (moInit.wszStatusUserFormat[0])
  {
    STATUSPART *sp;

    moInit.nStatusUserFormatLen=(int)xstrlenW(moInit.wszStatusUserFormat);
    TranslateStatusUser(NULL, moInit.wszStatusUserFormat, moInit.nStatusUserFormatLen, NULL, 0);

    moInit.dwStatusUserFlags=0;
    for (sp=hStatusStack.first; sp; sp=sp->next)
      moInit.dwStatusUserFlags|=sp->dwFormatFlags;
  }

  //Get ansi language module
  WideCharToMultiByte(CP_ACP, 0, moInit.wszLangModule, -1, moInit.szLangModule, MAX_PATH, NULL, NULL);

  //Copy initial options
  xmemcpy(&moCur, &moInit, sizeof(MAINOPTIONS));

  //Get common controls version
  GetFileVersionA("comctl32.dll", &nMajor, &nMinor, &nRelease, &nBuild, NULL);
  if (nMajor < 4 || (nMajor == 4 && nMinor < 71))
    bOldComctl32=TRUE;
  else
    bOldComctl32=FALSE;
  dwVerComctl32=MAKELONG(nMajor, nMinor);

  //Get functions addresses
  hUser32=GetModuleHandleA("user32.dll");
  MonitorFromPointPtr=(HMONITOR (WINAPI *)(POINT, DWORD))GetProcAddress(hUser32, "MonitorFromPoint");
  GetMonitorInfoAPtr=(BOOL (WINAPI *)(HMONITOR, LPMONITORINFO))GetProcAddress(hUser32, "GetMonitorInfoA");

  //Load DLL's
  hLangModule=hInstance;
  dwLangModule=RC_VERSIONLANGID;

  #ifdef _WIN64
    xstrcpyW(wszLangsDll, L"Langs64");
    xprintfW(wbuf, L"%s\\AkelFiles\\%s", wszExeDir, wszLangsDll);
    if (!DirExistsWide(wbuf))
      xstrcpyW(wszLangsDll, L"Langs");

    xstrcpyW(wszPlugsDll, L"Plugs64");
    xprintfW(wbuf, L"%s\\AkelFiles\\%s", wszExeDir, wszPlugsDll);
    if (!DirExistsWide(wbuf))
      xstrcpyW(wszPlugsDll, L"Plugs");

    xprintfW(wszAkelAdminExe, L"%s\\AkelFiles\\AkelAdmin64.exe", wszExeDir);
    if (!FileExistsWide(wszAkelAdminExe))
      xprintfW(wszAkelAdminExe, L"%s\\AkelFiles\\AkelAdmin.exe", wszExeDir);
  #else
    xstrcpyW(wszLangsDll, L"Langs");
    xstrcpyW(wszPlugsDll, L"Plugs");
    xprintfW(wszAkelAdminExe, L"%s\\AkelFiles\\AkelAdmin.exe", wszExeDir);
  #endif

  if (*moCur.wszLangModule)
  {
    BOOL bResult;

    xprintfW(wbuf, L"%s\\AkelFiles\\%s\\%s", wszExeDir, wszLangsDll, moCur.wszLangModule);
    if (bOldWindows)
    {
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, buf, MAX_PATH, NULL, NULL);
      bResult=GetFileVersionA(buf, &nMajor, &nMinor, &nRelease, &nBuild, &dwLangModule);
    }
    else bResult=GetFileVersionW(wbuf, &nMajor, &nMinor, &nRelease, &nBuild, &dwLangModule);

    if (bResult && MAKE_IDENTIFIER(nMajor, nMinor, nRelease, nBuild) == dwExeVersion)
    {
      if (!(hLangModule=LoadLibraryWide(wbuf)))
      {
        hLangModule=hInstance;
        API_LoadString(hLangModule, MSG_ERROR_LOAD_DLL, wbuf, BUFFER_SIZE);
        xprintfW(wszMsg, wbuf, moCur.wszLangModule);
        API_MessageBox(NULL, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
      }
    }
    else
    {
      API_LoadString(hLangModule, MSG_UPDATE_LANGMODULE, wbuf, BUFFER_SIZE);
      xprintfW(wszMsg, wbuf, moCur.wszLangModule,
                             nMajor, nMinor, nRelease, nBuild,
                             LOBYTE(dwExeVersion), HIBYTE(dwExeVersion), LOBYTE(HIWORD(dwExeVersion)), HIBYTE(HIWORD(dwExeVersion)));
      API_MessageBox(NULL, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
    }
  }

  #ifdef AKELEDIT_STATICBUILD
    OleInitialize(0);
    if (bOldWindows)
      AE_RegisterClassA(hInstance, TRUE, TRUE, TRUE);
    else
    {
      AE_RegisterClassA(hInstance, TRUE, FALSE, FALSE);
      AE_RegisterClassW(hInstance, TRUE);
    }
  #else
    if (!(hAkelLib=LoadLibraryWide(L"AkelEdit.dll")))
    {
      API_LoadString(hLangModule, MSG_ERROR_LOAD_DLL, wbuf, BUFFER_SIZE);
      xprintfW(wszMsg, wbuf, L"AkelEdit.dll");
      API_MessageBox(NULL, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
      goto Quit;
    }
  #endif

  //GetOpenFileName dialog file filter
  API_LoadString(hLangModule, STR_FILE_FILTER, wszFileFilter, MAX_PATH);
  for (nFileFilterLen=0; wszFileFilter[nFileFilterLen]; ++nFileFilterLen)
    if (wszFileFilter[nFileFilterLen] == L'|') wszFileFilter[nFileFilterLen]=L'\0';
  wszFileFilter[++nFileFilterLen]=L'\0';

  //AkelUpdater path
  xprintfW(wszAkelUpdaterExe, L"%s\\AkelFiles\\AkelUpdater.exe", wszExeDir);

  if (bOldWindows)
  {
    hGlobalAccel=API_LoadAcceleratorsA(hLangModule, MAKEINTRESOURCEA(IDA_ACCEL_GLOBAL));
    hMainAccel=API_LoadAcceleratorsA(hLangModule, MAKEINTRESOURCEA(IDA_ACCEL_MAIN));
    hMainIcon=API_LoadIconA(hLangModule, MAKEINTRESOURCEA(IDI_ICON_MAIN));
    hMainMenu=API_LoadMenuA(hLangModule, MAKEINTRESOURCEA(IDM_MENU_MAIN));
    hPopupMenu=API_LoadMenuA(hLangModule, MAKEINTRESOURCEA(IDM_MENU_POPUP));
    hCursorSizeWE=LoadCursor(NULL, IDC_SIZEWE);
    hCursorSizeNS=LoadCursor(NULL, IDC_SIZENS);
    hCursorSizeALL=LoadCursor(NULL, IDC_SIZEALL);
    hCursorHandOpen=(HCURSOR)API_LoadImageA(hLangModule, MAKEINTRESOURCEA(IDC_CURSOR_HANDOPEN), IMAGE_CURSOR, 0, 0, 0);
    hCursorHandClose=(HCURSOR)API_LoadImageA(hLangModule, MAKEINTRESOURCEA(IDC_CURSOR_HANDCLOSE), IMAGE_CURSOR, 0, 0, 0);
    if (nMDI)
    {
      hIconEmpty=(HICON)API_LoadImageA(hLangModule, MAKEINTRESOURCEA(IDI_ICON_EMPTY), IMAGE_ICON, 0, 0, 0);
      hCursorDragMove=(HCURSOR)API_LoadImageA(hLangModule, MAKEINTRESOURCEA(IDC_CURSOR_DRAGMOVE), IMAGE_CURSOR, 0, 0, 0);
    }
  }
  else
  {
    hGlobalAccel=API_LoadAcceleratorsW(hLangModule, MAKEINTRESOURCEW(IDA_ACCEL_GLOBAL));
    hMainAccel=API_LoadAcceleratorsW(hLangModule, MAKEINTRESOURCEW(IDA_ACCEL_MAIN));
    hMainIcon=API_LoadIconW(hLangModule, MAKEINTRESOURCEW(IDI_ICON_MAIN));
    hMainMenu=API_LoadMenuW(hLangModule, MAKEINTRESOURCEW(IDM_MENU_MAIN));
    hPopupMenu=API_LoadMenuW(hLangModule, MAKEINTRESOURCEW(IDM_MENU_POPUP));
    hCursorSizeWE=LoadCursor(NULL, IDC_SIZEWE);
    hCursorSizeNS=LoadCursor(NULL, IDC_SIZENS);
    hCursorSizeALL=LoadCursor(NULL, IDC_SIZEALL);
    hCursorHandOpen=(HCURSOR)API_LoadImageW(hLangModule, MAKEINTRESOURCEW(IDC_CURSOR_HANDOPEN), IMAGE_CURSOR, 0, 0, 0);
    hCursorHandClose=(HCURSOR)API_LoadImageW(hLangModule, MAKEINTRESOURCEW(IDC_CURSOR_HANDCLOSE), IMAGE_CURSOR, 0, 0, 0);
    if (nMDI)
    {
      hIconEmpty=(HICON)API_LoadImageW(hLangModule, MAKEINTRESOURCEW(IDI_ICON_EMPTY), IMAGE_ICON, 0, 0, 0);
      hCursorDragMove=(HCURSOR)API_LoadImageW(hLangModule, MAKEINTRESOURCEW(IDC_CURSOR_DRAGMOVE), IMAGE_CURSOR, 0, 0, 0);
    }
  }

  lpfnMainProc=MainProc;
  lpfnMainProcRet=NULL;
  lpfnFrameProc=FrameProc;
  lpfnFrameProcRet=NULL;
  lpfnEditProc=EditProc;
  lpfnEditProcRet=NULL;
  StackProcSet(&hMainProcStack, MainProc, NULL, NULL);
  StackProcSet(&hEditProcStack, EditProc, NULL, NULL);
  StackProcSet(&hFrameProcStack, FrameProc, NULL, NULL);

  InitCommonControls();

  wndclassW.style        =CS_DBLCLKS;
  wndclassW.lpfnWndProc  =CommonMainProc;
  wndclassW.cbClsExtra   =0;
  wndclassW.cbWndExtra   =DLGWINDOWEXTRA;
  wndclassW.hInstance    =hInstance;
  wndclassW.hIcon        =hMainIcon;
  wndclassW.hCursor      =LoadCursor(NULL, IDC_ARROW);
  //HOLLOW_BRUSH on Win7 with aero cause black flickering
  //wndclassW.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
  //so use COLOR_WINDOW and return 1 in WM_ERASEBKGND.
  wndclassW.hbrBackground=(HBRUSH)(UINT_PTR)(COLOR_WINDOW + 1);
  wndclassW.lpszMenuName =NULL;
  wndclassW.lpszClassName=wszMainClass;
  if (!RegisterClassWide(&wndclassW)) goto Quit;

  if (nMDI == WMD_MDI)
  {
    wndclassW.style        =0;
    wndclassW.lpfnWndProc  =CommonFrameProc;
    wndclassW.cbClsExtra   =0;
    wndclassW.cbWndExtra   =DLGWINDOWEXTRA;
    wndclassW.hInstance    =hInstance;
    wndclassW.hIcon        =NULL;
    wndclassW.hCursor      =NULL;
    wndclassW.hbrBackground=NULL;
    wndclassW.lpszMenuName =NULL;
    wndclassW.lpszClassName=APP_MDI_CLASSW;
    if (!RegisterClassWide(&wndclassW)) goto Quit;
  }
  else if (nMDI == WMD_SDI)
  {
    wndclassW.style        =0;
    wndclassW.lpfnWndProc  =DummyProc;
    wndclassW.cbClsExtra   =0;
    wndclassW.cbWndExtra   =DLGWINDOWEXTRA;
    wndclassW.hInstance    =hInstance;
    wndclassW.hIcon        =NULL;
    wndclassW.hCursor      =NULL;
    wndclassW.hbrBackground=NULL;
    wndclassW.lpszMenuName =NULL;
    wndclassW.lpszClassName=APP_SDI_CLASSW;
    if (!RegisterClassWide(&wndclassW)) goto Quit;
  }
  #ifndef AKELPAD_DLLBUILD
    EnsureWindowInMonitor(&moCur.rcMainWindowRestored);
    mc.x=moCur.rcMainWindowRestored.left;
    mc.y=moCur.rcMainWindowRestored.top;
    mc.nWidth=moCur.rcMainWindowRestored.right;
    mc.nHeight=moCur.rcMainWindowRestored.bottom;
    mc.hMenu=hMainMenu;
  #endif

  hMainWnd=CreateWindowExWide(0,
                              wszMainClass,    // window class name
                              APP_MAIN_TITLEW, // window caption
                              mc.dwStyle,      // window style
                              mc.x,            // initial x position
                              mc.y,            // initial y position
                              mc.nWidth,       // initial x size
                              mc.nHeight,      // initial y size
                              mc.hWndParent,   // parent window handle
                              mc.hMenu,        // window menu handle
                              hInstance,       // program instance handle
                              NULL);           // creation parameters
  if (!hMainWnd) goto Quit;

  #ifndef AKELPAD_DLLBUILD
  {
    //Message loop
    MSG msg;
    BOOL bMsgStatus;

    while ((bMsgStatus=GetMessageWide(&msg, NULL, 0, 0)) && bMsgStatus != -1)
    {
      TranslateMessageAll(TMSG_ALL, &msg);

      if (bMainCheckIdle)
      {
        if (GetQueueStatus(QS_ALLINPUT) == 0)
        {
          bMainCheckIdle=FALSE;
          SendMessage(hMainWnd, AKDN_MAIN_ONSTART_IDLE, 0, 0);
        }
      }
    }
    if (bMsgStatus == -1)
    {
      API_LoadString(hLangModule, MSG_ERROR_IN_MESSAGE_QUEUE, wszMsg, BUFFER_SIZE);
      API_MessageBox(NULL, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    }
  }
  #endif

  Quit:
  #ifndef AKELPAD_DLLBUILD
    WinMainCleanUp();

    if (hMutex)
    {
      SetEvent(hMutex);
      CloseHandle(hMutex);
      hMutex=0;
    }

    ExitProcess(0);
  #endif

  return 0;
}

void WinMainCleanUp()
{
  if (wpCmdLineBegin)
  {
    API_HeapFree(hHeap, 0, (LPVOID)wpCmdLineBegin);
    wpCmdLineBegin=NULL;
    nCmdLineBeginLen=0;
  }
  if (wpCmdLineEnd)
  {
    API_HeapFree(hHeap, 0, (LPVOID)wpCmdLineEnd);
    wpCmdLineEnd=NULL;
    nCmdLineEndLen=0;
  }
  if (hIconShieldAkelAdmin)
  {
    DestroyIcon(hIconShieldAkelAdmin);
    hIconShieldAkelAdmin=NULL;
  }
  AkelAdminExit();
  CodepageListFree(&lpCodepageList);
  RecentFilesZero(&hRecentFilesStack);
  SearchZero(&hFindStack);
  SearchZero(&hReplaceStack);
  SearchFreeLast();
  StackStatusPartFree(&hStatusStack);
  StackFontItemsFree(&hFontsStack);
  StackBkImageFree(&hBkImagesStack);
  StackModelessFree(&hEnumModelessStack);
  StackModelessFree(&hModelessStack);
  StackDockFree(&hDocksStack);
  StackThemeFree(&hThemesStack);
  StackFramesFree(&hFramesStack);
  StackPluginFree(&hPluginsStack);
  StackProcFree(&hMainProcStack);
  StackProcFree(&hMainProcRetStack);
  StackProcFree(&hEditProcStack);
  StackProcFree(&hEditProcRetStack);
  StackProcFree(&hFrameProcStack);
  StackProcFree(&hFrameProcRetStack);
  if (hLangModule && hLangModule != hInstance) FreeLibrary(hLangModule);
  #ifdef AKELEDIT_STATICBUILD
    AE_UnregisterClassA(hInstance);
    AE_UnregisterClassW(hInstance);
    OleUninitialize();
  #else
    if (hAkelLib) FreeLibrary(hAkelLib);
  #endif
}

LRESULT CALLBACK CommonMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (uMsg >= WM_USER)
  {
    if (uMsg == AKD_GETMAINPROC)
    {
      return StackProcGet(&hMainProcStack, (int)wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETMAINPROC)
    {
      return StackProcSet(&hMainProcStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, &lpfnMainProc);
    }
    else if (uMsg == AKD_GETMAINPROCRET)
    {
      return StackProcGet(&hMainProcRetStack, (int)wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETMAINPROCRET)
    {
      return StackProcSet(&hMainProcRetStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, (WNDPROC *)&lpfnMainProcRet);
    }
    else if (uMsg == AKD_GETEDITPROC)
    {
      return StackProcGet(&hEditProcStack, (int)wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETEDITPROC)
    {
      return StackProcSet(&hEditProcStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, &lpfnEditProc);
    }
    else if (uMsg == AKD_GETEDITPROCRET)
    {
      return StackProcGet(&hEditProcRetStack, (int)wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETEDITPROCRET)
    {
      return StackProcSet(&hEditProcRetStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, (WNDPROC *)&lpfnEditProcRet);
    }
    else if (uMsg == AKD_GETFRAMEPROC)
    {
      return StackProcGet(&hFrameProcStack, (int)wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETFRAMEPROC)
    {
      return StackProcSet(&hFrameProcStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, &lpfnFrameProc);
    }
    else if (uMsg == AKD_GETFRAMEPROCRET)
    {
      return StackProcGet(&hFrameProcRetStack, (int)wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETFRAMEPROCRET)
    {
      return StackProcSet(&hFrameProcRetStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, (WNDPROC *)&lpfnFrameProcRet);
    }
    else if (uMsg == AKD_DLLCALL ||
             uMsg == AKD_DLLCALLA ||
             uMsg == AKD_DLLCALLW)
    {
      PLUGINCALLSENDW *lpCallSend=(PLUGINCALLSENDW *)lParam;
      wchar_t wszPluginFunction[MAX_PATH];

      if (uMsg == AKD_DLLCALLA || (bOldWindows && uMsg == AKD_DLLCALL))
        xprintfW(wszPluginFunction, L"%S", (char *)lpCallSend->pFunction);
      else
        xprintfW(wszPluginFunction, L"%s", (wchar_t *)lpCallSend->pFunction);
      return CallPluginSend(NULL, wszPluginFunction, lpCallSend, (DWORD)wParam);
    }
    else if (uMsg == AKD_DLLUNLOAD)
    {
      PLUGINUNLOADPOST *pup;
      PLUGINHANDLE *phElement;

      if (phElement=StackHandleGet(&hHandlesStack, (HMODULE)wParam, NULL))
      {
        if (pup=(PLUGINUNLOADPOST *)GlobalAlloc(GPTR, sizeof(PLUGINUNLOADPOST)))
        {
          pup->phElement=phElement;
          pup->nCallCount=phElement->nCallCount;
          pup->hModule=(HMODULE)wParam;
          pup->hThread=(HANDLE)lParam;
          PostMessage(hMainWnd, AKD_DLLUNLOAD, 0, (LPARAM)pup);
        }
        return TRUE;
      }
      return FALSE;
    }
    else if (uMsg == AKD_DLLFIND ||
             uMsg == AKD_DLLFINDA ||
             uMsg == AKD_DLLFINDW)
    {
      wchar_t wszPluginFunction[MAX_PATH];
      int nPluginFunctionLen;

      if (wParam)
      {
        if (uMsg == AKD_DLLFINDA || (bOldWindows && uMsg == AKD_DLLFIND))
          nPluginFunctionLen=(int)xprintfW(wszPluginFunction, L"%S", (char *)wParam);
        else
          nPluginFunctionLen=(int)xprintfW(wszPluginFunction, L"%s", (wchar_t *)wParam);
        return (LRESULT)StackPluginFind(&hPluginsStack, wszPluginFunction, nPluginFunctionLen);
      }
      if (lParam)
      {
        return (LRESULT)StackHotkeyFind(&hPluginsStack, (WORD)lParam);
      }
      return 0;
    }
    else if (uMsg == AKD_DLLADD ||
             uMsg == AKD_DLLADDA ||
             uMsg == AKD_DLLADDW)
    {
      PLUGINADDW *pa=(PLUGINADDW *)lParam;
      wchar_t wszPluginFunction[MAX_PATH];
      int nPluginFunctionLen;

      if (uMsg == AKD_DLLADDA || (bOldWindows && uMsg == AKD_DLLADD))
        nPluginFunctionLen=(int)xprintfW(wszPluginFunction, L"%S", (char *)pa->pFunction);
      else
        nPluginFunctionLen=(int)xprintfW(wszPluginFunction, L"%s", (wchar_t *)pa->pFunction);
      return (LRESULT)StackPluginAdd(&hPluginsStack, wszPluginFunction, nPluginFunctionLen, pa->wHotkey, pa->bAutoLoad, pa->PluginProc, pa->lpParameter);
    }
    else if (uMsg == AKD_DLLDELETE)
    {
      StackPluginDelete(&hPluginsStack, (void *)lParam);
      return 0;
    }
    else if (uMsg == AKD_DLLSAVE)
    {
      if (wParam & DLLSF_NOW)
      {
        return StackPluginSave(&hPluginsStack, moCur.nSaveSettings);
      }
      if (wParam & DLLSF_ONEXIT)
      {
        bSavePluginsStackOnExit=TRUE;
        return TRUE;
      }
    }
    else if (uMsg == AKD_CHECKHOTKEY)
    {
      return CheckHotkey((WORD)wParam, (wchar_t *)lParam);
    }
  }

  lResult=lpfnMainProc(hWnd, uMsg, wParam, lParam);

  if (uMsg == AKDN_MAIN_ONFINISH)
  {
    nMainOnFinish=MOF_DESTROY;

    //Unload plugins
    StackHandleFree(&hHandlesStack);

    //Destroy windows
    if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
    {
      //Destroy virtual edits
      SplitDestroy(lpFrameCurrent, CN_CLONE1|CN_CLONE2|CN_CLONE3);

      if (nMDI == WMD_PMDI)
      {
        //Destroy last empty tab
        SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrameCurrent->ei.hWndEdit, (LPARAM)lpFrameCurrent->ei.hDocEdit);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETDOCUMENT, (WPARAM)fdDefault.ei.hDocEdit, FALSE);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_DELETEDOCUMENT, (WPARAM)lpFrameCurrent->ei.hDocEdit, 0);
      }

      //Destroy real edits
      SplitDestroy(&fdDefault, CN_CLONE1|CN_CLONE2|CN_CLONE3);

      bEditOnFinish=TRUE;
      SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)fdDefault.ei.hWndEdit, 0);
      DestroyWindow(fdDefault.ei.hWndEdit);
      fdDefault.ei.hWndEdit=NULL;
      fdDefault.ei.hDocEdit=NULL;
      bEditOnFinish=FALSE;

      if (nMDI == WMD_SDI)
      {
        CopyFrameSettings(&fdDefault, lpFrameCurrent);
      }
      StackFrameDelete(&hFramesStack, lpFrameCurrent);
    }
    else if (nMDI == WMD_MDI)
    {
      DestroyWindow(hMdiClient);
      hMdiClient=NULL;
    }
    SendMessage(hMainWnd, AKDN_MAIN_ONDESTROY, 0, 0);
    DestroyWindow(hMainWnd);
    hMainWnd=NULL;
  }

  if (lpfnMainProcRet)
  {
    CWPRETSTRUCT cwprs;

    cwprs.lResult=lResult;
    cwprs.lParam=lParam;
    cwprs.wParam=wParam;
    cwprs.message=uMsg;
    cwprs.hwnd=hWnd;
    lpfnMainProcRet(&cwprs);
  }
  return lResult;
}

LRESULT CALLBACK MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CREATE)
  {
    int *lpSBParts;
    int iBorders[3];
    CLIENTCREATESTRUCT ccs;
    DWORD dwClassStyle;
    RECT rcRect;
    HMENU hMenu;

    hMainWnd=hWnd;
    hPopupEdit=GetSubMenu(hPopupMenu, MENU_POPUP_EDIT);
    hPopupView=GetSubMenu(hPopupMenu, MENU_POPUP_VIEW);
    hPopupCodepage=GetSubMenu(hPopupMenu, MENU_POPUP_CODEPAGE);
    hPopupOpenCodepage=GetSubMenu(hPopupCodepage, MENU_POPUP_CODEPAGE_OPEN);
    hPopupSaveCodepage=GetSubMenu(hPopupCodepage, MENU_POPUP_CODEPAGE_SAVE);
    hPopupHeadline=GetSubMenu(hPopupMenu, MENU_POPUP_HEADLINE);

    hMenu=GetSubMenu(hMainMenu, MENU_FILE_POSITION);
    hMenuRecentFiles=GetSubMenu(hMenu, MENU_FILE_RECENTFILES_4X);
    hMenu=GetSubMenu(hMainMenu, MENU_VIEW_POSITION);
    hMenuLanguage=GetSubMenu(hMenu, MENU_VIEW_LANGUAGE_4X);

    if (!nMDI)
    {
      DeleteMenu(hMainMenu, IDM_FILE_SAVEALL, MF_BYCOMMAND);
      DeleteMenu(hMainMenu, IDM_FILE_SAVEALLAS, MF_BYCOMMAND);
      DeleteMenu(hMainMenu, IDM_OPTIONS_SINGLEOPEN_PROGRAM, MF_BYCOMMAND);
      DeleteMenu(hMainMenu, MENU_MDI_POSITION, MF_BYPOSITION);
      DrawMenuBar(hWnd);
      DragAcceptFiles(hWnd, TRUE);

      hDummyWindow=CreateWindowExWide(0,
                                      APP_SDI_CLASSW,        // window class name
                                      APP_SDI_TITLEW,        // window caption
                                      WS_POPUP,              // window style
                                      CW_USEDEFAULT,         // initial x position
                                      CW_USEDEFAULT,         // initial y position
                                      CW_USEDEFAULT,         // initial x size
                                      CW_USEDEFAULT,         // initial y size
                                      hWnd,                  // parent window handle
                                      NULL,                  // window menu handle
                                      hInstance,             // program instance handle
                                      NULL);                 // creation parameters
    }
    else
    {
      hMenuWindow=GetSubMenu(hMainMenu, MENU_MDI_POSITION);
      GetClientRect(hWnd, &rcRect);

      //MDI Client
      if (nMDI == WMD_MDI)
      {
        ccs.hWindowMenu=hMenuWindow;
        ccs.idFirstChild=ID_FIRSTMDI;

        hMdiClient=CreateWindowExWide(0,
                                      L"MdiClient",
                                      NULL,
                                      WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VSCROLL|WS_HSCROLL,
                                      rcRect.left, rcRect.top, rcRect.right, rcRect.bottom,
                                      hWnd,
                                      NULL,
                                      hInstance,
                                      (LPVOID)&ccs);

        dwClassStyle=(DWORD)GetClassLongPtrWide(hMdiClient, GCL_STYLE);
        SetClassLongPtrWide(hMdiClient, GCL_STYLE, dwClassStyle|CS_DBLCLKS);

        lpOldMdiClientProc=(WNDPROC)GetWindowLongPtrWide(hMdiClient, GWLP_WNDPROC);
        SetWindowLongPtrWide(hMdiClient, GWLP_WNDPROC, (UINT_PTR)NewMdiClientProc);

        InsertMenuWide(hMainMenu, IDM_WINDOW_FRAMECLOSE, MF_BYCOMMAND|MF_SEPARATOR, 0, NULL);
        DeleteMenu(hMainMenu, IDM_WINDOW_MDILIST, MF_BYCOMMAND);
        DragAcceptFiles(hMdiClient, TRUE);
      }

      //PMDI
      if (nMDI == WMD_PMDI)
      {
        DeleteMenu(hMainMenu, IDM_WINDOW_TILEHORIZONTAL, MF_BYCOMMAND);
        DeleteMenu(hMainMenu, IDM_WINDOW_TILEVERTICAL, MF_BYCOMMAND);
        DeleteMenu(hMainMenu, IDM_WINDOW_CASCADE, MF_BYCOMMAND);
        InsertMenuWide(hMainMenu, IDM_WINDOW_MDILIST, MF_BYCOMMAND|MF_SEPARATOR, 0, NULL);
        DragAcceptFiles(hWnd, TRUE);
      }

      //Tab Control
      hTab=CreateWindowExWide(0,
                              L"SysTabControl32",
                              NULL,
                              WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|TCS_SINGLELINE|TCS_FOCUSNEVER|TCS_HOTTRACK|
                                ((moCur.dwTabOptionsMDI & TAB_TYPE_STANDARD)?TCS_TABS:(TCS_BUTTONS|TCS_FLATBUTTONS)),
                              rcRect.left, rcRect.top, rcRect.right, TAB_HEIGHT,
                              hWnd,
                              (HMENU)(UINT_PTR)ID_TAB,
                              hInstance,
                              0);

      lpOldTabProc=(WNDPROC)GetWindowLongPtrWide(hTab, GWLP_WNDPROC);
      SetWindowLongPtrWide(hTab, GWLP_WNDPROC, (UINT_PTR)NewTabProc);

      SendMessage(hTab, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
      hImageList=ImageList_Create(16 /*GetSystemMetrics(SM_CXSMICON)*/, 16 /*GetSystemMetrics(SM_CYSMICON)*/, ILC_COLOR32|ILC_MASK, 0, 0);
      ImageList_SetBkColor(hImageList, CLR_NONE);
      SendMessage(hTab, TCM_SETIMAGELIST, 0, (LPARAM)hImageList);
      SendMessage(hTab, TCM_SETEXTENDEDSTYLE, 0, TCS_EX_FLATSEPARATORS|TCS_EX_REGISTERDROP);

      //Insert hIconEmpty to ImageList as first element
      {
        ASSOCICON *ai;

        if (ai=StackIconInsert(&hIconsStack, NULL, 0))
        {
          ai->hIcon=hIconEmpty;
          ImageList_AddIcon(hImageList, ai->hIcon);
        }
      }
    }

    //Status Bar
    hStatus=CreateWindowExWide(0,
                               L"msctls_statusbar32",
                               NULL,
                               WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|((mc.dwStyle & WS_CHILD)?0:SBARS_SIZEGRIP),
                               0, 0, 0, 0,
                               hWnd,
                               (HMENU)(UINT_PTR)ID_STATUS,
                               hInstance,
                               NULL);

    SendMessage(hStatus, SB_SIMPLE, FALSE, 0);
    GetWindowRect(hStatus, &rcRect);
    nStatusHeight=rcRect.bottom - rcRect.top;
    lpSBParts=SetStatusParts(&hStatusStack);

    //Progress Bar
    SendMessage(hStatus, SB_GETBORDERS, 0, (LPARAM)&iBorders);
    nProgressWidth=(lpSBParts[SBP_MODIFY] - lpSBParts[SBP_POSITION]) - iBorders[2];

    hProgress=CreateWindowExWide(0,
                                 L"msctls_progress32",
                                 NULL,
                                 WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|PBS_SMOOTH,
                                 lpSBParts[SBP_POSITION] + iBorders[2], iBorders[1], nProgressWidth, nStatusHeight - iBorders[1],
                                 hStatus,
                                 (HMENU)(UINT_PTR)ID_PROGRESS,
                                 hInstance,
                                 NULL);

    //Initialize status
    ssStatus.bModified=-1;
    ssStatus.bOvertypeMode=-1;
    ssStatus.nNewLine=-1;
    ssStatus.nCodePage=-1;
    ssStatus.bBOM=-1;
    API_HeapFree(hHeap, 0, (LPVOID)lpSBParts);

    PostMessage(hWnd, AKDN_MAIN_ONSTART, 0, 0);
  }

  if (uMsg >= WM_USER)
  {
    switch (uMsg)
    {
      case AKDN_MAIN_ONSTART:
      {
        NMAINSHOW nms;
        RECT rcRect;
        wchar_t *wpFileName;
        int i=0;

        bMainOnStart=TRUE;

        //Call plugins on start
        CallPluginsOnStart(&hPluginsStack);

        //AkelPad support 3 window modes:
        //SDI:
        //  - Maximum edit windows in SDI mode is 4. At next step we will create first one, later when window will be split
        //  we will create another 3 windows. Those 1 or 4 windows will be destroyed only on program exit.
        //  - SDI mode has one allocated FRAMEDATA.
        //  - fdDefault structure keeps default settings and also all created edit windows.
        //MDI:
        //  - Maximum edit windows in MDI mode is limited by GDI buffer. During tests this number is about 800-1000 windows.
        //  - MDI mode allocated FRAMEDATA count is equal to number of tabs (MDI client frame windows).
        //  - Each tab is 1 MDI client frame window and 1 edit window (or 4 if window split).
        //  - fdDefault structure keeps default settings, but not edit windows.
        //PMDI:
        //  - Allows you to open an unlimited number of files. In contrast to the MDI mode, opening a new tab does not lead
        //  creation of new graphical objects, thus avoid GDI buffer overflow.
        //  - Maximum edit windows in PMDI mode is 4. At next step we will create first one, later when window will be split
        //  we will create another 3 windows. Those 1 or 4 windows will be destroyed only on program exit.
        //  - PMDI mode allocated FRAMEDATA count is equal to number of tabs.
        //  - fdDefault structure keeps default settings and also all created real edit windows.
        //  - If you want to send any message to non-active frame in PMDI mode, you should activate it first with AKD_FRAMEACTIVATE or use special message AEM_SENDMESSAGE.

        if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
        {
          fdDefault.ei.hWndEdit=(HWND)CreateEditWindow(hMainWnd, NULL);
          fdDefault.lpEditProc=(AEEditProc)SendMessage(fdDefault.ei.hWndEdit, AEM_GETDOCUMENTPROC, (WPARAM)NULL, 0);
          fdDefault.ei.hDocEdit=(AEHDOC)SendMessage(fdDefault.ei.hWndEdit, AEM_GETDOCUMENT, 0, 0);

          if (nMDI == WMD_SDI)
          {
            if (lpFrameCurrent=CreateFrameData(hMainWnd, &fdDefault))
            {
              lpFrameCurrent->ei.hWndEdit=fdDefault.ei.hWndEdit;
              lpFrameCurrent->lpEditProc=fdDefault.lpEditProc;
              lpFrameCurrent->ei.hDocEdit=fdDefault.ei.hDocEdit;
              RestoreFrameData(lpFrameCurrent, 0);
              SetEditWindowSettings(lpFrameCurrent);
              SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrameCurrent->ei.hWndEdit, (LPARAM)lpFrameCurrent->ei.hDocEdit);
            }
          }
          else if (nMDI == WMD_PMDI)
          {
            CreateFrameWindow(NULL);
          }
        }
        else if (nMDI == WMD_MDI)
        {
          //Create MDI client frame window. In WM_CREATE of the frame procedure edit window will be created.
          if (moCur.dwMainStyle == WS_MAXIMIZE && !(mc.dwStyle & WS_CHILD))
          {
            rcRect.left=0;
            rcRect.top=0;
            rcRect.right=GetSystemMetrics(SM_CXMAXIMIZED);
            rcRect.bottom=GetSystemMetrics(SM_CYMAXIMIZED);
          }
          else GetClientRect(hMdiClient, &rcRect);

          rcRect.right-=(rcRect.right * 14 / 100);
          rcRect.bottom-=(rcRect.bottom * 45 / 100);
          CreateFrameWindow(&rcRect);
        }

        //Apply settings
        if (moCur.bOnTop)
          DoViewOnTop(moCur.bOnTop);
        if (moCur.bStatusBar)
          DoViewShowStatusBar(moCur.bStatusBar);
        DoSettingsSaveTime(moCur.bSaveTime);
        DoSettingsKeepSpace(moCur.bKeepSpace);
        DoSettingsWatchFile(moCur.bWatchFile);
        DoSettingsSingleOpenFile(moCur.bSingleOpenFile);
        if (!SearchPathWide(NULL, L"charmap.exe", NULL, BUFFER_SIZE, wbuf, &wpFileName))
          EnableMenuItem(hMainMenu, IDM_EDIT_INSERTCHAR, MF_GRAYED);

        if (nMDI == WMD_SDI)
          i=IDM_OPTIONS_SDI;
        else if (nMDI == WMD_MDI)
          i=IDM_OPTIONS_MDI;
        else if (nMDI == WMD_PMDI)
          i=IDM_OPTIONS_PMDI;
        CheckMenuRadioItem(hMainMenu, IDM_OPTIONS_SDI, IDM_OPTIONS_PMDI, i, MF_BYCOMMAND);

        if (nMDI)
        {
          DoSettingsSingleOpenProgram(moCur.dwSingleOpenProgram);
          DoWindowTabView(moCur.dwTabOptionsMDI, TRUE);
          DoWindowTabType(moCur.dwTabOptionsMDI, TRUE);
          if (bOldComctl32) EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_FLATBUTTONS, MF_GRAYED);
          DoWindowTabSwitch(moCur.dwTabOptionsMDI, TRUE);
        }

        //PreShow
        nms.dwStyle=&moCur.dwMainStyle;
        nms.dwShow=&dwCmdShow;
        nms.bProcess=TRUE;
        SendMessage(hMainWnd, AKDN_MAIN_ONSTART_PRESHOW, 0, (LPARAM)&nms);
        if (nms.bProcess)
        {
          if (!(mc.dwStyle & WS_VISIBLE))
          {
            if (dwCmdShow == SW_SHOWNORMAL || dwCmdShow == SW_SHOW || dwCmdShow == SW_SHOWMAXIMIZED)
            {
              if (moCur.dwMainStyle == WS_MAXIMIZE || dwCmdShow == SW_SHOWMAXIMIZED)
                ShowWindow(hMainWnd, SW_SHOWMAXIMIZED);
              else
                ShowWindow(hMainWnd, SW_SHOW);
            }
            else
            {
              //Shortcut
              if (moCur.dwMainStyle == WS_MAXIMIZE)
              {
                //Maximize window without showing it
                WINDOWPLACEMENT wp;
                UINT_PTR dwStyle;

                wp.length=sizeof(WINDOWPLACEMENT);
                GetWindowPlacement(hMainWnd, &wp);
                dwStyle=GetWindowLongPtrWide(hMainWnd, GWL_STYLE);
                SetWindowLongPtrWide(hMainWnd, GWL_STYLE, dwStyle|WS_MAXIMIZE);
                wp.showCmd=WS_MAXIMIZE;
                SetWindowPlacement(hMainWnd, &wp);
                SetWindowPos(hMainWnd, NULL, 0, 0, GetSystemMetrics(SM_CXMAXIMIZED), GetSystemMetrics(SM_CYMAXIMIZED), SWP_NOZORDER|SWP_NOACTIVATE);
              }
              ShowWindow(hMainWnd, dwCmdShow);
            }
          }
          if (mc.dwStyle & WS_CHILD)
            UpdateSize();

          //Update main window
          UpdateWindow(hMainWnd);
        }
        SendMessage(hMainWnd, AKDN_MAIN_ONSTART_SHOW, 0, 0);

        //Parse commmand line on show
        if (wpCmdLine)
        {
          int nResult=ParseCmdLine(&wpCmdLine, PCL_ONSHOW);

          wpCmdLine=NULL;
          if (nResult == PCLE_QUIT)
            PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
        }
        SetCurrentDirectoryWide(wszExeDir);

        #ifndef AKELPAD_DLLBUILD
          if (hMutex)
          {
            SetEvent(hMutex);
            CloseHandle(hMutex);
            hMutex=0;
          }
        #endif

        SendMessage(hMainWnd, AKDN_MAIN_ONSTART_FINISH, 0, 0);
        bMainOnStart=FALSE;
        bMainCheckIdle=TRUE;
        return 0;
      }
      case AKDN_FRAME_ACTIVATE:
      {
        if (!bEditOnFinish)
        {
          if (hDlgModeless && nModelessType != MLT_CUSTOM)
            SendMessage(hDlgModeless, WM_COMMAND, IDC_SETREADONLY, 0);

          if (IsAllowWatchFile(lpFrameCurrent) && !bCheckingWriteTime)
            PostMessage(hMainWnd, WM_COMMAND, IDM_INTERNAL_CHECKWRITETIME, (LPARAM)lpFrameCurrent);
        }
        return 0;
      }
      case AKDN_POSTDOCUMENT_START:
      {
        if ((FRAMEDATA *)wParam == lpFrameCurrent)
        {
          SendMessage(hMainWnd, AKDN_POSTDOCUMENT_FINISH, wParam, lParam);
        }
        return 0;
      }

      //Command line
      case AKD_GETCMDLINEOPTIONS:
      {
        return dwCmdLineOptions;
      }
      case AKD_SETCMDLINEOPTIONS:
      {
        dwCmdLineOptions=(DWORD)wParam;
        return 0;
      }
      case AKD_PARSECMDLINEW:
      {
        PARSECMDLINESENDW *pcls=(PARSECMDLINESENDW *)lParam;
        int nResult;

        if (pcls->pWorkDir && *pcls->pWorkDir)
          SetCurrentDirectoryWide(pcls->pWorkDir);
        nResult=ParseCmdLine(&pcls->pCmdLine, PCL_ONMESSAGE);
        if (pcls->pWorkDir && *pcls->pWorkDir)
          SetCurrentDirectoryWide(wszExeDir);
        if (!bCmdLineQuitAsEnd && nResult == PCLE_QUIT)
          PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
        bCmdLineQuitAsEnd=FALSE;

        return nResult;
      }
      case AKD_METHODPARSEPARAMETERS:
      {
        STACKEXTPARAM *hParamStack=(STACKEXTPARAM *)wParam;
        const wchar_t **wppText=(const wchar_t **)lParam;

        return MethodParseParameters(hParamStack, *wppText, wppText);
      }
      case AKD_METHODEXPANDPARAMETERS:
      {
        STACKEXTPARAM *hParamStack=(STACKEXTPARAM *)wParam;
        EXPPARAM *ep=(EXPPARAM *)lParam;

        MethodExpandParameters(hParamStack, ep);
        return 0;
      }
      case AKD_METHODGETPARAMETER:
      {
        STACKEXTPARAM *hParamStack=(STACKEXTPARAM *)wParam;

        return (LRESULT)MethodGetParameter(hParamStack, (int)lParam);
      }
      case AKD_METHODSTRUCTPARAMETERS:
      {
        STACKEXTPARAM *hParamStack=(STACKEXTPARAM *)wParam;
        unsigned char *lpStructure=(unsigned char *)lParam;

        return MethodStructParameters(hParamStack, lpStructure);
      }
      case AKD_METHODFREEPARAMETERS:
      {
        STACKEXTPARAM *hParamStack=(STACKEXTPARAM *)wParam;

        MethodFreeParameters(hParamStack);
        return 0;
      }
      case AKD_IFEXPRESSION:
      {
        STACKEXTPARAM hParamStack={0};
        STACKEXTPARAM *lpParamStack=&hParamStack;
        IFEXPRESSION *ie=(IFEXPRESSION *)lParam;
        const wchar_t *wpExpression=(const wchar_t *)wParam;
        const wchar_t *wpParamStr;
        const wchar_t *wpCount=wpExpression;
        const wchar_t *wpNext=NULL;
        const wchar_t *wpStop;
        DWORD dwFlags=0;
        int nError=0;
        INT_PTR nResult=0;

        if (ie) dwFlags=ie->dwFlags;

        if (!(dwFlags & IEF_IF))
        {
          if ((dwFlags & IEF_STACKEXTPARAM) && !(dwFlags & IEF_PARSEONLY))
            nError=IEE_UNKNOWNMETHOD;
          else if (MethodGetName(wpCount, wbuf, BUFFER_SIZE, &wpNext))
          {
            if (!xstrcmpiW(wbuf, L"If"))
            {
              dwFlags|=IEF_IF;
              wpCount=wpNext;
            }
            else nError=IEE_UNKNOWNMETHOD;
          }
          else nError=IEE_UNKNOWNMETHOD;
        }
        if (dwFlags & IEF_IF)
        {
          if (dwFlags & IEF_STACKEXTPARAM)
            lpParamStack=ie->sep;
          if (!(dwFlags & IEF_STACKEXTPARAM) || (dwFlags & IEF_PARSEONLY))
            MethodParseParameters(lpParamStack, wpCount, &wpNext);

          if (!(dwFlags & IEF_PARSEONLY) && lpParamStack->first)
          {
            if (dwFlags & IEF_IF)
            {
              if (lpParamStack->first->wpExpanded)
                wpParamStr=lpParamStack->first->wpExpanded;
              else
                wpParamStr=lpParamStack->first->wpString;
              if (wpParamStr)
              {
                nResult=IfExpression(wpParamStr, &wpStop, &nError);
                if (!(dwFlags & IEF_STACKEXTPARAM))
                  wpCount=wpExpression + (wpStop - wpParamStr) + (wpCount - wpExpression);
                else
                  wpCount=wpStop;
              }
            }
            if (!(dwFlags & IEF_STACKEXTPARAM))
              MethodFreeParameters(lpParamStack);
          }
        }
        if (ie)
        {
          ie->dwFlags|=dwFlags;
          ie->nError=nError;
          if (!nError && wpNext)
            ie->wpEnd=wpNext;
          else
            ie->wpEnd=wpCount;
        }
        return nResult;
      }

      //Text retrieval and modification
      case AKD_DETECTCASE:
      {
        HWND hWnd=(HWND)wParam;

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        return DetectCase(hWnd, (AECHARRANGE *)lParam);
      }
      case AKD_CONVERTCASE:
      {
        CONVERTCASE *cc=(CONVERTCASE *)lParam;

        ConvertCase(cc->wszText, cc->nTextLen, cc->nCase);
        return 0;
      }
      case AKD_DETECTANSITEXT:
      {
        DETECTANSITEXT *dat=(DETECTANSITEXT *)lParam;

        return AutodetectMultibyte(dat->dwLangID, (unsigned char *)dat->pText, dat->nTextLen, dat->nMinChars?dat->nMinChars:DETECTCHARS_REQUIRED, &dat->nCodePage);
      }
      case AKD_DETECTUNITEXT:
      {
        DETECTUNITEXT *dut=(DETECTUNITEXT *)lParam;

        return AutodetectWideChar(dut->dwLangID, dut->wpText, dut->nTextLen, dut->nMinChars?dut->nMinChars:DETECTCHARS_REQUIRED, &dut->nCodePageFrom, &dut->nCodePageTo);
      }
      case AKD_CONVERTANSITEXT:
      {
        CONVERTANSITEXT *cat=(CONVERTANSITEXT *)lParam;

        return ConvertAnsiString(cat->pInput, cat->nInputLen, cat->nCodePageFrom, cat->nCodePageTo, &cat->szOutput, &cat->nOutputLen);
      }
      case AKD_CONVERTUNITEXT:
      {
        CONVERTUNITEXT *cut=(CONVERTUNITEXT *)lParam;

        return ConvertWideString(cut->wpInput, cut->nInputLen, cut->nCodePageFrom, cut->nCodePageTo, &cut->wszOutput, &cut->nOutputLen);
      }
      case AKD_DETECTFILE:
      case AKD_DETECTFILEA:
      case AKD_DETECTFILEW:
      {
        DETECTFILEW *dc=(DETECTFILEW *)lParam;
        wchar_t *wpFile;
        int nResult;

        if (uMsg == AKD_DETECTFILEA || (bOldWindows && uMsg == AKD_DETECTFILE))
          wpFile=AllocWide((char *)dc->pFile);
        else
          wpFile=(wchar_t *)dc->pFile;

        nResult=AutodetectCodePage(wpFile, NULL, dc->dwBytesToCheck, dc->dwFlags, &dc->nCodePage, &dc->bBOM);

        if (uMsg == AKD_DETECTFILEA || (bOldWindows && uMsg == AKD_DETECTFILE))
          FreeWide(wpFile);
        return nResult;
      }
      case AKD_READFILECONTENT:
      {
        FILECONTENT *fc=(FILECONTENT *)lParam;

        return ReadFileContent(fc->hFile, fc->dwMax, fc->nCodePage, fc->bBOM, &fc->wpContent);
      }
      case AKD_WRITEFILECONTENT:
      {
        FILECONTENT *fc=(FILECONTENT *)lParam;

        return WriteFileContent(fc->hFile, fc->wpContent, fc->dwMax, fc->nCodePage, fc->bBOM);
      }
      case AKD_OPENDOCUMENT:
      case AKD_OPENDOCUMENTA:
      case AKD_OPENDOCUMENTW:
      {
        OPENDOCUMENTW *od=(OPENDOCUMENTW *)lParam;
        wchar_t *wpFile=API_AllocWide(MAX_PATH);
        wchar_t *wpWorkDir=API_AllocWide(MAX_PATH);
        int nResult=0;

        if (((HWND)wParam && !IsEditActive((HWND)wParam)) || nMDI || SaveChanged(0))
        {
          if (uMsg == AKD_OPENDOCUMENTA || (bOldWindows && uMsg == AKD_OPENDOCUMENT))
          {
            xprintfW(wpFile, L"%S", (char *)od->pFile);
            xprintfW(wpWorkDir, L"%S", (char *)od->pWorkDir);
          }
          else
          {
            xprintfW(wpFile, L"%s", (wchar_t *)od->pFile);
            xprintfW(wpWorkDir, L"%s", (wchar_t *)od->pWorkDir);
          }

          if (*wpWorkDir) SetCurrentDirectoryWide(wpWorkDir);
          nResult=OpenDocument((HWND)wParam, od->hDoc, wpFile, od->dwFlags, od->nCodePage, od->bBOM);
          if (*wpWorkDir) SetCurrentDirectoryWide(wszExeDir);
        }
        API_FreeWide(wpFile);
        API_FreeWide(wpWorkDir);
        return nResult;
      }
      case AKD_SAVEDOCUMENT:
      case AKD_SAVEDOCUMENTA:
      case AKD_SAVEDOCUMENTW:
      {
        SAVEDOCUMENTW *sd=(SAVEDOCUMENTW *)lParam;
        wchar_t *wpFile=API_AllocWide(MAX_PATH);
        int nResult;

        if (uMsg == AKD_SAVEDOCUMENTA || (bOldWindows && uMsg == AKD_SAVEDOCUMENT))
          xprintfW(wpFile, L"%S", (char *)sd->pFile);
        else
          xprintfW(wpFile, L"%s", (wchar_t *)sd->pFile);
        nResult=SaveDocument((HWND)wParam, sd->hDoc, wpFile, sd->nCodePage, sd->bBOM, sd->dwFlags);

        API_FreeWide(wpFile);
        return nResult;
      }
      case AKD_GETTEXTLENGTH:
      {
        HWND hWnd=(HWND)wParam;

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        return GetTextLength(hWnd);
      }
      case AKD_GETTEXTRANGE:
      case AKD_GETTEXTRANGEA:
      case AKD_GETTEXTRANGEW:
      {
        GETTEXTRANGE *gtr=(GETTEXTRANGE *)lParam;
        HWND hWnd=(HWND)wParam;

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        if (uMsg == AKD_GETTEXTRANGEA || (bOldWindows && uMsg == AKD_GETTEXTRANGE))
          return GetRangeTextA(hWnd, gtr->cpMin, gtr->cpMax, (char **)&gtr->pText);
        else
          return GetRangeTextW(hWnd, gtr->cpMin, gtr->cpMax, (wchar_t **)&gtr->pText);
      }
      case AKD_GETSELTEXTW:
      {
        AECHARRANGE cr;
        wchar_t *wpText=NULL;
        INT_PTR nTextLen;
        BOOL bColumnSel=FALSE;
        HWND hWnd=(HWND)wParam;
        INT_PTR *nResultLen=(INT_PTR *)lParam;

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        GetSel(hWnd, &cr, &bColumnSel, NULL);
        nTextLen=ExGetRangeTextW(hWnd, &cr.ciMin, &cr.ciMax, bColumnSel, &wpText, AELB_R, TRUE);
        if (nResultLen) *nResultLen=nTextLen;
        return (LRESULT)wpText;
      }
      case AKD_FREETEXT:
      {
        return FreeText((LPVOID)lParam);
      }
      case AKD_REPLACESEL:
      case AKD_REPLACESELA:
      case AKD_REPLACESELW:
      {
        HWND hWnd=(HWND)wParam;

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        if (uMsg == AKD_REPLACESELA || (bOldWindows && uMsg == AKD_REPLACESEL))
          ReplaceSelA(hWnd, (char *)lParam, -1, AELB_ASINPUT, AEREPT_COLUMNASIS, NULL, NULL);
        else
          ReplaceSelW(hWnd, (wchar_t *)lParam, -1, AELB_ASINPUT, AEREPT_COLUMNASIS, NULL, NULL);
        return 0;
      }
      case AKD_PASTE:
      {
        HWND hWnd=(HWND)wParam;
        DWORD dwFlags=(DWORD)(lParam % 100000);
        int nNewLine=(int)(lParam / 100000);

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        if (lParam & PASTE_CASE)
          return PasteCase(hWnd, (DWORD)(lParam & PASTE_ANSI));
        if (lParam & PASTE_SINGLELINE)
          return PasteInEditAsRichEdit(hWnd, 0);
        return DoEditPaste(hWnd, dwFlags, nNewLine);
      }
      case AKD_COPY:
      {
        HWND hWnd=(HWND)wParam;
        DWORD dwFlags=(DWORD)(lParam % 100000);
        int nNewLine=(int)(lParam / 100000);

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        return DoEditCopy(hWnd, dwFlags, nNewLine);
      }
      case AKD_TEXTFIND:
      case AKD_TEXTFINDA:
      case AKD_TEXTFINDW:
      {
        TEXTFINDW *tf=(TEXTFINDW *)lParam;
        FRAMEDATA *lpFrame;
        INT_PTR nResult=-1;

        if (lpFrame=GetFrameDataFromEditWindow((HWND)wParam))
        {
          if (uMsg == AKD_TEXTFINDA || (bOldWindows && uMsg == AKD_TEXTFIND))
          {
            wchar_t *wpFindIt;
            int nFindItLen;

            nFindItLen=MultiByteToWideChar(CP_ACP, 0, (char *)tf->pFindIt, tf->nFindItLen, NULL, 0);
            if (wpFindIt=(wchar_t *)GlobalAlloc(GPTR, nFindItLen * sizeof(wchar_t)))
            {
              MultiByteToWideChar(CP_ACP, 0, (char *)tf->pFindIt, tf->nFindItLen, wpFindIt, nFindItLen);

              nResult=TextFindW(lpFrame, tf->dwFlags, wpFindIt, tf->nFindItLen);
              GlobalFree((HGLOBAL)wpFindIt);
            }
          }
          else nResult=TextFindW(lpFrame, tf->dwFlags, tf->pFindIt, tf->nFindItLen);

          if (nResult == -1)
          {
            if (lpFrame->nCompileErrorOffset &&
                ((tf->dwFlags & FRF_REGEXP) ||
                 (tf->dwFlags & FRF_ESCAPESEQ)))
            {
              nResult=-(100 + lpFrame->nCompileErrorOffset - 1);
            }
          }
        }
        return nResult;
      }
      case AKD_TEXTREPLACE:
      case AKD_TEXTREPLACEA:
      case AKD_TEXTREPLACEW:
      {
        TEXTREPLACEW *tr=(TEXTREPLACEW *)lParam;
        FRAMEDATA *lpFrame;
        INT_PTR nResult=-1;

        if (lpFrame=GetFrameDataFromEditWindow((HWND)wParam))
        {
          if (uMsg == AKD_TEXTREPLACEA || (bOldWindows && uMsg == AKD_TEXTREPLACE))
          {
            wchar_t *wpFindIt;
            wchar_t *wpReplaceWith;
            int nFindItLen;
            int nReplaceWithLen;

            nFindItLen=MultiByteToWideChar(CP_ACP, 0, (char *)tr->pFindIt, tr->nFindItLen, NULL, 0);
            if (wpFindIt=(wchar_t *)GlobalAlloc(GPTR, nFindItLen * sizeof(wchar_t)))
            {
              MultiByteToWideChar(CP_ACP, 0, (char *)tr->pFindIt, tr->nFindItLen, wpFindIt, nFindItLen);

              nReplaceWithLen=MultiByteToWideChar(CP_ACP, 0, (char *)tr->pReplaceWith, tr->nReplaceWithLen, NULL, 0);
              if (wpReplaceWith=(wchar_t *)GlobalAlloc(GPTR, nReplaceWithLen * sizeof(wchar_t)))
              {
                MultiByteToWideChar(CP_ACP, 0, (char *)tr->pReplaceWith, tr->nReplaceWithLen, wpReplaceWith, nReplaceWithLen);

                nResult=TextReplaceW(lpFrame, tr->dwFindFlags, wpFindIt, tr->nFindItLen, wpReplaceWith, tr->nReplaceWithLen, tr->dwReplaceFlags, &tr->nChanges, NULL);
                GlobalFree((HGLOBAL)wpReplaceWith);
              }
              GlobalFree((HGLOBAL)wpFindIt);
            }
          }
          else nResult=TextReplaceW(lpFrame, tr->dwFindFlags, tr->pFindIt, tr->nFindItLen, tr->pReplaceWith, tr->nReplaceWithLen, tr->dwReplaceFlags, &tr->nChanges, NULL);

          if (nResult == -1)
          {
            if (lpFrame->nCompileErrorOffset &&
                ((tr->dwFindFlags & FRF_REGEXP) ||
                 (tr->dwFindFlags & FRF_ESCAPESEQ)))
            {
              nResult=-(100 + lpFrame->nCompileErrorOffset - 1);
            }
          }
        }
        return nResult;
      }
      case AKD_GOTO:
      case AKD_GOTOA:
      case AKD_GOTOW:
      {
        wchar_t *wpString=API_AllocWide(MAX_PATH);
        BOOL bResult;

        if (uMsg == AKD_GOTOA || (bOldWindows && uMsg == AKD_GOTO))
          xprintfW(wpString, L"%S", (char *)lParam);
        else
          xprintfW(wpString, L"%s", (wchar_t *)lParam);
        bResult=GoTo((DWORD)wParam, wpString);

        API_FreeWide(wpString);
        return bResult;
      }
      case AKD_RECODESEL:
      {
        TEXTRECODE *tr=(TEXTRECODE *)lParam;
        FRAMEDATA *lpFrame;

        if (lpFrame=GetFrameDataFromEditWindow((HWND)wParam))
          RecodeTextW(lpFrame, NULL, tr->dwFlags, &tr->nCodePageFrom, &tr->nCodePageTo);
        return 0;
      }

      //Print
      case AKD_GETPRINTINFO:
      {
        PRINTINFO *info=(PRINTINFO *)wParam;

        xmemcpy(info, &prninfo, sizeof(PRINTINFO));
        if (lParam)
          return GetPrinter(hMainWnd, info, TRUE);
        return 0;
      }
      case AKD_SETPRINTINFO:
      {
        PRINTINFO *info=(PRINTINFO *)wParam;

        xmemcpy(&prninfo, info, sizeof(PRINTINFO));
        return 0;
      }

      //Options
      case AKD_PROGRAMVERSION:
      {
        if (lParam)
          return IdentifierCompare(dwExeVersion, (DWORD)lParam);
        return dwExeVersion;
      }
      case AKD_PROGRAMARCHITECTURE:
      {
        if (lParam)
          return IdentifierCompare(AKELDLL, (DWORD)lParam);
        return AKELDLL;
      }
      case AKD_GETMAININFO:
      {
        switch (wParam)
        {
          case MI_AKELDIRA:
            return xstrcpynA((void *)lParam, szExeDir, MAX_PATH);
          case MI_AKELDIRW:
            return xstrcpynW((void *)lParam, wszExeDir, MAX_PATH);
          case MI_INSTANCEEXE:
            return (LRESULT)hInstance;
          case MI_PLUGINSSTACK:
          {
            if (lParam) xmemcpy((void *)lParam, &hPluginsStack, sizeof(HSTACK));
            return sizeof(HSTACK);
          }
          case MI_SAVESETTINGS:
            return (LRESULT)moCur.nSaveSettings;
          case MI_SAVEHISTORY:
            return (LRESULT)moCur.nSaveHistory;
          case MI_WNDPROGRESS:
            return (LRESULT)hProgress;
          case MI_WNDSTATUS:
            return (LRESULT)hStatus;
          case MI_WNDMDICLIENT:
            return (LRESULT)hMdiClient;
          case MI_WNDTAB:
            return (LRESULT)hTab;
          case MI_MENUMAIN:
            return (LRESULT)hMainMenu;
          case MI_MENURECENTFILES:
            return (LRESULT)hMenuRecentFiles;
          case MI_MENULANGUAGE:
            return (LRESULT)hMenuLanguage;
          case MI_MENUPOPUP:
            return (LRESULT)hPopupMenu;
          case MI_ICONMAIN:
            return (LRESULT)hMainIcon;
          case MI_ACCELGLOBAL:
            return (LRESULT)hGlobalAccel;
          case MI_ACCELMAIN:
            return (LRESULT)hMainAccel;
          case MI_OLDWINDOWS:
            return (LRESULT)bOldWindows;
          case MI_OLDRICHEDIT:
            return (LRESULT)bOldRichEdit;
          case MI_VERCOMCTL32:
            return (LRESULT)dwVerComctl32;
          case MI_AKELEDIT:
            return (LRESULT)bAkelEdit;
          case MI_MDI:
            return (LRESULT)nMDI;
          case MI_LANGHANDLE:
            return (LRESULT)hLangModule;
          case MI_LANGMODULEA:
            return xstrcpynA((void *)lParam, moCur.szLangModule, MAX_PATH);
          case MI_LANGMODULEW:
            return xstrcpynW((void *)lParam, moCur.wszLangModule, MAX_PATH);
          case MI_LANGIDSYSTEM:
            return (LRESULT)dwLangSystem;
          case MI_LANGIDMODULE:
            return (LRESULT)dwLangModule;
          case MI_ONSTART:
            return bMainOnStart;
          case MI_ONFINISH:
            return nMainOnFinish;
          case MI_AKELEXEA:
            return xstrcpynA((void *)lParam, szExeFile, MAX_PATH);
          case MI_AKELEXEW:
            return xstrcpynW((void *)lParam, wszExeFile, MAX_PATH);
          case MI_X64:
          {
            #ifdef _WIN64
              return TRUE;
            #else
              return FALSE;
            #endif
          }
          case MI_AKELEDITSTATICBUILD:
          {
            #ifdef AKELEDIT_STATICBUILD
              return TRUE;
            #else
              return FALSE;
            #endif
          }
          case MI_AKELPADDLLBUILD:
          {
            #ifdef AKELPAD_DLLBUILD
              return TRUE;
            #else
              return FALSE;
            #endif
          }
          case MI_CMDLINEBEGIN:
            return xstrcpyW((void *)lParam, wpCmdLineBegin);
          case MI_CMDLINEEND:
            return xstrcpyW((void *)lParam, wpCmdLineEnd);
          case MI_SHOWMODIFY:
            return moCur.dwShowModify;
          case MI_STATUSPOSTYPE:
            return moCur.dwStatusPosType;
          case MI_STATUSUSERFORMAT:
            return xstrcpynW((void *)lParam, moCur.wszStatusUserFormat, MAX_PATH);
          case MI_WORDBREAKCUSTOM:
            return moCur.dwWordBreakCustom;
          case MI_PAINTOPTIONS:
            return moCur.dwPaintOptions;
          case MI_FIXEDCHARWIDTH:
            return moCur.nFixedCharWidth;
          case MI_EDITSTYLE:
            return moCur.dwEditStyle;
          case MI_RICHEDITCLASS:
            return moCur.bRichEditClass;
          case MI_AKELADMINRESIDENT:
            return moCur.bAkelAdminResident;
          case MI_DATELOGFORMAT:
            return xstrcpynW((void *)lParam, moCur.wszDateLogFormat, 128);
          case MI_DATEINSERTFORMAT:
            return xstrcpynW((void *)lParam, moCur.wszDateInsertFormat, 128);
          case MI_AKELUPDATEROPTIONS:
            return xstrcpynW((void *)lParam, moCur.wszAkelUpdaterOptions, MAX_PATH);
          case MI_URLCOMMAND:
            return xstrcpynW((void *)lParam, moCur.wszUrlCommand, MAX_PATH);
          case MI_TABNAMEFIND:
            return xstrcpynW((void *)lParam, moCur.wszTabNameFind, MAX_PATH);
          case MI_TABNAMEREP:
            return xstrcpynW((void *)lParam, moCur.wszTabNameRep, MAX_PATH);
          case MI_ONTOP:
            return moCur.bOnTop;
          case MI_STATUSBAR:
            return moCur.bStatusBar;
          case MI_KEEPSPACE:
            return moCur.bKeepSpace;
          case MI_WATCHFILE:
            return moCur.bWatchFile;
          case MI_SAVETIME:
            return moCur.bSaveTime;
          case MI_SINGLEOPENFILE:
            return moCur.bSingleOpenFile;
          case MI_SINGLEOPENPROGRAM:
            return moCur.dwSingleOpenProgram;
          case MI_TABOPTIONSMDI:
            return moCur.dwTabOptionsMDI;
          case MI_EXECUTECOMMAND:
            return xstrcpynW((void *)lParam, moCur.wszExecuteCommand, BUFFER_SIZE);
          case MI_EXECUTEDIRECTORY:
            return xstrcpynW((void *)lParam, moCur.wszExecuteDirectory, MAX_PATH);
          case MI_CODEPAGELIST:
          {
            if (lParam) xmemcpy((void *)lParam, lpCodepageList, nCodepageListLen * sizeof(int));
            return nCodepageListLen * sizeof(int);
          }
          case MI_DEFAULTCODEPAGE:
            return moCur.nDefaultCodePage;
          case MI_DEFAULTBOM:
            return moCur.bDefaultBOM;
          case MI_NEWFILECODEPAGE:
            return moCur.nNewFileCodePage;
          case MI_NEWFILEBOM:
            return moCur.bNewFileBOM;
          case MI_NEWFILENEWLINE:
            return moCur.nNewFileNewLine;
          case MI_LANGCODEPAGERECOGNITION:
            return moCur.dwLangCodepageRecognition;
          case MI_CODEPAGERECOGNITIONBUFFER:
            return moCur.dwCodepageRecognitionBuffer;
          case MI_SAVEPOSITIONS:
            return moCur.bSavePositions;
          case MI_SAVECODEPAGES:
            return moCur.bSaveCodepages;
          case MI_RECENTFILES:
            return moCur.nRecentFiles;
          case MI_SEARCHSTRINGS:
            return moCur.nSearchStrings;
          case MI_FILETYPESOPEN:
            return xstrcpynW((void *)lParam, moCur.wszFileTypesOpen, MAX_PATH);
          case MI_FILETYPESEDIT:
            return xstrcpynW((void *)lParam, moCur.wszFileTypesEdit, MAX_PATH);
          case MI_FILETYPESPRINT:
            return xstrcpynW((void *)lParam, moCur.wszFileTypesPrint, MAX_PATH);
          case MI_FILETYPESASSOCIATED:
            return moCur.dwFileTypesAssociated;
          case MI_KEYBLAYOUTOPTIONS:
            return moCur.dwKeybLayoutOptions;
          case MI_SILENTCLOSEEMPTYMDI:
            return moCur.bSilentCloseEmptyMDI;
          case MI_DATELOG:
            return moCur.bDateLog;
          case MI_SAVEINREADONLYMSG:
            return moCur.bSaveInReadOnlyMsg;
          case MI_DEFAULTSAVEEXT:
            return xstrcpynW((void *)lParam, moCur.wszDefaultSaveExt, MAX_PATH);
          case MI_SEARCHOPTIONS:
            return moCur.dwSearchOptions;
          case MI_LASTFIND:
            return xstrcpyW((void *)lParam, wszFindText);
          case MI_LASTREPLACE:
            return xstrcpyW((void *)lParam, wszReplaceText);
          case MI_PRINTMARGINS:
          {
            if (lParam) xmemcpy((void *)lParam, &moCur.rcPrintMargins, sizeof(RECT));
            return sizeof(RECT);
          }
          case MI_PRINTCOLOR:
            return moCur.dwPrintColor;
          case MI_PRINTHEADERENABLE:
            return moCur.bPrintHeaderEnable;
          case MI_PRINTHEADER:
            return xstrcpynW((void *)lParam, moCur.wszPrintHeader, MAX_PATH);
          case MI_PRINTFOOTERENABLE:
            return moCur.bPrintFooterEnable;
          case MI_PRINTFOOTER:
            return xstrcpynW((void *)lParam, moCur.wszPrintFooter, MAX_PATH);
          case MI_PRINTFONTENABLE:
            return moCur.bPrintFontEnable;
          case MI_PRINTFONTW:
          {
            if (lParam) xmemcpy((void *)lParam, &moCur.lfPrintFont, sizeof(LOGFONTW));
            return sizeof(LOGFONTW);
          }
          case MI_OFNDIR:
            return xstrcpynW((void *)lParam, moCur.wszOfnDir, MAX_PATH);
          case MI_LASTDIR:
            return xstrcpynW((void *)lParam, moCur.wszLastDir, MAX_PATH);
          case MI_SHOWPLACESBAR:
            return moCur.bShowPlacesBar;
        }
        return 0;
      }
      case AKD_SETMAININFO:
      {
        switch (wParam)
        {
          case MIS_SAVESETTINGS:
          {
            if (SetOption(lParam, &moCur.nSaveSettings, sizeof(DWORD), INI_DWORD))
            {
              //Save to INI
              SaveOptions(&moCur, lpFrameCurrent, SS_INI, TRUE);
              SaveThemes(SS_INI);
              StackPluginSave(&hPluginsStack, SS_INI);

              //Save to registry
              SaveOptions(&moCur, lpFrameCurrent, SS_REGISTRY, TRUE);
              SaveThemes(SS_REGISTRY);
              StackPluginSave(&hPluginsStack, SS_REGISTRY);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_SAVEHISTORY:
          {
            if (SetOption(lParam, &moCur.nSaveHistory, sizeof(DWORD), INI_DWORD))
            {
              //Save to INI
              RecentFilesSave(&hRecentFilesStack, SS_INI);
              SearchSave(SS_INI);

              //Save to registry
              RecentFilesSave(&hRecentFilesStack, SS_REGISTRY);
              SearchSave(SS_REGISTRY);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_MDI:
          {
            if (nMDI != lParam)
            {
              //API_LoadString(hLangModule, MSG_RESTART_PROGRAM, wszMsg, BUFFER_SIZE);
              //API_MessageBox(hWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
              moCur.nMDI=(int)lParam;
              return TRUE;
            }
            return FALSE;
          }
          case MIS_LANGMODULEA:
          {
            if (SetOption(lParam, moCur.szLangModule, sizeof(moCur.szLangModule), INI_STRINGANSI))
            {
              MultiByteToWideChar(CP_ACP, 0, moCur.szLangModule, -1, moCur.wszLangModule, MAX_PATH);
              //API_LoadString(hLangModule, MSG_RESTART_PROGRAM, wszMsg, BUFFER_SIZE);
              //API_MessageBox(hWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_LANGMODULEW:
          {
            if (SetOption(lParam, moCur.wszLangModule, sizeof(moCur.wszLangModule), INI_STRINGUNICODE))
            {
              WideCharToMultiByte(CP_ACP, 0, moCur.wszLangModule, -1, moCur.szLangModule, MAX_PATH, NULL, NULL);
              //API_LoadString(hLangModule, MSG_RESTART_PROGRAM, wszMsg, BUFFER_SIZE);
              //API_MessageBox(hWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_CMDLINEBEGIN:
          {
            bCmdLineChanged=TRUE;
            if (wpCmdLineBegin)
              API_HeapFree(hHeap, 0, (LPVOID)wpCmdLineBegin);
            nCmdLineBeginLen=(int)xstrlenW((wchar_t *)lParam);

            if (wpCmdLineBegin=(wchar_t *)API_HeapAlloc(hHeap, 0, (nCmdLineBeginLen + 1) * sizeof(wchar_t)))
            {
              xstrcpynW(wpCmdLineBegin, (void *)lParam, nCmdLineBeginLen + 1);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_CMDLINEEND:
          {
            bCmdLineChanged=TRUE;
            if (wpCmdLineEnd)
              API_HeapFree(hHeap, 0, (LPVOID)wpCmdLineEnd);
            nCmdLineEndLen=(int)xstrlenW((wchar_t *)lParam);

            if (wpCmdLineEnd=(wchar_t *)API_HeapAlloc(hHeap, 0, (nCmdLineEndLen + 1) * sizeof(wchar_t)))
            {
              xstrcpynW(wpCmdLineEnd, (void *)lParam, nCmdLineEndLen + 1);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_SHOWMODIFY:
          {
            if ((DWORD)lParam != moCur.dwShowModify)
            {
              UpdateAsterisk(lpFrameCurrent, FALSE);
              SetModifyStatus(NULL, FALSE);
              moCur.dwShowModify=(DWORD)lParam;
              UpdateAsterisk(lpFrameCurrent, lpFrameCurrent->ei.bModified);
              SetModifyStatus(NULL, lpFrameCurrent->ei.bModified);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_STATUSPOSTYPE:
          {
            if (SetOption(lParam, &moCur.dwStatusPosType, sizeof(DWORD), INI_DWORD))
            {
              SetSelectionStatus(lpFrameCurrent->ei.hDocEdit, lpFrameCurrent->ei.hWndEdit, NULL, NULL);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_STATUSUSERFORMAT:
          {
            if (SetOption(lParam, moCur.wszStatusUserFormat, sizeof(moCur.wszStatusUserFormat), INI_STRINGUNICODE))
            {
              int *lpSBParts;

              StackStatusPartFree(&hStatusStack);

              //Get status bar user flags
              if (moCur.wszStatusUserFormat[0])
              {
                STATUSPART *sp;

                moCur.nStatusUserFormatLen=(int)xstrlenW(moCur.wszStatusUserFormat);
                TranslateStatusUser(NULL, moCur.wszStatusUserFormat, moCur.nStatusUserFormatLen, NULL, 0);

                moCur.dwStatusUserFlags=0;
                for (sp=hStatusStack.first; sp; sp=sp->next)
                  moCur.dwStatusUserFlags|=sp->dwFormatFlags;
              }
              if (lpSBParts=SetStatusParts(&hStatusStack))
                API_HeapFree(hHeap, 0, (LPVOID)lpSBParts);

              UpdateStatusUser(lpFrameCurrent, (DWORD)-1);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_WORDBREAKCUSTOM:
            return SetOption(lParam, &moCur.dwWordBreakCustom, sizeof(DWORD), INI_DWORD);
          case MIS_PAINTOPTIONS:
            return SetOption(lParam, &moCur.dwPaintOptions, sizeof(DWORD), INI_DWORD);
          case MIS_FIXEDCHARWIDTH:
            return SetOption(lParam, &moCur.nFixedCharWidth, sizeof(DWORD), INI_DWORD);
          case MIS_EDITSTYLE:
            return SetOption(lParam, &moCur.dwEditStyle, sizeof(DWORD), INI_DWORD);
          case MIS_RICHEDITCLASS:
            return SetOption(lParam, &moCur.bRichEditClass, sizeof(DWORD), INI_DWORD);
          case MIS_AKELADMINRESIDENT:
            return SetOption(lParam, &moCur.bAkelAdminResident, sizeof(DWORD), INI_DWORD);
          case MIS_DATELOGFORMAT:
            return SetOption(lParam, moCur.wszDateLogFormat, sizeof(moCur.wszDateLogFormat), INI_STRINGUNICODE);
          case MIS_DATEINSERTFORMAT:
            return SetOption(lParam, moCur.wszDateInsertFormat, sizeof(moCur.wszDateInsertFormat), INI_STRINGUNICODE);
          case MIS_AKELUPDATEROPTIONS:
            return SetOption(lParam, moCur.wszAkelUpdaterOptions, sizeof(moCur.wszAkelUpdaterOptions), INI_STRINGUNICODE);
          case MIS_URLCOMMAND:
            return SetOption(lParam, moCur.wszUrlCommand, sizeof(moCur.wszUrlCommand), INI_STRINGUNICODE);
          case MIS_TABNAMEFIND:
            return SetOption(lParam, moCur.wszTabNameFind, sizeof(moCur.wszTabNameFind), INI_STRINGUNICODE);
          case MIS_TABNAMEREP:
            return SetOption(lParam, moCur.wszTabNameRep, sizeof(moCur.wszTabNameRep), INI_STRINGUNICODE);
          case MIS_ONTOP:
          {
            if (lParam != moCur.bOnTop)
            {
              DoViewOnTop((BOOL)lParam);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_STATUSBAR:
          {
            if (lParam != moCur.bStatusBar)
            {
              DoViewShowStatusBar((BOOL)lParam);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_KEEPSPACE:
          {
            if (lParam != moCur.bKeepSpace)
            {
              DoSettingsKeepSpace((BOOL)lParam);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_WATCHFILE:
          {
            if (lParam != moCur.bWatchFile)
            {
              DoSettingsWatchFile((BOOL)lParam);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_SAVETIME:
          {
            if (lParam != moCur.bSaveTime)
            {
              DoSettingsSaveTime((BOOL)lParam);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_SINGLEOPENFILE:
          {
            if (lParam != moCur.bSingleOpenFile)
            {
              DoSettingsSingleOpenFile((BOOL)lParam);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_SINGLEOPENPROGRAM:
          {
            if ((DWORD)lParam != moCur.dwSingleOpenProgram)
            {
              DoSettingsSingleOpenProgram((DWORD)lParam);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_TABOPTIONSMDI:
          {
            if ((DWORD)lParam != moCur.dwTabOptionsMDI)
            {
              DoWindowTabView((DWORD)lParam, FALSE);
              DoWindowTabType((DWORD)lParam, FALSE);
              DoWindowTabSwitch((DWORD)lParam, FALSE);
              moCur.dwTabOptionsMDI=(DWORD)lParam;
              return TRUE;
            }
            return FALSE;
          }
          case MIS_EXECUTECOMMAND:
            return SetOption(lParam, moCur.wszExecuteCommand, sizeof(moCur.wszExecuteCommand), INI_STRINGUNICODE);
          case MIS_EXECUTEDIRECTORY:
            return SetOption(lParam, moCur.wszExecuteDirectory, sizeof(moCur.wszExecuteDirectory), INI_STRINGUNICODE);
          case MIS_CODEPAGELIST:
          {
            CodepageListFree(&lpCodepageList);
            lpCodepageList=(int *)lParam;
            nCodepageListLen=CodepageListLen(lpCodepageList);
            return TRUE;
          }
          case MIS_DEFAULTCODEPAGE:
            return SetOption(lParam, &moCur.nDefaultCodePage, sizeof(DWORD), INI_DWORD);
          case MIS_DEFAULTBOM:
            return SetOption(lParam, &moCur.bDefaultBOM, sizeof(DWORD), INI_DWORD);
          case MIS_NEWFILECODEPAGE:
            return SetOption(lParam, &moCur.nNewFileCodePage, sizeof(DWORD), INI_DWORD);
          case MIS_NEWFILEBOM:
            return SetOption(lParam, &moCur.bNewFileBOM, sizeof(DWORD), INI_DWORD);
          case MIS_NEWFILENEWLINE:
            return SetOption(lParam, &moCur.nNewFileNewLine, sizeof(DWORD), INI_DWORD);
          case MIS_LANGCODEPAGERECOGNITION:
            return SetOption(lParam, &moCur.dwLangCodepageRecognition, sizeof(DWORD), INI_DWORD);
          case MIS_CODEPAGERECOGNITIONBUFFER:
            return SetOption(lParam, &moCur.dwCodepageRecognitionBuffer, sizeof(DWORD), INI_DWORD);
          case MIS_SAVEPOSITIONS:
            return SetOption(lParam, &moCur.bSavePositions, sizeof(DWORD), INI_DWORD);
          case MIS_SAVECODEPAGES:
            return SetOption(lParam, &moCur.bSaveCodepages, sizeof(DWORD), INI_DWORD);
          case MIS_RECENTFILES:
          {
            //Same as AKD_RECENTFILES with RF_SET
            if (SetOption(lParam, &moCur.nRecentFiles, sizeof(DWORD), INI_DWORD))
            {
              RecentFilesCut(&hRecentFilesStack);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_SEARCHSTRINGS:
            return SetOption(lParam, &moCur.nSearchStrings, sizeof(DWORD), INI_DWORD);
          case MIS_FILETYPESOPEN:
            return SetOption(lParam, moCur.wszFileTypesOpen, sizeof(moCur.wszFileTypesOpen), INI_STRINGUNICODE);
          case MIS_FILETYPESEDIT:
            return SetOption(lParam, moCur.wszFileTypesEdit, sizeof(moCur.wszFileTypesEdit), INI_STRINGUNICODE);
          case MIS_FILETYPESPRINT:
            return SetOption(lParam, moCur.wszFileTypesPrint, sizeof(moCur.wszFileTypesPrint), INI_STRINGUNICODE);
          case MIS_FILETYPESASSOCIATED:
          {
            if (lParam & FTA_OPEN)
              AssociateFileTypesW(hInstance, moCur.wszFileTypesOpen, FTA_OPEN|FTA_ASSOCIATE);
            else
              AssociateFileTypesW(hInstance, moCur.wszFileTypesOpen, FTA_OPEN|FTA_DEASSOCIATE);

            if (lParam & FTA_EDIT)
              AssociateFileTypesW(hInstance, moCur.wszFileTypesEdit, FTA_EDIT|FTA_ASSOCIATE);
            else
              AssociateFileTypesW(hInstance, moCur.wszFileTypesEdit, FTA_EDIT|FTA_DEASSOCIATE);

            if (lParam & FTA_PRINT)
              AssociateFileTypesW(hInstance, moCur.wszFileTypesPrint, FTA_PRINT|FTA_ASSOCIATE);
            else
              AssociateFileTypesW(hInstance, moCur.wszFileTypesPrint, FTA_PRINT|FTA_DEASSOCIATE);

            return SetOption(lParam, &moCur.dwFileTypesAssociated, sizeof(DWORD), INI_DWORD);
          }
          case MIS_KEYBLAYOUTOPTIONS:
            return SetOption(lParam, &moCur.dwKeybLayoutOptions, sizeof(DWORD), INI_DWORD);
          case MIS_SILENTCLOSEEMPTYMDI:
            return SetOption(lParam, &moCur.bSilentCloseEmptyMDI, sizeof(DWORD), INI_DWORD);
          case MIS_DATELOG:
            return SetOption(lParam, &moCur.bDateLog, sizeof(DWORD), INI_DWORD);
          case MIS_SAVEINREADONLYMSG:
            return SetOption(lParam, &moCur.bSaveInReadOnlyMsg, sizeof(DWORD), INI_DWORD);
          case MIS_DEFAULTSAVEEXT:
            return SetOption(lParam, moCur.wszDefaultSaveExt, sizeof(moCur.wszDefaultSaveExt), INI_STRINGUNICODE);
          case MIS_SEARCHOPTIONS:
            return SetOption(lParam, &moCur.dwSearchOptions, sizeof(DWORD), INI_DWORD);
          case MIS_LASTFIND:
          {
            if (wszFindText)
              API_FreeWide(wszFindText);
            nFindTextLen=(int)xstrlenW((wchar_t *)lParam);

            if (wszFindText=API_AllocWide(nFindTextLen + 1))
            {
              xstrcpynW(wszFindText, (void *)lParam, nFindTextLen + 1);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_LASTREPLACE:
          {
            if (wszReplaceText)
              API_FreeWide(wszReplaceText);
            nReplaceTextLen=(int)xstrlenW((wchar_t *)lParam);

            if (wszReplaceText=API_AllocWide(nReplaceTextLen + 1))
            {
              xstrcpynW(wszReplaceText, (void *)lParam, nReplaceTextLen + 1);
              return TRUE;
            }
            return FALSE;
          }
          case MIS_PRINTMARGINS:
            return SetOption(lParam, &moCur.rcPrintMargins, sizeof(RECT), INI_BINARY);
          case MIS_PRINTCOLOR:
            return SetOption(lParam, &moCur.dwPrintColor, sizeof(DWORD), INI_DWORD);
          case MIS_PRINTHEADERENABLE:
            return SetOption(lParam, &moCur.bPrintHeaderEnable, sizeof(DWORD), INI_DWORD);
          case MIS_PRINTHEADER:
            return SetOption(lParam, moCur.wszPrintHeader, sizeof(moCur.wszPrintHeader), INI_STRINGUNICODE);
          case MIS_PRINTFOOTERENABLE:
            return SetOption(lParam, &moCur.bPrintFooterEnable, sizeof(DWORD), INI_DWORD);
          case MIS_PRINTFOOTER:
            return SetOption(lParam, moCur.wszPrintFooter, sizeof(moCur.wszPrintFooter), INI_STRINGUNICODE);
          case MIS_PRINTFONTENABLE:
            return SetOption(lParam, &moCur.bPrintFontEnable, sizeof(DWORD), INI_DWORD);
          case MIS_PRINTFONTW:
            return SetOption(lParam, &moCur.lfPrintFont, sizeof(LOGFONTW), INI_BINARY);
          case MIS_OFNDIR:
            return SetOption(lParam, moCur.wszOfnDir, sizeof(moCur.wszOfnDir), INI_STRINGUNICODE);
          case MIS_LASTDIR:
            return SetOption(lParam, moCur.wszLastDir, sizeof(moCur.wszLastDir), INI_STRINGUNICODE);
          case MIS_SHOWPLACESBAR:
            return SetOption(lParam, &moCur.bShowPlacesBar, sizeof(DWORD), INI_DWORD);
        }
        return FALSE;
      }
      case AKD_GETFRAMEINFO:
      {
        FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;
        int nType;
        UINT_PTR dwData=0;
        UINT_PTR *lpdwData;
        UINT_PTR dwSize=sizeof(UINT_PTR);

        if (!lpFrame)
        {
          if (lpFrameCurrent->hWndEditParent)
            lpFrame=lpFrameCurrent;
        }
        if (wParam > 0xFFFF)
        {
          FRAMEINFO *fi=(FRAMEINFO *)wParam;

          nType=fi->nType;
          lpdwData=&fi->dwData;
        }
        else
        {
          nType=(int)wParam;
          lpdwData=&dwData;
        }

        if (lpFrame)
        {
          switch (nType)
          {
            case FI_WNDEDITPARENT:
            {
              *lpdwData=(UINT_PTR)lpFrame->hWndEditParent;
              break;
            }
            case FI_WNDEDIT:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hWndEdit;
              break;
            }
            case FI_DOCEDIT:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hDocEdit;
              break;
            }
            case FI_WNDMASTER:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hWndMaster;
              break;
            }
            case FI_DOCMASTER:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hDocMaster;
              break;
            }
            case FI_WNDCLONE1:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hWndClone1;
              break;
            }
            case FI_DOCCLONE1:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hDocClone1;
              break;
            }
            case FI_WNDCLONE2:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hWndClone2;
              break;
            }
            case FI_DOCCLONE2:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hDocClone2;
              break;
            }
            case FI_WNDCLONE3:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hWndClone3;
              break;
            }
            case FI_DOCCLONE3:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.hDocClone3;
              break;
            }
            case FI_CODEPAGE:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.nCodePage;
              break;
            }
            case FI_BOM:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.bBOM;
              break;
            }
            case FI_NEWLINE:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.nNewLine;
              break;
            }
            case FI_MODIFIED:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.bModified;
              break;
            }
            case FI_READONLY:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.bReadOnly;
              break;
            }
            case FI_WORDWRAP:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.bWordWrap;
              break;
            }
            case FI_OVERTYPE:
            {
              *lpdwData=(UINT_PTR)lpFrame->ei.bOvertypeMode;
              break;
            }
            case FI_FILEA:
            {
              if (wParam > 0xFFFF)
                dwSize=xstrcpynA((void *)*lpdwData, lpFrame->szFile, MAX_PATH);
              else
                *lpdwData=(UINT_PTR)lpFrame->szFile;
              break;
            }
            case FI_FILEW:
            {
              if (wParam > 0xFFFF)
                dwSize=xstrcpynW((void *)*lpdwData, lpFrame->wszFile, MAX_PATH);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszFile;
              break;
            }
            case FI_FILELEN:
            {
              *lpdwData=(UINT_PTR)lpFrame->nFileLen;
              break;
            }
            case FI_STREAMOFFSET:
            {
              *lpdwData=(UINT_PTR)lpFrame->nStreamOffset;
              break;
            }
            case FI_FILEDIRW:
            {
              if (wParam > 0xFFFF)
                dwSize=xstrcpynW((void *)*lpdwData, lpFrame->wszFileDir, MAX_PATH);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszFileDir;
              break;
            }
            case FI_FILEDIRLEN:
            {
              *lpdwData=(UINT_PTR)lpFrame->nFileDirLen;
              break;
            }
            case FI_ICONHANDLE:
            {
              *lpdwData=(UINT_PTR)lpFrame->hIcon;
              break;
            }
            case FI_ICONINDEX:
            {
              *lpdwData=(UINT_PTR)lpFrame->nIconIndex;
              break;
            }
            case FI_RECTEDIT:
            {
              if (wParam > 0xFFFF)
              {
                dwSize=sizeof(RECT);
                if (*lpdwData) xmemcpy((void *)*lpdwData, &lpFrame->rcEditWindow, dwSize);
              }
              else *lpdwData=(UINT_PTR)&lpFrame->rcEditWindow;

              break;
            }
            case FI_RECTMASTER:
            {
              if (wParam > 0xFFFF)
              {
                dwSize=sizeof(RECT);
                if (*lpdwData) xmemcpy((void *)*lpdwData, &lpFrame->rcMasterWindow, dwSize);
              }
              else *lpdwData=(UINT_PTR)&lpFrame->rcMasterWindow;

              break;
            }
            case FI_EDITFONTW:
            {
              if (wParam > 0xFFFF)
              {
                dwSize=sizeof(LOGFONTW);
                if (*lpdwData) xmemcpy((void *)*lpdwData, &lpFrame->lf, dwSize);
              }
              else *lpdwData=(UINT_PTR)&lpFrame->lf;

              break;
            }
            case FI_TABSTOPSIZE:
            {
              *lpdwData=(UINT_PTR)lpFrame->nTabStopSize;
              break;
            }
            case FI_TABSTOPASSPACES:
            {
              *lpdwData=(UINT_PTR)lpFrame->bTabStopAsSpaces;
              break;
            }
            case FI_UNDOLIMIT:
            {
              *lpdwData=(UINT_PTR)lpFrame->nUndoLimit;
              break;
            }
            case FI_DETAILEDUNDO:
            {
              *lpdwData=(UINT_PTR)lpFrame->bDetailedUndo;
              break;
            }
            case FI_WRAPTYPE:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwWrapType;
              break;
            }
            case FI_WRAPLIMIT:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwWrapLimit;
              break;
            }
            case FI_MARKER:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwMarker;
              break;
            }
            case FI_CARETWIDTH:
            {
              *lpdwData=(UINT_PTR)lpFrame->nCaretWidth;
              break;
            }
            case FI_CARETOPTIONS:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwCaretOptions;
              break;
            }
            case FI_ALTLINEFILL:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwAltLineFill;
              break;
            }
            case FI_ALTLINESKIP:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwAltLineSkip;
              break;
            }
            case FI_ALTLINEBORDER:
            {
              *lpdwData=(UINT_PTR)lpFrame->bAltLineBorder;
              break;
            }
            case FI_RECTMARGINS:
            {
              *lpdwData=(UINT_PTR)&lpFrame->rcEditMargins;
              break;
            }
            case FI_LINEGAP:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwLineGap;
              break;
            }
            case FI_MOUSEOPTIONS:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwMouseOptions;
              break;
            }
            case FI_CLICKURL:
            {
              *lpdwData=(UINT_PTR)lpFrame->nClickURL;
              break;
            }
            case FI_SHOWURL:
            {
              *lpdwData=(UINT_PTR)lpFrame->bShowURL;
              break;
            }
            case FI_URLPREFIXESENABLE:
            {
              *lpdwData=(UINT_PTR)lpFrame->bUrlPrefixesEnable;
              break;
            }
            case FI_URLPREFIXES:
            {
              if (wParam > 0xFFFF)
                dwSize=xstrcpynW((void *)*lpdwData, lpFrame->wszUrlPrefixes, URL_PREFIXES_SIZE);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszUrlPrefixes;
              break;
            }
            case FI_URLDELIMITERSENABLE:
            {
              *lpdwData=(UINT_PTR)lpFrame->bUrlDelimitersEnable;
              break;
            }
            case FI_URLLEFTDELIMITERS:
            {
              if (wParam > 0xFFFF)
                dwSize=xarrcpynW((void *)*lpdwData, lpFrame->wszUrlLeftDelimiters, URL_DELIMITERS_SIZE);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszUrlLeftDelimiters;
              break;
            }
            case FI_URLRIGHTDELIMITERS:
            {
              if (wParam > 0xFFFF)
                dwSize=xarrcpynW((void *)*lpdwData, lpFrame->wszUrlRightDelimiters, URL_DELIMITERS_SIZE);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszUrlRightDelimiters;
              break;
            }
            case FI_WORDDELIMITERSENABLE:
            {
              *lpdwData=(UINT_PTR)lpFrame->bWordDelimitersEnable;
              break;
            }
            case FI_WORDDELIMITERS:
            {
              if (wParam > 0xFFFF)
                dwSize=xarrcpynW((void *)*lpdwData, lpFrame->wszWordDelimiters, WORD_DELIMITERS_SIZE);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszWordDelimiters;
              break;
            }
            case FI_WRAPDELIMITERSENABLE:
            {
              *lpdwData=(UINT_PTR)lpFrame->bWrapDelimitersEnable;
              break;
            }
            case FI_WRAPDELIMITERS:
            {
              if (wParam > 0xFFFF)
                dwSize=xarrcpynW((void *)*lpdwData, lpFrame->wszWrapDelimiters, WRAP_DELIMITERS_SIZE);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszWrapDelimiters;
              break;
            }
            case FI_MAPPEDPRINTWIDTH:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwMappedPrintWidth;
              break;
            }
            case FI_INPUTLOCALE:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwInputLocale;
              break;
            }
            case FI_LOCKINHERIT:
            {
              *lpdwData=(UINT_PTR)lpFrame->dwLockInherit;
              break;
            }
            case FI_FILETIME:
            {
              if (wParam > 0xFFFF)
              {
                dwSize=sizeof(FILETIME);
                if (*lpdwData) xmemcpy((void *)*lpdwData, &lpFrame->ft, dwSize);
              }
              else *lpdwData=(UINT_PTR)&lpFrame->ft;

              break;
            }
            case FI_COLORS:
            {
              if (wParam > 0xFFFF)
              {
                dwSize=sizeof(AECOLORS);
                if (*lpdwData) xmemcpy((void *)*lpdwData, &lpFrame->aec, dwSize);
              }
              else *lpdwData=(UINT_PTR)&lpFrame->aec;

              break;
            }
            case FI_BKIMAGEFILE:
            {
              if (wParam > 0xFFFF)
                dwSize=xstrcpynW((void *)*lpdwData, lpFrame->wszBkImageFile, MAX_PATH);
              else
                *lpdwData=(UINT_PTR)lpFrame->wszBkImageFile;
              break;
            }
            case FI_BKIMAGEALPHA:
            {
              *lpdwData=(UINT_PTR)lpFrame->nBkImageAlpha;
              break;
            }
            case FI_STRUCTSIZE:
            {
              *lpdwData=(UINT_PTR)lpFrame->cb;
              break;
            }
            case FI_FRAMEID:
            {
              *lpdwData=(UINT_PTR)lpFrame->nFrameID;
              break;
            }
          }

          if (wParam > 0xFFFF)
            return dwSize;
          else
            return *lpdwData;
        }
        return 0;
      }
      case AKD_SETFRAMEINFO:
      {
        FRAMEINFO *fi=(FRAMEINFO *)wParam;

        return SetFrameInfo((FRAMEDATA *)lParam, fi->nType, fi->dwData);
      }
      case AKD_GETEDITINFO:
      {
        return GetEditInfo((HWND)wParam, (EDITINFO *)lParam);
      }
      case AKD_SETMODIFY:
      {
        FRAMEDATA *lpFrame;

        if (lpFrame=GetFrameDataFromEditWindow((HWND)wParam))
          SetModifyStatus(lpFrame, (BOOL)lParam);
        return 0;
      }
      case AKD_SETNEWLINE:
      {
        FRAMEDATA *lpFrame;

        if (lpFrame=GetFrameDataFromEditWindow((HWND)wParam))
          SetNewLineStatus(lpFrame, (int)lParam, AENL_INPUT|AENL_OUTPUT);
        return 0;
      }
      case AKD_GETFONT:
      case AKD_GETFONTA:
      case AKD_GETFONTW:
      {
        FRAMEDATA *lpFrame;
        FONTITEM *fi;

        if (lpFrame=GetFrameDataFromEditWindow((HWND)wParam))
        {
          if (lParam)
          {
            if (uMsg == AKD_GETFONTA || (bOldWindows && uMsg == AKD_GETFONT))
              LogFontWtoA(&lpFrame->lf, (LOGFONTA *)lParam);
            else
              xmemcpy((LOGFONTW *)lParam, &lpFrame->lf, sizeof(LOGFONTW));
          }
          if (fi=StackFontItemGet(&hFontsStack, &lpFrame->lf))
            return (LRESULT)fi->hFont;
        }
        else if (!wParam)
        {
          //If there is no frames, return last font handle
          return (LRESULT)((FONTITEM *)hFontsStack.last)->hFont;
        }
        return (LRESULT)NULL;
      }
      case AKD_SETFONT:
      case AKD_SETFONTA:
      case AKD_SETFONTW:
      {
        HWND hWnd=(HWND)wParam;
        FRAMEDATA *lpFrame;
        LOGFONTW lfW;

        if (uMsg == AKD_SETFONTA || (bOldWindows && uMsg == AKD_SETFONT))
          LogFontAtoW((LOGFONTA *)lParam, &lfW);
        else
          xmemcpy(&lfW, (LOGFONTW *)lParam, sizeof(LOGFONTW));

        if (SetChosenFont(hWnd, &lfW))
        {
          if (lpFrame=GetFrameDataFromEditWindow(hWnd))
          {
            xmemcpy(&lpFrame->lf, &lfW, sizeof(LOGFONTW));
            UpdateMappedPrintWidth(lpFrame);
            UpdateStatusUser(lpFrame, CSB_FONTPOINT|CSB_MARKER);
          }
          return TRUE;
        }
        return FALSE;
      }
      case AKD_GETCODEPAGELIST:
      {
        int *lpnCodePage=(int *)wParam;

        if (lpnCodePage)
        {
          *lpnCodePage=moCur.nDefaultCodePage;
        }
        return (LRESULT)lpCodepageList;
      }
      case AKD_RECENTFILES:
      {
        if (wParam == RF_GET)
        {
          STACKRECENTFILE **rfs=(STACKRECENTFILE **)lParam;

          if (rfs) *rfs=&hRecentFilesStack;
          return moCur.nRecentFiles;
        }
        else if (wParam == RF_SET)
        {
          if (moCur.nRecentFiles != lParam)
          {
            moCur.nRecentFiles=(int)lParam;
            RecentFilesCut(&hRecentFilesStack);
          }
        }
        else if (wParam == RF_READ)
        {
          STACKRECENTFILE *rfs=(STACKRECENTFILE *)lParam;

          if (!rfs) rfs=&hRecentFilesStack;
          return RecentFilesRead(&moCur, rfs);
        }
        else if (wParam == RF_SAVE)
        {
          STACKRECENTFILE *rfs=(STACKRECENTFILE *)lParam;

          if (!rfs) rfs=&hRecentFilesStack;
          RecentFilesSave(rfs, moCur.nSaveHistory);
          bMenuRecentFiles=TRUE;
        }
        else if (wParam == RF_CLEAR)
        {
          STACKRECENTFILE *rfs=(STACKRECENTFILE *)lParam;

          if (!rfs) rfs=&hRecentFilesStack;
          RecentFilesZero(rfs);
        }
        else if (wParam == RF_DELETEOLD)
        {
          STACKRECENTFILE *rfs=(STACKRECENTFILE *)lParam;

          if (!rfs) rfs=&hRecentFilesStack;
          return RecentFilesDeleteOld(rfs);
        }
        else if (wParam == RF_FINDINDEX)
        {
          int nIndex;

          RecentFilesFindByName((wchar_t *)lParam, &nIndex);
          return nIndex;
        }
        else if (wParam == RF_DELETEINDEX)
        {
          RECENTFILE *rf;

          if (rf=RecentFilesFindByIndex((int)lParam))
          {
            RecentFilesDelete(&hRecentFilesStack, rf);
            bMenuRecentFiles=TRUE;
            return TRUE;
          }
        }
        else if (wParam == RF_FINDITEMBYINDEX)
        {
          return (LRESULT)RecentFilesFindByIndex((int)lParam);
        }
        else if (wParam == RF_FINDITEMBYNAME)
        {
          return (LRESULT)RecentFilesFindByName((wchar_t *)lParam, NULL);
        }
        else if (wParam == RF_DELETEITEM)
        {
          RecentFilesDelete(&hRecentFilesStack, (RECENTFILE *)lParam);
          bMenuRecentFiles=TRUE;
        }
        else if (wParam == RF_GETPARAM)
        {
          RECENTFILEPARAM *rfp=(RECENTFILEPARAM *)lParam;

          return (LRESULT)StackRecentFileParamGetByName(rfp->file, rfp->pParamName);
        }
        else if (wParam == RF_SETPARAM)
        {
          RECENTFILEPARAM *rfp=(RECENTFILEPARAM *)lParam;
          RECENTFILEPARAM *lpRecentFileParam;
          int nParamNameLen;
          int nParamValueLen;

          if (!(lpRecentFileParam=StackRecentFileParamGetByName(rfp->file, rfp->pParamName)))
            lpRecentFileParam=StackRecentFileParamAdd(rfp->file);

          if (lpRecentFileParam)
          {
            if (lpRecentFileParam->pParamName) API_FreeWide(lpRecentFileParam->pParamName);
            if (lpRecentFileParam->pParamValue) API_FreeWide(lpRecentFileParam->pParamValue);

            nParamNameLen=(int)xstrlenW(rfp->pParamName);
            if (lpRecentFileParam->pParamName=API_AllocWide(nParamNameLen + 1))
              xstrcpynW(lpRecentFileParam->pParamName, rfp->pParamName, nParamNameLen + 1);

            nParamValueLen=(int)xstrlenW(rfp->pParamValue);
            if (lpRecentFileParam->pParamValue=API_AllocWide(nParamValueLen + 1))
              xstrcpynW(lpRecentFileParam->pParamValue, rfp->pParamValue, nParamValueLen + 1);
          }
          return (LRESULT)lpRecentFileParam;
        }
        else if (wParam == RF_DELETEPARAM)
        {
          RECENTFILEPARAM *rfp=(RECENTFILEPARAM *)lParam;

          StackRecentFileParamDelete(rfp);
        }
        return 0;
      }
      case AKD_SEARCHHISTORY:
      {
        if (wParam == SH_GET)
        {
          return moCur.nSearchStrings;
        }
        else if (wParam == SH_CLEAR)
        {
          SearchZero(&hFindStack);
          SearchZero(&hReplaceStack);
          SearchSave(moCur.nSaveHistory);
          return moCur.nSearchStrings;
        }
        return 0;
      }
      case AKD_SETEDITNOTIFY:
      {
        SetEditNotify((HWND)wParam);
        return 0;
      }

      //Windows
      case AKD_GETMODELESS:
      {
        int *lpnType=(int *)lParam;

        if (!wParam || (HWND)wParam == hDlgModeless)
        {
          //Single
          if (lpnType) *lpnType=nModelessType;
          return (LRESULT)hDlgModeless;
        }
        else
        {
          //Stack
          MODELESS *lpModeless;

          if (lpModeless=StackModelessGet(&hModelessStack, (HWND)wParam))
          {
            if (lpnType) *lpnType=MLT_STACK;
            return (LRESULT)lpModeless->hWnd;
          }
          if (lpnType) *lpnType=MLT_NONE;
          return (LRESULT)NULL;
        }
      }
      case AKD_SETMODELESS:
      {
        if (lParam == MLA_SINGLE)
        {
          if ((nModelessType == MLT_NONE && wParam) ||
              (nModelessType == MLT_CUSTOM && !wParam))
          {
            hDlgModeless=(HWND)wParam;
            if (hDlgModeless)
              nModelessType=MLT_CUSTOM;
            else
              nModelessType=MLT_NONE;
            return TRUE;
          }
          return FALSE;
        }
        else if (lParam == MLA_ADD)
        {
          if (StackModelessAdd(&hModelessStack, (HWND)wParam))
            return TRUE;
          return FALSE;
        }
        else if (lParam == MLA_DELETE)
        {
          return StackModelessDelete(&hModelessStack, (HWND)wParam);
        }
        else if (lParam == MLA_CLEAR)
        {
          StackModelessFree(&hModelessStack);
          return TRUE;
        }
      }
      case AKD_RESIZE:
      {
        UpdateSize();
        return 0;
      }
      case AKD_DOCK:
      {
        DOCK *lpDock=(DOCK *)lParam;
        DOCK *lpResult=NULL;

        if (wParam & DK_ADD)
        {
          lpResult=StackDockAdd(&hDocksStack, lpDock);
          lpDock=lpResult;
        }
        if (wParam & DK_SUBCLASS)
        {
          if (lpDock->hWnd)
          {
            lpDock->lpOldDockProc=(WNDPROC)GetWindowLongPtrWide(lpDock->hWnd, GWLP_WNDPROC);
            SetWindowLongPtrWide(lpDock->hWnd, GWLP_WNDPROC, (UINT_PTR)DockProc);
          }
        }
        if (wParam & DK_UNSUBCLASS)
        {
          if (lpDock->hWnd)
          {
            SetWindowLongPtrWide(lpDock->hWnd, GWLP_WNDPROC, (UINT_PTR)lpDock->lpOldDockProc);
          }
        }
        if ((wParam & DK_SETLEFT) ||
            (wParam & DK_SETRIGHT) ||
            (wParam & DK_SETTOP) ||
            (wParam & DK_SETBOTTOM))
        {
          int nSide=0;

          if (wParam & DK_SETLEFT) nSide=DKS_LEFT;
          else if (wParam & DK_SETRIGHT) nSide=DKS_RIGHT;
          else if (wParam & DK_SETTOP) nSide=DKS_TOP;
          else if (wParam & DK_SETBOTTOM) nSide=DKS_BOTTOM;

          if (nSide)
          {
            DockSetSide(&hDocksStack, lpDock, nSide);
          }
        }
        if (wParam & DK_HIDE)
        {
          if (lpDock->hWnd)
          {
            lpDock->dwFlags|=DKF_HIDDEN;
            ShowWindow(lpDock->hWnd, SW_HIDE);
            UpdateSize();
          }
        }
        if (wParam & DK_SHOW)
        {
          if (lpDock->hWnd)
          {
            lpDock->dwFlags&=~DKF_HIDDEN;
            ShowWindow(lpDock->hWnd, SW_SHOW);
            UpdateSize();
          }
        }
        if ((wParam & DK_FINDDOCK) ||
            (wParam & DK_FINDCHILD))
        {
          if (lpDock->hWnd)
          {
            lpResult=StackDockFindWindow(&hDocksStack, lpDock->hWnd, (wParam & DK_FINDCHILD)?TRUE:FALSE);
          }
        }
        if (wParam & DK_DELETE)
        {
          if (lpDock)
          {
            StackDockDelete(&hDocksStack, lpDock);
          }
        }
        return (LRESULT)lpResult;
      }
      case AKD_SETBUTTONDRAW:
      {
        SetButtonDraw((HWND)wParam, (BUTTONDRAW *)lParam);
        return 0;
      }
      case AKD_SETHOTKEYINPUT:
      {
        SetWindowLongPtrWide((HWND)wParam, GWLP_USERDATA, lParam);
        lpOldHotkeyInputProc=(WNDPROC)GetWindowLongPtrWide((HWND)wParam, GWLP_WNDPROC);
        SetWindowLongPtrWide((HWND)wParam, GWLP_WNDPROC, (UINT_PTR)NewHotkeyInputProc);

        //Update cursor
        SendMessage((HWND)wParam, WM_SETCURSOR, 0, 0);
        return 0;
      }
      case AKD_RESIZEDIALOG:
      {
        RESIZEDIALOGMSG *rdsm=(RESIZEDIALOGMSG *)lParam;

        return ResizeDialogMessages(rdsm->rds, rdsm->rcMinMax, rdsm->rcCurrent, rdsm->dwFlags, rdsm->hDlg, rdsm->uMsg, rdsm->wParam, rdsm->lParam);
      }
      case AKD_UPDATESTATUSUSER:
      {
        UpdateStatusUser(lpFrameCurrent, (DWORD)-1);
        return 0;
      }

      //Frames
      case AKD_FRAMEACTIVATE:
      {
        return (LRESULT)ActivateFrameWindow((FRAMEDATA *)lParam, (DWORD)wParam);
      }
      case AKD_FRAMEDESTROY:
      {
        return DestroyFrameWindow((FRAMEDATA *)lParam);
      }
      case AKD_FRAMEFIND:
      case AKD_FRAMEFINDA:
      case AKD_FRAMEFINDW:
      {
        if (wParam == FWF_CURRENT)
        {
          if (nMDI == WMD_MDI)
          {
            if (!lpFrameCurrent->hWndEditParent)
              return (LRESULT)NULL;
          }
          return (LRESULT)lpFrameCurrent;
        }
        if (wParam == FWF_NEXT)
        {
          return (LRESULT)StackFrameGetNext(&hFramesStack, (FRAMEDATA *)lParam, FALSE);
        }
        if (wParam == FWF_PREV)
        {
          return (LRESULT)StackFrameGetNext(&hFramesStack, (FRAMEDATA *)lParam, TRUE);
        }
        if (wParam == FWF_BYINDEX)
          return (LRESULT)StackFrameGetByIndex(&hFramesStack, (int)lParam);
        if (wParam == FWF_BYFILENAME)
        {
          wchar_t *wpFileName=API_AllocWide(MAX_PATH);
          FRAMEDATA *lpResult;
          int nFileNameLen;

          if (uMsg == AKD_FRAMEFINDA || (bOldWindows && uMsg == AKD_FRAMEFIND))
            nFileNameLen=(int)xprintfW(wpFileName, L"%S", (char *)lParam);
          else
            nFileNameLen=(int)xprintfW(wpFileName, L"%s", (wchar_t *)lParam);
          lpResult=StackFrameGetByName(&hFramesStack, wpFileName, nFileNameLen);

          API_FreeWide(wpFileName);
          return (LRESULT)lpResult;
        }
        if (wParam == FWF_BYEDITWINDOW)
          return (LRESULT)GetFrameDataFromEditWindow((HWND)lParam);
        if (wParam == FWF_BYEDITDOCUMENT)
          return (LRESULT)GetFrameDataFromEditDocument((AEHDOC)lParam);
        if (wParam == FWF_BYTABINDEX)
          return (LRESULT)GetTabParamFromItem(hTab, (int)lParam);
        if (wParam == FWF_TABNEXT)
          return (LRESULT)GetNextTabFrame((FRAMEDATA *)lParam, FALSE);
        if (wParam == FWF_TABPREV)
          return (LRESULT)GetNextTabFrame((FRAMEDATA *)lParam, TRUE);
        return 0;
      }
      case AKD_FRAMESTATS:
      {
        if (wParam == FWS_COUNTALL)
          return nDocumentsCount;
        if (wParam == FWS_COUNTMODIFIED)
          return nDocumentsModified;
        if (wParam == FWS_COUNTSAVED)
          return nDocumentsCount - nDocumentsModified;
        if (wParam == FWS_CURSEL)
          return nDocumentIndex;
        if (wParam == FWS_COUNTNAMED)
        {
          FRAMEDATA *lpFrame;
          int nNamedCount=0;

          for (lpFrame=hFramesStack.first; lpFrame; lpFrame=lpFrame->next)
          {
            if (lpFrame->ei.wszFile[0])
              ++nNamedCount;
          }
          return nNamedCount;
        }
        if (wParam == FWS_COUNTFILE)
        {
          FRAMEDATA *lpFrame;
          wchar_t *wpFileName=(wchar_t *)lParam;
          int nFileNameLen=(int)xstrlenW(wpFileName);
          int nCount=0;

          for (lpFrame=hFramesStack.first; lpFrame; lpFrame=lpFrame->next)
          {
            if (lpFrame->nFileLen == nFileNameLen)
            {
              if (!xstrcmpiW(lpFrame->wszFile, wpFileName))
                ++nCount;
            }
          }
          return nCount;
        }
        return 0;
      }
      case AKD_FRAMENOWINDOWS:
      {
        return FrameNoWindows();
      }
      case AKD_FRAMEISVALID:
      {
        return (LRESULT)StackFrameIsValid(&hFramesStack, (FRAMEDATA *)lParam);
      }
      case AKD_FRAMEINDEX:
      {
        if (wParam)
          return StackFrameGetIndex(&hFramesStack, (FRAMEDATA *)lParam);
        else
          return GetTabItemFromParam(hTab, lParam);
      }
      case AKD_FRAMEINIT:
      {
        FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

        if (!lpFrame) lpFrame=lpFrameCurrent;
        CopyFrameData((FRAMEDATA *)lParam, lpFrame);
        return 0;
      }
      case AKD_FRAMEAPPLYEDIT:
      {
        SetEditWindowSettings((FRAMEDATA *)lParam);
        return 0;
      }

      //Thread
      case AKD_MEMCREATE:
      {
        return (LRESULT)MemCreate((const char *)wParam, (DWORD)lParam);
      }
      case AKD_MEMMAP:
      {
        return (LRESULT)MemMap((HANDLE)wParam, (DWORD)lParam);
      }
      case AKD_MEMUNMAP:
      {
        return (LRESULT)MemUnmap((void *)wParam);
      }
      case AKD_MEMCLOSE:
      {
        return (LRESULT)MemClose((HANDLE)wParam);
      }
      case AKD_STRLENA:
      {
        return xstrlenA((char *)wParam);
      }
      case AKD_STRLENW:
      {
        return xstrlenW((wchar_t *)wParam);
      }
      case AKD_CREATEWINDOW:
      {
        CREATEWINDOWW *cw=(CREATEWINDOWW *)lParam;

        return (LRESULT)CreateWindowExWide(0, cw->szClassName, cw->szWindowName, cw->dwStyle, cw->x, cw->y, cw->nWidth, cw->nHeight, cw->hWndParent, cw->hMenu, cw->hInstance, cw->lpParam);
      }
      case AKD_WAITKEYBOARD:
      {
        BYTE lpKeyState[256];
        MSG msg;
        int i;

        Loop:
        Sleep(0);

        if (GetKeyboardState(lpKeyState))
        {
          for (i=0; i < 256; ++i)
          {
            if (lpKeyState[i] & 0x80)
            {
              if (wParam) return 1;

              while (PeekMessageWide(&msg, NULL, 0, 0, PM_REMOVE))
              {
                TranslateMessageAll(TMSG_ALL, &msg);
              }
              goto Loop;
            }
          }
        }
        return 0;
      }
      case AKD_GETQUEUE:
      {
        return GetQueueStatus((UINT)wParam);
      }
      case AKD_TRANSLATEMESSAGE:
      {
        if (!lParam)
          return (LRESULT)TranslateMessageAll;
        return TranslateMessageAll((DWORD)wParam, (LPMSG)lParam);
      }
      case AKD_MESSAGEBOX:
      {
        DIALOGMESSAGEBOX *dmb=(DIALOGMESSAGEBOX *)lParam;

        return MessageBoxCustom(dmb->hWndParent, dmb->wpText, dmb->wpCaption, dmb->uType, dmb->hIcon, dmb->bmb);
      }
      case AKD_GETFOCUS:
      {
        return (LRESULT)GetFocus();
      }
      case AKD_PEEKMESSAGE:
      {
        return (LRESULT)PeekMessage((LPMSG)lParam, (HWND)wParam, 0, 0, PM_REMOVE);
      }
      case AKD_UNIQUEID:
      {
        return ++nUniqueID;
      }
      case AKD_GETCLIPBOARDTEXT:
      {
        return GetClipboardText((BOOL)wParam, (wchar_t **)lParam, MAXINT_PTR);
      }
      case AKD_SETCLIPBOARDTEXT:
      {
        return SetClipboardText((const wchar_t *)wParam, lParam);
      }

      //Plugin options
      case AKD_BEGINOPTIONS:
      case AKD_BEGINOPTIONSA:
      case AKD_BEGINOPTIONSW:
      {
        if (moCur.nSaveSettings == SS_REGISTRY)
        {
          REGHANDLE *rh;

          if (rh=(REGHANDLE *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(REGHANDLE)))
          {
            rh->dwType=(DWORD)wParam;

            if (!(rh->dwType & POB_PROGRAM) && lParam)
            {
              if (uMsg == AKD_BEGINOPTIONSA || (bOldWindows && uMsg == AKD_BEGINOPTIONS))
                xprintfW(rh->wszKey, L"%s\\Plugs\\%s%S", APP_REGHOMEW, ((rh->dwType & POB_SCRIPTS) && !(rh->dwType & POB_PLUGS))?L"Scripts\\":L"", (char *)lParam);
              else
                xprintfW(rh->wszKey, L"%s\\Plugs\\%s%s", APP_REGHOMEW, ((rh->dwType & POB_SCRIPTS) && !(rh->dwType & POB_PLUGS))?L"Scripts\\":L"", (wchar_t *)lParam);
            }
            else xprintfW(rh->wszKey, L"%s\\Options", APP_REGHOMEW);

            if (rh->dwType & POB_READ)
            {
              if (RegOpenKeyExWide(HKEY_CURRENT_USER, rh->wszKey, 0, KEY_READ, &rh->hKey) != ERROR_SUCCESS)
              {
                API_HeapFree(hHeap, 0, (LPVOID)rh);
                rh=NULL;
              }
            }
            else if (rh->dwType & POB_SAVE)
            {
              if (rh->dwType & POB_CLEAR)
              {
                RegClearKeyWide(HKEY_CURRENT_USER, rh->wszKey);
              }
              if (RegCreateKeyExWide(HKEY_CURRENT_USER, rh->wszKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rh->hKey, NULL) != ERROR_SUCCESS)
              {
                API_HeapFree(hHeap, 0, (LPVOID)rh);
                rh=NULL;
              }
            }
          }
          return (LRESULT)rh;
        }
        else
        {
          INIHANDLE *ih;

          if (ih=(INIHANDLE *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(INIHANDLE)))
          {
            ih->dwType=(DWORD)wParam;

            if (!(ih->dwType & POB_PROGRAM) && lParam)
            {
              if (uMsg == AKD_BEGINOPTIONSA || (bOldWindows && uMsg == AKD_BEGINOPTIONS))
                xprintfW(ih->wszIniFile, L"%s\\AkelFiles\\Plugs\\%s%S.ini", wszExeDir, ((ih->dwType & POB_SCRIPTS) && !(ih->dwType & POB_PLUGS))?L"Scripts\\":L"", (char *)lParam);
              else
                xprintfW(ih->wszIniFile, L"%s\\AkelFiles\\Plugs\\%s%s.ini", wszExeDir, ((ih->dwType & POB_SCRIPTS) && !(ih->dwType & POB_PLUGS))?L"Scripts\\":L"", (wchar_t *)lParam);
            }
            else xstrcpynW(ih->wszIniFile, wszAkelPadIni, MAX_PATH);

            if (!OpenIni(&ih->hIniFile, ih->wszIniFile, (ih->dwType & POB_SAVE)))
            {
              API_HeapFree(hHeap, 0, (LPVOID)ih);
              ih=NULL;
            }
            else if ((ih->dwType & POB_SAVE) && (ih->dwType & POB_CLEAR))
            {
              INISECTION *lpIniSection;

              if (lpIniSection=StackOpenIniSection(&ih->hIniFile, L"Options", -1, FALSE))
                StackDeleteIniSection(&ih->hIniFile, lpIniSection, TRUE);
            }
          }
          return (LRESULT)ih;
        }
      }
      case AKD_OPTION:
      case AKD_OPTIONA:
      case AKD_OPTIONW:
      {
        PLUGINOPTIONW *po=(PLUGINOPTIONW *)lParam;
        wchar_t *wpOptionName;
        DWORD dwType=0;
        int nResult=0;
        BOOL bAnsi;

        if (uMsg == AKD_OPTIONA || (bOldWindows && uMsg == AKD_OPTION))
        {
          bAnsi=TRUE;
          wpOptionName=AllocWide((char *)po->pOptionName);
        }
        else
        {
          bAnsi=FALSE;
          wpOptionName=(wchar_t *)po->pOptionName;
        }

        if (moCur.nSaveSettings == SS_REGISTRY)
        {
          REGHANDLE *rh=(REGHANDLE *)wParam;
          BYTE *lpData;
          DWORD dwSize;

          if (po->dwType == PO_DWORD)
            dwType=REG_DWORD;
          else if (po->dwType == PO_BINARY)
            dwType=REG_BINARY;
          else if (po->dwType == PO_STRING)
            dwType=REG_SZ;

          if (rh->dwType & POB_READ)
          {
            if (po->dwType != PO_REMOVE)
            {
              if (po->dwType == PO_ENUM)
              {
                if (!po->lpData)
                {
                  //RegEnumValue not accept NULL
                  lpData=(LPBYTE)wbuf;
                  dwSize=BUFFER_SIZE;
                }
                else
                {
                  lpData=po->lpData;
                  dwSize=po->dwData;
                }
                if (RegEnumValueWide(rh->hKey, (DWORD)(UINT_PTR)wpOptionName, (wchar_t *)lpData, &dwSize, NULL, &dwType, NULL, NULL) == ERROR_SUCCESS)
                  nResult=(dwSize + 1) * sizeof(wchar_t);
                else
                  nResult=-1;
              }
              else
              {
                dwSize=po->dwData;
                if (RegQueryValueExWide(rh->hKey, wpOptionName, NULL, &dwType, (LPBYTE)po->lpData, &dwSize) == ERROR_SUCCESS)
                  nResult=dwSize;
                else
                  nResult=-1;
              }
            }
          }
          else if (rh->dwType & POB_SAVE)
          {
            if (po->dwType == PO_REMOVE)
            {
              if (RegDeleteValueWide(rh->hKey, wpOptionName) == ERROR_SUCCESS)
                nResult=TRUE;
            }
            else
            {
              if (RegSetValueExWide(rh->hKey, wpOptionName, 0, dwType, (LPBYTE)po->lpData, po->dwData) == ERROR_SUCCESS)
                nResult=TRUE;
            }
          }
        }
        else
        {
          INIHANDLE *ih=(INIHANDLE *)wParam;

          if (po->dwType == PO_DWORD)
            dwType=INI_DWORD;
          else if (po->dwType == PO_BINARY)
            dwType=INI_BINARY;
          else if (po->dwType == PO_STRING)
            dwType=bAnsi?INI_STRINGANSI:INI_STRINGUNICODE;

          if (ih->dwType & POB_READ)
          {
            if (po->dwType != PO_REMOVE)
            {
              if (po->dwType == PO_ENUM)
                nResult=IniEnumKey(&ih->hIniFile, L"Options", (int)(UINT_PTR)wpOptionName, (LPBYTE)po->lpData, po->dwData);
              else
                nResult=IniGetValue(&ih->hIniFile, L"Options", wpOptionName, dwType, (LPBYTE)po->lpData, po->dwData);
            }
          }
          else if (ih->dwType & POB_SAVE)
          {
            if (po->dwType == PO_REMOVE)
              nResult=IniDelKey(&ih->hIniFile, L"Options", wpOptionName);
            else
              nResult=IniSetValue(&ih->hIniFile, L"Options", wpOptionName, dwType, (LPBYTE)po->lpData, po->dwData);
          }
        }
        if (bAnsi)
          FreeWide(wpOptionName);
        return nResult;
      }
      case AKD_ENDOPTIONS:
      {
        BOOL bResult=FALSE;

        if (wParam)
        {
          if (moCur.nSaveSettings == SS_REGISTRY)
          {
            REGHANDLE *rh=(REGHANDLE *)wParam;

            if (RegCloseKey(rh->hKey) == ERROR_SUCCESS)
              bResult=TRUE;
            API_HeapFree(hHeap, 0, (LPVOID)rh);
          }
          else
          {
            INIHANDLE *ih=(INIHANDLE *)wParam;

            if ((ih->dwType & POB_READ) || !ih->hIniFile.bModified)
              bResult=TRUE;
            else if (ih->dwType & POB_SAVE)
              bResult=SaveIni(&ih->hIniFile, ih->wszIniFile);
            StackFreeIni(&ih->hIniFile);
            API_HeapFree(hHeap, 0, (LPVOID)ih);
          }
        }
        return bResult;
      }
      case AKD_INIOPEN:
      case AKD_INIOPENA:
      case AKD_INIOPENW:
      {
        INIHANDLE *ih;

        if (ih=(INIHANDLE *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(INIHANDLE)))
        {
          ih->dwType=(DWORD)wParam;

          if (uMsg == AKD_INIOPENA || (bOldWindows && uMsg == AKD_INIOPEN))
            xprintfW(ih->wszIniFile, L"%S", (char *)lParam);
          else
            xprintfW(ih->wszIniFile, L"%s", (wchar_t *)lParam);

          if (!OpenIni(&ih->hIniFile, ih->wszIniFile, (ih->dwType & POB_SAVE)))
          {
            API_HeapFree(hHeap, 0, (LPVOID)ih);
            ih=NULL;
          }
        }
        return (LRESULT)ih;
      }
      case AKD_INIGETSECTION:
      case AKD_INIGETSECTIONA:
      case AKD_INIGETSECTIONW:
      {
        INIHANDLE *ih=(INIHANDLE *)wParam;
        wchar_t *wpSection=API_AllocWide(MAX_PATH);
        int nSectionLen;
        INISECTION *lpResult;

        if (uMsg == AKD_INIGETSECTIONA || (bOldWindows && uMsg == AKD_INIGETSECTION))
          nSectionLen=(int)xprintfW(wpSection, L"%S", (char *)lParam);
        else
          nSectionLen=(int)xprintfW(wpSection, L"%s", (wchar_t *)lParam);
        lpResult=StackOpenIniSection(&ih->hIniFile, wpSection, nSectionLen, FALSE);

        API_FreeWide(wpSection);
        return (LRESULT)lpResult;
      }
      case AKD_INICLEARSECTION:
      case AKD_INIDELETESECTION:
      {
        INIHANDLE *ih=(INIHANDLE *)wParam;
        INISECTION *lpIniSection=(INISECTION *)lParam;

        return StackDeleteIniSection(&ih->hIniFile, lpIniSection, (uMsg == AKD_INICLEARSECTION)?TRUE:FALSE);
      }
      case AKD_INIGETKEY:
      case AKD_INIGETKEYA:
      case AKD_INIGETKEYW:
      {
        INISECTION *lpIniSection=(INISECTION *)wParam;
        wchar_t *wpKey=API_AllocWide(MAX_PATH);
        int nKeyLen;
        INIKEY *lpResult;

        if (uMsg == AKD_INIGETKEYA || (bOldWindows && uMsg == AKD_INIGETKEY))
          nKeyLen=(int)xprintfW(wpKey, L"%S", (char *)lParam);
        else
          nKeyLen=(int)xprintfW(wpKey, L"%s", (wchar_t *)lParam);
        lpResult=StackOpenIniKey(lpIniSection, wpKey, nKeyLen, FALSE);

        API_FreeWide(wpKey);
        return (LRESULT)lpResult;
      }
      case AKD_INIDELETEKEY:
      {
        INISECTION *lpIniSection=(INISECTION *)wParam;
        INIKEY *lpIniKey=(INIKEY *)lParam;

        return StackDeleteIniKey(lpIniSection, lpIniKey);
      }
      case AKD_INIGETVALUE:
      case AKD_INIGETVALUEA:
      case AKD_INIGETVALUEW:
      {
        INIHANDLE *ih=(INIHANDLE *)wParam;
        INIVALUEW *iv=(INIVALUEW *)lParam;
        wchar_t *wpSection=API_AllocWide(MAX_PATH);
        wchar_t *wpKey=API_AllocWide(MAX_PATH);
        int nResult;

        if (uMsg == AKD_INIGETVALUEA || (bOldWindows && uMsg == AKD_INIGETVALUE))
        {
          xprintfW(wpSection, L"%S", (char *)iv->pSection);
          xprintfW(wpKey, L"%S", (char *)iv->pKey);
        }
        else
        {
          xprintfW(wpSection, L"%s", (wchar_t *)iv->pSection);
          xprintfW(wpKey, L"%s", (wchar_t *)iv->pKey);
        }
        nResult=IniGetValue(&ih->hIniFile, wpSection, wpKey, iv->dwType, (LPBYTE)iv->lpData, iv->dwData);

        API_FreeWide(wpSection);
        API_FreeWide(wpKey);
        return (LRESULT)nResult;
      }
      case AKD_INISETVALUE:
      case AKD_INISETVALUEA:
      case AKD_INISETVALUEW:
      {
        INIHANDLE *ih=(INIHANDLE *)wParam;
        INIVALUEW *iv=(INIVALUEW *)lParam;
        wchar_t *wpSection=API_AllocWide(MAX_PATH);
        wchar_t *wpKey=API_AllocWide(MAX_PATH);
        int nResult;

        if (uMsg == AKD_INISETVALUEA || (bOldWindows && uMsg == AKD_INISETVALUE))
        {
          xprintfW(wpSection, L"%S", (char *)iv->pSection);
          xprintfW(wpKey, L"%S", (char *)iv->pKey);
        }
        else
        {
          xprintfW(wpSection, L"%s", (wchar_t *)iv->pSection);
          xprintfW(wpKey, L"%s", (wchar_t *)iv->pKey);
        }
        nResult=IniSetValue(&ih->hIniFile, wpSection, wpKey, iv->dwType, (LPBYTE)iv->lpData, iv->dwData);

        API_FreeWide(wpSection);
        API_FreeWide(wpKey);
        return (LRESULT)nResult;
      }
      case AKD_INICLOSE:
      {
        INIHANDLE *ih=(INIHANDLE *)wParam;
        BOOL bResult=FALSE;

        if (ih)
        {
          if ((ih->dwType & POB_READ) || !ih->hIniFile.bModified)
            bResult=TRUE;
          else if (ih->dwType & POB_SAVE)
            bResult=SaveIni(&ih->hIniFile, ih->wszIniFile);
          StackFreeIni(&ih->hIniFile);
          API_HeapFree(hHeap, 0, (LPVOID)ih);
        }
        return bResult;
      }

      //Regular expressions
      case AKD_PATEXEC:
      {
        return PatStructExec((PATEXEC *)lParam);
      }
      case AKD_PATREPLACE:
      {
        return PatReplace((PATREPLACE *)lParam);
      }
      case AKD_PATGROUPSTR:
      {
        return PatGroupStr((PATGROUPSTR *)lParam);
      }
      case AKD_PATGETGROUP:
      {
        return (LRESULT)PatGetGroup((STACKREGROUP *)wParam, (int)lParam);
      }
      case AKD_PATNEXTGROUP:
      {
        return (LRESULT)PatNextGroup((REGROUP *)wParam);
      }
      case AKD_PATPREVGROUP:
      {
        return (LRESULT)PatPrevGroup((REGROUP *)wParam);
      }
      case AKD_PATFREE:
      {
        PatStructFree((PATEXEC *)lParam);
        return 0;
      }

      //AkelPad 4.x only messages
      case AKD_EXGETTEXTLENGTH:
      {
        HWND hWnd=(HWND)wParam;

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        return -IndexSubtract(hWnd, NULL, NULL, (int)lParam, FALSE);
      }
      case AKD_EXGETTEXTRANGE:
      case AKD_EXGETTEXTRANGEA:
      case AKD_EXGETTEXTRANGEW:
      {
        EXGETTEXTRANGE *tr=(EXGETTEXTRANGE *)lParam;
        HWND hWnd=(HWND)wParam;

        if (!hWnd)
          hWnd=lpFrameCurrent->ei.hWndEdit;

        if (uMsg == AKD_EXGETTEXTRANGEA || (bOldWindows && uMsg == AKD_EXGETTEXTRANGE))
          return ExGetRangeTextA(hWnd, tr->nCodePage, tr->lpDefaultChar, tr->lpUsedDefChar, &tr->cr.ciMin, &tr->cr.ciMax, tr->bColumnSel, (char **)&tr->pText, tr->nNewLine, TRUE);
        else
          return ExGetRangeTextW(hWnd, &tr->cr.ciMin, &tr->cr.ciMax, tr->bColumnSel, (wchar_t **)&tr->pText, tr->nNewLine, TRUE);
      }
    }
  }

  if (uMsg == WM_COPYDATA)
  {
    COPYDATASTRUCT *cds=(COPYDATASTRUCT *)lParam;
    int nResult=0;

    if (cds->dwData == CD_OPENDOCUMENT ||
        cds->dwData == CD_OPENDOCUMENTA ||
        cds->dwData == CD_OPENDOCUMENTW)
    {
      OPENDOCUMENTPOSTA *odpA=(OPENDOCUMENTPOSTA *)cds->lpData;
      OPENDOCUMENTPOSTW *odpW=(OPENDOCUMENTPOSTW *)cds->lpData;
      wchar_t *wpFile=API_AllocWide(MAX_PATH);
      wchar_t *wpWorkDir=API_AllocWide(MAX_PATH);

      if ((odpA->hWnd && !IsEditActive(odpA->hWnd)) || nMDI || SaveChanged(0))
      {
        if (cds->dwData == CD_OPENDOCUMENTA || (bOldWindows && cds->dwData == CD_OPENDOCUMENT))
        {
          xprintfW(wpFile, L"%S", (char *)odpA->szFile);
          xprintfW(wpWorkDir, L"%S", (char *)odpA->szWorkDir);
        }
        else
        {
          xprintfW(wpFile, L"%s", (wchar_t *)odpW->szFile);
          xprintfW(wpWorkDir, L"%s", (wchar_t *)odpW->szWorkDir);
        }

        if (*wpWorkDir) SetCurrentDirectoryWide(wpWorkDir);
        nResult=OpenDocument(odpA->hWnd, odpA->hDoc, wpFile, odpA->dwFlags, odpA->nCodePage, odpA->bBOM);
        if (*wpWorkDir) SetCurrentDirectoryWide(wszExeDir);
      }
      API_FreeWide(wpFile);
      API_FreeWide(wpWorkDir);
    }
    else if (cds->dwData == CD_PARSECMDLINEW)
    {
      PARSECMDLINEPOSTW *pclp=(PARSECMDLINEPOSTW *)cds->lpData;
      PMPARSECMDLINEW *pmpcl;

      if (pmpcl=(PMPARSECMDLINEW *)GlobalAlloc(GMEM_FIXED, sizeof(PMPARSECMDLINEW) + (pclp->nCmdLineLen + 1) * sizeof(wchar_t) + (pclp->nWorkDirLen + 1) * sizeof(wchar_t)))
      {
        //In this place: command line string in size (pclp->nCmdLineLen + 1) * sizeof(wchar_t).
        pmpcl->pcls.pCmdLine=(wchar_t *)((unsigned char *)pmpcl + sizeof(PMPARSECMDLINEW));
        xstrcpynW((wchar_t *)pmpcl->pcls.pCmdLine, pclp->szCmdLine, pclp->nCmdLineLen + 1);

        //In this place: working directory string in size (pclp->nWorkDirLen + 1) * sizeof(wchar_t).
        pmpcl->pcls.pWorkDir=(wchar_t *)((unsigned char *)pmpcl->pcls.pCmdLine + (pclp->nCmdLineLen + 1) * sizeof(wchar_t));
        xstrcpynW((wchar_t *)pmpcl->pcls.pWorkDir, pclp->szWorkDir, pclp->nWorkDirLen + 1);
      }
      bCmdLineQuitAsEnd=pclp->bQuitAsEnd;

      if (pclp->bPostMessage)
      {
        //Post message
        pmpcl->pm.hWnd=hMainWnd;
        pmpcl->pm.uMsg=AKD_PARSECMDLINEW;
        pmpcl->pm.wParam=0;
        pmpcl->pm.lParam=(LPARAM)&pmpcl->pcls;
        PostMessage(hMainWnd, AKD_POSTMESSAGE, 0, (LPARAM)pmpcl);
      }
      else nResult=(int)SendMessage(hMainWnd, AKD_PARSECMDLINEW, 0, (LPARAM)&pmpcl->pcls);
    }
    return nResult;
  }
  else if (uMsg == WM_ENTERMENULOOP)
  {
    bEnterMenuLoop=TRUE;
  }
  else if (uMsg == WM_EXITMENULOOP)
  {
    bEnterMenuLoop=FALSE;
  }
  else if (uMsg == WM_INITMENU)
  {
    if (!lParam || (lParam & IMENU_EDIT))
    {
      int nMenuState;

      if (lpFrameCurrent->ei.hWndEdit)
      {
        EnableMenuItem(hMainMenu, IDM_FILE_CREATENEW, (nMDI && (moCur.dwSingleOpenProgram & SOP_ON))?MF_GRAYED:MF_ENABLED);
        EnableMenuItem(hMainMenu, IDM_FILE_REOPEN, (lpFrameCurrent->wszFile[0])?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_FILE_SAVE, (lpFrameCurrent->ei.bModified || !lpFrameCurrent->wszFile[0])?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_FILE_SAVEALL, nDocumentsModified?MF_ENABLED:MF_GRAYED);

        EnableMenuItem(hMainMenu, IDM_EDIT_UNDO, (!lpFrameCurrent->ei.bReadOnly && SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANUNDO, 0, 0))?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_EDIT_REDO, (!lpFrameCurrent->ei.bReadOnly && SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANREDO, 0, 0))?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_EDIT_PASTE, (!lpFrameCurrent->ei.bReadOnly && SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANPASTE, 0, 0))?MF_ENABLED:MF_GRAYED);

        EnableMenuItem(hMainMenu, IDM_EDIT_CUT, (!lpFrameCurrent->ei.bReadOnly && AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax))?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_EDIT_CLEAR, (!lpFrameCurrent->ei.bReadOnly && AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax))?MF_ENABLED:MF_GRAYED);

        EnableMenuItem(hMainMenu, IDM_EDIT_COPY, AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax)?MF_ENABLED:MF_GRAYED);

        nMenuState=AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax)?MF_ENABLED:MF_GRAYED;
        EnableMenuItem(hMainMenu, IDM_EDIT_DELETE_FIRST_CHAR_MENU, nMenuState);
        EnableMenuItem(hMainMenu, IDM_EDIT_DELETESELWHITESPACES, nMenuState);

        nMenuState=(!DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK|STRSEL_MULTILINE, NULL))?MF_GRAYED:MF_ENABLED;
        EnableMenuItem(hMainMenu, IDM_EDIT_INSERT_TAB_MENU, nMenuState);
        EnableMenuItem(hMainMenu, IDM_EDIT_DELETE_TAB_MENU, nMenuState);
        EnableMenuItem(hMainMenu, IDM_EDIT_INSERT_SPACE_MENU, nMenuState);
        EnableMenuItem(hMainMenu, IDM_EDIT_DELETE_SPACE_MENU, nMenuState);
      }
      if (nMDI)
      {
        EnableMenuItem(hMainMenu, IDM_WINDOW_FRAMECLONE, (!moCur.bSingleOpenFile && lpFrameCurrent->ei.hWndEdit && lpFrameCurrent->wszFile[0])?MF_ENABLED:MF_GRAYED);
      }
      EnableMenuItem(hMainMenu, IDM_OPTIONS_EXEC, (*moCur.wszExecuteCommand)?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMainMenu, IDM_MANUAL, GetUserManual(NULL, BUFFER_SIZE)?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMainMenu, IDM_UPDATE, FileExistsWide(wszAkelUpdaterExe)?MF_ENABLED:MF_GRAYED);
    }
    if (!lParam || (lParam & IMENU_CHECKS))
    {
      if (lpFrameCurrent->ei.nNewLine == NEWLINE_WIN)
        CheckMenuRadioItem(hMainMenu, IDM_EDIT_NEWLINE_WIN, IDM_EDIT_NEWLINE_MAC, IDM_EDIT_NEWLINE_WIN, MF_BYCOMMAND);
      else if (lpFrameCurrent->ei.nNewLine == NEWLINE_UNIX)
        CheckMenuRadioItem(hMainMenu, IDM_EDIT_NEWLINE_WIN, IDM_EDIT_NEWLINE_MAC, IDM_EDIT_NEWLINE_UNIX, MF_BYCOMMAND);
      else if (lpFrameCurrent->ei.nNewLine == NEWLINE_MAC)
        CheckMenuRadioItem(hMainMenu, IDM_EDIT_NEWLINE_WIN, IDM_EDIT_NEWLINE_MAC, IDM_EDIT_NEWLINE_MAC, MF_BYCOMMAND);
      CheckMenuItem(hMainMenu, IDM_VIEW_READONLY, lpFrameCurrent->ei.bReadOnly?MF_CHECKED:MF_UNCHECKED);
      CheckMenuItem(hMainMenu, IDM_VIEW_WORDWRAP, lpFrameCurrent->ei.bWordWrap?MF_CHECKED:MF_UNCHECKED);
      CheckMenuItem(hMainMenu, IDM_VIEW_SPLIT_WINDOW_ONOFF, lpFrameCurrent->ei.hWndMaster?MF_CHECKED:MF_UNCHECKED);
    }
    bMenuLanguage=TRUE;
    if (lParam) return TRUE;
  }
  else if (uMsg == WM_INITMENUPOPUP)
  {
    if ((HMENU)wParam == hMenuLanguage)
    {
      if (bMenuLanguage)
      {
        bMenuLanguage=FALSE;
        LanguageMenu();
      }
    }
    else if ((HMENU)wParam == hMenuRecentFiles)
    {
      if (bMenuRecentFiles)
      {
        bMenuRecentFiles=FALSE;
        RecentFilesMenu();
      }
    }
    else if ((HMENU)wParam == hPopupOpenCodepage ||
             (HMENU)wParam == hPopupSaveCodepage)
    {
      if (bMenuPopupCodepage)
      {
        bMenuPopupCodepage=FALSE;
        FillMenuPopupCodepage();
      }

      //Check radio item
      {
        int nCurrentCodePageIndex;

        nCurrentCodePageIndex=CodepageListFind(lpCodepageList, lpFrameCurrent->ei.nCodePage);
        CheckMenuRadioItem(hPopupOpenCodepage, 0, nCodepageListLen - 1, nCurrentCodePageIndex, MF_BYPOSITION);
        CheckMenuRadioItem(hPopupSaveCodepage, 0, nCodepageListLen - 1, nCurrentCodePageIndex, MF_BYPOSITION);
      }
    }
  }
  else if (uMsg == WM_ACTIVATEAPP)
  {
    //if (nMDI == WMD_SDI)
    //{
    //  if (wParam)
    //  {
    //    //Activate keyboard layout
    //    if (moCur.dwKeybLayoutOptions & KLO_REMEMBERLAYOUT)
    //      ActivateKeyboard(lpFrameCurrent->dwInputLocale);
    //  }
    //  else
    //  {
    //    //Remember keyboard layout
    //    if (moCur.dwKeybLayoutOptions & KLO_REMEMBERLAYOUT)
    //      lpFrameCurrent->dwInputLocale=(HKL)GetKeyboardLayout(0);
    //  }
    //}
    if (wParam)
      UpdateStatusUser(lpFrameCurrent, CSB_NUMLOCK|CSB_CAPSLOCK);
  }
  else if (uMsg == WM_WINDOWPOSCHANGING)
  {
    WINDOWPOS *lpWindowPos=(WINDOWPOS *)lParam;

    if (!(lpWindowPos->flags & SWP_NOCOPYBITS))
    {
      if ((!(lpWindowPos->flags & SWP_NOMOVE) && (lpWindowPos->x != moCur.rcMainWindowRestored.left || lpWindowPos->y != moCur.rcMainWindowRestored.top)) &&
          (!(lpWindowPos->flags & SWP_NOSIZE) && (lpWindowPos->cx != moCur.rcMainWindowRestored.right || lpWindowPos->cy != moCur.rcMainWindowRestored.bottom)))
      {
        //Size and position changed don't copy bits to avoid blinking.
        lpWindowPos->flags|=SWP_NOCOPYBITS;
      }
    }
  }
  else if (uMsg == WM_MOVE)
  {
    UINT_PTR dwStyle=GetWindowLongPtrWide(hWnd, GWL_STYLE);

    if (!(dwStyle & WS_MAXIMIZE) && !(dwStyle & WS_MINIMIZE))
    {
      GetWindowSize(hWnd, NULL, &moCur.rcMainWindowRestored);
    }
  }
  else if (uMsg == WM_SIZE)
  {
    UINT_PTR dwStyle=GetWindowLongPtrWide(hWnd, GWL_STYLE);

    if (!(dwStyle & WS_MINIMIZE))
    {
      if (LOWORD(lParam) && HIWORD(lParam))
      {
        if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
          dwLastMainSizeType=(DWORD)wParam;

        if (wParam != SIZE_MAXIMIZED)
        {
          GetWindowSize(hWnd, NULL, &moCur.rcMainWindowRestored);
        }
        UpdateSize();
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_ERASEBKGND)
  {
    //We don't use HOLLOW_BRUSH for hbrBackground,
    //so deny erase background to avoid flickering.
    return 1;
  }
  else if (uMsg == WM_DROPFILES)
  {
    DropFiles((HDROP)wParam);
    return TRUE;
  }
  else if (uMsg == WM_COMMAND)
  {
    //WM_COMMAND (SDI/MDI/PMDI)
    WORD wCommand=LOWORD(wParam);

    //Ignore zero command
    if (!wCommand)
      return 0;

    switch (wCommand)
    {
      //File
      case IDM_FILE_NEW:
      {
        return DoFileNew();
      }
      case IDM_FILE_CREATENEW:
      {
        if (!nMDI || !(moCur.dwSingleOpenProgram & SOP_ON))
        {
          SaveOptions(&moCur, lpFrameCurrent, moCur.nSaveSettings, FALSE);
        }
        return (LRESULT)DoFileNewWindow(0);
      }
      case IDM_FILE_OPEN:
      {
        if (lParam)
          xstrcpynW(moCur.wszOfnDir, moCur.wszLastDir, MAX_PATH);
        return DoFileOpen();
      }
      case IDM_FILE_REOPEN:
      {
        return DoFileReopenAs(OD_ADT_DETECTBOM, lpFrameCurrent->ei.nCodePage, FALSE);
      }
      case IDM_FILE_SAVE:
      {
        return DoFileSave();
      }
      case IDM_FILE_SAVEAS:
      {
        if (lParam)
          xstrcpynW(moCur.wszOfnDir, moCur.wszLastDir, MAX_PATH);
        return DoFileSaveAs(-1, -1);
      }
      case IDM_FILE_SAVEALL:
      {
        if (!nMDI)
        {
          return DoFileSave();
        }
        else
        {
          if (nDocumentsModified)
          {
            FRAMEDATA *lpFrameInit=lpFrameCurrent;

            do
            {
              if (!DoFileSave()) return FALSE;
              lpFrameCurrent=ActivateFrameWindow(lpFrameCurrent, FWA_NEXT|FWA_NOUPDATEORDER);
            }
            while (lpFrameCurrent != lpFrameInit);
          }
        }
        return TRUE;
      }
      case IDM_FILE_SAVEALLAS:
      {
        return DoFileSaveAllAs();
      }
      case IDM_FILE_REDETECT:
      {
        if (!lpFrameCurrent->wszFile[0])
        {
          DoEditRecode();
          return EOD_SUCCESS;
        }
        return DoFileReopenAs(OD_ADT_BINARYERROR|OD_ADT_DETECTCODEPAGE|OD_ADT_DETECTBOM, 0, FALSE);
      }
      case IDM_FILE_REOPENAS_ANSI:
      {
        return DoFileReopenAs(0, nAnsiCodePage, FALSE);
      }
      case IDM_FILE_REOPENAS_OEM:
      {
        return DoFileReopenAs(0, nOemCodePage, FALSE);
      }
      case IDM_FILE_REOPENAS_KOIR:
      {
        return DoFileReopenAs(0, CP_KOI8_R, FALSE);
      }
      case IDM_FILE_REOPENAS_UTF16LE:
      {
        return DoFileReopenAs(OD_ADT_DETECTBOM, CP_UNICODE_UTF16LE, FALSE);
      }
      case IDM_FILE_REOPENAS_UTF16BE:
      {
        return DoFileReopenAs(OD_ADT_DETECTBOM, CP_UNICODE_UTF16BE, FALSE);
      }
      case IDM_FILE_REOPENAS_UTF8:
      {
        return DoFileReopenAs(OD_ADT_DETECTBOM, CP_UNICODE_UTF8, FALSE);
      }
      case IDM_FILE_SAVEAS_ANSI:
      {
        return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, nAnsiCodePage, FALSE, SD_UPDATE);
      }
      case IDM_FILE_SAVEAS_OEM:
      {
        return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, nOemCodePage, FALSE, SD_UPDATE);
      }
      case IDM_FILE_SAVEAS_KOIR:
      {
        return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, CP_KOI8_R, FALSE, SD_UPDATE);
      }
      case IDM_FILE_SAVEAS_UTF16LE:
      {
        return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, CP_UNICODE_UTF16LE, TRUE, SD_UPDATE);
      }
      case IDM_FILE_SAVEAS_UTF16BE:
      {
        return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, CP_UNICODE_UTF16BE, TRUE, SD_UPDATE);
      }
      case IDM_FILE_SAVEAS_UTF8:
      {
        return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, CP_UNICODE_UTF8, TRUE, SD_UPDATE);
      }
      case IDM_FILE_SAVEAS_UTF8_NOBOM:
      {
        return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, CP_UNICODE_UTF8, FALSE, SD_UPDATE);
      }
      case IDM_FILE_CODEPAGEMENU:
      {
        RECT rc;

        SendMessage(hStatus, SB_GETRECT, SBP_CODEPAGE, (LPARAM)&rc);
        ClientToScreen(hStatus, (POINT *)&rc);
        ShowMenuPopupCodepage((POINT *)&rc, FALSE);
        return 0;
      }
      case IDM_FILE_PAGESETUP:
      {
        return DoFilePageSetup(hMainWnd);
      }
      case IDM_FILE_PRINT:
      {
        FRAMEDATA *lpFrame=lParam?GetFrameDataFromEditWindow((HWND)lParam):lpFrameCurrent;

        return DoFilePrint(lpFrame, FALSE);
      }
      case IDM_FILE_PRINTPREVIEW:
      {
        HWND hWndForPreview=lParam?(HWND)lParam:lpFrameCurrent->ei.hWndEdit;

        DoFilePreview(hWndForPreview);
        return 0;
      }
      case IDM_FILE_SILENTPRINT:
      {
        FRAMEDATA *lpFrame=lParam?GetFrameDataFromEditWindow((HWND)lParam):lpFrameCurrent;

        return DoFilePrint(lpFrame, TRUE);
      }
      case IDM_FILE_EXIT:
      {
        #ifndef AKELPAD_DLLBUILD
          PostMessage(hWnd, WM_CLOSE, 0, 0);
        #endif
        return 0;
      }

      //View
      case IDM_VIEW_FONT:
      {
        if (DoViewFont(hMainWnd, &lpFrameCurrent->lf))
        {
          if (SetChosenFont(lpFrameCurrent->ei.hWndEdit, &lpFrameCurrent->lf))
          {
            UpdateMappedPrintWidth(lpFrameCurrent);
            UpdateStatusUser(lpFrameCurrent, CSB_FONTPOINT|CSB_MARKER);
            return TRUE;
          }
        }
        return FALSE;
      }
      case IDM_VIEW_COLORS:
      {
        return DoViewColors();
      }
      case IDM_VIEW_FONTSIZE_INCREASE:
      {
        DoViewFontSize(lpFrameCurrent, FONTSIZE_INCREASE);
        return 0;
      }
      case IDM_VIEW_FONTSIZE_DECREASE:
      {
        DoViewFontSize(lpFrameCurrent, FONTSIZE_DECREASE);
        return 0;
      }
      case IDM_VIEW_FONTSIZE_RESTORE:
      {
        DoViewFontSize(lpFrameCurrent, FONTSIZE_RESTORE);
        return 0;
      }
      case IDM_VIEW_READONLY:
      {
        DoViewReadOnly(lpFrameCurrent, !lpFrameCurrent->ei.bReadOnly);
        if (hDlgModeless && nModelessType != MLT_CUSTOM)
          SendMessage(hDlgModeless, WM_COMMAND, IDC_SETREADONLY, 0);
        return lpFrameCurrent->ei.bReadOnly;
      }
      case IDM_VIEW_WORDWRAP:
      {
        DoViewWordWrap(lpFrameCurrent, !lpFrameCurrent->ei.bWordWrap);
        return lpFrameCurrent->ei.bWordWrap;
      }
      case IDM_VIEW_SPLIT_WINDOW_ALL:
      case IDM_VIEW_SPLIT_WINDOW_WE:
      case IDM_VIEW_SPLIT_WINDOW_NS:
      case IDM_VIEW_SPLIT_WINDOW_ONOFF:
      {
        int nNewSplit=wCommand;

        if (nNewSplit == IDM_VIEW_SPLIT_WINDOW_ONOFF)
        {
          nNewSplit=IDM_VIEW_SPLIT_WINDOW_ALL;
        }
        else if (lpFrameCurrent->ei.hWndMaster)
        {
          if (nLastSplit != nNewSplit)
            DoViewSplitWindow(!lpFrameCurrent->ei.hWndMaster, nNewSplit);
        }
        nLastSplit=nNewSplit;

        DoViewSplitWindow(!lpFrameCurrent->ei.hWndMaster, nNewSplit);
        return 0;
      }
      case IDM_VIEW_ONTOP:
      {
        DoViewOnTop(!moCur.bOnTop);
        return moCur.bOnTop;
      }
      case IDM_VIEW_SHOW_STATUSBAR:
      {
        DoViewShowStatusBar(!moCur.bStatusBar);
        return moCur.bStatusBar;
      }

      //Options
      case IDM_OPTIONS_EXEC:
      {
        return DoSettingsExec();
      }
      case IDM_OPTIONS_KEEPSPACE:
      {
        DoSettingsKeepSpace(!moCur.bKeepSpace);
        return moCur.bKeepSpace;
      }
      case IDM_OPTIONS_WATCHFILE:
      {
        DoSettingsWatchFile(!moCur.bWatchFile);
        return moCur.bWatchFile;
      }
      case IDM_OPTIONS_SAVETIME:
      {
        DoSettingsSaveTime(!moCur.bSaveTime);
        return moCur.bSaveTime;
      }
      case IDM_OPTIONS_SINGLEOPEN_FILE:
      {
        DoSettingsSingleOpenFile(!moCur.bSingleOpenFile);
        SaveOptions(&moCur, lpFrameCurrent, moCur.nSaveSettings, FALSE);
        return moCur.bSingleOpenFile;
      }
      case IDM_OPTIONS_SINGLEOPEN_PROGRAM:
      {
        DWORD dwState=moCur.dwSingleOpenProgram;

        if (dwState & SOP_ON)
          dwState&=~SOP_ON;
        else
          dwState|=SOP_ON;
        DoSettingsSingleOpenProgram(dwState);
        SaveOptions(&moCur, lpFrameCurrent, moCur.nSaveSettings, FALSE);
        return moCur.dwSingleOpenProgram;
      }
      case IDM_OPTIONS_SDI:
      case IDM_OPTIONS_MDI:
      case IDM_OPTIONS_PMDI:
      {
        int nMode=WMD_SDI;

        if (wCommand == IDM_OPTIONS_SDI)
          nMode=WMD_SDI;
        else if (wCommand == IDM_OPTIONS_MDI)
          nMode=WMD_MDI;
        else if (wCommand == IDM_OPTIONS_PMDI)
          nMode=WMD_PMDI;

        if (nMDI != nMode)
        {
          API_LoadString(hLangModule, MSG_RESTART_PROGRAM, wszMsg, BUFFER_SIZE);
          API_MessageBox(hWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
          moCur.nMDI=nMode;
        }
        return 0;
      }
      case IDM_OPTIONS:
      {
        DoSettingsOptions();
        return 0;
      }
      case IDM_OPTIONS_PLUGINS:
      {
        DoSettingsPlugins();
        return 0;
      }

      //Window
      case IDM_SELECTWINDOW:
      case IDM_WINDOW_MDILIST:
      {
        if (nMDI) DoWindowSelectWindow();
        return 0;
      }
      case IDM_WINDOW_TABVIEW_TOP:
      {
        if (nMDI) DoWindowTabView(TAB_VIEW_TOP, FALSE);
        return 0;
      }
      case IDM_WINDOW_TABVIEW_BOTTOM:
      {
        if (nMDI) DoWindowTabView(TAB_VIEW_BOTTOM, FALSE);
        return 0;
      }
      case IDM_WINDOW_TABVIEW_NONE:
      {
        if (nMDI) DoWindowTabView(TAB_VIEW_NONE, FALSE);
        return 0;
      }
      case IDM_WINDOW_TABTYPE_STANDARD:
      {
        if (nMDI) DoWindowTabType(TAB_TYPE_STANDARD, FALSE);
        return 0;
      }
      case IDM_WINDOW_TABTYPE_BUTTONS:
      {
        if (nMDI) DoWindowTabType(TAB_TYPE_BUTTONS, FALSE);
        return 0;
      }
      case IDM_WINDOW_TABTYPE_FLATBUTTONS:
      {
        if (nMDI) DoWindowTabType(TAB_TYPE_FLATBUTTONS, FALSE);
        return 0;
      }
      case IDM_WINDOW_TABSWITCH_NEXTPREV:
      {
        if (nMDI) DoWindowTabSwitch(TAB_SWITCH_NEXTPREV, FALSE);
        return 0;
      }
      case IDM_WINDOW_TABSWITCH_RIGHTLEFT:
      {
        if (nMDI) DoWindowTabSwitch(TAB_SWITCH_RIGHTLEFT, FALSE);
        return 0;
      }
      case IDM_WINDOW_FRAMENEXT:
      {
        if (!nMDI) return (LRESULT)NULL;
        return (LRESULT)ActivateFrameWindow(lpFrameCurrent, FWA_NEXT|FWA_NOUPDATEORDER);
      }
      case IDM_WINDOW_FRAMEPREV:
      {
        if (!nMDI) return (LRESULT)NULL;
        return (LRESULT)ActivateFrameWindow(lpFrameCurrent, FWA_PREV|FWA_NOUPDATEORDER);
      }
      case IDM_WINDOW_FRAMECLOSE:
      {
        if (!nMDI)
          return CloseDocument(0);
        else
          return !DestroyFrameWindow(lpFrameCurrent);
      }
      case IDM_WINDOW_FRAMECLOSEALL:
      {
        if (!nMDI)
        {
          return CloseDocument(0);
        }
        else
        {
          BOOL bResult=TRUE;

          //Show "No to all" button if necessary
          if (nDocumentsModified > 1)
            dwChangedPrompt|=PROMPT_NOTOALLBUTTON;

          while (lpFrameCurrent->hWndEditParent)
          {
            if (DestroyFrameWindow(lpFrameCurrent) != FWDE_SUCCESS)
            {
              bResult=FALSE;
              break;
            }
          }
          dwChangedPrompt=0;

          return bResult;
        }
      }
      case IDM_WINDOW_FRAMECLOSEALL_BUTACTIVE:
      {
        FRAMEDATA *lpFrameInit=lpFrameCurrent;
        BOOL bResult=TRUE;

        if (!nMDI) return FALSE;

        //Show "No to all" button if necessary
        if (nDocumentsModified > 1)
          dwChangedPrompt|=PROMPT_NOTOALLBUTTON;

        for (;;)
        {
          lpFrameCurrent=ActivateFrameWindow(lpFrameCurrent, FWA_NEXT);
          if (lpFrameCurrent == lpFrameInit) break;

          if (DestroyFrameWindow(lpFrameCurrent) != FWDE_SUCCESS)
          {
            bResult=FALSE;
            break;
          }
        }
        dwChangedPrompt=0;

        return bResult;
      }
      case IDM_WINDOW_FRAMECLOSEALL_UNMODIFIED:
      {
        BOOL bModified=(BOOL)lParam;

        if (!nMDI)
        {
          if (lpFrameCurrent->ei.bModified == bModified)
            return CloseDocument(0);
          return TRUE;
        }
        else
        {
          FRAMEDATA *lpFrameInit=NULL;
          BOOL bResult=TRUE;

          if (bModified)
          {
            if (nDocumentsModified == 0)
              return TRUE;

            //Show "No to all" button if necessary
            if (nDocumentsModified > 1)
              dwChangedPrompt|=PROMPT_NOTOALLBUTTON;
          }

          while (lpFrameCurrent->hWndEditParent)
          {
            if (lpFrameCurrent->ei.bModified == bModified)
            {
              if (DestroyFrameWindow(lpFrameCurrent) != FWDE_SUCCESS)
              {
                bResult=FALSE;
                break;
              }
            }
            else
            {
              if (!lpFrameInit) lpFrameInit=lpFrameCurrent;
              lpFrameCurrent=ActivateFrameWindow(lpFrameCurrent, FWA_NEXT);
              if (lpFrameCurrent == lpFrameInit) break;
            }
          }
          dwChangedPrompt=0;
          return bResult;
        }
      }
      case IDM_WINDOW_FRAMECLONE:
      {
        FRAMEDATA *lpFrameClone=NULL;
        AESELECTION aes;
        AECHARINDEX ciCaret;
        POINT64 ptDocumentPos;

        if (!nMDI) return (LRESULT)NULL;

        if (!moCur.bSingleOpenFile && lpFrameCurrent->ei.hWndEdit && lpFrameCurrent->wszFile[0])
        {
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETSEL, (WPARAM)&ciCaret, (LPARAM)&aes);
          SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);

          if (OpenDocument(NULL, NULL, lpFrameCurrent->wszFile, OD_ADT_DETECTBOM|OD_NOSCROLL, lpFrameCurrent->ei.nCodePage, FALSE) == EOD_SUCCESS)
          {
            lpFrameClone=lpFrameCurrent;
            aes.dwFlags|=AESELT_LOCKSCROLL|AESELT_INDEXUPDATE;
            SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETSEL, (WPARAM)&ciCaret, (LPARAM)&aes);
            SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETSCROLLPOS, 0, (LPARAM)&ptDocumentPos);
          }
        }
        return (LRESULT)lpFrameClone;
      }
      case IDM_WINDOW_TILEHORIZONTAL:
      {
        if (nMDI == WMD_MDI)
          return SendMessage(hMdiClient, WM_MDITILE, MDITILE_HORIZONTAL, 0);
        return FALSE;
      }
      case IDM_WINDOW_TILEVERTICAL:
      {
        if (nMDI == WMD_MDI)
          return SendMessage(hMdiClient, WM_MDITILE, MDITILE_VERTICAL, 0);
        return FALSE;
      }
      case IDM_WINDOW_CASCADE:
      {
        if (nMDI == WMD_MDI)
          return SendMessage(hMdiClient, WM_MDICASCADE, 0, 0);
        return FALSE;
      }
      case IDM_WINDOW_COPYPATH:
      {
        if (lParam == CPF_DIR)
        {
          return SetClipboardText(lpFrameCurrent->wszFileDir, -1);
        }
        if (lParam == CPF_FILENAME)
        {
          const wchar_t *wpFileName=GetFileName(lpFrameCurrent->wszFile, lpFrameCurrent->nFileLen);

          return SetClipboardText(wpFileName, -1);
        }
        if (lParam == CPF_FILEBASENAME)
        {
          GetBaseName(lpFrameCurrent->wszFile, lpFrameCurrent->nFileLen, wbuf, BUFFER_SIZE);
          return SetClipboardText(wbuf, -1);
        }
        if (lParam == CPF_FILEEXT)
        {
          const wchar_t *wpExt=GetFileExt(lpFrameCurrent->wszFile, lpFrameCurrent->nFileLen);

          return SetClipboardText(wpExt, -1);
        }
        //CPF_FULLPATH
        return SetClipboardText(lpFrameCurrent->wszFile, -1);
      }
      case IDM_WINDOW_FILECLOSE:
      {
        return CloseDocument(0);
      }
      case IDM_WINDOW_FILEEXIT:
      {
        if (CloseDocument(0))
        {
          if (!nMDI)
            PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
          else
            DestroyFrameWindow(lpFrameCurrent);
          return TRUE;
        }
        return FALSE;
      }
      case IDM_WINDOW_CHANGESIZE:
      {
        int nCmdShow;

        if (GetWindowLongPtrWide(hWnd, GWL_STYLE) & WS_MAXIMIZE)
          nCmdShow=SW_RESTORE;
        else
          nCmdShow=SW_MAXIMIZE;
        ShowWindow(hWnd, nCmdShow);
        return nCmdShow;
      }
      case IDM_WINDOW_DLGNEXT:
      {
        return (LRESULT)NextDialog(FALSE);
      }
      case IDM_WINDOW_DLGPREV:
      {
        return (LRESULT)NextDialog(TRUE);
      }
      case IDM_WINDOW_CLONENEXT:
      {
        return (LRESULT)NextClone(FALSE);
      }
      case IDM_WINDOW_CLONEPREV:
      {
        return (LRESULT)NextClone(TRUE);
      }

      //Help
      case IDM_MANUAL:
      {
        if (GetUserManual(wbuf, BUFFER_SIZE))
        {
          ShellExecuteWide(hMainWnd, L"open", wbuf, NULL, NULL, SW_MAXIMIZE);
          return TRUE;
        }
        return FALSE;
      }
      case IDM_UPDATE:
      {
        if (moCur.bOnTop)
          xprintfW(wbuf, L"/ONTOP %s", moCur.wszAkelUpdaterOptions);
        else
          xprintfW(wbuf, L"%s", moCur.wszAkelUpdaterOptions);
        ShellExecuteWide(hMainWnd, L"open", wszAkelUpdaterExe, wbuf, NULL, SW_SHOWDEFAULT);
        return 0;
      }
      case IDM_ABOUT:
      {
        DoHelpAbout();
        return 0;
      }

      //Internal
      case IDM_INTERNAL_CHECKWRITETIME:
      {
        if (!bCheckingWriteTime)
        {
          bCheckingWriteTime=TRUE;

          if (!nMainOnFinish && (FRAMEDATA *)lParam == lpFrameCurrent)
          {
            if (IsAllowWatchFile(lpFrameCurrent))
            {
              FILETIME ftTmp;
              BOOL bWriteTime;

              if (!FileExistsWide(lpFrameCurrent->wszFile))
              {
                xmemset(&lpFrameCurrent->ft, 0, sizeof(FILETIME));

                //Free mouse
                if (GetCapture())
                  ReleaseCapture();
                SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_DRAGDROP, AEDD_STOPDRAG, 0);

                API_LoadString(hLangModule, MSG_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
                xprintfW(wszMsg, wbuf, lpFrameCurrent->wszFile);
                API_MessageBox(hMainWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
              }
              else
              {
                if (lpFrameCurrent->nStreamOffset) lpFrameCurrent->wszFile[lpFrameCurrent->nStreamOffset]=L'\0';
                bWriteTime=GetFileWriteTimeWide(lpFrameCurrent->wszFile, &ftTmp);
                if (lpFrameCurrent->nStreamOffset) lpFrameCurrent->wszFile[lpFrameCurrent->nStreamOffset]=L':';

                if (bWriteTime)
                {
                  if (CompareFileTime(&lpFrameCurrent->ft, &ftTmp))
                  {
                    lpFrameCurrent->ft=ftTmp;

                    //Free mouse
                    if (GetCapture())
                      ReleaseCapture();
                    SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_DRAGDROP, AEDD_STOPDRAG, 0);

                    API_LoadString(hLangModule, MSG_FILE_CHANGED, wbuf, BUFFER_SIZE);
                    xprintfW(wszMsg, wbuf, lpFrameCurrent->wszFile);
                    if (API_MessageBox(hMainWnd, wszMsg, APP_MAIN_TITLEW, MB_YESNO|MB_ICONQUESTION|(lpFrameCurrent->ei.bModified?MB_DEFBUTTON2:0)) == IDYES)
                    {
                      OpenDocument(NULL, NULL, lpFrameCurrent->wszFile, OD_ADT_DETECTBOM|OD_REOPEN, lpFrameCurrent->ei.nCodePage, FALSE);
                    }
                    else
                    {
                      if (lpFrameCurrent->ei.bModified)
                      {
                        //Remove save point
                        SetModifyStatus(lpFrameCurrent, FALSE);
                        SetModifyStatus(lpFrameCurrent, TRUE);
                      }
                      else SetModifyStatus(lpFrameCurrent, TRUE);
                    }
                  }
                }
                else xmemset(&lpFrameCurrent->ft, 0, sizeof(FILETIME));
              }
            }
          }
          bCheckingWriteTime=FALSE;
        }
        return 0;
      }
      case IDM_INTERNAL_ERRORIO_MSG:
      {
        if (!nMainOnFinish)
        {
          API_LoadString(hLangModule, MSG_ERROR_IO, wszMsg, BUFFER_SIZE);
          API_MessageBox(hMainWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
        }
        return 0;
      }
      case IDM_INTERNAL_UPDATEMAINCHILDREN:
      {
        HWND hWndChild;

        //Update main window non-edit children
        hWndChild=GetWindow(hMainWnd, GW_CHILD);

        while (hWndChild)
        {
          if (((nMDI == WMD_SDI || nMDI == WMD_PMDI) && GetWindowLongPtrWide(hWndChild, GWLP_ID) != ID_EDIT) ||
              (nMDI == WMD_MDI && hWndChild != hMdiClient))
          {
            UpdateWindow(hWndChild);
          }
          hWndChild=GetNextWindow(hWndChild, GW_HWNDNEXT);
        }
        return 0;
      }
    }

    //Edit
    if (wCommand >= IDM_EDIT_FIRST && wCommand <= IDM_EDIT_LAST)
    {
      if (!lpFrameCurrent->ei.hWndEdit)
        return 0;

      switch (wCommand)
      {
        case IDM_EDIT_UNDO:
        {
          return DoEditUndo(lpFrameCurrent->ei.hWndEdit);
        }
        case IDM_EDIT_REDO:
        {
          return DoEditRedo(lpFrameCurrent->ei.hWndEdit);
        }
        case IDM_EDIT_CUT:
        {
          DWORD dwFlags=(DWORD)(lParam % 100000);
          int nNewLine=(int)(lParam / 100000);

          return DoEditCut(lpFrameCurrent->ei.hWndEdit, dwFlags, nNewLine);
        }
        case IDM_EDIT_COPY:
        {
          DWORD dwFlags=(DWORD)(lParam % 100000);
          int nNewLine=(int)(lParam / 100000);

          return DoEditCopy(lpFrameCurrent->ei.hWndEdit, dwFlags, nNewLine);
        }
        case IDM_EDIT_PASTE:
        {
          DWORD dwFlags=(DWORD)(lParam % 100000);
          int nNewLine=(int)(lParam / 100000);

          return DoEditPaste(lpFrameCurrent->ei.hWndEdit, dwFlags, nNewLine);
        }
        case IDM_EDIT_CLEAR:
        {
          DoEditClear(lpFrameCurrent->ei.hWndEdit);
          return 0;
        }
        case IDM_EDIT_SELECTALL:
        {
          DoEditSelectAll(lpFrameCurrent->ei.hWndEdit);
          return 0;
        }
        case IDM_EDIT_FIND:
        {
          DoEditFind();
          return 0;
        }
        case IDM_EDIT_FINDNEXTDOWN:
        {
          return DoEditFindNextDown(lpFrameCurrent);
        }
        case IDM_EDIT_FINDNEXTUP:
        {
          return DoEditFindNextUp(lpFrameCurrent);
        }
        case IDM_EDIT_REPLACE:
        {
          DoEditReplace();
          return 0;
        }
        case IDM_EDIT_GOTO:
        {
          DoEditGoTo();
          return 0;
        }
        case IDM_EDIT_INSERTCHAR:
        {
          DoEditInsertChar();
          return 0;
        }
        case IDM_EDIT_INSERTDATE:
        {
          DoEditInsertDate(lpFrameCurrent->ei.hWndEdit);
          return 0;
        }
        case IDM_EDIT_RECODE:
        {
          DoEditRecode();
          return 0;
        }
        case IDM_EDIT_INSERT_TAB_MENU:
        case IDM_EDIT_INSERT_TAB:
        {
          if (!DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK|STRSEL_MULTILINE, NULL))
            return InsertTabStop(lpFrameCurrent->ei.hWndEdit);
          else
            return IndentTabStop(lpFrameCurrent->ei.hWndEdit, STRSEL_INSERT|STRSEL_LEADTAB|STRSEL_FULLLINE|STRSEL_MULTILINE);
        }
        case IDM_EDIT_DELETE_TAB_MENU:
        case IDM_EDIT_DELETE_TAB:
        {
          if (!DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK|STRSEL_MULTILINE, NULL))
          {
            if (lParam)
              return IndentTabStop(lpFrameCurrent->ei.hWndEdit, STRSEL_DELETE|STRSEL_LEADTAB);
            else
              return InsertTabStop(lpFrameCurrent->ei.hWndEdit);
          }
          return IndentTabStop(lpFrameCurrent->ei.hWndEdit, STRSEL_DELETE|STRSEL_LEADTAB|STRSEL_FULLLINE|STRSEL_MULTILINE);
        }
        case IDM_EDIT_INSERT_SPACE_MENU:
        case IDM_EDIT_INSERT_SPACE:
        {
          if (!DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK|STRSEL_MULTILINE, NULL))
          {
            if (!IsReadOnly(NULL))
            {
              MSG msg;

              xmemset(&msg, 0, sizeof(MSG));
              msg.hwnd=lpFrameCurrent->ei.hWndEdit;
              msg.message=WM_KEYDOWN;
              msg.wParam=VK_SPACE;
              msg.lParam=0x00390001;
              TranslateMessage(&msg);
              DispatchMessageWide(&msg);

              msg.hwnd=lpFrameCurrent->ei.hWndEdit;
              msg.message=WM_KEYUP;
              msg.wParam=VK_SPACE;
              msg.lParam=0xC0390001;
              TranslateMessage(&msg);
              DispatchMessageWide(&msg);

              //SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_INSERTCHAR, VK_SPACE, 0);
              return TRUE;
            }
            return FALSE;
          }
          return DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_INSERT|STRSEL_FULLLINE|STRSEL_MULTILINE, L" ");
        }
        case IDM_EDIT_DELETE_SPACE_MENU:
        case IDM_EDIT_DELETE_SPACE:
        {
          if (!DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK|STRSEL_MULTILINE, NULL))
          {
            if (lParam)
              return DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_DELETE|STRSEL_LEADSPACE, L" ");
            else
            {
              if (!IsReadOnly(NULL))
              {
                SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_INSERTCHAR, VK_SPACE, 0);
                return TRUE;
              }
              return FALSE;
            }
          }
          return DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_DELETE|STRSEL_LEADSPACE|STRSEL_FULLLINE|STRSEL_MULTILINE, L" ");
        }
        case IDM_EDIT_DELETE_FIRST_CHAR_MENU:
        case IDM_EDIT_DELETE_FIRST_CHAR:
        {
          return DoEditDeleteFirstCharW(lpFrameCurrent->ei.hWndEdit);
        }
        case IDM_EDIT_DELETE_TRAILING_WHITESPACES:
        {
          return DoEditDeleteTrailingWhitespacesW(lpFrameCurrent->ei.hWndEdit);
        }
        case IDM_EDIT_DELETESELWHITESPACES:
        {
          return DoEditModifyStringInSelection(lpFrameCurrent->ei.hWndEdit, STRSEL_DELETE|STRSEL_ALLSPACES, L" ");
        }
        case IDM_EDIT_UPPERCASE:
        {
          return DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, SCT_UPPERCASE, (BOOL)lParam);
        }
        case IDM_EDIT_LOWERCASE:
        {
          return DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, SCT_LOWERCASE, (BOOL)lParam);
        }
        case IDM_EDIT_SENTENCECASE:
        {
          return DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, SCT_SENTENCECASE, (BOOL)lParam);
        }
        case IDM_EDIT_TITLECASE:
        {
          return DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, SCT_TITLECASE, (BOOL)lParam);
        }
        case IDM_EDIT_INVERTCASE:
        {
          return DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, SCT_INVERTCASE, (BOOL)lParam);
        }
        case IDM_EDIT_LOOPCASE:
        {
          int nCase=nLoopCase;
          BOOL bResult;

          if (nCase >= SCT_INVERTCASE)
            nCase=SCT_UPPERCASE;
          else
            ++nCase;
          bResult=DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, nCase, (BOOL)lParam);
          nLoopCase=nCase;
          return bResult;
        }
        case IDM_EDIT_INSERTMODE:
        {
          SetOvertypeStatus(lpFrameCurrent, !lpFrameCurrent->ei.bOvertypeMode);
          return lpFrameCurrent->ei.bOvertypeMode;
        }
        case IDM_EDIT_PASTEANSI:
        {
          return DoEditPaste(lpFrameCurrent->ei.hWndEdit, PASTE_ANSI, 0);
        }
        case IDM_EDIT_PASTECOLUMN:
        {
          return DoEditPaste(lpFrameCurrent->ei.hWndEdit, PASTE_COLUMN|PASTE_SELECT, 0);
        }
        case IDM_EDIT_PASTEAFTER:
        {
          return DoEditPaste(lpFrameCurrent->ei.hWndEdit, PASTE_AFTER, 0);
        }
        case IDM_EDIT_PASTECASE:
        {
          return PasteCase(lpFrameCurrent->ei.hWndEdit, FALSE);
        }
        case IDM_EDIT_AUTOINDENT:
        {
          BOOL bResult=FALSE;

          if (moCur.bKeepSpace || !(bResult=AutoIndent(lpFrameCurrent->ei.hWndEdit, &crCurSel)))
            ReplaceSelW(lpFrameCurrent->ei.hWndEdit, L"\n", 1, AELB_ASINPUT, 0, NULL, NULL);
          return bResult;
        }
        case IDM_EDIT_DELLINE:
        {
          DoNonMenuDelLine(lpFrameCurrent->ei.hWndEdit, (BOOL)lParam);
          return 0;
        }
        case IDM_EDIT_SELJUMPCARET:
        {
          return DoNonMenuSelJumpCaret(lpFrameCurrent->ei.hWndEdit);
        }
        case IDM_EDIT_RECENTCARETPREV:
        case IDM_EDIT_RECENTCARETNEXT:
        {
          return RecentCaretGo(wCommand == IDM_EDIT_RECENTCARETNEXT);
        }
        case IDM_EDIT_NEWLINE_WIN:
        case IDM_EDIT_NEWLINE_UNIX:
        case IDM_EDIT_NEWLINE_MAC:
        {
          int nNewLine=0;

          if (wCommand == IDM_EDIT_NEWLINE_WIN)
            nNewLine=NEWLINE_WIN;
          else if (wCommand == IDM_EDIT_NEWLINE_UNIX)
            nNewLine=NEWLINE_UNIX;
          else if (wCommand == IDM_EDIT_NEWLINE_MAC)
            nNewLine=NEWLINE_MAC;

          if (!IsReadOnly(NULL))
          {
            if (lpFrameCurrent->ei.nNewLine != nNewLine)
            {
              SetNewLineStatus(lpFrameCurrent, nNewLine, AENL_INPUT|AENL_OUTPUT);
              SetModifyStatus(lpFrameCurrent, TRUE);
            }
          }
          return 0;
        }
      }
      return 0;
    }

    if (wCommand >= IDM_RECENT_FILES && wCommand <= (IDM_RECENT_FILES + moCur.nRecentFiles))
    {
      if (wCommand == IDM_RECENT_FILES)
      {
        int nDead=RecentFilesDeleteOld(&hRecentFilesStack);

        API_LoadString(hLangModule, MSG_RECENTFILES_DELETED, wbuf, BUFFER_SIZE);
        xprintfW(wszMsg, wbuf, nDead);
        API_MessageBox(hWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
        return nDead;
      }
      else
      {
        int nOpen=EOD_SUCCESS;

        if (nMDI || SaveChanged(0))
        {
          RECENTFILE *lpRecentFile;
          wchar_t *wpFile=API_AllocWide(MAX_PATH);

          if (lpRecentFile=RecentFilesFindByIndex(wCommand - IDM_RECENT_FILES - 1))
          {
            xstrcpynW(wpFile, lpRecentFile->wszFile, MAX_PATH);
            nOpen=OpenDocument(NULL, NULL, wpFile, OD_ADT_BINARYERROR|OD_ADT_REGCODEPAGE, 0, FALSE);
          }
          API_FreeWide(wpFile);
        }
        return nOpen;
      }
    }
    if (wCommand >= IDM_LANGUAGE && wCommand <= (IDM_LANGUAGE + nLangModuleCount + 1))
    {
      if (wCommand == IDM_LANGUAGE)
      {
        if (!*moCur.wszLangModule) return TRUE;
        moCur.wszLangModule[0]=L'\0';
      }
      else
      {
        GetMenuStringWide(hMainMenu, wCommand, wbuf, BUFFER_SIZE, MF_BYCOMMAND);
        xprintfW(wbuf2, L"%s.dll", wbuf);
        if (!xstrcmpiW(moCur.wszLangModule, wbuf2)) return TRUE;
        xstrcpynW(moCur.wszLangModule, wbuf2, MAX_PATH);
      }
      WideCharToMultiByte(CP_ACP, 0, moCur.wszLangModule, -1, moCur.szLangModule, MAX_PATH, NULL, NULL);
      API_LoadString(hLangModule, MSG_RESTART_PROGRAM, wszMsg, BUFFER_SIZE);
      API_MessageBox(hWnd, wszMsg, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
      return 0;
    }
    if (wCommand >= IDM_POPUP_OPENAS && wCommand < IDM_POPUP_OPENAS + nCodepageListLen)
    {
      return DoFileReopenAs(OD_ADT_DETECTBOM, lpCodepageList[wCommand - IDM_POPUP_OPENAS], TRUE);
    }
    if (wCommand >= IDM_POPUP_SAVEAS && wCommand < IDM_POPUP_SAVEAS + nCodepageListLen)
    {
      return SaveDocument(NULL, NULL, lpFrameCurrent->wszFile, lpCodepageList[wCommand - IDM_POPUP_SAVEAS], TRUE, SD_UPDATE);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    //WM_NOTIFY (SDI & MDI)
    if (wParam == ID_STATUS && (nMDI == WMD_SDI || nMDI == WMD_PMDI || lpFrameCurrent->hWndEditParent))
    {
      if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
      {
        if (((NMMOUSE *)lParam)->dwItemSpec == 0)
        {
          DoEditGoTo();
        }
        else if (((NMMOUSE *)lParam)->dwItemSpec == 2)
        {
          SetOvertypeStatus(lpFrameCurrent, !lpFrameCurrent->ei.bOvertypeMode);
        }
        else if (((NMMOUSE *)lParam)->dwItemSpec == 3)
        {
          if (!IsReadOnly(NULL))
          {
            if (lpFrameCurrent->ei.nNewLine == NEWLINE_WIN)
              SetNewLineStatus(lpFrameCurrent, NEWLINE_UNIX, AENL_INPUT|AENL_OUTPUT);
            else if (lpFrameCurrent->ei.nNewLine == NEWLINE_UNIX)
              SetNewLineStatus(lpFrameCurrent, NEWLINE_MAC, AENL_INPUT|AENL_OUTPUT);
            else if (lpFrameCurrent->ei.nNewLine == NEWLINE_MAC)
              SetNewLineStatus(lpFrameCurrent, NEWLINE_WIN, AENL_INPUT|AENL_OUTPUT);
            SetModifyStatus(lpFrameCurrent, TRUE);
          }
        }
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_RCLICK)
      {
        if (((NMMOUSE *)lParam)->dwItemSpec == 4)
        {
          POINT pt;

          GetCursorPos(&pt);
          ShowMenuPopupCodepage(&pt, TRUE);
        }
      }
    }

    //WM_NOTIFY (MDI)
    if (nMDI)
    {
      if (wParam == ID_TAB)
      {
        if (((NMHDR *)lParam)->code == TCN_GETOBJECT)
        {
          if (((LPNMOBJECTNOTIFY)lParam)->iItem != -1)
          {
            if (!dwTabOpenTimer)
            {
              dwTabOpenTimer=SetTimer(hTab, TIMERID_TABOPEN, TIME_TABOPEN, NULL);
              nTabOpenItem=((LPNMOBJECTNOTIFY)lParam)->iItem;
            }
            else if (nTabOpenItem != ((LPNMOBJECTNOTIFY)lParam)->iItem)
            {
              KillTimer(hTab, dwTabOpenTimer);
              dwTabOpenTimer=0;
              nTabOpenItem=-1;
            }
          }
          return 0;
        }
        else if (((NMHDR *)lParam)->code == TCN_SELCHANGING)
        {
          if (dwTabOpenTimer)
            return TRUE;
        }
        else if (((NMHDR *)lParam)->code == TCN_SELCHANGE)
        {
          FRAMEDATA *lpFrame;

          bTabPressing=TRUE;
          nDocumentIndex=(int)SendMessage(hTab, TCM_GETCURSEL, 0, 0);
          lpFrame=(FRAMEDATA *)GetTabParamFromItem(hTab, nDocumentIndex);
          ActivateFrameWindow(lpFrame, 0);
          bTabPressing=FALSE;
        }
      }
    }
  }
  else if (uMsg == WM_SYSCOMMAND)
  {
    if (wParam == SC_CLOSE)
    {
      //Alt+F4 in dockable window cause double sending of WM_SYSCOMMAND with SC_CLOSE
      if (nMainOnFinish)
        return 0;
    }
  }
  else if (uMsg == WM_CLOSE ||
           uMsg == WM_QUERYENDSESSION)
  {
    BOOL bEndSession=FALSE;

    if (uMsg == WM_QUERYENDSESSION)
      bEndSession=TRUE;
    nMainOnFinish=MOF_QUERYEND;

    if (!nMDI)
    {
      DWORD dwPrompt=0;

      if (!lpFrameCurrent->ei.bModified || (moCur.bSilentCloseEmptyMDI && !lpFrameCurrent->ei.wszFile[0] && !GetTextLength(lpFrameCurrent->ei.hWndEdit)))
        dwPrompt|=PROMPT_NONE;

      if (!SaveChanged(dwPrompt))
      {
        nMainOnFinish=MOF_NONE;
        return bEndSession?0:1;
      }
      RecentFilesFrameUpdate(lpFrameCurrent);
    }
    else
    {
      int nDestroyResult=FWDE_SUCCESS;

      if (lpFrameCurrent->hWndEditParent)
      {
        RecentFilesFrameUpdate(lpFrameCurrent);
        bFirstTabOnFinish=TRUE;
      }

      //Show "No to all" button if necessary
      if (nDocumentsModified > 1)
        dwChangedPrompt|=PROMPT_NOTOALLBUTTON;

      //Close modified first
      if ((moCur.dwTabOptionsMDI & TAB_ONEXITMODIFIEDFIRST) && nDocumentsModified > 0)
      {
        FRAMEDATA *lpFrameInit=NULL;

        while (lpFrameCurrent->hWndEditParent)
        {
          if (lpFrameCurrent->ei.bModified)
          {
            nDestroyResult=DestroyFrameWindow(lpFrameCurrent);

            if (nDestroyResult == FWDE_ABORT)
            {
              nMainOnFinish=MOF_NONE;
              break;
            }
            else if (nDestroyResult != FWDE_SUCCESS)
              break;
          }
          else
          {
            if (!lpFrameInit) lpFrameInit=lpFrameCurrent;
            lpFrameCurrent=ActivateFrameWindow(lpFrameCurrent, FWA_NEXT);
            if (lpFrameCurrent == lpFrameInit) break;
          }
        }
      }

      if (nDestroyResult == FWDE_SUCCESS)
      {
        while (lpFrameCurrent->hWndEditParent)
        {
          nDestroyResult=DestroyFrameWindow(lpFrameCurrent);

          if (nDestroyResult == FWDE_ABORT)
          {
            nMainOnFinish=MOF_NONE;
            break;
          }
          else if (nDestroyResult != FWDE_SUCCESS)
            break;
        }
      }
      dwChangedPrompt=0;

      if (!nMainOnFinish)
        return bEndSession?0:1;
    }

    //Close modeless dialog
    if (hDlgModeless)
      SendMessage(hDlgModeless, WM_CLOSE, 0, 0);

    #ifndef AKELPAD_DLLBUILD
      if (!SendMessage(hMainWnd, AKDN_MAIN_ONCLOSE_PREFINISH, 0, 0))
      {
        //Main window will be destroyed
        PostMessage(hWnd, AKDN_MAIN_ONFINISH, 0, 0);
      }
    #endif

    return bEndSession?1:0;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Save options
    SaveOptions(&moCur, &fdDefault, moCur.nSaveSettings, FALSE);

    //Save plugin stack
    if (bSavePluginsStackOnExit)
      StackPluginSave(&hPluginsStack, moCur.nSaveSettings);

    //Save search history
    SearchSave(moCur.nSaveHistory);

    //Save recent files
    RecentFilesSave(&hRecentFilesStack, moCur.nSaveHistory);

    //Clean up
    if (nMDI)
    {
      ImageList_Destroy(hImageList);
      DestroyCursor(hCursorDragMove);
      DestroyIcon(hIconEmpty);
      StackIconsFree(&hIconsStack);
    }
    DestroyAcceleratorTable(hMainAccel);
    DestroyAcceleratorTable(hGlobalAccel);
    DestroyCursor(hCursorHandOpen);
    DestroyCursor(hCursorHandClose);
    DestroyIcon(hMainIcon);
    DestroyMenu(hMainMenu);
    DestroyMenu(hPopupMenu);
    StackButtonDrawFree(&hButtonDrawStack);

    #ifndef AKELPAD_DLLBUILD
      PostQuitMessage(0);
    #endif
    return 0;
  }

  //Special messages
  if (nMDI)
  {
    if (uMsg == WM_LBUTTONDBLCLK)
    {
      if (!(moCur.dwTabOptionsMDI & TAB_NOADD_LBUTTONDBLCLK))
      {
        CreateFrameWindow(NULL);
      }
    }
    else if (uMsg == WM_MBUTTONDOWN)
    {
      if (!bEnterMenuLoop && !(moCur.dwTabOptionsMDI & TAB_NOADD_MBUTTONDOWN))
      {
        CreateFrameWindow(NULL);
      }
    }
  }
  if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
  {
    LRESULT lResult;

    if (EditParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
      return lResult;
  }

  if (bOldWindows)
  {
    if (nMDI == WMD_MDI)
      return DefFrameProcA(hWnd, hMdiClient, uMsg, wParam, lParam);
    else
      return DefWindowProcA(hWnd, uMsg, wParam, lParam);
  }
  else
  {
    if (nMDI == WMD_MDI)
      return DefFrameProcW(hWnd, hMdiClient, uMsg, wParam, lParam);
    else
      return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }
}

BOOL CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (uMsg == WM_SETFOCUS)
  {
    if (!bEditOnFinish)
    {
      if (lpFrameCurrent->ei.hWndEdit)
        SetFocus(lpFrameCurrent->ei.hWndEdit);

      if (IsAllowWatchFile(lpFrameCurrent) && !bCheckingWriteTime && !bFrameActivating)
        PostMessage(hMainWnd, WM_COMMAND, IDM_INTERNAL_CHECKWRITETIME, (LPARAM)lpFrameCurrent);
    }
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam != lpFrameCurrent->ei.hWndEdit)
    {
      if (GetWindowLongPtrWide((HWND)wParam, GWLP_ID) == ID_EDIT)
      {
        SetFocus((HWND)wParam);
      }
    }

    if ((HWND)wParam == lpFrameCurrent->ei.hWndEdit)
    {
      NCONTEXTMENU ncm;
      POINT pt;
      HMENU hTrackMenu;

      if (lParam == -1 && SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETCARETPOS, (WPARAM)&pt, 0))
      {
        pt.y+=(int)SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
        ClientToScreen(lpFrameCurrent->ei.hWndEdit, &pt);
      }
      else GetCursorPos(&pt);

      ncm.hWnd=lpFrameCurrent->ei.hWndEdit;
      ncm.uType=NCM_EDIT;
      ncm.pt=pt;
      ncm.bMouse=(lParam != -1);
      ncm.bProcess=TRUE;
      SendMessage(hMainWnd, AKDN_CONTEXTMENU, 0, (LPARAM)&ncm);
      if (ncm.bProcess)
      {
        if (lpFrameCurrent->ei.bReadOnly)
        {
          hTrackMenu=hPopupView;
          EnableMenuItem(hTrackMenu, IDM_EDIT_COPY, AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax)?MF_ENABLED:MF_GRAYED);
        }
        else
        {
          hTrackMenu=hPopupEdit;
          EnableMenuItem(hTrackMenu, IDM_EDIT_UNDO, SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANUNDO, 0, 0)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_REDO, SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANREDO, 0, 0)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_PASTE, SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANPASTE, 0, 0)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_CUT, AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_CLEAR, AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_COPY, AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax)?MF_ENABLED:MF_GRAYED);
        }
        TrackPopupMenu(hTrackMenu, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hMainWnd, NULL);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT)
    {
      if (((NMHDR *)lParam)->code == AEN_ERRSPACE)
      {
        BUTTONMESSAGEBOX bmb[]={{IDYES, MAKEINTRESOURCEW(STR_MESSAGEBOX_TERMINATE), BMB_DEFAULT},
                                {IDNO,  MAKEINTRESOURCEW(STR_MESSAGEBOX_CONTINUE),  0},
                                {0, 0, 0}};

        API_LoadString(hLangModule, MSG_ERROR_NOT_ENOUGH_MEMORY, wbuf, BUFFER_SIZE);
        if (MessageBoxCustom(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_ICONERROR, NULL, &bmb[0]) == IDYES)
          ExitProcess(0);
      }
      else if (((NMHDR *)lParam)->code == AEN_DROPSOURCE)
      {
        AENDROPSOURCE *aeds=(AENDROPSOURCE *)lParam;

        if (aeds->nAction == AEDS_SOURCEDONE)
        {
          if (nMDI == WMD_PMDI)
          {
            //Drag'n'Drop was from one tab to another, update caret position cause it has source window coordinates.
            if (lpFrameCurrent->ei.hDocEdit != aeds->hdr.docFrom)
              SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_UPDATECARET, 0, 0);
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGING)
      {
        AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;

        if (lpFrameCurrent == GetFrameDataFromEditWindow(aentc->hdr.hwndFrom))
          lpFrameCurrent->nSelSubtract=0;
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGED)
      {
        AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;

        if (!(aentc->dwType & AETCT_WRAP))
        {
          StackRecentCaretFree(&lpFrameCurrent->hRecentCaretStack);
          lpFrameCurrent->lpCurRecentCaret=NULL;
          lpFrameCurrent->hCurUndoItem=NULL;
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_SELCHANGING)
      {
        AENSELCHANGE *aensc=(AENSELCHANGE *)lParam;

        if (!nMainOnFinish)
        {
          if (lpFrameCurrent->ei.hDocEdit == aensc->hdr.docFrom)
          {
            if (lpFrameCurrent->nSelSubtract)
            {
              if (xmemcmp(&aensc->crSel, &lpFrameCurrent->crPrevSel, sizeof(AECHARRANGE)))
                lpFrameCurrent->nSelSubtract=0;
            }
          }
          RecentCaretSet(aensc);
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_SELCHANGED)
      {
        AENSELCHANGE *aensc=(AENSELCHANGE *)lParam;

        if (!nMainOnFinish)
        {
          SetSelectionStatus(aensc->hdr.docFrom, aensc->hdr.hwndFrom, &aensc->crSel, &aensc->ciCaret);

          if (!(aensc->dwType & AESCT_WRAP) && !(aensc->dwType & AESCT_UPDATESELECTION))
          {
            if (moCur.dwKeybLayoutOptions & KLO_SWITCHLAYOUT)
              SwitchLayout(aensc->hdr.hwndFrom, &aensc->ciCaret);
          }
          RecentCaretSet(aensc);
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_MODIFY)
      {
        AENMODIFY *aenm=(AENMODIFY *)lParam;
        FRAMEDATA *lpFrame;

        if (nMDI == WMD_PMDI)
          lpFrame=GetFrameDataFromEditDocument(aenm->hdr.docFrom);
        else
          lpFrame=GetFrameDataFromEditWindow(aenm->hdr.hwndFrom);

        if (nMDI)
        {
          if (aenm->bModified)
            ++nDocumentsModified;
          else
            --nDocumentsModified;
          UpdateStatusUser(lpFrame, CSB_DOCUMENTSMODIFIED|CSB_DOCUMENTSSAVED);
        }
        UpdateAsterisk(lpFrame, aenm->bModified);
        SetModifyStatus(lpFrame, aenm->bModified);
      }
      else if (((NMHDR *)lParam)->code == AEN_LINK)
      {
        AENLINK *aenl=(AENLINK *)lParam;
        static BOOL bDownURL=FALSE;

        if (aenl->hdr.docFrom == lpFrameCurrent->ei.hDocEdit)
        {
          if (lpFrameCurrent->nClickURL == 1 && aenl->uMsg == WM_LBUTTONDOWN)
          {
            bDownURL=TRUE;
          }
          else
          {
            if ((lpFrameCurrent->nClickURL == 1 && aenl->uMsg == WM_LBUTTONUP && bDownURL) ||
                (lpFrameCurrent->nClickURL == 2 && aenl->uMsg == WM_LBUTTONDBLCLK))
            {
              if (!AEC_IndexCompare(&crCurSel.ciMin, &crCurSel.ciMax))
              {
                wchar_t *wszURL;

                if (ExGetRangeTextW(lpFrameCurrent->ei.hWndEdit, &aenl->crLink.ciMin, &aenl->crLink.ciMax, FALSE, &wszURL, AELB_ASIS, FALSE))
                {
                  if (moCur.wszUrlCommand[0])
                    CallMethod(moCur.wszUrlCommand, wszURL);
                  else
                    ShellExecuteWide(lpFrameCurrent->ei.hWndEdit, L"open", wszURL, NULL, NULL, SW_SHOWNORMAL);
                  FreeText(wszURL);
                }
                ++aenl->nVisitCount;
                *lResult=1;
                return TRUE;
              }
            }
            bDownURL=FALSE;
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_MARKER)
      {
        AENMARKER *aenm=(AENMARKER *)lParam;
        DWORD dwNewPos;

        if (aenm->bMouse)
        {
          if (aenm->dwType == AEMT_PIXEL)
            dwNewPos=aenm->dwPos / (DWORD)SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_GETCHARSIZE, AECS_AVEWIDTH, 0);
          else
            dwNewPos=aenm->dwPos;

          if (lpFrameCurrent->dwMarker == lpFrameCurrent->dwWrapLimit)
          {
            SetFrameInfo(lpFrameCurrent, FIS_MARKER, dwNewPos);
            SetFrameInfo(lpFrameCurrent, FIS_WRAP, MAKELONG(lpFrameCurrent->dwWrapType, dwNewPos));
          }
          else SetFrameInfo(lpFrameCurrent, FIS_MARKER, dwNewPos);
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_PROGRESS)
      {
        AENPROGRESS *aenp=(AENPROGRESS *)lParam;
        MSG msg;
        BOOL bStop=FALSE;
        static DWORD dwProgressType=0;
        static INT_PTR nIncrement;
        static INT_PTR nBarrier;
        static DWORD dwSeconds;
        INT_PTR nCurrent;

        if (!dwProgressType || dwProgressType == aenp->dwType)
        {
          if (dwProgressType == AEPGS_STREAMIN)
            nCurrent=lpStreamInData->dwBytesCurrent;
          else
            nCurrent=aenp->nCurrent;

          //Start indication if elapsed time more than 0,5 second
          if (aenp->dwTimeElapsed > 500)
          {
            //Start progress
            if (!dwProgressType)
            {
              dwProgressType=aenp->dwType;

              SendMessage(hProgress, PBM_SETRANGE32, 0, aenp->nMaximum);
              SendMessage(hProgress, PBM_SETPOS, 0, 0);
              ShowWindow(hProgress, SW_SHOW);
              nIncrement=aenp->nMaximum / nProgressWidth;
              if (!nIncrement)
                nBarrier=0;
              else
                nBarrier=(nCurrent / nIncrement) * nIncrement;
              dwSeconds=1;
            }

            //Change position
            if (nCurrent >= nBarrier)
            {
              SendMessage(hProgress, PBM_SETPOS, nCurrent, 0);
              nBarrier+=nIncrement;
            }

            if (aenp->dwTimeElapsed / 500 > dwSeconds)
            {
              dwSeconds=aenp->dwTimeElapsed / 500;

              //Check for progress update
              while (PeekMessageWide(&msg, hProgress, 0, 0, PM_REMOVE))
              {
                TranslateMessage(&msg);
                DispatchMessageWide(&msg);
              }

              //Check for stop processing
              //if (dwProgressType == AEPGS_SETTEXT || dwProgressType == AEPGS_STREAMIN)
              {
                while (PeekMessageWide(&msg, hMainWnd, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
                {
                  if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
                  {
                    bStop=TRUE;
                  }
                }
              }

              //Win7: prevent system from mark program as hanged
              PeekMessageWide(&msg, hMainWnd, 0, 0, PM_NOREMOVE);
            }

            //End progress
            if (nCurrent >= aenp->nMaximum || bStop)
            {
              ShowWindow(hProgress, SW_HIDE);
              UpdateWindow(hStatus);
              dwProgressType=0;

              if (bStop)
              {
                CloseDocument(0);
                //PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
                *lResult=1;
                return TRUE;
              }
            }
          }
        }
      }
    }
  }
  return FALSE;
}

LRESULT CALLBACK CommonFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  lResult=lpfnFrameProc(hWnd, uMsg, wParam, lParam);

  if (lpfnFrameProcRet)
  {
    CWPRETSTRUCT cwprs;

    cwprs.lResult=lResult;
    cwprs.lParam=lParam;
    cwprs.wParam=wParam;
    cwprs.message=uMsg;
    cwprs.hwnd=hWnd;
    lpfnFrameProcRet(&cwprs);
  }
  return lResult;
}

LRESULT CALLBACK FrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  FRAMEDATA *lpFrame;

  if (uMsg == WM_CREATE)
  {
    //CREATESTRUCT *cs=(CREATESTRUCT *)lParam;
    //MDICREATESTRUCT *mcs=(MDICREATESTRUCT *)cs->lpCreateParams;
    //lpFrame=(FRAMEDATA *)mcs->lParam

    lpFrame=lpFrameCurrent;

    if (lpFrame=CreateFrameData(hWnd, lpFrame))
    {
      SetWindowLongPtrWide(hWnd, GWLP_USERDATA, (UINT_PTR)lpFrame);

      lpFrame->ei.hWndEdit=(HWND)CreateEditWindow(hWnd, NULL);
      lpFrame->lpEditProc=(AEEditProc)SendMessage(lpFrame->ei.hWndEdit, AEM_GETDOCUMENTPROC, (WPARAM)NULL, 0);
      lpFrame->ei.hDocEdit=(AEHDOC)SendMessage(lpFrame->ei.hWndEdit, AEM_GETDOCUMENT, 0, 0);

      InsertTabItem(hTab, (moCur.dwTabOptionsMDI & TAB_ADD_AFTERCURRENT)?nDocumentIndex + 1:-1, (LPARAM)lpFrame);

      SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)lpFrame->hIcon);
      SetEditWindowSettings(lpFrame);
      SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrame->ei.hWndEdit, (LPARAM)lpFrame->ei.hDocEdit);

      //Update status
      ++nDocumentsCount;
      UpdateStatusUser(lpFrame, CSB_DOCUMENTSCOUNT|CSB_DOCUMENTSMODIFIED|CSB_DOCUMENTSSAVED);
    }
  }
  else if (uMsg == WM_WINDOWPOSCHANGING)
  {
    WINDOWPOS *lpWindowPos=(WINDOWPOS *)lParam;
    RECT rcFrame;

    if (!(lpWindowPos->flags & SWP_NOCOPYBITS))
    {
      GetWindowSize(hWnd, hMdiClient, &rcFrame);

      if ((!(lpWindowPos->flags & SWP_NOMOVE) && (lpWindowPos->x != rcFrame.left || lpWindowPos->y != rcFrame.top)) &&
          (!(lpWindowPos->flags & SWP_NOSIZE) && (lpWindowPos->cx != rcFrame.right || lpWindowPos->cy != rcFrame.bottom)))
      {
        //Size and position changed don't copy bits to avoid blinking.
        lpWindowPos->flags|=SWP_NOCOPYBITS;
      }
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      FRAMEDATA *lpFrame;

      if (lpFrame=(FRAMEDATA *)GetWindowLongPtrWide(hWnd, GWLP_USERDATA))
      {
        lpFrame->rcEditWindow.left=0;
        lpFrame->rcEditWindow.top=0;
        lpFrame->rcEditWindow.right=LOWORD(lParam);
        lpFrame->rcEditWindow.bottom=HIWORD(lParam);
        ResizeEditWindow(lpFrame, 0);
      }
    }
  }
  else if (uMsg == WM_MDIACTIVATE)
  {
    int nMessages[]={IDM_FILE_REOPEN,
                     IDM_FILE_SAVE,
                     IDM_FILE_SAVEAS,
                     IDM_FILE_SAVEALL,
                     IDM_FILE_SAVEALLAS,
                     IDM_FILE_PAGESETUP,
                     IDM_FILE_PRINT,
                     IDM_FILE_PRINTPREVIEW,
                     IDM_EDIT_UNDO,
                     IDM_EDIT_REDO,
                     IDM_EDIT_CUT,
                     IDM_EDIT_COPY,
                     IDM_EDIT_PASTE,
                     IDM_EDIT_CLEAR,
                     IDM_EDIT_SELECTALL,
                     IDM_EDIT_FIND,
                     IDM_EDIT_FINDNEXTDOWN,
                     IDM_EDIT_FINDNEXTUP,
                     IDM_EDIT_REPLACE,
                     IDM_EDIT_GOTO,
                     IDM_EDIT_INSERTCHAR,
                     IDM_EDIT_INSERTDATE,
                     IDM_EDIT_PASTECOLUMN,
                     IDM_EDIT_INSERT_TAB_MENU,
                     IDM_EDIT_DELETE_TAB_MENU,
                     IDM_EDIT_INSERT_SPACE_MENU,
                     IDM_EDIT_DELETE_SPACE_MENU,
                     IDM_EDIT_DELETE_FIRST_CHAR_MENU,
                     IDM_EDIT_DELETE_TRAILING_WHITESPACES,
                     IDM_EDIT_DELETESELWHITESPACES,
                     IDM_EDIT_UPPERCASE,
                     IDM_EDIT_LOWERCASE,
                     IDM_EDIT_SENTENCECASE,
                     IDM_EDIT_TITLECASE,
                     IDM_EDIT_INVERTCASE,
                     IDM_EDIT_RECODE,
                     IDM_EDIT_NEWLINE_WIN,
                     IDM_EDIT_NEWLINE_UNIX,
                     IDM_EDIT_NEWLINE_MAC,
                     IDM_VIEW_FONT,
                     IDM_VIEW_COLORS,
                     IDM_VIEW_READONLY,
                     IDM_VIEW_WORDWRAP,
                     IDM_VIEW_SPLIT_WINDOW_ONOFF,
                     IDM_WINDOW_TILEHORIZONTAL,
                     IDM_WINDOW_TILEVERTICAL,
                     IDM_WINDOW_CASCADE,
                     IDM_WINDOW_FRAMECLOSE,
                     IDM_WINDOW_FRAMECLOSEALL,
                     IDM_WINDOW_FRAMECLOSEALL_BUTACTIVE,
                     IDM_WINDOW_FRAMECLONE,
                     IDM_WINDOW_COPYPATH,
                     0};
    int i;

    if (!lParam)
    {
      for (i=0; nMessages[i]; ++i)
        EnableMenuItem(hMainMenu, nMessages[i], MF_GRAYED);
      bMdiNoWindows=TRUE;

      //Clear status
      {
        int nStatusParts=SBP_USER + hStatusStack.nElements;

        ssStatus.bModified=-1;
        ssStatus.bOvertypeMode=-1;
        ssStatus.nNewLine=-1;
        ssStatus.nCodePage=-1;
        ssStatus.bBOM=-1;
        for (i=0; i < nStatusParts; ++i)
          StatusBar_SetTextWide(hStatus, i, L"");
      }
      SendMessage(hMainWnd, AKDN_FRAME_NOWINDOWS, 0, 0);
    }
    else
    {
      if (bMdiNoWindows)
      {
        for (i=0; nMessages[i]; ++i)
          EnableMenuItem(hMainMenu, nMessages[i], MF_ENABLED);
        bMdiNoWindows=FALSE;

        SetNewLineStatus(NULL, moCur.nNewFileNewLine, 0);
        SetOvertypeStatus(NULL, FALSE);
        SetModifyStatus(NULL, FALSE);
        SetCodePageStatus(NULL, moCur.nNewFileCodePage, moCur.bNewFileBOM);
      }

      //Variants:
      //A. MDI window just created - MDI client has now one window. WM_MDIACTIVATE sended 1 time:
      //  1. On enter lpFrameCurrent == &fdDefault, wParam == NULL, lParam != NULL
      //B. Another MDI window created or switching between MDI windows. WM_MDIACTIVATE sended 2 times:
      //  1. On enter lpFrameCurrent == wParam, wParam != NULL, lParam != NULL
      //  2. On leave lpFrameCurrent == lParam, wParam != NULL, lParam != NULL
      //C. Active MDI window just received WM_MDIDESTROY - MDI client has still one or more windows. WM_MDIACTIVATE sended 2 times:
      //  1. On enter lpFrameCurrent == &fdDefault, wParam != NULL, lParam != NULL
      //  2. On leave lpFrameCurrent == lParam, wParam != NULL, lParam != NULL
      //D. Active MDI window just received WM_MDIDESTROY - MDI client has no more windows. WM_MDIACTIVATE sended 1 time:
      //  1. On leave lpFrameCurrent == &fdDefault, wParam != NULL, lParam == NULL

      if (hWnd == (HWND)wParam)
      {
        if (lpFrame=(FRAMEDATA *)GetWindowLongPtrWide((HWND)wParam, GWLP_USERDATA))
          SendMessage(hMainWnd, AKDN_FRAME_DEACTIVATE, dwMdiFrameActivating, (LPARAM)lpFrame);

        //Switching between
        if (lpFrameCurrent->hWndEditParent)
        {
          //Remove blinking frame windows effect (begin)
          SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);
          if (bMdiMaximize) SendMessage(hMdiClient, WM_SETREDRAW, FALSE, 0);

          if (!(dwMdiFrameActivating & FWA_NOUPDATEORDER))
          {
            //Move item to the end of stack, to use access order later.
            if (lpFrame=(FRAMEDATA *)GetWindowLongPtrWide((HWND)lParam, GWLP_USERDATA))
              StackFrameMove(&hFramesStack, lpFrame, -1);
          }
        }
        //Destroying
        else if (lParam)
          dwMdiFrameActivating=FWA_NOTIFY_AFTERDESTROY;

        //Save deactivated MDI window settings
        if (lpFrame=(FRAMEDATA *)GetWindowLongPtrWide((HWND)wParam, GWLP_USERDATA))
          SaveFrameData(lpFrame);
      }
      else if (hWnd == (HWND)lParam)
      {
        //Change current frame handle in WM_MDIACTIVATE because WM_SETFOCUS can be uncalled if WM_MDINEXT used.
        if (lpFrameCurrent->hWndEditParent != (HWND)lParam)
        {
          if (lpFrame=(FRAMEDATA *)GetWindowLongPtrWide((HWND)lParam, GWLP_USERDATA))
            lpFrameCurrent=lpFrame;
        }

        //Restore activated MDI window settings
        RestoreFrameData(lpFrameCurrent, 0);

        //Remove blinking frame windows effect (end)
        if (bMdiMaximize)
        {
          SendMessage(hMdiClient, WM_SETREDRAW, TRUE, 0);
          InvalidateRect(lpFrameCurrent->hWndEditParent, NULL, FALSE);
        }
        else
        {
          if (mc.dwStyle & WS_CHILD)
          {
            SetFocus((HWND)lParam);
            RedrawWindow((HWND)lParam, NULL, NULL, RDW_FRAME|RDW_INVALIDATE|RDW_NOCHILDREN);
          }
        }
        SendMessage(hMainWnd, AKDN_FRAME_ACTIVATE, dwMdiFrameActivating, (LPARAM)lpFrameCurrent);
        dwMdiFrameActivating=0;
      }
    }
  }
  else if (uMsg == WM_SETFOCUS)
  {
    //Change current frame handle in WM_SETFOCUS because WM_MDIACTIVATE sended later.
    if (lpFrame=(FRAMEDATA *)GetWindowLongPtrWide(hWnd, GWLP_USERDATA))
      lpFrameCurrent=lpFrame;
  }

  //Special messages
  {
    LRESULT lResult;

    if (EditParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
      return lResult;
  }

  if (bOldWindows)
    return DefMDIChildProcA(hWnd, uMsg, wParam, lParam);
  else
    return DefMDIChildProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CommonEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  lResult=lpfnEditProc(hWnd, uMsg, wParam, lParam);

  if (lpfnEditProcRet)
  {
    CWPRETSTRUCT cwprs;

    cwprs.lResult=lResult;
    cwprs.lParam=lParam;
    cwprs.wParam=wParam;
    cwprs.message=uMsg;
    cwprs.hwnd=hWnd;
    lpfnEditProcRet(&cwprs);
  }
  return lResult;
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_SETFOCUS)
  {
    if (nMDI == WMD_MDI)
    {
      if (!IsEditActive(hWnd))
      {
        FRAMEDATA *lpFrame;

        if (lpFrame=GetFrameDataFromEditWindow(hWnd))
          ActivateFrameWindow(lpFrame, 0);
      }

      //Assign current window. Need for split windows.
      lpFrameCurrent->ei.hWndEdit=hWnd;
      lpFrameCurrent->ei.hDocEdit=(AEHDOC)SendMessage(hWnd, AEM_GETDOCUMENT, 0, 0);
    }
    else
    {
      //Assign current window. Need for split windows.
      lpFrameCurrent->ei.hWndEdit=hWnd;
      lpFrameCurrent->ei.hDocEdit=(AEHDOC)SendMessage(hWnd, AEM_GETDOCUMENT, 0, 0);
    }
    SetSelectionStatus(lpFrameCurrent->ei.hDocEdit, lpFrameCurrent->ei.hWndEdit, NULL, NULL);
  }
  else if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_CAPITAL)
    {
      UpdateStatusUser(lpFrameCurrent, CSB_CAPSLOCK);
    }
    else if (wParam == VK_NUMLOCK)
    {
      UpdateStatusUser(lpFrameCurrent, CSB_NUMLOCK);
    }
  }
  else if (uMsg == WM_CHAR)
  {
    if (wParam == VK_RETURN)
    {
      if (moCur.bKeepSpace)
      {
        if (GetKeyState(VK_MENU) >= 0 &&
            GetKeyState(VK_SHIFT) >= 0 &&
            GetKeyState(VK_CONTROL) >= 0)
        {
          if (AutoIndent(hWnd, &crCurSel))
            return TRUE;
        }
      }
    }
  }
  else if (uMsg == WM_MOUSEWHEEL)
  {
    if (!(lpFrameCurrent->dwMouseOptions & MO_NOWHEELFONTCHANGE))
    {
      if (LOWORD(wParam) == MK_CONTROL)
      {
        if ((short)HIWORD(wParam) < 0)
          DoViewFontSize(lpFrameCurrent, FONTSIZE_DECREASE);
        else
          DoViewFontSize(lpFrameCurrent, FONTSIZE_INCREASE);
        return TRUE;
      }
    }
  }

  //Clone messages
  {
    LRESULT lResult;

    if (CloneDragAndDropMessages(hWnd, uMsg, wParam, lParam, &lResult))
      return lResult;
  }

  return CallWindowProcWide(lpOldEditProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK CloneDragAndDropMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  static RECT rcInitMasterWindow;
  static POINT ptMouseDown;
  static int nMouseMove;

  if (uMsg == WM_SETCURSOR)
  {
    if (lpFrameCurrent->ei.hWndMaster &&
        (!(moCur.dwPaintOptions & PAINT_NOEDGE) ?
            LOWORD(lParam) == HTBORDER :
           (LOWORD(lParam) == HTHSCROLL || LOWORD(lParam) == HTVSCROLL)))
    {
      POINT pt;
      RECT rc;
      DWORD dwSide=0;

      GetCursorPos(&pt);
      GetWindowRect(hWnd, &rc);
      if (pt.x >= rc.left && pt.x <= rc.left + 2)
        dwSide|=SIDE_LEFT;
      if (pt.x >= rc.right - 2 && pt.x <= rc.right)
        dwSide|=SIDE_RIGHT;
      if (pt.y >= rc.top && pt.y <= rc.top + 2)
        dwSide|=SIDE_TOP;
      if (pt.y >= rc.bottom - 2 && pt.y <= rc.bottom)
        dwSide|=SIDE_BOTTOM;

      if ((hWnd == lpFrameCurrent->ei.hWndMaster && dwSide == SIDE_RIGHT && lpFrameCurrent->ei.hWndClone1) ||
          (hWnd == lpFrameCurrent->ei.hWndClone1 && dwSide == SIDE_LEFT && lpFrameCurrent->ei.hWndMaster) ||
          (hWnd == lpFrameCurrent->ei.hWndClone2 && dwSide == SIDE_RIGHT && lpFrameCurrent->ei.hWndClone3) ||
          (hWnd == lpFrameCurrent->ei.hWndClone3 && dwSide == SIDE_LEFT && lpFrameCurrent->ei.hWndClone2))
      {
        SetCursor(hCursorSizeWE);
        hCursorClone=hCursorSizeWE;
        *lResult=1;
        return TRUE;
      }
      else if ((hWnd == lpFrameCurrent->ei.hWndMaster && dwSide == SIDE_BOTTOM && lpFrameCurrent->ei.hWndClone2) ||
               (hWnd == lpFrameCurrent->ei.hWndClone1 && dwSide == SIDE_BOTTOM && lpFrameCurrent->ei.hWndClone3) ||
               (hWnd == lpFrameCurrent->ei.hWndClone2 && dwSide == SIDE_TOP && lpFrameCurrent->ei.hWndMaster) ||
               (hWnd == lpFrameCurrent->ei.hWndClone3 && dwSide == SIDE_TOP && lpFrameCurrent->ei.hWndClone1))
      {
        SetCursor(hCursorSizeNS);
        hCursorClone=hCursorSizeNS;
        *lResult=1;
        return TRUE;
      }
      else if (lpFrameCurrent->ei.hWndMaster && lpFrameCurrent->ei.hWndClone1 && lpFrameCurrent->ei.hWndClone2 && lpFrameCurrent->ei.hWndClone3 &&
               ((hWnd == lpFrameCurrent->ei.hWndMaster && (dwSide & SIDE_RIGHT) && (dwSide & SIDE_BOTTOM)) ||
                (hWnd == lpFrameCurrent->ei.hWndClone1 && (dwSide & SIDE_LEFT) && (dwSide & SIDE_BOTTOM)) ||
                (hWnd == lpFrameCurrent->ei.hWndClone2 && (dwSide & SIDE_RIGHT) && (dwSide & SIDE_TOP)) ||
                (hWnd == lpFrameCurrent->ei.hWndClone3 && (dwSide & SIDE_LEFT) && (dwSide & SIDE_TOP))))
      {
        SetCursor(hCursorSizeALL);
        hCursorClone=hCursorSizeALL;
        *lResult=1;
        return TRUE;
      }
    }
    hCursorClone=NULL;
  }
  else if (uMsg == WM_LBUTTONDBLCLK ||
           uMsg == WM_NCLBUTTONDBLCLK)
  {
    if (hCursorClone)
    {
      DWORD dwFlags=0;

      if (hCursorClone == hCursorSizeWE || hCursorClone == hCursorSizeALL)
        dwFlags|=CN_CLONE1|CN_CLONE3;
      if (hCursorClone == hCursorSizeNS || hCursorClone == hCursorSizeALL)
        dwFlags|=CN_CLONE2|CN_CLONE3;
      SplitDestroy(lpFrameCurrent, dwFlags);
      SetFocus(lpFrameCurrent->ei.hWndEdit);

      if (lpFrameCurrent->ei.hWndClone1 || lpFrameCurrent->ei.hWndClone2 || lpFrameCurrent->ei.hWndClone3)
      {
        UpdateShowHScroll(lpFrameCurrent);
        ResizeEditWindow(lpFrameCurrent, 0);
      }
      else DoViewSplitWindow(FALSE, 0);

      if (lpFrameCurrent->ei.hWndClone1)
        nLastSplit=IDM_VIEW_SPLIT_WINDOW_WE;
      else if (lpFrameCurrent->ei.hWndClone2)
        nLastSplit=IDM_VIEW_SPLIT_WINDOW_NS;

      //Signal to Toolbar plugin for buttons update
      SendMessage(hMainWnd, WM_COMMAND, 0, 0);

      *lResult=0;
      return TRUE;
    }
  }
  else if (uMsg == WM_RBUTTONDOWN ||
           uMsg == WM_NCRBUTTONDOWN)
  {
    if (hCursorClone)
    {
      HMENU hPopupSize;
      POINT ptPos;
      int nPart;

      if (hPopupSize=CreatePopupMenu())
      {
        for (nPart=10; nPart < 100; nPart+=10)
        {
          xprintfW(wbuf, L"&%d/%d", nPart, 100 - nPart);
          AppendMenuWide(hPopupSize, MF_STRING, nPart, wbuf);
        }
        SetMenuDefaultItem(hPopupSize, 50, FALSE);
        GetCursorPos(&ptPos);

        if (nPart=TrackPopupMenu(hPopupSize, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON|TPM_CENTERALIGN, ptPos.x, ptPos.y, 0, hMainWnd, NULL))
        {
          if (hCursorClone == hCursorSizeWE || hCursorClone == hCursorSizeALL)
            lpFrameCurrent->rcMasterWindow.right=lpFrameCurrent->rcEditWindow.right * nPart / 100;
          if (hCursorClone == hCursorSizeNS || hCursorClone == hCursorSizeALL)
            lpFrameCurrent->rcMasterWindow.bottom=lpFrameCurrent->rcEditWindow.bottom * nPart / 100;
          ResizeEditWindow(lpFrameCurrent, 0);
        }
        DestroyMenu(hPopupSize);
        *lResult=0;
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_LBUTTONDOWN ||
           uMsg == WM_NCLBUTTONDOWN)
  {
    if (!(dwMouseCapture & MSC_SPLITSIZE))
    {
      if (hCursorClone)
      {
        rcInitMasterWindow=lpFrameCurrent->rcMasterWindow;
        GetCursorPos(&ptMouseDown);
        nMouseMove=1;
        SetMouseCapture(hWnd, MSC_SPLITSIZE);
        *lResult=0;
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (dwMouseCapture & MSC_SPLITSIZE)
    {
      RECT rcBeforeMove=lpFrameCurrent->rcMasterWindow;
      POINT ptPos;

      if (nMouseMove > 0)
      {
        if (--nMouseMove == 0)
        {
          ResizeEditWindow(lpFrameCurrent, REW_TEST);
        }
      }
      else
      {
        ResizeEditWindow(lpFrameCurrent, REW_TEST);
        GetCursorPos(&ptPos);
        if (hCursorClone == hCursorSizeWE || hCursorClone == hCursorSizeALL)
          lpFrameCurrent->rcMasterWindow.right+=(ptPos.x - ptMouseDown.x);
        if (hCursorClone == hCursorSizeNS || hCursorClone == hCursorSizeALL)
          lpFrameCurrent->rcMasterWindow.bottom+=(ptPos.y - ptMouseDown.y);
        ResizeEditWindow(lpFrameCurrent, REW_TEST);

        ptMouseDown.x+=(lpFrameCurrent->rcMasterWindow.right - rcBeforeMove.right);
        ptMouseDown.y+=(lpFrameCurrent->rcMasterWindow.bottom - rcBeforeMove.bottom);
      }
      *lResult=0;
      return TRUE;
    }
  }
  else if (uMsg == WM_LBUTTONUP ||
           uMsg == WM_CAPTURECHANGED)
  {
    if (dwMouseCapture & MSC_SPLITSIZE)
    {
      ReleaseMouseCapture(MSC_SPLITSIZE);

      if (nMouseMove == 0)
      {
        ResizeEditWindow(lpFrameCurrent, REW_TEST);

        if (uMsg == WM_LBUTTONUP)
        {
          ResizeEditWindow(lpFrameCurrent, 0);
          *lResult=0;
          return TRUE;
        }
        else if (uMsg == WM_CAPTURECHANGED)
        {
          lpFrameCurrent->rcMasterWindow=rcInitMasterWindow;
        }
      }
    }
  }
  return FALSE;
}

LRESULT CALLBACK NewMdiClientProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_WINDOWPOSCHANGING)
  {
    WINDOWPOS *lpWindowPos=(WINDOWPOS *)lParam;
    RECT rcMDIClient;

    if (!(lpWindowPos->flags & SWP_NOCOPYBITS))
    {
      GetWindowSize(hWnd, hMainWnd, &rcMDIClient);

      if ((!(lpWindowPos->flags & SWP_NOMOVE) && (lpWindowPos->x != rcMDIClient.left || lpWindowPos->y != rcMDIClient.top)) &&
          (!(lpWindowPos->flags & SWP_NOSIZE) && (lpWindowPos->cx != rcMDIClient.right || lpWindowPos->cy != rcMDIClient.bottom)))
      {
        //Size and position changed don't copy bits to avoid blinking.
        lpWindowPos->flags|=SWP_NOCOPYBITS;
      }
    }
  }
  else if (uMsg == WM_DROPFILES)
  {
    DropFiles((HDROP)wParam);
    return TRUE;
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    if (!(moCur.dwTabOptionsMDI & TAB_NOADD_LBUTTONDBLCLK))
    {
      CreateFrameWindow(NULL);
    }
  }
  else if (uMsg == WM_MBUTTONDOWN)
  {
    if (!(moCur.dwTabOptionsMDI & TAB_NOADD_MBUTTONDOWN))
    {
      CreateFrameWindow(NULL);
    }
  }
  else if (uMsg == WM_MDINEXT)
  {
    ActivateFrameWindow(lpFrameCurrent, (lParam?FWA_PREV:FWA_NEXT)|FWA_NOUPDATEORDER);
    return TRUE;
  }
  else if (uMsg == WM_MDIDESTROY)
  {
    //WM_MDIDESTROY should not be called directly. Use DestroyFrameWindow function.
    if ((HWND)wParam)
    {
      FRAMEDATA *lpFrame;
      int nTabItem;
      DWORD dwPrompt=dwChangedPrompt;

      if (lpFrame=(FRAMEDATA *)GetWindowLongPtrWide((HWND)wParam, GWLP_USERDATA))
      {
        //Is save prompt required
        if ((dwChangedPrompt & PROMPT_NONE) || !lpFrame->ei.bModified || (moCur.bSilentCloseEmptyMDI && !lpFrame->ei.wszFile[0] && !GetTextLength(lpFrame->ei.hWndEdit)))
          dwPrompt|=PROMPT_NONE;

        //Activate frame
        if (!(dwPrompt & PROMPT_NONE))
          ActivateFrameWindow(lpFrame, FWA_NOTIFY_BEFOREDESTROY);

        //Ask if document unsaved
        if (!SaveChanged(dwPrompt)) return TRUE;
        RecentFilesFrameUpdate(lpFrame);

        if ((nTabItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
        {
          //Update status
          if (lpFrame->ei.bModified)
            --nDocumentsModified;
          --nDocumentsCount;
          UpdateStatusUser(lpFrame, CSB_DOCUMENTSCOUNT|CSB_DOCUMENTSMODIFIED|CSB_DOCUMENTSSAVED);

          SendMessage(hMainWnd, AKDN_FRAME_DESTROY, 0, (LPARAM)lpFrame);

          //Avoid program exit blinking on last frame close
          if (nMainOnFinish)
          {
            if (nDocumentsCount == 0)
              SendMessage(hMdiClient, WM_SETREDRAW, FALSE, 0);
          }

          //Get frame window maximize state
          SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);

          //Destroy edits
          SplitDestroy(lpFrame, CN_CLONE1|CN_CLONE2|CN_CLONE3);

          bEditOnFinish=TRUE;
          SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrame->ei.hWndEdit, (LPARAM)lpFrame->ei.hDocEdit);
          DestroyWindow(lpFrame->ei.hWndEdit);
          lpFrame->ei.hWndEdit=NULL;
          lpFrame->ei.hDocEdit=NULL;
          bEditOnFinish=FALSE;

          //Remove tab item
          DeleteTabItem(hTab, nTabItem);

          //Delete frame data
          SetWindowLongPtrWide((HWND)wParam, GWLP_USERDATA, (LONG)0);

          //Save frame settings
          if (!nMainOnFinish || bFirstTabOnFinish)
          {
            bFirstTabOnFinish=FALSE;
            CopyFrameSettings(&fdDefault, lpFrame);
          }
          StackFrameDelete(&hFramesStack, lpFrame);
        }
      }
    }
  }

  return CallWindowProcWide(lpOldMdiClientProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewTabProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static int nDragItem;
  static int nDropItem;
  static int nMouseMove;

  if (uMsg == WM_CONTEXTMENU)
  {
    if (lParam != -1)
    {
      NCONTEXTMENU ncm;
      TCHITTESTINFO thti;
      POINT pt;
      HMENU hMdiChildMenu;
      int nItem;
      int i;

      GetCursorPos(&pt);
      thti.pt=pt;
      ScreenToClient(hWnd, &thti.pt);
      nItem=(int)SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti);

      if (nItem != -1)
      {
        SelectTabItem(hWnd, nItem);

        ncm.hWnd=hWnd;
        ncm.uType=NCM_TAB;
        ncm.pt=pt;
        ncm.bMouse=TRUE;
        ncm.bProcess=TRUE;
        SendMessage(hMainWnd, AKDN_CONTEXTMENU, 0, (LPARAM)&ncm);
        if (ncm.bProcess)
        {
          if (nMDI == WMD_PMDI)
          {
            if (i=TrackPopupMenu(hMenuWindow, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hMainWnd, NULL))
              PostMessage(hMainWnd, WM_COMMAND, i, 0);
          }
          else if (nMDI == WMD_MDI)
          {
            hMdiChildMenu=GetSystemMenu(lpFrameCurrent->hWndEditParent, FALSE);
            if (i=TrackPopupMenu(hMdiChildMenu, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, lpFrameCurrent->hWndEditParent, NULL))
              PostMessage(lpFrameCurrent->hWndEditParent, WM_SYSCOMMAND, i, 0);
          }
        }
        return TRUE;
      }
    }
    return TRUE;
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    if (!(moCur.dwTabOptionsMDI & TAB_NODEL_LBUTTONDBLCLK))
    {
      DestroyFrameWindow(lpFrameCurrent);
      return TRUE;
    }
  }
  else if (uMsg == WM_MBUTTONDOWN)
  {
    FRAMEDATA *lpFrame;
    int nItem;

    if ((nItem=GetTabItemFromCursorPos(hTab)) != -1)
    {
      if (!(moCur.dwTabOptionsMDI & TAB_NODEL_MBUTTONDOWN))
      {
        lpFrame=(FRAMEDATA *)GetTabParamFromItem(hTab, nItem);
        DestroyFrameWindow(lpFrame);
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_TIMER)
  {
    if (wParam == TIMERID_TABOPEN)
    {
      if (dwTabOpenTimer)
      {
        int nItem;

        //Remove timer
        KillTimer(hTab, dwTabOpenTimer);
        dwTabOpenTimer=0;
        nTabOpenItem=-1;

        //Activate tab
        if ((nItem=GetTabItemFromCursorPos(hTab)) != -1)
        {
          SelectTabItem(hTab, nItem);

          if (nMDI == WMD_MDI)
          {
            //Restore minimized frame
            if (GetWindowLongPtrWide(lpFrameCurrent->hWndEditParent, GWL_STYLE) & WS_MINIMIZE)
            {
              SendMessage(hMdiClient, WM_MDIRESTORE, (WPARAM)lpFrameCurrent->hWndEditParent, 0);
            }
          }
        }
      }
    }
    return 0;
  }

  //Drag'n'Drop
  if (uMsg == WM_LBUTTONDOWN)
  {
    if (!(dwMouseCapture & MSC_TABDRAGDROP))
    {
      TCHITTESTINFO thti;

      thti.pt.x=LOWORD(lParam);
      thti.pt.y=HIWORD(lParam);
      nDragItem=(int)SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti);

      if (nDragItem != -1)
      {
        nMouseMove=4;
        SetMouseCapture(hWnd, MSC_TABDRAGDROP);

        SelectTabItem(hWnd, nDragItem);
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (dwMouseCapture & MSC_TABDRAGDROP)
    {
      if (nMouseMove > 0)
      {
        if (--nMouseMove == 0)
        {
          SetCursor(hCursorDragMove);
        }
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_LBUTTONUP)
  {
    if (dwMouseCapture & MSC_TABDRAGDROP)
    {
      POINT pt;

      ReleaseMouseCapture(MSC_TABDRAGDROP);

      if (nMouseMove == 0)
      {
        GetCursorPos(&pt);
        nDropItem=GetTabItemForDrop(hWnd, &pt);

        if (nDropItem != -1 && nDropItem != nDragItem)
        {
          MoveTabItem(hWnd, nDragItem, nDropItem);
        }
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_CAPTURECHANGED)
  {
    if (dwMouseCapture & MSC_TABDRAGDROP)
    {
      ReleaseMouseCapture(MSC_TABDRAGDROP);
    }
  }

  return CallWindowProcWide(lpOldTabProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK DockProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  DOCK *lpDock;
  LRESULT lResult;

  if (DockMessages(hWnd, uMsg, wParam, lParam, &lResult))
    return lResult;

  if (lpDock=StackDockFindWindow(&hDocksStack, hWnd, FALSE))
  {
    return CallWindowProcWide(lpDock->lpOldDockProc, hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

BOOL CALLBACK DockMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  static MINMAXINFO mmi;
  static RECT rcEdge;
  static RECT rcDrop;
  static DOCK *dkDragSource;
  static DOCK *dkDropTarget;
  static int nMouseEdge;
  static int nMouseMove;
  static int nDropSide;
  DOCK *dkData;
  POINT pt;
  RECT rc;

  if (uMsg == WM_LBUTTONDOWN ||
      uMsg == WM_MOUSEMOVE ||
      uMsg == WM_LBUTTONUP ||
      uMsg == WM_CAPTURECHANGED)
  {
    if (dkData=StackDockFindWindow(&hDocksStack, hWnd, FALSE))
    {
      if (uMsg == WM_LBUTTONDOWN)
      {
        if (!(dwMouseCapture & MSC_DOCKSIZE) &&
            !(dwMouseCapture & MSC_DOCKDRAGDROP))
        {
          GetCursorPos(&pt);

          if ((nMouseEdge=GetMouseEdge(hWnd, &pt)) == GetOppEdge(dkData->nSide))
          {
            if (!(dkData->dwFlags & DKF_FIXEDSIZE))
            {
              xmemset(&mmi, 0, sizeof(MINMAXINFO));
              mmi.ptMinTrackSize.x=0;
              mmi.ptMinTrackSize.y=0;
              mmi.ptMaxTrackSize.x=0x0FFFFFFF;
              mmi.ptMaxTrackSize.y=0x0FFFFFFF;
              SendMessage(hMainWnd, AKDN_DOCK_GETMINMAXINFO, (WPARAM)dkData, (LPARAM)&mmi);

              SetEdgeCursor(nMouseEdge);
              GetMovingRect(dkData, &pt, &mmi, &rcEdge);
              DrawMovingRect(&rcEdge);

              SetMouseCapture(hWnd, MSC_DOCKSIZE);

              SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONSTART, (WPARAM)dkData, DKC_SIZING);
            }
          }
          else
          {
            if (dkData->dwFlags & DKF_DRAGDROP)
            {
              rc=dkData->rcDragDrop;

              if (ClientSizeToScreenRect(dkData->hWnd, &rc))
              {
                if (PtInRect(&rc, pt))
                {
                  nDropSide=0;
                  dkDragSource=dkData;
                  dkDropTarget=dkData;

                  nMouseMove=4;
                  SetMouseCapture(hWnd, MSC_DOCKDRAGDROP);

                  SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONSTART, (WPARAM)dkData, DKC_DRAGDROP);
                }
              }
            }
          }
        }
      }
      else if (uMsg == WM_MOUSEMOVE)
      {
        GetCursorPos(&pt);

        if ((dwMouseCapture & MSC_DOCKSIZE) || (dwMouseCapture & MSC_DOCKDRAGDROP))
        {
          if (dwMouseCapture & MSC_DOCKSIZE)
          {
            DrawMovingRect(&rcEdge);
            GetMovingRect(dkData, &pt, &mmi, &rcEdge);
            DrawMovingRect(&rcEdge);
          }
          else if (dwMouseCapture & MSC_DOCKDRAGDROP)
          {
            if (nMouseMove > 0)
            {
              if (--nMouseMove == 0)
              {
                GetWindowRect(dkDropTarget->hWnd, &rcDrop);
                DrawMovingRect(&rcDrop);
                SetEdgeCursor(0);
              }
            }
            if (nMouseMove == 0)
            {
              DOCK *dkPoint;
              int nSide=0;

              if ((dkPoint=StackDockFromPoint(&hDocksStack, &pt)) && !(dkPoint->dwFlags & DKF_HIDDEN))
              {
                if (dkPoint != dkDropTarget)
                {
                  if ((dkPoint->nSide == DKS_LEFT && !(dkDragSource->dwFlags & DKF_NODROPLEFT)) ||
                      (dkPoint->nSide == DKS_TOP && !(dkDragSource->dwFlags & DKF_NODROPTOP)) ||
                      (dkPoint->nSide == DKS_RIGHT && !(dkDragSource->dwFlags & DKF_NODROPRIGHT)) ||
                      (dkPoint->nSide == DKS_BOTTOM && !(dkDragSource->dwFlags & DKF_NODROPBOTTOM)))
                  {
                    nDropSide=0;
                    dkDropTarget=dkPoint;
                    DrawMovingRect(&rcDrop);
                    GetWindowRect(dkDropTarget->hWnd, &rcDrop);
                    DrawMovingRect(&rcDrop);
                  }
                }
              }
              else
              {
                rc=nsSize.rcCurrent;

                if (ClientSizeToScreenRect(hMainWnd, &rc))
                {
                  if (PtInRect(&rc, pt))
                  {
                    if (pt.x >= rc.left && pt.x <= rc.left + DOCK_MAINMIN_X)
                    {
                      rc.right=rc.left + dkDragSource->rcSize.right;
                      nSide=DKS_LEFT;
                    }
                    else if (pt.y >= rc.top && pt.y <= rc.top + DOCK_MAINMIN_Y)
                    {
                      rc.bottom=rc.top + dkDragSource->rcSize.bottom;
                      nSide=DKS_TOP;
                    }
                    else if (pt.x <= rc.right && pt.x >= rc.right - DOCK_MAINMIN_X)
                    {
                      rc.left=rc.right - dkDragSource->rcSize.right;
                      nSide=DKS_RIGHT;
                    }
                    else if (pt.y <= rc.bottom && pt.y >= rc.bottom - DOCK_MAINMIN_Y)
                    {
                      rc.top=rc.bottom - dkDragSource->rcSize.bottom;
                      nSide=DKS_BOTTOM;
                    }

                    if (nSide && nSide != nDropSide && nSide != dkDragSource->nSide)
                    {
                      if ((nSide == DKS_LEFT && !(dkDragSource->dwFlags & DKF_NODROPLEFT)) ||
                          (nSide == DKS_TOP && !(dkDragSource->dwFlags & DKF_NODROPTOP)) ||
                          (nSide == DKS_RIGHT && !(dkDragSource->dwFlags & DKF_NODROPRIGHT)) ||
                          (nSide == DKS_BOTTOM && !(dkDragSource->dwFlags & DKF_NODROPBOTTOM)))
                      {
                        nDropSide=nSide;
                        dkDropTarget=NULL;
                        DrawMovingRect(&rcDrop);
                        rcDrop=rc;
                        DrawMovingRect(&rcDrop);
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else
        {
          if (!(dkData->dwFlags & DKF_FIXEDSIZE))
          {
            if ((nMouseEdge=GetMouseEdge(hWnd, &pt)) == GetOppEdge(dkData->nSide))
            {
              SetEdgeCursor(nMouseEdge);
            }
          }
        }
      }
      else if (uMsg == WM_LBUTTONUP)
      {
        if ((dwMouseCapture & MSC_DOCKSIZE) || (dwMouseCapture & MSC_DOCKDRAGDROP))
        {
          if (dwMouseCapture & MSC_DOCKSIZE)
          {
            DrawMovingRect(&rcEdge);
            ReleaseMouseCapture(MSC_DOCKSIZE);

            if (ScreenRectToClientSize(hMainWnd, &rcEdge))
            {
              if (dkData->nSide == DKS_LEFT ||
                  dkData->nSide == DKS_RIGHT)
              {
                dkData->rcSize.right=rcEdge.right;
              }
              else if (dkData->nSide == DKS_TOP ||
                       dkData->nSide == DKS_BOTTOM)
              {
                dkData->rcSize.bottom=rcEdge.bottom;
              }
              hDocksStack.nSizingSide=dkData->nSide;
              hDocksStack.nSizingType=DKC_SIZING;
              UpdateSize();
              hDocksStack.nSizingSide=0;
              hDocksStack.nSizingType=0;
              SendMessage(hMainWnd, AKDN_DOCK_RESIZE, (WPARAM)dkData, DKC_SIZING);
            }
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_SIZING);
          }
          else if (dwMouseCapture & MSC_DOCKDRAGDROP)
          {
            if (nMouseMove == 0)
              DrawMovingRect(&rcDrop);
            ReleaseMouseCapture(MSC_DOCKDRAGDROP);

            if (dkDropTarget != dkDragSource)
            {
              if (dkDropTarget)
                nDropSide=dkDropTarget->nSide;
              rc=rcDrop;

              if (ScreenRectToClientSize(hMainWnd, &rc))
              {
                if (nDropSide == DKS_LEFT ||
                    nDropSide == DKS_RIGHT)
                {
                  dkDragSource->rcSize.left=rc.left;
                }
                else if (nDropSide == DKS_TOP ||
                         nDropSide == DKS_BOTTOM)
                {
                  dkDragSource->rcSize.top=rc.top;
                }
                DockSetSide(&hDocksStack, dkDragSource, nDropSide);
                hDocksStack.nSizingSide=dkDragSource->nSide;
                hDocksStack.nSizingType=DKC_DRAGDROP;
                UpdateSize();
                hDocksStack.nSizingSide=0;
                hDocksStack.nSizingType=0;
                SendMessage(hMainWnd, AKDN_DOCK_RESIZE, (WPARAM)dkData, DKC_DRAGDROP);
              }
            }
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_DRAGDROP);
          }
        }
      }
      else if (uMsg == WM_CAPTURECHANGED)
      {
        if ((dwMouseCapture & MSC_DOCKSIZE) || (dwMouseCapture & MSC_DOCKDRAGDROP))
        {
          if (dwMouseCapture & MSC_DOCKSIZE)
          {
            DrawMovingRect(&rcEdge);
            ReleaseMouseCapture(MSC_DOCKSIZE);
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_SIZING);
          }
          else if (dwMouseCapture & MSC_DOCKDRAGDROP)
          {
            if (nMouseMove == 0)
              DrawMovingRect(&rcDrop);
            ReleaseMouseCapture(MSC_DOCKDRAGDROP);
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_DRAGDROP);
          }
        }
      }
    }
  }
  return FALSE;
}

LRESULT CALLBACK NewButtonDrawProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static BUTTONDRAWITEM *lpButtonDrawItem;
  static BOOL bMousePush=FALSE;
  LRESULT lResult;

  if (lpButtonDrawItem=StackButtonDrawGet(&hButtonDrawStack, hWnd))
  {
    if (uMsg == WM_LBUTTONDOWN ||
        uMsg == WM_LBUTTONDBLCLK)
    {
      if (!(dwMouseCapture & MSC_BUTTONPRESS))
      {
        ReleaseMouseCapture(MSC_BUTTONOVER);
        SetMouseCapture(hWnd, MSC_BUTTONPRESS);
        bMousePush=TRUE;
      }
      InvalidateRect(hWnd, NULL, TRUE);
      return 0;
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
      if (!(dwMouseCapture & MSC_BUTTONPRESS))
      {
        if (!(dwMouseCapture & MSC_BUTTONOVER))
        {
          SetMouseCapture(hWnd, MSC_BUTTONOVER);
          InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (!IsCursorOnWindow(hWnd))
        {
          ReleaseMouseCapture(MSC_BUTTONOVER);
          InvalidateRect(hWnd, NULL, TRUE);
          UpdateWindow(hWnd);
        }
      }
      else
      {
        if (IsCursorOnWindow(hWnd) != bMousePush)
        {
          bMousePush=!bMousePush;
          InvalidateRect(hWnd, NULL, TRUE);
        }
      }
    }
    else if (uMsg == WM_LBUTTONUP ||
             uMsg == WM_CAPTURECHANGED)
    {
      if (dwMouseCapture & MSC_BUTTONPRESS)
      {
        BOOL bPost=FALSE;

        if (uMsg == WM_LBUTTONUP)
          bPost=bMousePush;
        bMousePush=FALSE;
        ReleaseMouseCapture(MSC_BUTTONPRESS);
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);

        if (bPost)
          PostMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), 0), 0);
      }
      else if (dwMouseCapture & MSC_BUTTONOVER)
      {
        ReleaseMouseCapture(MSC_BUTTONOVER);
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
      }
      return 0;
    }
    else if (uMsg == WM_SETFOCUS ||
             uMsg == WM_KILLFOCUS)
    {
      if (lpButtonDrawItem->bd.dwFlags & BIF_ENABLEFOCUS)
        InvalidateRect(hWnd, NULL, TRUE);
    }
    else if (uMsg == WM_ENABLE)
    {
      InvalidateRect(hWnd, NULL, TRUE);
    }
    else if (uMsg == WM_ERASEBKGND)
    {
      //return 1;
    }
    else if (uMsg == WM_PAINT)
    {
      PAINTSTRUCT ps;
      HDC hDC;
      RECT rcButton;

      if (BeginPaint(hWnd, &ps))
      {
        hDC=ps.hdc;
        GetClientRect(hWnd, &rcButton);

        //Draw edge
        if ((dwMouseCapture & MSC_BUTTONPRESS) && bMousePush)
        {
          DrawEdge(hDC, &rcButton, EDGE_SUNKEN, BF_RECT);
        }
        else if (dwMouseCapture & MSC_BUTTONOVER)
        {
          DrawEdge(hDC, &rcButton, BDR_RAISEDINNER, BF_RECT);
        }
        else
        {
          if ((lpButtonDrawItem->bd.dwFlags & BIF_ENABLEFOCUS) && hWnd == GetFocus())
          {
            //Draw focus rect
            DrawFocusRect(hDC, &rcButton);
          }
          else if (lpButtonDrawItem->bd.dwFlags & BIF_ETCHED)
          {
            DrawEdge(hDC, &rcButton, EDGE_ETCHED, BF_RECT);
          }
        }

        if ((lpButtonDrawItem->bd.dwFlags & BIF_BITMAP) || (lpButtonDrawItem->bd.dwFlags & BIF_ICON))
        {
          //Draw image
          if (lpButtonDrawItem->bd.hImage)
          {
            int x=(rcButton.right - rcButton.left) / 2 - lpButtonDrawItem->bd.nImageWidth / 2 + (bMousePush?1:0);
            int y=(rcButton.bottom - rcButton.top) / 2 - lpButtonDrawItem->bd.nImageHeight / 2 + (bMousePush?1:0);
            DWORD dwFlags=0;

            if (lpButtonDrawItem->bd.dwFlags & BIF_BITMAP)
              dwFlags|=DST_BITMAP;
            else if (lpButtonDrawItem->bd.dwFlags & BIF_ICON)
              dwFlags|=DST_ICON;
            if (!IsWindowEnabled(hWnd))
              dwFlags|=DSS_DISABLED;
            DrawStateA(hDC, NULL, (DRAWSTATEPROC)NULL, (LPARAM)lpButtonDrawItem->bd.hImage, (WPARAM)NULL, x, y, 0, 0, dwFlags);
          }
        }
        else if ((lpButtonDrawItem->bd.dwFlags & BIF_CROSS) ||
                 (lpButtonDrawItem->bd.dwFlags & BIF_DOWNARROW) ||
                 (lpButtonDrawItem->bd.dwFlags & BIF_UPARROW) ||
                 (lpButtonDrawItem->bd.dwFlags & BIF_LEFTARROW) ||
                 (lpButtonDrawItem->bd.dwFlags & BIF_RIGHTARROW))
        {
          //Draw cross
          HPEN hPen;
          HPEN hPenOld;
          COLORREF crPenColor=GetSysColor(IsWindowEnabled(hWnd)?COLOR_BTNTEXT:COLOR_GRAYTEXT);
          int x=(rcButton.right - rcButton.left) / 2 + (bMousePush?1:0);
          int y=(rcButton.bottom - rcButton.top) / 2 + (bMousePush?1:0);

          if (hPen=CreatePen(PS_SOLID, 0, crPenColor))
          {
            hPenOld=(HPEN)SelectObject(hDC, hPen);

            //Draw cross manually
            {
              POINT ptCross[]={
                    {0, 0}, {2, 0},
                    {6, 0}, {8, 0},
                    {1, 1}, {3, 1},
                    {5, 1}, {7, 1},
                    {2, 2}, {6, 2},
                    {3, 3}, {5, 3},
                    {2, 4}, {6, 4},
                    {1, 5}, {3, 5},
                    {5, 5}, {7, 5},
                    {0, 6}, {2, 6},
                    {6, 6}, {8, 6}};
              POINT ptDownArrow[]={
                    {0, 0}, {7, 0},
                    {1, 1}, {6, 1},
                    {2, 2}, {5, 2},
                    {3, 3}, {4, 3}};
              POINT ptUpArrow[]={
                    {3, 0}, {4, 0},
                    {2, 1}, {5, 1},
                    {1, 2}, {6, 2},
                    {0, 3}, {7, 3}};
              POINT ptLeftArrow[]={
                    {3, 0}, {4, 0},
                    {2, 1}, {4, 1},
                    {1, 2}, {4, 2},
                    {0, 3}, {4, 3},
                    {1, 4}, {4, 4},
                    {2, 5}, {4, 5},
                    {3, 6}, {4, 6}};
              POINT ptRightArrow[]={
                    {0, 0}, {1, 0},
                    {0, 1}, {2, 1},
                    {0, 2}, {3, 2},
                    {0, 3}, {4, 3},
                    {0, 4}, {3, 4},
                    {0, 5}, {2, 5},
                    {0, 6}, {1, 6}};
              POINT *ptDraw=NULL;
              int nElements=0;
              int i;

              if (lpButtonDrawItem->bd.dwFlags & BIF_CROSS)
              {
                ptDraw=ptCross;
                nElements=sizeof(ptCross) / sizeof(POINT);
                x-=(8) / 2;
                y-=(7) / 2;

                //Draw on one pixel higher
                --y;
              }
              else if (lpButtonDrawItem->bd.dwFlags & BIF_DOWNARROW)
              {
                ptDraw=ptDownArrow;
                nElements=sizeof(ptDownArrow) / sizeof(POINT);
                x-=(7) / 2;
                y-=(4) / 2;
              }
              else if (lpButtonDrawItem->bd.dwFlags & BIF_UPARROW)
              {
                ptDraw=ptUpArrow;
                nElements=sizeof(ptUpArrow) / sizeof(POINT);
                x-=(7) / 2;
                y-=(4) / 2;
              }
              else if (lpButtonDrawItem->bd.dwFlags & BIF_LEFTARROW)
              {
                ptDraw=ptLeftArrow;
                nElements=sizeof(ptLeftArrow) / sizeof(POINT);
                x-=(4) / 2;
                y-=(7) / 2;
              }
              else if (lpButtonDrawItem->bd.dwFlags & BIF_RIGHTARROW)
              {
                ptDraw=ptRightArrow;
                nElements=sizeof(ptRightArrow) / sizeof(POINT);
                x-=(4) / 2;
                y-=(7) / 2;
              }

              for (i=0; i < nElements; i+=2)
              {
                MoveToEx(hDC, x + ptDraw[i].x, y + ptDraw[i].y, NULL);
                LineTo(hDC, x + ptDraw[i + 1].x, y + ptDraw[i + 1].y);
              }
            }
            SelectObject(hDC, hPenOld);
            DeleteObject(hPen);
          }
        }
        EndPaint(hWnd, &ps);
        return TRUE;
      }
    }

    if (!IsWindowUnicode(hWnd))
      lResult=CallWindowProcA(lpButtonDrawItem->OldButtonProc, hWnd, uMsg, wParam, lParam);
    else
      lResult=CallWindowProcW(lpButtonDrawItem->OldButtonProc, hWnd, uMsg, wParam, lParam);

    //Remove from stack
    if (uMsg == WM_DESTROY)
    {
      StackButtonDrawDelete(&hButtonDrawStack, lpButtonDrawItem);
    }
    return lResult;
  }
  return 0;
}

LRESULT CALLBACK NewHotkeyInputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static TOOLINFOA tiA;
  static TOOLINFOW tiW;
  static HWND hWndToolTip=NULL;
  static HWND hWndLButtonClick=NULL;
  static WORD wInitHotkey;
  LRESULT lResult=0;

  wInitHotkey=(WORD)GetWindowLongPtrWide(hWnd, GWLP_USERDATA);

  if (uMsg == HKM_SETHOTKEY)
  {
    wInitHotkey=(WORD)wParam;

    SetWindowLongPtrWide(hWnd, GWLP_USERDATA, wInitHotkey);
  }
  else if (uMsg == WM_LBUTTONDOWN)
  {
    hWndLButtonClick=hWnd;
  }
  else if (uMsg == WM_KILLFOCUS ||
           uMsg == WM_DESTROY)
  {
    if (hWndLButtonClick == hWnd)
      hWndLButtonClick=NULL;

    if (hWndToolTip)
    {
      if (GetFocus() == hWndToolTip)
        PostMessage(hWnd, WM_SETFOCUS, (WPARAM)NULL, 0);
      DestroyWindow(hWndToolTip);
      hWndToolTip=NULL;
    }
  }
  else if ((uMsg == WM_GETDLGCODE && lParam &&
            (((MSG *)lParam)->message == WM_KEYDOWN ||
             ((MSG *)lParam)->message == WM_SYSKEYDOWN)) ||
           uMsg == WM_KEYDOWN ||
           uMsg == WM_SYSKEYDOWN)
  {
    MSG msg;
    RECT rcWindow;
    BOOL bOwnKey=FALSE;
    BYTE nMod=0;
    WORD wHotkey;

    if (uMsg == WM_GETDLGCODE)
    {
      xmemcpy(&msg, (MSG *)lParam, sizeof(MSG));
    }
    else
    {
      msg.hwnd=hWnd;
      msg.message=uMsg;
      msg.wParam=wParam;
      msg.lParam=lParam;
    }

    if (msg.wParam == VK_ESCAPE ||
        msg.wParam == VK_BACK ||
        msg.wParam == VK_DELETE ||
        msg.wParam == VK_SPACE ||
        msg.wParam == VK_RETURN ||
        msg.wParam == VK_TAB)
    {
      if ((msg.lParam >> 24) & 1) nMod|=HOTKEYF_EXT;
      if (GetKeyState(VK_CONTROL) & 0x80) nMod|=HOTKEYF_CONTROL;
      if (GetKeyState(VK_MENU) & 0x80) nMod|=HOTKEYF_ALT;
      if (GetKeyState(VK_SHIFT) & 0x80) nMod|=HOTKEYF_SHIFT;

      if (msg.wParam == VK_ESCAPE && !(nMod & HOTKEYF_CONTROL) && !(nMod & HOTKEYF_ALT) && !(nMod & HOTKEYF_SHIFT))
      {
        if ((WORD)SendMessage(msg.hwnd, HKM_GETHOTKEY, 0, 0) != wInitHotkey)
        {
          if (uMsg == WM_GETDLGCODE)
            return DLGC_WANTMESSAGE;

          //Reset to initial hotkey
          if (!IsWindowUnicode(msg.hwnd))
            CallWindowProcA(lpOldHotkeyInputProc, msg.hwnd, HKM_SETHOTKEY, wInitHotkey, 0);
          else
            CallWindowProcW(lpOldHotkeyInputProc, msg.hwnd, HKM_SETHOTKEY, wInitHotkey, 0);
          SendMessage(GetParent(msg.hwnd), WM_COMMAND, GetDlgCtrlID(msg.hwnd), 0);
          bOwnKey=TRUE;
        }
      }
      else if ((msg.wParam == VK_BACK || msg.wParam == VK_DELETE) && !(nMod & HOTKEYF_CONTROL) && !(nMod & HOTKEYF_ALT) && !(nMod & HOTKEYF_SHIFT))
      {
        if ((WORD)SendMessage(msg.hwnd, HKM_GETHOTKEY, 0, 0) == 0)
        {
          if (uMsg == WM_GETDLGCODE)
            return DLGC_WANTMESSAGE;

          //If hotkey window already empty assign Backspace or Delete
          if (!IsWindowUnicode(msg.hwnd))
            CallWindowProcA(lpOldHotkeyInputProc, msg.hwnd, HKM_SETHOTKEY, MAKEWORD(msg.wParam, nMod), 0);
          else
            CallWindowProcW(lpOldHotkeyInputProc, msg.hwnd, HKM_SETHOTKEY, MAKEWORD(msg.wParam, nMod), 0);
          SendMessage(GetParent(msg.hwnd), WM_COMMAND, GetDlgCtrlID(msg.hwnd), 0);
          bOwnKey=TRUE;
        }
      }
      else if (msg.wParam == VK_ESCAPE ||
               msg.wParam == VK_BACK ||
               msg.wParam == VK_DELETE ||
               msg.wParam == VK_SPACE ||
               msg.wParam == VK_RETURN ||
               (msg.wParam == VK_TAB && (nMod & HOTKEYF_CONTROL)) ||
               (msg.wParam == VK_TAB && hWndLButtonClick == msg.hwnd))
      {
        if (uMsg == WM_GETDLGCODE)
          return DLGC_WANTMESSAGE;

        if (!IsWindowUnicode(msg.hwnd))
          CallWindowProcA(lpOldHotkeyInputProc, msg.hwnd, HKM_SETHOTKEY, MAKEWORD(msg.wParam, nMod), 0);
        else
          CallWindowProcW(lpOldHotkeyInputProc, msg.hwnd, HKM_SETHOTKEY, MAKEWORD(msg.wParam, nMod), 0);
        SendMessage(GetParent(msg.hwnd), WM_COMMAND, GetDlgCtrlID(msg.hwnd), 0);
        bOwnKey=TRUE;
      }
    }

    if (!bOwnKey)
    {
      if (!IsWindowUnicode(msg.hwnd))
        lResult=CallWindowProcA(lpOldHotkeyInputProc, hWnd, uMsg, wParam, lParam);
      else
        lResult=CallWindowProcW(lpOldHotkeyInputProc, hWnd, uMsg, wParam, lParam);
    }
    if (uMsg == WM_GETDLGCODE)
      return lResult;

    //Show tooltip if hotkey already exist
    {
      wchar_t wszHotkeyOwner[MAX_PATH];
      int nOwner=HKO_NONE;

      if (wHotkey=(WORD)SendMessage(hWnd, HKM_GETHOTKEY, 0, 0))
      {
        if (wHotkey != wInitHotkey)
        {
          if (nOwner=CheckHotkey(wHotkey, wszHotkeyOwner))
          {
            if (!hWndToolTip)
            {
              hWndToolTip=CreateWindowExWide(WS_EX_TOPMOST,
                                             L"tooltips_class32",
                                             NULL,
                                             WS_POPUP|TTS_NOPREFIX,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             hWnd,
                                             NULL,
                                             hInstance,
                                             NULL);

              if (bOldWindows)
              {
                WideCharToMultiByte(CP_ACP, 0, wszHotkeyOwner, -1, buf, MAX_PATH, NULL, NULL);
                tiA.cbSize=sizeof(TOOLINFOA);
                tiA.uFlags=TTF_ABSOLUTE|TTF_TRACK;
                tiA.hwnd=hWnd;
                tiA.hinst=hInstance;
                tiA.uId=0;
                tiA.lpszText=buf;
                SendMessage(hWndToolTip, TTM_ADDTOOLA, 0, (LPARAM)&tiA);
              }
              else
              {
                tiW.cbSize=sizeof(TOOLINFOW);
                tiW.uFlags=TTF_ABSOLUTE|TTF_TRACK;
                tiW.hwnd=hWnd;
                tiW.hinst=hInstance;
                tiW.uId=0;
                tiW.lpszText=wszHotkeyOwner;
                SendMessage(hWndToolTip, TTM_ADDTOOLW, 0, (LPARAM)&tiW);
              }
            }
            else
            {
              if (bOldWindows)
              {
                WideCharToMultiByte(CP_ACP, 0, wszHotkeyOwner, -1, buf, MAX_PATH, NULL, NULL);
                tiA.lpszText=buf;
                SendMessage(hWndToolTip, TTM_SETTOOLINFOA, 0, (LPARAM)&tiA);
              }
              else
              {
                tiW.lpszText=wszHotkeyOwner;
                SendMessage(hWndToolTip, TTM_SETTOOLINFOW, 0, (LPARAM)&tiW);
              }
            }
            GetWindowRect(hWnd, &rcWindow);
            SendMessage(hWndToolTip, TTM_TRACKPOSITION, 0, MAKELONG(rcWindow.left, rcWindow.top + RectH(&rcWindow)));
            SendMessage(hWndToolTip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&tiW);
          }
        }
      }
      if (!nOwner)
      {
        if (hWndToolTip) SendMessage(hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&tiW);
      }
    }
    return lResult;
  }

  if (!IsWindowUnicode(hWnd))
    lResult=CallWindowProcA(lpOldHotkeyInputProc, hWnd, uMsg, wParam, lParam);
  else
    lResult=CallWindowProcW(lpOldHotkeyInputProc, hWnd, uMsg, wParam, lParam);

  //Draw color rectangle if hotkey already exist
  if (uMsg == WM_PAINT)
  {
    HPEN hPen;
    HPEN hPenOld;
    HDC hDC;
    RECT rcWindow;
    WORD wHotkey;
    BOOL bDrawRect=FALSE;

    if (wHotkey=(WORD)SendMessage(hWnd, HKM_GETHOTKEY, 0, 0))
      if (wHotkey != wInitHotkey)
        if (CheckHotkey(wHotkey, NULL))
          bDrawRect=TRUE;

    if (bDrawRect)
    {
      if (hDC=GetDC(hWnd))
      {
        GetClientRect(hWnd, &rcWindow);
        hPen=CreatePen(PS_SOLID, 0, RGB(0xFF, 0x00, 0x00));
        hPenOld=(HPEN)SelectObject(hDC, hPen);

        MoveToEx(hDC, rcWindow.left, rcWindow.top, NULL);
        LineTo(hDC, rcWindow.left, rcWindow.bottom - 1);
        LineTo(hDC, rcWindow.right - 1, rcWindow.bottom - 1);
        LineTo(hDC, rcWindow.right - 1, rcWindow.top);
        LineTo(hDC, rcWindow.left, rcWindow.top);

        SelectObject(hDC, hPenOld);
        DeleteObject(hPen);
        ReleaseDC(hWnd, hDC);
      }
    }
  }
  return lResult;
}

LRESULT CALLBACK DummyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  return DefWindowProcWide(hWnd, uMsg, wParam, lParam);
}
