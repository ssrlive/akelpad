#ifndef _SCRIPTS_H_
#define _SCRIPTS_H_


//Includes
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "MethodFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"
#include "Scripts_h.h"
#include "IActiveScriptSite.h"
#include "IConstants.h"
#include "IGlobal.h"
#include "IDocument.h"
#include "IScriptSettings.h"
#include "ISystemFunction.h"
#include "IWArguments.h"
#include "IWScript.h"
#include "IServer.h"

//Defines
#define DLLA_SCRIPTS_EXEC            1
#define DLLA_SCRIPTS_EXECWAIT        2
#define DLLA_SCRIPTS_EDIT            3
#define DLLA_SCRIPTS_EXECMAINTHREAD  4
#define DLLA_SCRIPTS_HANDLEBYNAME    5
#define DLLA_SCRIPTS_HANDLEBYTHREAD  6
#define DLLA_SCRIPTS_DIRECTCALL      7

#define STRID_ARCHITECTURE_MISMATCH 1
#define STRID_HOTKEY_EXISTS         2
#define STRID_EXECUTE_ERROR         3
#define STRID_ENGINE_ERROR          4
#define STRID_READFILE_ERROR        5
#define STRID_DEBUG_ERROR           6
#define STRID_SCRIPTERROR           7
#define STRID_STOP                  8
#define STRID_CONTINUE              9
#define STRID_DEBUG                 10
#define STRID_JIT                   11
#define STRID_FROMSTART             12
#define STRID_CODE                  13
#define STRID_CODE_MEMREAD          14
#define STRID_CODE_MEMWRITE         15
#define STRID_CODE_MEMFREE          16
#define STRID_CODE_MEMLEAK          17
#define STRID_CODE_SYSCALL          18
#define STRID_DEBUG_MEMLOCATE       19
#define STRID_DEBUG_MEMREAD         20
#define STRID_DEBUG_MEMWRITE        21
#define STRID_DEBUG_MEMFREE         22
#define STRID_DEBUG_MEMLEAK         23
#define STRID_DEBUG_SYSCALL         24
#define STRID_DEBUG_SYSFUNCTION     25
#define STRID_COLUMNS               26
#define STRID_MENU_OPENSITE         27
#define STRID_MENU_COPYNAME         28
#define STRID_MENU_ITEMMOVEUP       29
#define STRID_MENU_ITEMMOVEDOWN     30
#define STRID_SCRIPT                31
#define STRID_HOTKEY                32
#define STRID_STATUS                33
#define STRID_VERSION               34
#define STRID_DESCRIPTION           35
#define STRID_AUTHOR                36
#define STRID_SITE                  37
#define STRID_RUNNING               38
#define STRID_WAITING               39
#define STRID_CONTAIN               40
#define STRID_EXEC                  41
#define STRID_EDIT                  42
#define STRID_ASSIGN                43
#define STRID_PLUGIN                44
#define STRID_OK                    45
#define STRID_CANCEL                46
#define STRID_CLOSE                 47

#define OF_RECT        0x01
#define OF_COLUMNS     0x02
#define OF_LASTSCRIPT  0x04
#define OF_HOTKEYS     0x08
#define OF_DEBUG       0x10

#define BUFFER_SIZE             1024

//List columns
#define LVI_SCRIPT        0
#define LVI_HOTKEY        1
#define LVI_STATUS        2
#define LVI_VERSION       3
#define LVI_DESCRIPTION   4
#define LVI_AUTHOR        5
#define LVI_SITE          6

//List columns flags
#define LCF_VISIBLE    0x1
#define LCF_CONTENT    0x2

//List columns name
#define LCN_SCRIPT      0x01
#define LCN_HOTKEY      0x02
#define LCN_STATUS      0x04
#define LCN_VERSION     0x08
#define LCN_DESCRIPTION 0x10
#define LCN_AUTHOR      0x20
#define LCN_SITE        0x40

#define JIT_NONE        0x0
#define JIT_DEBUG       0x1
#define JIT_FROMSTART   0x2

typedef struct {
  wchar_t *wpScript;
  wchar_t *wpArguments;
  int nArgumentsLen;
  HANDLE hInitMutex;
  int nExecType;
  PLUGINCALLSENDW *pcs;
} EXECSCRIPT;

typedef struct _SCRIPTARG {
  struct _SCRIPTARG *next;
  struct _SCRIPTARG *prev;
  wchar_t *wpArgument;
  int nArgumentLen;
  wchar_t *wpArgName;
  int nArgNameLen;
  wchar_t *wpArgValue;
  int nArgValueLen;
} SCRIPTARG;

