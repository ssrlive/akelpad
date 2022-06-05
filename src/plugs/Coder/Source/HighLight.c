#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
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
STACKHIGHLIGHTWINDOW hHighLightWindowsStack={0};
DWORD dwIgnoreFontStyle=0;
BOOL bAutoMarkEnable=TRUE;
int nAutoMarkType=MARKAUTO_WORDS;
int nAutoMarkMaxSel=MARKMAX_SELECTION;
DWORD dwAutoMarkFlags;
DWORD dwAutoMarkFontStyle;
DWORD dwAutoMarkTextColor;
DWORD dwAutoMarkBkColor;
BOOL bFindingMark=FALSE;
wchar_t wszOpenFile[MAX_PATH];
BOOL bUpdateTheme=TRUE;
AEHDOC hDocEditCurrent=NULL;
RECT rcUpdateRect;


//Plugin extern function
void __declspec(dllexport) HighLight(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon)
    InitCommon(pd);
  else if (!bInitHighLight)
    ReadOptions(OF_HIGHLIGHT);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (bInitHighLight)
    {
      if (nAction == DLLA_HIGHLIGHT_MARK)
      {
        EDITINFO ei;
        HIGHLIGHTWINDOW *lpHighlightWindow;
        unsigned char *pColorText=NULL;
        unsigned char *pColorBk=NULL;
        DWORD dwColorText=(DWORD)-1;
        DWORD dwColorBk=(DWORD)-1;
        DWORD dwFlags=MARKFLAG_MATCHCASE;
        DWORD dwFontStyle=0;
        DWORD dwMarkID=MARKID_AUTOASSIGN;
        wchar_t *wpMarkText=NULL;
        INT_PTR nMarkTextLen=-1;

        if (IsExtCallParamValid(pd->lParam, 2))
          pColorText=(unsigned char *)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pColorBk=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          dwFlags=(DWORD)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          dwFontStyle=(DWORD)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          dwMarkID=(DWORD)GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          wpMarkText=(wchar_t *)GetExtCallParam(pd->lParam, 7);
        if (IsExtCallParamValid(pd->lParam, 8))
          nMarkTextLen=(INT_PTR)GetExtCallParam(pd->lParam, 8);

        if (pd->dwSupport & PDS_STRANSI)
        {
          if (pColorText && *(char *)pColorText == '#')
            dwColorText=GetColorFromStrAnsi((char *)pColorText + 1, NULL);
          if (pColorBk && *(char *)pColorBk == '#')
            dwColorBk=GetColorFromStrAnsi((char *)pColorBk + 1, NULL);
        }
        else
        {
          if (pColorText && *(wchar_t *)pColorText == L'#')
            dwColorText=GetColorFromStr((wchar_t *)pColorText + 1, NULL);
          if (pColorBk && *(wchar_t *)pColorBk == L'#')
            dwColorBk=GetColorFromStr((wchar_t *)pColorBk + 1, NULL);
        }

        if (dwColorText != (DWORD)-1 || dwColorBk != (DWORD)-1)
        {
          if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)pd->hWndEdit, (LPARAM)&ei))
          {
            if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, ei.hWndMaster?ei.hWndMaster:ei.hWndEdit, ei.hDocMaster?ei.hDocMaster:ei.hDocEdit))
            {
              CHARRANGE64 cr;
              BOOL bUpdate=FALSE;
              DWORD dwHLFlags=0;

              if (dwFlags & MARKFLAG_MATCHCASE)
                dwHLFlags|=AEHLF_MATCHCASE;
              if (dwFlags & MARKFLAG_REGEXP)
                dwHLFlags|=AEHLF_REGEXP;
              if (dwFlags & MARKFLAG_WHOLEWORD)
                dwHLFlags|=AEHLF_WHOLEWORD;

              if (dwMarkID != MARKID_AUTOASSIGN || !SendMessage(ei.hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)NULL))
              {
                if (UnmarkSelection(lpHighlightWindow, dwMarkID, dwColorText, dwColorBk))
                  bUpdate=TRUE;
              }

              if (wpMarkText)
              {
                if (MarkSelection(lpHighlightWindow, wpMarkText, (int)nMarkTextLen, dwColorText, dwColorBk, dwHLFlags, dwFontStyle, dwMarkID))
                  bUpdate=TRUE;
              }
              else
              {
                SendMessage(ei.hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);

                if (cr.cpMax > cr.cpMin && cr.cpMax - cr.cpMin < nAutoMarkMaxSel)
                {
                  if (wpMarkText=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)ei.hWndEdit, (LPARAM)&nMarkTextLen))
                  {
                    if (MarkSelection(lpHighlightWindow, wpMarkText, (int)nMarkTextLen, dwColorText, dwColorBk, dwHLFlags, dwFontStyle, dwMarkID))
                      bUpdate=TRUE;
                    SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpMarkText);
                  }
                }
              }

              //Update edit
              if (bUpdate)
                UpdateEditAll(UE_DRAWRECT);
            }
          }
        }
      }
      else if (nAction == DLLA_HIGHLIGHT_UNMARK)
      {
        EDITINFO ei;
        HIGHLIGHTWINDOW *lpHighlightWindow;
        DWORD dwMarkID=0;
        unsigned char *pColorText=NULL;
        unsigned char *pColorBk=NULL;
        DWORD dwColorText=(DWORD)-1;
        DWORD dwColorBk=(DWORD)-1;

        if (IsExtCallParamValid(pd->lParam, 2))
          dwMarkID=(DWORD)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pColorText=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          pColorBk=(unsigned char *)GetExtCallParam(pd->lParam, 4);

        if (pd->dwSupport & PDS_STRANSI)
        {
          if (pColorText && *(char *)pColorText == '#')
            dwColorText=GetColorFromStrAnsi((char *)pColorText + 1, NULL);
          if (pColorBk && *(char *)pColorBk == '#')
            dwColorBk=GetColorFromStrAnsi((char *)pColorBk + 1, NULL);
        }
        else
        {
          if (pColorText && *(wchar_t *)pColorText == L'#')
            dwColorText=GetColorFromStr((wchar_t *)pColorText + 1, NULL);
          if (pColorBk && *(wchar_t *)pColorBk == L'#')
            dwColorBk=GetColorFromStr((wchar_t *)pColorBk + 1, NULL);
        }

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)pd->hWndEdit, (LPARAM)&ei))
        {
          if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, ei.hWndMaster?ei.hWndMaster:ei.hWndEdit, ei.hDocMaster?ei.hDocMaster:ei.hDocEdit))
          {
            if (dwMarkID == MARKID_SELECTION)
            {
              MARKTEXT *lpMarkText;
              AEMARKTEXTITEMW *lpMarkItem;
              CHARRANGE64 cr;
              wchar_t *wpMarkText;
              INT_PTR nMarkTextLen=0;

              SendMessage(ei.hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);

              if (cr.cpMax > cr.cpMin && cr.cpMax - cr.cpMin < nAutoMarkMaxSel)
              {
                if (wpMarkText=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)ei.hWndEdit, (LPARAM)&nMarkTextLen))
                {
                  if (lpMarkText=StackGetMarkByText(lpHighlightWindow, wpMarkText, (int)nMarkTextLen))
                  {
                    lpMarkItem=(AEMARKTEXTITEMW *)lpMarkText->hMarkTextHandle;
                    if (UnmarkSelection(lpHighlightWindow, lpMarkText->dwMarkID, lpMarkItem->crText, lpMarkItem->crBk))
                      UpdateEditAll(UE_DRAWRECT);
                  }
                  SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpMarkText);
                }
              }
            }
            else
            {
              if (UnmarkSelection(lpHighlightWindow, dwMarkID, dwColorText, dwColorBk))
                UpdateEditAll(UE_DRAWRECT);
            }
          }
        }
      }
      else if (nAction == DLLA_HIGHLIGHT_FINDMARK)
      {
        EDITINFO ei;
        HIGHLIGHTWINDOW *lpHighlightWindow;
        DWORD dwMarkID=0;
        unsigned char *pColorText=NULL;
        unsigned char *pColorBk=NULL;
        DWORD dwColorText=(DWORD)-1;
        DWORD dwColorBk=(DWORD)-1;
        BOOL bFindUp=FALSE;

        if (IsExtCallParamValid(pd->lParam, 2))
          dwMarkID=(DWORD)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pColorText=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          pColorBk=(unsigned char *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          bFindUp=(DWORD)GetExtCallParam(pd->lParam, 5);

        if (pd->dwSupport & PDS_STRANSI)
        {
          if (pColorText && *(char *)pColorText == '#')
            dwColorText=GetColorFromStrAnsi((char *)pColorText + 1, NULL);
          if (pColorBk && *(char *)pColorBk == '#')
            dwColorBk=GetColorFromStrAnsi((char *)pColorBk + 1, NULL);
        }
        else
        {
          if (pColorText && *(wchar_t *)pColorText == L'#')
            dwColorText=GetColorFromStr((wchar_t *)pColorText + 1, NULL);
          if (pColorBk && *(wchar_t *)pColorBk == L'#')
            dwColorBk=GetColorFromStr((wchar_t *)pColorBk + 1, NULL);
        }

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)pd->hWndEdit, (LPARAM)&ei))
        {
          if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, ei.hWndMaster?ei.hWndMaster:ei.hWndEdit, ei.hDocMaster?ei.hDocMaster:ei.hDocEdit))
          {
            FindMark(lpHighlightWindow, dwMarkID, dwColorText, dwColorBk, bFindUp);
          }
        }
      }
      else if (nAction == DLLA_HIGHLIGHT_CHECKMARK)
      {
        EDITINFO ei;
        HIGHLIGHTWINDOW *lpHighlightWindow;
        DWORD dwMarkID=0;
        unsigned char *pColorText=NULL;
        unsigned char *pColorBk=NULL;
        DWORD dwColorText=(DWORD)-1;
        DWORD dwColorBk=(DWORD)-1;
        BOOL *lpbActive=NULL;

        if (IsExtCallParamValid(pd->lParam, 2))
          dwMarkID=(DWORD)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pColorText=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          pColorBk=(unsigned char *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          lpbActive=(BOOL *)GetExtCallParam(pd->lParam, 5);

        if (pd->dwSupport & PDS_STRANSI)
        {
          if (pColorText && *(char *)pColorText == '#')
            dwColorText=GetColorFromStrAnsi((char *)pColorText + 1, NULL);
          if (pColorBk && *(char *)pColorBk == '#')
            dwColorBk=GetColorFromStrAnsi((char *)pColorBk + 1, NULL);
        }
        else
        {
          if (pColorText && *(wchar_t *)pColorText == L'#')
            dwColorText=GetColorFromStr((wchar_t *)pColorText + 1, NULL);
          if (pColorBk && *(wchar_t *)pColorBk == L'#')
            dwColorBk=GetColorFromStr((wchar_t *)pColorBk + 1, NULL);
        }

        if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)pd->hWndEdit, (LPARAM)&ei))
        {
          if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, ei.hWndMaster?ei.hWndMaster:ei.hWndEdit, ei.hDocMaster?ei.hDocMaster:ei.hDocEdit))
          {
            if (StackGetMarkByColorID(lpHighlightWindow, dwMarkID, dwColorText, dwColorBk))
              *lpbActive=TRUE;
            else
              *lpbActive=FALSE;
          }
        }
      }
      else if (nAction == DLLA_HIGHLIGHT_GETMARKSTACK)
      {
        HIGHLIGHTWINDOW *lpHighlightWindow;
        STACKMARKTEXT *lpMarkTextsStack=NULL;
        HWND hWnd=NULL;
        AEHDOC hDoc=NULL;

        if (IsExtCallParamValid(pd->lParam, 2))
          hWnd=(HWND)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          hDoc=(AEHDOC)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          lpMarkTextsStack=(STACKMARKTEXT *)GetExtCallParam(pd->lParam, 4);

        if (lpMarkTextsStack)
        {
          if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, hWnd, hDoc))
          {
            *lpMarkTextsStack=lpHighlightWindow->hMarkTextsStack;
          }
        }
      }
      else if (nAction == DLLA_HIGHLIGHT_ADDWINDOW)
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
            if (pd->dwSupport & PDS_STRANSI)
              MultiByteToWideChar(CP_ACP, 0, (char *)pAlias, -1, lpManual->wszAlias, MAX_PATH);
            else
              xstrcpynW(lpManual->wszAlias, (wchar_t *)pAlias, MAX_PATH);

            //Add HighLight element
            lpManual->dwDllFunction|=CODER_HIGHLIGHT;

            if (!StackGetHighLightWindow(&hHighLightWindowsStack, hWndEdit, NULL))
            {
              if (lpManual->lpHighlightWindow=StackInsertHighLightWindow(&hHighLightWindowsStack))
              {
                ((HIGHLIGHTWINDOW *)lpManual->lpHighlightWindow)->hWndEdit=lpManual->hWndEdit;
                ((HIGHLIGHTWINDOW *)lpManual->lpHighlightWindow)->hDocEdit=lpManual->hDocEdit;
                ((HIGHLIGHTWINDOW *)lpManual->lpHighlightWindow)->lpUser=lpManual;

                //Create FRAMEDATA in memory
                {
                  FRAMEDATA *lpFrameTarget;

                  if (lpFrameTarget=GlobalAlloc(GPTR, sizeof(FRAMEDATA)))
                  {
                    SendMessage(hMainWnd, AKD_FRAMEINIT, (WPARAM)NULL, (LPARAM)lpFrameTarget);
                    lpFrameTarget->ei.hWndEdit=lpManual->hWndEdit;
                    lpFrameTarget->ei.hDocEdit=lpManual->hDocEdit;
                    SendMessage(hMainWnd, AKD_FRAMEAPPLYEDIT, 0, (WPARAM)lpFrameTarget);
                  }
                  ((HIGHLIGHTWINDOW *)lpManual->lpHighlightWindow)->lpFrame=lpFrameTarget;
                }

                if (bInitHighLight)
                {
                  bUpdateTheme=TRUE;
                  UpdateEdit(hWndEdit, UE_DRAWRECT);
                }
              }
            }
          }
        }
      }
      else if (nAction == DLLA_HIGHLIGHT_DELWINDOW)
      {
        MANUALSET *lpManual;
        HWND hWndEdit=NULL;

        if (IsExtCallParamValid(pd->lParam, 2))
          hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);

        if (hWndEdit)
        {
          if (lpManual=StackGetManual(&hManualStack, hWndEdit, NULL))
          {
            if (lpManual->lpHighlightWindow && ((HIGHLIGHTWINDOW *)lpManual->lpHighlightWindow)->lpUser)
            {
              StackDeleteManual(&hManualStack, lpManual, CODER_HIGHLIGHT);
              SendMessage(hWndEdit, AEM_SETRECT, AERC_UPDATE, (LPARAM)NULL);
            }
          }
        }
      }
      else if (nAction == DLLA_HIGHLIGHT_GETWINDOW)
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
            if (lpManual->lpHighlightWindow && ((HIGHLIGHTWINDOW *)lpManual->lpHighlightWindow)->lpUser)
              bFound=TRUE;
          }
          *lpbFound=bFound;
        }
      }
    }
    else
    {
      xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_LOADFIRST), L"Coder::HighLight");
      MessageBoxW(pd->hMainWnd, wszBuffer, L"Coder::HighLight", MB_OK|MB_ICONEXCLAMATION);
    }

    //If plugin already loaded, stay in memory and don't change active status
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
    return;
  }

  //Initialize
  if (bInitHighLight)
  {
    UninitMain();
    UninitHighLight();
    UpdateEditAll(UE_DRAWRECT);

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitHighLight();
    if (!pd->bOnStart)
    {
      bUpdateTheme=TRUE;
      UpdateEditAll(UE_DRAWRECT);
    }

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

BOOL CALLBACK HighLightSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndGlobalFontStyleGroup;
  static HWND hWndGlobalFontStyleNormal;
  static HWND hWndGlobalFontStyleBold;
  static HWND hWndGlobalFontStyleItalic;
  static HWND hWndAutoMarkGroup;
  static HWND hWndAutoMarkEnable;
  static HWND hWndAutoMarkSymbols;
  static HWND hWndAutoMarkWords;
  static HWND hWndAutoMarkMaxSelLabel;
  static HWND hWndAutoMarkMaxSel;
  static HFONT hFontNormal;
  static HFONT hFontBold;
  static HFONT hFontItalic;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndGlobalFontStyleGroup=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_GROUP);
    hWndGlobalFontStyleNormal=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_NORMAL);
    hWndGlobalFontStyleBold=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_BOLD);
    hWndGlobalFontStyleItalic=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_ITALIC);
    hWndAutoMarkGroup=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_GROUP);
    hWndAutoMarkEnable=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_ENABLE);
    hWndAutoMarkSymbols=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_SYMBOLS);
    hWndAutoMarkWords=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_WORDS);
    hWndAutoMarkMaxSelLabel=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_MAXSEL_LABEL);
    hWndAutoMarkMaxSel=GetDlgItem(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_MAXSEL);

    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_GROUP, GetLangStringW(wLangModule, STRID_IGNORE_FONTSTYLE_GROUP));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_NORMAL, GetLangStringW(wLangModule, STRID_NORMAL));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_BOLD, GetLangStringW(wLangModule, STRID_BOLD));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_ITALIC, GetLangStringW(wLangModule, STRID_ITALIC));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_GROUP, GetLangStringW(wLangModule, STRID_AUTOMARK_GROUP));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_ENABLE, GetLangStringW(wLangModule, STRID_ENABLE));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_SYMBOLS, GetLangStringW(wLangModule, STRID_SYMBOLS));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_WORDS, GetLangStringW(wLangModule, STRID_WORDS));
    SetDlgItemTextWide(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_MAXSEL_LABEL, GetLangStringW(wLangModule, STRID_MAXSEL));

    SetDlgItemInt(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_MAXSEL, nAutoMarkMaxSel, FALSE);

    if (dwIgnoreFontStyle & AEHLO_IGNOREFONTNORMAL)
      SendMessage(hWndGlobalFontStyleNormal, BM_SETCHECK, BST_CHECKED, 0);
    if (dwIgnoreFontStyle & AEHLO_IGNOREFONTBOLD)
      SendMessage(hWndGlobalFontStyleBold, BM_SETCHECK, BST_CHECKED, 0);
    if (dwIgnoreFontStyle & AEHLO_IGNOREFONTITALIC)
      SendMessage(hWndGlobalFontStyleItalic, BM_SETCHECK, BST_CHECKED, 0);
    if (bAutoMarkEnable)
      SendMessage(hWndAutoMarkEnable, BM_SETCHECK, BST_CHECKED, 0);
    if (nAutoMarkType == MARKAUTO_SYMBOLS)
      SendMessage(hWndAutoMarkSymbols, BM_SETCHECK, BST_CHECKED, 0);
    else if (nAutoMarkType == MARKAUTO_WORDS)
      SendMessage(hWndAutoMarkWords, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hDlg, WM_COMMAND, IDC_HIGHLIGHT_SETUP_AUTOMARK_ENABLE, 0);

    //Set fonts
    {
      LOGFONTW lfGui;
      HFONT hGuiFont;

      //Default GUI font
      hGuiFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
      if (bOldWindows)
      {
        LOGFONTA lfA;

        GetObjectA(hGuiFont, sizeof(LOGFONTA), &lfA);
        LogFontAtoW(&lfA, &lfGui);
      }
      else GetObjectW(hGuiFont, sizeof(LOGFONTW), &lfGui);

      //Create normal font
      lfGui.lfWeight=FW_NORMAL;
      lfGui.lfItalic=FALSE;
      hFontNormal=(HFONT)CreateFontIndirectWide(&lfGui);
      SendMessage(hWndGlobalFontStyleNormal, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

      //Create bold font
      lfGui.lfWeight=FW_BOLD;
      lfGui.lfItalic=FALSE;
      hFontBold=(HFONT)CreateFontIndirectWide(&lfGui);
      SendMessage(hWndGlobalFontStyleBold, WM_SETFONT, (WPARAM)hFontBold, TRUE);

      //Create italic font
      lfGui.lfWeight=FW_NORMAL;
      lfGui.lfItalic=TRUE;
      hFontItalic=(HFONT)CreateFontIndirectWide(&lfGui);
      SendMessage(hWndGlobalFontStyleItalic, WM_SETFONT, (WPARAM)hFontItalic, TRUE);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) >= IDC_HIGHLIGHT_SETUP_GLOBAL_FONTSTYLE_GROUP &&
        LOWORD(wParam) <= IDC_HIGHLIGHT_SETUP_AUTOMARK_WORDS)
    {
      SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }

    if (LOWORD(wParam) == IDC_HIGHLIGHT_SETUP_AUTOMARK_ENABLE)
    {
      bState=(BOOL)SendMessage(hWndAutoMarkEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndAutoMarkSymbols, bState);
      EnableWindow(hWndAutoMarkWords, bState);
      EnableWindow(hWndAutoMarkMaxSelLabel, bState);
      EnableWindow(hWndAutoMarkMaxSel, bState);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_SETACTIVE)
    {
      if (nPropMaxVisitPage < PAGE_HIGHLIGHT)
        nPropMaxVisitPage=PAGE_HIGHLIGHT;
    }
    else if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      PSHNOTIFY *pshn=(PSHNOTIFY *)lParam;

      //Ignore font style
      dwIgnoreFontStyle=0;
      if (SendMessage(hWndGlobalFontStyleNormal, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwIgnoreFontStyle|=AEHLO_IGNOREFONTNORMAL;
      if (SendMessage(hWndGlobalFontStyleBold, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwIgnoreFontStyle|=AEHLO_IGNOREFONTBOLD;
      if (SendMessage(hWndGlobalFontStyleItalic, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwIgnoreFontStyle|=AEHLO_IGNOREFONTITALIC;

      //Auto mark
      bAutoMarkEnable=(BOOL)SendMessage(hWndAutoMarkEnable, BM_GETCHECK, 0, 0);
      if (SendMessage(hWndAutoMarkSymbols, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nAutoMarkType=MARKAUTO_SYMBOLS;
      else if (SendMessage(hWndAutoMarkWords, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nAutoMarkType=MARKAUTO_WORDS;
      nAutoMarkMaxSel=GetDlgItemInt(hDlg, IDC_HIGHLIGHT_SETUP_AUTOMARK_MAXSEL, NULL, FALSE);

      if (pshn->lParam)
      {
        //OK button pressed
        bSaveOptions=TRUE;
      }
      else
      {
        //Apply button pressed
        if (nPropMaxVisitPage == PAGE_HIGHLIGHT)
          UpdateAllOptions();
        return FALSE;
      }
    }

    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY ||
        ((NMHDR *)lParam)->code == (UINT)PSN_RESET)
    {
      SendMessage(hWndGlobalFontStyleNormal, WM_SETFONT, (WPARAM)NULL, TRUE);
      SendMessage(hWndGlobalFontStyleBold, WM_SETFONT, (WPARAM)NULL, TRUE);
      SendMessage(hWndGlobalFontStyleItalic, WM_SETFONT, (WPARAM)NULL, TRUE);
      if (hFontNormal) DeleteObject(hFontNormal);
      if (hFontBold) DeleteObject(hFontBold);
      if (hFontItalic) DeleteObject(hFontItalic);
    }
  }
  return FALSE;
}

BOOL CALLBACK HighLightParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (uMsg == AKDN_OPENDOCUMENT_START)
  {
    NOPENDOCUMENT *nod=(NOPENDOCUMENT *)lParam;
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (StackGetSyntaxFileByFile(&hSyntaxFilesStack, GetFileName(nod->wszFile, -1)) != StackGetSyntaxFileByFile(&hSyntaxFilesStack, GetFileName(lpFrame->ei.wszFile, -1)))
      bUpdateTheme=TRUE;

    //WM_PAINT can be send in AEM_STREAMIN of OpenDocument in Edit.c, so fill wszOpenFile.
    xstrcpynW(wszOpenFile, nod->wszFile, MAX_PATH);
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_START)
  {
    NSAVEDOCUMENT *nsd=(NSAVEDOCUMENT *)lParam;
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (nsd->dwFlags & SD_UPDATE)
    {
      if (StackGetSyntaxFileByFile(&hSyntaxFilesStack, GetFileName(nsd->wszFile, -1)) != StackGetSyntaxFileByFile(&hSyntaxFilesStack, GetFileName(lpFrame->ei.wszFile, -1)))
        bUpdateTheme=TRUE;
    }
  }
  else if (uMsg == AKDN_OPENDOCUMENT_FINISH)
  {
    wszOpenFile[0]=L'\0';
  }
  else if (uMsg == AKDN_SAVEDOCUMENT_FINISH)
  {
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (bUpdateTheme)
      UpdateEditAndClones(lpFrame->ei.hWndEdit, UE_DRAWRECT);
  }
  else if (uMsg == AKDN_EDIT_ONCLOSE)
  {
    HIGHLIGHTWINDOW *lpHighlightWindow;
    HWND hWndMaster=(HWND)SendMessage((HWND)wParam, AEM_GETMASTER, 0, 0);

    if (hDocEditCurrent == (AEHDOC)lParam)
      hDocEditCurrent=NULL;
    if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, hWndMaster?hWndMaster:(HWND)wParam, NULL))
      StackDeleteHighLightWindow(&hHighLightWindowsStack, lpHighlightWindow);
  }
  else if (uMsg == AKDN_EDIT_ONFINISH)
  {
    HIGHLIGHTWINDOW *lpHighlightWindow;
    HWND hWndMaster=(HWND)SendMessage((HWND)wParam, AEM_GETMASTER, 0, 0);

    if (!hWndMaster || hWndMaster == (HWND)wParam)
    {
      if (hDocEditCurrent == (AEHDOC)lParam)
        hDocEditCurrent=NULL;
      if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, (HWND)wParam, (AEHDOC)lParam))
        StackDeleteHighLightWindow(&hHighLightWindowsStack, lpHighlightWindow);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT || (HWND)*lResult)
    {
      if (((NMHDR *)lParam)->code == AEN_SELCHANGED)
      {
        AENSELCHANGE *aensc=(AENSELCHANGE *)lParam;
        SYNTAXFILE *lpSyntaxFile;
        DWORD *lpdwAutoMarkFlags;
        DWORD *lpdwAutoMarkFontStyle;
        DWORD *lpdwAutoMarkTextColor;
        DWORD *lpdwAutoMarkBkColor;

        if (!(aensc->dwType & AESCT_WRAP))
        {
          if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, aensc->hdr.hwndFrom, NULL, NULL))
          {
            StackRequestSyntaxFile(lpSyntaxFile);

            lpdwAutoMarkFlags=&lpSyntaxFile->dwAutoMarkFlags;
            lpdwAutoMarkFontStyle=&lpSyntaxFile->dwAutoMarkFontStyle;
            lpdwAutoMarkTextColor=&lpSyntaxFile->dwAutoMarkTextColor;
            lpdwAutoMarkBkColor=&lpSyntaxFile->dwAutoMarkBkColor;
          }
          else
          {
            //Document without syntax theme
            lpdwAutoMarkFlags=&dwAutoMarkFlags;
            lpdwAutoMarkFontStyle=&dwAutoMarkFontStyle;
            lpdwAutoMarkTextColor=&dwAutoMarkTextColor;
            lpdwAutoMarkBkColor=&dwAutoMarkBkColor;
          }

          if (bAutoMarkEnable &&
              (*lpdwAutoMarkFontStyle != AEHLS_NONE ||
               *lpdwAutoMarkTextColor != (DWORD)-1 ||
               *lpdwAutoMarkBkColor != (DWORD)-1))
          {
            HIGHLIGHTWINDOW *lpHighlightWindow;
            EDITINFO ei;
            CHARRANGE64 cr;

            if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)aensc->hdr.hwndFrom, (LPARAM)&ei))
            {
              if (lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, ei.hWndMaster?ei.hWndMaster:ei.hWndEdit, ei.hDocMaster?ei.hDocMaster:ei.hDocEdit))
              {
                SendMessage(ei.hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);

                if (!bFindingMark && cr.cpMax > cr.cpMin && cr.cpMax - cr.cpMin < nAutoMarkMaxSel &&
                     (nAutoMarkType == MARKAUTO_SYMBOLS ||
                       (nAutoMarkType == MARKAUTO_WORDS &&
                        SendMessage(ei.hWndEdit, AEM_ISDELIMITER, AEDLM_WORD|AEDLM_PREVCHAR, (LPARAM)&aensc->crSel.ciMin) &&
                        !SendMessage(ei.hWndEdit, AEM_ISDELIMITER, AEDLM_WORD, (LPARAM)&aensc->crSel.ciMin) &&
                        SendMessage(ei.hWndEdit, AEM_ISDELIMITER, AEDLM_WORD, (LPARAM)&aensc->crSel.ciMax) &&
                        !SendMessage(ei.hWndEdit, AEM_ISDELIMITER, AEDLM_WORD|AEDLM_PREVCHAR, (LPARAM)&aensc->crSel.ciMax))))
                {
                  wchar_t *wpMarkText;
                  INT_PTR nMarkTextLen=0;
                  BOOL bUpdate=FALSE;

                  if (UnmarkSelection(lpHighlightWindow, MARKID_SELECTION, (DWORD)-1, (DWORD)-1))
                    bUpdate=TRUE;

                  if (!SendMessage(ei.hWndEdit, AEM_GETLINENUMBER, AEGL_UNWRAPSELMULTILINE, 0))
                  {
                    if (wpMarkText=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)ei.hWndEdit, (LPARAM)&nMarkTextLen))
                    {
                      if (MarkSelection(lpHighlightWindow, wpMarkText, (int)nMarkTextLen, *lpdwAutoMarkTextColor, *lpdwAutoMarkBkColor, *lpdwAutoMarkFlags, *lpdwAutoMarkFontStyle, MARKID_SELECTION))
                        bUpdate=TRUE;
                      SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpMarkText);
                    }
                  }

                  //Update edit
                  if (bUpdate)
                  {
                    UpdateEditAndClones(ei.hWndEdit, UE_DRAWRECT);
                    //UpdateEditAll(UE_DRAWRECT);
                  }
                }
                else
                {
                  if (UnmarkSelection(lpHighlightWindow, MARKID_SELECTION, (DWORD)-1, (DWORD)-1))
                  {
                    UpdateEditAndClones(ei.hWndEdit, UE_DRAWRECT);
                    //UpdateEditAll(UE_DRAWRECT);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK HighLightEditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (uMsg == WM_PAINT)
  {
    SYNTAXFILE *lpSyntaxFile;
    HIGHLIGHTWINDOW *lpHighlightWindow;
    HWND hWndMaster=(HWND)SendMessage(hWnd, AEM_GETMASTER, 0, 0);
    AEHDOC hDocEdit;
    DWORD dwLockInherit;

    if (hWndMaster && hWndMaster != hWnd)
    {
      //Ignore theme checking for clones, cause they inherit theme from master
      return FALSE;
    }
    hDocEdit=(AEHDOC)SendMessage(hWnd, AEM_GETDOCUMENT, 0, 0);

    //Check theme
    if (bUpdateTheme || hDocEdit != hDocEditCurrent || wszOpenFile[0])
    {
      bUpdateTheme=FALSE;
      hDocEditCurrent=hDocEdit;
      lpSyntaxFile=NULL;

      if (wszOpenFile[0])
      {
        if (!StackGetManual(&hManualStack, hWnd, hDocEdit))
          lpSyntaxFile=StackGetSyntaxFileByFile(&hSyntaxFilesStack, GetFileName(wszOpenFile, -1));
      }
      if (!lpSyntaxFile)
        lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, NULL, NULL);
      if (lpSyntaxFile)
        StackRequestSyntaxFile(lpSyntaxFile);

      if (!(lpHighlightWindow=StackGetHighLightWindow(&hHighLightWindowsStack, hWnd, hDocEdit)))
      {
        if (lpHighlightWindow=StackInsertHighLightWindow(&hHighLightWindowsStack))
        {
          lpHighlightWindow->hWndEdit=hWnd;
          lpHighlightWindow->hDocEdit=hDocEdit;
          lpHighlightWindow->lpSyntaxFile=lpSyntaxFile;
          lpHighlightWindow->lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYEDITDOCUMENT, (LPARAM)hDocEdit);
        }
      }

      if (lpSyntaxFile)
      {
        if (!lpSyntaxFile->hThemeHighLight)
          CreateEditTheme(lpSyntaxFile, hWnd);

        if (lpSyntaxFile->hThemeHighLight != (HANDLE)SendMessage(hWnd, AEM_HLFINDTHEME, AEHLFT_CURRENT, 0))
        {
          if (lpHighlightWindow)
          {
            //Assign theme
            SendMessage(hWnd, AEM_HLSETTHEME, (WPARAM)lpSyntaxFile->hThemeHighLight, FALSE);
            SendMessage(hWnd, AEM_HLSETOPTIONS, AECOOP_XOR, AEHLO_IGNOREFONTNORMAL|AEHLO_IGNOREFONTBOLD|AEHLO_IGNOREFONTITALIC);
            SendMessage(hWnd, AEM_HLSETOPTIONS, AECOOP_OR, dwIgnoreFontStyle);

            //Set colors and font
            RestoreFontAndColors(lpHighlightWindow);
            GetFontAndColors(lpSyntaxFile, lpHighlightWindow);

            //Inheriting
            dwLockInherit=lpHighlightWindow->lpFrame->dwLockInherit;

            if (lpHighlightWindow->aecNew.dwFlags)
            {
              SetFrameInfo(lpHighlightWindow->lpFrame, FIS_COLORS, (UINT_PTR)&lpHighlightWindow->aecNew);
              dwLockInherit|=LI_COLORS;
            }
            if (lpHighlightWindow->dwFontFlags)
            {
              SendMessage(hMainWnd, AKD_SETFONTW, (WPARAM)hWnd, (LPARAM)&lpHighlightWindow->lfFontNew);
              dwLockInherit|=LI_FONT;
            }
            if (lpHighlightWindow->wszBkImageFileNew[0])
            {
              BKIMAGE bki;

              bki.wpFile=lpHighlightWindow->wszBkImageFileNew;
              bki.nAlpha=lpHighlightWindow->nBkImageAlphaNew;
              SetFrameInfo(lpHighlightWindow->lpFrame, FIS_BKIMAGE, (UINT_PTR)&bki);
              dwLockInherit|=LI_BKIMAGE;
            }

            if (dwLockInherit != lpHighlightWindow->lpFrame->dwLockInherit)
              SetFrameInfo(lpHighlightWindow->lpFrame, FIS_LOCKINHERIT, dwLockInherit);
          }
        }
      }
      else
      {
        //No syntax file associated, unassign theme
        if (lpHighlightWindow)
        {
          RestoreFontAndColors(lpHighlightWindow);
          UnassignTheme(lpHighlightWindow->hWndEdit);
        }
      }
    }
  }
  return FALSE;
}

