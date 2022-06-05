(*************************************************************************

=========  AkelPad text editor plugin API ===========
=========    Akel API version : 1.9.0.1   ===========

** Origin: AkelDLL.h located at
   http://akelpad.cvs.sourceforge.net/viewvc/akelpad/akelpad_4/AkelFiles/Plugs/AkelDLL/AkelDLL.h
** Converted with C to Pascal Converter 2.0
** Release: 2.20.11.2011
** Email: al_gun@ncable.net.au
** Updates: http://cc.codegear.com/Author/302259
** Blogs: http://delphiprogrammingwithalgun.blogspot.com/
** Copyright (c) 2005, 2011 Ural Gunaydin (a.k.a. Al Gun)

===========           Edited by Fr0sT           ===========
= Tested on RAD Studio 2010 but compiles on D7 and should =
= work on other versions also.                            =
= All RegExp defines were excluded (they are in separate file RegExpFunc.h now)

**************************************************************************)

unit AkelDLL_h;

interface

uses
  Windows, Messages;

//// Version
function MakeIdentifier(a, b, c, d: ShortInt): DWORD;
{$EXTERNALSYM MakeIdentifier}
function AkelDLL: DWORD;
{$EXTERNALSYM AkelDLL}

const AkelDLLVer: array[1..4] of Byte = (1, 9, 0, 1);
{$EXTERNALSYM AkelDLLVer}

//// Defines

//Control IDs
const ID_EDIT = 10001;
{$EXTERNALSYM ID_EDIT}
const ID_STATUS = 10002;
{$EXTERNALSYM ID_STATUS}
const ID_TAB = 10003;
{$EXTERNALSYM ID_TAB}
const ID_PROGRESS = 10004;
{$EXTERNALSYM ID_PROGRESS}
const ID_FIRSTMDI = 10010;
{$EXTERNALSYM ID_FIRSTMDI}

//Window mode
const WMD_SDI = 0;  //Single document interface (SDI).
{$EXTERNALSYM WMD_SDI}
const WMD_MDI = 1;  //Multiple document interface (MDI).
{$EXTERNALSYM WMD_MDI}
const WMD_PMDI = 2;  //Pseudo-Multiple document interface (PMDI).
{$EXTERNALSYM WMD_PMDI}

//Sizes
const COMMANDLINE_SIZE = 32768;
{$EXTERNALSYM COMMANDLINE_SIZE}
const COMMANDARG_SIZE = 16384;
{$EXTERNALSYM COMMANDARG_SIZE}
const WORD_DELIMITERS_SIZE = 128;
{$EXTERNALSYM WORD_DELIMITERS_SIZE}
const WRAP_DELIMITERS_SIZE = 128;
{$EXTERNALSYM WRAP_DELIMITERS_SIZE}
const URL_PREFIXES_SIZE = 128;
{$EXTERNALSYM URL_PREFIXES_SIZE}
const URL_DELIMITERS_SIZE = 128;
{$EXTERNALSYM URL_DELIMITERS_SIZE}

//Unload plugin flag
const UD_FAILED = -1;  //Operation failed. Don't use it.
{$EXTERNALSYM UD_FAILED}
const UD_UNLOAD = 0;  //Unload plugin (default).
{$EXTERNALSYM UD_UNLOAD}
const UD_NONUNLOAD_ACTIVE = $1;  //Don't unload plugin and set active status.
{$EXTERNALSYM UD_NONUNLOAD_ACTIVE}
const UD_NONUNLOAD_NONACTIVE = $2;  //Don't unload plugin and set non-active status.
{$EXTERNALSYM UD_NONUNLOAD_NONACTIVE}
const UD_NONUNLOAD_UNCHANGE = $4;  //Don't unload plugin and don't change active status.
{$EXTERNALSYM UD_NONUNLOAD_UNCHANGE}
const UD_HOTKEY_DODEFAULT = $8;  //Do default hotkey processing.
{$EXTERNALSYM UD_HOTKEY_DODEFAULT}

//Hotkey owner
const HKO_NONE = 0;  //Hotkey is not assigned.
{$EXTERNALSYM HKO_NONE}
const HKO_PLUGINFUNCTION = 1;  //Hotkey assigned to plugin function.
{$EXTERNALSYM HKO_PLUGINFUNCTION}
const HKO_HOTKEYSPLUGIN = 2;  //Hotkey assigned to Hotkeys plugin command.
{$EXTERNALSYM HKO_HOTKEYSPLUGIN}

//Command line options
const CLO_NONOTEPADCMD = $01;  //Don't use MS Notepad compatibility mode when parse command line parameters.
{$EXTERNALSYM CLO_NONOTEPADCMD}
const CLO_GLOBALPRINT = $02;  //Next opened file will be printed.
{$EXTERNALSYM CLO_GLOBALPRINT}
const CLO_MSGOPENCREATEYES = $04;  //Silently create new file, if doesn't exist.
{$EXTERNALSYM CLO_MSGOPENCREATEYES}
const CLO_MSGOPENCREATENO = $08;  //Don't create new file, if doesn't exist.
{$EXTERNALSYM CLO_MSGOPENCREATENO}
const CLO_MSGOPENBINARYYES = $10;  //Silently open file, if binary.
{$EXTERNALSYM CLO_MSGOPENBINARYYES}
const CLO_MSGOPENBINARYNO = $20;  //Don't open file, if binary.
{$EXTERNALSYM CLO_MSGOPENBINARYNO}
const CLO_MSGSAVELOSTSYMBOLSYES = $40;  //Silently save file, even if symbols will be lost.
{$EXTERNALSYM CLO_MSGSAVELOSTSYMBOLSYES}
const CLO_MSGSAVELOSTSYMBOLSNO = $80;  //Don't save file, if symbols will be lost.
{$EXTERNALSYM CLO_MSGSAVELOSTSYMBOLSNO}

//AKD_PARSECMDLINE return value
const PCLE_QUIT = $01;  //Stop parsing command line parameters and close program.
{$EXTERNALSYM PCLE_QUIT}
const PCLE_END = $02;  //Stop parsing command line parameters.
{$EXTERNALSYM PCLE_END}
const PCLE_ONLOAD = $04;  //Done parsing command line parameters on program load (used internally).
{$EXTERNALSYM PCLE_ONLOAD}

//Autodetect flags
const ADT_BINARY_ERROR = $00000001;  //Check if file is binary.
{$EXTERNALSYM ADT_BINARY_ERROR}
const ADT_REG_CODEPAGE = $00000002;  //If last open code page found in registry, then it will be
{$EXTERNALSYM ADT_REG_CODEPAGE}      //used with ADT_DETECT_BOM flag. If not found, then next flags
                                     //will be used ADT_DETECT_CODEPAGE|ADT_DETECT_BOM.
const ADT_DETECT_CODEPAGE = $00000004;  //Detect code page.
{$EXTERNALSYM ADT_DETECT_CODEPAGE}
const ADT_DETECT_BOM = $00000008;  //Detect BOM mark.
{$EXTERNALSYM ADT_DETECT_BOM}
const ADT_NOMESSAGES = $00000010;  //No messages, if errors in autodetect.
{$EXTERNALSYM ADT_NOMESSAGES}

//Autodetect errors
const EDT_SUCCESS = 0;   //Success.
{$EXTERNALSYM EDT_SUCCESS}
const EDT_OPEN = -1;   //Can't open file.
{$EXTERNALSYM EDT_OPEN}
const EDT_ALLOC = -2;   //Can't allocate buffer.
{$EXTERNALSYM EDT_ALLOC}
const EDT_READ = -3;   //Read file error.
{$EXTERNALSYM EDT_READ}
const EDT_BINARY = -4;   //File is binary.
{$EXTERNALSYM EDT_BINARY}

//Open document flags
const OD_ADT_BINARY_ERROR = ADT_BINARY_ERROR;     //See ADT_BINARY_ERROR.
{$EXTERNALSYM OD_ADT_BINARY_ERROR}
const OD_ADT_REG_CODEPAGE = ADT_REG_CODEPAGE;     //See ADT_REG_CODEPAGE.
{$EXTERNALSYM OD_ADT_REG_CODEPAGE}
const OD_ADT_DETECT_CODEPAGE = ADT_DETECT_CODEPAGE;  //See ADT_DETECT_CODEPAGE.
{$EXTERNALSYM OD_ADT_DETECT_CODEPAGE}
const OD_ADT_DETECT_BOM = ADT_DETECT_BOM;       //See ADT_DETECT_BOM.
{$EXTERNALSYM OD_ADT_DETECT_BOM}
const OD_ADT_NOMESSAGES = ADT_NOMESSAGES;       //See ADT_NOMESSAGES.
{$EXTERNALSYM OD_ADT_NOMESSAGES}
const OD_REOPEN = $00000100;           //Don't create new MDI window, use the exited one.
{$EXTERNALSYM OD_REOPEN}
const OD_NOSCROLL = $00000200;           //Don't restore scroll position.
{$EXTERNALSYM OD_NOSCROLL}

//Open document errors
const EOD_SUCCESS = 0;          //Success.
{$EXTERNALSYM EOD_SUCCESS}
const EOD_ADT_OPEN = EDT_OPEN;   //See EDT_OPEN.
{$EXTERNALSYM EOD_ADT_OPEN}
const EOD_ADT_ALLOC = EDT_ALLOC;  //See EDT_ALLOC.
{$EXTERNALSYM EOD_ADT_ALLOC}
const EOD_ADT_READ = EDT_READ;   //See EDT_READ.
{$EXTERNALSYM EOD_ADT_READ}
const EOD_OPEN = -11;        //Can't open file.
{$EXTERNALSYM EOD_OPEN}
const EOD_WINDOW_EXIST = -13;        //File already opened.
{$EXTERNALSYM EOD_WINDOW_EXIST}
const EOD_CODEPAGE_ERROR = -14;        //Code page isn't implemented.
{$EXTERNALSYM EOD_CODEPAGE_ERROR}
const EOD_STOP = -15;        //Stopped from AKDN_OPENDOCUMENT_START.
{$EXTERNALSYM EOD_STOP}
const EOD_STREAMIN = -16;        //Error in EM_STREAMIN.
{$EXTERNALSYM EOD_STREAMIN}
const EOD_DOCUMENTS_LIMIT = -17;        //Documents limit reached in MDI mode.
{$EXTERNALSYM EOD_DOCUMENTS_LIMIT}
const EOD_MSGNO = -20;        //File is skipped.
{$EXTERNALSYM EOD_MSGNO}
const EOD_MSGCANCEL = -21;        //User press cancel.
{$EXTERNALSYM EOD_MSGCANCEL}

//Save document flags
const SD_UPDATE = $00000001;  //Update file info.
{$EXTERNALSYM SD_UPDATE}
const SD_SELECTION = $00000002;  //Save only selection.
{$EXTERNALSYM SD_SELECTION}

//Save document errors
const ESD_SUCCESS = 0;   //Success.
{$EXTERNALSYM ESD_SUCCESS}
const ESD_OPEN = -1;  //Can't open file.
{$EXTERNALSYM ESD_OPEN}
const ESD_WRITE = -2;  //Can't write to file.
{$EXTERNALSYM ESD_WRITE}
const ESD_READONLY = -3;  //File has read-only attribute.
{$EXTERNALSYM ESD_READONLY}
const ESD_CODEPAGE_ERROR = -4;  //Code page isn't implemented.
{$EXTERNALSYM ESD_CODEPAGE_ERROR}
const ESD_STOP = -5;  //Stopped from AKDN_SAVEDOCUMENT_START.
{$EXTERNALSYM ESD_STOP}
const ESD_STREAMOUT = -6;  //Error in EM_STREAMOUT.
{$EXTERNALSYM ESD_STREAMOUT}

//Code pages int
const CP_UNICODE_UTF16LE = 1200;
{$EXTERNALSYM CP_UNICODE_UTF16LE}
const CP_UNICODE_UTF16BE = 1201;
{$EXTERNALSYM CP_UNICODE_UTF16BE}
const CP_UNICODE_UTF32LE = 12000;
{$EXTERNALSYM CP_UNICODE_UTF32LE}
const CP_UNICODE_UTF32BE = 12001;
{$EXTERNALSYM CP_UNICODE_UTF32BE}
const CP_UNICODE_UTF7 = 65000;
{$EXTERNALSYM CP_UNICODE_UTF7}
const CP_UNICODE_UTF8 = 65001;
{$EXTERNALSYM CP_UNICODE_UTF8}
const CP_KOI8_R = 20866;
{$EXTERNALSYM CP_KOI8_R}

//Save settings
const SS_REGISTRY = 1;  //Registry.
{$EXTERNALSYM SS_REGISTRY}
const SS_INI = 2;  //INI file.
{$EXTERNALSYM SS_INI}

//AKD_RECENTFILES flags
const RF_GET = 1;  //Retrieve current recent files info.
{$EXTERNALSYM RF_GET}             //(RECENTFILESTACK **)lParam is a pointer to a variable that receive pointer to a RECENTFILESTACK structure, can be NULL.
                                  //Return value is maximum number of recent files.
const RF_SET = 2;  //Set recent files number.
{$EXTERNALSYM RF_SET}             //(int)lParam is maximum number of recent files.
                                  //Return value is zero.
const RF_READ = 3;  //Read recent files from registry.
{$EXTERNALSYM RF_READ}            //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                                  //Return value is number of records read.
const RF_SAVE = 4;  //Save recent files to registry.
{$EXTERNALSYM RF_SAVE}            //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                                  //Return value is zero.
const RF_CLEAR = 5;  //Clear recent files stack. Use RF_SAVE to save result.
{$EXTERNALSYM RF_CLEAR}           //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                                  //Return value is zero.
const RF_DELETEOLD = 6;  //Delete non-existent recent files records. Use RF_SAVE to save result.
{$EXTERNALSYM RF_DELETEOLD}       //(RECENTFILESTACK *)lParam is a pointer to a RECENTFILESTACK structure, can be NULL.
                                  //Return value is number of records deleted.
const RF_FINDINDEX = 7;  //Find item index in recent files stack by file name.
{$EXTERNALSYM RF_FINDINDEX}       //(wchar_t *)lParam is a pointer to a file name.
                                  //Return value is item index in recent files stack, -1 if error.
const RF_DELETEINDEX = 8;  //Delete item from recent files stack by index.
{$EXTERNALSYM RF_DELETEINDEX}     //(int)lParam is index in recent files array to delete.
                                  //Return value is TRUE - success, FALSE - error.
const RF_FINDITEMBYINDEX = 9;  //Find pointer to a RECENTFILE structure by index.
{$EXTERNALSYM RF_FINDITEMBYINDEX} //(int)lParam is index of item in recent files stack.
                                  //Return value is a pointer to a RECENTFILE structure.
const RF_FINDITEMBYNAME = 10; //Find pointer to a RECENTFILE structure by file name.
{$EXTERNALSYM RF_FINDITEMBYNAME}  //(wchar_t *)lParam is a pointer to a file name.
                                  //Return value is a pointer to a RECENTFILE structure.
const RF_DELETEITEM = 11; //Delete item from recent files stack.
{$EXTERNALSYM RF_DELETEITEM}      //(RECENTFILE *)lParam a pointer to a RECENTFILE structure to delete.
                                  //Return value is zero.
const RF_GETPARAM = 21; //Get recent file parameter by name.
{$EXTERNALSYM RF_GETPARAM}        //(RECENTFILEPARAM *)lParam a pointer to a RECENTFILEPARAM structure with filled "file" and "pParamName" members.
                                  //Return value is a pointer to a real RECENTFILEPARAM structure or NULL if error.
const RF_SETPARAM = 22; //Set recent file parameter.
{$EXTERNALSYM RF_SETPARAM}        //(RECENTFILEPARAM *)lParam a pointer to a RECENTFILEPARAM structure with filled "file", "pParamName" and "pParamValue" members.
                                  //Return value is a pointer to a real RECENTFILEPARAM structure or NULL if error.
const RF_DELETEPARAM = 23; //Delete recent file parameter.
{$EXTERNALSYM RF_DELETEPARAM}     //(RECENTFILEPARAM *)lParam a pointer to a real RECENTFILEPARAM structure.
                                  //Return value is zero.

//AKD_SEARCHHISTORY flags
const SH_GET = 1;  //Retrieve search strings count.
{$EXTERNALSYM SH_GET}
const SH_CLEAR = 2;  //Clear search history.
{$EXTERNALSYM SH_CLEAR}

//AKD_GETMAININFO type

//PLUGINDATA
const MI_AKELDIRA = 1;    //Return: copied chars. (char *)lParam - buffer that receives AkelPad directory string.
{$EXTERNALSYM MI_AKELDIRA}
const MI_AKELDIRW = 2;    //Return: copied chars. (wchar_t *)lParam - buffer that receives AkelPad directory string.
{$EXTERNALSYM MI_AKELDIRW}
const MI_INSTANCEEXE = 3;    //Return: EXE instance.
{$EXTERNALSYM MI_INSTANCEEXE}
const MI_PLUGINSSTACK = 4;    //Return: copied bytes. (HSTACK *)lParam - buffer that receives plugin stack.
{$EXTERNALSYM MI_PLUGINSSTACK}
const MI_SAVESETTINGS = 5;    //Return: see SS_ * defines.
{$EXTERNALSYM MI_SAVESETTINGS}
const MI_WNDPROGRESS = 10;   //Return: progress bar window handle.
{$EXTERNALSYM MI_WNDPROGRESS}
const MI_WNDSTATUS = 11;   //Return: status bar window handle.
{$EXTERNALSYM MI_WNDSTATUS}
const MI_WNDMDICLIENT = 12;   //Return: MDI client window handle.
{$EXTERNALSYM MI_WNDMDICLIENT}
const MI_WNDTAB = 13;   //Return: tab window handle.
{$EXTERNALSYM MI_WNDTAB}
const MI_MENUMAIN = 21;   //Return: main menu handle.
{$EXTERNALSYM MI_MENUMAIN}
const MI_MENURECENTFILES = 22;   //Return: recent files menu handle.
{$EXTERNALSYM MI_MENURECENTFILES}
const MI_MENULANGUAGE = 23;   //Return: language menu handle.
{$EXTERNALSYM MI_MENULANGUAGE}
const MI_MENUPOPUP = 24;   //Return: right click menu handle.
{$EXTERNALSYM MI_MENUPOPUP}
const MI_ICONMAIN = 31;   //Return: main window icon handle.
{$EXTERNALSYM MI_ICONMAIN}
const MI_ACCELGLOBAL = 32;   //Return: global accelerator table (highest priority).
{$EXTERNALSYM MI_ACCELGLOBAL}
const MI_ACCELMAIN = 33;   //Return: main accelerator table (lowest priority).
{$EXTERNALSYM MI_ACCELMAIN}
const MI_OLDWINDOWS = 41;   //Return: non-Unicode Windows.
{$EXTERNALSYM MI_OLDWINDOWS}
const MI_OLDRICHEDIT = 42;   //Return: riched20.dll lower then 5.30 (v3.0).
{$EXTERNALSYM MI_OLDRICHEDIT}
const MI_OLDCOMCTL32 = 43;   //Return: comctl32.dll lower then 4.71.
{$EXTERNALSYM MI_OLDCOMCTL32}
const MI_AKELEDIT = 44;   //Return: AkelEdit control is used.
{$EXTERNALSYM MI_AKELEDIT}
const MI_MDI = 45;   //Return: window mode, see WMD_ * defines.
{$EXTERNALSYM MI_MDI}
const MI_LANGMODULEA = 51;   //Return: copied chars. (char *)lParam - buffer that receives language module string.
{$EXTERNALSYM MI_LANGMODULEA}
const MI_LANGMODULEW = 52;   //Return: copied chars. (wchar_t *)lParam - buffer that receives language module string.
{$EXTERNALSYM MI_LANGMODULEW}
const MI_LANGIDSYSTEM = 53;   //Return: system language ID.
{$EXTERNALSYM MI_LANGIDSYSTEM}
const MI_LANGIDMODULE = 54;   //Return: language module language ID.
{$EXTERNALSYM MI_LANGIDMODULE}

//Compile
const MI_X64 = 101;  //Return: TRUE - x64 version, FALSE - x86 version.
{$EXTERNALSYM MI_X64}
const MI_AKELEDITSTATICBUILD = 102;  //Return: TRUE - AkelEdit is compiled statically, FALSE - AkelEdit is compiled as standalone library.
{$EXTERNALSYM MI_AKELEDITSTATICBUILD}
const MI_AKELPADDLLBUILD = 103;  //Return: TRUE - AkelPad is compiled as library, FALSE - AkelPad is compiled as executable.
{$EXTERNALSYM MI_AKELPADDLLBUILD}

//Manual
const MI_CMDLINEBEGIN = 105;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "CmdLineBegin" string.
{$EXTERNALSYM MI_CMDLINEBEGIN}
const MI_CMDLINEEND = 106;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "CmdLineEnd" string.
{$EXTERNALSYM MI_CMDLINEEND}
const MI_SHOWMODIFY = 110;  //Return: "ShowModify" flags, see SM_ * defines.
{$EXTERNALSYM MI_SHOWMODIFY}
const MI_STATUSPOSTYPE = 111;  //Return: "StatusPosType" cType, see SPT_ * defines.
{$EXTERNALSYM MI_STATUSPOSTYPE}
const MI_STATUSUSERFORMAT = 112;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "StatusUserFormat" string.
{$EXTERNALSYM MI_STATUSUSERFORMAT}
const MI_WORDBREAKCUSTOM = 117;  //Return: "WordBreak" flags.
{$EXTERNALSYM MI_WORDBREAKCUSTOM}
const MI_PAINTOPTIONS = 121;  //Return: "PaintOptions" flags, see PAINT_ * defines.
{$EXTERNALSYM MI_PAINTOPTIONS}
const MI_RICHEDITCLASS = 125;  //Return: "RichEditClass" type.
{$EXTERNALSYM MI_RICHEDITCLASS}
const MI_AKELADMINRESIDENT = 126;  //Return: AkelAdmin.exe resident - TRUE or unloaded immediately after execution - FALSE.
{$EXTERNALSYM MI_AKELADMINRESIDENT}
const MI_DATELOGFORMAT = 129;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "DateLogFormat" string.
{$EXTERNALSYM MI_DATELOGFORMAT}
const MI_DATEINSERTFORMAT = 130;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "DateInsertFormat" string.
{$EXTERNALSYM MI_DATEINSERTFORMAT}
const MI_AKELUPDATEROPTIONS = 131;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "AkelUpdaterOptions" string.
{$EXTERNALSYM MI_AKELUPDATEROPTIONS}
const MI_URLCOMMAND = 132;  //Return: copied chars. (wchar_t *)lParam - buffer that receives "UrlCommand" string.
{$EXTERNALSYM MI_URLCOMMAND}

//Menu
const MI_ONTOP = 141;  //Return: always on top (on\off).
{$EXTERNALSYM MI_ONTOP}
const MI_STATUSBAR = 142;  //Return: show statusbar (on\off).
{$EXTERNALSYM MI_STATUSBAR}
const MI_KEEPSPACE = 146;  //Return: keep left space (on\off).
{$EXTERNALSYM MI_KEEPSPACE}
const MI_WATCHFILE = 147;  //Return: watch cFile change (on\off).
{$EXTERNALSYM MI_WATCHFILE}
const MI_SAVETIME = 148;  //Return: save original file time (on\off).
{$EXTERNALSYM MI_SAVETIME}
const MI_SINGLEOPENFILE = 152;  //Return: single open cFile (on\off).
{$EXTERNALSYM MI_SINGLEOPENFILE}
const MI_SINGLEOPENPROGRAM = 153;  //Return: single open cProgram (on\off).
{$EXTERNALSYM MI_SINGLEOPENPROGRAM}
const MI_TABOPTIONSMDI = 157;  //Return: tab flags, see TAB_ * defines.
{$EXTERNALSYM MI_TABOPTIONSMDI}

