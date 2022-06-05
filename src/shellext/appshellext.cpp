// appshellext.cpp : Implementation of CAppShellExt
#include "stdafx.h"
#include "appshellext.h"

#include "FileAssociate.h"

/////////////////////////////////////////////////////////////////////////////
// CAppShellExt

CAppShellExt::CAppShellExt()
{
}

HRESULT WINAPI CAppShellExt::UpdateRegistry(BOOL bRegister) throw()
{
	USES_CONVERSION;
	WCHAR bstrCLSID[MAX_PATH/2] = { 0 };
	StringFromGUID2(__uuidof(CAppShellExt), bstrCLSID, _countof(bstrCLSID));

    _ATL_REGMAP_ENTRY regMapEntries[] =
    {
        { L"CLSID", bstrCLSID },
        { L"CLASSNAME", T2CW(APPNAME) },
        { NULL, NULL }
    };
    return _pModule->UpdateRegistryFromResource(IDR_APPSHELLEXT, bRegister, regMapEntries);
}

HRESULT CAppShellExt::FinalConstruct() 
{ 
    return S_OK; 
} 

void CAppShellExt::FinalRelease() 
{ 
    std::map<UINT, HBITMAP>::iterator it; 
    for (it = bitmaps.begin(); it != bitmaps.end(); ++it) 
    { 
        ::DeleteObject(it->second); 
    } 
    bitmaps.clear(); 
} 

STDMETHODIMP CAppShellExt::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID ) 
{ 
    pidlFolder, pDataObj, hProgID; 
    UINT      uNumFiles = 0; 
    HDROP     hdrop = NULL; 
    FORMATETC etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL }; 
    STGMEDIUM stg = { TYMED_HGLOBAL }; 
    TCHAR szFile[MAX_PATH] = { 0 }; 

    // Read the list of folders from the data object.  They're stored in HDROP 
    // format, so just get the HDROP handle and then use the drag 'n' drop APIs 
    // on it. 
    if ( FAILED( pDataObj->GetData ( &etc, &stg ) )) 
        return E_INVALIDARG; 

    // Get an HDROP handle. 
    hdrop = (HDROP) GlobalLock ( stg.hGlobal ); 

    if ( NULL == hdrop ) 
    { 
        ReleaseStgMedium ( &stg ); 
        return E_INVALIDARG; 
    } 

    m_lsFiles.clear(); 

    // Determine how many files are involved in this operation. 
    uNumFiles = DragQueryFile ( hdrop, 0xFFFFFFFF, NULL, 0 ); 

    for ( UINT uFile = 0; uFile < uNumFiles; uFile++ ) 
    { 
        // Get the next filename. 
        if ( 0 == DragQueryFile ( hdrop, uFile, szFile, MAX_PATH ) ) 
            continue; 

        m_lsFiles.push_back ( szFile ); 
    }   // end for 

    // Release resources. 
    GlobalUnlock ( stg.hGlobal ); 
    ReleaseStgMedium ( &stg ); 

    // If we found any files we can work with, return S_OK.  Otherwise, 
    // return E_INVALIDARG so we don't get called again for this right-click 
    // operation. 
    return (m_lsFiles.size() > 0) ? S_OK : E_INVALIDARG; 
} 


