#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "RegExpFunc.h"
#include "AkelDLL.h"
#include "Coder.h"
#include "HighLight.h"
#include "CodeFold.h"
#include "AutoComplete.h"
#include "Resources\Resource.h"


//Global variables
PLUGINFUNCTION *pfCodeFold=NULL;
HWND hWndCodeFoldDlg=NULL;
HWND hWndCodeFoldList=NULL;
HWND hWndCodeFoldFilter=NULL;
WNDPROC lpOldFilterProc=NULL;
RECT rcCodeFoldCurrentDialog={0};
RECT rcCodeFoldDockRect={0};
int nCodeFoldDockSide=DKS_RIGHT;
DOCK *dkCodeFoldDlg=NULL;
BOOL bCodeFoldDockWaitResize=FALSE;
BOOL bCodeFoldDialogRectSave=FALSE;
STACKFOLDWINDOW hFoldWindowsStack={0};
FOLDWINDOW *lpCurrentFoldWindow=NULL;
BOOL bFocusResetList=FALSE;
BOOL bResetFold=FALSE;
BOOL bIgnoreFilter=FALSE;
FRAMEDATA *lpLastPostUpdateFrame=NULL;
DWORD dwLastPostUpdateFlags=0;
int nCharsLimit=1000000;
int nFoldLimit=10000;
int nShowDock=CFSD_AUTO;
int nFollowCaret=FCO_ANYWHERE;
BOOL bFoldListSystemColors=FALSE;
BOOL bFoldListSystemFont=TRUE;
BOOL bNoPrintCollapsed=FALSE;
BOOL bCollapseOnOpen=FALSE;
BOOL bTagMarkEnable=TRUE;
int nDrawNodeType=DNT_ROUND;
int nFindRootMaxDepth=0;
HCURSOR hCursorArrow=NULL;
DWORD dwFoldListTextColor;
DWORD dwFoldListBkColor;
DWORD dwCurrentCollapse=699;  //"Ctrl+="
DWORD dwCurrentGoBegin=731;   //"Ctrl+["
DWORD dwCurrentGoEnd=733;     //"Ctrl+]"
DWORD dwCurrentSelect=732;    //"Ctrl+\"
DWORD dwAllCollapse=1723;     //"Ctrl+Alt+="
DWORD dwAllNextLevel=1757;    //"Ctrl+Alt+]"
DWORD dwAllPrevLevel=1755;    //"Ctrl+Alt+["
DWORD dwFindRootLevel=123;    //"F12"
//DWORD dwFindRootLevel=703;    //"Ctrl+/"
PLUGINFUNCTION *pfwCurrentCollapse=NULL;
PLUGINFUNCTION *pfwCurrentGoBegin=NULL;
PLUGINFUNCTION *pfwCurrentGoEnd=NULL;
PLUGINFUNCTION *pfwCurrentSelect=NULL;
PLUGINFUNCTION *pfwAllCollapse=NULL;
PLUGINFUNCTION *pfwAllNextLevel=NULL;
PLUGINFUNCTION *pfwAllPrevLevel=NULL;
PLUGINFUNCTION *pfwFindRootLevel=NULL;

//Plugin extern function
void __declspec(dllexport) CodeFold(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (pd->bAkelEdit)
  {
    if (!bInitCommon)
      InitCommon(pd);
    else if (!nInitCodeFold)
      ReadOptions(OF_CODEFOLD);
    pfCodeFold=pd->lpPluginFunction;

    if (pd->lParam)
    {
      INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

      if (nInitCodeFold)
      {
        if (nAction == DLLA_CODEFOLD_SHOWDOCK)
        {
          HWND *lpWndDock=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            lpWndDock=(HWND *)GetExtCallParam(pd->lParam, 2);

          if (lpWndDock)
          {
            if (dkCodeFoldDlg && !(dkCodeFoldDlg->dwFlags & DKF_HIDDEN))
              *lpWndDock=dkCodeFoldDlg->hWnd;
          }
          else SendMessage(hWndCodeFoldDlg, WM_COMMAND, IDC_CODEFOLD_HIDE, 0);
        }
        else if (nAction == DLLA_CODEFOLD_ADDWINDOW)
        {
          MANUALSET *lpManual;
          HWND hWndEdit=NULL;
          HWND hWndParent=NULL;
          unsigned char *pAlias=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            pAlias=(unsigned char *)GetExtCallParam(pd->lParam, 3);
          hWndParent=GetParent(hWndEdit);

          if (hWndEdit && hWndParent && pAlias)
          {
            //Subclass window
            if (!(lpManual=StackGetManual(&hManualStack, hWndEdit, NULL)))
            {
              if (lpManual=StackInsertManual(&hManualStack))
              {
                lpManual->hWndEdit=hWndEdit;
                lpManual->hDocEdit=(AEHDOC)SendMessage(hWndEdit, AEM_GETDOCUMENT, 0, 0);
                lpManual->hWndParent=hWndParent;

                lpManual->lpOldEditProc=(WNDPROC)GetWindowLongPtrWide(hWndEdit, GWLP_WNDPROC);
                SetWindowLongPtrWide(hWndEdit, GWLP_WNDPROC, (UINT_PTR)NewUserEditProc);

                lpManual->lpOldParentProc=(WNDPROC)GetWindowLongPtrWide(hWndParent, GWLP_WNDPROC);
                SetWindowLongPtrWide(hWndParent, GWLP_WNDPROC, (UINT_PTR)NewUserParentProc);
              }
            }

            if (lpManual)
            {
              if (bOldWindows)
                MultiByteToWideChar(CP_ACP, 0, (char *)pAlias, -1, lpManual->wszAlias, MAX_PATH);
              else
                xstrcpynW(lpManual->wszAlias, (wchar_t *)pAlias, MAX_PATH);

              //Add CodeFold element
              lpManual->dwDllFunction|=CODER_CODEFOLD;

              if (!StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
              {
                if (lpManual->lpFoldWindow=FillLevelsStack(NULL, NULL, hWndEdit, NULL))
                {
                  ((FOLDWINDOW *)lpManual->lpFoldWindow)->hWndEdit=lpManual->hWndEdit;
                  ((FOLDWINDOW *)lpManual->lpFoldWindow)->hDocEdit=lpManual->hDocEdit;
                  ((FOLDWINDOW *)lpManual->lpFoldWindow)->lpUser=lpManual;

                  if (nInitCodeFold)
                  {
                    SendMessage(hMainWnd, AKD_SETEDITNOTIFY, (LPARAM)hWndEdit, 0);
                    UpdateEdit(hWndEdit, UE_ALLRECT);
                  }
                }
              }
            }
          }
        }
        else if (nAction == DLLA_CODEFOLD_DELWINDOW)
        {
          MANUALSET *lpManual;
          HWND hWndEdit=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);

          if (hWndEdit)
          {
            if (lpManual=StackGetManual(&hManualStack, hWndEdit, NULL))
            {
              if (lpManual->lpFoldWindow && ((FOLDWINDOW *)lpManual->lpFoldWindow)->lpUser)
                StackDeleteManual(&hManualStack, lpManual, CODER_CODEFOLD);
            }
          }
        }
        else if (nAction == DLLA_CODEFOLD_GETWINDOW)
        {
          MANUALSET *lpManual;
          HWND hWndEdit=NULL;
          BOOL *lpbFound=NULL;
          BOOL bFound=FALSE;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            lpbFound=(BOOL *)GetExtCallParam(pd->lParam, 3);

          if (hWndEdit && lpbFound)
          {
            if (lpManual=StackGetManual(&hManualStack, hWndEdit, NULL))
            {
              if (lpManual->lpFoldWindow && ((FOLDWINDOW *)lpManual->lpFoldWindow)->lpUser)
                bFound=TRUE;
            }
            *lpbFound=bFound;
          }
        }
      }
      else
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_LOADFIRST), L"Coder::CodeFold");
        MessageBoxW(pd->hMainWnd, wszBuffer, L"Coder::CodeFold", MB_OK|MB_ICONEXCLAMATION);
      }

      //If plugin already loaded, stay in memory and don't change active status
      if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
      return;
    }

    //Initialize
    if (nInitCodeFold)
    {
      DestroyDock(hWndCodeFoldDlg, DKT_KEEPAUTOLOAD);

      //Stay in memory and show as non-active
      pd->nUnload=UD_NONUNLOAD_NONACTIVE;
    }
    else
    {
      InitMain();
      InitCodeFold();

      bCodeFoldDockWaitResize=pd->bOnStart;
      CreateDock(&hWndCodeFoldDlg, &dkCodeFoldDlg, FALSE);

      if (!pd->bOnStart)
      {
        if (pd->hWndEdit)
        {
          UpdateEditAll(UE_FIRSTPIXEL);
          lpCurrentFoldWindow=SetActiveEdit(pd->hWndEdit, hWndCodeFoldList, SAE_RESETLIST);
        }
      }

      //Stay in memory, and show as active
      pd->nUnload=UD_NONUNLOAD_ACTIVE;
    }
  }
  else
  {
    wLangModule=PRIMARYLANGID(pd->wLangModule);
    MessageBoxW(pd->hMainWnd, GetLangStringW(wLangModule, STRID_RICHEDIT_NOTSUPPORTED), L"Coder::CodeFold", MB_OK|MB_ICONEXCLAMATION);

    //If plugin already loaded, stay in memory, but show as non-active
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
}

void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow)
{
  DOCK dk;

  *hWndDock=NULL;

  xmemset(&dk, 0, sizeof(DOCK));
  dk.dwFlags=DKF_DRAGDROP;
  dk.nSide=nCodeFoldDockSide;
  dk.rcSize=rcCodeFoldDockRect;

  if (*dkDock=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_ADD, (LPARAM)&dk))
  {
    *hWndDock=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_CODEFOLD_LIST), hMainWnd, (DLGPROC)CodeFoldDockDlgProc);

    if (*hWndDock)
    {
      if (!(*dkDock)->rcSize.right || !(*dkDock)->rcSize.bottom)
      {
        GetWindowRect(*hWndDock, &(*dkDock)->rcSize);
        (*dkDock)->rcSize.right-=(*dkDock)->rcSize.left;
        (*dkDock)->rcSize.bottom-=(*dkDock)->rcSize.top;
        (*dkDock)->rcSize.left=0;
        (*dkDock)->rcSize.top=0;
      }
      (*dkDock)->hWnd=*hWndDock;
      SendMessage(hMainWnd, AKD_DOCK, DK_SUBCLASS, (LPARAM)*dkDock);

      if (bShow)
        SendMessage(hMainWnd, AKD_DOCK, DK_SHOW, (LPARAM)*dkDock);
      else
        (*dkDock)->dwFlags|=DKF_HIDDEN;
    }
  }
}

void DestroyDock(HWND hWndDock, DWORD dwType)
{
  SendMessage(hWndDock, WM_COMMAND, IDC_CODEFOLD_CLOSE, dwType);
}

BOOL CALLBACK CodeFoldDockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndTitleText;
  static HWND hWndTitleHide;
  static HWND hWndTitleClose;
  static HMENU hMenuLabel;
  static HMENU hMenuTreeView;
  static DIALOGRESIZE drs[]={{&hWndTitleText,      DRS_SIZE|DRS_X, 0},
                             {&hWndTitleHide,      DRS_MOVE|DRS_X, 0},
                             {&hWndTitleClose,     DRS_MOVE|DRS_X, 0},
                             {&hWndCodeFoldList,   DRS_SIZE|DRS_X, 0},
                             {&hWndCodeFoldList,   DRS_SIZE|DRS_Y, 0},
                             {&hWndCodeFoldFilter, DRS_MOVE|DRS_Y, 0},
                             {&hWndCodeFoldFilter, DRS_SIZE|DRS_X, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    hWndCodeFoldList=GetDlgItem(hDlg, IDC_CODEFOLD_LIST);
    hWndCodeFoldFilter=GetDlgItem(hDlg, IDC_CODEFOLD_FILTER);
    hWndTitleText=GetDlgItem(hDlg, IDC_CODEFOLD_TITLETEXT);
    hWndTitleHide=GetDlgItem(hDlg, IDC_CODEFOLD_HIDE);
    hWndTitleClose=GetDlgItem(hDlg, IDC_CODEFOLD_CLOSE);

    hMenuLabel=CreatePopupMenu();
    AppendMenuWide(hMenuLabel, MF_STRING, IDM_CODEFOLD_SETUP, GetLangStringW(wLangModule, STRID_SETUP));

    hMenuTreeView=CreatePopupMenu();
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_GOBEGIN, GetLangStringW(wLangModule, STRID_GOBEGIN));
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_GOEND, GetLangStringW(wLangModule, STRID_GOEND));
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_SELECT, GetLangStringW(wLangModule, STRID_SELECT));
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_INVIEW, GetLangStringW(wLangModule, STRID_INVIEW));
    AppendMenuWide(hMenuTreeView, MF_SEPARATOR, 0, NULL);
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_LISTEXPAND, GetLangStringW(wLangModule, STRID_LISTEXPAND));
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_LISTCOLLAPSE, GetLangStringW(wLangModule, STRID_LISTCOLLAPSE));
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_STATISTICS, GetLangStringW(wLangModule, STRID_STATISTICS));
    AppendMenuWide(hMenuTreeView, MF_SEPARATOR, 0, NULL);
    AppendMenuWide(hMenuTreeView, MF_STRING, IDM_CODEFOLD_SETUP, GetLangStringW(wLangModule, STRID_SETUP));
    SetMenuDefaultItem(hMenuTreeView, IDM_CODEFOLD_GOBEGIN, FALSE);

    //Set dock title
    {
      BUTTONDRAW bd;

      xprintfW(wszBuffer, L"%s::CodeFold", wszPluginName);
      SetWindowTextWide(hWndTitleText, wszBuffer);

      bd.dwFlags=BIF_DOWNARROW|BIF_ETCHED;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndTitleHide, (LPARAM)&bd);

      bd.dwFlags=BIF_CROSS|BIF_ETCHED;
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndTitleClose, (LPARAM)&bd);
    }

    lpOldFilterProc=(WNDPROC)GetWindowLongPtrWide(hWndCodeFoldFilter, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndCodeFoldFilter, GWLP_WNDPROC, (UINT_PTR)NewFilterProc);
  }
  else if (uMsg == AKDLL_SETUP)
  {
    SettingsSheet(PAGE_CODEFOLD1);
  }
  else if (uMsg == AKDLL_SETFOCUS)
  {
    if (lpCurrentFoldWindow)
    {
      SetFocus(lpCurrentFoldWindow->hWndEdit);
    }
  }
  else if (uMsg == AKDLL_UPDATE)
  {
    FRAMEDATA *lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_CURRENT, 0);

    if (lpLastPostUpdateFrame && lpLastPostUpdateFrame == (FRAMEDATA *)lParam && lpFrame == (FRAMEDATA *)lParam)
    {
      //UpdateEditAll(UE_FIRSTPIXEL);
      UpdateEdit(lpFrame->ei.hWndEdit, UE_FIRSTPIXEL);
      lpCurrentFoldWindow=SetActiveEdit(lpFrame->ei.hWndEdit, hWndCodeFoldList, dwLastPostUpdateFlags);
      lpLastPostUpdateFrame=NULL;
      dwLastPostUpdateFlags=0;
    }
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    POINT pt;
    RECT rc;

    GetCursorPos(&pt);
    GetWindowRect(hWndTitleText, &rc);

    if (PtInRect(&rc, pt))
    {
      PostMessage(hDlg, AKDLL_SETUP, 0, 0);
    }
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hDlg)
    {
      RECT rc;
      POINT pt;

      GetCursorPos(&pt);
      GetWindowRect(hWndTitleText, &rc);

      if (PtInRect(&rc, pt))
      {
        int nCmd;

        nCmd=TrackPopupMenu(hMenuLabel, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWndTitleText, NULL);

        if (nCmd == IDM_CODEFOLD_SETUP)
          PostMessage(hDlg, AKDLL_SETUP, 0, 0);
      }
    }
    else if ((HWND)wParam == hWndCodeFoldList)
    {
      TVHITTESTINFO tvhti;
      RECT rcItem;
      POINT pt;
      int nCmd;

      if (lpCurrentFoldWindow)
      {
        if (lParam == -1)
        {
          *(HTREEITEM *)&rcItem=(HTREEITEM)SendMessage(hWndCodeFoldList, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);
          SendMessage(hWndCodeFoldList, TVM_GETITEMRECT, TRUE, (LPARAM)&rcItem);
          pt.x=rcItem.left;
          pt.y=rcItem.bottom;
          ClientToScreen(hWndCodeFoldList, &pt);
        }
        else
        {
          GetCursorPos(&tvhti.pt);
          ScreenToClient(hWndCodeFoldList, &tvhti.pt);
          SendMessage(hWndCodeFoldList, TVM_HITTEST, 0, (LPARAM)&tvhti);
          if (tvhti.hItem && (tvhti.flags & TVHT_ONITEM))
            SendMessage(hWndCodeFoldList, TVM_SELECTITEM, TVGN_CARET, (LPARAM)tvhti.hItem);
          GetCursorPos(&pt);
        }

        nCmd=TrackPopupMenu(hMenuTreeView, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWndCodeFoldList, NULL);

        if (nCmd == IDM_CODEFOLD_GOBEGIN ||
            nCmd == IDM_CODEFOLD_GOEND ||
            nCmd == IDM_CODEFOLD_INVIEW)
        {
          AEFOLD *lpFold;

          if (lpFold=GetCurFoldTreeView(hWndCodeFoldList))
          {
            FoldInView(lpCurrentFoldWindow, lpFold, nCmd);
            PostMessage(hWndCodeFoldDlg, AKDLL_SETFOCUS, 0, 0);
          }
        }
        else if (nCmd == IDM_CODEFOLD_SELECT)
        {
          AEFOLD *lpFold;

          if (lpFold=GetCurFoldTreeView(hWndCodeFoldList))
          {
            if (FoldSelect(lpCurrentFoldWindow, lpFold))
              PostMessage(hWndCodeFoldDlg, AKDLL_SETFOCUS, 0, 0);
          }
        }
        else if (nCmd == IDM_CODEFOLD_LISTEXPAND)
        {
          ExpandTreeView(hWndCodeFoldList, lpCurrentFoldWindow, TRUE);
        }
        else if (nCmd == IDM_CODEFOLD_LISTCOLLAPSE)
        {
          ExpandTreeView(hWndCodeFoldList, lpCurrentFoldWindow, FALSE);
        }
        else if (nCmd == IDM_CODEFOLD_STATISTICS)
        {
          AEFOLD *lpFold;
          AEFOLD *lpStopFold;
          int nRootFoldCount=0;
          int nSelChildCount=-1;
          int nAllFoldCount;

          for (lpFold=(AEFOLD *)lpCurrentFoldWindow->pfwd->lpFoldStack->first; lpFold; lpFold=lpFold->next)
          {
            ++nRootFoldCount;
          }
          if (lpFold=GetCurFoldTreeView(hWndCodeFoldList))
          {
            for (lpStopFold=AEC_NextFold(lpFold, FALSE); lpFold != lpStopFold; lpFold=AEC_NextFold(lpFold, TRUE))
            {
              ++nSelChildCount;
            }
          }
          nAllFoldCount=(int)SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_GETFOLDCOUNT, 0, 0);

          xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_STATISTICS_MSG), nAllFoldCount, nRootFoldCount, nAllFoldCount - nRootFoldCount, nSelChildCount);
          MessageBoxW(hMainWnd, wszBuffer, L"Coder::CodeFold", MB_OK|MB_ICONINFORMATION);
        }
        else if (nCmd == IDM_CODEFOLD_SETUP)
        {
          PostMessage(hDlg, AKDLL_SETUP, 0, 0);
        }
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_CODEFOLD_FILTER)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        if (lpCurrentFoldWindow && !bIgnoreFilter)
        {
          GetWindowTextWide(hWndCodeFoldFilter, lpCurrentFoldWindow->pfwd->wszFilter, MAX_PATH);
          FillTreeView(hWndCodeFoldList, lpCurrentFoldWindow, lpCurrentFoldWindow->pfwd->wszFilter);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_CODEFOLD_HIDE ||
             LOWORD(wParam) == IDCANCEL)
    {
      if (dkCodeFoldDlg && !(dkCodeFoldDlg->dwFlags & DKF_HIDDEN))
      {
        nShowDock=CFSD_NONE;
      }
      else
      {
        if (lpCurrentFoldWindow && lpCurrentFoldWindow->pfwd->lpSyntaxFile)
          nShowDock=CFSD_AUTO;
        else
          nShowDock=CFSD_ALWAYS;
      }
      lpCurrentFoldWindow=SetActiveEdit(GetFocusEdit(), hWndCodeFoldList, SAE_RESETLIST|SAE_IGNOREMAXFOLDS);
      dwSaveFlags|=OF_CODEFOLD_SETTINGS;

      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      AEFOLD *lpFold;

      if (lpFold=GetCurFoldTreeView(hWndCodeFoldList))
      {
        FoldInView(lpCurrentFoldWindow, lpFold, IDM_CODEFOLD_GOBEGIN);
        PostMessage(hWndCodeFoldDlg, AKDLL_SETFOCUS, 0, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_CODEFOLD_CLOSE)
    {
      //Save OF_CODEFOLD_RECT
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      if (!(lParam & DKT_ONMAINFINISH))
        StackEndBoard(&hFoldWindowsStack, NULL);
      UninitMain();
      UninitCodeFold((DWORD)-1);
      SendMessage(hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkCodeFoldDlg);
      dkCodeFoldDlg=NULL;

      DestroyMenu(hMenuLabel);
      DestroyMenu(hMenuTreeView);
      DestroyWindow(hWndCodeFoldDlg);
      hWndCodeFoldDlg=NULL;

      if (!(lParam & DKT_ONMAINFINISH))
      {
        SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
        SetFocus(hMainWnd);

        if (!(lParam & DKT_KEEPAUTOLOAD))
        {
          pfCodeFold->bAutoLoad=FALSE;
          SendMessage(hMainWnd, AKD_DLLSAVE, DLLSF_ONEXIT, 0);
        }
        if ((lParam & DKT_NOUNLOAD) || nInitMain)
        {
          pfCodeFold->bRunning=FALSE;
          SendMessage(hMainWnd, WM_COMMAND, 0, 0);
        }
        else PostMessage(hMainWnd, AKD_DLLUNLOAD, (WPARAM)hInstanceDLL, (LPARAM)NULL);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
    {
      TVHITTESTINFO tvhti;

      if (lpCurrentFoldWindow)
      {
        GetCursorPos(&tvhti.pt);
        ScreenToClient(hWndCodeFoldList, &tvhti.pt);
        SendMessage(hWndCodeFoldList, TVM_HITTEST, 0, (LPARAM)&tvhti);
        if (tvhti.hItem && (tvhti.flags & TVHT_ONITEM))
        {
          AEFOLD *lpFold;

          if (lpFold=GetCurFoldTreeView(hWndCodeFoldList))
          {
            FoldInView(lpCurrentFoldWindow, lpFold, IDM_CODEFOLD_GOBEGIN);
            PostMessage(hWndCodeFoldDlg, AKDLL_SETFOCUS, 0, 0);
          }
          SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, 1);
          return 1;
        }
      }
    }
  }

  //Dialog resize messages
  {
    DIALOGRESIZEMSG drsm={&drs[0], NULL, &rcCodeFoldCurrentDialog, 0, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_DIALOGRESIZE, 0, (LPARAM)&drsm))
      if (dkCodeFoldDlg) GetWindowPos(hWndTitleText, hDlg, &dkCodeFoldDlg->rcDragDrop);
  }

  return FALSE;
}

