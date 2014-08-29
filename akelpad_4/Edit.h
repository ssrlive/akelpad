#ifndef __EDIT_H__
#define __EDIT_H__


//// Defines

#ifdef _WIN64
  #define APP_ABOUT_VERSIONW        L"AkelPad 4.9.0 (x64)"
#else
  #define APP_ABOUT_VERSIONW        L"AkelPad 4.9.0 (x86)"
#endif
#ifdef AKELPAD_DLLBUILD
  #define APP_MAIN_CLASSA            "AkelPad4 Library"
  #define APP_MAIN_CLASSW           L"AkelPad4 Library"
#else
  #define APP_MAIN_CLASSA            "AkelPad4"
  #define APP_MAIN_CLASSW           L"AkelPad4"
#endif
#define APP_MAIN_TITLEW             L"AkelPad"
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

#define STR_NEWLINE_WIN             L"Win"
#define STR_NEWLINE_UNIX            L"Unix"
#define STR_NEWLINE_MAC             L"Mac"

#define STR_AKELADMINW              L"AkelAdmin"
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
#define EDIT_BKIMAGEALPHA          128

//ParseCmdLine flags
#define PCL_ONLOAD          0
#define PCL_ONSHOW          1
#define PCL_ONMESSAGE       2

//Open file dialog
#define IDC_OFN_LIST                   1120
#define IDC_OFN_CODEPAGECOMBOBOX_LABEL 1089
#define IDC_OFN_CODEPAGECOMBOBOX       1136
#define IDC_OFN_FILECOMBOBOX           1148
#define IDC_OFN_FILEEDIT               1152
#define IDC_OFN_PLACESBAR              1184

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

//File dialog notifications
#define AKDLG_SETSTREAM                (WM_USER + 100)

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

//Minimum detect characters representation
#define DETECTCHARS_REQUIRED      10

//Char layout
#define CHARLAYOUT_NONE       0
#define CHARLAYOUT_ENGLISH    1
#define CHARLAYOUT_NONENGLISH 2

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

//AkelAdmin action
#define AAA_CMDINIT             11
#define AAA_EXIT                20  //Exit from pipe server.
#define AAA_SECURITYGET         21  //Retrieve file security.
#define AAA_SECURITYSETEVERYONE 22  //Add all access for the file.
#define AAA_SECURITYRESTORE     23  //Restore saved security for the file.
#define AAA_SECURITYFREE        24  //Free saved security.
#define AAA_CREATEFILE          25  //Create new file.

//Find/Replace dialog message
#define AKDLG_PUTFIND           (WM_USER + 100)

//Recode dialog message
#define AKDLG_RECODEUPDATE      (WM_USER + 100)

//Insert/Delete char in selection
#define STRSEL_CHECK      0x00000001
#define STRSEL_MULTILINE  0x00000002
#define STRSEL_FULLLINE   0x00000004
#define STRSEL_INSERT     0x00000008
#define STRSEL_DELETE     0x00000010
#define STRSEL_LEADTAB    0x00000020  //Uses only with STRSEL_DELETE.
#define STRSEL_LEADSPACE  0x00000040  //Uses only with STRSEL_DELETE.
#define STRSEL_ALLSPACES  0x00000080  //Uses only with STRSEL_DELETE.

//DetectCase
#define DC_UPPERCASE     0x1
#define DC_LOWERCASE     0x2
#define DC_SENTENCECASE  0x4
#define DC_TITLECASE     0x8

//Change font size
#define FONTSIZE_INCREASE  1
#define FONTSIZE_DECREASE  2
#define FONTSIZE_RESTORE   3

//SaveChanged flags
#define PROMPT_NONE           0x001  //Don't prompt, answering "No".
#define PROMPT_NOTOALLBUTTON  0x002  //Show "No to all" button if necessary.

//Custom statusbar
#define CSB_CHARHEX           0x000001
#define CSB_CHARDEC           0x000002
#define CSB_CHARLETTER        0x000004
#define CSB_RICHOFFSET        0x000008
#define CSB_BYTEOFFSET        0x000010
#define CSB_LINEALLCOUNT      0x000020
#define CSB_LINESELCOUNT      0x000040
#define CSB_LINESELBEGIN      0x000080
#define CSB_LINESELEND        0x000100
#define CSB_RICHCOUNT         0x000200
#define CSB_FONTPOINT         0x000400
#define CSB_TABSIZE           0x000800
#define CSB_MARKER            0x001000
#define CSB_REPLACECOUNT      0x002000
#define CSB_SEARCHENDED       0x004000
#define CSB_DOCUMENTSCOUNT    0x008000
#define CSB_DOCUMENTSMODIFIED 0x010000
#define CSB_DOCUMENTSSAVED    0x020000
#define CSB_DOCUMENTINDEX     0x040000
#define CSB_CAPSLOCK          0x080000
#define CSB_NUMLOCK           0x100000

