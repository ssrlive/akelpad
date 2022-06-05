#include "SettingsDlg.h"
#include "AnyWindow.h"
#include "resource.h"
#include "c_base/types.h"
#include "Plugin.h"
#include "XBracketsLng.h"
#include "XMemStrFunc.h"
#include <commctrl.h>
#include <commdlg.h>

extern HWND        g_hMainWnd;
extern HINSTANCE   g_hInstanceDLL;
extern BOOL        g_bOldWindows;
extern BOOL        g_bInitialized;
extern LANGID      g_LangSystem;

extern BOOL        bBracketsAutoComplete;
extern UINT        uBracketsHighlight;
extern BOOL        bBracketsHighlightVisibleArea;
extern BOOL        bBracketsRightExistsOK;
extern BOOL        bBracketsDoDoubleQuote;
extern BOOL        bBracketsDoSingleQuote;
extern BOOL        bBracketsDoTag;
extern BOOL        bBracketsDoTag2;
extern BOOL        bBracketsDoTagIf;
extern BOOL        bBracketsHighlightDoubleQuote;
extern BOOL        bBracketsHighlightSingleQuote;
extern BOOL        bBracketsHighlightTag;
extern BOOL        bBracketsSkipEscaped1;
extern BOOL        bBracketsSkipComment1;
extern COLORREF    bracketsColourHighlight[2];
extern COLORREF    g_CustomColoursHighlight[MAX_CUSTOM_COLOURS];
extern char        strHtmlFileExtsA[STR_FILEEXTS_SIZE];
extern wchar_t     strHtmlFileExtsW[STR_FILEEXTS_SIZE];
extern char        strEscaped1FileExtsA[STR_FILEEXTS_SIZE];
extern wchar_t     strEscaped1FileExtsW[STR_FILEEXTS_SIZE];
extern char        strComment1FileExtsA[STR_FILEEXTS_SIZE];
extern wchar_t     strComment1FileExtsW[STR_FILEEXTS_SIZE];
extern wchar_t     strPluginFuncMainW[STR_PLUGINFUNC_SIZE];

static HWND        sd_hToolTip = NULL;

UINT CheckBox_GetState(HWND hDlg, UINT idCheckBox);
BOOL CheckBox_SetState(HWND hDlg, UINT idCheckBox, UINT uState);
BOOL CheckBox_IsChecked(HWND hDlg, UINT idCheckBox);
BOOL CheckBox_SetCheck(HWND hDlg, UINT idCheckBox, BOOL bChecked);
BOOL DlgItem_EnableWindow(HWND hDlg, UINT idDlgItem, BOOL bEnable);
BOOL DlgItem_SetText(HWND hDlg, UINT idDlgItem, BOOL bUnicode, const void* pszText);

static BOOL SettingsDlg_OnOK(HWND hDlg);
static void SettingsDlg_OnBtColor(HWND hDlg, UINT uButtonID);
static void SettingsDlg_OnChBracketsAutoCompleteClicked(HWND hDlg);
static void SettingsDlg_OnChBracketsHighlightClicked(HWND hDlg);
static void SettingsDlg_OnChBracketsDoDoubleQuoteClicked(HWND hDlg);
static void SettingsDlg_OnChBracketsDoSingleQuoteClicked(HWND hDlg);
static void SettingsDlg_OnChBracketsDoTagClicked(HWND hDlg);
static void SettingsDlg_OnChBracketsDoTagIfClicked(HWND hDlg);
static void SettingsDlg_OnChBracketsSkipEscaped1(HWND hDlg);
static void SettingsDlg_OnChBracketsSkipComment1(HWND hDlg);
static void SettingsDlg_OnDrawItem(HWND hDlg, UINT uControlID, DRAWITEMSTRUCT* pDis);
static void SettingsDlg_OnStPluginStateDblClicked(HWND hDlg);
static void SettingsDlg_OnInitDialog(HWND hDlg);
static void SettingsDlg_EndDialog(HWND hDlg, INT_PTR nResult);
static HWND SettingsDlg_InitToolTip(HWND hDlg);
static void SettingsDlg_UpdateStateOfChBrackets(HWND hDlg, BOOL bInvert);

