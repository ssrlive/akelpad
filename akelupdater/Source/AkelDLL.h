#ifndef __AKELDLL_H__
#define __AKELDLL_H__


//// Version

#ifndef MAKE_IDENTIFIER
  #define MAKE_IDENTIFIER(a, b, c, d)  ((DWORD)MAKELONG(MAKEWORD(a, b), MAKEWORD(c, d)))
#endif

#define AKELDLL MAKE_IDENTIFIER(1, 5, 0, 4)


//// Defines

//Control IDs
#define ID_EDIT           10001
#define ID_STATUS         10002
#define ID_TAB            10003
#define ID_PROGRESS       10004
#define ID_FIRSTMDI       10010

//Window mode
#define WMD_SDI   0  //Single document interface (SDI).
#define WMD_MDI   1  //Multiple document interface (MDI).
#define WMD_PMDI  2  //Pseudo-Multiple document interface (PMDI).

//Sizes
#define COMMANDLINE_SIZE        32768
#define COMMANDARG_SIZE         16384
#define WORD_DELIMITERS_SIZE    128
#define WRAP_DELIMITERS_SIZE    128
#define URL_PREFIXES_SIZE       128
#define URL_DELIMITERS_SIZE     128

//Unload plugin flag
#define UD_FAILED                -1  //Operation failed. Don't use it.
#define UD_UNLOAD                 0  //Unload plugin (default).
#define UD_NONUNLOAD_ACTIVE     0x1  //Don't unload plugin and set active status.
#define UD_NONUNLOAD_NONACTIVE  0x2  //Don't unload plugin and set non-active status.
#define UD_NONUNLOAD_UNCHANGE   0x4  //Don't unload plugin and don't change active status.
#define UD_HOTKEY_DODEFAULT     0x8  //Do default hotkey processing.

//Command line options
#define CLO_NONOTEPADCMD          0x01  //Don't use MS Notepad compatibility mode when parse command line parameters.
#define CLO_GLOBALPRINT           0x02  //Next opened file will be printed.
#define CLO_MSGOPENCREATEYES      0x04  //Silently create new file, if doesn't exist.
#define CLO_MSGOPENCREATENO       0x08  //Don't create new file, if doesn't exist.
#define CLO_MSGOPENBINARYYES      0x10  //Silently open file, if binary.
#define CLO_MSGOPENBINARYNO       0x20  //Don't open file, if binary.
#define CLO_MSGSAVELOSTSYMBOLSYES 0x40  //Silently save file, even if symbols will be lost.
#define CLO_MSGSAVELOSTSYMBOLSNO  0x80  //Don't save file, if symbols will be lost.

//AKD_PARSECMDLINE return value
#define PCLE_QUIT    0x01  //Stop parsing command line parameters and close program.
#define PCLE_END     0x02  //Stop parsing command line parameters.
#define PCLE_ONLOAD  0x04  //Done parsing command line parameters on program load (used internally).

//Autodetect flags
#define ADT_BINARY_ERROR        0x00000001  //Check if file is binary.
#define ADT_REG_CODEPAGE        0x00000002  //If last open code page found in registry, then it will be
                                            //used with ADT_DETECT_BOM flag. If not found, then next flags
                                            //will be used ADT_DETECT_CODEPAGE|ADT_DETECT_BOM.
#define ADT_DETECT_CODEPAGE     0x00000004  //Detect code page.
#define ADT_DETECT_BOM          0x00000008  //Detect BOM mark.
#define ADT_NOMESSAGES          0x00000010  //No messages, if errors in autodetect.

//Autodetect errors
#define EDT_SUCCESS        0   //Success.
#define EDT_OPEN          -1   //Can't open file.
#define EDT_ALLOC         -2   //Can't allocate buffer.
#define EDT_READ          -3   //Read file error.
#define EDT_BINARY        -4   //File is binary.

//Open document flags
#define OD_ADT_BINARY_ERROR      ADT_BINARY_ERROR     //See ADT_BINARY_ERROR.
#define OD_ADT_REG_CODEPAGE      ADT_REG_CODEPAGE     //See ADT_REG_CODEPAGE.
#define OD_ADT_DETECT_CODEPAGE   ADT_DETECT_CODEPAGE  //See ADT_DETECT_CODEPAGE.
#define OD_ADT_DETECT_BOM        ADT_DETECT_BOM       //See ADT_DETECT_BOM.
#define OD_ADT_NOMESSAGES        ADT_NOMESSAGES       //See ADT_NOMESSAGES.
#define OD_REOPEN                0x00000100           //Don't create new MDI window, use the exited one.

//Open document errors
#define EOD_SUCCESS              0          //Success.
#define EOD_ADT_OPEN             EDT_OPEN   //See EDT_OPEN.
#define EOD_ADT_ALLOC            EDT_ALLOC  //See EDT_ALLOC.
#define EOD_ADT_READ             EDT_READ   //See EDT_READ.
#define EOD_OPEN                 -11        //Can't open file.
#define EOD_WINDOW_EXIST         -13        //File already opened.
#define EOD_CODEPAGE_ERROR       -14        //Code page isn't implemented.
#define EOD_STOP                 -15        //Stopped from AKDN_OPENDOCUMENT_START.
#define EOD_STREAMIN             -16        //Error in EM_STREAMIN.
#define EOD_DOCUMENTS_LIMIT      -17        //Documents limit reached in MDI mode.
#define EOD_MSGNO                -20        //File is skipped.
#define EOD_MSGCANCEL            -21        //User press cancel.

//Save document flags
#define SD_UPDATE            0x00000001  //Update file info.
#define SD_SELECTION         0x00000002  //Save only selection.

//Save document errors
#define ESD_SUCCESS          0   //Success.
#define ESD_OPEN            -1  //Can't open file.
#define ESD_WRITE           -2  //Can't write to file.
#define ESD_READONLY        -3  //File has read-only attribute.
#define ESD_CODEPAGE_ERROR  -4  //Code page isn't implemented.
#define ESD_STOP            -5  //Stopped from AKDN_SAVEDOCUMENT_START.
#define ESD_STREAMOUT       -6  //Error in EM_STREAMOUT.

//Save settings
#define SS_REGISTRY   1  //Registry.
#define SS_INI        2  //INI file.

//AKD_RECENTFILES flags
#define RF_GET             1  //Retrive current recent files info.
                              //(RECENTFILESTACK **)lParam is a pointer to a variable that receive pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is maximum number of recent files.
#define RF_SET             2  //Set recent files number.
                              //(int)lParam is maximum number of recent files.
                              //Return value is zero.
#define RF_READ            3  //Read recent files from registry.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is number of records read.
#define RF_SAVE            4  //Save recent files to registry.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is zero.
#define RF_CLEAR           5  //Clear recent files stack. Use RF_SAVE to save result.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is zero.
#define RF_DELETEOLD       6  //Delete non-existent recent files records. Use RF_SAVE to save result.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is number of records deleted.
#define RF_FINDINDEX       7  //Find item index in recent files stack by file name.
                              //(wchar_t *)lParam is a pointer to a file name.
                              //Return value is item index in recent files stack, -1 if error.
#define RF_DELETEINDEX     8  //Delete item from recent files stack by index.
                              //(int)lParam is index in recent files array to delete.
                              //Return value is TRUE - success, FALSE - error.
#define RF_FINDITEMBYINDEX 9  //Find pointer to a RECENTFILE structure by index.
                              //(int)lParam is index of item in recent files stack.
                              //Return value is a pointer to a RECENTFILE structure.
#define RF_FINDITEMBYNAME  10 //Find pointer to a RECENTFILE structure by file name.
                              //(wchar_t *)lParam is a pointer to a file name.
                              //Return value is a pointer to a RECENTFILE structure.
#define RF_DELETEITEM      11 //Delete item from recent files stack.
                              //(RECENTFILE *)lParam a pointer to a RECENTFILE structure to delete.
                              //Return value is zero.
#define RF_GETPARAM        21 //Get recent file parameter by name.
                              //(RECENTFILEPARAM *)lParam a pointer to a RECENTFILEPARAM structure with filled "file" and "pParamName" members.
                              //Return value is a pointer to a real RECENTFILEPARAM structure or NULL if error.
#define RF_SETPARAM        22 //Set recent file parameter.
                              //(RECENTFILEPARAM *)lParam a pointer to a RECENTFILEPARAM structure with filled "file", "pParamName" and "pParamValue" members.
                              //Return value is a pointer to a real RECENTFILEPARAM structure or NULL if error.
#define RF_DELETEPARAM     23 //Delete recent file parameter.
                              //(RECENTFILEPARAM *)lParam a pointer to a real RECENTFILEPARAM structure.
                              //Return value is zero.

//AKD_SEARCHHISTORY flags
#define SH_GET    1  //Retrive search strings count.
#define SH_CLEAR  2  //Clear search history.

//AKD_SETEDITOPTION flags
#define EO_TEXTMARGINS 1  //The low-order word specifies the new width of the left margin, in pixels. The high-order word specifies the new width of the right margin, in pixels.
#define EO_TABSIZE     2  //Tabulation size.
#define EO_UNDOLIMIT   3  //Undo limit.
#define EO_WRAPLIMIT   4  //Wrap limit.
#define EO_MARKERPOS   5  //Column marker position.
#define EO_CARETWIDTH  6  //Caret width.
#define EO_LINEGAP     7  //Gap between lines.

//New line format
#define NEWLINE_WIN   1  //Windows/DOS new line format (\r\n).
#define NEWLINE_UNIX  2  //Unix new line format (\n).
#define NEWLINE_MAC   3  //MacOS new line format (\r).

//Status bar position type
#define SPT_COLUMN      0x00000001  //"Line:Column". By default: "Line:Symbol".
#define SPT_LINEWRAP    0x00000002  //Wrap line numbers. By default: Non-wrap line numbers.

//AKD_GOTO type
#define GT_LINE    0x1
#define GT_OFFSET  0x2

//Caret options
#define CO_CARETOUTEDGE  0x00000001  //Allow caret moving out of the line edge.
#define CO_CARETVERTLINE 0x00000002  //Draw caret vertical line.

//Mouse options
#define MO_LEFTMARGINSELECTION   0x00000001  //Enables left margin line selection with mouse.
#define MO_RICHEDITMOUSE         0x00000002  //After WM_LBUTTONUP message capture operations doesn't stopped.
#define MO_MOUSEDRAGGING         0x00000004  //Enables OLE text dragging.
#define MO_RCLICKMOVECARET       0x00000008  //WM_RBUTTONDOWN message moves caret to a click position.
#define MO_NONEWLINEMOUSESELECT  0x00000010  //Triple click and left margin click selects only line contents without new line.

//Keyboard layout options
#define MO_REMEMBERLAYOUT     0x00000001  //Remember keyboard layout for each tab (MDI).
#define MO_SWITCHLAYOUT       0x00000002  //Auto switch keyboard layout according to text under caret.

//Paint options
#define PAINT_PAINTGROUP            0x00000001  //Paint text by group of characters (default is character by character).
                                                //With this flag some text recognition programs could start to work, printer could print faster, but highlighted symbols and combined unicode symbols can be drawn differently and editing of whose characters may become uncomfortable.
#define PAINT_NONEWLINEDRAW         0x00000002  //Disables drawing new line selection as space character.
#define PAINT_ENTIRENEWLINEDRAW     0x00000004  //Draw new line selection to the right edge.
#define PAINT_HIDESEL               0x00000008  //Hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus.
#define PAINT_NOMARKERAFTERLASTLINE 0x00000010  //Disables marker painting after last line.
#define PAINT_HIDENOSCROLL          0x00000100  //Hides scroll bars instead of disabling them when they are not needed.
#define PAINT_STATICEDGE            0x00000200  //Draw thin edit window border.
#define PAINT_NOEDGE                0x00000400  //Draw no edit window border.

//Tab options MDI
#define TAB_VIEW_NONE         0x00000001
#define TAB_VIEW_TOP          0x00000002
#define TAB_VIEW_BOTTOM       0x00000004
#define TAB_TYPE_STANDARD     0x00000100
#define TAB_TYPE_BUTTONS      0x00000200
#define TAB_TYPE_FLATBUTTONS  0x00000400
#define TAB_SWITCH_NEXTPREV   0x00010000
#define TAB_SWITCH_RIGHTLEFT  0x00020000

//Status bar
#define STATUS_POSITION       0
#define STATUS_MODIFY         1
#define STATUS_INSERT         2
#define STATUS_NEWLINE        3
#define STATUS_CODEPAGE       4
#define STATUS_USER           5

//Main menu
#define MENU_FILE_POSITION     0
#define MENU_EDIT_POSITION     1
#define MENU_VIEW_POSITION     2
#define MENU_OPTIONS_POSITION  3
#define MENU_MDI_POSITION      4
#define MENU_ABOUT_POSITION    5

//Popup menu
#define MENU_POPUP_EDIT        0
#define MENU_POPUP_VIEW        1
#define MENU_POPUP_CODEPAGE    2
#define MENU_POPUP_HEADLINE    3

//Submenu position
#define MENU_FILE_RECENTFILES_3X   12
#define MENU_FILE_RECENTFILES_4X   14
#define MENU_VIEW_LANGUAGE_3X      11
#define MENU_VIEW_LANGUAGE_4X      9
#define MENU_POPUP_CODEPAGE_OPEN   0
#define MENU_POPUP_CODEPAGE_SAVE   2

//INI value types
#define INI_DWORD           1
#define INI_BINARY          2
#define INI_STRINGANSI      3
#define INI_STRINGUNICODE   4

//Options flags
#define POB_READ     0x01  //Begins read options.
#define POB_SAVE     0x02  //Begins save options.
#define POB_CLEAR    0x04  //Begins new save options (POB_SAVE|POB_CLEAR).
#define POB_SCRIPTS  0x10  //Begins Scripts plugin options.

//Option type
#define PO_DWORD     1   //32-bit number.
#define PO_BINARY    2   //Binary data in any form.
#define PO_STRING    3   //Null-terminated string.
#define PO_REMOVE    10  //Delete option.

//Support flags
#define PDS_SUPPORTALL   0x00000000  //Function support everything (default).
#define PDS_NOAUTOLOAD   0x00000001  //Function doesn't support autoload.
#define PDS_NOANSI       0x00000002  //Function doesn't support ansi API and can be executed only on unicode Windows (WinNT/2000/XP/2003/Vista/Seven).
#define PDS_NOUNICODE    0x00000004  //Function doesn't support unicode API.
#define PDS_NOSDI        0x00000008  //Function doesn't support SDI mode.
#define PDS_NOMDI        0x00000010  //Function doesn't support MDI mode.
#define PDS_NOPMDI       0x00000020  //Function doesn't support PMDI mode.
#define PDS_NORICHEDIT   0x00000040  //Reserved.
#define PDS_GETSUPPORT   0x10000000  //Flag is set if caller specified DLLCF_GETSUPPORT in AKD_DLLCALL and wants to get PDS_* flags without function execution.

//AKD_DLLCALL flags
#define DLLCF_ONPROGRAMLOAD   0x01  //Don't use it. For internal code only.
#define DLLCF_GETSUPPORT      0x02  //Get PDS_* flags without function execution.
#define DLLCF_SWITCHAUTOLOAD  0x04  //If function running after call then turn on autoload, if not then turn off autoload.
#define DLLCF_SAVENOW         0x08  //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_NOW after switching autoload flag.
#define DLLCF_SAVEONEXIT      0x10  //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_ONEXIT after switching autoload flag.

//AKD_DLLSAVE flags
#define DLLSF_NOW     0x1  //Save plugins stack immediately.
#define DLLSF_ONEXIT  0x2  //Save plugins stack on program exit.

//AKD_TRANSLATEMESSAGE return type
#define TMSG_GLOBAL       1  //Hotkey from global accelerator table (PLUGINDATA.hGlobalAccel) is translated.
#define TMSG_DIALOG       2  //Message from modeless (see AKD_SETMODELESS) or dockable dialog (see AKD_DOCK) is translated.
#define TMSG_PLUGIN       3  //Plugin message (see AKD_DLL*, AKD_CALLPROC, AKD_POSTMESSAGE) is translated.
#define TMSG_HOTKEY       4  //Plugin hotkey is translated.
#define TMSG_ACCELERATOR  5  //Hotkey from main accelerator table (PLUGINDATA.hMainAccel) is translated.
#define TMSG_DEFAULT      6  //Default message processing.

//Context menu owner
#define NCM_EDIT     1  //Edit control.
#define NCM_TAB      2  //Tab control.
#define NCM_STATUS   3  //Status bar control.

