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
  WScript_Quit,
  WScript_ConnectObject,
  WScript_DisconnectObject
};

const IDispatchVtbl MyISinkVtbl={
  Sink_QueryInterface,
  Sink_AddRef,
  Sink_Release,
  Sink_GetTypeInfoCount,
  Sink_GetTypeInfo,
  Sink_GetIDsOfNames,
  Sink_Invoke
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
  const wchar_t *wpExt=GetFileExt(lpScriptThread->wszScriptName, -1);

  lpScriptThread->bQuiting=TRUE;

  if (!xstrcmpiW(wpExt, L"vbs") && !bOldWindows)
  {
    //On Win9x this will cause crash, but it is necessary for VBScript code:
    //  On Error Resume Next
    //  WScript.Quit
    //  MsgBox "Not quit"
    lpScriptThread->objActiveScript->lpVtbl->SetScriptState(lpScriptThread->objActiveScript, SCRIPTSTATE_CLOSED);
  }

  //If we return SCRIPT_E_PROPAGATE (to skip OnScriptError), then jscript9Legacy.dll will crash.
  return E_FAIL;
}

HRESULT STDMETHODCALLTYPE WScript_ConnectObject(IWScript *this, IDispatch *objConnectTo, BSTR wpPrefix, VARIANT vtIID, int *nCount)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;
  VARIANT *pvtIID=&vtIID;
  IID *lpIID;
  SINKITEM *lpSinkItem;
  CONNECTITEM *lpConnectItem;
  IEnumConnectionPoints *objEnumConnectionPoints;
  IConnectionPoint *objConnectionPoint;
  IRealSink *objISink;
  DWORD dwFetched;
  HRESULT hr=NOERROR;

  if (lpSinkItem=StackInsertSink(&lpScriptThread->hSinkStack))
  {
    objISink=&lpSinkItem->objISink;
    objISink->lpVtbl=(IDispatch *)&MyISinkVtbl;
    objISink->dwCount=1;
    objISink->lpScriptThread=lpScriptThread;
    objISink->objConnectTo=objConnectTo;
    objISink->wpPrefix=SysAllocString(wpPrefix);

    InterlockedIncrement(&g_nObjs);
  }
  else return E_OUTOFMEMORY;

  if (pvtIID->vt == VT_BSTR && pvtIID->bstrVal && pvtIID->bstrVal[0])
  {
    IIDFromString(pvtIID->bstrVal, &objISink->iidConnect);
    objISink->piidConnect=&objISink->iidConnect;
  }
  else
  {
    if (lpIID=(IID *)GetVariantInt(pvtIID, &pvtIID))
    {
      xmemcpy(&objISink->iidConnect, lpIID, sizeof(IID));
      objISink->piidConnect=&objISink->iidConnect;
    }
  }

  if (!(hr=objConnectTo->lpVtbl->QueryInterface(objConnectTo, &IID_IConnectionPointContainer, (void **)&objISink->objConnectionPointContainer)))
  {
    if (objISink->piidConnect)
    {
      if (!(hr=objISink->objConnectionPointContainer->lpVtbl->FindConnectionPoint(objISink->objConnectionPointContainer, objISink->piidConnect, &objConnectionPoint)))
      {
        if (lpConnectItem=StackInsertConnection(&objISink->hConnectStack))
        {
          if (!(hr=objConnectionPoint->lpVtbl->Advise(objConnectionPoint, (IUnknown *)objISink, &lpConnectItem->dwCookie)))
            lpConnectItem->objConnectionPoint=objConnectionPoint;
          else
          {
            objConnectionPoint->lpVtbl->Release(objConnectionPoint);
            StackDeleteConnection(&objISink->hConnectStack, lpConnectItem);
          }
          if (objISink->hConnectStack.nElements) hr=S_OK;
        }
      }
    }
    else
    {
      if (!(hr=objISink->objConnectionPointContainer->lpVtbl->EnumConnectionPoints(objISink->objConnectionPointContainer, &objEnumConnectionPoints)))
      {
        while (!objEnumConnectionPoints->lpVtbl->Next(objEnumConnectionPoints, 1, &objConnectionPoint, &dwFetched))
        {
          if (lpConnectItem=StackInsertConnection(&objISink->hConnectStack))
          {
            if (!(hr=objConnectionPoint->lpVtbl->Advise(objConnectionPoint, (IUnknown *)objISink, &lpConnectItem->dwCookie)))
              lpConnectItem->objConnectionPoint=objConnectionPoint;
            else
            {
              objConnectionPoint->lpVtbl->Release(objConnectionPoint);
              StackDeleteConnection(&objISink->hConnectStack, lpConnectItem);
            }
          }
        }
        if (objISink->hConnectStack.nElements) hr=S_OK;
      }
    }
  }

  ////Show connected interfaces
  //{
  //  IID iidConnect;
  //  BSTR wpIDD;
  //
  //  for (lpConnectItem=objISink->hConnectStack.first; lpConnectItem; lpConnectItem=lpConnectItem->next)
  //  {
  //    lpConnectItem->objConnectionPoint->lpVtbl->GetConnectionInterface(lpConnectItem->objConnectionPoint, &iidConnect);
  //    StringFromIID(&iidConnect, &wpIDD);
  //    MessageBoxW(NULL, wpIDD, NULL, 0);
  //    SysFreeString(wpIDD);
  //  }
  //}

  *nCount=objISink->hConnectStack.nElements;
  if (!*nCount)
    StackDeleteSink(&lpScriptThread->hSinkStack, lpSinkItem);
  return hr;
}

