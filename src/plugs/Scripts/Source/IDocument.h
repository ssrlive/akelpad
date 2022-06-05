#ifndef _IDOCUMENT_H_
#define _IDOCUMENT_H_

/*
// IDocument's VTable
#undef INTERFACE
#define INTERFACE IDocument
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, Document_QueryInterface)(THIS_ const IID *, void **) PURE;
  STDMETHOD_(ULONG, Document_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Document_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, Document_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, Document_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, Document_GetIDsOfNames)(THIS_ const IID *, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, Document_Invoke)(THIS_ DISPID, const IID *, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  // IDocument methods
  STDMETHOD_(HRESULT, Document_Constants)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_Global)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_GetMainWnd)(THIS_ VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetAkelDir)(THIS_ int, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetInstanceExe)(THIS_ VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetInstanceDll)(THIS_ VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetLangId)(THIS_ int, int *) PURE;
  STDMETHOD_(HRESULT, Document_IsOldWindows)(THIS_ BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_IsAkelEdit)(THIS_ VARIANT, int *) PURE;
  STDMETHOD_(HRESULT, Document_IsMDI)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditWnd)(THIS_ VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_SetEditWnd)(THIS_ VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditDoc)(THIS_ VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditFile)(THIS_ VARIANT, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetFilePath)(THIS_ BSTR, int, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditCodePage)(THIS_ VARIANT, int *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditBOM)(THIS_ VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditNewLine)(THIS_ VARIANT, int *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditModified)(THIS_ VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_GetEditReadOnly)(THIS_ VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_SetFrameInfo)(THIS_ VARIANT, int, VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_SendMessage)(THIS_ VARIANT, UINT, VARIANT, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_MessageBox)(THIS_ VARIANT, BSTR, BSTR, UINT, SAFEARRAY **, int *) PURE;
  STDMETHOD_(HRESULT, Document_InputBox)(THIS_ VARIANT, BSTR, BSTR, BSTR, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetSelStart)(THIS_ VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetSelEnd)(THIS_ VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_SetSel)(THIS_ VARIANT, VARIANT, DWORD, DWORD ) PURE;
  STDMETHOD_(HRESULT, Document_GetSelText)(THIS_ int, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetTextRange)(THIS_ VARIANT, VARIANT, int, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_ReplaceSel)(THIS_ BSTR, int ) PURE;
  STDMETHOD_(HRESULT, Document_TextFind)(THIS_ VARIANT, BSTR, DWORD, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_TextReplace)(THIS_ VARIANT, BSTR, BSTR, DWORD, DWORD, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_GetClipboardText)(THIS_ BOOL, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_SetClipboardText)(THIS_ BSTR ) PURE;
  STDMETHOD_(HRESULT, Document_IsPluginRunning)(THIS_ BSTR, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_Call)(THIS_ BSTR, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_CallA)(THIS_ BSTR, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_CallW)(THIS_ BSTR, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_CallEx)(THIS_ DWORD, BSTR, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_CallExA)(THIS_ DWORD, BSTR, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_CallExW)(THIS_ DWORD, BSTR, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_Exec)(THIS_ BSTR, BSTR, int, int, DWORD *) PURE;
  STDMETHOD_(HRESULT, Document_Command)(THIS_ int, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_Font)(THIS_ BSTR, DWORD, int ) PURE;
  STDMETHOD_(HRESULT, Document_Recode)(THIS_ int, int ) PURE;
  STDMETHOD_(HRESULT, Document_Include)(THIS_ BSTR, DWORD, int, BOOL, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_IsInclude)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_OpenFile)(THIS_ BSTR, DWORD, int, BOOL, int *) PURE;
  STDMETHOD_(HRESULT, Document_ReadFile)(THIS_ BSTR, DWORD, int, BOOL, VARIANT, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_WriteFile)(THIS_ VARIANT, BSTR, VARIANT, int, BOOL, DWORD, int *) PURE;
  STDMETHOD_(HRESULT, Document_SaveFile)(THIS_ VARIANT, BSTR, int, BOOL, DWORD, VARIANT, int *) PURE;
  STDMETHOD_(HRESULT, Document_ScriptSettings)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_SystemFunction)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_MemAlloc)(THIS_ VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_MemCopy)(THIS_ VARIANT, VARIANT, DWORD, int, int *) PURE;
  STDMETHOD_(HRESULT, Document_MemRead)(THIS_ VARIANT, DWORD, int, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_MemStrPtr)(THIS_ BSTR, BOOL, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_MemPtrStr)(THIS_ VARIANT, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_MemFree)(THIS_ VARIANT ) PURE;
  STDMETHOD_(HRESULT, Document_DebugJIT)(THIS) PURE;
  STDMETHOD_(HRESULT, Document_Debug)(THIS_ DWORD, DWORD *) PURE;
  STDMETHOD_(HRESULT, Document_VarType)(THIS_ VARIANT, int *) PURE;
  STDMETHOD_(HRESULT, Document_VarDispatch)(THIS_ VARIANT, IDispatch **) PURE;
  STDMETHOD_(HRESULT, Document_GetArgLine)(THIS_ BOOL, BSTR *) PURE;
  STDMETHOD_(HRESULT, Document_GetArgValue)(THIS_ BSTR, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_CreateDialog)(THIS_ DWORD, VARIANT, VARIANT, DWORD, int, int, int, int, VARIANT, VARIANT, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_WindowRegisterClass)(THIS_ BSTR, SAFEARRAY **, WORD *) PURE;
  STDMETHOD_(HRESULT, Document_WindowUnregisterClass)(THIS_ BSTR, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_WindowRegisterDialog)(THIS_ VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_WindowUnregisterDialog)(THIS_ VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_WindowGetMessage)(THIS_ DWORD ) PURE;
  STDMETHOD_(HRESULT, Document_WindowSubClass)(THIS_ VARIANT, IDispatch *, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_WindowNextProc)(THIS_ VARIANT, VARIANT, UINT uMsg, VARIANT, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_WindowNoNextProc)(THIS_ VARIANT ) PURE;
  STDMETHOD_(HRESULT, Document_WindowUnsubClass)(THIS_ VARIANT ) PURE;
  STDMETHOD_(HRESULT, Document_ThreadHook)(THIS_ int, IDispatch *, DWORD, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_ThreadUnhook)(THIS_ VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_ScriptNoMutex)(THIS_ DWORD, DWORD *) PURE;
  STDMETHOD_(HRESULT, Document_ScriptExitCode)(THIS_ VARIANT, BOOL *) PURE;
  STDMETHOD_(HRESULT, Document_ScriptHandle)(THIS_ VARIANT, int, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Document_ConnectObject)(THIS_ IDispatch *, BSTR, VARIANT, int *) PURE;
  STDMETHOD_(HRESULT, Document_DisconnectObject)(THIS_ IDispatch *) PURE;
  STDMETHOD_(HRESULT, Document_ActiveXThis)(THIS_ IDispatch *) PURE;
};
//*/

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

