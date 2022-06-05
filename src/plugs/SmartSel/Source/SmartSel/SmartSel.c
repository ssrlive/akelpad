#include "SmartSel.h"


/* >>>>>>>>>>>>>>>>>>>>>>>> selection state >>>>>>>>>>>>>>>>>>>>>>>> */
typedef struct tSelectionState {
    BOOL        bMouseLButtonDown;
    BOOL        bShiftDown;
    BOOL        bColumnSelection;
    BOOL        bSpecialAction;
    CHARRANGE64 SelKeyDownRange;
} SelectionState;

void clearSelectionState(SelectionState* pSelState)
{
    pSelState->bMouseLButtonDown = FALSE;
    pSelState->bShiftDown = FALSE;
    pSelState->bColumnSelection = FALSE;
    pSelState->bSpecialAction = FALSE;
    pSelState->SelKeyDownRange.cpMin = 0;
    pSelState->SelKeyDownRange.cpMax = 0;
}

void initializeSelectionState(SelectionState* pSelState)
{
    clearSelectionState(pSelState);
}
/* <<<<<<<<<<<<<<<<<<<<<<<< selection state <<<<<<<<<<<<<<<<<<<<<<<< */


/* >>>>>>>>>>>>>>>>>>>>>>>> plugin state >>>>>>>>>>>>>>>>>>>>>>>> */
typedef struct tPluginState {
    DWORD        dwInitializedMode;
    HWND         hMainWnd;
    BOOL         bOldWindows;
    BOOL         bOldRichEdit; // TRUE means Rich Edit 2.0
    BOOL         nMDI;
    BOOL         bAkelEdit;
    WNDPROCDATA* pMainProcData;
    WNDPROCDATA* pEditProcData;
    WNDPROCDATA* pFrameProcData;
} PluginState;

void initializePluginState(PluginState* pPlugin)
{
    pPlugin->dwInitializedMode = 0;
    pPlugin->bAkelEdit = FALSE;
    pPlugin->hMainWnd = NULL;
    pPlugin->pMainProcData = NULL;
    pPlugin->pEditProcData = NULL;
    pPlugin->pFrameProcData = NULL;
}
/* <<<<<<<<<<<<<<<<<<<<<<<< plugin state <<<<<<<<<<<<<<<<<<<<<<<< */


/* >>>>>>>>>>>>>>>>>>>>>>>> data buffer >>>>>>>>>>>>>>>>>>>>>>>> */
typedef struct tDataBuffer {
    void* pData;      // pointer to the data
    int   nDataSize;  // data size in bytes
} DataBuffer;

void bufInitialize(DataBuffer* pBuf)
{
    pBuf->pData = NULL;
    pBuf->nDataSize = 0;
}

void bufFree(DataBuffer* pBuf)
{
    if ( pBuf->pData )
    {
        GlobalFree( (HGLOBAL) pBuf->pData );
        pBuf->pData = NULL;
    }
    pBuf->nDataSize = 0;
}

void bufFreeIfSizeExceeds(DataBuffer* pBuf, int nMaxDataSizeLimit)
{
    if ( pBuf->pData )
    {
        if ( pBuf->nDataSize > nMaxDataSizeLimit )
        {
            GlobalFree( (HGLOBAL) pBuf->pData );
            pBuf->pData = NULL;
            pBuf->nDataSize = 0;
        }
    }
    else
    {
        pBuf->nDataSize = 0;
    }
}

BOOL bufReserve(DataBuffer* pBuf, int nBytesToReserve)
{
    if ( pBuf->nDataSize < nBytesToReserve )
    {
        bufFree(pBuf);

        nBytesToReserve = (1 + nBytesToReserve/64)*64;

        if ( pBuf->pData = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, nBytesToReserve) )
            pBuf->nDataSize = nBytesToReserve;
        else
            return FALSE;
    }
    return TRUE;
}
/* <<<<<<<<<<<<<<<<<<<<<<<< data buffer <<<<<<<<<<<<<<<<<<<<<<<< */


// consts
const char* cszPluginName = "SmartSel Plugin";
const char* cszUnloadPluginQuestion = "Plugin is active. Unload it?";


// vars
PluginState    g_Plugin;
SelectionState g_SelState;
DataBuffer     g_TextBuf;
BOOL           g_bSmartEndMode2    = FALSE;
BOOL           g_bNoSelEOLMode2    = FALSE;
BOOL           g_bSmartUpDownMode2 = FALSE;
BOOL           g_bSmartHomeMode2   = FALSE;


#define  MODE_NOSELEOL        0x01
#define  MODE_SMARTEND        0x02
#define  MODE_SMARTHOME       0x04
#define  MODE_SMARTUPDOWN     0x08
#define  MODE_SMARTBACKSPACE  0x10

#define  MODE_ALL          (MODE_NOSELEOL | MODE_SMARTEND | MODE_SMARTHOME | MODE_SMARTUPDOWN | MODE_SMARTBACKSPACE)

#define  MAX_TEXTBUF_SIZE  (16*1024)

