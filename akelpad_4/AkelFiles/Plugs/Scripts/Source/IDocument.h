#ifndef _IDOCUMENT_H_
#define _IDOCUMENT_H_

// IDocument's VTable
#undef INTERFACE
#define INTERFACE IDocument
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, Document_QueryInterface)(THIS_ REFIID, void **) PURE;
  STDMETHOD_(ULONG, Document_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Document_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, Document_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, Document_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, Document_GetIDsOfNames)(THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, Document_Invoke)(THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  // IDocument methods
  STDMETHOD_(HRESULT, Document_Constants)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_GetMainWnd)(THIS_ HWND *) PURE;
  STDMETHOD_(HRESULT, Document_GetAkelDir)(THIS_ int, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetInstanceExe)(THIS_ INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetInstanceDll)(THIS_ INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetLangId)(THIS_ int, int *) PURE;
  STDMETHOD_(HRESULT, Document_IsOldWindows)(THIS_ BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_IsOldRichEdit)(THIS_ BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_IsOldComctl32)(THIS_ BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_IsAkelEdit)(THIS_ HWND, int *) PURE;
  STDMETHOD_(HRESULT, Document_IsMDI)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditWnd)(THIS_ HWND *) PURE;
  STDMETHOD_(HRESULT, Document_SetEditWnd)(THIS_ HWND, HWND *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditDoc)(THIS_ INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditFile)(THIS_ HWND, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditCodePage)(THIS_ HWND, int *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditBOM)(THIS_ HWND, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditNewLine)(THIS_ HWND, int *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditModified)(THIS_ HWND, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditReadOnly)(THIS_ HWND, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_SetFrameInfo)(THIS_ FRAMEDATA *, int, UINT_PTR, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_SendMessage)(THIS_ HWND, UINT, VARIANT, VARIANT, INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_MessageBox)(THIS_ HWND, BSTR, BSTR, UINT, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_InputBox)(THIS_ HWND, BSTR, BSTR, BSTR, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetSelStart)(THIS_ INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetSelEnd)(THIS_ INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_SetSel)(THIS_ INT_PTR, INT_PTR, DWORD, DWORD) PURE;
  STDMETHOD_(HRESULT, Document_GetSelText)(THIS_ int, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetTextRange)(THIS_ INT_PTR, INT_PTR, int, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_ReplaceSel)(THIS_ BSTR, BOOL) PURE;
  STDMETHOD_(HRESULT, Document_TextFind)(THIS_ HWND, BSTR, DWORD, INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_TextReplace)(THIS_ HWND, BSTR, BSTR, DWORD, BOOL, INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetClipboardText)(THIS_ BOOL, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_SetClipboardText)(THIS_ BSTR) PURE;
  STDMETHOD_(HRESULT, Document_IsPluginRunning)(THIS_ BSTR, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_Call)(THIS_ BSTR, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_CallA)(THIS_ BSTR, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_CallW)(THIS_ BSTR, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_CallEx)(THIS_ DWORD, BSTR, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_CallExA)(THIS_ DWORD, BSTR, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_CallExW)(THIS_ DWORD, BSTR, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_Exec)(THIS_ BSTR, BSTR, BOOL, DWORD *) PURE;
  STDMETHOD_(HRESULT, Document_Command)(THIS_ int, LPARAM lParam, INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_Font)(THIS_ BSTR, DWORD, int) PURE;
  STDMETHOD_(HRESULT, Document_Recode)(THIS_ int, int) PURE;
  STDMETHOD_(HRESULT, Document_Include)(THIS_ BSTR, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_IsInclude)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_OpenFile)(THIS_ BSTR, DWORD, int, BOOL, int *) PURE;
  STDMETHOD_(HRESULT, Document_ReadFile)(THIS_ BSTR, DWORD, int, BOOL, INT_PTR, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_WriteFile)(THIS_ VARIANT, BSTR, INT_PTR, int, BOOL, DWORD, int *) PURE;
  STDMETHOD_(HRESULT, Document_SaveFile)(THIS_ HWND, BSTR, int, BOOL, DWORD, int *) PURE;
  STDMETHOD_(HRESULT, Document_ScriptSettings)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_SystemFunction)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_MemAlloc)(THIS_ DWORD, INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_MemCopy)(THIS_ INT_PTR, VARIANT, DWORD, int, int *) PURE;
  STDMETHOD_(HRESULT, Document_MemRead)(THIS_ INT_PTR, DWORD, int, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_MemStrPtr)(THIS_ BSTR, INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_MemFree)(THIS_ INT_PTR) PURE;
  STDMETHOD_(HRESULT, Document_DebugJIT)(THIS) PURE;
  STDMETHOD_(HRESULT, Document_Debug)(THIS_ DWORD, DWORD *) PURE;
  STDMETHOD_(HRESULT, Document_VarType)(THIS_ VARIANT, int *) PURE;
  STDMETHOD_(HRESULT, Document_GetArgLine)(THIS_ BOOL, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetArgValue)(THIS_ BSTR, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_WindowRegisterClass)(THIS_ BSTR, WORD *) PURE;
  STDMETHOD_(HRESULT, Document_WindowUnregisterClass)(THIS_ BSTR, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_WindowRegisterDialog)(THIS_ HWND, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_WindowUnregisterDialog)(THIS_ HWND, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_WindowGetMessage)(THIS_ DWORD) PURE;
  STDMETHOD_(HRESULT, Document_WindowSubClass)(THIS_ HWND, IDispatch *, SAFEARRAY **, INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_WindowNextProc)(THIS_ INT_PTR *, HWND, UINT, WPARAM, LPARAM, LRESULT *) PURE;
  STDMETHOD_(HRESULT, Document_WindowNoNextProc)(THIS_ INT_PTR *) PURE;
  STDMETHOD_(HRESULT, Document_WindowUnsubClass)(THIS_ HWND) PURE;
  STDMETHOD_(HRESULT, Document_ThreadHook)(THIS_ int, IDispatch *, DWORD, HHOOK *) PURE;
  STDMETHOD_(HRESULT, Document_ThreadUnhook)(THIS_ HHOOK, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_ScriptNoMutex)(THIS_ DWORD, DWORD *) PURE;
  STDMETHOD_(HRESULT, Document_ScriptHandle)(THIS_ VARIANT, int, VARIANT *) PURE;
};

