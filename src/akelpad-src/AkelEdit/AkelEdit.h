#ifndef __AKELEDIT_H__
#define __AKELEDIT_H__

#define AEC_FUNCTIONS   1


//// Includes

#include <richedit.h>
#include <shellapi.h>


//// Defines

#define AES_AKELEDITA            "AkelEditA"
#define AES_AKELEDITA_UNICODE   L"AkelEditA"
#define AES_AKELEDITW_ANSI       "AkelEditW"
#define AES_AKELEDITW           L"AkelEditW"
#define AES_RICHEDIT20A          "RichEdit20A"
#define AES_RICHEDIT20A_UNICODE L"RichEdit20A"
#define AES_RICHEDIT20W_ANSI     "RichEdit20W"
#define AES_RICHEDIT20W         L"RichEdit20W"

//AEM_CONTROLCLASS
#define AECLASS_AKELEDIT        1
#define AECLASS_RICHEDIT        2

//Window styles
#ifndef ES_MULTILINE
  #define ES_MULTILINE        0x00000004  //See AECO_MULTILINE.
#endif
#ifndef ES_NOHIDESEL
  #define ES_NOHIDESEL        0x00000100  //See AECO_NOHIDESEL.
#endif
#ifndef ES_READONLY
  #define ES_READONLY         0x00000800  //See AECO_READONLY.
#endif
#ifndef ES_WANTRETURN
  #define ES_WANTRETURN       0x00001000  //See AECO_WANTRETURN.
#endif
#ifndef ES_DISABLENOSCROLL
  #define ES_DISABLENOSCROLL  0x00002000  //See AECO_DISABLENOSCROLL.
#endif
#define ES_GLOBALUNDO         0x00004000  //Use process heap for Undo/Redo instead of window heap. Required for AEM_DETACHUNDO and AEM_ATTACHUNDO.
                                          //Compatibility: define same as ES_SUNKEN.
#define ES_HEAPSERIALIZE      0x00008000  //Mutual exclusion will be used when the heap functions allocate and free memory from window heap. Serialization of heap access allows two or more threads to simultaneously allocate and free memory from the same heap.
                                          //Compatibility: define same as ES_SAVESEL.

//Strings
#define AES_WORDDELIMITERSW     L" \t\n'`\"\\|[](){}<>,.;:+-=~!@#$%^&*/?\0\0"
#define AES_WRAPDELIMITERSW     L" \t\0\0"
#define AES_URLLEFTDELIMITERSW  L" \t\n'`\"(<{[=\0\0"
#define AES_URLRIGHTDELIMITERSW L" \t\n'`\")>}]\0\0"
#define AES_URLPREFIXESW        L"http:\0https:\0www.\0ftp:\0file:\0mailto:\0\0"

//AEM_SETEVENTMASK flags
#define AENM_SCROLL             0x00000001  //Sends AEN_HSCROLL and AEN_VSCROLL notifications.
#define AENM_PAINT              0x00000002  //Sends AEN_PAINT notifications.
#define AENM_MAXTEXT            0x00000010  //Don't use it. For internal code only.
#define AENM_PROGRESS           0x00000020  //Sends AEN_PROGRESS notifications.
#define AENM_MODIFY             0x00000040  //Sends AEN_MODIFY notifications.
#define AENM_SELCHANGE          0x00000080  //Sends AEN_SELCHANGING and AEN_SELCHANGED notifications.
#define AENM_TEXTCHANGE         0x00000100  //Sends AEN_TEXTCHANGING and AEN_TEXTCHANGED notifications.
#define AENM_TEXTINSERT         0x00000200  //Sends AEN_TEXTINSERTBEGIN and AEN_TEXTINSERTEND notifications.
#define AENM_TEXTDELETE         0x00000400  //Sends AEN_TEXTDELETEBEGIN and AEN_TEXTDELETEEND notifications.
#define AENM_POINT              0x00000800  //Sends AEN_POINT notifications.
#define AENM_DROPFILES          0x00010000  //Sends AEN_DROPFILES notifications.
#define AENM_DRAGDROP           0x00020000  //Sends AEN_DROPSOURCE and AEN_DROPTARGET notifications.
#define AENM_LINK               0x00040000  //Sends AEN_LINK notifications.
#define AENM_MARKER             0x00080000  //Sends AEN_MARKER notifications.

//AEN_SELCHANGING and AEN_SELCHANGED types
#define AESCT_REPLACESEL        0x00000001  //Replacing selection.
#define AESCT_APPENDTEXT        0x00000002  //Appending text.
#define AESCT_SETTEXT           0x00000004  //Setting text.
#define AESCT_STREAMIN          0x00000008  //Stream in.
#define AESCT_WRAP              0x00000010  //Sending AEM_SETWORDWRAP.
#define AESCT_UNDO              0x00000020  //Undo.
#define AESCT_REDO              0x00000040  //Redo.
#define AESCT_CUT               0x00000080  //Cut.
#define AESCT_CHAR              0x00000100  //Inserting char.
#define AESCT_KEYRETURN         0x00000200  //Pressing VK_RETURN.
#define AESCT_KEYBACKSPACE      0x00000400  //Pressing VK_BACK.
#define AESCT_KEYDELETE         0x00000800  //Pressing VK_DELETE.
#define AESCT_DRAGDELETE        0x00001000  //Dragging text delete.
#define AESCT_DROPINSERT        0x00002000  //Dropping text insert.

#define AESCT_MOUSESINGLECLK    0x00100000  //Mouse single click.
#define AESCT_MOUSEDOUBLECLK    0x00200000  //Mouse double click.
#define AESCT_MOUSETRIPLECLK    0x00400000  //Mouse triple click.
#define AESCT_MOUSECAPTURE      0x00800000  //Mouse made non-empty selection.
#define AESCT_MOUSELEFTMARGIN   0x01000000  //Left margin mouse action.
#define AESCT_KEYBOARD          0x02000000  //Selection is changed by keyboard.
#define AESCT_SETSELMESSAGE     0x04000000  //Sending AEM_EXSETSEL, AEM_SETSEL, EM_SETSEL, EM_EXSETSEL.
#define AESCT_UPDATESELECTION   0x08000000  //Selection is updated.
#define AESCT_IME               0x10000000  //Input Method Editors (IME).

//AEN_TEXTCHANGING, AEN_TEXTINSERTBEGIN, AEN_TEXTINSERTEND, AEN_TEXTDELETEBEGIN, AEN_TEXTDELETEEND and AEN_TEXTCHANGED types
#define AETCT_REPLACESEL        0x00000001  //Replacing selection.
#define AETCT_APPENDTEXT        0x00000002  //Appending text.
#define AETCT_SETTEXT           0x00000004  //Setting text.
#define AETCT_STREAMIN          0x00000008  //Stream in.
#define AETCT_WRAP              0x00000010  //Sending AEM_SETWORDWRAP.
#define AETCT_UNDO              0x00000020  //Undo.
#define AETCT_REDO              0x00000040  //Redo.
#define AETCT_CUT               0x00000080  //Cut.
#define AETCT_CHAR              0x00000100  //Inserting char.
#define AETCT_KEYRETURN         0x00000200  //Pressing VK_RETURN.
#define AETCT_KEYBACKSPACE      0x00000400  //Pressing VK_BACK.
#define AETCT_KEYDELETE         0x00000800  //Pressing VK_DELETE.
#define AETCT_DRAGDELETE        0x00001000  //Dragging text delete.
#define AETCT_DROPINSERT        0x00002000  //Dropping text insert.
#define AETCT_COLUMNGROUP       0x00004000  //Undo/Redo column text is grouped from one-line actions.

//AEN_TEXTCHANGED types
#define AETCT_NONE              0x00100000  //No text is changed.
#define AETCT_DELETEALL         0x00200000  //Indicate that due to AETCT_* action all text has been modified.

//Insert text flags
#define AEINST_LOCKUNDO          0x00000001
#define AEINST_LOCKSCROLL        0x00000002
#define AEINST_LOCKUPDATEHSCROLL 0x00000004
#define AEINST_LOCKUPDATEVSCROLL 0x00000008
#define AEINST_LOCKUPDATECARET   0x00000010
#define AEINST_LOCKUPDATETEXT    0x00000020
#define AEINST_LOCKUPDATEALL    (AEINST_LOCKUPDATEHSCROLL |\
                                 AEINST_LOCKUPDATEVSCROLL |\
                                 AEINST_LOCKUPDATECARET   |\
                                 AEINST_LOCKUPDATETEXT)

//Delete text flags
#define AEDELT_LOCKUNDO          0x00000001
#define AEDELT_LOCKSCROLL        0x00000002
#define AEDELT_LOCKUPDATEHSCROLL 0x00000004
#define AEDELT_LOCKUPDATEVSCROLL 0x00000008
#define AEDELT_LOCKUPDATECARET   0x00000010
#define AEDELT_LOCKUPDATETEXT    0x00000020
#define AEDELT_SAVECOLUMNSEL     0x00000040
#define AEDELT_LOCKUPDATEALL    (AEDELT_LOCKUPDATEHSCROLL |\
                                 AEDELT_LOCKUPDATEVSCROLL |\
                                 AEDELT_LOCKUPDATECARET   |\
                                 AEDELT_LOCKUPDATETEXT)

//AEN_POINT types
#define AEPTT_SETTEXT           0x00000001  //All document text has been changed. All points reset to first character.
#define AEPTT_STREAMIN          0x00000002  //All document text has been changed. All points reset to first character.
#define AEPTT_INSERT            0x00000004  //Insert operation.
#define AEPTT_DELETE            0x00000008  //Delete operation.

//AEPOINT flags
#define AEPTF_MODIFY        0x00000002  //If set, text in (AEPOINT.ciPoint + AEPOINT.nPointLen) area has been modified.
#define AEPTF_INSERT        0x00000004  //If set, AEPOINT.nPointLen index has been increased. Additional for AEPTF_MODIFY flag.
#define AEPTF_DELETE        0x00000008  //If set, AEPOINT.nPointLen index has been decreased. Additional for AEPTF_MODIFY flag.
#define AEPTF_NOTIFYDELETE  0x00000010  //Don't use it. For internal code only.
#define AEPTF_NOTIFYINSERT  0x00000020  //Don't use it. For internal code only.
#define AEPTF_VALIDLINE     0x00000040  //Don't use it. For internal code only.
#define AEPTF_WRAPMOVESET   0x00000080  //Don't use it. For internal code only.
#define AEPTF_FOLD          0x00000100  //If set, AEPOINT.ciPoint index is used in fold. AEPOINT.dwUserData is pointer to a AEFOLD structure.
#define AEPTF_WRAPMOVE      0x00000200  //If set, move point to the next line if it located at wrap place.
#define AEPTF_MOVEOFFSET    0x00001000  //If set, AEPOINT.nPointOffset has been changed.
#define AEPTF_MOVELINE      0x00002000  //If set, AEPOINT.ciPoint.nLine has been changed.

//AEPOINT character offset value
#define AEPTO_IGNORE    -1  //Character RichEdit offset is not used in AEPOINT.
#define AEPTO_CALC      -2  //Character RichEdit offset will calculated automatically by AEM_ADDPOINT.

//AEM_GETFOLDCOUNT types
#define AEFC_ALL              0  //All folds.
#define AEFC_COLLAPSED        1  //Collapsed folds.
#define AEFC_COLORED          2  //Colored folds.
#define AEFC_WITHID           3  //Folds with ID.
#define AEFC_WITHTHEME        4  //Folds with highlight theme.

//AEM_COLLAPSELINE and AEM_COLLAPSEFOLD flags
#define AECF_EXPAND           0x00000000  //Expand fold (default).
#define AECF_COLLAPSE         0x00000001  //Collapse fold.
#define AECF_RECURSE          0x00000002  //Recursive processing with all children.
#define AECF_NOUPDATE         0x00000004  //Don't update scroll and selection.
#define AECF_NOCARETCORRECT   0x00000008  //If in collapsed fold located caret, don't move it to fold start.

//AEN_DROPTARGET actions
#define AEDT_TARGETENTER        1  //Enter into the target window.
#define AEDT_TARGETOVER         2  //Moving over the target window.
#define AEDT_TARGETLEAVE        3  //Leaving the target window.
#define AEDT_TARGETDROP         4  //Drops the data into the target window.

//AEN_DROPSOURCE actions
#define AEDS_SOURCEBEGIN        1  //Begin selection dragging.
#define AEDS_SOURCEEND          2  //End selection dragging. Before selection deletion.
#define AEDS_SOURCEDONE         3  //End dragging operation. Same as EN_DRAGDROPDONE.

//AEN_PROGRESS type
#define AEPGS_SETTEXT           0x00000001  //Setting text.
#define AEPGS_WRAPTEXT          0x00000002  //Wrapping text.
#define AEPGS_STREAMIN          0x00000004  //Receiving stream text.

//AEN_PAINT type
#define AEPNT_BEGIN             0x00000001  //Sends before painting is started, only AENPAINT.hDC member is valid.
#define AEPNT_END               0x00000002  //Sends before clean-up paint resources.
#define AEPNT_DRAWLINE          0x00000004  //Sends before line is drawn.

//AEM_SETOPTIONS flags
                                                  // Window styles:
#define AECO_READONLY                 0x00000001  //Set read-only mode. You can use ES_READONLY window style.
#define AECO_DISABLENOSCROLL          0x00000002  //Disables scroll bars instead of hiding them when they are not needed. You can use ES_DISABLENOSCROLL window style.
#define AECO_NOHIDESEL                0x00000004  //If you do not specify this style, then hides the selection when the control loses the input focus and inverts the selection when the control receives the input focus. You can use ES_NOHIDESEL window style.
#define AECO_WANTRETURN               0x00000008  //If you do not specify this style, pressing the ENTER key has the same effect as pressing the dialog box's default push button. You can use ES_WANTRETURN window style.
#define AECO_MULTILINE                0x00000010  //Designates a multiline edit control. The default is single-line edit control. You can use ES_MULTILINE window style.
                                                  // Other:
#define AECO_DETAILEDUNDO             0x00000020  //The control stores any typing action, into a new action in the undo queue.
#define AECO_PASTESELECTCOLUMN        0x00000040  //Selects pasted text in column mode.
#define AECO_DISABLEDRAG              0x00000080  //Disables OLE text dragging.
#define AECO_DISABLEDROP              0x00000100  //Disables OLE text dropping.
#define AECO_CARETOUTEDGE             0x00000200  //Allow caret moving out of the line edge.
#define AECO_ACTIVECOLUMN             0x00000400  //Draw caret vertical line.
#define AECO_ACTIVELINE               0x00000800  //Draw active line.
#define AECO_ACTIVELINEBORDER         0x00001000  //Draw active line border.
#define AECO_ALTLINEBORDER            0x00002000  //Draw alternating lines border.
#define AECO_NONEWLINEDRAW            0x00004000  //Disables drawing new line selection as space character.
#define AECO_ENTIRENEWLINEDRAW        0x00008000  //Draw new line selection to the right edge.
#define AECO_NONEWLINEMOUSESELECT     0x00010000  //Triple click and left margin click selects only line contents without new line.
#define AECO_LBUTTONUPCONTINUECAPTURE 0x00020000  //After WM_LBUTTONUP message capture operations doesn't stopped.
#define AECO_RBUTTONDOWNMOVECARET     0x00040000  //WM_RBUTTONDOWN message moves caret to a click position.
#define AECO_MBUTTONDOWNNOSCROLL      0x00080000  //No scrolling after WM_MBUTTONDOWN message.
#define AECO_SELUNWRAPLINE            0x00100000  //Line selection with mouse selects all wrapped line.
#define AECO_NOMARGINSEL              0x00200000  //Disables left margin line selection with mouse.
#define AECO_NOMARKERMOVE             0x00400000  //Disables changing position of column marker with mouse and shift button.
#define AECO_NOMARKERAFTERLASTLINE    0x00800000  //Disables marker painting after last line.
#define AECO_VSCROLLBYLINE            0x01000000  //Unit of vertical scrolling is line (default is pixel).
#define AECO_NOSCROLLDELETEALL        0x02000000  //Turn off scrolling to caret, when undo/redo operations replace all text.
#define AECO_NOSCROLLSELECTALL        0x04000000  //Turn off scrolling to caret, when all text is selected.
#define AECO_NOCOLUMNPASTEHOTKEY      0x08000000  //Turn off Alt+V for columnar paste.
#define AECO_DISABLEBEEP              0x10000000  //Disables sound beep, when unallowable action occur.
#define AECO_NODCBUFFER               0x20000000  //Don't use device context output buffering in AE_Paint. Cause edit window flashing.
#define AECO_PAINTGROUP               0x40000000  //Paint text by group of characters (default is character by character).
                                                  //With this flag some text recognition programs could start to work, printer could print faster, but highlighted symbols and combined unicode symbols can be drawn differently and editing of whose characters may become uncomfortable.

//AEM_EXSETOPTIONS flags
#define AECOE_DETECTURL               0x00000001  //Enables detection and highlighting of URLs by an edit control.
#define AECOE_LOCKSELECTION           0x00000002  //Prevent selection changing. Use it with AECO_READONLY flag.
#define AECOE_OVERTYPE                0x00000004  //Turn on overtype mode instead of insert mode.
#define AECOE_ALTDECINPUT             0x00000008  //Do Alt+NumPad decimal input with NumLock on (default is decimal input after two "Num 0").
#define AECOE_INVERTHORZWHEEL         0x00000010  //Invert mouse horizontal wheel.
#define AECOE_INVERTVERTWHEEL         0x00000020  //Invert mouse vertical wheel.
#define AECOE_NOPRINTCOLLAPSED        0x00001000  //Disables print collapsed lines. See AEM_COLLAPSEFOLD message.

#define AECOOP_SET              1  //Sets the options to those specified by lParam.
#define AECOOP_OR               2  //Combines the specified options with the current options.
#define AECOOP_AND              3  //Retains only those current options that are also specified by lParam.
#define AECOOP_XOR              4  //Logically exclusive OR the current options with those specified by lParam.

//Modifier flags
#define AEMOD_ALT               0x1  //ALT key
#define AEMOD_SHIFT             0x2  //SHIFT key
#define AEMOD_CONTROL           0x4  //CTRL key

//AEM_GETLINENUMBER flags
#define AEGL_LINECOUNT                 0  //Total number of text lines. If the control has no text, the return value is 1.
#define AEGL_FIRSTSELLINE              1  //First line of the selection.
#define AEGL_LASTSELLINE               2  //Last line of the selection.
#define AEGL_CARETLINE                 3  //Caret line.
#define AEGL_FIRSTVISIBLELINE          4  //First visible line.
#define AEGL_LASTVISIBLELINE           5  //Last visible line.
#define AEGL_FIRSTFULLVISIBLELINE      6  //First fully visible line.
#define AEGL_LASTFULLVISIBLELINE       7  //Last fully visible line.
#define AEGL_LINEUNWRAPCOUNT          11  //Total number of unwrapped text lines. If the control has no text, the return value is 1.
#define AEGL_UNWRAPSELMULTILINE       12  //Returns value: TRUE - if selection on multiple lines. FALSE - if no selection or selection is on single line.
                                          //  Next flags require RichEdit offset in lParam.
#define AEGL_LINEFROMRICHOFFSET       20  //Line of the specified RichEdit offset. lParam is RichEdit offset (if -1 caret offset). Equivalent to EM_EXLINEFROMCHAR.
#define AEGL_UNWRAPLINEFROMRICHOFFSET 21  //Unwrapped line of the specified RichEdit offset. lParam is RichEdit offset (if -1 caret offset).

//AEM_GETINDEX and AEM_GETRICHOFFSET flags
#define AEGI_FIRSTCHAR                 1  //First character.
#define AEGI_LASTCHAR                  2  //Last character.
#define AEGI_FIRSTSELCHAR              3  //First character of the selection.
#define AEGI_LASTSELCHAR               4  //Last character of the selection.
#define AEGI_CARETCHAR                 5  //Caret character.
#define AEGI_FIRSTVISIBLECHAR          6  //First visible character, collapsed lines are skipped.
#define AEGI_LASTVISIBLECHAR           7  //Last visible character, collapsed lines are skipped.
#define AEGI_FIRSTFULLVISIBLECHAR      8  //First fully visible character, collapsed lines are skipped.
#define AEGI_LASTFULLVISIBLECHAR       9  //Last fully visible character, collapsed lines are skipped.
#define AEGI_FIRSTVISIBLELINE         10  //First character of the first visible line, collapsed lines are skipped.
#define AEGI_LASTVISIBLELINE          11  //Last character of the last visible line, collapsed lines are skipped.
#define AEGI_FIRSTFULLVISIBLELINE     12  //First character of the first fully visible line, collapsed lines are skipped.
#define AEGI_LASTFULLVISIBLELINE      13  //Last character of the last fully visible line, collapsed lines are skipped.
                                          //  Next flags require in lParam:
                                          //    AEM_GETINDEX - pointer to a AECHARINDEX structure.
                                          //    AEM_GETRICHOFFSET - RichEdit offset.