//Settings dialog
const MI_EXECUTECOMMAND = 171;  //Return: copied chars. (wchar_t *)lParam - buffer that receives execution command string.
{$EXTERNALSYM MI_EXECUTECOMMAND}
const MI_EXECUTEDIRECTORY = 172;  //Return: copied chars. (wchar_t *)lParam - buffer that receives execution directory string.
{$EXTERNALSYM MI_EXECUTEDIRECTORY}
const MI_CODEPAGELIST = 176;  //Return: copied bytes. (int *)lParam - buffer that receives array of codepages, last element in array is zero.
{$EXTERNALSYM MI_CODEPAGELIST}
const MI_DEFAULTCODEPAGE = 177;  //Return: default codepage.
{$EXTERNALSYM MI_DEFAULTCODEPAGE}
const MI_DEFAULTBOM = 178;  //Return: default BOM.
{$EXTERNALSYM MI_DEFAULTBOM}
const MI_NEWFILECODEPAGE = 179;  //Return: new file codepage.
{$EXTERNALSYM MI_NEWFILECODEPAGE}
const MI_NEWFILEBOM = 180;  //Return: new file BOM.
{$EXTERNALSYM MI_NEWFILEBOM}
const MI_NEWFILENEWLINE = 181;  //Return: new file new line, see NEWLINE_* defines.
{$EXTERNALSYM MI_NEWFILENEWLINE}
const MI_LANGCODEPAGERECOGNITION = 183;  //Return: codepage recognition language defined as LANGID.
{$EXTERNALSYM MI_LANGCODEPAGERECOGNITION}
const MI_CODEPAGERECOGNITIONBUFFER = 184;  //Return: size of codepage recognition buffer.
{$EXTERNALSYM MI_CODEPAGERECOGNITIONBUFFER}
const MI_SAVEPOSITIONS = 192;  //Return: save recent file positions (on\off).
{$EXTERNALSYM MI_SAVEPOSITIONS}
const MI_SAVECODEPAGES = 193;  //Return: save recent file codepages (on\off).
{$EXTERNALSYM MI_SAVECODEPAGES}
const MI_RECENTFILES = 194;  //Return: number of recent files.
{$EXTERNALSYM MI_RECENTFILES}
const MI_SEARCHSTRINGS = 198;  //Return: number of search strings.
{$EXTERNALSYM MI_SEARCHSTRINGS}
const MI_FILETYPESOPEN = 202;  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated cFile types to open.
{$EXTERNALSYM MI_FILETYPESOPEN}
const MI_FILETYPESEDIT = 203;  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated cFile types to edit.
{$EXTERNALSYM MI_FILETYPESEDIT}
const MI_FILETYPESPRINT = 204;  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated cFile types to print.
{$EXTERNALSYM MI_FILETYPESPRINT}
const MI_FILETYPESASSOCIATED = 205;  //Return: associated cFile types, see FTA_ * defines.
{$EXTERNALSYM MI_FILETYPESASSOCIATED}
const MI_KEYBLAYOUTOPTIONS = 209;  //Return: keyboard layout options, see KLO_ * defines.
{$EXTERNALSYM MI_KEYBLAYOUTOPTIONS}
const MI_SILENTCLOSEEMPTYMDI = 213;  //Return: silently close unsaved empty MDI tab (on\off).
{$EXTERNALSYM MI_SILENTCLOSEEMPTYMDI}
const MI_DATELOG = 217;  //Return: insert date if file has .LOG at the beginning (on\off).
{$EXTERNALSYM MI_DATELOG}
const MI_SAVEINREADONLYMSG = 221;  //Return: save in read-only files warning (on\off).
{$EXTERNALSYM MI_SAVEINREADONLYMSG}
const MI_DEFAULTSAVEEXT = 224;  //Return: copied chars. (wchar_t *)lParam - buffer that receives default saving extension string.
{$EXTERNALSYM MI_DEFAULTSAVEEXT}
const MI_SEARCHOPTIONS = 228;  //Return: search options, see FRF_ * defines.
{$EXTERNALSYM MI_SEARCHOPTIONS}

//Print dialog
const MI_PRINTMARGINS = 251;  //Return: copied bytes. (RECT *)lParam - buffer that receives print margins.
{$EXTERNALSYM MI_PRINTMARGINS}
const MI_PRINTCOLOR = 255;  //Return: color printing, see PRNC_ * defines.
{$EXTERNALSYM MI_PRINTCOLOR}
const MI_PRINTHEADERENABLE = 259;  //Return: enable print header (on\off).
{$EXTERNALSYM MI_PRINTHEADERENABLE}
const MI_PRINTHEADER = 260;  //Return: copied chars. (wchar_t *)lParam - buffer that receives print header string.
{$EXTERNALSYM MI_PRINTHEADER}
const MI_PRINTFOOTERENABLE = 261;  //Return: enable print footer (on\off).
{$EXTERNALSYM MI_PRINTFOOTERENABLE}
const MI_PRINTFOOTER = 262;  //Return: copied chars. (wchar_t *)lParam - buffer that receives print footer string.
{$EXTERNALSYM MI_PRINTFOOTER}
const MI_PRINTFONTENABLE = 266;  //Return: enable print font (on\off).
{$EXTERNALSYM MI_PRINTFONTENABLE}
const MI_PRINTFONTW = 267;  //Return: copied bytes. (LOGFONTW *)lParam - buffer that receives print font data.
{$EXTERNALSYM MI_PRINTFONTW}

//Open dialog
const MI_LASTDIR = 281;  //Return: copied chars. (wchar_t *)lParam - buffer that receives last directory of open dialog.
{$EXTERNALSYM MI_LASTDIR}
const MI_SHOWPLACESBAR = 282;  //Return: show places bar in open dialog (on\off).
{$EXTERNALSYM MI_SHOWPLACESBAR}


//AKD_SETMAININFO type

//PLUGINDATA
const MIS_SAVESETTINGS = 5;   //(int)lParam - see SS_* defines.
{$EXTERNALSYM MIS_SAVESETTINGS}
const MIS_MDI = 45;  //(int)lParam - window mode, see WMD_* defines. Required program restart.
{$EXTERNALSYM MIS_MDI}
const MIS_LANGMODULEA = 51;  //(char *)lParam - language module string. Required program restart.
{$EXTERNALSYM MIS_LANGMODULEA}
const MIS_LANGMODULEW = 52;  //(wchar_t *)lParam - language module string. Required program restart.
{$EXTERNALSYM MIS_LANGMODULEW}
//Manual
const MIS_CMDLINEBEGIN = 105; //(wchar_t *)lParam - "CmdLineBegin" string.
{$EXTERNALSYM MIS_CMDLINEBEGIN}
const MIS_CMDLINEEND = 106; //(wchar_t *)lParam - "CmdLineEnd" string.
{$EXTERNALSYM MIS_CMDLINEEND}
const MIS_SHOWMODIFY = 110; //(DWORD)lParam - "ShowModify" flags, see SM_* defines.
{$EXTERNALSYM MIS_SHOWMODIFY}
const MIS_STATUSPOSTYPE = 111; //(DWORD)lParam - "StatusPosType" type, see SPT_* defines.
{$EXTERNALSYM MIS_STATUSPOSTYPE}
const MIS_STATUSUSERFORMAT = 112; //(wchar_t *)lParam - "StatusUserFormat" string.
{$EXTERNALSYM MIS_STATUSUSERFORMAT}
const MIS_WORDBREAKCUSTOM = 117; //(DWORD)lParam - "WordBreak" flags.
{$EXTERNALSYM MIS_WORDBREAKCUSTOM}
const MIS_PAINTOPTIONS = 121; //(DWORD)lParam - "PaintOptions" flags, see PAINT_* defines.
{$EXTERNALSYM MIS_PAINTOPTIONS}
const MIS_RICHEDITCLASS = 125; //(BOOL)lParam - "RichEditClass" type.
{$EXTERNALSYM MIS_RICHEDITCLASS}
const MIS_AKELADMINRESIDENT = 126; //(BOOL)lParam - AkelAdmin.exe resident - TRUE or unloaded immediately after execution - FALSE.
{$EXTERNALSYM MIS_AKELADMINRESIDENT}
const MIS_DATELOGFORMAT = 129; //(wchar_t *)lParam - "DateLogFormat" string.
{$EXTERNALSYM MIS_DATELOGFORMAT}
const MIS_DATEINSERTFORMAT = 130; //(wchar_t *)lParam - "DateInsertFormat" string.
{$EXTERNALSYM MIS_DATEINSERTFORMAT}
const MIS_AKELUPDATEROPTIONS = 131; //(wchar_t *)lParam - "AkelUpdaterOptions" string.
{$EXTERNALSYM MIS_AKELUPDATEROPTIONS}
const MIS_URLCOMMAND = 132; //(wchar_t *)lParam - "UrlCommand" string.
{$EXTERNALSYM MIS_URLCOMMAND}
//Menu
const MIS_ONTOP = 141; //(BOOL)lParam - always on top (on\off).
{$EXTERNALSYM MIS_ONTOP}
const MIS_STATUSBAR = 142; //(BOOL)lParam - show statusbar (on\off).
{$EXTERNALSYM MIS_STATUSBAR}
const MIS_KEEPSPACE = 146; //(BOOL)lParam - keep left space (on\off).
{$EXTERNALSYM MIS_KEEPSPACE}
const MIS_WATCHFILE = 147; //(BOOL)lParam - watch file change (on\off).
{$EXTERNALSYM MIS_WATCHFILE}
const MIS_SAVETIME = 148; //(BOOL)lParam - save original file time (on\off).
{$EXTERNALSYM MIS_SAVETIME}
const MIS_SINGLEOPENFILE = 152; //(BOOL)lParam - single open file (on\off).
{$EXTERNALSYM MIS_SINGLEOPENFILE}
const MIS_SINGLEOPENPROGRAM = 153; //(BOOL)lParam - single open program (on\off).
{$EXTERNALSYM MIS_SINGLEOPENPROGRAM}
const MIS_TABOPTIONSMDI = 157; //(DWORD)lParam - tab flags, see TAB_* defines.
{$EXTERNALSYM MIS_TABOPTIONSMDI}
//Settings dialog
const MIS_EXECUTECOMMAND = 171; //(wchar_t *)lParam - execution command string.
{$EXTERNALSYM MIS_EXECUTECOMMAND}
const MIS_EXECUTEDIRECTORY = 172; //(wchar_t *)lParam - execution directory string.
{$EXTERNALSYM MIS_EXECUTEDIRECTORY}
const MIS_CODEPAGELIST = 176; //(int *)lParam - array of codepages, last element in array is zero.
{$EXTERNALSYM MIS_CODEPAGELIST}
const MIS_DEFAULTCODEPAGE = 177; //(int)lParam - default codepage.
{$EXTERNALSYM MIS_DEFAULTCODEPAGE}
const MIS_DEFAULTBOM = 178; //(BOOL)lParam - default BOM.
{$EXTERNALSYM MIS_DEFAULTBOM}
const MIS_NEWFILECODEPAGE = 179; //(int)lParam - new file codepage.
{$EXTERNALSYM MIS_NEWFILECODEPAGE}
const MIS_NEWFILEBOM = 180; //(BOOL)lParam - new file BOM.
{$EXTERNALSYM MIS_NEWFILEBOM}
const MIS_NEWFILENEWLINE = 181; //(int)lParam - new file new line, see NEWLINE_* defines.
{$EXTERNALSYM MIS_NEWFILENEWLINE}
const MIS_LANGCODEPAGERECOGNITION = 183; //(DWORD)lParam - codepage recognition language defined as LANGID.
{$EXTERNALSYM MIS_LANGCODEPAGERECOGNITION}
const MIS_CODEPAGERECOGNITIONBUFFER = 184; //(DWORD)lParam - size of codepage recognition buffer.
{$EXTERNALSYM MIS_CODEPAGERECOGNITIONBUFFER}
const MIS_SAVEPOSITIONS = 192; //(BOOL)lParam - save recent file positions (on\off).
{$EXTERNALSYM MIS_SAVEPOSITIONS}
const MIS_SAVECODEPAGES = 193; //(BOOL)lParam - save recent file codepages (on\off).
{$EXTERNALSYM MIS_SAVECODEPAGES}
const MIS_RECENTFILES = 194; //(int)lParam - number of recent files.
{$EXTERNALSYM MIS_RECENTFILES}
const MIS_SEARCHSTRINGS = 198; //(int)lParam - number of search strings.
{$EXTERNALSYM MIS_SEARCHSTRINGS}
const MIS_FILETYPESOPEN = 202; //(wchar_t *)lParam - associated file types to open.
{$EXTERNALSYM MIS_FILETYPESOPEN}
const MIS_FILETYPESEDIT = 203; //(wchar_t *)lParam - associated file types to edit.
{$EXTERNALSYM MIS_FILETYPESEDIT}
const MIS_FILETYPESPRINT = 204; //(wchar_t *)lParam - associated file types to print.
{$EXTERNALSYM MIS_FILETYPESPRINT}
const MIS_FILETYPESASSOCIATED = 205; //(DWORD)lParam - associated file types, see FTA_* defines.
{$EXTERNALSYM MIS_FILETYPESASSOCIATED}
const MIS_KEYBLAYOUTOPTIONS = 209; //(DWORD)lParam - keyboard layout options, see KLO_* defines.
{$EXTERNALSYM MIS_KEYBLAYOUTOPTIONS}
const MIS_SILENTCLOSEEMPTYMDI = 213; //(BOOL)lParam - silently close unsaved empty MDI tab (on\off).
{$EXTERNALSYM MIS_SILENTCLOSEEMPTYMDI}
const MIS_DATELOG = 217; //(BOOL)lParam - insert date if file has .LOG at the beginning (on\off).
{$EXTERNALSYM MIS_DATELOG}
const MIS_SAVEINREADONLYMSG = 221; //(BOOL)lParam - save in read-only files warning (on\off).
{$EXTERNALSYM MIS_SAVEINREADONLYMSG}
const MIS_DEFAULTSAVEEXT = 224; //(wchar_t *)lParam - default saving extension string.
{$EXTERNALSYM MIS_DEFAULTSAVEEXT}
const MIS_SEARCHOPTIONS = 228; //(DWORD)lParam - search options, see FRF_* defines.
{$EXTERNALSYM MIS_SEARCHOPTIONS}
//Print dialog
const MIS_PRINTMARGINS = 251; //(RECT *)lParam - print margins.
{$EXTERNALSYM MIS_PRINTMARGINS}
const MIS_PRINTCOLOR = 255; //(DWORD)lParam - color printing, see PRNC_* defines.
{$EXTERNALSYM MIS_PRINTCOLOR}
const MIS_PRINTHEADERENABLE = 259; //(BOOL)lParam - enable print header (on\off).
{$EXTERNALSYM MIS_PRINTHEADERENABLE}
const MIS_PRINTHEADER = 260; //(wchar_t *)lParam - print header string.
{$EXTERNALSYM MIS_PRINTHEADER}
const MIS_PRINTFOOTERENABLE = 261; //(BOOL)lParam - enable print footer (on\off).
{$EXTERNALSYM MIS_PRINTFOOTERENABLE}
const MIS_PRINTFOOTER = 262; //(wchar_t *)lParam - print footer string.
{$EXTERNALSYM MIS_PRINTFOOTER}
const MIS_PRINTFONTENABLE = 266; //(BOOL)lParam - enable print font (on\off).
{$EXTERNALSYM MIS_PRINTFONTENABLE}
const MIS_PRINTFONTW = 267; //(LOGFONTW *)lParam - print font data.
{$EXTERNALSYM MIS_PRINTFONTW}
//Open dialog
const MIS_LASTDIR = 281; //(wchar_t *)lParam - last directory of open dialog.
{$EXTERNALSYM MIS_LASTDIR}
const MIS_SHOWPLACESBAR = 282; //(BOOL)lParam - show places bar in open dialog (on\off).
{$EXTERNALSYM MIS_SHOWPLACESBAR}

//AKD_GETFRAMEINFO type. See FRAMEDATA members description.
const FI_WNDEDITPARENT = 1;
{$EXTERNALSYM FI_WNDEDITPARENT}
const FI_WNDEDIT = 2;
{$EXTERNALSYM FI_WNDEDIT}
const FI_DOCEDIT = 3;
{$EXTERNALSYM FI_DOCEDIT}
const FI_WNDMASTER = 4;
{$EXTERNALSYM FI_WNDMASTER}
const FI_DOCMASTER = 5;
{$EXTERNALSYM FI_DOCMASTER}
const FI_WNDCLONE1 = 6;
{$EXTERNALSYM FI_WNDCLONE1}
const FI_DOCCLONE1 = 7;
{$EXTERNALSYM FI_DOCCLONE1}
const FI_WNDCLONE2 = 8;
{$EXTERNALSYM FI_WNDCLONE2}
const FI_DOCCLONE2 = 9;
{$EXTERNALSYM FI_DOCCLONE2}
const FI_WNDCLONE3 = 10;
{$EXTERNALSYM FI_WNDCLONE3}
const FI_DOCCLONE3 = 11;
{$EXTERNALSYM FI_DOCCLONE3}
const FI_CODEPAGE = 12;
{$EXTERNALSYM FI_CODEPAGE}
const FI_BOM = 13;
{$EXTERNALSYM FI_BOM}
const FI_NEWLINE = 14;
{$EXTERNALSYM FI_NEWLINE}
const FI_MODIFIED = 15;
{$EXTERNALSYM FI_MODIFIED}
const FI_READONLY = 16;
{$EXTERNALSYM FI_READONLY}
const FI_WORDWRAP = 17;
{$EXTERNALSYM FI_WORDWRAP}
const FI_OVERTYPE = 18;
{$EXTERNALSYM FI_OVERTYPE}
const FI_FILEA = 31;
{$EXTERNALSYM FI_FILEA}
const FI_FILEW = 32;
{$EXTERNALSYM FI_FILEW}
const FI_FILELEN = 33;
{$EXTERNALSYM FI_FILELEN}
const FI_STREAMOFFSET = 34;
{$EXTERNALSYM FI_STREAMOFFSET}
const FI_ICONHANDLE = 38;
{$EXTERNALSYM FI_ICONHANDLE}
const FI_ICONINDEX = 39;
{$EXTERNALSYM FI_ICONINDEX}
const FI_RECTEDIT = 43;
{$EXTERNALSYM FI_RECTEDIT}
const FI_RECTMASTER = 44;
{$EXTERNALSYM FI_RECTMASTER}
const FI_EDITFONTW = 48;
{$EXTERNALSYM FI_EDITFONTW}
const FI_TABSTOPSIZE = 52;
{$EXTERNALSYM FI_TABSTOPSIZE}
const FI_TABSTOPASSPACES = 53;
{$EXTERNALSYM FI_TABSTOPASSPACES}
const FI_UNDOLIMIT = 57;
{$EXTERNALSYM FI_UNDOLIMIT}
const FI_DETAILEDUNDO = 58;
{$EXTERNALSYM FI_DETAILEDUNDO}
const FI_WRAPTYPE = 62;
{$EXTERNALSYM FI_WRAPTYPE}
const FI_WRAPLIMIT = 63;
{$EXTERNALSYM FI_WRAPLIMIT}
const FI_MARKER = 64;
{$EXTERNALSYM FI_MARKER}
const FI_CARETWIDTH = 68;
{$EXTERNALSYM FI_CARETWIDTH}
const FI_CARETOPTIONS = 69;
{$EXTERNALSYM FI_CARETOPTIONS}
const FI_ALTLINEFILL = 73;
{$EXTERNALSYM FI_ALTLINEFILL}
const FI_ALTLINESKIP = 74;
{$EXTERNALSYM FI_ALTLINESKIP}
const FI_ALTLINEBORDER = 75;
{$EXTERNALSYM FI_ALTLINEBORDER}
const FI_RECTMARGINS = 79;
{$EXTERNALSYM FI_RECTMARGINS}
const FI_LINEGAP = 83;
{$EXTERNALSYM FI_LINEGAP}
const FI_MOUSEOPTIONS = 87;
{$EXTERNALSYM FI_MOUSEOPTIONS}
const FI_CLICKURL = 88;
{$EXTERNALSYM FI_CLICKURL}
const FI_SHOWURL = 89;
{$EXTERNALSYM FI_SHOWURL}
const FI_URLPREFIXESENABLE = 93;
{$EXTERNALSYM FI_URLPREFIXESENABLE}
const FI_URLPREFIXES = 94;
{$EXTERNALSYM FI_URLPREFIXES}
const FI_URLDELIMITERSENABLE = 98;
{$EXTERNALSYM FI_URLDELIMITERSENABLE}
const FI_URLLEFTDELIMITERS = 99;
{$EXTERNALSYM FI_URLLEFTDELIMITERS}
const FI_URLRIGHTDELIMITERS = 103;
{$EXTERNALSYM FI_URLRIGHTDELIMITERS}
const FI_WORDDELIMITERSENABLE = 107;
{$EXTERNALSYM FI_WORDDELIMITERSENABLE}
const FI_WORDDELIMITERS = 108;
{$EXTERNALSYM FI_WORDDELIMITERS}
const FI_WRAPDELIMITERSENABLE = 112;
{$EXTERNALSYM FI_WRAPDELIMITERSENABLE}
const FI_WRAPDELIMITERS = 113;
{$EXTERNALSYM FI_WRAPDELIMITERS}
const FI_MAPPEDPRINTWIDTH = 121;
{$EXTERNALSYM FI_MAPPEDPRINTWIDTH}
const FI_INPUTLOCALE = 125;
{$EXTERNALSYM FI_INPUTLOCALE}
const FI_LOCKINHERIT = 129;
{$EXTERNALSYM FI_LOCKINHERIT}
const FI_FILETIME = 133;
{$EXTERNALSYM FI_FILETIME}
const FI_COLORS = 137;
{$EXTERNALSYM FI_COLORS}
const FI_BKIMAGEFILE = 140;
{$EXTERNALSYM FI_BKIMAGEFILE}
const FI_BKIMAGEALPHA = 141;
{$EXTERNALSYM FI_BKIMAGEALPHA}

//AKD_SETFRAMEINFO type.
const FIS_TABSTOPSIZE = 1;   //(int)FRAMEINFO.dwData - tabulation size in characters.
{$EXTERNALSYM FIS_TABSTOPSIZE}
const FIS_TABSTOPASSPACES = 2;   //(BOOL)FRAMEINFO.dwData - insert tabulation as spaces.
{$EXTERNALSYM FIS_TABSTOPASSPACES}
const FIS_UNDOLIMIT = 6;   //(int)FRAMEINFO.dwData - undo limit.
{$EXTERNALSYM FIS_UNDOLIMIT}
const FIS_DETAILEDUNDO = 7;   //(BOOL)FRAMEINFO.dwData - detailed undo.
{$EXTERNALSYM FIS_DETAILEDUNDO}
const FIS_WRAP = 11;  //(DWORD)FRAMEINFO.dwData - the low-order word contains the wrap type (see AEWW_ * defines in AkelEdit.h). The high-order word contains the wrap limit in characters.
{$EXTERNALSYM FIS_WRAP}
const FIS_MARKER = 12;  //(int)FRAMEINFO.dwData - column marker position.
{$EXTERNALSYM FIS_MARKER}
const FIS_CARETWIDTH = 16;  //(int)FRAMEINFO.dwData - caret width.
{$EXTERNALSYM FIS_CARETWIDTH}
const FIS_CARETOPTIONS = 17;  //(DWORD)FRAMEINFO.dwData - caret options, see CO_ * defines.
{$EXTERNALSYM FIS_CARETOPTIONS}
const FIS_ALTLINES = 21;  //(DWORD)FRAMEINFO.dwData - alternating lines. The low-order word of FRAMEINFO.dwData contains the skip interval in lines. The high-order word of FRAMEINFO.dwData contains the fill interval in lines.
{$EXTERNALSYM FIS_ALTLINES}
const FIS_ALTLINEBORDER = 22;  //(BOOL)FRAMEINFO.dwData - draw alternating lines border.
{$EXTERNALSYM FIS_ALTLINEBORDER}
const FIS_RECTMARGINS = 26;  //(TRect *)FRAMEINFO.dwData - edit margins. FRAMEINFO.dwData contain left, top, right, bottom margins in pixels.
{$EXTERNALSYM FIS_RECTMARGINS}
const FIS_LINEGAP = 30;  //(int)FRAMEINFO.dwData - gap between lines.
{$EXTERNALSYM FIS_LINEGAP}
const FIS_MOUSEOPTIONS = 34;  //(DWORD)FRAMEINFO.dwData - mouse options, see MO_ * defines.
{$EXTERNALSYM FIS_MOUSEOPTIONS}
const FIS_SHOWURL = 35;  //(BOOL)FRAMEINFO.dwData - show URL.
{$EXTERNALSYM FIS_SHOWURL}
const FIS_CLICKURL = 36;  //(int)FRAMEINFO.dwData - number of clicks to open URL.
{$EXTERNALSYM FIS_CLICKURL}
const FIS_URLPREFIXESENABLE = 40;  //(BOOL)FRAMEINFO.dwData - URL prefixes enable.
{$EXTERNALSYM FIS_URLPREFIXESENABLE}
const FIS_URLPREFIXES = 41;  //(wchar_t *)FRAMEINFO.dwData - URL prefixes.
{$EXTERNALSYM FIS_URLPREFIXES}
const FIS_URLDELIMITERSENABLE = 45;  //(BOOL)FRAMEINFO.dwData - URL delimiters enable.
{$EXTERNALSYM FIS_URLDELIMITERSENABLE}
const FIS_URLLEFTDELIMITERS = 46;  //(wchar_t *)FRAMEINFO.dwData - URL left delimiters.
{$EXTERNALSYM FIS_URLLEFTDELIMITERS}
const FIS_URLRIGHTDELIMITERS = 50;  //(wchar_t *)FRAMEINFO.dwData - URL right delimiters.
{$EXTERNALSYM FIS_URLRIGHTDELIMITERS}
const FIS_WORDDELIMITERSENABLE = 54;  //(BOOL)FRAMEINFO.dwData - word delimiters enabled.
{$EXTERNALSYM FIS_WORDDELIMITERSENABLE}
const FIS_WORDDELIMITERS = 55;  //(wchar_t *)FRAMEINFO.dwData - word delimiters.
{$EXTERNALSYM FIS_WORDDELIMITERS}
const FIS_WRAPDELIMITERSENABLE = 59;  //(BOOL)FRAMEINFO.dwData - wrap delimiters enabled.
{$EXTERNALSYM FIS_WRAPDELIMITERSENABLE}
const FIS_WRAPDELIMITERS = 60;  //(wchar_t *)FRAMEINFO.dwData - wrap delimiters.
{$EXTERNALSYM FIS_WRAPDELIMITERS}
const FIS_LOCKINHERIT = 68;  //(DWORD)FRAMEINFO.dwData - lock inherit new document settings from current document. FRAMEINFO.dwData contain lock inherit flags, see LI_ * defines.
{$EXTERNALSYM FIS_LOCKINHERIT}
const FIS_COLORS = 72;  //(AECOLORS *)FRAMEINFO.dwData - set colors.
{$EXTERNALSYM FIS_COLORS}
const FIS_BKIMAGE = 73;  //(BKIMAGE *)FRAMEINFO.dwData - set background image.
{$EXTERNALSYM FIS_BKIMAGE}