LRESULT CALLBACK NewFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_DOWN || wParam == VK_UP)
    {
      if (GetFocus() != hWndCodeFoldList)
        SetFocus(hWndCodeFoldList);
      return SendMessage(hWndCodeFoldList, uMsg, wParam, lParam);
    }
  }
  return CallWindowProcWide(lpOldFilterProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK CodeFold1SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndShowDockAuto;
  static HWND hWndShowDockAlways;
  static HWND hWndShowDockNone;
  static HWND hWndShowDockFoldLimitLabel;
  static HWND hWndShowDockFoldLimit;
  static HWND hWndFollowCaretEverywhere;
  static HWND hWndFollowCaretOnlyRoot;
  static HWND hWndFollowCaretNone;
  static HWND hWndListSystemColors;
  static HWND hWndListSystemFont;
  static int nShowDockDlg;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndShowDockAuto=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_AUTO);
    hWndShowDockAlways=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_ALWAYS);
    hWndShowDockNone=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_NONE);
    hWndShowDockFoldLimitLabel=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_FOLDLIMIT_LABEL);
    hWndShowDockFoldLimit=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_FOLDLIMIT);
    hWndFollowCaretEverywhere=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_FOLLOWCARET_ANYWHERE);
    hWndFollowCaretOnlyRoot=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_FOLLOWCARET_ONLYROOT);
    hWndFollowCaretNone=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_FOLLOWCARET_NONE);
    hWndListSystemColors=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_LISTSYSTEMCOLORS);
    hWndListSystemFont=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_LISTSYSTEMFONT);

    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_GROUP, GetLangStringW(wLangModule, STRID_SHOWDOCK_GROUP));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_AUTO, GetLangStringW(wLangModule, STRID_AUTO));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_ALWAYS, GetLangStringW(wLangModule, STRID_ALWAYS));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_NONE, GetLangStringW(wLangModule, STRID_NONE));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_FOLDLIMIT_LABEL, GetLangStringW(wLangModule, STRID_FOLDLIMIT));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_FOLLOWCARET_GROUP, GetLangStringW(wLangModule, STRID_FOLLOWCARET_GROUP));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_FOLLOWCARET_ANYWHERE, GetLangStringW(wLangModule, STRID_ANYWHERE));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_FOLLOWCARET_ONLYROOT, GetLangStringW(wLangModule, STRID_ROOT));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_FOLLOWCARET_NONE, GetLangStringW(wLangModule, STRID_NONE));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_CHARSLIMIT_GROUP, GetLangStringW(wLangModule, STRID_MAXDOCUMENT));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_CHARSLIMIT_POSTLABEL, GetLangStringW(wLangModule, STRID_CHARS));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_LISTSYSTEMCOLORS, GetLangStringW(wLangModule, STRID_LISTSYSTEMCOLORS));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_LISTSYSTEMFONT, GetLangStringW(wLangModule, STRID_LISTSYSTEMFONT));

    nShowDockDlg=nShowDock;
    if (nShowDockDlg == CFSD_NONE)
      SendMessage(hWndShowDockNone, BM_SETCHECK, BST_CHECKED, 0);
    else if (nShowDockDlg == CFSD_ALWAYS)
      SendMessage(hWndShowDockAlways, BM_SETCHECK, BST_CHECKED, 0);
    else //if (nShowDockDlg == CFSD_AUTO)
      SendMessage(hWndShowDockAuto, BM_SETCHECK, BST_CHECKED, 0);
    SetDlgItemInt(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_FOLDLIMIT, nFoldLimit, FALSE);

    if (nFollowCaret == FCO_ANYWHERE)
      SendMessage(hWndFollowCaretEverywhere, BM_SETCHECK, BST_CHECKED, 0);
    else if (nFollowCaret == FCO_ONLYROOT)
      SendMessage(hWndFollowCaretOnlyRoot, BM_SETCHECK, BST_CHECKED, 0);
    else if (nFollowCaret == FCO_NONE)
      SendMessage(hWndFollowCaretNone, BM_SETCHECK, BST_CHECKED, 0);

    SetDlgItemInt(hDlg, IDC_CODEFOLD_SETUP_CHARSLIMIT, nCharsLimit, FALSE);

    if (bFoldListSystemColors)
      SendMessage(hWndListSystemColors, BM_SETCHECK, BST_CHECKED, 0);
    if (bFoldListSystemFont)
      SendMessage(hWndListSystemFont, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_CODEFOLD_SETUP_SHOWDOCK_AUTO ||
        LOWORD(wParam) == IDC_CODEFOLD_SETUP_SHOWDOCK_ALWAYS ||
        LOWORD(wParam) == IDC_CODEFOLD_SETUP_SHOWDOCK_NONE)
    {
      if (SendMessage(hWndShowDockNone, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nShowDockDlg=CFSD_NONE;
      else if (SendMessage(hWndShowDockAlways, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nShowDockDlg=CFSD_ALWAYS;
      else
        nShowDockDlg=CFSD_AUTO;

      EnableWindow(hWndShowDockFoldLimitLabel, (nShowDockDlg == CFSD_AUTO));
      EnableWindow(hWndShowDockFoldLimit, (nShowDockDlg == CFSD_AUTO));

      if (nShowDockDlg != nShowDock)
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }
    else if (LOWORD(wParam) == IDC_CODEFOLD_SETUP_FOLLOWCARET_ANYWHERE ||
             LOWORD(wParam) == IDC_CODEFOLD_SETUP_FOLLOWCARET_ONLYROOT ||
             LOWORD(wParam) == IDC_CODEFOLD_SETUP_FOLLOWCARET_NONE)
    {
      SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }
    else if (LOWORD(wParam) == IDC_CODEFOLD_SETUP_SHOWDOCK_FOLDLIMIT ||
             LOWORD(wParam) == IDC_CODEFOLD_SETUP_CHARSLIMIT)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_SETACTIVE)
    {
      if (nPropMaxVisitPage < PAGE_CODEFOLD1)
        nPropMaxVisitPage=PAGE_CODEFOLD1;
    }
    else if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      PSHNOTIFY *pshn=(PSHNOTIFY *)lParam;
      BOOL bState;

      if (nShowDockDlg != nShowDock)
      {
        nShowDock=nShowDockDlg;
        if (hWndCodeFoldDlg) PostMessage(hWndCodeFoldDlg, AKDLL_SETFOCUS, 0, 0);
      }
      nFoldLimit=GetDlgItemInt(hDlg, IDC_CODEFOLD_SETUP_SHOWDOCK_FOLDLIMIT, NULL, FALSE);

      if (SendMessage(hWndFollowCaretEverywhere, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nFollowCaret=FCO_ANYWHERE;
      else if (SendMessage(hWndFollowCaretOnlyRoot, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nFollowCaret=FCO_ONLYROOT;
      else if (SendMessage(hWndFollowCaretNone, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nFollowCaret=FCO_NONE;

      nCharsLimit=GetDlgItemInt(hDlg, IDC_CODEFOLD_SETUP_CHARSLIMIT, NULL, FALSE);

      bFoldListSystemColors=(BOOL)SendMessage(hWndListSystemColors, BM_GETCHECK, 0, 0);

      bState=(BOOL)SendMessage(hWndListSystemFont, BM_GETCHECK, 0, 0);
      if (bState != bFoldListSystemFont)
      {
        bFoldListSystemFont=bState;
        if (bFoldListSystemFont)
          SendMessage(hWndCodeFoldList, WM_SETFONT, (WPARAM)NULL, FALSE);
      }

      if (nInitCodeFold)
      {
        lpCurrentFoldWindow=SetActiveEdit(GetFocusEdit(), hWndCodeFoldList, SAE_RESETLIST);
      }

      if (pshn->lParam)
      {
        //OK button pressed
        bSaveOptions=TRUE;
      }
      else
      {
        //Apply button pressed
        if (nPropMaxVisitPage == PAGE_CODEFOLD1)
          UpdateAllOptions();
        return FALSE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK CodeFold2SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndDrawNodeSquare;
  static HWND hWndDrawNodeRound;
  static HWND hWndTagMark;
  static HWND hWndCollapseOnOpen;
  static HWND hWndNoPrintCollapsed;
  static HWND hWndFindRootHotkey;
  static HWND hWndFindRootDepth;
  static HWND hWndFindRootDepthSpin;
  static HWND hWndCurrentCollapse;
  static HWND hWndCurrentGoBegin;
  static HWND hWndCurrentGoEnd;
  static HWND hWndCurrentSelect;
  static HWND hWndAllCollapse;
  static HWND hWndAllNextLevel;
  static HWND hWndAllPrevLevel;
  static int nDrawNodeTypeDlg;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndDrawNodeSquare=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_DRAWNODE_SQUARE);
    hWndDrawNodeRound=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_DRAWNODE_ROUND);
    hWndTagMark=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_TAGMARK);
    hWndCollapseOnOpen=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_COLLAPSEONOPEN);
    hWndNoPrintCollapsed=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_NOPRINTCOLLAPSED);
    hWndFindRootHotkey=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_FINDROOTHOTKEY);
    hWndFindRootDepth=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_FINDROOTDEPTH_EDIT);
    hWndFindRootDepthSpin=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_FINDROOTDEPTH_SPIN);
    hWndCurrentCollapse=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_COLLAPSE);
    hWndCurrentGoBegin=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_GOBEGIN);
    hWndCurrentGoEnd=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_GOEND);
    hWndCurrentSelect=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_SELECT);
    hWndAllCollapse=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_HOTKEYALL_COLLAPSE);
    hWndAllNextLevel=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_HOTKEYALL_NEXTLEVEL);
    hWndAllPrevLevel=GetDlgItem(hDlg, IDC_CODEFOLD_SETUP_HOTKEYALL_PREVLEVEL);

    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_DRAWNODE_GROUP, GetLangStringW(wLangModule, STRID_DRAWNODE));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_DRAWNODE_SQUARE, GetLangStringW(wLangModule, STRID_SQUARE));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_DRAWNODE_ROUND, GetLangStringW(wLangModule, STRID_ROUND));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_TAGMARK, GetLangStringW(wLangModule, STRID_TAGMARK));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_COLLAPSEONOPEN, GetLangStringW(wLangModule, STRID_COLLAPSEONOPEN));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_NOPRINTCOLLAPSED, GetLangStringW(wLangModule, STRID_NOPRINTCOLLAPSED));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_FINDROOT_GROUP, GetLangStringW(wLangModule, STRID_FINDROOT));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_FINDROOTHOTKEY_LABEL, GetLangStringW(wLangModule, STRID_HOTKEY));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_FINDROOTDEPTH_LABEL, GetLangStringW(wLangModule, STRID_DEPTH));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_GROUP, GetLangStringW(wLangModule, STRID_HOTKEYCURRENT));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_COLLAPSE_LABEL, GetLangStringW(wLangModule, STRID_COLLAPSE));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_GOBEGIN_LABEL, GetLangStringW(wLangModule, STRID_GOBEGIN));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_GOEND_LABEL, GetLangStringW(wLangModule, STRID_GOEND));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYCURRENT_SELECT_LABEL, GetLangStringW(wLangModule, STRID_SELECT));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYALL_GROUP, GetLangStringW(wLangModule, STRID_HOTKEYALL));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYALL_COLLAPSE_LABEL, GetLangStringW(wLangModule, STRID_COLLAPSE));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYALL_NEXTLEVEL_LABEL, GetLangStringW(wLangModule, STRID_NEXTLEVEL));
    SetDlgItemTextWide(hDlg, IDC_CODEFOLD_SETUP_HOTKEYALL_PREVLEVEL_LABEL, GetLangStringW(wLangModule, STRID_PREVLEVEL));

    nDrawNodeTypeDlg=nDrawNodeType;
    if (nDrawNodeTypeDlg == DNT_ROUND)
      SendMessage(hWndDrawNodeRound, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndDrawNodeSquare, BM_SETCHECK, BST_CHECKED, 0);

    if (bTagMarkEnable)
      SendMessage(hWndTagMark, BM_SETCHECK, BST_CHECKED, 0);
    if (bCollapseOnOpen)
      SendMessage(hWndCollapseOnOpen, BM_SETCHECK, BST_CHECKED, 0);
    if (bNoPrintCollapsed)
      SendMessage(hWndNoPrintCollapsed, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hWndFindRootDepthSpin, UDM_SETBUDDY, (WPARAM)hWndFindRootDepth, 0);
    SendMessage(hWndFindRootDepthSpin, UDM_SETRANGE, 0, MAKELONG(99, 0));
    SetDlgItemInt(hDlg, IDC_CODEFOLD_SETUP_FINDROOTDEPTH_EDIT, nFindRootMaxDepth, FALSE);

    InitCodeFold();
    SendMessage(hWndFindRootHotkey, HKM_SETHOTKEY, dwFindRootLevel, 0);
    SendMessage(hWndCurrentCollapse, HKM_SETHOTKEY, dwCurrentCollapse, 0);
    SendMessage(hWndCurrentGoBegin, HKM_SETHOTKEY, dwCurrentGoBegin, 0);
    SendMessage(hWndCurrentGoEnd, HKM_SETHOTKEY, dwCurrentGoEnd, 0);
    SendMessage(hWndCurrentSelect, HKM_SETHOTKEY, dwCurrentSelect, 0);
    SendMessage(hWndAllCollapse, HKM_SETHOTKEY, dwAllCollapse, 0);
    SendMessage(hWndAllNextLevel, HKM_SETHOTKEY, dwAllNextLevel, 0);
    SendMessage(hWndAllPrevLevel, HKM_SETHOTKEY, dwAllPrevLevel, 0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndFindRootHotkey, pfwFindRootLevel?dwFindRootLevel:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCurrentCollapse, pfwCurrentCollapse?dwCurrentCollapse:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCurrentGoBegin, pfwCurrentGoBegin?dwCurrentGoBegin:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCurrentGoEnd, pfwCurrentGoEnd?dwCurrentGoEnd:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCurrentSelect, pfwCurrentSelect?dwCurrentSelect:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndAllCollapse, pfwAllCollapse?dwAllCollapse:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndAllNextLevel, pfwAllNextLevel?dwAllNextLevel:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndAllPrevLevel, pfwAllPrevLevel?dwAllPrevLevel:0);
    UninitCodeFold(INIT_CODEFOLD_HOTKEYS);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_CODEFOLD_SETUP_DRAWNODE_SQUARE ||
        LOWORD(wParam) == IDC_CODEFOLD_SETUP_DRAWNODE_ROUND)
    {
      if (SendMessage(hWndDrawNodeSquare, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDrawNodeTypeDlg=DNT_SQUARE;
      else if (SendMessage(hWndDrawNodeRound, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nDrawNodeTypeDlg=DNT_ROUND;

      if (nDrawNodeTypeDlg != nDrawNodeType)
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }
    else if (LOWORD(wParam) >= IDC_CODEFOLD_SETUP_TAGMARK &&
             LOWORD(wParam) <= IDC_CODEFOLD_SETUP_HOTKEYALL_PREVLEVEL)
    {
      SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_SETACTIVE)
    {
      if (nPropMaxVisitPage < PAGE_CODEFOLD2)
        nPropMaxVisitPage=PAGE_CODEFOLD2;
    }
    else if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      PSHNOTIFY *pshn=(PSHNOTIFY *)lParam;

      nDrawNodeType=nDrawNodeTypeDlg;
      bTagMarkEnable=(BOOL)SendMessage(hWndTagMark, BM_GETCHECK, 0, 0);
      bCollapseOnOpen=(BOOL)SendMessage(hWndCollapseOnOpen, BM_GETCHECK, 0, 0);
      bNoPrintCollapsed=(BOOL)SendMessage(hWndNoPrintCollapsed, BM_GETCHECK, 0, 0);

      dwFindRootLevel=(WORD)SendMessage(hWndFindRootHotkey, HKM_GETHOTKEY, 0, 0);
      nFindRootMaxDepth=GetDlgItemInt(hDlg, IDC_CODEFOLD_SETUP_FINDROOTDEPTH_EDIT, NULL, FALSE);

      dwCurrentCollapse=(WORD)SendMessage(hWndCurrentCollapse, HKM_GETHOTKEY, 0, 0);
      dwCurrentGoBegin=(WORD)SendMessage(hWndCurrentGoBegin, HKM_GETHOTKEY, 0, 0);
      dwCurrentGoEnd=(WORD)SendMessage(hWndCurrentGoEnd, HKM_GETHOTKEY, 0, 0);
      dwCurrentSelect=(WORD)SendMessage(hWndCurrentSelect, HKM_GETHOTKEY, 0, 0);
      dwAllCollapse=(WORD)SendMessage(hWndAllCollapse, HKM_GETHOTKEY, 0, 0);
      dwAllNextLevel=(WORD)SendMessage(hWndAllNextLevel, HKM_GETHOTKEY, 0, 0);
      dwAllPrevLevel=(WORD)SendMessage(hWndAllPrevLevel, HKM_GETHOTKEY, 0, 0);

      if (nInitCodeFold)
      {
        UninitCodeFold(INIT_CODEFOLD_HOTKEYS);
        InitCodeFold();
        UpdateEditAll(UE_FIRSTPIXEL);
      }

      if (pshn->lParam)
      {
        //OK button pressed
        bSaveOptions=TRUE;
      }
      else
      {
        //Apply button pressed
        if (nPropMaxVisitPage == PAGE_CODEFOLD2)
          UpdateAllOptions();
        return FALSE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK CodeFoldParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  static BOOL bTextChanging;

  if (uMsg == AKDN_OPENDOCUMENT_START)
  {
    NOPENDOCUMENT *nod=(NOPENDOCUMENT *)lParam;
    FOLDWINDOW *lpFoldWindow;

    if ((*nod->dwFlags) & OD_REOPEN)
    {
      if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, (HWND)wParam, NULL))
      {
        if (lpFoldWindow->pfwd->lpSyntaxFile && lpFoldWindow->pfwd->lpSyntaxFile->hFoldStack.first)
        {
          //Document reopened. Free folds here for correct AEM_GETSCROLLPOS, AEM_SETSCROLLPOS in OpenDocument of Edit.c.
          FreeFolds(lpFoldWindow, TRUE);
        }
      }
    }
    bResetFold=TRUE;
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_START)
  {
    NSAVEDOCUMENT *nsd=(NSAVEDOCUMENT *)lParam;
    FOLDWINDOW *lpFoldWindow;
    EDITINFO ei;

    if (nsd->dwFlags & SD_UPDATE)
    {
      if (SendMessage(hMainWnd, AKD_GETEDITINFO, wParam, (LPARAM)&ei))
      {
        if (StackGetSyntaxFileByFile(&hSyntaxFilesStack, GetFileName(nsd->wszFile, -1)) != StackGetSyntaxFileByFile(&hSyntaxFilesStack, GetFileName(ei.wszFile, -1)))
        {
          if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, (HWND)wParam, NULL))
            FreeFolds(lpFoldWindow, FALSE);
          bResetFold=TRUE;
        }
      }
    }
  }
  else if (uMsg == AKDN_OPENDOCUMENT_FINISH ||
           uMsg == AKDN_SAVEDOCUMENT_FINISH)
  {
    if (bResetFold)
    {
      FRAMEDATA *lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_CURRENT, 0);
      FOLDWINDOW *lpFoldWindow;
      bResetFold=FALSE;

      if (lpFrame->ei.hWndEdit == (HWND)wParam)
      {
        lpLastPostUpdateFrame=lpFrame;
        dwLastPostUpdateFlags|=SAE_RESETFOLD|SAE_RESETLIST;

        if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, (HWND)wParam, NULL))
        {
          if (lpFoldWindow->pfwd->lpSyntaxFile && lpFoldWindow->pfwd->lpSyntaxFile->hFoldStack.first)
          {
            //Document opened in the same window - SDI open command or reopen command in MDI/PMDI.
            if (nMDI == WMD_SDI)
              FreeFolds(lpFoldWindow, FALSE);
            FillLevelsStack(lpFoldWindow, &lpFoldWindow->pfwd->hLevelStack, hWndEdit, NULL);
            dwLastPostUpdateFlags&=~SAE_RESETFOLD;
          }
          else if (!lpFoldWindow->rcBoard.right)
          {
            //Delete to receive FillLevelsStack in WM_PAINT
            StackDeleteFoldWindow(&hFoldWindowsStack, lpFoldWindow);
          }
        }
        PostMessage(hWndCodeFoldDlg, AKDLL_UPDATE, 0, (LPARAM)lpFrame);
      }
      else SetActiveEdit((HWND)wParam, hWndCodeFoldList, SAE_RESETFOLD|SAE_NOLIST);
    }
  }
  else if (uMsg == AKDN_FRAME_ACTIVATE)
  {
    if (!(wParam & FWA_NOVISUPDATE))
    {
      FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;

      if (!lpCurrentFoldWindow ||
          (nMDI == WMD_MDI && lpCurrentFoldWindow->hWndEdit != lpFrame->ei.hWndEdit) ||
          (nMDI == WMD_PMDI && lpCurrentFoldWindow->hDocEdit != lpFrame->ei.hDocEdit))
      {
        lpLastPostUpdateFrame=lpFrame;
        dwLastPostUpdateFlags|=SAE_RESETLIST;
        PostMessage(hWndCodeFoldDlg, AKDLL_UPDATE, 0, (LPARAM)lpFrame);
      }
    }
  }
  else if (uMsg == AKDN_DOCK_RESIZE)
  {
    if (((DOCK *)wParam)->hWnd == dkCodeFoldDlg->hWnd)
      dwSaveFlags|=OF_CODEFOLD_RECT;
  }
  else if (uMsg == AKDN_SIZE_ONSTART)
  {
    if (bCodeFoldDockWaitResize)
    {
      bCodeFoldDockWaitResize=FALSE;
      ShowWindow(hWndCodeFoldDlg, SW_SHOW);
      dkCodeFoldDlg->dwFlags&=~DKF_HIDDEN;

      if (hWndEdit=GetCurEdit())
      {
        UpdateEditAll(UE_FIRSTPIXEL);
        lpCurrentFoldWindow=SetActiveEdit(hWndEdit, hWndCodeFoldList, SAE_RESETLIST);
      }
    }
  }
  else if (uMsg == AKDN_EDIT_ONCLOSE)
  {
    FOLDWINDOW *lpFoldWindow;

    if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, (HWND)wParam, NULL))
    {
      StackEndBoard(&hFoldWindowsStack, lpFoldWindow);
      StackDeleteFoldWindow(&hFoldWindowsStack, lpFoldWindow);

      //Update list
      lpLastPostUpdateFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_CURRENT, 0);
      dwLastPostUpdateFlags|=SAE_RESETLIST;
      PostMessage(hWndCodeFoldDlg, AKDLL_UPDATE, 0, (LPARAM)lpLastPostUpdateFrame);
    }
  }
  else if (uMsg == AKDN_EDIT_ONFINISH)
  {
    FOLDWINDOW *lpFoldWindow;

    if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, (HWND)wParam, NULL))
      StackDeleteFoldWindow(&hFoldWindowsStack, lpFoldWindow);
  }
  else if (uMsg == AKDN_FRAME_NOWINDOWS)
  {
    if (dkCodeFoldDlg && !(dkCodeFoldDlg->dwFlags & DKF_HIDDEN))
    {
      ClearTreeView(hWndCodeFoldList, TRUE);
      SetActiveEdit(NULL, hWndCodeFoldList, SAE_NOFOLD);
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    if (hWndCodeFoldDlg)
    {
      DestroyDock(hWndCodeFoldDlg, DKT_ONMAINFINISH);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT || (HWND)*lResult)
    {
      if (((NMHDR *)lParam)->code == AEN_SETRECT)
      {
        AENSETRECT *aensr=(AENSETRECT *)lParam;
        FOLDWINDOW *lpFoldWindow;
        RECT rcLineBoard={0};

        if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, aensr->hdr.hwndFrom, aensr->hdr.docFrom))
        {
          if (lpFoldWindow->rcBoard.right)
          {
            PLUGINFUNCTION *pf;

            if (pf=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"LineBoard::Main", 0))
            {
              if (pf->bRunning)
              {
                PLUGINCALLSENDW pcs;
                DLLEXTLINEBOARD delb;

                delb.dwStructSize=sizeof(DLLEXTLINEBOARD);
                delb.nAction=DLLA_LINEBOARD_GETBOARDRECT;
                delb.hWndEdit=aensr->hdr.hwndFrom;
                delb.hDocEdit=aensr->hdr.docFrom;
                delb.rcBoard=&rcLineBoard;

                pcs.pFunction=L"LineBoard::Main";
                pcs.lParam=(LPARAM)&delb;
                pcs.dwSupport=0;
                SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
              }
            }

            lpFoldWindow->rcBoard.right=rcLineBoard.left + (lpFoldWindow->rcBoard.right - lpFoldWindow->rcBoard.left);
            lpFoldWindow->rcBoard.left=rcLineBoard.left;
            lpFoldWindow->rcBoard.top=rcLineBoard.top;
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_SELCHANGED)
      {
        AENSELCHANGE *aensc=(AENSELCHANGE *)lParam;
        FOLDWINDOW *lpFoldWindow;

        if (aensc->hdr.hwndFrom == GetFocus() ||
            //F3 in Find/Replace dialog selects text when dialog window has focus
            aensc->hdr.hwndFrom == (HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL))
        {
          hWndEdit=aensc->hdr.hwndFrom;

          if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
          {
            if (dkCodeFoldDlg && !(dkCodeFoldDlg->dwFlags & DKF_HIDDEN))
            {
              SetCaretTreeView(hWndCodeFoldList, lpFoldWindow);
            }

            //Check TagMark
            if (!bTextChanging)
            {
              //UpdateTagMark also called after AEN_TEXTCHANGED
              UpdateTagMark(lpFoldWindow);
            }
          }
        }
        SendMessage(aensc->hdr.hwndFrom, AEM_COLLAPSELINE, aensc->ciCaret.nLine, AECF_EXPAND);
      }
      else
      {
        //Text change
        static FOLDWINDOW *fwChange;
        static AECHARRANGE crChangeBegin;

        if (((NMHDR *)lParam)->code == AEN_TEXTCHANGING)
        {
          AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;

          if (fwChange=StackGetFoldWindow(&hFoldWindowsStack, aentc->hdr.hwndFrom, aentc->hdr.docFrom))
          {
            if (!(aentc->dwType & AETCT_WRAP))
            {
              if ((aentc->dwType & AETCT_SETTEXT) ||
                  (aentc->dwType & AETCT_STREAMIN))
              {
                RemoveTagMark(fwChange);
              }
              if (!bResetFold)
              {
                if (fwChange->pfwd->lpSyntaxFile)
                {
                  fwChange->bLevelChanged=FALSE;
                  fwChange->lpTextChangeMinParent=NULL;
                  fwChange->lpTextChangeMaxParent=NULL;
                  fwChange->lpTextChangeMinPrevParent=NULL;
                  fwChange->lpTextChangeMaxPrevParent=NULL;
                  xmemset(&fwChange->crTextChange, 0, sizeof(AECHARRANGE));
                }
              }
            }
          }
          bTextChanging=TRUE;
        }
        else if (((NMHDR *)lParam)->code == AEN_POINT)
        {
          AENPOINT *aenpt=(AENPOINT *)lParam;
          AEFOLD *lpFold;

          if ((aenpt->dwType & AEPTT_SETTEXT) ||
              (aenpt->dwType & AEPTT_STREAMIN) ||
              (aenpt->lpPoint->dwFlags & AEPTF_FOLD))
          {
            if (!bResetFold && fwChange && fwChange->pfwd->lpSyntaxFile)
            {
              fwChange->bLevelChanged=TRUE;

              if (aenpt->lpPoint && (aenpt->lpPoint->dwFlags & AEPTF_FOLD))
              {
                //Get parent level
                lpFold=(AEFOLD *)aenpt->lpPoint->dwUserData;
                while (lpFold->parent)
                  lpFold=lpFold->parent;

                if (!fwChange->crTextChange.ciMin.lpLine || AEC_IndexCompare(&lpFold->lpMinPoint->ciPoint, &fwChange->crTextChange.ciMin) <= 0)
                {
                  fwChange->crTextChange.ciMin=lpFold->lpMinPoint->ciPoint;
                  fwChange->lpTextChangeMinParent=lpFold;
                }
                if (!fwChange->crTextChange.ciMax.lpLine || AEC_IndexCompare(&lpFold->lpMaxPoint->ciPoint, &fwChange->crTextChange.ciMax) >= 0)
                {
                  fwChange->crTextChange.ciMax=lpFold->lpMaxPoint->ciPoint;
                  fwChange->lpTextChangeMaxParent=lpFold;
                }
              }
            }
          }
        }
        else if (((NMHDR *)lParam)->code == AEN_TEXTINSERTBEGIN ||
                 ((NMHDR *)lParam)->code == AEN_TEXTDELETEBEGIN)
        {
          AECHARRANGE *lpcrRange;

          if (!bResetFold && fwChange && fwChange->pfwd->lpSyntaxFile)
          {
            if (((NMHDR *)lParam)->code == AEN_TEXTINSERTBEGIN)
              lpcrRange=&((AENTEXTINSERT *)lParam)->crAkelRange;
            else
              lpcrRange=&((AENTEXTDELETE *)lParam)->crAkelRange;

            //crChangeBegin.ciMin can be not equal to lpcrRange->ciMin, if insert
            //start position wrappen to next line, dangling crChangeBegin.ciMin.
            xmemcpy(&crChangeBegin, lpcrRange, sizeof(AECHARRANGE));
          }
        }
        else if (((NMHDR *)lParam)->code == AEN_TEXTINSERTEND ||
                 ((NMHDR *)lParam)->code == AEN_TEXTDELETEEND)
        {
          AECHARRANGE *lpcrRange;

          if (!bResetFold && fwChange && fwChange->pfwd->lpSyntaxFile)
          {
            if (((NMHDR *)lParam)->code == AEN_TEXTINSERTEND)
              lpcrRange=&((AENTEXTINSERT *)lParam)->crAkelRange;
            else
              lpcrRange=&((AENTEXTDELETE *)lParam)->crAkelRange;

            if (fwChange->crTextChange.ciMin.nLine == lpcrRange->ciMin.nLine)
              fwChange->crTextChange.ciMin.lpLine=lpcrRange->ciMin.lpLine;
            else if (fwChange->crTextChange.ciMin.nLine >= crChangeBegin.ciMin.nLine)
              fwChange->crTextChange.ciMin=lpcrRange->ciMin;
            //Drag'n'Drop "<div>" after "</div>" (AE_WrapLines updates also previous line):
            //<div>12345\WRAP
            //67890</div>
            else if (fwChange->crTextChange.ciMin.nLine == lpcrRange->ciMin.nLine - 1 && lpcrRange->ciMin.lpLine->prev->nLineBreak == AELB_WRAP)
              fwChange->crTextChange.ciMin.lpLine=lpcrRange->ciMin.lpLine->prev;

            if (fwChange->crTextChange.ciMax.nLine == lpcrRange->ciMax.nLine)
              fwChange->crTextChange.ciMax.lpLine=lpcrRange->ciMax.lpLine;
            else if (fwChange->crTextChange.ciMax.nLine <= crChangeBegin.ciMax.nLine)
              fwChange->crTextChange.ciMax=lpcrRange->ciMax;

            if (!fwChange->crTextChange.ciMin.lpLine || AEC_IndexCompare(&lpcrRange->ciMin, &fwChange->crTextChange.ciMin) <= 0)
              fwChange->crTextChange.ciMin=lpcrRange->ciMin;
            if (!fwChange->crTextChange.ciMax.lpLine || AEC_IndexCompare(&lpcrRange->ciMax, &fwChange->crTextChange.ciMax) >= 0)
              fwChange->crTextChange.ciMax=lpcrRange->ciMax;
          }
        }
        else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGED)
        {
          AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;
          AECHARINDEX ciChar;
          AEFOLD *lpFoldExist;
          AEFOLD *lpFoldNextExist;
          FOLDDATA *lpFoldDataExist;
          LEVEL *lpLevelTemp;
          LEVEL *lpLevelNextTemp;
          int nCompare;
          BOOL bNeedUpdate=FALSE;

          if (!(aentc->dwType & AETCT_NONE))
          {
            if (!bResetFold && fwChange && fwChange->pfwd->lpSyntaxFile)
            {
              if (aentc->dwType & AETCT_DELETEALL)
              {
                //All text is changed.
                if (lpCurrentFoldWindow && lpCurrentFoldWindow->hWndEdit == fwChange->hWndEdit)
                  SetActiveEdit(fwChange->hWndEdit, hWndCodeFoldList, SAE_RESETFOLD|SAE_RESETLIST);
                else
                  SetActiveEdit(fwChange->hWndEdit, hWndCodeFoldList, SAE_RESETFOLD);
              }
              else
              {
                if (!fwChange->bLevelChanged)
                {
                  if (fwChange->crTextChange.ciMin.lpLine &&
                      fwChange->crTextChange.ciMax.lpLine)
                  {
                    SendMessage(fwChange->hWndEdit, AEM_GETINDEX, AEGI_WRAPLINEBEGIN, (LPARAM)&fwChange->crTextChange.ciMin);
                    SendMessage(fwChange->hWndEdit, AEM_GETINDEX, AEGI_WRAPLINEEND, (LPARAM)&fwChange->crTextChange.ciMax);

                    //Find is any new folds appeared or existed doesn't match.
                    if (FindFold(fwChange, &fwChange->crTextChange))
                      fwChange->bLevelChanged=TRUE;
                  }
                }

                //Levels were changed, update info.
                if (fwChange->bLevelChanged)
                {
                  //Get min parent level.
                  if (!fwChange->lpTextChangeMinParent || AEC_IndexCompare(&fwChange->lpTextChangeMinParent->lpMinPoint->ciPoint, &fwChange->crTextChange.ciMin) > 0)
                  {
                    fwChange->lpTextChangeMinParent=FoldGet(fwChange, AEFF_FINDINDEX|AEFF_GETROOT, (UINT_PTR)&fwChange->crTextChange.ciMin, &fwChange->lpTextChangeMinPrevParent);
                  }
                  if (fwChange->lpTextChangeMinParent && fwChange->lpTextChangeMinParent->prev &&
                      (fwChange->lpTextChangeMinParent->lpMinPoint->dwFlags & AEPTF_MODIFY) &&
                      (FoldData(fwChange->lpTextChangeMinParent->prev)->lpFoldInfo->dwFlags & FIF_FOLDEND_NOCATCH))
                  {
                    //If previous parent has FIF_FOLDEND_NOCATCH use it as lpTextChangeMinParent
                    fwChange->lpTextChangeMinParent=NULL;
                  }
                  //Get max parent level.
                  if (fwChange->lpTextChangeMaxParent)
                    EndOfPoint(fwChange, fwChange->lpTextChangeMaxParent->lpMaxPoint, &ciChar);
                  if (!fwChange->lpTextChangeMaxParent || AEC_IndexCompare(&ciChar, &fwChange->crTextChange.ciMax) < 0)
                  {
                    fwChange->lpTextChangeMaxParent=FoldGet(fwChange, AEFF_FINDINDEX|AEFF_GETROOT, (UINT_PTR)&fwChange->crTextChange.ciMax, &fwChange->lpTextChangeMaxPrevParent);
                  }

                  //Get start index for rescan levels.
                  if (!fwChange->lpTextChangeMinParent)
                  {
                    if (!fwChange->lpTextChangeMinPrevParent)
                    {
                      fwChange->lpTextChangeMinParent=(AEFOLD *)fwChange->pfwd->lpFoldStack->first;
                      SendMessage(fwChange->hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&fwChange->crTextChange.ciMin);
                    }
                    else
                    {
                      fwChange->lpTextChangeMinParent=fwChange->lpTextChangeMinPrevParent;
                      if (AEC_IndexCompare(&fwChange->crTextChange.ciMin, &fwChange->lpTextChangeMinParent->lpMinPoint->ciPoint) > 0)
                        fwChange->crTextChange.ciMin=fwChange->lpTextChangeMinParent->lpMinPoint->ciPoint;
                    }
                  }
                  else fwChange->crTextChange.ciMin=fwChange->lpTextChangeMinParent->lpMinPoint->ciPoint;

                  //Get end index for rescan levels.
                  if (!fwChange->lpTextChangeMaxParent)
                  {
                    if (!fwChange->lpTextChangeMaxPrevParent)
                    {
                      fwChange->lpTextChangeMaxParent=(AEFOLD *)fwChange->pfwd->lpFoldStack->last;
                      SendMessage(fwChange->hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&fwChange->crTextChange.ciMax);
                    }
                    else
                    {
                      fwChange->lpTextChangeMaxParent=fwChange->lpTextChangeMaxPrevParent;
                      EndOfPoint(fwChange, fwChange->lpTextChangeMaxParent->lpMaxPoint, &ciChar);
                      if (AEC_IndexCompare(&fwChange->crTextChange.ciMax, &ciChar) < 0)
                        fwChange->crTextChange.ciMax=ciChar;
                    }
                  }
                  else EndOfPoint(fwChange, fwChange->lpTextChangeMaxParent->lpMaxPoint, &fwChange->crTextChange.ciMax);

                  //Parse changed text range and put results in temp stack.
                  fwChange->pfwd->hTempStack.first=0;
                  fwChange->pfwd->hTempStack.last=0;
                  FillLevelsStack(fwChange, &fwChange->pfwd->hTempStack, fwChange->hWndEdit, &fwChange->crTextChange);

                  //Transfer existed elements settings that matched with new ones to temp stack and delete them.
                  lpFoldExist=fwChange->lpTextChangeMinParent;
                  lpLevelTemp=(LEVEL *)fwChange->pfwd->hTempStack.first;

                  while (lpFoldExist && lpLevelTemp)
                  {
                    lpLevelNextTemp=lpLevelTemp->next;

                    nCompare=AEC_IndexCompare(&lpFoldExist->lpMinPoint->ciPoint, &lpLevelTemp->pointMin.ciPoint);

                    if (nCompare <= 0)
                    {
                      lpFoldNextExist=AEC_NextFold(lpFoldExist, TRUE);
                      lpFoldDataExist=FoldData(lpFoldExist);

                      if (nCompare == 0)
                      {
                        if (lpFoldDataExist->lpFoldInfo == lpLevelTemp->pfd->lpFoldInfo)
                        {
                          if (!AEC_IndexCompare(&lpFoldExist->lpMaxPoint->ciPoint, &lpLevelTemp->pointMax.ciPoint))
                          {
                            if (!(lpFoldExist->lpMinPoint->dwFlags & AEPTF_MODIFY) &&
                                !(lpFoldExist->lpMaxPoint->dwFlags & AEPTF_MODIFY))
                            {
                              DeleteFoldData(lpFoldDataExist);
                              lpFoldDataExist=NULL;

                              //Inherit fold
                              lpLevelTemp->pfd->lpFold=lpFoldExist;
                              lpLevelTemp->pfd->lpFold->dwUserData=(UINT_PTR)lpLevelTemp->pfd;

                              //LEVEL item now is not needed
                              StackDeleteLevel(&fwChange->pfwd->hTempStack, lpLevelTemp);
                            }
                          }
                        }
                        lpLevelTemp=lpLevelNextTemp;
                      }
                      if (lpFoldDataExist)
                      {
                        if (lpFoldExist->bCollapse)
                          bNeedUpdate=TRUE;
                        DeleteFoldData(lpFoldDataExist);
                        SendMessage(fwChange->hWndEdit, AEM_DELETEFOLD, (WPARAM)lpFoldExist, FALSE);
                      }
                      lpFoldExist=lpFoldNextExist;
                    }
                    else lpLevelTemp=lpLevelNextTemp;
                  }

                  //Delete remaining existed elements that are in changed text area.
                  while (lpFoldExist)
                  {
                    lpFoldNextExist=AEC_NextFold(lpFoldExist, TRUE);

                    if (AEC_IndexCompare(&lpFoldExist->lpMinPoint->ciPoint, &fwChange->crTextChange.ciMax) < 0 ||
                        (!AEC_IndexCompare(&lpFoldExist->lpMinPoint->ciPoint, &fwChange->crTextChange.ciMax) &&
                         !AEC_IndexCompare(&lpFoldExist->lpMinPoint->ciPoint, &lpFoldExist->lpMaxPoint->ciPoint)))
                    {
                      if (lpFoldExist->bCollapse)
                        bNeedUpdate=TRUE;
                      DeleteFoldData(FoldData(lpFoldExist));
                      SendMessage(fwChange->hWndEdit, AEM_DELETEFOLD, (WPARAM)lpFoldExist, FALSE);
                    }
                    else break;

                    lpFoldExist=lpFoldNextExist;
                  }

                  //Fill lpFold member.
                  CreateAllFolds(&fwChange->pfwd->hTempStack, fwChange->hWndEdit);

                  //Update list and edit control
                  if (lpCurrentFoldWindow && lpCurrentFoldWindow->hWndEdit == fwChange->hWndEdit)
                    SetActiveEdit(fwChange->hWndEdit, hWndCodeFoldList, SAE_RESETLIST);

                  if (bNeedUpdate)
                    SendMessage(fwChange->hWndEdit, AEM_UPDATEFOLD, 0, -1);
                  else
                    InvalidateRect(fwChange->hWndEdit, NULL, TRUE);
                }
                else
                {
                  //Update name of the fold
                  if (fwChange->pfwd->lpFoldStack->first)
                  {
                    AECHARRANGE crNameRange;
                    TVITEMW tvi;
                    AEFOLD *lpFold;
                    AEFOLD *lpPrevFold;

                    //Is caret in the fold name?
                    lpFold=FoldGet(fwChange, AEFF_FINDINDEX|AEFF_FOLDSTART|AEFF_FOLDEND|AEFF_RECURSE, (UINT_PTR)&aentc->ciCaret, &lpPrevFold);
                    if (lpPrevFold)
                      lpFold=AEC_NextFold(lpPrevFold, FALSE);
                    else if (lpFold)
                      lpFold=AEC_NextFold(lpFold, TRUE);
                    if (!lpFold)
                      lpFold=(AEFOLD *)fwChange->pfwd->lpFoldStack->first;
                    if (GetFoldName(FoldData(lpFold), &lpFold->lpMinPoint->ciPoint, &crNameRange))
                    {
                      if (AEC_IndexCompare(&crNameRange.ciMin, &aentc->ciCaret) <= 0 &&
                          AEC_IndexCompare(&crNameRange.ciMax, &aentc->ciCaret) >= 0)
                      {
                        GlobalFree((HGLOBAL)FoldData(lpFold)->wpName);
                        FoldData(lpFold)->wpName=NULL;
                        GetFoldName(FoldData(lpFold), &lpFold->lpMinPoint->ciPoint, NULL);

                        if (!(dkCodeFoldDlg->dwFlags & DKF_HIDDEN) && FoldData(lpFold)->hItem)
                        {
                          tvi.mask=TVIF_TEXT;
                          tvi.hItem=FoldData(lpFold)->hItem;
                          tvi.pszText=FoldData(lpFold)->wpName;
                          tvi.cchTextMax=MAX_PATH;
                          TreeView_SetItemWide(hWndCodeFoldList, &tvi);
                        }
                      }
                    }
                  }
                }
                fwChange->bLevelChanged=FALSE;
                xmemset(&fwChange->crTextChange, 0, sizeof(AECHARRANGE));
              }

              //Check TagMark
              UpdateTagMark(fwChange);
            }
          }
          bTextChanging=FALSE;
        }
      }
    }
  }
  else if (uMsg == AKD_SETFONT ||
           uMsg == AKD_SETFONTA ||
           uMsg == AKD_SETFONTW)
  {
    if (dkCodeFoldDlg && !(dkCodeFoldDlg->dwFlags & DKF_HIDDEN))
    {
      if (!bFoldListSystemFont)
      {
        HFONT hFontEdit;

        *lResult=NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
        hFontEdit=(HFONT)SendMessage(hMainWnd, AKD_GETFONT, (WPARAM)NULL, (LPARAM)NULL);
        SendMessage(hWndCodeFoldList, WM_SETFONT, (WPARAM)hFontEdit, FALSE);
        return TRUE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK CodeFoldEditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (uMsg == WM_PAINT)
  {
    FOLDWINDOW *lpFoldWindow;
    HDC hDC;
    RECT rcDraw;
    int nWidth;

    if (!(lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWnd, NULL)))
      lpFoldWindow=FillLevelsStack(NULL, NULL, hWnd, NULL);

    if (lpFoldWindow)
    {
      if (!lpFoldWindow->pfwd->lpSyntaxFile)
      {
        if (lpFoldWindow->rcBoard.right)
        {
          //Remove rect
          nWidth=lpFoldWindow->rcBoard.right - lpFoldWindow->rcBoard.left;
          lpFoldWindow->rcBoard.left=0;
          lpFoldWindow->rcBoard.right=0;
          SetEditRect(NULL, lpFoldWindow->hWndEdit, 0, nWidth);
          UpdateWindow(hWnd);

          //WM_PAINT was sended after UpdateWindow, so don't redraw edit.
          *lResult=0;
          return TRUE;
        }
      }
      else
      {
        if (!lpFoldWindow->rcBoard.right)
        {
          //Add rect
          SendMessage(hWnd, AEM_GETRECT, 0, (LPARAM)&rcDraw);
          GetClientRect(hWnd, &lpFoldWindow->rcBoard);
          lpFoldWindow->rcBoard.left=rcDraw.left;
          lpFoldWindow->rcBoard.right=rcDraw.left + BOARD_WIDTH + BOARD_EDGE * 2;
          SetEditRect(NULL, hWnd, lpFoldWindow->rcBoard.right - lpFoldWindow->rcBoard.left, 0);
          UpdateWindow(hWnd);

          //WM_PAINT was sended after UpdateWindow, so don't redraw edit.
          *lResult=0;
          return TRUE;
        }

        //Call next procedure
        if (lpFoldWindow->lpUser)
          *lResult=CallWindowProcWide(lpFoldWindow->lpUser->lpOldEditProc, hWnd, uMsg, wParam, lParam);
        else
          *lResult=NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);

        if (hDC=GetDC(hWnd))
        {
          AEFOLD *lpFold=NULL;
          AEFOLD *lpCaretFold=NULL;
          AEFOLD *lpRootFold;
          AEFOLD *lpDeepless;
          AECHARINDEX ciFirstLine;
          AECHARINDEX ciLastLine;
          HDC hBufferDC;
          HBITMAP hBitmap;
          HBITMAP hBitmapOld;
          HBRUSH hHollowBrush=NULL;
          HBRUSH hNormalFoldBrush;
          HBRUSH hActiveFoldBrush;
          HBRUSH hNormalNodeOpenBrush=NULL;
          HBRUSH hNormalNodeCloseBrush=NULL;
          HBRUSH hActiveNodeOpenBrush=NULL;
          HBRUSH hActiveNodeCloseBrush=NULL;
          HBRUSH hBrushOld;
          HBRUSH hFirstBkBrush;
          HPEN hSecondBkPen;
          HPEN hNormalFoldPen;
          HPEN hActiveFoldPen;
          HPEN hNormalNodeOpenPen=NULL;
          HPEN hNormalNodeClosePen=NULL;
          HPEN hActiveNodeOpenPen=NULL;
          HPEN hActiveNodeClosePen=NULL;
          HPEN hCurrentPen;
          HPEN hPenOld;
          RECT rcBoard;
          int nLastFigure=FRG_NONE;
          int nHPos;
          int nVPos;
          int nCharHeight;
          int nBoardWidth;
          int i;
          BOOL bCaretFold=FALSE;

          GetClientRect(hWnd, &rcBoard);
          lpFoldWindow->rcBoard.bottom=rcBoard.bottom;

          hBufferDC=CreateCompatibleDC(hDC);
          hBitmap=CreateCompatibleBitmap(hDC, lpFoldWindow->rcBoard.right, lpFoldWindow->rcBoard.bottom);
          hBitmapOld=(HBITMAP)SelectObject(hBufferDC, hBitmap);

          SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTVISIBLELINE, (LPARAM)&ciFirstLine);
          SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTVISIBLELINE, (LPARAM)&ciLastLine);
          AEC_NextLineEx(&ciLastLine, &ciLastLine);

          nHPos=lpFoldWindow->rcBoard.left + (lpFoldWindow->rcBoard.right - lpFoldWindow->rcBoard.left) / 2;
          nVPos=(int)SendMessage(hWnd, AEM_VPOSFROMLINE, AECT_CLIENT, ciFirstLine.nLine);
          nCharHeight=(int)SendMessage(hWnd, AEM_GETCHARSIZE, AECS_HEIGHT, 0);

          hNormalFoldPen=CreatePen(PS_SOLID, 0, lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalFoldColor);
          hActiveFoldPen=CreatePen(PS_SOLID, 0, lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveFoldColor);
          hSecondBkPen=CreatePen(PS_SOLID, 0, lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelSecondBkColor);
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeOpenSignColor != (DWORD)-1)
            hNormalNodeOpenPen=CreatePen(PS_SOLID, 0, lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeOpenSignColor);
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeCloseSignColor != (DWORD)-1)
            hNormalNodeClosePen=CreatePen(PS_SOLID, 0, lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeCloseSignColor);
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeOpenSignColor != (DWORD)-1)
            hActiveNodeOpenPen=CreatePen(PS_SOLID, 0, lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeOpenSignColor);
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeCloseSignColor != (DWORD)-1)
            hActiveNodeClosePen=CreatePen(PS_SOLID, 0, lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeCloseSignColor);

          //Erase board background
          hFirstBkBrush=CreateSolidBrush(lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelFirstBkColor);
          FillRect(hBufferDC, &lpFoldWindow->rcBoard, hFirstBkBrush);

          hPenOld=(HPEN)SelectObject(hBufferDC, hSecondBkPen);
          nBoardWidth=lpFoldWindow->rcBoard.right - lpFoldWindow->rcBoard.left;
          for (i=lpFoldWindow->rcBoard.top; i < lpFoldWindow->rcBoard.bottom + nBoardWidth; i+=2)
          {
            MoveToEx(hBufferDC, lpFoldWindow->rcBoard.left, i, NULL);
            LineTo(hBufferDC, lpFoldWindow->rcBoard.right, i - nBoardWidth);
          }
          hCurrentPen=hNormalFoldPen;
          SelectObject(hBufferDC, hCurrentPen);

          //Brushes
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeOpenBkColor != (DWORD)-1)
            hNormalNodeOpenBrush=CreateSolidBrush(lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeOpenBkColor);
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeCloseBkColor != (DWORD)-1)
            hNormalNodeCloseBrush=CreateSolidBrush(lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalNodeCloseBkColor);
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeOpenBkColor != (DWORD)-1)
            hActiveNodeOpenBrush=CreateSolidBrush(lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeOpenBkColor);
          if (lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeCloseBkColor != (DWORD)-1)
            hActiveNodeCloseBrush=CreateSolidBrush(lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveNodeCloseBkColor);
          hNormalFoldBrush=CreateSolidBrush(lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelNormalFoldColor);
          hActiveFoldBrush=CreateSolidBrush(lpFoldWindow->pfwd->lpSyntaxFile->dwFoldPanelActiveFoldColor);
          hHollowBrush=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
          hBrushOld=(HBRUSH)SelectObject(hBufferDC, hHollowBrush);

          while (ciFirstLine.nLine <= ciLastLine.nLine)
          {
            if (!SendMessage(hWnd, AEM_ISLINECOLLAPSED, ciFirstLine.nLine, 0))
            {
              lpFold=FoldGet(lpFoldWindow, AEFF_FINDLINE|AEFF_FOLDSTART|AEFF_FOLDEND|AEFF_RECURSE, ciFirstLine.nLine, NULL);

              //Skip one line folds
              while (lpFold)
              {
                if (lpFold->lpMinPoint->ciPoint.nLine == lpFold->lpMaxPoint->ciPoint.nLine)
                {
                  if (!lpFold->parent && lpFold->prev &&
                                         lpFold->prev->lpMaxPoint->ciPoint.nLine == ciFirstLine.nLine &&
                                         lpFold->prev->lpMinPoint->ciPoint.nLine != lpFold->prev->lpMaxPoint->ciPoint.nLine)
                  {
                    lpFold=lpFold->prev;
                  }
                  else lpFold=lpFold->parent;
                }
                else break;
              }

              if (lpFold)
              {
                //If folds start at the same line, use deepless
                for (lpDeepless=lpFold; lpDeepless; lpDeepless=lpDeepless->parent)
                {
                  if (lpDeepless->lpMinPoint->ciPoint.nLine == ciFirstLine.nLine)
                    lpFold=lpDeepless;
                  else
                    break;
                }

                //If folds end at the same line, use deepless
                for (lpDeepless=lpFold; lpDeepless; lpDeepless=lpDeepless->parent)
                {
                  if (lpDeepless->lpMaxPoint->ciPoint.nLine == ciFirstLine.nLine)
                    lpFold=lpDeepless;
                  else
                    break;
                }

                //If line is on fold end, then check is line also on next subling fold start
                if (lpFold->lpMaxPoint->ciPoint.nLine == ciFirstLine.nLine)
                {
                  if (lpFold->next &&
                      lpFold->next->lpMinPoint->ciPoint.nLine == ciFirstLine.nLine &&
                      lpFold->next->lpMinPoint->ciPoint.nLine != lpFold->next->lpMaxPoint->ciPoint.nLine)
                  {
                    lpFold=lpFold->next;
                  }
                }
              }

              if (lpFold)
              {
                //Active fold
                if (!bCaretFold)
                {
                  bCaretFold=TRUE;
                  lpCaretFold=GetCaretFold(lpFoldWindow, NULL);

                  //Skip one line folds
                  while (lpCaretFold)
                  {
                    if (lpCaretFold->lpMinPoint->ciPoint.nLine == lpCaretFold->lpMaxPoint->ciPoint.nLine)
                      lpCaretFold=lpCaretFold->parent;
                    else
                      break;
                  }
                }
                if (lpCaretFold && AEC_IndexCompare(&lpCaretFold->lpMinPoint->ciPoint, &lpFold->lpMinPoint->ciPoint) <= 0 &&
                                   AEC_IndexCompare(&lpCaretFold->lpMaxPoint->ciPoint, &lpFold->lpMaxPoint->ciPoint) >= 0)
                  hCurrentPen=hActiveFoldPen;
                else
                  hCurrentPen=hNormalFoldPen;
                SelectObject(hBufferDC, hCurrentPen);

                //Draw figures
                if (nLastFigure == FRG_FOLDEND)
                {
                  MoveToEx(hBufferDC, nHPos, nVPos - nCharHeight / 2, NULL);
                  LineTo(hBufferDC, nHPos, nVPos);
                }

                if (ciFirstLine.nLine == lpFold->lpMinPoint->ciPoint.nLine)
                {
                  if (nLastFigure != FRG_NONE)
                  {
                    //Line before node
                    MoveToEx(hBufferDC, nHPos, nVPos, NULL);
                    LineTo(hBufferDC, nHPos, nVPos + nCharHeight / 2 - BOARD_WIDTH / 2);
                  }

                  //Draw node
                  if (lpFold->bCollapse)
                  {
                    if (hActiveNodeCloseBrush && hCurrentPen == hActiveFoldPen)
                      SelectObject(hBufferDC, hActiveNodeCloseBrush);
                    else if (hNormalNodeCloseBrush && hCurrentPen != hActiveFoldPen)
                      SelectObject(hBufferDC, hNormalNodeCloseBrush);
                    else
                    {
                      if (hCurrentPen == hActiveFoldPen)
                        SelectObject(hBufferDC, hActiveFoldBrush);
                      else
                        SelectObject(hBufferDC, hNormalFoldBrush);
                    }
                  }
                  else
                  {
                    if (hActiveNodeOpenBrush && hCurrentPen == hActiveFoldPen)
                      SelectObject(hBufferDC, hActiveNodeOpenBrush);
                    else if (hNormalNodeOpenBrush && hCurrentPen != hActiveFoldPen)
                      SelectObject(hBufferDC, hNormalNodeOpenBrush);
                    else
                      SelectObject(hBufferDC, hHollowBrush);
                  }
                  RoundRect(hBufferDC, (nHPos - BOARD_WIDTH / 2), (nVPos + nCharHeight / 2 - BOARD_WIDTH / 2), (nHPos + BOARD_WIDTH / 2) + 1, (nVPos + nCharHeight / 2 + BOARD_WIDTH / 2) + 1, (nDrawNodeType == DNT_ROUND)?BOARD_WIDTH:0, (nDrawNodeType == DNT_ROUND)?BOARD_WIDTH:0);

                  //Line after node
                  MoveToEx(hBufferDC, nHPos, nVPos + nCharHeight / 2 + BOARD_WIDTH / 2, NULL);
                  LineTo(hBufferDC, nHPos, nVPos + nCharHeight);

                  //Draw sign
                  if (lpFold->bCollapse)
                  {
                    if (hActiveNodeClosePen && hCurrentPen == hActiveFoldPen)
                      SelectObject(hBufferDC, hActiveNodeClosePen);
                    else if (hNormalNodeClosePen && hCurrentPen != hActiveFoldPen)
                      SelectObject(hBufferDC, hNormalNodeClosePen);
                    else
                      SelectObject(hBufferDC, hSecondBkPen);
                  }
                  else
                  {
                    if (hActiveNodeOpenPen && hCurrentPen == hActiveFoldPen)
                      SelectObject(hBufferDC, hActiveNodeOpenPen);
                    else if (hNormalNodeOpenPen && hCurrentPen != hActiveFoldPen)
                      SelectObject(hBufferDC, hNormalNodeOpenPen);
                  }
                  if (lpFold->bCollapse)
                  {
                    //Plus
                    MoveToEx(hBufferDC, nHPos - BOARD_WIDTH / 2 + 2, nVPos + nCharHeight / 2, NULL);
                    LineTo(hBufferDC, nHPos + BOARD_WIDTH / 2 - 1, nVPos + nCharHeight / 2);
                    MoveToEx(hBufferDC, nHPos, nVPos + nCharHeight / 2 - BOARD_WIDTH / 2 + 2, NULL);
                    LineTo(hBufferDC, nHPos, nVPos + nCharHeight / 2 + BOARD_WIDTH / 2 - 1);

                    nLastFigure=FRG_FOLDSTARTPLUS;
                  }
                  else
                  {
                    //Minus
                    MoveToEx(hBufferDC, nHPos - BOARD_WIDTH / 2 + 2, nVPos + nCharHeight / 2, NULL);
                    LineTo(hBufferDC, nHPos + BOARD_WIDTH / 2 - 1, nVPos + nCharHeight / 2);

                    nLastFigure=FRG_FOLDSTARTMINUS;
                  }
                }
                else if (ciFirstLine.nLine == lpFold->lpMaxPoint->ciPoint.nLine)
                {
                  //Line to the right
                  MoveToEx(hBufferDC, nHPos, nVPos, NULL);
                  LineTo(hBufferDC, nHPos, nVPos + nCharHeight / 2);
                  MoveToEx(hBufferDC, nHPos, nVPos + nCharHeight / 2, NULL);
                  LineTo(hBufferDC, nHPos + BOARD_WIDTH / 2, nVPos + nCharHeight / 2);

                  if (lpFold->parent)
                  {
                    for (lpRootFold=lpFold->parent; lpRootFold->parent; lpRootFold=lpRootFold->parent);

                    if (ciFirstLine.nLine == lpRootFold->lpMaxPoint->ciPoint.nLine)
                      nLastFigure=FRG_NONE;
                    else
                      nLastFigure=FRG_FOLDEND;
                  }
                  else nLastFigure=FRG_NONE;
                }
                else
                {
                  if (lpFold->bCollapse)
                  {
                    nVPos-=nCharHeight;
                  }
                  else
                  {
                    //Vertical line
                    MoveToEx(hBufferDC, nHPos, nVPos, NULL);
                    LineTo(hBufferDC, nHPos, nVPos + nCharHeight);
                    nLastFigure=FRG_LINE;
                  }
                }
              }
              else nLastFigure=FRG_NONE;

              //Next line
              nVPos+=nCharHeight;
              AEC_NextLine(&ciFirstLine);
            }
            else SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTUNCOLLAPSEDLINE, (LPARAM)&ciFirstLine);
          }
          BitBlt(hDC, lpFoldWindow->rcBoard.left, lpFoldWindow->rcBoard.top, lpFoldWindow->rcBoard.right - lpFoldWindow->rcBoard.left, lpFoldWindow->rcBoard.bottom - lpFoldWindow->rcBoard.top, hBufferDC, lpFoldWindow->rcBoard.left, lpFoldWindow->rcBoard.top, SRCCOPY);

          //Brushes
          SelectObject(hBufferDC, hBrushOld);
          if (hNormalNodeOpenBrush) DeleteObject(hNormalNodeOpenBrush);
          if (hNormalNodeCloseBrush) DeleteObject(hNormalNodeCloseBrush);
          if (hActiveNodeOpenBrush) DeleteObject(hActiveNodeOpenBrush);
          if (hActiveNodeCloseBrush) DeleteObject(hActiveNodeCloseBrush);
          if (hFirstBkBrush) DeleteObject(hFirstBkBrush);
          DeleteObject(hNormalFoldBrush);
          DeleteObject(hActiveFoldBrush);

          //Pens
          SelectObject(hBufferDC, hPenOld);
          if (hNormalNodeOpenPen) DeleteObject(hNormalNodeOpenPen);
          if (hNormalNodeClosePen) DeleteObject(hNormalNodeClosePen);
          if (hActiveNodeOpenPen) DeleteObject(hActiveNodeOpenPen);
          if (hActiveNodeClosePen) DeleteObject(hActiveNodeClosePen);
          DeleteObject(hNormalFoldPen);
          DeleteObject(hActiveFoldPen);
          DeleteObject(hSecondBkPen);

          SelectObject(hBufferDC, hBitmapOld);
          DeleteDC(hBufferDC);
          DeleteObject(hBitmap);
          ReleaseDC(hWnd, hDC);
        }
        return TRUE;
      }
    }
  }
  else if (uMsg == AEM_STARTPRINTDOC)
  {
    FOLDWINDOW *lpFoldWindow;
    DWORD dwOptions;

    if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWnd, NULL))
    {
      dwOptions=(DWORD)SendMessage(hWnd, AEM_GETOPTIONS, 0, 0);

      if (!bNoPrintCollapsed != !(dwOptions & AECO_NOPRINTCOLLAPSED))
      {
        if (bNoPrintCollapsed)
          SendMessage(hWnd, AEM_SETOPTIONS, AECOOP_OR, AECO_NOPRINTCOLLAPSED);
        else
          SendMessage(hWnd, AEM_SETOPTIONS, AECOOP_XOR, AECO_NOPRINTCOLLAPSED);
        lpFoldWindow->bReturnPrintOption=TRUE;
      }
      else lpFoldWindow->bReturnPrintOption=FALSE;
    }
  }
  else if (uMsg == AEM_ENDPRINTDOC)
  {
    FOLDWINDOW *lpFoldWindow;

    if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWnd, NULL))
    {
      if (lpFoldWindow->bReturnPrintOption)
      {
        if (bNoPrintCollapsed)
          SendMessage(hWnd, AEM_SETOPTIONS, AECOOP_XOR, AECO_NOPRINTCOLLAPSED);
        else
          SendMessage(hWnd, AEM_SETOPTIONS, AECOOP_OR, AECO_NOPRINTCOLLAPSED);
      }
    }
  }
  else if (uMsg == WM_LBUTTONDOWN ||
           uMsg == WM_LBUTTONDBLCLK ||
           uMsg == WM_SETCURSOR)
  {
    FOLDWINDOW *lpFoldWindow;
    AEFOLD *lpClick=NULL;
    AEFOLD *lpDeepless;
    RECT rcClickable;
    POINT pt;
    int nClickLine;
    BOOL bWithChildren;

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWnd, NULL))
    {
      if (lpFoldWindow->pfwd->lpSyntaxFile)
      {
        rcClickable.left=lpFoldWindow->rcBoard.left + BOARD_EDGE;
        rcClickable.top=lpFoldWindow->rcBoard.top;
        rcClickable.right=lpFoldWindow->rcBoard.right - BOARD_EDGE;
        rcClickable.bottom=lpFoldWindow->rcBoard.bottom;

        if (PtInRect(&rcClickable, pt))
        {
          nClickLine=(int)SendMessage(hWnd, AEM_LINEFROMVPOS, AECT_CLIENT, pt.y);

          if (lpClick=FoldGet(lpFoldWindow, AEFF_FINDLINE|AEFF_FOLDSTART|AEFF_RECURSE, nClickLine, NULL))
          {
            if (lpClick->lpMinPoint->ciPoint.nLine == nClickLine)
            {
              //If folds start at the same line, use deepless
              for (lpDeepless=lpClick->parent; lpDeepless; lpDeepless=lpDeepless->parent)
              {
                if (lpDeepless->lpMinPoint->ciPoint.nLine == nClickLine)
                  lpClick=lpDeepless;
                else
                  break;
              }
              if (GetKeyState(VK_CONTROL) < 0)
                bWithChildren=TRUE;
              else
                bWithChildren=FALSE;

              if (uMsg == WM_SETCURSOR)
                SetCursor(hCursorArrow);
              else
                FoldSwitchCollapse(lpFoldWindow, lpClick, bWithChildren?AECF_RECURSE:0);
              *lResult=1;
              return TRUE;
            }
          }
        }
      }
    }
  }
  else if (uMsg == WM_SETFOCUS)
  {
    MANUALSET *lpManual;

    if (bFocusResetList || ((lpManual=StackGetManual(&hManualStack, hWnd, NULL)) && lpManual->lpFoldWindow && ((FOLDWINDOW *)lpManual->lpFoldWindow)->lpUser))
      lpCurrentFoldWindow=SetActiveEdit(hWnd, hWndCodeFoldList, SAE_RESETLIST);
    bFocusResetList=FALSE;
  }
  else if (uMsg == WM_KILLFOCUS)
  {
    MANUALSET *lpManual;

    if ((lpManual=StackGetManual(&hManualStack, hWnd, NULL)) && lpManual->lpFoldWindow && ((FOLDWINDOW *)lpManual->lpFoldWindow)->lpUser)
      bFocusResetList=TRUE;
  }
  return FALSE;
}

