#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Scripts.h"


//Global variables
ITypeInfo *g_ScriptSettingsTypeInfo=NULL;

const IScriptSettingsVtbl MyIScriptSettingsVtbl={
  ScriptSettings_QueryInterface,
  ScriptSettings_AddRef,
  ScriptSettings_Release,
  ScriptSettings_GetTypeInfoCount,
  ScriptSettings_GetTypeInfo,
  ScriptSettings_GetIDsOfNames,
  ScriptSettings_Invoke,
  ScriptSettings_Begin,
  ScriptSettings_Read,
  ScriptSettings_Write,
  ScriptSettings_Delete,
  ScriptSettings_End
};


//// IScriptSettings

HRESULT STDMETHODCALLTYPE ScriptSettings_QueryInterface(IScriptSettings *this, REFIID vTableGuid, void **ppv)
{
  if (!ppv) return E_POINTER;

  if (AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=this;
    this->lpVtbl->ScriptSettings_AddRef(this);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE ScriptSettings_AddRef(IScriptSettings *this)
{
  return ++((IRealScriptSettings *)this)->dwCount;
}

ULONG STDMETHODCALLTYPE ScriptSettings_Release(IScriptSettings *this)
{
  if (--((IRealScriptSettings *)this)->dwCount == 0)
  {
    GlobalFree(this);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealScriptSettings *)this)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE ScriptSettings_GetTypeInfoCount(IScriptSettings *this, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_GetTypeInfo(IScriptSettings *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  HRESULT hr;

  *pTypeInfo=NULL;

  if (itinfo)
  {
    hr=ResultFromScode(DISP_E_BADINDEX);
  }
  else if (g_ScriptSettingsTypeInfo)
  {
    g_ScriptSettingsTypeInfo->lpVtbl->AddRef(g_ScriptSettingsTypeInfo);
    hr=S_OK;
  }
  else
  {
    hr=LoadTypeInfoFromFile(NULL, NULL);
  }
  if (hr == S_OK) *pTypeInfo=g_ScriptSettingsTypeInfo;

  return hr;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_GetIDsOfNames(IScriptSettings *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_ScriptSettingsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_ScriptSettingsTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Invoke(IScriptSettings *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_ScriptSettingsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(this, g_ScriptSettingsTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}


//// IScriptSettings methods

HRESULT STDMETHODCALLTYPE ScriptSettings_Begin(IScriptSettings *this, BSTR wpScriptName, DWORD dwFlags, HANDLE *hOptions)
{
  HANDLE *lphOptions=&((IRealScriptSettings *)this)->hOptions;

  *lphOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, dwFlags|POB_SCRIPTS, (LPARAM)wpScriptName);
  *hOptions=*lphOptions;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Read(IScriptSettings *this, BSTR wpOptionName, DWORD dwType, VARIANT vtDefault, VARIANT *vtData)
{
  HANDLE hOptions=((IRealScriptSettings *)this)->hOptions;
  unsigned char *lpData;
  INT_PTR nDataSize;
  HRESULT hr=NOERROR;

  VariantInit(vtData);

  if (nDataSize=WideOption(hOptions, wpOptionName, dwType, NULL, 0))
  {
    if (lpData=(unsigned char *)GlobalAlloc(GPTR, nDataSize))
    {
      WideOption(hOptions, wpOptionName, dwType, lpData, (DWORD)nDataSize);

      if (dwType == PO_DWORD)
      {
        vtData->vt=VT_I4;
        vtData->lVal=*(int *)lpData;
      }
      else if (dwType == PO_BINARY)
      {
        #ifdef _WIN64
          vtData->vt=VT_I8;
          vtData->llVal=*(INT_PTR *)lpData;
        #else
          vtData->vt=VT_I4;
          vtData->lVal=*(INT_PTR *)lpData;
        #endif
      }
      else if (dwType == PO_STRING)
      {
        vtData->vt=VT_BSTR;
        if (!(vtData->bstrVal=SysAllocStringLen((wchar_t *)lpData, (UINT)(nDataSize / sizeof(wchar_t)) - 1)))
          hr=E_OUTOFMEMORY;
      }
      GlobalFree((HGLOBAL)lpData);
    }
  }
  else
  {
    if (vtDefault.vt != VT_ERROR)
      hr=VariantCopy(vtData, &vtDefault);
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Write(IScriptSettings *this, BSTR wpOptionName, DWORD dwType, VARIANT vtData, int nDataSize, int *nResult)
{
  HANDLE hOptions=((IRealScriptSettings *)this)->hOptions;
  VARIANT *pvtData=&vtData;
  UINT_PTR dwNumber=0;

  *nResult=0;
  if (!hOptions) return NOERROR;

  if (pvtData->vt == (VT_VARIANT|VT_BYREF))
    pvtData=pvtData->pvarVal;

  if (dwType == PO_STRING)
  {
    if (pvtData->vt == VT_BSTR)
    {
      if (nDataSize == -1)
        nDataSize=(SysStringLen(pvtData->bstrVal) + 1) * sizeof(wchar_t);
      *nResult=(int)WideOption(hOptions, wpOptionName, dwType, (unsigned char *)pvtData->bstrVal, nDataSize);
    }
  }
  else if (dwType == PO_DWORD || dwType == PO_BINARY)
  {
    if (pvtData->vt == VT_BSTR)
    {
      dwNumber=(UINT_PTR)pvtData->bstrVal;
    }
    else if (pvtData->vt == VT_BOOL)
    {
      dwNumber=pvtData->boolVal?TRUE:FALSE;
    }
    else
    {
      #ifdef _WIN64
        VariantChangeType(pvtData, pvtData, 0, VT_UI8);
        dwNumber=pvtData->ullVal;
      #else
        VariantChangeType(pvtData, pvtData, 0, VT_UI4);
        dwNumber=pvtData->ulVal;
      #endif
    }

    if (dwType == PO_DWORD)
    {
      if (nDataSize == -1)
        nDataSize=sizeof(DWORD);
      *nResult=(int)WideOption(hOptions, wpOptionName, dwType, (unsigned char *)&dwNumber, nDataSize);
    }
    else if (dwType == PO_BINARY)
    {
      if (nDataSize != -1)
        *nResult=(int)WideOption(hOptions, wpOptionName, dwType, (unsigned char *)dwNumber, nDataSize);
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Delete(IScriptSettings *this, BSTR wpOptionName, BOOL *bResult)
{
  HANDLE hOptions=((IRealScriptSettings *)this)->hOptions;

  *bResult=(BOOL)WideOption(hOptions, wpOptionName, PO_REMOVE, NULL, 0);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_End(IScriptSettings *this, BOOL *bResult)
{
  HANDLE *lphOptions=&((IRealScriptSettings *)this)->hOptions;

  *bResult=(BOOL)SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)*lphOptions, 0);
  *lphOptions=NULL;
  return NOERROR;
}
