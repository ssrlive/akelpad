#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include "StrFunc.h"
#include "x64Func.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include string functions
#define xmemcpy
#define xmemset
#define xmemcmp
#define xstrlenA
#define xstrlenW
#define xstrcpyW
#define xstrcpynW
#define xatoiA
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define hex2decA
#define hex2decW
#define xprintfW
#define UTF8toUTF16
#define UTF16toUTF8
#include "StrFunc.h"

//Include x64 functions
#define MultiByteToWideChar64
#define WideCharToMultiByte64
#include "x64Func.h"

//Include wide functions
#define AppendMenuWide
#define ComboBox_AddStringWide
#define CreateDialogWide
#define DialogBoxWide
#define GetCPInfoExWide
#define GetWindowTextWide
#define SetDlgItemTextWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_DIRECTION     1
#define STRID_STR2HEX       2
#define STRID_HEX2STR       3
#define STRID_SETUP         4
#define STRID_ENCLOSE       5
#define STRID_PREFIX        6
#define STRID_SUFFIX        7
#define STRID_SINGLEBYTE    8
#define STRID_AUTO          9
#define STRID_DOUBLEBYTE    10
#define STRID_RADIX         11
#define STRID_RADIXHEX      12
#define STRID_RADIXDEC      13
#define STRID_RADIXOCT      14
#define STRID_ONLYCONVERTED 15
#define STRID_PLUGIN        16
#define STRID_OK            17
#define STRID_CANCEL        18

#define DLLA_HEXSEL_CONVERTTEXT   1
#define DLLA_HEXSEL_EXCONVERTTEXT 2

#define CP_UNICODE_UCS2_LE  1200
#define CP_UNICODE_UCS2_BE  1201
#define CP_UNICODE_UTF7     65000
#define CP_UNICODE_UTF8     65001

#define STR_UNICODE_UCS2_LEA   "1200  (UTF-16 LE)"
#define STR_UNICODE_UCS2_LEW  L"1200  (UTF-16 LE)"
#define STR_UNICODE_UCS2_BEA   "1201  (UTF-16 BE)"
#define STR_UNICODE_UCS2_BEW  L"1201  (UTF-16 BE)"
#define STR_UNICODE_UTF8A      "65001 (UTF-8)"
#define STR_UNICODE_UTF8W     L"65001 (UTF-8)"
#define STR_UNICODE_UTF7A      "65000 (UTF-7)"
#define STR_UNICODE_UTF7W     L"65000 (UTF-7)"

//Direction
#define GH_DIRECTIONAUTO    0x0000
#define GH_DIRECTIONSTR2HEX 0x0001
#define GH_DIRECTIONHEX2STR 0x0002
//Codepage
#define GH_ANSI             0x0004
#define GH_UNICODE          0x0008
//Edit
#define GH_SELECT           0x0010
#define GH_CLEAREDIT        0x0040
//Radix
#define GH_RADIXHEX         0x0000
#define GH_RADIXDEC         0x0100
#define GH_RADIXOCT         0x0200
//Only converted to output
#define GH_ONLYCONVERTED    0x1000

#define RDX_HEX 1
#define RDX_DEC 2
#define RDX_OCT 3

#define AKDLL_GETHEX  (WM_USER + 100)
#define AKDLL_SETUP   (WM_USER + 101)

#define OF_RECT           0x1
#define OF_SETTINGS       0x2

//DestroyDock type
#define DKT_DEFAULT        0x0
#define DKT_NOUNLOAD       0x1
#define DKT_ONMAINFINISH   0x2
#define DKT_KEEPAUTOLOAD   0x4

//Functions prototypes
void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow);
void DestroyDock(HWND hWndDock, DWORD dwType);
BOOL CALLBACK DockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillCodepageListbox(HWND hWnd, int nCodePage);
void GetCodePageName(int nCodePage, wchar_t *wszCodePage, int nLen);
INT_PTR GetHex(HWND hWndHexView, HWND hWndEdit, DWORD dwFlags, const wchar_t *wpPrefix, const wchar_t *wpSuffix, int nAnsiCP, int nUnicodeCP, wchar_t *wszInput, INT_PTR nInputLen, wchar_t **wppOutput);
INT_PTR GetHexFromAnsiString(DWORD dwFlags, char *szInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, wchar_t **wppOutput);
INT_PTR GetHexFromUnicodeString(DWORD dwFlags, wchar_t *wszInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, int nUnicodeCP, wchar_t **wppOutput);
INT_PTR GetAnsiStringFromHex(DWORD dwFlags, const char *szInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, char **ppOutput, BOOL bStopOnError);
INT_PTR GetUnicodeStringFromHex(DWORD dwFlags, const wchar_t *wszInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, int nUnicodeCP, wchar_t **wppOutput, BOOL bStopOnError);
void FreeHexString(void *lpString);
INT_PTR dec2oct(INT_PTR nDec);
INT_PTR oct2dec(INT_PTR nOct);
BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc);
void ShowStandardViewMenu(HWND hWnd, HMENU hMenu, BOOL bMouse);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();

//Global variables
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
PLUGINFUNCTION *pfHexSel;
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hWndEdit;
HMENU hPopupView;
HICON hMainIcon;
BOOL bOldWindows;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
HWND hWndDockDlg=NULL;
RECT rcHexSelCurrentDialog={0};
RECT rcHexSelDockRect={0};
int nHexSelDockSide=DKS_BOTTOM;
DOCK *dkHexSelDlg=NULL;
BOOL bHexSelDockWaitResize=FALSE;
BOOL bOnMainStart=FALSE;
wchar_t wszHexPrefix[MAX_PATH];
wchar_t wszHexSuffix[MAX_PATH];
int nHexDirection=GH_DIRECTIONAUTO;
int nHexAnsiCP=-1;
int nHexUnicodeCP=CP_UNICODE_UCS2_LE;
int nRadix=RDX_HEX;
BOOL bOnlyConverted=FALSE;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="HexSel";
}

