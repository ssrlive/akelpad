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
= Tested on RAD Studio 2010 but compiles on D7 and should =
= work on other versions also.                            =

**************************************************************************)

unit AkelEdit_h;

interface

uses
  Windows, Messages, RichEdit,
  AkelDLL_h;

//// Defines

const AES_AKELEDITCLASSA = AnsiString('AkelEditA');
{$EXTERNALSYM AES_AKELEDITCLASSA}
const AES_AKELEDITCLASSW = WideString('AkelEditW');
{$EXTERNALSYM AES_AKELEDITCLASSW}
const AES_RICHEDIT20A = AnsiString('RichEdit20A');
{$EXTERNALSYM AES_RICHEDIT20A}
const AES_RICHEDIT20A_UNICODE = WideString('RichEdit20A');
{$EXTERNALSYM AES_RICHEDIT20A}
const AES_RICHEDIT20W = WideString('RichEdit20W');
{$EXTERNALSYM AES_RICHEDIT20W}

// Fr0sT: these C types aren't defined in used units
type
  SIZE_T = INT_PTR;
  HDROP  = THandle;

//AEM_CONTROLCLASS
const AECLASS_AKELEDIT = 1;
{$EXTERNALSYM AECLASS_AKELEDIT}
const AECLASS_RICHEDIT = 2;
{$EXTERNALSYM AECLASS_RICHEDIT}

//Window styles
{
  Fr0sT: these constants are already defined in RTL since D7
  const ES_MULTILINE       = $00000004;  //See AECO_MULTILINE.
  const ES_NOHIDESEL       = $00000100;  //See AECO_NOHIDESEL.
  const ES_READONLY        = $00000800;  //See AECO_READONLY.
  const ES_WANTRETURN      = $00001000;  //See AECO_WANTRETURN.
  const ES_DISABLENOSCROLL = $00002000;  //See AECO_DISABLENOSCROLL.
}
const ES_HEAP_SERIALIZE = $00008000;  //Mutual exclusion will be used when the heap functions allocate and free memory from window heap. Serialization of heap access allows two or more threads to simultaneously allocate and free memory from the same heap.
{$EXTERNALSYM ES_HEAP_SERIALIZE}      //Compatibility: define same as ES_SAVESEL.

const AES_WORDDELIMITERSW : WideString = ' '#9#13#10'''`\"\\|[](){}<>,.;:+-=~!@#$%^&*/?';
{$EXTERNALSYM AES_WORDDELIMITERSW}
const AES_WRAPDELIMITERSW : WideString = ' '#9;
{$EXTERNALSYM AES_WRAPDELIMITERSW}
const AES_URLLEFTDELIMITERSW : WideString = ' '#9#13#10'''`\"(<{[=';
{$EXTERNALSYM AES_URLLEFTDELIMITERSW}
const AES_URLRIGHTDELIMITERSW  : WideString = ' '#9#13#10'''`\")>}]';
{$EXTERNALSYM AES_URLRIGHTDELIMITERSW}
const AES_URLPREFIXESW : WideString = 'http:'#0'https:'#0'www.'#0'ftp:'#0'file:'#0'mailto:'#0#0;
{$EXTERNALSYM AES_URLPREFIXESW}


//AEM_SETEVENTMASK flags
const AENM_SCROLL = $00000001;  //Sends AEN_HSCROLL and AEN_VSCROLL notifications.
{$EXTERNALSYM AENM_SCROLL}
const AENM_PAINT = $00000002;  //Sends AEN_PAINT notifications.
{$EXTERNALSYM AENM_PAINT}
const AENM_MAXTEXT = $00000010;  //Don't use it. For internal code only.
{$EXTERNALSYM AENM_MAXTEXT}
const AENM_PROGRESS = $00000020;  //Sends AEN_PROGRESS notifications.
{$EXTERNALSYM AENM_PROGRESS}
const AENM_MODIFY = $00000040;  //Sends AEN_MODIFY notifications.
{$EXTERNALSYM AENM_MODIFY}
const AENM_SELCHANGE = $00000080;  //Sends AEN_SELCHANGING and AEN_SELCHANGED notifications.
{$EXTERNALSYM AENM_SELCHANGE}
const AENM_TEXTCHANGE = $00000100;  //Sends AEN_TEXTCHANGING and AEN_TEXTCHANGED notifications.
{$EXTERNALSYM AENM_TEXTCHANGE}
const AENM_TEXTINSERT = $00000200;  //Sends AEN_TEXTINSERTBEGIN and AEN_TEXTINSERTEND notifications.
{$EXTERNALSYM AENM_TEXTINSERT}
const AENM_TEXTDELETE = $00000400;  //Sends AEN_TEXTDELETEBEGIN and AEN_TEXTDELETEEND notifications.
{$EXTERNALSYM AENM_TEXTDELETE}
const AENM_POINT = $00000800;  //Sends AEN_POINT notifications.
{$EXTERNALSYM AENM_POINT}
const AENM_DROPFILES = $00010000;  //Sends AEN_DROPFILES notifications.
{$EXTERNALSYM AENM_DROPFILES}
const AENM_DRAGDROP = $00020000;  //Sends AEN_DROPSOURCE and AEN_DROPTARGET notifications.
{$EXTERNALSYM AENM_DRAGDROP}
const AENM_LINK = $00040000;  //Sends AEN_LINK notifications.
{$EXTERNALSYM AENM_LINK}
const AENM_MARKER = $00080000;  //Sends AEN_MARKER notifications.
{$EXTERNALSYM AENM_MARKER}

//AEN_SELCHANGING and AEN_SELCHANGED types
const AESCT_REPLACESEL = $00000001;  //Replacing selection.
{$EXTERNALSYM AESCT_REPLACESEL}
const AESCT_APPENDTEXT = $00000002;  //Appending text.
{$EXTERNALSYM AESCT_APPENDTEXT}
const AESCT_SETTEXT = $00000004;  //Setting text.
{$EXTERNALSYM AESCT_SETTEXT}
const AESCT_STREAMIN = $00000008;  //Stream in.
{$EXTERNALSYM AESCT_STREAMIN}
const AESCT_WRAP = $00000010;  //Sending AEM_SETWORDWRAP.
{$EXTERNALSYM AESCT_WRAP}
const AESCT_UNDO = $00000020;  //Undo.
{$EXTERNALSYM AESCT_UNDO}
const AESCT_REDO = $00000040;  //Redo.
{$EXTERNALSYM AESCT_REDO}
const AESCT_CUT = $00000080;  //Cut.
{$EXTERNALSYM AESCT_CUT}
const AESCT_CHAR = $00000100;  //Inserting char.
{$EXTERNALSYM AESCT_CHAR}
const AESCT_KEYRETURN = $00000200;  //Pressing VK_RETURN.
{$EXTERNALSYM AESCT_KEYRETURN}
const AESCT_KEYBACKSPACE = $00000400;  //Pressing VK_BACK.
{$EXTERNALSYM AESCT_KEYBACKSPACE}
const AESCT_KEYDELETE = $00000800;  //Pressing VK_DELETE.
{$EXTERNALSYM AESCT_KEYDELETE}
const AESCT_DRAGDELETE = $00001000;  //Dragging text delete.
{$EXTERNALSYM AESCT_DRAGDELETE}
const AESCT_DROPINSERT = $00002000;  //Dropping text insert.
{$EXTERNALSYM AESCT_DROPINSERT}

const AESCT_MOUSESINGLECLK = $00100000;  //Mouse single click.
{$EXTERNALSYM AESCT_MOUSESINGLECLK}
const AESCT_MOUSEDOUBLECLK = $00200000;  //Mouse Double click.
{$EXTERNALSYM AESCT_MOUSEDOUBLECLK}
const AESCT_MOUSETRIPLECLK = $00400000;  //Mouse triple click.
{$EXTERNALSYM AESCT_MOUSETRIPLECLK}
const AESCT_MOUSECAPTURE = $00800000;  //Mouse made non-empty selection.
{$EXTERNALSYM AESCT_MOUSECAPTURE}
const AESCT_MOUSELEFTMARGIN = $01000000;  //Left margin mouse action.
{$EXTERNALSYM AESCT_MOUSELEFTMARGIN}
const AESCT_KEYBOARD = $02000000;  //Selection is changed by keyboard.
{$EXTERNALSYM AESCT_KEYBOARD}
const AESCT_SETSELMESSAGE = $04000000;  //Sending AEM_EXSETSEL, AEM_SETSEL, EM_SETSEL, EM_EXSETSEL.
{$EXTERNALSYM AESCT_SETSELMESSAGE}
const AESCT_UPDATESELECTION = $08000000;  //Selection is updated.
{$EXTERNALSYM AESCT_UPDATESELECTION}
const AESCT_IME = $10000000;  //Input Method Editors (IME).
{$EXTERNALSYM AESCT_IME}

//AEN_TEXTCHANGING, AEN_TEXTINSERTBEGIN, AEN_TEXTINSERTEND, AEN_TEXTDELETEBEGIN, AEN_TEXTDELETEEND and AEN_TEXTCHANGED types
const AETCT_REPLACESEL = $00000001;  //Replacing selection.
{$EXTERNALSYM AETCT_REPLACESEL}
const AETCT_APPENDTEXT = $00000002;  //Appending text.
{$EXTERNALSYM AETCT_APPENDTEXT}
const AETCT_SETTEXT = $00000004;  //Setting text.
{$EXTERNALSYM AETCT_SETTEXT}
const AETCT_STREAMIN = $00000008;  //Stream in.
{$EXTERNALSYM AETCT_STREAMIN}
const AETCT_WRAP = $00000010;  //Sending AEM_SETWORDWRAP.
{$EXTERNALSYM AETCT_WRAP}
const AETCT_UNDO = $00000020;  //Undo.
{$EXTERNALSYM AETCT_UNDO}
const AETCT_REDO = $00000040;  //Redo.
{$EXTERNALSYM AETCT_REDO}
const AETCT_CUT = $00000080;  //Cut.
{$EXTERNALSYM AETCT_CUT}
const AETCT_CHAR = $00000100;  //Inserting PChar  action all text has been modified.
{$EXTERNALSYM AETCT_CHAR}
const AETCT_KEYRETURN = $00000200;  //Pressing VK_RETURN.
{$EXTERNALSYM AETCT_KEYRETURN}
const AETCT_KEYBACKSPACE = $00000400;  //Pressing VK_BACK.
{$EXTERNALSYM AETCT_KEYBACKSPACE}
const AETCT_KEYDELETE = $00000800;  //Pressing VK_DELETE.
{$EXTERNALSYM AETCT_KEYDELETE}
const AETCT_DRAGDELETE = $00001000;  //Dragging text delete.
{$EXTERNALSYM AETCT_DRAGDELETE}
const AETCT_DROPINSERT = $00002000;  //Dropping text insert.
{$EXTERNALSYM AETCT_DROPINSERT}

//AEN_TEXTCHANGED types
const AETCT_NONE = $00100000;  //No text is changed.

{$EXTERNALSYM AETCT_NONE}
const AETCT_DELETEALL = $00200000;  //Indicate that due to AETCT_ * action all text has been modified.
{$EXTERNALSYM AETCT_DELETEALL}

//Insert text flags
const AEINST_LOCKUNDO = $00000001;
{$EXTERNALSYM AEINST_LOCKUNDO}
const AEINST_LOCKSCROLL = $00000002;
{$EXTERNALSYM AEINST_LOCKSCROLL}
const AEINST_LOCKUPDATEHSCROLL = $00000004;
{$EXTERNALSYM AEINST_LOCKUPDATEHSCROLL}
const AEINST_LOCKUPDATEVSCROLL = $00000008;
{$EXTERNALSYM AEINST_LOCKUPDATEVSCROLL}
const AEINST_LOCKUPDATECARET = $00000010;
{$EXTERNALSYM AEINST_LOCKUPDATECARET}
const AEINST_LOCKUPDATETEXT = $00000020;
{$EXTERNALSYM AEINST_LOCKUPDATETEXT}
const AEINST_LOCKUPDATEALL = (AEINST_LOCKUPDATEHSCROLL or AEINST_LOCKUPDATEVSCROLL or AEINST_LOCKUPDATECARET or AEINST_LOCKUPDATETEXT);
{$EXTERNALSYM AEINST_LOCKUPDATEALL}

//Delete text flags
const AEDELT_LOCKUNDO = $00000001;
{$EXTERNALSYM AEDELT_LOCKUNDO}
const AEDELT_LOCKSCROLL = $00000002;
{$EXTERNALSYM AEDELT_LOCKSCROLL}
const AEDELT_LOCKUPDATEHSCROLL = $00000004;
{$EXTERNALSYM AEDELT_LOCKUPDATEHSCROLL}
const AEDELT_LOCKUPDATEVSCROLL = $00000008;
{$EXTERNALSYM AEDELT_LOCKUPDATEVSCROLL}
const AEDELT_LOCKUPDATECARET = $00000010;
{$EXTERNALSYM AEDELT_LOCKUPDATECARET}
const AEDELT_LOCKUPDATETEXT = $00000020;
{$EXTERNALSYM AEDELT_LOCKUPDATETEXT}
const AEDELT_SAVECOLUMNSEL = $00000040;
{$EXTERNALSYM AEDELT_SAVECOLUMNSEL}
const AEDELT_LOCKUPDATEALL = (AEDELT_LOCKUPDATEHSCROLL or AEDELT_LOCKUPDATEVSCROLL or AEDELT_LOCKUPDATECARET or AEDELT_LOCKUPDATETEXT);
{$EXTERNALSYM AEDELT_LOCKUPDATEALL}

//AEN_POINT types
const AEPTT_SETTEXT = $00000001;  //All document text has been changed. All points reset to first character.
{$EXTERNALSYM AEPTT_SETTEXT}
const AEPTT_STREAMIN = $00000002;  //All document text has been changed. All points reset to first character.
{$EXTERNALSYM AEPTT_STREAMIN}
const AEPTT_INSERT = $00000004;  //Insert operation.
{$EXTERNALSYM AEPTT_INSERT}
const AEPTT_DELETE = $00000008;  //Delete operation.
{$EXTERNALSYM AEPTT_DELETE}

//AEPOINT flags
const AEPTF_MODIFY = $00000002;  //If set, text in (AEPOINT.ciPoint + AEPOINT.nPointLen) area has been modified.
{$EXTERNALSYM AEPTF_MODIFY}
const AEPTF_INSERT = $00000004;  //If set, AEPOINT.nPointLen index has been increased. Additional for AEPTF_MODIFY flag.
{$EXTERNALSYM AEPTF_INSERT}
const AEPTF_DELETE = $00000008;  //If set, AEPOINT.nPointLen index has been decreased. Additional for AEPTF_MODIFY flag.
{$EXTERNALSYM AEPTF_DELETE}
const AEPTF_NOTIFYDELETE = $00000010;  //Don't use it. For internal code only.
{$EXTERNALSYM AEPTF_NOTIFYDELETE}
const AEPTF_NOTIFYINSERT = $00000020;  //Don't use it. For internal code only.
{$EXTERNALSYM AEPTF_NOTIFYINSERT}
const AEPTF_VALIDLINE = $00000040;  //Don't use it. For internal code only.
{$EXTERNALSYM AEPTF_VALIDLINE}
const AEPTF_FOLD = $00000100;  //If set, AEPOINT.ciPoint index is used in fold. AEPOINT.dwUserData is pointer to a AEFOLD structure.
{$EXTERNALSYM AEPTF_FOLD}
const AEPTF_MOVEOFFSET = $00001000;  //If set, AEPOINT.nPointOffset has been changed.
{$EXTERNALSYM AEPTF_MOVEOFFSET}
const AEPTF_MOVELINE = $00002000;  //If set, AEPOINT.ciPoint.nLine has been changed.
{$EXTERNALSYM AEPTF_MOVELINE}

//AEPOINT character offset value
const AEPTO_IGNORE = -1;  //Character RichEdit offset is not used in AEPOINT.
{$EXTERNALSYM AEPTO_IGNORE}
const AEPTO_CALC = -2;  //Character RichEdit offset will calculated automatically by AEM_ADDPOINT.
{$EXTERNALSYM AEPTO_CALC}

//AEM_COLLAPSELINE and AEM_COLLAPSEFOLD flags
const AECF_EXPAND = $00000000;  //Expand fold (default).
{$EXTERNALSYM AECF_EXPAND}
const AECF_COLLAPSE = $00000001;  //Collapse fold.
{$EXTERNALSYM AECF_COLLAPSE}
const AECF_RECURSE = $00000002;  //Recursive processing with all children.
{$EXTERNALSYM AECF_RECURSE}
const AECF_NOUPDATE = $00000004;  //Don't update scroll and selection.
{$EXTERNALSYM AECF_NOUPDATE}
const AECF_NOCARETCORRECT = $00000008;  //If in collapsed fold located caret, don't move it to fold start.
{$EXTERNALSYM AECF_NOCARETCORRECT}

//AEN_DROPTARGET actions
const AEDT_TARGETENTER = 1;  //Enter into the target window.
{$EXTERNALSYM AEDT_TARGETENTER}
const AEDT_TARGETOVER = 2;  //Moving over the target window.
{$EXTERNALSYM AEDT_TARGETOVER}
const AEDT_TARGETLEAVE = 3;  //Leaving the target window.
{$EXTERNALSYM AEDT_TARGETLEAVE}
const AEDT_TARGETDROP = 4;  //Drops the data into the target window.
{$EXTERNALSYM AEDT_TARGETDROP}

//AEN_DROPSOURCE actions
const AEDS_SOURCEBEGIN = 1;  //cBegin selection dragging.
{$EXTERNALSYM AEDS_SOURCEBEGIN}
const AEDS_SOURCEEND = 2;  //End selection dragging. Before selection deletion.
{$EXTERNALSYM AEDS_SOURCEEND}
const AEDS_SOURCEDONE = 3;  //End dragging operation. Same as EN_DRAGDROPDONE.
{$EXTERNALSYM AEDS_SOURCEDONE}

//AEN_PROGRESS type
const AEPGS_SETTEXT = $00000001;  //Setting text.
{$EXTERNALSYM AEPGS_SETTEXT}
const AEPGS_WRAPTEXT = $00000002;  //Wrapping text.
{$EXTERNALSYM AEPGS_WRAPTEXT}
const AEPGS_STREAMIN = $00000004;  //Receiving stream text.
{$EXTERNALSYM AEPGS_STREAMIN}

//AEN_PAINT type
const AEPNT_BEGIN = $00000001;  //Sends before painting is started, only AENPAINT.hDC member is valid.
{$EXTERNALSYM AEPNT_BEGIN}
const AEPNT_END = $00000002;  //Sends before clean-up paint resources.
{$EXTERNALSYM AEPNT_END}
const AEPNT_DRAWLINE = $00000004;  //Sends before line is drawn.
{$EXTERNALSYM AEPNT_DRAWLINE}

//AEM_SETOPTIONS flags
// Window styles:
const AECO_READONLY = $00000001;  //Set read-only mode. You can use ES_READONLY window style.
{$EXTERNALSYM AECO_READONLY}
const AECO_DISABLENOSCROLL = $00000002;  //Disables scroll bars instead of hiding them when they are not needed. You can use ES_DISABLENOSCROLL window style.
{$EXTERNALSYM AECO_DISABLENOSCROLL}
const AECO_NOHIDESEL = $00000004;  //If you do not specify this style, then hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus. You can use ES_NOHIDESEL window style.
{$EXTERNALSYM AECO_NOHIDESEL}
const AECO_WANTRETURN = $00000008;  //If you do not specify this style, pressing the ENTER key has the same effect as pressing the dialog box's default push button. You can use ES_WANTRETURN window style.
{$EXTERNALSYM AECO_WANTRETURN}
const AECO_MULTILINE = $00000010;  //Designates a multiline edit control. The default is single-line edit control. You can use ES_MULTILINE window style.
{$EXTERNALSYM AECO_MULTILINE}
// Other:
const AECO_DETAILEDUNDO = $00000020;  //The control stores any typing action, into a new action in the undo queue.
{$EXTERNALSYM AECO_DETAILEDUNDO}
const AECO_PASTESELECTCOLUMN = $00000040;  //Selects pasted text in column mode.
{$EXTERNALSYM AECO_PASTESELECTCOLUMN}
const AECO_DISABLEDRAG = $00000080;  //Disables OLE text dragging.
{$EXTERNALSYM AECO_DISABLEDRAG}
const AECO_DISABLEDROP = $00000100;  //Disables OLE text dropping.
{$EXTERNALSYM AECO_DISABLEDROP}
const AECO_CARETOUTEDGE = $00000200;  //Allow caret moving out of the line edge.
{$EXTERNALSYM AECO_CARETOUTEDGE}
const AECO_ACTIVECOLUMN = $00000400;  //Draw caret vertical line.
{$EXTERNALSYM AECO_ACTIVECOLUMN}
const AECO_ACTIVELINE = $00000800;  //Draw active line.
{$EXTERNALSYM AECO_ACTIVELINE}
const AECO_ACTIVELINEBORDER = $00001000;  //Draw active line border.
{$EXTERNALSYM AECO_ACTIVELINEBORDER}
const AECO_ALTLINEBORDER = $00002000;  //Draw alternating lines border.
{$EXTERNALSYM AECO_ALTLINEBORDER}
const AECO_NONEWLINEDRAW = $00004000;  //Disables drawing new line selection as space character.
{$EXTERNALSYM AECO_NONEWLINEDRAW}
const AECO_ENTIRENEWLINEDRAW = $00008000;  //Draw new line selection to the right edge.
{$EXTERNALSYM AECO_ENTIRENEWLINEDRAW}
const AECO_NONEWLINEMOUSESELECT = $00010000;  //Triple click and left margin click selects only line contents without new line.
{$EXTERNALSYM AECO_NONEWLINEMOUSESELECT}
const AECO_LBUTTONUPCONTINUECAPTURE = $00020000;  //After WM_LBUTTONUP message capture operations doesn't stopped.
{$EXTERNALSYM AECO_LBUTTONUPCONTINUECAPTURE}
const AECO_RBUTTONDOWNMOVECARET = $00040000;  //WM_RBUTTONDOWN message moves caret to a click position.
{$EXTERNALSYM AECO_RBUTTONDOWNMOVECARET}
const AECO_MARGINSELUNWRAPLINE = $00080000;  //Left margin line selection with mouse selects all wrapped line.
{$EXTERNALSYM AECO_MARGINSELUNWRAPLINE}
const AECO_LOCKSELECTION = $00100000;  //Prevent selection changing. Use it with AECO_READONLY flag.
{$EXTERNALSYM AECO_LOCKSELECTION}
const AECO_NOMARGINSEL = $00200000;  //Disables left margin line selection with mouse.
{$EXTERNALSYM AECO_NOMARGINSEL}
const AECO_NOMARKERMOVE = $00400000;  //Disables changing position of column marker with mouse and shift button.
{$EXTERNALSYM AECO_NOMARKERMOVE}
const AECO_NOMARKERAFTERLASTLINE = $00800000;  //Disables marker painting after last line.
{$EXTERNALSYM AECO_NOMARKERAFTERLASTLINE}
const AECO_VSCROLLBYLINE = $01000000;  //Unit of vertical scrolling is line (default is pixel).
{$EXTERNALSYM AECO_VSCROLLBYLINE}
const AECO_NOSCROLLDELETEALL = $02000000;  //Turn off scrolling to caret, when undo/redo operations replace all text.
{$EXTERNALSYM AECO_NOSCROLLDELETEALL}
const AECO_NOSCROLLSELECTALL = $04000000;  //Turn off scrolling to caret, when all text is selected.
{$EXTERNALSYM AECO_NOSCROLLSELECTALL}
const AECO_NOCOLUMNPASTEHOTKEY = $08000000;  //Turn off Alt+V for columnar paste.
{$EXTERNALSYM AECO_NOCOLUMNPASTEHOTKEY}
const AECO_DISABLEBEEP = $10000000;  //Disables sound beep, when unallowable action occur.
{$EXTERNALSYM AECO_DISABLEBEEP}
const AECO_ALTDECINPUT = $20000000;  //Do Alt+NumPad decimal input with NumLock on (default is decimal input after two "Num 0").
{$EXTERNALSYM AECO_ALTDECINPUT}
const AECO_PAINTGROUP = $40000000;  //Paint text by group of characters (default is character by character).
{$EXTERNALSYM AECO_PAINTGROUP}      //With this flag some text recognition programs could start to work, printer could print faster, but highlighted symbols and combined unicode symbols can be drawn differently and editing of whose characters may become uncomfortable.
const AECO_NOPRINTCOLLAPSED = $80000000;  //Disables print collapsed lines. See AEM_COLLAPSEFOLD message.
{$EXTERNALSYM AECO_NOPRINTCOLLAPSED}

//AEM_EXSETOPTIONS flags
const AECOE_DETECTURL = $00000001;  //Enables detection and highlighting of URLs by an edit control.
{$EXTERNALSYM AECOE_DETECTURL}
const AECOE_OVERTYPE = $00000002;  //Turn on overtype mode instead of insert mode.
{$EXTERNALSYM AECOE_OVERTYPE}

const AECOOP_SET = 1;  //Sets the options to those specified by lParam.
{$EXTERNALSYM AECOOP_SET}
const AECOOP_OR = 2;  //Combines the specified options with the current options.
{$EXTERNALSYM AECOOP_OR}
const AECOOP_AND = 3;  //Retains only those current options that are also specified by lParam.
{$EXTERNALSYM AECOOP_AND}
const AECOOP_XOR = 4;  //Logically exclusive OR the current options with those specified by lParam.
{$EXTERNALSYM AECOOP_XOR}

//Modifier flags
const AEMOD_ALT = $1;  //ALT key
{$EXTERNALSYM AEMOD_ALT}
const AEMOD_SHIFT = $2;  //SHIFT key
{$EXTERNALSYM AEMOD_SHIFT}
const AEMOD_CONTROL = $4;  //CTRL key
{$EXTERNALSYM AEMOD_CONTROL}

//AEM_GETLINENUMBER flags
const AEGL_LINECOUNT = 0;  //Total number of text lines. If the control has no text, the return value is 1.
{$EXTERNALSYM AEGL_LINECOUNT}
const AEGL_FIRSTSELLINE = 1;  //First line of the selection.
{$EXTERNALSYM AEGL_FIRSTSELLINE}
const AEGL_LASTSELLINE = 2;  //Last line of the selection.
{$EXTERNALSYM AEGL_LASTSELLINE}
const AEGL_CARETLINE = 3;  //Caret line.
{$EXTERNALSYM AEGL_CARETLINE}
const AEGL_FIRSTVISIBLELINE = 4;  //First visible line.
{$EXTERNALSYM AEGL_FIRSTVISIBLELINE}
const AEGL_LASTVISIBLELINE = 5;  //Last visible line.
{$EXTERNALSYM AEGL_LASTVISIBLELINE}
const AEGL_FIRSTFULLVISIBLELINE = 6;  //First fully visible line.
{$EXTERNALSYM AEGL_FIRSTFULLVISIBLELINE}
const AEGL_LASTFULLVISIBLELINE = 7;  //Last fully visible line.
{$EXTERNALSYM AEGL_LASTFULLVISIBLELINE}
const AEGL_LINEUNWRAPCOUNT = 11;  //Total number of unwrapped text lines. If the control has no text, the return value is 1.
{$EXTERNALSYM AEGL_LINEUNWRAPCOUNT}
const AEGL_UNWRAPSELMULTILINE = 12;  //Returns value: TRUE - if selection on multiple lines. FALSE - if no selection or selection is on single line.
{$EXTERNALSYM AEGL_UNWRAPSELMULTILINE}

//AEM_GETINDEX and AEM_GETRICHOFFSET flags
const AEGI_FIRSTCHAR = 1;  //First character.
{$EXTERNALSYM AEGI_FIRSTCHAR}
const AEGI_LASTCHAR = 2;  //Last character.
{$EXTERNALSYM AEGI_LASTCHAR}
const AEGI_FIRSTSELCHAR = 3;  //First character of the selection.
{$EXTERNALSYM AEGI_FIRSTSELCHAR}
const AEGI_LASTSELCHAR = 4;  //Last character of the selection.
{$EXTERNALSYM AEGI_LASTSELCHAR}
const AEGI_CARETCHAR = 5;  //Caret character.
{$EXTERNALSYM AEGI_CARETCHAR}
const AEGI_FIRSTVISIBLECHAR = 6;  //First visible character, collapsed lines are skipped.
{$EXTERNALSYM AEGI_FIRSTVISIBLECHAR}
const AEGI_LASTVISIBLECHAR = 7;  //Last visible character, collapsed lines are skipped.
{$EXTERNALSYM AEGI_LASTVISIBLECHAR}
const AEGI_FIRSTFULLVISIBLECHAR = 8;  //First fully visible character, collapsed lines are skipped.
{$EXTERNALSYM AEGI_FIRSTFULLVISIBLECHAR}
const AEGI_LASTFULLVISIBLECHAR = 9;  //Last fully visible character, collapsed lines are skipped.
{$EXTERNALSYM AEGI_LASTFULLVISIBLECHAR}
const AEGI_FIRSTVISIBLELINE = 10;  //First character of the first visible line, collapsed lines are skipped.
{$EXTERNALSYM AEGI_FIRSTVISIBLELINE}
const AEGI_LASTVISIBLELINE = 11;  //Last character of the last visible line, collapsed lines are skipped.
{$EXTERNALSYM AEGI_LASTVISIBLELINE}
const AEGI_FIRSTFULLVISIBLELINE = 12;  //First character of the first fully visible line, collapsed lines are skipped.
{$EXTERNALSYM AEGI_FIRSTFULLVISIBLELINE}
const AEGI_LASTFULLVISIBLELINE = 13;    //Last character of the last fully visible line, collapsed lines are skipped.
{$EXTERNALSYM AEGI_LASTFULLVISIBLELINE} //