INT_PTR CALLBACK SettingsDlgProc(
  HWND   hDlg, 
  UINT   uMessage, 
  WPARAM wParam, 
  LPARAM lParam)
{
  static COLORREF bracketsColourHighlight_0[2] = { 0, 0 };

  if (uMessage == WM_COMMAND)
  {
    switch (LOWORD(wParam)) 
    {
      case IDC_CH_BRACKETS_HIGHLIGHT:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_UpdateStateOfChBrackets(hDlg, TRUE);
          SettingsDlg_OnChBracketsHighlightClicked(hDlg);
        }
        break;
      }
      case IDC_CH_BRACKETCOLOR:
      case IDC_CH_BKGNDCOLOR:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_UpdateStateOfChBrackets(hDlg, FALSE);
        }
        break;
      }
      case IDC_CH_BRACKETS_AUTOCOMPLETE:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_OnChBracketsAutoCompleteClicked(hDlg);
        }
        break;
      }
      case IDC_CH_BRACKETS_DODOUBLEQUOTE:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_OnChBracketsDoDoubleQuoteClicked(hDlg);
        }
        break;
      }
      case IDC_CH_BRACKETS_DOSINGLEQUOTE:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_OnChBracketsDoSingleQuoteClicked(hDlg);
        }
        break;
      }
      case IDC_CH_BRACKETS_DOTAG:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_OnChBracketsDoTagClicked(hDlg);
        }
        break;
      }
      case IDC_CH_BRACKETS_DOTAGIF:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_OnChBracketsDoTagIfClicked(hDlg);
        }
        break;
      }
      case IDC_CH_BRACKETS_SKIPESCAPED1:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_OnChBracketsSkipEscaped1(hDlg);
        }
        break;
      }
      case IDC_CH_BRACKETS_SKIPCOMMENT1:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          SettingsDlg_OnChBracketsSkipComment1(hDlg);
        }
        break;
      }
      case IDC_ST_PLUGINSTATE:
      {
        if (HIWORD(wParam) == STN_DBLCLK)
        {
          SettingsDlg_OnStPluginStateDblClicked(hDlg);
        }
        break;
      }
      case IDC_BT_BRACKETCOLOR:
      case IDC_BT_BKGNDCOLOR:
      {
        SettingsDlg_OnBtColor(hDlg, LOWORD(wParam));
        break;
      }
      case IDC_BT_OK: // Run
      case IDOK:
      {
        if (SettingsDlg_OnOK(hDlg))
        {
          SettingsDlg_EndDialog(hDlg, 1); // OK - returns 1
        }
        return 1;
      }
      case IDC_BT_CANCEL: // Cancel
      case IDCANCEL:
      {
        bracketsColourHighlight[0] = bracketsColourHighlight_0[0];
        bracketsColourHighlight[1] = bracketsColourHighlight_0[1];
        SettingsDlg_EndDialog(hDlg, 0); // Cancel - returns 0
        return 1;
      }
      default:
        break;
    }
  }

  else if (uMessage == WM_DRAWITEM)
  {
    SettingsDlg_OnDrawItem(hDlg, (UINT)wParam, (DRAWITEMSTRUCT*)lParam);
    return TRUE;
  }

  else if (uMessage == WM_NOTIFY)
  {
    
  }

  else if (uMessage == WM_INITDIALOG)
  {
    bracketsColourHighlight_0[0] = bracketsColourHighlight[0];
    bracketsColourHighlight_0[1] = bracketsColourHighlight[1];
    SettingsDlg_OnInitDialog(hDlg);
  }

  return 0;
}

static void settdlg_WriteOptToStr(HWND hEd, char* strOptStrA, wchar_t* strOptStrW, int nMaxStr)
{
  if (hEd)
  {
    if (g_bOldWindows)
      GetWindowTextA(hEd, strOptStrA, nMaxStr);
    else
      GetWindowTextW(hEd, strOptStrW, nMaxStr);
  }
}

