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


//// ISystemFunction

HRESULT STDMETHODCALLTYPE SystemFunction_QueryInterface(ISystemFunction *this, REFIID vTableGuid, void **ppv)
{
  if (!ppv) return E_POINTER;

  if (AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=this;
    this->lpVtbl->SystemFunction_AddRef(this);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE SystemFunction_AddRef(ISystemFunction *this)
{
  return ++((IRealSystemFunction *)this)->dwCount;
}

ULONG STDMETHODCALLTYPE SystemFunction_Release(ISystemFunction *this)
{
  if (--((IRealSystemFunction *)this)->dwCount == 0)
  {
    GlobalFree(this);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealSystemFunction *)this)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE SystemFunction_GetTypeInfoCount(ISystemFunction *this, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE SystemFunction_GetTypeInfo(ISystemFunction *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
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

HRESULT STDMETHODCALLTYPE SystemFunction_GetIDsOfNames(ISystemFunction *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_SystemFunctionTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_SystemFunctionTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE SystemFunction_Invoke(ISystemFunction *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_SystemFunctionTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(this, g_SystemFunctionTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}


//// ISystemFunction methods

HRESULT STDMETHODCALLTYPE SystemFunction_AddParameter(ISystemFunction *this, VARIANT vtParameter)
{
  SYSPARAMSTACK *hStack=&((IRealSystemFunction *)this)->sf.hSysParamStack;
  SYSPARAMITEM *lpSysParam;
  VARIANT *pvtParameter=&vtParameter;

  if (lpSysParam=StackInsertSysParam(hStack))
  {
    if (pvtParameter->vt == (VT_VARIANT|VT_BYREF))
      pvtParameter=pvtParameter->pvarVal;
    lpSysParam->dwValue=GetVariantValue(pvtParameter, bOldWindows);
    lpSysParam->dwType=pvtParameter->vt;
  }
  else return E_OUTOFMEMORY;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE SystemFunction_Call(ISystemFunction *this, BSTR wpDllFunction, SAFEARRAY **psa, INT_PTR *nResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealSystemFunction *)this)->lpScriptThread;
  SYSTEMFUNCTION *sf;
  HMODULE hModule=NULL;
  FARPROC lpProcedure=NULL;
  char szFunction[MAX_PATH];
  wchar_t wszFunction[MAX_PATH];
  wchar_t wszDll[MAX_PATH];
  BOOL bLoadLibrary=FALSE;
  int i;

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
        SystemFunction_AddParameter(this, *pvtParameter);
      }
    }
  }

  *nResult=0;
  sf=&((IRealSystemFunction *)this)->sf;

  if (*wpDllFunction)
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
      {
        bLoadLibrary=TRUE;
      }
    }
    WideCharToMultiByte(CP_ACP, 0, wszFunction, -1, szFunction, MAX_PATH, NULL, NULL);

    if (hModule)
    {
      if (lpProcedure=GetProcAddress(hModule, szFunction))
      {
        //Call function
        *nResult=AsmCallSysFunc(&sf->hSysParamStack, &sf->hSaveStack, lpProcedure);

        //Get last error
        sf->dwLastError=GetLastError();

        //Free call parameters
        StackFreeSysParams(&sf->hSaveStack);
      }
      if (bLoadLibrary)
      {
        FreeLibrary(hModule);
      }
    }
  }

  if (lpScriptThread->dwDebug & DBG_SYSCALL)
  {
    if (!hModule)
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_SYSCALLDLL), wszDll);
      return E_POINTER;
    }
    if (!lpProcedure)
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_SYSCALLFUNCTION), wszFunction, wszDll);
      return E_POINTER;
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE SystemFunction_GetLastError(ISystemFunction *this, DWORD *dwLastError)
{
  *dwLastError=((IRealSystemFunction *)this)->sf.dwLastError;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE SystemFunction_RegisterCallback(ISystemFunction *this, IDispatch *objCallback, int nArgCount, VARIANT *vtFunction)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealSystemFunction *)this)->lpScriptThread;
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
        nArgCount=(int)GetVariantValue(&vtResult, bOldWindows);
    }
  }

  if (nArgCount >= 0)
  {
    CALLBACKITEM *lpCallback;
    SYSCALLBACK lpCallbackProc;
    int nIndex;

    //Find unhooked element if any
    if (lpCallback=StackGetCallbackByHandle(&g_hSysCallbackStack, NULL, NULL))
      nIndex=lpCallback->nStaticIndex - 1;
    else
      nIndex=g_hSysCallbackStack.nElements;

    //We support limited number of callbacks because with one callback we couldn't know who is called it
    if (nIndex == 0)
      lpCallbackProc=AsmCallback1Proc;
    else if (nIndex == 1)
      lpCallbackProc=AsmCallback2Proc;
    else if (nIndex == 2)
      lpCallbackProc=AsmCallback3Proc;
    else if (nIndex == 3)
      lpCallbackProc=AsmCallback4Proc;
    else if (nIndex == 4)
      lpCallbackProc=AsmCallback5Proc;
    else if (nIndex == 5)
      lpCallbackProc=AsmCallback6Proc;
    else if (nIndex == 6)
      lpCallbackProc=AsmCallback7Proc;
    else if (nIndex == 7)
      lpCallbackProc=AsmCallback8Proc;
    else if (nIndex == 8)
      lpCallbackProc=AsmCallback9Proc;
    else if (nIndex == 9)
      lpCallbackProc=AsmCallback10Proc;
    else if (nIndex == 10)
      lpCallbackProc=AsmCallback11Proc;
    else if (nIndex == 11)
      lpCallbackProc=AsmCallback12Proc;
    else if (nIndex == 12)
      lpCallbackProc=AsmCallback13Proc;
    else if (nIndex == 13)
      lpCallbackProc=AsmCallback14Proc;
    else if (nIndex == 14)
      lpCallbackProc=AsmCallback15Proc;
    else if (nIndex == 15)
      lpCallbackProc=AsmCallback16Proc;
    else if (nIndex == 16)
      lpCallbackProc=AsmCallback17Proc;
    else if (nIndex == 17)
      lpCallbackProc=AsmCallback18Proc;
    else if (nIndex == 18)
      lpCallbackProc=AsmCallback19Proc;
    else if (nIndex == 19)
      lpCallbackProc=AsmCallback20Proc;
    else if (nIndex == 20)
      lpCallbackProc=AsmCallback21Proc;
    else if (nIndex == 21)
      lpCallbackProc=AsmCallback22Proc;
    else if (nIndex == 22)
      lpCallbackProc=AsmCallback23Proc;
    else if (nIndex == 23)
      lpCallbackProc=AsmCallback24Proc;
    else if (nIndex == 24)
      lpCallbackProc=AsmCallback25Proc;
    else if (nIndex == 25)
      lpCallbackProc=AsmCallback26Proc;
    else if (nIndex == 26)
      lpCallbackProc=AsmCallback27Proc;
    else if (nIndex == 27)
      lpCallbackProc=AsmCallback28Proc;
    else if (nIndex == 28)
      lpCallbackProc=AsmCallback29Proc;
    else if (nIndex == 29)
      lpCallbackProc=AsmCallback30Proc;
    else
    {
      lpCallbackProc=NULL;
      objCallback=NULL;
      hr=DISP_E_BADINDEX;
    }

    if (lpCallbackProc)
    {
      if (!lpCallback)
        lpCallback=StackInsertCallback(&g_hSysCallbackStack);

      if (lpCallback)
      {
        objCallback->lpVtbl->AddRef(objCallback);
        lpCallback->hHandle=(HANDLE)(INT_PTR)lpCallbackProc;
        lpCallback->objFunction=objCallback;
        lpCallback->dwData=nArgCount;
        lpCallback->nCallbackType=CIT_SYSCALLBACK;
        lpCallback->lpScriptThread=(void *)lpScriptThread;

        if (!lpScriptThread->hWndScriptsThreadDummy)
        {
          lpScriptThread->hWndScriptsThreadDummy=CreateScriptsThreadDummyWindow();
        }
      }
    }
  }
  else if (hr == NOERROR)
  {
    hr=DISP_E_BADPARAMCOUNT;
  }

  objCallback->lpVtbl->AddRef(objCallback);
  vtFunction->vt=VT_DISPATCH;
  vtFunction->pdispVal=objCallback;

  return hr;
}