//Defines
#define DT_ANSI       0
#define DT_UNICODE    1
#define DT_QWORD      2
#define DT_DWORD      3
#define DT_WORD       4
#define DT_BYTE       5

#define FS_NONE            0
#define FS_FONTNORMAL      1
#define FS_FONTBOLD        2
#define FS_FONTITALIC      3
#define FS_FONTBOLDITALIC  4

//Document_GetAkelDir type
#define ADTYPE_ROOT        0
#define ADTYPE_AKELFILES   1
#define ADTYPE_DOCS        2
#define ADTYPE_LANGS       3
#define ADTYPE_PLUGS       4
#define ADTYPE_SCRIPTS     5
#define ADTYPE_INCLUDE     6

//Document_IsAkelEdit type
#define ISAEW_NONE         0
#define ISAEW_PROGRAM      1
#define ISAEW_PLUGIN       2

//Document_GetLangId type
#define LANGID_FULL        0
#define LANGID_PRIMARY     1
#define LANGID_SUB         2

//Document_WriteFile flags
#define WFF_WRITEREADONLY  0x1
#define WFF_APPENDFILE     0x2

//Document_WindowSubClass window handle
#define WSC_MAINPROC     1
#define WSC_EDITPROC     2
#define WSC_FRAMEPROC    3

//Document_WindowGetMessage flags
#define WGM_ENABLE    0x1
#define WGM_NOKEYSEND 0x2

//Document_ScriptNoMutex type
#define ULT_UNLOCKSCRIPTSQUEUE   0x1
#define ULT_UNLOCKPROGRAMTHREAD  0x2
#define ULT_LOCKMULTICOPY        0x4
#define ULT_UNLOCKMULTICOPY      0x8

//Document_ScriptHandle type
#define SH_FIRSTSCRIPT           1
#define SH_THISSCRIPT            2
#define SH_FINDSCRIPT            3
#define SH_GETTHREADHANDLE       11
#define SH_GETTHREADID           12
#define SH_GETMESSAGELOOP        13
#define SH_GETLOCKMULTICOPY      14
#define SH_GETLOCKSCRIPTSQUEUE   15
#define SH_GETLOCKPROGRAMTHREAD  16
#define SH_GETSERVICEWINDOW      17
#define SH_GETNAME               21
#define SH_GETFILE               22
#define SH_GETNCLUDE             23
#define SH_GETARGUMENTS          24
#define SH_NEXTSCRIPT            31
#define SH_NEXTSAMESCRIPT        32
#define SH_CLOSESCRIPT           33

