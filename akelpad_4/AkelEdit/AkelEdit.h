#ifndef __AKELEDIT_H__
#define __AKELEDIT_H__


//// Defines

#define AEN_SELCHANGE         (WM_USER + 1001)
#define AEN_TEXTCHANGE        (WM_USER + 1002)
#define AEN_MODIFYCHANGE      (WM_USER + 1003)
#define AEN_ERRSPACE          (WM_USER + 1004)

#define AEM_SETTEXTA          (WM_USER + 2001)
#define AEM_SETTEXTW          (WM_USER + 2002)
#define AEM_REPLACESELA       (WM_USER + 2003)
#define AEM_REPLACESELW       (WM_USER + 2004)
#define AEM_GETTEXTRANGEA     (WM_USER + 2005)
#define AEM_GETTEXTRANGEW     (WM_USER + 2006)
#define AEM_STREAMOUT         (WM_USER + 2007)
#define AEM_CANPASTE          (WM_USER + 2008)
#define AEM_CANUNDO           (WM_USER + 2009)
#define AEM_CANREDO           (WM_USER + 2010)
#define AEM_CUT               (WM_USER + 2011)
#define AEM_COPY              (WM_USER + 2012)
#define AEM_PASTE             (WM_USER + 2013)
#define AEM_UNDO              (WM_USER + 2014)
#define AEM_REDO              (WM_USER + 2015)
#define AEM_EMPTYUNDOBUFFER   (WM_USER + 2016)
#define AEM_STOPGROUPTYPING   (WM_USER + 2017)

#define AEM_GETSEL            (WM_USER + 2018)
#define AEM_SETSEL            (WM_USER + 2019)
#define AEM_GETCOLUMNSEL      (WM_USER + 2020)
#define AEM_UPDATESEL         (WM_USER + 2021)
#define AEM_GETLINECOUNT      (WM_USER + 2022)
#define AEM_GETINDEX          (WM_USER + 2023)
#define AEM_GETLINEINDEX      (WM_USER + 2024)
#define AEM_UPDATEINDEX       (WM_USER + 2025)
#define AEM_INDEXCOMPARE      (WM_USER + 2026)
#define AEM_INDEXSUBTRACT     (WM_USER + 2027)
#define AEM_INDEXOFFSET       (WM_USER + 2028)
#define AEM_INDEXTORICHOFFSET (WM_USER + 2029)
#define AEM_RICHOFFSETTOINDEX (WM_USER + 2030)
#define AEM_CHARFROMPOS       (WM_USER + 2031)
#define AEM_POSFROMCHAR       (WM_USER + 2032)
#define AEM_ISCARETVISIBLE    (WM_USER + 2033)
#define AEM_LINESCROLL        (WM_USER + 2034)
#define AEM_SCROLLCARET       (WM_USER + 2035)
#define AEM_STOPSCROLL        (WM_USER + 2036)
#define AEM_FINDTEXTA         (WM_USER + 2037)
#define AEM_FINDTEXTW         (WM_USER + 2038)
#define AEM_GETOPTIONS        (WM_USER + 2039)
#define AEM_SETOPTIONS        (WM_USER + 2040)
#define AEM_GETUNDOLIMIT      (WM_USER + 2041)
#define AEM_SETUNDOLIMIT      (WM_USER + 2042)
#define AEM_GETMODIFY         (WM_USER + 2043)
#define AEM_SETMODIFY         (WM_USER + 2044)
#define AEM_GETRECT           (WM_USER + 2045)
#define AEM_SETRECT           (WM_USER + 2046)
#define AEM_GETMARGINS        (WM_USER + 2047)
#define AEM_SETMARGINS        (WM_USER + 2048)
#define AEM_GETNEWLINE        (WM_USER + 2049)
#define AEM_SETNEWLINE        (WM_USER + 2050)
#define AEM_GETCOLORS         (WM_USER + 2051)
#define AEM_SETCOLORS         (WM_USER + 2052)
#define AEM_GETOVERTYPE       (WM_USER + 2053)
#define AEM_SETOVERTYPE       (WM_USER + 2054)
#define AEM_GETTABSTOP        (WM_USER + 2055)
#define AEM_SETTABSTOP        (WM_USER + 2056)
#define AEM_GETWORDWRAP       (WM_USER + 2057)
#define AEM_SETWORDWRAP       (WM_USER + 2058)
#define AEM_GETWORDDELIMITERS (WM_USER + 2059)
#define AEM_SETWORDDELIMITERS (WM_USER + 2060)
#define AEM_SHOWSCROLLBAR     (WM_USER + 2061)
#define AEM_CHECKCODEPAGE     (WM_USER + 2062)

