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
#define APP_ABOUT_VERSIONA           "AkelPad 4.4.4"
#define APP_ABOUT_VERSIONW          L"AkelPad 4.4.4"
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
#define STR_NOBOMA             "  *BOM"
#define STR_NOBOMW            L"  *BOM"

#ifndef TCN_GETOBJECT
  #define TCN_GETOBJECT (TCN_FIRST - 3)
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
#define FILELIST_BUFFER_SIZE       8192
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

//Print preview dialog notifications
#define AKDLG_PREVIEWKEYDOWN           (WM_USER + 100) //lParam - WM_KEYDOWN's lParam, wParam - control handle.
#define AKDLG_PREVIEWMOUSEWHEEL        (WM_USER + 151) //lParam - WM_MOUSEWHEEL's lParam, wParam - control handle.
#define AKDLG_PREVIEWSETZOOM           (WM_USER + 152) //lParam - percentage, wParam - update control immediately.
#define AKDLG_PREVIEWSETPAGE           (WM_USER + 153) //lParam - page number, wParam - not used.

//PrintDocument flags
#define PRND_REALPRINT          0x01  //Real printing
#define PRND_TEST               0x02  //Get preview information
#define PRND_ONEPAGE            0x04  //Print only one page
#define PRND_ALLTEXT            0x08  //Process all text, prn->crText member input ignored
#define PRND_SELECTION          0x10  //Process selection, prn->crText member input ignored
#define PRND_RANGE              0x20  //Process text range pointed with prn->crText member
#define PRND_ANSI               0x40  //Ansi output

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

//Search dialog
#define IDC_COMBOBOX_EDIT              1001

//Language identifiers
#define LANGID_RUSSIAN    0x0419
#define LANGID_ENGLISH    0x0409
#define LANGID_TURKISH    0x041F
#define LANGID_CHINESE    0x0404
#define LANGID_JAPANESE   0x0411
#define LANGID_KOREAN     0x0412

//Code pages int
#define CP_UNICODE_UCS2_LE  1200
#define CP_UNICODE_UCS2_BE  1201
#define CP_UNICODE_UTF7     65000
#define CP_UNICODE_UTF8     65001
#define CP_KOI8_R           20866

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

//STARTUPINFO flags
#define STARTF_NOMUTEX  0x00001000

//Autodetect flags
#define ADT_BINARY_ERROR        0x00000001
#define ADT_REG_CODEPAGE        0x00000002
#define ADT_DETECT_CODEPAGE     0x00000004
#define ADT_DETECT_BOM          0x00000008

//Autodetect errors
#define EDT_OPEN          -1
#define EDT_ALLOC         -2
#define EDT_READ          -3
#define EDT_BINARY        -4

//Search/Replace options
#define AEFR_UP                 0x00100000
#define AEFR_BEGINNING          0x00200000
#define AEFR_SELECTION          0x00400000
#define AEFR_ESCAPESEQ          0x00800000
#define AEFR_ALLFILES           0x01000000

//StrReplace options
#define AEFR_WHOLEWORDGOODSTART 0x02000000
#define AEFR_WHOLEWORDGOODEND   0x04000000

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


//// Structures

typedef BOOL (CALLBACK *EXPORTNAMESPROC)(char *, LPARAM);

typedef struct _HINISECTION {
  struct _HINISECTION *next;
  struct _HINISECTION *prev;
  wchar_t *wszSection;
  int nSectionBytes;
  HSTACK hKeysStack;
} HINISECTION;

typedef struct _HINIKEY {
  struct _HINIKEY *next;
  struct _HINIKEY *prev;
  wchar_t *wszKey;
  int nKeyBytes;
  wchar_t *wszString;
  int nStringBytes;
} HINIKEY;

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
  int nCallResult;
} PLUGINLISTITEMA;

typedef struct _PLUGINLISTITEMW {
  struct _PLUGINLISTITEMW *next;
  struct _PLUGINLISTITEMW *prev;
  PLUGINFUNCTIONW *pf;
  WORD wInitialHotkey;
  BOOL bInitialOnStart;
  int nAutoLoad;
  int nCallResult;
} PLUGINLISTITEMW;

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