STDMETHODIMP CAppShellExt::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, 
                                            UINT idCmdLast, UINT uFlags) 
{ 
    hmenu, indexMenu, idCmdFirst, idCmdLast, uFlags; 
    UINT uCmdID = idCmdFirst; 

    // If the flags include CMF_DEFAULTONLY then we shouldn't do anything. 
    if ( uFlags & CMF_DEFAULTONLY ) 
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0); 

    m_mapMenuInfo.clear(); 

    // Add our menu items. 
    TCHAR szFormat[MAX_PATH] = { 0 }; 
    TCHAR szMenuItem[MAX_PATH] = { 0 }; 
    SP_MENU_INFO stMenuInfo = { 0 }; 

    OSVERSIONINFOEX osvi = { sizeof(osvi) };
    if( !GetVersionEx ((OSVERSIONINFO *) &osvi)) {
        // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
        osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
        GetVersionEx ( (OSVERSIONINFO *) &osvi);
    }

    BOOL bVistaOrLater = FALSE;
    if (osvi.dwMajorVersion >= 6) {
        bVistaOrLater = TRUE;
    }

    { 
        ::LoadString(_Module.m_hInstResource, IDS_OPEN_WITH, szFormat, _countof(szFormat)); 
        _stprintf(szMenuItem, szFormat, _T("&") APPNAME); 
        UINT flags = (MF_STRING | MF_BYPOSITION) | (bVistaOrLater ? 0 : MF_OWNERDRAW);
        InsertMenu ( hmenu, indexMenu, flags, uCmdID, szMenuItem ); 

        stMenuInfo.uPosition = indexMenu; 
        stMenuInfo.uCmdID = uCmdID; 
        lstrcpyn(stMenuInfo.szMenuText, szMenuItem, _countof(stMenuInfo.szMenuText)); 
        stMenuInfo.imgID = IDI_PROGRAM; 

        m_mapMenuInfo[stMenuInfo.uCmdID] = stMenuInfo; 

        uCmdID++; 
    } 

    if (bVistaOrLater) {
        // Set the bitmap for the register item. 
        HBITMAP bmp = IconToBitmap(IDI_PROGRAM, (COLORREF)GetSysColor(COLOR_MENU)); 
        if ( NULL != bmp ) {
            SetMenuItemBitmaps ( hmenu, indexMenu, MF_BYPOSITION, bmp, bmp ); 
        }
    }

    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 2);
}


BOOL _BuildIndicateString(LPTSTR szInfo, UINT cchMax)
{
    szInfo, cchMax;
    TCHAR szOpenWith[MAX_PATH] = { 0 };
    ::LoadString(_Module.m_hInstResource, IDS_OPEN_WITH_2, szOpenWith, _countof(szOpenWith));
    _stprintf(szInfo, szOpenWith, APPNAME);
    return TRUE;
}


STDMETHODIMP CAppShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpici) 
{ 
    HRESULT hRes = E_FAIL; 
    USES_CONVERSION;
    do 
    {
        // If lpVerb really points to a string, ignore this function call and bail out. 
        if ( 0 != HIWORD( lpici->lpVerb )) { 
            TCHAR szPrompt[MAX_PATH] = { 0 }; 
            _BuildIndicateString(szPrompt, _countof(szPrompt));

            if (strcmp(lpici->lpVerb, T2CA(szPrompt)) == 0) {
                if (_ExecuteCommand()) {
                    hRes = S_OK;
                } else {
                    _ShowErrorMessage();
                }
            } else {
                hRes = E_INVALIDARG; 
            }
            break;
        } 

        // Check that lpVerb is one of our commands (0 or 1) 
        switch ( LOWORD( lpici->lpVerb )) 
        { 
        case 0: 
            if (_ExecuteCommand()) { 
                hRes = S_OK; 
            } else { 
                _ShowErrorMessage();
            } 
            break; 

        default: 
            hRes = E_INVALIDARG; 
            break; 
        } 
    } while (FALSE);
    return hRes; 
} 

STDMETHODIMP CAppShellExt::GetCommandString(IDCMD_TYPE idCmd, UINT uType, UINT * pwReserved, 
                                            LPSTR pszName, UINT cchMax) 
{ 
    idCmd, uType, pwReserved, pszName, cchMax; 
    USES_CONVERSION; 
    TCHAR szPrompt[MAX_PATH] = { 0 }; 

    if ( uType & GCS_HELPTEXT ) 
    { 
        switch ( idCmd ) 
        { 
        case 0: 
            _BuildIndicateString(szPrompt, _countof(szPrompt));
            break; 

        default: 
            return E_INVALIDARG; 
            break; 
        } 

        // Copy the help text into the supplied buffer.  If the shell wants 
        // a Unicode string, we need to case szName to an LPCWSTR. 
        if ( uType & GCS_UNICODE ) {
            lstrcpynW ( (LPWSTR) pszName, T2CW(szPrompt), cchMax ); 
        } else {
            lstrcpynA ( pszName, T2CA(szPrompt), cchMax ); 
        }
    } 

    return S_OK; 
} 

