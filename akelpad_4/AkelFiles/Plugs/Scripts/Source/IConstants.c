#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Scripts.h"


//Global variables
ITypeInfo *g_ConstantsTypeInfo=NULL;

const IConstantsVtbl MyIConstantsVtbl={
  Constants_QueryInterface,
  Constants_AddRef,
  Constants_Release,
  Constants_GetTypeInfoCount,
  Constants_GetTypeInfo,
  Constants_GetIDsOfNames,
  Constants_Invoke,
  Constants_TCHAR,
  Constants_vbTCHAR,
  Constants_TSTR,
  Constants_vbTSTR,
  Constants_TSIZE,
  Constants_vbTSIZE,
  Constants_X64,
  Constants_vbX64
};


//// IConstants

HRESULT STDMETHODCALLTYPE Constants_QueryInterface(IConstants *this, REFIID vTableGuid, void **ppv)
{
  if (!ppv) return E_POINTER;

  if (AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=this;
    this->lpVtbl->Constants_AddRef(this);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE Constants_AddRef(IConstants *this)
{
  return ++((IRealConstants *)this)->dwCount;
}

ULONG STDMETHODCALLTYPE Constants_Release(IConstants *this)
{
  if (--((IRealConstants *)this)->dwCount == 0)
  {
    GlobalFree(this);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealConstants *)this)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE Constants_GetTypeInfoCount(IConstants *this, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Constants_GetTypeInfo(IConstants *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  HRESULT hr;

  *pTypeInfo=NULL;

  if (itinfo)
  {
    hr=ResultFromScode(DISP_E_BADINDEX);
  }
  else if (g_ConstantsTypeInfo)
  {
    g_ConstantsTypeInfo->lpVtbl->AddRef(g_ConstantsTypeInfo);
    hr=S_OK;
  }
  else
  {
    hr=LoadTypeInfoFromFile(NULL, NULL);
  }
  if (hr == S_OK) *pTypeInfo=g_ConstantsTypeInfo;

  return hr;
}

HRESULT STDMETHODCALLTYPE Constants_GetIDsOfNames(IConstants *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_ConstantsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_ConstantsTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE Constants_Invoke(IConstants *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_ConstantsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(this, g_ConstantsTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}

HRESULT STDMETHODCALLTYPE Constants_TCHAR(IConstants *this, BSTR *wpChar)
{
  HRESULT hr=NOERROR;

  if (!(*wpChar=SysAllocString(bOldWindows?L"A":L"W")))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE Constants_vbTCHAR(IConstants *this, BSTR *wpChar)
{
  return Constants_TCHAR(this, wpChar);
}

HRESULT STDMETHODCALLTYPE Constants_TSTR(IConstants *this, int *nStr)
{
  *nStr=bOldWindows?DT_ANSI:DT_UNICODE;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Constants_vbTSTR(IConstants *this, int *nStr)
{
  return Constants_TSTR(this, nStr);
}

HRESULT STDMETHODCALLTYPE Constants_TSIZE(IConstants *this, int *nSize)
{
  *nSize=bOldWindows?sizeof(char):sizeof(wchar_t);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Constants_vbTSIZE(IConstants *this, int *nSize)
{
  return Constants_TSIZE(this, nSize);
}

HRESULT STDMETHODCALLTYPE Constants_X64(IConstants *this, BOOL *b64)
{
  #ifdef _WIN64
    *b64=TRUE;
  #else
    *b64=FALSE;
  #endif
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Constants_vbX64(IConstants *this, BOOL *b64)
{
  return Constants_X64(this, b64);
}