// funcs
void Initialize(PLUGINDATA* pd, DWORD dwMode);
void Uninitialize(DWORD dwMode);
LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnEditSelKeyDown(HWND hEdit, unsigned int uKey);
void OnEditSelKeyUp(HWND hEdit, unsigned int uKey);
void OnEditSelectionChanged(HWND hEdit, CHARRANGE64* cr);
BOOL OnEditEndKeyDown(HWND hEdit, LPARAM lParam);
BOOL OnEditBackspaceKeyDown(HWND hEdit, LPARAM lParam);
BOOL OnEditHomeKeyDown(HWND hEdit, LPARAM lParam);
BOOL OnEditArrowDownOrUpKeyDown(HWND hEdit, WPARAM wKey);
BOOL SmartHomeA(HWND hEdit);
BOOL SmartHomeW(HWND hEdit);
BOOL GetLineSpaces(AECHARINDEX *ciFirstCharInLine, int nWrapCharInLine, int nTabStopSize, int *lpnLineSpaces);
void CheckEditNotification(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


// Identification
/* extern "C" */
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
    pv->dwAkelDllVersion  = AKELDLL;
    pv->dwExeMinVersion3x = MAKE_IDENTIFIER(-1, -1, -1, -1);
    pv->dwExeMinVersion4x = MAKE_IDENTIFIER(4, 8, 8, 0);
    pv->pPluginName = "SmartSel";
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) NoSelEOL(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    // Is plugin already loaded?
    if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
    {
        Uninitialize(MODE_NOSELEOL);
        pd->nUnload = UD_NONUNLOAD_NONACTIVE;
        return;
    }
    else
    {
        Initialize(pd, MODE_NOSELEOL);
    }

    // Stay in memory, and show as active
    pd->nUnload = UD_NONUNLOAD_ACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) SmartBackspace(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    // Is plugin already loaded?
    if ( g_Plugin.dwInitializedMode & MODE_SMARTBACKSPACE )
    {
        Uninitialize(MODE_SMARTBACKSPACE);
        pd->nUnload = UD_NONUNLOAD_NONACTIVE;
        return;
    }
    else
    {
        Initialize(pd, MODE_SMARTBACKSPACE);
    }

    // Stay in memory, and show as active
    pd->nUnload = UD_NONUNLOAD_ACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) SmartHome(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    // Is plugin already loaded?
    if ( g_Plugin.dwInitializedMode & MODE_SMARTHOME )
    {
        Uninitialize(MODE_SMARTHOME);
        pd->nUnload = UD_NONUNLOAD_NONACTIVE;
        return;
    }
    else
    {
        Initialize(pd, MODE_SMARTHOME);
    }

    // Stay in memory, and show as active
    pd->nUnload = UD_NONUNLOAD_ACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) SmartEnd(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    // Is plugin already loaded?
    if ( g_Plugin.dwInitializedMode & MODE_SMARTEND )
    {
        Uninitialize(MODE_SMARTEND);
        pd->nUnload = UD_NONUNLOAD_NONACTIVE;
        return;
    }
    else
    {
        Initialize(pd, MODE_SMARTEND);
    }

    // Stay in memory, and show as active
    pd->nUnload = UD_NONUNLOAD_ACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) SmartUpDown(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    // Is plugin already loaded?
    if ( g_Plugin.dwInitializedMode & MODE_SMARTUPDOWN )
    {
        Uninitialize(MODE_SMARTUPDOWN);
        pd->nUnload = UD_NONUNLOAD_NONACTIVE;
        return;
    }
    else
    {
        Initialize(pd, MODE_SMARTUPDOWN);
    }

    // Stay in memory, and show as active
    pd->nUnload = UD_NONUNLOAD_ACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) altNoSelEOL(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    g_bNoSelEOLMode2 = !g_bNoSelEOLMode2;

    pd->nUnload = g_bNoSelEOLMode2 ? UD_NONUNLOAD_ACTIVE : UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) altSmartEnd(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    g_bSmartEndMode2 = !g_bSmartEndMode2;

    pd->nUnload = g_bSmartEndMode2 ? UD_NONUNLOAD_ACTIVE : UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) altSmartUpDown(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    g_bSmartUpDownMode2 = !g_bSmartUpDownMode2;

    pd->nUnload = g_bSmartUpDownMode2 ? UD_NONUNLOAD_ACTIVE : UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
/* extern "C" */
void __declspec(dllexport) altSmartHome(PLUGINDATA *pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    g_bSmartHomeMode2 = !g_bSmartHomeMode2;

    pd->nUnload = g_bSmartHomeMode2 ? UD_NONUNLOAD_ACTIVE : UD_NONUNLOAD_NONACTIVE;
}

// Entry point
/* extern "C" */
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD dwReason, LPVOID lpReserved)
{
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH:
            initializePluginState(&g_Plugin);
            initializeSelectionState(&g_SelState);
            bufInitialize(&g_TextBuf);
            break;
        case DLL_PROCESS_DETACH:
            Uninitialize(MODE_ALL);
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        default:
            break;
    }
    return TRUE;
}

void Initialize(PLUGINDATA* pd, DWORD dwMode)
{
    g_Plugin.hMainWnd     = pd->hMainWnd;
    g_Plugin.bOldWindows  = pd->bOldWindows;
    g_Plugin.bOldRichEdit = pd->bOldRichEdit;
    g_Plugin.nMDI         = pd->nMDI;
    g_Plugin.bAkelEdit    = pd->bAkelEdit;

    // dwMode mask (depends on possible mode flags)
    dwMode &= (MODE_ALL);

    if ( dwMode & MODE_NOSELEOL )
    {
        // this SubClassing is required
        // only for MODE_NOSELEOL
        if ( (g_Plugin.dwInitializedMode & MODE_NOSELEOL) == 0 )
        {
            clearSelectionState(&g_SelState);

            // SubClass main window
            g_Plugin.pMainProcData = NULL;
            SendMessage( g_Plugin.hMainWnd, AKD_SETMAINPROC,
                (WPARAM) NewMainProc, (LPARAM) &g_Plugin.pMainProcData );

            if ( g_Plugin.nMDI )
            {
                // SubClass frame window
                g_Plugin.pFrameProcData = NULL;
                SendMessage( g_Plugin.hMainWnd, AKD_SETFRAMEPROC,
                    (WPARAM) NewFrameProc, (LPARAM) &g_Plugin.pFrameProcData );
            }
        }
    }

    if ( dwMode )
    {
        // this SubClassing is required
        // for all MODE_NOSELEOL, MODE_SMARTEND, MODE_SMARTHOME and MODE_SMARTUPDOWN
        if ( !g_Plugin.dwInitializedMode )
        {
            // SubClass edit window
            g_Plugin.pEditProcData = NULL;
            SendMessage( g_Plugin.hMainWnd, AKD_SETEDITPROC,
                (WPARAM) NewEditProc, (LPARAM) &g_Plugin.pEditProcData );
        }
    }

    g_Plugin.dwInitializedMode |= dwMode;
}