BOOL SettingsDlg_OnOK(HWND hDlg)
{
  UINT uState;

  settdlg_WriteOptToStr( GetDlgItem(hDlg, IDC_ED_BRACKETS_DOTAGIF),
    strHtmlFileExtsA, strHtmlFileExtsW, STR_FILEEXTS_SIZE - 1 );

  settdlg_WriteOptToStr( GetDlgItem(hDlg, IDC_ED_BRACKETS_SKIPESCAPED1),
    strEscaped1FileExtsA, strEscaped1FileExtsW, STR_FILEEXTS_SIZE - 1 );

  settdlg_WriteOptToStr( GetDlgItem(hDlg, IDC_ED_BRACKETS_SKIPCOMMENT1),
    strComment1FileExtsA, strComment1FileExtsW, STR_FILEEXTS_SIZE - 1 );

  bBracketsAutoComplete = 
    CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_AUTOCOMPLETE);
  uBracketsHighlight = 0x00;
  if (CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_HIGHLIGHT))
    uBracketsHighlight |= BRHLF_ENABLED;
  if (CheckBox_IsChecked(hDlg, IDC_CH_BRACKETCOLOR))
    uBracketsHighlight |= BRHLF_TEXT;
  if (CheckBox_IsChecked(hDlg, IDC_CH_BKGNDCOLOR))
    uBracketsHighlight |= BRHLF_BKGND;
  bBracketsHighlightVisibleArea =
    CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_HIGHLIGHT_VISIBLEAREA);
  bBracketsRightExistsOK = 
    CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_RIGHTEXISTS_OK);

  uState = CheckBox_GetState(hDlg, IDC_CH_BRACKETS_DODOUBLEQUOTE);
  bBracketsDoDoubleQuote = (uState == BST_CHECKED);
  bBracketsHighlightDoubleQuote = (uState == BST_INDETERMINATE);

  uState = CheckBox_GetState(hDlg, IDC_CH_BRACKETS_DOSINGLEQUOTE);
  bBracketsDoSingleQuote = (uState == BST_CHECKED);
  bBracketsHighlightSingleQuote = (uState == BST_INDETERMINATE);

  uState = CheckBox_GetState(hDlg, IDC_CH_BRACKETS_DOTAG);
  bBracketsDoTag = (uState == BST_CHECKED);
  bBracketsHighlightTag = (uState == BST_INDETERMINATE);

  bBracketsDoTag2 = 
    CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_DOTAG2);
  bBracketsDoTagIf = 
    CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_DOTAGIF);
  bBracketsSkipEscaped1 =
    CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_SKIPESCAPED1);
  bBracketsSkipComment1 = 
    CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_SKIPCOMMENT1);

  return TRUE;
}

void SettingsDlg_OnBtColor(HWND hDlg, UINT uButtonID)
{
  COLORREF  localCustomColoursHighlight[MAX_CUSTOM_COLOURS];
  COLORREF* lpcrColor;
  HWND      hWndButton;
  int       i;

  x_mem_cpy(localCustomColoursHighlight, g_CustomColoursHighlight, sizeof(g_CustomColoursHighlight));

  switch (uButtonID)
  {
    case IDC_BT_BRACKETCOLOR:
      lpcrColor = &bracketsColourHighlight[0];
      hWndButton = GetDlgItem(hDlg, IDC_BT_BRACKETCOLOR);
      break;
    case IDC_BT_BKGNDCOLOR:
      lpcrColor = &bracketsColourHighlight[1];
      hWndButton = GetDlgItem(hDlg, IDC_BT_BKGNDCOLOR);
      break;
    default:
      lpcrColor = NULL;
      hWndButton = NULL;
      break;
  }

  if (lpcrColor)
  {
    i = 0;

    if (g_bOldWindows)
    {
      CHOOSECOLORA ccA;

      x_zero_mem(&ccA, sizeof(CHOOSECOLORA));

      ccA.lStructSize  = sizeof(CHOOSECOLORA);
      ccA.hwndOwner    = hDlg;
      ccA.lpCustColors = localCustomColoursHighlight;
      ccA.Flags        = CC_FULLOPEN | CC_RGBINIT;
      ccA.rgbResult    = *lpcrColor;

      if (ChooseColorA(&ccA))
      {
        *lpcrColor = ccA.rgbResult;
        i = 1;
      }
    }
    else
    {
      CHOOSECOLORW ccW;

      x_zero_mem(&ccW, sizeof(CHOOSECOLORW));

      ccW.lStructSize  = sizeof(CHOOSECOLORW);
      ccW.hwndOwner    = hDlg;
      ccW.lpCustColors = localCustomColoursHighlight;
      ccW.Flags        = CC_FULLOPEN | CC_RGBINIT;
      ccW.rgbResult    = *lpcrColor;

      if (ChooseColorW(&ccW))
      {
        *lpcrColor = ccW.rgbResult;
        i = 1;
      }
    }

    if (i != 0)
    {
      x_mem_cpy(g_CustomColoursHighlight, localCustomColoursHighlight, sizeof(localCustomColoursHighlight));

      InvalidateRect(hWndButton, NULL, FALSE);
    }
  }
}