//Next flags require pointer to the input index in lParam.
const AEGI_VALIDCHARINLINE = 15;      //Correct character to make sure that it is on line.
{$EXTERNALSYM AEGI_VALIDCHARINLINE}   //For better performance use AEC_ValidCharInLine instead.
const AEGI_LINEBEGIN = 16;            //First character in line.
{$EXTERNALSYM AEGI_LINEBEGIN}         //
const AEGI_LINEEND = 17;              //Last character in line.
{$EXTERNALSYM AEGI_LINEEND}           //

const AEGI_WRAPLINEBEGIN = 18;        //First character of the unwrapped line. Returns number of characters as AEM_GETINDEX result.
{$EXTERNALSYM AEGI_WRAPLINEBEGIN}     //For better performance use AEC_WrapLineBeginEx instead.
const AEGI_WRAPLINEEND = 19;          //Last character of the unwrapped line. Returns number of characters as AEM_GETINDEX result.
{$EXTERNALSYM AEGI_WRAPLINEEND}       //For better performance use AEC_WrapLineEndEx instead.
const AEGI_NEXTCHARINLINE = 20;       //Next character in line.
{$EXTERNALSYM AEGI_NEXTCHARINLINE}    //For better performance use AEC_NextCharInLineEx instead.
const AEGI_PREVCHARINLINE = 21;       //Previous character in line.
{$EXTERNALSYM AEGI_PREVCHARINLINE}    //For better performance use AEC_PrevCharInLineEx instead.
const AEGI_NEXTCHAR = 22;             //Next wide character.
{$EXTERNALSYM AEGI_NEXTCHAR}          //For better performance use AEC_NextCharEx instead.
const AEGI_PREVCHAR = 23;             //Previous wide character.
{$EXTERNALSYM AEGI_PREVCHAR}          //For better performance use AEC_PrevCharEx instead.
const AEGI_NEXTLINE = 24;             //First character of the next line.
{$EXTERNALSYM AEGI_NEXTLINE}          //For better performance use AEC_NextLineEx instead.
const AEGI_PREVLINE = 25;             //First character of the previous line.
{$EXTERNALSYM AEGI_PREVLINE}          //For better performance use AEC_PrevLineEx instead.
const AEGI_NEXTUNCOLLAPSEDCHAR = 26;  //Next wide character, collapsed lines are skipped.
{$EXTERNALSYM AEGI_NEXTUNCOLLAPSEDCHAR}
const AEGI_PREVUNCOLLAPSEDCHAR = 27;  //Previous wide character, collapsed lines are skipped.
{$EXTERNALSYM AEGI_PREVUNCOLLAPSEDCHAR}
const AEGI_NEXTUNCOLLAPSEDLINE = 28;  //First character of the next line, collapsed lines are skipped.
{$EXTERNALSYM AEGI_NEXTUNCOLLAPSEDLINE}
const AEGI_PREVUNCOLLAPSEDLINE = 29;  //First character of the previous line, collapsed lines are skipped.
{$EXTERNALSYM AEGI_PREVUNCOLLAPSEDLINE}

//AEM_ISDELIMITER parameter
const AEDLM_PREVCHAR = $00000001;  //Check previous char.
{$EXTERNALSYM AEDLM_PREVCHAR}
const AEDLM_WORD = $00000010;  //Word delimiter.
{$EXTERNALSYM AEDLM_WORD}
const AEDLM_WRAP = $00000020;  //Wrap delimiter.
{$EXTERNALSYM AEDLM_WRAP}
const AEDLM_URLLEFT = $00000040;  //URL left delimiter.
{$EXTERNALSYM AEDLM_URLLEFT}
const AEDLM_URLRIGHT = $00000080;  //URL right delimiter.
{$EXTERNALSYM AEDLM_URLRIGHT}

//AEM_SETSEL and AEM_UPDATESEL flags
const AESELT_COLUMNON = $00000001;  //Make column selection ON.
{$EXTERNALSYM AESELT_COLUMNON}
const AESELT_COLUMNASIS = $00000002;  //Leave column selection as is.
{$EXTERNALSYM AESELT_COLUMNASIS}
const AESELT_LOCKNOTIFY = $00000004;  //Disable AEN_SELCHANGING and AEN_SELCHANGED notifications.
{$EXTERNALSYM AESELT_LOCKNOTIFY}
const AESELT_LOCKSCROLL = $00000008;  //Lock edit window scroll.
{$EXTERNALSYM AESELT_LOCKSCROLL}
const AESELT_LOCKUPDATE = $00000010;  //Lock edit window update.
{$EXTERNALSYM AESELT_LOCKUPDATE}
const AESELT_LOCKCARET = $00000020;  //Lock caret reposition.
{$EXTERNALSYM AESELT_LOCKCARET}
const AESELT_LOCKUNDOGROUPING = $00000040;  //Don't use it. For internal code only.
{$EXTERNALSYM AESELT_LOCKUNDOGROUPING}
const AESELT_NOCARETHORZINDENT = $00000080;  //Caret horizontal indent isn't changed.
{$EXTERNALSYM AESELT_NOCARETHORZINDENT}
const AESELT_NOVERTSCROLLCORRECT = $00000100;  //On some conditions scroll can be increased to a height of one line.
{$EXTERNALSYM AESELT_NOVERTSCROLLCORRECT}
const AESELT_MOUSE = $00000200;  //Don't use it. For internal code only.
{$EXTERNALSYM AESELT_MOUSE}
const AESELT_RESETSELECTION = $00000400;  //Don't use it. For internal code only.
{$EXTERNALSYM AESELT_RESETSELECTION}
const AESELT_INDEXUPDATE = $00000800;  //Update lpLine member of the AEM_SETSEL message structures, to avoid dangling of a pointer after text change.
{$EXTERNALSYM AESELT_INDEXUPDATE}

//AEM_REPLACESEL flags
const AEREPT_COLUMNON = $00000001;  //Make column selection ON.
{$EXTERNALSYM AEREPT_COLUMNON}
const AEREPT_COLUMNASIS = $00000002;  //Leave column selection as is.
{$EXTERNALSYM AEREPT_COLUMNASIS}
const AEREPT_LOCKSCROLL = $00000004;  //Lock edit window scroll. However edit window can be scrolled during window resize when AECO_DISABLENOSCROLL option not set.
{$EXTERNALSYM AEREPT_LOCKSCROLL}
const AEREPT_UNDOGROUPING = $00000100;  //Continue undo grouping.
{$EXTERNALSYM AEREPT_UNDOGROUPING}

//AEM_CHARFROMPOS return value
const AEPC_ERROR = 0;  //Error.
{$EXTERNALSYM AEPC_ERROR}
const AEPC_EQUAL = 1;  //Char exactly in specified position.
{$EXTERNALSYM AEPC_EQUAL}
const AEPC_BEFORE = 2;  //Char before the specified position.
{$EXTERNALSYM AEPC_BEFORE}
const AEPC_AFTER = 3;  //Char after the specified position.
{$EXTERNALSYM AEPC_AFTER}

//New line value
const AELB_ASINPUT = 1;  //Use input new line, see AEM_SETNEWLINE with AENL_INPUT.
{$EXTERNALSYM AELB_ASINPUT}
const AELB_ASOUTPUT = 2;  //Use output new line, see AEM_SETNEWLINE with AENL_OUTPUT.
{$EXTERNALSYM AELB_ASOUTPUT}
const AELB_ASIS = 3;  //Use new line of the source.
{$EXTERNALSYM AELB_ASIS}
const AELB_EOF = 4;  //End-of-file, last line in document.
{$EXTERNALSYM AELB_EOF}
const AELB_R = 5;  //"\r" new line.
{$EXTERNALSYM AELB_R}
const AELB_N = 6;  //"\n" new line.
{$EXTERNALSYM AELB_N}
const AELB_RN = 7;  //"\r\n" new line.
{$EXTERNALSYM AELB_RN}
const AELB_RRN = 8;  //"\r\r\n" new line.
{$EXTERNALSYM AELB_RRN}
const AELB_WRAP = 9;  //No new line, this line is wrapped.
{$EXTERNALSYM AELB_WRAP}

//AEM_SETNEWLINE flags
const AENL_INPUT = $00000001;  //Sets default new line for the input operations, for example AEM_PASTE.
{$EXTERNALSYM AENL_INPUT}
const AENL_OUTPUT = $00000002;  //Sets default new line for the output operations, for example AEM_COPY.
{$EXTERNALSYM AENL_OUTPUT}

//AEM_PASTE flags
const AEPFC_ANSI = $00000001;  //Paste text as ANSI. Default is paste as Unicode text, if no Unicode text available ANSI text will be used.
{$EXTERNALSYM AEPFC_ANSI}
const AEPFC_COLUMN = $00000002;  //Paste to column selection.
{$EXTERNALSYM AEPFC_COLUMN}

//AEM_LOCKUPDATE FLAGS
const AELU_SCROLLBAR = $00000001;
{$EXTERNALSYM AELU_SCROLLBAR}
const AELU_CARET = $00000002;
{$EXTERNALSYM AELU_CARET}

//AEM_SETDOCUMENT flags
const AESWD_NOCHECKFOCUS = $00000001;  //Don't update focus state.
{$EXTERNALSYM AESWD_NOCHECKFOCUS}
const AESWD_NODRAGDROP = $00000002;  //Don't register drag-and-drop with a new IDropTarget.
{$EXTERNALSYM AESWD_NODRAGDROP}
const AESWD_NOSHOWSCROLLBARS = $00000004;  //Don't update scrollbars visibility.
{$EXTERNALSYM AESWD_NOSHOWSCROLLBARS}
const AESWD_NOUPDATESCROLLBARS = $00000008;  //Don't update scrollbars position.
{$EXTERNALSYM AESWD_NOUPDATESCROLLBARS}
const AESWD_NOUPDATECARET = $00000010;  //Don't update caret.
{$EXTERNALSYM AESWD_NOUPDATECARET}
const AESWD_NOINVALIDATERECT = $00000020;  //Don't redraw edit window.
{$EXTERNALSYM AESWD_NOINVALIDATERECT}

const AESWD_NOREDRAW = (AESWD_NOUPDATESCROLLBARS or AESWD_NOUPDATECARET or AESWD_NOINVALIDATERECT);
{$EXTERNALSYM AESWD_NOREDRAW}
const AESWD_NOALL = (AESWD_NOCHECKFOCUS or AESWD_NODRAGDROP or AESWD_NOSHOWSCROLLBARS or AESWD_NOREDRAW);
{$EXTERNALSYM AESWD_NOALL}

//AEM_DRAGDROP flags
const AEDD_GETDRAGWINDOW = 1;  //Return dragging window handle.
{$EXTERNALSYM AEDD_GETDRAGWINDOW}
const AEDD_STOPDRAG = 2;  //Set stop dragging operation flag.
{$EXTERNALSYM AEDD_STOPDRAG}

//AEM_SETCOLORS flags
const AECLR_DEFAULT = $00000001;  //Use default system colors for the specified flags, all members of the AECOLORS structure are ignored.
{$EXTERNALSYM AECLR_DEFAULT}
const AECLR_CARET = $00000002;  //Sets caret color. crCaret member is valid.
{$EXTERNALSYM AECLR_CARET}
const AECLR_BASICTEXT = $00000004;  //Sets basic text color. crBasicText member is valid.
{$EXTERNALSYM AECLR_BASICTEXT}
const AECLR_BASICBK = $00000008;  //Sets basic background color. crBasicBk member is valid.
{$EXTERNALSYM AECLR_BASICBK}
const AECLR_SELTEXT = $00000010;  //Sets text in selection color. crSelText member is valid.
{$EXTERNALSYM AECLR_SELTEXT}
const AECLR_SELBK = $00000020;  //Sets background in selection color. crSelBk member is valid.
{$EXTERNALSYM AECLR_SELBK}
const AECLR_ACTIVELINETEXT = $00000040;  //Sets active line text color. crActiveLineText member is valid.
{$EXTERNALSYM AECLR_ACTIVELINETEXT}
const AECLR_ACTIVELINEBK = $00000080;  //Sets active line background color. crActiveLineBk member is valid.
{$EXTERNALSYM AECLR_ACTIVELINEBK}
const AECLR_URLTEXT = $00000100;  //Sets hyperlink text color. crUrlText member is valid.
{$EXTERNALSYM AECLR_URLTEXT}
const AECLR_ACTIVECOLUMN = $00000200;  //Sets active column color. crActiveColumn member is valid.
{$EXTERNALSYM AECLR_ACTIVECOLUMN}
const AECLR_COLUMNMARKER = $00000400;  //Sets column marker color. crColumnMarker member is valid.
{$EXTERNALSYM AECLR_COLUMNMARKER}
const AECLR_URLCURSORTEXT = $00000800;  //Sets active hyperlink text color. crUrlCursorText member is valid.
{$EXTERNALSYM AECLR_URLCURSORTEXT}
const AECLR_URLVISITTEXT = $00001000;  //Sets visited hyperlink text color. crUrlVisitText member is valid.
{$EXTERNALSYM AECLR_URLVISITTEXT}
const AECLR_ACTIVELINEBORDER = $00002000;  //Sets active line border color. crActiveLineBorder member is valid.
{$EXTERNALSYM AECLR_ACTIVELINEBORDER}
const AECLR_ALTLINETEXT = $00004000;  //Sets alternating line text color. crAltLineText member is valid.
{$EXTERNALSYM AECLR_ALTLINETEXT}
const AECLR_ALTLINEBK = $00008000;  //Sets alternating line background color. crAltLineBk member is valid.
{$EXTERNALSYM AECLR_ALTLINEBK}
const AECLR_ALTLINEBORDER = $00010000;  //Sets alternating line border color. crAltLineBorder member is valid.
{$EXTERNALSYM AECLR_ALTLINEBORDER}

const AECLR_ALL = (AECLR_CARET or AECLR_BASICTEXT or AECLR_BASICBK or AECLR_SELTEXT or AECLR_SELBK or AECLR_ACTIVELINETEXT   or AECLR_ACTIVELINEBK     or AECLR_URLTEXT          or AECLR_ACTIVECOLUMN     or AECLR_COLUMNMARKER     or AECLR_URLCURSORTEXT    or AECLR_URLVISITTEXT     or AECLR_ACTIVELINEBORDER or AECLR_ALTLINETEXT or AECLR_ALTLINEBK or AECLR_ALTLINEBORDER);
{$EXTERNALSYM AECLR_ALL}

//Print
const AEPRN_TEST = $001;  //Calculate data without painting.
{$EXTERNALSYM AEPRN_TEST}
const AEPRN_INHUNDREDTHSOFMILLIMETERS = $002;  //Indicates that hundredths of millimeters are the unit of measurement for margins.
{$EXTERNALSYM AEPRN_INHUNDREDTHSOFMILLIMETERS}
const AEPRN_INTHOUSANDTHSOFINCHES = $004;  //Indicates that thousandths of inches are the unit of measurement for margins.
{$EXTERNALSYM AEPRN_INTHOUSANDTHSOFINCHES}
const AEPRN_WRAPNONE = $008;  //Print without wrapping.
{$EXTERNALSYM AEPRN_WRAPNONE}
const AEPRN_WRAPWORD = $010;  //Print with word wrapping (default).
{$EXTERNALSYM AEPRN_WRAPWORD}
const AEPRN_WRAPSYMBOL = $020;  //Print with symbols wrapping.
{$EXTERNALSYM AEPRN_WRAPSYMBOL}
const AEPRN_IGNOREFORMFEED = $040;  //Ignore form-feed character '\f'.
{$EXTERNALSYM AEPRN_IGNOREFORMFEED}
const AEPRN_ANSI = $080;  //Ansi text output. Can solve draw problems on Win95/98/Me.
{$EXTERNALSYM AEPRN_ANSI}
const AEPRN_COLOREDTEXT = $100;  //Print colored text.
{$EXTERNALSYM AEPRN_COLOREDTEXT}
const AEPRN_COLOREDBACKGROUND = $200;  //Print on colored background.
{$EXTERNALSYM AEPRN_COLOREDBACKGROUND}
const AEPRN_COLOREDSELECTION = $400;  //Print text selection.
{$EXTERNALSYM AEPRN_COLOREDSELECTION}

//Highlight options
const AEHLO_IGNOREFONTNORMAL = $00000001;  //Use AEHLS_NONE font style, if font style to change is AEHLS_FONTNORMAL.
{$EXTERNALSYM AEHLO_IGNOREFONTNORMAL}
const AEHLO_IGNOREFONTBOLD = $00000002;  //Use AEHLS_FONTNORMAL font style, if font style to change is AEHLS_FONTBOLD.
{$EXTERNALSYM AEHLO_IGNOREFONTBOLD}
                                                 //Use AEHLS_FONTITALIC font style, if font style to change is AEHLS_FONTBOLDITALIC.
const AEHLO_IGNOREFONTITALIC = $00000004;  //Use AEHLS_FONTNORMAL font style, if font style to change is AEHLS_FONTITALIC.
{$EXTERNALSYM AEHLO_IGNOREFONTITALIC}
                                                 //Use AEHLS_FONTBOLD font style, if font style to change is AEHLS_FONTBOLDITALIC.

//Highlight flags
const AEHLF_MATCHCASE = $00000001;  //If set, the highlight operation is -sensitive. If not set, the highlight operation is -insensitive.
{$EXTERNALSYM AEHLF_MATCHCASE}
const AEHLF_WORDCOMPOSITION = $00000002;  //Word is a composition of characters. For example, AEWORDITEM.pWord equal to "1234567890" with this flag, means highlight words that contain only digits.
{$EXTERNALSYM AEHLF_WORDCOMPOSITION}
const AEHLF_QUOTEEND_REQUIRED = $00000004;  //If quote end isn't found, text after quote start will not be highlighted.
{$EXTERNALSYM AEHLF_QUOTEEND_REQUIRED}
const AEHLF_QUOTESTART_ISDELIMITER = $00000008;  //Last meet delimiter used as quote start (AEQUOTEITEM.pQuoteStart member is ignored).
{$EXTERNALSYM AEHLF_QUOTESTART_ISDELIMITER}
const AEHLF_QUOTEEND_ISDELIMITER = $00000010;  //First meet delimiter used as quote end (AEQUOTEITEM.pQuoteEnd member is ignored).
{$EXTERNALSYM AEHLF_QUOTEEND_ISDELIMITER}
const AEHLF_QUOTESTART_NOHIGHLIGHT = $00000020;  //Don't highlight quote start string.
{$EXTERNALSYM AEHLF_QUOTESTART_NOHIGHLIGHT}
const AEHLF_QUOTEEND_NOHIGHLIGHT = $00000040;  //Don't highlight quote end string.
{$EXTERNALSYM AEHLF_QUOTEEND_NOHIGHLIGHT}
const AEHLF_QUOTESTART_NOCATCH = $00000080;  //Don't catch and don't highlight quote start string.
{$EXTERNALSYM AEHLF_QUOTESTART_NOCATCH}
const AEHLF_QUOTEEND_NOCATCH = $00000100;  //Don't catch and don't highlight quote end string.
{$EXTERNALSYM AEHLF_QUOTEEND_NOCATCH}
const AEHLF_ATLINESTART = $00000200;  //Quote start, delimiter or word located at line start.
{$EXTERNALSYM AEHLF_ATLINESTART}
const AEHLF_ATLINEEND = $00000400;  //Quote end, delimiter or word located at line end.
{$EXTERNALSYM AEHLF_ATLINEEND}
const AEHLF_QUOTESTART_ISWORD = $00000800;  //Quote start is surrounded with delimiters.
{$EXTERNALSYM AEHLF_QUOTESTART_ISWORD}
const AEHLF_QUOTEEND_ISWORD = $00001000;  //Quote end is surrounded with delimiters.
{$EXTERNALSYM AEHLF_QUOTEEND_ISWORD}
const AEHLF_QUOTEWITHOUTDELIMITERS = $00002000;  //Quote doesn't contain delimiters.
{$EXTERNALSYM AEHLF_QUOTEWITHOUTDELIMITERS}
const AEHLF_QUOTESTART_CATCHONLY = $00004000;  //Only quote start string is catched.
{$EXTERNALSYM AEHLF_QUOTESTART_CATCHONLY}
const AEHLF_QUOTEINCLUDE = $00010000;  //Quote include string is valid.
{$EXTERNALSYM AEHLF_QUOTEINCLUDE}
const AEHLF_QUOTEEXCLUDE = $00020000;  //Quote exclude string is valid.
{$EXTERNALSYM AEHLF_QUOTEEXCLUDE}
const AEHLF_REGEXP = $10000000;  //AEQUOTEITEM.pQuoteStart is a regular exression pattern,
{$EXTERNALSYM AEHLF_REGEXP}
                                                 //AEQUOTEITEM.pQuoteEnd is a regular exression match map in format:
                                                 //  "\BackRef1=(FontStyle,ColorText,ColorBk) \BackRef2=(FontStyle,ColorText,ColorBk) ..."
                                                 //Notes:
                                                 //  Color need to be in #RRGGBB format.
                                                 //  If color equal to zero, then color ignored.
                                                 //  Instead of color backreference can be used.
                                                 //Example (highlight quoted string):
                                                 //  AEQUOTEITEM.pQuoteStart  (")([^"\\]*(\\.[^"\\]*)*)(")
                                                 //  AEQUOTEITEM.pQuoteEnd    \1=(0,#FF0000,0) \2=(0,#0000FF,0) \4=(0,#FF0000,0)
                                                 //Example (highlight #RRGGBB word with its color):
                                                 //  AEQUOTEITEM.pQuoteStart  #[A-F\d]{6}
                                                 //  AEQUOTEITEM.pQuoteEnd    \0=(0,\0,0)

//Highlight font style
const AEHLS_NONE = 0;  //Current style.
{$EXTERNALSYM AEHLS_NONE}
const AEHLS_FONTNORMAL = 1;  //Normal style.
{$EXTERNALSYM AEHLS_FONTNORMAL}
const AEHLS_FONTBOLD = 2;  //Bold style.
{$EXTERNALSYM AEHLS_FONTBOLD}
const AEHLS_FONTITALIC = 3;  //Italic style.
{$EXTERNALSYM AEHLS_FONTITALIC}
const AEHLS_FONTBOLDITALIC = 4;  //Bold italic style.
{$EXTERNALSYM AEHLS_FONTBOLDITALIC}

//Highlight elements
const AEHLE_DELIMITER = 1;  //Delimiter. New line, start of file and end of file are delimiters by default.
{$EXTERNALSYM AEHLE_DELIMITER}
const AEHLE_WORD = 2;  //Word - string surrounded with delimiters.
{$EXTERNALSYM AEHLE_WORD}
const AEHLE_QUOTE = 3;  //Quote - text started with quote start string and ended with quote end string.
{$EXTERNALSYM AEHLE_QUOTE}
const AEHLE_MARKTEXT = 4;  //Mark text - mark specified text.
{$EXTERNALSYM AEHLE_MARKTEXT}
const AEHLE_MARKRANGE = 5;  //Mark range - mark specified range of characters.
{$EXTERNALSYM AEHLE_MARKRANGE}

//Highlight AEM_HLGETHIGHLIGHT flags
const AEGHF_NOSELECTION = $00000001; //Ignore text selection coloring.
{$EXTERNALSYM AEGHF_NOSELECTION}
const AEGHF_NOACTIVELINE = $00000002; //Ignore active line colors.
{$EXTERNALSYM AEGHF_NOACTIVELINE}
const AEGHF_NOALTLINE = $00000004; //Ignore alternating line colors.
{$EXTERNALSYM AEGHF_NOALTLINE}

//Highlight paint type
const AEHPT_SELECTION = $00000001;
{$EXTERNALSYM AEHPT_SELECTION}
const AEHPT_DELIM1 = $00000002;
{$EXTERNALSYM AEHPT_DELIM1}
const AEHPT_WORD = $00000004;
{$EXTERNALSYM AEHPT_WORD}
const AEHPT_DELIM2 = $00000008;
{$EXTERNALSYM AEHPT_DELIM2}
const AEHPT_QUOTE = $00000010;
{$EXTERNALSYM AEHPT_QUOTE}
const AEHPT_MARKTEXT = $00000020;
{$EXTERNALSYM AEHPT_MARKTEXT}
const AEHPT_MARKRANGE = $00000040;
{$EXTERNALSYM AEHPT_MARKRANGE}
const AEHPT_LINK = $00000080;
{$EXTERNALSYM AEHPT_LINK}
const AEHPT_FOLD = $00000100;
{$EXTERNALSYM AEHPT_FOLD}

//AEREGROUPCOLOR flags
const AEREGCF_BACKREFCOLORTEXT = $00000001;  //AEREGROUPCOLOR.crText is backreference index for text color in format #RRGGBB or RRGGBB.
{$EXTERNALSYM AEREGCF_BACKREFCOLORTEXT}
const AEREGCF_BACKREFCOLORBK = $00000002;  //AEREGROUPCOLOR.crBk is backreference index for background color in format #RRGGBB or RRGGBB.
{$EXTERNALSYM AEREGCF_BACKREFCOLORBK}

//AEM_FINDFOLD flags
const AEFF_FINDOFFSET = $00000001;  //AEFINDFOLD.dwFindIt is RichEdit offset.
{$EXTERNALSYM AEFF_FINDOFFSET}
const AEFF_FINDINDEX = $00000002;  //AEFINDFOLD.dwFindIt is pointer to a AECHARINDEX structure.
{$EXTERNALSYM AEFF_FINDINDEX}
const AEFF_FINDLINE = $00000004;  //AEFINDFOLD.dwFindIt is zero based line number.
{$EXTERNALSYM AEFF_FINDLINE}
const AEFF_FOLDSTART = $00000008;  //Fold is also accepted if AEFINDFOLD.dwFindIt points to fold start.
{$EXTERNALSYM AEFF_FOLDSTART}
const AEFF_FOLDEND = $00000010;  //Fold is also accepted if AEFINDFOLD.dwFindIt points to fold end.
{$EXTERNALSYM AEFF_FOLDEND}
const AEFF_RECURSE = $00000020;  //Recursive search. Returned fold will be deepest possible.
{$EXTERNALSYM AEFF_RECURSE}
const AEFF_GETROOT = $00000040;  //Return root fold.
{$EXTERNALSYM AEFF_GETROOT}      //
                                 //The following groups of flags cannot be used together:
                                 //  AEFF_FINDOFFSET, AEFF_FINDINDEX and AEFF_FINDLINE.
                                 //  AEFF_RECURSE and AEFF_GETROOT.

//AEM_SCROLL, AEM_LINESCROLL flags
const AESB_HORZ = $00000001;  //Horizontal scroll. Cannot be used with AESB_VERT.
{$EXTERNALSYM AESB_HORZ}
const AESB_VERT = $00000002;  //Vertical scroll. Cannot be used with AESB_HORZ.
{$EXTERNALSYM AESB_VERT}
const AESB_RETURNUNITS = $00000004;  //If AESB_HORZ specified, number of characters scrolled returns. If AESB_VERT specified, number of lines scrolled returns.
{$EXTERNALSYM AESB_RETURNUNITS}
const AESB_ALIGNLEFT = $00000008;  //Align first visible char.
{$EXTERNALSYM AESB_ALIGNLEFT}
const AESB_ALIGNRIGHT = $00000010;  //Align last visible char.
{$EXTERNALSYM AESB_ALIGNRIGHT}
const AESB_ALIGNTOP = $00000020;  //Align first visible line.
{$EXTERNALSYM AESB_ALIGNTOP}
const AESB_ALIGNBOTTOM = $00000040;  //Align last visible line.
{$EXTERNALSYM AESB_ALIGNBOTTOM}

//AEM_SCROLLTOPOINT flags
const AESC_TEST = $00000001;  //Only test for scroll. Returns result, but not actually scroll.
{$EXTERNALSYM AESC_TEST}
const AESC_POINTCARET = $00000010;  //Caret position is used and AESCROLLTOPOINT.ptPos is ignored.
{$EXTERNALSYM AESC_POINTCARET}
const AESC_POINTGLOBAL = $00000020;  //AESCROLLTOPOINT.ptPos is position in the virtual text space coordinates.
{$EXTERNALSYM AESC_POINTGLOBAL}
const AESC_POINTCLIENT = $00000040;  //AESCROLLTOPOINT.ptPos is position in the client area coordinates (default).
{$EXTERNALSYM AESC_POINTCLIENT}
const AESC_POINTOUT = $00000080;  //AESCROLLTOPOINT.ptPos will contain new scroll position after AEM_SCROLLTOPOINT returns.
{$EXTERNALSYM AESC_POINTOUT}
const AESC_OFFSETPIXELX = $00000100;  //AESCROLLTOPOINT.nOffsetX specifies pixels number.
{$EXTERNALSYM AESC_OFFSETPIXELX}
const AESC_OFFSETPIXELY = $00000200;  //AESCROLLTOPOINT.nOffsetY specifies pixels number.
{$EXTERNALSYM AESC_OFFSETPIXELY}
const AESC_OFFSETCHARX = $00000400;  //AESCROLLTOPOINT.nOffsetX specifies characters number.
{$EXTERNALSYM AESC_OFFSETCHARX}
const AESC_OFFSETCHARY = $00000800;  //AESCROLLTOPOINT.nOffsetY specifies characters number.
{$EXTERNALSYM AESC_OFFSETCHARY}
const AESC_OFFSETRECTDIVX = $00001000;  //AESCROLLTOPOINT.nOffsetX specifies divisor of the edit rectangle width.
{$EXTERNALSYM AESC_OFFSETRECTDIVX}
const AESC_OFFSETRECTDIVY = $00002000;  //AESCROLLTOPOINT.nOffsetY specifies divisor of the edit rectangle width.
{$EXTERNALSYM AESC_OFFSETRECTDIVY}
const AESC_FORCELEFT = $00010000;  //Scrolls to the left even if AESCROLLTOPOINT.ptPos visible.
{$EXTERNALSYM AESC_FORCELEFT}
const AESC_FORCETOP = $00020000;  //Scrolls to the top even if AESCROLLTOPOINT.ptPos visible.
{$EXTERNALSYM AESC_FORCETOP}
const AESC_FORCERIGHT = $00040000;  //Scrolls to the right even if AESCROLLTOPOINT.ptPos visible.
{$EXTERNALSYM AESC_FORCERIGHT}
const AESC_FORCEBOTTOM = $00080000;  //Scrolls to the bottom even if AESCROLLTOPOINT.ptPos visible.
{$EXTERNALSYM AESC_FORCEBOTTOM}