typedef struct {
  SCRIPTARG *first;
  SCRIPTARG *last;
  int nElements;
} HARGSTACK;

typedef struct _SCRIPTTHREAD {
  struct _SCRIPTTHREAD *next;
  struct _SCRIPTTHREAD *prev;
  int nRefCount;
  void *lpStack;
  HANDLE hInitMutex;
  HANDLE hExecMutex;
  BOOL bSingleCopy;
  BOOL bLockSendMessage;
  BOOL bWaiting;
  BOOL bQuiting;
  int nExecType;
  PLUGINCALLSENDW *pcs;
  HANDLE hThread;
  DWORD dwThreadID;
  IActiveScript *objActiveScript;
  IActiveScriptParse *objActiveScriptParse;
  IProcessDebugManager *objProcessDebugManager;
  IDebugApplication *objDebugApplication;
  IDebugDocumentHelper *objDebugDocumentHelper;
  IDispatch *objThis;
  IRealActiveScriptSite MyActiveScriptSite;
  IRealActiveScriptSiteWindow MyActiveScriptSiteWindow;
  IRealActiveScriptSiteDebug MyActiveScriptSiteDebug;
  wchar_t wszScriptName[MAX_PATH];
  wchar_t wszScriptBaseName[MAX_PATH];
  wchar_t wszScriptFile[MAX_PATH];
  wchar_t wszScriptInclude[MAX_PATH];
  HARGSTACK hArgStack;
  wchar_t *wszArguments;
  int nArgumentsLen;
  const wchar_t *wpScriptText;
  INT_PTR nScriptTextLen;

  //IDocument
  CALLBACKSTACK hDialogCallbackStack;
  HWND hWndScriptsThreadDummy;
  DWORD dwMessageLoop;
  BOOL bBusy;

  INCLUDESTACK hIncludesStack;
  POINTERSTACK hPointersStack;
  SINKSTACK hSinkStack;
  DWORD dwDebug;
  DWORD dwDebugJIT;
  BOOL bInitDebugJIT;
  BOOL bStopped;

  HWND hWndPluginEdit;
} SCRIPTTHREAD;

typedef struct {
  SCRIPTTHREAD *first;
  SCRIPTTHREAD *last;
  int nElements;
} HTHREADSTACK;

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

typedef struct {
  const wchar_t *wpKey;
  INT_PTR nKeyLen;
  wchar_t **wppValue;
  INT_PTR *lpnValueLen;
} CONTENTKEY;

#ifndef INVALID_FILE_ATTRIBUTES
  #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif
#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
  #define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#endif
#ifndef LVS_EX_GRIDLINES
  #define LVS_EX_GRIDLINES 0x00000001
#endif
#ifndef LVS_EX_FULLROWSELECT
  #define LVS_EX_FULLROWSELECT 0x00000020
#endif
#ifndef SCRIPT_E_PROPAGATE
  #define SCRIPT_E_PROPAGATE 0x80020102L
#endif


//Global variables
extern char szBuffer[BUFFER_SIZE];
extern wchar_t wszBuffer[BUFFER_SIZE];
extern wchar_t wszPluginName[MAX_PATH];
extern wchar_t wszPluginTitle[MAX_PATH];
extern HINSTANCE hInstanceEXE;
extern HINSTANCE hInstanceDLL;
extern HWND hMainWnd;
extern HACCEL hGlobalAccel;
extern BOOL bOldWindows;
extern BOOL bOldWindows;
extern BOOL bAkelEdit;
extern int nMDI;
extern LANGID wLangModule;
extern HICON g_hPluginIcon;
extern BOOL g_MainMessageBox;
extern BOOL bInitCommon;
extern TRANSLATEPROC TranslateMessageProc;
extern char szScriptsDir[MAX_PATH];
extern wchar_t wszScriptsDir[MAX_PATH];
extern char szAkelPadDir[MAX_PATH];
extern wchar_t wszAkelPadDir[MAX_PATH];
extern wchar_t wszMutexMsgName[MAX_PATH];
extern wchar_t wszErrorMsg[BUFFER_SIZE];
extern HTHREADSTACK hThreadStack;
extern SCRIPTTHREAD *lpScriptThreadActiveX;
extern HANDLE hMainThread;
extern HANDLE hMutexMsgFirst;
extern int nMutexMsgCount;
extern BOOL bGlobalDebugJITEnable;

