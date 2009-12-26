unit AkelDLL_h;

(*************************************************************************

 Converted with C to Pascal Converter 2.0
 Release: 2.12.7.2009
 Email: al_gun@ncable.net.au
 Updates: http://cc.codegear.com/Author/302259
 Copyright (c) 2005-2009, Ural Gunaydin

**************************************************************************)

interface

uses
  Windows, Messages;

(*$HPPEMIT ''*)
(*$HPPEMIT '#include "AkelDLL.h"'*)
(*$HPPEMIT ''*)

function PRIMARYLANGID(LangId: WORD): WORD; inline;

// Version
function MakeIdentifier(a, b, c, d: Byte): DWORD; inline;

const
  AKELDLL = (1 or 2 shl 8) or (0 or 0 shl 8) shl 16; // MakeIdentifier(1, 2, 0, 0);
  {$EXTERNALSYM AKELDLL}

  // Defines

  //Control IDs
  ID_EDIT     = 10001;
  {$EXTERNALSYM ID_EDIT}
  ID_STATUS   = 10002;
  {$EXTERNALSYM ID_STATUS}
  ID_TAB      = 10003;
  {$EXTERNALSYM ID_TAB}
  ID_PROGRESS = 10004;
  {$EXTERNALSYM ID_PROGRESS}
  ID_FIRSTMDI = 10010;
  {$EXTERNALSYM ID_FIRSTMDI}

  //Sizes
  WORD_DELIMITERS_SIZE = 128;
  {$EXTERNALSYM WORD_DELIMITERS_SIZE}
  WRAP_DELIMITERS_SIZE = 128;
  {$EXTERNALSYM WRAP_DELIMITERS_SIZE}
  URL_PREFIXES_SIZE    = 128;
  {$EXTERNALSYM URL_PREFIXES_SIZE}
  URL_DELIMITERS_SIZE  = 128;
  {$EXTERNALSYM URL_DELIMITERS_SIZE}

  //Unload plugin flag
  UD_FAILED              = -1;  // Operation failed. Don't use it.
  {$EXTERNALSYM UD_FAILED}
  UD_UNLOAD              =  0;  // Unload plugin (default)
  {$EXTERNALSYM UD_UNLOAD}
  UD_NONUNLOAD_ACTIVE    = $1;  // Don't unload plugin and set active status
  {$EXTERNALSYM UD_NONUNLOAD_ACTIVE}
  UD_NONUNLOAD_NONACTIVE = $2;  // Don't unload plugin and set non-active status
  {$EXTERNALSYM UD_NONUNLOAD_NONACTIVE}
  UD_NONUNLOAD_UNCHANGE  = $4;  // Don't unload plugin and don't change active status
  {$EXTERNALSYM UD_NONUNLOAD_UNCHANGE}
  UD_HOTKEY_DODEFAULT    = $8;  // Do default hotkey processing
  {$EXTERNALSYM UD_HOTKEY_DODEFAULT}

  //Open document flags
  OD_ADT_BINARY_ERROR = $00000001;    // Check if file is binary
  {$EXTERNALSYM OD_ADT_BINARY_ERROR}
  OD_ADT_REG_CODEPAGE = $00000002;    // If last open code page found in registry,
                                      // then it will be used with OD_ADT_DETECT_BOM flag,
                                      // if not found, then next flags will be used
                                      // OD_ADT_DETECT_CODEPAGE|OD_ADT_DETECT_BOM
  {$EXTERNALSYM OD_ADT_REG_CODEPAGE}
  OD_ADT_DETECT_CODEPAGE = $00000004; // Detect code page
  {$EXTERNALSYM OD_ADT_DETECT_CODEPAGE}
  OD_ADT_DETECT_BOM      = $00000008; // Detect BOM mark
  {$EXTERNALSYM OD_ADT_DETECT_BOM}

  //Open document errors
  EOD_SUCCESS        =  0;          // Success
  {$EXTERNALSYM EOD_SUCCESS}
  EOD_ADT_OPEN       = -1;          // Autodetect codepage, can't open file
  {$EXTERNALSYM EOD_ADT_OPEN}
  EOD_ADT_ALLOC      = -2;          // Autodetect codepage, can't allocate buffer
  {$EXTERNALSYM EOD_ADT_ALLOC}
  EOD_ADT_READ       = -3;          // Autodetect codepage, read file error
  {$EXTERNALSYM EOD_ADT_READ}
  EOD_ADT_BINARY     = -4;          // Autodetect codepage, file is binary
  {$EXTERNALSYM EOD_ADT_BINARY}
  EOD_OPEN           = -5;          // Can't open file
  {$EXTERNALSYM EOD_OPEN}
  EOD_CANCEL         = -6;          // User press cancel
  {$EXTERNALSYM EOD_CANCEL}
  EOD_WINDOW_EXIST   = -7;          // cFile already opened
  {$EXTERNALSYM EOD_WINDOW_EXIST}
  EOD_CODEPAGE_ERROR = -8;          // Code page isn't implemented
  {$EXTERNALSYM EOD_CODEPAGE_ERROR}
  EOD_STOP           = -9;          // Stopped from AKDN_OPENDOCUMENT_START
  {$EXTERNALSYM EOD_STOP}
  EOD_STREAMIN       = -10;         // Error in EM_STREAMIN
  {$EXTERNALSYM EOD_STREAMIN}

  //Save document flags
  SD_UPDATE    = $00000001;         // Update file info
  {$EXTERNALSYM SD_UPDATE}
  SD_SELECTION = $00000002;         // Save only selection
  {$EXTERNALSYM SD_SELECTION}

  //Save document errors
  ESD_SUCCESS        =  0;          // Success
  {$EXTERNALSYM ESD_SUCCESS}
  ESD_OPEN           = -1;          // Can't open file
  {$EXTERNALSYM ESD_OPEN}
  ESD_WRITE          = -2;          // Can't write to file
  {$EXTERNALSYM ESD_WRITE}
  ESD_READONLY       = -3;          // cFile has read-only attribute
  {$EXTERNALSYM ESD_READONLY}
  ESD_CODEPAGE_ERROR = -4;          // Code page isn't implemented
  {$EXTERNALSYM ESD_CODEPAGE_ERROR}
  ESD_STOP           = -5;          // Stopped from AKDN_SAVEDOCUMENT_START
  {$EXTERNALSYM ESD_STOP}
  ESD_STREAMOUT      = -6;          // Error in EM_STREAMOUT
  {$EXTERNALSYM ESD_STREAMOUT}

  //Save settings
  SS_REGISTRY = 1;                  // Registry
  {$EXTERNALSYM SS_REGISTRY}
  SS_INI      = 2;                  // INI file
  {$EXTERNALSYM SS_INI}

  //AKD_RECENTFILES flags
  RF_GET   = 1;                     // Retrive recent files info
  {$EXTERNALSYM RF_GET}
  RF_CLEAR = 2;                     // Clear recent files
  {$EXTERNALSYM RF_CLEAR}

  //AKD_SEARCHHISTORY flags
  SH_GET   = 1;                     // Retrive searh strings count
  {$EXTERNALSYM SH_GET}
  SH_CLEAR = 2;                     // Clear searh history
  {$EXTERNALSYM SH_CLEAR}

  //New line format
  NEWLINE_WIN  = 1;                 // Windows/DOS new line format (\r\n)
  {$EXTERNALSYM NEWLINE_WIN}
  NEWLINE_UNIX = 2;                 // Unix new line format (\n)
  {$EXTERNALSYM NEWLINE_UNIX}
  NEWLINE_MAC  = 3;                 // MacOS new line format (\r)
  {$EXTERNALSYM NEWLINE_MAC}

  //Status bar position type
  SPT_COLUMN   = $00000001; //"Line:Column". By default: "Line:Symbol".
  {$EXTERNALSYM SPT_COLUMN}
  SPT_LINEWRAP = $00000002; //Wrap line numbers. By default: Non-wrap line numbers.
  {$EXTERNALSYM SPT_LINEWRAP}

  //INI value types
  INI_DWORD         = 1; // Wrap line numbers. By default: Non-wrap line numbers.
  {$EXTERNALSYM INI_DWORD}
  INI_BINARY        = 2; // Wrap line numbers. By default: Non-wrap line numbers.
  {$EXTERNALSYM INI_BINARY}
  INI_STRINGANSI    = 3; // Wrap line numbers. By default: Non-wrap line numbers.
  {$EXTERNALSYM INI_STRINGANSI}
  INI_STRINGUNICODE = 4; // Wrap line numbers. By default: Non-wrap line numbers.
  {$EXTERNALSYM INI_STRINGUNICODE}

  //Options flags
  POB_READ  = $1;                // Begins read options
  {$EXTERNALSYM POB_READ}
  POB_SAVE  = $2;                // Begins save options
  {$EXTERNALSYM POB_SAVE}
  POB_CLEAR = $4;                // Begins new save options (POB_SAVE|POB_CLEAR)
  {$EXTERNALSYM POB_CLEAR}

  //Option type
  PO_DWORD  = 1;                 // 32-bit number
  {$EXTERNALSYM PO_DWORD}
  PO_BINARY = 2;                 // Binary data in any form
  {$EXTERNALSYM PO_BINARY}
  PO_STRING = 3;                 // Null-terminated string
  {$EXTERNALSYM PO_STRING}

  //Save plugins stack
  DLLS_CLEAR  = 1;               // Clear all records before saving
  {$EXTERNALSYM DLLS_CLEAR}
  DLLS_ONEXIT = 2;               // Save stack on program exit
  {$EXTERNALSYM DLLS_ONEXIT}

  //Context menu owner
  NCM_EDIT   = 1;                // Edit control
  {$EXTERNALSYM NCM_EDIT}
  NCM_TAB    = 2;                // Tab control
  {$EXTERNALSYM NCM_TAB}
  NCM_STATUS = 3;                // Status bar control
  {$EXTERNALSYM NCM_STATUS}

  //Find text flags
  {$IFNDEF FR_DOWN}
  FR_DOWN      = $00000001;      // Find down
  {$EXTERNALSYM FR_DOWN}
  {$ENDIF}
  {$IFNDEF FR_MATCHCASE}
  FR_MATCHCASE = $00000004;      // Search is case-sensitive
  {$EXTERNALSYM FR_MATCHCASE}
  {$ENDIF}
  FR_UP        = $00100000;  // Find up
  {$EXTERNALSYM FR_UP}
  FR_BEGINNING = $00200000;  // Search from beginning (usage: FR_DOWN|FR_BEGINNING)
  {$EXTERNALSYM FR_BEGINNING}
  FR_SELECTION = $00400000;  // Search in selection (usage: FR_DOWN|FR_SELECTION)
  {$EXTERNALSYM FR_SELECTION}
  FR_ESCAPESEQ = $00800000;  // Search with escape sequences
  {$EXTERNALSYM FR_ESCAPESEQ}
  FR_ALLFILES  = $01000000;  // Search in all openned MDI documents
                             // (usage: FR_DOWN|FR_BEGINNING|FR_ALLFILES)
  {$EXTERNALSYM FR_ALLFILES}

  //AKD_PASTE
  PASTE_UNICODE    = 0;           // paste as Unicode text, if no Unicode text
                                  // available ANSI text will be used (default).
  {$EXTERNALSYM PASTE_UNICODE}
  PASTE_ANSI       = 1;           // paste as ANSI text.
  {$EXTERNALSYM PASTE_ANSI}
  PASTE_SINGLELINE = 2;           // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM PASTE_SINGLELINE}

  //Autoanswer
  AUTOANSWER_ASK = 0;             // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM AUTOANSWER_ASK}
  AUTOANSWER_YES = 1;             // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM AUTOANSWER_YES}
  AUTOANSWER_NO  = 2;             // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM AUTOANSWER_NO}

  //Dock side
  DKS_LEFT   = 1;                 // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM DKS_LEFT}
  DKS_RIGHT  = 2;                 // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM DKS_RIGHT}
  DKS_TOP    = 3;                 // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM DKS_TOP}
  DKS_BOTTOM = 4;                 // paste multiline text to single line edit
                                  // control. All new lines replaced with '\r'.
  {$EXTERNALSYM DKS_BOTTOM}

  //Dock flags
  DKF_OWNTHREAD    = $00000001;   // Dock window has its own thread
  {$EXTERNALSYM DKF_OWNTHREAD}
  DKF_FIXEDSIZE    = $00000002;   // Dock window isn't resizeable
  {$EXTERNALSYM DKF_FIXEDSIZE}
  DKF_DRAGDROP     = $00000004;   // Dock window has drag-and-drop support,
                                  // the DOCK.rcDragDrop member is valid
  {$EXTERNALSYM DKF_DRAGDROP}
  DKF_HIDDEN       = $00000008;   // Dock window isn't visible
  {$EXTERNALSYM DKF_HIDDEN}
  DKF_NODROPLEFT   = $00000010;   // Disable drag-and-drop to the left side
  {$EXTERNALSYM DKF_NODROPLEFT}
  DKF_NODROPRIGHT  = $00000020;   // Disable drag-and-drop to the right side
  {$EXTERNALSYM DKF_NODROPRIGHT}
  DKF_NODROPTOP    = $00000040;   // Disable drag-and-drop to the top side
  {$EXTERNALSYM DKF_NODROPTOP}
  DKF_NODROPBOTTOM = $00000080;   // Disable drag-and-drop to the bottom side
  {$EXTERNALSYM DKF_NODROPBOTTOM}

  //Dock action
  DK_ADD        = $00000001;  // Add new dockable window
  {$EXTERNALSYM DK_ADD}
  DK_DELETE     = $00000002;  // Delete dockable window
  {$EXTERNALSYM DK_DELETE}
  DK_SUBCLASS   = $00000004;  // Assign dockable window handle (DOCK.hWnd)
  {$EXTERNALSYM DK_SUBCLASS}
  DK_UNSUBCLASS = $00000008;  // Deassign dockable window handle (DOCK.hWnd)
  {$EXTERNALSYM DK_UNSUBCLASS}
  DK_SETLEFT    = $00000010;  // Set DKS_LEFT side
  {$EXTERNALSYM DK_SETLEFT}
  DK_SETRIGHT   = $00000020;  // Set DKS_RIGHT side
  {$EXTERNALSYM DK_SETRIGHT}
  DK_SETTOP     = $00000040;  // Set DKS_TOP side
  {$EXTERNALSYM DK_SETTOP}
  DK_SETBOTTOM  = $00000080;  // Set DKS_BOTTOM side
  {$EXTERNALSYM DK_SETBOTTOM}
  DK_HIDE       = $00000100;  // Hide dockable window and set DKF_HIDDEN flag
  {$EXTERNALSYM DK_HIDE}
  DK_SHOW       = $00000200;  // Show dockable window and remove DKF_HIDDEN flag
  {$EXTERNALSYM DK_SHOW}

  //WM_INITMENU lParam
  IMENU_EDIT    = $00000001;  // Show dockable window and remove DKF_HIDDEN flag
  {$EXTERNALSYM IMENU_EDIT}
  IMENU_CHECKS  = $00000004;  // Show dockable window and remove DKF_HIDDEN flag
  {$EXTERNALSYM IMENU_CHECKS}

  // Structures

{$IFNDEF _HSTACK_STRUCT_}
{$DEFINE _HSTACK_STRUCT_}
type
  PHStack = ^THStack;
  {$EXTERNALSYM _HStack}
  _HStack = record
    First: Integer;
    Last: Integer;
  end;
  THStack = _HStack;
 {$EXTERNALSYM HSTACK}
  HSTACK = _HStack;
{$ENDIF}

type
  PluginProc = procedure(lpPluginProc: BOOL); stdcall;
  WndProcRet = procedure(lpWNDProcRet: PCWPRetStruct); stdcall;
  WndProc = function(hWnd: HWND; uMsg, WParam, LParam: Longint): Longint; stdcall;

