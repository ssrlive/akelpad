#ifndef __EDIT_H__
#define __EDIT_H__

//// Defines

#define APP_MAIN_CLASSA              "AkelPad4"
#define APP_MAIN_CLASSW             L"AkelPad4"
#define APP_MAIN_TITLEA              "AkelPad"
#define APP_MAIN_TITLEW             L"AkelPad"
#define APP_SDI_CLASSA               "AkelPad SDI Class"
#define APP_SDI_CLASSW              L"AkelPad SDI Class"
#define APP_SDI_TITLEA               ""
#define APP_SDI_TITLEW              L""
#define APP_MDI_CLASSA               "AkelPad MDI Class"
#define APP_MDI_CLASSW              L"AkelPad MDI Class"
#define APP_MUTEXA                   "AkelPad Mutex"
#define APP_MUTEXW                  L"AkelPad Mutex"
#define APP_ABOUT_VERSIONA           "AkelPad 4.0.0 alpha 3"
#define APP_ABOUT_VERSIONW          L"AkelPad 4.0.0 alpha 3"
#define APP_ABOUT_HOMEPAGEA          "http://akelpad.sf.net"
#define APP_ABOUT_HOMEPAGEW         L"http://akelpad.sf.net"
#define APP_ABOUT_EMAIL_SHENGALTSA   "shengalts@mail.ru"
#define APP_ABOUT_EMAIL_SHENGALTSW  L"shengalts@mail.ru"
#define APP_ABOUT_EMAIL_KUZNETSOVA   "developer@akelpad.net.ru"
#define APP_ABOUT_EMAIL_KUZNETSOVW  L"developer@akelpad.net.ru"

//Code pages strings
#define STR_UNICODE_UCS2_LEA   "1200  (UTF-16 LE)"
#define STR_UNICODE_UCS2_LEW  L"1200  (UTF-16 LE)"
#define STR_UNICODE_UCS2_BEA   "1201  (UTF-16 BE)"
#define STR_UNICODE_UCS2_BEW  L"1201  (UTF-16 BE)"
#define STR_UNICODE_UTF8A      "65001 (UTF-8)"
#define STR_UNICODE_UTF8W     L"65001 (UTF-8)"
#define STR_UNICODE_UTF7A      "65000 (UTF-7)"
#define STR_UNICODE_UTF7W     L"65000 (UTF-7)"

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
  #define mod(a) (((a) < 0)?(0 - (a)):(a))
#endif

#define BUFFER_SIZE                1024
#define FILELIST_BUFFER_SIZE       8192
#define TRANSLATE_BUFFER_SIZE      8192
#define PREVIEW_SIZE               8188  //4094*2; -1 preview all file
#define DELIMITERS_SIZE            128
#define DETECT_CODEPAGE_SIZE       1024
#define RECENTFILES_AMOUNT         10
#define RECENTFILES_RECORD_LENGTH  90
#define SEARCHSTRINGS_AMOUNT       10
#define EDIT_TABSTOPS              8
#define EDIT_UNDOLIMIT             1000
#define EDIT_MARGINS               MAKELONG(4, 4)
#define ASSOCIATE_OPENA            "txt;log;ini;inf"
#define ASSOCIATE_OPENW           L"txt;log;ini;inf"
#define ASSOCIATE_EDITA            "bat;cmd;reg;htm;html"
#define ASSOCIATE_EDITW           L"bat;cmd;reg;htm;html"
#define ASSOCIATE_PRINTA           "txt;log;ini;inf"
#define ASSOCIATE_PRINTW          L"txt;log;ini;inf"
#define WORD_DELIMITERSW          L" \t\r\\|[](){}<>,.;:+-=~!@#$%^&*/?'`\""
#define WORD_WHITESPACESW         L" \t\r"
#define SENTENCE_DELIMITERSA       ".?!"
#define SENTENCE_DELIMITERSW      L".?!"
#define TITLE_DELIMITERSA          " \t\r"
#define TITLE_DELIMITERSW         L" \t\r"
#define PRINT_HEADERA              "%l***%c<%f>%r***"
#define PRINT_HEADERW             L"%l***%c<%f>%r***"
#define PRINT_FOOTERA              "- %n[1] -"
#define PRINT_FOOTERW             L"- %n[1] -"

//Menu positions
#define MENU_FILE_POSITION              0
#define MENU_EDIT_POSITION              1
#define MENU_VIEW_POSITION              2
#define MENU_OPTIONS_POSITION           3
#define MENU_MDI_POSITION               4
#define MENU_ABOUT_POSITION             5
#define MENU_FILE_RECENTFILES_POSITION  11
#define MENU_VIEW_LANGUAGE_POSITION     7

//Open file dialog
#define IDC_OFN_EDIT                   1152

//Print page setup dialog
#define IDC_PSD_PRINTER_BUTTON         1026
#define IDC_PSD_FONT_GROUP             10001
#define IDC_PSD_FONT_CHECK             10002
#define IDC_PSD_FONT_BUTTON            10003
#define IDC_PSD_HEADLINE_GROUP         10004
#define IDC_PSD_HEADLINE_HEADER_CHECK  10004
#define IDC_PSD_HEADLINE_HEADER_EDIT   10005
#define IDC_PSD_HEADLINE_HEADER_HELP   10006
#define IDC_PSD_HEADLINE_FOOTER_CHECK  10007
#define IDC_PSD_HEADLINE_FOOTER_EDIT   10008
#define IDC_PSD_HEADLINE_FOOTER_HELP   10009

//Search dialog
#define IDC_COMBOBOX_EDIT              1001

//WM_INITMENU lParam
#define IMENU_EDIT     0x00000001
#define IMENU_CHECKS   0x00000004

//WM_COPYDATA
#define CD_OPENDOCUMENT   1

//Language identifiers
#define LANGID_ENGLISH    0x0409
#define LANGID_RUSSIAN    0x0419

//Code pages int
#define CP_UNICODE_UCS2_LE  1200
#define CP_UNICODE_UCS2_BE  1201
#define CP_UNICODE_UTF7     65000
#define CP_UNICODE_UTF8     65001
#define CP_KOI8_R           20866

//INI value types
#define INI_DWORD           1
#define INI_BINARY          2
#define INI_STRINGANSI      3
#define INI_STRINGUNICODE   4

//Save type
#define SS_REGISTRY 1
#define SS_INI      2

//New line formats
#define NEWLINE_WIN  0x00000001
#define NEWLINE_UNIX 0x00000002
#define NEWLINE_MAC  0x00000004
#define NEWLINE_MIX  0x00000008

//Autoanswer
#define AUTOANSWER_ASK  0
#define AUTOANSWER_YES  1
#define AUTOANSWER_NO   2

