#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Scripts.h"


//Global variables
ITypeInfo *g_WArgumentsTypeInfo=NULL;

const IWArgumentsVtbl MyIWArgumentsVtbl={
  WArguments_QueryInterface,
  WArguments_AddRef,
  WArguments_Release,
  WArguments_GetTypeInfoCount,
  WArguments_GetTypeInfo,
  WArguments_GetIDsOfNames,
  WArguments_Invoke,
  WArguments_Item,
  WArguments_Length,
  WArguments_Count
};


//// IWArguments

HRESULT STDMETHODCALLTYPE WArguments_QueryInterface(IWArguments *This, const IID * vTableGuid, void **ppv)
{
  if (!ppv) return E_POINTER;

  if (AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=This;
    This->lpVtbl->AddRef(This);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE WArguments_AddRef(IWArguments *This)
{
  return ++((IRealWArguments *)This)->dwCount;
}

ULONG STDMETHODCALLTYPE WArguments_Release(IWArguments *This)
{
  if (--((IRealWArguments *)This)->dwCount == 0)
  {
    GlobalFree(This);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealWArguments *)This)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE WArguments_GetTypeInfoCount(IWArguments *This, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE WArguments_GetTypeInfo(IWArguments *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  HRESULT hr;

  *pTypeInfo=NULL;

  if (itinfo)
  {
    hr=ResultFromScode(DISP_E_BADINDEX);
  }
  else if (g_WArgumentsTypeInfo)
  {
    g_WArgumentsTypeInfo->lpVtbl->AddRef(g_WArgumentsTypeInfo);
    hr=S_OK;
  }
  else
  {
    hr=LoadTypeInfoFromFile(NULL, NULL);
  }
  if (hr == S_OK) *pTypeInfo=g_WArgumentsTypeInfo;

  return hr;
}

HRESULT STDMETHODCALLTYPE WArguments_GetIDsOfNames(IWArguments *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_WArgumentsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_WArgumentsTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE WArguments_Invoke(IWArguments *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_WArgumentsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(This, g_WArgumentsTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}


//// IWArguments methods

HRESULT STDMETHODCALLTYPE WArguments_Item(IWArguments *This, int nIndex, BSTR *wpItem)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWArguments *)This)->lpScriptThread;
  SCRIPTARG *lpScriptArg;
  HRESULT hr=NOERROR;

  if (nIndex >= lpScriptThread->hArgStack.nElements)
    return 0x800A0009L;

  if (lpScriptArg=StackGetArgumentByIndex(&lpScriptThread->hArgStack, nIndex))
  {
    if (!(*wpItem=SysAllocString(lpScriptArg->wpArgument)))
      hr=E_OUTOFMEMORY;
  }
  else *wpItem=NULL;

  return hr;
}

HRESULT STDMETHODCALLTYPE WArguments_Length(IWArguments *This, int *nItems)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWArguments *)This)->lpScriptThread;

  *nItems=lpScriptThread->hArgStack.nElements;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE WArguments_Count(IWArguments *This, int *nItems)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWArguments *)This)->lpScriptThread;

  *nItems=lpScriptThread->hArgStack.nElements;
  return NOERROR;
}