//AEM_SCROLLTOPOINT return values
const AECSE_SCROLLEDX = $00000001;  //Edit control was horizontally scrolled.
{$EXTERNALSYM AECSE_SCROLLEDX}
const AECSE_SCROLLEDY = $00000002;  //Edit control was vertically scrolled.
{$EXTERNALSYM AECSE_SCROLLEDY}
const AECSE_SCROLLEDLEFT = $00000004;  //Edit control was scrolled left horizontally.
{$EXTERNALSYM AECSE_SCROLLEDLEFT}
const AECSE_SCROLLEDRIGHT = $00000008;  //Edit control was scrolled right horizontally.
{$EXTERNALSYM AECSE_SCROLLEDRIGHT}
const AECSE_SCROLLEDUP = $00000010;  //Edit control was scrolled up vertically.
{$EXTERNALSYM AECSE_SCROLLEDUP}
const AECSE_SCROLLEDDOWN = $00000020;  //Edit control was scrolled down vertically.
{$EXTERNALSYM AECSE_SCROLLEDDOWN}

//AEM_GETFONT type
const AEGF_CURRENT = 0;  //Current font handle.
{$EXTERNALSYM AEGF_CURRENT}
const AEGF_NORMAL = 1;  //Normal style font handle.
{$EXTERNALSYM AEGF_NORMAL}
const AEGF_BOLD = 2;  //Bold style font handle.
{$EXTERNALSYM AEGF_BOLD}
const AEGF_ITALIC = 3;  //Italic style font handle.
{$EXTERNALSYM AEGF_ITALIC}
const AEGF_BOLDITALIC = 4;  //Bold italic style font handle.
{$EXTERNALSYM AEGF_BOLDITALIC}
const AEGF_URL = 5;  //URL style font handle.
{$EXTERNALSYM AEGF_URL}

//AEM_GETCHARSIZE flags
const AECS_HEIGHT = 0;  //Current font character height. lParam not used.
{$EXTERNALSYM AECS_HEIGHT}
const AECS_AVEWIDTH = 1;  //Current font character average width. lParam not used.
{$EXTERNALSYM AECS_AVEWIDTH}
const AECS_INDEXWIDTH = 2;  //lParam is character index, which width is retrieving.
{$EXTERNALSYM AECS_INDEXWIDTH}
const AECS_POINTSIZE  =     3;  //Current font point size. lParam not used.
{$EXTERNALSYM AECS_POINTSIZE}
const AECS_SPACEWIDTH =     4;  //Current font space width. lParam not used.
{$EXTERNALSYM AECS_SPACEWIDTH}
const AECS_TABWIDTH   =     5;  //Current font tabulation width. lParam not used.
{$EXTERNALSYM AECS_TABWIDTH}

//AEM_CONVERTPOINT flags
const AECPT_GLOBALTOCLIENT = 0;  //Convert position in the virtual text space of the document, to client area coordinates.
{$EXTERNALSYM AECPT_GLOBALTOCLIENT}
const AECPT_CLIENTTOGLOBAL = 1;  //Convert position in the client area coordinates, to virtual text space of the document.
{$EXTERNALSYM AECPT_CLIENTTOGLOBAL}

//Coordinate type
const AECT_GLOBAL = 0;  //Position in the virtual text space coordinates.
{$EXTERNALSYM AECT_GLOBAL}
const AECT_CLIENT = 1;  //Position in the client area coordinates.
{$EXTERNALSYM AECT_CLIENT}

//Rectangle flags
const AERC_UPDATE = $01; //Redraw edit window. Only for AEM_SETRECT and AEM_SETERASERECT.
{$EXTERNALSYM AERC_UPDATE}
const AERC_MARGINS = $02;  //Rectangle contain edit area margins instead of edit area coordinates.
{$EXTERNALSYM AERC_MARGINS}
const AERC_NOLEFT = $04;  //Don't set/retrieve left side.
{$EXTERNALSYM AERC_NOLEFT}
const AERC_NOTOP = $08;  //Don't set/retrieve top side.
{$EXTERNALSYM AERC_NOTOP}
const AERC_NORIGHT = $10;  //Don't set/retrieve right side.
{$EXTERNALSYM AERC_NORIGHT}
const AERC_NOBOTTOM = $20;  //Don't set/retrieve bottom side.
{$EXTERNALSYM AERC_NOBOTTOM}

//AEM_POINTONMARGIN sides
const AESIDE_LEFT = $00000001;
{$EXTERNALSYM AESIDE_LEFT}
const AESIDE_TOP = $00000002;
{$EXTERNALSYM AESIDE_TOP}
const AESIDE_RIGHT = $00000004;
{$EXTERNALSYM AESIDE_RIGHT}
const AESIDE_BOTTOM = $00000008;
{$EXTERNALSYM AESIDE_BOTTOM}

//AEM_GETMOUSESTATE types
const AEMS_CAPTURE = 1;  //Capture state.
{$EXTERNALSYM AEMS_CAPTURE}
const AEMS_SELECTION = 2;  //Selection state.
{$EXTERNALSYM AEMS_SELECTION}

//AEM_GETMOUSESTATE capture
const AEMSC_MOUSEMOVE = $1;  //Text selection.
{$EXTERNALSYM AEMSC_MOUSEMOVE}
const AEMSC_MOUSESCROLL = $2;  //Middle button scroll.
{$EXTERNALSYM AEMSC_MOUSESCROLL}
const AEMSC_MOUSEDRAG = $4;  //Selection dragging.
{$EXTERNALSYM AEMSC_MOUSEDRAG}
const AEMSC_MOUSEMARKER = $8;  //Marker moving.
{$EXTERNALSYM AEMSC_MOUSEMARKER}

//AEM_GETMOUSESTATE selection
const AEMSS_LBUTTONUP = $1;  //WM_LBUTTONUP has been received.
{$EXTERNALSYM AEMSS_LBUTTONUP}
const AEMSS_CHARS = $2;  //Characters selection.
{$EXTERNALSYM AEMSS_CHARS}
const AEMSS_WORDS = $4;  //Words selection.
{$EXTERNALSYM AEMSS_WORDS}
const AEMSS_LINES = $8;  //Lines selection.
{$EXTERNALSYM AEMSS_LINES}

//AEM_FINDTEXT, AEM_ISMATCH flags
const AEFR_DOWN = $00000001;       //Same as FR_DOWN. If set, the search is from the beginning to the end of the search range. If not set, the search is from the end to the beginning of the search range.
{$EXTERNALSYM AEFR_DOWN}
const AEFR_WHOLEWORD = $00000002;  //Same as FR_WHOLEWORD. If set, the operation searches only for whole words that match the search string. If not set, the operation also searches for word fragments that match the search string.
{$EXTERNALSYM AEFR_WHOLEWORD}
const AEFR_MATCHCASE = $00000004;  //Same as FR_MATCHCASE. If set, the search operation is case-sensitive. If not set, the search operation is case-insensitive.
{$EXTERNALSYM AEFR_MATCHCASE}
const AEFR_REGEXP = $00080000;  //Regular expression search.
{$EXTERNALSYM AEFR_REGEXP}
const AEFR_REGEXPNONEWLINEDOT = $00100000;  //Symbol . specifies any character except new line.
{$EXTERNALSYM AEFR_REGEXPNONEWLINEDOT}
const AEFR_REGEXPMINMATCH = $00200000;  //Allow zero length match at string edges. For example: "^" at the string beginning or "$" at the string ending.
{$EXTERNALSYM AEFR_REGEXPMINMATCH}


//AEM_SETWORDWRAP flags
const AEWW_NONE = $00000000;  //Turn off wrap.
{$EXTERNALSYM AEWW_NONE}
const AEWW_WORD = $00000001;  //Wrap by words.
{$EXTERNALSYM AEWW_WORD}
const AEWW_SYMBOL = $00000002;  //Wrap by symbols.
{$EXTERNALSYM AEWW_SYMBOL}
const AEWW_LIMITPIXEL = $00000100;  //Limit in pixels (default).
{$EXTERNALSYM AEWW_LIMITPIXEL}
const AEWW_LIMITSYMBOL = $00000200;  //Limit in symbols.
{$EXTERNALSYM AEWW_LIMITSYMBOL}

//AEM_SETMARKER types
const AEMT_PIXEL = 0;  //Pixel integer.
{$EXTERNALSYM AEMT_PIXEL}
const AEMT_SYMBOL = 1;  //Column number.
{$EXTERNALSYM AEMT_SYMBOL}

//AEM_SETWORDBREAK flags
const AEWB_LEFTWORDSTART = $00000001;  //Left movement is stopped, when word start is found.
{$EXTERNALSYM AEWB_LEFTWORDSTART}
const AEWB_LEFTWORDEND = $00000002;  //Left movement is stopped, when word end is found.
{$EXTERNALSYM AEWB_LEFTWORDEND}
const AEWB_RIGHTWORDSTART = $00000004;  //Right movement is stopped, when word start is found.
{$EXTERNALSYM AEWB_RIGHTWORDSTART}
const AEWB_RIGHTWORDEND = $00000008;  //Right movement is stopped, when word end is found.
{$EXTERNALSYM AEWB_RIGHTWORDEND}
const AEWB_STOPSPACESTART = $00000010;  //Movement is stopped, when spacing start is found. Cannot be combined with AEWB_SKIPSPACESTART.
{$EXTERNALSYM AEWB_STOPSPACESTART}
const AEWB_STOPSPACEEND = $00000020;  //Movement is stopped, when spacing end is found. Cannot be combined with AEWB_SKIPSPACEEND.
{$EXTERNALSYM AEWB_STOPSPACEEND}
const AEWB_SKIPSPACESTART = $00000040;  //Movement is continued, when spacing start is found. Cannot be combined with AEWB_STOPSPACESTART.
{$EXTERNALSYM AEWB_SKIPSPACESTART}
const AEWB_SKIPSPACEEND = $00000080;  //Movement is continued, when spacing end is found. Cannot be combined with AEWB_STOPSPACEEND.
{$EXTERNALSYM AEWB_SKIPSPACEEND}
const AEWB_STOPNEWLINE = $00000100;  //Movement is stopped, when new line is found.
{$EXTERNALSYM AEWB_STOPNEWLINE}
const AEWB_MINMOVE = $00001000;  //Minimum movement or not move at all if flags matched.
{$EXTERNALSYM AEWB_MINMOVE}

//AEM_STREAMIN, AEM_STREAMOUT flags
const AESF_SELECTION = $00000001;  //Stream-in (read) or stream-out (write) the current selection. If not specified, stream-in (read) or stream-out (write) the entire contents of the control.
{$EXTERNALSYM AESF_SELECTION}
const AESF_FILLSPACES = $00000002;  //Stream-out (write) the current column selection with the filling empty spaces. Valid if bColumnSel member of a AESTREAMOUT structure is TRUE.
{$EXTERNALSYM AESF_FILLSPACES}

//AEM_ISRANGEMODIFIED return value
const AEIRM_UNMODIFIED = 1;
{$EXTERNALSYM AEIRM_UNMODIFIED}
const AEIRM_MODIFIEDUNSAVED = 2;
{$EXTERNALSYM AEIRM_MODIFIEDUNSAVED}
const AEIRM_MODIFIEDSAVED = 3;
{$EXTERNALSYM AEIRM_MODIFIEDSAVED}

{
  Fr0sT: these constants are defined in AkelDLL.pas so do not redefine them here

const FR_DOWN = $00000001;
const FR_WHOLEWORD = $00000002;
}

const FR_MATCHCASE = $00000004;
{$EXTERNALSYM FR_MATCHCASE}

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
{$EXTERNALSYM EM_SHOWSCROLLBAR}
const EM_GETSCROLLPOS = (WM_USER + 221);
{$EXTERNALSYM EM_GETSCROLLPOS}
const EM_SETSCROLLPOS = (WM_USER + 222);
{$EXTERNALSYM EM_SETSCROLLPOS}
const EM_SETTEXTEX = (WM_USER + 97);
{$EXTERNALSYM EM_SETTEXTEX}

const ST_DEFAULT = $0000;
{$EXTERNALSYM ST_DEFAULT}
const ST_KEEPUNDO = $0001;
{$EXTERNALSYM ST_KEEPUNDO}
const ST_SELECTION = $0002;
{$EXTERNALSYM ST_SELECTION}
const ST_NEWCHARS = $0004;
{$EXTERNALSYM ST_NEWCHARS}

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
  {$EXTERNALSYM AEHPRINT}
type
  AEHTHEME = THandle;
  {$EXTERNALSYM AEHTHEME}
type
  AEHDELIMITER = THandle;
  {$EXTERNALSYM AEHDELIMITER}
type
  AEHWORD = THandle;
  {$EXTERNALSYM AEHWORD}
type
  AEHQUOTE = THandle;
  {$EXTERNALSYM AEHQUOTE}
type
  AEHMARKTEXT = THandle;
  {$EXTERNALSYM AEHMARKTEXT}
type
  AEHMARKRANGE = THandle;
  {$EXTERNALSYM AEHMARKRANGE}

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
  {$EXTERNALSYM TCHARRANGE64}


type
  _TEXTRANGE64A = record
    chrg: TCHARRANGE64;
    lpstrText: LPSTR;
  end;
  TTEXTRANGE64A = _TEXTRANGE64A;
  {$EXTERNALSYM TTEXTRANGE64A}


type
  _TEXTRANGE64W = record
    chrg: TCHARRANGE64;
    lpstrText: LPWSTR;
  end;
  TTEXTRANGE64W = _TEXTRANGE64W;
  {$EXTERNALSYM TTEXTRANGE64W}


type
  _FINDTEXTEX64A = record
    chrg: TCHARRANGE64;
    lpstrText: LPCSTR;
    chrgText: TCHARRANGE64;
  end;
  TFINDTEXTEX64A = _FINDTEXTEX64A;
  {$EXTERNALSYM TFINDTEXTEX64A}


type
  _FINDTEXTEX64W = record
    chrg: TCHARRANGE64;
    lpstrText: LPCWSTR;
    chrgText: TCHARRANGE64;
  end;
  TFINDTEXTEX64W = _FINDTEXTEX64W;
  {$EXTERNALSYM TFINDTEXTEX64W}


type
  _GETTEXTEX64 = record
    cb: UINT_PTR;
    flags: DWORD;
    codepage: UINT;
    lpDefaultChar: LPCSTR;
    lpUsedDefChar: PBOOL;
  end;
  TGETTEXTEX64 = _GETTEXTEX64;
  {$EXTERNALSYM TGETTEXTEX64}


type
  _SELCHANGE64 = record
    nmhdr: NMHDR;
    chrg: TCHARRANGE;
    seltyp: WORD;
    chrg64: TCHARRANGE64;
  end;
  TSELCHANGE64 = _SELCHANGE64;
  {$EXTERNALSYM TSELCHANGE64}


type
  _ENDROPFILES64 = record
    nmhdr: NMHDR;
    hDrop: THandle;
    cp: LongInt;
    fProtected: BOOL;
    cp64: INT_PTR;
  end;
  TENDROPFILES64 = _ENDROPFILES64;
  {$EXTERNALSYM TENDROPFILES64}


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
  {$EXTERNALSYM TENLINK64}


//// Structures for x64 support

type
  _POINT64 = record
    x: INT_PTR;
    y: INT_PTR;
  end;
  TPOINT64 = _POINT64;
  {$EXTERNALSYM TPOINT64}


type
  _SIZE64 = record
    cx: INT_PTR;
    cy: INT_PTR;
  end;
  TSIZE64 = _SIZE64;
  {$EXTERNALSYM TSIZE64}


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
  end;
  TAESELECTION = _AESELECTION;
  {$EXTERNALSYM TAESELECTION}


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
  {$EXTERNALSYM TAEPOINT}


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
  {$EXTERNALSYM TAEFOLD}


type
  _AEFINDFOLD = record
    dwFlags: DWORD;         //[in]  See AEFF_* defines.
    dwFindIt: UINT_PTR;     //[in]  Depend on AEFF_FIND* define.
    lpParent: PAEFOLD;      //[out] Parent fold.
    lpPrevSubling: PAEFOLD; //[out] Previous subling fold.
  end;
  TAEFINDFOLD = _AEFINDFOLD;
  {$EXTERNALSYM TAEFINDFOLD}


type
  _AESETTEXTA = record
    pText: PAnsiChar;     //[in] Text to append.
    dwTextLen: UINT_PTR;  //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;    //[in] See AELB_* defines.
    nCodePage: Integer;   //[in] Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  end;
  TAESETTEXTA = _AESETTEXTA;
  {$EXTERNALSYM TAESETTEXTA}


type
  _AESETTEXTW = record
    pText: PWideChar;     //[in] Text to append.
    dwTextLen: UINT_PTR;  //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;    //[in] See AELB_* defines.
  end;
  TAESETTEXTW = _AESETTEXTW;
  {$EXTERNALSYM TAESETTEXTW}


type
  _AEAPPENDTEXTA = record
    pText: PAnsiChar;     //[in] Text to append.
    dwTextLen: UINT_PTR;  //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;    //[in] See AELB_* defines.
    nCodePage: Integer;   //[in] Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  end;
  TAEAPPENDTEXTA = _AEAPPENDTEXTA;
  {$EXTERNALSYM TAEAPPENDTEXTA}


type
  _AEAPPENDTEXTW = record
    pText: PWideChar;             //[in] Text to append.
    dwTextLen: UINT_PTR;          //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
    nNewLine: Integer;            //[in] See AELB_* defines.
  end;
  TAEAPPENDTEXTW = _AEAPPENDTEXTW;
  {$EXTERNALSYM TAEAPPENDTEXTW}


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
  {$EXTERNALSYM TAEREPLACESELA}


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
  {$EXTERNALSYM TAEREPLACESELW}


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
  {$EXTERNALSYM TAETEXTRANGEA}


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
  {$EXTERNALSYM TAETEXTRANGEW}


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
  {$EXTERNALSYM TAESTREAMIN}


type
  _AESTREAMOUT = record
    dwCookie: UINT_PTR;            //[in]  Specifies an application-defined value that the edit control passes to the AEStreamCallback function specified by the lpCallback member.
    dwError: DWORD;                //[out] Indicates the result of the stream-out (write) operation.
    lpCallback: TAEStreamCallback; //[in]  Pointer to an AEStreamCallback function, which is an application-defined function that the control calls to transfer data. The control calls the callback function repeatedly, transferring a portion of the data with each call.
    nNewLine: Integer;             //[in]  See AELB_* defines.
    bColumnSel: BOOL;              //[in]  Column selection. If this value is -1, use current selection type.
  end;
  TAESTREAMOUT = _AESTREAMOUT;
  {$EXTERNALSYM TAESTREAMOUT}


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
  {$EXTERNALSYM TAEFINDTEXTA}


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
  {$EXTERNALSYM TAEFINDTEXTW}

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
  {$EXTERNALSYM TAECHARCOLORS}


type
  _AEINDEXOFFSET = record
    ciCharIn: PAECHARINDEX;   //[in]  Input character index.
    ciCharOut: PAECHARINDEX;  //[out] Output character index (result).
    nOffset: INT_PTR;         //[in]  Offset can be positive or negative. For example, +1 will return next character, -1 will return previous character.
    nNewLine: Integer;        //[in]  See AELB_* defines.
  end;
  TAEINDEXOFFSET = _AEINDEXOFFSET;
  {$EXTERNALSYM TAEINDEXOFFSET}


type
  _AEINDEXSUBTRACT = record
    ciChar1: PAECHARINDEX;   //[in] First character index. If NULL, first character is used.
    ciChar2: PAECHARINDEX;   //[in] Second character index. If NULL, last character is used.
    bColumnSel: BOOL;        //[in] Column selection. If this value is -1, use current selection type.
    nNewLine: Integer;       //[in] See AELB_* defines.
  end;
  TAEINDEXSUBTRACT = _AEINDEXSUBTRACT;
  {$EXTERNALSYM TAEINDEXSUBTRACT}


type
  _AESCROLLTOPOINT = record
    dwFlags: DWORD;      //[in]     See AESC_* defines.
    ptPos: TPOINT64;     //[in,out] Point to scroll to, ignored if AESC_POINTCARET flag specified.
    nOffsetX: Integer;   //[in]     Horizontal scroll offset.
    nOffsetY: Integer;   //[in]     Vertical scroll offset.
  end;
  TAESCROLLTOPOINT = _AESCROLLTOPOINT;
  {$EXTERNALSYM TAESCROLLTOPOINT}


type
  _AESENDMESSAGE = record
    hDoc: AEHDOC;     //Document handle. See AEM_CREATEDOCUMENT message.
    uMsg: UINT;       //Window message.
    wParam: WPARAM;   //Window first additional parameter.
    lParam: LPARAM;   //Window second additional parameter.
    lResult: LRESULT; //cResult after window message returns.
  end;
  TAESENDMESSAGE = _AESENDMESSAGE;
  {$EXTERNALSYM TAESENDMESSAGE}


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
  {$EXTERNALSYM TAEPRINT}


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
  {$EXTERNALSYM TAEDELIMITEMA}


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
  {$EXTERNALSYM TAEDELIMITEMW}


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
  {$EXTERNALSYM TAEWORDITEMA}


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
  {$EXTERNALSYM TAEWORDITEMW}


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
  end;
  TAEQUOTEITEMA = _AEQUOTEITEMA;
  {$EXTERNALSYM TAEQUOTEITEMA}


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
    lpREGroupStack: Pointer;       //Don't use it. For internal code only.
  end;
  TAEQUOTEITEMW = _AEQUOTEITEMW;
  {$EXTERNALSYM TAEQUOTEITEMW}

type
  _AEREGROUPCOLOR = record
    dwFlags: DWORD;                //See AEREGCF_* defines.
    dwFontStyle: DWORD;            //See AEHLS_* defines.
    crText: COLORREF;              //Quote text color. If -1, then don't set.
    crBk: COLORREF;                //Quote background color. If -1, then don't set.
  end;
  TAEREGROUPCOLOR = _AEREGROUPCOLOR;
  {$EXTERNALSYM TAEREGROUPCOLOR}

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
  end;
  TAEMARKTEXTITEMA = _AEMARKTEXTITEMA;
  {$EXTERNALSYM TAEMARKTEXTITEMA}


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
  end;
  TAEMARKTEXTITEMW = _AEMARKTEXTITEMW;
  {$EXTERNALSYM TAEMARKTEXTITEMW}


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
  {$EXTERNALSYM TAEMARKRANGEITEM}



type
  _AEMARKTEXTMATCH = record
    lpMarkText: PAEMARKTEXTITEMW;
    crMarkText: TAECHARRANGE;
  end;
  TAEMARKTEXTMATCH = _AEMARKTEXTMATCH;
  {$EXTERNALSYM TAEMARKTEXTMATCH}


type
  _AEMARKRANGEMATCH = record
    lpMarkRange: PAEMARKRANGEITEM;
    crMarkRange: TCHARRANGE64;
  end;
  TAEMARKRANGEMATCH = _AEMARKRANGEMATCH;
  {$EXTERNALSYM TAEMARKRANGEMATCH}


type
  AEQUOTEMATCH = record
    lpQuote: PAEQUOTEITEMW;
    crQuoteStart: TAECHARRANGE;
    crQuoteEnd: TAECHARRANGE;
  end;
  TAEQUOTEMATCH = AEQUOTEMATCH;
  {$EXTERNALSYM TAEQUOTEMATCH}


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
  {$EXTERNALSYM TAEWORDMATCH}


type
  AEFOLDMATCH = record 
    crFold: TCHARRANGE64;
    lpFold: PAEFOLD;
  end;
  TAEFOLDMATCH = AEFOLDMATCH;
  {$EXTERNALSYM TAEFOLDMATCH}


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
  {$EXTERNALSYM TAEHLPAINT}


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
  {$EXTERNALSYM TAEGETHIGHLIGHT}


type
  _AENMHDR = record
    hwndFrom: HWND;
    idFrom: UINT_PTR;
    code: UINT;
    docFrom: AEHDOC;      //Document handle. See AEM_CREATEDOCUMENT message.
  end;
  TAENMHDR = _AENMHDR;
  {$EXTERNALSYM TAENMHDR}


type
  _AENERRSPACE = record
    hdr: TAENMHDR;
    dwBytes: SIZE_T;      //Number of bytes that cannot be allocated.
  end;
  TAENERRSPACE = _AENERRSPACE;
  {$EXTERNALSYM TAENERRSPACE}


type
  _AENFOCUS = record
    hdr: TAENMHDR;
    hWndChange: HWND;     //AEN_SETFOCUS - handle to the window that has lost the keyboard focus.
  end;
  TAENFOCUS = _AENFOCUS;
  {$EXTERNALSYM TAENFOCUS}


type
  _AENSCROLL = record
    hdr: TAENMHDR;
    nPosNew: INT_PTR;     //Current scroll position.
    nPosOld: INT_PTR;     //Previous scroll position.
    nPosMax: INT_PTR;     //Maximum scroll position.
  end;
  TAENSCROLL = _AENSCROLL;
  {$EXTERNALSYM TAENSCROLL}


type
  _AENSETRECT = record
    hdr: TAENMHDR;
    rcDraw: TRect;         //Draw rectangle.
    rcEdit: TRect;         //Edit client rectangle.
  end;
  TAENSETRECT = _AENSETRECT;
  {$EXTERNALSYM TAENSETRECT}


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
  {$EXTERNALSYM TAENPAINT}


type
  _AENMAXTEXT = record
    hdr: TAENMHDR;
    dwTextLimit: UINT_PTR;   //Current text limit.
  end;
  TAENMAXTEXT = _AENMAXTEXT;
  {$EXTERNALSYM TAENMAXTEXT}


type
  _AENPROGRESS = record
    hdr: TAENMHDR;
    dwType: DWORD;        //See AEPGS_* defines.
    dwTimeElapsed: DWORD; //Elapsed time since action was start.
    nCurrent: INT_PTR;    //Characters processed. Equal to zero, if first message.
    nMaximum: INT_PTR;    //Total number of characters. Equal to nCurrent member, if last message.
  end;
  TAENPROGRESS = _AENPROGRESS;
  {$EXTERNALSYM TAENPROGRESS}


type
  _AENMODIFY = record
    hdr: TAENMHDR;
    bModified: BOOL;      //TRUE document state is set to modified, FALSE document state is set to unmodified.
  end;
  TAENMODIFY = _AENMODIFY;
  {$EXTERNALSYM TAENMODIFY}


type
  _AENSELCHANGE = record
    hdr: TAENMHDR;
    aes: TAESELECTION;      //Current selection.
    ciCaret: TAECHARINDEX;  //Caret character index position.
    dwType: DWORD;         //See AESCT_* defines.
  end;
  TAENSELCHANGE = _AENSELCHANGE;
  {$EXTERNALSYM TAENSELCHANGE}


type
  _AENTEXTCHANGE = record
    hdr: TAENMHDR;
    aes: TAESELECTION;      //Current selection.
    ciCaret: TAECHARINDEX;  //Caret character index position.
    dwType: DWORD;         //See AETCT_* defines.
  end;
  TAENTEXTCHANGE = _AENTEXTCHANGE;
  {$EXTERNALSYM TAENTEXTCHANGE}


type
  _AENTEXTINSERT = record
    hdr: TAENMHDR;
    aes: TAESELECTION;         //Current selection.
    ciCaret: TAECHARINDEX;     //Caret character index position.
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
  {$EXTERNALSYM TAENTEXTINSERT}


type
  _AENTEXTDELETE = record
    hdr: TAENMHDR;
    aes: TAESELECTION;         //Current selection.
    ciCaret: TAECHARINDEX;     //Caret character index position.
    dwType: DWORD;            //See AETCT_* defines.
    bColumnSel: BOOL;         //Column selection.
    dwDeleteFlags: DWORD;     //See AEDELT_* defines.
    crAkelRange: TAECHARRANGE; //AEN_TEXTDELETEBEGIN - text delete range or AEN_TEXTDELETEEND - text range after deletion.
    crRichRange: TCHARRANGE64; //AEN_TEXTDELETEBEGIN - text delete range or AEN_TEXTDELETEEND - text range after deletion (RichEdit offset).
  end;
  TAENTEXTDELETE = _AENTEXTDELETE;
  {$EXTERNALSYM TAENTEXTDELETE}


