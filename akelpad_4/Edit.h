#ifndef __EDIT_H__
#define __EDIT_H__


//// Defines

#define APP_MAIN_CLASSA              "AkelPad4"
#define APP_MAIN_CLASSW             L"AkelPad4"
#define APP_MAIN_TITLEA              "AkelPad"
#define APP_MAIN_TITLEW             L"AkelPad"
#define APP_SDI_TITLEA               ""
#define APP_SDI_TITLEW              L""
#define APP_SDI_CLASSA               "AkelPad SDI Class"
#define APP_SDI_CLASSW              L"AkelPad SDI Class"
#define APP_MDI_CLASSA               "AkelPad MDI Class"
#define APP_MDI_CLASSW              L"AkelPad MDI Class"
#define APP_PRINTPREVIEW_CLASSA      "AkelPad Print Preview"
#define APP_PRINTPREVIEW_CLASSW     L"AkelPad Print Preview"
#define APP_MUTEXA                   "AkelPad Mutex"
#define APP_MUTEXW                  L"AkelPad Mutex"
#define APP_REGHOMEA                 "Software\\Akelsoft\\AkelPad"
#define APP_REGHOMEW                L"Software\\Akelsoft\\AkelPad"
#define APP_ABOUT_VERSIONA           "AkelPad 4.5.0"
#define APP_ABOUT_VERSIONW          L"AkelPad 4.5.0"
#define APP_ABOUT_HOMEPAGEA          "http://akelpad.sf.net"
#define APP_ABOUT_HOMEPAGEW         L"http://akelpad.sf.net"
#define APP_ABOUT_EMAIL_SHENGALTSA   "shengalts@mail.ru"
#define APP_ABOUT_EMAIL_SHENGALTSW  L"shengalts@mail.ru"
#define APP_ABOUT_EMAIL_KUZNETSOVA   "developer@akelpad.net.ru"
#define APP_ABOUT_EMAIL_KUZNETSOVW  L"developer@akelpad.net.ru"

//Code pages strings
#define STR_UNICODE_UTF16LEA   "1200  (UTF-16 LE)"
#define STR_UNICODE_UTF16LEW  L"1200  (UTF-16 LE)"
#define STR_UNICODE_UTF16BEA   "1201  (UTF-16 BE)"
#define STR_UNICODE_UTF16BEW  L"1201  (UTF-16 BE)"
#define STR_UNICODE_UTF8A      "65001 (UTF-8)"
#define STR_UNICODE_UTF8W     L"65001 (UTF-8)"
#define STR_UNICODE_UTF7A      "65000 (UTF-7)"
#define STR_UNICODE_UTF7W     L"65000 (UTF-7)"
#define STR_UNICODE_UTF32LEA   "12000  (UTF-32 LE)"
#define STR_UNICODE_UTF32LEW  L"12000  (UTF-32 LE)"
#define STR_UNICODE_UTF32BEA   "12001  (UTF-32 BE)"
#define STR_UNICODE_UTF32BEW  L"12001  (UTF-32 BE)"
#define STR_NOBOMA             "  *BOM"
#define STR_NOBOMW            L"  *BOM"

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
#ifndef BIF_NEWDIALOGSTYLE
  #define BIF_NEWDIALOGSTYLE 0x0040
#endif
#ifndef PROPSHEETHEADERA_V1_SIZE
  #define PROPSHEETHEADERA_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERA, pfnCallback)
#endif
#ifndef PROPSHEETHEADERW_V1_SIZE
  #define PROPSHEETHEADERW_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERW, pfnCallback)
#endif
#ifndef mod
  #define mod(a)  ((((int)(a)) < 0) ? (0 - ((int)(a))) : (a))
#endif

#define BUFFER_SIZE                1024
#define OPENFILELIST_SIZE          32768
#define TRANSLATE_BUFFER_SIZE      8192
#define PREVIEW_SIZE               8188  //4094*2; -1 preview all file
#define PUTFIND_MAXSEL             16384
#define DETECT_CODEPAGE_SIZE       1024
#define RECENTFILES_AMOUNT         10
#define RECENTFILES_RECORD_LENGTH  90
#define SEARCHSTRINGS_AMOUNT       10
#define EDIT_TABSTOPS              8
#define EDIT_UNDOLIMIT             1000
#define EDIT_MARGINS               MAKELONG(4, 4)

