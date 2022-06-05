//#define AEC_FUNCTIONS

#include "QSearch.h"
#include "QSearchDlg.h"
#include "QSearchSettDlg.h"
#include "QSearchLng.h"
#include "XMemStrFunc.h"


#define  QSEARCH_FIRST          0x000001
#define  QSEARCH_NEXT           0x000002
#define  QSEARCH_FINDALL        0x000010
#define  QSEARCH_FINDALLFILES   0x000020
#define  QSEARCH_SEL            0x000100
#define  QSEARCH_SEL_FINDUP     0x000200
#define  QSEARCH_NOFINDUP       0x001000
#define  QSEARCH_NOFINDBEGIN    0x002000
#define  QSEARCH_FINDUP         0x004000
#define  QSEARCH_FINDBEGIN      0x008000
#define  QSEARCH_USEDELAY       0x010000
#define  QSEARCH_NOSETSEL_FIRST 0x100000

#define  QSEARCH_EOF_DOWN    0x0001
#define  QSEARCH_EOF_UP      0x0002

#define  VK_QS_FINDBEGIN     VK_MENU     // Alt
#define  VK_QS_FINDUP        VK_SHIFT    // Shift
#define  VK_QS_PICKUPTEXT    VK_CONTROL  // Ctrl

#define  VK_QS_WW_SRCH_MODE  VK_CONTROL  // Ctrl


// extern vars
extern PluginState     g_Plugin;
extern QSearchDlgState g_QSearchDlg;
extern QSearchOpt      g_Options;
extern wchar_t         g_szFunctionQSearchW[128];
extern BOOL            g_bHighlightPlugin;
extern BOOL            g_bLogPlugin;
extern BOOL            g_bWordJustSelectedByFnd;


// helpers
static const wchar_t* getFileNameW(const wchar_t* cszFilePath)
{
    const wchar_t* p = cszFilePath;
    p += lstrlenW(cszFilePath);
    while ( p != cszFilePath )
    {
        --p;
        switch ( *p )
        {
            case L'\\':
            case L'/':
                return (p + 1);
        }
    }
    return cszFilePath;
}


// plugin call helpers
static void CallPluginFuncA(const char* cszFuncA, void* pParams)
{
    PLUGINCALLSENDA pcsA;

    pcsA.pFunction = cszFuncA;
    //pcsA.bOnStart = FALSE;
    pcsA.lParam = (LPARAM) pParams;
    pcsA.dwSupport = 0;

    SendMessageA( g_Plugin.hMainWnd, AKD_DLLCALLA, 0, (LPARAM) &pcsA );
}
static void CallPluginFuncW(const wchar_t* cszFuncW, void* pParams)
{
    PLUGINCALLSENDW pcsW;

    pcsW.pFunction = cszFuncW;
    //pcsW.bOnStart = FALSE;
    pcsW.lParam = (LPARAM) pParams;
    pcsW.dwSupport = 0;

    SendMessageW( g_Plugin.hMainWnd, AKD_DLLCALLW, 0, (LPARAM) &pcsW );
}


/* >>>>>>>>>>>>>>>>>>>>>>>> highlight plugin >>>>>>>>>>>>>>>>>>>>>>>> */
#define DLLA_HIGHLIGHT_MARK                2
#define DLLA_HIGHLIGHT_UNMARK              3
#define DLLA_HIGHLIGHT_FINDMARK            4

#define DLLA_CODER_SETALIAS         6
#define DLLA_CODER_GETALIAS         18
#define MAX_CODERALIAS              MAX_PATH

#define MARKFLAG_MATCHCASE 0x01
#define MARKFLAG_REGEXP    0x02
#define MARKFLAG_WHOLEWORD 0x04

// DLL External Call
typedef struct sDLLECHIGHLIGHT_MARK {
    UINT_PTR dwStructSize;
    INT_PTR nAction;
    unsigned char *pColorText;
    unsigned char *pColorBk;
    UINT_PTR dwMarkFlags;
    UINT_PTR dwFontStyle;
    UINT_PTR dwMarkID;
    wchar_t *wszMarkText;
} DLLECHIGHLIGHT_MARK;

typedef struct sDLLECHIGHLIGHT_UNMARK {
    UINT_PTR dwStructSize;
    INT_PTR nAction;
    UINT_PTR dwMarkID;
} DLLECHIGHLIGHT_UNMARK;

typedef struct sDLLECCODERSETTINGS_GETALIAS {
    UINT_PTR dwStructSize;
    INT_PTR nAction;
    HWND hWndEdit;
    AEHDOC hDoc;
    unsigned char* pszAlias;
} DLLECCODERSETTINGS_GETALIAS;

typedef struct sDLLECCODERSETTINGS_SETALIAS {
    UINT_PTR dwStructSize;
    INT_PTR nAction;
    const unsigned char* pszAlias;
} DLLECCODERSETTINGS_SETALIAS;

const char*    cszHighlightMainA = "Coder::HighLight";
const wchar_t* cszHighlightMainW = L"Coder::HighLight";

const char*    cszCoderSettingsA = "Coder::Settings";
const wchar_t* cszCoderSettingsW = L"Coder::Settings";

static void CallHighlightMain(void* phlParams)
{
    if ( g_Plugin.bOldWindows )
    {
        CallPluginFuncA(cszHighlightMainA, phlParams);
    }
    else
    {
        CallPluginFuncW(cszHighlightMainW, phlParams);
    }
}

static void CallCoderSettings(void* pstParams)
{
    if ( g_Plugin.bOldWindows )
    {
        CallPluginFuncA(cszCoderSettingsA, pstParams);
    }
    else
    {
        CallPluginFuncW(cszCoderSettingsW, pstParams);
    }
}

static void getCoderAliasW(wchar_t* pszAliasBufW)
{
    DLLECCODERSETTINGS_GETALIAS stParams;

    if ( pszAliasBufW )
        pszAliasBufW[0] = 0;

    stParams.dwStructSize = sizeof(DLLECCODERSETTINGS_GETALIAS);
    stParams.nAction = DLLA_CODER_GETALIAS;
    stParams.hWndEdit = NULL;
    stParams.hDoc = NULL;
    stParams.pszAlias = (unsigned char *) pszAliasBufW;

    CallCoderSettings( &stParams );
}

static void setCoderAliasW(const wchar_t* pszAliasBufW)
{
    DLLECCODERSETTINGS_SETALIAS stParams;

    stParams.dwStructSize = sizeof(DLLECCODERSETTINGS_SETALIAS);
    stParams.nAction = DLLA_CODER_SETALIAS;
    stParams.pszAlias = (const unsigned char *) pszAliasBufW;

    CallCoderSettings( &stParams );
}
/* <<<<<<<<<<<<<<<<<<<<<<<< highlight plugin <<<<<<<<<<<<<<<<<<<<<<<< */


/* >>>>>>>>>>>>>>>>>>>>>>>> log plugin >>>>>>>>>>>>>>>>>>>>>>>> */

// DLL External Call
typedef struct sDLLECLOG_OUTPUT_1 {
    UINT_PTR dwStructSize;
    INT_PTR nAction;
    LPCWSTR pszProgram;
    LPCWSTR pszWorkDir;
    LPCWSTR pszRePattern;
    LPCWSTR pszReTags;
    INT_PTR nInputCodepage;
    INT_PTR nOutputCodepage;
    UINT_PTR nFlags;
    LPCWSTR pszAlias;
} DLLECLOG_OUTPUT_1;

typedef struct sDLLECLOG_OUTPUT_2 {
    UINT_PTR dwStructSize;
    INT_PTR nAction;
    void* ptrToEditWnd;
} DLLECLOG_OUTPUT_2;

typedef struct sDLLECLOG_OUTPUT_4 {
    UINT_PTR dwStructSize;
    INT_PTR nAction;
    LPCWSTR pszText;
    INT_PTR nTextLen;
    INT_PTR nAppend;
    INT_PTR nCodepage;
    LPCWSTR pszAlias;
} DLLECLOG_OUTPUT_4;

const char*    cszLogOutputA = "Log::Output";
const wchar_t* cszLogOutputW = L"Log::Output";

static void CallLogOutput(void* ploParams)
{
    if ( g_Plugin.bOldWindows )
    {
        CallPluginFuncA(cszLogOutputA, ploParams);
    }
    else
    {
        CallPluginFuncW(cszLogOutputW, ploParams);
    }
}
/* <<<<<<<<<<<<<<<<<<<<<<<< log plugin <<<<<<<<<<<<<<<<<<<<<<<< */


/* >>>>>>>>>>>>>>>>>>>>>>>> qsearchdlg state >>>>>>>>>>>>>>>>>>>>>>>> */
    void initializeQSearchDlgState(QSearchDlgState* pQSearchDlg)
    {
        pQSearchDlg->hDlg = NULL;
        pQSearchDlg->hFindEdit = NULL;
        pQSearchDlg->hBtnFindNext = NULL;
        pQSearchDlg->hBtnFindPrev = NULL;
        pQSearchDlg->hBtnFindAll = NULL;
        pQSearchDlg->hStInfo = NULL;
        pQSearchDlg->hPopupMenu = NULL;
        pQSearchDlg->hFindAllPopupMenu = NULL;
        pQSearchDlg->bMatchCase = FALSE;
        //pQSearchDlg->bOnDlgStart = FALSE;
        pQSearchDlg->bQSearching = FALSE;
        pQSearchDlg->bIsQSearchingRightNow = FALSE;
        pQSearchDlg->bMouseJustLeavedFindEdit = FALSE;
        pQSearchDlg->pDockData = NULL;
        pQSearchDlg->pSearchResultsFrame = NULL;
        pQSearchDlg->szFindTextW[0] = 0;
        pQSearchDlg->uSearchOrigin = QS_SO_UNKNOWN;
        pQSearchDlg->uWmShowFlags = 0;
    }
/* <<<<<<<<<<<<<<<<<<<<<<<< qsearchdlg state <<<<<<<<<<<<<<<<<<<<<<<< */


// static (local) vars
static WNDPROC prev_editWndProc = NULL;
static WNDPROC prev_btnFindNextWndProc = NULL;
static WNDPROC prev_btnFindPrevWndProc = NULL;
static WNDPROC prev_btnFindAllWndProc = NULL;
static WNDPROC prev_chWholeWordWndProc = NULL;
static BOOL    qs_bEditIsActive = TRUE;
static BOOL    qs_bEditCanBeNonActive = TRUE;
static BOOL    qs_bEditSelJustChanged = FALSE;
static BOOL    qs_bEditNotFound = FALSE;
static BOOL    qs_bEditNotRegExp = FALSE;
static BOOL    qs_bEditIsEOF = FALSE;
static int     qs_nEditEOF = 0;
static BOOL    qs_bEditTextChanged = TRUE;
static BOOL    qs_bHotKeyPressedOnShow = FALSE;
static BOOL    qs_bForceFindFirst = FALSE;
static BOOL    qs_bBtnFindIsFocused = FALSE;
static DWORD   qs_dwHotKey = 0;

static UINT_PTR nFindAllTimerId = 0;
static CRITICAL_SECTION csFindAllTimerId;


static BOOL isCheckBoxChecked(HWND hDlg, int nItemId)
{
    BOOL bChecked = FALSE;
    HWND hCh = GetDlgItem(hDlg, nItemId);
    if ( hCh )
    {
        if ( SendMessage(hCh, BM_GETCHECK, 0, 0) == BST_CHECKED )
            bChecked = TRUE;
    }
    return bChecked;
}

static void qsSetInfoEmpty()
{
    if ( g_QSearchDlg.hStInfo )
    {
        if ( g_Plugin.bOldWindows )
            SetWindowTextA(g_QSearchDlg.hStInfo, "");
        else
            SetWindowTextW(g_QSearchDlg.hStInfo, L"");
    }
}

static void qsSetInfoOccurrencesFound(unsigned int nOccurrences)
{
    if ( g_QSearchDlg.hStInfo )
    {
        const wchar_t* cszTextFormat;
        int nLen;
        wchar_t szText[128];

        cszTextFormat = qsearchGetStringW(QS_STRID_FINDALL_OCCURRENCESFOUND);
        nLen = wsprintfW(szText, cszTextFormat, nOccurrences);
        if ( nLen > 0 )
        {
            --nLen;
            szText[nLen] = 0; // without the trailing '.'
        }
        SetWindowTextW(g_QSearchDlg.hStInfo, szText);
    }
}


// Helpers for find all...
// There is a single reason for so many helpers.
// The reason is: Flexibility.

// FindAllFlags
#define QS_FAF_SPECCHAR  0x0001
#define QS_FAF_REGEXP    0x0002
#define QS_FAF_MATCHCASE 0x0010
#define QS_FAF_WHOLEWORD 0x0020

typedef struct sFindAllContext {
    // input
    const wchar_t* cszFindWhat;
    const AEFINDTEXTW* pFindTextW;
    const FRAMEDATA* pFrame;
    DWORD dwFindAllMode;
    DWORD dwFindAllFlags;
    DWORD dwFindAllResult;
    unsigned int nOccurrences; // in current file
    unsigned int nTotalOccurrences; // in all files
    unsigned int nTotalFiles;
    unsigned int nFilesWithOccurrences;
    // output
    tDynamicBuffer ResultsBuf;
    tDynamicBuffer OccurrencesBuf;
} tFindAllContext;

typedef void (*tShowFindResults_Init)(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf);
typedef void (*tShowFindResults_AddOccurrence)(tFindAllContext* pFindContext, const tDynamicBuffer* pOccurrence);
typedef void (*tShowFindResults_Done)(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf);

typedef void (*tShowFindResults_AllFiles_Init)(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf);
typedef void (*tShowFindResults_AllFiles_Done)(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf);

// CountOnly...
static void qsShowFindResults_CountOnly_Init(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    // empty
}

static void qsShowFindResults_CountOnly_AddOccurrence(tFindAllContext* pFindContext, const tDynamicBuffer* pOccurrence)
{
    // empty
}

static void qsShowFindResults_CountOnly_Done(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    qsSetInfoOccurrencesFound(pFindContext->nOccurrences);
}

static void qsShowFindResults_CountOnly_AllFiles_Init(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    // empty
}

static void qsShowFindResults_CountOnly_AllFiles_Done(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    // empty
}

// LogOutput...
static void LogOutput_AddText(const wchar_t* cszText, UINT_PTR nLen)
{
    DLLECLOG_OUTPUT_4 loParams;

    loParams.dwStructSize = sizeof(DLLECLOG_OUTPUT_4);
    loParams.nAction = 4;
    loParams.pszText = cszText;
    loParams.nTextLen = nLen;
    loParams.nAppend = 2; // 2 = new line
    loParams.nCodepage = 0;
    loParams.pszAlias = NULL;
    CallLogOutput( &loParams );
}

static HWND LogOutput_GetEditHwnd()
{
    HWND hEditWnd = NULL;
    DLLECLOG_OUTPUT_2 loParams;

    loParams.dwStructSize = sizeof(DLLECLOG_OUTPUT_2);
    loParams.nAction = 2;
    loParams.ptrToEditWnd = &hEditWnd;
    CallLogOutput( &loParams );

    return hEditWnd;
}

static wchar_t getQuoteChar(const tFindAllContext* pFindContext)
{
    wchar_t chQuote = L'\"';
    if ( pFindContext->dwFindAllFlags & QS_FAF_REGEXP )
        chQuote = L'/';

    //if ( pFindContext->dwFindAllFlags & QS_FAF_MATCHCASE )
    //    ...

    //if ( pFindContext->dwFindAllFlags & QS_FAF_WHOLEWORD )
    //    ...

    return chQuote;
}

static UINT_PTR formatSearchingForStringToBuf(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    wchar_t* pszText;
    const wchar_t* cszTextFormat;
    const wchar_t* cszFileName;
    wchar_t chQuote;
    UINT_PTR nBytesToAllocate;
    UINT_PTR nLen;

    chQuote = getQuoteChar(pFindContext);

    cszTextFormat = qsearchGetStringW(QS_STRID_FINDALL_SEARCHINGFOR);

    cszFileName = (const wchar_t *) pFindContext->pFrame->ei.pFile;
    if ( cszFileName )
        cszFileName = getFileNameW(cszFileName);
    else
        cszFileName = L"";

    nBytesToAllocate = lstrlenW(cszTextFormat);
    nBytesToAllocate += lstrlenW(pFindContext->cszFindWhat);
    nBytesToAllocate += lstrlenW(cszFileName);
    nBytesToAllocate += 1; // 12;
    nBytesToAllocate *= sizeof(wchar_t);

    if ( !tDynamicBuffer_Allocate(pTempBuf, nBytesToAllocate) )
        return 0; // failed to allocate the memory

    pszText = (wchar_t *) pTempBuf->ptr;
    nLen = (UINT_PTR) wsprintfW(pszText, cszTextFormat, chQuote, pFindContext->cszFindWhat, chQuote, cszFileName, pFindContext->nOccurrences );
    pTempBuf->nBytesStored = nLen*sizeof(wchar_t);
    return nLen;
}

static UINT_PTR formatAllFilesSearchingForStringToBuf(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    wchar_t* pszText;
    const wchar_t* cszTextFormat;
    wchar_t chQuote;
    UINT_PTR nBytesToAllocate;
    UINT_PTR nLen;

    chQuote = getQuoteChar(pFindContext);

    cszTextFormat = qsearchGetStringW(QS_STRID_FINDALL_SEARCHINGFORINFILES);

    nBytesToAllocate = lstrlenW(cszTextFormat);
    nBytesToAllocate += lstrlenW(pFindContext->cszFindWhat);
    nBytesToAllocate += 10;
    nBytesToAllocate *= sizeof(wchar_t);

    if ( !tDynamicBuffer_Allocate(pTempBuf, nBytesToAllocate) )
        return 0; // failed to allocate the memory

    pszText = (wchar_t *) pTempBuf->ptr;
    nLen = (UINT_PTR) wsprintfW(pszText, cszTextFormat, chQuote, pFindContext->cszFindWhat, chQuote, pFindContext->nTotalFiles);
    pTempBuf->nBytesStored = nLen*sizeof(wchar_t);
    return nLen;
}

static void initLogOutput(DWORD dwFindAllResult)
{
    DLLECLOG_OUTPUT_1 loParams;
    wchar_t szCoderAlias[MAX_CODERALIAS + 1];

    if ( g_bHighlightPlugin )
    {
        if ( dwFindAllResult & QS_FINDALL_RSLT_CODERALIAS )
            getCoderAliasW(szCoderAlias);
        else
            lstrcpyW(szCoderAlias, L".qsfndall_tolog");
    }
    else
    {
        szCoderAlias[0] = 0;
    }

    loParams.dwStructSize = sizeof(DLLECLOG_OUTPUT_1);
    loParams.nAction = 1;
    loParams.pszProgram = NULL;
    loParams.pszWorkDir = NULL;
    if ( dwFindAllResult & QS_FINDALL_RSLT_ALLFILES )
    {
        loParams.pszRePattern = QS_FINDALL_REPATTERN_ALLFILES; // corresponds to the output string format
        loParams.pszReTags = QS_FINDALL_RETAGS_ALLFILES;
    }
    else
    {
        loParams.pszRePattern = QS_FINDALL_REPATTERN_SINGLEFILE; // corresponds to the output string format
        loParams.pszReTags = QS_FINDALL_RETAGS_SINGLEFILE;
    }
    loParams.nInputCodepage = -2;
    loParams.nOutputCodepage = -2;
    loParams.nFlags = 2; // 2 = no input line
    loParams.pszAlias = szCoderAlias[0] ? szCoderAlias : NULL;

    CallLogOutput( &loParams );
}

// funcs
enum eHighlightCondition {
    QHC_CHECKBOX_CHECKED = 0,
    QHC_ALWAYS
};
void qsearchDoTryHighlightAll(HWND hDlg, const DWORD dwOptFlags[], DWORD dwHighlightCondition);
void qsearchDoTryUnhighlightAll(void);

static void scrollEditToPosition(HWND hWndEdit, INT_PTR nPos, DWORD dwFrpHighlight, const tFindAllContext* pFindContext)
{
    int nFirstVisibleLine;
    AECHARINDEX ci;

    SendMessage( hWndEdit, AEM_RICHOFFSETTOINDEX, (WPARAM) nPos, (LPARAM) &ci );
    SendMessageW( hWndEdit, AEM_EXSETSEL, (WPARAM) &ci, (LPARAM) &ci );
    nFirstVisibleLine = (int) SendMessageW( hWndEdit, AEM_GETLINENUMBER, AEGL_FIRSTFULLVISIBLELINE, 0 );
    if ( ci.nLine > nFirstVisibleLine )
        SendMessageW( hWndEdit, AEM_LINESCROLL, AESB_VERT | AESB_ALIGNTOP, (LPARAM) (ci.nLine - nFirstVisibleLine) );

    if ( dwFrpHighlight != QSFRH_NONE )
    {
        BOOL bContinue = TRUE;
        AEFINDTEXTW aeftW;

        if ( ((pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0) &&
             ((pFindContext->dwFindAllResult & QS_FINDALL_RSLT_SEARCHING) != 0) )
        {
            wchar_t szQuoteTextW[2];

            szQuoteTextW[0] = getQuoteChar(pFindContext);
            szQuoteTextW[1] = 0;

            aeftW.dwFlags = pFindContext->pFindTextW->dwFlags;
            aeftW.pText = szQuoteTextW;
            aeftW.dwTextLen = 1;
            aeftW.nNewLine = pFindContext->pFindTextW->nNewLine;
            x_mem_cpy( &aeftW.crSearch.ciMin, &ci, sizeof(AECHARINDEX) );
            SendMessageW( hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM) &aeftW.crSearch.ciMax);
            if ( SendMessageW(hWndEdit, AEM_FINDTEXTW, 0, (LPARAM) &aeftW) )
                x_mem_cpy( &aeftW.crSearch.ciMin, &aeftW.crFound.ciMax, sizeof(AECHARINDEX) );
            else
                bContinue = FALSE;
        }
        else
        {
            x_mem_cpy( &aeftW.crSearch.ciMin, &ci, sizeof(AECHARINDEX) );
            SendMessageW( hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM) &aeftW.crSearch.ciMax);
        }

        if ( bContinue )
        {
            aeftW.dwFlags = pFindContext->pFindTextW->dwFlags;
            aeftW.pText = pFindContext->pFindTextW->pText;
            aeftW.dwTextLen = pFindContext->pFindTextW->dwTextLen;
            aeftW.nNewLine = pFindContext->pFindTextW->nNewLine;
            SendMessageW( hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM) &aeftW.crSearch.ciMax);

            if ( SendMessageW(hWndEdit, AEM_FINDTEXTW, 0, (LPARAM) &aeftW) )
            {
                // Select the text
                SendMessageW( hWndEdit, AEM_EXSETSEL, (WPARAM) &aeftW.crFound.ciMin, (LPARAM) &aeftW.crFound.ciMax );

                if ( (dwFrpHighlight == QSFRH_IFCHECKED) ||
                     (dwFrpHighlight == QSFRH_ALWAYS) )
                {
                    // Highlight All
                    if ( (dwFrpHighlight == QSFRH_ALWAYS) ||
                         isCheckBoxChecked(g_QSearchDlg.hDlg, IDC_CH_HIGHLIGHTALL) )
                    {
                        // Highlighting doesn't work without this:
                        SendMessageW( hWndEdit, WM_PAINT, 0, 0 );

                        // Actual highlighting:
                        qsearchDoTryHighlightAll(g_QSearchDlg.hDlg, g_Options.dwFlags, QHC_ALWAYS);
                    }
                }
            }
        }
    }
}

static void qsShowFindResults_LogOutput_Init(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_ALLFILES) == 0 )
    {
        HWND hWndEdit;
        INT_PTR nStartPos;

        initLogOutput(pFindContext->dwFindAllResult);

        hWndEdit = LogOutput_GetEditHwnd();
        nStartPos = SendMessageW(g_Plugin.hMainWnd, AKD_GETTEXTLENGTH, (WPARAM) hWndEdit, 0);

        if ( nStartPos != 0 )
            tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line
    }
}

static void qsShowFindResults_LogOutput_AddOccurrence(tFindAllContext* pFindContext, const tDynamicBuffer* pOccurrence)
{
    tDynamicBuffer_Append( &pFindContext->OccurrencesBuf, pOccurrence->ptr, pOccurrence->nBytesStored );
    tDynamicBuffer_Append( &pFindContext->OccurrencesBuf, L"\r", 1*sizeof(wchar_t) ); // new line
}

static void qsShowFindResults_LogOutput_Done(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    if ( (pFindContext->nOccurrences != 0) ||
         ((pFindContext->dwFindAllResult & QS_FINDALL_RSLT_ALLFILES) == 0) )
    {
        if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0 )
        {
            if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_SEARCHING )
            {
                if ( formatSearchingForStringToBuf(pFindContext, pTempBuf) )
                {
                    tDynamicBuffer_Append( &pFindContext->ResultsBuf, pTempBuf->ptr, pTempBuf->nBytesStored );
                    tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line
                }
            }
        }

        tDynamicBuffer_Append( &pFindContext->ResultsBuf, pFindContext->OccurrencesBuf.ptr, pFindContext->OccurrencesBuf.nBytesStored );
    }

    tDynamicBuffer_Clear( &pFindContext->OccurrencesBuf );

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_ALLFILES) == 0 )
    {
        HWND hWndEdit;
        INT_PTR nStartPos;
        UINT_PTR nLen;

        hWndEdit = LogOutput_GetEditHwnd();
        nStartPos = SendMessageW(g_Plugin.hMainWnd, AKD_GETTEXTLENGTH, (WPARAM) hWndEdit, 0);
        if ( nStartPos != 0 )
            ++nStartPos; // new line was added in LogOutput_Init

        nLen = pFindContext->ResultsBuf.nBytesStored/sizeof(wchar_t); // without the trailing '\0'
        tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\0", 1*sizeof(wchar_t) ); // the trailing '\0'
        LogOutput_AddText( (const wchar_t*) pFindContext->ResultsBuf.ptr, nLen );

        scrollEditToPosition(hWndEdit, nStartPos, g_Options.LogOutputFRP.nHighlight, pFindContext);
    }

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_ALLFILES) == 0 )
    {
        qsSetInfoOccurrencesFound(pFindContext->nOccurrences);
    }
}

static void qsShowFindResults_LogOutput_AllFiles_Init(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    HWND hWndEdit;
    INT_PTR nStartPos;

    initLogOutput(pFindContext->dwFindAllResult);

    hWndEdit = LogOutput_GetEditHwnd();
    nStartPos = SendMessageW(g_Plugin.hMainWnd, AKD_GETTEXTLENGTH, (WPARAM) hWndEdit, 0);

    if ( nStartPos != 0 )
        tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0 )
    {
        if ( formatAllFilesSearchingForStringToBuf(pFindContext, pTempBuf) )
        {
            tDynamicBuffer_Append( &pFindContext->ResultsBuf, pTempBuf->ptr, pTempBuf->nBytesStored );
            tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line
        }
    }
}

static void qsShowFindResults_LogOutput_AllFiles_Done(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    HWND hWndEdit;
    INT_PTR nStartPos;
    const wchar_t* cszTextFormat;
    UINT_PTR nLen;
    wchar_t szText[128];

    hWndEdit = LogOutput_GetEditHwnd();
    nStartPos = SendMessageW(g_Plugin.hMainWnd, AKD_GETTEXTLENGTH, (WPARAM) hWndEdit, 0);
    if ( nStartPos != 0 )
        ++nStartPos; // new line was added in LogOutput_AllFiles_Init

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0 )
    {
        cszTextFormat = qsearchGetStringW(QS_STRID_FINDALL_OCCURRENCESFOUNDINFILES);
        nLen = (UINT_PTR) wsprintfW(szText, cszTextFormat, pFindContext->nTotalOccurrences, pFindContext->nFilesWithOccurrences);
        tDynamicBuffer_Append( &pFindContext->ResultsBuf, szText, nLen*sizeof(wchar_t) );
        tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line
    }

    nLen = pFindContext->ResultsBuf.nBytesStored/sizeof(wchar_t); // without the trailing '\0'
    tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\0", 1*sizeof(wchar_t) ); // the trailing '\0'
    LogOutput_AddText( (const wchar_t*) pFindContext->ResultsBuf.ptr, nLen );

    scrollEditToPosition(hWndEdit, nStartPos, g_Options.LogOutputFRP.nHighlight, pFindContext);
}