//Plugin extern functions
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_HEXSEL_CONVERTTEXT || nAction == DLLA_HEXSEL_EXCONVERTTEXT)
    {
      DWORD dwFlags=GH_UNICODE|GH_SELECT;
      unsigned char *pPrefix=NULL;
      unsigned char *pSuffix=NULL;
      unsigned char *pInput=NULL;
      INT_PTR nInputLen=-1;
      int nInputCP=CP_ACP;
      wchar_t **wppOutput=NULL;
      INT_PTR *lpnOutputLen=NULL;
      wchar_t wszPrefix[MAX_PATH];
      wchar_t wszSuffix[MAX_PATH];
      int nAnsiCP=nHexAnsiCP;
      int nUnicodeCP=nHexUnicodeCP;
      INT_PTR nOutputLen=0;

      if (nAction == DLLA_HEXSEL_CONVERTTEXT)
      {
        if (IsExtCallParamValid(pd->lParam, 2))
          dwFlags=(DWORD)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pPrefix=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          pSuffix=(unsigned char *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          pInput=(unsigned char *)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          nInputLen=GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          nInputCP=(int)GetExtCallParam(pd->lParam, 7);
        if (IsExtCallParamValid(pd->lParam, 8))
          wppOutput=(wchar_t **)GetExtCallParam(pd->lParam, 8);
        if (IsExtCallParamValid(pd->lParam, 9))
          lpnOutputLen=(INT_PTR *)GetExtCallParam(pd->lParam, 9);
      }
      else if (nAction == DLLA_HEXSEL_EXCONVERTTEXT)
      {
        if (IsExtCallParamValid(pd->lParam, 2))
          dwFlags=(DWORD)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pPrefix=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          pSuffix=(unsigned char *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          nAnsiCP=(int)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          nUnicodeCP=(int)GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          pInput=(unsigned char *)GetExtCallParam(pd->lParam, 7);
        if (IsExtCallParamValid(pd->lParam, 8))
          nInputLen=GetExtCallParam(pd->lParam, 8);
        if (IsExtCallParamValid(pd->lParam, 9))
          nInputCP=(int)GetExtCallParam(pd->lParam, 9);
        if (IsExtCallParamValid(pd->lParam, 10))
          wppOutput=(wchar_t **)GetExtCallParam(pd->lParam, 10);
        if (IsExtCallParamValid(pd->lParam, 11))
          lpnOutputLen=(INT_PTR *)GetExtCallParam(pd->lParam, 11);
      }

      if (pd->hWndEdit)
      {
        dwFlags&=~GH_CLEAREDIT;

        if (pPrefix)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pPrefix, -1, wszPrefix, MAX_PATH);
          else
            xstrcpynW(wszPrefix, (wchar_t *)pPrefix, MAX_PATH);
        }
        else xstrcpynW(wszPrefix, wszHexPrefix, MAX_PATH);

        if (pSuffix)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pSuffix, -1, wszSuffix, MAX_PATH);
          else
            xstrcpynW(wszSuffix, (wchar_t *)pSuffix, MAX_PATH);
        }
        else xstrcpynW(wszSuffix, wszHexSuffix, MAX_PATH);

        if (nAnsiCP == -2) nAnsiCP=nHexAnsiCP;
        if (nUnicodeCP == -2) nUnicodeCP=nHexUnicodeCP;

        if (!pInput)
        {
          //Convert selection
          GetHex(pd->hWndEdit, pd->hWndEdit, dwFlags, wszPrefix, wszSuffix, nAnsiCP, nUnicodeCP, NULL, 0, NULL);
        }
        else
        {
          if (pd->dwSupport & PDS_STRANSI)
          {
            wchar_t *wszWideStr=NULL;
            int nWideChars;

            if (nInputLen == -1)
              nInputLen=xstrlenA((char *)pInput);

            //Convert Ansi to Unicode
            if (nWideChars=MultiByteToWideChar(nInputCP, 0, (char *)pInput, (int)nInputLen, NULL, 0))
            {
              if (wszWideStr=(wchar_t *)GlobalAlloc(GPTR, (nWideChars + 1) * sizeof(wchar_t)))
              {
                if (nWideChars=MultiByteToWideChar(nInputCP, 0, (char *)pInput, (int)nInputLen, wszWideStr, nWideChars))
                  wszWideStr[nWideChars]=L'\0';
                nOutputLen=GetHex(pd->hWndEdit, pd->hWndEdit, dwFlags, wszPrefix, wszSuffix, nAnsiCP, nUnicodeCP, wszWideStr, nWideChars, wppOutput);
                GlobalFree((HGLOBAL)wszWideStr);
              }
            }
          }
          else nOutputLen=GetHex(pd->hWndEdit, pd->hWndEdit, dwFlags, wszPrefix, wszSuffix, nAnsiCP, nUnicodeCP, (wchar_t *)pInput, nInputLen, wppOutput);

          if (lpnOutputLen) *lpnOutputLen=nOutputLen;
        }
      }
    }

    //If plugin already loaded, stay in memory and don't change active status
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
    return;
  }

  //Initialize
  if (bInitMain)
  {
    DestroyDock(hWndDockDlg, DKT_KEEPAUTOLOAD);

    //Stay in memory, but show as non-active
    pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    InitMain();

    pfHexSel=pd->lpPluginFunction;
    bHexSelDockWaitResize=pd->bOnStart;
    CreateDock(&hWndDockDlg, &dkHexSelDlg, !bHexSelDockWaitResize);

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow)
{
  DOCK dk;

  *hWndDock=NULL;

  xmemset(&dk, 0, sizeof(DOCK));
  dk.dwFlags=DKF_DRAGDROP;
  dk.nSide=nHexSelDockSide;
  dk.rcSize=rcHexSelDockRect;

  if (*dkDock=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_ADD, (LPARAM)&dk))
  {
    *hWndDock=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_HEXSEL), hMainWnd, (DLGPROC)DockDlgProc);

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
  SendMessage(hWndDock, WM_COMMAND, IDCANCEL, dwType);
}

