#include "QSearch.h"
#include <StrFunc.h>
#include <StackFunc.h>
#include "QSearchDlg.h"
#include "DialogSwitcher.h"
#include "XMemStrFunc.h"
#include "RegExpFunc.h"


#define TEST_UNINIT 0


#define AKEL_DLL_VERSION    AKELDLL
#define EXE_MIN_VERSION_3X  MAKE_IDENTIFIER(-1, -1, -1, -1)
#define EXE_MIN_VERSION_4X  MAKE_IDENTIFIER(4, 9, 8, 0)
#define PLUGIN_NAME         "QSearch"



#if TEST_UNINIT
HANDLE hLogFile = NULL;
CRITICAL_SECTION csLog;
void WriteLog(const char* str)
{
    static BOOL isCsLog = FALSE;
    DWORD dwWritten = 0;

    if ( !isCsLog )
    {
        isCsLog = TRUE;
        InitializeCriticalSection(&csLog);
    }

    EnterCriticalSection(&csLog);
    if ( !hLogFile )
    {
        hLogFile = CreateFileA("QSearch.log", GENERIC_WRITE,
            FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
        SetFilePointer(hLogFile, 0, NULL, FILE_END);
        WriteFile(hLogFile, "\n", 1, &dwWritten, NULL);
    }
    WriteFile(hLogFile, str, lstrlenA(str), &dwWritten, NULL);
    LeaveCriticalSection(&csLog);
}
void CloseLog(void)
{
    if ( hLogFile )
    {
        EnterCriticalSection(&csLog);
        CloseHandle(hLogFile);
        hLogFile = NULL;
        LeaveCriticalSection(&csLog);
    }
}
#endif


#define  WRONG_DWORD_VALUE           0xFEDC
#define  WRONG_INT_VALUE             0xFEDC
#define  WRONG_COLOR_VALUE           0xFFFFFFFFL
#define  DEFAULT_COLOR_NOTFOUND      RGB(0xFF, 0xE0, 0xE0)
#define  DEFAULT_COLOR_NOTREGEXP     RGB(0xF8, 0xE8, 0xF8)
#define  DEFAULT_COLOR_EOF           RGB(0xE4, 0xFF, 0xE4)
#define  DEFAULT_COLOR_HIGHLIGHT     RGB(0xC0, 0xFF, 0xC0)
#define  DEFAULT_HIGHLIGHT_MARK_ID   1001
#define  DEFAULT_USE_ALT_HOTKEYS     1
#define  DEFAULT_ALT_MATCHCASE       0x43  // 'C', Alt+C (Case)
#define  DEFAULT_ALT_WHOLEWORD       0x57  // 'W', Alt+W (Whole Word)
#define  DEFAULT_ALT_SEARCHMODE      0x52  // 'R', Alt+R (Search Mode)
#define  DEFAULT_ALT_HIGHLIGHTALL    0x48  // 'H', Alt+H (Highlight)
#define  DEFAULT_FIND_HISTORY_ITEMS  15
#define  DEFAULT_HISTORY_SAVE        0x03
#define  DEFAULT_QS_UI               QS_UI_NEW_02
#define  DEFAULT_SELECT_BY_F3        2
#define  DEFAULT_SELECT_BY_FND       1
#define  DEFAULT_SELECT_BY_SELFND    1
#define  DEFAULT_ADJ_INCOMPL_REGEXP  1
#define  DEFAULT_FINDALL_MODE        QS_FINDALL_LOGOUTPUT
#define  DEFAULT_FINDALL_RESULT      QS_FINDALL_RSLT_WHOLELINE | QS_FINDALL_RSLT_POS | QS_FINDALL_RSLT_SEARCHING
#define  DEFAULT_FINDALL_COUNT_DELAY 400
#define  DEFAULT_EDIT_MINWIDTH       0
#define  DEFAULT_EDIT_MAXWIDTH       640
#define  MIN_FIND_HISTORY_ITEMS      0
#define  MAX_FIND_HISTORY_ITEMS      100

#define LOGOUTPUT_FRP_MODE       QSFRM_CHARINLINE
#define LOGOUTPUT_FRP_BEFORE     100
#define LOGOUTPUT_FRP_AFTER      100
#define LOGOUTPUT_FRP_HIGHLIGHT  QSFRH_SELECT

#define FILEOUTPUT_FRP_MODE      QSFRM_LINE
#define FILEOUTPUT_FRP_BEFORE    0
#define FILEOUTPUT_FRP_AFTER     0
#define FILEOUTPUT_FRP_HIGHLIGHT QSFRH_IFCHECKED


/* >>>>>>>>>>>>>>>>>>>>>>>> plugin state >>>>>>>>>>>>>>>>>>>>>>>> */
    void initializePluginState(PluginState* pPlugin)
    {
        pPlugin->bInitialized = FALSE;
        pPlugin->hInstanceDLL = NULL;
        pPlugin->hMainWnd = NULL;
        pPlugin->bAkelEdit = FALSE;
        pPlugin->bAkelPadOnFinish = FALSE;
        pPlugin->nDockedDlgIsWaitingForOnStart = 0;
        pPlugin->wLangSystem = 0; /*GetUserDefaultLangID();*/
        pPlugin->pEditProcData = NULL;
        pPlugin->pMainProcData = NULL;
        pPlugin->pFrameProcData = NULL;
        pPlugin->dwProgramVersion__ = 0;
    }

    DWORD getProgramVersion(PluginState* pPlugin)
    {
        if ( pPlugin->dwProgramVersion__ == 0 )
        {
            pPlugin->dwProgramVersion__ = (DWORD) SendMessage(pPlugin->hMainWnd, AKD_PROGRAMVERSION, 0, 0);
        }
        return pPlugin->dwProgramVersion__;
    }
/* <<<<<<<<<<<<<<<<<<<<<<<< plugin state <<<<<<<<<<<<<<<<<<<<<<<< */


/* >>>>>>>>>>>>>>>>>>>>>>>> qsearch options >>>>>>>>>>>>>>>>>>>>>>>> */
    void initializeFRP(FindResultsOutputPolicy* pFRP)
    {
        pFRP->nMode = WRONG_INT_VALUE;
        pFRP->nBefore = WRONG_INT_VALUE;
        pFRP->nAfter = WRONG_INT_VALUE;
        pFRP->nHighlight = WRONG_INT_VALUE;
    }

    BOOL equalFRP(const FindResultsOutputPolicy* pFRP1, const FindResultsOutputPolicy* pFRP2)
    {
        if ( (pFRP1->nMode      != pFRP2->nMode)   ||
             (pFRP1->nBefore    != pFRP2->nBefore) ||
             (pFRP1->nAfter     != pFRP2->nAfter)  ||
             (pFRP1->nHighlight != pFRP2->nHighlight) )
        {
            return FALSE;
        }
        return TRUE;
    }

    void initializeOptions(QSearchOpt* pOptions)
    {
        int i;
        for ( i = 0; i < OPTF_COUNT; i++ )
        {
            pOptions->dwFlags[i] = WRONG_DWORD_VALUE;
        }
        pOptions->dockRect.left = 0;
        pOptions->dockRect.right = 0;
        pOptions->dockRect.top = 0;
        pOptions->dockRect.bottom = 0;
        pOptions->colorNotFound = WRONG_COLOR_VALUE;
        pOptions->colorNotRegExp = WRONG_COLOR_VALUE;
        pOptions->colorEOF = WRONG_COLOR_VALUE;
        pOptions->colorHighlight = WRONG_COLOR_VALUE;
        pOptions->dwHighlightMarkID = WRONG_DWORD_VALUE;
        pOptions->dwHighlightState = WRONG_DWORD_VALUE;
        pOptions->dwUseAltHotkeys = WRONG_DWORD_VALUE;
        pOptions->dwAltMatchCase = WRONG_DWORD_VALUE;
        pOptions->dwAltWholeWord = WRONG_DWORD_VALUE;
        pOptions->dwAltSearchMode = WRONG_DWORD_VALUE;
        pOptions->dwAltHighlightAll = WRONG_DWORD_VALUE;
        pOptions->dwFindHistoryItems = WRONG_DWORD_VALUE;
        pOptions->dwHistorySave = WRONG_DWORD_VALUE;
        pOptions->dwNewUI = WRONG_DWORD_VALUE;
        pOptions->dwSelectByF3 = WRONG_DWORD_VALUE;
        pOptions->dwSelectByFnd = WRONG_DWORD_VALUE;
        pOptions->dwSelectBySelFnd = WRONG_DWORD_VALUE;
        pOptions->dwAdjIncomplRegExp = WRONG_DWORD_VALUE;
        pOptions->dwFindAllMode = WRONG_DWORD_VALUE;
        pOptions->dwFindAllResult = WRONG_DWORD_VALUE;
        pOptions->dwFindAllCountDelay = WRONG_DWORD_VALUE;
        pOptions->dwEditMinWidth = WRONG_DWORD_VALUE;
        pOptions->dwEditMaxWidth = WRONG_DWORD_VALUE;
        initializeFRP(&pOptions->LogOutputFRP);
        initializeFRP(&pOptions->FileOutputFRP);
    }

    void copyOptionsFlags(DWORD dwOptFlagsDst[], const DWORD dwOptFlagsSrc[])
    {
        x_mem_cpy(dwOptFlagsDst, dwOptFlagsSrc, OPTF_COUNT*sizeof(DWORD));
    }

    void copyOptions(QSearchOpt* pOptDst, const QSearchOpt* pOptSrc)
    {
        x_mem_cpy(pOptDst, pOptSrc, sizeof(QSearchOpt));
    }

    BOOL equalOptions(const QSearchOpt* pOpt1, const QSearchOpt* pOpt2)
    {
        int i;
        for ( i = 0; i < OPTF_COUNT; i++ )
        {
            if ( pOpt1->dwFlags[i] != pOpt2->dwFlags[i] )
                return FALSE;
        }

        if ( (pOpt1->dockRect.left       !=  pOpt2->dockRect.left)       ||
             (pOpt1->dockRect.right      !=  pOpt2->dockRect.right)      ||
             (pOpt1->dockRect.top        !=  pOpt2->dockRect.top)        ||
             (pOpt1->dockRect.bottom     !=  pOpt2->dockRect.bottom)     ||
             (pOpt1->colorNotFound       !=  pOpt2->colorNotFound)       ||
             (pOpt1->colorNotRegExp      !=  pOpt2->colorNotRegExp)      ||
             (pOpt1->colorEOF            !=  pOpt2->colorEOF)            ||
             (pOpt1->colorHighlight      !=  pOpt2->colorHighlight)      ||
             (pOpt1->dwHighlightMarkID   !=  pOpt2->dwHighlightMarkID)   ||
             (pOpt1->dwHighlightState    !=  pOpt2->dwHighlightState)    ||
             (pOpt1->dwUseAltHotkeys     !=  pOpt2->dwUseAltHotkeys)     ||
             (pOpt1->dwAltMatchCase      !=  pOpt2->dwAltMatchCase)      ||
             (pOpt1->dwAltWholeWord      !=  pOpt2->dwAltWholeWord)      ||
             (pOpt1->dwAltSearchMode     !=  pOpt2->dwAltSearchMode)     ||
             (pOpt1->dwAltHighlightAll   !=  pOpt2->dwAltHighlightAll)   ||
             (pOpt1->dwFindHistoryItems  !=  pOpt2->dwFindHistoryItems)  ||
             (pOpt1->dwHistorySave       !=  pOpt2->dwHistorySave)       ||
             (pOpt1->dwNewUI             !=  pOpt2->dwNewUI)             ||
             (pOpt1->dwSelectByF3        !=  pOpt2->dwSelectByF3)        ||
             (pOpt1->dwSelectByFnd       !=  pOpt2->dwSelectByFnd)       ||
             (pOpt1->dwSelectBySelFnd    !=  pOpt2->dwSelectBySelFnd)    ||
             (pOpt1->dwAdjIncomplRegExp  !=  pOpt2->dwAdjIncomplRegExp)  ||
             (pOpt1->dwFindAllMode       !=  pOpt2->dwFindAllMode)       ||
             (pOpt1->dwFindAllResult     !=  pOpt2->dwFindAllResult)     ||
             (pOpt1->dwFindAllCountDelay !=  pOpt2->dwFindAllCountDelay) ||
             (pOpt1->dwEditMinWidth      !=  pOpt2->dwEditMinWidth)      ||
             (pOpt1->dwEditMaxWidth      !=  pOpt2->dwEditMaxWidth)      ||
             !equalFRP(&pOpt1->LogOutputFRP,  &pOpt2->LogOutputFRP)      ||
             !equalFRP(&pOpt1->FileOutputFRP, &pOpt2->FileOutputFRP) )
        {
            return FALSE;
        }

        return TRUE;
    }
/* <<<<<<<<<<<<<<<<<<<<<<<< qsearch options <<<<<<<<<<<<<<<<<<<<<<<< */


// const
const char*    CSZ_QSEARCH = "QSearch";
const wchar_t* CWSZ_QSEARCH = L"QSearch";

const char*    CSZ_OPTIONS[OPT_TOTALCOUNT] = {
    /* OPTF_SRCH_ONTHEFLY_MODE       0 */  "srch_onthefly_mode",
    /* OPTF_SRCH_FROM_BEGINNING      1 */  "srch_from_beginning",
    /* OPTF_SRCH_USE_SPECIALCHARS    2 */  "srch_use_specialchars",
    /* OPTF_SRCH_USE_REGEXP          3 */  "srch_use_regexp",
    /* OPTF_SRCH_REGEXP_DOT_NEWLINE  4 */  "srch_regexp_dot_newline",
    /* OPTF_SRCH_PICKUP_SELECTION    5 */  "srch_pickup_selection",
    /* OPTF_SRCH_SELFIND_PICKUP      6 */  "srch_selfind_pickup",
    /* OPTF_SRCH_STOP_EOF            7 */  "srch_stop_eof",
    /* OPTF_SRCH_WND_DOCKEDTOP       8 */  "srch_wnd_dockedtop",
    /* OPTF_EDIT_FOCUS_SELECTALL     9 */  "edit_focus_selectall",
    /* OPTF_CATCH_MAIN_F3           10 */  "catch_main_f3",
    /* OPTF_CATCH_MAIN_ESC          11 */  "catch_main_esc",
    /* OPTF_HOTKEY_HIDES_PANEL      12 */  "hotkey_hides_panel",
    /* OPTF_EDITOR_AUTOFOCUS_MOUSE  13 */  "editor_autofocus",
    /* OPTF_QSEARCH_AUTOFOCUS_MOUSE 14 */  "qsearch_autofocus",
    /* OPTF_QSEARCH_AUTOFOCUS_FILE  15 */  "qsearch_autofocus_file",
    /* OPTF_DOCK_RECT_DISABLED      16 */  "dock_rect_disabled",
    /* OPT_DOCK_RECT                17 */  "dock_rect",
    /* OPT_COLOR_NOTFOUND           18 */  "color_notfound",
    /* OPT_COLOR_NOTREGEXP          19 */  "color_notregexp",
    /* OPT_COLOR_EOF                20 */  "color_eof",
    /* OPT_COLOR_HIGHLIGHT          21 */  "color_highlight",
    /* OPT_HIGHLIGHT_MARK_ID        22 */  "highlight_mark_id",
    /* OPT_HIGHLIGHT_STATE          23 */  "highlight_state",
    /* OPT_USE_ALT_HOTKEYS          24 */  "use_alt_hotkeys",
    /* OPT_ALT_MATCHCASE            25 */  "alt_match_case",
    /* OPT_ALT_WHOLEWORD            26 */  "alt_whole_word",
    /* OPT_ALT_SEARCHMODE           27 */  "alt_search_mode",
    /* OPT_ALT_HIGHLIGHTALL         28 */  "alt_highlight_all",
    /* OPT_FIND_HISTORY_ITEMS       29 */  "find_history_items",
    /* OPT_HISTORY_SAVE             30 */  "history_save",
    /* OPT_NEW_UI                   31 */  "new_ui",
    /* OPT_SELECT_BY_F3             32 */  "select_by_f3",
    /* OPT_SELECT_BY_FND            33 */  "select_by_fnd",
    /* OPT_SELECT_BY_SELFND         34 */  "select_by_selfnd",
    /* OPT_ADJ_INCOMPL_REGEXP       35 */  "adj_incompl_regexp",
    /* OPT_FINDALL_MODE             36 */  "findall_mode",
    /* OPT_FINDALL_RESULT           37 */  "findall_result",
    /* OPT_FINDALL_COUNT_DELAY      38 */  "findall_count_delay",
    /* OPT_LOGOUTPUT_FRP_MODE       39 */  "logoutput_frp_mode",
    /* OPT_LOGOUTPUT_FRP_BEFORE     40 */  "logoutput_frp_before",
    /* OPT_LOGOUTPUT_FRP_AFTER      41 */  "logoutput_frp_after",
    /* OPT_LOGOUTPUT_FRP_HIGHLIGHT  42 */  "logoutput_frp_highlight",
    /* OPT_FILEOUTPUT_FRP_MODE      43 */  "fileoutput_frp_mode",
    /* OPT_FILEOUTPUT_FRP_BEFORE    44 */  "fileoutput_frp_before",
    /* OPT_FILEOUTPUT_FRP_AFTER     45 */  "fileoutput_frp_after",
    /* OPT_FILEOUTPUT_FRP_HIGHLIGHT 46 */  "fileoutput_frp_highlight",
    /* OPT_EDIT_MINWIDTH            47 */  "edit_minwidth",
    /* OPT_EDIT_MAXWIDTH            48 */  "edit_maxwidth"
};

const wchar_t* CWSZ_OPTIONS[OPT_TOTALCOUNT] = {
    /* OPTF_SRCH_ONTHEFLY_MODE       0 */  L"srch_onthefly_mode",
    /* OPTF_SRCH_FROM_BEGINNING      1 */  L"srch_from_beginning",
    /* OPTF_SRCH_USE_SPECIALCHARS    2 */  L"srch_use_specialchars",
    /* OPTF_SRCH_USE_REGEXP          3 */  L"srch_use_regexp",
    /* OPTF_SRCH_REGEXP_DOT_NEWLINE  4 */  L"srch_regexp_dot_newline",
    /* OPTF_SRCH_PICKUP_SELECTION    5 */  L"srch_pickup_selection",
    /* OPTF_SRCH_SELFIND_PICKUP      6 */  L"srch_selfind_pickup",
    /* OPTF_SRCH_STOP_EOF            7 */  L"srch_stop_eof",
    /* OPTF_SRCH_WND_DOCKEDTOP       8 */  L"srch_wnd_dockedtop",
    /* OPTF_EDIT_FOCUS_SELECTALL     9 */  L"edit_focus_selectall",
    /* OPTF_CATCH_MAIN_F3           10 */  L"catch_main_f3",
    /* OPTF_CATCH_MAIN_ESC          11 */  L"catch_main_esc",
    /* OPTF_HOTKEY_HIDES_PANEL      12 */  L"hotkey_hides_panel",
    /* OPTF_EDITOR_AUTOFOCUS_MOUSE  13 */  L"editor_autofocus",
    /* OPTF_QSEARCH_AUTOFOCUS_MOUSE 14 */  L"qsearch_autofocus",
    /* OPTF_QSEARCH_AUTOFOCUS_FILE  15 */  L"qsearch_autofocus_file",
    /* OPTF_DOCK_RECT_DISABLED      16 */  L"dock_rect_disabled",
    /* OPT_DOCK_RECT                17 */  L"dock_rect",
    /* OPT_COLOR_NOTFOUND           18 */  L"color_notfound",
    /* OPT_COLOR_NOTREGEXP          19 */  L"color_notregexp",
    /* OPT_COLOR_EOF                20 */  L"color_eof",
    /* OPT_COLOR_HIGHLIGHT          21 */  L"color_highlight",
    /* OPT_HIGHLIGHT_MARK_ID        22 */  L"highlight_mark_id",
    /* OPT_HIGHLIGHT_STATE          23 */  L"highlight_state",
    /* OPT_USE_ALT_HOTKEYS          24 */  L"use_alt_hotkeys",
    /* OPT_ALT_MATCHCASE            25 */  L"alt_match_case",
    /* OPT_ALT_WHOLEWORD            26 */  L"alt_whole_word",
    /* OPT_ALT_SEARCHMODE           27 */  L"alt_search_mode",
    /* OPT_ALT_HIGHLIGHTALL         28 */  L"alt_highlight_all",
    /* OPT_FIND_HISTORY_ITEMS       29 */  L"find_history_items",
    /* OPT_HISTORY_SAVE             30 */  L"history_save",
    /* OPT_NEW_UI                   31 */  L"new_ui",
    /* OPT_SELECT_BY_F3             32 */  L"select_by_f3",
    /* OPT_SELECT_BY_FND            33 */  L"select_by_fnd",
    /* OPT_SELECT_BY_SELFND         34 */  L"select_by_selfnd",
    /* OPT_ADJ_INCOMPL_REGEXP       35 */  L"adj_incompl_regexp",
    /* OPT_FINDALL_MODE             36 */  L"findall_mode",
    /* OPT_FINDALL_RESULT           37 */  L"findall_result",
    /* OPT_FINDALL_COUNT_DELAY      38 */  L"findall_count_delay",
    /* OPT_LOGOUTPUT_FRP_MODE       39 */  L"logoutput_frp_mode",
    /* OPT_LOGOUTPUT_FRP_BEFORE     40 */  L"logoutput_frp_before",
    /* OPT_LOGOUTPUT_FRP_AFTER      41 */  L"logoutput_frp_after",
    /* OPT_LOGOUTPUT_FRP_HIGHLIGHT  42 */  L"logoutput_frp_highlight",
    /* OPT_FILEOUTPUT_FRP_MODE      43 */  L"fileoutput_frp_mode",
    /* OPT_FILEOUTPUT_FRP_BEFORE    44 */  L"fileoutput_frp_before",
    /* OPT_FILEOUTPUT_FRP_AFTER     45 */  L"fileoutput_frp_after",
    /* OPT_FILEOUTPUT_FRP_HIGHLIGHT 46 */  L"fileoutput_frp_highlight",
    /* OPT_EDIT_MINWIDTH            47 */  L"edit_minwidth",
    /* OPT_EDIT_MAXWIDTH            48 */  L"edit_maxwidth"
};


// vars
PluginState     g_Plugin;
QSearchDlgState g_QSearchDlg;
QSearchOpt      g_Options;
QSearchOpt      g_Options0;
wchar_t         g_szFunctionQSearchW[128] = { 0 };
BOOL            g_bHighlightPlugin = FALSE;
BOOL            g_bLogPlugin = FALSE;
BOOL            g_bFrameActivated = FALSE;
BOOL            g_bWordJustSelectedByFnd = FALSE;


// funcs
void  ReadOptions(void);
void  SaveOptions(void);
void  readBinaryA(HANDLE, const char*, void*, DWORD);
void  readBinaryW(HANDLE, const wchar_t*, void*, DWORD);
DWORD readDwordA(HANDLE, const char*, DWORD);
DWORD readDwordW(HANDLE, const wchar_t*, DWORD);
void  writeBinaryA(HANDLE, const char*, const void*, DWORD);
void  writeBinaryW(HANDLE, const wchar_t*, const void*, DWORD);
void  writeDwordA(HANDLE, const char*, DWORD);
void  writeDwordW(HANDLE, const wchar_t*, DWORD);
LRESULT CALLBACK NewEditProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK NewMainProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK NewFrameProc(HWND, UINT, WPARAM, LPARAM);
void CheckEditNotification(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


static BOOL IsExtCallParamValid(LPARAM lParam, int nIndex)
{
    if ( lParam )
    {
        if ( *((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)) )
            return TRUE;
    }
    return FALSE;
}

static INT_PTR GetExtCallParam(LPARAM lParam, int nIndex)
{
    if ( IsExtCallParamValid(lParam, nIndex) )
        return *(((INT_PTR *)lParam) + nIndex);
    return 0;
}

static int doQSearch(PLUGINDATA* pd, BOOL bInternalCall)
{
    if ( !g_Plugin.bInitialized )
    {
        Initialize(pd);
    }

    if ( !bInternalCall )
    {
        if ( pd->bOldWindows && (0 == ((LPSTR) g_szFunctionQSearchW)[0]) )
        {
            lstrcpyA( (LPSTR) g_szFunctionQSearchW, (LPCSTR) pd->pFunction );
        }
        else if ( 0 == ((LPWSTR) g_szFunctionQSearchW)[0] )
        {
            lstrcpyW( (LPWSTR) g_szFunctionQSearchW, (LPCWSTR) pd->pFunction );
        }
    }

    if ( !g_QSearchDlg.hDlg )
    {
        int iddQSearch;

        switch ( g_Options.dwNewUI )
        {
            case QS_UI_NEW_02:
                iddQSearch = IDD_QSEARCH_NEW_02;
                break;
            case QS_UI_NEW_01:
                iddQSearch = IDD_QSEARCH_NEW_01;
                break;
            default:
                iddQSearch = IDD_QSEARCH;
                break;
        }

        if ( g_Plugin.bOldWindows )
        {
            g_QSearchDlg.hDlg = CreateDialogA(
                g_Plugin.hInstanceDLL,
                MAKEINTRESOURCEA(iddQSearch),
                g_Plugin.hMainWnd,
                qsearchDlgProc );
        }
        else
        {
            g_QSearchDlg.hDlg = CreateDialogW(
                g_Plugin.hInstanceDLL,
                MAKEINTRESOURCEW(iddQSearch),
                g_Plugin.hMainWnd,
                qsearchDlgProc );
        }

        if ( g_QSearchDlg.hDlg )
        {
            if ( g_QSearchDlg.pDockData )
            {
                RECT rcDlg;

                if ( GetWindowRect(g_QSearchDlg.hDlg, &rcDlg) )
                {
                    RECT* prcDockSize;

                    prcDockSize = &g_QSearchDlg.pDockData->rcSize;
                    //g_QSearchDlg.bOnDlgStart = TRUE;
                    if ( (prcDockSize->right == 0) || (prcDockSize->bottom == 0) ||
                         (prcDockSize->bottom - prcDockSize->top != rcDlg.bottom - rcDlg.top) )
                    {
                        prcDockSize->right = rcDlg.right - rcDlg.left;
                        prcDockSize->bottom = rcDlg.bottom - rcDlg.top;
                        prcDockSize->left = 0;
                        prcDockSize->top = 0;
                    }
                }
                g_QSearchDlg.pDockData->hWnd = g_QSearchDlg.hDlg;
                SendMessage( g_Plugin.hMainWnd, AKD_DOCK,
                  DK_SUBCLASS, (LPARAM) g_QSearchDlg.pDockData );
                if ( !bInternalCall )
                {
                    if ( g_Plugin.nDockedDlgIsWaitingForOnStart != 1 )
                    {
                        if ( g_QSearchDlg.uWmShowFlags == QS_SF_DLGSWITCH )
                            g_QSearchDlg.uWmShowFlags = 0; // forbid to pick up selected text on WM_SHOWWINDOW
                        else
                            g_QSearchDlg.uWmShowFlags = QS_SF_CANPICKUPSELTEXT; // allow to pick up selected text on WM_SHOWWINDOW

                        SendMessage( g_Plugin.hMainWnd, AKD_DOCK,
                          DK_SHOW, (LPARAM) g_QSearchDlg.pDockData );

                        g_QSearchDlg.uWmShowFlags = 0;
                    }
                    else
                        g_QSearchDlg.pDockData->dwFlags |= DKF_HIDDEN;
                }
            }
            else
            {
                SendMessage(g_QSearchDlg.hDlg, QSM_QUIT, 0, 0);
            }
        }
    }
    else if ( !bInternalCall )
    {
        if ( g_Options.dwFlags[OPTF_HOTKEY_HIDES_PANEL] & 0x01 )
        {
            if ( IsWindowVisible(g_QSearchDlg.hDlg) )
            {
                BOOL bHide = TRUE;

                if ( g_Options.dwFlags[OPTF_HOTKEY_HIDES_PANEL] == HOTKEY_HIDES_PANEL_FOCUSED )
                {
                    HWND hFocusedWnd = GetFocus();
                    if ( (hFocusedWnd != g_QSearchDlg.hDlg) && !IsChild(g_QSearchDlg.hDlg, hFocusedWnd) )
                    {
                        bHide = FALSE;
                    }
                }

                if (bHide)
                {
                    SendMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, FALSE, 0 );

                    // Stay in memory, and show as non-active
                    return UD_NONUNLOAD_NONACTIVE;
                }
            }
        }
        SendMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, TRUE, QS_SF_CANPICKUPSELTEXT ); // allow to pick up selected text
    }

    // Stay in memory, and show as active
    return UD_NONUNLOAD_ACTIVE;
}

