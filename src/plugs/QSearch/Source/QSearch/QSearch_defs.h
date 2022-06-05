#ifndef _akel_qsearch_defs_h_
#define _akel_qsearch_defs_h_
//---------------------------------------------------------------------------

#define  OPTF_SRCH_ONTHEFLY_MODE       0
#define  OPTF_SRCH_FROM_BEGINNING      1
#define  OPTF_SRCH_USE_SPECIALCHARS    2
#define  OPTF_SRCH_USE_REGEXP          3
#define  OPTF_SRCH_REGEXP_DOT_NEWLINE  4
#define  OPTF_SRCH_PICKUP_SELECTION    5
#define  OPTF_SRCH_SELFIND_PICKUP      6
#define  OPTF_SRCH_STOP_EOF            7
#define  OPTF_SRCH_WND_DOCKEDTOP       8
#define  OPTF_EDIT_FOCUS_SELECTALL     9
#define  OPTF_CATCH_MAIN_F3           10
#define  OPTF_CATCH_MAIN_ESC          11
#define  OPTF_HOTKEY_HIDES_PANEL      12
#define  OPTF_EDITOR_AUTOFOCUS_MOUSE  13
#define  OPTF_QSEARCH_AUTOFOCUS_MOUSE 14
#define  OPTF_QSEARCH_AUTOFOCUS_FILE  15
#define  OPTF_DOCK_RECT_DISABLED      16
#define  OPTF_COUNT                   17

#define  OPT_DOCK_RECT                17
#define  OPT_COLOR_NOTFOUND           18
#define  OPT_COLOR_NOTREGEXP          19
#define  OPT_COLOR_EOF                20
#define  OPT_COLOR_HIGHLIGHT          21
#define  OPT_HIGHLIGHT_MARK_ID        22
#define  OPT_HIGHLIGHT_STATE          23
#define  OPT_USE_ALT_HOTKEYS          24
#define  OPT_ALT_MATCHCASE            25
#define  OPT_ALT_WHOLEWORD            26
#define  OPT_ALT_SEARCHMODE           27
#define  OPT_ALT_HIGHLIGHTALL         28
#define  OPT_FIND_HISTORY_ITEMS       29
#define  OPT_HISTORY_SAVE             30
#define  OPT_NEW_UI                   31
#define  OPT_SELECT_BY_F3             32
#define  OPT_SELECT_BY_FND            33
#define  OPT_SELECT_BY_SELFND         34
#define  OPT_ADJ_INCOMPL_REGEXP       35
#define  OPT_FINDALL_MODE             36
#define  OPT_FINDALL_RESULT           37
#define  OPT_FINDALL_COUNT_DELAY      38

#define  OPT_LOGOUTPUT_FRP_MODE       39
#define  OPT_LOGOUTPUT_FRP_BEFORE     40
#define  OPT_LOGOUTPUT_FRP_AFTER      41
#define  OPT_LOGOUTPUT_FRP_HIGHLIGHT  42
#define  OPT_FILEOUTPUT_FRP_MODE      43
#define  OPT_FILEOUTPUT_FRP_BEFORE    44
#define  OPT_FILEOUTPUT_FRP_AFTER     45
#define  OPT_FILEOUTPUT_FRP_HIGHLIGHT 46
#define  OPT_EDIT_MINWIDTH            47
#define  OPT_EDIT_MAXWIDTH            48

#define  OPT_TOTALCOUNT               49

#define  PICKUP_SEL_ALWAYS             1
#define  PICKUP_SEL_IF_NOT_QSBTN      11

#define  STOP_EOF_WITH_MSG             1
#define  STOP_EOF_WITHOUT_MSG         11

#define  HLS_IS_CHECKED                1
#define  HLS_SET_ALWAYS               10

#define  ESC_CATCH_FROM_MAIN           1
#define  ESC_THROW_TO_MAIN            10

#define  HOTKEY_HIDES_PANEL_ALWAYS     1
#define  HOTKEY_HIDES_PANEL_FOCUSED   11

#define  AUTOFOCUS_EDITOR              1
#define  AUTOFOCUS_QSEARCH            10
#define  AUTOFOCUS_EDITOR_ALWAYS     100

#define  QS_UI_ORIGINAL                0
#define  QS_UI_NEW_01                  1
#define  QS_UI_NEW_02                  2

#define  QS_FINDALL_AUTO_COUNT         0
#define  QS_FINDALL_COUNTONLY          1
#define  QS_FINDALL_LOGOUTPUT          2
#define  QS_FINDALL_FILEOUTPUT_MULT    3
#define  QS_FINDALL_FILEOUTPUT_SNGL    4
#define  QS_FINDALL_TOTAL_TYPES        5 // Only "Find All" types
#define  QS_FINDALL_SHOWLINE           5
#define  QS_FINDALL_SHOWMATCHONLY      6
#define  QS_FINDALL_FILTERMODE         7
#define  QS_FINDALL_SETTINGSDLG        8
#define  QS_FINDALL_TOTAL              9 // All items of the "Find All" menu
#define  QS_FINDALL_MASK               0x000F
#define  QS_FINDALL_AUTO_COUNT_FLAG    0x0100

#define  QS_FINDALL_RSLT_WHOLELINE     0x0000
#define  QS_FINDALL_RSLT_MATCHONLY     0x0001
#define  QS_FINDALL_RSLT_FILTERMODE    0x0002
#define  QS_FINDALL_RSLT_POS           0x0010
#define  QS_FINDALL_RSLT_LEN           0x0020
#define  QS_FINDALL_RSLT_SEARCHING     0x0100
#define  QS_FINDALL_RSLT_OCCFOUND      0x0200
#define  QS_FINDALL_RSLT_CODERALIAS    0x1000
#define  QS_FINDALL_RSLT_ALLFILES      0x8000

//---------------------------------------------------------------------------
#endif
