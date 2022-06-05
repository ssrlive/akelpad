(*************************************************************************

=========  AkelPad text editor plugin API ===========

** Origin: AkelEdit.h located at
   http://akelpad.cvs.sourceforge.net/viewvc/akelpad/akelpad_4/AkelFiles/Plugs/AkelDLL/AkelEdit.h
** Converted with C to Pascal Converter 2.0
** Release: 2.20.11.2011
** Email: al_gun@ncable.net.au
** Updates: http://cc.codegear.com/Author/302259
** Blogs: http://delphiprogrammingwithalgun.blogspot.com/
** Copyright (c) 2005, 2011 Ural Gunaydin (a.k.a. Al Gun)

===========           Edited by Fr0sT           ===========
= Tested on RAD Studio XE2 but compiles on D7 and should =
= work on other versions also.                            =

**************************************************************************)

unit AkelEdit;

interface

uses
  Windows, Messages, RichEdit,
  AkelDLL;

//// Defines

const AES_AKELEDITA = AnsiString('AkelEditA');
const AES_AKELEDITA_UNICODE = WideString('AkelEditA');
const AES_AKELEDITW_ANSI = AnsiString('AkelEditW');
const AES_AKELEDITW = WideString('AkelEditW');
const AES_RICHEDIT20A = AnsiString('RichEdit20A');
const AES_RICHEDIT20A_UNICODE = WideString('RichEdit20A');
const AES_RICHEDIT20W_ANSI = AnsiString('RichEdit20W');
const AES_RICHEDIT20W = WideString('RichEdit20W');

// Fr0sT: these C types aren't defined in used units
type
  SIZE_T = INT_PTR;
  HDROP  = THandle;

//AEM_CONTROLCLASS
const AECLASS_AKELEDIT = 1;
const AECLASS_RICHEDIT = 2;

//Window styles
{
  Fr0sT: these constants are already defined in RTL since D7
  const ES_MULTILINE       = $00000004;  //See AECO_MULTILINE.
  const ES_NOHIDESEL       = $00000100;  //See AECO_NOHIDESEL.
  const ES_READONLY        = $00000800;  //See AECO_READONLY.
  const ES_WANTRETURN      = $00001000;  //See AECO_WANTRETURN.
  const ES_DISABLENOSCROLL = $00002000;  //See AECO_DISABLENOSCROLL.
}
const ES_GLOBALUNDO = $00004000;       //Use process heap for Undo/Redo instead of window heap. Required for AEM_DETACHUNDO and AEM_ATTACHUNDO.
                                       //Compatibility: define same as ES_SUNKEN.
const ES_HEAP_SERIALIZE = $00008000;   //Mutual exclusion will be used when the heap functions allocate and free memory from window heap. Serialization of heap access allows two or more threads to simultaneously allocate and free memory from the same heap.
                                       //Compatibility: define same as ES_SAVESEL.

const AES_WORDDELIMITERSW : WideString = ' '#9#13#10'''`\"\\|[](){}<>,.;:+-=~!@#$%^&*/?';
const AES_WRAPDELIMITERSW : WideString = ' '#9;
const AES_URLLEFTDELIMITERSW : WideString = ' '#9#13#10'''`\"(<{[=';
const AES_URLRIGHTDELIMITERSW  : WideString = ' '#9#13#10'''`\")>}]';
const AES_URLPREFIXESW : WideString = 'http:'#0'https:'#0'www.'#0'ftp:'#0'file:'#0'mailto:'#0#0;



//AEM_SETEVENTMASK flags
const AENM_SCROLL             = $00000001;  //Sends AEN_HSCROLL and AEN_VSCROLL notifications.
const AENM_PAINT              = $00000002;  //Sends AEN_PAINT notifications.
const AENM_MAXTEXT            = $00000010;  //Don't use it. For internal code only.
const AENM_PROGRESS           = $00000020;  //Sends AEN_PROGRESS notifications.
const AENM_MODIFY             = $00000040;  //Sends AEN_MODIFY notifications.
const AENM_SELCHANGE          = $00000080;  //Sends AEN_SELCHANGING and AEN_SELCHANGED notifications.
const AENM_TEXTCHANGE         = $00000100;  //Sends AEN_TEXTCHANGING and AEN_TEXTCHANGED notifications.
const AENM_TEXTINSERT         = $00000200;  //Sends AEN_TEXTINSERTBEGIN and AEN_TEXTINSERTEND notifications.
const AENM_TEXTDELETE         = $00000400;  //Sends AEN_TEXTDELETEBEGIN and AEN_TEXTDELETEEND notifications.
const AENM_POINT              = $00000800;  //Sends AEN_POINT notifications.
const AENM_DROPFILES          = $00010000;  //Sends AEN_DROPFILES notifications.
const AENM_DRAGDROP           = $00020000;  //Sends AEN_DROPSOURCE and AEN_DROPTARGET notifications.
const AENM_LINK               = $00040000;  //Sends AEN_LINK notifications.
const AENM_MARKER             = $00080000;  //Sends AEN_MARKER notifications.

//AEN_SELCHANGING and AEN_SELCHANGED types
const AESCT_REPLACESEL        = $00000001;  //Replacing selection.
const AESCT_APPENDTEXT        = $00000002;  //Appending text.
const AESCT_SETTEXT           = $00000004;  //Setting text.
const AESCT_STREAMIN          = $00000008;  //Stream in.
const AESCT_WRAP              = $00000010;  //Sending AEM_SETWORDWRAP.
const AESCT_UNDO              = $00000020;  //Undo.
const AESCT_REDO              = $00000040;  //Redo.
const AESCT_CUT               = $00000080;  //Cut.
const AESCT_CHAR              = $00000100;  //Inserting char.
const AESCT_KEYRETURN         = $00000200;  //Pressing VK_RETURN.
const AESCT_KEYBACKSPACE      = $00000400;  //Pressing VK_BACK.
const AESCT_KEYDELETE         = $00000800;  //Pressing VK_DELETE.
const AESCT_DRAGDELETE        = $00001000;  //Dragging text delete.
const AESCT_DROPINSERT        = $00002000;  //Dropping text insert.

const AESCT_MOUSESINGLECLK    = $00100000;  //Mouse single click.
const AESCT_MOUSEDOUBLECLK    = $00200000;  //Mouse double click.
const AESCT_MOUSETRIPLECLK    = $00400000;  //Mouse triple click.
const AESCT_MOUSECAPTURE      = $00800000;  //Mouse made non-empty selection.
const AESCT_MOUSELEFTMARGIN   = $01000000;  //Left margin mouse action.
const AESCT_KEYBOARD          = $02000000;  //Selection is changed by keyboard.
const AESCT_SETSELMESSAGE     = $04000000;  //Sending AEM_EXSETSEL, AEM_SETSEL, EM_SETSEL, EM_EXSETSEL.
const AESCT_UPDATESELECTION   = $08000000;  //Selection is updated.
const AESCT_IME               = $10000000;  //Input Method Editors (IME).

//AEN_TEXTCHANGING, AEN_TEXTINSERTBEGIN, AEN_TEXTINSERTEND, AEN_TEXTDELETEBEGIN, AEN_TEXTDELETEEND and AEN_TEXTCHANGED types
const AETCT_REPLACESEL        = $00000001;  //Replacing selection.
const AETCT_APPENDTEXT        = $00000002;  //Appending text.
const AETCT_SETTEXT           = $00000004;  //Setting text.
const AETCT_STREAMIN          = $00000008;  //Stream in.
const AETCT_WRAP              = $00000010;  //Sending AEM_SETWORDWRAP.
const AETCT_UNDO              = $00000020;  //Undo.
const AETCT_REDO              = $00000040;  //Redo.
const AETCT_CUT               = $00000080;  //Cut.
const AETCT_CHAR              = $00000100;  //Inserting char.
const AETCT_KEYRETURN         = $00000200;  //Pressing VK_RETURN.
const AETCT_KEYBACKSPACE      = $00000400;  //Pressing VK_BACK.
const AETCT_KEYDELETE         = $00000800;  //Pressing VK_DELETE.
const AETCT_DRAGDELETE        = $00001000;  //Dragging text delete.
const AETCT_DROPINSERT        = $00002000;  //Dropping text insert.
const AETCT_COLUMNGROUP       = $00004000;  //Undo/Redo column text is grouped from one-line actions.

//AEN_TEXTCHANGED types
const AETCT_NONE              = $00100000;  //No text is changed.
const AETCT_DELETEALL         = $00200000;  //Indicate that due to AETCT_* action all text has been modified.

//Insert text flags
const AEINST_LOCKUNDO          = $00000001;
const AEINST_LOCKSCROLL        = $00000002;
const AEINST_LOCKUPDATEHSCROLL = $00000004;
const AEINST_LOCKUPDATEVSCROLL = $00000008;
const AEINST_LOCKUPDATECARET   = $00000010;
const AEINST_LOCKUPDATETEXT    = $00000020;
const AEINST_LOCKUPDATEALL    = (AEINST_LOCKUPDATEHSCROLL or
                                 AEINST_LOCKUPDATEVSCROLL or
                                 AEINST_LOCKUPDATECARET   or
                                 AEINST_LOCKUPDATETEXT);

//Delete text flags
const AEDELT_LOCKUNDO          = $00000001;
const AEDELT_LOCKSCROLL        = $00000002;
const AEDELT_LOCKUPDATEHSCROLL = $00000004;
const AEDELT_LOCKUPDATEVSCROLL = $00000008;
const AEDELT_LOCKUPDATECARET   = $00000010;
const AEDELT_LOCKUPDATETEXT    = $00000020;
const AEDELT_SAVECOLUMNSEL     = $00000040;
const AEDELT_LOCKUPDATEALL    = (AEDELT_LOCKUPDATEHSCROLL or
                                 AEDELT_LOCKUPDATEVSCROLL or
                                 AEDELT_LOCKUPDATECARET   or
                                 AEDELT_LOCKUPDATETEXT);

//AEN_POINT types
const AEPTT_SETTEXT           = $00000001;  //All document text has been changed. All points reset to first character.
const AEPTT_STREAMIN          = $00000002;  //All document text has been changed. All points reset to first character.
const AEPTT_INSERT            = $00000004;  //Insert operation.
const AEPTT_DELETE            = $00000008;  //Delete operation.

//AEPOINT flags
const AEPTF_MODIFY        = $00000002;  //If set, text in (AEPOINT.ciPoint + AEPOINT.nPointLen) area has been modified.
const AEPTF_INSERT        = $00000004;  //If set, AEPOINT.nPointLen index has been increased. Additional for AEPTF_MODIFY flag.
const AEPTF_DELETE        = $00000008;  //If set, AEPOINT.nPointLen index has been decreased. Additional for AEPTF_MODIFY flag.
const AEPTF_NOTIFYDELETE  = $00000010;  //Don't use it. For internal code only.
const AEPTF_NOTIFYINSERT  = $00000020;  //Don't use it. For internal code only.
const AEPTF_VALIDLINE     = $00000040;  //Don't use it. For internal code only.
const AEPTF_FOLD          = $00000100;  //If set, AEPOINT.ciPoint index is used in fold. AEPOINT.dwUserData is pointer to a AEFOLD structure.
const AEPTF_MOVEOFFSET    = $00001000;  //If set, AEPOINT.nPointOffset has been changed.
const AEPTF_MOVELINE      = $00002000;  //If set, AEPOINT.ciPoint.nLine has been changed.

//AEPOINT character offset value
const AEPTO_IGNORE    = -1;  //Character RichEdit offset is not used in AEPOINT.
const AEPTO_CALC      = -2;  //Character RichEdit offset will calculated automatically by AEM_ADDPOINT.

//AEM_COLLAPSELINE and AEM_COLLAPSEFOLD flags
const AECF_EXPAND           = $00000000;  //Expand fold (default).
const AECF_COLLAPSE         = $00000001;  //Collapse fold.
const AECF_RECURSE          = $00000002;  //Recursive processing with all children.
const AECF_NOUPDATE         = $00000004;  //Don't update scroll and selection.
const AECF_NOCARETCORRECT   = $00000008;  //If in collapsed fold located caret, don't move it to fold start.

//AEN_DROPTARGET actions
const AEDT_TARGETENTER        = 1;  //Enter into the target window.
const AEDT_TARGETOVER         = 2;  //Moving over the target window.
const AEDT_TARGETLEAVE        = 3;  //Leaving the target window.
const AEDT_TARGETDROP         = 4;  //Drops the data into the target window.

//AEN_DROPSOURCE actions
const AEDS_SOURCEBEGIN        = 1;  //Begin selection dragging.
const AEDS_SOURCEEND          = 2;  //End selection dragging. Before selection deletion.
const AEDS_SOURCEDONE         = 3;  //End dragging operation. Same as EN_DRAGDROPDONE.

//AEN_PROGRESS type
const AEPGS_SETTEXT           = $00000001;  //Setting text.
const AEPGS_WRAPTEXT          = $00000002;  //Wrapping text.
const AEPGS_STREAMIN          = $00000004;  //Receiving stream text.

//AEN_PAINT type
const AEPNT_BEGIN             = $00000001;  //Sends before painting is started, only AENPAINT.hDC member is valid.
const AEPNT_END               = $00000002;  //Sends before clean-up paint resources.
const AEPNT_DRAWLINE          = $00000004;  //Sends before line is drawn.

//AEM_SETOPTIONS flags
                                                  // Window styles:
const AECO_READONLY                 = $00000001;  //Set read-only mode. You can use ES_READONLY window style.
const AECO_DISABLENOSCROLL          = $00000002;  //Disables scroll bars instead of hiding them when they are not needed. You can use ES_DISABLENOSCROLL window style.
const AECO_NOHIDESEL                = $00000004;  //If you do not specify this style, then hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus. You can use ES_NOHIDESEL window style.
const AECO_WANTRETURN               = $00000008;  //If you do not specify this style, pressing the ENTER key has the same effect as pressing the dialog box's default push button. You can use ES_WANTRETURN window style.
const AECO_MULTILINE                = $00000010;  //Designates a multiline edit control. The default is single-line edit control. You can use ES_MULTILINE window style.
                                                  // Other:
const AECO_DETAILEDUNDO             = $00000020;  //The control stores any typing action, into a new action in the undo queue.
const AECO_PASTESELECTCOLUMN        = $00000040;  //Selects pasted text in column mode.
const AECO_DISABLEDRAG              = $00000080;  //Disables OLE text dragging.
const AECO_DISABLEDROP              = $00000100;  //Disables OLE text dropping.
const AECO_CARETOUTEDGE             = $00000200;  //Allow caret moving out of the line edge.
const AECO_ACTIVECOLUMN             = $00000400;  //Draw caret vertical line.
const AECO_ACTIVELINE               = $00000800;  //Draw active line.
const AECO_ACTIVELINEBORDER         = $00001000;  //Draw active line border.
const AECO_ALTLINEBORDER            = $00002000;  //Draw alternating lines border.
const AECO_NONEWLINEDRAW            = $00004000;  //Disables drawing new line selection as space character.
const AECO_ENTIRENEWLINEDRAW        = $00008000;  //Draw new line selection to the right edge.
const AECO_NONEWLINEMOUSESELECT     = $00010000;  //Triple click and left margin click selects only line contents without new line.
const AECO_LBUTTONUPCONTINUECAPTURE = $00020000;  //After WM_LBUTTONUP message capture operations doesn't stopped.
const AECO_RBUTTONDOWNMOVECARET     = $00040000;  //WM_RBUTTONDOWN message moves caret to a click position.
const AECO_MBUTTONDOWNNOSCROLL      = $00080000;  //No scrolling after WM_MBUTTONDOWN message.
const AECO_MARGINSELUNWRAPLINE      = $00100000;  //Left margin line selection with mouse selects all wrapped line.
const AECO_NOMARGINSEL              = $00200000;  //Disables left margin line selection with mouse.
const AECO_NOMARKERMOVE             = $00400000;  //Disables changing position of column marker with mouse and shift button.
const AECO_NOMARKERAFTERLASTLINE    = $00800000;  //Disables marker painting after last line.
const AECO_VSCROLLBYLINE            = $01000000;  //Unit of vertical scrolling is line (default is pixel).
const AECO_NOSCROLLDELETEALL        = $02000000;  //Turn off scrolling to caret, when undo/redo operations replace all text.
const AECO_NOSCROLLSELECTALL        = $04000000;  //Turn off scrolling to caret, when all text is selected.
const AECO_NOCOLUMNPASTEHOTKEY      = $08000000;  //Turn off Alt+V for columnar paste.
const AECO_DISABLEBEEP              = $10000000;  //Disables sound beep, when unallowable action occur.
const AECO_NODCBUFFER               = $20000000;  //Don't use device context output buffering in AE_Paint. Cause edit window flashing.
const AECO_PAINTGROUP               = $40000000;  //Paint text by group of characters (default is character by character).
                                                  //With this flag some text recognition programs could start to work, printer could print faster, but highlighted symbols and combined unicode symbols can be drawn differently and editing of whose characters may become uncomfortable.