//Dock side
#define DKS_LEFT    1
#define DKS_RIGHT   2
#define DKS_TOP     3
#define DKS_BOTTOM  4

//Dock flags
#define DKF_OWNTHREAD     0x00000001
#define DKF_FIXEDSIZE     0x00000002
#define DKF_DRAGDROP      0x00000004
#define DKF_HIDDEN        0x00000008
#define DKF_NODROPLEFT    0x00000010
#define DKF_NODROPRIGHT   0x00000020
#define DKF_NODROPTOP     0x00000040
#define DKF_NODROPBOTTOM  0x00000080

//Dock action
#define DK_ADD         0x00000001
#define DK_DELETE      0x00000002
#define DK_SUBCLASS    0x00000004
#define DK_UNSUBCLASS  0x00000008
#define DK_SETLEFT     0x00000010
#define DK_SETRIGHT    0x00000020
#define DK_SETTOP      0x00000040
#define DK_SETBOTTOM   0x00000080
#define DK_HIDE        0x00000100
#define DK_SHOW        0x00000200

//STARTUPINFO flags
#define STARTF_NOMUTEX  0x00001000

//Autodetect flags
#define ADT_BINARY_ERROR         0x00000001
#define ADT_REG_CODEPAGE         0x00000002
#define ADT_DETECT_CODEPAGE      0x00000004
#define ADT_DETECT_BOM           0x00000008

//Autodetect errors
#define EDT_OPEN          -1
#define EDT_ALLOC         -2
#define EDT_READ          -3
#define EDT_BINARY        -4

//OpenDocument flags
#define OD_ADT_BINARY_ERROR      0x00000001
#define OD_ADT_REG_CODEPAGE      0x00000002
#define OD_ADT_DETECT_CODEPAGE   0x00000004
#define OD_ADT_DETECT_BOM        0x00000008

//OpenDocument errors
#define EOD_SUCCESS         0
#define EOD_ADT_OPEN        -1
#define EOD_ADT_ALLOC       -2
#define EOD_ADT_READ        -3
#define EOD_ADT_BINARY      -4
#define EOD_OPEN            -5
#define EOD_CANCEL          -6
#define EOD_WINDOW_EXIST    -7
#define EOD_CODEPAGE_ERROR  -8
#define EOD_STOP            -9
#define EOD_STREAMIN        -10

//SaveDocument errors
#define ESD_SUCCESS         0
#define ESD_OPEN            -1
#define ESD_WRITE           -2
#define ESD_READONLY        -3
#define ESD_CODEPAGE_ERROR  -4
#define ESD_STOP            -5
#define ESD_STREAMOUT       -6

//Search/Replace options
#define AEFR_UP          0x00100000
#define AEFR_BEGINNING   0x00200000
#define AEFR_SELECTION   0x00400000
#define AEFR_ESCAPESEQ   0x00800000
#define AEFR_ALLFILES    0x01000000

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

//File types association
#define AE_ASSOCIATE     0x00000001
#define AE_DEASSOCIATE   0x00000002
#define AE_OPEN          0x00000004
#define AE_EDIT          0x00000008
#define AE_PRINT         0x00000010

//Colors list
#define LVI_COLOR_BASIC        0
#define LVI_COLOR_SELECTION    1
#define LVI_COLOR_ACTIVELINE   2
#define LVI_COLOR_CARET        3

#define LVSI_COLOR_ELEMENT     0
#define LVSI_COLOR_TEXT        1
#define LVSI_COLOR_BACKGROUND  2
#define LVSI_COLOR_SAMPLE      3

//Plugins list
#define LVSI_FUNCTION_NAME     0
#define LVSI_FUNCTION_HOTKEY   1
#define LVSI_FUNCTION_STATUS   2

//CallPlugin errors
#define EDL_FAILED                 0
#define EDL_UNLOADED               1
#define EDL_NONUNLOADED_ACTIVE     2
#define EDL_NONUNLOADED_NONACTIVE  3
#define EDL_NONUNLOADED            4

//PLUGINDATA
#define UD_UNLOAD               0
#define UD_NONUNLOAD_ACTIVE     1
#define UD_NONUNLOAD_NONACTIVE  2
#define UD_NONUNLOAD            3

//Options flags
#define POB_READ    0x1
#define POB_SAVE    0x2
#define POB_CLEAR   0x4

//Option type
#define PO_DWORD    1
#define PO_BINARY   2
#define PO_STRING   3

//Save plugins stack
#define DLLS_CLEAR   1
#define DLLS_ONEXIT  2

//NCONTEXTMENU
#define NCM_EDIT    1
#define NCM_TAB     2
#define NCM_STATUS  3


//// Structures

typedef void (CALLBACK *PLUGINPROC)(void *);
typedef void (CALLBACK *WNDPROCRET)(CWPRETSTRUCT *);
typedef BOOL (CALLBACK *EXPORTNAMESPROC)(char *, LPARAM);

typedef struct _HINISECTION {
  struct _HINISECTION *next;
  struct _HINISECTION *prev;
  char *szSection;
  wchar_t *wszSection;
  int nSectionAnsiBytes;
  int nSectionUnicodeBytes;
  HSTACK hSectionStack;
} HINISECTION;

typedef struct _HINIKEY {
  struct _HINIKEY *next;
  struct _HINIKEY *prev;
  char *szKey;
  wchar_t *wszKey;
  int nKeyAnsiBytes;
  int nKeyUnicodeBytes;
  char *szString;
  wchar_t *wszString;
  int nStringAnsiBytes;
  int nStringUnicodeBytes;
} HINIKEY;

typedef struct _OPENDOCUMENTA {
  HWND hWnd;
  char szFile[MAX_PATH];
  char szWorkDir[MAX_PATH];
  DWORD dwFlags;
  int nCodePage;
  BOOL bBOM;
} OPENDOCUMENTA;

typedef struct _OPENDOCUMENTW {
  HWND hWnd;
  wchar_t wszFile[MAX_PATH];
  wchar_t wszWorkDir[MAX_PATH];
  DWORD dwFlags;
  int nCodePage;
  BOOL bBOM;
} OPENDOCUMENTW;

typedef struct _SAVEDOCUMENTA {
  char szFile[MAX_PATH];
  int nCodePage;
  BOOL bBOM;
  BOOL bUpdate;
} SAVEDOCUMENTA;

typedef struct _SAVEDOCUMENTW {
  wchar_t wszFile[MAX_PATH];
  int nCodePage;
  BOOL bBOM;
  BOOL bUpdate;
} SAVEDOCUMENTW;