type
  _AENPOINT = record
    hdr: TAENMHDR;
    dwType: DWORD;        //See AEPTT_* defines.
    lpPoint: PAEPOINT;    //Pointer to a AEPOINT structure. NULL if type is AEPTT_SETTEXT or AEPTT_STREAMIN.
  end;
  TAENPOINT = _AENPOINT;
  {$EXTERNALSYM TAENPOINT}


type
  _AENDROPFILES = record
    hdr: TAENMHDR;
    hDrop: HDROP;         //Handle to the dropped files list (same as with WM_DROPFILES).
    ciChar: TAECHARINDEX;  //Character index at which the dropped files would be inserted.
  end;
  TAENDROPFILES = _AENDROPFILES;
  {$EXTERNALSYM TAENDROPFILES}


type
  _AENDROPSOURCE = record
    hdr: TAENMHDR;
    nAction: Integer;         //See AEDS_* defines.
    dwEffect: DWORD;      //Cursor effect: DROPEFFECT_COPY, DROPEFFECT_MOVE or DROPEFFECT_NONE.
    dwDropResult: DWORD;  //Drop cResult. Valid if nAction equal to AEDS_SOURCEEND or AEDS_SOURCEDONE.
  end;
  TAENDROPSOURCE = _AENDROPSOURCE;
  {$EXTERNALSYM TAENDROPSOURCE}


type
  _AENDROPTARGET = record
    hdr: TAENMHDR;
    nAction: Integer;         //See AEDT_* defines.
    pt: TPoint;            //Cursor position in screen coordinates.
    dwEffect: DWORD;      //Cursor effect: DROPEFFECT_COPY, DROPEFFECT_MOVE or DROPEFFECT_NONE.
  end;
  TAENDROPTARGET = _AENDROPTARGET;
  {$EXTERNALSYM TAENDROPTARGET}


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
  {$EXTERNALSYM TAENLINK}


type
  _AENMARKER = record
    hdr: TAENMHDR;
    dwType: DWORD; //Column marker cType.
    dwPos: DWORD;  //Column marker position.
    bMouse: BOOL;  //Column marker position is changed with mouse.
  end;
  TAENMARKER = _AENMARKER;
  {$EXTERNALSYM TAENMARKER}


//// AkelEdit messages

//Error notifications
const AEN_ERRSPACE = (WM_USER + 1001);  //0x7E9
{$EXTERNALSYM AEN_ERRSPACE}

//Window notifications
const AEN_SETFOCUS = (WM_USER + 1026);  //0x802
{$EXTERNALSYM AEN_SETFOCUS}
const AEN_KILLFOCUS = (WM_USER + 1027);  //0x803
{$EXTERNALSYM AEN_KILLFOCUS}
const AEN_HSCROLL = (WM_USER + 1028);  //0x804
{$EXTERNALSYM AEN_HSCROLL}
const AEN_VSCROLL = (WM_USER + 1029);  //0x805
{$EXTERNALSYM AEN_VSCROLL}
const AEN_SETRECT = (WM_USER + 1030);  //0x806
{$EXTERNALSYM AEN_SETRECT}
const AEN_PAINT = (WM_USER + 1031);  //0x807
{$EXTERNALSYM AEN_PAINT}

//Text notifications
const AEN_MAXTEXT = (WM_USER + 1050);  //0x81A
{$EXTERNALSYM AEN_MAXTEXT}
const AEN_PROGRESS = (WM_USER + 1051);  //0x81B
{$EXTERNALSYM AEN_PROGRESS}
const AEN_MODIFY = (WM_USER + 1052);  //0x81C
{$EXTERNALSYM AEN_MODIFY}
const AEN_SELCHANGING = (WM_USER + 1053);  //0x81D
{$EXTERNALSYM AEN_SELCHANGING}
const AEN_SELCHANGED = (WM_USER + 1054);  //0x81E
{$EXTERNALSYM AEN_SELCHANGED}
const AEN_TEXTCHANGING = (WM_USER + 1055);  //0x81F
{$EXTERNALSYM AEN_TEXTCHANGING}
const AEN_TEXTINSERTBEGIN = (WM_USER + 1056);  //0x820
{$EXTERNALSYM AEN_TEXTINSERTBEGIN}
const AEN_TEXTINSERTEND = (WM_USER + 1057);  //0x821
{$EXTERNALSYM AEN_TEXTINSERTEND}
const AEN_TEXTDELETEBEGIN = (WM_USER + 1058);  //0x822
{$EXTERNALSYM AEN_TEXTDELETEBEGIN}
const AEN_TEXTDELETEEND = (WM_USER + 1059);  //0x823
{$EXTERNALSYM AEN_TEXTDELETEEND}
const AEN_TEXTCHANGED = (WM_USER + 1060);  //0x824
{$EXTERNALSYM AEN_TEXTCHANGED}
const AEN_POINT = (WM_USER + 1061);  //0x825
{$EXTERNALSYM AEN_POINT}

//Mouse notifications
const AEN_DROPFILES = (WM_USER + 1076);  //0x834
{$EXTERNALSYM AEN_DROPFILES}
const AEN_DROPSOURCE = (WM_USER + 1077);  //0x835
{$EXTERNALSYM AEN_DROPSOURCE}
const AEN_DROPTARGET = (WM_USER + 1078);  //0x836
{$EXTERNALSYM AEN_DROPTARGET}
const AEN_LINK = (WM_USER + 1079);  //0x837
{$EXTERNALSYM AEN_LINK}
const AEN_MARKER = (WM_USER + 1080);  //0x838
{$EXTERNALSYM AEN_MARKER}

//RichEdit Unicode extension
const EM_REPLACESELA = (WM_USER + 1901);
{$EXTERNALSYM EM_REPLACESELA}
const EM_REPLACESELW = (WM_USER + 1902);
{$EXTERNALSYM EM_REPLACESELW}
const EM_GETSELTEXTA = (WM_USER + 1905);
{$EXTERNALSYM EM_GETSELTEXTA}
const EM_GETSELTEXTW = (WM_USER + 1906);
{$EXTERNALSYM EM_GETSELTEXTW}
const EM_GETLINEA = (WM_USER + 1907);
{$EXTERNALSYM EM_GETLINEA}
const EM_GETLINEW = (WM_USER + 1908);
{$EXTERNALSYM EM_GETLINEW}

//RichEdit x64 extension
const EM_GETSEL64 = (WM_USER + 1951);
{$EXTERNALSYM EM_GETSEL64}
const EM_EXGETSEL64 = (WM_USER + 1952);
{$EXTERNALSYM EM_EXGETSEL64}
const EM_EXSETSEL64 = (WM_USER + 1953);
{$EXTERNALSYM EM_EXSETSEL64}
const EM_FINDTEXTEX64 = (WM_USER + 1954);
{$EXTERNALSYM EM_FINDTEXTEX64}
const EM_FINDTEXTEX64A = (WM_USER + 1955);
{$EXTERNALSYM EM_FINDTEXTEX64A}
const EM_FINDTEXTEX64W = (WM_USER + 1956);
{$EXTERNALSYM EM_FINDTEXTEX64W}
const EM_GETTEXTRANGE64 = (WM_USER + 1961);
{$EXTERNALSYM EM_GETTEXTRANGE64}
const EM_GETTEXTRANGE64A = (WM_USER + 1962);
{$EXTERNALSYM EM_GETTEXTRANGE64A}
const EM_GETTEXTRANGE64W = (WM_USER + 1963);
{$EXTERNALSYM EM_GETTEXTRANGE64W}
const EM_GETTEXTEX64 = (WM_USER + 1964);
{$EXTERNALSYM EM_GETTEXTEX64}

//Text retrieval and modification
const AEM_EXSETTEXTA = (WM_USER + 1999);
{$EXTERNALSYM AEM_EXSETTEXTA}
const AEM_EXSETTEXTW = (WM_USER + 2000);
{$EXTERNALSYM AEM_EXSETTEXTW}
const AEM_SETTEXTA = (WM_USER + 2001);
{$EXTERNALSYM AEM_SETTEXTA}
const AEM_SETTEXTW = (WM_USER + 2002);
{$EXTERNALSYM AEM_SETTEXTW}
const AEM_APPENDTEXTA = (WM_USER + 2003);
{$EXTERNALSYM AEM_APPENDTEXTA}
const AEM_APPENDTEXTW = (WM_USER + 2004);
{$EXTERNALSYM AEM_APPENDTEXTW}
const AEM_REPLACESELA = (WM_USER + 2005);
{$EXTERNALSYM AEM_REPLACESELA}
const AEM_REPLACESELW = (WM_USER + 2006);
{$EXTERNALSYM AEM_REPLACESELW}
const AEM_GETTEXTRANGEA = (WM_USER + 2007);
{$EXTERNALSYM AEM_GETTEXTRANGEA}
const AEM_GETTEXTRANGEW = (WM_USER + 2008);
{$EXTERNALSYM AEM_GETTEXTRANGEW}
const AEM_STREAMIN = (WM_USER + 2009);
{$EXTERNALSYM AEM_STREAMIN}
const AEM_STREAMOUT = (WM_USER + 2010);
{$EXTERNALSYM AEM_STREAMOUT}
const AEM_CANPASTE = (WM_USER + 2011);
{$EXTERNALSYM AEM_CANPASTE}
const AEM_PASTE = (WM_USER + 2012);
{$EXTERNALSYM AEM_PASTE}
const AEM_CUT = (WM_USER + 2013);
{$EXTERNALSYM AEM_CUT}
const AEM_COPY = (WM_USER + 2014);
{$EXTERNALSYM AEM_COPY}
const AEM_CHECKCODEPAGE = (WM_USER + 2015);
{$EXTERNALSYM AEM_CHECKCODEPAGE}
const AEM_FINDTEXTA = (WM_USER + 2016);
{$EXTERNALSYM AEM_FINDTEXTA}
const AEM_FINDTEXTW = (WM_USER + 2017);
{$EXTERNALSYM AEM_FINDTEXTW}
const AEM_ISMATCHA = (WM_USER + 2018);
{$EXTERNALSYM AEM_ISMATCHA}
const AEM_ISMATCHW = (WM_USER + 2019);
{$EXTERNALSYM AEM_ISMATCHW}
const AEM_KEYDOWN = (WM_USER + 2020);
{$EXTERNALSYM AEM_KEYDOWN}
const AEM_INSERTCHAR = (WM_USER + 2021);
{$EXTERNALSYM AEM_INSERTCHAR}
const AEM_CHARAT = (WM_USER + 2022);
{$EXTERNALSYM AEM_CHARAT}
const AEM_INPUTLANGUAGE = (WM_USER + 2023);
{$EXTERNALSYM AEM_INPUTLANGUAGE}
const AEM_DRAGDROP = (WM_USER + 2024);
{$EXTERNALSYM AEM_DRAGDROP}

//Undo and Redo
const AEM_CANUNDO = (WM_USER + 2051);
{$EXTERNALSYM AEM_CANUNDO}
const AEM_CANREDO = (WM_USER + 2052);
{$EXTERNALSYM AEM_CANREDO}
const AEM_UNDO = (WM_USER + 2053);
{$EXTERNALSYM AEM_UNDO}
const AEM_REDO = (WM_USER + 2054);
{$EXTERNALSYM AEM_REDO}
const AEM_EMPTYUNDOBUFFER = (WM_USER + 2055);
{$EXTERNALSYM AEM_EMPTYUNDOBUFFER}
const AEM_STOPGROUPTYPING = (WM_USER + 2056);
{$EXTERNALSYM AEM_STOPGROUPTYPING}
const AEM_BEGINUNDOACTION = (WM_USER + 2057);
{$EXTERNALSYM AEM_BEGINUNDOACTION}
const AEM_ENDUNDOACTION = (WM_USER + 2058);
{$EXTERNALSYM AEM_ENDUNDOACTION}
const AEM_LOCKCOLLECTUNDO = (WM_USER + 2059);
{$EXTERNALSYM AEM_LOCKCOLLECTUNDO}
const AEM_GETUNDOLIMIT = (WM_USER + 2060);
{$EXTERNALSYM AEM_GETUNDOLIMIT}
const AEM_SETUNDOLIMIT = (WM_USER + 2061);
{$EXTERNALSYM AEM_SETUNDOLIMIT}
const AEM_GETMODIFY = (WM_USER + 2062);
{$EXTERNALSYM AEM_GETMODIFY}
const AEM_SETMODIFY = (WM_USER + 2063);
{$EXTERNALSYM AEM_SETMODIFY}
const AEM_UNDOBUFFERSIZE = (WM_USER + 2064);
{$EXTERNALSYM AEM_UNDOBUFFERSIZE}
const AEM_ISRANGEMODIFIED = (WM_USER + 2065);
{$EXTERNALSYM AEM_ISRANGEMODIFIED}

//Text coordinates
const AEM_EXGETSEL = (WM_USER + 2099);
{$EXTERNALSYM AEM_EXGETSEL}
const AEM_EXSETSEL = (WM_USER + 2100);
{$EXTERNALSYM AEM_EXSETSEL}
const AEM_GETSEL = (WM_USER + 2101);
{$EXTERNALSYM AEM_GETSEL}
const AEM_SETSEL = (WM_USER + 2102);
{$EXTERNALSYM AEM_SETSEL}
const AEM_GETCOLUMNSEL = (WM_USER + 2103);
{$EXTERNALSYM AEM_GETCOLUMNSEL}
const AEM_UPDATESEL = (WM_USER + 2104);
{$EXTERNALSYM AEM_UPDATESEL}
const AEM_GETLINENUMBER = (WM_USER + 2105);
{$EXTERNALSYM AEM_GETLINENUMBER}
const AEM_GETINDEX = (WM_USER + 2106);
{$EXTERNALSYM AEM_GETINDEX}
const AEM_GETLINEINDEX = (WM_USER + 2107);
{$EXTERNALSYM AEM_GETLINEINDEX}
const AEM_INDEXUPDATE = (WM_USER + 2108);
{$EXTERNALSYM AEM_INDEXUPDATE}
const AEM_INDEXCOMPARE = (WM_USER + 2109);
{$EXTERNALSYM AEM_INDEXCOMPARE}
const AEM_INDEXSUBTRACT = (WM_USER + 2110);
{$EXTERNALSYM AEM_INDEXSUBTRACT}
const AEM_INDEXOFFSET = (WM_USER + 2111);
{$EXTERNALSYM AEM_INDEXOFFSET}
const AEM_INDEXTORICHOFFSET = (WM_USER + 2112);
{$EXTERNALSYM AEM_INDEXTORICHOFFSET}
const AEM_RICHOFFSETTOINDEX = (WM_USER + 2113);
{$EXTERNALSYM AEM_RICHOFFSETTOINDEX}
const AEM_GETRICHOFFSET = (WM_USER + 2114);
{$EXTERNALSYM AEM_GETRICHOFFSET}
const AEM_GETWRAPLINE = (WM_USER + 2118);
{$EXTERNALSYM AEM_GETWRAPLINE}
const AEM_GETUNWRAPLINE = (WM_USER + 2119);
{$EXTERNALSYM AEM_GETUNWRAPLINE}
const AEM_GETNEXTBREAK = (WM_USER + 2120);
{$EXTERNALSYM AEM_GETNEXTBREAK}
const AEM_GETPREVBREAK = (WM_USER + 2121);
{$EXTERNALSYM AEM_GETPREVBREAK}
const AEM_ISDELIMITER = (WM_USER + 2122);
{$EXTERNALSYM AEM_ISDELIMITER}
const AEM_INDEXTOCOLUMN = (WM_USER + 2123);
{$EXTERNALSYM AEM_INDEXTOCOLUMN}
const AEM_COLUMNTOINDEX = (WM_USER + 2124);
{$EXTERNALSYM AEM_COLUMNTOINDEX}
const AEM_INDEXINURL = (WM_USER + 2125);
{$EXTERNALSYM AEM_INDEXINURL}
const AEM_ADDPOINT = (WM_USER + 2141);
{$EXTERNALSYM AEM_ADDPOINT}
const AEM_DELPOINT = (WM_USER + 2142);
{$EXTERNALSYM AEM_DELPOINT}
const AEM_GETPOINTSTACK = (WM_USER + 2143);
{$EXTERNALSYM AEM_GETPOINTSTACK}

//Screen coordinates
const AEM_CHARFROMGLOBALPOS = (WM_USER + 2149);
{$EXTERNALSYM AEM_CHARFROMGLOBALPOS}
const AEM_GLOBALPOSFROMCHAR = (WM_USER + 2150);
{$EXTERNALSYM AEM_GLOBALPOSFROMCHAR}
const AEM_CHARFROMPOS = (WM_USER + 2151);
{$EXTERNALSYM AEM_CHARFROMPOS}
const AEM_POSFROMCHAR = (WM_USER + 2152);
{$EXTERNALSYM AEM_POSFROMCHAR}
const AEM_GETRECT = (WM_USER + 2153);
{$EXTERNALSYM AEM_GETRECT}
const AEM_SETRECT = (WM_USER + 2154);
{$EXTERNALSYM AEM_SETRECT}
const AEM_GETSCROLLPOS = (WM_USER + 2155);
{$EXTERNALSYM AEM_GETSCROLLPOS}
const AEM_SETSCROLLPOS = (WM_USER + 2156);
{$EXTERNALSYM AEM_SETSCROLLPOS}
const AEM_SCROLL = (WM_USER + 2157);
{$EXTERNALSYM AEM_SCROLL}
const AEM_LINESCROLL = (WM_USER + 2158);
{$EXTERNALSYM AEM_LINESCROLL}
const AEM_SCROLLTOPOINT = (WM_USER + 2159);
{$EXTERNALSYM AEM_SCROLLTOPOINT}
const AEM_LOCKSCROLL = (WM_USER + 2161);
{$EXTERNALSYM AEM_LOCKSCROLL}
const AEM_GETCHARSIZE = (WM_USER + 2164);
{$EXTERNALSYM AEM_GETCHARSIZE}
const AEM_GETSTRWIDTH = (WM_USER + 2165);
{$EXTERNALSYM AEM_GETSTRWIDTH}
const AEM_GETCARETPOS = (WM_USER + 2166);
{$EXTERNALSYM AEM_GETCARETPOS}
const AEM_GETCARETHORZINDENT = (WM_USER + 2167);
{$EXTERNALSYM AEM_GETCARETHORZINDENT}
const AEM_SETCARETHORZINDENT = (WM_USER + 2168);
{$EXTERNALSYM AEM_SETCARETHORZINDENT}
const AEM_CONVERTPOINT = (WM_USER + 2169);
{$EXTERNALSYM AEM_CONVERTPOINT}
const AEM_POINTONMARGIN = (WM_USER + 2170);
{$EXTERNALSYM AEM_POINTONMARGIN}
const AEM_POINTONSELECTION = (WM_USER + 2171);
{$EXTERNALSYM AEM_POINTONSELECTION}
const AEM_POINTONURL = (WM_USER + 2172);
{$EXTERNALSYM AEM_POINTONURL}
const AEM_LINEFROMVPOS = (WM_USER + 2173);
{$EXTERNALSYM AEM_LINEFROMVPOS}
const AEM_VPOSFROMLINE = (WM_USER + 2174);
{$EXTERNALSYM AEM_VPOSFROMLINE}
const AEM_GETMOUSESTATE = (WM_USER + 2175);
{$EXTERNALSYM AEM_GETMOUSESTATE}

//Options
const AEM_CONTROLCLASS = (WM_USER + 2199);
{$EXTERNALSYM AEM_CONTROLCLASS}
const AEM_CONTROLVERSION = (WM_USER + 2200);
{$EXTERNALSYM AEM_CONTROLVERSION}
const AEM_GETEVENTMASK = (WM_USER + 2201);
{$EXTERNALSYM AEM_GETEVENTMASK}
const AEM_SETEVENTMASK = (WM_USER + 2202);
{$EXTERNALSYM AEM_SETEVENTMASK}
const AEM_GETOPTIONS = (WM_USER + 2203);
{$EXTERNALSYM AEM_GETOPTIONS}
const AEM_SETOPTIONS = (WM_USER + 2204);
{$EXTERNALSYM AEM_SETOPTIONS}
const AEM_GETNEWLINE = (WM_USER + 2205);
{$EXTERNALSYM AEM_GETNEWLINE}
const AEM_SETNEWLINE = (WM_USER + 2206);
{$EXTERNALSYM AEM_SETNEWLINE}
const AEM_GETCOLORS = (WM_USER + 2207);
{$EXTERNALSYM AEM_GETCOLORS}
const AEM_SETCOLORS = (WM_USER + 2208);
{$EXTERNALSYM AEM_SETCOLORS}
const AEM_EXGETOPTIONS = (WM_USER + 2209);
{$EXTERNALSYM AEM_EXGETOPTIONS}
const AEM_EXSETOPTIONS = (WM_USER + 2210);
{$EXTERNALSYM AEM_EXSETOPTIONS}
const AEM_GETCARETWIDTH = (WM_USER + 2213);
{$EXTERNALSYM AEM_GETCARETWIDTH}
const AEM_SETCARETWIDTH = (WM_USER + 2214);
{$EXTERNALSYM AEM_SETCARETWIDTH}
const AEM_GETTABSTOP = (WM_USER + 2215);
{$EXTERNALSYM AEM_GETTABSTOP}
const AEM_SETTABSTOP = (WM_USER + 2216);
{$EXTERNALSYM AEM_SETTABSTOP}
const AEM_GETWORDWRAP = (WM_USER + 2217);
{$EXTERNALSYM AEM_GETWORDWRAP}
const AEM_SETWORDWRAP = (WM_USER + 2218);
{$EXTERNALSYM AEM_SETWORDWRAP}
const AEM_GETWORDDELIMITERS = (WM_USER + 2219);
{$EXTERNALSYM AEM_GETWORDDELIMITERS}
const AEM_SETWORDDELIMITERS = (WM_USER + 2220);
{$EXTERNALSYM AEM_SETWORDDELIMITERS}
const AEM_GETWRAPDELIMITERS = (WM_USER + 2221);
{$EXTERNALSYM AEM_GETWRAPDELIMITERS}
const AEM_SETWRAPDELIMITERS = (WM_USER + 2222);
{$EXTERNALSYM AEM_SETWRAPDELIMITERS}
const AEM_GETURLLEFTDELIMITERS = (WM_USER + 2223);
{$EXTERNALSYM AEM_GETURLLEFTDELIMITERS}
const AEM_SETURLLEFTDELIMITERS = (WM_USER + 2224);
{$EXTERNALSYM AEM_SETURLLEFTDELIMITERS}
const AEM_GETURLRIGHTDELIMITERS = (WM_USER + 2225);
{$EXTERNALSYM AEM_GETURLRIGHTDELIMITERS}
const AEM_SETURLRIGHTDELIMITERS = (WM_USER + 2226);
{$EXTERNALSYM AEM_SETURLRIGHTDELIMITERS}
const AEM_GETURLPREFIXES = (WM_USER + 2227);
{$EXTERNALSYM AEM_GETURLPREFIXES}
const AEM_SETURLPREFIXES = (WM_USER + 2228);
{$EXTERNALSYM AEM_SETURLPREFIXES}
const AEM_GETURLMAXLENGTH = (WM_USER + 2229);
{$EXTERNALSYM AEM_GETURLMAXLENGTH}
const AEM_SETURLMAXLENGTH = (WM_USER + 2230);
{$EXTERNALSYM AEM_SETURLMAXLENGTH}
const AEM_GETWORDBREAK = (WM_USER + 2231);
{$EXTERNALSYM AEM_GETWORDBREAK}
const AEM_SETWORDBREAK = (WM_USER + 2232);
{$EXTERNALSYM AEM_SETWORDBREAK}
const AEM_GETMARKER = (WM_USER + 2233);
{$EXTERNALSYM AEM_GETMARKER}
const AEM_SETMARKER = (WM_USER + 2234);
{$EXTERNALSYM AEM_SETMARKER}
const AEM_GETLINEGAP = (WM_USER + 2235);
{$EXTERNALSYM AEM_GETLINEGAP}
const AEM_SETLINEGAP = (WM_USER + 2236);
{$EXTERNALSYM AEM_SETLINEGAP}
const AEM_GETTEXTLIMIT = (WM_USER + 2237);
{$EXTERNALSYM AEM_GETTEXTLIMIT}
const AEM_SETTEXTLIMIT = (WM_USER + 2238);
{$EXTERNALSYM AEM_SETTEXTLIMIT}
const AEM_GETFONT = (WM_USER + 2239);
{$EXTERNALSYM AEM_GETFONT}
const AEM_GETALTLINE = (WM_USER + 2240);
{$EXTERNALSYM AEM_GETALTLINE}
const AEM_SETALTLINE = (WM_USER + 2241);
{$EXTERNALSYM AEM_SETALTLINE}
const AEM_GETCHARCOLORS = (WM_USER + 2242);
{$EXTERNALSYM AEM_GETCHARCOLORS}

//Draw
const AEM_SHOWSCROLLBAR = (WM_USER + 2351);
{$EXTERNALSYM AEM_SHOWSCROLLBAR}
const AEM_UPDATESCROLLBAR = (WM_USER + 2352);
{$EXTERNALSYM AEM_UPDATESCROLLBAR}
const AEM_UPDATECARET = (WM_USER + 2353);
{$EXTERNALSYM AEM_UPDATECARET}
const AEM_UPDATESIZE = (WM_USER + 2354);
{$EXTERNALSYM AEM_UPDATESIZE}
const AEM_LOCKUPDATE = (WM_USER + 2355);
{$EXTERNALSYM AEM_LOCKUPDATE}
const AEM_LOCKERASERECT = (WM_USER + 2356);
{$EXTERNALSYM AEM_LOCKERASERECT}
const AEM_GETERASERECT = (WM_USER + 2357);
{$EXTERNALSYM AEM_GETERASERECT}
const AEM_SETERASERECT = (WM_USER + 2358);
{$EXTERNALSYM AEM_SETERASERECT}
const AEM_HIDESELECTION = (WM_USER + 2361);
{$EXTERNALSYM AEM_HIDESELECTION}
const AEM_REDRAWLINERANGE = (WM_USER + 2362);
{$EXTERNALSYM AEM_REDRAWLINERANGE}
const AEM_GETBACKGROUNDIMAGE = (WM_USER + 2366);
{$EXTERNALSYM AEM_GETBACKGROUNDIMAGE}
const AEM_SETBACKGROUNDIMAGE = (WM_USER + 2367);
{$EXTERNALSYM AEM_SETBACKGROUNDIMAGE}

//Folding
const AEM_GETFOLDSTACK = (WM_USER + 2381);
{$EXTERNALSYM AEM_GETFOLDSTACK}
const AEM_GETFOLDCOUNT = (WM_USER + 2382);
{$EXTERNALSYM AEM_GETFOLDCOUNT}
const AEM_ADDFOLD = (WM_USER + 2383);
{$EXTERNALSYM AEM_ADDFOLD}
const AEM_DELETEFOLD = (WM_USER + 2384);
{$EXTERNALSYM AEM_DELETEFOLD}
const AEM_ISFOLDVALID = (WM_USER + 2385);
{$EXTERNALSYM AEM_ISFOLDVALID}
const AEM_FINDFOLD = (WM_USER + 2386);
{$EXTERNALSYM AEM_FINDFOLD}
const AEM_NEXTFOLD = (WM_USER + 2387);
{$EXTERNALSYM AEM_NEXTFOLD}
const AEM_PREVFOLD = (WM_USER + 2388);
{$EXTERNALSYM AEM_PREVFOLD}
const AEM_ISLINECOLLAPSED = (WM_USER + 2389);
{$EXTERNALSYM AEM_ISLINECOLLAPSED}
const AEM_COLLAPSELINE = (WM_USER + 2390);
{$EXTERNALSYM AEM_COLLAPSELINE}
const AEM_COLLAPSEFOLD = (WM_USER + 2391);
{$EXTERNALSYM AEM_COLLAPSEFOLD}
const AEM_UPDATEFOLD = (WM_USER + 2392);
{$EXTERNALSYM AEM_UPDATEFOLD}

//Document
const AEM_CREATEDOCUMENT = (WM_USER + 2401);
{$EXTERNALSYM AEM_CREATEDOCUMENT}
const AEM_DELETEDOCUMENT = (WM_USER + 2402);
{$EXTERNALSYM AEM_DELETEDOCUMENT}
const AEM_GETDOCUMENT = (WM_USER + 2403);
{$EXTERNALSYM AEM_GETDOCUMENT}
const AEM_SETDOCUMENT = (WM_USER + 2404);
{$EXTERNALSYM AEM_SETDOCUMENT}
const AEM_GETDOCUMENTPROC = (WM_USER + 2405);
{$EXTERNALSYM AEM_GETDOCUMENTPROC}
const AEM_GETDOCUMENTWINDOW = (WM_USER + 2406);
{$EXTERNALSYM AEM_GETDOCUMENTWINDOW}
const AEM_SENDMESSAGE = (WM_USER + 2407);
{$EXTERNALSYM AEM_SENDMESSAGE}

//Clone
const AEM_ADDCLONE = (WM_USER + 2421);
{$EXTERNALSYM AEM_ADDCLONE}
const AEM_DELCLONE = (WM_USER + 2422);
{$EXTERNALSYM AEM_DELCLONE}
const AEM_GETMASTER = (WM_USER + 2423);
{$EXTERNALSYM AEM_GETMASTER}
const AEM_GETCLONE = (WM_USER + 2424);
{$EXTERNALSYM AEM_GETCLONE}