typedef struct _HDOCK {
  HSTACK hStack;
  BOOL bSizing;
  int nSizingSide;
} HDOCK;

typedef struct _FONTITEMA {
  struct _FONTITEMA *next;
  struct _FONTITEMA *prev;
  LOGFONTA lfFont;
  HFONT hFont;
} FONTITEMA;

typedef struct _FONTITEMW {
  struct _FONTITEMW *next;
  struct _FONTITEMW *prev;
  LOGFONTW lfFont;
  HFONT hFont;
} FONTITEMW;

typedef struct _ASSOCICONA {
  struct _ASSOCICONA *next;
  struct _ASSOCICONA *prev;
  char szFile[MAX_PATH];
  char *pExt;
  HICON hIcon;
} ASSOCICONA;

typedef struct _ASSOCICONW {
  struct _ASSOCICONW *next;
  struct _ASSOCICONW *prev;
  wchar_t wszFile[MAX_PATH];
  wchar_t *wpExt;
  HICON hIcon;
} ASSOCICONW;

typedef struct {
  int nCodePage;
  BOOL bBOM;
  BOOL bResult;
} DIALOGCODEPAGE;


//// Functions prototype

HWND CreateEditWindowA(HWND hWnd);
HWND CreateEditWindowW(HWND hWnd);

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
int DoFileReopenAsA(DWORD dwFlags, int nCodePage, BOOL bBOM);
int DoFileReopenAsW(DWORD dwFlags, int nCodePage, BOOL bBOM);
BOOL DoFileSaveA();
BOOL DoFileSaveW();
BOOL DoFileSaveAsA(int nDialogCodePage, BOOL bDialogBOM);
BOOL DoFileSaveAsW(int nDialogCodePage, BOOL bDialogBOM);
void DoFileSaveAllAsA();
void DoFileSaveAllAsW();
BOOL DoFilePageSetupA(HWND hWndOwner);
BOOL DoFilePageSetupW(HWND hWndOwner);
int DoFilePrintA(HWND hWnd, BOOL bSilent);
int DoFilePrintW(HWND hWnd, BOOL bSilent);
void DoFilePreviewA(HWND hWnd);
void DoFilePreviewW(HWND hWnd);
BOOL DoFileExitA();
BOOL DoFileExitW();
void DoEditUndo(HWND hWnd);
void DoEditRedo(HWND hWnd);
void DoEditCut(HWND hWnd);
void DoEditCopy(HWND hWnd);
BOOL DoEditPaste(HWND hWnd, BOOL bAnsi);
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
void DoViewWordWrap(HWND hWnd, BOOL bState, BOOL bFirst);
void DoViewSplitWindow(BOOL bState, WPARAM wParam);
void DoViewOnTop(BOOL bState, BOOL bFirst);
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
void DoWindowTabView(DWORD dwNewView, BOOL bFirst);
void DoWindowTabType(DWORD dwNewType, BOOL bFirst);
void DoWindowSelectWindowA();
void DoWindowSelectWindowW();
void DoHelpAboutA();
void DoHelpAboutW();
void DoNonMenuDelLine(HWND hWnd);