//New line format
const NEWLINE_WIN = 1;  //Windows/DOS new line format (\r\n).
{$EXTERNALSYM NEWLINE_WIN}
const NEWLINE_UNIX = 2;  //Unix new line format (\n).
{$EXTERNALSYM NEWLINE_UNIX}
const NEWLINE_MAC = 3;  //MacOS new line format (\r).
{$EXTERNALSYM NEWLINE_MAC}

//AKD_GOTO type
const GT_LINE = $1;           //Go to "Line:Column".
{$EXTERNALSYM GT_LINE}
const GT_OFFSETBYTE = $2;     //Go to offset counted in bytes.
{$EXTERNALSYM GT_OFFSETBYTE}
const GT_OFFSETCHAR = $4;
{$EXTERNALSYM GT_OFFSETCHAR}  //Go to offset counted in characters (not present in "Go to..." dialog).

//Caret options
const CO_CARETOUTEDGE = $00000001;  //Allow caret moving out of the line edge.
{$EXTERNALSYM CO_CARETOUTEDGE}
const CO_CARETVERTLINE = $00000002;  //Draw caret vertical line.
{$EXTERNALSYM CO_CARETVERTLINE}
const CO_CARETACTIVELINE = $00000004;  //Draw active line.
{$EXTERNALSYM CO_CARETACTIVELINE}
const CO_CARETACTIVELINEBORDER = $00000008;  //Draw active line border.
{$EXTERNALSYM CO_CARETACTIVELINEBORDER}

//Mouse options
const MO_LEFTMARGINSELECTION = $00000001;  //Enables left margin line selection with mouse.
{$EXTERNALSYM MO_LEFTMARGINSELECTION}
const MO_RICHEDITMOUSE = $00000002;  //After WM_LBUTTONUP message capture operations doesn't stopped.
{$EXTERNALSYM MO_RICHEDITMOUSE}
const MO_MOUSEDRAGGING = $00000004;  //Enables OLE text dragging.
{$EXTERNALSYM MO_MOUSEDRAGGING}
const MO_RCLICKMOVECARET = $00000008;  //WM_RBUTTONDOWN message moves caret to a click position.
{$EXTERNALSYM MO_RCLICKMOVECARET}
const MO_NONEWLINEMOUSESELECT = $00000010;  //Triple click and left margin click selects only line contents without new line.
{$EXTERNALSYM MO_NONEWLINEMOUSESELECT}
const MO_NOWHEELFONTCHANGE = $00000020;  //Don't change font size with middle button scroll and Ctrl key.
{$EXTERNALSYM MO_NOWHEELFONTCHANGE}
const MO_MARGINSELUNWRAPLINE = $00000040;  //Left margin line selection with mouse selects all wrapped line.
{$EXTERNALSYM MO_MARGINSELUNWRAPLINE}

//Keyboard layout options
const KLO_REMEMBERLAYOUT = $00000001;  //Remember keyboard layout for each tab (MDI).
{$EXTERNALSYM KLO_REMEMBERLAYOUT}
const KLO_SWITCHLAYOUT = $00000002;  //Auto case keyboard layout according to text under caret.
{$EXTERNALSYM KLO_SWITCHLAYOUT}

//Paint options
const PAINT_PAINTGROUP = $00000001;  //Paint text by group of characters (default is character by character).
{$EXTERNALSYM PAINT_PAINTGROUP}      //With this flag some text recognition programs could start to work, printer could print faster, but highlighted symbols and combined unicode symbols can be drawn differently and editing of whose characters may become uncomfortable.
const PAINT_NONEWLINEDRAW = $00000002;  //Disables drawing new line selection as space character.
{$EXTERNALSYM PAINT_NONEWLINEDRAW}
const PAINT_ENTIRENEWLINEDRAW = $00000004;  //Draw new line selection to the right edge.
{$EXTERNALSYM PAINT_ENTIRENEWLINEDRAW}
const PAINT_HIDESEL = $00000008;  //Hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus.
{$EXTERNALSYM PAINT_HIDESEL}
const PAINT_NOMARKERAFTERLASTLINE = $00000010;  //Disables marker painting after last line.
{$EXTERNALSYM PAINT_NOMARKERAFTERLASTLINE}
const PAINT_HIDENOSCROLL = $00000100;  //Hides scroll bars instead of disabling them when they are not needed.
{$EXTERNALSYM PAINT_HIDENOSCROLL}
const PAINT_STATICEDGE = $00000200;  //Draw thin edit window border.
{$EXTERNALSYM PAINT_STATICEDGE}
const PAINT_NOEDGE = $00000400;  //Draw no edit window border.
{$EXTERNALSYM PAINT_NOEDGE}

//Status bar position type
const SPT_COLUMN = $00000001;  //"Line:Column". By default: "Line:Symbol".
{$EXTERNALSYM SPT_COLUMN}
const SPT_LINEWRAP = $00000002;  //Wrap line numbers. By default: Non-wrap line numbers.
{$EXTERNALSYM SPT_LINEWRAP}

//Show modify
const SM_NONE = $00000000;
{$EXTERNALSYM SM_NONE}
const SM_STATUSBAR = $00000001;
{$EXTERNALSYM SM_STATUSBAR}
const SM_MAINTITLE_SDI = $00000002;
{$EXTERNALSYM SM_MAINTITLE_SDI}
const SM_FRAMETITLE_MDI = $00000004;
{$EXTERNALSYM SM_FRAMETITLE_MDI}
const SM_TABTITLE_MDI = $00000008;
{$EXTERNALSYM SM_TABTITLE_MDI}

//Status bar
const SBP_POSITION = 0;
{$EXTERNALSYM SBP_POSITION}
const SBP_MODIFY = 1;
{$EXTERNALSYM SBP_MODIFY}
const SBP_INSERT = 2;
{$EXTERNALSYM SBP_INSERT}
const SBP_NEWLINE = 3;
{$EXTERNALSYM SBP_NEWLINE}
const SBP_CODEPAGE = 4;
{$EXTERNALSYM SBP_CODEPAGE}
const SBP_USER = 5;
{$EXTERNALSYM SBP_USER}

//Tab options MDI
const TAB_VIEW_NONE = $00000001;                //Hide tab bar.
{$EXTERNALSYM TAB_VIEW_NONE}
const TAB_VIEW_TOP = $00000002;                 //Show tab bar on top.
{$EXTERNALSYM TAB_VIEW_TOP}
const TAB_VIEW_BOTTOM = $00000004;              //Show tab bar at the bottom.
{$EXTERNALSYM TAB_VIEW_BOTTOM}
const TAB_TYPE_STANDARD = $00000100;            //Standard tab bar style.
{$EXTERNALSYM TAB_TYPE_STANDARD}
const TAB_TYPE_BUTTONS = $00000200;             //Buttons tab bar style.
{$EXTERNALSYM TAB_TYPE_BUTTONS}
const TAB_TYPE_FLATBUTTONS = $00000400;         //Flat buttons tab bar style.
{$EXTERNALSYM TAB_TYPE_FLATBUTTONS}
const TAB_SWITCH_NEXTPREV = $00010000;          //Switch between tabs: Next-Previous.
{$EXTERNALSYM TAB_SWITCH_NEXTPREV}
const TAB_SWITCH_RIGHTLEFT = $00020000;         //Switch between tabs: Left-Right.
{$EXTERNALSYM TAB_SWITCH_RIGHTLEFT}
const TAB_ADD_AFTERCURRENT = $00080000;         //Create tabs after the current one.
{$EXTERNALSYM TAB_ADD_AFTERCURRENT}
const TAB_NOADD_LBUTTONDBLCLK = $00100000;      //Don't create new tab by left button double click on the tab bar.
{$EXTERNALSYM TAB_NOADD_LBUTTONDBLCLK}
const TAB_NOADD_MBUTTONDOWN = $00200000;        //Don't create new tab by middle button click on the tab bar.
{$EXTERNALSYM TAB_NOADD_MBUTTONDOWN}
const TAB_NODEL_LBUTTONDBLCLK = $00400000;      //Don't close tab by left button double click on the tab.
{$EXTERNALSYM TAB_NODEL_LBUTTONDBLCLK}
const TAB_NODEL_MBUTTONDOWN = $00800000;        //Don't close tab by middle button click on the tab.
{$EXTERNALSYM TAB_NODEL_MBUTTONDOWN}

//File types association
const FTA_ASSOCIATE = $00000001;  //Internal.
{$EXTERNALSYM FTA_ASSOCIATE}
const FTA_DEASSOCIATE = $00000002;  //Internal.
{$EXTERNALSYM FTA_DEASSOCIATE}
const FTA_OPEN = $00000004;  //Open file types.
{$EXTERNALSYM FTA_OPEN}
const FTA_EDIT = $00000008;  //Edit file types.
{$EXTERNALSYM FTA_EDIT}
const FTA_PRINT = $00000010;  //Print file types.
{$EXTERNALSYM FTA_PRINT}

//Color printing
const PRNC_TEXT = $01;  //Print colored text.
{$EXTERNALSYM PRNC_TEXT}
const PRNC_BACKGROUND = $02;  //Print on colored background.
{$EXTERNALSYM PRNC_BACKGROUND}
const PRNC_SELECTION = $04;  //Print text selection.
{$EXTERNALSYM PRNC_SELECTION}

//Main menu
const MENU_FILE_POSITION = 0;
{$EXTERNALSYM MENU_FILE_POSITION}
const MENU_EDIT_POSITION = 1;
{$EXTERNALSYM MENU_EDIT_POSITION}
const MENU_VIEW_POSITION = 2;
{$EXTERNALSYM MENU_VIEW_POSITION}
const MENU_OPTIONS_POSITION = 3;
{$EXTERNALSYM MENU_OPTIONS_POSITION}
const MENU_MDI_POSITION = 4;
{$EXTERNALSYM MENU_MDI_POSITION}
const MENU_ABOUT_POSITION = 5;
{$EXTERNALSYM MENU_ABOUT_POSITION}

//Popup menu
const MENU_POPUP_EDIT = 0;
{$EXTERNALSYM MENU_POPUP_EDIT}
const MENU_POPUP_VIEW = 1;
{$EXTERNALSYM MENU_POPUP_VIEW}
const MENU_POPUP_CODEPAGE = 2;
{$EXTERNALSYM MENU_POPUP_CODEPAGE}
const MENU_POPUP_HEADLINE = 3;
{$EXTERNALSYM MENU_POPUP_HEADLINE}

//Submenu position
const MENU_FILE_RECENTFILES_3X = 12;
{$EXTERNALSYM MENU_FILE_RECENTFILES_3X}
const MENU_FILE_RECENTFILES_4X = 14;
{$EXTERNALSYM MENU_FILE_RECENTFILES_4X}
const MENU_VIEW_LANGUAGE_3X = 11;
{$EXTERNALSYM MENU_VIEW_LANGUAGE_3X}
const MENU_VIEW_LANGUAGE_4X = 9;
{$EXTERNALSYM MENU_VIEW_LANGUAGE_4X}
const MENU_POPUP_CODEPAGE_OPEN = 0;
{$EXTERNALSYM MENU_POPUP_CODEPAGE_OPEN}
const MENU_POPUP_CODEPAGE_SAVE = 2;
{$EXTERNALSYM MENU_POPUP_CODEPAGE_SAVE}

//INI value types
const INI_DWORD = 1;
{$EXTERNALSYM INI_DWORD}
const INI_BINARY = 2;
{$EXTERNALSYM INI_BINARY}
const INI_STRINGANSI = 3;
{$EXTERNALSYM INI_STRINGANSI}
const INI_STRINGUNICODE = 4;
{$EXTERNALSYM INI_STRINGUNICODE}

//Options flags
const POB_READ = $01;  //Begins read options.
{$EXTERNALSYM POB_READ}
const POB_SAVE = $02;  //Begins save options.
{$EXTERNALSYM POB_SAVE}
const POB_CLEAR = $04;  //Begins new save options (POB_SAVE|POB_CLEAR).
{$EXTERNALSYM POB_CLEAR}
const POB_SCRIPTS = $10;  //Begins Scripts plugin options.
{$EXTERNALSYM POB_SCRIPTS}

//Option type
const PO_DWORD = 1;   //32-bit number.
{$EXTERNALSYM PO_DWORD}
const PO_BINARY = 2;   //Binary data in any form.
{$EXTERNALSYM PO_BINARY}
const PO_STRING = 3;   //Null-terminated cString.
{$EXTERNALSYM PO_STRING}
const PO_REMOVE = 10;  //Delete option.
{$EXTERNALSYM PO_REMOVE}

//Support flags
const PDS_SUPPORTALL = $00000000;  //Function support everything (default).
{$EXTERNALSYM PDS_SUPPORTALL}
const PDS_NOAUTOLOAD = $00000001;  //Function doesn't support autoload.
{$EXTERNALSYM PDS_NOAUTOLOAD}
const PDS_NOANSI = $00000002;  //Function doesn't support ansi API and can be executed only on unicode Windows (WinNT/2000/XP/2003/Vista/Seven).
{$EXTERNALSYM PDS_NOANSI}
const PDS_NOUNICODE = $00000004;  //Function doesn't support unicode API.
{$EXTERNALSYM PDS_NOUNICODE}
const PDS_NOSDI = $00000008;  //Function doesn't support SDI mode.
{$EXTERNALSYM PDS_NOSDI}
const PDS_NOMDI = $00000010;  //Function doesn't support MDI mode.
{$EXTERNALSYM PDS_NOMDI}
const PDS_NOPMDI = $00000020;  //Function doesn't support PMDI mode.
{$EXTERNALSYM PDS_NOPMDI}
const PDS_NORICHEDIT = $00000040;  //Reserved.
{$EXTERNALSYM PDS_NORICHEDIT}
const PDS_GETSUPPORT = $10000000; //Flag is set if caller wants to get PDS_* flags without function execution.
{$EXTERNALSYM PDS_GETSUPPORT}
const PDS_STRANSI = $20000000;  //Flag is set if caller passes Ansi strings in external call arguments (PLUGINDATA.lParam).
{$EXTERNALSYM PDS_STRANSI}
const PDS_STRWIDE = $40000000;  //Flag is set if caller passes Unicode strings in external call arguments (PLUGINDATA.lParam).
{$EXTERNALSYM PDS_STRWIDE}  //If PDS_STRANSI and PDS_STRWIDE not specified then one of these flags will be set automatically depending on Windows version.

//AKD_DLLCALL flags
const DLLCF_ONPROGRAMLOAD = $001;//Don't use it. For internal code only.
{$EXTERNALSYM DLLCF_ONPROGRAMLOAD}
const DLLCF_SWITCHAUTOLOAD = $004; //If function running after call then turn on autoload, if not then turn off autoload.
{$EXTERNALSYM DLLCF_SWITCHAUTOLOAD}
const DLLCF_SAVENOW = $008; //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_NOW after switching autoload flag.
{$EXTERNALSYM DLLCF_SAVENOW}
const DLLCF_SAVEONEXIT = $010; //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_ONEXIT after switching autoload flag.
{$EXTERNALSYM DLLCF_SAVEONEXIT}

//AKD_DLLSAVE flags
const DLLSF_NOW = $1;  //Save plugins stack immediately.
{$EXTERNALSYM DLLSF_NOW}
const DLLSF_ONEXIT = $2;  //Save plugins stack on program exit.
{$EXTERNALSYM DLLSF_ONEXIT}

//AKD_TRANSLATEMESSAGE types
const TMSG_GLOBAL = $01;  //Translate hotkey from global accelerator table (PLUGINDATA.hGlobalAccel).
{$EXTERNALSYM TMSG_GLOBAL}
const TMSG_DIALOG = $02;  //Translate message from modeless (see AKD_SETMODELESS) or dockable dialog (see AKD_DOCK).
{$EXTERNALSYM TMSG_DIALOG}
const TMSG_PLUGIN = $04;  //Translate plugin message (see AKD_DLL *, AKD_CALLPROC, AKD_POSTMESSAGE).
{$EXTERNALSYM TMSG_PLUGIN}
const TMSG_HOTKEY = $08;  //Translate plugin hotkey.
{$EXTERNALSYM TMSG_HOTKEY}
const TMSG_ACCELERATOR = $10;  //Translate hotkey from main accelerator table (PLUGINDATA.hMainAccel).
{$EXTERNALSYM TMSG_ACCELERATOR}
const TMSG_DEFAULT = $20;  //Default message processing.
{$EXTERNALSYM TMSG_DEFAULT}

const TMSG_ALL = (TMSG_GLOBAL or TMSG_DIALOG or TMSG_PLUGIN or TMSG_HOTKEY or TMSG_ACCELERATOR or TMSG_DEFAULT);
{$EXTERNALSYM TMSG_ALL}

//Context menu owner
const NCM_EDIT = 1;  //Edit control.
{$EXTERNALSYM NCM_EDIT}
const NCM_TAB = 2;  //Tab control.
{$EXTERNALSYM NCM_TAB}
const NCM_STATUS = 3;  //Status bar control.
{$EXTERNALSYM NCM_STATUS}

//AKD_FRAMEFIND flags
const FWF_CURRENT = 1;  //Retrieve current frame data pointer. lParam not used.
{$EXTERNALSYM FWF_CURRENT}
const FWF_NEXT = 2;  //Retrieve next frame data pointer in frame stack. lParam is a frame data pointer.
{$EXTERNALSYM FWF_NEXT}
const FWF_PREV = 3;  //Retrieve previous frame data pointer in frame stack. lParam is a frame data pointer.
{$EXTERNALSYM FWF_PREV}
const FWF_BYINDEX = 4;  //Retrieve frame data by index in frame stack. lParam is frame index. First frame has index 1.
{$EXTERNALSYM FWF_BYINDEX}
const FWF_BYFILENAME = 5;     //Retrieve frame data by full cFile name. lParam is full cFile name string.
{$EXTERNALSYM FWF_BYFILENAME} // For AKD_FRAMEFINDA string is ansi.
                              // For AKD_FRAMEFINDW string is unicode.
const FWF_BYEDITWINDOW = 6;  //Retrieve frame data by edit window handle. lParam is edit window handle or NULL for current edit window handle.
{$EXTERNALSYM FWF_BYEDITWINDOW}
const FWF_BYEDITDOCUMENT = 7;  //Retrieve frame data by edit document handle. lParam is edit document handle or NULL for current edit document handle.
{$EXTERNALSYM FWF_BYEDITDOCUMENT}
const FWF_BYTABINDEX = 8;  //Retrieve frame data by tab item index. lParam is tab item index.
{$EXTERNALSYM FWF_BYTABINDEX}
const FWF_TABNEXT = 9;  //Retrieve next tab item frame data. lParam is a frame data pointer.
{$EXTERNALSYM FWF_TABNEXT}
const FWF_TABPREV = 10; //Retrieve previous tab item frame data. lParam is a frame data pointer.
{$EXTERNALSYM FWF_TABPREV}

//AKD_FRAMEACTIVATE and AKDN_FRAME_ACTIVATE flags
const FWA_NOUPDATEORDER = $00000001;  //Don't update access order during activating.
{$EXTERNALSYM FWA_NOUPDATEORDER}
const FWA_NOUPDATEEDIT = $00000002;  //For WMD_PMDI mode. Don't redraw edit control area during activating.
{$EXTERNALSYM FWA_NOUPDATEEDIT}
const FWA_NOVISUPDATE = $00000004;  //For WMD_PMDI mode. Don't make any visual updates during activating. Use it only if you later will activate back frame that has lost active status.
{$EXTERNALSYM FWA_NOVISUPDATE}      //AKDN_FRAME_ACTIVATE only flags:
const FWA_NOTIFY_CREATE = $00000100;  //Frame activating after creation.
{$EXTERNALSYM FWA_NOTIFY_CREATE}
const FWA_NOTIFY_BEFOREDESTROY = $00000200;  //Frame activating for destroying.
{$EXTERNALSYM FWA_NOTIFY_BEFOREDESTROY}
const FWA_NOTIFY_AFTERDESTROY = $00000400;  //Previous frame activating because current one has been destroyed.
{$EXTERNALSYM FWA_NOTIFY_AFTERDESTROY}
const FWA_NOTIFY_REOPEN = $00000800;  //Frame activating before reopening document.
{$EXTERNALSYM FWA_NOTIFY_REOPEN}

//AKD_FRAMEDESTROY return value
const FWDE_SUCCESS = 0;  //Operation is successful.
{$EXTERNALSYM FWDE_SUCCESS}
const FWDE_ABORT = 1;  //Operation is aborted by user.
{$EXTERNALSYM FWDE_ABORT}
const FWDE_LASTTAB = 2;  //For WMD_PMDI mode. Last tab cannot be destroyed.
{$EXTERNALSYM FWDE_LASTTAB}
const FWDE_NOWINDOW = 3;  //Frame doesn't have associated edit window (FRAMEDATA.ei.hWndEdit == NULL).
{$EXTERNALSYM FWDE_NOWINDOW}

//AKD_FRAMESTATS flags
const FWS_COUNTALL = 0;  //Count of windows.
{$EXTERNALSYM FWS_COUNTALL}
const FWS_COUNTMODIFIED = 1;  //Count of modified windows.
{$EXTERNALSYM FWS_COUNTMODIFIED}
const FWS_COUNTSAVED = 2;  //Count of unmodified windows.
{$EXTERNALSYM FWS_COUNTSAVED}
const FWS_CURSEL = 3;  //Active window zero based index.
{$EXTERNALSYM FWS_CURSEL}

//Lock inherit new document settings from current document
const LI_FONT = $00000001;  //Lock inherit font.
{$EXTERNALSYM LI_FONT}
const LI_COLORS = $00000002;  //Lock inherit colors.
{$EXTERNALSYM LI_COLORS}
const LI_BKIMAGE = $00000004;  //Lock inherit background image.
{$EXTERNALSYM LI_BKIMAGE}
const LI_WRAP = $00000008;  //Lock inherit wrapping.
{$EXTERNALSYM LI_WRAP}

