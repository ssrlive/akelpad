#ifndef _CODER_H_
#define _CODER_H_


//// Common defines

#define STRID_PLUGIN                1
#define STRID_OK                    2
#define STRID_CANCEL                3
#define STRID_CLOSE                 4
#define STRID_ENABLE                5
#define STRID_GENERAL               11
#define STRID_CODER_NOTRUNNING      12
#define STRID_LOADFIRST             13
#define STRID_CACHE_NONE            51
#define STRID_CACHE_FAST            52
#define STRID_CACHE_SMART           53
#define STRID_DEFAULTALIAS_ENABLE   54
#define STRID_VARTHEME_NAME         55
#define STRID_VARTHEME_SAVE         56
#define STRID_VARTHEME_DELETE       57
#define STRID_VARTHEME_RETURN       58
#define STRID_VARTHEME_GLOBAL       59
#define STRID_VARTHEME_LINK         60
#define STRID_MENU_ADDITEM          61
#define STRID_MENU_MODITEM          62
#define STRID_MENU_MOVEUPITEM       63
#define STRID_MENU_MOVEDOWNITEM     64
#define STRID_MENU_DELITEM          65
#define STRID_MENU_CLEARITEM        66
#define STRID_VARIABLE              67
#define STRID_VALUE                 68
#define STRID_MAKEGLOBAL            69
#define STRID_LOWPRIORITY           70
#define STRID_COLOR                 71
#define STRID_SYNTAXFILE            72
#define STRID_VARTHEMENAME          73
#define STRID_ADDVAR                74
#define STRID_EDITVAR               75
#define STRID_LOSTPROMPT            76
#define STRID_OVERWRITEPROMPT       77
#define STRID_DELETEPROMPT          78
#define STRID_VARMISSING            79
#define STRID_REGEXP_COMPILEERROR   80
#define STRID_UNKNOWNSECTION        81
#define STRID_UNKNOWNSYNTAXFILE     82
#define STRID_UNKNOWNVARTHEME       83

#define DLLA_CODER_SETEXTENSION     1
#define DLLA_CODER_CLEARCACHE       2
#define DLLA_CODER_SETTINGS         3
#define DLLA_CODER_EXPORT           4
#define DLLA_CODER_SETVARTHEME      5
#define DLLA_CODER_SETALIAS         6
#define DLLA_CODER_CHECKALIAS       12
#define DLLA_CODER_CHECKVARTHEME    14
#define DLLA_CODER_GETSYNTAXFILE    16
#define DLLA_CODER_GETALIAS         18
#define DLLA_CODER_GETVARTHEME      20
#define DLLA_CODER_GETVARIABLE      22
#define DLLA_CODER_FILLVARLIST      23
#define DLLA_CODER_GETVARTHEMEDATA  24

#define OF_CLEAR                    0x001
#define OF_GENERAL_GLOBALTHEME      0x002
#define OF_GENERAL_ACTIVETHEME      0x004
#define OF_GENERAL_ALLTHEMES        0x008
#define OF_GENERAL_THEMELINK        0x010
#define OF_GENERAL_SETTINGS         0x020
#define OF_HIGHLIGHT_SETTINGS       0x040
#define OF_CODEFOLD_RECT            0x080
#define OF_CODEFOLD_SETTINGS        0x100
#define OF_AUTOCOMPLETE_RECT        0x200
#define OF_AUTOCOMPLETE_SETTINGS    0x400

#define OF_GENERAL      (OF_GENERAL_ALLTHEMES |\
                         OF_GENERAL_THEMELINK|\
                         OF_GENERAL_SETTINGS)
#define OF_HIGHLIGHT     OF_HIGHLIGHT_SETTINGS
#define OF_CODEFOLD     (OF_CODEFOLD_RECT |\
                         OF_CODEFOLD_SETTINGS)
#define OF_AUTOCOMPLETE (OF_AUTOCOMPLETE_RECT |\
                         OF_AUTOCOMPLETE_SETTINGS)

#define BUFFER_SIZE      1024

#define FIRST_NONLATIN   128

#ifndef IDI_ICON_MAIN
  #define IDI_ICON_MAIN 1001
#endif
#ifndef PSCB_BUTTONPRESSED
  #define PSCB_BUTTONPRESSED 3
#endif
#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
  #define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#endif
#ifndef LVS_EX_GRIDLINES
  #define LVS_EX_GRIDLINES 0x00000001
#endif
#ifndef LVS_EX_FULLROWSELECT
  #define LVS_EX_FULLROWSELECT 0x00000020
#endif

//Combobox edit ID
#define IDC_COMBOBOXEDIT       1001

#define PAGE_GENERAL       0
#define PAGE_HIGHLIGHT     1
#define PAGE_CODEFOLD1     2
#define PAGE_CODEFOLD2     3
#define PAGE_AUTOCOMPLETE1 4
#define PAGE_AUTOCOMPLETE2 5
#define PAGE_MAX           6

#define LVSI_LIST_VARIABLE  0
#define LVSI_LIST_VALUE     1
#define LVSI_LIST_COLOR     2

#define LVSI_LINK_SYNTAXFILE 0
#define LVSI_LINK_VARTHEME   1

//Use cache
#define UC_NONE  0
#define UC_FAST  1
#define UC_SMART 2

//Save cache
#define SC_NONE        0x0
#define SC_SAVE        0x1
#define SC_CLEAR       0x2

#define CODER_HIGHLIGHT     0x01
#define CODER_CODEFOLD      0x02
#define CODER_AUTOCOMPLETE  0x04
#define CODER_ALL           (CODER_HIGHLIGHT   |\
                             CODER_CODEFOLD    |\
                             CODER_AUTOCOMPLETE)

//Variable flags
#define VARF_LOWPRIORITY   0x001 //Global variable has low priority.
                                 //Next flags for DLLA_CODER_FILLVARLIST:
#define VARF_EXTSTRING     0x100 //Copy string pointer to (const wchar_t *)CODERTHEMEITEM.nVarValue.
#define VARF_EXTINTCOLOR   0x200 //Copy color integer to (COLORREF)CODERTHEMEITEM.nVarValue or -1 if not color.
#define VARF_EXTLPINTCOLOR 0x400 //Copy color integer to (COLORREF *)CODERTHEMEITEM.nVarValue or -1 if not color.

//UpdateEdit flags
#define UE_ERASE        0x01
#define UE_ALLRECT      0x02
#define UE_DRAWRECT     0x04
#define UE_FIRSTPIXEL   0x08
#define UE_UPDATEWINDOW 0x10

//DLLA_CODER_EXPORT flags
#define HLEXP_FILE                 0x00000001  //Export to file.
#define HLEXP_COPY                 0x00000010  //Copy to clipboard.
#define HLEXP_COPYTEXT             0x00000020  //Fill plain-text format of clipboard with text. Must be combined with HLEXP_COPY.
#define HLEXP_COPYDATA             0x00000040  //Fill plain-text format of clipboard with formated data. Must be combined with HLEXP_COPY.
#define HLEXP_SELECTIONCOLOR       0x00000100  //Export text selection color.
#define HLEXP_ACTIVELINECOLOR      0x00000200  //Export active line colors.
#define HLEXP_ALTLINECOLOR         0x00000400  //Export alternating lines colors.
#define HLEXP_NOBASICTEXTCOLOR     0x00000800  //Don't export basic text color.
#define HLEXP_NOBASICBKCOLOR       0x00001000  //Don't export basic background color.
#define HLEXP_ONLYSELECTION        0x00010000  //Export only selected text. By default exported selection or all document if selection is empty.
#define HLEXP_ALLDOCUMENT          0x00020000  //Export all document. By default exported selection or all document if selection is empty.


//// Structures

typedef struct _RTFCOLORITEM {
  struct _RTFCOLORITEM *next;
  struct _RTFCOLORITEM *prev;
  DWORD dwColor;
  char szRGB[64];
  int nRGBLen;
} RTFCOLORITEM;

typedef struct {
  RTFCOLORITEM *first;
  RTFCOLORITEM *last;
  int nCount;
  int nTableLen;
} STACKRTFCOLOR;

typedef struct {
  char *szRtf;
  INT_PTR nRtfLen;
  STACKRTFCOLOR hColorStack;
  LOGFONTW lfFont;
  AECOLORS aec;
  int nPointSize;
  char szDefStyle[32];
  char szDefTextColor[32];
  char szDefBkColor[32];
  AECHARRANGE crRange;
  DWORD dwFlags;
} RTFDATA;

typedef struct _WILDCARDINFO {
  struct _WILDCARDINFO *next;
  struct _WILDCARDINFO *prev;
  wchar_t *wpWildcard;
  int nWildcardLen;
} WILDCARDINFO;

typedef struct {
  WILDCARDINFO *first;
  WILDCARDINFO *last;
} STACKWILDCARD;

typedef struct _DELIMITERINFO {
  struct _DELIMITERINFO *next;
  struct _DELIMITERINFO *prev;
  wchar_t *wpDelimiter;
  int nDelimiterLen;
  DWORD dwFlags;
  DWORD dwColor1;
  DWORD dwColor2;
  DWORD dwFontStyle;
  int nParentID;
} DELIMITERINFO;

typedef struct {
  DELIMITERINFO *first;
  DELIMITERINFO *last;
} STACKDELIM;

typedef struct _MANUALSET {
  struct _MANUALSET *next;
  struct _MANUALSET *prev;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  wchar_t wszAlias[MAX_PATH];

  //User window
  DWORD dwDllFunction;
  HWND hWndParent;
  WNDPROC lpOldEditProc;
  WNDPROC lpOldParentProc;
  void *lpHighlightWindow; //Pointer to HIGHLIGHTWINDOW
  void *lpFoldWindow;      //Pointer to FOLDWINDOW
} MANUALSET;

typedef struct {
  MANUALSET *first;
  MANUALSET *last;
} STACKMANUALSET;

typedef struct {
  INT_PTR first; //WORDINFO *
  INT_PTR last;  //WORDINFO *
  INT_PTR lpWordLens[MAX_PATH];
} STACKWORD;

typedef struct {
  INT_PTR first; //WORDORDER *
  INT_PTR last;  //WORDORDER *
} STACKWORDORDER;

typedef struct {
  INT_PTR first; //WORDALPHA *
  INT_PTR last;  //WORDALPHA *
  INT_PTR lpSorted[FIRST_NONLATIN + 1];
} STACKWORDALPHA;

typedef struct {
  INT_PTR first;
  INT_PTR last;
} STACKQUOTE;

typedef struct {
  INT_PTR first;
  INT_PTR last;
} STACKSKIP;

typedef struct {
  INT_PTR first;
  INT_PTR last;
  int nCommonFirstChar;
  BOOL bVisible;
  int nFoldWithThemeCount;
} STACKFOLD;

typedef struct {
  const wchar_t *wpVarName;
  INT_PTR nVarValue;
  DWORD dwVarFlags;         //See VARF_* defines.
} CODERTHEMEITEM;

typedef struct _VARINFO {
  struct _VARINFO *next;
  struct _VARINFO *prev;
  wchar_t *wpVarName;
  int nVarNameLen;
  wchar_t *wpVarValue;
  int nVarValueLen;
  DWORD dwVarFlags;         //See VARF_* defines.
} VARINFO;

typedef struct {
  VARINFO *first;
  VARINFO *last;
  void *lpVarThemeOwner;
} STACKVAR;

typedef struct _VARTHEME {
  struct _VARTHEME *next;
  struct _VARTHEME *prev;
  STACKVAR hVarStack;
  wchar_t wszVarThemeName[MAX_PATH];
  int nVarThemeNameLen;
  const wchar_t *wpTextData;
} VARTHEME;

typedef struct {
  VARTHEME *first;
  VARTHEME *last;
} STACKVARTHEME;