void SettingsDlg_OnChBracketsAutoCompleteClicked(HWND hDlg)
{
  BOOL bEnable = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_AUTOCOMPLETE);
  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETS_RIGHTEXISTS_OK, bEnable);
}

void SettingsDlg_OnChBracketsHighlightClicked(HWND hDlg)
{
  BOOL bEnable = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_HIGHLIGHT);
  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETS_HIGHLIGHT_VISIBLEAREA, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETCOLOR, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_BT_BRACKETCOLOR, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_CH_BKGNDCOLOR, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_BT_BKGNDCOLOR, bEnable);
}

void SettingsDlg_OnChBracketsDoDoubleQuoteClicked(HWND hDlg)
{
  xbrSetAutocompleteWindowText(hDlg, g_LangSystem);
}

void SettingsDlg_OnChBracketsDoSingleQuoteClicked(HWND hDlg)
{
  xbrSetAutocompleteWindowText(hDlg, g_LangSystem);
}

void SettingsDlg_OnChBracketsDoTagClicked(HWND hDlg)
{
  BOOL bEnable = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_DOTAG);
  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETS_DOTAG2, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETS_DOTAGIF, bEnable);
  if (bEnable)
  {
    bEnable = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_DOTAGIF);
  }
  DlgItem_EnableWindow(hDlg, IDC_ED_BRACKETS_DOTAGIF, bEnable);
  xbrSetAutocompleteWindowText(hDlg, g_LangSystem);
}

void SettingsDlg_OnChBracketsDoTagIfClicked(HWND hDlg)
{
  BOOL bEnable = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_DOTAGIF);
  DlgItem_EnableWindow(hDlg, IDC_ED_BRACKETS_DOTAGIF, bEnable);
}

void SettingsDlg_OnChBracketsSkipEscaped1(HWND hDlg)
{
  BOOL bEnable = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_SKIPESCAPED1);
  DlgItem_EnableWindow(hDlg, IDC_ED_BRACKETS_SKIPESCAPED1, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_ST_BRACKETS_SKIPESCAPED1, bEnable);
}

void SettingsDlg_OnChBracketsSkipComment1(HWND hDlg)
{
  BOOL bEnable = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_SKIPCOMMENT1);
  DlgItem_EnableWindow(hDlg, IDC_ED_BRACKETS_SKIPCOMMENT1, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_ST_BRACKETS_SKIPCOMMENT1, bEnable);
}

void SettingsDlg_OnDrawItem(HWND hDlg, UINT uControlID, DRAWITEMSTRUCT* pDis)
{
  HBRUSH   hBrush;
  COLORREF dwColor;

  (hDlg); // unreferenced parameter

  switch (uControlID)
  {
    case IDC_BT_BRACKETCOLOR:
      dwColor = bracketsColourHighlight[0];
      break;
    case IDC_BT_BKGNDCOLOR:
      dwColor = bracketsColourHighlight[1];
      break;
    default:
      dwColor = GetSysColor(COLOR_BTNFACE);
      break;
  }

  if (hBrush = CreateSolidBrush(dwColor))
  {
    FillRect(pDis->hDC, &pDis->rcItem, hBrush);
    if (pDis->hwndItem == GetFocus())
      DrawFocusRect(pDis->hDC, &pDis->rcItem);
    DeleteObject(hBrush);
  }
}

