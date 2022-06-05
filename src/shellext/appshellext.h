// appshellext.h : Declaration of the CAppShellExt

#ifndef __appshellext_H__
#define __appshellext_H__

#include "resource.h"       // main symbols

interface DECLSPEC_UUID("000214E4-0000-0000-C000-000000000046") IContextMenu; 
interface DECLSPEC_UUID("000214f4-0000-0000-c000-000000000046") IContextMenu2;
interface DECLSPEC_UUID("bcfce0a0-ec17-11d0-8d10-00a0c90f2719") IContextMenu3;


//===========================================================================
// The following two lines identify the difference applications.
#define APPNAME    _T("AkelPad") 
class DECLSPEC_UUID("6FB61F9F-E0DE-48E0-9370-883C8FAFD5A6") CAppShellExt; 
//===========================================================================

/////////////////////////////////////////////////////////////////////////////
// CAppShellExt
class ATL_NO_VTABLE CAppShellExt
    : public CComObjectRootEx < CComSingleThreadModel > 
    , public CComCoClass < CAppShellExt, & __uuidof(CAppShellExt) > 
    , public IShellExtInit 
    , public IContextMenu3
{
public:
    CAppShellExt();

    //DECLARE_REGISTRY_RESOURCEID(IDR_APPSHELLEXT)
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) throw();

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CAppShellExt)
        COM_INTERFACE_ENTRY(IShellExtInit) 
        COM_INTERFACE_ENTRY(IContextMenu) 
        COM_INTERFACE_ENTRY(IContextMenu2) 
        COM_INTERFACE_ENTRY(IContextMenu3) 
    END_COM_MAP()

    HRESULT FinalConstruct(); 
    void FinalRelease(); 


public: 

    // 
    // IShellExtInit 
    // 
    STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID); 

#if _MSC_VER > 1200
#define IDCMD_TYPE UINT_PTR
#else
#define IDCMD_TYPE UINT
#endif

    // 
    // IContextMenu 
    // 
    STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags); 
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici); 
    STDMETHOD(GetCommandString)(IDCMD_TYPE idCmd, UINT uType, UINT * pwReserved, LPSTR pszName, UINT cchMax); 

    // 
    // IContextMenu2 
    // 
    STDMETHOD(HandleMenuMsg)(UINT uMsg, WPARAM wParam, LPARAM lParam); 

    // 
    // IContextMenu3 
    // 
    STDMETHOD(HandleMenuMsg2)(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *pResult); 

protected: 
    HBITMAP IconToBitmap(UINT uIcon, COLORREF transparentColor); 
    std::map<UINT, HBITMAP> bitmaps; 

    typedef struct SP_MENU_INFO { 
        UINT uPosition; 
        UINT_PTR uCmdID; 
        TCHAR szMenuText[MAX_PATH]; 
        UINT_PTR imgID; 
    } SP_MENU_INFO; 

    std::map<UINT_PTR, SP_MENU_INFO> m_mapMenuInfo; 

    BOOL _ExecuteCommand(void);
    void _ShowErrorMessage(void);

protected: 
    _tstring_list   m_lsFiles; 
};

#endif //__appshellext_H__
