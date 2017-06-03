#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include <commctrl.h>
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


//// Global variables
SYNTAXFILE *lpSyntaxFileAutoComplete=NULL;
STACKDOCWORDS hDocWordsStack={0};
BLOCKINFO *lpCurrentBlockElement=NULL;
INT_PTR nWindowBlockBegin=0;
INT_PTR nWindowBlockEnd=0;
RECT rcAutoComplete={0, 0, 140, 102};
HWND hWndAutoComplete=NULL;
HWND hWndListBox=NULL;
WNDPROC OldListBoxProc=NULL;
HHOOK hHook=NULL;
BOOL bAutoListEnable=TRUE;
int nAutoListAfter=2;
BOOL bAlphaListEnable=FALSE;
int nAlphaListValue=220;
int nCompleteWithoutListAfter=2;
BOOL bOneWithoutListAndCompleteNext=TRUE;
DWORD dwCompleteWithList=544;  //"Ctrl+Space"
DWORD dwCompleteWithoutList=9; //"Ctrl+Space"
DWORD dwCompleteNext=0;        //"None"
DWORD dwCompletePrev=0;        //"None"
BOOL bAddDocumentWords=TRUE;
BOOL bCompleteNonSyntaxDocument=TRUE;
BOOL bSaveTypedCase=FALSE;
BOOL bSaveTypedCaseOnce=-1;
BOOL bInheritTypedCase=TRUE;
BOOL bMaxDocumentEnable=TRUE;
int nMaxDocumentChars=1000000;
BOOL bAddHighLightWords=TRUE;
BOOL bCompleteListItemHlBaseColors=TRUE;
BOOL bCompleteListSystemColors=FALSE;
DWORD dwCompleteListSymbolMark=CLSM_MARK;
BOOL bRightDelimitersEnable=FALSE;
BOOL bSyntaxDelimitersEnable=TRUE;
BOOL bCompleteCaseSensitive=TRUE;
BOOL bListShowOnlyMatched=TRUE;
BOOL bLockAutoList=FALSE;
BOOL bChangingText=FALSE;
BOOL bCompletingTitle=FALSE;
BOOL bFirstListBoxKey;
BLOCKINFO biHighLightBlock={0};
BLOCKINFO biDocWordBlock={0};
wchar_t wszHighLightTitle[MAX_PATH];
wchar_t wszDocWordTitle[MAX_PATH];
PLUGINFUNCTION *pfwCompleteWithList=NULL;
PLUGINFUNCTION *pfwCompleteWithoutList=NULL;
PLUGINFUNCTION *pfwCompleteNext=NULL;
PLUGINFUNCTION *pfwCompletePrev=NULL;
BOOL (WINAPI *SetLayeredWindowAttributesPtr)(HWND, COLORREF, BYTE, DWORD);