#define AEGI_VALIDCHARINLINE          15  //Correct character to make sure that it is on line. AEM_GETRICHOFFSET - not supported.
                                          //For better performance use AEC_ValidCharInLine instead.
#define AEGI_LINEBEGIN                16  //First character in line.
                                          //
#define AEGI_LINEEND                  17  //Last character in line.
                                          //
#define AEGI_WRAPLINEBEGIN            18  //First character of the unwrapped line. Returns number of characters as AEM_GETINDEX result.
                                          //For better performance use AEC_WrapLineBeginEx instead.
#define AEGI_WRAPLINEEND              19  //Last character of the unwrapped line. Returns number of characters as AEM_GETINDEX result.
                                          //For better performance use AEC_WrapLineEndEx instead.
#define AEGI_NEXTCHARINLINE           20  //Next character in line.
                                          //For better performance use AEC_NextCharInLineEx instead.
#define AEGI_PREVCHARINLINE           21  //Previous character in line.
                                          //For better performance use AEC_PrevCharInLineEx instead.
#define AEGI_NEXTCHAR                 22  //Next wide character.
                                          //For better performance use AEC_NextCharEx instead.
#define AEGI_PREVCHAR                 23  //Previous wide character.
                                          //For better performance use AEC_PrevCharEx instead.
#define AEGI_NEXTLINE                 24  //First character of the next line.
                                          //For better performance use AEC_NextLineEx instead.
#define AEGI_PREVLINE                 25  //First character of the previous line.
                                          //For better performance use AEC_PrevLineEx instead.
#define AEGI_NEXTUNCOLLAPSEDCHAR      26  //Next wide character, collapsed lines are skipped.
#define AEGI_PREVUNCOLLAPSEDCHAR      27  //Previous wide character, collapsed lines are skipped.
#define AEGI_NEXTUNCOLLAPSEDLINE      28  //First character of the next line, collapsed lines are skipped.
#define AEGI_PREVUNCOLLAPSEDLINE      29  //First character of the previous line, collapsed lines are skipped.
                                          //  Next flags require in lParam:
                                          //    AEM_GETINDEX - not supported.
                                          //    AEM_GETRICHOFFSET - line number.
#define AEGI_RICHOFFSETFROMLINE       40  //First character (RichEdit offset) of the specified line. lParam is line number (if -1 caret line). Equivalent to EM_LINEINDEX.
#define AEGI_RICHOFFSETFROMUNWRAPLINE 41  //First character (RichEdit offset) of the specified unwrapped line. lParam is line number (if -1 caret line).

//AEM_ISDELIMITER parameter
#define AEDLM_PREVCHAR  0x00000001  //Check previous char.
#define AEDLM_WORD      0x00000010  //Word delimiter.
#define AEDLM_WRAP      0x00000020  //Wrap delimiter.
#define AEDLM_URLLEFT   0x00000040  //URL left delimiter.
#define AEDLM_URLRIGHT  0x00000080  //URL right delimiter.

//AEM_COLUMNTOINDEX and AEM_INDEXTOCOLUMN flags
#define AECTI_WRAPLINEBEGIN 0x0001  //If set, scan from first character of the unwrapped line. If not set, scan from first character of the wrapped line.
#define AECTI_FIT           0x0002  //AEM_COLUMNTOINDEX: if set, character position must be equal or less than column boundary. If not set, character position must be equal or larger than column boundary.

//AEM_SETSEL and AEM_UPDATESEL flags
#define AESELT_COLUMNON            0x00000001  //Make column selection ON.
#define AESELT_COLUMNASIS          0x00000002  //Leave column selection as is.
#define AESELT_LOCKNOTIFY          0x00000004  //Disable AEN_SELCHANGING and AEN_SELCHANGED notifications.
#define AESELT_LOCKSCROLL          0x00000008  //Lock edit window scroll.
#define AESELT_LOCKUPDATE          0x00000010  //Lock edit window update.
#define AESELT_LOCKCARET           0x00000020  //Lock caret reposition.
#define AESELT_LOCKUNDOGROUPING    0x00000040  //Don't use it. For internal code only.
#define AESELT_NOCARETHORZINDENT   0x00000080  //Caret horizontal indent isn't changed.
#define AESELT_NOVERTSCROLLCORRECT 0x00000100  //On some conditions scroll can be increased to a height of one line.
#define AESELT_MOUSE               0x00000200  //Don't use it. For internal code only.
#define AESELT_RESETSELECTION      0x00000400  //Don't use it. For internal code only.
#define AESELT_INDEXUPDATE         0x00000800  //Update lpLine member of the AEM_SETSEL message structures, to avoid dangling of a pointer after text change.

//AEM_REPLACESEL flags
#define AEREPT_COLUMNON            0x00000001  //Make column selection ON.
#define AEREPT_COLUMNASIS          0x00000002  //Leave column selection as is.
#define AEREPT_LOCKSCROLL          0x00000004  //Lock edit window scroll. However edit window can be scrolled during window resize when AECO_DISABLENOSCROLL option not set.
#define AEREPT_UNDOGROUPING        0x00000100  //Continue undo grouping.
#define AEREPT_SELECT              0x00000200  //Select inserted text.

//AEM_CHARFROMPOS return value
#define AEPC_ERROR    0  //Error.
#define AEPC_EQUAL    1  //Char exactly in specified position.
#define AEPC_BEFORE   2  //Char before the specified position.
#define AEPC_AFTER    3  //Char after the specified position.

//New line value
#define AELB_ASINPUT  1  //Use input new line, see AEM_SETNEWLINE with AENL_INPUT.
#define AELB_ASOUTPUT 2  //Use output new line, see AEM_SETNEWLINE with AENL_OUTPUT.
#define AELB_ASIS     3  //Use new line of the source.
#define AELB_EOF      4  //End-of-file, last line in document.
#define AELB_R        5  //"\r" new line.
#define AELB_N        6  //"\n" new line.
#define AELB_RN       7  //"\r\n" new line.
#define AELB_RRN      8  //"\r\r\n" new line.
#define AELB_WRAP     9  //No new line, this line is wrapped.

//AEM_SETNEWLINE flags
#define AENL_INPUT           0x00000001  //Sets default new line for the input operations, like paste.
#define AENL_OUTPUT          0x00000002  //Sets default new line for the output operations, like cut,copy.

//AEM_CUT, AEM_COPY flags
#define AECFC_WORD           0x00000001  //Cut/Copy word under caret, if no selection.
#define AECFC_LINE           0x00000002  //Cut/Copy line under caret, if no selection.
#define AECFC_UNWRAPLINE     0x00000004  //Cut/Copy unwrapped line under caret, if no selection.
#define AECFC_NEWLINE        0x00000008  //Cut/Copy also new line. Uses with AECFC_LINE or AECFC_UNWRAPLINE.

//AEM_PASTE flags
#define AEPFC_ANSI           0x00000001  //Paste text as ANSI. Default is paste as Unicode text, if no Unicode text available ANSI text will be used.
#define AEPFC_COLUMN         0x00000002  //Paste to column selection.
#define AEPFC_SELECT         0x00000004  //Select pasted text.

//AEM_LOCKUPDATE FLAGS
#define AELU_SCROLLBAR  0x00000001
#define AELU_CARET      0x00000002

//AEM_SETDOCUMENT flags
#define AESWD_NOCHECKFOCUS        0x00000001  //Don't update focus state.
#define AESWD_NODRAGDROP          0x00000002  //Don't register drag-and-drop with a new IDropTarget.
#define AESWD_NOSHOWSCROLLBARS    0x00000004  //Don't update scrollbars visibility.
#define AESWD_NOUPDATESCROLLBARS  0x00000008  //Don't update scrollbars position.
#define AESWD_NOUPDATECARET       0x00000010  //Don't update caret.
#define AESWD_NOINVALIDATERECT    0x00000020  //Don't redraw edit window.

#define AESWD_NOREDRAW  (AESWD_NOUPDATESCROLLBARS |\
                         AESWD_NOUPDATECARET      |\
                         AESWD_NOINVALIDATERECT)
#define AESWD_NOALL     (AESWD_NOCHECKFOCUS     |\
                         AESWD_NODRAGDROP       |\
                         AESWD_NOSHOWSCROLLBARS |\
                         AESWD_NOREDRAW)

//AEM_DRAGDROP flags
#define AEDD_GETDRAGWINDOW   1  //Return dragging window handle.
#define AEDD_STOPDRAG        2  //Set stop dragging operation flag.

//AEM_URLVISIT operations
#define AEUV_STACK         1  //Retrieve URL visit stack.
                              //lParam                    == not used.
                              //(AESTACKURLVISIT *)Return == pointer to an URL visit stack.
#define AEUV_GETBYRANGE    2  //Retrieve URL visit item by characters range.
                              //(AECHARRANGE *)lParam     == URL range.
                              //(AEURLVISIT *)Return      == pointer to an URL visit item.
#define AEUV_GETBYTEXT     3  //Retrieve URL visit item by text.
                              //(const wchar_t *)lParam   == URL string.
                              //(AEURLVISIT *)Return      == pointer to an URL visit item.
#define AEUV_ADD           4  //Add URL visit item.
                              //(AECHARRANGE *)lParam     == URL range. Can be NULL.
                              //(AEURLVISIT *)Return      == pointer to an URL visit item.
#define AEUV_DEL           5  //Delete URL visit item.
                              //(AEURLVISIT *)lParam      == pointer to an URL visit item.
                              //Return                    == zero.
#define AEUV_FREE          6  //Free URL visit stack.
                              //lParam                    == not used.
                              //Return                    == zero.


//AEM_SETCOLORS flags
#define AECLR_DEFAULT          0x00000001  //Use default system colors for the specified flags, all members of the AECOLORS structure are ignored.
#define AECLR_CARET            0x00000002  //Sets caret color. crCaret member is valid.
#define AECLR_BASICTEXT        0x00000004  //Sets basic text color. crBasicText member is valid.
#define AECLR_BASICBK          0x00000008  //Sets basic background color. crBasicBk member is valid.
#define AECLR_SELTEXT          0x00000010  //Sets text in selection color. crSelText member is valid.
#define AECLR_SELBK            0x00000020  //Sets background in selection color. crSelBk member is valid.
#define AECLR_ACTIVELINETEXT   0x00000040  //Sets active line text color. crActiveLineText member is valid.
#define AECLR_ACTIVELINEBK     0x00000080  //Sets active line background color. crActiveLineBk member is valid.
#define AECLR_URLTEXT          0x00000100  //Sets hyperlink text color. crUrlText member is valid.
#define AECLR_ACTIVECOLUMN     0x00000200  //Sets active column color. crActiveColumn member is valid.
#define AECLR_COLUMNMARKER     0x00000400  //Sets column marker color. crColumnMarker member is valid.
#define AECLR_URLCURSORTEXT    0x00000800  //Sets active hyperlink text color. crUrlCursorText member is valid.
#define AECLR_URLVISITTEXT     0x00001000  //Sets visited hyperlink text color. crUrlVisitText member is valid.
#define AECLR_ACTIVELINEBORDER 0x00002000  //Sets active line border color. crActiveLineBorder member is valid.
#define AECLR_ALTLINETEXT      0x00004000  //Sets alternating line text color. crAltLineText member is valid.
#define AECLR_ALTLINEBK        0x00008000  //Sets alternating line background color. crAltLineBk member is valid.
#define AECLR_ALTLINEBORDER    0x00010000  //Sets alternating line border color. crAltLineBorder member is valid.

#define AECLR_ALL  (AECLR_CARET            |\
                    AECLR_BASICTEXT        |\
                    AECLR_BASICBK          |\
                    AECLR_SELTEXT          |\
                    AECLR_SELBK            |\
                    AECLR_ACTIVELINETEXT   |\
                    AECLR_ACTIVELINEBK     |\
                    AECLR_URLTEXT          |\
                    AECLR_ACTIVECOLUMN     |\
                    AECLR_COLUMNMARKER     |\
                    AECLR_URLCURSORTEXT    |\
                    AECLR_URLVISITTEXT     |\
                    AECLR_ACTIVELINEBORDER |\
                    AECLR_ALTLINETEXT      |\
                    AECLR_ALTLINEBK        |\
                    AECLR_ALTLINEBORDER)

//Print
#define AEPRN_TEST                      0x00001  //Calculate data without painting.
#define AEPRN_INHUNDREDTHSOFMILLIMETERS 0x00002  //Indicates that hundredths of millimeters are the unit of measurement for margins.
#define AEPRN_INTHOUSANDTHSOFINCHES     0x00004  //Indicates that thousandths of inches are the unit of measurement for margins.
#define AEPRN_WRAPNONE                  0x00008  //Print without wrapping.
#define AEPRN_WRAPWORD                  0x00010  //Print with word wrapping (default).
#define AEPRN_WRAPSYMBOL                0x00020  //Print with symbols wrapping.
#define AEPRN_IGNOREFORMFEED            0x00040  //Ignore form-feed character '\f'.
#define AEPRN_ANSI                      0x00080  //Ansi text output. Can solve draw problems on Win95/98/Me.
#define AEPRN_COLOREDTEXT               0x00100  //Print colored text.
#define AEPRN_COLOREDBACKGROUND         0x00200  //Print on colored background.
#define AEPRN_COLOREDSELECTION          0x00400  //Print text selection.
#define AEPRN_CALLEMPTY                 0x10000  //Don't use it. For internal code only.

//AEM_HLFINDTHEME type
#define AEHLFT_CURRENT 0  //Current theme handle.
                          //lParam == not used.
#define AEHLFT_BYNAMEA 1  //Find by ansi theme name.
                          //(char *)lParam == ansi theme name to retrieve. If NULL, active theme handle will be returned.
#define AEHLFT_BYNAMEW 2  //Find by unicode theme name.
                          //(wchar_t *)lParam == unicode theme name to retrieve. If NULL, active theme handle will be returned.
#define AEHLFT_BYFOLD  3  //Find theme at specified location (AEFINDFOLD.dwFindIt). If no folds found with own theme (AEFOLD.hRuleTheme), then current theme handle returns.
                          //(AEFINDFOLD *)lParam == pointer to a AEFINDFOLD structure.

//Highlight options
#define AEHLO_IGNOREFONTNORMAL       0x00000001  //Use AEHLS_NONE font style, if font style to change is AEHLS_FONTNORMAL.
#define AEHLO_IGNOREFONTBOLD         0x00000002  //Use AEHLS_FONTNORMAL font style, if font style to change is AEHLS_FONTBOLD.
                                                 //Use AEHLS_FONTITALIC font style, if font style to change is AEHLS_FONTBOLDITALIC.
#define AEHLO_IGNOREFONTITALIC       0x00000004  //Use AEHLS_FONTNORMAL font style, if font style to change is AEHLS_FONTITALIC.
                                                 //Use AEHLS_FONTBOLD font style, if font style to change is AEHLS_FONTBOLDITALIC.

//Highlight flags
#define AEHLF_MATCHCASE              0x00000001  //If set, the highlight operation is case-sensitive. If not set, the highlight operation is case-insensitive.
#define AEHLF_WHOLEWORD              0x00000002  //Only for AEMARKTEXTITEM.dwFlags. If set, the operation searches only for whole words that match the search string. If not set, the operation also searches for word fragments that match the search string.
#define AEHLF_WORDCOMPOSITION        0x00000002  //Only for AEWORDITEM.dwFlags. Word is a composition of characters. For example, AEWORDITEM.pWord equal to "1234567890" with this flag, means highlight words that contain only digits.
#define AEHLF_QUOTEEND_REQUIRED      0x00000004  //If quote end isn't found, text after quote start will not be highlighted.
#define AEHLF_QUOTESTART_ISDELIMITER 0x00000008  //Last meet delimiter used as quote start (AEQUOTEITEM.pQuoteStart member is ignored).
#define AEHLF_QUOTEEND_ISDELIMITER   0x00000010  //First meet delimiter used as quote end (AEQUOTEITEM.pQuoteEnd member is ignored).
#define AEHLF_QUOTESTART_NOHIGHLIGHT 0x00000020  //Don't highlight quote start string.
#define AEHLF_QUOTEEND_NOHIGHLIGHT   0x00000040  //Don't highlight quote end string.
#define AEHLF_QUOTESTART_NOCATCH     0x00000080  //Don't catch and don't highlight quote start string.
#define AEHLF_QUOTEEND_NOCATCH       0x00000100  //Don't catch and don't highlight quote end string.
#define AEHLF_ATLINESTART            0x00000200  //Quote start, delimiter or word located at line start.
#define AEHLF_ATLINEEND              0x00000400  //Quote end, delimiter or word located at line end.
#define AEHLF_QUOTESTART_ISWORD      0x00000800  //Quote start is surrounded with delimiters.
#define AEHLF_QUOTEEND_ISWORD        0x00001000  //Quote end is surrounded with delimiters.
#define AEHLF_QUOTEWITHOUTDELIMITERS 0x00002000  //Quote doesn't contain delimiters.
#define AEHLF_QUOTESTART_CATCHONLY   0x00004000  //Only quote start string is catched.
#define AEHLF_QUOTEEMPTY             0x00008000  //Quote doesn't contain any character.
#define AEHLF_QUOTEINCLUDE           0x00010000  //Quote include string is valid.
#define AEHLF_QUOTEEXCLUDE           0x00020000  //Quote exclude string is valid.
                                                 //Regular exression:
#define AEHLF_REGEXP                 0x10000000  //Can be used in AEQUOTEITEM.dwFlags.
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
#define AEHLF_STYLED                 0x80000000  //Don't use it. For internal code only.

//Highlight font style
#define AEHLS_NONE                   0  //Current style.
#define AEHLS_FONTNORMAL             1  //Normal style.
#define AEHLS_FONTBOLD               2  //Bold style.
#define AEHLS_FONTITALIC             3  //Italic style.
#define AEHLS_FONTBOLDITALIC         4  //Bold italic style.

//Highlight elements
#define AEHLE_DELIMITER              1  //Delimiter. New line, start of file and end of file are delimiters by default.
#define AEHLE_WORD                   2  //Word - string surrounded with delimiters.
#define AEHLE_QUOTE                  3  //Quote - text started with quote start string and ended with quote end string.
#define AEHLE_MARKTEXT               4  //Mark text - mark specified text.
#define AEHLE_MARKRANGE              5  //Mark range - mark specified range of characters.

//Highlight AEM_HLGETHIGHLIGHT flags
#define AEGHF_NOSELECTION            0x00000001  //Ignore text selection coloring.
#define AEGHF_NOACTIVELINE           0x00000002  //Ignore active line colors.
#define AEGHF_NOALTLINE              0x00000004  //Ignore alternating line colors.
#define AEGHF_CALLENDLINE            0x00010000  //Call function for end of non-empty line.
#define AEGHF_CALLENDRANGE           0x00020000  //Call function for end range.

//Highlight paint type
#define AEHPT_SELECTION              0x00000001
#define AEHPT_DELIM1                 0x00000002
#define AEHPT_WORD                   0x00000004
#define AEHPT_DELIM2                 0x00000008
#define AEHPT_QUOTE                  0x00000010
#define AEHPT_MARKTEXT               0x00000020
#define AEHPT_MARKRANGE              0x00000040
#define AEHPT_LINK                   0x00000080
#define AEHPT_FOLD                   0x00000100

//AEREGROUPCOLOR flags
#define AEREGCF_BACKREFCOLORTEXT  0x00000001  //AEREGROUPCOLOR.crText is backreference index for text color in format #RRGGBB or RRGGBB.
#define AEREGCF_BACKREFCOLORBK    0x00000002  //AEREGROUPCOLOR.crBk is backreference index for background color in format #RRGGBB or RRGGBB.

//Parent type
#define AEHAP_NONE   0
#define AEHAP_ROOT   1
#define AEHAP_QUOTE  2
#define AEHAP_FOLD   3

//Fold flags
#define AEFOLDF_COLLAPSED    0x00000001  //Fold is collapsed.
#define AEFOLDF_STYLED       0x80000000  //Don't use it. For internal code only.

//AEM_FINDFOLD flags
#define AEFF_FINDOFFSET      0x00000001  //AEFINDFOLD.dwFindIt is RichEdit offset.
#define AEFF_FINDINDEX       0x00000002  //AEFINDFOLD.dwFindIt is pointer to a AECHARINDEX structure.
#define AEFF_FINDLINE        0x00000004  //AEFINDFOLD.dwFindIt is zero based line number.
#define AEFF_FOLDSTART       0x00000008  //Fold is also accepted if AEFINDFOLD.dwFindIt points to fold start.
#define AEFF_FOLDEND         0x00000010  //Fold is also accepted if AEFINDFOLD.dwFindIt points to fold end.
#define AEFF_RECURSE         0x00000020  //Recursive search. Returned fold will be deepest possible.
#define AEFF_GETROOT         0x00000040  //Return root fold.
                                         //
                                         //The following groups of flags cannot be used together:
                                         //  AEFF_FINDOFFSET, AEFF_FINDINDEX and AEFF_FINDLINE.
                                         //  AEFF_RECURSE and AEFF_GETROOT.