// FileOutput...
static void qsShowFindResults_FileOutput_Init(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
}

static void qsShowFindResults_FileOutput_AddOccurrence(tFindAllContext* pFindContext, const tDynamicBuffer* pOccurrence)
{
    tDynamicBuffer_Append( &pFindContext->OccurrencesBuf, pOccurrence->ptr, pOccurrence->nBytesStored );
    tDynamicBuffer_Append( &pFindContext->OccurrencesBuf, L"\r", 1*sizeof(wchar_t) ); // new line
}

static void addResultsToFileOutput(tFindAllContext* pFindContext)
{
    BOOL bOutputResult;
    wchar_t szCoderAlias[MAX_CODERALIAS + 1];

    if ( g_bHighlightPlugin )
    {
        if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_CODERALIAS )
            getCoderAliasW(szCoderAlias);
        else
            lstrcpyW(szCoderAlias, L".qsfndall_tofile");
    }
    else
    {
        szCoderAlias[0] = 0;
    }

    bOutputResult = FALSE;

    if ( ((pFindContext->dwFindAllMode & QS_FINDALL_MASK) == QS_FINDALL_FILEOUTPUT_SNGL) &&
         (g_Plugin.nMDI == WMD_SDI) )
    {
        bOutputResult = TRUE;
    }
    else if ( ((pFindContext->dwFindAllMode & QS_FINDALL_MASK) == QS_FINDALL_FILEOUTPUT_SNGL) &&
              (g_QSearchDlg.pSearchResultsFrame != NULL) &&
              SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEISVALID, 0, (LPARAM) g_QSearchDlg.pSearchResultsFrame) )
    {
        SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM) g_QSearchDlg.pSearchResultsFrame);
        bOutputResult = TRUE;
    }
    else if ( SendMessageW(g_Plugin.hMainWnd, WM_COMMAND, IDM_FILE_NEW, 0) == TRUE )
    {
        g_QSearchDlg.pSearchResultsFrame = NULL;
        bOutputResult = TRUE;
    }

    if ( bOutputResult )
    {
        EDITINFO  ei;

        if ( g_QSearchDlg.pSearchResultsFrame == NULL )
        {
            g_QSearchDlg.pSearchResultsFrame = (FRAMEDATA *) SendMessageW( g_Plugin.hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0 );
        }

        ei.hWndEdit = NULL;
        SendMessageW( g_Plugin.hMainWnd, AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei );
        if ( ei.hWndEdit )
        {
            INT_PTR nStartPos;
            AEAPPENDTEXTW aeatW;

            if ( szCoderAlias[0] )
            {
                setCoderAliasW(szCoderAlias);
            }

            nStartPos = SendMessageW(g_Plugin.hMainWnd, AKD_GETTEXTLENGTH, (WPARAM) ei.hWndEdit, 0);

            if ( ((pFindContext->dwFindAllMode & QS_FINDALL_MASK) == QS_FINDALL_FILEOUTPUT_SNGL) )
            {
                if ( nStartPos != 0 )
                {
                    aeatW.pText = L"\r"; // new line
                    aeatW.dwTextLen = (UINT_PTR) (-1);
                    aeatW.nNewLine = AELB_ASINPUT;
                    SendMessageW( ei.hWndEdit, AEM_APPENDTEXTW, 0, (LPARAM) &aeatW );
                    nStartPos += 1; // the new line above
                }
            }

            /* SendMessageW( ei.hWndEdit, EM_REPLACESEL, FALSE, (LPARAM) pFindContext->ResultsBuf.ptr ); */
            aeatW.pText = (const wchar_t *) pFindContext->ResultsBuf.ptr;
            aeatW.dwTextLen = pFindContext->ResultsBuf.nBytesStored/sizeof(wchar_t) - 1; // excluding the trailing '\0'
            aeatW.nNewLine = AELB_ASINPUT;
            SendMessageW( ei.hWndEdit, AEM_APPENDTEXTW, 0, (LPARAM) &aeatW );

            scrollEditToPosition(ei.hWndEdit, nStartPos, g_Options.FileOutputFRP.nHighlight, pFindContext);
        }
    }
}

static void qsShowFindResults_FileOutput_Done(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    if ( (pFindContext->nOccurrences != 0) ||
         ((pFindContext->dwFindAllResult & QS_FINDALL_RSLT_ALLFILES) == 0) )
    {
        if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0 )
        {
            if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_SEARCHING )
            {
                if ( formatSearchingForStringToBuf(pFindContext, pTempBuf) )
                {
                    tDynamicBuffer_Append( &pFindContext->ResultsBuf, pTempBuf->ptr, pTempBuf->nBytesStored );
                    tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line
                }
            }
        }

        tDynamicBuffer_Append( &pFindContext->ResultsBuf, pFindContext->OccurrencesBuf.ptr, pFindContext->OccurrencesBuf.nBytesStored );
    }

    tDynamicBuffer_Clear( &pFindContext->OccurrencesBuf );

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_ALLFILES) == 0 )
    {
        tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\0", 1*sizeof(wchar_t) ); // the trailing '\0'
        addResultsToFileOutput( pFindContext );

        qsSetInfoOccurrencesFound(pFindContext->nOccurrences);
    }
}

static void qsShowFindResults_FileOutput_AllFiles_Init(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0 )
    {
        if ( formatAllFilesSearchingForStringToBuf(pFindContext, pTempBuf) )
        {
            tDynamicBuffer_Append( &pFindContext->ResultsBuf, pTempBuf->ptr, pTempBuf->nBytesStored );
            tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line
        }
    }
}

static void qsShowFindResults_FileOutput_AllFiles_Done(tFindAllContext* pFindContext, tDynamicBuffer* pTempBuf)
{
    const wchar_t* cszTextFormat;
    UINT_PTR nLen;
    wchar_t  szText[128];

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0 )
    {
        cszTextFormat = qsearchGetStringW(QS_STRID_FINDALL_OCCURRENCESFOUNDINFILES);
        nLen = (UINT_PTR) wsprintfW(szText, cszTextFormat, pFindContext->nTotalOccurrences, pFindContext->nFilesWithOccurrences);
        tDynamicBuffer_Append( &pFindContext->ResultsBuf, szText, nLen*sizeof(wchar_t) );
        tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\r", 1*sizeof(wchar_t) ); // new line
    }

    tDynamicBuffer_Append( &pFindContext->ResultsBuf, L"\0", 1*sizeof(wchar_t) ); // the trailing '\0'
    addResultsToFileOutput( pFindContext );
}

typedef struct sShowFindResults {
    tShowFindResults_Init pfnInit;                   // can't be NULL
    tShowFindResults_AddOccurrence pfnAddOccurrence; // can't be NULL
    tShowFindResults_Done pfnDone;                   // can't be NULL
    tShowFindResults_AllFiles_Init pfnAllFilesInit;  // can't be NULL
    tShowFindResults_AllFiles_Done pfnAllFilesDone;  // can't be NULL
} tShowFindResults;

typedef void (*tStoreResultCallback)(tFindAllContext* pFindContext, const AECHARRANGE* pcrFound, const int nLinesBeforeAfter[2], const tDynamicBuffer* pFindResult, tDynamicBuffer* pTempBuf, tShowFindResults_AddOccurrence pfnAddOccurrence);

static void formatFindResult(const tFindAllContext* pFindContext, const AECHARRANGE* pcrFound, const int nLinesBeforeAfter[2], const tDynamicBuffer* pFindResult, tDynamicBuffer* pTempBuf)
{
    pTempBuf->nBytesStored = 0;

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) == 0 )
    {
        HWND hWndEdit;
        BOOL bAddFramePtr;
        int nUnwrappedLine;
        int nPrefixLen;
        wchar_t szPrefixBuf[128];

        hWndEdit = pFindContext->pFrame->ei.hWndEdit;
        bAddFramePtr = FALSE;
        nUnwrappedLine = 0;
        nPrefixLen = 0;
        szPrefixBuf[0] = 0;

        if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_SEARCHING )
        {
            szPrefixBuf[nPrefixLen++] = L' ';
        }

        if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_POS )
        {
            INT_X nLinePos;
            AECHARINDEX ci;

            szPrefixBuf[nPrefixLen++] = L'(';
            if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_ALLFILES )
            {
                bAddFramePtr = TRUE;
            }
            else if ( g_Plugin.nMDI != WMD_SDI )
            {
                DWORD dwFindAllMode = (pFindContext->dwFindAllMode & QS_FINDALL_MASK);
                if ( (dwFindAllMode == QS_FINDALL_FILEOUTPUT_SNGL) ||
                     (dwFindAllMode == QS_FINDALL_FILEOUTPUT_MULT) )
                {
                    bAddFramePtr = TRUE;
                }
            }

            if ( bAddFramePtr )
            {
                nPrefixLen += xitoaW( (INT_PTR) pFindContext->pFrame, &szPrefixBuf[nPrefixLen] );
                szPrefixBuf[nPrefixLen++] = L' ';
            }

            if ( SendMessage(hWndEdit, AEM_GETWORDWRAP, 0, 0) != AEWW_NONE )
                nUnwrappedLine = (int) SendMessage(hWndEdit, AEM_GETUNWRAPLINE, pcrFound->ciMin.nLine, 0);
            else
                nUnwrappedLine = pcrFound->ciMin.nLine;

            x_mem_cpy(&ci, &pcrFound->ciMin, sizeof(AECHARINDEX));
            nLinePos = AEC_WrapLineBegin(&ci);

            nPrefixLen += xitoaW( nUnwrappedLine + 1, &szPrefixBuf[nPrefixLen] );
            szPrefixBuf[nPrefixLen++] = L':';
            nPrefixLen += xitoaW( nLinePos + 1, &szPrefixBuf[nPrefixLen] );
            szPrefixBuf[nPrefixLen++] = L')';
            szPrefixBuf[nPrefixLen++] = L'\t';
        }

        if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_LEN )
        {
            int nLen;
            AECHARINDEX ciBegin;
            AECHARINDEX ciEnd;
            AEINDEXSUBTRACT aeis;

            x_mem_cpy( &ciBegin, &pcrFound->ciMin, sizeof(AECHARINDEX) );
            x_mem_cpy( &ciEnd, &pcrFound->ciMax, sizeof(AECHARINDEX) );
            aeis.ciChar1 = &ciEnd;
            aeis.ciChar2 = &ciBegin;
            aeis.bColumnSel = FALSE;
            aeis.nNewLine = AELB_ASIS;
            nLen = (int) SendMessage( hWndEdit, AEM_INDEXSUBTRACT, 0, (LPARAM) &aeis );

            if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_POS )
            {
                nPrefixLen -= 1; // exclude the trailing '\t'
            }

            szPrefixBuf[nPrefixLen++] = L'(';
            nPrefixLen += xitoaW( nLen, &szPrefixBuf[nPrefixLen] );
            szPrefixBuf[nPrefixLen++] = L')';
            szPrefixBuf[nPrefixLen++] = L'\t';
        }

        szPrefixBuf[nPrefixLen] = 0; // the trailing '\0'

        if ( (nPrefixLen != 0) && (nLinesBeforeAfter[0] != 0 || nLinesBeforeAfter[1] != 0) )
        {
            int n1;
            int n2;
            int n;
            int i;
            wchar_t szPrefixBuf2[128];

            tDynamicBuffer_Allocate( pTempBuf, sizeof(wchar_t)*(nPrefixLen + 1)*(1 + nLinesBeforeAfter[0] + nLinesBeforeAfter[1]) + pFindResult->nBytesStored );

            n = -nLinesBeforeAfter[0];
            n1 = 0;
            n2 = x_wstr_findch( (const wchar_t *) pFindResult->ptr, L'\r', 0 );
            for ( ; ; )
            {
                if ( n == 0 )
                {
                    tDynamicBuffer_Append(pTempBuf, szPrefixBuf, sizeof(wchar_t)*nPrefixLen);
                }
                else
                {
                    i = -1;
                    lstrcpyW(szPrefixBuf2, szPrefixBuf);
                    if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_POS )
                    {
                        i = x_wstr_findch(szPrefixBuf2, L'(', 0);
                        if ( i != -1 )
                        {
                            // szPrefixBuf2[i] = (n < 0) ? L'\x2193' : L'\x2191';
                            if ( bAddFramePtr )
                            {
                                // i = x_wstr_findch(szPrefixBuf2, L' ', i + 1);
                                ++i;
                                for ( ; szPrefixBuf2[i] != L' '; ++i )
                                {
                                    szPrefixBuf2[i] = L' ';
                                }
                            }
                        }
                        if ( i != -1 )
                        {
                            ++i;
                            i += xitoaW(nUnwrappedLine + n + 1, &szPrefixBuf2[i]);
                            for ( ; szPrefixBuf2[i] != L')'; ++i )
                            {
                                szPrefixBuf2[i] = L' ';
                            }
                            // szPrefixBuf2[i] = (n < 0) ? L'\x2193' : L'\x2191';
                        }
                    }
                    if ( pFindContext->dwFindAllResult & QS_FINDALL_RSLT_LEN )
                    {
                        i = x_wstr_findch(szPrefixBuf2, L'(', (i == -1) ? 0 : i);
                        if ( i != -1 )
                        {
                            for ( ; szPrefixBuf2[i] != L'\t'; ++i )
                            {
                                szPrefixBuf2[i] = L' ';
                            }
                        }
                    }
                    tDynamicBuffer_Append(pTempBuf, szPrefixBuf2, sizeof(wchar_t)*nPrefixLen);
                }
                if ( n2 != -1 )
                {
                    tDynamicBuffer_Append( pTempBuf, ((const wchar_t *) pFindResult->ptr) + n1, sizeof(wchar_t)*(n2 - n1) );
                    tDynamicBuffer_Append( pTempBuf, L"\r", sizeof(wchar_t) );
                    if ( ++n <= nLinesBeforeAfter[1] )
                    {
                        n1 = n2 + 1;
                        n2 = x_wstr_findch( (const wchar_t *) pFindResult->ptr, L'\r', n1 );
                    }
                    else
                        break;
                }
                else
                {
                    tDynamicBuffer_Append( pTempBuf, ((const wchar_t *) pFindResult->ptr) + n1, pFindResult->nBytesStored - sizeof(wchar_t)*n1 );
                    break;
                }
            }
        }
        else
        {
            tDynamicBuffer_Allocate(pTempBuf, sizeof(wchar_t)*nPrefixLen + pFindResult->nBytesStored);
            if ( nPrefixLen != 0 )
            {
                tDynamicBuffer_Append(pTempBuf, szPrefixBuf, sizeof(wchar_t)*nPrefixLen);
            }
            tDynamicBuffer_Append(pTempBuf, pFindResult->ptr, pFindResult->nBytesStored);
        }
    }
    else
    {
        tDynamicBuffer_Append(pTempBuf, pFindResult->ptr, pFindResult->nBytesStored);
    }
}

static void qsStoreResultCallback(tFindAllContext* pFindContext, const AECHARRANGE* pcrFound, const int nLinesBeforeAfter[2], const tDynamicBuffer* pFindResult, tDynamicBuffer* pTempBuf, tShowFindResults_AddOccurrence pfnAddOccurrence)
{
    formatFindResult(pFindContext, pcrFound, nLinesBeforeAfter, pFindResult, pTempBuf);
    pfnAddOccurrence(pFindContext, pTempBuf);
}


typedef struct sGetFindResultPolicy {
    int nMode;       // one of QSFRM_*
    int nBefore;     // number of lines or chars before the search result
    int nAfter;      // number of lines or chars after the search result
    int nMaxLineLen; // max line length to show (0 - no limit)
    tStoreResultCallback pfnStoreResultCallback;
} tGetFindResultPolicy;
// nBefore=0 and nAfter=0 with nMode=QSFRM_LINE means: whole line
// nBefore=0 and nAfter=0 with nMode=QSFRM_CHAR means: just the matching word

typedef void (*tFindResultCallback)(tFindAllContext* pFindContext, const AECHARRANGE* pcrFound, const tGetFindResultPolicy* pfrPolicy, tDynamicBuffer* pTempBuf, tDynamicBuffer* pTempBuf2, tShowFindResults_AddOccurrence pfnAddOccurrence);

static void qsFindResultCallback(tFindAllContext* pFindContext, const AECHARRANGE* pcrFound, const tGetFindResultPolicy* pfrPolicy, tDynamicBuffer* pTempBuf, tDynamicBuffer* pTempBuf2, tShowFindResults_AddOccurrence pfnAddOccurrence)
{
    AETEXTRANGEW tr;
    HWND         hWndEdit;
    UINT_PTR     nBytesToAllocate;
    int          nLinesBeforeAfter[2];

    if ( !pfrPolicy->pfnStoreResultCallback )
        return; // no sense to retrieve the find result

    hWndEdit = pFindContext->pFrame->ei.hWndEdit;

    x_zero_mem( &tr, sizeof(AETEXTRANGEW) );
    x_mem_cpy( &tr.cr, pcrFound, sizeof(AECHARRANGE) );
    nLinesBeforeAfter[0] = 0;
    nLinesBeforeAfter[1] = 0;

    if ( (pFindContext->dwFindAllResult & QS_FINDALL_RSLT_MATCHONLY) == 0 )
    {
        // TODO: check pfrPolicy->nMaxLineLen
        if ( pfrPolicy->nMode == QSFRM_CHAR )
        {
            AEINDEXOFFSET io;

            if ( pfrPolicy->nBefore > 0 )
            {
                io.ciCharIn = (AECHARINDEX *) &pcrFound->ciMin;
                io.ciCharOut = &tr.cr.ciMin;
                io.nOffset = -pfrPolicy->nBefore;
                io.nNewLine = AELB_R;
                SendMessage( hWndEdit, AEM_INDEXOFFSET, 0, (LPARAM) &io );
            }
            if ( pfrPolicy->nAfter > 0)
            {
                io.ciCharIn = (AECHARINDEX *) &pcrFound->ciMax;
                io.ciCharOut = &tr.cr.ciMax;
                io.nOffset = pfrPolicy->nAfter;
                io.nNewLine = AELB_R;
                SendMessage( hWndEdit, AEM_INDEXOFFSET, 0, (LPARAM) &io );
            }
        }
        else if ( pfrPolicy->nMode == QSFRM_CHARINLINE )
        {
            int nBefore;
            int nAfter;
            AECHARINDEX ci;

            nBefore = pfrPolicy->nBefore;
            x_mem_cpy(&ci, &tr.cr.ciMin, sizeof(AECHARINDEX));
            while ( nBefore != 0 && AEC_PrevCharInLine(&ci) != NULL )
            {
                x_mem_cpy(&tr.cr.ciMin, &ci, sizeof(AECHARINDEX));
                --nBefore;
            }

            nAfter = pfrPolicy->nAfter;
            x_mem_cpy(&ci, &tr.cr.ciMax, sizeof(AECHARINDEX));
            while ( nAfter != 0 && AEC_NextCharInLine(&ci) != NULL )
            {
                x_mem_cpy(&tr.cr.ciMax, &ci, sizeof(AECHARINDEX));
                --nAfter;
            }
            if ( nAfter != 0 )
            {
                tr.cr.ciMax.nCharInLine = tr.cr.ciMax.lpLine->nLineLen;
            }
        }
        else // QSFRM_LINE or QSFRM_LINE_CR
        {
            if ( pfrPolicy->nBefore == 0 && pfrPolicy->nAfter == 0 )
            {
                AEC_WrapLineBegin(&tr.cr.ciMin);
                AEC_WrapLineEnd(&tr.cr.ciMax);
            }
            else
            {
                if ( pfrPolicy->nBefore > 0 )
                {
                    int nBefore = pfrPolicy->nBefore;
                    for ( ; ; )
                    {
                        AEC_WrapLineBegin(&tr.cr.ciMin);
                        if ( nBefore == 0 || tr.cr.ciMin.nLine == 0 )
                            break;

                        AEC_PrevLine(&tr.cr.ciMin);
                        --nBefore;
                    }
                    nLinesBeforeAfter[0] = pfrPolicy->nBefore - nBefore;
                }
                if ( pfrPolicy->nAfter > 0)
                {
                    int nAfter = pfrPolicy->nAfter;
                    for ( ; ; )
                    {
                        AEC_WrapLineEnd(&tr.cr.ciMax);
                        if ( nAfter == 0 || AEC_IsLastCharInFile(&tr.cr.ciMax) )
                            break;

                        AEC_NextLine(&tr.cr.ciMax);
                        --nAfter;
                    }
                    nLinesBeforeAfter[1] = pfrPolicy->nAfter - nAfter;
                }
            }
        }
    }

    tr.bColumnSel = FALSE;
    tr.pBuffer = NULL;
    tr.nNewLine = AELB_R;
    tr.bFillSpaces = FALSE;

    tr.dwBufferMax = (UINT_PTR) SendMessage( hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM) &tr );
    if ( tr.dwBufferMax == 0 )
        return; // no text to retrieve

    nBytesToAllocate = sizeof(wchar_t) * tr.dwBufferMax;
    if ( pfrPolicy->nMode == QSFRM_LINE_CR )
        nBytesToAllocate += 1*sizeof(wchar_t); // for the trailing '\r'

    if ( !tDynamicBuffer_Allocate(pTempBuf, nBytesToAllocate) )
        return; // failed to allocate the memory

    pTempBuf->nBytesStored = 0;
    tr.pBuffer = (wchar_t *) pTempBuf->ptr;
    tr.pBuffer[0] = 0;
    pTempBuf->nBytesStored += sizeof(wchar_t) * (UINT_PTR) SendMessage( hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM) &tr );
    if ( pfrPolicy->nMode == QSFRM_LINE_CR )
    {
        tr.pBuffer += pTempBuf->nBytesStored / sizeof(wchar_t);
        *tr.pBuffer = L'\r'; // the trailing '\r'
        pTempBuf->nBytesStored += 1*sizeof(wchar_t); // includes the trailing '\r'
    }
    pfrPolicy->pfnStoreResultCallback( pFindContext, pcrFound, nLinesBeforeAfter, pTempBuf, pTempBuf2, pfnAddOccurrence );
}

typedef struct sQSFindAll {
    tFindResultCallback pfnFindResultCallback; // can be NULL
    tGetFindResultPolicy GetFindResultPolicy;
    tShowFindResults ShowFindResults;
    tDynamicBuffer tempBuf;
    tDynamicBuffer tempBuf2;
} tQSFindAll;


// funcs
HWND qsearchDoInitToolTip(HWND hDlg, HWND hEdit);
void qsearchDoQuit(HWND hEdit, HWND hToolTip, HMENU hPopupMenuLoaded, HBRUSH hBrush1, HBRUSH hBrush2, HBRUSH hBrush3);
void qsearchDoSearchText(HWND hEdit, DWORD dwParams, const DWORD dwOptFlags[], tQSFindAll* pFindAll /* = NULL */);
void qsearchDoSelFind(HWND hEdit, BOOL bFindPrev, const DWORD dwOptFlags[]);
void qsearchDoSetNotFound(HWND hEdit, BOOL bNotFound, BOOL bNotRegExp, BOOL bEOF);
void qsearchDoShowHide(HWND hDlg, BOOL bShow, UINT uShowFlags, const DWORD dwOptFlags[]);
HWND qsearchGetFindEdit(HWND hDlg);

#define UFHF_MOVE_TO_TOP_IF_EXISTS 0x0001
#define UFHF_KEEP_EDIT_TEXT        0x0002
BOOL qsearchFindHistoryAdd(HWND hEdit, const wchar_t* szTextAW, UINT uUpdFlags);

BOOL qsearchIsFindHistoryEnabled(void)
{
    return (g_Options.dwFindHistoryItems > 1);
}

BOOL qsearchIsFindHistoryBeingSaved(void)
{
    return ((g_Options.dwHistorySave & 0x01) != 0);
}

BOOL qsearchIsSearchFlagsBeingSaved(void)
{
    return ((g_Options.dwHistorySave & 0x02) != 0);
}

BOOL qsearchIsSavingHistoryToStdLocation(void)
{
    return ((g_Options.dwHistorySave & 0x04) != 0);
}

/*static void editSetTrailSel(HWND hEdit)
{
    if ( g_Plugin.bOldWindows )
    {
        int len = GetWindowTextLengthA(hEdit);
        SendMessageA(hEdit, EM_SETSEL, len, len);
    }
    else
    {
        int len = GetWindowTextLengthW(hEdit);
        SendMessageW(hEdit, EM_SETSEL, len, len);
    }
}*/

static UINT qs_Get_SO_QSEARCH(const DWORD dwOptFlags[])
{
    UINT uSearchOrigin;
    if ( (dwOptFlags[OPTF_EDITOR_AUTOFOCUS_MOUSE] & 0x01) &&
         (dwOptFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01) )
        uSearchOrigin = QS_SO_QSEARCH_FORCED;
    else
        uSearchOrigin = QS_SO_QSEARCH;
    return uSearchOrigin;
}

static void qsdlgShowHideWholeWordCheckBox(HWND hDlg, const DWORD dwOptFlags[])
{
    HWND    hChWholeWord;
    wchar_t wszText[64];

    hChWholeWord = GetDlgItem(hDlg, IDC_CH_WHOLEWORD);

    if ( g_Plugin.bOldWindows )
    {
        lstrcpyA( (char *) wszText, qsearchGetTextA(IDC_CH_WHOLEWORD) );
        if ( dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] )
            lstrcatA( (char *) wszText, "*" );
        else if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
            lstrcatA( (char *) wszText, "^" );
        SetWindowTextA(hChWholeWord, (const char *) wszText);
    }
    else
    {
        lstrcpyW( wszText, qsearchGetTextW(IDC_CH_WHOLEWORD) );
        if ( dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] )
            lstrcatW( wszText, L"*" );
        else if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
            lstrcatW( wszText, L"^" );
        SetWindowTextW(hChWholeWord, wszText);
    }

    /*static int nChHighlightAllLeft0 = 0;
    HWND hChWholeWord;
    HWND hChHighlightAll;
    RECT rectDlg;
    RECT rectChWholeWord;
    RECT rectChHighlightAll;

    hChWholeWord = GetDlgItem(hDlg, IDC_CH_WHOLEWORD);
    hChHighlightAll = GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL);
    GetWindowRect(hDlg, &rectDlg);
    GetWindowRect(hChWholeWord, &rectChWholeWord);
    GetWindowRect(hChHighlightAll, &rectChHighlightAll);

    if ( nChHighlightAllLeft0 == 0 )
        nChHighlightAllLeft0 = rectChHighlightAll.left - rectDlg.left;

    ShowWindow(
        hChWholeWord,
        dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] ? SW_HIDE : SW_SHOWNORMAL
    );

    MoveWindow(
        hChHighlightAll,
        dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] ?
          (rectChWholeWord.left - rectDlg.left) : nChHighlightAllLeft0,
        rectChHighlightAll.top - rectDlg.top,
        rectChHighlightAll.right - rectChHighlightAll.left,
        rectChHighlightAll.bottom - rectChHighlightAll.top,
        TRUE
    );*/
}

static DWORD getQSearchHotKey(void)
{
    if ( g_szFunctionQSearchW[0] )
    {
        if ( g_Plugin.bOldWindows )
        {
            PLUGINFUNCTION* pfA;

            pfA = (PLUGINFUNCTION *) SendMessage( g_Plugin.hMainWnd,
              AKD_DLLFINDA, (WPARAM) g_szFunctionQSearchW, 0 );
            if ( pfA )
            {
                return pfA->wHotkey;
            }
        }
        else
        {
            PLUGINFUNCTION* pfW;

            pfW = (PLUGINFUNCTION *) SendMessage( g_Plugin.hMainWnd,
              AKD_DLLFINDW, (WPARAM) g_szFunctionQSearchW, 0 );
            if ( pfW )
            {
                return pfW->wHotkey;
            }
        }
    }
    return 0;
}

