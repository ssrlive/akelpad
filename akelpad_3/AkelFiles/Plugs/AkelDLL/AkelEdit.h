#ifndef __AKELEDIT_H__
#define __AKELEDIT_H__


//// Defines

#define AES_AKELEDITCLASSA     "AkelEditA"
#define AES_AKELEDITCLASSW    L"AkelEditW"
#define AES_RICHEDITCLASSA     "RichEdit20A"
#define AES_RICHEDITCLASSW    L"RichEdit20W"

#define AES_WORDDELIMITERSW   L" \t\n\\|[](){}<>,.;:+-=~!@#$%^&*/?'`\""
#define AES_WRAPDELIMITERSW   L" \t\n"

//AEM_SETOPTIONS flags
#define AECO_READONLY           0x00000001  //Set read-only mode. You can use ES_READONLY window style.
#define AECO_DISABLENOSCROLL    0x00000002  //Disables scroll bars instead of hiding them when they are not needed. You can use ES_DISABLENOSCROLL window style.
#define AECO_WANTRETURN         0x00000004  //If you do not specify this style, pressing the ENTER key has the same effect as pressing the dialog box's default push button. You can use ES_WANTRETURN window style.
#define AECO_DETAILEDUNDO       0x00000008  //The control stores any typing action, into a new action in the undo queue.
#define AECO_DISABLEBEEP        0x00000010  //Disables sound beep, when unallowable action occur.
#define AECO_PASTESELECTCOLUMN  0x00000020  //Selects pasted text in column mode.
#define AECO_DISABLEDRAG        0x00000040  //Disables OLE text dragging.
#define AECO_DISABLEDROP        0x00000080  //Disables OLE text dropping.

#define AECOOP_SET              0  //Sets the options to those specified by lParam.
#define AECOOP_OR               1  //Combines the specified options with the current options.
#define AECOOP_AND              2  //Retains only those current options that are also specified by lParam.
#define AECOOP_XOR              3  //Logically exclusive OR the current options with those specified by lParam.

//AEM_GETINDEX flags
#define AEGI_FIRSTCHAR          1   //First character
#define AEGI_LASTCHAR           2   //Last character
#define AEGI_FIRSTSELCHAR       3   //First character of the selection
#define AEGI_LASTSELCHAR        4   //Last character of the selection
#define AEGI_CARETCHAR          5   //Caret character
#define AEGI_FIRSTVISIBLELINE   6   //First character of the first visible line
#define AEGI_LASTVISIBLELINE    7   //Last character of the last visible line
#define AEGI_NEXTLINE           8   //First character of the next line
#define AEGI_PREVLINE           9   //First character of the previous line
#define AEGI_NEXTCHAR           10  //Next character
#define AEGI_PREVCHAR           11  //Previous character
#define AEGI_NEXTBREAK          12  //Next break index, see AEM_SETWORDDELIMITERS
#define AEGI_PREVBREAK          13  //Previous break index, see AEM_SETWORDDELIMITERS
#define AEGI_WRAPLINEBEGIN      14  //First character of the unwrapped line
#define AEGI_WRAPLINEEND        15  //Last character of the unwrapped line

//AEM_CHARFROMPOS return value
#define AEPC_ERROR    0  //Error
#define AEPC_EQUAL    1  //Char exactly in specified position
#define AEPC_BEFORE   2  //Char before the specified position
#define AEPC_AFTER    3  //Char after the specified position

//New line value
#define AELB_ASIS     1  //Use new line of the source
#define AELB_ASINPUT  2  //Use input new line, see AEM_SETNEWLINE with AENL_INPUT
#define AELB_ASOUTPUT 3  //Use output new line, see AEM_SETNEWLINE with AENL_OUTPUT
#define AELB_EOF      4  //End-of-file new line
#define AELB_R        5  //"\r" new line
#define AELB_N        6  //"\n" new line
#define AELB_RN       7  //"\r\n" new line
#define AELB_RRN      8  //"\r\r\n" new line
#define AELB_WRAP     9  //No new line, this line is wrapped