/*
void CALLBACK HighLightEditMessagesRet(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT lResult)
{
  if (uMsg == WM_PAINT)
  {
    if (!bAkelEdit)
    {
      SYNTAXFILE *lpSyntaxFile;

      if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, NULL, NULL))
      {
        StackRequestSyntaxFile(lpSyntaxFile);

        if (bOldWindows)
          PaintRichEditA(lpSyntaxFile, hWnd, &rcUpdateRect);
        else
          PaintRichEditW(lpSyntaxFile, hWnd, &rcUpdateRect);
      }
    }
  }
}

void PaintRichEditA(SYNTAXFILE *lpSyntaxFile, HWND hWnd, RECT *rcUpdateRect)
{
  //HighLight RichEdit (AkelPad 3.x.x)
  GETTEXTRANGE gtr;
  CHARRANGE64 cr;
  LOGFONTA lfFont;
  TEXTMETRICA tm;
  RECT rcUpdate=*rcUpdateRect;
  RECT rcEdit;
  RECT rcDraw;
  HDC hDC;
  HRGN hRgn;
  HRGN hRgnOld;
  HFONT hFont;
  HFONT hFontOld;
  int nFirstChar;
  int nLastChar;
  int nFirstCharInUpdateLine;
  int i;

  SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)&rcEdit);
  if (rcUpdate.left < rcEdit.left) rcUpdate.left=rcEdit.left;
  if (rcUpdate.top < rcEdit.top) rcUpdate.top=rcEdit.top;
  if (rcUpdate.right > rcEdit.right) rcUpdate.right=rcEdit.right;
  if (rcUpdate.bottom > rcEdit.bottom) rcUpdate.bottom=rcEdit.bottom;
  rcDraw.left=rcEdit.left;
  rcDraw.top=rcUpdate.top;
  rcDraw.right=rcEdit.right;
  rcDraw.bottom=rcUpdate.bottom;

  if (hDC=GetDC(hWnd))
  {
    HideCaret(hWnd);
    hRgn=CreateRectRgn(rcEdit.left, rcEdit.top, rcEdit.right, rcEdit.bottom);
    hRgnOld=(HRGN)SelectObject(hDC, hRgn);
    SendMessage(hMainWnd, AKD_GETFONTA, (WPARAM)hWnd, (LPARAM)&lfFont);
    hFont=CreateFontIndirectA(&lfFont);
    hFontOld=(HFONT)SelectObject(hDC, hFont);
    GetTextMetricsA(hDC, &tm);
    SetBkMode(hDC, TRANSPARENT);

    i=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&rcDraw.left);
    i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, i);
    nFirstChar=SendMessage(hWnd, EM_LINEINDEX, i, 0);
    i=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&rcDraw.right);
    i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, i);
    i=SendMessage(hWnd, EM_LINEINDEX, i, 0);
    nLastChar=i + SendMessage(hWnd, EM_LINELENGTH, i, 0);
    gtr.cpMin=nFirstChar;
    gtr.cpMax=nLastChar;
    SendMessage(hWnd, EM_EXGETSEL64, 0, (LPARAM)&cr);

    if (SendMessage(hMainWnd, AKD_GETTEXTRANGE, (WPARAM)hWnd, (LPARAM)&gtr))
    {
      DELIMITERINFO *lpDelimElement=NULL;
      WORDINFO *lpWordElement=NULL;
      char szWord[MAX_PATH];
      wchar_t wszWord[MAX_PATH];
      char *szText=(char *)gtr.pText;
      char *pStrBegin;
      char *pStrEnd;
      POINT ptUpdateLine;
      POINT ptDelimiter;
      POINT ptWordBegin;
      POINT ptWordEnd;
      POINT ptSelBegin;
      POINT ptSelEnd;
      int nDelimiterIndex;
      int nWordBeginIndex;
      int nWordEndIndex;
      int nWordLen;

      //Get first visible word
      nFirstCharInUpdateLine=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&rcUpdate);
      if (nFirstCharInUpdateLine > nFirstChar) --nFirstCharInUpdateLine;
      if (nFirstCharInUpdateLine >= nLastChar) goto FreeText;
      pStrEnd=szText + (nFirstCharInUpdateLine - nFirstChar);

      while (pStrEnd > szText && (*pStrEnd != ' ' && *pStrEnd != '\t' && *pStrEnd != '\r' && !StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *pStrEnd)))
        --pStrEnd;
      while (*pStrEnd == ' ' || *pStrEnd == '\t' || *pStrEnd == '\r')
        ++pStrEnd;
      pStrBegin=pStrEnd;

      GetPosFromChar(hWnd, nFirstCharInUpdateLine, &ptUpdateLine, &tm);
      ptUpdateLine.x=rcUpdate.left;

      for (;;)
      {
        lpDelimElement=NULL;

        if (*pStrEnd == ' ' ||
            *pStrEnd == '\t' ||
            *pStrEnd == '\r' ||
            *pStrEnd == '\0' ||
            (lpDelimElement=StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *pStrEnd)))
        {
          ////DELIMITER
          if (lpDelimElement)
          {
            if ((int)lpDelimElement->dwColor2 != -1 || (int)lpDelimElement->dwColor1 != -1)
            {
              nDelimiterIndex=nFirstChar + (pStrEnd - szText);
              GetPosFromChar(hWnd, nDelimiterIndex, &ptDelimiter, &tm);
              if (ptDelimiter.y < ptUpdateLine.y) goto FreeText;

              if (ptDelimiter.y > ptUpdateLine.y ||
                  (pStrBegin == pStrEnd &&
                   ptDelimiter.y == ptUpdateLine.y &&
                   ptDelimiter.x >= rcUpdate.right &&
                   nDelimiterIndex > nFirstCharInUpdateLine))
              {
                ptUpdateLine.y+=tm.tmHeight;

                nFirstCharInUpdateLine=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&ptUpdateLine);
                i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nFirstCharInUpdateLine);
                i=SendMessage(hWnd, EM_LINEINDEX, i, 0);
                if (nFirstCharInUpdateLine > i) --nFirstCharInUpdateLine;
                if (nFirstCharInUpdateLine >= nLastChar) goto FreeText;
                pStrBegin=szText + (i - nFirstChar);
                pStrEnd=szText + (nFirstCharInUpdateLine - nFirstChar);

                while (pStrEnd > pStrBegin && (*pStrEnd != ' ' && *pStrEnd != '\t' && *pStrEnd != '\r' && !StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *pStrEnd)))
                  --pStrEnd;
                while (*pStrEnd == ' ' || *pStrEnd == '\t' || *pStrEnd == '\r')
                  ++pStrEnd;
                pStrBegin=pStrEnd;
                continue;
              }
              else
              {
                if (ptDelimiter.x + tm.tmMaxCharWidth > rcUpdate.left)
                {
                  rcDraw.left=ptDelimiter.x;
                  rcDraw.top=ptDelimiter.y;

                  if (!(nDelimiterIndex >= cr.cpMin && nDelimiterIndex < cr.cpMax))
                  {
                    if ((int)lpDelimElement->dwColor1 != -1)
                    {
                      SetTextColor(hDC, lpDelimElement->dwColor1);
                      DrawTextA(hDC, pStrEnd, 1, &rcDraw, DT_NOPREFIX);
                    }
                  }
                  else
                  {
                    if ((int)lpDelimElement->dwColor2 != -1)
                    {
                      SetTextColor(hDC, lpDelimElement->dwColor2);
                      DrawTextA(hDC, pStrEnd, 1, &rcDraw, DT_NOPREFIX);
                    }
                  }
                }
              }
            }
          }
          nWordLen=pStrEnd - pStrBegin;

          ////WORD
          if (nWordLen && nWordLen < MAX_PATH)
          {
            nWordBeginIndex=nFirstChar + (pStrBegin - szText);
            nWordEndIndex=nFirstChar + (pStrEnd - szText);
            GetPosFromChar(hWnd, nWordBeginIndex, &ptWordBegin, &tm);
            GetPosFromChar(hWnd, nWordEndIndex, &ptWordEnd, &tm);
            if (ptWordBegin.y < ptUpdateLine.y) goto FreeText;

            if (ptWordBegin.y == ptWordEnd.y)
            {
              if (bOldRichEdit)
              {
                //RichEdit 2.0: make word highlited, even if its beginning is not displayed
                if (ptWordBegin.x > ptWordEnd.x) ptWordBegin.x-=0x10000;
              }
            }

            if (ptWordBegin.y != ptWordEnd.y ||
                ptWordBegin.y > ptUpdateLine.y ||
                (ptWordBegin.y == ptUpdateLine.y &&
                 ptWordBegin.x >= rcUpdate.right &&
                 nWordBeginIndex > nFirstCharInUpdateLine))
            {
              ptUpdateLine.y+=tm.tmHeight;

              nFirstCharInUpdateLine=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&ptUpdateLine);
              i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nFirstCharInUpdateLine);
              i=SendMessage(hWnd, EM_LINEINDEX, i, 0);
              if (nFirstCharInUpdateLine > i) --nFirstCharInUpdateLine;
              if (nFirstCharInUpdateLine >= nLastChar) goto FreeText;
              pStrBegin=szText + (i - nFirstChar);
              pStrEnd=szText + (nFirstCharInUpdateLine - nFirstChar);

              while (pStrEnd > pStrBegin && (*pStrEnd != ' ' && *pStrEnd != '\t' && *pStrEnd != '\r' && !StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *pStrEnd)))
                --pStrEnd;
              while (*pStrEnd == ' ' || *pStrEnd == '\t' || *pStrEnd == '\r')
                ++pStrEnd;
              pStrBegin=pStrEnd;
              continue;
            }
            else
            {
              if (ptWordEnd.x > rcUpdate.left)
              {
                lstrcpynA(szWord, pStrBegin, nWordLen + 1);
                rcDraw.left=ptWordBegin.x;
                rcDraw.top=ptWordBegin.y;

                MultiByteToWideChar(CP_ACP, 0, szWord, -1, wszWord, MAX_PATH);
                if (lpWordElement=StackGetWord(&lpSyntaxFile->hWordStack, wszWord, nWordLen))
                {
                  if (nWordBeginIndex >= cr.cpMax || nWordEndIndex <= cr.cpMin)
                  {
                    if ((int)lpWordElement->dwColor1 != -1)
                    {
                      SetTextColor(hDC, lpWordElement->dwColor1);
                      DrawTextA(hDC, pStrBegin, nWordLen, &rcDraw, DT_NOPREFIX);
                    }
                  }
                  else
                  {
                    //Highlight selection
                    GetPosFromChar(hWnd, cr.cpMin, &ptSelBegin, &tm);
                    GetPosFromChar(hWnd, cr.cpMax, &ptSelEnd, &tm);

                    if ((int)lpWordElement->dwColor1 != -1)
                    {
                      SetTextColor(hDC, lpWordElement->dwColor1);

                      if (nWordBeginIndex < cr.cpMin)
                      {
                        rcDraw.left=ptWordBegin.x;
                        DrawTextA(hDC, pStrBegin, cr.cpMin - nWordBeginIndex, &rcDraw, DT_NOPREFIX);
                      }
                      if (nWordEndIndex > cr.cpMax)
                      {
                        rcDraw.left=ptSelEnd.x;
                        DrawTextA(hDC, pStrBegin + (cr.cpMax - nWordBeginIndex), nWordEndIndex - cr.cpMax, &rcDraw, DT_NOPREFIX);
                      }
                    }
                    if ((int)lpWordElement->dwColor2 != -1)
                    {
                      if (cr.cpMin > nWordBeginIndex)
                        rcDraw.left=ptSelBegin.x;
                      else
                        rcDraw.left=ptWordBegin.x;
                      SetTextColor(hDC, lpWordElement->dwColor2);
                      DrawTextA(hDC, pStrBegin + (max(cr.cpMin, nWordBeginIndex) - nWordBeginIndex), min(cr.cpMax, nWordEndIndex) - max(cr.cpMin, nWordBeginIndex), &rcDraw, DT_NOPREFIX);
                    }
                  }
                }
              }
            }
          }
          if (!*pStrEnd) break;
          while (*++pStrEnd == ' ' || *pStrEnd == '\t' || *pStrEnd == '\r');
          pStrBegin=pStrEnd;
        }
        else ++pStrEnd;
      }
      FreeText:
      SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
    }
    if (hFontOld) SelectObject(hDC, hFontOld);
    DeleteObject(hFont);
    if (hRgnOld) SelectObject(hDC, hRgnOld);
    DeleteObject(hRgn);
    ShowCaret(hWnd);
    ReleaseDC(hWnd, hDC);
  }
}

void PaintRichEditW(SYNTAXFILE *lpSyntaxFile, HWND hWnd, RECT *rcUpdateRect)
{
  //HighLight RichEdit (AkelPad 3.x.x)
  GETTEXTRANGE gtr;
  CHARRANGE64 cr;
  LOGFONTW lfFont;
  TEXTMETRICW tm;
  RECT rcUpdate=*rcUpdateRect;
  RECT rcEdit;
  RECT rcDraw;
  HDC hDC;
  HRGN hRgn;
  HRGN hRgnOld;
  HFONT hFont;
  HFONT hFontOld;
  int nFirstChar;
  int nLastChar;
  int nFirstCharInUpdateLine;
  int i;

  SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)&rcEdit);
  if (rcUpdate.left < rcEdit.left) rcUpdate.left=rcEdit.left;
  if (rcUpdate.top < rcEdit.top) rcUpdate.top=rcEdit.top;
  if (rcUpdate.right > rcEdit.right) rcUpdate.right=rcEdit.right;
  if (rcUpdate.bottom > rcEdit.bottom) rcUpdate.bottom=rcEdit.bottom;
  rcDraw.left=rcEdit.left;
  rcDraw.top=rcUpdate.top;
  rcDraw.right=rcEdit.right;
  rcDraw.bottom=rcUpdate.bottom;

  if (hDC=GetDC(hWnd))
  {
    HideCaret(hWnd);
    hRgn=CreateRectRgn(rcEdit.left, rcEdit.top, rcEdit.right, rcEdit.bottom);
    hRgnOld=(HRGN)SelectObject(hDC, hRgn);
    SendMessage(hMainWnd, AKD_GETFONTW, (WPARAM)hWnd, (LPARAM)&lfFont);
    hFont=CreateFontIndirectW(&lfFont);
    hFontOld=(HFONT)SelectObject(hDC, hFont);
    GetTextMetricsW(hDC, &tm);
    SetBkMode(hDC, TRANSPARENT);

    i=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&rcDraw.left);
    i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, i);
    nFirstChar=SendMessage(hWnd, EM_LINEINDEX, i, 0);
    i=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&rcDraw.right);
    i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, i);
    i=SendMessage(hWnd, EM_LINEINDEX, i, 0);
    nLastChar=i + SendMessage(hWnd, EM_LINELENGTH, i, 0);
    gtr.cpMin=nFirstChar;
    gtr.cpMax=nLastChar;
    SendMessage(hWnd, EM_EXGETSEL64, 0, (LPARAM)&cr);

    if (SendMessage(hMainWnd, AKD_GETTEXTRANGE, (WPARAM)hWnd, (LPARAM)&gtr))
    {
      DELIMITERINFO *lpDelimElement=NULL;
      WORDINFO *lpWordElement=NULL;
      wchar_t wszWord[MAX_PATH];
      wchar_t *wszText=(wchar_t *)gtr.pText;
      wchar_t *wpStrBegin;
      wchar_t *wpStrEnd;
      POINT ptUpdateLine;
      POINT ptDelimiter;
      POINT ptWordBegin;
      POINT ptWordEnd;
      POINT ptSelBegin;
      POINT ptSelEnd;
      int nDelimiterIndex;
      int nWordBeginIndex;
      int nWordEndIndex;
      int nWordLen;

      //Get first visible word
      nFirstCharInUpdateLine=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&rcUpdate);
      if (nFirstCharInUpdateLine > nFirstChar) --nFirstCharInUpdateLine;
      if (nFirstCharInUpdateLine >= nLastChar) goto FreeText;
      wpStrEnd=wszText + (nFirstCharInUpdateLine - nFirstChar);

      while (wpStrEnd > wszText && (*wpStrEnd != ' ' && *wpStrEnd != '\t' && *wpStrEnd != '\r' && !StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *wpStrEnd)))
        --wpStrEnd;
      while (*wpStrEnd == ' ' || *wpStrEnd == '\t' || *wpStrEnd == '\r')
        ++wpStrEnd;
      wpStrBegin=wpStrEnd;

      GetPosFromChar(hWnd, nFirstCharInUpdateLine, &ptUpdateLine, (TEXTMETRICA *)&tm);
      ptUpdateLine.x=rcUpdate.left;

      for (;;)
      {
        lpDelimElement=NULL;

        if (*wpStrEnd == ' ' ||
            *wpStrEnd == '\t' ||
            *wpStrEnd == '\r' ||
            *wpStrEnd == '\0' ||
            (lpDelimElement=StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *wpStrEnd)))
        {
          ////DELIMITER
          if (lpDelimElement)
          {
            if ((int)lpDelimElement->dwColor2 != -1 || (int)lpDelimElement->dwColor1 != -1)
            {
              nDelimiterIndex=nFirstChar + (wpStrEnd - wszText);
              GetPosFromChar(hWnd, nDelimiterIndex, &ptDelimiter, (TEXTMETRICA *)&tm);
              if (ptDelimiter.y < ptUpdateLine.y) goto FreeText;

              if (ptDelimiter.y > ptUpdateLine.y ||
                  (wpStrBegin == wpStrEnd &&
                   ptDelimiter.y == ptUpdateLine.y &&
                   ptDelimiter.x >= rcUpdate.right &&
                   nDelimiterIndex > nFirstCharInUpdateLine))
              {
                ptUpdateLine.y+=tm.tmHeight;

                nFirstCharInUpdateLine=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&ptUpdateLine);
                i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nFirstCharInUpdateLine);
                i=SendMessage(hWnd, EM_LINEINDEX, i, 0);
                if (nFirstCharInUpdateLine > i) --nFirstCharInUpdateLine;
                if (nFirstCharInUpdateLine >= nLastChar) goto FreeText;
                wpStrBegin=wszText + (i - nFirstChar);
                wpStrEnd=wszText + (nFirstCharInUpdateLine - nFirstChar);

                while (wpStrEnd > wpStrBegin && (*wpStrEnd != ' ' && *wpStrEnd != '\t' && *wpStrEnd != '\r' && !StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *wpStrEnd)))
                  --wpStrEnd;
                while (*wpStrEnd == ' ' || *wpStrEnd == '\t' || *wpStrEnd == '\r')
                  ++wpStrEnd;
                wpStrBegin=wpStrEnd;
                continue;
              }
              else
              {
                if (ptDelimiter.x + tm.tmMaxCharWidth > rcUpdate.left)
                {
                  rcDraw.left=ptDelimiter.x;
                  rcDraw.top=ptDelimiter.y;

                  if (!(nDelimiterIndex >= cr.cpMin && nDelimiterIndex < cr.cpMax))
                  {
                    if ((int)lpDelimElement->dwColor1 != -1)
                    {
                      SetTextColor(hDC, lpDelimElement->dwColor1);
                      DrawTextW(hDC, wpStrEnd, 1, &rcDraw, DT_NOPREFIX);
                    }
                  }
                  else
                  {
                    if ((int)lpDelimElement->dwColor2 != -1)
                    {
                      SetTextColor(hDC, lpDelimElement->dwColor2);
                      DrawTextW(hDC, wpStrEnd, 1, &rcDraw, DT_NOPREFIX);
                    }
                  }
                }
              }
            }
          }
          nWordLen=wpStrEnd - wpStrBegin;

          ////WORD
          if (nWordLen && nWordLen < MAX_PATH)
          {
            nWordBeginIndex=nFirstChar + (wpStrBegin - wszText);
            nWordEndIndex=nFirstChar + (wpStrEnd - wszText);
            GetPosFromChar(hWnd, nWordBeginIndex, &ptWordBegin, (TEXTMETRICA *)&tm);
            GetPosFromChar(hWnd, nWordEndIndex, &ptWordEnd, (TEXTMETRICA *)&tm);
            if (ptWordBegin.y < ptUpdateLine.y) goto FreeText;

            if (ptWordBegin.y == ptWordEnd.y)
            {
              if (bOldRichEdit)
              {
                //RichEdit 2.0: make word highlited, even if its beginning is not displayed
                if (ptWordBegin.x > ptWordEnd.x) ptWordBegin.x-=0x10000;
              }
            }

            if (ptWordBegin.y != ptWordEnd.y ||
                ptWordBegin.y > ptUpdateLine.y ||
                (ptWordBegin.y == ptUpdateLine.y &&
                 ptWordBegin.x >= rcUpdate.right &&
                 nWordBeginIndex > nFirstCharInUpdateLine))
            {
              ptUpdateLine.y+=tm.tmHeight;

              nFirstCharInUpdateLine=SendMessage(hWnd, EM_CHARFROMPOS, 0, (LPARAM)&ptUpdateLine);
              i=SendMessage(hWnd, EM_EXLINEFROMCHAR, 0, nFirstCharInUpdateLine);
              i=SendMessage(hWnd, EM_LINEINDEX, i, 0);
              if (nFirstCharInUpdateLine > i) --nFirstCharInUpdateLine;
              if (nFirstCharInUpdateLine >= nLastChar) goto FreeText;
              wpStrBegin=wszText + (i - nFirstChar);
              wpStrEnd=wszText + (nFirstCharInUpdateLine - nFirstChar);

              while (wpStrEnd > wpStrBegin && (*wpStrEnd != ' ' && *wpStrEnd != '\t' && *wpStrEnd != '\r' && !StackGetDelimiter(&lpSyntaxFile->hDelimiterStack, *wpStrEnd)))
                --wpStrEnd;
              while (*wpStrEnd == ' ' || *wpStrEnd == '\t' || *wpStrEnd == '\r')
                ++wpStrEnd;
              wpStrBegin=wpStrEnd;
              continue;
            }
            else
            {
              if (ptWordEnd.x > rcUpdate.left)
              {
                xstrcpynW(wszWord, wpStrBegin, nWordLen + 1);
                rcDraw.left=ptWordBegin.x;
                rcDraw.top=ptWordBegin.y;

                if (lpWordElement=StackGetWord(&lpSyntaxFile->hWordStack, wszWord, nWordLen))
                {
                  if (nWordBeginIndex >= cr.cpMax || nWordEndIndex <= cr.cpMin)
                  {
                    if ((int)lpWordElement->dwColor1 != -1)
                    {
                      SetTextColor(hDC, lpWordElement->dwColor1);
                      DrawTextW(hDC, wpStrBegin, nWordLen, &rcDraw, DT_NOPREFIX);
                    }
                  }
                  else
                  {
                    //Highlight selection
                    GetPosFromChar(hWnd, cr.cpMin, &ptSelBegin, (TEXTMETRICA *)&tm);
                    GetPosFromChar(hWnd, cr.cpMax, &ptSelEnd, (TEXTMETRICA *)&tm);

                    if ((int)lpWordElement->dwColor1 != -1)
                    {
                      SetTextColor(hDC, lpWordElement->dwColor1);

                      if (nWordBeginIndex < cr.cpMin)
                      {
                        rcDraw.left=ptWordBegin.x;
                        DrawTextW(hDC, wpStrBegin, cr.cpMin - nWordBeginIndex, &rcDraw, DT_NOPREFIX);
                      }
                      if (nWordEndIndex > cr.cpMax)
                      {
                        rcDraw.left=ptSelEnd.x;
                        DrawTextW(hDC, wpStrBegin + (cr.cpMax - nWordBeginIndex), nWordEndIndex - cr.cpMax, &rcDraw, DT_NOPREFIX);
                      }
                    }
                    if ((int)lpWordElement->dwColor2 != -1)
                    {
                      if (cr.cpMin > nWordBeginIndex)
                        rcDraw.left=ptSelBegin.x;
                      else
                        rcDraw.left=ptWordBegin.x;
                      SetTextColor(hDC, lpWordElement->dwColor2);
                      DrawTextW(hDC, wpStrBegin + (max(cr.cpMin, nWordBeginIndex) - nWordBeginIndex), min(cr.cpMax, nWordEndIndex) - max(cr.cpMin, nWordBeginIndex), &rcDraw, DT_NOPREFIX);
                    }
                  }
                }
              }
            }
          }
          if (!*wpStrEnd) break;
          while (*++wpStrEnd == ' ' || *wpStrEnd == '\t' || *wpStrEnd == '\r');
          wpStrBegin=wpStrEnd;
        }
        else ++wpStrEnd;
      }
      FreeText:
      SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
    }
    if (hFontOld) SelectObject(hDC, hFontOld);
    DeleteObject(hFont);
    if (hRgnOld) SelectObject(hDC, hRgnOld);
    DeleteObject(hRgn);
    ShowCaret(hWnd);
    ReleaseDC(hWnd, hDC);
  }
}

void GetPosFromChar(HWND hWnd, int nCharIndex, POINT *pt, TEXTMETRICA *tm)
{
  DWORD dwPosition;

  if (bOldRichEdit)
  {
    dwPosition=SendMessage(hWnd, EM_POSFROMCHAR, nCharIndex, (LPARAM)NULL);
    pt->x=LOWORD(dwPosition);
    pt->y=HIWORD(dwPosition);

    if (tm)
    {
      if (pt->x > 0x10000 - tm->tmMaxCharWidth)
        pt->x-=0x10000;
      if (pt->y > 0x10000 - tm->tmHeight)
        pt->y-=0x10000;
    }
  }
  else
  {
    SendMessage(hWnd, EM_POSFROMCHAR, (WPARAM)pt, nCharIndex);
  }
}
*/