#define STR_DEFAULTSAVEEXTA            "txt"
#define STR_DEFAULTSAVEEXTW           L"txt"
#define STR_ASSOCIATE_OPENA            "txt;log;ini;inf"
#define STR_ASSOCIATE_OPENW           L"txt;log;ini;inf"
#define STR_ASSOCIATE_EDITA            "bat;cmd;reg;htm;html"
#define STR_ASSOCIATE_EDITW           L"bat;cmd;reg;htm;html"
#define STR_ASSOCIATE_PRINTA           "txt;log;ini;inf"
#define STR_ASSOCIATE_PRINTW          L"txt;log;ini;inf"
#define STR_WORD_DELIMITERSW          L" \t\n'`\"\\|[](){}<>,.;:+-=~!@#$%^&*/?"
#define STR_WRAP_DELIMITERSW          L" \t"
#define STR_URL_LEFTDELIMITERSW       L" \t\n'`\"(<{[="
#define STR_URL_RIGHTDELIMITERSW      L" \t\n'`\")>}]"
#define STR_URL_PREFIXESW             L"http: https: www. ftp: file: mailto:"
#define STR_SENTENCE_DELIMITERSA       ".?!"
#define STR_SENTENCE_DELIMITERSW      L".?!"
#define STR_PRINT_HEADERA              "%l***%c<%f>%r***"
#define STR_PRINT_HEADERW             L"%l***%c<%f>%r***"
#define STR_PRINT_FOOTERA              "- %n[1] -"
#define STR_PRINT_FOOTERW             L"- %n[1] -"

//Open file dialog
#define IDC_OFN_EDIT           1152

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
#define PRNC_TEXT                  0x01  //Print colored text
#define PRNC_BACKGROUND            0x02  //Print on colored background

//Language identifiers
#define LANGID_RUSSIAN    0x0419
#define LANGID_ENGLISH    0x0409
#define LANGID_TURKISH    0x041F
#define LANGID_CHINESE    0x0404
#define LANGID_JAPANESE   0x0411
#define LANGID_KOREAN     0x0412

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

//StrReplace options
#define AEFR_WHOLEWORDGOODSTART 0x10000000
#define AEFR_WHOLEWORDGOODEND   0x20000000

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
#define INCREASE_FONT  1
#define DECREASE_FONT  2

//Show modify
#define SM_NONE           0x00000000
#define SM_STATUSBAR      0x00000001
#define SM_MAINTITLE_SDI  0x00000002
#define SM_FRAMETITLE_MDI 0x00000004
#define SM_TABTITLE_MDI   0x00000008

//Status bar position type
#define SPT_COLUMN      0x00000001
#define SPT_LINEWRAP    0x00000002

//Go to line
#define NT_LINE    1
#define NT_OFFSET  2

//Custom statusbar
#define CSB_CHARHEX      0x01
#define CSB_CHARDEC      0x02
#define CSB_RICHOFFSET   0x04
#define CSB_BYTEOFFSET   0x08
#define CSB_FONTPOINT    0x10
#define CSB_TABSIZE      0x20
#define CSB_MARKER       0x40
#define CSB_REPLACECOUNT 0x80

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
  BOOL bKeybLayoutMDI;
  BOOL bDateLog;
  BOOL bSaveInReadOnlyMsg;
  wchar_t wszDefaultSaveExt[MAX_PATH];

  //Search dialog
  DWORD dwSearchOptions;

  //Open file dialog
  wchar_t wszLastDir[MAX_PATH];

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
  wchar_t *wszSection;
  int nSectionBytes;
  INIKEY *first;
  INIKEY *last;
} INISECTION;

typedef struct {
  INISECTION *first;
  INISECTION *last;
} INIFILE;

typedef struct _FILESTREAMDATA {
  HWND hWnd;
  HANDLE hFile;
  int nCodePage;
  DWORD dwFlags;
  int nNewLine;
  int nBytesCurrent;
  int nBytesMax;
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
} ASSOCICON;

