#ifndef _CODEFOLD_H_
#define _CODEFOLD_H_


//// Defines

#define STRID_RICHEDIT_NOTSUPPORTED  201
#define STRID_GOBEGIN                202
#define STRID_GOEND                  203
#define STRID_SELECT                 204
#define STRID_INVIEW                 205
#define STRID_LISTEXPAND             206
#define STRID_LISTCOLLAPSE           207
#define STRID_STATISTICS             208
#define STRID_STATISTICS_MSG         209
#define STRID_SETUP                  210
#define STRID_NONE                   211
#define STRID_SHOWDOCK_GROUP         212
#define STRID_AUTO                   213
#define STRID_ALWAYS                 214
#define STRID_FOLDLIMIT              215
#define STRID_FOLLOWCARET_GROUP      216
#define STRID_ANYWHERE               217
#define STRID_ROOT                   218
#define STRID_LISTSYSTEMCOLORS       219
#define STRID_LISTSYSTEMFONT         220
#define STRID_DRAWNODE               221
#define STRID_ROUND                  222
#define STRID_SQUARE                 223
#define STRID_TAGMARK                224
#define STRID_COLLAPSEONOPEN         225
#define STRID_NOPRINTCOLLAPSED       226
#define STRID_FINDROOT               227
#define STRID_HOTKEY                 228
#define STRID_DEPTH                  229
#define STRID_HOTKEYCURRENT          230
#define STRID_HOTKEYALL              231
#define STRID_COLLAPSE               232
#define STRID_NEXTLEVEL              233
#define STRID_PREVLEVEL              234

#define DLLA_CODEFOLD_SHOWDOCK   1
#define DLLA_CODEFOLD_ADDWINDOW  50
#define DLLA_CODEFOLD_DELWINDOW  51
#define DLLA_CODEFOLD_GETWINDOW  52

#define INIT_CODEFOLD_HOTKEYS      0x1
#define INIT_CODEFOLD_WINDOWS      0x2

//Dock messages
#define AKDLL_SETUP      (WM_USER + 100)
#define AKDLL_SETFOCUS   (WM_USER + 101)
#define AKDLL_UPDATE     (WM_USER + 102)

//DestroyDock type
#define DKT_DEFAULT        0x0
#define DKT_NOUNLOAD       0x1
#define DKT_ONMAINFINISH   0x2
#define DKT_KEEPAUTOLOAD   0x4

//Show dock setting
#define CFSD_NONE     0
#define CFSD_AUTO     1
#define CFSD_ALWAYS   2

//Sizes
#define BOARD_WIDTH      10
#define BOARD_EDGE       4

//Painting figures
#define FRG_NONE            0
#define FRG_LINE            1
#define FRG_FOLDSTARTPLUS   2
#define FRG_FOLDSTARTMINUS  3
#define FRG_FOLDEND         4

//Draw node types
#define DNT_SQUARE          0
#define DNT_ROUND           1

//Follow caret option
#define FCO_NONE            0
#define FCO_ANYWHERE        1
#define FCO_ONLYROOT        2

//SetActiveEdit flags
#define SAE_NOFOLD          0x01
#define SAE_NOLIST          0x02
#define SAE_RESETFOLD       0x04
#define SAE_RESETLIST       0x08
#define SAE_IGNOREMAXFOLDS  0x10

//IsFold input flags
#define IFF_CHECKFIRSTFOLDSTART 0x10

//IsFold return value
#define IFE_FOLDSTART           0x1
#define IFE_FOLDEND             0x2

//StackGetFoldByLine flags
#define GL_DEEPESTLEVEL  0x1
#define GL_DEEPLESSLINE  0x2