static void showPluginStatus(HWND hDlg)
{
  if (g_bOldWindows)
  {
    DlgItem_SetText(hDlg, IDC_ST_PLUGINSTATE, FALSE,
      g_bInitialized ? xbrGetStrA(XBR_STR_PLUGINSTATUSACTIVE, g_LangSystem) : 
        xbrGetStrA(XBR_STR_PLUGINSTATUSNOTACTIVE, g_LangSystem) );
  }
  else
  {
    DlgItem_SetText(hDlg, IDC_ST_PLUGINSTATE, TRUE,
      g_bInitialized ? xbrGetStrW(XBR_STR_PLUGINSTATUSACTIVE, g_LangSystem) :
        xbrGetStrW(XBR_STR_PLUGINSTATUSNOTACTIVE, g_LangSystem) );
  }
}

void SettingsDlg_OnStPluginStateDblClicked(HWND hDlg)
{
  BOOL bDoInitializeHere = TRUE;

  if (g_bInitialized)
  {
    Uninitialize(FALSE);
    bDoInitializeHere = FALSE;
  }

  if (g_bOldWindows)
  {
    if (bDoInitializeHere)
    {
      // XBrackets::Main is unloaded, let's load it
      PluginCallXBracketsMain(g_hMainWnd, g_bOldWindows);
    }
    else
    {
      PLUGINFUNCTION* pfA = (PLUGINFUNCTION *) SendMessageA( g_hMainWnd, 
        AKD_DLLFINDA, (WPARAM) strPluginFuncMainW, 0 );
      if (pfA)
      {
        pfA->bRunning = FALSE;
      }
    }
  }
  else
  {
    if (bDoInitializeHere)
    {
      // XBrackets::Main is unloaded, let's load it
      PluginCallXBracketsMain(g_hMainWnd, g_bOldWindows);
    }
    else
    {
      PLUGINFUNCTION* pfW = (PLUGINFUNCTION *) SendMessageW( g_hMainWnd,
        AKD_DLLFINDW, (WPARAM) strPluginFuncMainW, 0 );
      if (pfW)
      {
        pfW->bRunning = FALSE;
      }
    }
  }

  showPluginStatus(hDlg);
}

static void settdlg_InitOptFromStr(HWND hEd, const char* strOptStrA, const wchar_t* strOptStrW, int nMaxStr)
{
  if (hEd)
  {
    SendMessage(hEd, EM_LIMITTEXT, (WPARAM) nMaxStr, 0);

    if (g_bOldWindows)
      SetWindowTextA(hEd, strOptStrA);
    else
      SetWindowTextW(hEd, strOptStrW);
  }
}

