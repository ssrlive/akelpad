#include "DialogSwitcher.h"
#include "AkelPad/resource.h"
#include "QSearchDlg.h"
#include "XMemStrFunc.h"


#define  ACCEL_FVIRT_MASK        (FALT | FCONTROL | FSHIFT)
#define  DLG_ACCELKEYS_MAX       10
#define  DLG_ACCELKEYS_REQUIRED  3

#define  DLGTYPE_NONE            0
#define  DLGTYPE_QSEARCH         1
#define  DLGTYPE_FIND            2
#define  DLGTYPE_REPLACE         3
#define  DLGTYPE_GOTO            4
#define  DLGTYPE_UNKNOWN         5
#define  WNDTYPE_MAINEDIT       10


// extern vars
extern QSearchDlgState g_QSearchDlg;
extern wchar_t         g_szFunctionQSearchW[128];


// static (local) vars
static HWND  ds_hMainWnd = NULL;
static BOOL  ds_bOldWindows = FALSE;
static HHOOK ds_hDialogSwitcherHook = NULL;
static ACCEL ds_qsdlgAccelKey;
static ACCEL ds_dlgAccelKeys[DLG_ACCELKEYS_MAX];
static int   ds_nDlgAccelKeys = 0;


// funcs
void dlgswtchInitialize(const PLUGINDATA* pd);
void dlgswtchUninitialize(void);
BOOL dlgswtchIsActive(void);
void dlgswtchSetHookProc(const PLUGINDATA* pd);
void dlgswtchUnsetHookProc(void);
BOOL dlgswtchDoSwitch(int nAccelIndex);
LRESULT CALLBACK dlgswtchHookProc(int nCode, WPARAM wParam, LPARAM lParam);


static DWORD getQSearchHotKey(const PLUGINDATA* pd)
{
    if ( !g_szFunctionQSearchW[0] )
    {
        if ( pd->bOldWindows )
        {
            const char* pszFunc = (const char *) pd->pFunction;
            if ( pszFunc )
            {
                int n = lstrlenA(pszFunc) - 1;
                for ( ; n > 0; n-- )
                {
                    if ( (pszFunc[n] == ':') && (pszFunc[n-1] == ':') )
                    {
                        int i = 0;
                        for ( ; i < n + 1; i++ )
                        {
                            ((char *) g_szFunctionQSearchW)[i] = pszFunc[i];
                        }
                        lstrcpyA( ((char *) g_szFunctionQSearchW) + i, "QSearch" );
                        break;
                    }
                }
            }
        }
        else
        {
            const wchar_t* pszFunc = (const wchar_t *) pd->pFunction;
            if ( pszFunc )
            {
                int n = lstrlenW(pszFunc) - 1;
                for ( ; n > 0; n-- )
                {
                    if ( (pszFunc[n] == L':') && (pszFunc[n-1] == L':') )
                    {
                        int i = 0;
                        for ( ; i < n + 1; i++ )
                        {
                            g_szFunctionQSearchW[i] = pszFunc[i];
                        }
                        lstrcpyW( g_szFunctionQSearchW + i, L"QSearch" );
                        break;
                    }
                }
            }
        }
    }

    if ( g_szFunctionQSearchW[0] )
    {
        if ( pd->bOldWindows )
        {
            PLUGINFUNCTION* pfA;

            pfA = (PLUGINFUNCTION *) SendMessage( pd->hMainWnd, 
              AKD_DLLFINDA, (WPARAM) g_szFunctionQSearchW, 0 );
            if ( pfA )
            {
                return pfA->wHotkey;
            }
        }
        else
        {
            PLUGINFUNCTION* pfW;

            pfW = (PLUGINFUNCTION *) SendMessage( pd->hMainWnd, 
              AKD_DLLFINDW, (WPARAM) g_szFunctionQSearchW, 0 );
            if ( pfW )
            {
                return pfW->wHotkey;
            }
        }
    }

    return 0;
}


void initAccelFromKeyboard(ACCEL* pAccelKey, const WCHAR wKey)
{
    pAccelKey->key = wKey & 0xFF;
    pAccelKey->fVirt = 0;
    if ( GetKeyState(VK_MENU) & 0x80 )
        pAccelKey->fVirt |= FALT;
    if ( GetKeyState(VK_CONTROL) & 0x80 )
        pAccelKey->fVirt |= FCONTROL;
    if ( GetKeyState(VK_SHIFT) & 0x80 )
        pAccelKey->fVirt |= FSHIFT;
}