//Find/Replace flags
const FRF_DOWN = $00000001;  //Same as AEFR_DOWN.
{$EXTERNALSYM FRF_DOWN}
const FRF_WHOLEWORD = $00000002;  //Same as AEFR_WHOLEWORD.
{$EXTERNALSYM FRF_WHOLEWORD}
const FRF_MATCHCASE = $00000004;  //Same as AEFR_MATCHCASE.
{$EXTERNALSYM FRF_MATCHCASE}
const FRF_REGEXPNONEWLINEDOT = $00040000;  //Symbol . specifies any character except new line. Uses with FRF_REGEXP.
{$EXTERNALSYM FRF_REGEXPNONEWLINEDOT}
const FRF_REGEXP = $00080000;  //Same as AEFR_REGEXP.
{$EXTERNALSYM FRF_REGEXP}
const FRF_UP = $00100000;
{$EXTERNALSYM FRF_UP}
const FRF_BEGINNING = $00200000;
{$EXTERNALSYM FRF_BEGINNING}
const FRF_SELECTION = $00400000;
{$EXTERNALSYM FRF_SELECTION}
const FRF_ESCAPESEQ = $00800000;
{$EXTERNALSYM FRF_ESCAPESEQ}
const FRF_ALLFILES = $01000000;
{$EXTERNALSYM FRF_ALLFILES}
const FRF_REPLACEALLANDCLOSE = $02000000;
{$EXTERNALSYM FRF_REPLACEALLANDCLOSE}
const FRF_CHECKINSELIFSEL = $04000000;
{$EXTERNALSYM FRF_CHECKINSELIFSEL}
const FRF_CYCLESEARCH = $08000000;
{$EXTERNALSYM FRF_CYCLESEARCH}
const FRF_CYCLESEARCHPROMPT = $10000000;
{$EXTERNALSYM FRF_CYCLESEARCHPROMPT}
const FRF_FINDFROMREPLACE = $20000000;
{$EXTERNALSYM FRF_FINDFROMREPLACE}
const FRF_WHOLEWORDGOODSTART = $40000000;
{$EXTERNALSYM FRF_WHOLEWORDGOODSTART}
const FRF_WHOLEWORDGOODEND = $80000000;
{$EXTERNALSYM FRF_WHOLEWORDGOODEND}


//AKD_PASTE
const PASTE_ANSI = $00000001;  //Paste text as ANSI. Default is paste as Unicode text, if no Unicode text available ANSI text will be used.
{$EXTERNALSYM PASTE_ANSI}
const PASTE_COLUMN = $00000002;  //Paste to column selection.
{$EXTERNALSYM PASTE_COLUMN}
const PASTE_AFTER = $00001000;  //Paste text after caret.
{$EXTERNALSYM PASTE_AFTER}
const PASTE_SINGLELINE = $00002000;  //Paste multiline text to single line edit control. All new lines replaced with '\r'.
{$EXTERNALSYM PASTE_SINGLELINE}

//AKD_RECODESEL flags
const RCS_DETECTONLY = $00000001;  //Don't do text replacement, only detect codepages.
{$EXTERNALSYM RCS_DETECTONLY}

//AKD_GETMODELESS types
const MLT_NONE = 0; //No registered modeless dialog open.
{$EXTERNALSYM MLT_NONE}
const MLT_CUSTOM = 1; //Dialog registered with AKD_SETMODELESS.
{$EXTERNALSYM MLT_CUSTOM}
const MLT_RECODE = 2; //Recode dialog.
{$EXTERNALSYM MLT_RECODE}
const MLT_FIND = 3; //Find dialog.
{$EXTERNALSYM MLT_FIND}
const MLT_REPLACE = 4; //Replace dialog.
{$EXTERNALSYM MLT_REPLACE}
const MLT_GOTO = 5; //Go to dialog.
{$EXTERNALSYM MLT_GOTO}

//DIALOGRESIZEMSG flags
const DRM_PAINTSIZEGRIP = $2; //Draw resize grid.
{$EXTERNALSYM DRM_PAINTSIZEGRIP}

//DIALOGRESIZE type
const DRS_SIZE = $1; //Resize control. Can be combined with DRS_X ot DRS_Y.
{$EXTERNALSYM DRS_SIZE}
const DRS_MOVE = $2; //Move control. Can be combined with DRS_X ot DRS_Y.
{$EXTERNALSYM DRS_MOVE}
const DRS_X = $4; //X value. Can be combined with DRS_SIZE ot DRS_MOVE.
{$EXTERNALSYM DRS_X}
const DRS_Y = $8; //Y value. Can be combined with DRS_SIZE ot DRS_MOVE.
{$EXTERNALSYM DRS_Y}

//Dock side
const DKS_LEFT = 1;
{$EXTERNALSYM DKS_LEFT}
const DKS_RIGHT = 2;
{$EXTERNALSYM DKS_RIGHT}
const DKS_TOP = 3;
{$EXTERNALSYM DKS_TOP}
const DKS_BOTTOM = 4;
{$EXTERNALSYM DKS_BOTTOM}

//Dock flags
const DKF_OWNTHREAD = $00000001;  //Dock window has its own thread.
{$EXTERNALSYM DKF_OWNTHREAD}
const DKF_FIXEDSIZE = $00000002;  //Dock window isn't resizeable.
{$EXTERNALSYM DKF_FIXEDSIZE}
const DKF_DRAGDROP = $00000004;  //Dock window has drag-and-drop support, the DOCK.rcDragDrop member is valid.
{$EXTERNALSYM DKF_DRAGDROP}
const DKF_HIDDEN = $00000008;  //Dock window isn't visible.
{$EXTERNALSYM DKF_HIDDEN}
const DKF_NODROPLEFT = $00000010;  //Disable drag-and-drop to the left side.
{$EXTERNALSYM DKF_NODROPLEFT}
const DKF_NODROPRIGHT = $00000020;  //Disable drag-and-drop to the right side.
{$EXTERNALSYM DKF_NODROPRIGHT}
const DKF_NODROPTOP = $00000040;  //Disable drag-and-drop to the top side.
{$EXTERNALSYM DKF_NODROPTOP}
const DKF_NODROPBOTTOM = $00000080;  //Disable drag-and-drop to the bottom side.
{$EXTERNALSYM DKF_NODROPBOTTOM}

//Dock action
const DK_ADD = $00000001;  //Add new dockable window.
{$EXTERNALSYM DK_ADD}
const DK_DELETE = $00000002;  //Delete dockable window.
{$EXTERNALSYM DK_DELETE}
const DK_SUBCLASS = $00000004;  //Assign dockable window handle (DOCK.hWnd).
{$EXTERNALSYM DK_SUBCLASS}
const DK_UNSUBCLASS = $00000008;  //Deassign dockable window handle (DOCK.hWnd).
{$EXTERNALSYM DK_UNSUBCLASS}
const DK_SETLEFT = $00000010;  //Set DKS_LEFT side.
{$EXTERNALSYM DK_SETLEFT}
const DK_SETRIGHT = $00000020;  //Set DKS_RIGHT side.
{$EXTERNALSYM DK_SETRIGHT}
const DK_SETTOP = $00000040;  //Set DKS_TOP side.
{$EXTERNALSYM DK_SETTOP}
const DK_SETBOTTOM = $00000080;  //Set DKS_BOTTOM side.
{$EXTERNALSYM DK_SETBOTTOM}
const DK_HIDE = $00000100;  //Hide dockable window and set DKF_HIDDEN flag.
{$EXTERNALSYM DK_HIDE}
const DK_SHOW = $00000200;  //Show dockable window and remove DKF_HIDDEN flag.
{$EXTERNALSYM DK_SHOW}
const DK_FINDDOCK = $00000400;  //Find dock by dockable window handle (DOCK.hWnd). Cannot be combined with DK_FINDCHILD.
{$EXTERNALSYM DK_FINDDOCK}
const DK_FINDCHILD = $00000800;  //Find dock by dockable window or its child handle (DOCK.hWnd). Cannot be combined with DK_FINDDOCK.
{$EXTERNALSYM DK_FINDCHILD}

//Dock capture
const DKC_SIZING = 1;
{$EXTERNALSYM DKC_SIZING}
const DKC_DRAGDROP = 2;
{$EXTERNALSYM DKC_DRAGDROP}

//AKD_SETBUTTONDRAW flags
const BIF_BITMAP = $001; //Bitmap handle is used in BUTTONDRAW.hImage.
{$EXTERNALSYM BIF_BITMAP}
const BIF_ICON = $002; //Icon handle is used in BUTTONDRAW.hImage.
{$EXTERNALSYM BIF_ICON}
const BIF_CROSS = $004; //Draw small cross 8x7. BUTTONDRAW.hImage is ignored.
{$EXTERNALSYM BIF_CROSS}
const BIF_DOWNARROW = $008; //Draw small down arrow 7x4. BUTTONDRAW.hImage is ignored.
{$EXTERNALSYM BIF_DOWNARROW}
const BIF_UPARROW = $010; //Draw small up arrow 7x4. BUTTONDRAW.hImage is ignored.
{$EXTERNALSYM BIF_UPARROW}
const BIF_LEFTARROW = $020; //Draw small left arrow 4x7. BUTTONDRAW.hImage is ignored.
{$EXTERNALSYM BIF_LEFTARROW}
const BIF_RIGHTARROW = $040; //Draw small right arrow 4x7. BUTTONDRAW.hImage is ignored.
{$EXTERNALSYM BIF_RIGHTARROW}
const BIF_ETCHED = $100; //Draw edge around button.
{$EXTERNALSYM BIF_ETCHED}
const BIF_ENABLEFOCUS = $200; //Draw focus rectangle when button receive focus.
{$EXTERNALSYM BIF_ENABLEFOCUS}

//BUTTONMESSAGEBOX flags
const BMB_DEFAULT = $001;  //Default button.
{$EXTERNALSYM BMB_DEFAULT}
const BMB_DISABLED = $002;  //Button is disabled.
{$EXTERNALSYM BMB_DISABLED}

//WM_INITMENU lParam
const IMENU_EDIT = $00000001;
{$EXTERNALSYM IMENU_EDIT}
const IMENU_CHECKS = $00000004;
{$EXTERNALSYM IMENU_CHECKS}

////
type
  PHWND = ^HWND;
  {$EXTERNALSYM PHWND}
  // Fr0sT: these WinAPI types aren't defined in RTL until D2009 (?)
  {$IF CompilerVersion < 20}
    INT_PTR = Integer;
    {$EXTERNALSYM INT_PTR}
    LONG_PTR = Integer;
    {$EXTERNALSYM LONG_PTR}
    UINT_PTR = Cardinal;
    {$EXTERNALSYM UINT_PTR}
    ULONG_PTR = LongWord;
    {$EXTERNALSYM ULONG_PTR}
    DWORD_PTR = ULONG_PTR;
    {$EXTERNALSYM DWORD_PTR}
    HANDLE_PTR = type LongWord;
    {$EXTERNALSYM HANDLE_PTR}
  {$IFEND}

//// Structures

type
  PHSTACK = ^THSTACK;
  _HSTACK = record
    first: INT_PTR;
    last: INT_PTR;
  end;
  THSTACK = _HSTACK;
  {$EXTERNALSYM _HSTACK}

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
  {$EXTERNALSYM _AELINEDATA}


type
  _AELINEINDEX = record
    nLine: Integer;
    lpLine: PAELINEDATA;
  end;
  TAELINEINDEX = _AELINEINDEX;
  {$EXTERNALSYM _AELINEINDEX}

type
  PAECHARINDEX = ^TAECHARINDEX;
  _AECHARINDEX = record
    nLine: Integer;
    lpLine: PAELINEDATA;
    nCharInLine: Integer;
  end;
  TAECHARINDEX = _AECHARINDEX;
  {$EXTERNALSYM _AECHARINDEX}

  PAECHARRANGE = ^TAECHARRANGE;
  _AECHARRANGE = record
    ciMin : TAECHARINDEX;
    ciMax : TAECHARINDEX;
  end;
  TAECHARRANGE = _AECHARRANGE;
  {$EXTERNALSYM _AECHARRANGE}

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
  {$EXTERNALSYM _AECOLORS}

type
  AEHDOC = THandle;
  {$EXTERNALSYM AEHDOC}