// 
// IContextMenu2 
// 
STDMETHODIMP CAppShellExt::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    LRESULT res; 
    return HandleMenuMsg2(uMsg, wParam, lParam, &res); 
} 

// 
// IContextMenu3 
// 
STDMETHODIMP CAppShellExt::HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *pResult) 
{ 
    //a great tutorial on owner drawn menus in shell extension can be found 
    //here: http://www.codeproject.com/shell/shellextguide7.asp 

    LRESULT res; 
    if (pResult == NULL) 
        pResult = &res; 
    *pResult = FALSE; 
    TCHAR stringtablebuffer[MAX_PATH] = _T("") ; 
    int space = 0; 

    switch (uMsg) 
    { 
    case WM_MEASUREITEM: 
        { 
            MEASUREITEMSTRUCT* lpmis = (MEASUREITEMSTRUCT*)lParam; 
            if (lpmis==NULL) 
                break; 
            *pResult = TRUE; 
            lpmis->itemWidth = 0; 
            lpmis->itemHeight = 0; 
            POINT size; 
            //get the information about the shell DC, font, ... 
            NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) }; 
            if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0)==0) 
                break; 
            HDC hDC = CreateCompatibleDC(NULL); 
            if (hDC == NULL) 
                break; 
            HFONT hFont = CreateFontIndirect(&ncm.lfMenuFont); 
            if (hFont == NULL) 
                break; 
            HFONT oldFont = (HFONT)SelectObject(hDC, hFont); 

            std::map<UINT_PTR, SP_MENU_INFO>::iterator it; 
            it = m_mapMenuInfo.find(lpmis->itemID); 

            ATLASSERT(it != m_mapMenuInfo.end()); 

            lstrcpyn(stringtablebuffer, it->second.szMenuText, _countof(stringtablebuffer)); 

            GetTextExtentPoint32(hDC, stringtablebuffer, (int)_tcslen(stringtablebuffer), (SIZE*)&size); 
            LPtoDP(hDC, &size, 1); 
            //now release the font and DC 
            SelectObject(hDC, oldFont); 
            DeleteObject(hFont); 
            DeleteDC(hDC); 

            // width of string + height of string (~ width of icon) + space between 
            lpmis->itemWidth = size.x + size.y + 6; 
            // two pixels on both sides 
            lpmis->itemHeight = max(size.y + 4, ncm.iMenuHeight); 
        } 
        break; 
    case WM_DRAWITEM: 
        { 
            TCHAR *szItem = NULL; 
            DRAWITEMSTRUCT* lpdis = (DRAWITEMSTRUCT*)lParam; 
            if ((lpdis==NULL)||(lpdis->CtlType != ODT_MENU)) 
                return S_OK;		//not for a menu 

            std::map<UINT_PTR, SP_MENU_INFO>::iterator it; 
            it = m_mapMenuInfo.find(lpdis->itemID); 
            ATLASSERT(it != m_mapMenuInfo.end()); 
            lstrcpyn(stringtablebuffer, it->second.szMenuText, _countof(stringtablebuffer)); 

            szItem = stringtablebuffer; 
            int ix, iy; 
            RECT rt, rtTemp; 
            SIZE size; 
            // choose text colors 
            if (lpdis->itemState & ODS_SELECTED) 
            { 
                COLORREF crText; 
                if (lpdis->itemState & ODS_GRAYED) 
                    crText = SetTextColor(lpdis->hDC, GetSysColor(COLOR_GRAYTEXT)); //RGB(128, 128, 128)); 
                else 
                    crText = SetTextColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT)); 
                SetBkColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHT)); 
            } 
            CopyRect(&rtTemp, &(lpdis->rcItem)); 

            ix = lpdis->rcItem.left + space; 
            SetRect(&rt, ix, rtTemp.top, ix + 16, rtTemp.bottom); 

            HICON hIcon = (HICON)LoadImage(_Module.m_hInstResource, 
                MAKEINTRESOURCE(it->second.imgID), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR); 

            if (hIcon == NULL) 
                return S_OK; 
            // convert the icon into a bitmap 
            ICONINFO ii; 
            if (GetIconInfo(hIcon, &ii)==FALSE) 
                return S_OK; 
            HBITMAP hbmItem = ii.hbmColor; 
            if (hbmItem) 
            { 
                BITMAP bm; 
                GetObject(hbmItem, sizeof(bm), &bm); 

                int tempY = lpdis->rcItem.top + ((lpdis->rcItem.bottom - lpdis->rcItem.top) - bm.bmHeight) / 2; 
                SetRect(&rt, ix, tempY, ix + 16, tempY + 16); 
                ExtTextOut(lpdis->hDC, 0, 0, ETO_CLIPPED|ETO_OPAQUE, &rtTemp, NULL, 0, (LPINT)NULL); 
                rtTemp.left = rt.right; 

                HDC hDCTemp = CreateCompatibleDC(lpdis->hDC); 
                HGDIOBJ hOldBMP = SelectObject(hDCTemp, hbmItem); 
                DrawIconEx(lpdis->hDC, rt.left , rt.top, hIcon, bm.bmWidth, bm.bmHeight, 
                    0, NULL, DI_NORMAL); 

                SelectObject(hDCTemp, hOldBMP); 
                DeleteDC(hDCTemp); 
                DeleteObject(hbmItem); 
            } 
            ix = rt.right + space; 

            //free memory 
            DeleteObject(ii.hbmColor);
            DeleteObject(ii.hbmMask);
            DestroyIcon(hIcon);

            //draw menu text
            GetTextExtentPoint32(lpdis->hDC, szItem, lstrlen(szItem), &size);
            iy = ((lpdis->rcItem.bottom - lpdis->rcItem.top) - size.cy) / 2;
            iy = lpdis->rcItem.top + (iy>=0 ? iy : 0);
            SetRect(&rt, ix , iy, lpdis->rcItem.right - 4, lpdis->rcItem.bottom);
            ExtTextOut(lpdis->hDC, ix, iy, ETO_CLIPPED|ETO_OPAQUE, &rtTemp, NULL, 0, (LPINT)NULL);
            UINT uFormat = DT_LEFT|DT_EXPANDTABS;
            // only draw accelerators on the submenu!
            // Reason: we only get the WM_MENUCHAR message if the *whole* menu is ownerdrawn,
            // which the top level context menu is *not*. So drawing there the accelerators
            // is futile because they won't get used. 

            // int menuID = it->second.uPosition;

            {
                // If the "hide keyboard cues" option is turned off, we still
                // get the ODS_NOACCEL flag! So we have to check this setting
                // manually too.
                BOOL bShowAccels = FALSE;
#ifndef SPI_GETKEYBOARDCUES
#define SPI_GETKEYBOARDCUES                 0x100A
#endif
#ifndef ODS_NOACCEL
#define ODS_NOACCEL         0x0100
#endif
#ifndef DT_HIDEPREFIX
#define DT_HIDEPREFIX               0x00100000
#endif
                SystemParametersInfo(SPI_GETKEYBOARDCUES, 0, &bShowAccels, 0);
                uFormat |= ((lpdis->itemState & ODS_NOACCEL)&&(!bShowAccels)) ? DT_HIDEPREFIX : 0;
            }

            if (lpdis->itemState & ODS_GRAYED)
            {        
                SetBkMode(lpdis->hDC, TRANSPARENT);
                OffsetRect( &rt, 1, 1 );
                SetTextColor( lpdis->hDC, RGB(255,255,255) );
                DrawText( lpdis->hDC, szItem, lstrlen(szItem), &rt, uFormat );
                OffsetRect( &rt, -1, -1 );
                SetTextColor( lpdis->hDC, RGB(128,128,128) );
                DrawText( lpdis->hDC, szItem, lstrlen(szItem), &rt, uFormat ); 
            }
            else
                DrawText( lpdis->hDC, szItem, lstrlen(szItem), &rt, uFormat );
            *pResult = TRUE;
        }
        break; 
    case WM_MENUCHAR: 
        { 
            LPCTSTR resource = NULL; 
            TCHAR *szItem; 
            if (HIWORD(wParam) != MF_POPUP) 
                return NOERROR; 
            int nChar = LOWORD(wParam); 
            if (_istascii((wint_t)nChar) && _istupper((wint_t)nChar)) 
                nChar = tolower(nChar); 
            // we have the char the user pressed, now search that char in all our 
            // menu items 
            std::vector<int> accmenus; 
            std::map<UINT_PTR, UINT_PTR> mySubMenuMap; 
            std::map<UINT_PTR, UINT_PTR>::iterator It; 
            for (It = mySubMenuMap.begin(); It != mySubMenuMap.end(); ++It) 
            { 
                // resource = GetMenuTextFromResource(mySubMenuMap[It->first]); 
                if (resource == NULL) 
                    continue; 
                szItem = stringtablebuffer; 
                TCHAR * amp = _tcschr(szItem, '&'); 
                if (amp == NULL) 
                    continue; 
                amp++; 
                int ampChar = LOWORD(*amp); 
                if (_istascii((wint_t)ampChar) && _istupper((wint_t)ampChar)) 
                    ampChar = tolower(ampChar); 
                if (ampChar == nChar) 
                { 
                    // yep, we found a menu which has the pressed key 
                    // as an accelerator. Add that menu to the list to 
                    // process later. 
                    accmenus.push_back((int)It->first); 
                } 
            } 
            if (accmenus.size() == 0) 
            { 
                // no menu with that accelerator key. 
                *pResult = MAKELONG(0, MNC_IGNORE); 
                return NOERROR; 
            } 
            if (accmenus.size() == 1) 
            { 
                // Only one menu with that accelerator key. We're lucky! 
                // So just execute that menu entry. 
                *pResult = MAKELONG(accmenus[0], MNC_EXECUTE); 
                return NOERROR; 
            } 
            if (accmenus.size() > 1) 
            { 
                // we have more than one menu item with this accelerator key! 
                MENUITEMINFO mif; 
                mif.cbSize = sizeof(MENUITEMINFO); 
                mif.fMask = MIIM_STATE; 
                for (std::vector<int>::iterator it = accmenus.begin(); it != accmenus.end(); ++it) 
                { 
                    GetMenuItemInfo((HMENU)lParam, *it, TRUE, &mif); 
                    if (mif.fState == MFS_HILITE) 
                    { 
                        // this is the selected item, so select the next one 
                        ++it; 
                        if (it == accmenus.end()) 
                            *pResult = MAKELONG(accmenus[0], MNC_SELECT); 
                        else 
                            *pResult = MAKELONG(*it, MNC_SELECT); 
                        return NOERROR; 
                    } 
                } 
                *pResult = MAKELONG(accmenus[0], MNC_SELECT); 
            } 
        } 
        break; 
    default: 
        return NOERROR; 
    } 

    return NOERROR; 
} 