void SettingsDlg_OnInitDialog(HWND hDlg)
{
  BOOL bEnable;
  UINT uState;

  AnyWindow_CenterWindow(hDlg, g_hMainWnd, FALSE);

  CheckBox_SetCheck(hDlg, 
    IDC_CH_BRACKETS_AUTOCOMPLETE, bBracketsAutoComplete);
  CheckBox_SetCheck(hDlg, 
    IDC_CH_BRACKETS_RIGHTEXISTS_OK, bBracketsRightExistsOK);
  CheckBox_SetCheck(hDlg, 
    IDC_CH_BRACKETS_HIGHLIGHT, (uBracketsHighlight & BRHLF_ENABLED) != 0);
  CheckBox_SetCheck(hDlg, 
    IDC_CH_BRACKETS_HIGHLIGHT_VISIBLEAREA, bBracketsHighlightVisibleArea);
  CheckBox_SetCheck(hDlg,
    IDC_CH_BRACKETCOLOR, (uBracketsHighlight & BRHLF_TEXT) != 0);
  CheckBox_SetCheck(hDlg,
    IDC_CH_BKGNDCOLOR, (uBracketsHighlight & BRHLF_BKGND) != 0);
  SettingsDlg_UpdateStateOfChBrackets(hDlg, FALSE);

  uState = BST_UNCHECKED;
  if (bBracketsDoDoubleQuote)
    uState = BST_CHECKED;
  else if (bBracketsHighlightDoubleQuote)
    uState = BST_INDETERMINATE;
  CheckBox_SetState(hDlg, 
    IDC_CH_BRACKETS_DODOUBLEQUOTE, uState);

  uState = BST_UNCHECKED;
  if (bBracketsDoSingleQuote)
    uState = BST_CHECKED;
  else if (bBracketsHighlightSingleQuote)
    uState = BST_INDETERMINATE;
  CheckBox_SetState(hDlg,  
    IDC_CH_BRACKETS_DOSINGLEQUOTE, uState);

  uState = BST_UNCHECKED;
  if (bBracketsDoTag)
    uState = BST_CHECKED;
  else if (bBracketsHighlightTag)
    uState = BST_INDETERMINATE;
  CheckBox_SetState(hDlg, 
    IDC_CH_BRACKETS_DOTAG, uState);

  CheckBox_SetCheck(hDlg, 
    IDC_CH_BRACKETS_DOTAG2, bBracketsDoTag2);
  CheckBox_SetCheck(hDlg, 
    IDC_CH_BRACKETS_DOTAGIF, bBracketsDoTagIf);
  CheckBox_SetCheck(hDlg,
    IDC_CH_BRACKETS_SKIPESCAPED1, bBracketsSkipEscaped1);
  CheckBox_SetCheck(hDlg, 
    IDC_CH_BRACKETS_SKIPCOMMENT1, bBracketsSkipComment1);
  DlgItem_EnableWindow(hDlg, 
    IDC_CH_BRACKETS_RIGHTEXISTS_OK, bBracketsAutoComplete);
  bEnable = ((uBracketsHighlight & BRHLF_ENABLED) != 0);
  DlgItem_EnableWindow(hDlg, 
    IDC_CH_BRACKETS_HIGHLIGHT_VISIBLEAREA, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETCOLOR, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_BT_BRACKETCOLOR, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_CH_BKGNDCOLOR, bEnable);
  DlgItem_EnableWindow(hDlg, IDC_BT_BKGNDCOLOR, bEnable);
  DlgItem_EnableWindow(hDlg, 
    IDC_ED_BRACKETS_SKIPESCAPED1, bBracketsSkipEscaped1);
  DlgItem_EnableWindow(hDlg, 
    IDC_ST_BRACKETS_SKIPESCAPED1, bBracketsSkipEscaped1);
  DlgItem_EnableWindow(hDlg, 
    IDC_ED_BRACKETS_SKIPCOMMENT1, bBracketsSkipComment1);
  DlgItem_EnableWindow(hDlg, 
    IDC_ST_BRACKETS_SKIPCOMMENT1, bBracketsSkipComment1);

  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETS_DOTAG2, bBracketsDoTag || bBracketsHighlightTag);
  DlgItem_EnableWindow(hDlg, IDC_CH_BRACKETS_DOTAGIF, bBracketsDoTag || bBracketsHighlightTag);
  DlgItem_EnableWindow(hDlg, IDC_ED_BRACKETS_DOTAGIF, 
    (bBracketsDoTag || bBracketsHighlightTag) ? bBracketsDoTagIf : FALSE);

  xbrSetSettingsDlgLang(hDlg, g_LangSystem);

  showPluginStatus(hDlg);

  settdlg_InitOptFromStr( GetDlgItem(hDlg, IDC_ED_BRACKETS_DOTAGIF),
    strHtmlFileExtsA, strHtmlFileExtsW, STR_FILEEXTS_SIZE - 1 );

  settdlg_InitOptFromStr( GetDlgItem(hDlg, IDC_ED_BRACKETS_SKIPESCAPED1),
    strEscaped1FileExtsA, strEscaped1FileExtsW, STR_FILEEXTS_SIZE - 1 );

  settdlg_InitOptFromStr( GetDlgItem(hDlg, IDC_ED_BRACKETS_SKIPCOMMENT1),
    strComment1FileExtsA, strComment1FileExtsW, STR_FILEEXTS_SIZE - 1 );

  sd_hToolTip = SettingsDlg_InitToolTip(hDlg);
}