BOOL CALLBACK DockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndHexView;
  static HWND hWndTitleText;
  static HWND hWndTitleClose;
  static HMENU hMenuLabel;
  static RESIZEDIALOG rds[]={{&hWndTitleText,    RDS_SIZE|RDS_X, 0},
                             {&hWndTitleClose,   RDS_MOVE|RDS_X, 0},
                             {&hWndHexView,      RDS_SIZE|RDS_X, 0},
                             {&hWndHexView,      RDS_SIZE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    HFONT hFontEdit;

    hWndHexView=GetDlgItem(hDlg, IDC_HEXVIEW);
    hWndTitleText=GetDlgItem(hDlg, IDC_TITLETEXT);
    hWndTitleClose=GetDlgItem(hDlg, IDC_CLOSE);

    SendMessage(hWndHexView, EM_SETUNDOLIMIT, 0, 0);
    SendMessage(hWndHexView, EM_SHOWSCROLLBAR, SB_HORZ, FALSE);
    SendMessage(hWndHexView, AEM_SETWORDWRAP, AEWW_WORD, 0);
    hFontEdit=(HFONT)SendMessage(hMainWnd, AKD_GETFONT, (WPARAM)NULL, (LPARAM)NULL);
    SendMessage(hWndHexView, WM_SETFONT, (WPARAM)hFontEdit, FALSE);

    hMenuLabel=CreatePopupMenu();
    AppendMenuWide(hMenuLabel, MF_STRING, IDM_SETUP, GetLangStringW(wLangModule, STRID_SETUP));

    //Set dock title
    {
      BUTTONDRAW bd={BIF_CROSS|BIF_ETCHED};

      SetWindowTextWide(hWndTitleText, wszPluginName);
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndTitleClose, (LPARAM)&bd);
    }
    if (!bOnMainStart) PostMessage(hDlg, AKDLL_GETHEX, 0, 0);
  }
  else if (uMsg == AKDLL_GETHEX)
  {
    DWORD dwFlags=nHexDirection|GH_ANSI|GH_UNICODE|GH_CLEAREDIT;

    if (nRadix == RDX_OCT)
      dwFlags|=GH_RADIXOCT;
    else if (nRadix == RDX_DEC)
      dwFlags|=GH_RADIXDEC;
    if (bOnlyConverted)
      dwFlags|=GH_ONLYCONVERTED;
    GetHex(hWndHexView, (HWND)wParam, dwFlags, wszHexPrefix, wszHexSuffix, nHexAnsiCP, nHexUnicodeCP, NULL, 0, NULL);
  }
  else if (uMsg == AKDLL_SETUP)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hDlg, (DLGPROC)SetupDlgProc);
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
      POINT pt;
      RECT rc;

      GetCursorPos(&pt);
      GetWindowRect(hWndTitleText, &rc);

      if (PtInRect(&rc, pt))
      {
        int nCmd;

        nCmd=TrackPopupMenu(hMenuLabel, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWndTitleText, NULL);

        if (nCmd == IDM_SETUP)
          PostMessage(hDlg, AKDLL_SETUP, 0, 0);
      }
    }
    else if ((HWND)wParam == hWndHexView)
    {
      ShowStandardViewMenu((HWND)wParam, hPopupView, lParam != -1);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_CLOSE ||
        LOWORD(wParam) == IDCANCEL)
    {
      //Save OF_RECT
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      UninitMain();
      SendMessage(hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkHexSelDlg);
      dkHexSelDlg=NULL;

      DestroyMenu(hMenuLabel);
      DestroyWindow(hWndDockDlg);
      hWndDockDlg=NULL;

      if (!(lParam & DKT_ONMAINFINISH))
      {
        SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
        SetFocus(hMainWnd);

        if (!(lParam & DKT_KEEPAUTOLOAD))
        {
          pfHexSel->bAutoLoad=FALSE;
          SendMessage(hMainWnd, AKD_DLLSAVE, DLLSF_ONEXIT, 0);
        }
        if (!(lParam & DKT_NOUNLOAD))
          SendMessage(hMainWnd, AKD_DLLUNLOAD, (WPARAM)hInstanceDLL, (LPARAM)NULL);
      }
    }
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], NULL, &rcHexSelCurrentDialog, 0, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      if (dkHexSelDlg) GetWindowSize(hWndTitleText, hDlg, &dkHexSelDlg->rcDragDrop);
  }

  return FALSE;
}

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndDirectionAuto;
  static HWND hWndDirectionStr2Hex;
  static HWND hWndDirectionHex2Str;
  static HWND hWndPrefix;
  static HWND hWndSuffix;
  static HWND hWndAuto;
  static HWND hWndManual;
  static HWND hWndList;
  static HWND hWndUtf16LE;
  static HWND hWndUtf16BE;
  static HWND hWndRadixHex;
  static HWND hWndRadixDec;
  static HWND hWndRadixOct;
  static HWND hWndOnlyConverted;
  BOOL bState;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndDirectionAuto=GetDlgItem(hDlg, IDC_SETUP_DIRECTION_AUTO);
    hWndDirectionStr2Hex=GetDlgItem(hDlg, IDC_SETUP_DIRECTION_STR2HEX);
    hWndDirectionHex2Str=GetDlgItem(hDlg, IDC_SETUP_DIRECTION_HEX2STR);
    hWndPrefix=GetDlgItem(hDlg, IDC_SETUP_PREFIX);
    hWndSuffix=GetDlgItem(hDlg, IDC_SETUP_SUFFIX);
    hWndAuto=GetDlgItem(hDlg, IDC_SETUP_SINGLEBYTE_AUTO);
    hWndManual=GetDlgItem(hDlg, IDC_SETUP_SINGLEBYTE_MANUAL);
    hWndList=GetDlgItem(hDlg, IDC_SETUP_SINGLEBYTE_LIST);
    hWndUtf16LE=GetDlgItem(hDlg, IDC_SETUP_DOUBLEBYTE_UTF16LE);
    hWndUtf16BE=GetDlgItem(hDlg, IDC_SETUP_DOUBLEBYTE_UTF16BE);
    hWndRadixHex=GetDlgItem(hDlg, IDC_SETUP_RADIXHEX);
    hWndRadixDec=GetDlgItem(hDlg, IDC_SETUP_RADIXDEC);
    hWndRadixOct=GetDlgItem(hDlg, IDC_SETUP_RADIXOCT);
    hWndOnlyConverted=GetDlgItem(hDlg, IDC_SETUP_ONLYCONVERTED_CHECK);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_SETUP_DIRECTION_LABEL, GetLangStringW(wLangModule, STRID_DIRECTION));
    SetDlgItemTextWide(hDlg, IDC_SETUP_DIRECTION_AUTO, GetLangStringW(wLangModule, STRID_AUTO));
    SetDlgItemTextWide(hDlg, IDC_SETUP_DIRECTION_STR2HEX, GetLangStringW(wLangModule, STRID_STR2HEX));
    SetDlgItemTextWide(hDlg, IDC_SETUP_DIRECTION_HEX2STR, GetLangStringW(wLangModule, STRID_HEX2STR));
    SetDlgItemTextWide(hDlg, IDC_SETUP_ENCLOSE_LABEL, GetLangStringW(wLangModule, STRID_ENCLOSE));
    SetDlgItemTextWide(hDlg, IDC_SETUP_PREFIX_LABEL, GetLangStringW(wLangModule, STRID_PREFIX));
    SetDlgItemTextWide(hDlg, IDC_SETUP_SUFFIX_LABEL, GetLangStringW(wLangModule, STRID_SUFFIX));
    SetDlgItemTextWide(hDlg, IDC_SETUP_SINGLEBYTE_LABEL, GetLangStringW(wLangModule, STRID_SINGLEBYTE));
    SetDlgItemTextWide(hDlg, IDC_SETUP_SINGLEBYTE_AUTO, GetLangStringW(wLangModule, STRID_AUTO));
    SetDlgItemTextWide(hDlg, IDC_SETUP_DOUBLEBYTE_LABEL, GetLangStringW(wLangModule, STRID_DOUBLEBYTE));
    SetDlgItemTextWide(hDlg, IDC_SETUP_RADIX_LABEL, GetLangStringW(wLangModule, STRID_RADIX));
    SetDlgItemTextWide(hDlg, IDC_SETUP_RADIXHEX, GetLangStringW(wLangModule, STRID_RADIXHEX));
    SetDlgItemTextWide(hDlg, IDC_SETUP_RADIXDEC, GetLangStringW(wLangModule, STRID_RADIXDEC));
    SetDlgItemTextWide(hDlg, IDC_SETUP_RADIXOCT, GetLangStringW(wLangModule, STRID_RADIXOCT));
    SetDlgItemTextWide(hDlg, IDC_SETUP_ONLYCONVERTED_CHECK, GetLangStringW(wLangModule, STRID_ONLYCONVERTED));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    if (nHexDirection == GH_DIRECTIONSTR2HEX)
      SendMessage(hWndDirectionStr2Hex, BM_SETCHECK, BST_CHECKED, 0);
    else if (nHexDirection == GH_DIRECTIONHEX2STR)
      SendMessage(hWndDirectionHex2Str, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndDirectionAuto, BM_SETCHECK, BST_CHECKED, 0);

    SendMessage(hWndPrefix, EM_LIMITTEXT, MAX_PATH, 0);
    SendMessage(hWndSuffix, EM_LIMITTEXT, MAX_PATH, 0);
    SetWindowTextWide(hWndPrefix, wszHexPrefix);
    SetWindowTextWide(hWndSuffix, wszHexSuffix);

    FillCodepageListbox(hWndList, nHexAnsiCP);

    if (nHexAnsiCP == -1)
    {
      SendMessage(hWndAuto, BM_SETCHECK, BST_CHECKED, 0);
      EnableWindow(hWndList, FALSE);
    }
    else SendMessage(hWndManual, BM_SETCHECK, BST_CHECKED, 0);

    if (nHexUnicodeCP == CP_UNICODE_UCS2_LE)
      SendMessage(hWndUtf16LE, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndUtf16BE, BM_SETCHECK, BST_CHECKED, 0);

    if (nRadix == RDX_OCT)
      SendMessage(hWndRadixOct, BM_SETCHECK, BST_CHECKED, 0);
    else if (nRadix == RDX_DEC)
      SendMessage(hWndRadixDec, BM_SETCHECK, BST_CHECKED, 0);
    else
      SendMessage(hWndRadixHex, BM_SETCHECK, BST_CHECKED, 0);

    if (bOnlyConverted) SendMessage(hWndOnlyConverted, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SETUP_SINGLEBYTE_AUTO ||
        LOWORD(wParam) == IDC_SETUP_SINGLEBYTE_MANUAL)
    {
      bState=(BOOL)SendMessage(hWndManual, BM_GETCHECK, 0, 0);
      EnableWindow(hWndList, bState);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      if (SendMessage(hWndDirectionStr2Hex, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nHexDirection=GH_DIRECTIONSTR2HEX;
      else if (SendMessage(hWndDirectionHex2Str, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nHexDirection=GH_DIRECTIONHEX2STR;
      else
        nHexDirection=GH_DIRECTIONAUTO;

      GetWindowTextWide(hWndPrefix, wszHexPrefix, MAX_PATH);
      GetWindowTextWide(hWndSuffix, wszHexSuffix, MAX_PATH);

      if (SendMessage(hWndAuto, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nHexAnsiCP=-1;
      else
        nHexAnsiCP=GetDlgItemInt(hDlg, IDC_SETUP_SINGLEBYTE_LIST, NULL, FALSE);

      if (SendMessage(hWndUtf16LE, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nHexUnicodeCP=CP_UNICODE_UCS2_LE;
      else if (SendMessage(hWndUtf16BE, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nHexUnicodeCP=CP_UNICODE_UCS2_BE;

      if (SendMessage(hWndRadixOct, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nRadix=RDX_OCT;
      else if (SendMessage(hWndRadixDec, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nRadix=RDX_DEC;
      else if (SendMessage(hWndRadixHex, BM_GETCHECK, 0, 0) == BST_CHECKED)
        nRadix=RDX_HEX;

      bOnlyConverted=(BOOL)SendMessage(hWndOnlyConverted, BM_GETCHECK, 0, 0);
      dwSaveFlags|=OF_SETTINGS;

      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      EndDialog(hDlg, 0);
      PostMessage(hWndDockDlg, AKDLL_GETHEX, 0, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Destroy plugin icon
    DestroyIcon(hPluginIcon);
  }
  return FALSE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDN_MAIN_ONSTART_IDLE)
  {
    if (bOnMainStart) PostMessage(hWndDockDlg, AKDLL_GETHEX, 0, 0);
  }
  else if (uMsg == AKDN_FRAME_NOWINDOWS ||
           uMsg == AKDN_FRAME_ACTIVATE)
  {
    SendMessage(hWndDockDlg, AKDLL_GETHEX, 0, 0);
  }
  else if (uMsg == AKDN_DOCK_RESIZE)
  {
    if (((DOCK *)wParam)->hWnd == dkHexSelDlg->hWnd)
      dwSaveFlags|=OF_RECT;
  }
  else if (uMsg == AKDN_SIZE_ONSTART)
  {
    if (bHexSelDockWaitResize)
    {
      bHexSelDockWaitResize=FALSE;
      ShowWindow(hWndDockDlg, SW_SHOW);
      dkHexSelDlg->dwFlags&=~DKF_HIDDEN;
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

    if (hWndDockDlg)
    {
      DestroyDock(hWndDockDlg, DKT_ONMAINFINISH);
    }
    return FALSE;
  }

  //Edit parent
  {
   LRESULT lResult;

   if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
     return lResult;
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //Edit parent
  {
   LRESULT lResult;

   if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
     return lResult;
  }

  //Call next procedure
  return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_NOTIFY)
  {
    if (hWndDockDlg)
    {
      if (wParam == ID_EDIT)
      {
        if (((NMHDR *)lParam)->code == EN_SELCHANGE)
        {
          SendMessage(hWndDockDlg, AKDLL_GETHEX, (WPARAM)((NMHDR *)lParam)->hwndFrom, 0);
        }
      }
    }
  }
  return FALSE;
}

void FillCodepageListbox(HWND hWnd, int nCodePage)
{
  wchar_t wszCodepageName[MAX_PATH];
  int *lpCodepageList;
  int nSel=0;
  int nIndex=0;
  int i;

  if (lpCodepageList=(int *)SendMessage(hMainWnd, AKD_GETCODEPAGELIST, 0, 0))
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      if (lpCodepageList[i] != CP_UNICODE_UCS2_LE && lpCodepageList[i] != CP_UNICODE_UCS2_BE)
      {
        if (nCodePage == lpCodepageList[i]) nSel=nIndex;

        GetCodePageName(lpCodepageList[i], wszCodepageName, MAX_PATH);
        ComboBox_AddStringWide(hWnd, wszCodepageName);
        ++nIndex;
      }
    }
    if (!nSel) nCodePage=lpCodepageList[0];
    SendMessage(hWnd, CB_SETCURSEL, nSel, 0);
  }
}

void GetCodePageName(int nCodePage, wchar_t *wszCodePage, int nLen)
{
  CPINFOEXW CPInfoExW;
  wchar_t wszNumber[32];

  if (nCodePage)
  {
    if (nCodePage == CP_UNICODE_UCS2_LE)
      xstrcpynW(wszCodePage, STR_UNICODE_UCS2_LEW, nLen);
    else if (nCodePage == CP_UNICODE_UCS2_BE)
      xstrcpynW(wszCodePage, STR_UNICODE_UCS2_BEW, nLen);
    else if (nCodePage == CP_UNICODE_UTF8)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF8W, nLen);
    else if (nCodePage == CP_UNICODE_UTF7)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF7W, nLen);
    else
    {
      if (GetCPInfoExWide(nCodePage, 0, &CPInfoExW) && nCodePage == xatoiW(CPInfoExW.CodePageName, NULL))
      {
        xstrcpynW(wszCodePage, CPInfoExW.CodePageName, nLen);
      }
      else
      {
        xprintfW(wszNumber, L"%d ", nCodePage);
        xstrcpynW(wszCodePage, wszNumber, nLen);
      }
    }
  }
  else wszCodePage[0]='\0';
}

INT_PTR GetHex(HWND hWndHexView, HWND hWndEdit, DWORD dwFlags, const wchar_t *wpPrefix, const wchar_t *wpSuffix, int nAnsiCP, int nUnicodeCP, wchar_t *wszInput, INT_PTR nInputLen, wchar_t **wppOutput)
{
  EDITINFO ei;
  CHARRANGE64 cr;
  CHARRANGE64 crBefore;
  CHARRANGE64 crAfter;
  wchar_t *wszIn=wszInput;
  wchar_t *wszOut;
  char *szIn;
  char *szOut;
  INT_PTR nInputAnsiLen=0;
  INT_PTR nInputUnicodeLen=nInputLen;
  INT_PTR nOutputAnsiLen=0;
  INT_PTR nOutputUnicodeLen=0;
  INT_PTR nCaretOffset=0;
  int nSuccessCount=0;
  BOOL bStopOnError;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    if (!hWndEdit || ei.hWndEdit == hWndEdit)
    {
      if (!wszIn)
      {
        EXGETTEXTRANGE tr;

        SendMessage(ei.hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr);
        tr.pText=NULL;
        tr.nNewLine=AELB_ASIS;
        nInputUnicodeLen=SendMessage(hMainWnd, AKD_EXGETTEXTRANGEW, (WPARAM)ei.hWndEdit, (LPARAM)&tr);
        wszIn=(wchar_t *)tr.pText;
      }
      else
      {
        if (nInputUnicodeLen == -1)
          nInputUnicodeLen=xstrlenW(wszIn);
      }

      if (nInputUnicodeLen)
      {
        //Use file codepage if not unicode.
        if (nAnsiCP == -1)
        {
          if (ei.nCodePage != CP_UNICODE_UCS2_LE && ei.nCodePage != CP_UNICODE_UCS2_BE)
            nAnsiCP=ei.nCodePage;
          else
            nAnsiCP=CP_ACP;
        }

        if (!wppOutput)
        {
          if (dwFlags & GH_CLEAREDIT)
          {
            SetWindowTextA(hWndHexView, "");
            SendMessage(hWndHexView, WM_SETREDRAW, FALSE, 0);
          }
          if (dwFlags & GH_SELECT)
          {
            SendMessage(hWndHexView, EM_EXGETSEL64, 0, (LPARAM)&crBefore);
            nCaretOffset=SendMessage(hWndHexView, AEM_GETRICHOFFSET, AEGI_CARETCHAR, 0);
          }
        }
        else
        {
          //Output only one type
          if ((dwFlags & GH_ANSI) && (dwFlags & GH_UNICODE))
            dwFlags&=~GH_ANSI;
        }

        //Direction is GH_DIRECTIONAUTO
        if (!(dwFlags & GH_DIRECTIONHEX2STR) && !(dwFlags & GH_DIRECTIONSTR2HEX))
        {
          bStopOnError=TRUE;
          dwFlags|=GH_DIRECTIONHEX2STR|GH_DIRECTIONSTR2HEX;
        }
        else bStopOnError=FALSE;

        for (;;)
        {
          if (dwFlags & GH_DIRECTIONHEX2STR)
          {
            //Ansi
            nSuccessCount=0;

            if (dwFlags & GH_ANSI)
            {
              nInputAnsiLen=WideCharToMultiByte64(nAnsiCP, 0, wszIn, nInputUnicodeLen, NULL, 0, NULL, NULL);

              if (szIn=(char *)GlobalAlloc(GPTR, nInputAnsiLen + 1))
              {
                WideCharToMultiByte64(nAnsiCP, 0, wszIn, nInputUnicodeLen + 1, szIn, nInputAnsiLen + 1, NULL, NULL);

                if (nOutputAnsiLen=GetAnsiStringFromHex(dwFlags, szIn, nInputUnicodeLen, wpPrefix, wpSuffix, &szOut, bStopOnError))
                {
                  nOutputUnicodeLen=MultiByteToWideChar64(nAnsiCP, 0, szOut, nOutputAnsiLen, NULL, 0);

                  if (wszOut=(wchar_t *)GlobalAlloc(GPTR, (nOutputUnicodeLen + 1) * sizeof(wchar_t)))
                  {
                    MultiByteToWideChar64(nAnsiCP, 0, szOut, nOutputAnsiLen + 1, wszOut, nOutputUnicodeLen + 1);

                    if (!wppOutput)
                    {
                      SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndHexView, (LPARAM)wszOut);
                      SendMessage(hWndHexView, EM_EXGETSEL64, 0, (LPARAM)&crAfter);
                      GlobalFree((HGLOBAL)wszOut);
                    }
                    else *wppOutput=wszOut;

                    ++nSuccessCount;
                  }
                  FreeHexString(szOut);
                }
                GlobalFree((HGLOBAL)szIn);
              }
            }

            //Unicode
            if (dwFlags & GH_UNICODE)
            {
              if (nOutputUnicodeLen=GetUnicodeStringFromHex(dwFlags, wszIn, nInputUnicodeLen, wpPrefix, wpSuffix, nUnicodeCP, &wszOut, bStopOnError))
              {
                if (!wppOutput)
                {
                  if (nSuccessCount)
                  {
                    SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndHexView, (LPARAM)L"\n\n");
                    SendMessage(hWndHexView, EM_EXGETSEL64, 0, (LPARAM)&cr);
                  }
                  SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndHexView, (LPARAM)wszOut);
                  SendMessage(hWndHexView, EM_EXGETSEL64, 0, (LPARAM)&crAfter);
                  if (nSuccessCount)
                    SendMessage(hWndHexView, EM_EXSETSEL64, 0, (LPARAM)&cr);
                  FreeHexString(wszOut);
                }
                else *wppOutput=wszOut;

                ++nSuccessCount;
              }
            }

            if (bStopOnError && !nSuccessCount)
            {
              //No successfull StringFromHex convertion - use GH_DIRECTIONSTR2HEX
              dwFlags&=~GH_DIRECTIONHEX2STR;
              continue;
            }
          }
          else if (dwFlags & GH_DIRECTIONSTR2HEX)
          {
            //Ansi
            nSuccessCount=0;

            if (dwFlags & GH_ANSI)
            {
              nInputAnsiLen=WideCharToMultiByte64(nAnsiCP, 0, wszIn, nInputUnicodeLen, NULL, 0, NULL, NULL);

              if (szIn=(char *)GlobalAlloc(GPTR, nInputAnsiLen + 1))
              {
                WideCharToMultiByte64(nAnsiCP, 0, wszIn, nInputUnicodeLen + 1, szIn, nInputAnsiLen + 1, NULL, NULL);

                if (nOutputUnicodeLen=GetHexFromAnsiString(dwFlags, szIn, nInputAnsiLen, wpPrefix, wpSuffix, &wszOut))
                {
                  if (!wppOutput)
                  {
                    SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndHexView, (LPARAM)wszOut);
                    SendMessage(hWndHexView, EM_EXGETSEL64, 0, (LPARAM)&crAfter);
                    FreeHexString(wszOut);
                  }
                  else *wppOutput=wszOut;

                  ++nSuccessCount;
                }
                GlobalFree((HGLOBAL)szIn);
              }
            }

            //Unicode
            if (dwFlags & GH_UNICODE)
            {
              if (nOutputUnicodeLen=GetHexFromUnicodeString(dwFlags, wszIn, nInputUnicodeLen, wpPrefix, wpSuffix, nUnicodeCP, &wszOut))
              {
                if (!wppOutput)
                {
                  if (nSuccessCount)
                  {
                    SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndHexView, (LPARAM)L"\n\n");
                    SendMessage(hWndHexView, EM_EXGETSEL64, 0, (LPARAM)&cr);
                  }
                  SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndHexView, (LPARAM)wszOut);
                  SendMessage(hWndHexView, EM_EXGETSEL64, 0, (LPARAM)&crAfter);
                  if (nSuccessCount)
                    SendMessage(hWndHexView, EM_EXSETSEL64, 0, (LPARAM)&cr);
                  FreeHexString(wszOut);
                }
                else *wppOutput=wszOut;

                ++nSuccessCount;
              }
            }
          }
          break;
        }

        //Set selection
        if (!wppOutput)
        {
          if (dwFlags & GH_SELECT)
          {
            if (nSuccessCount)
            {
              if (nCaretOffset == crBefore.cpMin)
              {
                cr.cpMin=crAfter.cpMax;
                cr.cpMax=crBefore.cpMin;
              }
              else
              {
                cr.cpMin=crBefore.cpMin;
                cr.cpMax=crAfter.cpMax;
              }
              SendMessage(hWndHexView, EM_EXSETSEL64, 0, (LPARAM)&cr);
            }
          }
          if (dwFlags & GH_CLEAREDIT)
          {
            SendMessage(hWndHexView, WM_SETREDRAW, TRUE, 0);
            InvalidateRect(hWndHexView, NULL, FALSE);
          }
        }
      }
      else
      {
        if (!wppOutput)
        {
          if (dwFlags & GH_CLEAREDIT)
            SetWindowTextA(hWndHexView, "");
        }
      }
      if (!wszInput && wszIn) SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wszIn);
    }
  }
  else
  {
    if (!wppOutput)
    {
      if (dwFlags & GH_CLEAREDIT)
        SetWindowTextA(hWndHexView, "");
    }
  }
  return nOutputUnicodeLen;
}

INT_PTR GetHexFromAnsiString(DWORD dwFlags, char *szInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, wchar_t **wppOutput)
{
  wchar_t *wszOutput;
  int nPrefixLen;
  int nSuffixLen;
  int nSymbolLen;
  INT_PTR nOutputLen;
  INT_PTR a=0;
  INT_PTR b=0;

  nPrefixLen=(int)xstrlenW(wpPrefix);
  nSuffixLen=(int)xstrlenW(wpSuffix);
  nSymbolLen=nPrefixLen + nSuffixLen;
  if ((dwFlags & GH_RADIXDEC) || (dwFlags & GH_RADIXOCT))
    nSymbolLen+=3;
  else
    nSymbolLen+=2;
  nOutputLen=nInputLen * nSymbolLen;

  if (wszOutput=*wppOutput=(wchar_t *)GlobalAlloc(GPTR, (nOutputLen + 1) * sizeof(wchar_t)))
  {
    for (; a < nInputLen && (b + nSymbolLen) <= nOutputLen; ++a)
    {
      xmemcpy(wszOutput + b, wpPrefix, nPrefixLen * sizeof(wchar_t));
      b+=nPrefixLen;

      if (dwFlags & GH_RADIXOCT)
        b+=xitoaW(dec2oct((unsigned char)szInput[a]), wszOutput + b);
      else if (dwFlags & GH_RADIXDEC)
        b+=xitoaW((unsigned char)szInput[a], wszOutput + b);
      else
        b+=dec2hexW((unsigned char)szInput[a], wszOutput + b, 2, FALSE);

      xmemcpy(wszOutput + b, wpSuffix, nSuffixLen * sizeof(wchar_t));
      b+=nSuffixLen;
    }
    *(wszOutput + b)='\0';
  }

  if (!b)
  {
    GlobalFree(*wppOutput);
    *wppOutput=NULL;
  }
  return b;
}

INT_PTR GetHexFromUnicodeString(DWORD dwFlags, wchar_t *wszInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, int nUnicodeCP, wchar_t **wppOutput)
{
  wchar_t *wszOutput;
  wchar_t wchChar='\0';
  int nPrefixLen;
  int nSuffixLen;
  int nSymbolLen;
  INT_PTR nOutputLen;
  INT_PTR a=0;
  INT_PTR b=0;

  nPrefixLen=(int)xstrlenW(wpPrefix);
  nSuffixLen=(int)xstrlenW(wpSuffix);
  nSymbolLen=nPrefixLen + nSuffixLen;
  if ((dwFlags & GH_RADIXDEC) || (dwFlags & GH_RADIXOCT))
    nSymbolLen+=5;
  else
    nSymbolLen+=4;
  nOutputLen=nInputLen * nSymbolLen;

  if (wszOutput=*wppOutput=(wchar_t *)GlobalAlloc(GPTR, (nOutputLen + 1) * sizeof(wchar_t)))
  {
    for (; a < nInputLen && (b + nSymbolLen) <= nOutputLen; ++a)
    {
      xmemcpy(wszOutput + b, wpPrefix, nPrefixLen * sizeof(wchar_t));
      b+=nPrefixLen;

      if (nUnicodeCP == CP_UNICODE_UCS2_LE)
        wchChar=wszInput[a];
      else if (nUnicodeCP == CP_UNICODE_UCS2_BE)
        wchChar=MAKEWORD(HIBYTE(wszInput[a]), LOBYTE(wszInput[a]));

      if (dwFlags & GH_RADIXOCT)
        b+=xitoaW(dec2oct(wchChar), wszOutput + b);
      else if (dwFlags & GH_RADIXDEC)
        b+=xitoaW(wchChar, wszOutput + b);
      else
        b+=dec2hexW(wchChar, wszOutput + b, 4, FALSE);

      xmemcpy(wszOutput + b, wpSuffix, nSuffixLen * sizeof(wchar_t));
      b+=nSuffixLen;
    }
    *(wszOutput + b)='\0';
  }

  if (!b)
  {
    GlobalFree(*wppOutput);
    *wppOutput=NULL;
  }
  return b;
}

INT_PTR GetAnsiStringFromHex(DWORD dwFlags, const char *szInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, char **ppOutput, BOOL bStopOnError)
{
  char szPrefix[MAX_PATH];
  char szSuffix[MAX_PATH];
  const char *pNumberEnd;
  char *szOutput;
  const char *pIn;
  const char *pInMax;
  const char *pPrefixStart;
  char *pOut=NULL;
  int nPrefixLen;
  int nSuffixLen;
  int nSymbolLen;
  int nChar;
  INT_PTR nResult;

  nPrefixLen=WideCharToMultiByte(CP_ACP, 0, wpPrefix, -1, szPrefix, MAX_PATH, NULL, NULL) - 1;
  nSuffixLen=WideCharToMultiByte(CP_ACP, 0, wpSuffix, -1, szSuffix, MAX_PATH, NULL, NULL) - 1;
  nSymbolLen=nPrefixLen + nSuffixLen;

  if ((dwFlags & GH_RADIXDEC) || (dwFlags & GH_RADIXOCT))
  {
    if (!nSymbolLen) return 0;
  }

  if (szOutput=*ppOutput=(char *)GlobalAlloc(GPTR, nInputLen + 1))
  {
    pIn=szInput;
    pInMax=szInput + nInputLen;
    pOut=szOutput;

    while (pIn < pInMax)
    {
      pPrefixStart=pIn;

      if (!xmemcmp(pIn, szPrefix, nPrefixLen))
      {
        pIn+=nPrefixLen;

        if ((dwFlags & GH_RADIXDEC) || (dwFlags & GH_RADIXOCT))
        {
          nChar=(int)xatoiA(pIn, &pNumberEnd);
          if (pIn == pNumberEnd)
            goto NotHex;
          if (dwFlags & GH_RADIXOCT)
            nChar=(int)oct2dec(nChar);
          pIn=pNumberEnd;
        }
        else nChar=(int)hex2decA(pIn, 2, &pIn);

        if (nChar >= 0 && !xmemcmp(pIn, szSuffix, nSuffixLen))
        {
          pIn+=nSuffixLen;

          if (nChar > 0)
            *pOut++=(char)nChar;
          else
            *pOut++=' ';
          continue;
        }
      }
      NotHex:
      if (bStopOnError)
        goto Error;
      pIn=pPrefixStart;
      if (!(dwFlags & GH_ONLYCONVERTED))
        *pOut++=*pIn;
      ++pIn;
    }
    *pOut='\0';
  }
  goto End;

  Error:
  pOut=szOutput;

  End:
  nResult=pOut - szOutput;
  if (!nResult)
  {
    GlobalFree(*ppOutput);
    *ppOutput=NULL;
  }
  return nResult;
}

INT_PTR GetUnicodeStringFromHex(DWORD dwFlags, const wchar_t *wszInput, INT_PTR nInputLen, const wchar_t *wpPrefix, const wchar_t *wpSuffix, int nUnicodeCP, wchar_t **wppOutput, BOOL bStopOnError)
{
  wchar_t wszHexChar[5];
  const wchar_t *wpNumberEnd;
  wchar_t *wszOutput;
  const wchar_t *wpIn;
  const wchar_t *wpInMax;
  const wchar_t *wpPrefixStart;
  wchar_t *wpOut=NULL;
  int nPrefixLen;
  int nPrefixSize;
  int nSuffixLen;
  int nSuffixSize;
  int nSymbolLen;
  int nChar;
  INT_PTR nResult;

  nPrefixLen=(int)xstrlenW(wpPrefix);
  nSuffixLen=(int)xstrlenW(wpSuffix);
  nPrefixSize=nPrefixLen * sizeof(wchar_t);
  nSuffixSize=nSuffixLen * sizeof(wchar_t);
  nSymbolLen=nPrefixLen + nSuffixLen;

  if ((dwFlags & GH_RADIXDEC) || (dwFlags & GH_RADIXOCT))
  {
    if (!nSymbolLen) return 0;
  }

  if (wszOutput=*wppOutput=(wchar_t *)GlobalAlloc(GPTR, (nInputLen + 1) * sizeof(wchar_t)))
  {
    wpIn=wszInput;
    wpInMax=wszInput + nInputLen;
    wpOut=wszOutput;

    while (wpIn < wpInMax)
    {
      wpPrefixStart=wpIn;

      if (!xmemcmp(wpIn, wpPrefix, nPrefixSize))
      {
        wpIn+=nPrefixLen;

        if ((dwFlags & GH_RADIXDEC) || (dwFlags & GH_RADIXOCT))
        {
          nChar=(int)xatoiW(wpIn, &wpNumberEnd);
          if (wpIn == wpNumberEnd)
            goto NotHex;
          if (dwFlags & GH_RADIXOCT)
            nChar=(int)oct2dec(nChar);
          wpIn=wpNumberEnd;
        }
        else
        {
          if (wpIn + 4 > wpInMax)
            goto NotHex;
          if (nUnicodeCP == CP_UNICODE_UCS2_LE)
          {
            wszHexChar[0]=*wpIn++;
            wszHexChar[1]=*wpIn++;
            wszHexChar[2]=*wpIn++;
            wszHexChar[3]=*wpIn++;
          }
          else if (nUnicodeCP == CP_UNICODE_UCS2_BE)
          {
            wszHexChar[2]=*wpIn++;
            wszHexChar[3]=*wpIn++;
            wszHexChar[0]=*wpIn++;
            wszHexChar[1]=*wpIn++;
          }
          nChar=(int)hex2decW(wszHexChar, 4, NULL);
        }

        if (nChar >= 0 && !xmemcmp(wpIn, wpSuffix, nSuffixSize))
        {
          wpIn+=nSuffixLen;

          if (nChar > 0)
            *wpOut++=(wchar_t)nChar;
          else
            *wpOut++=' ';
          continue;
        }
      }
      NotHex:
      if (bStopOnError)
        goto Error;
      wpIn=wpPrefixStart;
      if (!(dwFlags & GH_ONLYCONVERTED))
        *wpOut++=*wpIn;
      ++wpIn;
    }
    *wpOut=L'\0';
  }
  goto End;

  Error:
  wpOut=wszOutput;

  End:
  nResult=wpOut - wszOutput;
  if (!nResult)
  {
    GlobalFree(*wppOutput);
    *wppOutput=NULL;
  }
  return nResult;
}

void FreeHexString(void *lpString)
{
  GlobalFree((HGLOBAL)lpString);
}

INT_PTR dec2oct(INT_PTR nDec)
{
  INT_PTR nMod;
  INT_PTR nResult=0;
  INT_PTR nDigits=1;

  do
  {
    nMod=nDec % 8;
    nDec=nDec / 8;

    nResult+=nDigits * nMod;
    nDigits*=10;
  }
  while (nDec);

  return nResult;
}

INT_PTR oct2dec(INT_PTR nOct)
{
  INT_PTR nMod;
  INT_PTR nResult=0;
  INT_PTR nDigits=1;

  do
  {
    nMod=nOct % 10;
    nOct=nOct / 10;

    nResult+=nDigits * nMod;
    nDigits*=8;
  }
  while (nOct);

  return nResult;
}

BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc)
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

void ShowStandardViewMenu(HWND hWnd, HMENU hMenu, BOOL bMouse)
{
  POINT pt;
  CHARRANGE64 cr;
  int nCmd;

  if (!bMouse && SendMessage(hWnd, AEM_GETCARETPOS, (WPARAM)&pt, 0))
  {
    pt.y+=(int)SendMessage(hWnd, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
    ClientToScreen(hWnd, &pt);
  }
  else GetCursorPos(&pt);

  SendMessage(hWnd, EM_EXGETSEL64, 0, (LPARAM)&cr);
  EnableMenuItem(hMenu, IDM_EDIT_COPY, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);

  nCmd=TrackPopupMenu(hMenu, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

  if (nCmd == IDM_EDIT_COPY)
    SendMessage(hWnd, WM_COPY, 0, 0);
  else if (nCmd == IDM_EDIT_SELECTALL)
    SendMessage(hWnd, EM_SETSEL, 0, -1);
}


//// Options

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData)
{
  PLUGINOPTIONW po;

  po.pOptionName=pOptionName;
  po.dwType=dwType;
  po.lpData=lpData;
  po.dwData=dwData;
  return SendMessage(hMainWnd, AKD_OPTIONW, (WPARAM)hOptions, (LPARAM)&po);
}

void ReadOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&nHexSelDockSide, sizeof(DWORD));
    WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&rcHexSelDockRect, sizeof(RECT));
    WideOption(hOptions, L"Direction", PO_DWORD, (LPBYTE)&nHexDirection, sizeof(DWORD));
    WideOption(hOptions, L"Prefix", PO_BINARY, (LPBYTE)wszHexPrefix, MAX_PATH * sizeof(wchar_t));
    WideOption(hOptions, L"Suffix", PO_BINARY, (LPBYTE)wszHexSuffix, MAX_PATH * sizeof(wchar_t));
    WideOption(hOptions, L"Ansi", PO_DWORD, (LPBYTE)&nHexAnsiCP, sizeof(DWORD));
    WideOption(hOptions, L"Unicode", PO_DWORD, (LPBYTE)&nHexUnicodeCP, sizeof(DWORD));
    WideOption(hOptions, L"Radix", PO_DWORD, (LPBYTE)&nRadix, sizeof(DWORD));
    WideOption(hOptions, L"OnlyConverted", PO_DWORD, (LPBYTE)&bOnlyConverted, sizeof(DWORD));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_RECT)
    {
      WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&dkHexSelDlg->nSide, sizeof(DWORD));
      WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&dkHexSelDlg->rcSize, sizeof(RECT));
    }
    if (dwFlags & OF_SETTINGS)
    {
      WideOption(hOptions, L"Direction", PO_DWORD, (LPBYTE)&nHexDirection, sizeof(DWORD));
      WideOption(hOptions, L"Prefix", PO_BINARY, (LPBYTE)wszHexPrefix, ((int)xstrlenW(wszHexPrefix) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"Suffix", PO_BINARY, (LPBYTE)wszHexSuffix, ((int)xstrlenW(wszHexSuffix) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"Ansi", PO_DWORD, (LPBYTE)&nHexAnsiCP, sizeof(DWORD));
      WideOption(hOptions, L"Unicode", PO_DWORD, (LPBYTE)&nHexUnicodeCP, sizeof(DWORD));
      WideOption(hOptions, L"Radix", PO_DWORD, (LPBYTE)&nRadix, sizeof(DWORD));
      WideOption(hOptions, L"OnlyConverted", PO_DWORD, (LPBYTE)&bOnlyConverted, sizeof(DWORD));
    }

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

const char* GetLangStringA(LANGID wLangID, int nStringID)
{
  static char szStringBuf[MAX_PATH];

  WideCharToMultiByte(CP_ACP, 0, GetLangStringW(wLangID, nStringID), -1, szStringBuf, MAX_PATH, NULL, NULL);
  return szStringBuf;
}

const wchar_t* GetLangStringW(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_DIRECTION)
      return L"\x041D\x0430\x043F\x0440\x0430\x0432\x043B\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_STR2HEX)
      return L"\x0422\x0435\x043A\x0441\x0442\x0020\x002D\x003E\x0020\x0048\x0065\x0078";
    if (nStringID == STRID_HEX2STR)
      return L"\x0048\x0065\x0078\x0020\x002D\x003E\x0020\x0422\x0435\x043A\x0441\x0442";
    if (nStringID == STRID_SETUP)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_ENCLOSE)
      return L"\x041E\x0431\x0440\x0430\x043C\x043B\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_PREFIX)
      return L"\x041F\x0440\x0435\x0444\x0438\x043A\x0441\x003A";
    if (nStringID == STRID_SUFFIX)
      return L"\x0421\x0443\x0444\x0444\x0438\x043A\x0441\x003A";
    if (nStringID == STRID_SINGLEBYTE)
      return L"\x041E\x0434\x0438\x043D\x0020\x0431\x0430\x0439\x0442";
    if (nStringID == STRID_AUTO)
      return L"\x0410\x0432\x0442\x043E\x043C\x0430\x0442\x0438\x0447\x0435\x0441\x043A\x0438";
    if (nStringID == STRID_DOUBLEBYTE)
      return L"\x0414\x0432\x0430\x0020\x0431\x0430\x0439\x0442\x0430";
    if (nStringID == STRID_RADIX)
      return L"\x0421\x0438\x0441\x0442\x0435\x043C\x0430\x0020\x0441\x0447\x0438\x0441\x043B\x0435\x043D\x0438\x044F";
    if (nStringID == STRID_RADIXHEX)
      return L"\x0428\x0435\x0441\x0442\x043D\x0430\x0434\x0446\x0430\x0442\x0435\x0440\x0438\x0447\x043D\x0430\x044F";
    if (nStringID == STRID_RADIXDEC)
      return L"\x0414\x0435\x0441\x044F\x0442\x0435\x0440\x0438\x0447\x043D\x0430\x044F";
    if (nStringID == STRID_RADIXOCT)
      return L"\x0412\x043E\x0441\x044C\x043C\x0435\x0440\x0438\x0447\x043D\x0430\x044F";
    if (nStringID == STRID_ONLYCONVERTED)
      return L"\x0412\x044B\x0432\x043E\x0434\x0438\x0442\x044C\x0020\x0442\x043E\x043B\x044C\x043A\x043E\x0020\x043F\x0440\x0435\x043E\x0431\x0440\x0430\x0437\x043E\x0432\x0430\x043D\x043D\x043E\x0435";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_DIRECTION)
      return L"Direction";
    if (nStringID == STRID_STR2HEX)
      return L"Text -> Hex";
    if (nStringID == STRID_HEX2STR)
      return L"Hex -> Text";
    if (nStringID == STRID_SETUP)
      return L"Settings...";
    if (nStringID == STRID_ENCLOSE)
      return L"Enclose";
    if (nStringID == STRID_PREFIX)
      return L"Prefix:";
    if (nStringID == STRID_SUFFIX)
      return L"Suffix:";
    if (nStringID == STRID_SINGLEBYTE)
      return L"Single byte";
    if (nStringID == STRID_AUTO)
      return L"Auto";
    if (nStringID == STRID_DOUBLEBYTE)
      return L"Double byte";
    if (nStringID == STRID_RADIX)
      return L"Radix";
    if (nStringID == STRID_RADIXHEX)
      return L"Hexadecimal";
    if (nStringID == STRID_RADIXDEC)
      return L"Decimal";
    if (nStringID == STRID_RADIXOCT)
      return L"Octal";
    if (nStringID == STRID_ONLYCONVERTED)
      return L"Only converted to output";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
  }
  return L"";
}