WORDINFO* StackInsertWord(STACKWORD *hStack, STACKWORDALPHA *hAlphaStack, STACKWORDORDER *hOrderStack, const wchar_t *wpWord, int nWordLen)
{
  WORDINFO *lpElement;
  WORDINFO *lpElementNew=NULL;
  WORDALPHA *lpWordAlpha=NULL;
  WORDORDER *lpWordOrder=NULL;

  if ((DWORD)nWordLen < sizeof(hStack->lpWordLens) / sizeof(INT_PTR))
  {
    if (hStack->lpWordLens[nWordLen])
    {
      lpElement=(WORDINFO *)hStack->lpWordLens[nWordLen];
    }
    else
    {
      lpElement=(WORDINFO *)hStack->first;

      while (lpElement)
      {
        if (lpElement->nWordLen >= nWordLen)
          break;

        lpElement=lpElement->next;
      }
    }
    StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement, (stack **)&lpElementNew, sizeof(WORDINFO));

    if (lpElementNew)
    {
      hStack->lpWordLens[nWordLen]=(INT_PTR)lpElementNew;

      if (bAddHighLightWords)
      {
        //Add to the alphabetically sorted stack
        if (lpWordAlpha=StackInsertWordAlpha(hAlphaStack, wpWord))
        {
          lpWordAlpha->wpWord=wpWord;
          lpWordAlpha->nWordLen=nWordLen;
          lpWordAlpha->lpWordInfo=lpElementNew;
        }
      }
      //Remember initial order
      if (!StackInsertIndex((stack **)&hOrderStack->first, (stack **)&hOrderStack->last, (stack **)&lpWordOrder, -1, sizeof(WORDORDER)))
        lpWordOrder->lpWordInfo=lpElementNew;
    }
  }
  return lpElementNew;
}