void SettingsDlg_EndDialog(HWND hDlg, INT_PTR nResult)
{
  if (sd_hToolTip)
  {
    DestroyWindow(sd_hToolTip);
    sd_hToolTip = NULL;
  }
  EndDialog(hDlg, nResult);
}

void SettingsDlg_UpdateStateOfChBrackets(HWND hDlg, BOOL bInvert)
{
  UINT uState;
  BOOL bChecked;

  uState = BST_UNCHECKED;
  bChecked = CheckBox_IsChecked(hDlg, IDC_CH_BRACKETS_HIGHLIGHT);
  if ((bChecked && !bInvert) || (bInvert && !bChecked))
  {
    if (CheckBox_IsChecked(hDlg, IDC_CH_BRACKETCOLOR) ||
        CheckBox_IsChecked(hDlg, IDC_CH_BKGNDCOLOR))
    {
      uState = BST_CHECKED;
    }
    else
    {
      uState = BST_INDETERMINATE;
    }
  }
  SendMessage(GetDlgItem(hDlg, IDC_CH_BRACKETS_HIGHLIGHT), 
    BM_SETCHECK, uState, 0);
}

static void fillToolInfoA(
  TOOLINFOA* lptiA,
  LPSTR      lpToolTipText,
  HWND       hWnd,
  UINT_PTR   uId)
{
    RECT rect;

    GetClientRect(hWnd, &rect);

    lptiA->cbSize = sizeof(TOOLINFOA);
    lptiA->uFlags = TTF_SUBCLASS;
    lptiA->hwnd = hWnd;
    lptiA->hinst = g_hInstanceDLL;
    lptiA->uId = uId;
    lptiA->lpszText = lpToolTipText;
    // ToolTip control will cover the whole window
    lptiA->rect.left = rect.left;
    lptiA->rect.top = rect.top;
    lptiA->rect.right = rect.right;
    lptiA->rect.bottom = rect.bottom;
    lptiA->lParam = 0;
}

static void fillToolInfoW(
  TOOLINFOW* lptiW,
  LPWSTR     lpToolTipText,
  HWND       hWnd,
  UINT_PTR   uId)
{
    RECT rect;

    GetClientRect(hWnd, &rect);

    lptiW->cbSize = sizeof(TOOLINFOW);
    lptiW->uFlags = TTF_SUBCLASS;
    lptiW->hwnd = hWnd;
    lptiW->hinst = g_hInstanceDLL;
    lptiW->uId = uId;
    lptiW->lpszText = lpToolTipText;
    // ToolTip control will cover the whole window
    lptiW->rect.left = rect.left;
    lptiW->rect.top = rect.top;
    lptiW->rect.right = rect.right;
    lptiW->rect.bottom = rect.bottom;
    lptiW->lParam = 0;
}

