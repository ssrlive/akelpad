#ifndef __AKELDLL_H__
#define __AKELDLL_H__


//// Version

#ifndef MAKE_IDENTIFIER
  #define MAKE_IDENTIFIER(a, b, c, d)  ((DWORD)MAKELONG(MAKEWORD(a, b), MAKEWORD(c, d)))
#endif

#define AKELDLL MAKE_IDENTIFIER(2, 0, 0, 2)


$$$ DEFINES1 $$$
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

//Hotkey owner
#define HKO_NONE                  0  //Hotkey is not assigned.
#define HKO_PLUGINFUNCTION        1  //Hotkey assigned to plugin function.
#define HKO_HOTKEYSPLUGIN         2  //Hotkey assigned to Hotkeys plugin command.

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

//MI_ONFINISH type
#define MOF_NONE        0
#define MOF_QUERYEND    1 //Processing WM_CLOSE or WM_QUERYENDSESSION message.
#define MOF_DESTROY     2 //Processing AKDN_MAIN_ONFINISH message.

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
#define OD_NOSCROLL              0x00000200           //Don't restore scroll position.
#define OD_MULTIFILE             0x00000400           //More documents is on queue. Use MB_YESNOCANCEL instead of MB_OKCANCEL.

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
#define EOD_MSGCANCELCREATE      -21        //User press cancel in message "Create new file?".
#define EOD_MSGCANCELBINARY      -22        //User press cancel in message "Binary file. Continue?".

//Save document flags
#define SD_UPDATE            0x00000001  //Update file info.
#define SD_SELECTION         0x00000002  //Save only selection.

//Save document errors
#define ESD_SUCCESS          0  //Success.
#define ESD_OPEN            -1  //Can't open file.
#define ESD_WRITE           -2  //Can't write to file.
#define ESD_READONLY        -3  //File has read-only attribute.
#define ESD_CODEPAGE_ERROR  -4  //Code page isn't implemented.
#define ESD_STOP            -5  //Stopped from AKDN_SAVEDOCUMENT_START.
#define ESD_STREAMOUT       -6  //Error in EM_STREAMOUT.
#define ESD_NOWINDOW        -7  //No window to save from.

//Code pages int
#define CP_UNICODE_UTF16LE  1200
#define CP_UNICODE_UTF16BE  1201
#define CP_UNICODE_UTF32LE  12000
#define CP_UNICODE_UTF32BE  12001
#define CP_UNICODE_UTF7     65000
#define CP_UNICODE_UTF8     65001
#define CP_KOI8_R           20866

//Save settings
#define SS_REGISTRY   1  //Registry.
#define SS_INI        2  //INI file.

//AKD_RECENTFILES flags
#define RF_GET             1  //Retrieve current recent files info.
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
#define SH_GET    1  //Retrieve search strings count.
#define SH_CLEAR  2  //Clear search history.

//AKD_GETMAININFO type

//PLUGINDATA
#define MI_AKELDIRA                  1    //Return: copied chars. (char *)lParam - buffer that receives AkelPad directory string.
#define MI_AKELDIRW                  2    //Return: copied chars. (wchar_t *)lParam - buffer that receives AkelPad directory string.
#define MI_INSTANCEEXE               3    //Return: EXE instance.
#define MI_PLUGINSSTACK              4    //Return: copied bytes. (HSTACK *)lParam - buffer that receives plugin stack.
#define MI_SAVESETTINGS              5    //Return: see SS_* defines.
#define MI_WNDPROGRESS               10   //Return: progress bar window handle.
#define MI_WNDSTATUS                 11   //Return: status bar window handle.
#define MI_WNDMDICLIENT              12   //Return: MDI client window handle.
#define MI_WNDTAB                    13   //Return: tab window handle.
#define MI_MENUMAIN                  21   //Return: main menu handle.
#define MI_MENURECENTFILES           22   //Return: recent files menu handle.
#define MI_MENULANGUAGE              23   //Return: language menu handle.
#define MI_MENUPOPUP                 24   //Return: right click menu handle.
#define MI_ICONMAIN                  31   //Return: main window icon handle.
#define MI_ACCELGLOBAL               32   //Return: global accelerator table (highest priority).
#define MI_ACCELMAIN                 33   //Return: main accelerator table (lowest priority).
#define MI_OLDWINDOWS                41   //Return: non-Unicode Windows.
#define MI_OLDRICHEDIT               42   //Return: riched20.dll lower then 5.30 (v3.0).
#define MI_OLDCOMCTL32               43   //Return: comctl32.dll lower then 4.71.
#define MI_AKELEDIT                  44   //Return: AkelEdit control is used.
#define MI_MDI                       45   //Return: window mode, see WMD_* defines.
#define MI_LANGMODULEA               51   //Return: copied chars. (char *)lParam - buffer that receives language module string.
#define MI_LANGMODULEW               52   //Return: copied chars. (wchar_t *)lParam - buffer that receives language module string.
#define MI_LANGIDSYSTEM              53   //Return: system language ID.
#define MI_LANGIDMODULE              54   //Return: language module language ID.
//Execution
#define MI_ONSTART                   90   //Return: TRUE - execution is between AKDN_MAIN_ONSTART and AKDN_MAIN_ONSTART_FINISH, FALSE - elsewhere.
#define MI_ONFINISH                  91   //Return: see MOF_* defines.
#define MI_AKELEXEA                  95   //Return: copied chars. (char *)lParam - buffer that receives AkelPad executable string.
#define MI_AKELEXEW                  96   //Return: copied chars. (wchar_t *)lParam - buffer that receives AkelPad executable string.
//Compile
#define MI_X64                       101  //Return: TRUE - x64 version, FALSE - x86 version.
#define MI_AKELEDITSTATICBUILD       102  //Return: TRUE - AkelEdit is compiled statically, FALSE - AkelEdit is compiled as standalone library.
#define MI_AKELPADDLLBUILD           103  //Return: TRUE - AkelPad is compiled as library, FALSE - AkelPad is compiled as executable.
//Manual
#define MI_CMDLINEBEGIN              105  //Return: copied chars. (wchar_t *)lParam - buffer that receives "CmdLineBegin" string.
#define MI_CMDLINEEND                106  //Return: copied chars. (wchar_t *)lParam - buffer that receives "CmdLineEnd" string.
#define MI_SHOWMODIFY                110  //Return: "ShowModify" flags, see SM_* defines.
#define MI_STATUSPOSTYPE             111  //Return: "StatusPosType" type, see SPT_* defines.
#define MI_STATUSUSERFORMAT          112  //Return: copied chars. (wchar_t *)lParam - buffer that receives "StatusUserFormat" string.
#define MI_WORDBREAKCUSTOM           117  //Return: "WordBreak" flags.
#define MI_PAINTOPTIONS              121  //Return: "PaintOptions" flags, see PAINT_* defines.
#define MI_EDITSTYLE                 122  //Return: "EditStyle" flags, see EDS_* defines.
#define MI_RICHEDITCLASS             125  //Return: "RichEditClass" type.
#define MI_AKELADMINRESIDENT         126  //Return: AkelAdmin.exe resident - TRUE or unloaded immediately after execution - FALSE.
#define MI_DATELOGFORMAT             129  //Return: copied chars. (wchar_t *)lParam - buffer that receives "DateLogFormat" string.
#define MI_DATEINSERTFORMAT          130  //Return: copied chars. (wchar_t *)lParam - buffer that receives "DateInsertFormat" string.
#define MI_AKELUPDATEROPTIONS        131  //Return: copied chars. (wchar_t *)lParam - buffer that receives "AkelUpdaterOptions" string.
#define MI_URLCOMMAND                132  //Return: copied chars. (wchar_t *)lParam - buffer that receives "UrlCommand" string.
#define MI_TABNAMEFIND               133  //Return: copied chars. (wchar_t *)lParam - buffer that receives "TabNameFind" string.
#define MI_TABNAMEREP                134  //Return: copied chars. (wchar_t *)lParam - buffer that receives "TabNameRep" string.
//Menu
#define MI_ONTOP                     141  //Return: always on top (on\off).
#define MI_STATUSBAR                 142  //Return: show statusbar (on\off).
#define MI_KEEPSPACE                 146  //Return: keep left space (on\off).
#define MI_WATCHFILE                 147  //Return: watch file change (on\off).
#define MI_SAVETIME                  148  //Return: save original file time (on\off).
#define MI_SINGLEOPENFILE            152  //Return: single open file (on\off).
#define MI_SINGLEOPENPROGRAM         153  //Return: single open program flags, see SOP_* defines.
#define MI_TABOPTIONSMDI             157  //Return: tab flags, see TAB_* defines.
//Settings dialog
#define MI_EXECUTECOMMAND            171  //Return: copied chars. (wchar_t *)lParam - buffer that receives execution command string.
#define MI_EXECUTEDIRECTORY          172  //Return: copied chars. (wchar_t *)lParam - buffer that receives execution directory string.
#define MI_CODEPAGELIST              176  //Return: copied bytes. (int *)lParam - buffer that receives array of codepages, last element in array is zero.
#define MI_DEFAULTCODEPAGE           177  //Return: default codepage.
#define MI_DEFAULTBOM                178  //Return: default BOM.
#define MI_NEWFILECODEPAGE           179  //Return: new file codepage.
#define MI_NEWFILEBOM                180  //Return: new file BOM.
#define MI_NEWFILENEWLINE            181  //Return: new file new line, see NEWLINE_* defines.
#define MI_LANGCODEPAGERECOGNITION   183  //Return: codepage recognition language defined as LANGID.
#define MI_CODEPAGERECOGNITIONBUFFER 184  //Return: size of codepage recognition buffer.
#define MI_SAVEPOSITIONS             192  //Return: save recent file positions (on\off).
#define MI_SAVECODEPAGES             193  //Return: save recent file codepages (on\off).
#define MI_RECENTFILES               194  //Return: number of recent files.
#define MI_SEARCHSTRINGS             198  //Return: number of search strings.
#define MI_FILETYPESOPEN             202  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated file types to open.
#define MI_FILETYPESEDIT             203  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated file types to edit.
#define MI_FILETYPESPRINT            204  //Return: copied chars. (wchar_t *)lParam - buffer that receives associated file types to print.
#define MI_FILETYPESASSOCIATED       205  //Return: associated file types, see FTA_* defines.
#define MI_KEYBLAYOUTOPTIONS         209  //Return: keyboard layout options, see KLO_* defines.
#define MI_SILENTCLOSEEMPTYMDI       213  //Return: silently close unsaved empty MDI tab (on\off).
#define MI_DATELOG                   217  //Return: insert date if file has .LOG at the beginning (on\off).
#define MI_SAVEINREADONLYMSG         221  //Return: save in read-only files warning (on\off).
#define MI_DEFAULTSAVEEXT            224  //Return: copied chars. (wchar_t *)lParam - buffer that receives default saving extension string.
#define MI_SEARCHOPTIONS             228  //Return: search options, see FRF_* defines.
//Print dialog
#define MI_PRINTMARGINS              251  //Return: copied bytes. (RECT *)lParam - buffer that receives print margins.
#define MI_PRINTCOLOR                255  //Return: color printing, see PRNC_* defines.
#define MI_PRINTHEADERENABLE         259  //Return: enable print header (on\off).
#define MI_PRINTHEADER               260  //Return: copied chars. (wchar_t *)lParam - buffer that receives print header string.
#define MI_PRINTFOOTERENABLE         261  //Return: enable print footer (on\off).
#define MI_PRINTFOOTER               262  //Return: copied chars. (wchar_t *)lParam - buffer that receives print footer string.
#define MI_PRINTFONTENABLE           266  //Return: enable print font (on\off).
#define MI_PRINTFONTW                267  //Return: copied bytes. (LOGFONTW *)lParam - buffer that receives print font data.
//Open dialog
#define MI_LASTDIR                   281  //Return: copied chars. (wchar_t *)lParam - buffer that receives last directory of open dialog.
#define MI_SHOWPLACESBAR             282  //Return: show places bar in open dialog (on\off).