//AEM_SCROLL, AEM_LINESCROLL flags
#define AESB_HORZ            0x00000001  //Horizontal scroll. Cannot be used with AESB_VERT.
#define AESB_VERT            0x00000002  //Vertical scroll. Cannot be used with AESB_HORZ.
#define AESB_RETURNUNITS     0x00000004  //If AESB_HORZ specified, number of characters scrolled returns. If AESB_VERT specified, number of lines scrolled returns.
#define AESB_ALIGNLEFT       0x00000008  //Align first visible char.
#define AESB_ALIGNRIGHT      0x00000010  //Align last visible char.
#define AESB_ALIGNTOP        0x00000020  //Align first visible line.
#define AESB_ALIGNBOTTOM     0x00000040  //Align last visible line.

//AEM_SCROLLTOPOINT flags
#define AESC_TEST            0x00000001  //Only test for scroll. Returns result, but not actually scroll.
#define AESC_POINTCARET      0x00000010  //Caret position is used and AESCROLLTOPOINT.ptPos is ignored.
#define AESC_POINTGLOBAL     0x00000020  //AESCROLLTOPOINT.ptPos is position in the virtual text space coordinates.
#define AESC_POINTCLIENT     0x00000040  //AESCROLLTOPOINT.ptPos is position in the client area coordinates (default).
#define AESC_POINTOUT        0x00000080  //AESCROLLTOPOINT.ptPos will contain new scroll position after AEM_SCROLLTOPOINT returns.
#define AESC_OFFSETPIXELX    0x00000100  //AESCROLLTOPOINT.nOffsetX specifies pixels number.
#define AESC_OFFSETPIXELY    0x00000200  //AESCROLLTOPOINT.nOffsetY specifies pixels number.
#define AESC_OFFSETCHARX     0x00000400  //AESCROLLTOPOINT.nOffsetX specifies characters number.
#define AESC_OFFSETCHARY     0x00000800  //AESCROLLTOPOINT.nOffsetY specifies characters number.
#define AESC_OFFSETRECTDIVX  0x00001000  //AESCROLLTOPOINT.nOffsetX specifies divisor of the edit rectangle width.
#define AESC_OFFSETRECTDIVY  0x00002000  //AESCROLLTOPOINT.nOffsetY specifies divisor of the edit rectangle width.
#define AESC_FORCELEFT       0x00010000  //Scrolls to the left even if AESCROLLTOPOINT.ptPos visible.
#define AESC_FORCETOP        0x00020000  //Scrolls to the top even if AESCROLLTOPOINT.ptPos visible.
#define AESC_FORCERIGHT      0x00040000  //Scrolls to the right even if AESCROLLTOPOINT.ptPos visible.
#define AESC_FORCEBOTTOM     0x00080000  //Scrolls to the bottom even if AESCROLLTOPOINT.ptPos visible.

//AEM_SCROLLTOPOINT return values
#define AECSE_SCROLLEDX      0x00000001  //Edit control was horizontally scrolled.
#define AECSE_SCROLLEDY      0x00000002  //Edit control was vertically scrolled.
#define AECSE_SCROLLEDLEFT   0x00000004  //Edit control was scrolled left horizontally.
#define AECSE_SCROLLEDRIGHT  0x00000008  //Edit control was scrolled right horizontally.
#define AECSE_SCROLLEDUP     0x00000010  //Edit control was scrolled up vertically.
#define AECSE_SCROLLEDDOWN   0x00000020  //Edit control was scrolled down vertically.

//AEM_GETFONT type
#define AEGF_CURRENT         0  //Current font handle.
#define AEGF_NORMAL          1  //Normal style font handle.
#define AEGF_BOLD            2  //Bold style font handle.
#define AEGF_ITALIC          3  //Italic style font handle.
#define AEGF_BOLDITALIC      4  //Bold italic style font handle.
#define AEGF_URL             5  //URL style font handle.

//AEM_GETCHARSIZE flags
#define AECS_HEIGHT          0  //Current font character height including line gap. lParam not used.
#define AECS_AVEWIDTH        1  //Current font latin character average width. lParam not used.
#define AECS_INDEXWIDTH      2  //lParam is character index, which width is retrieving.
#define AECS_POINTSIZE       3  //Current font point size. lParam not used.
#define AECS_SPACEWIDTH      4  //Current font space width. lParam not used.
#define AECS_TABWIDTH        5  //Current font tabulation width. lParam not used.
#define AECS_MAXWIDTH        6  //Current font latin character maximum width. lParam not used.
#define AECS_FIXEDCHARWIDTH  7  //Fixed character width that was set with AEM_FIXEDCHARWIDTH. lParam not used.
#define AECS_FIXEDTABWIDTH   8  //Fixed tabulation width that was set with AEM_FIXEDCHARWIDTH. lParam not used.

//AEM_CONVERTPOINT flags
#define AECPT_GLOBALTOCLIENT 0  //Convert position in the virtual text space of the document, to client area coordinates.
#define AECPT_CLIENTTOGLOBAL 1  //Convert position in the client area coordinates, to virtual text space of the document.

//Coordinate type
#define AECT_GLOBAL 0  //Position in the virtual text space coordinates.
#define AECT_CLIENT 1  //Position in the client area coordinates.

//Rectangle flags
#define AERC_UPDATE    0x01  //Redraw edit window. Only for AEM_SETRECT and AEM_SETERASERECT.
#define AERC_MARGINS   0x02  //Rectangle contain edit area margins instead of edit area coordinates.
#define AERC_NOLEFT    0x04  //Don't set/retrieve left side.
#define AERC_NOTOP     0x08  //Don't set/retrieve top side.
#define AERC_NORIGHT   0x10  //Don't set/retrieve right side.
#define AERC_NOBOTTOM  0x20  //Don't set/retrieve bottom side.

//AEM_POINTONMARGIN sides
#define AESIDE_LEFT          0x00000001
#define AESIDE_TOP           0x00000002
#define AESIDE_RIGHT         0x00000004
#define AESIDE_BOTTOM        0x00000008

//AEM_GETMOUSESTATE types
#define AEMS_CAPTURE         1  //Capture state.
#define AEMS_SELECTION       2  //Selection state.

//AEM_GETMOUSESTATE capture
#define AEMSC_MOUSEMOVE       0x1  //Text selection.
#define AEMSC_MOUSESCROLL     0x2  //Middle button scroll.
#define AEMSC_MOUSEDRAG       0x4  //Selection dragging.
#define AEMSC_MOUSEMARKER     0x8  //Marker moving.

//AEM_GETMOUSESTATE selection
#define AEMSS_LBUTTONUP      0x1  //WM_LBUTTONUP has been received.
#define AEMSS_CHARS          0x2  //Characters selection.
#define AEMSS_WORDS          0x4  //Words selection.
#define AEMSS_LINES          0x8  //Lines selection.

//AEM_FINDTEXT, AEM_ISMATCH flags
#define AEFR_DOWN               0x00000001  //Same as FR_DOWN. If set, the search is from the beginning to the end of the search range. If not set, the search is from the end to the beginning of the search range.
#define AEFR_WHOLEWORD          0x00000002  //Same as FR_WHOLEWORD. If set, the operation searches only for whole words that match the search string. If not set, the operation also searches for word fragments that match the search string.
#define AEFR_MATCHCASE          0x00000004  //Same as FR_MATCHCASE. If set, the search operation is case-sensitive. If not set, the search operation is case-insensitive.
#define AEFR_REGEXP             0x00080000  //Regular expression search.
#define AEFR_REGEXPNONEWLINEDOT 0x00100000  //Symbol . specifies any character except new line.
#define AEFR_REGEXPMINMATCH     0x00200000  //Allow zero length match at string edges. For example: "^" at the string beginning or "$" at the string ending.

//AEM_SETWORDWRAP flags
#define AEWW_NONE            0x00000000  //Turn off wrap.
#define AEWW_WORD            0x00000001  //Wrap by words.
#define AEWW_SYMBOL          0x00000002  //Wrap by symbols.
#define AEWW_LIMITPIXEL      0x00000100  //Limit in pixels (default).
#define AEWW_LIMITSYMBOL     0x00000200  //Limit in symbols.

//AEM_SETMARKER types
#define AEMT_PIXEL           0  //Pixel integer.
#define AEMT_SYMBOL          1  //Column number.

//AEM_SETWORDBREAK flags
#define AEWB_LEFTWORDSTART   0x00000001  //Left movement is stopped, when word start is found.
#define AEWB_LEFTWORDEND     0x00000002  //Left movement is stopped, when word end is found.
#define AEWB_RIGHTWORDSTART  0x00000004  //Right movement is stopped, when word start is found.
#define AEWB_RIGHTWORDEND    0x00000008  //Right movement is stopped, when word end is found.
#define AEWB_STOPSPACESTART  0x00000010  //Movement is stopped, when spacing start is found. Cannot be combined with AEWB_SKIPSPACESTART.
#define AEWB_STOPSPACEEND    0x00000020  //Movement is stopped, when spacing end is found. Cannot be combined with AEWB_SKIPSPACEEND.
#define AEWB_SKIPSPACESTART  0x00000040  //Movement is continued, when spacing start is found. Cannot be combined with AEWB_STOPSPACESTART.
#define AEWB_SKIPSPACEEND    0x00000080  //Movement is continued, when spacing end is found. Cannot be combined with AEWB_STOPSPACEEND.
#define AEWB_STOPNEWLINE     0x00000100  //Movement is stopped, when new line is found.
#define AEWB_MINMOVE         0x00001000  //Minimum movement or not move at all if flags matched.

//AEM_STREAMIN, AEM_STREAMOUT flags
#define AESF_SELECTION       0x00000001  //Stream-in (read) or stream-out (write) the current selection. If not specified, stream-in (read) or stream-out (write) the entire contents of the control.
#define AESF_FILLSPACES      0x00000002  //Stream-out (write) the current column selection with the filling empty spaces. Valid if bColumnSel member of a AESTREAMOUT structure is TRUE.

//AEM_ISRANGEMODIFIED return value
#define AEIRM_UNMODIFIED      1
#define AEIRM_MODIFIEDUNSAVED 2
#define AEIRM_MODIFIEDSAVED   3

//AEM_GETUNDOPOS flags
#define AEGUP_CURRENT         0x00
#define AEGUP_NEXT            0x01
#define AEGUP_PREV            0x02
#define AEGUP_FIRST           0x04 //Always return -1.
#define AEGUP_LAST            0x08 //Always return -1.
#define AEGUP_NOUNDO          0x10 //Return -1 if undo item located in undo.
#define AEGUP_NOREDO          0x20 //Return -1 if undo item located in redo.
#define AEGUP_SAVED           0x40 //Always return -1.

#ifndef FR_DOWN
  #define FR_DOWN 0x00000001
#endif
#ifndef FR_WHOLEWORD
  #define FR_WHOLEWORD 0x00000002
#endif
#ifndef FR_MATCHCASE
  #define FR_MATCHCASE 0x00000004
#endif
#ifndef EC_LEFTMARGIN
  #define EC_LEFTMARGIN 0x0001
#endif
#ifndef EC_RIGHTMARGIN
  #define EC_RIGHTMARGIN 0x0002
#endif
#ifndef GT_SELECTION
  #define GT_SELECTION 0x0002
#endif
#ifndef SPI_GETWHEELSCROLLLINES
  #define SPI_GETWHEELSCROLLLINES 0x0068
#endif
#ifndef WC_NO_BEST_FIT_CHARS
  #define WC_NO_BEST_FIT_CHARS 0x00000400
#endif
#ifndef WM_MOUSEWHEEL
  #define WM_MOUSEWHEEL 0x020A
#endif
#ifndef EN_DRAGDROPDONE
  #define EN_DRAGDROPDONE 0x070c
#endif
#ifndef EM_SHOWSCROLLBAR
  #define EM_SHOWSCROLLBAR (WM_USER + 96)
#endif
#ifndef EM_GETSCROLLPOS
  #define EM_GETSCROLLPOS (WM_USER + 221)
#endif
#ifndef EM_SETSCROLLPOS
  #define EM_SETSCROLLPOS (WM_USER + 222)
#endif
#ifndef EM_SETTEXTEX
  #define EM_SETTEXTEX (WM_USER + 97)

  #define ST_DEFAULT    0x0000
  #define ST_KEEPUNDO   0x0001
  #define ST_SELECTION  0x0002
  #define ST_NEWCHARS   0x0004

  typedef struct {
    DWORD flags;
    UINT codepage;
  } SETTEXTEX;
#endif

#ifndef MAKE_IDENTIFIER
  #define MAKE_IDENTIFIER(a, b, c, d)  ((DWORD)MAKELONG(MAKEWORD(a, b), MAKEWORD(c, d)))
#endif

#ifndef mod
  #define mod(a)  (((a) < 0) ? (0 - (a)) : (a))
#endif

DECLARE_HANDLE (AEHDOC);
DECLARE_HANDLE (AEHPRINT);
DECLARE_HANDLE (AEHTHEME);
DECLARE_HANDLE (AEHDELIMITER);
DECLARE_HANDLE (AEHWORD);
DECLARE_HANDLE (AEHQUOTE);
DECLARE_HANDLE (AEHMARKTEXT);
DECLARE_HANDLE (AEHMARKRANGE);

typedef LRESULT (CALLBACK *AEEditProc)(AEHDOC hDoc, UINT uMsg, WPARAM wParam, LPARAM lParam);
//hDoc        Document handle returned by AEM_GETDOCUMENT or AEM_CREATEDOCUMENT.
//uMsg        Message ID for example EM_SETSEL.
//lParam      Additional parameter.
//wParam      Additional parameter.
//
//Return Value
// Depends on message.


//// Structures for x64 RichEdit support

typedef struct {
  INT_PTR cpMin;
  INT_PTR cpMax;
} CHARRANGE64;

typedef struct {
  CHARRANGE64 chrg;
  LPSTR lpstrText;
} TEXTRANGE64A;

typedef struct {
  CHARRANGE64 chrg;
  LPWSTR lpstrText;
} TEXTRANGE64W;

typedef struct {
  CHARRANGE64 chrg;
  LPCSTR lpstrText;
  CHARRANGE64 chrgText;
} FINDTEXTEX64A;

typedef struct {
  CHARRANGE64 chrg;
  LPCWSTR lpstrText;
  CHARRANGE64 chrgText;
} FINDTEXTEX64W;

typedef struct {
  UINT_PTR cb;
  DWORD flags;
  UINT codepage;
  LPCSTR lpDefaultChar;
  LPBOOL lpUsedDefChar;
} GETTEXTEX64;

typedef struct {
  NMHDR nmhdr;
  CHARRANGE chrg;
  WORD seltyp;
  CHARRANGE64 chrg64;
} SELCHANGE64;

typedef struct {
  NMHDR nmhdr;
  HANDLE hDrop;
  LONG cp;
  BOOL fProtected;
  INT_PTR cp64;
} ENDROPFILES64;

typedef struct {
  NMHDR nmhdr;
  UINT msg;
  WPARAM wParam;
  LPARAM lParam;
  CHARRANGE chrg;
  CHARRANGE64 chrg64;
} ENLINK64;


//// Structures for x64 support

typedef struct {
  INT_PTR x;
  INT_PTR y;
} POINT64;

typedef struct {
  INT_PTR cx;
  INT_PTR cy;
} SIZE64;


//// Structures

#ifndef _HSTACK_STRUCT_
#define _HSTACK_STRUCT_
typedef struct {
  INT_PTR first;
  INT_PTR last;
} HSTACK;
#endif

#ifndef _STACK_STRUCT_
#define _STACK_STRUCT_
typedef struct _stack {
  struct _stack *next;
  struct _stack *prev;
} stack;
#endif

typedef struct _AELINEDATA {
  struct _AELINEDATA *next;   //Pointer to the next AELINEDATA structure.
  struct _AELINEDATA *prev;   //Pointer to the previous AELINEDATA structure.
  wchar_t *wpLine;            //Text of the line, terminated with NULL character.
  int nLineLen;               //Length of the wpLine, not including the terminating NULL character.
  BYTE nLineBreak;            //New line: AELB_EOF, AELB_R, AELB_N, AELB_RN, AELB_RRN or AELB_WRAP.
  BYTE nLineFlags;            //Reserved.
  WORD nReserved;             //Reserved.
  int nLineWidth;             //Width of the line in pixels.
  int nSelStart;              //Selection start character position in line.
  int nSelEnd;                //Selection end character position in line.
} AELINEDATA;

typedef struct _AELINEINDEX {
  int nLine;                  //Zero based line number in document.
  AELINEDATA *lpLine;         //Pointer to the AELINEDATA structure.
} AELINEINDEX;

typedef struct _AECHARINDEX {
  int nLine;                  //Zero based line number in document.
  AELINEDATA *lpLine;         //Pointer to the AELINEDATA structure.
  int nCharInLine;            //Character position in line.
} AECHARINDEX;

typedef struct {
  AECHARINDEX ciMin;  //First character index in range.
  AECHARINDEX ciMax;  //Last character index in range.
} AECHARRANGE;

typedef struct {
  AECHARRANGE crSel;  //Characters range.
  DWORD dwFlags;      //See AESELT_* defines.
  DWORD dwType;       //See AESCT_* defines.
} AESELECTION;

typedef struct _AEPOINT {
  struct _AEPOINT *next;   //Pointer to the next AEPOINT structure.
  struct _AEPOINT *prev;   //Pointer to the previous AEPOINT structure.
  AECHARINDEX ciPoint;     //Read-only character index.
  INT_PTR nPointOffset;    //Character RichEdit offset or one of the AEPTO_* define.
  int nPointLen;           //Point length.
  DWORD dwFlags;           //See AEPTF_* defines.
  UINT_PTR dwUserData;     //User data.
  INT_PTR nTmpPointOffset; //Don't use it. For internal code only.
  int nTmpPointLen;        //Don't use it. For internal code only.
} AEPOINT;

typedef struct _AEFOLD {
  struct _AEFOLD *next;       //Pointer to the next AEFOLD structure.
  struct _AEFOLD *prev;       //Pointer to the previous AEFOLD structure.
  struct _AEFOLD *parent;     //Pointer to the parent AEFOLD structure.
  struct _AEFOLD *firstChild; //Pointer to the first child AEFOLD structure.
  struct _AEFOLD *lastChild;  //Pointer to the last child AEFOLD structure.
  AEPOINT *lpMinPoint;        //Minimum line point.
  AEPOINT *lpMaxPoint;        //Maximum line point.
  DWORD dwFlags;              //See AEFOLDF_* defines.
  DWORD dwFontStyle;          //See AEHLS_* defines.
  COLORREF crText;            //Text color. If -1, then don't set.
  COLORREF crBk;              //Background color. If -1, then don't set.
  int nParentID;              //Parent rule identifier.
  int nRuleID;                //Rule identifier.
  AEHTHEME hRuleTheme;        //Rule highlight theme.
  UINT_PTR dwUserData;        //User data.
} AEFOLD;

typedef struct {
  DWORD dwFlags;         //[in]  See AEFF_* defines.
  UINT_PTR dwFindIt;     //[in]  Depend on AEFF_FIND* define.
  AEFOLD *lpParent;      //[out] Parent fold.
  AEFOLD *lpPrevSubling; //[out] Previous subling fold.
} AEFINDFOLD;

typedef struct {
  const char *pText;     //[in] Text to append.
  UINT_PTR dwTextLen;    //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;          //[in] See AELB_* defines.
  int nCodePage;         //[in] Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
} AESETTEXTA;

typedef struct {
  const wchar_t *pText;  //[in] Text to append.
  UINT_PTR dwTextLen;    //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;          //[in] See AELB_* defines.
} AESETTEXTW;

typedef struct {
  const char *pText;     //[in] Text to append.
  UINT_PTR dwTextLen;    //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;          //[in] See AELB_* defines.
  int nCodePage;         //[in] Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
} AEAPPENDTEXTA;

typedef struct {
  const wchar_t *pText;  //[in] Text to append.
  UINT_PTR dwTextLen;    //[in] Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;          //[in] See AELB_* defines.
} AEAPPENDTEXTW;

typedef struct {
  const char *pText;           //[in]  Text to replace with.
  UINT_PTR dwTextLen;          //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;                //[in]  See AELB_* defines.
  DWORD dwFlags;               //[in]  See AEREPT_* defines.
  AECHARINDEX *ciInsertStart;  //[out] Insert "from" character index after replacement.
  AECHARINDEX *ciInsertEnd;    //[out] Insert "to" character index after replacement.
  int nCodePage;               //[in]  Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
} AEREPLACESELA;

typedef struct {
  const wchar_t *pText;        //[in]  Text to replace with.
  UINT_PTR dwTextLen;          //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;                //[in]  See AELB_* defines.
  DWORD dwFlags;               //[in]  See AEREPT_* defines.
  AECHARINDEX *ciInsertStart;  //[out] Insert "from" character index after replacement.
  AECHARINDEX *ciInsertEnd;    //[out] Insert "to" character index after replacement.
} AEREPLACESELW;

