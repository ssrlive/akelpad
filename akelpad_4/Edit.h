#ifndef __EDIT_H__
#define __EDIT_H__


//// Defines

#ifdef _WIN64
  #define APP_ABOUT_VERSIONW        L"AkelPad 4.6.6 (x64)"
#else
  #define APP_ABOUT_VERSIONW        L"AkelPad 4.6.6 (x86)"
#endif
#define APP_MAIN_TITLEW             L"AkelPad"
#define APP_MAIN_CLASSA              "AkelPad4"
#define APP_MAIN_CLASSW             L"AkelPad4"
#define APP_SDI_TITLEW              L""
#define APP_SDI_CLASSW              L"AkelPad SDI Class"
#define APP_MDI_CLASSW              L"AkelPad MDI Class"
#define APP_PRINTPREVIEW_CLASSW     L"AkelPad Print Preview"
#define APP_MUTEXA                   "AkelPad Mutex"
#define APP_REGHOMEW                L"Software\\Akelsoft\\AkelPad"
#define APP_ABOUT_HOMEPAGEW         L"http://akelpad.sf.net"
#define APP_ABOUT_EMAIL_SHENGALTSW  L"shengalts@mail.ru"

//Code pages strings
#define STR_UNICODE_UTF16LEW        L"1200  (UTF-16 LE)"
#define STR_UNICODE_UTF16BEW        L"1201  (UTF-16 BE)"
#define STR_UNICODE_UTF8W           L"65001 (UTF-8)"
#define STR_UNICODE_UTF7W           L"65000 (UTF-7)"
#define STR_UNICODE_UTF32LEW        L"12000  (UTF-32 LE)"
#define STR_UNICODE_UTF32BEW        L"12001  (UTF-32 BE)"
#define STR_NOBOMW                  L"  *BOM"

#define STR_DEFAULTSAVEEXTW         L"txt"
#define STR_ASSOCIATE_OPENW         L"txt;log;ini;inf"
#define STR_ASSOCIATE_EDITW         L"bat;cmd;reg;htm;html"
#define STR_ASSOCIATE_PRINTW        L"txt;log;ini;inf"
#define STR_WORD_DELIMITERSW        L" \t\n'`\"\\|[](){}<>,.;:+-=~!@#$%^&*/?"
#define STR_WRAP_DELIMITERSW        L" \t"
#define STR_URL_LEFTDELIMITERSW     L" \t\n'`\"(<{[="
#define STR_URL_RIGHTDELIMITERSW    L" \t\n'`\")>}]"
#define STR_URL_PREFIXESW           L"http: https: www. ftp: file: mailto:"
#define STR_SENTENCE_DELIMITERSW    L".?!"
#define STR_PRINT_HEADERW           L"%l***%c<%f>%r***"
#define STR_PRINT_FOOTERW           L"- %n[1] -"

#ifndef TCN_GETOBJECT
  #define TCN_GETOBJECT (TCN_FIRST - 3)

  typedef struct tagNMOBJECTNOTIFY {
    NMHDR hdr;
    int iItem;
    const void *piid;
    void *pObject;
    HRESULT hResult;
    DWORD dwFlags;
  } NMOBJECTNOTIFY, *LPNMOBJECTNOTIFY;
#endif
#ifndef INVALID_FILE_ATTRIBUTES
  #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif
#ifndef SS_EDITCONTROL
  #define SS_EDITCONTROL 0x00002000L
#endif
#ifndef SPI_GETSNAPTODEFBUTTON
  #define SPI_GETSNAPTODEFBUTTON 0x005F
#endif
#ifndef BIF_NEWDIALOGSTYLE
  #define BIF_NEWDIALOGSTYLE 0x0040
#endif
#ifndef PROPSHEETHEADERA_V1_SIZE
  #define PROPSHEETHEADERA_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERA, pfnCallback)
#endif
#ifndef PROPSHEETHEADERW_V1_SIZE
  #define PROPSHEETHEADERW_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERW, pfnCallback)
#endif
#ifndef SEE_MASK_NOASYNC
  #define SEE_MASK_NOASYNC 0x00000100
#endif
#ifndef SEE_MASK_NOCLOSEPROCESS
  #define SEE_MASK_NOCLOSEPROCESS 0x00000040
#endif

#ifndef mod
  #define mod(a)  (((a) < 0) ? (0 - (a)) : (a))
#endif

#define BUFFER_SIZE                1024
#define OPENFILELIST_SIZE          32768
#define TRANSLATE_BUFFER_SIZE      8192
#define PREVIEW_SIZE               8188  //4094*2; -1 preview all file
#define PUTFIND_MAXSEL             16384
#define MDI_MAXDOCUMENTS           700
#define DETECT_CODEPAGE_SIZE       1024
#define RECENTFILES_AMOUNT         10
#define RECENTFILES_RECORD_LENGTH  90
#define SEARCHSTRINGS_AMOUNT       10
#define EDIT_TABSTOPS              8
#define EDIT_UNDOLIMIT             1000
#define EDIT_MARGINS               MAKELONG(3, 4)

//ParseCmdLine flags
#define PCL_ONLOAD          0
#define PCL_ONSHOW          1

//Open file dialog
#define IDC_OFN_LIST           1120
#define IDC_OFN_COMBOBOX_LABEL 1089
#define IDC_OFN_COMBOBOX       1136
#define IDC_OFN_EDIT           1152
#define IDC_OFN_PLACESBAR      1184

//Combobox edit ID
#define IDC_COMBOBOXEDIT       1001

//External actions
#define EXTACT_COMMAND    1
#define EXTACT_CALL       2
#define EXTACT_EXEC       3
#define EXTACT_OPENFILE   4
#define EXTACT_SAVEFILE   5
#define EXTACT_FONT       6
#define EXTACT_RECODE     7
#define EXTACT_INSERT     8

//External parameters
#define EXTPARAM_CHAR     1
#define EXTPARAM_INT      2

//Font styles
#define FS_NONE            0
#define FS_FONTNORMAL      1
#define FS_FONTBOLD        2
#define FS_FONTITALIC      3
#define FS_FONTBOLDITALIC  4

//Print preview dialog notifications
#define AKDLG_PREVIEWKEYDOWN           (WM_USER + 100) //lParam - WM_KEYDOWN's lParam, wParam - control handle.
#define AKDLG_PREVIEWMOUSEWHEEL        (WM_USER + 151) //lParam - WM_MOUSEWHEEL's lParam, wParam - control handle.
#define AKDLG_PREVIEWSETZOOM           (WM_USER + 152) //lParam - percentage, wParam - update control immediately.
#define AKDLG_PREVIEWSETPAGE           (WM_USER + 153) //lParam - page number, wParam - not used.

//PrintDocument flags
#define PRND_REALPRINT                 0x001  //Real printing
#define PRND_TEST                      0x002  //Get preview information
#define PRND_ONEPAGE                   0x004  //Print only one page
#define PRND_ALLTEXT                   0x008  //Process all text, prn->crText member input ignored
#define PRND_SELECTION                 0x010  //Process selection, prn->crText member input ignored
#define PRND_RANGE                     0x020  //Process text range pointed with prn->crText member
#define PRND_ANSI                      0x040  //Ansi output