typedef struct _TITLEINFO {
  struct _TITLEINFO *next;
  struct _TITLEINFO *prev;
  wchar_t *wpTitle;
  int nTitleLen;
  DWORD dwTitleFlags;     //See TF_* defines
  BOOL bExactTitle;
  STACKREGROUP *sregTitle;
} TITLEINFO;

typedef struct {
  TITLEINFO *first;
  TITLEINFO *last;
} STACKTITLE;

typedef struct _STACKBLOCK {
  INT_PTR first; //BLOCKINFO *
  INT_PTR last;  //BLOCKINFO *
  INT_PTR lpSorted[FIRST_NONLATIN + 1];
} STACKBLOCK;

typedef struct {
  INT_PTR first; //BLOCKORDER *
  INT_PTR last;  //BLOCKORDER *
} STACKBLOCKORDER;

typedef struct _SYNTAXFILE {
  struct _SYNTAXFILE *next;
  struct _SYNTAXFILE *prev;
  STACKWILDCARD hWildcardStack;
  STACKDELIM hDelimiterStack;
  STACKWORD hWordStack;
  STACKWORDALPHA hWordAlphaStack;
  STACKWORDORDER hWordOrderStack;
  STACKQUOTE hQuoteStack;
  STACKSKIP hSkipStack;
  HSTACK hSkipStartStack;
  STACKFOLD hFoldStack;
  HSTACK hFoldStartStack;
  STACKTITLE hTitleStack;
  STACKBLOCK hBlockStack;
  STACKBLOCK hExactBlockStack;
  STACKBLOCKORDER hBlockOrderStack;
  wchar_t wszSyntaxFileName[MAX_PATH];
  VARTHEME *lpVarThemeLink;
  AEHTHEME hThemeHighLight;
  //HighLight
  DWORD dwFontFlags;
  LOGFONTW lfFont;
  AECOLORS aecColors;
  wchar_t wszBkImageFile[MAX_PATH];
  int nBkImageAlpha;
  DWORD dwAutoMarkFlags;
  DWORD dwAutoMarkFontStyle;
  DWORD dwAutoMarkTextColor;
  DWORD dwAutoMarkBkColor;
  //CodeFold
  DWORD dwFoldPanelFirstBkColor;
  DWORD dwFoldPanelSecondBkColor;
  DWORD dwFoldPanelNormalFoldColor;
  DWORD dwFoldPanelActiveFoldColor;
  DWORD dwFoldPanelNormalNodeOpenBkColor;
  DWORD dwFoldPanelNormalNodeCloseBkColor;
  DWORD dwFoldPanelActiveNodeOpenBkColor;
  DWORD dwFoldPanelActiveNodeCloseBkColor;
  DWORD dwFoldPanelNormalNodeOpenSignColor;
  DWORD dwFoldPanelNormalNodeCloseSignColor;
  DWORD dwFoldPanelActiveNodeOpenSignColor;
  DWORD dwFoldPanelActiveNodeCloseSignColor;
  DWORD dwFoldListTextColor;
  DWORD dwFoldListBkColor;
  DWORD dwTagMarkFlags;
  DWORD dwTagMarkFontStyle;
  DWORD dwTagMarkTextColor;
  DWORD dwTagMarkBkColor;
  //AutoComplete
  DWORD dwCompleteListFontStyle;
  int nCompleteListFontSize;
  wchar_t wszCompleteListFaceName[LF_FACESIZE];
  HFONT hCompleteListFont;
  int nCompleteListLineGap;
  wchar_t wszCompleteListBlockIcon[MAX_PATH];
  DWORD dwCompleteListBlockIconMargins;
  wchar_t wszCompleteListHlBaseIcon[MAX_PATH];
  DWORD dwCompleteListHlBaseIconMargins;
  wchar_t wszCompleteListDocWordIcon[MAX_PATH];
  DWORD dwCompleteListDocWordIconMargins;
  DWORD dwCompleteListIcons;
  HICON hCompleteListBlockIcon;
  HICON hCompleteListHlBaseIcon;
  HICON hCompleteListDocWordIcon;
  DWORD dwCompleteListBasicTextColor;
  DWORD dwCompleteListBasicBkColor;
  DWORD dwCompleteListSelTextColor;
  DWORD dwCompleteListSelBkColor;
  //Other
  FILETIME ftTimeStamp;
  BOOL bReserveCacheWildcard;
  BOOL bExists;
  BOOL bCache;
} SYNTAXFILE;

typedef struct {
  SYNTAXFILE *first;
  SYNTAXFILE *last;
} STACKSYNTAXFILE;


//// Functions prototypes

void ExportToRtf(HWND hWndEdit, DWORD dwFlags, wchar_t *wszRtfFile);
HANDLE GetRTF(HWND hWndEdit, AECHARRANGE *crRange, DWORD dwFlags, INT_PTR *lpnRtfLen);
DWORD CALLBACK GetHighLightCallback(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp);
int StackInsertRTFColor(STACKRTFCOLOR *hStack, DWORD dwColor);
int StackGetRTFColor(STACKRTFCOLOR *hStack, DWORD dwColor);
void StackFreeRTFColors(STACKRTFCOLOR *hStack);
void SettingsSheet(int nStartPage);
LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam);
int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam);
BOOL CALLBACK GeneralSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GeneralVarEditDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GeneralLinkDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewUserParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewUserEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult);
SYNTAXFILE* StackLoadSyntaxFile(STACKSYNTAXFILE *hStack, SYNTAXFILE *lpSyntaxFile);
void StackRequestSyntaxFile(SYNTAXFILE *lpSyntaxFile);
SYNTAXFILE* StackAddSyntaxFile(STACKSYNTAXFILE *hStack, const wchar_t *wpFile);
SYNTAXFILE* StackPushSortSyntaxFile(STACKSYNTAXFILE *hStack, const wchar_t *wpFile, int nUpDown);
SYNTAXFILE* StackGetSyntaxFileByFile(STACKSYNTAXFILE *hStack, const wchar_t *wpFile);
SYNTAXFILE* StackGetSyntaxFileByName(STACKSYNTAXFILE *hStack, const wchar_t *wpSyntaxFileName);
SYNTAXFILE* StackGetSyntaxFileByTheme(STACKSYNTAXFILE *hStack, HANDLE hTheme);
SYNTAXFILE* StackGetSyntaxFileByIndex(STACKSYNTAXFILE *hStack, int nIndex);
SYNTAXFILE* StackGetSyntaxFileByWindow(STACKSYNTAXFILE *hStack, HWND hWnd, AEHDOC hDoc, const wchar_t **wppAlias);
void StackUnsetLink(STACKSYNTAXFILE *hStack, VARTHEME *lpVarTheme);
void StackFreeSyntaxFiles(STACKSYNTAXFILE *hStack);
WILDCARDINFO* StackInsertWildcard(STACKWILDCARD *hStack, int nWildcardLen);
WILDCARDINFO* StackGetWildcard(STACKWILDCARD *hStack, const wchar_t *wpFile);
void StackFreeWildcard(STACKWILDCARD *hStack);
DELIMITERINFO* StackInsertDelimiter(STACKDELIM *hStack, int nDelimiterLen);
DELIMITERINFO* StackGetDelimiter(STACKDELIM *hStack, wchar_t wchDelimiter);
void StackFreeDelimiter(STACKDELIM *hStack);
wchar_t* AKD_wcschr(const wchar_t *s, wchar_t c);
BOOL IsInDelimiterList(const wchar_t *wpList, int nListLen, wchar_t c);
BOOL IsDelimiter(STACKDELIM *hDelimiterStack, HWND hWnd, int nChar);
BOOL IsDelimiterFromLeft(STACKDELIM *hDelimiterStack, HWND hWnd, const AECHARINDEX *ciChar);
BOOL IsDelimiterFromRight(STACKDELIM *hDelimiterStack, HWND hWnd, const AECHARINDEX *ciChar);
MANUALSET* StackInsertManual(STACKMANUALSET *hStack);
MANUALSET* StackGetManual(STACKMANUALSET *hStack, HWND hWndMaster, AEHDOC hDocMaster);
MANUALSET* StackGetManualByUserParent(STACKMANUALSET *hStack, HWND hWndParent);
void StackDeleteManual(STACKMANUALSET *hStack, MANUALSET *lpManual, DWORD dwDllFunction);

VARTHEME* StackInsertVarTheme(STACKVARTHEME *hStack, int nIndex);
VARTHEME* StackGetVarThemeByName(STACKVARTHEME *hStack, const wchar_t *wpVarThemeName);
VARTHEME* StackGetVarThemeByIndex(STACKVARTHEME *hStack, int nIndex);
int StackGetVarThemeIndex(STACKVARTHEME *hStack, VARTHEME *lpVarTheme);
void StackMoveVarTheme(STACKVARTHEME *hStack, VARTHEME *lpVarTheme, int nIndex);
void StackCopyVarTheme(VARTHEME *vtSrc, VARTHEME *vtDst);
void StackDeleteVarTheme(STACKVARTHEME *hStack, VARTHEME *lpVarTheme);
void StackFreeVarThemes(STACKVARTHEME *hStack);
void AddInternalVarThemes(STACKVARTHEME *hStack);
VARTHEME* RequestVarTheme(STACKVARTHEME *hStack, const wchar_t *wpVarThemeName);
void GetVarThemeGlobals(VARTHEME *lpVarTheme);
int FillVarThemeList(HWND hWnd, VARTHEME *lpVarTheme);
LPARAM ListViewItemParam(HWND hWnd, int nItem);
VARINFO* StackInsertVar(STACKVAR *hStack, const wchar_t *wpVarName, int nVarNameLen, const wchar_t *wpVarValue, int nVarValueLen, BOOL bUnescape);
VARINFO* StackGetVarByName(STACKVAR *hStack, const wchar_t *wpVarName, int nVarNameLen);
VARINFO* StackGetVarByIndex(STACKVAR *hStack, int nIndex);
void StackDeleteVar(STACKVAR *hStack, VARINFO *lpElement);
void StackFreeVars(STACKVAR *hStack);
int ParseStringToVars(STACKVAR *lpVarStack, const wchar_t *wpText);
DWORD ParseVarsToString(STACKVAR *lpVarStack, wchar_t **wpText);

int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote, STACKVAR *lpVarStack);
INT_PTR ExpandVars(const wchar_t *wpString, INT_PTR nStringLen, wchar_t *wszBuffer, INT_PTR nBufferSize, STACKVAR *lpVarStack);
BOOL NextLine(const wchar_t **wpText);
BOOL SkipComment(const wchar_t **wpText);
BOOL FileMaskCmp(const wchar_t *wpMaskStr, const wchar_t *wpFileStr);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
int GetBaseName(const wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen);
HWND GetCurEdit();
HWND GetFocusEdit();
void UpdateEdit(HWND hWnd, DWORD dwFlags);
void UpdateEditAndClones(HWND hWnd, DWORD dwFlags);
void UpdateEditAll(DWORD dwFlags);
HFONT CreateFontMethod(const wchar_t *wpFaceName, DWORD dwFontStyle, int nPointSize);
HICON GetIconMethod(wchar_t *wszIconFile, int nFileIconIndex, BOOL bBigIcons);
void GetIconParameters(const wchar_t *wpText, wchar_t *wszIconFile, int nMaxIconFile, int *nIconIndex, const wchar_t **wppText);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);
INT_PTR EscapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput);
INT_PTR UnescapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput);
INT_PTR GetEscapeParam(const wchar_t *wpText, const wchar_t **wpParamStart, const wchar_t **wpParamEnd, const wchar_t **wpTextNext);
INT_PTR StrToArray(const wchar_t *wpStr, void *lpArray, INT_PTR nArraySize, int nArrayItemSize, const wchar_t **wpNextStr);
BOOL SelectColorDialog(HWND hWndOwner, COLORREF *crColor);
void GetPosFromChar(HWND hWnd, INT_PTR nCharIndex, POINT *pt);