BOOL CALLBACK CurrentCollapseProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AEFOLD *lpFold;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      if (lpFold=GetCaretFold(lpCurrentFoldWindow, NULL))
      {
        FoldSwitchCollapse(lpCurrentFoldWindow, lpFold, 0);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK CurrentGoBeginProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AEFOLD *lpFold;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      if (lpFold=GetCaretFold(lpCurrentFoldWindow, NULL))
      {
        FoldInView(lpCurrentFoldWindow, lpFold, IDM_CODEFOLD_GOBEGIN);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK CurrentGoEndProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AEFOLD *lpFold;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      if (lpFold=GetCaretFold(lpCurrentFoldWindow, NULL))
      {
        FoldInView(lpCurrentFoldWindow, lpFold, IDM_CODEFOLD_GOEND);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK CurrentSelectProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AEFOLD *lpFold;
  AECHARRANGE crSel;
  AECHARINDEX ciEndOfPoint;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      if (lpFold=GetCaretFold(lpCurrentFoldWindow, NULL))
      {
        //If selection covers caret fold, then select parent fold
        if (SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_EXGETSEL, (WPARAM)&crSel.ciMin, (LPARAM)&crSel.ciMax))
        {
          EndOfPoint(lpCurrentFoldWindow, lpFold->lpMaxPoint, &ciEndOfPoint);

          if (!AEC_IndexCompare(&crSel.ciMin, &lpFold->lpMinPoint->ciPoint) &&
              !AEC_IndexCompare(&crSel.ciMax, &ciEndOfPoint))
          {
            if (lpFold->parent)
              lpFold=lpFold->parent;
          }
        }
        FoldSelect(lpCurrentFoldWindow, lpFold);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK AllCollapseProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      FoldSwitchCollapse(lpCurrentFoldWindow, NULL, AECF_RECURSE);
    }
  }
  return TRUE;
}

