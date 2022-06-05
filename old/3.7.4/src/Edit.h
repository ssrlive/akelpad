#ifndef __EDIT_H__
#define __EDIT_H__


//// Defines

#define APP_MAIN_CLASSA              "AkelPad3"
#define APP_MAIN_CLASSW             L"AkelPad3"
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
#define APP_ABOUT_VERSIONA           "AkelPad 3.7.4"
#define APP_ABOUT_VERSIONW          L"AkelPad 3.7.4"
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

#ifndef WM_MOUSEWHEEL
  #define WM_MOUSEWHEEL 0x020A
#endif
#ifndef IMF_AUTOFONT
  #define IMF_AUTOFONT 0x0002
#endif
#ifndef IMF_AUTOFONTSIZEADJUST
  #define IMF_AUTOFONTSIZEADJUST 0x0010
#endif
#ifndef SPI_GETWHEELSCROLLLINES
  #define SPI_GETWHEELSCROLLLINES 0x0068
#endif
#ifndef BIF_NEWDIALOGSTYLE
  #define BIF_NEWDIALOGSTYLE 0x0040
#endif
#ifndef WBF_ISWHITE
  #define WBF_ISWHITE ((BYTE)0x10)
#endif
#ifndef TCS_BOTTOM
  #define TCS_BOTTOM 0x0002
#endif
#ifndef TCS_FLATBUTTONS
  #define TCS_FLATBUTTONS 0x0008
#endif
#ifndef TCS_EX_FLATSEPARATORS
  #define TCS_EX_FLATSEPARATORS 0x00000001
#endif
#ifndef TCM_SETEXTENDEDSTYLE
  #define TCM_SETEXTENDEDSTYLE (TCM_FIRST + 52)
#endif
#ifndef LVS_EX_GRIDLINES
  #define LVS_EX_GRIDLINES 0x00000001
#endif
#ifndef LVS_EX_CHECKBOXES
  #define LVS_EX_CHECKBOXES 0x00000004
#endif
#ifndef LVS_EX_FULLROWSELECT
  #define LVS_EX_FULLROWSELECT 0x00000020
#endif
#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
  #define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#endif
#ifndef PROPSHEETHEADERA_V1_SIZE
  #define PROPSHEETHEADERA_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERA, pfnCallback)
#endif
#ifndef PROPSHEETHEADERW_V1_SIZE
  #define PROPSHEETHEADERW_V1_SIZE CCSIZEOF_STRUCT(PROPSHEETHEADERW, pfnCallback)
#endif
//const CLSID IID_IRichEditOle={0x00020D00, 0x00, 0x00, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
//const CLSID IID_IRichEditOleCallback={0x00020D03, 0x00, 0x00, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

#ifndef mod
  #define mod(a)  ((((int)(a)) < 0) ? (0 - ((int)(a))) : (a))
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
#define MENU_VIEW_LANGUAGE_POSITION     11

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

//Search/Replace options
#define FR_UP          0x00100000
#define FR_BEGINNING   0x00200000
#define FR_SELECTION   0x00400000
#define FR_ESCAPESEQ   0x00800000
#define FR_ALLFILES    0x01000000

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

//Go to line
#define NT_LINE    1
#define NT_OFFSET  2

//File types association
#define AE_ASSOCIATE     0x00000001
#define AE_DEASSOCIATE   0x00000002
#define AE_OPEN          0x00000004
#define AE_EDIT          0x00000008
#define AE_PRINT         0x00000010

//Plugins list
#define LVSI_FUNCTION_NAME     0
#define LVSI_FUNCTION_HOTKEY   1
#define LVSI_FUNCTION_STATUS   2

//DIALOGRESIZE
#define DRS_SIZE  0x1 //Resize control
#define DRS_MOVE  0x2 //Move control
#define DRS_X     0x4 //X value
#define DRS_Y     0x8 //Y value


//// Structures