#define  SELECT_MODE_NONE    0x00
#define  SELECT_MODE_F3      0x01
#define  SELECT_MODE_FND     0x02
#define  SELECT_MODE_SELFND  0x04
#define  SELECT_MODE_FORCE   0x10

#define  SCW_FALSE           0x00  //  did not do anything
#define  SCW_TRUE            0x01  //  tried to select a word
#define  SCW_WORDSELECTED    0x02  //  selected a word

static UINT doSelectCurrentWord(HWND hEdit, UINT uSelectMode, CHARRANGE_X* pcrNewSelection)
{
    DWORD dwSelectWord;

    g_bWordJustSelectedByFnd = FALSE;

    if ( uSelectMode & SELECT_MODE_F3 )
        dwSelectWord = g_Options.dwSelectByF3;
    else if ( uSelectMode & SELECT_MODE_FND )
        dwSelectWord = g_Options.dwSelectByFnd;
    else if ( uSelectMode & SELECT_MODE_SELFND )
        dwSelectWord = g_Options.dwSelectBySelFnd;
    else
        dwSelectWord = 1; // enabled

    if ( dwSelectWord != 0 )
    {
        CHARRANGE_X cr = {0, 0};

        SendMessage( hEdit, EM_EXGETSEL_X, 0, (LPARAM) &cr );
        if ( (uSelectMode & SELECT_MODE_FORCE) || (cr.cpMin == cr.cpMax) )
        {
            // Implemented acccording to "WordCaretSelect.js" by FeyFre & VladSh
            // cr.cpMin==nPos; crWord.cpMin==nWordBeg; crWord.cpMax==nWordEnd
            UINT nResult = SCW_TRUE;
            CHARRANGE_X crWord;

            crWord.cpMin = (INT_X) SendMessage(hEdit, EM_FINDWORDBREAK, WB_LEFT, cr.cpMin);
            crWord.cpMax = (INT_X) SendMessage(hEdit, EM_FINDWORDBREAK, WB_RIGHTBREAK, crWord.cpMin);
            if ( crWord.cpMax < cr.cpMin )
            {
                // caret is located on word start position i.e. "prev-word |word-to-copy"
                crWord.cpMin = (INT_X) SendMessage(hEdit, EM_FINDWORDBREAK, WB_LEFT, cr.cpMin + 1);
                crWord.cpMax = (INT_X) SendMessage(hEdit, EM_FINDWORDBREAK, WB_RIGHTBREAK, crWord.cpMin);
            }
            if ( (crWord.cpMax >= cr.cpMin) &&
                 ( (dwSelectWord == 1) ||
                   ((cr.cpMin > crWord.cpMin) && (cr.cpMin < crWord.cpMax)) )
               )
            {
                SendMessage( hEdit, EM_EXSETSEL_X, 0, (LPARAM) &crWord );
                if ( pcrNewSelection )
                {
                    pcrNewSelection->cpMin = crWord.cpMin;
                    pcrNewSelection->cpMax = crWord.cpMax;
                }
                if ( crWord.cpMin != crWord.cpMax )
                {
                    nResult |= SCW_WORDSELECTED; // selected a word
                }
            }
            else
            {
                if ( pcrNewSelection )
                {
                    pcrNewSelection->cpMin = cr.cpMin;
                    pcrNewSelection->cpMax = cr.cpMax;
                }
            }

            return nResult;
        }
    }

    return SCW_FALSE;
}

// Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
    pv->dwAkelDllVersion  = AKEL_DLL_VERSION;
    pv->dwExeMinVersion3x = EXE_MIN_VERSION_3X;
    pv->dwExeMinVersion4x = EXE_MIN_VERSION_4X;
    pv->pPluginName = PLUGIN_NAME;
}

// Plugin extern function
void __declspec(dllexport) QSearch(PLUGINDATA* pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    g_Plugin.nDockedDlgIsWaitingForOnStart = pd->bOnStart ? 1 : 0;

    pd->nUnload = doQSearch(pd, FALSE);
}

// Plugin extern function
void __declspec(dllexport) FindNext(PLUGINDATA* pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    doQSearch(pd, TRUE);
    if ( !pd->bOnStart )
    {
        if ( g_QSearchDlg.hDlg )
        {
            UINT uSelectMode = SELECT_MODE_NONE;
            UINT nWordSelected = 0;

            switch ( GetExtCallParam(pd->lParam, 1) )
            {
                case 1: // Pick up the selected text
                    uSelectMode = SELECT_MODE_FND;
                    break;
                case 0: // default: Find Next
                    if ( g_Options.dwFlags[OPTF_CATCH_MAIN_F3] &&
                         (g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01) )
                    {
                        uSelectMode = SELECT_MODE_F3;
                    }
                    break;
            }

            if ( uSelectMode != SELECT_MODE_NONE )
            {
                UINT nPickedUp = 0;
                nWordSelected = doSelectCurrentWord(pd->hWndEdit, uSelectMode, NULL);
                SendMessage( g_QSearchDlg.hDlg, QSM_PICKUPSELTEXT, QS_PS_UPDATEHISTORY, (LPARAM) &nPickedUp );
                if ( nPickedUp & QS_PSF_TEXTCHANGED )
                {
                    SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_SETINFOEMPTY );
                }
                if ( uSelectMode == SELECT_MODE_FND && (nWordSelected & SCW_WORDSELECTED) )
                {
                    g_bWordJustSelectedByFnd = TRUE;
                    g_QSearchDlg.bQSearching = TRUE;
                }
            }

            if ( (nWordSelected == 0) ||
                 ((uSelectMode == SELECT_MODE_F3) && !(nWordSelected & SCW_WORDSELECTED)) )
            {
                if ( g_bFrameActivated )
                {
                    g_bFrameActivated = FALSE;
                    SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_FORCEFINDFIRST );
                }
                SendMessage( g_QSearchDlg.hDlg, QSM_FINDNEXT, FALSE, QS_FF_NOSETSELFIRST );
            }
        }
    }

    // Stay in memory, and show as non-active
    pd->nUnload = UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
void __declspec(dllexport) FindPrev(PLUGINDATA* pd)
{
    pd->dwSupport |= PDS_NOAUTOLOAD;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    doQSearch(pd, TRUE);
    if ( g_QSearchDlg.hDlg )
    {
        UINT uSelectMode = SELECT_MODE_NONE;
        UINT nWordSelected = 0;

        switch ( GetExtCallParam(pd->lParam, 1) )
        {
            case 1: // Pick up the selected text
                uSelectMode = SELECT_MODE_FND;
                break;
            case 0: // default: Find Prev
                if ( g_Options.dwFlags[OPTF_CATCH_MAIN_F3] &&
                     (g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01) )
                {
                    uSelectMode = SELECT_MODE_F3;
                }
                break;
        }

        if ( uSelectMode != SELECT_MODE_NONE )
        {
            UINT nPickedUp = 0;
            nWordSelected = doSelectCurrentWord(pd->hWndEdit, uSelectMode, NULL);
            SendMessage( g_QSearchDlg.hDlg, QSM_PICKUPSELTEXT, QS_PS_UPDATEHISTORY, (LPARAM) &nPickedUp );
            if ( nPickedUp & QS_PSF_TEXTCHANGED )
            {
                SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_SETINFOEMPTY );
            }
            if ( uSelectMode == SELECT_MODE_FND && (nWordSelected & SCW_WORDSELECTED) )
            {
                g_bWordJustSelectedByFnd = TRUE;
                g_QSearchDlg.bQSearching = TRUE;
            }
        }

        if ( (nWordSelected == 0) ||
             ((uSelectMode == SELECT_MODE_F3) && !(nWordSelected & SCW_WORDSELECTED)) )
        {
            if ( g_bFrameActivated )
            {
                g_bFrameActivated = FALSE;
                SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_FORCEFINDFIRST );
            }
            SendMessage( g_QSearchDlg.hDlg, QSM_FINDNEXT, TRUE, QS_FF_NOSETSELFIRST );
        }
    }

    // Stay in memory, and show as non-active
    pd->nUnload = UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
void __declspec(dllexport) FindAll(PLUGINDATA* pd)
{
    pd->dwSupport |= PDS_NOAUTOLOAD;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    doQSearch(pd, TRUE);
    if ( g_QSearchDlg.hDlg )
    {
        EDITINFO ei;
        DWORD dwAdditionalFlags = 0;
        UINT uSelectMode = SELECT_MODE_NONE;
        UINT nWordSelected = 0;

        switch ( GetExtCallParam(pd->lParam, 1) )
        {
            case 1: // Pick up the selected text
                uSelectMode = SELECT_MODE_FND;
                break;
            case 2: // Find All in All Files
                dwAdditionalFlags = QS_FINDALL_RSLT_ALLFILES;
                break;
            case 3: // Pick up the selected text + Find All in All Files
                uSelectMode = SELECT_MODE_FND;
                dwAdditionalFlags = QS_FINDALL_RSLT_ALLFILES;
                break;
            case 0: // default: Find All
                if ( g_Options.dwFlags[OPTF_CATCH_MAIN_F3] &&
                     (g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01) )
                {
                    uSelectMode = SELECT_MODE_F3;
                }
                break;
        }

        if ( uSelectMode != SELECT_MODE_NONE )
        {
            UINT nPickedUp = 0;
            nWordSelected = doSelectCurrentWord(pd->hWndEdit, uSelectMode, NULL);
            SendMessage( g_QSearchDlg.hDlg, QSM_PICKUPSELTEXT, QS_PS_UPDATEHISTORY, (LPARAM) &nPickedUp );
            if ( nPickedUp & QS_PSF_TEXTCHANGED )
                SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_SETINFOEMPTY );
        }

        if ( (nWordSelected == 0) ||
             ((uSelectMode == SELECT_MODE_F3) && !(nWordSelected & SCW_WORDSELECTED)) )
        {
            if ( g_bFrameActivated )
            {
                g_bFrameActivated = FALSE;
                SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_FORCEFINDFIRST );
            }
            SendMessage( g_QSearchDlg.hDlg, QSM_FINDALL, g_Options.dwFindAllMode | dwAdditionalFlags, 0 );

            ei.hWndEdit = NULL;
            SendMessage( g_Plugin.hMainWnd, AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei );
            if ( ei.hWndEdit )
            {
               SetFocus(ei.hWndEdit);
            }
        }
    }

    // Stay in memory, and show as non-active
    pd->nUnload = UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
void __declspec(dllexport) SelFindNext(PLUGINDATA* pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    if ( pd->bOnStart )
    {
        doQSearch(pd, TRUE);
        /*
        g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] |= 0x01;
        g_Options.dwFlags[OPTF_CATCH_MAIN_F3] |= 1;
        */
    }
    else
    {
        if ( !doSelectCurrentWord(pd->hWndEdit, SELECT_MODE_SELFND, NULL) )
        {
            doQSearch(pd, TRUE);
            if ( g_QSearchDlg.hDlg )
            {
                SendMessage( g_QSearchDlg.hDlg, QSM_SELFIND, FALSE, 0 );
            }
        }
    }

    // Stay in memory, and show as non-active
    pd->nUnload = UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
void __declspec(dllexport) SelFindPrev(PLUGINDATA* pd)
{
    pd->dwSupport |= PDS_NOAUTOLOAD;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    if ( !doSelectCurrentWord(pd->hWndEdit, SELECT_MODE_SELFND, NULL) )
    {
        doQSearch(pd, TRUE);
        if ( g_QSearchDlg.hDlg )
        {
            SendMessage( g_QSearchDlg.hDlg, QSM_SELFIND, TRUE, 0 );
        }
    }

    // Stay in memory, and show as non-active
    pd->nUnload = UD_NONUNLOAD_NONACTIVE;
}

// Plugin extern function
void __declspec(dllexport) DialogSwitcher(PLUGINDATA* pd)
{
    pd->dwSupport |= PDS_SUPPORTALL;
    if ( pd->dwSupport & PDS_GETSUPPORT )
        return;

    if ( dlgswtchIsActive() )
        dlgswtchUninitialize();
    else
        dlgswtchInitialize(pd);

    if ( dlgswtchIsActive() ) // ensure it's active
        pd->nUnload = UD_NONUNLOAD_ACTIVE;
    else
        pd->nUnload = UD_NONUNLOAD_NONACTIVE;
}

// Entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH:
            g_szFunctionQSearchW[0] = 0;
            initializePluginState( &g_Plugin );
            initializeQSearchDlgState( &g_QSearchDlg );
            initializeOptions( &g_Options );
            initializeOptions( &g_Options0 );
            break;
        case DLL_PROCESS_DETACH:
#if TEST_UNINIT
            WriteLog("DLL_PROCESS_DETACH: Before dlgswtchUninitialize()\n");
#endif
            dlgswtchUninitialize();
#if TEST_UNINIT
            WriteLog("DLL_PROCESS_DETACH: After dlgswtchUninitialize()\n");
#endif
            Uninitialize();
#if TEST_UNINIT
            WriteLog("DLL_PROCESS_DETACH: After Uninitialize()\n");
            CloseLog();
#endif
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

static LANGID getAkelPadLang(PLUGINDATA* pd)
{
    /*if ( pd->bOldWindows )
    {
        if ( pd->szLangModule && pd->szLangModule[0] )
        {
            char szModule[5];

            lstrcpynA( szModule, pd->szLangModule, 4 );
            CharUpperA( szModule );
            if ( lstrcmpA(szModule, "ENG") == 0 )
                return LANG_ENGLISH;
            if ( lstrcmpA(szModule, "RUS") == 0 )
                return LANG_RUSSIAN;
            if ( lstrcmpA(szModule, "UKR") == 0 )
                return LANG_UKRAINIAN;
        }
    }
    else
    {
        if ( pd->wszLangModule && pd->wszLangModule[0] )
        {
            wchar_t wszModule[5];

            lstrcpynW( wszModule, pd->wszLangModule, 4 );
            CharUpperW( wszModule );
            if ( lstrcmpW(wszModule, L"ENG") == 0 )
                return LANG_ENGLISH;
            if ( lstrcmpW(wszModule, L"RUS") == 0 )
                return LANG_RUSSIAN;
            if ( lstrcmpW(wszModule, L"UKR") == 0 )
                return LANG_UKRAINIAN;
        }
    }*/

    return (pd->wLangModule ? pd->wLangModule : pd->wLangSystem);
}

static BOOL findHighlightPlugin(PLUGINDATA* pd)
{
    BOOL bExists = FALSE;

    /*if ( pd->bOldWindows )
    {
        char             szPluginPath[2*MAX_PATH + 1];
        HANDLE           hFind;
        WIN32_FIND_DATAA findData;

        lstrcpyA( szPluginPath, (const char *) pd->pAkelDir );
        lstrcatA( szPluginPath, "\\AkelFiles\\Plugs\\Coder.dll" );

        hFind = FindFirstFileA(szPluginPath, &findData);
        if ( hFind && (hFind != INVALID_HANDLE_VALUE) )
        {
            if ( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                bExists = TRUE;
            }
            FindClose(hFind);
        }
    }
    else
    {
        wchar_t          szPluginPath[2*MAX_PATH + 1];
        HANDLE           hFind;
        WIN32_FIND_DATAW findData;

        lstrcpyW( szPluginPath, (const wchar_t *) pd->pAkelDir );
        lstrcatW( szPluginPath, L"\\AkelFiles\\Plugs\\Coder.dll" );

        hFind = FindFirstFileW(szPluginPath, &findData);
        if ( hFind && (hFind != INVALID_HANDLE_VALUE) )
        {
            if ( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                bExists = TRUE;
            }
            FindClose(hFind);
        }
    }*/

    return bExists;
}

static BOOL findLogPlugin(PLUGINDATA* pd)
{
    BOOL bExists = FALSE;

    if ( pd->bOldWindows )
    {
        char             szPluginPath[2*MAX_PATH + 1];
        HANDLE           hFind;
        WIN32_FIND_DATAA findData;

        lstrcpyA( szPluginPath, (const char *) pd->pAkelDir );
        lstrcatA( szPluginPath, "\\AkelFiles\\Plugs\\Log.dll" );

        hFind = FindFirstFileA(szPluginPath, &findData);
        if ( hFind && (hFind != INVALID_HANDLE_VALUE) )
        {
            if ( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                bExists = TRUE;
            }
            FindClose(hFind);
        }
    }
    else
    {
        wchar_t          szPluginPath[2*MAX_PATH + 1];
        HANDLE           hFind;
        WIN32_FIND_DATAW findData;

        lstrcpyW( szPluginPath, (const wchar_t *) pd->pAkelDir );
        lstrcatW( szPluginPath, L"\\AkelFiles\\Plugs\\Log.dll" );

        hFind = FindFirstFileW(szPluginPath, &findData);
        if ( hFind && (hFind != INVALID_HANDLE_VALUE) )
        {
            if ( !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                bExists = TRUE;
            }
            FindClose(hFind);
        }
    }

    return bExists;
}

void Initialize(PLUGINDATA* pd)
{
    if ( !g_Plugin.bInitialized )
    {
        DOCK dk;

        x_zero_mem(&dk, sizeof(DOCK));

        g_Plugin.bOldWindows  = pd->bOldWindows;
        g_Plugin.bOldRichEdit = pd->bOldRichEdit;
        g_Plugin.nMDI         = pd->nMDI;
        g_Plugin.hInstanceDLL = pd->hInstanceDLL;
        g_Plugin.hMainWnd     = pd->hMainWnd;
        g_Plugin.bAkelEdit    = pd->bAkelEdit;
        g_Plugin.wLangSystem  = getAkelPadLang(pd);

        g_bHighlightPlugin = findHighlightPlugin(pd);
        g_bLogPlugin = findLogPlugin(pd);

        ReadOptions();

        // SubClass edit window
        g_Plugin.pEditProcData = NULL;
        SendMessage( g_Plugin.hMainWnd, AKD_SETEDITPROC,
          (WPARAM) NewEditProc, (LPARAM) &g_Plugin.pEditProcData );

        // SubClass main window
        g_Plugin.pMainProcData = NULL;
        SendMessage( g_Plugin.hMainWnd, AKD_SETMAINPROC,
          (WPARAM) NewMainProc, (LPARAM) &g_Plugin.pMainProcData );

        if ( g_Plugin.nMDI == WMD_MDI )
        {
            // SubClass frame window
            g_Plugin.pFrameProcData = NULL;
            SendMessage( g_Plugin.hMainWnd, AKD_SETFRAMEPROC,
              (WPARAM) NewFrameProc, (LPARAM) &g_Plugin.pFrameProcData );
        }

        dk.nSide = g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] ? DKS_TOP : DKS_BOTTOM;
        dk.dwFlags = DKF_DRAGDROP | DKF_NODROPLEFT | DKF_NODROPRIGHT | DKF_FIXEDSIZE | DKF_HIDDEN;
        if ( !g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] )
        {
            dk.rcSize.left = g_Options.dockRect.left;
            dk.rcSize.right = g_Options.dockRect.right;
            dk.rcSize.top = g_Options.dockRect.top;
            dk.rcSize.bottom = g_Options.dockRect.bottom;
        }
        g_QSearchDlg.pDockData = (DOCK *) SendMessage(
          g_Plugin.hMainWnd, AKD_DOCK, DK_ADD, (LPARAM) &dk );

    }
    g_Plugin.bInitialized = TRUE;
}

