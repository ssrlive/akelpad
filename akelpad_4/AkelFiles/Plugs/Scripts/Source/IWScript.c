#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include "Scripts.h"


//Global variables
ITypeInfo *g_WScriptTypeInfo=NULL;

const IWScriptVtbl MyIWScriptVtbl={
  WScript_QueryInterface,
  WScript_AddRef,
  WScript_Release,
  WScript_GetTypeInfoCount,
  WScript_GetTypeInfo,
  WScript_GetIDsOfNames,
  WScript_Invoke,
  WScript_Arguments,
  WScript_ScriptFullName,
  WScript_ScriptName,
  WScript_ScriptBaseName,
  WScript_FullName,
  WScript_Path,
  WScript_Name,
  WScript_Echo,
  WScript_Sleep,
  WScript_Quit
};


//// IWScript

HRESULT STDMETHODCALLTYPE WScript_QueryInterface(IWScript *this, REFIID vTableGuid, void **ppv)
{
  if (AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IWScript) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=this;
    this->lpVtbl->WScript_AddRef(this);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE WScript_AddRef(IWScript *this)
{
  return ++((IRealWScript *)this)->dwCount;
}

ULONG STDMETHODCALLTYPE WScript_Release(IWScript *this)
{
  if (--((IRealWScript *)this)->dwCount == 0)
  {
    GlobalFree(this);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealWScript *)this)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE WScript_GetTypeInfoCount(IWScript *this, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE WScript_GetTypeInfo(IWScript *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  HRESULT hr;

  *pTypeInfo=NULL;

  if (itinfo)
  {
    hr=ResultFromScode(DISP_E_BADINDEX);
  }
  else if (g_WScriptTypeInfo)
  {
    g_WScriptTypeInfo->lpVtbl->AddRef(g_WScriptTypeInfo);
    hr=S_OK;
  }
  else
  {
    hr=LoadTypeInfoFromFile(NULL, NULL);
  }
  if (hr == S_OK) *pTypeInfo=g_WScriptTypeInfo;

  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_GetIDsOfNames(IWScript *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_WScriptTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_WScriptTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE WScript_Invoke(IWScript *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_WScriptTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(this, g_WScriptTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}


//// IWScript methods

HRESULT STDMETHODCALLTYPE WScript_Arguments(IWScript *this, IDispatch **objWArguments)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;
  IRealWArguments *lpWArguments;

  if (!objWArguments) return E_POINTER;

  if ((lpWArguments=(IRealWArguments *)GlobalAlloc(GPTR, sizeof(IRealWArguments))))
  {
    lpWArguments->lpVtbl=(IWArgumentsVtbl *)&MyIWArgumentsVtbl;
    lpWArguments->dwCount=1;
    lpWArguments->lpScriptThread=(void *)lpScriptThread;

    InterlockedIncrement(&g_nObjs);
    *objWArguments=(IDispatch *)lpWArguments;
  }
  else return E_OUTOFMEMORY;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE WScript_ScriptFullName(IWScript *this, BSTR *wpScriptFullName)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;
  HRESULT hr=NOERROR;

  if (!(*wpScriptFullName=SysAllocString(lpScriptThread->wszScriptFile)))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_ScriptName(IWScript *this, BSTR *wpScriptName)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;
  HRESULT hr=NOERROR;

  if (!(*wpScriptName=SysAllocString(lpScriptThread->wszScriptName)))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_ScriptBaseName(IWScript *this, BSTR *wpScriptBaseName)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;
  HRESULT hr=NOERROR;

  if (!(*wpScriptBaseName=SysAllocString(lpScriptThread->wszScriptBaseName)))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_FullName(IWScript *this, BSTR *wpDllFullName)
{
  HRESULT hr=NOERROR;

  xprintfW(wszBuffer, L"%s\\%s.dll", wszScriptsDir, wszPluginName);
  if (!(*wpDllFullName=SysAllocString(wszBuffer)))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_Path(IWScript *this, BSTR *wpDllPath)
{
  HRESULT hr=NOERROR;

  if (!(*wpDllPath=SysAllocString(wszScriptsDir)))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_Name(IWScript *this, BSTR *wpDllName)
{
  HRESULT hr=NOERROR;

  xprintfW(wszBuffer, L"%s.dll", wszPluginName);
  if (!(*wpDllName=SysAllocString(wszBuffer)))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_Echo(IWScript *this, BSTR wpText)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;

  MessageBoxW(hMainWnd, wpText, lpScriptThread->wszScriptName, MB_OK);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE WScript_Sleep(IWScript *this, int nTime)
{
  Sleep(nTime);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE WScript_Quit(IWScript *this, int nErrorCode)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;

  lpScriptThread->bQuiting=TRUE;

  if (!bOldWindows)
  {
    //On Win9x this will cause crash, but it is necessary for VBScript code:
    //  On Error Resume Next
    //  WScript.Quit
    //  MsgBox "Not quit"
    lpScriptThread->objActiveScript->lpVtbl->SetScriptState(lpScriptThread->objActiveScript, SCRIPTSTATE_DISCONNECTED);
    lpScriptThread->objActiveScript->lpVtbl->Close(lpScriptThread->objActiveScript);
  }

  //This error code disconnect script (skipping OnScriptError).
  //If we return E_FAIL, then debugger will start before execution go to OnScriptError.
  return SCRIPT_E_PROPAGATE;
}