typedef struct {
  AECHARRANGE cr;             //[in]  Characters range to retrieve.
  BOOL bColumnSel;            //[in]  Column selection. If this value is -1, use current selection type.
  char *pBuffer;              //[out] Pointer to buffer that receives the text. If this value is NULL, the function returns the required buffer size in characters.
  UINT_PTR dwBufferMax;       //[in]  Specifies the maximum number of characters to copy to the buffer, including the NULL character.
  int nNewLine;               //[in]  See AELB_* defines.
  int nCodePage;              //[in]  Code page identifier (any available in the system). You can also specify one of the following values: CP_ACP - ANSI code page, CP_OEMCP - OEM code page, CP_UTF8 - UTF-8 code page.
  const char *lpDefaultChar;  //[in]  Points to the character used if a wide character cannot be represented in the specified code page. If this member is NULL, a system default value is used.
  BOOL *lpUsedDefChar;        //[in]  Points to a flag that indicates whether a default character was used. The flag is set to TRUE if one or more wide characters in the source string cannot be represented in the specified code page. Otherwise, the flag is set to FALSE. This member may be NULL.
  BOOL bFillSpaces;           //[in]  If bColumnSel is TRUE, fill empties with spaces.
} AETEXTRANGEA;

typedef struct {
  AECHARRANGE cr;             //[in]  Characters range to retrieve.
  BOOL bColumnSel;            //[in]  Column selection. If this value is -1, use current selection type. If bColumnSel is TRUE, then selection must be exist, otherwise it is not necessary.
  wchar_t *pBuffer;           //[out] Pointer to buffer that receives the text. If this value is NULL, the function returns the required buffer size in characters.
  UINT_PTR dwBufferMax;       //[in]  Specifies the maximum number of characters to copy to the buffer, including the NULL character.
  int nNewLine;               //[in]  See AELB_* defines.
  int nCodePage;              //[in]  Ignored. Code page is always 1200 (UTF-16 LE).
  const char *lpDefaultChar;  //[in]  Ignored.
  BOOL *lpUsedDefChar;        //[in]  Ignored.
  BOOL bFillSpaces;           //[in]  If bColumnSel is TRUE, fill empties with spaces.
} AETEXTRANGEW;

typedef DWORD (CALLBACK *AEStreamCallback)(UINT_PTR dwCookie, wchar_t *wszBuf, DWORD dwBufBytesSize, DWORD *dwBufBytesDone);
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

typedef struct {
  UINT_PTR dwCookie;            //[in]  Specifies an application-defined value that the edit control passes to the AEStreamCallback function specified by the lpCallback member.
  DWORD dwError;                //[out] Indicates the results of the stream-in (read) operation.
  AEStreamCallback lpCallback;  //[in]  Pointer to an AEStreamCallback function, which is an application-defined function that the control calls to transfer data. The control calls the callback function repeatedly, transferring a portion of the data with each call.
  int nNewLine;                 //[in]  See AELB_* defines.
  UINT_PTR dwTextLen;           //[in]  Text length. Need if using AEN_PROGRESS notification.
  int nFirstNewLine;            //[out] Indicates first new line. See AELB_* defines.
} AESTREAMIN;

typedef struct {
  UINT_PTR dwCookie;            //[in]  Specifies an application-defined value that the edit control passes to the AEStreamCallback function specified by the lpCallback member.
  DWORD dwError;                //[out] Indicates the result of the stream-out (write) operation.
  AEStreamCallback lpCallback;  //[in]  Pointer to an AEStreamCallback function, which is an application-defined function that the control calls to transfer data. The control calls the callback function repeatedly, transferring a portion of the data with each call.
  int nNewLine;                 //[in]  See AELB_* defines.
  BOOL bColumnSel;              //[in]  Column selection. If this value is -1, use current selection type.
} AESTREAMOUT;

typedef struct {
  DWORD dwFlags;                //[in]  See AEFR_* defines.
  const char *pText;            //[in]  Text to find.
  UINT_PTR dwTextLen;           //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;                 //[in]  See AELB_* defines.
  AECHARRANGE crSearch;         //[in]  Range of characters to search.
  AECHARRANGE crFound;          //[out] Range of characters in which text is found.
  INT_PTR nCompileErrorOffset;  //[out] Contain pattern offset, if error occurred during compile pattern. Return when AEFR_REGEXP is set.
} AEFINDTEXTA;

typedef struct {
  DWORD dwFlags;                //[in]  See AEFR_* defines.
  const wchar_t *pText;         //[in]  Text to find.
  UINT_PTR dwTextLen;           //[in]  Text length. If this value is -1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;                 //[in]  See AELB_* defines.
  AECHARRANGE crSearch;         //[in]  Range of characters to search.
  AECHARRANGE crFound;          //[out] Range of characters in which text is found.
  INT_PTR nCompileErrorOffset;  //[out] Contain pattern offset, if error occurred during compile pattern. Return when AEFR_REGEXP is set.
} AEFINDTEXTW;

typedef struct {
  DWORD dwFlags;                  //[in] See AECLR_* defines.
  COLORREF crCaret;               //[in] Caret color.
  COLORREF crBasicText;           //[in] Basic text color.
  COLORREF crBasicBk;             //[in] Basic background color.
  COLORREF crSelText;             //[in] Text color in selection.
  COLORREF crSelBk;               //[in] Background color in selection.
  COLORREF crActiveLineText;      //[in] Text color in active line.
  COLORREF crActiveLineBk;        //[in] Background color in active line.
  COLORREF crUrlText;             //[in] Hyperlink text color.
  COLORREF crActiveColumn;        //[in] Active column color.
  COLORREF crColumnMarker;        //[in] Column marker color.
  COLORREF crUrlCursorText;       //[in] Active hyperlink text color.
  COLORREF crUrlVisitText;        //[in] Visited hyperlink text color.
  COLORREF crActiveLineBorder;    //[in] Border color in active line.
  COLORREF crAltLineText;         //[in] Text color in alternating line.
  COLORREF crAltLineBk;           //[in] Background color in alternating line.
  COLORREF crAltLineBorder;       //[in] Border color in alternating line.
} AECOLORS;

typedef struct {
  DWORD dwFlags;           //[in] See AEGHF_* defines.
  DWORD dwFontStyle;       //[Out] See AEHLS_* defines.
  COLORREF crText;         //[Out] Text color in line.
  COLORREF crBk;           //[Out] Background color in line.
  COLORREF crBorderTop;    //[Out] Top border color of the line.
  COLORREF crBorderBottom; //[Out] Bottom border color of the line.
} AECHARCOLORS;

typedef struct {
  AECHARINDEX *ciCharIn;   //[in]  Input character index.
  AECHARINDEX *ciCharOut;  //[out] Output character index (result).
  INT_PTR nOffset;         //[in]  Offset can be positive or negative. For example, +1 will return next character, -1 will return previous character.
  int nNewLine;            //[in]  See AELB_* defines.
} AEINDEXOFFSET;

typedef struct {
  AECHARINDEX *ciChar1;   //[in] First character index. If NULL, first character is used.
  AECHARINDEX *ciChar2;   //[in] Second character index. If NULL, last character is used.
  BOOL bColumnSel;        //[in] Column selection. If this value is -1, use current selection type.
  int nNewLine;           //[in] See AELB_* defines.
} AEINDEXSUBTRACT;

typedef struct {
  DWORD dwFlags;    //[in]     See AESC_* defines.
  POINT64 ptPos;    //[in,out] Point to scroll to, ignored if AESC_POINTCARET flag specified.
                    //         If AESC_POINTOUT flag specified, then after AEM_SCROLLTOPOINT returns ptPos will contain new scroll position, otherwise unchanged.
  int nOffsetX;     //[in]     Horizontal scroll offset.
  int nOffsetY;     //[in]     Vertical scroll offset.
} AESCROLLTOPOINT;

typedef struct {
  DWORD dwFlags;    //See AESC_OFFSET* defines.
  DWORD dwSelFlags; //See AESELT_* defines. Can be zero.
  DWORD dwSelType;  //See AESCT_* defines.
  int nOffsetX;     //Minimal number of characters to horizontal window edge.
  int nOffsetY;     //Minimal number of lines to vertical window edge.
} AESCROLLCARETOPTIONS;

typedef struct {
  AEHDOC hDoc;     //Document handle. See AEM_CREATEDOCUMENT message.
  UINT uMsg;       //Window message.
  WPARAM wParam;   //Window first additional parameter.
  LPARAM lParam;   //Window second additional parameter.
  LRESULT lResult; //Result after window message returns.
} AESENDMESSAGE;

typedef struct _AEURLVISIT {
  struct _AEURLVISIT *next;
  struct _AEURLVISIT *prev;
  wchar_t *pUrlText;    //URL string.
  INT_PTR nUrlTextLen;  //URL string length.
  int nVisitCount;      //Count of visits.
  BOOL bStatic;         //TRUE  Control don't delete item with zero visits.
                        //FALSE Control could automatically delete this item with zero visits.
} AEURLVISIT;

typedef struct {
  AEURLVISIT *first;
  AEURLVISIT *last;
} AESTACKURLVISIT;

typedef struct {
  DWORD dwFlags;          //[in]     See AEPRN_* defines.
  HDC hPrinterDC;         //[in]     Printer device context.
  HFONT hEditFont;        //[in]     Edit font.
  HFONT hPrintFont;       //[out]    Print font (mapped edit font).
  int nCharHeight;        //[out]    Print character height.
  int nAveCharWidth;      //[out]    Print character average width.
  int nSpaceCharWidth;    //[out]    Print space width.
  int nTabWidth;          //[out]    Print tabulation width.
  RECT rcMargins;         //[in]     Specifies the widths of the left, top, right, and bottom margins. The AEPRN_INHUNDREDTHSOFMILLIMETERS or AEPRN_INTHOUSANDTHSOFINCHES flag indicates the units of measurement.
  RECT rcPageFull;        //[out]    Complete page rectangle. Filled by AEM_STARTPRINTDOC message.
  RECT rcPageIn;          //[in,out] Available page rectangle (minus margins). Filled by AEM_STARTPRINTDOC message and can be modified by user before AEM_PRINTPAGE call.
  RECT rcPageOut;         //[out]    Filled page rectangle. Filled by AEM_PRINTPAGE message.
  AECHARRANGE crText;     //[in,out] Text range to print. Filled by user and changed after AEM_PRINTPAGE message.
} AEPRINT;

typedef struct _AEDELIMITEMA {
  struct _AEDELIMITEMA *next;
  struct _AEDELIMITEMA *prev;
  int nIndex;                //Position of the element if positive inserts to begin of stack if negative to end.
  const char *pDelimiter;    //Delimiter string.
  int nDelimiterLen;         //Delimiter string length.
  DWORD dwFlags;             //See AEHLF_* defines.
  DWORD dwFontStyle;         //See AEHLS_* defines.
  COLORREF crText;           //Delimiter text color. If -1, then don't set.
  COLORREF crBk;             //Delimiter background color. If -1, then don't set.
  int nParentID;             //Parent rule identifier.
} AEDELIMITEMA;

typedef struct _AEDELIMITEMW {
  struct _AEDELIMITEMW *next;
  struct _AEDELIMITEMW *prev;
  int nIndex;                //Position of the element if positive inserts to begin of stack if negative to end.
  const wchar_t *pDelimiter; //Delimiter string.
  int nDelimiterLen;         //Delimiter string length.
  DWORD dwFlags;             //See AEHLF_* defines.
  DWORD dwFontStyle;         //See AEHLS_* defines.
  COLORREF crText;           //Delimiter text color. If -1, then don't set.
  COLORREF crBk;             //Delimiter background color. If -1, then don't set.
  int nParentID;             //Parent rule identifier.
} AEDELIMITEMW;

typedef struct _AEWORDITEMA {
  struct _AEWORDITEMA *next;
  struct _AEWORDITEMA *prev;
  int nIndex;                //Reserved. Word items are automatically sorted.
  const char *pWord;         //Word string.
  int nWordLen;              //Word string length.
  DWORD dwFlags;             //See AEHLF_* defines.
  DWORD dwFontStyle;         //See AEHLS_* defines.
  COLORREF crText;           //Word text color. If -1, then don't set.
  COLORREF crBk;             //Word background color. If -1, then don't set.
  int nParentID;             //Parent rule identifier.
} AEWORDITEMA;

typedef struct _AEWORDITEMW {
  struct _AEWORDITEMW *next;
  struct _AEWORDITEMW *prev;
  int nIndex;                //Reserved. Word items are automatically sorted.
  const wchar_t *pWord;      //Word string.
  int nWordLen;              //Word string length.
  DWORD dwFlags;             //See AEHLF_* defines.
  DWORD dwFontStyle;         //See AEHLS_* defines.
  COLORREF crText;           //Word text color. If -1, then don't set.
  COLORREF crBk;             //Word background color. If -1, then don't set.
  int nParentID;             //Parent rule identifier.
} AEWORDITEMW;

typedef struct _AEQUOTEITEMA {
  struct _AEQUOTEITEMA *next;
  struct _AEQUOTEITEMA *prev;
  int nIndex;                    //Reserved. Quote start items are automatically grouped in standalone stack, if following members are equal: pQuoteStart, chEscape and dwFlags with AEHLF_QUOTESTART_ISDELIMITER, AEHLF_ATLINESTART, AEHLF_QUOTESTART_ISWORD.
  const char *pQuoteStart;       //Quote start string.
  int nQuoteStartLen;            //Quote start string length.
  const char *pQuoteEnd;         //Quote end string. If NULL, line end used as quote end.
  int nQuoteEndLen;              //Quote end string length.
  char chEscape;                 //Escape character. If it precedes quote string then quote ignored.
  const char *pQuoteInclude;     //Quote include string.
  int nQuoteIncludeLen;          //Quote include string length.
  const char *pQuoteExclude;     //Quote exclude string.
  int nQuoteExcludeLen;          //Quote exclude string length.
  DWORD dwFlags;                 //See AEHLF_* defines.
  DWORD dwFontStyle;             //See AEHLS_* defines.
  COLORREF crText;               //Quote text color. If -1, then don't set.
  COLORREF crBk;                 //Quote background color. If -1, then don't set.
  void *lpQuoteStart;            //Don't use it. For internal code only.
  int nParentID;                 //Parent rule identifier.
  int nRuleID;                   //Rule identifier.
  INT_PTR nCompileErrorOffset;   //Contain pQuoteStart offset, if error occurred during compile regular exression pattern.
} AEQUOTEITEMA;

typedef struct _AEQUOTEITEMW {
  struct _AEQUOTEITEMW *next;
  struct _AEQUOTEITEMW *prev;
  int nIndex;                    //Reserved. Quote start items are automatically grouped in standalone stack, if following members are equal: pQuoteStart, chEscape and dwFlags with AEHLF_QUOTESTART_ISDELIMITER, AEHLF_ATLINESTART, AEHLF_QUOTESTART_ISWORD.
  const wchar_t *pQuoteStart;    //Quote start string.
  int nQuoteStartLen;            //Quote start string length.
  const wchar_t *pQuoteEnd;      //Quote end string. If NULL, line end used as quote end.
  int nQuoteEndLen;              //Quote end string length.
  wchar_t chEscape;              //Escape character. If it precedes quote string then quote ignored.
  const wchar_t *pQuoteInclude;  //Quote include string.
  int nQuoteIncludeLen;          //Quote include string length.
  const wchar_t *pQuoteExclude;  //Quote exclude string.
  int nQuoteExcludeLen;          //Quote exclude string length.
  DWORD dwFlags;                 //See AEHLF_* defines.
  DWORD dwFontStyle;             //See AEHLS_* defines.
  COLORREF crText;               //Quote text color. If -1, then don't set.
  COLORREF crBk;                 //Quote background color. If -1, then don't set.
  int nParentID;                 //Parent rule identifier.
  int nRuleID;                   //Rule identifier.
  void *lpQuoteStart;            //Don't use it. For internal code only.
  union {
    void *lpREGroupStack;        //Don't use it. For internal code only.
    INT_PTR nCompileErrorOffset; //Contain pQuoteStart offset, if error occurred during compile regular exression pattern.
  };
} AEQUOTEITEMW;

typedef struct {
  DWORD dwFlags;                //See AEREGCF_* defines.
  DWORD dwFontStyle;            //See AEHLS_* defines.
  COLORREF crText;              //Quote text color. If -1, then don't set.
  COLORREF crBk;                //Quote background color. If -1, then don't set.
} AEREGROUPCOLOR;

typedef struct _AEMARKTEXTITEMA {
  struct _AEMARKTEXTITEMA *next;
  struct _AEMARKTEXTITEMA *prev;
  int nIndex;                  //Position of the element if positive inserts to begin of stack if negative to end.
  const char *pMarkText;       //Mark text.
  int nMarkTextLen;            //Mark text length.
  DWORD dwFlags;               //See AEHLF_* defines.
  DWORD dwFontStyle;           //See AEHLS_* defines.
  COLORREF crText;             //Mark text color. If -1, then don't set.
  COLORREF crBk;               //Mark background color. If -1, then don't set.
  INT_PTR nCompileErrorOffset; //Contain pMarkText offset, if error occurred during compile regular exression pattern.
} AEMARKTEXTITEMA;

typedef struct _AEMARKTEXTITEMW {
  struct _AEMARKTEXTITEMW *next;
  struct _AEMARKTEXTITEMW *prev;
  int nIndex;                    //Position of the element if positive inserts to begin of stack if negative to end.
  const wchar_t *pMarkText;      //Mark text.
  int nMarkTextLen;              //Mark text length.
  DWORD dwFlags;                 //See AEHLF_* defines.
  DWORD dwFontStyle;             //See AEHLS_* defines.
  COLORREF crText;               //Mark text color. If -1, then don't set.
  COLORREF crBk;                 //Mark background color. If -1, then don't set.
  union {
    void *lpREGroupStack;        //Don't use it. For internal code only.
    INT_PTR nCompileErrorOffset; //Contain pMarkText offset, if error occurred during compile regular exression pattern.
  };
} AEMARKTEXTITEMW;

typedef struct _AEMARKRANGEITEM {
  struct _AEMARKRANGEITEM *next;
  struct _AEMARKRANGEITEM *prev;
  int nIndex;              //Position of the element if positive inserts to begin of stack if negative to end.
  CHARRANGE64 crMarkRange; //cpMin member is the first character in the range (RichEdit offset), cpMax member is the last character in the range (RichEdit offset).
  DWORD dwFlags;           //Reserved.
  DWORD dwFontStyle;       //See AEHLS_* defines.
  COLORREF crText;         //Mark text color. If -1, then don't set.
  COLORREF crBk;           //Mark background color. If -1, then don't set.
} AEMARKRANGEITEM;

typedef struct {
  AEMARKTEXTITEMW *lpMarkText;
  AECHARRANGE crMarkText;
} AEMARKTEXTMATCH;

typedef struct {
  AEMARKRANGEITEM *lpMarkRange;
  CHARRANGE64 crMarkRange;
} AEMARKRANGEMATCH;

typedef struct _AEQUOTEMATCHITEM {
  struct _AEQUOTEMATCHITEM *next;
  struct _AEQUOTEMATCHITEM *prev;
  AEQUOTEITEMW *lpQuote;
  AECHARRANGE crQuoteStart;
  AECHARRANGE crQuoteEnd;
  INT_PTR nQuoteLen;
  DWORD dwFontStyle;   //See AEHLS_* defines.
  DWORD dwActiveText;  //Text color. If -1, then don't set.
  DWORD dwActiveBk;    //Background color. If -1, then don't set.
} AEQUOTEMATCHITEM;

typedef struct {
  AEQUOTEMATCHITEM *first;
  AEQUOTEMATCHITEM *last;
} AESTACKQUOTEMATCH;

typedef struct {
  AEQUOTEITEMW *lpQuote;
  AECHARRANGE crQuoteStart;
  AECHARRANGE crQuoteEnd;
  INT_PTR nQuoteLen;
  AECHARINDEX ciFindFirst;
  AECHARINDEX ciChildScan;
  AESTACKQUOTEMATCH hParentStack;
  int nParentType;     //See AEHAP_* defines.
  DWORD dwFontStyle;   //See AEHLS_* defines.
  DWORD dwActiveText;  //Text color. If -1, then don't set.
  DWORD dwActiveBk;    //Background color. If -1, then don't set.
} AEQUOTEMATCH;

typedef struct {
  AEDELIMITEMW *lpDelim1;
  AECHARRANGE crDelim1;
  AEWORDITEMW *lpWord;
  AECHARRANGE crWord;
  AEDELIMITEMW *lpDelim2;
  AECHARRANGE crDelim2;
} AEWORDMATCH;

typedef struct _AEACTIVETHEMEITEM {
  struct _AEACTIVETHEMEITEM *next;
  struct _AEACTIVETHEMEITEM *prev;
  AEHTHEME hActiveTheme;
} AEACTIVETHEMEITEM;

typedef struct {
  AEACTIVETHEMEITEM *first;
  AEACTIVETHEMEITEM *last;
} AESTACKACTIVETHEME;

typedef struct {
  AEFOLD *lpFold;
  CHARRANGE64 crFoldStart;
  CHARRANGE64 crFoldEnd;
  INT_PTR nFoldStartMax;
  AECHARINDEX ciFoldStartMax;
  AEHDOC hDoc;
  AEHTHEME hActiveThemeBegin;
  AESTACKACTIVETHEME hParentStack;
} AEFOLDMATCH;

