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

HRESULT STDMETHODCALLTYPE Global_QueryInterface(IGlobal *this, REFIID vTableGuid, void **ppv)
{
  if (!ppv) return E_POINTER;

  if (AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=this;
    this->lpVtbl->Global_AddRef(this);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE Global_AddRef(IGlobal *this)
{
  return ++((IRealGlobal *)this)->dwCount;
}

ULONG STDMETHODCALLTYPE Global_Release(IGlobal *this)
{
  if (--((IRealGlobal *)this)->dwCount == 0)
  {
    GlobalFree(this);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealGlobal *)this)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE Global_GetTypeInfoCount(IGlobal *this, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Global_GetTypeInfo(IGlobal *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
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

HRESULT STDMETHODCALLTYPE Global_GetIDsOfNames(IGlobal *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_GlobalTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_GlobalTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE Global_Invoke(IGlobal *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_GlobalTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(this, g_GlobalTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}

HRESULT STDMETHODCALLTYPE Global_PtrAdd(IGlobal *this, VARIANT vtPointer1, VARIANT vtPointer2, VARIANT *vtPointerResult)
{
  INT_PTR nPointer1=GetVariantInt(&vtPointer1, NULL);
  INT_PTR nPointer2=GetVariantInt(&vtPointer2, NULL);

  SetVariantInt(vtPointerResult, nPointer1 + nPointer2);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Global_vbPtrAdd(IGlobal *this, VARIANT vtPointer1, VARIANT vtPointer2, VARIANT *vtPointerResult)
{
  return Global_PtrAdd(this, vtPointer1, vtPointer2, vtPointerResult);
}

HRESULT STDMETHODCALLTYPE Global_PtrMath(IGlobal *this, VARIANT vtPointer1, BSTR wpSign, VARIANT vtPointer2, VARIANT *vtPointerResult)
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

HRESULT STDMETHODCALLTYPE Global_vbPtrMath(IGlobal *this, VARIANT vtPointer1, BSTR wpSign, VARIANT vtPointer2, VARIANT *vtPointerResult)
{
  return Global_PtrMath(this, vtPointer1, wpSign, vtPointer2, vtPointerResult);
}