static BOOL isQSearchHotKeyPressed(void)
{
    if ( qs_dwHotKey )
    {
        if ( GetKeyState(LOBYTE(qs_dwHotKey)) & 0x80 )
        {
            if ( GetKeyState(VK_MENU) & 0x80 )
            {
                if ( (HIBYTE(qs_dwHotKey) & HOTKEYF_ALT) != HOTKEYF_ALT )
                    return FALSE;
            }
            else
            {
                if ( HIBYTE(qs_dwHotKey) & HOTKEYF_ALT )
                    return FALSE;
            }

            if ( GetKeyState(VK_CONTROL) & 0x80 )
            {
                if ( (HIBYTE(qs_dwHotKey) & HOTKEYF_CONTROL) != HOTKEYF_CONTROL )
                    return FALSE;
            }
            else
            {
                if ( HIBYTE(qs_dwHotKey) & HOTKEYF_CONTROL )
                    return FALSE;
            }

            if ( GetKeyState(VK_SHIFT) & 0x80 )
            {
                if ( (HIBYTE(qs_dwHotKey) & HOTKEYF_SHIFT) != HOTKEYF_SHIFT )
                    return FALSE;
            }
            else
            {
                if ( HIBYTE(qs_dwHotKey) & HOTKEYF_SHIFT )
                    return FALSE;
            }

            return TRUE;
        }
    }
    return FALSE;
}

static void getEditFindText(HWND hEdit, wchar_t szTextAW[MAX_TEXT_SIZE])
{
    szTextAW[0] = 0;
    if ( g_Plugin.bOldWindows )
        GetWindowTextA( hEdit, (LPSTR) szTextAW, MAX_TEXT_SIZE - 1 );
    else
        GetWindowTextW( hEdit, (LPWSTR) szTextAW, MAX_TEXT_SIZE - 1 );
}

static void setEditFindText(HWND hEdit, const wchar_t* pszTextAW)
{
    if ( g_Plugin.bOldWindows )
    {
        SetWindowTextA( hEdit, (LPCSTR) pszTextAW );
    }
    else
    {
        SetWindowTextW( hEdit, (LPCWSTR) pszTextAW );
    }
}

static void cutEditText(HWND hEdit, BOOL bCutAfterCaret)
{
    DWORD   dwSelPos1 = 0;
    DWORD   dwSelPos2 = 0;
    UINT    len = 0;

    SendMessage( hEdit, EM_GETSEL, (WPARAM) &dwSelPos1, (LPARAM) &dwSelPos2 );

    if ( g_Plugin.bOldWindows )
        len = (UINT) GetWindowTextLengthA(hEdit);
    else
        len = (UINT) GetWindowTextLengthW(hEdit);

    if ( bCutAfterCaret )
    {
        if ( dwSelPos1 < len )
        {
            /*if ( dwSelPos1 < 0 )
                dwSelPos1 = 0;*/

            //SendMessage( hEdit, WM_SETREDRAW, FALSE, 0 );
            SendMessage( hEdit, EM_SETSEL, dwSelPos1, -1 );
            //SendMessage( hEdit, WM_SETREDRAW, TRUE, 0 );

            if ( g_Plugin.bOldWindows )
            {
                SendMessageA( hEdit, EM_REPLACESEL, TRUE, (LPARAM) "" );
            }
            else
            {
                SendMessageW( hEdit, EM_REPLACESEL, TRUE, (LPARAM) L"" );
            }

            SendMessage( hEdit, EM_SETSEL, dwSelPos1, dwSelPos1 );
        }
    }
    else
    {
        if ( dwSelPos2 > 0 )
        {
            if ( dwSelPos2 > len )
                dwSelPos2 = len;

            //SendMessage( hEdit, WM_SETREDRAW, FALSE, 0 );
            SendMessage( hEdit, EM_SETSEL, 0, dwSelPos2 );
            //SendMessage( hEdit, WM_SETREDRAW, TRUE, 0 );

            if ( g_Plugin.bOldWindows )
            {
                SendMessageA( hEdit, EM_REPLACESEL, TRUE, (LPARAM) "" );
            }
            else
            {
                SendMessageW( hEdit, EM_REPLACESEL, TRUE, (LPARAM) L"" );
            }

            SendMessage( hEdit, EM_SETSEL, 0, 0 );
        }
    }
}

static BOOL getAkelPadSelectedText(wchar_t szTextAW[MAX_TEXT_SIZE], const DWORD dwOptFlags[])
{
    EDITINFO  ei;

    g_bWordJustSelectedByFnd = FALSE;

    ei.hWndEdit = NULL;
    SendMessage( g_Plugin.hMainWnd,
      AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei );
    if ( ei.hWndEdit )
    {
        CHARRANGE_X cr = { 0, 0 };

        SendMessage( ei.hWndEdit, EM_EXGETSEL_X, 0, (LPARAM) &cr );
        if ( cr.cpMin != cr.cpMax )
        {
            if ( cr.cpMax >= cr.cpMin + MAX_TEXT_SIZE )
                cr.cpMax = cr.cpMin + MAX_TEXT_SIZE - 1;

            if ( g_Plugin.bOldWindows )
            {
                TEXTRANGEA_X trA;

                trA.chrg.cpMin = cr.cpMin;
                trA.chrg.cpMax = cr.cpMax;
                trA.lpstrText = (LPSTR) szTextAW;
                SendMessageA( ei.hWndEdit, EM_GETTEXTRANGE_X, 0, (LPARAM) &trA );
                if ( dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] ||
                     dwOptFlags[OPTF_SRCH_USE_REGEXP] )
                {
                    int  i;
                    int  k;
                    char szTextA[2*MAX_TEXT_SIZE];

                    i = 0;
                    k = 0;
                    while ( trA.lpstrText[i] )
                    {
                        switch ( trA.lpstrText[i] )
                        {
                            case '\t':
                                szTextA[k++] = '\\';
                                szTextA[k++] = 't';
                                break;
                            case '\r':
                                szTextA[k++] = '\\';
                                szTextA[k++] = 'n';
                                break;
                            case '\\':
                            case '*':
                            case '?':
                                szTextA[k++] = '\\';
                            default:
                                if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
                                {
                                    switch ( trA.lpstrText[i] )
                                    {
                                        case '(':
                                        case ')':
                                        case '[':
                                        case ']':
                                        case '{':
                                        case '}':
                                        case '.':
                                        case '!':
                                        case '|':
                                        case '+':
                                        case '^':
                                        case '$':
                                            szTextA[k++] = '\\';
                                            break;
                                    }
                                }
                                szTextA[k++] = trA.lpstrText[i];
                                break;
                        }
                        ++i;
                    }
                    if ( k > i )
                    {
                        if ( k > MAX_TEXT_SIZE - 1 )
                        {
                            k = MAX_TEXT_SIZE - 1;
                        }
                        szTextA[k] = 0;
                        i = 0;
                        while ( (trA.lpstrText[i] = szTextA[i]) != 0 )
                        {
                            ++i;
                        }
                    }
                }
            }
            else
            {
                TEXTRANGEW_X trW;

                trW.chrg.cpMin = cr.cpMin;
                trW.chrg.cpMax = cr.cpMax;
                trW.lpstrText = (LPWSTR) szTextAW;
                SendMessageW( ei.hWndEdit, EM_GETTEXTRANGE_X, 0, (LPARAM) &trW );
                if ( dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] ||
                     dwOptFlags[OPTF_SRCH_USE_REGEXP] )
                {
                    int     i;
                    int     k;
                    wchar_t szTextW[2*MAX_TEXT_SIZE];

                    i = 0;
                    k = 0;
                    while ( trW.lpstrText[i] )
                    {
                        switch ( trW.lpstrText[i] )
                        {
                            case L'\t':
                                szTextW[k++] = L'\\';
                                szTextW[k++] = L't';
                                break;
                            case L'\r':
                                szTextW[k++] = L'\\';
                                szTextW[k++] = L'n';
                                break;
                            case L'\\':
                            case L'*':
                            case L'?':
                                szTextW[k++] = L'\\';
                            default:
                                if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
                                {
                                    switch ( trW.lpstrText[i] )
                                    {
                                        case L'(':
                                        case L')':
                                        case L'[':
                                        case L']':
                                        case L'{':
                                        case L'}':
                                        case L'.':
                                        case L'!':
                                        case L'|':
                                        case L'+':
                                        case L'^':
                                        case L'$':
                                            szTextW[k++] = L'\\';
                                            break;
                                    }
                                }
                                szTextW[k++] = trW.lpstrText[i];
                                break;
                        }
                        ++i;
                    }
                    if ( k > i )
                    {
                        if ( k > MAX_TEXT_SIZE - 1 )
                        {
                            k = MAX_TEXT_SIZE - 1;
                        }
                        szTextW[k] = 0;
                        i = 0;
                        while ( (trW.lpstrText[i] = szTextW[i]) != 0 )
                        {
                            ++i;
                        }
                    }
                }
            }
            qs_bEditTextChanged = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

static BOOL EnableDlgItem(HWND hDlg, int itemID, BOOL bEnable)
{
    HWND hDlgItem = GetDlgItem(hDlg, itemID);
    return ( hDlgItem ? EnableWindow(hDlgItem, bEnable) : FALSE );
}

static WNDPROC setWndProc(HWND hWnd, WNDPROC newWndProc)
{
    if ( g_Plugin.bOldWindows )
        return (WNDPROC) SetWindowLongPtrA( hWnd, GWLP_WNDPROC, (LONG_PTR) newWndProc );
    else
        return (WNDPROC) SetWindowLongPtrW( hWnd, GWLP_WNDPROC, (LONG_PTR) newWndProc );
}

static LRESULT callWndProc(WNDPROC prevWndProc,
  HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( g_Plugin.bOldWindows )
        return CallWindowProcA(prevWndProc, hWnd, uMsg, wParam, lParam);
    else
        return CallWindowProcW(prevWndProc, hWnd, uMsg, wParam, lParam);
}

static LRESULT CALLBACK btnFindWndProc(HWND hBtn,
  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL bTrackingMouse = FALSE;
    WNDPROC prevWndProc;

    switch ( uMsg )
    {
        case WM_SETFOCUS:
        {
            qs_bBtnFindIsFocused = TRUE;
            break;
        }
        case WM_KILLFOCUS:
        {
            HWND hWndToFocus;

            hWndToFocus = (HWND) wParam;
            if ( (hWndToFocus != g_QSearchDlg.hBtnFindNext) &&
                 (hWndToFocus != g_QSearchDlg.hBtnFindPrev) &&
                 (hWndToFocus != g_QSearchDlg.hBtnFindAll) )
            {
                qs_bBtnFindIsFocused = FALSE;
                if ( hWndToFocus != g_QSearchDlg.hFindEdit )
                {
                    qs_nEditEOF = 0;
                    PostMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
                }
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            if ( !bTrackingMouse )
            {
                TRACKMOUSEEVENT tme;

                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hBtn;
                tme.dwHoverTime = HOVER_DEFAULT;
                if ( _TrackMouseEvent(&tme) )
                {
                    bTrackingMouse = TRUE;
                }
            }
            break;
        }
        case WM_MOUSELEAVE:
        {
            bTrackingMouse = FALSE;
            if ( qs_bBtnFindIsFocused )
            {
                g_QSearchDlg.bMouseJustLeavedFindEdit = TRUE;
            }
            break;
        }
        case WM_NOTIFY:
        {
            if ( hBtn == g_QSearchDlg.hBtnFindAll )
            {
                if ( g_Plugin.bOldWindows )
                {
                    if ( ((LPNMHDR) lParam)->code == TTN_GETDISPINFOA )
                    {
                        LPNMTTDISPINFOA lpnmdiA;

                        lpnmdiA = (LPNMTTDISPINFOA) lParam;
                        SendMessageA(lpnmdiA->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
                        lpnmdiA->lpszText = (LPSTR) qsearchGetHintA(IDC_BT_FINDALL);
                        return 0;
                    }
                }
                else
                {
                    if ( ((LPNMHDR) lParam)->code == TTN_GETDISPINFOW )
                    {
                        LPNMTTDISPINFOW lpnmdiW;

                        lpnmdiW = (LPNMTTDISPINFOW) lParam;
                        SendMessageW(lpnmdiW->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
                        lpnmdiW->lpszText = (LPWSTR) qsearchGetHintW(IDC_BT_FINDALL);
                        return 0;
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    if ( hBtn == g_QSearchDlg.hBtnFindNext )
        prevWndProc = prev_btnFindNextWndProc;
    else if ( hBtn == g_QSearchDlg.hBtnFindPrev )
        prevWndProc = prev_btnFindPrevWndProc;
    else
        prevWndProc = prev_btnFindAllWndProc;
    return callWndProc(prevWndProc, hBtn, uMsg, wParam, lParam);
}

/*
static LRESULT CALLBACK btnWndProc(HWND hBtn,
  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL qs_bBtnTrackMouse = FALSE;

    switch ( uMsg )
    {
        case WM_MOUSEMOVE:
        {
            if ( !qs_bBtnTrackMouse )
            {
                TRACKMOUSEEVENT tme;
                LONG_PTR        dwWndStyle;

                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hBtn;
                tme.dwHoverTime = HOVER_DEFAULT;
                if ( _TrackMouseEvent(&tme) )
                {
                    qs_bBtnTrackMouse = TRUE;
                }
                if ( g_Plugin.bOldWindows )
                    dwWndStyle = GetWindowLongPtrA(hBtn, GWL_STYLE);
                else
                    dwWndStyle = GetWindowLongPtrW(hBtn, GWL_STYLE);
                if ( dwWndStyle & BS_FLAT )
                {
                    dwWndStyle -= BS_FLAT;
                    dwWndStyle |= BS_PUSHLIKE;
                    if ( g_Plugin.bOldWindows )
                        SetWindowLongPtrA(hBtn, GWL_STYLE, dwWndStyle);
                    else
                        SetWindowLongPtrW(hBtn, GWL_STYLE, dwWndStyle);
                    SendMessage(
                      hBtn,
                      BM_SETSTYLE,
                      BS_PUSHLIKE | BS_BITMAP | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
                      TRUE );
                }
            }
            break;
        }
        case WM_MOUSELEAVE:
        {
            LONG_PTR dwWndStyle;

            if ( g_Plugin.bOldWindows )
                dwWndStyle = GetWindowLongPtrA(hBtn, GWL_STYLE);
            else
                dwWndStyle = GetWindowLongPtrW(hBtn, GWL_STYLE);
            if ( dwWndStyle & BS_PUSHLIKE )
            {
                dwWndStyle -= BS_PUSHLIKE;
                dwWndStyle |= BS_FLAT;
                if ( g_Plugin.bOldWindows )
                    SetWindowLongPtrA(hBtn, GWL_STYLE, dwWndStyle);
                else
                    SetWindowLongPtrW(hBtn, GWL_STYLE, dwWndStyle);
                SendMessage(
                  hBtn,
                  BM_SETSTYLE,
                  BS_FLAT | BS_BITMAP | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
                  TRUE );
            }
            qs_bBtnTrackMouse = FALSE;
            break;
        }
        default:
            break;
    }

    return callWndProc(prev_btnWndProc, hBtn, uMsg, wParam, lParam);
}
*/

static wchar_t virtKeyToCharW(DWORD dwKey)
{
    UINT uCh = MapVirtualKeyW(dwKey, 2 /*MAPVK_VK_TO_CHAR*/);
    wchar_t wch = (wchar_t) (uCh & 0xFFFF);
    return wch;
}

static char virtKeyToCharA(DWORD dwKey)
{
    UINT uCh = MapVirtualKeyA(dwKey, 2 /*MAPVK_VK_TO_CHAR*/);
    char ch = (char) (uCh & 0xFF);
    return ch;
}

#define AHKF_PARENTHESES 0x01
#define AHKF_INDENT      0x02
static void strFormatAltHotkeyW(wchar_t* szHotkeyW, DWORD dwAltHotkey, UINT uFlags)
{
    wchar_t* p;

    p = szHotkeyW;
    if ( uFlags & AHKF_INDENT )
        *(p++) = L' ';
    if ( uFlags & AHKF_PARENTHESES )
        *(p++) = L'(';
    *(p++) = L'A';
    *(p++) = L'l';
    *(p++) = L't';
    *(p++) = L'+';
    *(p++) = virtKeyToCharW(dwAltHotkey);
    if ( uFlags & AHKF_PARENTHESES )
        *(p++) = L')';
    *p = 0;
}

static void strFormatAltHotkeyA(char* szHotkeyA, DWORD dwAltHotkey, UINT uFlags)
{
    char* p;

    p = szHotkeyA;
    if ( uFlags & AHKF_INDENT )
        *(p++) = ' ';
    if ( uFlags & AHKF_PARENTHESES )
        *(p++) = '(';
    *(p++) = 'A';
    *(p++) = 'l';
    *(p++) = 't';
    *(p++) = '+';
    *(p++) = virtKeyToCharA(dwAltHotkey);
    if ( uFlags & AHKF_PARENTHESES )
        *(p++) = ')';
    *p = 0;
}

static void strAppendAltHotkeyW(wchar_t* strW, DWORD dwAltHotkey)
{
    wchar_t szHotKeyW[32];

    strFormatAltHotkeyW(szHotKeyW, dwAltHotkey, AHKF_INDENT | AHKF_PARENTHESES);
    lstrcatW(strW, szHotKeyW);
}

static void strAppendAltHotkeyA(char* strA, DWORD dwAltHotkey)
{
    char szHotKeyA[32];

    strFormatAltHotkeyA(szHotKeyA, dwAltHotkey, AHKF_INDENT | AHKF_PARENTHESES);
    lstrcatA(strA, szHotKeyA);
}

static void OnSrchModeChanged()
{
    qsdlgShowHideWholeWordCheckBox(g_QSearchDlg.hDlg, g_Options.dwFlags);
    qs_bForceFindFirst = TRUE;
    qs_nEditEOF = 0;
    qsearchDoSetNotFound( g_QSearchDlg.hFindEdit, FALSE, FALSE, FALSE );

    if ( isCheckBoxChecked(g_QSearchDlg.hDlg, IDC_CH_HIGHLIGHTALL) )
    {
        qsearchDoTryUnhighlightAll();
    }

    qsSetInfoEmpty();
}

static void OnChWholeWordSrchMode()
{
    if ( g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] )
    {
        g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 0;
        g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] = 1;
        CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHUSESPECIALCHARS, MF_BYCOMMAND | MF_UNCHECKED );
        CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE, MF_BYCOMMAND | MF_CHECKED );
        EnableMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE, MF_BYCOMMAND | MF_ENABLED );
    }
    else if ( g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] )
    {
        g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 0;
        g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] = 0;
        CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE, MF_BYCOMMAND | MF_UNCHECKED );
        EnableMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE, MF_BYCOMMAND | MF_GRAYED );
        CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHUSESPECIALCHARS, MF_BYCOMMAND | MF_UNCHECKED );
    }
    else
    {
        g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 1;
        g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] = 0;
        CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE, MF_BYCOMMAND | MF_UNCHECKED );
        EnableMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE, MF_BYCOMMAND | MF_GRAYED );
        CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHUSESPECIALCHARS, MF_BYCOMMAND | MF_CHECKED );
    }

    OnSrchModeChanged();
}