WORDALPHA* StackInsertWordAlpha(STACKWORDALPHA *hStack, const wchar_t *wpWord)
{
  WORDALPHA *lpElement=(WORDALPHA *)hStack->first;
  WORDALPHA *lpElementNew=NULL;
  wchar_t wchFirstChar=CompleteFirstChar(*wpWord);

  if (wchFirstChar < FIRST_NONLATIN)
  {
    if (hStack->lpSorted[wchFirstChar])
      lpElement=(WORDALPHA *)hStack->lpSorted[wchFirstChar];
    else
      lpElement=(WORDALPHA *)hStack->first;
  }
  else lpElement=(WORDALPHA *)hStack->lpSorted[FIRST_NONLATIN];

  while (lpElement)
  {
    if (lpElement->wchFirstChar >= wchFirstChar)
    {
      if (CompleteStrCmp(0, lpElement->wpWord, wpWord) >= 0)
        break;
    }
    lpElement=lpElement->next;
  }
  StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement, (stack **)&lpElementNew, sizeof(WORDALPHA));

  if (lpElementNew)
  {
    if (wchFirstChar < FIRST_NONLATIN)
    {
      if (!hStack->lpSorted[wchFirstChar] || (INT_PTR)lpElement == hStack->lpSorted[wchFirstChar])
        hStack->lpSorted[wchFirstChar]=(INT_PTR)lpElementNew;
    }
    else
    {
      if (!hStack->lpSorted[FIRST_NONLATIN] || (INT_PTR)lpElement == hStack->lpSorted[FIRST_NONLATIN])
        hStack->lpSorted[FIRST_NONLATIN]=(INT_PTR)lpElementNew;
    }
    lpElementNew->wchFirstChar=wchFirstChar;
  }
  return lpElementNew;
}

