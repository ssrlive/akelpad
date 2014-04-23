(*************************************************************************

=========  AkelPad text editor plugin API ===========
=========    Akel API version : 2.0.0.2   ===========

** Origin: AkelDLL.h located at
   http://akelpad.cvs.sourceforge.net/viewvc/akelpad/akelpad_4/AkelFiles/Plugs/AkelDLL/AkelDLL.h
** Converted with C to Pascal Converter 2.0
** Release: 2.20.11.2011
** Email: al_gun@ncable.net.au
** Updates: http://cc.codegear.com/Author/302259
** Blogs: http://delphiprogrammingwithalgun.blogspot.com/
** Copyright (c) 2005, 2011 Ural Gunaydin (a.k.a. Al Gun)

===========           Edited by Fr0sT           ===========
= Tested on RAD Studio XE2 but compiles on D7 and should =
= work on other versions also.                            =
= All RegExp defines were excluded (they are in separate file RegExpFunc.h now)

**************************************************************************)

unit AkelDLL;

interface

uses
  Windows, Messages;

//// Version
function MakeIdentifier(a, b, c, d: ShortInt): DWORD;
function AkelDLLVer: DWORD;

const AkelDLLVerArr: array[1..4] of Byte = (2, 0, 0, 2);


//// Defines

//Control IDs
const ID_EDIT           = 10001;
const ID_STATUS         = 10002;
const ID_TAB            = 10003;
const ID_PROGRESS       = 10004;
const ID_FIRSTMDI       = 10010;

//Window mode
const WMD_SDI   = 0;  //Single document interface (SDI).
const WMD_MDI   = 1;  //Multiple document interface (MDI).
const WMD_PMDI  = 2;  //Pseudo-Multiple document interface (PMDI).

//Sizes
const COMMANDLINE_SIZE        = 32768;
const COMMANDARG_SIZE         = 16384;
const WORD_DELIMITERS_SIZE    = 128;
const WRAP_DELIMITERS_SIZE    = 128;
const URL_PREFIXES_SIZE       = 128;
const URL_DELIMITERS_SIZE     = 128;

//Unload plugin flag
const UD_FAILED                = -1;  //Operation failed. Don't use it.
const UD_UNLOAD                 = 0;  //Unload plugin (default).
const UD_NONUNLOAD_ACTIVE     = $1;  //Don't unload plugin and set active status.
const UD_NONUNLOAD_NONACTIVE  = $2;  //Don't unload plugin and set non-active status.
const UD_NONUNLOAD_UNCHANGE   = $4;  //Don't unload plugin and don't change active status.
const UD_HOTKEY_DODEFAULT     = $8;  //Do default hotkey processing.

//Hotkey owner
const HKO_NONE                  = 0;  //Hotkey is not assigned.
const HKO_PLUGINFUNCTION        = 1;  //Hotkey assigned to plugin function.
const HKO_HOTKEYSPLUGIN         = 2;  //Hotkey assigned to Hotkeys plugin command.

//Command line options
const CLO_NONOTEPADCMD          = $01;  //Don't use MS Notepad compatibility mode when parse command line parameters.
const CLO_GLOBALPRINT           = $02;  //Next opened file will be printed.
const CLO_MSGOPENCREATEYES      = $04;  //Silently create new file, if doesn't exist.
const CLO_MSGOPENCREATENO       = $08;  //Don't create new file, if doesn't exist.
const CLO_MSGOPENBINARYYES      = $10;  //Silently open file, if binary.
const CLO_MSGOPENBINARYNO       = $20;  //Don't open file, if binary.
const CLO_MSGSAVELOSTSYMBOLSYES = $40;  //Silently save file, even if symbols will be lost.
const CLO_MSGSAVELOSTSYMBOLSNO  = $80;  //Don't save file, if symbols will be lost.

//AKD_PARSECMDLINE return value
const PCLE_QUIT    = $01;  //Stop parsing command line parameters and close program.
const PCLE_END     = $02;  //Stop parsing command line parameters.
const PCLE_ONLOAD  = $04;  //Done parsing command line parameters on program load (used internally).

//MI_ONFINISH type
const MOF_NONE        = 0;
const MOF_QUERYEND    = 1; //Processing WM_CLOSE or WM_QUERYENDSESSION message.
const MOF_DESTROY     = 2; //Processing AKDN_MAIN_ONFINISH message.

//Autodetect flags
const ADT_BINARY_ERROR        = $00000001;  //Check if file is binary.
const ADT_REG_CODEPAGE        = $00000002;  //If last open code page found in registry, then it will be
                                            //used with ADT_DETECT_BOM flag. If not found, then next flags
                                            //will be used ADT_DETECT_CODEPAGE|ADT_DETECT_BOM.
const ADT_DETECT_CODEPAGE     = $00000004;  //Detect code page.
const ADT_DETECT_BOM          = $00000008;  //Detect BOM mark.
const ADT_NOMESSAGES          = $00000010;  //No messages, if errors in autodetect.

//Autodetect errors
const EDT_SUCCESS        = 0;   //Success.
const EDT_OPEN          = -1;   //Can't open file.
const EDT_ALLOC         = -2;   //Can't allocate buffer.
const EDT_READ          = -3;   //Read file error.
const EDT_BINARY        = -4;   //File is binary.

//Open document flags
const OD_ADT_BINARY_ERROR      = ADT_BINARY_ERROR;     //See ADT_BINARY_ERROR.
const OD_ADT_REG_CODEPAGE      = ADT_REG_CODEPAGE;     //See ADT_REG_CODEPAGE.
const OD_ADT_DETECT_CODEPAGE   = ADT_DETECT_CODEPAGE;  //See ADT_DETECT_CODEPAGE.
const OD_ADT_DETECT_BOM        = ADT_DETECT_BOM;       //See ADT_DETECT_BOM.
const OD_ADT_NOMESSAGES        = ADT_NOMESSAGES;       //See ADT_NOMESSAGES.
const OD_REOPEN                = $00000100;           //Don't create new MDI window, use the exited one.
const OD_NOSCROLL              = $00000200;           //Don't restore scroll position.
const OD_MULTIFILE             = $00000400;           //More documents is on queue. Use MB_YESNOCANCEL instead of MB_OKCANCEL.

//Open document errors
const EOD_SUCCESS              = 0;          //Success.
const EOD_ADT_OPEN             = EDT_OPEN;   //See EDT_OPEN.
const EOD_ADT_ALLOC            = EDT_ALLOC;  //See EDT_ALLOC.
const EOD_ADT_READ             = EDT_READ;   //See EDT_READ.
const EOD_OPEN                 = -11;        //Can't open file.
const EOD_WINDOW_EXIST         = -13;        //File already opened.
const EOD_CODEPAGE_ERROR       = -14;        //Code page isn't implemented.
const EOD_STOP                 = -15;        //Stopped from AKDN_OPENDOCUMENT_START.
const EOD_STREAMIN             = -16;        //Error in EM_STREAMIN.
const EOD_DOCUMENTS_LIMIT      = -17;        //Documents limit reached in MDI mode.
const EOD_MSGNO                = -20;        //File is skipped.
const EOD_MSGCANCELCREATE      = -21;        //User press cancel in message "Create new file?".
const EOD_MSGCANCELBINARY      = -22;        //User press cancel in message "Binary file. Continue?".

//Save document flags
const SD_UPDATE            = $00000001;  //Update file info.
const SD_SELECTION         = $00000002;  //Save only selection.

//Save document errors
const ESD_SUCCESS          = 0;  //Success.
const ESD_OPEN            = -1;  //Can't open file.
const ESD_WRITE           = -2;  //Can't write to file.
const ESD_READONLY        = -3;  //File has read-only attribute.
const ESD_CODEPAGE_ERROR  = -4;  //Code page isn't implemented.
const ESD_STOP            = -5;  //Stopped from AKDN_SAVEDOCUMENT_START.
const ESD_STREAMOUT       = -6;  //Error in EM_STREAMOUT.
const ESD_NOWINDOW        = -7;  //No window to save from.

//Code pages int
const CP_UNICODE_UTF16LE  = 1200;
const CP_UNICODE_UTF16BE  = 1201;
const CP_UNICODE_UTF32LE  = 12000;
const CP_UNICODE_UTF32BE  = 12001;
const CP_UNICODE_UTF7     = 65000;
const CP_UNICODE_UTF8     = 65001;
const CP_KOI8_R           = 20866;

//Save settings
const SS_REGISTRY   = 1;  //Registry.
const SS_INI        = 2;  //INI file.

//AKD_RECENTFILES flags
const RF_GET             = 1;  //Retrieve current recent files info.
                              //(RECENTFILESTACK **)lParam is a pointer to a variable that receive pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is maximum number of recent files.
const RF_SET             = 2;  //Set recent files number.
                              //(int)lParam is maximum number of recent files.
                              //Return value is zero.
const RF_READ            = 3;  //Read recent files from registry.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is number of records read.
const RF_SAVE            = 4;  //Save recent files to registry.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is zero.
const RF_CLEAR           = 5;  //Clear recent files stack. Use RF_SAVE to save result.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is zero.
const RF_DELETEOLD       = 6;  //Delete non-existent recent files records. Use RF_SAVE to save result.
                              //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                              //Return value is number of records deleted.
const RF_FINDINDEX       = 7;  //Find item index in recent files stack by file name.
                              //(wchar_t *)lParam is a pointer to a file name.
                              //Return value is item index in recent files stack, -1 if error.
const RF_DELETEINDEX     = 8;  //Delete item from recent files stack by index.
                              //(int)lParam is index in recent files array to delete.
                              //Return value is TRUE - success, FALSE - error.
const RF_FINDITEMBYINDEX = 9;  //Find pointer to a RECENTFILE structure by index.
                              //(int)lParam is index of item in recent files stack.
                              //Return value is a pointer to a RECENTFILE structure.
const RF_FINDITEMBYNAME  = 10; //Find pointer to a RECENTFILE structure by file name.
                              //(wchar_t *)lParam is a pointer to a file name.
                              //Return value is a pointer to a RECENTFILE structure.
const RF_DELETEITEM      = 11; //Delete item from recent files stack.
                              //(RECENTFILE *)lParam a pointer to a RECENTFILE structure to delete.
                              //Return value is zero.
const RF_GETPARAM        = 21; //Get recent file parameter by name.
                              //(RECENTFILEPARAM *)lParam a pointer to a RECENTFILEPARAM structure with filled "file" and "pParamName" members.
                              //Return value is a pointer to a real RECENTFILEPARAM structure or NULL if error.
const RF_SETPARAM        = 22; //Set recent file parameter.
                              //(RECENTFILEPARAM *)lParam a pointer to a RECENTFILEPARAM structure with filled "file", "pParamName" and "pParamValue" members.
                              //Return value is a pointer to a real RECENTFILEPARAM structure or NULL if error.
const RF_DELETEPARAM     = 23; //Delete recent file parameter.
                              //(RECENTFILEPARAM *)lParam a pointer to a real RECENTFILEPARAM structure.
                              //Return value is zero.

//AKD_SEARCHHISTORY flags
const SH_GET    = 1;  //Retrieve search strings count.
const SH_CLEAR  = 2;  //Clear search history.

//AKD_GETMAININFO type

//PLUGINDATA
const MI_AKELDIRA                  = 1;    //Return: copied chars. (char *)lParam - buffer that receives AkelPad directory string.
const MI_AKELDIRW                  = 2;    //Return: copied chars. (wchar_t *)lParam - buffer that receives AkelPad directory string.
const MI_INSTANCEEXE               = 3;    //Return: EXE instance.
const MI_PLUGINSSTACK              = 4;    //Return: copied bytes. (HSTACK *)lParam - buffer that receives plugin stack.
const MI_SAVESETTINGS              = 5;    //Return: see SS_* defines.
const MI_WNDPROGRESS               = 10;   //Return: progress bar window handle.
const MI_WNDSTATUS                 = 11;   //Return: status bar window handle.
const MI_WNDMDICLIENT              = 12;   //Return: MDI client window handle.
const MI_WNDTAB                    = 13;   //Return: tab window handle.
const MI_MENUMAIN                  = 21;   //Return: main menu handle.
const MI_MENURECENTFILES           = 22;   //Return: recent files menu handle.
const MI_MENULANGUAGE              = 23;   //Return: language menu handle.
const MI_MENUPOPUP                 = 24;   //Return: right click menu handle.
const MI_ICONMAIN                  = 31;   //Return: main window icon handle.
const MI_ACCELGLOBAL               = 32;   //Return: global accelerator table (highest priority).
const MI_ACCELMAIN                 = 33;   //Return: main accelerator table (lowest priority).
const MI_OLDWINDOWS                = 41;   //Return: non-Unicode Windows.
const MI_OLDRICHEDIT               = 42;   //Return: riched20.dll lower then 5.30 (v3.0).
const MI_OLDCOMCTL32               = 43;   //Return: comctl32.dll lower then 4.71.
const MI_AKELEDIT                  = 44;   //Return: AkelEdit control is used.
const MI_MDI                       = 45;   //Return: window mode, see WMD_* defines.
const MI_LANGMODULEA               = 51;   //Return: copied chars. (char *)lParam - buffer that receives language module string.
const MI_LANGMODULEW               = 52;   //Return: copied chars. (wchar_t *)lParam - buffer that receives language module string.
const MI_LANGIDSYSTEM              = 53;   //Return: system language ID.
const MI_LANGIDMODULE              = 54;   //Return: language module language ID.
//Execution
const MI_ONSTART                   = 90;   //Return: TRUE - execution is between AKDN_MAIN_ONSTART and AKDN_MAIN_ONSTART_FINISH, FALSE - elsewhere.
const MI_ONFINISH                  = 91;   //Return: see MOF_* defines.
const MI_AKELEXEA                  = 95;   //Return: copied chars. (char *)lParam - buffer that receives AkelPad executable string.
const MI_AKELEXEW                  = 96;   //Return: copied chars. (wchar_t *)lParam - buffer that receives AkelPad executable string.
//Compile
const MI_X64                       = 101;  //Return: TRUE - x64 version, FALSE - x86 version.
const MI_AKELEDITSTATICBUILD       = 102;  //Return: TRUE - AkelEdit is compiled statically, FALSE - AkelEdit is compiled as standalone library.
const MI_AKELPADDLLBUILD           = 103;  //Return: TRUE - AkelPad is compiled as library, FALSE - AkelPad is compiled as executable.
//Manual
const MI_CMDLINEBEGIN              = 105;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "CmdLineBegin" string.
const MI_CMDLINEEND                = 106;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "CmdLineEnd" string.
const MI_SHOWMODIFY                = 110;  //Return: "ShowModify" flags, see SM_* defines.
const MI_STATUSPOSTYPE             = 111;  //Return: "StatusPosType" type, see SPT_* defines.
const MI_STATUSUSERFORMAT          = 112;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "StatusUserFormat" string.
const MI_WORDBREAKCUSTOM           = 117;  //Return: "WordBreak" flags.
const MI_PAINTOPTIONS              = 121;  //Return: "PaintOptions" flags, see PAINT_* defines.
const MI_EDITSTYLE                 = 122;  //Return: "EditStyle" flags, see EDS_* defines.
const MI_RICHEDITCLASS             = 125;  //Return: "RichEditClass" type.
const MI_AKELADMINRESIDENT         = 126;  //Return: AkelAdmin.exe resident - TRUE or unloaded immediately after execution - FALSE.
const MI_DATELOGFORMAT             = 129;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "DateLogFormat" string.
const MI_DATEINSERTFORMAT          = 130;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "DateInsertFormat" string.
const MI_AKELUPDATEROPTIONS        = 131;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "AkelUpdaterOptions" string.
const MI_URLCOMMAND                = 132;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "UrlCommand" string.
const MI_TABNAMEFIND               = 133;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "TabNameFind" string.
const MI_TABNAMEREP                = 134;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "TabNameRep" string.
//Menu
const MI_ONTOP                     = 141;  //Return: always on top (on\off).
const MI_STATUSBAR                 = 142;  //Return: show statusbar (on\off).
const MI_KEEPSPACE                 = 146;  //Return: keep left space (on\off).
const MI_WATCHFILE                 = 147;  //Return: watch file change (on\off).
const MI_SAVETIME                  = 148;  //Return: save original file time (on\off).
const MI_SINGLEOPENFILE            = 152;  //Return: single open file (on\off).
const MI_SINGLEOPENPROGRAM         = 153;  //Return: single open program flags, see SOP_* defines.
const MI_TABOPTIONSMDI             = 157;  //Return: tab flags, see TAB_* defines.
//Settings dialog
const MI_EXECUTECOMMAND            = 171;  //Return: copied chars. (wchar_t *)lParam - buffer that receives execution command string.
const MI_EXECUTEDIRECTORY          = 172;  //Return: copied chars. (wchar_t *)lParam - buffer that receives execution directory string.
const MI_CODEPAGELIST              = 176;  //Return: copied bytes. (int *)lParam - buffer that receives array of codepages, last element in array is zero.
const MI_DEFAULTCODEPAGE           = 177;  //Return: default codepage.
const MI_DEFAULTBOM                = 178;  //Return: default BOM.
const MI_NEWFILECODEPAGE           = 179;  //Return: new file codepage.
const MI_NEWFILEBOM                = 180;  //Return: new file BOM.
const MI_NEWFILENEWLINE            = 181;  //Return: new file new line, see NEWLINE_* defines.
const MI_LANGCODEPAGERECOGNITION   = 183;  //Return: codepage recognition language defined as LANGID.
const MI_CODEPAGERECOGNITIONBUFFER = 184;  //Return: size of codepage recognition buffer.
const MI_SAVEPOSITIONS             = 192;  //Return: save recent file positions (on\off).
const MI_SAVECODEPAGES             = 193;  //Return: save recent file codepages (on\off).
const MI_RECENTFILES               = 194;  //Return: number of recent files.
const MI_SEARCHSTRINGS             = 198;  //Return: number of search strings.
const MI_FILETYPESOPEN             = 202;  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated file types to open.
const MI_FILETYPESEDIT             = 203;  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated file types to edit.
const MI_FILETYPESPRINT            = 204;  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated file types to print.
const MI_FILETYPESASSOCIATED       = 205;  //Return: associated file types, see FTA_* defines.
const MI_KEYBLAYOUTOPTIONS         = 209;  //Return: keyboard layout options, see KLO_* defines.
const MI_SILENTCLOSEEMPTYMDI       = 213;  //Return: silently close unsaved empty MDI tab (on\off).
const MI_DATELOG                   = 217;  //Return: insert date if file has .LOG at the beginning (on\off).
const MI_SAVEINREADONLYMSG         = 221;  //Return: save in read-only files warning (on\off).
const MI_DEFAULTSAVEEXT            = 224;  //Return: copied chars. (wchar_t *)lParam - buffer that receives default saving extension string.
const MI_SEARCHOPTIONS             = 228;  //Return: search options, see FRF_* defines.
//Print dialog
const MI_PRINTMARGINS              = 251;  //Return: copied bytes. (RECT *)lParam - buffer that receives print margins.
const MI_PRINTCOLOR                = 255;  //Return: color printing, see PRNC_* defines.
const MI_PRINTHEADERENABLE         = 259;  //Return: enable print header (on\off).
const MI_PRINTHEADER               = 260;  //Return: copied chars. (wchar_t *)lParam - buffer that receives print header string.
const MI_PRINTFOOTERENABLE         = 261;  //Return: enable print footer (on\off).
const MI_PRINTFOOTER               = 262;  //Return: copied chars. (wchar_t *)lParam - buffer that receives print footer string.
const MI_PRINTFONTENABLE           = 266;  //Return: enable print font (on\off).
const MI_PRINTFONTW                = 267;  //Return: copied bytes. (LOGFONTW *)lParam - buffer that receives print font data.
//Open dialog
const MI_LASTDIR                   = 281;  //Return: copied chars. (wchar_t *)lParam - buffer that receives last directory of open dialog.
const MI_SHOWPLACESBAR             = 282;  //Return: show places bar in open dialog (on\off).