//Plugin extern function
void __declspec(dllexport) AutoComplete(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon)
    InitCommon(pd);
  else if (!bInitAutoComplete)
    ReadOptions(OF_AUTOCOMPLETE);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (bInitAutoComplete)
    {
      if (nAction == DLLA_AUTOCOMPLETE_ADDWINDOW)
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

            lpManual->dwDllFunction|=CODER_AUTOCOMPLETE;
          }
        }
      }
      else if (nAction == DLLA_AUTOCOMPLETE_DELWINDOW)
      {
        MANUALSET *lpManual;
        HWND hWndEdit=NULL;

        if (IsExtCallParamValid(pd->lParam, 2))
          hWndEdit=(HWND)GetExtCallParam(pd->lParam, 2);

        if (hWndEdit)
        {
          if (lpManual=StackGetManual(&hManualStack, hWndEdit, NULL))
            StackDeleteManual(&hManualStack, lpManual, CODER_AUTOCOMPLETE);
        }
      }
    }

    //If plugin already loaded, stay in memory and don't change active status
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
    return;
  }

  if (bInitAutoComplete)
  {
    UninitMain();
    UninitAutoComplete();

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();
    InitAutoComplete();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

BOOL CALLBACK AutoCompleteSetup1DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndAutoListEnable;
  static HWND hWndAutoListPreLabel;
  static HWND hWndAutoListPostLabel;
  static HWND hWndAutoListAfter;
  static HWND hWndAutoListAfterSpin;
  static HWND hWndAlphaListEnable;
  static HWND hWndAlphaListLabel;
  static HWND hWndAlphaListValue;
  static HWND hWndAlphaListValueSpin;
  static HWND hWndCompleteWithList;
  static HWND hWndCompleteWithoutList;
  static HWND hWndCompleteWithoutListAfter;
  static HWND hWndCompleteWithoutListAfterSpin;
  static HWND hWndCompleteNextCheck;
  static HWND hWndCompleteNext;
  static HWND hWndCompletePrev;
  static BOOL bAutoListEnableDlg;
  static BOOL bAlphaListEnableDlg;
  static BOOL bOneWithoutListAndCompleteNextDlg;
  static BOOL bInitDialog;
  static BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    bInitDialog=TRUE;
    bAutoListEnableDlg=bAutoListEnable;
    bAlphaListEnableDlg=bAlphaListEnable;
    bOneWithoutListAndCompleteNextDlg=bOneWithoutListAndCompleteNext;
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndAutoListEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLIST_ENABLE);
    hWndAutoListPreLabel=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER_PRELABEL);
    hWndAutoListPostLabel=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER_POSTLABEL);
    hWndAutoListAfter=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER);
    hWndAutoListAfterSpin=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER_SPIN);
    hWndAlphaListEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALIST_ENABLE);
    hWndAlphaListLabel=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALISTVALUE_LABEL);
    hWndAlphaListValue=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALISTVALUE);
    hWndAlphaListValueSpin=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALISTVALUE_SPIN);
    hWndCompleteWithList=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHLIST);
    hWndCompleteWithoutList=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLIST);
    hWndCompleteWithoutListAfter=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLISTAFTER);
    hWndCompleteWithoutListAfterSpin=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLISTAFTER_SPIN);
    hWndCompleteNextCheck=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETENEXT_CHECK);
    hWndCompleteNext=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETENEXT);
    hWndCompletePrev=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEPREV);

    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLIST_GROUP, GetLangStringW(wLangModule, STRID_AUTOLIST));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLIST_ENABLE, GetLangStringW(wLangModule, STRID_ENABLE));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER_PRELABEL, GetLangStringW(wLangModule, STRID_AUTOLIST_PRE));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER_POSTLABEL, GetLangStringW(wLangModule, STRID_AFTERCHAR_POST));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALIST_GROUP, GetLangStringW(wLangModule, STRID_TRANSPARENCY));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALIST_ENABLE, GetLangStringW(wLangModule, STRID_ENABLE));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALISTVALUE_LABEL, GetLangStringW(wLangModule, STRID_ALPHA));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYGROUP, GetLangStringW(wLangModule, STRID_HOTKEYS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHLIST_LABEL, GetLangStringW(wLangModule, STRID_COMPLETEWITHLIST));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLIST_LABEL, GetLangStringW(wLangModule, STRID_COMPLETEWITHOUTLIST));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLISTAFTER_PRELABEL, GetLangStringW(wLangModule, STRID_COMPLETEWITHOUTLIST_PRE));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLISTAFTER_POSTLABEL, GetLangStringW(wLangModule, STRID_AFTERCHAR_POST));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETENEXT_CHECK, GetLangStringW(wLangModule, STRID_COMPLETENEXT));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEPREV_LABEL, GetLangStringW(wLangModule, STRID_COMPLETEPREV));

    if (bAutoListEnableDlg) SendMessage(hWndAutoListEnable, BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(hWndAutoListAfterSpin, UDM_SETBUDDY, (WPARAM)hWndAutoListAfter, 0);
    SendMessage(hWndAutoListAfterSpin, UDM_SETRANGE, 0, MAKELONG(99, 1));
    SetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER, nAutoListAfter, FALSE);

    if (bAlphaListEnableDlg) SendMessage(hWndAlphaListEnable, BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(hWndAlphaListValueSpin, UDM_SETBUDDY, (WPARAM)hWndAlphaListValue, 0);
    SendMessage(hWndAlphaListValueSpin, UDM_SETRANGE, 0, MAKELONG(255, 0));
    SetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALISTVALUE, nAlphaListValue, FALSE);

    if (bOneWithoutListAndCompleteNextDlg) SendMessage(hWndCompleteNextCheck, BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(hWndCompleteWithoutListAfterSpin, UDM_SETBUDDY, (WPARAM)hWndCompleteWithoutListAfter, 0);
    SendMessage(hWndCompleteWithoutListAfterSpin, UDM_SETRANGE, 0, MAKELONG(99, 1));
    SetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLISTAFTER, nCompleteWithoutListAfter, FALSE);

    SendMessage(hWndCompleteWithList, HKM_SETHOTKEY, dwCompleteWithList, 0);
    SendMessage(hWndCompleteWithoutList, HKM_SETHOTKEY, dwCompleteWithoutList, 0);
    SendMessage(hWndCompletePrev, HKM_SETHOTKEY, dwCompletePrev, 0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCompleteWithList, pfwCompleteWithList?dwCompleteWithList:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCompleteWithoutList, pfwCompleteWithoutList?dwCompleteWithoutList:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCompleteNext, pfwCompleteNext?dwCompleteNext:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndCompletePrev, pfwCompletePrev?dwCompletePrev:0);

    SendMessage(hDlg, WM_COMMAND, IDC_AUTOCOMPLETE_SETUP_AUTOLIST_ENABLE, 0);
    SendMessage(hDlg, WM_COMMAND, IDC_AUTOCOMPLETE_SETUP_ALPHALIST_ENABLE, 0);
    SendMessage(hDlg, WM_COMMAND, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETENEXT_CHECK, 0);
    bInitDialog=FALSE;
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
      }
    }
    else if (LOWORD(wParam) >= IDC_AUTOCOMPLETE_SETUP_AUTOLIST_GROUP)
    {
      if (!bInitDialog)
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }

    if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_AUTOLIST_ENABLE)
    {
      bAutoListEnableDlg=(BOOL)SendMessage(hWndAutoListEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndAutoListAfter, bAutoListEnableDlg);
      EnableWindow(hWndAutoListAfterSpin, bAutoListEnableDlg);
      EnableWindow(hWndAutoListPreLabel, bAutoListEnableDlg);
      EnableWindow(hWndAutoListPostLabel, bAutoListEnableDlg);
    }
    else if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_ALPHALIST_ENABLE)
    {
      bAlphaListEnableDlg=(BOOL)SendMessage(hWndAlphaListEnable, BM_GETCHECK, 0, 0);
      bState=bAlphaListEnableDlg;
      if (!SetLayeredWindowAttributesPtr)
      {
        bState=FALSE;
        EnableWindow(hWndAlphaListEnable, FALSE);
      }
      EnableWindow(hWndAlphaListValue, bState);
      EnableWindow(hWndAlphaListValueSpin, bState);
      EnableWindow(hWndAlphaListLabel, bState);
    }
    else if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLIST)
    {
      WORD dwHotkey;

      if (bOneWithoutListAndCompleteNextDlg)
      {
        dwHotkey=(WORD)SendMessage(hWndCompleteWithoutList, HKM_GETHOTKEY, 0, 0);
        SendMessage(hWndCompleteNext, HKM_SETHOTKEY, dwHotkey, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETENEXT_CHECK)
    {
      bOneWithoutListAndCompleteNextDlg=(BOOL)SendMessage(hWndCompleteNextCheck, BM_GETCHECK, 0, 0);

      if (bOneWithoutListAndCompleteNextDlg)
        SendMessage(hWndCompleteNext, HKM_SETHOTKEY, dwCompleteWithoutList, 0);
      else
        SendMessage(hWndCompleteNext, HKM_SETHOTKEY, dwCompleteNext, 0);
      EnableWindow(hWndCompleteNext, !bOneWithoutListAndCompleteNextDlg);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_SETACTIVE)
    {
      if (nPropMaxVisitPage < PAGE_AUTOCOMPLETE1)
        nPropMaxVisitPage=PAGE_AUTOCOMPLETE1;
    }
    else if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      PSHNOTIFY *pshn=(PSHNOTIFY *)lParam;

      bAutoListEnable=bAutoListEnableDlg;
      nAutoListAfter=GetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_AUTOLISTAFTER, NULL, FALSE);

      bAlphaListEnable=bAlphaListEnableDlg;
      nAlphaListValue=GetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_ALPHALISTVALUE, NULL, FALSE);
      if (nAlphaListValue < 0) nAlphaListValue=0;
      if (nAlphaListValue > 255) nAlphaListValue=255;
      if (nAlphaListValue == 0 || nAlphaListValue == 255) bAlphaListEnable=FALSE;

      nCompleteWithoutListAfter=GetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_HOTKEYCOMPLETEWITHOUTLISTAFTER, NULL, FALSE);

      dwCompleteWithList=(WORD)SendMessage(hWndCompleteWithList, HKM_GETHOTKEY, 0, 0);
      dwCompleteWithoutList=(WORD)SendMessage(hWndCompleteWithoutList, HKM_GETHOTKEY, 0, 0);
      bOneWithoutListAndCompleteNext=bOneWithoutListAndCompleteNextDlg;
      if (!bOneWithoutListAndCompleteNext)
        dwCompleteNext=(WORD)SendMessage(hWndCompleteNext, HKM_GETHOTKEY, 0, 0);
      dwCompletePrev=(WORD)SendMessage(hWndCompletePrev, HKM_GETHOTKEY, 0, 0);

      if (bInitAutoComplete)
      {
        UninitAutoComplete();
        InitAutoComplete();
      }

      if (pshn->lParam)
      {
        //OK button pressed
        bSaveOptions=TRUE;
      }
      else
      {
        //Apply button pressed
        if (nPropMaxVisitPage == PAGE_AUTOCOMPLETE1)
          UpdateAllOptions();
        return FALSE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK AutoCompleteSetup2DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndAddDocumentWords;
  static HWND hWndCompleteNonSyntaxDocument;
  static HWND hWndSaveTypedCase;
  static HWND hWndInheritTypedCase;
  static HWND hWndMaxDocumentEnable;
  static HWND hWndMaxDocumentChars;
  static HWND hWndMaxDocumentPostLabel;
  static HWND hWndAddHighLightWords;
  static HWND hWndListItemHlBaseColorsEnable;
  static HWND hWndListSysColorsEnable;
  static HWND hWndListNoSymbolMarkEnable;
  static HWND hWndRightDelimitersEnable;
  static HWND hWndSyntaxDelimitersEnable;
  static HWND hWndCaseSensitiveEnable;
  static BOOL bAddDocumentWordsDlg;
  static BOOL bMaxDocumentEnableDlg;
  static BOOL bCompleteNonSyntaxDocumentDlg;
  static BOOL bSaveTypedCaseDlg;
  static BOOL bAddHighLightWordsDlg;
  static BOOL bCompleteListSystemColorsDlg;
  static BOOL bInitDialog;

  if (uMsg == WM_INITDIALOG)
  {
    bInitDialog=TRUE;
    bAddDocumentWordsDlg=bAddDocumentWords;
    bMaxDocumentEnableDlg=bMaxDocumentEnable;
    bCompleteNonSyntaxDocumentDlg=bCompleteNonSyntaxDocument;
    bSaveTypedCaseDlg=bSaveTypedCase;
    bAddHighLightWordsDlg=bAddHighLightWords;
    bCompleteListSystemColorsDlg=bCompleteListSystemColors;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndAddDocumentWords=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_ADDDOCUMENTWORDS);
    hWndCompleteNonSyntaxDocument=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_COMPLETENONSYNTAXDOCUMENT);
    hWndSaveTypedCase=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_SAVETYPEDCASE);
    hWndInheritTypedCase=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_INHERITTYPEDCASE);
    hWndMaxDocumentEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_ENABLE);
    hWndMaxDocumentChars=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_CHARS);
    hWndMaxDocumentPostLabel=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_POSTLABEL);
    hWndAddHighLightWords=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_ADDHIGHLIGHTWORDS);
    hWndListItemHlBaseColorsEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_LISTITEMHLBASECOLORS_ENABLE);
    hWndListSysColorsEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_LISTSYSCOLORS_ENABLE);
    hWndListNoSymbolMarkEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_LISTNOSYMBOLMARK_ENABLE);
    hWndRightDelimitersEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_RIGHTDELIMITERS_ENABLE);
    hWndSyntaxDelimitersEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_SYNTAXDELIMITERS_ENABLE);
    hWndCaseSensitiveEnable=GetDlgItem(hDlg, IDC_AUTOCOMPLETE_SETUP_CASESENSITIVE_ENABLE);

    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_DOCUMENTWORDS_GROUP, GetLangStringW(wLangModule, STRID_DOCUMENT));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_ADDDOCUMENTWORDS, GetLangStringW(wLangModule, STRID_ADDDOCUMENTWORDS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_COMPLETENONSYNTAXDOCUMENT, GetLangStringW(wLangModule, STRID_COMPLETENONSYNTAXDOCUMENT));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_SAVETYPEDCASE, GetLangStringW(wLangModule, STRID_SAVETYPEDCASE));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_INHERITTYPEDCASE, GetLangStringW(wLangModule, STRID_INHERITTYPEDCASE));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_ENABLE, GetLangStringW(wLangModule, STRID_MAXDOCUMENT));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_POSTLABEL, GetLangStringW(wLangModule, STRID_CHARS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_ADDHIGHLIGHTWORDS, GetLangStringW(wLangModule, STRID_ADDHIGHLIGHTWORDS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_LISTITEMHLBASECOLORS_ENABLE, GetLangStringW(wLangModule, STRID_HLBASECOLORS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_LISTSYSCOLORS_ENABLE, GetLangStringW(wLangModule, STRID_LISTSYSTEMCOLORS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_LISTNOSYMBOLMARK_ENABLE, GetLangStringW(wLangModule, STRID_NOSYMBOLMARK));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_RIGHTDELIMITERS_ENABLE, GetLangStringW(wLangModule, STRID_RIGHTDELIMITERS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_SYNTAXDELIMITERS_ENABLE, GetLangStringW(wLangModule, STRID_SYNTAXDELIMITERS));
    SetDlgItemTextWide(hDlg, IDC_AUTOCOMPLETE_SETUP_CASESENSITIVE_ENABLE, GetLangStringW(wLangModule, STRID_COMPLETECASESENSITIVE));

    if (bAddDocumentWords) SendMessage(hWndAddDocumentWords, BM_SETCHECK, BST_CHECKED, 0);
    if (bMaxDocumentEnable) SendMessage(hWndMaxDocumentEnable, BM_SETCHECK, BST_CHECKED, 0);
    SetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_CHARS, nMaxDocumentChars, FALSE);
    if (bCompleteNonSyntaxDocument) SendMessage(hWndCompleteNonSyntaxDocument, BM_SETCHECK, BST_CHECKED, 0);
    if (bSaveTypedCase) SendMessage(hWndSaveTypedCase, BM_SETCHECK, BST_CHECKED, 0);
    if (bInheritTypedCase) SendMessage(hWndInheritTypedCase, BM_SETCHECK, BST_CHECKED, 0);

    if (bAddHighLightWords) SendMessage(hWndAddHighLightWords, BM_SETCHECK, BST_CHECKED, 0);
    if (bCompleteListItemHlBaseColors) SendMessage(hWndListItemHlBaseColorsEnable, BM_SETCHECK, BST_CHECKED, 0);
    if (bCompleteListSystemColors) SendMessage(hWndListSysColorsEnable, BM_SETCHECK, BST_CHECKED, 0);
    if (!(dwCompleteListSymbolMark & CLSM_MARK)) SendMessage(hWndListNoSymbolMarkEnable, BM_SETCHECK, BST_CHECKED, 0);
    if (bRightDelimitersEnable) SendMessage(hWndRightDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    if (bSyntaxDelimitersEnable) SendMessage(hWndSyntaxDelimitersEnable, BM_SETCHECK, BST_CHECKED, 0);
    if (bCompleteCaseSensitive) SendMessage(hWndCaseSensitiveEnable, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hDlg, WM_COMMAND, IDC_AUTOCOMPLETE_SETUP_ADDDOCUMENTWORDS, 0);
    SendMessage(hDlg, WM_COMMAND, IDC_AUTOCOMPLETE_SETUP_ADDHIGHLIGHTWORDS, 0);
    SendMessage(hDlg, WM_COMMAND, IDC_AUTOCOMPLETE_SETUP_SAVETYPEDCASE, 0);
    bInitDialog=FALSE;
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_CHARS)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
      }
    }
    else if (LOWORD(wParam) <= IDC_AUTOCOMPLETE_SETUP_CASESENSITIVE_ENABLE)
    {
      if (!bInitDialog)
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }

    if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_ADDDOCUMENTWORDS ||
        LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_ENABLE ||
        LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_COMPLETENONSYNTAXDOCUMENT)
    {
      bAddDocumentWordsDlg=(BOOL)SendMessage(hWndAddDocumentWords, BM_GETCHECK, 0, 0);
      bMaxDocumentEnableDlg=(BOOL)SendMessage(hWndMaxDocumentEnable, BM_GETCHECK, 0, 0);
      bCompleteNonSyntaxDocumentDlg=(BOOL)SendMessage(hWndCompleteNonSyntaxDocument, BM_GETCHECK, 0, 0);

      EnableWindow(hWndMaxDocumentEnable, bAddDocumentWordsDlg);
      EnableWindow(hWndMaxDocumentChars, bAddDocumentWordsDlg && bMaxDocumentEnableDlg);
      EnableWindow(hWndMaxDocumentPostLabel, bAddDocumentWordsDlg && bMaxDocumentEnableDlg);
      EnableWindow(hWndCompleteNonSyntaxDocument, bAddDocumentWordsDlg);
    }
    else if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_ADDHIGHLIGHTWORDS ||
             LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_LISTSYSCOLORS_ENABLE)
    {
      bAddHighLightWordsDlg=(BOOL)SendMessage(hWndAddHighLightWords, BM_GETCHECK, 0, 0);
      bCompleteListSystemColorsDlg=(BOOL)SendMessage(hWndListSysColorsEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndListItemHlBaseColorsEnable, bAddHighLightWordsDlg && !bCompleteListSystemColorsDlg);
    }
    else if (LOWORD(wParam) == IDC_AUTOCOMPLETE_SETUP_SAVETYPEDCASE)
    {
      bSaveTypedCaseDlg=(BOOL)SendMessage(hWndSaveTypedCase, BM_GETCHECK, 0, 0);
      EnableWindow(hWndInheritTypedCase, bSaveTypedCaseDlg);
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_SETACTIVE)
    {
      if (nPropMaxVisitPage < PAGE_AUTOCOMPLETE2)
        nPropMaxVisitPage=PAGE_AUTOCOMPLETE2;
    }
    else if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      PSHNOTIFY *pshn=(PSHNOTIFY *)lParam;
      BOOL bState;

      bAddDocumentWords=bAddDocumentWordsDlg;
      bMaxDocumentEnable=bMaxDocumentEnableDlg;
      bCompleteNonSyntaxDocument=bCompleteNonSyntaxDocumentDlg;
      bSaveTypedCase=bSaveTypedCaseDlg;
      nMaxDocumentChars=GetDlgItemInt(hDlg, IDC_AUTOCOMPLETE_SETUP_MAXDOCUMENT_CHARS, NULL, FALSE);
      bInheritTypedCase=(BOOL)SendMessage(hWndInheritTypedCase, BM_GETCHECK, 0, 0);

      bAddHighLightWords=bAddHighLightWordsDlg;
      bCompleteListItemHlBaseColors=(BOOL)SendMessage(hWndListItemHlBaseColorsEnable, BM_GETCHECK, 0, 0);
      bCompleteListSystemColors=bCompleteListSystemColorsDlg;
      bState=(BOOL)SendMessage(hWndListNoSymbolMarkEnable, BM_GETCHECK, 0, 0);
      if (bState != !(dwCompleteListSymbolMark & CLSM_MARK))
      {
        if (bState)
          dwCompleteListSymbolMark=CLSM_NOMARKIFICON;
        else
          dwCompleteListSymbolMark=CLSM_MARK;
      }
      bRightDelimitersEnable=(BOOL)SendMessage(hWndRightDelimitersEnable, BM_GETCHECK, 0, 0);
      bSyntaxDelimitersEnable=(BOOL)SendMessage(hWndSyntaxDelimitersEnable, BM_GETCHECK, 0, 0);
      bCompleteCaseSensitive=(BOOL)SendMessage(hWndCaseSensitiveEnable, BM_GETCHECK, 0, 0);

      if (bInitAutoComplete)
      {
        UninitAutoComplete();
        InitAutoComplete();
      }

      if (pshn->lParam)
      {
        //OK button pressed
        bSaveOptions=TRUE;
      }
      else
      {
        //Apply button pressed
        if (nPropMaxVisitPage == PAGE_AUTOCOMPLETE2)
          UpdateAllOptions();
        return FALSE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK AutoCompleteParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  static BOOL bTextTypeChar;

  if (uMsg == AKDN_FRAME_ACTIVATE)
  {
    if (hWndAutoComplete)
      SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);

    if (lpCurrentBlockElement)
    {
      StackResetHotSpot(lpCurrentBlockElement);
      lpCurrentBlockElement=NULL;
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == ID_EDIT || (HWND)*lResult)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        if (GetFocus() == (HWND)lParam)
        {
          CHARRANGE64 cr;

          hWndEdit=(HWND)lParam;
          SendMessage(hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);

          if (bTextTypeChar)
          {
            if (hWndAutoComplete)
            {
              BLOCKINFO *lpBlockInfo=NULL;
              wchar_t wszTitlePart[MAX_PATH];
              int nIndex;
              int nTitlePartLen;
              BOOL bClose=TRUE;
              BOOL bEmpty=FALSE;
              BOOL bAddChar=FALSE;
              BOOL bDelChar=FALSE;
              BOOL bOnlyOne=FALSE;

              if (cr.cpMin == cr.cpMax)
              {
                if (nWindowBlockEnd == cr.cpMin - 1)
                  bAddChar=TRUE;
                if (nWindowBlockEnd == cr.cpMin + 1)
                  bDelChar=TRUE;
                if (nWindowBlockBegin == nWindowBlockEnd)
                  bEmpty=TRUE;

                if (bAddChar)
                {
                  if (GetEditTitlePart(lpSyntaxFileAutoComplete, wszTitlePart, MAX_PATH, &nWindowBlockBegin, &nWindowBlockEnd))
                  {
                    nTitlePartLen=(int)(nWindowBlockEnd - nWindowBlockBegin);

                    if (!bAutoListEnable || nTitlePartLen >= nAutoListAfter)
                    {
                      if (wszTitlePart[0])
                      {
                        if (!(lpBlockInfo=StackGetBlock(lpSyntaxFileAutoComplete, &hDocWordsStack, wszTitlePart, nTitlePartLen, &bOnlyOne)))
                        {
                          if (!bOnlyOne)
                            bLockAutoList=TRUE;
                        }
                      }
                      if (!wszTitlePart[0] || lpBlockInfo)
                      {
                        FillListbox(lpSyntaxFileAutoComplete, &hDocWordsStack, wszTitlePart, nTitlePartLen);
                        nIndex=wszTitlePart[0]?0:-1;
                        SetSelListbox(nIndex);
                        bClose=FALSE;
                      }
                    }
                  }
                }
                if (bClose) SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
              }
            }
            else if (bAutoListEnable)
            {
              if (!bLockAutoList)
              {
                SYNTAXFILE *lpSyntaxFile;

                if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWndEdit, NULL, NULL))
                  StackRequestSyntaxFile(lpSyntaxFile);

                if (lpSyntaxFile || (bAddDocumentWords && bCompleteNonSyntaxDocument))
                {
                  if (CreateAutoCompleteWindow(lpSyntaxFile, CAW_AUTOLIST|CAW_COMPLETEWINDOW) == CAWE_GETBLOCK)
                    bLockAutoList=TRUE;
                }
              }
            }
          }
          else if (!bCompletingTitle)
          {
            if (hWndAutoComplete)
              SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
          }

          //Remove all spots if selection outside the spot
          if (lpCurrentBlockElement)
          {
            if (!GetHotSpot(lpCurrentBlockElement, hWndEdit, &cr))
            {
              StackResetHotSpot(lpCurrentBlockElement);
              lpCurrentBlockElement=NULL;
            }
          }
        }
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT || (HWND)*lResult)
    {
      if (((NMHDR *)lParam)->code == EN_SELCHANGE)
      {
        if (!bChangingText && !bCompletingTitle)
        {
          if (hWndAutoComplete)
            SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
          bLockAutoList=FALSE;
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGING)
      {
        AENTEXTCHANGE *aentc=(AENTEXTCHANGE *)lParam;

        if (aentc->dwType & AETCT_CHAR)
        {
          bTextTypeChar=TRUE;
        }
        else
        {
          bTextTypeChar=FALSE;
          bLockAutoList=FALSE;
        }
        bChangingText=TRUE;
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTINSERTEND)
      {
        AENTEXTINSERT *aenti=(AENTEXTINSERT *)lParam;

        if (GetFocus() == aenti->hdr.hwndFrom)
        {
          hWndEdit=aenti->hdr.hwndFrom;

          //Check for unlock auto-list
          if (aenti->dwType & AETCT_CHAR)
          {
            if (bLockAutoList)
            {
              SYNTAXFILE *lpSyntaxFile;
              STACKDELIM *hDelimiterStack=NULL;

              if (bSyntaxDelimitersEnable)
              {
                if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWndEdit, NULL, NULL))
                  hDelimiterStack=&lpSyntaxFile->hDelimiterStack;
              }
              if (IsDelimiter(hDelimiterStack, hWndEdit, *aenti->wpText))
                bLockAutoList=FALSE;
            }
          }

          //Update hotspots
          if (lpCurrentBlockElement)
          {
            HOTSPOT *lpHotSpot=NULL;
            CHARRANGE64 cr;

            if (aenti->crRichRange.cpMin != aenti->crRichRange.cpMax)
            {
              cr.cpMin=aenti->crRichRange.cpMin;
              cr.cpMax=aenti->crRichRange.cpMin;

              if (lpHotSpot=GetHotSpot(lpCurrentBlockElement, hWndEdit, &cr))
              {
                lpHotSpot->nHotSpotLen+=(int)(aenti->crRichRange.cpMax - aenti->crRichRange.cpMin);

                while (lpHotSpot=lpHotSpot->next)
                {
                  if (aenti->crRichRange.cpMin <= lpCurrentBlockElement->nHotSpotBlockBegin + lpHotSpot->nHotSpotPos)
                  {
                    lpHotSpot->nHotSpotPos+=(int)(aenti->crRichRange.cpMax - aenti->crRichRange.cpMin);
                  }
                }
              }
              else
              {
                StackResetHotSpot(lpCurrentBlockElement);
                lpCurrentBlockElement=NULL;
              }
            }
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTDELETEBEGIN)
      {
        //Update hotspots
        if (lpCurrentBlockElement)
        {
          AENTEXTDELETE *aentd=(AENTEXTDELETE *)lParam;
          HOTSPOT *lpHotSpot=NULL;

          if (aentd->crRichRange.cpMin != aentd->crRichRange.cpMax)
          {
            if (lpHotSpot=GetHotSpot(lpCurrentBlockElement, aentd->hdr.hwndFrom, &aentd->crRichRange))
            {
              lpHotSpot->nHotSpotLen-=(int)(aentd->crRichRange.cpMax - aentd->crRichRange.cpMin);

              while (lpHotSpot=lpHotSpot->next)
              {
                if (aentd->crRichRange.cpMin <= lpCurrentBlockElement->nHotSpotBlockBegin + lpHotSpot->nHotSpotPos)
                {
                  lpHotSpot->nHotSpotPos-=(int)(aentd->crRichRange.cpMax - aentd->crRichRange.cpMin);
                }
              }
            }
            else
            {
              StackResetHotSpot(lpCurrentBlockElement);
              lpCurrentBlockElement=NULL;
            }
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGED)
      {
        bChangingText=FALSE;
      }
    }
  }
  return FALSE;
}

BOOL CALLBACK AutoCompleteEditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (uMsg == WM_KILLFOCUS)
  {
    if (hWndAutoComplete)
      SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);

    if (lpCurrentBlockElement)
    {
      StackResetHotSpot(lpCurrentBlockElement);
      lpCurrentBlockElement=NULL;
    }
  }
  return FALSE;
}

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
  if (code >= 0)
  {
    MSG *msg=(MSG *)lParam;

    if (hWndAutoComplete)
    {
      if (msg->message == WM_LBUTTONDOWN ||
          msg->message == WM_NCLBUTTONDOWN)
      {
        RECT rc;
        POINT pt;

        GetWindowRect(hWndAutoComplete, &rc);
        GetCursorPos(&pt);
        if (!PtInRect(&rc, pt))
          SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
      }
    }
    if (hWndListBox)
    {
      if (msg->message == WM_MOUSEWHEEL)
      {
        SendMessage(hWndListBox, msg->message, msg->wParam, msg->lParam);
        msg->message=WM_NULL;
      }
      else if (msg->message == WM_KEYDOWN ||
               msg->message == WM_KEYUP)
      {
        if (msg->wParam == VK_UP ||
            msg->wParam == VK_DOWN ||
            msg->wParam == VK_PRIOR ||
            msg->wParam == VK_NEXT)
        {
          if (bFirstListBoxKey && (msg->wParam == VK_UP ||
                                   (msg->wParam == VK_DOWN && SendMessage(hWndListBox, LB_GETCOUNT, 0, 0) == 1) ||
                                   GetKeyState(VK_MENU) < 0 ||
                                   GetKeyState(VK_SHIFT) < 0 ||
                                   GetKeyState(VK_CONTROL) < 0))
          {
            if (hWndAutoComplete) SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
          }
          else
          {
            bFirstListBoxKey=FALSE;

            if (bOldWindows)
            {
              SendMessage(hWndListBox, WM_SETREDRAW, FALSE, 0);
            }

            SendMessage(hWndListBox, msg->message, msg->wParam, msg->lParam);
            msg->message=WM_NULL;

            if (bOldWindows)
            {
              SendMessage(hWndListBox, WM_SETREDRAW, TRUE, 0);
              InvalidateRect(hWndListBox, NULL, TRUE);
            }
          }
        }
        else
        {
          if (msg->message == WM_KEYDOWN)
          {
            if (msg->wParam == VK_ESCAPE ||
                msg->wParam == VK_RETURN)
            {
              if (msg->wParam == VK_RETURN)
              {
                if (GetKeyState(VK_MENU) >= 0 &&
                    GetKeyState(VK_CONTROL) >= 0)
                {
                  BLOCKINFO *lpBlockInfo;

                  if (lpBlockInfo=GetDataListbox(-1))
                  {
                    if (GetKeyState(VK_SHIFT) < 0)
                      bSaveTypedCaseOnce=!bSaveTypedCase;
                    CompleteTitlePart(NULL, lpBlockInfo, nWindowBlockBegin, nWindowBlockEnd);
                    bSaveTypedCaseOnce=-1;
                    msg->message=WM_NULL;
                  }
                }
              }
              else msg->message=WM_NULL;

              if (hWndAutoComplete) SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
            }
          }
        }
      }
    }
  }
  return CallNextHookEx(hHook, code, wParam, lParam);
}

