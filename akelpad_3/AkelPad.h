#ifndef __AKELPAD_H__
#define __AKELPAD_H__

#include "AkelFiles\Langs\Resources\resource.h"

#define AKDN_MAIN_ONSTART          (WM_USER + 1)
#define AKDN_MAIN_ONSTART_PRESHOW  (WM_USER + 2)
#define AKDN_MAIN_ONSTART_SHOW     (WM_USER + 3)
#define AKDN_MAIN_ONSTART_FINISH   (WM_USER + 4)
#define AKDN_MAIN_ONFINISH         (WM_USER + 5)
#define AKDN_EDIT_ONSTART          (WM_USER + 6)
#define AKDN_EDIT_ONFINISH         (WM_USER + 7)
#define AKDN_CONTEXTMENU           (WM_USER + 8)
#define AKDN_FRAME_NOWINDOWS       (WM_USER + 9)
#define AKDN_OPENDOCUMENT_START    (WM_USER + 10)
#define AKDN_OPENDOCUMENT_FINISH   (WM_USER + 11)
#define AKDN_SAVEDOCUMENT_START    (WM_USER + 12)
#define AKDN_SAVEDOCUMENT_FINISH   (WM_USER + 13)
#define AKDN_SIZE                  (WM_USER + 14)
#define AKDN_SEARCH_ENDED          (WM_USER + 15)
#define AKDN_ACTIVATE              (WM_USER + 16)
#define AKDN_RECENTFILESDELETE     (WM_USER + 17)
#define AKDN_MAIN_ONSTART_IDLE     (WM_USER + 18)
#define AKDN_DOCK_GETMINMAXINFO    (WM_USER + 19)
#define AKDN_DLLCALL               (WM_USER + 20)
#define AKDN_DLLUNLOAD             (WM_USER + 21)
#define AKDN_FRAME_ACTIVATE        (WM_USER + 22)

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
#define AKD_BEGINOPTIONS           (WM_USER + 113)
#define AKD_OPTION                 (WM_USER + 114)
#define AKD_ENDOPTIONS             (WM_USER + 115)
#define AKD_DLLCALL                (WM_USER + 116)
#define AKD_DLLUNLOAD              (WM_USER + 117)
#define AKD_DLLFIND                (WM_USER + 118)
#define AKD_DLLADD                 (WM_USER + 119)
#define AKD_DLLDELETE              (WM_USER + 120)
#define AKD_SAVEDOCUMENT           (WM_USER + 121)
#define AKD_GETTEXTLENGTH          (WM_USER + 122)
#define AKD_GETTEXTRANGE           (WM_USER + 123)
#define AKD_FREETEXT               (WM_USER + 124)
#define AKD_REPLACESELA            (WM_USER + 125)
#define AKD_REPLACESELW            (WM_USER + 126)
#define AKD_PASTE                  (WM_USER + 127)
#define AKD_COPY                   (WM_USER + 128)
#define AKD_TEXTFIND               (WM_USER + 129)
#define AKD_TEXTREPLACE            (WM_USER + 130)
#define AKD_GETEDITINFO            (WM_USER + 131)
#define AKD_GETFONT                (WM_USER + 132)
#define AKD_SETFONT                (WM_USER + 133)
#define AKD_SETMODIFY              (WM_USER + 134)
#define AKD_SETFILEPRINT           (WM_USER + 135)
#define AKD_SETMSGCREATE           (WM_USER + 136)
#define AKD_SETMSGBINARY           (WM_USER + 137)
#define AKD_GETQUEUE               (WM_USER + 138)
#define AKD_GETPRINTDLG            (WM_USER + 139)
#define AKD_GETPAGEDLG             (WM_USER + 140)
#define AKD_GETSELTEXTW            (WM_USER + 141)
#define AKD_GLOBALALLOC            (WM_USER + 142)
#define AKD_GLOBALLOCK             (WM_USER + 143)
#define AKD_GLOBALUNLOCK           (WM_USER + 144)
#define AKD_GLOBALFREE             (WM_USER + 145)
#define AKD_GETMODELESS            (WM_USER + 146)
#define AKD_SETMODELESS            (WM_USER + 147)
#define AKD_GETCODEPAGELIST        (WM_USER + 148)
#define AKD_RECODESEL              (WM_USER + 149)
#define AKD_SETNEWLINE             (WM_USER + 150)
#define AKD_GETRECENTFILES         (WM_USER + 151)
#define AKD_CREATEWINDOW           (WM_USER + 152)
#define AKD_GETMAININFO            (WM_USER + 153)
#define AKD_WAITKEYBOARD           (WM_USER + 154)
#define AKD_CALLPROC               (WM_USER + 155)
#define AKD_DLLSAVE                (WM_USER + 156)
#define AKD_RESIZE                 (WM_USER + 157)
#define AKD_DOCK                   (WM_USER + 158)
#define AKD_POSTMESSAGE            (WM_USER + 159)
#define AKD_GETCHARCOLOR           (WM_USER + 160)

#define TAB_HEIGHT           24
#define TAB_WIDTH            90

#define TAB_VIEW_NONE         0
#define TAB_VIEW_TOP          1
#define TAB_VIEW_BOTTOM       2

#define TAB_TYPE_STANDARD     0
#define TAB_TYPE_BUTTONS      1
#define TAB_TYPE_FLATBUTTONS  2

#define TAB_SWITCH_NEXTPREV   0
#define TAB_SWITCH_RIGHTLEFT  1

#define STATUS_POSITION       0
#define STATUS_MODIFY         1
#define STATUS_INSERT         2
#define STATUS_NEWLINE        3
#define STATUS_CODEPAGE       4
#define STATUS_PARTS          5

#define DOCK_MAINMIN_X       30
#define DOCK_MAINMIN_Y       30
#define DOCK_BORDER_1X        3
#define DOCK_BORDER_2X       (DOCK_BORDER_1X * 2)

#define DOCK_SIZING           1
#define DOCK_DRAGDROP         2

#define ID_EDIT           10001
#define ID_STATUS         10002
#define ID_TAB            10003
#define ID_FIRSTMDI       10010

LRESULT CALLBACK CommonMainProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonMainProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessagesA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessagesW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonFrameProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonFrameProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonEditProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CommonEditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMdiClientProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMdiClientProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewTabProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewTabProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DockProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DockProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DockMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DummyProcA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DummyProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