typedef struct {
  DWORD cb;              //Size of the structure.
  DWORD dwDefaultText;   //Text color without highlighting.
  DWORD dwDefaultBk;     //Background color without highlighting.
  DWORD dwActiveText;    //Text color with highlighting.
  DWORD dwActiveBk;      //Background color with highlighting.
  DWORD dwFontStyle;     //See AEHLS_* defines.
  DWORD dwPaintType;     //See AEHPT_* defines.
  DWORD dwFindFirst;     //Don't use it. For internal code only.

  AEWORDMATCH wm;        //Word or/and delimiters items are found, if AEWORDITEMW.lpDelim1 or AEWORDITEMW.lpWord or AEWORDITEMW.lpDelim2 aren't NULL.
  AEQUOTEMATCH qm;       //Quote item is found, if AEQUOTEMATCH.lpQuote isn't NULL.
  AEMARKRANGEMATCH mrm;  //Mark range item is found, if AEMARKRANGEMATCH.lpMarkRange isn't NULL.
  AEMARKTEXTMATCH mtm;   //Mark text item is found, if AEMARKTEXTMATCH.lpMarkText isn't NULL.
  AEFOLDMATCH fm;        //Fold item is found, if AEFOLDMATCH.lpFold isn't NULL.
  AECHARRANGE crLink;    //URL item is found, if AECHARRANGE.ciMin.lpLine and AECHARRANGE.ciMax.lpLine aren't NULL.
} AEHLPAINT;

typedef DWORD (CALLBACK *AEGetHighLightCallback)(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp);
//dwCookie        Value of the dwCookie member of the AEGETHIGHLIGHT structure. The application specifies this value when it sends the AEM_HLGETHIGHLIGHT message.
//crAkelRange     Range of highlighted characters.
//crRichRange     Range of highlighted characters (RichEdit offset).
//hlp             Highlighted information.
//
//Return Value
// To continue processing, the callback function must return zero; to stop processing, it must return nonzero.

typedef struct {
  UINT_PTR dwCookie;                 //[in]  Specifies an application-defined value that the edit control passes to the AEGetHighLightCallback function specified by the lpCallback member.
  DWORD dwError;                     //[out] Indicates the result of the callback function.
  AEGetHighLightCallback lpCallback; //[in]  Pointer to an AEGetHighLightCallback function, which is an application-defined function that the control calls to pass highlighting information.
  AECHARRANGE crText;                //[in]  Range of characters to process.
  DWORD dwFlags;                     //[in]  See AEGHF_* defines.
} AEGETHIGHLIGHT;

typedef struct {
  //Standard NMHDR
  HWND hwndFrom;
  UINT_PTR idFrom;
  UINT code;

  //AkelEdit members
  AEHDOC docFrom;      //Document handle. See AEM_CREATEDOCUMENT message.
} AENMHDR;

typedef struct {
  AENMHDR hdr;
  SIZE_T dwBytes;      //Number of bytes that cannot be allocated.
} AENERRSPACE;

typedef struct {
  AENMHDR hdr;
  HWND hWndChange;     //AEN_SETFOCUS - handle to the window that has lost the keyboard focus.
                       //AEN_KILLFOCUS - handle to the window that receives the keyboard focus.
} AENFOCUS;

typedef struct {
  AENMHDR hdr;
  INT_PTR nPosNew;     //Current scroll position.
  INT_PTR nPosOld;     //Previous scroll position.
  INT_PTR nPosMax;     //Maximum scroll position.
} AENSCROLL;

typedef struct {
  AENMHDR hdr;
  RECT rcDraw;         //Draw rectangle.
  RECT rcEdit;         //Edit client rectangle.
} AENSETRECT;

typedef struct {
  AENMHDR hdr;
  DWORD dwType;           //See AEPNT_* defines.
  HDC hDC;                //Device context.
  AECHARINDEX ciMinDraw;  //First index in line to paint.
  AECHARINDEX ciMaxDraw;  //Last index in line to paint.
  INT_PTR nMinDrawOffset; //First character in line to paint (RichEdit offset).
  INT_PTR nMaxDrawOffset; //Last character in line to paint (RichEdit offset).
  POINT ptMinDraw;        //Left upper corner in client coordinates of first character in line to paint.
  POINT ptMaxDraw;        //Left upper corner in client coordinates of last character in line to paint.
} AENPAINT;

typedef struct {
  AENMHDR hdr;
  UINT_PTR dwTextLimit;   //Current text limit.
} AENMAXTEXT;

typedef struct {
  AENMHDR hdr;
  DWORD dwType;        //See AEPGS_* defines.
  DWORD dwTimeElapsed; //Elapsed time since action was start.
  INT_PTR nCurrent;    //Characters processed. Equal to zero, if first message.
  INT_PTR nMaximum;    //Total number of characters. Equal to nCurrent member, if last message.
} AENPROGRESS;

typedef struct {
  AENMHDR hdr;
  BOOL bModified;      //TRUE document state is set to modified, FALSE document state is set to unmodified.
} AENMODIFY;

typedef struct {
  AENMHDR hdr;
  AECHARRANGE crSel;     //Current selection.
  AECHARINDEX ciCaret;   //Caret character index position.
  DWORD dwType;          //See AESCT_* defines.
  BOOL bColumnSel;       //Column selection.
  CHARRANGE64 crRichSel; //Current selection (RichEdit offset).
} AENSELCHANGE;

typedef struct {
  AENMHDR hdr;
  AECHARRANGE crSel;     //Current selection.
  AECHARINDEX ciCaret;   //Caret character index position.
  DWORD dwType;          //See AETCT_* defines.
  BOOL bColumnSel;       //Column selection.
  CHARRANGE64 crRichSel; //Current selection (RichEdit offset).
} AENTEXTCHANGE;

typedef struct {
  AENMHDR hdr;
  AECHARRANGE crSel;       //Reserved.
  AECHARINDEX ciCaret;     //Reserved.
  DWORD dwType;            //See AETCT_* defines.
  const wchar_t *wpText;   //Text to insert.
  UINT_PTR dwTextLen;      //Text length.
  int nNewLine;            //See AELB_* defines.
  BOOL bColumnSel;         //Column selection.
  DWORD dwInsertFlags;     //See AEINST_* defines.
  AECHARRANGE crAkelRange; //AEN_TEXTINSERTBEGIN - text insert position or AEN_TEXTINSERTEND - text range after insertion.
  CHARRANGE64 crRichRange; //AEN_TEXTINSERTBEGIN - text insert position or AEN_TEXTINSERTEND - text range after insertion (RichEdit offset).
} AENTEXTINSERT;

typedef struct {
  AENMHDR hdr;
  AECHARRANGE crSel;       //Reserved.
  AECHARINDEX ciCaret;     //Reserved.
  DWORD dwType;            //See AETCT_* defines.
  BOOL bColumnSel;         //Column selection.
  DWORD dwDeleteFlags;     //See AEDELT_* defines.
  AECHARRANGE crAkelRange; //AEN_TEXTDELETEBEGIN - text delete range or AEN_TEXTDELETEEND - text range after deletion.
  CHARRANGE64 crRichRange; //AEN_TEXTDELETEBEGIN - text delete range or AEN_TEXTDELETEEND - text range after deletion (RichEdit offset).
} AENTEXTDELETE;

typedef struct {
  AENMHDR hdr;
  DWORD dwType;        //See AEPTT_* defines.
  AEPOINT *lpPoint;    //Pointer to a AEPOINT structure. NULL if type is AEPTT_SETTEXT or AEPTT_STREAMIN.
} AENPOINT;

typedef struct {
  AENMHDR hdr;
  HDROP hDrop;         //Handle to the dropped files list (same as with WM_DROPFILES).
  AECHARINDEX ciChar;  //Character index at which the dropped files would be inserted.
} AENDROPFILES;

typedef struct {
  AENMHDR hdr;
  int nAction;         //See AEDS_* defines.
  DWORD dwEffect;      //Cursor effect: DROPEFFECT_COPY, DROPEFFECT_MOVE or DROPEFFECT_NONE.
  DWORD dwDropResult;  //Drop result. Valid if nAction equal to AEDS_SOURCEEND or AEDS_SOURCEDONE.
} AENDROPSOURCE;

typedef struct {
  AENMHDR hdr;
  int nAction;         //See AEDT_* defines.
  POINT pt;            //Cursor position in screen coordinates.
  DWORD dwEffect;      //Cursor effect: DROPEFFECT_COPY, DROPEFFECT_MOVE or DROPEFFECT_NONE.
} AENDROPTARGET;

typedef struct {
  AENMHDR hdr;
  UINT uMsg;           //Mouse message: WM_LBUTTONDBLCLK, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE, WM_RBUTTONDBLCLK, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_SETCURSOR.
  WPARAM wParam;       //First parameter of a message.
  LPARAM lParam;       //Second parameter of a message.
  AECHARRANGE crLink;  //Range of characters which contain URL text.
  int nVisitCount;     //URL visit count. Variable must be incremented, if URL is opened.
} AENLINK;

typedef struct {
  AENMHDR hdr;
  DWORD dwType; //Column marker type.
  DWORD dwPos;  //Column marker position.
  BOOL bMouse;  //Column marker position is changed with mouse.
} AENMARKER;


//// AkelEdit messages

//Error notifications
#define AEN_ERRSPACE              (WM_USER + 1001)  //0x7E9

//Window notifications
#define AEN_SETFOCUS              (WM_USER + 1026)  //0x802
#define AEN_KILLFOCUS             (WM_USER + 1027)  //0x803
#define AEN_HSCROLL               (WM_USER + 1028)  //0x804
#define AEN_VSCROLL               (WM_USER + 1029)  //0x805
#define AEN_SETRECT               (WM_USER + 1030)  //0x806
#define AEN_PAINT                 (WM_USER + 1031)  //0x807

//Text notifications
#define AEN_MAXTEXT               (WM_USER + 1050)  //0x81A
#define AEN_PROGRESS              (WM_USER + 1051)  //0x81B
#define AEN_MODIFY                (WM_USER + 1052)  //0x81C
#define AEN_SELCHANGING           (WM_USER + 1053)  //0x81D
#define AEN_SELCHANGED            (WM_USER + 1054)  //0x81E
#define AEN_TEXTCHANGING          (WM_USER + 1055)  //0x81F
#define AEN_TEXTINSERTBEGIN       (WM_USER + 1056)  //0x820
#define AEN_TEXTINSERTEND         (WM_USER + 1057)  //0x821
#define AEN_TEXTDELETEBEGIN       (WM_USER + 1058)  //0x822
#define AEN_TEXTDELETEEND         (WM_USER + 1059)  //0x823
#define AEN_TEXTCHANGED           (WM_USER + 1060)  //0x824
#define AEN_POINT                 (WM_USER + 1061)  //0x825

//Mouse notifications
#define AEN_DROPFILES             (WM_USER + 1076)  //0x834
#define AEN_DROPSOURCE            (WM_USER + 1077)  //0x835
#define AEN_DROPTARGET            (WM_USER + 1078)  //0x836
#define AEN_LINK                  (WM_USER + 1079)  //0x837
#define AEN_MARKER                (WM_USER + 1080)  //0x838

//RichEdit Unicode extension
#define EM_REPLACESELA            (WM_USER + 1901)
#define EM_REPLACESELW            (WM_USER + 1902)
#define EM_GETSELTEXTA            (WM_USER + 1905)
#define EM_GETSELTEXTW            (WM_USER + 1906)
#define EM_GETLINEA               (WM_USER + 1907)
#define EM_GETLINEW               (WM_USER + 1908)

//RichEdit x64 extension
#define EM_GETSEL64               (WM_USER + 1951)
#define EM_EXGETSEL64             (WM_USER + 1952)
#define EM_EXSETSEL64             (WM_USER + 1953)
#define EM_FINDTEXTEX64           (WM_USER + 1954)
#define EM_FINDTEXTEX64A          (WM_USER + 1955)
#define EM_FINDTEXTEX64W          (WM_USER + 1956)
#define EM_GETTEXTRANGE64         (WM_USER + 1961)
#define EM_GETTEXTRANGE64A        (WM_USER + 1962)
#define EM_GETTEXTRANGE64W        (WM_USER + 1963)
#define EM_GETTEXTEX64            (WM_USER + 1964)

//Text retrieval and modification
#define AEM_EXSETTEXTA            (WM_USER + 1999)
#define AEM_EXSETTEXTW            (WM_USER + 2000)
#define AEM_SETTEXTA              (WM_USER + 2001)
#define AEM_SETTEXTW              (WM_USER + 2002)
#define AEM_APPENDTEXTA           (WM_USER + 2003)
#define AEM_APPENDTEXTW           (WM_USER + 2004)
#define AEM_REPLACESELA           (WM_USER + 2005)
#define AEM_REPLACESELW           (WM_USER + 2006)
#define AEM_GETTEXTRANGEA         (WM_USER + 2007)
#define AEM_GETTEXTRANGEW         (WM_USER + 2008)
#define AEM_STREAMIN              (WM_USER + 2009)
#define AEM_STREAMOUT             (WM_USER + 2010)
#define AEM_CANPASTE              (WM_USER + 2011)
#define AEM_PASTE                 (WM_USER + 2012)
#define AEM_CUT                   (WM_USER + 2013)
#define AEM_COPY                  (WM_USER + 2014)
#define AEM_CHECKCODEPAGE         (WM_USER + 2015)
#define AEM_FINDTEXTA             (WM_USER + 2016)
#define AEM_FINDTEXTW             (WM_USER + 2017)
#define AEM_ISMATCHA              (WM_USER + 2018)
#define AEM_ISMATCHW              (WM_USER + 2019)
#define AEM_KEYDOWN               (WM_USER + 2020)
#define AEM_INSERTCHAR            (WM_USER + 2021)
#define AEM_CHARAT                (WM_USER + 2022)
#define AEM_INPUTLANGUAGE         (WM_USER + 2023)
#define AEM_DRAGDROP              (WM_USER + 2024)

//Undo and Redo
#define AEM_CANUNDO               (WM_USER + 2051)
#define AEM_CANREDO               (WM_USER + 2052)
#define AEM_UNDO                  (WM_USER + 2053)
#define AEM_REDO                  (WM_USER + 2054)
#define AEM_EMPTYUNDOBUFFER       (WM_USER + 2055)
#define AEM_STOPGROUPTYPING       (WM_USER + 2056)
#define AEM_BEGINUNDOACTION       (WM_USER + 2057)
#define AEM_ENDUNDOACTION         (WM_USER + 2058)
#define AEM_LOCKCOLLECTUNDO       (WM_USER + 2059)
#define AEM_GETUNDOLIMIT          (WM_USER + 2060)
#define AEM_SETUNDOLIMIT          (WM_USER + 2061)
#define AEM_GETMODIFY             (WM_USER + 2062)
#define AEM_SETMODIFY             (WM_USER + 2063)
#define AEM_UNDOBUFFERSIZE        (WM_USER + 2064)
#define AEM_ISRANGEMODIFIED       (WM_USER + 2065)
#define AEM_DETACHUNDO            (WM_USER + 2066)
#define AEM_ATTACHUNDO            (WM_USER + 2067)
#define AEM_GETUNDOPOS            (WM_USER + 2068)

//Text coordinates
#define AEM_EXGETSEL              (WM_USER + 2099)
#define AEM_EXSETSEL              (WM_USER + 2100)
#define AEM_GETSEL                (WM_USER + 2101)
#define AEM_SETSEL                (WM_USER + 2102)
#define AEM_GETCOLUMNSEL          (WM_USER + 2103)
#define AEM_UPDATESEL             (WM_USER + 2104)
#define AEM_GETLINENUMBER         (WM_USER + 2105)
#define AEM_GETINDEX              (WM_USER + 2106)
#define AEM_GETLINEINDEX          (WM_USER + 2107)
#define AEM_INDEXUPDATE           (WM_USER + 2108)
#define AEM_INDEXCOMPARE          (WM_USER + 2109)
#define AEM_INDEXSUBTRACT         (WM_USER + 2110)
#define AEM_INDEXOFFSET           (WM_USER + 2111)
#define AEM_INDEXTORICHOFFSET     (WM_USER + 2112)
#define AEM_RICHOFFSETTOINDEX     (WM_USER + 2113)
#define AEM_GETRICHOFFSET         (WM_USER + 2114)
#define AEM_GETWRAPLINE           (WM_USER + 2118)
#define AEM_GETUNWRAPLINE         (WM_USER + 2119)
#define AEM_GETNEXTBREAK          (WM_USER + 2120)
#define AEM_GETPREVBREAK          (WM_USER + 2121)
#define AEM_ISDELIMITER           (WM_USER + 2122)
#define AEM_INDEXTOCOLUMN         (WM_USER + 2123)
#define AEM_COLUMNTOINDEX         (WM_USER + 2124)
#define AEM_INDEXINURL            (WM_USER + 2125)
#define AEM_URLVISIT              (WM_USER + 2126)
#define AEM_ADDPOINT              (WM_USER + 2141)
#define AEM_DELPOINT              (WM_USER + 2142)
#define AEM_GETPOINTSTACK         (WM_USER + 2143)

//Screen coordinates
#define AEM_CHARFROMGLOBALPOS     (WM_USER + 2149)
#define AEM_GLOBALPOSFROMCHAR     (WM_USER + 2150)
#define AEM_CHARFROMPOS           (WM_USER + 2151)
#define AEM_POSFROMCHAR           (WM_USER + 2152)
#define AEM_GETRECT               (WM_USER + 2153)
#define AEM_SETRECT               (WM_USER + 2154)
#define AEM_GETSCROLLPOS          (WM_USER + 2155)
#define AEM_SETSCROLLPOS          (WM_USER + 2156)
#define AEM_SCROLL                (WM_USER + 2157)
#define AEM_LINESCROLL            (WM_USER + 2158)
#define AEM_SCROLLTOPOINT         (WM_USER + 2159)
#define AEM_LOCKSCROLL            (WM_USER + 2161)
#define AEM_GETCHARSIZE           (WM_USER + 2164)
#define AEM_GETSTRWIDTH           (WM_USER + 2165)
#define AEM_GETCARETPOS           (WM_USER + 2166)
#define AEM_GETCARETHORZINDENT    (WM_USER + 2167)
#define AEM_SETCARETHORZINDENT    (WM_USER + 2168)
#define AEM_CONVERTPOINT          (WM_USER + 2169)
#define AEM_POINTONMARGIN         (WM_USER + 2170)
#define AEM_POINTONSELECTION      (WM_USER + 2171)
#define AEM_POINTONURL            (WM_USER + 2172)
#define AEM_LINEFROMVPOS          (WM_USER + 2173)
#define AEM_VPOSFROMLINE          (WM_USER + 2174)
#define AEM_GETMOUSESTATE         (WM_USER + 2175)

//Options
#define AEM_CONTROLCLASS          (WM_USER + 2199)
#define AEM_CONTROLVERSION        (WM_USER + 2200)
#define AEM_GETEVENTMASK          (WM_USER + 2201)
#define AEM_SETEVENTMASK          (WM_USER + 2202)
#define AEM_GETOPTIONS            (WM_USER + 2203)
#define AEM_SETOPTIONS            (WM_USER + 2204)
#define AEM_GETNEWLINE            (WM_USER + 2205)
#define AEM_SETNEWLINE            (WM_USER + 2206)
#define AEM_GETCOLORS             (WM_USER + 2207)
#define AEM_SETCOLORS             (WM_USER + 2208)
#define AEM_EXGETOPTIONS          (WM_USER + 2209)
#define AEM_EXSETOPTIONS          (WM_USER + 2210)
#define AEM_GETCARETWIDTH         (WM_USER + 2213)
#define AEM_SETCARETWIDTH         (WM_USER + 2214)
#define AEM_GETTABSTOP            (WM_USER + 2215)
#define AEM_SETTABSTOP            (WM_USER + 2216)
#define AEM_GETWORDWRAP           (WM_USER + 2217)
#define AEM_SETWORDWRAP           (WM_USER + 2218)
#define AEM_GETWORDDELIMITERS     (WM_USER + 2219)
#define AEM_SETWORDDELIMITERS     (WM_USER + 2220)
#define AEM_GETWRAPDELIMITERS     (WM_USER + 2221)
#define AEM_SETWRAPDELIMITERS     (WM_USER + 2222)
#define AEM_GETURLLEFTDELIMITERS  (WM_USER + 2223)
#define AEM_SETURLLEFTDELIMITERS  (WM_USER + 2224)
#define AEM_GETURLRIGHTDELIMITERS (WM_USER + 2225)
#define AEM_SETURLRIGHTDELIMITERS (WM_USER + 2226)
#define AEM_GETURLPREFIXES        (WM_USER + 2227)
#define AEM_SETURLPREFIXES        (WM_USER + 2228)
#define AEM_GETURLMAXLENGTH       (WM_USER + 2229)
#define AEM_SETURLMAXLENGTH       (WM_USER + 2230)
#define AEM_GETWORDBREAK          (WM_USER + 2231)
#define AEM_SETWORDBREAK          (WM_USER + 2232)
#define AEM_GETMARKER             (WM_USER + 2233)
#define AEM_SETMARKER             (WM_USER + 2234)
#define AEM_GETLINEGAP            (WM_USER + 2235)
#define AEM_SETLINEGAP            (WM_USER + 2236)
#define AEM_GETTEXTLIMIT          (WM_USER + 2237)
#define AEM_SETTEXTLIMIT          (WM_USER + 2238)
#define AEM_GETFONT               (WM_USER + 2239)
#define AEM_GETALTLINE            (WM_USER + 2240)
#define AEM_SETALTLINE            (WM_USER + 2241)
#define AEM_GETCHARCOLORS         (WM_USER + 2242)
#define AEM_SCROLLCARETOPTIONS    (WM_USER + 2243)
#define AEM_FIXEDCHARWIDTH        (WM_USER + 2244)
#define AEM_GETSCROLLSPEED        (WM_USER + 2245)
#define AEM_SETSCROLLSPEED        (WM_USER + 2246)

