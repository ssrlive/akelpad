#define _WIN32_IE 0x0400

#include <windows.h>
#include <tchar.h>
#define _RICHEDIT_VER 0x0200
#include <richedit.h>
#include <commctrl.h>
#include <stdio.h>
#include "akelpad.h"
#include "edit.h"
#include "codepage.h"
#include "langpack.h"

extern TCHAR g_szCurrentFileName[MAX_PATH+1]; 
extern BOOL keep_on;
extern BOOL need_autodetect;
extern BOOL open_in_new_window;
extern BOOL g_unix_newline;
extern BOOL support_asian;

extern TCHAR g_recent_files[RECENTFILES][MAX_PATH+1];
extern DWORD g_recent_files_position[RECENTFILES];

TCHAR *CmdLine;
UINT uiOpenOnStartMessage;
HWND g_hStatus;
BOOL g_modified=FALSE;
BOOL insertstate=TRUE;
BOOL GlobalPrint=FALSE;
int g_iCmdShow;
int g_max_chars=0x0FFFFFFF;
int mcstep=0x0FFFFFFF;

HMENU g_hFileMenu;

RECT MainWindowRect;

BOOL (WINAPI *GetCPInfoExPtr) (UINT,DWORD,LPCPINFOEX);

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PTSTR szCmdLine, int iCmdShow)
{
    TCHAR *szAppName = APP_FULL_TITLE;
    HACCEL hAccel;
    HWND         hWnd;
    MSG          msg;
    WNDCLASS     wndclass;
    HINSTANCE hRTFLib;
    BOOL msgstatus;
    HMODULE hKernel32;

    UNREFERENCED_PARAMETER(hPrev);

    hKernel32=LoadLibrary(_T("kernel32.dll"));
#if defined(UNICODE)
    GetCPInfoExPtr=(int(WINAPI *)(UINT,DWORD,LPCPINFOEX))GetProcAddress(hKernel32,"GetCPInfoExW");
#else
    GetCPInfoExPtr=(int(WINAPI *)(UINT,DWORD,LPCPINFOEX))GetProcAddress(hKernel32,"GetCPInfoExA");
#endif // defined(UNICODE)

    CmdLine=szCmdLine;
    g_iCmdShow=iCmdShow;

    MainWindowRect.left=CW_USEDEFAULT;
    MainWindowRect.top=CW_USEDEFAULT;
    MainWindowRect.right=CW_USEDEFAULT;
    MainWindowRect.bottom=CW_USEDEFAULT;

    hRTFLib=LoadLibrary(_T("RICHED32.DLL"));
    if(!hRTFLib) {
        MessageBox(NULL,MSG_ERROR_LOADING_DLL,APP_SHORT_TITLE,MB_OK|MB_ICONERROR);
        return 0;
    }

    uiOpenOnStartMessage=RegisterWindowMessage(_T("AkelPad Open-On-Start message"));

    InitCommonControls();

    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground =(HBRUSH) GetStockObject(HOLLOW_BRUSH);
    wndclass.lpszMenuName  = MAKEINTRESOURCE(IDM_APPMENU);
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);

    RestorePreOptionsFromRegistry();

    hWnd = CreateWindow(szAppName,                    // window class name
        APP_SHORT_TITLE,              // window caption
        WS_OVERLAPPEDWINDOW,          // window style
        MainWindowRect.left,                // initial x position
        MainWindowRect.top,                // initial y position
        MainWindowRect.right,                // initial x size
        MainWindowRect.bottom,                // initial y size
        NULL,                         // parent window handle
        NULL,                         // window menu handle
        hInstance,                    // program instance handle
        NULL);                        // creation parameters

    hAccel=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDM_APPMENU));
    UpdateWindow(hWnd);

    msgstatus=FALSE;

    while((msgstatus=GetMessage(&msg, NULL, 0, 0)) && msgstatus!=-1) {
        if(!TranslateAccelerator(hWnd,hAccel,&msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    if (msgstatus==-1) {
        MessageBox(NULL,MSG_ERROR_IN_MESSAGE_QUEUE,APP_SHORT_TITLE,MB_OK|MB_ICONERROR);
    }
    FreeLibrary(hRTFLib);

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndEdit;
    static HMENU hPopup,hMenu;
    static int iSBParts[]={100,200,300,-1};
    RECT ClientRect,StatusRect;
    CHARRANGE chrg;
    LPARAM lMenuState;
    DWORD dwLong;
    POINT point;

    TCHAR tmpbuf[256],*p;
    TCHAR *spacebuf;
    int line,column;
    int linelength;

    TCHAR szFullFileName[MAX_PATH];
    HWND hWndFriend;

    if(message==uiOpenOnStartMessage) {
        RestoreOptionsFromRegistry(hWndEdit);
        if(lstrlen(CmdLine)!=0) {
            if ((lstrlen(CmdLine)>=4) &&
                (CmdLine[0]==_T('/')) &&
                ((CmdLine[1]==_T('p'))||(CmdLine[1]==_T('P'))))
            {
                GlobalPrint=TRUE;
                CmdLine+=3;
            }
            if(CmdLine[0]==_T('\"')) {
                CmdLine[lstrlen(CmdLine)-1]=_T('\0');
                lstrcpyn(g_szCurrentFileName,CmdLine+1,MAX_PATH);
            }
            else lstrcpyn(g_szCurrentFileName,CmdLine,MAX_PATH);

            if(!open_in_new_window&&GetFullPathName(g_szCurrentFileName,MAX_PATH,szFullFileName,&p)) {
                hWndFriend=FindWindow(STR_DUMMY_NAME,szFullFileName);
                if(hWndFriend) {
                    hWndFriend=GetParent(hWndFriend);
                    if(hWndFriend) {
                        SetForegroundWindow(hWndFriend);
                        PostQuitMessage(0);
                        return 0;
                    }
                }
            }

            ShowWindow(hWnd, g_iCmdShow);

            need_autodetect=TRUE;
            OpenDocument(hWndEdit,FALSE);
            need_autodetect=FALSE;
        } else { 
            ShowWindow(hWnd, g_iCmdShow);
        }
        return 0;
    }

    switch(message)
    {
    case WM_CREATE:
        hMenu=GetMenu(hWnd);
        g_hFileMenu=GetSubMenu(hMenu,0);
        hPopup=LoadMenu(((LPCREATESTRUCT)lParam)->hInstance,MAKEINTRESOURCE(IDM_POPUP));
        hPopup=GetSubMenu(hPopup,0);

        GetClientRect(hWnd, &ClientRect);

        g_hStatus=CreateWindow(STATUSCLASSNAME,
            _T(""),
            WS_CHILD|WS_VISIBLE|SBARS_SIZEGRIP,
            0,0,0,0,
            hWnd,
            (HMENU)ID_STATUS,
            ((LPCREATESTRUCT)lParam)->hInstance,NULL);

        SendMessage(g_hStatus,SB_SIMPLE,FALSE,0);
        SendMessage(g_hStatus,SB_SETPARTS,4,(LPARAM)&iSBParts);
        SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)_T("1:1"));
        SendMessage(g_hStatus,SB_SETTEXT,1,(LPARAM)STR_MODE_INSERT);
        SendMessage(g_hStatus,SB_SETTEXT,2,(LPARAM)_T(""));

        ChangeCodePage(GetDefultCodePage());

        hWndEdit=CreateWindow(RICHEDIT_CLASS,
            NULL,
            WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|
            /*WS_BORDER|*/ES_LEFT|ES_MULTILINE|
            ES_AUTOHSCROLL|ES_AUTOVSCROLL|
            ES_SAVESEL|ES_DISABLENOSCROLL|
            ES_SUNKEN|ES_NOHIDESEL,
            0,
            0,
            ClientRect.right-ClientRect.left,
            ClientRect.bottom-ClientRect.top,
            hWnd,
            (HMENU)ID_EDIT,
            ((LPCREATESTRUCT)lParam)->hInstance,NULL);

        SendMessage(hWndEdit,EM_EXLIMITTEXT,0,g_max_chars);

        InitEditor(hWndEdit);

        ShowWindow(hWnd,SW_HIDE);

        DragAcceptFiles(hWnd,TRUE);

        //   RenewHistoryMenu();

        PostMessage(hWnd,uiOpenOnStartMessage,0,0);
        break;

    case WM_MENUSELECT:
    case WM_INITMENU:
    case WM_INITMENUPOPUP:
        //Check if any file is opened
        EnableMenuItem(hMenu,IDM_FILE_REOPEN,(*g_szCurrentFileName)?MF_ENABLED:MF_GRAYED);
        //Check if "Save" option should be enabled
        EnableMenuItem(hMenu,IDM_FILE_SAVE,(GetModify()||!g_szCurrentFileName[0])?MF_ENABLED:MF_GRAYED);
        //Check if the selection is empty
        SendMessage(hWndEdit,EM_EXGETSEL,0,(LPARAM)&chrg);
        if(chrg.cpMin==chrg.cpMax) lMenuState=MF_GRAYED;
        else lMenuState=MF_ENABLED;
        EnableMenuItem(hMenu,IDM_EDIT_CUT,lMenuState);
        EnableMenuItem(hMenu,IDM_EDIT_COPY,lMenuState);
        EnableMenuItem(hMenu,IDM_EDIT_CLEAR,lMenuState);
        //Check if paste, undo, redo is allowed
        EnableMenuItem(hMenu,IDM_EDIT_UNDO,SendMessage(hWndEdit,EM_CANUNDO,0,0)?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenu,IDM_EDIT_REDO,(SendMessage(hWndEdit,EM_CANREDO,0,0))?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMenu,IDM_EDIT_PASTE,SendMessage(hWndEdit,EM_CANPASTE,0,0)?MF_ENABLED:MF_GRAYED);
        //Set up Windows/Unix newline format selection
        if (g_unix_newline) {
            CheckMenuItem(hMenu,IDM_CODEPAGE_NEWLINE_UNIX,MF_CHECKED);
            CheckMenuItem(hMenu,IDM_CODEPAGE_NEWLINE_WIN,MF_UNCHECKED);
        } else { 
            CheckMenuItem(hMenu,IDM_CODEPAGE_NEWLINE_UNIX,MF_UNCHECKED);
            CheckMenuItem(hMenu,IDM_CODEPAGE_NEWLINE_WIN,MF_CHECKED);
        }
        return 0;

    case WM_SETFOCUS:
        SetFocus(hWndEdit);
        return 0;

    case WM_SIZE:
        SendMessage(g_hStatus,WM_SIZE,wParam,lParam);
        GetWindowRect(g_hStatus, &StatusRect);

        MoveWindow(hWndEdit,0,0,LOWORD(lParam),HIWORD(lParam)-(StatusRect.bottom-StatusRect.top),TRUE);
        return 0;

    case WM_DROPFILES:
        if(DoFileNew(hWndEdit)) {
            DragQueryFile((HDROP)wParam,0,g_szCurrentFileName,MAX_PATH);
            need_autodetect=TRUE;
            OpenDocument(hWndEdit,FALSE);
            need_autodetect=FALSE;
        }
        return 0;

    case WM_COMMAND:
        if((LOWORD(wParam)>=IDM_HISTORY_BASE)&&(LOWORD(wParam)<=IDM_HISTORY_BASE+1000)) {
            if(LOWORD(wParam)-IDM_HISTORY_BASE<=RECENTFILES-1) {
                DoFileOpenHistory(hWndEdit,LOWORD(wParam)-IDM_HISTORY_BASE);
            }
            return 0;
        }
        switch(LOWORD(wParam)) {
        case ID_EDIT:
            switch(HIWORD(wParam)) {
            case EN_CHANGE:
                SetModify(TRUE);
                break;
            case EN_MAXTEXT:
                //trying to reallocate
                MessageBox(hWnd,MSG_ERROR_LIMIT_REACHED,APP_SHORT_TITLE,MB_OK|MB_ICONERROR);
                g_max_chars+=mcstep;
                SendMessage(hWndEdit,EM_EXLIMITTEXT,0,g_max_chars);
                break;
            case EN_ERRSPACE:
                MessageBox(hWnd,MSG_ERROR_NOT_ENOUGH_MEMORY_FOR_RICHEDIT,APP_SHORT_TITLE,MB_OK|MB_ICONERROR);
                ExitProcess(1);
                break;
            }
            break;
        case IDM_FILE_NEW:
            DoFileNew(hWndEdit);
            break;
        case IDM_FILE_CREATENEW:
            SaveOptionsInRegistry(hWnd);
            DoFileNewWindow();
            break;
        case IDM_FILE_OPEN:
            DoFileOpen(hWndEdit);
            break;
        case IDM_FILE_REOPEN:
            DoFileReopen(hWndEdit);
            break;
        case IDM_FILE_SAVE:
            DoFileSave(hWndEdit);
            break;
        case IDM_FILE_SAVEAS:
            DoFileSaveAs(hWndEdit);
            break;
        case IDM_FILE_PAGESETUP:
            DoFilePageSetup(hWndEdit);
            break;
        case IDM_FILE_PRINT:
            DoFilePrint(hWndEdit);
            break;
        case IDM_FILE_EXIT:
            DoFileExit(hWndEdit);
            break;

        case IDM_EDIT_UNDO:
            DoEditUndo(hWndEdit);
            break;
        case IDM_EDIT_REDO:
            DoEditRedo(hWndEdit);
            break;
        case IDM_EDIT_CUT:
            DoEditCut(hWndEdit);
            break;
        case IDM_EDIT_COPY:
            DoEditCopy(hWndEdit);
            break;
        case IDM_EDIT_PASTE:
            DoEditPaste(hWndEdit);
            break;
        case IDM_EDIT_CLEAR:
            DoEditClear(hWndEdit);
            break;
        case IDM_EDIT_SELECTALL:
            DoEditSelectAll(hWndEdit);
            break;
        case IDM_EDIT_INSERTCHAR:
            DoEditInsertChar(hWndEdit);
            break;
        case IDM_EDIT_WRAP:
            DoEditWrap(hWndEdit);
            break;
        case IDM_EDIT_KEEPSPACE:
            DoEditKeepSpace(hWndEdit);
            break;

        case IDM_SEARCH_FIND:
            DoSearchFind(hWndEdit);
            break;
        case IDM_SEARCH_FINDNEXT:
            DoSearchFindNext(hWndEdit);
            break;
        case IDM_SEARCH_FINDOTHER:
            DoSearchFindOther(hWndEdit);
            break;
        case IDM_SEARCH_REPLACE:
            DoSearchReplace(hWndEdit);
            break;
        case IDM_SEARCH_GOTOLINE:
            DoSearchGoToLine(hWndEdit);
            break;

        case IDM_SETTINGS_FONT:
            DoSettingsFont(hWndEdit);
            break;
        case IDM_SETTINGS_BG:
            DoSettingsBG(hWndEdit);
            break;
        case IDM_SETTINGS_NOMULTIOPEN:
            DoSettingsNoMultiOpen(hWndEdit);
            SaveOptionsInRegistry(hWnd);
            break;
        case IDM_SETTINGS_ASIAN:
            DoSettingsAsian(hWndEdit);
            break;

        case IDM_CODEPAGE_OPENAS_ANSI:
            ChangeCodePage(GetDefultCodePage());
            DoCodePageOpenAs(hWndEdit);
            break;
        case IDM_CODEPAGE_OPENAS_OEM:
            ChangeCodePage(DOS_866);
            DoCodePageOpenAs(hWndEdit);
            break;
        case IDM_CODEPAGE_OPENAS_KOI:
            ChangeCodePage(KOI8_R);
            DoCodePageOpenAs(hWndEdit);
            break;
        case IDM_CODEPAGE_OPENAS_UNICODE_UCS2_LE:
            ChangeCodePage(CP_UNICODE_UCS2_LE);
            DoCodePageOpenAs(hWndEdit);
            break;
        case IDM_CODEPAGE_OPENAS_UNICODE_UCS2_BE:
            ChangeCodePage(CP_UNICODE_UCS2_BE);
            DoCodePageOpenAs(hWndEdit);
            break;
        case IDM_CODEPAGE_OPENAS_UNICODE_UTF8:
            ChangeCodePage(CP_UNICODE_UTF8);
            DoCodePageOpenAs(hWndEdit);
            break;
        case IDM_CODEPAGE_OPENAS:
            if(SelectCodePage(hWnd)) DoCodePageOpenAs(hWndEdit);
            break;
        case IDM_CODEPAGE_SAVEIN_ANSI:
            ChangeCodePage(GetDefultCodePage());
            DoCodePageSaveIn(hWndEdit);
            break;
        case IDM_CODEPAGE_SAVEIN_OEM:
            ChangeCodePage(DOS_866);
            DoCodePageSaveIn(hWndEdit);
            break;
        case IDM_CODEPAGE_SAVEIN_KOI:
            ChangeCodePage(KOI8_R);
            DoCodePageSaveIn(hWndEdit);
            break;
        case IDM_CODEPAGE_SAVEIN_UNICODE_UCS2_LE:
            ChangeCodePage(CP_UNICODE_UCS2_LE);
            DoCodePageSaveIn(hWndEdit);
            break;
        case IDM_CODEPAGE_SAVEIN_UNICODE_UCS2_BE:
            ChangeCodePage(CP_UNICODE_UCS2_BE);
            DoCodePageSaveIn(hWndEdit);
            break;
        case IDM_CODEPAGE_SAVEIN_UNICODE_UTF8:
            ChangeCodePage(CP_UNICODE_UTF8);
            DoCodePageSaveIn(hWndEdit);
            break;     
        case IDM_CODEPAGE_SAVEIN:
            if(SelectCodePage(hWnd)) DoCodePageSaveIn(hWndEdit);
            break;
        case IDM_CODEPAGE_NEWLINE_UNIX:
            if(!g_unix_newline) SetModify(TRUE);
            g_unix_newline=TRUE;
            break;
        case IDM_CODEPAGE_NEWLINE_WIN:
            if(g_unix_newline) SetModify(TRUE);
            g_unix_newline=FALSE;
            break;

        case IDM_HELP_ABOUT:
            DoHelpAbout(hWndEdit);
            break;

        case IDM_NONMENU_CHANGESIZE:
            dwLong=GetWindowLongPtr(hWnd,GWL_STYLE);
            if(dwLong&WS_MAXIMIZE) ShowWindow(hWnd,SW_RESTORE);
            else ShowWindow(hWnd,SW_MAXIMIZE);
            break;
        case IDM_NONMENU_DELLINE:
            DoNonMenuDelLine(hWndEdit);
            break;
        case IDM_NONMENU_TAB:
            DoNonMenuTab(hWndEdit,TRUE,TRUE);
            break;
        case IDM_NONMENU_SHIFTTAB:
            DoNonMenuTab(hWndEdit,TRUE,FALSE);
            break;
        case IDM_NONMENU_SPACE:
            DoNonMenuTab(hWndEdit,FALSE,TRUE);
            break;
        case IDM_NONMENU_SHIFTSPACE:
            DoNonMenuTab(hWndEdit,FALSE,FALSE);
            break;
        }
        return 0;

    case WM_NOTIFY:
        //Some unmaintainable code is here
        if(wParam==ID_STATUS) {
            if(((LPNMHDR)lParam)->code==(UINT)NM_CLICK) {
                switch(((LPNMMOUSE)lParam)->dwItemSpec) {
                case 1:
                    SendMessage(hWndEdit,WM_KEYDOWN,VK_INSERT,0x00520001);
                    SendMessage(hWndEdit,WM_KEYUP,VK_INSERT,0xC0520001);
                    break;
                case 0:
                    DoSearchGoToLine(hWndEdit);
                    break;
                }
            }    
        }
        else if(wParam==ID_EDIT) {
            if(((LPNMHDR)lParam)->code==EN_SELCHANGE) {
                SendMessage(hWndEdit,EM_EXGETSEL,0,(LPARAM)&chrg);

                line=SendMessage(hWndEdit,EM_EXLINEFROMCHAR,0,chrg.cpMin)+1;

                column=SendMessage(hWndEdit,EM_LINEINDEX,line-1,0); //column=index of first character in that string
                column=chrg.cpMin-column+1;

                if(line+1==SendMessage(hWndEdit,EM_GETLINECOUNT,0,0)&&column==SendMessage(hWndEdit,EM_LINELENGTH,chrg.cpMin,0)+2) {
                    line++;
                    column=1;
                }

                _stprintf(tmpbuf,_T("%u:%u"),line,column);
                SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)tmpbuf);
            }
            else if(((LPNMHDR)lParam)->code==EN_MSGFILTER) { //Space-keeping mode and other related stuff
                if(((MSGFILTER *)lParam)->msg==WM_KEYUP&&((MSGFILTER *)lParam)->wParam==VK_RETURN&&keep_on) {
                    return -1;
                }
                else if(((MSGFILTER *)lParam)->msg==WM_KEYDOWN&&((MSGFILTER *)lParam)->wParam==VK_RETURN&&keep_on) {
                    SendMessage(hWndEdit,EM_EXGETSEL,0,(LPARAM)&chrg);

                    line=SendMessage(hWndEdit,EM_EXLINEFROMCHAR,0,chrg.cpMin);
                    column=SendMessage(hWndEdit,EM_LINEINDEX,line,0); //column=index of first character in that string
                    column=chrg.cpMin-column+1;

                    if(line+2==SendMessage(hWndEdit,EM_GETLINECOUNT,0,0)&&column==SendMessage(hWndEdit,EM_LINELENGTH,chrg.cpMin,0)+2) {
                        return 0;
                    }

                    linelength=SendMessage(hWndEdit,EM_LINELENGTH,(chrg.cpMin-1>=0)?chrg.cpMin:0,0);

                    spacebuf=(TCHAR *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY, (linelength+lstrlen(_T("\n"))+1+2)*sizeof(TCHAR));
                    if(!spacebuf) {
                        MessageBox(NULL,MSG_ERROR_NOT_ENOUGH_MEMORY,NULL,MB_OK|MB_ICONERROR);
                        ExitProcess(1);
                    }

                    lstrcpy(spacebuf, _T("\n"));
                    *(WORD*)&spacebuf[lstrlen(_T("\n"))]=linelength+1;
                    SendMessage(hWndEdit,EM_GETLINE,line,(LPARAM)(spacebuf+lstrlen(_T("\n"))));
                    for(p=spacebuf+lstrlen(_T("\n"));(*p==_T(' ')||*p==0x09)&&*p&&column>1;p++,column--) { ; }
                    *p=_T('\0');

                    SendMessage(hWndEdit,EM_REPLACESEL,TRUE,(LPARAM)spacebuf);
                    HeapFree(GetProcessHeap(),0,spacebuf);

                    return -1;
                }
                else if(((MSGFILTER *)lParam)->msg==WM_KEYDOWN&&((MSGFILTER *)lParam)->wParam==VK_INSERT) {
                    if(GetKeyState(VK_SHIFT)>=0&&GetKeyState(VK_CONTROL)>=0) {
                        insertstate=!insertstate;
                        SendMessage(g_hStatus,SB_SETTEXT,1,(LPARAM)(insertstate?STR_MODE_INSERT:STR_MODE_OVERLAP));
                    }
                }
                else if(((MSGFILTER *)lParam)->msg==WM_RBUTTONUP) {
                    point.x=LOWORD(((MSGFILTER *)lParam)->lParam);
                    point.y=HIWORD(((MSGFILTER *)lParam)->lParam);
                    ClientToScreen(hWnd,&point);

                    //Check if the selection is empty
                    SendMessage(hWndEdit,EM_EXGETSEL,0,(LPARAM)&chrg);
                    if(chrg.cpMin==chrg.cpMax) lMenuState=MF_GRAYED;
                    else lMenuState=MF_ENABLED;
                    EnableMenuItem(hPopup,IDM_EDIT_CUT,lMenuState);
                    EnableMenuItem(hPopup,IDM_EDIT_COPY,lMenuState);
                    EnableMenuItem(hPopup,IDM_EDIT_CLEAR,lMenuState);
                    //Check if paste, undo, redo is allowed
                    EnableMenuItem(hPopup,IDM_EDIT_UNDO,SendMessage(hWndEdit,EM_CANUNDO,0,0)?MF_ENABLED:MF_GRAYED);
                    EnableMenuItem(hPopup,IDM_EDIT_REDO,SendMessage(hWndEdit,EM_CANREDO,0,0)?MF_ENABLED:MF_GRAYED);
                    EnableMenuItem(hPopup,IDM_EDIT_PASTE,SendMessage(hWndEdit,EM_CANPASTE,0,0)?MF_ENABLED:MF_GRAYED);

                    TrackPopupMenu(hPopup,
                        TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
                        point.x,
                        point.y,
                        0,
                        hWnd,
                        NULL);
                    return -1;
                }

            }
        }
        return 0;

    case WM_CLOSE:
        DoFileExit(hWndEdit);
        return 0;

    case WM_DESTROY:
        if(hWndEdit) DestroyWindow(hWndEdit);
        SaveOptionsInRegistry(hWnd);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd,message,wParam,lParam);
}