//Callback type
#define CIT_DIALOG       1
#define CIT_SUBCLASS     2
#define CIT_MAINPROC     3
#define CIT_EDITPROC     4
#define CIT_FRAMEPROC    5
#define CIT_HOOKCALLBACK 6
#define CIT_SYSCALLBACK  7

//Debug types
#define DBG_MEMREAD              0x01
#define DBG_MEMWRITE             0x02
#define DBG_MEMFREE              0x04
#define DBG_MEMLEAK              0x08
#define DBG_SYSCALL              0x10

#define AKDLL_SUBCLASSSEND  (WM_USER + 1)
#define AKDLL_HOOKSEND      (WM_USER + 2)
#define AKDLL_CALLBACKSEND  (WM_USER + 3)
#define AKDLL_POSTQUIT      (WM_USER + 4)

typedef struct {
  IDocumentVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
} IRealDocument;

typedef struct _INCLUDEITEM {
  struct _INCLUDEITEM *next;
  struct _INCLUDEITEM *prev;
  wchar_t wszInclude[MAX_PATH];
} INCLUDEITEM;

typedef struct {
  INCLUDEITEM *first;
  INCLUDEITEM *last;
  int nElements;
} INCLUDESTACK;

typedef struct _POINTERITEM {
  struct _POINTERITEM *next;
  struct _POINTERITEM *prev;
  void *lpData;
  UINT_PTR dwSize;
} POINTERITEM;

typedef struct {
  POINTERITEM *first;
  POINTERITEM *last;
  int nElements;
} POINTERSTACK;

typedef struct {
  INT_PTR lpProc;
  BOOL bBusy;
} CALLBACKBUSYNESS;

typedef struct _MSGINT {
  struct _MSGINT *next;
  struct _MSGINT *prev;
  UINT uMsg;
} MSGINT;

typedef struct {
  MSGINT *first;
  MSGINT *last;
  int nElements;
} MSGINTSTACK;

typedef struct _CALLBACKITEM {
  struct _CALLBACKITEM *next;
  struct _CALLBACKITEM *prev;
  int nRefCount;
  void *lpStack;
  int nBusyIndex;
  INT_PTR lpProc;         //SYSCALLBACK, HOOKPROC.
  HANDLE hHandle;         //HWND, HHOOK.
  IDispatch *objFunction; //Script function.
  UINT_PTR dwData;        //WNDPROC, nArgCount or nRetAddr.
  int nCallbackType;      //See CIT_* defines.
  void *lpScriptThread;
  MSGINTSTACK hMsgIntStack;
  BOOL bNoNextProc;
  BOOL bShow;
} CALLBACKITEM;

typedef struct {
  CALLBACKITEM *first;
  CALLBACKITEM *last;
  int nElements;
} CALLBACKSTACK;

typedef struct {
  CALLBACKITEM *lpCallback;
  HWND hWnd;
  UINT uMsg;
  WPARAM wParam;
  LPARAM lParam;
} MSGSEND;

typedef struct {
  BSTR wpCaption;
  BSTR wpLabel;
  BSTR wpEdit;
  VARIANT *vtResult;
  HRESULT hr;
} INPUTBOX;