//Print preview zoom
#define PREVIEWZOOM_FIT        -1
#define PREVIEWZOOM_WIDTH      -2

//Print preview scroll
#define PREVIEWSCROLL_OFFSET   -1
#define PREVIEWSCROLL_ABSOLUTE -2

//Print page setup dialog
#define IDC_PSD_PRINTER_BUTTON         1026
#define IDC_PSD_FONT_GROUP             10001
#define IDC_PSD_FONT_CHECK             10002
#define IDC_PSD_FONT_BUTTON            10003
#define IDC_PSD_COLOR_GROUP            10004
#define IDC_PSD_COLOR_TEXT_CHECK       10005
#define IDC_PSD_COLOR_BACKGROUND_CHECK 10006
#define IDC_PSD_HEADLINE_GROUP         10007
#define IDC_PSD_HEADLINE_HEADER_CHECK  10008
#define IDC_PSD_HEADLINE_HEADER_EDIT   10009
#define IDC_PSD_HEADLINE_HEADER_HELP   10010
#define IDC_PSD_HEADLINE_FOOTER_CHECK  10011
#define IDC_PSD_HEADLINE_FOOTER_EDIT   10012
#define IDC_PSD_HEADLINE_FOOTER_HELP   10013

//Color printing
#define PRNC_TEXT                  0x01  //Print colored text.
#define PRNC_BACKGROUND            0x02  //Print on colored background.
#define PRNC_SELECTION             0x04  //Print text selection.

//Language identifiers
#define LANGID_RUSSIAN    0x0419
#define LANGID_ENGLISH    0x0409
#define LANGID_POLISH     0x0415
#define LANGID_GERMAN     0x0407
#define LANGID_TURKISH    0x041F
#define LANGID_CHINESE    0x0404
#define LANGID_JAPANESE   0x0411
#define LANGID_KOREAN     0x0412

//Language detection order index
#define DETECTINDEX_NONE          0
#define DETECTINDEX_RUSSIAN       1
#define DETECTINDEX_EASTERNEUROPE 2
#define DETECTINDEX_WESTERNEUROPE 3
#define DETECTINDEX_TURKISH       4
#define DETECTINDEX_CHINESE       5
#define DETECTINDEX_JAPANESE      6
#define DETECTINDEX_KOREAN        7

//Char layout
#define CHARLAYOUT_NONE       0
#define CHARLAYOUT_ENGLISH    1
#define CHARLAYOUT_NONENGLISH 2

//Code pages int
#define CP_UNICODE_UTF16LE  1200
#define CP_UNICODE_UTF16BE  1201
#define CP_UNICODE_UTF32LE  12000
#define CP_UNICODE_UTF32BE  12001
#define CP_UNICODE_UTF7     65000
#define CP_UNICODE_UTF8     65001
#define CP_KOI8_R           20866

//Captures
#define MSC_PREVIEWMOVE   0x01
#define MSC_SPLITSIZE     0x02
#define MSC_TABDRAGDROP   0x04
#define MSC_DOCKSIZE      0x08
#define MSC_DOCKDRAGDROP  0x10
#define MSC_BUTTONPRESS   0x20
#define MSC_BUTTONOVER    0x40

//Sides
#define SIDE_LEFT    0x00000001
#define SIDE_RIGHT   0x00000002
#define SIDE_TOP     0x00000004
#define SIDE_BOTTOM  0x00000008

//Clone window
#define CN_EDIT    0x00000001
#define CN_MASTER  0x00000002
#define CN_CLONE1  0x00000004
#define CN_CLONE2  0x00000008
#define CN_CLONE3  0x00000010
#define CN_ALL     (CN_EDIT   |\
                    CN_MASTER |\
                    CN_CLONE1 |\
                    CN_CLONE2 |\
                    CN_CLONE3)

//ResizeEditWindow flags
#define REW_TEST      0x00000001
#define REW_NOREDRAW  0x00000002

//STARTUPINFO flags
#define STARTF_NOMUTEX  0x00001000

//Main option type
#define MOT_DWORD       0x01  //32-bit number.
#define MOT_BINARY      0x02  //Binary data in any form.
#define MOT_STRING      0x04  //Null-terminated string.
#define MOT_MAINOFFSET  0x10  //lpData specifies offset of MAINOPTIONS structure.
#define MOT_FRAMEOFFSET 0x20  //lpData specifies offset of FRAMEDATA structure.
#define MOT_MANUAL      0x40  //lpData specifies manual parameter.

//Search options
//#define AEFR_DOWN               0x00000001
//#define AEFR_WHOLEWORD          0x00000002
//#define AEFR_MATCHCASE          0x00000004
#define AEFR_UP                 0x00100000
#define AEFR_BEGINNING          0x00200000
#define AEFR_SELECTION          0x00400000
#define AEFR_ESCAPESEQ          0x00800000
#define AEFR_ALLFILES           0x01000000

//Find/Replace dialog options
#define AEFR_REPLACEALLANDCLOSE 0x02000000
#define AEFR_CHECKINSELIFSEL    0x04000000
#define AEFR_CYCLESEARCH        0x08000000

//StrReplace options
#define AEFR_WHOLEWORDGOODSTART 0x10000000
#define AEFR_WHOLEWORDGOODEND   0x20000000

//Find/Replace dialog message
#define AKDLG_PUTFIND           (WM_USER + 1)

//Recode dialog message
#define AKDLG_RECODEUPDATE      (WM_USER + 1)

//Insert/Delete char in selection
#define STRSEL_CHECK   0x00000001
#define STRSEL_INSERT  0x00000002
#define STRSEL_DELETE  0x00000004
#define STRSEL_TAB     0x00000008
#define STRSEL_SPACE   0x00000010

//Selection case
#define UPPERCASE      1
#define LOWERCASE      2
#define SENTENCECASE   3
#define TITLECASE      4
#define INVERTCASE     5

//Change font size
#define FONTSIZE_INCREASE  1
#define FONTSIZE_DECREASE  2
#define FONTSIZE_RESTORE   3

//BUTTONMESSAGEBOX flags
#define BMB_DEFAULT   0x001  //Default button.
#define BMB_DISABLED  0x002  //Button is disabled.

//SaveChanged flags
#define PROMPT_NONE           0x001  //Don't prompt, answering "No".
#define PROMPT_NOTOALLBUTTON  0x002  //Show "No to all" button if necessary.

//Show modify
#define SM_NONE           0x00000000
#define SM_STATUSBAR      0x00000001
#define SM_MAINTITLE_SDI  0x00000002
#define SM_FRAMETITLE_MDI 0x00000004
#define SM_TABTITLE_MDI   0x00000008

//Custom statusbar
#define CSB_CHARHEX           0x0001
#define CSB_CHARDEC           0x0002
#define CSB_CHARLETTER        0x0004
#define CSB_RICHOFFSET        0x0008
#define CSB_BYTEOFFSET        0x0010
#define CSB_LINECOUNT         0x0020
#define CSB_RICHCOUNT         0x0040
#define CSB_FONTPOINT         0x0080
#define CSB_TABSIZE           0x0100
#define CSB_MARKER            0x0200
#define CSB_REPLACECOUNT      0x0400
#define CSB_SEARCHENDED       0x0800
#define CSB_DOCUMENTSCOUNT    0x1000
#define CSB_DOCUMENTSMODIFIED 0x2000
#define CSB_DOCUMENTSSAVED    0x4000
#define CSB_DOCUMENTINDEX     0x8000