const AECO_NOPRINTCOLLAPSED         = $80000000;  //Disables print collapsed lines. See AEM_COLLAPSEFOLD message.

//AEM_EXSETOPTIONS flags
const AECOE_DETECTURL               = $00000001;  //Enables detection and highlighting of URLs by an edit control.
const AECOE_LOCKSELECTION           = $00000002;  //Prevent selection changing. Use it with AECO_READONLY flag.
const AECOE_OVERTYPE                = $00000004;  //Turn on overtype mode instead of insert mode.
const AECOE_ALTDECINPUT             = $00000008;  //Do Alt+NumPad decimal input with NumLock on (default is decimal input after two "Num 0").

const AECOOP_SET              = 1;  //Sets the options to those specified by lParam.
const AECOOP_OR               = 2;  //Combines the specified options with the current options.
const AECOOP_AND              = 3;  //Retains only those current options that are also specified by lParam.
const AECOOP_XOR              = 4;  //Logically exclusive OR the current options with those specified by lParam.

//Modifier flags
const AEMOD_ALT               = $1;  //ALT key
const AEMOD_SHIFT             = $2;  //SHIFT key
const AEMOD_CONTROL           = $4;  //CTRL key

//AEM_GETLINENUMBER flags
const AEGL_LINECOUNT              = 0;  //Total number of text lines. If the control has no text, the return value is 1.
const AEGL_FIRSTSELLINE           = 1;  //First line of the selection.
const AEGL_LASTSELLINE            = 2;  //Last line of the selection.
const AEGL_CARETLINE              = 3;  //Caret line.
const AEGL_FIRSTVISIBLELINE       = 4;  //First visible line.
const AEGL_LASTVISIBLELINE        = 5;  //Last visible line.
const AEGL_FIRSTFULLVISIBLELINE   = 6;  //First fully visible line.
const AEGL_LASTFULLVISIBLELINE    = 7;  //Last fully visible line.
const AEGL_LINEUNWRAPCOUNT       = 11;  //Total number of unwrapped text lines. If the control has no text, the return value is 1.
const AEGL_UNWRAPSELMULTILINE    = 12;  //Returns value: TRUE - if selection on multiple lines. FALSE - if no selection or selection is on single line.

//AEM_GETINDEX and AEM_GETRICHOFFSET flags
const AEGI_FIRSTCHAR              = 1;  //First character.
const AEGI_LASTCHAR               = 2;  //Last character.
const AEGI_FIRSTSELCHAR           = 3;  //First character of the selection.
const AEGI_LASTSELCHAR            = 4;  //Last character of the selection.
const AEGI_CARETCHAR              = 5;  //Caret character.
const AEGI_FIRSTVISIBLECHAR       = 6;  //First visible character, collapsed lines are skipped.
const AEGI_LASTVISIBLECHAR        = 7;  //Last visible character, collapsed lines are skipped.
const AEGI_FIRSTFULLVISIBLECHAR   = 8;  //First fully visible character, collapsed lines are skipped.
const AEGI_LASTFULLVISIBLECHAR    = 9;  //Last fully visible character, collapsed lines are skipped.
const AEGI_FIRSTVISIBLELINE      = 10;  //First character of the first visible line, collapsed lines are skipped.
const AEGI_LASTVISIBLELINE       = 11;  //Last character of the last visible line, collapsed lines are skipped.
const AEGI_FIRSTFULLVISIBLELINE  = 12;  //First character of the first fully visible line, collapsed lines are skipped.
const AEGI_LASTFULLVISIBLELINE   = 13;  //Last character of the last fully visible line, collapsed lines are skipped.
                                       //
//Next flags require pointer to the input index in lParam.
const AEGI_VALIDCHARINLINE       = 15;  //Correct character to make sure that it is on line.
                                       //For better performance use AEC_ValidCharInLine instead.
const AEGI_LINEBEGIN             = 16;  //First character in line.
                                       //
const AEGI_LINEEND               = 17;  //Last character in line.
                                       //
const AEGI_WRAPLINEBEGIN         = 18;  //First character of the unwrapped line. Returns number of characters as AEM_GETINDEX result.
                                       //For better performance use AEC_WrapLineBeginEx instead.
const AEGI_WRAPLINEEND           = 19;  //Last character of the unwrapped line. Returns number of characters as AEM_GETINDEX result.
                                       //For better performance use AEC_WrapLineEndEx instead.
const AEGI_NEXTCHARINLINE        = 20;  //Next character in line.
                                       //For better performance use AEC_NextCharInLineEx instead.
const AEGI_PREVCHARINLINE        = 21;  //Previous character in line.
                                       //For better performance use AEC_PrevCharInLineEx instead.
const AEGI_NEXTCHAR              = 22;  //Next wide character.
                                       //For better performance use AEC_NextCharEx instead.
const AEGI_PREVCHAR              = 23;  //Previous wide character.
                                       //For better performance use AEC_PrevCharEx instead.
const AEGI_NEXTLINE              = 24;  //First character of the next line.
                                       //For better performance use AEC_NextLineEx instead.
const AEGI_PREVLINE              = 25;  //First character of the previous line.
                                       //For better performance use AEC_PrevLineEx instead.
const AEGI_NEXTUNCOLLAPSEDCHAR   = 26;  //Next wide character, collapsed lines are skipped.
const AEGI_PREVUNCOLLAPSEDCHAR   = 27;  //Previous wide character, collapsed lines are skipped.
const AEGI_NEXTUNCOLLAPSEDLINE   = 28;  //First character of the next line, collapsed lines are skipped.
const AEGI_PREVUNCOLLAPSEDLINE   = 29;  //First character of the previous line, collapsed lines are skipped.

//AEM_ISDELIMITER parameter
const AEDLM_PREVCHAR  = $00000001;  //Check previous char.
const AEDLM_WORD      = $00000010;  //Word delimiter.
const AEDLM_WRAP      = $00000020;  //Wrap delimiter.
const AEDLM_URLLEFT   = $00000040;  //URL left delimiter.
const AEDLM_URLRIGHT  = $00000080;  //URL right delimiter.

//AEM_COLUMNTOINDEX and AEM_INDEXTOCOLUMN flags
const AECTI_WRAPLINEBEGIN = $0001;  //If set, scan from first character of the unwrapped line. If not set, scan from first character of the wrapped line.
const AECTI_FIT           = $0002;  //AEM_COLUMNTOINDEX: if set, character position must be equal or less than column boundary. If not set, character position must be equal or larger than column boundary.

//AEM_SETSEL and AEM_UPDATESEL flags
const AESELT_COLUMNON            = $00000001;  //Make column selection ON.
const AESELT_COLUMNASIS          = $00000002;  //Leave column selection as is.
const AESELT_LOCKNOTIFY          = $00000004;  //Disable AEN_SELCHANGING and AEN_SELCHANGED notifications.
const AESELT_LOCKSCROLL          = $00000008;  //Lock edit window scroll.
const AESELT_LOCKUPDATE          = $00000010;  //Lock edit window update.
const AESELT_LOCKCARET           = $00000020;  //Lock caret reposition.
const AESELT_LOCKUNDOGROUPING    = $00000040;  //Don't use it. For internal code only.
const AESELT_NOCARETHORZINDENT   = $00000080;  //Caret horizontal indent isn't changed.
const AESELT_NOVERTSCROLLCORRECT = $00000100;  //On some conditions scroll can be increased to a height of one line.
const AESELT_MOUSE               = $00000200;  //Don't use it. For internal code only.
const AESELT_RESETSELECTION      = $00000400;  //Don't use it. For internal code only.
const AESELT_INDEXUPDATE         = $00000800;  //Update lpLine member of the AEM_SETSEL message structures, to avoid dangling of a pointer after text change.

//AEM_REPLACESEL flags
const AEREPT_COLUMNON            = $00000001;  //Make column selection ON.
const AEREPT_COLUMNASIS          = $00000002;  //Leave column selection as is.
const AEREPT_LOCKSCROLL          = $00000004;  //Lock edit window scroll. However edit window can be scrolled during window resize when AECO_DISABLENOSCROLL option not set.
const AEREPT_UNDOGROUPING        = $00000100;  //Continue undo grouping.

//AEM_CHARFROMPOS return value
const AEPC_ERROR    = 0;  //Error.
const AEPC_EQUAL    = 1;  //Char exactly in specified position.
const AEPC_BEFORE   = 2;  //Char before the specified position.
const AEPC_AFTER    = 3;  //Char after the specified position.

//New line value
const AELB_ASINPUT  = 1;  //Use input new line, see AEM_SETNEWLINE with AENL_INPUT.
const AELB_ASOUTPUT = 2;  //Use output new line, see AEM_SETNEWLINE with AENL_OUTPUT.
const AELB_ASIS     = 3;  //Use new line of the source.
const AELB_EOF      = 4;  //End-of-file, last line in document.
const AELB_R        = 5;  //"\r" new line.
const AELB_N        = 6;  //"\n" new line.
const AELB_RN       = 7;  //"\r\n" new line.
const AELB_RRN      = 8;  //"\r\r\n" new line.
const AELB_WRAP     = 9;  //No new line, this line is wrapped.

//AEM_SETNEWLINE flags
const AENL_INPUT           = $00000001;  //Sets default new line for the input operations, for example AEM_PASTE.
const AENL_OUTPUT          = $00000002;  //Sets default new line for the output operations, for example AEM_COPY.

//AEM_PASTE flags
const AEPFC_ANSI           = $00000001;  //Paste text as ANSI. Default is paste as Unicode text, if no Unicode text available ANSI text will be used.
const AEPFC_COLUMN         = $00000002;  //Paste to column selection.

//AEM_LOCKUPDATE FLAGS
const AELU_SCROLLBAR  = $00000001;
const AELU_CARET      = $00000002;

//AEM_SETDOCUMENT flags
const AESWD_NOCHECKFOCUS        = $00000001;  //Don't update focus state.
const AESWD_NODRAGDROP          = $00000002;  //Don't register drag-and-drop with a new IDropTarget.
const AESWD_NOSHOWSCROLLBARS    = $00000004;  //Don't update scrollbars visibility.
const AESWD_NOUPDATESCROLLBARS  = $00000008;  //Don't update scrollbars position.
const AESWD_NOUPDATECARET       = $00000010;  //Don't update caret.
const AESWD_NOINVALIDATERECT    = $00000020;  //Don't redraw edit window.

const AESWD_NOREDRAW  = (AESWD_NOUPDATESCROLLBARS or
                         AESWD_NOUPDATECARET      or
                         AESWD_NOINVALIDATERECT);
const AESWD_NOALL     = (AESWD_NOCHECKFOCUS     or
                         AESWD_NODRAGDROP       or
                         AESWD_NOSHOWSCROLLBARS or
                         AESWD_NOREDRAW);

//AEM_DRAGDROP flags
const AEDD_GETDRAGWINDOW   = 1;  //Return dragging window handle.
const AEDD_STOPDRAG        = 2;  //Set stop dragging operation flag.

//AEM_SETCOLORS flags
const AECLR_DEFAULT          = $00000001;  //Use default system colors for the specified flags, all members of the AECOLORS structure are ignored.
const AECLR_CARET            = $00000002;  //Sets caret color. crCaret member is valid.
const AECLR_BASICTEXT        = $00000004;  //Sets basic text color. crBasicText member is valid.
const AECLR_BASICBK          = $00000008;  //Sets basic background color. crBasicBk member is valid.
const AECLR_SELTEXT          = $00000010;  //Sets text in selection color. crSelText member is valid.
const AECLR_SELBK            = $00000020;  //Sets background in selection color. crSelBk member is valid.
const AECLR_ACTIVELINETEXT   = $00000040;  //Sets active line text color. crActiveLineText member is valid.
const AECLR_ACTIVELINEBK     = $00000080;  //Sets active line background color. crActiveLineBk member is valid.
const AECLR_URLTEXT          = $00000100;  //Sets hyperlink text color. crUrlText member is valid.
const AECLR_ACTIVECOLUMN     = $00000200;  //Sets active column color. crActiveColumn member is valid.
const AECLR_COLUMNMARKER     = $00000400;  //Sets column marker color. crColumnMarker member is valid.
const AECLR_URLCURSORTEXT    = $00000800;  //Sets active hyperlink text color. crUrlCursorText member is valid.
const AECLR_URLVISITTEXT     = $00001000;  //Sets visited hyperlink text color. crUrlVisitText member is valid.
const AECLR_ACTIVELINEBORDER = $00002000;  //Sets active line border color. crActiveLineBorder member is valid.
const AECLR_ALTLINETEXT      = $00004000;  //Sets alternating line text color. crAltLineText member is valid.
const AECLR_ALTLINEBK        = $00008000;  //Sets alternating line background color. crAltLineBk member is valid.
const AECLR_ALTLINEBORDER    = $00010000;  //Sets alternating line border color. crAltLineBorder member is valid.

const AECLR_ALL  = (AECLR_CARET            or
                    AECLR_BASICTEXT        or
                    AECLR_BASICBK          or
                    AECLR_SELTEXT          or
                    AECLR_SELBK            or
                    AECLR_ACTIVELINETEXT   or
                    AECLR_ACTIVELINEBK     or
                    AECLR_URLTEXT          or
                    AECLR_ACTIVECOLUMN     or
                    AECLR_COLUMNMARKER     or
                    AECLR_URLCURSORTEXT    or
                    AECLR_URLVISITTEXT     or
                    AECLR_ACTIVELINEBORDER or
                    AECLR_ALTLINETEXT      or
                    AECLR_ALTLINEBK        or
                    AECLR_ALTLINEBORDER);

//Print
const AEPRN_TEST                      = $001;  //Calculate data without painting.
const AEPRN_INHUNDREDTHSOFMILLIMETERS = $002;  //Indicates that hundredths of millimeters are the unit of measurement for margins.
const AEPRN_INTHOUSANDTHSOFINCHES     = $004;  //Indicates that thousandths of inches are the unit of measurement for margins.
const AEPRN_WRAPNONE                  = $008;  //Print without wrapping.
const AEPRN_WRAPWORD                  = $010;  //Print with word wrapping (default).
const AEPRN_WRAPSYMBOL                = $020;  //Print with symbols wrapping.
const AEPRN_IGNOREFORMFEED            = $040;  //Ignore form-feed character '\f'.
const AEPRN_ANSI                      = $080;  //Ansi text output. Can solve draw problems on Win95/98/Me.
const AEPRN_COLOREDTEXT               = $100;  //Print colored text.
const AEPRN_COLOREDBACKGROUND         = $200;  //Print on colored background.
const AEPRN_COLOREDSELECTION          = $400;  //Print text selection.

//Highlight options
const AEHLO_IGNOREFONTNORMAL       = $00000001;  //Use AEHLS_NONE font style, if font style to change is AEHLS_FONTNORMAL.
const AEHLO_IGNOREFONTBOLD         = $00000002;  //Use AEHLS_FONTNORMAL font style, if font style to change is AEHLS_FONTBOLD.
                                                 //Use AEHLS_FONTITALIC font style, if font style to change is AEHLS_FONTBOLDITALIC.
const AEHLO_IGNOREFONTITALIC       = $00000004;  //Use AEHLS_FONTNORMAL font style, if font style to change is AEHLS_FONTITALIC.
                                                 //Use AEHLS_FONTBOLD font style, if font style to change is AEHLS_FONTBOLDITALIC.