//Document_SetEditWnd window handle
#define SEW_FOCUS          1

//Document_GetLangId type
#define LANGID_FULL        0
#define LANGID_PRIMARY     1
#define LANGID_SUB         2

//Document_WriteFile flags
#define WFF_WRITEREADONLY  0x1
#define WFF_APPENDFILE     0x2

//Document_ReplaceSel selection type
#define RST_NONE                 0
#define RST_SELECT               -1
#define RST_SELECTRESTORESCROLL  -2

//Document_CreateDialog flags
#define CDF_MODAL  0x1
#define CDF_PIXELS 0x2

//Document_WindowSubClass window handle
#define WSC_MAINPROC     1
#define WSC_EDITPROC     2
#define WSC_FRAMEPROC    3

//Document_WindowGetMessage flags
#define WGM_ENABLE    0x1
#define WGM_NOKEYSEND 0x2
#define WGM_KEYDOWNUP 0x4

//Document_ScriptNoMutex type
#define ULT_UNLOCKSCRIPTSQUEUE   0x01
#define ULT_UNLOCKPROGRAMTHREAD  0x02
#define ULT_LOCKMULTICOPY        0x04
#define ULT_UNLOCKMULTICOPY      0x08
#define ULT_LOCKSENDMESSAGE      0x10
#define ULT_UNLOCKSENDMESSAGE    0x20