//FOLDINFO flags
#define FIF_MATCHCASE               0x00000001  //Case sensitive.
#define FIF_NAMEFROMLEFT            0x00000002  //Force get fold name for function list from left at the fold start.
#define FIF_NAMEFROMRIGHT           0x00000004  //Force get fold name for function list from right at the fold start, including fold start.
#define FIF_FOLDSTART_ATLINESTART   0x00000008  //Fold start located at line start.
#define FIF_FOLDSTART_ATLINEEND     0x00000010  //Fold start located at line end.
#define FIF_FOLDEND_ATLINESTART     0x00000020  //Fold end located at line start.
#define FIF_FOLDEND_ATLINEEND       0x00000040  //Fold end located at line end.
#define FIF_FOLDEND_NOCATCH         0x00000080  //Don't catch fold end or skip end.
#define FIF_FOLDEND_NOCATCH_SIBLING 0x00000100  //Additional to 128 flag - more priority is given to sibling level instead of parent level.
#define FIF_COMMENTFOLD             0x00000200  //Comment fold - fold ignore any other folds and skips inside.
#define FIF_FOLDEND_REQUIRED        0x00000400  //Reserved.
#define FIF_DENYFOLD                0x00000800  //Deny fold - found fold will be ignored.
#define FIF_XMLTAG                  0x00001000  //XML fold - "<tag" and "</tag>", "<single" and "/>", "<single" and ">".
#define FIF_FOLDSTART_NOLEFTDELIM   0x00002000  //Don't check delimiters from left of fold start.
#define FIF_FOLDSTART_NORIGHTDELIM  0x00004000  //Don't check delimiters from right of fold start.
#define FIF_FOLDEND_NOLEFTDELIM     0x00008000  //Don't check delimiters from left of fold end.
#define FIF_FOLDEND_NORIGHTDELIM    0x00010000  //Don't check delimiters from right of fold end.
#define FIF_DENYDELIM               0x00020000  //Deny specified delimiters.
#define FIF_NOLISTFOLD              0x00040000  //Don't show fold in list.
#define FIF_REGEXPSTART             0x00100000  //Regular expressions of fixed length in fold start or skip start.
#define FIF_REGEXPEND               0x00200000  //Regular expressions of fixed length in fold end or skip end.
#define FIF_XMLNAMED_ONETAG         0x10000000  //
#define FIF_XMLNONAME_ONETAG        0x20000000  //
#define FIF_XMLNONAME_TWOTAG        0x40000000  //


//// Structures

typedef struct _SKIPSTART {
  struct _SKIPSTART *next;
  struct _SKIPSTART *prev;
  DWORD dwFlags;
  wchar_t *wpSkipStart;
  int nSkipStartLen;
  STACKREGROUP sregStart;

  //Stack with the same wpSkipStart.
  HSTACK hSkipInfoHandleStack;
  int nElements;
} SKIPSTART;

typedef struct _SKIPINFO {
  struct _SKIPINFO *next;
  struct _SKIPINFO *prev;
  DWORD dwFlags;
  SKIPSTART *lpSkipStart;
  wchar_t *wpSkipEnd;
  int nSkipEndLen;
  wchar_t wchEscape;
  STACKREGROUP sregEnd;
} SKIPINFO;

typedef struct _SKIPINFOHANDLE {
  struct _SKIPINFOHANDLE *next;
  struct _SKIPINFOHANDLE *prev;
  SKIPINFO *lpSkipInfo;
} SKIPINFOHANDLE;

typedef struct _FOLDSTART {
  struct _FOLDSTART *next;
  struct _FOLDSTART *prev;
  DWORD dwFlags;
  wchar_t *wpFoldStart;
  int nFoldStartLen;
  int nFoldStartPointLen;
  STACKREGROUP sregStart;

  //Stack with the same wpFoldStart.
  HSTACK hFoldInfoHandleStack;
  int nElements;
} FOLDSTART;

typedef struct _FOLDINFO {
  struct _FOLDINFO *next;
  struct _FOLDINFO *prev;
  DWORD dwFlags;
  FOLDSTART *lpFoldStart;
  wchar_t *wpFoldEnd;
  int nFoldEndLen;
  int nFoldEndPointLen;
  wchar_t *wpDelimiters;
  DWORD dwFontStyle;
  DWORD dwColor1;
  DWORD dwColor2;
  STACKREGROUP sregEnd;
} FOLDINFO;

typedef struct _FOLDINFOHANDLE {
  struct _FOLDINFOHANDLE *next;
  struct _FOLDINFOHANDLE *prev;
  FOLDINFO *lpFoldInfo;
} FOLDINFOHANDLE;

typedef struct {
  AEFOLD *lpFold;
  FOLDINFO *lpFoldInfo;
  HTREEITEM hItem;
  wchar_t *wpName;
} FOLDDATA;

//For FillLevelsStack
typedef struct _LEVEL {
  struct _LEVEL *next;
  struct _LEVEL *prev;
  struct _LEVEL *lpParent;
  FOLDDATA *pfd;
  AEPOINT pointMin;
  AEPOINT pointMax;
  AECHARRANGE crFoundMin;
  AECHARRANGE crFoundMax;
  BOOL bMultiLine;
} LEVEL;

typedef struct {
  LEVEL *first;
  LEVEL *last;
} STACKLEVEL;