type
  PPluginVersion = ^TPluginVersion;
  _PLUGINVERSION = record
    cb: DWORD;                 // Size of the structure
    hMainWnd: HWND;            // Main window
    dwAkelDllVersion: DWORD;   // Current AkelDLL version. Set it to AKELDLL.
    dwExeMinVersion3x: DWORD;  // Required minimum AkelPad 3.x version.
                               // Set as MAKE_IDENTIFIER(x, x, x, x) or
                               // if not supported MAKE_IDENTIFIER(-1, -1, -1, -1).
    dwExeMinVersion4x: DWORD;  // Required minimum AkelPad 4.x version.
                               // Set as MAKE_IDENTIFIER(x, x, x, x) or
                               // if not supported MAKE_IDENTIFIER(-1, -1, -1, -1).
    pPluginName: PChar;        // Plugin unique name
  end;
  {$EXTERNALSYM PLUGINVERSION}
  TPluginVersion = _PLUGINVERSION;
  PLUGINVERSION = _PLUGINVERSION;
  {$EXTERNALSYM PLUGINVERSION}

  PPluginData = ^TPluginData;
  _PLUGINDATA = record
    cb: DWORD;                 //Size of the structure
    pFunction: PByte;          //Called function name, format "Plugin::Function"
                               //pFunction: Pchar      if bOldWindows == TRUE
                               //pFunction: PwideChar  if bOldWindows == FALSE
    hInstanceDLL: HINST;       //DLL instance
    lpPluginFunction: Pointer; //cPointer to a PLUGINFUNCTION structure
    lpbAutoLoad: PBOOL;        //TRUE  if function supports autoload
                               //FALSE if function doesn't support autoload
    nUnload: Integer;          //See UD_* defines
    bInMemory: BOOL;           //Plugin already loaded
    bOnStart: BOOL;            //TRUE  if plugin called on start-up
                               //FALSE if plugin called manually
    lParam: Longint;           //Input data
    pAkelDir: PByte;           //AkelPad directory
                               //pAkelDir: Pchar      if bOldWindows == TRUE
                               //pAkelDir: PwideChar  if bOldWindows == FALSE
    hInstanceEXE: HINST;       //EXE instance
    hPluginsStack: PHStack;    //cPointer to a plugins stack
    hMainWnd: HWND;            //Main window
    hWndEdit: HWND;            //Edit window
    hStatus: HWND;             //StatusBar window
    hMdiClient: HWND;          //MDI client window (if bMDI == TRUE)
    hTab: HWND;                //Tab window        (if bMDI == TRUE)
    hMainMenu: HMENU;          //Main menu
    hMenuRecentFiles: HMENU;   //Recent files menu
    hMenuLanguage: HMENU;      //Language menu
    hPopupMenu: HMENU;         //Right click menu
    hMainIcon: HICON;          //Main window icon handle
    hGlobalAccel: HACCEL;      //Global accelerator table (highest priority)
    hMainAccel: HACCEL;        //Main accelerator table (lowest priority)
    bOldWindows: BOOL;         //Non-Unicode Windows
    bOldRichEdit: BOOL;        //riched20.dll lower then 5.30 (v3.0)
    bOldComctl32: BOOL;        //comctl32.dll lower then 4.71
    bAkelEdit: BOOL;           //AkelEdit control is used
    bMDI: BOOL;                //MDI mode
    nSaveSettings: Integer;    //See SS_* defines
    pLangModule: PByte;        //Language module
                               //pLangModule: Pchar      if bOldWindows == TRUE
                               //pLangModule: PwideChar  if bOldWindows == FALSE
    wLangSystem: LANGID;       //System language ID
  end;
  {$EXTERNALSYM _PLUGINDATA}
  TPluginData = _PLUGINDATA;
  PLUGINDATA = _PLUGINDATA;
  {$EXTERNALSYM PLUGINDATA}

  POpenDocumentA = ^TOpenDocumentA;
  _OPENDOCUMENTA = record
    hWnd: HWND;                               // Window to fill in, NULL for
                                              // current edit window
    szFile: array[0..MAX_PATH- 1] of Char;    // cFile to open
    szWorkDir: array[0..MAX_PATH- 1] of Char; // Set working directory before open,
                                              // if (!*szWorkDir) then don't set
    dwFlags: DWORD;                           // Open flags. See OD_ADT_* defines
    nCodePage: Integer;                       // cFile code page, ignored if
                                              // (dwFlags & OD_ADT_DETECT_CODEPAGE)
    bBOM: BOOL;                               // cFile BOM, ignored if
                                              // (dwFlags & OD_ADT_DETECT_BOM)
  end;
  {$EXTERNALSYM _OPENDOCUMENTA}
  TOpenDocumentA = _OPENDOCUMENTA;
  OPENDOCUMENTA = _OPENDOCUMENTA;
  {$EXTERNALSYM OPENDOCUMENTA}

  POpenDocumentW = ^TOpenDocumentW;
  _OPENDOCUMENTW = record
    hWnd: HWND;                                   // Window to fill in, NULL for
                                                  // current edit window
    wszFile: array[0..MAX_PATH-1] of WideChar;    // cFile to open
    wszWorkDir: array[0..MAX_PATH-1] of WideChar; // Set working directory before
                                                  // open, if (!*wszWorkDir) then
                                                  // don't set
    dwFlags: DWORD;                               // Open flags. See OD_ADT_* defines
    nCodePage: Integer;                           // cFile code page, ignored if
                                                  // (dwFlags & OD_ADT_DETECT_CODEPAGE)
    bBOM: BOOL;                                   // cFile BOM, ignored if
                                                  // (dwFlags & OD_ADT_DETECT_BOM)
  end;
  {$EXTERNALSYM _OPENDOCUMENTW}
  TOpenDocumentW = _OPENDOCUMENTW;
  OPENDOCUMENTW = _OPENDOCUMENTW;
  {$EXTERNALSYM OPENDOCUMENTW}

  PSaveDocumentA = ^TSaveDocumentA;
  _SAVEDOCUMENTA = record
    szFile: array[0..MAX_PATH-1] of Char;      // cFile to save
    nCodePage: Integer;                        // cFile code page
    bBOM: BOOL;                                // cFile BOM
    dwFlags: DWORD;                            // See SD_* defines
  end;
  {$EXTERNALSYM _SAVEDOCUMENTA}
  TSaveDocumentA = _SAVEDOCUMENTA;
  SAVEDOCUMENTA = _SAVEDOCUMENTA;
  {$EXTERNALSYM SAVEDOCUMENTA}

  PSaveDocumentW = ^TSaveDocumentW;
  _SAVEDOCUMENTW = record
    wszFile: array[0..MAX_PATH-1] of WideChar; // cFile to save
    nCodePage: Integer;                        // cFile code page
    bBOM: BOOL;                                // cFile BOM
    dwFlags: DWORD;                            // See SD_* defines
  end;
  {$EXTERNALSYM _SAVEDOCUMENTW}
  TSaveDocumentW = _SAVEDOCUMENTW;
  SAVEDOCUMENTW = _SAVEDOCUMENTW;
  {$EXTERNALSYM SAVEDOCUMENTW}

  PEditInfo = ^TEditInfo;
  _EDITINFO = record
    hWndEdit: HWND;            // Edit window
    pFile: PByte;              // Current editing file
                               // pFile: PChar     if bOldWindows == TRUE
                               // pFile: PWideChar if bOldWindows == FALSE
    nCodePage: Integer;        // Current code page
    bBOM: BOOL;                // Current BOM
    nNewLine: Integer;         // Current new line format, see NEWLINE_* defines
    bModified: BOOL;           // cFile has been modified
    bReadOnly: BOOL;           // Read only
    bWordWrap: BOOL;           // Word wrap
    bOvertypeMode: BOOL;       // Overtype mode
  end;
  {$EXTERNALSYM _EDITINFO}
  TEditInfo =_EDITINFO;
  EDITINFO = _EDITINFO;
  {$EXTERNALSYM EDITINFO}

  PAEColors = ^TAEColors;
  _AECOLORS = record
    dwFlags: DWORD;
    crCaret: TColorRef;
    crBasicText     : TColorRef;
    crBasicBk       : TColorRef;
    crSelText       : TColorRef;
    crSelBk         : TColorRef;
    crActiveLineText: TColorRef;
    crActiveLineBk  : TColorRef;
    crUrlText       : TColorRef;
    crActiveColumn  : TColorRef;
    crColumnMarker  : TColorRef;
  end;
  {$EXTERNALSYM _AECOLORS}
  TAEColors = _AECOLORS;
  AECOLORS = _AECOLORS;
  {$EXTERNALSYM AECOLORS}

  PWndFrameA = ^TWndFrameA;
  _WNDFRAMEA = record
    hIcon: HICON;                          // Frame icon
    szFile: array[0..MAX_PATH- 1] of Char; // Frame file
    ei: TEditInfo;                         // Edit info
    lf: TLogFontA;                         // Edit font
    aec: TAEColors;                        // Edit colors
    ft: TFileTime;                         // cFile time
    rcEditWindow: TRect;                   // Edit RECT
    dwEditMargins: DWORD;                  // Edit margins
    nTabStopSize: Integer;                 // Tab stop size
    bTabStopAsSpaces: BOOL;                // Insert tab stop as spaces
    nUndoLimit: Integer;                   // Undo limit
    bDetailedUndo: BOOL;                   // Detailed undo
    dwWrapType: DWORD;                     // Wrap type AEWW_WORD or
                                           // AEWW_SYMBOL (4.x only)
    dwWrapLimit: DWORD;                    // Wrap characters limit, zero if
                                           // wrap by window edge (4.x only)
    dwMarker: DWORD;                       // Vertical marker, zero if no marker
                                           // set (4.x only)
    dwMappedPrintWidth: DWORD;             // Mapped prinet page width (4.x only)
    bCaretOutEdge: BOOL;                   // Allow caret moving out of the line
                                           // edge (4.x only)
    bCaretVertLine: BOOL;                  // Draw caret vertical line (4.x only)
    nCaretWidth: Integer;                  // Caret width (4.x only)
    dwLineGap: DWORD;                      // Line gap (4.x only)
    bShowURL: BOOL;                        // Show URL
    bUrlPrefixesEnable: BOOL;              // URL prefixes enable (4.x only)
    wszUrlPrefixes: array[0..URL_PREFIXES_SIZE-1] of WideChar; //URL prefixes (4.x only)
    bUrlDelimitersEnable: BOOL;            // URL delimiters enable (4.x only)
    wszUrlLeftDelimiters: array[0..URL_DELIMITERS_SIZE-1] of WideChar;   // URL left delimiters (4.x only)
    wszUrlRightDelimiters: array[0..URL_DELIMITERS_SIZE-1] of WideChar;   // URL right delimiters (4.x only)
    bWordDelimitersEnable: BOOL;           // Word delimiters enabled
    wszWordDelimiters: array[0..WORD_DELIMITERS_SIZE-1] of WideChar; // Word delimiters
    bWrapDelimitersEnable: BOOL;           // Wrap delimiters enabled
    wszWrapDelimiters: array[0..WRAP_DELIMITERS_SIZE-1] of WideChar; // Wrap delimiters
    bSplitWindow: BOOL;                    // Edit window is splited (4.x only)
    hWndMaster: HWND;                      // Master window (4.x only)
    hWndClone1: HWND;                      // Clone window one (4.x only)
    hWndClone2: HWND;                      // Clone window two (4.x only)
    hWndClone3: HWND;                      // Clone window three (4.x only)
    rcMasterWindow: TRect;                 // Master window RECT (4.x only)
  end;
  {$EXTERNALSYM _WNDFRAMEA}
  TWndFrameA = _WNDFRAMEA;
  WNDFRAMEA = _WNDFRAMEA;
  {$EXTERNALSYM WNDFRAMEA}

  PWndFrameW = ^TWndFrameW;
  _WNDFRAMEW = record
    hIcon: HICON;                              // Frame icon
    wszFile: array[0..MAX_PATH-1] of WideChar; // Frame file
    ei: TEditInfo;                             // Edit info
    lf: TLogFontW;                             // Edit font
    aec: TAEColors;                            // Edit colors
    ft: TFileTime;                             // cFile time
    rcEditWindow: TRect;                       // Edit RECT
    dwEditMargins: DWORD;                      // Edit margins
    nTabStopSize: Integer;                     // Tab stop size
    bTabStopAsSpaces: BOOL;                    // Insert tab stop as spaces
    nUndoLimit: Integer;                       // Undo limit
    bDetailedUndo: BOOL;                       // Detailed undo
    dwWrapType: DWORD;                         // Wrap type AEWW_WORD or
                                               // AEWW_SYMBOL (4.x only)
    dwWrapLimit: DWORD;                        // Wrap characters limit, zero if
                                               // wrap by window edge (4.x only)
    dwMarker: DWORD;                           // Vertical marker, zero if no
                                               // marker set (4.x only)
    dwMappedPrintWidth: DWORD;                 // Mapped prinet page width (4.x only)
    bCaretOutEdge: BOOL;                       // Allow caret moving out of
                                               // the line edge (4.x only)
    bCaretVertLine: BOOL;                      // Draw caret vertical line (4.x only)
    nCaretWidth: Integer;                      // Caret width (4.x only)
    dwLineGap: DWORD;                          // Line gap (4.x only)
    bShowURL: BOOL;                            // Show URL
    bUrlPrefixesEnable: BOOL;                  // URL prefixes enable (4.x only)
    wszUrlPrefixes: array[0..URL_PREFIXES_SIZE-1] of WideChar;       // URL prefixes (4.x only)
    bUrlDelimitersEnable: BOOL;                // URL delimiters enable (4.x only)
    wszUrlLeftDelimiters: array[0..URL_DELIMITERS_SIZE-1] of WideChar;   // URL left delimiters (4.x only)
    wszUrlRightDelimiters: array[0..URL_DELIMITERS_SIZE-1] of WideChar;   // URL right delimiters (4.x only)
    bWordDelimitersEnable: BOOL;               // Word delimiters enabled
    wszWordDelimiters: array[0..WORD_DELIMITERS_SIZE-1] of WideChar; // Word delimiters
    bWrapDelimitersEnable: BOOL;               // Wrap delimiters enabled
    wszWrapDelimiters: array[0..WRAP_DELIMITERS_SIZE-1] of WideChar; // Wrap delimiters
    bSplitWindow: BOOL;                        // Edit window is splited (4.x only)
    hWndMaster: HWND;                          // Master window (4.x only)
    hWndClone1: HWND;                          // Clone window one (4.x only)
    hWndClone2: HWND;                          // Clone window two (4.x only)
    hWndClone3: HWND;                          // Clone window three (4.x only)
    rcMasterWindow: TRect;                     // Master window RECT (4.x only)
  end;
  {$EXTERNALSYM _WNDFRAMEW}
  TWndFrameW = _WNDFRAMEW;
  WNDFRAMEW = _WNDFRAMEW;
  {$EXTERNALSYM WNDFRAMEW}

  PWndProcData =^TWNDProcData;
  _WNDPROCDATA = record
    Next: PWndProcData;
    Prev: PWndProcData;
    CurProc: WndProc;
    NextProc: WndProc;
    PrevProc: WndProc;
  end;
  {$EXTERNALSYM _WNDPROCDATA}
  TWndProcData = _WNDPROCDATA;
  WNDPROCDATA = _WNDPROCDATA;
  {$EXTERNALSYM WNDPROCDATA}

  PWndProcRetData = ^TWndProcRetData;
  _WNDPROCRETDATA = record
    Next: PWndProcRetData;
    Prev: PWndProcRetData;
    CurProc: WndProcRet;
    NextProc: WndProcRet;
    PrevProc: WndProcRet;
  end;
  {$EXTERNALSYM _WNDPROCRETDATA}
  TWndProcRetData = _WNDPROCRETDATA;
  WNDPROCRETDATA = _WNDPROCRETDATA;
  {$EXTERNALSYM WNDPROCRETDATA}

  PPluginFunctionA = ^TPluginFunctionA;
  _PLUGINFUNCTIONA = record
    Next: PPluginFunctionA;
    Prev: PPluginFunctionA;
    szFunction: array[0..MAX_PATH-1] of Char; // Function name, format "Plugin::Function"
    nFunctionLen: Integer;                    // Function name length
    wHotkey: WORD;                            // Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
    bOnStart: BOOL;                           // Function autoload on start-up
    bRunning: BOOL;                           // Function is running
    PluginProc: PLUGINPROC;                   // Function procedure
    lpParameter: Pointer;                     // Procedure parameter
  end;
  {$EXTERNALSYM _PLUGINFUNCTIONA}
  TPluginFunctionA = _PLUGINFUNCTIONA;
  PLUGINFUNCTIONA = _PLUGINFUNCTIONA;
  {$EXTERNALSYM PLUGINFUNCTIONA}

  PPluginFunctionW = ^TPluginFunctionW;
  _PLUGINFUNCTIONW = record
    Next: PPLUGINFUNCTIONW;
    Prev: PPLUGINFUNCTIONW;
    wszFunction: array[0..MAX_PATH-1] of WideChar; // Function name, format L"Plugin::Function"
    nFunctionLen: Integer;                    // Function name length
    wHotkey: WORD;                            // Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
    bOnStart: BOOL;                           // Function autoload on start-up
    bRunning: BOOL;                           // Function is running
    PluginProc: PLUGINPROC;                   // Function procedure
    lpParameter: Pointer;                     // Procedure parameter
  end;
  {$EXTERNALSYM _PLUGINFUNCTIONW}
  TPluginFunctionW = _PLUGINFUNCTIONW;
  PLUGINFUNCTIONW = _PLUGINFUNCTIONW;
  {$EXTERNALSYM PLUGINFUNCTIONW}

  PPLuginCallSendA = ^TPLuginCallSendA;
  _PLUGINCALLSENDA = record
    pFunction: PChar;               // Function name, format "Plugin::Function"
    bOnStart: BOOL;                 // TRUE  if plugin called on start-up
                                    // FALSE if plugin called manually
    lParam: LPARAM;                 // Input data
    lpbAutoLoad: BOOL;              // If not NULL, then check plugin autoload
  end;
  {$EXTERNALSYM _PLUGINCALLSENDA}
  TPLuginCallSendA = _PLUGINCALLSENDA;
  PLUGINCALLSENDA = _PLUGINCALLSENDA;
  {$EXTERNALSYM PLUGINCALLSENDA}

  PPluginCallSendW = ^TPluginCallSendW;
  _PLUGINCALLSENDW = record
    wpFunction: PWideChar;          // Function name, format L"Plugin::Function"
    bOnStart: BOOL;                 // TRUE  if plugin called on start-up
                                    // FALSE if plugin called manually
    lParam: LPARAM;                 // Input data
    lpbAutoLoad: BOOL;              // If not NULL, then check plugin autoload
  end;
  {$EXTERNALSYM _PLUGINCALLSENDW}
  TPluginCallSendW = _PLUGINCALLSENDW;
  PLUGINCALLSENDW = _PLUGINCALLSENDW;
  {$EXTERNALSYM PLUGINCALLSENDW}

  PPluginCallPostA = ^TPluginCallPostA;
  _PLUGINCALLPOSTA = record
    szFunction: array[0..MAX_PATH-1] of Char; // Function name, format "Plugin::Function"
    bOnStart: BOOL;                           // TRUE  if plugin called on start-up
                                              // FALSE if plugin called manually
    lParam: LPARAM;                           // Input data
  end;
  {$EXTERNALSYM _PLUGINCALLPOSTA}
  TPluginCallPostA = _PLUGINCALLPOSTA;
  PLUGINCALLPOSTA = _PLUGINCALLPOSTA;
  {$EXTERNALSYM PLUGINCALLPOSTA}

  PPluginCallPostW = ^TPluginCallPostW;
  _PLUGINCALLPOSTW = record
    wszFunction: array[0..MAX_PATH-1] of WideChar; // Function name, format L"Plugin::Function"
    bOnStart: BOOL;                                // TRUE  if plugin called on start-up
                                                   // FALSE if plugin called manually
    lParam: LPARAM;                                // Input data
  end;
  {$EXTERNALSYM _PLUGINCALLPOSTW}
  TPluginCallPostW = _PLUGINCALLPOSTW;
  PLUGINCALLPOSTW = _PLUGINCALLPOSTW;
  {$EXTERNALSYM PLUGINCALLPOSTW}

  PPluginOptionA = ^TPluginOptionA;
  _PLUGINOPTIONA = record
    szOptionName: PChar;         // Option name.
    dwType: DWORD;               // Data type: PO_DWORD, PO_BINARY or PO_STRING.
    lpData: PBYTE;               // Data pointer. If NULL, AKD_OPTION returns
                                 // required buffer size in bytes.
    dwData: DWORD;               // Data size in bytes.
  end;
  {$EXTERNALSYM _PLUGINOPTIONA}
  TPluginOptionA = _PLUGINOPTIONA;
  PLUGINOPTIONA = _PLUGINOPTIONA;
  {$EXTERNALSYM PLUGINOPTIONA}

  PPluginOptionW = ^TPluginOptionW;
  _PLUGINOPTIONW = record
    wszOptionName: PWideChar;    // Option name.
    dwType: DWORD;               // Data type: PO_DWORD, PO_BINARY or PO_STRING.
    lpData: PBYTE;               // Data pointer. If NULL, AKD_OPTION returns
                                 // required buffer size in bytes.
    dwData: DWORD;               // Data size in bytes.
  end;
  {$EXTERNALSYM _PLUGINOPTIONW}
  TPluginOptionW = _PLUGINOPTIONW;
  PLUGINOPTIONW = _PLUGINOPTIONW;
  {$EXTERNALSYM PLUGINOPTIONW}

  PIniValueA = ^TIniValueA;
  _INIVALUEA = record
    szSection: PChar;            // Section name.
    szKey: PChar;                // Key name.
    dwType: DWORD;               // Data type: see INI_* defines.
    lpData: PBYTE;               // Data pointer. If NULL, AKD_INIGETVALUE
                                 // returns required buffer size in bytes.
    dwData: DWORD;               // Data size in bytes.
  end;
  {$EXTERNALSYM _INIVALUEA}
  TIniValueA = _INIVALUEA;
  INIVALUEA = _INIVALUEA;
  {$EXTERNALSYM INIVALUEA}

  PIniValueW = ^TIniValueW;
  _INIVALUEW = record
    wszSection: PWideChar;       // Section name.
    szKey: PChar;                // Key name.
    dwType: DWORD;               // Data type: see INI_* defines.
    lpData: PBYTE;               // Data pointer. If NULL, AKD_INIGETVALUE
                                 // returns required buffer size in bytes.
    dwData: DWORD;               // Data size in bytes.
  end;
  {$EXTERNALSYM _INIVALUEW}
  TIniValueW = _INIVALUEW;
  INIVALUEW = _INIVALUEW;
  {$EXTERNALSYM INIVALUEW}

  PGetTextRange = ^TGetTextRange;
  _GETTEXTRANGE = record
    cpMin: Integer;      // First character in the range. First char of text: 0.
    cpMax: Integer;      // Last character in the range. Last char of text: -1.
    pText: PByte;        // cPointer that receive allocated text. Must be
                         // eallocated with AKD_FREETEXT message.
                         // pText: Pchar      if bOldWindows == TRUE
                         // pText: PwideChar  if bOldWindows == FALSE
  end;
  {$EXTERNALSYM _GETTEXTRANGE}
  TGetTextRange = _GETTEXTRANGE;
  GETTEXTRANGE = _GETTEXTRANGE;
  {$EXTERNALSYM GETTEXTRANGE}