BOOL IsExtCallParamValid(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return TRUE;
  return FALSE;
}

INT_PTR GetExtCallParam(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return *(((INT_PTR *)lParam) + nIndex);
  return 0;
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hInstanceDLL=pd->hInstanceDLL;
  bOnMainStart=pd->bOnStart;
  hMainWnd=pd->hMainWnd;
  hWndEdit=pd->hWndEdit;
  hMainIcon=pd->hMainIcon;
  bOldWindows=pd->bOldWindows;
  nMDI=pd->nMDI;
  wLangModule=PRIMARYLANGID(pd->wLangModule);
  hPopupView=GetSubMenu(pd->hPopupMenu, MENU_POPUP_VIEW);

  //Initialize WideFunc.h header
  WideInitialize();

  //Plugin name
  {
    int i;

    for (i=0; pd->wszFunction[i] != L':'; ++i)
      wszPluginName[i]=pd->wszFunction[i];
    wszPluginName[i]=L'\0';
  }
  xprintfW(wszPluginTitle, GetLangStringW(wLangModule, STRID_PLUGIN), wszPluginName);

  //Read settings
  xstrcpyW(wszHexPrefix, L"<");
  xstrcpyW(wszHexSuffix, L">");
  ReadOptions(0);
}

void InitMain()
{
  bInitMain=TRUE;

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

  if (nMDI == WMD_MDI)
  {
    NewFrameProcData=NULL;
    SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewFrameProc, (LPARAM)&NewFrameProcData);
  }
}

void UninitMain()
{
  bInitMain=FALSE;

  //Remove subclass
  if (NewMainProcData)
  {
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
    NewMainProcData=NULL;
  }
  if (NewFrameProcData)
  {
    SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&NewFrameProcData);
    NewFrameProcData=NULL;
  }
}

//Entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_DETACH)
  {
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
  }
  return TRUE;
}