//Print
const AEM_STARTPRINTDOC = (WM_USER + 2451);
{$EXTERNALSYM AEM_STARTPRINTDOC}
const AEM_PRINTPAGE = (WM_USER + 2452);
{$EXTERNALSYM AEM_PRINTPAGE}
const AEM_ENDPRINTDOC = (WM_USER + 2453);
{$EXTERNALSYM AEM_ENDPRINTDOC}

//Highlight
const AEM_HLCREATETHEMEA = (WM_USER + 2501);
{$EXTERNALSYM AEM_HLCREATETHEMEA}
const AEM_HLCREATETHEMEW = (WM_USER + 2502);
{$EXTERNALSYM AEM_HLCREATETHEMEW}
const AEM_HLGETTHEMEA = (WM_USER + 2503);
{$EXTERNALSYM AEM_HLGETTHEMEA}
const AEM_HLGETTHEMEW = (WM_USER + 2504);
{$EXTERNALSYM AEM_HLGETTHEMEW}
const AEM_HLGETTHEMENAMEA = (WM_USER + 2505);
{$EXTERNALSYM AEM_HLGETTHEMENAMEA}
const AEM_HLGETTHEMENAMEW = (WM_USER + 2506);
{$EXTERNALSYM AEM_HLGETTHEMENAMEW}
const AEM_HLGETTHEMESTACK = (WM_USER + 2507);
{$EXTERNALSYM AEM_HLGETTHEMESTACK}
const AEM_HLTHEMEEXISTS = (WM_USER + 2508);
{$EXTERNALSYM AEM_HLTHEMEEXISTS}
const AEM_HLSETTHEME = (WM_USER + 2509);
{$EXTERNALSYM AEM_HLSETTHEME}
const AEM_HLDELETETHEME = (WM_USER + 2510);
{$EXTERNALSYM AEM_HLDELETETHEME}
const AEM_HLDELETETHEMEALL = (WM_USER + 2511);
{$EXTERNALSYM AEM_HLDELETETHEMEALL}
const AEM_HLADDDELIMITERA = (WM_USER + 2521);
{$EXTERNALSYM AEM_HLADDDELIMITERA}
const AEM_HLADDDELIMITERW = (WM_USER + 2522);
{$EXTERNALSYM AEM_HLADDDELIMITERW}
const AEM_HLDELETEDELIMITER = (WM_USER + 2523);
{$EXTERNALSYM AEM_HLDELETEDELIMITER}
const AEM_HLADDWORDA = (WM_USER + 2531);
{$EXTERNALSYM AEM_HLADDWORDA}
const AEM_HLADDWORDW = (WM_USER + 2532);
{$EXTERNALSYM AEM_HLADDWORDW}
const AEM_HLDELETEWORD = (WM_USER + 2533);
{$EXTERNALSYM AEM_HLDELETEWORD}
const AEM_HLADDQUOTEA = (WM_USER + 2541);
{$EXTERNALSYM AEM_HLADDQUOTEA}
const AEM_HLADDQUOTEW = (WM_USER + 2542);
{$EXTERNALSYM AEM_HLADDQUOTEW}
const AEM_HLDELETEQUOTE = (WM_USER + 2543);
{$EXTERNALSYM AEM_HLDELETEQUOTE}
const AEM_HLADDMARKTEXTA = (WM_USER + 2551);
{$EXTERNALSYM AEM_HLADDMARKTEXTA}
const AEM_HLADDMARKTEXTW = (WM_USER + 2552);
{$EXTERNALSYM AEM_HLADDMARKTEXTW}
const AEM_HLDELETEMARKTEXT = (WM_USER + 2553);
{$EXTERNALSYM AEM_HLDELETEMARKTEXT}
const AEM_HLADDMARKRANGE = (WM_USER + 2561);
{$EXTERNALSYM AEM_HLADDMARKRANGE}
const AEM_HLDELETEMARKRANGE = (WM_USER + 2562);
{$EXTERNALSYM AEM_HLDELETEMARKRANGE}
const AEM_HLGETHIGHLIGHT = (WM_USER + 2571);
{$EXTERNALSYM AEM_HLGETHIGHLIGHT}
const AEM_HLGETOPTIONS = (WM_USER + 2581);
{$EXTERNALSYM AEM_HLGETOPTIONS}
const AEM_HLSETOPTIONS = (WM_USER + 2582);
{$EXTERNALSYM AEM_HLSETOPTIONS}


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


//// AkelEdit messages description

AEN_ERRSPACE
____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control cannot allocate enough memory.

(int)wParam           == specifies the control identifier.
(AENERRSPACE * )lParam == pointer to a AENERRSPACE structure.

Return Value
 Zero.


AEN_SETFOCUS
____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control receives the keyboard focus.

(int)wParam     == specifies the control identifier.
(NMHDR * )lParam == pointer to a NMHDR structure.

Return Value
 Zero.


AEN_KILLFOCUS
_____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control loses the keyboard focus.

(int)wParam     == specifies the control identifier.
(NMHDR * )lParam == pointer to a NMHDR structure.

Return Value
 Zero.


AEN_HSCROLL
___________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before an edit control window scrolled vertically.

(int)wParam         == specifies the control identifier.
(AENSCROLL * )lParam == pointer to a AENSCROLL structure.

Return Value
 If zero, the control allows scroll operation.
 If a nonzero value, the control suppress scroll operation.

Remarks
 To receive AEN_HSCROLL notifications, specify AENM_SCROLL in the mask sent with the AEM_SETEVENTMASK message.


AEN_VSCROLL
___________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before an edit control window scrolled horizontally.

(int)wParam         == specifies the control identifier.
(AENSCROLL * )lParam == pointer to a AENSCROLL structure.

Return Value
 If zero, the control allows scroll operation.
 If a nonzero value, the control suppress scroll operation.

Remarks
 To receive AEN_VSCROLL notifications, specify AENM_SCROLL in the mask sent with the AEM_SETEVENTMASK message.


AEN_SETRECT
___________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after an edit control formatting rectangle changed.

(int)wParam          == specifies the control identifier.
(AENSETRECT * )lParam == pointer to a AENSETRECT structure.

Return Value
 Zero.

Remarks
 AEN_SETRECT notification is not sent during processing WM_SIZE message.


AEN_PAINT
_________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before line is painted.

(int)wParam        == specifies the control identifier.
(AENPAINT * )lParam == pointer to a AENPAINT structure.

Return Value
 Zero.

Remarks
 To receive AEN_PAINT notifications, specify AENM_PAINT in the mask sent with the AEM_SETEVENTMASK message.


AEN_MAXTEXT
___________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control rich text limit.

(int)wParam     == specifies the control identifier.
(NMHDR * )lParam == pointer to a NMHDR structure.

Return Value
 Zero.


AEN_PROGRESS
____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when required number of lines was proceed.

(int)wParam           == specifies the control identifier.
(AENPROGRESS * )lParam == pointer to a AENPROGRESS structure.

Return Value
 If zero, the control continues operation.
 If a nonzero value, the control stops operation.

Remarks
 During the operation which calls AEN_PROGRESS, the edit control contents are unstable.
 To receive AEN_PROGRESS notifications, specify AENM_PROGRESS in the mask sent with the AEM_SETEVENTMASK message.


AEN_MODIFY
__________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after the document modify state has changed.

(int)wParam         == specifies the control identifier.
(AENMODIFY * )lParam == pointer to a AENMODIFY structure.

Return Value
 Zero.

Remarks
 To receive AEN_MODIFY notifications, specify AENM_MODIFY in the mask sent with the AEM_SETEVENTMASK message.


AEN_SELCHANGING
_______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before the current selection has changed.

(int)wParam            == specifies the control identifier.
(AENSELCHANGE * )lParam == pointer to a AENSELCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_SELCHANGING notifications, specify AENM_SELCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_SELCHANGED
______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after the current selection has changed.

(int)wParam            == specifies the control identifier.
(AENSELCHANGE * )lParam == pointer to a AENSELCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_SELCHANGED notifications, specify AENM_SELCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTCHANGING
________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before the document text has changed.

(int)wParam             == specifies the control identifier.
(AENTEXTCHANGE * )lParam == pointer to a AENTEXTCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTCHANGING notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTINSERTBEGIN
___________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before text is inserted in the document.

(int)wParam             == specifies the control identifier.
(AENTEXTINSERT * )lParam == pointer to a AENTEXTINSERT structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTINSERTBEGIN notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTINSERTEND
_________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after text is inserted in the document.

(int)wParam             == specifies the control identifier.
(AENTEXTINSERT * )lParam == pointer to a AENTEXTINSERT structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTINSERTEND notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTDELETEBEGIN
___________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before text is deleted from the document.

(int)wParam             == specifies the control identifier.
(AENTEXTDELETE * )lParam == pointer to a AENTEXTDELETE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTDELETEBEGIN notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTDELETEEND
_________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after text is deleted from the document.

(int)wParam             == specifies the control identifier.
(AENTEXTDELETE * )lParam == pointer to a AENTEXTDELETE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTDELETEEND notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTCHANGED
_______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after the document text has changed.

(int)wParam             == specifies the control identifier.
(AENTEXTCHANGE * )lParam == pointer to a AENTEXTCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTCHANGED notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_POINT
_________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after text point has changed.

(int)wParam        == specifies the control identifier.
(AENPOINT * )lParam == pointer to a AENPOINT structure.

Return Value
 Zero.

Remarks
 To receive AEN_POINT notifications, specify AENM_POINT in the mask sent with the AEM_SETEVENTMASK message.


AEN_DROPFILES
_____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when the user is attempting to drop files into the control (when it receives the WM_DROPFILES message).

(int)wParam            == specifies the control identifier.
(AENDROPFILES * )lParam == pointer to a AENDROPFILES structure.

Return Value
 If zero, the control ignores the drop operation.
 If a nonzero value, the control allows the drop operation.

Remarks
 To receive AEN_DROPFILES notifications, specify AENM_DROPFILES in the mask sent with the AEM_SETEVENTMASK message.
 To receive WM_DROPFILES messages, the parent window must register the control as a drop target by using the DragAcceptFiles function. The control does not register itself.


AEN_DROPSOURCE
______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when the user is dragging the selection.

(int)wParam             == specifies the control identifier.
(AENDROPSOURCE * )lParam == pointer to a AENDROPSOURCE structure.

Return Value
 If zero, the control ignores the drop operation.
 If a nonzero value, the control allows the drop operation.

Remarks
 To receive AEN_DROPSOURCE notifications, specify AEN_DRAGDROP in the mask sent with the AEM_SETEVENTMASK message.


AEN_DROPTARGET
______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when the user is attempting to drop data into the control.

(int)wParam             == specifies the control identifier.
(AENDROPTARGET * )lParam == pointer to a AENDROPTARGET structure.

Return Value
 If zero, the control ignores the drop operation.
 If a nonzero value, the control allows the drop operation.

Remarks
 To receive AEN_DROPTARGET notifications, specify AEN_DRAGDROP in the mask sent with the AEM_SETEVENTMASK message.


AEN_LINK
________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when edit control receives mouse messages while the mouse pointer is over URL.

(int)wParam       == specifies the control identifier.
(AENLINK * )lParam == pointer to a AENLINK structure.

Return Value
 If zero, the control proceeds with its normal handling of the mouse message.
 If a nonzero value, the control does not handle the mouse message.

Remarks
 To receive AEN_LINK notifications, specify AENM_LINK in the mask sent with the AEM_SETEVENTMASK message and turn on URL detection with the AEM_SETDETECTURL message.


AEN_MARKER
__________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after column marker position or type is changed.

(int)wParam         == specifies the control identifier.
(AENMARKER * )lParam == pointer to a AENMARKER structure.

Return Value
 Zero.

Remarks
 To receive AEN_MARKER notifications, specify AENM_MARKER in the mask sent with the AEM_SETEVENTMASK message.


AEM_EXSETTEXTA
______________

Set ansi text of the edit control.

wParam               == not used.
(AESETTEXTA * )lParam == pointer to a AESETTEXTA structure.

Return Value
 Text length.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 AESETTEXTA st;

 st.pText="SomeText";
 st.dwTextLen=(UINT_PTR)-1;
 st.nNewLine=AELB_ASINPUT;
 st.nCodePage=CP_ACP;
 SendMessage(hWndEdit, AEM_EXSETTEXTA, 0, (LPARAM)&st);


AEM_EXSETTEXTW
______________

Set unicode text of the edit control.

wParam               == not used.
(AESETTEXTW * )lParam == pointer to a AESETTEXTW structure.

Return Value
 Text length.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 AESETTEXTW st;

 st.pText=L"SomeText";
 st.dwTextLen=(UINT_PTR)-1;
 st.nNewLine=AELB_ASINPUT;
 SendMessage(hWndEdit, AEM_EXSETTEXTW, 0, (LPARAM)&st);


AEM_SETTEXTA
____________

Set ansi text of the edit control.

(UINT_PTR)wParam == text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
(char * )lParam   == text pointer.

Return Value
 Text length.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 SendMessage(hWndEdit, AEM_SETTEXTA, (WPARAM)-1, (LPARAM)"SomeText");


AEM_SETTEXTW
____________

Set unicode text of the edit control.

(UINT_PTR)wParam  == text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
(wchar_t * )lParam == text pointer.

Return Value
 Text length.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 SendMessage(hWndEdit, AEM_SETTEXTW, (WPARAM)-1, (LPARAM)L"SomeText");


AEM_APPENDTEXTA
_______________

Append ansi text to the end of the edit control.

wParam                  == not used.
(AEAPPENDTEXTA * )lParam == pointer to a AEAPPENDTEXTA structure.

Return Value
 Zero.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 AEAPPENDTEXTA at;

 at.pText="SomeText";
 at.dwTextLen=(UINT_PTR)-1;
 at.nNewLine=AELB_ASINPUT;
 at.nCodePage=CP_ACP;
 SendMessage(hWndEdit, AEM_APPENDTEXTA, 0, (LPARAM)&at);


AEM_APPENDTEXTW
_______________

Append unicode text to the end of the edit control.

wParam                  == not used.
(AEAPPENDTEXTW * )lParam == pointer to a AEAPPENDTEXTW structure.

Return Value
 Zero.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 AEAPPENDTEXTW at;

 at.pText=L"SomeText";
 at.dwTextLen=(UINT_PTR)-1;
 at.nNewLine=AELB_ASINPUT;
 SendMessage(hWndEdit, AEM_APPENDTEXTW, 0, (LPARAM)&at);


AEM_REPLACESELA
_______________

Replace selection with the ansi text.

wParam                  == not used.
(AEREPLACESELA * )lParam == pointer to a AEREPLACESELA structure.

Return Value
 Zero.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 AEREPLACESELA rs;

 rs.pText="SomeText";
 rs.dwTextLen=(UINT_PTR)-1;
 rs.nNewLine=AELB_ASINPUT;
 rs.dwFlags=AEREPT_COLUMNASIS;
 rs.ciInsertStart=NULL;
 rs.ciInsertEnd=NULL;
 rs.nCodePage=CP_ACP;
 SendMessage(hWndEdit, AEM_REPLACESELA, 0, (LPARAM)&rs);


AEM_REPLACESELW
_______________

Replace selection with the unicode text.

wParam                  == not used.
(AEREPLACESELW * )lParam == pointer to a AEREPLACESELW structure.

Return Value
 Zero.

Remarks
 Message ignore AECO_READONLY flag.

Example:
 AEREPLACESELW rs;

 rs.pText=L"SomeText";
 rs.dwTextLen=(UINT_PTR)-1;
 rs.nNewLine=AELB_ASINPUT;
 rs.dwFlags=AEREPT_COLUMNASIS;
 rs.ciInsertStart=NULL;
 rs.ciInsertEnd=NULL;
 SendMessage(hWndEdit, AEM_REPLACESELW, 0, (LPARAM)&rs);


AEM_GETTEXTRANGEA
_________________

Retrieve a specified range of ansi characters from an edit control.

wParam                 == not used.
(AETEXTRANGEA * )lParam == pointer to a AETEXTRANGEA structure.

Return Value
 Number of characters copied, not including the terminating NULL character.

Example:
 AETEXTRANGEA tr;

 SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr);
 tr.pBuffer=NULL;
 tr.nNewLine=AELB_ASOUTPUT;
 tr.nCodePage=CP_ACP;
 tr.lpDefaultChar=NULL;
 tr.lpUsedDefChar=NULL;
 tr.bFillSpaces=FALSE;

 if (tr.dwBufferMax=SendMessage(hWndEdit, AEM_GETTEXTRANGEA, 0, (LPARAM)&tr))
 {
   if (tr.pBuffer=(char * )GlobalAlloc(GPTR, tr.dwBufferMax))
   {
     SendMessage(hWndEdit, AEM_GETTEXTRANGEA, 0, (LPARAM)&tr);
     MessageBoxA(NULL, tr.pBuffer, NULL, 0);

     GlobalFree((HGLOBAL)tr.pBuffer);
   }
 }


AEM_GETTEXTRANGEW
_________________

Retrieve a specified range of unicode characters from an edit control.

wParam                 == not used.
(AETEXTRANGEW * )lParam == pointer to a AETEXTRANGEW structure.

Return Value
 Number of characters copied, not including the terminating NULL character.

Example:
 AETEXTRANGEW tr;

 SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr);
 tr.pBuffer=NULL;
 tr.nNewLine=AELB_ASOUTPUT;
 tr.bFillSpaces=FALSE;

 if (tr.dwBufferMax=SendMessage(hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr))
 {
   if (tr.pBuffer=(wchar_t * )GlobalAlloc(GPTR, tr.dwBufferMax * sizeof(wchar_t)))
   {
     SendMessage(hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);
     MessageBoxW(NULL, tr.pBuffer, NULL, 0);

     GlobalFree((HGLOBAL)tr.pBuffer);
   }
 }


AEM_STREAMIN
____________

Replace the contents of an edit control with a stream of data provided by an application defined callback function.

(DWORD)wParam        == see AESF_* defines.
(AESTREAMIN * )lParam == pointer to a AESTREAMIN structure.

Return Value
 Number of bytes read.

Remarks
 Message ignore AECO_READONLY and !AECO_MULTILINE flag.

Example:
 typedef struct {
   wchar_t *wpData;
   UINT_PTR dwDataLen;
   UINT_PTR dwCount;
 } STREAMINDATA;

 AESTREAMIN aesi;
 STREAMINDATA sid;

 sid.wpData=L"SomeText";
 sid.dwDataLen=lstrlenW(sid.wpData);
 sid.dwCount=0;

 aesi.dwCookie=(UINT_PTR)&sid;
 aesi.lpCallback=InputStreamCallback;
 aesi.nNewLine=AELB_ASINPUT;
 aesi.dwTextLen=sid.dwDataLen;
 SendMessage(hWndEdit, AEM_STREAMIN, AESF_SELECTION, (LPARAM)&aesi);

 DWORD CALLBACK InputStreamCallback(UINT_PTR dwCookie, wchar_t *wszBuf, DWORD dwBufBytesSize, DWORD *dwBufBytesDone)
 {
   STREAMINDATA *lpData=(STREAMINDATA * )dwCookie;
   wchar_t *wpSrc=lpData->wpData;
   wchar_t *wpDest=wszBuf;
   DWORD dwDestLen=dwBufBytesSize / sizeof(wchar_t);
   DWORD i;

   for (i=0; i < dwDestLen; ++i)
   {
     if (lpData->dwCount >= lpData->dwDataLen)
       break;
     wpDest[i]=wpSrc[lpData->dwCount++];
   }
   *dwBufBytesDone=i * sizeof(wchar_t);

   return 0;
 }


AEM_STREAMOUT
_____________

Cause an edit control to pass its contents to an application defined callback function.
The callback function can then write the stream of data to a file or any other location that it chooses.

(DWORD)wParam         == see AESF_* defines.
(AESTREAMOUT * )lParam == pointer to a AESTREAMOUT structure.

Return Value
 Number of bytes written to the data stream.

Example:
 typedef struct {
   HANDLE hFile;
 } STREAMOUTDATA;

 AESTREAMOUT aeso;
 STREAMOUTDATA sod;

 sod.hFile=CreateFileA("C:\\Test.tmp", GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

 if (sod.hFile != INVALID_HANDLE_VALUE)
 {
   aeso.dwCookie=(UINT_PTR)&sod;
   aeso.lpCallback=OutputStreamCallback;
   aeso.nNewLine=AELB_ASOUTPUT;
   aeso.bColumnSel=FALSE;
   SendMessage(hWndEdit, AEM_STREAMOUT, 0, (LPARAM)&aeso);

   CloseHandle(sod.hFile);
 }

 DWORD CALLBACK OutputStreamCallback(UINT_PTR dwCookie, wchar_t *wszBuf, DWORD dwBufBytesSize, DWORD *dwBufBytesDone)
 {
   STREAMOUTDATA *lpData=(STREAMOUTDATA * )dwCookie;
   unsigned char *pDataToWrite=(unsigned char * )wszBuf;
   DWORD dwBytesToWrite=dwBufBytesSize;

   return !WriteFile(lpData->hFile, pDataToWrite, dwBytesToWrite, dwBufBytesDone, NULL);
 }


AEM_CANPASTE
____________

Determine whether an edit control can paste a text from clipboard.

wParam == not used.
lParam == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_CANPASTE, 0, 0);


AEM_PASTE
_________

Paste text from clipboard.

wParam        == not used.
(DWORD)lParam == see AEPFC_* defines.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_PASTE, 0, 0);


AEM_CUT
_______

Delete the current selection, if any, and copy the deleted text to the clipboard.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_CUT, 0, 0);


AEM_COPY
________

Copy the current selection to the clipboard.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_COPY, 0, 0);


AEM_CHECKCODEPAGE
_________________

Check that the contents of an edit control can be converted to the specified code page.

(int)wParam   == code page to check.
(int * )lParam == returns nonconverted character in line (first character == 1). Can be NULL.

Return Value
 Zero if successful, otherwise returns first met line number containing nonconverted character (first line == 1).

Remarks
 Windows 95 isn't supported.

Example:
 SendMessage(hWndEdit, AEM_CHECKCODEPAGE, 1251, (LPARAM)NULL);


AEM_FINDTEXTA
_____________

Find ansi text within an edit control.

wParam                == not used.
(AEFINDTEXTA * )lParam == pointer to a AEFINDTEXTA structure.

Return Value
 TRUE   found.
 FALSE  not found.

Example:
 AEFINDTEXTA ft;
 AESELECTION aes;

 ft.dwFlags=AEFR_DOWN;
 ft.pText="SomeText";
 ft.dwTextLen=(UINT_PTR)-1;
 ft.nNewLine=AELB_ASIS;
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ft.crSearch.ciMin);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ft.crSearch.ciMax);

 if (SendMessage(hWndEdit, AEM_FINDTEXTA, 0, (LPARAM)&ft))
 {
   aes.crSel=ft.crFound;
   aes.dwFlags=0;
   SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
 }


AEM_FINDTEXTW
_____________

Find unicode text within an edit control.

wParam                == not used.
(AEFINDTEXTW * )lParam == pointer to a AEFINDTEXTW structure.

Return Value
 TRUE   found.
 FALSE  not found.

Example:
 AEFINDTEXTW ft;
 AESELECTION aes;

 ft.dwFlags=AEFR_DOWN;
 ft.pText=L"SomeText";
 ft.dwTextLen=(UINT_PTR)-1;
 ft.nNewLine=AELB_ASIS;
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ft.crSearch.ciMin);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ft.crSearch.ciMax);

 if (SendMessage(hWndEdit, AEM_FINDTEXTW, 0, (LPARAM)&ft))
 {
   aes.crSel=ft.crFound;
   aes.dwFlags=0;
   SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
 }


AEM_ISMATCHA
____________

Is ansi text matched with text at specified position.

(AECHARINDEX * )wParam == position to check from.
(AEFINDTEXTA * )lParam == pointer to a AEFINDTEXTA structure. AEFINDTEXTA.crSearch member is ignored.

Return Value
 Length of the matched text or zero if not found.

Example:
 AEFINDTEXTA ft;
 AECHARINDEX ciChar;
 AESELECTION aes;

 ft.dwFlags=AEFR_MATCHCASE;
 ft.pText="SomeText";
 ft.dwTextLen=(UINT_PTR)-1;
 ft.nNewLine=AELB_ASIS;
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTSELCHAR, (LPARAM)&ciChar);

 if (SendMessage(hWndEdit, AEM_ISMATCHA, (WPARAM)&ciChar, (LPARAM)&ft))
 {
   aes.crSel=ft.crFound;
   aes.dwFlags=0;
   SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
 }


AEM_ISMATCHW
____________

Is unicode text matched with text at specified position.

(AECHARINDEX * )wParam == position to check from.
(AEFINDTEXTW * )lParam == pointer to a AEFINDTEXTW structure. AEFINDTEXTW.crSearch member is ignored.

Return Value
 Length of the matched text or zero if not found.

Example:
 AEFINDTEXTW ft;
 AECHARINDEX ciChar;
 AESELECTION aes;

 ft.dwFlags=AEFR_MATCHCASE;
 ft.pText=L"SomeText";
 ft.dwTextLen=(UINT_PTR)-1;
 ft.nNewLine=AELB_ASIS;
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTSELCHAR, (LPARAM)&ciChar);

 if (SendMessage(hWndEdit, AEM_ISMATCHW, (WPARAM)&ciChar, (LPARAM)&ft))
 {
   aes.crSel=ft.crFound;
   aes.dwFlags=0;
   SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
 }


AEM_KEYDOWN
___________

Emulate special key down pressing. For example: VK_HOME, VK_DOWN, VK_INSERT, VK_BACK, etc.

(int)wParam   == virtual-key code.
(DWORD)lParam == see AEMOD_* defines.

Return Value
 TRUE  virtual-key is processed.
 FALSE virtual-key not processed.

Remarks
 To emulate VK_RETURN key use WM_CHAR message.
 To emulate VK_TAB key use it with AEMOD_CONTROL modifier.

Example:
 SendMessage(hWndEdit, AEM_KEYDOWN, VK_RIGHT, AEMOD_SHIFT|AEMOD_CONTROL);


AEM_INSERTCHAR
______________

Insert character taking into account overtype mode and grouping undo.

(wchar_t)wParam == unicode character.
lParam          == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_INSERTCHAR, (WPARAM)L' ', 0);


AEM_CHARAT
__________

Retrieve character at index.

(AECHARINDEX * )wParam == character index.
lParam                == not used.

Return Value
 Unicode character or negative values:
  -AELB_EOF  end-of-file.
  -AELB_R    "\r" new line.
  -AELB_N    "\n" new line.
  -AELB_RN   "\r\n" new line.
  -AELB_RRN  "\r\r\n" new line.

Remarks
 For better performance use AEC_CharAtIndex instead.

Example:
 AECHARINDEX ciCaret;
 int nChar;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 nChar=SendMessage(hWndEdit, AEM_CHARAT, (WPARAM)&ciCaret, 0);


AEM_INPUTLANGUAGE
_________________

Retrieves the active input locale identifier.

wParam == not used.
lParam == not used.

Return Value
 Input locale identifier.

Example:
 HKL dwInputLocale=(HKL)SendMessage(hWndEdit, AEM_INPUTLANGUAGE, 0, 0);


AEM_DRAGDROP
____________

Operations with current drag'n'drop.

(DWORD)wParam == see AEDD_* defines.
lParam        == not used.

Return Value
 Value depended on the AEDD_* define.

Example:
 HWND hWndDragSource;

 hWndDragSource=(HWND)SendMessage(hWndEdit, AEM_DRAGDROP, AEDD_GETDRAGWINDOW, 0);


AEM_CANUNDO
___________

Determine whether there are any actions in an edit control's undo queue.

wParam == not used.
lParam == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_CANUNDO, 0, 0);


AEM_CANREDO
___________

Determine whether there are any actions in an edit control's redo queue.

wParam == not used.
lParam == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_CANREDO, 0, 0);


AEM_UNDO
________

Undo the last edit control operation in the control's undo queue.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_UNDO, 0, 0);


AEM_REDO
________

Redo the next edit control operation in the control's redo queue.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_REDO, 0, 0);


AEM_EMPTYUNDOBUFFER
___________________

Erase undo and redo history.

(BOOL)wParam == TRUE   erase only redo history.
                FALSE  erase undo and redo history.
lParam       == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_EMPTYUNDOBUFFER, FALSE, 0);


AEM_STOPGROUPTYPING
___________________

Stops the control from collecting additional typing actions into the current undo action.
The control stores the next typing action, if any, into a new action in the undo queue.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_STOPGROUPTYPING, 0, 0);


AEM_BEGINUNDOACTION
___________________

Beginning of a set of operations that will be undone all as one operation.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_STOPGROUPTYPING, 0, 0);
 SendMessage(hWndEdit, AEM_BEGINUNDOACTION, 0, 0);
 SendMessage(hWndEdit, EM_REPLACESEL, TRUE, (LPARAM)"123");
 SendMessage(hWndEdit, EM_REPLACESEL, TRUE, (LPARAM)"456");
 SendMessage(hWndEdit, AEM_ENDUNDOACTION, 0, 0);
 SendMessage(hWndEdit, AEM_STOPGROUPTYPING, 0, 0);


AEM_ENDUNDOACTION
_________________

End of a set of operations that will be undone all as one operation.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 See AEM_BEGINUNDOACTION example.


AEM_LOCKCOLLECTUNDO
___________________

Stop collect undo/redo history.

(BOOL)wParam == TRUE   stop collect.
                FALSE  start collect.
