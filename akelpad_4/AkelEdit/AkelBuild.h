#ifndef __AKELBUILD_H__
#define __AKELBUILD_H__


//// Includes

#include <shlobj.h>
#include "AkelEdit.h"


//// Defines

#define AETIMERID_MOUSEMOVE    1
#define AETIMERID_MOUSESCROLL  2

#define AETIME_BEFOREPROGRESS  0
#define AETIME_BEFOREUPDATE    100

#define AEURL_MAX_LENGTH       512

#ifndef NOTSRCINVERT
  #define NOTSRCINVERT         (DWORD)0x00990066 /* dest = (NOT source) XOR dest */
#endif

//WM_SETCURSOR
#define AECC_IBEAM          0
#define AECC_MARGIN         1
#define AECC_SELECTION      2
#define AECC_URL            3
#define AECC_MCENTER        4
#define AECC_MLEFT          5
#define AECC_MLEFTTOP       6
#define AECC_MTOP           7
#define AECC_MRIGHTTOP      8
#define AECC_MRIGHT         9
#define AECC_MRIGHTBOTTOM   10
#define AECC_MBOTTOM        11
#define AECC_MLEFTBOTTOM    12

//Print
#define AEPRNL_PRINTLINESIZE 32768

//Highlight search types
#define AEHF_ISFIRSTCHAR    0x00000001
#define AEHF_FINDFIRSTCHAR  0x00000002

//Highlight paint type
#define AEPT_SELECTION      0x00000001
#define AEPT_DELIM1         0x00000002
#define AEPT_WORD           0x00000004
#define AEPT_DELIM2         0x00000008
#define AEPT_QUOTE          0x00000010
#define AEPT_LINK           0x00000020

//Line selection
#define AELS_EMPTY    1
#define AELS_FULL     2
#define AELS_PARTLY   3

//AE_CalcLinesWidth flags
#define AECLW_FRESH            0x00000001
#define AECLW_LOCKUPDATE       0x00000002

//AE_GetCharInLine flags
#define AECIL_HALFFIT          0x00000001
#define AECIL_USELINEBEGINPOS  0x00000002
#define AECIL_USELINEENDPOS    0x00000004
#define AECIL_USECARETPOS      0x00000008

#define AECLR_ALLPOS          (AECIL_USELINEBEGINPOS |\
                               AECIL_USELINEENDPOS   |\
                               AECIL_USECARETPOS)

//StreamIn HIBYTE new line
#define AELB_SPLIT    1

//AE_InsertText flags
#define AEINST_LOCKUNDO        0x00000001
#define AEINST_LOCKSCROLL      0x00000002
#define AEINST_LOCKUPDATE      0x00000004

//AE_DeleteText flags
#define AEDELT_LOCKUNDO        0x00000001
#define AEDELT_LOCKSCROLL      0x00000002
#define AEDELT_LOCKUPDATE      0x00000004

//Undo/Redo flags
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

//Undo element
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


//// Hightlight

typedef struct _AEHDELIMSTACK {
  int first;
  int last;
} AEHDELIMSTACK;

typedef struct _AEHWORDSTACK {
  int first;
  int last;
  int lpWordLens[MAX_PATH];
} AEHWORDSTACK;

typedef struct _AEHQUOTESTACK {
  int first;
  int last;
} AEHQUOTESTACK;

typedef struct _AETHEMEITEMW {
  struct _AETHEMEITEMW *next;
  struct _AETHEMEITEMW *prev;
  wchar_t wszThemeName[MAX_PATH];
  AEHDELIMSTACK hDelimiterStack;
  AEHWORDSTACK hWordStack;
  AEHQUOTESTACK hQuoteStack;
} AETHEMEITEMW;

typedef struct _AEQUOTEMATCH {
  AEQUOTEITEMW *lpQuote;
  AECHARRANGE crQuoteStart;
  AECHARRANGE crQuoteEnd;
} AEQUOTEMATCH;

typedef struct _AEWORDMATCH {
  AEDELIMITEMW *lpDelim1;
  AECHARRANGE crDelim1;
  AEWORDITEMW *lpWord;
  AECHARRANGE crWord;
  AEDELIMITEMW *lpDelim2;
  AECHARRANGE crDelim2;
} AEWORDMATCH;