HRESULT STDMETHODCALLTYPE WScript_DisconnectObject(IWScript *this, IDispatch *objConnectTo)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealWScript *)this)->lpScriptThread;
  SINKITEM *lpSinkItem;
  CONNECTITEM *lpConnectItem;
  IRealSink *objISink;

  if (lpSinkItem=StackGetSink(&lpScriptThread->hSinkStack, objConnectTo))
  {
    objISink=&lpSinkItem->objISink;
    for (lpConnectItem=objISink->hConnectStack.first; lpConnectItem; lpConnectItem=lpConnectItem->next)
    {
      if (lpConnectItem->dwCookie)
        lpConnectItem->objConnectionPoint->lpVtbl->Unadvise(lpConnectItem->objConnectionPoint, lpConnectItem->dwCookie);
      if (lpConnectItem->objConnectionPoint)
        lpConnectItem->objConnectionPoint->lpVtbl->Release(lpConnectItem->objConnectionPoint);
    }
    if (objISink->objConnectionPointContainer)
      objISink->objConnectionPointContainer->lpVtbl->Release(objISink->objConnectionPointContainer);
    StackDeleteSink(&lpScriptThread->hSinkStack, lpSinkItem);
  }
  return NOERROR;
}

SINKITEM* StackInsertSink(SINKSTACK *hStack)
{
  SINKITEM *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(SINKITEM)))
    ++hStack->nElements;

  return lpElement;
}

SINKITEM* StackGetSink(SINKSTACK *hStack, IDispatch *objConnectTo)
{
  SINKITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (objConnectTo >= lpElement->objISink.objConnectTo)
      break;
  }
  return lpElement;
}

void StackDeleteSink(SINKSTACK *hStack, SINKITEM *lpSinkItem)
{
  StackFreeConnections(&lpSinkItem->objISink.hConnectStack);
  if (lpSinkItem->objISink.wpPrefix)
    SysFreeString(lpSinkItem->objISink.wpPrefix);
  if (!StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpSinkItem))
    --hStack->nElements;
}

void StackFreeSinks(SINKSTACK *hStack)
{
  SINKITEM *lpElement;
  SINKITEM *lpNextElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpNextElement)
  {
    lpNextElement=lpElement->next;
    StackDeleteSink(hStack, lpElement);
  }
}

CONNECTITEM* StackInsertConnection(CONNECTSTACK *hStack)
{
  CONNECTITEM *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(CONNECTITEM)))
    ++hStack->nElements;

  return lpElement;
}

void StackDeleteConnection(CONNECTSTACK *hStack, CONNECTITEM *lpConnectItem)
{
  if (!StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpConnectItem))
    --hStack->nElements;
}

void StackFreeConnections(CONNECTSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}


//// Events

