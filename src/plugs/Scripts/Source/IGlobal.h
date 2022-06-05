#ifndef _IGLOBAL_H_
#define _IGLOBAL_H_

/*
// IGlobal's VTable
#undef INTERFACE
#define INTERFACE IGlobal
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, Global_QueryInterface)(THIS_ const IID *, void **) PURE;
  STDMETHOD_(ULONG, Global_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, Global_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, Global_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, Global_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, Global_GetIDsOfNames)(THIS_ const IID *, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, Global_Invoke)(THIS_ DISPID, const IID *, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  STDMETHOD_(HRESULT, Global_PtrAdd)(THIS_ VARIANT, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Global_vbPtrAdd)(THIS_ VARIANT, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Global_PtrMath)(THIS_ VARIANT, BSTR, VARIANT, VARIANT *) PURE;
  STDMETHOD_(HRESULT, Global_vbPtrMath)(THIS_ VARIANT, BSTR, VARIANT, VARIANT *) PURE;
};
//*/

//Defines
typedef struct {
  IGlobalVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
} IRealGlobal;

//Global variables
extern ITypeInfo *g_GlobalTypeInfo;
extern const IGlobalVtbl MyIGlobalVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE Global_QueryInterface(IGlobal *This, const IID * vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE Global_AddRef(IGlobal *This);
ULONG STDMETHODCALLTYPE Global_Release(IGlobal *This);

HRESULT STDMETHODCALLTYPE Global_GetTypeInfoCount(IGlobal *This, UINT *pCount);
HRESULT STDMETHODCALLTYPE Global_GetTypeInfo(IGlobal *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE Global_GetIDsOfNames(IGlobal *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE Global_Invoke(IGlobal *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE Global_PtrAdd(IGlobal *This, VARIANT vtPointer1, VARIANT vtPointer2, VARIANT *vtPointerResult);
HRESULT STDMETHODCALLTYPE Global_vbPtrAdd(IGlobal *This, VARIANT vtPointer1, VARIANT vtPointer2, VARIANT *vtPointerResult);
HRESULT STDMETHODCALLTYPE Global_PtrMath(IGlobal *This, VARIANT vtPointer1, BSTR wpSign, VARIANT vtPointer2, VARIANT *vtPointerResult);
HRESULT STDMETHODCALLTYPE Global_vbPtrMath(IGlobal *This, VARIANT vtPointer1, BSTR wpSign, VARIANT vtPointer2, VARIANT *vtPointerResult);

#endif