//File types association
#define AE_ASSOCIATE     0x00000001
#define AE_DEASSOCIATE   0x00000002
#define AE_OPEN          0x00000004
#define AE_EDIT          0x00000008
#define AE_PRINT         0x00000010

//Colors list
#define LVI_COLOR_BASIC         0
#define LVI_COLOR_SELECTION     1
#define LVI_COLOR_ACTIVELINE    2
#define LVI_COLOR_ACTIVECOLUMN  3
#define LVI_COLOR_COLUMNMARKER  4
#define LVI_COLOR_CARET         5
#define LVI_COLOR_URL           6
#define LVI_COLOR_ACTIVEURL     7

#define LVSI_COLOR_ELEMENT     0
#define LVSI_COLOR_TEXT        1
#define LVSI_COLOR_BACKGROUND  2
#define LVSI_COLOR_SAMPLE      3

//Plugins list
#define LVSI_FUNCTION_NAME     0
#define LVSI_FUNCTION_HOTKEY   1
#define LVSI_FUNCTION_STATUS   2

//Mdi list options
#define MLO_ONLYNAMES      0x1

//Mdi list modify filter
#define MLMF_ALL            0
#define MLMF_ONLYMODIFIED   1
#define MLMF_ONLYUNMODIFIED 2


//// Structures

typedef BOOL (CALLBACK *EXPORTNAMESPROC)(char *, LPARAM);

typedef struct {
  //Save place
  int nSaveSettings;

  //Manual
  DWORD dwShowModify;
  DWORD dwStatusPosType;
  wchar_t wszStatusUserFormat[MAX_PATH];
  DWORD dwStatusUserFlags;
  DWORD dwWordBreakCustom;
  DWORD dwPaintOptions;
  BOOL bRichEditClass;
  wchar_t wszDateLogFormat[128];
  wchar_t wszDateInsertFormat[128];

  //Menu settings
  BOOL bOnTop;
  BOOL bStatusBar;
  char szLangModule[MAX_PATH];
  wchar_t wszLangModule[MAX_PATH];
  BOOL bKeepSpace;
  BOOL bWatchFile;
  BOOL bSaveTime;
  BOOL bSingleOpenFile;
  BOOL bSingleOpenProgram;
  BOOL nMDI;
  DWORD dwTabOptionsMDI;

  //Settings dialog
  wchar_t wszCommand[BUFFER_SIZE];
  wchar_t wszWorkDir[MAX_PATH];
  int nDefaultCodePage;
  int bDefaultBOM;
  int nDefaultNewLine;
  DWORD dwLangCodepageRecognition;
  DWORD dwCodepageRecognitionBuffer;
  BOOL bSavePositions;
  BOOL bSaveCodepages;
  int nRecentFiles;
  int nSearchStrings;
  wchar_t wszFileTypesOpen[MAX_PATH];
  wchar_t wszFileTypesEdit[MAX_PATH];
  wchar_t wszFileTypesPrint[MAX_PATH];
  DWORD dwFileTypesAssociated;
  int nClickURL;
  DWORD dwKeybLayoutOptions;
  BOOL bSilentCloseEmptyMDI;
  BOOL bDateLog;
  BOOL bSaveInReadOnlyMsg;
  wchar_t wszDefaultSaveExt[MAX_PATH];

  //Search dialog
  DWORD dwSearchOptions;

  //Open file dialog
  wchar_t wszLastDir[MAX_PATH];
  BOOL bShowPlacesBar;

  //Recode dialog
  RECT rcRecodeCurrentDialog;

  //Print dialog
  RECT rcPrintMargins;
  DWORD dwPrintColor;
  BOOL bPrintHeaderEnable;
  wchar_t wszPrintHeader[MAX_PATH];
  BOOL bPrintFooterEnable;
  wchar_t wszPrintFooter[MAX_PATH];
  BOOL bPrintFontEnable;
  LOGFONTW lfPrintFont;

  //Colors dialog
  RECT rcColorsCurrentDialog;

  //Plugin dialog
  RECT rcPluginsCurrentDialog;

  //Mdi list dialog
  DWORD dwMdiListOptions;
  RECT rcMdiListCurrentDialog;

  //Main window
  DWORD dwMainStyle;
  RECT rcMainWindowRestored;
  DWORD dwMdiStyle;
} MAINOPTIONS;

typedef struct {
  HANDLE hHandle;
  MAINOPTIONS *mo;
  FRAMEDATA *fd;
  int nSaveSettings;
  BOOL bForceWrite;
} OPTIONHANDLE;

typedef struct _EXTPARAM {
  struct _EXTPARAM *next;
  struct _EXTPARAM *prev;
  DWORD dwType;        //See EXTPARAM_* defines.
  int nNumber;         //External parameter number.
  char *pString;       //External parameter string (Ansi).
  wchar_t *wpString;   //External parameter string (Unicode).
  char *pExpanded;     //External parameter expanded string - without %variables% (Ansi).
  wchar_t *wpExpanded; //External parameter expanded string - without %variables% (Unicode).
} EXTPARAM;

typedef struct {
  EXTPARAM *first;
  EXTPARAM *last;
  int nElements;
} STACKEXTPARAM;

typedef struct _INIKEY {
  struct _INIKEY *next;
  struct _INIKEY *prev;
  wchar_t *wszKey;
  int nKeyBytes;
  wchar_t *wszString;
  int nStringBytes;
} INIKEY;

typedef struct _INISECTION {
  struct _INISECTION *next;
  struct _INISECTION *prev;
  HANDLE hIniFile;
  wchar_t *wszSection;
  int nSectionBytes;
  INIKEY *first;
  INIKEY *last;
} INISECTION;

typedef struct {
  INISECTION *first;
  INISECTION *last;
  BOOL bModified;
} INIFILE;

typedef struct _FILESTREAMDATA {
  HWND hWnd;
  HANDLE hFile;
  int nCodePage;
  DWORD dwFlags;
  int nNewLine;
  UINT_PTR dwBytesCurrent;
  UINT_PTR dwBytesMax;
  BOOL bResult;
} FILESTREAMDATA;

typedef struct _PRINTPAGE {
  struct _PRINTPAGE *next;
  struct _PRINTPAGE *prev;
  AECHARRANGE crText;
} PRINTPAGE;

typedef struct _COLORTHEME {
  struct _COLORTHEME *next;
  struct _COLORTHEME *prev;
  wchar_t wszName[MAX_PATH];
  int nNameLen;
  AECOLORS aec;
} COLORTHEME;

typedef struct _PLUGINHANDLE {
  struct _PLUGINHANDLE *next;
  struct _PLUGINHANDLE *prev;
  HMODULE hModule;
  int nCount;
} PLUGINHANDLE;

typedef struct _PLUGINLISTDATA {
  HWND hWndList;
  unsigned char *pBaseName;
} PLUGINLISTDATA;

typedef struct _PLUGINLISTITEM {
  struct _PLUGINLISTITEM *next;
  struct _PLUGINLISTITEM *prev;
  PLUGINFUNCTION *pf;
  WORD wInitialHotkey;
  BOOL bInitialOnStart;
  int nAutoLoad;
  int nCallResult;
} PLUGINLISTITEM;

