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
#define COMMANDLINE_SIZE           32768
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
  HSTACK hStack;
  wchar_t wszIniFile[MAX_PATH];
} INIHANDLE;

typedef struct _HDOCK {
  HSTACK hStack;
  BOOL bSizing;
  int nSizingSide;
} HDOCK;

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

FRAMEDATA* GetFrameDataFromEdit(HWND hWndEditCtrl);
BOOL CreateEditWindow(FRAMEDATA *lpFrameNew, FRAMEDATA *lpFrameOld, HWND *hWndEdit, HANDLE *hDataEdit);
FRAMEDATA* CreateFrameData(FRAMEDATA *lpFrameSource, HWND hWndEditParent);
void CopyFrameData(FRAMEDATA *lpFrameTarget, FRAMEDATA *lpFrameSource);
void SaveFrameData(FRAMEDATA *lpFrame);
void RestoreFrameData(FRAMEDATA *lpFrame);
BOOL CreateMdiFrameWindow(RECT *rcRect);
void ActivateMdiFrameWindow(FRAMEDATA *lpFrame);
FRAMEDATA* NextMdiFrameWindow(FRAMEDATA *lpFrame, BOOL bPrev);
int DestroyMdiFrameWindow(FRAMEDATA *lpFrame, int nTabItem);

BOOL DoFileNew();
HWND DoFileNewWindow(DWORD dwAddFlags);
BOOL CALLBACK EnumThreadProc(HWND hwnd, LPARAM lParam);
BOOL DoFileOpen();
int DoFileReopenAs(DWORD dwFlags, int nCodePage, BOOL bBOM);
BOOL DoFileSave();
BOOL DoFileSaveAs(int nDialogCodePage, BOOL bDialogBOM);
void DoFileSaveAllAs();
BOOL DoFilePageSetup(HWND hWndOwner);
int DoFilePrint(HWND hWnd, BOOL bSilent);
void DoFilePreview(HWND hWnd);
BOOL DoFileExit();
BOOL DoFileClose();
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
void DoEditFindNextDown(HWND hWnd);
void DoEditFindNextUp(HWND hWnd);
void DoEditReplace();
void DoEditGoToLine();
BOOL DoViewFont(HWND hWndOwner, LOGFONTW *lfFont);
void DoViewColors();
void DoViewFontSize(HWND hWnd, int nAction);
void DoViewWordWrap(HWND hWnd, BOOL bState, BOOL bFirst);
void DoViewSplitWindow(BOOL bState, WPARAM wParam);
void DoViewOnTop(BOOL bState, BOOL bFirst);
void DoViewShowStatusBar(BOOL bState, BOOL bFirst);
BOOL DoSettingsExec();
void DoSettingsReadOnly(HWND hWnd, BOOL bState, BOOL bFirst);
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

BOOL OpenIniA(HSTACK *hIniStack, const char *pFile, BOOL bCreate);
BOOL OpenIniW(HSTACK *hIniStack, const wchar_t *wpFile, BOOL bCreate);
BOOL SaveIniA(HSTACK *hIniStack, const char *pFile);
BOOL SaveIniW(HSTACK *hIniStack, const wchar_t *wpFile);
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

DWORD ReadOptionW(HANDLE lpHandle, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
DWORD SaveOptionW(HANDLE lpHandle, wchar_t *wpParam, DWORD dwType, void *lpData, DWORD dwSize);
void ReadOptions();
void RegisterPluginsHotkeys();
void RegReadSearch();
BOOL SaveOptions();

int OpenDocument(HWND hWnd, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM);
void FileStreamIn(FILESTREAMDATA *lpData);
DWORD CALLBACK InputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufDone);
DWORD ReadFileContent(HANDLE hFile, DWORD dwBytesMax, int nCodePage, BOOL bBOM, wchar_t **wpContent);
int SaveDocument(HWND hWnd, const wchar_t *wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags);
void FileStreamOut(FILESTREAMDATA *lpData);
DWORD CALLBACK OutputStreamCallback(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufDone);
BOOL OpenDirectory(wchar_t *wpPath, BOOL bSubDir);
void DropFiles(HDROP hDrop);
BOOL CALLBACK SaveAllAsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