void initAccelFromPluginHotkey(ACCEL* pAccelKey, const DWORD dwHotKey)
{
    pAccelKey->key = LOBYTE(dwHotKey);
    pAccelKey->fVirt = 0;
    if ( HIBYTE(dwHotKey) & HOTKEYF_ALT )
        pAccelKey->fVirt |= FALT;
    if ( HIBYTE(dwHotKey) & HOTKEYF_CONTROL )
        pAccelKey->fVirt |= FCONTROL;
    if ( HIBYTE(dwHotKey) & HOTKEYF_SHIFT )
        pAccelKey->fVirt |= FSHIFT;
}

BOOL isAccelKey(const ACCEL* pAccelKey1, const ACCEL* pAccelKey2)
{
    if ( ((pAccelKey1->key & 0xFF) == (pAccelKey2->key & 0xFF)) &&
         ((pAccelKey1->fVirt & ACCEL_FVIRT_MASK) == (pAccelKey2->fVirt & ACCEL_FVIRT_MASK)) )
    {
        return TRUE;
    }
    return FALSE;
}


void dlgswtchInitialize(const PLUGINDATA* pd)
{
    /*HMODULE hLangLib = NULL;*/

    ds_hMainWnd = pd->hMainWnd;
    ds_bOldWindows = pd->bOldWindows;

    /*if ( pd->bOldWindows )
    {
        const char* pszLangModule = (const char *) pd->pLangModule;
        if ( pszLangModule && pszLangModule[0] )
        {
            char szLangLibPath[MAX_PATH + 1];

            lstrcpyA( szLangLibPath, (const char *) pd->pAkelDir );
            lstrcatA( szLangLibPath, "\\AkelFiles\\Langs\\" );
            lstrcatA( szLangLibPath, pszLangModule );

            hLangLib = LoadLibraryA(szLangLibPath);
        }
    }
    else
    {
        const wchar_t* pszLangModule = (const wchar_t *) pd->pLangModule;
        if ( pszLangModule && pszLangModule[0] )
        {
            wchar_t szLangLibPath[2*MAX_PATH + 1];

            lstrcpyW( szLangLibPath, (const wchar_t *) pd->pAkelDir );
            lstrcatW( szLangLibPath, L"\\AkelFiles\\Langs\\" );
            lstrcatW( szLangLibPath, pszLangModule );

            hLangLib = LoadLibraryW(szLangLibPath);
        }
    }

    if ( !hLangLib )
        hLangLib = pd->hInstanceEXE;*/

    if ( pd->bOldWindows )
    {
        HACCEL hAccelTable = pd->hMainAccel; /*LoadAcceleratorsA(hLangLib, MAKEINTRESOURCEA(IDA_ACCEL_MAIN));*/
        if ( hAccelTable )
        {
            int nAccels = CopyAcceleratorTableA(hAccelTable, NULL, 0);
            if ( nAccels > 0 )
            {
                ACCEL* pAccels = (ACCEL *) SysMemAlloc( nAccels*sizeof(ACCEL) );
                if ( pAccels )
                {
                    if ( CopyAcceleratorTableA(hAccelTable, pAccels, nAccels) == nAccels )
                    {
                        int i;

                        for ( i = 0; i < nAccels; i++ )
                        {
                            switch ( pAccels[i].cmd )
                            {
                                case IDM_EDIT_FIND:
                                case IDM_EDIT_REPLACE:
                                case IDM_EDIT_GOTO:
                                    if ( ds_nDlgAccelKeys < DLG_ACCELKEYS_MAX )
                                    {
                                        ds_dlgAccelKeys[ds_nDlgAccelKeys++] = pAccels[i];
                                    }
                                    break;
                            }
                        }
                    }
                    SysMemFree( pAccels );
                }
            }
            /*DestroyAcceleratorTable(hAccelTable);*/
        }
    }
    else
    {
        HACCEL hAccelTable = pd->hMainAccel; /*LoadAcceleratorsW(hLangLib, MAKEINTRESOURCEW(IDA_ACCEL_MAIN));*/
        if ( hAccelTable )
        {
            int nAccels = CopyAcceleratorTableW(hAccelTable, NULL, 0);
            if ( nAccels > 0 )
            {
                ACCEL* pAccels = (ACCEL *) SysMemAlloc( nAccels*sizeof(ACCEL) );
                if ( pAccels )
                {
                    if ( CopyAcceleratorTableW(hAccelTable, pAccels, nAccels) == nAccels )
                    {
                        int i;

                        for ( i = 0; i < nAccels; i++ )
                        {
                            switch ( pAccels[i].cmd )
                            {
                                case IDM_EDIT_FIND:
                                case IDM_EDIT_REPLACE:
                                case IDM_EDIT_GOTO:
                                    if ( ds_nDlgAccelKeys < DLG_ACCELKEYS_MAX )
                                    {
                                        ds_dlgAccelKeys[ds_nDlgAccelKeys++] = pAccels[i];
                                    }
                                    break;
                            }
                        }
                    }
                    SysMemFree( pAccels );
                }
            }
            /*DestroyAcceleratorTable(hAccelTable);*/
        }
    }

    /*if ( hLangLib != pd->hInstanceEXE )
    {
        FreeLibrary(hLangLib);
    }*/

    if ( ds_nDlgAccelKeys >= DLG_ACCELKEYS_REQUIRED )
    {
        DWORD dwHotKey = getQSearchHotKey(pd);
        initAccelFromPluginHotkey( &ds_qsdlgAccelKey, dwHotKey );

        dlgswtchSetHookProc(pd);
    }

    if ( !ds_hDialogSwitcherHook )
    {
        ds_nDlgAccelKeys = 0;

        MessageBoxA(
          pd->hMainWnd, 
          "Can not initialize the Dialog Switcher",
          "QSearch ERROR", 
          MB_OK | MB_ICONERROR
        );
    }
}