static LRESULT CALLBACK chWholeWordWndProc(HWND hCh,
  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg )
    {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            if ( GetKeyState(VK_QS_WW_SRCH_MODE) & 0x80 )
            {
                OnChWholeWordSrchMode();
                return 0;
            }
            break;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONDBLCLK:
            OnChWholeWordSrchMode();
            return 0;

        case WM_NOTIFY:
            if ( g_Plugin.bOldWindows )
            {
                if ( ((LPNMHDR) lParam)->code == TTN_GETDISPINFOA )
                {
                    static char szHintA[128];
                    char szHotkeyWordA[32];
                    char szHotkeyModeA[32];
                    LPNMTTDISPINFOA lpnmdiA;

                    szHotkeyWordA[0] = 0;
                    szHotkeyModeA[0] = 0;
                    if ( g_Options.dwUseAltHotkeys )
                    {
                        strFormatAltHotkeyA(szHotkeyWordA, g_Options.dwAltWholeWord, AHKF_INDENT | AHKF_PARENTHESES);
                        strFormatAltHotkeyA(szHotkeyModeA, g_Options.dwAltSearchMode, 0);
                        lstrcatA(szHotkeyModeA, " / ");
                    }
                    wsprintfA(szHintA, qsearchGetHintA(IDC_CH_WHOLEWORD), szHotkeyWordA, szHotkeyModeA);

                    lpnmdiA = (LPNMTTDISPINFOA) lParam;
                    SendMessageA(lpnmdiA->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
                    lpnmdiA->lpszText = szHintA;
                    return 0;
                }
            }
            else
            {
                if ( ((LPNMHDR) lParam)->code == TTN_GETDISPINFOW )
                {
                    static wchar_t szHintW[128];
                    wchar_t szHotkeyWordW[32];
                    wchar_t szHotkeyModeW[32];
                    LPNMTTDISPINFOW lpnmdiW;

                    szHotkeyWordW[0] = 0;
                    szHotkeyModeW[0] = 0;
                    if ( g_Options.dwUseAltHotkeys )
                    {
                        strFormatAltHotkeyW(szHotkeyWordW, g_Options.dwAltWholeWord, AHKF_INDENT | AHKF_PARENTHESES);
                        strFormatAltHotkeyW(szHotkeyModeW, g_Options.dwAltSearchMode, 0);
                        lstrcatW(szHotkeyModeW, L" / ");
                    }
                    wsprintfW(szHintW, qsearchGetHintW(IDC_CH_WHOLEWORD), szHotkeyWordW, szHotkeyModeW);

                    lpnmdiW = (LPNMTTDISPINFOW) lParam;
                    SendMessageW(lpnmdiW->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
                    lpnmdiW->lpszText = szHintW;
                    return 0;
                }
            }
            break;
    }

    return callWndProc(prev_chWholeWordWndProc, hCh, uMsg, wParam, lParam);
}

UINT qsPickUpSelection(HWND hEdit, const DWORD dwOptFlags[], BOOL isHighlightAll)
{
    UINT nResult = 0;
    wchar_t prevFindTextW[MAX_TEXT_SIZE];

    if ( g_Plugin.bOldWindows )
    {
        lstrcpyA( (LPSTR) prevFindTextW, (LPCSTR) g_QSearchDlg.szFindTextW );
    }
    else
    {
        lstrcpyW( (LPWSTR) prevFindTextW, (LPCWSTR) g_QSearchDlg.szFindTextW );
    }

    if ( getAkelPadSelectedText(g_QSearchDlg.szFindTextW, dwOptFlags) )
    {
        BOOL bMatchCase = isCheckBoxChecked(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE);

        if ( !isHighlightAll )
        {
            qs_bEditTextChanged = FALSE;

            if ( g_Plugin.bOldWindows )
            {
                int (WINAPI *cmpfuncA)(LPCSTR, LPCSTR) = bMatchCase ? lstrcmpA : lstrcmpiA;
                if ( cmpfuncA((LPCSTR) prevFindTextW, (LPCSTR) g_QSearchDlg.szFindTextW) != 0 )
                {
                    qs_bEditTextChanged = TRUE;
                }
            }
            else
            {
                int (WINAPI *cmpfuncW)(LPCWSTR, LPCWSTR) = bMatchCase ? lstrcmpW : lstrcmpiW;
                if ( cmpfuncW((LPCWSTR) prevFindTextW, (LPCWSTR) g_QSearchDlg.szFindTextW) != 0 )
                {
                    qs_bEditTextChanged = TRUE;
                }
            }
        }
        else
        {
            qs_bEditTextChanged = TRUE;
        }

        if ( qs_bEditTextChanged )
        {
            qsearchDoSetNotFound(hEdit, FALSE, FALSE, FALSE);
            setEditFindText(hEdit, g_QSearchDlg.szFindTextW);

            SendMessage(hEdit, EM_SETSEL, 0, -1);
            qs_bEditSelJustChanged = TRUE;

            if ( dwOptFlags[OPTF_SRCH_ONTHEFLY_MODE] )
            {
                qs_bForceFindFirst = FALSE;
                qsearchDoTryHighlightAll(g_QSearchDlg.hDlg, dwOptFlags, QHC_CHECKBOX_CHECKED);
            }
            else
                qs_bForceFindFirst = TRUE;

            nResult |= QS_PSF_TEXTCHANGED;
        }

        nResult |= QS_PSF_PICKEDUP;
    }

    return nResult;
}

void qsChangeCkeckBoxState(WORD idCheckBox)
{
    const WPARAM wp = (BN_CLICKED << 16) & 0xFFFF0000L;
    WPARAM wState;
    HWND   hCh;

    hCh = GetDlgItem(g_QSearchDlg.hDlg, idCheckBox);

    if ( idCheckBox == IDC_CH_WHOLEWORD )
    {
        if ( !IsWindowVisible(hCh) )
            return;
    }

    if ( SendMessage(hCh, BM_GETCHECK, 0, 0) == BST_CHECKED )
        wState = BST_UNCHECKED;
    else
        wState = BST_CHECKED;

    SendMessage( hCh, BM_SETCHECK, wState, 0 );
    SendMessage( g_QSearchDlg.hDlg, WM_COMMAND, wp | idCheckBox, (LPARAM) hCh );
}

static LRESULT OnEditKeyDown_Enter_or_F3(HWND hEdit, WPARAM wParam, const DWORD dwOptFlags[])
{
    BOOL isPickupText;
    BOOL isFindBegin;
    BOOL isFindUp;

    isPickupText = GetKeyState(VK_QS_PICKUPTEXT) & 0x80; // Ctrl
    isFindBegin = GetKeyState(VK_QS_FINDBEGIN) & 0x80; // Alt
    isFindUp = GetKeyState(VK_QS_FINDUP) & 0x80; // Shift

    if ( isPickupText && !isFindBegin && !isFindUp )
    {
        if ( getAkelPadSelectedText(g_QSearchDlg.szFindTextW, dwOptFlags) )
        {
            setEditFindText(hEdit, g_QSearchDlg.szFindTextW);
            SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
            qsSetInfoEmpty();
        }
        SendMessage(hEdit, EM_SETSEL, 0, -1);
#ifdef _DEBUG
        Debug_Output("editWndProc, WM_KEYDOWN, (RETURN||F3)&&PickUp, SETSEL(0, -1)\n");
#endif
        qs_bEditTextChanged = TRUE;
        if ( dwOptFlags[OPTF_SRCH_ONTHEFLY_MODE] )
        {
            qs_bForceFindFirst = FALSE;
            qsearchDoTryHighlightAll(g_QSearchDlg.hDlg, dwOptFlags, QHC_CHECKBOX_CHECKED);
        }
        else
            qs_bForceFindFirst = TRUE;
    }
    else
    {
        if ( qsearchIsFindHistoryEnabled() && (wParam == VK_RETURN) )
        {
            HWND hCombo = GetDlgItem(g_QSearchDlg.hDlg, IDC_CB_FINDTEXT);
            if ( SendMessage(hCombo, CB_GETDROPPEDSTATE, 0, 0) )
            {
                // drop-down list box is shown
                int iItem = (int) SendMessage(hCombo, CB_GETCURSEL, 0, 0);
                if ( iItem != CB_ERR )
                {
                        //g_QSearchDlg.szFindTextW[0] = 0;
                        //SendMessageA(hCombo, CB_GETLBTEXT, iItem, (LPARAM) g_QSearchDlg.szFindTextW);
                        //SendMessageA(hCombo, CB_SETCURSEL, iItem, 0);
                    SendMessage(hCombo, CB_SHOWDROPDOWN, FALSE, 0);
                        //SetWindowTextA(hEdit, (LPCSTR) g_QSearchDlg.szFindTextW);
                    SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
                        //SendMessageA(hEdit, EM_SETSEL, 0, -1);
                    SetFocus(hEdit);
                    qs_bForceFindFirst = TRUE;
                    qs_bEditTextChanged = TRUE;
                    g_QSearchDlg.uSearchOrigin = qs_Get_SO_QSEARCH(dwOptFlags);
                }
                else
                {
                    SendMessage(hCombo, CB_SHOWDROPDOWN, FALSE, 0);
                    SetFocus(hEdit);
                }
                return 0;
            }
        }

        if ( isFindBegin && isFindUp && !isPickupText )
        {
            SendMessage( g_QSearchDlg.hDlg, QSM_FINDALL, g_Options.dwFindAllMode, 0 );
        }
        else if ( isPickupText && isFindUp )
        {
            SendMessage( g_QSearchDlg.hDlg, QSM_FINDALL, g_Options.dwFindAllMode | QS_FINDALL_RSLT_ALLFILES, 0 );
        }
        else if ( !isPickupText )
        {
            SendMessage( g_QSearchDlg.hDlg, WM_COMMAND, IDOK, 0 );
        }
    }

    return 0;
}

LRESULT CALLBACK editWndProc(HWND hEdit,
  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static DWORD dwSelPos1 = 0;
    static DWORD dwSelPos2 = (DWORD)(-1L);
    static BOOL  bHotKeyPressed = FALSE;
    static BOOL  bEditTrackingMouse = FALSE;

#ifdef _DEBUG
    Debug_Output("Edit Msg 0x%X:  0x0%X  0x0%X\n", uMsg, wParam, lParam);
#endif

    switch ( uMsg )
    {
        case WM_KEYDOWN:
        {
            bHotKeyPressed = FALSE;

            if ( (wParam == VK_DOWN) || (wParam == VK_UP) ||
                 (wParam == VK_PRIOR) || (wParam == VK_NEXT) )
            {
                if ( qsearchIsFindHistoryEnabled() )
                {
                    HWND hCombo = GetDlgItem(g_QSearchDlg.hDlg, IDC_CB_FINDTEXT);
                    if ( !SendMessage(hCombo, CB_GETDROPPEDSTATE, 0, 0) )
                    {
                        int iItem = (int) SendMessage(hCombo, CB_GETCURSEL, 0, 0);

                        getEditFindText(hEdit, g_QSearchDlg.szFindTextW);
                        if ( qsearchFindHistoryAdd(hEdit, g_QSearchDlg.szFindTextW, 0) )
                        {
                            if ( wParam == VK_DOWN ) // Arrow Down
                            {
                                if ( SendMessage(hCombo, CB_GETCOUNT, 0, 0) > (iItem + 1) )
                                    ++iItem; // next item
                            }
                            else if ( wParam == VK_NEXT ) // Page Down
                            {
                                iItem = (int) SendMessage(hCombo, CB_GETCOUNT, 0, 0);
                                if ( iItem > 0 )
                                    --iItem; // last item
                            }
                            SendMessage(hCombo, CB_SETCURSEL, iItem, 0);
                            g_QSearchDlg.uSearchOrigin = qs_Get_SO_QSEARCH(g_Options.dwFlags);
                        }
                    }
                }
            }

            if ( (wParam == VK_RETURN) || (wParam == VK_F3) )
            {
                return OnEditKeyDown_Enter_or_F3(hEdit, wParam, g_Options.dwFlags);
            }
            else if ( (wParam == VK_DELETE) || (wParam == VK_BACK) )
            {
                qs_bEditTextChanged = TRUE;
                g_QSearchDlg.uSearchOrigin = QS_SO_QSEARCH;

                if ( GetKeyState(VK_CONTROL) & 0x80 ) // Ctrl+Del, Ctrl+BS
                {
                    cutEditText( hEdit, (wParam == VK_DELETE) );
                    if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] )
                    {
                        SendMessage( g_QSearchDlg.hDlg, QSM_FINDFIRST, QSEARCH_NOFINDUP | QSEARCH_NOFINDBEGIN | QSEARCH_USEDELAY, 0 );
                    }
                    else
                    {
                        SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
                    }
                    return 0;
                }
                else if ( wParam == VK_DELETE ) // Del
                {
                    if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] )
                    {
                        LRESULT lResult = callWndProc(prev_editWndProc,
                                            hEdit, uMsg, wParam, lParam);
                        SendMessage( g_QSearchDlg.hDlg, QSM_FINDFIRST, QSEARCH_NOFINDUP | QSEARCH_NOFINDBEGIN | QSEARCH_USEDELAY, 0 );
                        return lResult;
                    }
                    else
                    {
                        LRESULT lResult = callWndProc(prev_editWndProc,
                                            hEdit, uMsg, wParam, lParam);
                        SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
                        return lResult;
                    }
                }
                // BS is processed in WM_CHAR
            }
            else if ( wParam == VK_ESCAPE )
            {
                HWND hCombo = NULL;

                if ( qsearchIsFindHistoryEnabled() )
                {
                    hCombo = GetDlgItem(g_QSearchDlg.hDlg, IDC_CB_FINDTEXT);
                    if ( !SendMessage(hCombo, CB_GETDROPPEDSTATE, 0, 0) )
                        hCombo = NULL;
                }

                if ( hCombo )
                {
                    // drop-down list box is shown - hide it
                    int iItem = (int) SendMessage(hCombo, CB_GETCURSEL, 0, 0);
                    if ( iItem != CB_ERR )
                    {
                        SendMessage(hCombo, CB_SHOWDROPDOWN, FALSE, 0);
                        SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
                        SetFocus(hEdit);
                        qs_bForceFindFirst = TRUE;
                        qs_bEditTextChanged = TRUE;
                        g_QSearchDlg.uSearchOrigin = qs_Get_SO_QSEARCH(g_Options.dwFlags);
                    }
                    else
                    {
                        SendMessage(hCombo, CB_SHOWDROPDOWN, FALSE, 0);
                        SetFocus(hEdit);
                    }
                    return 0;
                }
                else
                {
                    // drop-down list box is hidden
                    if ( g_Options.dwFlags[OPTF_CATCH_MAIN_ESC] == ESC_THROW_TO_MAIN )
                    {
                        PostMessage( g_Plugin.hMainWnd, WM_COMMAND, 0x10000 | IDM_FILE_EXIT, 0 );
                        return 0;
                    }

                    SendMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, FALSE, 0 );
                    return 0;
                }
            }
            else if ( wParam == VK_TAB )
            {
                // if ( GetKeyState(VK_CONTROL) & 0x80 ) // Ctrl+Tab, Ctrl+Shift+Tab, ...
                // {
                //
                // }
                return 0;
            }
            else if ( wParam == 0x41 ) // 'A'
            {
                if ( GetKeyState(VK_CONTROL) & 0x80 ) // Ctrl+A
                {
                    // we can have Grey Alt here (e.g. Polish keyboard layout)
                    // even though Alt usually comes under WM_SYSKEYDOWN
                    // Grey Alt comes under WM_KEYDOWN as VK_CONTROL + VK_MENU
                    if ( (GetKeyState(VK_MENU) & 0x80) != 0x80 ) // no Grey Alt
                    {
                        SendMessage( hEdit, EM_SETSEL, 0, -1 );
#ifdef _DEBUG
                        Debug_Output("editWndProc, WM_KEYDOWN, Ctrl+A, SETSEL(0, -1)\n");
#endif
                        return 0;
                    }
                }
            }
            else if ( LOBYTE(wParam) == LOBYTE(qs_dwHotKey) )
            {
                bHotKeyPressed = isQSearchHotKeyPressed();
                return 0;
            }
            break;
        }
        case WM_SYSKEYDOWN:
        {
            // it's required when qs_dwHotKey contains Alt
            // (Alt+key do not come as WM_KEYDOWN)
            if ( LOBYTE(wParam) == LOBYTE(qs_dwHotKey) )
            {
                bHotKeyPressed = isQSearchHotKeyPressed();
                return 0;
            }
            if ( (wParam == VK_RETURN) || (wParam == VK_F3) )
            {
                // (if Alt+Enter or Alt+F3 pressed)
                OnEditKeyDown_Enter_or_F3(hEdit, wParam, g_Options.dwFlags);
                return 0;
            }
            /*
            if ( g_Options.dwUseAltHotkeys )
            {
                if ( wParam == g_Options.dwAltMatchCase )
                {
                    qsChangeCkeckBoxState(IDC_CH_MATCHCASE);
                    return 1;
                }
                if ( wParam == g_Options.dwAltHighlightAll )
                {
                    qsChangeCkeckBoxState(IDC_CH_HIGHLIGHTALL);
                    return 1;
                }
                if ( wParam == g_Options.dwAltWholeWord )
                {
                    qsChangeCkeckBoxState(IDC_CH_WHOLEWORD);
                    return 1;
                }
            }
            */
            break;
        }
        case WM_KEYUP:
        {
            if ( bHotKeyPressed )
            {
                bHotKeyPressed = FALSE;
                if ( !qs_bHotKeyPressedOnShow )
                {
                    if ( g_Options.dwFlags[OPTF_HOTKEY_HIDES_PANEL] & 0x01 )
                    {
                        SendMessage( g_QSearchDlg.hDlg, QSM_SHOWHIDE, FALSE, 0 );
                    }
                    else
                    {
                        SetFocus( g_Plugin.hMainWnd );
                        /*if ( (g_Options.dwFlags[OPTF_EDITOR_AUTOFOCUS_MOUSE] & AUTOFOCUS_EDITOR_ALWAYS) == AUTOFOCUS_EDITOR_ALWAYS )
                        {
                            qs_bEditIsActive = FALSE;
                            qs_bEditTrackMouse = FALSE;
                        }*/
                    }
                    return 0;
                }
            }

            qs_bHotKeyPressedOnShow = FALSE;

            if ( (wParam == VK_RETURN) ||
                 (wParam == VK_ESCAPE) ||
                 (wParam == VK_TAB) )
            {
                return 0;
            }
            break;
        }
        case WM_SYSKEYUP:
        {
            if ( bHotKeyPressed )
            {
                SendMessage(hEdit, WM_KEYUP, wParam, lParam);
                return 0;
            }
            break;
        }
        case WM_CHAR:
        {
            if ( bHotKeyPressed ||
                 (wParam == VK_RETURN) ||
                 (wParam == VK_ESCAPE) ||
                 (wParam == VK_TAB) ||
                 (wParam == 0x0A) ||      /* 0x0A is Ctrl+Enter. Why? Ask M$ ;) */
                 (wParam == 0x7F) )       /* 0x7F is Ctrl+Backspace. Why? Ask M$ */
            {
                return 0;
            }
            else
            {
                qs_bEditTextChanged = TRUE;
                g_QSearchDlg.uSearchOrigin = QS_SO_QSEARCH;
                if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] )
                {
                    LRESULT lResult = callWndProc(prev_editWndProc,
                                        hEdit, uMsg, wParam, lParam);
                    SendMessage( g_QSearchDlg.hDlg, QSM_FINDFIRST, QSEARCH_NOFINDUP | QSEARCH_NOFINDBEGIN | QSEARCH_USEDELAY, 0 );
                    return lResult;
                }
                else
                {
                    LRESULT lResult = callWndProc(prev_editWndProc,
                                        hEdit, uMsg, wParam, lParam);
                    SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
                    return lResult;
                }
            }
            break;
        }
        case WM_SETFOCUS:
        {
            qs_bEditIsActive = TRUE;
            InvalidateRect(hEdit, NULL, TRUE);
            UpdateWindow(hEdit);
            if ( qs_bEditCanBeNonActive )
            {
                if ( g_Options.dwFlags[OPTF_EDIT_FOCUS_SELECTALL] )
                {
                    SendMessage(hEdit, EM_SETSEL, 0, -1);
#ifdef _DEBUG
                    Debug_Output("editWndProc, WM_SETFOCUS, if (SelectAll), SETSEL(0, -1)\n");
#endif
                }
                else
                {
                    if ( !qs_bEditSelJustChanged )
                    {
                        SendMessage(hEdit, EM_SETSEL, dwSelPos1, dwSelPos2);
#ifdef _DEBUG
                        Debug_Output("editWndProc, WM_SETFOCUS, if (!SelJustChanged), SETSEL(%d, %d)\n", dwSelPos1, dwSelPos2);
#endif
                    }
                    else
                    {
                        qs_bEditSelJustChanged = FALSE;
                        SendMessage( hEdit, EM_GETSEL,
                          (WPARAM) &dwSelPos1, (LPARAM) &dwSelPos2 );
#ifdef _DEBUG
                        Debug_Output("editWndProc, WM_GETFOCUS, if (SelJustChanged), GETSEL(%d, %d)\n", dwSelPos1, dwSelPos2);
#endif
                    }
                }
            }
            g_QSearchDlg.bMouseJustLeavedFindEdit = FALSE;
//#ifdef _DEBUG
//            Debug_Output("QSearchDlg.c, editWndProc, WM_SETFOCUS, g_QSearchDlg.MouseJustLeavedFindEdit = FALSE;\n");
//#endif
            break;
        }
        case WM_KILLFOCUS:
        {
            if ( qs_bEditCanBeNonActive )
            {
                HWND hWndToFocus;

                qs_bEditIsActive = FALSE;
                if ( !g_Options.dwFlags[OPTF_EDIT_FOCUS_SELECTALL] )
                {
                    SendMessage( hEdit, EM_GETSEL,
                      (WPARAM) &dwSelPos1, (LPARAM) &dwSelPos2 );
#ifdef _DEBUG
                    Debug_Output("editWndProc, WM_KILLFOCUS, GETSEL(%d, %d)\n", dwSelPos1, dwSelPos2);
#endif
                }

                hWndToFocus = (HWND) wParam;
                if ( (hWndToFocus != g_QSearchDlg.hBtnFindNext) &&
                     (hWndToFocus != g_QSearchDlg.hBtnFindPrev) &&
                     (hWndToFocus != g_QSearchDlg.hBtnFindAll) )
                {
                    qs_nEditEOF = 0;
                    SendMessage( g_QSearchDlg.hDlg, QSM_SETNOTFOUND, FALSE, 0 );
                    g_QSearchDlg.bMouseJustLeavedFindEdit = FALSE;
//#ifdef _DEBUG
//                    Debug_Output("QSearchDlg.c, editWndProc, WM_KILLFOCUS, g_QSearchDlg.MouseJustLeavedFindEdit = FALSE;\n");
//#endif
                }
                /*
                InvalidateRect(hEdit, NULL, TRUE);
                UpdateWindow(hEdit);
                */
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            if ( (!qs_bEditIsActive) && (!bEditTrackingMouse) )
            {
                if ( (g_Options.dwFlags[OPTF_QSEARCH_AUTOFOCUS_MOUSE] & 0x01) ||
                     (g_Options.dwFlags[OPTF_EDITOR_AUTOFOCUS_MOUSE] & 0x01) )
                {
                    HWND hFocusedWnd = GetFocus();
                    // When AkelPad's window is not active, GetFocus() returns NULL
                    if ( hFocusedWnd && hFocusedWnd != hEdit )
                    {
                        SetFocus(hEdit);
                    }
                }
            }
            if ( !bEditTrackingMouse )
            {
                TRACKMOUSEEVENT tme;

                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hEdit;
                tme.dwHoverTime = HOVER_DEFAULT;
                if ( _TrackMouseEvent(&tme) )
                {
                    bEditTrackingMouse = TRUE;
                }

                g_QSearchDlg.bMouseJustLeavedFindEdit = FALSE;
//#ifdef _DEBUG
//                Debug_Output("QSearchDlg.c, editWndProc, WM_MOUSEMOVE, g_QSearchDlg.MouseJustLeavedFindEdit = FALSE;\n");
//#endif
            }
            break;
        }
        case WM_MOUSELEAVE:
        {
            bEditTrackingMouse = FALSE;
            g_QSearchDlg.bMouseJustLeavedFindEdit = TRUE;
//#ifdef _DEBUG
//            Debug_Output("QSearchDlg.c, editWndProc, WM_MOUSELEAVE, g_QSearchDlg.MouseJustLeavedFindEdit = TRUE;\n");
//#endif
            break;
        }
        case WM_GETDLGCODE:
        {
            if ( !g_Options.dwFlags[OPTF_EDIT_FOCUS_SELECTALL] )
            {
                LRESULT lResult = callWndProc(prev_editWndProc, hEdit, uMsg, wParam, lParam);
                if ( lResult & DLGC_HASSETSEL )
                {
                    lResult -= DLGC_HASSETSEL;
#ifdef _DEBUG
                    {
                        DWORD pos1, pos2;
                        SendMessage(hEdit, EM_GETSEL, (WPARAM)&pos1, (LPARAM)&pos2);
                        Debug_Output("editWndProc, WM_GETDLGCODE, GETSEL(%d, %d)\n", pos1, pos2);
                    }
#endif
                }
                return lResult;
            }
            break;
        }
        case WM_NOTIFY:
        {
            if ( g_Plugin.bOldWindows )
            {
                if ( ((LPNMHDR) lParam)->code == TTN_GETDISPINFOA )
                {
                    LPNMTTDISPINFOA lpnmdiA = (LPNMTTDISPINFOA) lParam;
                    SendMessageA(lpnmdiA->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
                    lpnmdiA->lpszText = (LPSTR) qsearchGetHintA(IDC_ED_FINDTEXT);
                    return 0;
                }
            }
            else
            {
                if ( ((LPNMHDR) lParam)->code == TTN_GETDISPINFOW )
                {
                    LPNMTTDISPINFOW lpnmdiW = (LPNMTTDISPINFOW) lParam;
                    SendMessageW(lpnmdiW->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
                    lpnmdiW->lpszText = (LPWSTR) qsearchGetHintW(IDC_ED_FINDTEXT);
                    return 0;
                }
            }
            break;
        }
        default:
            break;
    }

    return callWndProc(prev_editWndProc, hEdit, uMsg, wParam, lParam);
}

static BOOL isHighlightMainActive(void)
{
    if ( g_Plugin.bOldWindows )
    {
        PLUGINFUNCTION *pfA = (PLUGINFUNCTION *) SendMessageA( g_Plugin.hMainWnd,
            AKD_DLLFINDA, (WPARAM) cszHighlightMainA, 0 );

        if ( pfA && pfA->bRunning )
        {
            return TRUE;
        }
    }
    else
    {
        PLUGINFUNCTION *pfW = (PLUGINFUNCTION *) SendMessageW( g_Plugin.hMainWnd,
            AKD_DLLFINDW, (WPARAM) cszHighlightMainW, 0 );

        if ( pfW && pfW->bRunning )
        {
            return TRUE;
        }
    }

    return FALSE;
}

static void qsUpdateHighlight(HWND hDlg, HWND hEdit, const DWORD dwOptFlags[])
{
    wchar_t szSelectedTextW[MAX_TEXT_SIZE];

    getEditFindText(hEdit, g_QSearchDlg.szFindTextW);

    if ( getAkelPadSelectedText(szSelectedTextW, dwOptFlags) )
    {
        BOOL bEqual = FALSE;

        if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
        {
            bEqual = TRUE;
        }
        else
        {
            wchar_t szText1[MAX_TEXT_SIZE];
            wchar_t szText2[MAX_TEXT_SIZE];
            const wchar_t* psz1;
            const wchar_t* psz2;

            if ( isCheckBoxChecked(hDlg, IDC_CH_MATCHCASE) )
            {
                // match case
                psz1 = g_QSearchDlg.szFindTextW;
                psz2 = szSelectedTextW;
            }
            else
            {
                // case-insensitive
                if ( g_Plugin.bOldWindows )
                {
                    lstrcpyA( (LPSTR) szText1, (LPCSTR) g_QSearchDlg.szFindTextW );
                    CharUpperA( (LPSTR) szText1 );
                    lstrcpyA( (LPSTR) szText2, (LPCSTR) szSelectedTextW );
                    CharUpperA( (LPSTR) szText2 );
                }
                else
                {
                    lstrcpyW( szText1, g_QSearchDlg.szFindTextW );
                    CharUpperW( szText1 );
                    lstrcpyW( szText2, szSelectedTextW );
                    CharUpperW( szText2 );
                }
                psz1 = szText1;
                psz2 = szText2;
            }

            if ( dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] )
            {
                if ( g_Plugin.bOldWindows )
                    bEqual = (match_mask((LPCSTR)psz1, (LPCSTR)psz2, 0, 0) > 0);
                else
                    bEqual = (match_maskw(psz1, psz2, 0, 0) > 0);
            }
            else
            {
                if ( g_Plugin.bOldWindows )
                    bEqual = (lstrcmpA((LPCSTR)psz1, (LPCSTR)psz2) == 0);
                else
                    bEqual = (lstrcmpW(psz1, psz2) == 0);
            }
        }

        qs_bEditTextChanged = FALSE;

        if ( !bEqual )
        {
            DWORD dwOptFlagsTemp[OPTF_COUNT];
            copyOptionsFlags(dwOptFlagsTemp, dwOptFlags);
            dwOptFlagsTemp[OPTF_SRCH_STOP_EOF] = 0; // disabling here
            qsearchDoSearchText( hEdit, QSEARCH_NEXT, dwOptFlagsTemp, NULL );
        }

        qsearchDoTryHighlightAll(hDlg, dwOptFlags, QHC_CHECKBOX_CHECKED);
    }
    else
    {
        // no text selected - trying to find & highlight
        DWORD dwOptFlagsTemp[OPTF_COUNT];
        copyOptionsFlags(dwOptFlagsTemp, dwOptFlags);
        dwOptFlagsTemp[OPTF_SRCH_STOP_EOF] = 0; // disabling here
        qsearchDoSearchText( hEdit, QSEARCH_NEXT, dwOptFlagsTemp, NULL );

        qsearchDoTryHighlightAll(hDlg, dwOptFlags, QHC_CHECKBOX_CHECKED);
    }
}

INT_PTR qsearchDlgOnAltHotkey(HWND hDlg, WPARAM wParam)
{
    if ( g_Options.dwUseAltHotkeys )
    {
        if ( wParam == g_Options.dwAltMatchCase )
        {
            qsChangeCkeckBoxState(IDC_CH_MATCHCASE);
            return 1;
        }
        if ( wParam == g_Options.dwAltHighlightAll )
        {
            qsChangeCkeckBoxState(IDC_CH_HIGHLIGHTALL);
            return 1;
        }
        if ( wParam == g_Options.dwAltWholeWord )
        {
            qsChangeCkeckBoxState(IDC_CH_WHOLEWORD);
            return 1;
        }
        if ( wParam == g_Options.dwAltSearchMode )
        {
            OnChWholeWordSrchMode();
            return 1;
        }
    }
    return 0;
}

static void OnChMatchCaseOrWholeWordClicked(HWND hDlg)
{
    UINT nPickedUp = 0;
    BOOL bHighlightAllChecked = isCheckBoxChecked(hDlg, IDC_CH_HIGHLIGHTALL);

    if ( bHighlightAllChecked &&
        g_QSearchDlg.uSearchOrigin == QS_SO_EDITOR &&
        g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] &&
        g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] == PICKUP_SEL_ALWAYS )
    {
        qs_bForceFindFirst = TRUE;
        nPickedUp = qsPickUpSelection(g_QSearchDlg.hFindEdit, g_Options.dwFlags, FALSE) & QS_PSF_PICKEDUP;
    }

    qsSetInfoEmpty();

    if ( !nPickedUp )
    {
        qs_bForceFindFirst = TRUE;
        qs_bEditTextChanged = TRUE;
        qsearchDoSetNotFound(g_QSearchDlg.hFindEdit, FALSE, FALSE, FALSE);

        if ( bHighlightAllChecked )
        {
            DWORD dwOptFlagsTemp[OPTF_COUNT];

            getEditFindText(g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW);

            copyOptionsFlags(dwOptFlagsTemp, g_Options.dwFlags);
            dwOptFlagsTemp[OPTF_SRCH_ONTHEFLY_MODE] = 1;
            dwOptFlagsTemp[OPTF_SRCH_STOP_EOF] = 0;
            qsearchDoSearchText( g_QSearchDlg.hFindEdit, QSEARCH_FIRST, dwOptFlagsTemp, NULL );

            qsearchDoTryHighlightAll(hDlg, g_Options.dwFlags, QHC_CHECKBOX_CHECKED);
        }
    }
    else
    {
        qs_bEditTextChanged = TRUE;
        qsearchDoSetNotFound(g_QSearchDlg.hFindEdit, FALSE, FALSE, FALSE);
    }
}

static void fillToolInfoA(
    TOOLINFOA* lptiA,
    LPSTR      lpToolTipText,
    HWND       hWnd,
    UINT       uId)
{
    RECT rect = { 0, 0, 0, 0 };

    GetClientRect(hWnd, &rect);

    lptiA->cbSize = sizeof(TOOLINFOA);
    lptiA->uFlags = TTF_SUBCLASS;
    lptiA->hwnd = hWnd;
    lptiA->hinst = g_Plugin.hInstanceDLL;
    lptiA->uId = uId;
    lptiA->lpszText = lpToolTipText;
    // ToolTip control will cover the whole window
    lptiA->rect.left = rect.left;
    lptiA->rect.top = rect.top;
    lptiA->rect.right = rect.right;
    lptiA->rect.bottom = rect.bottom;
    lptiA->lParam = 0;
}

static void fillToolInfoW(
    TOOLINFOW* lptiW,
    LPWSTR     lpToolTipText,
    HWND       hWnd,
    UINT       uId)
{
    RECT rect = { 0, 0, 0, 0 };

    GetClientRect(hWnd, &rect);

    lptiW->cbSize = sizeof(TOOLINFOW);
    lptiW->uFlags = TTF_SUBCLASS;
    lptiW->hwnd = hWnd;
    lptiW->hinst = g_Plugin.hInstanceDLL;
    lptiW->uId = uId;
    lptiW->lpszText = lpToolTipText;
    // ToolTip control will cover the whole window
    lptiW->rect.left = rect.left;
    lptiW->rect.top = rect.top;
    lptiW->rect.right = rect.right;
    lptiW->rect.bottom = rect.bottom;
    lptiW->lParam = 0;
}

static HWND GetDlgItemAndRect(HWND hDlg, int nItemId, RECT* pRect)
{
    HWND hDlgItm;
    
    hDlgItm = GetDlgItem(hDlg, nItemId);
    if ( hDlgItm )
    {
        GetWindowRect( hDlgItm, pRect );
        ScreenToClient( hDlg, (POINT *) &pRect->left );
        ScreenToClient( hDlg, (POINT *) &pRect->right );
    }

    return hDlgItm;
}

static void MoveWindowByDx(HWND hWnd, const RECT* rc0, LONG dx)
{
    if ( hWnd )
    {
        MoveWindow(hWnd, rc0->left + dx, rc0->top, rc0->right - rc0->left, rc0->bottom - rc0->top, TRUE);
    }
}

static void ResizeWindowByDx(HWND hWnd, const RECT* rc0, LONG dx)
{
    if ( hWnd )
    {
        MoveWindow(hWnd, rc0->left, rc0->top, rc0->right - rc0->left + dx, rc0->bottom - rc0->top, TRUE);
    }
}

static void RedrawWindowByDx(HWND hWnd)
{
    if ( hWnd )
    {
        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
    }
}