//Document_ScriptHandle type
#define SH_FIRSTSCRIPT           1
#define SH_THISSCRIPT            2
#define SH_FINDSCRIPT            3
#define SH_GETEXECTYPE           10
#define SH_GETTHREADHANDLE       11
#define SH_GETTHREADID           12
#define SH_GETMESSAGELOOP        13
#define SH_GETLOCKMULTICOPY      14
#define SH_GETLOCKSCRIPTSQUEUE   15
#define SH_GETLOCKPROGRAMTHREAD  16
#define SH_GETSERVICEWINDOW      17
#define SH_GETLOCKSENDMESSAGE    18
#define SH_GETBASENAME           20
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

#ifndef DS_SHELLFONT
  #define DS_SHELLFONT (DS_SETFONT|DS_FIXEDSYS)
#endif
#ifndef AlignValue
  #define AlignValue(v, a)  (((UINT_PTR)v) + (a - ((UINT_PTR)v) % a) % a)
#endif

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
  UINT_PTR dwData;        //Depend on nCallbackType.
  int nCallbackType;      //See CIT_* defines.
  void *lpScriptThread;
  MSGINTSTACK hMsgIntStack;
  wchar_t *wpClassName;
  BOOL bDlgProc;
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
  HANDLE hMutex;
  BOOL bSignaled;
  LRESULT lResult;
} MSGMUTEX;

typedef struct {
  BSTR wpCaption;
  BSTR wpLabel;
  BSTR wpEdit;
  VARIANT *vtResult;
  HRESULT hr;
} INPUTBOX;

typedef struct {
  WORD dlgVer;
  WORD signature;
  DWORD helpID;
  DWORD exStyle;
  DWORD style;
  WORD cDlgItems;
  short x;
  short y;
  short cx;
  short cy;
} DLGTEMPLATEEX;

typedef struct {
  DWORD helpID;
  DWORD exStyle;
  DWORD style;
  short x;
  short y;
  short cx;
  short cy;
  DWORD id;
} DLGITEMTEMPLATEEX;