//AEM_SETNEWLINE flags
#define AENL_INPUT           0x00000001  //Sets default new line for the input operations, for example AEM_PASTE
#define AENL_OUTPUT          0x00000002  //Sets default new line for the output operations, for example AEM_COPY

//AEM_SETCOLORS flags
#define AECLR_DEFAULT        0x00000001  //Use default system colors for the specified flags, all members of the AECOLORS structure are ignored.
#define AECLR_CARET          0x00000002  //Sets caret color. crCaret member is valid.
#define AECLR_BASICTEXT      0x00000004  //Sets basic text color. crBasicText member is valid.
#define AECLR_BASICBK        0x00000008  //Sets basic background color. crBasicBk member is valid.
#define AECLR_SELTEXT        0x00000010  //Sets text in selection color. crSelText member is valid.
#define AECLR_SELBK          0x00000020  //Sets background in selection color. crSelBk member is valid.
#define AECLR_ACTIVELINETEXT 0x00000040  //Sets active line text color. crActiveLineText member is valid.
#define AECLR_ACTIVELINEBK   0x00000080  //Sets active line background color. crActiveLineBk member is valid.
#define AECLR_ALL            (AECLR_CARET         |\
                              AECLR_BASICTEXT     |\
                              AECLR_BASICBK       |\
                              AECLR_SELTEXT       |\
                              AECLR_SELBK         |\
                              AECLR_ACTIVELINETEXT|\
                              AECLR_ACTIVELINEBK)

//AEM_SCROLLCARETTEST, AEM_SCROLLCARET flags
#define AECS_UNITPIXELX      0x00000001  //Low word of the lParam specifies pixels number
#define AECS_UNITPIXELY      0x00000002  //High word of the lParam specifies pixels number
#define AECS_UNITCHARX       0x00000004  //Low word of the lParam specifies characters number
#define AECS_UNITCHARY       0x00000008  //High word of the lParam specifies characters number
#define AECS_UNITRECTDIVX    0x00000010  //Low word of the lParam specifies divisor of the edit rect width
#define AECS_UNITRECTDIVY    0x00000020  //High word of the lParam specifies divisor of the edit rect width
#define AECS_FORCELEFT       0x00000040  //Scrolls to the left even if caret visible
#define AECS_FORCETOP        0x00000080  //Scrolls to the top even if caret visible
#define AECS_FORCERIGHT      0x00000100  //Scrolls to the right even if caret visible
#define AECS_FORCEBOTTOM     0x00000200  //Scrolls to the bottom even if caret visible

//AEM_SCROLLCARETTEST, AEM_SCROLLCARET return flags
#define AECSE_SCROLLEDX      0x00000001  //Edit control was horizontally scrolled
#define AECSE_SCROLLEDY      0x00000002  //Edit control was vertically scrolled

//AEM_FINDTEXTA, AEM_FINDTEXTW flags
#define AEFR_DOWN            0x00000001  //If set, the search is from the beginning to the end of the search range. If not set, the search is from the end to the beginning of the search range.
#define AEFR_WHOLEWORD       0x00000002  //If set, the operation searches only for whole words that match the search string. If not set, the operation also searches for word fragments that match the search string.
#define AEFR_MATCHCASE       0x00000004  //If set, the search operation is case-sensitive. If not set, the search operation is case-insensitive.

//AEM_SETWORDDELIMITERS flags
#define AEWB_LEFTWORDSTART   0x00000001  //Left movement is stopped, when word start is found.
#define AEWB_LEFTWORDEND     0x00000002  //Left movement is stopped, when word end is found.
#define AEWB_RIGHTWORDSTART  0x00000004  //Right movement is stopped, when word start is found.
#define AEWB_RIGHTWORDEND    0x00000008  //Right movement is stopped, when word end is found.

//AEM_STREAMIN, AEM_STREAMOUT flags
#define AESF_SELECTION       0x00000001  //Stream the current selection
#define AESF_COLUMNSEL       0x00000002  //Stream the current column selection
#define AESF_FILLSPACES      0x00000004  //Stream the current column selection with the filling empty spaces

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
#ifndef SPI_GETWHEELSCROLLLINES
  #define SPI_GETWHEELSCROLLLINES 0x0068