//Highlight flags
const AEHLF_MATCHCASE              = $00000001;  //If set, the highlight operation is case-sensitive. If not set, the highlight operation is case-insensitive.
const AEHLF_WORDCOMPOSITION        = $00000002;  //Word is a composition of characters. For example, AEWORDITEM.pWord equal to "1234567890" with this flag, means highlight words that contain only digits.
const AEHLF_QUOTEEND_REQUIRED      = $00000004;  //If quote end isn't found, text after quote start will not be highlighted.
const AEHLF_QUOTESTART_ISDELIMITER = $00000008;  //Last meet delimiter used as quote start (AEQUOTEITEM.pQuoteStart member is ignored).
const AEHLF_QUOTEEND_ISDELIMITER   = $00000010;  //First meet delimiter used as quote end (AEQUOTEITEM.pQuoteEnd member is ignored).
const AEHLF_QUOTESTART_NOHIGHLIGHT = $00000020;  //Don't highlight quote start string.
const AEHLF_QUOTEEND_NOHIGHLIGHT   = $00000040;  //Don't highlight quote end string.
const AEHLF_QUOTESTART_NOCATCH     = $00000080;  //Don't catch and don't highlight quote start string.
const AEHLF_QUOTEEND_NOCATCH       = $00000100;  //Don't catch and don't highlight quote end string.
const AEHLF_ATLINESTART            = $00000200;  //Quote start, delimiter or word located at line start.
const AEHLF_ATLINEEND              = $00000400;  //Quote end, delimiter or word located at line end.
const AEHLF_QUOTESTART_ISWORD      = $00000800;  //Quote start is surrounded with delimiters.
const AEHLF_QUOTEEND_ISWORD        = $00001000;  //Quote end is surrounded with delimiters.
const AEHLF_QUOTEWITHOUTDELIMITERS = $00002000;  //Quote doesn't contain delimiters.
const AEHLF_QUOTESTART_CATCHONLY   = $00004000;  //Only quote start string is catched.
const AEHLF_QUOTEEMPTY             = $00008000;  //Quote doesn't contain any character.
const AEHLF_QUOTEINCLUDE           = $00010000;  //Quote include string is valid.
const AEHLF_QUOTEEXCLUDE           = $00020000;  //Quote exclude string is valid.
                                                 //Regular exression:
const AEHLF_REGEXP                 = $10000000;  //Can be used in AEQUOTEITEM.dwFlags.
                                                 //  AEQUOTEITEM.pQuoteStart is a regular exression pattern,
                                                 //  AEQUOTEITEM.pQuoteEnd is a regular exression match map in format:
                                                 //    "\BackRef1=(FontStyle,ColorText,ColorBk) \BackRef2=(FontStyle,ColorText,ColorBk) ..."
                                                 //  Notes:
                                                 //    Color need to be in RRGGBB or RGB format with # prefix or without.
                                                 //    If color equal to zero, then color ignored.
                                                 //    Instead of color backreference can be used.
                                                 //  Example (highlight quoted string):
                                                 //    AEQUOTEITEM.pQuoteStart  (")([^"\\]*(\\.[^"\\]*)*)(")
                                                 //    AEQUOTEITEM.pQuoteEnd    \1=(0,#FF0000,0) \2=(0,#00F,0) \4=(0,#FF0000,0)
                                                 //  Example (highlight #RRGGBB or #RGB word with its color):
                                                 //    AEQUOTEITEM.pQuoteStart  #([A-F\d]{6}|[A-F\d]{3})\b
                                                 //    AEQUOTEITEM.pQuoteEnd    \0=(0,\1,0)
                                                 //Can be used in AEMARKTEXTITEM.dwFlags.
                                                 //  AEMARKTEXTITEM.pMarkText is a regular exression pattern.

//Highlight font style
const AEHLS_NONE                   = 0;  //Current style.
const AEHLS_FONTNORMAL             = 1;  //Normal style.
const AEHLS_FONTBOLD               = 2;  //Bold style.
const AEHLS_FONTITALIC             = 3;  //Italic style.
const AEHLS_FONTBOLDITALIC         = 4;  //Bold italic style.

//Highlight elements
const AEHLE_DELIMITER              = 1;  //Delimiter. New line, start of file and end of file are delimiters by default.
const AEHLE_WORD                   = 2;  //Word - string surrounded with delimiters.
const AEHLE_QUOTE                  = 3;  //Quote - text started with quote start string and ended with quote end string.
const AEHLE_MARKTEXT               = 4;  //Mark text - mark specified text.
const AEHLE_MARKRANGE              = 5;  //Mark range - mark specified range of characters.

//Highlight AEM_HLGETHIGHLIGHT flags
const AEGHF_NOSELECTION            = $00000001; //Ignore text selection coloring.
const AEGHF_NOACTIVELINE           = $00000002; //Ignore active line colors.
const AEGHF_NOALTLINE              = $00000004; //Ignore alternating line colors.

//Highlight paint type
const AEHPT_SELECTION              = $00000001;
const AEHPT_DELIM1                 = $00000002;
const AEHPT_WORD                   = $00000004;
const AEHPT_DELIM2                 = $00000008;
const AEHPT_QUOTE                  = $00000010;
const AEHPT_MARKTEXT               = $00000020;
const AEHPT_MARKRANGE              = $00000040;
const AEHPT_LINK                   = $00000080;
const AEHPT_FOLD                   = $00000100;

//AEREGROUPCOLOR flags
const AEREGCF_BACKREFCOLORTEXT  = $00000001;  //AEREGROUPCOLOR.crText is backreference index for text color in format #RRGGBB or RRGGBB.
const AEREGCF_BACKREFCOLORBK    = $00000002;  //AEREGROUPCOLOR.crBk is backreference index for background color in format #RRGGBB or RRGGBB.

//AEM_FINDFOLD flags
const AEFF_FINDOFFSET      = $00000001;  //AEFINDFOLD.dwFindIt is RichEdit offset.
const AEFF_FINDINDEX       = $00000002;  //AEFINDFOLD.dwFindIt is pointer to a AECHARINDEX structure.
const AEFF_FINDLINE        = $00000004;  //AEFINDFOLD.dwFindIt is zero based line number.
const AEFF_FOLDSTART       = $00000008;  //Fold is also accepted if AEFINDFOLD.dwFindIt points to fold start.
const AEFF_FOLDEND         = $00000010;  //Fold is also accepted if AEFINDFOLD.dwFindIt points to fold end.
const AEFF_RECURSE         = $00000020;  //Recursive search. Returned fold will be deepest possible.
const AEFF_GETROOT         = $00000040;  //Return root fold.
                                         //
                                         //The following groups of flags cannot be used together:
                                         //  AEFF_FINDOFFSET, AEFF_FINDINDEX and AEFF_FINDLINE.
                                         //  AEFF_RECURSE and AEFF_GETROOT.

//AEM_SCROLL, AEM_LINESCROLL flags
const AESB_HORZ            = $00000001;  //Horizontal scroll. Cannot be used with AESB_VERT.
const AESB_VERT            = $00000002;  //Vertical scroll. Cannot be used with AESB_HORZ.
const AESB_RETURNUNITS     = $00000004;  //If AESB_HORZ specified, number of characters scrolled returns. If AESB_VERT specified, number of lines scrolled returns.
const AESB_ALIGNLEFT       = $00000008;  //Align first visible char.
const AESB_ALIGNRIGHT      = $00000010;  //Align last visible char.
const AESB_ALIGNTOP        = $00000020;  //Align first visible line.
const AESB_ALIGNBOTTOM     = $00000040;  //Align last visible line.

//AEM_SCROLLTOPOINT flags
const AESC_TEST            = $00000001;  //Only test for scroll. Returns result, but not actually scroll.
const AESC_POINTCARET      = $00000010;  //Caret position is used and AESCROLLTOPOINT.ptPos is ignored.
const AESC_POINTGLOBAL     = $00000020;  //AESCROLLTOPOINT.ptPos is position in the virtual text space coordinates.
const AESC_POINTCLIENT     = $00000040;  //AESCROLLTOPOINT.ptPos is position in the client area coordinates (default).
const AESC_POINTOUT        = $00000080;  //AESCROLLTOPOINT.ptPos will contain new scroll position after AEM_SCROLLTOPOINT returns.
const AESC_OFFSETPIXELX    = $00000100;  //AESCROLLTOPOINT.nOffsetX specifies pixels number.
const AESC_OFFSETPIXELY    = $00000200;  //AESCROLLTOPOINT.nOffsetY specifies pixels number.
const AESC_OFFSETCHARX     = $00000400;  //AESCROLLTOPOINT.nOffsetX specifies characters number.
const AESC_OFFSETCHARY     = $00000800;  //AESCROLLTOPOINT.nOffsetY specifies characters number.
const AESC_OFFSETRECTDIVX  = $00001000;  //AESCROLLTOPOINT.nOffsetX specifies divisor of the edit rectangle width.
const AESC_OFFSETRECTDIVY  = $00002000;  //AESCROLLTOPOINT.nOffsetY specifies divisor of the edit rectangle width.
const AESC_FORCELEFT       = $00010000;  //Scrolls to the left even if AESCROLLTOPOINT.ptPos visible.
const AESC_FORCETOP        = $00020000;  //Scrolls to the top even if AESCROLLTOPOINT.ptPos visible.
const AESC_FORCERIGHT      = $00040000;  //Scrolls to the right even if AESCROLLTOPOINT.ptPos visible.
const AESC_FORCEBOTTOM     = $00080000;  //Scrolls to the bottom even if AESCROLLTOPOINT.ptPos visible.

//AEM_SCROLLTOPOINT return values
const AECSE_SCROLLEDX      = $00000001;  //Edit control was horizontally scrolled.
const AECSE_SCROLLEDY      = $00000002;  //Edit control was vertically scrolled.
const AECSE_SCROLLEDLEFT   = $00000004;  //Edit control was scrolled left horizontally.
const AECSE_SCROLLEDRIGHT  = $00000008;  //Edit control was scrolled right horizontally.
const AECSE_SCROLLEDUP     = $00000010;  //Edit control was scrolled up vertically.
const AECSE_SCROLLEDDOWN   = $00000020;  //Edit control was scrolled down vertically.

//AEM_GETFONT type
const AEGF_CURRENT         = 0;  //Current font handle.
const AEGF_NORMAL          = 1;  //Normal style font handle.
const AEGF_BOLD            = 2;  //Bold style font handle.
const AEGF_ITALIC          = 3;  //Italic style font handle.
const AEGF_BOLDITALIC      = 4;  //Bold italic style font handle.
const AEGF_URL             = 5;  //URL style font handle.

//AEM_GETCHARSIZE flags
const AECS_HEIGHT          = 0;  //Current font character height including line gap. lParam not used.
const AECS_AVEWIDTH        = 1;  //Current font character average width. lParam not used.
const AECS_INDEXWIDTH      = 2;  //lParam is character index, which width is retrieving.
const AECS_POINTSIZE       = 3;  //Current font point size. lParam not used.
const AECS_SPACEWIDTH      = 4;  //Current font space width. lParam not used.
const AECS_TABWIDTH        = 5;  //Current font tabulation width. lParam not used.

//AEM_CONVERTPOINT flags
const AECPT_GLOBALTOCLIENT = 0;  //Convert position in the virtual text space of the document, to client area coordinates.
const AECPT_CLIENTTOGLOBAL = 1;  //Convert position in the client area coordinates, to virtual text space of the document.

//Coordinate type
const AECT_GLOBAL = 0;  //Position in the virtual text space coordinates.
const AECT_CLIENT = 1;  //Position in the client area coordinates.

//Rectangle flags
const AERC_UPDATE    = $01;  //Redraw edit window. Only for AEM_SETRECT and AEM_SETERASERECT.
const AERC_MARGINS   = $02;  //Rectangle contain edit area margins instead of edit area coordinates.
const AERC_NOLEFT    = $04;  //Don't set/retrieve left side.
const AERC_NOTOP     = $08;  //Don't set/retrieve top side.
const AERC_NORIGHT   = $10;  //Don't set/retrieve right side.
const AERC_NOBOTTOM  = $20;  //Don't set/retrieve bottom side.

//AEM_POINTONMARGIN sides
const AESIDE_LEFT          = $00000001;
const AESIDE_TOP           = $00000002;
const AESIDE_RIGHT         = $00000004;
const AESIDE_BOTTOM        = $00000008;

//AEM_GETMOUSESTATE types
const AEMS_CAPTURE         = 1;  //Capture state.
const AEMS_SELECTION       = 2;  //Selection state.

//AEM_GETMOUSESTATE capture
const AEMSC_MOUSEMOVE       = $1;  //Text selection.
const AEMSC_MOUSESCROLL     = $2;  //Middle button scroll.
const AEMSC_MOUSEDRAG       = $4;  //Selection dragging.
const AEMSC_MOUSEMARKER     = $8;  //Marker moving.

//AEM_GETMOUSESTATE selection
const AEMSS_LBUTTONUP      = $1;  //WM_LBUTTONUP has been received.
const AEMSS_CHARS          = $2;  //Characters selection.
const AEMSS_WORDS          = $4;  //Words selection.
const AEMSS_LINES          = $8;  //Lines selection.

//AEM_FINDTEXT, AEM_ISMATCH flags
const AEFR_DOWN               = $00000001;  //Same as FR_DOWN. If set, the search is from the beginning to the end of the search range. If not set, the search is from the end to the beginning of the search range.
const AEFR_WHOLEWORD          = $00000002;  //Same as FR_WHOLEWORD. If set, the operation searches only for whole words that match the search string. If not set, the operation also searches for word fragments that match the search string.
const AEFR_MATCHCASE          = $00000004;  //Same as FR_MATCHCASE. If set, the search operation is case-sensitive. If not set, the search operation is case-insensitive.
const AEFR_REGEXP             = $00080000;  //Regular expression search.
const AEFR_REGEXPNONEWLINEDOT = $00100000;  //Symbol . specifies any character except new line.
const AEFR_REGEXPMINMATCH     = $00200000;  //Allow zero length match at string edges. For example: "^" at the string beginning or "$" at the string ending.

//AEM_SETWORDWRAP flags
const AEWW_NONE            = $00000000;  //Turn off wrap.
const AEWW_WORD            = $00000001;  //Wrap by words.
const AEWW_SYMBOL          = $00000002;  //Wrap by symbols.
const AEWW_LIMITPIXEL      = $00000100;  //Limit in pixels (default).
const AEWW_LIMITSYMBOL     = $00000200;  //Limit in symbols.

//AEM_SETMARKER types
const AEMT_PIXEL           = 0;  //Pixel integer.
const AEMT_SYMBOL          = 1;  //Column number.

//AEM_SETWORDBREAK flags
const AEWB_LEFTWORDSTART   = $00000001;  //Left movement is stopped, when word start is found.
const AEWB_LEFTWORDEND     = $00000002;  //Left movement is stopped, when word end is found.
const AEWB_RIGHTWORDSTART  = $00000004;  //Right movement is stopped, when word start is found.
const AEWB_RIGHTWORDEND    = $00000008;  //Right movement is stopped, when word end is found.
const AEWB_STOPSPACESTART  = $00000010;  //Movement is stopped, when spacing start is found. Cannot be combined with AEWB_SKIPSPACESTART.
const AEWB_STOPSPACEEND    = $00000020;  //Movement is stopped, when spacing end is found. Cannot be combined with AEWB_SKIPSPACEEND.
const AEWB_SKIPSPACESTART  = $00000040;  //Movement is continued, when spacing start is found. Cannot be combined with AEWB_STOPSPACESTART.
const AEWB_SKIPSPACEEND    = $00000080;  //Movement is continued, when spacing end is found. Cannot be combined with AEWB_STOPSPACEEND.
const AEWB_STOPNEWLINE     = $00000100;  //Movement is stopped, when new line is found.
const AEWB_MINMOVE         = $00001000;  //Minimum movement or not move at all if flags matched.

//AEM_STREAMIN, AEM_STREAMOUT flags
const AESF_SELECTION       = $00000001;  //Stream-in (read) or stream-out (write) the current selection. If not specified, stream-in (read) or stream-out (write) the entire contents of the control.
const AESF_FILLSPACES      = $00000002;  //Stream-out (write) the current column selection with the filling empty spaces. Valid if bColumnSel member of a AESTREAMOUT structure is TRUE.

//AEM_ISRANGEMODIFIED return value
const AEIRM_UNMODIFIED      = 1;
const AEIRM_MODIFIEDUNSAVED = 2;
const AEIRM_MODIFIEDSAVED   = 3;


{
  Fr0sT: these constants are defined in AkelDLL.pas so do not redefine them here

const FR_DOWN = $00000001;
const FR_WHOLEWORD = $00000002;
}

const FR_MATCHCASE = $00000004;

{
  Fr0sT: these constants are defined in RTL since D7

const EC_LEFTMARGIN = $0001;
const EC_RIGHTMARGIN = $0002;
const SPI_GETWHEELSCROLLLINES = $0068;
const WM_MOUSEWHEEL = $020A;
const EN_DRAGDROPDONE = $070c;
const WC_NO_BEST_FIT_CHARS = $00000400;
const GT_SELECTION = $0002;
}


const EM_SHOWSCROLLBAR = (WM_USER + 96);
const EM_GETSCROLLPOS = (WM_USER + 221);
const EM_SETSCROLLPOS = (WM_USER + 222);
const EM_SETTEXTEX = (WM_USER + 97);