void ReadSyntaxFiles();
void SaveCache(DWORD dwFlags);
void ClearCache(BOOL bForceNewCache);
void UpdateAllOptions();
INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();


//// Global variables

extern char szBuffer[BUFFER_SIZE];
extern wchar_t wszBuffer[BUFFER_SIZE];
extern wchar_t wszMessage[MAX_PATH];
extern wchar_t wszPluginName[MAX_PATH];
extern wchar_t wszPluginTitle[MAX_PATH];
extern wchar_t wszCoderDir[MAX_PATH];
extern HINSTANCE hInstanceDLL;
extern HWND hMainWnd;
extern HWND hWndEdit;
extern HWND hMdiClient;
extern HICON hMainIcon;
extern BOOL bOldWindows;
extern BOOL bOldRichEdit;
extern BOOL bAkelEdit;
extern int nMDI;
extern LANGID wLangModule;
extern BOOL bInitCommon;
extern int nInitMain;
extern DWORD dwSaveFlags;
extern BOOL bInitHighLight;
extern int nInitCodeFold;
extern BOOL bInitAutoComplete;
extern BOOL bUseCache;
extern BOOL bSaveCache;
extern STACKSYNTAXFILE hSyntaxFilesStack;
extern STACKMANUALSET hManualStack;
extern STACKVARTHEME hVarThemesStack;
extern SYNTAXFILE *lpLoadSyntaxFile;
extern VARTHEME *lpVarThemeActive;
extern WNDPROCDATA *NewMainProcData;
extern WNDPROCDATA *NewFrameProcData;
extern WNDPROCDATA *NewEditProcData;
extern HWND hWndPropSheet;
extern int nPropMaxVisitPage;
extern BOOL bSaveOptions;


//// Themes

//Default variable theme
#define TXT_DEFAULT_VARTHEME_BASIC \
L"STR #808080\r\
COMM #008000\r\
VAR #800000\r\
DEL1 #CC3333\r\
DEL2 0\r\
TYPE #000080\r\
OP #3333CC\r\
TAG #3333CC\r\
ATTR #339933\r\
IF #339933\r\
AREA #FF8000\r\
NUM #FF8000\r"

#define TXT_DEFAULT_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor 0\r\
HighLight_BasicBkColor 0\r\
HighLight_SelTextColor 0\r\
HighLight_SelBkColor 0\r\
HighLight_LineTextColor 0\r\
HighLight_LineBkColor 0\r\
HighLight_LineBorderColor 0\r\
HighLight_AltTextColor 0\r\
HighLight_AltBkColor 0\r\
HighLight_AltBorderColor 0\r\
HighLight_ColumnColor 0\r\
HighLight_MarkerColor 0\r\
HighLight_CaretColor 0\r\
HighLight_UrlColor 0\r\
HighLight_ActiveUrlColor 0\r\
HighLight_VisitUrlColor 0\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #000000\r\
HighLight_AutoMarkBkColor #75D975\r"

#define TXT_DEFAULT_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #FFFFFF\r\
CodeFold_PanelSecondBkColor #E9E9E9\r\
CodeFold_PanelNormalFoldColor #808080\r\
CodeFold_PanelActiveFoldColor #0000FF\r\
CodeFold_PanelNormalNodeOpenBkColor 0\r\
CodeFold_PanelNormalNodeCloseBkColor 0\r\
CodeFold_PanelActiveNodeOpenBkColor 0\r\
CodeFold_PanelActiveNodeCloseBkColor 0\r\
CodeFold_PanelNormalNodeOpenSignColor 0\r\
CodeFold_PanelNormalNodeCloseSignColor 0\r\
CodeFold_PanelActiveNodeOpenSignColor 0\r\
CodeFold_PanelActiveNodeCloseSignColor 0\r\
CodeFold_ListTextColor 0\r\
CodeFold_ListBkColor 0\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 2\r\
CodeFold_TagMarkTextColor 0\r\
CodeFold_TagMarkBkColor #E3CCFF\r"

#define TXT_DEFAULT_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor 0\r\
AutoComplete_ListBasicBkColor 0\r\
AutoComplete_ListSelTextColor 0\r\
AutoComplete_ListSelBkColor 0\r\
AutoComplete_Indent \"  \"\r"

#define TXT_DEFAULT_VARTHEME_LINEBOARD \
L"LineBoard_TextColor 0\r\
LineBoard_BkColor 0\r\
LineBoard_BorderColor 0\r\
LineBoard_BookmarkTextColor 0\r\
LineBoard_BookmarkBkColor 0\r\
LineBoard_BookmarkBorderColor 0\r\
LineBoard_LineUnsavedColor 0\r\
LineBoard_LineSavedColor 0\r\
LineBoard_RulerScaleColor 0\r\
LineBoard_RulerCaretColor 0\r"

#define TXT_DEFAULT_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor 0\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor 0\r\
SpecialChar_SelBkColor 0\r"

#define TXT_DEFAULT_VARTHEME_SESSIONS \
L"Sessions_ListTextColor 0\r\
Sessions_ListBkColor 0\r"

#define TXT_DEFAULT_VARTHEME (TXT_DEFAULT_VARTHEME_BASIC TXT_DEFAULT_VARTHEME_HIGHLIGHT TXT_DEFAULT_VARTHEME_CODEFOLD TXT_DEFAULT_VARTHEME_AUTOCOMPLETE TXT_DEFAULT_VARTHEME_LINEBOARD TXT_DEFAULT_VARTHEME_SPECIALCHAR TXT_DEFAULT_VARTHEME_SESSIONS)

//Active4D variable theme
#define TXT_ACTIVE4D_VARTHEME_BASIC \
L"STR #666666\r\
COMM #D33435\r\
VAR #0053FF\r\
DEL1 #7A7A7A\r\
DEL2 #000000\r\
TYPE #B7734C\r\
OP #45AE34\r\
TAG #016CFF\r\
ATTR #963DFF\r\
IF #006699\r\
AREA #317B24\r\
NUM #A8017E\r"

#define TXT_ACTIVE4D_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #000000\r\
HighLight_BasicBkColor #FFFFFF\r\
HighLight_SelTextColor #000000\r\
HighLight_SelBkColor #A5B0A2\r\
HighLight_LineTextColor #000000\r\
HighLight_LineBkColor #EBEBEB\r\
HighLight_LineBorderColor #E1E1E1\r\
HighLight_AltTextColor #000000\r\
HighLight_AltBkColor #F7F7F7\r\
HighLight_AltBorderColor #F2F2F2\r\
HighLight_ColumnColor #7A7A7A\r\
HighLight_MarkerColor #7A7A7A\r\
HighLight_CaretColor #000000\r\
HighLight_UrlColor #0079FF\r\
HighLight_ActiveUrlColor #0083AE\r\
HighLight_VisitUrlColor #0083AE\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #000000\r\
HighLight_AutoMarkBkColor #8DFF51\r"

#define TXT_ACTIVE4D_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #FFFFFF\r\
CodeFold_PanelSecondBkColor #FFFFFF\r\
CodeFold_PanelNormalFoldColor #7A7A7A\r\
CodeFold_PanelActiveFoldColor #000000\r\
CodeFold_PanelNormalNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelNormalNodeCloseBkColor #7A7A7A\r\
CodeFold_PanelActiveNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelActiveNodeCloseBkColor #000000\r\
CodeFold_PanelNormalNodeOpenSignColor #7A7A7A\r\
CodeFold_PanelNormalNodeCloseSignColor #FFFFFF\r\
CodeFold_PanelActiveNodeOpenSignColor #000000\r\
CodeFold_PanelActiveNodeCloseSignColor #FFFFFF\r\
CodeFold_ListTextColor #000000\r\
CodeFold_ListBkColor #FFFFFF\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #FFFFFF\r\
CodeFold_TagMarkBkColor #2FB1FF\r"

#define TXT_ACTIVE4D_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #000000\r\
AutoComplete_ListBasicBkColor #FFFFFF\r\
AutoComplete_ListSelTextColor #000000\r\
AutoComplete_ListSelBkColor #A5B0A2\r\
AutoComplete_Indent \"  \"\r"

#define TXT_ACTIVE4D_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #000000\r\
LineBoard_BkColor #EBEBEB\r\
LineBoard_BorderColor #FFFFFF\r\
LineBoard_BookmarkTextColor #FFFFFF\r\
LineBoard_BookmarkBkColor #666666\r\
LineBoard_BookmarkBorderColor #EBEBEB\r\
LineBoard_LineUnsavedColor #DBCD28\r\
LineBoard_LineSavedColor #50CF30\r\
LineBoard_RulerScaleColor #000000\r\
LineBoard_RulerCaretColor #DBCD28\r"

#define TXT_ACTIVE4D_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #CCCCCC\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #838383\r\
SpecialChar_SelBkColor 0\r"

#define TXT_ACTIVE4D_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #000000\r\
Sessions_ListBkColor #FFFFFF\r"

#define TXT_ACTIVE4D_VARTHEME (TXT_ACTIVE4D_VARTHEME_BASIC TXT_ACTIVE4D_VARTHEME_HIGHLIGHT TXT_ACTIVE4D_VARTHEME_CODEFOLD TXT_ACTIVE4D_VARTHEME_AUTOCOMPLETE TXT_ACTIVE4D_VARTHEME_LINEBOARD TXT_ACTIVE4D_VARTHEME_SPECIALCHAR TXT_ACTIVE4D_VARTHEME_SESSIONS)

//Bespin variable theme
#define TXT_BESPIN_VARTHEME_BASIC \
L"STR #80FF80\r\
COMM #707070\r\
VAR #FFFFFF\r\
DEL1 #A6DEFF\r\
DEL2 #FFAA00\r\
TYPE #FFAA00\r\
OP #37A8ED\r\
TAG #37A8ED\r\
ATTR #E5C138\r\
IF #F6F080\r\
AREA #55E439\r\
NUM #FF3A83\r"

#define TXT_BESPIN_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #E8E2DD\r\
HighLight_BasicBkColor #2B211C\r\
HighLight_SelTextColor #FFFFFF\r\
HighLight_SelBkColor #846759\r\
HighLight_LineTextColor #FFFFFF\r\
HighLight_LineBkColor #4B3D34\r\
HighLight_LineBorderColor #54443A\r\
HighLight_AltTextColor #E8E2DD\r\
HighLight_AltBkColor #281F1A\r\
HighLight_AltBorderColor #251D18\r\
HighLight_ColumnColor #656565\r\
HighLight_MarkerColor #656565\r\
HighLight_CaretColor #91EAB9\r\
HighLight_UrlColor #66C6FF\r\
HighLight_ActiveUrlColor #82EFFF\r\
HighLight_VisitUrlColor #82EFFF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #FFFFFF\r\
HighLight_AutoMarkBkColor #FF0080\r"

#define TXT_BESPIN_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #2B211C\r\
CodeFold_PanelSecondBkColor #2B211C\r\
CodeFold_PanelNormalFoldColor #494949\r\
CodeFold_PanelActiveFoldColor #808080\r\
CodeFold_PanelNormalNodeOpenBkColor #494949\r\
CodeFold_PanelNormalNodeCloseBkColor #494949\r\
CodeFold_PanelActiveNodeOpenBkColor #808080\r\
CodeFold_PanelActiveNodeCloseBkColor #808080\r\
CodeFold_PanelNormalNodeOpenSignColor #808080\r\
CodeFold_PanelNormalNodeCloseSignColor #C0C0C0\r\
CodeFold_PanelActiveNodeOpenSignColor #000000\r\
CodeFold_PanelActiveNodeCloseSignColor #FFFFFF\r\
CodeFold_ListTextColor #E8E2DD\r\
CodeFold_ListBkColor #2B211C\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #FFFFFF\r\
CodeFold_TagMarkBkColor #605620\r"