typedef BOOL (CALLBACK *EXPORTNAMESPROC)(char *, LPARAM);

typedef struct _HINISECTION {
  struct _HINISECTION *next;
  struct _HINISECTION *prev;
  char *szSection;
  wchar_t *wszSection;
  int nSectionAnsiBytes;
  int nSectionUnicodeBytes;
  HSTACK hKeysStack;
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

typedef struct _BUFFERSTREAMDATA {
  wchar_t *wpData;
  int nDataLen;
  int nCount;
} BUFFERSTREAMDATA;

typedef struct _FILESTREAMDATA {
  HWND hWnd;
  HANDLE hFile;
  int nCodePage;
  DWORD dwFlags;
  int nBytesMax;
  BOOL bResult;
} FILESTREAMDATA;

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


typedef struct {
  HWND *lpWnd;   //Control window
  DWORD dwType;  //See DRS_* defines
} DIALOGRESIZE;


//// Functions prototype

HWND CreateEditWindowA(HWND hWndParent);
HWND CreateEditWindowW(HWND hWndParent);

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
BOOL DoFilePrintA(HWND hWnd, BOOL bSilent);
BOOL DoFilePrintW(HWND hWnd, BOOL bSilent);
BOOL DoFileExitA();
BOOL DoFileExitW();
void DoEditUndo(HWND hWnd);
void DoEditRedo(HWND hWnd);
void DoEditCutA(HWND hWnd);
void DoEditCutW(HWND hWnd);
void DoEditCopy(HWND hWnd);
BOOL DoEditPaste(HWND hWnd, BOOL bAnsi);
void DoEditClearA(HWND hWnd);
void DoEditClearW(HWND hWnd);
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
BOOL DoViewColorA(HWND hWndOwner, COLORREF *crColor);
BOOL DoViewColorW(HWND hWndOwner, COLORREF *crColor);
void DoViewFontSizeA(HWND hWnd, int nAction);
void DoViewFontSizeW(HWND hWnd, int nAction);
void DoViewAlignmentA(HWND hWnd, WORD wAlignment);
void DoViewAlignmentW(HWND hWnd, WORD wAlignment);
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
void DoWindowSelectWindowA();
void DoWindowSelectWindowW();
void DoHelpAboutA();
void DoHelpAboutW();
void DoNonMenuDelLineA(HWND hWnd);
void DoNonMenuDelLineW(HWND hWnd);

BOOL OpenIniA(HSTACK *hIniStack, char *pFile, BOOL bCreate);
BOOL OpenIniW(HSTACK *hIniStack, wchar_t *wpFile, BOOL bCreate);
BOOL SaveIniA(HSTACK *hIniStack, char *pFile);
BOOL SaveIniW(HSTACK *hIniStack, wchar_t *wpFile);
BOOL ReadIni(HSTACK *hIniStack, HANDLE hFile);
BOOL WriteIni(HSTACK *hIniStack, HANDLE hFile);
HINISECTION* StackGetIniSectionA(HSTACK *hIniStack, char *pSection, int nSectionLen);
HINISECTION* StackGetIniSectionW(HSTACK *hIniStack, wchar_t *wpSection, int nSectionLen);
HINIKEY* StackGetIniKeyA(HINISECTION *lpIniSection, char *pKey, int nKeyLen);
HINIKEY* StackGetIniKeyW(HINISECTION *lpIniSection, wchar_t *wpKey, int nKeyLen);
int IniGetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
int IniGetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueA(HSTACK *hIniStack, char *pSection, char *pKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
BOOL IniSetValueW(HSTACK *hIniStack, wchar_t *wpSection, wchar_t *wpKey, int nType, unsigned char *lpData, DWORD dwDataBytes);
void StackDeleteIniKey(HINISECTION *lpIniSection, HINIKEY *lpIniKey);
void StackDeleteIniSection(HSTACK *hIniStack, HINISECTION *lpIniSection, BOOL bClear);
void StackFreeIni(HSTACK *hIniStack);
DWORD HexStrToDataA(char *pHexStr, unsigned char *lpData, DWORD dwDataBytes);
DWORD HexStrToDataW(wchar_t *wpHexStr, unsigned char *lpData, DWORD dwDataBytes);

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

int OpenDocumentA(HWND hWnd, char *szFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
int OpenDocumentW(HWND hWnd, wchar_t *wszFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
void FileStreamIn(FILESTREAMDATA *lpData);
DWORD CALLBACK InputStreamCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
int SaveDocumentA(HWND hWnd, char *szFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
int SaveDocumentW(HWND hWnd, wchar_t *wszFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
void FileStreamOut(FILESTREAMDATA *lpData);
DWORD CALLBACK OutputStreamCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
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

UINT_PTR CALLBACK CodePageDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
UINT_PTR CALLBACK CodePageDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
unsigned int UTF8toUTF16(const unsigned char *pMultiString, unsigned int nMultiStringLen, unsigned int *nMultiStringDone,  wchar_t *wszWideString, unsigned int nWideStringMax);
unsigned int UTF16toUTF8(const wchar_t *wpWideString, unsigned int nWideStringLen, unsigned int *nWideStringDone, char *szMultiString, unsigned int nMultiStringMax);
void ChangeByteOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
BOOL CheckCodePage(int nCodePage);
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
int _FindTextA(HWND hWnd, DWORD dwFlags, char *pFindIt);
int _FindTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt);
int _ReplaceTextA(HWND hWnd, DWORD dwFlags, char *pFindIt, char *pReplaceWith, BOOL bAll, int *nReplaceCount);
int _ReplaceTextW(HWND hWnd, DWORD dwFlags, wchar_t *wpFindIt, wchar_t *wpReplaceWith, BOOL bAll, int *nReplaceCount);
int _StrReplaceA(char *pText, char *pIt, char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult, char **ppMin, char **ppMax, char **ppFirstVisible);
int _StrReplaceW(wchar_t *wpText, wchar_t *wpIt, wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult, wchar_t **wppMin, wchar_t **wppMax, wchar_t **wppFirstVisible);
BOOL EscapeStringToEscapeDataA(char *pInput, char *szOutput);
BOOL EscapeStringToEscapeDataW(wchar_t *wpInput, wchar_t *wszOutput);
void EscapeDataToEscapeStringW(wchar_t *wpInput, wchar_t *wszOutput);

void ReplaceSelA(HWND hWnd, char *pData, int nDataLen);
void ReplaceSelW(HWND hWnd, wchar_t *wpData, int nDataLen);
wchar_t* GetSelTextW(HWND hWnd, int *nTextLen);
DWORD CALLBACK GetSelTextCallbackW(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
void SetTextSel(HWND hWnd, int nMin, int nMax);
int GetTextLength(HWND hWnd);
int GetRangeTextA(HWND hWnd, int nMin, int nMax, char **pText);
int GetRangeTextW(HWND hWnd, int nMin, int nMax, wchar_t **wpText);
BOOL FreeText(LPVOID pText);
BOOL PasteInEditAsRichEdit(HWND hWnd);
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

BOOL isInDelimiterList(wchar_t c);
BOOL isInWhitespaceList(wchar_t c);
int CALLBACK EditWordBreakProc(LPTSTR pchText, int ichCurrent, int cch, int code);

void RecodeTextW(HWND hWnd, int nCodePageFrom, int nCodePageTo);
BOOL CALLBACK RecodeDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK RecodeDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK PluginsDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PluginsDlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
BOOL CALLBACK OptionsAdvanced1DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvanced1DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvanced2DlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OptionsAdvanced2DlgProcW(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

void SetSelectionStatusA(HWND hWnd, CHARRANGE *cr);
void SetSelectionStatusW(HWND hWnd, CHARRANGE *cr);
void SetModifyStatusA(HWND hWnd, BOOL bState, BOOL bFirst);
void SetModifyStatusW(HWND hWnd, BOOL bState, BOOL bFirst);
void SetNewLineStatusA(HWND hWnd, BOOL bState, BOOL bFirst);
void SetNewLineStatusW(HWND hWnd, BOOL bState, BOOL bFirst);
void SetInsertStateStatusA(HWND hWnd, BOOL bState, BOOL bFirst);
void SetInsertStateStatusW(HWND hWnd, BOOL bState, BOOL bFirst);
void SetCodePageStatusA(int nCodePage, BOOL bBOM, BOOL bFirst);
void SetCodePageStatusW(int nCodePage, BOOL bBOM, BOOL bFirst);

int _SetLayout(HWND hWnd);
void RestoreLayout(HWND hWnd, DWORD dwLayout);
void SaveCharScroll(HWND hWnd, RECT *rc, int *nBeforeChar);
void RestoreCharScroll(HWND hWnd, RECT *rc, int *nBeforeChar);
void SaveLineScroll(HWND hWnd, RECT *rc, int *nBeforeLine);
void RestoreLineScroll(HWND hWnd, RECT *rc, int *nBeforeLine);
BOOL SetChosenFontA(HWND hWnd, LOGFONTA *lfA);
BOOL SetChosenFontW(HWND hWnd, LOGFONTW *lfW);
void SetChosenFontColorA(HWND hWnd, COLORREF crFont);
void SetChosenFontColorW(HWND hWnd, COLORREF crFont);
void SetChosenFontSizeA(HWND hWnd, int nPixels);
void SetChosenFontSizeW(HWND hWnd, int nPixels);
void SetTabStopsA(HWND hWnd, int nTabStops, BOOL bSetRedraw);
void SetTabStopsW(HWND hWnd, int nTabStops, BOOL bSetRedraw);
int GetCharWidthTwipsA(HWND hWnd);
int GetCharWidthTwipsW(HWND hWnd);
BOOL GetCharColor(HWND hWnd, CHARCOLOR *cc);
BOOL InsertTabStopW(HWND hWnd);
BOOL IndentTabStopW(HWND hWnd, int nAction);
BOOL AutoIndentA(HWND hWnd, CHARRANGE *cr);
BOOL AutoIndentW(HWND hWnd, CHARRANGE *cr);
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
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2);
int TranslateFileStringA(char *pCommand, char *szBuffer, int nBufferSize);
int TranslateFileStringW(wchar_t *wpCommand, wchar_t *wszBuffer, int nBufferSize);
void ActivateWindow(HWND hWnd);
HWND NextDialog(BOOL bPrevious);
void UpdateEdit(HWND hWnd);
void DestroyEdit(HWND *hWndEdit);
void ResizeEdit(HWND hWnd, int X, int Y, int nWidth, int nHeight);
void UpdateSize();
BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcInit, RECT *rcCurrent, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetMovingRect(DOCK *dkData, POINT *pt, MINMAXINFO *mmi, RECT *rcScreen);
void DrawMovingRect(RECT *rcScreen);
int GetMouseEdge(HWND hWnd, POINT *pt);
void SetEdgeCursor(int nEdge);
int GetOppEdge(int nEdge);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL ScreenToClientRect(HWND hWnd, RECT *rc);
BOOL ClientToScreenRect(HWND hWnd, RECT *rc);
int RectW(const RECT *rc);
int RectH(const RECT *rc);
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


//// OLE Drag'n'Drop

#if defined(__cplusplus)

typedef struct _IRichEditOleCallbackVtbl {
  // IUnknown implementation
  HRESULT (WINAPI *QueryInterface)(IRichEditOleCallback *, REFIID, void **);
  ULONG (WINAPI *AddRef)(IRichEditOleCallback *);
  ULONG (WINAPI *Release)(IRichEditOleCallback *);

  // Methods of the IRichEditOleCallback interface
  HRESULT (WINAPI *GetNewStorage)(IRichEditOleCallback *, LPSTORAGE FAR *);
  HRESULT (WINAPI *GetInPlaceContext)(IRichEditOleCallback *, LPOLEINPLACEFRAME FAR *, LPOLEINPLACEUIWINDOW FAR *, LPOLEINPLACEFRAMEINFO);
  HRESULT (WINAPI *ShowContainerUI)(IRichEditOleCallback *, BOOL);
  HRESULT (WINAPI *QueryInsertObject)(IRichEditOleCallback *, LPCLSID, LPSTORAGE, LONG);
  HRESULT (WINAPI *DeleteObject)(IRichEditOleCallback *, LPOLEOBJECT);
  HRESULT (WINAPI *QueryAcceptData)(IRichEditOleCallback *, LPDATAOBJECT, CLIPFORMAT FAR *, DWORD, BOOL, HGLOBAL);
  HRESULT (WINAPI *ContextSensitiveHelp)(IRichEditOleCallback *, BOOL);
  HRESULT (WINAPI *GetClipboardData)(IRichEditOleCallback *, CHARRANGE FAR *, DWORD, LPDATAOBJECT FAR *);
  HRESULT (WINAPI *GetDragDropEffect)(IRichEditOleCallback *, BOOL, DWORD, LPDWORD);
  HRESULT (WINAPI *GetContextMenu)(IRichEditOleCallback *, WORD, LPOLEOBJECT, CHARRANGE FAR *, HMENU FAR *);
} IRichEditOleCallbackVtbl;

#endif // defined(__cplusplus)

typedef struct _REOLE {
  struct IRichEditOleCallbackVtbl *lpTable;
  ULONG uRefCount;
} REOLE;

ULONG WINAPI REOLE_AddRef(IRichEditOleCallback *lpTable);
ULONG WINAPI REOLE_Release(IRichEditOleCallback *lpTable);
HRESULT WINAPI REOLE_QueryInterface(IRichEditOleCallback *lpTable, REFIID riid, void **ppvObj);
HRESULT WINAPI REOLE_GetNewStorage(IRichEditOleCallback * lpTable, LPSTORAGE FAR *lplpstg);
HRESULT WINAPI REOLE_GetInPlaceContext(IRichEditOleCallback * lpTable, LPOLEINPLACEFRAME FAR *lplpFrame, LPOLEINPLACEUIWINDOW FAR *lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
HRESULT WINAPI REOLE_ShowContainerUI(IRichEditOleCallback * lpTable, BOOL fShow);
HRESULT WINAPI REOLE_QueryInsertObject(IRichEditOleCallback * lpTable, LPCLSID lpclsid, LPSTORAGE lpstg, LONG cp);
HRESULT WINAPI REOLE_DeleteObject(IRichEditOleCallback * lpTable, LPOLEOBJECT lpoleobj);
HRESULT WINAPI REOLE_QueryAcceptData(IRichEditOleCallback * lpTable, LPDATAOBJECT lpdataobj, CLIPFORMAT FAR *lpcfFormat, DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
HRESULT WINAPI REOLE_ContextSensitiveHelp(IRichEditOleCallback * lpTable, BOOL fEnterMode);
HRESULT WINAPI REOLE_GetClipboardData(IRichEditOleCallback * lpTable, CHARRANGE FAR *lpchrg, DWORD reco, LPDATAOBJECT FAR *lplpdataobj);
HRESULT WINAPI REOLE_GetDragDropEffect(IRichEditOleCallback * lpTable, BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect);
HRESULT WINAPI REOLE_GetContextMenu(IRichEditOleCallback * lpTable, WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE FAR *lpchrg, HMENU FAR *lphmenu);
HWND RealChildWindowFromPoint95(HWND hWndParent, POINT pt);

#endif