BOOL CALLBACK CompleteWithListProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  SYNTAXFILE *lpSyntaxFile;
  DWORD dwCreateFlags=0;
  BOOL bCatchHotkey=TRUE;

  if (hWndEdit=GetFocusEdit())
  {
    if (!lpParameter)
      dwCreateFlags=CAW_COMPLETEONE|CAW_COMPLETEWINDOW;
    else
      dwCreateFlags=(DWORD)(UINT_PTR)lpParameter;

    if (hWndAutoComplete)
      SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);

    if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWndEdit, NULL, NULL))
      StackRequestSyntaxFile(lpSyntaxFile);

    if (lpSyntaxFile || (bAddDocumentWords && bCompleteNonSyntaxDocument))
    {
      if (CreateAutoCompleteWindow(lpSyntaxFile, dwCreateFlags))
      {
        if (!(dwCreateFlags & CAW_COMPLETEWINDOW))
          bCatchHotkey=FALSE;
      }
    }
    else if (!(dwCreateFlags & CAW_COMPLETEWINDOW))
      bCatchHotkey=FALSE;
  }
  return bCatchHotkey;
}

BOOL CALLBACK CompleteWithoutListProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  BOOL bCatchHotkey=FALSE;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentBlockElement && bOneWithoutListAndCompleteNext)
    {
      if (!NextHotSpot(lpCurrentBlockElement, hWndEdit, FALSE))
      {
        StackResetHotSpot(lpCurrentBlockElement);
        lpCurrentBlockElement=NULL;
      }
      else bCatchHotkey=TRUE;
    }
    else
    {
      bCatchHotkey=CompleteWithListProc((void *)(UINT_PTR)(CAW_COMPLETEONE|CAW_COMPLETEEXACT), 0, 0);
    }
  }
  return bCatchHotkey;
}

BOOL CALLBACK CompleteNextProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  BOOL bCatchHotkey=FALSE;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentBlockElement)
    {
      if (!NextHotSpot(lpCurrentBlockElement, hWndEdit, FALSE))
      {
        StackResetHotSpot(lpCurrentBlockElement);
        lpCurrentBlockElement=NULL;
      }
      else bCatchHotkey=TRUE;
    }
  }
  return bCatchHotkey;
}

BOOL CALLBACK CompletePrevProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  BOOL bCatchHotkey=FALSE;

  if (hWndEdit=GetFocusEdit())
  {
    if (lpCurrentBlockElement)
    {
      if (!NextHotSpot(lpCurrentBlockElement, hWndEdit, TRUE))
      {
        StackResetHotSpot(lpCurrentBlockElement);
        lpCurrentBlockElement=NULL;
      }
      else bCatchHotkey=TRUE;
    }
  }
  return bCatchHotkey;
}