void dlgswtchUninitialize(void)
{
    dlgswtchUnsetHookProc();
    ds_nDlgAccelKeys = 0;
}

BOOL dlgswtchIsActive(void)
{
    return (ds_hDialogSwitcherHook ? TRUE : FALSE);
}

void dlgswtchSetHookProc(const PLUGINDATA* pd)
{
    if ( !ds_hDialogSwitcherHook )
    {
        if ( pd->bOldWindows )
        {
            ds_hDialogSwitcherHook = SetWindowsHookExA(WH_GETMESSAGE, 
                dlgswtchHookProc, pd->hInstanceDLL, GetCurrentThreadId());
        }
        else
        {
            ds_hDialogSwitcherHook = SetWindowsHookExW(WH_GETMESSAGE, 
                dlgswtchHookProc, pd->hInstanceDLL, GetCurrentThreadId());
        }
    }

}

void dlgswtchUnsetHookProc(void)
{
    if ( ds_hDialogSwitcherHook )
    {
        UnhookWindowsHookEx(ds_hDialogSwitcherHook);
        ds_hDialogSwitcherHook = NULL;
    }
}

#define MAX_FIND_TEXT_TO_CHECK 16384
static BOOL isFindTextPickedUpFromMainEdit(HWND hFindTextWnd, HWND hMainEditWnd)
{
    CHARRANGE_X cr = { 0, 0 };

    SendMessage( hMainEditWnd, EM_EXGETSEL_X, 0, (LPARAM) &cr );
    if ( cr.cpMin != cr.cpMax )
    {
        INT_X x;
        INT_X nSelTextLen;

        if ( cr.cpMin > cr.cpMax )
        {
            x = cr.cpMin;
            cr.cpMin = cr.cpMax;
            cr.cpMax = x;
        }
        
        nSelTextLen = cr.cpMax - cr.cpMin;
        if ( nSelTextLen > MAX_FIND_TEXT_TO_CHECK - 1 )
        {
            nSelTextLen = MAX_FIND_TEXT_TO_CHECK - 1;
            cr.cpMax = cr.cpMin + nSelTextLen;
        }

        if ( ds_bOldWindows )
        {
            x = (INT_X) SendMessageA( hFindTextWnd, WM_GETTEXTLENGTH, 0, 0 );
            if ( x == nSelTextLen )
            {
                BOOL         bResult;
                char*        pFindTextA;
                TEXTRANGEA_X trA;

                bResult = FALSE;
                pFindTextA = (char *) SysMemAlloc( nSelTextLen + 1 );
                if ( pFindTextA )
                {
                    trA.chrg.cpMin = cr.cpMin;
                    trA.chrg.cpMax = cr.cpMax;
                    trA.lpstrText = (char *) SysMemAlloc( nSelTextLen + 1 );
                    if ( trA.lpstrText )
                    {
                        trA.lpstrText[0] = 0;
                        SendMessageA( hMainEditWnd, EM_GETTEXTRANGE_X, 0, (LPARAM) &trA );
                        pFindTextA[0] = 0;
                        SendMessageA( hFindTextWnd, WM_GETTEXT, nSelTextLen + 1, (LPARAM) pFindTextA );
                        if ( lstrcmpA(trA.lpstrText, pFindTextA) == 0 )
                        {
                            bResult = TRUE;
                        }
                        SysMemFree( trA.lpstrText );
                    }
                    SysMemFree( pFindTextA );
                }
                return bResult;
            }
        }
        else
        {
            x = (INT_X) SendMessageW( hFindTextWnd, WM_GETTEXTLENGTH, 0, 0 );
            if ( x == nSelTextLen )
            {
                BOOL         bResult;
                wchar_t*     pFindTextW;
                TEXTRANGEW_X trW;

                bResult = FALSE;
                pFindTextW = (wchar_t *) SysMemAlloc( (nSelTextLen + 1)*sizeof(wchar_t) );
                if ( pFindTextW )
                {
                    trW.chrg.cpMin = cr.cpMin;
                    trW.chrg.cpMax = cr.cpMax;
                    trW.lpstrText = (wchar_t *) SysMemAlloc( (nSelTextLen + 1)*sizeof(wchar_t) );
                    if ( trW.lpstrText )
                    {
                        trW.lpstrText[0] = 0;
                        SendMessageW( hMainEditWnd, EM_GETTEXTRANGE_X, 0, (LPARAM) &trW );
                        pFindTextW[0] = 0;
                        SendMessageW( hFindTextWnd, WM_GETTEXT, nSelTextLen + 1, (LPARAM) pFindTextW );
                        if ( lstrcmpW(trW.lpstrText, pFindTextW) == 0 )
                        {
                            bResult = TRUE;
                        }
                        SysMemFree( trW.lpstrText );
                    }
                    SysMemFree( pFindTextW );
                }
                return bResult;
            }
        }
    }

    return FALSE;
}

