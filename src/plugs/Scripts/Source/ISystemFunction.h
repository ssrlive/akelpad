#ifndef _ISYSTEMFUNCTION_H_
#define _ISYSTEMFUNCTION_H_

/*
// ISystemFunction's VTable
#undef INTERFACE
#define INTERFACE ISystemFunction
DECLARE_INTERFACE_ (INTERFACE, IDispatch)
{
  // IUnknown
  STDMETHOD_(HRESULT, SystemFunction_QueryInterface)(THIS_ const IID *, void **) PURE;
  STDMETHOD_(ULONG, SystemFunction_AddRef)(THIS) PURE;
  STDMETHOD_(ULONG, SystemFunction_Release)(THIS) PURE;

  // IDispatch
  STDMETHOD_(HRESULT, SystemFunction_GetTypeInfoCount)(THIS_ UINT *) PURE;
  STDMETHOD_(HRESULT, SystemFunction_GetTypeInfo)(THIS_ UINT, LCID, ITypeInfo **) PURE;
  STDMETHOD_(HRESULT, SystemFunction_GetIDsOfNames)(THIS_ const IID *, LPOLESTR *, UINT, LCID, DISPID *) PURE;
  STDMETHOD_(HRESULT, SystemFunction_Invoke)(THIS_ DISPID, const IID *, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) PURE;

  // ISystemFunction methods
  STDMETHOD_(HRESULT, SystemFunction_AddParameter)(THIS_ VARIANT) PURE;
  STDMETHOD_(HRESULT, SystemFunction_Call)(THIS_ VARIANT, SAFEARRAY **, VARIANT *) PURE;
  STDMETHOD_(HRESULT, SystemFunction_GetLastError)(THIS_ DWORD *) PURE;
  STDMETHOD_(HRESULT, SystemFunction_RegisterCallback)(THIS_ IDispatch *, int, VARIANT *) PURE;
  STDMETHOD_(HRESULT, SystemFunction_UnregisterCallback)(THIS_ IDispatch *) PURE;
};
//*/

//Defines
typedef struct _SYSPARAMITEM {
  struct _SYSPARAMITEM *next;
  struct _SYSPARAMITEM *prev;
  UINT_PTR dwType;
  UINT_PTR dwValue;
} SYSPARAMITEM;

typedef struct {
  SYSPARAMITEM *first;
  SYSPARAMITEM *last;
  int nElements;
} SYSPARAMSTACK;

typedef struct _SYSTEMFUNCTION {
  SYSPARAMSTACK hSysParamStack;
  SYSPARAMSTACK hSaveStack;
  DWORD dwLastError;
} SYSTEMFUNCTION;

typedef struct {
  ISystemFunctionVtbl *lpVtbl;
  DWORD dwCount;
  void *lpScriptThread;
  SYSTEMFUNCTION sf;
} IRealSystemFunction;

typedef LRESULT (*SYSCALLBACK)();

//Global variables
extern ITypeInfo *g_SystemFunctionTypeInfo;
extern CALLBACKSTACK g_hSysCallbackStack;
extern const ISystemFunctionVtbl MyISystemFunctionVtbl;

//Functions prototypes
HRESULT STDMETHODCALLTYPE SystemFunction_QueryInterface(ISystemFunction *This, const IID * vTableGuid, void **ppv);
ULONG STDMETHODCALLTYPE SystemFunction_AddRef(ISystemFunction *This);
ULONG STDMETHODCALLTYPE SystemFunction_Release(ISystemFunction *This);

HRESULT STDMETHODCALLTYPE SystemFunction_GetTypeInfoCount(ISystemFunction *This, UINT *pCount);
HRESULT STDMETHODCALLTYPE SystemFunction_GetTypeInfo(ISystemFunction *This, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo);
HRESULT STDMETHODCALLTYPE SystemFunction_GetIDsOfNames(ISystemFunction *This, const IID * riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
HRESULT STDMETHODCALLTYPE SystemFunction_Invoke(ISystemFunction *This, DISPID dispid, const IID * riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr);

HRESULT STDMETHODCALLTYPE SystemFunction_AddParameter(ISystemFunction *This, VARIANT vtParameter);
HRESULT STDMETHODCALLTYPE SystemFunction_Call(ISystemFunction *This, VARIANT vtDllFunction, SAFEARRAY **psa, VARIANT *vtResult);
HRESULT STDMETHODCALLTYPE SystemFunction_GetLastError(ISystemFunction *This, DWORD *dwLastError);
HRESULT STDMETHODCALLTYPE SystemFunction_RegisterCallback(ISystemFunction *This, IDispatch *objCallback, int nArgCount, VARIANT *vtFunction);
HRESULT STDMETHODCALLTYPE SystemFunction_UnregisterCallback(ISystemFunction *This, IDispatch *objFunction);
SYSPARAMITEM* StackInsertSysParam(SYSPARAMSTACK *hStack);
void StackSaveSysParam(SYSPARAMSTACK *hFromStack, SYSPARAMSTACK *hToStack);
void StackFreeSysParams(SYSPARAMSTACK *hStack);
int AsmCallSysFunc(SYSPARAMSTACK *hCurStack, SYSPARAMSTACK *hSaveStack, FARPROC lpProcedure);
LRESULT AsmCallback1Proc();
LRESULT AsmCallback2Proc();
LRESULT AsmCallback3Proc();
LRESULT AsmCallback4Proc();
LRESULT AsmCallback5Proc();
LRESULT AsmCallback6Proc();
LRESULT AsmCallback7Proc();
LRESULT AsmCallback8Proc();
LRESULT AsmCallback9Proc();
LRESULT AsmCallback10Proc();
LRESULT AsmCallback11Proc();
LRESULT AsmCallback12Proc();
LRESULT AsmCallback13Proc();
LRESULT AsmCallback14Proc();
LRESULT AsmCallback15Proc();
LRESULT AsmCallback16Proc();
LRESULT AsmCallback17Proc();
LRESULT AsmCallback18Proc();
LRESULT AsmCallback19Proc();
LRESULT AsmCallback20Proc();
LRESULT AsmCallback21Proc();
LRESULT AsmCallback22Proc();
LRESULT AsmCallback23Proc();
LRESULT AsmCallback24Proc();
LRESULT AsmCallback25Proc();
LRESULT AsmCallback26Proc();
LRESULT AsmCallback27Proc();
LRESULT AsmCallback28Proc();
LRESULT AsmCallback29Proc();
LRESULT AsmCallback30Proc();
LRESULT AsmCallbackCommonProc(int nCallbackIndex);
LRESULT AsmCallbackHelper(INT_PTR *lpnFirstArg, int nCallbackIndex, int *lpnArgSize);

#endif