typedef struct {
  HANDLE hThread;
  HDROP hDrop;
} DROPFILESTHREAD;

typedef struct {
  BOOL bModified;
  BOOL bReadOnly;
  BOOL bOvertypeMode;
  int nNewLine;
  int nCodePage;
  BOOL bBOM;
} STATUSSTATE;

typedef struct {
  int nCodePage;
  BOOL bBOM;
  BOOL bResult;
} DIALOGCODEPAGE;


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
LRESULT SendToDoc(AEHDOC hDocEdit, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL DoFileNew();
BOOL CloseDocument();
HWND DoFileNewWindow(DWORD dwAddFlags);
BOOL CALLBACK EnumThreadWindowsProc(HWND hwnd, LPARAM lParam);
BOOL DoFileOpen();
int DoFileReopenAs(DWORD dwFlags, int nCodePage, BOOL bBOM);
BOOL DoFileSave();
BOOL SaveChanged();
BOOL DoFileSaveAs(int nDialogCodePage, BOOL bDialogBOM);
void DoFileSaveAllAs();
BOOL DoFilePageSetup(HWND hWndOwner);
int DoFilePrint(FRAMEDATA *lpFrame, BOOL bSilent);
void DoFilePreview(HWND hWnd);
void DoEditUndo(HWND hWnd);
void DoEditRedo(HWND hWnd);
void DoEditCut(HWND hWnd);
void DoEditCopy(HWND hWnd);
BOOL DoEditPaste(HWND hWnd, BOOL bAnsi);
void DoEditClear(HWND hWnd);
void DoEditSelectAll(HWND hWnd);
void DoEditInsertDate(HWND hWnd);
void DoEditInsertChar();
void DoEditRecode(HWND hWnd);
BOOL DoEditInsertStringInSelectionW(HWND hWnd, int nAction, wchar_t *wpString);
BOOL DoEditDeleteFirstCharW(HWND hWnd);
BOOL DoEditDeleteTrailingWhitespacesW(HWND hWnd);
BOOL DoEditChangeCaseW(HWND hWnd, int nCase);
void DoEditFind();
int DoEditFindNextDown(HWND hWnd);
int DoEditFindNextUp(HWND hWnd);
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
void DoSettingsPlugins(HWND hWnd);
void DoSettingsOptionsA();
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
INISECTION* StackOpenIniSectionA(INIFILE *hIniFile, char *pSection, int nSectionLen, BOOL bCreate);
INISECTION* StackOpenIniSectionW(INIFILE *hIniFile, wchar_t *wpSection, int nSectionLen, BOOL bCreate);
INIKEY* StackOpenIniKeyA(INISECTION *lpIniSection, char *pKey, int nKeyLen, BOOL bCreate);
INIKEY* StackOpenIniKeyW(INISECTION *lpIniSection, wchar_t *wpKey, int nKeyLen, BOOL bCreate);
int StackGetIniData(INIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL StackSetIniData(INIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
int IniGetValueA(INIFILE *hIniFile, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
int IniGetValueW(INIFILE *hIniFile, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueA(INIFILE *hIniFile, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueW(INIFILE *hIniFile, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
void StackDeleteIniKey(INISECTION *lpIniSection, INIKEY *lpIniKey);
void StackDeleteIniSection(INIFILE *hIniFile, INISECTION *lpIniSection, BOOL bOnlyClear);
void StackFreeIni(INIFILE *hIniFile);

DWORD ReadOption(OPTIONHANDLE *oh, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
DWORD SaveOption(OPTIONHANDLE *oh, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
void ReadOptions(MAINOPTIONS *mo, FRAMEDATA *fd);
BOOL SaveOptions(MAINOPTIONS *mo, FRAMEDATA *fd, int nSaveSettings, BOOL bForceWrite);

int OpenDocument(HWND hWnd, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
void FileStreamIn(FILESTREAMDATA *lpData);
DWORD CALLBACK InputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufBytesLen, DWORD *dwBufBytesDone);
DWORD ReadFileContent(HANDLE hFile, DWORD dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wpContent);
BOOL OpenDocumentSend(HWND hWnd, HWND hWndEditCtrl, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess);
int SaveDocument(HWND hWnd, const wchar_t *wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
void FileStreamOut(FILESTREAMDATA *lpData);
DWORD CALLBACK OutputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufBytesLen, DWORD *dwBufBytesDone);
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
int FilePreview(HWND hWnd, wchar_t *wpFile, int nPreviewBytes, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
int AutodetectCodePage(const wchar_t *wpFile, DWORD dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
BOOL AutodetectMultibyte(DWORD dwLangID, unsigned char *pBuffer, DWORD dwBytesToCheck, int *nCodePage);
unsigned int UTF32toUTF16(const unsigned long *pSource, unsigned int nSourceLen, unsigned int *nSourceDone, unsigned short *szTarget, unsigned int nTargetMax);
unsigned int UTF16toUTF32(const unsigned short *pSource, unsigned int nSourceLen, unsigned int *nSourceDone, unsigned long *szTarget, unsigned int nTargetMax);
unsigned int UTF16toUTF8(const unsigned short *pSource, unsigned int nSourceLen, unsigned int *nSourceDone, unsigned char *szTarget, unsigned int nTargetMax);
unsigned int UTF8toUTF16(const unsigned char *pSource, unsigned int nSourceLen, unsigned int *nSourceDone,  unsigned short *szTarget, unsigned int nTargetMax);
void ChangeTwoBytesOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
void ChangeFourBytesOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
BOOL IsCodePageUnicode(int nCodePage);
BOOL IsCodePageValid(int nCodePage);
unsigned int TranslateNewLinesToUnixW(wchar_t *wszWideString, unsigned int nWideStringLen);

void RegReadSearch();
BOOL CALLBACK FindAndReplaceDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewComboboxEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxSearch(HWND hWndFind, HWND hWndReplace);
int GetComboboxSearchText(HWND hWnd, wchar_t **wszText_orig, wchar_t **wszText, int nNewLine);
void SaveComboboxSearch(HWND hWndFind, HWND hWndReplace);
int FindTextW(HWND hWnd, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen);
int ReplaceTextW(HWND hWnd, DWORD dwFlags, const wchar_t *wpFindIt, int nFindItLen, const wchar_t *wpReplaceWith, int nReplaceWithLen, BOOL bAll, int *nReplaceCount);
int StrReplaceW(const wchar_t *wpText, int nTextLen, const wchar_t *wpIt, int nItLen, const wchar_t *wpWith, int nWithLen, DWORD dwFlags, wchar_t *wszResult, int *nResultLen, int *nMin, int *nMax, int *nFirstVisible);
int EscapeStringToEscapeDataW(const wchar_t *wpInput, wchar_t *wszOutput, int nNewLine);
void EscapeDataToEscapeStringW(const wchar_t *wpInput, wchar_t *wszOutput);

void GetSel(HWND hWnd, AECHARRANGE *crSel, BOOL *bColumnSel, AECHARINDEX *ciCaret);
void SetSel(HWND hWnd, AECHARRANGE *crSel, DWORD dwFlags, AECHARINDEX *ciCaret);
void ReplaceSelA(HWND hWnd, const char *pData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
void ReplaceSelW(HWND hWnd, const wchar_t *wpData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
int IndexSubtract(HWND hWnd, AECHARINDEX *ciChar1, AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel);
int IndexOffset(HWND hWnd, AECHARINDEX *ciChar, int nOffset, int nNewLine);
int AkelIndexToRichOffset(HWND hWnd, AECHARINDEX *ciChar);
void RichOffsetToAkelIndex(HWND hWnd, int nOffset, AECHARINDEX *ciChar);
int GetTextLength(HWND hWnd);
int GetRangeTextA(HWND hWnd, int nMin, int nMax, char **pText);
int GetRangeTextW(HWND hWnd, int nMin, int nMax, wchar_t **wpText);
int ExGetRangeTextA(HWND hWnd, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefChar, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, char **pText, int nNewLine, BOOL bFillSpaces);
int ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, wchar_t **wpText, int nNewLine, BOOL bFillSpaces);
BOOL FreeText(LPVOID pText);
BOOL PasteInEditAsRichEdit(HWND hWnd);
BOOL ColumnPaste(HWND hWnd);
BOOL PasteAfter(HWND hWnd, BOOL bAnsi);

BOOL CALLBACK GoToDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL RecentFilesAlloc();
void RecentFilesZero();
int RecentFilesRead();
BOOL RecentFilesGet(const wchar_t *wpFile, int *nPosition, int *nCodePage);
BOOL RecentFilesUpdate(const wchar_t *wpFile, int nPosition, int nCodePage);
void RecentFilesSaveCurrentFile();
int RecentFilesDeleteOld();
void RecentFilesSave();
void RecentFilesMenu();
void FreeMemoryRecentFiles();
int TrimPathW(wchar_t *wszResult, const wchar_t *wpPath, int nMax);
int FixAmpW(const wchar_t *wpInput, wchar_t *wszOutput, int nOutputMax);

void LanguageMenu();

void FillMenuPopupCodepage();
void ShowMenuPopupCodepage(POINT *pt);

void RecodeTextW(HWND hWnd, int nCodePageFrom, int nCodePageTo);
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
BOOL TranslateGlobal(LPMSG lpMsg);
BOOL TranslatePlugin(LPMSG lpMsg);
int TranslateHotkey(HSTACK *hStack, LPMSG lpMsg);
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

LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam);
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

DOCK* StackDockAdd(HDOCK *hDocks, DOCK *dkData);
int DockSetSide(HDOCK *hDocks, DOCK *dkData, int nSide);
void StackDockDelete(HDOCK *hDocks, DOCK *dkData);
DOCK* StackDockFindWindow(HDOCK *hDocks, HWND hWnd, BOOL bChild);
HWND StackDockNextWindow(HDOCK *hDocks, DOCK *dkData, BOOL bPrevious);
DOCK* StackDockFromPoint(HDOCK *hDocks, POINT *pt);
void StackDockSize(HDOCK *hDocks, int nSide, NSIZE *ns);
BOOL StackDockUpdateCheck(HDOCK *hDocks);
void StackDockFree(HDOCK *hDocks);
BOOL TranslateDialog(HDOCK *hDocks, LPMSG lpMsg);

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

void SetSelectionStatus(AEHDOC hDocEdit, HWND hWndEdit, AECHARRANGE *cr, AECHARINDEX *ci);
void SetModifyStatus(FRAMEDATA *lpFrame, BOOL bState);
void SetOvertypeStatus(FRAMEDATA *lpFrame, BOOL bState);
void SetNewLineStatus(FRAMEDATA *lpFrame, int nState, DWORD dwFlags);
void SetCodePageStatus(FRAMEDATA *lpFrame, int nCodePage, BOOL bBOM);
void UpdateStatusUser(FRAMEDATA *lpFrame, DWORD dwFlags);
DWORD TranslateStatusUser(FRAMEDATA *lpFrame, const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);

const wchar_t* GetAssociatedIconW(const wchar_t *wpFile, wchar_t *wszIconFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
void AssociateFileTypesW(HINSTANCE hInstance, const wchar_t *wpFileTypes, DWORD dwFlags);
ASSOCICON* StackIconInsert(HSTACK *hStack, const wchar_t *wpFile, int nFileLen);
ASSOCICON* StackIconGet(HSTACK *hStack, const wchar_t *wpFile, int nFileLen, const wchar_t *wpExt);
void StackIconsFree(HSTACK *hStack);

HFONT SetChosenFont(HWND hWnd, const LOGFONTW *lfFont);
int GetFontPoint(HWND hWnd, const LOGFONTW *lfFont);
FONTITEM* StackFontItemInsert(HSTACK *hStack, const LOGFONTW *lfFont);
FONTITEM* StackFontItemGet(HSTACK *hStack, const LOGFONTW *lfFont);
void StackFontItemsFree(HSTACK *hStack);

wchar_t* GetCommandLineWide(void);
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg, BOOL bParseAsNotepad);
int ParseCmdLine(const wchar_t **wppCmdLine, BOOL bOnLoad);
void SendCmdLine(HWND hWnd, const wchar_t *wpCmdLine, BOOL bPost);
void GetMethodParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpText, const wchar_t **wppText);
void SetParametersExpChar(STACKEXTPARAM *hParamStack, const wchar_t *wpFile, const wchar_t *wpExeDir);
int CreateParametersStruct(STACKEXTPARAM *hParamStack, unsigned char **lppStruct);
int GetParameterInt(STACKEXTPARAM *hParamStack, int nIndex);
char* GetParameterCharA(STACKEXTPARAM *hParamStack, int nIndex);
wchar_t* GetParameterCharW(STACKEXTPARAM *hParamStack, int nIndex);
char* GetParameterExpCharA(STACKEXTPARAM *hParamStack, int nIndex);
wchar_t* GetParameterExpCharW(STACKEXTPARAM *hParamStack, int nIndex);
int TranslateEscapeString(FRAMEDATA *lpFrame, const wchar_t *wpInput, wchar_t *wszOutput);
void FreeMethodParameters(STACKEXTPARAM *hParamStack);

BOOL GetEditInfo(HWND hWnd, EDITINFO *ei);
BOOL SetCurEditOption(int nType, DWORD dwData);
DWORD IsEditActive(HWND hWnd);
void UpdateShowHScroll(FRAMEDATA *lpFrame);
int SaveLineScroll(HWND hWnd);
void RestoreLineScroll(HWND hWnd, int nBeforeLine);
DWORD ScrollCaret(HWND hWnd);
BOOL SelectColorDialogA(HWND hWndOwner, COLORREF *crColor);
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
void TrimModifyState(wchar_t *wszFile);
void GetTimeString(const wchar_t *wpFormat, wchar_t *wszOutput, BOOL bWithoutSeconds);
BOOL GetFileWriteTimeWide(const wchar_t *wpFile, FILETIME *ft);
BOOL GetFileVersionA(char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
BOOL GetFileVersionW(wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild, DWORD *dwLanguage);
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2);
int TranslateFileString(const wchar_t *wpCommand, wchar_t *wszBuffer, int nBufferSize);
void SetMouseCapture(HWND hWnd, DWORD dwType);
void ReleaseMouseCapture(DWORD dwType);
void ActivateKeyboard(DWORD dwInputLocale);
void ActivateWindow(HWND hWnd);
HWND NextDialog(BOOL bPrevious);
HWND NextClone(BOOL bPrevious);
void UpdateSize();
BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcInit, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetMovingRect(DOCK *dkData, POINT *pt, MINMAXINFO *mmi, RECT *rcScreen);
void DrawMovingRect(RECT *rcScreen);
int GetMouseEdge(HWND hWnd, POINT *pt);
void SetEdgeCursor(int nEdge);
int GetOppEdge(int nEdge);
BOOL IsCursorOnWindow(HWND hWnd);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL ScreenToClientRect(HWND hWnd, RECT *rc);
BOOL ClientToScreenRect(HWND hWnd, RECT *rc);
BOOL EnsureWindowInMonitor(RECT *rcWindow);
void UpdateTitle(FRAMEDATA *lpFrame, const wchar_t *wszFile);
void UpdateTabs(HWND hWnd);
int AddTabItem(HWND hWnd, HICON hIcon, LPARAM lParam);
LPARAM GetTabParamFromItem(HWND hWnd, int nItem);
int GetTabItemFromParam(HWND hWnd, LPARAM lParam);
int GetTabItemFromCursorPos(HWND hWnd);
int GetTabItemForDrop(HWND hWnd, POINT *pt);
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
HWND API_CreateDialogA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
HWND API_CreateDialogW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBoxA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBoxW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBoxParamA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
INT_PTR API_DialogBoxParamW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
HANDLE API_CreateFileA(const char *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE API_CreateFileW(const wchar_t *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL API_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL API_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
LPVOID API_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
BOOL API_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
wchar_t* AllocWideStr(DWORD dwSize);
BOOL FreeWideStr(wchar_t *wpVar);

#endif