WORDINFO* StackGetWord(STACKWORD *hStack, wchar_t *wpWord, int nWordLen)
{
  WORDINFO *lpElement=NULL;

  if ((DWORD)nWordLen < sizeof(hStack->lpWordLens) / sizeof(INT_PTR))
  {
    lpElement=(WORDINFO *)hStack->lpWordLens[nWordLen];

    while (lpElement)
    {
      if (lpElement->nWordLen != nWordLen)
        break;
      if (lpElement->dwFlags & AEHLF_MATCHCASE)
      {
        if (!xstrcmpW(lpElement->wpWord, wpWord))
          return lpElement;
      }
      else
      {
        if (!xstrcmpiW(lpElement->wpWord, wpWord))
          return lpElement;
      }
      lpElement=lpElement->next;
    }
  }
  return NULL;
}

void StackFreeWord(STACKWORD *hStack, STACKWORDALPHA *hAlphaStack, STACKWORDORDER *hOrderStack)
{
  WORDINFO *lpElement=(WORDINFO *)hStack->first;

  while (lpElement)
  {
    GlobalFree((HGLOBAL)lpElement->wpWord);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  xmemset(hStack->lpWordLens, 0, sizeof(hStack->lpWordLens));

  if (hAlphaStack)
    StackClear((stack **)&hAlphaStack->first, (stack **)&hAlphaStack->last);
  if (hOrderStack)
    StackClear((stack **)&hOrderStack->first, (stack **)&hOrderStack->last);
}

QUOTEINFO* StackInsertQuote(STACKQUOTE *hStack, int nQuoteStartLen)
{
  QUOTEINFO *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(QUOTEINFO));
  return lpElement;
}

