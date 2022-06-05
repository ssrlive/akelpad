#ifndef __AKELBUILD_H__
#define __AKELBUILD_H__


//// Includes

#include <shlobj.h>
#include <imm.h>
#include "AkelEdit.h"
#include "RegExpFunc.h"


//// Defines

#define AEMAX_HEAPCREATE       1000
#define AEMAX_LINKLENGTH       512
#define AEMAX_WORDLENGTH       256
#define AEMAX_DELIMLENGTH      128

//Tabulation
#define AETAB_DEFAULTSIZE      8

//Times
#define AETIME_BEFOREPROGRESS  0
#define AETIME_BEFOREUPDATE    100

//Timer IDs
#define AETIMERID_MOUSEMOVE       1
#define AETIMERID_MOUSESCROLL     2

//Required mouse moves before ...
#define AEMMB_MBUTTONSCROLL  1
#define AEMMB_BEGINDRAG      5
#define AEMMB_DROPSCROLL     60

//WM_SETCURSOR
#define AECC_IBEAM          0
#define AECC_MARGIN         1
#define AECC_SELECTION      2
#define AECC_URL            3
#define AECC_MARKER         4
#define AECC_MCENTER        5
#define AECC_MLEFT          6
#define AECC_MLEFTTOP       7
#define AECC_MTOP           8
#define AECC_MRIGHTTOP      9
#define AECC_MRIGHT         10
#define AECC_MRIGHTBOTTOM   11
#define AECC_MBOTTOM        12
#define AECC_MLEFTBOTTOM    13

//WM_MOUSEACTIVATE
#define AEMA_NONE           0
#define AEMA_LBUTTONDOWN    1
#define AEMA_IGNORE         2

//Print
#define AEPRNL_PRINTLINESIZE 32768

//Highlight search types
#define AEHF_ISFIRSTCHAR    0x00000001
#define AEHF_FINDFIRSTCHAR  0x00000002
#define AEHF_FINDCHILD      0x00000004

//AE_HighlightIsDelimiter flags
#define AEHID_BACK          0x00000001  //Check backward.
#define AEHID_LINEEDGE      0x00000002  //Check AEHLF_ATLINESTART and AEHLF_ATLINEEND flags.

//AE_VPos flags
#define AEVPF_LINEFROMVPOS     0x00000001
#define AEVPF_VPOSFROMLINE     0x00000002

//AE_CalcLinesWidth flags
#define AECLW_FRESH              0x00000001
#define AECLW_LOCKUPDATEHSCROLL  0x00000002

//AE_GetCharInLine flags
#define AECIL_HALFFIT          0x00000001
#define AECIL_USELINEBEGINPOS  0x00000002
#define AECIL_USELINEENDPOS    0x00000004
#define AECIL_USECARETPOS      0x00000008
#define AECIL_ALLPOS          (AECIL_USELINEBEGINPOS |\
                               AECIL_USELINEENDPOS   |\
                               AECIL_USECARETPOS)

//AE_GetNextBreak, AE_GetPrevBreak spaces flags
#define AEWBS_SPACE            0x00000001
#define AEWBS_TAB              0x00000002

//Alt+NumPad input
#define AEAC_NONE       0
#define AEAC_KEYDOWN   -1
#define AEAC_KEYUP     -2
#define AEAC_NUMPAD0   -3
#define AEAC_DECINPUT  -4
#define AEAC_DODEFAULT -5

//StreamIn AELINEDATA.nLineFlags
#define AELF_SPLIT    0x1

//Undo/Redo flags
#define AEUN_INSERT          0x00000001
#define AEUN_DELETE          0x00000002
#define AEUN_SETSEL          0x00000004
#define AEUN_CARETATSTART    0x00000008
#define AEUN_COLUMNSEL       0x00000010
#define AEUN_COLUMNGROUP     0x00000020
#define AEUN_STOPGROUP       0x00000040
#define AEUN_SINGLECHAR      0x00000080
#define AEUN_OVERTYPECHAR    0x00000100
#define AEUN_BACKSPACEKEY    0x00000200
#define AEUN_DELETEKEY       0x00000400
#define AEUN_EXTRAOFFSET     0x00000800
#define AEUN_UNDOONLY        0x00001000
#define AEUN_REDOONLY        0x00002000

#ifndef IDC_HAND
  #define IDC_HAND  MAKEINTRESOURCE(32649)
#endif
#ifndef NOTSRCINVERT
  #define NOTSRCINVERT  (DWORD)0x00990066 /* dest = (NOT source) XOR dest */
#endif
#ifndef COLOR_HOTLIGHT
  #define COLOR_HOTLIGHT  26
#endif
#ifndef AC_SRC_ALPHA
  #define AC_SRC_ALPHA  0x01
#endif
#ifndef WM_MOUSEHWHEEL
  #define WM_MOUSEHWHEEL  0x020E
#endif
#ifndef SPI_GETWHEELSCROLLCHARS
  #define SPI_GETWHEELSCROLLCHARS  0x006C
#endif
#ifndef WM_IME_REQUEST
  #define WM_IME_REQUEST         0x0288
#endif
#ifndef IMR_COMPOSITIONWINDOW
  #define IMR_COMPOSITIONWINDOW  0x0001
#endif
#ifndef IMR_CANDIDATEWINDOW
  #define IMR_CANDIDATEWINDOW    0x0002
#endif
#ifndef IMR_COMPOSITIONFONT
  #define IMR_COMPOSITIONFONT    0x0003
#endif
#ifndef IMR_RECONVERTSTRING
  #define IMR_RECONVERTSTRING    0x0004

  typedef struct tagRECONVERTSTRING {
    DWORD dwSize;
    DWORD dwVersion;
    DWORD dwStrLen;
    DWORD dwStrOffset;
    DWORD dwCompStrLen;
    DWORD dwCompStrOffset;
    DWORD dwTargetStrLen;
    DWORD dwTargetStrOffset;
  } RECONVERTSTRING;
#endif

//Undo element
typedef struct _AEUNDOITEM {
  struct _AEUNDOITEM *next;
  struct _AEUNDOITEM *prev;
  DWORD dwFlags;
  INT_PTR nActionStartOffset;
  INT_PTR nActionEndOffset;
  int nExtraStartOffset;
  int nExtraEndOffset;
  wchar_t *wpText;
  UINT_PTR dwTextLen;
  int nNewLine;
  int nItemId;
} AEUNDOITEM;

//"AkelEditText" clipboard
typedef struct {
  DWORD dwAnsiLen;
  DWORD dwUnicodeLen;
  __int64 dwAnsiLen64;
  __int64 dwUnicodeLen64;
} AECLIPBOARDINFO;


//// OLE Drag'n'Drop

typedef struct _AEIDropTargetCallbackVtbl {
  // IUnknown implementation
  HRESULT (WINAPI *QueryInterface)(LPUNKNOWN, REFIID, void **);
  ULONG (WINAPI *AddRef)(LPUNKNOWN);
  ULONG (WINAPI *Release)(LPUNKNOWN);

  // Methods of the IDropTarget interface
  HRESULT (WINAPI *DragEnter)(LPUNKNOWN, IDataObject *, DWORD, POINTL, DWORD *);
  HRESULT (WINAPI *DragOver)(LPUNKNOWN, DWORD, POINTL, DWORD *);
  HRESULT (WINAPI *DragLeave)(LPUNKNOWN);
  HRESULT (WINAPI *Drop)(LPUNKNOWN, IDataObject *, DWORD, POINTL, DWORD *);
} AEIDropTargetCallbackVtbl;

typedef struct _AEIDropSourceCallbackVtbl {
  // IUnknown implementation
  HRESULT (WINAPI *QueryInterface)(LPUNKNOWN, REFIID, void **);
  ULONG (WINAPI *AddRef)(LPUNKNOWN);
  ULONG (WINAPI *Release)(LPUNKNOWN);

  // Methods of the IDropSource interface
  HRESULT (WINAPI *QueryContinueDrag)(LPUNKNOWN, BOOL, DWORD);
  HRESULT (WINAPI *GiveFeedback)(LPUNKNOWN, DWORD);
} AEIDropSourceCallbackVtbl;