typedef struct _REGHANDLE {
  DWORD dwType;
  HKEY hKey;
  wchar_t wszKey[MAX_PATH];
} REGHANDLE;

typedef struct _INIHANDLE {
  DWORD dwType;
  INIFILE hIniFile;
  wchar_t wszIniFile[MAX_PATH];
} INIHANDLE;

typedef struct {
  HWND hWndToFind;
  HWND hWndResult;
  HWND hWndNextResult;
  HWND hWndNextNextResult;
  BOOL bFound;
  BOOL bNextNext;
  int nCount;
} ENUMDLG;

typedef struct _HDOCK {
  HSTACK hStack;
  BOOL bSizing;
  int nSizingSide;
  int nSizingType;
} HDOCK;

typedef struct _BUTTONDRAWITEM {
  struct _BUTTONDRAWITEM *next;
  struct _BUTTONDRAWITEM *prev;
  HWND hWnd;
  WNDPROC OldButtonProc;
  BUTTONDRAW bd;
} BUTTONDRAWITEM;

typedef struct _FONTITEM {
  struct _FONTITEM *next;
  struct _FONTITEM *prev;
  LOGFONTW lfFont;
  HFONT hFont;
} FONTITEM;

typedef struct _ASSOCICON {
  struct _ASSOCICON *next;
  struct _ASSOCICON *prev;
  wchar_t wszFile[MAX_PATH];
  int nFileLen;
  const wchar_t *wpExt;
  HICON hIcon;
  int nIconIndex;
} ASSOCICON;

typedef struct {
  ASSOCICON *first;
  ASSOCICON *last;
  int nValidIcons;
} STACKASSOCICON;

typedef struct {
  HANDLE hThread;
  HDROP hDrop;
} DROPFILESTHREAD;

typedef struct {
  int nCodePage;
  BOOL bBOM;
  BOOL bResult;
} DIALOGCODEPAGE;

//OPENFILENAMEW with _WIN32_WINNT >= 0x0500 members
typedef struct {
  DWORD lStructSize;
  HWND hwndOwner;
  HINSTANCE hInstance;
  LPCWSTR lpstrFilter;
  LPWSTR lpstrCustomFilter;
  DWORD nMaxCustFilter;
  DWORD nFilterIndex;
  LPWSTR lpstrFile;
  DWORD nMaxFile;
  LPWSTR lpstrFileTitle;
  DWORD nMaxFileTitle;
  LPCWSTR lpstrInitialDir;
  LPCWSTR lpstrTitle;
  DWORD Flags;
  WORD nFileOffset;
  WORD nFileExtension;
  LPCWSTR lpstrDefExt;
  LPARAM lCustData;
  LPOFNHOOKPROC lpfnHook;
  LPCWSTR lpTemplateName;
  void *pvReserved;
  DWORD dwReserved;
  DWORD FlagsEx;
} OPENFILENAME_2000W;

typedef struct {
  int nButtonControlID;
  int nButtonStringID;
  DWORD dwFlags;         //See BMB_* defines.
} BUTTONMESSAGEBOX;

typedef struct {
  HWND hWndParent;
  const wchar_t *wpText;
  const wchar_t *wpCaption;
  UINT uType;
  HICON hIcon;
  BUTTONMESSAGEBOX *btn;
} DIALOGMESSAGEBOX;

typedef struct {
  POSTMESSAGE pm;
  PARSECMDLINESENDW pcls;
} PMPARSECMDLINEW;

typedef struct {
  BOOL bModified;
  BOOL bReadOnly;
  BOOL bOvertypeMode;
  int nNewLine;
  int nCodePage;
  BOOL bBOM;
} STATUSSTATE;


//// Functions prototype

HANDLE CreateEditWindow(HWND hWndParent, HWND hWndEditPMDI);
void SetEditWindowSettings(FRAMEDATA *lpFrame);
void ResizeEditWindow(FRAMEDATA *lpFrame, DWORD dwFlags);
FRAMEDATA* GetFrameDataFromEditWindow(HWND hWndEditCtrl);
FRAMEDATA* GetFrameDataFromEditDocument(AEHDOC hDocEditHandle);
FRAMEDATA* CreateFrameData(HWND hWndEditParent, FRAMEDATA *lpFrameSource);
void CopyFrameData(FRAMEDATA *lpFrameTarget, FRAMEDATA *lpFrameSource);
void SaveFrameData(FRAMEDATA *lpFrame);
void RestoreFrameData(FRAMEDATA *lpFrame, DWORD dwFlagsPMDI);
BOOL CreateMdiFrameWindow(RECT *rcRect);
FRAMEDATA* ActivateMdiFrameWindow(FRAMEDATA *lpFrame, DWORD dwFlags);
FRAMEDATA* NextMdiFrameWindow(FRAMEDATA *lpFrame, BOOL bPrev);
int DestroyMdiFrameWindow(FRAMEDATA *lpFrame);
BOOL FrameNoWindows();
void SplitCreate(FRAMEDATA *lpFrame, DWORD dwFlags);
void SplitDestroy(FRAMEDATA *lpFrame, DWORD dwFlags);
void SplitVisUpdate(FRAMEDATA *lpFrame);

BOOL DoFileNew();
BOOL CloseDocument(DWORD dwPrompt);
HWND DoFileNewWindow(DWORD dwAddFlags);
BOOL CALLBACK EnumThreadWindowsProc(HWND hwnd, LPARAM lParam);
BOOL DoFileOpen();
int DoFileReopenAs(DWORD dwFlags, int nCodePage, BOOL bBOM);
BOOL DoFileSave();
BOOL SaveChanged(DWORD dwPrompt);
BOOL DoFileSaveAs(int nDialogCodePage, BOOL bDialogBOM);
void DoFileSaveAllAs();
BOOL DoFilePageSetup(HWND hWndOwner);
int DoFilePrint(FRAMEDATA *lpFrame, BOOL bSilent);
void DoFilePreview(HWND hWnd);
void DoEditUndo(HWND hWnd);
void DoEditRedo(HWND hWnd);
void DoEditCut(HWND hWnd);
void DoEditCopy(HWND hWnd);
BOOL DoEditPaste(HWND hWnd, DWORD dwFlags);
void DoEditClear(HWND hWnd);
void DoEditSelectAll(HWND hWnd);
void DoEditInsertDate(HWND hWnd);
void DoEditInsertChar();
void DoEditRecode();
BOOL DoEditInsertStringInSelectionW(HWND hWnd, int nAction, const wchar_t *wpString);
BOOL DoEditDeleteFirstCharW(HWND hWnd);
BOOL DoEditDeleteTrailingWhitespacesW(HWND hWnd);
BOOL DoEditChangeCaseW(HWND hWnd, int nCase);
void DoEditFind();
INT_PTR DoEditFindNextDown(FRAMEDATA *lpFrame);
INT_PTR DoEditFindNextUp(FRAMEDATA *lpFrame);
void DoEditReplace();
void DoEditGoTo();
BOOL DoViewFont(HWND hWndOwner, LOGFONTW *lfFont);
void DoViewColors();
void DoViewFontSize(FRAMEDATA *lpFrame, int nAction);
void DoViewReadOnly(FRAMEDATA *lpFrame, BOOL bState, BOOL bFirst);
void DoViewWordWrap(FRAMEDATA *lpFrame, BOOL bState, BOOL bFirst);
void DoViewSplitWindow(BOOL bState, WPARAM wParam);
void DoViewOnTop(BOOL bState, BOOL bFirst);
void DoViewShowStatusBar(BOOL bState, BOOL bFirst);
BOOL DoSettingsExec();
void DoSettingsSaveTime(BOOL bState);
void DoSettingsWatchFile(BOOL bState);
void DoSettingsSingleOpenFile(BOOL bState);
void DoSettingsSingleOpenProgram(BOOL bState);
void DoSettingsKeepSpace(BOOL bState);
void DoSettingsPlugins();
void DoSettingsOptions();
void DoWindowTabView(DWORD dwNewView, BOOL bFirst);
void DoWindowTabType(DWORD dwNewType, BOOL bFirst);
void DoWindowSelectWindow();
void DoHelpAbout();
void DoNonMenuDelLine(HWND hWnd);