//Draw
#define AEM_SHOWSCROLLBAR         (WM_USER + 2351)
#define AEM_UPDATESCROLLBAR       (WM_USER + 2352)
#define AEM_UPDATECARET           (WM_USER + 2353)
#define AEM_UPDATESIZE            (WM_USER + 2354)
#define AEM_LOCKUPDATE            (WM_USER + 2355)
#define AEM_LOCKERASERECT         (WM_USER + 2356)
#define AEM_GETERASERECT          (WM_USER + 2357)
#define AEM_SETERASERECT          (WM_USER + 2358)
#define AEM_HIDESELECTION         (WM_USER + 2361)
#define AEM_REDRAWLINERANGE       (WM_USER + 2362)
#define AEM_GETBACKGROUNDIMAGE    (WM_USER + 2366)
#define AEM_SETBACKGROUNDIMAGE    (WM_USER + 2367)

//Folding
#define AEM_GETFOLDSTACK          (WM_USER + 2381)
#define AEM_GETFOLDCOUNT          (WM_USER + 2382)
#define AEM_ADDFOLD               (WM_USER + 2383)
#define AEM_DELETEFOLD            (WM_USER + 2384)
#define AEM_ISFOLDVALID           (WM_USER + 2385)
#define AEM_FINDFOLD              (WM_USER + 2386)
#define AEM_NEXTFOLD              (WM_USER + 2387)
#define AEM_PREVFOLD              (WM_USER + 2388)
#define AEM_ISLINECOLLAPSED       (WM_USER + 2389)
#define AEM_COLLAPSELINE          (WM_USER + 2390)
#define AEM_COLLAPSEFOLD          (WM_USER + 2391)
#define AEM_UPDATEFOLD            (WM_USER + 2392)
#define AEM_GETFOLDHIDEOFFSET     (WM_USER + 2393)
#define AEM_SETFOLDHIDEOFFSET     (WM_USER + 2394)

//Document
#define AEM_CREATEDOCUMENT        (WM_USER + 2401)
#define AEM_DELETEDOCUMENT        (WM_USER + 2402)
#define AEM_GETDOCUMENT           (WM_USER + 2403)
#define AEM_SETDOCUMENT           (WM_USER + 2404)
#define AEM_GETDOCUMENTPROC       (WM_USER + 2405)
#define AEM_GETDOCUMENTWINDOW     (WM_USER + 2406)
#define AEM_SENDMESSAGE           (WM_USER + 2407)

//Clone
#define AEM_ADDCLONE              (WM_USER + 2421)
#define AEM_DELCLONE              (WM_USER + 2422)
#define AEM_GETMASTER             (WM_USER + 2423)
#define AEM_GETCLONE              (WM_USER + 2424)

//Print
#define AEM_STARTPRINTDOC         (WM_USER + 2451)
#define AEM_PRINTPAGE             (WM_USER + 2452)
#define AEM_ENDPRINTDOC           (WM_USER + 2453)

//Highlight
#define AEM_HLCREATETHEMEA        (WM_USER + 2501)
#define AEM_HLCREATETHEMEW        (WM_USER + 2502)
#define AEM_HLFINDTHEME           (WM_USER + 2504)
#define AEM_HLGETTHEMENAMEA       (WM_USER + 2505)
#define AEM_HLGETTHEMENAMEW       (WM_USER + 2506)
#define AEM_HLGETTHEMESTACK       (WM_USER + 2507)
#define AEM_HLTHEMEEXISTS         (WM_USER + 2508)
#define AEM_HLSETTHEME            (WM_USER + 2509)
#define AEM_HLDELETETHEME         (WM_USER + 2510)
#define AEM_HLDELETETHEMEALL      (WM_USER + 2511)
#define AEM_HLADDDELIMITERA       (WM_USER + 2521)
#define AEM_HLADDDELIMITERW       (WM_USER + 2522)
#define AEM_HLDELETEDELIMITER     (WM_USER + 2523)
#define AEM_HLADDWORDA            (WM_USER + 2531)
#define AEM_HLADDWORDW            (WM_USER + 2532)
#define AEM_HLDELETEWORD          (WM_USER + 2533)
#define AEM_HLADDQUOTEA           (WM_USER + 2541)
#define AEM_HLADDQUOTEW           (WM_USER + 2542)
#define AEM_HLDELETEQUOTE         (WM_USER + 2543)
#define AEM_HLADDMARKTEXTA        (WM_USER + 2551)
#define AEM_HLADDMARKTEXTW        (WM_USER + 2552)
#define AEM_HLDELETEMARKTEXT      (WM_USER + 2553)
#define AEM_HLADDMARKRANGE        (WM_USER + 2561)
#define AEM_HLDELETEMARKRANGE     (WM_USER + 2562)
#define AEM_HLGETHIGHLIGHT        (WM_USER + 2571)
#define AEM_HLGETOPTIONS          (WM_USER + 2581)
#define AEM_HLSETOPTIONS          (WM_USER + 2582)


//// RichEdit messages

/*
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
*/


//// AkelEdit messages description