BOOL CALLBACK AllNextLevelProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AEFOLD *lpFold=NULL;
  AEFOLD *lpPrevFold=NULL;
  AEFOLD *lpNextFold=NULL;
  AECHARINDEX ciGoto={0};
  AECHARINDEX ciCaret;
  AECHARINDEX ciEndOfPoint;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);

      if (lpFold=GetCaretFold(lpCurrentFoldWindow, &lpPrevFold))
      {
        EndOfPoint(lpCurrentFoldWindow, lpFold->lpMaxPoint, &ciEndOfPoint);

        if (!AEC_IndexCompare(&ciEndOfPoint, &ciCaret))
        {
          //Right to left
          lpNextFold=lpFold;

          if (lpNextFold->next)
          {
            for (lpNextFold=lpNextFold->next; lpNextFold->firstChild; lpNextFold=lpNextFold->firstChild);
          }
          else lpNextFold=lpNextFold->parent;
        }
        else
        {
          //Left to right
          lpNextFold=AEC_NextFold(lpFold, TRUE);
        }

        if (lpNextFold)
        {
          if (!AEC_IndexCompare(&ciEndOfPoint, &ciCaret))
            EndOfPoint(lpCurrentFoldWindow, lpNextFold->lpMaxPoint, &ciGoto);
          else
            ciGoto=lpNextFold->lpMinPoint->ciPoint;
        }
      }
      else if (lpPrevFold)
      {
        if (lpNextFold=AEC_NextFold(lpPrevFold, FALSE))
        {
          ciGoto=lpNextFold->lpMinPoint->ciPoint;
        }
      }

      if (ciGoto.lpLine)
      {
        AESELECTION aes;

        aes.crSel.ciMin=ciGoto;
        aes.crSel.ciMax=ciGoto;
        aes.dwFlags=AESELT_LOCKSCROLL;
        aes.dwType=0;
        SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);

        ScrollToPoint(lpCurrentFoldWindow->hWndEdit, NULL);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK AllPrevLevelProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AEFOLD *lpFold=NULL;
  AEFOLD *lpPrevFold=NULL;
  AECHARINDEX ciGoto={0};
  AECHARINDEX ciCaret;
  AECHARINDEX ciEndOfPoint;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);

      if (lpFold=GetCaretFold(lpCurrentFoldWindow, &lpPrevFold))
      {
        EndOfPoint(lpCurrentFoldWindow, lpFold->lpMaxPoint, &ciEndOfPoint);

        if (!AEC_IndexCompare(&ciEndOfPoint, &ciCaret))
        {
          //Left to right
          lpPrevFold=AEC_PrevFold(lpFold, TRUE);
        }
        else
        {
          //Right to left
          lpPrevFold=lpFold;

          if (!AEC_IndexCompare(&lpFold->lpMinPoint->ciPoint, &ciCaret))
          {
            if (lpPrevFold->prev)
            {
              for (lpPrevFold=lpPrevFold->prev; lpPrevFold->lastChild; lpPrevFold=lpPrevFold->lastChild);
            }
            else lpPrevFold=lpPrevFold->parent;
          }
        }

        if (lpPrevFold)
        {
          if (!AEC_IndexCompare(&ciEndOfPoint, &ciCaret))
            EndOfPoint(lpCurrentFoldWindow, lpPrevFold->lpMaxPoint, &ciGoto);
          else
            ciGoto=lpPrevFold->lpMinPoint->ciPoint;
        }
      }
      else if (lpPrevFold)
      {
        while (lpPrevFold->lastChild)
          lpPrevFold=lpPrevFold->lastChild;
        ciGoto=lpPrevFold->lpMinPoint->ciPoint;
      }

      if (ciGoto.lpLine)
      {
        AESELECTION aes;

        aes.crSel.ciMin=ciGoto;
        aes.crSel.ciMax=ciGoto;
        aes.dwFlags=AESELT_LOCKSCROLL;
        aes.dwType=0;
        SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);

        ScrollToPoint(lpCurrentFoldWindow->hWndEdit, NULL);
      }
    }
  }
  return TRUE;
}

BOOL CALLBACK FindRootLevelProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
    {
      if (lpCurrentFoldWindow->pfwd->lpFoldStack->first)
      {
        if (!SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_GETLINENUMBER, AEGL_UNWRAPSELMULTILINE, 0))
        {
          AECHARRANGE crInitRange;
          AECHARRANGE crSelRange;
          EXGETTEXTRANGE tr;
          wchar_t *wszText;
          BOOL bFound=FALSE;

          if (!(wszText=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)lpCurrentFoldWindow->hWndEdit, (LPARAM)NULL)))
          {
            AECHARINDEX ciCaret;

            //Get word under caret
            SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
            tr.cr.ciMin=ciCaret;
            tr.cr.ciMax=ciCaret;

            if (SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_GETPREVBREAK, AEWB_LEFTWORDSTART|AEWB_LEFTWORDEND|AEWB_MINMOVE, (LPARAM)&tr.cr.ciMin) &&
                SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_GETNEXTBREAK, AEWB_RIGHTWORDSTART|AEWB_RIGHTWORDEND|AEWB_MINMOVE, (LPARAM)&tr.cr.ciMax))
            {
              tr.bColumnSel=FALSE;
              tr.pText=NULL;
              tr.nNewLine=AELB_ASIS;
              if (SendMessage(hMainWnd, AKD_EXGETTEXTRANGEW, (WPARAM)lpCurrentFoldWindow->hWndEdit, (LPARAM)&tr))
                wszText=(wchar_t *)tr.pText;
            }
            crInitRange.ciMin=tr.cr.ciMin;
            crInitRange.ciMax=tr.cr.ciMax;
          }
          else SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_EXGETSEL, (WPARAM)&crInitRange.ciMin, (LPARAM)&crInitRange.ciMax);

          if (wszText)
          {
            AEFINDTEXTW ft;
            AECHARRANGE crNameRange;
            AEFOLD *lpFold;
            AEFOLD *lpParent;
            AEFOLD *lpPrevFold=NULL;
            int nCurDepth=0;
            BOOL bFindFromBeginning=FALSE;

            if (lpCurrentFoldWindow->pfwd->lpFoldStack->first)
            {
              lpFold=GetCaretFold(lpCurrentFoldWindow, &lpPrevFold);
              if (lpPrevFold)
                lpFold=AEC_NextFold(lpPrevFold, FALSE);
              if (!lpFold)
                lpFold=(AEFOLD *)lpCurrentFoldWindow->pfwd->lpFoldStack->first;
              for (lpParent=lpFold->parent; lpParent; lpParent=lpParent->parent)
                ++nCurDepth;

              for (;;)
              {
                for (; lpFold; lpFold=FoldNextDepth(lpFold, &nCurDepth, nFindRootMaxDepth))
                {
                  if (FoldData(lpFold)->wpName && xstrstrW(FoldData(lpFold)->wpName, -1, wszText, -1, FALSE, NULL, NULL))
                  {
                    if (GetFoldName(FoldData(lpFold), &lpFold->lpMinPoint->ciPoint, &crNameRange))
                    {
                      if (bFindFromBeginning)
                      {
                        if (AEC_IndexCompare(&crNameRange.ciMin, &crInitRange.ciMax) > 0)
                          break;
                      }
                      else
                      {
                        if (AEC_IndexCompare(&crNameRange.ciMax, &crInitRange.ciMin) <= 0)
                          continue;
                      }

                      ft.dwFlags=AEFR_DOWN;
                      ft.pText=wszText;
                      ft.dwTextLen=(UINT_PTR)-1;
                      ft.nNewLine=AELB_ASIS;
                      if (!bFindFromBeginning && AEC_IndexCompare(&crNameRange.ciMin, &crInitRange.ciMax) < 0)
                        ft.crSearch.ciMin=crInitRange.ciMax;
                      else
                        ft.crSearch.ciMin=crNameRange.ciMin;
                      ft.crSearch.ciMax=crNameRange.ciMax;

                      if (SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_FINDTEXTW, 0, (LPARAM)&ft))
                      {
                        crSelRange.ciMin=ft.crFound.ciMin;
                        crSelRange.ciMax=ft.crFound.ciMax;
                        bFound=TRUE;
                        break;
                      }
                    }
                  }
                }
                if (bFound) break;

                if (!bFindFromBeginning)
                {
                  bFindFromBeginning=TRUE;
                  lpFold=(AEFOLD *)lpCurrentFoldWindow->pfwd->lpFoldStack->first;
                  continue;
                }
                break;
              }
            }
            SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wszText);
          }

          if (bFound)
          {
            AESELECTION aes;

            aes.crSel.ciMin=crSelRange.ciMin;
            aes.crSel.ciMax=crSelRange.ciMax;
            aes.dwFlags=AESELT_LOCKSCROLL;
            aes.dwType=0;
            SendMessage(lpCurrentFoldWindow->hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);

            ScrollToPoint(lpCurrentFoldWindow->hWndEdit, NULL);
            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

SKIPINFO* StackInsertSkipInfo(STACKSKIP *hStack)
{
  SKIPINFO *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(SKIPINFO));
  return lpElement;
}