typedef struct _AEHLPAINT {
  DWORD dwActiveBG;
  DWORD dwActiveText;
  DWORD dwDefaultText;
  DWORD dwDefaultBG;
  HBRUSH hbrDefaultBG;
  DWORD dwPaintType;
  DWORD dwFontStyle;
  HBRUSH hbrActiveBG;
  AEWORDMATCH wm;
  AEQUOTEMATCH qm;
  AECHARRANGE crLink;
} AEHLPAINT;


//// Font widths

typedef struct _AEFONTCHARSA {
  struct _AEFONTCHARSA *next;
  struct _AEFONTCHARSA *prev;
  LOGFONTA lfFont;
  WORD lpCharWidths[65536];
} AEFONTCHARSA;

typedef struct _AEFONTCHARSW {
  struct _AEFONTCHARSW *next;
  struct _AEFONTCHARSW *prev;
  LOGFONTW lfFont;
  WORD lpCharWidths[65536];
} AEFONTCHARSW;


//// AKELEDIT

typedef struct {
  HANDLE hHeap;
  HSTACK hLinesStack;
  HSTACK hPointsStack;
  HFONT hFont;
  HFONT hFontNormal;
  HFONT hFontBold;
  HFONT hFontItalic;
  HFONT hFontBoldItalic;
  HFONT hFontUrl;
  LOGFONTA lfFontA;
  LOGFONTW lfFontW;
  int nCharHeight;
  int nLineGap;
  int nAveCharWidth;
  int nSpaceCharWidth;
  int nTabWidth;
  int nTabStop;
  WORD *lpCharWidths;
  HSTACK hUndoStack;
  AEUNDOITEM *lpCurrentUndo;
  AEUNDOITEM *lpSavePoint;
  BOOL bSavePointExist;
  BOOL bModified;
  BOOL bLockGroupStopInt;
  BOOL bLockGroupStopExt;
  BOOL bLockCollectUndo;
  DWORD dwUndoLimit;
  DWORD dwUndoCount;
  AELINEINDEX liMaxWidthLine;
  AELINEINDEX liLineUnwrapLastCall;
  int nLastCharOffset;
  int nLineCount;
  int nLineUnwrapCount;
  int nLineUnwrapLastCall;
  int nHScrollMax;
  int nVScrollMax;
  int nWordWrap;
  DWORD dwWrapLimit;
  wchar_t wszWrapDelimiters[128];
} AKELTEXT;

typedef struct {
  DWORD dwEventMask;
  DWORD dwRichEventMask;
  DWORD dwOptions;
  int nInputNewLine;
  int nOutputNewLine;
  BOOL bVScrollShow;
  BOOL bHScrollShow;
  BOOL bVScrollLock;
  BOOL bHScrollLock;
  BOOL bHideSelection;
  COLORREF crCaret;
  COLORREF crBasicText;
  COLORREF crBasicBk;
  COLORREF crSelText;
  COLORREF crSelBk;
  COLORREF crActiveLineText;
  COLORREF crActiveLineBk;
  COLORREF crUrlText;
  COLORREF crActiveColumn;
  COLORREF crColumnMarker;
  COLORREF crEnableBasicBk;
  COLORREF crEnableSelBk;
  COLORREF crEnableActiveLineBk;
  BOOL bDefaultColors;
  HBRUSH hBasicBk;
  HBRUSH hSelBk;
  HBRUSH hActiveLineBk;
  HBITMAP hCaretInsert;
  HBITMAP hCaretOvertype;
  int nCaretInsertWidth;
  int nCaretOvertypeHeight;
  BOOL bOverType;
  HBRUSH hActiveColumn;
  BOOL bActiveColumnDraw;
  int nColumnMarker;
  wchar_t wszWordDelimiters[128];
  DWORD dwWordBreak;
  wchar_t wszUrlDelimiters[128];
  wchar_t wszUrlPrefixes[128];
  wchar_t *lpUrlPrefixes[32];
  DWORD dwUrlMaxLength;
  BOOL bDetectUrl;
  AETHEMEITEMW *lpActiveTheme;
} AKELOPTIONS;