#define AES_AKELEDITCLASSA     "AkelEditA"
#define AES_AKELEDITCLASSW    L"AkelEditW"
#define AES_RICHEDITCLASSA     "RichEdit20A"
#define AES_RICHEDITCLASSW    L"RichEdit20W"

#define AES_WORDDELIMITERSW   L" \t\n\\|[](){}<>,.;:+-=~!@#$%^&*/?'`\""
#define AES_WRAPDELIMITERSW   L" \t\n"

#define AETIMERID_MOUSEMOVE    1

#define AECO_READONLY           0x00000001
#define AECO_DISABLENOSCROLL    0x00000002
#define AECO_WANTRETURN         0x00000004
#define AECO_DETAILEDUNDO       0x00000008
#define AECO_DISABLEBEEP        0x00000010
#define AECO_PASTESELECTCOLUMN  0x00000020

#define AECOOP_SET              0
#define AECOOP_OR               1
#define AECOOP_AND              2
#define AECOOP_XOR              3

#define AEGI_FIRSTCHAR         1
#define AEGI_LASTCHAR          2
#define AEGI_FIRSTSELCHAR      3
#define AEGI_LASTSELCHAR       4
#define AEGI_CARETCHAR         5
#define AEGI_FIRSTVISIBLELINE  6
#define AEGI_LASTVISIBLELINE   7
#define AEGI_NEXTLINE          8
#define AEGI_PREVLINE          9
#define AEGI_NEXTCHAR          10
#define AEGI_PREVCHAR          11
#define AEGI_NEXTBREAK         12
#define AEGI_PREVBREAK         13

#define AELS_EMPTY   1
#define AELS_FULL    2
#define AELS_PARTLY  3

#define AELB_ASIS     1
#define AELB_ASINPUT  2
#define AELB_ASOUTPUT 3
#define AELB_EOF      4
#define AELB_R        5
#define AELB_N        6
#define AELB_RN       7
#define AELB_RRN      8
#define AELB_WRAP     9

#define AENL_INPUT           0x00000001
#define AENL_OUTPUT          0x00000002

#define AEUN_INSERT          0x00000001
#define AEUN_DELETE          0x00000002
#define AEUN_SETSEL          0x00000004
#define AEUN_COLUMNSEL       0x00000008
#define AEUN_COLUMNGROUP     0x00000010
#define AEUN_STOPGROUP       0x00000020
#define AEUN_SINGLECHAR      0x00000040
#define AEUN_OVERTYPECHAR    0x00000080
#define AEUN_BACKSPACEKEY    0x00000100
#define AEUN_DELETEKEY       0x00000200
#define AEUN_EXTRAOFFSET     0x00000400
#define AEUN_UNDOONLY        0x00000800
#define AEUN_REDOONLY        0x00001000

#define AECLR_DEFAULT        0x00000001
#define AECLR_CARET          0x00000002
#define AECLR_BASICTEXT      0x00000004
#define AECLR_BASICBK        0x00000008
#define AECLR_SELTEXT        0x00000010
#define AECLR_SELBK          0x00000020
#define AECLR_ACTIVELINETEXT 0x00000040
#define AECLR_ACTIVELINEBK   0x00000080
#define AECLR_ALL            (AECLR_CARET         |\
                              AECLR_BASICTEXT     |\
                              AECLR_BASICBK       |\
                              AECLR_SELTEXT       |\
                              AECLR_SELBK         |\
                              AECLR_ACTIVELINETEXT|\
                              AECLR_ACTIVELINEBK)