type
  // hDoc        Document handle returned by AEM_GETDOCUMENT or AEM_CREATEDOCUMENT.
  // uMsg        Message ID for example EM_SETSEL.
  // lParam      Additional parameter.
  // wParam      Additional parameter.
  //
  // Return Value
  //   Depends on message.
  TAEEditProc = function (hDoc: AEHDOC; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;
  {$EXTERNALSYM TAEEditProc}
  TWndProc = function (hWnd: HWND; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;
  {$EXTERNALSYM TWndProc}

type
  HINIFILE = THandle;
  {$EXTERNALSYM HINIFILE}
type
  HINISECTION = THandle;
  {$EXTERNALSYM HINISECTION}
type
  HINIKEY = THandle;
  {$EXTERNALSYM HINIKEY}

type
  TPluginProc = function(): BOOL; stdcall;
  {$EXTERNALSYM TPluginProc}
  TWndProcRet = procedure(lpWNDProcRet: PCWPRETSTRUCT); stdcall;
  {$EXTERNALSYM TWndProcRet}

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
  {$EXTERNALSYM _PLUGINVERSION}


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
  {$EXTERNALSYM _PLUGINFUNCTION}


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
  {$EXTERNALSYM _PLUGINDATA}


type
  _UNISTRING = record
    pString: PBYTE;         //Depend on Windows. Win9x or WinNT.
    szString: PAnsiChar;    //Ansi cString.
    wszString: PWideChar;   //Unicode cString.
  end;
  TUNISTRING = _UNISTRING;
  {$EXTERNALSYM _UNISTRING}


type
  _DETECTANSITEXT = record
    dwLangID: DWORD;         //Codepage recognition language defined as LANGID. If -1, then use current settings.
    pText: PAnsiChar;        //Ansi text.
    nTextLen: INT_PTR;       //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nMinChars: INT_PTR;      //Minimum detect characters representation. If zero, default number is used.
    nCodePage: Integer;      //Result: detected Ansi codepage.
  end;
  TDETECTANSITEXT = _DETECTANSITEXT;
  {$EXTERNALSYM _DETECTANSITEXT}


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
  {$EXTERNALSYM _DETECTUNITEXT}


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
  {$EXTERNALSYM _CONVERTANSITEXT}


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
  {$EXTERNALSYM _CONVERTUNITEXT}


type
  _DETECTFILEA = record
    pFile: PAnsiChar;       //cFile to detect.
    dwBytesToCheck: DWORD;  //How many bytes will be checked.
    dwFlags: DWORD;         //See ADT_* defines.
    nCodePage: Integer;     //Detected codepage.
    bBOM: BOOL;             //Detected BOM.
  end;
  TDETECTFILEA = _DETECTFILEA;
  {$EXTERNALSYM _DETECTFILEA}


type
  _DETECTFILEW = record
    pFile: PWideChar;       //cFile to detect.
    dwBytesToCheck: DWORD;  //How many bytes will be checked.
    dwFlags: DWORD;         //See ADT_* defines.
    nCodePage: Integer;     //Detected codepage.
    bBOM: BOOL;             //Detected BOM.
  end;
  TDETECTFILEW = _DETECTFILEW;
  {$EXTERNALSYM _DETECTFILEW}


type
  _FILECONTENT = record
    hFile: THandle;         //cFile handle, returned by CreateFile function.
    dwBytesMax: UINT_PTR;   //Maximum bytes to read, if -1 read entire file.
    nCodePage: Integer;     //File codepage.
    bBOM: BOOL;             //File BOM.
    wpContent: PWideChar;   //Returned file contents.
  end;
  TFILECONTENT = _FILECONTENT;
  {$EXTERNALSYM _FILECONTENT}


type
  _OPENDOCUMENTA = record
    pFile: PAnsiChar;       //cFile to open.
    pWorkDir: PAnsiChar;    //Set working directory before open, if NULL then don't set.
    dwFlags: DWORD;         //Open flags. See OD_* defines.
    nCodePage: Integer;     //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
    bBOM: BOOL;             //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
  end;
  TOPENDOCUMENTA = _OPENDOCUMENTA;
  {$EXTERNALSYM _OPENDOCUMENTA}


type
  _OPENDOCUMENTW = record
    pFile: PWideChar;       //cFile to open.
    pWorkDir: PWideChar;    //Set working directory before open, if NULL then don't set.
    dwFlags: DWORD;         //Open flags. See OD_* defines.
    nCodePage: Integer;     //File code page, ignored if (dwFlags & OD_ADT_DETECT_CODEPAGE).
    bBOM: BOOL;             //File BOM, ignored if (dwFlags & OD_ADT_DETECT_BOM).
  end;
  TOPENDOCUMENTW = _OPENDOCUMENTW;
  {$EXTERNALSYM _OPENDOCUMENTW}


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
  {$EXTERNALSYM _OPENDOCUMENTPOSTA}


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
  {$EXTERNALSYM _OPENDOCUMENTPOSTW}


type
  _SAVEDOCUMENTA = record
    pFile: PAnsiChar;       //cFile to save.
    nCodePage: Integer;     //File code page.
    bBOM: BOOL;             //File BOM.
    dwFlags: DWORD;         //See SD_* defines.
  end;
  TSAVEDOCUMENTA = _SAVEDOCUMENTA;
  {$EXTERNALSYM _SAVEDOCUMENTA}


type
  _SAVEDOCUMENTW = record
    pFile: PWideChar;       //cFile to save.
    nCodePage: Integer;     //File code page.
    bBOM: BOOL;             //File BOM.
    dwFlags: DWORD;         //See SD_* defines.
  end;
  TSAVEDOCUMENTW = _SAVEDOCUMENTW;
  {$EXTERNALSYM _SAVEDOCUMENTW}


type
  PRECENTCARETITEM = ^TRECENTCARETITEM;
  _RECENTCARETITEM = record
    next: PRECENTCARETITEM;
    prev: PRECENTCARETITEM;
    nCaretOffset: INT_PTR;
  end;
  TRECENTCARETITEM = _RECENTCARETITEM;
  {$EXTERNALSYM _RECENTCARETITEM}


type
  _STACKRECENTCARET = record
    first: PRECENTCARETITEM;
    last: PRECENTCARETITEM;
  end;
  TSTACKRECENTCARET = _STACKRECENTCARET;
  {$EXTERNALSYM _STACKRECENTCARET}


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
  {$EXTERNALSYM _EDITINFO}


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
  {$EXTERNALSYM _PLUGINVERSION}


//AKD_SETFRAMEINFO
type
  _FRAMEINFO = record
    nType: Integer;        //See FIS_* defines.
    dwData: UINT_PTR;  //Depend on FIS_* define.
  end;
  TFRAMEINFO = _FRAMEINFO;
  {$EXTERNALSYM _FRAMEINFO}


type
  _BKIMAGE = record
    wpFile: PWideChar; //Background image file.
    nAlpha: Integer;   //Alpha transparency value that ranges from 0 to 255.
  end;
  TBKIMAGE = _BKIMAGE;
  {$EXTERNALSYM _BKIMAGE}


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
  {$EXTERNALSYM _WNDPROCDATA}


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
  {$EXTERNALSYM _WNDPROCRETDATA}


type
  _PLUGINADDA = record
    pFunction: PAnsiChar;      //cFunction name, format "Plugin::cFunction".
    wHotkey: WORD;               //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
    bAutoLoad: BOOL;             //TRUE  if function has autoload flag.
    PluginProc: TPluginProc;      //Function procedure.
    lpParameter: Pointer;          //Procedure parameter.
  end;
  TPLUGINADDA = _PLUGINADDA;
  {$EXTERNALSYM _PLUGINADDA}


type
  _PLUGINADDW = record
    pFunction: PWideChar;   //cFunction name, format "Plugin::cFunction".
    wHotkey: WORD;               //Function hotkey. See HKM_GETHOTKEY message return value (MSDN).
    bAutoLoad: BOOL;             //TRUE  if function has autoload flag.
    PluginProc: TPLUGINPROC;      //Function procedure.
    lpParameter: Pointer;          //Procedure parameter.
  end;
  TPLUGINADDW = _PLUGINADDW;
  {$EXTERNALSYM _PLUGINADDW}


type
  _PLUGINCALLSENDA = record
    pFunction: PAnsiChar;      //cFunction name, format "Plugin::cFunction".
    lParam: LPARAM;              //Input data.
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLSENDA = _PLUGINCALLSENDA;
  {$EXTERNALSYM _PLUGINCALLSENDA}


type
  _PLUGINCALLSENDW = record
    pFunction: PWideChar;   //cFunction name, format L"Plugin::cFunction".
    lParam: LPARAM;              //Input data.
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLSENDW = _PLUGINCALLSENDW;
  {$EXTERNALSYM _PLUGINCALLSENDW}


type
  _PLUGINCALLPOSTA = record
    lParam: LPARAM;              //Input data.
    szFunction : array[0..MAX_PATH-1] of AnsiChar;  //cFunction name;
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLPOSTA = _PLUGINCALLPOSTA;
  {$EXTERNALSYM _PLUGINCALLPOSTA}


type
  _PLUGINCALLPOSTW = record
    lParam: LPARAM;                  //Input data.
    szFunction : array[0..MAX_PATH-1] of  WideChar;   //cFunction name;
    dwSupport: DWORD;            //See PDS_* defines.
  end;
  TPLUGINCALLPOSTW = _PLUGINCALLPOSTW;
  {$EXTERNALSYM _PLUGINCALLPOSTW}


type
  _PLUGINOPTIONA = record
    pOptionName: PAnsiChar;       //Option name.
    dwType: DWORD;                  //Data cType: see PO_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TPLUGINOPTIONA = _PLUGINOPTIONA;
  {$EXTERNALSYM _PLUGINOPTIONA}


type
  _PLUGINOPTIONW = record
    pOptionName: PWideChar;    //Option name.
    dwType: DWORD;                  //Data cType: see PO_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_OPTION returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TPLUGINOPTIONW = _PLUGINOPTIONW;
  {$EXTERNALSYM _PLUGINOPTIONW}


type
  _INIVALUEA = record
    pSection: PAnsiChar;          //Section name.
    pKey: PAnsiChar;              //Key name.
    dwType: DWORD;                  //Data cType: see INI_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TINIVALUEA = _INIVALUEA;
  {$EXTERNALSYM _INIVALUEA}


type
  _INIVALUEW = record
    pSection: PWideChar;       //Section name.
    pKey: PWideChar;           //Key name.
    dwType: DWORD;                  //Data cType: see INI_* defines.
    lpData: PBYTE;                  //Data pointer. If NULL, AKD_INIGETVALUE returns required buffer size in bytes.
    dwData: DWORD;                  //Data size in bytes.
  end;
  TINIVALUEW = _INIVALUEW;
  {$EXTERNALSYM _INIVALUEW}


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
  {$EXTERNALSYM _INIKEY}


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
  {$EXTERNALSYM _INISECTION}


type
  _INIFILE = record
    first: PINISECTION;
    last: PINISECTION;
    bModified: BOOL;
  end;
  TINIFILE = _INIFILE;
  {$EXTERNALSYM _INIFILE}


type
  _GETTEXTRANGE = record
    cpMin: INT_PTR;              //First character in the range. First char of text: 0.
    cpMax: INT_PTR;              //Last character in the range. Last char of text: -1.
    pText: PByte;       //Pointer that receive allocated text, new lines are forced to "\r". Must be deallocated with AKD_FREETEXT message.
  end;
  TGETTEXTRANGE = _GETTEXTRANGE;
  {$EXTERNALSYM _GETTEXTRANGE}


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
  {$EXTERNALSYM _EXGETTEXTRANGE}


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
  {$EXTERNALSYM _RECENTFILE}


type
  _RECENTFILESTACK = record
    first: PRECENTFILE;          //Pointer to the first RECENTFILE structure.
    last: PRECENTFILE;           //Pointer to the last RECENTFILE structure.
    nElements: Integer;              //Items in stack.
    dwSaveTime: DWORD;           //GetTickCount() for the last recent files save operation.
  end;
  TRECENTFILESTACK = _RECENTFILESTACK;
  {$EXTERNALSYM _RECENTFILESTACK}


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
  {$EXTERNALSYM _RECENTFILEPARAM}

type
  _RECENTFILEPARAMSTACK = record
    first: PRECENTFILEPARAM;
    last: PRECENTFILEPARAM;
  end;
  TRECENTFILEPARAMSTACK = _RECENTFILEPARAMSTACK;
  {$EXTERNALSYM _RECENTFILEPARAMSTACK}


type
  _TEXTFINDA = record
    dwFlags: DWORD;            //See FRF_* defines.
    pFindIt: PAnsiChar;      //Find string.
    nFindItLen: Integer;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  end;
  TTEXTFINDA = _TEXTFINDA;
  {$EXTERNALSYM _TEXTFINDA}


type
  _TEXTFINDW = record
    dwFlags: DWORD;            //See FRF_* defines.
    pFindIt: PWideChar;   //Find string.
    nFindItLen: Integer;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  end;
  TTEXTFINDW = _TEXTFINDW;
  {$EXTERNALSYM _TEXTFINDW}


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
  {$EXTERNALSYM _TEXTREPLACEA}


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
  {$EXTERNALSYM _TEXTREPLACEW}


type
  _TEXTRECODE = record
    nCodePageFrom: Integer;        //Source code page.
    nCodePageTo: Integer;          //Target code page.
    dwFlags: DWORD;            //See RCS_* defines.
  end;
  TTEXTRECODE = _TEXTRECODE;
  {$EXTERNALSYM _TEXTRECODE}



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
  {$EXTERNALSYM _CREATEWINDOWA}


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
  {$EXTERNALSYM _CREATEWINDOWW}


type
  PDIALOGRESIZE = ^TDIALOGRESIZE;
  _DIALOGRESIZE = record
    lpWnd: PHWND;              //Control window.
    dwType: DWORD;             //See DRS_* defines.
    nOffset: Integer;          //Control offset, set it to zero.
  end;
  TDIALOGRESIZE = _DIALOGRESIZE;
  {$EXTERNALSYM _DIALOGRESIZE}


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
  {$EXTERNALSYM _DIALOGRESIZEMSG}


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
  {$EXTERNALSYM _DOCK}


type
  _BUTTONDRAW = record
    dwFlags: DWORD;          //See BIF_* defines.
    hImage: THandle;          //Bitmap handle if BIF_BITMAP specified or icon handle if BIF_ICON specified.
    nImageWidth: Integer;        //Image width.
    nImageHeight: Integer;       //Image height.
  end;
  TBUTTONDRAW = _BUTTONDRAW;
  {$EXTERNALSYM _BUTTONDRAW}

  PBUTTONMESSAGEBOX = ^TBUTTONMESSAGEBOX;
  _BUTTONMESSAGEBOX  = record
    nButtonControlID: Integer;     //ID of the button that returned in result
    wpButtonStr: PWideChar;        //Pointer to Unicode text or ID of the string resource in current language module.
    dwFlags: DWORD;                //See BMB_* defines.
  end;
  TBUTTONMESSAGEBOX = _BUTTONMESSAGEBOX;
  {$EXTERNALSYM _BUTTONMESSAGEBOX}


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
  {$EXTERNALSYM _DIALOGMESSAGEBOX}


type
  _POSTMESSAGE = record
    hWnd: HWND;           //Window handle.
    uMsg: UINT;           //Specifies the message to be sent.
    wParam: WPARAM;       //Specifies additional message-specific information.
    lParam: LPARAM;       //Specifies additional message-specific information.
  end;
  TPOSTMESSAGE = _POSTMESSAGE;
  {$EXTERNALSYM _POSTMESSAGE}


type
  _PARSECMDLINESENDW = record
    pCmdLine: PWideChar; //Command line string. On return contain pointer to a unprocessed string.
    pWorkDir: PWideChar; //Command line string.
  end;
  TPARSECMDLINESENDW = _PARSECMDLINESENDW;
  {$EXTERNALSYM _PARSECMDLINESENDW}


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
  {$EXTERNALSYM _PARSECMDLINEPOSTW}


type
  _PRINTINFO = record
    hDevMode: HGLOBAL;
    hDevNames: HGLOBAL;
    hDC: HDC;
    dwPrintFlags: DWORD;
    nFromPage: WORD;
    nToPage: WORD;
    dwPageFlags: DWORD;
    rtMargin: TRect;
  end;
  TPRINTINFO = _PRINTINFO;
  {$EXTERNALSYM _PRINTINFO}


type
  _CHARCOLOR = record
    nCharPos: INT_PTR;       //Char position.
    crText: COLORREF;        //Text color.
    crBk: COLORREF;          //Background color.
  end;
  TCHARCOLOR = _CHARCOLOR;
  {$EXTERNALSYM _CHARCOLOR}


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
  {$EXTERNALSYM _NOPENDOCUMENT}


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
  {$EXTERNALSYM _NSAVEDOCUMENT}


type
  _NMAINSHOW = record
    dwStyle: PDWORD;         //Pointer to a maximized state variable (WS_MAXIMIZE or zero).
    dwShow: PDWORD;          //Pointer to a SW_ constants combination variable.
    bProcess: BOOL;          //TRUE   show main window.
  end;
  TNMAINSHOW = _NMAINSHOW;
  {$EXTERNALSYM _NMAINSHOW}


type
  _NCONTEXTMENU = record
    hWnd: HWND;              //Context menu window.
    uType: UINT;             //cType:    NCM_EDIT, NCM_TAB or NCM_STATUS.
    pt: TPoint;               //Context menu coordiates.
    bMouse: BOOL;            //Context menu is requested with mouse.
    bProcess: BOOL;          //TRUE   show context menu.
  end;
  TNCONTEXTMENU = _NCONTEXTMENU;
  {$EXTERNALSYM _NCONTEXTMENU}


type
  _NSIZE = record
    rcInitial: TRect;         //Initial client TRect (read-only).
    rcCurrent: TRect;         //Current client TRect (writeable).
  end;
  TNSIZE = _NSIZE;
  {$EXTERNALSYM _NSIZE}



//// AkelPad menu messages

const IDM_FILE_NEW = 4101;  //Create new edit window.
{$EXTERNALSYM IDM_FILE_NEW}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_CREATENEW = 4102;  //Create new instance of program.
{$EXTERNALSYM IDM_FILE_CREATENEW}
                                              //Return Value: new main window handle.
                                              //
const IDM_FILE_OPEN = 4103;  //Open cFile dialog.
{$EXTERNALSYM IDM_FILE_OPEN}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_REOPEN = 4104;  //Reopen cFile.
{$EXTERNALSYM IDM_FILE_REOPEN}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_SAVE = 4105;  //Save cFile.
{$EXTERNALSYM IDM_FILE_SAVE}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_SAVEAS = 4106;  //Save cFile dialog.
{$EXTERNALSYM IDM_FILE_SAVEAS}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_PAGESETUP = 4107;  //Print setup dialog.
{$EXTERNALSYM IDM_FILE_PAGESETUP}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_PRINT = 4108;  //Print dialog. lParam can be used to pass edit window handle.
{$EXTERNALSYM IDM_FILE_PRINT}
                                              //Return Value: number of printed pages.
                                              //
const IDM_FILE_EXIT = 4109;  //Exit program.
{$EXTERNALSYM IDM_FILE_EXIT}
                                              //Return Value: zero.
                                              //
const IDM_FILE_SAVEALL = 4110;  //Save all documents.
{$EXTERNALSYM IDM_FILE_SAVEALL}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_FILE_SAVEALLAS = 4111;  //Save all as dialog.
{$EXTERNALSYM IDM_FILE_SAVEALLAS}
                                              //Return Value: TRUE - "OK" pressed, FALSE - "Cancel" pressed.
                                              //
const IDM_FILE_SILENTPRINT = 4113;  //Print without dialog. lParam can be used to pass edit window handle.
{$EXTERNALSYM IDM_FILE_SILENTPRINT}
                                              //Return Value: number of printed pages.
                                              //
const IDM_FILE_PRINTPREVIEW = 4114;  //Print preview dialog. lParam can be used to pass edit window handle.
{$EXTERNALSYM IDM_FILE_PRINTPREVIEW}
                                              //Return Value: zero.
                                              //
const IDM_FILE_REDETECT = 4121;  //Redetect code page of the current cFile.
{$EXTERNALSYM IDM_FILE_REDETECT}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_ANSI = 4122;  //Reopen cFile as ANSI.
{$EXTERNALSYM IDM_FILE_REOPENAS_ANSI}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_OEM = 4123;  //Reopen cFile as OEM.
{$EXTERNALSYM IDM_FILE_REOPENAS_OEM}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_KOIR = 4124;  //Reopen cFile as KOI-R.
{$EXTERNALSYM IDM_FILE_REOPENAS_KOIR}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_UTF16LE = 4125;  //Reopen cFile as UTF16LE.
{$EXTERNALSYM IDM_FILE_REOPENAS_UTF16LE}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_UTF16BE = 4126;  //Reopen cFile as UTF16BE.
{$EXTERNALSYM IDM_FILE_REOPENAS_UTF16BE}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_REOPENAS_UTF8 = 4127;  //Reopen cFile as UTF8.
{$EXTERNALSYM IDM_FILE_REOPENAS_UTF8}
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_FILE_SAVEAS_ANSI = 4131;  //Save cFile as ANSI.
{$EXTERNALSYM IDM_FILE_SAVEAS_ANSI}
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_OEM = 4132;  //Save cFile as OEM.
{$EXTERNALSYM IDM_FILE_SAVEAS_OEM}
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_KOIR = 4133;  //Save cFile as KOI-R.
{$EXTERNALSYM IDM_FILE_SAVEAS_KOIR}
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF16LE = 4134;  //Save cFile as UTF16LE.
{$EXTERNALSYM IDM_FILE_SAVEAS_UTF16LE}
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF16BE = 4135;  //Save cFile as UTF16BE.
{$EXTERNALSYM IDM_FILE_SAVEAS_UTF16BE}
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF8 = 4136;  //Save cFile as UTF8.
{$EXTERNALSYM IDM_FILE_SAVEAS_UTF8}
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_SAVEAS_UTF8_NOBOM = 4137;  //Save cFile as UTF8 without BOM.
{$EXTERNALSYM IDM_FILE_SAVEAS_UTF8_NOBOM}
                                              //Return Value: see ESD_* defines.
                                              //
const IDM_FILE_CODEPAGEMENU = 4140;  //Show codepages context menu.
{$EXTERNALSYM IDM_FILE_CODEPAGEMENU}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_UNDO = 4151;  //Undo last operation.
{$EXTERNALSYM IDM_EDIT_UNDO}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_REDO = 4152;  //Redo last operation.
{$EXTERNALSYM IDM_EDIT_REDO}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_CUT = 4153;  //Cut.
{$EXTERNALSYM IDM_EDIT_CUT}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_COPY = 4154;  //Copy.
{$EXTERNALSYM IDM_EDIT_COPY}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_PASTE = 4155;  //Paste.
{$EXTERNALSYM IDM_EDIT_PASTE}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_CLEAR = 4156;  //Delete.
{$EXTERNALSYM IDM_EDIT_CLEAR}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_SELECTALL = 4157;  //Select all.
{$EXTERNALSYM IDM_EDIT_SELECTALL}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_FIND = 4158;  //Find dialog.
{$EXTERNALSYM IDM_EDIT_FIND}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_FINDNEXTDOWN = 4159;  //Find last string down.
{$EXTERNALSYM IDM_EDIT_FINDNEXTDOWN}
                                              //Return Value: Character position of the next match. If there are no more matches, the return value is –1.
                                              //
const IDM_EDIT_FINDNEXTUP = 4160;  //Find last string up.
{$EXTERNALSYM IDM_EDIT_FINDNEXTUP}
                                              //Return Value: Character position of the next match. If there are no more matches, the return value is –1.
                                              //
const IDM_EDIT_REPLACE = 4161;  //Replace dialog.
{$EXTERNALSYM IDM_EDIT_REPLACE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_GOTO = 4162;  //Go to dialog.
{$EXTERNALSYM IDM_EDIT_GOTO}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERTCHAR = 4163;  //Run charmap.exe.
{$EXTERNALSYM IDM_EDIT_INSERTCHAR}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERT_TAB = 4164;  //Insert tabulation.
{$EXTERNALSYM IDM_EDIT_INSERT_TAB}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERT_TAB_MENU = 4165;  //Insert tabulation only if several lines selected.
{$EXTERNALSYM IDM_EDIT_INSERT_TAB_MENU}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_DELETE_TAB = 4166;  //Delete tabulation.
{$EXTERNALSYM IDM_EDIT_DELETE_TAB}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_DELETE_TAB_MENU = 4167;  //Delete tabulation only if several lines selected.
{$EXTERNALSYM IDM_EDIT_DELETE_TAB_MENU}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERT_SPACE = 4168;  //Insert space.
{$EXTERNALSYM IDM_EDIT_INSERT_SPACE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERT_SPACE_MENU = 4169;  //Insert space only if several lines selected.
{$EXTERNALSYM IDM_EDIT_INSERT_SPACE_MENU}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_DELETE_SPACE = 4170;  //Delete space.
{$EXTERNALSYM IDM_EDIT_DELETE_SPACE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_DELETE_SPACE_MENU = 4171;  //Delete space only if several lines selected.
{$EXTERNALSYM IDM_EDIT_DELETE_SPACE_MENU}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_DELETE_FIRST_CHAR = 4172;  //Delete first char.
{$EXTERNALSYM IDM_EDIT_DELETE_FIRST_CHAR}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_DELETE_FIRST_CHAR_MENU = 4173;  //Delete first char only if selection not empty.
{$EXTERNALSYM IDM_EDIT_DELETE_FIRST_CHAR_MENU}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_DELETE_TRAILING_WHITESPACES = 4174;  //Delete trailing whitespaces.
{$EXTERNALSYM IDM_EDIT_DELETE_TRAILING_WHITESPACES}
                                                   //Return Value: zero.
                                                   //
const IDM_EDIT_UPPERCASE = 4175;  //Convert text to UPPERCASE.
{$EXTERNALSYM IDM_EDIT_UPPERCASE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_LOWERCASE = 4176;  //Convert text to lowercase.
{$EXTERNALSYM IDM_EDIT_LOWERCASE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_SENTENCECASE = 4177;  //Convert text to Sentence .
{$EXTERNALSYM IDM_EDIT_SENTENCECASE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_TITLECASE = 4178;  //Convert text to Title .
{$EXTERNALSYM IDM_EDIT_TITLECASE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INVERTCASE = 4179;  //Convert text to iNVERT .
{$EXTERNALSYM IDM_EDIT_INVERTCASE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_LOOPCASE = 4180;  //Cycle .
{$EXTERNALSYM IDM_EDIT_LOOPCASE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_RECODE = 4182;  //Recode dialog.
{$EXTERNALSYM IDM_EDIT_RECODE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERTDATE = 4183;  //Insert date.
{$EXTERNALSYM IDM_EDIT_INSERTDATE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_NEWLINE_WIN = 4184;  //Windows new line format.
{$EXTERNALSYM IDM_EDIT_NEWLINE_WIN}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_NEWLINE_UNIX = 4185;  //Unix new line format.
{$EXTERNALSYM IDM_EDIT_NEWLINE_UNIX}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_NEWLINE_MAC = 4186;  //Mac new line format.
{$EXTERNALSYM IDM_EDIT_NEWLINE_MAC}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_INSERTMODE = 4190;  //Insert mode (on\off).
{$EXTERNALSYM IDM_EDIT_INSERTMODE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_PASTEANSI = 4191;  //Paste as ANSI text.
{$EXTERNALSYM IDM_EDIT_PASTEANSI}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_PASTECOLUMN = 4192;  //Paste to column selection.
{$EXTERNALSYM IDM_EDIT_PASTECOLUMN}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_PASTEAFTER = 4193;  //Paste text after caret.
{$EXTERNALSYM IDM_EDIT_PASTEAFTER}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_EDIT_AUTOINDENT = 4196;  //Insert new line with keeping left spaces.
{$EXTERNALSYM IDM_EDIT_AUTOINDENT}
                                              //Return Value: TRUE - inserted with spaces, FALSE - inserted without spaces.
                                              //
const IDM_EDIT_DELLINE = 4197;  //Delete current line.
{$EXTERNALSYM IDM_EDIT_DELLINE}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_SELJUMPCARET = 4198;  //Move caret on the contrary side of selection.
{$EXTERNALSYM IDM_EDIT_SELJUMPCARET}
                                              //Return Value: TRUE - jump to selection beginning, FALSE - jump to selection ending.
                                              //
const IDM_EDIT_RECENTCARETPREV = 4199;  //Move caret to the previous position.
{$EXTERNALSYM IDM_EDIT_RECENTCARETPREV}
                                              //Return Value: zero.
                                              //
const IDM_EDIT_RECENTCARETNEXT = 4200;  //Move caret to the next position.
{$EXTERNALSYM IDM_EDIT_RECENTCARETNEXT}
                                              //Return Value: zero.
                                              //

const IDM_VIEW_FONT = 4201;  //Font dialog.
{$EXTERNALSYM IDM_VIEW_FONT}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_VIEW_COLORS = 4202;  //Color theme dialog.
{$EXTERNALSYM IDM_VIEW_COLORS}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_VIEW_FONTSIZE_INCREASE = 4204;  //Increase font 1px.
{$EXTERNALSYM IDM_VIEW_FONTSIZE_INCREASE}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_FONTSIZE_DECREASE = 4205;  //Decrease font 1px.
{$EXTERNALSYM IDM_VIEW_FONTSIZE_DECREASE}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_FONTSIZE_RESTORE = 4206;  //Decrease font 1px.
{$EXTERNALSYM IDM_VIEW_FONTSIZE_RESTORE}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_WORDWRAP = 4209;  //Word wrap (on\off).
{$EXTERNALSYM IDM_VIEW_WORDWRAP}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_ONTOP = 4210;  //Always on top (on\off).
{$EXTERNALSYM IDM_VIEW_ONTOP}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SHOW_STATUSBAR = 4211;  //Show statusbar (on\off).
{$EXTERNALSYM IDM_VIEW_SHOW_STATUSBAR}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_ALL = 4212;  //Split window into four panes. Turn off, if window already split into four panes.
{$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_ALL}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_WE = 4213;  //Split window into two vertical panes. Turn off, if window already split into two vertical panes.
{$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_WE}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_NS = 4214;  //Split window into two horizontal panes. Turn off, if window already split into two horizontal panes.
{$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_NS}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_SPLIT_WINDOW_ONOFF = 4215;  //Split window into four panes. Turn off, if window already split.
{$EXTERNALSYM IDM_VIEW_SPLIT_WINDOW_ONOFF}
                                              //Return Value: zero.
                                              //
const IDM_VIEW_READONLY = 4216;  //Read only (on\off).
{$EXTERNALSYM IDM_VIEW_READONLY}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_EXEC = 4251;  //Execute command.
{$EXTERNALSYM IDM_OPTIONS_EXEC}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_OPTIONS_SAVETIME = 4252;  //Save file time (on\off).
{$EXTERNALSYM IDM_OPTIONS_SAVETIME}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_WATCHFILE = 4253;  //Watch cFile change (on\off).
{$EXTERNALSYM IDM_OPTIONS_WATCHFILE}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_KEEPSPACE = 4254;  //Keep space (on\off).
{$EXTERNALSYM IDM_OPTIONS_KEEPSPACE}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_SINGLEOPEN_FILE = 4255;  //Single open cFile (on\off).
{$EXTERNALSYM IDM_OPTIONS_SINGLEOPEN_FILE}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_SINGLEOPEN_PROGRAM = 4256;  //Single open cProgram (on\off).
{$EXTERNALSYM IDM_OPTIONS_SINGLEOPEN_PROGRAM}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_PLUGINS = 4259;  //Plugins dialog.
{$EXTERNALSYM IDM_OPTIONS_PLUGINS}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS = 4260;  //Options dialog.
{$EXTERNALSYM IDM_OPTIONS}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_SDI = 4261;  //SDI mode.
{$EXTERNALSYM IDM_OPTIONS_SDI}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_MDI = 4262;  //MDI mode.
{$EXTERNALSYM IDM_OPTIONS_MDI}
                                              //Return Value: zero.
                                              //
const IDM_OPTIONS_PMDI = 4263;  //Pseudo MDI mode.
{$EXTERNALSYM IDM_OPTIONS_PMDI}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABVIEW_TOP = 4301;  //MDI tab window at the top of the main window.
{$EXTERNALSYM IDM_WINDOW_TABVIEW_TOP}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABVIEW_BOTTOM = 4302;  //MDI tab window at the bottom of the main window.
{$EXTERNALSYM IDM_WINDOW_TABVIEW_BOTTOM}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABVIEW_NONE = 4303;  //Hide MDI tab.
{$EXTERNALSYM IDM_WINDOW_TABVIEW_NONE}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABTYPE_STANDARD = 4304;  //Standard MDI tab style.
{$EXTERNALSYM IDM_WINDOW_TABTYPE_STANDARD}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABTYPE_BUTTONS = 4305;  //Buttons MDI tab style.
{$EXTERNALSYM IDM_WINDOW_TABTYPE_BUTTONS}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABTYPE_FLATBUTTONS = 4306;  //Flat buttons MDI tab style.
{$EXTERNALSYM IDM_WINDOW_TABTYPE_FLATBUTTONS}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TILEHORIZONTAL = 4307;  //MDI windows - horizontal tile.
{$EXTERNALSYM IDM_WINDOW_TILEHORIZONTAL}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TILEVERTICAL = 4308;  //MDI windows - vertical tile.
{$EXTERNALSYM IDM_WINDOW_TILEVERTICAL}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_CASCADE = 4309;  //MDI windows - cascade.
{$EXTERNALSYM IDM_WINDOW_CASCADE}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABSWITCH_NEXTPREV = 4310;  //Switch MDI windows: next-previous.
{$EXTERNALSYM IDM_WINDOW_TABSWITCH_NEXTPREV}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_TABSWITCH_RIGHTLEFT = 4311;  //Switch MDI windows: right-left.
{$EXTERNALSYM IDM_WINDOW_TABSWITCH_RIGHTLEFT}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_FRAMENEXT = 4316;  //Activate next MDI window.
{$EXTERNALSYM IDM_WINDOW_FRAMENEXT}
                                              //Return Value: pointer to a FRAMEDATA structure that has been activated.
                                              //
const IDM_WINDOW_FRAMEPREV = 4317;  //Activate previous MDI window.
{$EXTERNALSYM IDM_WINDOW_FRAMEPREV}
                                              //Return Value: pointer to a FRAMEDATA structure that has been activated.
                                              //
const IDM_WINDOW_FRAMECLOSE = 4318;  //Close current MDI window.
{$EXTERNALSYM IDM_WINDOW_FRAMECLOSE}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_FRAMECLOSEALL = 4319;  //Close all documents.
{$EXTERNALSYM IDM_WINDOW_FRAMECLOSEALL}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_FRAMECLOSEALL_BUTACTIVE = 4320;  //Close all documents, but active.
{$EXTERNALSYM IDM_WINDOW_FRAMECLOSEALL_BUTACTIVE}
                                                  //Return Value: TRUE - success, FALSE - failed.
                                                  //
const IDM_WINDOW_FRAMECLOSEALL_UNMODIFIED = 4321;  //Close all unmodified documents.
{$EXTERNALSYM IDM_WINDOW_FRAMECLOSEALL_UNMODIFIED}
                                                  //Return Value: TRUE - success, FALSE - failed.
                                                  //
const IDM_WINDOW_FILECLOSE = 4324;  //Close cFile.
{$EXTERNALSYM IDM_WINDOW_FILECLOSE}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_FILEEXIT = 4325;  //Close cFile and exit program (SDI) or close tab of a cFile (MDI or PMDI).
{$EXTERNALSYM IDM_WINDOW_FILEEXIT}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
const IDM_WINDOW_MDILIST = 4327;  //Select window dialog (MDI). Same as IDM_SELECTWINDOW.
{$EXTERNALSYM IDM_WINDOW_MDILIST}
                                              //Return Value: zero.
                                              //
const IDM_WINDOW_CHANGESIZE = 4331;  //Change style of the main window SW_RESTORE\SW_MAXIMIZE.
{$EXTERNALSYM IDM_WINDOW_CHANGESIZE}
                                              //Return Value: SW_RESTORE - new style is SW_RESTORE, SW_MAXIMIZE - new style is SW_MAXIMIZE.
                                              //
const IDM_WINDOW_DLGNEXT = 4332;  //Activate next dialog window.
{$EXTERNALSYM IDM_WINDOW_DLGNEXT}
                                              //Return Value: activated dialog handle.
                                              //
const IDM_WINDOW_DLGPREV = 4333;  //Activate previous dialog window.
{$EXTERNALSYM IDM_WINDOW_DLGPREV}
                                              //Return Value: activated dialog handle.
                                              //
const IDM_WINDOW_CLONENEXT = 4341;  //Activate next pane (split window).
{$EXTERNALSYM IDM_WINDOW_CLONENEXT}
                                              //Return Value: activated pane handle.
                                              //
const IDM_WINDOW_CLONEPREV = 4342;  //Activate previous pane (split window).
{$EXTERNALSYM IDM_WINDOW_CLONEPREV}
                                              //Return Value: activated pane handle.
                                              //
const IDM_ABOUT = 4351;  //About dialog.
{$EXTERNALSYM IDM_ABOUT}
                                              //Return Value: zero.
                                              //
const IDM_MANUAL = 4352;  //Open user's manual.
{$EXTERNALSYM IDM_MANUAL}
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //

const IDM_UPDATE = 4353;  //Open AkelUpdater
{$EXTERNALSYM IDM_UPDATE}  //Return Value: zero.
                           //

const IDM_INTERNAL_REOPEN_MSG = 4601;  //Internal command.
{$EXTERNALSYM IDM_INTERNAL_REOPEN_MSG}
                                              //Return Value: zero.
                                              //
const IDM_INTERNAL_CANTOPEN_MSG = 4602;  //Internal command.
{$EXTERNALSYM IDM_INTERNAL_CANTOPEN_MSG}
                                              //Return Value: zero.
                                              //
const IDM_INTERNAL_ERRORIO_MSG = 4603;  //Internal command.
{$EXTERNALSYM IDM_INTERNAL_ERRORIO_MSG}
                                              //Return Value: zero.
                                              //
const IDM_INTERNAL_UPDATEMAINCHILDREN = 4604;  //Internal command.
{$EXTERNALSYM IDM_INTERNAL_UPDATEMAINCHILDREN}
                                              //Return Value: zero.
                                              //
const IDM_RECENT_FILES = 5001;  //Delete dead recent files.
{$EXTERNALSYM IDM_RECENT_FILES}
                                              //5001 + n   Open recent file n.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_LANGUAGE = 6001;  //Activate internal language.
{$EXTERNALSYM IDM_LANGUAGE}
                                              //6001 + n   Activate language n.
                                              //Return Value: zero.
                                              //
const IDM_POPUP_OPENAS = 7001;  //Reopen file in first codepage of codepage list.
{$EXTERNALSYM IDM_POPUP_OPENAS}
                                              //7001 + n   Reopen file in codepage n.
                                              //Return Value: see EOD_* defines.
                                              //
const IDM_POPUP_SAVEAS = 8001;  //Save file in first codepage of codepage list.
{$EXTERNALSYM IDM_POPUP_SAVEAS}
                                              //8001 + n   Save file in codepage n.
                                              //Return Value: see ESD_* defines.
const IDM_SELECTWINDOW = 10019; //Select window dialog (MDI). Same as IDM_WINDOW_MDILIST.
{$EXTERNALSYM IDM_SELECTWINDOW}
                                              //Return Value: zero.
                                              //
                                              //
                                              //Example of usage:
                                              //SendMessage(pd->hMainWnd, WM_COMMAND, IDM_FILE_NEW, 0);

//// AkelPad main window WM_USER messages

//Notification messages
const AKDN_MAIN_ONSTART = (WM_USER + 1);   //0x401
{$EXTERNALSYM AKDN_MAIN_ONSTART}
const AKDN_MAIN_ONSTART_PRESHOW = (WM_USER + 2);   //0x402
{$EXTERNALSYM AKDN_MAIN_ONSTART_PRESHOW}
const AKDN_MAIN_ONSTART_SHOW = (WM_USER + 3);   //0x403
{$EXTERNALSYM AKDN_MAIN_ONSTART_SHOW}
const AKDN_MAIN_ONSTART_FINISH = (WM_USER + 4);   //0x404
{$EXTERNALSYM AKDN_MAIN_ONSTART_FINISH}
const AKDN_MAIN_ONSTART_IDLE = (WM_USER + 5);   //0x405
{$EXTERNALSYM AKDN_MAIN_ONSTART_IDLE}
const AKDN_MAIN_ONFINISH = (WM_USER + 6);   //0x406
{$EXTERNALSYM AKDN_MAIN_ONFINISH}
const AKDN_MAIN_ONDESTROY = (WM_USER + 7);   //0x407
{$EXTERNALSYM AKDN_MAIN_ONDESTROY}

const AKDN_EDIT_ONSTART = (WM_USER + 11);  //0x40B
{$EXTERNALSYM AKDN_EDIT_ONSTART}
const AKDN_EDIT_ONFINISH = (WM_USER + 12);  //0x40C
{$EXTERNALSYM AKDN_EDIT_ONFINISH}
const AKDN_EDIT_ONCLOSE = (WM_USER + 13);  //0x40D
{$EXTERNALSYM AKDN_EDIT_ONCLOSE}

const AKDN_FRAME_NOWINDOWS = (WM_USER + 21);  //0x415
{$EXTERNALSYM AKDN_FRAME_NOWINDOWS}
const AKDN_FRAME_ACTIVATE = (WM_USER + 22);  //0x416
{$EXTERNALSYM AKDN_FRAME_ACTIVATE}
const AKDN_FRAME_DEACTIVATE = (WM_USER + 23);  //0x417
{$EXTERNALSYM AKDN_FRAME_DEACTIVATE}
const AKDN_FRAME_DESTROY = (WM_USER + 24);  //0x418
{$EXTERNALSYM AKDN_FRAME_DESTROY}

const AKDN_DOCK_GETMINMAXINFO = (WM_USER + 31);  //0x41F
{$EXTERNALSYM AKDN_DOCK_GETMINMAXINFO}
const AKDN_DOCK_CAPTURE_ONSTART = (WM_USER + 32);  //0x420
{$EXTERNALSYM AKDN_DOCK_CAPTURE_ONSTART}
const AKDN_DOCK_CAPTURE_ONFINISH = (WM_USER + 33);  //0x421
{$EXTERNALSYM AKDN_DOCK_CAPTURE_ONFINISH}
const AKDN_DOCK_RESIZE = (WM_USER + 34);  //0x422
{$EXTERNALSYM AKDN_DOCK_RESIZE}

const AKDN_DLLCALL = (WM_USER + 41);  //0x429
{$EXTERNALSYM AKDN_DLLCALL}
const AKDN_DLLUNLOAD = (WM_USER + 42);  //0x42A
{$EXTERNALSYM AKDN_DLLUNLOAD}

const AKDN_ACTIVATE = (WM_USER + 50);  //0x432
{$EXTERNALSYM AKDN_ACTIVATE}
const AKDN_SIZE_ONSTART = (WM_USER + 51);  //0x433
{$EXTERNALSYM AKDN_SIZE_ONSTART}
const AKDN_SIZE_ONFINISH = (WM_USER + 52);  //0x434
{$EXTERNALSYM AKDN_SIZE_ONFINISH}
const AKDN_OPENDOCUMENT_START = (WM_USER + 53);  //0x435
{$EXTERNALSYM AKDN_OPENDOCUMENT_START}
const AKDN_OPENDOCUMENT_FINISH = (WM_USER + 54);  //0x436
{$EXTERNALSYM AKDN_OPENDOCUMENT_FINISH}
const AKDN_SAVEDOCUMENT_START = (WM_USER + 55);  //0x437
{$EXTERNALSYM AKDN_SAVEDOCUMENT_START}
const AKDN_SAVEDOCUMENT_FINISH = (WM_USER + 56);  //0x438
{$EXTERNALSYM AKDN_SAVEDOCUMENT_FINISH}
const AKDN_HOTKEY = (WM_USER + 57);  //0x439
{$EXTERNALSYM AKDN_HOTKEY}
const AKDN_CONTEXTMENU = (WM_USER + 58);  //0x43A
{$EXTERNALSYM AKDN_CONTEXTMENU}
const AKDN_SEARCH_ENDED = (WM_USER + 59);  //0x43B
{$EXTERNALSYM AKDN_SEARCH_ENDED}
const AKDN_MESSAGEBOXBEGIN = (WM_USER + 61);  //0x43D
{$EXTERNALSYM AKDN_MESSAGEBOXBEGIN}
const AKDN_MESSAGEBOXEND = (WM_USER + 62);  //0x43E
{$EXTERNALSYM AKDN_MESSAGEBOXEND}

//SubClass
const AKD_GETMAINPROC = (WM_USER + 101);
{$EXTERNALSYM AKD_GETMAINPROC}
const AKD_SETMAINPROC = (WM_USER + 102);
{$EXTERNALSYM AKD_SETMAINPROC}
const AKD_GETMAINPROCRET = (WM_USER + 103);
{$EXTERNALSYM AKD_GETMAINPROCRET}
const AKD_SETMAINPROCRET = (WM_USER + 104);
{$EXTERNALSYM AKD_SETMAINPROCRET}
const AKD_GETEDITPROC = (WM_USER + 105);
{$EXTERNALSYM AKD_GETEDITPROC}
const AKD_SETEDITPROC = (WM_USER + 106);
{$EXTERNALSYM AKD_SETEDITPROC}
const AKD_GETEDITPROCRET = (WM_USER + 107);
{$EXTERNALSYM AKD_GETEDITPROCRET}
const AKD_SETEDITPROCRET = (WM_USER + 108);
{$EXTERNALSYM AKD_SETEDITPROCRET}
const AKD_GETFRAMEPROC = (WM_USER + 109);
{$EXTERNALSYM AKD_GETFRAMEPROC}
const AKD_SETFRAMEPROC = (WM_USER + 110);
{$EXTERNALSYM AKD_SETFRAMEPROC}
const AKD_GETFRAMEPROCRET = (WM_USER + 111);
{$EXTERNALSYM AKD_GETFRAMEPROCRET}
const AKD_SETFRAMEPROCRET = (WM_USER + 112);
{$EXTERNALSYM AKD_SETFRAMEPROCRET}

//Command line
const AKD_GETCMDLINEOPTIONS = (WM_USER + 121);
{$EXTERNALSYM AKD_GETCMDLINEOPTIONS}
const AKD_SETCMDLINEOPTIONS = (WM_USER + 122);
{$EXTERNALSYM AKD_SETCMDLINEOPTIONS}
const AKD_PARSECMDLINEW = (WM_USER + 125);
{$EXTERNALSYM AKD_PARSECMDLINEW}

//Text retrieval and modification
const AKD_DETECTANSITEXT = (WM_USER + 146);
{$EXTERNALSYM AKD_DETECTANSITEXT}
const AKD_DETECTUNITEXT = (WM_USER + 147);
{$EXTERNALSYM AKD_DETECTUNITEXT}
const AKD_CONVERTANSITEXT = (WM_USER + 148);
{$EXTERNALSYM AKD_CONVERTANSITEXT}
const AKD_CONVERTUNITEXT = (WM_USER + 149);
{$EXTERNALSYM AKD_CONVERTUNITEXT}
const AKD_DETECTFILE = (WM_USER + 151);
{$EXTERNALSYM AKD_DETECTFILE}
const AKD_DETECTFILEA = (WM_USER + 152);
{$EXTERNALSYM AKD_DETECTFILEA}
const AKD_DETECTFILEW = (WM_USER + 153);
{$EXTERNALSYM AKD_DETECTFILEW}
const AKD_READFILECONTENT = (WM_USER + 154);
{$EXTERNALSYM AKD_READFILECONTENT}
const AKD_OPENDOCUMENT = (WM_USER + 155);
{$EXTERNALSYM AKD_OPENDOCUMENT}
const AKD_OPENDOCUMENTA = (WM_USER + 156);
{$EXTERNALSYM AKD_OPENDOCUMENTA}
const AKD_OPENDOCUMENTW = (WM_USER + 157);
{$EXTERNALSYM AKD_OPENDOCUMENTW}
const AKD_SAVEDOCUMENT = (WM_USER + 158);
{$EXTERNALSYM AKD_SAVEDOCUMENT}
const AKD_SAVEDOCUMENTA = (WM_USER + 159);
{$EXTERNALSYM AKD_SAVEDOCUMENTA}
const AKD_SAVEDOCUMENTW = (WM_USER + 160);
{$EXTERNALSYM AKD_SAVEDOCUMENTW}
const AKD_GETTEXTLENGTH = (WM_USER + 161);
{$EXTERNALSYM AKD_GETTEXTLENGTH}
const AKD_GETTEXTRANGE = (WM_USER + 162);
{$EXTERNALSYM AKD_GETTEXTRANGE}
const AKD_GETTEXTRANGEA = (WM_USER + 163);
{$EXTERNALSYM AKD_GETTEXTRANGEA}
const AKD_GETTEXTRANGEW = (WM_USER + 164);
{$EXTERNALSYM AKD_GETTEXTRANGEW}
const AKD_GETSELTEXTW = (WM_USER + 165);
{$EXTERNALSYM AKD_GETSELTEXTW}
const AKD_FREETEXT = (WM_USER + 166);
{$EXTERNALSYM AKD_FREETEXT}
const AKD_REPLACESEL = (WM_USER + 167);
{$EXTERNALSYM AKD_REPLACESEL}
const AKD_REPLACESELA = (WM_USER + 168);
{$EXTERNALSYM AKD_REPLACESELA}
const AKD_REPLACESELW = (WM_USER + 169);
{$EXTERNALSYM AKD_REPLACESELW}
const AKD_PASTE = (WM_USER + 170);
{$EXTERNALSYM AKD_PASTE}
const AKD_COPY = (WM_USER + 171);
{$EXTERNALSYM AKD_COPY}
const AKD_TEXTFIND = (WM_USER + 172);
{$EXTERNALSYM AKD_TEXTFIND}
const AKD_TEXTFINDA = (WM_USER + 173);
{$EXTERNALSYM AKD_TEXTFINDA}
const AKD_TEXTFINDW = (WM_USER + 174);
{$EXTERNALSYM AKD_TEXTFINDW}
const AKD_TEXTREPLACE = (WM_USER + 175);
{$EXTERNALSYM AKD_TEXTREPLACE}
const AKD_TEXTREPLACEA = (WM_USER + 176);
{$EXTERNALSYM AKD_TEXTREPLACEA}
const AKD_TEXTREPLACEW = (WM_USER + 177);
{$EXTERNALSYM AKD_TEXTREPLACEW}
const AKD_RECODESEL = (WM_USER + 178);
{$EXTERNALSYM AKD_RECODESEL}
const AKD_GOTO = (WM_USER + 180);
{$EXTERNALSYM AKD_GOTO}
const AKD_GOTOA = (WM_USER + 181);
{$EXTERNALSYM AKD_GOTOA}
const AKD_GOTOW = (WM_USER + 182);
{$EXTERNALSYM AKD_GOTOW}

//Print
const AKD_GETPRINTINFO = (WM_USER + 191);
{$EXTERNALSYM AKD_GETPRINTINFO}
const AKD_SETPRINTINFO = (WM_USER + 192);
{$EXTERNALSYM AKD_SETPRINTINFO}

//Options
const AKD_SETMAININFO = (WM_USER + 195);
{$EXTERNALSYM AKD_SETMAININFO}
const AKD_SETFRAMEINFO = (WM_USER + 196);
{$EXTERNALSYM AKD_SETFRAMEINFO}
const AKD_GETMAININFO = (WM_USER + 198);
{$EXTERNALSYM AKD_GETMAININFO}
const AKD_GETFRAMEINFO = (WM_USER + 199);
{$EXTERNALSYM AKD_GETFRAMEINFO}
const AKD_GETEDITINFO = (WM_USER + 200);
{$EXTERNALSYM AKD_GETEDITINFO}
const AKD_PROGRAMVERSION = (WM_USER + 201);
{$EXTERNALSYM AKD_PROGRAMVERSION}
const AKD_PROGRAMARCHITECTURE = (WM_USER + 202);
{$EXTERNALSYM AKD_PROGRAMARCHITECTURE}
const AKD_SETMODIFY = (WM_USER + 205);
{$EXTERNALSYM AKD_SETMODIFY}
const AKD_SETNEWLINE = (WM_USER + 206);
{$EXTERNALSYM AKD_SETNEWLINE}
const AKD_GETFONT = (WM_USER + 207);
{$EXTERNALSYM AKD_GETFONT}
const AKD_GETFONTA = (WM_USER + 208);
{$EXTERNALSYM AKD_GETFONTA}
const AKD_GETFONTW = (WM_USER + 209);
{$EXTERNALSYM AKD_GETFONTW}
const AKD_SETFONT = (WM_USER + 210);
{$EXTERNALSYM AKD_SETFONT}
const AKD_SETFONTA = (WM_USER + 211);
{$EXTERNALSYM AKD_SETFONTA}
const AKD_SETFONTW = (WM_USER + 212);
{$EXTERNALSYM AKD_SETFONTW}
const AKD_GETCODEPAGELIST = (WM_USER + 213);
{$EXTERNALSYM AKD_GETCODEPAGELIST}
const AKD_RECENTFILES = (WM_USER + 214);
{$EXTERNALSYM AKD_RECENTFILES}
const AKD_SEARCHHISTORY = (WM_USER + 215);
{$EXTERNALSYM AKD_SEARCHHISTORY}
const AKD_SETEDITNOTIFY = (WM_USER + 216);
{$EXTERNALSYM AKD_SETEDITNOTIFY}

//Windows
const AKD_GETMODELESS = (WM_USER + 251);
{$EXTERNALSYM AKD_GETMODELESS}
const AKD_SETMODELESS = (WM_USER + 252);
{$EXTERNALSYM AKD_SETMODELESS}
const AKD_RESIZE = (WM_USER + 253);
{$EXTERNALSYM AKD_RESIZE}
const AKD_DOCK = (WM_USER + 254);
{$EXTERNALSYM AKD_DOCK}
const AKD_SETBUTTONDRAW = (WM_USER + 255);
{$EXTERNALSYM AKD_SETBUTTONDRAW}
const AKD_SETHOTKEYINPUT = (WM_USER + 256);
{$EXTERNALSYM AKD_SETHOTKEYINPUT}
const AKD_DIALOGRESIZE = (WM_USER + 257);
{$EXTERNALSYM AKD_DIALOGRESIZE}
const AKD_UPDATESTATUSUSER = (WM_USER + 258);
{$EXTERNALSYM AKD_UPDATESTATUSUSER}

//Frames
const AKD_FRAMEACTIVATE = (WM_USER + 261);
{$EXTERNALSYM AKD_FRAMEACTIVATE}
const AKD_FRAMENEXT = (WM_USER + 262);
{$EXTERNALSYM AKD_FRAMENEXT}
const AKD_FRAMEDESTROY = (WM_USER + 263);
{$EXTERNALSYM AKD_FRAMEDESTROY}
const AKD_FRAMEFIND = (WM_USER + 264);
{$EXTERNALSYM AKD_FRAMEFIND}
const AKD_FRAMEFINDA = (WM_USER + 265);
{$EXTERNALSYM AKD_FRAMEFINDA}
const AKD_FRAMEFINDW = (WM_USER + 266);
{$EXTERNALSYM AKD_FRAMEFINDW}
const AKD_FRAMESTATS = (WM_USER + 267);
{$EXTERNALSYM AKD_FRAMESTATS}
const AKD_FRAMENOWINDOWS = (WM_USER + 268);
{$EXTERNALSYM AKD_FRAMENOWINDOWS}
const AKD_FRAMEISVALID = (WM_USER + 269);
{$EXTERNALSYM AKD_FRAMEISVALID}
const AKD_FRAMEINDEX = (WM_USER + 270);
{$EXTERNALSYM AKD_FRAMEINDEX}
const AKD_FRAMEINIT = (WM_USER + 271);
{$EXTERNALSYM AKD_FRAMEINIT}
const AKD_FRAMEAPPLYEDIT = (WM_USER + 272);
{$EXTERNALSYM AKD_FRAMEAPPLYEDIT}

//Thread
const AKD_GLOBALALLOC = (WM_USER + 281);
{$EXTERNALSYM AKD_GLOBALALLOC}
const AKD_GLOBALLOCK = (WM_USER + 282);
{$EXTERNALSYM AKD_GLOBALLOCK}
const AKD_GLOBALUNLOCK = (WM_USER + 283);
{$EXTERNALSYM AKD_GLOBALUNLOCK}
const AKD_GLOBALFREE = (WM_USER + 284);
{$EXTERNALSYM AKD_GLOBALFREE}
const AKD_STRLENA = (WM_USER + 285);
{$EXTERNALSYM AKD_STRLENA}
const AKD_STRLENW = (WM_USER + 286);
{$EXTERNALSYM AKD_STRLENW}
const AKD_CREATEWINDOW = (WM_USER + 287);
{$EXTERNALSYM AKD_CREATEWINDOW}
const AKD_WAITKEYBOARD = (WM_USER + 288);
{$EXTERNALSYM AKD_WAITKEYBOARD}
const AKD_GETQUEUE = (WM_USER + 289);
{$EXTERNALSYM AKD_GETQUEUE}
const AKD_POSTMESSAGE = (WM_USER + 290);
{$EXTERNALSYM AKD_POSTMESSAGE}
const AKD_TRANSLATEMESSAGE = (WM_USER + 291);
{$EXTERNALSYM AKD_TRANSLATEMESSAGE}
const AKD_MESSAGEBOX = (WM_USER + 292);
{$EXTERNALSYM AKD_MESSAGEBOX}
const AKD_GETFOCUS = (WM_USER + 293);
{$EXTERNALSYM AKD_GETFOCUS}
const AKD_PEEKMESSAGE = (WM_USER + 294);
{$EXTERNALSYM AKD_PEEKMESSAGE}

//Plugin load
const AKD_DLLCALL = (WM_USER + 301);
{$EXTERNALSYM AKD_DLLCALL}
const AKD_DLLCALLA = (WM_USER + 302);
{$EXTERNALSYM AKD_DLLCALLA}
const AKD_DLLCALLW = (WM_USER + 303);
{$EXTERNALSYM AKD_DLLCALLW}
const AKD_DLLUNLOAD = (WM_USER + 304);
{$EXTERNALSYM AKD_DLLUNLOAD}
const AKD_DLLFIND = (WM_USER + 305);
{$EXTERNALSYM AKD_DLLFIND}
const AKD_DLLFINDA = (WM_USER + 306);
{$EXTERNALSYM AKD_DLLFINDA}
const AKD_DLLFINDW = (WM_USER + 307);
{$EXTERNALSYM AKD_DLLFINDW}
const AKD_DLLADD = (WM_USER + 308);
{$EXTERNALSYM AKD_DLLADD}
const AKD_DLLADDA = (WM_USER + 309);
{$EXTERNALSYM AKD_DLLADDA}
const AKD_DLLADDW = (WM_USER + 310);
{$EXTERNALSYM AKD_DLLADDW}
const AKD_DLLDELETE = (WM_USER + 311);
{$EXTERNALSYM AKD_DLLDELETE}
const AKD_DLLSAVE = (WM_USER + 312);
{$EXTERNALSYM AKD_DLLSAVE}
const AKD_CALLPROC = (WM_USER + 313);
{$EXTERNALSYM AKD_CALLPROC}
const AKD_CHECKHOTKEY = (WM_USER + 314);
{$EXTERNALSYM AKD_CHECKHOTKEY}

//Plugin options
const AKD_BEGINOPTIONS = (WM_USER + 331);
{$EXTERNALSYM AKD_BEGINOPTIONS}
const AKD_BEGINOPTIONSA = (WM_USER + 332);
{$EXTERNALSYM AKD_BEGINOPTIONSA}
const AKD_BEGINOPTIONSW = (WM_USER + 333);
{$EXTERNALSYM AKD_BEGINOPTIONSW}
const AKD_OPTION = (WM_USER + 334);
{$EXTERNALSYM AKD_OPTION}
const AKD_OPTIONA = (WM_USER + 335);
{$EXTERNALSYM AKD_OPTIONA}
const AKD_OPTIONW = (WM_USER + 336);
{$EXTERNALSYM AKD_OPTIONW}
const AKD_ENDOPTIONS = (WM_USER + 341);
{$EXTERNALSYM AKD_ENDOPTIONS}
const AKD_INIOPEN = (WM_USER + 342);
{$EXTERNALSYM AKD_INIOPEN}
const AKD_INIOPENA = (WM_USER + 343);
{$EXTERNALSYM AKD_INIOPENA}
const AKD_INIOPENW = (WM_USER + 344);
{$EXTERNALSYM AKD_INIOPENW}
const AKD_INIGETSECTION = (WM_USER + 345);
{$EXTERNALSYM AKD_INIGETSECTION}
const AKD_INIGETSECTIONA = (WM_USER + 346);
{$EXTERNALSYM AKD_INIGETSECTIONA}
const AKD_INIGETSECTIONW = (WM_USER + 347);
{$EXTERNALSYM AKD_INIGETSECTIONW}
const AKD_INICLEARSECTION = (WM_USER + 348);
{$EXTERNALSYM AKD_INICLEARSECTION}
const AKD_INIDELETESECTION = (WM_USER + 349);
{$EXTERNALSYM AKD_INIDELETESECTION}
const AKD_INIGETKEY = (WM_USER + 350);
{$EXTERNALSYM AKD_INIGETKEY}
const AKD_INIGETKEYA = (WM_USER + 351);
{$EXTERNALSYM AKD_INIGETKEYA}
const AKD_INIGETKEYW = (WM_USER + 352);
{$EXTERNALSYM AKD_INIGETKEYW}
const AKD_INIDELETEKEY = (WM_USER + 353);
{$EXTERNALSYM AKD_INIDELETEKEY}
const AKD_INIGETVALUE = (WM_USER + 354);
{$EXTERNALSYM AKD_INIGETVALUE}
const AKD_INIGETVALUEA = (WM_USER + 355);
{$EXTERNALSYM AKD_INIGETVALUEA}
const AKD_INIGETVALUEW = (WM_USER + 356);
{$EXTERNALSYM AKD_INIGETVALUEW}
const AKD_INISETVALUE = (WM_USER + 357);
{$EXTERNALSYM AKD_INISETVALUE}
const AKD_INISETVALUEA = (WM_USER + 358);
{$EXTERNALSYM AKD_INISETVALUEA}
const AKD_INISETVALUEW = (WM_USER + 359);
{$EXTERNALSYM AKD_INISETVALUEW}
const AKD_INICLOSE = (WM_USER + 360);
{$EXTERNALSYM AKD_INICLOSE}

//Regular expressions. Requires for include "RegExpFunc.h".
const AKD_PATEXEC = (WM_USER + 391);
{$EXTERNALSYM AKD_PATEXEC}
const AKD_PATREPLACE = (WM_USER + 392);
{$EXTERNALSYM AKD_PATREPLACE}
const AKD_PATGROUPSTR = (WM_USER + 393);
{$EXTERNALSYM AKD_PATGROUPSTR}
const AKD_PATGETGROUP = (WM_USER + 395);
{$EXTERNALSYM AKD_PATGETGROUP}
const AKD_PATNEXTGROUP = (WM_USER + 396);
{$EXTERNALSYM AKD_PATNEXTGROUP}
const AKD_PATPREVGROUP = (WM_USER + 397);
{$EXTERNALSYM AKD_PATPREVGROUP}
const AKD_PATFREE = (WM_USER + 399);
{$EXTERNALSYM AKD_PATFREE}

//AkelPad 4.x messages
const AKD_EXGETTEXTLENGTH = (WM_USER + 401);
{$EXTERNALSYM AKD_EXGETTEXTLENGTH}
const AKD_EXGETTEXTRANGE = (WM_USER + 402);
{$EXTERNALSYM AKD_EXGETTEXTRANGE}
const AKD_EXGETTEXTRANGEA = (WM_USER + 403);
{$EXTERNALSYM AKD_EXGETTEXTRANGEA}
const AKD_EXGETTEXTRANGEW = (WM_USER + 404);
{$EXTERNALSYM AKD_EXGETTEXTRANGEW}

{$IF CompilerVersion > 20}{$REGION 'Message manual'}{$IFEND}

(*

AKDN_MAIN_ONSTART
_________________

Internal message.


AKDN_MAIN_ONSTART_PRESHOW
_________________________

Notification message, sends to the main procedure before showing main window.

wParam              == not used.
(NMAINSHOW * )lParam == show main window characteristics, program show code is:
                        NMAINSHOW *nms=(NMAINSHOW * )lParam;

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

Notification message, sends to the main procedure before destroying main window and its children.

wParam == not used.
lParam == not used.

Return Value
 Zero.


AKDN_MAIN_ONDESTROY
___________________

Notification message, sends to the main procedure after children destroyed, but before destroying main window.

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
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure, that has been activated.

Return Value
 Zero.


AKDN_FRAME_DEACTIVATE
_____________________

Notification message, sends to the main procedure before deactivating frame.

(DWORD)wParam       == see FWA_* defines.
(FRAMEDATA * )lParam == pointer to a current FRAMEDATA structure, that will be deactivated.

Return Value
 Zero.


AKDN_FRAME_DESTROY
__________________

Notification message, sends to the main procedure before destroying frame.

wParam              == not used.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure, that will be destroyed.

Return Value
 Zero.


AKDN_DOCK_GETMINMAXINFO
_______________________

Notification message, sends to the main procedure before changing dock window size.

(DOCK * )wParam       == pointer to a DOCK structure.
(MINMAXINFO * )lParam == pointer to a MINMAXINFO structure.

Return Value
 Zero.


AKDN_DOCK_CAPTURE_ONSTART
_________________________

Notification message, sends to the main procedure after mouse capturing started.

(DOCK * )wParam == pointer to a DOCK structure.
(int)lParam    == see DKC_* defines.

Return Value
 Zero.


AKDN_DOCK_CAPTURE_ONFINISH
__________________________

Notification message, sends to the main procedure after mouse capturing finished.

(DOCK * )wParam == pointer to a DOCK structure.
(int)lParam    == see DKC_* defines.

Return Value
 Zero.


AKDN_DOCK_RESIZE
________________

Notification message, sends to the main procedure after dock window size or side is changed.

(DOCK * )wParam == pointer to a DOCK structure.
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


AKDN_SIZE_ONSTART
_________________

Notification message, sends to the main procedure before the main window client RECT changed.

wParam           == not used.
(NSIZE * )lParam  == pointer to a NSIZE structure.

Return Value
 Zero.


AKDN_SIZE_ONFINISH
___________________

Notification message, sends to the main procedure after the main window client RECT changed.

wParam           == not used.
(NSIZE * )lParam  == pointer to a NSIZE structure.

Return Value
 Zero.


AKDN_OPENDOCUMENT_START
_______________________

Notification message, sends to the main procedure before document opened.

(HWND)wParam            == edit window.
(NOPENDOCUMENT * )lParam == pointer to a NOPENDOCUMENT structure.

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
(NSAVEDOCUMENT * )lParam == pointer to a NSAVEDOCUMENT structure.

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
(PLUGINDATA * )lParam == pointer to a PLUGINDATA structure.

Return Value
 Zero.


AKDN_DLLUNLOAD
______________

Notification message, sends to the main procedure after plugin unload.

wParam              == not used.
(UNISTRING * )lParam == pointer to a UNISTRING structure, that specified unloaded function name in format "Plugin::Function".

Return Value
 Zero.


AKDN_HOTKEY
___________

Notification message, sends to the main procedure when keyboard key is pressed.

(WORD)wParam  == hotkey returned by HKM_GETHOTKEY.
(int * )lParam ==  1  stop message processing.
                  0  continue message processing (default).
                 -1  continue message processing, but skip accelerators.

Return Value
 Zero.


AKDN_CONTEXTMENU
________________

Notification message, sends to the main procedure before displaying context menu.

wParam                 == not used.
(NCONTEXTMENU * )lParam == pointer to a NCONTEXTMENU structure.

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
(WNDPROCDATA ** )lParam == procedure data.

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
(WNDPROCDATA ** )lParam == procedure data.
                          If (lParam == NULL) then create new procedure data.
                          If (*(WNDPROCDATA ** )lParam == NULL) then create new
                            procedure data and set it on top of the main window
                            subclass chain.
                          If (*(WNDPROCDATA ** )lParam != NULL) then set wParam
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
(WNDPROCRETDATA ** )lParam == procedure data.

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
(WNDPROCDATA ** )lParam == procedure data.
                          If (lParam == NULL) then create new procedure data.
                          If (*(WNDPROCDATA ** )lParam == NULL) then create new
                            procedure data and set it on top of the main window
                            subclass chain.
                          If (*(WNDPROCDATA ** )lParam != NULL) then set wParam
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

Parse command line string.

wParam                      == not used.
(PARSECMDLINESENDW * )lpData == pointer to a PARSECMDLINESENDW structure.

Return Value
 See PCLE_* defines.

Example:
 PARSECMDLINESENDW pcls;

 pcls.pCmdLine=L"/p \"C:\\MyFile.txt\"";
 pcls.pWorkDir=L"";
 SendMessage(pd->hMainWnd, AKD_PARSECMDLINEW, 0, (LPARAM)&pcls);


AKD_DETECTANSITEXT
__________________

Detect codepage of ansi text.

lParam                   == not used.
(DETECTANSITEXT * )lParam == pointer to a DETECTANSITEXT structure.

Return Value
 TRUE  success.
 FALSE error.

Example:
 DETECTANSITEXT dat;

 dat.dwLangID=(DWORD)-1;
 dat.pText="\x91\x20\xE7\xA5\xA3\xAE\x20\xAD\xA0\xE7\xA8\xAD\xA0\xA5\xE2\xE1\xEF\x20\x90\xAE\xA4\xA8\xAD\xA0";
 dat.nTextLen=-1;
 dat.nMinChars=0;
 SendMessage(pd->hMainWnd, AKD_DETECTANSITEXT, 0, (LPARAM)&dat);


AKD_DETECTUNITEXT
_________________

Detect ansi codepage of unicode text.

lParam                  == not used.
(DETECTUNITEXT * )lParam == pointer to a DETECTUNITEXT structure.

Return Value
 TRUE  success.
 FALSE error.

Example:
 DETECTUNITEXT dut;

 dut.dwLangID=(DWORD)-1;
 dut.wpText=L"\x2018\x0020\x0437\x0490\x0408\x00AE\x0020\x00AD\x00A0\x0437\x0401\x00AD\x00A0\x0490\x0432\x0431\x043F\x0020\x0452\x00AE\x00A4\x0401\x00AD\x00A0";
 dut.nTextLen=-1;
 dut.nMinChars=0;
 SendMessage(pd->hMainWnd, AKD_DETECTUNITEXT, 0, (LPARAM)&dut);


AKD_CONVERTANSITEXT
___________________

Change codepage of ansi text.

lParam                    == not used.
(CONVERTANSITEXT * )lParam == pointer to a CONVERTANSITEXT structure.

Return Value
 TRUE  success.
 FALSE error.

Example:
 CONVERTANSITEXT cat;

 cat.pInput="\x91\x20\xE7\xA5\xA3\xAE\x20\xAD\xA0\xE7\xA8\xAD\xA0\xA5\xE2\xE1\xEF\x20\x90\xAE\xA4\xA8\xAD\xA0";
 cat.nInputLen=-1;
 cat.nCodePageFrom=866;
 cat.nCodePageTo=1251;
 SendMessage(pd->hMainWnd, AKD_CONVERTANSITEXT, 0, (LPARAM)&cat);


AKD_CONVERTUNITEXT
__________________

Change codepage of unicode text.

lParam                   == not used.
(CONVERTUNITEXT * )lParam == pointer to a CONVERTUNITEXT structure.

Return Value
 TRUE  success.
 FALSE error.

Example:
 CONVERTUNITEXT cut;

 cut.wpInput=L"\x2018\x0020\x0437\x0490\x0408\x00AE\x0020\x00AD\x00A0\x0437\x0401\x00AD\x00A0\x0490\x0432\x0431\x043F\x0020\x0452\x00AE\x00A4\x0401\x00AD\x00A0";
 cut.nInputLen=-1;
 cut.nCodePageFrom=1251;
 cut.nCodePageTo=866;
 SendMessage(pd->hMainWnd, AKD_CONVERTUNITEXT, 0, (LPARAM)&cut);


AKD_DETECTFILE, AKD_DETECTFILEA, AKD_DETECTFILEW
______________  _______________  _______________

Detect codepage of a file.

lParam               == not used.
(DETECTFILE * )lParam == pointer to a DETECTFILE structure.

Return Value
 See EDT_* defines.

Example:
 See AKD_READFILECONTENT example.


AKD_READFILECONTENT
___________________

Read contents of a file.

lParam                == not used.
(FILECONTENT * )lParam == pointer to a FILECONTENT structure.

Return Value
 Number of wide characters copied to a FILECONTENT.wpContent buffer.
 When you no longer need the buffer, call the AKD_FREETEXT function to delete it.

Example (bOldWindows == TRUE):
 int ReadFileContentA(const char *pFile, DWORD dwFlags, int nCodePage, BOOL bBOM, wchar_t **wpContent)
 {
   DETECTFILEA df;
   FILECONTENT fc;
   int nResult=0;

   *wpContent=NULL;

   //Detect codepage
   df.pFile=pFile;
   df.dwBytesToCheck=1024;
   df.dwFlags=dwFlags;
   df.nCodePage=nCodePage;
   df.bBOM=bBOM;
   if (SendMessage(hMainWnd, AKD_DETECTFILEA, 0, (LPARAM)&df) == EDT_SUCCESS)
   {
     //Read contents
     if ((fc.hFile=CreateFileA(df.pFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
     {
       fc.dwBytesMax=(UINT_PTR)-1;
       fc.nCodePage=df.nCodePage;
       fc.bBOM=df.bBOM;
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
   DETECTFILEW df;
   FILECONTENT fc;
   int nResult=0;

   *wpContent=NULL;

   //Detect codepage
   df.pFile=wpFile;
   df.dwBytesToCheck=1024;
   df.dwFlags=dwFlags;
   df.nCodePage=nCodePage;
   df.bBOM=bBOM;
   if (SendMessage(hMainWnd, AKD_DETECTFILEW, 0, (LPARAM)&df) == EDT_SUCCESS)
   {
     //Read contents
     if ((fc.hFile=CreateFileW(df.pFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
     {
       fc.dwBytesMax=(UINT_PTR)-1;
       fc.nCodePage=df.nCodePage;
       fc.bBOM=df.bBOM;
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
(OPENDOCUMENT * )lParam == pointer to a OPENDOCUMENT structure.

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
(SAVEDOCUMENT * )lParam == pointer to a SAVEDOCUMENT structure.

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
(GETTEXTRANGE * )lParam == pointer to a GETTEXTRANGE structure.

Return Value
 Text length in TCHARs without null character.

Example (Ansi):
 GETTEXTRANGE gtr;

 gtr.cpMin=0;
 gtr.cpMax=-1;
 if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGEA, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
 {
   MessageBoxA(pd->hMainWnd, (char * )gtr.pText, "Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
 }

Example (Unicode):
 GETTEXTRANGE gtr;

 gtr.cpMin=0;
 gtr.cpMax=-1;
 if (SendMessage(pd->hMainWnd, AKD_GETTEXTRANGEW, (WPARAM)pd->hWndEdit, (LPARAM)&gtr))
 {
   MessageBoxW(pd->hMainWnd, (wchar_t * )gtr.pText, L"Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
 }


AKD_GETSELTEXTW
_______________

Retrieves the currently selected text in a edit control.

(HWND)wParam  == edit window, NULL for current edit window.
(int * )lParam == pointer to a variable that receive text length (not including the terminating null character), can be NULL.

Return Value
 Text pointer, new lines are forced to "\r".

Example:
 wchar_t *wpText;
 int nTextLen=0;

 wpText=(wchar_t * )SendMessage(pd->hMainWnd, AKD_GETSELTEXTW, (WPARAM)pd->hWndEdit, (LPARAM)&nTextLen);
 SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpText);


AKD_FREETEXT
____________

Free text buffer allocated with some AKD_* messages.

wParam                  == not used.
(unsigned char * )lParam == buffer pointer.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_GETTEXTRANGE.


AKD_REPLACESEL, AKD_REPLACESELA, AKD_REPLACESELW
______________  _______________  _______________

Replace selection of the edit control.

(HWND)wParam                  == edit window, NULL for current edit window.
(const unsigned char * )lParam == text pointer.

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
(TEXTFIND * )lParam == pointer to a TEXTFIND structure.

Return Value
 Character position of the next match.
 If there are no more matches, the return value is -1.
 If there is syntax error occurred with FRF_REGEXP or FRF_ESCAPESEQ flag, the return value is (100 - PatternOffset).
 For example, TEXTFINDW.pFindIt equal to "ab[c" with FRF_REGEXP, syntax error in third symbol, return value is 102.

Example (Unicode):
 TEXTFINDW tf;

 tf.dwFlags=FRF_DOWN|FRF_BEGINNING|FRF_MATCHCASE;
 tf.pFindIt=L"Text to find";
 tf.nFindItLen=-1;
 SendMessage(pd->hMainWnd, AKD_TEXTFINDW, (WPARAM)pd->hWndEdit, (LPARAM)&tf);


AKD_TEXTREPLACE, AKD_TEXTREPLACEA, AKD_TEXTREPLACEW
_______________  ________________  ________________

Replaces text in a edit control.

(HWND)wParam          == edit window, NULL for current edit window.
(TEXTREPLACE * )lParam == pointer to a TEXTREPLACE structure.

Return Value
 Character position of the next match.
 If there are no more matches, the return value is -1.
 If there is syntax error occurred with FRF_REGEXP or FRF_ESCAPESEQ flag, the return value is (100 - PatternOffset).
 For example, TEXTREPLACEW.pFindIt equal to "ab[c" with FRF_REGEXP, syntax error in third symbol, return value is 102.

Example (Unicode):
 TEXTREPLACEW tr;

 tr.dwFlags=FRF_DOWN|FRF_BEGINNING|FRF_MATCHCASE;
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
(TEXTRECODE * )lParam == pointer to a TEXTRECODE structure.

Return Value
 Zero.

Example:
 TEXTRECODE tr;

 tr.nCodePageFrom=1251;
 tr.nCodePageTo=866;
 tr.dwFlags=0;
 SendMessage(pd->hMainWnd, AKD_RECODESEL, (WPARAM)pd->hWndEdit, (LPARAM)&tr);


AKD_GOTO, AKD_GOTOA, AKD_GOTOW
________  _________  _________

Go to specified position in text.

(DWORD)wParam                 == see GT_* defines.
(const unsigned char * )lParam == pointer to a string that specified position in text. It is equal to "Go to..." dialog string.

Return Value
 TRUE   success.
 FALSE  failed.

Example (Unicode):
 SendMessage(pd->hMainWnd, AKD_GOTOW, GT_LINE, (LPARAM)L"100:3");


AKD_GETPRINTINFO
________________

Get print information.

(PRINTINFO * )lParam == pointer to a PRINTINFO structure.
lParam              == not used.

Return Value
 Zero.

Example:
 PRINTINFO pi;

 SendMessage(pd->hMainWnd, AKD_GETPRINTINFO, (WPARAM)&pi, 0);


AKD_SETPRINTINFO
________________

Set print information.

(PRINTINFO * )lParam == pointer to a PRINTINFO structure.
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

(int)wParam  == see MI_* defines.
(void)lParam == depend on wParam.

Return Value
 Depend on wParam.

Example (get status bar hadle):
 HWND hWndStatus=(HWND)SendMessage(pd->hMainWnd, AKD_GETMAININFO, MI_WNDSTATUS, 0);

Example (get AkelPad directory):
 wchar_t wszAkelDir[MAX_PATH];

 SendMessage(pd->hMainWnd, AKD_GETMAININFO, MI_AKELDIRW, (WPARAM)wszAkelDir);


AKD_SETMAININFO
_______________

Set main AkelPad data.

(int)wParam  == see MIS_* defines.
(void)lParam == depend on wParam.

Return Value
 TRUE  info changed.
 FALSE info not changed.

Example:
 SendMessage(pd->hMainWnd, AKD_SETMAININFO, MIS_SAVESETTINGS, SS_INI);



AKD_GETFRAMEINFO
________________

Get frame data info.

(void)wParam        == can be combination of FI_* defines or pointer to a FRAMEINFO structure.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure, NULL for current frame data.

Return Value
 If wParam is combination of FI_* defines, then return value is the requested data.
 If wParam is a pointer to a FRAMEINFO structure, then return value is the size copied to FRAMEINFO.dwData member or zero if error.

Example (current edit window):
 HWND hWndEdit=(HWND)SendMessage(pd->hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL);

Example (colors):
 FRAMEINFO fi;
 AECOLORS aec;

 fi.nType=FI_COLORS;
 fi.dwData=(UINT_PTR)&aec;
 SendMessage(pd->hMainWnd, AKD_GETFRAMEINFO, (WPARAM)&fi, (LPARAM)NULL);


AKD_SETFRAMEINFO
________________

Set frame data info.

(FRAMEINFO * )wParam == pointer to a FRAMEINFO structure.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure, NULL for current frame data.

Return Value
 TRUE  info is changed.
 FALSE info isn't changed.

Example:
 BOOL SetFrameInfo(FRAMEDATA *lpFrame, int nType, UINT_PTR dwData)
 {
   FRAMEINFO fi;

   fi.nType=nType;
   fi.dwData=dwData;
   return (BOOL)SendMessage(hMainWnd, AKD_SETFRAMEINFO, (WPARAM)&fi, (LPARAM)lpFrame);
 }
 SetFrameInfo(NULL, FIS_TABSTOPSIZE, 4);


AKD_GETEDITINFO
_______________

Get edit window info.

(HWND)wParam       == edit window, NULL for current edit window.
(EDITINFO * )lParam == pointer to a EDITINFO structure. Can be NULL.

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
(LOGFONT * )lParam == pointer to a LOGFONT structure. Can be NULL.

Return Value
 HFONT handle. If wParam is NULL and no frame windows exist, then returned last collected font handle.

Example:
 See AKD_SETFONT example.


AKD_SETFONT, AKD_SETFONTA, AKD_SETFONTW
___________  ____________  ____________

Set font.

(HWND)wParam      == edit window, NULL for current edit window.
(LOGFONT * )lParam == pointer to a LOGFONT structure.

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

(int * )wParam == if this parameter is not NULL, then the default codepage identifier copies to the variable.
lParam        == not used.

Return Value
 Pointer to a codepage array, in which last element is zero.

Example:
 int *lpCodepageList;
 int nDefaultCodepage;

 lpCodepageList=(int * )SendMessage(pd->hMainWnd, AKD_GETCODEPAGELIST, (WPARAM)&nDefaultCodepage, 0);


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
     MessageBoxW(pd->hMainWnd, rf->wszFile, L"Test", MB_OK);
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


AKD_SETEDITNOTIFY
_________________

Set standard AkelPad's event mask for an edit control.

(HWND)wParam == edit window.
lParam       == not used.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_SETEDITNOTIFY, (LPARAM)hWndEdit, 0);



AKD_GETMODELESS
_______________

Get modeless dialog handle.

wParam        == not used.
(int * )lParam == pointer to a variable that receive dialog MLT_* type. Can be NULL.

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

Remarks
 Only one dialog can be registered as modeless. Application should unregister dialog before closing, passing NULL in wParam.

Example:
 SendMessage(pd->hMainWnd, AKD_SETMODELESS, (LPARAM)hMyDialog, 0);


AKD_RESIZE
__________

Update main window size with AKDN_SIZE_* notification messages.

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
(DOCK * )lParam == pointer to a DOCK structure.

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
 dkNew=(DOCK * )SendMessage(pd->hMainWnd, AKD_DOCK, DK_ADD|DK_SUBCLASS, (LPARAM)&dk);

 SendMessage(pd->hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkNew);


AKD_SETBUTTONDRAW
_________________

Draw icon or bitmap on button.

(HWND)wParam         == button handle. Button must have BS_OWNERDRAW style.
(BUTTONDRAW * )lParam == pointer to a BUTTONDRAW structure. If NULL, button draw processing will be removed.

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
(DIALOGRESIZEMSG * )lParam == pointer to a DIALOGRESIZEMSG structure.

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


AKD_UPDATESTATUSUSER
____________________

Force update user statusbar.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(pd->hMainWnd, AKD_UPDATESTATUSUSER, 0, 0);


AKD_FRAMEACTIVATE
_________________

Activate specified frame.

(DWORD)wParam       == see FWA_* defines.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure.

Return Value
 Pointer to a FRAMEDATA structure that has lost active status.

Example:
 FRAMEDATA *lpFrame=(FRAMEDATA * )SendMessage(pd->hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)L"C:\\File.txt");
 SendMessage(pd->hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrame);


AKD_FRAMENEXT
_____________

Activate next or previous frame.

(BOOL)wParam        == TRUE  activate previous frame.
                       FALSE activate next frame.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure.

Return Value
 Pointer to a FRAMEDATA structure that has been activated.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMENEXT, FALSE, (LPARAM)lpFrameCurrent);


AKD_FRAMEDESTROY
________________

Destroy specified frame.

wParam              == not used.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure.

Return Value
 See FWDE_* defines.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMEDESTROY, 0, (LPARAM)lpFrameCurrent);


AKD_FRAMEFIND, AKD_FRAMEFINDA, AKD_FRAMEFINDW
_____________  ______________  ______________

Find frame data pointer.

(int)wParam    == see FWF_* defines.
(void * )lParam == depend on FWF_ value.

Return Value
 Pointer to a FRAMEDATA structure.

Example:
 See AKD_FRAMEACTIVATE example.


AKD_FRAMESTATS
______________

Retrieve windows statistics.

(int)wParam == see FWS_* defines.
lParam      == not used.

Return Value
 Depend of FWS_* define.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMESTATS, FWS_COUNTALL, 0);


AKD_FRAMENOWINDOWS
__________________

Retrieve is there no windows in MDI client (WMD_MDI) or only one empty window (WMD_PMDI or WMD_SDI).

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
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure.

Return Value
 TRUE   pointer is valid.
 FALSE  pointer is invalid.

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMEISVALID, 0, (LPARAM)lpFrame);


AKD_FRAMEINDEX
______________

Retrieve index of the frame.

(BOOL)wParam        == TRUE   Retrieve frame index in frame stack. First frame has index 1.
                       FALSE  Retrieve frame index in tab item. First frame has index 0.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure.

Return Value
 Frame index. If frame not found then index will be 0 (wParam == TRUE) or -1 (wParam == FALSE).

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMEINDEX, FALSE, (LPARAM)lpFrame);


AKD_FRAMEINIT
_____________

Initialize frame data.

(FRAMEDATA * )wParam == pointer to a source FRAMEDATA structure. If NULL, current frame will be used as source.
(FRAMEDATA * )lParam == pointer to a target FRAMEDATA structure.

Return Value
 Zero.

Example:
 FRAMEDATA *lpFrameTarget;

 if (lpFrameTarget=GlobalAlloc(GPTR, sizeof(FRAMEDATA)))
 {
   SendMessage(pd->hMainWnd, AKD_FRAMEINIT, (WPARAM)NULL, (LPARAM)lpFrameTarget);
   lpFrameTarget->ei.hWndEdit=hWndEdit;
   lpFrameTarget->ei.hDocEdit=hDocEdit;
   SendMessage(pd->hMainWnd, AKD_FRAMEAPPLYEDIT, 0, (WPARAM)lpFrameTarget);
 }


AKD_FRAMEAPPLYEDIT
__________________

Apply frame data to edit window.

wParam              == not used.
(FRAMEDATA * )lParam == pointer to a FRAMEDATA structure.

Return Value
 Zero.

Example:
 See AKD_FRAMEINIT example.



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
 void *pMem=(void * )SendMessage(pd->hMainWnd, AKD_GLOBALLOCK, (WPARAM)hMem, 0);


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

(char * )wParam == ANSI string.
lParam         == not used.

Return Value
 String length.

Example:
 SendMessage(pd->hMainWnd, AKD_STRLENA, (WPARAM)"123", 0);


AKD_STRLENW
___________

Returns the length in WCHAR's of the specified string (not including the terminating null character).

(wchar_t * )wParam == Unicode string.
lParam            == not used.

Return Value
 String length.

Example:
 SendMessage(pd->hMainWnd, AKD_STRLENW, (WPARAM)L"123", 0);


AKD_CREATEWINDOW
________________

Create window.

wParam                 == not used.
(CREATEWINDOW * )lParam == pointer to a CREATEWINDOW structure.
                          (CREATEWINDOWA * )lParam   if bOldWindows == TRUE
                          (CREATEWINDOWW * )lParam   if bOldWindows == FALSE

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
(POSTMESSAGE * )lParam == pointer to a POSTMESSAGE, allocated with GlobalAlloc.

Return Value
 Zero.

Example (Unicode):
 typedef struct {
   POSTMESSAGE pm;
   SAVEDOCUMENTW sd;
   wchar_t szFile[MAX_PATH];
 } PMSAVEDOCUMENTW;

 PMSAVEDOCUMENTW *pmsd;

 if (pmsd=(PMSAVEDOCUMENTW * )GlobalAlloc(GPTR, sizeof(PMSAVEDOCUMENTW)))
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

(DWORD)wParam == see TMSG_* defines.
(MSG * )lParam == pointer to an MSG structure that contains message information.

Return Value
 One of the TMSG_* defines, that was last processed.

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
       SendMessage(pd->hMainWnd, AKD_TRANSLATEMESSAGE, TMSG_ALL, (LPARAM)&msg);
   }
   if (bExitLoop)
     break;
   WaitMessage();
 }


AKD_MESSAGEBOX
______________

Show custom message box.

wParam                     == not used.
(DIALOGMESSAGEBOX * )lParam == pointer to an DIALOGMESSAGEBOX structure that contains message box information.

Return Value
 Button ID.

Example:
 DIALOGMESSAGEBOX dmb;
 BUTTONMESSAGEBOX bmb[]={{IDOK,     L"My &Ok",     BMB_DEFAULT},
                         {IDCANCEL, L"My &Cancel", 0},
                         {0, 0, 0}};

 dmb.hWndParent=pd->hMainWnd;
 dmb.wpText=L"Are you sure you want to quit?";
 dmb.wpCaption=L"Quit prompt";
 dmb.uType=MB_ICONQUESTION;
 dmb.hIcon=NULL;
 dmb.bmb=&bmb[0];
 if (SendMessage(pd->hMainWnd, AKD_MESSAGEBOX, 0, (LPARAM)&dmb) == IDOK)
 {
   //"My &Ok"
 }
 else
 {
   //"My &Cancel"
 }


AKD_GETFOCUS
____________

Retrieve the handle to the window that has the keyboard focus.

wParam == not used.
lParam == not used.

Return Value
 Handle to the window with the keyboard focus. If the main thread's message queue does not have an associated window with the keyboard focus, the return value is NULL.

Example:
 HWND hWndFocus=(HWND)SendMessage(pd->hMainWnd, AKD_GETFOCUS, 0, 0);


AKD_PEEKMESSAGE
_______________

Checks the thread message queue for a posted message, and retrieves the message (if any exist).

(HWND)wParam  == a handle to the window whose messages are to be retrieved. If hWnd is NULL, AKD_PEEKMESSAGE retrieves messages for any window that belongs to the main thread.
(MSG * )lParam == pointer to an MSG structure that contains message information.

Return Value
 TRUE  message is available.
 FALSE no messages are available.

Remarks
 Messages are removed from the queue after processing by AKD_PEEKMESSAGE.

Example (wait for handle and process messages):
void WaitForMutex(hMutex)
{
  MSG msg;
  BOOL bExitLoop=FALSE;

  for (;;)
  {
    while (SendMessage(hMainWnd, AKD_PEEKMESSAGE, (WPARAM)NULL, (LPARAM)&msg))
    {
      if (msg.message == WM_QUIT)
        bExitLoop=TRUE;
      else
        SendMessage(hMainWnd, AKD_TRANSLATEMESSAGE, TMSG_ALL, (LPARAM)&msg);
    }
    if (bExitLoop)
      break;
    if (MsgWaitForMultipleObjects(1, &hMutex, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
      break;
  }
}


AKD_DLLCALL, AKD_DLLCALLA, AKD_DLLCALLW
___________  ____________  ____________

Call dll.

(DWORD)wParam            == see DLLCF_* defines.
(PLUGINCALLSEND * )lParam == pointer to a PLUGINCALLSEND structure if SendMessage used
                            or pointer to a PLUGINCALLPOST, allocated with GlobalAlloc, if PostMessage used.

Return Value
 See UD_* defines.

Example SendMessage (Unicode):
 PLUGINCALLSENDW pcs;

 pcs.pFunction=L"Plugin::Function";
 pcs.lParam=0;
 pcs.dwSupport=0;
 SendMessage(pd->hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

Example PostMessage (Unicode):
 PLUGINCALLPOSTW *pcp;

 if (pcp=(PLUGINCALLPOSTW * )GlobalAlloc(GPTR, sizeof(PLUGINCALLPOSTW)))
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

(const unsigned char * )wParam == function name, format "Plugin::Function".
(WORD)lParam                  == hotkey returned by HKM_GETHOTKEY,
                                 to search by hotkey set wParam to NULL.

Return Value
 Pointer to a PLUGINFUNCTION structure.

Example find by name (Unicode):
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION * )SendMessage(pd->hMainWnd, AKD_DLLFINDW, (WPARAM)L"SomePlugin::SomeFunction", 0))
   if (pf->bRunning) MessageBoxW(pd->hMainWnd, L"Plugin is running", L"Test", MB_OK);

Example find by hotkey:
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION * )SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)NULL, 3112))
   if (pf->bRunning) MessageBoxW(pd->hMainWnd, L"Plugin is running", L"Test", MB_OK);


AKD_DLLADD, AKD_DLLADDA, AKD_DLLADDW
__________  ___________  ___________

Add dll function.

wParam              == not used.
(PLUGINADD * )lParam == function structure pointer.

Return Value
 Pointer to a PLUGINFUNCTION structure in stack.

Example add plugin hotkey (Unicode):
 BOOL CALLBACK PluginProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
 {
   return TRUE; //TRUE - catch hotkey, FALSE - do default hotkey processing.
 }
 PLUGINADDW pa;

 pa.pFunction=L"MyDLL::MyFunction";
 pa.wHotkey=589;       //Ctrl+M
 pa.bAutoLoad=FALSE;
 pa.PluginProc=(PLUGINPROC)PluginProc;
 pa.lpParameter=NULL;
 SendMessage(pd->hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);


AKD_DLLDELETE
_____________

Delete dll function.

wParam                   == not used.
(PLUGINFUNCTION * )lParam == pointer to a PLUGINFUNCTION structure.

Return Value
 Zero.

Example (bOldWindows == TRUE):
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION * )SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)"SomePlugin::SomeFunction", 0))
 {
   SendMessage(pd->hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pf);
   pf=NULL;
 }

Example (bOldWindows == FALSE):
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION * )SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)L"SomePlugin::SomeFunction", 0))
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

(CALLPROC)wParam == procedure address.
(void * )lParam   == pointer to a variable to be passed to the procedure.

Return Value
 Zero.

Example:
 void CALLBACK MyProcedure(void *lpParameter)
 {
 }
 PostMessage(pd->hMainWnd, AKD_CALLPROC, (WPARAM)MyProcedure, (LPARAM)NULL);


AKD_CHECKHOTKEY
_______________

Get hotkey owner and existence.

(WORD)wParam      == hotkey returned by HKM_GETHOTKEY.
(wchar_t * )lParam == buffer that received assigned owner, format L"Plugin::Function". Can be NULL. If not NULL, buffer size must be at least for MAX_PATH characters.

Return Value
 See HKO_* defines.

Example (check F11 hotkey):
 wchar_t wszHotkeyOwner[MAX_PATH];

 if (SendMessage(pd->hMainWnd, AKD_CHECKHOTKEY, 122, (LPARAM)wszHotkeyOwner))
   MessageBoxW(pd->hMainWnd, wszHotkeyOwner, L"Test", MB_OK);
 else
   MessageBoxW(pd->hMainWnd, L"Hotkey not exists", L"Test", MB_OK);


AKD_BEGINOPTIONS, AKD_BEGINOPTIONSA, AKD_BEGINOPTIONSW
________________  _________________  _________________

Begins read or save plugin or program options.

(DWORD)wParam                 == see POB_* defines.
(const unsigned char * )lParam == plugin name, if NULL then begin work with program options.

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
(PLUGINOPTION * )lParam == pointer to a PLUGINOPTION structure.

Return Value
 Size of the data copied to the buffer.

Example:
 See AKD_BEGINOPTIONS examples.


AKD_ENDOPTIONS
______________

Ends plugin options.

(HANDLE)wParam == HANDLE returned by AKD_BEGINOPTIONS.
lParam         == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_BEGINOPTIONS examples.


AKD_INIOPEN, AKD_INIOPENA, AKD_INIOPENW
___________  ____________  ____________

Opens ini file.

(DWORD)wParam                 == see POB_* defines.
(const unsigned char * )lParam == ini file.

Return Value
 HINIFILE. For direct access use pointer to INIFILE structure.

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
(const unsigned char * )lParam == section name.

Return Value
 HINISECTION. For direct access use pointer to INISECTION structure.

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
(const unsigned char * )lParam == key name.

Return Value
 HINIKEY. For direct access use pointer to INIKEY structure.

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
 See AKD_INIGETKEY examples.


AKD_INIGETVALUE, AKD_INIGETVALUEA, AKD_INIGETVALUEW
_______________  ________________  ________________

Retrieve ini value.

(HINIFILE)wParam   == ini file handle.
(INIVALUE * )lParam == pointer to a INIVALUE structure.

Return Value
 Size of the data copied to the buffer.

Example:
 See AKD_INIOPEN examples.


AKD_INISETVALUE, AKD_INISETVALUEA, AKD_INISETVALUEW
_______________  ________________  ________________

Set ini value.

(HINIFILE)wParam   == ini file handle.
(INIVALUE * )lParam == pointer to a INIVALUE structure.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_INIOPEN examples.


AKD_INICLOSE
____________

Close ini file handle.

(HINIFILE)wParam == ini file handle.
lParam           == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 See AKD_INIOPEN examples.


AKD_PATEXEC
___________

Compile and execute regular expressions pattern. Requires for include "RegExpFunc.h".

wParam            == not used.
(PATEXEC * )lParam == pointer to a PATEXEC structure.

Return Value
 Match count.

Example:
 PATEXEC pe;
 REGROUP *lpREGroupRoot;
 REGROUP *lpREGroupNext;
 wchar_t wszResult[MAX_PATH];
 wchar_t *wpResult;

 //Fill structure for AKD_PATEXEC
 pe.lpREGroupStack=0;
 pe.wpStr=L"1234567890 11223344556677889900";
 pe.wpMaxStr=pe.wpStr + lstrlenW(pe.wpStr);
 pe.wpPat=L"(23)(.* )(89)";
 pe.wpMaxPat=pe.wpPat + lstrlenW(pe.wpPat);
 pe.dwOptions=REPE_MATCHCASE;
 pe.wpDelim=NULL;
 pe.lpCallback=NULL;

 while (SendMessage(pd->hMainWnd, AKD_PATEXEC, 0, (LPARAM)&pe))
 {
   lpREGroupRoot=pe.lpREGroupStack->first;
   lpREGroupNext=lpREGroupRoot;
   wpResult=wszResult;

   do
   {
     if (lpREGroupNext->wpStrStart != lpREGroupNext->wpStrEnd && lpREGroupNext->nIndex != -1)
     {
       //wpResult+=xprintfW(wpResult, L"%d [%.%ds]\n", lpREGroupNext->nIndex, lpREGroupNext->wpStrEnd - lpREGroupNext->wpStrStart, lpREGroupNext->wpStrStart);
       wpResult+=wsprintfW(wpResult, L"%d [", lpREGroupNext->nIndex);
       lstrcpynW(wpResult, lpREGroupNext->wpStrStart, (lpREGroupNext->wpStrEnd - lpREGroupNext->wpStrStart) + 1);
       wpResult+=lpREGroupNext->wpStrEnd - lpREGroupNext->wpStrStart;
       wpResult+=wsprintfW(wpResult, L"]\n");
     }
   }
   while (lpREGroupNext=(REGROUP * )SendMessage(pd->hMainWnd, AKD_PATNEXTGROUP, (WPARAM)lpREGroupNext, 0));

   if (MessageBoxW(pd->hMainWnd, wszResult, L"Find next?", MB_YESNO) == IDNO)
     break;
   pe.wpStr=lpREGroupRoot->wpStrEnd;
 }
 SendMessage(pd->hMainWnd, AKD_PATFREE, 0, (LPARAM)&pe);


AKD_PATREPLACE
______________

Replace in string using regular expressions. Requires for include "RegExpFunc.h".

wParam               == not used.
(PATREPLACE * )lParam == pointer to a PATREPLACE structure.

Return Value
 Result string length in characters.

Example:
 PATREPLACE pr;
 INT_PTR nLen;

 //Calculate result string length
 pr.wpStr=L"123ABC200DEF";
 pr.wpMaxStr=pr.wpStr + lstrlenW(pr.wpStr);
 pr.wpPat=L"(.2)";
 pr.wpMaxPat=pr.wpPat + lstrlenW(pr.wpPat);
 pr.wpRep=L"[$1]";
 pr.wpMaxRep=pr.wpRep + lstrlenW(pr.wpRep);
 pr.dwOptions=REPE_GLOBAL|REPE_MATCHCASE;
 pr.wpDelim=NULL;
 pr.wpNewLine=NULL;
 pr.wszResult=NULL;
 nLen=SendMessage(pd->hMainWnd, AKD_PATREPLACE, 0, (LPARAM)&pr);

 //Receive result string
 if (pr.wszResult=(wchar_t * )GlobalAlloc(GMEM_FIXED, nLen * sizeof(wchar_t)))
 {
   SendMessage(pd->hMainWnd, AKD_PATREPLACE, 0, (LPARAM)&pr);
   MessageBoxW(pd->hMainWnd, pr.wszResult, L"Test", MB_OK);
   GlobalFree((HGLOBAL)pr.wszResult);
 }


AKD_PATGROUPSTR
_______________

Translate string that contain group indexes, like "[$1$2]". Requires for include "RegExpFunc.h".

wParam                == not used.
(PATGROUPSTR * )lParam == pointer to a PATGROUPSTR structure.

Return Value
 Result string length in characters.

Example:
 PATEXEC pe;

 //Fill structure for AKD_PATEXEC
 pe.lpREGroupStack=0;
 pe.wpStr=L"1234567890";
 pe.wpMaxStr=pe.wpStr + lstrlenW(pe.wpStr);
 pe.wpPat=L"(23)(.* )(89)";
 pe.wpMaxPat=pe.wpPat + lstrlenW(pe.wpPat);
 pe.dwOptions=REPE_MATCHCASE;
 pe.wpDelim=NULL;
 pe.lpCallback=NULL;

 if (SendMessage(pd->hMainWnd, AKD_PATEXEC, 0, (LPARAM)&pe))
 {
   PATGROUPSTR pgs;
   INT_PTR nLen;

   //Calculate result string length
   pgs.lpREGroupStack=pe.lpREGroupStack;
   pgs.wpStr=L"<$3><$1><$2>";
   pgs.wpMaxStr=pgs.wpStr + lstrlenW(pgs.wpStr);
   pgs.wszResult=NULL;
   nLen=SendMessage(pd->hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs);

   //Receive result string
   if (pgs.wszResult=(wchar_t * )GlobalAlloc(GMEM_FIXED, nLen * sizeof(wchar_t)))
   {
     SendMessage(pd->hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs);
     MessageBoxW(pd->hMainWnd, pgs.wszResult, L"Test", MB_OK);
     GlobalFree((HGLOBAL)pgs.wszResult);
   }
 }
 SendMessage(pd->hMainWnd, AKD_PATFREE, 0, (LPARAM)&pe);


AKD_PATGETGROUP
_______________

Retrieve pattern group by index. Requires for include "RegExpFunc.h".

(STACKREGROUP * )wParam == pointer to a STACKREGROUP structure.
(int)lParam            == group index.

Return Value
 Pointer to a REGROUP structure.

Example:
 See AKD_PATEXEC example.


AKD_PATNEXTGROUP
________________

Retrieve next pattern group. Requires for include "RegExpFunc.h".

(REGROUP * )wParam == pointer to a REGROUP structure.
lParam            == not used.

Return Value
 Pointer to a next REGROUP structure.

Example:
 See AKD_PATEXEC example.


AKD_PATPREVGROUP
________________

Retrieve previous pattern group. Requires for include "RegExpFunc.h".

(REGROUP * )wParam == pointer to a REGROUP structure.
lParam            == not used.

Return Value
 Pointer to a previous REGROUP structure.

Example:
 See AKD_PATEXEC example.


AKD_PATFREE
___________

Free regular expressions pattern. Requires for include "RegExpFunc.h".

wParam            == not used.
(PATEXEC * )lParam == pointer to a PATEXEC structure.

Return Value
 Zero.

Example:
 See AKD_PATEXEC example.


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
(EXGETTEXTRANGE * )lParam == pointer to a EXGETTEXTRANGE structure.

Return Value
 Text length in TCHARs.

Example (bOldWindows == FALSE):
 EXGETTEXTRANGE tr;

 SendMessage(pd->hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr);
 tr.pText=NULL;
 tr.nNewLine=AELB_ASIS;

 if (SendMessage(pd->hMainWnd, AKD_EXGETTEXTRANGE, (WPARAM)pd->hWndEdit, (LPARAM)&tr))
 {
   MessageBoxW(pd->hMainWnd, (wchar_t * )tr.pText, L"Test", MB_OK);
   SendMessage(pd->hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
 }

*)

{$IF CompilerVersion > 20}{$ENDREGION}{$IFEND}

//// AkelPad WM_COPYDATA messages

const CD_OPENDOCUMENT   = 1;
{$EXTERNALSYM CD_OPENDOCUMENT}
const CD_OPENDOCUMENTA  = 2;
{$EXTERNALSYM CD_OPENDOCUMENTA}
const CD_OPENDOCUMENTW  = 3;
{$EXTERNALSYM CD_OPENDOCUMENTW}
const CD_PARSECMDLINEW  = 9;
{$EXTERNALSYM CD_PARSECMDLINEW}

(*
CD_OPENDOCUMENT, CD_OPENDOCUMENTA, CD_OPENDOCUMENTW
_______________  ________________  ________________

Open file. Same as AKD_OPENDOCUMENT, but can be used from outside of AkelPad process.

(DWORD)dwData              == CD_OPENDOCUMENT.
(DWORD)cbData              == sizeof(OPENDOCUMENTPOSTA)    if dwData == CD_OPENDOCUMENTA
                              sizeof(OPENDOCUMENTPOSTW)    if dwData == CD_OPENDOCUMENTW
(OPENDOCUMENTPOST * )lpData == (OPENDOCUMENTPOSTA * )lpData  if dwData == CD_OPENDOCUMENTA
                              (OPENDOCUMENTPOSTW * )lpData  if dwData == CD_OPENDOCUMENTW

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
(PARSECMDLINEPOSTW * )lpData == pointer to a PARSECMDLINEPOSTW structure.

Return Value
 If PARSECMDLINEPOST.bPostMessage is FALSE, then return value is PCLE_* defines.
 If PARSECMDLINEPOST.bPostMessage is TRUE, then return value is zero.

Example (Unicode):
 COPYDATASTRUCT cds;
 PARSECMDLINEPOSTW *pclp;
 wchar_t *wpCmdLine=L"/p \"C:\\MyFile.txt\""

 if (pclp=(PARSECMDLINEPOSTW * )GlobalAlloc(GMEM_FIXED, sizeof(PARSECMDLINEPOSTW)))
 {
   pclp->bPostMessage=TRUE;
   pclp->nCmdLineLen=lstrcpynW(pclp->szCmdLine, wpCmdLine, COMMANDLINE_SIZE);
   pclp->nWorkDirLen=GetCurrentDirectoryWide(MAX_PATH, pclp->szWorkDir);

   cds.dwData=CD_PARSECMDLINEW;
   cds.cbData=sizeof(PARSECMDLINEPOSTW);
   cds.lpData=(PVOID)pclp;
   SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
   GlobalFree((HGLOBAL)pclp);
 }
*)

//// UNICODE define
type
{$ifndef UNICODE}
  TDETECTFILE = TDETECTFILEA;
  {$EXTERNALSYM TDETECTFILE}
  TOPENDOCUMENT = TOPENDOCUMENTA;
  {$EXTERNALSYM TOPENDOCUMENT}
  TOPENDOCUMENTPOST = TOPENDOCUMENTPOSTA;
  {$EXTERNALSYM TOPENDOCUMENTPOST}
  TSAVEDOCUMENT = TSAVEDOCUMENTA;
  {$EXTERNALSYM TSAVEDOCUMENT}
  TPLUGINADD = TPLUGINADDA;
  {$EXTERNALSYM TPLUGINADD}
  TPLUGINCALLSEND = TPLUGINCALLSENDA;
  {$EXTERNALSYM TPLUGINCALLSEND}
  TPLUGINCALLPOST = TPLUGINCALLPOSTA;
  {$EXTERNALSYM TPLUGINCALLPOST}
  TPLUGINOPTION = TPLUGINOPTIONA;
  {$EXTERNALSYM TPLUGINOPTION}
  TINIVALUE = TINIVALUEA;
  {$EXTERNALSYM TINIVALUE}
  TTEXTFIND = TTEXTFINDA;
  {$EXTERNALSYM TTEXTFIND}
  TTEXTREPLACE = TTEXTREPLACEA;
  {$EXTERNALSYM TTEXTREPLACE}
  TCREATEWINDOW = TCREATEWINDOWA;
  {$EXTERNALSYM TCREATEWINDOW}
{$else}
  TDETECTFILE = TDETECTFILEW;
  {$EXTERNALSYM TDETECTFILE}
  TOPENDOCUMENT = TOPENDOCUMENTW;
  {$EXTERNALSYM TOPENDOCUMENT}
  TOPENDOCUMENTPOST = TOPENDOCUMENTPOSTW;
  {$EXTERNALSYM TOPENDOCUMENTPOST}
  TSAVEDOCUMENT = TSAVEDOCUMENTW;
  {$EXTERNALSYM TSAVEDOCUMENT}
  TPLUGINADD = TPLUGINADDW;
  {$EXTERNALSYM TPLUGINADD}
  TPLUGINCALLSEND = TPLUGINCALLSENDW;
  {$EXTERNALSYM TPLUGINCALLSEND}
  TPLUGINCALLPOST = TPLUGINCALLPOSTW;
  {$EXTERNALSYM TPLUGINCALLPOST}
  TPLUGINOPTION = TPLUGINOPTIONW;
  {$EXTERNALSYM TPLUGINOPTION}
  TINIVALUE = TINIVALUEW;
  {$EXTERNALSYM TINIVALUE}
  TTEXTFIND = TTEXTFINDW;
  {$EXTERNALSYM TTEXTFIND}
  TTEXTREPLACE = TTEXTREPLACEW;
  {$EXTERNALSYM TTEXTREPLACE}
  TCREATEWINDOW = TCREATEWINDOWW;
  {$EXTERNALSYM TCREATEWINDOW}
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

function AkelDLL: DWORD;
begin
  Result := MakeIdentifier(AkelDLLVer[1], AkelDLLVer[2], AkelDLLVer[3], AkelDLLVer[4]);
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