SKIPSTART* StackInsertSkipStart(HSTACK *hSkipStartStack, SKIPINFO *lpSkipInfo, wchar_t *wpSkipStart, int nSkipStartLen)
{
  SKIPSTART *lpSkipStart=(SKIPSTART *)hSkipStartStack->first;
  SKIPINFOHANDLE *lpSkipInfoHandle;

  while (lpSkipStart)
  {
    if (lpSkipStart->nSkipStartLen == nSkipStartLen)
    {
      if (!(lpSkipStart->dwFlags & FIF_MATCHCASE) == !(lpSkipInfo->dwFlags & FIF_MATCHCASE))
      {
        if (((lpSkipStart->dwFlags & FIF_MATCHCASE) && !xstrcmpW(lpSkipStart->wpSkipStart, wpSkipStart)) ||
            (!(lpSkipStart->dwFlags & FIF_MATCHCASE) && !xstrcmpiW(lpSkipStart->wpSkipStart, wpSkipStart)))
        {
          break;
        }
      }
    }
    lpSkipStart=lpSkipStart->next;
  }

  if (!lpSkipStart)
  {
    if (!StackInsertIndex((stack **)&hSkipStartStack->first, (stack **)&hSkipStartStack->last, (stack **)&lpSkipStart, -1, sizeof(SKIPSTART)))
    {
      lpSkipStart->dwFlags=lpSkipInfo->dwFlags;
      lpSkipStart->wpSkipStart=wpSkipStart;
      lpSkipStart->nSkipStartLen=nSkipStartLen;

      if (lpSkipStart->dwFlags & FIF_REGEXPSTART)
      {
        lpSkipStart->sregStart.dwOptions=REO_MULTILINE|(lpSkipStart->dwFlags & FIF_MATCHCASE?REO_MATCHCASE:0);
        if (nSkipStartLen && !PatCompile(&lpSkipStart->sregStart, wpSkipStart, wpSkipStart + nSkipStartLen))
        {
          lpSkipStart->sregStart.first->dwFlags&=~REGF_ROOTANY;
          if (lpSkipStart->sregStart.first->nGroupLen == -1)
          {
            xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_FIXEDLENGTH), lpLoadSyntaxFile->wszSyntaxFileName, nSkipStartLen, wpSkipStart);
            MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
            PatFree(&lpSkipStart->sregStart);
            StackDelete((stack **)&hSkipStartStack->first, (stack **)&hSkipStartStack->last, (stack *)lpSkipStart);
            return NULL;
          }
        }
        else
        {
          xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_COMPILEERROR), lpLoadSyntaxFile->wszSyntaxFileName, nSkipStartLen, wpSkipStart);
          MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
          StackDelete((stack **)&hSkipStartStack->first, (stack **)&hSkipStartStack->last, (stack *)lpSkipStart);
          return NULL;
        }
      }
    }
  }
  else GlobalFree((HGLOBAL)wpSkipStart);

  if (!StackInsertIndex((stack **)&lpSkipStart->hSkipInfoHandleStack.first, (stack **)&lpSkipStart->hSkipInfoHandleStack.last, (stack **)&lpSkipInfoHandle, -1, sizeof(SKIPINFOHANDLE)))
  {
    lpSkipInfoHandle->lpSkipInfo=lpSkipInfo;
    ++lpSkipStart->nElements;
  }
  return lpSkipStart;
}

void StackDeleteSkipInfo(STACKSKIP *hSkipStack, HSTACK *hSkipStartStack, SKIPINFO *lpSkipInfo)
{
  if (lpSkipInfo->lpSkipStart)
  {
    StackClear((stack **)&lpSkipInfo->lpSkipStart->hSkipInfoHandleStack.first, (stack **)&lpSkipInfo->lpSkipStart->hSkipInfoHandleStack.last);
    if (lpSkipInfo->lpSkipStart->wpSkipStart) GlobalFree((HGLOBAL)lpSkipInfo->lpSkipStart->wpSkipStart);
    PatFree(&lpSkipInfo->lpSkipStart->sregStart);
    StackDelete((stack **)&hSkipStartStack->first, (stack **)&hSkipStartStack->last, (stack *)lpSkipInfo->lpSkipStart);
  }
  if (lpSkipInfo->wpSkipEnd) GlobalFree((HGLOBAL)lpSkipInfo->wpSkipEnd);
  PatFree(&lpSkipInfo->sregEnd);
  StackDelete((stack **)&hSkipStack->first, (stack **)&hSkipStack->last, (stack *)lpSkipInfo);
}

void StackFreeSkipInfo(STACKSKIP *hSkipStack, HSTACK *hSkipStartStack)
{
  SKIPINFO *lpSkipInfo;
  SKIPSTART *lpSkipStart;

  for (lpSkipStart=(SKIPSTART *)hSkipStartStack->first; lpSkipStart; lpSkipStart=lpSkipStart->next)
  {
    StackClear((stack **)&lpSkipStart->hSkipInfoHandleStack.first, (stack **)&lpSkipStart->hSkipInfoHandleStack.last);
    GlobalFree((HGLOBAL)lpSkipStart->wpSkipStart);
    PatFree(&lpSkipStart->sregStart);
  }
  StackClear((stack **)&hSkipStartStack->first, (stack **)&hSkipStartStack->last);

  for (lpSkipInfo=(SKIPINFO *)hSkipStack->first; lpSkipInfo; lpSkipInfo=lpSkipInfo->next)
  {
    GlobalFree((HGLOBAL)lpSkipInfo->wpSkipEnd);
    PatFree(&lpSkipInfo->sregEnd);
  }
  StackClear((stack **)&hSkipStack->first, (stack **)&hSkipStack->last);
}

FOLDINFO* StackInsertFoldInfo(STACKFOLD *hStack)
{
  FOLDINFO *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(FOLDINFO));
  return lpElement;
}

FOLDSTART* StackInsertFoldStart(HSTACK *hFoldStartStack, FOLDINFO *lpFoldInfo, wchar_t *wpFoldStart, int nFoldStartLen)
{
  FOLDSTART *lpFoldStart=(FOLDSTART *)hFoldStartStack->first;
  FOLDINFOHANDLE *lpFoldInfoHandle;

  while (lpFoldStart)
  {
    if (lpFoldStart->nFoldStartLen == nFoldStartLen)
    {
      if (!(lpFoldStart->dwFlags & FIF_MATCHCASE) == !(lpFoldInfo->dwFlags & FIF_MATCHCASE))
      {
        if (((lpFoldStart->dwFlags & FIF_MATCHCASE) && !xstrcmpW(lpFoldStart->wpFoldStart, wpFoldStart)) ||
            (!(lpFoldStart->dwFlags & FIF_MATCHCASE) && !xstrcmpiW(lpFoldStart->wpFoldStart, wpFoldStart)))
        {
          break;
        }
      }
    }
    lpFoldStart=lpFoldStart->next;
  }

  if (!lpFoldStart)
  {
    if (!StackInsertIndex((stack **)&hFoldStartStack->first, (stack **)&hFoldStartStack->last, (stack **)&lpFoldStart, -1, sizeof(FOLDSTART)))
    {
      lpFoldStart->dwFlags=lpFoldInfo->dwFlags;
      lpFoldStart->wpFoldStart=wpFoldStart;
      lpFoldStart->nFoldStartLen=nFoldStartLen;
      lpFoldStart->nFoldStartPointLen=nFoldStartLen;

      if (lpFoldStart->dwFlags & FIF_REGEXPSTART)
      {
        lpFoldStart->sregStart.dwOptions=REO_MULTILINE|(lpFoldStart->dwFlags & FIF_MATCHCASE?REO_MATCHCASE:0);;
        if (nFoldStartLen && !PatCompile(&lpFoldStart->sregStart, wpFoldStart, wpFoldStart + nFoldStartLen))
        {
          lpFoldStart->sregStart.first->dwFlags&=~REGF_ROOTANY;
          lpFoldStart->nFoldStartPointLen=(int)lpFoldStart->sregStart.first->nGroupLen;
          if (lpFoldStart->nFoldStartPointLen == -1)
          {
            xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_FIXEDLENGTH), lpLoadSyntaxFile->wszSyntaxFileName, nFoldStartLen, wpFoldStart);
            MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
            PatFree(&lpFoldStart->sregStart);
            StackDelete((stack **)&hFoldStartStack->first, (stack **)&hFoldStartStack->last, (stack *)lpFoldStart);
            return NULL;
          }
        }
        else
        {
          xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_COMPILEERROR), lpLoadSyntaxFile->wszSyntaxFileName, nFoldStartLen, wpFoldStart);
          MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
          StackDelete((stack **)&hFoldStartStack->first, (stack **)&hFoldStartStack->last, (stack *)lpFoldStart);
          return NULL;
        }
      }
    }
  }
  else GlobalFree((HGLOBAL)wpFoldStart);

  if (!StackInsertIndex((stack **)&lpFoldStart->hFoldInfoHandleStack.first, (stack **)&lpFoldStart->hFoldInfoHandleStack.last, (stack **)&lpFoldInfoHandle, -1, sizeof(FOLDINFOHANDLE)))
  {
    lpFoldInfoHandle->lpFoldInfo=lpFoldInfo;
    ++lpFoldStart->nElements;
  }
  return lpFoldStart;
}

void StackDeleteFoldInfo(STACKFOLD *hFoldStack, HSTACK *hFoldStartStack, FOLDINFO *lpFoldInfo)
{
  if (lpFoldInfo->lpFoldStart)
  {
    StackClear((stack **)&lpFoldInfo->lpFoldStart->hFoldInfoHandleStack.first, (stack **)&lpFoldInfo->lpFoldStart->hFoldInfoHandleStack.last);
    if (lpFoldInfo->lpFoldStart->wpFoldStart) GlobalFree((HGLOBAL)lpFoldInfo->lpFoldStart->wpFoldStart);
    PatFree(&lpFoldInfo->lpFoldStart->sregStart);
    StackDelete((stack **)&hFoldStartStack->first, (stack **)&hFoldStartStack->last, (stack *)lpFoldInfo->lpFoldStart);
  }
  if (lpFoldInfo->wpFoldEnd) GlobalFree((HGLOBAL)lpFoldInfo->wpFoldEnd);
  if (lpFoldInfo->wpDelimiters) GlobalFree((HGLOBAL)lpFoldInfo->wpDelimiters);
  PatFree(&lpFoldInfo->sregEnd);
  StackDelete((stack **)&hFoldStack->first, (stack **)&hFoldStack->last, (stack *)lpFoldInfo);
}

void StackFreeFoldInfo(STACKFOLD *hFoldStack, HSTACK *hFoldStartStack)
{
  FOLDINFO *lpFoldInfo;
  FOLDSTART *lpFoldStart;

  for (lpFoldStart=(FOLDSTART *)hFoldStartStack->first; lpFoldStart; lpFoldStart=lpFoldStart->next)
  {
    StackClear((stack **)&lpFoldStart->hFoldInfoHandleStack.first, (stack **)&lpFoldStart->hFoldInfoHandleStack.last);
    GlobalFree((HGLOBAL)lpFoldStart->wpFoldStart);
    PatFree(&lpFoldStart->sregStart);
  }
  StackClear((stack **)&hFoldStartStack->first, (stack **)&hFoldStartStack->last);

  for (lpFoldInfo=(FOLDINFO *)hFoldStack->first; lpFoldInfo; lpFoldInfo=lpFoldInfo->next)
  {
    GlobalFree((HGLOBAL)lpFoldInfo->wpFoldEnd);
    GlobalFree((HGLOBAL)lpFoldInfo->wpDelimiters);
    PatFree(&lpFoldInfo->sregEnd);
  }
  StackClear((stack **)&hFoldStack->first, (stack **)&hFoldStack->last);
}

void StackEndBoard(STACKFOLDWINDOW *hStack, FOLDWINDOW *lpFoldWindow)
{
  FOLDWINDOW *lpElement;
  HWND hWndCurEdit;
  int nWidth;

  if (!lpFoldWindow)
    hStack->bLock=TRUE;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpFoldWindow == NULL || lpFoldWindow == lpElement)
    {
      if (lpElement->rcBoard.right)
      {
        //Remove rect
        nWidth=lpElement->rcBoard.right - lpElement->rcBoard.left;
        lpElement->rcBoard.left=0;
        lpElement->rcBoard.right=0;
        SetEditRect(lpElement->hDocEdit, lpElement->hWndEdit, 0, nWidth);
      }
    }
  }

  if (nMDI == WMD_PMDI)
  {
    if (lpFoldWindow == NULL)
      hWndCurEdit=GetCurEdit();
    else
      hWndCurEdit=lpFoldWindow->hWndEdit;
    SendMessage(hWndCurEdit, AEM_UPDATESCROLLBAR, SB_BOTH, 0);
    SendMessage(hWndCurEdit, AEM_UPDATECARET, 0, 0);
  }
  if (!lpFoldWindow)
    hStack->bLock=FALSE;
}

FOLDWINDOW* StackInsertFoldWindow(STACKFOLDWINDOW *hStack)
{
  FOLDWINDOW *lpFoldWindow=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpFoldWindow, -1, sizeof(FOLDWINDOW));

  return lpFoldWindow;
}

FOLDWINDOW* StackGetFoldWindow(STACKFOLDWINDOW *hStack, HWND hWndEdit, AEHDOC hDocEdit)
{
  FOLDWINDOW *lpFoldWindow=NULL;

  if (hStack->bLock) return NULL;

  if (lpCurrentFoldWindow)
  {
    if (lpCurrentFoldWindow->hWndEdit == hWndEdit)
    {
      lpFoldWindow=lpCurrentFoldWindow;
      if (lpFoldWindow->lpUser)
        return lpFoldWindow;
    }
  }
  if (!lpFoldWindow)
  {
    for (lpFoldWindow=hStack->first; lpFoldWindow; lpFoldWindow=lpFoldWindow->next)
    {
      if (lpFoldWindow->hWndEdit == hWndEdit)
      {
        if (lpFoldWindow->lpUser)
          return lpFoldWindow;
        break;
      }
    }
  }

  if (lpFoldWindow && nMDI == WMD_PMDI)
  {
    if (!hDocEdit)
      hDocEdit=(AEHDOC)SendMessage(hWndEdit, AEM_GETDOCUMENT, 0, 0);

    if (lpCurrentFoldWindow)
    {
      if (lpCurrentFoldWindow->hDocEdit == hDocEdit)
        return lpCurrentFoldWindow;
    }
    for (lpFoldWindow=hStack->first; lpFoldWindow; lpFoldWindow=lpFoldWindow->next)
    {
      if (lpFoldWindow->hDocEdit == hDocEdit)
        return lpFoldWindow;
    }
  }
  return lpFoldWindow;
}

void StackDeleteFoldWindow(STACKFOLDWINDOW *hStack, FOLDWINDOW *lpFoldWindow)
{
  if (lpCurrentFoldWindow == lpFoldWindow)
    lpCurrentFoldWindow=NULL;

  if (lpFoldWindow->pfwd == &lpFoldWindow->fwd)
    FreeFolds(lpFoldWindow, FALSE);
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpFoldWindow);
}

void StackFreeFoldWindows(STACKFOLDWINDOW *hStack)
{
  FOLDWINDOW *lpFoldWindow;
  FOLDWINDOW *lpNextFoldWindow;
  HWND hWndCurEdit;

  for (lpFoldWindow=hStack->first; lpFoldWindow; lpFoldWindow=lpNextFoldWindow)
  {
    lpNextFoldWindow=lpFoldWindow->next;

    if (lpFoldWindow->lpUser)
      StackDeleteManual(&hManualStack, lpFoldWindow->lpUser, CODER_CODEFOLD);
    else
      FreeFolds(lpFoldWindow, TRUE);
  }

  if (nMDI == WMD_PMDI)
  {
    hWndCurEdit=GetCurEdit();
    SendMessage(hWndCurEdit, AEM_UPDATESCROLLBAR, SB_BOTH, 0);
    SendMessage(hWndCurEdit, AEM_UPDATECARET, 0, 0);
  }
  lpCurrentFoldWindow=NULL;

  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

FOLDWINDOW* FillLevelsStack(FOLDWINDOW *lpFoldWindow, STACKLEVEL *hLevelStack, HWND hWnd, AECHARRANGE *crRange)
{
  SYNTAXFILE *lpSyntaxFile;
  AEFINDTEXTW ft;
  AECHARRANGE crScan;
  AECHARINDEX ciCount;
  AECHARINDEX ciOpenTag;
  AECHARINDEX ciCloseTag;
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;
  LEVEL *lpLevel=NULL;
  LEVEL *lpParent=NULL;
  FOLDINFO *lpFoldInfo;
  SKIPINFO *lpSkipInfo=NULL;
  SKIPSTART *lpSkipStart;
  SKIPINFOHANDLE *lpSkipInfoHandle;
  int nFirstVisibleLine=0;
  int nTagNameLen;
  DWORD dwMatchResult=0;

  if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, NULL, NULL))
  {
    if (!lpFoldWindow || hLevelStack != &lpFoldWindow->pfwd->hTempStack)
    {
      if (SendMessage(hMainWnd, AKD_GETTEXTLENGTH, (WPARAM)hWnd, 0) > nCharsLimit)
        lpSyntaxFile=NULL;
    }
    if (lpSyntaxFile)
    {
      StackRequestSyntaxFile(lpSyntaxFile);
      if (!lpSyntaxFile->hFoldStack.first)
        lpSyntaxFile=NULL;
    }
  }

  if (!lpFoldWindow)
  {
    if (lpFoldWindow=StackInsertFoldWindow(&hFoldWindowsStack))
    {
      lpFoldWindow->hWndEdit=hWnd;
      lpFoldWindow->hDocEdit=(AEHDOC)SendMessage(hWnd, AEM_GETDOCUMENT, 0, 0);

      //Associate data
      {
        FOLDWINDOW *lpFoldMasterWindow;
        HWND hWndMaster=(HWND)SendMessage(hWnd, AEM_GETMASTER, 0, 0);

        if (hWndMaster && hWndMaster != hWnd)
        {
          //hWnd is clone window
          if (lpFoldMasterWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndMaster, NULL))
          {
            lpFoldWindow->pfwd=&lpFoldMasterWindow->fwd;
            return lpFoldWindow;
          }
          else if (lpFoldMasterWindow=StackInsertFoldWindow(&hFoldWindowsStack))
          {
            lpFoldMasterWindow->hWndEdit=hWndMaster;
            lpFoldMasterWindow->hDocEdit=(AEHDOC)SendMessage(hWndMaster, AEM_GETDOCUMENT, 0, 0);
            lpFoldMasterWindow->pfwd=&lpFoldMasterWindow->fwd;
            lpFoldMasterWindow->pfwd->lpSyntaxFile=lpSyntaxFile;
            lpFoldMasterWindow->pfwd->lpFoldStack=(HSTACK *)SendMessage(hWndMaster, AEM_GETFOLDSTACK, 0, 0);

            lpFoldWindow->pfwd=&lpFoldMasterWindow->fwd;
          }
        }
        else
        {
          lpFoldWindow->pfwd=&lpFoldWindow->fwd;
          lpFoldWindow->pfwd->lpSyntaxFile=lpSyntaxFile;
          lpFoldWindow->pfwd->lpFoldStack=(HSTACK *)SendMessage(hWnd, AEM_GETFOLDSTACK, 0, 0);
        }
      }
      hLevelStack=&lpFoldWindow->pfwd->hLevelStack;
    }
  }
  else
  {
    if (lpFoldWindow->hWndEdit != hWnd)
      return NULL;
    lpFoldWindow->pfwd->lpSyntaxFile=lpSyntaxFile;
  }

  if (lpFoldWindow && lpFoldWindow->pfwd->lpSyntaxFile)
  {
    SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ciFirstChar);
    SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ciLastChar);

    if (crRange)
    {
      crScan.ciMin=crRange->ciMin;
      crScan.ciMax=crRange->ciMax;
      //SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEBEGIN, (LPARAM)&crScan.ciMin);
      //SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEEND, (LPARAM)&crScan.ciMax);
    }
    else
    {
      crScan.ciMin=ciFirstChar;
      crScan.ciMax=ciLastChar;
      if (bCollapseOnOpen) nFirstVisibleLine=(int)SendMessage(hWnd, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0);
    }
    ciCount=crScan.ciMin;

    while (ciCount.lpLine)
    {
      CheckChar:
      while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
      {
        if (!lpParent && AEC_IndexCompare(&ciCount, &crScan.ciMax) >= 0)
          goto Close;

        if (!lpSkipInfo)
        {
          if (lpFoldInfo=IsFold(lpFoldWindow, lpLevel, &ft, &ciCount, &dwMatchResult))
          {
            if (dwMatchResult & IFE_FOLDEND)
            {
              if (lpLevel)
              {
                lpParent=lpLevel->lpParent;
                if (lpParent && lpLevel->bMultiLine)
                  lpParent->bMultiLine=TRUE;

                if ((lpFoldInfo->dwFlags & FIF_DENYFOLD) ||
                    (!bTagMarkEnable && !lpLevel->bMultiLine))
                {
                  if ((lpFoldInfo->dwFlags & FIF_DENYFOLD) && lpFoldInfo->lpFoldStart->nElements > 1)
                  {
                    lpParent=lpLevel;
                    ciCount=ft.crFound.ciMax;
                    goto CheckChar;
                  }
                  DeleteFoldData(lpLevel->pfd);
                  StackDeleteLevel(hLevelStack, lpLevel);
                  lpLevel=lpParent;
                  goto CheckNoCatch;
                }
                if (lpFoldInfo->dwFlags & FIF_FOLDEND_NOCATCH)
                {
                  lpLevel->pointMax.nPointLen=0;
                }
                else if ((lpFoldInfo->dwFlags & FIF_XMLTAG) &&
                         (lpFoldInfo->dwFlags & FIF_XMLNONAME_TWOTAG))
                {
                  AEC_NextCharEx(&lpLevel->crFoundMin.ciMin, &ciOpenTag);
                  AEC_ValidCharInLine(&ft.crFound.ciMax);
                  ciCloseTag=ft.crFound.ciMax;

                  //Expand XML tag from "</" to "</name>"
                  lpLevel->pointMax.nPointLen=lpFoldInfo->nFoldEndPointLen;

                  do
                  {
                    ++lpLevel->pointMax.nPointLen;

                    if (AEC_CharAtIndex(&ft.crFound.ciMax) == L'>')
                    {
                      AEC_NextCharInLine(&ft.crFound.ciMax);
                      break;
                    }
                  }
                  while (AEC_NextCharInLine(&ft.crFound.ciMax));

                  //Compare open and close tag names
                  if (lpLevel->pointMin.nPointLen - 1 == // 1 - <
                      lpLevel->pointMax.nPointLen - 3)   // 3 - </>
                  {
                    nTagNameLen=lpLevel->pointMin.nPointLen - 1;

                    do
                    {
                      if (WideCharLower((wchar_t)AEC_CharAtIndex(&ciCloseTag)) !=
                          WideCharLower((wchar_t)AEC_CharAtIndex(&ciOpenTag)))
                        break;
                      if (!--nTagNameLen) goto SetLevelMax;
                    }
                    while (AEC_NextCharInLine(&ciCloseTag) && AEC_NextCharInLine(&ciOpenTag));
                  }
                  ciCount=ft.crFound.ciMax;
                  lpParent=lpLevel;
                  goto CheckChar;
                }
                else lpLevel->pointMax.nPointLen=lpFoldInfo->nFoldEndPointLen;

                SetLevelMax:
                lpLevel->pfd->lpFoldInfo=lpFoldInfo;
                lpLevel->pointMax.ciPoint=ft.crFound.ciMin;
                lpLevel->pointMax.nPointOffset=AEPTO_CALC;
                lpLevel->crFoundMax.ciMin=ft.crFound.ciMin;
                lpLevel->crFoundMax.ciMax=ft.crFound.ciMax;
                if (hLevelStack != &lpFoldWindow->pfwd->hTempStack)
                {
                  //Create fold
                  CreateFold(lpLevel, hWnd, (bCollapseOnOpen && !crRange && !lpLevel->lpParent));

                  //LEVEL item now is not needed
                  StackDeleteLevel(hLevelStack, lpLevel);
                }
                lpLevel=lpParent;
              }
            }
            if (dwMatchResult & IFE_FOLDSTART)
            {
              if ((lpFoldInfo->dwFlags & FIF_DENYFOLD) && !lpFoldInfo->nFoldEndLen)
              {
                //Ignore this fold start
              }
              else
              {
                if (lpLevel=StackInsertLevel(hLevelStack, &ft.crFound.ciMin))
                {
                  if ((lpFoldInfo->dwFlags & FIF_XMLTAG) &&
                      (lpFoldInfo->dwFlags & FIF_XMLNONAME_TWOTAG))
                  {
                    lpLevel->pointMin.nPointLen=lpFoldInfo->lpFoldStart->nFoldStartPointLen;
                    AEC_ValidCharInLine(&ft.crFound.ciMax);

                    do
                    {
                      switch (AEC_CharAtIndex(&ft.crFound.ciMax))
                      {
                        case L' ':
                        case L'\t':
                        case L'/':
                        case L'>':
                          goto SetLevelMin;
                      }
                      ++lpLevel->pointMin.nPointLen;
                    }
                    while (AEC_NextCharInLine(&ft.crFound.ciMax));
                  }
                  else lpLevel->pointMin.nPointLen=lpFoldInfo->lpFoldStart->nFoldStartPointLen;

                  SetLevelMin:
                  lpLevel->lpParent=lpParent;
                  lpLevel->pfd->lpFoldInfo=lpFoldInfo;
                  lpLevel->pointMin.ciPoint=ft.crFound.ciMin;
                  lpLevel->pointMin.nPointOffset=AEPTO_CALC;
                  lpLevel->crFoundMin.ciMin=ft.crFound.ciMin;
                  lpLevel->crFoundMin.ciMax=ft.crFound.ciMax;
                  GetFoldName(lpLevel->pfd, &lpLevel->pointMin.ciPoint, NULL);

                  lpParent=lpLevel;
                }
              }
            }

            CheckNoCatch:
            if ((lpFoldInfo->dwFlags & FIF_FOLDEND_NOCATCH) && (dwMatchResult & IFE_FOLDEND))
            {
              if (lpFoldInfo->dwFlags & FIF_FOLDEND_NOCATCH_SIBLING)
                dwMatchResult=IFF_CHECKFIRSTFOLDSTART;
              ciCount=ft.crFound.ciMin;
            }
            else ciCount=ft.crFound.ciMax;

            goto CheckChar;
          }
          else dwMatchResult=0;

          //Check skips
          if (lpLevel && (lpLevel->pfd->lpFoldInfo->dwFlags & FIF_COMMENTFOLD))
          {
            //Ignore skips for comment fold end
          }
          else
          {
            //Skip start
            for (lpSkipStart=(SKIPSTART *)lpFoldWindow->pfwd->lpSyntaxFile->hSkipStartStack.first; lpSkipStart; lpSkipStart=lpSkipStart->next)
            {
              if (lpSkipInfo=IsSkipStart(lpSkipStart, &ft, &ciCount))
                goto NextChar;
            }
          }
        }
        else
        {
          //Skip end
          for (lpSkipInfoHandle=(SKIPINFOHANDLE *)lpSkipInfo->lpSkipStart->hSkipInfoHandleStack.first; lpSkipInfoHandle; lpSkipInfoHandle=lpSkipInfoHandle->next)
          {
            if (!lpSkipInfoHandle->lpSkipInfo->nSkipEndLen)
              break;
            if (IsSkipEnd(lpSkipInfoHandle->lpSkipInfo, &ft, &ciCount))
            {
              lpSkipInfo=NULL;

              if (lpSkipInfoHandle->lpSkipInfo->dwFlags & FIF_FOLDEND_NOCATCH)
                goto CheckChar;
              break;
            }
          }
        }

        NextChar:
        AEC_IndexInc(&ciCount);
      }
      if (ciCount.lpLine->nLineBreak != AELB_WRAP)
      {
        //Force one line quote
        lpSkipInfo=NULL;

        //Level contain more than one line
        if (lpLevel) lpLevel->bMultiLine=TRUE;
      }
      AEC_NextLine(&ciCount);
    }

    Close:
    //Update crRange->ciMax
    if (crRange)
    {
      if (ciCount.lpLine)
        crRange->ciMax=ciCount;
      else
        crRange->ciMax=ciLastChar;
    }

    //Close non-closed folds
    ciCount=ciLastChar;

    while (lpLevel)
    {
      lpParent=lpLevel->lpParent;

      if (!lpLevel->pointMax.ciPoint.lpLine)
      {
        //if (lpLevel->pfd->lpFoldInfo->dwFlags & FIF_FOLDEND_REQUIRED)
        //{
        //  DeleteFoldData(lpLevel->pfd);
        //  StackDeleteLevel(hLevelStack, lpLevel);
        //}
        //else
        {
          lpLevel->pointMax.ciPoint=ciCount;
          lpLevel->pointMax.nPointOffset=AEPTO_CALC;
          lpLevel->pointMax.nPointLen=0;

          if (hLevelStack != &lpFoldWindow->pfwd->hTempStack)
          {
            //Create fold
            CreateFold(lpLevel, hWnd, (bCollapseOnOpen && !crRange && !lpLevel->lpParent));

            //LEVEL item now is not needed
            StackDeleteLevel(hLevelStack, lpLevel);
          }
        }
      }
      lpLevel=lpParent;
    }

    if (bCollapseOnOpen && !crRange) SendMessage(hWnd, AEM_UPDATEFOLD, 0, (LPARAM)nFirstVisibleLine);
  }
  return lpFoldWindow;
}