BOOL GetModify() {
    return g_modified;
}

void SetModify(BOOL state) {
    g_modified=state;
    SendMessage(g_hStatus,SB_SETTEXT,2,(LPARAM)(g_modified?STR_MODIFIED:_T("")));
}

int RenewHistoryMenu() {
    int i=0;
    int added=0;
    int res;
    TCHAR buf[HISTORY_STRING_LENGTH+1];
    HMENU hHistoryMenu;
    
    hHistoryMenu=GetSubMenu(g_hFileMenu, HISTORY_MENU_POSITION);

    while(TRUE) {
        res=DeleteMenu(hHistoryMenu,i,MF_BYPOSITION);
        if(!res) {
            break;
        }
    }

    for(i=0; i<RECENTFILES; i++) {
        if(*g_recent_files[i]) {
            //if(*g_szCurrentFileName) {
            //    if(!lstrcmp(g_szCurrentFileName, g_recent_files[i])) continue;
            //}
            if(lstrlen(g_recent_files[i])<=HISTORY_STRING_LENGTH) {
                lstrcpy(buf, g_recent_files[i]);
            } else {
                lstrcpy(buf,_T("..."));
                lstrcpy(buf+3, g_recent_files[i]+lstrlen(g_recent_files[i])-HISTORY_STRING_LENGTH+3);
            }
            AppendMenu(hHistoryMenu,MF_STRING|MF_ENABLED,IDM_HISTORY_BASE+i,buf);
            added++;
        }
    }

    if(!added) {
        EnableMenuItem(g_hFileMenu,HISTORY_MENU_POSITION,MF_BYPOSITION|MF_GRAYED);
    } else {
        EnableMenuItem(g_hFileMenu,HISTORY_MENU_POSITION,MF_BYPOSITION|MF_ENABLED);
    }

    return 0;
}