BOOL dlgswtchDoSwitch(int nAccelIndex)
{
    //wchar_t  szText[MAX_TEXT_SIZE];
    int      nTextLen;
    wchar_t* pszTextW;
    char*    pszTextA;
    EDITINFO ei;
    RECT     rcEditUpdate;
    HWND     hModelessDlg;
    HWND     hFocusedWnd;
    BOOL     bSetText;
    BOOL     bQSearchDlgIsVisible;
    BOOL     bQSearchDlgIsFocused;
    BOOL     bModelessDlgIsFocused;
    int      nCurrentDlgType;
    int      nSwitchDlgType;
    int      nFocusedWndType;

    nSwitchDlgType = DLGTYPE_NONE;
    if ( (nAccelIndex >= 0) && (nAccelIndex < ds_nDlgAccelKeys) )
    {
        switch ( ds_dlgAccelKeys[nAccelIndex].cmd )
        {
            case IDM_EDIT_FIND:
                nSwitchDlgType = DLGTYPE_FIND;
                break;

            case IDM_EDIT_REPLACE:
                nSwitchDlgType = DLGTYPE_REPLACE;
                break;

            case IDM_EDIT_GOTO:
                nSwitchDlgType = DLGTYPE_GOTO;
                break;
        }
    }
    else
    {
        nSwitchDlgType = DLGTYPE_QSEARCH;
    }

    if ( nSwitchDlgType == DLGTYPE_NONE )
        return FALSE;

    nFocusedWndType = DLGTYPE_NONE;
    nCurrentDlgType = DLGTYPE_NONE;
    bModelessDlgIsFocused = FALSE;
    bQSearchDlgIsFocused = FALSE;
    bQSearchDlgIsVisible = FALSE;
    if ( g_QSearchDlg.hDlg && IsWindowVisible(g_QSearchDlg.hDlg) )
        bQSearchDlgIsVisible = TRUE;
    hFocusedWnd = GetFocus();
    hModelessDlg = (HWND) SendMessage(ds_hMainWnd, AKD_GETMODELESS, 0, (LPARAM)&nCurrentDlgType);
    if ( hModelessDlg )
    {
        if ( nCurrentDlgType == MLT_REPLACE )
            nCurrentDlgType = DLGTYPE_REPLACE;
        else if ( nCurrentDlgType == MLT_FIND )
            nCurrentDlgType = DLGTYPE_FIND;
        else if ( nCurrentDlgType == MLT_GOTO )
            nCurrentDlgType = DLGTYPE_GOTO;
        else
            nCurrentDlgType = DLGTYPE_UNKNOWN;

        if ( (hModelessDlg == hFocusedWnd) || IsChild(hModelessDlg, hFocusedWnd) )
        {
            bModelessDlgIsFocused = TRUE;
            nFocusedWndType = nCurrentDlgType;
        }
    }
    if ( bQSearchDlgIsVisible )
    {
        if ( (g_QSearchDlg.hDlg == hFocusedWnd) || IsChild(g_QSearchDlg.hDlg, hFocusedWnd) )
        {
            bQSearchDlgIsFocused = TRUE;
            nFocusedWndType = DLGTYPE_QSEARCH;
        }

        if ( nCurrentDlgType == DLGTYPE_NONE )
            nCurrentDlgType = DLGTYPE_QSEARCH;
    }

    ei.hWndEdit = NULL;
    SendMessage( ds_hMainWnd, AKD_GETEDITINFO, 0, (LPARAM) &ei );

    if ( (!bModelessDlgIsFocused) && (!bQSearchDlgIsFocused) )
    {
        if ( ei.hWndEdit != hFocusedWnd )
            return FALSE;
    }

    if ( nFocusedWndType == DLGTYPE_NONE )
    {
        if ( ei.hWndEdit == hFocusedWnd )
            nFocusedWndType = WNDTYPE_MAINEDIT;
    }

    if ( (nCurrentDlgType == DLGTYPE_NONE) ||  
         ((!bQSearchDlgIsFocused) && (!hModelessDlg)) ||
         ((!bModelessDlgIsFocused) && (nSwitchDlgType == DLGTYPE_QSEARCH)) ||
         ((nSwitchDlgType == nCurrentDlgType) && (!bQSearchDlgIsFocused)) )
    {
        if ( hModelessDlg && 
             (!bModelessDlgIsFocused) && 
             (nCurrentDlgType == nSwitchDlgType) )
        {
            HWND hItem;

            switch ( nSwitchDlgType )
            {
                case DLGTYPE_FIND:
                case DLGTYPE_REPLACE:
                    if ( hItem = GetDlgItem(hModelessDlg, IDC_SEARCH_FIND) )
                    {
                        if ( !IsWindowVisible(hModelessDlg) )
                            ShowWindow(hModelessDlg, SW_SHOW);
                        SetFocus(hItem);
                    }
                    break;

                case DLGTYPE_GOTO:
                    if ( hItem = GetDlgItem(hModelessDlg, IDC_GOTO_NUMBER) )
                    {
                        if ( !IsWindowVisible(hModelessDlg) )
                            ShowWindow(hModelessDlg, SW_SHOW);
                        SetFocus(hItem);
                    }
                    break;
            }
        }
        return FALSE;
    }

    bSetText = FALSE;

    SetRectEmpty(&rcEditUpdate);
    HideCaret(ei.hWndEdit);

    if ( bQSearchDlgIsFocused ||
         (nSwitchDlgType == DLGTYPE_QSEARCH) ||
         (nCurrentDlgType == DLGTYPE_QSEARCH) )
    {
        nTextLen = MAX_TEXT_SIZE - 1;
        if ( ds_bOldWindows )
        {
            pszTextW = NULL;
            pszTextA = (char *) SysMemAlloc( nTextLen + 1 );
        }
        else
        {
            pszTextA = NULL;
            pszTextW = (wchar_t *) SysMemAlloc( (nTextLen + 1)*sizeof(wchar_t) );
        }
    }
    else
    {
        nTextLen = 0;
        pszTextA = NULL;
        pszTextW = NULL;
    }

    switch ( nCurrentDlgType )
    {
        case DLGTYPE_FIND:
        case DLGTYPE_REPLACE:
            if ( bModelessDlgIsFocused || !bQSearchDlgIsFocused )
            {
                HWND hFindTextWnd = GetDlgItem(hModelessDlg, IDC_SEARCH_FIND);

                bSetText = TRUE;
                if ( ds_bOldWindows )
                {
                    if ( !pszTextA )
                    {
                        nTextLen = GetWindowTextLengthA(hFindTextWnd);
                        if ( nTextLen > 0 )
                        {
                            ++nTextLen;
                            pszTextA = (char *) SysMemAlloc( nTextLen + 1 );
                        }
                    }
                    if ( pszTextA )
                    {
                        pszTextA[0] = 0;
                        GetWindowTextA(hFindTextWnd, pszTextA, nTextLen);
                    }
                }
                else
                {
                    if ( !pszTextW )
                    {
                        nTextLen = GetWindowTextLengthW(hFindTextWnd);
                        if ( nTextLen > 0 )
                        {
                            ++nTextLen;
                            pszTextW = (wchar_t *) SysMemAlloc( (nTextLen + 1)*sizeof(wchar_t) );
                        }
                    }
                    if ( pszTextW )
                    {
                        pszTextW[0] = 0;
                        GetWindowTextW(hFindTextWnd, pszTextW, nTextLen);
                    }
                }
            }
        case DLGTYPE_GOTO:
        case DLGTYPE_UNKNOWN:
            if ( (nSwitchDlgType != DLGTYPE_QSEARCH) ||
                 (bModelessDlgIsFocused && !bQSearchDlgIsVisible) ||
                 (!IsWindowVisible(hModelessDlg)) )
            {
                if ( IsWindowVisible(hModelessDlg) )
                {
                    RECT rcEdit = { 0, 0, 0, 0 };
                    RECT rcDlg = { 0, 0, 0, 0 };

                    GetWindowRect(ei.hWndEdit, &rcEdit);
                    GetWindowRect(hModelessDlg, &rcDlg);
                    if ( IntersectRect(&rcEditUpdate, &rcEdit, &rcDlg) )
                    {
                        rcEditUpdate.left -= (rcEdit.left + 2);
                        rcEditUpdate.right -= (rcEdit.left - 2);
                        rcEditUpdate.top -= (rcEdit.top + 2);
                        rcEditUpdate.bottom -= (rcEdit.top - 2);
                    }
                }
                SendMessage(ei.hWndEdit, WM_SETREDRAW, FALSE, 0);
                SendMessage(hModelessDlg, WM_COMMAND, IDCANCEL, 0);
            }
        case DLGTYPE_QSEARCH:
            if ( bQSearchDlgIsFocused )
            {
                HWND hFindTextWnd = g_QSearchDlg.hFindEdit;

                bSetText = TRUE;
                if ( ds_bOldWindows )
                {
                    if ( pszTextA )
                    {
                        pszTextA[0] = 0;
                        SendMessageA( hFindTextWnd, WM_GETTEXT, nTextLen, (LPARAM) pszTextA );
                    }
                }
                else
                {
                    if ( pszTextW )
                    {
                        pszTextW[0] = 0;
                        SendMessageW( hFindTextWnd, WM_GETTEXT, nTextLen, (LPARAM) pszTextW );
                    }
                }

                if ( nCurrentDlgType == DLGTYPE_QSEARCH )
                    SendMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, FALSE, 0 );
            }
            break;
    }

    switch ( nSwitchDlgType )
    {
        case DLGTYPE_FIND:
        case DLGTYPE_REPLACE:
        case DLGTYPE_GOTO:
            SendMessage(ds_hMainWnd, WM_COMMAND, ds_dlgAccelKeys[nAccelIndex].cmd, 0);
            hModelessDlg = (HWND) SendMessage(ds_hMainWnd, AKD_GETMODELESS, 0, 0);
            if ( bSetText && (nSwitchDlgType != DLGTYPE_GOTO) )
            {
                HWND hFindTextWnd = GetDlgItem(hModelessDlg, IDC_SEARCH_FIND);
                if ( ds_bOldWindows )
                {
                    if ( (nFocusedWndType != WNDTYPE_MAINEDIT) || 
                         (!isFindTextPickedUpFromMainEdit(hFindTextWnd, ei.hWndEdit)) )
                    {
                        if ( pszTextA && pszTextA[0] )
                            SendMessageA( hFindTextWnd, WM_SETTEXT, 0, (LPARAM) pszTextA );
                        SendMessageA( hFindTextWnd, CB_SETEDITSEL, 0, 0xFFFF0000L );
                    }
                }
                else
                {
                    if ( (nFocusedWndType != WNDTYPE_MAINEDIT) || 
                         (!isFindTextPickedUpFromMainEdit(hFindTextWnd, ei.hWndEdit)) )
                    {
                        if ( pszTextW && pszTextW[0] )
                            SendMessageW( hFindTextWnd, WM_SETTEXT, 0, (LPARAM) pszTextW );
                        SendMessageW( hFindTextWnd, CB_SETEDITSEL, 0, 0xFFFF0000L );
                    }
                }
            }
            break;

        case DLGTYPE_QSEARCH:
            SendMessage(ei.hWndEdit, WM_SETREDRAW, TRUE, 0);
            if ( g_QSearchDlg.hDlg )
            {
                SendMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, TRUE, 0 ); // forbid to pick up selected text
            }
            else
            {
                g_QSearchDlg.uWmShowFlags = QS_SF_DLGSWITCH;

                if ( ds_bOldWindows )
                {
                    PLUGINCALLSENDA pcsA;

                    pcsA.pFunction = (char *) g_szFunctionQSearchW;
                    //pcsA.bOnStart = FALSE;
                    pcsA.lParam = 0;
                    pcsA.dwSupport = 0;

                    SendMessageA( ds_hMainWnd, AKD_DLLCALLA, 0, (LPARAM) &pcsA );
                }
                else
                {
                    PLUGINCALLSENDW pcsW;

                    pcsW.pFunction = g_szFunctionQSearchW;
                    //pcsW.bOnStart = FALSE;
                    pcsW.lParam = 0;
                    pcsW.dwSupport = 0;

                    SendMessageW( ds_hMainWnd, AKD_DLLCALLW, 0, (LPARAM) &pcsW );
                }

                g_QSearchDlg.uWmShowFlags = 0;
            }
            if ( bSetText && g_QSearchDlg.hDlg )
            {
                HWND hFindTextWnd = g_QSearchDlg.hFindEdit;
                if ( ds_bOldWindows )
                {
                    if ( pszTextA && pszTextA[0] )
                        SendMessageA( hFindTextWnd, WM_SETTEXT, 0, (LPARAM) pszTextA );
                    SendMessageA( hFindTextWnd, EM_SETSEL, 0, -1 );
                }
                else
                {
                    if ( pszTextW && pszTextW[0] )
                        SendMessageW( hFindTextWnd, WM_SETTEXT, 0, (LPARAM) pszTextW );
                    SendMessageW( hFindTextWnd, EM_SETSEL, 0, -1 );
                }
                SendMessage( g_QSearchDlg.hDlg, QSN_DLGSWITCH, TRUE, 0 );
            }
            break;
    }

    if ( ds_bOldWindows )
    {
        if ( pszTextA )
            SysMemFree( pszTextA );
    }
    else
    {
        if ( pszTextW )
            SysMemFree( pszTextW );
    }

    SendMessage(ei.hWndEdit, WM_SETREDRAW, TRUE, 0);
    if ( !IsRectEmpty(&rcEditUpdate) )
    {
        InvalidateRect(ei.hWndEdit, &rcEditUpdate, TRUE);
        UpdateWindow(ei.hWndEdit);
    }
    ShowCaret(ei.hWndEdit);

    return TRUE;
}

LRESULT CALLBACK dlgswtchHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if ( (nCode == HC_ACTION) || (nCode == HC_NOREMOVE) )
    {
        MSG* pmsg = (MSG *) lParam;
        switch ( pmsg->message )
        {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            //case WM_CHAR: // AkelPad uses only virtual-key codes
            {
                ACCEL accelKey;

                initAccelFromKeyboard( &accelKey, LOWORD(pmsg->wParam) );
                if ( isAccelKey(&accelKey, &ds_qsdlgAccelKey) )
                {
                    if ( dlgswtchDoSwitch(ds_nDlgAccelKeys) )
                    {
                        pmsg->message = WM_NULL;
                    }
                }
                else
                {
                    int i;

                    for ( i = 0; i < ds_nDlgAccelKeys; i++ )
                    {
                        if ( isAccelKey(&accelKey, &ds_dlgAccelKeys[i]) )
                        {
                            if ( dlgswtchDoSwitch(i) )
                            {
                                pmsg->message = WM_NULL;
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }
    }

    return CallNextHookEx(ds_hDialogSwitcherHook, nCode, wParam, lParam);
}