typedef struct _WNDFRAMEA {
  HICON hIcon;
  char szFile[MAX_PATH];
  int nCodePage;
  BOOL bBOM;
  int nNewLine;
  BOOL bModified;
  BOOL bReadOnly;
  BOOL bWordWrap;
  BOOL bInsertState;
  int nTabStopSize;
  int nUndoLimit;
  BOOL bShowURL;
  DWORD dwEditMargins;
  BOOL bDelimitersEnable;
  COLORREF crFont;
  COLORREF crBackground;
  FILETIME ft;
  LOGFONTA lf;
} WNDFRAMEA;

typedef struct _WNDFRAMEW {
  HICON hIcon;
  wchar_t wszFile[MAX_PATH];
  int nCodePage;
  BOOL bBOM;
  int nNewLine;
  BOOL bModified;
  BOOL bReadOnly;
  BOOL bWordWrap;
  BOOL bInsertState;
  int nTabStopSize;
  int nUndoLimit;
  BOOL bShowURL;
  DWORD dwEditMargins;
  BOOL bDelimitersEnable;
  COLORREF crFont;
  COLORREF crBackground;
  FILETIME ft;
  LOGFONTW lf;
} WNDFRAMEW;

typedef struct _EDITINFO {
  HWND hWndEdit;
  unsigned char *pFile;
  int nCodePage;
  BOOL bBOM;
  int nNewLine;
  BOOL bModified;
  BOOL bReadOnly;
  BOOL bWordWrap;
  BOOL bInsertState;
  int nTabStopSize;
  BOOL bTabStopAsSpaces;
  int nUndoLimit;
  BOOL bDetailedUndo;
  BOOL bShowURL;
  DWORD dwEditMargins;
  BOOL bDelimitersEnable;
  COLORREF crFont;
  COLORREF crBackground;
  FILETIME ft;
} EDITINFO;

typedef struct _WNDPROCDATA {
  struct _WNDPROCDATA *next;
  struct _WNDPROCDATA *prev;
  WNDPROC CurProc;
  WNDPROC NextProc;
  WNDPROC PrevProc;
} WNDPROCDATA;

typedef struct _WNDPROCRETDATA {
  struct _WNDPROCRETDATA *next;
  struct _WNDPROCRETDATA *prev;
  WNDPROCRET CurProc;
  WNDPROCRET NextProc;
  WNDPROCRET PrevProc;
} WNDPROCRETDATA;

typedef struct _FILESTREAMDATA {
  HWND hWnd;
  HANDLE hFile;
  int nCodePage;
  int nBytesMax;
  BOOL bResult;
} FILESTREAMDATA;

typedef struct _COLORTHEMEA {
  struct _COLORTHEMEA *next;
  struct _COLORTHEMEA *prev;
  char szName[MAX_PATH];
  int nNameLen;
  AECOLORS aec;
} COLORTHEMEA;

typedef struct _COLORTHEMEW {
  struct _COLORTHEMEW *next;
  struct _COLORTHEMEW *prev;
  wchar_t wszName[MAX_PATH];
  int nNameLen;
  AECOLORS aec;
} COLORTHEMEW;

typedef struct _PLUGINDATA {
  DWORD cb;
  unsigned char *pFunction;
  HINSTANCE hInstanceDLL;
  BOOL *lpbAutoLoad;
  int nUnload;
  BOOL bActive;
  BOOL bOnStart;
  LPARAM lParam;
  unsigned char *pAkelDir;
  HINSTANCE hInstanceEXE;
  HWND hMainWnd;
  HWND hWndEdit;
  HWND hStatus;
  HWND hMdiClient;
  HWND hTab;
  HMENU hMainMenu;
  HMENU hMenuRecentFiles;
  HMENU hMenuLanguage;
  HMENU hPopupMenu;
  HICON hMainIcon;
  BOOL bOldWindows;
  BOOL bOldRichEdit;
  BOOL bOldComctl32;
  BOOL bMDI;
  int nSaveSettings;
  LANGID wLangSystem;
  HSTACK *hPluginsStack;
  void *lpPluginFunction;
  HACCEL hGlobalAccel;
} PLUGINDATA;

typedef struct _PLUGINFUNCTIONA {
  struct _PLUGINFUNCTIONA *next;
  struct _PLUGINFUNCTIONA *prev;
  char szFunction[MAX_PATH];
  int nFunctionLen;
  WORD wHotkey;
  BOOL bOnStart;
  BOOL bRunning;
  PLUGINPROC PluginProc;
  void *lpParameter;
} PLUGINFUNCTIONA;

typedef struct _PLUGINFUNCTIONW {
  struct _PLUGINFUNCTIONW *next;
  struct _PLUGINFUNCTIONW *prev;
  wchar_t wszFunction[MAX_PATH];
  int nFunctionLen;
  WORD wHotkey;
  BOOL bOnStart;
  BOOL bRunning;
  PLUGINPROC PluginProc;
  void *lpParameter;
} PLUGINFUNCTIONW;

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

typedef struct _PLUGINLISTITEMA {
  struct _PLUGINLISTITEMA *next;
  struct _PLUGINLISTITEMA *prev;
  PLUGINFUNCTIONA *pf;
  WORD wInitialHotkey;
  BOOL bInitialOnStart;
  int nAutoLoad;
} PLUGINLISTITEMA;

typedef struct _PLUGINLISTITEMW {
  struct _PLUGINLISTITEMW *next;
  struct _PLUGINLISTITEMW *prev;
  PLUGINFUNCTIONW *pf;
  WORD wInitialHotkey;
  BOOL bInitialOnStart;
  int nAutoLoad;
} PLUGINLISTITEMW;

typedef struct _PLUGINCALLSENDA {
  char *pFunction;
  BOOL bOnStart;
  LPARAM lParam;
  BOOL *lpbAutoLoad;
} PLUGINCALLSENDA;

typedef struct _PLUGINCALLSENDW {
  wchar_t *wpFunction;
  BOOL bOnStart;
  LPARAM lParam;
  BOOL *lpbAutoLoad;
} PLUGINCALLSENDW;

typedef struct _PLUGINCALLPOSTA {
  char szFunction[MAX_PATH];
  BOOL bOnStart;
  LPARAM lParam;
} PLUGINCALLPOSTA;

typedef struct _PLUGINCALLPOSTW {
  wchar_t wszFunction[MAX_PATH];
  BOOL bOnStart;
  LPARAM lParam;
} PLUGINCALLPOSTW;

typedef struct PLUGINUNLOADPOSTW {
  wchar_t wszFunction[MAX_PATH];
  HMODULE hInstanceDLL;
  HANDLE hThread;
} PLUGINUNLOADPOSTW;

typedef struct _REGHANDLEA {
  DWORD dwType;
  HKEY hKey;
  char szKey[MAX_PATH];
} REGHANDLEA;

