#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Scripts.h"


//Global variables
ITypeInfo *g_SystemFunctionTypeInfo=NULL;
CALLBACKSTACK g_hSysCallbackStack={0};

const ISystemFunctionVtbl MyISystemFunctionVtbl={
  SystemFunction_QueryInterface,
  SystemFunction_AddRef,
  SystemFunction_Release,
  SystemFunction_GetTypeInfoCount,
  SystemFunction_GetTypeInfo,
  SystemFunction_GetIDsOfNames,
  SystemFunction_Invoke,
  SystemFunction_AddParameter,
  SystemFunction_Call,
  SystemFunction_GetLastError,
  SystemFunction_RegisterCallback,
  SystemFunction_UnregisterCallback
};

CALLBACKBUSYNESS g_cbAsm[]={{(INT_PTR)AsmCallback1Proc,  FALSE},
                            {(INT_PTR)AsmCallback2Proc,  FALSE},
                            {(INT_PTR)AsmCallback3Proc,  FALSE},
                            {(INT_PTR)AsmCallback4Proc,  FALSE},
                            {(INT_PTR)AsmCallback5Proc,  FALSE},
                            {(INT_PTR)AsmCallback6Proc,  FALSE},
                            {(INT_PTR)AsmCallback7Proc,  FALSE},
                            {(INT_PTR)AsmCallback8Proc,  FALSE},
                            {(INT_PTR)AsmCallback9Proc,  FALSE},
                            {(INT_PTR)AsmCallback10Proc, FALSE},
                            {(INT_PTR)AsmCallback11Proc, FALSE},
                            {(INT_PTR)AsmCallback12Proc, FALSE},
                            {(INT_PTR)AsmCallback13Proc, FALSE},
                            {(INT_PTR)AsmCallback14Proc, FALSE},
                            {(INT_PTR)AsmCallback15Proc, FALSE},
                            {(INT_PTR)AsmCallback16Proc, FALSE},
                            {(INT_PTR)AsmCallback17Proc, FALSE},
                            {(INT_PTR)AsmCallback18Proc, FALSE},
                            {(INT_PTR)AsmCallback19Proc, FALSE},
                            {(INT_PTR)AsmCallback20Proc, FALSE},
                            {(INT_PTR)AsmCallback21Proc, FALSE},
                            {(INT_PTR)AsmCallback22Proc, FALSE},
                            {(INT_PTR)AsmCallback23Proc, FALSE},
                            {(INT_PTR)AsmCallback24Proc, FALSE},
                            {(INT_PTR)AsmCallback25Proc, FALSE},
                            {(INT_PTR)AsmCallback26Proc, FALSE},
                            {(INT_PTR)AsmCallback27Proc, FALSE},
                            {(INT_PTR)AsmCallback28Proc, FALSE},
                            {(INT_PTR)AsmCallback29Proc, FALSE},
                            {(INT_PTR)AsmCallback30Proc, FALSE},
                            {0, 0}};


//// ISystemFunction

HRESULT STDMETHODCALLTYPE SystemFunction_QueryInterface(ISystemFunction *This, const IID * vTableGuid, void **ppv)
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

ULONG STDMETHODCALLTYPE SystemFunction_AddRef(ISystemFunction *This)
{
  return ++((IRealSystemFunction *)This)->dwCount;
}