//Colors list
#define LVI_COLOR_BASIC            0
#define LVI_COLOR_SELECTION        1
#define LVI_COLOR_ACTIVELINE       2
#define LVI_COLOR_ACTIVELINEBORDER 3
#define LVI_COLOR_ALTLINE          4
#define LVI_COLOR_ALTLINEBORDER    5
#define LVI_COLOR_ACTIVECOLUMN     6
#define LVI_COLOR_COLUMNMARKER     7
#define LVI_COLOR_CARET            8
#define LVI_COLOR_URL              9
#define LVI_COLOR_ACTIVEURL        10
#define LVI_COLOR_VISITURL         11

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
  unsigned char *pCmdLine;
  DWORD dwStyle;
  int x;
  int y;
  int nWidth;
  int nHeight;
  HWND hWndParent;
  HMENU hMenu;
} MAINCREATE;

typedef struct {
  //Save place
  int nSaveSettings;

  //Manual
  DWORD dwShowModify;
  DWORD dwStatusPosType;
  wchar_t wszStatusUserFormat[MAX_PATH];
  int nStatusUserFormatLen;
  DWORD dwStatusUserFlags;
  DWORD dwWordBreakCustom;
  DWORD dwPaintOptions;
  DWORD dwEditStyle;
  BOOL bRichEditClass;
  BOOL bAkelAdminResident;
  wchar_t wszDateLogFormat[128];
  wchar_t wszDateInsertFormat[128];
  wchar_t wszAkelUpdaterOptions[MAX_PATH];
  wchar_t wszUrlCommand[MAX_PATH];
  wchar_t wszTabNameFind[MAX_PATH];
  wchar_t wszTabNameRep[MAX_PATH];

  //Menu settings
  BOOL bOnTop;
  BOOL bStatusBar;
  char szLangModule[MAX_PATH];
  wchar_t wszLangModule[MAX_PATH];
  BOOL bKeepSpace;
  BOOL bWatchFile;
  BOOL bSaveTime;
  BOOL bSingleOpenFile;
  DWORD dwSingleOpenProgram;
  int nMDI;
  DWORD dwTabOptionsMDI;

  //Settings dialog
  wchar_t wszExecuteCommand[BUFFER_SIZE];
  wchar_t wszExecuteDirectory[MAX_PATH];
  int nDefaultCodePage;
  int bDefaultBOM;
  int nNewFileCodePage;
  int bNewFileBOM;
  int nNewFileNewLine;
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
  DWORD dwType;            //See EXTPARAM_* defines.
  INT_PTR nNumber;         //External parameter number.
  char *pString;           //External parameter string (Ansi).
  wchar_t *wpString;       //External parameter string (Unicode).
  char *pExpanded;         //External parameter expanded string - without %variables% (Ansi).
  int nExpandedAnsiLen;    //External parameter expanded ansi string length.
  wchar_t *wpExpanded;     //External parameter expanded string - without %variables% (Unicode).
  int nExpandedUnicodeLen; //External parameter expanded unicode string length.
} EXTPARAM;

typedef struct {
  EXTPARAM *first;
  EXTPARAM *last;
  int nElements;
} STACKEXTPARAM;