{$IFDEF __AKELEDIT_H__}
  PExGetTextRange = ^TExGetTextRange;
  _EXGETTEXTRANGE = record
    cr: AECHARRANGE;      // Characters range to retrieve
    bColumnSel: BOOL;     // Column selection. If this value is –1, active column
                          // selection mode is used.
    pText: PByte;         // cPointer that receive allocated text. Must be
                          // deallocated with AKD_FREETEXT message.
                          // pText: PChar       if bOldWindows == TRUE
                          // pText: PwideChar   if bOldWindows == FALSE
    nNewLine: Integer;    // see AELB_* defines
    nCodePage: Integer;   // Valid if bOldWindows == TRUE. Code page identifier
                          // (any available in the system). You can also specify
                          // one of the following values: CP_ACP - ANSI code page,
                          // CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
    lpDefaultChar: PChar; // Valid if bOldWindows == TRUE. Points to the character
                          // used if a wide character cannot be represented in the
                          // specified code page. If this member is NULL, a system
                          // default value is used.
    lpUsedDefChar: PBOOL; // Valid if bOldWindows == TRUE. Points to a flag that
                          // indicates whether a default character was used.
                          // The flag is set to TRUE if one or more wide characters
                          // in the source string cannot be represented in the
                          // specified code page. Otherwise, the flag is set to FALSE.
                          // This member may be NULL.
  end;
  {$EXTERNALSYM _EXGETTEXTRANGE}
  TExGetTextRange = _EXGETTEXTRANGE;
  EXGETTEXTRANGE = _EXGETTEXTRANGE;
  {$EXTERNALSYM EXGETTEXTRANGE}
{$ENDIF}

  PRecentFilesA = ^TRecentFilesA;
  _RECENTFILESA = record
    lpszRecentNames: array[0..MAX_PATH-1] of PChar;      // Recent files names
    lpdwRecentPositions: PDWORD;                         // Recent files positions
    lpdwRecentCodepages: PDWORD;                         // Recent files codepages
  end;
  {$EXTERNALSYM _RECENTFILESA}
  TRecentFilesA = _RECENTFILESA;
  RECENTFILESA = _RECENTFILESA;
  {$EXTERNALSYM RECENTFILESA}

  PRecentFilesW = ^TRecentFilesW;
  _RECENTFILESW = record
    wlpszRecentNames: array[0..MAX_PATH-1] of PWideChar; // Recent files names
    lpdwRecentPositions: PDWORD;                         // Recent files positions
    lpdwRecentCodepages: PDWORD;                         // Recent files codepages
  end;
  {$EXTERNALSYM _RECENTFILESW}
  TRecentFilesW = _RECENTFILESW;
  RECENTFILESW = _RECENTFILESW;
  {$EXTERNALSYM RECENTFILESW}

  PTextFindA = ^TTextFindA;
  _TEXTFINDA = record
    dwFlags: DWORD;            // See FR_* defines
    pFindIt: PChar;            // Find string
  end;
  {$EXTERNALSYM _TEXTFINDA}
  TTextFindA = _TEXTFINDA;
  TEXTFINDA = _TEXTFINDA;
  {$EXTERNALSYM TEXTFINDA}

  PTextFindW = ^TTextFindW;
  _TEXTFINDW = record
    dwFlags: DWORD;            // See FR_* defines
    wpFindIt: PWideChar;       // Find string
  end;
  {$EXTERNALSYM _TEXTFINDW}
  TTextFindW = _TEXTFINDW;
  TEXTFINDW = _TEXTFINDW;
  {$EXTERNALSYM TEXTFINDW}

  PTextReplaceA = ^TTextReplaceA;
  _TEXTREPLACEA = record
    dwFlags: DWORD;            // See FR_* defines
    pFindIt: PChar;            // Find string
    pReplaceWith: PChar;       // Replace string
    bAll: BOOL;                // Replace all
    nChanges: Integer;         // Count of changes
  end;
  {$EXTERNALSYM _TEXTREPLACEA}
  TTextReplaceA = _TEXTREPLACEA;
  TEXTREPLACEA = _TEXTREPLACEA;
  {$EXTERNALSYM TEXTREPLACEA}

  PTextReplaceW = ^TTextReplaceW;
  _TEXTREPLACEW = record
    dwFlags: DWORD;            // See FR_* defines
    wpFindIt: PWideChar;       // Find string
    wpReplaceWith: PWideChar;  // Replace string
    bAll: BOOL;                // Replace all
    nChanges: Integer;         // Count of changes
  end;
  {$EXTERNALSYM _TEXTREPLACEW}
  TTextReplaceW = _TEXTREPLACEW;
  TEXTREPLACEW = _TEXTREPLACEW;
  {$EXTERNALSYM TEXTREPLACEW}

  PTextRecode = ^TTextRecode;
  _TEXTRECODE = record
    nCodePageFrom: Integer;    // Source code page
    nCodePageTo: Integer;      // Target code page
  end;
  {$EXTERNALSYM _TEXTRECODE}
  TTextRecode = _TEXTRECODE;
  TEXTRECODE = _TEXTRECODE;
  {$EXTERNALSYM TEXTRECODE}

  PCreateWindowA = ^TCreateWindowA;
  _CREATEWINDOWA = record
    szClassName: array[0..MAX_PATH-1] of Char;       // Window class name
    szWindowName: array[0..MAX_PATH-1] of Char;      // Window caption
    dwStyle: DWORD;                                  // Window style
    x: Integer;                                      // x position
    y: Integer;                                      // y position
    nWidth: Integer;                                 // x size
    nHeight: Integer;                                // y size
    hWndParentL: HWND;                               // Parent window handle
    hMenu: HMENU;                                    // Window menu handle
    hInstance: HINST;                                // cProgram instance handle
    lpParam: Pointer;                                // Creation parameters
  end;
  {$EXTERNALSYM _CREATEWINDOWA}
  TCreateWindowA = _CREATEWINDOWA;
  CREATEWINDOWA = _CREATEWINDOWA;
  {$EXTERNALSYM CREATEWINDOWA}

  PCreateWindowW = ^TCreateWindowW;
  _CREATEWINDOWW = record
    wszClassName: array[0..MAX_PATH-1] of WideChar;  //Window class name
    wszWindowName: array[0..MAX_PATH-1] of WideChar; //Window caption
    dwStyle: DWORD;                                  // Window style
    x: Integer;                                      // x position
    y: Integer;                                      // y position
    nWidth: Integer;                                 // x size
    nHeight: Integer;                                // y size
    hWndParentL: HWND;                               // Parent window handle
    hMenu: HMENU;                                    // Window menu handle
    hInstance: HINST;                                // cProgram instance handle
    lpParam: Pointer;                                // Creation parameters
  end;
  {$EXTERNALSYM _CREATEWINDOWW}
  TCreateWindowW = _CREATEWINDOWW;
  CREATEWINDOWW = _CREATEWINDOWW;
  {$EXTERNALSYM CREATEWINDOWW}

  PDock = ^TDock;
  _DOCK = record
    Next: PDock;
    Prev: PDock;
    dwFlags: DWORD;     // Dock flags: see DKF_* defines
    hWnd: HWND;         // Dock window handle
    nSide: Integer;     // Dock side: DKS_LEFT, DKS_TOP, DKS_RIGHT or DKS_BOTTOM
    rcSize: TRect;      // Dock window client RECT
    rcDragDrop: TRect;  // Drag-and-drop client RECT
  end;
  {$EXTERNALSYM _DOCK}
  TDock = _DOCK;
  DOCK = _DOCK;
  {$EXTERNALSYM DOCK}

  PPostMessage = ^TPostMessage;
  _POSTMESSAGE = record
    hWnd: HWND;             // Window handle
    uMsg: UINT;             // Specifies the message to be sent
    wParam: WPARAM;         // Specifies additional message-specific information
    lParam: LPARAM;         // Specifies additional message-specific information
  end;
  {$EXTERNALSYM _POSTMESSAGE}
  TPostMessage = _POSTMESSAGE;
  POSTMESSAGE = _POSTMESSAGE;
  {$EXTERNALSYM POSTMESSAGE}

  PCharColor = ^TCharColor;
  _CHARCOLOR = record
    nCharPos: Integer;          // Char position
    crText: COLORREF;           // Text color
    crBk: COLORREF;             // Background color
  end;
  {$EXTERNALSYM _CHARCOLOR}
  TCharColor = _CHARCOLOR;
  CHARCOLOR = _CHARCOLOR;
  {$EXTERNALSYM CHARCOLOR}

  PNOpenDocumentA = ^TNOpenDocumentA;
  _NOPENDOCUMENTA = record
    szFile: PChar;              // cPointer to a file string buffer
    nCodePage: PInteger;        // cPointer to a code page variable
    bBOM: PBOOL;                // cPointer to a BOM variable
    dwFlags: PDWORD;            // cPointer to a open flags variable
    bProcess: BOOL;             // TRUE   open file
                                // FALSE  do not open file
  end;
  {$EXTERNALSYM _NOPENDOCUMENTA}
  TNOpenDocumentA = _NOPENDOCUMENTA;
  NOPENDOCUMENTA = _NOPENDOCUMENTA;
  {$EXTERNALSYM NOPENDOCUMENTA}

  PNOpenDocumentW = ^TNOpenDocumentW;
  _NOPENDOCUMENTW = record
    wszFile: PWideChar;         // cPointer to a file string buffer
    nCodePage: PInteger;        // cPointer to a code page variable
    bBOM: PBOOL;                // cPointer to a BOM variable
    dwFlags: PDWORD;            // cPointer to a open flags variable
    bProcess: BOOL;             // TRUE   open file
                                // FALSE  do not open file
  end;
  {$EXTERNALSYM _NOPENDOCUMENTW}
  TNOpenDocumentW = _NOPENDOCUMENTW;
  NOPENDOCUMENTW = _NOPENDOCUMENTW;
  {$EXTERNALSYM NOPENDOCUMENTW}

  PNSaveDocumentA = ^TNSaveDocumentA;
  _NSAVEDOCUMENTA = record
    szFile: PChar;              // cPointer to a file string buffer
    nCodePage: PInteger;        // cPointer to a code page variable
    bBOM: PBOOL;                // cPointer to a BOM variable
    bProcess: BOOL;             // TRUE   save file
                                // FALSE  do not save file
  end;
  {$EXTERNALSYM _NSAVEDOCUMENTA}
  TNSaveDocumentA = _NSAVEDOCUMENTA;
  NSAVEDOCUMENTA = _NSAVEDOCUMENTA;
  {$EXTERNALSYM NSAVEDOCUMENTA}

  PNSaveDocumentW = ^TNSaveDocumentW;
  _NSAVEDOCUMENTW = record
    wszFile: PWideChar;         // cPointer to a file string buffer
    nCodePage: PInteger;        // cPointer to a code page variable
    bBOM: PBOOL;                // cPointer to a BOM variable
    bProcess: BOOL;             // TRUE   save file
                                // FALSE  do not save file
  end;
  {$EXTERNALSYM _NSAVEDOCUMENTW}
  TNSaveDocumentW = _NSAVEDOCUMENTW;
  NSAVEDOCUMENTW = _NSAVEDOCUMENTW;
  {$EXTERNALSYM NSAVEDOCUMENTW}

  PNMainShow = ^TNMainShow;
  _NMAINSHOW = record
    dwStyle: PDWORD; // cPointer to a maximized state variable (WS_MAXIMIZE or zero)
    dwShow: PDWORD;  // cPointer to a SW_ constants combination variable
    bProcess: BOOL;  // TRUE   show main window
                     // FALSE  do not show main window
  end;
  {$EXTERNALSYM _NMAINSHOW}
  TNMainShow = _NMAINSHOW;
  NMAINSHOW = _NMAINSHOW;
  {$EXTERNALSYM NMAINSHOW}

  PNContexTmenu = ^TNContexTmenu;
  _NCONTEXTMENU = record
    hWnd: HWND;                   // Context menu window
    uType: UINT;                  // cType:    NCM_EDIT, NCM_TAB or NCM_STATUS
    pt: TPoint;                   // Context menu coordiates
    bProcess: BOOL;               // TRUE   show context menu
                                  // FALSE  do not show context menu
  end;
  {$EXTERNALSYM _NCONTEXTMENU}
  TNContexTmenu = _NCONTEXTMENU;
  NCONTEXTMENU = _NCONTEXTMENU;
  {$EXTERNALSYM NCONTEXTMENU}

  PNSize = ^TNSize;
  _NSIZE = record
    rcInitial: TRect;             // initial client RECT (read-only)
    rcCurrent: TRect;             // current client RECT (writeable)
  end;
  {$EXTERNALSYM _NSIZE}
  TNSize = _NSIZE;
  NSIZE = _NSIZE;
  {$EXTERNALSYM NSIZE}

// ---------------------- AkelPad menu messages --------------------------------