INT_PTR CALLBACK qsearchDlgProc(HWND hDlg,
  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND   hToolTip = NULL;
    static HMENU  hPopupMenuLoaded = NULL;
    static HBRUSH hTextNotFoundBrush = NULL;
    static HBRUSH hTextNotRegExpBrush = NULL;
    static HBRUSH hTextEOFBrush = NULL;
    //static BOOL   bHotKeyPressed = FALSE;

    static HWND hEdFindText = NULL;
    static HWND hCbFindText = NULL;
    static HWND hBtFindNext = NULL, hBtFindPrev = NULL, hBtFindAll = NULL;
    static HWND hBtCancel = NULL;
    static HWND hPbProgress = NULL;
    static HWND hChMatchCase = NULL, hChWholeWord = NULL, hChHighlightAll = NULL;
    static HWND hStInfo = NULL;

    static RECT rcDlg_0 = { 0, 0, 0, 0 };
    static RECT rcStInfo_0 = { 0, 0, 0, 0 };
    static RECT rcChHighlightAll_0 = { 0, 0, 0, 0 };
    static RECT rcChWholeWord_0 = { 0, 0, 0, 0 };
    static RECT rcChMatchCase_0 = { 0, 0, 0, 0 };
    static RECT rcPbProgress_0 = { 0, 0, 0, 0 };
    static RECT rcBtFindAll_0 = { 0, 0, 0, 0 };
    static RECT rcBtFindPrev_0 = { 0, 0, 0, 0 };
    static RECT rcBtFindNext_0 = { 0, 0, 0, 0 };
    static RECT rcCbFindText_0 = { 0, 0, 0, 0 };
    static RECT rcEdFindText_0 = { 0, 0, 0, 0 };

#ifdef _DEBUG
    Debug_Output("Dlg Msg 0x%X:  0x0%X  0x0%X\n", uMsg, wParam, lParam);
#endif

    switch ( uMsg )
    {
        case WM_COMMAND:
        {
            unsigned int id = LOWORD(wParam);
            if ( id == IDOK || id == IDOK_FINDPREV )
            {
                // Originally we get here when Enter is pressed. Why? Ask M$ ;)
                unsigned int uSearch;

                if ( id == IDOK )
                {
                    if ( GetKeyState(VK_QS_PICKUPTEXT) & 0x80 )
                    {
                        OnEditKeyDown_Enter_or_F3(g_QSearchDlg.hFindEdit, VK_RETURN, g_Options.dwFlags);
                        return 1;
                    }
                }

                uSearch = QSEARCH_NEXT;
                if ( qs_bEditTextChanged &&
                     (qs_bForceFindFirst ||
                      g_Options.dwFlags[OPTF_SRCH_FROM_BEGINNING] ||
                      !g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE]) )
                {
                    uSearch = QSEARCH_FIRST;
                    qs_bForceFindFirst = FALSE;
                }
                if ( id == IDOK_FINDPREV )
                {
                    uSearch |= QSEARCH_FINDUP;
                }
                qs_bEditTextChanged = FALSE;
                /*if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
                {
                    getEditFindText( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW );
                    qsearchFindHistoryAdd( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW, 0 );
                    qsPickUpSelection( g_QSearchDlg.hFindEdit, g_Options.dwFlags, FALSE );
                }*/
                getEditFindText( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW );
                qsearchDoSearchText( g_QSearchDlg.hFindEdit, uSearch, g_Options.dwFlags, NULL );
                if ( uSearch & QSEARCH_FIRST )
                {
                    qsearchDoTryHighlightAll( hDlg, g_Options.dwFlags, QHC_CHECKBOX_CHECKED );
                }
                return 1;
            }
            else if ( id == IDCANCEL )
            {
                // We get here when Esc is pressed. Why? Ask M$ ;)
                if ( g_Options.dwFlags[OPTF_CATCH_MAIN_ESC] == ESC_THROW_TO_MAIN )
                {
                    PostMessage( g_Plugin.hMainWnd, WM_COMMAND, 0x10000 | IDM_FILE_EXIT, 0 );
                    return 0;
                }

                //qsearchDoQuit( g_QSearchDlg.hFindEdit, hToolTip, hPopupMenuLoaded, hTextNotFoundBrush, hTextNotRegExpBrush, hTextEOFBrush );
                qsearchDoShowHide(hDlg, FALSE, 0, g_Options.dwFlags);
                return 1;
            }
            else if ( id == IDC_BT_CANCEL )
            {
                //qsearchDoQuit( g_QSearchDlg.hFindEdit, hToolTip, hPopupMenuLoaded, hTextNotFoundBrush, hTextNotRegExpBrush, hTextEOFBrush );
                qsearchDoShowHide(hDlg, FALSE, 0, g_Options.dwFlags);
                return 1;
            }
            else if ( id == IDC_BT_FINDNEXT )
            {
                LPARAM uFindFlags = 0;
                if (g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] == PICKUP_SEL_IF_NOT_QSBTN)
                    uFindFlags |= QS_FF_NOPICKUPSEL;
                PostMessage( hDlg, QSM_FINDNEXT, FALSE, uFindFlags );
                //PostMessage( hDlg, WM_COMMAND, IDOK, 0 );
                return 1;
            }
            else if ( id == IDC_BT_FINDPREV )
            {
                LPARAM uFindFlags = 0;
                if (g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] == PICKUP_SEL_IF_NOT_QSBTN)
                    uFindFlags |= QS_FF_NOPICKUPSEL;
                PostMessage( hDlg, QSM_FINDNEXT, TRUE, uFindFlags );
                //PostMessage( hDlg, WM_COMMAND, IDOK_FINDPREV, 0 );
                return 1;
            }
            else if ( id == IDC_BT_FINDALL )
            {
                DWORD dwAdditionalFlags = 0;
                if ( (GetKeyState(VK_MENU) & 0x80) ||
                     (GetKeyState(VK_CONTROL) & 0x80) ||
                     (GetKeyState(VK_SHIFT) & 0x80) )
                {
                    dwAdditionalFlags = QS_FINDALL_RSLT_ALLFILES;
                }
                PostMessage( hDlg, QSM_FINDALL, g_Options.dwFindAllMode | dwAdditionalFlags, 0 );
                return 1;
            }
            else if ( id == IDC_CH_MATCHCASE )
            {
                if ( HIWORD(wParam) == BN_CLICKED )
                {
                    OnChMatchCaseOrWholeWordClicked(hDlg);
                }
            }
            else if ( id == IDC_CH_WHOLEWORD )
            {
                if ( GetKeyState(VK_QS_WW_SRCH_MODE) & 0x80 )
                {
                }
                else if ( HIWORD(wParam) == BN_CLICKED )
                {
                    OnChMatchCaseOrWholeWordClicked(hDlg);
                }
            }
            else if ( id == IDC_CH_HIGHLIGHTALL )
            {
                if ( HIWORD(wParam) == BN_CLICKED )
                {
                    UINT nPickedUp = 0;
                    BOOL bHighlightAllChecked = isCheckBoxChecked(hDlg, IDC_CH_HIGHLIGHTALL);

                    if ( bHighlightAllChecked &&
                         g_QSearchDlg.uSearchOrigin == QS_SO_EDITOR &&
                         g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] &&
                         g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] == PICKUP_SEL_ALWAYS )
                    {
                        g_Options.dwHighlightState |= HLS_IS_CHECKED;
                        nPickedUp = qsPickUpSelection(g_QSearchDlg.hFindEdit, g_Options.dwFlags, TRUE) & QS_PSF_PICKEDUP;
                    }

                    qsSetInfoEmpty();

                    if ( !nPickedUp )
                    {
                        if ( bHighlightAllChecked )
                        {
                            g_Options.dwHighlightState |= HLS_IS_CHECKED;
                            qsUpdateHighlight(hDlg, g_QSearchDlg.hFindEdit, g_Options.dwFlags);
                        }
                        else
                        {
                            if ( (g_Options.dwHighlightState & HLS_IS_CHECKED) == HLS_IS_CHECKED )
                                g_Options.dwHighlightState -= HLS_IS_CHECKED;
                            qsearchDoTryUnhighlightAll();
                        }
                    }

                    if ( bHighlightAllChecked &&
                         (g_Options.dwNewUI == QS_UI_NEW_02) &&
                         (g_Options.dwFindAllMode & QS_FINDALL_AUTO_COUNT_FLAG) )
                    {
                        PostMessage( g_QSearchDlg.hDlg, QSM_FINDALL, QS_FINDALL_COUNTONLY, 0 );
                    }
                }
            }
            else if ( id == IDC_CB_FINDTEXT )
            {
                switch ( HIWORD(wParam) )
                {
                    case CBN_SELENDOK:
                    case CBN_SELCHANGE:
                        qs_bEditTextChanged = TRUE;
                        qs_bForceFindFirst = TRUE;
                        g_QSearchDlg.uSearchOrigin = qs_Get_SO_QSEARCH(g_Options.dwFlags);
                        qsearchDoSetNotFound(g_QSearchDlg.hFindEdit, FALSE, FALSE, FALSE);
                        qsSetInfoEmpty();
                        break;
                    case CBN_DROPDOWN:
                        getEditFindText(g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW);
                        qsearchFindHistoryAdd(g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW, 0);
                        /*{
                            HWND hCombo;
                            int iItem;

                            hCombo = GetDlgItem(g_QSearchDlg.hDlg, IDC_CB_FINDTEXT);
                            if ( g_Plugin.bOldWindows )
                                iItem = (int) SendMessageA(hCombo, CB_FINDSTRING, (WPARAM) (0), (LPARAM) g_QSearchDlg.szFindTextW);
                            else
                                iItem = (int) SendMessageW(hCombo, CB_FINDSTRING, (WPARAM) (0), (LPARAM) g_QSearchDlg.szFindTextW);

                            if ( iItem != CB_ERR )
                                SendMessage(hCombo, CB_SETCURSEL, iItem, 0);
                        }*/
                        break;
                }
            }
            else if ( (id >= IDM_START) && (id < IDM_START + OPTF_COUNT - 1) )
            {
                if ( g_QSearchDlg.hPopupMenu )
                {
                    unsigned int state = GetMenuState(g_QSearchDlg.hPopupMenu, id, MF_BYCOMMAND);
                    switch ( id - IDM_START )
                    {
                        case OPTF_SRCH_PICKUP_SELECTION:
                        case OPTF_SRCH_STOP_EOF:
                        case OPTF_CATCH_MAIN_ESC:
                        case OPTF_HOTKEY_HIDES_PANEL:
                        case OPTF_EDITOR_AUTOFOCUS_MOUSE:
                            if ( (state & MF_CHECKED) == MF_CHECKED )
                            {
                                if ( g_Options.dwFlags[id - IDM_START] & 0x01 )
                                {
                                    g_Options.dwFlags[id - IDM_START] -= 0x01;

                                    if ( id == IDM_START + OPTF_SRCH_STOP_EOF )
                                        qsearchDoSetNotFound(g_QSearchDlg.hFindEdit, FALSE, FALSE, FALSE);
                                }
                            }
                            else
                            {
                                g_Options.dwFlags[id - IDM_START] |= 0x01;
                            }
                            break;

                        default:
                            g_Options.dwFlags[id - IDM_START] =
                                ( (state & MF_CHECKED) == MF_CHECKED ) ? 0 : 1;
                            break;
                    }
                    if ( id == IDM_SRCHONTHEFLYMODE )
                    {
                        /*if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] )
                        {
                            CheckMenuItem( hPopupMenu, IDM_SRCHUSESPECIALCHARS,
                              MF_BYCOMMAND | MF_UNCHECKED );
                            g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 0;
                        }*/
                        qsdlgShowHideWholeWordCheckBox(hDlg, g_Options.dwFlags);
                        qs_nEditEOF = 0;
                        qsearchDoSetNotFound( g_QSearchDlg.hFindEdit, FALSE, FALSE, FALSE );
                    }
                    else if ( id == IDM_SRCHUSESPECIALCHARS )
                    {
                        if ( g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] )
                        {
                            CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHUSEREGEXP,
                              MF_BYCOMMAND | MF_UNCHECKED );
                            EnableMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE,
                              MF_BYCOMMAND | MF_GRAYED );
                            g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] = 0;
                        }
                        OnSrchModeChanged();
                    }
                    else if ( id == IDM_SRCHUSEREGEXP )
                    {
                        if ( g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] )
                        {
                            CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHUSESPECIALCHARS,
                              MF_BYCOMMAND | MF_UNCHECKED );
                            EnableMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE,
                              MF_BYCOMMAND | MF_ENABLED );
                            g_Options.dwFlags[OPTF_SRCH_USE_SPECIALCHARS] = 0;
                        }
                        OnSrchModeChanged();
                    }
                    else if ( id == IDM_SRCHPICKUPSELECTION )
                    {
                        if ( !(g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01) )
                        {
                            g_QSearchDlg.uSearchOrigin = QS_SO_UNKNOWN;
                        }
                    }
                    else if ( id == IDM_SRCHWNDDOCKEDTOP )
                    {
                        if ( g_QSearchDlg.pDockData )
                        {
                            state = DK_SETBOTTOM;
                            if ( g_Options.dwFlags[OPTF_SRCH_WND_DOCKEDTOP] )
                            {
                                state = DK_SETTOP;
                            }
                            SendMessage( g_Plugin.hMainWnd, AKD_DOCK,
                              state, (LPARAM) g_QSearchDlg.pDockData );
                            PostMessage( g_Plugin.hMainWnd, AKD_RESIZE, 0, 0 );
                        }
                    }
                }
            }
            else if ( id == IDM_FINDALL_AUTO_COUNT )
            {
                if ( g_QSearchDlg.hFindAllPopupMenu )
                {
                    unsigned int state = GetMenuState(g_QSearchDlg.hFindAllPopupMenu, id, MF_BYCOMMAND);
                    if ( (state & MF_CHECKED) == MF_CHECKED )
                    {
                        if ( g_Options.dwFindAllMode & QS_FINDALL_AUTO_COUNT_FLAG )
                            g_Options.dwFindAllMode -= QS_FINDALL_AUTO_COUNT_FLAG;
                    }
                    else
                        g_Options.dwFindAllMode |= QS_FINDALL_AUTO_COUNT_FLAG;
                }
            }
            else if ( (id >= IDM_FINDALL_START + QS_FINDALL_COUNTONLY) && (id < IDM_FINDALL_START + QS_FINDALL_TOTAL_TYPES) )
            {
                DWORD dwFlags = 0;
                if ( g_Options.dwFindAllMode & QS_FINDALL_AUTO_COUNT_FLAG )
                    dwFlags |= QS_FINDALL_AUTO_COUNT_FLAG;
                g_Options.dwFindAllMode = id - IDM_FINDALL_START;
                g_Options.dwFindAllMode |= dwFlags;
            }
            else if ( id == IDM_FINDALL_SHOWLINE )
            {
                g_Options.dwFindAllResult |= QS_FINDALL_RSLT_WHOLELINE;
                if ( g_Options.dwFindAllResult & QS_FINDALL_RSLT_MATCHONLY )
                    g_Options.dwFindAllResult -= QS_FINDALL_RSLT_MATCHONLY;
            }
            else if ( id == IDM_FINDALL_SHOWMATCHONLY )
            {
                g_Options.dwFindAllResult |= QS_FINDALL_RSLT_MATCHONLY;
                if ( g_Options.dwFindAllResult & QS_FINDALL_RSLT_WHOLELINE )
                    g_Options.dwFindAllResult -= QS_FINDALL_RSLT_WHOLELINE;
            }
            else if ( id == IDM_FINDALL_FILTERMODE )
            {
                if ( g_Options.dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE )
                    g_Options.dwFindAllResult -= QS_FINDALL_RSLT_FILTERMODE;
                else
                    g_Options.dwFindAllResult |= QS_FINDALL_RSLT_FILTERMODE;
            }
            else if ( id == IDM_FINDALL_SETTINGSDLG )
            {
                if ( g_Plugin.bOldWindows )
                {
                    /*
                    DialogBoxA(g_Plugin.hInstanceDLL,
                               MAKEINTRESOURCEA(IDD_FINDALL_SETTINGS),
                               g_Plugin.hMainWnd,
                               QSFndAllSettDlgProc);
                    */
                }
                else
                {
                    DialogBoxW(g_Plugin.hInstanceDLL,
                               MAKEINTRESOURCEW(IDD_FINDALL_SETTINGS),
                               g_Plugin.hMainWnd,
                               QSFndAllSettDlgProc);
                }
            }
            break;
        }
        case WM_KEYDOWN:
        {
            if ( wParam == VK_TAB )
            {
                if ( GetKeyState(VK_CONTROL) & 0x80 ) // Ctrl+Tab, Ctrl+Shift+Tab, ...
                {
                    EDITINFO ei;

                    ei.hWndEdit = 0;
                    SendMessage( g_Plugin.hMainWnd, AKD_GETEDITINFO, 0, (LPARAM) &ei );
                    if ( ei.hWndEdit )
                    {
                        if ( g_Plugin.bOldWindows )
                            PostMessageA(ei.hWndEdit, WM_KEYDOWN, wParam, lParam);
                        else
                            PostMessageW(ei.hWndEdit, WM_KEYDOWN, wParam, lParam);
                        return 1;
                    }
                }
            }
            break;
        }
        //case WM_KEYDOWN:
        //{
        //    bHotKeyPressed = FALSE;
        //    if ( LOBYTE(wParam) == LOBYTE(qs_dwHotKey) )
        //    {
        //        bHotKeyPressed = isQSearchHotKeyPressed();
        //        return 0;
        //    }
        //    break;
        //}
        case WM_KEYUP:
        {
            if ( wParam == VK_TAB )
            {
                if ( GetKeyState(VK_CONTROL) & 0x80 ) // Ctrl+Tab, Ctrl+Shift+Tab, ...
                {
                    return 1;
                }
            }
            break;
        }
        //case WM_KEYUP:
        //{
        //    if ( bHotKeyPressed )
        //    {
        //        bHotKeyPressed = FALSE;
        //        if ( !qs_bHotKeyPressedOnShow )
        //        {
        //            if ( g_QSearchDlg.hFindEdit )
        //            {
        //                SetFocus(g_QSearchDlg.hFindEdit);
        //            }
        //            return 0;
        //        }
        //    }
        //
        //    qs_bHotKeyPressedOnShow = FALSE;
        //    break;
        //}
        case WM_SYSKEYDOWN:
        {
            //qsearchDlgOnAltHotkey(hDlg, wParam);
            break;
        }
        //case WM_SYSKEYUP:
        //{
        //    if ( bHotKeyPressed )
        //    {
        //        SendMessage(hDlg, WM_KEYUP, wParam, lParam);
        //        return 0;
        //    }
        //    break;
        //}
        case WM_SYSCOMMAND:
        {
            if ( wParam == SC_KEYMENU )
            {
                if ( lParam == 0x0D )
                {
                    // this is {Enter} in {Alt+Enter}
                    return 1; // disabling the annoying "ding" sound
                }
            }
            break;
        }
        case WM_SETFOCUS:
        {
            EnableWindow(
              GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL),
              g_bHighlightPlugin
            );

            if ( !g_QSearchDlg.hFindEdit )
            {
                g_QSearchDlg.hFindEdit = qsearchGetFindEdit(hDlg);
            }
            if ( g_Plugin.nDockedDlgIsWaitingForOnStart == 0 )
            {
                qs_bEditIsActive = TRUE;
                SetFocus(g_QSearchDlg.hFindEdit);
            }
            else
            {
                qs_bEditIsActive = FALSE;
                SetFocus(g_Plugin.hMainWnd);
            }
            break;
        }
        case WM_CTLCOLOREDIT:
        {
            if ( g_QSearchDlg.hFindEdit == (HWND) lParam )
            {
                int nBkColor = COLOR_BTNFACE; // normal inactive
                if ( qs_bEditIsActive || qs_bBtnFindIsFocused )
                {
                    if ( !qs_bEditNotFound )
                    {
                        if ( (!qs_bEditIsEOF) || (g_Options.colorEOF == RGB(0xFF,0xFF,0xFF)) )
                            nBkColor = COLOR_WINDOW; // normal active
                        else
                            nBkColor = 0x7FFE; // EOF active
                    }
                    else
                    {
                        if ( !qs_bEditNotRegExp )
                            nBkColor = 0x7FFF; // not found active
                        else
                            nBkColor = 0x7FFD; // not regexp
                    }
                }
                SetTextColor( (HDC) wParam, GetSysColor(COLOR_WINDOWTEXT) );
                SetBkMode( (HDC) wParam, TRANSPARENT );
                switch ( nBkColor )
                {
                    case 0x7FFF:
                        SetBkColor( (HDC) wParam, g_Options.colorNotFound );
                        return (LRESULT) hTextNotFoundBrush;

                    case 0x7FFE:
                        SetBkColor( (HDC) wParam, g_Options.colorEOF );
                        return (LRESULT) hTextEOFBrush;

                    case 0x7FFD:
                        SetBkColor( (HDC) wParam, g_Options.colorNotRegExp );
                        return (LRESULT) hTextNotRegExpBrush;

                    default:
                        SetBkColor( (HDC) wParam, GetSysColor(nBkColor) );
                        return (LRESULT) GetSysColorBrush(nBkColor);
                }
            }
            break;
        }
        case WM_SHOWWINDOW:
        {
            if ( wParam )
            {
                HWND hChHighlightAll;

                g_bHighlightPlugin = isHighlightMainActive();

                hChHighlightAll = GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL);
                EnableWindow( hChHighlightAll, g_bHighlightPlugin );
                SendMessage( hChHighlightAll, BM_SETCHECK,
                  (((g_Options.dwHighlightState & HLS_IS_CHECKED) == HLS_IS_CHECKED) ?
                       BST_CHECKED : BST_UNCHECKED), 0 );

                qs_dwHotKey = getQSearchHotKey();
                qs_bHotKeyPressedOnShow = isQSearchHotKeyPressed();

                if ( g_QSearchDlg.uWmShowFlags & QS_SF_CANPICKUPSELTEXT )
                {
                    if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
                    {
                        if ( getAkelPadSelectedText(g_QSearchDlg.szFindTextW, g_Options.dwFlags) )
                        {
                            if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] )
                            {
                                qs_bForceFindFirst = FALSE;
                                qsearchDoTryHighlightAll(hDlg, g_Options.dwFlags, QHC_CHECKBOX_CHECKED);
                            }
                            else
                                qs_bForceFindFirst = TRUE;
                        }
                    }
                }

                if ( g_Plugin.nDockedDlgIsWaitingForOnStart == 0 )
                {
                    qs_bEditIsActive = TRUE;
                    SetFocus(g_QSearchDlg.hFindEdit);
                }
                else
                {
                    qs_bEditIsActive = FALSE;
                    SetFocus(g_Plugin.hMainWnd);
                }
            }
            break;
        }
        case WM_CONTEXTMENU:
        {
            HMENU hPopMnu;
            UINT  i;
            UINT  uCheck;
            POINT pt;

            // the cursor position can be negative (!) in case of 2nd monitor
            // pt.x = (int) (short) LOWORD(lParam);
            // pt.y = (int) (short) HIWORD(lParam);
            GetCursorPos(&pt);

            if ( (g_QSearchDlg.hBtnFindAll == (HWND) wParam) && !g_Plugin.bOldWindows )
            {
                uCheck = (g_Options.dwFindAllMode & QS_FINDALL_AUTO_COUNT_FLAG) ? MF_CHECKED : MF_UNCHECKED;
                CheckMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_START, MF_BYCOMMAND | uCheck );

                for ( i = QS_FINDALL_COUNTONLY; i < QS_FINDALL_TOTAL_TYPES; i++ )
                {
                    uCheck = (i == (g_Options.dwFindAllMode & QS_FINDALL_MASK)) ? MF_CHECKED : MF_UNCHECKED;
                    CheckMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_START + i, MF_BYCOMMAND | uCheck );
                }

                if ( g_Options.dwFindAllResult & QS_FINDALL_RSLT_MATCHONLY )
                {
                    CheckMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_SHOWLINE, MF_BYCOMMAND | MF_UNCHECKED );
                    CheckMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_SHOWMATCHONLY, MF_BYCOMMAND | MF_CHECKED );
                }
                else
                {
                    CheckMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_SHOWMATCHONLY, MF_BYCOMMAND | MF_UNCHECKED );
                    CheckMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_SHOWLINE, MF_BYCOMMAND | MF_CHECKED );
                }

                uCheck = (g_Options.dwFindAllResult & QS_FINDALL_RSLT_FILTERMODE) ? MF_CHECKED : MF_UNCHECKED;
                CheckMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_FILTERMODE, MF_BYCOMMAND | uCheck );

                if ( !g_bLogPlugin )
                    EnableMenuItem( g_QSearchDlg.hFindAllPopupMenu, IDM_FINDALL_LOGOUTPUT, MF_BYCOMMAND | MF_DISABLED );

                hPopMnu = g_QSearchDlg.hFindAllPopupMenu;
            }
            else
            {
                for ( i = 0; i < OPTF_COUNT - 1; i++ )
                {
                    switch ( i )
                    {
                        case OPTF_SRCH_PICKUP_SELECTION:
                        case OPTF_SRCH_STOP_EOF:
                        case OPTF_CATCH_MAIN_ESC:
                        case OPTF_HOTKEY_HIDES_PANEL:
                        case OPTF_EDITOR_AUTOFOCUS_MOUSE:
                            uCheck = ((g_Options.dwFlags[i] & 0x01) ? MF_CHECKED : MF_UNCHECKED);
                            break;

                        default:
                            uCheck = (g_Options.dwFlags[i] ? MF_CHECKED : MF_UNCHECKED);
                            break;
                    }
                    CheckMenuItem( g_QSearchDlg.hPopupMenu, IDM_START + i, MF_BYCOMMAND | uCheck );
                }

                if ( g_Options.dwFlags[OPTF_SRCH_USE_REGEXP] )
                    uCheck = MF_BYCOMMAND | MF_ENABLED;
                else
                    uCheck = MF_BYCOMMAND | MF_GRAYED;
                EnableMenuItem( g_QSearchDlg.hPopupMenu, IDM_SRCHREGEXPDOTNEWLINE, uCheck );

                hPopMnu = g_QSearchDlg.hPopupMenu;
            }

            TrackPopupMenuEx(hPopMnu, 0, pt.x, pt.y, hDlg, NULL);

            break;
        }
        case QSM_FINDNEXT:
        case QSM_FINDALL:
        {
            DWORD dwSearch;
            DWORD dwFindAllMode;
            tQSFindAll qsfa;
            tQSFindAll* pqsfa;

            dwSearch = 0;
            if ( lParam & QS_FF_NOSETSELFIRST )
            {
                dwSearch |= QSEARCH_NOSETSEL_FIRST;
            }

            if ( uMsg == QSM_FINDALL )
            {
                dwFindAllMode = (DWORD) (wParam & QS_FINDALL_MASK);

                x_zero_mem(&qsfa, sizeof(tQSFindAll));
                tDynamicBuffer_Init(&qsfa.tempBuf);
                tDynamicBuffer_Init(&qsfa.tempBuf2);
                if ( (dwFindAllMode == QS_FINDALL_LOGOUTPUT) ||
                     (g_Options.dwNewUI != QS_UI_NEW_02) )
                {
                    // LogOutput
                    dwSearch |= QSEARCH_FINDALL;
                    if ( wParam & QS_FINDALL_RSLT_ALLFILES )
                        dwSearch |= QSEARCH_FINDALLFILES;
                    qsfa.pfnFindResultCallback = qsFindResultCallback;
                    qsfa.GetFindResultPolicy.nMode = g_Options.LogOutputFRP.nMode;
                    qsfa.GetFindResultPolicy.nBefore = g_Options.LogOutputFRP.nBefore;
                    qsfa.GetFindResultPolicy.nAfter = g_Options.LogOutputFRP.nAfter;
                    qsfa.GetFindResultPolicy.nMaxLineLen = 0;
                    qsfa.GetFindResultPolicy.pfnStoreResultCallback = qsStoreResultCallback;
                    qsfa.ShowFindResults.pfnInit = qsShowFindResults_LogOutput_Init;
                    qsfa.ShowFindResults.pfnAddOccurrence = qsShowFindResults_LogOutput_AddOccurrence;
                    qsfa.ShowFindResults.pfnDone = qsShowFindResults_LogOutput_Done;
                    qsfa.ShowFindResults.pfnAllFilesInit = qsShowFindResults_LogOutput_AllFiles_Init;
                    qsfa.ShowFindResults.pfnAllFilesDone = qsShowFindResults_LogOutput_AllFiles_Done;
                }
                else if ( dwFindAllMode == QS_FINDALL_FILEOUTPUT_MULT ||
                          dwFindAllMode == QS_FINDALL_FILEOUTPUT_SNGL )
                {
                    // FileOutput
                    dwSearch |= QSEARCH_FINDALL;
                    if ( wParam & QS_FINDALL_RSLT_ALLFILES )
                        dwSearch |= QSEARCH_FINDALLFILES;
                    qsfa.pfnFindResultCallback = qsFindResultCallback;
                    qsfa.GetFindResultPolicy.nMode = g_Options.FileOutputFRP.nMode;
                    qsfa.GetFindResultPolicy.nBefore = g_Options.FileOutputFRP.nBefore;
                    qsfa.GetFindResultPolicy.nAfter = g_Options.FileOutputFRP.nAfter;
                    qsfa.GetFindResultPolicy.nMaxLineLen = 0;
                    qsfa.GetFindResultPolicy.pfnStoreResultCallback = qsStoreResultCallback;
                    qsfa.ShowFindResults.pfnInit = qsShowFindResults_FileOutput_Init;
                    qsfa.ShowFindResults.pfnAddOccurrence = qsShowFindResults_FileOutput_AddOccurrence;
                    qsfa.ShowFindResults.pfnDone = qsShowFindResults_FileOutput_Done;
                    qsfa.ShowFindResults.pfnAllFilesInit = qsShowFindResults_FileOutput_AllFiles_Init;
                    qsfa.ShowFindResults.pfnAllFilesDone = qsShowFindResults_FileOutput_AllFiles_Done;
                }
                else
                {
                    // CountOnly
                    qsfa.pfnFindResultCallback = NULL;
                    qsfa.ShowFindResults.pfnInit = qsShowFindResults_CountOnly_Init;
                    qsfa.ShowFindResults.pfnAddOccurrence = qsShowFindResults_CountOnly_AddOccurrence;
                    qsfa.ShowFindResults.pfnDone = qsShowFindResults_CountOnly_Done;
                    qsfa.ShowFindResults.pfnAllFilesInit = qsShowFindResults_CountOnly_AllFiles_Init;
                    qsfa.ShowFindResults.pfnAllFilesDone = qsShowFindResults_CountOnly_AllFiles_Done;
                }
                pqsfa = &qsfa;
            }
            else
                pqsfa = NULL;

            if ( (g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01) &&
                 (g_QSearchDlg.uSearchOrigin == QS_SO_EDITOR) &&
                 (!(lParam & QS_FF_NOPICKUPSEL)) )
            {
                getEditFindText( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW );
                qsearchFindHistoryAdd( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW, 0 );
                qsPickUpSelection( g_QSearchDlg.hFindEdit, g_Options.dwFlags, FALSE );
                dwSearch |= QSEARCH_NOSETSEL_FIRST;
            }
            getEditFindText( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW );
            if ( qs_bForceFindFirst || qs_bEditTextChanged )
            {
                qs_bForceFindFirst = FALSE;
                qs_bEditTextChanged = FALSE;
                dwSearch |= (QSEARCH_FIRST | QSEARCH_NOFINDBEGIN);
                if ( wParam )
                    dwSearch |= QSEARCH_FINDUP;
                qsearchDoSearchText( g_QSearchDlg.hFindEdit, dwSearch, g_Options.dwFlags, pqsfa );
                qsearchDoTryHighlightAll( hDlg, g_Options.dwFlags, QHC_CHECKBOX_CHECKED );
            }
            else
            {
                dwSearch |= QSEARCH_NEXT;
                if ( wParam )
                    dwSearch |= QSEARCH_FINDUP;
                qsearchDoSearchText( g_QSearchDlg.hFindEdit, dwSearch, g_Options.dwFlags, pqsfa );
            }
            return 1;
        }
        case QSM_FINDFIRST:
        {
            DWORD dwSearch = QSEARCH_FIRST;

            if ( wParam & QSEARCH_NOFINDUP )
                dwSearch |= QSEARCH_NOFINDUP;

            if ( wParam & QSEARCH_NOFINDBEGIN )
                dwSearch |= QSEARCH_NOFINDBEGIN;

            if ( wParam & QSEARCH_USEDELAY )
                dwSearch |= QSEARCH_USEDELAY;

            getEditFindText( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW );
            qsearchDoSearchText( g_QSearchDlg.hFindEdit, dwSearch, g_Options.dwFlags, NULL );
            qsearchDoTryHighlightAll( hDlg, g_Options.dwFlags, QHC_CHECKBOX_CHECKED );
            return 1;
        }
        case QSM_SELFIND:
        {
            qsearchDoSelFind( g_QSearchDlg.hFindEdit, (BOOL) wParam, g_Options.dwFlags );
            return 1;
        }
        case QSM_PICKUPSELTEXT:
        {
            UINT nPickedUp;
            if ( wParam & QS_PS_UPDATEHISTORY )
            {
                getEditFindText( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW );
                qsearchFindHistoryAdd( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW, 0 );
            }
            nPickedUp = qsPickUpSelection(g_QSearchDlg.hFindEdit, g_Options.dwFlags, FALSE);
            if ( lParam )
                *((UINT *)lParam) = nPickedUp; // see QS_PSF_*
            return 1;
        }
        case QSM_GETHWNDEDIT:
        {
            if ( lParam )
                *((HWND *)lParam) = g_QSearchDlg.hFindEdit;
            return 1;
        }
        case QSM_GETHWNDCOMBO:
        {
            if ( lParam )
                *((HWND *)lParam) = GetDlgItem(hDlg, IDC_CB_FINDTEXT);
            return 1;
        }
        case QSM_SHOWHIDE:
        {
            qsearchDoShowHide( hDlg, (BOOL) wParam, (UINT) lParam, g_Options.dwFlags );
            return 1;
        }
        case QSM_SETNOTFOUND:
        {
            UINT uFlags;

            qsearchDoSetNotFound( g_QSearchDlg.hFindEdit, (BOOL) wParam, FALSE, FALSE );

            uFlags = (UINT) lParam;
            if ( uFlags & QS_SNF_SETINFOEMPTY )
            {
                qsSetInfoEmpty();
            }
            if ( uFlags & QS_SNF_FORCEFINDFIRST )
            {
                qs_bForceFindFirst = TRUE;
                qs_bEditTextChanged = TRUE;
            }

            return 1;
        }
        case QSM_CHECKHIGHLIGHT:
        {
            HWND hChHighlightAll;

            g_bHighlightPlugin = isHighlightMainActive();

            hChHighlightAll = GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL);
            EnableWindow( hChHighlightAll, g_bHighlightPlugin );
            SendMessage( hChHighlightAll, BM_SETCHECK,
              (((g_Options.dwHighlightState & HLS_IS_CHECKED) == HLS_IS_CHECKED) ?
                   BST_CHECKED : BST_UNCHECKED), 0 );

            return 1;
        }
        case QSM_UPDATEUI:
        {
            HWND hFocusedWnd = GetFocus();

            if ( wParam & QS_UU_WHOLEWORD )
            {
                // show/hide whole word check-box
                qsdlgShowHideWholeWordCheckBox(hDlg, g_Options.dwFlags);
            }

            if ( wParam & QS_UU_FIND )
            {
                // set edit or combo-box find control
                BOOL isFindEditFocused = FALSE;

                if ( hFocusedWnd )
                {
                    if ( hFocusedWnd == g_QSearchDlg.hFindEdit )
                        isFindEditFocused = TRUE;
                }

                if ( qsearchIsFindHistoryEnabled() )
                {
                    // hide edit control
                    ShowWindow( GetDlgItem(hDlg, IDC_ED_FINDTEXT), SW_HIDE );
                    // show combo-box control
                    ShowWindow( GetDlgItem(hDlg, IDC_CB_FINDTEXT), SW_SHOWNORMAL );
                }
                else
                {
                    // hide combo-box control
                    ShowWindow( GetDlgItem(hDlg, IDC_CB_FINDTEXT), SW_HIDE );
                    // show edit control
                    ShowWindow( GetDlgItem(hDlg, IDC_ED_FINDTEXT), SW_SHOWNORMAL );
                }

                // update hFindEdit
                g_QSearchDlg.hFindEdit = qsearchGetFindEdit(hDlg);
                // set focus if needed
                if ( isFindEditFocused )
                    SetFocus(g_QSearchDlg.hFindEdit);
            }

            return 1;
        }
        case QSM_QUIT:
        {
            qsearchDoQuit( g_QSearchDlg.hFindEdit, hToolTip, hPopupMenuLoaded, hTextNotFoundBrush, hTextNotRegExpBrush, hTextEOFBrush );
            return 1;
        }
        case QSN_DLGSWITCH:
        {
            g_QSearchDlg.uSearchOrigin = QS_SO_QSEARCH;
            qs_bEditTextChanged = TRUE;
            qs_bForceFindFirst = TRUE;
            //getEditFindText( g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW );
            //if ( g_Options.dwFlags[OPTF_SRCH_ONTHEFLY_MODE] )
            //{
            //    qsearchDoTryHighlightAll( hDlg, g_Options.dwFlags, QHC_CHECKBOX_CHECKED );
            //}
            return 1;
        }
        case WM_INITDIALOG:
        {
            HWND hDlgItm;

            InitializeCriticalSection(&csFindAllTimerId);

            GetClientRect(hDlg, &rcDlg_0);
            hEdFindText = GetDlgItemAndRect(hDlg, IDC_ED_FINDTEXT, &rcEdFindText_0);
            hCbFindText = GetDlgItemAndRect(hDlg, IDC_CB_FINDTEXT, &rcCbFindText_0);
            hBtFindNext = GetDlgItemAndRect(hDlg, IDC_BT_FINDNEXT, &rcBtFindNext_0);
            hBtFindPrev = GetDlgItemAndRect(hDlg, IDC_BT_FINDPREV, &rcBtFindPrev_0);
            hBtFindAll = GetDlgItemAndRect(hDlg, IDC_BT_FINDALL, &rcBtFindAll_0);
            hBtCancel = GetDlgItem(hDlg, IDC_BT_CANCEL);
            hPbProgress = GetDlgItemAndRect(hDlg, IDC_PB_PROGRESS, &rcPbProgress_0);
            hChMatchCase = GetDlgItemAndRect(hDlg, IDC_CH_MATCHCASE, &rcChMatchCase_0);
            hChWholeWord = GetDlgItemAndRect(hDlg, IDC_CH_WHOLEWORD, &rcChWholeWord_0);
            hChHighlightAll = GetDlgItemAndRect(hDlg, IDC_CH_HIGHLIGHTALL, &rcChHighlightAll_0);
            hStInfo = GetDlgItemAndRect(hDlg, IDC_ST_INFO, &rcStInfo_0);

            g_QSearchDlg.bIsQSearchingRightNow = FALSE;
            g_QSearchDlg.bMouseJustLeavedFindEdit = FALSE;
            g_QSearchDlg.hDlg = hDlg;
            g_QSearchDlg.hFindEdit = qsearchGetFindEdit(hDlg);

            if ( hChMatchCase )
            {
                SendMessage( hChMatchCase, BM_SETCHECK,
                  g_QSearchDlg.bMatchCase ? BST_CHECKED : BST_UNCHECKED, 0 );
            }

            // restoring initial values of static vars
            qs_bEditIsActive = TRUE;
            qs_bEditCanBeNonActive = TRUE;
            qs_bEditSelJustChanged = FALSE;
            qs_bEditNotFound = FALSE;
            qs_bEditNotRegExp = FALSE;
            qs_bEditIsEOF = FALSE;
            qs_nEditEOF = 0;
            qs_bEditTextChanged = TRUE;
            qs_bHotKeyPressedOnShow = FALSE;
            qs_bBtnFindIsFocused = FALSE;
            qs_dwHotKey = 0;

            hTextNotFoundBrush = CreateSolidBrush(g_Options.colorNotFound);
            hTextNotRegExpBrush = CreateSolidBrush(g_Options.colorNotRegExp);
            if ( g_Options.colorEOF != RGB(0xFF,0xFF,0xFF) )
            {
                hTextEOFBrush = CreateSolidBrush(g_Options.colorEOF);
            }

            if ( qsearchIsFindHistoryEnabled() )
                hDlgItm = hCbFindText;
            else
                hDlgItm = hEdFindText;
            ShowWindow(hDlgItm, SW_SHOWNORMAL);

            SendMessage(g_QSearchDlg.hFindEdit, EM_SETLIMITTEXT, MAX_TEXT_SIZE - 1, 0);
            prev_editWndProc = setWndProc(g_QSearchDlg.hFindEdit, editWndProc);
            if ( hBtCancel )
            {
                BUTTONDRAW bd;

                bd.dwFlags = BIF_CROSS;
                bd.hImage = NULL;
                bd.nImageWidth = 0;
                bd.nImageHeight = 0;
                SendMessage(g_Plugin.hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hBtCancel, (LPARAM)&bd);

                /* prev_btnWndProc = setWndProc(hBtCancel, btnWndProc); */
            }
            if ( hChWholeWord )
            {
                prev_chWholeWordWndProc = setWndProc(hChWholeWord, chWholeWordWndProc);
            }

            g_QSearchDlg.hBtnFindNext = hBtFindNext;
            if ( g_QSearchDlg.hBtnFindNext )
            {
                prev_btnFindNextWndProc = setWndProc(g_QSearchDlg.hBtnFindNext, btnFindWndProc);
            }
            g_QSearchDlg.hBtnFindPrev = hBtFindPrev;
            if ( g_QSearchDlg.hBtnFindPrev )
            {
                prev_btnFindPrevWndProc = setWndProc(g_QSearchDlg.hBtnFindPrev, btnFindWndProc);
            }
            g_QSearchDlg.hBtnFindAll = hBtFindAll;
            if ( g_QSearchDlg.hBtnFindAll )
            {
                prev_btnFindAllWndProc = setWndProc(g_QSearchDlg.hBtnFindAll, btnFindWndProc);
            }
            g_QSearchDlg.hStInfo = hStInfo;

            hToolTip = qsearchDoInitToolTip(hDlg, g_QSearchDlg.hFindEdit);
            if ( g_Plugin.bOldWindows )
            {
                hPopupMenuLoaded = LoadMenuA( g_Plugin.hInstanceDLL,
                  MAKEINTRESOURCEA(IDR_MENU_OPTIONS) );
            }
            else
            {
                hPopupMenuLoaded = LoadMenuW( g_Plugin.hInstanceDLL,
                  MAKEINTRESOURCEW(IDR_MENU_OPTIONS) );
            }
            g_QSearchDlg.hPopupMenu = GetSubMenu(hPopupMenuLoaded, 0);
            g_QSearchDlg.hFindAllPopupMenu = GetSubMenu(hPopupMenuLoaded, 1);
            qsearchSetPopupMenuLang(g_QSearchDlg.hPopupMenu);
            qsearchSetFindAllPopupMenuLang(g_QSearchDlg.hFindAllPopupMenu);
            qsearchSetMainDlgLang(hDlg);

            qsdlgShowHideWholeWordCheckBox(hDlg, g_Options.dwFlags);

            if ( g_Plugin.bOldWindows )
            {
                ReadFindHistoryA();
            }
            else
            {
                ReadFindHistoryW();
            }

            if ( g_Options.dwFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
            {
                getAkelPadSelectedText(g_QSearchDlg.szFindTextW, g_Options.dwFlags);
            }
            setEditFindText(g_QSearchDlg.hFindEdit, g_QSearchDlg.szFindTextW);
            //SetFocus(g_QSearchDlg.hFindEdit);
            if ( !g_Options.dwFlags[OPTF_EDIT_FOCUS_SELECTALL] )
            {
                SendMessage(g_QSearchDlg.hFindEdit, EM_SETSEL, 0, -1);
#ifdef _DEBUG
                Debug_Output("qsearchDlgProc, WM_INITDIALOG, if (!SelectAll), SETSEL(0, -1)\n");
#endif
            }

            g_bHighlightPlugin = isHighlightMainActive();

            EnableWindow( hChHighlightAll, g_bHighlightPlugin );
            SendMessage( hChHighlightAll, BM_SETCHECK,
              (((g_Options.dwHighlightState & HLS_IS_CHECKED) == HLS_IS_CHECKED) ?
                   BST_CHECKED : BST_UNCHECKED), 0 );

            break;
        }
        default:
            break;
    }

    // Dialog resize messages
    switch ( uMsg )
    {
        case WM_CREATE:
        case WM_INITDIALOG:
        case WM_GETMINMAXINFO:
        case WM_WINDOWPOSCHANGING:
        case WM_MOVE:
        case WM_SIZE:
        case WM_PAINT:
        {
            RECT rcDlgCurr;

            if ( GetClientRect(hDlg, &rcDlgCurr) )
            {
                LONG nWidthInit;
                LONG nWidthCurr;
                LONG dx;

                nWidthCurr = rcDlgCurr.right - rcDlgCurr.left;
                nWidthInit = rcDlg_0.right - rcDlg_0.left;
                dx = nWidthCurr - nWidthInit;

                if ( dx < 0 )
                {
                    if ( g_Options.dwEditMinWidth != 0 )
                    {
                        if ( rcEdFindText_0.right - rcEdFindText_0.left + dx < (LONG) g_Options.dwEditMinWidth )
                            dx = rcEdFindText_0.left + g_Options.dwEditMinWidth - rcEdFindText_0.right;
                    }
                    else
                        dx = 0;
                }
                else // dx >= 0
                {
                    if ( g_Options.dwEditMaxWidth != 0 )
                    {
                        if ( rcEdFindText_0.right - rcEdFindText_0.left + dx > (LONG) g_Options.dwEditMaxWidth )
                            dx = rcEdFindText_0.left + g_Options.dwEditMaxWidth - rcEdFindText_0.right;
                    }
                }

                MoveWindowByDx(hStInfo, &rcStInfo_0, dx);
                MoveWindowByDx(hChHighlightAll, &rcChHighlightAll_0, dx);
                MoveWindowByDx(hChWholeWord, &rcChWholeWord_0, dx);
                MoveWindowByDx(hChMatchCase, &rcChMatchCase_0, dx);
                MoveWindowByDx(hPbProgress, &rcPbProgress_0, dx);
                MoveWindowByDx(hBtFindAll, &rcBtFindAll_0, dx);
                MoveWindowByDx(hBtFindPrev, &rcBtFindPrev_0, dx);
                MoveWindowByDx(hBtFindNext, &rcBtFindNext_0, dx);
                ResizeWindowByDx(hCbFindText, &rcCbFindText_0, dx);
                ResizeWindowByDx(hEdFindText, &rcEdFindText_0, dx);

                RedrawWindowByDx(hEdFindText);
                RedrawWindowByDx(hCbFindText);
                RedrawWindowByDx(hBtFindNext);
                RedrawWindowByDx(hBtFindPrev);
                RedrawWindowByDx(hBtFindAll);
                RedrawWindowByDx(hPbProgress);
                RedrawWindowByDx(hChMatchCase);
                RedrawWindowByDx(hChWholeWord);
                RedrawWindowByDx(hChHighlightAll);
                RedrawWindowByDx(hStInfo);

                if ( g_QSearchDlg.pDockData )
                {
                    GetClientRect(hDlg, &g_QSearchDlg.pDockData->rcDragDrop);
                }

                if ( g_QSearchDlg.hFindEdit )
                {
                    if ( g_Plugin.bOldWindows )
                    {
                        TOOLINFOA tiA;

                        fillToolInfoA( &tiA, LPSTR_TEXTCALLBACKA, g_QSearchDlg.hFindEdit, IDC_ED_FINDTEXT );
                        // LPSTR_TEXTCALLBACKA means "send TTN_GETDISPINFOA to hEdit"
                        SendMessage( hToolTip, TTM_NEWTOOLRECTA, 0, (LPARAM) &tiA );
                    }
                    else
                    {
                        TOOLINFOW tiW;

                        fillToolInfoW( &tiW, LPSTR_TEXTCALLBACKW, g_QSearchDlg.hFindEdit, IDC_ED_FINDTEXT );
                        // LPSTR_TEXTCALLBACKW means "send TTN_GETDISPINFOW to hEdit"
                        SendMessageW( hToolTip, TTM_NEWTOOLRECTW, 0, (LPARAM) &tiW );
                    }
                }
            }
            break;
        }
    }

    return 0;
}

