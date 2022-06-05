#ifndef _auto_brackets_h_
#define _auto_brackets_h_
//---------------------------------------------------------------------------
#include "base.h"
#include "AnyRichEdit.h"

#define MAX_USER_BRACKETS          31
#define MAX_PREV_NEXT_CHAR_OK_SIZE 96

// highlight style flags
#define XBR_HSF_BOLDFONT      0x0001
#define XBR_HSF_REDRAWCODER   0x0002

// highlight xmode flags
#define XBR_HXM_XBRINTERNAL   0x0001
#define XBR_HXM_AKELHIGHLIGHT 0x0002
#define XBR_HXM_AKELFOLD      0x0004
#define XBR_HXM_ALL           (XBR_HXM_XBRINTERNAL | XBR_HXM_AKELHIGHLIGHT | XBR_HXM_AKELFOLD)

// get active brackets flags
#define XBR_GBF_HIGHLIGHTBR  0x0001 // highlight active brackets
#define XBR_GBF_UPDATEHLDATA 0x0100 // update highlight data of "cached" brackets

// nearbr goto flags
#define XBR_NBR_GOTO_OUTERPOS     0x0001
#define XBR_NBR_GOTO_ALWAYSLEFT   0x0010
#define XBR_NBR_GOTO_ALWAYSRIGHT  0x0020

// nearbr selto flags
#define XBR_NBR_SELTO_OUTERPOS    0x0001
#define XBR_NBR_SELTO_WIDEN       0x0002


// default values
#define DEFAULT_MAX_DETECT_DUPPAIR_LINES  1
#define DEFAULT_MAX_KNOWN_DUPPAIR_LINES  10

#define DEFAULT_MAX_BR_LINES  0

#define DEFAULT_NEARESTBR_GOTO_FLAGS   0
#define DEFAULT_NEARESTBR_SELTO_FLAGS  XBR_NBR_SELTO_WIDEN
#define DEFAULT_NEARESTBR_MAX_LINES    0


enum eXBracketsAction {
  XBRA_GOTO = 1,
  XBRA_SELTO,
  XBRA_GOTO_NEABR,
  XBRA_SELTO_NEARBR
};

typedef struct sMSGINFO {
  HWND   hWnd;
  UINT   uMsg;
  WPARAM wParam;
  LPARAM lParam;
} MSGINFO;

// For brackets auto-completion only.
void  OnEditCharPressed(MSGINFO* pmsgi);

// For both brackets auto-completion and active brackets highlight.
// Mostly used for active brackets highlight.
// Uses "cached" brackets data if it exists.
void  OnEditGetActiveBrackets(MSGINFO* pmsgi, const unsigned int uFlags /* = XBR_GBF_HIGHLIGHTBR */);

// Highlights active brackets.
// Mostly this function is called from OnEditGetActiveBrackets.
void  OnEditHighlightActiveBrackets(void);

// Goes to the nearest bracket/quote or extends the selection to the nearest pair
void  OnEditGetNearestBracketsFunc(int action, HWND hEditWnd, INT_X nCharacterPosition);

// Removes all the "cached" brackets data.
// Also repaints the brackets when bRepaint=TRUE.
void  RemoveAllHighlightInfo(const BOOL bRepaint /* = TRUE */);

// Gets file type to use proper file-type-depenedent options.
int   getFileType(int* pnCurrentFileType2);

// To be called on plugin unloading or unitialization.
void  AutoBrackets_Uninitialize(void);

void  setNextCharOkW(const wchar_t* cszNextCharOkW);
void  setPrevCharOkW(const wchar_t* cszPrevCharOkW);
void  setUserBracketsA(const char* cszUserBracketsA);
void  setUserBracketsW(const wchar_t* cszUserBracketsW);

const char*    getCurrentBracketsPairA(void);
const wchar_t* getCurrentBracketsPairW(void);

BOOL WidenNearestBracketsSelection(HWND hWndEdit, const void* crSel); // CHARRANGE_X* crSel

//---------------------------------------------------------------------------
#endif