//AKD_SETMAININFO type

//PLUGINDATA
#define MIS_SAVESETTINGS              5    //(int)lParam - see SS_* defines.
#define MIS_MDI                       45   //(int)lParam - window mode, see WMD_* defines. Required program restart.
#define MIS_LANGMODULEA               51   //(char *)lParam - language module string. Required program restart.
#define MIS_LANGMODULEW               52   //(wchar_t *)lParam - language module string. Required program restart.
//Manual
#define MIS_CMDLINEBEGIN              105  //(wchar_t *)lParam - "CmdLineBegin" string.
#define MIS_CMDLINEEND                106  //(wchar_t *)lParam - "CmdLineEnd" string.
#define MIS_SHOWMODIFY                110  //(DWORD)lParam - "ShowModify" flags, see SM_* defines.
#define MIS_STATUSPOSTYPE             111  //(DWORD)lParam - "StatusPosType" type, see SPT_* defines.
#define MIS_STATUSUSERFORMAT          112  //(wchar_t *)lParam - "StatusUserFormat" string.
#define MIS_WORDBREAKCUSTOM           117  //(DWORD)lParam - "WordBreak" flags. Changes are applied for a new edit windows.
#define MIS_PAINTOPTIONS              121  //(DWORD)lParam - "PaintOptions" flags, see PAINT_* defines. Changes are applied for a new edit windows.
#define MIS_EDITSTYLE                 122  //(DWORD)lParam - "EditStyle" flags, see EDS_* defines. Changes are applied for a new edit windows.
#define MIS_RICHEDITCLASS             125  //(BOOL)lParam - "RichEditClass" type. Changes are applied for a new edit windows.
#define MIS_AKELADMINRESIDENT         126  //(BOOL)lParam - AkelAdmin.exe resident - TRUE or unloaded immediately after execution - FALSE.
#define MIS_DATELOGFORMAT             129  //(wchar_t *)lParam - "DateLogFormat" string.
#define MIS_DATEINSERTFORMAT          130  //(wchar_t *)lParam - "DateInsertFormat" string.
#define MIS_AKELUPDATEROPTIONS        131  //(wchar_t *)lParam - "AkelUpdaterOptions" string.
#define MIS_URLCOMMAND                132  //(wchar_t *)lParam - "UrlCommand" string.
#define MIS_TABNAMEFIND               133  //(wchar_t *)lParam - "TabNameFind" string. Changes are applied for a new opened documents.
#define MIS_TABNAMEREP                134  //(wchar_t *)lParam - "TabNameRep" string. Changes are applied for a new opened documents.
//Menu
#define MIS_ONTOP                     141  //(BOOL)lParam - always on top (on\off).
#define MIS_STATUSBAR                 142  //(BOOL)lParam - show statusbar (on\off).
#define MIS_KEEPSPACE                 146  //(BOOL)lParam - keep left space (on\off).
#define MIS_WATCHFILE                 147  //(BOOL)lParam - watch file change (on\off).
#define MIS_SAVETIME                  148  //(BOOL)lParam - save original file time (on\off).
#define MIS_SINGLEOPENFILE            152  //(BOOL)lParam - single open file (on\off).
#define MIS_SINGLEOPENPROGRAM         153  //(DWORD)lParam - single open program flags, see SOP_* defines.
#define MIS_TABOPTIONSMDI             157  //(DWORD)lParam - tab flags, see TAB_* defines.
//Settings dialog
#define MIS_EXECUTECOMMAND            171  //(wchar_t *)lParam - execution command string.
#define MIS_EXECUTEDIRECTORY          172  //(wchar_t *)lParam - execution directory string.
#define MIS_CODEPAGELIST              176  //(int *)lParam - array of codepages, last element in array is zero.
#define MIS_DEFAULTCODEPAGE           177  //(int)lParam - default codepage.
#define MIS_DEFAULTBOM                178  //(BOOL)lParam - default BOM.
#define MIS_NEWFILECODEPAGE           179  //(int)lParam - new file codepage.
#define MIS_NEWFILEBOM                180  //(BOOL)lParam - new file BOM.
#define MIS_NEWFILENEWLINE            181  //(int)lParam - new file new line, see NEWLINE_* defines.
#define MIS_LANGCODEPAGERECOGNITION   183  //(DWORD)lParam - codepage recognition language defined as LANGID.
#define MIS_CODEPAGERECOGNITIONBUFFER 184  //(DWORD)lParam - size of codepage recognition buffer.
#define MIS_SAVEPOSITIONS             192  //(BOOL)lParam - save recent file positions (on\off).
#define MIS_SAVECODEPAGES             193  //(BOOL)lParam - save recent file codepages (on\off).
#define MIS_RECENTFILES               194  //(int)lParam - number of recent files.
#define MIS_SEARCHSTRINGS             198  //(int)lParam - number of search strings.
#define MIS_FILETYPESOPEN             202  //(wchar_t *)lParam - associated file types to open.
#define MIS_FILETYPESEDIT             203  //(wchar_t *)lParam - associated file types to edit.
#define MIS_FILETYPESPRINT            204  //(wchar_t *)lParam - associated file types to print.
#define MIS_FILETYPESASSOCIATED       205  //(DWORD)lParam - associated file types, see FTA_* defines. If FTA_ flag is set file types will be associated. If FTA_ flag is not set file types will be deassociated.
#define MIS_KEYBLAYOUTOPTIONS         209  //(DWORD)lParam - keyboard layout options, see KLO_* defines.
#define MIS_SILENTCLOSEEMPTYMDI       213  //(BOOL)lParam - silently close unsaved empty MDI tab (on\off).
#define MIS_DATELOG                   217  //(BOOL)lParam - insert date if file has .LOG at the beginning (on\off).
#define MIS_SAVEINREADONLYMSG         221  //(BOOL)lParam - save in read-only files warning (on\off).
#define MIS_DEFAULTSAVEEXT            224  //(wchar_t *)lParam - default saving extension string.
#define MIS_SEARCHOPTIONS             228  //(DWORD)lParam - search options, see FRF_* defines.
//Print dialog
#define MIS_PRINTMARGINS              251  //(RECT *)lParam - print margins.
#define MIS_PRINTCOLOR                255  //(DWORD)lParam - color printing, see PRNC_* defines.
#define MIS_PRINTHEADERENABLE         259  //(BOOL)lParam - enable print header (on\off).
#define MIS_PRINTHEADER               260  //(wchar_t *)lParam - print header string.
#define MIS_PRINTFOOTERENABLE         261  //(BOOL)lParam - enable print footer (on\off).
#define MIS_PRINTFOOTER               262  //(wchar_t *)lParam - print footer string.
#define MIS_PRINTFONTENABLE           266  //(BOOL)lParam - enable print font (on\off).
#define MIS_PRINTFONTW                267  //(LOGFONTW *)lParam - print font data.
//Open dialog
#define MIS_LASTDIR                   281  //(wchar_t *)lParam - last directory of open dialog.
#define MIS_SHOWPLACESBAR             282  //(BOOL)lParam - show places bar in open dialog (on\off).

