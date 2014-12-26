#ifndef _ISCRIPTSETTINGS_H_
#define _ISCRIPTSETTINGS_H_

//ScriptSettings().Read
#define PO_BINARYSTRING 20
#define PO_BINARYSIZE   21

// IScriptSettings's VTable
#undef INTERFACE
#define INTERFACE IScriptSettings
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, ScriptSettings_QueryInterface)(THIS_ REFIID, void **) PURE;
  STDMETHOD_(ULONG, ScriptSettings_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, ScriptSettings_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, ScriptSettings_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, ScriptSettings_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, ScriptSettings_GetIDsOfNames)(THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, ScriptSettings_Invoke)(THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  // IScriptSettings methods
  STDMETHOD_(HRESULT, ScriptSettings_Begin)(THIS_ BSTR, DWORD, VARIANT *) PURE;
  STDMETHOD_(HRESULT, ScriptSettings_Read)(THIS_ VARIANT, DWORD, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, ScriptSettings_Write)(THIS_ BSTR, DWORD, VARIANT, int, int *) PURE;
  STDMETHOD_(HRESULT, ScriptSettings_Delete)(THIS_ BSTR, BOOL *) PURE;
  STDMETHOD_(HRESULT, ScriptSettings_End)(THIS_ BOOL *) PURE;
};

//Defines
typedef struct {
  IScriptSettingsVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
  HANDLE hOptions;
} IRealScriptSettings;

//Global variables
extern ITypeInfo *g_ScriptSettingsTypeInfo;
extern const IScriptSettingsVtbl MyIScriptSettingsVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE ScriptSettings_QueryInterface(IScriptSettings *this, REFIID vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE ScriptSettings_AddRef(IScriptSettings *this);
ULONG STDMETHODCALLTYPE ScriptSettings_Release(IScriptSettings *this);

HRESULT STDMETHODCALLTYPE ScriptSettings_GetTypeInfoCount(IScriptSettings *this, UINT *pCount);
HRESULT STDMETHODCALLTYPE ScriptSettings_GetTypeInfo(IScriptSettings *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE ScriptSettings_GetIDsOfNames(IScriptSettings *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE ScriptSettings_Invoke(IScriptSettings *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE ScriptSettings_Begin(IScriptSettings *this, BSTR wpScriptName, DWORD dwFlags, VARIANT *vtOptions);
HRESULT STDMETHODCALLTYPE ScriptSettings_Read(IScriptSettings *this, VARIANT vtOptionName, DWORD dwType, VARIANT vtDefault, VARIANT *vtData);
HRESULT STDMETHODCALLTYPE ScriptSettings_Write(IScriptSettings *this, BSTR wpOptionName, DWORD dwType, VARIANT vtData, int nDataSize, int *nResult);
HRESULT STDMETHODCALLTYPE ScriptSettings_Delete(IScriptSettings *this, BSTR wpOptionName, BOOL *bResult);
HRESULT STDMETHODCALLTYPE ScriptSettings_End(IScriptSettings *this, BOOL *bResult);

#endif