//Global variables
extern CALLBACKSTACK g_hSubclassCallbackStack;
extern CALLBACKSTACK g_hHookCallbackStack;
extern ITypeInfo *g_DocumentTypeInfo;
extern const IDocumentVtbl MyIDocumentVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE Document_QueryInterface(IDocument *this, REFIID vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE Document_AddRef(IDocument *this);
ULONG STDMETHODCALLTYPE Document_Release(IDocument *this);

HRESULT STDMETHODCALLTYPE Document_GetTypeInfoCount(IDocument *this, UINT *pCount);
HRESULT STDMETHODCALLTYPE Document_GetTypeInfo(IDocument *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE Document_GetIDsOfNames(IDocument *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE Document_Invoke(IDocument *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE Document_Constants(IDocument *this, IDispatch **objConstants);
HRESULT STDMETHODCALLTYPE Document_GetMainWnd(IDocument *this, HWND *hWnd);
HRESULT STDMETHODCALLTYPE Document_GetEditWnd(IDocument *this, HWND *hWnd);
HRESULT STDMETHODCALLTYPE Document_SetEditWnd(IDocument *this, HWND hWnd, HWND *hWndResult);
HRESULT STDMETHODCALLTYPE Document_GetEditDoc(IDocument *this, INT_PTR *hDoc);
HRESULT STDMETHODCALLTYPE Document_GetAkelDir(IDocument *this, int nSubDir, BSTR *wpDir);
HRESULT STDMETHODCALLTYPE Document_GetInstanceExe(IDocument *this, INT_PTR *hInstance);
HRESULT STDMETHODCALLTYPE Document_GetInstanceDll(IDocument *this, INT_PTR *hInstance);
HRESULT STDMETHODCALLTYPE Document_GetLangId(IDocument *this, int nType, int *nLangModule);
HRESULT STDMETHODCALLTYPE Document_IsOldWindows(IDocument *this, BOOL *bIsOld);
HRESULT STDMETHODCALLTYPE Document_IsOldRichEdit(IDocument *this, BOOL *bIsOld);
HRESULT STDMETHODCALLTYPE Document_IsOldComctl32(IDocument *this, BOOL *bIsOld);
HRESULT STDMETHODCALLTYPE Document_IsAkelEdit(IDocument *this, HWND hWnd, int *nIsAkelEdit);
HRESULT STDMETHODCALLTYPE Document_IsMDI(IDocument *this, int *nIsMDI);
HRESULT STDMETHODCALLTYPE Document_GetEditFile(IDocument *this, HWND hWnd, BSTR *wpFile);
HRESULT STDMETHODCALLTYPE Document_GetEditCodePage(IDocument *this, HWND hWnd, int *nCodePage);
HRESULT STDMETHODCALLTYPE Document_GetEditBOM(IDocument *this, HWND hWnd, BOOL *bBOM);
HRESULT STDMETHODCALLTYPE Document_GetEditNewLine(IDocument *this, HWND hWnd, int *nNewLine);
HRESULT STDMETHODCALLTYPE Document_GetEditModified(IDocument *this, HWND hWnd, BOOL *bModified);
HRESULT STDMETHODCALLTYPE Document_GetEditReadOnly(IDocument *this, HWND hWnd, BOOL *bReadOnly);
HRESULT STDMETHODCALLTYPE Document_SetFrameInfo(IDocument *this, FRAMEDATA *lpFrame, int nType, UINT_PTR dwData, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_SendMessage(IDocument *this, HWND hWnd, UINT uMsg, VARIANT vtwParam, VARIANT vtlParam, INT_PTR *nResult);
HRESULT STDMETHODCALLTYPE Document_MessageBox(IDocument *this, HWND hWnd, BSTR pText, BSTR pCaption, UINT uType, SAFEARRAY **psa, int *nResult);
BUTTONMESSAGEBOX* FillButtonsArray(SAFEARRAY *psa, HICON *hIcon);
HRESULT STDMETHODCALLTYPE Document_InputBox(IDocument *this, HWND hWnd, BSTR wpCaption, BSTR wpLabel, BSTR wpEdit, VARIANT *vtResult);
BOOL CALLBACK InputBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT STDMETHODCALLTYPE Document_GetSelStart(IDocument *this, INT_PTR *nSelStart);
HRESULT STDMETHODCALLTYPE Document_GetSelEnd(IDocument *this, INT_PTR *nSelEnd);
HRESULT STDMETHODCALLTYPE Document_SetSel(IDocument *this, INT_PTR nSelStart, INT_PTR nSelEnd, DWORD dwFlags, DWORD dwType);
HRESULT STDMETHODCALLTYPE Document_GetSelText(IDocument *this, int nNewLine, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_GetTextRange(IDocument *this, INT_PTR nRangeStart, INT_PTR nRangeEnd, int nNewLine, BSTR *wpText);
HRESULT GetTextRange(HWND hWnd, INT_PTR nRangeStart, INT_PTR nRangeEnd, int nNewLine, BOOL bColumnSel, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_ReplaceSel(IDocument *this, BSTR wpText, BOOL bSelect);
HRESULT STDMETHODCALLTYPE Document_TextFind(IDocument *this, HWND hWnd, BSTR wpFindIt, DWORD dwFlags, INT_PTR *nResult);
HRESULT STDMETHODCALLTYPE Document_TextReplace(IDocument *this, HWND hWnd, BSTR wpFindIt, BSTR wpReplaceWith, DWORD dwFlags, BOOL bAll, INT_PTR *nResult);
HRESULT STDMETHODCALLTYPE Document_GetClipboardText(IDocument *this, BOOL bAnsi, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_SetClipboardText(IDocument *this, BSTR wpText);
HRESULT STDMETHODCALLTYPE Document_IsPluginRunning(IDocument *this, BSTR wpFunction, BOOL *bRunning);
HRESULT STDMETHODCALLTYPE Document_Call(IDocument *this, BSTR wpFunction, SAFEARRAY **psa, int *nResult);
HRESULT STDMETHODCALLTYPE Document_CallA(IDocument *this, BSTR wpFunction, SAFEARRAY **psa, int *nResult);
HRESULT STDMETHODCALLTYPE Document_CallW(IDocument *this, BSTR wpFunction, SAFEARRAY **psa, int *nResult);
HRESULT STDMETHODCALLTYPE Document_CallEx(IDocument *this, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, int *nResult);
HRESULT STDMETHODCALLTYPE Document_CallExA(IDocument *this, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, int *nResult);
HRESULT STDMETHODCALLTYPE Document_CallExW(IDocument *this, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, int *nResult);
HRESULT CallPlugin(DWORD dwFlags, DWORD dwSupport, BSTR wpFunction, SAFEARRAY **psa, int *nResult);
HRESULT STDMETHODCALLTYPE Document_Exec(IDocument *this, BSTR wpCmdLine, BSTR wpWorkDir, int nWait, DWORD *dwExit);
HRESULT STDMETHODCALLTYPE Document_Command(IDocument *this, int nCommand, LPARAM lParam, INT_PTR *nResult);
HRESULT STDMETHODCALLTYPE Document_Font(IDocument *this, BSTR wpFaceName, DWORD dwFontStyle, int nPointSize);
HRESULT STDMETHODCALLTYPE Document_Recode(IDocument *this, int nCodePageFrom, int nCodePageTo);
HRESULT STDMETHODCALLTYPE Document_Include(IDocument *this, BSTR wpFileName, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_IsInclude(IDocument *this, BSTR *wpFileName);
HRESULT STDMETHODCALLTYPE Document_OpenFile(IDocument *this, BSTR wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, int *nResult);
HRESULT STDMETHODCALLTYPE Document_ReadFile(IDocument *this, BSTR wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, INT_PTR nBytesMax, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_WriteFile(IDocument *this, VARIANT vtFile, BSTR wpContent, INT_PTR nContentLen, int nCodePage, BOOL bBOM, DWORD dwFlags, int *nResult);
HRESULT STDMETHODCALLTYPE Document_SaveFile(IDocument *this, HWND hWnd, BSTR wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags, int *nResult);
HRESULT STDMETHODCALLTYPE Document_ScriptSettings(IDocument *this, IDispatch **objSet);
HRESULT STDMETHODCALLTYPE Document_SystemFunction(IDocument *this, IDispatch **objSys);
HRESULT STDMETHODCALLTYPE Document_MemAlloc(IDocument *this, DWORD dwSize, INT_PTR *nPointer);
HRESULT STDMETHODCALLTYPE Document_MemCopy(IDocument *this, INT_PTR nPointer, VARIANT vtData, DWORD dwType, int nDataLen, int *nBytes);
HRESULT STDMETHODCALLTYPE Document_MemRead(IDocument *this, INT_PTR nPointer, DWORD dwType, int nDataLen, VARIANT *vtData);
HRESULT STDMETHODCALLTYPE Document_MemStrPtr(IDocument *this, BSTR wpString, INT_PTR *nPointer);
HRESULT STDMETHODCALLTYPE Document_MemFree(IDocument *this, INT_PTR nPointer);
HRESULT STDMETHODCALLTYPE Document_DebugJIT(IDocument *this);
HRESULT STDMETHODCALLTYPE Document_Debug(IDocument *this, DWORD dwDebug, DWORD *dwResult);
HRESULT STDMETHODCALLTYPE Document_VarType(IDocument *this, VARIANT vtData, int *nType);
HRESULT STDMETHODCALLTYPE Document_GetArgLine(IDocument *this, BOOL bNoEncloseQuotes, BSTR *wpArgLine);
HRESULT STDMETHODCALLTYPE Document_GetArgValue(IDocument *this, BSTR wpArgName, VARIANT vtDefault, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_WindowRegisterClass(IDocument *this, BSTR wpClassName, WORD *wAtom);
HRESULT STDMETHODCALLTYPE Document_WindowUnregisterClass(IDocument *this, BSTR wpClassName, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_WindowRegisterDialog(IDocument *this, HWND hDlg, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_WindowUnregisterDialog(IDocument *this, HWND hDlg, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_WindowGetMessage(IDocument *this, DWORD dwFlags);
HRESULT STDMETHODCALLTYPE Document_WindowSubClass(IDocument *this, HWND hWnd, IDispatch *objFunction, SAFEARRAY **psa, INT_PTR *lpCallbackItem);
HRESULT STDMETHODCALLTYPE Document_WindowNextProc(IDocument *this, INT_PTR *lpCallbackItem, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
HRESULT STDMETHODCALLTYPE Document_WindowNoNextProc(IDocument *this, INT_PTR *lpCallbackItem);
HRESULT STDMETHODCALLTYPE Document_WindowUnsubClass(IDocument *this, HWND hWnd);
HRESULT WindowUnsubClass(void *lpScriptThread, HWND hWnd);
HRESULT STDMETHODCALLTYPE Document_ThreadHook(IDocument *this, int idHook, IDispatch *objCallback, DWORD dwThreadId, HHOOK *hHook);
HRESULT STDMETHODCALLTYPE Document_ThreadUnhook(IDocument *this, HHOOK hHook, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_ScriptNoMutex(IDocument *this, DWORD dwUnlockType, DWORD *dwResult);
HRESULT STDMETHODCALLTYPE Document_ScriptHandle(IDocument *this, VARIANT vtData, int nOperation, VARIANT *vtResult);
HWND GetCurEdit(IDocument *this);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);
INCLUDEITEM* StackInsertInclude(INCLUDESTACK *hStack);
INCLUDEITEM* StackGetInclude(INCLUDESTACK *hStack, DWORD dwIndex);
void StackFreeIncludes(INCLUDESTACK *hStack);
POINTERITEM* StackInsertPointer(POINTERSTACK *hStack);
POINTERITEM* StackGetPointer(POINTERSTACK *hStack, void *lpData, INT_PTR nRange);
void StackDeletePointer(POINTERSTACK *hStack, POINTERITEM *lpPointer);
void StackFreePointers(POINTERSTACK *hStack);
MSGINT* StackInsertMessage(MSGINTSTACK *hStack, UINT uMsg);
MSGINT* StackGetMessage(MSGINTSTACK *hStack, UINT uMsg);
void StackDeleteMessage(MSGINTSTACK *hStack, MSGINT *lpMessage);
void StackFreeMessages(MSGINTSTACK *hStack);
void StackFillMessages(MSGINTSTACK *hStack, SAFEARRAY *psa);
int RetriveCallbackProc(CALLBACKBUSYNESS *cb);
CALLBACKITEM* StackInsertCallback(CALLBACKSTACK *hStack);
int StackGetCallbackCount(CALLBACKSTACK *hStack, int nCallbackType);
CALLBACKITEM* StackGetCallbackByHandle(CALLBACKSTACK *hStack, HANDLE hHandle, void *lpScriptThread);
CALLBACKITEM* StackGetCallbackByProc(CALLBACKSTACK *hStack, INT_PTR lpProc);
CALLBACKITEM* StackGetCallbackByObject(CALLBACKSTACK *hStack, IDispatch *objFunction);
BOOL StackDeleteCallback(CALLBACKITEM *lpElement);
void StackFreeCallbacks(CALLBACKSTACK *hStack);
LRESULT CALLBACK DialogCallbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SubclassCallbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SubclassCallbackCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK SubclassMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SubclassMainCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK SubclassEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SubclassEditCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK SubclassFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SubclassFrameCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK SubclassSend(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback1Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback2Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback3Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback4Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback5Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback6Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback7Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback8Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback9Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback10Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback11Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback12Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback13Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback14Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback15Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback16Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback17Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback18Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback19Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback20Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback21Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback22Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback23Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback24Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback25Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback26Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback27Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback28Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback29Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallback30Proc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookCallbackCommonProc(int nCallbackIndex, int nCode, WPARAM wParam, LPARAM lParam);
HWND CreateScriptsThreadDummyWindow();
LRESULT CALLBACK ScriptsThreadProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT CallScriptProc(IDispatch *objFunction, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);

#endif