//AKD_GETFRAMEINFO type. See FRAMEDATA members description.
#define FI_WNDEDITPARENT        1
#define FI_WNDEDIT              2
#define FI_DOCEDIT              3
#define FI_WNDMASTER            4
#define FI_DOCMASTER            5
#define FI_WNDCLONE1            6
#define FI_DOCCLONE1            7
#define FI_WNDCLONE2            8
#define FI_DOCCLONE2            9
#define FI_WNDCLONE3            10
#define FI_DOCCLONE3            11
#define FI_CODEPAGE             12
#define FI_BOM                  13
#define FI_NEWLINE              14
#define FI_MODIFIED             15
#define FI_READONLY             16
#define FI_WORDWRAP             17
#define FI_OVERTYPE             18
#define FI_FILEA                31
#define FI_FILEW                32
#define FI_FILELEN              33
#define FI_STREAMOFFSET         34
#define FI_ICONHANDLE           38
#define FI_ICONINDEX            39
#define FI_RECTEDIT             43
#define FI_RECTMASTER           44
#define FI_EDITFONTW            48
#define FI_TABSTOPSIZE          52
#define FI_TABSTOPASSPACES      53
#define FI_UNDOLIMIT            57
#define FI_DETAILEDUNDO         58
#define FI_WRAPTYPE             62
#define FI_WRAPLIMIT            63
#define FI_MARKER               64
#define FI_CARETWIDTH           68
#define FI_CARETOPTIONS         69
#define FI_ALTLINEFILL          73
#define FI_ALTLINESKIP          74
#define FI_ALTLINEBORDER        75
#define FI_RECTMARGINS          79
#define FI_LINEGAP              83
#define FI_MOUSEOPTIONS         87
#define FI_CLICKURL             88
#define FI_SHOWURL              89
#define FI_URLPREFIXESENABLE    93
#define FI_URLPREFIXES          94
#define FI_URLDELIMITERSENABLE  98
#define FI_URLLEFTDELIMITERS    99
#define FI_URLRIGHTDELIMITERS   103
#define FI_WORDDELIMITERSENABLE 107
#define FI_WORDDELIMITERS       108
#define FI_WRAPDELIMITERSENABLE 112
#define FI_WRAPDELIMITERS       113
#define FI_MAPPEDPRINTWIDTH     121
#define FI_INPUTLOCALE          125
#define FI_LOCKINHERIT          129
#define FI_FILETIME             133
#define FI_COLORS               137
#define FI_BKIMAGEFILE          140
#define FI_BKIMAGEALPHA         141

//AKD_SETFRAMEINFO type.
#define FIS_TABSTOPSIZE          1   //(int)FRAMEINFO.dwData - tabulation size in characters.
#define FIS_TABSTOPASSPACES      2   //(BOOL)FRAMEINFO.dwData - insert tabulation as spaces.
#define FIS_UNDOLIMIT            6   //(int)FRAMEINFO.dwData - undo limit.
#define FIS_DETAILEDUNDO         7   //(BOOL)FRAMEINFO.dwData - detailed undo.
#define FIS_WRAP                 11  //(DWORD)FRAMEINFO.dwData - the low-order word contains the wrap type (see AEWW_* defines in AkelEdit.h). The high-order word contains the wrap limit in characters.
#define FIS_MARKER               12  //(int)FRAMEINFO.dwData - column marker position.
#define FIS_CARETWIDTH           16  //(int)FRAMEINFO.dwData - caret width.
#define FIS_CARETOPTIONS         17  //(DWORD)FRAMEINFO.dwData - caret options, see CO_* defines.
#define FIS_ALTLINES             21  //(DWORD)FRAMEINFO.dwData - alternating lines. The low-order word of FRAMEINFO.dwData contains the skip interval in lines. The high-order word of FRAMEINFO.dwData contains the fill interval in lines.
#define FIS_ALTLINEBORDER        22  //(BOOL)FRAMEINFO.dwData - draw alternating lines border.
#define FIS_RECTMARGINS          26  //(RECT *)FRAMEINFO.dwData - edit margins. FRAMEINFO.dwData contain left, top, right, bottom margins in pixels.
#define FIS_LINEGAP              30  //(int)FRAMEINFO.dwData - gap between lines.
#define FIS_MOUSEOPTIONS         34  //(DWORD)FRAMEINFO.dwData - mouse options, see MO_* defines.
#define FIS_SHOWURL              35  //(BOOL)FRAMEINFO.dwData - show URL.
#define FIS_CLICKURL             36  //(int)FRAMEINFO.dwData - number of clicks to open URL.
#define FIS_URLPREFIXESENABLE    40  //(BOOL)FRAMEINFO.dwData - URL prefixes enable.
#define FIS_URLPREFIXES          41  //(wchar_t *)FRAMEINFO.dwData - URL prefixes.
#define FIS_URLDELIMITERSENABLE  45  //(BOOL)FRAMEINFO.dwData - URL delimiters enable.
#define FIS_URLLEFTDELIMITERS    46  //(wchar_t *)FRAMEINFO.dwData - URL left delimiters.
#define FIS_URLRIGHTDELIMITERS   50  //(wchar_t *)FRAMEINFO.dwData - URL right delimiters.
#define FIS_WORDDELIMITERSENABLE 54  //(BOOL)FRAMEINFO.dwData - word delimiters enabled.
#define FIS_WORDDELIMITERS       55  //(wchar_t *)FRAMEINFO.dwData - word delimiters.
#define FIS_WRAPDELIMITERSENABLE 59  //(BOOL)FRAMEINFO.dwData - wrap delimiters enabled.
#define FIS_WRAPDELIMITERS       60  //(wchar_t *)FRAMEINFO.dwData - wrap delimiters.
#define FIS_LOCKINHERIT          68  //(DWORD)FRAMEINFO.dwData - lock inherit new document settings from current document. FRAMEINFO.dwData contain lock inherit flags, see LI_* defines.
#define FIS_COLORS               72  //(AECOLORS *)FRAMEINFO.dwData - set colors.
#define FIS_BKIMAGE              73  //(BKIMAGE *)FRAMEINFO.dwData - set background image.

//New line format
#define NEWLINE_WIN   1  //Windows/DOS new line format (\r\n).
#define NEWLINE_UNIX  2  //Unix new line format (\n).
#define NEWLINE_MAC   3  //MacOS new line format (\r).

//AKD_GOTO type
#define GT_LINE        0x1 //Go to "Line:Column".
#define GT_OFFSETBYTE  0x2 //Go to offset counted in bytes.
#define GT_OFFSETCHAR  0x4 //Go to offset counted in characters (not present in "Go to..." dialog).

//Caret options
#define CO_CARETOUTEDGE          0x00000001  //Allow caret moving out of the line edge.
#define CO_CARETVERTLINE         0x00000002  //Draw caret vertical line.
#define CO_CARETACTIVELINE       0x00000004  //Draw active line.
#define CO_CARETACTIVELINEBORDER 0x00000008  //Draw active line border.

//Mouse options
#define MO_LEFTMARGINSELECTION   0x00000001  //Enables left margin line selection with mouse.
#define MO_RICHEDITMOUSE         0x00000002  //After WM_LBUTTONUP message capture operations doesn't stopped.
#define MO_MOUSEDRAGGING         0x00000004  //Enables OLE text dragging.
#define MO_RCLICKMOVECARET       0x00000008  //WM_RBUTTONDOWN message moves caret to a click position.
#define MO_NONEWLINEMOUSESELECT  0x00000010  //Triple click and left margin click selects only line contents without new line.
#define MO_NOWHEELFONTCHANGE     0x00000020  //Don't change font size with middle button scroll and Ctrl key.
#define MO_MARGINSELUNWRAPLINE   0x00000040  //Left margin line selection with mouse selects all wrapped line.
#define MO_MBUTTONDOWNNOSCROLL   0x00000080  //No scrolling after WM_MBUTTONDOWN message.

//Keyboard layout options
#define KLO_REMEMBERLAYOUT     0x00000001  //Remember keyboard layout for each tab (MDI).
#define KLO_SWITCHLAYOUT       0x00000002  //Auto switch keyboard layout according to text under caret.

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

//Edit styles
#define EDS_GLOBALUNDO              0x00000001  //Use process heap for Undo/Redo instead of window heap. Required for AEM_DETACHUNDO and AEM_ATTACHUNDO.
                                                //Compatibility: define same as ES_SUNKEN.
#define EDS_HEAPSERIALIZE           0x00000002  //Mutual exclusion will be used when the heap functions allocate and free memory from window heap. Serialization of heap access allows two or more threads to simultaneously allocate and free memory from the same heap.
                                                //Compatibility: define same as ES_SAVESEL.

//Status bar position type
#define SPT_COLUMN      0x00000001  //"Line:Column". By default: "Line:Symbol".
#define SPT_LINEWRAP    0x00000002  //Wrap line numbers. By default: Non-wrap line numbers.

//Show modify
#define SM_NONE           0x00000000
#define SM_STATUSBAR      0x00000001
#define SM_MAINTITLE_SDI  0x00000002
#define SM_FRAMETITLE_MDI 0x00000004
#define SM_TABTITLE_MDI   0x00000008

//StatusBar parts
#define SBP_POSITION   0
#define SBP_MODIFY     1
#define SBP_INSERT     2
#define SBP_NEWLINE    3
#define SBP_CODEPAGE   4
#define SBP_USER       5

//"Don't open a program twice" flags
#define SOP_ON       0x00000001  //"Don't open a program twice" is on.
#define SOP_SAMEEXE  0x00000002  //"Don't open a program twice" only if AkelPad executable is the same.