void Uninitialize(DWORD dwMode)
{
    if ( !g_Plugin.dwInitializedMode )
        return;

    // dwMode mask (depends on current value of dwInitializedMode)
    dwMode &= g_Plugin.dwInitializedMode;

    if ( dwMode & MODE_NOSELEOL )
    {
        // these functions were subclassed
        // only for MODE_NOSELEOL
        if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
        {
            if ( g_Plugin.pMainProcData )
            {
                // Remove subclass (main window)
                SendMessage( g_Plugin.hMainWnd, AKD_SETMAINPROC,
                    (WPARAM) NULL, (LPARAM) &g_Plugin.pMainProcData );
                g_Plugin.pMainProcData = NULL;
            }
            if ( g_Plugin.nMDI && g_Plugin.pFrameProcData )
            {
                // Remove subclass (frame window)
                SendMessage( g_Plugin.hMainWnd, AKD_SETFRAMEPROC,
                    (WPARAM) NULL, (LPARAM) &g_Plugin.pFrameProcData );
                g_Plugin.pFrameProcData = NULL;
            }
        }
    }

    if ( dwMode )
    {
        // this function was subclassed
        // for all MODE_NOSELEOL, MODE_SMARTEND, MODE_SMARTHOME and MODE_SMARTUPDOWN
        if ( g_Plugin.dwInitializedMode == dwMode ) // dwMode is already masked
        {
            if ( g_Plugin.pEditProcData )
            {
                // Remove subclass (edit window)
                SendMessage( g_Plugin.hMainWnd, AKD_SETEDITPROC,
                    (WPARAM) NULL, (LPARAM) &g_Plugin.pEditProcData );
                g_Plugin.pEditProcData = NULL;
            }
        }
    }

    g_Plugin.dwInitializedMode -= dwMode;

    if ( !(g_Plugin.dwInitializedMode & MODE_SMARTEND) &&
         !(g_Plugin.dwInitializedMode & MODE_SMARTHOME) && 
         !(g_Plugin.dwInitializedMode & MODE_SMARTBACKSPACE) )
    {
        bufFree(&g_TextBuf);
    }

}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg )
    {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            if ( g_Plugin.dwInitializedMode & MODE_SMARTBACKSPACE )
            {
                if ( wParam == VK_BACK )
                {
                    if ( ((GetKeyState(VK_CONTROL) & 0x80) == 0) &&
                         ((GetKeyState(VK_MENU) & 0x80) == 0) )
                    {
                        if ( OnEditBackspaceKeyDown(hWnd, lParam) )
                            return TRUE;
                    }
                }
            }

            if ( g_Plugin.dwInitializedMode & MODE_SMARTHOME )
            {
                if ( wParam == VK_HOME )
                {
                    if ( GetKeyState(VK_CONTROL) >= 0 )
                    {
                        if ( OnEditHomeKeyDown(hWnd, lParam) )
                            return TRUE;
                    }
                }
            }

            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                if ( !g_SelState.bShiftDown )
                {
                    if ( wParam == VK_SHIFT /*GetKeyState(VK_SHIFT) & 0x80*/ )
                    {
                        LRESULT lResult = 0;

                        if ( g_Plugin.pEditProcData && g_Plugin.pEditProcData->NextProc )
                            lResult = g_Plugin.pEditProcData->NextProc(hWnd, uMsg, wParam, lParam);

                        OnEditSelKeyDown(hWnd, VK_SHIFT);

                        return lResult;
                    }
                }
            }

            if ( uMsg == WM_SYSKEYDOWN )
                break;

            if ( g_Plugin.dwInitializedMode & MODE_SMARTEND )
            {
                if ( wParam == VK_END )
                {
                    if ( ((GetKeyState(VK_CONTROL) & 0x80) == 0) &&
                         ((GetKeyState(VK_MENU) & 0x80) == 0) )
                    {
                        if ( OnEditEndKeyDown(hWnd, lParam) )
                            return TRUE;
                    }
                }
            }

            if ( g_Plugin.dwInitializedMode & MODE_SMARTUPDOWN )
            {
                if ( (wParam == VK_DOWN) ||  // Arrow Down
                     (wParam == VK_UP)   ||  // Arrow Up
                     ( g_bSmartUpDownMode2 &&
                       ( (wParam == VK_PRIOR) ||  // Page Up
                         (wParam == VK_NEXT) )    // Page Down
                     )
                   )
                {
                    if ( ((GetKeyState(VK_CONTROL) & 0x80) == 0) &&
                         ((GetKeyState(VK_MENU) & 0x80) == 0) )
                    {
                        if ( OnEditArrowDownOrUpKeyDown(hWnd, wParam) )
                            return TRUE;
                    }
                }
            }

            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                if ( g_SelState.bShiftDown )
                {
                    if ( wParam == VK_SHIFT /*(GetKeyState(VK_SHIFT) & 0x80) == 0*/ )
                    {
                        LRESULT lResult = 0;

                        if ( g_Plugin.pEditProcData && g_Plugin.pEditProcData->NextProc )
                            lResult = g_Plugin.pEditProcData->NextProc(hWnd, uMsg, wParam, lParam);

                        OnEditSelKeyUp(hWnd, VK_SHIFT);

                        return lResult;
                    }
                }
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                if ( !g_SelState.bMouseLButtonDown )
                {
                    LRESULT lResult = 0;

                    if ( g_Plugin.pEditProcData && g_Plugin.pEditProcData->NextProc )
                        lResult = g_Plugin.pEditProcData->NextProc(hWnd, uMsg, wParam, lParam);

                    OnEditSelKeyDown(hWnd, VK_LBUTTON);

                    return lResult;
                }
            }
            break;
        }
        case WM_LBUTTONUP:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                if ( g_SelState.bMouseLButtonDown )
                {
                    LRESULT lResult = 0;

                    if ( g_Plugin.pEditProcData && g_Plugin.pEditProcData->NextProc )
                        lResult = g_Plugin.pEditProcData->NextProc(hWnd, uMsg, wParam, lParam);

                    OnEditSelKeyUp(hWnd, VK_LBUTTON);

                    return lResult;
                }
            }
            break;
        }
    }

    if ( g_Plugin.pEditProcData && g_Plugin.pEditProcData->NextProc )
        return g_Plugin.pEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
    else
        return 0;
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg )
    {
        /*
        case WM_NOTIFY:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                LPNMHDR pnmh = (LPNMHDR) lParam;
                if ( pnmh->code == EN_SELCHANGE )
                {
                    EDITINFO ei;

                    SendMessage( g_Plugin.hMainWnd,
                        AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei );

                    if ( ei.hWndEdit == pnmh->hwndFrom )
                    {

                    }
                }
            }
            break;
        }
        */
        case WM_MDIACTIVATE:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                clearSelectionState(&g_SelState);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    CheckEditNotification(hWnd, uMsg, wParam, lParam);

    if ( g_Plugin.pFrameProcData && g_Plugin.pFrameProcData->NextProc )
        return g_Plugin.pFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
    else
        return 0;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg )
    {
        /*
        case WM_NOTIFY:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                LPNMHDR pnmh = (LPNMHDR) lParam;
                if ( pnmh->code == EN_SELCHANGE )
                {
                    EDITINFO ei;

                    SendMessage( g_Plugin.hMainWnd,
                        AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei );

                    if ( ei.hWndEdit == pnmh->hwndFrom )
                    {

                    }
                }
            }
            break;
        }
        */
        case AKDN_EDIT_ONSTART:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                clearSelectionState(&g_SelState);
            }
            break;
        }
        case AKDN_EDIT_ONFINISH:
        {
            if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
            {
                clearSelectionState(&g_SelState);
            }
            break;
        }
    }

    CheckEditNotification(hWnd, uMsg, wParam, lParam);

    if ( g_Plugin.pMainProcData && g_Plugin.pMainProcData->NextProc )
        return g_Plugin.pMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
    else
        return 0;
}