#define TXT_BESPIN_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #E8E2DD\r\
AutoComplete_ListBasicBkColor #2B211C\r\
AutoComplete_ListSelTextColor #FFFFFF\r\
AutoComplete_ListSelBkColor #846759\r\
AutoComplete_Indent \"  \"\r"

#define TXT_BESPIN_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #E8E2DD\r\
LineBoard_BkColor #4B3D34\r\
LineBoard_BorderColor #2B211C\r\
LineBoard_BookmarkTextColor #2B211C\r\
LineBoard_BookmarkBkColor #E8E2DD\r\
LineBoard_BookmarkBorderColor #4B3D34\r\
LineBoard_LineUnsavedColor #F6F080\r\
LineBoard_LineSavedColor #80FF80\r\
LineBoard_RulerScaleColor #E8E2DD\r\
LineBoard_RulerCaretColor #F6F080\r"

#define TXT_BESPIN_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #555555\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #9B9B9B\r\
SpecialChar_SelBkColor 0\r"

#define TXT_BESPIN_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #E8E2DD\r\
Sessions_ListBkColor #2B211C\r"

#define TXT_BESPIN_VARTHEME (TXT_BESPIN_VARTHEME_BASIC TXT_BESPIN_VARTHEME_HIGHLIGHT TXT_BESPIN_VARTHEME_CODEFOLD TXT_BESPIN_VARTHEME_AUTOCOMPLETE TXT_BESPIN_VARTHEME_LINEBOARD TXT_BESPIN_VARTHEME_SPECIALCHAR TXT_BESPIN_VARTHEME_SESSIONS)

//Cobalt variable theme
#define TXT_COBALT_VARTHEME_BASIC \
L"STR #3AD900\r\
COMM #0088FF\r\
VAR #E1EFFF\r\
DEL1 #E1EFFF\r\
DEL2 #FF9D00\r\
TYPE #FFAA00\r\
OP #FFEE80\r\
TAG #9EFFFF\r\
ATTR #9EFFFF\r\
IF #FF9D00\r\
AREA #9DF39F\r\
NUM #FF628C\r"

#define TXT_COBALT_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #FFFFFF\r\
HighLight_BasicBkColor #002240\r\
HighLight_SelTextColor #FFFFFF\r\
HighLight_SelBkColor #86543A\r\
HighLight_LineTextColor #FFFFFF\r\
HighLight_LineBkColor #001629\r\
HighLight_LineBorderColor #001122\r\
HighLight_AltTextColor #FFFFFF\r\
HighLight_AltBkColor #001F3C\r\
HighLight_AltBorderColor #001D37\r\
HighLight_ColumnColor #656565\r\
HighLight_MarkerColor #656565\r\
HighLight_CaretColor #91EAB9\r\
HighLight_UrlColor #66C6FF\r\
HighLight_ActiveUrlColor #B9E4FF\r\
HighLight_VisitUrlColor #B9E4FF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #FFFFFF\r\
HighLight_AutoMarkBkColor #4000FF\r"

#define TXT_COBALT_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #002240\r\
CodeFold_PanelSecondBkColor #002240\r\
CodeFold_PanelNormalFoldColor #47545F\r\
CodeFold_PanelActiveFoldColor #7F909F\r\
CodeFold_PanelNormalNodeOpenBkColor #47545F\r\
CodeFold_PanelNormalNodeCloseBkColor #47545F\r\
CodeFold_PanelActiveNodeOpenBkColor #7F909F\r\
CodeFold_PanelActiveNodeCloseBkColor #7F909F\r\
CodeFold_PanelNormalNodeOpenSignColor #7F909F\r\
CodeFold_PanelNormalNodeCloseSignColor #C2CAD1\r\
CodeFold_PanelActiveNodeOpenSignColor #030405\r\
CodeFold_PanelActiveNodeCloseSignColor #FFFFFF\r\
CodeFold_ListTextColor #FFFFFF\r\
CodeFold_ListBkColor #002240\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #FFFFFF\r\
CodeFold_TagMarkBkColor #433B3D\r"

#define TXT_COBALT_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #FFFFFF\r\
AutoComplete_ListBasicBkColor #002240\r\
AutoComplete_ListSelTextColor #FFFFFF\r\
AutoComplete_ListSelBkColor #86543A\r\
AutoComplete_Indent \"  \"\r"

#define TXT_COBALT_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #FFFFFF\r\
LineBoard_BkColor #001629\r\
LineBoard_BorderColor #002240\r\
LineBoard_BookmarkTextColor #002240\r\
LineBoard_BookmarkBkColor #E1EFFF\r\
LineBoard_BookmarkBorderColor #001629\r\
LineBoard_LineUnsavedColor #FF9D00\r\
LineBoard_LineSavedColor #3AD900\r\
LineBoard_RulerScaleColor #FFFFFF\r\
LineBoard_RulerCaretColor #FF9D00\r"

#define TXT_COBALT_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #404A55\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #95A3B0\r\
SpecialChar_SelBkColor 0\r"

#define TXT_COBALT_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #FFFFFF\r\
Sessions_ListBkColor #002240\r"

#define TXT_COBALT_VARTHEME (TXT_COBALT_VARTHEME_BASIC TXT_COBALT_VARTHEME_HIGHLIGHT TXT_COBALT_VARTHEME_CODEFOLD TXT_COBALT_VARTHEME_AUTOCOMPLETE TXT_COBALT_VARTHEME_LINEBOARD TXT_COBALT_VARTHEME_SPECIALCHAR TXT_COBALT_VARTHEME_SESSIONS)

//Dawn variable theme
#define TXT_DAWN_VARTHEME_BASIC \
L"STR #0B6125\r\
COMM #5A525F\r\
VAR #080808\r\
DEL1 #080808\r\
DEL2 #794938\r\
TYPE #691C97\r\
OP #BF4F24\r\
TAG #BF4F24\r\
ATTR #A71D5D\r\
IF #794938\r\
AREA #5A525F\r\
NUM #811F24\r"

#define TXT_DAWN_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #080808\r\
HighLight_BasicBkColor #F9F9F9\r\
HighLight_SelTextColor #080808\r\
HighLight_SelBkColor #B9CAFA\r\
HighLight_LineTextColor #080808\r\
HighLight_LineBkColor #DFE6F0\r\
HighLight_LineBorderColor #D7DFEC\r\
HighLight_AltTextColor #080808\r\
HighLight_AltBkColor #F4F4F4\r\
HighLight_AltBorderColor #EFEFEF\r\
HighLight_ColumnColor #7C7C7C\r\
HighLight_MarkerColor #7C7C7C\r\
HighLight_CaretColor #080808\r\
HighLight_UrlColor #0FA5FF\r\
HighLight_ActiveUrlColor #6B40FF\r\
HighLight_VisitUrlColor #6B40FF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #080808\r\
HighLight_AutoMarkBkColor #DAFFB5\r"

#define TXT_DAWN_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #F9F9F9\r\
CodeFold_PanelSecondBkColor #F9F9F9\r\
CodeFold_PanelNormalFoldColor #808080\r\
CodeFold_PanelActiveFoldColor #080808\r\
CodeFold_PanelNormalNodeOpenBkColor #F9F9F9\r\
CodeFold_PanelNormalNodeCloseBkColor #808080\r\
CodeFold_PanelActiveNodeOpenBkColor #F9F9F9\r\
CodeFold_PanelActiveNodeCloseBkColor #080808\r\
CodeFold_PanelNormalNodeOpenSignColor #808080\r\
CodeFold_PanelNormalNodeCloseSignColor #F9F9F9\r\
CodeFold_PanelActiveNodeOpenSignColor #080808\r\
CodeFold_PanelActiveNodeCloseSignColor #F9F9F9\r\
CodeFold_ListTextColor #080808\r\
CodeFold_ListBkColor #F9F9F9\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #080808\r\
CodeFold_TagMarkBkColor #BAE2FA\r"

#define TXT_DAWN_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #080808\r\
AutoComplete_ListBasicBkColor #F9F9F9\r\
AutoComplete_ListSelTextColor #080808\r\
AutoComplete_ListSelBkColor #B9CAFA\r\
AutoComplete_Indent \"  \"\r"

#define TXT_DAWN_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #080808\r\
LineBoard_BkColor #E7E7E7\r\
LineBoard_BorderColor #F9F9F9\r\
LineBoard_BookmarkTextColor #F9F9F9\r\
LineBoard_BookmarkBkColor #7A7A7A\r\
LineBoard_BookmarkBorderColor #DFE6F0\r\
LineBoard_LineUnsavedColor #D35727\r\
LineBoard_LineSavedColor #108B35\r\
LineBoard_RulerScaleColor #080808\r\
LineBoard_RulerCaretColor #D35727\r"

#define TXT_DAWN_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #CCCCCC\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #999999\r\
SpecialChar_SelBkColor 0\r"

#define TXT_DAWN_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #080808\r\
Sessions_ListBkColor #F9F9F9\r"

#define TXT_DAWN_VARTHEME (TXT_DAWN_VARTHEME_BASIC TXT_DAWN_VARTHEME_HIGHLIGHT TXT_DAWN_VARTHEME_CODEFOLD TXT_DAWN_VARTHEME_AUTOCOMPLETE TXT_DAWN_VARTHEME_LINEBOARD TXT_DAWN_VARTHEME_SPECIALCHAR TXT_DAWN_VARTHEME_SESSIONS)

//Earth variable theme
#define TXT_EARTH_VARTHEME_BASIC \
L"STR #CC7800\r\
COMM #748AB0\r\
VAR #6D3206\r\
DEL1 #000000\r\
DEL2 #000000\r\
TYPE #3B5BB5\r\
OP #07377E\r\
TAG #0A267C\r\
ATTR #000000\r\
IF #0A327C\r\
AREA #748AB0\r\
NUM #3B5BB5\r"

#define TXT_EARTH_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #000000\r\
HighLight_BasicBkColor #FFFFFF\r\
HighLight_SelTextColor #000000\r\
HighLight_SelBkColor #B5CCE3\r\
HighLight_LineTextColor #000000\r\
HighLight_LineBkColor #F1F1DA\r\
HighLight_LineBorderColor #EDEDCD\r\
HighLight_AltTextColor #000000\r\
HighLight_AltBkColor #FAFAFA\r\
HighLight_AltBorderColor #F4F4F4\r\
HighLight_ColumnColor #7C7C7C\r\
HighLight_MarkerColor #7C7C7C\r\
HighLight_CaretColor #000000\r\
HighLight_UrlColor #0A0A7C\r\
HighLight_ActiveUrlColor #4A4AF0\r\
HighLight_VisitUrlColor #4A4AF0\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #000000\r\
HighLight_AutoMarkBkColor #C0FF97\r"

#define TXT_EARTH_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #FFFFFF\r\
CodeFold_PanelSecondBkColor #FFFFFF\r\
CodeFold_PanelNormalFoldColor #7F7F7F\r\
CodeFold_PanelActiveFoldColor #000000\r\
CodeFold_PanelNormalNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelNormalNodeCloseBkColor #7F7F7F\r\
CodeFold_PanelActiveNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelActiveNodeCloseBkColor #000000\r\
CodeFold_PanelNormalNodeOpenSignColor #7F7F7F\r\
CodeFold_PanelNormalNodeCloseSignColor #FFFFFF\r\
CodeFold_PanelActiveNodeOpenSignColor #000000\r\
CodeFold_PanelActiveNodeCloseSignColor #FFFFFF\r\
CodeFold_ListTextColor #000000\r\
CodeFold_ListBkColor #FFFFFF\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #000000\r\
CodeFold_TagMarkBkColor #B0EAFF\r"

#define TXT_EARTH_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #000000\r\
AutoComplete_ListBasicBkColor #FFFFFF\r\
AutoComplete_ListSelTextColor #000000\r\
AutoComplete_ListSelBkColor #B5CCE3\r\
AutoComplete_Indent \"  \"\r"