//AKD_SETMAININFO type

//PLUGINDATA
const MIS_SAVESETTINGS              = 5;    //(int)lParam - see SS_* defines.
const MIS_MDI                       = 45;   //(int)lParam - window mode, see WMD_* defines. Required program restart.
const MIS_LANGMODULEA               = 51;   //(char *)lParam - language module string. Required program restart.
const MIS_LANGMODULEW               = 52;   //(wchar_t *)lParam - language module string. Required program restart.
//Manual
const MIS_CMDLINEBEGIN              = 105;  //(wchar_t *)lParam - "CmdLineBegin" string.
const MIS_CMDLINEEND                = 106;  //(wchar_t *)lParam - "CmdLineEnd" string.
const MIS_SHOWMODIFY                = 110;  //(DWORD)lParam - "ShowModify" flags, see SM_* defines.
const MIS_STATUSPOSTYPE             = 111;  //(DWORD)lParam - "StatusPosType" type, see SPT_* defines.
const MIS_STATUSUSERFORMAT          = 112;  //(wchar_t *)lParam - "StatusUserFormat" string.
const MIS_WORDBREAKCUSTOM           = 117;  //(DWORD)lParam - "WordBreak" flags. Changes are applied for a new edit windows.
const MIS_PAINTOPTIONS              = 121;  //(DWORD)lParam - "PaintOptions" flags, see PAINT_* defines. Changes are applied for a new edit windows.
const MIS_EDITSTYLE                 = 122;  //(DWORD)lParam - "EditStyle" flags, see EDS_* defines. Changes are applied for a new edit windows.
const MIS_RICHEDITCLASS             = 125;  //(BOOL)lParam - "RichEditClass" type. Changes are applied for a new edit windows.
const MIS_AKELADMINRESIDENT         = 126;  //(BOOL)lParam - AkelAdmin.exe resident - TRUE or unloaded immediately after execution - FALSE.
const MIS_DATELOGFORMAT             = 129;  //(wchar_t *)lParam - "DateLogFormat" string.
const MIS_DATEINSERTFORMAT          = 130;  //(wchar_t *)lParam - "DateInsertFormat" string.
const MIS_AKELUPDATEROPTIONS        = 131;  //(wchar_t *)lParam - "AkelUpdaterOptions" string.
const MIS_URLCOMMAND                = 132;  //(wchar_t *)lParam - "UrlCommand" string.
const MIS_TABNAMEFIND               = 133;  //(wchar_t *)lParam - "TabNameFind" string. Changes are applied for a new opened documents.
const MIS_TABNAMEREP                = 134;  //(wchar_t *)lParam - "TabNameRep" string. Changes are applied for a new opened documents.
//Menu
const MIS_ONTOP                     = 141;  //(BOOL)lParam - always on top (on\off).
const MIS_STATUSBAR                 = 142;  //(BOOL)lParam - show statusbar (on\off).
const MIS_KEEPSPACE                 = 146;  //(BOOL)lParam - keep left space (on\off).
const MIS_WATCHFILE                 = 147;  //(BOOL)lParam - watch file change (on\off).
const MIS_SAVETIME                  = 148;  //(BOOL)lParam - save original file time (on\off).
const MIS_SINGLEOPENFILE            = 152;  //(BOOL)lParam - single open file (on\off).
const MIS_SINGLEOPENPROGRAM         = 153;  //(DWORD)lParam - single open program flags, see SOP_* defines.
const MIS_TABOPTIONSMDI             = 157;  //(DWORD)lParam - tab flags, see TAB_* defines.
//Settings dialog
const MIS_EXECUTECOMMAND            = 171;  //(wchar_t *)lParam - execution command string.
const MIS_EXECUTEDIRECTORY          = 172;  //(wchar_t *)lParam - execution directory string.
const MIS_CODEPAGELIST              = 176;  //(int *)lParam - array of codepages, last element in array is zero.
const MIS_DEFAULTCODEPAGE           = 177;  //(int)lParam - default codepage.
const MIS_DEFAULTBOM                = 178;  //(BOOL)lParam - default BOM.
const MIS_NEWFILECODEPAGE           = 179;  //(int)lParam - new file codepage.
const MIS_NEWFILEBOM                = 180;  //(BOOL)lParam - new file BOM.
const MIS_NEWFILENEWLINE            = 181;  //(int)lParam - new file new line, see NEWLINE_* defines.
const MIS_LANGCODEPAGERECOGNITION   = 183;  //(DWORD)lParam - codepage recognition language defined as LANGID.
const MIS_CODEPAGERECOGNITIONBUFFER = 184;  //(DWORD)lParam - size of codepage recognition buffer.
const MIS_SAVEPOSITIONS             = 192;  //(BOOL)lParam - save recent file positions (on\off).
const MIS_SAVECODEPAGES             = 193;  //(BOOL)lParam - save recent file codepages (on\off).
const MIS_RECENTFILES               = 194;  //(int)lParam - number of recent files.
const MIS_SEARCHSTRINGS             = 198;  //(int)lParam - number of search strings.
const MIS_FILETYPESOPEN             = 202;  //(wchar_t *)lParam - associated file types to open.
const MIS_FILETYPESEDIT             = 203;  //(wchar_t *)lParam - associated file types to edit.
const MIS_FILETYPESPRINT            = 204;  //(wchar_t *)lParam - associated file types to print.
const MIS_FILETYPESASSOCIATED       = 205;  //(DWORD)lParam - associated file types, see FTA_* defines. If FTA_ flag is set file types will be associated. If FTA_ flag is not set file types will be deassociated.
const MIS_KEYBLAYOUTOPTIONS         = 209;  //(DWORD)lParam - keyboard layout options, see KLO_* defines.
const MIS_SILENTCLOSEEMPTYMDI       = 213;  //(BOOL)lParam - silently close unsaved empty MDI tab (on\off).
const MIS_DATELOG                   = 217;  //(BOOL)lParam - insert date if file has .LOG at the beginning (on\off).
const MIS_SAVEINREADONLYMSG         = 221;  //(BOOL)lParam - save in read-only files warning (on\off).
const MIS_DEFAULTSAVEEXT            = 224;  //(wchar_t *)lParam - default saving extension string.
const MIS_SEARCHOPTIONS             = 228;  //(DWORD)lParam - search options, see FRF_* defines.
//Print dialog
const MIS_PRINTMARGINS              = 251;  //(RECT *)lParam - print margins.
const MIS_PRINTCOLOR                = 255;  //(DWORD)lParam - color printing, see PRNC_* defines.
const MIS_PRINTHEADERENABLE         = 259;  //(BOOL)lParam - enable print header (on\off).
const MIS_PRINTHEADER               = 260;  //(wchar_t *)lParam - print header string.
const MIS_PRINTFOOTERENABLE         = 261;  //(BOOL)lParam - enable print footer (on\off).
const MIS_PRINTFOOTER               = 262;  //(wchar_t *)lParam - print footer string.
const MIS_PRINTFONTENABLE           = 266;  //(BOOL)lParam - enable print font (on\off).
const MIS_PRINTFONTW                = 267;  //(LOGFONTW *)lParam - print font data.
//Open dialog
const MIS_LASTDIR                   = 281;  //(wchar_t *)lParam - last directory of open dialog.
const MIS_SHOWPLACESBAR             = 282;  //(BOOL)lParam - show places bar in open dialog (on\off).

//AKD_GETFRAMEINFO type. See FRAMEDATA members description.
const FI_WNDEDITPARENT        = 1;
const FI_WNDEDIT              = 2;
const FI_DOCEDIT              = 3;
const FI_WNDMASTER            = 4;
const FI_DOCMASTER            = 5;
const FI_WNDCLONE1            = 6;
const FI_DOCCLONE1            = 7;
const FI_WNDCLONE2            = 8;
const FI_DOCCLONE2            = 9;
const FI_WNDCLONE3            = 10;
const FI_DOCCLONE3            = 11;
const FI_CODEPAGE             = 12;
const FI_BOM                  = 13;
const FI_NEWLINE              = 14;
const FI_MODIFIED             = 15;
const FI_READONLY             = 16;
const FI_WORDWRAP             = 17;
const FI_OVERTYPE             = 18;
const FI_FILEA                = 31;
const FI_FILEW                = 32;
const FI_FILELEN              = 33;
const FI_STREAMOFFSET         = 34;
const FI_ICONHANDLE           = 38;
const FI_ICONINDEX            = 39;
const FI_RECTEDIT             = 43;
const FI_RECTMASTER           = 44;
const FI_EDITFONTW            = 48;
const FI_TABSTOPSIZE          = 52;
const FI_TABSTOPASSPACES      = 53;
const FI_UNDOLIMIT            = 57;
const FI_DETAILEDUNDO         = 58;
const FI_WRAPTYPE             = 62;
const FI_WRAPLIMIT            = 63;
const FI_MARKER               = 64;
const FI_CARETWIDTH           = 68;
const FI_CARETOPTIONS         = 69;
const FI_ALTLINEFILL          = 73;
const FI_ALTLINESKIP          = 74;
const FI_ALTLINEBORDER        = 75;
const FI_RECTMARGINS          = 79;
const FI_LINEGAP              = 83;
const FI_MOUSEOPTIONS         = 87;
const FI_CLICKURL             = 88;
const FI_SHOWURL              = 89;
const FI_URLPREFIXESENABLE    = 93;
const FI_URLPREFIXES          = 94;
const FI_URLDELIMITERSENABLE  = 98;
const FI_URLLEFTDELIMITERS    = 99;
const FI_URLRIGHTDELIMITERS   = 103;
const FI_WORDDELIMITERSENABLE = 107;
const FI_WORDDELIMITERS       = 108;
const FI_WRAPDELIMITERSENABLE = 112;
const FI_WRAPDELIMITERS       = 113;
const FI_MAPPEDPRINTWIDTH     = 121;
const FI_INPUTLOCALE          = 125;
const FI_LOCKINHERIT          = 129;
const FI_FILETIME             = 133;
const FI_COLORS               = 137;
const FI_BKIMAGEFILE          = 140;
const FI_BKIMAGEALPHA         = 141;