HBITMAP CAppShellExt::IconToBitmap(UINT uIcon, COLORREF transparentColor) 
{ 
    std::map<UINT, HBITMAP>::iterator bitmap_it = bitmaps.lower_bound(uIcon); 
    if (bitmap_it != bitmaps.end() && bitmap_it->first == uIcon) 
        return bitmap_it->second; 

    HICON hIcon = (HICON)LoadImage(_Module.m_hInstResource, MAKEINTRESOURCE(uIcon), 
        IMAGE_ICON, 12, 12, LR_DEFAULTCOLOR); 
    if (!hIcon) 
        return NULL; 

    RECT     rect; 

    rect.right = ::GetSystemMetrics(SM_CXMENUCHECK); 
    rect.bottom = ::GetSystemMetrics(SM_CYMENUCHECK); 

    rect.left = rect.top  = 0; 

    HWND desktop    = ::GetDesktopWindow(); 
    if (desktop == NULL) 
    { 
        DestroyIcon(hIcon); 
        return NULL; 
    } 

    HDC  screen_dev = ::GetDC(desktop); 
    if (screen_dev == NULL) 
    { 
        DestroyIcon(hIcon); 
        return NULL; 
    } 

    // Create a compatible DC 
    HDC dst_hdc = ::CreateCompatibleDC(screen_dev); 
    if (dst_hdc == NULL) 
    { 
        DestroyIcon(hIcon); 
        ::ReleaseDC(desktop, screen_dev); 
        return NULL; 
    } 

    // Create a new bitmap of icon size 
    HBITMAP bmp = ::CreateCompatibleBitmap(screen_dev, rect.right, rect.bottom); 
    if (bmp == NULL) 
    { 
        DestroyIcon(hIcon); 
        ::DeleteDC(dst_hdc); 
        ::ReleaseDC(desktop, screen_dev); 
        return NULL; 
    } 

    // Select it into the compatible DC 
    HBITMAP old_dst_bmp = (HBITMAP)::SelectObject(dst_hdc, bmp); 
    if (old_dst_bmp == NULL) 
    { 
        DestroyIcon(hIcon); 
        return NULL; 
    } 

    // Fill the background of the compatible DC with the given color 
    ::SetBkColor(dst_hdc, transparentColor); 
    ::ExtTextOut(dst_hdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL); 

    // Draw the icon into the compatible DC 
    ::DrawIconEx(dst_hdc, 0, 0, hIcon, rect.right, rect.bottom, 0, NULL, DI_NORMAL); 

    // Restore settings 
    ::SelectObject(dst_hdc, old_dst_bmp); 
    ::DeleteDC(dst_hdc); 
    ::ReleaseDC(desktop, screen_dev); 
    DestroyIcon(hIcon); 
    if (bmp) 
        bitmaps.insert(bitmap_it, std::make_pair(uIcon, bmp)); 
    return bmp; 
} 