typedef struct _REGHANDLEW {
  DWORD dwType;
  HKEY hKey;
  wchar_t wszKey[MAX_PATH];
} REGHANDLEW;

typedef struct _INIHANDLEA {
  DWORD dwType;
  HSTACK hStack;
  char szIniFile[MAX_PATH];
} INIHANDLEA;

typedef struct _INIHANDLEW {
  DWORD dwType;
  HSTACK hStack;
  wchar_t wszIniFile[MAX_PATH];
} INIHANDLEW;

typedef struct _PLUGINOPTIONA {
  char *szOptionName;
  DWORD dwType;
  BYTE *lpData;
  DWORD dwData;
} PLUGINOPTIONA;

typedef struct _PLUGINOPTIONW {
  wchar_t *wszOptionName;
  DWORD dwType;
  BYTE *lpData;
  DWORD dwData;
} PLUGINOPTIONW;

typedef struct _GETTEXTRANGE {
  int cpMin;
  int cpMax;
  unsigned char *pText;
} GETTEXTRANGE;

typedef struct _RECENTFILESA {
  char (*lpszRecentNames)[MAX_PATH];
  DWORD *lpdwRecentPositions;
  DWORD *lpdwRecentCodepages;
  int nRecentFiles;
} RECENTFILESA;

typedef struct _RECENTFILESW {
  wchar_t (*lpwszRecentNames)[MAX_PATH];
  DWORD *lpdwRecentPositions;
  DWORD *lpdwRecentCodepages;
  int nRecentFiles;
} RECENTFILESW;

typedef struct _TEXTFINDA {
  DWORD dwFlags;
  char *pFindIt;
} TEXTFINDA;

typedef struct _TEXTFINDW {
  DWORD dwFlags;
  wchar_t *wpFindIt;
} TEXTFINDW;

typedef struct _TEXTREPLACEA {
  DWORD dwFlags;
  char *pFindIt;
  char *pReplaceWith;
  BOOL bAll;
  int nChanges;
} TEXTREPLACEA;

typedef struct _TEXTREPLACEW {
  DWORD dwFlags;
  wchar_t *wpFindIt;
  wchar_t *wpReplaceWith;
  BOOL bAll;
  int nChanges;
} TEXTREPLACEW;

typedef struct _TEXTRECODE {
  int nCodePageFrom;
  int nCodePageTo;
} TEXTRECODE;

typedef struct _CREATEWINDOWA {
  char szClassName[MAX_PATH];
  char szWindowName[MAX_PATH];
  DWORD dwStyle;
  int x;
  int y;
  int nWidth;
  int nHeight;
  HWND hWndParent;
  HMENU hMenu;
  HINSTANCE hInstance;
  LPVOID lpParam;
} CREATEWINDOWA;

typedef struct _CREATEWINDOWW {
  wchar_t wszClassName[MAX_PATH];
  wchar_t wszWindowName[MAX_PATH];
  DWORD dwStyle;
  int x;
  int y;
  int nWidth;
  int nHeight;
  HWND hWndParent;
  HMENU hMenu;
  HINSTANCE hInstance;
  LPVOID lpParam;
} CREATEWINDOWW;

typedef struct _DOCK {
  struct _DOCK *next;
  struct _DOCK *prev;
  DWORD dwFlags;
  HWND hWnd;
  int nSide;
  RECT rcSize;
  RECT rcDragDrop;
} DOCK;

typedef struct _HDOCK {
  HSTACK hStack;
  BOOL bSizing;
  int nSizingSide;
} HDOCK;

typedef struct _POSTMESSAGE {
  HWND hWnd;
  UINT uMsg;
  WPARAM wParam;
  LPARAM lParam;
} POSTMESSAGE;

typedef struct _NOPENDOCUMENTA {
  char *szFile;
  int *nCodePage;
  BOOL *bBOM;
  DWORD *dwFlags;
  BOOL bProcess;
} NOPENDOCUMENTA;

typedef struct _NOPENDOCUMENTW {
  wchar_t *wszFile;
  int *nCodePage;
  BOOL *bBOM;
  DWORD *dwFlags;
  BOOL bProcess;
} NOPENDOCUMENTW;

typedef struct _NSAVEDOCUMENTA {
  char *szFile;
  int *nCodePage;
  BOOL *bBOM;
  BOOL bProcess;
} NSAVEDOCUMENTA;

typedef struct _NSAVEDOCUMENTW {
  wchar_t *wszFile;
  int *nCodePage;
  BOOL *bBOM;
  BOOL bProcess;
} NSAVEDOCUMENTW;

typedef struct _NMAINSHOW {
  DWORD *dwStyle;
  DWORD *dwShow;
  BOOL bProcess;
} NMAINSHOW;

typedef struct _NCONTEXTMENU {
  HWND hWnd;
  UINT uType;
  POINT pt;
  BOOL bProcess;
} NCONTEXTMENU;

typedef struct _NSIZE {
  RECT rcInitial;
  RECT rcCurrent;
} NSIZE;


//// Functions prototype

void CreateEditWindowA(HWND hWnd);
void CreateEditWindowW(HWND hWnd);