#define TXT_EARTH_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #000000\r\
LineBoard_BkColor #E5E5E5\r\
LineBoard_BorderColor #FFFFFF\r\
LineBoard_BookmarkTextColor #FFFFFF\r\
LineBoard_BookmarkBkColor #747474\r\
LineBoard_BookmarkBorderColor #F1F1DA\r\
LineBoard_LineUnsavedColor #CC7800\r\
LineBoard_LineSavedColor #2FB900\r\
LineBoard_RulerScaleColor #000000\r\
LineBoard_RulerCaretColor #CC7800\r"

#define TXT_EARTH_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #D3D3D3\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #9A9A9A\r\
SpecialChar_SelBkColor 0\r"

#define TXT_EARTH_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #000000\r\
Sessions_ListBkColor #FFFFFF\r"

#define TXT_EARTH_VARTHEME (TXT_EARTH_VARTHEME_BASIC TXT_EARTH_VARTHEME_HIGHLIGHT TXT_EARTH_VARTHEME_CODEFOLD TXT_EARTH_VARTHEME_AUTOCOMPLETE TXT_EARTH_VARTHEME_LINEBOARD TXT_EARTH_VARTHEME_SPECIALCHAR TXT_EARTH_VARTHEME_SESSIONS)

//iPlastic variable theme
#define TXT_IPLASTIC_VARTHEME_BASIC \
L"STR #009933\r\
COMM #0066FF\r\
VAR #000000\r\
DEL1 #0033CC\r\
DEL2 #0000FF\r\
TYPE #9700CC\r\
OP #1A00FF\r\
TAG #0033CC\r\
ATTR #3366CC\r\
IF #0000FF\r\
AREA #748AB0\r\
NUM #0066FF\r"

#define TXT_IPLASTIC_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #000000\r\
HighLight_BasicBkColor #EEEEEE\r\
HighLight_SelTextColor #000000\r\
HighLight_SelBkColor #BAD6FD\r\
HighLight_LineTextColor #000000\r\
HighLight_LineBkColor #DFDFDF\r\
HighLight_LineBorderColor #DADADA\r\
HighLight_AltTextColor #000000\r\
HighLight_AltBkColor #F2F2F2\r\
HighLight_AltBorderColor #E9E9E9\r\
HighLight_ColumnColor #777777\r\
HighLight_MarkerColor #777777\r\
HighLight_CaretColor #000000\r\
HighLight_UrlColor #004DCC\r\
HighLight_ActiveUrlColor #0D68FF\r\
HighLight_VisitUrlColor #0D68FF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #EEEEEE\r\
HighLight_AutoMarkBkColor #FF0080\r"

#define TXT_IPLASTIC_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #EEEEEE\r\
CodeFold_PanelSecondBkColor #EEEEEE\r\
CodeFold_PanelNormalFoldColor #777777\r\
CodeFold_PanelActiveFoldColor #000000\r\
CodeFold_PanelNormalNodeOpenBkColor #EEEEEE\r\
CodeFold_PanelNormalNodeCloseBkColor #777777\r\
CodeFold_PanelActiveNodeOpenBkColor #EEEEEE\r\
CodeFold_PanelActiveNodeCloseBkColor #000000\r\
CodeFold_PanelNormalNodeOpenSignColor #777777\r\
CodeFold_PanelNormalNodeCloseSignColor #EEEEEE\r\
CodeFold_PanelActiveNodeOpenSignColor #000000\r\
CodeFold_PanelActiveNodeCloseSignColor #EEEEEE\r\
CodeFold_ListTextColor #000000\r\
CodeFold_ListBkColor #EEEEEE\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #000000\r\
CodeFold_TagMarkBkColor #D4E2F5\r"

#define TXT_IPLASTIC_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #000000\r\
AutoComplete_ListBasicBkColor #EEEEEE\r\
AutoComplete_ListSelTextColor #000000\r\
AutoComplete_ListSelBkColor #BAD6FD\r\
AutoComplete_Indent \"  \"\r"

#define TXT_IPLASTIC_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #000000\r\
LineBoard_BkColor #DFDFDF\r\
LineBoard_BorderColor #EEEEEE\r\
LineBoard_BookmarkTextColor #EEEEEE\r\
LineBoard_BookmarkBkColor #777777\r\
LineBoard_BookmarkBorderColor #DFDFDF\r\
LineBoard_LineUnsavedColor #EC9900\r\
LineBoard_LineSavedColor #009933\r\
LineBoard_RulerScaleColor #000000\r\
LineBoard_RulerCaretColor #EC9900\r"

#define TXT_IPLASTIC_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #C3C3C3\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #999999\r\
SpecialChar_SelBkColor 0\r"

#define TXT_IPLASTIC_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #000000\r\
Sessions_ListBkColor #EEEEEE\r"

#define TXT_IPLASTIC_VARTHEME (TXT_IPLASTIC_VARTHEME_BASIC TXT_IPLASTIC_VARTHEME_HIGHLIGHT TXT_IPLASTIC_VARTHEME_CODEFOLD TXT_IPLASTIC_VARTHEME_AUTOCOMPLETE TXT_IPLASTIC_VARTHEME_LINEBOARD TXT_IPLASTIC_VARTHEME_SPECIALCHAR TXT_IPLASTIC_VARTHEME_SESSIONS)

//Lazy variable theme
#define TXT_LAZY_VARTHEME_BASIC \
L"STR #409B1C\r\
COMM #8C868F\r\
VAR #000000\r\
DEL1 #3A4A64\r\
DEL2 #FF7800\r\
TYPE #3B5BB5\r\
OP #3A4A64\r\
TAG #416476\r\
ATTR #384965\r\
IF #FF7800\r\
AREA #8C868F\r\
NUM #3B5BB5\r"

#define TXT_LAZY_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #000000\r\
HighLight_BasicBkColor #FFFFFF\r\
HighLight_SelTextColor #000000\r\
HighLight_SelBkColor #E3FC8D\r\
HighLight_LineTextColor #000000\r\
HighLight_LineBkColor #F6FDCD\r\
HighLight_LineBorderColor #F1FEB6\r\
HighLight_AltTextColor #000000\r\
HighLight_AltBkColor #F7F7F7\r\
HighLight_AltBorderColor #F3F3F3\r\
HighLight_ColumnColor #7C7C7C\r\
HighLight_MarkerColor #7C7C7C\r\
HighLight_CaretColor #000000\r\
HighLight_UrlColor #0080FF\r\
HighLight_ActiveUrlColor #005AE1\r\
HighLight_VisitUrlColor #005AE1\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #000000\r\
HighLight_AutoMarkBkColor #FCF98D\r"

#define TXT_LAZY_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #FFFFFF\r\
CodeFold_PanelSecondBkColor #FFFFFF\r\
CodeFold_PanelNormalFoldColor #7F7F7F\r\
CodeFold_PanelActiveFoldColor #000000\r\
CodeFold_PanelNormalNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelNormalNodeCloseBkColor #7F7F7F\r\
CodeFold_PanelActiveNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelActiveNodeCloseBkColor #000000\r\
CodeFold_PanelNormalNodeOpenSignColor #7F7F7F\r\
CodeFold_PanelNormalNodeCloseSignColor #FFFFFF\r\
CodeFold_PanelActiveNodeOpenSignColor #000000\r\
CodeFold_PanelActiveNodeCloseSignColor #FFFFFF\r\
CodeFold_ListTextColor #000000\r\
CodeFold_ListBkColor #FFFFFF\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #000000\r\
CodeFold_TagMarkBkColor #FAD872\r"

#define TXT_LAZY_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #000000\r\
AutoComplete_ListBasicBkColor #FFFFFF\r\
AutoComplete_ListSelTextColor #000000\r\
AutoComplete_ListSelBkColor #E3FC8D\r\
AutoComplete_Indent \"  \"\r"

#define TXT_LAZY_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #000000\r\
LineBoard_BkColor #E5E5E5\r\
LineBoard_BorderColor #FFFFFF\r\
LineBoard_BookmarkTextColor #FFFFFF\r\
LineBoard_BookmarkBkColor #A0A0A0\r\
LineBoard_BookmarkBorderColor #E5E5E5\r\
LineBoard_LineUnsavedColor #FF7800\r\
LineBoard_LineSavedColor #409B1C\r\
LineBoard_RulerScaleColor #000000\r\
LineBoard_RulerCaretColor #FF7800\r"

#define TXT_LAZY_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #D2D2D2\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #BBBBBB\r\
SpecialChar_SelBkColor 0\r"

#define TXT_LAZY_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #000000\r\
Sessions_ListBkColor #FFFFFF\r"

#define TXT_LAZY_VARTHEME (TXT_LAZY_VARTHEME_BASIC TXT_LAZY_VARTHEME_HIGHLIGHT TXT_LAZY_VARTHEME_CODEFOLD TXT_LAZY_VARTHEME_AUTOCOMPLETE TXT_LAZY_VARTHEME_LINEBOARD TXT_LAZY_VARTHEME_SPECIALCHAR TXT_LAZY_VARTHEME_SESSIONS)

//Mac Classic variable theme
#define TXT_MACCLASSIC_VARTHEME_BASIC \
L"STR #036A07\r\
COMM #0066FF\r\
VAR #000000\r\
DEL1 #1C02FF\r\
DEL2 #687687\r\
TYPE #585CF6\r\
OP #1A00FF\r\
TAG #0A267C\r\
ATTR #1C02FF\r\
IF #0000FF\r\
AREA #748AB0\r\
NUM #0000CD\r"

#define TXT_MACCLASSIC_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #000000\r\
HighLight_BasicBkColor #FFFFFF\r\
HighLight_SelTextColor #000000\r\
HighLight_SelBkColor #C4DCFF\r\
HighLight_LineTextColor #000000\r\
HighLight_LineBkColor #ECECEC\r\
HighLight_LineBorderColor #E7E7E7\r\
HighLight_AltTextColor #000000\r\
HighLight_AltBkColor #F8F8F8\r\
HighLight_AltBorderColor #F4F4F4\r\
HighLight_ColumnColor #7C7C7C\r\
HighLight_MarkerColor #7C7C7C\r\
HighLight_CaretColor #000000\r\
HighLight_UrlColor #0A0A7C\r\
HighLight_ActiveUrlColor #1212DE\r\
HighLight_VisitUrlColor #1212DE\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #000000\r\
HighLight_AutoMarkBkColor #95DAFF\r"

#define TXT_MACCLASSIC_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #FFFFFF\r\
CodeFold_PanelSecondBkColor #FFFFFF\r\
CodeFold_PanelNormalFoldColor #7F7F7F\r\
CodeFold_PanelActiveFoldColor #000000\r\
CodeFold_PanelNormalNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelNormalNodeCloseBkColor #7F7F7F\r\
CodeFold_PanelActiveNodeOpenBkColor #FFFFFF\r\
CodeFold_PanelActiveNodeCloseBkColor #000000\r\
CodeFold_PanelNormalNodeOpenSignColor #7F7F7F\r\
CodeFold_PanelNormalNodeCloseSignColor #FFFFFF\r\
CodeFold_PanelActiveNodeOpenSignColor #000000\r\
CodeFold_PanelActiveNodeCloseSignColor #FFFFFF\r\
CodeFold_ListTextColor #000000\r\
CodeFold_ListBkColor #FFFFFF\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #000000\r\
CodeFold_TagMarkBkColor #D8FF95\r"

#define TXT_MACCLASSIC_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #000000\r\
AutoComplete_ListBasicBkColor #FFFFFF\r\
AutoComplete_ListSelTextColor #000000\r\
AutoComplete_ListSelBkColor #C4DCFF\r\
AutoComplete_Indent \"  \"\r"