HWND qsearchDoInitToolTip(HWND hDlg, HWND hEdit)
{
    INITCOMMONCONTROLSEX iccex;
    HWND                 hToolTip = NULL;

    iccex.dwICC = ICC_WIN95_CLASSES;
    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx( &iccex );

    if ( g_Plugin.bOldWindows )
    {
        hToolTip = CreateWindowExA( WS_EX_TOPMOST, TOOLTIPS_CLASSA,
          0, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
          hDlg, 0, g_Plugin.hInstanceDLL, 0 );
    }
    else
    {
        hToolTip = CreateWindowExW( WS_EX_TOPMOST, TOOLTIPS_CLASSW,
          0, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
          hDlg, 0, g_Plugin.hInstanceDLL, 0 );

    }

    if ( hToolTip )
    {
        SetWindowPos( hToolTip, HWND_TOPMOST,
          0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        if ( g_Plugin.bOldWindows )
        {
            TOOLINFOA tiA;
            char szHintA[128];

            fillToolInfoA( &tiA, LPSTR_TEXTCALLBACKA, hEdit, IDC_ED_FINDTEXT );
            // LPSTR_TEXTCALLBACKA means "send TTN_GETDISPINFOA to hEdit"
            SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

            lstrcpyA( szHintA, qsearchGetHintA(IDC_BT_CANCEL) );
            fillToolInfoA( &tiA, szHintA, GetDlgItem(hDlg, IDC_BT_CANCEL), IDC_BT_CANCEL );
            SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

            lstrcpyA( szHintA, qsearchGetHintA(IDC_CH_MATCHCASE) );
            if ( g_Options.dwUseAltHotkeys )
            {
                strAppendAltHotkeyA( szHintA, g_Options.dwAltMatchCase );
            }
            fillToolInfoA( &tiA, szHintA, GetDlgItem(hDlg, IDC_CH_MATCHCASE), IDC_CH_MATCHCASE );
            SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

            fillToolInfoA( &tiA, LPSTR_TEXTCALLBACKA, GetDlgItem(hDlg, IDC_CH_WHOLEWORD), IDC_CH_WHOLEWORD );
            // LPSTR_TEXTCALLBACKA means "send TTN_GETDISPINFOA to hCheckbox"
            SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

            lstrcpyA( szHintA, qsearchGetHintA(IDC_CH_HIGHLIGHTALL) );
            if ( g_Options.dwUseAltHotkeys )
            {
                strAppendAltHotkeyA( szHintA, g_Options.dwAltHighlightAll );
            }
            fillToolInfoA( &tiA, szHintA, GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL), IDC_CH_HIGHLIGHTALL );
            SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

            if ( g_Options.dwNewUI == QS_UI_NEW_01 ||
                 g_Options.dwNewUI == QS_UI_NEW_02 )
            {
                fillToolInfoA( &tiA, (LPSTR) qsearchGetHintA(IDC_BT_FINDNEXT),
                  GetDlgItem(hDlg, IDC_BT_FINDNEXT), IDC_BT_FINDNEXT );
                SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

                fillToolInfoA( &tiA, (LPSTR) qsearchGetHintA(IDC_BT_FINDPREV),
                  GetDlgItem(hDlg, IDC_BT_FINDPREV), IDC_BT_FINDPREV );
                SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

                if ( g_Options.dwNewUI == QS_UI_NEW_02 )
                {
                    fillToolInfoA( &tiA, LPSTR_TEXTCALLBACKA,
                      GetDlgItem(hDlg, IDC_BT_FINDALL), IDC_BT_FINDALL );
                    SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );
                }
            }
        }
        else
        {
            TOOLINFOW tiW;
            wchar_t szHintW[128];

            fillToolInfoW( &tiW, LPSTR_TEXTCALLBACKW, hEdit, IDC_ED_FINDTEXT );
            // LPSTR_TEXTCALLBACKW means "send TTN_GETDISPINFOW to hEdit"
            SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

            lstrcpyW( szHintW, qsearchGetHintW(IDC_BT_CANCEL) );
            fillToolInfoW( &tiW, szHintW, GetDlgItem(hDlg, IDC_BT_CANCEL), IDC_BT_CANCEL );
            SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

            lstrcpyW( szHintW, qsearchGetHintW(IDC_CH_MATCHCASE) );
            if ( g_Options.dwUseAltHotkeys )
            {
                strAppendAltHotkeyW( szHintW, g_Options.dwAltMatchCase );
            }
            fillToolInfoW( &tiW, szHintW, GetDlgItem(hDlg, IDC_CH_MATCHCASE), IDC_CH_MATCHCASE );
            SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

            fillToolInfoW( &tiW, LPSTR_TEXTCALLBACKW, GetDlgItem(hDlg, IDC_CH_WHOLEWORD), IDC_CH_WHOLEWORD );
            // LPSTR_TEXTCALLBACKW means "send TTN_GETDISPINFOW to hCheckbox"
            SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

            lstrcpyW( szHintW, qsearchGetHintW(IDC_CH_HIGHLIGHTALL) );
            if ( g_Options.dwUseAltHotkeys )
            {
                strAppendAltHotkeyW( szHintW, g_Options.dwAltHighlightAll );
            }
            fillToolInfoW( &tiW, szHintW, GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL), IDC_CH_HIGHLIGHTALL );
            SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

            if ( g_Options.dwNewUI == QS_UI_NEW_01 ||
                 g_Options.dwNewUI == QS_UI_NEW_02 )
            {
                fillToolInfoW( &tiW, (LPWSTR) qsearchGetHintW(IDC_BT_FINDNEXT),
                  GetDlgItem(hDlg, IDC_BT_FINDNEXT), IDC_BT_FINDNEXT );
                SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

                fillToolInfoW( &tiW, (LPWSTR) qsearchGetHintW(IDC_BT_FINDPREV),
                  GetDlgItem(hDlg, IDC_BT_FINDPREV), IDC_BT_FINDPREV );
                SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

                if ( g_Options.dwNewUI == QS_UI_NEW_02 )
                {
                    fillToolInfoW( &tiW, LPSTR_TEXTCALLBACKW,
                      GetDlgItem(hDlg, IDC_BT_FINDALL), IDC_BT_FINDALL );
                    SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );
                }
            }
        }
    }

    return hToolTip;
}

void qsearchDoQuit(HWND hEdit, HWND hToolTip, HMENU hPopupMenuLoaded, HBRUSH hBrush1, HBRUSH hBrush2, HBRUSH hBrush3)
{
    HWND hDlgItm;

    if ( g_Plugin.bOldWindows )
    {
        SaveFindHistoryA();
    }
    else
    {
        SaveFindHistoryW();
    }

    hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE);
    if ( hDlgItm )
    {
        if ( SendMessage(hDlgItm, BM_GETCHECK, 0, 0) == BST_CHECKED )
            g_QSearchDlg.bMatchCase = TRUE;
        else
            g_QSearchDlg.bMatchCase = FALSE;
    }

    getEditFindText(hEdit, g_QSearchDlg.szFindTextW);

    if ( prev_editWndProc )
    {
        setWndProc(hEdit, prev_editWndProc);
        prev_editWndProc = NULL;
    }
    /* if ( prev_btnWndProc )
    {
        hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_BT_CANCEL);
        setWndProc(hDlgItm, prev_btnWndProc);
        prev_btnWndProc = NULL;
    } */
    if ( prev_chWholeWordWndProc )
    {
        hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_WHOLEWORD);
        setWndProc(hDlgItm, prev_chWholeWordWndProc);
        prev_chWholeWordWndProc = NULL;
    }
    if ( prev_btnFindNextWndProc )
    {
        setWndProc(g_QSearchDlg.hBtnFindNext, prev_btnFindNextWndProc);
        prev_btnFindNextWndProc = NULL;
    }
    if ( prev_btnFindPrevWndProc )
    {
        setWndProc(g_QSearchDlg.hBtnFindPrev, prev_btnFindPrevWndProc);
        prev_btnFindPrevWndProc = NULL;
    }
    if ( prev_btnFindAllWndProc )
    {
        setWndProc(g_QSearchDlg.hBtnFindAll, prev_btnFindAllWndProc);
        prev_btnFindAllWndProc = NULL;
    }
    if ( hToolTip )
    {
        DestroyWindow( hToolTip );
    }
    if ( hBrush1 )
    {
        DeleteObject(hBrush1);
    }
    if ( hBrush2 )
    {
        DeleteObject(hBrush2);
    }
    if ( hBrush3 )
    {
        DeleteObject(hBrush3);
    }
    if ( hPopupMenuLoaded )
    {
        DestroyMenu(hPopupMenuLoaded);
    }
    DestroyWindow( g_QSearchDlg.hDlg );
    g_QSearchDlg.hDlg = NULL;

    if ( !g_Plugin.bAkelPadOnFinish )
    {
        Uninitialize();
        SendMessage(g_Plugin.hMainWnd, AKD_RESIZE, 0, 0);
        SetFocus(g_Plugin.hMainWnd);
    }

    DeleteCriticalSection(&csFindAllTimerId);
}

void qsearchDoSetNotFound(HWND hEdit, BOOL bNotFound, BOOL bNotRegExp, BOOL bEOF)
{
    if ( bNotFound )
    {
        qsearchDoTryUnhighlightAll();
        qsSetInfoEmpty();
    }

    qs_bEditNotFound = bNotFound;
    qs_bEditNotRegExp = bNotRegExp;
    qs_bEditIsEOF = bEOF;
    InvalidateRect(hEdit, NULL, TRUE);
    UpdateWindow(hEdit);
}