void StackFreeQuote(STACKQUOTE *hStack)
{
  QUOTEINFO *lpElement=(QUOTEINFO *)hStack->first;

  while (lpElement)
  {
    GlobalFree((HGLOBAL)lpElement->wpQuoteStart);
    GlobalFree((HGLOBAL)lpElement->wpQuoteEnd);
    if (lpElement->wpQuoteInclude) GlobalFree((HGLOBAL)lpElement->wpQuoteInclude);
    if (lpElement->wpQuoteExclude) GlobalFree((HGLOBAL)lpElement->wpQuoteExclude);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

HIGHLIGHTWINDOW* StackInsertHighLightWindow(STACKHIGHLIGHTWINDOW *hStack)
{
  HIGHLIGHTWINDOW *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(HIGHLIGHTWINDOW));

  return lpElement;
}

HIGHLIGHTWINDOW* StackGetHighLightWindow(STACKHIGHLIGHTWINDOW *hStack, HWND hWndMaster, AEHDOC hDocMaster)
{
  HIGHLIGHTWINDOW *lpHighlightWindow;

  for (lpHighlightWindow=hStack->first; lpHighlightWindow; lpHighlightWindow=lpHighlightWindow->next)
  {
    if (lpHighlightWindow->hWndEdit == hWndMaster)
    {
      if (lpHighlightWindow->lpUser)
        return lpHighlightWindow;
      break;
    }
  }

  if (lpHighlightWindow && nMDI == WMD_PMDI)
  {
    if (!hDocMaster)
      hDocMaster=(AEHDOC)SendMessage(hWndMaster, AEM_GETDOCUMENT, 0, 0);

    for (lpHighlightWindow=hStack->first; lpHighlightWindow; lpHighlightWindow=lpHighlightWindow->next)
    {
      if (lpHighlightWindow->hDocEdit == hDocMaster)
        return lpHighlightWindow;
    }
  }
  return lpHighlightWindow;
}

void StackDeleteHighLightWindow(STACKHIGHLIGHTWINDOW *hStack, HIGHLIGHTWINDOW *lpHighlightWindow)
{
  FRAMEDATA *lpFrameToRestore=NULL;
  MANUALSET *lpUser=lpHighlightWindow->lpUser;

  if (nMDI == WMD_PMDI && !lpUser)
  {
    //In WMD_PMDI mode to work with non-active frame we should activate it first.
    lpFrameToRestore=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, (LPARAM)NULL);
    if (lpFrameToRestore == lpHighlightWindow->lpFrame)
      lpFrameToRestore=NULL;
    else
      SendMessage(hMainWnd, AKD_FRAMEACTIVATE, FWA_NOVISUPDATE|FWA_NOUPDATEORDER, (LPARAM)lpHighlightWindow->lpFrame);
  }

  UnmarkSelection(lpHighlightWindow, 0, (DWORD)-1, (DWORD)-1);
  RestoreFontAndColors(lpHighlightWindow);
  UnassignTheme(lpHighlightWindow->hWndEdit);
  if (lpUser) GlobalFree((HGLOBAL)lpHighlightWindow->lpFrame);
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpHighlightWindow);

  if (nMDI == WMD_PMDI && !lpUser)
  {
    if (lpFrameToRestore)
    {
      SendMessage(hMainWnd, AKD_FRAMEACTIVATE, FWA_NOVISUPDATE|FWA_NOUPDATEORDER, (LPARAM)lpFrameToRestore);
      lpFrameToRestore=NULL;
    }
  }
}

void StackFreeHighLightWindow(STACKHIGHLIGHTWINDOW *hStack)
{
  HIGHLIGHTWINDOW *lpElement;
  HIGHLIGHTWINDOW *lpNextElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpNextElement)
  {
    lpNextElement=lpElement->next;

    if (lpElement->lpUser)
      StackDeleteManual(&hManualStack, lpElement->lpUser, CODER_HIGHLIGHT);
    else
      StackDeleteHighLightWindow(hStack, lpElement);
  }
}

MARKTEXT* StackInsertMark(HIGHLIGHTWINDOW *lpHighlightWindow)
{
  MARKTEXT *lpMarkText=NULL;

  StackInsertIndex((stack **)&lpHighlightWindow->hMarkTextsStack.first, (stack **)&lpHighlightWindow->hMarkTextsStack.last, (stack **)&lpMarkText, -1, sizeof(MARKTEXT));
  return lpMarkText;
}

MARKTEXT* StackGetMarkByColorID(HIGHLIGHTWINDOW *lpHighlightWindow, DWORD dwMarkID, DWORD dwColorText, DWORD dwColorBk)
{
  MARKTEXT *lpMarkText;
  AEMARKTEXTITEMW *lpMarkItem;

  if (dwMarkID == MARKID_AUTOASSIGN)
  {
    for (lpMarkText=lpHighlightWindow->hMarkTextsStack.first; lpMarkText; lpMarkText=lpMarkText->next)
    {
      if (lpMarkText->dwMarkID >= MARKID_AUTOMIN && lpMarkText->dwMarkID <= MARKID_AUTOMAX)
      {
        lpMarkItem=(AEMARKTEXTITEMW *)lpMarkText->hMarkTextHandle;

        if ((dwColorText == (DWORD)-1 || lpMarkItem->crText == dwColorText) &&
            (dwColorBk == (DWORD)-1 || lpMarkItem->crBk == dwColorBk))
        {
          return lpMarkText;
        }
      }
    }
  }
  else
  {
    for (lpMarkText=lpHighlightWindow->hMarkTextsStack.first; lpMarkText; lpMarkText=lpMarkText->next)
    {
      if (lpMarkText->dwMarkID == dwMarkID)
        return lpMarkText;
    }
  }
  return NULL;
}

MARKTEXT* StackGetMarkByText(HIGHLIGHTWINDOW *lpHighlightWindow, const wchar_t *wpText, int nTextLen)
{
  MARKTEXT *lpMarkText;
  MARKTEXT *lpNextMarkText;
  AEMARKTEXTITEMW *lpMarkItem;

  if (nTextLen == -1)
    nTextLen=(int)xstrlenW(wpText);

  for (lpMarkText=lpHighlightWindow->hMarkTextsStack.first; lpMarkText; lpMarkText=lpNextMarkText)
  {
    lpNextMarkText=lpMarkText->next;
    lpMarkItem=(AEMARKTEXTITEMW *)lpMarkText->hMarkTextHandle;

    if (nTextLen == lpMarkItem->nMarkTextLen &&
        ((lpMarkItem->dwFlags & AEHLF_MATCHCASE) ?
         !xstrcmpW(lpMarkItem->pMarkText, wpText) :
         !xstrcmpiW(lpMarkItem->pMarkText, wpText)))
    {
      return lpMarkText;
    }
  }
  return NULL;
}

DWORD StackAssignMarkID(HIGHLIGHTWINDOW *lpHighlightWindow)
{
  MARKTEXT *lpMarkText;
  DWORD dwMarkID=MARKID_AUTOMIN;

  for (lpMarkText=lpHighlightWindow->hMarkTextsStack.first; lpMarkText; lpMarkText=lpMarkText->next)
  {
    if (lpMarkText->dwMarkID < MARKID_AUTOMAX)
    {
      if (lpMarkText->dwMarkID >= dwMarkID)
        dwMarkID=lpMarkText->dwMarkID + 1;
    }
  }
  return dwMarkID;
}

BOOL StackIsAnyMark(HIGHLIGHTWINDOW *lpHighlightWindow)
{
  if (lpHighlightWindow->hMarkTextsStack.first)
    return TRUE;
  return FALSE;
}

void StackDeleteMark(HIGHLIGHTWINDOW *lpHighlightWindow, MARKTEXT *lpMarkText)
{
  SendMessage(lpHighlightWindow->hWndEdit, AEM_HLDELETEMARKTEXT, (WPARAM)NULL, (LPARAM)lpMarkText->hMarkTextHandle);
  StackDelete((stack **)&lpHighlightWindow->hMarkTextsStack.first, (stack **)&lpHighlightWindow->hMarkTextsStack.last, (stack *)lpMarkText);
}

BOOL StackFreeMark(HIGHLIGHTWINDOW *lpHighlightWindow)
{
  MARKTEXT *lpMarkText;
  MARKTEXT *lpNextMarkText;
  BOOL bResult=(lpHighlightWindow->hMarkTextsStack.first)?TRUE:FALSE;

  for (lpMarkText=(MARKTEXT *)lpHighlightWindow->hMarkTextsStack.first; lpMarkText; lpMarkText=lpNextMarkText)
  {
    lpNextMarkText=lpMarkText->next;
    StackDeleteMark(lpHighlightWindow, lpMarkText);
  }
  return bResult;
}