#endif
#ifndef WC_NO_BEST_FIT_CHARS
  #define WC_NO_BEST_FIT_CHARS 0x00000400
#endif
#ifndef WM_MOUSEWHEEL
  #define WM_MOUSEWHEEL 0x020A
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
#ifndef mod
  #define mod(a) (((a) < 0)?(0 - (a)):(a))
#endif

typedef BOOL (CALLBACK *AEStreamCallback)(DWORD dwCookie, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufDone);
//dwCookie    Value of the dwCookie member of the AESTREAM structure. The application specifies this value when it sends the EM_STREAMIN or EM_STREAMOUT message.
//wszBuf      Pointer to a buffer to read from or write to. For a stream-in (read) operation, the callback function fills this buffer with data to transfer into the edit control. For a stream-out (write) operation, the buffer contains data from the control that the callback function writes to some storage.
//dwBufLen    Number of bytes to read or write.
//dwBufDone   Pointer to a variable that the callback function sets to the number of bytes actually read or written.
//
//Return Value
//The callback function returns zero to indicate success.
//
//Remarks
//The control continues to call the callback function until one of the following conditions occurs: 
//* The callback function returns a nonzero value. 
//* The callback function returns zero in the *dwBufDone parameter. 


//// Structures

#ifndef _HSTACK_STRUCT_
#define _HSTACK_STRUCT_
typedef struct {
  int first;
  int last;
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
  struct _AELINEDATA *next;   //Pointer to the next AELINEDATA structure
  struct _AELINEDATA *prev;   //Pointer to the previous AELINEDATA structure
  wchar_t *wpLine;            //Text of the line, terminated with NULL character
  int nLineLen;               //Length of the wpLine, not including the terminating NULL character
  int nLineBreak;             //New line: AELB_EOF, AELB_R, AELB_N, AELB_RN, AELB_RRN or AELB_WRAP
  int nLineWidth;             //Width of the line in pixels
  int nSelStart;              //Selection start character position in line
  int nSelEnd;                //Selection end character position in line
} AELINEDATA;

typedef struct _AELINEINDEX {
  int nLine;                  //Line number in document
  AELINEDATA *lpLine;         //Pointer to the AELINEDATA structure
} AELINEINDEX;

typedef struct _AECHARINDEX {
  int nLine;                  //Line number in document
  AELINEDATA *lpLine;         //Pointer to the AELINEDATA structure
  int nCharInLine;            //Character position in line
} AECHARINDEX;

typedef struct {
  AECHARINDEX ciMin;  //First character index in range
  AECHARINDEX ciMax;  //Last character index in range
} AECHARRANGE;

typedef struct {
  AECHARRANGE crSel;  //Characters range
  BOOL bColumnSel;    //Column selection
} AESELECTION;

typedef struct {
  char *pText;        //Text to append
  DWORD dwTextLen;    //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  BOOL bColumnSel;    //Column selection
} AEAPPENDTEXTA;

typedef struct {
  wchar_t *wpText;    //Text to append
  DWORD dwTextLen;    //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  BOOL bColumnSel;    //Column selection
} AEAPPENDTEXTW;

typedef struct {
  char *pText;                 //Text to replace with
  DWORD dwTextLen;             //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  BOOL bColumnSel;             //Column selection
  AECHARINDEX *ciInsertStart;  //Insert "from" character index after replacement
  AECHARINDEX *ciInsertEnd;    //Insert "to" character index after replacement
} AEREPLACESELA;

typedef struct {
  wchar_t *wpText;             //Text to replace with
  DWORD dwTextLen;             //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  BOOL bColumnSel;             //Column selection
  AECHARINDEX *ciInsertStart;  //Insert "from" character index after replacement
  AECHARINDEX *ciInsertEnd;    //Insert "to" character index after replacement
} AEREPLACESELW;

typedef struct {
  AECHARRANGE cr;    //Characters range to retrieve
  BOOL bColumnSel;   //Column selection
  char *pText;       //Pointer to buffer that receives the text
  int nNewLine;      //see AELB_* defines
} AETEXTRANGEA;