typedef struct _AKELEDIT {
  struct _AKELEDIT *next;
  struct _AKELEDIT *prev;

  //Ident
  HWND hWndEdit;
  HWND hWndParent;
  int nEditCtrlID;
  BOOL bUnicodeWindow;
  BOOL bRichEditClass;

  //Text
  AKELTEXT txt;
  AKELTEXT *ptxt;

  //Options
  AKELOPTIONS opt;
  AKELOPTIONS *popt;

  //Current selection
  AELINEINDEX liFirstDrawLine;
  AECHARINDEX ciSelStartIndex;
  AECHARINDEX ciSelEndIndex;
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciLastCallIndex;
  int nFirstDrawLineOffset;
  int nSelStartCharOffset;
  int nSelEndCharOffset;
  int nLastCallOffset;
  int nHScrollPos;
  int nVScrollPos;
  int nLastHScrollPos;
  int nLastVScrollPos;
  POINT ptCaret;
  int nHorizCaretPos;
  BOOL bColumnSel;

  //Dinamic
  HDC hDC;
  RECT rcEdit;
  RECT rcDraw;
  RECT rcErase;
  POINT ptActiveColumnDraw;
  DWORD dwNotify;
  DWORD dwInputLocale;
  DWORD dwImeChar;
  BOOL bCaretVisible;
  BOOL bFocus;

  //Cursor
  AECHARRANGE crMouseOnLink;
  AECHARINDEX ciLButtonClick;
  AECHARINDEX ciLButtonStart;
  AECHARINDEX ciLButtonEnd;
  POINT ptLButtonDownPrevPos;
  int nLButtonDownPrevTime;
  int nLButtonDownCount;
  DWORD dwMouseMoveTimer;
  DWORD dwMouseScrollTimer;
  int nCurrentCursor;
  POINT ptMButtonDown;
  POINT ptMButtonScroll;
  HBITMAP hMButtonBitmap;
  BOOL bMButtonBitmapDraw;
  BOOL bMButtonDown;
  BOOL bMButtonUp;
  int nMButtonMove;

  //OLE Drag'n'Drop
  AEIDropTargetCallbackVtbl idtVtbl;
  AEIDropSourceCallbackVtbl idsVtbl;
  AEIDataObjectCallbackVtbl idoVtbl;
  AEIDropTarget idt;
  AEIDropSource ids;
  AEIDataObject ido;
  BOOL bDropping;
  BOOL bDragging;
  BOOL bDragSelectionDelete;
  DWORD dwDragSelectionLength;
  int nMoveBeforeDragging;

  //Clone
  HSTACK hClonesStack;
  int nCloneCount;
  struct _AKELEDIT *lpMaster;
  AEPOINT *lpSelStartPoint;
  AEPOINT *lpSelEndPoint;
  AEPOINT *lpCaretPoint;
} AKELEDIT;


//// Clone

typedef struct _AECLONE {
  struct _AECLONE *next;
  struct _AECLONE *prev;
  AKELEDIT *aeClone;
} AECLONE;


//// Print

typedef struct {
  AKELEDIT aePrint;
  char szPrintLine[AEPRNL_PRINTLINESIZE];
  wchar_t wszPrintLine[AEPRNL_PRINTLINESIZE];
} AEPRINTHANDLE;


//// Functions prototypes