//Tab options MDI
#define TAB_VIEW_NONE           0x00000001  //Hide tab bar.
#define TAB_VIEW_TOP            0x00000002  //Show tab bar on top.
#define TAB_VIEW_BOTTOM         0x00000004  //Show tab bar at the bottom.
#define TAB_TYPE_STANDARD       0x00000100  //Standard tab bar style.
#define TAB_TYPE_BUTTONS        0x00000200  //Buttons tab bar style.
#define TAB_TYPE_FLATBUTTONS    0x00000400  //Flat buttons tab bar style.
#define TAB_SWITCH_NEXTPREV     0x00010000  //Switch between tabs: Next-Previous.
#define TAB_SWITCH_RIGHTLEFT    0x00020000  //Switch between tabs: Left-Right.
#define TAB_ADD_AFTERCURRENT    0x00080000  //Create tabs after the current one.
#define TAB_NOADD_LBUTTONDBLCLK 0x00100000  //Don't create new tab by left button double click on the tab bar.
#define TAB_NOADD_MBUTTONDOWN   0x00200000  //Don't create new tab by middle button click on the tab bar.
#define TAB_NODEL_LBUTTONDBLCLK 0x00400000  //Don't close tab by left button double click on the tab.
#define TAB_NODEL_MBUTTONDOWN   0x00800000  //Don't close tab by middle button click on the tab.

//File types association
#define FTA_ASSOCIATE     0x00000001  //Internal.
#define FTA_DEASSOCIATE   0x00000002  //Internal.
#define FTA_OPEN          0x00000004  //Open file types.
#define FTA_EDIT          0x00000008  //Edit file types.
#define FTA_PRINT         0x00000010  //Print file types.

//Color printing
#define PRNC_TEXT         0x01  //Print colored text.
#define PRNC_BACKGROUND   0x02  //Print on colored background.
#define PRNC_SELECTION    0x04  //Print text selection.

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
#define PDS_GETSUPPORT   0x10000000  //Flag is set if caller wants to get PDS_* flags without function execution.
#define PDS_STRANSI      0x20000000  //Flag is set if caller passes Ansi strings in external call arguments (PLUGINDATA.lParam).
#define PDS_STRWIDE      0x40000000  //Flag is set if caller passes Unicode strings in external call arguments (PLUGINDATA.lParam).
                                     //If PDS_STRANSI and PDS_STRWIDE not specified then one of these flags will be set automatically depending on Windows version.

//AKD_DLLCALL flags
#define DLLCF_ONPROGRAMLOAD   0x001  //Don't use it. For internal code only.
#define DLLCF_SWITCHAUTOLOAD  0x004  //If function running after call then turn on autoload, if not then turn off autoload.
#define DLLCF_SAVENOW         0x008  //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_NOW after switching autoload flag.
#define DLLCF_SAVEONEXIT      0x010  //Using with DLLCF_SWITCHAUTOLOAD. Call AKD_DLLSAVE with DLLSF_ONEXIT after switching autoload flag.

//AKD_DLLSAVE flags
#define DLLSF_NOW     0x1  //Save plugins stack immediately.
#define DLLSF_ONEXIT  0x2  //Save plugins stack on program exit.

//AKD_TRANSLATEMESSAGE types
#define TMSG_GLOBAL       0x01  //Translate hotkey from global accelerator table (PLUGINDATA.hGlobalAccel).
#define TMSG_DIALOG       0x02  //Translate message from modeless (see AKD_SETMODELESS) or dockable dialog (see AKD_DOCK).
#define TMSG_PLUGIN       0x04  //Translate plugin message (see AKD_DLL*, AKD_CALLPROC, AKD_POSTMESSAGE).
#define TMSG_HOTKEY       0x08  //Translate plugin hotkey.
#define TMSG_ACCELERATOR  0x10  //Translate hotkey from main accelerator table (PLUGINDATA.hMainAccel).
#define TMSG_DEFAULT      0x20  //Default message processing.

#define TMSG_ALL  (TMSG_GLOBAL      |\
                   TMSG_DIALOG      |\
                   TMSG_PLUGIN      |\
                   TMSG_HOTKEY      |\
                   TMSG_ACCELERATOR |\
                   TMSG_DEFAULT)

//Context menu owner
#define NCM_EDIT     1  //Edit control.
#define NCM_TAB      2  //Tab control.
#define NCM_STATUS   3  //Status bar control.

//AKD_FRAMEFIND flags
#define FWF_CURRENT        1  //Retrieve current frame data pointer. lParam not used.
#define FWF_NEXT           2  //Retrieve next frame data pointer in frame stack. lParam is a frame data pointer.
#define FWF_PREV           3  //Retrieve previous frame data pointer in frame stack. lParam is a frame data pointer.
#define FWF_BYINDEX        4  //Retrieve frame data by index in frame stack. lParam is frame index. First frame has index 1, last -1.
#define FWF_BYFILENAME     5  //Retrieve frame data by full file name. lParam is full file name string.
                              // For AKD_FRAMEFINDA string is ansi.
                              // For AKD_FRAMEFINDW string is unicode.
#define FWF_BYEDITWINDOW   6  //Retrieve frame data by edit window handle. lParam is edit window handle or NULL for current edit window handle.
#define FWF_BYEDITDOCUMENT 7  //Retrieve frame data by edit document handle. lParam is edit document handle or NULL for current edit document handle.
#define FWF_BYTABINDEX     8  //Retrieve frame data by tab item index. lParam is tab item index.
#define FWF_TABNEXT        9  //Retrieve next tab item frame data. lParam is a frame data pointer.
#define FWF_TABPREV        10 //Retrieve previous tab item frame data. lParam is a frame data pointer.

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
#define LI_BKIMAGE        0x00000004  //Lock inherit background image.
#define LI_WRAP           0x00000008  //Lock inherit wrapping.

//Find/Replace flags
#define FRF_DOWN               0x00000001  //Same as AEFR_DOWN.
#define FRF_WHOLEWORD          0x00000002  //Same as AEFR_WHOLEWORD.
#define FRF_MATCHCASE          0x00000004  //Same as AEFR_MATCHCASE.
#define FRF_WHOLEWORDGOODSTART 0x00000010  //Internal.
#define FRF_WHOLEWORDGOODEND   0x00000020  //Internal.
#define FRF_FINDFROMREPLACE    0x00000040  //Internal.
#define FRF_REGEXPNONEWLINEDOT 0x00040000  //Symbol . specifies any character except new line. Uses with FRF_REGEXP.
#define FRF_REGEXP             0x00080000  //Same as AEFR_REGEXP.
#define FRF_UP                 0x00100000
#define FRF_BEGINNING          0x00200000
#define FRF_SELECTION          0x00400000
#define FRF_ESCAPESEQ          0x00800000
#define FRF_ALLFILES           0x01000000
#define FRF_REPLACEALLANDCLOSE 0x02000000
#define FRF_CHECKINSELIFSEL    0x04000000
#define FRF_CYCLESEARCH        0x08000000
#define FRF_CYCLESEARCHPROMPT  0x10000000
#define FRF_REPLACEALLNOMSG    0x20000000

//AKD_PASTE
#define PASTE_ANSI       0x00000001  //Paste text as ANSI. Default is paste as Unicode text, if no Unicode text available ANSI text will be used.
#define PASTE_COLUMN     0x00000002  //Paste to column selection.
#define PASTE_AFTER      0x00001000  //Paste text after caret.
#define PASTE_SINGLELINE 0x00002000  //Paste multiline text to single line edit control. All new lines replaced with '\r'.

//AKD_RECODESEL flags
#define RCS_DETECTONLY   0x00000001  //Don't do text replacement, only detect codepages.

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

//BUTTONMESSAGEBOX flags
#define BMB_DEFAULT   0x001  //Default button.
#define BMB_DISABLED  0x002  //Button is disabled.