const ST_DEFAULT = $0000;
const ST_KEEPUNDO = $0001;
const ST_SELECTION = $0002;
const ST_NEWCHARS = $0004;

type
  SETTEXTEX = record
    flags: DWORD;
    codepage: UINT;
   end;

// Fr0sT: mod => Abs()
// #define mod(a)  (((a) < 0) ? (0 - (a)) : (a))

// Fr0sT: already defined in AkelDLL
//  AEHDOC = THandle;

type
  AEHPRINT = THandle;
type
  AEHTHEME = THandle;
type
  AEHDELIMITER = THandle;
type
  AEHWORD = THandle;
type
  AEHQUOTE = THandle;
type
  AEHMARKTEXT = THandle;
type
  AEHMARKRANGE = THandle;

// Fr0sT: already defined in AkelDLL
// hDoc        Document handle returned by AEM_GETDOCUMENT or AEM_CREATEDOCUMENT.
// uMsg        Message ID for example EM_SETSEL.
// lParam      Additional parameter.
// wParam      Additional parameter.
//
// Return Value
//   Depends on message.
//  AEEditProc = function(hDoc: AEHDOC; uMsg: UINT; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;


//// Structures for x64 RichEdit support

type
  PCHARRANGE64 = ^TCHARRANGE64;
  _CHARRANGE64 = record
    cpMin: INT_PTR;
    cpMax: INT_PTR;
  end;
  TCHARRANGE64 = _CHARRANGE64;


type
  _TEXTRANGE64A = record
    chrg: TCHARRANGE64;
    lpstrText: LPSTR;
  end;
  TTEXTRANGE64A = _TEXTRANGE64A;


type
  _TEXTRANGE64W = record
    chrg: TCHARRANGE64;
    lpstrText: LPWSTR;
  end;
  TTEXTRANGE64W = _TEXTRANGE64W;


type
  _FINDTEXTEX64A = record
    chrg: TCHARRANGE64;
    lpstrText: LPCSTR;
    chrgText: TCHARRANGE64;
  end;
  TFINDTEXTEX64A = _FINDTEXTEX64A;


type
  _FINDTEXTEX64W = record
    chrg: TCHARRANGE64;
    lpstrText: LPCWSTR;
    chrgText: TCHARRANGE64;
  end;
  TFINDTEXTEX64W = _FINDTEXTEX64W;


type
  _GETTEXTEX64 = record
    cb: UINT_PTR;
    flags: DWORD;
    codepage: UINT;
    lpDefaultChar: LPCSTR;
    lpUsedDefChar: PBOOL;
  end;
  TGETTEXTEX64 = _GETTEXTEX64;


type
  _SELCHANGE64 = record
    nmhdr: NMHDR;
    chrg: TCHARRANGE;
    seltyp: WORD;
    chrg64: TCHARRANGE64;
  end;
  TSELCHANGE64 = _SELCHANGE64;


type
  _ENDROPFILES64 = record
    nmhdr: NMHDR;
    hDrop: THandle;
    cp: LongInt;
    fProtected: BOOL;
    cp64: INT_PTR;
  end;
  TENDROPFILES64 = _ENDROPFILES64;


type
  _ENLINK64 = record
    nmhdr: NMHDR;
    msg: UINT;
    wParam: WPARAM;
    lParam: LPARAM;
    chrg: TCHARRANGE;
    chrg64: TCHARRANGE64;
  end;
  TENLINK64 = _ENLINK64;


//// Structures for x64 support

type
  _POINT64 = record
    x: INT_PTR;
    y: INT_PTR;
  end;
  TPOINT64 = _POINT64;


type
  _SIZE64 = record
    cx: INT_PTR;
    cy: INT_PTR;
  end;
  TSIZE64 = _SIZE64;


//// Structures

// Fr0sT: already defined in AkelDLL
//   HSTACK

type
  PStack = ^TStack;
  TStack = record
    next: PStack;
    prev: PStack;
  end;

// Fr0sT: already defined in AkelDLL
//   AELINEDATA
//   AELINEINDEX
//   AECHARINDEX
//    AECHARRANGE


type
  _AESELECTION = record
    crSel: TAECHARRANGE;  //Characters range.
    dwFlags: DWORD;      //See AESELT_* defines.
    dwType: DWORD;       //See AESCT_* defines.
  end;
  TAESELECTION = _AESELECTION;


type
  PAEPOINT = ^TAEPOINT;
  _AEPOINT = record
    next: PAEPOINT;   //Pointer to the next AEPOINT structure.
    prev: PAEPOINT;   //Pointer to the previous AEPOINT structure.
    ciPoint: TAECHARINDEX;     //Read-only character index.
    nPointOffset: INT_PTR;    //Character RichEdit offset or one of the AEPTO_* define.
    nPointLen: Integer;           //Point length.
    dwFlags: DWORD;           //See AEPTF_* defines.
    dwUserData: UINT_PTR;     //User data.
    nTmpPointOffset: INT_PTR; //Don't use it. For internal code only.
    nTmpPointLen: Integer;        //Don't use it. For internal code only.
  end;
  TAEPOINT = _AEPOINT;


type
  PAEFOLD = ^TAEFOLD;
  _AEFOLD = record
    next: PAEFOLD;       //Pointer to the next AEFOLD structure.
    prev: PAEFOLD;       //Pointer to the previous AEFOLD structure.
    parent: PAEFOLD;     //Pointer to the parent AEFOLD structure.
    firstChild: PAEFOLD; //Pointer to the first child AEFOLD structure.
    lastChild: PAEFOLD;  //Pointer to the last child AEFOLD structure.
    lpMinPoint: PAEPOINT;        //Minimum line point.
    lpMaxPoint: PAEPOINT;        //Maximum line point.
    bCollapse: BOOL;             //Collapse state.
    dwFontStyle: DWORD;          //See AEHLS_* defines.
    crText: COLORREF;            //Text color. If -1, then don't set.
    crBk: COLORREF;              //Background color. If -1, then don't set.
    dwUserData: UINT_PTR;        //User data.
  end;
  TAEFOLD = _AEFOLD;


type
  _AEFINDFOLD = record
    dwFlags: DWORD;         //[in]  See AEFF_* defines.
    dwFindIt: UINT_PTR;     //[in]  Depend on AEFF_FIND* define.
    lpParent: PAEFOLD;      //[out] Parent fold.
    lpPrevSubling: PAEFOLD; //[out] Previous subling fold.
  end;
  TAEFINDFOLD = _AEFINDFOLD;


type
  _AESETTEXTA = record
    pText: PAnsiChar;     //[in] Text to append.
    dwTextLen: UINT_PTR;  //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;    //[in] See AELB_* defines.
    nCodePage: Integer;   //[in] Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  end;
  TAESETTEXTA = _AESETTEXTA;


type
  _AESETTEXTW = record
    pText: PWideChar;     //[in] Text to append.
    dwTextLen: UINT_PTR;  //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;    //[in] See AELB_* defines.
  end;
  TAESETTEXTW = _AESETTEXTW;


type
  _AEAPPENDTEXTA = record
    pText: PAnsiChar;     //[in] Text to append.
    dwTextLen: UINT_PTR;  //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;    //[in] See AELB_* defines.
    nCodePage: Integer;   //[in] Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  end;
  TAEAPPENDTEXTA = _AEAPPENDTEXTA;


type
  _AEAPPENDTEXTW = record
    pText: PWideChar;             //[in] Text to append.
    dwTextLen: UINT_PTR;          //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;            //[in] See AELB_* defines.
  end;
  TAEAPPENDTEXTW = _AEAPPENDTEXTW;


type
  _AEREPLACESELA = record
    pText: PAnsiChar;             //[in]  Text to replace with.
    dwTextLen: UINT_PTR;          //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;            //[in]  See AELB_* defines.
    dwFlags: DWORD;               //[in]  See AEREPT_* defines.
    ciInsertStart: PAECHARINDEX;  //[out] Insert "from" character index after replacement.
    ciInsertEnd: PAECHARINDEX;    //[out] Insert "to" character index after replacement.
    nCodePage: Integer;           //[in]  Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  end;
  TAEREPLACESELA = _AEREPLACESELA;


type
  _AEREPLACESELW = record
    pText: PWideChar;             //[in]  Text to replace with.
    dwTextLen: UINT_PTR;          //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;            //[in]  See AELB_* defines.
    dwFlags: DWORD;               //[in]  See AEREPT_* defines.
    ciInsertStart: PAECHARINDEX;  //[out] Insert "from" character index after replacement.
    ciInsertEnd: PAECHARINDEX;    //[out] Insert "to" character index after replacement.
  end;
  TAEREPLACESELW = _AEREPLACESELW;


type
  _AETEXTRANGEA = record
    cr: TAECHARRANGE;             //[in]  Characters range to retrieve.
    bColumnSel: BOOL;            //[in]  Column selection. If this value is -1, use current selection type.
    pBuffer: PAnsiChar;          //[out] Pointer to buffer that receives the text. If this value is NULL, the function returns the required buffer size in characters.
    dwBufferMax: UINT_PTR;       //[in]  Specifies the maximum number of characters to copy to the buffer, including the NULL character.
    nNewLine: Integer;           //[in]  See AELB_* defines.
    nCodePage: Integer;          //[in]  Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
    lpDefaultChar: PAnsiChar;    //[in]  Points to the character used if a wide character cannot be represented in the specified code page. If this member is NULL, a system default value is used.
    lpUsedDefChar: PBOOL;        //[in]  Points to a flag that indicates whether a default character was used. The flag is set to TRUE if one or more wide characters in the source string cannot be represented in the specified code page. Otherwise, the flag is set to FALSE. This member may be NULL.
    bFillSpaces: BOOL;           //[in]  If bColumnSel is TRUE, fill empties with spaces.
  end;
  TAETEXTRANGEA = _AETEXTRANGEA;


type
  _AETEXTRANGEW = record
    cr: TAECHARRANGE;            //[in]  Characters range to retrieve.
    bColumnSel: BOOL;            //[in]  Column selection. If this value is -1, use current selection type. If bColumnSel is TRUE, then selection must be exist, otherwise it is not necessary.
    pBuffer: PWideChar;          //[out] Pointer to buffer that receives the text. If this value is NULL, the function returns the required buffer size in characters.
    dwBufferMax: UINT_PTR;       //[in]  Specifies the maximum number of characters to copy to the buffer, including the NULL character.
    nNewLine: Integer;           //[in]  See AELB_* defines.
    nCodePage: Integer;          //[in]  Ignored. Code page is always 1200 (UTF-16 LE).
    lpDefaultChar: PWideChar;    //[in]  Ignored.
    lpUsedDefChar: PBOOL;        //[in]  Ignored.
    bFillSpaces: BOOL;           //[in]  If bColumnSel is TRUE, fill empties with spaces.
  end;
  TAETEXTRANGEW = _AETEXTRANGEW;


type
  //dwCookie        Value of the dwCookie member of the AESTREAMIN or AESTREAMOUT structure. The application specifies this value when it sends the AEM_STREAMIN or AEM_STREAMOUT message.
  //wszBuf          Pointer to a buffer to read from or write to. For a stream-in (read) operation, the callback function fills this buffer with data to transfer into the edit control. For a stream-out (write) operation, the buffer contains data from the control that the callback function writes to some storage.
  //dwBufBytesSize  Number of bytes to read or write.
  //dwBufBytesDone  Pointer to a variable that the callback function sets to the number of bytes actually read or written.
  //
  //Return Value
  // The callback function returns zero to indicate success.
  //
  //Remarks
  // The control continues to call the callback function until one of the following conditions occurs:
  // * The callback function returns a nonzero value.
  // * The callback function returns zero in the *dwBufBytesDone parameter.
  TAEStreamCallback = function(dwCookie: UINT_PTR; var wszBuf: PWideChar; dwBufBytesSize: DWORD; var dwBufBytesDone: DWORD): DWORD; stdcall;

type
  _AESTREAMIN = record
    dwCookie: UINT_PTR;            //[in]  Specifies an application-defined value that the edit control passes to the AEStreamCallback function specified by the lpCallback member.
    dwError: DWORD;                //[out] Indicates the results of the stream-in (read) operation.
    lpCallback: TAEStreamCallback; //[in]  Pointer to an AEStreamCallback function, which is an application-defined function that the control calls to transfer data. The control calls the callback function repeatedly, transferring a portion of the data with each call.
    nNewLine: Integer;             //[in]  See AELB_* defines.
    dwTextLen: UINT_PTR;           //[in]  Text length. Need if using AEN_PROGRESS notification.
    nFirstNewLine: Integer;        //[out] Indicates first new line. See AELB_* defines.
  end;
  TAESTREAMIN = _AESTREAMIN;


type
  _AESTREAMOUT = record
    dwCookie: UINT_PTR;            //[in]  Specifies an application-defined value that the edit control passes to the AEStreamCallback function specified by the lpCallback member.
    dwError: DWORD;                //[out] Indicates the result of the stream-out (write) operation.
    lpCallback: TAEStreamCallback; //[in]  Pointer to an AEStreamCallback function, which is an application-defined function that the control calls to transfer data. The control calls the callback function repeatedly, transferring a portion of the data with each call.
    nNewLine: Integer;             //[in]  See AELB_* defines.
    bColumnSel: BOOL;              //[in]  Column selection. If this value is -1, use current selection type.
  end;
  TAESTREAMOUT = _AESTREAMOUT;


type
  _AEFINDTEXTA = record
    dwFlags: DWORD;               //[in]  See AEFR_* defines.
    pText: PAnsiChar;             //[in]  Text to find.
    dwTextLen: UINT_PTR;          //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;            //[in]  See AELB_* defines.
    crSearch: TAECHARRANGE;       //[in]  Range of characters to search.
    crFound: TAECHARRANGE;        //[out] Range of characters in which text is found.
    nCompileErrorOffset: INT_PTR; //[out] Contain pattern offset, if error occurred during compile pattern. Return when AEFR_REGEXP is set.
  end;
  TAEFINDTEXTA = _AEFINDTEXTA;


type
  _AEFINDTEXTW = record
    dwFlags: DWORD;               //[in]  See AEFR_* defines.
    pText: PWideChar;             //[in]  Text to find.
    dwTextLen: UINT_PTR;          //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;            //[in]  See AELB_* defines.
    crSearch: TAECHARRANGE;       //[in]  Range of characters to search.
    crFound: TAECHARRANGE;        //[out] Range of characters in which text is found.
    nCompileErrorOffset: INT_PTR; //[out] Contain pattern offset, if error occurred during compile pattern. Return when AEFR_REGEXP is set.
  end;
  TAEFINDTEXTW = _AEFINDTEXTW;

// Fr0sT: already defined in AkelDLL
//   AECOLORS = record


type
  _AECHARCOLORS = record
    dwFlags: DWORD;           //[in] See AEGHF_* defines.
    dwFontStyle: DWORD;       //[Out] See AEHLS_* defines.
    crText: COLORREF;         //[Out] Text color in line.
    crBk: COLORREF;           //[Out] Background color in line.
    crBorderTop: COLORREF;    //[Out] Top border color of the line.
    crBorderBottom: COLORREF; //[Out] Bottom border color of the line.
  end;
  TAECHARCOLORS = _AECHARCOLORS;


type
  _AEINDEXOFFSET = record
    ciCharIn: PAECHARINDEX;   //[in]  Input character index.
    ciCharOut: PAECHARINDEX;  //[out] Output character index (result).
    nOffset: INT_PTR;         //[in]  Offset can be positive or negative. For example, +1 will return next character, -1 will return previous character.
    nNewLine: Integer;        //[in]  See AELB_* defines.
  end;
  TAEINDEXOFFSET = _AEINDEXOFFSET;


type
  _AEINDEXSUBTRACT = record
    ciChar1: PAECHARINDEX;   //[in] First character index. If NULL, first character is used.
    ciChar2: PAECHARINDEX;   //[in] Second character index. If NULL, last character is used.
    bColumnSel: BOOL;        //[in] Column selection. If this value is -1, use current selection type.
    nNewLine: Integer;       //[in] See AELB_* defines.
  end;
  TAEINDEXSUBTRACT = _AEINDEXSUBTRACT;


type
  _AESCROLLTOPOINT = record
    dwFlags: DWORD;      //[in]     See AESC_* defines.
    ptPos: TPOINT64;     //[in,out] Point to scroll to, ignored if AESC_POINTCARET flag specified.
    nOffsetX: Integer;   //[in]     Horizontal scroll offset.
    nOffsetY: Integer;   //[in]     Vertical scroll offset.
  end;
  TAESCROLLTOPOINT = _AESCROLLTOPOINT;