unsigned int CALLBACK PrintPageSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL GetPrinterA(HWND hWndOwner, PRINTINFO *prninfo, BOOL bSilent);
BOOL GetPrinterW(HWND hWndOwner, PRINTINFO *prninfo, BOOL bSilent);
DWORD GetMappedPrintWidth(HWND hWnd);
BOOL UpdateMappedPrintWidth(HWND hWnd);
int PrintDocumentA(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage);
int PrintDocumentW(HWND hWnd, AEPRINT *prn, DWORD dwFlags, int nInitPage);
BOOL PrintHeadlineA(HDC hDC, RECT *rc, char *pHeadline, int nPageNumber);
BOOL PrintHeadlineW(HDC hDC, RECT *rc, wchar_t *wpHeadline, int nPageNumber);
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
void FillListboxCodepage(HWND hWnd, int *lpCodepageList);
void ClearCombobox(HWND hWnd);
void ClearListbox(HWND hWnd);
int GetComboboxCodepage(HWND hWnd);
int GetListboxCodepage(HWND hWnd);
int SelectComboboxCodepage(HWND hWnd, int nCodepage);
int SelectListboxCodepage(HWND hWnd, int nCodepage);
void GetListboxCodepageList(HWND hWnd, int **lpCodepageList);
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
unsigned int UTF8toUTF16(const unsigned char *pMultiString, unsigned int nMultiStringLen, unsigned int *nMultiStringDone,  wchar_t *wszWideString, unsigned int nWideStringMax);
unsigned int UTF16toUTF8(const wchar_t *wpWideString, unsigned int nWideStringLen, unsigned int *nWideStringDone, char *szMultiString, unsigned int nMultiStringMax);
void ChangeByteOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
BOOL IsCodePageValid(int nCodePage);
unsigned int TranslateNewLinesToUnixW(wchar_t *wszWideString, unsigned int nWideStringLen);

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

BOOL CALLBACK GoToLineDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL RecentFilesAlloc();
void RecentFilesZero();
void RecentFilesRead();
BOOL RecentFilesGet(const wchar_t *wpFile, int *nPosition, int *nCodePage);
BOOL RecentFilesUpdate(const wchar_t *wpFile, int nPosition, int nCodePage);
int RecentFilesDeleteDead();
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
void ReadThemes();
BOOL SaveThemes();

BOOL CALLBACK PluginsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);
void FillPluginList(HWND hWnd);
BOOL CALLBACK FillPluginListProc(char *pExportName, LPARAM lParam);
PLUGINLISTITEM* GetPluginListItem(HSTACK *hStack, int nIndex);
void FreePluginList(HSTACK *hStack);
int GetHotkeyString(WORD wHotkey, wchar_t *wszString);
BOOL ParsePluginNameW(const wchar_t *wpFullName, wchar_t *wszPlugin, wchar_t *wszFunction);
void CallPluginsOnStart(HSTACK *hStack);
int CallPluginSend(PLUGINFUNCTION **ppfElement, PLUGINCALLSENDW *pcs, BOOL bOnStart);
int CallPlugin(PLUGINFUNCTION *lpPluginFunction, PLUGINCALLSENDW *pcs, BOOL bOnStart);
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
PLUGINFUNCTION* StackPluginFind(HSTACK *hStack, const wchar_t *wpPluginFunction, int nPluginFunctionLen);
PLUGINFUNCTION* StackHotkeyFind(HSTACK *hStack, WORD wHotkey);
PLUGINFUNCTION* StackPluginAdd(HSTACK *hStack, const wchar_t *wpString, int nStringLen, WORD wHotkey, BOOL bOnStart, PLUGINPROC PluginProc, void *lpParameter);
void StackPluginDelete(HSTACK *hStack, void *lpElement);
BOOL StackPluginSave(HSTACK *hStack);
void StackPluginCleanUp(HSTACK *hStack, BOOL bDeleteNonExistentDLL);
void StackPluginFree(HSTACK *hStack);
BOOL IsMainFunctionW(const wchar_t *wpFunction);
BOOL TranslateDialogA(HDOCK *hDocks, LPMSG lpMsg);
BOOL TranslateDialogW(HDOCK *hDocks, LPMSG lpMsg);
BOOL TranslatePlugin(LPMSG lpMsg);
BOOL TranslateHotkey(HSTACK *hStack, LPMSG lpMsg);

BOOL OpenDocumentSend(HWND hWnd, HWND hWndEdit, const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL bOtherProcess);

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
void FillMdiListListbox(HWND hWnd, BOOL bSort, BOOL bOnlyModified);
int MoveListboxItem(HWND hWnd, int nOldIndex, int nNewIndex);
BOOL ShiftListboxSelItems(HWND hWnd, BOOL bMoveDown);
BOOL SaveListboxSelItems(HWND hWnd);
void ArrangeListboxSelItems(HWND hWnd, int nBar);
BOOL CloseListboxSelItems(HWND hWnd);
int GetListboxSelItems(HWND hWnd, int **lpSelItems);
void FreeListboxSelItems(int **lpSelItems);

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

FRAMEDATA* StackFrameInsert(HSTACK *hStack);
FRAMEDATA* StackFrameGetByIndex(HSTACK *hStack, int nIndex);
FRAMEDATA* StackFrameGetByHandle(HSTACK *hStack, HANDLE hDataHandle);
FRAMEDATA* StackFrameGetByName(HSTACK *hStack, const wchar_t *wpFileName, int nFileNameLen);
void StackFrameMove(HSTACK *hStack, FRAMEDATA *lpFrame, int nIndex);
void StackFrameDelete(HSTACK *hStack, FRAMEDATA *lpFrame);
void StackFramesFree(HSTACK *hStack);

