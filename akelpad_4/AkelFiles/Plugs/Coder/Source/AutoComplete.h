#ifndef _AUTOCOMPLETE_H_
#define _AUTOCOMPLETE_H_


//// Defines

#define STRID_HOTKEYS                    301
#define STRID_AUTOLIST                   302
#define STRID_AUTOLIST_PRE               303
#define STRID_AFTERCHAR_POST             304
#define STRID_TRANSPARENCY               305
#define STRID_ALPHA                      306
#define STRID_COMPLETEWITHLIST           307
#define STRID_COMPLETEWITHOUTLIST        308
#define STRID_COMPLETEWITHOUTLIST_PRE    309
#define STRID_COMPLETENEXT               310
#define STRID_COMPLETEPREV               311
#define STRID_DOCUMENT                   312
#define STRID_ADDDOCUMENTWORDS           313
#define STRID_COMPLETENONSYNTAXDOCUMENT  314
#define STRID_SAVETYPEDCASE              315
#define STRID_MAXDOCUMENT                316
#define STRID_CHARS                      317
#define STRID_ADDHIGHLIGHTWORDS          318
#define STRID_HLBASECOLORS               319
#define STRID_NOSYMBOLMARK               320
#define STRID_RIGHTDELIMITERS            321
#define STRID_SYNTAXDELIMITERS           322

#define DLLA_AUTOCOMPLETE_ADDWINDOW 50
#define DLLA_AUTOCOMPLETE_DELWINDOW 51

#define SIZE_MINX      30
#define SIZE_MINY      30

#define SIZE_ICON      16

//CreateAutoCompleteWindow flags
#define CAW_COMPLETEONE    0x1
#define CAW_COMPLETEEXACT  0x2
#define CAW_COMPLETEWINDOW 0x4
#define CAW_AUTOLIST       0x8

//CreateAutoCompleteWindow return value
#define CAWE_SUCCESS        0
#define CAWE_NOEDITWINDOW   1
#define CAWE_DOCUMENTLIMIT  2
#define CAWE_GETTITLEPART   3
#define CAWE_AUTOLIST       4
#define CAWE_COMPLETEEXACT  5
#define CAWE_GETBLOCK       6
#define CAWE_OTHER          7

//BLOCKINFO structure types
#define BIT_BLOCK          0x1
#define BIT_HLBASE         0x2
#define BIT_DOCWORD        0x4
#define BIT_NOSYNTAXFILE   0x8

//dwCompleteListSymbolMark
#define CLSM_NOMARK        0x0
#define CLSM_NOMARKIFICON  0x1
#define CLSM_MARK          0x2

#define AUTOCOMPLETEA   "AutoComplete"
#define AUTOCOMPLETEW  L"AutoComplete"
#define IDC_LIST        1001

#ifndef WM_MOUSEWHEEL
  #define WM_MOUSEWHEEL 0x020A
#endif
#ifndef LWA_ALPHA
  #define LWA_ALPHA 0x00000002
#endif
#ifndef WS_EX_LAYERED
  #define WS_EX_LAYERED 0x00080000
#endif


//// Structures

typedef struct _BLOCKINFO {
  struct _BLOCKINFO *next;
  struct _BLOCKINFO *prev;
  DWORD dwStructType;
  wchar_t wchFirstLowerChar;
  wchar_t *wpTitle;
  int nTitleLen;
  BOOL bExactTitle;

  struct _BLOCKINFO *master;
  INT_PTR *firstHandle;
  INT_PTR *lastHandle;
  wchar_t *wpBlock;
  int nBlockLen;
  int nLinesInBlock;
  HSTACK hHotSpotStack;
  INT_PTR nHotSpotBlockBegin;
  void *lpRef;
} BLOCKINFO;

typedef struct _BLOCKINFOHANDLE {
  struct _BLOCKINFOHANDLE *next;
  struct _BLOCKINFOHANDLE *prev;
  BLOCKINFO *lpBlockInfo;
} BLOCKINFOHANDLE;

typedef struct _HOTSPOT {
  struct _HOTSPOT *next;
  struct _HOTSPOT *prev;
  int nHotSpotInitPos;
  int nHotSpotInitLen;
  int nHotSpotPos;
  int nHotSpotLen;
} HOTSPOT;