#define TXT_MACCLASSIC_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #000000\r\
LineBoard_BkColor #ECECEC\r\
LineBoard_BorderColor #FFFFFF\r\
LineBoard_BookmarkTextColor #FFFFFF\r\
LineBoard_BookmarkBkColor #7C7C7C\r\
LineBoard_BookmarkBorderColor #ECECEC\r\
LineBoard_LineUnsavedColor #FDA91E\r\
LineBoard_LineSavedColor #04D20F\r\
LineBoard_RulerScaleColor #000000\r\
LineBoard_RulerCaretColor #FDA91E\r"

#define TXT_MACCLASSIC_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #D2D2D2\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #A2A2A2\r\
SpecialChar_SelBkColor 0\r"

#define TXT_MACCLASSIC_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #000000\r\
Sessions_ListBkColor #FFFFFF\r"

#define TXT_MACCLASSIC_VARTHEME (TXT_MACCLASSIC_VARTHEME_BASIC TXT_MACCLASSIC_VARTHEME_HIGHLIGHT TXT_MACCLASSIC_VARTHEME_CODEFOLD TXT_MACCLASSIC_VARTHEME_AUTOCOMPLETE TXT_MACCLASSIC_VARTHEME_LINEBOARD TXT_MACCLASSIC_VARTHEME_SPECIALCHAR TXT_MACCLASSIC_VARTHEME_SESSIONS)

//Monokai variable theme
#define TXT_MONOKAI_VARTHEME_BASIC \
L"STR #E6DB74\r\
COMM #75715E\r\
VAR #A6E22E\r\
DEL1 #F8F8F2\r\
DEL2 #F8F8F2\r\
TYPE #FD971F\r\
OP #66D9EF\r\
TAG #F92672\r\
ATTR #A6E22E\r\
IF #F92672\r\
AREA #75715E\r\
NUM #AE81FF\r"

#define TXT_MONOKAI_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #F8F8F2\r\
HighLight_BasicBkColor #272822\r\
HighLight_SelTextColor #F8F8F2\r\
HighLight_SelBkColor #49483E\r\
HighLight_LineTextColor #F8F8F2\r\
HighLight_LineBkColor #3E3D32\r\
HighLight_LineBorderColor #434238\r\
HighLight_AltTextColor #F8F8F2\r\
HighLight_AltBkColor #252620\r\
HighLight_AltBorderColor #22231F\r\
HighLight_ColumnColor #8F908A\r\
HighLight_MarkerColor #8F908A\r\
HighLight_CaretColor #FFFFFF\r\
HighLight_UrlColor #65B8EF\r\
HighLight_ActiveUrlColor #9BDEF4\r\
HighLight_VisitUrlColor #9BDEF4\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #F8F8F2\r\
HighLight_AutoMarkBkColor #F92672\r"

#define TXT_MONOKAI_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #272822\r\
CodeFold_PanelSecondBkColor #272822\r\
CodeFold_PanelNormalFoldColor #8F908A\r\
CodeFold_PanelActiveFoldColor #F8F8F2\r\
CodeFold_PanelNormalNodeOpenBkColor #272822\r\
CodeFold_PanelNormalNodeCloseBkColor #8F908A\r\
CodeFold_PanelActiveNodeOpenBkColor #272822\r\
CodeFold_PanelActiveNodeCloseBkColor #F8F8F2\r\
CodeFold_PanelNormalNodeOpenSignColor #8F908A\r\
CodeFold_PanelNormalNodeCloseSignColor #272822\r\
CodeFold_PanelActiveNodeOpenSignColor #F8F8F2\r\
CodeFold_PanelActiveNodeCloseSignColor #272822\r\
CodeFold_ListTextColor #F8F8F2\r\
CodeFold_ListBkColor #272822\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #F8F8F2\r\
CodeFold_TagMarkBkColor #75715E\r"

#define TXT_MONOKAI_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #F8F8F2\r\
AutoComplete_ListBasicBkColor #272822\r\
AutoComplete_ListSelTextColor #F8F8F2\r\
AutoComplete_ListSelBkColor #49483E\r\
AutoComplete_Indent \"  \"\r"

#define TXT_MONOKAI_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #F8F8F2\r\
LineBoard_BkColor #3E3D32\r\
LineBoard_BorderColor #272822\r\
LineBoard_BookmarkTextColor #272822\r\
LineBoard_BookmarkBkColor #F8F8F2\r\
LineBoard_BookmarkBorderColor #3E3D32\r\
LineBoard_LineUnsavedColor #FD971F\r\
LineBoard_LineSavedColor #A6E22E\r\
LineBoard_RulerScaleColor #F8F8F2\r\
LineBoard_RulerCaretColor #FD971F\r"

#define TXT_MONOKAI_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #535353\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #747474\r\
SpecialChar_SelBkColor 0\r"

#define TXT_MONOKAI_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #F8F8F2\r\
Sessions_ListBkColor #272822\r"

#define TXT_MONOKAI_VARTHEME (TXT_MONOKAI_VARTHEME_BASIC TXT_MONOKAI_VARTHEME_HIGHLIGHT TXT_MONOKAI_VARTHEME_CODEFOLD TXT_MONOKAI_VARTHEME_AUTOCOMPLETE TXT_MONOKAI_VARTHEME_LINEBOARD TXT_MONOKAI_VARTHEME_SPECIALCHAR TXT_MONOKAI_VARTHEME_SESSIONS)

//Obsidian variable theme
#define TXT_OBSIDIAN_VARTHEME_BASIC \
L"STR #EC7600\r\
COMM #7D8C93\r\
VAR #8C8CB4\r\
DEL1 #E8E2B7\r\
DEL2 #E8E2B7\r\
TYPE #D75028\r\
OP #93C763\r\
TAG #8CBBAD\r\
ATTR #678CB1\r\
IF #A082BD\r\
AREA #99A38A\r\
NUM #FFCD22\r"

#define TXT_OBSIDIAN_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #E0E2E4\r\
HighLight_BasicBkColor #293134\r\
HighLight_SelTextColor #FFFFFF\r\
HighLight_SelBkColor #4B595C\r\
HighLight_LineTextColor #E0E2E4\r\
HighLight_LineBkColor #2E383B\r\
HighLight_LineBorderColor #424C50\r\
HighLight_AltTextColor #E0E2E4\r\
HighLight_AltBkColor #2A3336\r\
HighLight_AltBorderColor #262D2F\r\
HighLight_ColumnColor #7F878B\r\
HighLight_MarkerColor #467984\r\
HighLight_CaretColor #FFD155\r\
HighLight_UrlColor #66C6FF\r\
HighLight_ActiveUrlColor #A8DEFF\r\
HighLight_VisitUrlColor #A8DEFF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #FFFFFF\r\
HighLight_AutoMarkBkColor #963A46\r"

#define TXT_OBSIDIAN_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #2E373A\r\
CodeFold_PanelSecondBkColor #2E373A\r\
CodeFold_PanelNormalFoldColor #106678\r\
CodeFold_PanelActiveFoldColor #A1C2C9\r\
CodeFold_PanelNormalNodeOpenBkColor #2E373A\r\
CodeFold_PanelNormalNodeCloseBkColor #106678\r\
CodeFold_PanelActiveNodeOpenBkColor #2E373A\r\
CodeFold_PanelActiveNodeCloseBkColor #A1C2C9\r\
CodeFold_PanelNormalNodeOpenSignColor #106678\r\
CodeFold_PanelNormalNodeCloseSignColor #76A7B1\r\
CodeFold_PanelActiveNodeOpenSignColor #A1C2C9\r\
CodeFold_PanelActiveNodeCloseSignColor #2E373A\r\
CodeFold_ListTextColor #E0E2E4\r\
CodeFold_ListBkColor #293134\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #E0E2E4\r\
CodeFold_TagMarkBkColor #37625E\r"

#define TXT_OBSIDIAN_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #E0E2E4\r\
AutoComplete_ListBasicBkColor #293134\r\
AutoComplete_ListSelTextColor #FFFFFF\r\
AutoComplete_ListSelBkColor #4B595C\r\
AutoComplete_Indent \"  \"\r"

#define TXT_OBSIDIAN_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #C0C9CB\r\
LineBoard_BkColor #4E595E\r\
LineBoard_BorderColor #4E595E\r\
LineBoard_BookmarkTextColor #222222\r\
LineBoard_BookmarkBkColor #D3D155\r\
LineBoard_BookmarkBorderColor #EDECBA\r\
LineBoard_LineUnsavedColor #FFB13E\r\
LineBoard_LineSavedColor #AEDB5B\r\
LineBoard_RulerScaleColor #9FABAF\r\
LineBoard_RulerCaretColor #FF3515\r"

#define TXT_OBSIDIAN_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #4E595E\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #737C80\r\
SpecialChar_SelBkColor 0\r"

#define TXT_OBSIDIAN_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #E0E2E4\r\
Sessions_ListBkColor #293134\r"

#define TXT_OBSIDIAN_VARTHEME (TXT_OBSIDIAN_VARTHEME_BASIC TXT_OBSIDIAN_VARTHEME_HIGHLIGHT TXT_OBSIDIAN_VARTHEME_CODEFOLD TXT_OBSIDIAN_VARTHEME_AUTOCOMPLETE TXT_OBSIDIAN_VARTHEME_LINEBOARD TXT_OBSIDIAN_VARTHEME_SPECIALCHAR TXT_OBSIDIAN_VARTHEME_SESSIONS)

//Solarized Light variable theme
#define TXT_SOLARIZEDLIGHT_VARTHEME_BASIC \
L"STR #2AA198\r\
COMM #839496\r\
VAR #849900\r\
DEL1 #839496\r\
DEL2 #6C71C4\r\
TYPE #CB4B16\r\
OP #B58900\r\
TAG #268BD2\r\
ATTR #7D9394\r\
IF #849900\r\
AREA #2AA198\r\
NUM #D33682\r"

#define TXT_SOLARIZEDLIGHT_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #586E75\r\
HighLight_BasicBkColor #FDF6E3\r\
HighLight_SelTextColor #586E75\r\
HighLight_SelBkColor #FFFFFF\r\
HighLight_LineTextColor #586E75\r\
HighLight_LineBkColor #EEE8D5\r\
HighLight_LineBorderColor #EEEAD5\r\
HighLight_AltTextColor #586E75\r\
HighLight_AltBkColor #FEF9E9\r\
HighLight_AltBorderColor #FDF2DB\r\
HighLight_ColumnColor #839496\r\
HighLight_MarkerColor #839496\r\
HighLight_CaretColor #000000\r\
HighLight_UrlColor #06A2FF\r\
HighLight_ActiveUrlColor #0087EA\r\
HighLight_VisitUrlColor #0087EA\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #FFFFFF\r\
HighLight_AutoMarkBkColor #268BD2\r"

#define TXT_SOLARIZEDLIGHT_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #EEE8D5\r\
CodeFold_PanelSecondBkColor #EEE8D5\r\
CodeFold_PanelNormalFoldColor #657B83\r\
CodeFold_PanelActiveFoldColor #93A1A1\r\
CodeFold_PanelNormalNodeOpenBkColor #EEE8D5\r\
CodeFold_PanelNormalNodeCloseBkColor #657B83\r\
CodeFold_PanelActiveNodeOpenBkColor #EEE8D5\r\
CodeFold_PanelActiveNodeCloseBkColor #93A1A1\r\
CodeFold_PanelNormalNodeOpenSignColor #657B83\r\
CodeFold_PanelNormalNodeCloseSignColor #EEE8D5\r\
CodeFold_PanelActiveNodeOpenSignColor #93A1A1\r\
CodeFold_PanelActiveNodeCloseSignColor #EEE8D5\r\
CodeFold_ListTextColor #586E75\r\
CodeFold_ListBkColor #FDF6E3\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #FFFFFF\r\
CodeFold_TagMarkBkColor #ABC9D4\r"