//AKD_SETFRAMEINFO type.
const FIS_TABSTOPSIZE          = 1;   //(int)FRAMEINFO.dwData - tabulation size in characters.
const FIS_TABSTOPASSPACES      = 2;   //(BOOL)FRAMEINFO.dwData - insert tabulation as spaces.
const FIS_UNDOLIMIT            = 6;   //(int)FRAMEINFO.dwData - undo limit.
const FIS_DETAILEDUNDO         = 7;   //(BOOL)FRAMEINFO.dwData - detailed undo.
const FIS_WRAP                 = 11;  //(DWORD)FRAMEINFO.dwData - the low-order word contains the wrap type (see AEWW_* defines in AkelEdit.h). The high-order word contains the wrap limit in characters.
const FIS_MARKER               = 12;  //(int)FRAMEINFO.dwData - column marker position.
const FIS_CARETWIDTH           = 16;  //(int)FRAMEINFO.dwData - caret width.
const FIS_CARETOPTIONS         = 17;  //(DWORD)FRAMEINFO.dwData - caret options, see CO_* defines.
const FIS_ALTLINES             = 21;  //(DWORD)FRAMEINFO.dwData - alternating lines. The low-order word of FRAMEINFO.dwData contains the skip interval in lines. The high-order word of FRAMEINFO.dwData contains the fill interval in lines.
const FIS_ALTLINEBORDER        = 22;  //(BOOL)FRAMEINFO.dwData - draw alternating lines border.
const FIS_RECTMARGINS          = 26;  //(RECT *)FRAMEINFO.dwData - edit margins. FRAMEINFO.dwData contain left, top, right, bottom margins in pixels.
const FIS_LINEGAP              = 30;  //(int)FRAMEINFO.dwData - gap between lines.
const FIS_MOUSEOPTIONS         = 34;  //(DWORD)FRAMEINFO.dwData - mouse options, see MO_* defines.
const FIS_SHOWURL              = 35;  //(BOOL)FRAMEINFO.dwData - show URL.
const FIS_CLICKURL             = 36;  //(int)FRAMEINFO.dwData - number of clicks to open URL.
const FIS_URLPREFIXESENABLE    = 40;  //(BOOL)FRAMEINFO.dwData - URL prefixes enable.
const FIS_URLPREFIXES          = 41;  //(wchar_t *)FRAMEINFO.dwData - URL prefixes.
const FIS_URLDELIMITERSENABLE  = 45;  //(BOOL)FRAMEINFO.dwData - URL delimiters enable.
const FIS_URLLEFTDELIMITERS    = 46;  //(wchar_t *)FRAMEINFO.dwData - URL left delimiters.
const FIS_URLRIGHTDELIMITERS   = 50;  //(wchar_t *)FRAMEINFO.dwData - URL right delimiters.
const FIS_WORDDELIMITERSENABLE = 54;  //(BOOL)FRAMEINFO.dwData - word delimiters enabled.
const FIS_WORDDELIMITERS       = 55;  //(wchar_t *)FRAMEINFO.dwData - word delimiters.
const FIS_WRAPDELIMITERSENABLE = 59;  //(BOOL)FRAMEINFO.dwData - wrap delimiters enabled.
const FIS_WRAPDELIMITERS       = 60;  //(wchar_t *)FRAMEINFO.dwData - wrap delimiters.
const FIS_LOCKINHERIT          = 68;  //(DWORD)FRAMEINFO.dwData - lock inherit new document settings from current document. FRAMEINFO.dwData contain lock inherit flags, see LI_* defines.
const FIS_COLORS               = 72;  //(AECOLORS *)FRAMEINFO.dwData - set colors.
const FIS_BKIMAGE              = 73;  //(BKIMAGE *)FRAMEINFO.dwData - set background image.

//New line format
const NEWLINE_WIN   = 1;  //Windows/DOS new line format (\r\n).
const NEWLINE_UNIX  = 2;  //Unix new line format (\n).
const NEWLINE_MAC   = 3;  //MacOS new line format (\r).

//AKD_GOTO type
const GT_LINE        = $1; //Go to "Line:Column".
const GT_OFFSETBYTE  = $2; //Go to offset counted in bytes.
const GT_OFFSETCHAR  = $4; //Go to offset counted in characters (not present in "Go to..." dialog).

//Caret options
const CO_CARETOUTEDGE          = $00000001;  //Allow caret moving out of the line edge.
const CO_CARETVERTLINE         = $00000002;  //Draw caret vertical line.
const CO_CARETACTIVELINE       = $00000004;  //Draw active line.
const CO_CARETACTIVELINEBORDER = $00000008;  //Draw active line border.

//Mouse options
const MO_LEFTMARGINSELECTION   = $00000001;  //Enables left margin line selection with mouse.
const MO_RICHEDITMOUSE         = $00000002;  //After WM_LBUTTONUP message capture operations doesn't stopped.
const MO_MOUSEDRAGGING         = $00000004;  //Enables OLE text dragging.
const MO_RCLICKMOVECARET       = $00000008;  //WM_RBUTTONDOWN message moves caret to a click position.
const MO_NONEWLINEMOUSESELECT  = $00000010;  //Triple click and left margin click selects only line contents without new line.
const MO_NOWHEELFONTCHANGE     = $00000020;  //Don't change font size with middle button scroll and Ctrl key.
const MO_MARGINSELUNWRAPLINE   = $00000040;  //Left margin line selection with mouse selects all wrapped line.
const MO_MBUTTONDOWNNOSCROLL   = $00000080;  //No scrolling after WM_MBUTTONDOWN message.

//Keyboard layout options
const KLO_REMEMBERLAYOUT     = $00000001;  //Remember keyboard layout for each tab (MDI).
const KLO_SWITCHLAYOUT       = $00000002;  //Auto switch keyboard layout according to text under caret.

//Paint options
const PAINT_PAINTGROUP            = $00000001;  //Paint text by group of characters (default is character by character).
                                                //With this flag some text recognition programs could start to work, printer could print faster, but highlighted symbols and combined unicode symbols can be drawn differently and editing of whose characters may become uncomfortable.
const PAINT_NONEWLINEDRAW         = $00000002;  //Disables drawing new line selection as space character.
const PAINT_ENTIRENEWLINEDRAW     = $00000004;  //Draw new line selection to the right edge.
const PAINT_HIDESEL               = $00000008;  //Hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus.
const PAINT_NOMARKERAFTERLASTLINE = $00000010;  //Disables marker painting after last line.
const PAINT_HIDENOSCROLL          = $00000100;  //Hides scroll bars instead of disabling them when they are not needed.
const PAINT_STATICEDGE            = $00000200;  //Draw thin edit window border.
const PAINT_NOEDGE                = $00000400;  //Draw no edit window border.

//Edit styles
const EDS_GLOBALUNDO              = $00000001;  //Use process heap for Undo/Redo instead of window heap. Required for AEM_DETACHUNDO and AEM_ATTACHUNDO.
                                                //Compatibility: define same as ES_SUNKEN.
const EDS_HEAPSERIALIZE           = $00000002;  //Mutual exclusion will be used when the heap functions allocate and free memory from window heap. Serialization of heap access allows two or more threads to simultaneously allocate and free memory from the same heap.
                                                //Compatibility: define same as ES_SAVESEL.

//Status bar position type
const SPT_COLUMN      = $00000001;  //"Line:Column". By default: "Line:Symbol".
const SPT_LINEWRAP    = $00000002;  //Wrap line numbers. By default: Non-wrap line numbers.

//Show modify
const SM_NONE           = $00000000;
const SM_STATUSBAR      = $00000001;
const SM_MAINTITLE_SDI  = $00000002;
const SM_FRAMETITLE_MDI = $00000004;
const SM_TABTITLE_MDI   = $00000008;

//StatusBar parts
const SBP_POSITION   = 0;
const SBP_MODIFY     = 1;
const SBP_INSERT     = 2;
const SBP_NEWLINE    = 3;
const SBP_CODEPAGE   = 4;
const SBP_USER       = 5;

//"Don't open a program twice" flags
const SOP_ON       = $00000001;  //"Don't open a program twice" is on.
const SOP_SAMEEXE  = $00000002;  //"Don't open a program twice" only if AkelPad executable is the same.

//Tab options MDI
const TAB_VIEW_NONE           = $00000001;  //Hide tab bar.
const TAB_VIEW_TOP            = $00000002;  //Show tab bar on top.
const TAB_VIEW_BOTTOM         = $00000004;  //Show tab bar at the bottom.
const TAB_TYPE_STANDARD       = $00000100;  //Standard tab bar style.
const TAB_TYPE_BUTTONS        = $00000200;  //Buttons tab bar style.
const TAB_TYPE_FLATBUTTONS    = $00000400;  //Flat buttons tab bar style.
const TAB_SWITCH_NEXTPREV     = $00010000;  //Switch between tabs: Next-Previous.
const TAB_SWITCH_RIGHTLEFT    = $00020000;  //Switch between tabs: Left-Right.
const TAB_ADD_AFTERCURRENT    = $00080000;  //Create tabs after the current one.
const TAB_NOADD_LBUTTONDBLCLK = $00100000;  //Don't create new tab by left button double click on the tab bar.
const TAB_NOADD_MBUTTONDOWN   = $00200000;  //Don't create new tab by middle button click on the tab bar.
const TAB_NODEL_LBUTTONDBLCLK = $00400000;  //Don't close tab by left button double click on the tab.
const TAB_NODEL_MBUTTONDOWN   = $00800000;  //Don't close tab by middle button click on the tab.

//File types association
const FTA_ASSOCIATE     = $00000001;  //Internal.
const FTA_DEASSOCIATE   = $00000002;  //Internal.
const FTA_OPEN          = $00000004;  //Open file types.
const FTA_EDIT          = $00000008;  //Edit file types.
const FTA_PRINT         = $00000010;  //Print file types.

//Color printing
const PRNC_TEXT         = $01;  //Print colored text.
const PRNC_BACKGROUND   = $02;  //Print on colored background.
const PRNC_SELECTION    = $04;  //Print text selection.

//Main menu
const MENU_FILE_POSITION     = 0;
const MENU_EDIT_POSITION     = 1;
const MENU_VIEW_POSITION     = 2;
const MENU_OPTIONS_POSITION  = 3;
const MENU_MDI_POSITION      = 4;
const MENU_ABOUT_POSITION    = 5;

//Popup menu
const MENU_POPUP_EDIT        = 0;
const MENU_POPUP_VIEW        = 1;
const MENU_POPUP_CODEPAGE    = 2;
const MENU_POPUP_HEADLINE    = 3;

//Submenu position
const MENU_FILE_RECENTFILES_3X   = 12;
const MENU_FILE_RECENTFILES_4X   = 14;
const MENU_VIEW_LANGUAGE_3X      = 11;
const MENU_VIEW_LANGUAGE_4X      = 9;
const MENU_POPUP_CODEPAGE_OPEN   = 0;
const MENU_POPUP_CODEPAGE_SAVE   = 2;

//INI value types
const INI_DWORD           = 1;
const INI_BINARY          = 2;
const INI_STRINGANSI      = 3;
const INI_STRINGUNICODE   = 4;

//Options flags
const POB_READ     = $01;  //Begins read options.
const POB_SAVE     = $02;  //Begins save options.
const POB_CLEAR    = $04;  //Begins new save options (POB_SAVE|POB_CLEAR).
const POB_SCRIPTS  = $10;  //Begins Scripts plugin options.

//Option type
const PO_DWORD     = 1;   //32-bit number.
const PO_BINARY    = 2;   //Binary data in any form.
const PO_STRING    = 3;   //Null-terminated string.
const PO_REMOVE    = 10;  //Delete option.

//Support flags
const PDS_SUPPORTALL   = $00000000;  //Function support everything (default).
const PDS_NOAUTOLOAD   = $00000001;  //Function doesn't support autoload.
const PDS_NOANSI       = $00000002;  //Function doesn't support ansi API and can be executed only on unicode Windows (WinNT/2000/XP/2003/Vista/Seven).
const PDS_NOUNICODE    = $00000004;  //Function doesn't support unicode API.
const PDS_NOSDI        = $00000008;  //Function doesn't support SDI mode.
const PDS_NOMDI        = $00000010;  //Function doesn't support MDI mode.
const PDS_NOPMDI       = $00000020;  //Function doesn't support PMDI mode.
const PDS_NORICHEDIT   = $00000040;  //Reserved.
const PDS_GETSUPPORT   = $10000000;  //Flag is set if caller wants to get PDS_* flags without function execution.
const PDS_STRANSI      = $20000000;  //Flag is set if caller passes Ansi strings in external call arguments (PLUGINDATA.lParam).
const PDS_STRWIDE      = $40000000;  //Flag is set if caller passes Unicode strings in external call arguments (PLUGINDATA.lParam).
                                     //If PDS_STRANSI and PDS_STRWIDE not specified then one of these flags will be set automatically depending on Windows version.

//AKD_DLLCALL flags
const DLLCF_ONPROGRAMLOAD   = $001;  //Don't use it. For internal code only.
const DLLCF_SWITCHAUTOLOAD  = $004;  //If function running after call then turn on autoload, if not then turn off autoload.
const DLLCF_SAVENOW         = $008;  //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_NOW after switching autoload flag.
const DLLCF_SAVEONEXIT      = $010;  //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_ONEXIT after switching autoload flag.

//AKD_DLLSAVE flags
const DLLSF_NOW     = $1;  //Save plugins stack immediately.
const DLLSF_ONEXIT  = $2;  //Save plugins stack on program exit.

//AKD_TRANSLATEMESSAGE types
const TMSG_GLOBAL       = $01;  //Translate hotkey from global accelerator table (PLUGINDATA.hGlobalAccel).
const TMSG_DIALOG       = $02;  //Translate message from modeless (see AKD_SETMODELESS) or dockable dialog (see AKD_DOCK).
const TMSG_PLUGIN       = $04;  //Translate plugin message (see AKD_DLL*, AKD_CALLPROC, AKD_POSTMESSAGE).
const TMSG_HOTKEY       = $08;  //Translate plugin hotkey.
const TMSG_ACCELERATOR  = $10;  //Translate hotkey from main accelerator table (PLUGINDATA.hMainAccel).
const TMSG_DEFAULT      = $20;  //Default message processing.

const TMSG_ALL  = (TMSG_GLOBAL      or
                   TMSG_DIALOG      or
                   TMSG_PLUGIN      or
                   TMSG_HOTKEY      or
                   TMSG_ACCELERATOR or
                   TMSG_DEFAULT);

//Context menu owner
const NCM_EDIT     = 1;  //Edit control.
const NCM_TAB      = 2;  //Tab control.
const NCM_STATUS   = 3;  //Status bar control.

//AKD_FRAMEFIND flags
const FWF_CURRENT        = 1;  //Retrieve current frame data pointer. lParam not used.
const FWF_NEXT           = 2;  //Retrieve next frame data pointer in frame stack. lParam is a frame data pointer.
const FWF_PREV           = 3;  //Retrieve previous frame data pointer in frame stack. lParam is a frame data pointer.
const FWF_BYINDEX        = 4;  //Retrieve frame data by index in frame stack. lParam is frame index. First frame has index 1, last -1.
const FWF_BYFILENAME     = 5;  //Retrieve frame data by full file name. lParam is full file name string.
                              // For AKD_FRAMEFINDA string is ansi.
                              // For AKD_FRAMEFINDW string is unicode.
const FWF_BYEDITWINDOW   = 6;  //Retrieve frame data by edit window handle. lParam is edit window handle or NULL for current edit window handle.
const FWF_BYEDITDOCUMENT = 7;  //Retrieve frame data by edit document handle. lParam is edit document handle or NULL for current edit document handle.
const FWF_BYTABINDEX     = 8;  //Retrieve frame data by tab item index. lParam is tab item index.
const FWF_TABNEXT        = 9;  //Retrieve next tab item frame data. lParam is a frame data pointer.
const FWF_TABPREV        = 10; //Retrieve previous tab item frame data. lParam is a frame data pointer.

//AKD_FRAMEACTIVATE and AKDN_FRAME_ACTIVATE flags
const FWA_NOUPDATEORDER         = $00000001;  //Don't update access order during activating.
const FWA_NOUPDATEEDIT          = $00000002;  //For WMD_PMDI mode. Don't redraw edit control area during activating.
const FWA_NOVISUPDATE           = $00000004;  //For WMD_PMDI mode. Don't make any visual updates during activating. Use it only if you later will activate back frame that has lost active status.
                                              //AKDN_FRAME_ACTIVATE only flags:
const FWA_NOTIFY_CREATE         = $00000100;  //Frame activating after creation.
const FWA_NOTIFY_BEFOREDESTROY  = $00000200;  //Frame activating for destroying.
const FWA_NOTIFY_AFTERDESTROY   = $00000400;  //Previous frame activating because current one has been destroyed.
const FWA_NOTIFY_REOPEN         = $00000800;  //Frame activating before reopening document.

//AKD_FRAMEDESTROY return value
const FWDE_SUCCESS   = 0;  //Operation is successful.
const FWDE_ABORT     = 1;  //Operation is aborted by user.
const FWDE_LASTTAB   = 2;  //For WMD_PMDI mode. Last tab cannot be destroyed.
const FWDE_NOWINDOW  = 3;  //Frame doesn't have associated edit window (FRAMEDATA.ei.hWndEdit == NULL).