BOOL OpenIniA(INIFILE *hIniFile, const char *pFile, BOOL bCreate);
BOOL OpenIniW(INIFILE *hIniFile, const wchar_t *wpFile, BOOL bCreate);
BOOL SaveIniA(INIFILE *hIniFile, const char *pFile);
BOOL SaveIniW(INIFILE *hIniFile, const wchar_t *wpFile);
BOOL ReadIni(INIFILE *hIniFile, HANDLE hFile);
BOOL WriteIni(INIFILE *hIniFile, HANDLE hFile);
INISECTION* StackOpenIniSectionA(INIFILE *hIniFile, const char *pSection, int nSectionLen, BOOL bCreate);
INISECTION* StackOpenIniSectionW(INIFILE *hIniFile, const wchar_t *wpSection, int nSectionLen, BOOL bCreate);
INIKEY* StackOpenIniKeyA(INISECTION *lpIniSection, const char *pKey, int nKeyLen, BOOL bCreate);
INIKEY* StackOpenIniKeyW(INISECTION *lpIniSection, const wchar_t *wpKey, int nKeyLen, BOOL bCreate);
int StackGetIniData(INIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL StackSetIniData(INIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL StackDeleteIniKey(INISECTION *lpIniSection, INIKEY *lpIniKey);
BOOL StackDeleteIniSection(INIFILE *hIniFile, INISECTION *lpIniSection, BOOL bOnlyClear);
void StackFreeIni(INIFILE *hIniFile);
INIKEY* IniOpenKeyA(INIFILE *hIniFile, const char *pSection, const char *pKey, BOOL bCreate, INISECTION **lppIniSection);
INIKEY* IniOpenKeyW(INIFILE *hIniFile, const wchar_t *wpSection, const wchar_t *wpKey, BOOL bCreate, INISECTION **lppIniSection);
int IniGetValueA(INIFILE *hIniFile, const char *pSection, const char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
int IniGetValueW(INIFILE *hIniFile, const wchar_t *wpSection, const wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueA(INIFILE *hIniFile, const char *pSection, const char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueW(INIFILE *hIniFile, const wchar_t *wpSection, const wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniDelKeyA(INIFILE *hIniFile, const char *pSection, const char *pKey);
BOOL IniDelKeyW(INIFILE *hIniFile, const wchar_t *wpSection, const wchar_t *wpKey);

DWORD ReadOption(OPTIONHANDLE *oh, const wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
DWORD SaveOption(OPTIONHANDLE *oh, const wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
void ReadOptions(MAINOPTIONS *mo, FRAMEDATA *fd);
BOOL SaveOptions(MAINOPTIONS *mo, FRAMEDATA *fd, int nSaveSettings, BOOL bForceWrite);

int OpenDocument(HWND hWnd, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
void FileStreamIn(FILESTREAMDATA *lpData);
DWORD CALLBACK InputStreamCallback(UINT_PTR dwCookie, wchar_t *wszBuf, DWORD dwBufBytesLen, DWORD *dwBufBytesDone);
UINT_PTR ReadFileContent(HANDLE hFile, UINT_PTR dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wpContent);
BOOL OpenDocumentSend(HWND hWnd, HWND hWndEditCtrl, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess);
int SaveDocument(HWND hWnd, const wchar_t *wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
void FileStreamOut(FILESTREAMDATA *lpData);
DWORD CALLBACK OutputStreamCallback(UINT_PTR dwCookie, wchar_t *wszBuf, DWORD dwBufBytesLen, DWORD *dwBufBytesDone);
BOOL OpenDirectory(wchar_t *wpPath, BOOL bSubDir);
void DropFiles(HDROP hDrop);
void CheckModificationTime(FRAMEDATA *lpFrame);
BOOL CALLBACK SaveAllAsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

unsigned int CALLBACK PrintPageSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL GetPrinterA(HWND hWndOwner, PRINTINFO *prninfo, BOOL bSilent);
BOOL GetPrinterW(HWND hWndOwner, PRINTINFO *prninfo, BOOL bSilent);
DWORD GetMappedPrintWidth(HWND hWnd);
BOOL UpdateMappedPrintWidth(FRAMEDATA *lpFrame);
int PrintDocument(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage);
BOOL PrintHeadline(HDC hDC, RECT *rc, wchar_t *wpHeadline, int nPageNumber);
BOOL CALLBACK PreviewDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL PreviewInit(HWND hWndSelection);
void PreviewUninit();
LRESULT CALLBACK PreviewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PreviewMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void PreviewPaint(HWND hWnd, HDC hPaintDC, HENHMETAFILE hMetaFile);
int PreviewHScroll(HWND hWnd, int nAction, int nPos);
int PreviewVScroll(HWND hWnd, int nAction, int nPos);
void PreviewScrollUpdate(HWND hWnd);
BOOL FitInside(int nWidth, int nHeight, int nMaxWidth, int nMaxHeight, SIZE *s);
int RectW(const RECT *rc);
int RectH(const RECT *rc);
PRINTPAGE* StackPageInsert(HSTACK *hStack);
PRINTPAGE* StackPageGet(HSTACK *hStack, int nPage);
int StackPageFind(HSTACK *hStack, const AECHARINDEX *ciPos);
void StackPageFree(HSTACK *hStack);

UINT_PTR CALLBACK CodePageDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFilePreviewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxCodepage(HWND hWnd, int *lpCodepageList);
void FillListBoxCodepage(HWND hWnd, int *lpCodepageList);
void ClearCombobox(HWND hWnd);
void ClearListBox(HWND hWnd);
int GetComboboxCodepage(HWND hWnd);
int GetListBoxCodepage(HWND hWnd);
int SelectComboboxCodepage(HWND hWnd, int nCodepage);
int SelectListBoxCodepage(HWND hWnd, int nCodepage);
void GetListBoxCodepageList(HWND hWnd, int **lpCodepageList);
int EnumCodepageList(int **lpCodepageList);
void RegEnumSystemCodePagesA();
BOOL CALLBACK EnumCodePagesProc(wchar_t *wpCodePage);
int CodepageListLen(int *lpCodepageList);
int CodepageListFind(int *lpCodepageList, int nCodePage);
void CodepageListFree(int **lpCodepageList);
void GetCodePageName(int nCodePage, wchar_t *wszCodePage, int nLen);
int FilePreview(HWND hWnd, wchar_t *wpFile, UINT_PTR dwPreviewBytes, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
int AutodetectCodePage(const wchar_t *wpFile, UINT_PTR dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
BOOL AutodetectMultibyte(DWORD dwLangID, unsigned char *pBuffer, UINT_PTR dwBytesToCheck, int *nCodePage);
int GetDetectionIndex(DWORD dwLangID);
BOOL IsLangEasternEurope(DWORD dwLangID);
BOOL IsLangWesternEurope(DWORD dwLangID);
BOOL IsCharLegalUTF8(const unsigned char *pSource, unsigned int nTrailingBytes);
void ChangeTwoBytesOrder(unsigned char *lpBuffer, UINT_PTR dwBufferLen);
void ChangeFourBytesOrder(unsigned char *lpBuffer, UINT_PTR dwBufferLen);
BOOL IsCodePageUnicode(int nCodePage);
BOOL IsCodePageValid(int nCodePage);
unsigned int TranslateNewLinesToUnixW(wchar_t *wszWideString, unsigned int nWideStringLen);

void RegReadSearch();
BOOL CALLBACK FindAndReplaceDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewComboboxEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxSearch(HWND hWndFind, HWND hWndReplace);
int GetComboboxSearchText(HWND hWnd, wchar_t **wszText_orig, wchar_t **wszText, int nNewLine);
void SaveComboboxSearch(HWND hWndFind, HWND hWndReplace);
INT_PTR TextFindW(FRAMEDATA *lpFrame, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen);
INT_PTR TextReplaceW(FRAMEDATA *lpFrame, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen, const wchar_t *wpReplaceWith, int nReplaceWithLen, BOOL bAll, INT_PTR *nReplaceCount);
INT_PTR StrReplaceW(const wchar_t *wpText, INT_PTR nTextLen, const wchar_t *wpIt, int nItLen, const wchar_t *wpWith, int nWithLen, DWORD dwFlags, wchar_t *wszResult, INT_PTR *nResultLen, INT_PTR *nMin, INT_PTR *nMax, INT_PTR *nFirstVis);
INT_PTR EscapeStringToEscapeDataW(const wchar_t *wpInput, wchar_t *wszOutput, int nNewLine);
void EscapeDataToEscapeStringW(const wchar_t *wpInput, wchar_t *wszOutput);

void GetSel(HWND hWnd, AECHARRANGE *crSel, BOOL *bColumnSel, AECHARINDEX *ciCaret);
void SetSel(HWND hWnd, AECHARRANGE *crSel, DWORD dwFlags, AECHARINDEX *ciCaret);
void ReplaceSelA(HWND hWnd, const char *pData, INT_PTR nDataLen, BOOL bColumnSel, int nNewLine, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
void ReplaceSelW(HWND hWnd, const wchar_t *wpData, INT_PTR nDataLen, int nNewLine, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
INT_PTR IndexSubtract(HWND hWnd, AECHARINDEX *ciChar1, AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel);
INT_PTR IndexOffset(HWND hWnd, AECHARINDEX *ciChar, INT_PTR nOffset, int nNewLine);
INT_PTR AkelIndexToRichOffset(HWND hWnd, AECHARINDEX *ciChar);
void RichOffsetToAkelIndex(HWND hWnd, INT_PTR nOffset, AECHARINDEX *ciChar);
INT_PTR GetTextLength(HWND hWnd);
INT_PTR GetRangeTextA(HWND hWnd, INT_PTR nMin, INT_PTR nMax, char **pText);
INT_PTR GetRangeTextW(HWND hWnd, INT_PTR nMin, INT_PTR nMax, wchar_t **wpText);
INT_PTR ExGetRangeTextA(HWND hWnd, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefChar, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, char **pText, int nNewLine, BOOL bFillSpaces);
INT_PTR ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, wchar_t **wpText, int nNewLine, BOOL bFillSpaces);
BOOL FreeText(LPVOID pText);
BOOL PasteInEditAsRichEdit(HWND hWnd, int nMaxLenght);
void ShowStandardViewMenu(HWND hWnd, HMENU hMenu, BOOL bMouse);

BOOL CALLBACK GoToDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL GoTo(DWORD dwGotoType, const wchar_t *wpString);

RECENTFILE* RecentFilesInsert(RECENTFILESTACK *hStack, int nIndex);
void RecentFilesDelete(RECENTFILESTACK *hStack, RECENTFILE *lpRecentFile);
void RecentFilesZero(RECENTFILESTACK *hStack);
RECENTFILE* RecentFilesFindByName(const wchar_t *wpFile, int *lpIndex);
RECENTFILE* RecentFilesFindByIndex(int nIndex);
RECENTFILE* RecentFilesUpdate(const wchar_t *wpFile);
int RecentFilesDeleteOld(RECENTFILESTACK *hStack);
int RecentFilesRead(RECENTFILESTACK *hStack);
void RecentFilesSave(RECENTFILESTACK *hStack);
void RecentFilesSaveFile(FRAMEDATA *lpFrame);
void RecentFilesMenu();
RECENTFILEPARAM* StackRecentFileParamAdd(RECENTFILE *lpRecentFile);
RECENTFILEPARAM* StackRecentFileParamGetByName(RECENTFILE *lpRecentFile, const wchar_t *wpParamName);
void StackRecentFileParamDelete(RECENTFILEPARAM *lpRecentFileParam);
void StackRecentFileParamFree(RECENTFILE *lpRecentFile);
int TrimPathW(wchar_t *wszResult, const wchar_t *wpPath, int nMax);
int FixAmpW(const wchar_t *wpInput, wchar_t *wszOutput, int nOutputMax);

void LanguageMenu();

void FillMenuPopupCodepage();
void ShowMenuPopupCodepage(POINT *ptScreen);

void RecodeTextW(FRAMEDATA *lpFrame, HWND hWndPreview, DWORD dwFlags, int *nCodePageFrom, int *nCodePageTo);
BOOL CALLBACK RecodeDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ColorsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxThemes(HWND hWnd);
COLORTHEME* StackThemeAdd(HSTACK *hStack, const wchar_t *wpName, AECOLORS *aec);
COLORTHEME* StackThemeGetByName(HSTACK *hStack, const wchar_t *wpName);
COLORTHEME* StackThemeGetByColors(HSTACK *hStack, AECOLORS *aec);
void StackThemeDelete(HSTACK *hStack, COLORTHEME *ctElement);
void StackThemeFree(HSTACK *hStack);
void ReadThemes(MAINOPTIONS *mo);
BOOL SaveThemes(int nSaveSettings);

void RegisterPluginsHotkeys(MAINOPTIONS *mo);
PLUGINFUNCTION* StackPluginFind(HSTACK *hStack, const wchar_t *wpPluginFunction, int nPluginFunctionLen);
PLUGINFUNCTION* StackHotkeyFind(HSTACK *hStack, WORD wHotkey);
PLUGINFUNCTION* StackPluginAdd(HSTACK *hStack, const wchar_t *wpString, int nStringLen, WORD wHotkey, BOOL bOnStart, PLUGINPROC PluginProc, void *lpParameter);
void StackPluginDelete(HSTACK *hStack, void *lpElement);
BOOL StackPluginSave(HSTACK *hStack, int nSaveSettings);
void StackPluginCleanUp(HSTACK *hStack, BOOL bDeleteNonExistentDLL);
void StackPluginFree(HSTACK *hStack);
void CallPluginsOnStart(HSTACK *hStack);
int CallPluginSend(PLUGINFUNCTION **ppfElement, PLUGINCALLSENDW *pcs, DWORD dwFlags);
int CallPlugin(PLUGINFUNCTION *lpPluginFunction, PLUGINCALLSENDW *pcs, DWORD dwFlags);
int TranslateMessageAll(LPMSG lpMsg);
BOOL TranslateMessageGlobal(LPMSG lpMsg);
BOOL TranslateMessagePlugin(LPMSG lpMsg);
int TranslateMessageHotkey(HSTACK *hStack, LPMSG lpMsg);
BOOL ParsePluginNameW(const wchar_t *wpFullName, wchar_t *wszPlugin, wchar_t *wszFunction);
BOOL IsMainFunctionW(const wchar_t *wpFunction);

BOOL CALLBACK PluginsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);
void FillPluginList(HWND hWnd);
BOOL CALLBACK FillPluginListProc(char *pExportName, LPARAM lParam);
PLUGINLISTITEM* GetPluginListItem(HSTACK *hStack, int nIndex);
void FreePluginList(HSTACK *hStack);
BOOL GetExportNames(HMODULE hInstance, EXPORTNAMESPROC lpExportNamesProc, LPARAM lParam);
int GetHotkeyString(WORD wHotkey, wchar_t *wszString);

LRESULT CALLBACK CBTPropertySheetProc(int iCode, WPARAM wParam, LPARAM lParam);
int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam);
BOOL CALLBACK OptionsGeneralDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL CALLBACK OptionsGeneralFilterDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsRegistryDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsEditor1DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsEditor2DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvancedDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK MdiListDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillMdiListListBox(HWND hWnd, BOOL bSort, int nModifyFilter, BOOL bOnlyNames);
int MoveListBoxItem(HWND hWnd, int nOldIndex, int nNewIndex);
BOOL ShiftListBoxSelItems(HWND hWnd, BOOL bMoveDown);
BOOL SaveListBoxSelItems(HWND hWnd);
void ArrangeListBoxSelItems(HWND hWnd, int nBar);
BOOL CloseListBoxSelItems(HWND hWnd);
int GetListBoxSelItems(HWND hWnd, int **lpSelItems);
void FreeListBoxSelItems(int **lpSelItems);

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GetUserManual(wchar_t *wszManual, int nManualLen);

int MessageBoxCustom(HWND hWndParent, const wchar_t *wpText, const wchar_t *wpCaption, UINT uType, HICON hIcon, BUTTONMESSAGEBOX *btn);
BOOL CALLBACK MessageBoxDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL ScaleInit(HDC hDC, HWND hWnd);
int ScaleX(int x);
int ScaleY(int y);

DOCK* StackDockAdd(HDOCK *hDocks, DOCK *dkData);
int DockSetSide(HDOCK *hDocks, DOCK *dkData, int nSide);
void StackDockDelete(HDOCK *hDocks, DOCK *dkData);
DOCK* StackDockFindWindow(HDOCK *hDocks, HWND hWnd, BOOL bChild);
HWND StackDockNextWindow(HDOCK *hDocks, DOCK *dkData, BOOL bPrevious);
DOCK* StackDockFromPoint(HDOCK *hDocks, POINT *ptScreen);
void StackDockSize(HDOCK *hDocks, int nSide, NSIZE *ns);
BOOL StackDockUpdateCheck(HDOCK *hDocks);
void StackDockFree(HDOCK *hDocks);
BOOL TranslateMessageDialog(HDOCK *hDocks, LPMSG lpMsg);

int StackProcGet(HSTACK *hStack, int nIndex, WNDPROCDATA **ProcData);
int StackProcSet(HSTACK *hStack, WNDPROC NewProc, WNDPROCDATA **NewProcData, WNDPROC *FirstProc);
void StackProcFree(HSTACK *hStack);

PLUGINHANDLE* StackHandleIncrease(HSTACK *hStack, HMODULE hModule);
PLUGINHANDLE* StackHandleDecrease(HSTACK *hStack, HMODULE hModule);
void StackHandleFree(HSTACK *hStack);

FRAMEDATA* StackFrameInsert(HSTACK *hStack);
FRAMEDATA* StackFrameGetByIndex(HSTACK *hStack, int nIndex);
FRAMEDATA* StackFrameGetByHandle(HSTACK *hStack, AEHDOC hDataHandle);
FRAMEDATA* StackFrameGetByName(HSTACK *hStack, const wchar_t *wpFileName, int nFileNameLen);
FRAMEDATA* StackFrameGetNext(HSTACK *hStack, FRAMEDATA *lpFrame, BOOL bPrev);
FRAMEDATA* StackFrameIsValid(HSTACK *hStack, FRAMEDATA *lpFramePointer);
void StackFrameMove(HSTACK *hStack, FRAMEDATA *lpFrame, int nIndex);
void StackFrameDelete(HSTACK *hStack, FRAMEDATA *lpFrame);
void StackFramesFree(HSTACK *hStack);

BUTTONDRAWITEM* StackButtonDrawInsert(HSTACK *hStack);
BUTTONDRAWITEM* StackButtonDrawGet(HSTACK *hStack, HWND hWnd);
void StackButtonDrawDelete(HSTACK *hStack, BUTTONDRAWITEM *lpButtonDraw);
void StackButtonDrawFree(HSTACK *hStack);
void SetButtonDraw(HWND hWndButton, BUTTONDRAW *bd);

void SetSelectionStatus(AEHDOC hDocEdit, HWND hWndEdit, AECHARRANGE *cr, AECHARINDEX *ci);
void SetModifyStatus(FRAMEDATA *lpFrame, BOOL bState);
void SetOvertypeStatus(FRAMEDATA *lpFrame, BOOL bState);
void SetNewLineStatus(FRAMEDATA *lpFrame, int nState, DWORD dwFlags);
void SetCodePageStatus(FRAMEDATA *lpFrame, int nCodePage, BOOL bBOM);
void UpdateStatusUser(FRAMEDATA *lpFrame, DWORD dwFlags);
DWORD TranslateStatusUser(FRAMEDATA *lpFrame, const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);

const wchar_t* GetAssociatedIconW(const wchar_t *wpFile, wchar_t *wszIconFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
void AssociateFileTypesW(HINSTANCE hInstance, const wchar_t *wpFileTypes, DWORD dwFlags);
ASSOCICON* StackIconInsert(STACKASSOCICON *hStack, const wchar_t *wpFile, int nFileLen);
ASSOCICON* StackIconGet(STACKASSOCICON *hStack, const wchar_t *wpFile, int nFileLen, const wchar_t *wpExt);
void StackIconsFree(STACKASSOCICON *hStack);

HFONT SetChosenFont(HWND hWnd, const LOGFONTW *lfFont);
FONTITEM* StackFontItemInsert(HSTACK *hStack, const LOGFONTW *lfFont);
FONTITEM* StackFontItemGet(HSTACK *hStack, const LOGFONTW *lfFont);
void StackFontItemsFree(HSTACK *hStack);

wchar_t* GetCommandLineParamsWide(void);
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg, BOOL bParseAsNotepad);
int ParseCmdLine(const wchar_t **wppCmdLine, int nType);
void SendCmdLine(HWND hWnd, const wchar_t *wpCmdLine, BOOL bPost);
void ParseMethodParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpText, const wchar_t **wppText);
void ExpandMethodParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpFile, const wchar_t *wpExeDir);
int StructMethodParameters(STACKEXTPARAM *hParamStack, unsigned char **lppStruct);
EXTPARAM* GetMethodParameter(STACKEXTPARAM *hParamStack, int nIndex);
void FreeMethodParameters(STACKEXTPARAM *hParamStack);
INT_PTR TranslateEscapeString(FRAMEDATA *lpFrame, const wchar_t *wpInput, wchar_t *wszOutput, DWORD *lpdwCaret);

BOOL GetEditInfo(HWND hWnd, EDITINFO *ei);
BOOL SetCurEditOption(int nType, UINT_PTR dwData);
DWORD IsEditActive(HWND hWnd);
void UpdateShowHScroll(FRAMEDATA *lpFrame);
int SaveLineScroll(HWND hWnd);
void RestoreLineScroll(HWND hWnd, int nBeforeLine);
DWORD ScrollCaret(HWND hWnd);
BOOL SelectColorDialog(HWND hWndOwner, COLORREF *crColor);
BOOL GetCharColor(FRAMEDATA *lpFrame, CHARCOLOR *cc);
void SetMarker(FRAMEDATA *lpFrame, DWORD dwPos);
void SetWordWrap(FRAMEDATA *lpFrame, DWORD dwType, DWORD dwLimit);
void SetMargins(HWND hWnd, DWORD dwNewMargins, DWORD dwOldMargins);
void SetTabStops(HWND hWnd, int nTabStops, BOOL bSetRedraw);
BOOL InsertTabStop(HWND hWnd);
BOOL IndentTabStop(HWND hWnd, int nAction);
BOOL AutoIndent(HWND hWnd, AECHARRANGE *cr);
int SetUrlPrefixes(HWND hWnd, const wchar_t *wpPrefixes);
BOOL IsReadOnly(HWND hWnd);
int IsFile(const wchar_t *wpFile);
BOOL IsPathFull(const wchar_t *wpPath);
int GetExeDir(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen);
int GetFileDir(const wchar_t *wpFile, wchar_t *wszFileDir, int nFileDirLen);
BOOL GetFullName(const wchar_t *wpFile, wchar_t *wszFileFullName, int nFileMax);
const wchar_t* GetFileName(const wchar_t *wpFile);
int GetBaseName(const wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen);
const wchar_t* GetFileExt(const wchar_t *wpFile);
void TrimPathBackslash(wchar_t *wszPath);
void TrimModifyState(wchar_t *wszFile);
void GetTimeString(const wchar_t *wpFormat, wchar_t *wszOutput, BOOL bWithoutSeconds);
BOOL GetFileWriteTimeWide(const wchar_t *wpFile, FILETIME *ft);
BOOL GetFileVersionA(const char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
BOOL GetFileVersionW(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2);
int TranslateFileString(const wchar_t *wpCommand, wchar_t *wszBuffer, int nBufferSize);
void SetMouseCapture(HWND hWnd, DWORD dwType);
void ReleaseMouseCapture(DWORD dwType);
BOOL SwitchLayout(HWND hWndEdit, AECHARINDEX *lpciCaret);
int DetectCharLayout(int nChar);
void ActivateKeyboard(HKL dwInputLocale);
void ActivateWindow(HWND hWnd);
HWND NextDialog(BOOL bPrevious);
BOOL CALLBACK EnumDialogsProc(HWND hWnd, LPARAM lParam);
HWND NextClone(BOOL bPrevious);
void UpdateSize();
BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcInit, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetMovingRect(DOCK *dkData, POINT *ptScreen, MINMAXINFO *mmi, RECT *rcScreen);
void DrawMovingRect(RECT *rcScreen);
int GetMouseEdge(HWND hWnd, POINT *ptScreen);
void SetEdgeCursor(int nEdge);
int GetOppEdge(int nEdge);
BOOL IsCursorOnWindow(HWND hWnd);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL ScreenToClientRect(HWND hWnd, RECT *rc);
BOOL ClientToScreenRect(HWND hWnd, RECT *rc);
BOOL EnsureWindowInMonitor(RECT *rcWindow);
void UpdateTitle(FRAMEDATA *lpFrame, const wchar_t *wszFile);
void UpdateTabs(HWND hWnd);
int AddTabItem(HWND hWnd, LPARAM lParam);
LPARAM GetTabParamFromItem(HWND hWnd, int nItem);
int GetTabItemFromParam(HWND hWnd, LPARAM lParam);
int GetTabItemFromCursorPos(HWND hWnd);
int GetTabItemForDrop(HWND hWnd, POINT *rcScreen);
int SelectTabItem(HWND hWnd, int nIndex);
int MoveTabItem(HWND hWnd, int nIndexOld, int nIndexNew);
BOOL DeleteTabItem(HWND hWnd, int nIndex);
void FreeMemorySearch();
int BytesInString(const wchar_t *wpString);
char* AKD_strchr(const char *s, int c);
wchar_t* AKD_wcschr(const wchar_t *s, wchar_t c);

HMENU API_LoadMenuA(HINSTANCE hLoadInstance, char *lpMenuName);
HMENU API_LoadMenuW(HINSTANCE hLoadInstance, wchar_t *lpMenuName);
HACCEL API_LoadAcceleratorsA(HINSTANCE hLoadInstance, char *lpTableName);
HACCEL API_LoadAcceleratorsW(HINSTANCE hLoadInstance, wchar_t *lpTableName);
HICON API_LoadIconA(HINSTANCE hLoadInstance, char *lpIconName);
HICON API_LoadIconW(HINSTANCE hLoadInstance, wchar_t *lpIconName);
HANDLE API_LoadImageA(HINSTANCE hLoadInstance, char *lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);
HANDLE API_LoadImageW(HINSTANCE hLoadInstance, wchar_t *lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);
int API_LoadStringA(HINSTANCE hLoadInstance, UINT uID, char *lpBuffer, int nBufferMax);
int API_LoadStringW(HINSTANCE hLoadInstance, UINT uID, wchar_t *lpBuffer, int nBufferMax);
int API_MessageBox(HWND hWnd, const wchar_t *lpText, const wchar_t *lpCaption, UINT uType);
HWND API_CreateDialog(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBox(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBoxParam(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
HANDLE API_CreateFileA(const char *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE API_CreateFileW(const wchar_t *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL API_WriteFile(HANDLE hFile, LPCVOID lpBuffer, UINT_PTR nNumberOfBytesToWrite, UINT_PTR *lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
LPVOID API_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
BOOL API_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
wchar_t* AllocWideStr(SIZE_T dwSize);
BOOL FreeWideStr(wchar_t *wpVar);

#endif
