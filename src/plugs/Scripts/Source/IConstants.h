#ifndef _ICONSTANTS_H_
#define _ICONSTANTS_H_

/*
// IConstants's VTable
#undef INTERFACE
#define INTERFACE IConstants
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, Constants_QueryInterface)(THIS_ const IID *, void **) PURE;
  STDMETHOD_(ULONG, Constants_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Constants_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, Constants_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, Constants_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, Constants_GetIDsOfNames)(THIS_ const IID *, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, Constants_Invoke)(THIS_ DISPID, const IID *, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  STDMETHOD_(HRESULT, Constants_TCHAR)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, Constants_vbTCHAR)(THIS_ BSTR *) PURE;
  STDMETHOD_(HRESULT, Constants_TSTR)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_vbTSTR)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_TSIZE)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_vbTSIZE)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, Constants_X64)(THIS_ BOOL *) PURE;
  STDMETHOD_(HRESULT, Constants_vbX64)(THIS_ BOOL *) PURE;
};
//*/

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
HRESULT STDMETHODCALLTYPE Constants_QueryInterface(IConstants *This, const IID * vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE Constants_AddRef(IConstants *This);
ULONG STDMETHODCALLTYPE Constants_Release(IConstants *This);

HRESULT STDMETHODCALLTYPE Constants_GetTypeInfoCount(IConstants *This, UINT *pCount);
HRESULT STDMETHODCALLTYPE Constants_GetTypeInfo(IConstants *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE Constants_GetIDsOfNames(IConstants *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE Constants_Invoke(IConstants *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE Constants_TCHAR(IConstants *This, BSTR *wpChar);
HRESULT STDMETHODCALLTYPE Constants_vbTCHAR(IConstants *This, BSTR *wpChar);
HRESULT STDMETHODCALLTYPE Constants_TSTR(IConstants *This, int *nStr);
HRESULT STDMETHODCALLTYPE Constants_vbTSTR(IConstants *This, int *nStr);
HRESULT STDMETHODCALLTYPE Constants_TSIZE(IConstants *This, int *nSize);
HRESULT STDMETHODCALLTYPE Constants_vbTSIZE(IConstants *This, int *nSize);
HRESULT STDMETHODCALLTYPE Constants_X64(IConstants *This, BOOL *b64);
HRESULT STDMETHODCALLTYPE Constants_vbX64(IConstants *This, BOOL *b64);

#endif