typedef struct {
  AECHARRANGE cr;    //Characters range to retrieve
  BOOL bColumnSel;   //Column selection
  wchar_t *wpText;   //Pointer to buffer that receives the text
  int nNewLine;      //see AELB_* defines
} AETEXTRANGEW;

typedef struct {
  DWORD dwCookie;               //Specifies an application-defined value that the edit control passes to the AEStreamCallback function specified by the lpCallback member.
  DWORD dwError;                //Indicates the results of the stream-in (read) or stream-out (write) operation.
  AEStreamCallback lpCallback;  //Pointer to an AEStreamCallback function, which is an application-defined function that the control calls to transfer data. The control calls the callback function repeatedly, transferring a portion of the data with each call.
  int nNewLine;                 //see AELB_* defines
  BOOL bColumnSel;              //Column selection
} AESTREAM;

typedef struct {
  DWORD dwFlags;           //See AEFR_* defines
  AECHARRANGE crSearch;    //Range of characters to search
  char *pText;             //Text to find
  DWORD dwTextLen;         //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;            //See AELB_* defines
  AECHARRANGE crFound;     //Range of characters in which text is found
} AEFINDTEXTA;

typedef struct {
  DWORD dwFlags;           //See AEFR_* defines
  AECHARRANGE crSearch;    //Range of characters to search
  wchar_t *wpText;         //Text to find
  DWORD dwTextLen;         //Text length. If this value is –1, the string is assumed to be null-terminated and the length is calculated automatically.
  int nNewLine;            //See AELB_* defines
  AECHARRANGE crFound;     //Range of characters in which text is found
} AEFINDTEXTW;

typedef struct {
  DWORD dwFlags;             //See AECLR_* defines
  COLORREF crCaret;          //Caret color
  COLORREF crBasicText;      //Basic text color
  COLORREF crBasicBk;        //Basic background color
  COLORREF crSelText;        //Text color in selection
  COLORREF crSelBk;          //Background color in selection
  COLORREF crActiveLineText; //Text color in active line
  COLORREF crActiveLineBk;   //Background color in active line
} AECOLORS;

typedef struct {
  AECHARINDEX *ciCharIn;   //Input character index
  AECHARINDEX *ciCharOut;  //Output character index (result)
  int nOffset;             //Offset can be positive or negative. For example, +1 will return next character, -1 will return previous character
  int nNewLine;            //See AELB_* defines
} AEINDEXOFFSET;

typedef struct {
  AECHARINDEX *ciChar1;   //First character index
  AECHARINDEX *ciChar2;   //Second character index
  int nNewLine;           //See AELB_* defines
  BOOL bColumnSel;        //Column selection
} AEINDEXSUBTRACT;

typedef struct {
  NMHDR hdr;
  AESELECTION aes;      //Current selection
  AECHARINDEX ciCaret;  //Caret character index position
} AENSELCHANGE;

typedef struct {
  NMHDR hdr;
} AENTEXTCHANGE;

typedef struct {
  NMHDR hdr;
  BOOL bModified;  //TRUE document state is set to modified, TRUE document state is set to unmodified
} AENMODIFYCHANGE;


//// Messages

#define AEN_SELCHANGE         (WM_USER + 1001)
#define AEN_TEXTCHANGE        (WM_USER + 1002)
#define AEN_MODIFYCHANGE      (WM_USER + 1003)
#define AEN_ERRSPACE          (WM_USER + 1004)

#define AEM_SETTEXTA          (WM_USER + 2001)
#define AEM_SETTEXTW          (WM_USER + 2002)
#define AEM_APPENDTEXTA       (WM_USER + 2003)
#define AEM_APPENDTEXTW       (WM_USER + 2004)
#define AEM_REPLACESELA       (WM_USER + 2005)
#define AEM_REPLACESELW       (WM_USER + 2006)
#define AEM_GETTEXTRANGEA     (WM_USER + 2007)
#define AEM_GETTEXTRANGEW     (WM_USER + 2008)
#define AEM_STREAMIN          (WM_USER + 2009)
#define AEM_STREAMOUT         (WM_USER + 2010)
#define AEM_CANPASTE          (WM_USER + 2011)
#define AEM_PASTE             (WM_USER + 2012)
#define AEM_CUT               (WM_USER + 2013)
#define AEM_COPY              (WM_USER + 2014)
#define AEM_FINDTEXTA         (WM_USER + 2015)
#define AEM_FINDTEXTW         (WM_USER + 2016)
#define AEM_CHECKCODEPAGE     (WM_USER + 2017)