LRESULT CALLBACK AutoCompleteWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static POINT ptResizeHitOffset={0};
  static int nResizeHit=0;
  static int nCharHeight;
  static HBRUSH hListBoxBrush;

  if (uMsg == WM_CREATE)
  {
    wchar_t wszIconFile[MAX_PATH];
    HFONT hListboxFont;
    COLORREF crBk;
    int nItemHeight;
    int nFileIconIndex;

    if (lpSyntaxFileAutoComplete && ((!bCompleteListSystemColors &&
        (lpSyntaxFileAutoComplete->dwCompleteListBasicTextColor != (DWORD)-1 ||
         lpSyntaxFileAutoComplete->dwCompleteListBasicBkColor != (DWORD)-1 ||
         lpSyntaxFileAutoComplete->dwCompleteListSelTextColor != (DWORD)-1 ||
         lpSyntaxFileAutoComplete->dwCompleteListSelBkColor != (DWORD)-1)) ||
         lpSyntaxFileAutoComplete->dwCompleteListIcons ||
         bCompleteListItemHlBaseColors))
    {
      //ListBox background
      if (bCompleteListSystemColors || lpSyntaxFileAutoComplete->dwCompleteListBasicBkColor == (DWORD)-1)
        crBk=GetSysColor(COLOR_WINDOW);
      else
        crBk=lpSyntaxFileAutoComplete->dwCompleteListBasicBkColor;
      hListBoxBrush=CreateSolidBrush(crBk);
    }
    else hListBoxBrush=NULL;

    hWndListBox=CreateWindowExWide(0,
                            L"LISTBOX",
                            NULL,
                            WS_CHILD|WS_VSCROLL|WS_VISIBLE|
                            LBS_DISABLENOSCROLL|LBS_NOINTEGRALHEIGHT|(hListBoxBrush?LBS_OWNERDRAWFIXED:0),
                            0, 0, 0, 0,
                            hWnd,
                            (HMENU)(UINT_PTR)IDC_LIST,
                            hInstanceDLL,
                            NULL);

    //Font
    if (lpSyntaxFileAutoComplete && lpSyntaxFileAutoComplete->hCompleteListFont)
      hListboxFont=lpSyntaxFileAutoComplete->hCompleteListFont;
    else
    {
      if (lpSyntaxFileAutoComplete && (*lpSyntaxFileAutoComplete->wszCompleteListFaceName || lpSyntaxFileAutoComplete->nCompleteListFontSize || lpSyntaxFileAutoComplete->dwCompleteListFontStyle))
      {
        lpSyntaxFileAutoComplete->hCompleteListFont=CreateFontMethod(lpSyntaxFileAutoComplete->wszCompleteListFaceName, lpSyntaxFileAutoComplete->dwCompleteListFontStyle, lpSyntaxFileAutoComplete->nCompleteListFontSize);
        hListboxFont=lpSyntaxFileAutoComplete->hCompleteListFont;
      }
      else hListboxFont=(HFONT)SendMessage(hWndEdit, AEM_GETFONT, AEGF_CURRENT, 0);
    }
    SendMessage(hWndListBox, WM_SETFONT, (WPARAM)hListboxFont, TRUE);

    //Icons
    if (lpSyntaxFileAutoComplete)
    {
      if ((lpSyntaxFileAutoComplete->dwCompleteListIcons & BIT_BLOCK) && !lpSyntaxFileAutoComplete->hCompleteListBlockIcon)
      {
        GetIconParameters(lpSyntaxFileAutoComplete->wszCompleteListBlockIcon, wszIconFile, MAX_PATH, &nFileIconIndex, NULL);
        lpSyntaxFileAutoComplete->hCompleteListBlockIcon=GetIconMethod(wszIconFile, nFileIconIndex, FALSE);
      }
      if ((lpSyntaxFileAutoComplete->dwCompleteListIcons & BIT_HLBASE) && !lpSyntaxFileAutoComplete->hCompleteListHlBaseIcon)
      {
        GetIconParameters(lpSyntaxFileAutoComplete->wszCompleteListHlBaseIcon, wszIconFile, MAX_PATH, &nFileIconIndex, NULL);
        lpSyntaxFileAutoComplete->hCompleteListHlBaseIcon=GetIconMethod(wszIconFile, nFileIconIndex, FALSE);
      }
      if ((lpSyntaxFileAutoComplete->dwCompleteListIcons & BIT_DOCWORD) && !lpSyntaxFileAutoComplete->hCompleteListDocWordIcon)
      {
        GetIconParameters(lpSyntaxFileAutoComplete->wszCompleteListDocWordIcon, wszIconFile, MAX_PATH, &nFileIconIndex, NULL);
        lpSyntaxFileAutoComplete->hCompleteListDocWordIcon=GetIconMethod(wszIconFile, nFileIconIndex, FALSE);
      }
    }

    //Height
    {
      SIZE sizeChar={0};
      HDC hDC;
      HFONT hFontOld;

      if (hDC=GetDC(hWndListBox))
      {
        hFontOld=(HFONT)SelectObject(hDC, hListboxFont);
        GetTextExtentPoint32W(hDC, L"A", 1, &sizeChar);
        if (hFontOld) SelectObject(hDC, hFontOld);
        ReleaseDC(hWndListBox, hDC);
      }
      nCharHeight=sizeChar.cy;
      nItemHeight=nCharHeight;
    }
    if (lpSyntaxFileAutoComplete && lpSyntaxFileAutoComplete->dwCompleteListIcons)
      nItemHeight=max(nItemHeight, SIZE_ICON);
    if (lpSyntaxFileAutoComplete && lpSyntaxFileAutoComplete->nCompleteListLineGap)
      nItemHeight+=lpSyntaxFileAutoComplete->nCompleteListLineGap;
    if (SendMessage(hWndListBox, LB_GETITEMHEIGHT, 0, 0) < nItemHeight)
      SendMessage(hWndListBox, LB_SETITEMHEIGHT, 0, nItemHeight);

    OldListBoxProc=(WNDPROC)GetWindowLongPtrWide(hWndListBox, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndListBox, GWLP_WNDPROC, (UINT_PTR)NewListboxProc);
  }
  else if (uMsg == WM_SIZE)
  {
    if (hWndListBox)
    {
      SetWindowPos(hWndListBox, NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE);
    }
    return FALSE;
  }
  else if (uMsg == WM_CTLCOLORLISTBOX)
  {
    if (hListBoxBrush)
      return (LRESULT)hListBoxBrush;
  }
  else if (uMsg == WM_DRAWITEM)
  {
    DRAWITEMSTRUCT *dis=(DRAWITEMSTRUCT *)lParam;
    DWORD dwIconMargins=0;
    HICON hIcon=NULL;
    COLORREF crText;
    COLORREF crBk;
    COLORREF crHlBk=(DWORD)-1;
    HBRUSH hBrushBk=NULL;
    int nModeBkOld;
    int nIconSize=0;
    BLOCKINFO *lpBlockInfo;
    WORDINFO *lpWordInfo=NULL;
    BOOL bHideMark;

    if (dis->CtlID == IDC_LIST)
    {
      if (lpBlockInfo=GetDataListbox(dis->itemID))
      {
        //Set background
        if (bCompleteListItemHlBaseColors && (lpBlockInfo->dwStructType & BIT_HLBASE))
          lpWordInfo=(WORDINFO *)lpBlockInfo->lpRef;

        if (dis->itemState & ODS_SELECTED)
        {
          if (bCompleteListSystemColors || lpSyntaxFileAutoComplete->dwCompleteListSelTextColor == (DWORD)-1)
            crText=GetSysColor(COLOR_HIGHLIGHTTEXT);
          else
            crText=lpSyntaxFileAutoComplete->dwCompleteListSelTextColor;

          if (bCompleteListSystemColors || lpSyntaxFileAutoComplete->dwCompleteListSelBkColor == (DWORD)-1)
            crBk=GetSysColor(COLOR_HIGHLIGHT);
          else
            crBk=lpSyntaxFileAutoComplete->dwCompleteListSelBkColor;
          hBrushBk=CreateSolidBrush(crBk);
        }
        else
        {
          if (!bCompleteListSystemColors && lpWordInfo && lpWordInfo->dwColor1 != (DWORD)-1)
            crText=lpWordInfo->dwColor1;
          else if (!bCompleteListSystemColors && lpSyntaxFileAutoComplete->dwCompleteListBasicTextColor != (DWORD)-1)
            crText=lpSyntaxFileAutoComplete->dwCompleteListBasicTextColor;
          else
            crText=GetSysColor(COLOR_WINDOWTEXT);

          if (!bCompleteListSystemColors && lpWordInfo && lpWordInfo->dwColor2 != (DWORD)-1)
            crHlBk=lpWordInfo->dwColor2;
          if (!bCompleteListSystemColors && lpSyntaxFileAutoComplete->dwCompleteListBasicBkColor != (DWORD)-1)
            crBk=lpSyntaxFileAutoComplete->dwCompleteListBasicBkColor;
          else
            crBk=GetSysColor(COLOR_WINDOW);
          hBrushBk=CreateSolidBrush(crBk);
        }
        FillRect(dis->hDC, &dis->rcItem, hBrushBk);
        nModeBkOld=SetBkMode(dis->hDC, TRANSPARENT);

        //Draw icon
        if (lpSyntaxFileAutoComplete->dwCompleteListIcons & lpBlockInfo->dwStructType)
        {
          nIconSize=SIZE_ICON;
          if (lpBlockInfo->dwStructType & BIT_BLOCK)
          {
            hIcon=lpSyntaxFileAutoComplete->hCompleteListBlockIcon;
            dwIconMargins=lpSyntaxFileAutoComplete->dwCompleteListBlockIconMargins;
          }
          else if (lpBlockInfo->dwStructType & BIT_HLBASE)
          {
            hIcon=lpSyntaxFileAutoComplete->hCompleteListHlBaseIcon;
            dwIconMargins=lpSyntaxFileAutoComplete->dwCompleteListHlBaseIconMargins;
          }
          else if (lpBlockInfo->dwStructType & BIT_DOCWORD)
          {
            hIcon=lpSyntaxFileAutoComplete->hCompleteListDocWordIcon;
            dwIconMargins=lpSyntaxFileAutoComplete->dwCompleteListDocWordIconMargins;
          }
          if (hIcon)
            DrawIconEx(dis->hDC, dis->rcItem.left + LOWORD(dwIconMargins), dis->rcItem.top + (dis->rcItem.bottom - dis->rcItem.top) / 2 - nIconSize / 2, hIcon, nIconSize, nIconSize, 0, 0, DI_NORMAL);
        }

        //Draw text
        SetTextColor(dis->hDC, crText);
        if (crHlBk != (DWORD)-1)
        {
          SetBkMode(dis->hDC, OPAQUE);
          SetBkColor(dis->hDC, crHlBk);
        }
        else SetBkColor(dis->hDC, crBk);

        if ((lpBlockInfo->dwStructType & (BIT_HLBASE|BIT_DOCWORD)) && (dwCompleteListSymbolMark & CLSM_NOMARKIFICON) && hIcon)
          bHideMark=TRUE;
        else
          bHideMark=FALSE;
        TextOutW(dis->hDC, dis->rcItem.left + LOWORD(dwIconMargins) + nIconSize + HIWORD(dwIconMargins), dis->rcItem.top + (dis->rcItem.bottom - dis->rcItem.top) / 2 - nCharHeight / 2, lpBlockInfo->wpTitle, lpBlockInfo->nTitleLen - (bHideMark?1:0));

        SetBkMode(dis->hDC, nModeBkOld);
        if (hBrushBk) DeleteObject(hBrushBk);
      }
    }
  }
  else if (uMsg == WM_MOUSEACTIVATE)
  {
    return MA_NOACTIVATE;
  }
  else if (uMsg == WM_NCHITTEST)
  {
    LRESULT lResult;

    lResult=DefWindowProcWide(hWnd, uMsg, wParam, lParam);

    if (lResult == HTRIGHT ||
        lResult == HTBOTTOMRIGHT ||
        lResult == HTBOTTOM)
    {
      return lResult;
    }
    return HTERROR;
  }
  else if (uMsg == WM_NCLBUTTONDOWN)
  {
    RECT rc;
    POINT pt;

    GetWindowRect(hWnd, &rc);
    GetCursorPos(&pt);

    if (wParam == HTRIGHT ||
        wParam == HTBOTTOMRIGHT ||
        wParam == HTBOTTOM)
    {
      ptResizeHitOffset.x=rc.right - pt.x;
      ptResizeHitOffset.y=rc.bottom - pt.y;
    }
    else if (wParam == HTTOPRIGHT)
    {
      ptResizeHitOffset.x=rc.right - pt.x;
      ptResizeHitOffset.y=rc.top - pt.y;
    }
    else if (wParam == HTLEFT ||
             wParam == HTTOPLEFT ||
             wParam == HTTOP)
    {
      ptResizeHitOffset.x=rc.left - pt.x;
      ptResizeHitOffset.y=rc.top - pt.y;
    }
    else if (wParam == HTBOTTOMLEFT)
    {
      ptResizeHitOffset.x=rc.left - pt.x;
      ptResizeHitOffset.y=rc.bottom - pt.y;
    }
    SetCapture(hWnd);
    nResizeHit=(int)wParam;
    return FALSE;
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (nResizeHit)
    {
      RECT rc;
      POINT pt;

      GetWindowRect(hWnd, &rc);
      GetCursorPos(&pt);

      pt.x+=ptResizeHitOffset.x;
      pt.y+=ptResizeHitOffset.y;

      if (nResizeHit == HTLEFT)
      {
        rc.left=pt.x;
      }
      else if (nResizeHit == HTRIGHT)
      {
        rc.right=pt.x;
      }
      else if (nResizeHit == HTTOP)
      {
        rc.top=pt.y;
      }
      else if (nResizeHit == HTTOPLEFT)
      {
        rc.top=pt.y;
        rc.left=pt.x;
      }
      else if (nResizeHit == HTTOPRIGHT)
      {
        rc.top=pt.y;
        rc.right=pt.x;
      }
      else if (nResizeHit == HTBOTTOM)
      {
        rc.bottom=pt.y;
      }
      else if (nResizeHit == HTBOTTOMLEFT)
      {
        rc.bottom=pt.y;
        rc.left=pt.x;
      }
      else if (nResizeHit == HTBOTTOMRIGHT)
      {
        rc.bottom=pt.y;
        rc.right=pt.x;
      }
      SetWindowPos(hWnd, NULL, rc.left, rc.top, max(rc.right - rc.left, SIZE_MINX), max(rc.bottom - rc.top, SIZE_MINY), SWP_NOZORDER|SWP_NOACTIVATE);
      return FALSE;
    }
  }
  else if (uMsg == WM_LBUTTONUP ||
           uMsg == WM_CANCELMODE)
  {
    if (nResizeHit)
    {
      nResizeHit=0;
      ReleaseCapture();
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    RECT rc;

    GetWindowRect(hWnd, &rc);
    rc.right-=rc.left;
    rc.bottom-=rc.top;
    if (rcAutoComplete.right != rc.right ||
        rcAutoComplete.bottom != rc.bottom)
    {
      rcAutoComplete=rc;
      dwSaveFlags|=OF_AUTOCOMPLETE_RECT;
    }
    CloseAutoCompleteWindow();
  }
  else if (uMsg == WM_DESTROY)
  {
    hWndAutoComplete=NULL;
    hWndListBox=NULL;
    if (hListBoxBrush) DeleteObject(hListBoxBrush);
  }

  return DefWindowProcWide(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewListboxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_MOUSEACTIVATE)
  {
    return MA_NOACTIVATE;
  }
  else if (uMsg == WM_LBUTTONDOWN)
  {
    LRESULT lResult=SendMessage(hWnd, LB_ITEMFROMPOINT, 0, lParam);

    if (HIWORD(lResult) == 0)
    {
      SendMessage(hWnd, LB_SETCURSEL, LOWORD(lResult), 0);
    }
    return FALSE;
  }
  else if (uMsg == WM_LBUTTONUP)
  {
    return FALSE;
  }
  else if (uMsg == WM_MBUTTONDOWN)
  {
    return FALSE;
  }
  else if (uMsg == WM_MBUTTONUP)
  {
    return FALSE;
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    BLOCKINFO *lpBlockInfo;

    if (lpBlockInfo=GetDataListbox(-1))
    {
      if (GetKeyState(VK_SHIFT) < 0)
        bSaveTypedCaseOnce=!bSaveTypedCase;
      CompleteTitlePart(NULL, lpBlockInfo, nWindowBlockBegin, nWindowBlockEnd);
      bSaveTypedCaseOnce=-1;
    }
    SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
    return FALSE;
  }

  if (bOldWindows)
    return CallWindowProcA(OldListBoxProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldListBoxProc, hWnd, uMsg, wParam, lParam);
}

DWORD CreateAutoCompleteWindow(SYNTAXFILE *lpSyntaxFile, DWORD dwFlags)
{
  BLOCKINFO *lpBlockInfo=NULL;
  wchar_t wszTitlePart[MAX_PATH];
  int nTitlePartLen;
  int nIndex;
  INT_PTR nBlockBegin;
  INT_PTR nBlockEnd;
  BOOL bOnlyOne=FALSE;
  BOOL bSkipAddDocumentWords=FALSE;

  if (hWndEdit=GetFocusEdit())
  {
    if (bAddDocumentWords && bMaxDocumentEnable && nMaxDocumentChars)
    {
      GETTEXTLENGTHEX gtl;
      INT_PTR nTextLength;

      gtl.flags=GTL_DEFAULT;
      nTextLength=SendMessage(hWndEdit, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);
      if (nMaxDocumentChars < nTextLength)
      {
        if (!lpSyntaxFile)
          return CAWE_DOCUMENTLIMIT;
        bSkipAddDocumentWords=TRUE;
      }
    }
  }
  else return CAWE_NOEDITWINDOW;

  if (lpSyntaxFile && lpSyntaxFile->hFoldStack.nFoldWithThemeCount && lpCurrentFoldWindow)
  {
    SYNTAXFILE *lpSyntaxFileCaret;
    AEFOLD *lpFold;

    if (lpFold=GetCaretFold(lpCurrentFoldWindow, NULL))
    {
      if (lpSyntaxFileCaret=GetSyntaxFileByFold(lpCurrentFoldWindow, lpFold))
        lpSyntaxFile=lpSyntaxFileCaret;
    }
  }

  if (lpSyntaxFile && lpSyntaxFile->hExactBlockStack.first && (dwFlags & CAW_COMPLETEONE))
  {
    AECHARINDEX ciCaret;
    AECHARINDEX ciLineBegin;
    BLOCKINFO *lpBlockMaster;
    INT_PTR nCaretOffset;
    AESELECTION aes;

    aes.dwFlags=AESELT_LOCKSCROLL;
    aes.dwType=0;
    SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
    nCaretOffset=SendMessage(hWndEdit, AEM_GETRICHOFFSET, AEGI_CARETCHAR, 0);

    //Regular expression match: "$(4)=..."
    for (lpBlockInfo=(BLOCKINFO *)lpSyntaxFile->hExactBlockStack.first; lpBlockInfo; lpBlockInfo=lpBlockInfo->next)
    {
      if (lpBlockInfo->dwTitleFlags & TF_REGEXP)
      {
        AEC_WrapLineBeginEx(&ciCaret, &ciLineBegin);
        lpBlockInfo->sregTitle->ciRange=ciCaret;
        lpBlockInfo->sregTitle->ciMaxRange=ciCaret;

        if (IsMatchRE(lpBlockInfo->sregTitle, &aes.crSel, &ciLineBegin) >= 0)
        {
          SendMessage(hWndEdit, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aes);
          if (lpBlockInfo->master)
            lpBlockMaster=lpBlockInfo->master;
          else
            lpBlockMaster=lpBlockInfo;
          SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndEdit, (LPARAM)lpBlockMaster->wpBlock);
          return CAWE_SUCCESS;
        }
      }
    }
    if (lpBlockInfo=StackGetExactBlock(lpSyntaxFile, &ciCaret, nCaretOffset, &nBlockBegin, &nBlockEnd))
    {
      CompleteTitlePart(lpSyntaxFile, lpBlockInfo, nBlockBegin, nBlockEnd);
      return CAWE_SUCCESS;
    }
  }

  if (!GetEditTitlePart(lpSyntaxFile, wszTitlePart, MAX_PATH, &nBlockBegin, &nBlockEnd))
    return CAWE_GETTITLEPART;
  nTitlePartLen=(int)(nBlockEnd - nBlockBegin);

  if (dwFlags & CAW_AUTOLIST)
  {
    if (nTitlePartLen < nAutoListAfter)
      return CAWE_AUTOLIST;
  }
  if ((dwFlags & CAW_COMPLETEONE) && (dwFlags & CAW_COMPLETEEXACT))
  {
    if (nTitlePartLen < nCompleteWithoutListAfter)
      return CAWE_COMPLETEEXACT;
  }

  //Complete without window
  if (wszTitlePart[0])
  {
    if (bAddDocumentWords && !bSkipAddDocumentWords)
      StackFillDocWord(lpSyntaxFile, &hDocWordsStack, wszTitlePart, nTitlePartLen);

    if (lpBlockInfo=StackGetBlock(lpSyntaxFile, &hDocWordsStack, wszTitlePart, nTitlePartLen, &bOnlyOne))
    {
      if (bOnlyOne || ((dwFlags & CAW_COMPLETEEXACT) && !CompleteStrCmp(lpBlockInfo->dwTitleFlags, lpBlockInfo->wpTitle, wszTitlePart)))
      {
        if (dwFlags & CAW_COMPLETEONE)
        {
          CompleteTitlePart(lpSyntaxFile, lpBlockInfo, nBlockBegin, nBlockEnd);
          StackFreeDocWord(&hDocWordsStack);
          return CAWE_SUCCESS;
        }
      }
    }
    else
    {
      StackFreeDocWord(&hDocWordsStack);
      return CAWE_GETBLOCK;
    }
  }

  //Complete with window
  if (dwFlags & CAW_COMPLETEWINDOW)
  {
    if (!wszTitlePart[0] || lpBlockInfo)
    {
      lpSyntaxFileAutoComplete=lpSyntaxFile;
      nWindowBlockBegin=nBlockBegin;
      nWindowBlockEnd=nBlockEnd;

      hWndAutoComplete=CreateWindowExWide((bAlphaListEnable && SetLayeredWindowAttributesPtr)?WS_EX_LAYERED:0,
                            AUTOCOMPLETEW,
                            NULL,
                            WS_POPUP|WS_THICKFRAME,
                            0, 0, rcAutoComplete.right, rcAutoComplete.bottom,
                            hMainWnd,
                            NULL,
                            hInstanceDLL,
                            NULL);

      if (MoveAutoCompleteWindow())
      {
        FillListbox(lpSyntaxFile, &hDocWordsStack, wszTitlePart, nTitlePartLen);
        nIndex=wszTitlePart[0]?0:-1;
        SetSelListbox(nIndex);
        ShowWindow(hWndAutoComplete, SW_SHOWNOACTIVATE);
        if (bAlphaListEnable && SetLayeredWindowAttributesPtr)
        {
          SetLayeredWindowAttributesPtr(hWndAutoComplete, 0, 0, LWA_ALPHA);
          UpdateWindow(hWndAutoComplete);
          SetLayeredWindowAttributesPtr(hWndAutoComplete, 0, (BYTE)nAlphaListValue, LWA_ALPHA);
        }

        bFirstListBoxKey=TRUE;
        hHook=SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, NULL, GetWindowThreadProcessId(hWndEdit, NULL));
        return CAWE_SUCCESS;
      }
      else SendMessage(hWndAutoComplete, WM_CLOSE, 0, 0);
    }
  }
  return CAWE_OTHER;
}