//AKD_FRAMESTATS flags
const FWS_COUNTALL      = 0;  //Count of windows.
const FWS_COUNTMODIFIED = 1;  //Count of modified windows.
const FWS_COUNTSAVED    = 2;  //Count of unmodified windows.
const FWS_CURSEL        = 3;  //Active window zero based index.

//Lock inherit new document settings from current document
const LI_FONT           = $00000001;  //Lock inherit font.
const LI_COLORS         = $00000002;  //Lock inherit colors.
const LI_BKIMAGE        = $00000004;  //Lock inherit background image.
const LI_WRAP           = $00000008;  //Lock inherit wrapping.

//Find/Replace flags
const FRF_DOWN               = $00000001;  //Same as AEFR_DOWN.
const FRF_WHOLEWORD          = $00000002;  //Same as AEFR_WHOLEWORD.
const FRF_MATCHCASE          = $00000004;  //Same as AEFR_MATCHCASE.
const FRF_WHOLEWORDGOODSTART = $00000010;  //Internal.
const FRF_WHOLEWORDGOODEND   = $00000020;  //Internal.
const FRF_FINDFROMREPLACE    = $00000040;  //Internal.
const FRF_REGEXPNONEWLINEDOT = $00040000;  //Symbol . specifies any character except new line. Uses with FRF_REGEXP.
const FRF_REGEXP             = $00080000;  //Same as AEFR_REGEXP.
const FRF_UP                 = $00100000;
const FRF_BEGINNING          = $00200000;
const FRF_SELECTION          = $00400000;
const FRF_ESCAPESEQ          = $00800000;
const FRF_ALLFILES           = $01000000;
const FRF_REPLACEALLANDCLOSE = $02000000;
const FRF_CHECKINSELIFSEL    = $04000000;
const FRF_CYCLESEARCH        = $08000000;
const FRF_CYCLESEARCHPROMPT  = $10000000;
const FRF_REPLACEALLNOMSG    = $20000000;

//AKD_PASTE
const PASTE_ANSI       = $00000001;  //Paste text as ANSI. Default is paste as Unicode text, if no Unicode text available ANSI text will be used.
const PASTE_COLUMN     = $00000002;  //Paste to column selection.
const PASTE_AFTER      = $00001000;  //Paste text after caret.
const PASTE_SINGLELINE = $00002000;  //Paste multiline text to single line edit control. All new lines replaced with '\r'.

//AKD_RECODESEL flags
const RCS_DETECTONLY   = $00000001;  //Don't do text replacement, only detect codepages.

//AKD_GETMODELESS types
const MLT_NONE     = 0; //No registered modeless dialog open.
const MLT_CUSTOM   = 1; //Dialog registered with AKD_SETMODELESS.
const MLT_RECODE   = 2; //Recode dialog.
const MLT_FIND     = 3; //Find dialog.
const MLT_REPLACE  = 4; //Replace dialog.
const MLT_GOTO     = 5; //Go to dialog.

//DIALOGRESIZEMSG flags
const DRM_PAINTSIZEGRIP = $2; //Draw resize grid.

//DIALOGRESIZE type
const DRS_SIZE  = $1; //Resize control. Can be combined with DRS_X ot DRS_Y.
const DRS_MOVE  = $2; //Move control. Can be combined with DRS_X ot DRS_Y.
const DRS_X     = $4; //X value. Can be combined with DRS_SIZE ot DRS_MOVE.
const DRS_Y     = $8; //Y value. Can be combined with DRS_SIZE ot DRS_MOVE.

//Dock side
const DKS_LEFT    = 1;
const DKS_RIGHT   = 2;
const DKS_TOP     = 3;
const DKS_BOTTOM  = 4;

//Dock flags
const DKF_OWNTHREAD     = $00000001;  //Dock window has its own thread.
const DKF_FIXEDSIZE     = $00000002;  //Dock window isn't resizeable.
const DKF_DRAGDROP      = $00000004;  //Dock window has drag-and-drop support, the DOCK.rcDragDrop member is valid.
const DKF_HIDDEN        = $00000008;  //Dock window isn't visible.
const DKF_NODROPLEFT    = $00000010;  //Disable drag-and-drop to the left side.
const DKF_NODROPRIGHT   = $00000020;  //Disable drag-and-drop to the right side.
const DKF_NODROPTOP     = $00000040;  //Disable drag-and-drop to the top side.
const DKF_NODROPBOTTOM  = $00000080;  //Disable drag-and-drop to the bottom side.

//Dock action
const DK_ADD         = $00000001;  //Add new dockable window.
const DK_DELETE      = $00000002;  //Delete dockable window.
const DK_SUBCLASS    = $00000004;  //Assign dockable window handle (DOCK.hWnd).
const DK_UNSUBCLASS  = $00000008;  //Deassign dockable window handle (DOCK.hWnd).
const DK_SETLEFT     = $00000010;  //Set DKS_LEFT side.
const DK_SETRIGHT    = $00000020;  //Set DKS_RIGHT side.
const DK_SETTOP      = $00000040;  //Set DKS_TOP side.
const DK_SETBOTTOM   = $00000080;  //Set DKS_BOTTOM side.
const DK_HIDE        = $00000100;  //Hide dockable window and set DKF_HIDDEN flag.
const DK_SHOW        = $00000200;  //Show dockable window and remove DKF_HIDDEN flag.
const DK_FINDDOCK    = $00000400;  //Find dock by dockable window handle (DOCK.hWnd). Cannot be combined with DK_FINDCHILD.
const DK_FINDCHILD   = $00000800;  //Find dock by dockable window or its child handle (DOCK.hWnd). Cannot be combined with DK_FINDDOCK.

//Dock capture
const DKC_SIZING     = 1;
const DKC_DRAGDROP   = 2;

//AKD_SETBUTTONDRAW flags
const BIF_BITMAP      = $001; //Bitmap handle is used in BUTTONDRAW.hImage.
const BIF_ICON        = $002; //Icon handle is used in BUTTONDRAW.hImage.
const BIF_CROSS       = $004; //Draw small cross 8x7. BUTTONDRAW.hImage is ignored.
const BIF_DOWNARROW   = $008; //Draw small down arrow 7x4. BUTTONDRAW.hImage is ignored.
const BIF_UPARROW     = $010; //Draw small up arrow 7x4. BUTTONDRAW.hImage is ignored.
const BIF_LEFTARROW   = $020; //Draw small left arrow 4x7. BUTTONDRAW.hImage is ignored.
const BIF_RIGHTARROW  = $040; //Draw small right arrow 4x7. BUTTONDRAW.hImage is ignored.
const BIF_ETCHED      = $100; //Draw edge around button.
const BIF_ENABLEFOCUS = $200; //Draw focus rectangle when button receive focus.

//BUTTONMESSAGEBOX flags
const BMB_DEFAULT   = $001;  //Default button.
const BMB_DISABLED  = $002;  //Button is disabled.

//WM_INITMENU lParam
const IMENU_EDIT     = $00000001;
const IMENU_CHECKS   = $00000004;


////
type
  PHWND = ^HWND;
  // Fr0sT: these WinAPI types aren't defined in RTL until D2009 (?)
  {$IF CompilerVersion < 20}
    INT_PTR = Integer;
    LONG_PTR = Integer;
    UINT_PTR = Cardinal;
    ULONG_PTR = LongWord;
    DWORD_PTR = ULONG_PTR;
    HANDLE_PTR = type LongWord;
  {$IFEND}

//// Structures

type
  PHSTACK = ^THSTACK;
  _HSTACK = record
    first: INT_PTR;
    last: INT_PTR;
  end;
  THSTACK = _HSTACK;

type
  PAELINEDATA = ^TAELINEDATA;
  _AELINEDATA = record
    next: PAELINEDATA;          //Pointer to the next AELINEDATA structure.
    prev: PAELINEDATA;          //Pointer to the previous AELINEDATA structure.
    wpLine: PWideChar;          //Text of the line, terminated with NULL character.
    nLineLen: Integer;          //Length of the wpLine, not including the terminating NULL character.
    nLineBreak: BYTE;           //New line: AELB_EOF, AELB_R, AELB_N, AELB_RN, AELB_RRN or AELB_WRAP.
    nLineFlags: BYTE;           //Reserved.
    nReserved: WORD;            //Reserved.
    nLineWidth: Integer;        //Width of the line in pixels.
    nSelStart: Integer;         //Selection start character position in line.
    nSelEnd: Integer;           //Selection end character position in line.
  end;
  TAELINEDATA = _AELINEDATA;


type
  _AELINEINDEX = record
    nLine: Integer;
    lpLine: PAELINEDATA;
  end;
  TAELINEINDEX = _AELINEINDEX;

type
  PAECHARINDEX = ^TAECHARINDEX;
  _AECHARINDEX = record
    nLine: Integer;
    lpLine: PAELINEDATA;
    nCharInLine: Integer;
  end;
  TAECHARINDEX = _AECHARINDEX;

  PAECHARRANGE = ^TAECHARRANGE;
  _AECHARRANGE = record
    ciMin : TAECHARINDEX;
    ciMax : TAECHARINDEX;
  end;
  TAECHARRANGE = _AECHARRANGE;

  _AECOLORS = record
    dwFlags : DWORD;
    crCaret : TCOLORREF;
    crBasicText : TCOLORREF;
    crBasicBk : TCOLORREF;
    crSelText : TCOLORREF;
    crSelBk : TCOLORREF;
    crActiveLineText : TCOLORREF;
    crActiveLineBk : TCOLORREF;
    crUrlText : TCOLORREF;
    crActiveColumn : TCOLORREF;
    crColumnMarker : TCOLORREF;
    crUrlCursorText : TCOLORREF;
    crUrlVisitText : TCOLORREF;
    crActiveLineBorder : TCOLORREF;
    crAltLineText : TCOLORREF;
    crAltLineBk : TCOLORREF;
    crAltLineBorder : TCOLORREF;
  end;
  TAECOLORS = _AECOLORS;

type
  AEHDOC = THandle;

