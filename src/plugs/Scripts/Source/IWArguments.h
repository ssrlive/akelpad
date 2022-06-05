#ifndef _IWARGUMENTS_H_
#define _IWARGUMENTS_H_

/*
// IWArguments's VTable
#undef INTERFACE
#define INTERFACE IWArguments
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, WArguments_QueryInterface)(THIS_ const IID *, void **) PURE;
  STDMETHOD_(ULONG, WArguments_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, WArguments_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, WArguments_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, WArguments_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, WArguments_GetIDsOfNames)(THIS_ const IID *, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, WArguments_Invoke)(THIS_ DISPID, const IID *, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  // IWArguments methods
  STDMETHOD_(HRESULT, WArguments_Item)(THIS_ int, BSTR *) PURE;
  STDMETHOD_(HRESULT, WArguments_Length)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, WArguments_Count)(THIS_ int *) PURE;
};
//*/

//Defines
typedef struct {
  IWArgumentsVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
} IRealWArguments;

//Global variables
extern ITypeInfo *g_WArgumentsTypeInfo;
extern const IWArgumentsVtbl MyIWArgumentsVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE WArguments_QueryInterface(IWArguments *This, const IID * vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE WArguments_AddRef(IWArguments *This);
ULONG STDMETHODCALLTYPE WArguments_Release(IWArguments *This);

HRESULT STDMETHODCALLTYPE WArguments_GetTypeInfoCount(IWArguments *This, UINT *pCount);
HRESULT STDMETHODCALLTYPE WArguments_GetTypeInfo(IWArguments *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE WArguments_GetIDsOfNames(IWArguments *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE WArguments_Invoke(IWArguments *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE WArguments_Item(IWArguments *This, int nIndex, BSTR *wpItem);
HRESULT STDMETHODCALLTYPE WArguments_Length(IWArguments *This, int *nItems);
HRESULT STDMETHODCALLTYPE WArguments_Count(IWArguments *This, int *nItems);

#endif