const
  IDM_FILE_NEW  =                     4101; // Create new edit window. Return
                                            // Value: TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_FILE_NEW}

  IDM_FILE_CREATENEW  =               4102; // Create new instance of program
                                            // Return Value: new main window handle
  {$EXTERNALSYM IDM_FILE_CREATENEW}

  IDM_FILE_OPEN  =                    4103; // Open file dialog. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_FILE_OPEN}

  IDM_FILE_REOPEN  =                  4104; // Reopen file. Return Value: zero
  {$EXTERNALSYM IDM_FILE_REOPEN}

  IDM_FILE_SAVE  =                    4105; // Save file. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_FILE_SAVE}

  IDM_FILE_SAVEAS  =                  4106; // Save file dialog. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_FILE_SAVEAS}

  IDM_FILE_PAGESETUP  =               4107; // Print setup dialog. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_FILE_PAGESETUP}

  IDM_FILE_PRINT  =                   4108; // Print dialog. lParam can be used
                                            // to pass edit window handle.
                                            // Return Value: number of printed pages
  {$EXTERNALSYM IDM_FILE_PRINT}

  IDM_FILE_EXIT  =                    4109; // Exit program. Return Value: zero
  {$EXTERNALSYM IDM_FILE_EXIT}

  IDM_FILE_SAVEALL  =                 4110; // Save all documents. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_FILE_SAVEALL}

  IDM_FILE_CLOSEALL  =                4111; // Close all documents. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_FILE_CLOSEALL}

  IDM_FILE_CLOSEALL_BUTACTIVE  =      4112; // Close all documents, but active
                                            // Return Value: TRUE - success,
                                            // FALSE - failed
  {$EXTERNALSYM IDM_FILE_CLOSEALL_BUTACTIVE}

  IDM_FILE_SILENTPRINT  =             4113; // Print without dialog. lParam can
                                            // be used to pass edit window handle.
                                            // Return Value: number of printed pages
  {$EXTERNALSYM IDM_FILE_SILENTPRINT}

  IDM_FILE_PRINTPREVIEW  =            4114; // Print preview dialog. lParam can
                                            // be used to pass edit window handle.
                                            // Return Value: zero
  {$EXTERNALSYM IDM_FILE_PRINTPREVIEW}

  IDM_EDIT_UNDO  =                    4151; // Undo last operation
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_UNDO}

  IDM_EDIT_REDO  =                    4152; // Redo last operation
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_REDO}

  IDM_EDIT_CUT  =                     4153; // Cut. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_CUT}

  IDM_EDIT_COPY  =                    4154; // Copy. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_COPY}

  IDM_EDIT_PASTE  =                   4155; // Paste. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_EDIT_PASTE}

  IDM_EDIT_CLEAR  =                   4156; // Delete. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_CLEAR}

  IDM_EDIT_SELECTALL  =               4157; // Select all. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_SELECTALL}

  IDM_EDIT_FIND  =                    4158; // Find dialog. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_FIND}

  IDM_EDIT_FINDNEXTDOWN  =            4159; // Find last string down
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_FINDNEXTDOWN}

  IDM_EDIT_FINDNEXTUP  =              4160; // Find last string up
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_FINDNEXTUP}

  IDM_EDIT_REPLACE  =                 4161; // Replace dialog. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_REPLACE}

  IDM_EDIT_GOTOLINE  =                4162; // Go to line dialog. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_GOTOLINE}

  IDM_EDIT_INSERTCHAR  =              4163; // Run charmap.exe. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_INSERTCHAR}

  IDM_EDIT_INSERT_TAB  =              4164; // Insert tabulation
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_INSERT_TAB}

  IDM_EDIT_INSERT_TAB_MENU  =         4165; // Insert tabulation only if
                                            // several lines selected
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_INSERT_TAB_MENU}

  IDM_EDIT_DELETE_TAB  =              4166; // Delete tabulation. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_DELETE_TAB}

  IDM_EDIT_DELETE_TAB_MENU  =         4167; // Delete tabulation only if several
                                            // lines selected. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_DELETE_TAB_MENU}

  IDM_EDIT_INSERT_SPACE  =            4168; // Insert space. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_INSERT_SPACE}

  IDM_EDIT_INSERT_SPACE_MENU  =       4169; // Insert space only if several
                                            // lines selected. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_INSERT_SPACE_MENU}

  IDM_EDIT_DELETE_SPACE  =            4170; // Delete space. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_DELETE_SPACE}

  IDM_EDIT_DELETE_SPACE_MENU  =       4171; // Delete space only if several
                                            // lines selected. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_DELETE_SPACE_MENU}

  IDM_EDIT_DELETE_FIRST_CHAR  =       4172; // Delete first char. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_DELETE_FIRST_CHAR}

  IDM_EDIT_DELETE_FIRST_CHAR_MENU =   4173; // Delete first char only if selection
                                            // not empty. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_DELETE_FIRST_CHAR_MENU}

  IDM_EDIT_DELETE_TRAILING_WHITESPACES = 4174; // Delete trailing whitespaces
                                               // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_DELETE_TRAILING_WHITESPACES}

  IDM_EDIT_UPPERCASE  =               4175; // Convert text to UPPERCASE
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_UPPERCASE}

  IDM_EDIT_LOWERCASE  =               4176; // Convert text to lowercase
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_LOWERCASE}

  IDM_EDIT_SENTENCECASE  =            4177; // Convert text to Sentence case.
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_SENTENCECASE}

  IDM_EDIT_TITLECASE  =               4178; // Convert text to Title
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_TITLECASE}

  IDM_EDIT_INVERTCASE  =              4179; // Convert text to iNVERT
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_INVERTCASE}

  IDM_EDIT_LOOPCASE  =                4180; // Cycle case. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_LOOPCASE}

  IDM_EDIT_RECODE  =                  4182; // Recode dialog. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_RECODE}

  IDM_EDIT_INSERTDATE  =              4183; // Insert date. Return Value: zero
  {$EXTERNALSYM IDM_EDIT_INSERTDATE}

  IDM_EDIT_NEWLINE_WIN  =             4184; // Windows new line format
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_NEWLINE_WIN}

  IDM_EDIT_NEWLINE_UNIX  =            4185; // Unix new line format
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_NEWLINE_UNIX}

  IDM_EDIT_NEWLINE_MAC  =             4186; // Mac new line format
                                            // Return Value: zero
  {$EXTERNALSYM IDM_EDIT_NEWLINE_MAC}

  IDM_VIEW_FONT  =                    4201; // Font dialog. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_VIEW_FONT}

  IDM_VIEW_COLORS  =                  4202; // Color theme dialog. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_VIEW_COLORS}

  IDM_VIEW_INCREASE_FONT  =           4204; // Increase font 1px. Return Value: zero
  {$EXTERNALSYM IDM_VIEW_INCREASE_FONT}

  IDM_VIEW_DECREASE_FONT  =           4205; // Decrease font 1px. Return Value: zero
  {$EXTERNALSYM IDM_VIEW_DECREASE_FONT}

  IDM_VIEW_WORDWRAP  =                4209; // Word wrap (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_VIEW_WORDWRAP}

  IDM_VIEW_ONTOP  =                   4210; // Always on top (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_VIEW_ONTOP}

  IDM_VIEW_SHOW_STATUSBAR  =          4211; // Show statusbar (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_VIEW_SHOW_STATUSBAR}

  IDM_VIEW_SPLIT_WINDOW_ALL  =        4212; // Split window into four panes
                                            // Return Value: zero
  {$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_ALL}

  IDM_VIEW_SPLIT_WINDOW_WE  =         4213; // Split window into two vertical panes
                                            // Return Value: zero
  {$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_WE}

  IDM_VIEW_SPLIT_WINDOW_NS  =         4214; // Split window into two horizontal panes
                                            // Return Value: zero
  {$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_NS}

  IDM_VIEW_SPLIT_WINDOW_OFF  =        4215; // Reserved.
                                            // Return Value: zero
  {$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_OFF}

  IDM_OPTIONS_EXEC  =                 4251; // Execute command. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_OPTIONS_EXEC}

  IDM_OPTIONS_SAVETIME  =             4252; // Save file time (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_SAVETIME}

  IDM_OPTIONS_WATCHFILE  =            4253; // Watch file change (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_WATCHFILE}

  IDM_OPTIONS_KEEPSPACE  =            4254; // Keep space (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_KEEPSPACE}

  IDM_OPTIONS_SINGLEOPEN_FILE  =      4255; // Single open file (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_SINGLEOPEN_FILE}

  IDM_OPTIONS_SINGLEOPEN_PROGRAM  =   4256; // Single open program (on\off)
                                            //Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_SINGLEOPEN_PROGRAM}

  IDM_OPTIONS_SDI  =                  4257; // SDI mode. Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_SDI}

  IDM_OPTIONS_MDI  =                  4258; // MDI mode. Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_MDI}

  IDM_OPTIONS_PLUGINS  =              4259; // Plugins dialog. Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_PLUGINS}

  IDM_OPTIONS  =                      4260; // Options dialog. Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS}

  IDM_OPTIONS_READONLY  =             4261; // Read only (on\off). Return Value: zero
  {$EXTERNALSYM IDM_OPTIONS_READONLY}

  IDM_WINDOW_TABVIEW_TOP  =           4301; // MDI tab window at the top of the
                                            // main window. Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TABVIEW_TOP}

  IDM_WINDOW_TABVIEW_BOTTOM  =        4302; // MDI tab window at the bottom of
                                            // the main window. Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TABVIEW_BOTTOM}

  IDM_WINDOW_TABVIEW_NONE  =          4303; // Hide MDI tab. Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TABVIEW_NONE}

  IDM_WINDOW_TABTYPE_STANDARD  =      4304; // Standard MDI tab style
                                            // Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TABTYPE_STANDARD}

  IDM_WINDOW_TABTYPE_BUTTONS  =       4305; // Buttons MDI tab style
                                            // Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TABTYPE_BUTTONS}

  IDM_WINDOW_TABTYPE_FLATBUTTONS  =   4306; // Flat buttons MDI tab style
                                            // Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TABTYPE_FLATBUTTONS}

  IDM_WINDOW_TILEHORIZONTAL  =        4307; // MDI windows - horizontal tile
                                            // Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TILEHORIZONTAL}

  IDM_WINDOW_TILEVERTICAL  =          4308; // MDI windows - vertical tile
                                            // Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_TILEVERTICAL}

  IDM_WINDOW_CASCADE  =               4309; // MDI windows - cascade
                                            // Return Value: zero
  {$EXTERNALSYM IDM_WINDOW_CASCADE}

  IDM_ABOUT  =                        4351; // About dialog. Return Value: zero
  {$EXTERNALSYM IDM_ABOUT}

  IDM_NONMENU_CHANGESIZE  =           4401; // Change style of the main window
                                            // SW_RESTORE\SW_MAXIMIZE
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_CHANGESIZE}

  IDM_NONMENU_DELLINE  =              4402; // Delete current line
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_DELLINE}

  IDM_NONMENU_PASTEANSI  =            4403; // Paste as ANSI text. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_NONMENU_PASTEANSI}

  IDM_NONMENU_MDINEXT  =              4404; // Activate next MDI window
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_MDINEXT}

  IDM_NONMENU_MDIPREV  =              4405; // Activate previous MDI window
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_MDIPREV}

  IDM_NONMENU_MDICLOSE  =             4406; // Close current MDI window
                                            // Return Value: TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_NONMENU_MDICLOSE}

  IDM_NONMENU_REOPEN_MSG  =           4407; // Internal command
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REOPEN_MSG}

  IDM_NONMENU_REDETECT  =             4408; // Redetect code page of the current
                                            // file. Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REDETECT}

  IDM_NONMENU_REOPENAS_ANSI  =        4409; // Reopen file as ANSI
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REOPENAS_ANSI}

  IDM_NONMENU_REOPENAS_OEM  =         4410; // Reopen file as OEM
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REOPENAS_OEM}

  IDM_NONMENU_REOPENAS_UTF16LE  =     4411; // Reopen file as UTF16LE
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REOPENAS_UTF16LE}

  IDM_NONMENU_REOPENAS_UTF16BE  =     4412; // Reopen file as UTF16BE
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REOPENAS_UTF16BE}

  IDM_NONMENU_REOPENAS_UTF8  =        4413; // Reopen file as UTF8
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REOPENAS_UTF8}

  IDM_NONMENU_REOPENAS_KOIR  =        4414; // Reopen file as KOI-R
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_REOPENAS_KOIR}

  IDM_NONMENU_SAVEAS_ANSI  =          4415; // Save file as ANSI
                                            // Return Value: see ESD_* defines
  {$EXTERNALSYM IDM_NONMENU_SAVEAS_ANSI}

  IDM_NONMENU_SAVEAS_OEM  =           4416; // Save file as OEM
                                            // Return Value: see ESD_* defines
  {$EXTERNALSYM IDM_NONMENU_SAVEAS_OEM}

  IDM_NONMENU_SAVEAS_UTF16LE  =       4417; // Save file as UTF16LE
                                            // Return Value: see ESD_* defines
  {$EXTERNALSYM IDM_NONMENU_SAVEAS_UTF16LE}

  IDM_NONMENU_SAVEAS_UTF16BE  =       4418; // Save file as UTF16BE
                                            // Return Value: see ESD_* defines
  {$EXTERNALSYM IDM_NONMENU_SAVEAS_UTF16BE}

  IDM_NONMENU_SAVEAS_UTF8  =          4419; // Save file as UTF8
                                            // Return Value: see ESD_* defines
  {$EXTERNALSYM IDM_NONMENU_SAVEAS_UTF8}

  IDM_NONMENU_SAVEAS_KOIR  =          4420; // Save file as KOI-R
                                            // Return Value: see ESD_* defines
  {$EXTERNALSYM IDM_NONMENU_SAVEAS_KOIR}

  IDM_NONMENU_INSERTMODE  =           4421; // Insert mode (on\off)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_INSERTMODE}

  IDM_NONMENU_FILECLOSE  =            4422; // Close file. Return Value:
                                            // TRUE - success, FALSE - failed
  {$EXTERNALSYM IDM_NONMENU_FILECLOSE}

  IDM_NONMENU_DLGNEXT  =              4423; // Activate next dialog window
                                            // Return Value: activated dialog handle
  {$EXTERNALSYM IDM_NONMENU_DLGNEXT}

  IDM_NONMENU_DLGPREV  =              4424; // Activate previous dialog window
                                            // Return Value: activated dialog handle
  {$EXTERNALSYM IDM_NONMENU_DLGPREV}

  IDM_NONMENU_AUTOINDENT  =           4425; // Insert new line with keeping
                                            // left spaces. Return Value:
                                            // TRUE - inserted with spaces,
                                            // FALSE - inserted without spaces
  {$EXTERNALSYM IDM_NONMENU_AUTOINDENT}

  IDM_NONMENU_SAVEAS_UTF8_NOBOM  =    4426; // Save file as UTF8 without BOM
                                            // Return Value: see ESD_* defines
  {$EXTERNALSYM IDM_NONMENU_SAVEAS_UTF8_NOBOM}

  IDM_NONMENU_CLONENEXT  =            4427; // Activate next pane (split window)
                                            // Return Value: activated pane handle
  {$EXTERNALSYM IDM_NONMENU_CLONENEXT}

  IDM_NONMENU_CLONEPREV  =            4428; // Activate previous pane (split window)
                                            // Return Value: activated pane handle
  {$EXTERNALSYM IDM_NONMENU_CLONEPREV}

  IDM_NONMENU_COLUMNPASTE  =          4429; // Paste to column selection
                                            // Return Value: TRUE - success,
                                            // FALSE - failed
  {$EXTERNALSYM IDM_NONMENU_COLUMNPASTE}

  IDM_NONMENU_PASTEAFTER  =           4430; // Paste text after caret
                                            // Return Value: TRUE - success,
                                            // FALSE - failed
  {$EXTERNALSYM IDM_NONMENU_PASTEAFTER}

  IDM_NONMENU_CANTOPEN_MSG  =         4431; // Internal command
                                            // Return Value: zero
  {$EXTERNALSYM IDM_NONMENU_CANTOPEN_MSG}

  IDM_RECENT_FILES  =                 5001; // Delete dead recent files
                                            // 5001 + n  open recent file n
                                            // Return Value: zero
  {$EXTERNALSYM IDM_RECENT_FILES}

  IDM_LANGUAGE  =                     6001; // Activate internal language
                                            // 6001 + n  activate language n
                                            // Return Value: zero
  {$EXTERNALSYM IDM_LANGUAGE}

  IDM_POPUP_OPENAS  =                 7001; // Open as last popup menu item
                                            // 7001 + n  open as popup menu item
                                             // Return Value: zero
  {$EXTERNALSYM IDM_POPUP_OPENAS}

  IDM_POPUP_SAVEAS  =                 8001; // Save as last popup menu item
                                            // 8001 + n  save as popup menu item
                                            // Return Value: see ESD_* defines
                                            // Example:
                                            // SendMessage(pd->hMainWnd,
                                            //     WM_COMMAND, IDM_FILE_NEW, 0);
  {$EXTERNALSYM IDM_POPUP_SAVEAS}

  IDM_SELECTWINDOW  =                10019; // Select window dialog (MDI)
                                            // Return Value: zero
  {$EXTERNALSYM IDM_SELECTWINDOW}


//// AkelPad main window WM_USER messages

//Notification messages
  AKDN_MAIN_ONSTART  =           (WM_USER + 1);   //0x401
  {$EXTERNALSYM AKDN_MAIN_ONSTART}
  AKDN_MAIN_ONSTART_PRESHOW  =   (WM_USER + 2);   //0x402
  {$EXTERNALSYM AKDN_MAIN_ONSTART_PRESHOW}
  AKDN_MAIN_ONSTART_SHOW  =      (WM_USER + 3);   //0x403
  {$EXTERNALSYM AKDN_MAIN_ONSTART_SHOW}
  AKDN_MAIN_ONSTART_FINISH  =    (WM_USER + 4);   //0x404
  {$EXTERNALSYM AKDN_MAIN_ONSTART_FINISH}
  AKDN_MAIN_ONSTART_IDLE  =      (WM_USER + 5);   //0x405
  {$EXTERNALSYM AKDN_MAIN_ONSTART_IDLE}
  AKDN_MAIN_ONFINISH  =          (WM_USER + 6);   //0x406
  {$EXTERNALSYM AKDN_MAIN_ONFINISH}
  AKDN_EDIT_ONSTART  =           (WM_USER + 7);   //0x407
  {$EXTERNALSYM AKDN_EDIT_ONSTART}
  AKDN_EDIT_ONFINISH  =          (WM_USER + 8);   //0x408
  {$EXTERNALSYM AKDN_EDIT_ONFINISH}
  AKDN_FRAME_NOWINDOWS  =        (WM_USER + 9);   //0x409
  {$EXTERNALSYM AKDN_FRAME_NOWINDOWS}
  AKDN_FRAME_ACTIVATE  =         (WM_USER + 10);  //0x40A
  {$EXTERNALSYM AKDN_FRAME_ACTIVATE}
  AKDN_DOCK_GETMINMAXINFO  =     (WM_USER + 11);  //0x40B
  {$EXTERNALSYM AKDN_DOCK_GETMINMAXINFO}

  AKDN_ACTIVATE  =               (WM_USER + 21);  //0x415
  {$EXTERNALSYM AKDN_ACTIVATE}
  AKDN_SIZE  =                   (WM_USER + 22);  //0x416
  {$EXTERNALSYM AKDN_SIZE}
  AKDN_OPENDOCUMENT_START  =     (WM_USER + 23);  //0x417
  {$EXTERNALSYM AKDN_OPENDOCUMENT_START}
  AKDN_OPENDOCUMENT_FINISH  =    (WM_USER + 24);  //0x418
  {$EXTERNALSYM AKDN_OPENDOCUMENT_FINISH}
  AKDN_SAVEDOCUMENT_START  =     (WM_USER + 25);  //0x419
  {$EXTERNALSYM AKDN_SAVEDOCUMENT_START}
  AKDN_SAVEDOCUMENT_FINISH  =    (WM_USER + 26);  //0x41A
  {$EXTERNALSYM AKDN_SAVEDOCUMENT_FINISH}
  AKDN_DLLCALL  =                (WM_USER + 27);  //0x41B
  {$EXTERNALSYM AKDN_DLLCALL}
  AKDN_DLLUNLOAD  =              (WM_USER + 28);  //0x41C
  {$EXTERNALSYM AKDN_DLLUNLOAD}
  AKDN_HOTKEY  =                 (WM_USER + 29);  //0x41D
  {$EXTERNALSYM AKDN_HOTKEY}
  AKDN_CONTEXTMENU  =            (WM_USER + 30);  //0x41E
  {$EXTERNALSYM AKDN_CONTEXTMENU}
  AKDN_RECENTFILESDELETE  =      (WM_USER + 31);  //0x41F
  {$EXTERNALSYM AKDN_RECENTFILESDELETE}
  AKDN_SEARCH_ENDED  =           (WM_USER + 32);  //0x420
  {$EXTERNALSYM AKDN_SEARCH_ENDED}