BOOL DoFileNewA();
BOOL DoFileNewW();
BOOL DoFileCloseA();
BOOL DoFileCloseW();
HWND DoFileNewWindowA(DWORD dwAddFlags);
HWND DoFileNewWindowW(DWORD dwAddFlags);
BOOL CALLBACK EnumThreadProcA(HWND hwnd, LPARAM lParam);
BOOL CALLBACK EnumThreadProcW(HWND hwnd, LPARAM lParam);
BOOL DoFileOpenA();
BOOL DoFileOpenW();
void DoFileReopenAsA(DWORD dwFlags, int nCodePage, BOOL bBOM);
void DoFileReopenAsW(DWORD dwFlags, int nCodePage, BOOL bBOM);
BOOL DoFileSaveA();
BOOL DoFileSaveW();
BOOL DoFileSaveAsA();
BOOL DoFileSaveAsW();
BOOL DoFilePageSetupA();
BOOL DoFilePageSetupW();
BOOL DoFilePrintA(BOOL bSilent);
BOOL DoFilePrintW(BOOL bSilent);
BOOL DoFileExitA();
BOOL DoFileExitW();
void DoEditUndo(HWND hWnd);
void DoEditRedo(HWND hWnd);
void DoEditCut(HWND hWnd);
void DoEditCopy(HWND hWnd);
void DoEditPaste(HWND hWnd, BOOL bAnsi);
void DoEditClear(HWND hWnd);
void DoEditSelectAll(HWND hWnd);
void DoEditInsertDateA(HWND hWnd);
void DoEditInsertDateW(HWND hWnd);
void DoEditInsertChar();
void DoEditRecodeA(HWND hWnd);
void DoEditRecodeW(HWND hWnd);
BOOL DoEditInsertStringInSelectionW(HWND hWnd, int nAction, wchar_t *wpString);
BOOL DoEditDeleteFirstCharW(HWND hWnd);
BOOL DoEditDeleteTrailingWhitespacesW(HWND hWnd);
BOOL DoEditChangeCaseA(HWND hWnd, int nCase);
BOOL DoEditChangeCaseW(HWND hWnd, int nCase);
void DoEditFindA();
void DoEditFindW();
void DoEditFindNextDownA(HWND hWnd);
void DoEditFindNextDownW(HWND hWnd);
void DoEditFindNextUpA(HWND hWnd);
void DoEditFindNextUpW(HWND hWnd);
void DoEditReplaceA();
void DoEditReplaceW();
void DoEditGoToLineA();
void DoEditGoToLineW();
BOOL DoViewFontA(HWND hWndOwner, LOGFONTA *lfA);
BOOL DoViewFontW(HWND hWndOwner, LOGFONTW *lfW);
void DoViewColorsA();
void DoViewColorsW();
void DoViewFontSizeA(HWND hWnd, int nAction);
void DoViewFontSizeW(HWND hWnd, int nAction);
void DoViewOnTop(BOOL bState, BOOL bFirst);
void DoViewWordWrap(HWND hWnd, BOOL bState, BOOL bFirst);
void DoViewShowStatusBar(BOOL bState, BOOL bFirst);
BOOL DoSettingsExecA();
BOOL DoSettingsExecW();
void DoSettingsReadOnly(HWND hWnd, BOOL bState, BOOL bFirst);
void DoSettingsSaveTime(BOOL bState);
void DoSettingsWatchFile(BOOL bState);
void DoSettingsSingleOpenFile(BOOL bState);
void DoSettingsSingleOpenProgram(BOOL bState);
void DoSettingsKeepSpace(BOOL bState);
void DoSettingsPluginsA(HWND hWnd);
void DoSettingsPluginsW(HWND hWnd);
void DoSettingsOptionsA();
void DoSettingsOptionsW();
void DoWindowTabView(int nView, BOOL bFirst);
void DoWindowTabType(int nType, BOOL bFirst);
void DoHelpAboutA();
void DoHelpAboutW();
void DoNonMenuDelLine(HWND hWnd);

BOOL OpenIniA(HSTACK *hIniStack, char *pFile);
BOOL OpenIniW(HSTACK *hIniStack, wchar_t *wpFile);
BOOL CreateIniA(HSTACK *hIniStack, char *pFile);
BOOL CreateIniW(HSTACK *hIniStack, wchar_t *wpFile);
BOOL ReadIni(HSTACK *hIniStack, HANDLE hFile);
BOOL SaveIniA(HSTACK *hIniStack, char *pFile);
BOOL SaveIniW(HSTACK *hIniStack, wchar_t *wpFile);
BOOL WriteIni(HSTACK *hIniStack, HANDLE hFile);
HINISECTION* StackGetIniSectionA(HSTACK *hIniStack, char *pSection, int nSectionLen);
HINISECTION* StackGetIniSectionW(HSTACK *hIniStack, wchar_t *wpSection, int nSectionLen);
HINIKEY* StackGetIniKeyA(HSTACK *hSectionStack, char *pKey, int nKeyLen);
HINIKEY* StackGetIniKeyW(HSTACK *hSectionStack, wchar_t *wpKey, int nKeyLen);
void StackFreeIni(HSTACK *hIniStack);
void StackFreeIniSection(HSTACK *hSectionStack);
int IniGetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
int IniGetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
DWORD HexStrToDataA(char *pHexStr, unsigned char *lpData, DWORD dwDataBytes);
DWORD HexStrToDataW(wchar_t *wpHexStr, unsigned char *lpData, DWORD dwDataBytes);

void RegReadOptionsA();
void RegReadOptionsW();
void IniReadOptionsA();
void IniReadOptionsW();
void RegRegisterPluginsHotkeysA();
void RegRegisterPluginsHotkeysW();
void IniRegisterPluginsHotkeysA();
void IniRegisterPluginsHotkeysW();
void RegReadSearchA();
void RegReadSearchW();
BOOL RegSaveOptionsA();
BOOL RegSaveOptionsW();
BOOL IniSaveOptionsA();
BOOL IniSaveOptionsW();
void ReadThemesA();
void ReadThemesW();
BOOL SaveThemesA(BOOL bCleanOld);
BOOL SaveThemesW(BOOL bCleanOld);