void CreateFold(LEVEL *lpLevel, HWND hWnd, BOOL bCollapse)
{
  AEFOLD fold;

  fold.lpMinPoint=&lpLevel->pointMin;
  fold.lpMaxPoint=&lpLevel->pointMax;
  fold.bCollapse=bCollapse;
  fold.dwFontStyle=lpLevel->pfd->lpFoldInfo->dwFontStyle;
  fold.crText=lpLevel->pfd->lpFoldInfo->dwColor1;
  fold.crBk=lpLevel->pfd->lpFoldInfo->dwColor2;
  fold.dwUserData=(UINT_PTR)lpLevel->pfd;
  fold.dwFontStyle=fold.dwFontStyle;
  lpLevel->pfd->lpFold=(AEFOLD *)SendMessage(hWnd, AEM_ADDFOLD, 0, (LPARAM)&fold);
  lpLevel->pfd->lpFold->lpMinPoint->dwUserData=(UINT_PTR)lpLevel->pfd->lpFold;
  lpLevel->pfd->lpFold->lpMaxPoint->dwUserData=(UINT_PTR)lpLevel->pfd->lpFold;
}

void CreateAllFolds(STACKLEVEL *hLevelStack, HWND hWnd)
{
  LEVEL *lpLevel=(LEVEL *)hLevelStack->first;
  LEVEL *lpNextLevel;

  while (lpLevel)
  {
    lpNextLevel=lpLevel->next;

    if (!lpLevel->pfd->lpFold)
    {
      //Create fold
      CreateFold(lpLevel, hWnd, FALSE);

      //LEVEL item now is not needed
      StackDeleteLevel(hLevelStack, lpLevel);
    }
    lpLevel=lpNextLevel;
  }
}

LEVEL* StackInsertLevel(STACKLEVEL *hLevelStack, AECHARINDEX *ciChar)
{
  LEVEL *lpElement1=(LEVEL *)hLevelStack->last;
  LEVEL *lpElement2=NULL;

  while (lpElement1)
  {
    if (lpElement1->pfd->lpFold)
    {
      if (AEC_IndexCompare(ciChar, &lpElement1->pfd->lpFold->lpMinPoint->ciPoint) >= 0)
        break;
    }
    else
    {
      if (AEC_IndexCompare(ciChar, &lpElement1->pointMin.ciPoint) >= 0)
        break;
    }
    lpElement1=lpElement1->prev;
  }
  StackInsertAfter((stack **)&hLevelStack->first, (stack **)&hLevelStack->last, (stack *)lpElement1, (stack **)&lpElement2, sizeof(LEVEL));

  if (lpElement2)
  {
    lpElement2->pfd=(FOLDDATA *)GlobalAlloc(GPTR, sizeof(FOLDDATA));
    lpElement2->pointMin.nPointOffset=AEPTO_IGNORE;
    lpElement2->pointMax.nPointOffset=AEPTO_IGNORE;
  }
  return lpElement2;
}

void StackDeleteLevel(STACKLEVEL *hLevelStack, LEVEL *lpLevel)
{
  StackDelete((stack **)&hLevelStack->first, (stack **)&hLevelStack->last, (stack *)lpLevel);
}

void StackFreeLevels(STACKLEVEL *hLevelStack)
{
  StackClear((stack **)&hLevelStack->first, (stack **)&hLevelStack->last);
}

AEFOLD* FoldNextDepth(AEFOLD *lpFold, int *nCurDepth, int nFindRootMaxDepth)
{
  if (lpFold)
  {
    if (lpFold->firstChild && *nCurDepth < nFindRootMaxDepth)
    {
      ++*nCurDepth;
      return lpFold->firstChild;
    }

    for (;;)
    {
      if (lpFold->next)
        return lpFold->next;
      if (!(lpFold=lpFold->parent))
        break;
      --*nCurDepth;
    }
  }
  return lpFold;
}

AEFOLD* FoldGet(FOLDWINDOW *lpFoldWindow, DWORD dwFlags, UINT_PTR dwFindIt, AEFOLD **lpPrevSubling)
{
  AEFINDFOLD ff;

  ff.dwFlags=dwFlags;
  ff.dwFindIt=dwFindIt;
  SendMessage(lpFoldWindow->hWndEdit, AEM_FINDFOLD, (WPARAM)&ff, 0);
  if (lpPrevSubling)
    *lpPrevSubling=ff.lpPrevSubling;
  return ff.lpParent;
}

AEFOLD* FoldAtIndex(FOLDWINDOW *lpFoldWindow, AECHARINDEX *ciChar, DWORD dwFoldStop)
{
  AEFINDFOLD ff;
  AECHARINDEX ciEndOfPoint;

  ff.dwFlags=AEFF_FINDINDEX|(dwFoldStop & IFE_FOLDSTART?AEFF_FOLDSTART:0)|(dwFoldStop & IFE_FOLDEND?AEFF_FOLDEND:0)|AEFF_RECURSE;
  ff.dwFindIt=(UINT_PTR)ciChar;
  SendMessage(lpFoldWindow->hWndEdit, AEM_FINDFOLD, (WPARAM)&ff, 0);

  if (ff.lpParent)
  {
    if (dwFoldStop & IFE_FOLDSTART)
    {
      if (!AEC_IndexCompare(&ff.lpParent->lpMinPoint->ciPoint, ciChar))
        return ff.lpParent;
    }
    if (dwFoldStop & IFE_FOLDEND)
    {
      EndOfPoint(lpFoldWindow, ff.lpParent->lpMaxPoint, &ciEndOfPoint);
      if (!AEC_IndexCompare(&ciEndOfPoint, ciChar))
        return ff.lpParent;
    }
  }
  return NULL;
}

AEFOLD* GetCaretFold(FOLDWINDOW *lpFoldWindow, AEFOLD **lpPrevSubling)
{
  AEFOLD *lpFold;
  AECHARINDEX ciCaret;
  int nCaretOffset;

  SendMessage(lpFoldWindow->hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
  nCaretOffset=(int)SendMessage(lpFoldWindow->hWndEdit, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&ciCaret);

  if (lpFold=FoldGet(lpFoldWindow, AEFF_FINDOFFSET|AEFF_FOLDSTART|AEFF_FOLDEND|AEFF_RECURSE, nCaretOffset, lpPrevSubling))
  {
    ////If folds end at the same index, use deepless
    //for (lpDeepless=lpFold; lpDeepless; lpDeepless=lpDeepless->parent)
    //{
    //  if (lpDeepless->lpMaxPoint->nPointOffset + lpDeepless->lpMaxPoint->nPointLen == nCaretOffset)
    //    lpFold=lpDeepless;
    //  else
    //    break;
    //}
  }
  return lpFold;
}

BOOL IsFoldNameFromLeft(FOLDDATA *lpFoldData)
{
  if (lpFoldData && ((lpFoldData->lpFoldInfo->dwFlags & FIF_NAMEFROMLEFT) ||
                  (!(lpFoldData->lpFoldInfo->dwFlags & FIF_NAMEFROMRIGHT) &&
                     lpFoldData->lpFoldInfo->lpFoldStart->nFoldStartPointLen == 1)))
  {
    return TRUE;
  }
  return FALSE;
}

BOOL GetFoldName(FOLDDATA *lpFoldData, const AECHARINDEX *ciInput, AECHARRANGE *crNameRange)
{
  AECHARRANGE cr;
  AELINEDATA *lpCount;
  int nNameLen;
  int nCount;

  if (IsFoldNameFromLeft(lpFoldData))
  {
    for (cr.ciMax=*ciInput; AEC_PrevChar(&cr.ciMax); )
    {
      if (cr.ciMax.nCharInLine < cr.ciMax.lpLine->nLineLen)
      {
        if (cr.ciMax.lpLine->wpLine[cr.ciMax.nCharInLine] != L' ' &&
            cr.ciMax.lpLine->wpLine[cr.ciMax.nCharInLine] != L'\t')
        {
          AEC_NextChar(&cr.ciMax);
          nNameLen=AEC_WrapLineBeginEx(&cr.ciMax, &cr.ciMin);

          while (cr.ciMin.lpLine->wpLine[cr.ciMin.nCharInLine] == L' ' ||
                 cr.ciMin.lpLine->wpLine[cr.ciMin.nCharInLine] == L'\t')
          {
            AEC_NextChar(&cr.ciMin);
            --nNameLen;
          }
          nNameLen=min(nNameLen, MAX_PATH);

          if (lpFoldData)
          {
            if (!lpFoldData->wpName)
            {
              if (lpFoldData->wpName=(wchar_t *)GlobalAlloc(GPTR, (nNameLen + 1) * sizeof(wchar_t)))
              {
                lpCount=cr.ciMin.lpLine;
                nCount=(int)xstrcpynW(lpFoldData->wpName, cr.ciMin.lpLine->wpLine + cr.ciMin.nCharInLine, min(nNameLen, cr.ciMin.lpLine->nLineLen - cr.ciMin.nCharInLine) + 1);

                for (;;)
                {
                  if (lpCount->nLineBreak != AELB_WRAP)
                    break;

                  if (nNameLen > nCount)
                  {
                    lpCount=lpCount->next;
                    nCount+=(int)xstrcpynW(lpFoldData->wpName + nCount, lpCount->wpLine, min(nNameLen - nCount, lpCount->nLineLen) + 1);
                  }
                  else break;
                }
              }
            }
          }
          if (crNameRange)
          {
            crNameRange->ciMin=cr.ciMin;
            crNameRange->ciMax=cr.ciMax;
          }
          return nNameLen;
        }
      }
    }
  }
  else
  {
    nNameLen=min(ciInput->lpLine->nLineLen - ciInput->nCharInLine, MAX_PATH);

    if (lpFoldData)
    {
      if (!lpFoldData->wpName)
      {
        if (lpFoldData->wpName=(wchar_t *)GlobalAlloc(GPTR, (nNameLen + 1) * sizeof(wchar_t)))
          xstrcpynW(lpFoldData->wpName, ciInput->lpLine->wpLine + ciInput->nCharInLine, nNameLen + 1);
      }
    }
    if (crNameRange)
    {
      crNameRange->ciMin=*ciInput;
      crNameRange->ciMax=*ciInput;
      crNameRange->ciMax.nCharInLine+=nNameLen;
    }
    return nNameLen;
  }
  return 0;
}

void FoldInView(FOLDWINDOW *lpFoldWindow, AEFOLD *lpFold, int nMenuAction)
{
  AECHARINDEX ciChar;
  POINT64 ptGlobal;

  if (nMenuAction == IDM_CODEFOLD_GOEND)
    EndOfPoint(lpFoldWindow, lpFold->lpMaxPoint, &ciChar);
  else
    ciChar=lpFold->lpMinPoint->ciPoint;

  if (nMenuAction == IDM_CODEFOLD_INVIEW)
  {
    SendMessage(lpFoldWindow->hWndEdit, AEM_GLOBALPOSFROMCHAR, (WPARAM)&ptGlobal, (LPARAM)&ciChar);
    SendMessage(lpFoldWindow->hWndEdit, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobal);
  }
  else
  {
    AESELECTION aes;

    aes.crSel.ciMin=ciChar;
    aes.crSel.ciMax=ciChar;
    aes.dwFlags=AESELT_LOCKSCROLL;
    aes.dwType=0;
    SendMessage(lpFoldWindow->hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);

    ScrollToPoint(lpFoldWindow->hWndEdit, NULL);
  }
}

BOOL FoldSelect(FOLDWINDOW *lpFoldWindow, AEFOLD *lpFold)
{
  AECHARRANGE crNameRange;

  if (!GetFoldName(FoldData(lpFold), &lpFold->lpMinPoint->ciPoint, &crNameRange))
    crNameRange.ciMin=lpFold->lpMinPoint->ciPoint;
  EndOfPoint(lpFoldWindow, lpFold->lpMaxPoint, &crNameRange.ciMax);

  //Set selection
  {
    AESELECTION aes;

    aes.crSel.ciMin=crNameRange.ciMin;
    aes.crSel.ciMax=crNameRange.ciMax;
    aes.dwFlags=AESELT_LOCKSCROLL;
    aes.dwType=0;
    SendMessage(lpFoldWindow->hWndEdit, AEM_SETSEL, (WPARAM)&crNameRange.ciMin, (LPARAM)&aes);

    ScrollToPoint(lpFoldWindow->hWndEdit, NULL);
  }
  return TRUE;
}

void FoldSwitchCollapse(FOLDWINDOW *lpFoldWindow, AEFOLD *lpFold, DWORD dwFlags)
{
  if (lpFold)
  {
    SendMessage(lpFoldWindow->hWndEdit, AEM_COLLAPSEFOLD, (WPARAM)lpFold, (lpFold->bCollapse?AECF_EXPAND:AECF_COLLAPSE)|dwFlags);
  }
  else
  {
    if (lpFold=(AEFOLD *)lpFoldWindow->pfwd->lpFoldStack->first)
      SendMessage(lpFoldWindow->hWndEdit, AEM_COLLAPSEFOLD, (WPARAM)NULL, (lpFold->bCollapse?AECF_EXPAND:AECF_COLLAPSE)|dwFlags);
  }
}

void DeleteFoldData(FOLDDATA *lpFoldData)
{
  if (lpFoldData->wpName)
    GlobalFree((HGLOBAL)lpFoldData->wpName);
  GlobalFree((HGLOBAL)lpFoldData);
}

void FreeFolds(FOLDWINDOW *lpFoldWindow, BOOL bUpdate)
{
  AEFOLD *lpFold;

  RemoveTagMark(lpFoldWindow);

  for (lpFold=(AEFOLD *)lpFoldWindow->pfwd->lpFoldStack->first; lpFold; lpFold=AEC_NextFold(lpFold, TRUE))
  {
    DeleteFoldData(FoldData(lpFold));
  }

  if (nMDI == WMD_PMDI)
    SendToDoc(lpFoldWindow->hDocEdit, lpFoldWindow->hWndEdit, AEM_DELETEFOLD, (WPARAM)NULL, bUpdate);
  else
    SendMessage(lpFoldWindow->hWndEdit, AEM_DELETEFOLD, (WPARAM)NULL, bUpdate);
}

INT_PTR EndOfPoint(FOLDWINDOW *lpFoldWindow, const AEPOINT *lpPoint, AECHARINDEX *ciChar)
{
  AEINDEXOFFSET io;

  *ciChar=lpPoint->ciPoint;
  io.ciCharIn=ciChar;
  io.ciCharOut=ciChar;
  io.nOffset=lpPoint->nPointLen;
  io.nNewLine=AELB_R;
  return SendMessage(lpFoldWindow->hWndEdit, AEM_INDEXOFFSET, 0, (LPARAM)&io);
}

void ClearTreeView(HWND hWndTreeView, BOOL bRedraw)
{
  if (SendMessage(hWndTreeView, TVM_GETCOUNT, 0, 0))
  {
    SendMessage(hWndTreeView, WM_SETREDRAW, FALSE, 0);
    SendMessage(hWndTreeView, TVM_DELETEITEM, 0, (LPARAM)NULL);
    SendMessage(hWndTreeView, WM_SETREDRAW, TRUE, 0);
    SetScrollRange(hWndTreeView, SB_HORZ, 0, 0, TRUE);
    SetScrollRange(hWndTreeView, SB_VERT, 0, 0, TRUE);
    if (bRedraw) InvalidateRect(hWndTreeView, NULL, TRUE);
  }
}

void FillTreeView(HWND hWndTreeView, FOLDWINDOW *lpFoldWindow, const wchar_t *wpFilter)
{
  AEFOLD *lpFold=(AEFOLD *)lpFoldWindow->pfwd->lpFoldStack->first;
  TVINSERTSTRUCTW tvis;

  SendMessage(hWndTreeView, WM_SETREDRAW, FALSE, 0);
  SendMessage(hWndTreeView, TVM_DELETEITEM, 0, (LPARAM)NULL);

  if (wpFilter && *wpFilter)
  {
    while (lpFold)
    {
      if (!(FoldData(lpFold)->lpFoldInfo->dwFlags & FIF_NOLISTFOLD) &&
            FoldData(lpFold)->wpName && xstrstrW(FoldData(lpFold)->wpName, -1, wpFilter, -1, FALSE, NULL, NULL))
      {
        tvis.item.mask=TVIF_TEXT|TVIF_PARAM;
        tvis.item.pszText=FoldData(lpFold)->wpName;
        tvis.item.cchTextMax=MAX_PATH;
        tvis.item.lParam=(LPARAM)lpFold;
        tvis.hInsertAfter=TVI_LAST;
        tvis.hParent=NULL;
        FoldData(lpFold)->hItem=(HTREEITEM)TreeView_InsertItemWide(hWndTreeView, &tvis);
      }
      lpFold=AEC_NextFold(lpFold, TRUE);
    }
  }
  else
  {
    while (lpFold)
    {
      if (!(FoldData(lpFold)->lpFoldInfo->dwFlags & FIF_NOLISTFOLD))
      {
        tvis.item.mask=TVIF_TEXT|TVIF_PARAM;
        tvis.item.pszText=FoldData(lpFold)->wpName;
        tvis.item.cchTextMax=MAX_PATH;
        tvis.item.lParam=(LPARAM)lpFold;
        tvis.hInsertAfter=TVI_LAST;
        tvis.hParent=lpFold->parent?FoldData(lpFold->parent)->hItem:NULL;
        FoldData(lpFold)->hItem=(HTREEITEM)TreeView_InsertItemWide(hWndTreeView, &tvis);
      }
      lpFold=AEC_NextFold(lpFold, TRUE);
    }
  }
  SendMessage(hWndTreeView, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWndTreeView, NULL, TRUE);
}

void ExpandTreeView(HWND hWndTreeView, FOLDWINDOW *lpFoldWindow, BOOL bExpand)
{
  AEFOLD *lpFold=(AEFOLD *)lpFoldWindow->pfwd->lpFoldStack->first;

  SendMessage(hWndTreeView, WM_SETREDRAW, FALSE, 0);

  while (lpFold)
  {
    if (lpFold->firstChild)
    {
      if (bExpand)
        SendMessage(hWndTreeView, TVM_EXPAND, TVE_EXPAND, (LPARAM)FoldData(lpFold)->hItem);
      else
        SendMessage(hWndTreeView, TVM_EXPAND, TVE_COLLAPSE, (LPARAM)FoldData(lpFold)->hItem);
    }
    lpFold=AEC_NextFold(lpFold, TRUE);
  }
  SendMessage(hWndTreeView, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWndTreeView, NULL, TRUE);
}

AEFOLD* GetCurFoldTreeView(HWND hWndTreeView)
{
  TVITEMW tvi;

  tvi.mask=TVIF_PARAM;
  tvi.hItem=(HTREEITEM)SendMessage(hWndTreeView, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)NULL);

  if (TreeView_GetItemWide(hWndTreeView, &tvi))
    return (AEFOLD *)tvi.lParam;
  return NULL;
}