BOOL CAppShellExt::_ExecuteCommand( void )
{
    BOOL bRs = FALSE;
    do 
    {
        TCHAR szExeFile[MAX_PATH]; 
        if (FALSE == FindExeFile(_Module.m_hInst, APPNAME _T(".exe"),
            szExeFile, _countof(szExeFile)))
        { 
            break;
        } 

        ATLASSERT(m_lsFiles.size()); 
        if(0 == m_lsFiles.size()) {
            break;
        }

        TCHAR * lpszFiles = (TCHAR *) LocalAlloc(LPTR, 
            (m_lsFiles.size()*(MAX_PATH+3) + 1)*sizeof(TCHAR)); 
        _tstring_list::iterator it; 
        for(it=m_lsFiles.begin(); it != m_lsFiles.end(); it++) { 
            TCHAR szFileName[MAX_PATH+4] = { 0 }; 
            wsprintf(szFileName, _T("\"%s\" "), it->c_str()); 
            lstrcat(lpszFiles, szFileName); 
        } 

        SHELLEXECUTEINFO ExecuteInfo = { 0 }; 

        ExecuteInfo.cbSize = sizeof(ExecuteInfo); 
        ExecuteInfo.fMask = 0; 
        ExecuteInfo.hwnd = 0; 
        ExecuteInfo.lpVerb = _T("open");        // Operation to perform 
        ExecuteInfo.lpFile = szExeFile;         // Application name 
        ExecuteInfo.lpParameters = lpszFiles;   // Additional parameters 
        ExecuteInfo.lpDirectory = 0;            // Default directory 
        ExecuteInfo.nShow = SW_SHOW; 
        ExecuteInfo.hInstApp = 0; 

        bRs = ShellExecuteEx(&ExecuteInfo);

        LocalFree(lpszFiles); 
    } while (FALSE);
    return bRs;
}

void CAppShellExt::_ShowErrorMessage( void )
{
    TCHAR szFmt[MAX_PATH] = { 0 };
    ::LoadString(_Module.m_hInstResource, IDS_NO_PROG, szFmt, _countof(szFmt));
    TCHAR szOut[MAX_PATH] = { 0 };
    wsprintf(szOut, szFmt, APPNAME);

    TCHAR szTitle[MAX_PATH] = { 0 };
    ::LoadString(_Module.m_hInstResource, IDS_ERROR, szTitle, _countof(szTitle));
    ::MessageBox(::GetActiveWindow(), szOut, szTitle, MB_OK|MB_ICONSTOP); 
}