typedef struct _DOCWORDINFO {
  struct _DOCWORDINFO *next;
  struct _DOCWORDINFO *prev;
  DWORD dwStructType;
  wchar_t wchFirstLowerChar;
  wchar_t *wpDocWord;
  int nDocWordLen;
} DOCWORDINFO;

typedef struct {
  DOCWORDINFO *first;
  DOCWORDINFO *last;
  INT_PTR lpSorted[FIRST_NONLATIN + 1];
} HDOCWORDS;


//// Prototypes

BOOL CALLBACK AutoCompleteSetup1DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AutoCompleteSetup2DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AutoCompleteParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
BOOL CALLBACK AutoCompleteEditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CompleteWithListProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK CompleteWithoutListProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK CompleteNextProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK CompletePrevProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
LRESULT CALLBACK AutoCompleteWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewListboxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//AutoComplete window
DWORD CreateAutoCompleteWindow(SYNTAXFILE *lpTheme, DWORD dwFlags);
BOOL MoveAutoCompleteWindow();
void CloseAutoCompleteWindow();

//AutoComplete window listbox
void FillListbox(SYNTAXFILE *lpSyntaxFile, HDOCWORDS *hDocWordsStack, const wchar_t *wpTitlePart);
void SetSelListbox(int nIndex);
BLOCKINFO* GetDataListbox(int nItem);

//Scheme
int ParseBlock(SYNTAXFILE *lpScheme, HSTACK *hHotSpotStack, const wchar_t *wpInput, int nInputLen, wchar_t *wszOutput, int *nOutputLines);

//Title
TITLEINFO* StackInsertTitle(STACKTITLE *hStack);
void StackFreeTitle(STACKTITLE *hStack);

//Title part
BOOL GetEditTitlePart(STACKDELIM *hDelimiterStack, wchar_t *wszTitle, int nTitleMax, INT_PTR *nMin, INT_PTR *nMax);
void CompleteTitlePart(BLOCKINFO *lpBlockInfo, INT_PTR nMin, INT_PTR nMax);

//Block
BLOCKINFO* StackInsertBlock(STACKBLOCK *hBlockStack);
BLOCKINFO* StackInsertAndSortBlock(STACKBLOCK *hBlockStack, wchar_t *wpTitle, int nTitleLen);
BLOCKINFO* StackGetExactBlock(SYNTAXFILE *lpSyntaxFile, AECHARINDEX *ciCaret, INT_PTR nCaretOffset, INT_PTR *nMin, INT_PTR *nMax);
BLOCKINFO* StackGetBlock(SYNTAXFILE *lpSyntaxFile, HDOCWORDS *hDocWordsStack, const wchar_t *wpTitlePart, int nTitlePartLen, BOOL *bOnlyOne);
void StackFreeBlock(STACKBLOCK *hBlockStack);

//Hot spot
HOTSPOT* StackInsertHotSpot(HSTACK *hStack, int nHotSpotPos);
HOTSPOT* GetHotSpot(BLOCKINFO *lpBlockInfo, HWND hWnd, CHARRANGE64 *cr);
BOOL NextHotSpot(BLOCKINFO *lpBlockInfo, HWND hWnd, BOOL bPrev);
void StackResetHotSpot(BLOCKINFO *lpBlockInfo);
void StackFreeHotSpot(HSTACK *hStack);

//Document words
void StackFillDocWord(SYNTAXFILE *lpSyntaxFile, HDOCWORDS *hDocWordsStack, const wchar_t *wpTitlePart, int nTitlePartLen);
DOCWORDINFO* StackInsertDocWord(HDOCWORDS *hStack, wchar_t *wpWordDoc, int nWordDocLen);
DOCWORDINFO* StackGetDocWord(HDOCWORDS *hStack, const wchar_t *wpDocWord, int nDocWordLen);
void StackFreeDocWord(HDOCWORDS *hStack);

//Options
void ReadAutoCompleteOptions(HANDLE hOptions);
void SaveAutoCompleteOptions(HANDLE hOptions, DWORD dwFlags);
void InitAutoComplete();
void UninitAutoComplete();


//// Global variables

extern SYNTAXFILE *lpSyntaxFileAutoComplete;

#endif