BOOL AE_RegisterClassA(HINSTANCE hInstance);
BOOL AE_RegisterClassW(HINSTANCE hInstance);
BOOL AE_UnregisterClassA(HINSTANCE hInstance);
BOOL AE_UnregisterClassW(HINSTANCE hInstance);
LRESULT CALLBACK AE_EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HANDLE AE_HeapCreate(AKELEDIT *ae);
LPVOID AE_HeapAlloc(AKELEDIT *ae, DWORD dwFlags, SIZE_T dwBytes);
BOOL AE_HeapFree(AKELEDIT *ae, DWORD dwFlags, LPVOID lpMem);
int AE_HeapStackInsertIndex(AKELEDIT *ae, stack **first, stack **last, stack **element, int nIndex, int nBytes);
int AE_HeapStackInsertBefore(AKELEDIT *ae, stack **first, stack **last, stack *index, stack **element, int nBytes);
int AE_HeapStackInsertAfter(AKELEDIT *ae, stack **first, stack **last, stack *index, stack **element, int nBytes);
int AE_HeapStackDelete(AKELEDIT *ae, stack **first, stack **last, stack *element);
void AE_HeapStackClear(AKELEDIT *ae, stack **first, stack **last);
AKELEDIT* AE_StackWindowInsert(HSTACK *hStack);
AKELEDIT* AE_StackWindowGet(HSTACK *hStack, HWND hWndEdit);
void AE_StackWindowFree(HSTACK *hStack);
AECLONE* AE_StackCloneIndex(AKELEDIT *ae, DWORD dwIndex);
AECLONE* AE_StackCloneGet(AKELEDIT *ae, HWND hWnd);
AECLONE* AE_StackCloneAdd(AKELEDIT *aeMaster, AKELEDIT *aeClone);
void AE_ActivateClone(AKELEDIT *lpAkelEditPrev, AKELEDIT *ae);
void AE_StackCloneDelete(AECLONE *aec);
void AE_StackCloneDeleteAll(AKELEDIT *ae);
void AE_StackUpdateClones(AKELEDIT *ae);
AKELEDIT* AE_StackDraggingGet(AKELEDIT *ae);
WORD* AE_StackFontCharsInsertA(HSTACK *hStack, LOGFONTA *lfFont);
WORD* AE_StackFontCharsInsertW(HSTACK *hStack, LOGFONTW *lfFont);
WORD* AE_StackFontCharsGetA(HSTACK *hStack, LOGFONTA *lfFont);
WORD* AE_StackFontCharsGetW(HSTACK *hStack, LOGFONTW *lfFont);
void AE_StackFontCharsFree(HSTACK *hStack);
AEPOINT* AE_StackPointInsert(AKELEDIT *ae, AECHARINDEX *ciPoint);
void AE_StackPointSetModify(AKELEDIT *ae, BOOL bModify);
void AE_StackPointReset(AKELEDIT *ae);
void AE_StackPointDelete(AKELEDIT *ae, AEPOINT *lpElement);
void AE_StackPointFree(AKELEDIT *ae);
AEUNDOITEM* AE_StackUndoItemInsert(AKELEDIT *ae);
void AE_StackUndoItemDelete(AKELEDIT *ae, AEUNDOITEM *lpItem);
void AE_StackRedoDeleteAll(AKELEDIT *ae, AEUNDOITEM *lpItem);
DWORD AE_StackUndoSize(AKELEDIT *ae);
void AE_StackUndoGroupStop(AKELEDIT *ae);
AELINEDATA* AE_StackLineAdd(AKELEDIT *ae);
AELINEDATA* AE_StackLineInsertBefore(AKELEDIT *ae, AELINEDATA *lpLine);
AELINEDATA* AE_StackLineInsertAfter(AKELEDIT *ae, AELINEDATA *lpLine);
void AE_StackLineDelete(AKELEDIT *ae, AELINEDATA *lpElement);
void AE_StackLineFree(AKELEDIT *ae);
AELINEDATA* AE_GetLineData(AKELEDIT *ae, int nLine);
int AE_GetWrapLine(AKELEDIT *ae, int nLine, AECHARINDEX *ciChar);
int AE_GetUnwrapLine(AKELEDIT *ae, int nLine);
void AE_RichOffsetToAkelIndex(AKELEDIT *ae, DWORD dwOffset, AECHARINDEX *ciCharIndex);
int AE_AkelIndexToRichOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIndex);
int AE_GetIndex(AKELEDIT *ae, int nType, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, BOOL bColumnSel);
int AE_IndexCompare(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2);
int AE_IndexInc(AECHARINDEX *ciChar);
int AE_IndexDec(AECHARINDEX *ciChar);
DWORD AE_IndexSubtract(AKELEDIT *ae, const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
DWORD AE_IndexOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, int nOffset, int nNewLine);
BOOL AE_IndexNormalize(AECHARINDEX *ciChar);
BOOL AE_IndexUpdate(AKELEDIT *ae, AECHARINDEX *ciChar);
int AE_UpdateWrap(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, int nWrap);
int AE_WrapLines(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, int nWrap);
int AE_LineWrap(AKELEDIT *ae, const AELINEINDEX *liLine, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwMaxWidth, int nWrap);
int AE_LineUnwrap(AKELEDIT *ae, AELINEINDEX *liLine, DWORD dwMaxWidth);
void AE_CalcLinesWidth(AKELEDIT *ae, const AELINEINDEX *liStartLine, const AELINEINDEX *liEndLine, DWORD dwFlags);
int AE_CheckCodepage(AKELEDIT *ae, int nCodePage);
void AE_SetDrawRect(AKELEDIT *ae, const RECT *lprcDraw, BOOL bRedraw);
void AE_SetEditFontA(AKELEDIT *ae, HFONT hFont, BOOL bRedraw);
void AE_SetEditFontW(AKELEDIT *ae, HFONT hFont, BOOL bRedraw);
void AE_SetSelectionPos(AKELEDIT *ae, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, BOOL bColumnSel, DWORD dwSelFlags);
void AE_UpdateSelection(AKELEDIT *ae, DWORD dwSelFlags);
void AE_SetMouseSelection(AKELEDIT *ae, const POINT *ptPos, BOOL bColumnSel, BOOL bShift);
void AE_GlobalToClient(AKELEDIT *ae, const POINT *ptGlobal, POINT *ptClient);
void AE_ClientToGlobal(AKELEDIT *ae, const POINT *ptClient, POINT *ptGlobal);
void AE_UpdateCursor();
int AE_SetCursor(AKELEDIT *ae);
BOOL AE_IsCursorOnLeftMargin(AKELEDIT *ae, const POINT *ptPos);
BOOL AE_IsCursorOnSelection(AKELEDIT *ae, const POINT *ptPos);
DWORD AE_IsCursorOnUrl(AKELEDIT *ae, const POINT *ptPos, AECHARRANGE *crLink);
DWORD AE_HighlightFindUrl(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, int nLastLine, AECHARRANGE *crLink);
int AE_HighlightFindQuote(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEQUOTEMATCH *wm);
int AE_HighlightFindWord(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEWORDMATCH *wm);
AEDELIMITEMW* AE_HighlightIsDelimiter(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar, BOOL bBack);
AEWORDITEMW* AE_HighlightIsWord(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar, int nWordLen);
AETHEMEITEMW* AE_HighlightCreateTheme(AKELEDIT *ae, wchar_t *wpThemeName);
AETHEMEITEMW* AE_HighlightGetTheme(AKELEDIT *ae, wchar_t *wpThemeName);
BOOL AE_HighlightIsThemeExists(AKELEDIT *ae, AETHEMEITEMW *aeti);
void AE_HighlightDeleteTheme(AKELEDIT *ae, AETHEMEITEMW *aeti);
void AE_HighlightDeleteThemeAll(AKELEDIT *ae);
AEDELIMITEMW* AE_HighlightInsertDelimiter(AKELEDIT *ae, AETHEMEITEMW *aeti, int nDelimiterLen);
AEDELIMITEMW* AE_HighlightGetDelimiter(AKELEDIT *ae, AETHEMEITEMW *aeti, const wchar_t *wpDelimiter, int nDelimiterLen, DWORD dwFlags);
void AE_HighlightDeleteDelimiter(AKELEDIT *ae, AETHEMEITEMW *aeti, AEDELIMITEMW *aedi);
void AE_HighlightDeleteDelimiterAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
AEWORDITEMW* AE_HighlightInsertWord(AKELEDIT *ae, AETHEMEITEMW *aeti, int nWordLen);
AEWORDITEMW* AE_HighlightGetWord(AKELEDIT *ae, AETHEMEITEMW *aeti, const wchar_t *wpWord, int nWordLen, DWORD dwFlags);
void AE_HighlightDeleteWord(AKELEDIT *ae, AETHEMEITEMW *aeti, AEWORDITEMW *aewi);
void AE_HighlightDeleteWordAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
AEQUOTEITEMW* AE_HighlightInsertQuote(AKELEDIT *ae, AETHEMEITEMW *aeti, int nQuoteStartLen);
AEQUOTEITEMW* AE_HighlightGetQuote(AKELEDIT *ae, AETHEMEITEMW *aeti, const wchar_t *wpQuoteStart, int nQuoteStartLen, DWORD dwFlags);
void AE_HighlightDeleteQuote(AKELEDIT *ae, AETHEMEITEMW *aeti, AEQUOTEITEMW *aeqi);
void AE_HighlightDeleteQuoteAll(AKELEDIT *ae, AETHEMEITEMW *aeti);
void AE_MouseMove(AKELEDIT *ae);
HBITMAP AE_CreateBitmap(AKELEDIT *ae, int nWidth, int nHeight, COLORREF crBasic, COLORREF crInvert, BOOL bZebra);
HBITMAP AE_LoadBitmapFromMemory(AKELEDIT *ae, const BYTE *lpBmpFileData);
BOOL AE_UpdateCaret(AKELEDIT *ae, BOOL bFocus, BOOL bFresh);
BOOL AE_SetCaretPos(AKELEDIT *ae, const POINT *ptCaret);
void SetCaretVis(AKELEDIT *ae, const POINT *ptCaret);
void AE_ScrollToCaret(AKELEDIT *ae, const POINT *ptCaret, BOOL bVertCorrect);
DWORD AE_ScrollToCaretEx(AKELEDIT *ae, const POINT *ptCaret, DWORD dwFlags, WORD wUnitX, WORD wUnitY, BOOL bTest);
void AE_UpdateScrollBars(AKELEDIT *ae, int nBar);
int AE_ScrollEditWindow(AKELEDIT *ae, int nBar, int nPos);
int AE_HScroll(AKELEDIT *ae, int nAction);
int AE_VScroll(AKELEDIT *ae, int nAction);
int AE_HScrollLine(AKELEDIT *ae, int nChar);
int AE_VScrollLine(AKELEDIT *ae, int nLine);
AEPRINTHANDLE* AE_StartPrintDocA(AKELEDIT *ae, AEPRINT *prn);
AEPRINTHANDLE* AE_StartPrintDocW(AKELEDIT *ae, AEPRINT *prn);
void AE_GetPrintPage(AEPRINT *prn, const RECT *rcMargins, RECT *rcPage);
BOOL AE_PrintPage(AKELEDIT *ae, AEPRINTHANDLE *ph, AEPRINT *prn);
void AE_EndPrintDoc(AKELEDIT *ae, AEPRINTHANDLE *ph, AEPRINT *prn);
void AE_Paint(AKELEDIT *ae);
void AE_PaintTextOut(AKELEDIT *ae, HDC hDC, AEHLPAINT *hlp, const POINT *ptDraw, const wchar_t *wpLine, int nLineLen, int nLineWidth, wchar_t **wpTextInLine, int *nTextInLineWidth);
void AE_MButtonDraw(AKELEDIT *ae);
void AE_MButtonErase(AKELEDIT *ae);
BOOL AE_ActiveColumnCreate(AKELEDIT *ae);
void AE_ActiveColumnDraw(AKELEDIT *ae);
void AE_ActiveColumnErase(AKELEDIT *ae);
void AE_UpdateEditWindow(HWND hWndEdit, BOOL bErase);
void AE_ColumnMarkerDraw(AKELEDIT *ae);
void AE_ColumnMarkerErase(AKELEDIT *ae);
void AE_RedrawLineRange(AKELEDIT *ae, int nFirstLine, int nLastLine, BOOL bErase);
void AE_HideSelection(AKELEDIT *ae, BOOL bHide);
int AE_GetFirstVisibleLine(AKELEDIT *ae);
int AE_GetLastVisibleLine(AKELEDIT *ae);
int IsSurrogate(wchar_t wchChar);
int IsHighSurrogate(wchar_t wchChar);
int IsLowSurrogate(wchar_t wchChar);
int AE_GetTextExtentPoint32(AKELEDIT *ae, const wchar_t *wpString, int nStringLen, SIZE *lpSize);
int AE_GetCharWidth(AKELEDIT *ae, wchar_t *wpChar, int *nDone);
int AE_GetStringWidth(AKELEDIT *ae, wchar_t *wpString, int nStringLen, int nFirstCharExtent);
void AE_GetLineWidth(AKELEDIT *ae, AELINEDATA *lpLine);
BOOL AE_GetPosFromChar(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, POINT *ptClientPos);
int AE_GetCharInLine(AKELEDIT *ae, const AELINEDATA *lpLine, int nMaxExtent, DWORD dwFlags, int *nCharIndex, int *nCharPos, BOOL bColumnSel);
BOOL AE_GetCharRangeInLine(AKELEDIT *ae, AELINEDATA *lpLine, int nMinExtent, int nMaxExtent, int *nMinCharIndex, int *nMinCharPos, int *nMaxCharIndex, int *nMaxCharPos, BOOL bColumnSel);
int AE_GetCharFromPos(AKELEDIT *ae, const POINT *ptClientPos, AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, BOOL bColumnSel);
BOOL AE_GetNextBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciNextBreak, BOOL bColumnSel, DWORD dwFlags);
BOOL AE_GetPrevBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciPrevBreak, BOOL bColumnSel, DWORD dwFlags);
BOOL AE_GetNextWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel, DWORD dwFlags, BOOL bSearch);
BOOL AE_GetPrevWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel, DWORD dwFlags, BOOL bSearch);
BOOL AE_IsFirstCharInLine(const AECHARINDEX *ciChar);
BOOL AE_IsLastCharInLine(const AECHARINDEX *ciChar);
BOOL AE_IsCharInSelection(const AECHARINDEX *ciChar);
BOOL AE_IsEscaped(const AECHARINDEX *ciChar, wchar_t wchEscape);
BOOL AE_IsInDelimiterList(const wchar_t *wpList, wchar_t c);
BOOL AE_IsSpace(wchar_t c);
int AE_GetUrlPrefixes(AKELEDIT *ae);
int AE_GetLineSelection(AKELEDIT *ae, const AELINEINDEX *liLine, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, POINT *ptSelStart, POINT *ptSelEnd, int *nSelStartIndexInLine, int *nSelEndIndexInLine, BOOL bColumnSel);
DWORD AE_GetTextRangeAnsi(AKELEDIT *ae, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefaultChar, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, char *szBuffer, DWORD dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
DWORD AE_GetTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, wchar_t *wszBuffer, DWORD dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces);
DWORD AE_SetTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, DWORD dwTextLen, int nNewLine);
DWORD AE_SetText(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int nNewLine);
DWORD AE_StreamIn(AKELEDIT *ae, DWORD dwFlags, AESTREAMIN *aesi);
int AE_JoinNewLines(AKELEDIT *ae);
DWORD AE_StreamOut(AKELEDIT *ae, DWORD dwFlags, AESTREAMOUT *aeso);
BOOL AE_StreamOutHelper(AESTREAMOUT *aeso, const AECHARINDEX *ciCount, const AECHARINDEX *ciEnd, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufCount, DWORD *dwResult);
void AE_AppendTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, DWORD dwTextLen, BOOL bColumnSel);
void AE_AppendText(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, BOOL bColumnSel);
void AE_ReplaceSelAnsi(AKELEDIT *ae, int nCodePage, const char *pText, DWORD dwTextLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
void AE_ReplaceSel(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
DWORD AE_DeleteTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, BOOL bColumnSel, DWORD dwDeleteFlags);
DWORD AE_InsertText(AKELEDIT *ae, const AECHARINDEX *ciInsertPos, const wchar_t *wpText, DWORD dwTextLen, int nNewLine, BOOL bColumnSel, DWORD dwInsertFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
wchar_t* AE_GetNextLine(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int *nLineLen, int *nLineBreak);
int AE_GetNewLineString(AKELEDIT *ae, int nNewLine, wchar_t **wpNewLine);
BOOL AE_FindTextAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA);
BOOL AE_FindText(AKELEDIT *ae, AEFINDTEXTW *ft);
DWORD AE_IsMatchAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA, const AECHARINDEX *ciChar);
DWORD AE_IsMatch(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar);
void AE_UpdateCandidatePos(AKELEDIT *ae);
BOOL AE_GetModify(AKELEDIT *ae);
void AE_SetModify(AKELEDIT *ae, BOOL bState, BOOL bMessage);
void AE_EmptyUndoBuffer(AKELEDIT *ae);
BOOL AE_IsReadOnly(AKELEDIT *ae);
BOOL AE_KeyDown(AKELEDIT *ae, int nVk, BOOL bAlt, BOOL bShift, BOOL bControl);
BOOL AE_EditCanPaste(AKELEDIT *ae);
BOOL AE_EditCanRedo(AKELEDIT *ae);
BOOL AE_EditCanUndo(AKELEDIT *ae);
void AE_EditUndo(AKELEDIT *ae);
void AE_EditRedo(AKELEDIT *ae);
void AE_EditCut(AKELEDIT *ae);
void AE_EditCopyToClipboard(AKELEDIT *ae);
BOOL AE_EditPasteFromClipboard(AKELEDIT *ae, BOOL bAnsi);
void AE_EditChar(AKELEDIT *ae, WPARAM wParam);
void AE_EditKeyReturn(AKELEDIT *ae);
void AE_EditKeyBackspace(AKELEDIT *ae, BOOL bControl);
void AE_EditKeyDelete(AKELEDIT *ae, BOOL bControl);
void AE_EditSelectAll(AKELEDIT *ae);
BOOL AE_AkelEditGetSel(AKELEDIT *ae, AESELECTION *aes, AECHARINDEX *lpciCaret);
void AE_AkelEditSetSel(AKELEDIT *ae, const AESELECTION *aes, const AECHARINDEX *lpciCaret);
BOOL AE_RichEditGetSel(AKELEDIT *ae, LONG *nMin, LONG *nMax);
void AE_RichEditSetSel(AKELEDIT *ae, LONG nMin, LONG nMax, BOOL bColumnSel);
void AE_GetColors(AKELEDIT *ae, AECOLORS *aec);
void AE_SetColors(AKELEDIT *ae, const AECOLORS *aec);
void AE_NotifyErrSpace(AKELEDIT *ae, DWORD dwBytes);
void AE_NotifySetFocus(AKELEDIT *ae);
void AE_NotifyKillFocus(AKELEDIT *ae);
void AE_NotifySelChanging(AKELEDIT *ae);
void AE_NotifySelChanged(AKELEDIT *ae);
void AE_NotifyTextChanging(AKELEDIT *ae, DWORD dwType);
void AE_NotifyTextChanged(AKELEDIT *ae, DWORD dwType);
void AE_NotifyChanging(AKELEDIT *ae, DWORD dwType);
void AE_NotifyChanged(AKELEDIT *ae, DWORD dwType);
void AE_NotifyHScroll(AKELEDIT *ae);
void AE_NotifyVScroll(AKELEDIT *ae);
BOOL AE_NotifyMsgFilter(AKELEDIT *ae, UINT uMsg, WPARAM *wParam, LPARAM *lParam);
BOOL AE_NotifyDropFiles(AKELEDIT *ae, HDROP hDrop);
void AE_NotifyDragDropDone(AKELEDIT *ae);
BOOL AE_NotifyDropSource(AKELEDIT *ae, int nAction, DWORD *lpdwEffect, DWORD dwDropResult);
BOOL AE_NotifyDropTarget(AKELEDIT *ae, int nAction, POINT *pt, DWORD *lpdwEffect);
BOOL AE_NotifyLink(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam, const AECHARRANGE *crLink);
BOOL AE_NotifyProgress(AKELEDIT *ae, DWORD dwType, DWORD dwTimeElapsed, int nCurrent, int nMaximum);
wchar_t AE_WideCharUpper(wchar_t c);
int AE_WideStrCmp(const wchar_t *wpString, const wchar_t *wpString2);
int AE_WideStrCmpI(const wchar_t *wpString, const wchar_t *wpString2);
int AE_WideStrCmpLen(const wchar_t *wpString, const wchar_t *wpString2, DWORD dwMaxLength);
int AE_WideStrCmpLenI(const wchar_t *wpString, const wchar_t *wpString2, DWORD dwMaxLength);
void AE_ChangeByteOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
int AE_strncpy(char *dest, const char *src, unsigned int count);
int AE_wcsncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
wchar_t* AE_wcschr(const wchar_t *s, wchar_t c);
void* AE_memcpy(void *dest, const void *src, unsigned int count);
int AE_memcmp(const void *buf1, const void *buf2, unsigned int count);
void* AE_memset(void *dest, int c, unsigned int count);

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
HRESULT WINAPI AEIDataObject_GetCanonicalFormatEtc(LPUNKNOWN lpTable, FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut);
HRESULT WINAPI AEIDataObject_SetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium, BOOL fRelease);
HRESULT WINAPI AEIDataObject_EnumFormatEtc(LPUNKNOWN lpTable, DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc);
HRESULT WINAPI AEIDataObject_DAdvise(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
HRESULT WINAPI AEIDataObject_DUnadvise(LPUNKNOWN lpTable, DWORD dwConnection);
HRESULT WINAPI AEIDataObject_EnumDAdvise(LPUNKNOWN lpTable, IEnumSTATDATA **ppEnumAdvise);
int AE_DataObjectLookupFormatEtc(AEIDataObject *pDataObj, FORMATETC *pFormatEtc);
DWORD AE_DataObjectCopySelection(AKELEDIT *ae);
void AE_DataObjectFreeSelection(AKELEDIT *ae);

#endif