void OnEditSelKeyDown(HWND hEdit, unsigned int uKey)
{
    if ( uKey == VK_SHIFT )
        g_SelState.bShiftDown = TRUE;
    else if ( uKey == VK_LBUTTON )
        g_SelState.bMouseLButtonDown = TRUE;

    g_SelState.SelKeyDownRange.cpMin = 0;
    g_SelState.SelKeyDownRange.cpMax = 0;
    SendMessage( hEdit, EM_EXGETSEL64, 0, (LPARAM) &g_SelState.SelKeyDownRange );
}

void OnEditSelKeyUp(HWND hEdit, unsigned int uKey)
{
    CHARRANGE64 SelKeyUpRange;

    SelKeyUpRange.cpMin = 0;
    SelKeyUpRange.cpMax = 0;
    SendMessage( hEdit, EM_EXGETSEL64, 0, (LPARAM) &SelKeyUpRange );
    if ( SelKeyUpRange.cpMin != SelKeyUpRange.cpMax )
    {
        if ( (SelKeyUpRange.cpMax != g_SelState.SelKeyDownRange.cpMax) ||
             (SelKeyUpRange.cpMin > g_SelState.SelKeyDownRange.cpMin) ||
             (g_SelState.bSpecialAction && (SelKeyUpRange.cpMin == g_SelState.SelKeyDownRange.cpMin)) )
        {
            // selection from right to left is not processed here
            // (the condition for cpMin works as "==" in case of
            //  mouse-triple-click and left-margin-mouse-click)
            OnEditSelectionChanged(hEdit, &SelKeyUpRange);
        }
    }

    if ( uKey == VK_SHIFT )
        g_SelState.bShiftDown = FALSE;
    else if ( uKey == VK_LBUTTON )
        g_SelState.bMouseLButtonDown = FALSE;

    g_SelState.bSpecialAction = FALSE;
    g_SelState.SelKeyDownRange.cpMin = 0;
    g_SelState.SelKeyDownRange.cpMax = 0;
}