typedef struct _FILESTREAMDATA {
  HWND hWnd;
  HANDLE hFile;
  int nCodePage;
  DWORD dwMaxCharSize;
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
  COLORREF crReserved1;
  COLORREF crReserved2;
  COLORREF crReserved3;
  COLORREF crReserved4;
  wchar_t wszBkImageFile[MAX_PATH];
  int nBkImageAlpha;
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

typedef struct _BKIMAGEITEM {
  struct _BKIMAGEITEM *next;
  struct _BKIMAGEITEM *prev;
  wchar_t wszBkImageFile[MAX_PATH];
  HBITMAP hBkImageBitmap;
} BKIMAGEITEM;

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

typedef struct _STATUSPART {
  struct _STATUSPART *next;
  struct _STATUSPART *prev;
  DWORD dwFormatFlags;
  const wchar_t *wpFormat;
  int nFormatLen;
  int nPartSize;
  int nIndex;
} STATUSPART;

typedef struct {
  STATUSPART *first;
  STATUSPART *last;
  int nElements;
} STACKSTATUSPART;

typedef struct {
  BOOL bModified;
  BOOL bReadOnly;
  BOOL bOvertypeMode;
  int nNewLine;
  int nCodePage;
  BOOL bBOM;
} STATUSSTATE;

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
  DWORD dwExitCode;
  int nAction;
  wchar_t wszFile[MAX_PATH];
  DWORD dwLangModule;
} ADMINPIPE;

typedef struct {
  POSTMESSAGE pm;
  PARSECMDLINESENDW pcls;
} PMPARSECMDLINEW;


//// Functions prototype

HANDLE CreateEditWindow(HWND hWndParent, HWND hWndEditPMDI);
void SetEditWindowSettings(FRAMEDATA *lpFrame);
void SetEditNotify(HWND hWnd);
void ResizeEditWindow(FRAMEDATA *lpFrame, DWORD dwFlags);
FRAMEDATA* GetFrameDataFromEditWindow(HWND hWndEditCtrl);
FRAMEDATA* GetFrameDataFromEditDocument(AEHDOC hDocEditHandle);
FRAMEDATA* CreateFrameData(HWND hWndEditParent, FRAMEDATA *lpFrameSource);
void CopyFrameData(FRAMEDATA *lpFrameTarget, FRAMEDATA *lpFrameSource);
void CopyFrameSettings(FRAMEDATA *lpFrameTarget, FRAMEDATA *lpFrameSource);
void SaveFrameData(FRAMEDATA *lpFrame);
void RestoreFrameData(FRAMEDATA *lpFrame, DWORD dwFlagsPMDI);
BOOL CreateFrameWindow(RECT *rcRect);
FRAMEDATA* ActivateFrameWindow(FRAMEDATA *lpFrame, DWORD dwFlags);
FRAMEDATA* ActivateNextFrameWindow(FRAMEDATA *lpFrame, BOOL bPrev);
FRAMEDATA* GetNextTabFrame(FRAMEDATA *lpFrame, BOOL bPrev);
int DestroyFrameWindow(FRAMEDATA *lpFrame);
BOOL FrameNoWindows();
void SplitCreate(FRAMEDATA *lpFrame, DWORD dwFlags);
void SplitDestroy(FRAMEDATA *lpFrame, DWORD dwFlags);
void SplitVisUpdate(FRAMEDATA *lpFrame);

BOOL DoFileNew();
BOOL CloseDocument(DWORD dwPrompt);
HWND DoFileNewWindow(DWORD dwAddFlags);
BOOL CALLBACK EnumThreadWindowsProc(HWND hWnd, LPARAM lParam);
BOOL DoFileOpen();
int DoFileReopenAs(DWORD dwFlags, int nCodePage, BOOL bBOM);
BOOL DoFileSave();
BOOL SaveChanged(DWORD dwPrompt);
BOOL DoFileSaveAs(int nDialogCodePage, BOOL bDialogBOM);
BOOL DoFileSaveAllAs();
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
BOOL DoEditModifyStringInSelection(HWND hWnd, int nAction, const wchar_t *wpString);
BOOL DoEditDeleteFirstCharW(HWND hWnd);
BOOL DoEditDeleteTrailingWhitespacesW(HWND hWnd);
BOOL DoEditChangeCaseW(HWND hWnd, int nCase, BOOL bSelCurWord);
void ConvertCase(wchar_t *wszText, INT_PTR nTextLen, int nCase);
int DetectSelCase(HWND hWnd);
void DoEditFind();
INT_PTR DoEditFindNextDown(FRAMEDATA *lpFrame);
INT_PTR DoEditFindNextUp(FRAMEDATA *lpFrame);
void DoEditReplace();
void DoEditGoTo();
BOOL DoViewFont(HWND hWndOwner, LOGFONTW *lfFont);
BOOL DoViewColors();
void DoViewFontSize(FRAMEDATA *lpFrame, int nAction);
void DoViewReadOnly(FRAMEDATA *lpFrame, BOOL bState, BOOL bFirst);
void DoViewWordWrap(FRAMEDATA *lpFrame, BOOL bState, BOOL bFirst);
void DoViewSplitWindow(BOOL bState, WPARAM wParam);
void DoViewOnTop(BOOL bState, BOOL bFirst);
void DoViewShowStatusBar(BOOL bState, BOOL bFirst);
BOOL DoSettingsExec();
void DoSettingsKeepSpace(BOOL bState);
void DoSettingsWatchFile(BOOL bState);
void DoSettingsSaveTime(BOOL bState);
void DoSettingsSingleOpenFile(BOOL bState);
void DoSettingsSingleOpenProgram(DWORD dwState);
void DoSettingsPlugins();
void DoSettingsOptions();
void DoWindowTabView(DWORD dwNewView, BOOL bFirst);
void DoWindowTabType(DWORD dwNewType, BOOL bFirst);
void DoWindowTabSwitch(DWORD dwNewSwitch, BOOL bFirst);
void DoWindowSelectWindow();
void DoHelpAbout();
void DoNonMenuDelLine(HWND hWnd, BOOL bAllWrap);
BOOL DoNonMenuSelJumpCaret(HWND hWnd);

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
BOOL SetOption(LPARAM lParam, void *lpData, int nDataSize, int nType);