//Functions prototypes
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
BOOL CALLBACK ColumnsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int SetColumnItem(HWND hWnd, int nIndex, LISTCOLUMN *lpColumn, BOOL bCheck);
BOOL CALLBACK CodeDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL RegisterHotkey(wchar_t *wszScriptName, WORD wHotkey);
void CreateColumns(HWND hWnd, LISTCOLUMN *lpColumns);
LISTCOLUMN* GetColumnByID(int nID, int *lpnColumnIndex);
LISTCOLUMN* GetColumnByIndex(int nColumnIndex);
DWORD GetColumnFlags(LISTCOLUMN *lpColumns);
void FillScriptList(STACKLISTITEM *hStack, LISTCOLUMN *lpColumns, HWND hWnd, const wchar_t *wpFilter);
void StackFillListItem(STACKLISTITEM *hStack, LISTCOLUMN *lpColumns);
LISTITEM* StackInsertListItem(STACKLISTITEM *hStack);
void StackFreeListItem(STACKLISTITEM *hStack);
LISTITEM* GetItemParam(HWND hWnd, int nIndex);
int LangMatchRate(LANGID wCompareIt, LANGID wCompareWith);

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
BOOL CALLBACK HotkeyProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
int EditScript(wchar_t *wpScript);
void ExecScript(wchar_t *wpScript, wchar_t *wszArguments, int nExecType, PLUGINCALLSENDW *pcs);
DWORD WINAPI ExecThreadProc(LPVOID lpParameter);
SCRIPTTHREAD* StackInsertScriptThread(HTHREADSTACK *hStack);
SCRIPTTHREAD* StackGetScriptThreadCurrent();
SCRIPTTHREAD* StackGetScriptThreadByID(HTHREADSTACK *hStack, DWORD dwThreadID);
SCRIPTTHREAD* StackGetScriptThreadByName(HTHREADSTACK *hStack, const wchar_t *wpScriptName);
SCRIPTTHREAD* StackGetScriptThreadByPCS(HTHREADSTACK *hStack, PLUGINCALLSENDW *pcs);
BOOL StackDeleteScriptThread(SCRIPTTHREAD *lpScriptThread);
void FreeScriptResources(SCRIPTTHREAD *lpScriptThread, BOOL bDebug);
BOOL CloseScriptThread(SCRIPTTHREAD *lpScriptThread);
void CloseScriptThreadAll(int *nCloseOK, int *nCloseERR);
void PostQuitScriptAll();
void ShowScriptWindows(SCRIPTTHREAD *lpScriptThread, BOOL bShow);
void ShowScriptWindowsAll(BOOL bShow);
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg);
SCRIPTARG* StackInsertArgument(HARGSTACK *hStack);
SCRIPTARG* StackGetArgumentByIndex(HARGSTACK *hStack, int nIndex);
SCRIPTARG* StackGetArgumentByName(HARGSTACK *hStack, const wchar_t *wpArgName, int nArgNameLen);
void StackFreeArguments(HARGSTACK *hStack);
UINT_PTR GetVariantValue(VARIANT *pvtParameter, VARIANT **ppvtParameter, BOOL bAnsi);
UINT_PTR GetVariantInt(VARIANT *pvtParameter, VARIANT **ppvtParameter);
HRESULT SetVariantInt(VARIANT *pvtParameter, INT_PTR nValue);
int GetHotkeyString(WORD wHotkey, wchar_t *wszString);
INT_PTR DetectAndReadFile(HANDLE *lphFile, const wchar_t *wpFile, DWORD dwFlags, int *lpnCodePage, BOOL *lpbBOM, wchar_t **wpContent, UINT_PTR dwBytesMax);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, int nFileDirMax);
const char* GetFileNameAnsi(const char *pFile, int nFileLen);
const wchar_t* GetFileNameWide(const wchar_t *wpFile, int nFileLen);
int GetBaseName(const wchar_t *wpFile, int nFileLen, wchar_t *wszBaseName, int nBaseNameMax);
const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen);
int GetFileStreamOffset(const wchar_t *wpFile, int nFileLen);
DWORD ScrollCaret(HWND hWnd);
int SaveLineScroll(HWND hWnd);
void RestoreLineScroll(HWND hWnd, int nBeforeLine);
BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc);
INT_PTR CopyWideStr(const wchar_t *wpSrc, INT_PTR nSrcLen, wchar_t **wppDst);
BOOL FreeWideStr(wchar_t **wppWideStr);

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

#endif