typedef struct _AEIDataObjectCallbackVtbl {
  // IUnknown implementation
  HRESULT (WINAPI *QueryInterface)(LPUNKNOWN, REFIID, void **);
  ULONG (WINAPI *AddRef)(LPUNKNOWN);
  ULONG (WINAPI *Release)(LPUNKNOWN);

  // Methods of the IDataObject interface
  HRESULT (WINAPI *GetData)(LPUNKNOWN, FORMATETC *pFormatEtc, STGMEDIUM *pMedium);
  HRESULT (WINAPI *GetDataHere)(LPUNKNOWN, FORMATETC *pFormatEtc, STGMEDIUM *pMedium);
  HRESULT (WINAPI *QueryGetData)(LPUNKNOWN, FORMATETC *pFormatEtc);
  HRESULT (WINAPI *GetCanonicalFormatEtc)(LPUNKNOWN, FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut);
  HRESULT (WINAPI *SetData)(LPUNKNOWN, FORMATETC *pFormatEtc, STGMEDIUM *pMedium, BOOL fRelease);
  HRESULT (WINAPI *EnumFormatEtc)(LPUNKNOWN, DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
  HRESULT (WINAPI *DAdvise)(LPUNKNOWN, FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
  HRESULT (WINAPI *DUnadvise)(LPUNKNOWN, DWORD dwConnection);
  HRESULT (WINAPI *EnumDAdvise)(LPUNKNOWN, IEnumSTATDATA **ppEnumAdvise);
} AEIDataObjectCallbackVtbl;

typedef struct _AEIEnumFORMATETCCallbackVtbl {
  // IUnknown implementation
  HRESULT (WINAPI *QueryInterface)(LPUNKNOWN, REFIID, void **);
  ULONG (WINAPI *AddRef)(LPUNKNOWN);
  ULONG (WINAPI *Release)(LPUNKNOWN);

  // Methods of the IEnumFORMATETC interface
  HRESULT (WINAPI *Next)(LPUNKNOWN, ULONG celt, FORMATETC *rgelt, ULONG *pceltFetched);
  HRESULT (WINAPI *Skip)(LPUNKNOWN, ULONG celt);
  HRESULT (WINAPI *Reset)(LPUNKNOWN);
  HRESULT (WINAPI *Clone)(LPUNKNOWN, IEnumFORMATETC **ppEnum);
} AEIEnumFORMATETCCallbackVtbl;

typedef struct _AEIDropTarget {
  AEIDropTargetCallbackVtbl *lpTable;
  ULONG uRefCount;
  void *ae;
  BOOL bAllowDrop;
  BOOL bColumnSel;
} AEIDropTarget;

typedef struct _AEIDropSource {
  AEIDropSourceCallbackVtbl *lpTable;
  ULONG uRefCount;
  void *ae;
} AEIDropSource;

typedef struct _AEIDataObject {
  AEIDataObjectCallbackVtbl *lpTable;
  ULONG uRefCount;
  void *ae;
  FORMATETC fmtetc[3];
  STGMEDIUM stgmed[3];
  LONG nNumFormats;
} AEIDataObject;

typedef struct _AEIEnumFORMATETC {
  AEIEnumFORMATETCCallbackVtbl *lpTable;
  ULONG uRefCount;
  void *ae;
  int nPos;
} AEIEnumFORMATETC;


//// Highlight

typedef struct {
  AEDELIMITEMW *first;
  AEDELIMITEMW *last;
} AESTACKDELIM;

typedef struct {
  AEWORDITEMW *first;
  AEWORDITEMW *last;
  INT_PTR lpWordLens[MAX_PATH];
} AESTACKWORD;

typedef struct {
  AEQUOTEITEMW *first;
  AEQUOTEITEMW *last;
  int nElementsAll;
  int nElementsRegExp;
} AESTACKQUOTE;

typedef struct {
  AEMARKRANGEITEM *first;
  AEMARKRANGEITEM *last;
} AESTACKMARKRANGE;

typedef struct {
  AEMARKTEXTITEMW *first;
  AEMARKTEXTITEMW *last;
} AESTACKMARKTEXT;

typedef struct _AEQUOTEITEMHANDLE {
  struct _AEQUOTEITEMHANDLE *next;
  struct _AEQUOTEITEMHANDLE *prev;
  AEQUOTEITEMW *lpQuoteItem;
} AEQUOTEITEMHANDLE;

typedef struct {
  AEQUOTEITEMHANDLE *first;
  AEQUOTEITEMHANDLE *last;
} AESTACKQUOTEITEMHANDLE;

typedef struct _AEQUOTESTART {
  struct _AEQUOTESTART *next;
  struct _AEQUOTESTART *prev;
  const wchar_t *pQuoteStart;
  int nQuoteStartLen;
  wchar_t chEscape;
  DWORD dwFlags;
  int nRuleID;
  int nParentID;

  //Stack with the same pQuoteStart.
  AESTACKQUOTEITEMHANDLE hQuoteItemHandleStack;
} AEQUOTESTART;

typedef struct {
  AEQUOTESTART *first;
  AEQUOTESTART *last;
} AESTACKQUOTESTART;

typedef struct _AETHEMEITEMW {
  struct _AETHEMEITEMW *next;
  struct _AETHEMEITEMW *prev;
  wchar_t wszThemeName[MAX_PATH];
  WPARAM wParam;                      //Value passed to WPARAM of AEM_HLCREATETHEME.
  AESTACKDELIM hDelimiterStack;
  AESTACKWORD hWordStack;
  AESTACKQUOTE hQuoteStack;
  AESTACKQUOTESTART hQuoteStartStack;
  AESTACKMARKTEXT hMarkTextStack;
  AESTACKMARKRANGE hMarkRangeStack;
} AETHEMEITEMW;

typedef struct {
  AETHEMEITEMW *first;
  AETHEMEITEMW *last;
} AESTACKTHEME;

typedef struct {
  HDC hDC;
  AECHARINDEX ciDrawLine;
  INT_PTR nDrawCharOffset;
  INT_PTR nDrawLineWidth;
  POINT64 ptFirstCharInLine;
  wchar_t *wpStartDraw;
  INT_PTR nStartDrawWidth;
  int nMaxDrawCharsCount;
  DWORD dwPrintFlags;
  AEGETHIGHLIGHT *gh;
} AETEXTOUT;


//// WM_ERASEBKGND

typedef struct _AEERASE {
  struct _AEERASE *next;
  struct _AEERASE *prev;
  RECT rcErase;
} AEERASE;


//// Manage GDI objects

typedef struct _AEFONTITEMA {
  struct _AEFONTITEMA *next;
  struct _AEFONTITEMA *prev;
  LOGFONTA lfFont;
  HFONT hFontNormal;
  HFONT hFontBold;
  HFONT hFontItalic;
  HFONT hFontBoldItalic;
  HFONT hFontUrl;
  WORD lpCharWidths[65536];
} AEFONTITEMA;

typedef struct _AEFONTITEMW {
  struct _AEFONTITEMW *next;
  struct _AEFONTITEMW *prev;
  LOGFONTW lfFont;
  HFONT hFontNormal;
  HFONT hFontBold;
  HFONT hFontItalic;
  HFONT hFontBoldItalic;
  HFONT hFontUrl;
  WORD lpCharWidths[65536];
} AEFONTITEMW;

typedef struct {
  int nWidth;
  int nHeight;
  COLORREF crBasic;
  COLORREF crInvert;
} AEBITMAPDATA;

typedef struct _AEBITMAPITEM {
  struct _AEBITMAPITEM *next;
  struct _AEBITMAPITEM *prev;
  AEBITMAPDATA bd;
  HBITMAP hBitmap;
} AEBITMAPITEM;

typedef struct _AEDCITEM {
  struct _AEDCITEM *next;
  struct _AEDCITEM *prev;
  int nRefCount;
  HBITMAP hBitmap;
  HBITMAP hBitmapOld;
  HDC hDC;
  int nBitmapX;
  int nBitmapY;
} AEDCITEM;

typedef struct _AEPENITEM {
  struct _AEPENITEM *next;
  struct _AEPENITEM *prev;
  COLORREF crPenColor;
  HPEN hPen;
} AEPENITEM;


//// Stacks

typedef struct {
  AELINEDATA *first;
  AELINEDATA *last;
} AESTACKLINE;

typedef struct {
  AEPOINT *first;
  AEPOINT *last;
} AESTACKPOINT;

typedef struct {
  AEFOLD *first;
  AEFOLD *last;
} AESTACKFOLD;

typedef struct {
  AEUNDOITEM *first;
  AEUNDOITEM *last;
  int nMaxItemId;
} AESTACKUNDO;

typedef struct {
  AEUNDOITEM *first;
  AEUNDOITEM *last;
  DWORD dwUndoCount;
} AEUNDOATTACH;

typedef struct {
  AEERASE *first;
  AEERASE *last;
} AESTACKERASE;


//// AKELEDIT

typedef struct {
  HANDLE hHeap;
  AESTACKLINE hLinesStack;
  AESTACKPOINT hPointsStack;

  //Font
  HFONT hFont;
  HFONT hFontNormal;
  HFONT hFontBold;
  HFONT hFontItalic;
  HFONT hFontBoldItalic;
  HFONT hFontUrl;
  LOGFONTA lfFontA;
  LOGFONTW lfFontW;
  int nPointSize;
  int nCharHeight;
  int nLineGap;
  int nMaxCharWidth;
  int nAveCharWidth;
  int nSpaceCharWidth;
  int nTabWidth;
  int nTabStop;
  int nInitFixedCharWidth;
  int nFixedCharWidth;
  int nFixedTabWidth;
  WORD *lpCharWidths;
  AELINEINDEX liMaxWidthLine;

  //Scroll
  INT_PTR nHScrollMax;
  INT_PTR nVScrollMax;

  //Text size
  INT_PTR nLastCharOffset;
  int nLineCount;

  //Wrap
  AELINEINDEX liLineUnwrapLastCall;
  int nLineUnwrapCount;
  int nLineUnwrapLastCall;
  UINT_PTR dwTextLimit;
  DWORD dwWordWrap;
  DWORD dwWrapLimit;
  wchar_t wszWrapDelimiters[AEMAX_DELIMLENGTH];
  int nWrapDelimitersLen;

  //Undo/Redo
  AESTACKUNDO hUndoStack;
  AEUNDOITEM *lpCurrentUndo;
  AEUNDOITEM *lpSavePoint;
  BOOL bSavePointExist;
  BOOL bModified;
  BOOL bLockGroupStopInt;
  BOOL bLockGroupStopExt;
  BOOL bLockCollectUndo;
  DWORD dwUndoLimit;
  DWORD dwUndoCount;

  //Visited URLs
  AESTACKURLVISIT hUrlVisitStack;

  //Highlight (default window theme)
  AESTACKDELIM hDelimiterStack;
  AESTACKWORD hWordStack;
  AESTACKQUOTE hQuoteStack;
  AESTACKQUOTESTART hQuoteStartStack;
  AESTACKMARKTEXT hMarkTextStack;
  AESTACKMARKRANGE hMarkRangeStack;

  //Folding
  AESTACKFOLD hFoldsStack;
  int nFoldAllCount;
  int nFoldCollapseCount;
  int nFoldColorCount;
  int nFoldWithIdCount;
  int nFoldWithThemeCount;
  int nHideMinLineOffset;
  int nHideMaxLineOffset;
  AEFOLD *lpFindFoldLastCall;
  AEFOLD *lpIsCollapsedLastCall;
  AEFOLD *lpVPosFold;
  int nVPosFoldHiddenLines;
} AKELTEXT;

typedef struct {
  DWORD dwEventMask;
  DWORD dwRichEventMask;
  DWORD dwOptions;
  DWORD dwOptionsEx;
  int nInputNewLine;
  int nOutputNewLine;
  int nVScrollLock;
  int nHScrollLock;
  DWORD dwVScrollMaxOffset;
  DWORD dwMScrollSpeed;
  BOOL bHideSelection;
  DWORD dwLockUpdate;
  BOOL bHeapSerialize;
  AECOLORS aec;
  AECOLORS aecEnable;
  COLORREF crActiveLineTextWithAltText;
  COLORREF crActiveLineBkWithAltBk;
  COLORREF crActiveLineBorderWithAltBk;
  COLORREF crActiveLineBorderWithAltBorder;
  BOOL bDefaultColors;
  HBRUSH hbrBasicBk;
  AEDCITEM *lpBkImage;
  int nBkImageAlpha;
  HBITMAP hCaretInsert;
  HBITMAP hCaretOvertype;
  int nCaretInsertWidth;
  int nCaretOvertypeHeight;
  DWORD dwCaretScrollFlags;
  DWORD dwCaretScrollSelFlags;
  DWORD dwCaretScrollSelType;
  int nCaretScrollOffsetX;
  int nCaretScrollOffsetY;
  HPEN hActiveColumnPen;
  HPEN hColumnMarkerPen;
  DWORD dwColumnMarkerType;
  DWORD dwColumnMarkerPos;
  DWORD dwAltLineSkip;
  DWORD dwAltLineFill;
  wchar_t wszWordDelimiters[AEMAX_DELIMLENGTH];
  int nWordDelimitersLen;
  DWORD dwWordBreak;
  wchar_t wszUrlLeftDelimiters[AEMAX_DELIMLENGTH];
  int nUrlLeftDelimitersLen;
  wchar_t wszUrlRightDelimiters[AEMAX_DELIMLENGTH];
  int nUrlRightDelimitersLen;
  wchar_t wszUrlPrefixes[AEMAX_DELIMLENGTH];
  int nUrlPrefixesLen;
  wchar_t *lpUrlPrefixes[32];
  DWORD dwUrlMaxLength;
  AETHEMEITEMW *lpActiveTheme;
  DWORD dwHLOptions;
} AKELOPTIONS;

typedef struct _AKELEDIT {
  struct _AKELEDIT *next;
  struct _AKELEDIT *prev;

  //Thread
  int nThreadCount;
  HANDLE hThreadWork;
  HANDLE hThreadMutex;

  //Text
  AKELTEXT *ptxt;
  AKELTEXT txt;

  //Options
  AKELOPTIONS *popt;
  AKELOPTIONS opt;

  //Selection
  AELINEINDEX liFirstDrawLine;
  AECHARINDEX ciSelStartIndex;
  AECHARINDEX ciSelEndIndex;
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciLastCallIndex;
  INT_PTR nFirstDrawLineOffset;
  INT_PTR nSelStartCharOffset;
  INT_PTR nSelEndCharOffset;
  INT_PTR nLastCallOffset;
  INT_PTR nHScrollPos;
  INT_PTR nVScrollPos;
  INT_PTR nLastHScrollPos;
  INT_PTR nLastVScrollPos;
  POINT64 ptCaret;
  INT_PTR nCaretHorzIndent;
  BOOL bColumnSel;

  //Ident
  AEEditProc lpEditProc;
  HWND hWndEdit;
  HWND hWndParent;
  int nEditCtrlID;
  BOOL bUnicodeWindow;
  BOOL bRichEditClass;

  //Painting
  HDC hDC;
  RECT rcEdit;
  RECT rcDraw;
  RECT rcErase;
  AESTACKERASE hEraseStack;
  POINT ptActiveColumnDraw;
  BOOL bHScrollShow;
  BOOL bVScrollShow;
  BOOL bCaretVisible;
  BOOL bPrinting;

  //Input
  BOOL bFocus;
  DWORD dwImeChar;
  BOOL bImeComposition;
  int nAltChar;
  BOOL bBeepEnable;

  //Notification
  DWORD dwNotifyFlags;
  DWORD dwNotifyTextChange;
  DWORD dwNotifySelChange;

  //Cursor
  DWORD dwMouseCapture;
  AECHARRANGE crMouseOnLink;
  DWORD dwMouseSelType;
  AECHARINDEX ciMouseSelClick;
  AECHARINDEX ciMouseSelStart;
  AECHARINDEX ciMouseSelEnd;
  POINT ptLButtonDownPrevPos;
  int nLButtonDownPrevTime;
  int nLButtonDownCount;
  int nMouseActive;
  UINT_PTR dwMouseMoveTimer;
  UINT_PTR dwMouseScrollTimer;
  int nCurrentCursor;
  POINT ptMButtonDown;
  POINT64 ptMButtonScroll;
  HBITMAP hMButtonBitmap;
  BOOL bMButtonBitmapDraw;
  BOOL bMButtonDown;
  BOOL bMButtonUp;
  int nMButtonMoveBeforeScroll;
  DWORD dwInitMarkerPos;

  //OLE Drag'n'Drop
  AEIDropTargetCallbackVtbl idtVtbl;
  AEIDropSourceCallbackVtbl idsVtbl;
  AEIDataObjectCallbackVtbl idoVtbl;
  AEIEnumFORMATETCCallbackVtbl iefVtbl;
  AEIDropTarget idt;
  AEIDropSource ids;
  AEIDataObject ido;
  BOOL bDropping;
  BOOL bDragging;
  BOOL bDragSelectionDelete;
  UINT_PTR dwDragSelectionLength;
  int nMoveBeforeBeginDrag;
  int nMoveBeforeDropScroll;

  //Clone
  HSTACK hClonesStack;
  int nCloneCount;
  struct _AKELEDIT *lpMaster;
  AEPOINT *lpSelStartPoint;
  AEPOINT *lpSelEndPoint;
  AEPOINT *lpCaretPoint;
  //RECT rcCloneMargins;

  //Undo window heap or global heap
  struct _AKELEDIT *aeUndo;
} AKELEDIT;

typedef struct {
  AKELEDIT *first;
  AKELEDIT *last;
} AESTACKEDIT;


//// Clone

typedef struct _AECLONE {
  struct _AECLONE *next;
  struct _AECLONE *prev;
  AKELEDIT *aeClone;
} AECLONE;


//// Print

typedef struct {
  AKELEDIT aePrint;
} AEPRINTHANDLE;


//// Char color callback

typedef struct {
  AKELEDIT *ae;
  AECHARCOLORS *aecc;
} AECHARCOLORSCOOKIE;


//// Functions as macros

#ifndef AE_IsSurrogate
  #define AE_IsSurrogate(c)  ((wchar_t)(c) >= 0xD800 && (wchar_t)(c) <= 0xDFFF)
#endif

#ifndef AE_IsHighSurrogate
  #define AE_IsHighSurrogate(c)  ((wchar_t)(c) >= 0xD800 && (wchar_t)(c) <= 0xDBFF)
#endif

#ifndef AE_IsLowSurrogate
  #define AE_IsLowSurrogate(c)  ((wchar_t)(c) >= 0xDC00 && (wchar_t)(c) <= 0xDFFF)
#endif

#ifndef AE_FirstCollapsibleLine
  #define AE_FirstCollapsibleLine(ae, lpFold)  (((AEFOLD *)lpFold)->lpMinPoint->ciPoint.nLine + ((AKELEDIT *)ae)->ptxt->nHideMinLineOffset)
#endif


//// Functions prototypes

BOOL AE_RegisterClassA(HINSTANCE hInstance, BOOL bRegRichEdit, BOOL bRegAkelEditW, BOOL bRegRichEditW);
BOOL AE_RegisterClassW(HINSTANCE hInstance, BOOL bRegisterRichEdit);
void AE_RegisterClassCommon(HINSTANCE hInstance);
BOOL AE_UnregisterClassA(HINSTANCE hInstance);
BOOL AE_UnregisterClassW(HINSTANCE hInstance);
void AE_UnregisterClassCommon(HINSTANCE hInstance);
LRESULT CALLBACK AE_EditShellProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AE_EditProc(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam);
AKELEDIT* AE_CreateWindowData(HWND hWnd, CREATESTRUCTA *cs, AEEditProc lpEditProc);
AKELEDIT* AE_SetWindowData(AKELEDIT *aeOld, AKELEDIT *aeNew, DWORD dwFlags);
void AE_DestroyWindowData(AKELEDIT *ae);
HANDLE AE_HeapCreate(AKELEDIT *ae);
LPVOID AE_HeapAlloc(AKELEDIT *ae, DWORD dwFlags, SIZE_T dwBytes);
BOOL AE_HeapFree(AKELEDIT *ae, DWORD dwFlags, LPVOID lpMem);
int AE_HeapStackInsertIndex(AKELEDIT *ae, stack **first, stack **last, stack **element, int nIndex, int nBytes);
int AE_HeapStackInsertBefore(AKELEDIT *ae, stack **first, stack **last, stack *index, stack **element, int nBytes);
int AE_HeapStackInsertAfter(AKELEDIT *ae, stack **first, stack **last, stack *index, stack **element, int nBytes);
int AE_HeapStackMoveBefore(AKELEDIT *ae, stack **first, stack **last, stack *src, stack *dst);
int AE_HeapStackDelete(AKELEDIT *ae, stack **first, stack **last, stack *element);
void AE_HeapStackClear(AKELEDIT *ae, stack **first, stack **last);
AKELEDIT* AE_StackWindowInsert(AESTACKEDIT *hStack);
AKELEDIT* AE_StackWindowGet(AESTACKEDIT *hStack, HWND hWndEdit);
BOOL AE_StackWindowValid(AESTACKEDIT *hStack, AKELEDIT *ae);
void AE_StackWindowMakeFirst(AESTACKEDIT *hStack, AKELEDIT *ae);
void AE_StackWindowFree(AESTACKEDIT *hStack);
AECLONE* AE_StackCloneIndex(AKELEDIT *ae, DWORD dwIndex);
AECLONE* AE_StackCloneGet(AKELEDIT *aeMaster, AKELEDIT *aeClone);
AECLONE* AE_StackCloneAdd(AKELEDIT *aeMaster, AKELEDIT *aeClone);
void AE_CloneActivate(AKELEDIT *lpAkelEditPrev, AKELEDIT *ae);
void AE_CloneRestoreSelection(AKELEDIT *ae);
void AE_StackCloneDelete(AECLONE *aec);
void AE_StackCloneDeleteAll(AKELEDIT *ae);
void AE_StackCloneUpdate(AKELEDIT *ae);
AKELEDIT* AE_StackDraggingGet(AKELEDIT *ae);
AEERASE* AE_StackEraseInsert(AKELEDIT *ae, RECT *rcErase);
BOOL AE_StackEraseMore(AKELEDIT *ae, RECT *rcErase);
void AE_StackEraseDelete(AKELEDIT *ae, AEERASE *lpErase);
void AE_StackEraseFree(AKELEDIT *ae);
AEFONTITEMA* AE_StackFontItemInsertA(HSTACK *hStack, LOGFONTA *lfFont);
AEFONTITEMW* AE_StackFontItemInsertW(HSTACK *hStack, LOGFONTW *lfFont);
AEFONTITEMA* AE_StackFontItemGetA(HSTACK *hStack, LOGFONTA *lfFont);
AEFONTITEMW* AE_StackFontItemGetW(HSTACK *hStack, LOGFONTW *lfFont);
void AE_StackFontItemsFreeA(HSTACK *hStack);
void AE_StackFontItemsFreeW(HSTACK *hStack);
AEBITMAPITEM* AE_StackBitmapItemInsert(HSTACK *hStack, AEBITMAPDATA *bd);
AEBITMAPITEM* AE_StackBitmapItemGet(HSTACK *hStack, AEBITMAPDATA *bd);
void AE_StackBitmapItemsFree(HSTACK *hStack);
AEDCITEM* AE_StackDcItemInsert(HSTACK *hStack, HBITMAP hBitmap);
AEDCITEM* AE_StackDcItemGet(HSTACK *hStack, HBITMAP hBitmap);
void AE_StackDcItemDelete(HSTACK *hStack, AEDCITEM *lpElement);
void AE_StackDcItemsFree(HSTACK *hStack);
AEPENITEM* AE_StackPenItemInsert(HSTACK *hStack, COLORREF crPenColor, COLORREF crInvertColor);
AEPENITEM* AE_StackPenItemGet(HSTACK *hStack, COLORREF crPenColor, COLORREF crInvertColor);
void AE_StackPenItemsFree(HSTACK *hStack);
AEFOLD* AE_StackFoldInsert(AKELEDIT *ae, const AEFOLD *lpFold);
void AE_StackFindFold(AKELEDIT *ae, DWORD dwFlags, UINT_PTR dwFindIt, AEFOLD *lpForce, AEFOLD **lpParentOut, AEFOLD **lpPrevSublingOut);
BOOL AE_StackFoldIsValid(AKELEDIT *ae, AEFOLD *lpFold);
BOOL AE_StackFoldDelete(AKELEDIT *ae, AEFOLD *lpFold);
int AE_StackFoldFree(AKELEDIT *ae);
int AE_LastCollapsibleLine(AKELEDIT *ae, AEFOLD *lpFold);
AEFOLD* AE_IsLineCollapsed(AKELEDIT *ae, int nLine);
int AE_LineCollapse(AKELEDIT *ae, int nLine, DWORD dwFlags);
int AE_FoldCollapse(AKELEDIT *ae, AEFOLD *lpFold, DWORD dwFlags);
void AE_FoldScroll(AKELEDIT *ae, AEFOLD *lpFold, DWORD dwFlags);
INT_PTR AE_FoldUpdate(AKELEDIT *ae, int nFirstVisibleLine);
int AE_LineFromVPos(AKELEDIT *ae, INT_PTR nVPos);
INT_PTR AE_VPosFromLine(AKELEDIT *ae, int nLine);
INT_PTR AE_GetVScrollMax(AKELEDIT *ae);
AEPOINT* AE_StackPointInsert(AKELEDIT *ae, AECHARINDEX *ciPoint);
void AE_StackPointUnset(AKELEDIT *ae, DWORD dwFlags);
void AE_StackPointUnreserve(AKELEDIT *ae);
void AE_StackPointReset(AKELEDIT *ae, DWORD dwType);
void AE_StackPointDelete(AKELEDIT *ae, AEPOINT *lpElement);
void AE_StackPointFree(AKELEDIT *ae);
AEUNDOITEM* AE_StackUndoItemInsert(AKELEDIT *ae);
void AE_StackUndoItemDelete(AKELEDIT *ae, AEUNDOITEM *lpItem);
void AE_StackRedoDeleteAll(AKELEDIT *ae, AEUNDOITEM *lpItem);
UINT_PTR AE_StackUndoSize(AKELEDIT *ae);
void AE_StackUndoResetId(AKELEDIT *ae);
int AE_StackIsRangeModified(AKELEDIT *ae, const CHARRANGE64 *lpcrRange);
INT_PTR AE_StackGetUndoPos(AKELEDIT *ae, const AEUNDOITEM *lpCurrentUndo, const AEUNDOITEM *lpUndoItem);
AEUNDOATTACH* AE_StackUndoDetach(AKELEDIT *ae);
BOOL AE_StackUndoAttach(AKELEDIT *ae, AEUNDOATTACH *hUndoAttach);
wchar_t* AE_GetAllTextForUndo(AKELEDIT *ae, UINT_PTR *lpdwUndoTextLen);
void AE_StackUndoGroupStop(AKELEDIT *ae);
AELINEDATA* AE_StackLineAdd(AKELEDIT *ae);
AELINEDATA* AE_StackLineInsertBefore(AKELEDIT *ae, AELINEDATA *lpLine);
AELINEDATA* AE_StackLineInsertAfter(AKELEDIT *ae, AELINEDATA *lpLine);
void AE_StackLineDelete(AKELEDIT *ae, AELINEDATA *lpElement);
void AE_StackLineFree(AKELEDIT *ae);
AELINEDATA* AE_GetLineData(AKELEDIT *ae, int nLine);
int AE_GetLineNumber(AKELEDIT *ae, int nType, INT_PTR nCharOffset);
int AE_GetWrapLine(AKELEDIT *ae, int nLine, AECHARINDEX *ciCharOut);
int AE_GetUnwrapLine(AKELEDIT *ae, int nLine);
void AE_RichOffsetToAkelIndex(AKELEDIT *ae, INT_PTR nOffset, AECHARINDEX *ciCharIndex);
INT_PTR AE_AkelIndexToRichOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIndex);
AELINEDATA* AE_GetIndex(AKELEDIT *ae, int nType, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut);
INT_PTR AE_GetRichOffset(AKELEDIT *ae, int nType, INT_PTR nCharOffsetOrLine);
INT_PTR AE_IndexSubtract(AKELEDIT *ae, const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
UINT_PTR AE_IndexOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, INT_PTR nOffset, int nNewLine);
BOOL AE_IndexNormalize(AECHARINDEX *ciChar);
BOOL AE_IndexUpdate(AKELEDIT *ae, AECHARINDEX *ciChar);
int AE_UpdateWrap(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwWrap);
int AE_WrapLines(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwWrap);
int AE_LineWrap(AKELEDIT *ae, const AELINEINDEX *liLine, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwMaxWidth, DWORD dwWrap, AEPOINT **lppPoint, int nLineOffset);
int AE_LineUnwrap(AKELEDIT *ae, AELINEINDEX *liLine, DWORD dwMaxWidth, AEPOINT **lppPoint, int nLineOffset);
void AE_CalcLinesWidth(AKELEDIT *ae, const AELINEINDEX *liStartLine, const AELINEINDEX *liEndLine, DWORD dwFlags);
int AE_CheckCodepage(AKELEDIT *ae, int nCodePage, int *lpdwCharInLine);
void AE_SetDrawRect(AKELEDIT *ae, const RECT *lprcDraw, BOOL bRedraw);
void AE_GetFontCharWidth(AKELEDIT *ae, HDC hDC);
int AE_GetFixedCharWidth(AKELEDIT *ae);
void AE_SetEditFontA(AKELEDIT *ae, HFONT hFont, BOOL bRedraw);
void AE_SetEditFontW(AKELEDIT *ae, HFONT hFont, BOOL bRedraw);
void AE_GetLineSelection(AKELEDIT *ae, const AELINEINDEX *liLine, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, INT_PTR nSelStartX, INT_PTR nSelEndX, int *nSelStartIndexInLine, int *nSelEndIndexInLine, BOOL bColumnSel);
void AE_GetSelLines(AKELEDIT *ae, const AECHARINDEX *ciMin, const AECHARINDEX *ciMax, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, INT_PTR nSelStartX, INT_PTR nSelEndX, BOOL bColumnSel);
void AE_ClearSelLines(const AECHARINDEX *ciMin, const AECHARINDEX *ciMax);
void AE_SetSelectionPos(AKELEDIT *ae, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, BOOL bColumnSel, DWORD dwSelFlags, DWORD dwSelType);
void AE_UpdateSelection(AKELEDIT *ae, DWORD dwSelFlags);
void AE_SetMouseSelection(AKELEDIT *ae, const POINT *ptPos, BOOL bColumnSel, BOOL bShift);
void AE_MouseMove(AKELEDIT *ae);
void AE_SetMouseCapture(AKELEDIT *ae, DWORD dwType);
void AE_ReleaseMouseCapture(AKELEDIT *ae, DWORD dwType);
void AE_GlobalToClient(AKELEDIT *ae, const POINT64 *ptGlobal, POINT64 *ptClient64, POINT *ptClient);
void AE_ClientToGlobal(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY, POINT64 *ptGlobal);
int AE_SetCursor(AKELEDIT *ae);
DWORD AE_IsPointOnMargin(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY);
BOOL AE_IsPointOnSelection(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY);
DWORD AE_IsPointOnUrl(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY, AECHARRANGE *crLink);
BOOL AE_IsPointOnMarker(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY);
AEURLVISIT* AE_UrlVisitInsert(AKELEDIT *ae, const AECHARRANGE *crUrl);
AEURLVISIT* AE_UrlVisitGetByRange(AKELEDIT *ae, const AECHARRANGE *crUrl);
AEURLVISIT* AE_UrlVisitGetByText(AKELEDIT *ae, const wchar_t *wpText);
void AE_UrlVisitDelete(AKELEDIT *ae, AEURLVISIT *lpUrlVisit);
void AE_UrlVisitFree(AKELEDIT *ae);
DWORD AE_HighlightFindUrl(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, int nLastLine, AECHARRANGE *crLink);
BOOL AE_HighlightFindMarkText(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEMARKTEXTMATCH *mtm);
AEMARKTEXTITEMW* AE_HighlightIsMarkText(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar, AECHARINDEX *ciMaxRE, AESTACKMARKTEXT *lpMarkTextStack);
INT_PTR AE_HighlightFindMarkRange(AKELEDIT *ae, INT_PTR nCharOffset, AEMARKRANGEMATCH *mrm);
INT_PTR AE_HighlightFindQuote(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEHLPAINT *hlp);
int AE_HighlightFindWord(AKELEDIT *ae, const AECHARINDEX *ciChar, INT_PTR nCharOffset, DWORD dwSearchType, AEWORDMATCH *wm, AEQUOTEMATCH *qm, AEFOLDMATCH *fm);
AEDELIMITEMW* AE_HighlightIsDelimiter(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar, DWORD dwFlags, AEQUOTEITEMW *lpQuote, AEFOLDMATCH *fm);
AEWORDITEMW* AE_HighlightIsWord(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARRANGE *crWord, int nWordLen, AEQUOTEITEMW *lpQuote, AEFOLDMATCH *fm);
AETHEMEITEMW* AE_HighlightCreateTheme(WPARAM wParam, wchar_t *wpThemeName);
AETHEMEITEMW* AE_HighlightGetTheme(wchar_t *wpThemeName);
BOOL AE_HighlightIsThemeExists(AETHEMEITEMW *aeti);
void AE_HighlightDeleteTheme(AKELEDIT *ae, AETHEMEITEMW *aeti);
void AE_HighlightDeleteThemeAll();
void AE_HighlightUnsetTheme(AETHEMEITEMW *aeti);
AEDELIMITEMW* AE_HighlightInsertDelimiter(AKELEDIT *ae, AETHEMEITEMW *aeti, int nDelimiterLen, int nIndex);
void AE_HighlightDeleteDelimiter(AKELEDIT *ae, AETHEMEITEMW *aeti, AEDELIMITEMW *aedi);
void AE_HighlightDeleteDelimiterAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
AEWORDITEMW* AE_HighlightInsertWord(AKELEDIT *ae, AETHEMEITEMW *aeti, int nWordLen);
void AE_HighlightDeleteWord(AKELEDIT *ae, AETHEMEITEMW *aeti, AEWORDITEMW *aewi);
void AE_HighlightDeleteWordAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
AEQUOTEITEMW* AE_HighlightAddQuote(AKELEDIT *ae, AETHEMEITEMW *lpTheme, AEQUOTEITEMW *lpQuoteSrc, AEQUOTEITEMW *lpQuoteDst);
AEQUOTEITEMW* AE_HighlightInsertQuote(AKELEDIT *ae, AETHEMEITEMW *aeti, int nIndex);
AEQUOTESTART* AE_HighlightInsertQuoteStart(AKELEDIT *ae, AETHEMEITEMW *aeti, AEQUOTEITEMW *lpQuoteItem);
void AE_HighlightDeleteQuoteData(AEQUOTEITEMW *aeqi);
void AE_HighlightDeleteQuote(AKELEDIT *ae, AETHEMEITEMW *aeti, AEQUOTEITEMW *aeqi);
void AE_HighlightDeleteQuoteAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
AEMARKTEXTITEMW* AE_HighlightAddMarkText(AKELEDIT *ae, AETHEMEITEMW *lpTheme, AEMARKTEXTITEMW *lpMarkTextSrc, AEMARKTEXTITEMW *lpMarkTextDst);
AEMARKTEXTITEMW* AE_HighlightInsertMarkText(AKELEDIT *ae, AETHEMEITEMW *aeti, int nIndex);
void AE_HighlightDeleteMarkTextData(AEMARKTEXTITEMW *aemti);
void AE_HighlightDeleteMarkText(AKELEDIT *ae, AETHEMEITEMW *aeti, AEMARKTEXTITEMW *aemti);
void AE_HighlightDeleteMarkTextAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
AEMARKRANGEITEM* AE_HighlightInsertMarkRange(AKELEDIT *ae, AETHEMEITEMW *aeti, int nIndex);
void AE_HighlightDeleteMarkRange(AKELEDIT *ae, AETHEMEITEMW *aeti, AEMARKRANGEITEM *aemri);
void AE_HighlightDeleteMarkRangeAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
HBITMAP AE_CreateBitmap(int nWidth, int nHeight, COLORREF crBasic, COLORREF crInvert);
HBITMAP AE_LoadBitmapFromMemory(const BYTE *lpBmpFileData);
BOOL AE_UpdateCaret(AKELEDIT *ae, BOOL bFocus);
BOOL AE_SetCaretPos(AKELEDIT *ae, const POINT64 *ptCaret);
void AE_SetCaretVis(AKELEDIT *ae, const POINT64 *ptCaret);
void AE_ScrollToCaret(AKELEDIT *ae, const POINT64 *ptCaret, DWORD dwSelFlags, DWORD dwSelType);
DWORD AE_ScrollToPoint(AKELEDIT *ae, POINT64 *ptPosition);
DWORD AE_ScrollToPointEx(AKELEDIT *ae, DWORD dwFlags, POINT64 *ptPosition, int nOffsetX, int nOffsetY);
void AE_UpdateScrollBars(AKELEDIT *ae, int nBar);
INT_PTR AE_ScrollEditWindow(AKELEDIT *ae, int nBar, INT_PTR nPos);
void AE_SetScrollPos(AKELEDIT *ae, INT_PTR nHPos, INT_PTR nVPos);
INT_PTR AE_HScroll(AKELEDIT *ae, int nAction, DWORD dwAlign);
INT_PTR AE_VScroll(AKELEDIT *ae, int nAction, DWORD dwAlign);
INT_PTR AE_HScrollChar(AKELEDIT *ae, int nChar, DWORD dwAlign);
INT_PTR AE_VScrollLine(AKELEDIT *ae, int nLine, DWORD dwAlign);
INT_PTR AE_AlignPos(AKELEDIT *ae, INT_PTR nPos, DWORD dwAlign);
AEPRINTHANDLE* AE_StartPrintDocA(AKELEDIT *ae, AEPRINT *prn);
AEPRINTHANDLE* AE_StartPrintDocW(AKELEDIT *ae, AEPRINT *prn);
void AE_GetPrintRect(AEPRINT *prn, const RECT *rcMargins, RECT *rcPage);
BOOL AE_PrintPage(AKELEDIT *ae, AEPRINTHANDLE *ph, AEPRINT *prn);
void AE_EndPrintDoc(AKELEDIT *ae, AEPRINTHANDLE *ph, AEPRINT *prn);
void AE_FillRect(AKELEDIT *ae, HDC hDC, const RECT *lpRect, HBRUSH hbr);
void AE_FillRectWithBorder(AKELEDIT *ae, HDC hDC, const RECT *lpRect, HBRUSH hbrDefaultBk, HBRUSH hbrBorderTop, HBRUSH hbrBorderBottom);
void AE_Paint(AKELEDIT *ae, const RECT *lprcUpdate);
void AE_PaintTextOut(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp);
REGROUP* AE_PatCharInGroup(STACKREGROUP *hStack, const AECHARINDEX *ciChar, REGROUP **lppREGroupEnd);
void AE_PaintCheckHighlightOpenItem(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, int nLastDrawLine);
void AE_PaintCheckHighlightCloseItem(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp);
void AE_PaintCheckHighlightCleanUp(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, AECHARINDEX *ciChar);
void AE_PaintCheckHighlightReset(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, AECHARINDEX *ciChar);
int AE_HighlightAllowed(AEQUOTEITEMW *lpQuote, AEFOLDMATCH *fm, int nParentID, int nQuoteRuleID, const AECHARINDEX *ciChar);
void AE_SetDefaultStyle(AEHLPAINT *hlp, int nParentType);
void AE_GetHighLight(AKELEDIT *ae, AEGETHIGHLIGHT *gh);
void AE_MButtonDraw(AKELEDIT *ae);
void AE_MButtonErase(AKELEDIT *ae);
void AE_ActiveColumnDraw(AKELEDIT *ae, HDC hDC, int nTop, int nBottom);
void AE_UpdateSize(AKELEDIT *ae);
void AE_UpdateEditWindow(HWND hWndEdit, BOOL bErase);
BOOL AE_ColumnMarkerSet(AKELEDIT *ae, DWORD dwType, int nPos, BOOL bMouse);
void AE_ColumnMarkerDraw(AKELEDIT *ae, HDC hDC, int nTop, int nBottom);
void AE_ColumnMarkerErase(AKELEDIT *ae);
void AE_RedrawLineRange(AKELEDIT *ae, int nFirstLine, int nLastLine, BOOL bErase);
void AE_RedrawIndexRange(AKELEDIT *ae, const AECHARINDEX *ciFirstChar, const AECHARINDEX *ciLastChar, BOOL bErase);
void AE_HideSelection(AKELEDIT *ae, BOOL bHide);
int AE_GetFirstVisibleLine(AKELEDIT *ae);
int AE_GetLastVisibleLine(AKELEDIT *ae);
int AE_GetFirstFullVisibleLine(AKELEDIT *ae);
int AE_GetLastFullVisibleLine(AKELEDIT *ae);
int AE_GetTextExtentPoint32(AKELEDIT *ae, const wchar_t *wpString, int nStringLen, SIZE64 *lpSize);
int AE_GetCharWidth(AKELEDIT *ae, wchar_t *wpChar, INT_PTR nCharExtent);
INT_PTR AE_GetStringWidth(AKELEDIT *ae, wchar_t *wpString, int nStringLen, INT_PTR nFirstCharExtent);
void AE_GetLineWidth(AKELEDIT *ae, AELINEDATA *lpLine);
BOOL AE_GetPosFromChar(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT64 *ptGlobalPos, POINT *ptClientPos);
int AE_GetCharInLine(AKELEDIT *ae, const AELINEDATA *lpLine, INT_PTR nMaxExtent, DWORD dwFlags, int *nCharIndex, INT_PTR *nCharPos, BOOL bColumnSel);
BOOL AE_GetCharRangeInLine(AKELEDIT *ae, AELINEDATA *lpLine, INT_PTR nMinExtent, INT_PTR nMaxExtent, int *nMinCharIndex, INT_PTR *nMinCharPos, int *nMaxCharIndex, INT_PTR *nMaxCharPos, BOOL bColumnSel);
int AE_GetCharFromPos(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY, AECHARINDEX *ciCharIndex, POINT64 *ptGlobalPos, BOOL bColumnSel);
int AE_GetNextBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciNextBreak, BOOL bColumnSel, DWORD dwFlags);
int AE_GetPrevBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciPrevBreak, BOOL bColumnSel, DWORD dwFlags);
BOOL AE_IsSpacesFromLeft(const AECHARINDEX *ciChar);
BOOL AE_IsSpacesFromRight(const AECHARINDEX *ciChar);
BOOL AE_IsEscaped(const AECHARINDEX *ciChar, wchar_t wchEscape);
BOOL AE_IsDelimiter(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwType);
BOOL AE_IsInDelimiterList(const wchar_t *wpList, int nListLen, wchar_t c);
BOOL AE_IsSpace(wchar_t c, DWORD dwSpacesFlags);
int AE_GetUrlPrefixes(AKELEDIT *ae);
UINT_PTR AE_GetTextRangeAnsi(AKELEDIT *ae, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefaultChar, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, char *szBuffer, UINT_PTR dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
UINT_PTR AE_GetTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, wchar_t *wszBuffer, UINT_PTR dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
UINT_PTR AE_SetTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, UINT_PTR dwTextLen, int nNewLine);
UINT_PTR AE_SetText(AKELEDIT *ae, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine, BOOL bOnInitWindow);
UINT_PTR AE_StreamIn(AKELEDIT *ae, DWORD dwFlags, AESTREAMIN *aesi);
void AE_FixEdit(AKELEDIT *ae, BOOL bJoinNewLines);
int AE_JoinNewLines(AKELEDIT *ae);
UINT_PTR AE_StreamOut(AKELEDIT *ae, DWORD dwFlags, AESTREAMOUT *aeso);
BOOL AE_StreamOutHelper(AESTREAMOUT *aeso, AECHARINDEX *ciCount, const AECHARINDEX *ciEnd, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufCount, UINT_PTR *dwResult);
void AE_AppendTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, UINT_PTR dwTextLen, int nNewLine);
void AE_AppendText(AKELEDIT *ae, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine);
void AE_ReplaceSelAnsi(AKELEDIT *ae, int nCodePage, const char *pText, UINT_PTR dwTextLen, int nNewLine, DWORD dwFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
void AE_ReplaceSel(AKELEDIT *ae, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine, DWORD dwFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
INT_PTR AE_DeleteTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, BOOL bColumnSel, DWORD dwDeleteFlags);
UINT_PTR AE_InsertText(AKELEDIT *ae, const AECHARINDEX *ciInsertPos, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine, BOOL bColumnSel, DWORD dwInsertFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
wchar_t* AE_GetNextLine(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int *nLineLen, BYTE *nLineBreak);
DWORD AE_GetLinesCount(const wchar_t *wpText, INT_PTR nTextLen);
int AE_GetNewLineString(int nNewLine, const wchar_t **wpNewLine);
BOOL AE_FindTextAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA);
BOOL AE_FindText(AKELEDIT *ae, AEFINDTEXTW *ft);
UINT_PTR AE_IsMatchAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA, const AECHARINDEX *ciChar);
UINT_PTR AE_IsMatch(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar);
UINT_PTR AE_IsMatchRE(void *lpREGroupStack, AECHARRANGE *crFound, const AECHARINDEX *ciChar, const AECHARINDEX *ciMaxRE);
void AE_UpdateCompositionPos(AKELEDIT *ae, HIMC hImc);
void AE_UpdateCandidatePos(AKELEDIT *ae, HIMC hImc);
BOOL AE_GetModify(AKELEDIT *ae);
void AE_SetModify(AKELEDIT *ae, BOOL bState);
void AE_EmptyUndoBuffer(AKELEDIT *ae);
BOOL AE_IsReadOnly(AKELEDIT *ae);
BOOL AE_MessageBeep(AKELEDIT *ae, UINT uType);
BOOL AE_KeyDown(AKELEDIT *ae, int nVk, BOOL bAlt, BOOL bShift, BOOL bControl);
void AE_ImeComplete(AKELEDIT *ae);
BOOL AE_EditCanPaste(AKELEDIT *ae);
BOOL AE_EditCanRedo(AKELEDIT *ae);
BOOL AE_EditCanUndo(AKELEDIT *ae);
BOOL AE_EditUndo(AKELEDIT *ae);
BOOL AE_EditRedo(AKELEDIT *ae);
BOOL AE_NoSelectionRange(AKELEDIT *ae, AECHARRANGE *cr, DWORD dwFlags);
BOOL AE_EditCut(AKELEDIT *ae, DWORD dwFlags, int nNewLine);
BOOL AE_EditCopyToClipboard(AKELEDIT *ae, AECHARRANGE *cr, int nNewLine, BOOL bColumnSel);
INT_PTR AE_EditPasteFromClipboard(AKELEDIT *ae, DWORD dwFlags, int nNewLine);
void AE_EditChar(AKELEDIT *ae, WPARAM wParam, BOOL bUnicode);
void AE_EditKeyReturn(AKELEDIT *ae);
void AE_EditKeyBackspace(AKELEDIT *ae, BOOL bControl);
void AE_EditKeyDelete(AKELEDIT *ae, BOOL bControl);
void AE_EditSelectAll(AKELEDIT *ae, DWORD dwSelFlags, DWORD dwSelType);
BOOL AE_AkelEditGetSel(AKELEDIT *ae, AESELECTION *aes, AECHARINDEX *lpciCaret);
void AE_AkelEditSetSel(AKELEDIT *ae, const AESELECTION *aes, const AECHARINDEX *lpciCaret);
INT_PTR AE_RichEditGetSel(AKELEDIT *ae, INT_PTR *nMin, INT_PTR *nMax);
void AE_RichEditSetSel(AKELEDIT *ae, INT_PTR nMin, INT_PTR nMax);
void AE_GetColors(AKELEDIT *ae, AECOLORS *aec);
void AE_SetColors(AKELEDIT *ae, const AECOLORS *aec, BOOL bUpdate);
COLORREF AE_ColorCombine(COLORREF crColor1Cur, COLORREF crColor2NewBase, COLORREF crColor3CurBase);
COLORREF AE_ColorBrightness(COLORREF crColor, int nPercent);
COLORREF AE_ColorSmooth(COLORREF crColor, int nPercent);
COLORREF AE_GetColorFromStr(const wchar_t *wpColor, const wchar_t **wpNext);
BOOL AE_GetBasicCharColors(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARCOLORS *aecc);
BOOL AE_GetHighLightCharColors(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARCOLORS *aecc);
DWORD CALLBACK AE_GetHighLightCharColorCallback(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp);
void AE_NotifyErrSpace(AKELEDIT *ae, SIZE_T dwBytes);
void AE_NotifySetFocus(AKELEDIT *ae, HWND hWndLost);
void AE_NotifyKillFocus(AKELEDIT *ae, HWND hWndReceive);
void AE_NotifyHScroll(AKELEDIT *ae);
void AE_NotifyVScroll(AKELEDIT *ae);
void AE_NotifySetRect(AKELEDIT *ae);
void AE_NotifyPaint(AKELEDIT *ae, DWORD dwType, AENPAINT *pnt);
void AE_NotifyMaxText(AKELEDIT *ae);
DWORD AE_NotifyProgress(AKELEDIT *ae, DWORD dwType, DWORD dwTimeElapsed, INT_PTR nCurrent, INT_PTR nMaximum);
void AE_NotifyModify(AKELEDIT *ae);
void AE_NotifySelChanging(AKELEDIT *ae, DWORD dwType);
void AE_NotifySelChanged(AKELEDIT *ae);
void AE_NotifyTextChanging(AKELEDIT *ae, DWORD dwType);
void AE_NotifyTextChanged(AKELEDIT *ae);
void AE_NotifyChanging(AKELEDIT *ae, DWORD dwType);
void AE_NotifyChanged(AKELEDIT *ae);
void AE_NotifyPoint(AKELEDIT *ae, DWORD dwType, AEPOINT *lpPoint);
BOOL AE_NotifyDropFiles(AKELEDIT *ae, HDROP hDrop);
BOOL AE_NotifyDropSource(AKELEDIT *ae, int nAction, DWORD *lpdwEffect, DWORD dwDropResult);
BOOL AE_NotifyDropTarget(AKELEDIT *ae, int nAction, POINT *pt, DWORD *lpdwEffect);
BOOL AE_NotifyLink(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam, const AECHARRANGE *crLink);
void AE_NotifyMarker(AKELEDIT *ae, BOOL bMouse);
BOOL AE_NotifyMsgFilter(AKELEDIT *ae, UINT uMsg, WPARAM *wParam, LPARAM *lParam);
void AE_SendEraseBackground(AKELEDIT *ae, HDC hDC);
LRESULT AE_SendMessage(AKELEDIT *ae, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AE_ChangeTwoBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst);
wchar_t* AE_wcschr(const wchar_t *s, wchar_t c, BOOL bMatchCase);