type
  // hDoc        Document handle returned by AEM_GETDOCUMENT or AEM_CREATEDOCUMENT.
  // uMsg        Message ID for example EM_SETSEL.
  // lParam      Additional parameter.
  // wParam      Additional parameter.
  //
  // Return Value
  //   Depends on message.
  TAEEditProc = function (hDoc: AEHDOC; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;
  TWndProc = function (hWnd: HWND; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;

type
  HINIFILE = THandle;
type
  HINISECTION = THandle;
type
  HINIKEY = THandle;

type
  TPluginProc = function(): BOOL; stdcall;
  TWndProcRet = procedure(lpWNDProcRet: PCWPRETSTRUCT); stdcall;


type
  _EDITINFO = record
    hWndEdit: HWND;           //Edit window.
    hDocEdit: AEHDOC;         //Edit document.
    pFile: PBYTE;             //Current editing cFile.
    szFile: PAnsiChar;        //Current editing cFile (Ansi).
    wszFile: PWideChar;       //Current editing cFile (Unicode).
    nCodePage: Integer;       //Current code page.
    bBOM: BOOL;               //Current BOM.
    nNewLine: Integer;        //Current new line format, see NEWLINE_* defines.
    bModified: BOOL;          //File has been modified.
    bReadOnly: BOOL;          //Read only.
    bWordWrap: BOOL;          //Word wrap.
    bOvertypeMode: BOOL;      //Overtype mode.
    hWndMaster: HWND;         //Master window.
    hDocMaster: AEHDOC;       //Master document.
    hWndClone1: HWND;         //First clone window.
    hDocClone1: AEHDOC;       //First clone document.
    hWndClone2: HWND;         //Second clone window.
    hDocClone2: AEHDOC;       //Second clone document.
    hWndClone3: HWND;         //Third clone window.
    hDocClone3: AEHDOC;       //Third clone document.
  end;
  TEDITINFO = _EDITINFO;


type
  PRECENTCARETITEM = ^TRECENTCARETITEM;
  _RECENTCARETITEM = record
    next: PRECENTCARETITEM;
    prev: PRECENTCARETITEM;
    nCaretOffset: INT_PTR;
  end;
  TRECENTCARETITEM = _RECENTCARETITEM;


type
  _STACKRECENTCARET = record
    first: PRECENTCARETITEM;
    last: PRECENTCARETITEM;
  end;
  TSTACKRECENTCARET = _STACKRECENTCARET;


type
  PFRAMEDATA = ^TFRAMEDATA;
  _FRAMEDATA = record
    next: PFRAMEDATA;
    prev: PFRAMEDATA;
    //Edit state external
    hWndEditParent : HWND;                                 //Edit parent window.
    ei : TEDITINFO;                                        //Edit info.
    szFile : array[0..MAX_PATH-1] of AnsiChar;             //Frame cFile (Ansi).
    wszFile : array[0..MAX_PATH-1] of WideChar;            //Frame cFile (Unicode).
    nFileLen : Integer;                                    //Frame cFile length.
    nStreamOffset: Integer;                                //":" symbol offset in FRAMEDATA.wszFile.
    hIcon : HICON;                                         //Frame icon.
    nIconIndex : Integer;                                  //Frame ImageList icon index.
    rcEditWindow : TRECT;                                  //Edit TRect. rcEditWindow.right - is width and rcEditWindow.bottom is height.
    rcMasterWindow : TRECT;                                //Master window TRect. rcMasterWindow.right - is width and rcMasterWindow.bottom is height.
    //Edit settings (AkelPad)
    dwLockInherit: DWORD;                                  //See LI_* defines.
    lf : LOGFONTW;                                         //Edit font.
    bTabStopAsSpaces : BOOL;                               //Insert tab stop as spaces.
    dwCaretOptions : DWORD;                                //See CO_* defines.
    dwMouseOptions : DWORD;                                //See MO_* defines.
    nClickURL : Integer;                                   //Number of clicks to open URL.
    bUrlPrefixesEnable : BOOL;                             //URL prefixes enable.
    bUrlDelimitersEnable : BOOL;                           //URL delimiters enable.
    bWordDelimitersEnable : BOOL;                          //Word delimiters enabled.
    bWrapDelimitersEnable : BOOL;                          //Wrap delimiters enabled.
    dwMappedPrintWidth : DWORD;                            //Mapped print page width.
    //Edit settings (AkelPad)
    rcEditMargins : TRECT;                                 //Edit margins.
    nTabStopSize : Integer;                                //Tab stop size.
    nUndoLimit : Integer;                                  //Undo limit.
    bDetailedUndo : BOOL;                                  //Detailed undo.
    dwWrapType : DWORD;                                    //Wrap cType AEWW_WORD or AEWW_SYMBOL.
    dwWrapLimit : DWORD;                                   //Wrap characters limit, zero if wrap by window edge.
    dwMarker : DWORD;                                      //Vertical marker, zero if no marker set.
    nCaretWidth : Integer;                                 //Caret width.
    dwAltLineFill : DWORD;                                 //Alternating lines fill interval.
    dwAltLineSkip : DWORD;                                 //Alternating lines skip interval.
    bAltLineBorder : BOOL;                                 //Draw alternating lines border.
    dwLineGap : DWORD;                                     //Line gap.
    bShowURL : BOOL;                                       //Show URL.
    wszUrlPrefixes : array[0..URL_PREFIXES_SIZE-1] of WideChar;          //URL prefixes.
    wszUrlLeftDelimiters : array[0..URL_DELIMITERS_SIZE-1] of WideChar;  //URL left delimiters.
    wszUrlRightDelimiters : array[0..URL_DELIMITERS_SIZE-1] of WideChar; //URL right delimiters.
    wszWordDelimiters : array[0..WORD_DELIMITERS_SIZE-1] of WideChar;    //Word delimiters.
    wszWrapDelimiters : array[0..WRAP_DELIMITERS_SIZE-1] of WideChar;    //Wrap delimiters.
    wszBkImageFile: array[0..MAX_PATH-1] of WideChar;                   //Background image file.
    nBkImageAlpha: Integer;                                  //Alpha transparency value that ranges from 0 to 255.
    hBkImageBitmap: HBITMAP;                             //Background image handle.
    aec : TAECOLORS;                                        //Edit colors.
    //Edit state internal. AKD_FRAMEINIT not copy data below.
    lpEditProc : TAEEditProc;                              //Edit window procedure.
    ft : FILETIME;                                         //cFile time.
    dwInputLocale : HKL;                                   //Keyboard layout.
    hRecentCaretStack: TSTACKRECENTCARET;                 //Recent caret stack.
    lpCurRecentCaret: PRECENTCARETITEM;                  //Current recent caret position.
    //Find/Replace
    nCompileErrorOffset: INT_PTR;                        //Contain pattern offset, if error occurred during compile pattern.
    bCompileErrorReplace: BOOL;                          //TRUE - error in "ReplaceWith" complitaion, FALSE - error in "FindIt" complitaion.
  
    //Statusbar
    crPrevSel : TAECHARRANGE;
    nSelSubtract : INT_PTR;
    nCaretRichOffset : INT_PTR;
    nCaretByteOffset : INT_PTR;
    nCaretChar : Integer;
    nCaretLine: Integer;
    nCaretColumn: Integer;
    nLineCountAll: Integer;
    nLineCountSel: Integer;
    nLineSelBegin: Integer;
    nLineSelEnd: Integer;
    nRichCount : INT_PTR;
    nFontPoint : Integer;
    bCapsLock : BOOL;
    bNumLock : BOOL;
    bReachedEOF : BOOL;
    nReplaceCount : INT_PTR;
  end;
  TFRAMEDATA = _FRAMEDATA;

type
  _PLUGINVERSION = record
    cb: DWORD;                   //Size of the structure.
    hMainWnd: HWND;              //Main window.
    dwAkelDllVersion: DWORD;     //Current AkelDLL version. Set it to AKELDLL.
    dwExeMinVersion3x: DWORD;    //Required minimum AkelPad 3.x version.
    dwExeMinVersion4x: DWORD;    //Required minimum AkelPad 4.x version.
    pPluginName: PAnsiChar;      //Plugin unique name.
  end;
  TPLUGINVERSION = _PLUGINVERSION;


type
  PPLUGINFUNCTION = ^TPLUGINFUNCTION;
  _PLUGINFUNCTION = record
    next: PPLUGINFUNCTION;
    prev: PPLUGINFUNCTION;
    pFunction: PBYTE;               //Function name, format "Plugin::Function".
                                    //  const char *pFunction      if bOldWindows == TRUE
                                    //  const wchar_t *pFunction   if bOldWindows == FALSE
    szFunction : array[0..MAX_PATH-1] of AnsiChar;      //cFunction name (Ansi).
    wszFunction : array[0..MAX_PATH-1] of WideChar;     //cFunction name (Unicode).
    nFunctionLen: Integer;          //Function name length.
    wHotkey: WORD;                  //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
    bAutoLoad: BOOL;                //TRUE  if function has autoload flag.
                                    //FALSE if function has no autoload flag.
    bRunning: BOOL;                 //Function is running.
    PluginProc: TPLUGINPROC;        //Function procedure.
    lpParameter: Pointer;           //Procedure parameter.
    nRefCount: Integer;             //Internal.
  end;
  TPLUGINFUNCTION = _PLUGINFUNCTION;


type
  _PLUGINDATA = record
    cb: DWORD;                         //Size of the structure.
    dwSupport: DWORD;                  //If (dwSupport & PDS_GETSUPPORT) != 0, then caller wants to get PDS_* flags without function execution.
    pFunction: PBYTE;                  //Called cFunction name, format "Plugin::cFunction".
                                       //  const char *pFunction     if bOldWindows == TRUE
                                       //  const wchar_t *pFunction  if bOldWindows == FALSE
    szFunction: PAnsiChar;             //Called cFunction name (Ansi).
    wszFunction: PWideChar;            //Called cFunction name (Unicode).
    hInstanceDLL: HINST;               //DLL instance.
    lpPluginFunction: PPLUGINFUNCTION; //Pointer to a PLUGINFUNCTION structure.
    nUnload: Integer;                  //See UD_* defines.
    bInMemory: BOOL;                   //Plugin already loaded.
    bOnStart: BOOL;                    //Indicates when function has been called:
                                       //  TRUE  if function called on start-up.
                                       //  FALSE if function called manually.
    lParam: LPARAM;                    //Input data.
    pAkelDir: PBYTE;                   //AkelPad directory.
                                       //  const char *pAkelDir      if bOldWindows == TRUE
                                       //  const wchar_t *pAkelDir   if bOldWindows == FALSE
    szAkelDir: PAnsiChar;              //AkelPad directory (Ansi).
    wszAkelDir: PWideChar;             //AkelPad directory (Unicode).
    hInstanceEXE: HINST;               //EXE instance.
    hPluginsStack: PHSTACK;            //Pointer to a plugins stack with PLUGINFUNCTION elements.
    nSaveSettings: Integer;            //See SS_* defines.
    hMainWnd: HWND;                    //Main window.
    lpFrameData: PFRAMEDATA;           //Pointer to a current FRAMEDATA structure.
    hWndEdit: HWND;                    //Edit window.
    hDocEdit: AEHDOC;                  //Edit document.
    hStatus: HWND;                     //StatusBar window.
    hMdiClient: HWND;                  //MDI client window (if nMDI == WMD_MDI).
    hTab: HWND;                        //Tab window        (if nMDI == WMD_MDI || nMDI == WMD_PMDI).
    hMainMenu: HMENU;                  //Main menu.
    hMenuRecentFiles: HMENU;           //Recent files menu.
    hMenuLanguage: HMENU;              //Language menu.
    hPopupMenu: HMENU;                 //Right click menu.
    hMainIcon: HICON;                  //Main window icon handle.
    hGlobalAccel: HACCEL;              //Global accelerator table (highest priority).
    hMainAccel: HACCEL;                //Main accelerator table (lowest priority).
    bOldWindows: BOOL;                 //Non-Unicode Windows.
    bOldRichEdit: BOOL;                //Riched20.dll lower then 5.30 (v3.0).
    bOldComctl32: BOOL;                //Comctl32.dll lower then 4.71.
    bAkelEdit: BOOL;                   //AkelEdit control is used.
    nMDI: Integer;                     //Window mode, see WMD_* defines.
    pLangModule: PBYTE;                //Language module.
                                       //  const char *pLangModule      if bOldWindows == TRUE
                                       //  const wchar_t *pLangModule   if bOldWindows == FALSE
    szLangModule: PAnsiChar;           //Language module (Ansi).
    wszLangModule: PWideChar;          //Language module (Unicode).
    wLangSystem: LANGID;               //System language ID.
    wLangModule: LANGID;               //Language module language ID.
  end;
  TPLUGINDATA = _PLUGINDATA;


type
  _UNISTRING = record
    pString: PBYTE;         //Depend on Windows. Win9x or WinNT.
    szString: PAnsiChar;    //Ansi cString.
    wszString: PWideChar;   //Unicode cString.
  end;
  TUNISTRING = _UNISTRING;


type
  _DETECTANSITEXT = record
    dwLangID: DWORD;         //Codepage recognition language defined as LANGID. If -1, then use current settings.
    pText: PAnsiChar;        //Ansi text.
    nTextLen: INT_PTR;       //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nMinChars: INT_PTR;      //Minimum detect characters representation. If zero, default number is used.
    nCodePage: Integer;      //Result: detected Ansi codepage.
  end;
  TDETECTANSITEXT = _DETECTANSITEXT;


type
  _DETECTUNITEXT = record
    dwLangID: DWORD;         //Codepage recognition language defined as LANGID. If -1, then use current settings.
    wpText: PWideChar;       //Unicode text.
    nTextLen: INT_PTR;       //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nMinChars: INT_PTR;      //Minimum detect characters representation. If zero, default number is used.
    nCodePageFrom: Integer;  //Result: codepage that converts text to Ansi without character lost.
    nCodePageTo: Integer;    //Result: detected Ansi codepage.
  end;
  TDETECTUNITEXT = _DETECTUNITEXT;


type
  _CONVERTANSITEXT = record
    pInput: PAnsiChar;       //Ansi text.
    nInputLen: INT_PTR;      //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nCodePageFrom: Integer;  //Codepage for Ansi to Unicode conversion.
    nCodePageTo: Integer;    //Codepage for Unicode to Ansi conversion.
    szOutput: PAnsiChar;     //Result: pointer that receive allocated text. Must be deallocated with AKD_FREETEXT message.
    nOutputLen: INT_PTR;     //Result: text length.
  end;
  TCONVERTANSITEXT = _CONVERTANSITEXT;


type
  _CONVERTUNITEXT = record
    wpInput: PWideChar;      //Unicode text.
    nInputLen: INT_PTR;      //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nCodePageFrom: Integer;  //Codepage for Unicode to Ansi conversion.
    nCodePageTo: Integer;    //Codepage for Ansi to Unicode conversion.
    wszOutput: PWideChar;    //Result: pointer that receive allocated text. Must be deallocated with AKD_FREETEXT message.
    nOutputLen: INT_PTR;     //Result: text length.
  end;
  TCONVERTUNITEXT = _CONVERTUNITEXT;


type
  _DETECTFILEA = record
    pFile: PAnsiChar;       //cFile to detect.
    dwBytesToCheck: DWORD;  //How many bytes will be checked.
    dwFlags: DWORD;         //See ADT_* defines.
    nCodePage: Integer;     //Detected codepage.
    bBOM: BOOL;             //Detected BOM.
  end;
  TDETECTFILEA = _DETECTFILEA;


type
  _DETECTFILEW = record
    pFile: PWideChar;       //cFile to detect.
    dwBytesToCheck: DWORD;  //How many bytes will be checked.
    dwFlags: DWORD;         //See ADT_* defines.
    nCodePage: Integer;     //Detected codepage.
    bBOM: BOOL;             //Detected BOM.
  end;
  TDETECTFILEW = _DETECTFILEW;


type
  _FILECONTENT = record
    hFile: THandle;         //cFile handle, returned by CreateFile function.
    dwMax: UINT_PTR;        //AKD_READFILECONTENT: maximum bytes to read, if -1 read entire file.
                            //AKD_WRITEFILECONTENT: wpContent length in characters. If this value is -1, the wpContent is assumed to be null-terminated and the length is calculated automatically.
    nCodePage: Integer;     //File codepage.
    bBOM: BOOL;             //File BOM.
    wpContent: PWideChar;   //AKD_READFILECONTENT: returned file contents.
                            //AKD_WRITEFILECONTENT: text to save.
  end;
  TFILECONTENT = _FILECONTENT;


type
  _OPENDOCUMENTA = record
    pFile: PAnsiChar;       //cFile to open.
    pWorkDir: PAnsiChar;    //Set working directory before open, if NULL then don't set.
    dwFlags: DWORD;         //Open flags. See OD_* defines.
    nCodePage: Integer;     //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
    bBOM: BOOL;             //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
  end;
  TOPENDOCUMENTA = _OPENDOCUMENTA;


type
  _OPENDOCUMENTW = record
    pFile: PWideChar;       //cFile to open.
    pWorkDir: PWideChar;    //Set working directory before open, if NULL then don't set.
    dwFlags: DWORD;         //Open flags. See OD_* defines.
    nCodePage: Integer;     //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
    bBOM: BOOL;             //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
  end;
  TOPENDOCUMENTW = _OPENDOCUMENTW;


type
  _OPENDOCUMENTPOSTA = record
    hWnd: HWND;             //Window to fill in, NULL for current edit window.
    dwFlags: DWORD;         //Open flags. See OD_* defines.
    nCodePage: Integer;     //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
    bBOM: BOOL;             //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
    szFile : array[0..MAX_PATH-1] of AnsiChar;     //cFile to open.
    szWorkDir : array[0..MAX_PATH-1] of AnsiChar;  //Set working directory before open;
  end;
  TOPENDOCUMENTPOSTA = _OPENDOCUMENTPOSTA;


type
  _OPENDOCUMENTPOSTW = record
    hWnd: HWND;             //Window to fill in, NULL for current edit window.
    dwFlags: DWORD;         //Open flags. See OD_* defines.
    nCodePage: Integer;     //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
    bBOM: BOOL;             //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
    szFile : array[0..MAX_PATH-1] of  WideChar;     //cFile to open.
    szWorkDir : array[0..MAX_PATH-1] of  WideChar;  //Set working directory before open;
  end;
  TOPENDOCUMENTPOSTW = _OPENDOCUMENTPOSTW;


type
  _SAVEDOCUMENTA = record
    pFile: PAnsiChar;       //cFile to save.
    nCodePage: Integer;     //File code page.
    bBOM: BOOL;             //File BOM.
    dwFlags: DWORD;         //See SD_* defines.
  end;
  TSAVEDOCUMENTA = _SAVEDOCUMENTA;


type
  _SAVEDOCUMENTW = record
    pFile: PWideChar;       //cFile to save.
    nCodePage: Integer;     //File code page.
    bBOM: BOOL;             //File BOM.
    dwFlags: DWORD;         //See SD_* defines.
  end;
  TSAVEDOCUMENTW = _SAVEDOCUMENTW;


//AKD_SETFRAMEINFO
type
  _FRAMEINFO = record
    nType: Integer;        //See FIS_* defines.
    dwData: UINT_PTR;  //Depend on FIS_* define.
  end;
  TFRAMEINFO = _FRAMEINFO;


type
  _BKIMAGE = record
    wpFile: PWideChar; //Background image file.
    nAlpha: Integer;   //Alpha transparency value that ranges from 0 to 255.
  end;
  TBKIMAGE = _BKIMAGE;


type
  PWNDPROCDATA = ^TWNDPROCDATA;
  _WNDPROCDATA = record
    next: PWNDPROCDATA;
    prev: PWNDPROCDATA;
    CurProc  : TWndProc;
    NextProc : TWndProc;
    PrevProc : TWndProc;
  end;
  TWNDPROCDATA = _WNDPROCDATA;


type
  PWNDPROCRETDATA = ^TWNDPROCRETDATA;
  _WNDPROCRETDATA = record
    next: PWNDPROCRETDATA;
    prev: PWNDPROCRETDATA;
    CurProc  : TWndProcRet;
    NextProc : TWndProcRet;
    PrevProc : TWndProcRet;
  end;
  TWNDPROCRETDATA = _WNDPROCRETDATA;


type
  _PLUGINADDA = record
    pFunction: PAnsiChar;      //cFunction name, format "Plugin::cFunction".
    wHotkey: WORD;               //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
    bAutoLoad: BOOL;             //TRUE  if function has autoload flag.
    PluginProc: TPluginProc;      //Function procedure.
    lpParameter: Pointer;          //Procedure parameter.
  end;
  TPLUGINADDA = _PLUGINADDA;


type
  _PLUGINADDW = record
    pFunction: PWideChar;   //cFunction name, format "Plugin::cFunction".
    wHotkey: WORD;               //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
    bAutoLoad: BOOL;             //TRUE  if function has autoload flag.
    PluginProc: TPLUGINPROC;      //Function procedure.
    lpParameter: Pointer;          //Procedure parameter.
  end;
  TPLUGINADDW = _PLUGINADDW;


type
  _PLUGINCALLSENDA = record
    pFunction: PAnsiChar;      //cFunction name, format "Plugin::cFunction".
    lParam: LPARAM;              //Input data.
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLSENDA = _PLUGINCALLSENDA;


type
  _PLUGINCALLSENDW = record
    pFunction: PWideChar;   //cFunction name, format L"Plugin::cFunction".
    lParam: LPARAM;              //Input data.
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLSENDW = _PLUGINCALLSENDW;


type
  _PLUGINCALLPOSTA = record
    lParam: LPARAM;              //Input data.
    szFunction : array[0..MAX_PATH-1] of AnsiChar;  //cFunction name;
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLPOSTA = _PLUGINCALLPOSTA;


type
  _PLUGINCALLPOSTW = record
    lParam: LPARAM;                  //Input data.
    szFunction : array[0..MAX_PATH-1] of  WideChar;   //cFunction name;
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLPOSTW = _PLUGINCALLPOSTW;


type
  _PLUGINOPTIONA = record
    pOptionName: PAnsiChar;       //Option name.
    dwType: DWORD;                  //Data cType: see PO_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TPLUGINOPTIONA = _PLUGINOPTIONA;


type
  _PLUGINOPTIONW = record
    pOptionName: PWideChar;    //Option name.
    dwType: DWORD;                  //Data cType: see PO_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TPLUGINOPTIONW = _PLUGINOPTIONW;


type
  _INIVALUEA = record
    pSection: PAnsiChar;          //Section name.
    pKey: PAnsiChar;              //Key name.
    dwType: DWORD;                  //Data cType: see INI_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TINIVALUEA = _INIVALUEA;


type
  _INIVALUEW = record
    pSection: PWideChar;       //Section name.
    pKey: PWideChar;           //Key name.
    dwType: DWORD;                  //Data cType: see INI_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TINIVALUEW = _INIVALUEW;


type
  PINIKEY = ^TINIKEY;
  _INIKEY = record
    next: PINIKEY;
    prev: PINIKEY;
    wszKey: PWideChar;
    nKeyBytes: Integer;
    wszString: PWideChar;
    nStringBytes: Integer;
  end;
  TINIKEY = _INIKEY;


type
  PINISECTION = ^TINISECTION;
  _INISECTION = record
    next: PINISECTION;
    prev: PINISECTION;
    hIniFile: THandle;
    wszSection: PWideChar;
    nSectionBytes: Integer;
    first: PINIKEY;
    las: PINIKEY;
  end;
  TINISECTION = _INISECTION;


type
  _INIFILE = record
    first: PINISECTION;
    last: PINISECTION;
    bModified: BOOL;
  end;
  TINIFILE = _INIFILE;


type
  _GETTEXTRANGE = record
    cpMin: INT_PTR;              //First character in the range. First char of text: 0.
    cpMax: INT_PTR;              //Last character in the range. Last char of text: -1.
    pText: PByte;       //Pointer that receive allocated text, new lines are forced to "\r". Must be deallocated with AKD_FREETEXT message.
  end;
  TGETTEXTRANGE = _GETTEXTRANGE;


type
  _EXGETTEXTRANGE = record
    cr: TAECHARRANGE;             //Characters range to retrieve.
    bColumnSel: BOOL;            //Column selection. If this value is –1, active column selection mode is used.
    pText: PByte;       //Pointer that receive allocated text.
    nNewLine: Integer;               //See AELB_* defines.
    nCodePage: Integer;              //Valid if bOldWindows == TRUE. Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
    lpDefaultChar: PAnsiChar;  //Valid if bOldWindows == TRUE. Points to the character used if a wide character cannot be represented in the specified code page. If this member is NULL, a system default value is used.
    lpUsedDefChar: PBOOL;        //Valid if bOldWindows == TRUE. Points to a flag that indicates whether a default character was used. The flag is set to TRUE if one or more wide characters in the source string cannot be represented in the specified code page. Otherwise, the flag is set to FALSE. This member may be NULL.
  end;
  TEXGETTEXTRANGE = _EXGETTEXTRANGE;


type
  PRECENTFILE = ^TRECENTFILE;
  _RECENTFILE = record
    next: PRECENTFILE;
    prev: PRECENTFILE;
    wszFile : array[0..MAX_PATH-1] of WideChar;  //Recent cFile name.
    nFileLen: Integer;               //Recent cFile name length.
    nCodePage: Integer;              //Recent file codepages.
    cpMin: INT_PTR;              //First character in selection range.
    cpMax: INT_PTR;              //Last character in selection range.
    lpParamsStack: THSTACK;       //Additional parameters storage (RECENTFILEPARAMSTACK structure).
  end;
  TRECENTFILE = _RECENTFILE;


type
  _RECENTFILESTACK = record
    first: PRECENTFILE;          //Pointer to the first RECENTFILE structure.
    last: PRECENTFILE;           //Pointer to the last RECENTFILE structure.
    nElements: Integer;              //Items in stack.
    dwSaveTime: DWORD;           //GetTickCount() for the last recent files save operation.
  end;
  TRECENTFILESTACK = _RECENTFILESTACK;


type
  PRECENTFILEPARAM = ^TRECENTFILEPARAM;
  _RECENTFILEPARAM = record
    next: PRECENTFILEPARAM;
    prev: PRECENTFILEPARAM;
    pFile: PRECENTFILE;
    pParamName: PWideChar;
    pParamValue: PWideChar;
  end;
  TRECENTFILEPARAM = _RECENTFILEPARAM;

type
  _RECENTFILEPARAMSTACK = record
    first: PRECENTFILEPARAM;
    last: PRECENTFILEPARAM;
  end;
  TRECENTFILEPARAMSTACK = _RECENTFILEPARAMSTACK;


type
  _TEXTFINDA = record
    dwFlags: DWORD;            //See FRF_* defines.
    pFindIt: PAnsiChar;      //Find string.
    nFindItLen: Integer;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  end;
  TTEXTFINDA = _TEXTFINDA;


type
  _TEXTFINDW = record
    dwFlags: DWORD;            //See FRF_* defines.
    pFindIt: PWideChar;   //Find string.
    nFindItLen: Integer;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  end;
  TTEXTFINDW = _TEXTFINDW;


type
  _TEXTREPLACEA = record
    dwFlags: DWORD;               //See FRF_* defines.
    pFindIt: PAnsiChar;         //Find string.
    nFindItLen: Integer;              //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    pReplaceWith: PAnsiChar;    //Replace string.
    nReplaceWithLen: Integer;         //Replace string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    bAll: BOOL;                   //Replace all.
    nChanges: INT_PTR;            //Count of changes.
  end;
  TTEXTREPLACEA = _TEXTREPLACEA;


type
  _TEXTREPLACEW = record
    dwFlags: DWORD;               //See FRF_* defines.
    pFindIt: PWideChar;      //Find string.
    nFindItLen: Integer;              //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    pReplaceWith: PWideChar; //Replace string.
    nReplaceWithLen: Integer;         //Replace string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    bAll: BOOL;                   //Replace all.
    nChanges: INT_PTR;            //Count of changes.
  end;
  TTEXTREPLACEW = _TEXTREPLACEW;


type
  _TEXTRECODE = record
    nCodePageFrom: Integer;        //Source code page.
    nCodePageTo: Integer;          //Target code page.
    dwFlags: DWORD;            //See RCS_* defines.
  end;
  TTEXTRECODE = _TEXTRECODE;



type
  _CREATEWINDOWA = record
    szClassName : array[0..MAX_PATH-1] of AnsiChar;   //Window class name.
    szWindowName : array[0..MAX_PATH-1] of AnsiChar;  //Window caption.
    dwStyle: DWORD;            //Window style.
    x: Integer;                //X position.
    y: Integer;                //Y position.
    nWidth: Integer;           //X size.
    nHeight: Integer;          //Y size.
    hWndParent: HWND;          //Parent window handle.
    hMenu: HMENU;              //Window menu handle.
    hInstance: HINST;          //Program instance handle.
    lpPar: Pointer;            //Creation parameters.
  end;
  TCREATEWINDOWA = _CREATEWINDOWA;


type
  _CREATEWINDOWW = record
    szClassName : array[0..MAX_PATH-1] of  WideChar;    //Window class name.
    szWindowName : array[0..MAX_PATH-1] of  WideChar;   //Window caption.
    dwStyle: DWORD;            //Window style.
    x: Integer;                //X position.
    y: Integer;                //Y position.
    nWidth: Integer;           //X size.
    nHeight: Integer;          //Y size.
    hWndParent: HWND;          //Parent window handle.
    hMenu: HMENU;              //Window menu handle.
    hInstance: HINST;          //Program instance handle.
    lpPar: Pointer;            //Creation parameters.
  end;
  TCREATEWINDOWW = _CREATEWINDOWW;


type
  PDIALOGRESIZE = ^TDIALOGRESIZE;
  _DIALOGRESIZE = record
    lpWnd: PHWND;              //Control window.
    dwType: DWORD;             //See DRS_* defines.
    nOffset: Integer;          //Control offset, set it to zero.
  end;
  TDIALOGRESIZE = _DIALOGRESIZE;


type
  _DIALOGRESIZEMSG = record
    drs: PDIALOGRESIZE;  //Pointer to a first DIALOGRESIZE element in array.
    rcMinMax: PRect;     //Pointer to a min/max sizes. Each member is valid if not equal to zero. Can be NULL.
    rcCurrent: PRect;    //Pointer to a current rectangle. Set all members of TRect to zero at first call.
    dwFlags: DWORD;      //See DRM_* defines.
    hDlg: HWND;          //Dialog handle.
    uMsg: UINT;          //Dialog message.
    wParam: WPARAM;      //First message parameter.
    lParam: LPARAM;      //Second message parameter.
  end;
  TDIALOGRESIZEMSG = _DIALOGRESIZEMSG;


type
  PDOCK = ^TDOCK;
  _DOCK = record
    next: PDOCK;
    prev: PDOCK;
    dwFlags: DWORD;          //Dock flags: see DKF_* defines.
    hWnd: HWND;              //Dock window handle.
    nSide: Integer;              //Dock side: DKS_LEFT, DKS_TOP, DKS_RIGHT or DKS_BOTTOM.
    rcSize: TRect;            //Dock window client TRect.
    rcDragDrop: TRect;        //Drag-and-drop client TRect.
    lpOldDockProc: TWndProc;  //Procedure address before subclassing.
  end;
  TDOCK = _DOCK;


type
  _BUTTONDRAW = record
    dwFlags: DWORD;          //See BIF_* defines.
    hImage: THandle;          //Bitmap handle if BIF_BITMAP specified or icon handle if BIF_ICON specified.
    nImageWidth: Integer;        //Image width.
    nImageHeight: Integer;       //Image height.
  end;
  TBUTTONDRAW = _BUTTONDRAW;

  PBUTTONMESSAGEBOX = ^TBUTTONMESSAGEBOX;
  _BUTTONMESSAGEBOX  = record
    nButtonControlID: Integer;     //ID of the button that returned in result
    wpButtonStr: PWideChar;        //Pointer to Unicode text or ID of the string resource in current language module.
    dwFlags: DWORD;                //See BMB_* defines.
  end;
  TBUTTONMESSAGEBOX = _BUTTONMESSAGEBOX;


type
  _DIALOGMESSAGEBOX = record
    hWndParent: HWND;           //Handle to the owner window.
    wpText: PWideChar;          //Pointer to a null-terminated string that contains the message to be displayed.
    wpCaption: PWideChar;       //Pointer to a null-terminated string that contains the dialog box title.
    uType: UINT;                //Specifies the standard message box icon. See MSDN for MB_ICON* defines of the MessageBox function.
    hIcon: HICON;               //Custom icon.
    bmb: PBUTTONMESSAGEBOX;     //Array of the BUTTONMESSAGEBOX structures. Each element specified one message box button. Last item in the array should contain all zeros in members.
  end;
  TDIALOGMESSAGEBOX = _DIALOGMESSAGEBOX;


type
  _POSTMESSAGE = record
    hWnd: HWND;           //Window handle.
    uMsg: UINT;           //Specifies the message to be sent.
    wParam: WPARAM;       //Specifies additional message-specific information.
    lParam: LPARAM;       //Specifies additional message-specific information.
  end;
  TPOSTMESSAGE = _POSTMESSAGE;


type
  _PARSECMDLINESENDW = record
    pCmdLine: PWideChar; //Command line string. On return contain pointer to a unprocessed string.
    pWorkDir: PWideChar; //Command line string.
  end;
  TPARSECMDLINESENDW = _PARSECMDLINESENDW;


type
  _PARSECMDLINEPOSTW = record
    bPostMessage: BOOL;                   //FALSE for sending message (wait for return).
    szCmdLine : array[0..COMMANDLINE_SIZE-1] of WideChar; //Command line string.
    nCmdLineLen: Integer;                     //Command line length, not including the terminating null character.
    szWorkDir : array[0..MAX_PATH-1] of WideChar;         //Working directory string.
    nWorkDirLen: Integer;                     //Working directory length, not including the terminating null character.
    bQuitAsEnd: BOOL;                     //Internal variable - "/quit" stops parsing command line parameters, but not closes program.
  end;
  TPARSECMDLINEPOSTW = _PARSECMDLINEPOSTW;


type
  _PRINTINFO = record
    hDevMode: HGLOBAL;
    hDevNames: HGLOBAL;
    hDC: HDC;
    dwPrintFlags: DWORD;
    nFromPage: WORD;
    nToPage: WORD;
    nCopies: WORD;
    dwPageFlags: DWORD;
    rtMargin: TRect;
  end;
  TPRINTINFO = _PRINTINFO;


type
  _CHARCOLOR = record
    nCharPos: INT_PTR;       //Char position.
    crText: COLORREF;        //Text color.
    crBk: COLORREF;          //Background color.
  end;
  TCHARCOLOR = _CHARCOLOR;


type
  _NOPENDOCUMENT = record
    pFile: PBYTE;      //Opening cFile.
    szFile: PAnsiChar;     //Opening cFile (Ansi).
    wszFile: PWideChar; //Opening cFile (Unicode).
    nCodePage: PInteger;         //Pointer to a code page variable.
    bBOM: PBOOL;             //Pointer to a BOM variable.
    dwFlags: PDWORD;         //Pointer to a open flags variable. See OD_* defines.
    bProcess: BOOL;          //TRUE   open file.
  end;
  TNOPENDOCUMENT = _NOPENDOCUMENT;


type
  _NSAVEDOCUMENT = record
    pFile: PBYTE;      //Saving cFile.
    szFile: PAnsiChar;     //Saving cFile (Ansi).
    wszFile: PWideChar; //Saving cFile (Unicode).
    nCodePage: PInteger;         //Pointer to a code page variable.
    bBOM: PBOOL;             //Pointer to a BOM variable.
    dwFlags: DWORD;          //See SD_* defines.
    bProcess: BOOL;          //TRUE   save file.
  end;
  TNSAVEDOCUMENT = _NSAVEDOCUMENT;


type
  _NMAINSHOW = record
    dwStyle: PDWORD;         //Pointer to a maximized state variable (WS_MAXIMIZE or zero).
    dwShow: PDWORD;          //Pointer to a SW_ constants combination variable.
    bProcess: BOOL;          //TRUE   show main window.
  end;
  TNMAINSHOW = _NMAINSHOW;


type
  _NCONTEXTMENU = record
    hWnd: HWND;              //Context menu window.
    uType: UINT;             //cType:    NCM_EDIT, NCM_TAB or NCM_STATUS.
    pt: TPoint;               //Context menu coordiates.
    bMouse: BOOL;            //Context menu is requested with mouse.
    bProcess: BOOL;          //TRUE   show context menu.
  end;
  TNCONTEXTMENU = _NCONTEXTMENU;


type
  _NSIZE = record
    rcInitial: TRect;         //Initial client TRect (read-only).
    rcCurrent: TRect;         //Current client TRect (writeable).
  end;
  TNSIZE = _NSIZE;


//// AkelPad menu messages

const IDM_FILE_NEW                    = 4101;  //Create new edit window.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_CREATENEW              = 4102;  //Create new instance of program.
                                              //Return Value: new main window handle.
                                              //
const IDM_FILE_OPEN                   = 4103;  //Open file dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_REOPEN                 = 4104;  //Reopen file.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_SAVE                   = 4105;  //Save file.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_SAVEAS                 = 4106;  //Save file dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_PAGESETUP              = 4107;  //Print setup dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_PRINT                  = 4108;  //Print dialog. lParam can be used to pass edit window handle.
                                              //Return Value: number of printed pages.
                                              //
const IDM_FILE_EXIT                   = 4109;  //Exit program.
                                              //Return Value: zero.
                                              //
const IDM_FILE_SAVEALL                = 4110;  //Save all documents.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_SAVEALLAS              = 4111;  //Save all as dialog.
                                              //Return Value: TRUE - "OK" pressed, FALSE - "Cancel" pressed.
                                              //
const IDM_FILE_SILENTPRINT            = 4113;  //Print without dialog. lParam can be used to pass edit window handle.
                                              //Return Value: number of printed pages.
                                              //
const IDM_FILE_PRINTPREVIEW           = 4114;  //Print preview dialog. lParam can be used to pass edit window handle.
                                              //Return Value: zero.
                                              //
const IDM_FILE_REDETECT               = 4121;  //Redetect code page of the current file.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_ANSI          = 4122;  //Reopen file as ANSI.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_OEM           = 4123;  //Reopen file as OEM.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_KOIR          = 4124;  //Reopen file as KOI-R.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_UTF16LE       = 4125;  //Reopen file as UTF16LE.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_UTF16BE       = 4126;  //Reopen file as UTF16BE.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_UTF8          = 4127;  //Reopen file as UTF8.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_SAVEAS_ANSI            = 4131;  //Save file as ANSI.
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_OEM             = 4132;  //Save file as OEM.
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_KOIR            = 4133;  //Save file as KOI-R.
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF16LE         = 4134;  //Save file as UTF16LE.
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF16BE         = 4135;  //Save file as UTF16BE.
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF8            = 4136;  //Save file as UTF8.
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF8_NOBOM      = 4137;  //Save file as UTF8 without BOM.
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_CODEPAGEMENU           = 4140;  //Show codepages context menu.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_UNDO                   = 4151;  //Undo last operation.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_REDO                   = 4152;  //Redo last operation.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_CUT                    = 4153;  //Cut.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_COPY                   = 4154;  //Copy.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_PASTE                  = 4155;  //Paste.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_CLEAR                  = 4156;  //Delete.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_SELECTALL              = 4157;  //Select all.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_FIND                   = 4158;  //Find dialog.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_FINDNEXTDOWN           = 4159;  //Find last string down.
                                              //Return Value: Character position of the next match. If there are no more matches, the return value is �1.
                                              //
const IDM_EDIT_FINDNEXTUP             = 4160;  //Find last string up.
                                              //Return Value: Character position of the next match. If there are no more matches, the return value is �1.
                                              //
const IDM_EDIT_REPLACE                = 4161;  //Replace dialog.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_GOTO                   = 4162;  //Go to dialog.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERTCHAR             = 4163;  //Run charmap.exe.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERT_TAB             = 4164;  //Insert tabulation.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_INSERT_TAB_MENU        = 4165;  //Insert tabulation only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_DELETE_TAB             = 4166;  //Delete tabulation.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_DELETE_TAB_MENU        = 4167;  //Delete tabulation only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_INSERT_SPACE           = 4168;  //Insert space.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_INSERT_SPACE_MENU      = 4169;  //Insert space only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_DELETE_SPACE           = 4170;  //Delete space.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_DELETE_SPACE_MENU      = 4171;  //Delete space only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed..
                                              //
const IDM_EDIT_DELETE_FIRST_CHAR      = 4172;  //Delete first char.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_DELETE_FIRST_CHAR_MENU = 4173;  //Delete first char only if selection not empty.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_DELETE_TRAILING_WHITESPACES = 4174;  //Delete trailing whitespaces.
                                                   //Return Value: TRUE - success, FALSE - failed.
                                                   //
const IDM_EDIT_UPPERCASE              = 4175;  //Convert text to UPPERCASE. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_LOWERCASE              = 4176;  //Convert text to lowercase. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_SENTENCECASE           = 4177;  //Convert text to Sentence case. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_TITLECASE              = 4178;  //Convert text to Title Case. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_INVERTCASE             = 4179;  //Convert text to iNVERT cASE. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_LOOPCASE               = 4180;  //Cycle case. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_RECODE                 = 4182;  //Recode dialog.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERTDATE             = 4183;  //Insert date.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_NEWLINE_WIN            = 4184;  //Windows new line format.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_NEWLINE_UNIX           = 4185;  //Unix new line format.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_NEWLINE_MAC            = 4186;  //Mac new line format.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERTMODE             = 4190;  //Insert mode (on\off).
                                              //Return Value: zero.
                                              //
const IDM_EDIT_PASTEANSI              = 4191;  //Paste as ANSI text.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_PASTECOLUMN            = 4192;  //Paste to column selection.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_PASTEAFTER             = 4193;  //Paste text after caret.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_DELETESELWHITESPACES   = 4195;  //Delete all whitespaces in selection.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_AUTOINDENT             = 4196;  //Insert new line with keeping left spaces.
                                              //Return Value: TRUE - inserted with spaces, FALSE - inserted without spaces.
                                              //
const IDM_EDIT_DELLINE                = 4197;  //Delete current line. lParam: TRUE - delete all wrapped line, FALSE - normal behavior.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_SELJUMPCARET           = 4198;  //Move caret on the contrary side of selection.
                                              //Return Value: TRUE - jump to selection beginning, FALSE - jump to selection ending.
                                              //
const IDM_EDIT_RECENTCARETPREV        = 4199;  //Move caret to the previous position.
                                              //Return Value: zero.
                                              //
const IDM_EDIT_RECENTCARETNEXT        = 4200;  //Move caret to the next position.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_FONT                   = 4201;  //Font dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_VIEW_COLORS                 = 4202;  //Color theme dialog.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_VIEW_FONTSIZE_INCREASE      = 4204;  //Increase font 1px.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_FONTSIZE_DECREASE      = 4205;  //Decrease font 1px.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_FONTSIZE_RESTORE       = 4206;  //Decrease font 1px.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_WORDWRAP               = 4209;  //Word wrap (on\off).
                                              //Return Value: zero.
                                              //
const IDM_VIEW_ONTOP                  = 4210;  //Always on top (on\off).
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SHOW_STATUSBAR         = 4211;  //Show statusbar (on\off).
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_ALL       = 4212;  //Split window into four panes. Turn off, if window already split into four panes.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_WE        = 4213;  //Split window into two vertical panes. Turn off, if window already split into two vertical panes.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_NS        = 4214;  //Split window into two horizontal panes. Turn off, if window already split into two horizontal panes.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_ONOFF     = 4215;  //Split window into four panes. Turn off, if window already split.
                                              //Return Value: zero.
                                              //
const IDM_VIEW_READONLY               = 4216;  //Read only (on\off).
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_EXEC                = 4251;  //Execute command.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_OPTIONS_SAVETIME            = 4252;  //Save file time (on\off).
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_WATCHFILE           = 4253;  //Watch file change (on\off).
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_KEEPSPACE           = 4254;  //Keep space (on\off).
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_SINGLEOPEN_FILE     = 4255;  //Single open file (on\off).
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_SINGLEOPEN_PROGRAM  = 4256;  //Single open program (on\off).
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_PLUGINS             = 4259;  //Plugins dialog.
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS                     = 4260;  //Options dialog.
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_SDI                 = 4261;  //SDI mode.
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_MDI                 = 4262;  //MDI mode.
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_PMDI                = 4263;  //Pseudo MDI mode.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABVIEW_TOP          = 4301;  //MDI tab window at the top of the main window.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABVIEW_BOTTOM       = 4302;  //MDI tab window at the bottom of the main window.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABVIEW_NONE         = 4303;  //Hide MDI tab.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABTYPE_STANDARD     = 4304;  //Standard MDI tab style.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABTYPE_BUTTONS      = 4305;  //Buttons MDI tab style.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABTYPE_FLATBUTTONS  = 4306;  //Flat buttons MDI tab style.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TILEHORIZONTAL       = 4307;  //MDI windows - horizontal tile.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TILEVERTICAL         = 4308;  //MDI windows - vertical tile.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_CASCADE              = 4309;  //MDI windows - cascade.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABSWITCH_NEXTPREV   = 4310;  //Switch MDI windows: next-previous.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABSWITCH_RIGHTLEFT  = 4311;  //Switch MDI windows: right-left.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_FRAMENEXT            = 4316;  //Activate next MDI window.
                                              //Return Value: pointer to a FRAMEDATA structure that has been activated.
                                              //
const IDM_WINDOW_FRAMEPREV            = 4317;  //Activate previous MDI window.
                                              //Return Value: pointer to a FRAMEDATA structure that has been activated.
                                              //
const IDM_WINDOW_FRAMECLOSE           = 4318;  //Close current MDI window.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_FRAMECLOSEALL        = 4319;  //Close all documents.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_FRAMECLOSEALL_BUTACTIVE  = 4320;  //Close all documents, but active.
                                                  //Return Value: TRUE - success, FALSE - failed.
                                                  //
const IDM_WINDOW_FRAMECLOSEALL_UNMODIFIED = 4321;  //Close all unmodified documents.
                                                  //Return Value: TRUE - success, FALSE - failed.
                                                  //
const IDM_WINDOW_FILECLOSE            = 4324;  //Close file.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_FILEEXIT             = 4325;  //Close file and exit program (SDI) or close tab of a file (MDI or PMDI).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_MDILIST              = 4327;  //Select window dialog (MDI). Same as IDM_SELECTWINDOW.
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_CHANGESIZE           = 4331;  //Change style of the main window SW_RESTORE\SW_MAXIMIZE.
                                              //Return Value: SW_RESTORE - new style is SW_RESTORE, SW_MAXIMIZE - new style is SW_MAXIMIZE.
                                              //
const IDM_WINDOW_DLGNEXT              = 4332;  //Activate next dialog window.
                                              //Return Value: activated dialog handle.
                                              //
const IDM_WINDOW_DLGPREV              = 4333;  //Activate previous dialog window.
                                              //Return Value: activated dialog handle.
                                              //
const IDM_WINDOW_CLONENEXT            = 4341;  //Activate next pane (split window).
                                              //Return Value: activated pane handle.
                                              //
const IDM_WINDOW_CLONEPREV            = 4342;  //Activate previous pane (split window).
                                              //Return Value: activated pane handle.
                                              //
const IDM_ABOUT                       = 4351;  //About dialog.
                                              //Return Value: zero.
                                              //
const IDM_MANUAL                      = 4352;  //Open user's manual.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_UPDATE                      = 4353;  //Open AkelUpdater.
                                              //Return Value: zero.
                                              //
const IDM_INTERNAL_REOPEN_MSG         = 4601;  //Internal command.
                                              //Return Value: zero.
                                              //
const IDM_INTERNAL_CANTOPEN_MSG       = 4602;  //Internal command.
                                              //Return Value: zero.
                                              //
const IDM_INTERNAL_ERRORIO_MSG        = 4603;  //Internal command.
                                              //Return Value: zero.
                                              //
const IDM_INTERNAL_UPDATEMAINCHILDREN = 4604;  //Internal command.
                                              //Return Value: zero.
                                              //
const IDM_RECENT_FILES                = 5001;  //Delete dead recent files.
                                              //5001 + n   Open recent file n.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_LANGUAGE                    = 6001;  //Activate internal language.
                                              //6001 + n   Activate language n.
                                              //Return Value: zero.
                                              //
const IDM_POPUP_OPENAS                = 7001;  //Reopen file in first codepage of codepage list.
                                              //7001 + n   Reopen file in codepage n.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_POPUP_SAVEAS                = 8001;  //Save file in first codepage of codepage list.
                                              //8001 + n   Save file in codepage n.
                                              //Return Value: see ESD_* defines.
const IDM_SELECTWINDOW                = 10019; //Select window dialog (MDI). Same as IDM_WINDOW_MDILIST.
                                              //Return Value: zero.
                                              //
                                              //
                                              //Example of usage:
                                              //SendMessage(pd->hMainWnd, WM_COMMAND, IDM_FILE_NEW, 0);

//// AkelPad main window WM_USER messages

//Notification messages
const AKDN_MAIN_ONSTART          = (WM_USER + 1);   //0x401
const AKDN_MAIN_ONSTART_PRESHOW  = (WM_USER + 2);   //0x402
const AKDN_MAIN_ONSTART_SHOW     = (WM_USER + 3);   //0x403
const AKDN_MAIN_ONSTART_FINISH   = (WM_USER + 4);   //0x404
const AKDN_MAIN_ONSTART_IDLE     = (WM_USER + 5);   //0x405
const AKDN_MAIN_ONFINISH         = (WM_USER + 6);   //0x406
const AKDN_MAIN_ONDESTROY        = (WM_USER + 7);   //0x407

const AKDN_EDIT_ONSTART          = (WM_USER + 11);  //0x40B
const AKDN_EDIT_ONFINISH         = (WM_USER + 12);  //0x40C
const AKDN_EDIT_ONCLOSE          = (WM_USER + 13);  //0x40D

const AKDN_FRAME_NOWINDOWS       = (WM_USER + 21);  //0x415
const AKDN_FRAME_ACTIVATE        = (WM_USER + 22);  //0x416
const AKDN_FRAME_DEACTIVATE      = (WM_USER + 23);  //0x417
const AKDN_FRAME_DESTROY         = (WM_USER + 24);  //0x418

const AKDN_DOCK_GETMINMAXINFO    = (WM_USER + 31);  //0x41F
const AKDN_DOCK_CAPTURE_ONSTART  = (WM_USER + 32);  //0x420
const AKDN_DOCK_CAPTURE_ONFINISH = (WM_USER + 33);  //0x421
const AKDN_DOCK_RESIZE           = (WM_USER + 34);  //0x422

const AKDN_DLLCALL               = (WM_USER + 41);  //0x429
const AKDN_DLLUNLOAD             = (WM_USER + 42);  //0x42A

const AKDN_ACTIVATE              = (WM_USER + 50);  //0x432
const AKDN_SIZE_ONSTART          = (WM_USER + 51);  //0x433
const AKDN_SIZE_ONFINISH         = (WM_USER + 52);  //0x434
const AKDN_OPENDOCUMENT_START    = (WM_USER + 53);  //0x435
const AKDN_OPENDOCUMENT_FINISH   = (WM_USER + 54);  //0x436
const AKDN_SAVEDOCUMENT_START    = (WM_USER + 55);  //0x437
const AKDN_SAVEDOCUMENT_FINISH   = (WM_USER + 56);  //0x438
const AKDN_HOTKEY                = (WM_USER + 57);  //0x439
const AKDN_CONTEXTMENU           = (WM_USER + 58);  //0x43A
const AKDN_SEARCH_ENDED          = (WM_USER + 59);  //0x43B
const AKDN_MESSAGEBOXBEGIN       = (WM_USER + 61);  //0x43D
const AKDN_MESSAGEBOXEND         = (WM_USER + 62);  //0x43E

//SubClass
const AKD_GETMAINPROC            = (WM_USER + 101);
const AKD_SETMAINPROC            = (WM_USER + 102);
const AKD_GETMAINPROCRET         = (WM_USER + 103);
const AKD_SETMAINPROCRET         = (WM_USER + 104);
const AKD_GETEDITPROC            = (WM_USER + 105);
const AKD_SETEDITPROC            = (WM_USER + 106);
const AKD_GETEDITPROCRET         = (WM_USER + 107);
const AKD_SETEDITPROCRET         = (WM_USER + 108);
const AKD_GETFRAMEPROC           = (WM_USER + 109);
const AKD_SETFRAMEPROC           = (WM_USER + 110);
const AKD_GETFRAMEPROCRET        = (WM_USER + 111);
const AKD_SETFRAMEPROCRET        = (WM_USER + 112);

//Command line
const AKD_GETCMDLINEOPTIONS      = (WM_USER + 121);
const AKD_SETCMDLINEOPTIONS      = (WM_USER + 122);
const AKD_PARSECMDLINEW          = (WM_USER + 125);

//Text retrieval and modification
const AKD_WRITEFILECONTENT       = (WM_USER + 141);
const AKD_DETECTANSITEXT         = (WM_USER + 146);
const AKD_DETECTUNITEXT          = (WM_USER + 147);
const AKD_CONVERTANSITEXT        = (WM_USER + 148);
const AKD_CONVERTUNITEXT         = (WM_USER + 149);
const AKD_DETECTFILE             = (WM_USER + 151);
const AKD_DETECTFILEA            = (WM_USER + 152);
const AKD_DETECTFILEW            = (WM_USER + 153);
const AKD_READFILECONTENT        = (WM_USER + 154);
const AKD_OPENDOCUMENT           = (WM_USER + 155);
const AKD_OPENDOCUMENTA          = (WM_USER + 156);
const AKD_OPENDOCUMENTW          = (WM_USER + 157);
const AKD_SAVEDOCUMENT           = (WM_USER + 158);
const AKD_SAVEDOCUMENTA          = (WM_USER + 159);
const AKD_SAVEDOCUMENTW          = (WM_USER + 160);
const AKD_GETTEXTLENGTH          = (WM_USER + 161);
const AKD_GETTEXTRANGE           = (WM_USER + 162);
const AKD_GETTEXTRANGEA          = (WM_USER + 163);
const AKD_GETTEXTRANGEW          = (WM_USER + 164);
const AKD_GETSELTEXTW            = (WM_USER + 165);
const AKD_FREETEXT               = (WM_USER + 166);
const AKD_REPLACESEL             = (WM_USER + 167);
const AKD_REPLACESELA            = (WM_USER + 168);
const AKD_REPLACESELW            = (WM_USER + 169);
const AKD_PASTE                  = (WM_USER + 170);
const AKD_COPY                   = (WM_USER + 171);
const AKD_TEXTFIND               = (WM_USER + 172);
const AKD_TEXTFINDA              = (WM_USER + 173);
const AKD_TEXTFINDW              = (WM_USER + 174);
const AKD_TEXTREPLACE            = (WM_USER + 175);
const AKD_TEXTREPLACEA           = (WM_USER + 176);
const AKD_TEXTREPLACEW           = (WM_USER + 177);
const AKD_RECODESEL              = (WM_USER + 178);
const AKD_GOTO                   = (WM_USER + 180);
const AKD_GOTOA                  = (WM_USER + 181);
const AKD_GOTOW                  = (WM_USER + 182);

//Print
const AKD_GETPRINTINFO           = (WM_USER + 191);
const AKD_SETPRINTINFO           = (WM_USER + 192);

//Options
const AKD_SETMAININFO            = (WM_USER + 195);
const AKD_SETFRAMEINFO           = (WM_USER + 196);
const AKD_GETMAININFO            = (WM_USER + 198);
const AKD_GETFRAMEINFO           = (WM_USER + 199);
const AKD_GETEDITINFO            = (WM_USER + 200);
const AKD_PROGRAMVERSION         = (WM_USER + 201);
const AKD_PROGRAMARCHITECTURE    = (WM_USER + 202);
const AKD_SETMODIFY              = (WM_USER + 205);
const AKD_SETNEWLINE             = (WM_USER + 206);
const AKD_GETFONT                = (WM_USER + 207);
const AKD_GETFONTA               = (WM_USER + 208);
const AKD_GETFONTW               = (WM_USER + 209);
const AKD_SETFONT                = (WM_USER + 210);
const AKD_SETFONTA               = (WM_USER + 211);
const AKD_SETFONTW               = (WM_USER + 212);
const AKD_GETCODEPAGELIST        = (WM_USER + 213);
const AKD_RECENTFILES            = (WM_USER + 214);
const AKD_SEARCHHISTORY          = (WM_USER + 215);
const AKD_SETEDITNOTIFY          = (WM_USER + 216);

//Windows
const AKD_GETMODELESS            = (WM_USER + 251);
const AKD_SETMODELESS            = (WM_USER + 252);
const AKD_RESIZE                 = (WM_USER + 253);
const AKD_DOCK                   = (WM_USER + 254);
const AKD_SETBUTTONDRAW          = (WM_USER + 255);
const AKD_SETHOTKEYINPUT         = (WM_USER + 256);
const AKD_DIALOGRESIZE           = (WM_USER + 257);
const AKD_UPDATESTATUSUSER       = (WM_USER + 258);

//Frames
const AKD_FRAMEACTIVATE          = (WM_USER + 261);
const AKD_FRAMENEXT              = (WM_USER + 262);
const AKD_FRAMEDESTROY           = (WM_USER + 263);
const AKD_FRAMEFIND              = (WM_USER + 264);
const AKD_FRAMEFINDA             = (WM_USER + 265);
const AKD_FRAMEFINDW             = (WM_USER + 266);
const AKD_FRAMESTATS             = (WM_USER + 267);
const AKD_FRAMENOWINDOWS         = (WM_USER + 268);
const AKD_FRAMEISVALID           = (WM_USER + 269);
const AKD_FRAMEINDEX             = (WM_USER + 270);
const AKD_FRAMEINIT              = (WM_USER + 271);
const AKD_FRAMEAPPLYEDIT         = (WM_USER + 272);

//Process and Threads
const AKD_MEMCREATE              = (WM_USER + 281);
const AKD_MEMMAP                 = (WM_USER + 282);
const AKD_MEMUNMAP               = (WM_USER + 283);
const AKD_MEMCLOSE               = (WM_USER + 284);
const AKD_STRLENA                = (WM_USER + 285);
const AKD_STRLENW                = (WM_USER + 286);
const AKD_CREATEWINDOW           = (WM_USER + 287);
const AKD_WAITKEYBOARD           = (WM_USER + 288);
const AKD_GETQUEUE               = (WM_USER + 289);
const AKD_POSTMESSAGE            = (WM_USER + 290);
const AKD_TRANSLATEMESSAGE       = (WM_USER + 291);
const AKD_MESSAGEBOX             = (WM_USER + 292);
const AKD_GETFOCUS               = (WM_USER + 293);
const AKD_PEEKMESSAGE            = (WM_USER + 294);
const AKD_UNIQUEID               = (WM_USER + 295);

//Plugin load
const AKD_DLLCALL                = (WM_USER + 301);
const AKD_DLLCALLA               = (WM_USER + 302);
const AKD_DLLCALLW               = (WM_USER + 303);
const AKD_DLLUNLOAD              = (WM_USER + 304);
const AKD_DLLFIND                = (WM_USER + 305);
const AKD_DLLFINDA               = (WM_USER + 306);
const AKD_DLLFINDW               = (WM_USER + 307);
const AKD_DLLADD                 = (WM_USER + 308);
const AKD_DLLADDA                = (WM_USER + 309);
const AKD_DLLADDW                = (WM_USER + 310);
const AKD_DLLDELETE              = (WM_USER + 311);
const AKD_DLLSAVE                = (WM_USER + 312);
const AKD_CALLPROC               = (WM_USER + 313);
const AKD_CHECKHOTKEY            = (WM_USER + 314);

//Plugin options
const AKD_BEGINOPTIONS           = (WM_USER + 331);
const AKD_BEGINOPTIONSA          = (WM_USER + 332);
const AKD_BEGINOPTIONSW          = (WM_USER + 333);
const AKD_OPTION                 = (WM_USER + 334);
const AKD_OPTIONA                = (WM_USER + 335);
const AKD_OPTIONW                = (WM_USER + 336);
const AKD_ENDOPTIONS             = (WM_USER + 341);
const AKD_INIOPEN                = (WM_USER + 342);
const AKD_INIOPENA               = (WM_USER + 343);
const AKD_INIOPENW               = (WM_USER + 344);
const AKD_INIGETSECTION          = (WM_USER + 345);
const AKD_INIGETSECTIONA         = (WM_USER + 346);
const AKD_INIGETSECTIONW         = (WM_USER + 347);
const AKD_INICLEARSECTION        = (WM_USER + 348);
const AKD_INIDELETESECTION       = (WM_USER + 349);
const AKD_INIGETKEY              = (WM_USER + 350);
const AKD_INIGETKEYA             = (WM_USER + 351);
const AKD_INIGETKEYW             = (WM_USER + 352);
const AKD_INIDELETEKEY           = (WM_USER + 353);
const AKD_INIGETVALUE            = (WM_USER + 354);
const AKD_INIGETVALUEA           = (WM_USER + 355);
const AKD_INIGETVALUEW           = (WM_USER + 356);
const AKD_INISETVALUE            = (WM_USER + 357);
const AKD_INISETVALUEA           = (WM_USER + 358);
const AKD_INISETVALUEW           = (WM_USER + 359);
const AKD_INICLOSE               = (WM_USER + 360);

//Regular expressions. Requires include of "RegExpFunc.h".
const AKD_PATEXEC                = (WM_USER + 391);
const AKD_PATREPLACE             = (WM_USER + 392);
const AKD_PATGROUPSTR            = (WM_USER + 393);
const AKD_PATGETGROUP            = (WM_USER + 395);
const AKD_PATNEXTGROUP           = (WM_USER + 396);
const AKD_PATPREVGROUP           = (WM_USER + 397);
const AKD_PATFREE                = (WM_USER + 399);

//AkelPad 4.x messages
const AKD_EXGETTEXTLENGTH        = (WM_USER + 401);
const AKD_EXGETTEXTRANGE         = (WM_USER + 402);
const AKD_EXGETTEXTRANGEA        = (WM_USER + 403);
const AKD_EXGETTEXTRANGEW        = (WM_USER + 404);



// Messages manual: look AkelDLL.h

//// UNICODE define
type
{$ifndef UNICODE}
  TDETECTFILE = TDETECTFILEA;
  TOPENDOCUMENT = TOPENDOCUMENTA;
  TOPENDOCUMENTPOST = TOPENDOCUMENTPOSTA;
  TSAVEDOCUMENT = TSAVEDOCUMENTA;
  TPLUGINADD = TPLUGINADDA;
  TPLUGINCALLSEND = TPLUGINCALLSENDA;
  TPLUGINCALLPOST = TPLUGINCALLPOSTA;
  TPLUGINOPTION = TPLUGINOPTIONA;
  TINIVALUE = TINIVALUEA;
  TTEXTFIND = TTEXTFINDA;
  TTEXTREPLACE = TTEXTREPLACEA;
  TCREATEWINDOW = TCREATEWINDOWA;
{$else}
  TDETECTFILE = TDETECTFILEW;
  TOPENDOCUMENT = TOPENDOCUMENTW;
  TOPENDOCUMENTPOST = TOPENDOCUMENTPOSTW;
  TSAVEDOCUMENT = TSAVEDOCUMENTW;
  TPLUGINADD = TPLUGINADDW;
  TPLUGINCALLSEND = TPLUGINCALLSENDW;
  TPLUGINCALLPOST = TPLUGINCALLPOSTW;
  TPLUGINOPTION = TPLUGINOPTIONW;
  TINIVALUE = TINIVALUEW;
  TTEXTFIND = TTEXTFINDW;
  TTEXTREPLACE = TTEXTREPLACEW;
  TCREATEWINDOW = TCREATEWINDOWW;
{$endif}

// Parameters for current call are kept in the following form:
//   pd.lParam - pointer to array of INT_PTR (let's call it Params)
//   Params[0] = size of the whole array in bytes including the 0-th element itself.
//               So Length(Params) = Params[0] div SizeOf(INT_PTR), ParamCount = Length(Params) - 1
//   Params[1] = pointer to parameter string OR the value of numerical parameter
//   ...
// WARNING. There's no way to distinguish what kind of parameter is passed so be careful

{$IF CompilerVersion < 20} // ?
  {*****************************************************************************
    Usage for Delphi versions earlier than D2009
    procedure Init(var pd: TPLUGINDATA);
    var
      AkelParams: PAkelParamArr;
    begin
      AkelParams := PAkelParamArr(pd.lParam);
      if AkelParams_Count(AkelParams) > 0 then
        sParam := AkelParams_ParamStr(AkelParams, 0); // returns empty string if parameter is NULL
        ...
    end;
  *****************************************************************************}

type
  TAkelParamArr = array[0..$FFFF] of INT_PTR;
  PAkelParamArr = ^TAkelParamArr;

function AkelParams_Count(Params: PAkelParamArr): Integer;
function AkelParams_ParamInt(Params: PAkelParamArr; Idx: Integer): INT_PTR;
function AkelParams_ParamStr(Params: PAkelParamArr; Idx: Integer): string;

{$ELSE}
  {*****************************************************************************
    Usage for Delphi versions D2009 and later
    procedure Init(var pd: TPLUGINDATA);
    var
      AkelParams: TAkelParams;
    begin
      AkelParams.Init(Pointer(pd.lParam));
      if AkelParams.Count > 0 then
      begin
        sParam := AkelParams.ParamStr(0); // returns empty string if parameter is NULL
        ...
    end;
  *****************************************************************************}

type
  TAkelParams = record
  strict private
    type
      TAkelParamArr = array[0..$FFFF] of INT_PTR;
      PAkelParamArr = ^TAkelParamArr;
    var
      FParamArr: PAkelParamArr;
  public
    procedure Init(ParamArr: Pointer);
    function Count: Integer;
    function ParamStr(Idx: Integer): string;
    function ParamInt(Idx: Integer): INT_PTR;
  end;
{$IFEND}

implementation

function MakeIdentifier(a, b, c, d: ShortInt): DWORD;
begin
  Result := DWORD(MAKELONG(MAKEWORD(Byte(a), Byte(b)), MAKEWORD(Byte(c), Byte(d))));
end;

function AkelDLLVer: DWORD;
begin
  Result := MakeIdentifier(AkelDLLVerArr[1], AkelDLLVerArr[2], AkelDLLVerArr[3], AkelDLLVerArr[4]);
end;

{$IF CompilerVersion < 20}

function AkelParams_Count(Params: PAkelParamArr): Integer;
begin
  if Params = nil then
    Result := 0
  else
    Result := Params^[0] div SizeOf(INT_PTR) - 1;
end;

function AkelParams_ParamInt(Params: PAkelParamArr; Idx: Integer): INT_PTR;
begin
  if (Idx >= AkelParams_Count(Params)) or (Idx < 0) then
    Result := 0
  else
    Result := Params^[Idx + 1];
end;

function AkelParams_ParamStr(Params: PAkelParamArr; Idx: Integer): string;
begin
  Result := string(PChar(AkelParams_ParamInt(Params, Idx)));
end;

{$ELSE}

procedure TAkelParams.Init(ParamArr: Pointer);
begin
  FParamArr := PAkelParamArr(ParamArr);
end;

function TAkelParams.Count: Integer;
begin
  if FParamArr = nil then
    Result := 0
  else
    Result := FParamArr^[0] div SizeOf(INT_PTR) - 1;
end;

function TAkelParams.ParamInt(Idx: Integer): INT_PTR;
begin
  if (Idx >= Count) or (Idx < 0) then
    Result := 0
  else
    Result := FParamArr^[Idx + 1];
end;

function TAkelParams.ParamStr(Idx: Integer): string;
begin
  Result := string(PChar(ParamInt(Idx)));
end;

{$IFEND}

end.