lParam       == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_EMPTYUNDOBUFFER, FALSE, 0);
 SendMessage(hWndEdit, AEM_LOCKCOLLECTUNDO, TRUE, 0);
 SendMessage(hWndEdit, EM_REPLACESEL, TRUE, (LPARAM)"123");
 SendMessage(hWndEdit, EM_REPLACESEL, TRUE, (LPARAM)"456");
 SendMessage(hWndEdit, AEM_LOCKCOLLECTUNDO, FALSE, 0);


AEM_GETUNDOLIMIT
________________

Get the maximum number of actions that can stored in the undo queue.

wParam          == not used.
(DWORD * )lParam == pointer to a variable that receives current number of actions stored in the undo queue. Can be NULL.

Return Value
 Maximum number of actions that can be stored in the undo queue.

Example:
 SendMessage(hWndEdit, AEM_GETUNDOLIMIT, 0, (LPARAM)NULL);


AEM_SETUNDOLIMIT
________________

Set the maximum number of actions that can stored in the undo queue.

(DWORD)wParam == maximum number of actions that can be stored in the undo queue. By default, the maximum number of actions in the undo queue is unlimited.
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETUNDOLIMIT, 100, 0);


AEM_GETMODIFY
_____________

Retrieve the state of an edit control's modification flag.

wParam == not used.
lParam == not used.

Return Value
 TRUE   text has been modified.
 FALSE  text has not been modified.

Example:
 SendMessage(hWndEdit, AEM_GETMODIFY, 0, 0);


AEM_SETMODIFY
_____________

Set the state of an edit control's modification flag.

(BOOL)wParam == TRUE   text has been modified.
                FALSE  text has not been modified.
lParam       == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETMODIFY, FALSE, 0);


AEM_UNDOBUFFERSIZE
__________________

Get the number of bytes stored in the undo queue.

wParam == not used.
lParam == not used.

Return Value
 Number of bytes.

Example:
 SendMessage(hWndEdit, AEM_UNDOBUFFERSIZE, 0, 0);


AEM_ISRANGEMODIFIED
___________________

Retrieve modification flag of specified range of characters.

wParam                == not used.
(CHARRANGE64 * )lParam == range of characters to check (RichEdit offset).

Return Value
 See AEIRM_* defines.

Example (check is line has been modified):
 AECHARINDEX ciCaretLine;
 CHARRANGE64 crLineRange;
 int nLineModified;

 //Get first char of caret line
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaretLine);
 ciCaretLine.nCharInLine=0;

 //Get caret line coordinates in RichEdit offsets
 crLineRange.cpMin=SendMessage(hWndEdit, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&ciCaretLine);
 crLineRange.cpMax=crLineRange.cpMin + ciCaretLine.lpLine->nLineLen;
 if (ciCaretLine.lpLine->nLineBreak != AELB_WRAP)
   ++crLineRange.cpMax;

 nLineModified=(int)SendMessage(hWndEdit, AEM_ISRANGEMODIFIED, 0, (LPARAM)&crLineRange);


AEM_EXGETSEL
____________

Retrieve the current selection information of an edit control. Simple form of AEM_GETSEL.

(AECHARINDEX * )wParam == selection start index. Can be NULL.
(AECHARINDEX * )lParam == selection end index. Can be NULL.

Return Value
 TRUE   selection is not empty.
 FALSE  selection is empty.

Example:
 AECHARRANGE aecr;

 SendMessage(hWndEdit, AEM_EXGETSEL, (WPARAM)&aecr.ciMin, (LPARAM)&aecr.ciMax);


AEM_EXSETSEL
____________

Set the current selection of an edit control. Simple form of AEM_SETSEL.

(AECHARINDEX * )wParam == selection start index.
(AECHARINDEX * )lParam == selection end index.

Return Value
 Zero.

Example:
 AECHARRANGE aecr;
 BOOL bCaretAtEnd=TRUE;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&aecr.ciMin);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&aecr.ciMax);

 if (bCaretAtEnd)
   SendMessage(hWndEdit, AEM_EXSETSEL, (WPARAM)&aecr.ciMin, (LPARAM)&aecr.ciMax);
 else
   SendMessage(hWndEdit, AEM_EXSETSEL, (WPARAM)&aecr.ciMax, (LPARAM)&aecr.ciMin);


AEM_GETSEL
__________

Retrieve the current selection information of an edit control.

(AECHARINDEX * )wParam == caret index, NULL if not needed.
(AESELECTION * )lParam == pointer to a AESELECTION structure, NULL if not needed.

Return Value
 TRUE   selection is not empty.
 FALSE  selection is empty.

Example:
 AESELECTION aes;
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)&ciCaret, (LPARAM)&aes);


AEM_SETSEL
__________

Set the current selection of an edit control.

(AECHARINDEX * )wParam == caret index, can be NULL.
(AESELECTION * )lParam == pointer to a AESELECTION structure.

Return Value
 Zero.

Example:
 AESELECTION aes;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&aes.crSel.ciMin);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&aes.crSel.ciMax);
 aes.dwFlags=0;
 SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)&aes.crSel.ciMax, (LPARAM)&aes);


AEM_GETCOLUMNSEL
________________

Retrieve the column mode of the current selection.

wParam == not used.
lParam == not used.

Return Value
 TRUE   selection is in column mode.
 FALSE  selection is in non-column mode.

Example:
 SendMessage(hWndEdit, AEM_GETCOLUMNSEL, 0, 0);


AEM_UPDATESEL
_____________

Update current selection.

(DWORD)wParam == see AESELT_* defines.
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_UPDATESEL, AESELT_LOCKSCROLL, 0);


AEM_GETLINENUMBER
_________________

Retrieve the specified line number.

(int)wParam == see AEGL_* defines.
lParam      == not used.

Return Value
 Zero based line number.

Example:
 SendMessage(hWndEdit, AEM_GETLINENUMBER, AEGL_LINECOUNT, 0);


AEM_GETINDEX
____________

Retrieve the specified character index.

(int)wParam           == see AEGI_* defines.
(AECHARINDEX * )lParam == character index.

Return Value
 Zero if failed, non-zero if success.

Example:
 AECHARINDEX ciChar;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ciChar);


AEM_GETLINEINDEX
________________

Retrieve the first character index in the specified line.

(int)wParam           == zero based line number.
(AECHARINDEX * )lParam == character index.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AECHARINDEX ciChar;

 SendMessage(hWndEdit, AEM_GETLINEINDEX, 10, (LPARAM)&ciChar);


AEM_INDEXUPDATE
_______________

Update lpLine member of the AECHARINDEX structure, to avoid dangling of a pointer after text change.

wParam                == not used.
(AECHARINDEX * )lParam == character index.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AESELECTION aes;

 SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&aes);
 SendMessage(hWndEdit, EM_REPLACESEL, TRUE, (LPARAM)"123");
 SendMessage(hWndEdit, AEM_INDEXUPDATE, 0, (LPARAM)&aes.crSel.ciMin);
 SendMessage(hWndEdit, AEM_INDEXUPDATE, 0, (LPARAM)&aes.crSel.ciMax);


AEM_INDEXCOMPARE
________________

Compare two character indexes.

(AECHARINDEX * )wParam == first character index.
(AECHARINDEX * )lParam == second character index.

Return Value
 -1  first index is less than second index
  0  first index is equal to second index
  1  first index is greater than second index

Example:
 AESELECTION aes;

 SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&aes);
 SendMessage(hWndEdit, AEM_INDEXCOMPARE, (WPARAM)&aes.crSel.ciMin, (LPARAM)&aes.crSel.ciMax);

For better performance instead of AEM_INDEXCOMPARE message add:
 #define AEC_FUNCTIONS
 #include "AkelEdit.h"
And use AEC_IndexCompare call:
 AEC_IndexCompare(&aes.crSel.ciMin, &aes.crSel.ciMax);


AEM_INDEXSUBTRACT
_________________

Retrieve how many characters between two character indexes.

wParam                    == not used.
(AEINDEXSUBTRACT * )lParam == pointer to a AEINDEXSUBTRACT structure.

Return Value
 Number of characters. Negative if AEINDEXSUBTRACT.ciChar1 < AEINDEXSUBTRACT.ciChar2.

Example:
 AEINDEXSUBTRACT is;
 AECHARINDEX ciChar1;
 AECHARINDEX ciChar2;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ciChar1);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ciChar2);
 is.ciChar1=&ciChar1;
 is.ciChar2=&ciChar2;
 is.bColumnSel=FALSE;
 is.nNewLine=AELB_ASIS;
 SendMessage(hWndEdit, AEM_INDEXSUBTRACT, 0, (LPARAM)&is);


AEM_INDEXOFFSET
_______________

Retrieve the target character index at the specified offset from source character index.

wParam                  == not used.
(AEINDEXOFFSET * )lParam == pointer to a AEINDEXOFFSET structure.

Return Value
 Processed number of characters.

Example:
 AEINDEXOFFSET io;
 AECHARINDEX ciCharIn;
 AECHARINDEX ciCharOut;

 //Get 10 character index of the edit control
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ciCharIn);
 io.ciCharIn=&ciCharIn;
 io.ciCharOut=&ciCharOut;
 io.nOffset=10;
 io.nNewLine=AELB_R;
 SendMessage(hWndEdit, AEM_INDEXOFFSET, 0, (LPARAM)&io);


AEM_INDEXTORICHOFFSET
_____________________

Convert AkelEdit character index to RichEdit offset.

wParam                == not used.
(AECHARINDEX * )lParam == AkelEdit character index.

Return Value
 RichEdit offset. If error, -1 is returned.

Example:
 AECHARRANGE aecr;
 CHARRANGE recr;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&aecr.ciMin);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTVISIBLELINE, (LPARAM)&aecr.ciMax);
 recr.cpMin=SendMessage(hWndEdit, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&aecr.ciMin);
 recr.cpMax=SendMessage(hWndEdit, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&aecr.ciMax);
 SendMessage(hWndEdit, EM_EXSETSEL, 0, (LPARAM)&recr);


AEM_RICHOFFSETTOINDEX
_____________________

Convert RichEdit offset to AkelEdit character index.

(UINT_PTR)wParam      == RichEdit offset.
(AECHARINDEX * )lParam == AkelEdit character index.

Return Value
 Zero.

Example:
 AECHARRANGE aecr;
 CHARRANGE recr;

 SendMessage(hWndEdit, EM_EXGETSEL, 0, (LPARAM)&recr);
 SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, (WPARAM)recr.cpMin, (LPARAM)&aecr.ciMin);
 SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, (WPARAM)recr.cpMax, (LPARAM)&aecr.ciMax);


AEM_GETRICHOFFSET
_________________

Retrieve the specified RichEdit offset.

(int)wParam     == see AEGI_* defines.
(INT_PTR)lParam == input character RichEdit offset, if required.

Return Value
 RichEdit offset.

Example:
 INT_PTR nOffset;

 nOffset=SendMessage(hWndEdit, AEM_GETRICHOFFSET, AEGI_LASTCHAR, 0);


AEM_GETWRAPLINE
_______________

Convert unwrapped line number to wrapped line number.

(int)wParam           == zero based unwrapped line.
(AECHARINDEX * )lParam == returned first character in wrapped line. Can be NULL.

Return Value
 Wrapped line.

Example:
 AECHARINDEX ciChar;

 SendMessage(hWndEdit, AEM_GETWRAPLINE, (WPARAM)10, (LPARAM)&ciChar);


AEM_GETUNWRAPLINE
_________________

Convert wrapped line number to unwrapped line number.

(int)wParam == zero based wrapped line.
lParam      == not used.

Return Value
 Unwrapped line.

Example:
 AECHARINDEX ciCaret;
 int nUnwrappedLine;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);

 if (SendMessage(hWndEdit, AEM_GETWORDWRAP, 0, (LPARAM)NULL))
   nUnwrappedLine=SendMessage(hWndEdit, AEM_GETUNWRAPLINE, (WPARAM)ciCaret.nLine, 0);
 else
   nUnwrappedLine=ciCaret.nLine;


AEM_GETNEXTBREAK
________________

Retrieve next word break index.

(DWORD)wParam         == see AEWB_* defines. If zero, use current word break settings, see AEM_SETWORDBREAK message.
(AECHARINDEX * )lParam == character index (input/output).

Return Value
 Number of characters to break.

Example:
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_GETNEXTBREAK, AEWB_RIGHTWORDSTART|AEWB_RIGHTWORDEND, (LPARAM)&ciCaret);


AEM_GETPREVBREAK
________________

Retrieve previous word break index.

(DWORD)wParam         == see AEWB_* defines. If zero, use current word break settings, see AEM_SETWORDBREAK message.
(AECHARINDEX * )lParam == character index (input/output).

Return Value
 Number of characters to break.

Example:
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_GETPREVBREAK, AEWB_LEFTWORDSTART|AEWB_LEFTWORDEND, (LPARAM)&ciCaret);


AEM_ISDELIMITER
_______________

Retrieve character index delimiter or not.

(DWORD)wParam         == see AEDLM_* defines.
(AECHARINDEX * )lParam == character index.

Return Value
 TRUE   character index is a delimiter.
 FALSE  character index isn't a delimiter.

Example:
 AECHARINDEX ciChar;

 SendMessage(hWndEdit, AEM_ISDELIMITER, AEDLM_WORD, (LPARAM)&ciChar);


AEM_INDEXTOCOLUMN
_________________

Retrieve the column in line of the character index taking into account tab stop size.

(DWORD)wParam         == low-order word:
                          tab stop size in characters. Use current value if zero.
                         high-order word:
                          TRUE   scan all wrapped lines.
                          FALSE  scan to first char in line.
(AECHARINDEX * )lParam == AkelEdit character index.

Return Value
 Zero based column in line of the character index.

Example:
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_INDEXTOCOLUMN, MAKELONG(0, TRUE), (LPARAM)&ciCaret);


AEM_COLUMNTOINDEX
_________________

Retrieve the character index of the column in line taking into account tab stop size.

(DWORD)wParam         == low-order word:
                          tab stop size in characters. Use current value if zero.
                         high-order word:
                          TRUE   scan all wrapped lines.
                          FALSE  scan to first char in line.
(AECHARINDEX * )lParam == Input:  AECHARINDEX.lpLine and AECHARINDEX.nLine members specifies line to scan from.
                                 AECHARINDEX.nCharInLine specifies zero based column in line.
                         Output: AECHARINDEX structure is filled with result character index.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 ciCaret.nCharInLine=10;
 SendMessage(hWndEdit, AEM_COLUMNTOINDEX, MAKELONG(0, TRUE), (LPARAM)&ciCaret);


AEM_INDEXINURL
______________

Checks is index in URL.

(AECHARINDEX * )wParam == character index.
(AECHARRANGE * )lParam == pointer to a AECHARRANGE structure, that receives URL range, if character in URL.

Return Value
 Length of the URL if index in URL or zero if not in URL.

Remarks
 Use AEM_POINTONURL to detect URL under point.

Example:
 AECHARINDEX ciCaret;
 AECHARRANGE crUrl;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_INDEXINURL, (WPARAM)&ciCaret, (LPARAM)&crUrl);


AEM_ADDPOINT
____________

Add character index to a points stack. Character index will be updated after every text change.

wParam            == not used.
(AEPOINT * )lParam == pointer to a filled AEPOINT structure. AEPOINT.ciPoint and AEPOINT.nPointOffset members required.

Return Value
 Pointer to a created AEPOINT structure.

Example:
 AEPOINT *lpPoint;
 AEPOINT point;
 AECHARINDEX ciChar;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTSELCHAR, (LPARAM)&point.ciPoint);
 point.nPointOffset=AEPTO_CALC;
 point.nPointLen=0;
 point.dwFlags=0;
 point.dwUserData=0;

 if (lpPoint=(AEPOINT * )SendMessage(hWndEdit, AEM_ADDPOINT, 0, (LPARAM)&point))
 {
   SendMessage(hWndEdit, EM_REPLACESEL, TRUE, (LPARAM)"123");
   ciChar=lpPoint->ciPoint;  //Read new position
   SendMessage(hWndEdit, AEM_DELPOINT, (WPARAM)lpPoint, 0);
 }


AEM_DELPOINT
____________

Delete character index from points stack.

(AEPOINT * )wParam == pointer to a AEPOINT structure, returned by AEM_ADDPOINT.
lParam            == not used.

Return Value
 Zero.

Example:
 See AEM_ADDPOINT example.


AEM_GETPOINTSTACK
_________________

Retrieve point stack handle.

wParam == not used.
lParam == not used.

Return Value
 Pointer to a HSTACK structure.

Example:
void RemoveChangedPoints(HWND hWnd)
{
  HSTACK *hPointStack;
  AEPOINT *lpPointItem;
  AEPOINT *lpPointNextItem;

  if (hPointStack=(HSTACK * )SendMessage(hWndEdit, AEM_GETPOINTSTACK, 0, 0))
  {
    lpPointItem=(AEPOINT * )hPointStack->first;

    while (lpPointItem)
    {
      lpPointNextItem=lpPointItem->next;

      if ((lpPointItem->dwFlags & AEPTF_INSERT) ||
          (lpPointItem->dwFlags & AEPTF_DELETE))
      {
        SendMessage(hWndEdit, AEM_DELPOINT, (WPARAM)lpPointItem, 0);
      }
      lpPointItem=lpPointNextItem;
    }
  }
}


AEM_CHARFROMGLOBALPOS
_____________________

Retrieve the closest wide character to a specified point in the virtual text space of an edit control.

(POINT64 * )wParam     == Input:  coordinates of a point in the control's virtual text space.
                         Output: exact coordinates in the virtual text space of a returned character.
(AECHARINDEX * )lParam == character index.

Return Value
 See AEPC_* defines.

Example:
 AECHARINDEX ciCaret;
 POINT64 ptGlobal;

 SendMessage(hWndEdit, AEM_GETCARETPOS, (LPARAM)NULL, (WPARAM)&ptGlobal);
 SendMessage(hWndEdit, AEM_CHARFROMGLOBALPOS, (WPARAM)&ptGlobal, (LPARAM)&ciCaret);


AEM_GLOBALPOSFROMCHAR
_____________________

Retrieve the coordinates in the virtual text space of a specified character in an edit control.

(POINT64 * )wParam     == coordinates of a point in the control's virtual text space.
(AECHARINDEX * )lParam == character index.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AECHARINDEX ciChar;
 POINT64 ptGlobal;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTVISIBLECHAR, (LPARAM)&ciChar);
 SendMessage(hWndEdit, AEM_GLOBALPOSFROMCHAR, (WPARAM)&ptGlobal, (LPARAM)&ciChar);


AEM_CHARFROMPOS
_______________

Retrieve the closest wide character to a specified point in the client area of an edit control.

(POINT * )wParam       == Input:  coordinates of a point in the control's client area.
                         Output: exact client coordinates of a returned character.
(AECHARINDEX * )lParam == character index.

Return Value
 See AEPC_* defines.

Example:
 AECHARINDEX ciFirstVisible;
 POINT ptClient;

 ptClient.x=10;
 ptClient.y=10;
 SendMessage(hWndEdit, AEM_CHARFROMPOS, (WPARAM)&ptClient, (LPARAM)&ciFirstVisible);


AEM_POSFROMCHAR
_______________

Retrieve the client area coordinates of a specified character in an edit control.

(POINT * )wParam       == coordinates of a point in the control's client area.
(AECHARINDEX * )lParam == character index.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AECHARINDEX ciChar;
 POINT ptClient;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciChar);
 SendMessage(hWndEdit, AEM_POSFROMCHAR, (WPARAM)&ptClient, (LPARAM)&ciChar);


AEM_GETRECT
___________

Retrieve the formatting rectangle of an edit control. The formatting rectangle is the limiting rectangle into which the control draws the text.

(DWORD)wParam  == see AERC_* defines.
(RECT * )lParam == pointer to a RECT structure that receives the formatting rectangle.

Return Value
 Zero.

Example:
 RECT rc;

 SendMessage(hWndEdit, AEM_GETRECT, 0, (LPARAM)&rc);


AEM_SETRECT
___________

Set the formatting rectangle of an edit control. The formatting rectangle is the limiting rectangle into which the control draws the text.

(DWORD)wParam  == see AERC_* defines.
(RECT * )lParam == pointer to a RECT structure that specifies the new dimensions of the rectangle. If this parameter is NULL, the formatting rectangle is set to its default values.

Return Value
 Zero.

Example:
 RECT rc;

 rc.left=10;
 rc.top=10;
 rc.right=200;
 rc.bottom=200;
 SendMessage(hWndEdit, AEM_SETRECT, AERC_UPDATE, (LPARAM)&rc);


AEM_GETSCROLLPOS
________________

Obtain the current and/or maximum scroll position of the edit control.

(POINT64 * )wParam == pointer to a POINT64 structure that receives the maximum scroll position in the virtual text space of the document, expressed in pixels. Can be NULL.
(POINT64 * )lParam == pointer to a POINT64 structure that receives the upper-left corner position in the virtual text space of the document, expressed in pixels. Can be NULL.

Return Value
 Zero.

Example:
 POINT64 ptGlobalMax;
 POINT64 ptGlobalPos;

 SendMessage(hWndEdit, AEM_GETSCROLLPOS, (WPARAM)&ptGlobalMax, (LPARAM)&ptGlobalPos);


AEM_SETSCROLLPOS
________________

Scroll an edit control to a particular point.

wParam            == not used.
(POINT64 * )lParam == pointer to a POINT64 structure which specifies a point in the virtual text space of the document, expressed in pixels.
                     If POINT64.x equal to -1, then horizontal scroll is not changed.
                     If POINT64.y equal to -1, then vertical scroll is not changed.

Return Value
 Zero.

Example:
 POINT64 ptGlobal;

 ptGlobal.x=20;
 ptGlobal.y=10;
 SendMessage(hWndEdit, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobal);


AEM_SCROLL
__________

Scroll the text horizontally or vertically in an edit control. This message is equivalent of WM_HSCROLL and WM_VSCROLL.

(DWORD)wParam == see AESB_* defines.
(int)lParam   == action that can be specified with AESB_HORZ:
                  SB_LINELEFT  scrolls left by one character.
                  SB_LINERIGHT scrolls right by one character.
                  SB_PAGELEFT  scrolls left by the width of the window.
                  SB_PAGERIGHT scrolls right by the width of the window.
                  SB_LEFT      scrolls to the most left.
                  SB_RIGHT     scrolls to the most right.
                 action that can be specified with AESB_VERT:
                  SB_LINEUP    scrolls one line up.
                  SB_LINEDOWN  scrolls one line down.
                  SB_PAGEUP    scrolls one page up.
                  SB_PAGEDOWN  scrolls one page down.
                  SB_TOP       scrolls to the most up.
                  SB_BOTTOM    scrolls to the most down.

Return Value
 Pixels scrolled.

Example:
 SendMessage(hWndEdit, AEM_SCROLL, AESB_VERT, SB_LINEDOWN);


AEM_LINESCROLL
______________

Scroll the text horizontally or vertically on the specified number of characters or lines.

(DWORD)wParam == see AESB_* defines.
(int)lParam   == If AESB_HORZ specified, the number of characters to scroll horizontally.
                 If AESB_VERT specified, the number of lines to scroll vertically.

Return Value
 Pixels scrolled.

Example:
 SendMessage(hWndEdit, AEM_LINESCROLL, AESB_VERT, -10);


AEM_SCROLLTOPOINT
_________________

Scroll the specified point into view in an edit control.

wParam                    == not used.
(AESCROLLTOPOINT * )lParam == pointer to a AESCROLLTOPOINT structure.

Return Value
 See AECSE_* defines.

Example:
 AESCROLLTOPOINT stp={0};

 stp.dwFlags=AESC_POINTCARET|AESC_OFFSETCHARX|AESC_OFFSETCHARY;
 stp.nOffsetX=1;
 stp.nOffsetY=1;
 SendMessage(hWndEdit, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);


AEM_LOCKSCROLL
______________

Lock scrolling of an edit control.

(int)wParam  == SB_BOTH  lock horizontal and vertical scroll.
                SB_HORZ  lock horizontal scroll.
                SB_VERT  lock vertical scroll.
                -1       only retrieve current SB_* lock, lParam is ignored.
(BOOL)lParam == TRUE   lock scroll.
                FALSE  unlock scroll.

Return Value
 Previous SB_* lock or -1 if no locking information defined.

Remarks
 Locking is cumulative. If your application locks scroll five times in a row, it must also unlock scroll five times before the scroll unlocks.

Example:
 SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, TRUE);
 SendMessage(hWndEdit, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
 SendMessage(hWndEdit, AEM_LOCKSCROLL, SB_BOTH, FALSE);


AEM_GETCHARSIZE
_______________

Retrieve current font character height/width or average width.

(DWORD)wParam         == see AECS_* defines.
(AECHARINDEX * )lParam == character index.

Return Value
 Integer that depend on AECS_* value.

Example:
 SendMessage(hWndEdit, AEM_GETCHARSIZE, AECS_HEIGHT, (LPARAM)NULL);


AEM_GETSTRWIDTH
_______________

Retrieve string width. Uses the currently selected font.

(wchar_t * )wParam == unicode string.
(int)lParam       == string length.

Return Value
 String width.

Example:
 SendMessage(hWndEdit, AEM_GETSTRWIDTH, (WPARAM)L"ABC", 3);


AEM_GETCARETPOS
_______________

Obtain the current caret position.

(POINT * )wParam   == pointer to a POINT structure that receives the caret position in the client area coordinates, expressed in pixels. Can be NULL.
(POINT64 * )lParam == pointer to a POINT64 structure that receives the caret position in the virtual text space of the document, expressed in pixels. Can be NULL.

Return Value
 TRUE   caret is visible.
 FALSE  caret isn't visible.

Example:
 POINT ptClient;
 POINT64 ptGlobal;

 SendMessage(hWndEdit, AEM_GETCARETPOS, (LPARAM)&ptClient, (WPARAM)&ptGlobal);


AEM_GETCARETHORZINDENT
______________________

Retrieve caret horizontal indent. It's unchanged after VK_UP, VK_DOWN, VK_PRIOR, VK_NEXT key pressed.

wParam == not used.
lParam == not used.

Return Value
 Caret horizontal indent in the virtual text space of the document.

Example:
 SendMessage(hWndEdit, AEM_GETCARETHORZINDENT, 0, 0);


AEM_SETCARETHORZINDENT
______________________

Set caret horizontal indent. It's unchanged after VK_UP, VK_DOWN, VK_PRIOR, VK_NEXT key pressed.

(DWORD)wParam == caret horizontal indent in the virtual text space of the document.
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETCARETHORZINDENT, 10, 0);


AEM_CONVERTPOINT
________________

Convert position coordinates.

(DWORD)wParam     == see AECPT_* defines.
(POINT64 * )lParam == pointer to a POINT64 structure.

Return Value
 Zero.

Example:
 POINT64 pt;

 pt.x=SendMessage(hWndEdit, AEM_GETCARETHORZINDENT, 0, 0);
 pt.y=0;
 SendMessage(hWndEdit, AEM_CONVERTPOINT, AECPT_GLOBALTOCLIENT, (LPARAM)&pt);


AEM_POINTONMARGIN
_________________

Checks is point on margin.

(POINT * )wParam == coordinates of a point in the control's client area.
lParam          == not used.

Return Value
 See AESIDE_* defines.

Example:
 POINT ptClient;

 ptClient.x=10;
 ptClient.y=10;
 SendMessage(hWndEdit, AEM_POINTONMARGIN, (WPARAM)&ptClient, 0);


AEM_POINTONSELECTION
____________________

Checks is point on selection.

(POINT * )wParam == coordinates of a point in the control's client area.
lParam          == not used.

Return Value
 TRUE  on selection.
 FALSE not on selection.

Example:
 POINT ptClient;

 ptClient.x=10;
 ptClient.y=10;
 SendMessage(hWndEdit, AEM_POINTONSELECTION, (WPARAM)&ptClient, 0);


AEM_POINTONURL
______________

Checks is point on URL.

(POINT * )wParam       == coordinates of a point in the control's client area.
(AECHARRANGE * )lParam == pointer to a AECHARRANGE structure, that receives URL range, if found. Can be NULL.

Return Value
 Detected URL length or zero if not found.

Example:
 AECHARRANGE cr;
 POINT ptClient;

 ptClient.x=10;
 ptClient.y=10;
 SendMessage(hWndEdit, AEM_POINTONURL, (WPARAM)&ptClient, (LPARAM)&cr);


AEM_LINEFROMVPOS
________________

Retrieves line from vertical position.

(int)wParam     == see AECT_* defines.
(INT_PTR)lParam == vertical position.

Return Value
 Zero based line number.

Example:
 POINT ptClient;

 SendMessage(hWndEdit, AEM_GETCARETPOS, (LPARAM)&ptClient, (LPARAM)NULL);
 SendMessage(hWndEdit, AEM_LINEFROMVPOS, AECT_CLIENT, ptClient.y);


AEM_VPOSFROMLINE
________________

Retrieves vertical position from line.

(int)wParam == see AECT_* defines.
(int)lParam == zero based line number.

Return Value
 Vertical position.

Example:
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_VPOSFROMLINE, AECT_CLIENT, ciCaret.nLine);


AEM_GETMOUSESTATE
_________________

Retrieve mouse states.

(int)wParam == State type. See AEMS_* defines.
lParam      == not used.

Return Value
 If AEMS_CAPTURE specified, see AEMSC_* defines.
 If AEMS_SELECTION specified, see AEMSS_* defines.

Example:
 SendMessage(hWndEdit, AEM_GETMOUSESTATE, AEMS_CAPTURE, 0);


AEM_CONTROLCLASS
________________