type
  _AESCROLLCARETOPTIONS = record
    dwFlags: DWORD;      //See AESC_OFFSET* defines.
    dwSelFlags: DWORD;   //See AESELT_* defines. Can be zero.
    dwSelType: DWORD;    //See AESCT_* defines.
    nOffsetX: Integer;   //Minimal number of characters to horizontal window edge.
    nOffsetY: Integer;   //Minimal number of lines to vertical window edge.
  end;
  TAESCROLLCARETOPTIONS = _AESCROLLCARETOPTIONS;


type
  _AESENDMESSAGE = record
    hDoc: AEHDOC;     //Document handle. See AEM_CREATEDOCUMENT message.
    uMsg: UINT;       //Window message.
    wParam: WPARAM;   //Window first additional parameter.
    lParam: LPARAM;   //Window second additional parameter.
    lResult: LRESULT; //cResult after window message returns.
  end;
  TAESENDMESSAGE = _AESENDMESSAGE;


type
  _AEPRINT = record
    dwFlags: DWORD;              //[in]     See AEPRN_* defines.
    hPrinterDC: HDC;             //[in]     Printer device context.
    hEditFont: HFONT;            //[in]     Edit font.
    hPrintFont: HFONT;           //[out]    Print font (mapped edit font).
    nCharHeight: Integer;        //[out]    Print character height.
    nAveCharWidth: Integer;      //[out]    Print character average width.
    nSpaceCharWidth: Integer;    //[out]    Print space width.
    nTabWidth: Integer;          //[out]    Print tabulation width.
    rcMargins: TRect;            //[in]     Specifies the widths of the left, top, right, and bottom margins. The AEPRN_INHUNDREDTHSOFMILLIMETERS or AEPRN_INTHOUSANDTHSOFINCHES flag indicates the units of measurement.
    rcPageFull: TRect;           //[out]    Complete page rectangle. Filled by AEM_STARTPRINTDOC message.
    rcPageIn: TRect;             //[in,out] Available page rectangle (minus margins). Filled by AEM_STARTPRINTDOC message and can be modified by user before AEM_PRINTPAGE call.
    rcPageOut: TRect;            //[out]    Filled page rectangle. Filled by AEM_PRINTPAGE message.
    crText: TAECHARRANGE;        //[in,out] Text range to print. Filled by user and changed after AEM_PRINTPAGE message.
  end;
  TAEPRINT = _AEPRINT;


type
  PAEDELIMITEMA = ^TAEDELIMITEMA;
  _AEDELIMITEMA = record
    next: PAEDELIMITEMA;
    prev: PAEDELIMITEMA;
    nIndex: Integer;            //Position of the element if positive inserts to begin of stack if negative to end.
    pDelimiter: PAnsiChar;      //Delimiter string.
    nDelimiterLen: Integer;     //Delimiter string length.
    dwFlags: DWORD;             //See AEHLF_* defines.
    dwFontStyle: DWORD;         //See AEHLS_* defines.
    crText: COLORREF;           //Delimiter text color. If -1, then don't set.
    crBk: COLORREF;             //Delimiter background color. If -1, then don't set.
  end;
  TAEDELIMITEMA = _AEDELIMITEMA;


type
  PAEDELIMITEMW = ^TAEDELIMITEMW;
  _AEDELIMITEMW = record
    next: PAEDELIMITEMW;
    prev: PAEDELIMITEMW;
    nIndex: Integer;            //Position of the element if positive inserts to begin of stack if negative to end.
    pDelimiter: PWideChar;      //Delimiter string.
    nDelimiterLen: Integer;     //Delimiter string length.
    dwFlags: DWORD;             //See AEHLF_* defines.
    dwFontStyle: DWORD;         //See AEHLS_* defines.
    crText: COLORREF;           //Delimiter text color. If -1, then don't set.
    crBk: COLORREF;             //Delimiter background color. If -1, then don't set.
  end;
  TAEDELIMITEMW = _AEDELIMITEMW;


type
  PAEWORDITEMA = ^TAEWORDITEMA;
  _AEWORDITEMA = record
    next: PAEWORDITEMA;
    prev: PAEWORDITEMA;
    nIndex: Integer;            //Reserved. Word items are automatically sorted.
    pWord: PAnsiChar;           //Word string.
    nWordLen: Integer;          //Word string length.
    dwFlags: DWORD;             //See AEHLF_* defines.
    dwFontStyle: DWORD;         //See AEHLS_* defines.
    crText: COLORREF;           //Word text color. If -1, then don't set.
    crBk: COLORREF;             //Word background color. If -1, then don't set.
  end;
  TAEWORDITEMA = _AEWORDITEMA;


type
  PAEWORDITEMW = ^TAEWORDITEMW;
  _AEWORDITEMW = record
    next: PAEWORDITEMW;
    prev: PAEWORDITEMW;
    nIndex: Integer;            //Reserved. Word items are automatically sorted.
    pWord: PWideChar;           //Word string.
    nWordLen: Integer;          //Word string length.
    dwFlags: DWORD;             //See AEHLF_* defines.
    dwFontStyle: DWORD;         //See AEHLS_* defines.
    crText: COLORREF;           //Word text color. If -1, then don't set.
    crBk: COLORREF;             //Word background color. If -1, then don't set.
  end;
  TAEWORDITEMW = _AEWORDITEMW;


type
  PAEQUOTEITEMA = ^TAEQUOTEITEMA;
  _AEQUOTEITEMA = record
    next: PAEQUOTEITEMA;
    prev: PAEQUOTEITEMA;
    nIndex: Integer;                //Reserved. Quote start items are automatically grouped in standalone stack, if following members are equal: pQuoteStart, chEscape and dwFlags with AEHLF_QUOTESTART_ISDELIMITER, AEHLF_ATLINESTART, AEHLF_QUOTESTART_ISWORD.
    pQuoteStart: PAnsiChar;   //Quote start string.
    nQuoteStartLen: Integer;        //Quote start string length.
    pQuoteEnd: PAnsiChar;     //Quote end string. If NULL, line end used as quote end.
    nQuoteEndLen: Integer;          //Quote end string length.
    chEscape: AnsiChar;             //Escape character. If it precedes quote string then quote ignored.
    pQuoteInclude: PAnsiChar; //Quote include string.
    nQuoteIncludeLen: Integer;      //Quote include string length.
    pQuoteExclude: PAnsiChar; //Quote exclude string.
    nQuoteExcludeLen: Integer;      //Quote exclude string length.
    dwFlags: DWORD;             //See AEHLF_* defines.
    dwFontStyle: DWORD;         //See AEHLS_* defines.
    crText: COLORREF;           //Quote text color. If -1, then don't set.
    crBk: COLORREF;             //Quote background color. If -1, then don't set.
    lpQuoteStart: Pointer;        //Don't use it. For internal code only.
    nCompileErrorOffset: INT_PTR;  //Contain pQuoteStart offset, if error occurred during compile regular exression pattern.
  end;
  TAEQUOTEITEMA = _AEQUOTEITEMA;


type
  PAEQUOTEITEMW = ^TAEQUOTEITEMW;
  _AEQUOTEITEMW = record
    next: PAEQUOTEITEMW;
    prev: PAEQUOTEITEMW;
    nIndex: Integer;               //Reserved. Quote start items are automatically grouped in standalone stack, if following members are equal: pQuoteStart, chEscape and dwFlags with AEHLF_QUOTESTART_ISDELIMITER, AEHLF_ATLINESTART, AEHLF_QUOTESTART_ISWORD.
    pQuoteStart: PWideChar;        //Quote start string.
    nQuoteStartLen: Integer;       //Quote start string length.
    pQuoteEnd: PWideChar;          //Quote end string. If NULL, line end used as quote end.
    nQuoteEndLen: Integer;         //Quote end string length.
    chEscape: WideChar;            //Escape character. If it precedes quote string then quote ignored.
    pQuoteInclude: PWideChar;      //Quote include string.
    nQuoteIncludeLen: Integer;     //Quote include string length.
    pQuoteExclude: PWideChar;      //Quote exclude string.
    nQuoteExcludeLen: Integer;     //Quote exclude string length.
    dwFlags: DWORD;                //See AEHLF_* defines.
    dwFontStyle: DWORD;            //See AEHLS_* defines.
    crText: COLORREF;              //Quote text color. If -1, then don't set.
    crBk: COLORREF;                //Quote background color. If -1, then don't set.
    lpQuoteStart: Pointer;         //Don't use it. For internal code only.
    nCompileErrorOffset: INT_PTR;  //Contain pQuoteStart offset, if error occurred during compile regular exression pattern.
    (* !! original:
      union {
        void *lpREGroupStack;        //Don't use it. For internal code only.
        INT_PTR nCompileErrorOffset; //Contain pQuoteStart offset, if error occurred during compile regular exression pattern.
      };
    *)
    {$IF SizeOf(INT_PTR) <> SizeOf(Pointer)}oops{$IFEND}
  end;
  TAEQUOTEITEMW = _AEQUOTEITEMW;

type
  _AEREGROUPCOLOR = record
    dwFlags: DWORD;                //See AEREGCF_* defines.
    dwFontStyle: DWORD;            //See AEHLS_* defines.
    crText: COLORREF;              //Quote text color. If -1, then don't set.
    crBk: COLORREF;                //Quote background color. If -1, then don't set.
  end;
  TAEREGROUPCOLOR = _AEREGROUPCOLOR;

type
  PAEMARKTEXTITEMA = ^TAEMARKTEXTITEMA;
  _AEMARKTEXTITEMA = record
    next: PAEMARKTEXTITEMA;
    prev: PAEMARKTEXTITEMA;
    nIndex: Integer;            //Position of the element if positive inserts to begin of stack if negative to end.
    pMarkText: PAnsiChar;       //Mark text.
    nMarkTextLen: Integer;      //Mark text length.
    dwFlags: DWORD;             //See AEHLF_* defines.
    dwFontStyle: DWORD;         //See AEHLS_* defines.
    crText: COLORREF;           //Mark text color. If -1, then don't set.
    crBk: COLORREF;             //Mark background color. If -1, then don't set.
    nCompileErrorOffset: INT_PTR; //Contain pMarkText offset, if error occurred during compile regular exression pattern.
  end;
  TAEMARKTEXTITEMA = _AEMARKTEXTITEMA;


type
  PAEMARKTEXTITEMW = ^TAEMARKTEXTITEMW;
  _AEMARKTEXTITEMW = record
    next: PAEMARKTEXTITEMW;
    prev: PAEMARKTEXTITEMW;
    nIndex: Integer;            //Position of the element if positive inserts to begin of stack if negative to end.
    pMarkText: PWideChar;       //Mark text.
    nMarkTextLen: Integer;      //Mark text length.
    dwFlags: DWORD;             //See AEHLF_* defines.
    dwFontStyle: DWORD;         //See AEHLS_* defines.
    crText: COLORREF;           //Mark text color. If -1, then don't set.
    crBk: COLORREF;             //Mark background color. If -1, then don't set.
    nCompileErrorOffset: INT_PTR;  //Contain pMarkText offset, if error occurred during compile regular exression pattern.
    (* !! original:
      union {
        void *lpREGroupStack;        //Don't use it. For internal code only.
        INT_PTR nCompileErrorOffset; //Contain pMarkText offset, if error occurred during compile regular exression pattern.
      };
    *)
    {$IF SizeOf(INT_PTR) <> SizeOf(Pointer)}oops{$IFEND}
  end;
  TAEMARKTEXTITEMW = _AEMARKTEXTITEMW;


type
  PAEMARKRANGEITEM = ^TAEMARKRANGEITEM;
  _AEMARKRANGEITEM = record
    next: PAEMARKRANGEITEM;
    prev: PAEMARKRANGEITEM;
    nIndex: Integer;          //Position of the element if positive inserts to begin of stack if negative to end.
    crMarkRange: TCHARRANGE64; //cpMin member is the first character in the range (RichEdit offset), cpMax member is the last character in the range (RichEdit offset).
    dwFlags: DWORD;           //Reserved.
    dwFontStyle: DWORD;       //See AEHLS_* defines.
    crText: COLORREF;         //Mark text color. If -1, then don't set.
    crBk: COLORREF;           //Mark background color. If -1, then don't set.
  end;
  TAEMARKRANGEITEM = _AEMARKRANGEITEM;



type
  _AEMARKTEXTMATCH = record
    lpMarkText: PAEMARKTEXTITEMW;
    crMarkText: TAECHARRANGE;
  end;
  TAEMARKTEXTMATCH = _AEMARKTEXTMATCH;


type
  _AEMARKRANGEMATCH = record
    lpMarkRange: PAEMARKRANGEITEM;
    crMarkRange: TCHARRANGE64;
  end;
  TAEMARKRANGEMATCH = _AEMARKRANGEMATCH;


type
  AEQUOTEMATCH = record
    lpQuote: PAEQUOTEITEMW;
    crQuoteStart: TAECHARRANGE;
    crQuoteEnd: TAECHARRANGE;
  end;
  TAEQUOTEMATCH = AEQUOTEMATCH;


type
  AEWORDMATCH = record 
    lpDelim1: PAEDELIMITEMW;
    crDelim1: TAECHARRANGE;
    lpWord: PAEWORDITEMW;
    crWord: TAECHARRANGE;
    lpDelim2: PAEDELIMITEMW;
    crDelim2: TAECHARRANGE;
  end;
  TAEWORDMATCH = AEWORDMATCH;


type
  AEFOLDMATCH = record 
    crFold: TCHARRANGE64;
    lpFold: PAEFOLD;
  end;
  TAEFOLDMATCH = AEFOLDMATCH;


type
  PAEHLPAINT = ^TAEHLPAINT;
  _AEHLPAINT = record
    dwDefaultText: DWORD;   //Text color without highlighting.
    dwDefaultBk: DWORD;     //Background color without highlighting.
    dwActiveText: DWORD;    //Text color with highlighting.
    dwActiveBk: DWORD;      //Background color with highlighting.
    dwFontStyle: DWORD;     //See AEHLS_* defines.
    dwPaintType: DWORD;     //See AEHPT_* defines.
    dwFindFirst: DWORD;     //Don't use it. For internal code only.
    wm: TAEWORDMATCH;        //Word or/and delimiters items are found, if AEWORDITEMW.lpDelim1 or AEWORDITEMW.lpWord or AEWORDITEMW.lpDelim2 aren't NULL.
    qm: TAEQUOTEMATCH;       //Quote item is found, if AEQUOTEMATCH.lpQuote isn't NULL.
    mrm: TAEMARKRANGEMATCH;  //Mark range item is found, if AEMARKRANGEMATCH.lpMarkRange isn't NULL.
    mtm: TAEMARKTEXTMATCH;   //Mark text item is found, if AEMARKTEXTMATCH.lpMarkText isn't NULL.
    fm: TAEFOLDMATCH;        //Fold item is found, if AEFOLDMATCH.lpFold isn't NULL.
    crLink: TAECHARRANGE;    //URL item is found, if AECHARRANGE.ciMin.lpLine and AECHARRANGE.ciMax.lpLine aren't NULL.
  end;
  TAEHLPAINT = _AEHLPAINT;


type
  //dwCookie        Value of the dwCookie member of the AEGETHIGHLIGHT structure. The application specifies this value when it sends the AEM_HLGETHIGHLIGHT message.
  //crAkelRange     Range of highlighted characters.
  //crRichRange     Range of highlighted characters (RichEdit offset).
  //hlp             Highlighted information.
  //
  //Return Value
  // To continue processing, the callback function must return zero; to stop processing, it must return nonzero.
  TAEGetHighLightCallback = function(dwCookie: UINT_PTR; crAkelRange: PAECHARRANGE; crRichRange: PCHARRANGE64; hlp: PAEHLPAINT): DWORD; stdcall;


type
  _AEGETHIGHLIGHT = record
    dwCookie: UINT_PTR;                  //[in]  Specifies an application-defined value that the edit control passes to the AEGetHighLightCallback function specified by the lpCallback member.
    dwError: DWORD;                      //[out] Indicates the result of the callback function.
    lpCallback: TAEGetHighLightCallback; //[in]  Pointer to an AEGetHighLightCallback function, which is an application-defined function that the control calls to pass highlighting information.
    crText: TAECHARRANGE;                //[in]  Range of characters to process.
    dwFlags: DWORD;                      //[in]  See AEGHF_* defines.
  end;
  TAEGETHIGHLIGHT = _AEGETHIGHLIGHT;


type
  _AENMHDR = record
    hwndFrom: HWND;
    idFrom: UINT_PTR;
    code: UINT;
    docFrom: AEHDOC;      //Document handle. See AEM_CREATEDOCUMENT message.
  end;
  TAENMHDR = _AENMHDR;