#define AEM_CANUNDO           (WM_USER + 2051)
#define AEM_CANREDO           (WM_USER + 2052)
#define AEM_UNDO              (WM_USER + 2053)
#define AEM_REDO              (WM_USER + 2054)
#define AEM_EMPTYUNDOBUFFER   (WM_USER + 2055)
#define AEM_BEGINUNDOACTION   (WM_USER + 2056)
#define AEM_ENDUNDOACTION     (WM_USER + 2057)
#define AEM_LOCKCOLLECTUNDO   (WM_USER + 2058)
#define AEM_GETUNDOLIMIT      (WM_USER + 2059)
#define AEM_SETUNDOLIMIT      (WM_USER + 2060)
#define AEM_GETMODIFY         (WM_USER + 2061)
#define AEM_SETMODIFY         (WM_USER + 2062)

#define AEM_GETSEL            (WM_USER + 2101)
#define AEM_SETSEL            (WM_USER + 2102)
#define AEM_GETCOLUMNSEL      (WM_USER + 2103)
#define AEM_UPDATESEL         (WM_USER + 2104)
#define AEM_GETLINECOUNT      (WM_USER + 2105)
#define AEM_GETINDEX          (WM_USER + 2106)
#define AEM_GETLINEINDEX      (WM_USER + 2107)
#define AEM_UPDATEINDEX       (WM_USER + 2108)
#define AEM_INDEXCOMPARE      (WM_USER + 2109)
#define AEM_INDEXSUBTRACT     (WM_USER + 2110)
#define AEM_INDEXOFFSET       (WM_USER + 2111)
#define AEM_INDEXTORICHOFFSET (WM_USER + 2112)
#define AEM_RICHOFFSETTOINDEX (WM_USER + 2113)

#define AEM_CHARFROMPOS       (WM_USER + 2151)
#define AEM_POSFROMCHAR       (WM_USER + 2152)
#define AEM_GETRECT           (WM_USER + 2153)
#define AEM_SETRECT           (WM_USER + 2154)
#define AEM_GETSCROLLPOS      (WM_USER + 2155)
#define AEM_SETSCROLLPOS      (WM_USER + 2156)
#define AEM_SCROLL            (WM_USER + 2157)
#define AEM_LINESCROLL        (WM_USER + 2158)
#define AEM_SCROLLCARETTEST   (WM_USER + 2159)
#define AEM_SCROLLCARET       (WM_USER + 2160)
#define AEM_LOCKSCROLL        (WM_USER + 2161)

#define AEM_GETOPTIONS        (WM_USER + 2201)
#define AEM_SETOPTIONS        (WM_USER + 2202)
#define AEM_GETNEWLINE        (WM_USER + 2203)
#define AEM_SETNEWLINE        (WM_USER + 2204)
#define AEM_GETCOLORS         (WM_USER + 2205)
#define AEM_SETCOLORS         (WM_USER + 2206)
#define AEM_GETOVERTYPE       (WM_USER + 2207)
#define AEM_SETOVERTYPE       (WM_USER + 2208)
#define AEM_GETTABSTOP        (WM_USER + 2209)
#define AEM_SETTABSTOP        (WM_USER + 2210)
#define AEM_GETWORDWRAP       (WM_USER + 2211)
#define AEM_SETWORDWRAP       (WM_USER + 2212)
#define AEM_GETWORDDELIMITERS (WM_USER + 2213)
#define AEM_SETWORDDELIMITERS (WM_USER + 2214)
#define AEM_GETWRAPDELIMITERS (WM_USER + 2215)
#define AEM_SETWRAPDELIMITERS (WM_USER + 2216)
#define AEM_SHOWSCROLLBAR     (WM_USER + 2217)

#endif