BOOL SetCaretTreeView(HWND hWndTreeView, FOLDWINDOW *lpFoldWindow)
{
  if (nFollowCaret)
  {
    if (lpFoldWindow->pfwd->lpFoldStack->first)
    {
      AECHARRANGE crNameRange;
      AECHARINDEX ciCaret;
      AEFOLD *lpCaretFold;
      AEFOLD *lpFold;
      AEFOLD *lpPrevFold;
      AEFOLD *lpEnsureVisible=NULL;

      //Get caret fold
      SendMessage(lpFoldWindow->hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
      lpCaretFold=FoldGet(lpFoldWindow, AEFF_FINDINDEX|AEFF_FOLDSTART|AEFF_FOLDEND|AEFF_RECURSE, (UINT_PTR)&ciCaret, &lpPrevFold);

      //Is caret in the fold name?
      lpFold=lpCaretFold;
      if (lpPrevFold)
        lpFold=AEC_NextFold(lpPrevFold, FALSE);
      else if (lpFold)
        lpFold=AEC_NextFold(lpFold, TRUE);
      if (!lpFold)
        lpFold=(AEFOLD *)lpFoldWindow->pfwd->lpFoldStack->first;
      if (GetFoldName(FoldData(lpFold), &lpFold->lpMinPoint->ciPoint, &crNameRange))
      {
        if (AEC_IndexCompare(&crNameRange.ciMin, &ciCaret) <= 0 &&
            AEC_IndexCompare(&lpFold->lpMaxPoint->ciPoint, &ciCaret) >= 0)
        {
          //Caret in fold or in fold name
        }
        else lpFold=lpCaretFold;
      }
      else lpFold=lpCaretFold;

      if (lpFold)
      {
        if (nFollowCaret == FCO_ONLYROOT)
        {
          while (lpFold->parent)
            lpFold=lpFold->parent;
        }
        if (FoldData(lpFold)->hItem != (HTREEITEM)SendMessage(hWndTreeView, TVM_GETNEXTITEM, TVGN_CARET, 0))
        {
          if (lpFold->parent)
          {
            if (lpFold->parent->parent)
              lpEnsureVisible=lpFold->parent->parent;
            else
              lpEnsureVisible=lpFold->parent;
          }
          else lpEnsureVisible=lpFold;

          if (lpEnsureVisible)
          {
            if (!lpEnsureVisible->parent)
              SendMessage(hWndTreeView, WM_HSCROLL, SB_LEFT, 0);
            else
              SendMessage(hWndTreeView, TVM_ENSUREVISIBLE, 0, (LPARAM)FoldData(lpEnsureVisible)->hItem);
          }
          SendMessage(hWndTreeView, TVM_SELECTITEM, TVGN_CARET, (LPARAM)FoldData(lpFold)->hItem);
        }
        return TRUE;
      }
      else SendMessage(hWndTreeView, TVM_SELECTITEM, TVGN_CARET, (LPARAM)NULL);
    }
  }
  return FALSE;
}

FOLDWINDOW* SetActiveEdit(HWND hWndEdit, HWND hWndTreeView, DWORD dwFlags)
{
  FOLDWINDOW *lpFoldWindow=NULL;
  HWND hWndFocus;

  if (!(dwFlags & SAE_NOFOLD))
  {
    if (hWndEdit)
    {
      if (lpFoldWindow=StackGetFoldWindow(&hFoldWindowsStack, hWndEdit, NULL))
      {
        if (dwFlags & SAE_RESETFOLD)
        {
          FreeFolds(lpFoldWindow, FALSE);
          FillLevelsStack(lpFoldWindow, &lpFoldWindow->pfwd->hLevelStack, hWndEdit, NULL);
        }
      }
      else lpFoldWindow=FillLevelsStack(NULL, NULL, hWndEdit, NULL);
    }
  }

  if (!(dwFlags & SAE_NOLIST))
  {
    if ((nShowDock == CFSD_ALWAYS) ||
        (nShowDock == CFSD_AUTO && lpFoldWindow &&
                                        lpFoldWindow->pfwd->lpSyntaxFile &&
                                        lpFoldWindow->pfwd->lpSyntaxFile->hFoldStack.bVisible &&
                                        ((dwFlags & SAE_IGNOREMAXFOLDS) || SendMessage(lpFoldWindow->hWndEdit, AEM_GETFOLDCOUNT, 0, 0) < nFoldLimit)))
    {
      if (hWndTreeView)
      {
        DWORD dwText;
        DWORD dwBk;

        if (bFoldListSystemColors)
        {
          dwText=(DWORD)-1;
          dwBk=(DWORD)-1;
        }
        else
        {
          if (lpFoldWindow && lpFoldWindow->pfwd->lpSyntaxFile)
          {
            dwText=lpFoldWindow->pfwd->lpSyntaxFile->dwFoldListTextColor;
            dwBk=lpFoldWindow->pfwd->lpSyntaxFile->dwFoldListBkColor;
          }
          else
          {
            dwText=dwFoldListTextColor;
            dwBk=dwFoldListBkColor;
          }
        }
        if ((DWORD)SendMessage(hWndTreeView, TVM_GETTEXTCOLOR, 0, 0) != dwText)
          SendMessage(hWndTreeView, TVM_SETTEXTCOLOR, 0, (LPARAM)dwText);
        if ((DWORD)SendMessage(hWndTreeView, TVM_GETBKCOLOR, 0, 0) != dwBk)
          SendMessage(hWndTreeView, TVM_SETBKCOLOR, 0, (LPARAM)dwBk);

        if (!bFoldListSystemFont)
        {
          HFONT hFontEdit;

          hFontEdit=(HFONT)SendMessage(hMainWnd, AKD_GETFONT, (WPARAM)NULL, (LPARAM)NULL);
          SendMessage(hWndTreeView, WM_SETFONT, (WPARAM)hFontEdit, FALSE);
        }
      }

      if (dkCodeFoldDlg && (dkCodeFoldDlg->dwFlags & DKF_HIDDEN))
        SendMessage(hMainWnd, AKD_DOCK, DK_SHOW, (LPARAM)dkCodeFoldDlg);

      if (dwFlags & SAE_RESETLIST)
      {
        if (hWndTreeView)
        {
          if (lpFoldWindow && lpFoldWindow->pfwd->lpSyntaxFile)
          {
            bIgnoreFilter=TRUE;
            SetWindowTextWide(hWndCodeFoldFilter, lpFoldWindow->pfwd->wszFilter);
            FillTreeView(hWndTreeView, lpFoldWindow, lpFoldWindow->pfwd->wszFilter);
            SetCaretTreeView(hWndTreeView, lpFoldWindow);
            bIgnoreFilter=FALSE;
          }
          else ClearTreeView(hWndTreeView, TRUE);
        }
      }
    }
    else
    {
      if (dkCodeFoldDlg && !(dkCodeFoldDlg->dwFlags & DKF_HIDDEN))
      {
        if ((hWndFocus=GetFocus()) && IsChild(dkCodeFoldDlg->hWnd, hWndFocus))
          SetFocus(hMainWnd);
        SendMessage(hMainWnd, AKD_DOCK, DK_HIDE, (LPARAM)dkCodeFoldDlg);
      }
    }
  }

  if (lpFoldWindow && lpLastPostUpdateFrame && lpFoldWindow->hDocEdit == lpLastPostUpdateFrame->ei.hDocEdit)
  {
    dwLastPostUpdateFlags=dwLastPostUpdateFlags & ~dwFlags;
    if (!dwLastPostUpdateFlags)
    {
      lpLastPostUpdateFrame=NULL;
      dwLastPostUpdateFlags=0;
    }
  }
  return lpFoldWindow;
}

void UpdateTagMark(FOLDWINDOW *lpFoldWindow)
{
  AEFINDTEXTW ftXmlHeadClose;
  AECHARINDEX ciCaret;
  CHARRANGE64 crXmlHeadClose={0};
  SYNTAXFILE *lpSyntaxFile=lpFoldWindow->pfwd->lpSyntaxFile;
  AEFOLD *lpFold;
  FOLDINFO *lpFoldInfo;
  int nCaretOffset;
  int nTagEndLen;
  BOOL bXmlHeadClose=FALSE;
  BOOL bMark=FALSE;
  BOOL bRedraw=FALSE;

  if (bTagMarkEnable && lpSyntaxFile &&
      (lpSyntaxFile->dwTagMarkFontStyle != AEHLS_NONE ||
       lpSyntaxFile->dwTagMarkTextColor != (DWORD)-1 ||
       lpSyntaxFile->dwTagMarkBkColor != (DWORD)-1))
  {
    SendMessage(lpFoldWindow->hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
    nCaretOffset=(int)SendMessage(lpFoldWindow->hWndEdit, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&ciCaret);

    if (lpFold=FoldGet(lpFoldWindow, AEFF_FINDOFFSET|AEFF_FOLDSTART|AEFF_FOLDEND|AEFF_RECURSE, nCaretOffset, NULL))
    {
      lpFoldInfo=FoldData(lpFold)->lpFoldInfo;
      if (lpFoldInfo->dwFlags & FIF_FOLDEND_NOCATCH)
      {
        if (lpFold->next || lpFold->parent)
          nTagEndLen=lpFoldInfo->nFoldEndPointLen;
        else
          lpFoldInfo=NULL;
      }
      else
      {
        if (lpFold->lpMaxPoint->nPointLen)
          nTagEndLen=lpFold->lpMaxPoint->nPointLen;
        else
          lpFoldInfo=NULL;
      }

      if (lpFoldInfo)
      {
        if (((lpFold->lpMinPoint->nPointLen == 1 && !(lpFoldInfo->dwFlags & FIF_XMLTAG)) ?
               (nCaretOffset == lpFold->lpMinPoint->nPointOffset || nCaretOffset == lpFold->lpMinPoint->nPointOffset + lpFold->lpMinPoint->nPointLen) :
               (nCaretOffset > lpFold->lpMinPoint->nPointOffset && nCaretOffset < lpFold->lpMinPoint->nPointOffset + lpFold->lpMinPoint->nPointLen)) ||
            ((nTagEndLen == 1 && !(lpFoldInfo->dwFlags & FIF_XMLTAG)) ?
               (nCaretOffset == lpFold->lpMaxPoint->nPointOffset || nCaretOffset == lpFold->lpMaxPoint->nPointOffset + nTagEndLen) :
               (nCaretOffset > lpFold->lpMaxPoint->nPointOffset && nCaretOffset < lpFold->lpMaxPoint->nPointOffset + nTagEndLen)))
        {
          bMark=TRUE;
        }
        if (lpFoldInfo->dwFlags & FIF_XMLTAG)
        {
          if ((lpFoldInfo->dwFlags & FIF_XMLNONAME_ONETAG) ||
              (lpFoldInfo->dwFlags & FIF_XMLNAMED_ONETAG))
          {
            if (nCaretOffset > lpFold->lpMinPoint->nPointOffset && nCaretOffset < lpFold->lpMaxPoint->nPointOffset + nTagEndLen)
              bMark=TRUE;
          }
          else
          {
            ftXmlHeadClose.dwFlags=AEFR_DOWN|AEFR_MATCHCASE;
            ftXmlHeadClose.pText=L">";
            ftXmlHeadClose.dwTextLen=1;
            ftXmlHeadClose.nNewLine=AELB_ASIS;
            ftXmlHeadClose.crSearch.ciMin=lpFold->lpMinPoint->ciPoint;
            ftXmlHeadClose.crSearch.ciMax=lpFold->lpMaxPoint->ciPoint;
            if (bXmlHeadClose=(BOOL)SendMessage(hWndEdit, AEM_FINDTEXTW, 0, (LPARAM)&ftXmlHeadClose))
            {
              crXmlHeadClose.cpMin=SendMessage(lpFoldWindow->hWndEdit, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&ftXmlHeadClose.crFound.ciMin);
              crXmlHeadClose.cpMax=SendMessage(lpFoldWindow->hWndEdit, AEM_INDEXTORICHOFFSET, 0, (LPARAM)&ftXmlHeadClose.crFound.ciMax);
              if (nCaretOffset > lpFold->lpMinPoint->nPointOffset && nCaretOffset < crXmlHeadClose.cpMax)
                bMark=TRUE;
            }
          }
        }

        if (bMark)
        {
          if (lpFold == lpFoldWindow->pfwd->lpTagMark &&
                lpFoldWindow->pfwd->hTagMarkFirst &&
                  ((AEMARKRANGEITEM *)lpFoldWindow->pfwd->hTagMarkFirst)->crMarkRange.cpMin == lpFold->lpMinPoint->nPointOffset &&
                  ((AEMARKRANGEITEM *)lpFoldWindow->pfwd->hTagMarkFirst)->crMarkRange.cpMax == lpFold->lpMinPoint->nPointOffset + lpFold->lpMinPoint->nPointLen &&
                !lpFoldWindow->pfwd->hTagMarkSecond == !bXmlHeadClose &&
                  (!lpFoldWindow->pfwd->hTagMarkSecond ||
                    (((AEMARKRANGEITEM *)lpFoldWindow->pfwd->hTagMarkSecond)->crMarkRange.cpMin == crXmlHeadClose.cpMin &&
                     ((AEMARKRANGEITEM *)lpFoldWindow->pfwd->hTagMarkSecond)->crMarkRange.cpMax == crXmlHeadClose.cpMax)) &&
                lpFoldWindow->pfwd->hTagMarkThird &&
                  ((AEMARKRANGEITEM *)lpFoldWindow->pfwd->hTagMarkThird)->crMarkRange.cpMin == lpFold->lpMaxPoint->nPointOffset &&
                  ((AEMARKRANGEITEM *)lpFoldWindow->pfwd->hTagMarkThird)->crMarkRange.cpMax == lpFold->lpMaxPoint->nPointOffset + nTagEndLen)
          {
            //Highlighted fold unchanged
          }
          else
          {
            AEMARKRANGEITEM mri;

            RemoveTagMark(lpFoldWindow);
            lpFoldWindow->pfwd->lpTagMark=lpFold;

            //Common members
            mri.nIndex=-1;
            mri.dwFlags=0;
            mri.dwFontStyle=lpSyntaxFile->dwTagMarkFontStyle;
            mri.crText=lpSyntaxFile->dwTagMarkTextColor;
            mri.crBk=lpSyntaxFile->dwTagMarkBkColor;
            mri.dwFontStyle=mri.dwFontStyle;

            //Highlight beginning
            mri.crMarkRange.cpMin=lpFold->lpMinPoint->nPointOffset;
            mri.crMarkRange.cpMax=lpFold->lpMinPoint->nPointOffset + lpFold->lpMinPoint->nPointLen;
            lpFoldWindow->pfwd->hTagMarkFirst=(AEHMARKRANGE)SendMessage(lpFoldWindow->hWndEdit, AEM_HLADDMARKRANGE, (WPARAM)NULL, (LPARAM)&mri);

            //Highlight XML beginning close ">"
            if (bXmlHeadClose)
            {
              mri.crMarkRange.cpMin=crXmlHeadClose.cpMin;
              mri.crMarkRange.cpMax=crXmlHeadClose.cpMax;
              lpFoldWindow->pfwd->hTagMarkSecond=(AEHMARKRANGE)SendMessage(lpFoldWindow->hWndEdit, AEM_HLADDMARKRANGE, (WPARAM)NULL, (LPARAM)&mri);
            }

            //Highlight ending
            mri.crMarkRange.cpMin=lpFold->lpMaxPoint->nPointOffset;
            mri.crMarkRange.cpMax=lpFold->lpMaxPoint->nPointOffset + nTagEndLen;
            lpFoldWindow->pfwd->hTagMarkThird=(AEHMARKRANGE)SendMessage(lpFoldWindow->hWndEdit, AEM_HLADDMARKRANGE, (WPARAM)NULL, (LPARAM)&mri);

            bRedraw=TRUE;
          }
        }
      }
    }
    if (!bMark)
    {
      if (RemoveTagMark(lpFoldWindow))
        bRedraw=TRUE;
    }
    if (bRedraw)
      UpdateEditAndClones(lpFoldWindow->hWndEdit, UE_DRAWRECT);
  }
}

BOOL RemoveTagMark(FOLDWINDOW *lpFoldWindow)
{
  BOOL bResult=FALSE;

  if (lpFoldWindow->pfwd->hTagMarkFirst)
  {
    if (nMDI == WMD_PMDI)
      SendToDoc(lpFoldWindow->hDocEdit, lpFoldWindow->hWndEdit, AEM_HLDELETEMARKRANGE, (WPARAM)NULL, (LPARAM)lpFoldWindow->pfwd->hTagMarkFirst);
    else
      SendMessage(lpFoldWindow->hWndEdit, AEM_HLDELETEMARKRANGE, (WPARAM)NULL, (LPARAM)lpFoldWindow->pfwd->hTagMarkFirst);
    lpFoldWindow->pfwd->hTagMarkFirst=NULL;
    bResult=TRUE;
  }
  if (lpFoldWindow->pfwd->hTagMarkSecond)
  {
    if (nMDI == WMD_PMDI)
      SendToDoc(lpFoldWindow->hDocEdit, lpFoldWindow->hWndEdit, AEM_HLDELETEMARKRANGE, (WPARAM)NULL, (LPARAM)lpFoldWindow->pfwd->hTagMarkSecond);
    else
      SendMessage(lpFoldWindow->hWndEdit, AEM_HLDELETEMARKRANGE, (WPARAM)NULL, (LPARAM)lpFoldWindow->pfwd->hTagMarkSecond);
    lpFoldWindow->pfwd->hTagMarkSecond=NULL;
    bResult=TRUE;
  }
  if (lpFoldWindow->pfwd->hTagMarkThird)
  {
    if (nMDI == WMD_PMDI)
      SendToDoc(lpFoldWindow->hDocEdit, lpFoldWindow->hWndEdit, AEM_HLDELETEMARKRANGE, (WPARAM)NULL, (LPARAM)lpFoldWindow->pfwd->hTagMarkThird);
    else
      SendMessage(lpFoldWindow->hWndEdit, AEM_HLDELETEMARKRANGE, (WPARAM)NULL, (LPARAM)lpFoldWindow->pfwd->hTagMarkThird);
    lpFoldWindow->pfwd->hTagMarkThird=NULL;
    bResult=TRUE;
  }
  lpFoldWindow->pfwd->lpTagMark=NULL;
  return bResult;
}

DWORD CALLBACK IsMatch(AEFINDTEXTW *ft, const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount;
  DWORD dwCount;

  //Optimization: compare first character here
  if (ft->dwFlags & FIF_MATCHCASE)
  {
    if (ciChar->lpLine->wpLine[ciChar->nCharInLine] != *ft->pText)
      return 0;
  }
  else
  {
    if (WideCharLower(ciChar->lpLine->wpLine[ciChar->nCharInLine]) != WideCharLower(*ft->pText))
      return 0;
  }
  ciCount=*ciChar;
  dwCount=0;
  goto CharMatched;

  for (;;)
  {
    for (; ciCount.nCharInLine < ciCount.lpLine->nLineLen; ++ciCount.nCharInLine)
    {
      if (ft->dwFlags & FIF_MATCHCASE)
      {
        if (ciCount.lpLine->wpLine[ciCount.nCharInLine] != ft->pText[dwCount])
          return 0;
      }
      else
      {
        if (WideCharLower(ciCount.lpLine->wpLine[ciCount.nCharInLine]) != WideCharLower(ft->pText[dwCount]))
          return 0;
      }

      CharMatched:
      if (++dwCount >= ft->dwTextLen)
      {
        ++ciCount.nCharInLine;
        goto Found;
      }
    }
    if (ciCount.lpLine->nLineBreak != AELB_WRAP)
      return 0;

    if (ciCount.lpLine->next)
      AEC_NextLine(&ciCount);
    else
      return 0;

    if (dwCount >= ft->dwTextLen)
      goto Found;
  }

  Found:
  ft->crFound.ciMin=*ciChar;
  ft->crFound.ciMax=ciCount;
  return dwCount;
}

DWORD CALLBACK IsMatchRE(STACKREGROUP *sreg, AECHARRANGE *crFound, const AECHARINDEX *ciChar)
{
  AECHARINDEX ciStr=*ciChar;
  DWORD dwCount=0;

  if (!sreg->first) return 0;

  if (AE_PatExec(sreg, sreg->first, &ciStr, &RegExpGlobal_ciMaxStr))
  {
    crFound->ciMin=sreg->first->ciStrStart;
    crFound->ciMax=sreg->first->ciStrEnd;
    dwCount=(DWORD)sreg->first->nStrLen;
  }
  AE_PatReset(sreg);

  return dwCount;
}

BOOL IsEscaped(const AECHARINDEX *ciChar, wchar_t wchEscape)
{
  AECHARINDEX ciCount=*ciChar;
  int nEscapeCount=0;

  while (AEC_PrevCharInLine(&ciCount))
  {
    if (*(ciCount.lpLine->wpLine + ciCount.nCharInLine) == wchEscape)
      ++nEscapeCount;
    else
      break;
  }
  if (nEscapeCount % 2)
    return TRUE;
  return FALSE;
}

FOLDINFO* IsFold(FOLDWINDOW *lpFoldWindow, LEVEL *lpLevel, AEFINDTEXTW *ft, AECHARINDEX *ciChar, DWORD *dwFoldStop)
{
  FOLDINFOHANDLE *lpFoldInfoHandle;
  FOLDINFO *lpFoldInfo;
  FOLDSTART *lpFoldStart;

  if (*dwFoldStop & IFF_CHECKFIRSTFOLDSTART)
    goto FoldStart;

  //Fold end
  FoldEnd:
  if (lpLevel)
  {
    for (lpFoldInfoHandle=(FOLDINFOHANDLE *)lpLevel->pfd->lpFoldInfo->lpFoldStart->hFoldInfoHandleStack.first; lpFoldInfoHandle; lpFoldInfoHandle=lpFoldInfoHandle->next)
    {
      if (IsFoldEnd(lpFoldInfoHandle->lpFoldInfo, ft, ciChar))
      {
        if (lpLevel->pfd->lpFoldInfo->dwFlags != lpFoldInfoHandle->lpFoldInfo->dwFlags)
        {
          if (!CheckFlags(lpFoldInfoHandle->lpFoldInfo, &lpLevel->crFoundMin, IFE_FOLDSTART))
            continue;
        }
        *dwFoldStop=IFE_FOLDEND;
        return lpFoldInfoHandle->lpFoldInfo;
      }
    }
    if (lpLevel->pfd->lpFoldInfo->dwFlags & FIF_COMMENTFOLD)
      return NULL;
  }
  if (*dwFoldStop & IFF_CHECKFIRSTFOLDSTART)
    return NULL;

  //Fold start
  FoldStart:
  if (lpFoldWindow->pfwd->lpSyntaxFile->hFoldStack.nCommonFirstChar == -1 ||
      (wchar_t)lpFoldWindow->pfwd->lpSyntaxFile->hFoldStack.nCommonFirstChar == ciChar->lpLine->wpLine[ciChar->nCharInLine])
  {
    for (lpFoldStart=(FOLDSTART *)lpFoldWindow->pfwd->lpSyntaxFile->hFoldStartStack.first; lpFoldStart; lpFoldStart=lpFoldStart->next)
    {
      if (lpFoldInfo=IsFoldStart(lpFoldStart, ft, ciChar))
      {
        *dwFoldStop=IFE_FOLDSTART;
        return lpFoldInfo;
      }
    }
  }
  if (*dwFoldStop & IFF_CHECKFIRSTFOLDSTART)
    goto FoldEnd;

  return NULL;
}

FOLDINFO* IsFoldStart(FOLDSTART *lpFoldStart, AEFINDTEXTW *ft, AECHARINDEX *ciChar)
{
  BOOL bMatch;

  if (lpFoldStart->dwFlags & FIF_REGEXPSTART)
    bMatch=IsMatchRE(&lpFoldStart->sregStart, &ft->crFound, ciChar);
  else
  {
    ft->dwFlags=lpFoldStart->dwFlags;
    ft->pText=lpFoldStart->wpFoldStart;
    ft->dwTextLen=lpFoldStart->nFoldStartLen;
    bMatch=IsMatch(ft, ciChar);
  }

  if (bMatch)
  {
    FOLDINFOHANDLE *lpFoldInfoHandle;

    for (lpFoldInfoHandle=(FOLDINFOHANDLE *)lpFoldStart->hFoldInfoHandleStack.first; lpFoldInfoHandle; lpFoldInfoHandle=lpFoldInfoHandle->next)
    {
      if (CheckFlags(lpFoldInfoHandle->lpFoldInfo, &ft->crFound, IFE_FOLDSTART))
        return lpFoldInfoHandle->lpFoldInfo;
    }
  }
  return NULL;
}

FOLDINFO* IsFoldEnd(FOLDINFO *lpFoldInfo, AEFINDTEXTW *ft, AECHARINDEX *ciChar)
{
  BOOL bMatch;

  if (lpFoldInfo->dwFlags & FIF_REGEXPEND)
    bMatch=IsMatchRE(&lpFoldInfo->sregEnd, &ft->crFound, ciChar);
  else
  {
    ft->dwFlags=lpFoldInfo->dwFlags;
    ft->pText=lpFoldInfo->wpFoldEnd;
    ft->dwTextLen=lpFoldInfo->nFoldEndLen;
    bMatch=IsMatch(ft, ciChar);
  }

  if (bMatch)
  {
    if (CheckFlags(lpFoldInfo, &ft->crFound, IFE_FOLDEND))
      return lpFoldInfo;
  }
  return NULL;
}

SKIPINFO* IsSkipStart(SKIPSTART *lpSkipStart, AEFINDTEXTW *ft, AECHARINDEX *ciChar)
{
  BOOL bMatch;

  if (lpSkipStart->dwFlags & FIF_REGEXPSTART)
    bMatch=IsMatchRE(&lpSkipStart->sregStart, &ft->crFound, ciChar);
  else
  {
    ft->dwFlags=lpSkipStart->dwFlags;
    ft->pText=lpSkipStart->wpSkipStart;
    ft->dwTextLen=lpSkipStart->nSkipStartLen;
    bMatch=IsMatch(ft, ciChar);
  }

  if (bMatch)
  {
    SKIPINFOHANDLE *lpSkipInfoHandle;

    for (lpSkipInfoHandle=(SKIPINFOHANDLE *)lpSkipStart->hSkipInfoHandleStack.first; lpSkipInfoHandle; lpSkipInfoHandle=lpSkipInfoHandle->next)
    {
      if (!IsEscaped(ciChar, lpSkipInfoHandle->lpSkipInfo->wchEscape))
        return lpSkipInfoHandle->lpSkipInfo;
    }
  }
  return NULL;
}

SKIPINFO* IsSkipEnd(SKIPINFO *lpSkipInfo, AEFINDTEXTW *ft, AECHARINDEX *ciChar)
{
  BOOL bMatch;

  if (lpSkipInfo->dwFlags & FIF_REGEXPEND)
    bMatch=IsMatchRE(&lpSkipInfo->sregEnd, &ft->crFound, ciChar);
  else
  {
    ft->dwFlags=lpSkipInfo->dwFlags;
    ft->pText=lpSkipInfo->wpSkipEnd;
    ft->dwTextLen=lpSkipInfo->nSkipEndLen;
    bMatch=IsMatch(ft, ciChar);
  }

  if (bMatch)
  {
    if (!IsEscaped(ciChar, lpSkipInfo->wchEscape))
      return lpSkipInfo;
  }
  return NULL;
}

FOLDINFO* FindFold(FOLDWINDOW *lpFoldWindow, const AECHARRANGE *crSearchRange)
{
  AEFOLD *lpFold;
  FOLDINFO *lpFoldInfo;
  SKIPINFO *lpSkipInfo=NULL;
  SKIPSTART *lpSkipStart;
  SKIPINFOHANDLE *lpSkipInfoHandle;
  AEFINDTEXTW ft;
  AECHARINDEX ciCount=crSearchRange->ciMin;
  DWORD dwFoldMatch=0;
  BOOL bMatch;

  while (ciCount.lpLine)
  {
    CheckChar:
    while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
    {
      if (AEC_IndexCompare(&ciCount, &crSearchRange->ciMax) >= 0)
        return NULL;

      if (!lpSkipInfo)
      {
        for (lpFoldInfo=(FOLDINFO *)lpFoldWindow->pfwd->lpSyntaxFile->hFoldStack.first; lpFoldInfo; lpFoldInfo=lpFoldInfo->next)
        {
          //Fold start
          if (lpFoldInfo->lpFoldStart->dwFlags & FIF_REGEXPSTART)
            bMatch=IsMatchRE(&lpFoldInfo->lpFoldStart->sregStart, &ft.crFound, &ciCount);
          else
          {
            ft.dwFlags=lpFoldInfo->dwFlags;
            ft.pText=lpFoldInfo->lpFoldStart->wpFoldStart;
            ft.dwTextLen=lpFoldInfo->lpFoldStart->nFoldStartLen;
            bMatch=IsMatch(&ft, &ciCount);
          }
          if (bMatch)
          {
            if (!CheckFlags(lpFoldInfo, &ft.crFound, IFE_FOLDSTART) != !FoldAtIndex(lpFoldWindow, &ft.crFound.ciMin, IFE_FOLDSTART))
              return lpFoldInfo;
            dwFoldMatch=IFE_FOLDSTART;
          }

          //Fold end
          if (lpFoldInfo->dwFlags & FIF_REGEXPEND)
            bMatch=IsMatchRE(&lpFoldInfo->sregEnd, &ft.crFound, &ciCount);
          else
          {
            ft.dwFlags=lpFoldInfo->dwFlags;
            ft.pText=lpFoldInfo->wpFoldEnd;
            ft.dwTextLen=lpFoldInfo->nFoldEndLen;
            bMatch=IsMatch(&ft, &ciCount);
          }
          if (bMatch)
          {
            if (!CheckFlags(lpFoldInfo, &ft.crFound, IFE_FOLDEND) != !FoldAtIndex(lpFoldWindow, &ft.crFound.ciMax, IFE_FOLDEND))
              return lpFoldInfo;
            dwFoldMatch=IFE_FOLDEND;
          }
        }

        //Skip start
        if (!dwFoldMatch)
        {
          for (lpSkipStart=(SKIPSTART *)lpFoldWindow->pfwd->lpSyntaxFile->hSkipStartStack.first; lpSkipStart; lpSkipStart=lpSkipStart->next)
          {
            if (lpSkipInfo=IsSkipStart(lpSkipStart, &ft, &ciCount))
              goto NextChar;
          }
        }
        else dwFoldMatch=0;
      }
      else
      {
        //Skip end
        for (lpSkipInfoHandle=(SKIPINFOHANDLE *)lpSkipInfo->lpSkipStart->hSkipInfoHandleStack.first; lpSkipInfoHandle; lpSkipInfoHandle=lpSkipInfoHandle->next)
        {
          if (!lpSkipInfoHandle->lpSkipInfo->nSkipEndLen)
            break;
          if (IsSkipEnd(lpSkipInfoHandle->lpSkipInfo, &ft, &ciCount))
          {
            lpSkipInfo=NULL;

            if (lpSkipInfoHandle->lpSkipInfo->dwFlags & FIF_FOLDEND_NOCATCH)
              goto CheckChar;
            break;
          }
        }

        if (lpFold=FoldAtIndex(lpFoldWindow, &ciCount, IFE_FOLDSTART|IFE_FOLDEND))
          return FoldData(lpFold)->lpFoldInfo;
      }

      NextChar:
      AEC_IndexInc(&ciCount);
    }
    //Force one line quote
    if (ciCount.lpLine->nLineBreak != AELB_WRAP)
      lpSkipInfo=NULL;
    AEC_NextLine(&ciCount);
  }
  return NULL;
}

BOOL CheckFlags(FOLDINFO *lpFoldInfo, AECHARRANGE *crFound, DWORD dwFoldStop)
{
  AECHARINDEX ciTmp;
  int nChar;
  BOOL bInList;

  if (dwFoldStop & IFE_FOLDSTART)
  {
    if (lpFoldInfo->dwFlags & FIF_FOLDSTART_ATLINESTART)
    {
      if (!IsSpacesFromLeft(&crFound->ciMin))
        return FALSE;
    }
    if (lpFoldInfo->dwFlags & FIF_FOLDSTART_ATLINEEND)
    {
      if (!IsSpacesFromRight(&crFound->ciMax))
        return FALSE;
    }

    if (*lpFoldInfo->wpDelimiters)
    {
      if (!(lpFoldInfo->dwFlags & FIF_FOLDSTART_NOLEFTDELIM))
      {
        ciTmp=crFound->ciMin;

        if (AEC_PrevChar(&ciTmp))
        {
          if ((nChar=AEC_CharAtIndex(&ciTmp)) >= 0)
          {
            bInList=IsInDelimiterList(lpFoldInfo->wpDelimiters, (wchar_t)nChar);

            if (!bInList == !(lpFoldInfo->dwFlags & FIF_DENYDELIM))
              return FALSE;
          }
        }
      }
      if (!(lpFoldInfo->dwFlags & FIF_FOLDSTART_NORIGHTDELIM))
      {
        if ((nChar=AEC_CharAtIndex(&crFound->ciMax)) >= 0)
        {
          bInList=IsInDelimiterList(lpFoldInfo->wpDelimiters, (wchar_t)nChar);

          if (!bInList == !(lpFoldInfo->dwFlags & FIF_DENYDELIM))
            return FALSE;
        }
      }
    }
  }
  else //(dwFoldStop & IFE_FOLDEND)
  {
    if (lpFoldInfo->dwFlags & FIF_FOLDEND_ATLINESTART)
    {
      if (!IsSpacesFromLeft(&crFound->ciMin))
        return FALSE;
    }
    if (lpFoldInfo->dwFlags & FIF_FOLDEND_ATLINEEND)
    {
      if (!IsSpacesFromRight(&crFound->ciMax))
        return FALSE;
    }

    if (*lpFoldInfo->wpDelimiters)
    {
      if (!(lpFoldInfo->dwFlags & FIF_FOLDEND_NOLEFTDELIM))
      {
        ciTmp=crFound->ciMin;

        if (AEC_PrevChar(&ciTmp))
        {
          if ((nChar=AEC_CharAtIndex(&ciTmp)) >= 0)
          {
            bInList=IsInDelimiterList(lpFoldInfo->wpDelimiters, (wchar_t)nChar);

            if (!bInList == !(lpFoldInfo->dwFlags & FIF_DENYDELIM))
              return FALSE;
          }
        }
      }
      if (!(lpFoldInfo->dwFlags & FIF_FOLDEND_NORIGHTDELIM))
      {
        if ((nChar=AEC_CharAtIndex(&crFound->ciMax)) >= 0)
        {
          bInList=IsInDelimiterList(lpFoldInfo->wpDelimiters, (wchar_t)nChar);

          if (!bInList == !(lpFoldInfo->dwFlags & FIF_DENYDELIM))
            return FALSE;
        }
      }
    }
  }
  return TRUE;
}

BOOL IsSpacesFromLeft(const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount=*ciChar;
  wchar_t wchChar;

  if (AEC_IsFirstCharInLine(&ciCount))
    return TRUE;

  while (AEC_PrevCharInLine(&ciCount))
  {
    wchChar=*(ciCount.lpLine->wpLine + ciCount.nCharInLine);
    if (wchChar != L' ' && wchChar != L'\t')
      return FALSE;
  }
  return TRUE;
}

BOOL IsSpacesFromRight(const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount=*ciChar;
  wchar_t wchChar;

  if (AEC_IsLastCharInLine(&ciCount))
    return TRUE;

  do
  {
    wchChar=*(ciCount.lpLine->wpLine + ciCount.nCharInLine);
    if (wchChar != L' ' && wchChar != L'\t')
      return FALSE;
  }
  while (AEC_NextCharInLine(&ciCount));

  return TRUE;
}

void SetEditRect(AEHDOC hDocEdit, HWND hWndEdit, int nNewWidth, int nOldWidth)
{
  RECT rcErase;
  RECT rcDraw;
  DWORD dwFlags=AERC_NOTOP|AERC_NORIGHT|AERC_NOBOTTOM;

  if (nMDI == WMD_PMDI)
  {
    SendToDoc(hDocEdit, hWndEdit, AEM_GETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendToDoc(hDocEdit, hWndEdit, AEM_GETRECT, dwFlags, (LPARAM)&rcDraw);
  }
  else
  {
    SendMessage(hWndEdit, AEM_GETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendMessage(hWndEdit, AEM_GETRECT, dwFlags, (LPARAM)&rcDraw);
  }

  rcDraw.left+=nNewWidth - nOldWidth;
  rcErase.left+=nNewWidth - nOldWidth;

  if (nMDI == WMD_PMDI)
  {
    SendToDoc(hDocEdit, hWndEdit, AEM_SETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendToDoc(hDocEdit, hWndEdit, AEM_SETRECT, dwFlags|AERC_UPDATE, (LPARAM)&rcDraw);
  }
  else
  {
    SendMessage(hWndEdit, AEM_SETERASERECT, dwFlags, (LPARAM)&rcErase);
    SendMessage(hWndEdit, AEM_SETRECT, dwFlags|AERC_UPDATE, (LPARAM)&rcDraw);
  }
}

DWORD ScrollToPoint(HWND hWnd, POINT *ptPos)
{
  AESCROLLTOPOINT stp;
  DWORD dwScrollResult;

  //Test scroll to caret
  stp.dwFlags=AESC_TEST|(ptPos?AESC_POINTCLIENT:AESC_POINTCARET)|AESC_OFFSETCHARX|AESC_OFFSETCHARY;
  stp.ptPos.x=ptPos?ptPos->x:0;
  stp.ptPos.y=ptPos?ptPos->y:0;
  stp.nOffsetX=1;
  stp.nOffsetY=0;
  dwScrollResult=(DWORD)SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);

  //Scroll to caret
  stp.dwFlags=(ptPos?AESC_POINTCLIENT:AESC_POINTCARET);
  stp.nOffsetX=3;
  stp.nOffsetY=2;
  if (dwScrollResult & AECSE_SCROLLEDX)
    stp.dwFlags|=AESC_OFFSETRECTDIVX;
  if (dwScrollResult & AECSE_SCROLLEDY)
    stp.dwFlags|=AESC_OFFSETRECTDIVY;
  return (DWORD)SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);
}

BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetWindowRect(hWnd, rc))
  {
    rc->right-=rc->left;
    rc->bottom-=rc->top;

    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

LRESULT SendToDoc(AEHDOC hDocEdit, HWND hWndEdit, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AESENDMESSAGE sm;

  sm.hDoc=hDocEdit;
  sm.uMsg=uMsg;
  sm.wParam=wParam;
  sm.lParam=lParam;
  if (SendMessage(hWndEdit, AEM_SENDMESSAGE, 0, (LPARAM)&sm))
    return sm.lResult;
  return 0;
}


//// Options

void ReadCodeFoldOptions(HANDLE hOptions)
{
  WideOption(hOptions, L"CodeFoldDockSide", PO_DWORD, (LPBYTE)&nCodeFoldDockSide, sizeof(DWORD));
  WideOption(hOptions, L"CodeFoldDockRect", PO_BINARY, (LPBYTE)&rcCodeFoldDockRect, sizeof(RECT));
  WideOption(hOptions, L"ShowDock", PO_DWORD, (LPBYTE)&nShowDock, sizeof(DWORD));
  WideOption(hOptions, L"FoldLimit", PO_DWORD, (LPBYTE)&nFoldLimit, sizeof(DWORD));
  WideOption(hOptions, L"CharsLimit", PO_DWORD, (LPBYTE)&nCharsLimit, sizeof(DWORD));
  WideOption(hOptions, L"FollowCaret", PO_DWORD, (LPBYTE)&nFollowCaret, sizeof(DWORD));
  WideOption(hOptions, L"FoldListSystemColors", PO_DWORD, (LPBYTE)&bFoldListSystemColors, sizeof(DWORD));
  WideOption(hOptions, L"FoldListSystemFont", PO_DWORD, (LPBYTE)&bFoldListSystemFont, sizeof(DWORD));
  WideOption(hOptions, L"DrawNodeType", PO_DWORD, (LPBYTE)&nDrawNodeType, sizeof(DWORD));
  WideOption(hOptions, L"TagMarkEnable", PO_DWORD, (LPBYTE)&bTagMarkEnable, sizeof(DWORD));
  WideOption(hOptions, L"CollapseOnOpen", PO_DWORD, (LPBYTE)&bCollapseOnOpen, sizeof(DWORD));
  WideOption(hOptions, L"NoPrintCollapsed", PO_DWORD, (LPBYTE)&bNoPrintCollapsed, sizeof(DWORD));
  WideOption(hOptions, L"FindRootMaxDepth", PO_DWORD, (LPBYTE)&nFindRootMaxDepth, sizeof(DWORD));
  WideOption(hOptions, L"CurrentCollapse", PO_DWORD, (LPBYTE)&dwCurrentCollapse, sizeof(DWORD));
  WideOption(hOptions, L"CurrentGoBegin", PO_DWORD, (LPBYTE)&dwCurrentGoBegin, sizeof(DWORD));
  WideOption(hOptions, L"CurrentGoEnd", PO_DWORD, (LPBYTE)&dwCurrentGoEnd, sizeof(DWORD));
  WideOption(hOptions, L"CurrentSelect", PO_DWORD, (LPBYTE)&dwCurrentSelect, sizeof(DWORD));
  WideOption(hOptions, L"AllCollapse", PO_DWORD, (LPBYTE)&dwAllCollapse, sizeof(DWORD));
  WideOption(hOptions, L"AllNextLevel", PO_DWORD, (LPBYTE)&dwAllNextLevel, sizeof(DWORD));
  WideOption(hOptions, L"AllPrevLevel", PO_DWORD, (LPBYTE)&dwAllPrevLevel, sizeof(DWORD));
  WideOption(hOptions, L"FindRootLevel", PO_DWORD, (LPBYTE)&dwFindRootLevel, sizeof(DWORD));
}

void SaveCodeFoldOptions(HANDLE hOptions, DWORD dwFlags)
{
  if (dwFlags & OF_CODEFOLD_RECT)
  {
    if (dkCodeFoldDlg)
    {
      WideOption(hOptions, L"CodeFoldDockSide", PO_DWORD, (LPBYTE)&dkCodeFoldDlg->nSide, sizeof(DWORD));
      WideOption(hOptions, L"CodeFoldDockRect", PO_BINARY, (LPBYTE)&dkCodeFoldDlg->rcSize, sizeof(RECT));
    }
    else
    {
      WideOption(hOptions, L"CodeFoldDockSide", PO_DWORD, (LPBYTE)&nCodeFoldDockSide, sizeof(DWORD));
      WideOption(hOptions, L"CodeFoldDockRect", PO_BINARY, (LPBYTE)&rcCodeFoldDockRect, sizeof(RECT));
    }
  }
  if (dwFlags & OF_CODEFOLD_SETTINGS)
  {
    WideOption(hOptions, L"ShowDock", PO_DWORD, (LPBYTE)&nShowDock, sizeof(DWORD));
    WideOption(hOptions, L"FoldLimit", PO_DWORD, (LPBYTE)&nFoldLimit, sizeof(DWORD));
    WideOption(hOptions, L"CharsLimit", PO_DWORD, (LPBYTE)&nCharsLimit, sizeof(DWORD));
    WideOption(hOptions, L"FollowCaret", PO_DWORD, (LPBYTE)&nFollowCaret, sizeof(DWORD));
    WideOption(hOptions, L"FoldListSystemColors", PO_DWORD, (LPBYTE)&bFoldListSystemColors, sizeof(DWORD));
    WideOption(hOptions, L"FoldListSystemFont", PO_DWORD, (LPBYTE)&bFoldListSystemFont, sizeof(DWORD));
    WideOption(hOptions, L"DrawNodeType", PO_DWORD, (LPBYTE)&nDrawNodeType, sizeof(DWORD));
    WideOption(hOptions, L"TagMarkEnable", PO_DWORD, (LPBYTE)&bTagMarkEnable, sizeof(DWORD));
    WideOption(hOptions, L"CollapseOnOpen", PO_DWORD, (LPBYTE)&bCollapseOnOpen, sizeof(DWORD));
    WideOption(hOptions, L"NoPrintCollapsed", PO_DWORD, (LPBYTE)&bNoPrintCollapsed, sizeof(DWORD));
    WideOption(hOptions, L"FindRootMaxDepth", PO_DWORD, (LPBYTE)&nFindRootMaxDepth, sizeof(DWORD));
    WideOption(hOptions, L"CurrentCollapse", PO_DWORD, (LPBYTE)&dwCurrentCollapse, sizeof(DWORD));
    WideOption(hOptions, L"CurrentGoBegin", PO_DWORD, (LPBYTE)&dwCurrentGoBegin, sizeof(DWORD));
    WideOption(hOptions, L"CurrentGoEnd", PO_DWORD, (LPBYTE)&dwCurrentGoEnd, sizeof(DWORD));
    WideOption(hOptions, L"CurrentSelect", PO_DWORD, (LPBYTE)&dwCurrentSelect, sizeof(DWORD));
    WideOption(hOptions, L"AllCollapse", PO_DWORD, (LPBYTE)&dwAllCollapse, sizeof(DWORD));
    WideOption(hOptions, L"AllNextLevel", PO_DWORD, (LPBYTE)&dwAllNextLevel, sizeof(DWORD));
    WideOption(hOptions, L"AllPrevLevel", PO_DWORD, (LPBYTE)&dwAllPrevLevel, sizeof(DWORD));
    WideOption(hOptions, L"FindRootLevel", PO_DWORD, (LPBYTE)&dwFindRootLevel, sizeof(DWORD));
  }
}

void InitCodeFold()
{
  hCursorArrow=LoadCursor(NULL, IDC_ARROW);

  if (!nInitCodeFold++)
  {
    //Add hotkeys
    {
      PLUGINADDW pa={0};

      if (dwCurrentCollapse && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCurrentCollapse))
      {
        pa.pFunction=L"Coder::CodeFold::CurrentCollapse";
        pa.wHotkey=(WORD)dwCurrentCollapse;
        pa.PluginProc=CurrentCollapseProc;
        pfwCurrentCollapse=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwCurrentGoBegin && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCurrentGoBegin))
      {
        pa.pFunction=L"Coder::CodeFold::CurrentGoBegin";
        pa.wHotkey=(WORD)dwCurrentGoBegin;
        pa.PluginProc=CurrentGoBeginProc;
        pfwCurrentGoBegin=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwCurrentGoEnd && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCurrentGoEnd))
      {
        pa.pFunction=L"Coder::CodeFold::CurrentGoEnd";
        pa.wHotkey=(WORD)dwCurrentGoEnd;
        pa.PluginProc=CurrentGoEndProc;
        pfwCurrentGoEnd=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwCurrentSelect && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCurrentSelect))
      {
        pa.pFunction=L"Coder::CodeFold::CurrentSelect";
        pa.wHotkey=(WORD)dwCurrentSelect;
        pa.PluginProc=CurrentSelectProc;
        pfwCurrentSelect=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwAllCollapse && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwAllCollapse))
      {
        pa.pFunction=L"Coder::CodeFold::AllCollapse";
        pa.wHotkey=(WORD)dwAllCollapse;
        pa.PluginProc=AllCollapseProc;
        pfwAllCollapse=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwAllNextLevel && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwAllNextLevel))
      {
        pa.pFunction=L"Coder::CodeFold::AllNextLevel";
        pa.wHotkey=(WORD)dwAllNextLevel;
        pa.PluginProc=AllNextLevelProc;
        pfwAllNextLevel=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwAllPrevLevel && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwAllPrevLevel))
      {
        pa.pFunction=L"Coder::CodeFold::AllPrevLevel";
        pa.wHotkey=(WORD)dwAllPrevLevel;
        pa.PluginProc=AllPrevLevelProc;
        pfwAllPrevLevel=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwFindRootLevel && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwFindRootLevel))
      {
        pa.pFunction=L"Coder::CodeFold::FindRootLevel";
        pa.wHotkey=(WORD)dwFindRootLevel;
        pa.PluginProc=FindRootLevelProc;
        pfwFindRootLevel=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
    }
  }
}