//Global variables
extern CALLBACKSTACK g_hSubclassCallbackStack;
extern CALLBACKSTACK g_hHookCallbackStack;
extern ITypeInfo *g_DocumentTypeInfo;
extern BOOL g_bScriptArg;
extern const IDocumentVtbl MyIDocumentVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE Document_QueryInterface(IDocument *This, const IID * vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE Document_AddRef(IDocument *This);
ULONG STDMETHODCALLTYPE Document_Release(IDocument *This);

HRESULT STDMETHODCALLTYPE Document_GetTypeInfoCount(IDocument *This, UINT *pCount);
HRESULT STDMETHODCALLTYPE Document_GetTypeInfo(IDocument *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE Document_GetIDsOfNames(IDocument *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE Document_Invoke(IDocument *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE Document_Constants(IDocument *This, IDispatch **objConstants);
HRESULT STDMETHODCALLTYPE Document_Global(IDocument *This, IDispatch **objGlobal);
HRESULT STDMETHODCALLTYPE Document_GetMainWnd(IDocument *This, VARIANT *vtWnd);
HRESULT STDMETHODCALLTYPE Document_GetAkelDir(IDocument *This, int nSubDir, BSTR *wpDir);
HRESULT STDMETHODCALLTYPE Document_GetInstanceExe(IDocument *This, VARIANT *vtInstance);
HRESULT STDMETHODCALLTYPE Document_GetInstanceDll(IDocument *This, VARIANT *vtInstance);
HRESULT STDMETHODCALLTYPE Document_GetLangId(IDocument *This, int nType, int *nLangModule);
HRESULT STDMETHODCALLTYPE Document_IsOldWindows(IDocument *This, BOOL *bIsOld);
HRESULT STDMETHODCALLTYPE Document_IsAkelEdit(IDocument *This, VARIANT vtWnd, int *nIsAkelEdit);
HRESULT STDMETHODCALLTYPE Document_IsMDI(IDocument *This, int *nIsMDI);
HRESULT STDMETHODCALLTYPE Document_GetEditWnd(IDocument *This, VARIANT *vtWnd);
HRESULT STDMETHODCALLTYPE Document_SetEditWnd(IDocument *This, VARIANT vtWnd, VARIANT *vtWndResult);
HRESULT STDMETHODCALLTYPE Document_GetEditDoc(IDocument *This, VARIANT *vtDoc);
HRESULT STDMETHODCALLTYPE Document_GetEditFile(IDocument *This, VARIANT vtWnd, BSTR *wpFile);
HRESULT STDMETHODCALLTYPE Document_GetFilePath(IDocument *This, BSTR wpFile, int nPart, BSTR *wpPath);
HRESULT STDMETHODCALLTYPE Document_GetEditCodePage(IDocument *This, VARIANT vtWnd, int *nCodePage);
HRESULT STDMETHODCALLTYPE Document_GetEditBOM(IDocument *This, VARIANT vtWnd, BOOL *bBOM);
HRESULT STDMETHODCALLTYPE Document_GetEditNewLine(IDocument *This, VARIANT vtWnd, int *nNewLine);
HRESULT STDMETHODCALLTYPE Document_GetEditModified(IDocument *This, VARIANT vtWnd, BOOL *bModified);
HRESULT STDMETHODCALLTYPE Document_GetEditReadOnly(IDocument *This, VARIANT vtWnd, BOOL *bReadOnly);
HRESULT STDMETHODCALLTYPE Document_SetFrameInfo(IDocument *This, VARIANT vtFrame, int nType, VARIANT vtData, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_SendMessage(IDocument *This, VARIANT vtWnd, UINT uMsg, VARIANT vtWParam, VARIANT vtLParam, VARIANT *vtResult);
void CALLBACK SendMessageAsyncProc(HWND hWnd, UINT uMsg, UINT_PTR dwData, LRESULT lResult);
HRESULT STDMETHODCALLTYPE Document_MessageBox(IDocument *This, VARIANT vtWnd, BSTR pText, BSTR pCaption, UINT uType, SAFEARRAY **psa, int *nResult);
BUTTONMESSAGEBOX* FillButtonsArray(SAFEARRAY *psa, HICON *hIcon);
HRESULT STDMETHODCALLTYPE Document_InputBox(IDocument *This, VARIANT vtWnd, BSTR wpCaption, BSTR wpLabel, BSTR wpEdit, VARIANT *vtResult);
BOOL CALLBACK InputBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT STDMETHODCALLTYPE Document_GetSelStart(IDocument *This, VARIANT *vtSelStart);
HRESULT STDMETHODCALLTYPE Document_GetSelEnd(IDocument *This, VARIANT *vtSelEnd);
HRESULT STDMETHODCALLTYPE Document_SetSel(IDocument *This, VARIANT vtSelStart, VARIANT vtSelEnd, DWORD dwFlags, DWORD dwType);
HRESULT STDMETHODCALLTYPE Document_GetSelText(IDocument *This, int nNewLine, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_GetTextRange(IDocument *This, VARIANT vtRangeStart, VARIANT vtRangeEnd, int nNewLine, BSTR *wpText);
HRESULT GetTextRange(HWND hWnd, INT_PTR nRangeStart, INT_PTR nRangeEnd, int nNewLine, BOOL bColumnSel, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_ReplaceSel(IDocument *This, BSTR wpText, int nSelect);
HRESULT STDMETHODCALLTYPE Document_TextFind(IDocument *This, VARIANT vtWnd, BSTR wpFindIt, DWORD dwFlags, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_TextReplace(IDocument *This, VARIANT vtWnd, BSTR wpFindIt, BSTR wpReplaceWith, DWORD dwFindFlags, DWORD dwReplaceFlags, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_GetClipboardText(IDocument *This, BOOL bAnsi, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_SetClipboardText(IDocument *This, BSTR wpText);
HRESULT STDMETHODCALLTYPE Document_IsPluginRunning(IDocument *This, BSTR wpFunction, BOOL *bRunning);
HRESULT STDMETHODCALLTYPE Document_Call(IDocument *This, BSTR wpFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_CallA(IDocument *This, BSTR wpFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_CallW(IDocument *This, BSTR wpFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_CallEx(IDocument *This, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_CallExA(IDocument *This, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_CallExW(IDocument *This, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT CallPlugin(DWORD dwFlags, DWORD dwSupport, BSTR wpFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_Exec(IDocument *This, BSTR wpCmdLine, BSTR wpWorkDir, int nWait, int nShowWindow, DWORD *dwExit);
HRESULT STDMETHODCALLTYPE Document_Command(IDocument *This, int nCommand, VARIANT vtLParam, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_Font(IDocument *This, BSTR wpFaceName, DWORD dwFontStyle, int nPointSize);
HRESULT STDMETHODCALLTYPE Document_Recode(IDocument *This, int nCodePageFrom, int nCodePageTo);
HRESULT STDMETHODCALLTYPE Document_Include(IDocument *This, BSTR wpFileName, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_IsInclude(IDocument *This, BSTR *wpFileName);
HRESULT STDMETHODCALLTYPE Document_OpenFile(IDocument *This, BSTR wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, int *nResult);
HRESULT STDMETHODCALLTYPE Document_ReadFile(IDocument *This, BSTR wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, VARIANT vtBytesMax, BSTR *wpText);
HRESULT STDMETHODCALLTYPE Document_WriteFile(IDocument *This, VARIANT vtFile, BSTR wpContent, VARIANT vtContentLen, int nCodePage, BOOL bBOM, DWORD dwFlags, int *nResult);
HRESULT STDMETHODCALLTYPE Document_SaveFile(IDocument *This, VARIANT vtWnd, BSTR wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags, VARIANT vtDoc, int *nResult);
HRESULT STDMETHODCALLTYPE Document_ScriptSettings(IDocument *This, IDispatch **objSet);
HRESULT STDMETHODCALLTYPE Document_SystemFunction(IDocument *This, IDispatch **objSys);
HRESULT STDMETHODCALLTYPE Document_MemAlloc(IDocument *This, VARIANT vtSize, VARIANT *vtPointer);
HRESULT STDMETHODCALLTYPE Document_MemCopy(IDocument *This, VARIANT vtPointer, VARIANT vtData, DWORD dwType, int nDataLen, int *nBytes);
HRESULT STDMETHODCALLTYPE Document_MemRead(IDocument *This, VARIANT vtPointer, DWORD dwType, int nDataLen, VARIANT *vtData);
HRESULT STDMETHODCALLTYPE Document_MemStrPtr(IDocument *This, BSTR wpString, BOOL bConvert, VARIANT *vtPointer);
HRESULT STDMETHODCALLTYPE Document_MemPtrStr(IDocument *This, VARIANT vtPointer, BSTR *wpString);
HRESULT STDMETHODCALLTYPE Document_MemFree(IDocument *This, VARIANT vtPointer);
HRESULT STDMETHODCALLTYPE Document_DebugJIT(IDocument *This);
HRESULT STDMETHODCALLTYPE Document_Debug(IDocument *This, DWORD dwDebug, DWORD *dwResult);
HRESULT STDMETHODCALLTYPE Document_VarType(IDocument *This, VARIANT vtData, int *nType);
HRESULT STDMETHODCALLTYPE Document_VarDispatch(IDocument *This, VARIANT vtPointer, IDispatch **objDispatch);
HRESULT STDMETHODCALLTYPE Document_GetArgLine(IDocument *This, BOOL bNoEncloseQuotes, BSTR *wpArgLine);
HRESULT STDMETHODCALLTYPE Document_GetArgValue(IDocument *This, BSTR wpArgName, VARIANT vtDefault, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_CreateDialog(IDocument *This, DWORD dwExStyle, VARIANT vtClassName, VARIANT vtTitle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, VARIANT vtWndParent, VARIANT vtParam, SAFEARRAY **lpItems, VARIANT *vtWnd);
HRESULT FillDialogTemplate(DLGTEMPLATEEX *lpdt, DWORD dwFlags, DWORD dwExStyle, wchar_t *wpClassName, wchar_t *wpTitle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HMENU hMenu, BSTR wpFaceName, DWORD dwFontStyle, int nPointSize, SAFEARRAY **lpItems, DWORD dwElement, DWORD *lpdwSize);
BOOL GetDialogUnits(HDC hDC, HFONT hFont, POINT *ptUnitCur, POINT *ptUnit96);
int PixelToUnitX(int x, const POINT *ptUnitCur, const POINT *ptUnit96);
int PixelToUnitY(int y, const POINT *ptUnitCur, const POINT *ptUnit96);
LOGFONTW* LogFontAtoW(const LOGFONTA *lfA, LOGFONTW *lfW);
LOGFONTA* LogFontWtoA(const LOGFONTW *lfW, LOGFONTA *lfA);
HRESULT STDMETHODCALLTYPE Document_WindowRegisterClass(IDocument *This, BSTR wpClassName, SAFEARRAY **psa, WORD *wAtom);
HRESULT STDMETHODCALLTYPE Document_WindowUnregisterClass(IDocument *This, BSTR wpClassName, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_WindowRegisterDialog(IDocument *This, VARIANT vtDlg, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_WindowUnregisterDialog(IDocument *This, VARIANT vtDlg, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_WindowGetMessage(IDocument *This, DWORD dwFlags);
HRESULT STDMETHODCALLTYPE Document_WindowSubClass(IDocument *This, VARIANT vtWnd, IDispatch *objFunction, SAFEARRAY **psa, VARIANT *vtCallbackItem);
HRESULT STDMETHODCALLTYPE Document_WindowNextProc(IDocument *This, VARIANT vtCallbackItem, VARIANT vtWnd, UINT uMsg, VARIANT vtWParam, VARIANT vtLParam, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_WindowNoNextProc(IDocument *This, VARIANT vtCallbackItem);
HRESULT STDMETHODCALLTYPE Document_WindowUnsubClass(IDocument *This, VARIANT vtWnd);
HRESULT WindowUnsubClass(void *lpScriptThread, HWND hWnd);
HRESULT STDMETHODCALLTYPE Document_ThreadHook(IDocument *This, int idHook, IDispatch *objCallback, DWORD dwThreadId, SAFEARRAY **psa, VARIANT *vtHook);
HRESULT STDMETHODCALLTYPE Document_ThreadUnhook(IDocument *This, VARIANT vtHook, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_ScriptNoMutex(IDocument *This, DWORD dwUnlockType, DWORD *dwResult);
HRESULT STDMETHODCALLTYPE Document_ScriptExitCode(IDocument *This, VARIANT vtExitCode, BOOL *bResult);
HRESULT STDMETHODCALLTYPE Document_ScriptHandle(IDocument *This, VARIANT vtData, int nOperation, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE Document_ConnectObject(IDocument *This, IDispatch *objConnectTo, BSTR wpPrefix, VARIANT vtIID, int *nCount);
HRESULT STDMETHODCALLTYPE Document_DisconnectObject(IDocument *This, IDispatch *objConnectTo);
HRESULT STDMETHODCALLTYPE Document_ActiveXThis(IDocument *This, IDispatch *objConnectTo);

HWND GetCurEdit(IDocument *This);
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
CALLBACKITEM* StackInsertCallback(CALLBACKSTACK *hStack, IDispatch *objCallback);
int StackGetCallbackCount(CALLBACKSTACK *hStack, int nCallbackType);
BOOL StackIsCallback(CALLBACKSTACK *hStack, CALLBACKITEM *lpCallback);
CALLBACKITEM* StackGetCallbackByHandle(CALLBACKSTACK *hStack, HANDLE hHandle, void *lpScriptThread);
CALLBACKITEM* StackGetCallbackByProc(CALLBACKSTACK *hStack, INT_PTR lpProc);
CALLBACKITEM* StackGetCallbackByObject(CALLBACKSTACK *hStack, IDispatch *objFunction);
CALLBACKITEM* StackGetCallbackByData(CALLBACKSTACK *hStack, UINT_PTR dwData);
CALLBACKITEM* StackGetCallbackByClass(CALLBACKSTACK *hStack, const wchar_t *wpClassName);
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