type
  _AENERRSPACE = record
    hdr: TAENMHDR;
    dwBytes: SIZE_T;      //Number of bytes that cannot be allocated.
  end;
  TAENERRSPACE = _AENERRSPACE;


type
  _AENFOCUS = record
    hdr: TAENMHDR;
    hWndChange: HWND;     //AEN_SETFOCUS - handle to the window that has lost the keyboard focus.
  end;
  TAENFOCUS = _AENFOCUS;


type
  _AENSCROLL = record
    hdr: TAENMHDR;
    nPosNew: INT_PTR;     //Current scroll position.
    nPosOld: INT_PTR;     //Previous scroll position.
    nPosMax: INT_PTR;     //Maximum scroll position.
  end;
  TAENSCROLL = _AENSCROLL;


type
  _AENSETRECT = record
    hdr: TAENMHDR;
    rcDraw: TRect;         //Draw rectangle.
    rcEdit: TRect;         //Edit client rectangle.
  end;
  TAENSETRECT = _AENSETRECT;


type
  _AENPAINT = record
    hdr: TAENMHDR;
    dwType: DWORD;            //See AEPNT_* defines.
    hDC: HDC;                 //Device context.
    ciMinDraw: TAECHARINDEX;  //First index in line to paint.
    ciMaxDraw: TAECHARINDEX;  //Last index in line to paint.
    nMinDrawOffset: INT_PTR;  //First character in line to paint (RichEdit offset).
    nMaxDrawOffset: INT_PTR;  //Last character in line to paint (RichEdit offset).
    ptMinDraw: TPoint;        //Left upper corner in client coordinates of first character in line to paint.
    ptMaxDraw: TPoint;        //Left upper corner in client coordinates of last character in line to paint.
  end;
  TAENPAINT = _AENPAINT;


type
  _AENMAXTEXT = record
    hdr: TAENMHDR;
    dwTextLimit: UINT_PTR;   //Current text limit.
  end;
  TAENMAXTEXT = _AENMAXTEXT;


type
  _AENPROGRESS = record
    hdr: TAENMHDR;
    dwType: DWORD;        //See AEPGS_* defines.
    dwTimeElapsed: DWORD; //Elapsed time since action was start.
    nCurrent: INT_PTR;    //Characters processed. Equal to zero, if first message.
    nMaximum: INT_PTR;    //Total number of characters. Equal to nCurrent member, if last message.
  end;
  TAENPROGRESS = _AENPROGRESS;


type
  _AENMODIFY = record
    hdr: TAENMHDR;
    bModified: BOOL;      //TRUE document state is set to modified, FALSE document state is set to unmodified.
  end;
  TAENMODIFY = _AENMODIFY;


type
  _AENSELCHANGE = record
    hdr: TAENMHDR;
    crSel: TAECHARRANGE;      //Current selection.
    ciCaret: TAECHARINDEX;  //Caret character index position.
    dwType: DWORD;         //See AESCT_* defines.
    bColumnSel: BOOL;       //Column selection.
    crRichSel: TCHARRANGE64; //Current selection (RichEdit offset).
  end;
  TAENSELCHANGE = _AENSELCHANGE;


type
  _AENTEXTCHANGE = record
    hdr: TAENMHDR;
    crSel: TAECHARRANGE;      //Current selection.
    ciCaret: TAECHARINDEX;  //Caret character index position.
    dwType: DWORD;         //See AETCT_* defines.
    bColumnSel: BOOL;       //Column selection.
    crRichSel: TCHARRANGE64; //Current selection (RichEdit offset).
  end;
  TAENTEXTCHANGE = _AENTEXTCHANGE;


type
  _AENTEXTINSERT = record
    hdr: TAENMHDR;
    crSel: TAECHARRANGE;       //Reserved.
    ciCaret: TAECHARINDEX;     //Reserved.
    dwType: DWORD;            //See AETCT_* defines.
    wpText: PWideChar;   //Text to insert.
    dwTextLen: UINT_PTR;      //Text length.
    nNewLine: Integer;            //See AELB_* defines.
    bColumnSel: BOOL;         //Column selection.
    dwInsertFlags: DWORD;     //See AEINST_* defines.
    crAkelRange: TAECHARRANGE; //AEN_TEXTINSERTBEGIN - text insert position or AEN_TEXTINSERTEND - text range after insertion.
    crRichRange: TCHARRANGE64; //AEN_TEXTINSERTBEGIN - text insert position or AEN_TEXTINSERTEND - text range after insertion (RichEdit offset).
  end;
  TAENTEXTINSERT = _AENTEXTINSERT;


type
  _AENTEXTDELETE = record
    hdr: TAENMHDR;
    crSel: TAECHARRANGE;       //Reserved.
    ciCaret: TAECHARINDEX;     //Reserved.
    dwType: DWORD;            //See AETCT_* defines.
    bColumnSel: BOOL;         //Column selection.
    dwDeleteFlags: DWORD;     //See AEDELT_* defines.
    crAkelRange: TAECHARRANGE; //AEN_TEXTDELETEBEGIN - text delete range or AEN_TEXTDELETEEND - text range after deletion.
    crRichRange: TCHARRANGE64; //AEN_TEXTDELETEBEGIN - text delete range or AEN_TEXTDELETEEND - text range after deletion (RichEdit offset).
  end;
  TAENTEXTDELETE = _AENTEXTDELETE;


type
  _AENPOINT = record
    hdr: TAENMHDR;
    dwType: DWORD;        //See AEPTT_* defines.
    lpPoint: PAEPOINT;    //Pointer to a AEPOINT structure. NULL if type is AEPTT_SETTEXT or AEPTT_STREAMIN.
  end;
  TAENPOINT = _AENPOINT;


type
  _AENDROPFILES = record
    hdr: TAENMHDR;
    hDrop: HDROP;         //Handle to the dropped files list (same as with WM_DROPFILES).
    ciChar: TAECHARINDEX;  //Character index at which the dropped files would be inserted.
  end;
  TAENDROPFILES = _AENDROPFILES;


type
  _AENDROPSOURCE = record
    hdr: TAENMHDR;
    nAction: Integer;         //See AEDS_* defines.
    dwEffect: DWORD;      //Cursor effect: DROPEFFECT_COPY, DROPEFFECT_MOVE or DROPEFFECT_NONE.
    dwDropResult: DWORD;  //Drop cResult. Valid if nAction equal to AEDS_SOURCEEND or AEDS_SOURCEDONE.
  end;
  TAENDROPSOURCE = _AENDROPSOURCE;


type
  _AENDROPTARGET = record
    hdr: TAENMHDR;
    nAction: Integer;         //See AEDT_* defines.
    pt: TPoint;            //Cursor position in screen coordinates.
    dwEffect: DWORD;      //Cursor effect: DROPEFFECT_COPY, DROPEFFECT_MOVE or DROPEFFECT_NONE.
  end;
  TAENDROPTARGET = _AENDROPTARGET;


type
  _AENLINK = record
    hdr: TAENMHDR;
    uMsg: UINT;           //Mouse message: WM_LBUTTONDBLCLK, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE, WM_RBUTTONDBLCLK, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_SETCURSOR.
    wParam: WPARAM;       //First parameter of a message.
    lParam: LPARAM;       //Second parameter of a message.
    crLink: TAECHARRANGE;  //Range of characters which contain URL text.
    nVisitCount: Integer;     //URL visit count. Variable must be incremented, if URL is opened.
  end;
  TAENLINK = _AENLINK;


type
  _AENMARKER = record
    hdr: TAENMHDR;
    dwType: DWORD; //Column marker cType.
    dwPos: DWORD;  //Column marker position.
    bMouse: BOOL;  //Column marker position is changed with mouse.
  end;
  TAENMARKER = _AENMARKER;



//// AkelEdit messages

//Error notifications
const AEN_ERRSPACE              = (WM_USER + 1001);  //0x7E9

//Window notifications
const AEN_SETFOCUS              = (WM_USER + 1026);  //0x802
const AEN_KILLFOCUS             = (WM_USER + 1027);  //0x803
const AEN_HSCROLL               = (WM_USER + 1028);  //0x804
const AEN_VSCROLL               = (WM_USER + 1029);  //0x805
const AEN_SETRECT               = (WM_USER + 1030);  //0x806
const AEN_PAINT                 = (WM_USER + 1031);  //0x807

//Text notifications
const AEN_MAXTEXT               = (WM_USER + 1050);  //0x81A
const AEN_PROGRESS              = (WM_USER + 1051);  //0x81B
const AEN_MODIFY                = (WM_USER + 1052);  //0x81C
const AEN_SELCHANGING           = (WM_USER + 1053);  //0x81D
const AEN_SELCHANGED            = (WM_USER + 1054);  //0x81E
const AEN_TEXTCHANGING          = (WM_USER + 1055);  //0x81F
const AEN_TEXTINSERTBEGIN       = (WM_USER + 1056);  //0x820
const AEN_TEXTINSERTEND         = (WM_USER + 1057);  //0x821
const AEN_TEXTDELETEBEGIN       = (WM_USER + 1058);  //0x822
const AEN_TEXTDELETEEND         = (WM_USER + 1059);  //0x823
const AEN_TEXTCHANGED           = (WM_USER + 1060);  //0x824
const AEN_POINT                 = (WM_USER + 1061);  //0x825

//Mouse notifications
const AEN_DROPFILES             = (WM_USER + 1076);  //0x834
const AEN_DROPSOURCE            = (WM_USER + 1077);  //0x835
const AEN_DROPTARGET            = (WM_USER + 1078);  //0x836
const AEN_LINK                  = (WM_USER + 1079);  //0x837
const AEN_MARKER                = (WM_USER + 1080);  //0x838

//RichEdit Unicode extension
const EM_REPLACESELA            = (WM_USER + 1901);
const EM_REPLACESELW            = (WM_USER + 1902);
const EM_GETSELTEXTA            = (WM_USER + 1905);
const EM_GETSELTEXTW            = (WM_USER + 1906);
const EM_GETLINEA               = (WM_USER + 1907);
const EM_GETLINEW               = (WM_USER + 1908);

//RichEdit x64 extension
const EM_GETSEL64               = (WM_USER + 1951);
const EM_EXGETSEL64             = (WM_USER + 1952);
const EM_EXSETSEL64             = (WM_USER + 1953);
const EM_FINDTEXTEX64           = (WM_USER + 1954);
const EM_FINDTEXTEX64A          = (WM_USER + 1955);
const EM_FINDTEXTEX64W          = (WM_USER + 1956);
const EM_GETTEXTRANGE64         = (WM_USER + 1961);
const EM_GETTEXTRANGE64A        = (WM_USER + 1962);
const EM_GETTEXTRANGE64W        = (WM_USER + 1963);
const EM_GETTEXTEX64            = (WM_USER + 1964);

//Text retrieval and modification
const AEM_EXSETTEXTA            = (WM_USER + 1999);
const AEM_EXSETTEXTW            = (WM_USER + 2000);
const AEM_SETTEXTA              = (WM_USER + 2001);
const AEM_SETTEXTW              = (WM_USER + 2002);
const AEM_APPENDTEXTA           = (WM_USER + 2003);
const AEM_APPENDTEXTW           = (WM_USER + 2004);
const AEM_REPLACESELA           = (WM_USER + 2005);
const AEM_REPLACESELW           = (WM_USER + 2006);
const AEM_GETTEXTRANGEA         = (WM_USER + 2007);
const AEM_GETTEXTRANGEW         = (WM_USER + 2008);
const AEM_STREAMIN              = (WM_USER + 2009);
const AEM_STREAMOUT             = (WM_USER + 2010);
const AEM_CANPASTE              = (WM_USER + 2011);
const AEM_PASTE                 = (WM_USER + 2012);
const AEM_CUT                   = (WM_USER + 2013);
const AEM_COPY                  = (WM_USER + 2014);
const AEM_CHECKCODEPAGE         = (WM_USER + 2015);
const AEM_FINDTEXTA             = (WM_USER + 2016);
const AEM_FINDTEXTW             = (WM_USER + 2017);
const AEM_ISMATCHA              = (WM_USER + 2018);
const AEM_ISMATCHW              = (WM_USER + 2019);
const AEM_KEYDOWN               = (WM_USER + 2020);
const AEM_INSERTCHAR            = (WM_USER + 2021);
const AEM_CHARAT                = (WM_USER + 2022);
const AEM_INPUTLANGUAGE         = (WM_USER + 2023);
const AEM_DRAGDROP              = (WM_USER + 2024);

//Undo and Redo
const AEM_CANUNDO               = (WM_USER + 2051);
const AEM_CANREDO               = (WM_USER + 2052);
const AEM_UNDO                  = (WM_USER + 2053);
const AEM_REDO                  = (WM_USER + 2054);
const AEM_EMPTYUNDOBUFFER       = (WM_USER + 2055);
const AEM_STOPGROUPTYPING       = (WM_USER + 2056);
const AEM_BEGINUNDOACTION       = (WM_USER + 2057);
const AEM_ENDUNDOACTION         = (WM_USER + 2058);
const AEM_LOCKCOLLECTUNDO       = (WM_USER + 2059);
const AEM_GETUNDOLIMIT          = (WM_USER + 2060);
const AEM_SETUNDOLIMIT          = (WM_USER + 2061);
const AEM_GETMODIFY             = (WM_USER + 2062);
const AEM_SETMODIFY             = (WM_USER + 2063);
const AEM_UNDOBUFFERSIZE        = (WM_USER + 2064);
const AEM_ISRANGEMODIFIED       = (WM_USER + 2065);
const AEM_DETACHUNDO            = (WM_USER + 2066);
const AEM_ATTACHUNDO            = (WM_USER + 2067);

//Text coordinates
const AEM_EXGETSEL              = (WM_USER + 2099);
const AEM_EXSETSEL              = (WM_USER + 2100);
const AEM_GETSEL                = (WM_USER + 2101);
const AEM_SETSEL                = (WM_USER + 2102);
const AEM_GETCOLUMNSEL          = (WM_USER + 2103);
const AEM_UPDATESEL             = (WM_USER + 2104);
const AEM_GETLINENUMBER         = (WM_USER + 2105);
const AEM_GETINDEX              = (WM_USER + 2106);
const AEM_GETLINEINDEX          = (WM_USER + 2107);
const AEM_INDEXUPDATE           = (WM_USER + 2108);
const AEM_INDEXCOMPARE          = (WM_USER + 2109);
const AEM_INDEXSUBTRACT         = (WM_USER + 2110);
const AEM_INDEXOFFSET           = (WM_USER + 2111);
const AEM_INDEXTORICHOFFSET     = (WM_USER + 2112);
const AEM_RICHOFFSETTOINDEX     = (WM_USER + 2113);
const AEM_GETRICHOFFSET         = (WM_USER + 2114);
const AEM_GETWRAPLINE           = (WM_USER + 2118);
const AEM_GETUNWRAPLINE         = (WM_USER + 2119);
const AEM_GETNEXTBREAK          = (WM_USER + 2120);
const AEM_GETPREVBREAK          = (WM_USER + 2121);
const AEM_ISDELIMITER           = (WM_USER + 2122);
const AEM_INDEXTOCOLUMN         = (WM_USER + 2123);
const AEM_COLUMNTOINDEX         = (WM_USER + 2124);
const AEM_INDEXINURL            = (WM_USER + 2125);
const AEM_ADDPOINT              = (WM_USER + 2141);
const AEM_DELPOINT              = (WM_USER + 2142);
const AEM_GETPOINTSTACK         = (WM_USER + 2143);

//Screen coordinates
const AEM_CHARFROMGLOBALPOS     = (WM_USER + 2149);
const AEM_GLOBALPOSFROMCHAR     = (WM_USER + 2150);
const AEM_CHARFROMPOS           = (WM_USER + 2151);
const AEM_POSFROMCHAR           = (WM_USER + 2152);
const AEM_GETRECT               = (WM_USER + 2153);
const AEM_SETRECT               = (WM_USER + 2154);
const AEM_GETSCROLLPOS          = (WM_USER + 2155);
const AEM_SETSCROLLPOS          = (WM_USER + 2156);
const AEM_SCROLL                = (WM_USER + 2157);
const AEM_LINESCROLL            = (WM_USER + 2158);
const AEM_SCROLLTOPOINT         = (WM_USER + 2159);
const AEM_LOCKSCROLL            = (WM_USER + 2161);
const AEM_GETCHARSIZE           = (WM_USER + 2164);
const AEM_GETSTRWIDTH           = (WM_USER + 2165);
const AEM_GETCARETPOS           = (WM_USER + 2166);
const AEM_GETCARETHORZINDENT    = (WM_USER + 2167);
const AEM_SETCARETHORZINDENT    = (WM_USER + 2168);
const AEM_CONVERTPOINT          = (WM_USER + 2169);
const AEM_POINTONMARGIN         = (WM_USER + 2170);
const AEM_POINTONSELECTION      = (WM_USER + 2171);
const AEM_POINTONURL            = (WM_USER + 2172);
const AEM_LINEFROMVPOS          = (WM_USER + 2173);
const AEM_VPOSFROMLINE          = (WM_USER + 2174);
const AEM_GETMOUSESTATE         = (WM_USER + 2175);