void qsearchDoShowHide(HWND hDlg, BOOL bShow, UINT uShowFlags, const DWORD dwOptFlags[])
{
    BOOL bChangeSelection = !IsWindowVisible(hDlg);

    qsearchDoSetNotFound( qsearchGetFindEdit(hDlg), FALSE, FALSE, FALSE );
    qsSetInfoEmpty();

    if ( bShow )
    {
        HWND hCloseBtn = GetDlgItem(hDlg, IDC_BT_CANCEL);

        SendMessage(hCloseBtn, BM_SETSTATE, FALSE, 0);
        SendMessage(
            hCloseBtn,
            BM_SETSTYLE,
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_OWNERDRAW | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            TRUE );

        /*
        // The following line is commented because
        // the hot key is re-read on WM_SHOWWINDOW.
        qs_dwHotKey = getQSearchHotKey();
        */
        qs_bHotKeyPressedOnShow = isQSearchHotKeyPressed();
    }
    else
    {
        qs_nEditEOF = 0;
    }

    g_QSearchDlg.uWmShowFlags = 0; // forbid to pick up selected text on WM_SHOWWINDOW
    SendMessage( g_Plugin.hMainWnd, AKD_DOCK,
      (bShow ? DK_SHOW : DK_HIDE), (LPARAM) g_QSearchDlg.pDockData );
    g_QSearchDlg.uWmShowFlags = 0; // just in case :)

    // Change AkelPad's plugin status (running/not running)
    if ( g_szFunctionQSearchW[0] )
    {
        if ( g_Plugin.bOldWindows )
        {
            PLUGINFUNCTION* pfA;

            pfA = (PLUGINFUNCTION *) SendMessage( g_Plugin.hMainWnd,
              AKD_DLLFINDA, (WPARAM) g_szFunctionQSearchW, 0 );
            if ( pfA )
            {
                pfA->bRunning = bShow;
            }
        }
        else
        {
            PLUGINFUNCTION* pfW;

            pfW = (PLUGINFUNCTION *) SendMessage( g_Plugin.hMainWnd,
              AKD_DLLFINDW, (WPARAM) g_szFunctionQSearchW, 0 );
            if ( pfW )
            {
                pfW->bRunning = bShow;
            }
        }

        // Send "empty" command to AkelPad to update the Toolbar plugin state
        SendMessage(g_Plugin.hMainWnd, WM_COMMAND, 0, 0);
    }

    if ( bShow )
    {
        BOOL bGotSelectedText = FALSE;
        HWND hEdit = qsearchGetFindEdit(hDlg);

        if ( uShowFlags & QS_SF_CANPICKUPSELTEXT )
        {
            if ( dwOptFlags[OPTF_SRCH_PICKUP_SELECTION] & 0x01 )
            {
                bGotSelectedText = getAkelPadSelectedText(g_QSearchDlg.szFindTextW, dwOptFlags);
                if ( bGotSelectedText )
                {
                    bChangeSelection = TRUE;
                }
                else
                {
                    getEditFindText(hEdit, g_QSearchDlg.szFindTextW);
                }
                setEditFindText(hEdit, g_QSearchDlg.szFindTextW);
                if ( (!bChangeSelection) || dwOptFlags[OPTF_EDIT_FOCUS_SELECTALL] )
                {
                    SendMessage(hEdit, EM_SETSEL, 0, -1);
    #ifdef _DEBUG
                    Debug_Output("qsearchDoShowHide, PickUpSel, SETSEL(0, -1)\n");
    #endif
                    qs_bEditSelJustChanged = TRUE;
                }
            }
        }
        if ( bChangeSelection && !dwOptFlags[OPTF_EDIT_FOCUS_SELECTALL] )
        {
            SendMessage(hEdit, EM_SETSEL, 0, -1);
#ifdef _DEBUG
            Debug_Output("qsearchDoShowHide, ChangeSel, SETSEL(0, -1)\n");
#endif
            qs_bEditSelJustChanged = TRUE;
        }
        if ( bGotSelectedText )
        {
            if ( dwOptFlags[OPTF_SRCH_ONTHEFLY_MODE] )
            {
                qs_bForceFindFirst = FALSE;
                qsearchDoTryHighlightAll(g_QSearchDlg.hDlg, dwOptFlags, QHC_CHECKBOX_CHECKED);
            }
            else
                qs_bForceFindFirst = TRUE;
        }
        SetFocus(hEdit);
    }
    else
    {
        //SendMessage(g_Plugin.hMainWnd, AKD_RESIZE, 0, 0);
        SetFocus(g_Plugin.hMainWnd);
    }
}

void qsearchDoSelFind(HWND hEdit, BOOL bFindPrev, const DWORD dwOptFlags[])
{
    wchar_t prevFindTextW[MAX_TEXT_SIZE];
    DWORD   dwOptFlagsTemp[OPTF_COUNT];

    if ( g_Plugin.bOldWindows )
    {
        lstrcpyA( (LPSTR) prevFindTextW, (LPCSTR) g_QSearchDlg.szFindTextW );
    }
    else
    {
        lstrcpyW( (LPWSTR) prevFindTextW, (LPCWSTR) g_QSearchDlg.szFindTextW );
    }

    if ( dwOptFlags[OPTF_SRCH_SELFIND_PICKUP] )
    {
        // getting selected text with current search flags
        if ( getAkelPadSelectedText(g_QSearchDlg.szFindTextW, dwOptFlags) )
        {
            setEditFindText(hEdit, g_QSearchDlg.szFindTextW);
        }
    }

    copyOptionsFlags(dwOptFlagsTemp, dwOptFlags);
    // these search flags must be disabled here
    dwOptFlagsTemp[OPTF_SRCH_FROM_BEGINNING] = 0;
    dwOptFlagsTemp[OPTF_SRCH_USE_SPECIALCHARS] = 0;
    dwOptFlagsTemp[OPTF_SRCH_USE_REGEXP] = 0;

    // getting selected text with modified search flags
    if ( getAkelPadSelectedText(g_QSearchDlg.szFindTextW, dwOptFlagsTemp) )
    {
        DWORD  dwSearchParams;
        BOOL   bMatchCase;

        bMatchCase = isCheckBoxChecked(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE);

        // clear the "not found" flag
        qs_bEditNotFound = FALSE;
        qs_bEditNotRegExp = FALSE;
        qs_bEditIsEOF = FALSE;

        if ( g_Plugin.bOldWindows )
        {
            int (WINAPI *cmpfuncA)(LPCSTR, LPCSTR) = bMatchCase ? lstrcmpA : lstrcmpiA;
            if ( cmpfuncA((LPCSTR) prevFindTextW, (LPCSTR) g_QSearchDlg.szFindTextW) != 0 )
            {
                qs_bEditTextChanged = TRUE;
                qs_nEditEOF = 0;
            }
        }
        else
        {
            int (WINAPI *cmpfuncW)(LPCWSTR, LPCWSTR) = bMatchCase ? lstrcmpW : lstrcmpiW;
            if ( cmpfuncW((LPCWSTR) prevFindTextW, (LPCWSTR) g_QSearchDlg.szFindTextW) != 0 )
            {
                qs_bEditTextChanged = TRUE;
                qs_nEditEOF = 0;
            }
        }

        dwSearchParams = QSEARCH_NEXT | QSEARCH_SEL;
        if ( bFindPrev )  dwSearchParams |= QSEARCH_SEL_FINDUP;
        qsearchDoSearchText(hEdit, dwSearchParams, dwOptFlagsTemp, NULL);
    }
}

static void adjustIncompleteRegExA(char* szTextA, const DWORD dwOptFlags[])
{
    int n1, n2;

    if ( g_Options.dwAdjIncomplRegExp == 0 )
        return;

    if ( g_Options.dwAdjIncomplRegExp == 2 )
    {
        if ( dwOptFlags[OPTF_SRCH_ONTHEFLY_MODE] == 0 )
            return;
    }

    if ( !szTextA )
        return; // NULL, nothing to do

    n1 = lstrlenA(szTextA);
    if ( n1 == 0 )
        return; // just "", nothing to do

    n2 = n1;
    // skipping trailing '?'
    while ( szTextA[n1 - 1] == '?' )
    {
        --n1;
        if ( n2 - n1 > 1 )
            return; // syntax error: "??"

        if ( n1 == 0 )
            return; // just "?", nothing to do
    }

    n2 = n1;
    // skipping trailing '+' or '*'
    while ( (szTextA[n1 - 1] == '+') || (szTextA[n1 - 1] == '*') )
    {
        --n1;
        if ( n2 - n1 > 1 )
        {
            // "++", "**", "+*" or "*+"
            // special case to preserve the first '+' or '*' from the two at the end
            szTextA[n1 + 1] = 0;
            return;
        }

        if ( n1 == 0 )
            return; // just "+(?)" or "*(?)", nothing to do
    }

    if ( n1 == n2 )
        return; // no '+' or '*' found, nothing to do

    n2 = n1;
    // skipping '\' before the trailing '+' or '*'
    while ( (n2 > 0) && (szTextA[n2 - 1] == '\\') )
    {
        --n2;
    }

    if ( ((n1 - n2) % 2) != 0 )
        return; // '+' or '*' is escaped by '\', nothing to do

    // exclude trailing '+' or '*'
    szTextA[n1] = 0;
}

static void adjustIncompleteRegExW(wchar_t* szTextW, const DWORD dwOptFlags[])
{
    int n1, n2;

    if ( g_Options.dwAdjIncomplRegExp == 0 )
        return;

    if ( g_Options.dwAdjIncomplRegExp == 2 )
    {
        if ( dwOptFlags[OPTF_SRCH_ONTHEFLY_MODE] == 0 )
            return;
    }

    if ( !szTextW )
        return; // NULL, nothing to do

    n1 = lstrlenW(szTextW);
    if ( n1 == 0 )
        return; // just "", nothing to do

    n2 = n1;
    // skipping trailing '?'
    while ( szTextW[n1 - 1] == L'?' )
    {
        --n1;
        if ( n2 - n1 > 1 )
            return; // syntax error: "??"

        if ( n1 == 0 )
            return; // just "?", nothing to do
    }

    n2 = n1;
    // skipping trailing '+' or '*'
    while ( (szTextW[n1 - 1] == L'+') || (szTextW[n1 - 1] == L'*') )
    {
        --n1;
        if ( n2 - n1 > 1 )
        {
            // "++", "**", "+*" or "*+"
            // special case to preserve the first '+' or '*' from the two at the end
            szTextW[n1 + 1] = 0;
            return;
        }

        if ( n1 == 0 )
            return; // just "+(?)" or "*(?)", nothing to do
    }

    if ( n1 == n2 )
        return; // no '+' or '*' found, nothing to do

    n2 = n1;
    // skipping '\' before the trailing '+' or '*'
    while ( (n2 > 0) && (szTextW[n2 - 1] == L'\\') )
    {
        --n2;
    }

    if ( ((n1 - n2) % 2) != 0 )
        return; // '+' or '*' is escaped by '\', nothing to do

    // exclude trailing '+' or '*'
    szTextW[n1] = 0;
}

static void convertFindExToRegExA(const char* cszFindExA, char* pszRegExA)
{
    char ch;
    char next_ch;

    while ( (ch = *cszFindExA) != 0 )
    {
        switch ( ch )
        {
            case '\\':
                *(pszRegExA++) = '\\';
                next_ch = *(cszFindExA + 1);
                switch ( next_ch )
                {
                    case '\\':  // "\\" -> "\\"
                    case 'n':   // "\n" -> "\n"
                    case 't':   // "\t" -> "\t"
                    case '*':   // "\*" -> "\*"
                    case '?':   // "\?" -> "\?"
                        *(pszRegExA++) = next_ch;
                        ++cszFindExA;
                        break;

                    default:     // "\" -> "\\"
                        *(pszRegExA++) = '\\';
                        break;
                }
                break;

            case '*':  // "*" -> ".*?" (any 0 or more chars, not greedy)
                *(pszRegExA++) = '.';
                *(pszRegExA++) = '*';
                *(pszRegExA++) = '?';
                break;

            case '?':  // "?" -> "." (any single char)
                *(pszRegExA++) = '.';
                break;

            case '.':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '^':
            case '$':
            case '+':
            case '|':
                *(pszRegExA++) = '\\';
                *(pszRegExA++) = ch;
                break;

            default:
                *(pszRegExA++) = ch;
                break;
        }

        ++cszFindExA;
    }

    *pszRegExA = 0;
}

static void convertFindExToRegExW(const wchar_t* cszFindExW, wchar_t* pszRegExW)
{
    wchar_t wch;
    wchar_t next_wch;

    while ( (wch = *cszFindExW) != 0 )
    {
        switch ( wch )
        {
            case L'\\':
                *(pszRegExW++) = L'\\';
                next_wch = *(cszFindExW + 1);
                switch ( next_wch )
                {
                    case L'\\':  // "\\" -> "\\"
                    case L'n':   // "\n" -> "\n"
                    case L't':   // "\t" -> "\t"
                    case L'*':   // "\*" -> "\*"
                    case L'?':   // "\?" -> "\?"
                        *(pszRegExW++) = next_wch;
                        ++cszFindExW;
                        break;

                    default:     // "\" -> "\\"
                        *(pszRegExW++) = L'\\';
                        break;
                }
                break;

            case L'*':  // "*" -> ".*?" (any 0 or more chars, not greedy)
                *(pszRegExW++) = L'.';
                *(pszRegExW++) = L'*';
                *(pszRegExW++) = L'?';
                break;

            case L'?':  // "?" -> "." (any single char)
                *(pszRegExW++) = L'.';
                break;

            case L'.':
            case L'(':
            case L')':
            case L'[':
            case L']':
            case L'{':
            case L'}':
            case L'^':
            case L'$':
            case L'+':
            case L'|':
                *(pszRegExW++) = L'\\';
                *(pszRegExW++) = wch;
                break;

            default:
                *(pszRegExW++) = wch;
                break;
        }

        ++cszFindExW;
    }

    *pszRegExW = 0;
}

static void CALLBACK CountAllTimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    EnterCriticalSection(&csFindAllTimerId);
    if ( idEvent == nFindAllTimerId )
    {
        KillTimer(NULL, idEvent);
        nFindAllTimerId = 0;
    }
    LeaveCriticalSection(&csFindAllTimerId);

    PostMessage( g_QSearchDlg.hDlg, QSM_FINDALL, QS_FINDALL_COUNTONLY, 0 );
}