int OpenDocument(HWND hWnd, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
void FileStreamIn(FILESTREAMDATA *lpData);
DWORD CALLBACK InputStreamCallback(UINT_PTR dwCookie, wchar_t *wszBuf, DWORD dwBufBytesLen, DWORD *dwBufBytesDone);
UINT_PTR ReadFileContent(HANDLE hFile, UINT_PTR dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wpContent);
int WriteFileContent(HANDLE hFile, const wchar_t *wpContent, INT_PTR nContentLen, int nCodePage, BOOL bBOM);
BOOL OpenDocumentSend(HWND hWnd, HWND hWndEditCtrl, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess);
int SaveDocument(HWND hWnd, const wchar_t *wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
void FileStreamOut(FILESTREAMDATA *lpData);
DWORD CALLBACK OutputStreamCallback(UINT_PTR dwCookie, wchar_t *wszBuf, DWORD dwBufBytesLen, DWORD *dwBufBytesDone);
BOOL OpenDirectory(wchar_t *wpPath, BOOL bSubDir);
void DropFiles(HDROP hDrop);
BOOL IsAllowWatchFile(FRAMEDATA *lpFrame);
BOOL CALLBACK SaveAllAsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

unsigned int CALLBACK PrintPageSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL GetPrinter(HWND hWndOwner, PRINTINFO *prninfo, BOOL bSilent);
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

UINT_PTR CALLBACK FileDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFilePreviewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFileParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
int AutodetectCodePage(const wchar_t *wpFile, HANDLE hFile, UINT_PTR dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
BOOL AutodetectMultibyte(DWORD dwLangID, const unsigned char *pBuffer, UINT_PTR dwBytesToCheck, UINT_PTR dwCharsRequired, int *nCodePage);
BOOL AutodetectWideChar(DWORD dwLangID, const wchar_t *wpText, INT_PTR nTextLen, INT_PTR nMinChars, int *nCodePageFrom, int *nCodePageTo);
BOOL ConvertAnsiString(const char *pInput, INT_PTR nInputLen, int nCodePageFrom, int nCodePageTo, char **szOutput, INT_PTR *lpnOutputLen);
BOOL ConvertWideString(const wchar_t *wpInput, INT_PTR nInputLen, int nCodePageFrom, int nCodePageTo, wchar_t **wszOutput, INT_PTR *lpnOutputLen);
int GetDetectionIndex(DWORD dwLangID);
BOOL IsLangEasternEurope(DWORD dwLangID);
BOOL IsLangWesternEurope(DWORD dwLangID);
BOOL IsCharLegalUTF8(const unsigned char *pSource, unsigned int nTrailingBytes);
void ChangeTwoBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst);
void ChangeFourBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst);
BOOL IsCodePageUnicode(int nCodePage);
BOOL IsCodePageValid(int nCodePage);
unsigned int TranslateNewLinesToUnixW(wchar_t *wszWideString, unsigned int nWideStringLen);

void RegReadSearch();
BOOL CALLBACK FindAndReplaceDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewComboboxEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxSearch(HWND hWndFind, HWND hWndReplace);
int GetComboboxSearchText(HWND hWnd, wchar_t **wszText, int nNewLine);
void SaveComboboxSearch(HWND hWndFind, HWND hWndReplace);
INT_PTR TextFindW(FRAMEDATA *lpFrame, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen);
INT_PTR TextReplaceW(FRAMEDATA *lpFrame, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen, const wchar_t *wpReplaceWith, int nReplaceWithLen, BOOL bAll, INT_PTR *nReplaceCount);
INT_PTR StrReplaceW(const wchar_t *wpText, INT_PTR nTextLen, const wchar_t *wpIt, int nItLen, const wchar_t *wpWith, int nWithLen, DWORD dwFlags, wchar_t *wszResult, INT_PTR *nResultLen, INT_PTR *nMin, INT_PTR *nMax, INT_PTR *nFirstVis);
INT_PTR EscapeStringToEscapeDataW(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput, int nNewLine);
void EscapeDataToEscapeStringW(const wchar_t *wpInput, wchar_t *wszOutput);
BOOL SetDefButtonStyle(HWND hWnd, HWND hWndNewDef);

BOOL GetSel(HWND hWnd, AECHARRANGE *crSel, BOOL *bColumnSel, AECHARINDEX *ciCaret);
void SetSel(HWND hWnd, AECHARRANGE *crSel, DWORD dwFlags, AECHARINDEX *ciCaret);
void SetSelRE(HWND hWnd, INT_PTR nSelStart, INT_PTR nSelEnd);
void ReplaceSelA(HWND hWnd, const char *pData, INT_PTR nDataLen, DWORD dwFlags, int nNewLine, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
void ReplaceSelW(HWND hWnd, const wchar_t *wpData, INT_PTR nDataLen, int nNewLine, DWORD dwFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
INT_PTR IndexSubtract(HWND hWnd, AECHARINDEX *ciChar1, AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel);
INT_PTR IndexOffset(HWND hWnd, AECHARINDEX *ciChar, INT_PTR nOffset, int nNewLine);
INT_PTR AkelIndexToRichOffset(HWND hWnd, AECHARINDEX *ciChar);
void RichOffsetToAkelIndex(HWND hWnd, INT_PTR nOffset, AECHARINDEX *ciChar);
int GetLineNumber(FRAMEDATA *lpFrame, int nWrappedLine);
int GetIndexColumn(FRAMEDATA *lpFrame, const AECHARINDEX *ciChar);
INT_PTR GetTextLength(HWND hWnd);
INT_PTR GetRangeTextA(HWND hWnd, INT_PTR nMin, INT_PTR nMax, char **pText);
INT_PTR GetRangeTextW(HWND hWnd, INT_PTR nMin, INT_PTR nMax, wchar_t **wpText);
INT_PTR ExGetRangeTextA(HWND hWnd, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefChar, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, char **pText, int nNewLine, BOOL bFillSpaces);
INT_PTR ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, wchar_t **wpText, int nNewLine, BOOL bFillSpaces);
BOOL FreeText(LPVOID pText);
BOOL PasteInEditAsRichEdit(HWND hWnd, int nMaxLenght);
int PasteCase(HWND hWnd, BOOL bAnsi);
void ShowStandardViewMenu(HWND hWnd, HMENU hMenu, BOOL bMouse);

BOOL CALLBACK GoToDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL GoTo(DWORD dwGotoType, const wchar_t *wpString);

RECENTFILE* RecentFilesInsert(RECENTFILESTACK *hStack, int nIndex);
void RecentFilesDelete(RECENTFILESTACK *hStack, RECENTFILE *lpRecentFile);
void RecentFilesZero(RECENTFILESTACK *hStack);
RECENTFILE* RecentFilesFindByName(const wchar_t *wpFile, int *lpIndex);
RECENTFILE* RecentFilesFindByIndex(int nIndex);
RECENTFILE* RecentFilesUpdate(const wchar_t *wpFile);
void RecentFilesRefresh(RECENTFILESTACK *hStack);
int RecentFilesDeleteOld(RECENTFILESTACK *hStack);
int RecentFilesRead(RECENTFILESTACK *hStack);
void RecentFilesSave(RECENTFILESTACK *hStack);
void RecentFilesSaveFile(FRAMEDATA *lpFrame);
void RecentFilesMenu();
RECENTFILEPARAM* StackRecentFileParamAdd(RECENTFILE *lpRecentFile);
RECENTFILEPARAM* StackRecentFileParamGetByName(RECENTFILE *lpRecentFile, const wchar_t *wpParamName);
void StackRecentFileParamDelete(RECENTFILEPARAM *lpRecentFileParam);
void StackRecentFileParamFree(RECENTFILE *lpRecentFile);
int TrimPath(wchar_t *wszResult, const wchar_t *wpPath, int nMax);
int FixAmpW(const wchar_t *wpInput, wchar_t *wszOutput, int nOutputMax);

void LanguageMenu();

void FillMenuPopupCodepage();
void ShowMenuPopupCodepage(POINT *ptScreen, BOOL bMouse);

void RecodeTextW(FRAMEDATA *lpFrame, HWND hWndPreview, DWORD dwFlags, int *nCodePageFrom, int *nCodePageTo);
BOOL CALLBACK RecodeDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ColorsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxThemes(HWND hWnd);
COLORTHEME* StackThemeAdd(HSTACK *hStack, const wchar_t *wpName, AECOLORS *aec, const wchar_t *wpFile, int nBkImageAlpha, int nIndex);
COLORTHEME* StackThemeGetByName(HSTACK *hStack, const wchar_t *wpName);
COLORTHEME* StackThemeGetByData(HSTACK *hStack, AECOLORS *aec, const wchar_t *wpBkImageFile, int nBkImageAlpha);
void StackThemeDelete(HSTACK *hStack, COLORTHEME *ctElement);
void StackThemeFree(HSTACK *hStack);
void ReadThemes(MAINOPTIONS *mo);
BOOL SaveThemes(int nSaveSettings);

BKIMAGEITEM* StackBkImageInsert(HSTACK *hStack, const wchar_t *wpFile);
BKIMAGEITEM* StackBkImageGet(HSTACK *hStack, const wchar_t *wpFile);
void StackBkImageFree(HSTACK *hStack);
BOOL SetBkImage(FRAMEDATA *lpFrame, const wchar_t *wpFile, int nBkImageAlpha);
HBITMAP LoadPictureFile(wchar_t *wpFile);

void RegisterPluginsHotkeys(MAINOPTIONS *mo);
PLUGINFUNCTION* StackPluginFind(HSTACK *hStack, const wchar_t *wpPluginFunction, int nPluginFunctionLen);
PLUGINFUNCTION* StackHotkeyFind(HSTACK *hStack, WORD wHotkey);
PLUGINFUNCTION* StackPluginAdd(HSTACK *hStack, const wchar_t *wpString, int nStringLen, WORD wHotkey, BOOL bOnStart, PLUGINPROC PluginProc, void *lpParameter);
void StackPluginDelete(HSTACK *hStack, PLUGINFUNCTION *pfElement);
BOOL StackPluginSave(HSTACK *hStack, int nSaveSettings);
void StackPluginCleanUp(HSTACK *hStack, BOOL bDeleteNonExistentDLL);
void StackPluginFree(HSTACK *hStack);
void CallPluginsOnStart(HSTACK *hStack);
int CallPluginSend(PLUGINFUNCTION **ppfElement, PLUGINCALLSENDW *pcs, DWORD dwFlags);
int CallPlugin(PLUGINFUNCTION *lpPluginFunction, PLUGINCALLSENDW *pcs, DWORD dwFlags);
DWORD TranslateMessageAll(DWORD dwType, LPMSG lpMsg);
BOOL TranslateMessageGlobal(LPMSG lpMsg);
BOOL TranslateMessagePlugin(LPMSG lpMsg);
int TranslateMessageHotkey(HSTACK *hStack, LPMSG lpMsg);
BOOL ParsePluginNameW(const wchar_t *wpFullName, wchar_t *wszPlugin, wchar_t *wszFunction);
BOOL IsMainFunctionW(const wchar_t *wpFunction);
int CheckHotkey(WORD wHotkey, wchar_t *wszHotkeyOwner);

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

int MessageBoxCustom(HWND hWndParent, const wchar_t *wpText, const wchar_t *wpCaption, UINT uType, HICON hIcon, BUTTONMESSAGEBOX *bmb);
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
DWORD StackFrameGetIndex(HSTACK *hStack, FRAMEDATA *lpFramePointer);
FRAMEDATA* StackFrameIsValid(HSTACK *hStack, FRAMEDATA *lpFramePointer);
void StackFrameMove(HSTACK *hStack, FRAMEDATA *lpFrame, int nIndex);
void StackFrameDelete(HSTACK *hStack, FRAMEDATA *lpFrame);
void StackFramesFree(HSTACK *hStack);

BUTTONDRAWITEM* StackButtonDrawInsert(HSTACK *hStack);
BUTTONDRAWITEM* StackButtonDrawGet(HSTACK *hStack, HWND hWnd);
void StackButtonDrawDelete(HSTACK *hStack, BUTTONDRAWITEM *lpButtonDraw);
void StackButtonDrawFree(HSTACK *hStack);
void SetButtonDraw(HWND hWndButton, BUTTONDRAW *bd);

RECENTCARETITEM* StackRecentCaretInsert(STACKRECENTCARET *hStack);
void StackRecentCaretFree(STACKRECENTCARET *hStack);
void RecentCaretSet(AENSELCHANGE *aensc);

void SetSelectionStatus(AEHDOC hDocEdit, HWND hWndEdit, AECHARRANGE *cr, AECHARINDEX *ci);
void SetModifyStatus(FRAMEDATA *lpFrame, BOOL bState);
void SetOvertypeStatus(FRAMEDATA *lpFrame, BOOL bState);
void SetNewLineStatus(FRAMEDATA *lpFrame, int nState, DWORD dwFlags);
void SetCodePageStatus(FRAMEDATA *lpFrame, int nCodePage, BOOL bBOM);
void UpdateStatusUser(FRAMEDATA *lpFrame, DWORD dwFlags);
void SetTextStatusUser(FRAMEDATA *lpFrame, DWORD dwFlags);
DWORD TranslateStatusUser(FRAMEDATA *lpFrame, const wchar_t *wpString, int nStringLen, wchar_t *wszBuffer, int nBufferSize);
int* SetStatusParts(STACKSTATUSPART *lpStatusStack);
STATUSPART* StackStatusPartInsert(STACKSTATUSPART *hStack);
void StackStatusPartFree(STACKSTATUSPART *hStack);

const wchar_t* GetAssociatedIconW(const wchar_t *wpFile, wchar_t *wszIconFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
void AssociateFileTypesW(HINSTANCE hInstance, const wchar_t *wpFileTypes, DWORD dwFlags);
ASSOCICON* StackIconInsert(STACKASSOCICON *hStack, const wchar_t *wpFile, int nFileLen);
ASSOCICON* StackIconGet(STACKASSOCICON *hStack, const wchar_t *wpFile, int nFileLen, const wchar_t *wpExt);
void StackIconsFree(STACKASSOCICON *hStack);

HFONT SetChosenFont(HWND hWnd, const LOGFONTW *lfFont);
FONTITEM* StackFontItemInsert(HSTACK *hStack, const LOGFONTW *lfFont);
FONTITEM* StackFontItemGet(HSTACK *hStack, const LOGFONTW *lfFont);
void StackFontItemsFree(HSTACK *hStack);

HANDLE MemCreate(const char *pName, DWORD dwSize);
void* MemMap(HANDLE hMem, DWORD dwSize);
BOOL MemUnmap(void *lpMem);
BOOL MemClose(HANDLE hMem);

BOOL AkelAdminInit(const wchar_t *wpFile);
BOOL AkelAdminSend(int nAction, const wchar_t *wpFile);
void AkelAdminExit();

wchar_t* GetCommandLineParamsWide(const unsigned char *pCmdParams, wchar_t **wppCmdParamsStart, wchar_t **wppCmdParamsEnd);
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg, BOOL bParseAsNotepad);
int ParseCmdLine(const wchar_t **wppCmdLine, int nType);
void SendCmdLine(HWND hWnd, const wchar_t *wpCmdLine, BOOL bPost, BOOL bQuitAsEnd);
DWORD CallMethod(const wchar_t *wpMethod, const wchar_t *wpUrlLink);
void ParseMethodParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpText, const wchar_t **wppText);
void ExpandMethodParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpFile, const wchar_t *wpExeDir, const wchar_t *wpUrlLink);
int StructMethodParameters(STACKEXTPARAM *hParamStack, unsigned char *lpStruct);
EXTPARAM* GetMethodParameter(STACKEXTPARAM *hParamStack, int nIndex);
void FreeMethodParameters(STACKEXTPARAM *hParamStack);
INT_PTR TranslateEscapeString(FRAMEDATA *lpFrame, const wchar_t *wpInput, wchar_t *wszOutput, DWORD *lpdwCaret);