//Options
const AEM_CONTROLCLASS          = (WM_USER + 2199);
const AEM_CONTROLVERSION        = (WM_USER + 2200);
const AEM_GETEVENTMASK          = (WM_USER + 2201);
const AEM_SETEVENTMASK          = (WM_USER + 2202);
const AEM_GETOPTIONS            = (WM_USER + 2203);
const AEM_SETOPTIONS            = (WM_USER + 2204);
const AEM_GETNEWLINE            = (WM_USER + 2205);
const AEM_SETNEWLINE            = (WM_USER + 2206);
const AEM_GETCOLORS             = (WM_USER + 2207);
const AEM_SETCOLORS             = (WM_USER + 2208);
const AEM_EXGETOPTIONS          = (WM_USER + 2209);
const AEM_EXSETOPTIONS          = (WM_USER + 2210);
const AEM_GETCARETWIDTH         = (WM_USER + 2213);
const AEM_SETCARETWIDTH         = (WM_USER + 2214);
const AEM_GETTABSTOP            = (WM_USER + 2215);
const AEM_SETTABSTOP            = (WM_USER + 2216);
const AEM_GETWORDWRAP           = (WM_USER + 2217);
const AEM_SETWORDWRAP           = (WM_USER + 2218);
const AEM_GETWORDDELIMITERS     = (WM_USER + 2219);
const AEM_SETWORDDELIMITERS     = (WM_USER + 2220);
const AEM_GETWRAPDELIMITERS     = (WM_USER + 2221);
const AEM_SETWRAPDELIMITERS     = (WM_USER + 2222);
const AEM_GETURLLEFTDELIMITERS  = (WM_USER + 2223);
const AEM_SETURLLEFTDELIMITERS  = (WM_USER + 2224);
const AEM_GETURLRIGHTDELIMITERS = (WM_USER + 2225);
const AEM_SETURLRIGHTDELIMITERS = (WM_USER + 2226);
const AEM_GETURLPREFIXES        = (WM_USER + 2227);
const AEM_SETURLPREFIXES        = (WM_USER + 2228);
const AEM_GETURLMAXLENGTH       = (WM_USER + 2229);
const AEM_SETURLMAXLENGTH       = (WM_USER + 2230);
const AEM_GETWORDBREAK          = (WM_USER + 2231);
const AEM_SETWORDBREAK          = (WM_USER + 2232);
const AEM_GETMARKER             = (WM_USER + 2233);
const AEM_SETMARKER             = (WM_USER + 2234);
const AEM_GETLINEGAP            = (WM_USER + 2235);
const AEM_SETLINEGAP            = (WM_USER + 2236);
const AEM_GETTEXTLIMIT          = (WM_USER + 2237);
const AEM_SETTEXTLIMIT          = (WM_USER + 2238);
const AEM_GETFONT               = (WM_USER + 2239);
const AEM_GETALTLINE            = (WM_USER + 2240);
const AEM_SETALTLINE            = (WM_USER + 2241);
const AEM_GETCHARCOLORS         = (WM_USER + 2242);
const AEM_SCROLLCARETOPTIONS    = (WM_USER + 2243);

//Draw
const AEM_SHOWSCROLLBAR         = (WM_USER + 2351);
const AEM_UPDATESCROLLBAR       = (WM_USER + 2352);
const AEM_UPDATECARET           = (WM_USER + 2353);
const AEM_UPDATESIZE            = (WM_USER + 2354);
const AEM_LOCKUPDATE            = (WM_USER + 2355);
const AEM_LOCKERASERECT         = (WM_USER + 2356);
const AEM_GETERASERECT          = (WM_USER + 2357);
const AEM_SETERASERECT          = (WM_USER + 2358);
const AEM_HIDESELECTION         = (WM_USER + 2361);
const AEM_REDRAWLINERANGE       = (WM_USER + 2362);
const AEM_GETBACKGROUNDIMAGE    = (WM_USER + 2366);
const AEM_SETBACKGROUNDIMAGE    = (WM_USER + 2367);

//Folding
const AEM_GETFOLDSTACK          = (WM_USER + 2381);
const AEM_GETFOLDCOUNT          = (WM_USER + 2382);
const AEM_ADDFOLD               = (WM_USER + 2383);
const AEM_DELETEFOLD            = (WM_USER + 2384);
const AEM_ISFOLDVALID           = (WM_USER + 2385);
const AEM_FINDFOLD              = (WM_USER + 2386);
const AEM_NEXTFOLD              = (WM_USER + 2387);
const AEM_PREVFOLD              = (WM_USER + 2388);
const AEM_ISLINECOLLAPSED       = (WM_USER + 2389);
const AEM_COLLAPSELINE          = (WM_USER + 2390);
const AEM_COLLAPSEFOLD          = (WM_USER + 2391);
const AEM_UPDATEFOLD            = (WM_USER + 2392);

//Document
const AEM_CREATEDOCUMENT        = (WM_USER + 2401);
const AEM_DELETEDOCUMENT        = (WM_USER + 2402);
const AEM_GETDOCUMENT           = (WM_USER + 2403);
const AEM_SETDOCUMENT           = (WM_USER + 2404);
const AEM_GETDOCUMENTPROC       = (WM_USER + 2405);
const AEM_GETDOCUMENTWINDOW     = (WM_USER + 2406);
const AEM_SENDMESSAGE           = (WM_USER + 2407);

//Clone
const AEM_ADDCLONE              = (WM_USER + 2421);
const AEM_DELCLONE              = (WM_USER + 2422);
const AEM_GETMASTER             = (WM_USER + 2423);
const AEM_GETCLONE              = (WM_USER + 2424);

//Print
const AEM_STARTPRINTDOC         = (WM_USER + 2451);
const AEM_PRINTPAGE             = (WM_USER + 2452);
const AEM_ENDPRINTDOC           = (WM_USER + 2453);

//Highlight
const AEM_HLCREATETHEMEA        = (WM_USER + 2501);
const AEM_HLCREATETHEMEW        = (WM_USER + 2502);
const AEM_HLGETTHEMEA           = (WM_USER + 2503);
const AEM_HLGETTHEMEW           = (WM_USER + 2504);
const AEM_HLGETTHEMENAMEA       = (WM_USER + 2505);
const AEM_HLGETTHEMENAMEW       = (WM_USER + 2506);
const AEM_HLGETTHEMESTACK       = (WM_USER + 2507);
const AEM_HLTHEMEEXISTS         = (WM_USER + 2508);
const AEM_HLSETTHEME            = (WM_USER + 2509);
const AEM_HLDELETETHEME         = (WM_USER + 2510);
const AEM_HLDELETETHEMEALL      = (WM_USER + 2511);
const AEM_HLADDDELIMITERA       = (WM_USER + 2521);
const AEM_HLADDDELIMITERW       = (WM_USER + 2522);
const AEM_HLDELETEDELIMITER     = (WM_USER + 2523);
const AEM_HLADDWORDA            = (WM_USER + 2531);
const AEM_HLADDWORDW            = (WM_USER + 2532);
const AEM_HLDELETEWORD          = (WM_USER + 2533);
const AEM_HLADDQUOTEA           = (WM_USER + 2541);
const AEM_HLADDQUOTEW           = (WM_USER + 2542);
const AEM_HLDELETEQUOTE         = (WM_USER + 2543);
const AEM_HLADDMARKTEXTA        = (WM_USER + 2551);
const AEM_HLADDMARKTEXTW        = (WM_USER + 2552);
const AEM_HLDELETEMARKTEXT      = (WM_USER + 2553);
const AEM_HLADDMARKRANGE        = (WM_USER + 2561);
const AEM_HLDELETEMARKRANGE     = (WM_USER + 2562);
const AEM_HLGETHIGHLIGHT        = (WM_USER + 2571);
const AEM_HLGETOPTIONS          = (WM_USER + 2581);
const AEM_HLSETOPTIONS          = (WM_USER + 2582);


//// RichEdit messages

(*

//// RichEdit messages

AkelEdit can emulate RichEdit 3.0 and support the following messages:

EN_CHANGE
EN_DRAGDROPDONE
EN_DROPFILES
EN_ERRSPACE
EN_HSCROLL
EN_KILLFOCUS
EN_LINK
EN_MAXTEXT
EN_MSGFILTER
EN_SELCHANGE
EN_SETFOCUS
EN_VSCROLL

EM_AUTOURLDETECT
EM_CANPASTE
EM_CANREDO
EM_CANUNDO
EM_CHARFROMPOS
EM_EMPTYUNDOBUFFER
EM_EXGETSEL
EM_EXLIMITTEXT
EM_EXLINEFROMCHAR
EM_EXSETSEL
EM_FINDTEXT
EM_FINDTEXTEX
EM_FINDTEXTEXW
EM_FINDTEXTW
EM_FINDWORDBREAK
EM_GETAUTOURLDETECT
EM_GETEVENTMASK
EM_GETFIRSTVISIBLELINE
EM_GETLIMITTEXT
EM_GETLINE
EM_GETLINECOUNT
EM_GETMARGINS
EM_GETMODIFY
EM_GETOPTIONS
EM_GETRECT
EM_GETSCROLLPOS
EM_GETSEL
EM_GETSELTEXT
EM_GETTEXTEX
EM_GETTEXTLENGTHEX
EM_GETTEXTRANGE
EM_GETTHUMB
EM_HIDESELECTION
EM_LIMITTEXT
EM_LINEFROMCHAR
EM_LINEINDEX
EM_LINELENGTH
EM_LINESCROLL
EM_POSFROMCHAR
EM_REDO
EM_REPLACESEL
EM_SCROLL
EM_SCROLLCARET
EM_SELECTIONTYPE
EM_SETBKGNDCOLOR
EM_SETEVENTMASK
EM_SETLIMITTEXT
EM_SETMARGINS
EM_SETMODIFY
EM_SETOPTIONS
EM_SETREADONLY
EM_SETRECT
EM_SETRECTNP
EM_SETSCROLLPOS
EM_SETSEL
EM_SETTEXTEX
EM_SETUNDOLIMIT
EM_SHOWSCROLLBAR
EM_STOPGROUPTYPING
EM_STREAMIN
EM_STREAMOUT
EM_UNDO

Additional messages for Unicode support:
EM_REPLACESELA
EM_REPLACESELW
EM_GETTEXTRANGEA
EM_GETTEXTRANGEW
EM_GETSELTEXTA
EM_GETSELTEXTW
EM_GETLINEA
EM_GETLINEW

Additional messages for x64 support:
EM_GETSEL64
EM_EXGETSEL64
EM_EXSETSEL64
EM_FINDTEXTEX64
EM_FINDTEXTEX64A
EM_FINDTEXTEX64W
EM_GETTEXTRANGE64
EM_GETTEXTRANGE64A
EM_GETTEXTRANGE64W
EM_GETTEXTEX64

*)

// Messages manual: look AkelEdit.h

//// UNICODE define
{$ifndef UNICODE}
const
  AES_AKELEDIT = AES_AKELEDITA;
  AES_RICHEDIT20 = AES_RICHEDIT20A;

type
  TTEXTRANGE64 = TTEXTRANGE64A;
  TFINDTEXTEX64 = TFINDTEXTEX64A;

  TAEAPPENDTEXT = TAEAPPENDTEXTA;
  TAEREPLACESEL = TAEREPLACESELA;
  TAETEXTRANGE = TAETEXTRANGEA;
  TAEFINDTEXT = TAEFINDTEXTA;
  TAEDELIMITEM = TAEDELIMITEMA;
  TAEWORDITEM = TAEWORDITEMA;
  TAEQUOTEITEM = TAEQUOTEITEMA;
  TAEMARKTEXTITEM = TAEMARKTEXTITEMA;

const
  AEM_SETTEXT = AEM_SETTEXTA;
  AEM_APPENDTEXT = AEM_APPENDTEXTA;
  AEM_REPLACESEL = AEM_REPLACESELA;
  AEM_GETTEXTRANGE = AEM_GETTEXTRANGEA;
  AEM_FINDTEXT = AEM_FINDTEXTA;
  AEM_ISMATCH = AEM_ISMATCHA;
  AEM_HLCREATETHEME = AEM_HLCREATETHEMEA;
  AEM_HLGETTHEME = AEM_HLGETTHEMEA;
  AEM_HLGETTHEMENAME = AEM_HLGETTHEMENAMEA;
  AEM_HLADDDELIMITER = AEM_HLADDDELIMITERA;
  AEM_HLADDWORD = AEM_HLADDWORDA;
  AEM_HLADDQUOTE = AEM_HLADDQUOTEA;
  AEM_HLADDMARKTEXT = AEM_HLADDMARKTEXTA;
{$else}
const
  AES_AKELEDIT = AES_AKELEDITW;
  AES_RICHEDIT20 = AES_RICHEDIT20W;

type
  TTEXTRANGE64 = TTEXTRANGE64W;
  TFINDTEXTEX64 = TFINDTEXTEX64W;

  TAEAPPENDTEXT = TAEAPPENDTEXTW;
  TAEREPLACESEL = TAEREPLACESELW;
  TAETEXTRANGE = TAETEXTRANGEW;
  TAEFINDTEXT = TAEFINDTEXTW;
  TAEDELIMITEM = TAEDELIMITEMW;
  TAEWORDITEM = TAEWORDITEMW;
  TAEQUOTEITEM = TAEQUOTEITEMW;
  TAEMARKTEXTITEM = TAEMARKTEXTITEMW;

const
  AEM_SETTEXT = AEM_SETTEXTW;
  AEM_APPENDTEXT = AEM_APPENDTEXTW;
  AEM_REPLACESEL = AEM_REPLACESELW;
  AEM_GETTEXTRANGE = AEM_GETTEXTRANGEW;
  AEM_FINDTEXT = AEM_FINDTEXTW;
  AEM_ISMATCH = AEM_ISMATCHW;
  AEM_HLCREATETHEME = AEM_HLCREATETHEMEW;
  AEM_HLGETTHEME = AEM_HLGETTHEMEW;
  AEM_HLGETTHEMENAME = AEM_HLGETTHEMENAMEW;
  AEM_HLADDDELIMITER = AEM_HLADDDELIMITERW;
  AEM_HLADDWORD = AEM_HLADDWORDW;
  AEM_HLADDQUOTE = AEM_HLADDQUOTEW;
  AEM_HLADDMARKTEXT = AEM_HLADDMARKTEXTW;
{$endif}

// Fr0sT: inlines available since D2006
{$IF CompilerVersion >= 16}
  {$DEFINE INLINES}
{$IFEND}

function AEC_IsSurrogate(c: WideChar): Boolean;     {$IFDEF INLINES}inline;{$ENDIF}
function AEC_IsHighSurrogate(c: WideChar): Boolean; {$IFDEF INLINES}inline;{$ENDIF}
function AEC_IsLowSurrogate(c: WideChar): Boolean;  {$IFDEF INLINES}inline;{$ENDIF}
function AEC_ScalarFromSurrogate(high, low: WideChar): UCS4Char;
function AEC_HighSurrogateFromScalar(c: UCS4Char): WideChar;
function AEC_LowSurrogateFromScalar(c: UCS4Char): WideChar;

