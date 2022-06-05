#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Scripts.h"


//Global variables
ITypeInfo *g_GlobalTypeInfo=NULL;

const IGlobalVtbl MyIGlobalVtbl={
  Global_QueryInterface,
  Global_AddRef,
  Global_Release,
  Global_GetTypeInfoCount,
  Global_GetTypeInfo,
  Global_GetIDsOfNames,
  Global_Invoke,
  Global_PtrAdd,
  Global_vbPtrAdd,
  Global_PtrMath,
  Global_vbPtrMath
};


//// IGlobal

HRESULT STDMETHODCALLTYPE Global_QueryInterface(IGlobal *This, const IID * vTableGuid, void **ppv)
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

ULONG STDMETHODCALLTYPE Global_AddRef(IGlobal *This)
{
  return ++((IRealGlobal *)This)->dwCount;
}

ULONG STDMETHODCALLTYPE Global_Release(IGlobal *This)
{
  if (--((IRealGlobal *)This)->dwCount == 0)
  {
    GlobalFree(This);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealGlobal *)This)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE Global_GetTypeInfoCount(IGlobal *This, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Global_GetTypeInfo(IGlobal *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  HRESULT hr;

  *pTypeInfo=NULL;

  if (itinfo)
  {
    hr=ResultFromScode(DISP_E_BADINDEX);
  }
  else if (g_GlobalTypeInfo)
  {
    g_GlobalTypeInfo->lpVtbl->AddRef(g_GlobalTypeInfo);
    hr=S_OK;
  }
  else
  {
    hr=LoadTypeInfoFromFile(NULL, NULL);
  }
  if (hr == S_OK) *pTypeInfo=g_GlobalTypeInfo;

  return hr;
}

HRESULT STDMETHODCALLTYPE Global_GetIDsOfNames(IGlobal *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_GlobalTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_GlobalTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE Global_Invoke(IGlobal *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_GlobalTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(This, g_GlobalTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}

HRESULT STDMETHODCALLTYPE Global_PtrAdd(IGlobal *This, VARIANT vtPointer1, VARIANT vtPointer2, VARIANT *vtPointerResult)
{
  INT_PTR nPointer1=GetVariantInt(&vtPointer1, NULL);
  INT_PTR nPointer2=GetVariantInt(&vtPointer2, NULL);

  SetVariantInt(vtPointerResult, nPointer1 + nPointer2);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Global_vbPtrAdd(IGlobal *This, VARIANT vtPointer1, VARIANT vtPointer2, VARIANT *vtPointerResult)
{
  return Global_PtrAdd(This, vtPointer1, vtPointer2, vtPointerResult);
}

HRESULT STDMETHODCALLTYPE Global_PtrMath(IGlobal *This, VARIANT vtPointer1, BSTR wpSign, VARIANT vtPointer2, VARIANT *vtPointerResult)
{
  INT_PTR nPointer1=GetVariantInt(&vtPointer1, NULL);
  INT_PTR nPointer2=GetVariantInt(&vtPointer2, NULL);
  INT_PTR nPointerResult=0;

  if (!xstrcmpW(wpSign, L"+"))
    nPointerResult=nPointer1 + nPointer2;
  else if (!xstrcmpW(wpSign, L"-"))
    nPointerResult=nPointer1 - nPointer2;
  else if (!xstrcmpW(wpSign, L"*"))
    nPointerResult=nPointer1 * nPointer2;
  else if (!xstrcmpW(wpSign, L"/"))
    nPointerResult=nPointer1 / nPointer2;
  else if (!xstrcmpW(wpSign, L"%"))
    nPointerResult=nPointer1 % nPointer2;
  else if (!xstrcmpW(wpSign, L"&"))
    nPointerResult=nPointer1 & nPointer2;
  else if (!xstrcmpW(wpSign, L"|"))
    nPointerResult=nPointer1 | nPointer2;
  else if (!xstrcmpW(wpSign, L"^"))
    nPointerResult=nPointer1 ^ nPointer2;
  else if (!xstrcmpW(wpSign, L">"))
    nPointerResult=nPointer1 > nPointer2;
  else if (!xstrcmpW(wpSign, L"<"))
    nPointerResult=nPointer1 < nPointer2;
  else if (!xstrcmpW(wpSign, L"^"))
    nPointerResult=nPointer1 ^ nPointer2;
  else if (!xstrcmpW(wpSign, L"=="))
    nPointerResult=nPointer1 == nPointer2;
  else if (!xstrcmpW(wpSign, L"!="))
    nPointerResult=nPointer1 != nPointer2;
  else if (!xstrcmpW(wpSign, L">="))
    nPointerResult=nPointer1 >= nPointer2;
  else if (!xstrcmpW(wpSign, L"<="))
    nPointerResult=nPointer1 <= nPointer2;
  else if (!xstrcmpW(wpSign, L"<<"))
    nPointerResult=nPointer1 << nPointer2;
  else if (!xstrcmpW(wpSign, L">>"))
    nPointerResult=nPointer1 >> nPointer2;

  SetVariantInt(vtPointerResult, nPointerResult);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Global_vbPtrMath(IGlobal *This, VARIANT vtPointer1, BSTR wpSign, VARIANT vtPointer2, VARIANT *vtPointerResult)
{
  return Global_PtrMath(This, vtPointer1, wpSign, vtPointer2, vtPointerResult);
}