BOOL MoveAutoCompleteWindow()
{
  RECT rcEdit;
  POINT ptOpen;
  POINT ptPrimaryMax;
  int nCharHeight;

  if (!hWndAutoComplete) return FALSE;
  GetPosFromChar(hWndEdit, nWindowBlockBegin, &ptOpen);
  nCharHeight=(int)SendMessage(hWndEdit, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
  ptOpen.y+=nCharHeight;
  SendMessage(hWndEdit, AEM_GETRECT, 0, (LPARAM)&rcEdit);
  if (ptOpen.x < rcEdit.left || ptOpen.x > rcEdit.right || ptOpen.y < rcEdit.top || ptOpen.y - nCharHeight > rcEdit.bottom)
    return FALSE;
  ptPrimaryMax.x=GetSystemMetrics(SM_CXMAXIMIZED);
  ptPrimaryMax.y=GetSystemMetrics(SM_CYMAXIMIZED);

  if (ClientToScreen(hWndEdit, &ptOpen))
  {
    //Correct window position only for primary monitor
    if (ptOpen.x < ptPrimaryMax.x && ptOpen.x + rcAutoComplete.right > ptPrimaryMax.x)
      ptOpen.x=max(ptOpen.x - rcAutoComplete.right, 0);
    if (ptOpen.y < ptPrimaryMax.y && ptOpen.y + rcAutoComplete.bottom > ptPrimaryMax.y)
      ptOpen.y=max(ptOpen.y - rcAutoComplete.bottom - nCharHeight, 0);
    SetWindowPos(hWndAutoComplete, NULL, ptOpen.x, ptOpen.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
    return TRUE;
  }
  return FALSE;
}

void CloseAutoCompleteWindow()
{
  if (hHook)
  {
    if (UnhookWindowsHookEx(hHook))
      hHook=NULL;
  }
  if (bAddDocumentWords)
    StackFreeDocWord(&hDocWordsStack);
}

void FillListbox(SYNTAXFILE *lpSyntaxFile, STACKDOCWORDS *hDocWordsStack, const wchar_t *wpTitlePart, int nTitlePartLen)
{
  int nItem;

  SendMessage(hWndListBox, LB_RESETCONTENT, 0, 0);

  if (lpSyntaxFile)
  {
    //Blocks
    BLOCKORDER *lpBlockOrder=(BLOCKORDER *)lpSyntaxFile->hBlockOrderStack.first;

    while (lpBlockOrder)
    {
      if (lpBlockOrder->lpBlockInfo->nTitleLen >= nTitlePartLen && !(lpBlockOrder->lpBlockInfo->dwTitleFlags & TF_NOLISTBOX))
      {
        if (!CompleteStrCmpLen(lpBlockOrder->lpBlockInfo->dwTitleFlags, wpTitlePart, lpBlockOrder->lpBlockInfo->wpTitle, (UINT_PTR)-1))
        {
          if ((nItem=ListBox_AddStringWide(hWndListBox, lpBlockOrder->lpBlockInfo->wpTitle)) != LB_ERR)
            SendMessage(hWndListBox, LB_SETITEMDATA, nItem, (LPARAM)lpBlockOrder->lpBlockInfo);
        }
      }
      lpBlockOrder=lpBlockOrder->next;
    }

    if (!wpTitlePart[0])
    {
      BLOCKINFO *lpBlockInfo=(BLOCKINFO *)lpSyntaxFile->hExactBlockStack.first;

      while (lpBlockInfo)
      {
        if (!(lpBlockInfo->dwTitleFlags & TF_NOLISTBOX))
        {
          if ((nItem=ListBox_AddStringWide(hWndListBox, lpBlockInfo->wpTitle)) != LB_ERR)
            SendMessage(hWndListBox, LB_SETITEMDATA, nItem, (LPARAM)lpBlockInfo);
        }
        lpBlockInfo=lpBlockInfo->next;
      }
    }

    //Words from HighLight base
    if (bAddHighLightWords)
    {
      WORDORDER *lpWordOrder=(WORDORDER *)lpSyntaxFile->hWordOrderStack.first;
      WORDORDER *lpWordAdded=NULL;
      wchar_t *wpString=wszBuffer;
      int nItem;

      for (; lpWordOrder; lpWordOrder=lpWordOrder->next)
      {
        if (lpWordOrder->lpWordInfo->nWordLen >= nTitlePartLen)
        {
          if (!CompleteStrCmpLen(0, wpTitlePart, lpWordOrder->lpWordInfo->wpWord, (UINT_PTR)-1))
          {
            //Avoid to add the same word to list. Can appear if different parent used.
            if (lpWordAdded && (bCompleteListSystemColors || !bCompleteListItemHlBaseColors || (lpWordAdded->lpWordInfo->dwColor1 == lpWordOrder->lpWordInfo->dwColor1 &&
                                                                                                lpWordAdded->lpWordInfo->dwColor2 == lpWordOrder->lpWordInfo->dwColor2)) &&
                               !xstrcmpW(lpWordAdded->lpWordInfo->wpWord, lpWordOrder->lpWordInfo->wpWord))
              continue;

            if (dwCompleteListSymbolMark == CLSM_NOMARK)
              wpString=lpWordOrder->lpWordInfo->wpWord;
            else
              xprintfW(wpString, L"%s*", lpWordOrder->lpWordInfo->wpWord);
            if ((nItem=ListBox_AddStringWide(hWndListBox, wpString)) != LB_ERR)
              SendMessage(hWndListBox, LB_SETITEMDATA, nItem, (LPARAM)lpWordOrder->lpWordInfo);
            lpWordAdded=lpWordOrder;
          }
        }
      }
    }
  }

  //Document words
  if (bAddDocumentWords)
  {
    DOCWORDINFO *lpDocWordInfo=hDocWordsStack->first;
    wchar_t *wpString=wszBuffer;
    int nItem;

    while (lpDocWordInfo)
    {
      if (lpDocWordInfo->nDocWordLen >= nTitlePartLen)
      {
        if (!CompleteStrCmpLen(0, wpTitlePart, lpDocWordInfo->wpDocWord, (UINT_PTR)-1))
        {
          if (dwCompleteListSymbolMark == CLSM_NOMARK)
            wpString=lpDocWordInfo->wpDocWord;
          else
            xprintfW(wpString, L"%s+", lpDocWordInfo->wpDocWord);
          if ((nItem=ListBox_AddStringWide(hWndListBox, wpString)) != LB_ERR)
            SendMessage(hWndListBox, LB_SETITEMDATA, nItem, (LPARAM)lpDocWordInfo);
        }
      }
      lpDocWordInfo=lpDocWordInfo->next;
    }
  }
}

void SetSelListbox(int nIndex)
{
  if (bOldWindows)
  {
    SendMessage(hWndListBox, WM_SETREDRAW, FALSE, 0);
  }
  if (nIndex != -1)
  {
    SendMessage(hWndListBox, LB_SETCURSEL, nIndex, 0);
    SendMessage(hWndListBox, LB_SETTOPINDEX, nIndex, 0);
  }
  else
  {
    SendMessage(hWndListBox, LB_SETCURSEL, 0, 0);
    SendMessage(hWndListBox, LB_SETCURSEL, (WPARAM)-1, 0);
    SendMessage(hWndListBox, LB_SETTOPINDEX, 0, 0);
  }
  if (bOldWindows)
  {
    SendMessage(hWndListBox, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hWndListBox, NULL, TRUE);
  }
}

BLOCKINFO* GetDataListbox(int nItem)
{
  BLOCKINFO *lpBlock;
  WORDINFO *lpHighLightWord;
  DOCWORDINFO *lpDocWord;

  if (nItem == -1)
    nItem=(int)SendMessage(hWndListBox, LB_GETCURSEL, 0, 0);

  if (nItem != LB_ERR)
  {
    lpBlock=(BLOCKINFO *)SendMessage(hWndListBox, LB_GETITEMDATA, nItem, 0);

    //Item data is BLOCKINFO pointer
    if (lpBlock->dwStructType & BIT_BLOCK)
      return lpBlock;

    //Item data is DOCWORDINFO pointer
    if (lpBlock->dwStructType & BIT_DOCWORD)
    {
      lpDocWord=(DOCWORDINFO *)lpBlock;
      biDocWordBlock.dwStructType=BIT_DOCWORD;
      if (dwCompleteListSymbolMark == CLSM_NOMARK)
      {
        biDocWordBlock.wpTitle=lpDocWord->wpDocWord;
        biDocWordBlock.nTitleLen=lpDocWord->nDocWordLen;
      }
      else
      {
        biDocWordBlock.wpTitle=wszDocWordTitle;
        biDocWordBlock.nTitleLen=(int)xprintfW(wszDocWordTitle, L"%s+", lpDocWord->wpDocWord);
      }
      biDocWordBlock.wpBlock=lpDocWord->wpDocWord;
      biDocWordBlock.nBlockLen=lpDocWord->nDocWordLen;
      biDocWordBlock.nLinesInBlock=1;
      biDocWordBlock.lpRef=lpDocWord;
      return &biDocWordBlock;
    }

    //Item data is WORDINFO pointer
    lpHighLightWord=(WORDINFO *)lpBlock;
    biHighLightBlock.dwStructType=BIT_HLBASE;
    if (dwCompleteListSymbolMark == CLSM_NOMARK)
    {
      biHighLightBlock.wpTitle=lpHighLightWord->wpWord;
      biHighLightBlock.nTitleLen=lpHighLightWord->nWordLen;
    }
    else
    {
      biHighLightBlock.wpTitle=wszHighLightTitle;
      biHighLightBlock.nTitleLen=(int)xprintfW(wszHighLightTitle, L"%s*", lpHighLightWord->wpWord);
    }
    biHighLightBlock.wpBlock=lpHighLightWord->wpWord;
    biHighLightBlock.nBlockLen=lpHighLightWord->nWordLen;
    biHighLightBlock.nLinesInBlock=1;
    biHighLightBlock.lpRef=lpHighLightWord;
    return &biHighLightBlock;
  }
  return NULL;
}

int CompleteStrCmp(DWORD dwTitleFlags, const wchar_t *wpString1, const wchar_t *wpString2)
{
  if ((dwTitleFlags & (TF_FORCECASESENSITIVE|TF_FORCECASEINSENSITIVE)) ? (dwTitleFlags & TF_FORCECASESENSITIVE) : (DWORD)bCompleteCaseSensitive)
    return xstrcmpW(wpString1, wpString2);
  return xstrcmpiW(wpString1, wpString2);
}

int CompleteStrCmpLen(DWORD dwTitleFlags, const wchar_t *wpString1, const wchar_t *wpString2, UINT_PTR dwMaxLength)
{
  if ((dwTitleFlags & (TF_FORCECASESENSITIVE|TF_FORCECASEINSENSITIVE)) ? (dwTitleFlags & TF_FORCECASESENSITIVE) : (DWORD)bCompleteCaseSensitive)
    return xstrcmpnW(wpString1, wpString2, dwMaxLength);
  return xstrcmpinW(wpString1, wpString2, dwMaxLength);
}

wchar_t CompleteFirstChar(wchar_t wchChar)
{
  if (bCompleteCaseSensitive)
    return wchChar;
  return WideCharLower(wchChar);
}

int ParseBlock(SYNTAXFILE *lpScheme, HSTACK *hHotSpotStack, const wchar_t *wpInput, int nInputLen, wchar_t *wszOutput, int *nOutputLines)
{
  VARINFO *lpElement;
  HOTSPOT *lpHotSpot=NULL;
  const wchar_t *wpVarNameStart;
  int nLines=1;
  int nVarNameLen;
  int a;
  int b;

  for (a=0, b=0; b < nInputLen; ++b)
  {
    if (wpInput[b] == L'$')
    {
      if (wpInput[++b] == L';')
      {
        while (++b < nInputLen && wpInput[b] != L'\r' && wpInput[b] != L'\0');
      }
      else if (wpInput[b] == L'$')
      {
        if (wszOutput) wszOutput[a]=L'$';
        ++a;
        if (lpHotSpot) ++lpHotSpot->nHotSpotInitLen;
      }
      else if (wpInput[b] == L'\\')
      {
        if (++b >= nInputLen) break;
        if (wszOutput) wszOutput[a]=wpInput[b];
        ++a;
        if (lpHotSpot) ++lpHotSpot->nHotSpotInitLen;
      }
      else if (wpInput[b] == L'[')
      {
        if (wszOutput) lpHotSpot=StackInsertHotSpot(hHotSpotStack, a);
      }
      else if (wpInput[b] == L'{')
      {
        wpVarNameStart=wpInput + b + 1;
        for (nVarNameLen=0; ++b < nInputLen && wpInput[b] != L'}'; ++nVarNameLen);
        if (b >= nInputLen) break;

        if (lpElement=StackGetVarByName(&lpVarThemeActive->hVarStack, wpVarNameStart, nVarNameLen))
        {
          if (wszOutput) xmemcpy(wszOutput + a, lpElement->wpVarValue, lpElement->nVarValueLen * sizeof(wchar_t));
          a+=lpElement->nVarValueLen;
          if (lpHotSpot) lpHotSpot->nHotSpotInitLen+=lpElement->nVarValueLen;
        }
      }
      continue;
    }
    else if (wpInput[b] == L'\n')
    {
      //Skip '\n'
      continue;
    }
    else if (wpInput[b] == L'\r')
    {
      ++nLines;
    }
    else if (wpInput[b] == L']')
    {
      if (lpHotSpot)
      {
        lpHotSpot->nHotSpotLen=lpHotSpot->nHotSpotInitLen;
        lpHotSpot=NULL;
        continue;
      }
    }
    if (wszOutput) wszOutput[a]=wpInput[b];
    ++a;
    if (lpHotSpot) ++lpHotSpot->nHotSpotInitLen;
  }
  if (wszOutput)
    wszOutput[a]=L'\0';
  else
    ++a;
  if (nOutputLines) *nOutputLines=nLines;
  return a;
}

TITLEINFO* StackInsertTitle(STACKTITLE *hStack)
{
  TITLEINFO *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(TITLEINFO));
  return lpElement;
}

void StackFreeTitle(STACKTITLE *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

BOOL GetEditTitlePart(SYNTAXFILE *lpSyntaxFile, wchar_t *wszTitle, int nTitleMax, INT_PTR *nMin, INT_PTR *nMax)
{
  STACKDELIM *hDelimiterStack=NULL;
  AETEXTRANGEW tr;
  AESELECTION aes;
  AECHARINDEX ciCaret;
  AECHARINDEX ciCount;
  CHARRANGE64 cr;
  int nTitleLen;

  if (bSyntaxDelimitersEnable && lpSyntaxFile)
    hDelimiterStack=&lpSyntaxFile->hDelimiterStack;

  if (!SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)&ciCaret, (LPARAM)&aes) && ciCaret.nCharInLine <= ciCaret.lpLine->nLineLen)
  {
    //Check right delimiter
    if (bRightDelimitersEnable)
    {
      if (!IsDelimiterFromRight(hDelimiterStack, hWndEdit, &aes.crSel.ciMin))
        return FALSE;
    }
    SendMessage(hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
    nTitleLen=0;
    ciCount=aes.crSel.ciMin;

    while (AEC_PrevCharInLine(&ciCount))
    {
      if (IsDelimiter(hDelimiterStack, hWndEdit, *(ciCount.lpLine->wpLine + ciCount.nCharInLine)))
      {
        AEC_NextCharInLine(&ciCount);
        break;
      }
      ++nTitleLen;
    }

    if (nTitleLen)
    {
      tr.cr.ciMin=ciCount;
      tr.cr.ciMax=aes.crSel.ciMax;
      tr.bColumnSel=FALSE;
      tr.pBuffer=wszTitle;
      tr.dwBufferMax=nTitleMax;
      tr.nNewLine=AELB_ASOUTPUT;
      tr.bFillSpaces=FALSE;
      nTitleLen=(int)SendMessage(hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);

      if (nTitleLen + 1 < nTitleMax)
      {
        *nMin=cr.cpMin - nTitleLen;
        *nMax=cr.cpMin;
        return TRUE;
      }
    }

    //Empty word
    wszTitle[0]=L'\0';
    *nMin=cr.cpMin;
    *nMax=cr.cpMin;
    return TRUE;
  }
  return FALSE;
}

void CompleteTitlePart(SYNTAXFILE *lpSyntaxFile, BLOCKINFO *lpBlockInfo, INT_PTR nMin, INT_PTR nMax)
{
  STACKDELIM *hDelimiterStack=NULL;
  AEFINDTEXTW ft;
  AECHARINDEX ciChar;
  AECHARRANGE aecr;
  CHARRANGE64 cr;
  GETTEXTRANGE gtr;
  BLOCKINFO *lpBlockMaster;
  WORDINFO *lpWordInfo=NULL;
  BLOCKINFOHANDLE *lpBlockHandle;
  HOTSPOT *lpHotSpot;
  const wchar_t *wpTitleCaret;
  const wchar_t *wpStrInit;
  const wchar_t *wpStrBegin;
  const wchar_t *wpStrEnd;
  wchar_t *wpLine;
  wchar_t *wpIndentBlock=NULL;
  int nMinLine;
  INT_PTR nMinLineIndex;
  int nLen;
  int nSpaceCount=0;
  int a;
  int b;
  BOOL bReplaced=FALSE;

  if (hWndEdit=GetFocusEdit())
  {
    if (bSyntaxDelimitersEnable)
    {
      if (!lpSyntaxFile)
        lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWndEdit, NULL, NULL);
      if (lpSyntaxFile)
        hDelimiterStack=&lpSyntaxFile->hDelimiterStack;
    }

    if (!(SendMessage(hWndEdit, AEM_GETOPTIONS, 0, 0) & AECO_READONLY))
    {
      bCompletingTitle=TRUE;

      if (lpBlockInfo->master)
        lpBlockMaster=lpBlockInfo->master;
      else
        lpBlockMaster=lpBlockInfo;
      if (lpBlockInfo->dwStructType & BIT_HLBASE)
        lpWordInfo=(WORDINFO *)lpBlockInfo->lpRef;

      if (lpBlockInfo->dwStructType & BIT_BLOCK)
      {
        if (lpCurrentBlockElement)
          StackResetHotSpot(lpCurrentBlockElement);
        lpCurrentBlockElement=NULL;
      }
      ft.dwFlags=0;

      //Smart complete multiple abbreviations, like $~GetAkelDir $~AkelPad.GetAkelDir.
      //Avoid expanding "AkelPad.GetAkel" to "AkelPad.AkelPad.GetAkelDir();".
      for (lpBlockHandle=(BLOCKINFOHANDLE *)lpBlockMaster->firstHandle; lpBlockHandle; lpBlockHandle=lpBlockHandle->next)
      {
        if (lpBlockHandle->lpBlockInfo->nTitleLen > lpBlockInfo->nTitleLen)
        {
          if (xstrstrW(lpBlockHandle->lpBlockInfo->wpTitle, lpBlockHandle->lpBlockInfo->nTitleLen, lpBlockInfo->wpTitle, lpBlockInfo->nTitleLen, FALSE, &wpStrBegin, &wpStrEnd))
          {
            ft.pText=lpBlockHandle->lpBlockInfo->wpTitle;
            ft.dwTextLen=wpStrBegin - lpBlockHandle->lpBlockInfo->wpTitle;

            if (ft.dwTextLen < (UINT_PTR)nMin)
            {
              SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, nMin - ft.dwTextLen, (LPARAM)&ciChar);

              if (IsMatch(&ft, &ciChar))
              {
                nMin-=ft.dwTextLen;
                lpBlockInfo=lpBlockHandle->lpBlockInfo;
                break;
              }
            }
          }
        }
      }
      if (nMax - nMin > lpBlockInfo->nTitleLen)
        return;
      wpTitleCaret=lpBlockInfo->wpTitle + (nMax - nMin);

      //Smart complete block abbreviations (from left).
      //Avoid expanding "<td" to "<<td".
      if (nMax - nMin > 0)
      {
        if (xstrstrW(lpBlockMaster->wpBlock, lpBlockMaster->nBlockLen, lpBlockInfo->wpTitle, (int)(nMax - nMin), FALSE, &wpStrBegin, &wpStrEnd))
        {
          ft.pText=lpBlockMaster->wpBlock;
          ft.dwTextLen=wpStrBegin - lpBlockMaster->wpBlock;

          if (ft.dwTextLen <= (UINT_PTR)nMin)
          {
            SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, nMin - ft.dwTextLen, (LPARAM)&ciChar);

            if (IsMatch(&ft, &ciChar))
            {
              nMin-=ft.dwTextLen;
            }
          }
        }
      }

      //Smart complete block abbreviations (from right).
      //Avoid expanding "Ake|lPa " to "AkelPad|lPa ".
      //Expand depending on delimiter: "Ake|lPa " to "AkelPad| ", but not "Ake|lParam" to "AkelPad|ram".
      wpStrBegin=lpBlockMaster->wpBlock;
      if (lpBlockMaster->nBlockLen <= lpBlockInfo->nTitleLen || xstrstrW(lpBlockMaster->wpBlock, lpBlockMaster->nBlockLen, lpBlockInfo->wpTitle, lpBlockInfo->nTitleLen, FALSE, &wpStrBegin, &wpStrEnd))
      {
        wpStrInit=wpStrBegin + (wpTitleCaret - lpBlockInfo->wpTitle);
        wpStrBegin=wpStrInit;
        wpStrEnd=lpBlockMaster->wpBlock + lpBlockMaster->nBlockLen;
        SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, nMax, (LPARAM)&ciChar);

        //Check full block match from right
        ft.pText=wpStrBegin;
        ft.dwTextLen=wpStrEnd - wpStrBegin;

        if (IsMatch(&ft, &ciChar))
        {
          nMax+=ft.dwTextLen;
        }
        else
        {
          //Check partial (until delimiter) block match from right
          do
          {
            if (IsDelimiterFromRight(hDelimiterStack, hWndEdit, &ciChar))
              break;
            if (WideCharLower((wchar_t)AEC_CharAtIndex(&ciChar)) != WideCharLower(*wpStrBegin) || ++wpStrBegin > wpStrEnd)
            {
              wpStrInit=NULL;
              break;
            }
          }
          while (AEC_NextCharInLine(&ciChar));

          if (wpStrInit)
            nMax+=wpStrBegin - wpStrInit;
        }
      }

      if (lpBlockMaster->nLinesInBlock > 1)
      {
        nMinLine=(int)SendMessage(hWndEdit, EM_EXLINEFROMCHAR, 0, nMin);
        nMinLineIndex=SendMessage(hWndEdit, EM_LINEINDEX, nMinLine, 0);

        if (nMinLineIndex != nMin)
        {
          gtr.cpMin=nMinLineIndex;
          gtr.cpMax=nMin;

          //Get line before word
          if (nLen=(int)SendMessage(hMainWnd, AKD_GETTEXTRANGEW, (WPARAM)hWndEdit, (LPARAM)&gtr))
          {
            wpLine=(wchar_t *)gtr.pText;

            //Get spaces
            for (nSpaceCount=0; wpLine[nSpaceCount] == L' ' || wpLine[nSpaceCount] == L'\t'; ++nSpaceCount);
            wpLine[nSpaceCount]=L'\0';

            if (nSpaceCount)
            {
              nLen=lpBlockMaster->nBlockLen + (lpBlockMaster->nLinesInBlock - 1) * nSpaceCount;

              if (wpIndentBlock=(wchar_t *)GlobalAlloc(GPTR, nLen * sizeof(wchar_t) + 2))
              {
                for (a=0, b=0; b < lpBlockMaster->nBlockLen;)
                {
                  if (lpBlockMaster->wpBlock[b] == L'\r')
                  {
                    wpIndentBlock[a++]=lpBlockMaster->wpBlock[b++];

                    for (lpHotSpot=(HOTSPOT *)lpBlockMaster->hHotSpotStack.first; lpHotSpot; lpHotSpot=lpHotSpot->next)
                    {
                      if (lpHotSpot->nHotSpotPos >= a)
                      {
                        lpHotSpot->nHotSpotPos+=nSpaceCount;
                      }
                    }
                    xstrcpyW(wpIndentBlock + a, wpLine);
                    a+=nSpaceCount;
                  }
                  else wpIndentBlock[a++]=lpBlockMaster->wpBlock[b++];
                }
                wpIndentBlock[a]=L'\0';
              }
            }
            SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)gtr.pText);
          }
        }
      }
      if (!wpIndentBlock)
        wpIndentBlock=(wchar_t *)lpBlockMaster->wpBlock;

      if (bSaveTypedCaseOnce > -1 ? bSaveTypedCaseOnce :
                                   (bSaveTypedCase && ((lpBlockInfo->dwStructType & BIT_DOCWORD) ||
                                                       ((lpBlockInfo->dwStructType & BIT_HLBASE) && !(lpWordInfo->dwFlags & AEHLF_MATCHCASE)))))
      {
        CONVERTCASE cc;
        wchar_t *wpReplaceWith=wpIndentBlock;

        SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, nMin, (LPARAM)&aecr.ciMin);
        SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, nMax, (LPARAM)&aecr.ciMax);

        cc.wszText=NULL;
        if (bInheritTypedCase)
        {
          cc.nCase=(int)SendMessage(hMainWnd, AKD_DETECTCASE, (WPARAM)hWndEdit, (LPARAM)&aecr);
          if (cc.nCase != SCT_NONE)
          {
            cc.nTextLen=xstrlenW(wpReplaceWith);
            if (cc.wszText=(wchar_t *)GlobalAlloc(GPTR, cc.nTextLen * sizeof(wchar_t) + 2))
            {
              xmemcpy(cc.wszText, wpReplaceWith, cc.nTextLen * sizeof(wchar_t) + 2);
              SendMessage(hMainWnd, AKD_CONVERTCASE, 0, (LPARAM)&cc);
              wpReplaceWith=cc.wszText;
            }
          }
        }
        cr.cpMin=nMax;
        cr.cpMax=nMax;
        SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
        SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndEdit, (LPARAM)(wpReplaceWith + (nMax - nMin)));
        bReplaced=TRUE;
        if (cc.wszText) GlobalFree((HGLOBAL)cc.wszText);
      }
      if (!bReplaced)
      {
        cr.cpMin=nMin;
        cr.cpMax=nMax;
        SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
        SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndEdit, (LPARAM)wpIndentBlock);
      }

      if (lpHotSpot=(HOTSPOT *)lpBlockMaster->hHotSpotStack.first)
      {
        cr.cpMin=nMin + lpHotSpot->nHotSpotPos;
        cr.cpMax=nMin + lpHotSpot->nHotSpotPos + lpHotSpot->nHotSpotLen;
        SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
      }
      lpBlockMaster->nHotSpotBlockBegin=nMin;
      if (!lpCurrentBlockElement)
        lpCurrentBlockElement=lpBlockMaster;

      if (wpIndentBlock != lpBlockMaster->wpBlock)
        GlobalFree((HGLOBAL)wpIndentBlock);
      bCompletingTitle=FALSE;
    }
    else MessageBeep(MB_OK);
  }
}