function AEC_CopyChar(wszTarget: PWideChar; dwTargetSize: DWORD; const wpSource: PWideChar): Integer;
function AEC_IndexInc(var ciChar: TAECHARINDEX): Integer;
function AEC_IndexDec(var ciChar: TAECHARINDEX): Integer;
function AEC_IndexLen(const ciChar: TAECHARINDEX): Integer;
function AEC_IndexCompare(const ciChar1, ciChar2: TAECHARINDEX): Integer;
function AEC_IndexCompareEx(const ciChar1, ciChar2: TAECHARINDEX): Integer;
function AEC_NextLine(var ciChar: TAECHARINDEX): PAELINEDATA;
function AEC_PrevLine(var ciChar: TAECHARINDEX): PAELINEDATA;
function AEC_NextLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
function AEC_PrevLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
function AEC_NextChar(var ciChar: TAECHARINDEX): PAELINEDATA;
function AEC_PrevChar(var ciChar: TAECHARINDEX): PAELINEDATA;
function AEC_NextCharEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
function AEC_PrevCharEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
function AEC_NextCharInLine(var ciChar: TAECHARINDEX): PAELINEDATA;
function AEC_PrevCharInLine(var ciChar: TAECHARINDEX): PAELINEDATA;
function AEC_NextCharInLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
function AEC_PrevCharInLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
function AEC_ValidCharInLine(var ciChar: TAECHARINDEX): PAELINEDATA;
function AEC_WrapLineBegin(var ciChar: TAECHARINDEX): Integer;
function AEC_WrapLineEnd(var ciChar: TAECHARINDEX): Integer;
function AEC_WrapLineBeginEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): Integer;
function AEC_WrapLineEndEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): Integer;
function AEC_CharAtIndex(var ciChar: TAECHARINDEX): Integer;
function AEC_IsCharInSelection(var ciChar: TAECHARINDEX): Boolean;
function AEC_IsFirstCharInLine(var ciChar: TAECHARINDEX): Boolean;
function AEC_IsLastCharInLine(var ciChar: TAECHARINDEX): Boolean;
function AEC_IsFirstCharInFile(var ciChar: TAECHARINDEX): Boolean;
function AEC_IsLastCharInFile(var ciChar: TAECHARINDEX): Boolean;
function AEC_NextFold(var lpFold: PAEFOLD; bRecursive: Boolean): PAEFOLD;
function AEC_PrevFold(var lpFold: PAEFOLD; bRecursive: Boolean): PAEFOLD;

implementation

//// AkelEdit functions

function AEC_IsSurrogate(c: WideChar): Boolean;
begin
  Result := (c >= #$D800) and (c <= #$DFFF);
end;

function AEC_IsHighSurrogate(c: WideChar): Boolean;
begin
  Result := (c >= #$D800) and (c <= #$DBFF);
end;

function AEC_IsLowSurrogate(c: WideChar): Boolean;
begin
  Result := (c >= #$DC00) and (c <= #$DFFF);
end;

function AEC_ScalarFromSurrogate(high, low: WideChar): UCS4Char;
begin
//  ((((high) - $D800) * $400) + ((low) - $DC00) + $10000)
  Result := (Word(high) - $D800)*$400 + (Word(low) - $DC00) + $10000;
end;

function AEC_HighSurrogateFromScalar(c: UCS4Char): WideChar;
begin
  Result := WideChar((c - $10000) div $400 + $D800);
end;

function AEC_LowSurrogateFromScalar(c: UCS4Char): WideChar;
begin
  Result := WideChar((c - $10000) mod $400 + $DC00);
end;

function AEC_CopyChar(wszTarget: PWideChar; dwTargetSize: DWORD; const wpSource: PWideChar): Integer;
begin
  Result := 0;
  if AEC_IsSurrogate(wpSource^) then
  begin
    if dwTargetSize >= 2 then
    begin
      if AEC_IsHighSurrogate(wpSource^) and AEC_IsLowSurrogate((wpSource + 1)^) then
      begin
        if wszTarget <> nil then
        begin
          wszTarget^ := wpSource^;
          (wszTarget + 1)^ := (wpSource + 1)^;
        end;
        Result := 2;
      end;
    end;
  end
  else
  begin
    if wszTarget <> nil then
      wszTarget^ := wpSource^;
    Result := 1;
  end;
end;

function AEC_IndexInc(var ciChar: TAECHARINDEX): Integer;
begin
  Result := 1;

  if (ciChar.nCharInLine >= 0) and (ciChar.nCharInLine + 1 < ciChar.lpLine.nLineLen) then
    if AEC_IsHighSurrogate(ciChar.lpLine.wpLine[ciChar.nCharInLine]) and
       AEC_IsLowSurrogate(ciChar.lpLine.wpLine[ciChar.nCharInLine + 1]) then
         Result := 2;

  Inc(ciChar.nCharInLine, Result);
end;

function AEC_IndexDec(var ciChar: TAECHARINDEX): Integer;
begin
  Result := 1;

  if (ciChar.nCharInLine - 2 >= 0) and (ciChar.nCharInLine - 1 < ciChar.lpLine.nLineLen) then
    if AEC_IsLowSurrogate(ciChar.lpLine.wpLine[ciChar.nCharInLine - 1]) and
       AEC_IsHighSurrogate(ciChar.lpLine.wpLine[ciChar.nCharInLine - 2]) then
         Result := 2;

  Dec(ciChar.nCharInLine, Result);
end;

function AEC_IndexLen(const ciChar: TAECHARINDEX): Integer;
begin
  Result := 1;
  if (ciChar.nCharInLine >= 0) and (ciChar.nCharInLine + 1 < ciChar.lpLine.nLineLen) then
    if AEC_IsHighSurrogate(ciChar.lpLine.wpLine[ciChar.nCharInLine]) and
       AEC_IsLowSurrogate(ciChar.lpLine.wpLine[ciChar.nCharInLine + 1]) then
         Result := 2;
end;

function AEC_IndexCompare(const ciChar1, ciChar2: TAECHARINDEX): Integer;
begin
  if (ciChar1.nLine = ciChar2.nLine) and
     (ciChar1.nCharInLine = ciChar2.nCharInLine) then
     Result := 0
  else
  if (ciChar1.nLine < ciChar2.nLine) or
     ((ciChar1.nLine = ciChar2.nLine) and (ciChar1.nCharInLine < ciChar2.nCharInLine)) then
    Result := -1
  else
    Result := 1;
end;

function AEC_IndexCompareEx(const ciChar1, ciChar2: TAECHARINDEX): Integer;
begin
  if
    ( (ciChar1.nLine = ciChar2.nLine) and (ciChar1.nCharInLine = ciChar2.nCharInLine) ) or
    (
      (ciChar1.lpLine <> nil) and (ciChar2.lpLine <> nil) and
      (
        ( (ciChar1.lpLine.next = ciChar2.lpLine) and (ciChar1.lpLine.nLineBreak = AELB_WRAP) and (ciChar1.nCharInLine = ciChar1.lpLine.nLineLen) and (ciChar2.nCharInLine = 0) ) or
        ( (ciChar2.lpLine.next = ciChar1.lpLine) and (ciChar2.lpLine.nLineBreak = AELB_WRAP) and (ciChar2.nCharInLine = ciChar2.lpLine.nLineLen) and (ciChar1.nCharInLine = 0) )
      ))
    then Result := 0
  else if
    (ciChar1.nLine < ciChar2.nLine) or
    ( (ciChar1.nLine = ciChar2.nLine) and (ciChar1.nCharInLine < ciChar2.nCharInLine) )
    then Result := -1
  else
    Result := 1;
end;

function AEC_NextLine(var ciChar: TAECHARINDEX): PAELINEDATA;
begin
  if ciChar.lpLine <> nil then
  begin
    Inc(ciChar.nLine);
    ciChar.lpLine := ciChar.lpLine.next;
    ciChar.nCharInLine :=0;
  end;
  Result := ciChar.lpLine;
end;

function AEC_PrevLine(var ciChar: TAECHARINDEX): PAELINEDATA;
begin
  if ciChar.lpLine <> nil then
  begin
    Dec(ciChar.nLine);
    ciChar.lpLine := ciChar.lpLine.prev;
    if ciChar.lpLine <> nil then
      ciChar.nCharInLine := ciChar.lpLine.nLineLen
    else
      ciChar.nCharInLine := 0;
  end;
  Result := ciChar.lpLine;
end;

function AEC_NextLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
var ciTmp: TAECHARINDEX;
begin
  ciTmp := ciIn;
  if AEC_NextLine(ciTmp) <> nil then
  begin
    ciOut := ciTmp;
    Result := ciOut.lpLine;
  end
  else
  begin
    ciOut := ciIn;
    Result := nil;
  end
end;

function AEC_PrevLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
var ciTmp: TAECHARINDEX;
begin
  ciTmp := ciIn;
  if AEC_PrevLine(ciTmp) <> nil then
  begin
    ciOut := ciTmp;
    Result := ciOut.lpLine;
  end
  else
  begin
    ciOut := ciIn;
    Result := nil;
  end
end;

function AEC_NextChar(var ciChar: TAECHARINDEX): PAELINEDATA;
begin
  AEC_IndexInc(ciChar);
  if ciChar.nCharInLine >= ciChar.lpLine.nLineLen then
    if (ciChar.nCharInLine > ciChar.lpLine.nLineLen) or
       (ciChar.lpLine.nLineBreak = AELB_WRAP) then
      AEC_NextLine(ciChar);
  Result := ciChar.lpLine;
end;

function AEC_PrevChar(var ciChar: TAECHARINDEX): PAELINEDATA;
begin
  AEC_IndexDec(ciChar);
  if ciChar.nCharInLine < 0 then
    if AEC_PrevLine(ciChar) <> nil then
      if ciChar.lpLine.nLineBreak = AELB_WRAP then
        AEC_IndexDec(ciChar);
  Result := ciChar.lpLine;
end;

function AEC_NextCharEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
var ciTmp: TAECHARINDEX;
begin
  ciTmp := ciIn;
  if AEC_NextChar(ciTmp) <> nil then
  begin
    ciOut := ciTmp;
    Result := ciOut.lpLine;
  end
  else
  begin
    ciOut := ciIn;
    Result := nil;
  end
end;

function AEC_PrevCharEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
var ciTmp: TAECHARINDEX;
begin
  ciTmp := ciIn;
  if AEC_PrevChar(ciTmp) <> nil then
  begin
    ciOut := ciTmp;
    Result := ciOut.lpLine;
  end
  else
  begin
    ciOut := ciIn;
    Result := nil;
  end
end;

function AEC_NextCharInLine(var ciChar: TAECHARINDEX): PAELINEDATA;
begin
  AEC_IndexInc(ciChar);
  if ciChar.nCharInLine >= ciChar.lpLine.nLineLen then
    if ciChar.lpLine.nLineBreak = AELB_WRAP then
      AEC_NextLine(ciChar)
    else
    begin
      Result := nil;
      Exit;
    end;
  Result := ciChar.lpLine;
end;

function AEC_PrevCharInLine(var ciChar: TAECHARINDEX): PAELINEDATA;
begin
  if ciChar.nCharInLine = 0 then
    if (ciChar.lpLine.prev = nil) or (ciChar.lpLine.prev.nLineBreak <> AELB_WRAP) then
    begin
      Result := nil;
      Exit;
    end;
  AEC_PrevChar(ciChar);
  Result := ciChar.lpLine;
end;

function AEC_NextCharInLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
var ciTmp: TAECHARINDEX;
begin
  ciTmp := ciIn;
  if AEC_NextCharInLine(ciTmp) <> nil then
  begin
    ciOut := ciTmp;
    Result := ciOut.lpLine;
  end
  else
  begin
    ciOut := ciIn;
    Result := nil;
  end
end;

function AEC_PrevCharInLineEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): PAELINEDATA;
var ciTmp: TAECHARINDEX;
begin
  ciTmp := ciIn;
  if AEC_PrevCharInLine(ciTmp) <> nil then
  begin
    ciOut := ciTmp;
    Result := ciOut.lpLine;
  end
  else
  begin
    ciOut := ciIn;
    Result := nil;
  end
end;

function AEC_ValidCharInLine(var ciChar: TAECHARINDEX): PAELINEDATA;
begin
  if ciChar.nCharInLine >= ciChar.lpLine.nLineLen then
    if ciChar.lpLine.nLineBreak = AELB_WRAP then
      AEC_NextLine(ciChar)
    else
      ciChar.nCharInLine := ciChar.lpLine.nLineLen
  else if ciChar.nCharInLine < 0 then
    ciChar.nCharInLine := 0;
  Result := ciChar.lpLine;
end;

function AEC_WrapLineBegin(var ciChar: TAECHARINDEX): Integer;
begin
  Result := ciChar.nCharInLine;
  if ciChar.lpLine <> nil then
  begin
    while ciChar.lpLine.prev <> nil do
    begin
      if ciChar.lpLine.prev.nLineBreak <> AELB_WRAP then
        Break;
      Dec(ciChar.nLine);
      ciChar.lpLine := ciChar.lpLine.prev;
      Inc(Result, ciChar.lpLine.nLineLen);
    end;
  end;
  ciChar.nCharInLine := 0;
end;

function AEC_WrapLineEnd(var ciChar: TAECHARINDEX): Integer;
begin
  Result := ciChar.lpLine.nLineLen - ciChar.nCharInLine;
  while ciChar.lpLine <> nil do
  begin
    if ciChar.lpLine.nLineBreak <> AELB_WRAP then
      Break;
    Inc(ciChar.nLine);
    ciChar.lpLine := ciChar.lpLine.next;
    Inc(Result, ciChar.lpLine.nLineLen);
  end;
  ciChar.nCharInLine := ciChar.lpLine.nLineLen;
end;

function AEC_WrapLineBeginEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): Integer;
begin
  ciOut := ciIn;
  Result := AEC_WrapLineBegin(ciOut);
end;

function AEC_WrapLineEndEx(const ciIn: TAECHARINDEX; var ciOut: TAECHARINDEX): Integer;
begin
  ciOut := ciIn;
  Result := AEC_WrapLineEnd(ciOut);
end;

// Fr0sT: Returns WideChar if >= 0 and line break type if < 0
function AEC_CharAtIndex(var ciChar: TAECHARINDEX): Integer;
begin
  if ciChar.nCharInLine >= ciChar.lpLine.nLineLen then
    if ciChar.lpLine.nLineBreak = AELB_WRAP then
    begin
      Result := Integer(ciChar.lpLine.next.wpLine^);
      if AEC_IsHighSurrogate(WideChar(Result)) then
        Result := AEC_ScalarFromSurrogate(WideChar(Result), (ciChar.lpLine.next.wpLine + 1)^);
      Exit;
    end
    else
      Result := -ciChar.lpLine.nLineBreak
  else
  begin
    Result := Integer( (ciChar.lpLine.wpLine + ciChar.nCharInLine)^ );
    if AEC_IsHighSurrogate(WideChar(Result)) then
      Result := AEC_ScalarFromSurrogate(WideChar(Result), (ciChar.lpLine.next.wpLine + ciChar.nCharInLine + 1)^);
    Exit;
  end;
end;

function AEC_IsCharInSelection(var ciChar: TAECHARINDEX): Boolean;
begin
  Result :=
    (ciChar.lpLine.nSelStart <= ciChar.nCharInLine) and
    (ciChar.nCharInLine < ciChar.lpLine.nSelEnd);
end;

function AEC_IsFirstCharInLine(var ciChar: TAECHARINDEX): Boolean;
begin
  Result :=
    (ciChar.nCharInLine = 0) and
    ((ciChar.lpLine.prev <> nil) or (ciChar.lpLine.prev.nLineBreak <> AELB_WRAP));
end;

function AEC_IsLastCharInLine(var ciChar: TAECHARINDEX): Boolean;
begin
  Result :=
    (ciChar.nCharInLine = ciChar.lpLine.nLineLen) and
    (ciChar.lpLine.nLineBreak <> AELB_WRAP);
end;

function AEC_IsFirstCharInFile(var ciChar: TAECHARINDEX): Boolean;
begin
  Result :=
    (ciChar.nCharInLine = 0) and
    (ciChar.lpLine.prev <> nil);
end;

function AEC_IsLastCharInFile(var ciChar: TAECHARINDEX): Boolean;
begin
  Result :=
    (ciChar.nCharInLine = ciChar.lpLine.nLineLen) and
    (ciChar.lpLine.next <> nil);
end;

function AEC_NextFold(var lpFold: PAEFOLD; bRecursive: Boolean): PAEFOLD;
begin
  if lpFold <> nil then
  begin
    if bRecursive then
      if lpFold.firstChild <> nil then
      begin
        Result := lpFold.firstChild;
        Exit;
      end;
    repeat
      if lpFold.next <> nil then
      begin
        Result := lpFold.next;
        Exit;
      end;
      lpFold := lpFold.parent;
    until lpFold = nil;
  end;
  Result := lpFold;
end;

function AEC_PrevFold(var lpFold: PAEFOLD; bRecursive: Boolean): PAEFOLD;
begin
  if lpFold <> nil then
  begin
    if bRecursive then
      if lpFold.lastChild <> nil then
      begin
        Result := lpFold.lastChild;
        Exit;
      end;
    repeat
      if lpFold.prev <> nil then
      begin
        Result := lpFold.prev;
        Exit;
      end;
      lpFold := lpFold.parent;
    until lpFold = nil;
  end;
  Result := lpFold;
end;

end.