typedef struct {
  SYNTAXFILE *lpSyntaxFile;
  HSTACK *lpFoldStack;
  STACKLEVEL hLevelStack;
  STACKLEVEL hTempStack;
  AEHMARKRANGE hTagMarkFirst;
  AEHMARKRANGE hTagMarkSecond;
  AEHMARKRANGE hTagMarkThird;
  AEFOLD *lpTagMark;
  wchar_t wszFilter[MAX_PATH];
} FOLDWINDOWDATA;

typedef struct _FOLDWINDOW {
  struct _FOLDWINDOW *next;
  struct _FOLDWINDOW *prev;
  FOLDWINDOWDATA fwd;
  FOLDWINDOWDATA *pfwd;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  RECT rcBoard;
  AECHARRANGE crTextChange;
  AEFOLD *lpTextChangeMinParent;
  AEFOLD *lpTextChangeMaxParent;
  AEFOLD *lpTextChangeMinPrevParent;
  AEFOLD *lpTextChangeMaxPrevParent;
  BOOL bLevelChanged;
  BOOL bReturnPrintOption;

  //User window
  MANUALSET *lpUser;
} FOLDWINDOW;

typedef struct {
  FOLDWINDOW *first;
  FOLDWINDOW *last;
  BOOL bLock;
} STACKFOLDWINDOW;

//LineBoard external call
typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  RECT *rcBoard;
} DLLEXTLINEBOARD;

#define DLLA_LINEBOARD_GETBOARDRECT  11


//// Macros

#ifndef FoldData
  #define FoldData(lpFold)  ((FOLDDATA *)((AEFOLD *)lpFold)->dwUserData)
#endif


//// Prototypes

void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow);
void DestroyDock(HWND hWndDock, DWORD dwType);
BOOL CALLBACK CodeFoldDockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CodeFold1SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CodeFold2SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CodeFoldParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
BOOL CALLBACK CodeFoldEditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
BOOL CALLBACK CurrentCollapseProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK CurrentGoBeginProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK CurrentGoEndProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK CurrentSelectProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK AllCollapseProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK AllNextLevelProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK AllPrevLevelProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK FindRootLevelProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);