//AKD_FRAMEFIND flags
#define FWF_CURRENT        1  //Retrive current frame data pointer. lParam not used.
#define FWF_NEXT           2  //Retrive next frame data pointer. lParam is a frame data pointer.
#define FWF_PREV           3  //Retrive previous frame data pointer. lParam is a frame data pointer.
#define FWF_BYINDEX        4  //Retrive frame data by index. lParam is frame index. First frame has index 1.
#define FWF_BYFILENAME     5  //Retrive frame data by full file name. lParam is full file name string.
                              // For AKD_FRAMEFINDA string is ansi.
                              // For AKD_FRAMEFINDW string is unicode.
#define FWF_BYEDITWINDOW   6  //Retrive frame data by edit window handle. lParam is edit window handle.
#define FWF_BYEDITDOCUMENT 7  //Retrive frame data by edit document handle. lParam is edit document handle.
#define FWF_BYTABINDEX     8  //Retrive frame data by tab item index. lParam is tab item index.

//AKD_FRAMEACTIVATE and AKDN_FRAME_ACTIVATE flags
#define FWA_NOUPDATEORDER         0x00000001  //Don't update access order during activating.
#define FWA_NOUPDATEEDIT          0x00000002  //For WMD_PMDI mode. Don't redraw edit control area during activating.
#define FWA_NOVISUPDATE           0x00000004  //For WMD_PMDI mode. Don't make any visual updates during activating. Use it only if you later will activate back frame that has lost active status.
                                              //AKDN_FRAME_ACTIVATE only flags:
#define FWA_NOTIFY_CREATE         0x00000100  //Frame activating after creation.
#define FWA_NOTIFY_BEFOREDESTROY  0x00000200  //Frame activating for destroying.
#define FWA_NOTIFY_AFTERDESTROY   0x00000400  //Previous frame activating because current one has been destroyed.
#define FWA_NOTIFY_REOPEN         0x00000800  //Frame activating before reopening document.

//AKD_FRAMEDESTROY return value
#define FWDE_SUCCESS   0  //Operation is successful.
#define FWDE_ABORT     1  //Operation is aborted by user.
#define FWDE_LASTTAB   2  //For WMD_PMDI mode. Last tab cannot be destroyed.
#define FWDE_NOWINDOW  3  //Frame doesn't have associated edit window (FRAMEDATA.ei.hWndEdit == NULL).

//AKD_FRAMESTATS flags
#define FWS_COUNTALL      0  //Count of windows.
#define FWS_COUNTMODIFIED 1  //Count of modified windows.
#define FWS_COUNTSAVED    2  //Count of unmodified windows.
#define FWS_CURSEL        3  //Active window zero based index.

//Lock inherit new document settings from current document
#define LI_FONT           0x00000001  //Lock inherit font.
#define LI_COLORS         0x00000002  //Lock inherit colors.
#define LI_WRAP           0x00000004  //Lock inherit wrapping.

//Find text flags
#ifndef FR_DOWN
  #define FR_DOWN        0x00000001  //Find down.
#endif                               //
#ifndef FR_MATCHCASE                 //
  #define FR_MATCHCASE   0x00000004  //Search is case-sensitive.
#endif                               //
#define FR_UP            0x00100000  //Find up.
#define FR_BEGINNING     0x00200000  //Search from beginning (usage: FR_DOWN|FR_BEGINNING).
#define FR_SELECTION     0x00400000  //Search in selection (usage: FR_DOWN|FR_SELECTION).
#define FR_ESCAPESEQ     0x00800000  //Search with escape sequences.
#define FR_ALLFILES      0x01000000  //Search in all openned MDI documents (usage: FR_DOWN|FR_BEGINNING|FR_ALLFILES).
#define FR_CYCLESEARCH   0x08000000  //Cycle search.

//AKD_RECODESEL flags
#define RCS_DETECTONLY   0x00000001  //Don't do text replacement, only detect codepages.

//AKD_PASTE
#define PASTE_ANSI       0x00000001  //Paste text as ANSI. Default is paste as Unicode text, if no Unicode text available ANSI text will be used.
#define PASTE_COLUMN     0x00000002  //Paste to column selection.
#define PASTE_AFTER      0x00001000  //Paste text after caret.
#define PASTE_SINGLELINE 0x00002000  //Paste multiline text to single line edit control. All new lines replaced with '\r'.

//AKD_GETMODELESS types
#define MLT_NONE     0 //No registered modeless dialog open.
#define MLT_CUSTOM   1 //Dialog registered with AKD_SETMODELESS.
#define MLT_RECODE   2 //Recode dialog.
#define MLT_FIND     3 //Find dialog.
#define MLT_REPLACE  4 //Replace dialog.
#define MLT_GOTO     5 //Go to dialog.

//DIALOGRESIZEMSG flags
#define DRM_PAINTSIZEGRIP 0x2 //Draw resize grid.

//DIALOGRESIZE type
#define DRS_SIZE  0x1 //Resize control. Can be combined with DRS_X ot DRS_Y.
#define DRS_MOVE  0x2 //Move control. Can be combined with DRS_X ot DRS_Y.
#define DRS_X     0x4 //X value. Can be combined with DRS_SIZE ot DRS_MOVE.
#define DRS_Y     0x8 //Y value. Can be combined with DRS_SIZE ot DRS_MOVE.

//Dock side
#define DKS_LEFT    1
#define DKS_RIGHT   2
#define DKS_TOP     3
#define DKS_BOTTOM  4

//Dock flags
#define DKF_OWNTHREAD     0x00000001  //Dock window has its own thread.
#define DKF_FIXEDSIZE     0x00000002  //Dock window isn't resizeable.
#define DKF_DRAGDROP      0x00000004  //Dock window has drag-and-drop support, the DOCK.rcDragDrop member is valid.
#define DKF_HIDDEN        0x00000008  //Dock window isn't visible.
#define DKF_NODROPLEFT    0x00000010  //Disable drag-and-drop to the left side.
#define DKF_NODROPRIGHT   0x00000020  //Disable drag-and-drop to the right side.
#define DKF_NODROPTOP     0x00000040  //Disable drag-and-drop to the top side.
#define DKF_NODROPBOTTOM  0x00000080  //Disable drag-and-drop to the bottom side.

//Dock action
#define DK_ADD         0x00000001  //Add new dockable window.
#define DK_DELETE      0x00000002  //Delete dockable window.
#define DK_SUBCLASS    0x00000004  //Assign dockable window handle (DOCK.hWnd).
#define DK_UNSUBCLASS  0x00000008  //Deassign dockable window handle (DOCK.hWnd).
#define DK_SETLEFT     0x00000010  //Set DKS_LEFT side.
#define DK_SETRIGHT    0x00000020  //Set DKS_RIGHT side.
#define DK_SETTOP      0x00000040  //Set DKS_TOP side.
#define DK_SETBOTTOM   0x00000080  //Set DKS_BOTTOM side.
#define DK_HIDE        0x00000100  //Hide dockable window and set DKF_HIDDEN flag.
#define DK_SHOW        0x00000200  //Show dockable window and remove DKF_HIDDEN flag.
#define DK_FINDDOCK    0x00000400  //Find dock by dockable window handle (DOCK.hWnd). Cannot be combined with DK_FINDCHILD.
#define DK_FINDCHILD   0x00000800  //Find dock by dockable window or its child handle (DOCK.hWnd). Cannot be combined with DK_FINDDOCK.

//Dock capture
#define DKC_SIZING     1
#define DKC_DRAGDROP   2

//AKD_SETBUTTONDRAW flags
#define BIF_BITMAP      0x001 //Bitmap handle is used in BUTTONDRAW.hImage.
#define BIF_ICON        0x002 //Icon handle is used in BUTTONDRAW.hImage.
#define BIF_CROSS       0x004 //Draw small cross 8x7. BUTTONDRAW.hImage is ignored.
#define BIF_DOWNARROW   0x008 //Draw small down arrow 7x4. BUTTONDRAW.hImage is ignored.
#define BIF_UPARROW     0x010 //Draw small up arrow 7x4. BUTTONDRAW.hImage is ignored.
#define BIF_LEFTARROW   0x020 //Draw small left arrow 4x7. BUTTONDRAW.hImage is ignored.
#define BIF_RIGHTARROW  0x040 //Draw small right arrow 4x7. BUTTONDRAW.hImage is ignored.
#define BIF_ETCHED      0x100 //Draw edge around button.
#define BIF_ENABLEFOCUS 0x200 //Draw focus rectangle when button receive focus.

//WM_INITMENU lParam
#define IMENU_EDIT     0x00000001
#define IMENU_CHECKS   0x00000004

//GetWindowLongPtr/SetWindowLongPtr
#ifndef GWLP_WNDPROC
  #define DWLP_MSGRESULT 0
#endif
#ifndef GWLP_WNDPROC
  #define GWLP_WNDPROC (-4)
#endif
#ifndef GWLP_HINSTANCE
  #define GWLP_HINSTANCE (-6)
#endif
#ifndef GWLP_HWNDPARENT
  #define GWLP_HWNDPARENT (-8)
#endif
#ifndef GWLP_ID
  #define GWLP_ID (-12)
#endif
#ifndef GWLP_USERDATA
  #define GWLP_USERDATA (-21)
#endif


//// Structures

#ifndef _HSTACK_STRUCT_
#define _HSTACK_STRUCT_
typedef struct {
  INT_PTR first;
  INT_PTR last;
} HSTACK;
#endif