//WM_INITMENU lParam
#define IMENU_EDIT     0x00000001
#define IMENU_CHECKS   0x00000004
$$$ DEFINES1 END $$$

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
    BYTE nLineBreak;
    BYTE nLineFlags;
    WORD nReserved;
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
    COLORREF crUrlCursorText;
    COLORREF crUrlVisitText;
    COLORREF crActiveLineBorder;
    COLORREF crAltLineText;
    COLORREF crAltLineBk;
    COLORREF crAltLineBorder;
  } AECOLORS;

  DECLARE_HANDLE (AEHDOC);

  typedef LRESULT (CALLBACK *AEEditProc)(AEHDOC hDoc, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

DECLARE_HANDLE (HINIFILE);
DECLARE_HANDLE (HINISECTION);
DECLARE_HANDLE (HINIKEY);

typedef void (CALLBACK *WNDPROCRET)(CWPRETSTRUCT *);
typedef void (CALLBACK *CALLPROC)(void *);
typedef BOOL (CALLBACK *PLUGINPROC)(void *lpParameter, LPARAM lParam, DWORD dwSupport);
//lpParameter  Procedure parameter. Specified in AKD_DLLADD message (PLUGINADD.lpParameter).
//lParam       Input data. Specified in AKD_DLLCALL message.
//dwSupport    See PDS_* defines. Specified in AKD_DLLCALL message.
//
//Return Value
// TRUE  catch hotkey.
// FALSE do default hotkey processing.

typedef struct {
  HWND hWndEdit;           //Edit window.
  AEHDOC hDocEdit;         //Edit document.
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
  HWND hWndMaster;         //Master window.
  AEHDOC hDocMaster;       //Master document.
  HWND hWndClone1;         //First clone window.
  AEHDOC hDocClone1;       //First clone document.
  HWND hWndClone2;         //Second clone window.
  AEHDOC hDocClone2;       //Second clone document.
  HWND hWndClone3;         //Third clone window.
  AEHDOC hDocClone3;       //Third clone document.
} EDITINFO;

typedef struct _RECENTCARETITEM {
  struct _RECENTCARETITEM *next;
  struct _RECENTCARETITEM *prev;
  INT_PTR nCaretOffset;
} RECENTCARETITEM;

typedef struct {
  RECENTCARETITEM *first;
  RECENTCARETITEM *last;
} STACKRECENTCARET;

typedef struct _FRAMEDATA {
  struct _FRAMEDATA *next;
  struct _FRAMEDATA *prev;

  //Edit state external
  HWND hWndEditParent;                                //Edit parent window.
  EDITINFO ei;                                        //Edit info.
  char szFile[MAX_PATH];                              //Frame file (Ansi).
  wchar_t wszFile[MAX_PATH];                          //Frame file (Unicode).
  int nFileLen;                                       //Frame file length.
  int nStreamOffset;                                  //":" symbol offset in FRAMEDATA.wszFile.
  HICON hIcon;                                        //Frame icon.
  int nIconIndex;                                     //Frame ImageList icon index.
  RECT rcEditWindow;                                  //Edit RECT. rcEditWindow.right - is width and rcEditWindow.bottom is height.
  RECT rcMasterWindow;                                //Master window RECT. rcMasterWindow.right - is width and rcMasterWindow.bottom is height.

  //Edit settings (AkelPad)
  DWORD dwLockInherit;                                //See LI_* defines.
  LOGFONTW lf;                                        //Edit font.
  BOOL bTabStopAsSpaces;                              //Insert tab stop as spaces.
  DWORD dwCaretOptions;                               //See CO_* defines.
  DWORD dwMouseOptions;                               //See MO_* defines.
  int nClickURL;                                      //Number of clicks to open URL.
  BOOL bUrlPrefixesEnable;                            //URL prefixes enable.
  BOOL bUrlDelimitersEnable;                          //URL delimiters enable.
  BOOL bWordDelimitersEnable;                         //Word delimiters enabled.
  BOOL bWrapDelimitersEnable;                         //Wrap delimiters enabled.
  DWORD dwMappedPrintWidth;                           //Mapped print page width.

  //Edit settings (AkelEdit)
  RECT rcEditMargins;                                 //Edit margins.
  int nTabStopSize;                                   //Tab stop size.
  int nUndoLimit;                                     //Undo limit.
  BOOL bDetailedUndo;                                 //Detailed undo.
  DWORD dwWrapType;                                   //Wrap type AEWW_WORD or AEWW_SYMBOL.
  DWORD dwWrapLimit;                                  //Wrap characters limit, zero if wrap by window edge.
  DWORD dwMarker;                                     //Vertical marker, zero if no marker set.
  int nCaretWidth;                                    //Caret width.
  DWORD dwAltLineFill;                                //Alternating lines fill interval.
  DWORD dwAltLineSkip;                                //Alternating lines skip interval.
  BOOL bAltLineBorder;                                //Draw alternating lines border.
  DWORD dwLineGap;                                    //Line gap.
  BOOL bShowURL;                                      //Show URL.
  wchar_t wszUrlPrefixes[URL_PREFIXES_SIZE];          //URL prefixes.
  wchar_t wszUrlLeftDelimiters[URL_DELIMITERS_SIZE];  //URL left delimiters.
  wchar_t wszUrlRightDelimiters[URL_DELIMITERS_SIZE]; //URL right delimiters.
  wchar_t wszWordDelimiters[WORD_DELIMITERS_SIZE];    //Word delimiters.
  wchar_t wszWrapDelimiters[WRAP_DELIMITERS_SIZE];    //Wrap delimiters.
  wchar_t wszBkImageFile[MAX_PATH];                   //Background image file.
  int nBkImageAlpha;                                  //Alpha transparency value that ranges from 0 to 255.
  HBITMAP hBkImageBitmap;                             //Background image handle.
  AECOLORS aec;                                       //Edit colors.

  //Edit state internal. AKD_FRAMEINIT not copy data below.
  AEEditProc lpEditProc;                              //Edit window procedure.
  FILETIME ft;                                        //File time.
  HKL dwInputLocale;                                  //Keyboard layout.
  STACKRECENTCARET hRecentCaretStack;                 //Recent caret stack.
  RECENTCARETITEM *lpCurRecentCaret;                  //Current recent caret position.

  //Find/Replace
  INT_PTR nCompileErrorOffset;                        //Contain pattern offset, if error occurred during compile pattern.
  BOOL bCompileErrorReplace;                          //TRUE - error in "ReplaceWith" complitaion, FALSE - error in "FindIt" complitaion.

  //Statusbar
  AECHARRANGE crPrevSel;
  INT_PTR nSelSubtract;
  INT_PTR nCaretRichOffset;
  INT_PTR nCaretByteOffset;
  int nCaretChar;
  int nCaretLine;
  int nCaretColumn;
  int nLineCountAll;
  int nLineCountSel;
  int nLineSelBegin;
  int nLineSelEnd;
  INT_PTR nRichCount;
  int nFontPoint;
  BOOL bCapsLock;
  BOOL bNumLock;
  BOOL bReachedEOF;
  INT_PTR nReplaceCount;
} FRAMEDATA;

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
  int nRefCount;                  //Internal.
} PLUGINFUNCTION;

typedef struct {
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
  int nSaveSettings;                //See SS_* defines.
  HWND hMainWnd;                    //Main window.
  FRAMEDATA *lpFrameData;           //Pointer to a current FRAMEDATA structure.
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
  DWORD dwLangID;         //Codepage recognition language defined as LANGID. If -1, then use current settings.
  const char *pText;      //Ansi text.
  INT_PTR nTextLen;       //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  INT_PTR nMinChars;      //Minimum detect characters representation. If zero, default number is used.
  int nCodePage;          //Result: detected Ansi codepage.
} DETECTANSITEXT;

typedef struct {
  DWORD dwLangID;         //Codepage recognition language defined as LANGID. If -1, then use current settings.
  const wchar_t *wpText;  //Unicode text.
  INT_PTR nTextLen;       //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  INT_PTR nMinChars;      //Minimum detect characters representation. If zero, default number is used.
  int nCodePageFrom;      //Result: codepage that converts text to Ansi without character lost.
  int nCodePageTo;        //Result: detected Ansi codepage.
} DETECTUNITEXT;

typedef struct {
  const char *pInput;     //Ansi text.
  INT_PTR nInputLen;      //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nCodePageFrom;      //Codepage for Ansi to Unicode conversion.
  int nCodePageTo;        //Codepage for Unicode to Ansi conversion.
  char *szOutput;         //Result: pointer that receive allocated text. Must be deallocated with AKD_FREETEXT message.
  INT_PTR nOutputLen;     //Result: text length.
} CONVERTANSITEXT;

typedef struct {
  const wchar_t *wpInput; //Unicode text.
  INT_PTR nInputLen;      //Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nCodePageFrom;      //Codepage for Unicode to Ansi conversion.
  int nCodePageTo;        //Codepage for Ansi to Unicode conversion.
  wchar_t *wszOutput;     //Result: pointer that receive allocated text. Must be deallocated with AKD_FREETEXT message.
  INT_PTR nOutputLen;     //Result: text length.
} CONVERTUNITEXT;

typedef struct {
  const char *pFile;     //File to detect.
  DWORD dwBytesToCheck;  //How many bytes will be checked.
  DWORD dwFlags;         //See ADT_* defines.
  int nCodePage;         //Detected codepage.
  BOOL bBOM;             //Detected BOM.
} DETECTFILEA;

typedef struct {
  const wchar_t *pFile;  //File to detect.
  DWORD dwBytesToCheck;  //How many bytes will be checked.
  DWORD dwFlags;         //See ADT_* defines.
  int nCodePage;         //Detected codepage.
  BOOL bBOM;             //Detected BOM.
} DETECTFILEW;

typedef struct {
  HANDLE hFile;          //File handle, returned by CreateFile function.
  UINT_PTR dwMax;        //AKD_READFILECONTENT: maximum bytes to read, if -1 read entire file.
                         //AKD_WRITEFILECONTENT: wpContent length in characters. If this value is -1, the wpContent is assumed to be null-terminated and the length is calculated automatically.
  int nCodePage;         //File codepage.
  BOOL bBOM;             //File BOM.
  wchar_t *wpContent;    //AKD_READFILECONTENT: returned file contents.
                         //AKD_WRITEFILECONTENT: text to save.
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


//AKD_SETFRAMEINFO
typedef struct {
  int nType;        //See FIS_* defines.
  UINT_PTR dwData;  //Depend on FIS_* define.
} FRAMEINFO;

typedef struct {
  const wchar_t *wpFile; //Background image file.
  int nAlpha;            //Alpha transparency value that ranges from 0 to 255.
} BKIMAGE;

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
  DWORD dwSupport;            //See PDS_* defines.
} PLUGINCALLSENDA;

typedef struct {
  const wchar_t *pFunction;   //Function name, format L"Plugin::Function".
  LPARAM lParam;              //Input data.
  DWORD dwSupport;            //See PDS_* defines.
} PLUGINCALLSENDW;

typedef struct {
  LPARAM lParam;                //Input data.
  char szFunction[MAX_PATH];    //Function name, format "Plugin::Function".
  DWORD dwSupport;              //See PDS_* defines.
} PLUGINCALLPOSTA;

typedef struct {
  LPARAM lParam;                //Input data.
  wchar_t szFunction[MAX_PATH]; //Function name, format L"Plugin::Function".
  DWORD dwSupport;              //See PDS_* defines.
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
  HANDLE hIniFile;
  wchar_t *wszSection;
  int nSectionBytes;
  INIKEY *first;
  INIKEY *last;
} INISECTION;

typedef struct {
  INISECTION *first;
  INISECTION *last;
  BOOL bModified;
} INIFILE;

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
  DWORD dwFlags;            //See FRF_* defines.
  const char *pFindIt;      //Find string.
  int nFindItLen;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
} TEXTFINDA;

typedef struct {
  DWORD dwFlags;            //See FRF_* defines.
  const wchar_t *pFindIt;   //Find string.
  int nFindItLen;           //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
} TEXTFINDW;