Retrieve control class AkelEdit or RichEdit.

wParam == not used.
lParam == not used.

Return Value
 See AECLASS_* defines.

Example:
 SendMessage(hWndEdit, AEM_CONTROLCLASS, 0, 0);


AEM_CONTROLVERSION
__________________

Retrieve control version.

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

 dwVersion=SendMessage(hWndEdit, AEM_CONTROLVERSION, 0, 0);
 dwMajor=LOBYTE(LOWORD(dwVersion));
 dwMinor=HIBYTE(LOWORD(dwVersion));
 dwRelease=LOBYTE(HIWORD(dwVersion));
 dwBuild=HIBYTE(HIWORD(dwVersion));


AEM_GETEVENTMASK
________________

Retrieve the event mask for an edit control. The event mask specifies which notification messages the control sends to its parent window.

wParam == not used.
lParam == not used.

Return Value
 See AENM_* defines.

Example:
 SendMessage(hWndEdit, AEM_GETEVENTMASK, 0, 0);


AEM_SETEVENTMASK
________________

Set the event mask for an edit control. The event mask specifies which notification messages the control sends to its parent window.

wParam        == not used.
(DWORD)lParam == see AENM_* defines.

Return Value
 Previous event mask.

Example:
 SendMessage(hWndEdit, AEM_SETEVENTMASK, 0, AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY);


AEM_GETOPTIONS
______________

Retrieve edit control options.

wParam == not used.
lParam == not used.

Return Value
 Combination of the current option flag values. See AECO_* defines.

Example:
 SendMessage(hWndEdit, AEM_GETOPTIONS, 0, 0);


AEM_SETOPTIONS
______________

Set the options for an edit control.

(DWORD)wParam == see AECOOP_* defines.
(DWORD)lParam == see AECO_* defines.

Return Value
 Current options of edit control.

Example 1:
 SendMessage(hWndEdit, AEM_SETOPTIONS, AECOOP_OR, AECO_DISABLEDRAG|AECO_DISABLEDROP);

Example 2:
 if (bDisableNoScroll)
 {
   SendMessage(hWndEdit, AEM_SETOPTIONS, AECOOP_OR, AECO_DISABLENOSCROLL);
   ShowScrollBar(hWndEdit, SB_BOTH, TRUE);
   SetScrollRange(hWndEdit, SB_HORZ, 0, 100, TRUE);
   SetScrollRange(hWndEdit, SB_VERT, 0, 100, TRUE);
   SendMessage(hWndEdit, AEM_UPDATESCROLLBAR, SB_BOTH, 0);
 }
 else
 {
   SendMessage(hWndEdit, AEM_SETOPTIONS, AECOOP_XOR, AECO_DISABLENOSCROLL);
   ShowScrollBar(hWndEdit, SB_BOTH, TRUE);
   SendMessage(hWndEdit, AEM_UPDATESCROLLBAR, SB_BOTH, 0);
 }


AEM_GETNEWLINE
______________

Retrieve the default new line options for an edit control.

wParam == not used.
lParam == not used.

Return Value
 The low-order word contains the default input new line.
 The high-order word contains the default output new line.

Remarks
 Possible new lines: AELB_ASIS, AELB_R, AELB_N, AELB_RN, AELB_RRN.

Example:
 SendMessage(hWndEdit, AEM_GETNEWLINE, 0, 0);


AEM_SETNEWLINE
______________

Set the default new line options for an edit control.

(DWORD)wParam == see AENL_* defines.
(DWORD)lParam == the low-order word contains the default input new line. Valid if AENL_INPUT is specified.
                 the high-order word contains the default output new line. Valid if AENL_OUTPUT is specified.

Return Value
 Zero.

Remarks
 Possible new lines: AELB_ASIS, AELB_R, AELB_N, AELB_RN, AELB_RRN.

Example:
 SendMessage(hWndEdit, AEM_SETNEWLINE, AENL_INPUT|AENL_OUTPUT, MAKELONG(AELB_R, AELB_RN));


AEM_GETCOLORS
_____________

Retrieve colors of the edit control.

wParam             == not used.
(AECOLORS * )lParam == pointer to a AECOLORS structure.

Return Value
 Zero.

Example:
 AECOLORS aec;

 aec.dwFlags=AECLR_ALL;
 SendMessage(hWndEdit, AEM_GETCOLORS, 0, (LPARAM)&aec);


AEM_SETCOLORS
_____________

Set colors of the edit control.

wParam             == not used.
(AECOLORS * )lParam == pointer to a AECOLORS structure.

Return Value
 Zero.

Example:
 AECOLORS aec;

 aec.dwFlags=AECLR_BASICTEXT|AECLR_BASICBK;
 aec.crBasicText=GetSysColor(COLOR_WINDOWTEXT);
 aec.crBasicBk=GetSysColor(COLOR_WINDOW);
 SendMessage(hWndEdit, AEM_SETCOLORS, 0, (LPARAM)&aec);


AEM_EXGETOPTIONS
________________

Retrieve edit control extended options.

wParam == not used.
lParam == not used.

Return Value
 Combination of the current extended option flag values. See AECOE_* defines.

Example:
 SendMessage(hWndEdit, AEM_EXGETOPTIONS, 0, 0);


AEM_EXSETOPTIONS
________________

Set the options for an edit control.

(DWORD)wParam == see AECOOP_* defines.
(DWORD)lParam == see AECOE_* defines.

Return Value
 Current extended options of edit control.

Example:
 SendMessage(hWndEdit, AEM_EXSETOPTIONS, AECOOP_OR, AECOE_DETECTURL|AECOE_OVERTYPE);


AEM_GETCARETWIDTH
_________________

Retrieve caret width.

wParam          == not used.
(POINT * )lParam == pointer to a POINT structure that receives the insert mode caret width in x member and overtype mode caret height in y member.

Return Value
 Zero.

Example:
 POINT pt;

 SendMessage(hWndEdit, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);


AEM_SETCARETWIDTH
_________________

Set caret width.

wParam          == not used.
(POINT * )lParam == pointer to a POINT structure which specifies the insert mode caret width in x member and overtype mode caret height in y member.

Return Value
 Zero.

Example:
 POINT pt;

 pt.x=2;
 pt.y=3;
 SendMessage(hWndEdit, AEM_SETCARETWIDTH, 0, (LPARAM)&pt);


AEM_GETTABSTOP
______________

Retrieve tab stop size.

wParam == not used.
lParam == not used.

Return Value
 Tab stop size in characters.

Example:
 SendMessage(hWndEdit, AEM_GETTABSTOP, 0, 0);


AEM_SETTABSTOP
______________

Set tab stop size.

(int)wParam == new tab stop size in characters. Default value is used if zero.
lParam      == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETTABSTOP, 4, 0);


AEM_GETWORDWRAP
_______________

Retrieve word wrap mode.

wParam          == not used.
(DWORD * )lParam == pointer to a variable that receives wrap limit number. Can be NULL.

Return Value
 See AEWW_* defines.

Example:
 DWORD dwWrapLimit;

 SendMessage(hWndEdit, AEM_GETWORDWRAP, 0, (LPARAM)&dwWrapLimit);


AEM_SETWORDWRAP
_______________

Set word wrap mode.

(DWORD)wParam == see AEWW_* defines.
(int)lParam   == wrap limit, if zero window right edge is used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETWORDWRAP, AEWW_WORD, 0);


AEM_GETWORDDELIMITERS
_____________________

Retrieve word break delimiters.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t * )lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, not including the terminating NULL character.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETWORDDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETWORDDELIMITERS
_____________________

Set word break delimiters.

wParam            == not used.
(wchar_t * )lParam == string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n[](){}<>";

 SendMessage(hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETWRAPDELIMITERS
_____________________

Retrieve word wrapping delimiters.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t * )lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, not including the terminating NULL character.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETWRAPDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETWRAPDELIMITERS
_____________________

Set delimiters for word wrapping.

wParam            == not used.
(wchar_t * )lParam == string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n[](){}<>";

 SendMessage(hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETURLLEFTDELIMITERS
________________________

Retrieve URL left delimiters.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t * )lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, not including the terminating NULL character.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETURLLEFTDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETURLLEFTDELIMITERS
________________________

Set left delimiters for URL detection.

wParam            == not used.
(wchar_t * )lParam == string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n'`\"(<{[=";

 SendMessage(hWndEdit, AEM_SETURLLEFTDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETURLRIGHTDELIMITERS
________________________

Retrieve URL right delimiters.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t * )lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, not including the terminating NULL character.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETURLRIGHTDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETURLRIGHTDELIMITERS
________________________

Set right delimiters for URL detection.

wParam            == not used.
(wchar_t * )lParam == string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n'`\")>}]";

 SendMessage(hWndEdit, AEM_SETURLRIGHTDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETURLPREFIXES
__________________

Retrieve URL prefixes.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t * )lParam == pointer to a buffer that receives pairs of null-terminated prefixes strings. The last string terminated by two NULL characters.

Return Value
 Number of characters copied, including two NULL terminated characters.

Example:
 wchar_t wszPrefixes[128];

 SendMessage(hWndEdit, AEM_GETURLPREFIXES, 128, (LPARAM)wszPrefixes);


AEM_SETURLPREFIXES
__________________

Set prefixes for URL detection.

wParam            == not used.
(wchar_t * )lParam == buffer containing pairs of null-terminated prefixes strings. The last string in the buffer must be terminated by two NULL characters. If NULL, then default prefixes will be used.

Return Value
 Number of prefixes strings.

Example:
 wchar_t wszPrefixes[128]=L"http:\0https:\0www.\0ftp:\0file:\0mailto:\0\0"

 SendMessage(hWndEdit, AEM_SETURLPREFIXES, 0, (LPARAM)wszPrefixes);


AEM_GETURLMAXLENGTH
___________________

Retrieve URL maximum length.

wParam == not used.
lParam == not used.

Return Value
 URL maximum length.

Example:
 SendMessage(hWndEdit, AEM_GETURLMAXLENGTH, 0, 0);


AEM_SETURLMAXLENGTH
___________________

Set URL maximum length.

(DWORD)wParam == URL maximum length (default is 512).
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETURLMAXLENGTH, 1024, 0);


AEM_GETWORDBREAK
________________

Retrieve word break movement.

wParam == not used.
lParam == not used.

Return Value
 See AEWB_* defines.

Example:
 SendMessage(hWndEdit, AEM_GETWORDBREAK, 0, 0);


AEM_SETWORDBREAK
________________

Set word break movement.

(DWORD)wParam == see AEWB_* defines.
lParam        == not used.

Return Value
 Previous word break movement. See AEWB_* defines.

Example:
 SendMessage(hWndEdit, AEM_SETWORDBREAK, AEWB_LEFTWORDSTART|AEWB_RIGHTWORDSTART, 0);


AEM_GETMARKER
_____________

Retrieve marker column.

(int * )wParam == pointer to a variable that receives marker type, see AEMT_* defines. Can be NULL.
lParam        == not used.

Return Value
 Column number, zero if no marker set.

Example:
 SendMessage(hWndEdit, AEM_GETMARKER, (WPARAM)NULL, 0);


AEM_SETMARKER
_____________

Set marker at specified position.

(int)wParam == see AEMT_* defines.
(int)lParam == integer, zero to clear marker.

Return Value
 TRUE  marker type or position is changed.
 FALSE no changes made.

Example:
 SendMessage(hWndEdit, AEM_SETMARKER, AEMT_SYMBOL, 80);


AEM_GETLINEGAP
______________

Retrieve character external leading.

wParam == not used.
lParam == not used.

Return Value
 Line gap (external leading).

Example:
 SendMessage(hWndEdit, AEM_GETLINEGAP, 0, 0);


AEM_SETLINEGAP
______________

Set character external leading.

(DWORD)wParam == line gap (external leading), default is zero.
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETLINEGAP, 3, 0);


AEM_GETTEXTLIMIT
________________

Gets the current text limit for an edit control.

wParam == not used.
lParam == not used.

Return Value
 Text limit.

Example:
 SendMessage(hWndEdit, AEM_GETTEXTLIMIT, 0, 0);


AEM_SETTEXTLIMIT
________________

Sets the text limit of an edit control. The text limit is the maximum amount of text, in TCHARs, that the user can type into the edit control.

(UINT_PTR)wParam == the maximum number of TCHARs the user can enter. Default is unlimited.
lParam           == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETTEXTLIMIT, MAX_PATH, 0);


AEM_GETFONT
___________

Retrieve font handle.

(int)wParam == see AEGF_* defines.
lParam      == not used.

Return Value
 Font handle.

Example:
 HFONT hFontItalic=(HFONT)SendMessage(hWndEdit, AEM_GETFONT, AEGF_ITALIC, 0);


AEM_GETALTLINE
______________

Retrieve alternating lines intervals.

wParam == not used.
lParam == not used.

Return Value
 The low-order word contains the skip interval in lines.
 The high-order word contains the fill interval in lines.

Example:
 SendMessage(hWndEdit, AEM_GETALTLINE, 0, 0);


AEM_SETALTLINE
______________

Set alternating lines intervals.

(DWORD)wParam == the low-order word contains the skip interval in lines.
                 the high-order word contains the fill interval in lines.
lParam        == not used.

Return Value
 Zero.

Remarks
 If wParam is zero, even lines drawing is off.

Example:
 SendMessage(hWndEdit, AEM_SETALTLINE, MAKELONG(1, 1), 0);


AEM_GETCHARCOLORS
_________________

Retrieve character colors.

(AECHARINDEX * )wParam  == character index.
(AECHARCOLORS * )lParam == pointer to a AECHARCOLORS structure.

Return Value
 TRUE  character in selection.
 FALSE character not in selection.

Example:
 AECHARINDEX ciCaret;
 AECHARCOLORS aelc;

 aelc.dwFlags=0;
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_GETCHARCOLORS, (WPARAM)&ciCaret, (LPARAM)&aelc);


AEM_SHOWSCROLLBAR
_________________

Show or hide scroll bars in the edit control.

(int)wParam  == SB_BOTH  horizontal and vertical scroll bars.
                SB_HORZ  horizontal scroll bar.
                SB_VERT  vertical scroll bar.
                -1       only retrieve current SB_* visibility, lParam is ignored.
(BOOL)lParam == TRUE   show.
                FALSE  hide.

Return Value
 Previous SB_* visibility or -1 if no visibile scroll bars.

Example:
 SendMessage(hWndEdit, AEM_SHOWSCROLLBAR, SB_BOTH, FALSE);


AEM_UPDATESCROLLBAR
___________________

Update scroll bars in the edit control.

(int)wParam  == SB_BOTH  horizontal and vertical scroll bars.
                SB_HORZ  horizontal scroll bar.
                SB_VERT  vertical scroll bar.
lParam       == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_UPDATESCROLLBAR, SB_BOTH, 0);


AEM_UPDATECARET
_______________

Update caret in the edit control.

wParam == not used.
lParam == not used.

Return Value
 TRUE   window has focus.
 FALSE  window hasn't focus.

Example:
 SendMessage(hWndEdit, AEM_UPDATECARET, 0, 0);


AEM_UPDATESIZE
______________

Update edit control after window resizing. Useful for virtual document handle.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_UPDATESIZE, 0, 0);


AEM_LOCKUPDATE
______________

Lock update of specified objects.

(DWORD)wParam == see AELU_* defines.
(BOOL)lParam  == TRUE  lock update.
                 FALSE unlock update.

Return Value
 New update state, see AELU_* defines.

Example:
 RECT rc={10, 10, 200, 200};

 SendMessage(hWndEdit, AEM_LOCKUPDATE, AELU_SCROLLBAR|AELU_CARET, TRUE);
 SendMessage(hWndEdit, AEM_SETRECT, FALSE, (LPARAM)&rc);
 SendMessage(hWndEdit, AEM_LOCKUPDATE, AELU_SCROLLBAR|AELU_CARET, FALSE);


AEM_LOCKERASERECT
_________________

Lock the erasing rectangle of an edit control. Make sense in WM_ERASEBKGND respond.

wParam               == not used.
(const RECT * )lParam == pointer to a RECT structure that specifies the erasing rectangle.

Return Value
 TRUE   rectangle has been locked.
 FALSE  rectangle not in erase area.

Example:
 if (uMsg == WM_ERASEBKGND)
 {
   RECT rcKeep={10, 0, 100, 100}; //Don't erase this rectangle, to avoid flashing.

   SendMessage(hWndEdit, AEM_LOCKERASERECT, 0, (LPARAM)&rcKeep);
 }


AEM_GETERASERECT
________________

Retrieve the erasing rectangle of an edit control. By default all edit area is erased.

(DWORD)wParam  == see AERC_* defines.
(RECT * )lParam == pointer to a RECT structure that receives the erasing rectangle.

Return Value
 Zero.

Example:
 RECT rc;

 SendMessage(hWndEdit, AEM_GETERASERECT, 0, (LPARAM)&rc);


AEM_SETERASERECT
________________

Set the erasing rectangle of an edit control. The erasing rectangle is the limiting rectangle into which the control erase background.

(DWORD)wParam  == see AERC_* defines.
(RECT * )lParam == pointer to a RECT structure that specifies the new dimensions of the rectangle. If this parameter is NULL, the erasing rectangle is set to its default values.

Return Value
 Zero.

Example (exclude 10 left pixels from erasing):
 RECT rc;

 rc.left=10;
 SendMessage(hWndEdit, AEM_SETERASERECT, AERC_NOTOP|AERC_NORIGHT|AERC_NOBOTTOM, (LPARAM)&rc);


AEM_HIDESELECTION
_________________

Hides or shows the selection in an edit control.

(BOOL)wParam == TRUE   hide selection.
                FALSE  show selection.
lParam       == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_HIDESELECTION, TRUE, 0);


AEM_REDRAWLINERANGE
___________________

Redraws range of lines.

(int)wParam == first line to redraw. If -1, redraw from top of edit control.
(int)lParam == last line to redraw. If -1, redraw to bottom of edit control.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_REDRAWLINERANGE, 10, (LPARAM)-1);


AEM_GETBACKGROUNDIMAGE
______________________

Retrieve background image.

wParam == not used.
lParam == not used.

Return Value
 Image handle or NULL, if not set.

Example:
 HBITMAP hBitmap=(HBITMAP)SendMessage(hWndEdit, AEM_GETBACKGROUNDIMAGE, 0, 0);


AEM_SETBACKGROUNDIMAGE
______________________

Set background image.

(HBITMAP)wParam == image handle. If NULL, image removed from background.
(int)lParam     == alpha transparency value that ranges from 0 to 255 (default is 128).

Return Value
 TRUE   success.
 FALSE  failed.

Example:
HBITMAP hBkImage;

if (hBkImage=(HBITMAP)LoadImageA(NULL, "c:\\MyBackground.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE))
{
  SendMessage(hWndEdit, AEM_SETBACKGROUNDIMAGE, (WPARAM)hBkImage, 128);
}


AEM_GETFOLDSTACK
________________

Retrieve fold stack handle.

wParam == not used.
lParam == not used.

Return Value
 Pointer to a HSTACK structure.

Example:
AEFOLD* GetFold(HWND hWnd, int nLine)
{
  HSTACK *hFoldsStack;
  AEFOLD *lpFold;
  AEFOLD *lpResult=NULL;

  hFoldsStack=(HSTACK * )SendMessage(hWndEdit, AEM_GETFOLDSTACK, 0, 0);

  for (lpFold=(AEFOLD * )hFoldsStack->first; lpFold; lpFold=AEC_NextFold(lpFold, TRUE))
  {
    if (lpFold->lpMinPoint->ciPoint.nLine > nLine)
      break;
    if (lpFold->lpMaxPoint->ciPoint.nLine >= nLine)
      lpResult=lpFold;
  }
  return lpResult;
}


AEM_GETFOLDCOUNT
________________

Retrieve folds count.

wParam == not used.
lParam == not used.

Return Value
 Number of folds.

Example:
  SendMessage(hWndEdit, AEM_GETFOLDCOUNT, 0, 0);


AEM_ADDFOLD
___________

Add fold.

wParam           == not used.
(AEFOLD * )lParam == pointer to a filled AEFOLD structure. AEFOLD.next, AEFOLD.prev, AEFOLD.parent, AEFOLD.firstChild, AEFOLD.lastChild are ignored.

Return Value
 Fold handle (pointer to a AEFOLD structure).

Example:
 AEPOINT pointMin={0};
 AEPOINT pointMax={0};
 AEFOLD fold;

 pointMin.nPointOffset=AEPTO_CALC;
 pointMax.nPointOffset=AEPTO_CALC;
 SendMessage(hWndEdit, AEM_EXGETSEL, (WPARAM)&pointMin.ciPoint, (LPARAM)&pointMax.ciPoint);
 fold.lpMinPoint=&pointMin;
 fold.lpMaxPoint=&pointMax;
 fold.bCollapse=FALSE;
 fold.dwFontStyle=AEHLS_NONE;
 fold.crText=RGB(0xFF, 0x00, 0x00);
 fold.crBk=(DWORD)-1;
 fold.dwUserData=0;
 SendMessage(hWndEdit, AEM_ADDFOLD, 0, (LPARAM)&fold);


AEM_DELETEFOLD
______________

Deletes specified or all folds.

(AEFOLD * )wParam == fold handle (pointer to a AEFOLD structure). If NULL, then delete all folds.
(BOOL)lParam     == TRUE  update scroll and selection.
                    FALSE don't update scroll and selection.

Return Value
 Number of deleted folds that were collapsed.

Example:
 SendMessage(hWndEdit, AEM_DELETEFOLD, (WPARAM)lpFold, TRUE);


AEM_ISFOLDVALID
_______________

Checks is fold handle valid.

(AEFOLD * )wParam == fold handle (pointer to a AEFOLD structure), returned by AEM_ADDFOLD.
lParam           == not used.

Return Value
 TRUE   fold handle is valid.
 FALSE  fold handle isn't valid.

Example:
 SendMessage(hWndEdit, AEM_ISFOLDVALID, (WPARAM)lpFold, 0);


AEM_FINDFOLD
____________

Find fold handle.

(AEFINDFOLD * )wParam == pointer to a AEFINDFOLD structure.
lParam               == not used.

Return Value
 Zero.

Example:
 AEFINDFOLD ff;
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 ff.dwFlags=AEFF_FINDINDEX|AEFF_FOLDSTART|AEFF_GETROOT;
 ff.dwFindIt=(UINT_PTR)&ciCaret;
 SendMessage(hWndEdit, AEM_FINDFOLD, (WPARAM)&ff, 0);


AEM_NEXTFOLD
____________

Retrieves next fold handle.

(AEFOLD * )wParam == fold handle (pointer to a AEFOLD structure).
(BOOL)lParam     == TRUE   go into fold children if possible (recursive).
                    FALSE  don't go into fold children.

Return Value
 Fold handle (pointer to a AEFOLD structure).

Remarks
 For better performance use AEC_NextFold instead.

Example:
 AEFOLD *lpNextFold;

 lpNextFold=(AEFOLD * )SendMessage(hWndEdit, AEM_NEXTFOLD, (WPARAM)lpFold, TRUE);


AEM_PREVFOLD
____________

Retrieves previous fold handle.

(AEFOLD * )wParam == fold handle (pointer to a AEFOLD structure).
(BOOL)lParam     == TRUE   go into fold children if possible (recursive).
                    FALSE  don't go into fold children.

Return Value
 Fold handle (pointer to a AEFOLD structure).

Remarks
 For better performance use AEC_PrevFold instead.

Example:
 AEFOLD *lpPrevFold;

 lpPrevFold=(AEFOLD * )SendMessage(hWndEdit, AEM_PREVFOLD, (WPARAM)lpFold, TRUE);


AEM_ISLINECOLLAPSED
___________________

Checks is line collapsed.

(int)wParam == line number.
lParam      == not used.

Return Value
 Fold handle (pointer to a AEFOLD structure) that hides the line or NULL if line isn't collapsed.

Example:
 AEFOLD *lpFold;

 lpFold=(AEFOLD * )SendMessage(hWndEdit, AEM_ISLINECOLLAPSED, 5, 0);


AEM_COLLAPSELINE
________________

Collapse or expand all folds that contain line.

(int)wParam   == zero based line number.
(DWORD)lParam == see AECF_* defines.

Return Value
 Number of folds changed.

Example:
 SendMessage(hWndEdit, AEM_COLLAPSELINE, 5, AECF_EXPAND);


AEM_COLLAPSEFOLD
________________

Sets fold collapse state.

(AEFOLD * )wParam == fold handle (pointer to a AEFOLD structure), returned by AEM_ADDFOLD. If NULL, then process all folds.
(DWORD)lParam    == see AECF_* defines.

Return Value
 Number of folds changed.

Example:
 SendMessage(hWndEdit, AEM_COLLAPSEFOLD, (WPARAM)lpFold, AECF_COLLAPSE);


AEM_UPDATEFOLD
______________

Update scroll and selection. Typically can be used after fold deletion/collaption.

wParam      == not used.
(int)lParam == first visible line that was before fold deletion/collaption. If -1, ignored.

Return Value
 Pixels scrolled.

Example:
 nFirstVisibleLine=SendMessage(hWndEdit, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0);
 SendMessage(hWndEdit, AEM_DELETEFOLD, (WPARAM)lpFold1, FALSE);
 SendMessage(hWndEdit, AEM_COLLAPSEFOLD, (WPARAM)lpFold2, AECF_EXPAND|AECF_NOUPDATE);
 SendMessage(hWndEdit, AEM_UPDATEFOLD, 0, nFirstVisibleLine);


AEM_CREATEDOCUMENT
__________________

Create new virtual document, that later can be associated with any edit window. Note that new edit window after creation already has assigned document handle.

wParam                 == not used.
(CREATESTRUCT * )lParam == pointer to a CREATESTRUCT structure.

Return Value
 Created document handle.

Example:
 CREATESTRUCTA cs;
 AEHDOC hDocNew;
 AEHDOC hDocOld;

 cs.lpCreateParams=NULL;
 cs.hInstance=GetModuleHandle(NULL);
 cs.hMenu=(HMENU)100;
 cs.hwndParent=GetParent(hWndEdit);
 cs.cy=CW_USEDEFAULT;
 cs.cx=CW_USEDEFAULT;
 cs.y=CW_USEDEFAULT;
 cs.x=CW_USEDEFAULT;
 cs.style=WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_DISABLENOSCROLL;
 cs.lpszName=NULL;
 cs.lpszClass=AES_AKELEDITCLASSA;
 cs.dwExStyle=WS_EX_CLIENTEDGE;

 hDocNew=(AEHDOC)SendMessage(hWndEdit, AEM_CREATEDOCUMENT, 0, (LPARAM)&cs);
 hDocOld=(AEHDOC)SendMessage(hWndEdit, AEM_SETDOCUMENT, (WPARAM)hDocNew, 0);


AEM_DELETEDOCUMENT
__________________

Destroys document handle. Document handle must be not associated with any edit window.

(AEHDOC)wParam == document handle.
lParam         == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_DELETEDOCUMENT, (WPARAM)hDoc, 0);


AEM_GETDOCUMENT
_______________

Retrieve document handle.

wParam == not used.
lParam == not used.

Return Value
 Document handle.

Example:
 AEHDOC hDoc=(AEHDOC)SendMessage(hWndEdit, AEM_GETDOCUMENT, 0, 0);


AEM_SETDOCUMENT
_______________

Associate or deassociate document handle with edit control to which message is send.

(AEHDOC)wParam == document handle.
(DWORD)lParam  == see AESWD_* defines.

Return Value
 Old document handle.

Example:
 See AEM_CREATEDOCUMENT example.


AEM_GETDOCUMENTPROC
___________________

Retrieve procedure of document.

(AEHDOC)wParam == document handle. If NULL, current procedure returned.
lParam         == not used.

Return Value
 Pointer to an AEEditProc procedure.

Example (Call window procedure directly):
 AEHDOC hDoc=(AEHDOC)SendMessage(hWndEdit, AEM_GETDOCUMENT, 0, 0);
 AEEditProc lpEditProc=(AEEditProc)SendMessage(hWndEdit, AEM_GETDOCUMENTPROC, (WPARAM)hDoc, 0);
 LRESULT lResult;

 lResult=lpEditProc(hDoc, EM_SETSEL, (WPARAM)0, (LPARAM)-1);


AEM_GETDOCUMENTWINDOW
_____________________

Retrieve edit control of document.

(AEHDOC)wParam == document handle. If NULL, current edit control returned.
lParam         == not used.

Return Value
 Edit control handle.

Example:
 AEHDOC hDoc=(AEHDOC)SendMessage(hWndEditFirst, AEM_GETDOCUMENT, 0, 0);
 HWND hWnd=(HWND)SendMessage(hWndEditSecond, AEM_GETDOCUMENTWINDOW, (WPARAM)hDoc, 0);
 //hWnd == hWndEditFirst


AEM_SENDMESSAGE
_______________

Send message to an associated or deassociated document handle.

lParam                  == not used.
(AESENDMESSAGE * )lParam == pointer to a AESENDMESSAGE structure.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AESENDMESSAGE sm;

 sm.hDoc=hDoc;
 sm.uMsg=EM_SETSEL;
 sm.wParam=0;
 sm.lParam=(LPARAM)-1;
 if (SendMessage(hWndAnyEdit, AEM_SENDMESSAGE, 0, (LPARAM)&sm))
   return sm.lResult;


AEM_ADDCLONE
____________

Adds clone to the master window. Message send to a master window which will be cloned.

(HWND)wParam == edit control handle which become a clone. Text of the master and clone windows will be the same.
lParam       == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_ADDCLONE, (WPARAM)hWndEdit2, 0);