#define AEFR_DOWN            0x00000001
#define AEFR_WHOLEWORD       0x00000002
#define AEFR_MATCHCASE       0x00000004

#define AEWB_LEFTWORDSTART   0x00000001
#define AEWB_LEFTWORDEND     0x00000002
#define AEWB_RIGHTWORDSTART  0x00000004
#define AEWB_RIGHTWORDEND    0x00000008

#ifndef FR_DOWN
  #define FR_DOWN 0x00000001
#endif
#ifndef FR_WHOLEWORD
  #define FR_WHOLEWORD 0x00000002
#endif
#ifndef FR_MATCHCASE
  #define FR_MATCHCASE 0x00000004
#endif
#ifndef EC_LEFTMARGIN
  #define EC_LEFTMARGIN 0x0001
#endif
#ifndef EC_RIGHTMARGIN
  #define EC_RIGHTMARGIN 0x0002
#endif
#ifndef SPI_GETWHEELSCROLLLINES
  #define SPI_GETWHEELSCROLLLINES 0x0068
#endif
#ifndef WC_NO_BEST_FIT_CHARS
  #define WC_NO_BEST_FIT_CHARS 0x00000400
#endif
#ifndef WM_MOUSEWHEEL
  #define WM_MOUSEWHEEL 0x020A
#endif
#ifndef EM_SHOWSCROLLBAR
  #define EM_SHOWSCROLLBAR (WM_USER + 96)
#endif
#ifndef EM_GETSCROLLPOS
  #define EM_GETSCROLLPOS (WM_USER + 221)
#endif
#ifndef EM_SETSCROLLPOS
  #define EM_SETSCROLLPOS (WM_USER + 222)
#endif
#ifndef mod
  #define mod(a) (((a) < 0)?(0 - (a)):(a))
#endif

typedef BOOL (CALLBACK *AEStreamCallback)(DWORD, wchar_t *, DWORD);


//// Structures

#ifndef _HSTACK_STRUCT_
#define _HSTACK_STRUCT_
typedef struct {
  int first;
  int last;
} HSTACK;
#endif

#ifndef _STACK_STRUCT_
#define _STACK_STRUCT_
typedef struct _stack {
  struct _stack *next;
  struct _stack *prev;
} stack;
#endif

typedef struct _AELINEDATA {
  struct _AELINEDATA *next;
  struct _AELINEDATA *prev;
  wchar_t *wpLine;
  int nLineLen;
  int nLineBreak;
  int nLineWidth;
  int nSelStart;
  int nSelEnd;
} AELINEDATA;

typedef struct _AELINEINDEX {
  int nLine;
  AELINEDATA *lpLine;
} AELINEINDEX;

typedef struct _AECHARINDEX {
  int nLine;
  AELINEDATA *lpLine;
  int nCharInLine;
} AECHARINDEX;

typedef struct _AEUNDOITEM {
  struct _AEUNDOITEM *next;
  struct _AEUNDOITEM *prev;
  DWORD dwFlags;
  int nActionStartOffset;
  int nActionEndOffset;
  int nExtraStartOffset;
  int nExtraEndOffset;
  wchar_t *wpText;
  DWORD dwTextLen;
} AEUNDOITEM;

typedef struct {
  AECHARINDEX ciMin;
  AECHARINDEX ciMax;
} AECHARRANGE;

typedef struct {
  AECHARRANGE crSel;
  AECHARINDEX *lpciCaret;
  BOOL bColumnSel;
} AESELECTION;

typedef struct {
  char *pText;
  DWORD dwTextLen;
  BOOL bColumnSel;
  AECHARINDEX *ciInsertStart;
  AECHARINDEX *ciInsertEnd;
} AEREPLACESELA;

typedef struct {
  wchar_t *wpText;
  DWORD dwTextLen;
  BOOL bColumnSel;
  AECHARINDEX *ciInsertStart;
  AECHARINDEX *ciInsertEnd;
} AEREPLACESELW;

typedef struct {
  AECHARRANGE cr;
  char *pText;
  int nNewLine;
  BOOL bColumnSel;
} AETEXTRANGEA;