// searches for g_QSearchDlg.szFindTextW
void qsearchDoSearchText(HWND hEdit, DWORD dwParams, const DWORD dwOptFlags[], tQSFindAll* pFindAll)
{
    EDITINFO ei;
    HWND     hDlgItm;
    BOOL     bNotFound = FALSE;
    BOOL     bNotRegExp = FALSE;
    BOOL     bEOF = FALSE;
    BOOL     bNeedsFindAllCountOnly = FALSE;
    DWORD    dwSearchFlags = FR_DOWN;

    if ( dwParams & QSEARCH_FIRST )
    {
        // searching for the first time, clear the EOF flag
        qs_nEditEOF = 0;
    }
    else
    {
        // searching for the same text again
        if ( qs_bEditNotFound )
            if ( dwOptFlags[OPTF_SRCH_STOP_EOF] != STOP_EOF_WITHOUT_MSG )
                return;
    }

    if ( g_Plugin.bOldWindows )
    {
        if ( g_QSearchDlg.hStInfo )
        {
            if ( (g_Options.dwNewUI == QS_UI_NEW_02) &&
                 (g_Options.dwFindAllMode & QS_FINDALL_AUTO_COUNT_FLAG) &&
                 (0 != ((LPCSTR) g_QSearchDlg.szFindTextW)[0]) )
            {
                char szInfoTextA[128];
                int nLen;

                szInfoTextA[0] = 0;
                nLen = GetWindowTextA(g_QSearchDlg.hStInfo, szInfoTextA, 124);

                if ( szInfoTextA[0] == 0 )
                {
                    if ( (dwParams & QSEARCH_SEL) == 0 )
                        bNeedsFindAllCountOnly = TRUE;
                }
                else if ( dwParams & QSEARCH_FIRST )
                {
                    if ( (dwParams & QSEARCH_SEL) == 0 )
                        bNeedsFindAllCountOnly = TRUE;

                    if ( (nLen < 3) || (lstrcmpA(szInfoTextA + nLen - 3, "...") != 0) )
                    {
                        lstrcatA(szInfoTextA, "...");
                        SetWindowTextA(g_QSearchDlg.hStInfo, szInfoTextA);
                    }
                }
            }
            else
                SetWindowTextA(g_QSearchDlg.hStInfo, "");
        }

        if ( 0 == ((LPCSTR) g_QSearchDlg.szFindTextW)[0] )
        {
            qsearchDoSetNotFound(hEdit, FALSE, FALSE, FALSE);
            return;
        }
    }
    else
    {
        if ( g_QSearchDlg.hStInfo )
        {
            if ( (g_Options.dwNewUI == QS_UI_NEW_02) &&
                 (g_Options.dwFindAllMode & QS_FINDALL_AUTO_COUNT_FLAG) &&
                 (0 != ((LPCWSTR) g_QSearchDlg.szFindTextW)[0]) )
            {
                wchar_t szInfoTextW[128];
                int nLen;

                szInfoTextW[0] = 0;
                nLen = GetWindowTextW(g_QSearchDlg.hStInfo, szInfoTextW, 124);

                if ( szInfoTextW[0] == 0 )
                {
                    if ( (dwParams & QSEARCH_SEL) == 0 )
                        bNeedsFindAllCountOnly = TRUE;
                }
                else if ( dwParams & QSEARCH_FIRST )
                {
                    if ( (dwParams & QSEARCH_SEL) == 0 )
                        bNeedsFindAllCountOnly = TRUE;

                    if ( (nLen < 3) || (lstrcmpW(szInfoTextW + nLen - 3, L"...") != 0) )
                    {
                        lstrcatW(szInfoTextW, L"...");
                        SetWindowTextW(g_QSearchDlg.hStInfo, szInfoTextW);
                    }
                }
            }
            else
                SetWindowTextW(g_QSearchDlg.hStInfo, L"");
        }

        if ( 0 == ((LPCWSTR) g_QSearchDlg.szFindTextW)[0] )
        {
            qsearchDoSetNotFound(hEdit, FALSE, FALSE, FALSE);
            return;
        }
    }

    ei.hWndEdit = 0;
    SendMessage( g_Plugin.hMainWnd, AKD_GETEDITINFO, 0, (LPARAM) &ei );
    if ( !ei.hWndEdit )
    {
        return;
    }

    g_QSearchDlg.bIsQSearchingRightNow = TRUE;

    if ( ((dwParams & QSEARCH_SEL) != QSEARCH_SEL) ||
         dwOptFlags[OPTF_SRCH_SELFIND_PICKUP] )
    {
        // not SelFindNext or SelFindPrev
        // or srch_selfind_pickup = 1
        g_QSearchDlg.bQSearching = TRUE;
    }

    if ( !pFindAll )
    {
        if ( (dwParams & QSEARCH_SEL_FINDUP) ||
             (dwParams & QSEARCH_FINDUP) ||
             (((dwParams & QSEARCH_SEL) != QSEARCH_SEL) &&
              ((dwParams & QSEARCH_NOFINDUP) != QSEARCH_NOFINDUP) &&
              ((GetKeyState(VK_QS_FINDUP) & 0x80) == 0x80)) )
        {
            dwSearchFlags = FR_UP;
        }

        if ( ((dwParams & QSEARCH_SEL) != QSEARCH_SEL) &&
             ((dwParams & QSEARCH_NOFINDBEGIN) != QSEARCH_NOFINDBEGIN) &&
             ((dwParams & QSEARCH_FINDBEGIN) ||
              ((GetKeyState(VK_QS_FINDBEGIN) & 0x80) == 0x80)) )
        {
            dwSearchFlags |= FR_BEGINNING;
            qs_nEditEOF = 0;
        }
    }
    else // pFindAll
    {
        qs_nEditEOF = 0;
    }

    if ( hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_MATCHCASE) )
    {
        if ( SendMessage(hDlgItm, BM_GETCHECK, 0, 0) == BST_CHECKED )
            dwSearchFlags |= FR_MATCHCASE;
    }

    //if ( !dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] )
    {
        if ( hDlgItm = GetDlgItem(g_QSearchDlg.hDlg, IDC_CH_WHOLEWORD) )
        {
            if ( SendMessage(hDlgItm, BM_GETCHECK, 0, 0) == BST_CHECKED )
                dwSearchFlags |= FR_WHOLEWORD;
        }
    }

    // this forbids deactivating of the Edit control
    // when the text is typed and searched
    qs_bEditCanBeNonActive = FALSE;

    if ( dwParams & QSEARCH_FIRST )
    {
        if ( dwOptFlags[OPTF_SRCH_ONTHEFLY_MODE] && !pFindAll &&
             (dwParams & QSEARCH_NOSETSEL_FIRST) == 0 )
        {
            CHARRANGE_X cr = {0, 0};

            SendMessage( ei.hWndEdit, EM_EXGETSEL_X, 0, (LPARAM) &cr );
            cr.cpMax = cr.cpMin;
            SendMessage( ei.hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &cr );
        }
        if ( dwOptFlags[OPTF_SRCH_FROM_BEGINNING] && ((dwParams & QSEARCH_NOFINDBEGIN) != QSEARCH_NOFINDBEGIN) )
            dwSearchFlags |= FR_BEGINNING;
    }

    if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
    {
        dwSearchFlags |= FRF_REGEXP;
        if ( dwOptFlags[OPTF_SRCH_REGEXP_DOT_NEWLINE] == 0 )
        {
            dwSearchFlags |= FRF_REGEXPNONEWLINEDOT;
        }
    }

    if ( g_Plugin.bOldWindows )
    {
        TEXTFINDA tfA;
        int       srchEOF;
        BOOL      bSearchEx = FALSE;
        INT_X     iFindResult = -1;
        char      szFindTextA[MAX_TEXT_SIZE];

        //g_QSearchDlg.szFindTextW[0] = 0;
        //GetWindowTextA( hEdit, (LPSTR) g_QSearchDlg.szFindTextW, MAX_TEXT_SIZE - 1 );
        getTextToSearchA( (LPCSTR) g_QSearchDlg.szFindTextW, &bSearchEx, dwOptFlags, szFindTextA );
        if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
        {
            adjustIncompleteRegExA(szFindTextA, dwOptFlags);
        }

        if ( !pFindAll )
        {
            tfA.dwFlags = dwSearchFlags;
            tfA.pFindIt = szFindTextA;
            tfA.nFindItLen = -1;
            srchEOF = (dwSearchFlags & FR_UP) ? QSEARCH_EOF_UP : QSEARCH_EOF_DOWN;
            if ( ((qs_nEditEOF & srchEOF) == 0) || !IsWindowVisible(g_QSearchDlg.hDlg) )
            {
                if ( !bSearchEx )
                {
                    iFindResult = (INT_X) SendMessage( g_Plugin.hMainWnd,
                      AKD_TEXTFINDA, (WPARAM) ei.hWndEdit, (LPARAM) &tfA );
                }
                else
                {
                    iFindResult = doFindTextExA( ei.hWndEdit, &tfA ) ;
                }
            }
            if ( iFindResult < 0 )
            {
                BOOL bContinueSearch = FALSE;

                if ( (dwSearchFlags & FRF_REGEXP) && (iFindResult <= -100) )
                {
                    // RegExp syntax error
                    bNotRegExp = TRUE;
                    bNotFound = TRUE;
                    bEOF = TRUE;
                    qs_nEditEOF = 0;
                }
                else if ( dwOptFlags[OPTF_SRCH_STOP_EOF] == STOP_EOF_WITHOUT_MSG )
                {
                    bNotFound = TRUE;
                    bEOF = TRUE;
                    qs_nEditEOF = srchEOF;
                }
                else if ( (dwOptFlags[OPTF_SRCH_STOP_EOF] & 0x01) &&
                          ((qs_nEditEOF & srchEOF) == 0) &&
                          (dwParams & QSEARCH_NEXT) )
                {
                    HWND hWndFocused = GetFocus();

                    qs_nEditEOF = srchEOF;
                    if ( MessageBoxA(
                           g_Plugin.hMainWnd,
                           qsearchGetHintA(IDS_EOFREACHED),
                           "AkelPad (QSearch)",
                           MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1
                           ) == IDOK )
                    {
                        bContinueSearch = TRUE;
                    }
                    else
                    {
                        // When a Find Button loses focus, the value of qs_nEditEOF resets to 0.
                        // So we need to set the value of qs_nEditEOF here again.
                        qs_nEditEOF = srchEOF;
                    }

                    if ( IsWindowVisible(g_QSearchDlg.hDlg) && (hWndFocused != ei.hWndEdit) )
                        SetFocus(hEdit);
                    else
                        SetFocus(ei.hWndEdit);
                }
                else
                {
                    bContinueSearch = TRUE;
                }

                if ( bContinueSearch )
                {
                    bEOF = TRUE;

                    if ( (dwSearchFlags & FR_UP) == FR_UP )
                    {
                        INT_X       pos = 0;
                        CHARRANGE_X cr = {0, 0};

                        SendMessage( ei.hWndEdit, EM_EXGETSEL_X, 0, (LPARAM) &cr );
                        pos = cr.cpMin;
                        cr.cpMin = -1;
                        cr.cpMax = -1;
                        SendMessage( ei.hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &cr );
                        if ( tfA.dwFlags & FR_BEGINNING )
                        {
                            tfA.dwFlags -= FR_BEGINNING;
                        }
                        if ( !bSearchEx )
                        {
                            iFindResult = (INT_X) SendMessage( g_Plugin.hMainWnd,
                              AKD_TEXTFINDA, (WPARAM) ei.hWndEdit, (LPARAM) &tfA );
                        }
                        else
                        {
                            iFindResult = doFindTextExA( ei.hWndEdit, &tfA ) ;
                        }
                        if ( iFindResult < 0 )
                        {
                            cr.cpMin = pos;
                            cr.cpMax = pos;
                            SendMessage( ei.hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &cr );
                            bNotFound = TRUE;
                        }
                    }
                    else
                    {
                        if ( ((dwSearchFlags & FR_BEGINNING) != FR_BEGINNING) ||
                             (qs_nEditEOF & srchEOF) )
                        {
                            tfA.dwFlags = dwSearchFlags | FR_BEGINNING;
                            if ( !bSearchEx )
                            {
                                iFindResult = (INT_X) SendMessage( g_Plugin.hMainWnd,
                                  AKD_TEXTFINDA, (WPARAM) ei.hWndEdit, (LPARAM) &tfA );
                            }
                            else
                            {
                                iFindResult = doFindTextExA( ei.hWndEdit, &tfA ) ;
                            }
                            if ( iFindResult < 0 )
                            {
                                bNotFound = TRUE;
                            }
                        }
                        else
                        {
                            bNotFound = TRUE;
                        }
                    }
                    qs_nEditEOF = 0;
                }
            }
            else
            {
                qs_nEditEOF = 0;
            }
        }
        else // pFindAll
        {
            MessageBoxA(
              g_Plugin.hMainWnd,
              "Not supported in non-Unicode version",
              "AkelPad (QSearch)",
              MB_OK | MB_ICONEXCLAMATION
            );
        }
    }
    else // !g_Plugin.bOldWindows
    {
        TEXTFINDW tfW;
        int       srchEOF;
        BOOL      bSearchEx = FALSE;
        INT_X     iFindResult = -1;
        wchar_t   szFindTextW[MAX_TEXT_SIZE];

        //g_QSearchDlg.szFindTextW[0] = 0;
        //GetWindowTextW( hEdit, (LPWSTR) g_QSearchDlg.szFindTextW, MAX_TEXT_SIZE - 1 );
        getTextToSearchW( g_QSearchDlg.szFindTextW, &bSearchEx, dwOptFlags, szFindTextW );
        if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
        {
            adjustIncompleteRegExW(szFindTextW, dwOptFlags);
        }

        if ( !pFindAll )
        {
            tfW.dwFlags = dwSearchFlags;
            tfW.pFindIt = szFindTextW;
            tfW.nFindItLen = -1;
            srchEOF = (dwSearchFlags & FR_UP) ? QSEARCH_EOF_UP : QSEARCH_EOF_DOWN;
            if ( ((qs_nEditEOF & srchEOF) == 0) || !IsWindowVisible(g_QSearchDlg.hDlg) )
            {
                if ( !bSearchEx )
                {
                    iFindResult = (INT_X) SendMessage( g_Plugin.hMainWnd,
                      AKD_TEXTFINDW, (WPARAM) ei.hWndEdit, (LPARAM) &tfW );
                }
                else
                {
                    iFindResult = doFindTextExW( ei.hWndEdit, &tfW ) ;
                }
            }
            if ( iFindResult < 0 )
            {
                BOOL bContinueSearch = FALSE;

                if ( (dwSearchFlags & FRF_REGEXP) && (iFindResult <= -100) )
                {
                    // RegExp syntax error
                    bNotRegExp = TRUE;
                    bNotFound = TRUE;
                    bEOF = TRUE;
                    qs_nEditEOF = 0;
                }
                else if ( dwOptFlags[OPTF_SRCH_STOP_EOF] == STOP_EOF_WITHOUT_MSG )
                {
                    bNotFound = TRUE;
                    bEOF = TRUE;
                    qs_nEditEOF = srchEOF;
                }
                else if ( (dwOptFlags[OPTF_SRCH_STOP_EOF] & 0x01) &&
                          ((qs_nEditEOF & srchEOF) == 0) &&
                          (dwParams & QSEARCH_NEXT) )
                {
                    HWND hWndFocused = GetFocus();

                    qs_nEditEOF = srchEOF;
                    if ( MessageBoxW(
                           g_Plugin.hMainWnd,
                           qsearchGetHintW(IDS_EOFREACHED),
                           L"AkelPad (QSearch)",
                           MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1
                           ) == IDOK )
                    {
                        bContinueSearch = TRUE;
                    }
                    else
                    {
                        // When a Find Button loses focus, the value of qs_nEditEOF resets to 0.
                        // So we need to set the value of qs_nEditEOF here again.
                        qs_nEditEOF = srchEOF;
                    }

                    if ( IsWindowVisible(g_QSearchDlg.hDlg) && (hWndFocused != ei.hWndEdit) )
                        SetFocus(hEdit);
                    else
                        SetFocus(ei.hWndEdit);
                }
                else
                {
                    bContinueSearch = TRUE;
                }

                if ( bContinueSearch )
                {
                    bEOF = TRUE;

                    if ( (dwSearchFlags & FR_UP) == FR_UP )
                    {
                        INT_X       pos = 0;
                        CHARRANGE_X cr = {0, 0};

                        SendMessage( ei.hWndEdit, EM_EXGETSEL_X, 0, (LPARAM) &cr );
                        pos = cr.cpMin;
                        cr.cpMin = -1;
                        cr.cpMax = -1;
                        SendMessage( ei.hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &cr );
                        if ( tfW.dwFlags & FR_BEGINNING )
                        {
                            tfW.dwFlags -= FR_BEGINNING;
                        }
                        if ( !bSearchEx )
                        {
                            iFindResult = (INT_X) SendMessage( g_Plugin.hMainWnd,
                              AKD_TEXTFINDW, (WPARAM) ei.hWndEdit, (LPARAM) &tfW );
                        }
                        else
                        {
                            iFindResult = doFindTextExW( ei.hWndEdit, &tfW );
                        }
                        if ( iFindResult < 0 )
                        {
                            cr.cpMin = pos;
                            cr.cpMax = pos;
                            SendMessage( ei.hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &cr );
                            bNotFound = TRUE;
                        }
                    }
                    else
                    {
                        if ( ((dwSearchFlags & FR_BEGINNING) != FR_BEGINNING) ||
                             (qs_nEditEOF & srchEOF) )
                        {
                            tfW.dwFlags = dwSearchFlags | FR_BEGINNING;
                            if ( !bSearchEx )
                            {
                                iFindResult = (INT_X) SendMessage( g_Plugin.hMainWnd,
                                  AKD_TEXTFINDW, (WPARAM) ei.hWndEdit, (LPARAM) &tfW );
                            }
                            else
                            {
                                iFindResult = doFindTextExW( ei.hWndEdit, &tfW );
                            }
                            if ( iFindResult < 0 )
                            {
                                bNotFound = TRUE;
                            }
                        }
                        else
                        {
                            bNotFound = TRUE;
                        }
                    }
                    qs_nEditEOF = 0;
                }
            }
            else
            {
                qs_nEditEOF = 0;
            }
        }
        else // pFindAll
        {
            const FRAMEDATA*     pFrameInitial;   // all files
            HWND                 hWndEdit;
            unsigned int         nCurrentMatches; // current (initial) file
            BOOL                 bAllFiles;
            tFindAllContext      FindContext;
            AEFINDTEXTW          aeftW;
            wchar_t              szFindAllW[2*MAX_TEXT_SIZE];

            pFrameInitial = NULL;
            nCurrentMatches = 0;
            bAllFiles = FALSE;

            aeftW.dwFlags = AEFR_DOWN;
            if ( dwSearchFlags & FR_MATCHCASE )
                aeftW.dwFlags |= AEFR_MATCHCASE;
            if ( dwSearchFlags & FR_WHOLEWORD )
                aeftW.dwFlags |= AEFR_WHOLEWORD;
            if ( bSearchEx )
            {
                aeftW.dwFlags |= (AEFR_REGEXP | AEFR_REGEXPNONEWLINEDOT);
            }
            else if ( dwSearchFlags & FRF_REGEXP )
            {
                aeftW.dwFlags |= AEFR_REGEXP;
                if ( dwSearchFlags & FRF_REGEXPNONEWLINEDOT )
                    aeftW.dwFlags |= AEFR_REGEXPNONEWLINEDOT;
            }

            if ( bSearchEx )
                convertFindExToRegExW(szFindTextW, szFindAllW);
            else
                lstrcpyW(szFindAllW, szFindTextW);
            aeftW.pText = szFindAllW;
            aeftW.dwTextLen = lstrlenW(szFindAllW);
            aeftW.nNewLine = AELB_ASIS;

            FindContext.cszFindWhat = szFindTextW;
            FindContext.pFindTextW = &aeftW;
            FindContext.pFrame = NULL;
            FindContext.dwFindAllMode = g_Options.dwFindAllMode;
            FindContext.dwFindAllFlags = 0;
            FindContext.dwFindAllResult = g_Options.dwFindAllResult;
            FindContext.nOccurrences = 0;
            FindContext.nTotalOccurrences = 0;
            FindContext.nTotalFiles = 0;
            FindContext.nFilesWithOccurrences = 0;
            tDynamicBuffer_Init( &FindContext.ResultsBuf );
            tDynamicBuffer_Init( &FindContext.OccurrencesBuf );
            tDynamicBuffer_Allocate( &FindContext.ResultsBuf, 128*1024*sizeof(wchar_t) );
            tDynamicBuffer_Allocate( &FindContext.OccurrencesBuf, 128*1024*sizeof(wchar_t) );

            if ( bSearchEx )
                FindContext.dwFindAllFlags |= QS_FAF_SPECCHAR;
            else if ( dwSearchFlags & FRF_REGEXP )
                FindContext.dwFindAllFlags |= QS_FAF_REGEXP;
            if ( dwSearchFlags & FR_MATCHCASE )
                FindContext.dwFindAllFlags |= QS_FAF_MATCHCASE;
            if ( dwSearchFlags & FR_WHOLEWORD )
                FindContext.dwFindAllFlags |= QS_FAF_WHOLEWORD;

            if ( g_Plugin.nMDI == WMD_SDI )
            {
                if ( FindContext.dwFindAllResult & QS_FINDALL_RSLT_ALLFILES )
                    FindContext.dwFindAllResult ^= QS_FINDALL_RSLT_ALLFILES;
            }
            else // MDI, PMDI
            {
                if ( dwParams & QSEARCH_FINDALLFILES )
                    FindContext.dwFindAllResult |= QS_FINDALL_RSLT_ALLFILES;
            }

            if ( (FindContext.dwFindAllResult & QS_FINDALL_RSLT_ALLFILES) &&
                 (pFindAll->pfnFindResultCallback != NULL) ) // not CountOnly
            {
                bAllFiles = TRUE;
            }

            FindContext.pFrame = (FRAMEDATA *) SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0);
            if ( bAllFiles )
            {
                pFrameInitial = FindContext.pFrame;

                FindContext.nTotalFiles = (unsigned int) SendMessage(g_Plugin.hMainWnd, AKD_FRAMESTATS, FWS_COUNTALL, 0);
                if ( ((g_Options.dwFindAllMode & QS_FINDALL_MASK) == QS_FINDALL_FILEOUTPUT_SNGL) ||
                     ((g_Options.dwFindAllMode & QS_FINDALL_MASK) == QS_FINDALL_FILEOUTPUT_MULT) )
                {
                    const FRAMEDATA* pFr;

                    pFr = pFrameInitial;
                    for ( ; ; )
                    {
                        if ( pFr == g_QSearchDlg.pSearchResultsFrame )
                        {
                            --FindContext.nTotalFiles;
                            break;
                        }

                        pFr = (FRAMEDATA *) SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEFIND, FWF_NEXT, (LPARAM) pFr);
                        if ( pFr == pFrameInitial )
                            break;
                    }
                }
                pFindAll->ShowFindResults.pfnAllFilesInit(&FindContext, &pFindAll->tempBuf);
            }

            for ( ; ; )
            {
                if ( bAllFiles && FindContext.pFrame == g_QSearchDlg.pSearchResultsFrame )
                {
                    // skip the pSearchResultsFrame
                    FindContext.pFrame = (FRAMEDATA *) SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEFIND, FWF_NEXT, (LPARAM) FindContext.pFrame);
                    if ( FindContext.pFrame != pFrameInitial )
                    {
                        if ( g_Plugin.nMDI == WMD_PMDI )
                        {
                            SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM) FindContext.pFrame);
                        }
                    }
                    else
                        break;
                }

                hWndEdit = FindContext.pFrame->ei.hWndEdit;
                SendMessageW( hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM) &aeftW.crSearch.ciMin );
                SendMessageW( hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM) &aeftW.crSearch.ciMax);

                FindContext.nOccurrences = 0;

                pFindAll->ShowFindResults.pfnInit(&FindContext, &pFindAll->tempBuf);

                while ( SendMessageW(hWndEdit, AEM_FINDTEXTW, 0, (LPARAM) &aeftW) )
                {
                    ++FindContext.nOccurrences;
                    ++FindContext.nTotalOccurrences;

                    if ( pFindAll->pfnFindResultCallback )
                        pFindAll->pfnFindResultCallback(&FindContext, &aeftW.crFound, &pFindAll->GetFindResultPolicy, &pFindAll->tempBuf, &pFindAll->tempBuf2, pFindAll->ShowFindResults.pfnAddOccurrence);

                    x_mem_cpy( &aeftW.crSearch.ciMin, &aeftW.crFound.ciMax, sizeof(AECHARINDEX) );
                }

                if ( FindContext.nOccurrences != 0 )
                {
                    ++FindContext.nFilesWithOccurrences;
                }

                if ( pFrameInitial == FindContext.pFrame )
                {
                    nCurrentMatches = FindContext.nOccurrences;
                }

                pFindAll->ShowFindResults.pfnDone(&FindContext, &pFindAll->tempBuf);

                if ( bAllFiles )
                {
                    FindContext.pFrame = (FRAMEDATA *) SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEFIND, FWF_NEXT, (LPARAM) FindContext.pFrame);
                    if ( FindContext.pFrame != pFrameInitial )
                    {
                        if ( g_Plugin.nMDI == WMD_PMDI )
                        {
                            SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM) FindContext.pFrame);
                        }
                    }
                    else
                        break;
                }
                else
                {
                    tDynamicBuffer_Clear(&FindContext.ResultsBuf);
                    tDynamicBuffer_Clear(&FindContext.OccurrencesBuf);
                    break;
                }
            }

            if ( bAllFiles )
            {
                if ( g_Plugin.nMDI == WMD_PMDI )
                {
                    SendMessageW(g_Plugin.hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM) pFrameInitial);
                }

                bNeedsFindAllCountOnly = FALSE;

                if ( pFrameInitial != g_QSearchDlg.pSearchResultsFrame )
                    qsSetInfoOccurrencesFound(nCurrentMatches);
                else
                    qsSetInfoEmpty();

                pFindAll->ShowFindResults.pfnAllFilesDone(&FindContext, &pFindAll->tempBuf);
                tDynamicBuffer_Clear(&FindContext.ResultsBuf);
                tDynamicBuffer_Clear(&FindContext.OccurrencesBuf);
            }

            tDynamicBuffer_Free(&pFindAll->tempBuf);
            tDynamicBuffer_Free(&pFindAll->tempBuf2);

            tDynamicBuffer_Free(&FindContext.ResultsBuf);
            tDynamicBuffer_Free(&FindContext.OccurrencesBuf);

            if ( FindContext.nTotalOccurrences == 0 )
            {
                bNotFound = TRUE;
                if ( (dwSearchFlags & FRF_REGEXP) && (aeftW.nCompileErrorOffset > 0) )
                {
                    bNotRegExp = TRUE;
                    bEOF = TRUE;
                    qs_nEditEOF = 0;
                }
                else
                {
                    bEOF = TRUE;
                    qs_nEditEOF = QSEARCH_EOF_DOWN | QSEARCH_EOF_UP;
                }
            }
        }
    }

    if ( (!bNotFound) && (!qs_nEditEOF) )
    {
        if ( !pFindAll )
        {
            int         nLine;
            int         nLine1st;
            CHARRANGE_X cr = { 0, 0 };

            SendMessage( ei.hWndEdit, EM_EXGETSEL_X, 0, (LPARAM) &cr );
            nLine = (int) SendMessage(ei.hWndEdit, EM_EXLINEFROMCHAR, 0, cr.cpMin);
            nLine1st = (int) SendMessage(ei.hWndEdit, EM_GETFIRSTVISIBLELINE, 0, 0);
            if ( nLine < nLine1st )
            {
                SendMessage(ei.hWndEdit, EM_LINESCROLL, 0, nLine - nLine1st);
            }

            if ( (dwParams & QSEARCH_NEXT) ||
                 ((dwParams & QSEARCH_FIRST) && !dwOptFlags[OPTF_SRCH_ONTHEFLY_MODE]) )
            {
                UINT uUpdFlags = UFHF_MOVE_TO_TOP_IF_EXISTS;
                if ( (dwParams & QSEARCH_SEL) && !dwOptFlags[OPTF_SRCH_SELFIND_PICKUP] )
                    uUpdFlags |= UFHF_KEEP_EDIT_TEXT;
                qsearchFindHistoryAdd(hEdit, g_QSearchDlg.szFindTextW, uUpdFlags);
            }
        }
        else if ( dwParams & QSEARCH_FINDALL )
        {
            UINT uUpdFlags = UFHF_MOVE_TO_TOP_IF_EXISTS;
            if ( (dwParams & QSEARCH_SEL) && !dwOptFlags[OPTF_SRCH_SELFIND_PICKUP] )
                uUpdFlags |= UFHF_KEEP_EDIT_TEXT;
            qsearchFindHistoryAdd(hEdit, g_QSearchDlg.szFindTextW, uUpdFlags);
        }
    }

    // now the Edit control can be deactivated
    qs_bEditCanBeNonActive = TRUE;
    qsearchDoSetNotFound(hEdit, bNotFound, bNotRegExp, bEOF);

    g_QSearchDlg.bIsQSearchingRightNow = FALSE;

    if ( bNeedsFindAllCountOnly && !pFindAll && IsWindowVisible(g_QSearchDlg.hDlg) )
    {
        UINT nDelayMs;
        UINT_PTR nTimerId;

        if ( (dwParams & (QSEARCH_FIRST | QSEARCH_USEDELAY)) == (QSEARCH_FIRST | QSEARCH_USEDELAY) )
            nDelayMs = g_Options.dwFindAllCountDelay;
        else
            nDelayMs = 0;

        if ( nDelayMs != 0 )
        {
            nTimerId = SetTimer(NULL, 0, nDelayMs, CountAllTimerProc);
        }
        else
        {
            nTimerId = 0;
        }

        EnterCriticalSection(&csFindAllTimerId);
        if ( nFindAllTimerId != 0 )
        {
            KillTimer(NULL, nFindAllTimerId);
        }
        nFindAllTimerId = nTimerId;
        LeaveCriticalSection(&csFindAllTimerId);

        if ( nDelayMs == 0 )
        {
            PostMessage( g_QSearchDlg.hDlg, QSM_FINDALL, QS_FINDALL_COUNTONLY, 0 );
        }
    }
}

void qsearchDoTryHighlightAll(HWND hDlg, const DWORD dwOptFlags[], DWORD dwHighlightCondition)
{
    if ( g_Plugin.bOldWindows )
    {
        if ( 0 == ((LPCSTR) g_QSearchDlg.szFindTextW)[0] )
        {
            // no text to search for: removing the highlight
            qsearchDoTryUnhighlightAll();
            return;
        }
    }
    else
    {
        if ( 0 == ((LPCWSTR) g_QSearchDlg.szFindTextW)[0] )
        {
            // no text to search for: removing the highlight
            qsearchDoTryUnhighlightAll();
            return;
        }
    }

    if ( g_bHighlightPlugin && !qs_bEditNotFound )
    {
        if ( (dwHighlightCondition == QHC_ALWAYS) ||
             isCheckBoxChecked(hDlg, IDC_CH_HIGHLIGHTALL) )
        {
            EDITINFO  ei;

            ei.hWndEdit = NULL;
            SendMessage( g_Plugin.hMainWnd,
            AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei );

            if ( ei.hWndEdit )
            {
                CHARRANGE_X cr = { 0, 0 };

                SendMessage( ei.hWndEdit, EM_EXGETSEL_X, 0, (LPARAM) &cr );
                if ( cr.cpMin != cr.cpMax )
                {
                    DLLECHIGHLIGHT_MARK hlParams;
                    wchar_t szTextColor[16];
                    wchar_t szBkgndColor[16];
                    wchar_t szMarkTextBufW[MAX_TEXT_SIZE + 4]; // plus room for the leading & trailing "\\b"

                    if ( g_Plugin.bOldWindows )
                    {
                        wsprintfA( (char *) szTextColor, "0" );
                        wsprintfA( (char *) szBkgndColor, "#%02X%02X%02X",
                          GetRValue(g_Options.colorHighlight),
                          GetGValue(g_Options.colorHighlight),
                          GetBValue(g_Options.colorHighlight)
                        );
                    }
                    else
                    {
                        wsprintfW( szTextColor, L"0" );
                        wsprintfW( szBkgndColor, L"#%02X%02X%02X",
                          GetRValue(g_Options.colorHighlight),
                          GetGValue(g_Options.colorHighlight),
                          GetBValue(g_Options.colorHighlight)
                        );
                    }

                    hlParams.dwStructSize = sizeof(DLLECHIGHLIGHT_MARK);
                    hlParams.nAction = DLLA_HIGHLIGHT_MARK;
                    hlParams.pColorText = (unsigned char *) szTextColor;
                    hlParams.pColorBk = (unsigned char *) szBkgndColor;
                    hlParams.dwMarkFlags = 0;
                    hlParams.dwFontStyle = 0;
                    hlParams.dwMarkID = g_Options.dwHighlightMarkID;
                    hlParams.wszMarkText = NULL;

                    if ( isCheckBoxChecked(hDlg, IDC_CH_MATCHCASE) )
                        hlParams.dwMarkFlags |= MARKFLAG_MATCHCASE;

                    if ( isCheckBoxChecked(hDlg, IDC_CH_WHOLEWORD) )
                        hlParams.dwMarkFlags |= MARKFLAG_WHOLEWORD;

#if AKELPAD_RUNTIME_VERSION_CHECK
                    if ( VersionCompare(getProgramVersion(&g_Plugin), MAKE_IDENTIFIER(4, 8, 8, 0)) >= 0 )
#endif
                    {
                        BOOL bFindExAsRegExp = FALSE;

                        if ( dwOptFlags[OPTF_SRCH_USE_SPECIALCHARS] && !dwOptFlags[OPTF_SRCH_USE_REGEXP] )
                        {
                            if ( g_Plugin.bOldWindows )
                            {
                                if ( findSpecialCharA((LPCSTR) g_QSearchDlg.szFindTextW) != -1 )
                                    bFindExAsRegExp = TRUE;
                            }
                            else
                            {
                                if ( findSpecialCharW(g_QSearchDlg.szFindTextW) != -1 )
                                    bFindExAsRegExp = TRUE;
                            }
                        }

                        if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] || bFindExAsRegExp )
                        {
                            wchar_t* pszMarkTextW;
                            int nLen;

                            pszMarkTextW = szMarkTextBufW + 2;
                            pszMarkTextW[0] = 0;
                            if ( g_Plugin.bOldWindows )
                            {
                                const char* cszFindTextA;
                                char szRegExBufA[2*MAX_TEXT_SIZE + 4];

                                if ( bFindExAsRegExp )
                                {
                                    convertFindExToRegExA( (LPCSTR) g_QSearchDlg.szFindTextW, szRegExBufA );
                                    cszFindTextA = szRegExBufA;
                                }
                                else
                                    cszFindTextA = (LPCSTR) g_QSearchDlg.szFindTextW;

                                MultiByteToWideChar( CP_ACP, 0, cszFindTextA, -1, pszMarkTextW, MAX_TEXT_SIZE - 1 );
                            }
                            else
                            {
                                if ( bFindExAsRegExp )
                                    convertFindExToRegExW( g_QSearchDlg.szFindTextW, pszMarkTextW );
                                else
                                    lstrcpyW( pszMarkTextW, g_QSearchDlg.szFindTextW );
                            }

                            if ( dwOptFlags[OPTF_SRCH_USE_REGEXP] )
                            {
                                adjustIncompleteRegExW( pszMarkTextW, dwOptFlags );
                            }

                            if ( pszMarkTextW[0] != 0 )
                            {
                                hlParams.dwMarkFlags |= MARKFLAG_REGEXP;
                                if ( hlParams.dwMarkFlags & MARKFLAG_WHOLEWORD )
                                {
                                    // When MARKFLAG_REGEXP is set in dwMarkFlags,
                                    // MARKFLAG_WHOLEWORD seems to be ignored...
                                    hlParams.dwMarkFlags ^= MARKFLAG_WHOLEWORD;
                                    nLen = lstrlenW(pszMarkTextW);
                                    if ( !x_wstr_endswith(pszMarkTextW, nLen, L"\\b", 2) )
                                    {
                                        // the trailing "\\b"
                                        pszMarkTextW[nLen] = L'\\'; ++nLen;
                                        pszMarkTextW[nLen] = L'b'; ++nLen;
                                        pszMarkTextW[nLen] = 0;
                                    }
                                    if ( !x_wstr_startswith(pszMarkTextW, L"\\b") )
                                    {
                                        // the leading "\\b"
                                        pszMarkTextW = szMarkTextBufW;
                                        pszMarkTextW[0] = L'\\';
                                        pszMarkTextW[1] = L'b';
                                    }
                                }
                                hlParams.wszMarkText = pszMarkTextW;
                            }
                        }
                    }

                    CallHighlightMain( &hlParams );
                }
                else
                    qsearchDoTryUnhighlightAll(); // no selection
            }
        }
    }
    else
        qsearchDoTryUnhighlightAll(); // not found
}

void qsearchDoTryUnhighlightAll(void)
{
    if ( g_bHighlightPlugin )
    {
        DLLECHIGHLIGHT_UNMARK hlParams;

        hlParams.dwStructSize = sizeof(DLLECHIGHLIGHT_UNMARK);
        hlParams.nAction = DLLA_HIGHLIGHT_UNMARK;
        hlParams.dwMarkID = g_Options.dwHighlightMarkID;

        CallHighlightMain( &hlParams );
    }
}

HWND qsearchGetFindEdit(HWND hDlg)
{
    HWND hEdit;

    if ( qsearchIsFindHistoryEnabled() )
    {
        HWND  hCombo;
        POINT pt;

        hCombo = GetDlgItem(hDlg, IDC_CB_FINDTEXT);
        pt.x = 5;
        pt.y = 5;
        hEdit = ChildWindowFromPoint(hCombo, pt);
    }
    else
    {
        hEdit = GetDlgItem(hDlg, IDC_ED_FINDTEXT);
    }

    return hEdit;
}

BOOL qsearchFindHistoryAdd(HWND hEdit, const wchar_t* cszTextAW, UINT uUpdFlags)
{
    if ( qsearchIsFindHistoryEnabled() && cszTextAW && ((const char *)cszTextAW)[0] )
    {
        int   iItem;
        DWORD dwEditSel;
        HWND  hCombo;

        hCombo = GetDlgItem(g_QSearchDlg.hDlg, IDC_CB_FINDTEXT);
        if ( g_Plugin.bOldWindows )
        {
            dwEditSel = (DWORD) SendMessageA(hCombo, CB_GETEDITSEL, 0, 0);
            iItem = (int) SendMessageA(hCombo, CB_FINDSTRINGEXACT, (WPARAM) (-1), (LPARAM) cszTextAW);
            if ( iItem != 0 )
            {
                if ( (uUpdFlags & UFHF_MOVE_TO_TOP_IF_EXISTS) || (iItem == CB_ERR) )
                {
                    while ( iItem != CB_ERR )
                    {
                        SendMessageA(hCombo, CB_DELETESTRING, iItem, 0);
                        iItem = (int) SendMessageA(hCombo, CB_FINDSTRINGEXACT, (WPARAM) (-1), (LPARAM) cszTextAW);
                    }

                    iItem = (int) SendMessageA(hCombo, CB_GETCOUNT, 0, 0);
                    if ( iItem > (int) (g_Options.dwFindHistoryItems - 1) )
                    {
                        iItem = (int) (g_Options.dwFindHistoryItems - 1);
                        if ( iItem == (int) SendMessageW(hCombo, CB_GETCURSEL, 0, 0) )
                            --iItem; // do not remove the current item
                        if ( iItem >= 0 )
                            SendMessageA(hCombo, CB_DELETESTRING, g_Options.dwFindHistoryItems - 1, 0);
                    }

                    if ( SendMessageA(hCombo, CB_INSERTSTRING, 0, (LPARAM) cszTextAW) >= 0 )
                    {
                        if ( !(uUpdFlags & UFHF_KEEP_EDIT_TEXT) )
                        {
                            SendMessageA(hEdit, WM_SETREDRAW, FALSE, 0);
                            SendMessageA(hCombo, CB_SETCURSEL, 0, 0);
                            SendMessageA(hEdit, WM_SETREDRAW, TRUE, 0);
                            //editSetTrailSel(hEdit);
                            SendMessageA(hCombo, CB_SETEDITSEL, 0, dwEditSel);
                        }
                        return TRUE;
                    }
                }
                /*else
                {
                    SendMessageA(hEdit, WM_SETREDRAW, FALSE, 0);
                    SendMessageA(hCombo, CB_SETCURSEL, iItem, 0);
                    SendMessageA(hEdit, WM_SETREDRAW, TRUE, 0);
                    //editSetTrailSel(hEdit);
                    SendMessageA(hCombo, CB_SETEDITSEL, 0, dwEditSel);
                }*/
            }
        }
        else if ( cszTextAW[0] )
        {
            dwEditSel = (DWORD) SendMessageW(hCombo, CB_GETEDITSEL, 0, 0);
            iItem = (int) SendMessageW(hCombo, CB_FINDSTRINGEXACT, (WPARAM) (-1), (LPARAM) cszTextAW);
            if ( iItem != 0 )
            {
                if ( (uUpdFlags & UFHF_MOVE_TO_TOP_IF_EXISTS) || (iItem == CB_ERR) )
                {
                    while ( iItem != CB_ERR )
                    {
                        SendMessageW(hCombo, CB_DELETESTRING, iItem, 0);
                        iItem = (int) SendMessageW(hCombo, CB_FINDSTRINGEXACT, (WPARAM) (-1), (LPARAM) cszTextAW);
                    }

                    iItem = (int) SendMessageW(hCombo, CB_GETCOUNT, 0, 0);
                    if ( iItem > (int) (g_Options.dwFindHistoryItems - 1) )
                    {
                        iItem = (int) (g_Options.dwFindHistoryItems - 1);
                        if ( iItem == (int) SendMessageW(hCombo, CB_GETCURSEL, 0, 0) )
                            --iItem; // do not remove the current item
                        if ( iItem >= 0 )
                            SendMessageW(hCombo, CB_DELETESTRING, iItem, 0);
                    }

                    if ( SendMessageW(hCombo, CB_INSERTSTRING, 0, (LPARAM) cszTextAW) >= 0 )
                    {
                        if ( !(uUpdFlags & UFHF_KEEP_EDIT_TEXT) )
                        {
                            SendMessageW(hEdit, WM_SETREDRAW, FALSE, 0);
                            SendMessageW(hCombo, CB_SETCURSEL, 0, 0);
                            SendMessageW(hEdit, WM_SETREDRAW, TRUE, 0);
                            //editSetTrailSel(hEdit);
                            SendMessageW(hCombo, CB_SETEDITSEL, 0, dwEditSel);
                        }
                        return TRUE;
                    }
                }
                /*else
                {
                    SendMessageW(hEdit, WM_SETREDRAW, FALSE, 0);
                    SendMessageW(hCombo, CB_SETCURSEL, iItem, 0);
                    SendMessageW(hEdit, WM_SETREDRAW, TRUE, 0);
                    //editSetTrailSel(hEdit);
                    SendMessageW(hCombo, CB_SETEDITSEL, 0, dwEditSel);
                }*/
            }
        }
    }

    return FALSE;
}