HWND SettingsDlg_InitToolTip(HWND hDlg)
{
  HWND     hToolTip = NULL;
  UINT_PTR uId = 0;
  INITCOMMONCONTROLSEX iccex;

  iccex.dwICC = ICC_WIN95_CLASSES;
  iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  InitCommonControlsEx(&iccex);

  if (g_bOldWindows)
  {
    hToolTip = CreateWindowExA(WS_EX_TOPMOST, TOOLTIPS_CLASSA, 
      0, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
      hDlg, 0, g_hInstanceDLL, 0);
  }
  else
  {
    hToolTip = CreateWindowExW(WS_EX_TOPMOST, TOOLTIPS_CLASSW, 
      0, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
      hDlg, 0, g_hInstanceDLL, 0);

  }

  if (hToolTip)
  {
    SetWindowPos(hToolTip, HWND_TOPMOST,
      0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    if (g_bOldWindows)
    {
      TOOLINFOA tiA;
      const char* cszHintTextA = xbrGetStrA(XBR_STR_HINTAUTOCOMPLHGLT, g_LangSystem);

      fillToolInfoA( &tiA, (LPSTR) cszHintTextA, 
        GetDlgItem(hDlg, IDC_CH_BRACKETS_DODOUBLEQUOTE), ++uId );
      SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

      fillToolInfoA( &tiA, (LPSTR) cszHintTextA, 
        GetDlgItem(hDlg, IDC_CH_BRACKETS_DOSINGLEQUOTE), ++uId );
      SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );

      fillToolInfoA( &tiA, (LPSTR) cszHintTextA, 
        GetDlgItem(hDlg, IDC_CH_BRACKETS_DOTAG), ++uId );
      SendMessage( hToolTip, TTM_ADDTOOLA, 0, (LPARAM) &tiA );
    }
    else
    {
      TOOLINFOW tiW;
      const WCHAR* cszHintTextW = xbrGetStrW(XBR_STR_HINTAUTOCOMPLHGLT, g_LangSystem);

      fillToolInfoW( &tiW, (LPWSTR) cszHintTextW, 
        GetDlgItem(hDlg, IDC_CH_BRACKETS_DODOUBLEQUOTE), ++uId );
      SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

      fillToolInfoW( &tiW, (LPWSTR) cszHintTextW, 
        GetDlgItem(hDlg, IDC_CH_BRACKETS_DOSINGLEQUOTE), ++uId );
      SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

      fillToolInfoW( &tiW, (LPWSTR) cszHintTextW, 
        GetDlgItem(hDlg, IDC_CH_BRACKETS_DOTAG), ++uId );
      SendMessageW( hToolTip, TTM_ADDTOOLW, 0, (LPARAM) &tiW );

    }
  }

  return hToolTip;
}

//---------------------------------------------------------------------------

UINT CheckBox_GetState(HWND hDlg, UINT idCheckBox)
{
  HWND hCheckBox = GetDlgItem(hDlg, idCheckBox);
  if (hCheckBox)
  {
    return (UINT) SendMessage(hCheckBox, BM_GETCHECK, 0, 0);
    // BST_CHECKED, BST_INDETERMINATE, BST_UNCHECKED
  }
  return 0;
}

BOOL CheckBox_SetState(HWND hDlg, UINT idCheckBox, UINT uState)
{
  HWND hCheckBox = GetDlgItem(hDlg, idCheckBox);
  if (hCheckBox)
  {
    SendMessage(hCheckBox, BM_SETCHECK, uState, 0);
    return TRUE;
  }
  return FALSE;
}

BOOL CheckBox_IsChecked(HWND hDlg, UINT idCheckBox)
{
  HWND hCheckBox = GetDlgItem(hDlg, idCheckBox);
  if (hCheckBox)
  {
    UINT uState = (UINT) SendMessage(hCheckBox, BM_GETCHECK, 0, 0);
    if (uState == BST_CHECKED || uState == BST_INDETERMINATE)
      return TRUE;
  }
  return FALSE;
}

BOOL CheckBox_SetCheck(HWND hDlg, UINT idCheckBox, BOOL bChecked)
{
  HWND hCheckBox = GetDlgItem(hDlg, idCheckBox);
  if (hCheckBox)
  {
    SendMessage(hCheckBox, BM_SETCHECK,
      (WPARAM) (bChecked ? BST_CHECKED : BST_UNCHECKED), 0);
    return TRUE;
  }
  return FALSE;
}

BOOL DlgItem_EnableWindow(HWND hDlg, UINT idDlgItem, BOOL bEnable)
{
  HWND hDlgItem = GetDlgItem(hDlg, idDlgItem);
  if (hDlgItem)
  {
    return EnableWindow(hDlgItem, bEnable);
  }
  return FALSE;
}

BOOL DlgItem_SetText(HWND hDlg, UINT idDlgItem, BOOL bUnicode, const void* pszText)
{
  HWND hDlgItem = GetDlgItem(hDlg, idDlgItem);
  if (hDlgItem)
  {
    if (!bUnicode)
      return SetWindowTextA(hDlgItem, (const char *) pszText);
    else
      return SetWindowTextW(hDlgItem, (const wchar_t *) pszText);
  }
  return FALSE;
}