/*
AEN_ERRSPACE
____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control cannot allocate enough memory.

(int)wParam           == specifies the control identifier.
(AENERRSPACE *)lParam == pointer to a AENERRSPACE structure.

Return Value
 Zero.


AEN_SETFOCUS
____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control receives the keyboard focus.

(int)wParam     == specifies the control identifier.
(NMHDR *)lParam == pointer to a NMHDR structure.

Return Value
 Zero.


AEN_KILLFOCUS
_____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control loses the keyboard focus.

(int)wParam     == specifies the control identifier.
(NMHDR *)lParam == pointer to a NMHDR structure.

Return Value
 Zero.


AEN_HSCROLL
___________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before an edit control window scrolled horizontally.

(int)wParam         == specifies the control identifier.
(AENSCROLL *)lParam == pointer to a AENSCROLL structure.

Return Value
 If zero, the control allows scroll operation.
 If a nonzero value, the control suppress scroll operation.

Remarks
 To receive AEN_HSCROLL notifications, specify AENM_SCROLL in the mask sent with the AEM_SETEVENTMASK message.


AEN_VSCROLL
___________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before an edit control window scrolled vertically.

(int)wParam         == specifies the control identifier.
(AENSCROLL *)lParam == pointer to a AENSCROLL structure.

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
(AENSETRECT *)lParam == pointer to a AENSETRECT structure.

Return Value
 Zero.

Remarks
 AEN_SETRECT notification is not sent during processing WM_SIZE message.


AEN_PAINT
_________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before line is painted.

(int)wParam        == specifies the control identifier.
(AENPAINT *)lParam == pointer to a AENPAINT structure.

Return Value
 Zero.

Remarks
 To receive AEN_PAINT notifications, specify AENM_PAINT in the mask sent with the AEM_SETEVENTMASK message.


AEN_MAXTEXT
___________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when an edit control rich text limit.

(int)wParam     == specifies the control identifier.
(NMHDR *)lParam == pointer to a NMHDR structure.

Return Value
 Zero.


AEN_PROGRESS
____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when required number of lines was proceed.

(int)wParam           == specifies the control identifier.
(AENPROGRESS *)lParam == pointer to a AENPROGRESS structure.

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
(AENMODIFY *)lParam == pointer to a AENMODIFY structure.

Return Value
 Zero.

Remarks
 To receive AEN_MODIFY notifications, specify AENM_MODIFY in the mask sent with the AEM_SETEVENTMASK message.


AEN_SELCHANGING
_______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before the current selection has changed.

(int)wParam            == specifies the control identifier.
(AENSELCHANGE *)lParam == pointer to a AENSELCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_SELCHANGING notifications, specify AENM_SELCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_SELCHANGED
______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after the current selection has changed.

(int)wParam            == specifies the control identifier.
(AENSELCHANGE *)lParam == pointer to a AENSELCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_SELCHANGED notifications, specify AENM_SELCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTCHANGING
________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before the document text has changed.

(int)wParam             == specifies the control identifier.
(AENTEXTCHANGE *)lParam == pointer to a AENTEXTCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTCHANGING notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTINSERTBEGIN
___________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before text is inserted in the document.

(int)wParam             == specifies the control identifier.
(AENTEXTINSERT *)lParam == pointer to a AENTEXTINSERT structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTINSERTBEGIN notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTINSERTEND
_________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after text is inserted in the document.

(int)wParam             == specifies the control identifier.
(AENTEXTINSERT *)lParam == pointer to a AENTEXTINSERT structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTINSERTEND notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTDELETEBEGIN
___________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure before text is deleted from the document.

(int)wParam             == specifies the control identifier.
(AENTEXTDELETE *)lParam == pointer to a AENTEXTDELETE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTDELETEBEGIN notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTDELETEEND
_________________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after text is deleted from the document.

(int)wParam             == specifies the control identifier.
(AENTEXTDELETE *)lParam == pointer to a AENTEXTDELETE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTDELETEEND notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_TEXTCHANGED
_______________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after the document text has changed.

(int)wParam             == specifies the control identifier.
(AENTEXTCHANGE *)lParam == pointer to a AENTEXTCHANGE structure.

Return Value
 Zero.

Remarks
 To receive AEN_TEXTCHANGED notifications, specify AENM_TEXTCHANGE in the mask sent with the AEM_SETEVENTMASK message.


AEN_POINT
_________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after text point has changed.

(int)wParam        == specifies the control identifier.
(AENPOINT *)lParam == pointer to a AENPOINT structure.

Return Value
 Zero.

Remarks
 To receive AEN_POINT notifications, specify AENM_POINT in the mask sent with the AEM_SETEVENTMASK message.


AEN_DROPFILES
_____________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure when the user is attempting to drop files into the control (when it receives the WM_DROPFILES message).

(int)wParam            == specifies the control identifier.
(AENDROPFILES *)lParam == pointer to a AENDROPFILES structure.

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
(AENDROPSOURCE *)lParam == pointer to a AENDROPSOURCE structure.

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
(AENDROPTARGET *)lParam == pointer to a AENDROPTARGET structure.

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
(AENLINK *)lParam == pointer to a AENLINK structure.

Return Value
 If zero, the control proceeds with its normal handling of the mouse message.
 If a nonzero value, the control does not handle the mouse message.

Remarks
 To receive AEN_LINK notifications, specify AENM_LINK in the mask sent with the AEM_SETEVENTMASK message and turn on URL detection with AECOE_DETECTURL flag.


AEN_MARKER
__________

Notification message sends in the form of a WM_NOTIFY message.
Sends to the parent window procedure after column marker position or type is changed.

(int)wParam         == specifies the control identifier.
(AENMARKER *)lParam == pointer to a AENMARKER structure.

Return Value
 Zero.

Remarks
 To receive AEN_MARKER notifications, specify AENM_MARKER in the mask sent with the AEM_SETEVENTMASK message.


AEM_EXSETTEXTA
______________

Set ansi text of the edit control.

wParam               == not used.
(AESETTEXTA *)lParam == pointer to a AESETTEXTA structure.

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
(AESETTEXTW *)lParam == pointer to a AESETTEXTW structure.

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
(char *)lParam   == text pointer.

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
(wchar_t *)lParam == text pointer.

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
(AEAPPENDTEXTA *)lParam == pointer to a AEAPPENDTEXTA structure.

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
(AEAPPENDTEXTW *)lParam == pointer to a AEAPPENDTEXTW structure.

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
(AEREPLACESELA *)lParam == pointer to a AEREPLACESELA structure.

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
(AEREPLACESELW *)lParam == pointer to a AEREPLACESELW structure.

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
(AETEXTRANGEA *)lParam == pointer to a AETEXTRANGEA structure.

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
   if (tr.pBuffer=(char *)GlobalAlloc(GPTR, tr.dwBufferMax))
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
(AETEXTRANGEW *)lParam == pointer to a AETEXTRANGEW structure.

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
   if (tr.pBuffer=(wchar_t *)GlobalAlloc(GPTR, tr.dwBufferMax * sizeof(wchar_t)))
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
(AESTREAMIN *)lParam == pointer to a AESTREAMIN structure.

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
   STREAMINDATA *lpData=(STREAMINDATA *)dwCookie;
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
(AESTREAMOUT *)lParam == pointer to a AESTREAMOUT structure.

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
   STREAMOUTDATA *lpData=(STREAMOUTDATA *)dwCookie;
   unsigned char *pDataToWrite=(unsigned char *)wszBuf;
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

(int)wParam   == see AELB_* defines.
(DWORD)lParam == see AEPFC_* defines.

Return Value
 Number of characters pasted, -1 if error.

Example:
 SendMessage(hWndEdit, AEM_PASTE, 0, 0);


AEM_CUT
_______

Delete the current selection, if any, and copy the deleted text to the clipboard.

(int)wParam   == see AELB_* defines.
(DWORD)lParam == see AECFC_* defines.

Return Value
 TRUE   clipboard changed.
 FALSE  clipboard not changed.

Example:
 SendMessage(hWndEdit, AEM_CUT, 0, 0);


AEM_COPY
________

Copy the current selection to the clipboard.

(int)wParam   == see AELB_* defines.
(DWORD)lParam == see AECFC_* defines.

Return Value
 TRUE   clipboard changed.
 FALSE  clipboard not changed.

Example:
 SendMessage(hWndEdit, AEM_COPY, 0, 0);


AEM_CHECKCODEPAGE
_________________

Check that the contents of an edit control can be converted to the specified code page.

(int)wParam   == code page to check.
(int *)lParam == returns nonconverted character in line (first character == 1). Can be NULL.

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
(AEFINDTEXTA *)lParam == pointer to a AEFINDTEXTA structure.

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
   aes.dwType=0;
   SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
 }


AEM_FINDTEXTW
_____________

Find unicode text within an edit control.

wParam                == not used.
(AEFINDTEXTW *)lParam == pointer to a AEFINDTEXTW structure.

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
   aes.dwType=0;
   SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
 }


AEM_ISMATCHA
____________

Is ansi text matched with text at specified position.

(AECHARINDEX *)wParam == position to check from.
(AEFINDTEXTA *)lParam == pointer to a AEFINDTEXTA structure. AEFINDTEXTA.crSearch member is ignored.

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
   aes.dwType=0;
   SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
 }


AEM_ISMATCHW
____________

Is unicode text matched with text at specified position.

(AECHARINDEX *)wParam == position to check from.
(AEFINDTEXTW *)lParam == pointer to a AEFINDTEXTW structure. AEFINDTEXTW.crSearch member is ignored.

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
   aes.dwType=0;
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

(AECHARINDEX *)wParam == character index.
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
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_UNDO, 0, 0);


AEM_REDO
________

Redo the next edit control operation in the control's redo queue.

wParam == not used.
lParam == not used.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 SendMessage(hWndEdit, AEM_REDO, 0, 0);


AEM_EMPTYUNDOBUFFER
___________________

Erase undo and redo history.

(BOOL)wParam   == TRUE   erase only redo history.
                  FALSE  erase undo and redo history.
(HANDLE)lParam == undo stack returned by AEM_DETACHUNDO. If not zero, then wParam is ignored.

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
(DWORD *)lParam == pointer to a variable that receives current number of actions stored in the undo queue. Can be NULL.

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
(CHARRANGE64 *)lParam == range of characters to check (RichEdit offset).

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


AEM_DETACHUNDO
______________

Detach undo stack.

wParam == not used.
lParam == not used.

Return Value
 Undo stack.

Remarks
 AEM_DETACHUNDO requires ES_GLOBALUNDO style.
 If undo stack not attached by AEM_ATTACHUNDO, then it must be released by AEM_EMPTYUNDOBUFFER.

Example:
 HANDLE hUndoStack;

 hUndoStack=(HANDLE)SendMessage(hWndEdit1, AEM_DETACHUNDO, 0, 0);
 SendMessage(hWndEdit2, AEM_ATTACHUNDO, 0, (LPARAM)hUndoStack);


AEM_ATTACHUNDO
______________

Attach undo stack.

wParam         == not used.
(HANDLE)lParam == undo stack returned by AEM_DETACHUNDO. Current undo stack will be replaced with this one.

Return Value
 TRUE   success.
 FALSE  failed.

Remarks
 AEM_ATTACHUNDO requires ES_GLOBALUNDO style.

Example:
 See AEM_DETACHUNDO example.


AEM_GETUNDOPOS
______________

Retrieve RichEdit offset of undo item.

(DWORD)wParam    == see AEGUP_* defines.
(HANDLE *)lParam == pointer to a undo item handle.

Return Value
 RichEdit offset of undo item. -1 if error.

Example (go to the previous undo position):
 HANDLE hUndoItem=NULL;
 INT_PTR nOffset=SendMessage(hWndEdit, AEM_GETUNDOPOS, AEGUP_PREV, (LPARAM)&hUndoItem);

 if (nOffset != -1)
   SendMessage(hWndEdit, EM_SETSEL, nOffset, nOffset);


AEM_EXGETSEL
____________

Retrieve the current selection information of an edit control. Simple form of AEM_GETSEL.

(AECHARINDEX *)wParam == selection start index. Can be NULL.
(AECHARINDEX *)lParam == selection end index. Can be NULL.

Return Value
 TRUE   selection is not empty.
 FALSE  selection is empty.

Example:
 AECHARRANGE aecr;

 SendMessage(hWndEdit, AEM_EXGETSEL, (WPARAM)&aecr.ciMin, (LPARAM)&aecr.ciMax);


AEM_EXSETSEL
____________

Set the current selection of an edit control. Simple form of AEM_SETSEL.

(AECHARINDEX *)wParam == selection start index.
(AECHARINDEX *)lParam == selection end index.

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

(AECHARINDEX *)wParam == caret index, NULL if not needed.
(AESELECTION *)lParam == pointer to a AESELECTION structure, NULL if not needed.

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

(AECHARINDEX *)wParam == caret index, can be NULL.
(AESELECTION *)lParam == pointer to a AESELECTION structure.

Return Value
 Zero.

Example:
 AESELECTION aes;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&aes.crSel.ciMin);
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&aes.crSel.ciMax);
 aes.dwFlags=0;
 aes.dwType=0;
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

(int)wParam     == see AEGL_* defines.
(INT_PTR)lParam == input character RichEdit offset, if required.

Return Value
 Zero based line number. -1 if error.

Example:
 SendMessage(hWndEdit, AEM_GETLINENUMBER, AEGL_LINECOUNT, 0);


AEM_GETINDEX
____________

Retrieve the specified character index.

(int)wParam           == see AEGI_* defines.
(AECHARINDEX *)lParam == character index.

Return Value
 Zero if failed, non-zero if success.

Example:
 AECHARINDEX ciChar;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ciChar);


AEM_GETLINEINDEX
________________

Retrieve the first character index in the specified line.

(int)wParam           == zero based line number.
(AECHARINDEX *)lParam == character index.

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
(AECHARINDEX *)lParam == character index.

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

(AECHARINDEX *)wParam == first character index.
(AECHARINDEX *)lParam == second character index.

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
(AEINDEXSUBTRACT *)lParam == pointer to a AEINDEXSUBTRACT structure.

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
(AEINDEXOFFSET *)lParam == pointer to a AEINDEXOFFSET structure.

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
(AECHARINDEX *)lParam == AkelEdit character index.

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
(AECHARINDEX *)lParam == AkelEdit character index.

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
(AECHARINDEX *)lParam == returned first character in wrapped line. Can be NULL.

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
(AECHARINDEX *)lParam == character index (input/output).

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
(AECHARINDEX *)lParam == character index (input/output).

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
(AECHARINDEX *)lParam == character index.

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
                          see AECTI_* defines.
(AECHARINDEX *)lParam == AkelEdit character index.

Return Value
 Zero based column in line of the character index.

Example:
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_INDEXTOCOLUMN, MAKELONG(0, AECTI_WRAPLINEBEGIN), (LPARAM)&ciCaret);


AEM_COLUMNTOINDEX
_________________

Retrieve the character index of the column in line taking into account tab stop size.

(DWORD)wParam         == low-order word:
                          tab stop size in characters. Use current value if zero.
                         high-order word:
                          see AECTI_* defines.
(AECHARINDEX *)lParam == Input:  AECHARINDEX.lpLine and AECHARINDEX.nLine members specifies line to scan from.
                                 AECHARINDEX.nCharInLine specifies zero based column in line.
                         Output: AECHARINDEX structure is filled with result character index.

Return Value
 TRUE   success.
 FALSE  failed.

Example:
 AECHARINDEX ciCaret;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 ciCaret.nCharInLine=10;
 SendMessage(hWndEdit, AEM_COLUMNTOINDEX, MAKELONG(0, AECTI_WRAPLINEBEGIN), (LPARAM)&ciCaret);


AEM_INDEXINURL
______________

Checks is index in URL.

(AECHARINDEX *)wParam == character index.
(AECHARRANGE *)lParam == pointer to a AECHARRANGE structure, that receives URL range, if character in URL.

Return Value
 Length of the URL if index in URL or zero if not in URL.

Remarks
 Use AEM_POINTONURL to detect URL under point.

Example:
 AECHARINDEX ciCaret;
 AECHARRANGE crUrl;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_INDEXINURL, (WPARAM)&ciCaret, (LPARAM)&crUrl);


AEM_URLVISIT
____________

URL visit operations.

(int)wParam  == see AEUV_* defines.
(void)lParam == depend of AEUV_* define.

Return Value
 Depend of AEUV_* define.

Example:
 AECHARINDEX ciCaret;
 AECHARRANGE crUrl;
 AEURLVISIT *lpUrlVisit;

 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 if (SendMessage(hWndEdit, AEM_INDEXINURL, (WPARAM)&ciCaret, (LPARAM)&crUrl))
 {
   if (lpUrlVisit=(AEURLVISIT *)SendMessage(hWndEdit, AEM_URLVISIT, AEUV_GET, (LPARAM)&crUrl))
   {
     //Reset count of visits. Control could automatically delete item with zero visits.
     lpUrlVisit->nVisitCount=0;
     InvalidateRect(hWndEdit, NULL, FALSE);
   }
 }


AEM_ADDPOINT
____________

Add character index to a points stack. Character index will be updated after every text change.

wParam            == not used.
(AEPOINT *)lParam == pointer to a filled AEPOINT structure. AEPOINT.ciPoint and AEPOINT.nPointOffset members required.

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

 if (lpPoint=(AEPOINT *)SendMessage(hWndEdit, AEM_ADDPOINT, 0, (LPARAM)&point))
 {
   SendMessage(hWndEdit, EM_REPLACESEL, TRUE, (LPARAM)"123");
   ciChar=lpPoint->ciPoint;  //Read new position
   SendMessage(hWndEdit, AEM_DELPOINT, (WPARAM)lpPoint, 0);
 }


AEM_DELPOINT
____________

Delete character index from points stack.

(AEPOINT *)wParam == pointer to a AEPOINT structure, returned by AEM_ADDPOINT.
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

  if (hPointStack=(HSTACK *)SendMessage(hWndEdit, AEM_GETPOINTSTACK, 0, 0))
  {
    lpPointItem=(AEPOINT *)hPointStack->first;

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

(POINT64 *)wParam     == Input:  coordinates of a point in the control's virtual text space.
                         Output: exact coordinates in the virtual text space of a returned character.
(AECHARINDEX *)lParam == character index.

Return Value
 See AEPC_* defines.

Example:
 AECHARINDEX ciCaret;
 POINT64 ptGlobal;

 SendMessage(hWndEdit, AEM_GETCARETPOS, (WPARAM)NULL, (LPARAM)&ptGlobal);
 SendMessage(hWndEdit, AEM_CHARFROMGLOBALPOS, (WPARAM)&ptGlobal, (LPARAM)&ciCaret);


AEM_GLOBALPOSFROMCHAR
_____________________

Retrieve the coordinates in the virtual text space of a specified character in an edit control.

(POINT64 *)wParam     == coordinates of a point in the control's virtual text space.
(AECHARINDEX *)lParam == character index.

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

(POINT *)wParam       == Input:  coordinates of a point in the control's client area.
                         Output: exact client coordinates of a returned character.
(AECHARINDEX *)lParam == character index.

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

(POINT *)wParam       == coordinates of a point in the control's client area.
(AECHARINDEX *)lParam == character index.

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
(RECT *)lParam == pointer to a RECT structure that receives the formatting rectangle.

Return Value
 Zero.

Example:
 RECT rc;

 SendMessage(hWndEdit, AEM_GETRECT, 0, (LPARAM)&rc);


AEM_SETRECT
___________

Set the formatting rectangle of an edit control. The formatting rectangle is the limiting rectangle into which the control draws the text.

(DWORD)wParam  == see AERC_* defines.
(RECT *)lParam == pointer to a RECT structure that specifies the new dimensions of the rectangle. If this parameter is NULL, the formatting rectangle is set to its default values.

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

(POINT64 *)wParam == pointer to a POINT64 structure that receives the maximum scroll position in the virtual text space of the document, expressed in pixels. Can be NULL.
(POINT64 *)lParam == pointer to a POINT64 structure that receives the upper-left corner position in the virtual text space of the document, expressed in pixels. Can be NULL.

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
(POINT64 *)lParam == pointer to a POINT64 structure which specifies a point in the virtual text space of the document, expressed in pixels.
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
(AESCROLLTOPOINT *)lParam == pointer to a AESCROLLTOPOINT structure. If lParam is NULL, then do smart scroll to caret position.

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
(AECHARINDEX *)lParam == character index.

Return Value
 Integer that depend on AECS_* value.

Example:
 SendMessage(hWndEdit, AEM_GETCHARSIZE, AECS_HEIGHT, (LPARAM)NULL);


AEM_GETSTRWIDTH
_______________

Retrieve string width. Uses the currently selected font.

(wchar_t *)wParam == unicode string.
(int)lParam       == string length.

Return Value
 String width.

Example:
 SendMessage(hWndEdit, AEM_GETSTRWIDTH, (WPARAM)L"ABC", 3);


AEM_GETCARETPOS
_______________

Obtain the current caret position.

(POINT *)wParam   == pointer to a POINT structure that receives the caret position in the client area coordinates, expressed in pixels. Can be NULL.
(POINT64 *)lParam == pointer to a POINT64 structure that receives the caret position in the virtual text space of the document, expressed in pixels. Can be NULL.

Return Value
 TRUE   caret is visible.
 FALSE  caret isn't visible.

Example:
 POINT ptClient;
 POINT64 ptGlobal;

 SendMessage(hWndEdit, AEM_GETCARETPOS, (WPARAM)&ptClient, (LPARAM)&ptGlobal);


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
(POINT64 *)lParam == pointer to a POINT64 structure.

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

(POINT *)wParam == coordinates of a point in the control's client area.
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

(POINT *)wParam == coordinates of a point in the control's client area.
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

(POINT *)wParam       == coordinates of a point in the control's client area.
(AECHARRANGE *)lParam == pointer to a AECHARRANGE structure, that receives URL range, if found. Can be NULL.

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
(AECOLORS *)lParam == pointer to a AECOLORS structure.

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
(AECOLORS *)lParam == pointer to a AECOLORS structure.

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
(POINT *)lParam == pointer to a POINT structure that receives the insert mode caret width in x member and overtype mode caret height in y member.

Return Value
 Zero.

Example:
 POINT pt;

 SendMessage(hWndEdit, AEM_GETCARETWIDTH, 0, (LPARAM)&pt);


AEM_SETCARETWIDTH
_________________

Set caret width.

wParam          == not used.
(POINT *)lParam == pointer to a POINT structure which specifies the insert mode caret width in x member and overtype mode caret height in y member.

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
(DWORD *)lParam == pointer to a variable that receives wrap limit number. Can be NULL.

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
(wchar_t *)lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, including the terminating two NULL characters.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETWORDDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETWORDDELIMITERS
_____________________

Set word break delimiters.

wParam            == not used.
(wchar_t *)lParam == two null-terminated string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n[](){}<>\0\0";

 SendMessage(hWndEdit, AEM_SETWORDDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETWRAPDELIMITERS
_____________________

Retrieve word wrapping delimiters.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t *)lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, including the terminating two NULL characters.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETWRAPDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETWRAPDELIMITERS
_____________________

Set delimiters for word wrapping.

wParam            == not used.
(wchar_t *)lParam == two null-terminated string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n[](){}<>\0\0";

 SendMessage(hWndEdit, AEM_SETWRAPDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETURLLEFTDELIMITERS
________________________

Retrieve URL left delimiters.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t *)lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, including the terminating two NULL characters.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETURLLEFTDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETURLLEFTDELIMITERS
________________________

Set left delimiters for URL detection.

wParam            == not used.
(wchar_t *)lParam == two null-terminated string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n'`\"(<{[=\0\0";

 SendMessage(hWndEdit, AEM_SETURLLEFTDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETURLRIGHTDELIMITERS
________________________

Retrieve URL right delimiters.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t *)lParam == pointer to a buffer that receives delimiter characters.

Return Value
 Number of characters copied, including the terminating two NULL characters.

Example:
 wchar_t wszDelimiters[128];

 SendMessage(hWndEdit, AEM_GETURLRIGHTDELIMITERS, 128, (LPARAM)wszDelimiters);


AEM_SETURLRIGHTDELIMITERS
________________________

Set right delimiters for URL detection.

wParam            == not used.
(wchar_t *)lParam == two null-terminated string that specifies delimiter characters. If NULL, then default delimiters will be used.

Return Value
 Zero.

Example:
 wchar_t wszDelimiters[128]=L" \t\n'`\")>}]\0\0";

 SendMessage(hWndEdit, AEM_SETURLRIGHTDELIMITERS, 0, (LPARAM)wszDelimiters);


AEM_GETURLPREFIXES
__________________

Retrieve URL prefixes.

(int)wParam       == size of the buffer in TCHARs.
(wchar_t *)lParam == pointer to a buffer that receives pairs of null-terminated prefixes strings. The last string terminated by two NULL characters.

Return Value
 Number of characters copied, including the terminating two NULL characters.

Example:
 wchar_t wszPrefixes[128];

 SendMessage(hWndEdit, AEM_GETURLPREFIXES, 128, (LPARAM)wszPrefixes);


AEM_SETURLPREFIXES
__________________

Set prefixes for URL detection.

wParam            == not used.
(wchar_t *)lParam == buffer containing pairs of null-terminated prefixes strings. The last string in the buffer must be terminated by two NULL characters. If NULL, then default prefixes will be used.

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

(int *)wParam == pointer to a variable that receives marker type, see AEMT_* defines. Can be NULL.
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

(int)wParam == line gap (external leading), default is zero.
lParam      == not used.

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

(AECHARINDEX *)wParam  == character index.
(AECHARCOLORS *)lParam == pointer to a AECHARCOLORS structure.

Return Value
 TRUE  character in selection.
 FALSE character not in selection.

Example:
 AECHARINDEX ciCaret;
 AECHARCOLORS aelc;

 aelc.dwFlags=0;
 SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
 SendMessage(hWndEdit, AEM_GETCHARCOLORS, (WPARAM)&ciCaret, (LPARAM)&aelc);


AEM_SCROLLCARETOPTIONS
______________________

Retrieve or set scroll to caret options.

(BOOL)wParam                   == TRUE   set caret operation.
                                  FALSE  retrieve caret operation.
(AESCROLLCARETOPTIONS *)lParam == pointer to a AESCROLLCARETOPTIONS structure. If this parameter is NULL and wParam is TRUE, the caret scroll options are set to its default values.

Return Value
 zero

Example:
 AESCROLLCARETOPTIONS sco;

 sco.dwFlags=AESC_OFFSETCHARX|AESC_OFFSETCHARY;
 sco.dwSelFlags=0;
 sco.dwSelType=AESCT_KEYBOARD;
 sco.nOffsetX=10;
 sco.nOffsetY=5;
 SendMessage(hWndEdit, AEM_SCROLLCARETOPTIONS, TRUE, (LPARAM)&sco);


AEM_FIXEDCHARWIDTH
__________________

Set fixed character width. All symbols, including proportional fonts, will be paint in this width.

(int)wParam  == character width or one of the following values:
                -AECS_AVEWIDTH   font latin character average width.
                -AECS_SPACEWIDTH font space width.
                -AECS_MAXWIDTH   font latin character maximum width.
                0                disable fixed width.
lParam       == not used.

Return Value
 Previous fixed width.

Remarks
 To retrieve current fixed width use AEM_GETCHARSIZE.

Example:
 SendMessage(hWndEdit, AEM_FIXEDCHARWIDTH, (WPARAM)-AECS_MAXWIDTH, 0);


AEM_GETSCROLLSPEED
___________________

Retrieve MButton scroll speed in percentage.

wParam == not used.
lParam == not used.

Return Value
 MButton scroll speed.

Example:
 SendMessage(hWndEdit, AEM_GETSCROLLSPEED, 0, 0);


AEM_SETSCROLLSPEED
___________________

Set MButton scroll speed.

(DWORD)wParam == MButton scroll speed in percentage (default is 10).
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETSCROLLSPEED, 50, 0);


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
(const RECT *)lParam == pointer to a RECT structure that specifies the erasing rectangle.

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
(RECT *)lParam == pointer to a RECT structure that receives the erasing rectangle.

Return Value
 Zero.

Example:
 RECT rc;

 SendMessage(hWndEdit, AEM_GETERASERECT, 0, (LPARAM)&rc);


AEM_SETERASERECT
________________

Set the erasing rectangle of an edit control. The erasing rectangle is the limiting rectangle into which the control erase background.

(DWORD)wParam  == see AERC_* defines.
(RECT *)lParam == pointer to a RECT structure that specifies the new dimensions of the rectangle. If this parameter is NULL, the erasing rectangle is set to its default values.

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

  hFoldsStack=(HSTACK *)SendMessage(hWndEdit, AEM_GETFOLDSTACK, 0, 0);

  for (lpFold=(AEFOLD *)hFoldsStack->first; lpFold; lpFold=AEC_NextFold(lpFold, TRUE))
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

(int)wParam == see AEFC_* defines.
lParam      == not used.

Return Value
 Number of folds.

Example:
  SendMessage(hWndEdit, AEM_GETFOLDCOUNT, AEFC_ALL, 0);


AEM_ADDFOLD
___________

Add fold.

wParam           == not used.
(AEFOLD *)lParam == pointer to a filled AEFOLD structure. AEFOLD.next, AEFOLD.prev, AEFOLD.parent, AEFOLD.firstChild, AEFOLD.lastChild are ignored.

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
 fold.dwFlags=0;
 fold.dwFontStyle=AEHLS_NONE;
 fold.crText=RGB(0xFF, 0x00, 0x00);
 fold.crBk=(DWORD)-1;
 fold.dwUserData=0;
 SendMessage(hWndEdit, AEM_ADDFOLD, 0, (LPARAM)&fold);


AEM_DELETEFOLD
______________

Deletes specified or all folds.

(AEFOLD *)wParam == fold handle (pointer to a AEFOLD structure). If NULL, then delete all folds.
(BOOL)lParam     == TRUE  update scroll and selection.
                    FALSE don't update scroll and selection.

Return Value
 Number of deleted folds that were collapsed.

Example:
 SendMessage(hWndEdit, AEM_DELETEFOLD, (WPARAM)lpFold, TRUE);


AEM_ISFOLDVALID
_______________

Checks is fold handle valid.

(AEFOLD *)wParam == fold handle (pointer to a AEFOLD structure), returned by AEM_ADDFOLD.
lParam           == not used.

Return Value
 TRUE   fold handle is valid.
 FALSE  fold handle isn't valid.

Example:
 SendMessage(hWndEdit, AEM_ISFOLDVALID, (WPARAM)lpFold, 0);


AEM_FINDFOLD
____________

Find fold handle.

(AEFINDFOLD *)wParam == pointer to a AEFINDFOLD structure.
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

(AEFOLD *)wParam == fold handle (pointer to a AEFOLD structure).
(BOOL)lParam     == TRUE   go into fold children if possible (recursive).
                    FALSE  don't go into fold children.

Return Value
 Fold handle (pointer to a AEFOLD structure).

Remarks
 For better performance use AEC_NextFold instead.

Example:
 AEFOLD *lpNextFold;

 lpNextFold=(AEFOLD *)SendMessage(hWndEdit, AEM_NEXTFOLD, (WPARAM)lpFold, TRUE);


AEM_PREVFOLD
____________

Retrieves previous fold handle.

(AEFOLD *)wParam == fold handle (pointer to a AEFOLD structure).
(BOOL)lParam     == TRUE   go into fold children if possible (recursive).
                    FALSE  don't go into fold children.

Return Value
 Fold handle (pointer to a AEFOLD structure).

Remarks
 For better performance use AEC_PrevFold instead.

Example:
 AEFOLD *lpPrevFold;

 lpPrevFold=(AEFOLD *)SendMessage(hWndEdit, AEM_PREVFOLD, (WPARAM)lpFold, TRUE);


AEM_ISLINECOLLAPSED
___________________

Checks is line collapsed.

(int)wParam == line number.
lParam      == not used.

Return Value
 Fold handle (pointer to a AEFOLD structure) that hides the line or NULL if line isn't collapsed.

Example:
 AEFOLD *lpFold;

 lpFold=(AEFOLD *)SendMessage(hWndEdit, AEM_ISLINECOLLAPSED, 5, 0);


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

(AEFOLD *)wParam == fold handle (pointer to a AEFOLD structure), returned by AEM_ADDFOLD. If NULL, then process all folds.
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


AEM_GETFOLDHIDEOFFSET
_____________________

Retrieve folding hide line offsets.

wParam == not used.
lParam == not used.

Return Value
 The low-order word contains the fold begin line offset to hide.
 The high-order word contains the fold end line offset to hide.

Example:
 DWORD dwHideLineOffsets=SendMessage(hWndEdit, AEM_GETFOLDHIDEOFFSET, 0, 0);
 int nHideMinLineOffset=(short)LOWORD(dwHideLineOffsets);
 int nHideMaxLineOffset=(short)HIWORD(dwHideLineOffsets);


AEM_SETFOLDHIDEOFFSET
_____________________

Set folding hide line offsets.

(DWORD)wParam == the low-order word contains the fold begin line offset to hide. Must be >= 1. Default is 1.
                 the high-order word contains the fold end line offset to hide. Must be <= 0. Default is -1.
lParam        == not used.

Return Value
 Zero.

Example:
 SendMessage(hWndEdit, AEM_SETFOLDHIDEOFFSET, MAKELONG(1, 0), 0);


AEM_CREATEDOCUMENT
__________________

Create new virtual document, that later can be associated with any edit window. Note that new edit window after creation already has assigned document handle.

wParam                 == not used.
(CREATESTRUCT *)lParam == pointer to a CREATESTRUCT structure.

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
 cs.lpszClass=AES_AKELEDITA;
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
(AESENDMESSAGE *)lParam == pointer to a AESENDMESSAGE structure.

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
(AEPRINT *)lParam == pointer to a AEPRINT structure.

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
(AEPRINT *)lParam == pointer to a AEPRINT structure.

Return Value
 TRUE   page is printed and there is more text for printing.
 FALSE  page is printed and there is no more text for printing.

Example:
 See AEM_STARTPRINTDOC example.


AEM_ENDPRINTDOC
_______________

Close print document handle.

(AEHPRINT)wParam  == document handle returned by AEM_STARTPRINTDOC.
(AEPRINT *)lParam == pointer to a AEPRINT structure.

Return Value
 Zero.

Example:
 See AEM_STARTPRINTDOC example.


AEM_HLCREATETHEMEA
__________________

Create empty highlight theme.

wParam         == not used.
(char *)lParam == ansi theme name.

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
(wchar_t *)lParam == unicode theme name.

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


AEM_HLFINDTHEME
_______________

Find highlight theme handle.

(int)wParam  == see AEHLFT_* defines.
(void)lParam == depend of AEHLFT_* define.

Return Value
 Theme handle.

Example:
 AEHTHEME hTheme=(AEHTHEME)SendMessage(hWndEdit, AEM_HLFINDTHEME, AEHLFT_BYNAMEW, (LPARAM)L"MyTheme");


AEM_HLGETTHEMENAMEA
___________________

Retrieve ansi highlight theme name.

(AEHTHEME)wParam == theme handle.
(char *)lParam   == pointer to a buffer that receives ansi highlight theme name. If NULL, return value is the required buffer size.

Return Value
 Length of the string copied to the buffer, in TCHARs, not including the terminating null character.

Example:
 char szThemeName[MAX_PATH];

 SendMessage(hWndEdit, AEM_HLGETTHEMENAMEA, (WPARAM)hTheme, (LPARAM)szThemeName);


AEM_HLGETTHEMENAMEW
___________________

Retrieve unicode highlight theme name.

(AEHTHEME)wParam  == theme handle.
(wchar_t *)lParam == pointer to a buffer that receives unicode highlight theme name. If NULL, return value is the required buffer size.

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
    hDelimStack=(HSTACK *)SendMessage(hWndEdit, AEM_HLGETTHEMESTACK, (WPARAM)hTheme, AEHLE_DELIMITER);

    for (lpDelimItem=(AEDELIMITEMW *)hDelimStack->first; lpDelimItem; lpDelimItem=lpDelimItem->next)
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
(AEDELIMITEMA *)lParam == pointer to a AEDELIMITEMA structure.

Return Value
 Delimiter handle - AEHDELIMITER (pointer to a AEDELIMITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDDELIMITERW
___________________

Add unicode delimiter to highlight theme.

(AEHTHEME)wParam       == theme handle or NULL for default window theme.
(AEDELIMITEMW *)lParam == pointer to a AEDELIMITEMW structure.

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
(AEWORDITEMA *)lParam == pointer to a AEWORDITEMA structure.

Return Value
 Word handle - AEHWORD (pointer to a AEWORDITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDWORDW
______________

Add unicode word to highlight theme.

(AEHTHEME)wParam      == theme handle or NULL for default window theme.
(AEWORDITEMW *)lParam == pointer to a AEWORDITEMW structure.

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
(AEQUOTEITEMA *)lParam == pointer to a AEQUOTEITEMA structure.

Return Value
 Quote handle - AEHQUOTE (pointer to a AEQUOTEITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDQUOTEW
_______________

Add unicode quote to highlight theme.

(AEHTHEME)wParam       == theme handle or NULL for default window theme.
(AEQUOTEITEMW *)lParam == pointer to a AEQUOTEITEMW structure.

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
(AEMARKTEXTITEMA *)lParam == pointer to a AEMARKTEXTITEMA structure.

Return Value
 Text mark handle - AEHMARKTEXT (pointer to a AEMARKTEXTITEMW structure).

Example:
 See AEM_HLCREATETHEMEA example.


AEM_HLADDMARKTEXTW
__________________

Add unicode text mark to highlight theme.

(AEHTHEME)wParam          == theme handle or NULL for default window theme.
(AEMARKTEXTITEMW *)lParam == pointer to a AEMARKTEXTITEMW structure.

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
(AEMARKRANGEITEM *)lParam == pointer to a AEMARKRANGEITEM structure.

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
(AEGETHIGHLIGHT *)lParam == pointer to a AEGETHIGHLIGHT structure.

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

*/