ULONG STDMETHODCALLTYPE SystemFunction_Release(ISystemFunction *This)
{
  if (--((IRealSystemFunction *)This)->dwCount == 0)
  {
    GlobalFree(This);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealSystemFunction *)This)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE SystemFunction_GetTypeInfoCount(ISystemFunction *This, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE SystemFunction_GetTypeInfo(ISystemFunction *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  HRESULT hr;

  *pTypeInfo=NULL;

  if (itinfo)
  {
    hr=ResultFromScode(DISP_E_BADINDEX);
  }
  else if (g_SystemFunctionTypeInfo)
  {
    g_SystemFunctionTypeInfo->lpVtbl->AddRef(g_SystemFunctionTypeInfo);
    hr=S_OK;
  }
  else
  {
    hr=LoadTypeInfoFromFile(NULL, NULL);
  }
  if (hr == S_OK) *pTypeInfo=g_SystemFunctionTypeInfo;

  return hr;
}

HRESULT STDMETHODCALLTYPE SystemFunction_GetIDsOfNames(ISystemFunction *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_SystemFunctionTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_SystemFunctionTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE SystemFunction_Invoke(ISystemFunction *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_SystemFunctionTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(This, g_SystemFunctionTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}


//// ISystemFunction methods

HRESULT STDMETHODCALLTYPE SystemFunction_AddParameter(ISystemFunction *This, VARIANT vtParameter)
{
  SYSPARAMSTACK *hStack=&((IRealSystemFunction *)This)->sf.hSysParamStack;
  SYSPARAMITEM *lpSysParam;
  VARIANT *pvtParameter=&vtParameter;

  if (lpSysParam=StackInsertSysParam(hStack))
  {
    lpSysParam->dwValue=GetVariantValue(pvtParameter, &pvtParameter, bOldWindows);
    lpSysParam->dwType=pvtParameter->vt;
  }
  else return E_OUTOFMEMORY;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE SystemFunction_Call(ISystemFunction *This, VARIANT vtDllFunction, SAFEARRAY **psa, VARIANT *vtResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealSystemFunction *)This)->lpScriptThread;
  VARIANT *pvtDllFunction=&vtDllFunction;
  SYSTEMFUNCTION *sf;
  HMODULE hModule=NULL;
  FARPROC lpProcedure=NULL;
  char szFunction[MAX_PATH];
  wchar_t wszFunction[MAX_PATH];
  wchar_t wszDll[MAX_PATH];
  wchar_t *wpDllFunction=NULL;
  UINT_PTR dwDllFunction;
  INT_PTR nResult=0;
  BOOL bLoadLibrary=FALSE;
  int i;

  sf=&((IRealSystemFunction *)This)->sf;
  dwDllFunction=GetVariantValue(pvtDllFunction, &pvtDllFunction, FALSE);
  if (pvtDllFunction->vt == VT_BSTR && pvtDllFunction->bstrVal && pvtDllFunction->bstrVal[0])
    wpDllFunction=(wchar_t *)dwDllFunction;
  else
    lpProcedure=(FARPROC)dwDllFunction;

  //Fill call stack
  {
    VARIANT *pvtParameter;
    unsigned char *lpData;
    DWORD dwElement;
    DWORD dwElementSum;

    lpData=(unsigned char *)((*psa)->pvData);
    dwElementSum=(*psa)->rgsabound[0].cElements;

    if (dwElementSum)
    {
      for (dwElement=0; dwElement < dwElementSum; ++dwElement)
      {
        pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
        SystemFunction_AddParameter(This, *pvtParameter);
      }
    }
  }

  if (wpDllFunction)
  {
    wszFunction[0]=L'\0';
    wszDll[0]=L'\0';

    for (i=0; wpDllFunction[i]; ++i)
    {
      if (wpDllFunction[i] == ':' && wpDllFunction[i + 1] == ':')
      {
        xstrcpynW(wszDll, wpDllFunction, i + 1);
        xstrcpynW(wszFunction, wpDllFunction + i + 2, MAX_PATH);
        break;
      }
    }

    if (!(hModule=GetModuleHandleWide(wszDll)))
    {
      if (hModule=LoadLibraryWide(wszDll))
        bLoadLibrary=TRUE;
    }
    WideCharToMultiByte(CP_ACP, 0, wszFunction, -1, szFunction, MAX_PATH, NULL, NULL);

    if (hModule)
      lpProcedure=GetProcAddress(hModule, szFunction);
  }
  if (lpProcedure)
  {
    //Call function
    nResult=AsmCallSysFunc(&sf->hSysParamStack, &sf->hSaveStack, lpProcedure);
    
    //Get last error
    sf->dwLastError=GetLastError();
  }

  //Free call parameters
  StackFreeSysParams(&sf->hSaveStack);

  if (bLoadLibrary)
  {
    FreeLibrary(hModule);
  }

  if (lpScriptThread && (lpScriptThread->dwDebug & DBG_SYSCALL))
  {
    if (wpDllFunction && !hModule)
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_SYSCALL), wszDll);
      return E_POINTER;
    }
    if (!lpProcedure)
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_SYSFUNCTION), wszFunction, wszDll);
      return E_POINTER;
    }
  }
  SetVariantInt(vtResult, nResult);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE SystemFunction_GetLastError(ISystemFunction *This, DWORD *dwLastError)
{
  *dwLastError=((IRealSystemFunction *)This)->sf.dwLastError;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE SystemFunction_RegisterCallback(ISystemFunction *This, IDispatch *objCallback, int nArgCount, VARIANT *vtFunction)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealSystemFunction *)This)->lpScriptThread;
  wchar_t *wpLength=L"length";
  DISPID dispidCallbackName;
  DISPPARAMS dispp;
  VARIANT vtResult;
  HRESULT hr=NOERROR;

  VariantInit(vtFunction);
  if (!objCallback)
    return hr;

  if (nArgCount == -1)
  {
    //JScript only
    if ((hr=objCallback->lpVtbl->GetIDsOfNames(objCallback, &IID_NULL, &wpLength, 1, LOCALE_USER_DEFAULT, &dispidCallbackName)) == S_OK)
    {
      dispp.rgvarg=NULL;
      dispp.rgdispidNamedArgs=NULL;
      dispp.cArgs=0;
      dispp.cNamedArgs=0;

      if ((hr=objCallback->lpVtbl->Invoke(objCallback, dispidCallbackName, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispp, &vtResult, 0, 0)) == S_OK)
        nArgCount=(int)GetVariantValue(&vtResult, NULL, bOldWindows);
    }
  }

  if (nArgCount >= 0)
  {
    CALLBACKITEM *lpCallback;
    int nBusyIndex;

    if ((nBusyIndex=RetriveCallbackProc(g_cbAsm)) >= 0)
    {
      if (lpCallback=StackGetCallbackByObject(&g_hSysCallbackStack, objCallback))
      {
        ++lpCallback->nRefCount;
      }
      else if (lpCallback=StackInsertCallback(&g_hSysCallbackStack, objCallback))
      {
        g_cbAsm[nBusyIndex].bBusy=TRUE;
        lpCallback->nBusyIndex=nBusyIndex;
        lpCallback->lpProc=g_cbAsm[nBusyIndex].lpProc;
        lpCallback->hHandle=NULL;
        lpCallback->dwData=nArgCount;
        lpCallback->nCallbackType=CIT_SYSCALLBACK;
        lpCallback->lpScriptThread=(void *)lpScriptThread;

        if (!lpScriptThread->hWndScriptsThreadDummy)
        {
          lpScriptThread->hWndScriptsThreadDummy=CreateScriptsThreadDummyWindow();
        }
      }
      else
      {
        objCallback=NULL;
        hr=E_OUTOFMEMORY;
      }
    }
    else
    {
      objCallback=NULL;
      hr=DISP_E_BADINDEX;
    }
  }
  else if (hr == NOERROR)
  {
    objCallback=NULL;
    hr=DISP_E_BADPARAMCOUNT;
  }

  vtFunction->vt=VT_DISPATCH;
  vtFunction->pdispVal=objCallback;
  return hr;
}

