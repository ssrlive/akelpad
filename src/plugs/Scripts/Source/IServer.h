#ifndef _ISERVER_H_
#define _ISERVER_H_

//GUID
extern const struct _GUID CLSID_IDocument;
extern const struct _GUID CLSID_TypeLib;
extern const struct _GUID IID_IWScript;
extern const struct _GUID IID_IWArguments;
extern const struct _GUID IID_IDocument;
extern const struct _GUID IID_ISystemFunction;
extern const struct _GUID IID_IConstants;
extern const struct _GUID IID_IGlobal;

typedef struct {
  const GUID *guid;
  ITypeInfo **ppTypeInfo;
} GUIDTYPEINFO;

//Global variables
extern LONG g_nObjs;
extern LONG g_nLocks;
extern IClassFactory MyIClassFactoryObj;
extern const IClassFactoryVtbl MyIClassFactoryVtbl;

//Functions prototypes
HRESULT LoadTypeInfoFromFile(const GUID *guid, ITypeInfo **ppTypeInfo);

HRESULT STDMETHODCALLTYPE Class_QueryInterface(IClassFactory *This, const IID * factoryGuid, void **ppv);
ULONG STDMETHODCALLTYPE Class_AddRef(IClassFactory *This);
ULONG STDMETHODCALLTYPE Class_Release(IClassFactory *This);
HRESULT STDMETHODCALLTYPE Class_CreateInstance(IClassFactory *This, IUnknown *punkOuter, const IID * vTableGuid, void **objHandle);
HRESULT STDMETHODCALLTYPE Class_LockServer(IClassFactory *This, BOOL flock);

BOOL AKD_IsEqualIID(const GUID *rguid1, const GUID *rguid2);

HRESULT WINAPI DllGetClassObject(REFCLSID objGuid, const IID * factoryGuid, void **factoryHandle);
HRESULT WINAPI DllCanUnloadNow(void);
HRESULT WINAPI DllRegisterServer();
HRESULT WINAPI DllUnregisterServer();

void stringFromCLSIDA(char *szString, REFCLSID clsid);
HRESULT IsServerRegisteredA(HINSTANCE hInstance, REFCLSID clsid, const char *pNameTypeLib, const char *pNameProgID, const char *pObjectDescription, char *szDllFile, int nDllFileLen);
HRESULT RegisterServerA(HINSTANCE hInstance, REFCLSID clsid, const char *pNameTypeLib, const char *pNameProgID, const char *pObjectDescription);
HRESULT UnregisterServerA(REFCLSID clsid, REFGUID typelib, const char *pNameProgID);

#endif