#ifndef __AKELEDIT_H__
  typedef struct _AELINEDATA {
    struct _AELINEDATA *next;
    struct _AELINEDATA *prev;
    wchar_t *wpLine;
    int nLineLen;
    int nLineBreak;
    int nLineWidth;
    int nSelStart;
    int nSelEnd;
  } AELINEDATA;

  typedef struct _AELINEINDEX {
    int nLine;
    AELINEDATA *lpLine;
  } AELINEINDEX;

  typedef struct _AECHARINDEX {
    int nLine;
    AELINEDATA *lpLine;
    int nCharInLine;
  } AECHARINDEX;

  typedef struct {
    AECHARINDEX ciMin;
    AECHARINDEX ciMax;
  } AECHARRANGE;

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

  DECLARE_HANDLE (AEHDOC);

  typedef LRESULT (CALLBACK *AEEditProc)(AEHDOC hDoc, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

DECLARE_HANDLE (HINIFILE);
DECLARE_HANDLE (HINISECTION);
DECLARE_HANDLE (HINIKEY);

typedef BOOL (CALLBACK *PLUGINPROC)(void *);
typedef void (CALLBACK *WNDPROCRET)(CWPRETSTRUCT *);

typedef struct {
  DWORD cb;                   //Size of the structure.
  HWND hMainWnd;              //Main window.
  DWORD dwAkelDllVersion;     //Current AkelDLL version. Set it to AKELDLL.
  DWORD dwExeMinVersion3x;    //Required minimum AkelPad 3.x version.
                              //Set as MAKE_IDENTIFIER(x, x, x, x) or.
                              //If not supported MAKE_IDENTIFIER(-1, -1, -1, -1).
  DWORD dwExeMinVersion4x;    //Required minimum AkelPad 4.x version.
                              //Set as MAKE_IDENTIFIER(x, x, x, x) or.
                              //If not supported MAKE_IDENTIFIER(-1, -1, -1, -1).
  const char *pPluginName;    //Plugin unique name.
} PLUGINVERSION;

typedef struct _PLUGINFUNCTION {
  struct _PLUGINFUNCTION *next;
  struct _PLUGINFUNCTION *prev;
  const BYTE *pFunction;          //Function name, format "Plugin::Function".
                                  //  const char *pFunction      if bOldWindows == TRUE
                                  //  const wchar_t *pFunction   if bOldWindows == FALSE
  char szFunction[MAX_PATH];      //Function name (Ansi).
  wchar_t wszFunction[MAX_PATH];  //Function name (Unicode).
  int nFunctionLen;               //Function name length.
  WORD wHotkey;                   //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
  BOOL bAutoLoad;                 //TRUE  if function has autoload flag.
                                  //FALSE if function has no autoload flag.
  BOOL bRunning;                  //Function is running.
  PLUGINPROC PluginProc;          //Function procedure.
  void *lpParameter;              //Procedure parameter.
} PLUGINFUNCTION;

typedef struct _PLUGINDATA {
  DWORD cb;                         //Size of the structure.
  DWORD dwSupport;                  //If (dwSupport & PDS_GETSUPPORT) != 0, then caller wants to get PDS_* flags without function execution.
  const BYTE *pFunction;            //Called function name, format "Plugin::Function".
                                    //  const char *pFunction     if bOldWindows == TRUE
                                    //  const wchar_t *pFunction  if bOldWindows == FALSE
  const char *szFunction;           //Called function name (Ansi).
  const wchar_t *wszFunction;       //Called function name (Unicode).
  HINSTANCE hInstanceDLL;           //DLL instance.
  PLUGINFUNCTION *lpPluginFunction; //Pointer to a PLUGINFUNCTION structure.
  int nUnload;                      //See UD_* defines.
  BOOL bInMemory;                   //Plugin already loaded.
  BOOL bOnStart;                    //Indicates when function has been called:
                                    //  TRUE  if function called on start-up.
                                    //  FALSE if function called manually.
  LPARAM lParam;                    //Input data.
  const BYTE *pAkelDir;             //AkelPad directory.
                                    //  const char *pAkelDir      if bOldWindows == TRUE
                                    //  const wchar_t *pAkelDir   if bOldWindows == FALSE
  const char *szAkelDir;            //AkelPad directory (Ansi).
  const wchar_t *wszAkelDir;        //AkelPad directory (Unicode).
  HINSTANCE hInstanceEXE;           //EXE instance.
  HSTACK *hPluginsStack;            //Pointer to a plugins stack with PLUGINFUNCTION elements.
  HWND hMainWnd;                    //Main window.
  HWND hWndEdit;                    //Edit window.
  AEHDOC hDocEdit;                  //Edit document.
  HWND hStatus;                     //StatusBar window.
  HWND hMdiClient;                  //MDI client window (if nMDI == WMD_MDI).
  HWND hTab;                        //Tab window        (if nMDI == WMD_MDI || nMDI == WMD_PMDI).
  HMENU hMainMenu;                  //Main menu.
  HMENU hMenuRecentFiles;           //Recent files menu.
  HMENU hMenuLanguage;              //Language menu.
  HMENU hPopupMenu;                 //Right click menu.
  HICON hMainIcon;                  //Main window icon handle.
  HACCEL hGlobalAccel;              //Global accelerator table (highest priority).
  HACCEL hMainAccel;                //Main accelerator table (lowest priority).
  BOOL bOldWindows;                 //Non-Unicode Windows.
  BOOL bOldRichEdit;                //Riched20.dll lower then 5.30 (v3.0).
  BOOL bOldComctl32;                //Comctl32.dll lower then 4.71.
  BOOL bAkelEdit;                   //AkelEdit control is used.
  int nMDI;                         //Window mode, see WMD_* defines.
  int nSaveSettings;                //See SS_* defines.
  const BYTE *pLangModule;          //Language module.
                                    //  const char *pLangModule      if bOldWindows == TRUE
                                    //  const wchar_t *pLangModule   if bOldWindows == FALSE
  const char *szLangModule;         //Language module (Ansi).
  const wchar_t *wszLangModule;     //Language module (Unicode).
  LANGID wLangSystem;               //System language ID.
  LANGID wLangModule;               //Language module language ID.
} PLUGINDATA;

typedef struct {
  const BYTE *pString;        //Depend on Windows. Win9x or WinNT.
                              //  const char *pString     if bOldWindows == TRUE
                              //  const wchar_t *pString  if bOldWindows == FALSE
  const char *szString;       //Ansi string.
  const wchar_t *wszString;   //Unicode string.
} UNISTRING;

typedef struct {
  const char *pFile;     //File to detect.
  DWORD dwBytesToCheck;  //How many bytes will be checked.
  DWORD dwFlags;         //See ADT_* defines.
  int nCodePage;         //Detected codepage.
  BOOL bBOM;             //Detected BOM.
} DETECTCODEPAGEA;

typedef struct {
  const wchar_t *pFile;  //File to detect.
  DWORD dwBytesToCheck;  //How many bytes will be checked.
  DWORD dwFlags;         //See ADT_* defines.
  int nCodePage;         //Detected codepage.
  BOOL bBOM;             //Detected BOM.
} DETECTCODEPAGEW;

typedef struct {
  HANDLE hFile;          //File handle, returned by CreateFile function.
  UINT_PTR dwBytesMax;   //Maximum bytes to read, if -1 read entire file.
  int nCodePage;         //File codepage.
  BOOL bBOM;             //File BOM.
  wchar_t *wpContent;    //Returned file contents.
} FILECONTENT;

typedef struct {
  const char *pFile;        //File to open.
  const char *pWorkDir;     //Set working directory before open, if NULL then don't set.
  DWORD dwFlags;            //Open flags. See OD_* defines.
  int nCodePage;            //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
  BOOL bBOM;                //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
} OPENDOCUMENTA;

typedef struct {
  const wchar_t *pFile;     //File to open.
  const wchar_t *pWorkDir;  //Set working directory before open, if NULL then don't set.
  DWORD dwFlags;            //Open flags. See OD_* defines.
  int nCodePage;            //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
  BOOL bBOM;                //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
} OPENDOCUMENTW;

typedef struct {
  HWND hWnd;                 //Window to fill in, NULL for current edit window.
  DWORD dwFlags;             //Open flags. See OD_* defines.
  int nCodePage;             //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
  BOOL bBOM;                 //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
  char szFile[MAX_PATH];     //File to open.
  char szWorkDir[MAX_PATH];  //Set working directory before open, if (!*szWorkDir) then don't set.
} OPENDOCUMENTPOSTA;

typedef struct {
  HWND hWnd;                    //Window to fill in, NULL for current edit window.
  DWORD dwFlags;                //Open flags. See OD_* defines.
  int nCodePage;                //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
  BOOL bBOM;                    //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
  wchar_t szFile[MAX_PATH];     //File to open.
  wchar_t szWorkDir[MAX_PATH];  //Set working directory before open, if (!*szWorkDir) then don't set.
} OPENDOCUMENTPOSTW;

typedef struct {
  const char *pFile;     //File to save.
  int nCodePage;         //File code page.
  BOOL bBOM;             //File BOM.
  DWORD dwFlags;         //See SD_* defines.
} SAVEDOCUMENTA;

typedef struct {
  const wchar_t *pFile;  //File to save.
  int nCodePage;         //File code page.
  BOOL bBOM;             //File BOM.
  DWORD dwFlags;         //See SD_* defines.
} SAVEDOCUMENTW;

typedef struct {
  HWND hWndEdit;           //Edit window.
  AEHDOC hDocEdit;         //Edit document (4.x only).
  const BYTE *pFile;       //Current editing file.
                           //  const char *pFile         if bOldWindows == TRUE
                           //  const wchar_t *pFile      if bOldWindows == FALSE
  const char *szFile;      //Current editing file (Ansi).
  const wchar_t *wszFile;  //Current editing file (Unicode).
  int nCodePage;           //Current code page.
  BOOL bBOM;               //Current BOM.
  int nNewLine;            //Current new line format, see NEWLINE_* defines.
  BOOL bModified;          //File has been modified.
  BOOL bReadOnly;          //Read only.
  BOOL bWordWrap;          //Word wrap.
  BOOL bOvertypeMode;      //Overtype mode.
  HWND hWndMaster;         //Master window (4.x only).
  AEHDOC hDocMaster;       //Master document (4.x only).
  HWND hWndClone1;         //First clone window (4.x only).
  AEHDOC hDocClone1;       //First clone document (4.x only).
  HWND hWndClone2;         //Second clone window (4.x only).
  AEHDOC hDocClone2;       //Second clone document (4.x only).
  HWND hWndClone3;         //Third clone window (4.x only).
  AEHDOC hDocClone3;       //Third clone document (4.x only).
} EDITINFO;

typedef struct _FRAMEDATA {
  struct _FRAMEDATA *next;
  struct _FRAMEDATA *prev;

  //Edit state external
  HWND hWndEditParent;                                //Edit parent window.
  EDITINFO ei;                                        //Edit info.
  char szFile[MAX_PATH];                              //Frame file (Ansi).
  wchar_t wszFile[MAX_PATH];                          //Frame file (Unicode).
  int nFileLen;                                       //Frame file length.
  HICON hIcon;                                        //Frame icon.
  int nIconIndex;                                     //Frame ImageList icon index.
  RECT rcEditWindow;                                  //Edit RECT. rcEditWindow.right - is width and rcEditWindow.bottom is height.
  RECT rcMasterWindow;                                //Master window RECT (4.x only). rcMasterWindow.right - is width and rcMasterWindow.bottom is height.

  //Edit settings
  LOGFONTW lf;                                        //Edit font.
  AECOLORS aec;                                       //Edit colors.
  DWORD dwEditMargins;                                //Edit margins.
  int nTabStopSize;                                   //Tab stop size.
  BOOL bTabStopAsSpaces;                              //Insert tab stop as spaces.
  int nUndoLimit;                                     //Undo limit.
  BOOL bDetailedUndo;                                 //Detailed undo.
  DWORD dwWrapType;                                   //Wrap type AEWW_WORD or AEWW_SYMBOL (4.x only).
  DWORD dwWrapLimit;                                  //Wrap characters limit, zero if wrap by window edge (4.x only).
  DWORD dwMarker;                                     //Vertical marker, zero if no marker set (4.x only).
  DWORD dwMappedPrintWidth;                           //Mapped print page width (4.x only).
  DWORD dwCaretOptions;                               //See CO_* defines (4.x only).
  int nCaretWidth;                                    //Caret width (4.x only).
  DWORD dwMouseOptions;                               //See MO_* defines (4.x only).
  DWORD dwLineGap;                                    //Line gap (4.x only).
  BOOL bShowURL;                                      //Show URL.
  int nClickURL;                                      //Number of clicks to open URL.
  BOOL bUrlPrefixesEnable;                            //URL prefixes enable (4.x only).
  BOOL bUrlDelimitersEnable;                          //URL delimiters enable (4.x only).
  BOOL bWordDelimitersEnable;                         //Word delimiters enabled.
  BOOL bWrapDelimitersEnable;                         //Wrap delimiters enabled (4.x only).
  wchar_t wszUrlPrefixes[URL_PREFIXES_SIZE];          //URL prefixes (4.x only).
  wchar_t wszUrlLeftDelimiters[URL_DELIMITERS_SIZE];  //URL left delimiters (4.x only).
  wchar_t wszUrlRightDelimiters[URL_DELIMITERS_SIZE]; //URL right delimiters (4.x only).
  wchar_t wszWordDelimiters[WORD_DELIMITERS_SIZE];    //Word delimiters.
  wchar_t wszWrapDelimiters[WRAP_DELIMITERS_SIZE];    //Wrap delimiters (4.x only).

  //Edit state internal
  AEEditProc lpEditProc;                              //Edit window procedure (4.x only).
  FILETIME ft;                                        //File time.
  HKL dwInputLocale;                                  //Keyboard layout (4.x only).
  DWORD dwLockInherit;                                //See LI_* defines.

  //Substract selection
  AECHARRANGE crPrevSel;
  INT_PTR nSelSubtract;

  //"StatusUserFormat" variables.
  INT_PTR nCaretRichOffset;
  INT_PTR nCaretByteOffset;
  int nCaretChar;
  int nLineCount;
  INT_PTR nRichCount;
  int nFontPoint;
  BOOL bReachedEOF;
  INT_PTR nReplaceCount;
} FRAMEDATA;

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

typedef struct {
  const char *pFunction;      //Function name, format "Plugin::Function".
  WORD wHotkey;               //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
  BOOL bAutoLoad;             //TRUE  if function has autoload flag.
                              //FALSE if function has no autoload flag.
  PLUGINPROC PluginProc;      //Function procedure.
  void *lpParameter;          //Procedure parameter.
} PLUGINADDA;

typedef struct {
  const wchar_t *pFunction;   //Function name, format "Plugin::Function".
  WORD wHotkey;               //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
  BOOL bAutoLoad;             //TRUE  if function has autoload flag.
                              //FALSE if function has no autoload flag.
  PLUGINPROC PluginProc;      //Function procedure.
  void *lpParameter;          //Procedure parameter.
} PLUGINADDW;

typedef struct {
  const char *pFunction;      //Function name, format "Plugin::Function".
  LPARAM lParam;              //Input data.
  DWORD dwSupport;            //Receives PDS_* flags.
} PLUGINCALLSENDA;

typedef struct {
  const wchar_t *pFunction;   //Function name, format L"Plugin::Function".
  LPARAM lParam;              //Input data.
  DWORD dwSupport;            //Receives PDS_* flags.
} PLUGINCALLSENDW;

typedef struct {
  LPARAM lParam;              //Input data.
  char szFunction[MAX_PATH];  //Function name, format "Plugin::Function".
} PLUGINCALLPOSTA;

typedef struct {
  LPARAM lParam;                  //Input data.
  wchar_t szFunction[MAX_PATH];   //Function name, format L"Plugin::Function".
} PLUGINCALLPOSTW;

typedef struct {
  const char *pOptionName;       //Option name.
  DWORD dwType;                  //Data type: see PO_* defines.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} PLUGINOPTIONA;

typedef struct {
  const wchar_t *pOptionName;    //Option name.
  DWORD dwType;                  //Data type: see PO_* defines.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} PLUGINOPTIONW;

typedef struct {
  const char *pSection;          //Section name.
  const char *pKey;              //Key name.
  DWORD dwType;                  //Data type: see INI_* defines.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} INIVALUEA;

typedef struct {
  const wchar_t *pSection;       //Section name.
  const wchar_t *pKey;           //Key name.
  DWORD dwType;                  //Data type: see INI_* defines.
  BYTE *lpData;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
  DWORD dwData;                  //Data size in bytes.
} INIVALUEW;

typedef struct {
  INT_PTR cpMin;              //First character in the range. First char of text: 0.
  INT_PTR cpMax;              //Last character in the range. Last char of text: -1.
  unsigned char *pText;       //Pointer that receive allocated text, new lines are forced to "\r". Must be deallocated with AKD_FREETEXT message.
                              //  char *pText      if bOldWindows == TRUE
                              //  wchar_t *pText   if bOldWindows == FALSE
} GETTEXTRANGE;

#ifdef __AKELEDIT_H__
typedef struct {
  AECHARRANGE cr;             //Characters range to retrieve.
  BOOL bColumnSel;            //Column selection. If this value is –1, active column selection mode is used.
  unsigned char *pText;       //Pointer that receive allocated text.
                              //Must be deallocated with AKD_FREETEXT message.
                              //  char *pText      if bOldWindows == TRUE
                              //  wchar_t *pText   if bOldWindows == FALSE
  int nNewLine;               //See AELB_* defines.
  int nCodePage;              //Valid if bOldWindows == TRUE. Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  const char *lpDefaultChar;  //Valid if bOldWindows == TRUE. Points to the character used if a wide character cannot be represented in the specified code page. If this member is NULL, a system default value is used.
  BOOL *lpUsedDefChar;        //Valid if bOldWindows == TRUE. Points to a flag that indicates whether a default character was used. The flag is set to TRUE if one or more wide characters in the source string cannot be represented in the specified code page. Otherwise, the flag is set to FALSE. This member may be NULL.
} EXGETTEXTRANGE;
#endif

typedef struct _RECENTFILE {
  struct _RECENTFILE *next;
  struct _RECENTFILE *prev;
  wchar_t wszFile[MAX_PATH];  //Recent file name.
  int nFileLen;               //Recent file name length.
  int nCodePage;              //Recent file codepages.
  INT_PTR cpMin;              //First character in selection range.
  INT_PTR cpMax;              //Last character in selection range.
  HSTACK lpParamsStack;       //Additional parameters storage (RECENTFILEPARAMSTACK structure).
} RECENTFILE;

typedef struct {
  RECENTFILE *first;          //Pointer to the first RECENTFILE structure.
  RECENTFILE *last;           //Pointer to the last RECENTFILE structure.
  int nElements;              //Items in stack.
  DWORD dwSaveTime;           //GetTickCount() for the last recent files save operation.
} RECENTFILESTACK;

typedef struct _RECENTFILEPARAM {
  struct _RECENTFILEPARAM *next;
  struct _RECENTFILEPARAM *prev;
  RECENTFILE *file;
  wchar_t *pParamName;
  wchar_t *pParamValue;
} RECENTFILEPARAM;

typedef struct {
  RECENTFILEPARAM *first;
  RECENTFILEPARAM *last;
} RECENTFILEPARAMSTACK;

typedef struct {
  DWORD dwFlags;            //See FR_* defines.
  const char *pFindIt;      //Find string.
  int nFindItLen;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically (4.x only).
} TEXTFINDA;

typedef struct {
  DWORD dwFlags;            //See FR_* defines.
  const wchar_t *pFindIt;   //Find string.
  int nFindItLen;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically (4.x only).
} TEXTFINDW;

typedef struct {
  DWORD dwFlags;               //See FR_* defines.
  const char *pFindIt;         //Find string.
  int nFindItLen;              //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically (4.x only).
  const char *pReplaceWith;    //Replace string.
  int nReplaceWithLen;         //Replace string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically (4.x only).
  BOOL bAll;                   //Replace all.
  INT_PTR nChanges;            //Count of changes.
} TEXTREPLACEA;

typedef struct {
  DWORD dwFlags;               //See FR_* defines.
  const wchar_t *pFindIt;      //Find string.
  int nFindItLen;              //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically (4.x only).
  const wchar_t *pReplaceWith; //Replace string.
  int nReplaceWithLen;         //Replace string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically (4.x only).
  BOOL bAll;                   //Replace all.
  INT_PTR nChanges;            //Count of changes.
} TEXTREPLACEW;

typedef struct {
  int nCodePageFrom;        //Source code page.
  int nCodePageTo;          //Target code page.
  DWORD dwFlags;            //See RCS_* defines.
} TEXTRECODE;

typedef struct {
  char szClassName[MAX_PATH];   //Window class name.
  char szWindowName[MAX_PATH];  //Window caption.
  DWORD dwStyle;                //Window style.
  int x;                        //X position.
  int y;                        //Y position.
  int nWidth;                   //X size.
  int nHeight;                  //Y size.
  HWND hWndParent;              //Parent window handle.
  HMENU hMenu;                  //Window menu handle.
  HINSTANCE hInstance;          //Program instance handle.
  LPVOID lpParam;               //Creation parameters.
} CREATEWINDOWA;

typedef struct {
  wchar_t szClassName[MAX_PATH];    //Window class name.
  wchar_t szWindowName[MAX_PATH];   //Window caption.
  DWORD dwStyle;                    //Window style.
  int x;                            //X position.
  int y;                            //Y position.
  int nWidth;                       //X size.
  int nHeight;                      //Y size.
  HWND hWndParent;                  //Parent window handle.
  HMENU hMenu;                      //Window menu handle.
  HINSTANCE hInstance;              //Program instance handle.
  LPVOID lpParam;                   //Creation parameters.
} CREATEWINDOWW;

typedef struct {
  HWND *lpWnd;   //Control window.
  DWORD dwType;  //See DRS_* defines.
  int nOffset;   //Control offset, set it to zero.
} DIALOGRESIZE;

typedef struct {
  DIALOGRESIZE *drs;  //Pointer to a first DIALOGRESIZE element in array.
  RECT *rcMinMax;     //Pointer to a min/max sizes. Each member is valid if not equal to zero. Can be NULL.
                      //RECT->rcMinMax.left, RECT->rcMinMax.top specifies minimum dialog size.
                      //RECT->rcMinMax.right, RECT->rcMinMax.bottom specifies maximum dialog size.
  RECT *rcCurrent;    //Pointer to a current rectangle. Set all members of RECT to zero at first call.
  DWORD dwFlags;      //See DRM_* defines.
  HWND hDlg;          //Dialog handle.
  UINT uMsg;          //Dialog message.
  WPARAM wParam;      //First message parameter.
  LPARAM lParam;      //Second message parameter.
} DIALOGRESIZEMSG;

typedef struct _DOCK {
  struct _DOCK *next;
  struct _DOCK *prev;
  DWORD dwFlags;          //Dock flags: see DKF_* defines.
  HWND hWnd;              //Dock window handle.
  int nSide;              //Dock side: DKS_LEFT, DKS_TOP, DKS_RIGHT or DKS_BOTTOM.
  RECT rcSize;            //Dock window client RECT.
  RECT rcDragDrop;        //Drag-and-drop client RECT.
  WNDPROC lpOldDockProc;  //Procedure address before subclassing.
} DOCK;

typedef struct {
  DWORD dwFlags;          //See BIF_* defines.
  HANDLE hImage;          //Bitmap handle if BIF_BITMAP specified or icon handle if BIF_ICON specified.
  int nImageWidth;        //Image width.
  int nImageHeight;       //Image height.
} BUTTONDRAW;

typedef struct {
  HWND hWnd;           //Window handle.
  UINT uMsg;           //Specifies the message to be sent.
  WPARAM wParam;       //Specifies additional message-specific information.
  LPARAM lParam;       //Specifies additional message-specific information.
} POSTMESSAGE;

typedef struct {
  const wchar_t *pCmdLine; //Command line string. On return contain pointer to a unprocessed string.
  const wchar_t *pWorkDir; //Command line string.
} PARSECMDLINESENDW;

typedef struct {
  BOOL bPostMessage;                   //FALSE for sending message (wait for return).
                                       //TRUE for posting message (returns immediately).
  wchar_t szCmdLine[COMMANDLINE_SIZE]; //Command line string.
  int nCmdLineLen;                     //Command line length, not including the terminating null character.
  wchar_t szWorkDir[MAX_PATH];         //Working directory string.
  int nWorkDirLen;                     //Working directory length, not including the terminating null character.
} PARSECMDLINEPOSTW;

typedef struct {
  HGLOBAL hDevMode;
  HGLOBAL hDevNames;

  //PRINTDLG
  HDC hDC;
  DWORD dwPrintFlags;
  WORD nFromPage;
  WORD nToPage;

  //PAGESETUPDLG
  DWORD dwPageFlags;
  RECT rtMargin;
} PRINTINFO;

typedef struct {
  INT_PTR nCharPos;       //Char position.
  COLORREF crText;        //Text color.
  COLORREF crBk;          //Background color.
} CHARCOLOR;

typedef struct {
  const BYTE *pFile;      //Opening file.
                          //  const char *pFile         if bOldWindows == TRUE
                          //  const wchar_t *pFile      if bOldWindows == FALSE
  const char *szFile;     //Opening file (Ansi).
  const wchar_t *wszFile; //Opening file (Unicode).
  int *nCodePage;         //Pointer to a code page variable.
  BOOL *bBOM;             //Pointer to a BOM variable.
  DWORD *dwFlags;         //Pointer to a open flags variable.
  BOOL bProcess;          //TRUE   open file.
                          //FALSE  do not open file.
} NOPENDOCUMENT;

typedef struct {
  const BYTE *pFile;      //Saving file.
                          //  const char *pFile         if bOldWindows == TRUE
                          //  const wchar_t *pFile      if bOldWindows == FALSE
  const char *szFile;     //Saving file (Ansi).
  const wchar_t *wszFile; //Saving file (Unicode).
  int *nCodePage;         //Pointer to a code page variable.
  BOOL *bBOM;             //Pointer to a BOM variable.
  DWORD dwFlags;          //See SD_* defines.
  BOOL bProcess;          //TRUE   save file.
                          //FALSE  do not save file.
} NSAVEDOCUMENT;

typedef struct {
  DWORD *dwStyle;         //Pointer to a maximized state variable (WS_MAXIMIZE or zero).
  DWORD *dwShow;          //Pointer to a SW_ constants combination variable.
  BOOL bProcess;          //TRUE   show main window.
                          //FALSE  do not show main window.
} NMAINSHOW;

typedef struct {
  HWND hWnd;              //Context menu window.
  UINT uType;             //Type:    NCM_EDIT, NCM_TAB or NCM_STATUS.
  POINT pt;               //Context menu coordiates.
  BOOL bProcess;          //TRUE   show context menu.
                          //FALSE  do not show context menu.
} NCONTEXTMENU;

typedef struct {
  RECT rcInitial;         //Initial client RECT (read-only).
  RECT rcCurrent;         //Current client RECT (writeable).
} NSIZE;


//// AkelPad menu messages

#define IDM_FILE_NEW                    4101  //Create new edit window.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_FILE_CREATENEW              4102  //Create new instance of program.
                                              //Return Value: new main window handle.
                                              //
#define IDM_FILE_OPEN                   4103  //Open file dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_FILE_REOPEN                 4104  //Reopen file.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_SAVE                   4105  //Save file.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_FILE_SAVEAS                 4106  //Save file dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_FILE_PAGESETUP              4107  //Print setup dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_FILE_PRINT                  4108  //Print dialog. lParam can be used to pass edit window handle.
                                              //Return Value: number of printed pages.
                                              //
#define IDM_FILE_EXIT                   4109  //Exit program.
                                              //Return Value: zero.
                                              //
#define IDM_FILE_SAVEALL                4110  //Save all documents.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_FILE_SAVEALLAS              4111  //Save all as dialog.
                                              //Return Value: zero.
                                              //
#define IDM_FILE_SILENTPRINT            4113  //Print without dialog. lParam can be used to pass edit window handle.
                                              //Return Value: number of printed pages.
                                              //
#define IDM_FILE_PRINTPREVIEW           4114  //Print preview dialog. lParam can be used to pass edit window handle.
                                              //Return Value: zero.
                                              //
#define IDM_FILE_REDETECT               4121  //Redetect code page of the current file.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_REOPENAS_ANSI          4122  //Reopen file as ANSI.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_REOPENAS_OEM           4123  //Reopen file as OEM.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_REOPENAS_KOIR          4124  //Reopen file as KOI-R.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_REOPENAS_UTF16LE       4125  //Reopen file as UTF16LE.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_REOPENAS_UTF16BE       4126  //Reopen file as UTF16BE.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_REOPENAS_UTF8          4127  //Reopen file as UTF8.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_FILE_SAVEAS_ANSI            4131  //Save file as ANSI.
                                              //Return Value: see ESD_* defines.
                                              //
#define IDM_FILE_SAVEAS_OEM             4132  //Save file as OEM.
                                              //Return Value: see ESD_* defines.
                                              //
#define IDM_FILE_SAVEAS_KOIR            4133  //Save file as KOI-R.
                                              //Return Value: see ESD_* defines.
                                              //
#define IDM_FILE_SAVEAS_UTF16LE         4134  //Save file as UTF16LE.
                                              //Return Value: see ESD_* defines.
                                              //
#define IDM_FILE_SAVEAS_UTF16BE         4135  //Save file as UTF16BE.
                                              //Return Value: see ESD_* defines.
                                              //
#define IDM_FILE_SAVEAS_UTF8            4136  //Save file as UTF8.
                                              //Return Value: see ESD_* defines.
                                              //
#define IDM_FILE_SAVEAS_UTF8_NOBOM      4137  //Save file as UTF8 without BOM.
                                              //Return Value: see ESD_* defines.
                                              //
#define IDM_FILE_CODEPAGEMENU           4140  //Show codepages context menu.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_UNDO                   4151  //Undo last operation.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_REDO                   4152  //Redo last operation.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_CUT                    4153  //Cut.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_COPY                   4154  //Copy.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_PASTE                  4155  //Paste.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_CLEAR                  4156  //Delete.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_SELECTALL              4157  //Select all.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_FIND                   4158  //Find dialog.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_FINDNEXTDOWN           4159  //Find last string down.
                                              //Return Value: Character position of the next match. If there are no more matches, the return value is –1.
                                              //
#define IDM_EDIT_FINDNEXTUP             4160  //Find last string up.
                                              //Return Value: Character position of the next match. If there are no more matches, the return value is –1.
                                              //
#define IDM_EDIT_REPLACE                4161  //Replace dialog.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_GOTO                   4162  //Go to dialog.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INSERTCHAR             4163  //Run charmap.exe.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INSERT_TAB             4164  //Insert tabulation.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INSERT_TAB_MENU        4165  //Insert tabulation only if several lines selected.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_DELETE_TAB             4166  //Delete tabulation.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_DELETE_TAB_MENU        4167  //Delete tabulation only if several lines selected.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INSERT_SPACE           4168  //Insert space.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INSERT_SPACE_MENU      4169  //Insert space only if several lines selected.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_DELETE_SPACE           4170  //Delete space.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_DELETE_SPACE_MENU      4171  //Delete space only if several lines selected.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_DELETE_FIRST_CHAR      4172  //Delete first char.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_DELETE_FIRST_CHAR_MENU 4173  //Delete first char only if selection not empty.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_DELETE_TRAILING_WHITESPACES 4174  //Delete trailing whitespaces.
                                                   //Return Value: zero.
                                                   //
#define IDM_EDIT_UPPERCASE              4175  //Convert text to UPPERCASE.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_LOWERCASE              4176  //Convert text to lowercase.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_SENTENCECASE           4177  //Convert text to Sentence case.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_TITLECASE              4178  //Convert text to Title Case.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INVERTCASE             4179  //Convert text to iNVERT cASE.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_LOOPCASE               4180  //Cycle case.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_RECODE                 4182  //Recode dialog.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INSERTDATE             4183  //Insert date.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_NEWLINE_WIN            4184  //Windows new line format.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_NEWLINE_UNIX           4185  //Unix new line format.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_NEWLINE_MAC            4186  //Mac new line format.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_INSERTMODE             4190  //Insert mode (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_PASTEANSI              4191  //Paste as ANSI text.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_PASTECOLUMN            4192  //Paste to column selection.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_PASTEAFTER             4193  //Paste text after caret.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_AUTOINDENT             4196  //Insert new line with keeping left spaces.
                                              //Return Value: TRUE - inserted with spaces, FALSE - inserted without spaces.
                                              //
#define IDM_EDIT_DELLINE                4197  //Delete current line.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_FONT                   4201  //Font dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_VIEW_COLORS                 4202  //Color theme dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_VIEW_FONTSIZE_INCREASE      4204  //Increase font 1px.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_FONTSIZE_DECREASE      4205  //Decrease font 1px.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_FONTSIZE_RESTORE       4206  //Decrease font 1px.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_WORDWRAP               4209  //Word wrap (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_ONTOP                  4210  //Always on top (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SHOW_STATUSBAR         4211  //Show statusbar (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SPLIT_WINDOW_ALL       4212  //Split window into four panes.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SPLIT_WINDOW_WE        4213  //Split window into two vertical panes.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SPLIT_WINDOW_NS        4214  //Split window into two horizontal panes.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SPLIT_WINDOW_OFF       4215  //Reserved.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_READONLY               4216  //Read only (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_EXEC                4251  //Execute command.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_OPTIONS_SAVETIME            4252  //Save file time (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_WATCHFILE           4253  //Watch file change (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_KEEPSPACE           4254  //Keep space (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_SINGLEOPEN_FILE     4255  //Single open file (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_SINGLEOPEN_PROGRAM  4256  //Single open program (on\off).
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_PLUGINS             4259  //Plugins dialog.
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS                     4260  //Options dialog.
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_SDI                 4261  //SDI mode.
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_MDI                 4262  //MDI mode.
                                              //Return Value: zero.
                                              //
#define IDM_OPTIONS_PMDI                4263  //Pseudo MDI mode.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABVIEW_TOP          4301  //MDI tab window at the top of the main window.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABVIEW_BOTTOM       4302  //MDI tab window at the bottom of the main window.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABVIEW_NONE         4303  //Hide MDI tab.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABTYPE_STANDARD     4304  //Standard MDI tab style.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABTYPE_BUTTONS      4305  //Buttons MDI tab style.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABTYPE_FLATBUTTONS  4306  //Flat buttons MDI tab style.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TILEHORIZONTAL       4307  //MDI windows - horizontal tile.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TILEVERTICAL         4308  //MDI windows - vertical tile.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_CASCADE              4309  //MDI windows - cascade.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABSWITCH_NEXTPREV   4310  //Switch MDI windows: next-previous.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_TABSWITCH_RIGHTLEFT  4311  //Switch MDI windows: right-left.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_FRAMENEXT            4316  //Activate next MDI window.
                                              //Return Value: pointer to a FRAMEDATA structure that has been activated.
                                              //
#define IDM_WINDOW_FRAMEPREV            4317  //Activate previous MDI window.
                                              //Return Value: pointer to a FRAMEDATA structure that has been activated.
                                              //
#define IDM_WINDOW_FRAMECLOSE           4318  //Close current MDI window.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_WINDOW_FRAMECLOSEALL        4319  //Close all documents.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_WINDOW_FRAMECLOSEALL_BUTACTIVE  4320  //Close all documents, but active.
                                                  //Return Value: TRUE - success, FALSE - failed.
                                                  //
#define IDM_WINDOW_FRAMECLOSEALL_UNMODIFIED 4321  //Close all unmodified documents.
                                                  //Return Value: TRUE - success, FALSE - failed.
                                                  //
#define IDM_WINDOW_FILECLOSE            4324  //Close file.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_WINDOW_FILEEXIT             4325  //Close file and exit program (SDI) or close tab of a file (MDI or PMDI).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_WINDOW_MDILIST              4327  //Select window dialog (MDI). Same as IDM_SELECTWINDOW.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_CHANGESIZE           4331  //Change style of the main window SW_RESTORE\SW_MAXIMIZE.
                                              //Return Value: zero.
                                              //
#define IDM_WINDOW_DLGNEXT              4332  //Activate next dialog window.
                                              //Return Value: activated dialog handle.
                                              //
#define IDM_WINDOW_DLGPREV              4333  //Activate previous dialog window.
                                              //Return Value: activated dialog handle.
                                              //
#define IDM_WINDOW_CLONENEXT            4341  //Activate next pane (split window).
                                              //Return Value: activated pane handle.
                                              //
#define IDM_WINDOW_CLONEPREV            4342  //Activate previous pane (split window).
                                              //Return Value: activated pane handle.
                                              //
#define IDM_ABOUT                       4351  //About dialog.
                                              //Return Value: zero.
                                              //
#define IDM_MANUAL                      4352  //Open user's manual.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_INTERNAL_REOPEN_MSG         4601  //Internal command.
                                              //Return Value: zero.
                                              //
#define IDM_INTERNAL_CANTOPEN_MSG       4602  //Internal command.
                                              //Return Value: zero.
                                              //
#define IDM_INTERNAL_ERRORIO_MSG        4603  //Internal command.
                                              //Return Value: zero.
                                              //
#define IDM_INTERNAL_UPDATEMAINCHILDREN 4604  //Internal command.
                                              //Return Value: zero.
                                              //
#define IDM_RECENT_FILES                5001  //Delete dead recent files.
                                              //5001 + n   Open recent file n.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_LANGUAGE                    6001  //Activate internal language.
                                              //6001 + n   Activate language n.
                                              //Return Value: zero.
                                              //
#define IDM_POPUP_OPENAS                7001  //Reopen file in first codepage of codepage list.
                                              //7001 + n   Reopen file in codepage n.
                                              //Return Value: see EOD_* defines.
                                              //
#define IDM_POPUP_SAVEAS                8001  //Save file in first codepage of codepage list.
                                              //8001 + n   Save file in codepage n.
                                              //Return Value: see ESD_* defines.
#define IDM_SELECTWINDOW                10019 //Select window dialog (MDI). Same as IDM_WINDOW_MDILIST.
                                              //Return Value: zero.
                                              //
                                              //
                                              //Example of usage:
                                              //SendMessage(pd->hMainWnd, WM_COMMAND, IDM_FILE_NEW, 0);

//// AkelPad main window WM_USER messages

//Notification messages
#define AKDN_MAIN_ONSTART          (WM_USER + 1)   //0x401
#define AKDN_MAIN_ONSTART_PRESHOW  (WM_USER + 2)   //0x402
#define AKDN_MAIN_ONSTART_SHOW     (WM_USER + 3)   //0x403
#define AKDN_MAIN_ONSTART_FINISH   (WM_USER + 4)   //0x404
#define AKDN_MAIN_ONSTART_IDLE     (WM_USER + 5)   //0x405
#define AKDN_MAIN_ONFINISH         (WM_USER + 6)   //0x406

#define AKDN_EDIT_ONSTART          (WM_USER + 11)  //0x40B
#define AKDN_EDIT_ONFINISH         (WM_USER + 12)  //0x40C
#define AKDN_EDIT_ONCLOSE          (WM_USER + 13)  //0x40D

#define AKDN_FRAME_NOWINDOWS       (WM_USER + 21)  //0x415
#define AKDN_FRAME_ACTIVATE        (WM_USER + 22)  //0x416
#define AKDN_FRAME_DESTROY         (WM_USER + 23)  //0x417

#define AKDN_DOCK_GETMINMAXINFO    (WM_USER + 31)  //0x41F
#define AKDN_DOCK_CAPTURE_ONSTART  (WM_USER + 32)  //0x420
#define AKDN_DOCK_CAPTURE_ONFINISH (WM_USER + 33)  //0x421
#define AKDN_DOCK_RESIZE           (WM_USER + 34)  //0x422

#define AKDN_DLLCALL               (WM_USER + 41)  //0x429
#define AKDN_DLLUNLOAD             (WM_USER + 42)  //0x42A

#define AKDN_ACTIVATE              (WM_USER + 51)  //0x433
#define AKDN_SIZE                  (WM_USER + 52)  //0x434
#define AKDN_OPENDOCUMENT_START    (WM_USER + 53)  //0x435
#define AKDN_OPENDOCUMENT_FINISH   (WM_USER + 54)  //0x436
#define AKDN_SAVEDOCUMENT_START    (WM_USER + 55)  //0x437
#define AKDN_SAVEDOCUMENT_FINISH   (WM_USER + 56)  //0x438
#define AKDN_HOTKEY                (WM_USER + 57)  //0x439
#define AKDN_CONTEXTMENU           (WM_USER + 58)  //0x43A
#define AKDN_SEARCH_ENDED          (WM_USER + 59)  //0x43B
#define AKDN_MESSAGEBOXBEGIN       (WM_USER + 61)  //0x43D
#define AKDN_MESSAGEBOXEND         (WM_USER + 62)  //0x43E

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

//Command line
#define AKD_GETCMDLINEOPTIONS      (WM_USER + 121)
#define AKD_SETCMDLINEOPTIONS      (WM_USER + 122)
#define AKD_PARSECMDLINEW          (WM_USER + 125)

//Text retrieval and modification
#define AKD_DETECTCODEPAGE         (WM_USER + 151)
#define AKD_DETECTCODEPAGEA        (WM_USER + 152)
#define AKD_DETECTCODEPAGEW        (WM_USER + 153)
#define AKD_READFILECONTENT        (WM_USER + 154)
#define AKD_OPENDOCUMENT           (WM_USER + 155)
#define AKD_OPENDOCUMENTA          (WM_USER + 156)
#define AKD_OPENDOCUMENTW          (WM_USER + 157)
#define AKD_SAVEDOCUMENT           (WM_USER + 158)
#define AKD_SAVEDOCUMENTA          (WM_USER + 159)
#define AKD_SAVEDOCUMENTW          (WM_USER + 160)
#define AKD_GETTEXTLENGTH          (WM_USER + 161)
#define AKD_GETTEXTRANGE           (WM_USER + 162)
#define AKD_GETTEXTRANGEA          (WM_USER + 163)
#define AKD_GETTEXTRANGEW          (WM_USER + 164)
#define AKD_GETSELTEXTW            (WM_USER + 165)
#define AKD_FREETEXT               (WM_USER + 166)
#define AKD_REPLACESEL             (WM_USER + 167)
#define AKD_REPLACESELA            (WM_USER + 168)
#define AKD_REPLACESELW            (WM_USER + 169)
#define AKD_PASTE                  (WM_USER + 170)
#define AKD_COPY                   (WM_USER + 171)
#define AKD_TEXTFIND               (WM_USER + 172)
#define AKD_TEXTFINDA              (WM_USER + 173)
#define AKD_TEXTFINDW              (WM_USER + 174)
#define AKD_TEXTREPLACE            (WM_USER + 175)
#define AKD_TEXTREPLACEA           (WM_USER + 176)
#define AKD_TEXTREPLACEW           (WM_USER + 177)
#define AKD_RECODESEL              (WM_USER + 178)
#define AKD_GETCHARCOLOR           (WM_USER + 179)
#define AKD_GOTO                   (WM_USER + 180)
#define AKD_GOTOA                  (WM_USER + 181)
#define AKD_GOTOW                  (WM_USER + 182)

//Print
#define AKD_GETPRINTINFO           (WM_USER + 191)
#define AKD_SETPRINTINFO           (WM_USER + 192)

//Options
#define AKD_PROGRAMVERSION         (WM_USER + 201)
#define AKD_PROGRAMARCHITECTURE    (WM_USER + 202)
#define AKD_GETMAININFO            (WM_USER + 203)
#define AKD_GETEDITINFO            (WM_USER + 204)
#define AKD_SETMODIFY              (WM_USER + 205)
#define AKD_SETNEWLINE             (WM_USER + 206)
#define AKD_GETFONT                (WM_USER + 207)
#define AKD_GETFONTA               (WM_USER + 208)
#define AKD_GETFONTW               (WM_USER + 209)
#define AKD_SETFONT                (WM_USER + 210)
#define AKD_SETFONTA               (WM_USER + 211)
#define AKD_SETFONTW               (WM_USER + 212)
#define AKD_GETCODEPAGELIST        (WM_USER + 213)
#define AKD_RECENTFILES            (WM_USER + 214)
#define AKD_SEARCHHISTORY          (WM_USER + 215)
#define AKD_GETEDITOPTION          (WM_USER + 216)
#define AKD_SETEDITOPTION          (WM_USER + 217)
#define AKD_GETCOLORS              (WM_USER + 218)
#define AKD_SETCOLORS              (WM_USER + 219)
#define AKD_GETLOCKINHERIT         (WM_USER + 220)
#define AKD_SETLOCKINHERIT         (WM_USER + 221)

//Windows
#define AKD_GETMODELESS            (WM_USER + 251)
#define AKD_SETMODELESS            (WM_USER + 252)
#define AKD_RESIZE                 (WM_USER + 253)
#define AKD_DOCK                   (WM_USER + 254)
#define AKD_SETBUTTONDRAW          (WM_USER + 255)
#define AKD_SETHOTKEYINPUT         (WM_USER + 256)
#define AKD_DIALOGRESIZE           (WM_USER + 257)

//Frames
#define AKD_FRAMEACTIVATE          (WM_USER + 261)
#define AKD_FRAMENEXT              (WM_USER + 262)
#define AKD_FRAMEDESTROY           (WM_USER + 263)
#define AKD_FRAMEFIND              (WM_USER + 264)
#define AKD_FRAMEFINDA             (WM_USER + 265)
#define AKD_FRAMEFINDW             (WM_USER + 266)
#define AKD_FRAMESTATS             (WM_USER + 267)
#define AKD_FRAMENOWINDOWS         (WM_USER + 268)
#define AKD_FRAMEISVALID           (WM_USER + 269)

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
#define AKD_TRANSLATEMESSAGE       (WM_USER + 291)

//Plugin load
#define AKD_DLLCALL                (WM_USER + 301)
#define AKD_DLLCALLA               (WM_USER + 302)
#define AKD_DLLCALLW               (WM_USER + 303)
#define AKD_DLLUNLOAD              (WM_USER + 304)
#define AKD_DLLFIND                (WM_USER + 305)
#define AKD_DLLFINDA               (WM_USER + 306)
#define AKD_DLLFINDW               (WM_USER + 307)
#define AKD_DLLADD                 (WM_USER + 308)
#define AKD_DLLADDA                (WM_USER + 309)
#define AKD_DLLADDW                (WM_USER + 310)
#define AKD_DLLDELETE              (WM_USER + 311)
#define AKD_DLLSAVE                (WM_USER + 312)
#define AKD_CALLPROC               (WM_USER + 313)

//Plugin options
#define AKD_BEGINOPTIONS           (WM_USER + 331)
#define AKD_BEGINOPTIONSA          (WM_USER + 332)
#define AKD_BEGINOPTIONSW          (WM_USER + 333)
#define AKD_OPTION                 (WM_USER + 334)
#define AKD_OPTIONA                (WM_USER + 335)
#define AKD_OPTIONW                (WM_USER + 336)
#define AKD_ENDOPTIONS             (WM_USER + 341)
#define AKD_INIOPEN                (WM_USER + 342)
#define AKD_INIOPENA               (WM_USER + 343)
#define AKD_INIOPENW               (WM_USER + 344)
#define AKD_INIGETSECTION          (WM_USER + 345)
#define AKD_INIGETSECTIONA         (WM_USER + 346)
#define AKD_INIGETSECTIONW         (WM_USER + 347)
#define AKD_INICLEARSECTION        (WM_USER + 348)
#define AKD_INIDELETESECTION       (WM_USER + 349)
#define AKD_INIGETKEY              (WM_USER + 350)
#define AKD_INIGETKEYA             (WM_USER + 351)
#define AKD_INIGETKEYW             (WM_USER + 352)
#define AKD_INIDELETEKEY           (WM_USER + 353)
#define AKD_INIGETVALUE            (WM_USER + 354)
#define AKD_INIGETVALUEA           (WM_USER + 355)
#define AKD_INIGETVALUEW           (WM_USER + 356)
#define AKD_INISETVALUE            (WM_USER + 357)
#define AKD_INISETVALUEA           (WM_USER + 358)
#define AKD_INISETVALUEW           (WM_USER + 359)
#define AKD_INICLOSE               (WM_USER + 360)

//AkelPad 4.x messages
#define AKD_EXGETTEXTLENGTH        (WM_USER + 401)
#define AKD_EXGETTEXTRANGE         (WM_USER + 402)
#define AKD_EXGETTEXTRANGEA        (WM_USER + 403)
#define AKD_EXGETTEXTRANGEW        (WM_USER + 404)
#define AKD_GETSTATUSPOSTYPE       (WM_USER + 405)


/*
AKDN_MAIN_ONSTART
_________________

Internal message.


AKDN_MAIN_ONSTART_PRESHOW
_________________________

Notification message, sends to the main procedure before showing main window.

wParam              == not used.
(NMAINSHOW *)lParam == show main window characteristics, program show code is:
                        NMAINSHOW *nms=(NMAINSHOW *)lParam;

                        ShowWindow(hMainWnd, (nms->dwStyle == WS_MAXIMIZE)?SW_SHOWMAXIMIZED:SW_SHOW);
                        if (nms->dwShow != SW_SHOWNORMAL) ShowWindow(hMainWnd, nms->dwShow);

Return Value
 Zero.


AKDN_MAIN_ONSTART_SHOW
______________________

Notification message, sends to the main procedure when main window is shown.

wParam == not used.
lParam == not used.

Return Value
 Zero.


AKDN_MAIN_ONSTART_FINISH
________________________

Notification message, sends to the main procedure when main window is finished initialize.

wParam == not used.
lParam == not used.

Return Value
 Zero.


AKDN_MAIN_ONSTART_IDLE
______________________

Notification message, sends to the main procedure when main window is finished initialize and there are no messages in the main thread's queue.

wParam == not used.
lParam == not used.

Return Value
 Zero.


AKDN_MAIN_ONFINISH
__________________

Notification message, sends to the main procedure before destroying main window.

wParam == not used.
lParam == not used.

Return Value
 Zero.


AKDN_EDIT_ONSTART
_________________

Notification message, sends to the main procedure after edit window created.

(HWND)wParam   == edit window.
(AEHDOC)lParam == edit document handle.

Return Value
 Zero.


AKDN_EDIT_ONFINISH
__________________

Notification message, sends to the main procedure before destroying edit window.

(HWND)wParam   == edit window.
(AEHDOC)lParam == edit document handle.

Return Value
 Zero.


AKDN_EDIT_ONCLOSE
_________________

Notification message, sends to the main procedure before closing edit window document. After closing current document will be empty.

(HWND)wParam   == edit window.
(AEHDOC)lParam == edit document handle.

Return Value
 Zero.


AKDN_FRAME_NOWINDOWS
____________________

Notification message, sends to the main procedure after closing tab when no windows in MDI client (WMD_MDI) or when there is only one empty window (WMD_PMDI).

wParam == not used.
lParam == not used.

Return Value
 Zero.


AKDN_FRAME_ACTIVATE
___________________

Notification message, sends to the main procedure when frame has been activated.

(DWORD)wParam       == see FWA_* defines.
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure, that has been activated.

Return Value
 Zero.


AKDN_FRAME_DESTROY
__________________

Notification message, sends to the main procedure before destroying frame.

wParam              == not used.
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure, that will be destroyed.

Return Value
 Zero.


AKDN_DOCK_GETMINMAXINFO
_______________________

Notification message, sends to the main procedure before changing dock window size.

(DOCK *)wParam       == pointer to a DOCK structure.
(MINMAXINFO *)lParam == pointer to a MINMAXINFO structure.

Return Value
 Zero.


AKDN_DOCK_CAPTURE_ONSTART
_________________________

Notification message, sends to the main procedure after mouse capturing started.

(DOCK *)wParam == pointer to a DOCK structure.
(int)lParam    == see DKC_* defines.

Return Value
 Zero.


AKDN_DOCK_CAPTURE_ONFINISH
__________________________

Notification message, sends to the main procedure after mouse capturing finished.

(DOCK *)wParam == pointer to a DOCK structure.
(int)lParam    == see DKC_* defines.

Return Value
 Zero.


AKDN_DOCK_RESIZE
________________

Notification message, sends to the main procedure after dock window size or side is changed.

(DOCK *)wParam == pointer to a DOCK structure.
(int)lParam    == see DKC_* defines.

Return Value
 Zero.


AKDN_ACTIVATE
_____________

Notification message, sends to the main procedure after another instance of the program activates main window.

wParam == not used.
lParam == not used.

Return Value
 Zero.


AKDN_SIZE
_________

Notification message, sends to the main procedure before the main window client RECT changed.

wParam           == not used.
(NSIZE *)lParam  == pointer to a NSIZE structure.

Return Value
 Zero.


AKDN_OPENDOCUMENT_START
_______________________

Notification message, sends to the main procedure before document opened.

(HWND)wParam            == edit window.
(NOPENDOCUMENT *)lParam == pointer to a NOPENDOCUMENT structure.

Return Value
 Zero.


AKDN_OPENDOCUMENT_FINISH
________________________

Notification message, sends to the main procedure after document opened.

(HWND)wParam == edit window.
(int)lParam  == see EOD_* defines.

Return Value
 Zero.


AKDN_SAVEDOCUMENT_START
_______________________

Notification message, sends to the main procedure before document saved.

(HWND)wParam            == edit window.
(NSAVEDOCUMENT *)lParam == pointer to a NSAVEDOCUMENT structure.

Return Value
 Zero.


AKDN_SAVEDOCUMENT_FINISH
________________________

Notification message, sends to the main procedure after document saved.

(HWND)wParam == edit window.
(int)lParam  == see ESD_* defines.

Return Value
 Zero.


AKDN_DLLCALL
____________

Notification message, sends to the main procedure after plugin call.

wParam               == not used.
(PLUGINDATA *)lParam == pointer to a PLUGINDATA structure.

Return Value
 Zero.


AKDN_DLLUNLOAD
______________

Notification message, sends to the main procedure after plugin unload.

wParam              == not used.
(UNISTRING *)lParam == pointer to a UNISTRING structure, that specified unloaded function name in format "Plugin::Function".

Return Value
 Zero.


AKDN_HOTKEY
___________

Notification message, sends to the main procedure when keyboard key is pressed.

(WORD)wParam  == hotkey returned by HKM_GETHOTKEY.
(int *)lParam ==  1  stop message processing.
                  0  continue message processing (default).
                 -1  continue message processing, but skip accelerators.

Return Value
 Zero.


AKDN_CONTEXTMENU
________________

Notification message, sends to the main procedure before displaying context menu.

wParam                 == not used.
(NCONTEXTMENU *)lParam == pointer to a NCONTEXTMENU structure.

Return Value
 Zero.


AKDN_SEARCH_ENDED
_________________

Notification message, sends to the main procedure after find/replace dialog found nothing.

(HWND)wParam == find/replace dialog.
lParam       == not used.

Return Value
 Zero.


AKDN_MESSAGEBOXBEGIN
____________________

Notification message, sends to the main procedure before messagebox is open.

(HWND)wParam == parent window of the messagebox.
lParam       == not used.

Return Value
 Zero.


AKDN_MESSAGEBOXEND
__________________

Notification message, sends to the main procedure after messagebox is closed.

(HWND)wParam == parent window of the messagebox.
lParam       == not used.

Return Value
 Zero.


AKD_GETMAINPROC, AKD_GETEDITPROC, AKD_GETFRAMEPROC
_______________  _______________  ________________

Get procedure from main window subclass chain.

(int)wParam            == index of procedure in chain.
                          If positive search from top.
                          If negative from beginning
(WNDPROCDATA **)lParam == procedure data.

Return Value
 Zero, if successfull.

Example (get program main procedure data):
 WNDPROCDATA *wpd;
 SendMessage(pd->hMainWnd, AKD_GETMAINPROC, -1, (LPARAM)&wpd);


AKD_SETMAINPROC, AKD_SETEDITPROC, AKD_SETFRAMEPROC
_______________  _______________  ________________

Set procedure in main window subclass chain.

(WNDPROC)wParam        == procedure address.
                          If (wParam == NULL) then lParam must point to the procedure
                            data that will be removed from main window subclass chain.
(WNDPROCDATA **)lParam == procedure data.
                          If (lParam == NULL) then create new procedure data.
                          If (*(WNDPROCDATA **)lParam == NULL) then create new
                            procedure data and set it on top of the main window
                            subclass chain.
                          If (*(WNDPROCDATA **)lParam != NULL) then set wParam
                            procedure in procedure data and update main window
                            subclass chain.

Return Value
 Zero.

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

(int)wParam               == index of procedure in chain.
                             If positive search from top.
                             If negative from beginning.
(WNDPROCRETDATA **)lParam == procedure data.

Return Value
 Zero, if successfull.

Example:
 WNDPROCRETDATA *wprd;
 SendMessage(pd->hMainWnd, AKD_GETMAINPROC, 2, (LPARAM)&wprd);


AKD_SETMAINPROCRET, AKD_SETEDITPROCRET, AKD_SETFRAMEPROCRET
__________________  __________________  ___________________

Set return procedure in main window subclass chain. This procedure calls after message have been processed.

(WNDPROC)wParam        == procedure address.
                          If (wParam == NULL) then lParam must point to the procedure
                            data that will be removed from main window subclass chain.
(WNDPROCDATA **)lParam == procedure data.
                          If (lParam == NULL) then create new procedure data.
                          If (*(WNDPROCDATA **)lParam == NULL) then create new
                            procedure data and set it on top of the main window
                            subclass chain.
                          If (*(WNDPROCDATA **)lParam != NULL) then set wParam
                            procedure in procedure data and update main window
                            subclass chain.

Return Value
 Zero.

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


AKD_GETCMDLINEOPTIONS
_____________________

Retrieve current command line options.

wParam == not used.
lParam == not used.

Return Value
 See CLO_* defines.

Example:
 SendMessage(pd->hMainWnd, AKD_GETCMDLINEOPTIONS, 0, 0);


AKD_SETCMDLINEOPTIONS
_____________________

Set command line options.

(DWORD)wParam == see CLO_* defines.
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_SETCMDLINEOPTIONS, CLO_MSGOPENCREATENO|CLO_MSGOPENBINARYNO, 0);


AKD_PARSECMDLINEW
_________________

Set command line options.

wParam                      == not used.
(PARSECMDLINESENDW *)lpData == pointer to a PARSECMDLINESENDW structure.

Return Value
 See PCLE_* defines.

Example:
 PARSECMDLINESENDW pcls;

 pcls.pCmdLine=L"/p \"C:\\MyFile.txt\"";
 pcls.pWorkDir=L"";
 SendMessage(pd->hMainWnd, AKD_PARSECMDLINEW, 0, (LPARAM)&pcls);


AKD_DETECTCODEPAGE, AKD_DETECTCODEPAGEA, AKD_DETECTCODEPAGEW
__________________  ___________________  ___________________

Detect codepage of a file.

lParam                   == not used.
(DETECTCODEPAGE *)lParam == pointer to a DETECTCODEPAGE structure.

Return Value
 See EDT_* defines.

Example:
 See AKD_READFILECONTENT example.


AKD_READFILECONTENT
___________________

Read contents of a file.

lParam                == not used.
(FILECONTENT *)lParam == pointer to a FILECONTENT structure.

Return Value
 Number of wide characters copied to a FILECONTENT.wpContent buffer.
 When you no longer need the buffer, call the AKD_FREETEXT function to delete it.

Example (bOldWindows == TRUE):
 int ReadFileContentA(const char *pFile, DWORD dwFlags, int nCodePage, BOOL bBOM, wchar_t **wpContent)
 {
   DETECTCODEPAGEA dc;
   FILECONTENT fc;
   int nResult=0;

   *wpContent=NULL;

   //Detect codepage
   dc.pFile=pFile;
   dc.dwBytesToCheck=1024;
   dc.dwFlags=dwFlags;
   dc.nCodePage=nCodePage;
   dc.bBOM=bBOM;
   if (SendMessage(hMainWnd, AKD_DETECTCODEPAGEA, 0, (LPARAM)&dc) == EDT_SUCCESS)
   {
     //Read contents
     if ((fc.hFile=CreateFileA(dc.pFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
     {
       fc.dwBytesMax=(UINT_PTR)-1;
       fc.nCodePage=dc.nCodePage;
       fc.bBOM=dc.bBOM;
       if (nResult=SendMessage(hMainWnd, AKD_READFILECONTENT, 0, (LPARAM)&fc))
       {
         CloseHandle(fc.hFile);
         fc.hFile=NULL;

         *wpContent=fc.wpContent;
       }
       if (fc.hFile) CloseHandle(fc.hFile);
     }
   }
   return nResult;
 }

 //Call ReadFileContentA function
 {
   wchar_t *wpContent;

   if (ReadFileContentA("C:\\MyFile.txt", ADT_BINARY_ERROR|ADT_DETECT_CODEPAGE|ADT_DETECT_BOM, 0, 0, &wpContent))
   {
     //Show contents
     MessageBoxW(hMainWnd, wpContent, NULL, MB_OK);
     SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpContent);
   }
 }

Example (bOldWindows == FALSE):
 int ReadFileContentW(const wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, wchar_t **wpContent)
 {
   DETECTCODEPAGEW dc;
   FILECONTENT fc;
   int nResult=0;

   *wpContent=NULL;

   //Detect codepage
   dc.pFile=wpFile;
   dc.dwBytesToCheck=1024;
   dc.dwFlags=dwFlags;
   dc.nCodePage=nCodePage;
   dc.bBOM=bBOM;
   if (SendMessage(hMainWnd, AKD_DETECTCODEPAGEW, 0, (LPARAM)&dc) == EDT_SUCCESS)
   {
     //Read contents
     if ((fc.hFile=CreateFileW(dc.pFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
     {
       fc.dwBytesMax=(UINT_PTR)-1;
       fc.nCodePage=dc.nCodePage;
       fc.bBOM=dc.bBOM;
       if (nResult=SendMessage(hMainWnd, AKD_READFILECONTENT, 0, (LPARAM)&fc))
       {
         CloseHandle(fc.hFile);
         fc.hFile=NULL;

         *wpContent=fc.wpContent;
       }
       if (fc.hFile) CloseHandle(fc.hFile);
     }
   }
   return nResult;
 }

 //Call ReadFileContentW function
 {
   wchar_t *wpContent;

   if (ReadFileContentW(L"C:\\MyFile.txt", ADT_BINARY_ERROR|ADT_DETECT_CODEPAGE|ADT_DETECT_BOM, 0, 0, &wpContent))
   {
     //Show contents
     MessageBoxW(hMainWnd, wpContent, NULL, MB_OK);
     SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpContent);
   }
 }


AKD_OPENDOCUMENT, AKD_OPENDOCUMENTA, AKD_OPENDOCUMENTW
________________  _________________  _________________

Open file. See also WM_COPYDATA with CD_OPENDOCUMENT for different process.

(HWND)wParam           == edit window. Set it to NULL.
(OPENDOCUMENT *)lParam == pointer to a OPENDOCUMENT structure.

Return Value
 See EOD_* defines.

Example (Unicode):
  OPENDOCUMENTW od;

  od.pFile=L"C:\\MyFile.txt";
  od.pWorkDir=NULL;
  od.dwFlags=OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE;
  od.nCodePage=0;
  od.bBOM=0;
  SendMessage(pd->hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);


AKD_SAVEDOCUMENT, AKD_SAVEDOCUMENTA, AKD_SAVEDOCUMENTW
________________  _________________  _________________

Save file.

(HWND)wParam           == edit window, NULL for current edit window.
(SAVEDOCUMENT *)lParam == pointer to a SAVEDOCUMENT structure.

Return Value
 See ESD_* defines.

Example (Unicode):
 SAVEDOCUMENTW sd;

 sd.pFile=L"C:\\MyFile.txt";
 sd.nCodePage=65001;
 sd.bBOM=TRUE;
 sd.dwFlags=SD_UPDATE;
 SendMessage(pd->hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)NULL, (LPARAM)&sd);


AKD_GETTEXTLENGTH
_________________

Get edit window text length.

(HWND)wParam == edit window, NULL for current edit window.
lParam       == not used.

Return Value
 Text length.

Example:
 int nLength=SendMessage(pd->hMainWnd, AKD_GETTEXTLENGTH, (WPARAM)pd->hWndEdit, 0);


AKD_GETTEXTRANGE, AKD_GETTEXTRANGEA, AKD_GETTEXTRANGEW
________________  _________________  _________________

Retrieves a specified range of characters from a edit control.

(HWND)wParam           == edit window, NULL for current edit window.
(GETTEXTRANGE *)lParam == pointer to a GETTEXTRANGE structure.

Return Value
 Text length in TCHARs without null character.

Example (Ansi):
 GETTEXTRANGE gtr;

 gtr.cpMin=0;
 gtr.cpMax=-1;
 if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGEA, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
 {
   MessageBoxA(pd->hMainWnd, (char *)gtr.pText, "Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
 }

Example (Unicode):
 GETTEXTRANGE gtr;

 gtr.cpMin=0;
 gtr.cpMax=-1;
 if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGEW, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
 {
   MessageBoxW(pd->hMainWnd, (wchar_t *)gtr.pText, L"Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
 }


AKD_GETSELTEXTW
_______________

Retrieves the currently selected text in a edit control.

(HWND)wParam  == edit window, NULL for current edit window.
(int *)lParam == pointer to a variable that receive text length (not including the terminating null character), can be NULL.

Return Value
 Text pointer, new lines are forced to "\r".

Example:
 wchar_t *wpText;
 int nTextLen=0;

 wpText=(wchar_t *)SendMessage(pd->hMainWnd, AKD_GETSELTEXTW, (WPARAM)pd->hWndEdit, (LPARAM)&nTextLen);
 SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpText);


AKD_FREETEXT
____________

Free text buffer allocated with AKD_GETTEXTRANGE.

wParam                  == not used.
(unsigned char *)lParam == buffer pointer.

Return Value
 TRUE   success.
 FALSE  failed.

See AKD_GETTEXTRANGE.


AKD_REPLACESEL, AKD_REPLACESELA, AKD_REPLACESELW
______________  _______________  _______________

Replace selection of the edit control.

(HWND)wParam                  == edit window, NULL for current edit window.
(const unsigned char *)lParam == text pointer.

Return Value
 Zero.

Example (Unicode):
 SendMessage(pd->hMainWnd, AKD_REPLACESELW, (WPARAM)pd->hWndEdit, (LPARAM)L"SomeString");


AKD_PASTE
_________

Paste text from clipboard to the edit control.

(HWND)wParam  == edit window, NULL for current edit window.
(DWORD)lParam == see PASTE_* defines.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(pd->hMainWnd, AKD_PASTE, (WPARAM)pd->hWndEdit, 0);


AKD_COPY
________

Copy text to clipboard from edit control.

(HWND)wParam == edit window, NULL for current edit window.
lParam       == not used.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_COPY, (WPARAM)pd->hWndEdit, 0);


AKD_TEXTFIND, AKD_TEXTFINDA, AKD_TEXTFINDW
____________  _____________  _____________

Finds text in a edit control.

(HWND)wParam       == edit window, NULL for current edit window.
(TEXTFIND *)lParam == pointer to a TEXTFIND structure.

Return Value
 Character position of the next match. If there are no more matches, the return value is –1.

Example (Unicode):
 TEXTFINDW tf;

 tf.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
 tf.pFindIt=L"Text to find";
 tf.nFindItLen=-1;
 SendMessage(pd->hMainWnd, AKD_TEXTFINDW, (WPARAM)pd->hWndEdit, (LPARAM)&tf);


AKD_TEXTREPLACE, AKD_TEXTREPLACEA, AKD_TEXTREPLACEW
_______________  ________________  ________________

Replaces text in a edit control.

(HWND)wParam          == edit window, NULL for current edit window.
(TEXTREPLACE *)lParam == pointer to a TEXTREPLACE structure.

Return Value
 Character position of the next match. If there are no more matches, the return value is –1.

Example (Unicode):
 TEXTREPLACEW tr;

 tr.dwFlags=FR_DOWN|FR_BEGINNING|FR_MATCHCASE;
 tr.pFindIt=L"Text to find";
 tr.nFindItLen=-1;
 tr.pReplaceWith=L"Text to replace";
 tr.nReplaceWithLen=-1;
 tr.bAll=TRUE;
 SendMessage(pd->hMainWnd, AKD_TEXTREPLACEW, (WPARAM)pd->hWndEdit, (LPARAM)&tr);


AKD_RECODESEL
_____________

Recode text in a edit control.

(HWND)wParam         == edit window, NULL for current edit window.
(TEXTRECODE *)lParam == pointer to a TEXTRECODE structure.

Return Value
 Zero.

Example:
 TEXTRECODE tr;

 tr.nCodePageFrom=1251;
 tr.nCodePageTo=866;
 tr.dwFlags=0;
 SendMessage(pd->hMainWnd, AKD_RECODESEL, (WPARAM)pd->hWndEdit, (LPARAM)&tr);


AKD_GETCHARCOLOR
________________

Get colors of the specified char.

(HWND)wParam        == edit window, NULL for current edit window.
(CHARCOLOR *)lParam == pointer to a CHARCOLOR structure.

Return Value
 TRUE   specified char in selection.
 FALSE  specified char not in selection.

Example:
 CHARCOLOR cc;

 cc.nCharPos=10;
 SendMessage(pd->hMainWnd, AKD_GETCHARCOLOR, (WPARAM)pd->hWndEdit, (LPARAM)&cc);


AKD_GOTO, AKD_GOTOA, AKD_GOTOW
________  _________  _________

Go to specified position in text.

(DWORD)wParam                 == see GT_* defines.
(const unsigned char *)lParam == pointer to a string that specified position in text. It is equal to "Go to..." dialog string.

Return Value
 TRUE   success.
 FALSE  failed.

Example (Unicode):
 SendMessage(pd->hMainWnd, AKD_GOTOW, GT_LINE, (LPARAM)L"100:3");


AKD_GETPRINTINFO
________________

Get print information.

(PRINTINFO *)lParam == pointer to a PRINTINFO structure.
lParam              == not used.

Return Value
 Zero.

Example:
 PRINTINFO pi;

 SendMessage(pd->hMainWnd, AKD_GETPRINTINFO, (WPARAM)&pi, 0);


AKD_SETPRINTINFO
________________

Set print information.

(PRINTINFO *)lParam == pointer to a PRINTINFO structure.
lParam              == not used.

Return Value
 Zero.

Example:
 PRINTINFO pi;

 SendMessage(pd->hMainWnd, AKD_SETPRINTINFO, (WPARAM)&pi, 0);


AKD_PROGRAMVERSION
__________________

Get program version.

wParam == not used.
lParam == not used.

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

wParam == not used.
lParam == not used.

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

wParam               == not used.
(PLUGINDATA *)lParam == pointer to a PLUGINDATA structure.

Return Value
 Zero.

Example:
 PLUGINDATA pd;

 SendMessage(pd->hMainWnd, AKD_GETMAININFO, 0, (LPARAM)&pd);


AKD_GETEDITINFO
_______________

Get edit window info.

(HWND)wParam       == edit window, NULL for current edit window.
(EDITINFO *)lParam == pointer to a EDITINFO structure.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 EDITINFO ei;

 SendMessage(pd->hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei);


AKD_SETMODIFY
_____________

Set edit window modification flag.

(HWND)wParam == edit window, NULL for current edit window.
(BOOL)lParam == TRUE  contents of edit control have been modified.
                FALSE contents of edit control haven't been modified.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_SETMODIFY, (WPARAM)pd->hWndEdit, TRUE);


AKD_SETNEWLINE
______________

Set edit window new line format.

(HWND)wParam == edit window, NULL for current edit window.
(int)lParam  == see NEWLINE_* defines.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_SETNEWLINE, (WPARAM)pd->hWndEdit, NEWLINE_UNIX);


AKD_GETFONT, AKD_GETFONTA, AKD_GETFONTW
___________  ____________  ____________

Get font.

(HWND)wParam      == edit window, NULL for current edit window.
(LOGFONT *)lParam == pointer to a LOGFONT structure. Can be NULL.

Return Value
 HFONT handle. If wParam is NULL and no frame windows exist, then returned last collected font handle.

Example:
 See AKD_SETFONT example.


AKD_SETFONT, AKD_SETFONTA, AKD_SETFONTW
___________  ____________  ____________

Set font.

(HWND)wParam      == edit window, NULL for current edit window.
(LOGFONT *)lParam == pointer to a LOGFONT structure.

Return Value
 TRUE   success.
 FALSE  failed.

Example (Unicode):
 LOGFONTW lfFont;

 SendMessage(pd->hMainWnd, AKD_GETFONTW, (WPARAM)NULL, (LPARAM)&lfFont);
 lfFont.lfHeight-=2;
 lstrcpynW(lfFont.lfFaceName, L"Courier New", LF_FACESIZE);
 SendMessage(pd->hMainWnd, AKD_SETFONTW, (WPARAM)NULL, (LPARAM)&lfFont);


AKD_GETCODEPAGELIST
___________________

Get codepage list.

(int *)wParam == if this parameter is not NULL, then the default codepage identifier copies to the variable.
lParam        == not used.

Return Value
 Pointer to a codepage array, in which last element is zero.

Example:
 int *lpCodepageList;
 int nDefaultCodepage;

 lpCodepageList=(int *)SendMessage(pd->hMainWnd, AKD_GETCODEPAGELIST, (WPARAM)&nDefaultCodepage, 0);


AKD_RECENTFILES
_______________

Recent files operations.

(int)wParam  == see RF_* defines.
(void)lParam == depend of RF_* define.

Return Value
 Depend on RF_* define.

Example:
 RECENTFILESTACK *rfs;
 RECENTFILE *rf;
 int nMaxRecentFiles;

 if (nMaxRecentFiles=SendMessage(pd->hMainWnd, AKD_RECENTFILES, RF_GET, (LPARAM)&rfs))
 {
   for (rf=rfs->first; rf; rf=rf->next)
   {
     MessageBoxW(NULL, rf->wszFile, NULL, 0);
   }
 }


AKD_SEARCHHISTORY
_________________

Search history operations.

(int)wParam  == see SH_* defines.
lParam       == not used.

Return Value
 Number of search strings.

Example:
 SendMessage(pd->hMainWnd, AKD_SEARCHHISTORY, SH_GET, 0);


AKD_GETEDITOPTION
_________________

Get option from current edit window.

(int)wParam  == see EO_* defines.
lParam       == not used.

Return Value
 Depend of EO_* define.

Example:
 int nTabSize=SendMessage(pd->hMainWnd, AKD_GETEDITOPTION, EO_TABSIZE, 0);


AKD_SETEDITOPTION
_________________

Set option for current edit window.

(int)wParam  == see EO_* defines.
(void)lParam == depend of EO_* define.

Return Value
 TRUE  success.
 FALSE error.

Example:
 SendMessage(pd->hMainWnd, AKD_SETEDITOPTION, EO_TEXTMARGINS, MAKELONG(4, 4));


AKD_GETCOLORS
_____________

Get colors.

(HWND)wParam       == edit window, NULL for current edit window.
(AECOLORS *)lParam == pointer to a AECOLORS structure.

Return Value
 Pointer to a AECOLORS structure.

Example:
 See AKD_SETCOLORS example.


AKD_SETCOLORS
_____________

Set colors.

(HWND)wParam       == edit window, NULL for current edit window.
(AECOLORS *)lParam == pointer to a AECOLORS structure.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AECOLORS aec;

 SendMessage(pd->hMainWnd, AKD_GETCOLORS, (WPARAM)NULL, (LPARAM)&aec);
 aec.crBasicBk=RGB(0x00, 0xFF, 0x00);
 SendMessage(pd->hMainWnd, AKD_SETCOLORS, (WPARAM)NULL, (LPARAM)&aec);


AKD_GETLOCKINHERIT
__________________

Get lock inherit flags.

(HWND)wParam == edit window, NULL for current edit window.
lParam       == not used.

Return Value
 Lock inherit flags, see LI_* defines. If error -1 is returned.

Example:
 SendMessage(pd->hMainWnd, AKD_GETLOCKINHERIT, (WPARAM)NULL, 0);


AKD_SETLOCKINHERIT
__________________

Lock inherit new document settings from current document.

(HWND)wParam  == edit window, NULL for current edit window.
(DWORD)lParam == lock inherit flags.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 DWORD dwLockInherit;

 dwLockInherit=(DWORD)SendMessage(pd->hMainWnd, AKD_GETLOCKINHERIT, (WPARAM)NULL, 0);
 dwLockInherit|=LI_FONT;
 SendMessage(pd->hMainWnd, AKD_SETLOCKINHERIT, (WPARAM)NULL, dwLockInherit);


AKD_GETMODELESS
_______________

Get modeless dialog handle.

wParam        == not used.
(int *)lParam == pointer to a variable that receive dialog MLT_* type. Can be NULL.

Return Value
 Dialog handle.

Example:
 int nType;
 HWND hDlg=(HWND)SendMessage(pd->hMainWnd, AKD_GETMODELESS, 0, (LPARAM)&nType);


AKD_SETMODELESS
_______________

Set modeless dialog handle.

(HWND)wParam == dialog handle.
lParam       == not used.

Return Value
 Zero.

Note
 Only one dialog can be registered as modeless. Application should unregister dialog before closing, passing NULL in wParam.

Example:
 SendMessage(pd->hMainWnd, AKD_SETMODELESS, (LPARAM)hMyDialog, 0);


AKD_RESIZE
__________

Update main window size with AKDN_SIZE notification message.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_RESIZE, 0, 0);


AKD_DOCK
________

Message to work with dockable window.

(DWORD)wParam  == see DK_* defines.
(DOCK *)lParam == pointer to a DOCK structure.

Return Value
 If DK_ADD, DK_FINDDOCK or DK_FINDCHILD specified, return value is a pointer to a DOCK structure. Otherwise return value is zero.

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


AKD_SETBUTTONDRAW
_________________

Draw icon or bitmap on button.

(HWND)wParam         == button handle. Button must have BS_OWNERDRAW style.
(BUTTONDRAW *)lParam == pointer to a BUTTONDRAW structure. If NULL, button draw processing will be removed.

Return Value
 Zero.

Example:
 BUTTONDRAW bd;

 bd.dwFlags=BIF_CROSS|BIF_ETCHED;
 bd.hImage=NULL;
 bd.nImageWidth=0;
 bd.nImageHeight=0;
 SendMessage(pd->hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndButton, (LPARAM)&bd);


AKD_SETHOTKEYINPUT
__________________

Associate hotkey control with plugins hotkey input.

(HWND)wParam == hotkey control handle.
(WORD)lParam == initial hotkey value. See HKM_GETHOTKEY message return value (MSDN).
                Initial hotkey value is sets automatically after HKM_SETHOTKEY message.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);


AKD_DIALOGRESIZE
________________

Automatic controls alignment in dialog.

wParam                    == not used.
(DIALOGRESIZEMSG *)lParam == pointer to a DIALOGRESIZEMSG structure.

Return Value
 TRUE  dialog size changed.
 FALSE dialog size unchanged.

Example:
RECT rcMainInitDialog={0};
RECT rcMainCurrentDialog={0};
BOOL bMainDialogRectChanged=FALSE;

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndOK;
  static HWND hWndCancel;
  static DIALOGRESIZE drs[]={{&hWndOK,     DRS_MOVE|DRS_X, 0},
                             {&hWndOK,     DRS_MOVE|DRS_Y, 0},
                             {&hWndCancel, DRS_MOVE|DRS_X, 0},
                             {&hWndCancel, DRS_MOVE|DRS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
  }

  //...Dialog messages processing

  //Dialog resize messages
  {
    DIALOGRESIZEMSG drsm={&drs[0], &rcMainInitDialog, &rcMainCurrentDialog, DRM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_DIALOGRESIZE, 0, (LPARAM)&drsm))
      bMainDialogRectChanged=TRUE;
  }
  return FALSE;
}


AKD_FRAMEACTIVATE
_________________

Activate specified frame.

(DWORD)wParam       == see FWA_* defines.
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure.

Return Value
 Pointer to a FRAMEDATA structure that has lost active status.

Example:
 FRAMEDATA *lpFrame=(FRAMEDATA *)SendMessage(pd->hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)L"C:\\File.txt");
 SendMessage(pd->hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrame);


AKD_FRAMENEXT
_____________

Activate next or previous frame.

(BOOL)wParam        == TRUE  activate previous frame.
                       FALSE activate next frame.
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure.

Return Value
 Pointer to a FRAMEDATA structure that has been activated.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMENEXT, FALSE, (LPARAM)lpFrameCurrent);


AKD_FRAMEDESTROY
________________

Destroy specified frame.

wParam              == not used.
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure.

Return Value
 See FWDE_* defines.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMEDESTROY, 0, (LPARAM)lpFrameCurrent);


AKD_FRAMEFIND, AKD_FRAMEFINDA, AKD_FRAMEFINDW
_____________  ______________  ______________

Find frame data pointer.

(int)wParam    == see FWF_* defines.
(void *)lParam == depend on FWF_ value.

Return Value
 Pointer to a FRAMEDATA structure.

Example:
 See AKD_FRAMEACTIVATE example.


AKD_FRAMESTATS
______________

Retrive windows statistics.

(int)wParam == see FWS_* defines.
lParam      == not used.

Return Value
 Depend of FWS_* define.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMESTATS, FWS_COUNTALL, 0);


AKD_FRAMENOWINDOWS
__________________

Retrive is there no windows in MDI client (WMD_MDI) or only one empty window (WMD_PMDI or WMD_SDI).

wParam == not used.
lParam == not used.

Return Value
 TRUE   no windows in MDI client (WMD_MDI),
        only one empty window (WMD_PMDI or WMD_SDI).
 FALSE  one or more windows in MDI client (WMD_MDI),
        one non-empty window (WMD_PMDI or WMD_SDI) or more than one window (WMD_PMDI).

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMENOWINDOWS, 0, 0);


AKD_FRAMEISVALID
________________

Determines whether the specified handle identifies an existing frame data pointer.

wParam              == not used.
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure.

Return Value
 TRUE   pointer is valid.
 FALSE  pointer is invalid.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMEISVALID, 0, (LPARAM)lpFrame);


AKD_GLOBALALLOC
_______________

Allocates the specified number of bytes from the heap (see description for GlobalAlloc in MSDN).

(DWORD)wParam == memory allocation attributes.
(DWORD)lParam == number of bytes to allocate.

Return Value
 Handle to the newly allocated memory object.

Example:
 HGLOBAL hMem=(HGLOBAL)SendMessage(pd->hMainWnd, AKD_GLOBALALLOC, GPTR, 128);


AKD_GLOBALLOCK
______________

Locks a global memory object (see description for GlobalLock in MSDN).

(HGLOBAL)wParam == handle to the global memory object.
lParam          == not used.

Return Value
 Pointer to the first byte of the memory block.

Example:
 void *pMem=(void *)SendMessage(pd->hMainWnd, AKD_GLOBALLOCK, (WPARAM)hMem, 0);


AKD_GLOBALUNLOCK
________________

Decrements the lock count associated with a memory object (see description for GlobalUnlock in MSDN).

(HGLOBAL)wParam == handle to the global memory object.
lParam          == not used.

Return Value
 Unlock result.

Example:
 BOOL bUnlock=SendMessage(pd->hMainWnd, AKD_GLOBALUNLOCK, (WPARAM)hMem, 0);


AKD_GLOBALFREE
______________

Frees the specified global memory object (see description for GlobalFree in MSDN).

(HGLOBAL)wParam == handle to the global memory object.
lParam          == not used.

Return Value
 Free result.

Example:
 HGLOBAL hMem=SendMessage(pd->hMainWnd, AKD_GLOBALFREE, (WPARAM)hMem, 0);


AKD_STRLENA
___________

Returns the length in bytes of the specified string (not including the terminating null character).

(char *)wParam == ANSI string.
lParam         == not used.

Return Value
 String length.

Example:
 SendMessage(pd->hMainWnd, AKD_STRLENA, (WPARAM)"123", 0);


AKD_STRLENW
___________

Returns the length in WCHAR's of the specified string (not including the terminating null character).

(wchar_t *)wParam == Unicode string.
lParam            == not used.

Return Value
 String length.

Example:
 SendMessage(pd->hMainWnd, AKD_STRLENW, (WPARAM)L"123", 0);


AKD_CREATEWINDOW
________________

Create window.

wParam                 == not used.
(CREATEWINDOW *)lParam == pointer to a CREATEWINDOW structure.
                          (CREATEWINDOWA *)lParam   if bOldWindows == TRUE
                          (CREATEWINDOWW *)lParam   if bOldWindows == FALSE

Return Value
 Window handle.

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

(BOOL)wParam == TRUE  test is any key pressed.
                FALSE wait for keys release.
lParam       == not used.

Return Value
 If wParam == FALSE the return value:
   Zero.
 If wParam == TRUE the return value:
   TRUE  keyboard is free.
   FALSE keyboard is busy.

Example:
 SendMessage(pd->hMainWnd, AKD_WAITKEYBOARD, 0, 0);


AKD_GETQUEUE
____________

Indicates the type of messages found in the main thread's message queue
(see description for GetQueueStatus in MSDN).

(DWORD)wParam == specifies the types of messages for which to check.
lParam        == not used.

Return Value
 Number of messages.

Example:
 DWORD dwStatus=SendMessage(pd->hMainWnd, AKD_GETQUEUE, QS_ALLEVENTS, 0);


AKD_POSTMESSAGE
_______________

Post message.

wParam                == not used.
(POSTMESSAGE *)lParam == pointer to a POSTMESSAGE, allocated with GlobalAlloc.

Return Value
 Zero.

Example (Unicode):
 typedef struct {
   POSTMESSAGE pm;
   SAVEDOCUMENTW sd;
   wchar_t szFile[MAX_PATH];
 } PMSAVEDOCUMENTW;

 PMSAVEDOCUMENTW *pmsd;

 if (pmsd=(PMSAVEDOCUMENTW *)GlobalAlloc(GPTR, sizeof(PMSAVEDOCUMENTW)))
 {
   lstrcpynW(pmsd->szFile, L"C:\\MyFile.txt", MAX_PATH);
   pmsd->sd.pFile=pmsd->szFile;
   pmsd->sd.nCodePage=65001;
   pmsd->sd.bBOM=TRUE;
   pmsd->sd.dwFlags=SD_UPDATE;

   //Post message
   pmsd->pm.hWnd=pd->hMainWnd;
   pmsd->pm.uMsg=AKD_SAVEDOCUMENTW;
   pmsd->pm.wParam=(WPARAM)pd->hWndEdit;
   pmsd->pm.lParam=(LPARAM)&pmsd->sd;
   PostMessage(pd->hMainWnd, AKD_POSTMESSAGE, 0, (LPARAM)pmsd);
 }


AKD_TRANSLATEMESSAGE
____________________

Process window message.

wParam        == not used.
(MSG *)lParam == pointer to an MSG structure that contains message information.

Return Value
 See TMSG_* defines.

Example:
 MSG msg;
 BOOL bExitLoop=FALSE;

 for (;;)
 {
   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
   {
     if (msg.message == WM_QUIT)
       bExitLoop=TRUE;
     else
       SendMessage(pd->hMainWnd, AKD_TRANSLATEMESSAGE, 0, (LPARAM)&msg);
   }
   if (bExitLoop)
     break;
   WaitMessage();
 }



AKD_DLLCALL, AKD_DLLCALLA, AKD_DLLCALLW
___________  ____________  ____________

Call dll.

(DWORD)wParam            == see DLLCF_* defines.
(PLUGINCALLSEND *)lParam == pointer to a PLUGINCALLSEND structure if SendMessage used
                            or pointer to a PLUGINCALLPOST, allocated with GlobalAlloc, if PostMessage used.

Return Value
 See EDL_* defines.

Example SendMessage (Unicode):
 PLUGINCALLSENDW pcs;

 pcs.pFunction=L"Plugin::Function";
 pcs.lParam=0;
 pcs.dwSupport=0;
 SendMessage(pd->hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

Example PostMessage (Unicode):
 PLUGINCALLPOSTW *pcp;

 if (pcp=(PLUGINCALLPOSTW *)GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW)))
 {
   lstrcpynW(pcp->szFunction, L"Plugin::Function", MAX_PATH);
   pcp->lParam=0;
   PostMessage(pd->hMainWnd, AKD_DLLCALLW, 0, (LPARAM)pcp);
 }


AKD_DLLUNLOAD
_____________

Exit from thread and unload dll.

(HMODULE)wParam == handle to the module.
(HANDLE)lParam  == handle to the thread, NULL if plugin not exiting from thread.

Return Value
 Zero.

Example:
 PostMessage(pd->hMainWnd, AKD_DLLUNLOAD, (WPARAM)pd->hInstanceDLL, (LPARAM)NULL);


AKD_DLLFIND, AKD_DLLFINDA, AKD_DLLFINDW
___________  ____________  ____________

Get dll function.

(const unsigned char *)wParam == function name, format "Plugin::Function".
(WORD)lParam                  == hotkey returned by HKM_GETHOTKEY,
                                 to search by hotkey set wParam to NULL.

Return Value
 Pointer to a PLUGINFUNCTION structure.

Example find by name (Unicode):
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION *)SendMessage(pd->hMainWnd, AKD_DLLFINDW, (WPARAM)L"SomePlugin::SomeFunction", 0))
   if (pf->bRunning) MessageBoxW(NULL, L"Plugin is running", NULL, 0);

Example find by hotkey:
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)NULL, 3112))
   if (pf->bRunning) MessageBoxW(NULL, L"Plugin is running", NULL, 0);


AKD_DLLADD, AKD_DLLADDA, AKD_DLLADDW
__________  ___________  ___________

Add dll function.

wParam              == not used.
(PLUGINADD *)lParam == function structure pointer.

Return Value
 Pointer to a PLUGINFUNCTION structure in stack.

Example add plugin hotkey (Unicode):
 BOOL CALLBACK PluginProc(void *lpParameter)
 {
   return TRUE; //TRUE - catch hotkey, FALSE - do default hotkey processing.
 }
 PLUGINADDW pf;

 pf.pFunction=L"MyDLL::MyFunction";
 pf.wHotkey=589;       //Ctrl+M
 pf.bAutoLoad=FALSE;
 pf.PluginProc=(PLUGINPROC)PluginProc;
 pf.lpParameter=NULL;
 SendMessage(pd->hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pf);


AKD_DLLDELETE
_____________

Delete dll function.

wParam                   == not used.
(PLUGINFUNCTION *)lParam == pointer to a PLUGINFUNCTION structure.

Return Value
 Zero.

Example (bOldWindows == TRUE):
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)"SomePlugin::SomeFunction", 0))
 {
   SendMessage(pd->hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pf);
   pf=NULL;
 }

Example (bOldWindows == FALSE):
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)L"SomePlugin::SomeFunction", 0))
 {
   SendMessage(pd->hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pf);
   pf=NULL;
 }


AKD_DLLSAVE
___________

Save dll stack.

(DWORD)wParam == see DLLSF_* defines.
lParam        == not used.

Return Value
 TRUE  success.
 FALSE error.

Example:
 SendMessage(pd->hMainWnd, AKD_DLLSAVE, DLLSF_NOW, 0);


AKD_CALLPROC
____________

Call procedure.

(PLUGINPROC)wParam == procedure address.
(void *)lParam     == pointer to a variable to be passed to the procedure.

Return Value
 Zero.

Example:
 void CALLBACK MyProcedure(void *lpParameter)
 {
 }
 PostMessage(pd->hMainWnd, AKD_CALLPROC, (WPARAM)MyProcedure, (LPARAM)NULL);


AKD_BEGINOPTIONS, AKD_BEGINOPTIONSA, AKD_BEGINOPTIONSW
________________  _________________  _________________

Begins read or save plugin or program options.

(DWORD)wParam                 == see POB_* defines.
(const unsigned char *)lParam == plugin name, if NULL then begin work with program options.

Return Value
 HANDLE.

Example read (bOldWindows == TRUE):
 PLUGINOPTIONA po;
 HANDLE hOptions;
 char szDir[MAX_PATH];

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_READ, (LPARAM)"SaveFile"))
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

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_READ, (LPARAM)L"SaveFile"))
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

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_SAVE, (LPARAM)"SaveFile"))
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

 if (hOptions=(HANDLE)SendMessage(pd->hMainWnd, AKD_BEGINOPTIONS, POB_SAVE, (LPARAM)L"SaveFile"))
 {
   po.pOptionName=L"SaveDir";
   po.dwType=PO_STRING;
   po.lpData=(LPBYTE)wszDir;
   po.dwData=(lstrlenW(wszDir) + 1) * sizeof(wchar_t);
   SendMessage(pd->hMainWnd, AKD_OPTION, (WPARAM)hOptions, (LPARAM)&po);

   SendMessage(pd->hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
 }


AKD_OPTION, AKD_OPTIONA, AKD_OPTIONW
__________  ___________  ___________

Read or save plugin option.

(HANDLE)wParam         == HANDLE returned by AKD_BEGINOPTIONS.
(PLUGINOPTION *)lParam == pointer to a PLUGINOPTION structure.

Return Value
 Size of the data copied to the buffer.

Example:
 See AKD_BEGINOPTIONS examples


AKD_ENDOPTIONS
______________

Ends plugin options.

(HANDLE)wParam == HANDLE returned by AKD_BEGINOPTIONS.
lParam         == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_BEGINOPTIONS examples


AKD_INIOPEN, AKD_INIOPENA, AKD_INIOPENW
___________  ____________  ____________

Opens ini file.

(DWORD)wParam                 == see POB_* defines.
(const unsigned char *)lParam == ini file.

Return Value
 HINIFILE.

Example read (bOldWindows == TRUE):
 INIVALUEA iv;
 HINIFILE hIniFile;
 char szDir[MAX_PATH];

 if (hIniFile=(HINIFILE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_READ, (LPARAM)"C:\\File.ini"))
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
 HINIFILE hIniFile;
 wchar_t wszDir[MAX_PATH];

 if (hIniFile=(HINIFILE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_READ, (LPARAM)L"C:\\File.ini"))
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
 HINIFILE hIniFile;
 char szDir[MAX_PATH]="C:\\Temp";

 if (hIniFile=(HINIFILE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_SAVE, (LPARAM)"C:\\File.ini"))
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
 HINIFILE hIniFile;
 wchar_t wszDir[MAX_PATH]=L"C:\\Temp";

 if (hIniFile=(HINIFILE)SendMessage(pd->hMainWnd, AKD_INIOPEN, POB_SAVE, (LPARAM)L"C:\\File.ini"))
 {
   iv.pSection=L"Options";
   iv.pKey=L"SaveDir";
   iv.dwType=INI_STRINGUNICODE;
   iv.lpData=(LPBYTE)wszDir;
   iv.dwData=(lstrlenW(wszDir) + 1) * sizeof(wchar_t);
   SendMessage(pd->hMainWnd, AKD_INISETVALUE, (WPARAM)hIniFile, (LPARAM)&iv);

   SendMessage(pd->hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
 }


AKD_INIGETSECTION, AKD_INIGETSECTIONA, AKD_INIGETSECTIONW
_________________  __________________  __________________

Retrieve ini section handle.

(HINIFILE)wParam              == ini file handle.
(const unsigned char *)lParam == section name.

Return Value
 HINISECTION.

Example (bOldWindows == TRUE):
 HINISECTION hIniSection;

 if (hIniSection=(HINISECTION)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
   SendMessage(pd->hMainWnd, AKD_INICLEARSECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);

Example (bOldWindows == FALSE):
 HINISECTION hIniSection;

 if (hIniSection=(HINISECTION)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
   SendMessage(pd->hMainWnd, AKD_INICLEARSECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);


AKD_INICLEARSECTION
___________________

Removes all keys in ini section.

(HINIFILE)wParam    == ini file handle.
(HINISECTION)lParam == ini section handle.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_INIGETSECTION example


AKD_INIDELETESECTION
____________________

Deletes ini section.

(HINIFILE)wParam    == ini file handle.
(HINISECTION)lParam == ini section handle.

Return Value
 TRUE   success.
 FALSE  failed.

Example (bOldWindows == TRUE):
 HINISECTION hIniSection;

 if (hIniSection=(HINISECTION)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
   SendMessage(pd->hMainWnd, AKD_INIDELETESECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);

Example (bOldWindows == FALSE):
 HINISECTION hIniSection;

 if (hIniSection=(HINISECTION)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
   SendMessage(pd->hMainWnd, AKD_INIDELETESECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);


AKD_INIGETKEY, AKD_INIGETKEYA, AKD_INIGETKEYW
_____________  ______________  ______________

Retrieve key handle.

(HINISECTION)wParam           == ini section handle.
(const unsigned char *)lParam == key name.

Return Value
 HINIKEY.

Example (bOldWindows == TRUE):
 HINISECTION hIniSection;
 HINIKEY hIniKey;

 if (hIniSection=(HINISECTION)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)"Options"))
   if (hIniKey=(HINIKEY)SendMessage(pd->hMainWnd, AKD_INIGETKEY, (WPARAM)hIniSection, (LPARAM)"SaveDir"))
     SendMessage(pd->hMainWnd, AKD_INIDELETEKEY, (WPARAM)hIniSection, (LPARAM)hIniKey);

Example (bOldWindows == FALSE):
 HINISECTION hIniSection;
 HINIKEY hIniKey;

 if (hIniSection=(HINISECTION)SendMessage(pd->hMainWnd, AKD_INIGETSECTION, (WPARAM)hIniFile, (LPARAM)L"Options"))
   if (hIniKey=(HINIKEY)SendMessage(pd->hMainWnd, AKD_INIGETKEY, (WPARAM)hIniSection, (LPARAM)L"SaveDir"))
     SendMessage(pd->hMainWnd, AKD_INIDELETEKEY, (WPARAM)hIniSection, (LPARAM)hIniKey);


AKD_INIDELETEKEY
________________

Deletes ini key.

(HINISECTION)wParam == ini section handle.
(HINIKEY)lParam     == key handle.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_INIGETKEY examples


AKD_INIGETVALUE, AKD_INIGETVALUEA, AKD_INIGETVALUEW
_______________  ________________  ________________

Retrieve ini value.

(HINIFILE)wParam   == ini file handle.
(INIVALUE *)lParam == pointer to a INIVALUE structure.

Return Value
 Size of the data copied to the buffer.

Example:
 See AKD_INIOPEN examples


AKD_INISETVALUE, AKD_INISETVALUEA, AKD_INISETVALUEW
_______________  ________________  ________________

Set ini value.

(HINIFILE)wParam   == ini file handle.
(INIVALUE *)lParam == pointer to a INIVALUE structure.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_INIOPEN examples


AKD_INICLOSE
____________

Close ini file handle.

(HINIFILE)wParam == ini file handle.
lParam           == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_INIOPEN examples


AKD_EXGETTEXTLENGTH
___________________

Get AkelEdit window text length.

(HWND)wParam == AkelEdit window, NULL for current edit window.
(int)lParam  == see AELB_* defines.

Return Value
 Text length.

Example:
 int nLength=SendMessage(pd->hMainWnd, AKD_EXGETTEXTLENGTH, (WPARAM)pd->hWndEdit, AELB_ASOUTPUT);


AKD_EXGETTEXTRANGE, AKD_EXGETTEXTRANGEA, AKD_EXGETTEXTRANGEW
__________________  ___________________  ___________________

Retrieves a specified range of characters from a AkelEdit control.

(HWND)wParam             == AkelEdit window, NULL for current edit window.
(EXGETTEXTRANGE *)lParam == pointer to a EXGETTEXTRANGE structure.

Return Value
 Text length in TCHARs. Without null character if EXGETTEXTRANGE.pText member is not NULL or including null character if EXGETTEXTRANGE.pText member is NULL.

Example (bOldWindows == FALSE):
 EXGETTEXTRANGE tr;

 SendMessage(pd->hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr);
 tr.pText=NULL;
 tr.nNewLine=AELB_ASIS;

 if (SendMessage(pd->hMainWnd, AKD_EXGETTEXTRANGE, (WPARAM)pd->hWndEdit, (LPARAM)&tr))
 {
   MessageBoxW(pd->hMainWnd, (wchar_t *)tr.pText, L"Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
 }


AKD_GETSTATUSPOSTYPE
____________________

Get status position type parameter.

wParam == not used.
lParam == not used.

Return Value
 See SPT_* defines.

Example:
 DWORD dwStatusPosType=SendMessage(pd->hMainWnd, AKD_GETSTATUSPOSTYPE, 0, 0);

*/


//// AkelPad WM_COPYDATA messages

#define CD_OPENDOCUMENT   1
#define CD_OPENDOCUMENTA  2
#define CD_OPENDOCUMENTW  3
#define CD_PARSECMDLINEW  9



/*
CD_OPENDOCUMENT, CD_OPENDOCUMENTA, CD_OPENDOCUMENTW
_______________  ________________  ________________

Open file. Same as AKD_OPENDOCUMENT, but can be used from outside of AkelPad process.

(DWORD)dwData              == CD_OPENDOCUMENT.
(DWORD)cbData              == sizeof(OPENDOCUMENTPOSTA)    if dwData == CD_OPENDOCUMENTA
                              sizeof(OPENDOCUMENTPOSTW)    if dwData == CD_OPENDOCUMENTW
(OPENDOCUMENTPOST *)lpData == (OPENDOCUMENTPOSTA *)lpData  if dwData == CD_OPENDOCUMENTA
                              (OPENDOCUMENTPOSTW *)lpData  if dwData == CD_OPENDOCUMENTW

Return Value
 See EOD_* defines.

Example (Ansi):
  OPENDOCUMENTPOSTA odp;
  COPYDATASTRUCT cds;

  lstrcpynA(odp.szFile, "C:\\MyFile.txt", MAX_PATH);
  odp.szWorkDir[0]='\0';
  odp.hWnd=NULL;
  odp.dwFlags=OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE;
  odp.nCodePage=0;
  odp.bBOM=0;

  cds.dwData=CD_OPENDOCUMENTA;
  cds.cbData=sizeof(OPENDOCUMENTPOSTA);
  cds.lpData=&odp;
  SendMessage(hWndDestination, WM_COPYDATA, (WPARAM)pd->hMainWnd, (LPARAM)&cds);


CD_PARSECMDLINEW
________________

Parse command line. Same as AKD_PARSECMDLINE, but can be used from outside of AkelPad process.

(DWORD)dwData               == CD_PARSECMDLINE.
(DWORD)cbData               == sizeof(PARSECMDLINEPOSTW)
(PARSECMDLINEPOSTW *)lpData == pointer to a PARSECMDLINEPOSTW structure.

Return Value
 If PARSECMDLINEPOST.bPostMessage is FALSE, then return value is PCLE_* defines.
 If PARSECMDLINEPOST.bPostMessage is TRUE, then return value is zero.

Example (Unicode):
 COPYDATASTRUCT cds;
 PARSECMDLINEPOSTW *pclp;
 wchar_t *wpCmdLine=L"/p \"C:\\MyFile.txt\""

 if (pclp=(PARSECMDLINEPOSTW *)GlobalAlloc(GMEM_FIXED, sizeof(PARSECMDLINEPOSTW)))
 {
   pclp->bPostMessage=TRUE;
   pclp->nCmdLineLen=xstrcpynW(pclp->szCmdLine, wpCmdLine, COMMANDLINE_SIZE);
   pclp->nWorkDirLen=GetCurrentDirectoryWide(MAX_PATH, pclp->szWorkDir);

   cds.dwData=CD_PARSECMDLINEW;
   cds.cbData=sizeof(PARSECMDLINEPOSTW);
   cds.lpData=(PVOID)pclp;
   SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
   GlobalFree((HGLOBAL)pclp);
 }
*/

//// UNICODE define

#ifndef UNICODE
  #define DETECTCODEPAGE DETECTCODEPAGEA
  #define OPENDOCUMENT OPENDOCUMENTA
  #define SAVEDOCUMENT SAVEDOCUMENTA
  #define PLUGINCALLSEND PLUGINCALLSENDA
  #define PLUGINCALLPOST PLUGINCALLPOSTA
  #define PLUGINOPTION PLUGINOPTIONA
  #define INIVALUE INIVALUEA
  #define TEXTFIND TEXTFINDA
  #define TEXTREPLACE TEXTREPLACEA
  #define CREATEWINDOW CREATEWINDOWA
#else
  #define DETECTCODEPAGE DETECTCODEPAGEW
  #define OPENDOCUMENT OPENDOCUMENTW
  #define SAVEDOCUMENT SAVEDOCUMENTW
  #define PLUGINCALLSEND PLUGINCALLSENDW
  #define PLUGINCALLPOST PLUGINCALLPOSTW
  #define PLUGINOPTION PLUGINOPTIONW
  #define INIVALUE INIVALUEW
  #define TEXTFIND TEXTFINDW
  #define TEXTREPLACE TEXTREPLACEW
  #define CREATEWINDOW CREATEWINDOWW
#endif

#endif