HRESULT STDMETHODCALLTYPE Sink_QueryInterface(IDispatch *this, REFIID vTableGuid, void **ppv)
{
  if (((IRealSink *)this)->piidConnect && AKD_IsEqualIID(vTableGuid, ((IRealSink *)this)->piidConnect) ||
      AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=this;
    this->lpVtbl->AddRef(this);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE Sink_AddRef(IDispatch *this)
{
  return ++((IRealSink *)this)->dwCount;
}

ULONG STDMETHODCALLTYPE Sink_Release(IDispatch *this)
{
  if (--((IRealSink *)this)->dwCount == 0)
  {
    GlobalFree(this);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealSink *)this)->dwCount;
}

HRESULT STDMETHODCALLTYPE Sink_GetTypeInfoCount(IDispatch *this, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Sink_GetTypeInfo(IDispatch *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  *pTypeInfo=NULL;

  if (itinfo)
    return ResultFromScode(DISP_E_BADINDEX);
  *pTypeInfo=NULL;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Sink_GetIDsOfNames(IDispatch *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Sink_Invoke(IDispatch *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  IRealSink *objRealSink=(IRealSink *)this;
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)objRealSink->lpScriptThread;
  ITypeLib *objTypeLib=NULL;
  ITypeInfo *objTypeInfo;
  ITypeInfo *objTypeInfoGuid=NULL;
  BSTR wpName;
  UINT dwIndex;
  UINT dwCount;
  DISPID dispidScript;
  HRESULT hr=NOERROR;

  if (!objRealSink->objConnectTo->lpVtbl->GetTypeInfo(objRealSink->objConnectTo, 0, lcid, &objTypeInfo))
  {
    if (hr=objTypeInfo->lpVtbl->GetDocumentation(objTypeInfo, dispid, &wpName, NULL, NULL, NULL))
    {
      if (!(hr=objTypeInfo->lpVtbl->GetContainingTypeLib(objTypeInfo, &objTypeLib, &dwIndex)))
      {
        if (objRealSink->piidConnect)
        {
          if (!(hr=objTypeLib->lpVtbl->GetTypeInfoOfGuid(objTypeLib, objRealSink->piidConnect, &objTypeInfoGuid)))
            hr=objTypeInfoGuid->lpVtbl->GetDocumentation(objTypeInfoGuid, dispid, &wpName, NULL, NULL, NULL);
        }
        else
        {
          //Search in all TypeInfo's of TypeLib
          for (dwCount=0; ; ++dwCount)
          {
            if (dwCount != dwIndex)
            {
              if (hr=objTypeLib->lpVtbl->GetTypeInfo(objTypeLib, dwCount, &objTypeInfoGuid))
                break;
              if (!(hr=objTypeInfoGuid->lpVtbl->GetDocumentation(objTypeInfoGuid, dispid, &wpName, NULL, NULL, NULL)))
                break;
              objTypeInfoGuid->lpVtbl->Release(objTypeInfoGuid);
              objTypeInfoGuid=NULL;
            }
          }
        }
      }
    }
    if (!hr)
    {
      if (lpScriptThread->objThis)
      {
        wchar_t *wszCallFunc;
        BSTR wpCallFunc;
        int nCallFuncLen=SysStringLen(objRealSink->wpPrefix) + SysStringLen(wpName);

        if (wszCallFunc=(wchar_t *)GlobalAlloc(GPTR, (2 + nCallFuncLen + 2) * sizeof(wchar_t)))
        {
          //First four bytes in BSTR is number of bytes in the following data string terminated with two NULLs
          *(DWORD *)wszCallFunc=nCallFuncLen * sizeof(wchar_t);
          wpCallFunc=wszCallFunc + 2;
          xprintfW(wpCallFunc, L"%s%s", objRealSink->wpPrefix, wpName);

          if (!(hr=lpScriptThread->objThis->lpVtbl->GetIDsOfNames(lpScriptThread->objThis, riid, &wpCallFunc, 1, lcid, &dispidScript)))
          {
            hr=lpScriptThread->objThis->lpVtbl->Invoke(lpScriptThread->objThis, dispidScript, riid, lcid, wFlags, params, result, pexcepinfo, puArgErr);
          }
          GlobalFree((HGLOBAL)wszCallFunc);
        }
      }
      if (objTypeInfoGuid)
        objTypeInfoGuid->lpVtbl->Release(objTypeInfoGuid);
      SysFreeString(wpName);
    }
    if (objTypeLib)
      objTypeLib->lpVtbl->Release(objTypeLib);
    objTypeInfo->lpVtbl->Release(objTypeInfo);
  }
  return hr;
}