BOOL OpenIniA(HSTACK *hIniStack, char *pFile, BOOL bCreate);
BOOL OpenIniW(HSTACK *hIniStack, wchar_t *wpFile, BOOL bCreate);
BOOL SaveIniA(HSTACK *hIniStack, char *pFile);
BOOL SaveIniW(HSTACK *hIniStack, wchar_t *wpFile);
BOOL ReadIni(HSTACK *hIniStack, HANDLE hFile);
BOOL WriteIni(HSTACK *hIniStack, HANDLE hFile);
HINISECTION* StackOpenIniSectionA(HSTACK *hIniStack, char *pSection, int nSectionLen, BOOL bCreate);
HINISECTION* StackOpenIniSectionW(HSTACK *hIniStack, wchar_t *wpSection, int nSectionLen, BOOL bCreate);
HINIKEY* StackOpenIniKeyA(HINISECTION *lpIniSection, char *pKey, int nKeyLen, BOOL bCreate);
HINIKEY* StackOpenIniKeyW(HINISECTION *lpIniSection, wchar_t *wpKey, int nKeyLen, BOOL bCreate);
int StackGetIniData(HINIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL StackSetIniData(HINIKEY *lpIniKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
int IniGetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
int IniGetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
void StackDeleteIniKey(HINISECTION *lpIniSection, HINIKEY *lpIniKey);
void StackDeleteIniSection(HSTACK *hIniStack, HINISECTION *lpIniSection, BOOL bOnlyClear);
void StackFreeIni(HSTACK *hIniStack);

DWORD ReadOptionA(HANDLE lpHandle, char *pParam, DWORD dwType, void *lpData, DWORD dwSize);
DWORD ReadOptionW(HANDLE lpHandle, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
DWORD SaveOptionA(HANDLE lpHandle, char *pParam, DWORD dwType, void *lpData, DWORD dwSize);
DWORD SaveOptionW(HANDLE lpHandle, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
void ReadOptionsA();
void ReadOptionsW();
void RegisterPluginsHotkeysA();
void RegisterPluginsHotkeysW();
void RegReadSearchA();
void RegReadSearchW();
BOOL SaveOptionsA();
BOOL SaveOptionsW();
void ReadThemesA();
void ReadThemesW();
BOOL SaveThemesA(BOOL bCleanOld);
BOOL SaveThemesW(BOOL bCleanOld);

int OpenDocumentA(HWND hWnd, char *szFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
int OpenDocumentW(HWND hWnd, wchar_t *wszFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
void FileStreamIn(FILESTREAMDATA *lpData);
DWORD CALLBACK InputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufDone);
int SaveDocumentA(HWND hWnd, char *szFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
int SaveDocumentW(HWND hWnd, wchar_t *wszFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
void FileStreamOut(FILESTREAMDATA *lpData);
DWORD CALLBACK OutputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufDone);
BOOL OpenDirectoryA(char *pPath, BOOL bSubDir);
BOOL OpenDirectoryW(wchar_t *wpPath, BOOL bSubDir);
BOOL CALLBACK SaveAllAsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

unsigned int CALLBACK PrintPageSetupDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
unsigned int CALLBACK PrintPageSetupDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetPrinterDCA(PRINTDLGA *pdA);
void GetPrinterDCW(PRINTDLGW *pdW);
DWORD GetMappedPrintWidthA(HWND hWnd);
DWORD GetMappedPrintWidthW(HWND hWnd);
BOOL UpdateMappedPrintWidth(HWND hWnd);
int PrintDocumentA(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage);
int PrintDocumentW(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage);
BOOL PrintHeadlineA(HDC hDC, RECT *rc, char *pHeadline, int nPageNumber);
BOOL PrintHeadlineW(HDC hDC, RECT *rc, wchar_t *wpHeadline, int nPageNumber);
BOOL CALLBACK PreviewDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PreviewDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL PreviewInitA(HWND hWndSelection);
BOOL PreviewInitW(HWND hWndSelection);
void PreviewUninitA();
void PreviewUninitW();
LRESULT CALLBACK PreviewProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PreviewProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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

UINT_PTR CALLBACK CodePageDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
UINT_PTR CALLBACK CodePageDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFilePreviewProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFilePreviewProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
int EnumCodepageList(int **lpCodepageList);
void RegEnumSystemCodePagesA();
BOOL CALLBACK EnumCodePagesProc(wchar_t *wpCodePage);
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
unsigned int UTF8toUTF16(const unsigned char *pMultiString, unsigned int nMultiStringLen, unsigned int *nMultiStringDone,  wchar_t *wszWideString, unsigned int nWideStringMax);
unsigned int UTF16toUTF8(const wchar_t *wpWideString, unsigned int nWideStringLen, unsigned int *nWideStringDone, char *szMultiString, unsigned int nMultiStringMax);
void ChangeByteOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
BOOL IsCodePageValid(int nCodePage);
unsigned int TranslateNewLinesToUnixW(wchar_t *wszWideString, unsigned int nWideStringLen);

BOOL CALLBACK FindAndReplaceDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK FindAndReplaceDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewComboboxEditProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewComboboxEditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillComboboxSearchA(HWND hWndFind, HWND hWndReplace);
void FillComboboxSearchW(HWND hWndFind, HWND hWndReplace);
int GetComboboxSearchTextA(HWND hWnd, char **szText_orig, char **szText, int nNewLine);
int GetComboboxSearchTextW(HWND hWnd, wchar_t **wszText_orig, wchar_t **wszText, int nNewLine);
void SaveComboboxSearchA(HWND hWndFind, HWND hWndReplace);
void SaveComboboxSearchW(HWND hWndFind, HWND hWndReplace);
int FindTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, int nFindItLen);
int FindTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, int nFindItLen);
int ReplaceTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, int nFindItLen, char *pReplaceWith, int nReplaceWithLen, BOOL bAll, int *nReplaceCount);
int ReplaceTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, int nFindItLen, wchar_t *wpReplaceWith, int nReplaceWithLen, BOOL bAll, int *nReplaceCount);
int StrReplaceA(char *pText, int nTextLen, char *pIt, int nItLen, char *pWith, int nWithLen, DWORD dwFlags, char *szResult, int *nResultLen, int *nMin, int *nMax, int *nFirstVisible);
int StrReplaceW(wchar_t *wpText, int nTextLen, wchar_t *wpIt, int nItLen, wchar_t *wpWith, int nWithLen, DWORD dwFlags, wchar_t *wszResult, int *nResultLen, int *nMin, int *nMax, int *nFirstVisible);
int EscapeStringToEscapeDataA(char *pInput, char *szOutput, int nNewLine);
int EscapeStringToEscapeDataW(wchar_t *wpInput, wchar_t *wszOutput, int nNewLine);
void EscapeDataToEscapeStringW(wchar_t *wpInput, wchar_t *wszOutput);

void GetSel(HWND hWnd, AECHARRANGE *crSel, BOOL *bColumnSel, AECHARINDEX *ciCaret);
void SetSel(HWND hWnd, AECHARRANGE *crSel, DWORD dwFlags, AECHARINDEX *ciCaret);
void ReplaceSelA(HWND hWnd, char *pData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
void ReplaceSelW(HWND hWnd, wchar_t *wpData, int nDataLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd);
int IndexSubtract(HWND hWnd, AECHARINDEX *ciChar1, AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel);
int IndexOffset(HWND hWnd, AECHARINDEX *ciChar, int nOffset, int nNewLine);
int AkelIndexToRichOffset(HWND hWnd, AECHARINDEX *ciChar);
void RichOffsetToAkelIndex(HWND hWnd, int nOffset, AECHARINDEX *ciChar);
int GetTextLength(HWND hWnd);
int GetRangeTextA(HWND hWnd, int nMin, int nMax, char **pText);
int GetRangeTextW(HWND hWnd, int nMin, int nMax, wchar_t **wpText);
int ExGetRangeTextA(HWND hWnd, int nCodePage, char *lpDefaultChar, BOOL *lpUsedDefChar, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, char **pText, int nNewLine, BOOL bFillSpaces);
int ExGetRangeTextW(HWND hWnd, AECHARINDEX *ciMin, AECHARINDEX *ciMax, BOOL bColumnSel, wchar_t **wpText, int nNewLine, BOOL bFillSpaces);
BOOL FreeText(LPVOID pText);
BOOL PasteInEditAsRichEdit(HWND hWnd);
BOOL ColumnPaste(HWND hWnd);
BOOL PasteAfter(HWND hWnd, BOOL bAnsi);

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
int FixAmpA(const char *pInput, char *szOutput, int nOutputMax);
int FixAmpW(const wchar_t *wpInput, wchar_t *wszOutput, int nOutputMax);

void LanguageMenuA();
void LanguageMenuW();

void FillMenuPopupCodepageA();
void FillMenuPopupCodepageW();
void ShowMenuPopupCodepageA(POINT *pt);
void ShowMenuPopupCodepageW(POINT *pt);

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
void StackPluginCleanUpA(HSTACK *hStack, BOOL bDeleteNonExistentDLL);
void StackPluginCleanUpW(HSTACK *hStack, BOOL bDeleteNonExistentDLL);
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
BOOL CALLBACK OptionsRegistryDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsRegistryDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsEditor1DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsEditor1DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsEditor2DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsEditor2DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvancedDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvancedDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK MdiListDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MdiListDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillMdiListListboxA(HWND hWnd, BOOL bSort, BOOL bOnlyModified);
void FillMdiListListboxW(HWND hWnd, BOOL bSort, BOOL bOnlyModified);
int MoveListboxItemA(HWND hWnd, int nOldIndex, int nNewIndex);
int MoveListboxItemW(HWND hWnd, int nOldIndex, int nNewIndex);
BOOL ShiftListboxSelItems(HWND hWnd, BOOL bMoveDown);
BOOL SaveListboxSelItems(HWND hWnd);
void ArrangeListboxSelItems(HWND hWnd, int nBar);
BOOL CloseListboxSelItems(HWND hWnd);
int GetListboxSelItems(HWND hWnd, int **lpSelItems);
void FreeListboxSelItems(int **lpSelItems);

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

char* GetAssociatedIconA(const char *pFile, char *szIconFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
wchar_t* GetAssociatedIconW(const wchar_t *wpFile, wchar_t *wszIconFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
void AssociateFileTypesA(HINSTANCE hInstance, char *pFileTypes, DWORD dwFlags);
void AssociateFileTypesW(HINSTANCE hInstance, wchar_t *wpFileTypes, DWORD dwFlags);
ASSOCICONA* StackIconInsertA(HSTACK *hStack, const char *pFile);
ASSOCICONW* StackIconInsertW(HSTACK *hStack, const wchar_t *wpFile);
ASSOCICONA* StackIconGetA(HSTACK *hStack, const char *pFile, const char *pExt);
ASSOCICONW* StackIconGetW(HSTACK *hStack, const wchar_t *wpFile, const wchar_t *wpExt);
void StackIconsFreeA(HSTACK *hStack);
void StackIconsFreeW(HSTACK *hStack);

HFONT SetChosenFontA(HWND hWnd, LOGFONTA *lfA);
HFONT SetChosenFontW(HWND hWnd, LOGFONTW *lfW);
FONTITEMA* StackFontItemInsertA(HSTACK *hStack, LOGFONTA *lfFont);
FONTITEMW* StackFontItemInsertW(HSTACK *hStack, LOGFONTW *lfFont);
FONTITEMA* StackFontItemGetA(HSTACK *hStack, LOGFONTA *lfFont);
FONTITEMW* StackFontItemGetW(HSTACK *hStack, LOGFONTW *lfFont);
void StackFontItemsFreeA(HSTACK *hStack);
void StackFontItemsFreeW(HSTACK *hStack);

BOOL GetEditInfoA(HWND hWnd, EDITINFO *ei);
BOOL GetEditInfoW(HWND hWnd, EDITINFO *ei);
DWORD IsEditActive(HWND hWnd);
void UpdateShowHScroll(HWND hWnd);
int SaveLineScroll(HWND hWnd);
void RestoreLineScroll(HWND hWnd, int nBeforeLine);
DWORD ScrollCaret(HWND hWnd);
BOOL SelectColorDialogA(HWND hWndOwner, COLORREF *crColor);
BOOL SelectColorDialogW(HWND hWndOwner, COLORREF *crColor);
BOOL GetCharColor(HWND hWnd, CHARCOLOR *cc);
void SetMarker(HWND hWnd, DWORD dwPos);
void SetWordWrap(HWND hWnd, DWORD dwType, DWORD dwLimit);
void SetMargins(HWND hWnd, DWORD dwNewMargins, DWORD dwOldMargins);
void SetTabStops(HWND hWnd, int nTabStops, BOOL bSetRedraw);
BOOL InsertTabStopW(HWND hWnd);
BOOL IndentTabStopW(HWND hWnd, int nAction);
BOOL AutoIndent(HWND hWnd, AECHARRANGE *cr);
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();
int GetCommandLineArgA(char *pCmdLine, char *szArgName, int nArgNameLen, char **pArgOption, int *nArgOptionLen, char **pNextArg, BOOL bParseAsNotepad);
int GetCommandLineArgW(wchar_t *wpCmdLine, wchar_t *wszArgName, int nArgNameLen, wchar_t **wpArgOption, int *nArgOptionLen, wchar_t **wpNextArg, BOOL bParseAsNotepad);
void RegClearKeyA(HKEY hKey, char *pSubKey);
void RegClearKeyW(HKEY hKey, wchar_t *wpSubKey);
int SetUrlPrefixes(HWND hWnd, wchar_t *wpPrefixes);
BOOL GetFileWriteTimeA(char *pFile, FILETIME *ft);
BOOL GetFileWriteTimeW(wchar_t *wpFile, FILETIME *ft);
BOOL IsReadOnly();
BOOL SaveChangedA();
BOOL SaveChangedW();
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
BOOL GetFullNameA(char *szFile, int nFileLen);
BOOL GetFullNameW(wchar_t *wszFile, int nFileLen);
char* GetFileNameA(char *pFile);
wchar_t* GetFileNameW(wchar_t *wpFile);
int GetBaseNameA(char *pFile, char *szBaseName, int nBaseNameMaxLen);
int GetBaseNameW(wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen);
char* GetFileExtA(const char *pFile);
wchar_t* GetFileExtW(const wchar_t *wpFile);
void TrimModifyStateA(char *szFile);
void TrimModifyStateW(wchar_t *wszFile);
BOOL GetFileVersionA(char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild);
BOOL GetFileVersionW(wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild);
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2);
int TranslateFileStringA(char *pCommand, char *szBuffer, int nBufferSize);
int TranslateFileStringW(wchar_t *wpCommand, wchar_t *wszBuffer, int nBufferSize);
void ActivateKeyboard(int nKeybLayout);
void ActivateWindow(HWND hWnd);
HWND NextDialog(BOOL bPrevious);
HWND NextClone(BOOL bPrevious);
void DestroyEdit(DWORD dwFlags, HWND *hWndEdit, HWND *hWndMaster, HWND *hWndClone1, HWND *hWndClone2, HWND *hWndClone3);
void ResizeEdit(HWND hWndEdit, HWND hWndMaster, HWND hWndClone1, HWND hWndClone2, HWND hWndClone3, int X, int Y, int nWidth, int nHeight, RECT *rcMasterWindow, RECT *rcEditWindow, BOOL bTest);
void UpdateSize();
BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcInit, RECT *rcCurrent, DWORD dwFlags, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetMovingRect(DOCK *dkData, POINT *pt, MINMAXINFO *mmi, RECT *rcScreen);
void DrawMovingRect(RECT *rcScreen);
int GetMouseEdge(HWND hWnd, POINT *pt);
void SetEdgeCursor(int nEdge);
int GetOppEdge(int nEdge);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL ScreenToClientRect(HWND hWnd, RECT *rc);
BOOL ClientToScreenRect(HWND hWnd, RECT *rc);
BOOL EnsureWindowInMonitor(RECT *rcWindow);
void UpdateTitleA(HWND hWndEditParent, char *szFile);
void UpdateTitleW(HWND hWndEditParent, wchar_t *wszFile);
void UpdateTabs(HWND hWnd);
int GetTabItemFromParam(HWND hWnd, LPARAM lParam);
int GetTabItemFromPoint(HWND hWnd, POINT *pt);
int SelectTabItem(HWND hWnd, int nIndex);
int MoveTabItemA(HWND hWnd, int nIndexOld, int nIndexNew);
int MoveTabItemW(HWND hWnd, int nIndexOld, int nIndexNew);
BOOL DeleteTabItem(HWND hWnd, int nIndex);
void FreeMemorySearchA();
void FreeMemorySearchW();
void FreeMemoryRecentFilesA();
void FreeMemoryRecentFilesW();
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