void OnEditSelectionChanged(HWND hEdit, CHARRANGE64* cr)
{
    if ( !g_SelState.bColumnSelection )
    {
        INT_PTR i;
        wchar_t szTextW[4];

        if ( cr->cpMin == 0 )
        {
            GETTEXTLENGTHEX gtl;
            INT_PTR nTextLen;

            gtl.flags = GTL_DEFAULT | GTL_PRECISE;
            gtl.codepage = 0;
            nTextLen = (INT_PTR) SendMessage(hEdit, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);

            if ( cr->cpMax >= nTextLen )
            {
                // selection at EOF
                return;
            }
        }

        if ( g_bNoSelEOLMode2 )
        {
            if ( g_Plugin.bAkelEdit )
            {
                AECHARINDEX aeIndex;
                AECHARINDEX aeEnd;
                int         nRealEOL;

                nRealEOL = 0;
                SendMessage(hEdit, AEM_RICHOFFSETTOINDEX, cr->cpMin, (LPARAM) &aeIndex);
                SendMessage(hEdit, AEM_RICHOFFSETTOINDEX, cr->cpMax, (LPARAM) &aeEnd);
                while ( (nRealEOL < 2) && (aeIndex.nLine < aeEnd.nLine) )
                {
                    if ( aeIndex.lpLine->nLineBreak != AELB_WRAP )
                        ++nRealEOL;
                    AEC_NextLine(&aeIndex);
                }

                if ( nRealEOL >= 2 )
                    return;
            }
        }

        if ( g_Plugin.bOldWindows )
        {
            TEXTRANGE64A trA;

            trA.chrg.cpMin = (cr->cpMax >= 2) ? (cr->cpMax - 2) : 0;
            trA.chrg.cpMax = cr->cpMax;
            trA.lpstrText = (LPSTR) szTextW;
            // get last 2 characters from the selection to inspect for EOL
            i = (INT_PTR) SendMessageA( hEdit, EM_GETTEXTRANGE64A, 0, (LPARAM) &trA );
            if ( i < (trA.chrg.cpMax - trA.chrg.cpMin) )
            {
                // last symbol is '\0' - end of file
                if ( cr->cpMax - cr->cpMin == 1 ) // just '\0' is selected
                    --cr->cpMax;
            }
            else
            {
                if ( (i > 0) && (trA.lpstrText[i-1] == '\n') )
                {
                    --i;
                    --cr->cpMax;
                }
                if ( (i > 0) && (trA.lpstrText[i-1] == '\r') )
                {
                    --cr->cpMax;
                }
            }
            if ( cr->cpMax != trA.chrg.cpMax )
            {
                if ( g_Plugin.bAkelEdit )
                {
                    AESELECTION aes;

                    aes.dwFlags = AESELT_LOCKSCROLL;
                    aes.dwType = 0;
                    SendMessage( hEdit, AEM_RICHOFFSETTOINDEX, cr->cpMin, (LPARAM) &aes.crSel.ciMin );
                    SendMessage( hEdit, AEM_RICHOFFSETTOINDEX, cr->cpMax, (LPARAM) &aes.crSel.ciMax );
                    SendMessage( hEdit, AEM_SETSEL, 0, (LPARAM) &aes );
                }
                else
                {
                    // EM_GETSCROLLPOS - WM_SETREDRAW - EM_EXSETSEL64 - EM_SETSCROLLPOS - WM_SETREDRAW
                    SendMessage( hEdit, EM_EXSETSEL64, 0, (LPARAM) cr );
                }
            }
        }
        else
        {
            TEXTRANGE64W trW;

            trW.chrg.cpMin = (cr->cpMax >= 2) ? (cr->cpMax - 2) : 0;
            trW.chrg.cpMax = cr->cpMax;
            trW.lpstrText = (LPWSTR) szTextW;
            i = (INT_PTR) SendMessageW( hEdit, EM_GETTEXTRANGE64W, 0, (LPARAM) &trW );
            if ( i < (trW.chrg.cpMax - trW.chrg.cpMin) )
            {
                // last symbol is L'\0' - end of file
                if ( cr->cpMax - cr->cpMin == 1 ) // just L'\0' is selected
                    --cr->cpMax;
            }
            else
            {
                if ( (i > 0) && (trW.lpstrText[i-1] == L'\n') )
                {
                    --i;
                    --cr->cpMax;
                }
                if ( (i > 0) && (trW.lpstrText[i-1] == L'\r') )
                {
                    --cr->cpMax;
                }
            }
            if ( cr->cpMax != trW.chrg.cpMax )
            {
                if ( g_Plugin.bAkelEdit )
                {
                    AESELECTION aes;

                    aes.dwFlags = AESELT_LOCKSCROLL;
                    aes.dwType = 0;
                    SendMessageW( hEdit, AEM_RICHOFFSETTOINDEX, cr->cpMin, (LPARAM) &aes.crSel.ciMin );
                    SendMessageW( hEdit, AEM_RICHOFFSETTOINDEX, cr->cpMax, (LPARAM) &aes.crSel.ciMax );
                    SendMessageW( hEdit, AEM_SETSEL, 0, (LPARAM) &aes );
                }
                else
                {
                    // EM_GETSCROLLPOS - WM_SETREDRAW - EM_EXSETSEL64 - EM_SETSCROLLPOS - WM_SETREDRAW
                    SendMessageW( hEdit, EM_EXSETSEL64, 0, (LPARAM) cr );
                }
            }
        }
    }
    else
    {
        g_SelState.bColumnSelection = FALSE;
    }
}