BLOCKINFO* StackInsertExactBlock(STACKBLOCK *hStack)
{
  BLOCKINFO *lpElementNew=NULL;

  if (!StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)NULL, (stack **)&lpElementNew, sizeof(BLOCKINFO)))
    lpElementNew->dwStructType=BIT_BLOCK;
  return lpElementNew;
}

BLOCKINFO* StackInsertBlock(STACKBLOCK *hStack, STACKBLOCKORDER *hOrderStack, wchar_t *wpTitle)
{
  BLOCKINFO *lpElement=(BLOCKINFO *)hStack->first;
  BLOCKINFO *lpElementNew=NULL;
  BLOCKORDER *lpBlockOrder=NULL;
  wchar_t wchFirstLowerChar=WideCharLower(*wpTitle);

  if (wchFirstLowerChar < FIRST_NONLATIN)
  {
    if (hStack->lpSorted[wchFirstLowerChar])
      lpElement=(BLOCKINFO *)hStack->lpSorted[wchFirstLowerChar];
    else
      lpElement=(BLOCKINFO *)hStack->first;
  }
  else lpElement=(BLOCKINFO *)hStack->lpSorted[FIRST_NONLATIN];

  while (lpElement)
  {
    if (lpElement->wchFirstLowerChar >= wchFirstLowerChar)
    {
      if (CompleteStrCmp(lpElement->dwTitleFlags, lpElement->wpTitle, wpTitle) >= 0)
        break;
    }
    lpElement=lpElement->next;
  }
  StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement, (stack **)&lpElementNew, sizeof(BLOCKINFO));

  if (lpElementNew)
  {
    if (wchFirstLowerChar < FIRST_NONLATIN)
    {
      if (!hStack->lpSorted[wchFirstLowerChar] || (INT_PTR)lpElement == hStack->lpSorted[wchFirstLowerChar])
        hStack->lpSorted[wchFirstLowerChar]=(INT_PTR)lpElementNew;
    }
    else
    {
      if (!hStack->lpSorted[FIRST_NONLATIN] || (INT_PTR)lpElement == hStack->lpSorted[FIRST_NONLATIN])
        hStack->lpSorted[FIRST_NONLATIN]=(INT_PTR)lpElementNew;
    }
    lpElementNew->dwStructType=BIT_BLOCK;
    lpElementNew->wchFirstLowerChar=wchFirstLowerChar;

    //Remember initial order
    if (!StackInsertIndex((stack **)&hOrderStack->first, (stack **)&hOrderStack->last, (stack **)&lpBlockOrder, -1, sizeof(BLOCKORDER)))
      lpBlockOrder->lpBlockInfo=lpElementNew;
  }
  return lpElementNew;
}