//SubClass
  AKD_GETMAINPROC  =             (WM_USER + 101);  //0x420
  {$EXTERNALSYM AKD_GETMAINPROC}
  AKD_SETMAINPROC  =             (WM_USER + 102);  //0x420
  {$EXTERNALSYM AKD_SETMAINPROC}
  AKD_GETMAINPROCRET  =          (WM_USER + 103);  //0x420
  {$EXTERNALSYM AKD_GETMAINPROCRET}
  AKD_SETMAINPROCRET  =          (WM_USER + 104);  //0x420
  {$EXTERNALSYM AKD_SETMAINPROCRET}
  AKD_GETEDITPROC  =             (WM_USER + 105);  //0x420
  {$EXTERNALSYM AKD_GETEDITPROC}
  AKD_SETEDITPROC  =             (WM_USER + 106);  //0x420
  {$EXTERNALSYM AKD_SETEDITPROC}
  AKD_GETEDITPROCRET  =          (WM_USER + 107);  //0x420
  {$EXTERNALSYM AKD_GETEDITPROCRET}
  AKD_SETEDITPROCRET  =          (WM_USER + 108);  //0x420
  {$EXTERNALSYM AKD_SETEDITPROCRET}
  AKD_GETFRAMEPROC  =            (WM_USER + 109);  //0x420
  {$EXTERNALSYM AKD_GETFRAMEPROC}
  AKD_SETFRAMEPROC  =            (WM_USER + 110);  //0x420
  {$EXTERNALSYM AKD_SETFRAMEPROC}
  AKD_GETFRAMEPROCRET  =         (WM_USER + 111);  //0x420
  {$EXTERNALSYM AKD_GETFRAMEPROCRET}
  AKD_SETFRAMEPROCRET  =         (WM_USER + 112);  //0x420
  {$EXTERNALSYM AKD_SETFRAMEPROCRET}

//Plugin load
  AKD_DLLCALL  =                 (WM_USER + 121);  //0x420
  {$EXTERNALSYM AKD_DLLCALL}
  AKD_DLLUNLOAD  =               (WM_USER + 122);  //0x420
  {$EXTERNALSYM AKD_DLLUNLOAD}
  AKD_DLLFIND  =                 (WM_USER + 123);  //0x420
  {$EXTERNALSYM AKD_DLLFIND}
  AKD_DLLADD  =                  (WM_USER + 124);  //0x420
  {$EXTERNALSYM AKD_DLLADD}
  AKD_DLLDELETE  =               (WM_USER + 125);  //0x420
  {$EXTERNALSYM AKD_DLLDELETE}
  AKD_DLLSAVE  =                 (WM_USER + 126);  //0x420
  {$EXTERNALSYM AKD_DLLSAVE}
  AKD_CALLPROC  =                (WM_USER + 127);  //0x420
  {$EXTERNALSYM AKD_CALLPROC}

//Plugin options
  AKD_BEGINOPTIONS  =            (WM_USER + 131);  //0x420
  {$EXTERNALSYM AKD_BEGINOPTIONS}
  AKD_OPTION  =                  (WM_USER + 132);  //0x420
  {$EXTERNALSYM AKD_OPTION}
  AKD_ENDOPTIONS  =              (WM_USER + 133);  //0x420
  {$EXTERNALSYM AKD_ENDOPTIONS}
  AKD_INIOPEN  =                 (WM_USER + 134);  //0x420
  {$EXTERNALSYM AKD_INIOPEN}
  AKD_INIGETSECTION  =           (WM_USER + 135);  //0x420
  {$EXTERNALSYM AKD_INIGETSECTION}
  AKD_INICLEARSECTION  =         (WM_USER + 136);  //0x420
  {$EXTERNALSYM AKD_INICLEARSECTION}
  AKD_INIDELETESECTION  =        (WM_USER + 137);  //0x420
  {$EXTERNALSYM AKD_INIDELETESECTION}
  AKD_INIGETKEY  =               (WM_USER + 138);  //0x420
  {$EXTERNALSYM AKD_INIGETKEY}
  AKD_INIDELETEKEY  =            (WM_USER + 139);  //0x420
  {$EXTERNALSYM AKD_INIDELETEKEY}
  AKD_INIGETVALUE  =             (WM_USER + 140);  //0x420
  {$EXTERNALSYM AKD_INIGETVALUE}
  AKD_INISETVALUE  =             (WM_USER + 141);  //0x420
  {$EXTERNALSYM AKD_INISETVALUE}
  AKD_INICLOSE  =                (WM_USER + 142);  //0x420
  {$EXTERNALSYM AKD_INICLOSE}

//Text retrieval and modification
  AKD_SAVEDOCUMENT  =            (WM_USER + 151);  //0x420
  {$EXTERNALSYM AKD_SAVEDOCUMENT}
  AKD_GETTEXTLENGTH  =           (WM_USER + 152);  //0x420
  {$EXTERNALSYM AKD_GETTEXTLENGTH}
  AKD_GETTEXTRANGE  =            (WM_USER + 153);  //0x420
  {$EXTERNALSYM AKD_GETTEXTRANGE}
  AKD_GETSELTEXTW  =             (WM_USER + 154);  //0x420
  {$EXTERNALSYM AKD_GETSELTEXTW}
  AKD_FREETEXT  =                (WM_USER + 155);  //0x420
  {$EXTERNALSYM AKD_FREETEXT}
  AKD_REPLACESELA  =             (WM_USER + 156);  //0x420
  {$EXTERNALSYM AKD_REPLACESELA}
  AKD_REPLACESELW  =             (WM_USER + 157);  //0x420
  {$EXTERNALSYM AKD_REPLACESELW}
  AKD_PASTE  =                   (WM_USER + 158);  //0x420
  {$EXTERNALSYM AKD_PASTE}
  AKD_COPY  =                    (WM_USER + 159);  //0x420
  {$EXTERNALSYM AKD_COPY}
  AKD_TEXTFIND  =                (WM_USER + 160);  //0x420
  {$EXTERNALSYM AKD_TEXTFIND}
  AKD_TEXTREPLACE  =             (WM_USER + 161);  //0x420
  {$EXTERNALSYM AKD_TEXTREPLACE}
  AKD_RECODESEL  =               (WM_USER + 162);  //0x420
  {$EXTERNALSYM AKD_RECODESEL}
  AKD_GETCHARCOLOR  =            (WM_USER + 163);  //0x420
  {$EXTERNALSYM AKD_GETCHARCOLOR}

//Print
  AKD_GETFILEPRINT  =            (WM_USER + 191);  //0x420
  {$EXTERNALSYM AKD_GETFILEPRINT}
  AKD_SETFILEPRINT  =            (WM_USER + 192);  //0x420
  {$EXTERNALSYM AKD_SETFILEPRINT}
  AKD_GETPRINTDLG  =             (WM_USER + 193);  //0x420
  {$EXTERNALSYM AKD_GETPRINTDLG}
  AKD_GETPAGEDLG  =              (WM_USER + 194);  //0x420
  {$EXTERNALSYM AKD_GETPAGEDLG}

//Options
  AKD_PROGRAMVERSION  =          (WM_USER + 201);  //0x420
  {$EXTERNALSYM AKD_PROGRAMVERSION}
  AKD_PROGRAMARCHITECTURE  =     (WM_USER + 202);  //0x420
  {$EXTERNALSYM AKD_PROGRAMARCHITECTURE}
  AKD_GETMAININFO  =             (WM_USER + 203);  //0x420
  {$EXTERNALSYM AKD_GETMAININFO}
  AKD_GETEDITINFO  =             (WM_USER + 204);  //0x420
  {$EXTERNALSYM AKD_GETEDITINFO}
  AKD_SETMODIFY  =               (WM_USER + 205);  //0x420
  {$EXTERNALSYM AKD_SETMODIFY}
  AKD_SETNEWLINE  =              (WM_USER + 206);  //0x420
  {$EXTERNALSYM AKD_SETNEWLINE}
  AKD_GETFONT  =                 (WM_USER + 207);  //0x420
  {$EXTERNALSYM AKD_GETFONT}
  AKD_SETFONT  =                 (WM_USER + 208);  //0x420
  {$EXTERNALSYM AKD_SETFONT}
  AKD_GETMSGCREATE  =            (WM_USER + 209);  //0x420
  {$EXTERNALSYM AKD_GETMSGCREATE}
  AKD_SETMSGCREATE  =            (WM_USER + 210);  //0x420
  {$EXTERNALSYM AKD_SETMSGCREATE}
  AKD_GETMSGBINARY  =            (WM_USER + 211);  //0x420
  {$EXTERNALSYM AKD_GETMSGBINARY}
  AKD_SETMSGBINARY  =            (WM_USER + 212);  //0x420
  {$EXTERNALSYM AKD_SETMSGBINARY}
  AKD_GETCODEPAGELIST  =         (WM_USER + 213);  //0x420
  {$EXTERNALSYM AKD_GETCODEPAGELIST}
  AKD_RECENTFILES  =             (WM_USER + 214);  //0x420
  {$EXTERNALSYM AKD_RECENTFILES}
  AKD_SEARCHHISTORY  =           (WM_USER + 215);  //0x420
  {$EXTERNALSYM AKD_SEARCHHISTORY}

//Windows
  AKD_GETMODELESS  =             (WM_USER + 251);  //0x420
  {$EXTERNALSYM AKD_GETMODELESS}
  AKD_SETMODELESS  =             (WM_USER + 252);  //0x420
  {$EXTERNALSYM AKD_SETMODELESS}
  AKD_RESIZE  =                  (WM_USER + 253);  //0x420
  {$EXTERNALSYM AKD_RESIZE}
  AKD_DOCK  =                    (WM_USER + 254);  //0x420
  {$EXTERNALSYM AKD_DOCK}

//Thread
  AKD_GLOBALALLOC  =             (WM_USER + 281);  //0x420
  {$EXTERNALSYM AKD_GLOBALALLOC}
  AKD_GLOBALLOCK  =              (WM_USER + 282);  //0x420
  {$EXTERNALSYM AKD_GLOBALLOCK}
  AKD_GLOBALUNLOCK  =            (WM_USER + 283);  //0x420
  {$EXTERNALSYM AKD_GLOBALUNLOCK}
  AKD_GLOBALFREE  =              (WM_USER + 284);  //0x420
  {$EXTERNALSYM AKD_GLOBALFREE}
  AKD_STRLENA  =                 (WM_USER + 285);  //0x420
  {$EXTERNALSYM AKD_STRLENA}
  AKD_STRLENW  =                 (WM_USER + 286);  //0x420
  {$EXTERNALSYM AKD_STRLENW}
  AKD_CREATEWINDOW  =            (WM_USER + 287);  //0x420
  {$EXTERNALSYM AKD_CREATEWINDOW}
  AKD_WAITKEYBOARD  =            (WM_USER + 288);  //0x420
  {$EXTERNALSYM AKD_WAITKEYBOARD}
  AKD_GETQUEUE  =                (WM_USER + 289);  //0x420
  {$EXTERNALSYM AKD_GETQUEUE}
  AKD_POSTMESSAGE  =             (WM_USER + 290);  //0x420
  {$EXTERNALSYM AKD_POSTMESSAGE}

//AkelPad 4.x messages
  AKD_EXGETTEXTLENGTH  =         (WM_USER + 401);  //0x420
  {$EXTERNALSYM AKD_EXGETTEXTLENGTH}
  AKD_EXGETTEXTRANGEA  =         (WM_USER + 402);  //0x420
  {$EXTERNALSYM AKD_EXGETTEXTRANGEA}
  AKD_EXGETTEXTRANGEW  =         (WM_USER + 403);  //0x420
  {$EXTERNALSYM AKD_EXGETTEXTRANGEW}
  AKD_GETSTATUSPOSTYPE  =        (WM_USER + 404);  //0x420
  {$EXTERNALSYM AKD_GETSTATUSPOSTYPE}