void Uninitialize(void)
{
#if TEST_UNINIT
    WriteLog("Entering Uninitialize()\n");
#endif

    if ( g_Plugin.bInitialized )
    {
#if TEST_UNINIT
        WriteLog("Uninitialize(): Is Initialized\n");
#endif

        SaveOptions();

        if ( g_Plugin.pEditProcData )
        {
            // Remove subclass (edit window)
            SendMessage( g_Plugin.hMainWnd, AKD_SETEDITPROC,
              (WPARAM) 0, (LPARAM) &g_Plugin.pEditProcData );
            g_Plugin.pEditProcData = NULL;
        }

        if ( g_Plugin.pMainProcData )
        {
            // Remove subclass (main window)
            SendMessage( g_Plugin.hMainWnd, AKD_SETMAINPROC,
              (WPARAM) 0, (LPARAM) &g_Plugin.pMainProcData );
            g_Plugin.pMainProcData = NULL;
        }

        if ( (g_Plugin.nMDI == WMD_MDI) && g_Plugin.pFrameProcData )
        {
            // Remove subclass (frame window)
            SendMessage( g_Plugin.hMainWnd, AKD_SETFRAMEPROC,
              (WPARAM) 0, (LPARAM) &g_Plugin.pFrameProcData );
            g_Plugin.pFrameProcData = NULL;
        }

        if ( g_QSearchDlg.hDlg )
        {
            SendMessage(g_QSearchDlg.hDlg, QSM_QUIT, 0, 0);
        }

        if ( g_QSearchDlg.pDockData )
        {
            SendMessage( g_Plugin.hMainWnd, AKD_DOCK,
              DK_DELETE, (LPARAM) g_QSearchDlg.pDockData );
        }

        g_Plugin.bInitialized = FALSE;
    }
#if TEST_UNINIT
    else
        WriteLog("Uninitialize(): Is Not Initialized\n");
#endif

#if TEST_UNINIT
    WriteLog("Exiting Uninitialize()\n");
#endif
}

#define xbr_diff(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

static BOOL GetLineAtIndex(HWND hWndEdit, const AECHARINDEX* ciChar, AETEXTRANGEW* tr, tDynamicBuffer* pTextBuf);
static BOOL PatOpenLine(HWND hWndEdit);

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg )
    {
        case WM_MOUSEMOVE:
            if ( g_Options.dwFlags[OPTF_EDITOR_AUTOFOCUS_MOUSE] & 0x01 )
            {
                if ( g_QSearchDlg.hDlg && !g_QSearchDlg.bIsQSearchingRightNow )
                {
                    BOOL bCanBeFocused = g_QSearchDlg.bMouseJustLeavedFindEdit;
                    if ( !bCanBeFocused )
                    {
                        if ( (g_Options.dwFlags[OPTF_QSEARCH_AUTOFOCUS_MOUSE] & 0x01) == 0 )
                        {
                            static WORD f = 0;
                            static WORD x = 0;
                            static WORD y = 0;
                            WORD xx, yy;

                            xx = LOWORD(lParam);
                            yy = HIWORD(lParam);

                            if ( f != 0 )
                            {
                                if ( xbr_diff(x, xx) >= 2 )
                                {
                                    bCanBeFocused = TRUE;
                                }
                                else if ( xbr_diff(y, yy) >= 2 )
                                {
                                    bCanBeFocused = TRUE;
                                }
                                if ( bCanBeFocused )
                                {
                                    x = xx;
                                    y = yy;
                                }
                            }
                            else
                            {
                                f = 1;
                                x = xx;
                                y = yy;
                            }
                        }
                    }

                    if ( bCanBeFocused )
                    {
                        HWND hFocusedWnd = GetFocus();
                        // When AkelPad's window is not active, GetFocus() returns NULL
                        if ( hFocusedWnd && hFocusedWnd != hWnd )
                        {
                            if ( (hFocusedWnd == g_QSearchDlg.hFindEdit) ||
                                 (GetParent(hFocusedWnd) == g_QSearchDlg.hDlg) )
                            {
                                SetFocus(hWnd);
                                if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
                                {
                                    CHARRANGE_X cr = {0, 0};

                                    SendMessage( hWnd, EM_EXGETSEL_X, 0, (LPARAM) &cr );
                                    if ( cr.cpMin != cr.cpMax )
                                    {
                                        if ( g_QSearchDlg.uSearchOrigin == QS_SO_QSEARCH_FORCED )
                                            g_QSearchDlg.uSearchOrigin = QS_SO_QSEARCH;
                                        else
                                            g_QSearchDlg.uSearchOrigin = QS_SO_EDITOR;
                                    }
                                }
                            }
                        }
                        g_QSearchDlg.bMouseJustLeavedFindEdit = FALSE;
                        //#ifdef _DEBUG
                        //  Debug_Output("QSearch.c, NewEditProc, WM_MOUSEMOVE, g_QSearchDlg.MouseJustLeavedFindEdit = FALSE;\n");
                        //#endif
                    }
                }
            }
            break;

        case WM_LBUTTONDBLCLK:
            if ( g_QSearchDlg.pSearchResultsFrame &&
                 SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEISVALID, 0, (LPARAM) g_QSearchDlg.pSearchResultsFrame) )
            {
                const FRAMEDATA* pFrame;

                pFrame = (FRAMEDATA *) SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEFIND, FWF_BYEDITWINDOW, (LPARAM) hWnd);
                if ( pFrame == g_QSearchDlg.pSearchResultsFrame )
                {
                    if ( PatOpenLine(hWnd) )
                        return 0;
                }
            }
            break;
    }

    if ( g_Plugin.pEditProcData && g_Plugin.pEditProcData->NextProc )
        return g_Plugin.pEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
    else
        return 0;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg )
    {
        case WM_COMMAND:
        {
            switch ( LOWORD(wParam) )
            {
                case IDM_FILE_EXIT:
                    if ( HIWORD(wParam) == 1 )
                    {
                        // this code works when Esc is pressed in AkelPad
                        if ( (g_Options.dwFlags[OPTF_CATCH_MAIN_ESC] & 0x01) &&
                             g_QSearchDlg.hDlg &&
                             IsWindowVisible(g_QSearchDlg.hDlg) )
                        {
                            SendMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, FALSE, 0 );
                            return 0;
                        }
                    }
                    break;
                case IDM_EDIT_FINDNEXTDOWN:
                case IDM_EDIT_FINDNEXTUP:
                    if ( g_Options.dwFlags[OPTF_CATCH_MAIN_F3] )
                    {
                        if ( g_QSearchDlg.bQSearching ||
                             (g_QSearchDlg.hDlg && (g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01)) )
                        {
                            WPARAM bFindPrev;

                            if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
                            {
                                EDITINFO  ei;

                                ei.hWndEdit = NULL;
                                SendMessage( g_Plugin.hMainWnd,
                                  AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei );
                                if ( ei.hWndEdit )
                                {
                                    if ( doSelectCurrentWord(ei.hWndEdit, SELECT_MODE_F3, NULL) & SCW_WORDSELECTED )
                                    {
                                        return 0; // just select current word - and nothing more
                                    }
                                }
                            }

                            bFindPrev = (LOWORD(wParam) == IDM_EDIT_FINDNEXTUP) ? TRUE : FALSE;
                            SendMessage( g_QSearchDlg.hDlg, QSM_FINDNEXT, bFindPrev, g_bWordJustSelectedByFnd ? QS_FF_NOSETSELFIRST : 0 );
                            g_bWordJustSelectedByFnd = FALSE;
                            return 0;
                        }
                    }
                    break;
                case IDM_EDIT_FIND:
                case IDM_EDIT_REPLACE:
                    // AkelPad's Find or Replace Dialog is shown
                    g_QSearchDlg.bQSearching = FALSE;
                    break;
                default:
                    break;
            }
            break;
        }
        case AKDN_HOTKEYGLOBAL:
        case AKDN_HOTKEY:
        {
            if ( g_Options.dwUseAltHotkeys && g_QSearchDlg.hDlg )
            {
                BYTE uModifier = HIBYTE(wParam);
                if ( uModifier == HOTKEYF_ALT )
                {
                    BYTE uKeyCode = LOBYTE(wParam);
                    if ( uKeyCode == g_Options.dwAltMatchCase ||
                         uKeyCode == g_Options.dwAltWholeWord ||
                         uKeyCode == g_Options.dwAltSearchMode ||
                         uKeyCode == g_Options.dwAltHighlightAll )
                    {
                        HWND hFocusedWnd = GetFocus();
                        if ( (hFocusedWnd == g_QSearchDlg.hFindEdit) ||
                             (GetParent(hFocusedWnd) == g_QSearchDlg.hDlg) )
                        {
                            int* pKeyProcessing = (int *) lParam;
                            *pKeyProcessing = 1; // stop processing
                            qsearchDlgOnAltHotkey(g_QSearchDlg.hDlg, uKeyCode);
                            return 0;
                        }
                    }
                }
            }
            break;
        }
        case AKDN_SIZE_ONSTART:
        {
            if ( g_QSearchDlg.pDockData )
            {
                if ( g_QSearchDlg.pDockData->nSide == DKS_TOP )
                    g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] = 1;
                else
                    g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] = 0;

                if ( g_Plugin.nDockedDlgIsWaitingForOnStart == 1 )
                {
                    // This code has been originally taken from HexSel plugin :)
                    // We get here when QSearch::QSearch is autoloaded
                    g_Plugin.nDockedDlgIsWaitingForOnStart = 2;
                    g_QSearchDlg.uWmShowFlags = 0; // forbid to pick up selected text on WM_SHOWWINDOW
                    ShowWindow(g_QSearchDlg.hDlg, SW_SHOW);
                    g_QSearchDlg.uWmShowFlags = 0; // just in case :)
                    g_QSearchDlg.pDockData->dwFlags &= ~DKF_HIDDEN;
                    g_Plugin.nDockedDlgIsWaitingForOnStart = 0;
                }
            }
            break;
        }
        case AKDN_FRAME_ACTIVATE:
        case AKDN_OPENDOCUMENT_FINISH:
        {
            if ( uMsg == AKDN_FRAME_ACTIVATE )
            {
                if ( g_Plugin.nMDI == WMD_PMDI )
                {
                    if ( g_QSearchDlg.hDlg )
                    {
                        g_QSearchDlg.uSearchOrigin = QS_SO_UNKNOWN;
                        SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_SETINFOEMPTY );
                    }
                }
                g_bFrameActivated = TRUE;
            }
            if ( g_Options.dwFlags[OPTF_QSEARCH_AUTOFOCUS_FILE] )
            {
                if ( g_QSearchDlg.hDlg && IsWindowVisible(g_QSearchDlg.hDlg) )
                {
                    LRESULT lResult = 0;

                    if ( g_Plugin.pMainProcData && g_Plugin.pMainProcData->NextProc )
                        lResult = g_Plugin.pMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

                    PostMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, TRUE, 0 );

                    return lResult;
                }
            }
            break;
        }
        case AKDN_FRAME_DESTROY:
        {
            const FRAMEDATA* pFrame = (FRAMEDATA *) lParam;
            if ( pFrame == g_QSearchDlg.pSearchResultsFrame )
            {
                g_QSearchDlg.pSearchResultsFrame = NULL;
            }
            break;
        }
        case AKDN_MAIN_ONFINISH:
        {
            LRESULT lResult = 0;

            if ( g_Plugin.pMainProcData && g_Plugin.pMainProcData->NextProc )
                lResult = g_Plugin.pMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

            g_Plugin.bAkelPadOnFinish = TRUE;

#if TEST_UNINIT
            WriteLog("AKDN_MAIN_ONFINISH: Before dlgswtchUninitialize()\n");
#endif
            dlgswtchUninitialize();
#if TEST_UNINIT
            WriteLog("AKDN_MAIN_ONFINISH: After dlgswtchUninitialize()\n");
#endif

            if ( g_QSearchDlg.hDlg )
            {
                SendMessage( g_QSearchDlg.hDlg, QSM_QUIT, 0, 0 );
            }

            Uninitialize();

#if TEST_UNINIT
            WriteLog("AKDN_MAIN_ONFINISH: After Uninitialize()\n");
            CloseLog();
#endif

            return lResult;
        }
        case AKDN_MAIN_ONSTART_FINISH:
        {
            if ( g_QSearchDlg.hDlg )
            {
                PostMessage(g_QSearchDlg.hDlg, QSM_CHECKHIGHLIGHT, 0, 0);
            }
            break;
        }
        default:
            break;
    }

    CheckEditNotification(hWnd, uMsg, wParam, lParam);

    if ( g_Plugin.pMainProcData && g_Plugin.pMainProcData->NextProc )
        return g_Plugin.pMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
    else
        return 0;
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg )
    {
        case WM_MDIACTIVATE:
            if ( g_QSearchDlg.hDlg )
            {
                g_QSearchDlg.uSearchOrigin = QS_SO_UNKNOWN;
                SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, QS_SNF_SETINFOEMPTY );
                g_bFrameActivated = TRUE;
            }
            break;
        default:
            break;
    }

    CheckEditNotification(hWnd, uMsg, wParam, lParam);

    if ( g_Plugin.pFrameProcData && g_Plugin.pFrameProcData->NextProc )
        return g_Plugin.pFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
    else
        return 0;
}