void UninitCodeFold(DWORD dwFlags)
{
  if (!--nInitCodeFold)
  {
    if (dwFlags & INIT_CODEFOLD_HOTKEYS)
    {
      //Remove hotkeys
      if (pfwCurrentCollapse)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCurrentCollapse);
        pfwCurrentCollapse=NULL;
      }
      if (pfwCurrentGoBegin)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCurrentGoBegin);
        pfwCurrentGoBegin=NULL;
      }
      if (pfwCurrentGoEnd)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCurrentGoEnd);
        pfwCurrentGoEnd=NULL;
      }
      if (pfwCurrentSelect)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCurrentSelect);
        pfwCurrentSelect=NULL;
      }
      if (pfwAllCollapse)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwAllCollapse);
        pfwAllCollapse=NULL;
      }
      if (pfwAllNextLevel)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwAllNextLevel);
        pfwAllNextLevel=NULL;
      }
      if (pfwAllPrevLevel)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwAllPrevLevel);
        pfwAllPrevLevel=NULL;
      }
      if (pfwFindRootLevel)
      {
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwFindRootLevel);
        pfwFindRootLevel=NULL;
      }
    }
    if (dwFlags & INIT_CODEFOLD_WINDOWS)
    {
      StackFreeFoldWindows(&hFoldWindowsStack);
    }
  }
}