void SetSelectionStatus(HWND hWnd, AECHARRANGE *cr, AECHARINDEX *ci);
void SetModifyStatus(HWND hWnd, BOOL bState);
void SetOvertypeStatus(HWND hWnd, BOOL bState);
void SetNewLineStatus(HWND hWnd, int nState, DWORD dwFlags);
void SetCodePageStatus(HWND hWnd, int nCodePage, BOOL bBOM);

const wchar_t* GetAssociatedIconW(const wchar_t *wpFile, wchar_t *wszIconFile, int *nIconIndex, HICON *phiconLarge, HICON *phiconSmall);
void AssociateFileTypesW(HINSTANCE hInstance, const wchar_t *wpFileTypes, DWORD dwFlags);
ASSOCICON* StackIconInsert(HSTACK *hStack, const wchar_t *wpFile, int nFileLen);
ASSOCICON* StackIconGet(HSTACK *hStack, const wchar_t *wpFile, int nFileLen, const wchar_t *wpExt);
void StackIconsFree(HSTACK *hStack);

HFONT SetChosenFont(HWND hWnd, LOGFONTW *lfFont);
FONTITEM* StackFontItemInsert(HSTACK *hStack, LOGFONTW *lfFont);
FONTITEM* StackFontItemGet(HSTACK *hStack, LOGFONTW *lfFont);
void StackFontItemsFree(HSTACK *hStack);

BOOL GetEditInfo(HWND hWnd, EDITINFO *ei);
DWORD IsEditActive(HWND hWnd);
void UpdateShowHScroll(HWND hWnd);
int SaveLineScroll(HWND hWnd);
void RestoreLineScroll(HWND hWnd, int nBeforeLine);
DWORD ScrollCaret(HWND hWnd);
BOOL SelectColorDialogA(HWND hWndOwner, COLORREF *crColor);
BOOL SelectColorDialog(HWND hWndOwner, COLORREF *crColor);
BOOL GetCharColor(HWND hWnd, CHARCOLOR *cc);
void SetMarker(HWND hWnd, DWORD dwPos);
void SetWordWrap(HWND hWnd, DWORD dwType, DWORD dwLimit);
void SetMargins(HWND hWnd, DWORD dwNewMargins, DWORD dwOldMargins);
void SetTabStops(HWND hWnd, int nTabStops, BOOL bSetRedraw);
BOOL InsertTabStopW(HWND hWnd);
BOOL IndentTabStopW(HWND hWnd, int nAction);
BOOL AutoIndent(HWND hWnd, AECHARRANGE *cr);
wchar_t* GetCommandLineWide(void);
char* GetCommandLineParamsA();
wchar_t* GetCommandLineParamsW();
int GetCommandLineArgA(const char *pCmdLine, char *szArgName, int nArgNameLen, const char **pArgOption, int *nArgOptionLen, const char **pNextArg, BOOL bParseAsNotepad);
int GetCommandLineArgW(const wchar_t *wpCmdLine, wchar_t *wszArgName, int nArgNameLen, const wchar_t **wpArgOption, int *nArgOptionLen, const wchar_t **wpNextArg, BOOL bParseAsNotepad);
int SetUrlPrefixes(HWND hWnd, const wchar_t *wpPrefixes);
BOOL IsReadOnly(HWND hWnd);
BOOL SaveChanged();
int IsFileW(const wchar_t *wpFile);
BOOL IsPathFullW(const wchar_t *wpPath);
int GetExeDirW(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen);
int GetFileDirW(const wchar_t *wpFile, wchar_t *wszFileDir, int nFileDirLen);
BOOL GetFullNameW(const wchar_t *wpFile, wchar_t *wszFileFullName, int nFileMax);
const wchar_t* GetFileNameW(const wchar_t *wpFile);
int GetBaseNameW(const wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen);
const wchar_t* GetFileExtW(const wchar_t *wpFile);
void TrimModifyStateW(wchar_t *wszFile);
BOOL GetFileWriteTimeWide(const wchar_t *wpFile, FILETIME *ft);
BOOL GetFileVersionA(char *pFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild);
BOOL GetFileVersionW(wchar_t *wpFile, int *nMajor, int *nMinor, int *nRelease, int *nBuild);
int VersionCompare(DWORD dwVersion1, DWORD dwVersion2);
int TranslateFileStringW(const wchar_t *wpCommand, wchar_t *wszBuffer, int nBufferSize);
void ActivateKeyboard(DWORD dwInputLocale);
void ActivateWindow(HWND hWnd);
HWND NextDialog(BOOL bPrevious);
HWND NextClone(BOOL bPrevious);
void DestroyEdit(DWORD dwFlags, FRAMEDATA *lpFrame);
void ResizeEdit(FRAMEDATA *lpFrame, int X, int Y, int nWidth, int nHeight, BOOL bTest);
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
void UpdateTitle(FRAMEDATA *lpFrame, const wchar_t *wszFile);
void UpdateTabs(HWND hWnd);
int AddTabItem(HWND hWnd, HICON hIcon, LPARAM lParam);
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
void FreeWideStr(wchar_t *wpVar);

#endif