SKIPINFO* StackInsertSkipInfo(STACKSKIP *hStack);
SKIPSTART* StackInsertSkipStart(HSTACK *hStack, SKIPINFO *lpSkipInfo, wchar_t *wpSkipStart, int nSkipStartLen);
void StackDeleteSkipInfo(STACKSKIP *hSkipStack, HSTACK *hSkipStartStack, SKIPINFO *lpSkipInfo);
void StackFreeSkipInfo(STACKSKIP *hSkipStack, HSTACK *hSkipStartStack);
FOLDINFO* StackInsertFoldInfo(STACKFOLD *hStack);
FOLDSTART* StackInsertFoldStart(HSTACK *hFoldStartStack, FOLDINFO *lpFoldInfo, wchar_t *wpFoldStart, int nFoldStartLen);
void StackDeleteFoldInfo(STACKFOLD *hFoldStack, HSTACK *hFoldStartStack, FOLDINFO *lpFoldInfo);
void StackFreeFoldInfo(STACKFOLD *hFoldStack, HSTACK *hFoldStartStack);
void StackEndBoard(STACKFOLDWINDOW *hStack, FOLDWINDOW *lpFoldWindow);
FOLDWINDOW* StackInsertFoldWindow(STACKFOLDWINDOW *hStack);
FOLDWINDOW* StackGetFoldWindow(STACKFOLDWINDOW *hStack, HWND hWndEdit, AEHDOC hDocEdit);
FOLDWINDOW* StackGetFoldWindowByUserEdit(STACKFOLDWINDOW *hStack, HWND hWndEdit);
void StackDeleteFoldWindow(STACKFOLDWINDOW *hStack, FOLDWINDOW *lpFoldWindow);
void StackFreeFoldWindows(STACKFOLDWINDOW *hStack);
FOLDWINDOW* FillLevelsStack(FOLDWINDOW *lpFoldWindow, STACKLEVEL *hLevelStack, HWND hWnd, AECHARRANGE *crRange);
void CreateFold(LEVEL *lpLevel, HWND hWnd, BOOL bCollapse);
void CreateAllFolds(STACKLEVEL *hLevelStack, HWND hWnd);
LEVEL* StackInsertLevel(STACKLEVEL *hLevelStack, AECHARINDEX *ciChar);
void StackDeleteLevel(STACKLEVEL *hLevelStack, LEVEL *lpLevel);
void StackFreeLevels(STACKLEVEL *hLevelStack);
AEFOLD* FoldNextDepth(AEFOLD *lpFold, int *nCurDepth, int nMaxDepth);
AEFOLD* FoldGet(FOLDWINDOW *lpFoldWindow, DWORD dwFlags, UINT_PTR dwFindIt, AEFOLD **lpPrevSubling);
AEFOLD* FoldAtIndex(FOLDWINDOW *lpFoldWindow, AECHARINDEX *ciChar, DWORD dwFoldStop);
AEFOLD* GetCaretFold(FOLDWINDOW *lpFoldWindow, AEFOLD **lpPrevSubling);
BOOL IsFoldNameFromLeft(FOLDDATA *lpFoldData);
BOOL GetFoldName(FOLDDATA *lpFoldData, const AECHARINDEX *ciInput, AECHARRANGE *crNameRange);
void FoldInView(FOLDWINDOW *lpFoldWindow, AEFOLD *lpFold, int nMenuAction);
BOOL FoldSelect(FOLDWINDOW *lpFoldWindow, AEFOLD *lpFold);
void FoldSwitchCollapse(FOLDWINDOW *lpFoldWindow, AEFOLD *lpFold, DWORD dwFlags);
void DeleteFoldData(FOLDDATA *lpFoldData);
void FreeFolds(FOLDWINDOW *lpFoldWindow, BOOL bUpdate);
INT_PTR EndOfPoint(FOLDWINDOW *lpFoldWindow, const AEPOINT *lpPoint, AECHARINDEX *ciChar);
void ClearTreeView(HWND hWndTreeView, BOOL bRedraw);
void FillTreeView(HWND hWndTreeView, FOLDWINDOW *lpFoldWindow, const wchar_t *wpFilter);
void ExpandTreeView(HWND hWndTreeView, FOLDWINDOW *lpFoldWindow, BOOL bExpand);
AEFOLD* GetCurFoldTreeView(HWND hWndTreeView);
BOOL SetCaretTreeView(HWND hWndTreeView, FOLDWINDOW *lpFoldWindow);
FOLDWINDOW* SetActiveEdit(HWND hWndEdit, HWND hWndTreeView, DWORD dwFlags);
void UpdateTagMark(FOLDWINDOW *lpFoldWindow);
BOOL RemoveTagMark(FOLDWINDOW *lpFoldWindow);
DWORD CALLBACK IsMatch(AEFINDTEXTW *ft, const AECHARINDEX *ciChar);
DWORD CALLBACK IsMatchRE(STACKREGROUP *sreg, AECHARRANGE *crFound, const AECHARINDEX *ciChar);
BOOL IsEscaped(const AECHARINDEX *ciChar, wchar_t wchEscape);
FOLDINFO* IsFold(FOLDWINDOW *lpFoldWindow, LEVEL *lpLevel, AEFINDTEXTW *ft, AECHARINDEX *ciChar, DWORD *dwFoldStop);
FOLDINFO* IsFoldStart(FOLDSTART *lpFoldStart, AEFINDTEXTW *ft, AECHARINDEX *ciChar);
FOLDINFO* IsFoldEnd(FOLDINFO *lpFoldInfo, AEFINDTEXTW *ft, AECHARINDEX *ciChar);
SKIPINFO* IsSkipStart(SKIPSTART *lpSkipStart, AEFINDTEXTW *ft, AECHARINDEX *ciChar);
SKIPINFO* IsSkipEnd(SKIPINFO *lpSkipInfo, AEFINDTEXTW *ft, AECHARINDEX *ciChar);
FOLDINFO* FindFold(FOLDWINDOW *lpFoldWindow, const AECHARRANGE *crSearchRange);
BOOL CheckFlags(FOLDINFO *lpFoldInfo, AECHARRANGE *crFound, DWORD dwFoldStop);
BOOL IsSpacesFromLeft(const AECHARINDEX *ciChar);
BOOL IsSpacesFromRight(const AECHARINDEX *ciChar);
void SetEditRect(AEHDOC hDocEdit, HWND hWndEdit, int nNewWidth, int nOldWidth);
DWORD ScrollToPoint(HWND hWnd, POINT *ptPos);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
LRESULT SendToDoc(AEHDOC hDocEdit, HWND hWndEdit, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ReadCodeFoldOptions(HANDLE hOptions);
void SaveCodeFoldOptions(HANDLE hOptions, DWORD dwFlags);
void InitCodeFold();
void UninitCodeFold(DWORD dwFlags);


//// Global variables

extern STACKFOLDWINDOW hFoldWindowsStack;
extern FOLDWINDOW *lpCurrentFoldWindow;
extern HWND hWndCodeFoldList;
extern DWORD dwFoldListTextColor;
extern DWORD dwFoldListBkColor;

#endif