HRESULT STDMETHODCALLTYPE SystemFunction_UnregisterCallback(ISystemFunction *this, IDispatch *objFunction)
{
  CALLBACKITEM *lpCallback;

  if (lpCallback=StackGetCallbackByObject(&g_hSysCallbackStack, objFunction))
  {
    lpCallback->objFunction->lpVtbl->Release(lpCallback->objFunction);

    //We don't use StackDeleteCallback, because stack elements is linked to static procedure addresses.
    lpCallback->hHandle=NULL;
    lpCallback->objFunction=NULL;
    lpCallback->dwData=0;
    lpCallback->lpScriptThread=NULL;
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

  if (lpSysCallback=StackGetCallbackByIndex(&g_hSysCallbackStack, nCallbackIndex))
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
          VariantInit(vtCount);
          #ifdef _WIN64
            vtCount->llVal=*(lpnFirstArg + i);
            vtCount->vt=VT_I8;
          #else
            vtCount->lVal=*(lpnFirstArg + i);
            vtCount->vt=VT_I4;
          #endif
          --vtCount;
        }
      }
    }
    xmemset(&dispp, 0, sizeof(DISPPARAMS));
    dispp.cArgs=nArgCount;
    dispp.rgvarg=vtArg;

    //Because objFunction->lpVtbl->Invoke cause error for different thread, we send message from this thread to hWndScriptsThreadDummy.
    msgs.lpCallback=lpSysCallback;
    msgs.lParam=(LPARAM)&dispp;
    lResult=SendMessage(lpScriptThread->hWndScriptsThreadDummy, AKDLL_CALLBACKSEND, 0, (LPARAM)&msgs);

    if (vtArg) GlobalFree((HGLOBAL)vtArg);
  }
  if (lpnArgSize) *lpnArgSize=nArgCount * sizeof(INT_PTR);
  return lResult;
}