//// UNICODE define

#ifndef UNICODE
  #define AES_AKELEDIT AES_AKELEDITA
  #define AES_RICHEDIT20 AES_RICHEDIT20A

  #define TEXTRANGE64 TEXTRANGE64A
  #define FINDTEXTEX64 FINDTEXTEX64A

  #define AEAPPENDTEXT AEAPPENDTEXTA
  #define AEREPLACESEL AEREPLACESELA
  #define AETEXTRANGE AETEXTRANGEA
  #define AEFINDTEXT AEFINDTEXTA
  #define AEDELIMITEM AEDELIMITEMA
  #define AEWORDITEM AEWORDITEMA
  #define AEQUOTEITEM AEQUOTEITEMA
  #define AEMARKTEXTITEM AEMARKTEXTITEMA

  #define AEM_SETTEXT AEM_SETTEXTA
  #define AEM_APPENDTEXT AEM_APPENDTEXTA
  #define AEM_REPLACESEL AEM_REPLACESELA
  #define AEM_GETTEXTRANGE AEM_GETTEXTRANGEA
  #define AEM_FINDTEXT AEM_FINDTEXTA
  #define AEM_ISMATCH AEM_ISMATCHA
  #define AEM_HLCREATETHEME AEM_HLCREATETHEMEA
  #define AEM_HLGETTHEME AEM_HLGETTHEMEA
  #define AEM_HLGETTHEMENAME AEM_HLGETTHEMENAMEA
  #define AEM_HLADDDELIMITER AEM_HLADDDELIMITERA
  #define AEM_HLADDWORD AEM_HLADDWORDA
  #define AEM_HLADDQUOTE AEM_HLADDQUOTEA
  #define AEM_HLADDMARKTEXT AEM_HLADDMARKTEXTA
#else
  #define AES_AKELEDIT AES_AKELEDITW
  #define AES_RICHEDIT20 AES_RICHEDIT20W

  #define TEXTRANGE64 TEXTRANGE64W
  #define FINDTEXTEX64 FINDTEXTEX64W

  #define AEAPPENDTEXT AEAPPENDTEXTW
  #define AEREPLACESEL AEREPLACESELW
  #define AETEXTRANGE AETEXTRANGEW
  #define AEFINDTEXT AEFINDTEXTW
  #define AEDELIMITEM AEDELIMITEMW
  #define AEWORDITEM AEWORDITEMW
  #define AEQUOTEITEM AEQUOTEITEMW
  #define AEMARKTEXTITEM AEMARKTEXTITEMW

  #define AEM_SETTEXT AEM_SETTEXTW
  #define AEM_APPENDTEXT AEM_APPENDTEXTW
  #define AEM_REPLACESEL AEM_REPLACESELW
  #define AEM_GETTEXTRANGE AEM_GETTEXTRANGEW
  #define AEM_FINDTEXT AEM_FINDTEXTW
  #define AEM_ISMATCH AEM_ISMATCHW
  #define AEM_HLCREATETHEME AEM_HLCREATETHEMEW
  #define AEM_HLGETTHEME AEM_HLGETTHEMEW
  #define AEM_HLGETTHEMENAME AEM_HLGETTHEMENAMEW
  #define AEM_HLADDDELIMITER AEM_HLADDDELIMITERW
  #define AEM_HLADDWORD AEM_HLADDWORDW
  #define AEM_HLADDQUOTE AEM_HLADDQUOTEW
  #define AEM_HLADDMARKTEXT AEM_HLADDMARKTEXTW
#endif


//// AkelEdit functions

#ifndef AEC_IsSurrogate
  #define AEC_IsSurrogate(c)  ((wchar_t)(c) >= 0xD800 && (wchar_t)(c) <= 0xDFFF)
#endif
#ifndef AEC_IsHighSurrogate
  #define AEC_IsHighSurrogate(c)  ((wchar_t)(c) >= 0xD800 && (wchar_t)(c) <= 0xDBFF)
#endif
#ifndef AEC_IsLowSurrogate
  #define AEC_IsLowSurrogate(c)  ((wchar_t)(c) >= 0xDC00 && (wchar_t)(c) <= 0xDFFF)
#endif
#ifndef AEC_ScalarFromSurrogate
  #define AEC_ScalarFromSurrogate(high, low)  ((((high) - 0xD800) * 0x400) + ((low) - 0xDC00) + 0x10000)
#endif
#ifndef AEC_HighSurrogateFromScalar
  #define AEC_HighSurrogateFromScalar(s)  ((wchar_t)((((s) - 0x10000) / 0x400) + 0xD800))
#endif
#ifndef AEC_LowSurrogateFromScalar
  #define AEC_LowSurrogateFromScalar(s)  ((wchar_t)((((s) - 0x10000) % 0x400) + 0xDC00))
#endif

#ifdef AEC_FUNCTIONS
#define AEC_FUNCTIONS_INCLUDED
__inline int AEC_CopyChar(wchar_t *wszTarget, DWORD dwTargetSize, const wchar_t *wpSource)
  {
    if (AEC_IsSurrogate(*wpSource))
    {
      if (dwTargetSize >= 2)
      {
        if (AEC_IsHighSurrogate(*wpSource) && AEC_IsLowSurrogate(*(wpSource + 1)))
        {
          if (wszTarget)
          {
            *wszTarget=*wpSource;
            *(wszTarget + 1)=*(wpSource + 1);
          }
          return 2;
        }
      }
    }
    else
    {
      if (wszTarget) *wszTarget=*wpSource;
      return 1;
    }
    return 0;
  }

__inline int AEC_IndexInc(AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine >= 0 &&
        ciChar->nCharInLine + 1 < ciChar->lpLine->nLineLen)
    {
      if (AEC_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine]) &&
          AEC_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine + 1]))
      {
        ciChar->nCharInLine+=2;
        return 2;
      }
    }
    ++ciChar->nCharInLine;
    return 1;
  }

__inline int AEC_IndexDec(AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine - 2 >= 0 &&
        ciChar->nCharInLine - 1 < ciChar->lpLine->nLineLen)
    {
      if (AEC_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine - 1]) &&
          AEC_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine - 2]))
      {
        ciChar->nCharInLine-=2;
        return 2;
      }
    }
    --ciChar->nCharInLine;
    return 1;
  }

__inline int AEC_IndexLen(AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine >= 0 &&
        ciChar->nCharInLine + 1 < ciChar->lpLine->nLineLen)
    {
      if (AEC_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine]) &&
          AEC_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine + 1]))
      {
        return 2;
      }
    }
    return 1;
  }

__inline int AEC_IndexCompare(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2)
  {
    if (ciChar1->nLine == ciChar2->nLine &&
        ciChar1->nCharInLine == ciChar2->nCharInLine)
    {
      return 0;
    }
    if ((ciChar1->nLine < ciChar2->nLine) ||
        (ciChar1->nLine == ciChar2->nLine &&
         ciChar1->nCharInLine < ciChar2->nCharInLine))
    {
      return -1;
    }
    return 1;
  }

__inline int AEC_IndexCompareEx(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2)
  {
    if ((ciChar1->nLine == ciChar2->nLine &&
         ciChar1->nCharInLine == ciChar2->nCharInLine) ||
        (ciChar1->lpLine && ciChar2->lpLine &&
          ((ciChar1->lpLine->next == ciChar2->lpLine &&
            ciChar1->lpLine->nLineBreak == AELB_WRAP &&
            ciChar1->nCharInLine == ciChar1->lpLine->nLineLen &&
            ciChar2->nCharInLine == 0) ||
           (ciChar2->lpLine->next == ciChar1->lpLine &&
            ciChar2->lpLine->nLineBreak == AELB_WRAP &&
            ciChar2->nCharInLine == ciChar2->lpLine->nLineLen &&
            ciChar1->nCharInLine == 0))))
    {
      return 0;
    }
    if ((ciChar1->nLine < ciChar2->nLine) ||
        (ciChar1->nLine == ciChar2->nLine &&
         ciChar1->nCharInLine < ciChar2->nCharInLine))
    {
      return -1;
    }
    return 1;
  }

__inline AELINEDATA* AEC_NextLine(AECHARINDEX *ciChar)
  {
    if (ciChar->lpLine)
    {
      ciChar->nLine+=1;
      ciChar->lpLine=ciChar->lpLine->next;
      ciChar->nCharInLine=0;
    }
    return ciChar->lpLine;
  }

__inline AELINEDATA* AEC_PrevLine(AECHARINDEX *ciChar)
  {
    if (ciChar->lpLine)
    {
      ciChar->nLine-=1;
      ciChar->lpLine=ciChar->lpLine->prev;
      if (ciChar->lpLine)
        ciChar->nCharInLine=ciChar->lpLine->nLineLen;
      else
        ciChar->nCharInLine=0;
    }
    return ciChar->lpLine;
  }

__inline AELINEDATA* AEC_NextLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    AECHARINDEX ciTmp=*ciIn;

    if (AEC_NextLine(&ciTmp))
    {
      *ciOut=ciTmp;
      return ciOut->lpLine;
    }
    else
    {
      *ciOut=*ciIn;
      return NULL;
    }
  }

__inline AELINEDATA* AEC_PrevLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    AECHARINDEX ciTmp=*ciIn;

    if (AEC_PrevLine(&ciTmp))
    {
      *ciOut=ciTmp;
      return ciOut->lpLine;
    }
    else
    {
      *ciOut=*ciIn;
      return NULL;
    }
  }

__inline AELINEDATA* AEC_NextChar(AECHARINDEX *ciChar)
  {
    AEC_IndexInc(ciChar);

    if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
    {
      if (ciChar->nCharInLine > ciChar->lpLine->nLineLen ||
          ciChar->lpLine->nLineBreak == AELB_WRAP)
      {
        AEC_NextLine(ciChar);
      }
    }
    return ciChar->lpLine;
  }

__inline AELINEDATA* AEC_PrevChar(AECHARINDEX *ciChar)
  {
    AEC_IndexDec(ciChar);

    if (ciChar->nCharInLine < 0)
    {
      if (AEC_PrevLine(ciChar))
      {
        if (ciChar->lpLine->nLineBreak == AELB_WRAP)
          AEC_IndexDec(ciChar);
      }
    }
    return ciChar->lpLine;
  }

__inline AELINEDATA* AEC_NextCharEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    AECHARINDEX ciTmp=*ciIn;

    if (AEC_NextChar(&ciTmp))
    {
      *ciOut=ciTmp;
      return ciOut->lpLine;
    }
    else
    {
      *ciOut=*ciIn;
      return NULL;
    }
  }

__inline AELINEDATA* AEC_PrevCharEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    AECHARINDEX ciTmp=*ciIn;

    if (AEC_PrevChar(&ciTmp))
    {
      *ciOut=ciTmp;
      return ciOut->lpLine;
    }
    else
    {
      *ciOut=*ciIn;
      return NULL;
    }
  }

__inline AELINEDATA* AEC_NextCharInLine(AECHARINDEX *ciChar)
  {
    AEC_IndexInc(ciChar);

    if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
    {
      if (ciChar->lpLine->nLineBreak == AELB_WRAP)
        AEC_NextLine(ciChar);
      else
        return NULL;
    }
    return ciChar->lpLine;
  }

__inline AELINEDATA* AEC_PrevCharInLine(AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine == 0)
    {
      if (!ciChar->lpLine->prev || ciChar->lpLine->prev->nLineBreak != AELB_WRAP)
        return NULL;
    }
    AEC_PrevChar(ciChar);
    return ciChar->lpLine;
  }

__inline AELINEDATA* AEC_NextCharInLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    AECHARINDEX ciTmp=*ciIn;

    if (AEC_NextCharInLine(&ciTmp))
    {
      *ciOut=ciTmp;
      return ciOut->lpLine;
    }
    else
    {
      *ciOut=*ciIn;
      return NULL;
    }
  }

__inline AELINEDATA* AEC_PrevCharInLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    AECHARINDEX ciTmp=*ciIn;

    if (AEC_PrevCharInLine(&ciTmp))
    {
      *ciOut=ciTmp;
      return ciOut->lpLine;
    }
    else
    {
      *ciOut=*ciIn;
      return NULL;
    }
  }

__inline AELINEDATA* AEC_ValidCharInLine(AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
    {
      if (ciChar->lpLine->nLineBreak == AELB_WRAP)
        AEC_NextLine(ciChar);
      else
        ciChar->nCharInLine=ciChar->lpLine->nLineLen;
    }
    else if (ciChar->nCharInLine < 0)
    {
      ciChar->nCharInLine=0;
    }
    return ciChar->lpLine;
  }

__inline int AEC_WrapLineBegin(AECHARINDEX *ciChar)
  {
    int nCount=ciChar->nCharInLine;

    if (ciChar->lpLine)
    {
      while (ciChar->lpLine->prev)
      {
        if (ciChar->lpLine->prev->nLineBreak != AELB_WRAP)
          break;

        --ciChar->nLine;
        ciChar->lpLine=ciChar->lpLine->prev;
        nCount+=ciChar->lpLine->nLineLen;
      }
    }
    ciChar->nCharInLine=0;
    return nCount;
  }

__inline int AEC_WrapLineEnd(AECHARINDEX *ciChar)
  {
    int nCount=ciChar->lpLine->nLineLen - ciChar->nCharInLine;

    while (ciChar->lpLine)
    {
      if (ciChar->lpLine->nLineBreak != AELB_WRAP)
        break;

      ++ciChar->nLine;
      ciChar->lpLine=ciChar->lpLine->next;
      nCount+=ciChar->lpLine->nLineLen;
    }
    ciChar->nCharInLine=ciChar->lpLine->nLineLen;
    return nCount;
  }

__inline int AEC_WrapLineBeginEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    *ciOut=*ciIn;
    return AEC_WrapLineBegin(ciOut);
  }

__inline int AEC_WrapLineEndEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
  {
    *ciOut=*ciIn;
    return AEC_WrapLineEnd(ciOut);
  }

__inline int AEC_CharAtIndex(const AECHARINDEX *ciChar)
  {
    int nChar;

    if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
    {
      if (ciChar->lpLine->nLineBreak == AELB_WRAP)
      {
        nChar=ciChar->lpLine->next->wpLine[0];
        if (AEC_IsHighSurrogate(nChar))
          nChar=AEC_ScalarFromSurrogate(nChar, ciChar->lpLine->next->wpLine[1]);
        return nChar;
      }
      return -ciChar->lpLine->nLineBreak;
    }
    nChar=ciChar->lpLine->wpLine[ciChar->nCharInLine];
    if (AEC_IsHighSurrogate(nChar))
      nChar=AEC_ScalarFromSurrogate(nChar, ciChar->lpLine->wpLine[ciChar->nCharInLine + 1]);
    return nChar;
  }

__inline BOOL AEC_IsCharInSelection(const AECHARINDEX *ciChar)
  {
    if (ciChar->lpLine->nSelStart <= ciChar->nCharInLine && ciChar->nCharInLine < ciChar->lpLine->nSelEnd)
      return TRUE;
    return FALSE;
  }

__inline BOOL AEC_IsFirstCharInLine(const AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine == 0 && (!ciChar->lpLine->prev || ciChar->lpLine->prev->nLineBreak != AELB_WRAP))
      return TRUE;
    return FALSE;
  }

__inline BOOL AEC_IsLastCharInLine(const AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine == ciChar->lpLine->nLineLen && ciChar->lpLine->nLineBreak != AELB_WRAP)
      return TRUE;
    return FALSE;
  }

__inline BOOL AEC_IsFirstCharInFile(const AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine == 0 && !ciChar->lpLine->prev)
      return TRUE;
    return FALSE;
  }

__inline BOOL AEC_IsLastCharInFile(const AECHARINDEX *ciChar)
  {
    if (ciChar->nCharInLine == ciChar->lpLine->nLineLen && !ciChar->lpLine->next)
      return TRUE;
    return FALSE;
  }

__inline AEFOLD* AEC_NextFold(AEFOLD *lpFold, BOOL bRecursive)
  {
    if (lpFold)
    {
      if (bRecursive)
      {
        if (lpFold->firstChild)
          return lpFold->firstChild;
      }

      do
      {
        if (lpFold->next)
          return lpFold->next;
      }
      while (lpFold=lpFold->parent);
    }
    return lpFold;
  }

__inline AEFOLD* AEC_PrevFold(AEFOLD *lpFold, BOOL bRecursive)
  {
    if (lpFold)
    {
      if (bRecursive)
      {
        if (lpFold->lastChild)
          return lpFold->lastChild;
      }

      do
      {
        if (lpFold->prev)
          return lpFold->prev;
      }
      while (lpFold=lpFold->parent);
    }
    return lpFold;
  }
#else
  int AEC_CopyChar(wchar_t *wszTarget, DWORD dwTargetSize, const wchar_t *wpSource);
  int AEC_IndexInc(AECHARINDEX *ciChar);
  int AEC_IndexDec(AECHARINDEX *ciChar);
  int AEC_IndexLen(AECHARINDEX *ciChar);
  int AEC_IndexCompare(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2);
  int AEC_IndexCompareEx(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2);
  AELINEDATA* AEC_NextLine(AECHARINDEX *ciChar);
  AELINEDATA* AEC_PrevLine(AECHARINDEX *ciChar);
  AELINEDATA* AEC_NextLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  AELINEDATA* AEC_PrevLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  AELINEDATA* AEC_NextChar(AECHARINDEX *ciChar);
  AELINEDATA* AEC_PrevChar(AECHARINDEX *ciChar);
  AELINEDATA* AEC_NextCharEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  AELINEDATA* AEC_PrevCharEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  AELINEDATA* AEC_NextCharInLine(AECHARINDEX *ciChar);
  AELINEDATA* AEC_PrevCharInLine(AECHARINDEX *ciChar);
  AELINEDATA* AEC_NextCharInLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  AELINEDATA* AEC_PrevCharInLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  AELINEDATA* AEC_ValidCharInLine(AECHARINDEX *ciChar);
  int AEC_WrapLineBegin(AECHARINDEX *ciChar);
  int AEC_WrapLineEnd(AECHARINDEX *ciChar);
  int AEC_WrapLineBeginEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  int AEC_WrapLineEndEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut);
  int AEC_CharAtIndex(const AECHARINDEX *ciChar);
  BOOL AEC_IsCharInSelection(const AECHARINDEX *ciChar);
  BOOL AEC_IsFirstCharInLine(const AECHARINDEX *ciChar);
  BOOL AEC_IsLastCharInLine(const AECHARINDEX *ciChar);
  BOOL AEC_IsFirstCharInFile(const AECHARINDEX *ciChar);
  BOOL AEC_IsLastCharInFile(const AECHARINDEX *ciChar);
  AEFOLD* AEC_NextFold(AEFOLD *lpFold, BOOL bRecursive);
  AEFOLD* AEC_PrevFold(AEFOLD *lpFold, BOOL bRecursive);
#endif //AEC_FUNCTIONS

#endif //__AKELEDIT_H__
