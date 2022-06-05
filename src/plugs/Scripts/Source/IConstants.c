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

HRESULT STDMETHODCALLTYPE Constants_QueryInterface(IConstants *This, const IID * vTableGuid, void **ppv)
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

ULONG STDMETHODCALLTYPE Constants_AddRef(IConstants *This)
{
  return ++((IRealConstants *)This)->dwCount;
}

ULONG STDMETHODCALLTYPE Constants_Release(IConstants *This)
{
  if (--((IRealConstants *)This)->dwCount == 0)
  {
    GlobalFree(This);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealConstants *)This)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE Constants_GetTypeInfoCount(IConstants *This, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Constants_GetTypeInfo(IConstants *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
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

HRESULT STDMETHODCALLTYPE Constants_GetIDsOfNames(IConstants *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_ConstantsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_ConstantsTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE Constants_Invoke(IConstants *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_ConstantsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(This, g_ConstantsTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}

HRESULT STDMETHODCALLTYPE Constants_TCHAR(IConstants *This, BSTR *wpChar)
{
  HRESULT hr=NOERROR;

  if (!(*wpChar=SysAllocString(bOldWindows?L"A":L"W")))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE Constants_vbTCHAR(IConstants *This, BSTR *wpChar)
{
  return Constants_TCHAR(This, wpChar);
}

HRESULT STDMETHODCALLTYPE Constants_TSTR(IConstants *This, int *nStr)
{
  *nStr=bOldWindows?DT_ANSI:DT_UNICODE;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Constants_vbTSTR(IConstants *This, int *nStr)
{
  return Constants_TSTR(This, nStr);
}

HRESULT STDMETHODCALLTYPE Constants_TSIZE(IConstants *This, int *nSize)
{
  *nSize=bOldWindows?sizeof(char):sizeof(wchar_t);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Constants_vbTSIZE(IConstants *This, int *nSize)
{
  return Constants_TSIZE(This, nSize);
}

HRESULT STDMETHODCALLTYPE Constants_X64(IConstants *This, BOOL *b64)
{
  #ifdef _WIN64
    *b64=TRUE;
  #else
    *b64=FALSE;
  #endif
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Constants_vbX64(IConstants *This, BOOL *b64)
{
  return Constants_X64(This, b64);
}