typedef struct {
  DWORD dwFlags;               //See FRF_* defines.
  const char *pFindIt;         //Find string.
  int nFindItLen;              //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  const char *pReplaceWith;    //Replace string.
  int nReplaceWithLen;         //Replace string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  BOOL bAll;                   //Replace all.
  INT_PTR nChanges;            //Count of changes.
} TEXTREPLACEA;

typedef struct {
  DWORD dwFlags;               //See FRF_* defines.
  const wchar_t *pFindIt;      //Find string.
  int nFindItLen;              //Find string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  const wchar_t *pReplaceWith; //Replace string.
  int nReplaceWithLen;         //Replace string length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
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
  int nButtonControlID;       //ID of the button that returned in result
  const wchar_t *wpButtonStr; //Pointer to Unicode text or ID of the string resource in current language module.
  DWORD dwFlags;              //See BMB_* defines.
} BUTTONMESSAGEBOX;

typedef struct {
  HWND hWndParent;            //Handle to the owner window.
  const wchar_t *wpText;      //Pointer to a null-terminated string that contains the message to be displayed.
  const wchar_t *wpCaption;   //Pointer to a null-terminated string that contains the dialog box title.
  UINT uType;                 //Specifies the standard message box icon. See MSDN for MB_ICON* defines of the MessageBox function.
  HICON hIcon;                //Custom icon.
  BUTTONMESSAGEBOX *bmb;      //Array of the BUTTONMESSAGEBOX structures. Each element specified one message box button. Last item in the array should contain all zeros in members.
} DIALOGMESSAGEBOX;

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
  BOOL bQuitAsEnd;                     //Internal variable - "/quit" stops parsing command line parameters, but not closes program.
} PARSECMDLINEPOSTW;

typedef struct {
  HGLOBAL hDevMode;
  HGLOBAL hDevNames;

  //PRINTDLG
  HDC hDC;
  DWORD dwPrintFlags;
  WORD nFromPage;
  WORD nToPage;
  WORD nCopies;

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
  DWORD *dwFlags;         //Pointer to a open flags variable. See OD_* defines.
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
  BOOL bMouse;            //Context menu is requested with mouse.
  BOOL bProcess;          //TRUE   show context menu.
                          //FALSE  do not show context menu.
} NCONTEXTMENU;

typedef struct {
  RECT rcInitial;         //Initial client RECT (read-only).
  RECT rcCurrent;         //Current client RECT (writeable).
} NSIZE;


$$$ DEFINES2 $$$
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
                                              //Return Value: TRUE - "OK" pressed, FALSE - "Cancel" pressed.
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
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_INSERT_TAB_MENU        4165  //Insert tabulation only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_DELETE_TAB             4166  //Delete tabulation.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_DELETE_TAB_MENU        4167  //Delete tabulation only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_INSERT_SPACE           4168  //Insert space.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_INSERT_SPACE_MENU      4169  //Insert space only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_DELETE_SPACE           4170  //Delete space.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_DELETE_SPACE_MENU      4171  //Delete space only if several lines selected.
                                              //Return Value: TRUE - success, FALSE - failed..
                                              //
#define IDM_EDIT_DELETE_FIRST_CHAR      4172  //Delete first char.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_DELETE_FIRST_CHAR_MENU 4173  //Delete first char only if selection not empty.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_DELETE_TRAILING_WHITESPACES 4174  //Delete trailing whitespaces.
                                                   //Return Value: TRUE - success, FALSE - failed.
                                                   //
#define IDM_EDIT_UPPERCASE              4175  //Convert text to UPPERCASE. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_LOWERCASE              4176  //Convert text to lowercase. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_SENTENCECASE           4177  //Convert text to Sentence case. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_TITLECASE              4178  //Convert text to Title Case. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_INVERTCASE             4179  //Convert text to iNVERT cASE. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_LOOPCASE               4180  //Cycle case. lParam: TRUE - change case for current word if no selection, FALSE - change case for all text if no selection (default).
                                              //Return Value: TRUE - success, FALSE - failed.
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
#define IDM_EDIT_DELETESELWHITESPACES   4195  //Delete all whitespaces in selection.
                                              //Return Value: TRUE - success, FALSE - failed.
                                              //
#define IDM_EDIT_AUTOINDENT             4196  //Insert new line with keeping left spaces.
                                              //Return Value: TRUE - inserted with spaces, FALSE - inserted without spaces.
                                              //
#define IDM_EDIT_DELLINE                4197  //Delete current line. lParam: TRUE - delete all wrapped line, FALSE - normal behavior.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_SELJUMPCARET           4198  //Move caret on the contrary side of selection.
                                              //Return Value: TRUE - jump to selection beginning, FALSE - jump to selection ending.
                                              //
#define IDM_EDIT_RECENTCARETPREV        4199  //Move caret to the previous position.
                                              //Return Value: zero.
                                              //
#define IDM_EDIT_RECENTCARETNEXT        4200  //Move caret to the next position.
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
#define IDM_VIEW_SPLIT_WINDOW_ALL       4212  //Split window into four panes. Turn off, if window already split into four panes.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SPLIT_WINDOW_WE        4213  //Split window into two vertical panes. Turn off, if window already split into two vertical panes.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SPLIT_WINDOW_NS        4214  //Split window into two horizontal panes. Turn off, if window already split into two horizontal panes.
                                              //Return Value: zero.
                                              //
#define IDM_VIEW_SPLIT_WINDOW_ONOFF     4215  //Split window into four panes. Turn off, if window already split.
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
                                              //Return Value: SW_RESTORE - new style is SW_RESTORE, SW_MAXIMIZE - new style is SW_MAXIMIZE.
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
#define IDM_UPDATE                      4353  //Open AkelUpdater.
                                              //Return Value: zero.
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
#define AKDN_MAIN_ONDESTROY        (WM_USER + 7)   //0x407

#define AKDN_EDIT_ONSTART          (WM_USER + 11)  //0x40B
#define AKDN_EDIT_ONFINISH         (WM_USER + 12)  //0x40C
#define AKDN_EDIT_ONCLOSE          (WM_USER + 13)  //0x40D

#define AKDN_FRAME_NOWINDOWS       (WM_USER + 21)  //0x415
#define AKDN_FRAME_ACTIVATE        (WM_USER + 22)  //0x416
#define AKDN_FRAME_DEACTIVATE      (WM_USER + 23)  //0x417
#define AKDN_FRAME_DESTROY         (WM_USER + 24)  //0x418

#define AKDN_DOCK_GETMINMAXINFO    (WM_USER + 31)  //0x41F
#define AKDN_DOCK_CAPTURE_ONSTART  (WM_USER + 32)  //0x420
#define AKDN_DOCK_CAPTURE_ONFINISH (WM_USER + 33)  //0x421
#define AKDN_DOCK_RESIZE           (WM_USER + 34)  //0x422

#define AKDN_DLLCALL               (WM_USER + 41)  //0x429
#define AKDN_DLLUNLOAD             (WM_USER + 42)  //0x42A

#define AKDN_ACTIVATE              (WM_USER + 50)  //0x432
#define AKDN_SIZE_ONSTART          (WM_USER + 51)  //0x433
#define AKDN_SIZE_ONFINISH         (WM_USER + 52)  //0x434
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
#define AKD_WRITEFILECONTENT       (WM_USER + 141)
#define AKD_DETECTANSITEXT         (WM_USER + 146)
#define AKD_DETECTUNITEXT          (WM_USER + 147)
#define AKD_CONVERTANSITEXT        (WM_USER + 148)
#define AKD_CONVERTUNITEXT         (WM_USER + 149)
#define AKD_DETECTFILE             (WM_USER + 151)
#define AKD_DETECTFILEA            (WM_USER + 152)
#define AKD_DETECTFILEW            (WM_USER + 153)
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
#define AKD_GOTO                   (WM_USER + 180)
#define AKD_GOTOA                  (WM_USER + 181)
#define AKD_GOTOW                  (WM_USER + 182)

//Print
#define AKD_GETPRINTINFO           (WM_USER + 191)
#define AKD_SETPRINTINFO           (WM_USER + 192)

//Options
#define AKD_SETMAININFO            (WM_USER + 195)
#define AKD_SETFRAMEINFO           (WM_USER + 196)
#define AKD_GETMAININFO            (WM_USER + 198)
#define AKD_GETFRAMEINFO           (WM_USER + 199)
#define AKD_GETEDITINFO            (WM_USER + 200)
#define AKD_PROGRAMVERSION         (WM_USER + 201)
#define AKD_PROGRAMARCHITECTURE    (WM_USER + 202)
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
#define AKD_SETEDITNOTIFY          (WM_USER + 216)

//Windows
#define AKD_GETMODELESS            (WM_USER + 251)
#define AKD_SETMODELESS            (WM_USER + 252)
#define AKD_RESIZE                 (WM_USER + 253)
#define AKD_DOCK                   (WM_USER + 254)
#define AKD_SETBUTTONDRAW          (WM_USER + 255)
#define AKD_SETHOTKEYINPUT         (WM_USER + 256)
#define AKD_DIALOGRESIZE           (WM_USER + 257)
#define AKD_UPDATESTATUSUSER       (WM_USER + 258)

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
#define AKD_FRAMEINDEX             (WM_USER + 270)
#define AKD_FRAMEINIT              (WM_USER + 271)
#define AKD_FRAMEAPPLYEDIT         (WM_USER + 272)

//Process and Threads
#define AKD_MEMCREATE              (WM_USER + 281)
#define AKD_MEMMAP                 (WM_USER + 282)
#define AKD_MEMUNMAP               (WM_USER + 283)
#define AKD_MEMCLOSE               (WM_USER + 284)
#define AKD_STRLENA                (WM_USER + 285)
#define AKD_STRLENW                (WM_USER + 286)
#define AKD_CREATEWINDOW           (WM_USER + 287)
#define AKD_WAITKEYBOARD           (WM_USER + 288)
#define AKD_GETQUEUE               (WM_USER + 289)
#define AKD_POSTMESSAGE            (WM_USER + 290)
#define AKD_TRANSLATEMESSAGE       (WM_USER + 291)
#define AKD_MESSAGEBOX             (WM_USER + 292)
#define AKD_GETFOCUS               (WM_USER + 293)
#define AKD_PEEKMESSAGE            (WM_USER + 294)
#define AKD_UNIQUEID               (WM_USER + 295)

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
#define AKD_CHECKHOTKEY            (WM_USER + 314)

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