void CheckEditNotification(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( (uMsg == WM_NOTIFY) && (wParam == ID_EDIT) )
    {
        NMHDR* hdr = (NMHDR *) lParam;
        if ( g_Plugin.bAkelEdit )
        {
            if ( hdr->code == AEN_SELCHANGED )
            {
                if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
                {
                    AECHARRANGE* aeCrSel = &(((AENSELCHANGE *) hdr)->crSel);
                    if ( (aeCrSel->ciMin.nLine != aeCrSel->ciMax.nLine) ||
                         (aeCrSel->ciMin.nCharInLine != aeCrSel->ciMax.nCharInLine) )
                    {
                        if ( GetFocus() == hdr->hwndFrom )
                        {
                            // selection in the editor window
                            g_QSearchDlg.uSearchOrigin = QS_SO_EDITOR;
                        }
                    }
                }
            }
        }
        else
        {
            if ( hdr->code == EN_SELCHANGE )
            {
                if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
                {
                    if ( GetFocus() == hdr->hwndFrom )
                    {
                        CHARRANGE_X cr = {0, 0};

                        SendMessage( hdr->hwndFrom, EM_EXGETSEL_X, 0, (LPARAM) &cr );
                        if ( cr.cpMin != cr.cpMax )
                        {
                            // selection in the editor window
                            g_QSearchDlg.uSearchOrigin = QS_SO_EDITOR;
                        }
                    }
                }
            }
        }
    }
}

void ReadOptions(void)
{
    HANDLE hOptions;

    if ( g_Plugin.bOldWindows )
    {
        if ( hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
               AKD_BEGINOPTIONS, POB_READ, (LPARAM) CSZ_QSEARCH) )
        {
            int i;
            for ( i = 0; i < OPTF_COUNT; i++ )
            {
                g_Options.dwFlags[i] = readDwordA( hOptions,
                  CSZ_OPTIONS[i], WRONG_DWORD_VALUE );
            }

            if ( !g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] )
            {
                readBinaryA( hOptions, CSZ_OPTIONS[OPT_DOCK_RECT],
                  &g_Options.dockRect, sizeof(RECT) );
            }

            readBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_NOTFOUND],
              &g_Options.colorNotFound, sizeof(COLORREF) );

            readBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_NOTREGEXP],
              &g_Options.colorNotRegExp, sizeof(COLORREF) );

            readBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_EOF],
              &g_Options.colorEOF, sizeof(COLORREF) );

            readBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_HIGHLIGHT],
              &g_Options.colorHighlight, sizeof(COLORREF) );

            g_Options.dwHighlightMarkID = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_HIGHLIGHT_MARK_ID], WRONG_DWORD_VALUE );

            g_Options.dwHighlightState = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_HIGHLIGHT_STATE], WRONG_DWORD_VALUE );

            g_Options.dwUseAltHotkeys = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_USE_ALT_HOTKEYS], WRONG_DWORD_VALUE );

            g_Options.dwAltMatchCase = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_ALT_MATCHCASE], WRONG_DWORD_VALUE );

            g_Options.dwAltWholeWord = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_ALT_WHOLEWORD], WRONG_DWORD_VALUE );

            g_Options.dwAltSearchMode = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_ALT_SEARCHMODE], WRONG_DWORD_VALUE );

            g_Options.dwAltHighlightAll = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_ALT_HIGHLIGHTALL], WRONG_DWORD_VALUE );

            g_Options.dwFindHistoryItems = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FIND_HISTORY_ITEMS], WRONG_DWORD_VALUE );

            g_Options.dwHistorySave = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_HISTORY_SAVE], WRONG_DWORD_VALUE );

            g_Options.dwNewUI = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_NEW_UI], WRONG_DWORD_VALUE );

            g_Options.dwSelectByF3 = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_SELECT_BY_F3], WRONG_DWORD_VALUE );

            g_Options.dwSelectByFnd = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_SELECT_BY_FND], WRONG_DWORD_VALUE );

            g_Options.dwSelectBySelFnd = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_SELECT_BY_SELFND], WRONG_DWORD_VALUE );

            g_Options.dwAdjIncomplRegExp = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_ADJ_INCOMPL_REGEXP], WRONG_DWORD_VALUE );

            g_Options.dwFindAllMode = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FINDALL_MODE], WRONG_DWORD_VALUE );

            g_Options.dwFindAllResult = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FINDALL_RESULT], WRONG_DWORD_VALUE );

            g_Options.dwFindAllCountDelay = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FINDALL_COUNT_DELAY], WRONG_DWORD_VALUE );

            g_Options.LogOutputFRP.nMode = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_MODE], WRONG_INT_VALUE );

            g_Options.LogOutputFRP.nBefore = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_BEFORE], WRONG_INT_VALUE );

            g_Options.LogOutputFRP.nAfter = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_AFTER], WRONG_INT_VALUE );

            g_Options.LogOutputFRP.nHighlight = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_HIGHLIGHT], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nMode = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_MODE], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nBefore = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_BEFORE], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nAfter = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_AFTER], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nHighlight = (int) readDwordA( hOptions,
              CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_HIGHLIGHT], WRONG_INT_VALUE );

            g_Options.dwEditMinWidth = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_EDIT_MINWIDTH], WRONG_DWORD_VALUE );

            g_Options.dwEditMaxWidth = readDwordA( hOptions,
              CSZ_OPTIONS[OPT_EDIT_MAXWIDTH], WRONG_DWORD_VALUE );

            // all options have been read
            SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
        }
    }
    else
    {
        if ( hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
               AKD_BEGINOPTIONS, POB_READ, (LPARAM) CWSZ_QSEARCH) )
        {
            int i;
            for ( i = 0; i < OPTF_COUNT; i++ )
            {
                g_Options.dwFlags[i] = readDwordW( hOptions,
                  CWSZ_OPTIONS[i], WRONG_DWORD_VALUE );
            }

            if ( !g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] )
            {
                readBinaryW( hOptions, CWSZ_OPTIONS[OPT_DOCK_RECT],
                  &g_Options.dockRect, sizeof(RECT) );
            }

            readBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_NOTFOUND],
              &g_Options.colorNotFound, sizeof(COLORREF) );

            readBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_NOTREGEXP],
              &g_Options.colorNotRegExp, sizeof(COLORREF) );

            readBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_EOF],
              &g_Options.colorEOF, sizeof(COLORREF) );

            readBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_HIGHLIGHT],
              &g_Options.colorHighlight, sizeof(COLORREF) );

            g_Options.dwHighlightMarkID = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_HIGHLIGHT_MARK_ID], WRONG_DWORD_VALUE );

            g_Options.dwHighlightState = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_HIGHLIGHT_STATE], WRONG_DWORD_VALUE );

            g_Options.dwUseAltHotkeys = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_USE_ALT_HOTKEYS], WRONG_DWORD_VALUE );

            g_Options.dwAltMatchCase = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_ALT_MATCHCASE], WRONG_DWORD_VALUE );

            g_Options.dwAltWholeWord = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_ALT_WHOLEWORD], WRONG_DWORD_VALUE );

            g_Options.dwAltSearchMode = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_ALT_SEARCHMODE], WRONG_DWORD_VALUE );

            g_Options.dwAltHighlightAll = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_ALT_HIGHLIGHTALL], WRONG_DWORD_VALUE );

            g_Options.dwFindHistoryItems = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FIND_HISTORY_ITEMS], WRONG_DWORD_VALUE );

            g_Options.dwHistorySave = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_HISTORY_SAVE], WRONG_DWORD_VALUE );

            g_Options.dwNewUI = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_NEW_UI], WRONG_DWORD_VALUE );

            g_Options.dwSelectByF3 = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_SELECT_BY_F3], WRONG_DWORD_VALUE );

            g_Options.dwSelectByFnd = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_SELECT_BY_FND], WRONG_DWORD_VALUE );

            g_Options.dwSelectBySelFnd = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_SELECT_BY_SELFND], WRONG_DWORD_VALUE );

            g_Options.dwAdjIncomplRegExp = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_ADJ_INCOMPL_REGEXP], WRONG_DWORD_VALUE );

            g_Options.dwFindAllMode = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FINDALL_MODE], WRONG_DWORD_VALUE );

            g_Options.dwFindAllResult = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FINDALL_RESULT], WRONG_DWORD_VALUE );

            g_Options.dwFindAllCountDelay = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FINDALL_COUNT_DELAY], WRONG_DWORD_VALUE );

            g_Options.LogOutputFRP.nMode = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_MODE], WRONG_INT_VALUE );

            g_Options.LogOutputFRP.nHighlight = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_HIGHLIGHT], WRONG_INT_VALUE );

            g_Options.LogOutputFRP.nBefore = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_BEFORE], WRONG_INT_VALUE );

            g_Options.LogOutputFRP.nAfter = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_AFTER], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nMode = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_MODE], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nBefore = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_BEFORE], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nAfter = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_AFTER], WRONG_INT_VALUE );

            g_Options.FileOutputFRP.nHighlight = (int) readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_HIGHLIGHT], WRONG_INT_VALUE );

            g_Options.dwEditMinWidth = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_EDIT_MINWIDTH], WRONG_DWORD_VALUE );

            g_Options.dwEditMaxWidth = readDwordW( hOptions,
              CWSZ_OPTIONS[OPT_EDIT_MAXWIDTH], WRONG_DWORD_VALUE );

            // all options have been read
            SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
        }
    }
    copyOptions( &g_Options0, &g_Options );

    // normalize option values
    if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] = 1;

    /*if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] )
        g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 0;*/

    if ( g_Options.dwFlags[OPTF_SRCH_FROM_BEGINNING] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_FROM_BEGINNING] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_REGEXP_DOT_NEWLINE] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_REGEXP_DOT_NEWLINE] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] )
        g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_SELFIND_PICKUP] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_SELFIND_PICKUP] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_STOP_EOF] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_STOP_EOF] = 0;

    if ( g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] = 0;

    if ( g_Options.dwFlags[OPTF_EDIT_FOCUS_SELECTALL] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_EDIT_FOCUS_SELECTALL] = 0;

    if ( g_Options.dwFlags[OPTF_CATCH_MAIN_F3] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_CATCH_MAIN_F3] = 0;

    if ( g_Options.dwFlags[OPTF_CATCH_MAIN_ESC] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_CATCH_MAIN_ESC] = 0;

    if ( g_Options.dwFlags[OPTF_HOTKEY_HIDES_PANEL] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_HOTKEY_HIDES_PANEL] = 1;

    if ( g_Options.dwFlags[OPTF_EDITOR_AUTOFOCUS_MOUSE] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_EDITOR_AUTOFOCUS_MOUSE] = 0;

    if ( g_Options.dwFlags[OPTF_QSEARCH_AUTOFOCUS_MOUSE] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_QSEARCH_AUTOFOCUS_MOUSE] = 1;

    if ( g_Options.dwFlags[OPTF_QSEARCH_AUTOFOCUS_FILE] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_QSEARCH_AUTOFOCUS_FILE] = 0;

    if ( g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] == WRONG_DWORD_VALUE )
        g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] = 0;

    if ( g_Options.colorNotFound == WRONG_COLOR_VALUE )
        g_Options.colorNotFound = DEFAULT_COLOR_NOTFOUND;

    if ( g_Options.colorNotRegExp == WRONG_COLOR_VALUE )
        g_Options.colorNotRegExp = DEFAULT_COLOR_NOTREGEXP;

    if ( g_Options.colorEOF == WRONG_COLOR_VALUE )
        g_Options.colorEOF = DEFAULT_COLOR_EOF;

    if ( g_Options.colorHighlight == WRONG_COLOR_VALUE )
        g_Options.colorHighlight = DEFAULT_COLOR_HIGHLIGHT;

    if ( g_Options.dwHighlightMarkID == WRONG_DWORD_VALUE )
        g_Options.dwHighlightMarkID = DEFAULT_HIGHLIGHT_MARK_ID;

    if ( g_Options.dwHighlightState == WRONG_DWORD_VALUE )
        g_Options.dwHighlightState = 0;

    if ( g_Options.dwUseAltHotkeys == WRONG_DWORD_VALUE )
        g_Options.dwUseAltHotkeys = DEFAULT_USE_ALT_HOTKEYS;

    if ( g_Options.dwAltMatchCase == WRONG_DWORD_VALUE )
        g_Options.dwAltMatchCase = DEFAULT_ALT_MATCHCASE;

    if ( g_Options.dwAltWholeWord == WRONG_DWORD_VALUE )
        g_Options.dwAltWholeWord = DEFAULT_ALT_WHOLEWORD;

    if ( g_Options.dwAltSearchMode == WRONG_DWORD_VALUE )
        g_Options.dwAltSearchMode = DEFAULT_ALT_SEARCHMODE;

    if ( g_Options.dwAltHighlightAll == WRONG_DWORD_VALUE )
        g_Options.dwAltHighlightAll = DEFAULT_ALT_HIGHLIGHTALL;

    if ( g_Options.dwFindHistoryItems == WRONG_DWORD_VALUE )
        g_Options.dwFindHistoryItems = DEFAULT_FIND_HISTORY_ITEMS;
    if ( g_Options.dwFindHistoryItems < MIN_FIND_HISTORY_ITEMS )
        g_Options.dwFindHistoryItems = MIN_FIND_HISTORY_ITEMS;
    if ( g_Options.dwFindHistoryItems > MAX_FIND_HISTORY_ITEMS )
        g_Options.dwFindHistoryItems = MAX_FIND_HISTORY_ITEMS;

    if ( g_Options.dwHistorySave == WRONG_DWORD_VALUE )
        g_Options.dwHistorySave = DEFAULT_HISTORY_SAVE;

    if ( g_Options.dwNewUI == WRONG_DWORD_VALUE || g_Options.dwNewUI > QS_UI_NEW_02 )
        g_Options.dwNewUI = DEFAULT_QS_UI;

    if ( g_Options.dwSelectByF3 == WRONG_DWORD_VALUE )
        g_Options.dwSelectByF3 = DEFAULT_SELECT_BY_F3;

    if ( g_Options.dwSelectByFnd == WRONG_DWORD_VALUE )
        g_Options.dwSelectByFnd = DEFAULT_SELECT_BY_FND;

    if ( g_Options.dwSelectBySelFnd == WRONG_DWORD_VALUE )
        g_Options.dwSelectBySelFnd = DEFAULT_SELECT_BY_SELFND;

    if ( g_Options.dwAdjIncomplRegExp == WRONG_DWORD_VALUE )
        g_Options.dwAdjIncomplRegExp = DEFAULT_ADJ_INCOMPL_REGEXP;

    if ( g_Options.dwFindAllMode == WRONG_DWORD_VALUE )
        g_Options.dwFindAllMode = DEFAULT_FINDALL_MODE;
    if ( (g_Options.dwFindAllMode == QS_FINDALL_LOGOUTPUT) && !g_bLogPlugin )
        g_Options.dwFindAllMode = QS_FINDALL_FILEOUTPUT_SNGL;

    if ( g_Options.dwFindAllResult == WRONG_DWORD_VALUE )
        g_Options.dwFindAllResult = DEFAULT_FINDALL_RESULT;

    if ( g_Options.dwFindAllCountDelay == WRONG_DWORD_VALUE )
        g_Options.dwFindAllCountDelay = DEFAULT_FINDALL_COUNT_DELAY;

    if ( g_Options.LogOutputFRP.nMode == WRONG_INT_VALUE )
        g_Options.LogOutputFRP.nMode = LOGOUTPUT_FRP_MODE;

    if ( g_Options.LogOutputFRP.nBefore == WRONG_INT_VALUE )
        g_Options.LogOutputFRP.nBefore = LOGOUTPUT_FRP_BEFORE;

    if ( g_Options.LogOutputFRP.nAfter == WRONG_INT_VALUE )
        g_Options.LogOutputFRP.nAfter = LOGOUTPUT_FRP_AFTER;

    if ( g_Options.LogOutputFRP.nHighlight == WRONG_INT_VALUE )
        g_Options.LogOutputFRP.nHighlight = LOGOUTPUT_FRP_HIGHLIGHT;

    if ( g_Options.FileOutputFRP.nMode == WRONG_INT_VALUE )
        g_Options.FileOutputFRP.nMode = FILEOUTPUT_FRP_MODE;

    if ( g_Options.FileOutputFRP.nBefore == WRONG_INT_VALUE )
        g_Options.FileOutputFRP.nBefore = FILEOUTPUT_FRP_BEFORE;

    if ( g_Options.FileOutputFRP.nAfter == WRONG_INT_VALUE )
        g_Options.FileOutputFRP.nAfter = FILEOUTPUT_FRP_AFTER;

    if ( g_Options.FileOutputFRP.nHighlight == WRONG_INT_VALUE )
        g_Options.FileOutputFRP.nHighlight = FILEOUTPUT_FRP_HIGHLIGHT;

    if ( g_Options.dwEditMinWidth == WRONG_DWORD_VALUE )
        g_Options.dwEditMinWidth = DEFAULT_EDIT_MINWIDTH;

    if ( g_Options.dwEditMaxWidth == WRONG_DWORD_VALUE )
        g_Options.dwEditMaxWidth = DEFAULT_EDIT_MAXWIDTH;
}

