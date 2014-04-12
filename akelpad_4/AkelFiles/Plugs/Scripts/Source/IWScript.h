#ifndef _IWSCRIPT_H_
#define _IWSCRIPT_H_

// IWScript's VTable
#undef INTERFACE
#define INTERFACE IWScript
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, WScript_QueryInterface)(THIS_ REFIID, void **) PURE;
  STDMETHOD_(ULONG, WScript_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, WScript_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, WScript_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, WScript_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, WScript_GetIDsOfNames)(THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, WScript_Invoke)(THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  // IWScript methods
  STDMETHOD_(HRESULT, WScript_Arguments)(THIS_ IDispatch **) PURE;
  STDMETHOD_(HRESULT, WScript_ScriptFullName)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, WScript_ScriptName)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, WScript_ScriptBaseName)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, WScript_FullName)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, WScript_Path)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, WScript_Name)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, WScript_Echo)(THIS_ BSTR) PURE;
  STDMETHOD_(HRESULT, WScript_Sleep)(THIS_ int) PURE;
  STDMETHOD_(HRESULT, WScript_Quit)(THIS_ int) PURE;
};

//Defines
typedef struct {
  IWScriptVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
} IRealWScript;


//Global variables
extern ITypeInfo *g_WScriptTypeInfo;
extern const IWScriptVtbl MyIWScriptVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE WScript_QueryInterface(IWScript *this, REFIID vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE WScript_AddRef(IWScript *this);
ULONG STDMETHODCALLTYPE WScript_Release(IWScript *this);

HRESULT STDMETHODCALLTYPE WScript_GetTypeInfoCount(IWScript *this, UINT *pCount);
HRESULT STDMETHODCALLTYPE WScript_GetTypeInfo(IWScript *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE WScript_GetIDsOfNames(IWScript *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE WScript_Invoke(IWScript *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE WScript_Arguments(IWScript *this, IDispatch **objWArguments);
HRESULT STDMETHODCALLTYPE WScript_ScriptFullName(IWScript *this, BSTR *wpScriptFullName);
HRESULT STDMETHODCALLTYPE WScript_ScriptName(IWScript *this, BSTR *wpScriptName);
HRESULT STDMETHODCALLTYPE WScript_ScriptBaseName(IWScript *this, BSTR *wpScriptBaseName);
HRESULT STDMETHODCALLTYPE WScript_FullName(IWScript *this, BSTR *wpDllFullName);
HRESULT STDMETHODCALLTYPE WScript_Path(IWScript *this, BSTR *wpDllPath);
HRESULT STDMETHODCALLTYPE WScript_Name(IWScript *this, BSTR *wpDllName);
HRESULT STDMETHODCALLTYPE WScript_Echo(IWScript *this, BSTR wpText);
HRESULT STDMETHODCALLTYPE WScript_Sleep(IWScript *this, int nTime);
HRESULT STDMETHODCALLTYPE WScript_Quit(IWScript *this, int nErrorCode);

#endif