//Regular expressions. Requires include of "RegExpFunc.h".
#define AKD_PATEXEC                (WM_USER + 391)
#define AKD_PATREPLACE             (WM_USER + 392)
#define AKD_PATGROUPSTR            (WM_USER + 393)
#define AKD_PATGETGROUP            (WM_USER + 395)
#define AKD_PATNEXTGROUP           (WM_USER + 396)
#define AKD_PATPREVGROUP           (WM_USER + 397)
#define AKD_PATFREE                (WM_USER + 399)

//AkelPad 4.x messages
#define AKD_EXGETTEXTLENGTH        (WM_USER + 401)
#define AKD_EXGETTEXTRANGE         (WM_USER + 402)
#define AKD_EXGETTEXTRANGEA        (WM_USER + 403)
#define AKD_EXGETTEXTRANGEW        (WM_USER + 404)
$$$ DEFINES2 END $$$


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
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure, that has been activated.

Return Value
 Zero.


AKDN_FRAME_DEACTIVATE
_____________________

Notification message, sends to the main procedure before deactivating frame.

(DWORD)wParam       == see FWA_* defines.
(FRAMEDATA *)lParam == pointer to a current FRAMEDATA structure, that will be deactivated.

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


AKDN_SIZE_ONSTART
_________________

Notification message, sends to the main procedure before the main window client RECT changed.

wParam           == not used.
(NSIZE *)lParam  == pointer to a NSIZE structure.

Return Value
 Zero.


AKDN_SIZE_ONFINISH
___________________

Notification message, sends to the main procedure after the main window client RECT changed.

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

Parse command line string.

wParam                      == not used.
(PARSECMDLINESENDW *)lpData == pointer to a PARSECMDLINESENDW structure.

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
(DETECTANSITEXT *)lParam == pointer to a DETECTANSITEXT structure.

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
(DETECTUNITEXT *)lParam == pointer to a DETECTUNITEXT structure.

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
(CONVERTANSITEXT *)lParam == pointer to a CONVERTANSITEXT structure.

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
(CONVERTUNITEXT *)lParam == pointer to a CONVERTUNITEXT structure.

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
(DETECTFILE *)lParam == pointer to a DETECTFILE structure.

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
       fc.dwMax=(UINT_PTR)-1;
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
       fc.dwMax=(UINT_PTR)-1;
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


AKD_WRITEFILECONTENT
____________________

Write file contents.

lParam                == not used.
(FILECONTENT *)lParam == pointer to a FILECONTENT structure.

Return Value
 See ESD_* defines.

