#ifndef __AKELDLL_H__
#define __AKELDLL_H__


//// Version

#ifndef MAKE_IDENTIFIER
  #define MAKE_IDENTIFIER(a, b, c, d)  ((DWORD)MAKELONG(MAKEWORD(a, b), MAKEWORD(c, d)))
#endif

#define AKELDLL MAKE_IDENTIFIER(1, 2, 0, 0)


//// Defines

//Control IDs
#define ID_EDIT           10001
#define ID_STATUS         10002
#define ID_TAB            10003
#define ID_PROGRESS       10004
#define ID_FIRSTMDI       10010

//Sizes
#define WORD_DELIMITERS_SIZE    128
#define WRAP_DELIMITERS_SIZE    128
#define URL_PREFIXES_SIZE       128
#define URL_DELIMITERS_SIZE     128

//Unload plugin flag
#define UD_FAILED                -1  //Operation failed. Don't use it.
#define UD_UNLOAD                 0  //Unload plugin (default)
#define UD_NONUNLOAD_ACTIVE     0x1  //Don't unload plugin and set active status
#define UD_NONUNLOAD_NONACTIVE  0x2  //Don't unload plugin and set non-active status
#define UD_NONUNLOAD_UNCHANGE   0x4  //Don't unload plugin and don't change active status
#define UD_HOTKEY_DODEFAULT     0x8  //Do default hotkey processing

//Open document flags
#define OD_ADT_BINARY_ERROR      0x00000001  //Check if file is binary
#define OD_ADT_REG_CODEPAGE      0x00000002  //If last open code page found in registry, then it will be
                                             //used with OD_ADT_DETECT_BOM flag, if not found, then next flags
                                             //will be used OD_ADT_DETECT_CODEPAGE|OD_ADT_DETECT_BOM
#define OD_ADT_DETECT_CODEPAGE   0x00000004  //Detect code page
#define OD_ADT_DETECT_BOM        0x00000008  //Detect BOM mark

//Open document errors
#define EOD_SUCCESS              0  //Success
#define EOD_ADT_OPEN            -1  //Autodetect codepage, can't open file
#define EOD_ADT_ALLOC           -2  //Autodetect codepage, can't allocate buffer
#define EOD_ADT_READ            -3  //Autodetect codepage, read file error
#define EOD_ADT_BINARY          -4  //Autodetect codepage, file is binary
#define EOD_OPEN                -5  //Can't open file
#define EOD_CANCEL              -6  //User press cancel
#define EOD_WINDOW_EXIST        -7  //File already opened
#define EOD_CODEPAGE_ERROR      -8  //Code page isn't implemented
#define EOD_STOP                -9  //Stopped from AKDN_OPENDOCUMENT_START
#define EOD_STREAMIN           -10  //Error in EM_STREAMIN

//Save document flags
#define SD_UPDATE            0x00000001  //Update file info
#define SD_SELECTION         0x00000002  //Save only selection

//Save document errors
#define ESD_SUCCESS          0   //Success
#define ESD_OPEN            -1  //Can't open file
#define ESD_WRITE           -2  //Can't write to file
#define ESD_READONLY        -3  //File has read-only attribute
#define ESD_CODEPAGE_ERROR  -4  //Code page isn't implemented
#define ESD_STOP            -5  //Stopped from AKDN_SAVEDOCUMENT_START
#define ESD_STREAMOUT       -6  //Error in EM_STREAMOUT

//Save settings
#define SS_REGISTRY   1  //Registry
#define SS_INI        2  //INI file

//AKD_RECENTFILES flags
#define RF_GET    1  //Retrive recent files info
#define RF_CLEAR  2  //Clear recent files

//AKD_SEARCHHISTORY flags
#define SH_GET    1  //Retrive searh strings count
#define SH_CLEAR  2  //Clear searh history

//New line format
#define NEWLINE_WIN   1  //Windows/DOS new line format (\r\n)
#define NEWLINE_UNIX  2  //Unix new line format (\n)
#define NEWLINE_MAC   3  //MacOS new line format (\r)

//Status bar position type
#define SPT_COLUMN      0x00000001  //"Line:Column". By default: "Line:Symbol".
#define SPT_LINEWRAP    0x00000002  //Wrap line numbers. By default: Non-wrap line numbers.

//INI value types
#define INI_DWORD           1
#define INI_BINARY          2
#define INI_STRINGANSI      3
#define INI_STRINGUNICODE   4

//Options flags
#define POB_READ     0x1  //Begins read options
#define POB_SAVE     0x2  //Begins save options
#define POB_CLEAR    0x4  //Begins new save options (POB_SAVE|POB_CLEAR)

//Option type
#define PO_DWORD     1  //32-bit number
#define PO_BINARY    2  //Binary data in any form
#define PO_STRING    3  //Null-terminated string

//Save plugins stack
#define DLLS_CLEAR   1  //Clear all records before saving
#define DLLS_ONEXIT  2  //Save stack on program exit

//Context menu owner
#define NCM_EDIT     1  //Edit control
#define NCM_TAB      2  //Tab control
#define NCM_STATUS   3  //Status bar control

//Find text flags
#ifndef FR_DOWN
  #define FR_DOWN      0x00000001  //Find down
#endif                             //
#ifndef FR_MATCHCASE               //
  #define FR_MATCHCASE 0x00000004  //Search is case-sensitive
#endif                             //
#define FR_UP          0x00100000  //Find up
#define FR_BEGINNING   0x00200000  //Search from beginning (usage: FR_DOWN|FR_BEGINNING)
#define FR_SELECTION   0x00400000  //Search in selection (usage: FR_DOWN|FR_SELECTION)
#define FR_ESCAPESEQ   0x00800000  //Search with escape sequences
#define FR_ALLFILES    0x01000000  //Search in all openned MDI documents (usage: FR_DOWN|FR_BEGINNING|FR_ALLFILES)

//AKD_PASTE
#define PASTE_UNICODE     0 //paste as Unicode text, if no Unicode text available ANSI text will be used (default).
#define PASTE_ANSI        1 //paste as ANSI text.
#define PASTE_SINGLELINE  2 //paste multiline text to single line edit control. All new lines replaced with '\r'.

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
#define DKF_OWNTHREAD     0x00000001  //Dock window has its own thread
#define DKF_FIXEDSIZE     0x00000002  //Dock window isn't resizeable
#define DKF_DRAGDROP      0x00000004  //Dock window has drag-and-drop support, the DOCK.rcDragDrop member is valid
#define DKF_HIDDEN        0x00000008  //Dock window isn't visible
#define DKF_NODROPLEFT    0x00000010  //Disable drag-and-drop to the left side
#define DKF_NODROPRIGHT   0x00000020  //Disable drag-and-drop to the right side
#define DKF_NODROPTOP     0x00000040  //Disable drag-and-drop to the top side
#define DKF_NODROPBOTTOM  0x00000080  //Disable drag-and-drop to the bottom side

//Dock action
#define DK_ADD         0x00000001  //Add new dockable window
#define DK_DELETE      0x00000002  //Delete dockable window
#define DK_SUBCLASS    0x00000004  //Assign dockable window handle (DOCK.hWnd)
#define DK_UNSUBCLASS  0x00000008  //Deassign dockable window handle (DOCK.hWnd)
#define DK_SETLEFT     0x00000010  //Set DKS_LEFT side
#define DK_SETRIGHT    0x00000020  //Set DKS_RIGHT side
#define DK_SETTOP      0x00000040  //Set DKS_TOP side
#define DK_SETBOTTOM   0x00000080  //Set DKS_BOTTOM side
#define DK_HIDE        0x00000100  //Hide dockable window and set DKF_HIDDEN flag
#define DK_SHOW        0x00000200  //Show dockable window and remove DKF_HIDDEN flag

//WM_INITMENU lParam
#define IMENU_EDIT     0x00000001
#define IMENU_CHECKS   0x00000004


//// Structures

#ifndef _HSTACK_STRUCT_
#define _HSTACK_STRUCT_
typedef struct {
  int first;
  int last;
} HSTACK;
#endif

typedef BOOL (CALLBACK *PLUGINPROC)(void *);
typedef void (CALLBACK *WNDPROCRET)(CWPRETSTRUCT *);

typedef struct _PLUGINVERSION {
  DWORD cb;                   //Size of the structure
  HWND hMainWnd;              //Main window
  DWORD dwAkelDllVersion;     //Current AkelDLL version. Set it to AKELDLL.
  DWORD dwExeMinVersion3x;    //Required minimum AkelPad 3.x version.
                              //Set as MAKE_IDENTIFIER(x, x, x, x) or
                              //if not supported MAKE_IDENTIFIER(-1, -1, -1, -1).
  DWORD dwExeMinVersion4x;    //Required minimum AkelPad 4.x version.
                              //Set as MAKE_IDENTIFIER(x, x, x, x) or
                              //if not supported MAKE_IDENTIFIER(-1, -1, -1, -1).
  char *pPluginName;          //Plugin unique name
} PLUGINVERSION;

typedef struct _PLUGINDATA {
  DWORD cb;                   //Size of the structure
  unsigned char *pFunction;   //Called function name, format "Plugin::Function"
                              //char *pFunction     if bOldWindows == TRUE
                              //wchar_t *pFunction  if bOldWindows == FALSE
  HINSTANCE hInstanceDLL;     //DLL instance
  void *lpPluginFunction;     //Pointer to a PLUGINFUNCTION structure
  BOOL *lpbAutoLoad;          //TRUE  if function supports autoload
                              //FALSE if function doesn't support autoload
  int nUnload;                //See UD_* defines
  BOOL bInMemory;             //Plugin already loaded
  BOOL bOnStart;              //TRUE  if plugin called on start-up
                              //FALSE if plugin called manually
  LPARAM lParam;              //Input data
  unsigned char *pAkelDir;    //AkelPad directory
                              //char *pAkelDir      if bOldWindows == TRUE
                              //wchar_t *pAkelDir   if bOldWindows == FALSE
  HINSTANCE hInstanceEXE;     //EXE instance
  HSTACK *hPluginsStack;      //Pointer to a plugins stack with PLUGINFUNCTION elements
  HWND hMainWnd;              //Main window
  HWND hWndEdit;              //Edit window
  HWND hStatus;               //StatusBar window
  HWND hMdiClient;            //MDI client window (if bMDI == TRUE)
  HWND hTab;                  //Tab window        (if bMDI == TRUE)
  HMENU hMainMenu;            //Main menu
  HMENU hMenuRecentFiles;     //Recent files menu
  HMENU hMenuLanguage;        //Language menu
  HMENU hPopupMenu;           //Right click menu
  HICON hMainIcon;            //Main window icon handle
  HACCEL hGlobalAccel;        //Global accelerator table (highest priority)
  HACCEL hMainAccel;          //Main accelerator table (lowest priority)
  BOOL bOldWindows;           //Non-Unicode Windows
  BOOL bOldRichEdit;          //riched20.dll lower then 5.30 (v3.0)
  BOOL bOldComctl32;          //comctl32.dll lower then 4.71
  BOOL bAkelEdit;             //AkelEdit control is used
  BOOL bMDI;                  //MDI mode
  int nSaveSettings;          //See SS_* defines
  unsigned char *pLangModule; //Language module
                              //char *pLangModule      if bOldWindows == TRUE
                              //wchar_t *pLangModule   if bOldWindows == FALSE
  LANGID wLangSystem;         //System language ID
} PLUGINDATA;

typedef struct _OPENDOCUMENTA {
  HWND hWnd;                   //Window to fill in, NULL for current edit window
  char szFile[MAX_PATH];       //File to open
  char szWorkDir[MAX_PATH];    //Set working directory before open, if (!*szWorkDir) then don't set
  DWORD dwFlags;               //Open flags. See OD_ADT_* defines
  int nCodePage;               //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE)
  BOOL bBOM;                   //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM)
} OPENDOCUMENTA;

typedef struct _OPENDOCUMENTW {
  HWND hWnd;                     //Window to fill in, NULL for current edit window
  wchar_t szFile[MAX_PATH];      //File to open
  wchar_t szWorkDir[MAX_PATH];   //Set working directory before open, if (!*szWorkDir) then don't set
  DWORD dwFlags;                 //Open flags. See OD_ADT_* defines
  int nCodePage;                 //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE)
  BOOL bBOM;                     //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM)
} OPENDOCUMENTW;

typedef struct _SAVEDOCUMENTA {
  char szFile[MAX_PATH];      //File to save
  int nCodePage;              //File code page
  BOOL bBOM;                  //File BOM
  DWORD dwFlags;              //See SD_* defines
} SAVEDOCUMENTA;

typedef struct _SAVEDOCUMENTW {
  wchar_t szFile[MAX_PATH];   //File to save
  int nCodePage;              //File code page
  BOOL bBOM;                  //File BOM
  DWORD dwFlags;              //See SD_* defines
} SAVEDOCUMENTW;

typedef struct _EDITINFO {
  HWND hWndEdit;              //Edit window
  unsigned char *pFile;       //Current editing file
                              //char *pFile         if bOldWindows == TRUE
                              //wchar_t *pFile      if bOldWindows == FALSE
  int nCodePage;              //Current code page
  BOOL bBOM;                  //Current BOM
  int nNewLine;               //Current new line format, see NEWLINE_* defines
  BOOL bModified;             //File has been modified
  BOOL bReadOnly;             //Read only
  BOOL bWordWrap;             //Word wrap
  BOOL bOvertypeMode;         //Overtype mode
} EDITINFO;