typedef struct {
  AECHARRANGE cr;
  wchar_t *wpText;
  int nNewLine;
  BOOL bColumnSel;
} AETEXTRANGEW;

typedef struct {
  AEStreamCallback lpCallback;
  DWORD dwCookie;
} AESTREAM;

typedef struct {
  DWORD dwFlags;           //See AEFR_* defines
  AECHARRANGE crSearch;    //Range of characters to search.
  char *pText;             //Text to find
  DWORD dwTextLen;         //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;            //See AELB_* defines
  AECHARRANGE crFound;     //Range of characters in which text is found
} AEFINDTEXTA;

typedef struct {
  DWORD dwFlags;           //See AEFR_* defines
  AECHARRANGE crSearch;    //Range of characters to search.
  wchar_t *wpText;         //Text to find
  DWORD dwTextLen;         //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;            //See AELB_* defines
  AECHARRANGE crFound;     //Range of characters in which text is found
} AEFINDTEXTW;

typedef struct {
  DWORD dwFlags;             //See AECLR_* defines
  COLORREF crCaret;          //Caret color
  COLORREF crBasicText;      //Basic text color
  COLORREF crBasicBk;        //Basic background color
  COLORREF crSelText;        //Text color in selection
  COLORREF crSelBk;          //Background color in selection
  COLORREF crActiveLineText; //Text color in active line
  COLORREF crActiveLineBk;   //Background color in active line
} AECOLORS;

typedef struct {
  AECHARINDEX *ciCharIn;
  AECHARINDEX *ciCharOut;
  int nOffset;
  int nNewLine;
} AEINDEXOFFSET;

typedef struct {
  AECHARINDEX *ciChar1;
  AECHARINDEX *ciChar2;
  int nNewLine;
  BOOL bColumnSel;
} AEINDEXSUBTRACT;

typedef struct {
  NMHDR hdr;
  AESELECTION aes;
} AENSELCHANGE;

typedef struct {
  NMHDR hdr;
} AENTEXTCHANGE;

typedef struct {
  NMHDR hdr;
  BOOL bModified;
} AENMODIFYCHANGE;

typedef struct _AKELEDIT {
  struct _AKELEDIT *next;
  struct _AKELEDIT *prev;
  BOOL bUnicodeWindow;
  HANDLE hHeap;
  HWND hWndEdit;
  HWND hWndParent;
  int nEditCtrlID;
  DWORD dwOptions;
  RECT rcEdit;
  RECT rcDraw;
  RECT rcErase;
  LOGFONTA lfEditA;
  LOGFONTW lfEditW;
  COLORREF crBasicText;
  COLORREF crBasicBk;
  COLORREF crSelText;
  COLORREF crSelBk;
  COLORREF crActiveLineText;
  COLORREF crActiveLineBk;
  COLORREF crEnableBasicBk;
  COLORREF crEnableSelBk;
  COLORREF crEnableActiveLineBk;
  HDC hDC;
  HFONT hFont;
  HBRUSH hBasicBk;
  HBRUSH hSelBk;
  HBRUSH hActiveLineBk;
  int nCharHeight;
  int nAveCharWidth;
  int nSpaceCharWidth;
  int nTabWidth;
  BOOL bFixedCharWidth;
  int nTabStop;
  BOOL bOverType;
  HSTACK hUndoStack;
  AEUNDOITEM *lpCurrentUndo;
  AEUNDOITEM *lpSavePoint;
  BOOL bSavePointExist;
  BOOL bModified;
  DWORD dwUndoLimit;
  DWORD dwUndoCount;
  HSTACK hLinesStack;
  AELINEINDEX liFirstDrawLine;
  AELINEINDEX liMaxWidthLine;
  AECHARINDEX ciSelStartIndex;
  AECHARINDEX ciSelEndIndex;
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciLastCallIndex;
  int nLineCount;
  int nInputNewLine;
  int nOutputNewLine;
  int nHScrollPos;
  int nVScrollPos;
  int nHScrollMax;
  int nVScrollMax;
  BOOL bVScrollShow;
  BOOL bHScrollShow;
  BOOL bVScrollStop;
  BOOL bHScrollStop;
  HBITMAP hCaretInsert;
  HBITMAP hCaretOvertype;
  COLORREF crCaret;
  int nCaretInsertWidth;
  int nCaretOvertypeHeight;
  POINT ptCaret;
  int nHorizCaretPos;
  BOOL bFocus;
  BOOL bCaretVisible;
  BOOL bColumnSel;
  BOOL bWordWrap;
  DWORD dwMouseMoveTimer;
  DWORD dwWordBreak;
  wchar_t wszWordDelimiters[128];
  wchar_t wszWrapDelimiters[128];

  //RichEdit emulation
  BOOL bRichEditClass;
  DWORD dwEventMask;
  int nLastCharOffset;
  int nFirstDrawLineOffset;
  int nSelStartLineOffset;
  int nSelEndLineOffset;
  int nLastCallOffset;
} AKELEDIT;