#define TXT_SOLARIZEDLIGHT_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #586E75\r\
AutoComplete_ListBasicBkColor #FDF6E3\r\
AutoComplete_ListSelTextColor #586E75\r\
AutoComplete_ListSelBkColor #FFFFFF\r\
AutoComplete_Indent \"  \"\r"

#define TXT_SOLARIZEDLIGHT_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #586E75\r\
LineBoard_BkColor #EEE8D5\r\
LineBoard_BorderColor #FDF6E3\r\
LineBoard_BookmarkTextColor #FDF6E3\r\
LineBoard_BookmarkBkColor #586E75\r\
LineBoard_BookmarkBorderColor #EEE8D5\r\
LineBoard_LineUnsavedColor #E3A420\r\
LineBoard_LineSavedColor #99BF00\r\
LineBoard_RulerScaleColor #586E75\r\
LineBoard_RulerCaretColor #E3A420\r"

#define TXT_SOLARIZEDLIGHT_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #D2D2D2\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #BBBBBB\r\
SpecialChar_SelBkColor 0\r"

#define TXT_SOLARIZEDLIGHT_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #586E75\r\
Sessions_ListBkColor #FDF6E3\r"

#define TXT_SOLARIZEDLIGHT_VARTHEME (TXT_SOLARIZEDLIGHT_VARTHEME_BASIC TXT_SOLARIZEDLIGHT_VARTHEME_HIGHLIGHT TXT_SOLARIZEDLIGHT_VARTHEME_CODEFOLD TXT_SOLARIZEDLIGHT_VARTHEME_AUTOCOMPLETE TXT_SOLARIZEDLIGHT_VARTHEME_LINEBOARD TXT_SOLARIZEDLIGHT_VARTHEME_SPECIALCHAR TXT_SOLARIZEDLIGHT_VARTHEME_SESSIONS)

//Solarized Dark variable theme
#define TXT_SOLARIZEDDARK_VARTHEME_BASIC \
L"STR #2AA198\r\
COMM #586E75\r\
VAR #849900\r\
DEL1 #839496\r\
DEL2 #839496\r\
TYPE #CB4B16\r\
OP #B58900\r\
TAG #268BD2\r\
ATTR #7D9394\r\
IF #849900\r\
AREA #2AA198\r\
NUM #D33682\r"

#define TXT_SOLARIZEDDARK_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #839496\r\
HighLight_BasicBkColor #002B36\r\
HighLight_SelTextColor #FFFFFF\r\
HighLight_SelBkColor #586E75\r\
HighLight_LineTextColor #93A1A1\r\
HighLight_LineBkColor #07404B\r\
HighLight_LineBorderColor #074552\r\
HighLight_AltTextColor #839496\r\
HighLight_AltBkColor #002833\r\
HighLight_AltBorderColor #002631\r\
HighLight_ColumnColor #839496\r\
HighLight_MarkerColor #839496\r\
HighLight_CaretColor #91EAB9\r\
HighLight_UrlColor #06A2FF\r\
HighLight_ActiveUrlColor #75C1FF\r\
HighLight_VisitUrlColor #75C1FF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #FFFFFF\r\
HighLight_AutoMarkBkColor #DC322F\r"

#define TXT_SOLARIZEDDARK_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #053542\r\
CodeFold_PanelSecondBkColor #053542\r\
CodeFold_PanelNormalFoldColor #657B83\r\
CodeFold_PanelActiveFoldColor #93A1A1\r\
CodeFold_PanelNormalNodeOpenBkColor #053542\r\
CodeFold_PanelNormalNodeCloseBkColor #657B83\r\
CodeFold_PanelActiveNodeOpenBkColor #053542\r\
CodeFold_PanelActiveNodeCloseBkColor #93A1A1\r\
CodeFold_PanelNormalNodeOpenSignColor #657B83\r\
CodeFold_PanelNormalNodeCloseSignColor #053542\r\
CodeFold_PanelActiveNodeOpenSignColor #93A1A1\r\
CodeFold_PanelActiveNodeCloseSignColor #053542\r\
CodeFold_ListTextColor #839496\r\
CodeFold_ListBkColor #002B36\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #FFFFFF\r\
CodeFold_TagMarkBkColor #105A5D\r"

#define TXT_SOLARIZEDDARK_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #839496\r\
AutoComplete_ListBasicBkColor #002B36\r\
AutoComplete_ListSelTextColor #FFFFFF\r\
AutoComplete_ListSelBkColor #586E75\r\
AutoComplete_Indent \"  \"\r"

#define TXT_SOLARIZEDDARK_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #839496\r\
LineBoard_BkColor #07404B\r\
LineBoard_BorderColor #002B36\r\
LineBoard_BookmarkTextColor #002B36\r\
LineBoard_BookmarkBkColor #839496\r\
LineBoard_BookmarkBorderColor #07404B\r\
LineBoard_LineUnsavedColor #AC4113\r\
LineBoard_LineSavedColor #849900\r\
LineBoard_RulerScaleColor #839496\r\
LineBoard_RulerCaretColor #AC4113\r"

#define TXT_SOLARIZEDDARK_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #575757\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #A8A8A8\r\
SpecialChar_SelBkColor 0\r"

#define TXT_SOLARIZEDDARK_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #839496\r\
Sessions_ListBkColor #002B36\r"

#define TXT_SOLARIZEDDARK_VARTHEME (TXT_SOLARIZEDDARK_VARTHEME_BASIC TXT_SOLARIZEDDARK_VARTHEME_HIGHLIGHT TXT_SOLARIZEDDARK_VARTHEME_CODEFOLD TXT_SOLARIZEDDARK_VARTHEME_AUTOCOMPLETE TXT_SOLARIZEDDARK_VARTHEME_LINEBOARD TXT_SOLARIZEDDARK_VARTHEME_SPECIALCHAR TXT_SOLARIZEDDARK_VARTHEME_SESSIONS)

//SpaceCadet variable theme
#define TXT_SPACECADET_VARTHEME_BASIC \
L"STR #805978\r\
COMM #564954\r\
VAR #7587A6\r\
DEL1 #DDE6CF\r\
DEL2 #E8EEDF\r\
TYPE #A86D59\r\
OP #CDA869\r\
TAG #6078BF\r\
ATTR #6084BF\r\
IF #728059\r\
AREA #596380\r\
NUM #A8885A\r"

#define TXT_SPACECADET_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #DDE6CF\r\
HighLight_BasicBkColor #0D0D0D\r\
HighLight_SelTextColor #DDE6CF\r\
HighLight_SelBkColor #40002F\r\
HighLight_LineTextColor #FFFFFF\r\
HighLight_LineBkColor #191919\r\
HighLight_LineBorderColor #1E1E1E\r\
HighLight_AltTextColor #DDE6CF\r\
HighLight_AltBkColor #0F0F0F\r\
HighLight_AltBorderColor #0A0A0A\r\
HighLight_ColumnColor #75796E\r\
HighLight_MarkerColor #75796E\r\
HighLight_CaretColor #FFFFFF\r\
HighLight_UrlColor #769FBE\r\
HighLight_ActiveUrlColor #95BFDB\r\
HighLight_VisitUrlColor #95BFDB\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #DDE6CF\r\
HighLight_AutoMarkBkColor #72349E\r"

#define TXT_SPACECADET_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #0D0D0D\r\
CodeFold_PanelSecondBkColor #0D0D0D\r\
CodeFold_PanelNormalFoldColor #75796E\r\
CodeFold_PanelActiveFoldColor #DDE6CF\r\
CodeFold_PanelNormalNodeOpenBkColor #0D0D0D\r\
CodeFold_PanelNormalNodeCloseBkColor #75796E\r\
CodeFold_PanelActiveNodeOpenBkColor #0D0D0D\r\
CodeFold_PanelActiveNodeCloseBkColor #DDE6CF\r\
CodeFold_PanelNormalNodeOpenSignColor #75796E\r\
CodeFold_PanelNormalNodeCloseSignColor #0D0D0D\r\
CodeFold_PanelActiveNodeOpenSignColor #DDE6CF\r\
CodeFold_PanelActiveNodeCloseSignColor #0D0D0D\r\
CodeFold_ListTextColor #DDE6CF\r\
CodeFold_ListBkColor #0D0D0D\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #FFFFFF\r\
CodeFold_TagMarkBkColor #003040\r"

#define TXT_SPACECADET_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #DDE6CF\r\
AutoComplete_ListBasicBkColor #0D0D0D\r\
AutoComplete_ListSelTextColor #DDE6CF\r\
AutoComplete_ListSelBkColor #40002F\r\
AutoComplete_Indent \"  \"\r"

#define TXT_SPACECADET_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #DDE6CF\r\
LineBoard_BkColor #191919\r\
LineBoard_BorderColor #0D0D0D\r\
LineBoard_BookmarkTextColor #0D0D0D\r\
LineBoard_BookmarkBkColor #DDE6CF\r\
LineBoard_BookmarkBorderColor #191919\r\
LineBoard_LineUnsavedColor #DE9B58\r\
LineBoard_LineSavedColor #A2D95E\r\
LineBoard_RulerScaleColor #DDE6CF\r\
LineBoard_RulerCaretColor #DE9B58\r"

#define TXT_SPACECADET_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #444444\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #585858\r\
SpecialChar_SelBkColor 0\r"

#define TXT_SPACECADET_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #DDE6CF\r\
Sessions_ListBkColor #0D0D0D\r"

#define TXT_SPACECADET_VARTHEME (TXT_SPACECADET_VARTHEME_BASIC TXT_SPACECADET_VARTHEME_HIGHLIGHT TXT_SPACECADET_VARTHEME_CODEFOLD TXT_SPACECADET_VARTHEME_AUTOCOMPLETE TXT_SPACECADET_VARTHEME_LINEBOARD TXT_SPACECADET_VARTHEME_SPECIALCHAR TXT_SPACECADET_VARTHEME_SESSIONS)

//Sunburst variable theme
#define TXT_SUNBURST_VARTHEME_BASIC \
L"STR #99CF50\r\
COMM #AEAEAE\r\
VAR #69A3E9\r\
DEL1 #AC885B\r\
DEL2 #E28964\r\
TYPE #9B859D\r\
OP #CE8964\r\
TAG #89BDFF\r\
ATTR #E0C589\r\
IF #65B042\r\
AREA #8FAFDF\r\
NUM #0080FF\r"

#define TXT_SUNBURST_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #F8F8F8\r\
HighLight_BasicBkColor #060606\r\
HighLight_SelTextColor #F8F8F8\r\
HighLight_SelBkColor #2C3033\r\
HighLight_LineTextColor #F8F8F8\r\
HighLight_LineBkColor #16181B\r\
HighLight_LineBorderColor #1A1E20\r\
HighLight_AltTextColor #F8F8F8\r\
HighLight_AltBkColor #090909\r\
HighLight_AltBorderColor #040404\r\
HighLight_ColumnColor #7C7C7C\r\
HighLight_MarkerColor #7C7C7C\r\
HighLight_CaretColor #FFFFFF\r\
HighLight_UrlColor #35AEFF\r\
HighLight_ActiveUrlColor #84F0FF\r\
HighLight_VisitUrlColor #84F0FF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #F8F8F8\r\
HighLight_AutoMarkBkColor #CF6A4C\r"

#define TXT_SUNBURST_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #060606\r\
CodeFold_PanelSecondBkColor #060606\r\
CodeFold_PanelNormalFoldColor #7C7C7C\r\
CodeFold_PanelActiveFoldColor #F8F8F8\r\
CodeFold_PanelNormalNodeOpenBkColor #060606\r\
CodeFold_PanelNormalNodeCloseBkColor #7C7C7C\r\
CodeFold_PanelActiveNodeOpenBkColor #060606\r\
CodeFold_PanelActiveNodeCloseBkColor #F8F8F8\r\
CodeFold_PanelNormalNodeOpenSignColor #7C7C7C\r\
CodeFold_PanelNormalNodeCloseSignColor #060606\r\
CodeFold_PanelActiveNodeOpenSignColor #F8F8F8\r\
CodeFold_PanelActiveNodeCloseSignColor #060606\r\
CodeFold_ListTextColor #F8F8F8\r\
CodeFold_ListBkColor #060606\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #F8F8F8\r\
CodeFold_TagMarkBkColor #3D4B63\r"