BOOL MarkSelection(HIGHLIGHTWINDOW *lpHighlightWindow, const wchar_t *wpText, int nTextLen, DWORD dwColorText, DWORD dwColorBk, DWORD dwFlags, DWORD dwFontStyle, DWORD dwMarkID)
{
  BOOL bResult=FALSE;

  if (dwMarkID)
  {
    AEMARKTEXTITEMW mti;
    AEHMARKTEXT hMarkTextHandle;
    MARKTEXT *lpMarkText;

    mti.nIndex=1;
    mti.pMarkText=wpText;
    mti.nMarkTextLen=(nTextLen == -1?(int)xstrlenW(wpText):nTextLen);
    mti.dwFlags=dwFlags;
    mti.crText=dwColorText;
    mti.crBk=dwColorBk;
    mti.dwFontStyle=dwFontStyle;

    if (hMarkTextHandle=(AEHMARKTEXT)SendMessage(lpHighlightWindow->hWndEdit, AEM_HLADDMARKTEXTW, (WPARAM)NULL, (LPARAM)&mti))
    {
      if (lpMarkText=StackInsertMark(lpHighlightWindow))
      {
        lpMarkText->hMarkTextHandle=hMarkTextHandle;
        if (dwMarkID == MARKID_AUTOASSIGN)
          lpMarkText->dwMarkID=StackAssignMarkID(lpHighlightWindow);
        else
          lpMarkText->dwMarkID=dwMarkID;

        bResult=TRUE;
      }
    }
  }
  return bResult;
}

BOOL UnmarkSelection(HIGHLIGHTWINDOW *lpHighlightWindow, DWORD dwMarkID, DWORD dwColorText, DWORD dwColorBk)
{
  MARKTEXT *lpMarkText;
  BOOL bResult=FALSE;

  if (!dwMarkID)
    return StackFreeMark(lpHighlightWindow);

  while (lpMarkText=StackGetMarkByColorID(lpHighlightWindow, dwMarkID, dwColorText, dwColorBk))
  {
    StackDeleteMark(lpHighlightWindow, lpMarkText);
    bResult=TRUE;
  }
  return bResult;
}

BOOL FindMark(HIGHLIGHTWINDOW *lpHighlightWindow, DWORD dwMarkID, DWORD dwColorText, DWORD dwColorBk, BOOL bFindUp)
{
  AEFINDTEXTW ft;
  AECHARRANGE crSel;
  MARKTEXT *lpSingleMarkText=NULL;
  MARKTEXT *lpMarkText;
  MARKTEXT *lpNextMarkText;
  AEMARKTEXTITEMW *lpMarkItem=NULL;
  STACKDELIM *lpDelimiterStack=NULL;
  AECHARINDEX ciCount;
  BOOL bMatched;

  if (!lpHighlightWindow->hMarkTextsStack.first)
    return FALSE;
  if (lpHighlightWindow->lpSyntaxFile)
    lpDelimiterStack=&lpHighlightWindow->lpSyntaxFile->hDelimiterStack;

  SendMessage(lpHighlightWindow->hWndEdit, AEM_EXGETSEL, (WPARAM)&crSel.ciMin, (LPARAM)&crSel.ciMax);
  if (bFindUp)
  {
    if (!AEC_PrevCharEx(&crSel.ciMin, &ciCount))
      return FALSE;
  }
  else ciCount=crSel.ciMax;

  if (dwMarkID == MARKID_SELECTION)
  {
    for (lpMarkText=lpHighlightWindow->hMarkTextsStack.first; lpMarkText; lpMarkText=lpNextMarkText)
    {
      lpNextMarkText=lpMarkText->next;
      lpMarkItem=(AEMARKTEXTITEMW *)lpMarkText->hMarkTextHandle;

      if (lpMarkText->dwMarkID != MARKID_SELECTION)
      {
        bMatched=IsMatchMark(lpMarkItem, lpDelimiterStack, lpHighlightWindow->hWndEdit, &ft, &crSel.ciMin);

        if (bMatched && !AEC_IndexCompare(&ft.crFound.ciMax, &crSel.ciMax))
        {
          dwColorText=lpMarkItem->crText;
          dwColorBk=lpMarkItem->crBk;
          dwMarkID=0;
        }
      }
    }
    if (dwMarkID) return FALSE;
  }
  if (dwMarkID != 0 && dwMarkID != MARKID_AUTOASSIGN)
  {
    if (lpSingleMarkText=StackGetMarkByColorID(lpHighlightWindow, dwMarkID, dwColorText, dwColorBk))
    {
      lpMarkItem=(AEMARKTEXTITEMW *)lpSingleMarkText->hMarkTextHandle;
      ft.dwFlags=(lpMarkItem->dwFlags & AEHLF_MATCHCASE)?FIF_MATCHCASE:0;
      ft.pText=lpMarkItem->pMarkText;
      ft.dwTextLen=lpMarkItem->nMarkTextLen;
    }
    else return FALSE;
  }

  do
  {
    if (lpSingleMarkText)
    {
      if (IsMatchMark(lpMarkItem, lpDelimiterStack, lpHighlightWindow->hWndEdit, &ft, &ciCount))
        goto Find;
    }
    else
    {
      for (lpMarkText=lpHighlightWindow->hMarkTextsStack.first; lpMarkText; lpMarkText=lpNextMarkText)
      {
        lpNextMarkText=lpMarkText->next;
        lpMarkItem=(AEMARKTEXTITEMW *)lpMarkText->hMarkTextHandle;

        if (lpMarkText->dwMarkID != MARKID_SELECTION)
        {
          if ((dwMarkID == 0 || (dwMarkID == MARKID_AUTOASSIGN && lpMarkText->dwMarkID >= MARKID_AUTOMIN && lpMarkText->dwMarkID <= MARKID_AUTOMAX)) &&
              (dwColorText == (DWORD)-1 || lpMarkItem->crText == dwColorText) &&
              (dwColorBk == (DWORD)-1 || lpMarkItem->crBk == dwColorBk))
          {
            if (IsMatchMark(lpMarkItem, lpDelimiterStack, lpHighlightWindow->hWndEdit, &ft, &ciCount))
              goto Find;
          }
        }
      }
    }
  }
  while (bFindUp?AEC_PrevChar(&ciCount):AEC_NextChar(&ciCount));

  return FALSE;

  Find:
  bFindingMark=TRUE;
  SendMessage(lpHighlightWindow->hWndEdit, AEM_EXSETSEL, (WPARAM)&ft.crFound.ciMin, (LPARAM)&ft.crFound.ciMax);
  bFindingMark=FALSE;
  return TRUE;
}

BOOL IsMatchMark(AEMARKTEXTITEMW *lpMarkItem, STACKDELIM *lpDelimiterStack, HWND hWnd, AEFINDTEXTW *ft, const AECHARINDEX *ciChar)
{
  BOOL bMatched;

  if (lpMarkItem->dwFlags & AEHLF_REGEXP)
    return (IsMatchRE(lpMarkItem->lpREGroupStack, &ft->crFound, ciChar) >= 0);
  if (lpMarkItem->dwFlags & AEHLF_WHOLEWORD)
    if (!IsDelimiterFromLeft(lpDelimiterStack, hWnd, ciChar))
      return FALSE;
  ft->dwFlags=(lpMarkItem->dwFlags & AEHLF_MATCHCASE)?FIF_MATCHCASE:0;
  ft->pText=lpMarkItem->pMarkText;
  ft->dwTextLen=lpMarkItem->nMarkTextLen;
  if (bMatched=IsMatch(ft, ciChar))
  {
    if (lpMarkItem->dwFlags & AEHLF_WHOLEWORD)
      if (!IsDelimiterFromRight(lpDelimiterStack, hWnd, &ft->crFound.ciMax))
        return FALSE;
  }
  return bMatched;
}

void CreateEditTheme(SYNTAXFILE *lpSyntaxFile, HWND hWnd)
{
  AEDELIMITEMW di;
  AEWORDITEMW wi;
  AEQUOTEITEMW qi;

  xprintfW(wszBuffer, L"HighLight::File::%s", lpSyntaxFile->wszSyntaxFileName);

  if (lpSyntaxFile->hThemeHighLight=(HANDLE)SendMessage(hWnd, AEM_HLCREATETHEMEW, 0, (LPARAM)wszBuffer))
  {
    //Transfer delimiters
    {
      DELIMITERINFO *lpElement=(DELIMITERINFO *)lpSyntaxFile->hDelimiterStack.first;

      while (lpElement)
      {
        di.nIndex=-1;
        di.pDelimiter=lpElement->wpDelimiter;
        di.nDelimiterLen=lpElement->nDelimiterLen;
        di.dwFlags=lpElement->dwFlags;
        di.crText=lpElement->dwColor1;
        di.crBk=lpElement->dwColor2;
        di.dwFontStyle=lpElement->dwFontStyle;
        di.nParentID=lpElement->nParentID;
        SendMessage(hWnd, AEM_HLADDDELIMITERW, (WPARAM)lpSyntaxFile->hThemeHighLight, (LPARAM)&di);

        lpElement=lpElement->next;
      }
    }

    //Transfer words
    {
      WORDINFO *lpElement=(WORDINFO *)lpSyntaxFile->hWordStack.last;

      while (lpElement)
      {
        wi.pWord=lpElement->wpWord;
        wi.nWordLen=lpElement->nWordLen;
        wi.dwFlags=lpElement->dwFlags;
        wi.crText=lpElement->dwColor1;
        wi.crBk=lpElement->dwColor2;
        wi.dwFontStyle=lpElement->dwFontStyle;
        wi.nParentID=lpElement->nParentID;
        SendMessage(hWnd, AEM_HLADDWORDW, (WPARAM)lpSyntaxFile->hThemeHighLight, (LPARAM)&wi);

        lpElement=lpElement->prev;
      }
    }

    //Transfer quotes
    {
      QUOTEINFO *lpElement=(QUOTEINFO *)lpSyntaxFile->hQuoteStack.first;

      while (lpElement)
      {
        qi.nIndex=-1;
        qi.pQuoteStart=lpElement->wpQuoteStart;
        qi.nQuoteStartLen=lpElement->nQuoteStartLen;
        qi.pQuoteEnd=lpElement->wpQuoteEnd;
        qi.nQuoteEndLen=lpElement->nQuoteEndLen;
        qi.chEscape=lpElement->wchEscape;
        qi.pQuoteInclude=lpElement->wpQuoteInclude;
        qi.nQuoteIncludeLen=lpElement->nQuoteIncludeLen;
        qi.pQuoteExclude=lpElement->wpQuoteExclude;
        qi.nQuoteExcludeLen=lpElement->nQuoteExcludeLen;
        qi.dwFlags=lpElement->dwFlags;
        qi.dwFontStyle=lpElement->dwFontStyle;
        qi.crText=lpElement->dwColor1;
        qi.crBk=lpElement->dwColor2;
        qi.nParentID=lpElement->nParentID;
        qi.nRuleID=lpElement->nRuleID;

        if (qi.nQuoteIncludeLen)
          qi.dwFlags|=AEHLF_QUOTEINCLUDE;
        if (qi.nQuoteExcludeLen)
          qi.dwFlags|=AEHLF_QUOTEEXCLUDE;
        if (!SendMessage(hWnd, AEM_HLADDQUOTEW, (WPARAM)lpSyntaxFile->hThemeHighLight, (LPARAM)&qi))
        {
          if (qi.dwFlags & AEHLF_REGEXP)
          {
            xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_COMPILEERROR), lpSyntaxFile->wszSyntaxFileName, qi.nQuoteStartLen, qi.pQuoteStart);
            if (MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OKCANCEL|MB_ICONEXCLAMATION) == IDCANCEL)
              break;
          }
        }
        lpElement=lpElement->next;
      }
    }
  }
}