void SaveOptions(void)
{
#if TEST_UNINIT
    WriteLog("Entering SaveOptions()\n");
#endif

    if ( g_QSearchDlg.pDockData )
    {
        if ( g_QSearchDlg.pDockData->nSide == DKS_TOP )
            g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] = 1;
        else
            g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] = 0;

        if ( !g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] )
        {
            g_Options.dockRect.left = g_QSearchDlg.pDockData->rcSize.left;
            g_Options.dockRect.right = g_QSearchDlg.pDockData->rcSize.right;
            g_Options.dockRect.top = g_QSearchDlg.pDockData->rcSize.top;
            g_Options.dockRect.bottom = g_QSearchDlg.pDockData->rcSize.bottom;
        }
    }

    if ( !equalOptions(&g_Options0, &g_Options) )
    {
        HANDLE hOptions;

#if TEST_UNINIT
        char str[120];

        WriteLog("SaveOptions(): Options are not equal\n");
        wsprintfA(str, ".bOldWindows is %s\n", g_Plugin.bOldWindows ? "TRUE" : "FALSE");
        WriteLog(str);
        wsprintfA(str, ".hMainWnd is 0x%X, IsWindow() is %s\n", g_Plugin.hMainWnd,
            IsWindow(g_Plugin.hMainWnd) ? "TRUE" : "FALSE");
        WriteLog(str);
#endif

        if ( g_Plugin.bOldWindows )
        {
            if ( hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
                   AKD_BEGINOPTIONS, POB_SAVE, (LPARAM) CSZ_QSEARCH) )
            {
                int i;

#if TEST_UNINIT
                WriteLog("SaveOptions(): Saving options...\n");
#endif

                for ( i = 0; i < OPTF_COUNT; i++ )
                {
                    writeDwordA( hOptions,
                      CSZ_OPTIONS[i], g_Options.dwFlags[i] );
                }

                if ( !g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] )
                {
                    writeBinaryA( hOptions, CSZ_OPTIONS[OPT_DOCK_RECT],
                      &g_Options.dockRect, sizeof(RECT) );
                }

                writeBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_NOTFOUND],
                  &g_Options.colorNotFound, sizeof(COLORREF) );

                writeBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_NOTREGEXP],
                  &g_Options.colorNotRegExp, sizeof(COLORREF) );

                writeBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_EOF],
                  &g_Options.colorEOF, sizeof(COLORREF) );

                writeBinaryA( hOptions, CSZ_OPTIONS[OPT_COLOR_HIGHLIGHT],
                  &g_Options.colorHighlight, sizeof(COLORREF) );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_HIGHLIGHT_MARK_ID],
                  g_Options.dwHighlightMarkID );

                if ( (g_Options.dwHighlightState & HLS_SET_ALWAYS) != HLS_SET_ALWAYS )
                {
                    writeDwordA( hOptions, CSZ_OPTIONS[OPT_HIGHLIGHT_STATE],
                      g_Options.dwHighlightState );
                }

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_USE_ALT_HOTKEYS],
                  g_Options.dwUseAltHotkeys );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_ALT_MATCHCASE],
                  g_Options.dwAltMatchCase );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_ALT_WHOLEWORD],
                  g_Options.dwAltWholeWord );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_ALT_SEARCHMODE],
                  g_Options.dwAltSearchMode );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_ALT_HIGHLIGHTALL],
                  g_Options.dwAltHighlightAll );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FIND_HISTORY_ITEMS],
                  g_Options.dwFindHistoryItems );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_HISTORY_SAVE],
                  g_Options.dwHistorySave );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_NEW_UI],
                  g_Options.dwNewUI );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_SELECT_BY_F3],
                  g_Options.dwSelectByF3 );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_SELECT_BY_FND],
                  g_Options.dwSelectByFnd );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_SELECT_BY_SELFND],
                  g_Options.dwSelectBySelFnd );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_ADJ_INCOMPL_REGEXP],
                  g_Options.dwAdjIncomplRegExp );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FINDALL_MODE],
                  g_Options.dwFindAllMode );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FINDALL_RESULT],
                  g_Options.dwFindAllResult );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FINDALL_COUNT_DELAY],
                  g_Options.dwFindAllCountDelay );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_MODE],
                  g_Options.LogOutputFRP.nMode );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_BEFORE],
                  g_Options.LogOutputFRP.nBefore );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_AFTER],
                  g_Options.LogOutputFRP.nAfter );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_LOGOUTPUT_FRP_HIGHLIGHT],
                  g_Options.LogOutputFRP.nHighlight );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_MODE],
                  g_Options.FileOutputFRP.nMode );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_BEFORE],
                  g_Options.FileOutputFRP.nBefore );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_AFTER],
                  g_Options.FileOutputFRP.nAfter );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_FILEOUTPUT_FRP_HIGHLIGHT],
                  g_Options.FileOutputFRP.nHighlight );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_EDIT_MINWIDTH],
                  g_Options.dwEditMinWidth );

                writeDwordA( hOptions, CSZ_OPTIONS[OPT_EDIT_MAXWIDTH],
                  g_Options.dwEditMaxWidth );

                // all options have been saved
                SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
            }
#if TEST_UNINIT
            else
                WriteLog("SaveOptions(): AKD_BEGINOPTIONS, POB_SAVE failed\n");
#endif
        }
        else
        {
            if ( hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
                   AKD_BEGINOPTIONS, POB_SAVE, (LPARAM) CWSZ_QSEARCH) )
            {
                int i;

#if TEST_UNINIT
                WriteLog("SaveOptions(): Saving options...\n");
#endif

                for ( i = 0; i < OPTF_COUNT; i++ )
                {
                    writeDwordW( hOptions,
                      CWSZ_OPTIONS[i], g_Options.dwFlags[i] );
                }

                if ( !g_Options.dwFlags[OPTF_DOCK_RECT_DISABLED] )
                {
                    writeBinaryW( hOptions, CWSZ_OPTIONS[OPT_DOCK_RECT],
                      &g_Options.dockRect, sizeof(RECT) );
                }

                writeBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_NOTFOUND],
                  &g_Options.colorNotFound, sizeof(COLORREF) );

                writeBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_NOTREGEXP],
                  &g_Options.colorNotRegExp, sizeof(COLORREF) );

                writeBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_EOF],
                  &g_Options.colorEOF, sizeof(COLORREF) );

                writeBinaryW( hOptions, CWSZ_OPTIONS[OPT_COLOR_HIGHLIGHT],
                  &g_Options.colorHighlight, sizeof(COLORREF) );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_HIGHLIGHT_MARK_ID],
                  g_Options.dwHighlightMarkID );

                if ( (g_Options.dwHighlightState & HLS_SET_ALWAYS) != HLS_SET_ALWAYS )
                {
                    writeDwordW( hOptions, CWSZ_OPTIONS[OPT_HIGHLIGHT_STATE],
                      g_Options.dwHighlightState );
                }

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_USE_ALT_HOTKEYS],
                  g_Options.dwUseAltHotkeys );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_ALT_MATCHCASE],
                  g_Options.dwAltMatchCase );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_ALT_WHOLEWORD],
                  g_Options.dwAltWholeWord );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_ALT_SEARCHMODE],
                  g_Options.dwAltSearchMode );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_ALT_HIGHLIGHTALL],
                  g_Options.dwAltHighlightAll );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FIND_HISTORY_ITEMS],
                  g_Options.dwFindHistoryItems );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_HISTORY_SAVE],
                  g_Options.dwHistorySave );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_NEW_UI],
                  g_Options.dwNewUI );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_SELECT_BY_F3],
                  g_Options.dwSelectByF3 );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_SELECT_BY_FND],
                  g_Options.dwSelectByFnd );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_SELECT_BY_SELFND],
                  g_Options.dwSelectBySelFnd );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_ADJ_INCOMPL_REGEXP],
                  g_Options.dwAdjIncomplRegExp );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FINDALL_MODE],
                  g_Options.dwFindAllMode );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FINDALL_RESULT],
                  g_Options.dwFindAllResult );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FINDALL_COUNT_DELAY],
                  g_Options.dwFindAllCountDelay );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_MODE],
                  g_Options.LogOutputFRP.nMode );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_BEFORE],
                  g_Options.LogOutputFRP.nBefore );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_AFTER],
                  g_Options.LogOutputFRP.nAfter );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_LOGOUTPUT_FRP_HIGHLIGHT],
                  g_Options.LogOutputFRP.nHighlight );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_MODE],
                  g_Options.FileOutputFRP.nMode );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_BEFORE],
                  g_Options.FileOutputFRP.nBefore );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_AFTER],
                  g_Options.FileOutputFRP.nAfter );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_FILEOUTPUT_FRP_HIGHLIGHT],
                  g_Options.FileOutputFRP.nHighlight );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_EDIT_MINWIDTH],
                  g_Options.dwEditMinWidth );

                writeDwordW( hOptions, CWSZ_OPTIONS[OPT_EDIT_MAXWIDTH],
                  g_Options.dwEditMaxWidth );

                // all options have been saved
                SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
            }
#if TEST_UNINIT
            else
                WriteLog("SaveOptions(): AKD_BEGINOPTIONS, POB_SAVE failed\n");
#endif
        }
        copyOptions( &g_Options0, &g_Options );
    }
#if TEST_UNINIT
    WriteLog("Exiting SaveOptions()\n");
#endif
}

void readBinaryA(HANDLE hOptions, const char* szOptionNameA,
                 void* pData, DWORD dwDataSize)
{
    PLUGINOPTIONA poA;

    poA.pOptionName = (char *) szOptionNameA;
    poA.dwType = PO_BINARY;
    poA.lpData = (BYTE *) pData;
    poA.dwData = dwDataSize;
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poA );
}

void readBinaryW(HANDLE hOptions, const wchar_t* szOptionNameW,
                 void* pData, DWORD dwDataSize)
{
    PLUGINOPTIONW poW;

    poW.pOptionName = (wchar_t *) szOptionNameW;
    poW.dwType = PO_BINARY;
    poW.lpData = (BYTE *) pData;
    poW.dwData = dwDataSize;
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poW );
}

void readStringA(HANDLE hOptions, const char* szOptionNameA,
                 char* pStr, DWORD dwStrMaxSize)
{
    PLUGINOPTIONA poA;

    poA.pOptionName = (char *) szOptionNameA;
    poA.dwType = PO_STRING;
    poA.lpData = (BYTE *) pStr;
    poA.dwData = dwStrMaxSize * sizeof(char);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poA );
}

void readStringW(HANDLE hOptions, const wchar_t* szOptionNameW,
                 wchar_t* pStr, DWORD dwStrMaxSize)
{
    PLUGINOPTIONW poW;

    poW.pOptionName = (wchar_t *) szOptionNameW;
    poW.dwType = PO_STRING;
    poW.lpData = (BYTE *) pStr;
    poW.dwData = dwStrMaxSize * sizeof(wchar_t);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poW );
}

DWORD readDwordA(HANDLE hOptions, const char* szOptionNameA, DWORD dwDefaultVal)
{
    PLUGINOPTIONA poA;

    poA.pOptionName = (char *) szOptionNameA;
    poA.dwType = PO_DWORD;
    poA.lpData = (BYTE *) &dwDefaultVal;
    poA.dwData = sizeof(DWORD);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poA );
    return dwDefaultVal;
}

DWORD readDwordW(HANDLE hOptions, const wchar_t* szOptionNameW, DWORD dwDefaultVal)
{
    PLUGINOPTIONW poW;

    poW.pOptionName = (wchar_t *) szOptionNameW;
    poW.dwType = PO_DWORD;
    poW.lpData = (BYTE *) &dwDefaultVal;
    poW.dwData = sizeof(DWORD);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poW );
    return dwDefaultVal;
}

void writeBinaryA(HANDLE hOptions, const char* szOptionNameA,
                   const void* pData, DWORD dwDataSize)
{
    PLUGINOPTIONA poA;

    poA.pOptionName = (char *) szOptionNameA;
    poA.dwType = PO_BINARY;
    poA.lpData = (BYTE *) pData;
    poA.dwData = dwDataSize;
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poA );
}

void writeBinaryW(HANDLE hOptions, const wchar_t* szOptionNameW,
                   const void* pData, DWORD dwDataSize)
{
    PLUGINOPTIONW poW;

    poW.pOptionName = (wchar_t *) szOptionNameW;
    poW.dwType = PO_BINARY;
    poW.lpData = (BYTE *) pData;
    poW.dwData = dwDataSize;
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poW );
}

void writeStringA(HANDLE hOptions, const char* szOptionNameA,
                   const char* pStr)
{
    PLUGINOPTIONA poA;

    poA.pOptionName = (char *) szOptionNameA;
    poA.dwType = PO_STRING;
    poA.lpData = (BYTE *) pStr;
    poA.dwData = (lstrlenA(pStr) + 1) * sizeof(char);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poA );
}

void writeStringW(HANDLE hOptions, const wchar_t* szOptionNameW,
                   const wchar_t* pStr)
{
    PLUGINOPTIONW poW;

    poW.pOptionName = (wchar_t *) szOptionNameW;
    poW.dwType = PO_STRING;
    poW.lpData = (BYTE *) pStr;
    poW.dwData = (lstrlenW(pStr) + 1) * sizeof(wchar_t);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poW );
}