BOOL OnEditEndKeyDown(HWND hEdit, LPARAM lParam)
{
    int         nLine;
    INT_PTR     nLineIndex;
    int         nLineLen;
    CHARRANGE64 cr = { 0, 0 };

    SendMessage(hEdit, EM_EXGETSEL64, 0, (LPARAM) &cr);
    nLine = (int) SendMessage(hEdit, EM_EXLINEFROMCHAR, 0, (LPARAM)(-1));
    // wow, "magic" parameter (-1) gives REAL line number!!!
    nLineIndex = (INT_PTR) SendMessage(hEdit, EM_LINEINDEX, nLine, 0);
    nLineLen = (int) SendMessage(hEdit, EM_LINELENGTH, nLineIndex, 0);
    if ( nLineLen <= 0 )
        return FALSE;

    /*
    if ( nLineIndex == cr.cpMax ) // line beginning or WordWrap'ed line end
    {
        CHARRANGE64 cr2 = { 0, 0 };

        HideCaret(hEdit);                                  // hide caret
        SendMessage(hEdit, WM_SETREDRAW, FALSE, 0);        // forbid redraw
        g_bProcessEndKey = FALSE;
        SendMessage(hEdit, WM_KEYDOWN, VK_END, lParam);    // emulate End pressed
        g_bProcessEndKey = TRUE;
        SendMessage(hEdit, EM_EXGETSEL64, 0, (LPARAM) &cr2); // get new selection
        SendMessage(hEdit, EM_EXSETSEL64, 0, (LPARAM) &cr);  // restore selection
        SendMessage(hEdit, WM_SETREDRAW, TRUE, 0);         // allow redraw
        ShowCaret(hEdit);                                  // show caret

        if ( cr.cpMax == cr2.cpMax ) // WordWrap'ed line end
        {
            --nLine; // previous line
            nLineIndex = (int) SendMessage(hEdit, EM_LINEINDEX, nLine, 0);
            nLineLen = (int) SendMessage(hEdit, EM_LINELENGTH, nLineIndex, 0);
            if ( nLineLen <= 0 )
                return FALSE;
        }
    }
    */

    if ( g_Plugin.dwInitializedMode & MODE_SMARTEND )
    {
        int  nSize;
        BOOL bShiftPressed = (GetKeyState(VK_SHIFT) & 0x80) ? TRUE : FALSE;

        if ( g_Plugin.bOldWindows )
            nSize = (nLineLen + 1)*sizeof(char);
        else
            nSize = (nLineLen + 1)*sizeof(wchar_t);

        if ( bufReserve(&g_TextBuf, nSize) )
        {
            INT_PTR i;

            if ( g_Plugin.bOldWindows )
            {
                TEXTRANGE64A trA;
                LPSTR        lpText = (LPSTR) g_TextBuf.pData;

                lpText[0] = 0;
                trA.chrg.cpMin = nLineIndex;
                trA.chrg.cpMax = nLineIndex + nLineLen;
                trA.lpstrText = lpText;
                i = (INT_PTR) SendMessageA( hEdit, EM_GETTEXTRANGE64A, 0, (LPARAM) &trA );
                while ( --i >= 0 )
                {
                    if ( (lpText[i] != ' ') && (lpText[i] != '\t') )
                        break;
                }
            }
            else
            {
                TEXTRANGE64W trW;
                LPWSTR       lpText = (LPWSTR) g_TextBuf.pData;

                lpText[0] = 0;
                trW.chrg.cpMin = nLineIndex;
                trW.chrg.cpMax = nLineIndex + nLineLen;
                trW.lpstrText = lpText;
                i = (INT_PTR) SendMessageW( hEdit, EM_GETTEXTRANGE64W, 0, (LPARAM) &trW );
                while ( --i >= 0 )
                {
                    if ( (lpText[i] != L' ') && (lpText[i] != L'\t') )
                        break;
                }
            }

            bufFreeIfSizeExceeds(&g_TextBuf, MAX_TEXTBUF_SIZE);
            // lpText is no more valid

            ++i;

            if ( cr.cpMax == nLineIndex + nLineLen )
            {
                // selection ends at the end of line
                if ( (cr.cpMax == cr.cpMin) && (i == nLineLen) )
                {
                    // The caret is at the end of line (no selection).
                    // No trailing tabs or spaces, thus no SmartEnd available:
                    // there's nowhere to move.
                    return FALSE;
                }
                else if ( (cr.cpMax != cr.cpMin) && g_bSmartEndMode2 && !bShiftPressed )
                {
                    // altSmartEnd: jump to the end of line
                    return FALSE;
                }
            }

            if ( g_bSmartEndMode2 ?
                  (cr.cpMax == nLineIndex + nLineLen) :
                   (cr.cpMax != nLineIndex + i)
               )
            {
                if ( bShiftPressed && (cr.cpMin == nLineIndex + i) )
                {
                    cr.cpMin = cr.cpMax;
                    cr.cpMax = nLineIndex + nLineLen;
                }
                else
                {
                    if ( !bShiftPressed )
                        cr.cpMin = nLineIndex + i;
                    cr.cpMax = nLineIndex + i;
                }
            }
            else
            {
                if ( !bShiftPressed )
                    cr.cpMin = nLineIndex + nLineLen;
                cr.cpMax = nLineIndex + nLineLen;
            }

            if ( cr.cpMax == nLineIndex + nLineLen )
            {
                if ( nLine < (int) SendMessage(hEdit, EM_EXLINEFROMCHAR, 0, cr.cpMax) )
                    return FALSE;
            }

            SendMessage( hEdit, EM_EXSETSEL64, 0, (LPARAM) &cr );
        }
    }
    else if ( g_Plugin.dwInitializedMode & MODE_NOSELEOL )
    {
        cr.cpMax = nLineIndex + nLineLen;
        SendMessage( hEdit, EM_EXSETSEL64, 0, (LPARAM) &cr );
    }

    return TRUE;
}