#ifndef __AKELEDIT_H__
  typedef struct {
    DWORD dwFlags;
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
  } AECOLORS;
#endif

typedef struct _WNDFRAMEA {
  HICON hIcon;                                        //Frame icon
  char szFile[MAX_PATH];                              //Frame file
  EDITINFO ei;                                        //Edit info
  LOGFONTA lf;                                        //Edit font
  AECOLORS aec;                                       //Edit colors
  FILETIME ft;                                        //File time
  RECT rcEditWindow;                                  //Edit RECT
  DWORD dwEditMargins;                                //Edit margins
  int nTabStopSize;                                   //Tab stop size
  BOOL bTabStopAsSpaces;                              //Insert tab stop as spaces
  int nUndoLimit;                                     //Undo limit
  BOOL bDetailedUndo;                                 //Detailed undo
  DWORD dwWrapType;                                   //Wrap type AEWW_WORD or AEWW_SYMBOL (4.x only)
  DWORD dwWrapLimit;                                  //Wrap characters limit, zero if wrap by window edge (4.x only)
  DWORD dwMarker;                                     //Vertical marker, zero if no marker set (4.x only)
  DWORD dwMappedPrintWidth;                           //Mapped prinet page width (4.x only)
  BOOL bCaretOutEdge;                                 //Allow caret moving out of the line edge (4.x only)
  BOOL bCaretVertLine;                                //Draw caret vertical line (4.x only)
  int nCaretWidth;                                    //Caret width (4.x only)
  DWORD dwLineGap;                                    //Line gap (4.x only)
  BOOL bShowURL;                                      //Show URL
  BOOL bUrlPrefixesEnable;                            //URL prefixes enable (4.x only)
  wchar_t wszUrlPrefixes[URL_PREFIXES_SIZE];          //URL prefixes (4.x only)
  BOOL bUrlDelimitersEnable;                          //URL delimiters enable (4.x only)
  wchar_t wszUrlLeftDelimiters[URL_DELIMITERS_SIZE];  //URL left delimiters (4.x only)
  wchar_t wszUrlRightDelimiters[URL_DELIMITERS_SIZE]; //URL right delimiters (4.x only)
  BOOL bWordDelimitersEnable;                         //Word delimiters enabled
  wchar_t wszWordDelimiters[WORD_DELIMITERS_SIZE];    //Word delimiters
  BOOL bWrapDelimitersEnable;                         //Wrap delimiters enabled
  wchar_t wszWrapDelimiters[WRAP_DELIMITERS_SIZE];    //Wrap delimiters
  BOOL bSplitWindow;                                  //Edit window is splited (4.x only)
  HWND hWndMaster;                                    //Master window (4.x only)
  HWND hWndClone1;                                    //Clone window one (4.x only)
  HWND hWndClone2;                                    //Clone window two (4.x only)
  HWND hWndClone3;                                    //Clone window three (4.x only)
  RECT rcMasterWindow;                                //Master window RECT (4.x only)
} WNDFRAMEA;

typedef struct _WNDFRAMEW {
  HICON hIcon;                                        //Frame icon
  wchar_t szFile[MAX_PATH];                           //Frame file
  EDITINFO ei;                                        //Edit info
  LOGFONTW lf;                                        //Edit font
  AECOLORS aec;                                       //Edit colors
  FILETIME ft;                                        //File time
  RECT rcEditWindow;                                  //Edit RECT
  DWORD dwEditMargins;                                //Edit margins
  int nTabStopSize;                                   //Tab stop size
  BOOL bTabStopAsSpaces;                              //Insert tab stop as spaces
  int nUndoLimit;                                     //Undo limit
  BOOL bDetailedUndo;                                 //Detailed undo
  DWORD dwWrapType;                                   //Wrap type AEWW_WORD or AEWW_SYMBOL (4.x only)
  DWORD dwWrapLimit;                                  //Wrap characters limit, zero if wrap by window edge (4.x only)
  DWORD dwMarker;                                     //Vertical marker, zero if no marker set (4.x only)
  DWORD dwMappedPrintWidth;                           //Mapped prinet page width (4.x only)
  BOOL bCaretOutEdge;                                 //Allow caret moving out of the line edge (4.x only)
  BOOL bCaretVertLine;                                //Draw caret vertical line (4.x only)
  int nCaretWidth;                                    //Caret width (4.x only)
  DWORD dwLineGap;                                    //Line gap (4.x only)
  BOOL bShowURL;                                      //Show URL
  BOOL bUrlPrefixesEnable;                            //URL prefixes enable (4.x only)
  wchar_t wszUrlPrefixes[URL_PREFIXES_SIZE];          //URL prefixes (4.x only)
  BOOL bUrlDelimitersEnable;                          //URL delimiters enable (4.x only)
  wchar_t wszUrlLeftDelimiters[URL_DELIMITERS_SIZE];  //URL left delimiters (4.x only)
  wchar_t wszUrlRightDelimiters[URL_DELIMITERS_SIZE]; //URL right delimiters (4.x only)
  BOOL bWordDelimitersEnable;                         //Word delimiters enabled
  wchar_t wszWordDelimiters[WORD_DELIMITERS_SIZE];    //Word delimiters
  BOOL bWrapDelimitersEnable;                         //Wrap delimiters enabled
  wchar_t wszWrapDelimiters[WRAP_DELIMITERS_SIZE];    //Wrap delimiters
  BOOL bSplitWindow;                                  //Edit window is splited (4.x only)
  HWND hWndMaster;                                    //Master window (4.x only)
  HWND hWndClone1;                                    //Clone window one (4.x only)
  HWND hWndClone2;                                    //Clone window two (4.x only)
  HWND hWndClone3;                                    //Clone window three (4.x only)
  RECT rcMasterWindow;                                //Master window RECT (4.x only)
} WNDFRAMEW;

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

typedef struct _PLUGINFUNCTIONA {
  struct _PLUGINFUNCTIONA *next;
  struct _PLUGINFUNCTIONA *prev;
  char szFunction[MAX_PATH];      //Function name, format "Plugin::Function"
  int nFunctionLen;               //Function name length
  WORD wHotkey;                   //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
  BOOL bOnStart;                  //Function autoload on start-up
  BOOL bRunning;                  //Function is running
  PLUGINPROC PluginProc;          //Function procedure
  void *lpParameter;              //Procedure parameter
} PLUGINFUNCTIONA;

typedef struct _PLUGINFUNCTIONW {
  struct _PLUGINFUNCTIONW *next;
  struct _PLUGINFUNCTIONW *prev;
  wchar_t szFunction[MAX_PATH];   //Function name, format L"Plugin::Function"
  int nFunctionLen;               //Function name length
  WORD wHotkey;                   //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
  BOOL bOnStart;                  //Function autoload on start-up
  BOOL bRunning;                  //Function is running
  PLUGINPROC PluginProc;          //Function procedure
  void *lpParameter;              //Procedure parameter
} PLUGINFUNCTIONW;

typedef struct _PLUGINCALLSENDA {
  char *pFunction;                //Function name, format "Plugin::Function"
  BOOL bOnStart;                  //TRUE  if plugin called on start-up
                                  //FALSE if plugin called manually
  LPARAM lParam;                  //Input data
  BOOL *lpbAutoLoad;              //If not NULL, then check plugin autoload
} PLUGINCALLSENDA;

typedef struct _PLUGINCALLSENDW {
  wchar_t *pFunction;             //Function name, format L"Plugin::Function"
  BOOL bOnStart;                  //TRUE  if plugin called on start-up
                                  //FALSE if plugin called manually
  LPARAM lParam;                  //Input data
  BOOL *lpbAutoLoad;              //If not NULL, then check plugin autoload
} PLUGINCALLSENDW;

typedef struct _PLUGINCALLPOSTA {
  char szFunction[MAX_PATH];      //Function name, format "Plugin::Function"
  BOOL bOnStart;                  //TRUE  if plugin called on start-up
                                  //FALSE if plugin called manually
  LPARAM lParam;                  //Input data
} PLUGINCALLPOSTA;

typedef struct _PLUGINCALLPOSTW {
  wchar_t szFunction[MAX_PATH];   //Function name, format L"Plugin::Function"
  BOOL bOnStart;                  //TRUE  if plugin called on start-up
                                  //FALSE if plugin called manually
  LPARAM lParam;                  //Input data
} PLUGINCALLPOSTW;

