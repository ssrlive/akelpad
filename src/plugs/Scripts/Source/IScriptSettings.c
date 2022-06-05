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

HRESULT STDMETHODCALLTYPE ScriptSettings_QueryInterface(IScriptSettings *This, const IID * vTableGuid, void **ppv)
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

ULONG STDMETHODCALLTYPE ScriptSettings_AddRef(IScriptSettings *This)
{
  return ++((IRealScriptSettings *)This)->dwCount;
}

ULONG STDMETHODCALLTYPE ScriptSettings_Release(IScriptSettings *This)
{
  if (--((IRealScriptSettings *)This)->dwCount == 0)
  {
    GlobalFree(This);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealScriptSettings *)This)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE ScriptSettings_GetTypeInfoCount(IScriptSettings *This, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_GetTypeInfo(IScriptSettings *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
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

HRESULT STDMETHODCALLTYPE ScriptSettings_GetIDsOfNames(IScriptSettings *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_ScriptSettingsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_ScriptSettingsTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Invoke(IScriptSettings *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_ScriptSettingsTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(This, g_ScriptSettingsTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}


//// IScriptSettings methods

HRESULT STDMETHODCALLTYPE ScriptSettings_Begin(IScriptSettings *This, BSTR wpScriptName, DWORD dwFlags, VARIANT *vtOptions)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealScriptSettings *)This)->lpScriptThread;
  HANDLE *lphOptions=&((IRealScriptSettings *)This)->hOptions;

  if (!wpScriptName || !*wpScriptName)
    wpScriptName=lpScriptThread->wszScriptBaseName;
  *lphOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, dwFlags|POB_SCRIPTS, (LPARAM)wpScriptName);
  SetVariantInt(vtOptions, (INT_PTR)*lphOptions);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Read(IScriptSettings *This, VARIANT vtOptionName, DWORD dwType, VARIANT vtDefault, VARIANT *vtData)
{
  HANDLE hOptions=((IRealScriptSettings *)This)->hOptions;
  const wchar_t *wpOptionName;
  unsigned char *lpData;
  INT_PTR nDataSize;
  DWORD dwOptionType=dwType;
  HRESULT hr=NOERROR;

  VariantInit(vtData);

  wpOptionName=(wchar_t *)GetVariantValue(&vtOptionName, NULL, FALSE);

  if (dwType == PO_BINARYSIZE)
  {
    dwOptionType=PO_BINARY;
  }
  else if (dwType == PO_BINARYSTRING)
  {
    dwOptionType=PO_BINARY;
    dwType=PO_STRING;
  }
  nDataSize=WideOption(hOptions, wpOptionName, dwOptionType, NULL, 0);

  if (dwType == PO_BINARYSIZE)
  {
    SetVariantInt(vtData, nDataSize);
    return hr;
  }
  if (nDataSize >= 0)
  {
    if (lpData=(unsigned char *)GlobalAlloc(GPTR, nDataSize))
    {
      WideOption(hOptions, wpOptionName, dwOptionType, lpData, (DWORD)nDataSize);

      if (dwType == PO_DWORD)
      {
        vtData->vt=VT_I4;
        vtData->lVal=*(int *)lpData;
      }
      else if (dwType == PO_BINARY)
      {
        SetVariantInt(vtData, (INT_PTR)lpData);
        lpData=NULL;
      }
      else if (dwType == PO_STRING || dwType == PO_ENUM)
      {
        vtData->vt=VT_BSTR;
        if (!(vtData->bstrVal=SysAllocStringLen((wchar_t *)lpData, (UINT)(nDataSize / sizeof(wchar_t)) - 1)))
          hr=E_OUTOFMEMORY;
      }
      if (lpData) GlobalFree((HGLOBAL)lpData);
    }
  }
  else
  {
    if (vtDefault.vt != VT_ERROR)
      hr=VariantCopy(vtData, &vtDefault);
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Write(IScriptSettings *This, BSTR wpOptionName, DWORD dwType, VARIANT vtData, int nDataSize, int *nResult)
{
  HANDLE hOptions=((IRealScriptSettings *)This)->hOptions;
  unsigned char *lpData=NULL;
  VARIANT *pvtData=&vtData;
  UINT_PTR dwNumber=0;

  *nResult=0;
  if (!hOptions) return NOERROR;
  dwNumber=GetVariantValue(pvtData, NULL, FALSE);

  if (pvtData->vt == VT_BSTR && (dwType == PO_STRING || dwType == PO_BINARY))
  {
    #if defined(_WIN64) || (defined(SCRIPTS_MAXHANDLE) && SCRIPTS_MAXHANDLE < 0xFFFFFFFF)
      if (pvtData->bstrVal && !pvtData->bstrVal[0] && SysStringLen(pvtData->bstrVal) > 0)
      {
        //JScript doesn't support VT_I8, so __int64 number converted to string.
      }
      else
    #endif
    {
      lpData=(unsigned char *)pvtData->bstrVal;
      if (nDataSize == -1)
        nDataSize=(SysStringLen(pvtData->bstrVal) + 1) * sizeof(wchar_t);
      if (!lpData)
        lpData=(unsigned char *)L"";
    }
  }
  if (!lpData && (dwType == PO_DWORD || dwType == PO_BINARY))
  {
    if (dwType == PO_DWORD)
    {
      if (nDataSize == -1)
        nDataSize=sizeof(DWORD);
      lpData=(unsigned char *)&dwNumber;
    }
    else if (dwType == PO_BINARY)
    {
      if (nDataSize != -1)
        lpData=(unsigned char *)dwNumber;
    }
  }
  if (lpData && nDataSize > 0)
    *nResult=(int)WideOption(hOptions, wpOptionName, dwType, lpData, nDataSize);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_Delete(IScriptSettings *This, BSTR wpOptionName, BOOL *bResult)
{
  HANDLE hOptions=((IRealScriptSettings *)This)->hOptions;

  *bResult=(BOOL)WideOption(hOptions, wpOptionName, PO_REMOVE, NULL, 0);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE ScriptSettings_End(IScriptSettings *This, BOOL *bResult)
{
  HANDLE *lphOptions=&((IRealScriptSettings *)This)->hOptions;

  *bResult=(BOOL)SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)*lphOptions, 0);
  *lphOptions=NULL;
  return NOERROR;
}
