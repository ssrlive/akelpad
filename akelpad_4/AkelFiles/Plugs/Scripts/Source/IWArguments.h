#ifndef _IWARGUMENTS_H_
#define _IWARGUMENTS_H_

// IWArguments's VTable
#undef INTERFACE
#define INTERFACE IWArguments
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, WArguments_QueryInterface)(THIS_ REFIID, void **) PURE;
  STDMETHOD_(ULONG, WArguments_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, WArguments_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, WArguments_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, WArguments_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, WArguments_GetIDsOfNames)(THIS_ REFIID, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, WArguments_Invoke)(THIS_ DISPID, REFIID, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  // IWArguments methods
  STDMETHOD_(HRESULT, WArguments_Item)(THIS_ int, BSTR *) PURE;
  STDMETHOD_(HRESULT, WArguments_Length)(THIS_ int *) PURE;
  STDMETHOD_(HRESULT, WArguments_Count)(THIS_ int *) PURE;
};

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
HRESULT STDMETHODCALLTYPE WArguments_QueryInterface(IWArguments *this, REFIID vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE WArguments_AddRef(IWArguments *this);
ULONG STDMETHODCALLTYPE WArguments_Release(IWArguments *this);

HRESULT STDMETHODCALLTYPE WArguments_GetTypeInfoCount(IWArguments *this, UINT *pCount);
HRESULT STDMETHODCALLTYPE WArguments_GetTypeInfo(IWArguments *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE WArguments_GetIDsOfNames(IWArguments *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE WArguments_Invoke(IWArguments *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE WArguments_Item(IWArguments *this, int nIndex, BSTR *wpItem);
HRESULT STDMETHODCALLTYPE WArguments_Length(IWArguments *this, int *nItems);
HRESULT STDMETHODCALLTYPE WArguments_Count(IWArguments *this, int *nItems);

#endif