AEM_DELCLONE
____________

Removes clone from the master window. Message send to a master window.

(HWND)wParam == edit control handle that will lose clone status. Text of the clone window will be restored.
lParam       == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_DELCLONE, (WPARAM)hWndEdit2, 0);


AEM_GETMASTER
_____________

Retrieve master window handle. Message send to a master or slave window.

wParam == not used.
lParam == not used.

Return Value
 Master window handle.

Example:
 HWND hWndMaster;

 if (hWndMaster=(HWND)SendMessage(hWndEdit, AEM_GETMASTER, 0, 0))
 {
   if (hWndMaster == hWndEdit)
     MessageBoxA(NULL, "hWndEdit is master", NULL, 0);
   else
     MessageBoxA(NULL, "hWndEdit is slave", NULL, 0);
 }
 else MessageBoxA(NULL, "hWndEdit nor master, nor slave", NULL, 0);


AEM_GETCLONE
____________

Retrieve clone window handle. Message send to a master window.

(DWORD)wParam == zero based clone index.
lParam        == not used.

Return Value
 Clone window handle.

Example:
 SendMessage(hWndEdit, AEM_GETCLONE, 2, 0);


AEM_STARTPRINTDOC
_________________

Prepare document printing.

wParam            == not used.
(AEPRINT * )lParam == pointer to a AEPRINT structure.

Return Value
 Document handle.

Example:
 PRINTDLGA pdlg={0};
 DOCINFOA di={0};
 AEPRINT prn;
 AEHPRINT hPrintDoc;
 BOOL bPrintStop=FALSE;

 //Choose printer
 pdlg.lStructSize=sizeof(PRINTDLGA);
 pdlg.hwndOwner=hWndEdit;
 pdlg.Flags=PD_ALLPAGES|PD_NOPAGENUMS|PD_RETURNDC|PD_USEDEVMODECOPIESANDCOLLATE;
 pdlg.nCopies=1;
 if (!PrintDlgA(&pdlg)) return;

 //Set print settings
 prn.dwFlags=AEPRN_INHUNDREDTHSOFMILLIMETERS|AEPRN_WRAPWORD;
 prn.hPrinterDC=pdlg.hDC;
 prn.hEditFont=(HFONT)SendMessage(hWndEdit, WM_GETFONT, 0, 0);
 prn.rcMargins.left=1000;
 prn.rcMargins.top=1000;
 prn.rcMargins.right=1000;
 prn.rcMargins.bottom=1000;
 if (pdlg.Flags & PD_SELECTION)
 {
   SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTSELCHAR, (LPARAM)&prn.crText.ciMin);
   SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTSELCHAR, (LPARAM)&prn.crText.ciMax);
 }
 else
 {
   SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&prn.crText.ciMin);
   SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&prn.crText.ciMax);
 }

 //Document properties
 di.cbSize=sizeof(DOCINFOA);
 di.lpszDocName="AkelEdit document";
 if (pdlg.Flags & PD_PRINTTOFILE)
   di.lpszOutput="FILE:";

 //Start document
 if (StartDocA(pdlg.hDC, &di) > 0)
 {
   if (hPrintDoc=(AEHPRINT)SendMessage(hWndEdit, AEM_STARTPRINTDOC, 0, (LPARAM)&prn))
   {
     while (!bPrintStop)
     {
       if (StartPage(prn.hPrinterDC) > 0)
       {
         //Print page
         if (!SendMessage(hWndEdit, AEM_PRINTPAGE, (WPARAM)hPrintDoc, (LPARAM)&prn))
           bPrintStop=TRUE;

         if (EndPage(prn.hPrinterDC) <= 0)
           bPrintStop=TRUE;
       }
       else bPrintStop=TRUE;
     }
     SendMessage(hWndEdit, AEM_ENDPRINTDOC, (WPARAM)hPrintDoc, (LPARAM)&prn);
   }
   EndDoc(pdlg.hDC);
 }
 DeleteDC(pdlg.hDC);
 pdlg.hDC=NULL;


AEM_PRINTPAGE
_____________

Print page.

(AEHPRINT)wParam  == document handle returned by AEM_STARTPRINTDOC.
(AEPRINT * )lParam == pointer to a AEPRINT structure.

Return Value
 TRUE   page is printed and there is more text for printing.
 FALSE  page is printed and there is no more text for printing.

Example:
 See AEM_STARTPRINTDOC example.


AEM_ENDPRINTDOC
_______________

Close print document handle.

(AEHPRINT)wParam  == document handle returned by AEM_STARTPRINTDOC.
(AEPRINT * )lParam == pointer to a AEPRINT structure.

Return Value
 Zero.

Example:
 See AEM_STARTPRINTDOC example.


AEM_HLCREATETHEMEA
__________________

Create empty highlight theme.

wParam         == not used.
(char * )lParam == ansi theme name.

Return Value
 Theme handle.

Example:
 AEHTHEME hTheme;
 AEDELIMITEMA di;
 AEWORDITEMA wi;
 AEQUOTEITEMA qi;
 AEMARKRANGEITEM mri;
 AEMARKTEXTITEMA mti;

 if (hTheme=(AEHTHEME)SendMessage(hWndEdit, AEM_HLCREATETHEMEA, 0, (LPARAM)"MyTheme"))
 {
   di.nIndex=-1;
   di.pDelimiter=" ";
   di.nDelimiterLen=lstrlenA(di.pDelimiter);
   di.dwFlags=AEHLF_MATCHCASE;
   di.dwFontStyle=AEHLS_NONE;
   di.crText=(DWORD)-1;
   di.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDDELIMITERA, (WPARAM)hTheme, (LPARAM)&di);

   wi.pWord="for";
   wi.nWordLen=lstrlenA(wi.pWord);
   wi.dwFlags=AEHLF_MATCHCASE;
   wi.dwFontStyle=AEHLS_NONE;
   wi.crText=RGB(0x00, 0xFF, 0x00);
   wi.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDWORDA, (WPARAM)hTheme, (LPARAM)&wi);

   qi.nIndex=-1;
   qi.pQuoteStart="\"";
   qi.nQuoteStartLen=lstrlenA(qi.pQuoteStart);
   qi.pQuoteEnd="\"";
   qi.nQuoteEndLen=lstrlenA(qi.pQuoteEnd);
   qi.chEscape='\\';
   qi.pQuoteInclude=NULL;
   qi.nQuoteIncludeLen=0
   qi.pQuoteExclude=NULL;
   qi.nQuoteExcludeLen=0;
   qi.dwFlags=AEHLF_MATCHCASE;
   qi.dwFontStyle=AEHLS_NONE;
   qi.crText=RGB(0x00, 0x00, 0xFF);
   qi.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDQUOTEA, (WPARAM)hTheme, (LPARAM)&qi);

   mri.nIndex=-1;
   mri.crMarkRange.cpMin=10;
   mri.crMarkRange.cpMax=20;
   mri.dwFlags=0;
   mri.dwFontStyle=AEHLS_NONE;
   mri.crText=RGB(0xFF, 0x00, 0x00);
   mri.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDMARKRANGE, (WPARAM)hTheme, (LPARAM)&mri);

   mti.nIndex=-1;
   mti.pMarkText="or";
   mti.nMarkTextLen=lstrlenA(mti.pMarkText);
   mti.dwFlags=AEHLF_MATCHCASE;
   mti.dwFontStyle=AEHLS_NONE;
   mti.crText=(DWORD)-1;
   mti.crBk=RGB(0xFF, 0x00, 0x00);
   SendMessage(hWndEdit, AEM_HLADDMARKTEXTA, (WPARAM)hTheme, (LPARAM)&mti);

   SendMessage(hWndEdit, AEM_HLSETTHEME, (WPARAM)hTheme, TRUE);
 }


AEM_HLCREATETHEMEW
__________________

Create empty highlight theme.

wParam            == not used.
(wchar_t * )lParam == unicode theme name.

Return Value
 Theme handle.

Example:
 AEHTHEME hTheme;
 AEDELIMITEMW di;
 AEWORDITEMW wi;
 AEQUOTEITEMW qi;
 AEMARKRANGEITEM mri;
 AEMARKTEXTITEMW mti;

 if (hTheme=(AEHTHEME)SendMessage(hWndEdit, AEM_HLCREATETHEMEW, 0, (LPARAM)L"MyTheme"))
 {
   di.nIndex=-1;
   di.pDelimiter=L" ";
   di.nDelimiterLen=lstrlenW(di.pDelimiter);
   di.dwFlags=AEHLF_MATCHCASE;
   di.dwFontStyle=AEHLS_NONE;
   di.crText=(DWORD)-1;
   di.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDDELIMITERW, (WPARAM)hTheme, (LPARAM)&di);

   wi.pWord=L"for";
   wi.nWordLen=lstrlenW(wi.pWord);
   wi.dwFlags=AEHLF_MATCHCASE;
   wi.dwFontStyle=AEHLS_NONE;
   wi.crText=RGB(0x00, 0xFF, 0x00);
   wi.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDWORDW, (WPARAM)hTheme, (LPARAM)&wi);

   qi.nIndex=-1;
   qi.pQuoteStart=L"\"";
   qi.nQuoteStartLen=lstrlenW(qi.pQuoteStart);
   qi.pQuoteEnd=L"\"";
   qi.nQuoteEndLen=lstrlenW(qi.pQuoteEnd);
   qi.chEscape=L'\\';
   qi.pQuoteInclude=NULL;
   qi.nQuoteIncludeLen=0
   qi.pQuoteExclude=NULL;
   qi.nQuoteExcludeLen=0;
   qi.dwFlags=AEHLF_MATCHCASE;
   qi.dwFontStyle=AEHLS_NONE;
   qi.crText=RGB(0x00, 0x00, 0xFF);
   qi.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDQUOTEW, (WPARAM)hTheme, (LPARAM)&qi);

   mri.nIndex=-1;
   mri.crMarkRange.cpMin=10;
   mri.crMarkRange.cpMax=20;
   mri.dwFlags=0;
   mri.dwFontStyle=AEHLS_NONE;
   mri.crText=RGB(0xFF, 0x00, 0x00);
   mri.crBk=(DWORD)-1;
   SendMessage(hWndEdit, AEM_HLADDMARKRANGE, (WPARAM)hTheme, (LPARAM)&mri);

   mti.nIndex=-1;
   mti.pMarkText=L"or";
   mti.nMarkTextLen=lstrlenW(mti.pMarkText);
   mti.dwFlags=AEHLF_MATCHCASE;
   mti.dwFontStyle=AEHLS_NONE;
   mti.crText=(DWORD)-1;
   mti.crBk=RGB(0xFF, 0x00, 0x00);
   SendMessage(hWndEdit, AEM_HLADDMARKTEXTW, (WPARAM)hTheme, (LPARAM)&mti);

   SendMessage(hWndEdit, AEM_HLSETTHEME, (WPARAM)hTheme, TRUE);
 }


AEM_HLGETTHEMEA
_______________

Retrieve highlight theme handle.

wParam         == not used.
(char * )lParam == ansi theme name to retrieve. If NULL, active theme handle will be returned.

Return Value
 Theme handle.

Example:
 AEHTHEME hTheme=(AEHTHEME)SendMessage(hWndEdit, AEM_HLGETTHEMEA, 0, (LPARAM)"MyTheme");


AEM_HLGETTHEMEW
_______________

Retrieve highlight theme handle.

wParam            == not used.
(wchar_t * )lParam == unicode theme name to retrieve. If NULL, active theme handle will be returned.

Return Value
 Theme handle.

Example:
 AEHTHEME hTheme=(AEHTHEME)SendMessage(hWndEdit, AEM_HLGETTHEMEW, 0, (LPARAM)L"MyTheme");


AEM_HLGETTHEMENAMEA
___________________

Retrieve ansi highlight theme name.

(AEHTHEME)wParam == theme handle.
(char * )lParam   == pointer to a buffer that receives ansi highlight theme name. If NULL, return value is the required buffer size.

Return Value
 Length of the string copied to the buffer, in TCHARs, not including the terminating null character.

Example:
 char szThemeName[MAX_PATH];

 SendMessage(hWndEdit, AEM_HLGETTHEMENAMEA, (WPARAM)hTheme, (LPARAM)szThemeName);


AEM_HLGETTHEMENAMEW
___________________

Retrieve unicode highlight theme name.

(AEHTHEME)wParam  == theme handle.
(wchar_t * )lParam == pointer to a buffer that receives unicode highlight theme name. If NULL, return value is the required buffer size.

Return Value
 Length of the string copied to the buffer, in TCHARs, not including the terminating null character.

Example:
 wchar_t wszThemeName[MAX_PATH];

 SendMessage(hWndEdit, AEM_HLGETTHEMENAMEW, (WPARAM)hTheme, (LPARAM)wszThemeName);


AEM_HLGETTHEMESTACK
___________________

Retrieve one of the theme's stack.

(AEHTHEME)wParam == theme handle or NULL for default window theme.
(int)lParam      == see AEHLE_* defines.

Return Value
 Pointer to a HSTACK structure.

Example:
BOOL IsCharDelimiter(HWND hWnd, const AECHARINDEX *ciChar, BOOL bPrevious)
{
  AECHARINDEX ciTmp=*ciChar;
  AEHTHEME hTheme;
  HSTACK *hDelimStack;
  AEDELIMITEMW *lpDelimItem; //Original stack item is always unicode.
  int nChar;

  if (bPrevious)
  {
    if (!SendMessage(hWndEdit, AEM_GETINDEX, AEGI_PREVCHAR, (LPARAM)&ciTmp))
      return TRUE;
  }
  nChar=SendMessage(hWndEdit, AEM_CHARAT, (WPARAM)&ciTmp, 0);
  if (nChar < 0) return TRUE;

  if (hTheme=(AEHTHEME)SendMessage(hWndEdit, AEM_HLGETTHEMEW, 0, (LPARAM)NULL))
  {
    hDelimStack=(HSTACK * )SendMessage(hWndEdit, AEM_HLGETTHEMESTACK, (WPARAM)hTheme, AEHLE_DELIMITER);

    for (lpDelimItem=(AEDELIMITEMW * )hDelimStack->first; lpDelimItem; lpDelimItem=lpDelimItem->next)
    {
      if (lpDelimItem->nDelimiterLen == 1)
      {
        if (lpDelimItem->pDelimiter[0] == nChar)
          return TRUE;
      }
    }
  }
  return FALSE;
}


AEM_HLTHEMEEXISTS
_________________

Retrieve highlight theme existence.

(AEHTHEME)wParam == theme handle.
lParam           == not used.

Return Value
 TRUE   theme exists.
 FALSE  theme doesn't exist.

Example:
 SendMessage(hWndEdit, AEM_HLTHEMEEXISTS, (WPARAM)hTheme, 0);


AEM_HLSETTHEME
______________

Activate highlight theme.

(AEHTHEME)wParam == theme handle. If NULL, active theme will be deactivated.
(BOOL)lParam     == TRUE   redraw the edit window.
                    FALSE  don't redraw the edit window.

Return Value
 Zero.

Example:
 See AEM_HLCREATETHEMEA or AEM_HLCREATETHEMEW example.


AEM_HLDELETETHEME
_________________

Delete highlight theme.

(AEHTHEME)wParam == theme handle or NULL for remove all element in default window theme.
lParam           == not used.

Return Value
 Zero.

Example:
 AEHTHEME hTheme;

 hTheme=(AEHTHEME)SendMessage(hWndEdit, AEM_HLCREATETHEMEW, 0, (LPARAM)L"MyTheme");
 SendMessage(hWndEdit, AEM_HLDELETETHEME, (WPARAM)hTheme, 0);


AEM_HLDELETETHEMEALL
____________________

Delete all highlight themes created with AEM_HLCREATETHEME.

wParam == not used.
lParam == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_HLDELETETHEMEALL, 0, 0);


AEM_HLADDDELIMITERA
___________________

Add ansi delimiter to highlight theme.

(AEHTHEME)wParam       == theme handle or NULL for default window theme.
(AEDELIMITEMA * )lParam == pointer to a AEDELIMITEMA structure.

Return Value
 Delimiter handle - AEHDELIMITER (pointer to a AEDELIMITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDDELIMITERW
___________________

Add unicode delimiter to highlight theme.

(AEHTHEME)wParam       == theme handle or NULL for default window theme.
(AEDELIMITEMW * )lParam == pointer to a AEDELIMITEMW structure.

Return Value
 Delimiter handle - AEHDELIMITER (pointer to a AEDELIMITEMW structure).

Example:
 See AEM_HLCREATETHEMEW example.


AEM_HLDELETEDELIMITER
_____________________

Delete delimiter from highlight theme.

(AEHTHEME)wParam     == theme handle or NULL for default window theme.
(AEHDELIMITER)lParam == delimiter handle. If NULL, delete all delimiters.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_HLDELETEDELIMITER, (WPARAM)hTheme, (LPARAM)hDelimiter);


AEM_HLADDWORDA
______________

Add ansi word to highlight theme.

(AEHTHEME)wParam      == theme handle or NULL for default window theme.
(AEWORDITEMA * )lParam == pointer to a AEWORDITEMA structure.

Return Value
 Word handle - AEHWORD (pointer to a AEWORDITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDWORDW
______________

Add unicode word to highlight theme.

(AEHTHEME)wParam      == theme handle or NULL for default window theme.
(AEWORDITEMW * )lParam == pointer to a AEWORDITEMW structure.

Return Value
 Word handle - AEHWORD (pointer to a AEWORDITEMW structure).

Example:
 See AEM_HLCREATETHEMEW example.


AEM_HLDELETEWORD
________________

Delete word from highlight theme.

(AEHTHEME)wParam == theme handle or NULL for default window theme.
(AEHWORD)lParam  == word handle. If NULL, delete all words.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_HLDELETEWORD, (WPARAM)hTheme, (LPARAM)hWord);


AEM_HLADDQUOTEA
_______________

Add ansi quote to highlight theme.

(AEHTHEME)wParam       == theme handle or NULL for default window theme.
(AEQUOTEITEMA * )lParam == pointer to a AEQUOTEITEMA structure.

Return Value
 Quote handle - AEHQUOTE (pointer to a AEQUOTEITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDQUOTEW
_______________

Add unicode quote to highlight theme.

(AEHTHEME)wParam       == theme handle or NULL for default window theme.
(AEQUOTEITEMW * )lParam == pointer to a AEQUOTEITEMW structure.

Return Value
 Quote handle - AEHQUOTE (pointer to a AEQUOTEITEMW structure).

Example:
 See AEM_HLCREATETHEMEW example.


AEM_HLDELETEQUOTE
_________________

Delete quote from highlight theme.

(AEHTHEME)wParam == theme handle or NULL for default window theme.
(AEHQUOTE)lParam == quote handle. If NULL, delete all quotes.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_HLDELETEQUOTE, (WPARAM)hTheme, (LPARAM)hQuote);


AEM_HLADDMARKTEXTA
__________________

Add ansi text mark to highlight theme.

(AEHTHEME)wParam          == theme handle or NULL for default window theme.
(AEMARKTEXTITEMA * )lParam == pointer to a AEMARKTEXTITEMA structure.

Return Value
 Text mark handle - AEHMARKTEXT (pointer to a AEMARKTEXTITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDMARKTEXTW
__________________

Add unicode text mark to highlight theme.

(AEHTHEME)wParam          == theme handle or NULL for default window theme.
(AEMARKTEXTITEMW * )lParam == pointer to a AEMARKTEXTITEMW structure.

Return Value
 Text mark handle - AEHMARKTEXT (pointer to a AEMARKTEXTITEMW structure).

Example:
 See AEM_HLCREATETHEMEW example.


AEM_HLDELETEMARKTEXT
____________________

Delete text mark from highlight theme.

(AEHTHEME)wParam    == theme handle or NULL for default window theme.
(AEHMARKTEXT)lParam == text mark handle. If NULL, delete all marks.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_HLDELETEMARKTEXT, (WPARAM)hTheme, (LPARAM)hMark);


AEM_HLADDMARKRANGE
__________________

Add unicode range mark to highlight theme.

(AEHTHEME)wParam          == theme handle or NULL for default window theme.
(AEMARKRANGEITEM * )lParam == pointer to a AEMARKRANGEITEM structure.

Return Value
 Range mark handle - AEHMARKRANGE (pointer to a AEMARKRANGEITEM structure).

Example:
 See AEM_HLCREATETHEMEW example.


AEM_HLDELETEMARKRANGE
_____________________

Delete range mark from highlight theme.

(AEHTHEME)wParam     == theme handle or NULL for default window theme.
(AEHMARKRANGE)lParam == range mark handle. If NULL, delete all marks.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_HLDELETEMARKRANGE, (WPARAM)hTheme, (LPARAM)hMark);


AEM_HLGETHIGHLIGHT
__________________

Retrieve the highlighting information provided by an application defined callback function.

wParam                   == not used.
(AEGETHIGHLIGHT * )lParam == pointer to a AEGETHIGHLIGHT structure.

Return Value
 Zero.

Example:
 AEGETHIGHLIGHT aegh;

 aegh.dwCookie=0;
 aegh.lpCallback=GetHighLightCallback;
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&aegh.crText.ciMin);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&aegh.crText.ciMax);
 aegh.dwFlags=0;
 SendMessage(hWndEdit, AEM_HLGETHIGHLIGHT, 0, (LPARAM)&aegh);

 DWORD CALLBACK GetHighLightCallback(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp)
 {
   wchar_t wszRange[MAX_PATH];
   wchar_t wszMessage[MAX_PATH];

   lstrcpynW(wszRange, crAkelRange->ciMin.lpLine->wpLine + crAkelRange->ciMin.nCharInLine, crRichRange->cpMax - crRichRange->cpMin + 1);
   wsprintfW(wszMessage, L"[%s]\nFontStyle=%d\nActiveText=0x%06x\nActiveBk=0x%06x", wszRange, hlp->dwFontStyle, hlp->dwActiveText, hlp->dwActiveBk);
   MessageBoxW(NULL, wszMessage, NULL, 0);

   return 0;
 }


AEM_HLGETOPTIONS
________________

Retrieve highlighting options.

wParam == not used.
lParam == not used.

Return Value
 Combination of the current option flag values. See AEHLO_* defines.

Example:
 SendMessage(hWndEdit, AEM_HLGETOPTIONS, 0, 0);


AEM_HLSETOPTIONS
________________

Set highlighting options.

(DWORD)wParam == see AECOOP_* defines.
(DWORD)lParam == see AEHLO_* defines.

Return Value
 Current options of highlighting.

Example:
 SendMessage(hWndEdit, AEM_HLSETOPTIONS, AECOOP_OR, AEHLO_IGNOREFONTITALIC);

*)

{$IF CompilerVersion > 20}{$ENDREGION}{$IFEND}


//// UNICODE define
{$ifndef UNICODE}
const
  AES_AKELEDITCLASS = AES_AKELEDITCLASSA;
  {$EXTERNALSYM AES_AKELEDITCLASS}
  AES_RICHEDIT20 = AES_RICHEDIT20A;
  {$EXTERNALSYM AES_RICHEDIT20}

type
  TTEXTRANGE64 = TTEXTRANGE64A;
  {$EXTERNALSYM TTEXTRANGE64}
  TFINDTEXTEX64 = TFINDTEXTEX64A;
  {$EXTERNALSYM TFINDTEXTEX64}

  TAEAPPENDTEXT = TAEAPPENDTEXTA;
  {$EXTERNALSYM TAEAPPENDTEXT}
  TAEREPLACESEL = TAEREPLACESELA;
  {$EXTERNALSYM TAEREPLACESEL}
  TAETEXTRANGE = TAETEXTRANGEA;
  {$EXTERNALSYM TAETEXTRANGE}
  TAEFINDTEXT = TAEFINDTEXTA;
  {$EXTERNALSYM TAEFINDTEXT}
  TAEDELIMITEM = TAEDELIMITEMA;
  {$EXTERNALSYM TAEDELIMITEM}
  TAEWORDITEM = TAEWORDITEMA;
  {$EXTERNALSYM TAEWORDITEM}
  TAEQUOTEITEM = TAEQUOTEITEMA;
  {$EXTERNALSYM TAEQUOTEITEM}
  TAEMARKTEXTITEM = TAEMARKTEXTITEMA;
  {$EXTERNALSYM TAEMARKTEXTITEM}

const
  AEM_SETTEXT = AEM_SETTEXTA;
  {$EXTERNALSYM AEM_SETTEXT}
  AEM_APPENDTEXT = AEM_APPENDTEXTA;
  {$EXTERNALSYM AEM_APPENDTEXT}
  AEM_REPLACESEL = AEM_REPLACESELA;
  {$EXTERNALSYM AEM_REPLACESEL}
  AEM_GETTEXTRANGE = AEM_GETTEXTRANGEA;
  {$EXTERNALSYM AEM_GETTEXTRANGE}
  AEM_FINDTEXT = AEM_FINDTEXTA;
  {$EXTERNALSYM AEM_FINDTEXT}
  AEM_ISMATCH = AEM_ISMATCHA;
  {$EXTERNALSYM AEM_ISMATCH}
  AEM_HLCREATETHEME = AEM_HLCREATETHEMEA;
  {$EXTERNALSYM AEM_HLCREATETHEME}
  AEM_HLGETTHEME = AEM_HLGETTHEMEA;
  {$EXTERNALSYM AEM_HLGETTHEME}
  AEM_HLGETTHEMENAME = AEM_HLGETTHEMENAMEA;
  {$EXTERNALSYM AEM_HLGETTHEMENAME}
  AEM_HLADDDELIMITER = AEM_HLADDDELIMITERA;
  {$EXTERNALSYM AEM_HLADDDELIMITER}
  AEM_HLADDWORD = AEM_HLADDWORDA;
  {$EXTERNALSYM AEM_HLADDWORD}
  AEM_HLADDQUOTE = AEM_HLADDQUOTEA;
  {$EXTERNALSYM AEM_HLADDQUOTE}
  AEM_HLADDMARKTEXT = AEM_HLADDMARKTEXTA;
  {$EXTERNALSYM AEM_HLADDMARKTEXT}
{$else}
const
  AES_AKELEDITCLASS = AES_AKELEDITCLASSW;
  {$EXTERNALSYM AES_AKELEDITCLASS}
  AES_RICHEDIT20 = AES_RICHEDIT20W;
  {$EXTERNALSYM AES_RICHEDIT20}

type
  TTEXTRANGE64 = TTEXTRANGE64W;
  {$EXTERNALSYM TTEXTRANGE64}
  TFINDTEXTEX64 = TFINDTEXTEX64W;
  {$EXTERNALSYM TFINDTEXTEX64}

  TAEAPPENDTEXT = TAEAPPENDTEXTW;
  {$EXTERNALSYM TAEAPPENDTEXT}
  TAEREPLACESEL = TAEREPLACESELW;
  {$EXTERNALSYM TAEREPLACESEL}
  TAETEXTRANGE = TAETEXTRANGEW;
  {$EXTERNALSYM TAETEXTRANGE}
  TAEFINDTEXT = TAEFINDTEXTW;
  {$EXTERNALSYM TAEFINDTEXT}
  TAEDELIMITEM = TAEDELIMITEMW;
  {$EXTERNALSYM TAEDELIMITEM}
  TAEWORDITEM = TAEWORDITEMW;
  {$EXTERNALSYM TAEWORDITEM}
  TAEQUOTEITEM = TAEQUOTEITEMW;
  {$EXTERNALSYM TAEQUOTEITEM}
  TAEMARKTEXTITEM = TAEMARKTEXTITEMW;
  {$EXTERNALSYM TAEMARKTEXTITEM}

const
  AEM_SETTEXT = AEM_SETTEXTW;
  {$EXTERNALSYM AEM_SETTEXT}
  AEM_APPENDTEXT = AEM_APPENDTEXTW;
  {$EXTERNALSYM AEM_APPENDTEXT}
  AEM_REPLACESEL = AEM_REPLACESELW;
  {$EXTERNALSYM AEM_REPLACESEL}
  AEM_GETTEXTRANGE = AEM_GETTEXTRANGEW;
  {$EXTERNALSYM AEM_GETTEXTRANGE}
  AEM_FINDTEXT = AEM_FINDTEXTW;
  {$EXTERNALSYM AEM_FINDTEXT}
  AEM_ISMATCH = AEM_ISMATCHW;
  {$EXTERNALSYM AEM_ISMATCH}
  AEM_HLCREATETHEME = AEM_HLCREATETHEMEW;
  {$EXTERNALSYM AEM_HLCREATETHEME}
  AEM_HLGETTHEME = AEM_HLGETTHEMEW;
  {$EXTERNALSYM AEM_HLGETTHEME}
  AEM_HLGETTHEMENAME = AEM_HLGETTHEMENAMEW;
  {$EXTERNALSYM AEM_HLGETTHEMENAME}
  AEM_HLADDDELIMITER = AEM_HLADDDELIMITERW;
  {$EXTERNALSYM AEM_HLADDDELIMITER}
  AEM_HLADDWORD = AEM_HLADDWORDW;
  {$EXTERNALSYM AEM_HLADDWORD}
  AEM_HLADDQUOTE = AEM_HLADDQUOTEW;
  {$EXTERNALSYM AEM_HLADDQUOTE}
  AEM_HLADDMARKTEXT = AEM_HLADDMARKTEXTW;
  {$EXTERNALSYM AEM_HLADDMARKTEXT}
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
//  ((((high) - 0xD800) * 0x400) + ((low) - 0xDC00) + 0x10000)
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