BLOCKINFO* StackGetExactBlock(SYNTAXFILE *lpSyntaxFile, AECHARINDEX *ciCaret, INT_PTR nCaretOffset, INT_PTR *nMin, INT_PTR *nMax)
{
  BLOCKINFO *lpBlockInfo;
  AECHARINDEX ciPrevChar;
  AECHARINDEX ciCount;
  int nTitleCount;
  int nChar;

  if (lpSyntaxFile)
  {
    if (!AEC_PrevCharInLineEx(ciCaret, &ciPrevChar))
      return NULL;

    for (lpBlockInfo=(BLOCKINFO *)lpSyntaxFile->hExactBlockStack.first; lpBlockInfo; lpBlockInfo=lpBlockInfo->next)
    {
      nTitleCount=lpBlockInfo->nTitleLen - 1;
      ciCount=ciPrevChar;

      while ((nChar=AEC_CharAtIndex(&ciCount)) >= 0 && nTitleCount >= 0)
      {
        if (WideCharLower(lpBlockInfo->wpTitle[nTitleCount]) != WideCharLower((wchar_t)nChar))
          break;
        --nTitleCount;
        if (!AEC_PrevCharInLine(&ciCount))
          break;
      }
      if (nTitleCount < 0)
      {
        *nMin=nCaretOffset - lpBlockInfo->nTitleLen;
        *nMax=nCaretOffset;
        return lpBlockInfo;
      }
    }
  }
  return NULL;
}

BLOCKINFO* StackGetBlock(SYNTAXFILE *lpSyntaxFile, STACKDOCWORDS *hDocWordsStack, const wchar_t *wpTitlePart, int nTitlePartLen, BOOL *bOnlyOne)
{
  BLOCKINFO *lpBlockInfo;
  wchar_t wchFirstChar=CompleteFirstChar(*wpTitlePart);
  wchar_t wchFirstLowerChar=WideCharLower(*wpTitlePart);

  if (lpSyntaxFile)
  {
    //Blocks
    if (wchFirstLowerChar < FIRST_NONLATIN)
      lpBlockInfo=(BLOCKINFO *)lpSyntaxFile->hBlockStack.lpSorted[wchFirstLowerChar];
    else
      lpBlockInfo=(BLOCKINFO *)lpSyntaxFile->hBlockStack.lpSorted[FIRST_NONLATIN];

    while (lpBlockInfo)
    {
      if (wchFirstLowerChar < FIRST_NONLATIN && wchFirstLowerChar != lpBlockInfo->wchFirstLowerChar)
        break;

      if (nTitlePartLen <= lpBlockInfo->nTitleLen)
      {
        if (!CompleteStrCmpLen(lpBlockInfo->dwTitleFlags, wpTitlePart, lpBlockInfo->wpTitle, nTitlePartLen))
        {
          if (bOnlyOne)
          {
            if (lpBlockInfo->next && !CompleteStrCmpLen(lpBlockInfo->next->dwTitleFlags, wpTitlePart, lpBlockInfo->next->wpTitle, nTitlePartLen))
              *bOnlyOne=FALSE;
            else
              *bOnlyOne=TRUE;
          }
          return lpBlockInfo;
        }
      }
      lpBlockInfo=lpBlockInfo->next;
    }

    //HighLight words
    if (bAddHighLightWords)
    {
      WORDALPHA *lpWordAlpha;

      if (wchFirstChar < FIRST_NONLATIN)
        lpWordAlpha=(WORDALPHA *)lpSyntaxFile->hWordAlphaStack.lpSorted[wchFirstChar];
      else
        lpWordAlpha=(WORDALPHA *)lpSyntaxFile->hWordAlphaStack.lpSorted[FIRST_NONLATIN];

      while (lpWordAlpha)
      {
        if (wchFirstChar < FIRST_NONLATIN && wchFirstChar != lpWordAlpha->wchFirstChar)
          break;

        if (nTitlePartLen <= lpWordAlpha->nWordLen)
        {
          if (!CompleteStrCmpLen(0, wpTitlePart, lpWordAlpha->wpWord, nTitlePartLen))
          {
            if (bOnlyOne)
            {
              if (lpWordAlpha->next && !CompleteStrCmpLen(0, wpTitlePart, lpWordAlpha->next->wpWord, nTitlePartLen))
                *bOnlyOne=FALSE;
              else
                *bOnlyOne=TRUE;
              if (*bOnlyOne)
              {
                //Skip exact matching word
                if (!CompleteStrCmp(0, wpTitlePart, lpWordAlpha->wpWord))
                  break;
              }
            }
            biHighLightBlock.dwStructType=BIT_HLBASE;
            if (dwCompleteListSymbolMark == CLSM_NOMARK)
            {
              biHighLightBlock.wpTitle=lpWordAlpha->wpWord;
              biHighLightBlock.nTitleLen=lpWordAlpha->nWordLen;
            }
            else
            {
              biHighLightBlock.wpTitle=wszHighLightTitle;
              biHighLightBlock.nTitleLen=(int)xprintfW(wszHighLightTitle, L"%s*", lpWordAlpha->wpWord);
            }
            biHighLightBlock.wpBlock=lpWordAlpha->wpWord;
            biHighLightBlock.nBlockLen=lpWordAlpha->nWordLen;
            biHighLightBlock.nLinesInBlock=1;
            biHighLightBlock.lpRef=lpWordAlpha->lpWordInfo;
            return &biHighLightBlock;
          }
        }
        lpWordAlpha=lpWordAlpha->next;
      }
    }
  }

  //Document words
  if (bAddDocumentWords)
  {
    DOCWORDINFO *lpDocWord;

    if (wchFirstChar < FIRST_NONLATIN)
      lpDocWord=(DOCWORDINFO *)hDocWordsStack->lpSorted[wchFirstChar];
    else
      lpDocWord=(DOCWORDINFO *)hDocWordsStack->lpSorted[FIRST_NONLATIN];

    while (lpDocWord)
    {
      if (wchFirstChar < FIRST_NONLATIN && wchFirstChar != lpDocWord->wchFirstChar)
        break;

      if (nTitlePartLen <= lpDocWord->nDocWordLen)
      {
        if (!CompleteStrCmpLen(0, wpTitlePart, lpDocWord->wpDocWord, nTitlePartLen))
        {
          if (bOnlyOne)
          {
            if (lpDocWord->next && !CompleteStrCmpLen(0, wpTitlePart, lpDocWord->next->wpDocWord, nTitlePartLen))
              *bOnlyOne=FALSE;
            else
              *bOnlyOne=TRUE;
            if (*bOnlyOne)
            {
              //Skip exact matching word
              if (!CompleteStrCmp(0, wpTitlePart, lpDocWord->wpDocWord))
                break;
            }
          }
          biDocWordBlock.dwStructType=BIT_DOCWORD;
          if (dwCompleteListSymbolMark == CLSM_NOMARK)
          {
            biDocWordBlock.wpTitle=lpDocWord->wpDocWord;
            biDocWordBlock.nTitleLen=lpDocWord->nDocWordLen;
          }
          else
          {
            biDocWordBlock.wpTitle=wszDocWordTitle;
            biDocWordBlock.nTitleLen=(int)xprintfW(wszDocWordTitle, L"%s+", lpDocWord->wpDocWord);
          }
          biDocWordBlock.wpBlock=lpDocWord->wpDocWord;
          biDocWordBlock.nBlockLen=lpDocWord->nDocWordLen;
          biDocWordBlock.nLinesInBlock=1;
          biDocWordBlock.lpRef=lpDocWord;
          return &biDocWordBlock;
        }
      }
      lpDocWord=lpDocWord->next;
    }
  }
  return NULL;
}

void StackFreeBlock(STACKBLOCK *hStack, STACKBLOCKORDER *hOrderStack)
{
  BLOCKINFO *lpElement=(BLOCKINFO *)hStack->first;

  while (lpElement)
  {
    GlobalFree((HGLOBAL)lpElement->wpTitle);
    if (lpElement->dwTitleFlags & TF_REGEXP)
    {
      PatFree(lpElement->sregTitle);
      GlobalFree((HGLOBAL)lpElement->sregTitle);
    }
    if (!lpElement->master)
    {
      StackClear((stack **)&lpElement->firstHandle, (stack **)&lpElement->lastHandle);
      GlobalFree((HGLOBAL)lpElement->wpBlock);
      StackFreeHotSpot(&lpElement->hHotSpotStack);
    }
    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);

  if (hOrderStack)
    StackClear((stack **)&hOrderStack->first, (stack **)&hOrderStack->last);
}

HOTSPOT* StackInsertHotSpot(HSTACK *hStack, int nHotSpotPos)
{
  HOTSPOT *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(HOTSPOT)))
  {
    lpElement->nHotSpotInitPos=nHotSpotPos;
    lpElement->nHotSpotInitLen=0;
    lpElement->nHotSpotPos=nHotSpotPos;
    lpElement->nHotSpotLen=0;
  }
  return lpElement;
}

HOTSPOT* GetHotSpot(BLOCKINFO *lpBlockInfo, HWND hWnd, CHARRANGE64 *cr)
{
  HOTSPOT *lpHotSpot=(HOTSPOT *)lpBlockInfo->hHotSpotStack.first;

  while (lpHotSpot)
  {
    if (lpHotSpot->nHotSpotPos >= 0)
    {
      if (cr->cpMin >= lpBlockInfo->nHotSpotBlockBegin + lpHotSpot->nHotSpotPos &&
          cr->cpMax <= lpBlockInfo->nHotSpotBlockBegin + lpHotSpot->nHotSpotPos + lpHotSpot->nHotSpotLen)
      {
        break;
      }
    }
    lpHotSpot=lpHotSpot->next;
  }
  return lpHotSpot;
}

BOOL NextHotSpot(BLOCKINFO *lpBlockInfo, HWND hWnd, BOOL bPrev)
{
  HOTSPOT *lpHotSpot;
  CHARRANGE64 crOld;
  CHARRANGE64 crNew;

  SendMessage(hWnd, EM_EXGETSEL64, 0, (LPARAM)&crOld);

  if (lpHotSpot=GetHotSpot(lpBlockInfo, hWnd, &crOld))
  {
    if (!bPrev)
    {
      if (lpHotSpot && lpHotSpot->next)
        lpHotSpot=lpHotSpot->next;
      else
        lpHotSpot=(HOTSPOT *)lpBlockInfo->hHotSpotStack.first;
    }
    else
    {
      if (lpHotSpot && lpHotSpot->prev)
        lpHotSpot=lpHotSpot->prev;
      else
        lpHotSpot=(HOTSPOT *)lpBlockInfo->hHotSpotStack.last;
    }

    if (lpHotSpot)
    {
      crNew.cpMin=lpBlockInfo->nHotSpotBlockBegin + lpHotSpot->nHotSpotPos;
      crNew.cpMax=lpBlockInfo->nHotSpotBlockBegin + lpHotSpot->nHotSpotPos + lpHotSpot->nHotSpotLen;
      if (crOld.cpMin != crNew.cpMin || crOld.cpMax != crNew.cpMax)
      {
        SendMessage(hWnd, EM_EXSETSEL64, 0, (LPARAM)&crNew);
        return TRUE;
      }
    }
  }
  return FALSE;
}

void StackResetHotSpot(BLOCKINFO *lpBlockInfo)
{
  HOTSPOT *lpElement=(HOTSPOT *)lpBlockInfo->hHotSpotStack.first;

  while (lpElement)
  {
    lpElement->nHotSpotPos=lpElement->nHotSpotInitPos;
    lpElement->nHotSpotLen=lpElement->nHotSpotInitLen;

    lpElement=lpElement->next;
  }
}

