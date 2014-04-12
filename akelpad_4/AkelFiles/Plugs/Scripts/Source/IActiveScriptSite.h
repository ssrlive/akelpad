#ifndef _IACTIVESCRIPTSITE_H_
#define _IACTIVESCRIPTSITE_H_

#include <activscp.h>
#include <activdbg.h>

//Define MYDWORD_PTR to avoid compiler warnings
#ifdef _WIN64
  typedef UINT_PTR MYDWORD_PTR;
#else
  typedef DWORD MYDWORD_PTR;
#endif

//GUID
extern const struct _GUID IID_IActiveScriptSiteDebug;
extern const struct _GUID IID_IProcessDebugManager;
extern const struct _GUID CLSID_ProcessDebugManager;

//Defines
typedef struct {
  IActiveScriptSiteVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
} IRealActiveScriptSite;

typedef struct {
  IActiveScriptSiteWindowVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
} IRealActiveScriptSiteWindow;

typedef struct {
  #ifdef _WIN64
    IActiveScriptSiteDebug64Vtbl *lpVtbl;
  #else
    IActiveScriptSiteDebug32Vtbl *lpVtbl;
  #endif
  DWORD dwCount;
  void *lpScriptThread;
} IRealActiveScriptSiteDebug;


//Functions prototypes
HRESULT ExecScriptText(void *lpScriptThread, GUID *guidEngine);
HRESULT GetScriptEngineA(char *szExt, GUID *guidEngine);
int CBTMessageBox(HWND hWnd, const wchar_t *wpText, const wchar_t *wpCaption, UINT uType);
LRESULT CALLBACK CBTMessageBoxProc(INT nCode, WPARAM wParam, LPARAM lParam);

HRESULT STDMETHODCALLTYPE QueryInterface(IActiveScriptSite *, REFIID, void **);
ULONG STDMETHODCALLTYPE AddRef(IActiveScriptSite *);
ULONG STDMETHODCALLTYPE Release(IActiveScriptSite *);
HRESULT STDMETHODCALLTYPE GetLCID(IActiveScriptSite *, LCID *);
HRESULT STDMETHODCALLTYPE GetItemInfo(IActiveScriptSite *, LPCOLESTR, DWORD, IUnknown **, ITypeInfo **);
HRESULT STDMETHODCALLTYPE GetDocVersionString(IActiveScriptSite *, BSTR *);
HRESULT STDMETHODCALLTYPE OnScriptTerminate(IActiveScriptSite *, const VARIANT *, const EXCEPINFO *);
HRESULT STDMETHODCALLTYPE OnStateChange(IActiveScriptSite *, SCRIPTSTATE);
HRESULT STDMETHODCALLTYPE OnScriptError(IActiveScriptSite *, IActiveScriptError *);
HRESULT STDMETHODCALLTYPE OnEnterScript(IActiveScriptSite *);
HRESULT STDMETHODCALLTYPE OnLeaveScript(IActiveScriptSite *);

HRESULT STDMETHODCALLTYPE SiteWindow_QueryInterface(IActiveScriptSiteWindow *, REFIID, void **);
ULONG STDMETHODCALLTYPE SiteWindow_AddRef(IActiveScriptSiteWindow *);
ULONG STDMETHODCALLTYPE SiteWindow_Release(IActiveScriptSiteWindow *);
HRESULT STDMETHODCALLTYPE GetSiteWindow(IActiveScriptSiteWindow *, HWND *);
HRESULT STDMETHODCALLTYPE EnableModeless(IActiveScriptSiteWindow *, BOOL);

HRESULT STDMETHODCALLTYPE SiteDebug_QueryInterface(IActiveScriptSiteDebug *, REFIID, void **);
ULONG STDMETHODCALLTYPE SiteDebug_AddRef(IActiveScriptSiteDebug *);
ULONG STDMETHODCALLTYPE SiteDebug_Release(IActiveScriptSiteDebug *);
HRESULT STDMETHODCALLTYPE SiteDebug_GetDocumentContextFromPosition(IActiveScriptSiteDebug *, MYDWORD_PTR, ULONG, ULONG, IDebugDocumentContext **);
HRESULT STDMETHODCALLTYPE SiteDebug_GetApplication(IActiveScriptSiteDebug *, IDebugApplication **);
HRESULT STDMETHODCALLTYPE SiteDebug_GetRootApplicationNode(IActiveScriptSiteDebug *, IDebugApplicationNode **);
HRESULT STDMETHODCALLTYPE SiteDebug_OnScriptErrorDebug(IActiveScriptSiteDebug *, IActiveScriptErrorDebug *, BOOL *, BOOL *);

#endif