//AKDN_MAIN_ONSTART
//_________________
//
//Internal message.
//
//
//AKDN_MAIN_ONSTART_PRESHOW
//_________________________
//
//Notification message, sends to the main procedure before showing main window.
//
//wParam              == not used
//(NMAINSHOW *)lParam == show main window characteristics, program show code is:
//                         NMAINSHOW *nms=(NMAINSHOW *)lParam;
//
//                         ShowWindow(hMainWnd, (nms->dwStyle == WS_MAXIMIZE)?SW_SHOWMAXIMIZED:SW_SHOW);
//                         if (nms->dwShow != SW_SHOWNORMAL) ShowWindow(hMainWnd, nms->dwShow);
//
//Return Value
// zero
//
//
//AKDN_MAIN_ONSTART_SHOW
//______________________
//
//Notification message, sends to the main procedure when main window is shown.
//
//wParam == not used
//lParam == not used
//
//Return Value
// zero
//
//
//AKDN_MAIN_ONSTART_FINISH
//________________________
//
//Notification message, sends to the main procedure when main window is finished initialize.
//
//wParam == not used
//lParam == not used
//
//Return Value
// zero
//
//
//AKDN_MAIN_ONSTART_IDLE
//______________________
//
//Notification message, sends to the main procedure when main window is finished initialize and there is no messages in the main thread's queue.
//
//wParam == not used
//lParam == not used
//
//Return Value
// zero
//
//
//AKDN_MAIN_ONFINISH
//__________________
//
//Notification message, sends to the main procedure before destroying main window.
//
//wParam == not used
//lParam == not used
//
//Return Value
// zero
//
//
//AKDN_EDIT_ONSTART
//_________________
//
//Notification message, sends to the main procedure after edit window created.
//
//(HWND)wParam == edit window
//lParam       == not used
//
//Return Value
// zero
//
//
//AKDN_EDIT_ONFINISH
//__________________
//
//Notification message, sends to the main procedure before destroying edit window.
//
//(HWND)wParam == edit window
//lParam       == not used
//
//Return Value
// zero
//
//
//AKDN_FRAME_NOWINDOWS
//____________________
//
//Notification message, sends to the main procedure when no frame windows in MDI client.
//
//wParam == not used
//lParam == not used
//
//Return Value
// zero
//
//
//AKDN_FRAME_ACTIVATE
//___________________
//
//Notification message, sends to the main procedure when frame window has been activated.
//
//wParam       == not used
//(HWND)lParam == handle to the MDI frame window being activated
//
//Return Value
// zero
//
//
//AKDN_DOCK_GETMINMAXINFO
//_______________________
//
//Notification message, sends to the main procedure before changing dock window size.
//
//(DOCK *)wParam       == pointer to a DOCK structure
//(MINMAXINFO *)lParam == pointer to a MINMAXINFO structure
//
//Return Value
// zero
//
//
//AKDN_ACTIVATE
//_____________
//
//Notification message, sends to the main procedure after another instance of the program activates main window.
//
//wParam == not used
//lParam == not used
//
//Return Value
// zero
//
//
//AKDN_SIZE
//_________
//
//Notification message, sends to the main procedure before the main window client RECT changed.
//
//wParam           == not used
//(NSIZE *)lParam  == pointer to a NSIZE structure
//
//Return Value
// zero
//
//
//AKDN_OPENDOCUMENT_START
//_______________________
//
//Notification message, sends to the main procedure before document opened.
//
//(HWND)wParam            == edit window
//(NOPENDOCUMENT *)lParam == pointer to a NOPENDOCUMENT structure
//                           (NOPENDOCUMENTA *)lParam   if bOldWindows == TRUE
//                           (NOPENDOCUMENTW *)lParam   if bOldWindows == FALSE
//
//Return Value
// zero
//
//
//AKDN_OPENDOCUMENT_FINISH
//________________________
//
//Notification message, sends to the main procedure after document opened.
//
//(HWND)wParam == edit window
//(int)lParam  == see EOD_* defines
//
//Return Value
// zero
//
//
//AKDN_SAVEDOCUMENT_START
//_______________________
//
//Notification message, sends to the main procedure before document saved.
//
//(HWND)wParam            == edit window
//(NSAVEDOCUMENT *)lParam == pointer to a NSAVEDOCUMENT structure
//                           (NSAVEDOCUMENTA *)lParam   if bOldWindows == TRUE
//                           (NSAVEDOCUMENTW *)lParam   if bOldWindows == FALSE
//
//Return Value
// zero
//
//
//AKDN_SAVEDOCUMENT_FINISH
//________________________
//
//Notification message, sends to the main procedure after document saved.
//
//(HWND)wParam == edit window
//(int)lParam  == see ESD_* defines
//
//Return Value
// zero
//
//
//AKDN_DLLCALL
//____________
//
//Notification message, sends to the main procedure after plugin call.
//
//wParam               == not used
//(PLUGINDATA *)lParam == pointer to a PLUGINDATA structure
//
//Return Value
// zero
//
//
//AKDN_DLLUNLOAD
//______________
//
//Notification message, sends to the main procedure after plugin unload.
//
//wParam                  == not used
//(unsigned char *)lParam == unloaded function name, format "Plugin::Function"
//                           (char *)lParam     if bOldWindows == TRUE
//                           (wchar_t *)lParam  if bOldWindows == FALSE
//
//Return Value
// zero
//
//
//AKDN_HOTKEY
//___________
//
//Notification message, sends to the main procedure when keyboard key is pressed.
//
//(WORD)wParam   == hotkey returned by HKM_GETHOTKEY
//(BOOL *)lParam == TRUE   stop message processing
//                  FALSE  continue message processing (default)
//
//Return Value
// zero
//
//
//AKDN_CONTEXTMENU
//________________
//
//Notification message, sends to the main procedure before displaying context menu.
//
//wParam                 == not used
//(NCONTEXTMENU *)lParam == pointer to a NCONTEXTMENU structure
//
//Return Value
// zero
//
//
//AKDN_RECENTFILESDELETE
//______________________
//
//Notification message, sends to the main procedure before displaying message about deleted non-existent recent files records.
//
//(int)wParam    == records deleted
//(BOOL *)lParam == TRUE   show message (default)
//                  FALSE  don't show message
//
//Return Value
// zero
//
//
//AKDN_SEARCH_ENDED
//_________________
//
//Notification message, sends to the main procedure after find/replace dialog found nothing.
//
//(HWND)wParam == find/replace dialog
//lParam       == not used
//
//Return Value
// zero
//
//
//AKD_GETMAINPROC, AKD_GETEDITPROC, AKD_GETFRAMEPROC
//_______________  _______________  ________________
//
//Get procedure from main window subclass chain.
//
//(int)wParam            == index of procedure in chain, if positive search from top
//                          if negative from beginning
//(WNDPROCDATA **)lParam == procedure data
//
//Return Value
// zero, if successfull
//
//Example (get program main procedure data):
// WNDPROCDATA *wpd;
// SendMessage(pd->hMainWnd, AKD_GETMAINPROC, -1, (LPARAM)&wpd);
//
//
//AKD_SETMAINPROC, AKD_SETEDITPROC, AKD_SETFRAMEPROC
//_______________  _______________  ________________
//
//Set procedure in main window subclass chain.
//
//(WNDPROC)wParam        == procedure address,
//                          if (wParam == NULL) then lParam must point to the procedure
//                            data that will be removed from main window subclass chain.
//(WNDPROCDATA **)lParam == procedure data,
//                          if (lParam == NULL) then create new procedure data
//                          if (*(WNDPROCDATA **)lParam == NULL) then create new
//                            procedure data and set it on top of the main window
//                            subclass chain.
//                          if (*(WNDPROCDATA **)lParam != NULL) then set wParam
//                            procedure in procedure data and update main window
//                            subclass chain.
//
//Return Value
// zero
//
//Example:
// WNDPROCDATA *wpd=NULL;
// LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
//   //...
//
//   return wpd->NextProc(hWnd, uMsg, wParam, lParam);
// }
// wpd=NULL;
// SendMessage(pd->hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&wpd);
//
//
//AKD_GETMAINPROCRET, AKD_GETEDITPROCRET, AKD_GETFRAMEPROCRET
//__________________  __________________  ___________________
//
//Get return procedure from main window subclass chain. This procedure calls after message have been processed.
//
//(int)wParam               == index of procedure in chain, if positive search from top
//                             if negative from beginning
//(WNDPROCRETDATA **)lParam == procedure data
//
//Return Value
// zero, if successfull
//
//Example:
// WNDPROCRETDATA *wprd;
// SendMessage(pd->hMainWnd, AKD_GETMAINPROC, 2, (LPARAM)&wprd);
//
//
//AKD_SETMAINPROCRET, AKD_SETEDITPROCRET, AKD_SETFRAMEPROCRET
//__________________  __________________  ___________________
//
//Set return procedure in main window subclass chain. This procedure calls after message have been processed.
//
//(WNDPROC)wParam        == procedure address,
//                          if (wParam == NULL) then lParam must point to the procedure
//                            data that will be removed from main window subclass chain.
//(WNDPROCDATA **)lParam == procedure data,
//                          if (lParam == NULL) then create new procedure data
//                          if (*(WNDPROCDATA **)lParam == NULL) then create new
//                            procedure data and set it on top of the main window
//                            subclass chain.
//                          if (*(WNDPROCDATA **)lParam != NULL) then set wParam
//                            procedure in procedure data and update main window
//                            subclass chain.
//
//Return Value
// zero
//
//Example:
// WNDPROCRETDATA *wprd=NULL;
// void CALLBACK NewMainProcRet(CWPRETSTRUCT *cwprs)
// {
//   if (cwprs->message == WM_NOTIFY)
//   {
//     if (cwprs->wParam == ID_EDIT)
//     {
//       //...
//     }
//   }
//
//   if (wprd->NextProc)
//     wprd->NextProc(cwprs);
// }
// wprd=NULL;
// SendMessage(pd->hMainWnd, AKD_SETMAINPROCRET, (WPARAM)NewMainProcRet, (LPARAM)&wprd);
//
//
//AKD_DLLCALL
//___________
//
//Call dll.
//
//wParam                   == not used
//(PLUGINCALLSEND *)lParam == pointer to a PLUGINCALLSEND structure if SendMessage used
//                            (PLUGINCALLSENDA *)lParam   if bOldWindows == TRUE
//                            (PLUGINCALLSENDW *)lParam   if bOldWindows == FALSE
//                            or pointer to a PLUGINCALLPOST, allocated with GlobalAlloc, if PostMessage used
//                            (PLUGINCALLPOSTA *)lParam   if bOldWindows == TRUE
//                            (PLUGINCALLPOSTW *)lParam   if bOldWindows == FALSE
//
//Return Value
// see EDL_* defines
//
//Example SendMessage (bOldWindows == TRUE):
// PLUGINCALLSENDA pcs;
// pcs.pFunction="Plugin::Function";
// pcs.bOnStart=FALSE;
// pcs.lParam=0;
// pcs.lpbAutoLoad=NULL;
// SendMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)&pcs);
//
//Example SendMessage (bOldWindows == FALSE):
// PLUGINCALLSENDW pcs;
// pcs.wpFunction=L"Plugin::Function";
// pcs.bOnStart=FALSE;
// pcs.lParam=0;
// pcs.lpbAutoLoad=NULL;
// SendMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)&pcs);
//
//Example PostMessage (bOldWindows == TRUE):
// PLUGINCALLPOSTA *pcp;
// if (pcp=(PLUGINCALLPOSTA *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTA)))
// {
//   lstrcpynA(pcp->szFunction, "Plugin::Function", MAX_PATH);
//   pcp->bOnStart=FALSE;
//   pcp->lParam=0;
//   PostMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)pcp);
// }
//
//Example PostMessage (bOldWindows == FALSE):
// PLUGINCALLPOSTW *pcp;
// if (pcp=(PLUGINCALLPOSTW *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW)))
// {
//   lstrcpynW(pcp->wszFunction, L"Plugin::Function", MAX_PATH);
//   pcp->bOnStart=FALSE;
//   pcp->lParam=0;
//   PostMessage(pd->hMainWnd, AKD_DLLCALL, 0, (LPARAM)pcp);
// }
//
//
//AKD_DLLUNLOAD
//_____________
//
//Exit from thread and unload dll.
//
//(HMODULE)wParam == handle to the module
//(HANDLE)lParam  == handle to the thread, NULL if plugin not exiting from thread
//
//Return Value
// zero
//
//Example:
// PostMessage(pd->hMainWnd, AKD_DLLUNLOAD, (WPARAM)pd->hInstanceDLL, (LPARAM)NULL);
//
//
//AKD_DLLFIND
//___________
//
//Get dll stack function structure pointer.
//
//(unsigned char *)wParam == function name, format "Plugin::Function"
//                           (char *)wParam      if bOldWindows == TRUE
//                           (wchar_t *)wParam   if bOldWindows == FALSE
//(WORD)lParam            == hotkey returned by HKM_GETHOTKEY,
//                           to search by hotkey set wParam to NULL
//
//Return Value
// pointer to a PLUGINFUNCTION structure
//
//Example find by name (bOldWindows == TRUE):
// PLUGINFUNCTIONA *pf;
// if (pf=(PLUGINFUNCTIONA *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)"SomePlugin::SomeFunction", 0))
//   if (pf->bRunning) MessageBoxA(NULL, "Plugin is running", NULL, 0);
//
//Example find by name (bOldWindows == FALSE):
// PLUGINFUNCTIONW *pf;
// if (pf=(PLUGINFUNCTIONW *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)L"SomePlugin::SomeFunction", 0))
//   if (pf->bRunning) MessageBoxW(NULL, L"Plugin is running", NULL, 0);
//
//Example find by hotkey (bOldWindows == TRUE):
// PLUGINFUNCTIONA *pf;
// if (pf=(PLUGINFUNCTIONA *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)NULL, 3112))
//   if (pf->bRunning) MessageBoxA(NULL, "Plugin is running", NULL, 0);
//
//Example find by hotkey (bOldWindows == FALSE):
// PLUGINFUNCTIONW *pf;
// if (pf=(PLUGINFUNCTIONW *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)NULL, 3112))
//   if (pf->bRunning) MessageBoxW(NULL, L"Plugin is running", NULL, 0);
//
//
//AKD_DLLADD
//__________
//
//Add dll stack function structure.
//
//wParam                   == not used
//(PLUGINFUNCTION *)lParam == function structure pointer
//                            (PLUGINFUNCTIONA *)lParam   if bOldWindows == TRUE
//                            (PLUGINFUNCTIONW *)lParam   if bOldWindows == FALSE
//
//Return Value
// pointer to a PLUGINFUNCTION structure in stack
//
//Example add plugin hotkey (bOldWindows == TRUE):
// BOOL CALLBACK PluginProc(void *lpParameter)
// {
//   return TRUE; //TRUE - catch hotkey, FALSE - do default hotkey processing
// }
// PLUGINFUNCTIONA pf;
// pf.szFunction[0]='\0';
// pf.nFunctionLen=0;
// pf.wHotkey=589;       //Ctrl+M
// pf.bOnStart=FALSE;
// pf.bRunning=FALSE;
// pf.PluginProc=(PLUGINPROC)PluginProc;
// pf.lpParameter=NULL;
// SendMessage(pd->hMainWnd, AKD_DLLADD, 0, (LPARAM)&pf);
//
//Example add plugin hotkey (bOldWindows == FALSE):
// BOOL CALLBACK PluginProc(void *lpParameter)
// {
//   return TRUE; //TRUE - catch hotkey, FALSE - do default hotkey processing
// }
// PLUGINFUNCTIONW pf;
// pf.wszFunction[0]='\0';
// pf.nFunctionLen=0;
// pf.wHotkey=589;       //Ctrl+M
// pf.bOnStart=FALSE;
// pf.bRunning=FALSE;
// pf.PluginProc=(PLUGINPROC)PluginProc;
// pf.lpParameter=NULL;
// SendMessage(pd->hMainWnd, AKD_DLLADD, 0, (LPARAM)&pf);
//
//
//AKD_DLLDELETE
//_____________
//
//Delete dll stack function structure.
//
//wParam                   == not used
//(PLUGINFUNCTION *)lParam == pointer to a PLUGINFUNCTION structure
//                            (PLUGINFUNCTIONA *)lParam   if bOldWindows == TRUE
//                            (PLUGINFUNCTIONW *)lParam   if bOldWindows == FALSE
//
//Return Value
// zero
//
//Example (bOldWindows == TRUE):
// PLUGINFUNCTIONA *pf;
// if (pf=(PLUGINFUNCTIONA *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)"SomePlugin::SomeFunction", 0))
// {
//   SendMessage(pd->hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pf);
//   pf=NULL;
// }
//
//Example (bOldWindows == FALSE):
// PLUGINFUNCTIONW *pf;
// if (pf=(PLUGINFUNCTIONW *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)L"SomePlugin::SomeFunction", 0))
// {
//   SendMessage(pd->hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pf);
//   pf=NULL;
// }
//
//
//AKD_DLLSAVE
//___________
//
//Save dll stack.
//
//wParam      == not used
//(int)lParam == see DLLS_* defines
//
//Return Value
// TRUE  success
// FALSE error
//
//Example:
// SendMessage(pd->hMainWnd, AKD_DLLSAVE, 0, DLLS_CLEAR);
//
//
//AKD_CALLPROC
//____________
//
//Call procedure.
//
//(PLUGINPROC)wParam == procedure address
//(void *)lParam     == pointer to a variable to be passed to the procedure
//
//Return Value
// void
//
//Example:
// void CALLBACK MyProcedure(void *lpParameter)
// {
// }
// PostMessage(pd->hMainWnd, AKD_CALLPROC, (WPARAM)MyProcedure, (LPARAM)NULL);
//
//
//AKD_BEGINOPTIONS
//________________
//
//Begins read or save plugin or program options.
//
//(DWORD)wParam           == see POB_* defines
//(unsigned char *)lParam == plugin name
//                           (char *)lParam      if bOldWindows == TRUE
//                           (wchar_t *)lParam   if bOldWindows == FALSE
//                           if NULL then begin work with program options
//
//Return Value
// HANDLE
//
//Example read (bOldWindows == TRUE):
// PLUGINOPTIONA po;
// HANDLE hOptions;
// char szDir[MAX_PATH];
//
// if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_READ, (LPARAM)"AutoSave"))
// {
//   po.szOptionName="SaveDir";
//   po.dwType=PO_STRING;
//   po.lpData=(LPBYTE)szDir;
//   po.dwData=MAX_PATH;
//   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);
//
//   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
// }
//
//Example read (bOldWindows == FALSE):
// PLUGINOPTIONW po;
// HANDLE hOptions;
// wchar_t wszDir[MAX_PATH];
//
// if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_READ, (LPARAM)L"AutoSave"))
// {
//   po.wszOptionName=L"SaveDir";
//   po.dwType=PO_STRING;
//   po.lpData=(LPBYTE)wszDir;
//   po.dwData=MAX_PATH * sizeof(wchar_t);
//   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);
//
//   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
// }
//
//Example save (bOldWindows == TRUE):
// PLUGINOPTIONA po;
// HANDLE hOptions;
// char szDir[MAX_PATH]="C:\\Temp";
//
// if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_SAVE, (LPARAM)"AutoSave"))
// {
//   po.szOptionName="SaveDir";
//   po.dwType=PO_STRING;
//   po.lpData=(LPBYTE)szDir;
//   po.dwData=lstrlenA(szDir) + 1;
//   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);
//
//   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
// }
//
//Example save (bOldWindows == FALSE):
// PLUGINOPTIONW po;
// HANDLE hOptions;
// wchar_t wszDir[MAX_PATH]=L"C:\\Temp";
//
// if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_SAVE, (LPARAM)L"AutoSave"))
// {
//   po.wszOptionName=L"SaveDir";
//   po.dwType=PO_STRING;
//   po.lpData=(LPBYTE)wszDir;
//   po.dwData=lstrlenW(wszDir) * sizeof(wchar_t) + 2;
//   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);
//
//   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
// }
//
//
//AKD_OPTION
//__________
//
//Read or save plugin option.
//
//(HANDLE)wParam         == HANDLE returned by AKD_BEGINOPTIONS
//(PLUGINOPTION *)lParam == pointer to a PLUGINOPTION structure
//                          (PLUGINOPTIONA *)lParam   if bOldWindows == TRUE
//                          (PLUGINOPTIONW *)lParam   if bOldWindows == FALSE
//Return Value
// Size of the data copied to the buffer
//
//Example:
// See AKD_BEGINOPTIONS examples
//
//
//AKD_ENDOPTION
//_____________
//
//Ends plugin options.
//
//(HANDLE)wParam == HANDLE returned by AKD_BEGINOPTIONS
//lParam         == not used
//
//Return Value
// TRUE   success
// FALSE  failed
//
//Example:
// See AKD_BEGINOPTIONS examples
//
//
//AKD_INIOPEN
//___________
//
//Opens ini file.
//
//(DWORD)wParam           == see POB_* defines
//(unsigned char *)lParam == ini file
//                           (char *)lParam      if bOldWindows == TRUE
//                           (wchar_t *)lParam   if bOldWindows == FALSE
//
//Return Value
// HANDLE
//
//Example read (bOldWindows == TRUE):
// INIVALUEA iv;
// HANDLE hIniFile;
// char szDir[MAX_PATH];
//
// if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_READ, (LPARAM)"C:\\File.ini"))
// {
//   iv.szSection="Options";
//   iv.szKey="SaveDir";
//   iv.dwType=INI_STRINGANSI;
//   iv.lpData=(LPBYTE)szDir;
//   iv.dwData=MAX_PATH;
//   SendMessage(pd->hMainWnd, AKD_INIGETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);
//
//   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
// }
//
//Example read (bOldWindows == FALSE):
// INIVALUEW iv;
// HANDLE hIniFile;
// wchar_t wszDir[MAX_PATH];
//
// if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_READ, (LPARAM)L"C:\\File.ini"))
// {
//   iv.wszSection=L"Options";
//   iv.wszKey=L"SaveDir";
//   iv.dwType=INI_STRINGUNICODE;
//   iv.lpData=(LPBYTE)wszDir;
//   iv.dwData=MAX_PATH * sizeof(wchar_t);
//   SendMessage(pd->hMainWnd, AKD_INIGETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);
//
//   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
// }
//
//Example save (bOldWindows == TRUE):
// INIVALUEA iv;
// HANDLE hIniFile;
// char szDir[MAX_PATH]="C:\\Temp";
//
// if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_SAVE, (LPARAM)"C:\\File.ini"))
// {
//   iv.szSection="Options";
//   iv.szKey="SaveDir";
//   iv.dwType=INI_STRINGANSI;
//   iv.lpData=(LPBYTE)szDir;
//   iv.dwData=lstrlenA(szDir) + 1;
//   SendMessage(pd->hMainWnd, AKD_INISETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);
//
//   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
// }
//
//Example save (bOldWindows == FALSE):
// INIVALUEW iv;
// HANDLE hIniFile;
// wchar_t wszDir[MAX_PATH]=L"C:\\Temp";
//
// if (hIniFile=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_SAVE, (LPARAM)L"C:\\File.ini"))
// {
//   iv.wszSection=L"Options";
//   iv.wszKey=L"SaveDir";
//   iv.dwType=INI_STRINGUNICODE;
//   iv.lpData=(LPBYTE)wszDir;
//   iv.dwData=lstrlenW(wszDir) * sizeof(wchar_t) + 2;
//   SendMessage(pd->hMainWnd, AKD_INISETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);
//
//   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
// }
//
//
//AKD_INIGETSECTION
//_________________
//
//Retrieve ini section handle.
//
//(HANDLE)wParam          == ini file handle
//(unsigned char *)lParam == section name
//                           (char *)lParam      if bOldWindows == TRUE
//                           (wchar_t *)lParam   if bOldWindows == FALSE
//
//Return Value
// HANDLE
//
//Example (bOldWindows == TRUE):
// HANDLE hIniSection;
//
// if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
//   SendMessage(pd->hMainWnd, AKD_INICLEARSECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);
//
//Example (bOldWindows == FALSE):
// HANDLE hIniSection;
//
// if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
//   SendMessage(pd->hMainWnd, AKD_INICLEARSECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);
//
//
//AKD_INICLEARSECTION
//___________________
//
//Removes all keys in ini section.
//
//(HANDLE)wParam == ini file handle
//(HANDLE)lParam == ini section handle
//
//Return Value
// zero
//
//Example:
// See AKD_INIGETSECTION example
//
//
//AKD_INIDELETESECTION
//____________________
//
//Deletes ini section.
//
//(HANDLE)wParam == ini file handle
//(HANDLE)lParam == ini section handle
//
//Return Value
// zero
//
//Example (bOldWindows == TRUE):
// HANDLE hIniSection;
//
// if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
//   SendMessage(pd->hMainWnd, AKD_INIDELETESECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);
//
//Example (bOldWindows == FALSE):
// HANDLE hIniSection;
//
// if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
//   SendMessage(pd->hMainWnd, AKD_INIDELETESECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);
//
//
//AKD_INIGETKEY
//_____________
//
//Retrieve key handle.
//
//(HANDLE)wParam          == ini section handle
//(unsigned char *)lParam == key name
//                           (char *)lParam      if bOldWindows == TRUE
//                           (wchar_t *)lParam   if bOldWindows == FALSE
//Return Value
// HANDLE
//
//Example (bOldWindows == TRUE):
// HANDLE hIniSection;
// HANDLE hIniKey;
//
// if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
//   if (hIniKey=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETKEY, (WPARAM)hIniSection, (LPARAM)"SaveDir"))
//     SendMessage(pd->hMainWnd, AKD_INIDELETEKEY, (WPARAM)hIniSection, (LPARAM)hIniKey);
//
//Example (bOldWindows == FALSE):
// HANDLE hIniSection;
// HANDLE hIniKey;
//
// if (hIniSection=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
//   if (hIniKey=(HANDLE)SendMessage(pd->hMainWnd, AKD_INIGETKEY, (WPARAM)hIniSection, (LPARAM)L"SaveDir"))
//     SendMessage(pd->hMainWnd, AKD_INIDELETEKEY, (WPARAM)hIniSection, (LPARAM)hIniKey);
//
//
//AKD_INIDELETEKEY
//________________
//
//Deletes ini key.
//
//(HANDLE)wParam == ini section handle
//(HANDLE)lParam == key handle
//
//Return Value
// zero
//
//Example:
// See AKD_INIGETKEY examples
//
//
//AKD_INIGETVALUE
//_______________
//
//Retrieve ini value.
//
//(HANDLE)wParam     == ini file handle
//(INIVALUE *)lParam == pointer to a INIVALUE structure
//                      (INIVALUEA *)lParam   if bOldWindows == TRUE
//                      (INIVALUEW *)lParam   if bOldWindows == FALSE
//
//Return Value
// Size of the data copied to the buffer
//
//Example:
// See AKD_INIOPEN examples
//
//
//AKD_INISETVALUE
//_______________
//
//Set ini value.
//
//(HANDLE)wParam     == ini file handle
//(INIVALUE *)lParam == pointer to a INIVALUE structure
//                      (INIVALUEA *)lParam   if bOldWindows == TRUE
//                      (INIVALUEW *)lParam   if bOldWindows == FALSE
//
//Return Value
// TRUE   success
// FALSE  failed
//
//Example:
// See AKD_INIOPEN examples
//
//
//AKD_INICLOSE
//____________
//
//Close ini file handle.
//
//(HANDLE)wParam == ini file handle
//lParam         == not used
//
//Return Value
// TRUE   success
// FALSE  failed
//
//Example:
// See AKD_INIOPEN examples
//
//
//AKD_SAVEDOCUMENT
//________________
//
//Save file.
//
//(HWND)wParam           == edit window
//(SAVEDOCUMENT *)lParam == pointer to a SAVEDOCUMENT structure
//                         (SAVEDOCUMENTA *)lParam   if bOldWindows == TRUE
//                         (SAVEDOCUMENTW *)lParam   if bOldWindows == FALSE
//
//Return Value
// see ESD_* defines
//
//Example (bOldWindows == TRUE):
// SAVEDOCUMENTA sd;
//
// lstrcpynA(sd.szFile, "C:\\MyFile.txt", MAX_PATH);
// sd.nCodePage=65001;
// sd.bBOM=TRUE;
// sd.dwFlags=SD_UPDATE;
// SendMessage(pd->hMainWnd, AKD_SAVEDOCUMENT, (WPARAM)pd->hWndEdit, (LPARAM)&sd);
//
//Example (bOldWindows == FALSE):
// SAVEDOCUMENTW sd;
//
// lstrcpynW(sd.wszFile, L"C:\\MyFile.txt", MAX_PATH);
// sd.nCodePage=65001;
// sd.bBOM=TRUE;
// sd.dwFlags=SD_UPDATE;
// SendMessage(pd->hMainWnd, AKD_SAVEDOCUMENT, (WPARAM)pd->hWndEdit, (LPARAM)&sd);
//
//
//AKD_GETTEXTLENGTH
//_________________
//
//Get edit window text length.
//
//(HWND)wParam == edit window
//lParam       == not used
//
//Return Value
// text length
//
//Example:
// int nLength=SendMessage(pd->hMainWnd, AKD_GETTEXTLENGTH, (WPARAM)pd->hWndEdit, 0);
//
//
//AKD_GETTEXTRANGE
//________________
//
//Retrieves a specified range of characters from a edit control.
//
//(HWND)wParam           == edit window
//(GETTEXTRANGE *)lParam == pointer to a GETTEXTRANGE structure
//
//Return Value
// Text length in TCHARs without null character
//
//Example (bOldWindows == TRUE):
// GETTEXTRANGE gtr;
//
// gtr.cpMin=0;
// gtr.cpMax=-1;
// if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGE, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
// {
//   MessageBoxA(pd->hMainWnd, (char *)gtr.pText, "Test", MB_OK);
//   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
// }
//
//Example (bOldWindows == FALSE):
// GETTEXTRANGE gtr;
//
// gtr.cpMin=0;
// gtr.cpMax=-1;
// if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGE, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
// {
//   MessageBoxW(pd->hMainWnd, (wchar_t *)gtr.pText, L"Test", MB_OK);
//   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
// }
//
//
//AKD_GETSELTEXTW
//_______________
//
//Retrieves the currently selected text in a edit control.
//
//(HWND)wParam  == edit window
//(int *)lParam == pointer to a variable that receive text length, can be NULL
//
//Return Value
// text pointer
//
//Example:
// wchar_t *wpText;
// int nTextLen=0;
//
// wpText=(wchar_t *)SendMessage(pd->hMainWnd, AKD_GETSELTEXTW, (WPARAM)pd->hWndEdit, (LPARAM)&nTextLen);
// SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpText);
//
//
//AKD_FREETEXT
//____________
//
//Free text buffer allocated with AKD_GETTEXTRANGE.
//
//wParam                  == not used
//(unsigned char *)lParam == buffer pointer
//
//Return Value
// TRUE   success
// FALSE  failed
//
//See AKD_GETTEXTRANGE.
//
//
//AKD_REPLACESELA
//_______________
//
//Replace selection of the edit control.
//
//(HWND)wParam   == edit window
//(char *)lParam == text pointer
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_REPLACESELA, (WPARAM)pd->hWndEdit, (LPARAM)"SomeString");
//
//
//AKD_REPLACESELW
//_______________
//
//Replace selection of the edit control.
//
//(HWND)wParam      == edit window
//(wchar_t *)lParam == text pointer
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_REPLACESELW, (WPARAM)pd->hWndEdit, (LPARAM)L"SomeString");
//
//
//AKD_PASTE
//_________
//
//Paste text from clipboard to the edit control.
//
//(HWND)wParam == edit window
//(int)lParam  == see PASTE_* defines.
//
//Return Value
// TRUE   success
// FALSE  failed
//
//Example:
// SendMessage(pd->hMainWnd, AKD_PASTE, (WPARAM)pd->hWndEdit, PASTE_UNICODE);
//
//
//AKD_COPY
//________
//
//Copy text to clipboard from edit control.
//
//(HWND)wParam == edit window
//lParam       == not used
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_COPY, (WPARAM)pd->hWndEdit, 0);
//
//
//AKD_TEXTFIND
//____________
//
//Finds text in a edit control.
//
//(HWND)wParam       == edit window
//(TEXTFIND *)lParam == pointer to a TEXTFIND structure
//                      (TEXTFINDA *)lParam   if bOldWindows == TRUE
//                      (TEXTFINDW *)lParam   if bOldWindows == FALSE
//Return Value
// Character position of the next match. If there are no more matches, the return value is –1.
//
//Example (bOldWindows == TRUE):
// TEXTFINDA tf;
//
// tf.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
// tf.pFindIt="Text to find";
// SendMessage(pd->hMainWnd, AKD_TEXTFIND, (WPARAM)pd->hWndEdit, (LPARAM)&tf);
//
//Example (bOldWindows == FALSE):
// TEXTFINDW tf;
//
// tf.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
// tf.wpFindIt=L"Text to find";
// SendMessage(pd->hMainWnd, AKD_TEXTFIND, (WPARAM)pd->hWndEdit, (LPARAM)&tf);
//
//
//AKD_TEXTREPLACE
//_______________
//
//Replaces text in a edit control.
//
//(HWND)wParam          == edit window
//(TEXTREPLACE *)lParam == pointer to a TEXTREPLACE structure
//                         (TEXTREPLACEA *)lParam   if bOldWindows == TRUE
//                         (TEXTREPLACEW *)lParam   if bOldWindows == FALSE
//Return Value
// Character position of the next match. If there are no more matches, the return value is –1.
//
//Example (bOldWindows == TRUE):
// TEXTREPLACEA tr;
//
// tr.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
// tr.pFindIt="Text to find";
// tr.pReplaceWith="Text to replace";
// tr.bAll=TRUE;
// SendMessage(pd->hMainWnd, AKD_TEXTREPLACE, (WPARAM)pd->hWndEdit, (LPARAM)&tr);
//
//Example (bOldWindows == FALSE):
// TEXTREPLACEW tr;
//
// tr.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
// tr.wpFindIt=L"Text to find";
// tr.wpReplaceWith=L"Text to replace";
// tr.bAll=TRUE;
// SendMessage(pd->hMainWnd, AKD_TEXTREPLACE, (WPARAM)pd->hWndEdit, (LPARAM)&tr);
//
//
//AKD_RECODESEL
//_____________
//
//Recode text in a edit control.
//
//(HWND)wParam         == edit window
//(TEXTRECODE *)lParam == pointer to a TEXTRECODE structure
//
//Return Value
// zero
//
//Example:
// TEXTRECODE tr;
//
// tr.nCodePageFrom=1251;
// tr.nCodePageTo=866;
// SendMessage(pd->hMainWnd, AKD_RECODESEL, (WPARAM)pd->hWndEdit, (LPARAM)&tr);
//
//
//AKD_GETCHARCOLOR
//________________
//
//Get colors of the specified char.
//
//(HWND)wParam        == edit window
//(CHARCOLOR *)lParam == pointer to a CHARCOLOR structure
//
//Return Value
// TRUE   specified char in selection
// FALSE  specified char not in selection
//
//Example:
// CHARCOLOR cc;
//
// cc.nCharPos=10;
// SendMessage(pd->hMainWnd, AKD_GETCHARCOLOR, (WPARAM)pd->hWndEdit, (LPARAM)&cc);
//
//
//AKD_GETFILEPRINT
//________________
//
//Retrieve global print state.
//
//wParam == not used
//lParam == not used
//
//Return Value
// TRUE  next opened file will be printed
// FALSE next opened file will not be printed
//
//Example:
// SendMessage(pd->hMainWnd, AKD_GETFILEPRINT, 0, 0);
//
//
//AKD_SETFILEPRINT
//________________
//
//Next opened file will be printed.
//
//(BOOL)wParam == TRUE  will be printed
//                FALSE will not be printed
//lParam       == not used
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_SETFILEPRINT, TRUE, 0);
//
//
//AKD_GETPRINTDLG
//_______________
//
//Get print dialog settings.
//
//wParam == not used
//lParam == not used
//
//Return Value
// pointer to a PRINTDLG structure
//
//Example (bOldWindows == TRUE):
// PRINTDLGA *pdg=(PRINTDLGA *)SendMessage(pd->hMainWnd, AKD_GETPRINTDLG, 0, 0);
//
//Example (bOldWindows == FALSE):
// PRINTDLGW *pdg=(PRINTDLGW *)SendMessage(pd->hMainWnd, AKD_GETPRINTDLG, 0, 0);
//
//
//AKD_GETPAGEDLG
//______________
//
//Get page dialog settings.
//
//wParam == not used
//lParam == not used
//
//Return Value
// pointer to a PAGESETUPDLG structure
//
//Example (bOldWindows == TRUE):
// PAGESETUPDLGA *psdg=(PAGESETUPDLGA *)SendMessage(pd->hMainWnd, AKD_GETPAGEDLG, 0, 0);
//
//Example (bOldWindows == FALSE):
// PAGESETUPDLGW *psdg=(PAGESETUPDLGW *)SendMessage(pd->hMainWnd, AKD_GETPAGEDLG, 0, 0);
//
//
//AKD_PROGRAMVERSION
//__________________
//
//Get program version.
//
//wParam == not used
//lParam == not used
//
//Return Value
// Version number. Created as: MAKE_IDENTIFIER(dwMajor, dwMinor, dwRelease, dwBuild).
//
//Example:
// DWORD dwVersion;
// DWORD dwMajor;
// DWORD dwMinor;
// DWORD dwRelease;
// DWORD dwBuild;
//
// dwVersion=SendMessage(pd->hMainWnd, AKD_PROGRAMVERSION, 0, 0);
// dwMajor=LOBYTE(LOWORD(dwVersion));
// dwMinor=HIBYTE(LOWORD(dwVersion));
// dwRelease=LOBYTE(HIWORD(dwVersion));
// dwBuild=HIBYTE(HIWORD(dwVersion));
//
//
//AKD_PROGRAMARCHITECTURE
//_______________________
//
//Get program architecture (AkelDLL) version.
//
//wParam == not used
//lParam == not used
//
//Return Value
// Version number. Created as: MAKE_IDENTIFIER(dwMajor, dwMinor, dwRelease, dwBuild).
//
//Example:
// DWORD dwVersion;
// DWORD dwMajor;
// DWORD dwMinor;
// DWORD dwRelease;
// DWORD dwBuild;
//
// dwVersion=SendMessage(pd->hMainWnd, AKD_PROGRAMARCHITECTURE, 0, 0);
// dwMajor=LOBYTE(LOWORD(dwVersion));
// dwMinor=HIBYTE(LOWORD(dwVersion));
// dwRelease=LOBYTE(HIWORD(dwVersion));
// dwBuild=HIBYTE(HIWORD(dwVersion));
//
//
//AKD_GETMAININFO
//_______________
//
//Get main AkelPad data.
//
//wParam               == not used
//(PLUGINDATA *)lParam == pointer to a PLUGINDATA structure
//
//Return Value
// zero
//
//Example:
// PLUGINDATA pd;
//
// SendMessage(pd->hMainWnd, AKD_GETMAININFO, 0, (LPARAM)&pd);
//
//
//AKD_GETEDITINFO
//_______________
//
//Get edit window info.
//
//(HWND)wParam       == edit window,
//                      NULL for current edit window
//(EDITINFO *)lParam == pointer to a EDITINFO structure
//
//Return Value
// TRUE   success
// FALSE  failed
//
//Example:
// EDITINFO ei;
//
// SendMessage(pd->hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei);
//
//
//AKD_SETMODIFY
//_____________
//
//Set edit window modification flag.
//
//(HWND)wParam == edit window,
//                NULL for current edit window
//(BOOL)lParam == TRUE  contents of edit control have been modified
//                FALSE contents of edit control haven't been modified
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_SETMODIFY, (WPARAM)pd->hWndEdit, TRUE);
//
//
//AKD_SETNEWLINE
//______________
//
//Set edit window new line format.
//
//(HWND)wParam == edit window,
//                NULL for current edit window
//(int)lParam  == see NEWLINE_* defines
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_SETNEWLINE, (WPARAM)pd->hWndEdit, NEWLINE_UNIX);
//
//
//AKD_GETFONT
//___________
//
//Get font.
//
//(HWND)wParam == edit window,
//                NULL for current edit window
//lParam       == not used
//
//Return Value
// pointer to a LOGFONT structure
//
//Example (bOldWindows == TRUE):
// LOGFONTA *lf=(LOGFONTA *)SendMessage(pd->hMainWnd, AKD_GETFONT, (WPARAM)NULL, 0);
//
//Example (bOldWindows == FALSE):
// LOGFONTW *lf=(LOGFONTW *)SendMessage(pd->hMainWnd, AKD_GETFONT, (WPARAM)NULL, 0);
//
//
//AKD_SETFONT
//___________
//
//Set font.
//
//(HWND)wParam      == edit window
//(LOGFONT *)lParam == pointer to a LOGFONT structure
//
//Return Value
// TRUE   success
// FALSE  failed
//
//Example (bOldWindows == TRUE):
// LOGFONTA *lf;
//
// lf=(LOGFONTA *)SendMessage(pd->hMainWnd, AKD_GETFONT, 0, 0);
// lf->lfHeight-=2;
// lstrcpynA(lf->lfFaceName, "Courier New", LF_FACESIZE);
// SendMessage(pd->hMainWnd, AKD_SETFONT, (WPARAM)pd->hWndEdit, (LPARAM)lf);
//
//Example (bOldWindows == FALSE):
// LOGFONTW *lf;
//
// lf=(LOGFONTW *)SendMessage(pd->hMainWnd, AKD_GETFONT, 0, 0);
// lf->lfHeight-=2;
// lstrcpynW(lf->lfFaceName, L"Courier New", LF_FACESIZE);
// SendMessage(pd->hMainWnd, AKD_SETFONT, (WPARAM)pd->hWndEdit, (LPARAM)lf);
//
//
//AKD_GETMSGCREATE
//________________
//
//Retrieve unexisted file autoanswer.
//
//wParam == not used
//lParam == not used
//
//Return Value
// AUTOANSWER_ASK   Show message (default)
// AUTOANSWER_YES   Create unexisted file
// AUTOANSWER_NO    Don't create unexisted file
//
//Example:
// SendMessage(pd->hMainWnd, AKD_GETMSGCREATE, 0, 0);
//
//
//AKD_SETMSGCREATE
//________________
//
//Create unexisted file autoanswer.
//
//(int)wParam == AUTOANSWER_ASK   Show message (default)
//               AUTOANSWER_YES   Create unexisted file
//               AUTOANSWER_NO    Don't create unexisted file
//lParam      == not used
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_SETMSGCREATE, AUTOANSWER_YES, 0);
//
//
//AKD_GETMSGBINARY
//________________
//
//Retrive binary file autoanswer.
//
//wParam == not used
//lParam == not used
//
//Return Value
// AUTOANSWER_ASK  Show message (default)
// AUTOANSWER_YES  Open binary file
// AUTOANSWER_NO   Don't open binary file
//
//Example:
// SendMessage(pd->hMainWnd, AKD_GETMSGBINARY, 0, 0);
//
//
//AKD_SETMSGBINARY
//________________
//
//Open binary file autoanswer.
//
//(int)wParam == AUTOANSWER_ASK  Show message (default)
//               AUTOANSWER_YES  Open binary file
//               AUTOANSWER_NO   Don't open binary file
//lParam      == not used
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_SETMSGBINARY, AUTOANSWER_YES, 0);
//
//
//AKD_GETCODEPAGELIST
//___________________
//
//Get codepage list.
//
//(int *)wParam == if this parameter is not NULL, then the default codepage identifier
//                 copies to the variable.
//lParam        == not used
//
//Return Value
// pointer to a codepage array, in which last element is zero
//
//Example:
// int *lpCodepageList;
// int nDefaultCodepage;
//
// lpCodepageList=(int *)SendMessage(pd->hMainWnd, AKD_GETCODEPAGELIST, (WPARAM)&nDefaultCodepage, 0);
//
//
//AKD_RECENTFILES
//_______________
//
//Recent files operations.
//
//(int)wParam           == see RF_* defines
//(RECENTFILES *)lParam == pointer to a RECENTFILES structure, can be NULL
//                         (RECENTFILESA *)lParam   if bOldWindows == TRUE
//                         (RECENTFILESW *)lParam   if bOldWindows == FALSE
//
//Return Value
// Number of recent files.
//
//Example (bOldWindows == TRUE):
// RECENTFILESA rf;
//
// SendMessage(pd->hMainWnd, AKD_RECENTFILES, RF_GET, (LPARAM)&rf);
//
//Example (bOldWindows == FALSE):
// RECENTFILESW rf;
//
// SendMessage(pd->hMainWnd, AKD_RECENTFILES, RF_GET, (LPARAM)&rf);
//
//
//AKD_SEARCHHISTORY
//_________________
//
//Search history operations.
//
//(int)wParam  == see SH_* defines
//lParam       == not used
//
//Return Value
// Number of search strings.
//
//Example:
// SendMessage(pd->hMainWnd, AKD_SEARCHHISTORY, SH_GET, 0);
//
//
//AKD_GETMODELESS
//_______________
//
//Get modeless dialog handle.
//
//wParam == not used
//lParam == not used
//
//Return Value
// dialog handle
//
//Example:
// HWND hDlg=(HWND)SendMessage(pd->hMainWnd, AKD_GETMODELESS, 0, 0);
//
//
//AKD_SETMODELESS
//_______________
//
//Set modeless dialog handle.
//
//(HWND)wParam == dialog handle
//lParam       == not used
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_SETMODELESS, (LPARAM)hMyDialog, 0);
//
//
//AKD_RESIZE
//__________
//
//Update main window size with AKDN_SIZE notification message.
//
//wParam == not used
//lParam == not used
//
//Return Value
// zero
//
//Example:
// SendMessage(pd->hMainWnd, AKD_RESIZE, 0, 0);
//
//
//AKD_DOCK
//________
//
//Message to work with dockable window.
//
//(DWORD)wParam  == see DK_* defines
//(DOCK *)lParam == pointer to a DOCK structure
//
//Return Value
// pointer to a DOCK structure
//
//Example:
// DOCK dk={0};
// DOCK *dkNew;
//
// dk.dwFlags=DKF_FIXEDSIZE;
// dk.hWnd=hDlg;
// dk.nSide=DKS_RIGHT;
// dk.rcSize.left=0;
// dk.rcSize.top=0;
// dk.rcSize.right=50;
// dk.rcSize.bottom=50;
// dkNew=(DOCK *)SendMessage(pd->hMainWnd, AKD_DOCK, DK_ADD|DK_SUBCLASS, (LPARAM)&dk);
//
// SendMessage(pd->hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkNew);
//
//
//AKD_GLOBALALLOC
//_______________
//
//Allocates the specified number of bytes from the heap (see description for GlobalAlloc in MSDN).
//
//wParam == memory allocation attributes
//lParam == number of bytes to allocate
//
//Return Value
//  handle to the newly allocated memory object
//
//Example:
// HGLOBAL hMem=(HGLOBAL)SendMessage(pd->hMainWnd, AKD_GLOBALALLOC, GPTR, 128);
//
//
//AKD_GLOBALLOCK
//______________
//
//Locks a global memory object (see description for GlobalLock in MSDN).
//
//wParam == handle to the global memory object
//lParam == not used
//
//Return Value
//  pointer to the first byte of the memory block
//
//Example:
// void *pMem=(void *)SendMessage(pd->hMainWnd, AKD_GLOBALLOCK, (WPARAM)hMem, 0);
//
//
//AKD_GLOBALUNLOCK
//________________
//
//Decrements the lock count associated with a memory object (see description for GlobalUnlock in MSDN).
//
//wParam == handle to the global memory object
//lParam == not used
//
//Return Value
// unlock result
//
//Example:
// BOOL bUnlock=SendMessage(pd->hMainWnd, AKD_GLOBALUNLOCK, (WPARAM)hMem, 0);
//
//
//AKD_GLOBALFREE
//______________
//
//Frees the specified global memory object (see description for GlobalFree in MSDN).
//
//wParam == handle to the global memory object
//lParam == not used
//
//Return Value
// free result
//
//Example:
// HGLOBAL hMem=SendMessage(pd->hMainWnd, AKD_GLOBALFREE, (WPARAM)hMem, 0);
//
//
//AKD_STRLENA
//___________
//
//Returns the length in bytes of the specified string (not including the terminating null character).
//
//(char *)wParam == ANSI string
//lParam         == not used
//
//Return Value
// string length
//
//Example:
// SendMessage(pd->hMainWnd, AKD_STRLENA, (WPARAM)"123", 0);
//
//
//AKD_STRLENW
//___________
//
//Returns the length in WCHAR's of the specified string (not including the terminating null character).
//
//(wchar_t *)wParam == Unicode string
//lParam            == not used
//
//Return Value
// string length
//
//Example:
// SendMessage(pd->hMainWnd, AKD_STRLENW, (WPARAM)L"123", 0);
//
//
//AKD_CREATEWINDOW
//________________
//
//Create window.
//
//wParam                 == not used
//(CREATEWINDOW *)lParam == pointer to a CREATEWINDOW structure
//                          (CREATEWINDOWA *)lParam   if bOldWindows == TRUE
//                          (CREATEWINDOWW *)lParam   if bOldWindows == FALSE
//
//Return Value
// window handle
//
//Example (bOldWindows == TRUE):
// CREATEWINDOWA cw;
// HWND hWndMemEdit;
//
// lstrcpynA(cw.szClassName, "RichEdit20A", MAX_PATH);
// cw.szWindowName[0]='\0';
// cw.dwStyle=WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_LEFT|ES_MULTILINE|ES_DISABLENOSCROLL|ES_SUNKEN|ES_NOHIDESEL;
// cw.x=0;
// cw.y=0;
// cw.nWidth=100;
// cw.nHeight=100;
// cw.hWndParent=pd->hMainWnd;
// cw.hMenu=(HMENU)0;
// cw.hInstance=pd->hInstanceEXE;
// cw.lpParam=NULL;
//
// hWndMemEdit=(HWND)SendMessage(pd->hMainWnd, AKD_CREATEWINDOW, 0, (LPARAM)&cw);
//
//Example (bOldWindows == FALSE):
// CREATEWINDOWW cw;
// HWND hWndMemEdit;
//
// lstrcpynW(cw.wszClassName, L"RichEdit20W", MAX_PATH);
// cw.wszWindowName[0]='\0';
// cw.dwStyle=WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_LEFT|ES_MULTILINE|ES_DISABLENOSCROLL|ES_SUNKEN|ES_NOHIDESEL;
// cw.x=0;
// cw.y=0;
// cw.nWidth=100;
// cw.nHeight=100;
// cw.hWndParent=pd->hMainWnd;
// cw.hMenu=(HMENU)0;
// cw.hInstance=pd->hInstanceEXE;
// cw.lpParam=NULL;
//
// hWndMemEdit=(HWND)SendMessage(pd->hMainWnd, AKD_CREATEWINDOW, 0, (LPARAM)&cw);
//
//
//AKD_WAITKEYBOARD
//________________
//
//Wait for release all virtual keys.
//
//(BOOL)wParam == TRUE  test is any key pressed
//                FALSE wait for keys release
//lParam       == not used
//
//Return Value
// if wParam == FALSE the return value:
//   zero
// if wParam == TRUE the return value:
//   TRUE  keyboard is free
//   FALSE keyboard is busy
//
//Example:
// SendMessage(pd->hMainWnd, AKD_WAITKEYBOARD, 0, 0);
//
//
//AKD_GETQUEUE
//____________
//
//Indicates the type of messages found in the main thread's message queue
//(see description for GetQueueStatus in MSDN).
//
//wParam == specifies the types of messages for which to check
//lParam == not used
//
//Return Value
//  number of messages
//
//Example:
// DWORD dwStatus=SendMessage(pd->hMainWnd, AKD_GETQUEUE, QS_ALLEVENTS, 0);
//
//
//AKD_POSTMESSAGE
//_______________
//
//Post message.
//
//wParam                == not used
//(POSTMESSAGE *)lParam == pointer to a POSTMESSAGE, allocated with GlobalAlloc
//
//Return Value
//  zero
//
//
//Example (bOldWindows == TRUE):
// typedef struct _PMSAVEDOCUMENTA {
//   POSTMESSAGE pm;
//   SAVEDOCUMENTA sd;
// } PMSAVEDOCUMENTA;
//
// PMSAVEDOCUMENTA *pmsd;
//
// if (pmsd=(PMSAVEDOCUMENTA *)GlobalAlloc(GPTR, sizeof(PMSAVEDOCUMENTA)))
// {
//   lstrcpynA(pmsd->sd.szFile, "C:\\MyFile.txt", MAX_PATH);
//   pmsd->sd.nCodePage=65001;
//   pmsd->sd.bBOM=TRUE;
//   pmsd->sd.bUpdate=TRUE;
//
//   //Post message
//   pmsd->pm.hWnd=pd->hMainWnd;
//   pmsd->pm.uMsg=AKD_SAVEDOCUMENT;
//   pmsd->pm.wParam=(WPARAM)pd->hWndEdit;
//   pmsd->pm.lParam=(LPARAM)&pmsd->sd;
//   PostMessage(pd->hMainWnd, AKD_POSTMESSAGE, 0, (LPARAM)pmsd);
// }
//
//Example (bOldWindows == FALSE):
// typedef struct _PMSAVEDOCUMENTW {
//   POSTMESSAGE pm;
//   SAVEDOCUMENTW sd;
// } PMSAVEDOCUMENTW;
//
// PMSAVEDOCUMENTW *pmsd;
//
// if (pmsd=(PMSAVEDOCUMENTW *)GlobalAlloc(GPTR, sizeof(PMSAVEDOCUMENTW)))
// {
//   lstrcpynW(pmsd->sd.wszFile, L"C:\\MyFile.txt", MAX_PATH);
//   pmsd->sd.nCodePage=65001;
//   pmsd->sd.bBOM=TRUE;
//   pmsd->sd.bUpdate=TRUE;
//
//   //Post message
//   pmsd->pm.hWnd=pd->hMainWnd;
//   pmsd->pm.uMsg=AKD_SAVEDOCUMENT;
//   pmsd->pm.wParam=(WPARAM)pd->hWndEdit;
//   pmsd->pm.lParam=(LPARAM)&pmsd->sd;
//   PostMessage(pd->hMainWnd, AKD_POSTMESSAGE, 0, (LPARAM)pmsd);
// }
//
//
//AKD_EXGETTEXTLENGTH
//___________________
//
//Get AkelEdit window text length.
//
//(HWND)wParam == AkelEdit window
//(int)lParam  == see AELB_* defines
//
//Return Value
// text length
//
//Example:
// int nLength=SendMessage(pd->hMainWnd, AKD_EXGETTEXTLENGTH, (WPARAM)pd->hWndEdit, AELB_ASOUTPUT);
//
//
//AKD_EXGETTEXTRANGEA
//___________________
//
//Retrieves a specified range of characters from a AkelEdit control.
//
//(HWND)wParam             == AkelEdit window
//(EXGETTEXTRANGE *)lParam == pointer to a EXGETTEXTRANGE structure
//
//Return Value
// Text length in TCHARs without null character
//
//Example:
// EXGETTEXTRANGE tr;
//
// SendMessage(pd->hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr);
// tr.pText=NULL;
// tr.nNewLine=AELB_ASIS;
// tr.nCodePage=CP_ACP;
// tr.lpDefaultChar=NULL;
// tr.lpUsedDefChar=NULL;
//
// if (SendMessage(pd->hMainWnd, AKD_EXGETTEXTRANGEA, (WPARAM)pd->hWndEdit, (LPARAM)&tr))
// {
//   MessageBoxA(pd->hMainWnd, (char *)tr.pText, "Test", MB_OK);
//   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
// }
//
//
//AKD_EXGETTEXTRANGEW
//___________________
//
//Retrieves a specified range of characters from a AkelEdit control.
//
//(HWND)wParam             == AkelEdit window
//(EXGETTEXTRANGE *)lParam == pointer to a EXGETTEXTRANGE structure
//
//Return Value
// Text length in TCHARs without null character
//
//Example:
// EXGETTEXTRANGE tr;
//
// SendMessage(pd->hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr);
// tr.pText=NULL;
// tr.nNewLine=AELB_ASIS;
//
// if (SendMessage(pd->hMainWnd, AKD_EXGETTEXTRANGEW, (WPARAM)pd->hWndEdit, (LPARAM)&tr))
// {
//   MessageBoxW(pd->hMainWnd, (wchar_t *)tr.pText, L"Test", MB_OK);
//   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
// }
//
//
//AKD_GETSTATUSPOSTYPE
//____________________
//
//Get status position type parameter.
//
//wParam == not used
//lParam == not used
//
//Return Value
// see SPT_* defines
//
//Example:
// DWORD dwStatusPosType=SendMessage(pd->hMainWnd, AKD_GETSTATUSPOSTYPE, 0, 0);
//
//*/
//
//
////// AkelPad WM_COPYDATA messages
//
//#define CD_OPENDOCUMENT   1
//
//
///*
//CD_OPENDOCUMENT
//_______________
//
//Open file.
//
//(DWORD)dwData          == CD_OPENDOCUMENT
//(DWORD)cbData          == sizeof(OPENDOCUMENTA)    if bOldWindows == TRUE
//                          sizeof(OPENDOCUMENTW)    if bOldWindows == FALSE
//(OPENDOCUMENT *)lpData == (OPENDOCUMENTA *)lpData  if bOldWindows == TRUE
//                          (OPENDOCUMENTW *)lpData  if bOldWindows == FALSE
//
//Return Value
// see EOD_* defines
//
//Example (bOldWindows == TRUE):
//  OPENDOCUMENTA od;
//  COPYDATASTRUCT cds;
//
//  od.hWnd=NULL;
//  lstrcpynA(od.szFile, "C:\\MyFile.txt", MAX_PATH);
//  od.szWorkDir[0]='\0';
//  od.dwFlags=OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE;
//  od.nCodePage=0;
//  od.bBOM=0;
//
//  cds.dwData=CD_OPENDOCUMENT;
//  cds.cbData=sizeof(OPENDOCUMENTA);
//  cds.lpData=&od;
//  SendMessage(pd->hMainWnd, WM_COPYDATA, (WPARAM)pd->hMainWnd, (LPARAM)&cds);
//
//Example (bOldWindows == FALSE):
//  OPENDOCUMENTW od;
//  COPYDATASTRUCT cds;
//
//  od.hWnd=NULL;
//  lstrcpynW(od.wszFile, L"C:\\MyFile.txt", MAX_PATH);
//  od.wszWorkDir[0]='\0';
//  od.dwFlags=OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE;
//  od.nCodePage=0;
//  od.bBOM=0;
//
//  cds.dwData=CD_OPENDOCUMENT;
//  cds.cbData=sizeof(OPENDOCUMENTW);
//  cds.lpData=&od;
//  SendMessage(pd->hMainWnd, WM_COPYDATA, (WPARAM)pd->hMainWnd, (LPARAM)&cds);


implementation

function PRIMARYLANGID(LangId: WORD): WORD;
begin
  Result:= LangId and $03FF;
end;

function MakeIdentifier(a, b, c, d: Byte): DWORD;
begin
  Result:= MakeLong(MakeWord(a, b), MakeWord(c, d));
end;

end.