HRESULT WINAPI AEIDropTarget_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj);
ULONG WINAPI AEIDropTarget_AddRef(LPUNKNOWN lpTable);
ULONG WINAPI AEIDropTarget_Release(LPUNKNOWN lpTable);
HRESULT WINAPI AEIDropTarget_DragEnter(LPUNKNOWN lpTable, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
HRESULT WINAPI AEIDropTarget_DragOver(LPUNKNOWN lpTable, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
HRESULT WINAPI AEIDropTarget_DragLeave(LPUNKNOWN lpTable);
HRESULT WINAPI AEIDropTarget_Drop(LPUNKNOWN lpTable, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
DWORD AE_DropTargetDropEffect(DWORD grfKeyState, DWORD dwAllowed);
void AE_DropTargetDropCursor(AEIDropTarget *pDropTarget, POINTL *pt, DWORD *pdwEffect);
HRESULT WINAPI AEIDropSource_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj);
ULONG WINAPI AEIDropSource_AddRef(LPUNKNOWN lpTable);
ULONG WINAPI AEIDropSource_Release(LPUNKNOWN lpTable);
HRESULT WINAPI AEIDropSource_QueryContinueDrag(LPUNKNOWN lpTable, BOOL fEscapePressed, DWORD grfKeyState);
HRESULT WINAPI AEIDropSource_GiveFeedback(LPUNKNOWN lpTable, DWORD dwEffect);
HRESULT WINAPI AEIDataObject_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj);
ULONG WINAPI AEIDataObject_AddRef(LPUNKNOWN lpTable);
ULONG WINAPI AEIDataObject_Release(LPUNKNOWN lpTable);
HRESULT WINAPI AEIDataObject_GetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium);
HRESULT WINAPI AEIDataObject_GetDataHere(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium);
HRESULT WINAPI AEIDataObject_QueryGetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc);
HRESULT WINAPI AEIDataObject_GetCanonicalFormatEtc(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, FORMATETC *pFormatEtcOut);
HRESULT WINAPI AEIDataObject_SetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium, BOOL fRelease);
HRESULT WINAPI AEIDataObject_EnumFormatEtc(LPUNKNOWN lpTable, DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
HRESULT WINAPI AEIDataObject_DAdvise(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
HRESULT WINAPI AEIDataObject_DUnadvise(LPUNKNOWN lpTable, DWORD dwConnection);
HRESULT WINAPI AEIDataObject_EnumDAdvise(LPUNKNOWN lpTable, IEnumSTATDATA **ppEnumAdvise);
int AE_DataObjectLookupFormatEtc(AEIDataObject *pDataObj, FORMATETC *pFormatEtc);
UINT_PTR AE_DataObjectCopySelection(AKELEDIT *ae);
void AE_DataObjectFreeSelection(AKELEDIT *ae);
HRESULT WINAPI AEIEnumFORMATETC_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj);
ULONG WINAPI AEIEnumFORMATETC_AddRef(LPUNKNOWN lpTable);
ULONG WINAPI AEIEnumFORMATETC_Release(LPUNKNOWN lpTable);
HRESULT WINAPI AEIEnumFORMATETC_Next(LPUNKNOWN lpTable, ULONG celt, FORMATETC *rgelt, ULONG *pceltFetched);
HRESULT WINAPI AEIEnumFORMATETC_Skip(LPUNKNOWN lpTable, ULONG celt);
HRESULT WINAPI AEIEnumFORMATETC_Reset(LPUNKNOWN lpTable);
HRESULT WINAPI AEIEnumFORMATETC_Clone(LPUNKNOWN lpTable, IEnumFORMATETC **ppEnum);
BOOL AE_IsEqualIID(const GUID *rguid1, const GUID *rguid2);

#endif
