#ifndef _akel_qsearch_h_
#define _akel_qsearch_h_
//---------------------------------------------------------------------------
#include <windows.h>
#include <richedit.h>
#include <TCHAR.h>
#include "AkelDllHeader.h"
#include "QSearch_defs.h"

// When EXE_MIN_VERSION_4X is set to (4, 9, 1, 0)
// there's no sense to check for (4, 8, 8, 0) in runtime
#define AKELPAD_RUNTIME_VERSION_CHECK 0


/* >>>>>>>>>>>>>>>>>>>>>>>> plugin state >>>>>>>>>>>>>>>>>>>>>>>> */
    typedef struct tPluginState {
        BOOL         bInitialized;
        BOOL         bOldWindows;
        BOOL         bOldRichEdit; // TRUE means Rich Edit 2.0
        int          nMDI;
        HINSTANCE    hInstanceDLL;
        HWND         hMainWnd;
        BOOL         bAkelEdit;
        BOOL         bAkelPadOnFinish;
        int          nDockedDlgIsWaitingForOnStart;
        LANGID       wLangSystem;
        WNDPROCDATA* pEditProcData;
        WNDPROCDATA* pMainProcData;
        WNDPROCDATA* pFrameProcData;
        DWORD        dwProgramVersion__; // use it through getProgramVersion()!
    } PluginState;

    void initializePluginState(PluginState* pPlugin);
    DWORD getProgramVersion(PluginState* pPlugin);
/* <<<<<<<<<<<<<<<<<<<<<<<< plugin state <<<<<<<<<<<<<<<<<<<<<<<< */


/* >>>>>>>>>>>>>>>>>>>>>>>> qsearch options >>>>>>>>>>>>>>>>>>>>>>>> */
    typedef struct tFindResultsOutputPolicy {
        int nMode;   // see tGetFindResultPolicy.nMode
        int nBefore; // see tGetFindResultPolicy.nBefore
        int nAfter;  // see tGetFindResultPolicy.nAfter
        int nHighlight; // one of QSFRH_*
    } FindResultsOutputPolicy;

    void initializeFRP(FindResultsOutputPolicy* pFRP);
    BOOL equalFRP(const FindResultsOutputPolicy* pFRP1, const FindResultsOutputPolicy* pFRP2);

    typedef struct tQSearchOpt {
        DWORD    dwFlags[OPTF_COUNT];
        RECT     dockRect;
        COLORREF colorNotFound;
        COLORREF colorNotRegExp;
        COLORREF colorEOF;
        COLORREF colorHighlight;
        DWORD    dwHighlightMarkID;
        DWORD    dwHighlightState;
        DWORD    dwUseAltHotkeys;
        DWORD    dwAltMatchCase;
        DWORD    dwAltWholeWord;
        DWORD    dwAltSearchMode;
        DWORD    dwAltHighlightAll;
        DWORD    dwFindHistoryItems; // QSM_UPDATEUI, QS_UI_FIND, 0
        DWORD    dwHistorySave;
        DWORD    dwNewUI;
        DWORD    dwSelectByF3;
        DWORD    dwSelectByFnd;
        DWORD    dwSelectBySelFnd;
        DWORD    dwAdjIncomplRegExp;
        DWORD    dwFindAllMode;
        DWORD    dwFindAllResult;
        DWORD    dwFindAllCountDelay;
        DWORD    dwEditMinWidth;
        DWORD    dwEditMaxWidth;
        FindResultsOutputPolicy LogOutputFRP;
        FindResultsOutputPolicy FileOutputFRP;
    } QSearchOpt;

    void initializeOptions(QSearchOpt* pOptions);
    void copyOptionsFlags(DWORD dwOptFlagsDst[], const DWORD dwOptFlagsSrc[]);
    void copyOptions(QSearchOpt* pOptDst, const QSearchOpt* pOptSrc);
    BOOL equalOptions(const QSearchOpt* pOpt1, const QSearchOpt* pOpt2);
/* <<<<<<<<<<<<<<<<<<<<<<<< qsearch options <<<<<<<<<<<<<<<<<<<<<<<< */


// funcs
void  Initialize(PLUGINDATA* pd);
void  Uninitialize(void);

void  ReadFindHistoryA(void);
void  ReadFindHistoryW(void);
void  SaveFindHistoryA(void);
void  SaveFindHistoryW(void);

#if AKELPAD_RUNTIME_VERSION_CHECK
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2);
#endif

// debug helper
#ifdef _DEBUG
void Debug_Output(const char* szFormat, ...);
#endif

//---------------------------------------------------------------------------
#endif