BOOL OnEditBackspaceKeyDown(HWND hEdit, LPARAM lParam)
{
  if (g_Plugin.bAkelEdit)
  {
    // AkelEdit
    AESELECTION aes;
    AECHARINDEX ciCaret;

    if (!SendMessage(hEdit, AEM_GETSEL, (WPARAM)&ciCaret, (LPARAM)&aes))
    {
      AECHARINDEX ciCaretLineBegin;
      AECHARINDEX ciPrevLineBegin;
      AECHARINDEX ciMinSel;
      wchar_t *wszIndent=NULL;
      int nLineSpaces=0;
      int nPrevLineSpaces=0;
      int nTabStopSize;
      int nCaretWrapCharInLine;

      nTabStopSize=(int)SendMessage(hEdit, AEM_GETTABSTOP, 0, 0);
      nCaretWrapCharInLine=AEC_WrapLineBeginEx(&ciCaret, &ciCaretLineBegin);

      if (ciCaretLineBegin.lpLine->prev)
      {
        if (GetLineSpaces(&ciCaretLineBegin, nCaretWrapCharInLine, nTabStopSize, &nLineSpaces))
        {
          ciPrevLineBegin=ciCaretLineBegin;

          while (AEC_PrevLine(&ciPrevLineBegin))
          {
            AEC_WrapLineBeginEx(&ciPrevLineBegin, &ciPrevLineBegin);
            if (!GetLineSpaces(&ciPrevLineBegin, 0x7FFFFFFF, nTabStopSize, &nPrevLineSpaces))
            {
              if (ciPrevLineBegin.lpLine->nLineLen && nPrevLineSpaces < nLineSpaces)
              {
                ciMinSel.nLine=ciCaretLineBegin.nLine;
                ciMinSel.lpLine=ciCaretLineBegin.lpLine;
                ciMinSel.nCharInLine=nPrevLineSpaces;
                SendMessage(hEdit, AEM_COLUMNTOINDEX, MAKELONG(nTabStopSize, AECTI_WRAPLINEBEGIN|AECTI_FIT), (LPARAM)&ciMinSel);
                nLineSpaces=(int)SendMessage(hEdit, AEM_INDEXTOCOLUMN, MAKELONG(nTabStopSize, AECTI_WRAPLINEBEGIN), (LPARAM)&ciMinSel);

                if (nPrevLineSpaces > nLineSpaces)
                {
                  //In:
                  //---->---->·123
                  //---->---->---->---->|456
                  //Out:
                  //---->---->·123
                  //---->---->·|456
                  if ( bufReserve(&g_TextBuf, (nPrevLineSpaces - nLineSpaces + 1) * sizeof(wchar_t)) )
                  {
                    wchar_t *wpIndentMax;
                    wchar_t *wpCount;

                    wszIndent = (wchar_t *) g_TextBuf.pData;
                    wpIndentMax=wszIndent + (nPrevLineSpaces - nLineSpaces);
                    wpCount=wszIndent;

                    for (; wpCount < wpIndentMax; ++wpCount)
                      *wpCount=L' ';
                    *wpCount=0; // trailing NULL
                  }
                }
                SendMessage(hEdit, AEM_EXSETSEL, (WPARAM)&ciMinSel, (LPARAM)&ciCaret);
                SendMessage(g_Plugin.hMainWnd, AKD_REPLACESELW, (WPARAM)hEdit, (LPARAM)(wszIndent ? wszIndent : L""));
                if (wszIndent != NULL)
                  bufFreeIfSizeExceeds(&g_TextBuf, MAX_TEXTBUF_SIZE);
                return TRUE;
              }
            }
          }
        }
      }
    }
  }
  return FALSE;
}

BOOL OnEditHomeKeyDown(HWND hEdit, LPARAM lParam)
{
    if ( g_Plugin.bAkelEdit )
    {
        // AkelEdit
        AESELECTION aes;
        AECHARINDEX ciCaret;
        int i;

        SendMessage( hEdit, AEM_GETSEL, (WPARAM) &ciCaret, (LPARAM) &aes );

        for ( i = 0; ciCaret.lpLine->wpLine[i] == ' ' || ciCaret.lpLine->wpLine[i] == '\t'; ++i ) ;

        if ( i )
        {
            if ( ciCaret.nCharInLine == 0 || ( g_bSmartHomeMode2 ? 0 : ciCaret.nCharInLine > i ) )
            {
                if ( !AEC_IndexCompare(&ciCaret, &aes.crSel.ciMin) )
                {
                    ciCaret.nCharInLine = i;
                    aes.crSel.ciMin = ciCaret;
                }
                else
                {
                    ciCaret.nCharInLine = i;
                    aes.crSel.ciMax = ciCaret;
                }

                if ( GetKeyState(VK_SHIFT) >= 0 )
                {
                    aes.crSel.ciMin = ciCaret;
                    aes.crSel.ciMax = ciCaret;
                }

                SendMessage( hEdit, AEM_SETSEL, (WPARAM) &ciCaret, (LPARAM) &aes );

                return TRUE;
            }
        }
    }
    else
    {
        // RichEdit
        if ( g_Plugin.bOldWindows )
        {
            if ( SmartHomeA(hEdit) )
                return TRUE;
        }
        else
        {
            if ( SmartHomeW(hEdit) )
                return TRUE;
        }
    }

    return FALSE;
}