int OpenDocumentA(HWND hWnd, char *szFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
int OpenDocumentW(HWND hWnd, wchar_t *wszFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
void FileStreamIn(FILESTREAMDATA *lpData);
int SaveDocumentA(HWND hWnd, char *szFile, int nCodePage, BOOL bBOM, BOOL bUpdate);
int SaveDocumentW(HWND hWnd, wchar_t *wszFile, int nCodePage, BOOL bBOM, BOOL bUpdate);
void FileStreamOut(FILESTREAMDATA *lpData);
BOOL CALLBACK OutputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen);
BOOL OpenDirectoryA(char *pPath, BOOL bSubDir);
BOOL OpenDirectoryW(wchar_t *wpPath, BOOL bSubDir);

unsigned int CALLBACK PrintPageSetupDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
unsigned int CALLBACK PrintPageSetupDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetPrintPage(HDC hDC, RECT *rcMargin, BOOL bIsMarginInMillimeters, RECT *rcPage);
void GetPrinterDCA(PRINTDLGA *pdA);
void GetPrinterDCW(PRINTDLGW *pdW);
BOOL PrintTextA(HDC hDC, RECT *rc, TEXTMETRICA *tmA, DRAWTEXTPARAMS *dtp, char *pText, char **pNextText);
BOOL PrintTextW(HDC hDC, RECT *rc, TEXTMETRICW *tmW, DRAWTEXTPARAMS *dtp, wchar_t *wpText, wchar_t **wpNextText);
BOOL PrintHeadlineA(HDC hDC, RECT *rc, char *pHeadline, int nPageNumber);
BOOL PrintHeadlineW(HDC hDC, RECT *rc, wchar_t *wpHeadline, int nPageNumber);

unsigned int CALLBACK CodePageDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
unsigned int CALLBACK CodePageDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewPreviewProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewPreviewProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxCodepageA(HWND hWnd, int *lpCodepageList);
void FillComboboxCodepageW(HWND hWnd, int *lpCodepageList);
void FillListboxCodepageA(HWND hWnd, int *lpCodepageList);
void FillListboxCodepageW(HWND hWnd, int *lpCodepageList);
void ClearCombobox(HWND hWnd);
void ClearListbox(HWND hWnd);
int GetComboboxCodepageA(HWND hWnd);
int GetComboboxCodepageW(HWND hWnd);
int GetListboxCodepageA(HWND hWnd);
int GetListboxCodepageW(HWND hWnd);
int SelectComboboxCodepageA(HWND hWnd, int nCodepage);
int SelectComboboxCodepageW(HWND hWnd, int nCodepage);
int SelectListboxCodepageA(HWND hWnd, int nCodepage);
int SelectListboxCodepageW(HWND hWnd, int nCodepage);
void GetListboxCodepageListA(HWND hWnd, int **lpCodepageList);
void GetListboxCodepageListW(HWND hWnd, int **lpCodepageList);
DWORD EnumCodepageList(int **lpCodepageList);
int CodepageListLen(int *lpCodepageList);
int CodepageListFind(int *lpCodepageList, int nCodePage);
void CodepageListFree(int **lpCodepageList);
void GetCodePageNameA(int nCodePage, char *szCodePage, int nLen);
void GetCodePageNameW(int nCodePage, wchar_t *wszCodePage, int nLen);
int FilePreviewA(HWND hWnd, char *pFile, int nPreviewBytes, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
int FilePreviewW(HWND hWnd, wchar_t *wpFile, int nPreviewBytes, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
int AutodetectCodePageA(char *pFile, DWORD dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
int AutodetectCodePageW(wchar_t *wpFile, DWORD dwBytesToCheck, DWORD dwFlags, int *nCodePage, BOOL *bBOM);
BOOL AutodetectMultibyte(DWORD dwLangID, unsigned char *pBuffer, DWORD dwBytesToCheck, int *nCodePage);
unsigned int UTF8toUTF16(const char *pMultiString, unsigned int nMultiString, unsigned int *nMultiStringRemain,  wchar_t *wszWideString, unsigned int nWideString);
unsigned int UTF16toUTF8(const wchar_t *wpWideString, unsigned int nWideString, char *szMultiString, unsigned int nMultiString);
void ChangeByteOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
BOOL IsCodePageValid(int nCodePage);
unsigned int TranslateNewLinesToUnixW(wchar_t *wpWideString, unsigned int nWideString);

BOOL CALLBACK FindAndReplaceDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindAndReplaceDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewComboboxEditProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewComboboxEditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxSearchA(HWND hWndFind, HWND hWndReplace);
void FillComboboxSearchW(HWND hWndFind, HWND hWndReplace);
BOOL GetComboboxSearchTextA(HWND hWnd, char **szText_orig, char **szText);
BOOL GetComboboxSearchTextW(HWND hWnd, wchar_t **wszText_orig, wchar_t **wszText);
void SaveComboboxSearchA(HWND hWndFind, HWND hWndReplace);
void SaveComboboxSearchW(HWND hWndFind, HWND hWndReplace);
int FindTextA(HWND hWnd, DWORD dwFlags, char *pFindIt);
int FindTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt);
int ReplaceTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, char *pReplaceWith, BOOL bAll, int *nReplaceCount);
int ReplaceTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, wchar_t *wpReplaceWith, BOOL bAll, int *nReplaceCount);
int StrReplaceA(char *pText, char *pIt, char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult, char **ppMin, char **ppMax, char **ppFirstVisible);
int StrReplaceW(wchar_t *wpText, wchar_t *wpIt, wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult, wchar_t **wppMin, wchar_t **wppMax, wchar_t **wppFirstVisible);
BOOL EscapeStringToEscapeDataA(char *pInput, char *szOutput);
BOOL EscapeStringToEscapeDataW(wchar_t *wpInput, wchar_t *wszOutput);
void EscapeDataToEscapeStringW(wchar_t *wpInput, wchar_t *wszOutput);

void ReplaceSelA(HWND hWnd, char *pData, int nDataLen);
void ReplaceSelW(HWND hWnd, wchar_t *wpData, int nDataLen);
int IndexSubtract(HWND hWnd, AECHARINDEX *ciChar1, AECHARINDEX *ciChar2, int nNewLine);
int IndexOffset(HWND hWnd, AECHARINDEX *ciChar, int nOffset, int nNewLine);
int AkelIndexToRichOffset(HWND hWnd, AECHARINDEX *ciChar);
void RichOffsetToAkelIndex(HWND hWnd, int nOffset, AECHARINDEX *ciChar);
int GetTextLength(HWND hWnd);
int GetRangeTextA(HWND hWnd, int nMin, int nMax, char **pText);
int GetRangeTextW(HWND hWnd, int nMin, int nMax, wchar_t **wpText);
int ExGetRangeTextA(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, char **pText, int nNewLine);
int ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, wchar_t **wpText, int nNewLine);
BOOL FreeText(LPVOID pText);
void PasteInEditAsRichEdit(HWND hWnd);

BOOL CALLBACK GoToLineDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GoToLineDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL RecentFilesAllocA();
BOOL RecentFilesAllocW();
void RecentFilesZeroA();
void RecentFilesZeroW();
void RecentFilesReadA();
void RecentFilesReadW();
BOOL RecentFilesGetA(char *pFile, int *nPosition, int *nCodePage);
BOOL RecentFilesGetW(wchar_t *wpFile, int *nPosition, int *nCodePage);
BOOL RecentFilesUpdateA(char *pFile, int nPosition, int nCodePage);
BOOL RecentFilesUpdateW(wchar_t *wpFile, int nPosition, int nCodePage);
int RecentFilesDeleteDeadA();
int RecentFilesDeleteDeadW();
void RecentFilesSaveA();
void RecentFilesSaveW();
void RecentFilesMenuA();
void RecentFilesMenuW();
int TrimPathA(char *szResult, char *pPath, int nMax);
int TrimPathW(wchar_t *wszResult, wchar_t *wpPath, int nMax);

void LanguageMenuA();
void LanguageMenuW();

void FillMenuPopupCodepageA();
void FillMenuPopupCodepageW();
void ShowMenuPopupCodepageA(POINT *pt);
void ShowMenuPopupCodepageW(POINT *pt);

BOOL isInDelimiterList(wchar_t c);
BOOL isInWhitespaceList(wchar_t c);
int CALLBACK EditWordBreakProc(LPTSTR pchText, int ichCurrent, int cch, int code);

void RecodeTextW(HWND hWnd, int nCodePageFrom, int nCodePageTo);
BOOL CALLBACK RecodeDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RecodeDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK ColorsDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ColorsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxThemesA(HWND hWnd);
void FillComboboxThemesW(HWND hWnd);