void writeDwordA(HANDLE hOptions, const char* szOptionNameA, DWORD dwVal)
{
    PLUGINOPTIONA poA;

    poA.pOptionName = (char *) szOptionNameA;
    poA.dwType = PO_DWORD;
    poA.lpData = (BYTE *) &dwVal;
    poA.dwData = sizeof(DWORD);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poA );
}

void writeDwordW(HANDLE hOptions, const wchar_t* szOptionNameW, DWORD dwVal)
{
    PLUGINOPTIONW poW;

    poW.pOptionName = (wchar_t *) szOptionNameW;
    poW.dwType = PO_DWORD;
    poW.lpData = (BYTE *) &dwVal;
    poW.dwData = sizeof(DWORD);
    SendMessage( g_Plugin.hMainWnd, AKD_OPTION, (WPARAM) hOptions, (LPARAM) &poW );
}

//-----------------------------------------------------------------------------

const char* QSEARCH_REG_HOMEA        = "Software\\Akelsoft\\AkelPad\\Plugs\\QSearch";
const char* QSEARCH_REG_SEARCHFLAGSA = "srch_flags";
const char* QSEARCH_REG_FINDHISTORYA = "FindHistory";
const char* QSEARCH_REG_COUNTA       = "count";
const char* QSEARCH_REG_ITEMFMTA     = "%02d";

const wchar_t* QSEARCH_REG_HOMEW        = L"Software\\Akelsoft\\AkelPad\\Plugs\\QSearch";
const wchar_t* QSEARCH_REG_SEARCHFLAGSW = L"srch_flags";
const wchar_t* QSEARCH_REG_FINDHISTORYW = L"FindHistory";
const wchar_t* QSEARCH_REG_COUNTW       = L"count";
const wchar_t* QSEARCH_REG_ITEMFMTW     = L"%02d";

#define QSF_MATCHCASE 0x01
#define QSF_WHOLEWORD 0x02

void ReadFindHistoryA(void)
{
    if ( g_QSearchDlg.hDlg )
    {
        HKEY    hKey;
        HANDLE  hOptions;
        char    szKeyA[200];

        hKey = NULL;
        hOptions = NULL;

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            if ( qsearchIsSearchFlagsBeingSaved() ||
                 (qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved()) )
            {
                hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
                                      AKD_BEGINOPTIONS, POB_READ, (LPARAM) CSZ_QSEARCH);
            }
            if ( !hOptions )
                return;
        }

        if ( qsearchIsSearchFlagsBeingSaved() )
        {
            DWORD dwSearchFlags = 0;

            if ( qsearchIsSavingHistoryToStdLocation() )
            {
                dwSearchFlags = readDwordA(hOptions, QSEARCH_REG_SEARCHFLAGSA, 0);
            }
            else if ( RegOpenKeyExA(HKEY_CURRENT_USER, QSEARCH_REG_HOMEA, 0, KEY_READ, &hKey) == ERROR_SUCCESS )
            {
                DWORD dwValType = 0;
                DWORD dwValSize = sizeof(DWORD);

                if ( RegQueryValueExA(hKey, QSEARCH_REG_SEARCHFLAGSA, NULL, &dwValType, (BYTE *) &dwSearchFlags, &dwValSize) == ERROR_SUCCESS )
                {
                    if ( dwValType != REG_DWORD )
                        dwSearchFlags = 0;
                }

                RegCloseKey(hKey);
            }

            if ( dwSearchFlags & QSF_MATCHCASE )
            {
                HWND hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE);
                if ( hDlgItm )
                {
                    SendMessage(hDlgItm, BM_SETCHECK, BST_CHECKED, 0);
                    g_QSearchDlg.bMatchCase = TRUE;
                }
            }
            if ( dwSearchFlags & QSF_WHOLEWORD )
            {
                HWND hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_WHOLEWORD);
                if ( hDlgItm )
                {
                    SendMessage(hDlgItm, BM_SETCHECK, BST_CHECKED, 0);
                }
            }
        }

        if ( qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved() )
        {
            wsprintfA(szKeyA, "%s\\%s", QSEARCH_REG_HOMEA, QSEARCH_REG_FINDHISTORYA);
            if ( qsearchIsSavingHistoryToStdLocation() ||
                 (RegOpenKeyExA(HKEY_CURRENT_USER, szKeyA, 0, KEY_READ, &hKey) == ERROR_SUCCESS) )
            {
                DWORD dwCount = 0;
                DWORD dwValType = 0;
                DWORD dwValSize = sizeof(DWORD);

                if ( qsearchIsSavingHistoryToStdLocation() )
                {
                    wsprintfA(szKeyA, "%s.%s", QSEARCH_REG_FINDHISTORYA, QSEARCH_REG_COUNTA);
                    dwCount = readDwordA(hOptions, szKeyA, 0);
                }
                else if ( RegQueryValueExA(hKey, QSEARCH_REG_COUNTA, NULL, &dwValType, (BYTE *) &dwCount, &dwValSize) == ERROR_SUCCESS )
                {
                    if ( dwValType != REG_DWORD )
                        dwCount = 0;
                }

                if ( dwCount > 0 )
                {
                    DWORD   n;
                    HWND    hFindComboWnd = NULL;
                    char    szItemA[MAX_TEXT_SIZE + 1];

                    if ( dwCount > g_Options.dwFindHistoryItems )
                        dwCount = g_Options.dwFindHistoryItems;

                    SendMessageA(g_QSearchDlg.hDlg, QSM_GETHWNDCOMBO, 0, (LPARAM) &hFindComboWnd);
                    for ( n = 0; n < dwCount; n++ )
                    {
                        if ( qsearchIsSavingHistoryToStdLocation() )
                        {
                            lstrcpyA(szItemA, QSEARCH_REG_FINDHISTORYA);
                            lstrcatA(szItemA, ".");
                            lstrcatA(szItemA, QSEARCH_REG_ITEMFMTA);
                            wsprintfA(szKeyA, szItemA, n);
                            szItemA[0] = 0;
                            readStringA(hOptions, szKeyA, szItemA, MAX_TEXT_SIZE);
                            if ( szItemA[0] != 0 )
                                SendMessageA(hFindComboWnd, CB_ADDSTRING, 0, (LPARAM) szItemA);
                        }
                        else
                        {
                            dwValType = 0;
                            dwValSize = MAX_TEXT_SIZE*sizeof(char);
                            wsprintfA(szKeyA, QSEARCH_REG_ITEMFMTA, n);
                            switch ( RegQueryValueExA(hKey, szKeyA, NULL, &dwValType, (BYTE *) szItemA, &dwValSize) )
                            {
                                case ERROR_SUCCESS:
                                    // OK
                                    if ( dwValType == REG_SZ )
                                    {
                                        dwValSize /= sizeof(char);
                                        if ( dwValSize >= MAX_TEXT_SIZE )
                                            dwValSize = MAX_TEXT_SIZE - 1;
                                        szItemA[dwValSize] = 0;
                                        SendMessageA(hFindComboWnd, CB_ADDSTRING, 0, (LPARAM) szItemA);
                                    }
                                    break;
                                case ERROR_MORE_DATA:
                                    // the buffer is not large enough - skip this item
                                    break;
                                default:
                                    // some error - exit the loop
                                    n = dwCount;
                                    break;
                            }
                        }
                    }
                }

                if ( !qsearchIsSavingHistoryToStdLocation() )
                {
                    RegCloseKey(hKey);
                }
            }
        }

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
        }
    }
}

void ReadFindHistoryW(void)
{
    if ( g_QSearchDlg.hDlg )
    {
        HKEY    hKey;
        HANDLE  hOptions;
        wchar_t szKeyW[200];

        hKey = NULL;
        hOptions = NULL;

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            if ( qsearchIsSearchFlagsBeingSaved() ||
                 (qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved()) )
            {
                hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
                                      AKD_BEGINOPTIONS, POB_READ, (LPARAM) CWSZ_QSEARCH);
            }
            if ( !hOptions )
                return;
        }

        if ( qsearchIsSearchFlagsBeingSaved() )
        {
            DWORD dwSearchFlags = 0;

            if ( qsearchIsSavingHistoryToStdLocation() )
            {
                dwSearchFlags = readDwordW(hOptions, QSEARCH_REG_SEARCHFLAGSW, 0);
            }
            else if ( RegOpenKeyExW(HKEY_CURRENT_USER, QSEARCH_REG_HOMEW, 0, KEY_READ, &hKey) == ERROR_SUCCESS )
            {
                DWORD dwValType = 0;
                DWORD dwValSize = sizeof(DWORD);

                if ( RegQueryValueExW(hKey, QSEARCH_REG_SEARCHFLAGSW, NULL, &dwValType, (BYTE *) &dwSearchFlags, &dwValSize) == ERROR_SUCCESS )
                {
                    if ( dwValType != REG_DWORD )
                        dwSearchFlags = 0;
                }

                RegCloseKey(hKey);
            }

            if ( dwSearchFlags & QSF_MATCHCASE )
            {
                HWND hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE);
                if ( hDlgItm )
                {
                    SendMessage(hDlgItm, BM_SETCHECK, BST_CHECKED, 0);
                    g_QSearchDlg.bMatchCase = TRUE;
                }
            }
            if ( dwSearchFlags & QSF_WHOLEWORD )
            {
                HWND hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_WHOLEWORD);
                if ( hDlgItm )
                {
                    SendMessage(hDlgItm, BM_SETCHECK, BST_CHECKED, 0);
                }
            }
        }

        if ( qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved() )
        {
            wsprintfW(szKeyW, L"%s\\%s", QSEARCH_REG_HOMEW, QSEARCH_REG_FINDHISTORYW);
            if ( qsearchIsSavingHistoryToStdLocation() ||
                 (RegOpenKeyExW(HKEY_CURRENT_USER, szKeyW, 0, KEY_READ, &hKey) == ERROR_SUCCESS) )
            {
                DWORD dwCount = 0;
                DWORD dwValType = 0;
                DWORD dwValSize = sizeof(DWORD);

                if ( qsearchIsSavingHistoryToStdLocation() )
                {
                    wsprintfW(szKeyW, L"%s.%s", QSEARCH_REG_FINDHISTORYW, QSEARCH_REG_COUNTW);
                    dwCount = readDwordW(hOptions, szKeyW, 0);
                }
                else if ( RegQueryValueExW(hKey, QSEARCH_REG_COUNTW, NULL, &dwValType, (BYTE *) &dwCount, &dwValSize) == ERROR_SUCCESS )
                {
                    if ( dwValType != REG_DWORD )
                        dwCount = 0;
                }

                if ( dwCount > 0 )
                {
                    DWORD   n;
                    HWND    hFindComboWnd = NULL;
                    wchar_t szItemW[MAX_TEXT_SIZE + 1];

                    if ( dwCount > g_Options.dwFindHistoryItems )
                        dwCount = g_Options.dwFindHistoryItems;

                    SendMessageW(g_QSearchDlg.hDlg, QSM_GETHWNDCOMBO, 0, (LPARAM) &hFindComboWnd);
                    for ( n = 0; n < dwCount; n++ )
                    {
                        if ( qsearchIsSavingHistoryToStdLocation() )
                        {
                            lstrcpyW(szItemW, QSEARCH_REG_FINDHISTORYW);
                            lstrcatW(szItemW, L".");
                            lstrcatW(szItemW, QSEARCH_REG_ITEMFMTW);
                            wsprintfW(szKeyW, szItemW, n);
                            szItemW[0] = 0;
                            readStringW(hOptions, szKeyW, szItemW, MAX_TEXT_SIZE);
                            if ( szItemW[0] != 0 )
                                SendMessageW(hFindComboWnd, CB_ADDSTRING, 0, (LPARAM) szItemW);
                        }
                        else
                        {
                            dwValType = 0;
                            dwValSize = MAX_TEXT_SIZE*sizeof(wchar_t);
                            wsprintfW(szKeyW, QSEARCH_REG_ITEMFMTW, n);
                            switch ( RegQueryValueExW(hKey, szKeyW, NULL, &dwValType, (BYTE *) szItemW, &dwValSize) )
                            {
                                case ERROR_SUCCESS:
                                    // OK
                                    if ( dwValType == REG_SZ )
                                    {
                                        dwValSize /= sizeof(wchar_t);
                                        if ( dwValSize >= MAX_TEXT_SIZE )
                                            dwValSize = MAX_TEXT_SIZE - 1;
                                        szItemW[dwValSize] = 0;
                                        SendMessageW(hFindComboWnd, CB_ADDSTRING, 0, (LPARAM) szItemW);
                                    }
                                    break;
                                case ERROR_MORE_DATA:
                                    // the buffer is not large enough - skip this item
                                    break;
                                default:
                                    // some error - exit the loop
                                    n = dwCount;
                                    break;
                            }
                        }
                    }
                }

                if ( !qsearchIsSavingHistoryToStdLocation() )
                {
                    RegCloseKey(hKey);
                }
            }
        }

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
        }
    }
}