#define TXT_SUNBURST_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #F8F8F8\r\
AutoComplete_ListBasicBkColor #060606\r\
AutoComplete_ListSelTextColor #F8F8F8\r\
AutoComplete_ListSelBkColor #2C3033\r\
AutoComplete_Indent \"  \"\r"

#define TXT_SUNBURST_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #F8F8F8\r\
LineBoard_BkColor #16181B\r\
LineBoard_BorderColor #060606\r\
LineBoard_BookmarkTextColor #060606\r\
LineBoard_BookmarkBkColor #F8F8F8\r\
LineBoard_BookmarkBorderColor #16181B\r\
LineBoard_LineUnsavedColor #F7B02D\r\
LineBoard_LineSavedColor #A4E13E\r\
LineBoard_RulerScaleColor #F8F8F8\r\
LineBoard_RulerCaretColor #F7B02D\r"

#define TXT_SUNBURST_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #373737\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #515151\r\
SpecialChar_SelBkColor 0\r"

#define TXT_SUNBURST_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #F8F8F8\r\
Sessions_ListBkColor #060606\r"

#define TXT_SUNBURST_VARTHEME (TXT_SUNBURST_VARTHEME_BASIC TXT_SUNBURST_VARTHEME_HIGHLIGHT TXT_SUNBURST_VARTHEME_CODEFOLD TXT_SUNBURST_VARTHEME_AUTOCOMPLETE TXT_SUNBURST_VARTHEME_LINEBOARD TXT_SUNBURST_VARTHEME_SPECIALCHAR TXT_SUNBURST_VARTHEME_SESSIONS)

//Twilight variable theme
#define TXT_TWILIGHT_VARTHEME_BASIC \
L"STR #8F9D6A\r\
COMM #5F5A60\r\
VAR #7587A6\r\
DEL1 #AC885B\r\
DEL2 #E0C589\r\
TYPE #CF6A4C\r\
OP #CDA869\r\
TAG #E0C589\r\
ATTR #AC885B\r\
IF #CDA869\r\
AREA #A3B5CB\r\
NUM #CA7840\r"

#define TXT_TWILIGHT_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #F8F8F8\r\
HighLight_BasicBkColor #141414\r\
HighLight_SelTextColor #F8F8F8\r\
HighLight_SelBkColor #3C4043\r\
HighLight_LineTextColor #F8F8F8\r\
HighLight_LineBkColor #252327\r\
HighLight_LineBorderColor #2D2428\r\
HighLight_AltTextColor #F8F8F8\r\
HighLight_AltBkColor #121212\r\
HighLight_AltBorderColor #0F0F0F\r\
HighLight_ColumnColor #868686\r\
HighLight_MarkerColor #868686\r\
HighLight_CaretColor #FFFFFF\r\
HighLight_UrlColor #769FBE\r\
HighLight_ActiveUrlColor #A9C2D6\r\
HighLight_VisitUrlColor #A9C2D6\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #F8F8F8\r\
HighLight_AutoMarkBkColor #CF6A4C\r"

#define TXT_TWILIGHT_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #141414\r\
CodeFold_PanelSecondBkColor #141414\r\
CodeFold_PanelNormalFoldColor #868686\r\
CodeFold_PanelActiveFoldColor #F8F8F8\r\
CodeFold_PanelNormalNodeOpenBkColor #141414\r\
CodeFold_PanelNormalNodeCloseBkColor #868686\r\
CodeFold_PanelActiveNodeOpenBkColor #141414\r\
CodeFold_PanelActiveNodeCloseBkColor #F8F8F8\r\
CodeFold_PanelNormalNodeOpenSignColor #868686\r\
CodeFold_PanelNormalNodeCloseSignColor #141414\r\
CodeFold_PanelActiveNodeOpenSignColor #F8F8F8\r\
CodeFold_PanelActiveNodeCloseSignColor #141414\r\
CodeFold_ListTextColor #F8F8F8\r\
CodeFold_ListBkColor #141414\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #F8F8F8\r\
CodeFold_TagMarkBkColor #27303F\r"

#define TXT_TWILIGHT_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #F8F8F8\r\
AutoComplete_ListBasicBkColor #141414\r\
AutoComplete_ListSelTextColor #F8F8F8\r\
AutoComplete_ListSelBkColor #3C4043\r\
AutoComplete_Indent \"  \"\r"

#define TXT_TWILIGHT_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #F8F8F8\r\
LineBoard_BkColor #252327\r\
LineBoard_BorderColor #141414\r\
LineBoard_BookmarkTextColor #141414\r\
LineBoard_BookmarkBkColor #F8F8F8\r\
LineBoard_BookmarkBorderColor #252327\r\
LineBoard_LineUnsavedColor #F8DA72\r\
LineBoard_LineSavedColor #95DA2E\r\
LineBoard_RulerScaleColor #F8F8F8\r\
LineBoard_RulerCaretColor #F8DA72\r"

#define TXT_TWILIGHT_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #464646\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #6F6F6F\r\
SpecialChar_SelBkColor 0\r"

#define TXT_TWILIGHT_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #F8F8F8\r\
Sessions_ListBkColor #141414\r"

#define TXT_TWILIGHT_VARTHEME (TXT_TWILIGHT_VARTHEME_BASIC TXT_TWILIGHT_VARTHEME_HIGHLIGHT TXT_TWILIGHT_VARTHEME_CODEFOLD TXT_TWILIGHT_VARTHEME_AUTOCOMPLETE TXT_TWILIGHT_VARTHEME_LINEBOARD TXT_TWILIGHT_VARTHEME_SPECIALCHAR TXT_TWILIGHT_VARTHEME_SESSIONS)

//Zenburn variable theme
#define TXT_ZENBURN_VARTHEME_BASIC \
L"STR #DCA5A5\r\
COMM #C0C0C0\r\
VAR #E4E49A\r\
DEL1 #ACACED\r\
DEL2 #95B6D4\r\
TYPE #FFAA00\r\
OP #FFC66D\r\
TAG #E4E49A\r\
ATTR #FFCC6C\r\
IF #C0E380\r\
AREA #7D9EC0\r\
NUM #8ACCCF\r"

#define TXT_ZENBURN_VARTHEME_HIGHLIGHT \
L"HighLight_FontStyle 0\r\
HighLight_FontSize 0\r\
HighLight_FaceName \"\"\r\
HighLight_BasicTextColor #FFFFFF\r\
HighLight_BasicBkColor #3F3F3F\r\
HighLight_SelTextColor #DFDFBF\r\
HighLight_SelBkColor #585858\r\
HighLight_LineTextColor #DFDFBF\r\
HighLight_LineBkColor #5D666E\r\
HighLight_LineBorderColor #616972\r\
HighLight_AltTextColor #FFFFFF\r\
HighLight_AltBkColor #3B3B3B\r\
HighLight_AltBorderColor #393939\r\
HighLight_ColumnColor #5D666E\r\
HighLight_MarkerColor #5D666E\r\
HighLight_CaretColor #91EAB9\r\
HighLight_UrlColor #66C6FF\r\
HighLight_ActiveUrlColor #A8DEFF\r\
HighLight_VisitUrlColor #A8DEFF\r\
HighLight_BkImageFile \"\"\r\
HighLight_BkImageAlpha 128\r\
HighLight_AutoMarkFlags 1\r\
HighLight_AutoMarkFontStyle 0\r\
HighLight_AutoMarkTextColor #FFFFFF\r\
HighLight_AutoMarkBkColor #82A8A8\r"

#define TXT_ZENBURN_VARTHEME_CODEFOLD \
L"CodeFold_PanelFirstBkColor #3F3F3F\r\
CodeFold_PanelSecondBkColor #3F3F3F\r\
CodeFold_PanelNormalFoldColor #808080\r\
CodeFold_PanelActiveFoldColor #B9B9B9\r\
CodeFold_PanelNormalNodeOpenBkColor #3F3F3F\r\
CodeFold_PanelNormalNodeCloseBkColor #808080\r\
CodeFold_PanelActiveNodeOpenBkColor #3F3F3F\r\
CodeFold_PanelActiveNodeCloseBkColor #B9B9B9\r\
CodeFold_PanelNormalNodeOpenSignColor #808080\r\
CodeFold_PanelNormalNodeCloseSignColor #3F3F3F\r\
CodeFold_PanelActiveNodeOpenSignColor #B9B9B9\r\
CodeFold_PanelActiveNodeCloseSignColor #3F3F3F\r\
CodeFold_ListTextColor #FFFFFF\r\
CodeFold_ListBkColor #3F3F3F\r\
CodeFold_TagMarkFlags 0\r\
CodeFold_TagMarkFontStyle 1\r\
CodeFold_TagMarkTextColor #FFFFFF\r\
CodeFold_TagMarkBkColor #3F706B\r"

#define TXT_ZENBURN_VARTHEME_AUTOCOMPLETE \
L"AutoComplete_ListFontStyle 0\r\
AutoComplete_ListFontSize 0\r\
AutoComplete_ListFaceName \"\"\r\
AutoComplete_ListLineGap 0\r\
AutoComplete_ListBlockIcon \"\"\r\
AutoComplete_ListBlockIconMargins 0;1\r\
AutoComplete_ListHlBaseIcon \"\"\r\
AutoComplete_ListHlBaseIconMargins 0;1\r\
AutoComplete_ListDocWordIcon \"\"\r\
AutoComplete_ListDocWordIconMargins 0;1\r\
AutoComplete_ListBasicTextColor #FFFFFF\r\
AutoComplete_ListBasicBkColor #3F3F3F\r\
AutoComplete_ListSelTextColor #DFDFBF\r\
AutoComplete_ListSelBkColor #585858\r\
AutoComplete_Indent \"  \"\r"

#define TXT_ZENBURN_VARTHEME_LINEBOARD \
L"LineBoard_TextColor #FFFFFF\r\
LineBoard_BkColor #5D666E\r\
LineBoard_BorderColor #3F3F3F\r\
LineBoard_BookmarkTextColor #3F3F3F\r\
LineBoard_BookmarkBkColor #FFFFFF\r\
LineBoard_BookmarkBorderColor #5D666E\r\
LineBoard_LineUnsavedColor #FFB13E\r\
LineBoard_LineSavedColor #AEDB5B\r\
LineBoard_RulerScaleColor #FFFFFF\r\
LineBoard_RulerCaretColor #FFB13E\r"

#define TXT_ZENBURN_VARTHEME_SPECIALCHAR \
L"SpecialChar_BasicFontStyle 0\r\
SpecialChar_BasicTextColor #6A6A6A\r\
SpecialChar_BasicBkColor 0\r\
SpecialChar_SelFontStyle 0\r\
SpecialChar_SelTextColor #9F9F9F\r\
SpecialChar_SelBkColor 0\r"

#define TXT_ZENBURN_VARTHEME_SESSIONS \
L"Sessions_ListTextColor #FFFFFF\r\
Sessions_ListBkColor #3F3F3F\r"

#define TXT_ZENBURN_VARTHEME (TXT_ZENBURN_VARTHEME_BASIC TXT_ZENBURN_VARTHEME_HIGHLIGHT TXT_ZENBURN_VARTHEME_CODEFOLD TXT_ZENBURN_VARTHEME_AUTOCOMPLETE TXT_ZENBURN_VARTHEME_LINEBOARD TXT_ZENBURN_VARTHEME_SPECIALCHAR TXT_ZENBURN_VARTHEME_SESSIONS)


#endif
