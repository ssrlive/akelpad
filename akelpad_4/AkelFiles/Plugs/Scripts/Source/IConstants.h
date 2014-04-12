#ifndef _ICONSTANTS_H_
#define _ICONSTANTS_H_

// IConstants's VTable
#undef INTERFACE
#define INTERFACE IConstants
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, Constants_QueryInterface)(THIS_ REFIID, void **) PURE;
  STDMETHOD_(ULONG, Constants_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Constants_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, Constants_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, Constants_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, Constants_GetIDsOfNames)(THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, Constants_Invoke)(THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  STDMETHOD_(HRESULT, Constants_TCHAR)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, Constants_vbTCHAR)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, Constants_TSTR)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_vbTSTR)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_TSIZE)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_vbTSIZE)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_X64)(THIS_ BOOL *) PURE;
  STDMETHOD_(HRESULT, Constants_vbX64)(THIS_ BOOL *) PURE;
};

//Defines
typedef struct {
  IConstantsVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
} IRealConstants;

//Global variables
extern ITypeInfo *g_ConstantsTypeInfo;
extern const IConstantsVtbl MyIConstantsVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE Constants_QueryInterface(IConstants *this, REFIID vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE Constants_AddRef(IConstants *this);
ULONG STDMETHODCALLTYPE Constants_Release(IConstants *this);

HRESULT STDMETHODCALLTYPE Constants_GetTypeInfoCount(IConstants *this, UINT *pCount);
HRESULT STDMETHODCALLTYPE Constants_GetTypeInfo(IConstants *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE Constants_GetIDsOfNames(IConstants *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE Constants_Invoke(IConstants *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE Constants_TCHAR(IConstants *this, BSTR *wpChar);
HRESULT STDMETHODCALLTYPE Constants_vbTCHAR(IConstants *this, BSTR *wpChar);
HRESULT STDMETHODCALLTYPE Constants_TSTR(IConstants *this, int *nStr);
HRESULT STDMETHODCALLTYPE Constants_vbTSTR(IConstants *this, int *nStr);
HRESULT STDMETHODCALLTYPE Constants_TSIZE(IConstants *this, int *nSize);
HRESULT STDMETHODCALLTYPE Constants_vbTSIZE(IConstants *this, int *nSize);
HRESULT STDMETHODCALLTYPE Constants_X64(IConstants *this, BOOL *b64);
HRESULT STDMETHODCALLTYPE Constants_vbX64(IConstants *this, BOOL *b64);

#endif