BOOL OnEditArrowDownOrUpKeyDown(HWND hEdit, WPARAM wKey)
{
    int         nLine;
    int         nMaxLine;
    CHARRANGE64 crOld;
    CHARRANGE64 cr;

    cr.cpMin = 0;
    cr.cpMax = 0;
    SendMessage(hEdit, EM_EXGETSEL64, 0, (LPARAM) &cr);
    crOld.cpMin = cr.cpMin;
    crOld.cpMax = cr.cpMax;

    if ( wKey == VK_DOWN || wKey == VK_NEXT )
    {
        nLine = (int) SendMessage(hEdit, EM_EXLINEFROMCHAR, 0, cr.cpMax);
        nMaxLine = (int) SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
        --nMaxLine;
        if ( nLine == nMaxLine )
        {
            GETTEXTLENGTHEX gtl;

            gtl.flags = GTL_DEFAULT | GTL_PRECISE;
            gtl.codepage = 0;
            cr.cpMax = (INT_PTR) SendMessage(hEdit, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);

            if ( (GetKeyState(VK_SHIFT) & 0x80) == 0 )
            {
                cr.cpMin = cr.cpMax;
            }

            if ( cr.cpMin != crOld.cpMin || cr.cpMax != crOld.cpMax )
            {
                SendMessage( hEdit, EM_EXSETSEL64, 0, (LPARAM) &cr );
                return TRUE;
            }
        }
    }
    else // ( wKey == VK_UP || wKey == VK_PRIOR )
    {
        nLine = (int) SendMessage(hEdit, EM_EXLINEFROMCHAR, 0, cr.cpMin);
        if ( nLine == 0 )
        {
            if ( GetKeyState(VK_SHIFT) & 0x80 )
            {
                cr.cpMin = cr.cpMax;
                cr.cpMax = 0;
            }
            else
            {
                cr.cpMin = 0;
                cr.cpMax = 0;
            }

            if ( cr.cpMin != crOld.cpMax || cr.cpMax != crOld.cpMin )
            {
                SendMessage( hEdit, EM_EXSETSEL64, 0, (LPARAM) &cr );
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL SmartHomeA(HWND hEdit)
{
    CHARRANGE64  crCurrent;
    CHARRANGE64  crNew;
    TEXTRANGE64A txtrng;
    char*        pText;
    int          nMinLine;
    INT_PTR      nMinLineIndex;
    int          nMinLineLength;
    int          i;
    BOOL         bResult = FALSE;

    SendMessage(hEdit, EM_EXGETSEL64, 0, (LPARAM)&crCurrent);
    nMinLine = (int) SendMessage(hEdit, EM_EXLINEFROMCHAR, 0, crCurrent.cpMin);
    nMinLineIndex = (INT_PTR) SendMessage(hEdit, EM_LINEINDEX, nMinLine, 0);
    nMinLineLength = (int) SendMessage(hEdit, EM_LINELENGTH, nMinLineIndex, 0);

    if ( bufReserve(&g_TextBuf, nMinLineLength + 1) )
    {
        pText = (char *) g_TextBuf.pData;
        txtrng.chrg.cpMin = nMinLineIndex;
        txtrng.chrg.cpMax = nMinLineIndex + nMinLineLength;
        txtrng.lpstrText = pText;
        SendMessageA(hEdit, EM_GETTEXTRANGE64A, 0, (LPARAM)&txtrng);

        for ( i = 0; pText[i] == ' ' || pText[i] == '\t'; ++i ) ;

        bufFreeIfSizeExceeds(&g_TextBuf, MAX_TEXTBUF_SIZE);
        // pText is no more valid

        if ( i )
        {
            if ( (crCurrent.cpMin - nMinLineIndex) > i || (crCurrent.cpMin - nMinLineIndex) == 0 )
                crNew.cpMin = nMinLineIndex + i;
            else
                crNew.cpMin = nMinLineIndex;

            if ( GetKeyState(VK_SHIFT) < 0 )
                crNew.cpMax = crCurrent.cpMax;
            else
                crNew.cpMax = crNew.cpMin;

            SendMessage(hEdit, EM_EXSETSEL64, 0, (LPARAM)&crNew);

            bResult = TRUE;
        }
    }

    return bResult;
}

BOOL SmartHomeW(HWND hEdit)
{
    CHARRANGE64  crCurrent;
    CHARRANGE64  crNew;
    TEXTRANGE64W txtrng;
    wchar_t*     wpText;
    int          nMinLine;
    INT_PTR      nMinLineIndex;
    int          nMinLineLength;
    int          i;
    BOOL         bResult = FALSE;

    SendMessage(hEdit, EM_EXGETSEL64, 0, (LPARAM)&crCurrent);
    nMinLine = (int) SendMessage(hEdit, EM_EXLINEFROMCHAR, 0, crCurrent.cpMin);
    nMinLineIndex = (INT_PTR) SendMessage(hEdit, EM_LINEINDEX, nMinLine, 0);
    nMinLineLength = (int) SendMessage(hEdit, EM_LINELENGTH, nMinLineIndex, 0);

    if ( bufReserve(&g_TextBuf, (nMinLineLength + 1)*sizeof(wchar_t)) )
    {
        wpText = (wchar_t *) g_TextBuf.pData;
        txtrng.chrg.cpMin = nMinLineIndex;
        txtrng.chrg.cpMax = nMinLineIndex + nMinLineLength;
        txtrng.lpstrText = wpText;
        SendMessageW(hEdit, EM_GETTEXTRANGE64W, 0, (LPARAM)&txtrng);

        for ( i = 0; wpText[i] == ' ' || wpText[i] == '\t'; ++i ) ;

        bufFreeIfSizeExceeds(&g_TextBuf, MAX_TEXTBUF_SIZE);
        // wpText is no more valid

        if ( i )
        {
            if ( (crCurrent.cpMin - nMinLineIndex) > i || (crCurrent.cpMin - nMinLineIndex) == 0 )
                crNew.cpMin = nMinLineIndex + i;
            else
                crNew.cpMin = nMinLineIndex;

            if ( GetKeyState(VK_SHIFT) < 0 )
                crNew.cpMax = crCurrent.cpMax;
            else
                crNew.cpMax = crNew.cpMin;

            SendMessage(hEdit, EM_EXSETSEL64, 0, (LPARAM)&crNew);

            bResult = TRUE;
        }
    }

    return bResult;
}

BOOL GetLineSpaces(AECHARINDEX *ciFirstCharInLine, int nWrapCharInLine, int nTabStopSize, int *lpnLineSpaces)
{
  AECHARINDEX ciCount=*ciFirstCharInLine;
  int nLineSpaces=0;
  int nCountCharInLine=0;

  while (nCountCharInLine < nWrapCharInLine)
  {
    if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L' ')
      ++nLineSpaces;
    else if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\t')
      nLineSpaces+=nTabStopSize - nLineSpaces % nTabStopSize;
    else
    {
      *lpnLineSpaces=nLineSpaces;
      return FALSE;
    }
    if (!AEC_NextCharInLine(&ciCount))
      break;
    ++nCountCharInLine;
  }
  *lpnLineSpaces=nLineSpaces;
  return TRUE;
}

void CheckEditNotification(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( g_Plugin.bAkelEdit && (g_Plugin.dwInitializedMode & MODE_NOSELEOL) )
    {
        if ( (uMsg == WM_NOTIFY) && (wParam == ID_EDIT) )
        {
            if ( ((NMHDR *)lParam)->code == AEN_SELCHANGING )
            {
                AENSELCHANGE* psc = (AENSELCHANGE *) lParam;

                if ( psc->bColumnSel )
                    g_SelState.bColumnSelection = TRUE;
                else
                    g_SelState.bColumnSelection = FALSE;

                if ( (psc->dwType & AESCT_MOUSETRIPLECLK) || (psc->dwType & AESCT_MOUSELEFTMARGIN) )
                    g_SelState.bSpecialAction = TRUE;
                else
                    g_SelState.bSpecialAction = FALSE;
            }
        }
    }
}