HRESULT STDMETHODCALLTYPE SystemFunction_UnregisterCallback(ISystemFunction *This, IDispatch *objFunction)
{
  CALLBACKITEM *lpCallback;
  int nBusyIndex;

  if (lpCallback=StackGetCallbackByObject(&g_hSysCallbackStack, objFunction))
  {
    nBusyIndex=lpCallback->nBusyIndex;
    if (StackDeleteCallback(lpCallback))
      g_cbAsm[nBusyIndex].bBusy=FALSE;
  }
  return NOERROR;
}

SYSPARAMITEM* StackInsertSysParam(SYSPARAMSTACK *hStack)
{
  SYSPARAMITEM *lpSysParam;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpSysParam, 1, sizeof(SYSPARAMITEM)))
    ++hStack->nElements;

  return lpSysParam;
}

void StackSaveSysParam(SYSPARAMSTACK *hFromStack, SYSPARAMSTACK *hToStack)
{
  StackJoin((stack **)&hToStack->first, (stack **)&hToStack->last, NULL, (stack *)hFromStack->first, (stack *)hFromStack->last);
  hFromStack->first=0;
  hFromStack->last=0;
  hFromStack->nElements=0;
}

void StackFreeSysParams(SYSPARAMSTACK *hStack)
{
  SYSPARAMITEM *lpSysParam;

  if (bOldWindows)
  {
    for (lpSysParam=hStack->first; lpSysParam; lpSysParam=lpSysParam->next)
    {
      if (lpSysParam->dwType == VT_BSTR)
      {
        if (lpSysParam->dwValue)
          GlobalFree((HGLOBAL)lpSysParam->dwValue);
      }
    }
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}

#ifndef _WIN64
int __declspec(naked) AsmCallSysFunc(SYSPARAMSTACK *hCurStack, SYSPARAMSTACK *hSaveStack, FARPROC lpProcedure)
{
  SYSPARAMITEM *lpSysParam;
  DWORD dwValue;

  __asm
  {
    //Prolog
    push  ebp
    mov   ebp, esp
    sub   esp, __LOCAL_SIZE
  }

  //Push parameters to stack
  for (lpSysParam=hCurStack->first; lpSysParam; lpSysParam=lpSysParam->next)
  {
    dwValue=lpSysParam->dwValue;

    __asm
    {
      push dwValue
    }
  }

  //Move arguments from hCurStack to hSaveStack stack to support recursive calls
  StackSaveSysParam(hCurStack, hSaveStack);

  __asm
  {
    //Call
    call  lpProcedure

    //Epilog
    mov  esp, ebp
    pop  ebp
    ret
  }
}

LRESULT __declspec(naked) AsmCallback1Proc()
{
  AsmCallbackCommonProc(1);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback2Proc()
{
  AsmCallbackCommonProc(2);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback3Proc()
{
  AsmCallbackCommonProc(3);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback4Proc()
{
  AsmCallbackCommonProc(4);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback5Proc()
{
  AsmCallbackCommonProc(5);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback6Proc()
{
  AsmCallbackCommonProc(6);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback7Proc()
{
  AsmCallbackCommonProc(7);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback8Proc()
{
  AsmCallbackCommonProc(8);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback9Proc()
{
  AsmCallbackCommonProc(9);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback10Proc()
{
  AsmCallbackCommonProc(10);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback11Proc()
{
  AsmCallbackCommonProc(11);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback12Proc()
{
  AsmCallbackCommonProc(12);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback13Proc()
{
  AsmCallbackCommonProc(13);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback14Proc()
{
  AsmCallbackCommonProc(14);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback15Proc()
{
  AsmCallbackCommonProc(15);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback16Proc()
{
  AsmCallbackCommonProc(16);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback17Proc()
{
  AsmCallbackCommonProc(17);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback18Proc()
{
  AsmCallbackCommonProc(18);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback19Proc()
{
  AsmCallbackCommonProc(19);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback20Proc()
{
  AsmCallbackCommonProc(20);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback21Proc()
{
  AsmCallbackCommonProc(21);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback22Proc()
{
  AsmCallbackCommonProc(22);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback23Proc()
{
  AsmCallbackCommonProc(23);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback24Proc()
{
  AsmCallbackCommonProc(24);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback25Proc()
{
  AsmCallbackCommonProc(25);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback26Proc()
{
  AsmCallbackCommonProc(26);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback27Proc()
{
  AsmCallbackCommonProc(27);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback28Proc()
{
  AsmCallbackCommonProc(28);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback29Proc()
{
  AsmCallbackCommonProc(29);
  __asm ret
}

LRESULT __declspec(naked) AsmCallback30Proc()
{
  AsmCallbackCommonProc(30);
  __asm ret
}

LRESULT __declspec(naked) AsmCallbackCommonProc(int nCallbackIndex)
{
  //More info at: http://en.wikibooks.org/wiki/X86_Disassembly/Functions_and_Stack_Frames
  int *lpnFirstArg;
  int nArgSize;
  int nOldESI;
  int nOldEDI;
  LRESULT lResult;

  __asm
  {
    //Prolog
    push ebp
    mov  ebp, esp
    sub  esp, __LOCAL_SIZE

    //Remember EDI/ESI registers for debugger cause API calls change it
    mov  nOldESI, esi
    mov  nOldEDI, edi

    //Get pointer to first element in stack
    mov  lpnFirstArg, ebp

    //To get first argument pointer we skip first four elements in stack:
    //ebp+0  saved ebp in prolog
    //esp+4  AsmCallbackCommonProc return address
    //esp+8  nCallbackIndex argument
    //esp+12 AsmCallbackXProc return address
    add  lpnFirstArg, 16
  }
  lResult=AsmCallbackHelper(lpnFirstArg, nCallbackIndex, &nArgSize);

  __asm
  {
    //Remember nArgSize in ecx
    mov  ecx, nArgSize

    //Return value
    mov  eax, lResult

    //Restore EDI/ESI registers
    mov  edi, nOldEDI
    mov  esi, nOldESI

    //Epilog
    mov  esp, ebp
    pop  ebp

    //We will use AsmCallbackXProc return address:
    //esp+0   AsmCallbackCommonProc return address
    //esp+4   nCallbackIndex argument
    //esp+8   AsmCallbackXProc return address
    //esp+12  Pushed function arguments with size nArgSize
    add  esp, 8      //Now point to AsmCallbackXProc return address.

    //Required for x86: we are stdcall so remove pushed function arguments.
    pop  edx         //Remove from stack and save return address in ecx register.
    add  esp, ecx    //Correct stack pointer.
    push edx         //Restore return address on place of the last argument.
    ret
  }
}
#endif

LRESULT AsmCallbackHelper(INT_PTR *lpnFirstArg, int nCallbackIndex, int *lpnArgSize)
{
  CALLBACKITEM *lpSysCallback;
  SCRIPTTHREAD *lpScriptThread;
  MSGSEND msgs;
  DISPPARAMS dispp;
  VARIANT *vtArg=NULL;
  VARIANT *vtCount;
  LRESULT lResult=0;
  int nArgCount=0;
  int i;

  if (lpSysCallback=StackGetCallbackByProc(&g_hSysCallbackStack, g_cbAsm[nCallbackIndex - 1].lpProc))
  {
    lpScriptThread=(SCRIPTTHREAD *)lpSysCallback->lpScriptThread;
    nArgCount=(int)lpSysCallback->dwData;

    if (nArgCount)
    {
      if (vtArg=(VARIANT *)GlobalAlloc(GPTR, sizeof(VARIANT) * nArgCount))
      {
        //Get last VARIANT pointer, because DISPPARAMS filled in reverse order
        vtCount=vtArg + (nArgCount - 1);

        for (i=0; i < nArgCount; ++i)
        {
          SetVariantInt(vtCount, *(lpnFirstArg + i));
          --vtCount;
        }
      }
    }
    xmemset(&dispp, 0, sizeof(DISPPARAMS));
    dispp.cArgs=nArgCount;
    dispp.rgvarg=vtArg;

    //Because objFunction->lpVtbl->Invoke cause error for different thread, we send message from This thread to hWndScriptsThreadDummy.
    msgs.lpCallback=lpSysCallback;
    msgs.lParam=(LPARAM)&dispp;
    lResult=SendMessage(lpScriptThread->hWndScriptsThreadDummy, AKDLL_CALLBACKSEND, 0, (LPARAM)&msgs);

    if (vtArg) GlobalFree((HGLOBAL)vtArg);
  }
  if (lpnArgSize) *lpnArgSize=nArgCount * sizeof(INT_PTR);
  return lResult;
}