Example (bOldWindows == TRUE):
 int WriteFileContentA(const char *pFile, wchar_t *wpContent, INT_PTR nContentLen, int nCodePage, BOOL bBOM)
 {
   FILECONTENT fc;
   int nResult=0;

   if ((fc.hFile=CreateFileA(pFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
   {
     fc.wpContent=wpContent;
     fc.dwMax=nContentLen;
     fc.nCodePage=nCodePage;
     fc.bBOM=bBOM;
     nResult=SendMessage(hMainWnd, AKD_WRITEFILECONTENT, 0, (LPARAM)&fc);
     CloseHandle(fc.hFile);
   }
   return nResult;
 }
 //Call WriteFileContentA function
 WriteFileContentA("C:\\MyFile.txt", L"123", -1, 65001, TRUE);

Example (bOldWindows == FALSE):
 int WriteFileContentW(const wchar_t *wpFile, wchar_t *wpContent, INT_PTR nContentLen, int nCodePage, BOOL bBOM)
 {
   FILECONTENT fc;
   int nResult=0;

   if ((fc.hFile=CreateFileW(wpFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
   {
     fc.wpContent=wpContent;
     fc.dwMax=nContentLen;
     fc.nCodePage=nCodePage;
     fc.bBOM=bBOM;
     nResult=SendMessage(hMainWnd, AKD_WRITEFILECONTENT, 0, (LPARAM)&fc);
     CloseHandle(fc.hFile);
   }
   return nResult;
 }
 //Call WriteFileContentW function
 WriteFileContentW(L"C:\\MyFile.txt", L"123", -1, 65001, TRUE);


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

Free text buffer allocated with some AKD_* messages.

wParam                  == not used.
(unsigned char *)lParam == buffer pointer.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
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
 Character position of the next match.
 If there are no more matches, the return value is –1.
 If there is syntax error occurred with FRF_REGEXP or FRF_ESCAPESEQ flag, the return value is (–100 - PatternOffset).
 For example, TEXTFINDW.pFindIt equal to "ab[c" with FRF_REGEXP, syntax error in third symbol, return value is –102.

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
(TEXTREPLACE *)lParam == pointer to a TEXTREPLACE structure.

Return Value
 Character position of the next match.
 If there are no more matches, the return value is –1.
 If there is syntax error occurred with FRF_REGEXP or FRF_ESCAPESEQ flag, the return value is (–100 - PatternOffset).
 For example, TEXTREPLACEW.pFindIt equal to "ab[c" with FRF_REGEXP, syntax error in third symbol, return value is –102.

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
(TEXTRECODE *)lParam == pointer to a TEXTRECODE structure.

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
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure, NULL for current frame data.

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

(FRAMEINFO *)wParam == pointer to a FRAMEINFO structure.
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure, NULL for current frame data.

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
(EDITINFO *)lParam == pointer to a EDITINFO structure. Can be NULL.

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
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure.

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
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure.

Return Value
 Frame index. If frame not found then index will be 0 (wParam == TRUE) or -1 (wParam == FALSE).

Example:
 SendMessage(pd->hMainWnd, AKD_FRAMEINDEX, FALSE, (LPARAM)lpFrame);


AKD_FRAMEINIT
_____________

Initialize frame data.

(FRAMEDATA *)wParam == pointer to a source FRAMEDATA structure. If NULL, current frame will be used as source.
(FRAMEDATA *)lParam == pointer to a target FRAMEDATA structure.

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
(FRAMEDATA *)lParam == pointer to a FRAMEDATA structure.

Return Value
 Zero.

Example:
 See AKD_FRAMEINIT example.


AKD_MEMCREATE
_____________

Creates or opens a named memory object.

(const char *)wParam == memory object name (see description for CreateFileMapping in MSDN).
(DWORD)lParam        == number of bytes to allocate. If zero, then open already created memory object.

Return Value
 Handle to the memory object.

Example (get executable file of specified AkelPad window):
 wchar_t szExeFile[MAX_PATH];

 GetAkelPadExe(hWndRemote, pd->hMainWnd, szExeFile, MAX_PATH);

 int GetAkelPadExe(HWND hWndRemote, HWND hWndLocal, wchar_t *szExeFile, int nExeFileMax)
 {
   HANDLE hMemRemote;
   HANDLE hMemLocal;
   wchar_t *wszMemRemote;
   wchar_t *wszMemLocal;
   DWORD dwMemSize=nExeFileMax * sizeof(wchar_t);
   int nResult=0;

   if (hMemRemote=(HANDLE)SendMessage(hWndRemote, AKD_MEMCREATE, (WPARAM)"Global\\AkelPad", dwMemSize))
   {
     if (wszMemRemote=(wchar_t *)SendMessage(hWndRemote, AKD_MEMMAP, (WPARAM)hMemRemote, dwMemSize))
     {
       SendMessage(hWndRemote, AKD_GETMAININFO, MI_AKELEXEW, (WPARAM)wszMemRemote);

       //Read data from other process
       if (hMemLocal=(HANDLE)SendMessage(hWndLocal, AKD_MEMCREATE, (WPARAM)"Global\\AkelPad", 0))
       {
         if (wszMemLocal=(wchar_t *)SendMessage(hWndLocal, AKD_MEMMAP, (WPARAM)hMemLocal, dwMemSize))
         {
           lstrcpynW(szExeFile, wszMemLocal, nExeFileMax);
           nResult=lstrlenW(szExeFile);
           SendMessage(hWndLocal, AKD_MEMUNMAP, (WPARAM)wszMemLocal, 0);
         }
         SendMessage(hWndLocal, AKD_MEMCLOSE, (WPARAM)hMemLocal, 0);
       }
       SendMessage(hWndRemote, AKD_MEMUNMAP, (WPARAM)wszMemRemote, 0);
     }
     SendMessage(hWndRemote, AKD_MEMCLOSE, (WPARAM)hMemRemote, 0);
   }
   return nResult;
 }


AKD_MEMMAP
__________

Maps memory object into the address space of a calling process.

(HANDLE)wParam == handle to the memory object.
(DWORD)lParam  == number of bytes to map.

Return Value
 Pointer to the first byte of the memory block.

Example:
 See AKD_MEMCREATE example.


AKD_MEMUNMAP
____________

Unmaps a mapped memory object from the calling process's address space.

(HANDLE)wParam == pointer to the first byte of the memory block.
lParam         == not used.

Return Value
 TRUE  success.
 FALSE error.

Example:
 See AKD_MEMCREATE example.


AKD_MEMCLOSE
____________

Close the specified memory object.

(HANDLE)wParam == handle to the memory object.
lParam         == not used.

Return Value
 TRUE  success.
 FALSE error.

Example:
 See AKD_MEMCREATE example.


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

(DWORD)wParam == see TMSG_* defines.
(MSG *)lParam == pointer to an MSG structure that contains message information.

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
(DIALOGMESSAGEBOX *)lParam == pointer to an DIALOGMESSAGEBOX structure that contains message box information.

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
(MSG *)lParam == pointer to an MSG structure that contains message information.

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


AKD_UNIQUEID
____________

Retrieve unique identifier inside current process.

wParam == not used.
lParam == not used.

Return Value
 Identifier.

Example:
 INT_PTR nMyID=SendMessage(pd->hMainWnd, AKD_UNIQUEID, 0, 0);


AKD_DLLCALL, AKD_DLLCALLA, AKD_DLLCALLW
___________  ____________  ____________

Call dll.

(DWORD)wParam            == see DLLCF_* defines.
(PLUGINCALLSEND *)lParam == pointer to a PLUGINCALLSEND structure if SendMessage used
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
   if (pf->bRunning) MessageBoxW(pd->hMainWnd, L"Plugin is running", L"Test", MB_OK);

Example find by hotkey:
 PLUGINFUNCTION *pf;
 if (pf=(PLUGINFUNCTION *)SendMessage(pd->hMainWnd, AKD_DLLFIND, (WPARAM)NULL, 3112))
   if (pf->bRunning) MessageBoxW(pd->hMainWnd, L"Plugin is running", L"Test", MB_OK);


AKD_DLLADD, AKD_DLLADDA, AKD_DLLADDW
__________  ___________  ___________

Add dll function.

wParam              == not used.
(PLUGINADD *)lParam == function structure pointer.

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

(CALLPROC)wParam == procedure address.
(void *)lParam   == pointer to a variable to be passed to the procedure.

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
(wchar_t *)lParam == buffer that received assigned owner, format L"Plugin::Function". Can be NULL. If not NULL, buffer size must be at least for MAX_PATH characters.

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
(const unsigned char *)lParam == ini file.

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
(const unsigned char *)lParam == section name.

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
(const unsigned char *)lParam == key name.

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
(INIVALUE *)lParam == pointer to a INIVALUE structure.

Return Value
 Size of the data copied to the buffer.

Example:
 See AKD_INIOPEN examples.


AKD_INISETVALUE, AKD_INISETVALUEA, AKD_INISETVALUEW
_______________  ________________  ________________

Set ini value.

(HINIFILE)wParam   == ini file handle.
(INIVALUE *)lParam == pointer to a INIVALUE structure.

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

Compile and execute regular expressions pattern. Requires include of "RegExpFunc.h".

wParam            == not used.
(PATEXEC *)lParam == pointer to a PATEXEC structure.

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
 pe.wpText=pe.wpStr;
 pe.wpMaxText=pe.wpMaxStr;
 pe.wpPat=L"(23)(.*)(89)";
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
     if (lpREGroupNext->nStrLen && lpREGroupNext->nIndex != -1)
     {
       //wpResult+=xprintfW(wpResult, L"%d [%.%ds]\n", lpREGroupNext->nIndex, lpREGroupNext->nStrLen, lpREGroupNext->wpStrStart);
       wpResult+=wsprintfW(wpResult, L"%d [", lpREGroupNext->nIndex);
       lstrcpynW(wpResult, lpREGroupNext->wpStrStart, (lpREGroupNext->nStrLen) + 1);
       wpResult+=lpREGroupNext->nStrLen;
       wpResult+=wsprintfW(wpResult, L"]\n");
     }
   }
   while (lpREGroupNext=(REGROUP *)SendMessage(pd->hMainWnd, AKD_PATNEXTGROUP, (WPARAM)lpREGroupNext, 0));

   if (MessageBoxW(pd->hMainWnd, wszResult, L"Find next?", MB_YESNO) == IDNO)
     break;
   pe.wpStr=lpREGroupRoot->wpStrEnd;
 }
 SendMessage(pd->hMainWnd, AKD_PATFREE, 0, (LPARAM)&pe);


AKD_PATREPLACE
______________

Replace in string using regular expressions. Requires include of "RegExpFunc.h".

wParam               == not used.
(PATREPLACE *)lParam == pointer to a PATREPLACE structure.

Return Value
 Result string length in characters.

Example:
 PATREPLACE pr;
 INT_PTR nLen;

 //Calculate result string length
 pr.wpStr=L"1234567890 1234567890";
 pr.wpMaxStr=pr.wpStr + lstrlenW(pr.wpStr);
 pr.wpText=pr.wpStr;
 pr.wpMaxText=pr.wpMaxStr;
 pr.wpPat=L"(23)(.*)(89)";
 pr.wpMaxPat=pr.wpPat + lstrlenW(pr.wpPat);
 pr.wpRep=L"\\1abc\\3";
 pr.wpMaxRep=pr.wpRep + lstrlenW(pr.wpRep);
 pr.dwOptions=REPE_GLOBAL|REPE_MULTILINE;
 pr.wpDelim=NULL;
 pr.wpNewLine=NULL;
 pr.wszResult=NULL;
 nLen=SendMessage(pd->hMainWnd, AKD_PATREPLACE, 0, (LPARAM)&pr);

 //Receive result string
 if (pr.wszResult=(wchar_t *)GlobalAlloc(GMEM_FIXED, nLen * sizeof(wchar_t)))
 {
   SendMessage(pd->hMainWnd, AKD_PATREPLACE, 0, (LPARAM)&pr);
   MessageBoxW(pd->hMainWnd, pr.wszResult, L"Test", MB_OK);
   GlobalFree((HGLOBAL)pr.wszResult);
 }


AKD_PATGROUPSTR
_______________

Expand string that contain group indexes, like "\2 and \1". Requires include of "RegExpFunc.h".

wParam                == not used.
(PATGROUPSTR *)lParam == pointer to a PATGROUPSTR structure.

Return Value
 Result string length in characters.

Example:
 PATEXEC pe;

 //Fill structure for AKD_PATEXEC
 pe.lpREGroupStack=0;
 pe.wpStr=L"1234567890";
 pe.wpMaxStr=pe.wpStr + lstrlenW(pe.wpStr);
 pe.wpText=pe.wpStr;
 pe.wpMaxText=pe.wpMaxStr;
 pe.wpPat=L"(23)(.*)(89)";
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
   pgs.wpStr=L"<\\3><\\1><\\2>";
   pgs.wpMaxStr=pgs.wpStr + lstrlenW(pgs.wpStr);
   pgs.wszResult=NULL;
   nLen=SendMessage(pd->hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs);

   //Receive result string
   if (pgs.wszResult=(wchar_t *)GlobalAlloc(GMEM_FIXED, nLen * sizeof(wchar_t)))
   {
     SendMessage(pd->hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs);
     MessageBoxW(pd->hMainWnd, pgs.wszResult, L"Test", MB_OK);
     GlobalFree((HGLOBAL)pgs.wszResult);
   }
 }
 SendMessage(pd->hMainWnd, AKD_PATFREE, 0, (LPARAM)&pe);


AKD_PATGETGROUP
_______________

Retrieve pattern group by index. Requires include of "RegExpFunc.h".

(STACKREGROUP *)wParam == pointer to a STACKREGROUP structure.
(int)lParam            == group index.

Return Value
 Pointer to a REGROUP structure.

Example:
 See AKD_PATEXEC example.


AKD_PATNEXTGROUP
________________

Retrieve next pattern group. Requires include of "RegExpFunc.h".

(REGROUP *)wParam == pointer to a REGROUP structure.
lParam            == not used.

Return Value
 Pointer to a next REGROUP structure.

Example:
 See AKD_PATEXEC example.


AKD_PATPREVGROUP
________________

Retrieve previous pattern group. Requires include of "RegExpFunc.h".

(REGROUP *)wParam == pointer to a REGROUP structure.
lParam            == not used.

Return Value
 Pointer to a previous REGROUP structure.

Example:
 See AKD_PATEXEC example.


AKD_PATFREE
___________

Free regular expressions pattern. Requires include of "RegExpFunc.h".

wParam            == not used.
(PATEXEC *)lParam == pointer to a PATEXEC structure.

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
(EXGETTEXTRANGE *)lParam == pointer to a EXGETTEXTRANGE structure.

Return Value
 Text length in TCHARs.

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
   pclp->nCmdLineLen=lstrcpynW(pclp->szCmdLine, wpCmdLine, COMMANDLINE_SIZE);
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
  #define DETECTFILE DETECTFILEA
  #define OPENDOCUMENT OPENDOCUMENTA
  #define OPENDOCUMENTPOST OPENDOCUMENTPOSTA
  #define SAVEDOCUMENT SAVEDOCUMENTA
  #define PLUGINADD PLUGINADDA
  #define PLUGINCALLSEND PLUGINCALLSENDA
  #define PLUGINCALLPOST PLUGINCALLPOSTA
  #define PLUGINOPTION PLUGINOPTIONA
  #define INIVALUE INIVALUEA
  #define TEXTFIND TEXTFINDA
  #define TEXTREPLACE TEXTREPLACEA
  #define CREATEWINDOW CREATEWINDOWA
#else
  #define DETECTFILE DETECTFILEW
  #define OPENDOCUMENT OPENDOCUMENTW
  #define OPENDOCUMENTPOST OPENDOCUMENTPOSTW
  #define SAVEDOCUMENT SAVEDOCUMENTW
  #define PLUGINADD PLUGINADDW
  #define PLUGINCALLSEND PLUGINCALLSENDW
  #define PLUGINCALLPOST PLUGINCALLPOSTW
  #define PLUGINOPTION PLUGINOPTIONW
  #define INIVALUE INIVALUEW
  #define TEXTFIND TEXTFINDW
  #define TEXTREPLACE TEXTREPLACEW
  #define CREATEWINDOW CREATEWINDOWW
#endif

#endif