BOOL SetFrameInfo(FRAMEDATA *lpFrame, int nType, UINT_PTR dwData);
BOOL GetEditInfo(HWND hWnd, EDITINFO *ei);
DWORD IsEditActive(HWND hWnd);
void UpdateShowHScroll(FRAMEDATA *lpFrame);
int SaveLineScroll(HWND hWnd);
void RestoreLineScroll(HWND hWnd, int nBeforeLine);
DWORD ScrollCaret(HWND hWnd);
BOOL SelectColorDialog(HWND hWndOwner, COLORREF *crColor);
void SetMarker(FRAMEDATA *lpFrame, DWORD dwPos);
void SetWordWrap(FRAMEDATA *lpFrame, DWORD dwType, DWORD dwLimit);
void SetMargins(HWND hWnd, const RECT *lprcNewMargins, const RECT *lprcOldMargins);
void SetTabStops(HWND hWnd, int nTabStops, BOOL bSetRedraw);
BOOL InsertTabStop(HWND hWnd);
BOOL IndentTabStop(HWND hWnd, int nAction);
BOOL AutoIndent(HWND hWnd, AECHARRANGE *cr);
int SetUrlPrefixes(HWND hWnd, const wchar_t *wpPrefixes);
BOOL IsReadOnly(HWND hWnd);
int IsFile(const wchar_t *wpFile);
BOOL IsPathFull(const wchar_t *wpPath);
BOOL GetFileWin32Data(const wchar_t *wpFile, WIN32_FIND_DATAW *wfd);
int GetFileDir(const wchar_t *wpFile, int nFileLen, wchar_t *wszFileDir, DWORD dwFileDirMax);
BOOL GetFullName(const wchar_t *wpFile, wchar_t *wszFileFullName, int nFileMax, int *lpnFileLen);
const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen);
int GetBaseName(const wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen);
const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen);
int GetFileStreamOffset(const wchar_t *wpFile, int nFileLen);
const wchar_t* GetNewLineString(int nNewLine);
void GetTimeString(const wchar_t *wpFormat, wchar_t *wszOutput, BOOL bWithoutSeconds);
BOOL GetFileWriteTimeWide(const wchar_t *wpFile, FILETIME *ft);
BOOL GetFileVersionA(const char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
BOOL GetFileVersionW(const wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2);
int TranslateFileString(const wchar_t *wpCommand, wchar_t *wszBuffer, int nBufferSize);
void TrimModifyState(wchar_t *wszFile, int nFileLen);
void UpdateAsterisk(FRAMEDATA *lpFrame, BOOL bModified);
void SetMouseCapture(HWND hWnd, DWORD dwType);
void ReleaseMouseCapture(DWORD dwType);
BOOL SwitchLayout(HWND hWndEdit, AECHARINDEX *lpciCaret);
int DetectCharLayout(int nChar);
void ActivateKeyboard(HKL dwInputLocale);
void ActivateWindow(HWND hWnd);
HWND FindAkelCopy();
BOOL CALLBACK EnumAkelCopyProc(HWND hWnd, LPARAM lParam);
int GetAkelPadExe(HWND hWnd, wchar_t *szExeFile, int nExeFileMax);
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
void UpdateTitle(FRAMEDATA *lpFrame);
void UpdateTabs(HWND hWnd);
int InsertTabItem(HWND hWnd, int nIndex, LPARAM lParam);
LPARAM GetTabParamFromItem(HWND hWnd, int nItem);
int GetTabItemFromParam(HWND hWnd, LPARAM lParam);
int GetTabItemFromCursorPos(HWND hWnd);
int GetTabItemForDrop(HWND hWnd, POINT *rcScreen);
int SelectTabItem(HWND hWnd, int nIndex);
int MoveTabItem(HWND hWnd, int nIndexOld, int nIndexNew);
BOOL DeleteTabItem(HWND hWnd, int nIndex);
void FreeMemorySearch();
int BytesInString(const wchar_t *wpString);
const wchar_t* FindArrayByIndex(const wchar_t *wpString, int nIndex);
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