void StackFreeHotSpot(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void StackFillDocWord(SYNTAXFILE *lpSyntaxFile, STACKDOCWORDS *hDocWordsStack, const wchar_t *wpTitlePart, int nTitlePartLen)
{
  STACKDELIM *hDelimiterStack=NULL;
  DOCWORDINFO *lpDocWordInfo;
  AEFINDTEXTW ft;
  AECHARINDEX ciCaret;
  AECHARINDEX ciCount;
  int nDocWordLen;
  BOOL bFound;

  StackFreeDocWord(hDocWordsStack);

  if (hWndEdit=GetFocusEdit())
  {
    if (bSyntaxDelimitersEnable && lpSyntaxFile)
      hDelimiterStack=&lpSyntaxFile->hDelimiterStack;

    SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&ciCount);
    SendMessage(hWndEdit, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);

    ft.dwFlags=0;
    ft.pText=wpTitlePart;
    ft.dwTextLen=nTitlePartLen;
    ft.nNewLine=AELB_R;

    for (;;)
    {
      if (IsMatch(&ft, &ciCount))
      {
        if (IsDelimiterFromLeft(hDelimiterStack, hWndEdit, &ft.crFound.ciMin))
        {
          //Find word end
          ciCount=ft.crFound.ciMax;
          nDocWordLen=(int)ft.dwTextLen;
          lpDocWordInfo=NULL;
          bFound=FALSE;

          do
          {
            if (IsDelimiterFromRight(hDelimiterStack, hWndEdit, &ciCount))
            {
              if (ft.crFound.ciMin.nLine == ft.crFound.ciMax.nLine &&
                  (AEC_IndexCompare(&ciCaret, &ft.crFound.ciMin) < 0 ||
                   AEC_IndexCompare(&ciCaret, &ft.crFound.ciMax) > 0))
              {
                xstrcpynW(wszBuffer, ft.crFound.ciMin.lpLine->wpLine + ft.crFound.ciMin.nCharInLine, nDocWordLen + 1);
                lpDocWordInfo=StackGetDocWord(hDocWordsStack, wszBuffer, nDocWordLen);
                bFound=TRUE;
              }
              break;
            }
            if (++nDocWordLen >= BUFFER_SIZE) break;
          }
          while (AEC_NextCharEx(&ciCount, &ciCount));

          if (bFound && !lpDocWordInfo)
          {
            //Add new word to list
            if (lpDocWordInfo=StackInsertDocWord(hDocWordsStack, wszBuffer, nDocWordLen))
            {
              if (lpDocWordInfo->wpDocWord=(wchar_t *)GlobalAlloc(GPTR, (nDocWordLen + 1) * sizeof(wchar_t)))
                xmemcpy(lpDocWordInfo->wpDocWord, wszBuffer, (nDocWordLen + 1) * sizeof(wchar_t));
              lpDocWordInfo->nDocWordLen=nDocWordLen;
            }
          }
        }
      }

      //Next char
      AEC_IndexInc(&ciCount);
      if (ciCount.nCharInLine >= ciCount.lpLine->nLineLen)
        if (!AEC_NextLine(&ciCount))
          break;
    }
  }
}

DOCWORDINFO* StackInsertDocWord(STACKDOCWORDS *hStack, wchar_t *wpDocWord, int nDocWordLen)
{
  DOCWORDINFO *lpElement=hStack->first;
  DOCWORDINFO *lpElementNew=NULL;
  wchar_t wchFirstChar=CompleteFirstChar(*wpDocWord);

  if (wchFirstChar < FIRST_NONLATIN)
  {
    if (hStack->lpSorted[wchFirstChar])
      lpElement=(DOCWORDINFO *)hStack->lpSorted[wchFirstChar];
    else
      lpElement=(DOCWORDINFO *)hStack->first;
  }
  else lpElement=(DOCWORDINFO *)hStack->lpSorted[FIRST_NONLATIN];

  while (lpElement)
  {
    if (lpElement->wchFirstChar >= wchFirstChar)
    {
      if (CompleteStrCmp(0, lpElement->wpDocWord, wpDocWord) >= 0)
        break;
    }
    lpElement=lpElement->next;
  }
  StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement, (stack **)&lpElementNew, sizeof(DOCWORDINFO));

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

    lpElementNew->dwStructType=BIT_DOCWORD;
    lpElementNew->wchFirstChar=wchFirstChar;
  }
  return lpElementNew;
}

DOCWORDINFO* StackGetDocWord(STACKDOCWORDS *hStack, const wchar_t *wpDocWord, int nDocWordLen)
{
  DOCWORDINFO *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->nDocWordLen == nDocWordLen)
    {
      if (!CompleteStrCmp(0, wpDocWord, lpElement->wpDocWord))
        return lpElement;
    }
  }
  return NULL;
}

void StackFreeDocWord(STACKDOCWORDS *hStack)
{
  DOCWORDINFO *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    GlobalFree((HGLOBAL)lpElement->wpDocWord);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  xmemset(hStack->lpSorted, 0, sizeof(hStack->lpSorted));
}


//// Options

void ReadAutoCompleteOptions(HANDLE hOptions)
{
  WideOption(hOptions, L"AutoListEnable", PO_DWORD, (LPBYTE)&bAutoListEnable, sizeof(DWORD));
  WideOption(hOptions, L"AutoListAfter", PO_DWORD, (LPBYTE)&nAutoListAfter, sizeof(DWORD));
  WideOption(hOptions, L"AlphaListEnable", PO_DWORD, (LPBYTE)&bAlphaListEnable, sizeof(DWORD));
  WideOption(hOptions, L"AlphaListValue", PO_DWORD, (LPBYTE)&nAlphaListValue, sizeof(DWORD));
  WideOption(hOptions, L"AutoCompleteWindowRect", PO_BINARY, (LPBYTE)&rcAutoComplete, sizeof(RECT));
  WideOption(hOptions, L"CompleteWithList", PO_DWORD, (LPBYTE)&dwCompleteWithList, sizeof(DWORD));
  WideOption(hOptions, L"CompleteWithoutList", PO_DWORD, (LPBYTE)&dwCompleteWithoutList, sizeof(DWORD));
  WideOption(hOptions, L"CompleteWithoutListAfter", PO_DWORD, (LPBYTE)&nCompleteWithoutListAfter, sizeof(DWORD));
  WideOption(hOptions, L"OneWithoutListAndCompleteNext", PO_DWORD, (LPBYTE)&bOneWithoutListAndCompleteNext, sizeof(DWORD));
  WideOption(hOptions, L"CompleteNext", PO_DWORD, (LPBYTE)&dwCompleteNext, sizeof(DWORD));
  WideOption(hOptions, L"CompletePrev", PO_DWORD, (LPBYTE)&dwCompletePrev, sizeof(DWORD));
  WideOption(hOptions, L"AddDocumentWords", PO_DWORD, (LPBYTE)&bAddDocumentWords, sizeof(DWORD));
  WideOption(hOptions, L"CompleteNonSyntaxDocument", PO_DWORD, (LPBYTE)&bCompleteNonSyntaxDocument, sizeof(DWORD));
  WideOption(hOptions, L"SaveTypedCase", PO_DWORD, (LPBYTE)&bSaveTypedCase, sizeof(DWORD));
  WideOption(hOptions, L"InheritTypedCase", PO_DWORD, (LPBYTE)&bInheritTypedCase, sizeof(DWORD));
  WideOption(hOptions, L"MaxDocumentEnable", PO_DWORD, (LPBYTE)&bMaxDocumentEnable, sizeof(DWORD));
  WideOption(hOptions, L"MaxDocumentChars", PO_DWORD, (LPBYTE)&nMaxDocumentChars, sizeof(DWORD));
  WideOption(hOptions, L"AddHighLightWords", PO_DWORD, (LPBYTE)&bAddHighLightWords, sizeof(DWORD));
  WideOption(hOptions, L"CompleteListItemHlBaseColors", PO_DWORD, (LPBYTE)&bCompleteListItemHlBaseColors, sizeof(DWORD));
  WideOption(hOptions, L"CompleteListSystemColors", PO_DWORD, (LPBYTE)&bCompleteListSystemColors, sizeof(DWORD));
  WideOption(hOptions, L"CompleteListSymbolMark", PO_DWORD, (LPBYTE)&dwCompleteListSymbolMark, sizeof(DWORD));
  WideOption(hOptions, L"RightDelimitersEnable", PO_DWORD, (LPBYTE)&bRightDelimitersEnable, sizeof(DWORD));
  WideOption(hOptions, L"SyntaxDelimitersEnable", PO_DWORD, (LPBYTE)&bSyntaxDelimitersEnable, sizeof(DWORD));
  WideOption(hOptions, L"CompleteCaseSensitive", PO_DWORD, (LPBYTE)&bCompleteCaseSensitive, sizeof(DWORD));
}

void SaveAutoCompleteOptions(HANDLE hOptions, DWORD dwFlags)
{
  if (dwFlags & OF_AUTOCOMPLETE_RECT)
  {
    WideOption(hOptions, L"AutoCompleteWindowRect", PO_BINARY, (LPBYTE)&rcAutoComplete, sizeof(RECT));
  }
  if (dwFlags & OF_AUTOCOMPLETE_SETTINGS)
  {
    WideOption(hOptions, L"AutoListEnable", PO_DWORD, (LPBYTE)&bAutoListEnable, sizeof(DWORD));
    WideOption(hOptions, L"AutoListAfter", PO_DWORD, (LPBYTE)&nAutoListAfter, sizeof(DWORD));
    WideOption(hOptions, L"AlphaListEnable", PO_DWORD, (LPBYTE)&bAlphaListEnable, sizeof(DWORD));
    WideOption(hOptions, L"AlphaListValue", PO_DWORD, (LPBYTE)&nAlphaListValue, sizeof(DWORD));
    WideOption(hOptions, L"CompleteWithList", PO_DWORD, (LPBYTE)&dwCompleteWithList, sizeof(DWORD));
    WideOption(hOptions, L"CompleteWithoutList", PO_DWORD, (LPBYTE)&dwCompleteWithoutList, sizeof(DWORD));
    WideOption(hOptions, L"CompleteWithoutListAfter", PO_DWORD, (LPBYTE)&nCompleteWithoutListAfter, sizeof(DWORD));
    WideOption(hOptions, L"OneWithoutListAndCompleteNext", PO_DWORD, (LPBYTE)&bOneWithoutListAndCompleteNext, sizeof(DWORD));
    WideOption(hOptions, L"CompleteNext", PO_DWORD, (LPBYTE)&dwCompleteNext, sizeof(DWORD));
    WideOption(hOptions, L"CompletePrev", PO_DWORD, (LPBYTE)&dwCompletePrev, sizeof(DWORD));
    WideOption(hOptions, L"AddDocumentWords", PO_DWORD, (LPBYTE)&bAddDocumentWords, sizeof(DWORD));
    WideOption(hOptions, L"CompleteNonSyntaxDocument", PO_DWORD, (LPBYTE)&bCompleteNonSyntaxDocument, sizeof(DWORD));
    WideOption(hOptions, L"SaveTypedCase", PO_DWORD, (LPBYTE)&bSaveTypedCase, sizeof(DWORD));
    WideOption(hOptions, L"InheritTypedCase", PO_DWORD, (LPBYTE)&bInheritTypedCase, sizeof(DWORD));
    WideOption(hOptions, L"MaxDocumentEnable", PO_DWORD, (LPBYTE)&bMaxDocumentEnable, sizeof(DWORD));
    WideOption(hOptions, L"MaxDocumentChars", PO_DWORD, (LPBYTE)&nMaxDocumentChars, sizeof(DWORD));
    WideOption(hOptions, L"AddHighLightWords", PO_DWORD, (LPBYTE)&bAddHighLightWords, sizeof(DWORD));
    WideOption(hOptions, L"CompleteListItemHlBaseColors", PO_DWORD, (LPBYTE)&bCompleteListItemHlBaseColors, sizeof(DWORD));
    WideOption(hOptions, L"CompleteListSystemColors", PO_DWORD, (LPBYTE)&bCompleteListSystemColors, sizeof(DWORD));
    WideOption(hOptions, L"CompleteListSymbolMark", PO_DWORD, (LPBYTE)&dwCompleteListSymbolMark, sizeof(DWORD));
    WideOption(hOptions, L"RightDelimitersEnable", PO_DWORD, (LPBYTE)&bRightDelimitersEnable, sizeof(DWORD));
    WideOption(hOptions, L"SyntaxDelimitersEnable", PO_DWORD, (LPBYTE)&bSyntaxDelimitersEnable, sizeof(DWORD));
    WideOption(hOptions, L"CompleteCaseSensitive", PO_DWORD, (LPBYTE)&bCompleteCaseSensitive, sizeof(DWORD));
  }
}

void InitAutoComplete()
{
  bInitAutoComplete=TRUE;

  //Add hotkeys
  {
    PLUGINADDW pa;

    pa.bAutoLoad=FALSE;
    pa.lpParameter=0;

    if (dwCompleteWithList && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCompleteWithList))
    {
      pa.pFunction=L"Coder::AutoComplete::WithList";
      pa.wHotkey=(WORD)dwCompleteWithList;
      pa.PluginProc=CompleteWithListProc;
      pfwCompleteWithList=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
    }
    if (dwCompleteWithoutList && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCompleteWithoutList))
    {
      pa.pFunction=L"Coder::AutoComplete::WithoutList";
      pa.wHotkey=(WORD)dwCompleteWithoutList;
      pa.PluginProc=CompleteWithoutListProc;
      pfwCompleteWithoutList=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
    }
    if (dwCompleteNext && !bOneWithoutListAndCompleteNext && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCompleteNext))
    {
      pa.pFunction=L"Coder::AutoComplete::Next";
      pa.wHotkey=(WORD)dwCompleteNext;
      pa.PluginProc=CompleteNextProc;
      pfwCompleteNext=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
    }
    if (dwCompletePrev && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwCompletePrev))
    {
      pa.pFunction=L"Coder::AutoComplete::Prev";
      pa.wHotkey=(WORD)dwCompletePrev;
      pa.PluginProc=CompletePrevProc;
      pfwCompletePrev=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
    }
  }

  //Register AutoComplete class
  {
    WNDCLASSW wndclass;

    wndclass.style        =0;
    wndclass.lpfnWndProc  =AutoCompleteWindowProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstanceDLL;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=AUTOCOMPLETEW;
    RegisterClassWide(&wndclass);
  }

  //Get functions addresses
  {
    HMODULE hUser32;

    hUser32=GetModuleHandleA("user32.dll");
    SetLayeredWindowAttributesPtr=(BOOL (WINAPI *)(HWND, COLORREF, BYTE, DWORD))GetProcAddress(hUser32, "SetLayeredWindowAttributes");
  }
}

void UninitAutoComplete()
{
  bInitAutoComplete=FALSE;

  //Remove hotkeys
  if (pfwCompleteWithList)
  {
    SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCompleteWithList);
    pfwCompleteWithList=NULL;
  }
  if (pfwCompleteWithoutList)
  {
    SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCompleteWithoutList);
    pfwCompleteWithoutList=NULL;
  }
  if (pfwCompleteNext)
  {
    SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCompleteNext);
    pfwCompleteNext=NULL;
  }
  if (pfwCompletePrev)
  {
    SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwCompletePrev);
    pfwCompletePrev=NULL;
  }
  CloseAutoCompleteWindow();

  UnregisterClassWide(AUTOCOMPLETEW, hInstanceDLL);

  //Save OF_AUTOCOMPLETE_RECT
  if (dwSaveFlags)
  {
    SaveOptions(dwSaveFlags);
    dwSaveFlags=0;
  }
}