void GetFontAndColors(SYNTAXFILE *lpSyntaxFile, HIGHLIGHTWINDOW *lpHighlightWindow)
{
  if (!lpHighlightWindow->bOldColors)
  {
    lpHighlightWindow->bOldColors=TRUE;
    lpHighlightWindow->aecNew.dwFlags=lpSyntaxFile->aecColors.dwFlags;

    if (lpHighlightWindow->aecNew.dwFlags)
    {
      //Current AECOLORS
      xmemcpy(&lpHighlightWindow->aecOld, &lpHighlightWindow->lpFrame->aec, sizeof(AECOLORS));
      //lpHighlightWindow->aecOld.dwFlags=AECLR_ALL;

      //New AECOLORS
      xmemcpy(&lpHighlightWindow->aecNew, &lpSyntaxFile->aecColors, sizeof(AECOLORS));
    }
  }
  if (!lpHighlightWindow->bOldFont)
  {
    lpHighlightWindow->bOldFont=TRUE;
    lpHighlightWindow->dwFontFlags=lpSyntaxFile->dwFontFlags;

    if (lpHighlightWindow->dwFontFlags)
    {
      HFONT hFontOld;

      hFontOld=(HFONT)SendMessage(lpHighlightWindow->hWndEdit, WM_GETFONT, 0, 0);

      //Current LOGFONTW
      if (bOldWindows)
      {
        LOGFONTA lfA;

        GetObjectA(hFontOld, sizeof(LOGFONTA), &lfA);
        LogFontAtoW(&lfA, &lpHighlightWindow->lfFontOld);
      }
      else GetObjectW(hFontOld, sizeof(LOGFONTW), &lpHighlightWindow->lfFontOld);

      //New LOGFONTW
      xmemcpy(&lpHighlightWindow->lfFontNew, &lpSyntaxFile->lfFont, sizeof(LOGFONTW));

      if (!(lpHighlightWindow->dwFontFlags & FF_FONTSTYLE))
      {
        lpHighlightWindow->lfFontNew.lfWeight=lpHighlightWindow->lfFontOld.lfWeight;
        lpHighlightWindow->lfFontNew.lfItalic=lpHighlightWindow->lfFontOld.lfItalic;
      }
      if (!(lpHighlightWindow->dwFontFlags & FF_FONTSIZE))
      {
        lpHighlightWindow->lfFontNew.lfHeight=-mod(lpHighlightWindow->lfFontOld.lfHeight);
      }
      if (!(lpHighlightWindow->dwFontFlags & FF_FONTFACE))
      {
        xstrcpynW(lpHighlightWindow->lfFontNew.lfFaceName, lpHighlightWindow->lfFontOld.lfFaceName, LF_FACESIZE);
      }
    }
  }
  if (!lpHighlightWindow->bOldBkImage)
  {
    lpHighlightWindow->bOldBkImage=TRUE;
    xstrcpynW(lpHighlightWindow->wszBkImageFileOld, lpHighlightWindow->lpFrame->wszBkImageFile, MAX_PATH);
    xstrcpynW(lpHighlightWindow->wszBkImageFileNew, lpSyntaxFile->wszBkImageFile, MAX_PATH);
    lpHighlightWindow->nBkImageAlphaOld=lpHighlightWindow->lpFrame->nBkImageAlpha;
    lpHighlightWindow->nBkImageAlphaNew=lpSyntaxFile->nBkImageAlpha;
  }
}

void RestoreFontAndColors(HIGHLIGHTWINDOW *lpHighlightWindow)
{
  DWORD dwLockInherit=lpHighlightWindow->lpFrame->dwLockInherit;

  if (lpHighlightWindow->bOldColors)
  {
    lpHighlightWindow->bOldColors=FALSE;

    if (lpHighlightWindow->aecNew.dwFlags)
    {
      if (lpHighlightWindow->lpFrame->dwLockInherit & LI_COLORS)
      {
        lpHighlightWindow->aecOld.dwFlags=GetColorsToRestore(lpHighlightWindow, &lpHighlightWindow->aecNew);
        SetFrameInfo(lpHighlightWindow->lpFrame, FIS_COLORS, (UINT_PTR)&lpHighlightWindow->aecOld);
        dwLockInherit&=~LI_COLORS;
      }
    }
  }
  if (lpHighlightWindow->bOldFont)
  {
    lpHighlightWindow->bOldFont=FALSE;

    if (lpHighlightWindow->dwFontFlags)
    {
      if (lpHighlightWindow->lpFrame->dwLockInherit & LI_FONT)
      {
        SendMessage(hMainWnd, AKD_SETFONTW, (WPARAM)lpHighlightWindow->hWndEdit, (LPARAM)&lpHighlightWindow->lfFontOld);
        dwLockInherit&=~LI_FONT;
      }
    }
  }
  if (lpHighlightWindow->bOldBkImage)
  {
    lpHighlightWindow->bOldBkImage=FALSE;

    if (lpHighlightWindow->lpFrame->dwLockInherit & LI_BKIMAGE)
    {
      BKIMAGE bki;

      bki.wpFile=lpHighlightWindow->wszBkImageFileOld;
      bki.nAlpha=lpHighlightWindow->nBkImageAlphaOld;
      SetFrameInfo(lpHighlightWindow->lpFrame, FIS_BKIMAGE, (UINT_PTR)&bki);
      dwLockInherit&=~LI_BKIMAGE;
    }
  }

  if (dwLockInherit != lpHighlightWindow->lpFrame->dwLockInherit)
    SetFrameInfo(lpHighlightWindow->lpFrame, FIS_LOCKINHERIT, dwLockInherit);
}

LOGFONTW* LogFontAtoW(const LOGFONTA *lfA, LOGFONTW *lfW)
{
  xmemcpy(lfW, lfA, offsetof(LOGFONTW, lfFaceName));
  MultiByteToWideChar(CP_ACP, 0, lfA->lfFaceName, -1, lfW->lfFaceName, LF_FACESIZE);
  return lfW;
}

LOGFONTA* LogFontWtoA(const LOGFONTW *lfW, LOGFONTA *lfA)
{
  xmemcpy(lfA, lfW, offsetof(LOGFONTW, lfFaceName));
  WideCharToMultiByte(CP_ACP, 0, lfW->lfFaceName, -1, lfA->lfFaceName, LF_FACESIZE, NULL, NULL);
  return lfA;
}

DWORD GetColorsToRestore(HIGHLIGHTWINDOW *lpHighlightWindow, AECOLORS *aecHighlight)
{
  AECOLORS aecCurrent;
  DWORD dwFlags=aecHighlight->dwFlags;

  //Get current colors
  xmemcpy(&aecCurrent, &lpHighlightWindow->lpFrame->aec, sizeof(AECOLORS));

  //Get changed colors
  if (dwFlags & AECLR_CARET)
  {
    if (aecHighlight->crCaret != aecCurrent.crCaret)
      dwFlags&=~AECLR_CARET;
  }
  if (dwFlags & AECLR_BASICTEXT)
  {
    if (aecHighlight->crBasicText != aecCurrent.crBasicText)
      dwFlags&=~AECLR_BASICTEXT;
  }
  if (dwFlags & AECLR_BASICBK)
  {
    if (aecHighlight->crBasicBk != aecCurrent.crBasicBk)
      dwFlags&=~AECLR_BASICBK;
  }
  if (dwFlags & AECLR_SELTEXT)
  {
    if (aecHighlight->crSelText != aecCurrent.crSelText)
      dwFlags&=~AECLR_SELTEXT;
  }
  if (dwFlags & AECLR_SELBK)
  {
    if (aecHighlight->crSelBk != aecCurrent.crSelBk)
      dwFlags&=~AECLR_SELBK;
  }
  if (dwFlags & AECLR_ACTIVELINETEXT)
  {
    if (aecHighlight->crActiveLineText != aecCurrent.crActiveLineText)
      dwFlags&=~AECLR_ACTIVELINETEXT;
  }
  if (dwFlags & AECLR_ACTIVELINEBK)
  {
    if (aecHighlight->crActiveLineBk != aecCurrent.crActiveLineBk)
      dwFlags&=~AECLR_ACTIVELINEBK;
  }
  if (dwFlags & AECLR_URLTEXT)
  {
    if (aecHighlight->crUrlText != aecCurrent.crUrlText)
      dwFlags&=~AECLR_URLTEXT;
  }
  if (dwFlags & AECLR_ACTIVECOLUMN)
  {
    if (aecHighlight->crActiveColumn != aecCurrent.crActiveColumn)
      dwFlags&=~AECLR_ACTIVECOLUMN;
  }
  if (dwFlags & AECLR_COLUMNMARKER)
  {
    if (aecHighlight->crColumnMarker != aecCurrent.crColumnMarker)
      dwFlags&=~AECLR_COLUMNMARKER;
  }
  return dwFlags;
}

void UnassignTheme(HWND hWnd)
{
  HANDLE hThemeHighLight;
  wchar_t wszThemeName[MAX_PATH];

  if (hThemeHighLight=(HANDLE)SendMessage(hWnd, AEM_HLFINDTHEME, AEHLFT_CURRENT, 0))
  {
    if (SendMessage(hWnd, AEM_HLGETTHEMENAMEW, (WPARAM)hThemeHighLight, (LPARAM)wszThemeName))
    {
      if (!xstrcmpnW(L"HighLight::File::", wszThemeName, (UINT_PTR)-1))
      {
        SendMessage(hWnd, AEM_HLSETTHEME, (WPARAM)NULL, FALSE);
      }
    }
  }
}

COLORREF GetColorFromStrAnsi(const char *pColor, const char **pNext)
{
  wchar_t wszColor[6];
  const wchar_t *wpNext=wszColor;
  COLORREF crColor;

  MultiByteToWideChar(CP_ACP, 0, (char *)pColor, 6, wszColor, 6);
  crColor=GetColorFromStr(wszColor, &wpNext);
  if (pNext) *pNext=pColor + (wpNext - wszColor);
  return crColor;
}

COLORREF GetColorFromStr(const wchar_t *wpColor, const wchar_t **wpNext)
{
  COLORREF crColor;

  if ((crColor=(COLORREF)hex2decW(wpColor, 6, NULL)) != (DWORD)-1)
  {
    //RRGGBB
    if (wpNext) *wpNext=wpColor + 6;
    crColor=RGB(GetBValue(crColor), GetGValue(crColor), GetRValue(crColor));
  }
  else
  {
    //RGB->RRGGBB
    wchar_t wszRBG[6];
    wchar_t *wpRBG=wszRBG;
    wchar_t *wpMaxRBG=wszRBG + 6;

    while (wpRBG < wpMaxRBG)
    {
      if ((*wpColor >= '0' && *wpColor <= '9') ||
          (*wpColor >= 'a' && *wpColor <= 'f') ||
          (*wpColor >= 'A' && *wpColor <= 'F'))
      {
        *wpRBG++=*wpColor;
        *wpRBG++=*wpColor++;
      }
      else return (COLORREF)-1;
    }
    if (wpNext) *wpNext=wpColor;
    if ((crColor=(COLORREF)hex2decW(wszRBG, 6, NULL)) != (COLORREF)-1)
      crColor=RGB(GetBValue(crColor), GetGValue(crColor), GetRValue(crColor));
  }
  return crColor;
}

int GetStrFromColor(COLORREF crColor, wchar_t *wszColor)
{
  return (int)xprintfW(wszColor, L"%02X%02X%02X", GetRValue(crColor), GetGValue(crColor), GetBValue(crColor));
}

BOOL SetFrameInfo(FRAMEDATA *lpFrame, int nType, UINT_PTR dwData)
{
  FRAMEINFO fi;

  fi.nType=nType;
  fi.dwData=dwData;
  return (BOOL)SendMessage(hMainWnd, AKD_SETFRAMEINFO, (WPARAM)&fi, (LPARAM)lpFrame);
}


//// Options

void ReadHighLightOptions(HANDLE hOptions)
{
  WideOption(hOptions, L"IgnoreFontStyle", PO_DWORD, (LPBYTE)&dwIgnoreFontStyle, sizeof(DWORD));
  WideOption(hOptions, L"AutoMarkEnable", PO_DWORD, (LPBYTE)&bAutoMarkEnable, sizeof(DWORD));
  WideOption(hOptions, L"AutoMarkType", PO_DWORD, (LPBYTE)&nAutoMarkType, sizeof(DWORD));
  WideOption(hOptions, L"AutoMarkMaxSel", PO_DWORD, (LPBYTE)&nAutoMarkMaxSel, sizeof(DWORD));
}

void SaveHighLightOptions(HANDLE hOptions, DWORD dwFlags)
{
  if (dwFlags & OF_HIGHLIGHT_SETTINGS)
  {
    WideOption(hOptions, L"IgnoreFontStyle", PO_DWORD, (LPBYTE)&dwIgnoreFontStyle, sizeof(DWORD));
    WideOption(hOptions, L"AutoMarkEnable", PO_DWORD, (LPBYTE)&bAutoMarkEnable, sizeof(DWORD));
    WideOption(hOptions, L"AutoMarkType", PO_DWORD, (LPBYTE)&nAutoMarkType, sizeof(DWORD));
    WideOption(hOptions, L"AutoMarkMaxSel", PO_DWORD, (LPBYTE)&nAutoMarkMaxSel, sizeof(DWORD));
  }
}

void InitHighLight()
{
  bInitHighLight=TRUE;
}

void UninitHighLight()
{
  bInitHighLight=FALSE;

  StackFreeHighLightWindow(&hHighLightWindowsStack);
}