typedef struct _PLUGINOPTIONA {
  char *pOptionName;             //Option name.
  DWORD dwType;                  //Data type: PO_DWORD, PO_BINARY or PO_STRING.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} PLUGINOPTIONA;

typedef struct _PLUGINOPTIONW {
  wchar_t *pOptionName;          //Option name.
  DWORD dwType;                  //Data type: PO_DWORD, PO_BINARY or PO_STRING.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} PLUGINOPTIONW;

typedef struct _INIVALUEA {
  char *pSection;                //Section name.
  char *pKey;                    //Key name.
  DWORD dwType;                  //Data type: see INI_* defines.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} INIVALUEA;

typedef struct _INIVALUEW {
  wchar_t *pSection;             //Section name.
  wchar_t *pKey;                 //Key name.
  DWORD dwType;                  //Data type: see INI_* defines.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} INIVALUEW;

typedef struct _GETTEXTRANGE {
  int cpMin;                      //First character in the range. First char of text: 0.
  int cpMax;                      //Last character in the range. Last char of text: -1.
  unsigned char *pText;           //Pointer that receive allocated text. Must be deallocated with AKD_FREETEXT message.
                                  //char *pText      if bOldWindows == TRUE
                                  //wchar_t *pText   if bOldWindows == FALSE
} GETTEXTRANGE;

#ifdef __AKELEDIT_H__
typedef struct _EXGETTEXTRANGE {
  AECHARRANGE cr;                 //Characters range to retrieve
  BOOL bColumnSel;                //Column selection. If this value is ?, active column selection mode is used.
  unsigned char *pText;           //Pointer that receive allocated text. Must be deallocated with AKD_FREETEXT message.
                                  //char *pText      if bOldWindows == TRUE
                                  //wchar_t *pText   if bOldWindows == FALSE
  int nNewLine;                   //see AELB_* defines
  int nCodePage;                  //Valid if bOldWindows == TRUE. Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  char *lpDefaultChar;            //Valid if bOldWindows == TRUE. Points to the character used if a wide character cannot be represented in the specified code page. If this member is NULL, a system default value is used.
  BOOL *lpUsedDefChar;            //Valid if bOldWindows == TRUE. Points to a flag that indicates whether a default character was used. The flag is set to TRUE if one or more wide characters in the source string cannot be represented in the specified code page. Otherwise, the flag is set to FALSE. This member may be NULL.
} EXGETTEXTRANGE;
#endif

typedef struct _RECENTFILESA {
  char (*lpszRecentNames)[MAX_PATH];      //Recent files names
  DWORD *lpdwRecentPositions;             //Recent files positions
  DWORD *lpdwRecentCodepages;             //Recent files codepages
} RECENTFILESA;

typedef struct _RECENTFILESW {
  wchar_t (*lpszRecentNames)[MAX_PATH];   //Recent files names
  DWORD *lpdwRecentPositions;             //Recent files positions
  DWORD *lpdwRecentCodepages;             //Recent files codepages
} RECENTFILESW;

typedef struct _TEXTFINDA {
  DWORD dwFlags;            //See FR_* defines
  char *pFindIt;            //Find string
} TEXTFINDA;

typedef struct _TEXTFINDW {
  DWORD dwFlags;            //See FR_* defines
  wchar_t *pFindIt;         //Find string
} TEXTFINDW;

typedef struct _TEXTREPLACEA {
  DWORD dwFlags;            //See FR_* defines
  char *pFindIt;            //Find string
  char *pReplaceWith;       //Replace string
  BOOL bAll;                //Replace all
  int nChanges;             //Count of changes
} TEXTREPLACEA;

typedef struct _TEXTREPLACEW {
  DWORD dwFlags;            //See FR_* defines
  wchar_t *pFindIt;         //Find string
  wchar_t *pReplaceWith;    //Replace string
  BOOL bAll;                //Replace all
  int nChanges;             //Count of changes
} TEXTREPLACEW;

typedef struct _TEXTRECODE {
  int nCodePageFrom;        //Source code page
  int nCodePageTo;          //Target code page
} TEXTRECODE;

typedef struct _CREATEWINDOWA {
  char szClassName[MAX_PATH];   //Window class name
  char szWindowName[MAX_PATH];  //Window caption
  DWORD dwStyle;                //Window style
  int x;                        //x position
  int y;                        //y position
  int nWidth;                   //x size
  int nHeight;                  //y size
  HWND hWndParent;              //Parent window handle
  HMENU hMenu;                  //Window menu handle
  HINSTANCE hInstance;          //Program instance handle
  LPVOID lpParam;               //Creation parameters
} CREATEWINDOWA;

typedef struct _CREATEWINDOWW {
  wchar_t szClassName[MAX_PATH];    //Window class name
  wchar_t szWindowName[MAX_PATH];   //Window caption
  DWORD dwStyle;                    //Window style
  int x;                            //x position
  int y;                            //y position
  int nWidth;                       //x size
  int nHeight;                      //y size
  HWND hWndParent;                  //Parent window handle
  HMENU hMenu;                      //Window menu handle
  HINSTANCE hInstance;              //Program instance handle
  LPVOID lpParam;                   //Creation parameters
} CREATEWINDOWW;

typedef struct _DOCK {
  struct _DOCK *next;
  struct _DOCK *prev;
  DWORD dwFlags;       //Dock flags: see DKF_* defines
  HWND hWnd;           //Dock window handle
  int nSide;           //Dock side: DKS_LEFT, DKS_TOP, DKS_RIGHT or DKS_BOTTOM
  RECT rcSize;         //Dock window client RECT
  RECT rcDragDrop;     //Drag-and-drop client RECT
} DOCK;

typedef struct _POSTMESSAGE {
  HWND hWnd;                 //Window handle
  UINT uMsg;                 //Specifies the message to be sent
  WPARAM wParam;             //Specifies additional message-specific information
  LPARAM lParam;             //Specifies additional message-specific information
} POSTMESSAGE;

typedef struct _CHARCOLOR {
  int nCharPos;           //Char position
  COLORREF crText;        //Text color
  COLORREF crBk;          //Background color
} CHARCOLOR;

typedef struct _NOPENDOCUMENTA {
  char *pFile;                   //Pointer to a file string buffer
  int *nCodePage;                //Pointer to a code page variable
  BOOL *bBOM;                    //Pointer to a BOM variable
  DWORD *dwFlags;                //Pointer to a open flags variable
  BOOL bProcess;                 //TRUE   open file
                                 //FALSE  do not open file
} NOPENDOCUMENTA;

typedef struct _NOPENDOCUMENTW {
  wchar_t *pFile;                //Pointer to a file string buffer
  int *nCodePage;                //Pointer to a code page variable
  BOOL *bBOM;                    //Pointer to a BOM variable
  DWORD *dwFlags;                //Pointer to a open flags variable
  BOOL bProcess;                 //TRUE   open file
                                 //FALSE  do not open file
} NOPENDOCUMENTW;

typedef struct _NSAVEDOCUMENTA {
  char *pFile;                   //Pointer to a file string buffer
  int *nCodePage;                //Pointer to a code page variable
  BOOL *bBOM;                    //Pointer to a BOM variable
  BOOL bProcess;                 //TRUE   save file
                                 //FALSE  do not save file
} NSAVEDOCUMENTA;

typedef struct _NSAVEDOCUMENTW {
  wchar_t *pFile;                //Pointer to a file string buffer
  int *nCodePage;                //Pointer to a code page variable
  BOOL *bBOM;                    //Pointer to a BOM variable
  BOOL bProcess;                 //TRUE   save file
                                 //FALSE  do not save file
} NSAVEDOCUMENTW;

typedef struct _NMAINSHOW {
  DWORD *dwStyle;           //Pointer to a maximized state variable (WS_MAXIMIZE or zero)
  DWORD *dwShow;            //Pointer to a SW_ constants combination variable
  BOOL bProcess;            //TRUE   show main window
                            //FALSE  do not show main window
} NMAINSHOW;

typedef struct _NCONTEXTMENU {
  HWND hWnd;                   //Context menu window
  UINT uType;                  //Type:    NCM_EDIT, NCM_TAB or NCM_STATUS
  POINT pt;                    //Context menu coordiates
  BOOL bProcess;               //TRUE   show context menu
                               //FALSE  do not show context menu
} NCONTEXTMENU;

typedef struct _NSIZE {
  RECT rcInitial;       //initial client RECT (read-only)
  RECT rcCurrent;       //current client RECT (writeable)
} NSIZE;


//// AkelPad menu messages

#define IDM_FILE_NEW                    4101  //Create new edit window
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_CREATENEW              4102  //Create new instance of program
                                              //Return Value: new main window handle
                                              //
#define IDM_FILE_OPEN                   4103  //Open file dialog
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_REOPEN                 4104  //Reopen file
                                              //Return Value: zero
                                              //
#define IDM_FILE_SAVE                   4105  //Save file
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_SAVEAS                 4106  //Save file dialog
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_PAGESETUP              4107  //Print setup dialog
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_PRINT                  4108  //Print dialog. lParam can be used to pass edit window handle.
                                              //Return Value: number of printed pages
                                              //
#define IDM_FILE_EXIT                   4109  //Exit program
                                              //Return Value: zero
                                              //
#define IDM_FILE_SAVEALL                4110  //Save all documents
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_CLOSEALL               4111  //Close all documents
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_CLOSEALL_BUTACTIVE     4112  //Close all documents, but active
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_FILE_SILENTPRINT            4113  //Print without dialog. lParam can be used to pass edit window handle.
                                              //Return Value: number of printed pages
                                              //
#define IDM_FILE_PRINTPREVIEW           4114  //Print preview dialog. lParam can be used to pass edit window handle.
                                              //Return Value: zero
                                              //
#define IDM_EDIT_UNDO                   4151  //Undo last operation
                                              //Return Value: zero
                                              //
#define IDM_EDIT_REDO                   4152  //Redo last operation
                                              //Return Value: zero
                                              //
#define IDM_EDIT_CUT                    4153  //Cut
                                              //Return Value: zero
                                              //
#define IDM_EDIT_COPY                   4154  //Copy
                                              //Return Value: zero
                                              //
#define IDM_EDIT_PASTE                  4155  //Paste
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_EDIT_CLEAR                  4156  //Delete
                                              //Return Value: zero
                                              //
#define IDM_EDIT_SELECTALL              4157  //Select all
                                              //Return Value: zero
                                              //
#define IDM_EDIT_FIND                   4158  //Find dialog
                                              //Return Value: zero
                                              //
#define IDM_EDIT_FINDNEXTDOWN           4159  //Find last string down
                                              //Return Value: zero
                                              //
#define IDM_EDIT_FINDNEXTUP             4160  //Find last string up
                                              //Return Value: zero
                                              //
#define IDM_EDIT_REPLACE                4161  //Replace dialog
                                              //Return Value: zero
                                              //
#define IDM_EDIT_GOTOLINE               4162  //Go to line dialog
                                              //Return Value: zero
                                              //
#define IDM_EDIT_INSERTCHAR             4163  //Run charmap.exe
                                              //Return Value: zero
                                              //
#define IDM_EDIT_INSERT_TAB             4164  //Insert tabulation
                                              //Return Value: zero
                                              //
#define IDM_EDIT_INSERT_TAB_MENU        4165  //Insert tabulation only if several lines selected
                                              //Return Value: zero
                                              //
#define IDM_EDIT_DELETE_TAB             4166  //Delete tabulation
                                              //Return Value: zero
                                              //
#define IDM_EDIT_DELETE_TAB_MENU        4167  //Delete tabulation only if several lines selected
                                              //Return Value: zero
                                              //
#define IDM_EDIT_INSERT_SPACE           4168  //Insert space
                                              //Return Value: zero
                                              //
#define IDM_EDIT_INSERT_SPACE_MENU      4169  //Insert space only if several lines selected
                                              //Return Value: zero
                                              //
#define IDM_EDIT_DELETE_SPACE           4170  //Delete space
                                              //Return Value: zero
                                              //
#define IDM_EDIT_DELETE_SPACE_MENU      4171  //Delete space only if several lines selected
                                              //Return Value: zero
                                              //
#define IDM_EDIT_DELETE_FIRST_CHAR      4172  //Delete first char
                                              //Return Value: zero
                                              //
#define IDM_EDIT_DELETE_FIRST_CHAR_MENU 4173  //Delete first char only if selection not empty
                                              //Return Value: zero
                                              //
#define IDM_EDIT_DELETE_TRAILING_WHITESPACES 4174  //Delete trailing whitespaces
                                                   //Return Value: zero
                                                   //
#define IDM_EDIT_UPPERCASE              4175  //Convert text to UPPERCASE
                                              //Return Value: zero
                                              //
#define IDM_EDIT_LOWERCASE              4176  //Convert text to lowercase
                                              //Return Value: zero
                                              //
#define IDM_EDIT_SENTENCECASE           4177  //Convert text to Sentence case.
                                              //Return Value: zero
                                              //
#define IDM_EDIT_TITLECASE              4178  //Convert text to Title Case
                                              //Return Value: zero
                                              //
#define IDM_EDIT_INVERTCASE             4179  //Convert text to iNVERT cASE
                                              //Return Value: zero
                                              //
#define IDM_EDIT_LOOPCASE               4180  //Cycle case
                                              //Return Value: zero
                                              //
#define IDM_EDIT_RECODE                 4182  //Recode dialog
                                              //Return Value: zero
                                              //
#define IDM_EDIT_INSERTDATE             4183  //Insert date
                                              //Return Value: zero
                                              //
#define IDM_EDIT_NEWLINE_WIN            4184  //Windows new line format
                                              //Return Value: zero
                                              //
#define IDM_EDIT_NEWLINE_UNIX           4185  //Unix new line format
                                              //Return Value: zero
                                              //
#define IDM_EDIT_NEWLINE_MAC            4186  //Mac new line format
                                              //Return Value: zero
                                              //
#define IDM_VIEW_FONT                   4201  //Font dialog
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_VIEW_COLORS                 4202  //Color theme dialog
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_VIEW_INCREASE_FONT          4204  //Increase font 1px
                                              //Return Value: zero
                                              //
#define IDM_VIEW_DECREASE_FONT          4205  //Decrease font 1px
                                              //Return Value: zero
                                              //
#define IDM_VIEW_WORDWRAP               4209  //Word wrap (on\off)
                                              //Return Value: zero
                                              //
#define IDM_VIEW_ONTOP                  4210  //Always on top (on\off)
                                              //Return Value: zero
                                              //
#define IDM_VIEW_SHOW_STATUSBAR         4211  //Show statusbar (on\off)
                                              //Return Value: zero
                                              //
#define IDM_VIEW_SPLIT_WINDOW_ALL       4212  //Split window into four panes
                                              //Return Value: zero
                                              //
#define IDM_VIEW_SPLIT_WINDOW_WE        4213  //Split window into two vertical panes
                                              //Return Value: zero
                                              //
#define IDM_VIEW_SPLIT_WINDOW_NS        4214  //Split window into two horizontal panes
                                              //Return Value: zero
                                              //
#define IDM_VIEW_SPLIT_WINDOW_OFF       4215  //Reserved.
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_EXEC                4251  //Execute command
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_OPTIONS_SAVETIME            4252  //Save file time (on\off)
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_WATCHFILE           4253  //Watch file change (on\off)
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_KEEPSPACE           4254  //Keep space (on\off)
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_SINGLEOPEN_FILE     4255  //Single open file (on\off)
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_SINGLEOPEN_PROGRAM  4256  //Single open program (on\off)
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_SDI                 4257  //SDI mode
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_MDI                 4258  //MDI mode
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_PLUGINS             4259  //Plugins dialog
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS                     4260  //Options dialog
                                              //Return Value: zero
                                              //
#define IDM_OPTIONS_READONLY            4261  //Read only (on\off)
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TABVIEW_TOP          4301  //MDI tab window at the top of the main window
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TABVIEW_BOTTOM       4302  //MDI tab window at the bottom of the main window
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TABVIEW_NONE         4303  //Hide MDI tab
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TABTYPE_STANDARD     4304  //Standard MDI tab style
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TABTYPE_BUTTONS      4305  //Buttons MDI tab style
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TABTYPE_FLATBUTTONS  4306  //Flat buttons MDI tab style
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TILEHORIZONTAL       4307  //MDI windows - horizontal tile
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_TILEVERTICAL         4308  //MDI windows - vertical tile
                                              //Return Value: zero
                                              //
#define IDM_WINDOW_CASCADE              4309  //MDI windows - cascade
                                              //Return Value: zero
                                              //
#define IDM_ABOUT                       4351  //About dialog
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_CHANGESIZE          4401  //Change style of the main window SW_RESTORE\SW_MAXIMIZE
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_DELLINE             4402  //Delete current line
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_PASTEANSI           4403  //Paste as ANSI text
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_NONMENU_MDINEXT             4404  //Activate next MDI window
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_MDIPREV             4405  //Activate previous MDI window
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_MDICLOSE            4406  //Close current MDI window
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_NONMENU_REOPEN_MSG          4407  //Internal command
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_REDETECT            4408  //Redetect code page of the current file
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_REOPENAS_ANSI       4409  //Reopen file as ANSI
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_REOPENAS_OEM        4410  //Reopen file as OEM
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_REOPENAS_UTF16LE    4411  //Reopen file as UTF16LE
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_REOPENAS_UTF16BE    4412  //Reopen file as UTF16BE
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_REOPENAS_UTF8       4413  //Reopen file as UTF8
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_REOPENAS_KOIR       4414  //Reopen file as KOI-R
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_SAVEAS_ANSI         4415  //Save file as ANSI
                                              //Return Value: see ESD_* defines
                                              //
#define IDM_NONMENU_SAVEAS_OEM          4416  //Save file as OEM
                                              //Return Value: see ESD_* defines
                                              //
#define IDM_NONMENU_SAVEAS_UTF16LE      4417  //Save file as UTF16LE
                                              //Return Value: see ESD_* defines
                                              //
#define IDM_NONMENU_SAVEAS_UTF16BE      4418  //Save file as UTF16BE
                                              //Return Value: see ESD_* defines
                                              //
#define IDM_NONMENU_SAVEAS_UTF8         4419  //Save file as UTF8
                                              //Return Value: see ESD_* defines
                                              //
#define IDM_NONMENU_SAVEAS_KOIR         4420  //Save file as KOI-R
                                              //Return Value: see ESD_* defines
                                              //
#define IDM_NONMENU_INSERTMODE          4421  //Insert mode (on\off)
                                              //Return Value: zero
                                              //
#define IDM_NONMENU_FILECLOSE           4422  //Close file
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_NONMENU_DLGNEXT             4423  //Activate next dialog window
                                              //Return Value: activated dialog handle
                                              //
#define IDM_NONMENU_DLGPREV             4424  //Activate previous dialog window
                                              //Return Value: activated dialog handle
                                              //
#define IDM_NONMENU_AUTOINDENT          4425  //Insert new line with keeping left spaces
                                              //Return Value: TRUE - inserted with spaces, FALSE - inserted without spaces
                                              //
#define IDM_NONMENU_SAVEAS_UTF8_NOBOM   4426  //Save file as UTF8 without BOM
                                              //Return Value: see ESD_* defines
                                              //
#define IDM_NONMENU_CLONENEXT           4427  //Activate next pane (split window)
                                              //Return Value: activated pane handle
                                              //
#define IDM_NONMENU_CLONEPREV           4428  //Activate previous pane (split window)
                                              //Return Value: activated pane handle
                                              //
#define IDM_NONMENU_COLUMNPASTE         4429  //Paste to column selection
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_NONMENU_PASTEAFTER          4430  //Paste text after caret
                                              //Return Value: TRUE - success, FALSE - failed
                                              //
#define IDM_NONMENU_CANTOPEN_MSG        4431  //Internal command
                                              //Return Value: zero
                                              //
#define IDM_RECENT_FILES                5001  //Delete dead recent files
                                              //5001 + n  open recent file n
                                              //Return Value: zero
                                              //
#define IDM_LANGUAGE                    6001  //Activate internal language
                                              //6001 + n  activate language n
                                              //Return Value: zero
                                              //
#define IDM_POPUP_OPENAS                7001  //Open as last popup menu item
                                              //7001 + n  open as popup menu item
                                              //Return Value: zero
                                              //
#define IDM_POPUP_SAVEAS                8001  //Save as last popup menu item
                                              //8001 + n  save as popup menu item
                                              //Return Value: see ESD_* defines
                                              //
                                              //Example:
                                              //SendMessage(pd->hMainWnd, WM_COMMAND, IDM_FILE_NEW, 0);
#define IDM_SELECTWINDOW                10019 //Select window dialog (MDI)
                                              //Return Value: zero
                                              //

//// AkelPad main window WM_USER messages

//Notification messages
#define AKDN_MAIN_ONSTART          (WM_USER + 1)   //0x401
#define AKDN_MAIN_ONSTART_PRESHOW  (WM_USER + 2)   //0x402
#define AKDN_MAIN_ONSTART_SHOW     (WM_USER + 3)   //0x403
#define AKDN_MAIN_ONSTART_FINISH   (WM_USER + 4)   //0x404
#define AKDN_MAIN_ONSTART_IDLE     (WM_USER + 5)   //0x405
#define AKDN_MAIN_ONFINISH         (WM_USER + 6)   //0x406
#define AKDN_EDIT_ONSTART          (WM_USER + 7)   //0x407
#define AKDN_EDIT_ONFINISH         (WM_USER + 8)   //0x408
#define AKDN_FRAME_NOWINDOWS       (WM_USER + 9)   //0x409
#define AKDN_FRAME_ACTIVATE        (WM_USER + 10)  //0x40A
#define AKDN_DOCK_GETMINMAXINFO    (WM_USER + 11)  //0x40B

#define AKDN_ACTIVATE              (WM_USER + 21)  //0x415
#define AKDN_SIZE                  (WM_USER + 22)  //0x416
#define AKDN_OPENDOCUMENT_START    (WM_USER + 23)  //0x417
#define AKDN_OPENDOCUMENT_FINISH   (WM_USER + 24)  //0x418
#define AKDN_SAVEDOCUMENT_START    (WM_USER + 25)  //0x419
#define AKDN_SAVEDOCUMENT_FINISH   (WM_USER + 26)  //0x41A
#define AKDN_DLLCALL               (WM_USER + 27)  //0x41B
#define AKDN_DLLUNLOAD             (WM_USER + 28)  //0x41C
#define AKDN_HOTKEY                (WM_USER + 29)  //0x41D
#define AKDN_CONTEXTMENU           (WM_USER + 30)  //0x41E
#define AKDN_RECENTFILESDELETE     (WM_USER + 31)  //0x41F
#define AKDN_SEARCH_ENDED          (WM_USER + 32)  //0x420

//SubClass
#define AKD_GETMAINPROC            (WM_USER + 101)
#define AKD_SETMAINPROC            (WM_USER + 102)
#define AKD_GETMAINPROCRET         (WM_USER + 103)
#define AKD_SETMAINPROCRET         (WM_USER + 104)
#define AKD_GETEDITPROC            (WM_USER + 105)
#define AKD_SETEDITPROC            (WM_USER + 106)
#define AKD_GETEDITPROCRET         (WM_USER + 107)
#define AKD_SETEDITPROCRET         (WM_USER + 108)
#define AKD_GETFRAMEPROC           (WM_USER + 109)
#define AKD_SETFRAMEPROC           (WM_USER + 110)
#define AKD_GETFRAMEPROCRET        (WM_USER + 111)
#define AKD_SETFRAMEPROCRET        (WM_USER + 112)

//Plugin load
#define AKD_DLLCALL                (WM_USER + 121)
#define AKD_DLLUNLOAD              (WM_USER + 122)
#define AKD_DLLFIND                (WM_USER + 123)
#define AKD_DLLADD                 (WM_USER + 124)
#define AKD_DLLDELETE              (WM_USER + 125)
#define AKD_DLLSAVE                (WM_USER + 126)
#define AKD_CALLPROC               (WM_USER + 127)

//Plugin options
#define AKD_BEGINOPTIONS           (WM_USER + 131)
#define AKD_OPTION                 (WM_USER + 132)
#define AKD_ENDOPTIONS             (WM_USER + 133)
#define AKD_INIOPEN                (WM_USER + 134)
#define AKD_INIGETSECTION          (WM_USER + 135)
#define AKD_INICLEARSECTION        (WM_USER + 136)
#define AKD_INIDELETESECTION       (WM_USER + 137)
#define AKD_INIGETKEY              (WM_USER + 138)
#define AKD_INIDELETEKEY           (WM_USER + 139)
#define AKD_INIGETVALUE            (WM_USER + 140)
#define AKD_INISETVALUE            (WM_USER + 141)
#define AKD_INICLOSE               (WM_USER + 142)

//Text retrieval and modification
#define AKD_SAVEDOCUMENT           (WM_USER + 151)
#define AKD_GETTEXTLENGTH          (WM_USER + 152)
#define AKD_GETTEXTRANGE           (WM_USER + 153)
#define AKD_GETSELTEXTW            (WM_USER + 154)
#define AKD_FREETEXT               (WM_USER + 155)
#define AKD_REPLACESELA            (WM_USER + 156)
#define AKD_REPLACESELW            (WM_USER + 157)
#define AKD_PASTE                  (WM_USER + 158)
#define AKD_COPY                   (WM_USER + 159)
#define AKD_TEXTFIND               (WM_USER + 160)
#define AKD_TEXTREPLACE            (WM_USER + 161)
#define AKD_RECODESEL              (WM_USER + 162)
#define AKD_GETCHARCOLOR           (WM_USER + 163)

//Print
#define AKD_GETFILEPRINT           (WM_USER + 191)
#define AKD_SETFILEPRINT           (WM_USER + 192)
#define AKD_GETPRINTDLG            (WM_USER + 193)
#define AKD_GETPAGEDLG             (WM_USER + 194)

//Options
#define AKD_PROGRAMVERSION         (WM_USER + 201)
#define AKD_PROGRAMARCHITECTURE    (WM_USER + 202)
#define AKD_GETMAININFO            (WM_USER + 203)
#define AKD_GETEDITINFO            (WM_USER + 204)
#define AKD_SETMODIFY              (WM_USER + 205)
#define AKD_SETNEWLINE             (WM_USER + 206)
#define AKD_GETFONT                (WM_USER + 207)
#define AKD_SETFONT                (WM_USER + 208)
#define AKD_GETMSGCREATE           (WM_USER + 209)
#define AKD_SETMSGCREATE           (WM_USER + 210)
#define AKD_GETMSGBINARY           (WM_USER + 211)
#define AKD_SETMSGBINARY           (WM_USER + 212)
#define AKD_GETCODEPAGELIST        (WM_USER + 213)
#define AKD_RECENTFILES            (WM_USER + 214)
#define AKD_SEARCHHISTORY          (WM_USER + 215)

//Windows
#define AKD_GETMODELESS            (WM_USER + 251)
#define AKD_SETMODELESS            (WM_USER + 252)
#define AKD_RESIZE                 (WM_USER + 253)
#define AKD_DOCK                   (WM_USER + 254)
#define AKD_SETCLOSEBUTTON         (WM_USER + 255)
#define AKD_SETHOTKEYINPUT         (WM_USER + 256)

//Thread
#define AKD_GLOBALALLOC            (WM_USER + 281)
#define AKD_GLOBALLOCK             (WM_USER + 282)
#define AKD_GLOBALUNLOCK           (WM_USER + 283)
#define AKD_GLOBALFREE             (WM_USER + 284)
#define AKD_STRLENA                (WM_USER + 285)
#define AKD_STRLENW                (WM_USER + 286)
#define AKD_CREATEWINDOW           (WM_USER + 287)
#define AKD_WAITKEYBOARD           (WM_USER + 288)
#define AKD_GETQUEUE               (WM_USER + 289)
#define AKD_POSTMESSAGE            (WM_USER + 290)

//AkelPad 4.x messages
#define AKD_EXGETTEXTLENGTH        (WM_USER + 401)
#define AKD_EXGETTEXTRANGEA        (WM_USER + 402)
#define AKD_EXGETTEXTRANGEW        (WM_USER + 403)
#define AKD_GETSTATUSPOSTYPE       (WM_USER + 404)


/*
AKDN_MAIN_ONSTART
_________________

Internal message.


AKDN_MAIN_ONSTART_PRESHOW
_________________________

Notification message, sends to the main procedure before showing main window.

wParam              == not used
(NMAINSHOW *)lParam == show main window characteristics, program show code is:
                         NMAINSHOW *nms=(NMAINSHOW *)lParam;

                         ShowWindow(hMainWnd, (nms->dwStyle == WS_MAXIMIZE)?SW_SHOWMAXIMIZED:SW_SHOW);
                         if (nms->dwShow != SW_SHOWNORMAL) ShowWindow(hMainWnd, nms->dwShow);

Return Value
 zero


AKDN_MAIN_ONSTART_SHOW
______________________

Notification message, sends to the main procedure when main window is shown.

wParam == not used
lParam == not used

Return Value
 zero


AKDN_MAIN_ONSTART_FINISH
________________________

Notification message, sends to the main procedure when main window is finished initialize.

wParam == not used
lParam == not used

Return Value
 zero


AKDN_MAIN_ONSTART_IDLE
______________________

Notification message, sends to the main procedure when main window is finished initialize and there is no messages in the main thread's queue.

wParam == not used
lParam == not used

Return Value
 zero


AKDN_MAIN_ONFINISH
__________________

Notification message, sends to the main procedure before destroying main window.

wParam == not used
lParam == not used

Return Value
 zero


AKDN_EDIT_ONSTART
_________________

Notification message, sends to the main procedure after edit window created.

(HWND)wParam == edit window
lParam       == not used

Return Value
 zero


AKDN_EDIT_ONFINISH
__________________

Notification message, sends to the main procedure before destroying edit window.

(HWND)wParam == edit window
lParam       == not used

Return Value
 zero


AKDN_FRAME_NOWINDOWS
____________________

Notification message, sends to the main procedure when no frame windows in MDI client.

wParam == not used
lParam == not used

Return Value
 zero


AKDN_FRAME_ACTIVATE
___________________

Notification message, sends to the main procedure when frame window has been activated.

wParam       == not used
(HWND)lParam == handle to the MDI frame window being activated

Return Value
 zero


AKDN_DOCK_GETMINMAXINFO
_______________________

Notification message, sends to the main procedure before changing dock window size.

(DOCK *)wParam       == pointer to a DOCK structure
(MINMAXINFO *)lParam == pointer to a MINMAXINFO structure

Return Value
 zero


AKDN_ACTIVATE
_____________

Notification message, sends to the main procedure after another instance of the program activates main window.

wParam == not used
lParam == not used

Return Value
 zero


AKDN_SIZE
_________

Notification message, sends to the main procedure before the main window client RECT changed.

wParam           == not used
(NSIZE *)lParam  == pointer to a NSIZE structure

Return Value
 zero


AKDN_OPENDOCUMENT_START
_______________________

Notification message, sends to the main procedure before document opened.

(HWND)wParam            == edit window
(NOPENDOCUMENT *)lParam == pointer to a NOPENDOCUMENT structure
                           (NOPENDOCUMENTA *)lParam   if bOldWindows == TRUE
                           (NOPENDOCUMENTW *)lParam   if bOldWindows == FALSE

Return Value
 zero


AKDN_OPENDOCUMENT_FINISH
________________________

Notification message, sends to the main procedure after document opened.

(HWND)wParam == edit window
(int)lParam  == see EOD_* defines

Return Value
 zero


AKDN_SAVEDOCUMENT_START
_______________________

Notification message, sends to the main procedure before document saved.

(HWND)wParam            == edit window
(NSAVEDOCUMENT *)lParam == pointer to a NSAVEDOCUMENT structure
                           (NSAVEDOCUMENTA *)lParam   if bOldWindows == TRUE
                           (NSAVEDOCUMENTW *)lParam   if bOldWindows == FALSE

Return Value
 zero


AKDN_SAVEDOCUMENT_FINISH
________________________

Notification message, sends to the main procedure after document saved.

(HWND)wParam == edit window
(int)lParam  == see ESD_* defines

Return Value
 zero


AKDN_DLLCALL
____________

Notification message, sends to the main procedure after plugin call.

wParam               == not used
(PLUGINDATA *)lParam == pointer to a PLUGINDATA structure

Return Value
 zero


AKDN_DLLUNLOAD
______________

Notification message, sends to the main procedure after plugin unload.

wParam                  == not used
(unsigned char *)lParam == unloaded function name, format "Plugin::Function"
                           (char *)lParam     if bOldWindows == TRUE
                           (wchar_t *)lParam  if bOldWindows == FALSE

Return Value
 zero


AKDN_HOTKEY
___________

Notification message, sends to the main procedure when keyboard key is pressed.

(WORD)wParam   == hotkey returned by HKM_GETHOTKEY
(BOOL *)lParam == TRUE   stop message processing
                  FALSE  continue message processing (default)

Return Value
 zero


AKDN_CONTEXTMENU
________________

Notification message, sends to the main procedure before displaying context menu.

wParam                 == not used
(NCONTEXTMENU *)lParam == pointer to a NCONTEXTMENU structure

Return Value
 zero


AKDN_RECENTFILESDELETE
______________________

Notification message, sends to the main procedure before displaying message about deleted non-existent recent files records.

(int)wParam    == records deleted
(BOOL *)lParam == TRUE   show message (default)
                  FALSE  don't show message

Return Value
 zero


AKDN_SEARCH_ENDED
_________________

Notification message, sends to the main procedure after find/replace dialog found nothing.

(HWND)wParam == find/replace dialog
lParam       == not used

Return Value
 zero


AKD_GETMAINPROC, AKD_GETEDITPROC, AKD_GETFRAMEPROC
_______________  _______________  ________________

Get procedure from main window subclass chain.

(int)wParam            == index of procedure in chain, if positive search from top
                          if negative from beginning
(WNDPROCDATA **)lParam == procedure data

Return Value
 zero, if successfull

Example (get program main procedure data):
 WNDPROCDATA *wpd;
 SendMessage(pd->hMainWnd, AKD_GETMAINPROC, -1, (LPARAM)&wpd);


AKD_SETMAINPROC, AKD_SETEDITPROC, AKD_SETFRAMEPROC
_______________  _______________  ________________

Set procedure in main window subclass chain.

(WNDPROC)wParam        == procedure address,
                          if (wParam == NULL) then lParam must point to the procedure
                            data that will be removed from main window subclass chain.
(WNDPROCDATA **)lParam == procedure data,
                          if (lParam == NULL) then create new procedure data
                          if (*(WNDPROCDATA **)lParam == NULL) then create new
                            procedure data and set it on top of the main window
                            subclass chain.
                          if (*(WNDPROCDATA **)lParam != NULL) then set wParam
                            procedure in procedure data and update main window
                            subclass chain.

Return Value
 zero

Example:
 WNDPROCDATA *wpd=NULL;
 LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
   //...

   return wpd->NextProc(hWnd, uMsg, wParam, lParam);
 }
 wpd=NULL;
 SendMessage(pd->hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&wpd);


AKD_GETMAINPROCRET, AKD_GETEDITPROCRET, AKD_GETFRAMEPROCRET
__________________  __________________  ___________________

Get return procedure from main window subclass chain. This procedure calls after message have been processed.

(int)wParam               == index of procedure in chain, if positive search from top
                             if negative from beginning
(WNDPROCRETDATA **)lParam == procedure data

Return Value
 zero, if successfull

Example:
 WNDPROCRETDATA *wprd;
 SendMessage(pd->hMainWnd, AKD_GETMAINPROC, 2, (LPARAM)&wprd);


AKD_SETMAINPROCRET, AKD_SETEDITPROCRET, AKD_SETFRAMEPROCRET
__________________  __________________  ___________________

Set return procedure in main window subclass chain. This procedure calls after message have been processed.

(WNDPROC)wParam        == procedure address,
                          if (wParam == NULL) then lParam must point to the procedure
                            data that will be removed from main window subclass chain.
(WNDPROCDATA **)lParam == procedure data,
                          if (lParam == NULL) then create new procedure data
                          if (*(WNDPROCDATA **)lParam == NULL) then create new
                            procedure data and set it on top of the main window
                            subclass chain.
                          if (*(WNDPROCDATA **)lParam != NULL) then set wParam
                            procedure in procedure data and update main window
                            subclass chain.

Return Value
 zero

Example:
 WNDPROCRETDATA *wprd=NULL;
 void CALLBACK NewMainProcRet(CWPRETSTRUCT *cwprs)
 {
   if (cwprs->message == WM_NOTIFY)
   {
     if (cwprs->wParam == ID_EDIT)
     {
       //...
     }
   }

   if (wprd->NextProc)
     wprd->NextProc(cwprs);
 }
 wprd=NULL;
 SendMessage(pd->hMainWnd, AKD_SETMAINPROCRET, (WPARAM)NewMainProcRet, (LPARAM)&wprd);


AKD_DLLCALL
___________

Call dll.

wParam                   == not used
(PLUGINCALLSEND *)lParam == pointer to a PLUGINCALLSEND structure if SendMessage used
                            (PLUGINCALLSENDA *)lParam   if bOldWindows == TRUE
                            (PLUGINCALLSENDW *)lParam   if bOldWindows == FALSE
                            or pointer to a PLUGINCALLPOST, allocated with GlobalAlloc, if PostMessage used
                            (PLUGINCALLPOSTA *)lParam   if bOldWindows == TRUE
                            (PLUGINCALLPOSTW *)lParam   if bOldWindows == FALSE

Return Value
 see EDL_* defines

Example SendMessage (bOldWindows == TRUE):
 PLUGINCALLSENDA pcs;
 pcs.pFunction="Plugin::Function";
 pcs.bOnStart=FALSE;
 pcs.lParam=0;
 pcs.lpbAutoLoad=NULL;
 SendMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)&pcs);

Example SendMessage (bOldWindows == FALSE):
 PLUGINCALLSENDW pcs;
 pcs.pFunction=L"Plugin::Function";
 pcs.bOnStart=FALSE;
 pcs.lParam=0;
 pcs.lpbAutoLoad=NULL;
 SendMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)&pcs);

Example PostMessage (bOldWindows == TRUE):
 PLUGINCALLPOSTA *pcp;
 if (pcp=(PLUGINCALLPOSTA *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTA)))
 {
   lstrcpynA(pcp->szFunction, "Plugin::Function", MAX_PATH);
   pcp->bOnStart=FALSE;
   pcp->lParam=0;
   PostMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)pcp);
 }

Example PostMessage (bOldWindows == FALSE):
 PLUGINCALLPOSTW *pcp;
 if (pcp=(PLUGINCALLPOSTW *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW)))
 {
   lstrcpynW(pcp->szFunction, L"Plugin::Function", MAX_PATH);
   pcp->bOnStart=FALSE;
   pcp->lParam=0;
   PostMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)pcp);
 }


AKD_DLLUNLOAD
_____________

Exit from thread and unload dll.

(HMODULE)wParam == handle to the module
(HANDLE)lParam  == handle to the thread, NULL if plugin not exiting from thread

Return Value
 zero

Example:
 PostMessage(pd->hMainWnd, AKD_DLLUNLOAD, (WPARAM)pd->hInstanceDLL, (LPARAM)NULL);


AKD_DLLFIND
___________

Get dll stack function structure pointer.

(unsigned char *)wParam == function name, format "Plugin::Function"
                           (char *)wParam      if bOldWindows == TRUE
                           (wchar_t *)wParam   if bOldWindows == FALSE
(WORD)lParam            == hotkey returned by HKM_GETHOTKEY,
                           to search by hotkey set wParam to NULL

Return Value
 pointer to a PLUGINFUNCTION structure

Example find by name (bOldWindows == TRUE):
 PLUGINFUNCTIONA *pf;
 if (pf=(PLUGINFUNCTIONA *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)"SomePlugin::SomeFunction", 0))
   if (pf->bRunning) MessageBoxA(NULL, "Plugin is running", NULL, 0);

Example find by name (bOldWindows == FALSE):
 PLUGINFUNCTIONW *pf;
 if (pf=(PLUGINFUNCTIONW *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)L"SomePlugin::SomeFunction", 0))
   if (pf->bRunning) MessageBoxW(NULL, L"Plugin is running", NULL, 0);

Example find by hotkey (bOldWindows == TRUE):
 PLUGINFUNCTIONA *pf;
 if (pf=(PLUGINFUNCTIONA *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)NULL, 3112))
   if (pf->bRunning) MessageBoxA(NULL, "Plugin is running", NULL, 0);

Example find by hotkey (bOldWindows == FALSE):
 PLUGINFUNCTIONW *pf;
 if (pf=(PLUGINFUNCTIONW *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)NULL, 3112))
   if (pf->bRunning) MessageBoxW(NULL, L"Plugin is running", NULL, 0);


AKD_DLLADD
__________

Add dll stack function structure.

wParam                   == not used
(PLUGINFUNCTION *)lParam == function structure pointer
                            (PLUGINFUNCTIONA *)lParam   if bOldWindows == TRUE
                            (PLUGINFUNCTIONW *)lParam   if bOldWindows == FALSE

Return Value
 pointer to a PLUGINFUNCTION structure in stack

Example add plugin hotkey (bOldWindows == TRUE):
 BOOL CALLBACK PluginProc(void *lpParameter)
 {
   return TRUE; //TRUE - catch hotkey, FALSE - do default hotkey processing
 }
 PLUGINFUNCTIONA pf;
 pf.szFunction[0]='\0';
 pf.nFunctionLen=0;
 pf.wHotkey=589;       //Ctrl+M
 pf.bOnStart=FALSE;
 pf.bRunning=FALSE;
 pf.PluginProc=(PLUGINPROC)PluginProc;
 pf.lpParameter=NULL;
 SendMessage(pd->hMainWnd, AKD_DLLADD, 0, (LPARAM)&pf);

Example add plugin hotkey (bOldWindows == FALSE):
 BOOL CALLBACK PluginProc(void *lpParameter)
 {
   return TRUE; //TRUE - catch hotkey, FALSE - do default hotkey processing
 }
 PLUGINFUNCTIONW pf;
 pf.szFunction[0]='\0';
 pf.nFunctionLen=0;
 pf.wHotkey=589;       //Ctrl+M
 pf.bOnStart=FALSE;
 pf.bRunning=FALSE;
 pf.PluginProc=(PLUGINPROC)PluginProc;
 pf.lpParameter=NULL;
 SendMessage(pd->hMainWnd, AKD_DLLADD, 0, (LPARAM)&pf);


AKD_DLLDELETE
_____________

Delete dll stack function structure.

wParam                   == not used
(PLUGINFUNCTION *)lParam == pointer to a PLUGINFUNCTION structure
                            (PLUGINFUNCTIONA *)lParam   if bOldWindows == TRUE
                            (PLUGINFUNCTIONW *)lParam   if bOldWindows == FALSE

Return Value
 zero

Example (bOldWindows == TRUE):
 PLUGINFUNCTIONA *pf;
 if (pf=(PLUGINFUNCTIONA *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)"SomePlugin::SomeFunction", 0))
 {
   SendMessage(pd->hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pf);
   pf=NULL;
 }

Example (bOldWindows == FALSE):
 PLUGINFUNCTIONW *pf;
 if (pf=(PLUGINFUNCTIONW *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)L"SomePlugin::SomeFunction", 0))
 {
   SendMessage(pd->hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pf);
   pf=NULL;
 }


AKD_DLLSAVE
___________

Save dll stack.

wParam      == not used
(int)lParam == see DLLS_* defines

Return Value
 TRUE  success
 FALSE error

Example:
 SendMessage(pd->hMainWnd, AKD_DLLSAVE, 0, DLLS_CLEAR);


AKD_CALLPROC
____________

Call procedure.

(PLUGINPROC)wParam == procedure address
(void *)lParam     == pointer to a variable to be passed to the procedure

Return Value
 void

Example:
 void CALLBACK MyProcedure(void *lpParameter)
 {
 }
 PostMessage(pd->hMainWnd, AKD_CALLPROC, (WPARAM)MyProcedure, (LPARAM)NULL);


AKD_BEGINOPTIONS
________________

Begins read or save plugin or program options.

(DWORD)wParam           == see POB_* defines
(unsigned char *)lParam == plugin name
                           (char *)lParam      if bOldWindows == TRUE
                           (wchar_t *)lParam   if bOldWindows == FALSE
                           if NULL then begin work with program options

Return Value
 HANDLE

Example read (bOldWindows == TRUE):
 PLUGINOPTIONA po;
 HANDLE hOptions;
 char szDir[MAX_PATH];

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_READ, (LPARAM)"AutoSave"))
 {
   po.pOptionName="SaveDir";
   po.dwType=PO_STRING;
   po.lpData=(LPBYTE)szDir;
   po.dwData=MAX_PATH;
   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);

   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
 }

Example read (bOldWindows == FALSE):
 PLUGINOPTIONW po;
 HANDLE hOptions;
 wchar_t wszDir[MAX_PATH];

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_READ, (LPARAM)L"AutoSave"))
 {
   po.pOptionName=L"SaveDir";
   po.dwType=PO_STRING;
   po.lpData=(LPBYTE)wszDir;
   po.dwData=MAX_PATH * sizeof(wchar_t);
   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);

   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
 }

Example save (bOldWindows == TRUE):
 PLUGINOPTIONA po;
 HANDLE hOptions;
 char szDir[MAX_PATH]="C:\\Temp";

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_SAVE, (LPARAM)"AutoSave"))
 {
   po.pOptionName="SaveDir";
   po.dwType=PO_STRING;
   po.lpData=(LPBYTE)szDir;
   po.dwData=lstrlenA(szDir) + 1;
   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);

   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
 }

Example save (bOldWindows == FALSE):
 PLUGINOPTIONW po;
 HANDLE hOptions;
 wchar_t wszDir[MAX_PATH]=L"C:\\Temp";

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_SAVE, (LPARAM)L"AutoSave"))
 {
   po.pOptionName=L"SaveDir";
   po.dwType=PO_STRING;
   po.lpData=(LPBYTE)wszDir;
   po.dwData=lstrlenW(wszDir) * sizeof(wchar_t) + 2;
   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);

   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
 }


AKD_OPTION
__________

Read or save plugin option.

(HANDLE)wParam         == HANDLE returned by AKD_BEGINOPTIONS
(PLUGINOPTION *)lParam == pointer to a PLUGINOPTION structure
                          (PLUGINOPTIONA *)lParam   if bOldWindows == TRUE
                          (PLUGINOPTIONW *)lParam   if bOldWindows == FALSE
Return Value
 Size of the data copied to the buffer

Example:
 See AKD_BEGINOPTIONS examples


AKD_ENDOPTION
_____________

Ends plugin options.

(HANDLE)wParam == HANDLE returned by AKD_BEGINOPTIONS
lParam         == not used

Return Value
 TRUE   success
 FALSE  failed

Example:
 See AKD_BEGINOPTIONS examples


AKD_INIOPEN
___________

Opens ini file.

(DWORD)wParam           == see POB_* defines
(unsigned char *)lParam == ini file
                           (char *)lParam      if bOldWindows == TRUE
                           (wchar_t *)lParam   if bOldWindows == FALSE

Return Value
 HANDLE

Example read (bOldWindows == TRUE):
 INIVALUEA iv;
 HANDLE hIniFile;
 char szDir[MAX_PATH];

 if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_READ, (LPARAM)"C:\\File.ini"))
 {
   iv.pSection="Options";
   iv.pKey="SaveDir";
   iv.dwType=INI_STRINGANSI;
   iv.lpData=(LPBYTE)szDir;
   iv.dwData=MAX_PATH;
   SendMessage(pd->hMainWnd, AKD_INIGETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);

   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
 }

Example read (bOldWindows == FALSE):
 INIVALUEW iv;
 HANDLE hIniFile;
 wchar_t wszDir[MAX_PATH];

 if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_READ, (LPARAM)L"C:\\File.ini"))
 {
   iv.pSection=L"Options";
   iv.pKey=L"SaveDir";
   iv.dwType=INI_STRINGUNICODE;
   iv.lpData=(LPBYTE)wszDir;
   iv.dwData=MAX_PATH * sizeof(wchar_t);
   SendMessage(pd->hMainWnd, AKD_INIGETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);

   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
 }

Example save (bOldWindows == TRUE):
 INIVALUEA iv;
 HANDLE hIniFile;
 char szDir[MAX_PATH]="C:\\Temp";

 if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_SAVE, (LPARAM)"C:\\File.ini"))
 {
   iv.pSection="Options";
   iv.pKey="SaveDir";
   iv.dwType=INI_STRINGANSI;
   iv.lpData=(LPBYTE)szDir;
   iv.dwData=lstrlenA(szDir) + 1;
   SendMessage(pd->hMainWnd, AKD_INISETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);

   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
 }

Example save (bOldWindows == FALSE):
 INIVALUEW iv;
 HANDLE hIniFile;
 wchar_t wszDir[MAX_PATH]=L"C:\\Temp";

 if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_SAVE, (LPARAM)L"C:\\File.ini"))
 {
   iv.pSection=L"Options";
   iv.pKey=L"SaveDir";
   iv.dwType=INI_STRINGUNICODE;
   iv.lpData=(LPBYTE)wszDir;
   iv.dwData=lstrlenW(wszDir) * sizeof(wchar_t) + 2;
   SendMessage(pd->hMainWnd, AKD_INISETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);

   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
 }


AKD_INIGETSECTION
_________________

Retrieve ini section handle.

(HANDLE)wParam          == ini file handle
(unsigned char *)lParam == section name
                           (char *)lParam      if bOldWindows == TRUE
                           (wchar_t *)lParam   if bOldWindows == FALSE

Return Value
 HANDLE

Example (bOldWindows == TRUE):
 HANDLE hIniSection;

 if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
   SendMessage(pd->hMainWnd, AKD_INICLEARSECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);

Example (bOldWindows == FALSE):
 HANDLE hIniSection;

 if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
   SendMessage(pd->hMainWnd, AKD_INICLEARSECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);


AKD_INICLEARSECTION
___________________

Removes all keys in ini section.

(HANDLE)wParam == ini file handle
(HANDLE)lParam == ini section handle

Return Value
 zero

Example:
 See AKD_INIGETSECTION example


AKD_INIDELETESECTION
____________________

Deletes ini section.

(HANDLE)wParam == ini file handle
(HANDLE)lParam == ini section handle

Return Value
 zero

Example (bOldWindows == TRUE):
 HANDLE hIniSection;

 if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
   SendMessage(pd->hMainWnd, AKD_INIDELETESECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);

Example (bOldWindows == FALSE):
 HANDLE hIniSection;

 if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
   SendMessage(pd->hMainWnd, AKD_INIDELETESECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);


AKD_INIGETKEY
_____________

Retrieve key handle.

(HANDLE)wParam          == ini section handle
(unsigned char *)lParam == key name
                           (char *)lParam      if bOldWindows == TRUE
                           (wchar_t *)lParam   if bOldWindows == FALSE
Return Value
 HANDLE

Example (bOldWindows == TRUE):
 HANDLE hIniSection;
 HANDLE hIniKey;

 if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
   if (hIniKey=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETKEY, (WPARAM)hIniSection, (LPARAM)"SaveDir"))
     SendMessage(pd->hMainWnd, AKD_INIDELETEKEY, (WPARAM)hIniSection, (LPARAM)hIniKey);

Example (bOldWindows == FALSE):
 HANDLE hIniSection;
 HANDLE hIniKey;

 if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
   if (hIniKey=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETKEY, (WPARAM)hIniSection, (LPARAM)L"SaveDir"))
     SendMessage(pd->hMainWnd, AKD_INIDELETEKEY, (WPARAM)hIniSection, (LPARAM)hIniKey);


AKD_INIDELETEKEY
________________

Deletes ini key.

(HANDLE)wParam == ini section handle
(HANDLE)lParam == key handle

Return Value
 zero

Example:
 See AKD_INIGETKEY examples


AKD_INIGETVALUE
_______________

Retrieve ini value.

(HANDLE)wParam     == ini file handle
(INIVALUE *)lParam == pointer to a INIVALUE structure
                      (INIVALUEA *)lParam   if bOldWindows == TRUE
                      (INIVALUEW *)lParam   if bOldWindows == FALSE

Return Value
 Size of the data copied to the buffer

Example:
 See AKD_INIOPEN examples


AKD_INISETVALUE
_______________

Set ini value.

(HANDLE)wParam     == ini file handle
(INIVALUE *)lParam == pointer to a INIVALUE structure
                      (INIVALUEA *)lParam   if bOldWindows == TRUE
                      (INIVALUEW *)lParam   if bOldWindows == FALSE

Return Value
 TRUE   success
 FALSE  failed

Example:
 See AKD_INIOPEN examples


AKD_INICLOSE
____________

Close ini file handle.

(HANDLE)wParam == ini file handle
lParam         == not used

Return Value
 TRUE   success
 FALSE  failed

Example:
 See AKD_INIOPEN examples


AKD_SAVEDOCUMENT
________________

Save file.

(HWND)wParam           == edit window
(SAVEDOCUMENT *)lParam == pointer to a SAVEDOCUMENT structure
                         (SAVEDOCUMENTA *)lParam   if bOldWindows == TRUE
                         (SAVEDOCUMENTW *)lParam   if bOldWindows == FALSE

Return Value
 see ESD_* defines

Example (bOldWindows == TRUE):
 SAVEDOCUMENTA sd;

 lstrcpynA(sd.szFile, "C:\\MyFile.txt", MAX_PATH);
 sd.nCodePage=65001;
 sd.bBOM=TRUE;
 sd.dwFlags=SD_UPDATE;
 SendMessage(pd->hMainWnd, AKD_SAVEDOCUMENT, (WPARAM)pd->hWndEdit, (LPARAM)&sd);

Example (bOldWindows == FALSE):
 SAVEDOCUMENTW sd;

 lstrcpynW(sd.szFile, L"C:\\MyFile.txt", MAX_PATH);
 sd.nCodePage=65001;
 sd.bBOM=TRUE;
 sd.dwFlags=SD_UPDATE;
 SendMessage(pd->hMainWnd, AKD_SAVEDOCUMENT, (WPARAM)pd->hWndEdit, (LPARAM)&sd);


AKD_GETTEXTLENGTH
_________________

Get edit window text length.

(HWND)wParam == edit window
lParam       == not used

Return Value
 text length

Example:
 int nLength=SendMessage(pd->hMainWnd, AKD_GETTEXTLENGTH, (WPARAM)pd->hWndEdit, 0);


AKD_GETTEXTRANGE
________________

Retrieves a specified range of characters from a edit control.

(HWND)wParam           == edit window
(GETTEXTRANGE *)lParam == pointer to a GETTEXTRANGE structure

Return Value
 Text length in TCHARs without null character

Example (bOldWindows == TRUE):
 GETTEXTRANGE gtr;

 gtr.cpMin=0;
 gtr.cpMax=-1;
 if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGE, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
 {
   MessageBoxA(pd->hMainWnd, (char *)gtr.pText, "Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
 }

Example (bOldWindows == FALSE):
 GETTEXTRANGE gtr;

 gtr.cpMin=0;
 gtr.cpMax=-1;
 if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGE, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
 {
   MessageBoxW(pd->hMainWnd, (wchar_t *)gtr.pText, L"Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
 }


AKD_GETSELTEXTW
_______________

Retrieves the currently selected text in a edit control.

(HWND)wParam  == edit window
(int *)lParam == pointer to a variable that receive text length, can be NULL

Return Value
 text pointer

Example:
 wchar_t *wpText;
 int nTextLen=0;

 wpText=(wchar_t *)SendMessage(pd->hMainWnd, AKD_GETSELTEXTW, (WPARAM)pd->hWndEdit, (LPARAM)&nTextLen);
 SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpText);


AKD_FREETEXT
____________

Free text buffer allocated with AKD_GETTEXTRANGE.

wParam                  == not used
(unsigned char *)lParam == buffer pointer

Return Value
 TRUE   success
 FALSE  failed

See AKD_GETTEXTRANGE.


AKD_REPLACESELA
_______________

Replace selection of the edit control.

(HWND)wParam   == edit window
(char *)lParam == text pointer

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_REPLACESELA, (WPARAM)pd->hWndEdit, (LPARAM)"SomeString");


AKD_REPLACESELW
_______________

Replace selection of the edit control.

(HWND)wParam      == edit window
(wchar_t *)lParam == text pointer

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_REPLACESELW, (WPARAM)pd->hWndEdit, (LPARAM)L"SomeString");


AKD_PASTE
_________

Paste text from clipboard to the edit control.

(HWND)wParam == edit window
(int)lParam  == see PASTE_* defines.

Return Value
 TRUE   success
 FALSE  failed

Example:
 SendMessage(pd->hMainWnd, AKD_PASTE, (WPARAM)pd->hWndEdit, PASTE_UNICODE);


AKD_COPY
________

Copy text to clipboard from edit control.

(HWND)wParam == edit window
lParam       == not used

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_COPY, (WPARAM)pd->hWndEdit, 0);


AKD_TEXTFIND
____________

Finds text in a edit control.

(HWND)wParam       == edit window
(TEXTFIND *)lParam == pointer to a TEXTFIND structure
                      (TEXTFINDA *)lParam   if bOldWindows == TRUE
                      (TEXTFINDW *)lParam   if bOldWindows == FALSE
Return Value
 Character position of the next match. If there are no more matches, the return value is ?.

Example (bOldWindows == TRUE):
 TEXTFINDA tf;

 tf.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
 tf.pFindIt="Text to find";
 SendMessage(pd->hMainWnd, AKD_TEXTFIND, (WPARAM)pd->hWndEdit, (LPARAM)&tf);

Example (bOldWindows == FALSE):
 TEXTFINDW tf;

 tf.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
 tf.pFindIt=L"Text to find";
 SendMessage(pd->hMainWnd, AKD_TEXTFIND, (WPARAM)pd->hWndEdit, (LPARAM)&tf);


AKD_TEXTREPLACE
_______________

Replaces text in a edit control.

(HWND)wParam          == edit window
(TEXTREPLACE *)lParam == pointer to a TEXTREPLACE structure
                         (TEXTREPLACEA *)lParam   if bOldWindows == TRUE
                         (TEXTREPLACEW *)lParam   if bOldWindows == FALSE
Return Value
 Character position of the next match. If there are no more matches, the return value is ?.

Example (bOldWindows == TRUE):
 TEXTREPLACEA tr;

 tr.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
 tr.pFindIt="Text to find";
 tr.pReplaceWith="Text to replace";
 tr.bAll=TRUE;
 SendMessage(pd->hMainWnd, AKD_TEXTREPLACE, (WPARAM)pd->hWndEdit, (LPARAM)&tr);

Example (bOldWindows == FALSE):
 TEXTREPLACEW tr;

 tr.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
 tr.pFindIt=L"Text to find";
 tr.pReplaceWith=L"Text to replace";
 tr.bAll=TRUE;
 SendMessage(pd->hMainWnd, AKD_TEXTREPLACE, (WPARAM)pd->hWndEdit, (LPARAM)&tr);


AKD_RECODESEL
_____________

Recode text in a edit control.

(HWND)wParam         == edit window
(TEXTRECODE *)lParam == pointer to a TEXTRECODE structure

Return Value
 zero

Example:
 TEXTRECODE tr;

 tr.nCodePageFrom=1251;
 tr.nCodePageTo=866;
 SendMessage(pd->hMainWnd, AKD_RECODESEL, (WPARAM)pd->hWndEdit, (LPARAM)&tr);


AKD_GETCHARCOLOR
________________

Get colors of the specified char.

(HWND)wParam        == edit window
(CHARCOLOR *)lParam == pointer to a CHARCOLOR structure

Return Value
 TRUE   specified char in selection
 FALSE  specified char not in selection

Example:
 CHARCOLOR cc;

 cc.nCharPos=10;
 SendMessage(pd->hMainWnd, AKD_GETCHARCOLOR, (WPARAM)pd->hWndEdit, (LPARAM)&cc);


AKD_GETFILEPRINT
________________

Retrieve global print state.

wParam == not used
lParam == not used

Return Value
 TRUE  next opened file will be printed
 FALSE next opened file will not be printed

Example:
 SendMessage(pd->hMainWnd, AKD_GETFILEPRINT, 0, 0);


AKD_SETFILEPRINT
________________

Next opened file will be printed.

(BOOL)wParam == TRUE  will be printed
                FALSE will not be printed
lParam       == not used

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETFILEPRINT, TRUE, 0);


AKD_GETPRINTDLG
_______________

Get print dialog settings.

wParam == not used
lParam == not used

Return Value
 pointer to a PRINTDLG structure

Example (bOldWindows == TRUE):
 PRINTDLGA *pdg=(PRINTDLGA *)SendMessage(pd->hMainWnd, AKD_GETPRINTDLG, 0, 0);

Example (bOldWindows == FALSE):
 PRINTDLGW *pdg=(PRINTDLGW *)SendMessage(pd->hMainWnd, AKD_GETPRINTDLG, 0, 0);


AKD_GETPAGEDLG
______________

Get page dialog settings.

wParam == not used
lParam == not used

Return Value
 pointer to a PAGESETUPDLG structure

Example (bOldWindows == TRUE):
 PAGESETUPDLGA *psdg=(PAGESETUPDLGA *)SendMessage(pd->hMainWnd, AKD_GETPAGEDLG, 0, 0);

Example (bOldWindows == FALSE):
 PAGESETUPDLGW *psdg=(PAGESETUPDLGW *)SendMessage(pd->hMainWnd, AKD_GETPAGEDLG, 0, 0);


AKD_PROGRAMVERSION
__________________

Get program version.

wParam == not used
lParam == not used

Return Value
 Version number. Created as: MAKE_IDENTIFIER(dwMajor, dwMinor, dwRelease, dwBuild).

Example:
 DWORD dwVersion;
 DWORD dwMajor;
 DWORD dwMinor;
 DWORD dwRelease;
 DWORD dwBuild;

 dwVersion=SendMessage(pd->hMainWnd, AKD_PROGRAMVERSION, 0, 0);
 dwMajor=LOBYTE(LOWORD(dwVersion));
 dwMinor=HIBYTE(LOWORD(dwVersion));
 dwRelease=LOBYTE(HIWORD(dwVersion));
 dwBuild=HIBYTE(HIWORD(dwVersion));


AKD_PROGRAMARCHITECTURE
_______________________

Get program architecture (AkelDLL) version.

wParam == not used
lParam == not used

Return Value
 Version number. Created as: MAKE_IDENTIFIER(dwMajor, dwMinor, dwRelease, dwBuild).

Example:
 DWORD dwVersion;
 DWORD dwMajor;
 DWORD dwMinor;
 DWORD dwRelease;
 DWORD dwBuild;

 dwVersion=SendMessage(pd->hMainWnd, AKD_PROGRAMARCHITECTURE, 0, 0);
 dwMajor=LOBYTE(LOWORD(dwVersion));
 dwMinor=HIBYTE(LOWORD(dwVersion));
 dwRelease=LOBYTE(HIWORD(dwVersion));
 dwBuild=HIBYTE(HIWORD(dwVersion));


AKD_GETMAININFO
_______________

Get main AkelPad data.

wParam               == not used
(PLUGINDATA *)lParam == pointer to a PLUGINDATA structure

Return Value
 zero

Example:
 PLUGINDATA pd;

 SendMessage(pd->hMainWnd, AKD_GETMAININFO, 0, (LPARAM)&pd);


AKD_GETEDITINFO
_______________

Get edit window info.

(HWND)wParam       == edit window,
                      NULL for current edit window
(EDITINFO *)lParam == pointer to a EDITINFO structure

Return Value
 TRUE   success
 FALSE  failed

Example:
 EDITINFO ei;

 SendMessage(pd->hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei);


AKD_SETMODIFY
_____________

Set edit window modification flag.

(HWND)wParam == edit window,
                NULL for current edit window
(BOOL)lParam == TRUE  contents of edit control have been modified
                FALSE contents of edit control haven't been modified

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETMODIFY, (WPARAM)pd->hWndEdit, TRUE);


AKD_SETNEWLINE
______________

Set edit window new line format.

(HWND)wParam == edit window,
                NULL for current edit window
(int)lParam  == see NEWLINE_* defines

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETNEWLINE, (WPARAM)pd->hWndEdit, NEWLINE_UNIX);


AKD_GETFONT
___________

Get font.

(HWND)wParam == edit window,
                NULL for current edit window
lParam       == not used

Return Value
 pointer to a LOGFONT structure

Example (bOldWindows == TRUE):
 LOGFONTA *lf=(LOGFONTA *)SendMessage(pd->hMainWnd, AKD_GETFONT, (WPARAM)NULL, 0);

Example (bOldWindows == FALSE):
 LOGFONTW *lf=(LOGFONTW *)SendMessage(pd->hMainWnd, AKD_GETFONT, (WPARAM)NULL, 0);


AKD_SETFONT
___________

Set font.

(HWND)wParam      == edit window
(LOGFONT *)lParam == pointer to a LOGFONT structure

Return Value
 TRUE   success
 FALSE  failed

Example (bOldWindows == TRUE):
 LOGFONTA *lf;

 lf=(LOGFONTA *)SendMessage(pd->hMainWnd, AKD_GETFONT, 0, 0);
 lf->lfHeight-=2;
 lstrcpynA(lf->lfFaceName, "Courier New", LF_FACESIZE);
 SendMessage(pd->hMainWnd, AKD_SETFONT, (WPARAM)pd->hWndEdit, (LPARAM)lf);

Example (bOldWindows == FALSE):
 LOGFONTW *lf;

 lf=(LOGFONTW *)SendMessage(pd->hMainWnd, AKD_GETFONT, 0, 0);
 lf->lfHeight-=2;
 lstrcpynW(lf->lfFaceName, L"Courier New", LF_FACESIZE);
 SendMessage(pd->hMainWnd, AKD_SETFONT, (WPARAM)pd->hWndEdit, (LPARAM)lf);


AKD_GETMSGCREATE
________________

Retrieve unexisted file autoanswer.

wParam == not used
lParam == not used

Return Value
 AUTOANSWER_ASK   Show message (default)
 AUTOANSWER_YES   Create unexisted file
 AUTOANSWER_NO    Don't create unexisted file

Example:
 SendMessage(pd->hMainWnd, AKD_GETMSGCREATE, 0, 0);


AKD_SETMSGCREATE
________________

Create unexisted file autoanswer.

(int)wParam == AUTOANSWER_ASK   Show message (default)
               AUTOANSWER_YES   Create unexisted file
               AUTOANSWER_NO    Don't create unexisted file
lParam      == not used

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETMSGCREATE, AUTOANSWER_YES, 0);


AKD_GETMSGBINARY
________________

Retrive binary file autoanswer.

wParam == not used
lParam == not used

Return Value
 AUTOANSWER_ASK  Show message (default)
 AUTOANSWER_YES  Open binary file
 AUTOANSWER_NO   Don't open binary file

Example:
 SendMessage(pd->hMainWnd, AKD_GETMSGBINARY, 0, 0);


AKD_SETMSGBINARY
________________

Open binary file autoanswer.

(int)wParam == AUTOANSWER_ASK  Show message (default)
               AUTOANSWER_YES  Open binary file
               AUTOANSWER_NO   Don't open binary file
lParam      == not used

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETMSGBINARY, AUTOANSWER_YES, 0);


AKD_GETCODEPAGELIST
___________________

Get codepage list.

(int *)wParam == if this parameter is not NULL, then the default codepage identifier
                 copies to the variable.
lParam        == not used

Return Value
 pointer to a codepage array, in which last element is zero

Example:
 int *lpCodepageList;
 int nDefaultCodepage;

 lpCodepageList=(int *)SendMessage(pd->hMainWnd, AKD_GETCODEPAGELIST, (WPARAM)&nDefaultCodepage, 0);


AKD_RECENTFILES
_______________

Recent files operations.

(int)wParam           == see RF_* defines
(RECENTFILES *)lParam == pointer to a RECENTFILES structure, can be NULL
                         (RECENTFILESA *)lParam   if bOldWindows == TRUE
                         (RECENTFILESW *)lParam   if bOldWindows == FALSE

Return Value
 Number of recent files.

Example (bOldWindows == TRUE):
 RECENTFILESA rf;

 SendMessage(pd->hMainWnd, AKD_RECENTFILES, RF_GET, (LPARAM)&rf);

Example (bOldWindows == FALSE):
 RECENTFILESW rf;

 SendMessage(pd->hMainWnd, AKD_RECENTFILES, RF_GET, (LPARAM)&rf);


AKD_SEARCHHISTORY
_________________

Search history operations.

(int)wParam  == see SH_* defines
lParam       == not used

Return Value
 Number of search strings.

Example:
 SendMessage(pd->hMainWnd, AKD_SEARCHHISTORY, SH_GET, 0);


AKD_GETMODELESS
_______________

Get modeless dialog handle.

wParam == not used
lParam == not used

Return Value
 dialog handle

Example:
 HWND hDlg=(HWND)SendMessage(pd->hMainWnd, AKD_GETMODELESS, 0, 0);


AKD_SETMODELESS
_______________

Set modeless dialog handle.

(HWND)wParam == dialog handle
lParam       == not used

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETMODELESS, (LPARAM)hMyDialog, 0);


AKD_RESIZE
__________

Update main window size with AKDN_SIZE notification message.

wParam == not used
lParam == not used

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_RESIZE, 0, 0);


AKD_DOCK
________

Message to work with dockable window.

(DWORD)wParam  == see DK_* defines
(DOCK *)lParam == pointer to a DOCK structure

Return Value
 pointer to a DOCK structure

Example:
 DOCK dk={0};
 DOCK *dkNew;

 dk.dwFlags=DKF_FIXEDSIZE;
 dk.hWnd=hDlg;
 dk.nSide=DKS_RIGHT;
 dk.rcSize.left=0;
 dk.rcSize.top=0;
 dk.rcSize.right=50;
 dk.rcSize.bottom=50;
 dkNew=(DOCK *)SendMessage(pd->hMainWnd, AKD_DOCK, DK_ADD|DK_SUBCLASS, (LPARAM)&dk);

 SendMessage(pd->hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkNew);


AKD_SETCLOSEBUTTON
__________________

Draw bitmap on button and make it unfocusable.

(HWND)wParam    == button handle. Button must have BS_BITMAP style and if BS_NOTIFY specified, some notifications can be ignored.
(HBITMAP)lParam == bitmap handle. If NULL, then default bitmap is small cross 8x7.

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETCLOSEBUTTON, (WPARAM)hWndButton, (LPARAM)NULL);


AKD_SETHOTKEYINPUT
__________________

Associate hotkey control with plugins hotkey input.

(HWND)wParam == hotkey control handle.
(WORD)lParam == initial hotkey value. See HKM_GETHOTKEY message return value (MSDN).
                Initial hotkey value is sets automatically after HKM_SETHOTKEY message.

Return Value
 zero

Example:
 SendMessage(pd->hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);


AKD_GLOBALALLOC
_______________

Allocates the specified number of bytes from the heap (see description for GlobalAlloc in MSDN).

wParam == memory allocation attributes
lParam == number of bytes to allocate

Return Value
  handle to the newly allocated memory object

Example:
 HGLOBAL hMem=(HGLOBAL)SendMessage(pd->hMainWnd, AKD_GLOBALALLOC, GPTR, 128);


AKD_GLOBALLOCK
______________

Locks a global memory object (see description for GlobalLock in MSDN).

wParam == handle to the global memory object
lParam == not used

Return Value
  pointer to the first byte of the memory block

Example:
 void *pMem=(void *)SendMessage(pd->hMainWnd, AKD_GLOBALLOCK, (WPARAM)hMem, 0);


AKD_GLOBALUNLOCK
________________

Decrements the lock count associated with a memory object (see description for GlobalUnlock in MSDN).

wParam == handle to the global memory object
lParam == not used

Return Value
 unlock result

Example:
 BOOL bUnlock=SendMessage(pd->hMainWnd, AKD_GLOBALUNLOCK, (WPARAM)hMem, 0);


AKD_GLOBALFREE
______________

Frees the specified global memory object (see description for GlobalFree in MSDN).

wParam == handle to the global memory object
lParam == not used

Return Value
 free result

Example:
 HGLOBAL hMem=SendMessage(pd->hMainWnd, AKD_GLOBALFREE, (WPARAM)hMem, 0);


AKD_STRLENA
___________

Returns the length in bytes of the specified string (not including the terminating null character).

(char *)wParam == ANSI string
lParam         == not used

Return Value
 string length

Example:
 SendMessage(pd->hMainWnd, AKD_STRLENA, (WPARAM)"123", 0);


AKD_STRLENW
___________

Returns the length in WCHAR's of the specified string (not including the terminating null character).

(wchar_t *)wParam == Unicode string
lParam            == not used

Return Value
 string length

Example:
 SendMessage(pd->hMainWnd, AKD_STRLENW, (WPARAM)L"123", 0);


AKD_CREATEWINDOW
________________

Create window.

wParam                 == not used
(CREATEWINDOW *)lParam == pointer to a CREATEWINDOW structure
                          (CREATEWINDOWA *)lParam   if bOldWindows == TRUE
                          (CREATEWINDOWW *)lParam   if bOldWindows == FALSE

Return Value
 window handle

Example (bOldWindows == TRUE):
 CREATEWINDOWA cw;
 HWND hWndMemEdit;

 lstrcpynA(cw.szClassName, "RichEdit20A", MAX_PATH);
 cw.szWindowName[0]='\0';
 cw.dwStyle=WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_LEFT|ES_MULTILINE|ES_DISABLENOSCROLL|ES_SUNKEN|ES_NOHIDESEL;
 cw.x=0;
 cw.y=0;
 cw.nWidth=100;
 cw.nHeight=100;
 cw.hWndParent=pd->hMainWnd;
 cw.hMenu=(HMENU)0;
 cw.hInstance=pd->hInstanceEXE;
 cw.lpParam=NULL;

 hWndMemEdit=(HWND)SendMessage(pd->hMainWnd, AKD_CREATEWINDOW, 0, (LPARAM)&cw);

Example (bOldWindows == FALSE):
 CREATEWINDOWW cw;
 HWND hWndMemEdit;

 lstrcpynW(cw.szClassName, L"RichEdit20W", MAX_PATH);
 cw.szWindowName[0]='\0';
 cw.dwStyle=WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_LEFT|ES_MULTILINE|ES_DISABLENOSCROLL|ES_SUNKEN|ES_NOHIDESEL;
 cw.x=0;
 cw.y=0;
 cw.nWidth=100;
 cw.nHeight=100;
 cw.hWndParent=pd->hMainWnd;
 cw.hMenu=(HMENU)0;
 cw.hInstance=pd->hInstanceEXE;
 cw.lpParam=NULL;

 hWndMemEdit=(HWND)SendMessage(pd->hMainWnd, AKD_CREATEWINDOW, 0, (LPARAM)&cw);


AKD_WAITKEYBOARD
________________

Wait for release all virtual keys.

(BOOL)wParam == TRUE  test is any key pressed
                FALSE wait for keys release
lParam       == not used

Return Value
 if wParam == FALSE the return value:
   zero
 if wParam == TRUE the return value:
   TRUE  keyboard is free
   FALSE keyboard is busy

Example:
 SendMessage(pd->hMainWnd, AKD_WAITKEYBOARD, 0, 0);


AKD_GETQUEUE
____________

Indicates the type of messages found in the main thread's message queue
(see description for GetQueueStatus in MSDN).

wParam == specifies the types of messages for which to check
lParam == not used

Return Value
  number of messages

Example:
 DWORD dwStatus=SendMessage(pd->hMainWnd, AKD_GETQUEUE, QS_ALLEVENTS, 0);


AKD_POSTMESSAGE
_______________

Post message.

wParam                == not used
(POSTMESSAGE *)lParam == pointer to a POSTMESSAGE, allocated with GlobalAlloc

Return Value
  zero


Example (bOldWindows == TRUE):
 typedef struct _PMSAVEDOCUMENTA {
   POSTMESSAGE pm;
   SAVEDOCUMENTA sd;
 } PMSAVEDOCUMENTA;

 PMSAVEDOCUMENTA *pmsd;

 if (pmsd=(PMSAVEDOCUMENTA *)GlobalAlloc(GPTR, sizeof(PMSAVEDOCUMENTA)))
 {
   lstrcpynA(pmsd->sd.szFile, "C:\\MyFile.txt", MAX_PATH);
   pmsd->sd.nCodePage=65001;
   pmsd->sd.bBOM=TRUE;
   pmsd->sd.bUpdate=TRUE;

   //Post message
   pmsd->pm.hWnd=pd->hMainWnd;
   pmsd->pm.uMsg=AKD_SAVEDOCUMENT;
   pmsd->pm.wParam=(WPARAM)pd->hWndEdit;
   pmsd->pm.lParam=(LPARAM)&pmsd->sd;
   PostMessage(pd->hMainWnd, AKD_POSTMESSAGE, 0, (LPARAM)pmsd);
 }

Example (bOldWindows == FALSE):
 typedef struct _PMSAVEDOCUMENTW {
   POSTMESSAGE pm;
   SAVEDOCUMENTW sd;
 } PMSAVEDOCUMENTW;

 PMSAVEDOCUMENTW *pmsd;

 if (pmsd=(PMSAVEDOCUMENTW *)GlobalAlloc(GPTR, sizeof(PMSAVEDOCUMENTW)))
 {
   lstrcpynW(pmsd->sd.szFile, L"C:\\MyFile.txt", MAX_PATH);
   pmsd->sd.nCodePage=65001;
   pmsd->sd.bBOM=TRUE;
   pmsd->sd.bUpdate=TRUE;

   //Post message
   pmsd->pm.hWnd=pd->hMainWnd;
   pmsd->pm.uMsg=AKD_SAVEDOCUMENT;
   pmsd->pm.wParam=(WPARAM)pd->hWndEdit;
   pmsd->pm.lParam=(LPARAM)&pmsd->sd;
   PostMessage(pd->hMainWnd, AKD_POSTMESSAGE, 0, (LPARAM)pmsd);
 }


AKD_EXGETTEXTLENGTH
___________________

Get AkelEdit window text length.

(HWND)wParam == AkelEdit window
(int)lParam  == see AELB_* defines

Return Value
 text length

Example:
 int nLength=SendMessage(pd->hMainWnd, AKD_EXGETTEXTLENGTH, (WPARAM)pd->hWndEdit, AELB_ASOUTPUT);


AKD_EXGETTEXTRANGEA
___________________

Retrieves a specified range of characters from a AkelEdit control.

(HWND)wParam             == AkelEdit window
(EXGETTEXTRANGE *)lParam == pointer to a EXGETTEXTRANGE structure

Return Value
 Text length in TCHARs without null character

Example:
 EXGETTEXTRANGE tr;

 SendMessage(pd->hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr);
 tr.pText=NULL;
 tr.nNewLine=AELB_ASIS;
 tr.nCodePage=CP_ACP;
 tr.lpDefaultChar=NULL;
 tr.lpUsedDefChar=NULL;

 if (SendMessage(pd->hMainWnd, AKD_EXGETTEXTRANGEA, (WPARAM)pd->hWndEdit, (LPARAM)&tr))
 {
   MessageBoxA(pd->hMainWnd, (char *)tr.pText, "Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
 }


AKD_EXGETTEXTRANGEW
___________________

Retrieves a specified range of characters from a AkelEdit control.

(HWND)wParam             == AkelEdit window
(EXGETTEXTRANGE *)lParam == pointer to a EXGETTEXTRANGE structure

Return Value
 Text length in TCHARs without null character

Example:
 EXGETTEXTRANGE tr;

 SendMessage(pd->hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr);
 tr.pText=NULL;
 tr.nNewLine=AELB_ASIS;

 if (SendMessage(pd->hMainWnd, AKD_EXGETTEXTRANGEW, (WPARAM)pd->hWndEdit, (LPARAM)&tr))
 {
   MessageBoxW(pd->hMainWnd, (wchar_t *)tr.pText, L"Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
 }


AKD_GETSTATUSPOSTYPE
____________________

Get status position type parameter.

wParam == not used
lParam == not used

Return Value
 see SPT_* defines

Example:
 DWORD dwStatusPosType=SendMessage(pd->hMainWnd, AKD_GETSTATUSPOSTYPE, 0, 0);

*/


//// AkelPad WM_COPYDATA messages

#define CD_OPENDOCUMENT   1


/*
CD_OPENDOCUMENT
_______________

Open file.

(DWORD)dwData          == CD_OPENDOCUMENT
(DWORD)cbData          == sizeof(OPENDOCUMENTA)    if bOldWindows == TRUE
                          sizeof(OPENDOCUMENTW)    if bOldWindows == FALSE
(OPENDOCUMENT *)lpData == (OPENDOCUMENTA *)lpData  if bOldWindows == TRUE
                          (OPENDOCUMENTW *)lpData  if bOldWindows == FALSE

Return Value
 see EOD_* defines

Example (bOldWindows == TRUE):
  OPENDOCUMENTA od;
  COPYDATASTRUCT cds;

  od.hWnd=NULL;
  lstrcpynA(od.szFile, "C:\\MyFile.txt", MAX_PATH);
  od.szWorkDir[0]='\0';
  od.dwFlags=OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE;
  od.nCodePage=0;
  od.bBOM=0;

  cds.dwData=CD_OPENDOCUMENT;
  cds.cbData=sizeof(OPENDOCUMENTA);
  cds.lpData=&od;
  SendMessage(pd->hMainWnd, WM_COPYDATA, (WPARAM)pd->hMainWnd, (LPARAM)&cds);

Example (bOldWindows == FALSE):
  OPENDOCUMENTW od;
  COPYDATASTRUCT cds;

  od.hWnd=NULL;
  lstrcpynW(od.szFile, L"C:\\MyFile.txt", MAX_PATH);
  od.szWorkDir[0]='\0';
  od.dwFlags=OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE;
  od.nCodePage=0;
  od.bBOM=0;

  cds.dwData=CD_OPENDOCUMENT;
  cds.cbData=sizeof(OPENDOCUMENTW);
  cds.lpData=&od;
  SendMessage(pd->hMainWnd, WM_COPYDATA, (WPARAM)pd->hMainWnd, (LPARAM)&cds);
*/

//// UNICODE define

#ifndef UNICODE
  #define OPENDOCUMENT OPENDOCUMENTA
  #define SAVEDOCUMENT SAVEDOCUMENTA
  #define WNDFRAME WNDFRAMEA
  #define PLUGINFUNCTION PLUGINFUNCTIONA
  #define PLUGINCALLSEND PLUGINCALLSENDA
  #define PLUGINCALLPOST PLUGINCALLPOSTA
  #define PLUGINOPTION PLUGINOPTIONA
  #define INIVALUE INIVALUEA
  #define RECENTFILES RECENTFILESA
  #define TEXTFIND TEXTFINDA
  #define TEXTREPLACE TEXTREPLACEA
  #define CREATEWINDOW CREATEWINDOWA
  #define NOPENDOCUMENT NOPENDOCUMENTA
  #define NSAVEDOCUMENT NSAVEDOCUMENTA
#else
  #define OPENDOCUMENT OPENDOCUMENTW
  #define SAVEDOCUMENT SAVEDOCUMENTW
  #define WNDFRAME WNDFRAMEW
  #define PLUGINFUNCTION PLUGINFUNCTIONW
  #define PLUGINCALLSEND PLUGINCALLSENDW
  #define PLUGINCALLPOST PLUGINCALLPOSTW
  #define PLUGINOPTION PLUGINOPTIONW
  #define INIVALUE INIVALUEW
  #define RECENTFILES RECENTFILESW
  #define TEXTFIND TEXTFINDW
  #define TEXTREPLACE TEXTREPLACEW
  #define CREATEWINDOW CREATEWINDOWW
  #define NOPENDOCUMENT NOPENDOCUMENTW
  #define NSAVEDOCUMENT NSAVEDOCUMENTW
#endif

#endif