void SaveFindHistoryA(void)
{
    if ( g_QSearchDlg.hDlg )
    {
        HANDLE  hOptions;

        hOptions = NULL;

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            if ( qsearchIsSearchFlagsBeingSaved() ||
                 (qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved()) )
            {
                hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
                                      AKD_BEGINOPTIONS, POB_SAVE, (LPARAM) CSZ_QSEARCH);
            }
            if ( !hOptions )
                return;
        }

        if ( qsearchIsSearchFlagsBeingSaved() )
        {
            DWORD dwSearchFlags;
            HWND  hDlgItm;
            HKEY  hKey;

            dwSearchFlags = 0;
            hKey = NULL;

            hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE);
            if ( hDlgItm )
            {
                if ( SendMessage(hDlgItm, BM_GETCHECK, 0, 0) == BST_CHECKED )
                {
                    dwSearchFlags |= QSF_MATCHCASE;
                }
            }

            hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_WHOLEWORD);
            if ( hDlgItm )
            {
                if ( SendMessage(hDlgItm, BM_GETCHECK, 0, 0) == BST_CHECKED )
                {
                    dwSearchFlags |= QSF_WHOLEWORD;
                }
            }

            if ( qsearchIsSavingHistoryToStdLocation() )
            {
                writeDwordA(hOptions, QSEARCH_REG_SEARCHFLAGSA, dwSearchFlags);
            }
            else if ( RegCreateKeyExA(HKEY_CURRENT_USER, QSEARCH_REG_HOMEA, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS )
            {
                RegSetValueExA(hKey, QSEARCH_REG_SEARCHFLAGSA, 0, REG_DWORD, (const BYTE *) &dwSearchFlags, sizeof(DWORD));

                RegCloseKey(hKey);
            }
        }

        if ( qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved() )
        {
            int  nCount;
            HWND hFindComboWnd = NULL;

            SendMessageA(g_QSearchDlg.hDlg, QSM_GETHWNDCOMBO, 0, (LPARAM) &hFindComboWnd);
            nCount = (int) SendMessageA(hFindComboWnd, CB_GETCOUNT, 0, 0);
            if ( nCount > 0 )
            {
                HKEY    hKey;
                char    szKeyA[200];

                hKey = NULL;

                wsprintfA(szKeyA, "%s\\%s", QSEARCH_REG_HOMEA, QSEARCH_REG_FINDHISTORYA);
                if ( qsearchIsSavingHistoryToStdLocation() ||
                     (RegCreateKeyExA(HKEY_CURRENT_USER, szKeyA, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) )
                {
                    DWORD   dwRealCount;
                    DWORD   dwValSize;
                    int     n;
                    char    szItemA[MAX_TEXT_SIZE + 1];

                    dwRealCount = 0;
                    for ( n = 0; n < nCount; n++ )
                    {
                        if ( qsearchIsSavingHistoryToStdLocation() )
                        {
                            lstrcpyA(szItemA, QSEARCH_REG_FINDHISTORYA);
                            lstrcatA(szItemA, ".");
                            lstrcatA(szItemA, QSEARCH_REG_ITEMFMTA);
                            wsprintfA(szKeyA, szItemA, dwRealCount);
                            szItemA[0] = 0;
                            SendMessageA(hFindComboWnd, CB_GETLBTEXT, n, (LPARAM) szItemA);
                            if ( szItemA[0] )
                            {
                                writeStringA(hOptions, szKeyA, szItemA);
                                ++dwRealCount;
                            }
                        }
                        else
                        {
                            szItemA[0] = 0;
                            SendMessageA(hFindComboWnd, CB_GETLBTEXT, n, (LPARAM) szItemA);
                            if ( szItemA[0] )
                            {
                                dwValSize = (lstrlenA(szItemA) + 1)*sizeof(char);
                                wsprintfA(szKeyA, QSEARCH_REG_ITEMFMTA, dwRealCount);
                                if ( RegSetValueExA(hKey, szKeyA, 0, REG_SZ, (const BYTE *) szItemA, dwValSize) == ERROR_SUCCESS )
                                    ++dwRealCount;
                            }
                        }
                    }

                    if ( qsearchIsSavingHistoryToStdLocation() )
                    {
                        wsprintfA(szKeyA, "%s.%s", QSEARCH_REG_FINDHISTORYA, QSEARCH_REG_COUNTA);
                        writeDwordA(hOptions, szKeyA, dwRealCount);
                    }
                    else
                    {
                        RegSetValueExA(hKey, QSEARCH_REG_COUNTA, 0, REG_DWORD, (const BYTE *) &dwRealCount, sizeof(DWORD));

                        RegCloseKey(hKey);
                    }
                }
            }
        }

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
        }
    }
}

void SaveFindHistoryW(void)
{
    if ( g_QSearchDlg.hDlg )
    {
        HANDLE  hOptions;

        hOptions = NULL;

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            if ( qsearchIsSearchFlagsBeingSaved() ||
                 (qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved()) )
            {
                hOptions = (HANDLE) SendMessage(g_Plugin.hMainWnd,
                                      AKD_BEGINOPTIONS, POB_SAVE, (LPARAM) CWSZ_QSEARCH);
            }
            if ( !hOptions )
                return;
        }

        if ( qsearchIsSearchFlagsBeingSaved() )
        {
            DWORD dwSearchFlags;
            HWND  hDlgItm;
            HKEY  hKey;

            dwSearchFlags = 0;
            hKey = NULL;

            hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE);
            if ( hDlgItm )
            {
                if ( SendMessage(hDlgItm, BM_GETCHECK, 0, 0) == BST_CHECKED )
                {
                    dwSearchFlags |= QSF_MATCHCASE;
                }
            }

            hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_WHOLEWORD);
            if ( hDlgItm )
            {
                if ( SendMessage(hDlgItm, BM_GETCHECK, 0, 0) == BST_CHECKED )
                {
                    dwSearchFlags |= QSF_WHOLEWORD;
                }
            }

            if ( qsearchIsSavingHistoryToStdLocation() )
            {
                writeDwordW(hOptions, QSEARCH_REG_SEARCHFLAGSW, dwSearchFlags);
            }
            else if ( RegCreateKeyExW(HKEY_CURRENT_USER, QSEARCH_REG_HOMEW, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS )
            {
                RegSetValueExW(hKey, QSEARCH_REG_SEARCHFLAGSW, 0, REG_DWORD, (const BYTE *) &dwSearchFlags, sizeof(DWORD));

                RegCloseKey(hKey);
            }
        }

        if ( qsearchIsFindHistoryEnabled() && qsearchIsFindHistoryBeingSaved() )
        {
            int  nCount;
            HWND hFindComboWnd = NULL;

            SendMessageW(g_QSearchDlg.hDlg, QSM_GETHWNDCOMBO, 0, (LPARAM) &hFindComboWnd);
            nCount = (int) SendMessageW(hFindComboWnd, CB_GETCOUNT, 0, 0);
            if ( nCount > 0 )
            {
                HKEY    hKey;
                wchar_t szKeyW[200];

                hKey = NULL;

                wsprintfW(szKeyW, L"%s\\%s", QSEARCH_REG_HOMEW, QSEARCH_REG_FINDHISTORYW);
                if ( qsearchIsSavingHistoryToStdLocation() ||
                     (RegCreateKeyExW(HKEY_CURRENT_USER, szKeyW, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) )
                {
                    DWORD   dwRealCount;
                    DWORD   dwValSize;
                    int     n;
                    wchar_t szItemW[MAX_TEXT_SIZE + 1];

                    dwRealCount = 0;
                    for ( n = 0; n < nCount; n++ )
                    {
                        if ( qsearchIsSavingHistoryToStdLocation() )
                        {
                            lstrcpyW(szItemW, QSEARCH_REG_FINDHISTORYW);
                            lstrcatW(szItemW, L".");
                            lstrcatW(szItemW, QSEARCH_REG_ITEMFMTW);
                            wsprintfW(szKeyW, szItemW, dwRealCount);
                            szItemW[0] = 0;
                            SendMessageW(hFindComboWnd, CB_GETLBTEXT, n, (LPARAM) szItemW);
                            if ( szItemW[0] )
                            {
                                writeStringW(hOptions, szKeyW, szItemW);
                                ++dwRealCount;
                            }
                        }
                        else
                        {
                            szItemW[0] = 0;
                            SendMessageW(hFindComboWnd, CB_GETLBTEXT, n, (LPARAM) szItemW);
                            if ( szItemW[0] )
                            {
                                dwValSize = (lstrlenW(szItemW) + 1)*sizeof(wchar_t);
                                wsprintfW(szKeyW, QSEARCH_REG_ITEMFMTW, dwRealCount);
                                if ( RegSetValueExW(hKey, szKeyW, 0, REG_SZ, (const BYTE *) szItemW, dwValSize) == ERROR_SUCCESS )
                                    ++dwRealCount;
                            }
                        }
                    }

                    if ( qsearchIsSavingHistoryToStdLocation() )
                    {
                        wsprintfW(szKeyW, L"%s.%s", QSEARCH_REG_FINDHISTORYW, QSEARCH_REG_COUNTW);
                        writeDwordW(hOptions, szKeyW, dwRealCount);
                    }
                    else
                    {
                        RegSetValueExW(hKey, QSEARCH_REG_COUNTW, 0, REG_DWORD, (const BYTE *) &dwRealCount, sizeof(DWORD));

                        RegCloseKey(hKey);
                    }
                }
            }
        }

        if ( qsearchIsSavingHistoryToStdLocation() )
        {
            SendMessage(g_Plugin.hMainWnd, AKD_ENDOPTIONS, (WPARAM) hOptions, 0);
        }
    }
}

#if AKELPAD_RUNTIME_VERSION_CHECK
// from AkelPad's "Edit.c"
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2)
{
    if (LOBYTE(dwVersion1) != LOBYTE(dwVersion2))
        return LOBYTE(dwVersion1) - LOBYTE(dwVersion2);
    if (HIBYTE(dwVersion1) != HIBYTE(dwVersion2))
        return HIBYTE(dwVersion1) - HIBYTE(dwVersion2);
    if (LOBYTE(HIWORD(dwVersion1)) != LOBYTE(HIWORD(dwVersion2)))
        return LOBYTE(HIWORD(dwVersion1)) - LOBYTE(HIWORD(dwVersion2));
    if (HIBYTE(HIWORD(dwVersion1)) != HIBYTE(HIWORD(dwVersion2)))
        return HIBYTE(HIWORD(dwVersion1)) - HIBYTE(HIWORD(dwVersion2));
    return 0;
}
#endif

// debug helper
#ifdef _DEBUG
void Debug_Output(const char* szFormat, ...)
{
    char szBuf[1024];
    va_list arg;
    va_start(arg, szFormat);
    wvsprintfA(szBuf, szFormat, arg);
    va_end(arg);

    OutputDebugString(szBuf);
}
#endif

// The functions below are based on "Log.c":
//
// if (uMsg == WM_LBUTTONDBLCLK)
// {
//     AETEXTRANGEW tr;
//     AESELECTION aes;
//     AECHARINDEX ciCaret;

//     SendMessage(hWnd, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
//     if (PatOpenLine(hWnd, &oe, &ciCaret, &tr))
//     {
//         aes.crSel.ciMin=tr.cr.ciMin;
//         aes.crSel.ciMax=tr.cr.ciMax;
//         aes.dwFlags=AESELT_LOCKSCROLL;
//         aes.dwType=0;
//         SendMessage(hWnd, AEM_SETSEL, (WPARAM)&aes.crSel.ciMin, (LPARAM)&aes);
//         SetFocus(hMainWnd);
//         return TRUE;
//     }
// }
//
BOOL GetLineAtIndex(HWND hWndEdit, const AECHARINDEX* ciChar, AETEXTRANGEW* tr, tDynamicBuffer* pTextBuf)
{
    AEC_WrapLineBeginEx(ciChar, &tr->cr.ciMin);
    AEC_WrapLineEndEx(ciChar, &tr->cr.ciMax);
    tr->bColumnSel = FALSE;
    tr->pBuffer = NULL;
    tr->dwBufferMax = 0;
    tr->nNewLine = AELB_ASOUTPUT;
    tr->bFillSpaces = FALSE;

    tr->dwBufferMax = SendMessage(hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM) tr);
    if ( tr->dwBufferMax != 0 )
    {
        if ( tDynamicBuffer_Allocate(pTextBuf, sizeof(wchar_t) * tr->dwBufferMax) )
        {
            tr->pBuffer = (wchar_t *) pTextBuf->ptr;
            tr->dwBufferMax = SendMessage(hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM) tr);
            if ( tr->dwBufferMax != 0 )
            {
                pTextBuf->nBytesStored = sizeof(wchar_t) * tr->dwBufferMax;
                return TRUE;
            }
        }
    }

    return FALSE;
}
//
static unsigned int MatchRePattern(const wchar_t* wszPattern, const AETEXTRANGEW* tr, wchar_t* wszGroups[])
{
    unsigned int nGroups;
    PATEXEC pe;
    REGROUP* pReGroup;

    // Fill the structure for AKD_PATEXEC
    x_zero_mem( &pe, sizeof(PATEXEC) );
    pe.wpStr = tr->pBuffer;
    pe.wpMaxStr = tr->pBuffer + tr->dwBufferMax;
    pe.wpText = pe.wpStr;
    pe.wpMaxText = pe.wpMaxStr;
    pe.wpPat = wszPattern;
    pe.wpMaxPat = pe.wpPat + lstrlenW(pe.wpPat);
    pe.dwOptions = RESE_MATCHCASE;

    nGroups = 0;
    if ( SendMessage(g_Plugin.hMainWnd, AKD_PATEXEC, 0, (LPARAM) &pe) )
    {
        pReGroup = pe.lpREGroupStack->first;
        do
        {
            if ( pReGroup->nIndex > 0 )
            {
                lstrcpynW(wszGroups[nGroups], pReGroup->wpStrStart, (int) pReGroup->nStrLen + 1);
                wszGroups[nGroups][pReGroup->nStrLen] = 0;
                ++nGroups;
            }
            pReGroup = (REGROUP *) SendMessage( g_Plugin.hMainWnd, AKD_PATNEXTGROUP, (WPARAM) pReGroup, 0 );
        }
        while ( pReGroup );
    }
    SendMessage( g_Plugin.hMainWnd, AKD_PATFREE, 0, (LPARAM) &pe );

    return nGroups;
}
//
BOOL PatOpenLine(HWND hWndEdit)
{
    tDynamicBuffer textBuf;
    AECHARINDEX    ciCaret;
    AETEXTRANGEW   tr;
    BOOL           bResult;

    bResult = FALSE;
    tDynamicBuffer_Init( &textBuf );
    x_zero_mem( &tr, sizeof(AETEXTRANGEW) );
    x_zero_mem( &ciCaret, sizeof(AECHARINDEX) );

    SendMessage( hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM) &ciCaret );
    if ( GetLineAtIndex(hWndEdit, &ciCaret, &tr, &textBuf) )
    {
        unsigned int nGroups;
        wchar_t* wszGroups[4];
        wchar_t wszGroup1[24];
        wchar_t wszGroup2[24];
        wchar_t wszGroup3[24];
        wchar_t wszGroup4[24];

        wszGroups[0] = wszGroup1;
        wszGroups[1] = wszGroup2;
        wszGroups[2] = wszGroup3;
        wszGroups[3] = wszGroup4;

        wszGroup1[0] = 0;
        wszGroup2[0] = 0;
        wszGroup3[0] = 0;
        wszGroup4[0] = 0;

        nGroups = MatchRePattern(QS_FINDALL_REPATTERN_ALLFILES, &tr, wszGroups);
        if ( nGroups == 0 )
            nGroups = MatchRePattern(QS_FINDALL_REPATTERN_SINGLEFILE, &tr, wszGroups);

        if ( nGroups > 1 )
        {
            unsigned int i = 0;
            if ( nGroups > 2 )
            {
                // -> FRAME
                FRAMEDATA* pFrame = (FRAMEDATA *) xatoiW(wszGroups[0], NULL);
                if ( SendMessage(g_Plugin.hMainWnd, AKD_FRAMEISVALID, 0, (LPARAM) pFrame) )
                {
                    SendMessage( g_Plugin.hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM) pFrame );
                    hWndEdit = (HWND) SendMessage( g_Plugin.hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM) NULL );
                    bResult = TRUE;
                }
                ++i;
            }

            if ( hWndEdit )
            {
                int nLineCount = (int) SendMessage( hWndEdit, EM_GETLINECOUNT, 0, 0 );
                if ( nLineCount >= (int) xatoiW(wszGroups[i], NULL) )
                {
                    // -> LINE:COLUMN
                    tDynamicBuffer_Clear( &textBuf );
                    tDynamicBuffer_Append( &textBuf, wszGroups[i], sizeof(wchar_t) * lstrlenW(wszGroups[i]) ); // Line
                    tDynamicBuffer_Append( &textBuf, L":", sizeof(wchar_t) ); // ':'
                    tDynamicBuffer_Append( &textBuf, wszGroups[i+1], sizeof(wchar_t) * lstrlenW(wszGroups[i+1]) ); // Column
                    tDynamicBuffer_Append( &textBuf, L"\0", sizeof(wchar_t) ); // trailing '\0'

                    if ( SendMessage(g_Plugin.hMainWnd, AKD_GOTOW, GT_LINE, (LPARAM) textBuf.ptr) )
                        bResult = TRUE;
                }
            }
        }
    }

    tDynamicBuffer_Free( &textBuf );

    return bResult;
}