BOOL CALLBACK PluginsDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PluginsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);
void FillPluginListA(HWND hWnd);
void FillPluginListW(HWND hWnd);
BOOL CALLBACK FillPluginListProcA(char *pExportName, LPARAM lParam);
BOOL CALLBACK FillPluginListProcW(char *pExportName, LPARAM lParam);
PLUGINLISTITEMA* GetPluginListItemA(HSTACK *hStack, int nIndex);
PLUGINLISTITEMW* GetPluginListItemW(HSTACK *hStack, int nIndex);
void FreePluginList(HSTACK *hStack);
void GetHotkeyStringA(WORD wHotkey, char *szString);
void GetHotkeyStringW(WORD wHotkey, wchar_t *wszString);
BOOL ParsePluginNameA(char *pFullName, char *szPlugin, char *szFunction);
BOOL ParsePluginNameW(wchar_t *wpFullName, wchar_t *wszPlugin, wchar_t *wszFunction);
int CallPluginA(PLUGINFUNCTIONA *lpPluginFunction, char *pFullName, BOOL bOnStart, LPARAM lParam, BOOL *lpbAutoLoad);
int CallPluginW(PLUGINFUNCTIONW *lpPluginFunction, wchar_t *wpFullName, BOOL bOnStart, LPARAM lParam, BOOL *lpbAutoLoad);
void CallPluginsOnStartA(HSTACK *hStack);
void CallPluginsOnStartW(HSTACK *hStack);
int CallPluginReceiveSendA(PLUGINCALLSENDA *pcs);
int CallPluginReceiveSendW(PLUGINCALLSENDW *pcs);
void CallPluginReceivePostA(PLUGINCALLPOSTA *pcp);
void CallPluginReceivePostW(PLUGINCALLPOSTW *pcp);
BOOL GetExportNames(HMODULE hInstance, EXPORTNAMESPROC lpExportNamesProc, LPARAM lParam);

int StackProcGet(HSTACK *hStack, int nIndex, WNDPROCDATA **ProcData);
int StackProcSet(HSTACK *hStack, WNDPROC NewProc, WNDPROCDATA **NewProcData, WNDPROC *FirstProc);
void StackProcFree(HSTACK *hStack);
COLORTHEMEA* StackThemeAddA(HSTACK *hStack, char *pName, AECOLORS *aec);
COLORTHEMEW* StackThemeAddW(HSTACK *hStack, wchar_t *wpName, AECOLORS *aec);
COLORTHEMEA* StackThemeGetByNameA(HSTACK *hStack, char *pName);
COLORTHEMEW* StackThemeGetByNameW(HSTACK *hStack, wchar_t *wpName);
COLORTHEMEA* StackThemeGetByColorsA(HSTACK *hStack, AECOLORS *aec);
COLORTHEMEW* StackThemeGetByColorsW(HSTACK *hStack, AECOLORS *aec);
void StackThemeDeleteA(HSTACK *hStack, COLORTHEMEA *ctElement);
void StackThemeDeleteW(HSTACK *hStack, COLORTHEMEW *ctElement);
void StackThemeFree(HSTACK *hStack);
PLUGINHANDLE* StackHandleIncrease(HSTACK *hStack, HMODULE hModule);
PLUGINHANDLE* StackHandleDecrease(HSTACK *hStack, HMODULE hModule);
void StackHandleFree(HSTACK *hStack);
DOCK* StackDockAdd(HDOCK *hDocks, DOCK *dkData);
int DockSetSide(HDOCK *hDocks, DOCK *dkData, int nSide);
void StackDockDelete(HDOCK *hDocks, DOCK *dkData);
DOCK* StackDockFindWindow(HDOCK *hDocks, HWND hWnd, BOOL bChild);
HWND StackDockNextWindow(HDOCK *hDocks, DOCK *dkData, BOOL bPrevious);
DOCK* StackDockFromPoint(HDOCK *hDocks, POINT *pt);
void StackDockSize(HDOCK *hDocks, int nSide, NSIZE *ns);
BOOL StackDockUpdateCheck(HDOCK *hDocks);
void StackDockFree(HDOCK *hDocks);
PLUGINFUNCTIONA* StackPluginFindA(HSTACK *hStack, char *pString);
PLUGINFUNCTIONW* StackPluginFindW(HSTACK *hStack, wchar_t *wpString);
PLUGINFUNCTIONA* StackHotkeyFindA(HSTACK *hStack, WORD wHotkey);
PLUGINFUNCTIONW* StackHotkeyFindW(HSTACK *hStack, WORD wHotkey);
PLUGINFUNCTIONA* StackPluginAddA(HSTACK *hStack, char *pString, int nStringLen, WORD wHotkey, BOOL bOnStart, BOOL bRunning, PLUGINPROC PluginProc, void *lpParameter);
PLUGINFUNCTIONW* StackPluginAddW(HSTACK *hStack, wchar_t *wpString, int nStringLen, WORD wHotkey, BOOL bOnStart, BOOL bRunning, PLUGINPROC PluginProc, void *lpParameter);
void StackPluginDelete(HSTACK *hStack, void *lpElement);
BOOL StackPluginSaveA(HSTACK *hStack, BOOL bCleanOld);
BOOL StackPluginSaveW(HSTACK *hStack, BOOL bCleanOld);
void StackPluginCleanUpA(HSTACK *hStack);
void StackPluginCleanUpW(HSTACK *hStack);
void StackPluginFree(HSTACK *hStack);
BOOL IsMainFunctionA(char *pFunction);
BOOL IsMainFunctionW(wchar_t *wpFunction);
BOOL TranslateDialogA(HDOCK *hDocks, LPMSG lpMsg);
BOOL TranslateDialogW(HDOCK *hDocks, LPMSG lpMsg);
BOOL TranslatePluginA(LPMSG lpMsg);
BOOL TranslatePluginW(LPMSG lpMsg);
BOOL TranslateHotkeyA(HSTACK *hStack, LPMSG lpMsg);
BOOL TranslateHotkeyW(HSTACK *hStack, LPMSG lpMsg);

BOOL OpenDocumentSendA(HWND hWnd, HWND hWndEdit, char *pFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess);
BOOL OpenDocumentSendW(HWND hWnd, HWND hWndEdit, wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess);

LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam);
int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam);
BOOL CALLBACK OptionsGeneralDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsGeneralDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK BrowseCallbackProcA(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
int CALLBACK BrowseCallbackProcW(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL CALLBACK OptionsGeneralFilterDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsGeneralFilterDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvanced1DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvanced1DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvanced2DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvanced2DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK AboutDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

void SetSelectionStatusA(HWND hWnd, AECHARRANGE *cr, AECHARINDEX *ci);
void SetSelectionStatusW(HWND hWnd, AECHARRANGE *cr, AECHARINDEX *ci);
void SetModifyStatusA(HWND hWnd, BOOL bState, BOOL bFirst);
void SetModifyStatusW(HWND hWnd, BOOL bState, BOOL bFirst);
void SetNewLineStatusA(HWND hWnd, BOOL bState, DWORD dwFlags, BOOL bFirst);
void SetNewLineStatusW(HWND hWnd, BOOL bState, DWORD dwFlags, BOOL bFirst);
void SetInsertStateStatusA(HWND hWnd, BOOL bState, BOOL bFirst);
void SetInsertStateStatusW(HWND hWnd, BOOL bState, BOOL bFirst);
void SetCodePageStatusA(int nCodePage, BOOL bBOM, BOOL bFirst);
void SetCodePageStatusW(int nCodePage, BOOL bBOM, BOOL bFirst);

void SaveLineScroll(HWND hWnd, int *nBeforeLine);
void RestoreLineScroll(HWND hWnd, int *nBeforeLine);
BOOL SelectColorDialogA(HWND hWndOwner, COLORREF *crColor);
BOOL SelectColorDialogW(HWND hWndOwner, COLORREF *crColor);
HFONT SetChosenFontA(HWND hWnd, LOGFONTA *lfA, BOOL bDeleteOld);
HFONT SetChosenFontW(HWND hWnd, LOGFONTW *lfW, BOOL bDeleteOld);
void SetTabStops(HWND hWnd, int nTabStops, BOOL bSetRedraw);
BOOL InsertTabStopW(HWND hWnd);
BOOL IndentTabStopW(HWND hWnd, int nAction);
BOOL AutoIndent(HWND hWnd, AECHARRANGE *cr);
int PixelsToTwips(HWND hWnd, int nPixels);
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();
int GetCommandLineArgA(char *pCmdLine, char *szArgName, int nArgNameLen, char **pArgOption, int *nArgOptionLen, char **pNextArg, BOOL bParseAsNotepad);
int GetCommandLineArgW(wchar_t *wpCmdLine, wchar_t *wszArgName, int nArgNameLen, wchar_t **wpArgOption, int *nArgOptionLen, wchar_t **wpNextArg, BOOL bParseAsNotepad);
void RegClearKeyA(HKEY hKey, char *pSubKey);
void RegClearKeyW(HKEY hKey, wchar_t *wpSubKey);
void GetAssociatedIconA(char *pExt, char *szFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
void GetAssociatedIconW(wchar_t *wpExt, wchar_t *wszFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
void AssociateFileTypesA(HINSTANCE hInstance, char *pFileTypes, DWORD dwFlags);
void AssociateFileTypesW(HINSTANCE hInstance, wchar_t *wpFileTypes, DWORD dwFlags);
BOOL GetFileWriteTimeA(char *pFile, FILETIME *ft);
BOOL GetFileWriteTimeW(wchar_t *wpFile, FILETIME *ft);
BOOL IsReadOnly();
BOOL SaveChangedA();
BOOL SaveChangedW();
void ShowURL(HWND hWnd, BOOL bShow);
BOOL FileExistsA(char *pFile);
BOOL FileExistsW(wchar_t *wpFile);
int IsFileA(char *pFile);
int IsFileW(wchar_t *wpFile);
BOOL IsPathFullA(char *pPath);
BOOL IsPathFullW(wchar_t *wpPath);
int GetExeDirA(HINSTANCE hInstance, char *szExeDir, int nLen);
int GetExeDirW(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen);
int GetFileDirA(char *pFile, char *szFileDir, int nFileDirLen);
int GetFileDirW(wchar_t *wpFile, wchar_t *wszFileDir, int nFileDirLen);
BOOL GetFullNameA(char *szFile);
BOOL GetFullNameW(wchar_t *wszFile);
char* GetFileNameA(char *pFile);
wchar_t* GetFileNameW(wchar_t *wpFile);
int GetBaseNameA(char *pFile, char *szBaseName, int nBaseNameMaxLen);
int GetBaseNameW(wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen);
char* GetFileExtA(char *pFile);
wchar_t* GetFileExtW(wchar_t *wpFile);
BOOL GetFileVersionA(char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild);
BOOL GetFileVersionW(wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild);
int TranslateFileStringA(char *pCommand, char *szBuffer, int nBufferSize);
int TranslateFileStringW(wchar_t *wpCommand, wchar_t *wszBuffer, int nBufferSize);
void ActivateWindow(HWND hWnd);
HWND NextDialog(BOOL bPrevious);
void UpdateEdit(HWND hWnd);
void ResizeEdit(HWND hWnd, int X, int Y, int nWidth, int nHeight);
void UpdateSize();
void GetMovingRect(DOCK *dkData, POINT *pt, MINMAXINFO *mmi, RECT *rcScreen);
void DrawMovingRect(RECT *rcScreen);
int GetMouseEdge(HWND hWnd, POINT *pt);
void SetEdgeCursor(int nEdge);
int GetOppEdge(int nEdge);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL ScreenToClientRect(HWND hWnd, RECT *rc);
BOOL ClientToScreenRect(HWND hWnd, RECT *rc);
void UpdateTitleA(HWND hWndEditParent, char *szFile);
void UpdateTitleW(HWND hWndEditParent, wchar_t *wszFile);
void UpdateTabs(HWND hWnd);
int GetTabItemFromParam(HWND hWnd, LPARAM lParam);
int GetTabItemFromPoint(HWND hWnd, POINT *pt);
int SelectTabItem(HWND hWnd, int nIndex);
BOOL DeleteTabItem(HWND hWnd, int nIndex);
void FreeMemorySearchA();
void FreeMemorySearchW();
void FreeMemoryRecentFilesA();
void FreeMemoryRecentFilesW();

int AEC_IndexCompare(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2);
wchar_t AEC_WideCharUpper(wchar_t c);
int AEC_WideStrCmp(const wchar_t *wpString, const wchar_t *wpString2);
int AEC_WideStrCmpI(const wchar_t *wpString, const wchar_t *wpString2);

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
HWND API_CreateDialogA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
HWND API_CreateDialogW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBoxA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBoxW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc);
INT_PTR API_DialogBoxParamA(HINSTANCE hLoadInstance, char *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
INT_PTR API_DialogBoxParamW(HINSTANCE hLoadInstance, wchar_t *lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
HANDLE API_CreateFileA(char *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE API_CreateFileW(wchar_t *lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL API_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL API_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
LPVOID API_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
BOOL API_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);

#endif