//// Functions prototypes

BOOL AE_RegisterClassA(HINSTANCE hInstance);
BOOL AE_RegisterClassW(HINSTANCE hInstance);
BOOL AE_UnregisterClassA(HINSTANCE hInstance);
BOOL AE_UnregisterClassW(HINSTANCE hInstance);
LRESULT CALLBACK AE_EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LPVOID AE_HeapAlloc(AKELEDIT *ae, DWORD dwFlags, SIZE_T dwBytes);
BOOL AE_HeapFree(AKELEDIT *ae, DWORD dwFlags, LPVOID lpMem);
int AE_HeapStackInsert(AKELEDIT *ae, stack **first, stack **last, stack **element, int nIndex, int nBytes);
int AE_HeapStackInsertBefore(AKELEDIT *ae, stack **first, stack **last, stack **element, stack *index, int nBytes);
int AE_HeapStackInsertAfter(AKELEDIT *ae, stack **first, stack **last, stack **element, stack *index, int nBytes);
int AE_HeapStackDelete(AKELEDIT *ae, stack **first, stack **last, stack *element);
void AE_HeapStackClear(AKELEDIT *ae, stack **first, stack **last);
AKELEDIT* AE_StackWindowInsert(HSTACK *hStack);
AKELEDIT* AE_StackWindowGet(HSTACK *hStack, HWND hWndEdit);
void AE_StackWindowDelete(HSTACK *hStack, HWND hWndEdit);
void AE_StackWindowFree(HSTACK *hStack);
AEUNDOITEM* AE_StackUndoItemInsert(AKELEDIT *ae);
void AE_StackUndoItemDelete(AKELEDIT *ae, AEUNDOITEM *lpItem);
void AE_StackRedoDeleteAll(AKELEDIT *ae, AEUNDOITEM *lpItem);
void AE_StackUndoGroupStop(AKELEDIT *ae);
AELINEDATA* AE_StackLineAdd(AKELEDIT *ae);
AELINEDATA* AE_StackLineInsertBefore(AKELEDIT *ae, AELINEDATA *lpLine);
AELINEDATA* AE_StackLineInsertAfter(AKELEDIT *ae, AELINEDATA *lpLine);
void AE_StackLineDelete(AKELEDIT *ae, AELINEDATA *lpElement);
void AE_StackLineFree(AKELEDIT *ae);
AELINEDATA* AE_GetLineData(AKELEDIT *ae, int nLine);
void AE_RichOffsetToAkelIndex(AKELEDIT *ae, int nOffset, AECHARINDEX *ciCharIndex);
int AE_AkelIndexToRichOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIndex);
BOOL AE_GetIndex(AKELEDIT *ae, int nType, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, BOOL bColumnSel);
int AE_IndexCompare(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2);
DWORD AE_IndexSubtract(AKELEDIT *ae, const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
DWORD AE_IndexOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, int nOffset, int nNewLine);
BOOL AE_UpdateIndex(AKELEDIT *ae, AECHARINDEX *ciChar);
int AE_WrapLines(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, AECHARINDEX *ciPointOne, AECHARINDEX *ciPointTwo, BOOL bWrap);
int AE_LineWrap(AKELEDIT *ae, const AELINEINDEX *liLine, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwMaxWidth, AECHARINDEX *ciPointOne, AECHARINDEX *ciPointTwo);
int AE_LineUnwrap(AKELEDIT *ae, AELINEINDEX *liLine, DWORD dwMaxWidth, AECHARINDEX *ciPointOne, AECHARINDEX *ciPointTwo);
void AE_CalcLinesWidth(AKELEDIT *ae, AELINEINDEX *liStartLine, AELINEINDEX *liEndLine, BOOL bFresh);
int AE_CheckCodepage(AKELEDIT *ae, int nCodePage);
void AE_SetDrawRect(AKELEDIT *ae, RECT *rcDraw, BOOL bRedraw);
void AE_SetEditFontA(AKELEDIT *ae, HFONT hFont, BOOL bNoRedraw);
void AE_SetEditFontW(AKELEDIT *ae, HFONT hFont, BOOL bNoRedraw);
void AE_SetSelectionPos(AKELEDIT *ae, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, BOOL bColumnSel, BOOL bUpdate);
void AE_UpdateSelection(AKELEDIT *ae);
void AE_SetMouseSelection(AKELEDIT *ae, POINT *ptPos, BOOL bShift, BOOL bColumnSel);
BOOL AE_IsCursorOnSelection(AKELEDIT *ae, POINT *ptPos);
HBITMAP AE_CreateCaretBitmap(AKELEDIT *ae, COLORREF crCaret, int nCaretWidth, int nCaretHeight);
HBITMAP AE_LoadBitmapFromMemory(HDC hDC, BYTE *lpBmpFileData);
BOOL AE_UpdateCaret(AKELEDIT *ae, BOOL bFresh);
BOOL AE_SetCaretPos(AKELEDIT *ae);
void SetCaretVis(AKELEDIT *ae);
void AE_ScrollToCaret(AKELEDIT *ae, POINT *ptCaret);
void AE_CenterCaret(AKELEDIT *ae, POINT *ptCaret);
void AE_ScrollEditWindow(AKELEDIT *ae, int nBar, int nPos);
void AE_UpdateScrollBars(AKELEDIT *ae, int nBar);
void AE_UpdateEditWindow(HWND hWndEdit, BOOL bErase);
void AE_RedrawLineRange(AKELEDIT *ae, int nFirstLine, int nLastLine, BOOL bErase);
int AE_GetFirstVisibleLine(AKELEDIT *ae);
int AE_GetLastVisibleLine(AKELEDIT *ae);
BOOL AE_GetTextExtentPoint32(AKELEDIT *ae, wchar_t *wpString, int nStringLen, SIZE *lpSize);
BOOL AE_GetLineWidth(AKELEDIT *ae, AELINEDATA *lpLine);
BOOL AE_GetPosFromChar(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, POINT *ptClientPos);
BOOL AE_GetPosFromCharEx(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, POINT *ptClientPos);
BOOL AE_GetCharInLine(AKELEDIT *ae, const wchar_t *wpString, int nStringLen, int nMaxExtent, BOOL bHalfFit, int *nCharIndex, int *nCharPos, BOOL bColumnSel);
BOOL AE_GetCharInLineEx(AKELEDIT *ae, const AELINEDATA *lpLine, int nMaxExtent, BOOL bHalfFit, int *nCharIndex, int *nCharPos, BOOL bColumnSel);
BOOL AE_GetCharRangeInLine(AKELEDIT *ae, AELINEDATA *lpLine, int nMinExtent, int nMaxExtent, int *nMinCharIndex, int *nMinCharPos, int *nMaxCharIndex, int *nMaxCharPos, BOOL bColumnSel);
BOOL AE_GetCharFromPos(AKELEDIT *ae, POINT *ptClientPos, AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, BOOL bColumnSel);
BOOL AE_GetNextBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciNextBreak, BOOL bColumnSel);
BOOL AE_GetPrevBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciPrevBreak, BOOL bColumnSel);
BOOL AE_GetNextWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel, BOOL bSearch);
BOOL AE_GetPrevWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel, BOOL bSearch);
BOOL AE_IsInDelimiterList(wchar_t *wpList, wchar_t c);
int AE_GetLineSelection(AKELEDIT *ae, const AELINEINDEX *liLine, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, POINT *ptSelStart, POINT *ptSelEnd, int *nSelStartIndexInLine, int *nSelEndIndexInLine, BOOL bColumnSel);
DWORD AE_GetTextRangeAnsi(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, char *szBuffer, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
DWORD AE_GetTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, wchar_t *wszBuffer, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
void AE_DeleteTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, BOOL bColumnSel, BOOL bEnableUndo, BOOL bUpdate);
DWORD AE_GetTextAnsi(AKELEDIT *ae, char *szText, DWORD dwMaxTextLen);
DWORD AE_GetText(AKELEDIT *ae, wchar_t *wszText, DWORD dwMaxTextLen);
DWORD AE_SetTextAnsi(AKELEDIT *ae, char *pText, DWORD dwTextLen, int nNewLine);
DWORD AE_SetText(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, int nNewLine);
void AE_ReplaceSelAnsi(AKELEDIT *ae, char *pText, DWORD dwTextLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
void AE_ReplaceSel(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
DWORD AE_InsertText(AKELEDIT *ae, const AECHARINDEX *ciInsertPos, wchar_t *wpText, DWORD dwTextLen, int nNewLine, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd, BOOL bEnableUndo, BOOL bUpdate);
wchar_t* AE_GetNextLine(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, int *nLineLen, int *nLineBreak);
int AE_GetNewLineString(AKELEDIT *ae, int nNewLine, wchar_t **wpNewLine);
BOOL AE_StreamOut(AKELEDIT *ae, DWORD dwFlags, AEStreamCallback lpCallback, DWORD dwCookie);
BOOL AE_FindTextAnsi(AKELEDIT *ae, AEFINDTEXTA *ftA);
BOOL AE_FindText(AKELEDIT *ae, AEFINDTEXTW *ft);
BOOL AE_IsMatch(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar);
BOOL AE_GetModify(AKELEDIT *ae);
void AE_SetModify(AKELEDIT *ae, BOOL bState);
BOOL AE_EditCanPaste(AKELEDIT *ae);
BOOL AE_EditCanRedo(AKELEDIT *ae);
BOOL AE_EditCanUndo(AKELEDIT *ae);
void AE_EditUndo(AKELEDIT *ae);
void AE_EditRedo(AKELEDIT *ae);
void AE_EditCut(AKELEDIT *ae);
void AE_EditCopyToClipboard(AKELEDIT *ae);
void AE_EditPasteFromClipboard(AKELEDIT *ae, BOOL bAnsi);
void AE_EditChar(AKELEDIT *ae, WPARAM wParam);
void AE_EditKeyReturn(AKELEDIT *ae);
void AE_EditKeyBackspace(AKELEDIT *ae);
void AE_EditKeyDelete(AKELEDIT *ae);
void AE_EditSelectAll(AKELEDIT *ae);
void AE_AkelEditGetSel(AKELEDIT *ae, AESELECTION *aes);
void AE_AkelEditSetSel(AKELEDIT *ae, AESELECTION *aes);
void AE_RichEditGetSel(AKELEDIT *ae, LONG *nMin, LONG *nMax);
void AE_RichEditSetSel(AKELEDIT *ae, LONG nMin, LONG nMax, BOOL bColumnSel);
void AE_GetColors(AKELEDIT *ae, AECOLORS *aec);
void AE_SetColors(AKELEDIT *ae, AECOLORS *aec);
wchar_t AE_WideCharUpper(wchar_t c);
int AE_WideStrCmp(const wchar_t *wpString, const wchar_t *wpString2);
int AE_WideStrCmpI(const wchar_t *wpString, const wchar_t *wpString2);
wchar_t* AE_wcschr(const wchar_t *s, wchar_t c);
void* AE_memcpy(void *dest, const void *src, unsigned int count);

#endif
