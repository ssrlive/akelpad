#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"

/*
//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"

//Include stack functions
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackDelete
#define StackClear
#define StackCopy
#include "StackFunc.h"

//Include string functions
#define xmemcpy
#define xmemcmp
#define xmemset
#define xstrlenW
#define xstrcpyW
#define xstrcpynW
#define xstrcmpW
#define xstrcmpnW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define hex2decA
#define hex2decW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define ComboBox_AddStringWide
#define CreateFontIndirectWide
#define DialogBoxWide
#define GetWindowTextWide
#define ListBox_AddStringWide
#define ListBox_InsertStringWide
#define SetDlgItemTextWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_DEFAULTSPECIALCHARS  1
#define STRID_SPECIALCHAR_EXISTS   2
#define STRID_ADD                  3
#define STRID_EDIT                 4
#define STRID_DELETE               5
#define STRID_NAME                 6
#define STRID_OLDCHAR              7
#define STRID_HEX                  8
#define STRID_SPECIAL              9
#define STRID_SOLIDINDENTLINE      10
#define STRID_INDENTLINESIZE       11
#define STRID_NEWCHAR              12
#define STRID_DEFAULT              13
#define STRID_BASICTEXT            14
#define STRID_SELTEXT              15
#define STRID_FONTSTYLE            16
#define STRID_CODERTHEME           17
#define STRID_NEWLINE              18
#define STRID_WRAP                 19
#define STRID_INDENTLINE           20
#define STRID_EOF                  21
#define STRID_IGNORE               22
#define STRID_NORMAL               23
#define STRID_BOLD                 24
#define STRID_ITALIC               25
#define STRID_BOLDITALIC           26
#define STRID_PLUGIN               27
#define STRID_OK                   28
#define STRID_CANCEL               29

#define DLLA_SPECIALCHAR_OLDSET 1
#define DLLA_SPECIALCHAR_OLDGET 2
#define DLLA_SPECIALCHAR_NEWSET 3
#define DLLA_SPECIALCHAR_NEWGET 4

#define AKDLL_UPDATEADDMODIFY    (WM_USER + 100)
#define AKDLL_UPDATENEWCHARGROUP (WM_USER + 101)

#define OF_LISTTEXT       0x1
#define OF_SETTINGS       0x2

#define SCO_SPACE        0
#define SCO_TAB          1
#define SCO_NEWLINE      2
#define SCO_VERTICALTAB  3
#define SCO_FORMFEED     4
#define SCO_NULL         5
#define SCO_WRAP         6
#define SCO_INDENTLINE   7
#define SCO_EOF          8
#define SCO_MAX          9

#define SC_ERR          -1
#define SC_NEWLINE      -2
#define SC_WRAP         -3
#define SC_INDENTLINE   -4
#define SC_EOF          -5

#define SCF_BASICENABLE      0x00001
#define SCF_BASICTEXTCOLOR   0x00004
#define SCF_BASICBKCOLOR     0x00008
#define SCF_SELENABLE        0x00100
#define SCF_SELTEXTCOLOR     0x00400
#define SCF_SELBKCOLOR       0x00800

typedef struct _SPECIALCHAR {
  struct _SPECIALCHAR *next;
  struct _SPECIALCHAR *prev;
  wchar_t wszName[MAX_PATH];
  int nOldChar;
  int nNewChar;
  DWORD dwFlags;
  DWORD dwBasicFontStyle;
  DWORD dwBasicTextColor;
  DWORD dwBasicBkColor;
  DWORD dwSelFontStyle;
  DWORD dwSelTextColor;
  DWORD dwSelBkColor;
} SPECIALCHAR;

typedef struct {
  SPECIALCHAR *first;
  SPECIALCHAR *last;
} STACKSPECIALCHAR;

//UpdateEdit flags
#define UE_ERASE        0x01
#define UE_ALLRECT      0x02
#define UE_DRAWRECT     0x04
#define UE_FIRSTPIXEL   0x08
#define UE_UPDATEWINDOW 0x10

#ifndef SPI_GETFONTSMOOTHING
  #define SPI_GETFONTSMOOTHING 0x004A
#endif
#ifndef SPI_GETFONTSMOOTHINGTYPE
  #define SPI_GETFONTSMOOTHINGTYPE 0x200A
#endif
#ifndef FE_FONTSMOOTHINGCLEARTYPE
  #define FE_FONTSMOOTHINGCLEARTYPE 0x0002
#endif

//Coder external call
#define DLLA_CODER_FILLVARLIST      23

//Variable flags
#define VARF_LOWPRIORITY   0x001 //Global variable has low priority.
                                 //Next flags for DLLA_CODER_FILLVARLIST:
#define VARF_EXTSTRING     0x100 //Copy string pointer to (const wchar_t *)CODERTHEMEITEM.nVarValue.
#define VARF_EXTINTCOLOR   0x200 //Copy color integer to (COLORREF)CODERTHEMEITEM.nVarValue or -1 if not color.
#define VARF_EXTLPINTCOLOR 0x400 //Copy color integer to (COLORREF *)CODERTHEMEITEM.nVarValue or -1 if not color.

typedef struct {
  const wchar_t *wpVarName;
  INT_PTR nVarValue;
  DWORD dwVarFlags;         //See VARF_* defines.
} CODERTHEMEITEM;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDocEdit;
  CODERTHEMEITEM *cti;
} DLLEXTCODERFILLVARLIST;

//Functions prototypes
BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
COLORREF GetIndentColor(BOOL bCharInSel, SPECIALCHAR *pscIndentDraw, AECOLORS *aec);
void GetCoderColors(HWND hWnd);
void CreateSpecialCharStack(STACKSPECIALCHAR *hStack, const wchar_t *wpText);
void FreeSpecialCharStack(STACKSPECIALCHAR *hStack);
int HexCharToValue(const wchar_t **wpText);
int ValueToHexChar(int nValue, wchar_t *wszHexChar);
int ValueToNormalChar(int nValue, wchar_t *wszNormalChar);
void SkipWhitespace(const wchar_t **wpText);
int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote);
BOOL NextLine(const wchar_t **wpText);
BOOL GetLineSpaces(const AECHARINDEX *ciMinDraw, int nTabStopSize, INT_PTR *lpnLineSpaces);
BOOL GetCharColor(HWND hWndEdit, INT_PTR nCharOffset, AECHARCOLORS *aecc);
COLORREF GetColorFromStrAnsi(char *pColor);
COLORREF GetColorFromStr(wchar_t *wpColor);
int GetNewLineString(int nNewLine, const wchar_t **wpNewLine);
BOOL GetClearType();
void UpdateEdit(HWND hWnd, DWORD dwFlags);
void UpdateEditAll(DWORD dwFlags);

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

void DrawVisualEndOfLine(HDC hDC, const RECTL *rc, COLORREF color, int lbType);

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hMdiClient;
BOOL bOldWindows;
BOOL bOldRichEdit;
BOOL bAkelEdit;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
DWORD dwSaveFlags=0;
char *szSpecialChar=NULL;
wchar_t *wszSpecialCharText=NULL;
STACKSPECIALCHAR hSpecialCharStack={0};
SPECIALCHAR *pscIndentLine=NULL;
int nIndentLineSize=0;
BOOL bIndentLineSolid=FALSE;
DWORD dwPaintOptions=0;
SPECIALCHAR scCoder;
CODERTHEMEITEM cti[]={{L"SpecialChar_BasicFontStyle", (INT_PTR)&scCoder.dwBasicFontStyle, VARF_EXTLPINTCOLOR},
                      {L"SpecialChar_BasicTextColor", (INT_PTR)&scCoder.dwBasicTextColor, VARF_EXTLPINTCOLOR},
                      {L"SpecialChar_BasicBkColor",   (INT_PTR)&scCoder.dwBasicBkColor,   VARF_EXTLPINTCOLOR},
                      {L"SpecialChar_SelFontStyle",   (INT_PTR)&scCoder.dwSelFontStyle,   VARF_EXTLPINTCOLOR},
                      {L"SpecialChar_SelTextColor",   (INT_PTR)&scCoder.dwSelTextColor,   VARF_EXTLPINTCOLOR},
                      {L"SpecialChar_SelBkColor",     (INT_PTR)&scCoder.dwSelBkColor,     VARF_EXTLPINTCOLOR},
                      {0, 0, 0}};
BOOL bCoderTheme=TRUE;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;
WNDPROCDATA *NewEditProcData=NULL;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="SpecialChar";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  //Is plugin already loaded?
  if (bInitMain)
  {
    UninitMain();
    UpdateEditAll(UE_DRAWRECT);
  }
  else
  {
    InitMain();
    if (!pd->bOnStart)
      UpdateEditAll(UE_DRAWRECT);

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
}

void __declspec(dllexport) Settings(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_SPECIALCHAR_OLDSET ||
        nAction == DLLA_SPECIALCHAR_NEWSET)
    {
      wchar_t wszOldChars[MAX_PATH];
      wchar_t wszNewChars[MAX_PATH];
      unsigned char *pOldChars=(unsigned char *)L"";
      unsigned char *pNewChars=(unsigned char *)L"";
      BOOL bBasicEnable=-2;
      DWORD dwBasicFontStyle=AEHLS_NONE;
      unsigned char *pBasicTextColor=NULL;
      unsigned char *pBasicBkColor=NULL;
      BOOL bSelEnable=-2;
      DWORD dwSelFontStyle=AEHLS_NONE;
      unsigned char *pSelTextColor=NULL;
      unsigned char *pSelBkColor=NULL;
      const wchar_t *wpNextOldChar=wszOldChars;
      const wchar_t *wpNextNewChar=wszNewChars;
      SPECIALCHAR *pscFirst=NULL;
      SPECIALCHAR *pscChar;
      int nOldChar;
      int nNewChar;
      BOOL bUpdate=FALSE;

      if (nAction == DLLA_SPECIALCHAR_OLDSET)
      {
        if (IsExtCallParamValid(pd->lParam, 2))
          pOldChars=(unsigned char *)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pBasicTextColor=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          pSelTextColor=(unsigned char *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          bBasicEnable=(BOOL)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          bSelEnable=(BOOL)GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          pNewChars=(unsigned char *)GetExtCallParam(pd->lParam, 7);
      }
      else if (nAction == DLLA_SPECIALCHAR_NEWSET)
      {
        if (IsExtCallParamValid(pd->lParam, 2))
          pOldChars=(unsigned char *)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pNewChars=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          bBasicEnable=(BOOL)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          dwBasicFontStyle=(DWORD)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          pBasicTextColor=(unsigned char *)GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          pBasicBkColor=(unsigned char *)GetExtCallParam(pd->lParam, 7);
        if (IsExtCallParamValid(pd->lParam, 8))
          bSelEnable=(BOOL)GetExtCallParam(pd->lParam, 8);
        if (IsExtCallParamValid(pd->lParam, 9))
          dwSelFontStyle=(DWORD)GetExtCallParam(pd->lParam, 9);
        if (IsExtCallParamValid(pd->lParam, 10))
          pSelTextColor=(unsigned char *)GetExtCallParam(pd->lParam, 10);
        if (IsExtCallParamValid(pd->lParam, 11))
          pSelBkColor=(unsigned char *)GetExtCallParam(pd->lParam, 11);
      }

      if (pd->dwSupport & PDS_STRANSI)
      {
        MultiByteToWideChar(CP_ACP, 0, (char *)pOldChars, -1, wszOldChars, MAX_PATH);
        MultiByteToWideChar(CP_ACP, 0, (char *)pNewChars, -1, wszNewChars, MAX_PATH);
      }
      else
      {
        xstrcpynW(wszOldChars, (wchar_t *)pOldChars, MAX_PATH);
        xstrcpynW(wszNewChars, (wchar_t *)pNewChars, MAX_PATH);
      }

      for (;;)
      {
        SkipWhitespace(&wpNextOldChar);
        if (*wpNextOldChar)
          nOldChar=HexCharToValue(&wpNextOldChar);
        else
          break;

        SkipWhitespace(&wpNextNewChar);
        if (*wpNextNewChar)
        {
          nNewChar=HexCharToValue(&wpNextNewChar);
          if (*wpNextNewChar == L',')
            ++wpNextNewChar;
        }
        else nNewChar=SC_ERR;

        if (nOldChar != SC_ERR)
        {
          if (nAction == DLLA_SPECIALCHAR_OLDSET && nOldChar <= SCO_MAX)
          {
            //Zero-based
            --nOldChar;

            if (nOldChar == SCO_SPACE)
              nOldChar=L' ';
            else if (nOldChar == SCO_TAB)
              nOldChar=L'\t';
            else if (nOldChar == SCO_NEWLINE)
              nOldChar=SC_NEWLINE;
            else if (nOldChar == SCO_VERTICALTAB)
              nOldChar=L'\v';
            else if (nOldChar == SCO_FORMFEED)
              nOldChar=L'\f';
            else if (nOldChar == SCO_NULL)
              nOldChar=L'\0';
            else if (nOldChar == SCO_WRAP)
              nOldChar=SC_WRAP;
            else if (nOldChar == SCO_INDENTLINE)
              nOldChar=SC_INDENTLINE;
            else if (nOldChar == SCO_EOF)
              nOldChar=SC_EOF;
          }

          for (pscChar=hSpecialCharStack.first; pscChar; pscChar=pscChar->next)
          {
            if (nOldChar == pscChar->nOldChar)
              break;
          }
          if (!pscChar)
          {
            if (!StackInsertIndex((stack **)&hSpecialCharStack.first, (stack **)&hSpecialCharStack.last, (stack **)&pscChar, -1, sizeof(SPECIALCHAR)))
            {
              pscChar->nOldChar=nOldChar;
              ValueToHexChar(pscChar->nOldChar, pscChar->wszName);
              if (pscChar->nOldChar == SC_INDENTLINE)
                pscIndentLine=pscChar;
            }
          }
          if (!pscFirst) pscFirst=pscChar;

          if (pscChar)
          {
            if (nNewChar != SC_ERR)
              pscChar->nNewChar=nNewChar;

            //Basic
            if (bBasicEnable != -2)
            {
              if (bBasicEnable < 0)
              {
                if (pscChar->dwFlags & SCF_BASICENABLE)
                {
                  pscChar->dwFlags&=~SCF_BASICENABLE;
                  if (bBasicEnable == -3 && pscFirst == pscChar)
                    bBasicEnable=FALSE;
                }
                else
                {
                  pscChar->dwFlags|=SCF_BASICENABLE;
                  if (bBasicEnable == -3 && pscFirst == pscChar)
                    bBasicEnable=TRUE;
                }
              }
              else
              {
                if (bBasicEnable)
                  pscChar->dwFlags|=SCF_BASICENABLE;
                else
                  pscChar->dwFlags&=~SCF_BASICENABLE;
              }
            }
            pscChar->dwBasicFontStyle=dwBasicFontStyle;

            if (pd->dwSupport & PDS_STRANSI)
            {
              if (pBasicTextColor && *(char *)pBasicTextColor == '#')
              {
                pscChar->dwBasicTextColor=GetColorFromStrAnsi((char *)pBasicTextColor + 1);
                pscChar->dwFlags|=SCF_BASICTEXTCOLOR;
              }
              if (pBasicBkColor && *(char *)pBasicBkColor == '#')
              {
                pscChar->dwBasicBkColor=GetColorFromStrAnsi((char *)pBasicBkColor + 1);
                pscChar->dwFlags|=SCF_BASICBKCOLOR;
              }
            }
            else
            {
              if (pBasicTextColor && *(wchar_t *)pBasicTextColor == L'#')
              {
                pscChar->dwBasicTextColor=GetColorFromStr((wchar_t *)pBasicTextColor + 1);
                pscChar->dwFlags|=SCF_BASICTEXTCOLOR;
              }
              if (pBasicBkColor && *(wchar_t *)pBasicBkColor == L'#')
              {
                pscChar->dwBasicBkColor=GetColorFromStr((wchar_t *)pBasicBkColor + 1);
                pscChar->dwFlags|=SCF_BASICBKCOLOR;
              }
            }
            if (!pBasicTextColor)
              pscChar->dwFlags&=~SCF_BASICTEXTCOLOR;
            if (!pBasicBkColor)
              pscChar->dwFlags&=~SCF_BASICBKCOLOR;

            //Selection
            if (bSelEnable != -2)
            {
              if (bSelEnable < 0)
              {
                if (pscChar->dwFlags & SCF_SELENABLE)
                {
                  pscChar->dwFlags&=~SCF_SELENABLE;
                  if (bSelEnable == -3 && pscFirst == pscChar)
                    bSelEnable=FALSE;
                }
                else
                {
                  pscChar->dwFlags|=SCF_SELENABLE;
                  if (bSelEnable == -3 && pscFirst == pscChar)
                    bSelEnable=TRUE;
                }
              }
              else
              {
                if (bSelEnable)
                  pscChar->dwFlags|=SCF_SELENABLE;
                else
                  pscChar->dwFlags&=~SCF_SELENABLE;
              }
            }
            pscChar->dwSelFontStyle=dwSelFontStyle;

            if (pd->dwSupport & PDS_STRANSI)
            {
              if (pSelTextColor && *(char *)pSelTextColor == '#')
              {
                pscChar->dwSelTextColor=GetColorFromStrAnsi((char *)pSelTextColor + 1);
                pscChar->dwFlags|=SCF_SELTEXTCOLOR;
              }
              if (pSelBkColor && *(char *)pSelBkColor == '#')
              {
                pscChar->dwSelBkColor=GetColorFromStrAnsi((char *)pSelBkColor + 1);
                pscChar->dwFlags|=SCF_SELBKCOLOR;
              }
            }
            else
            {
              if (pSelTextColor && *(wchar_t *)pSelTextColor == L'#')
              {
                pscChar->dwSelTextColor=GetColorFromStr((wchar_t *)pSelTextColor + 1);
                pscChar->dwFlags|=SCF_SELTEXTCOLOR;
              }
              if (pSelBkColor && *(wchar_t *)pSelBkColor == L'#')
              {
                pscChar->dwSelBkColor=GetColorFromStr((wchar_t *)pSelBkColor + 1);
                pscChar->dwFlags|=SCF_SELBKCOLOR;
              }
            }
            if (!pSelTextColor)
              pscChar->dwFlags&=~SCF_SELTEXTCOLOR;
            if (!pSelBkColor)
              pscChar->dwFlags&=~SCF_SELBKCOLOR;

            bUpdate=TRUE;
          }
        }
        if (*wpNextOldChar == L',')
          ++wpNextOldChar;
        else
          break;
      }

      if (bUpdate)
      {
        UpdateEditAll(UE_DRAWRECT);
        dwSaveFlags|=OF_LISTTEXT;
      }
    }
    else if (nAction == DLLA_SPECIALCHAR_OLDGET ||
             nAction == DLLA_SPECIALCHAR_NEWGET)
    {
      wchar_t wszOldChars[MAX_PATH];
      wchar_t wszNewChars[MAX_PATH];
      unsigned char *pOldChars=NULL;
      unsigned char *pNewChar=NULL;
      BOOL *lpbBasicEnable=NULL;
      DWORD *lpdwBasicFontStyle=NULL;
      COLORREF *lpcrBasicTextColor=NULL;
      COLORREF *lpcrBasicBkColor=NULL;
      BOOL *lpbSelEnable=NULL;
      DWORD *lpdwSelFontStyle=NULL;
      COLORREF *lpcrSelTextColor=NULL;
      COLORREF *lpcrSelBkColor=NULL;
      const wchar_t *wpNextOldChar=wszOldChars;
      SPECIALCHAR *pscChar;
      int nOldChar;
      BOOL bCheckAll=FALSE;
      BOOL bBasicEnable=TRUE;
      BOOL bSelEnable=TRUE;

      if (nAction == DLLA_SPECIALCHAR_OLDGET)
      {
        if (IsExtCallParamValid(pd->lParam, 2))
          pOldChars=(unsigned char *)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          lpcrBasicTextColor=(COLORREF *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          lpcrSelTextColor=(COLORREF *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          lpbBasicEnable=(BOOL *)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          lpbSelEnable=(BOOL *)GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          pNewChar=(unsigned char *)GetExtCallParam(pd->lParam, 7);
      }
      else if (nAction == DLLA_SPECIALCHAR_NEWGET)
      {
        if (IsExtCallParamValid(pd->lParam, 2))
          pOldChars=(unsigned char *)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pNewChar=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          lpbBasicEnable=(BOOL *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          lpdwBasicFontStyle=(DWORD *)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          lpcrBasicTextColor=(COLORREF *)GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          lpcrBasicBkColor=(COLORREF *)GetExtCallParam(pd->lParam, 7);
        if (IsExtCallParamValid(pd->lParam, 8))
          lpbSelEnable=(BOOL *)GetExtCallParam(pd->lParam, 8);
        if (IsExtCallParamValid(pd->lParam, 9))
          lpdwSelFontStyle=(DWORD *)GetExtCallParam(pd->lParam, 9);
        if (IsExtCallParamValid(pd->lParam, 10))
          lpcrSelTextColor=(COLORREF *)GetExtCallParam(pd->lParam, 10);
        if (IsExtCallParamValid(pd->lParam, 11))
          lpcrSelBkColor=(COLORREF *)GetExtCallParam(pd->lParam, 11);
      }

      if (pd->dwSupport & PDS_STRANSI)
        MultiByteToWideChar(CP_ACP, 0, (char *)pOldChars, -1, wszOldChars, MAX_PATH);
      else
        xstrcpynW(wszOldChars, (wchar_t *)pOldChars, MAX_PATH);
      if ((lpbBasicEnable || lpbSelEnable) && !pNewChar && !lpcrBasicTextColor && !lpcrBasicBkColor && !lpcrSelTextColor && !lpcrSelBkColor)
        bCheckAll=TRUE;

      for (;;)
      {
        SkipWhitespace(&wpNextOldChar);
        nOldChar=HexCharToValue(&wpNextOldChar);

        if (nOldChar != SC_ERR)
        {
          if (nAction == DLLA_SPECIALCHAR_OLDGET && nOldChar <= SCO_MAX)
          {
            //Zero-based
            --nOldChar;

            if (nOldChar == SCO_SPACE)
              nOldChar=L' ';
            else if (nOldChar == SCO_TAB)
              nOldChar=L'\t';
            else if (nOldChar == SCO_NEWLINE)
              nOldChar=SC_NEWLINE;
            else if (nOldChar == SCO_VERTICALTAB)
              nOldChar=L'\v';
            else if (nOldChar == SCO_FORMFEED)
              nOldChar=L'\f';
            else if (nOldChar == SCO_NULL)
              nOldChar=L'\0';
            else if (nOldChar == SCO_WRAP)
              nOldChar=SC_WRAP;
            else if (nOldChar == SCO_INDENTLINE)
              nOldChar=SC_INDENTLINE;
            else if (nOldChar == SCO_EOF)
              nOldChar=SC_EOF;
          }
          for (pscChar=hSpecialCharStack.first; pscChar; pscChar=pscChar->next)
          {
            if (nOldChar == pscChar->nOldChar)
              break;
          }
          if (pscChar)
          {
            if (pNewChar)
            {
              ValueToHexChar(pscChar->nNewChar, wszNewChars);
              if (pd->dwSupport & PDS_STRANSI)
                WideCharToMultiByte(CP_ACP, 0, wszNewChars, -1, (char *)pNewChar, MAX_PATH, NULL, NULL);
              else
                xstrcpynW((wchar_t *)pNewChar, wszNewChars, MAX_PATH);
            }

            if (lpbBasicEnable)
            {
              if (pscChar->dwFlags & SCF_BASICENABLE)
                *lpbBasicEnable=TRUE;
              else
              {
                *lpbBasicEnable=FALSE;
                bBasicEnable=FALSE;
              }
            }
            if (lpcrBasicTextColor)
              *lpcrBasicTextColor=pscChar->dwBasicTextColor;
            if (lpcrBasicBkColor)
              *lpcrBasicBkColor=pscChar->dwBasicBkColor;

            if (lpbSelEnable)
            {
              if (pscChar->dwFlags & SCF_SELENABLE)
                *lpbSelEnable=TRUE;
              else
              {
                *lpbSelEnable=FALSE;
                bSelEnable=FALSE;
              }
            }
            if (lpcrSelTextColor)
              *lpcrSelTextColor=pscChar->dwSelTextColor;
            if (lpcrSelBkColor)
              *lpcrSelBkColor=pscChar->dwSelBkColor;
          }
          else
          {
            bBasicEnable=FALSE;
            bSelEnable=FALSE;
          }

          //Check only first special char?
          if (!bCheckAll) break;
          if (!bBasicEnable && !bSelEnable) break;
        }
        else
        {
          bBasicEnable=FALSE;
          bSelEnable=FALSE;
        }

        if (*wpNextOldChar == L',')
          ++wpNextOldChar;
        else
          break;
      }
      if (bCheckAll)
      {
        if (lpbBasicEnable) *lpbBasicEnable=bBasicEnable;
        if (lpbSelEnable) *lpbSelEnable=bSelEnable;
      }
    }

    //If plugin already loaded, stay in memory and don't change active status
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
    return;
  }

  DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SETUP), hMainWnd, (DLGPROC)SetupDlgProc);

  //If plugin already loaded, stay in memory, but show as non-active
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}

BOOL CALLBACK SetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndList;
  static HWND hWndAddModifyButton;
  static HWND hWndDeleteButton;
  static HWND hWndNameEdit;
  static HWND hWndOldCharHexRadio;
  static HWND hWndOldCharHexEdit;
  static HWND hWndOldCharHexPreview;
  static HWND hWndOldCharSpecialRadio;
  static HWND hWndOldCharSpecialCombo;
  static HWND hWndSolidIndentLineCheck;
  static HWND hWndIndentLineSizeLabel;
  static HWND hWndIndentLineSizeEdit;
  static HWND hWndIndentLineSizeSpin;
  static HWND hWndNewCharHexEdit;
  static HWND hWndNewCharHexPreview;
  static HWND hWndNewCharBasicTextCheck;
  static HWND hWndNewCharBasicTextCustomRadio;
  static HWND hWndNewCharBasicTextCustomButton;
  static HWND hWndNewCharBasicTextDefaultRadio;
  static HWND hWndNewCharSelTextCheck;
  static HWND hWndNewCharSelTextCustomRadio;
  static HWND hWndNewCharSelTextCustomButton;
  static HWND hWndNewCharSelTextDefaultRadio;
  static HWND hWndNewCharFontStyleLabel;
  static HWND hWndNewCharFontStyleCombo;
  static HWND hWndCoderThemeCheck;
  static STACKSPECIALCHAR hSpecialCharDlgStack;
  static SPECIALCHAR scDlgChar;
  static SPECIALCHAR *pscCurChar;
  static int nCurItem;
  static BOOL bListChanged;
  SPECIALCHAR *pscChar;
  int nIndex;
  BOOL bEnable;

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndList=GetDlgItem(hDlg, IDC_LIST);
    hWndAddModifyButton=GetDlgItem(hDlg, IDC_ADDMODIFY_BUTTON);
    hWndDeleteButton=GetDlgItem(hDlg, IDC_DELETE_BUTTON);
    hWndNameEdit=GetDlgItem(hDlg, IDC_NAME_EDIT);
    hWndOldCharHexRadio=GetDlgItem(hDlg, IDC_OLDCHARHEX_RADIO);
    hWndOldCharHexEdit=GetDlgItem(hDlg, IDC_OLDCHARHEX_EDIT);
    hWndOldCharHexPreview=GetDlgItem(hDlg, IDC_OLDCHARHEX_PREVIEW);
    hWndOldCharSpecialRadio=GetDlgItem(hDlg, IDC_OLDCHARSPECIAL_RADIO);
    hWndOldCharSpecialCombo=GetDlgItem(hDlg, IDC_OLDCHARSPECIAL_COMBO);
    hWndSolidIndentLineCheck=GetDlgItem(hDlg, IDC_SOLIDINDENTLINE_CHECK);
    hWndIndentLineSizeLabel=GetDlgItem(hDlg, IDC_INDENTLINESIZE_LABEL);
    hWndIndentLineSizeEdit=GetDlgItem(hDlg, IDC_INDENTLINESIZE_EDIT);
    hWndIndentLineSizeSpin=GetDlgItem(hDlg, IDC_INDENTLINESIZE_SPIN);
    hWndNewCharHexEdit=GetDlgItem(hDlg, IDC_NEWCHARHEX_EDIT);
    hWndNewCharHexPreview=GetDlgItem(hDlg, IDC_NEWCHARHEX_PREVIEW);
    hWndNewCharBasicTextCheck=GetDlgItem(hDlg, IDC_NEWCHARBASICTEXT_CHECK);
    hWndNewCharBasicTextCustomRadio=GetDlgItem(hDlg, IDC_NEWCHARBASICTEXTCUSTOM_RADIO);
    hWndNewCharBasicTextCustomButton=GetDlgItem(hDlg, IDC_NEWCHARBASICTEXTCUSTOM_BUTTON);
    hWndNewCharBasicTextDefaultRadio=GetDlgItem(hDlg, IDC_NEWCHARBASICTEXTDEFAULT_RADIO);
    hWndNewCharSelTextCheck=GetDlgItem(hDlg, IDC_NEWCHARSELTEXT_CHECK);
    hWndNewCharSelTextCustomRadio=GetDlgItem(hDlg, IDC_NEWCHARSELTEXTCUSTOM_RADIO);
    hWndNewCharSelTextCustomButton=GetDlgItem(hDlg, IDC_NEWCHARSELTEXTCUSTOM_BUTTON);
    hWndNewCharSelTextDefaultRadio=GetDlgItem(hDlg, IDC_NEWCHARSELTEXTDEFAULT_RADIO);
    hWndNewCharFontStyleLabel=GetDlgItem(hDlg, IDC_NEWCHARFONTSTYLE_LABEL);
    hWndNewCharFontStyleCombo=GetDlgItem(hDlg, IDC_NEWCHARFONTSTYLE_COMBO);
    hWndCoderThemeCheck=GetDlgItem(hDlg, IDC_CODERTHEME_CHECK);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_DELETE_BUTTON, GetLangStringW(wLangModule, STRID_DELETE));
    SetDlgItemTextWide(hDlg, IDC_NAME_LABEL, GetLangStringW(wLangModule, STRID_NAME));
    SetDlgItemTextWide(hDlg, IDC_OLDCHAR_GROUP, GetLangStringW(wLangModule, STRID_OLDCHAR));
    SetDlgItemTextWide(hDlg, IDC_OLDCHARHEX_RADIO, GetLangStringW(wLangModule, STRID_HEX));
    SetDlgItemTextWide(hDlg, IDC_OLDCHARSPECIAL_RADIO, GetLangStringW(wLangModule, STRID_SPECIAL));
    SetDlgItemTextWide(hDlg, IDC_SOLIDINDENTLINE_CHECK, GetLangStringW(wLangModule, STRID_SOLIDINDENTLINE));
    SetDlgItemTextWide(hDlg, IDC_INDENTLINESIZE_LABEL, GetLangStringW(wLangModule, STRID_INDENTLINESIZE));
    SetDlgItemTextWide(hDlg, IDC_NEWCHAR_GROUP, GetLangStringW(wLangModule, STRID_NEWCHAR));
    SetDlgItemTextWide(hDlg, IDC_NEWCHARHEX_LABEL, GetLangStringW(wLangModule, STRID_HEX));
    SetDlgItemTextWide(hDlg, IDC_NEWCHARBASICTEXT_CHECK, GetLangStringW(wLangModule, STRID_BASICTEXT));
    SetDlgItemTextWide(hDlg, IDC_NEWCHARBASICTEXTDEFAULT_RADIO, GetLangStringW(wLangModule, STRID_DEFAULT));
    SetDlgItemTextWide(hDlg, IDC_NEWCHARSELTEXT_CHECK, GetLangStringW(wLangModule, STRID_SELTEXT));
    SetDlgItemTextWide(hDlg, IDC_NEWCHARSELTEXTDEFAULT_RADIO, GetLangStringW(wLangModule, STRID_DEFAULT));
    SetDlgItemTextWide(hDlg, IDC_NEWCHARFONTSTYLE_LABEL, GetLangStringW(wLangModule, STRID_FONTSTYLE));
    SetDlgItemTextWide(hDlg, IDC_CODERTHEME_CHECK, GetLangStringW(wLangModule, STRID_CODERTHEME));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    ComboBox_AddStringWide(hWndOldCharSpecialCombo, GetLangStringW(wLangModule, STRID_NEWLINE));
    ComboBox_AddStringWide(hWndOldCharSpecialCombo, GetLangStringW(wLangModule, STRID_WRAP));
    ComboBox_AddStringWide(hWndOldCharSpecialCombo, GetLangStringW(wLangModule, STRID_INDENTLINE));
    ComboBox_AddStringWide(hWndOldCharSpecialCombo, GetLangStringW(wLangModule, STRID_EOF));

    ComboBox_AddStringWide(hWndNewCharFontStyleCombo, GetLangStringW(wLangModule, STRID_IGNORE));
    ComboBox_AddStringWide(hWndNewCharFontStyleCombo, GetLangStringW(wLangModule, STRID_NORMAL));
    ComboBox_AddStringWide(hWndNewCharFontStyleCombo, GetLangStringW(wLangModule, STRID_BOLD));
    ComboBox_AddStringWide(hWndNewCharFontStyleCombo, GetLangStringW(wLangModule, STRID_ITALIC));
    ComboBox_AddStringWide(hWndNewCharFontStyleCombo, GetLangStringW(wLangModule, STRID_BOLDITALIC));

    SendMessage(hWndIndentLineSizeSpin, UDM_SETRANGE, 0, MAKELONG(999, 0));
    SendMessage(hWndIndentLineSizeSpin, UDM_SETBUDDY, (WPARAM)hWndIndentLineSizeEdit, 0);
    SetDlgItemInt(hDlg, IDC_INDENTLINESIZE_EDIT, nIndentLineSize, FALSE);

    SendMessage(hWndSolidIndentLineCheck, BM_SETCHECK, bIndentLineSolid, 0);

    SendMessage(hWndCoderThemeCheck, BM_SETCHECK, bCoderTheme, 0);

    FreeSpecialCharStack(&hSpecialCharDlgStack);
    StackCopy((stack *)hSpecialCharStack.first, (stack *)hSpecialCharStack.last, (stack **)&hSpecialCharDlgStack.first, (stack **)&hSpecialCharDlgStack.last, sizeof(SPECIALCHAR));
    bListChanged=FALSE;

    for (pscChar=hSpecialCharDlgStack.first; pscChar; pscChar=pscChar->next)
    {
      nIndex=ListBox_AddStringWide(hWndList, pscChar->wszName);
      SendMessage(hWndList, LB_SETITEMDATA, (WPARAM)nIndex, (LPARAM)pscChar);
    }

    SendMessage(hWndList, LB_SETCURSEL, (WPARAM)nCurItem, 0);
    SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_LIST, LBN_SELCHANGE), (LPARAM)hWndList);
    SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_OLDCHARSPECIAL_COMBO, CBN_SELENDOK), (LPARAM)hWndOldCharSpecialCombo);
    SendMessage(hDlg, WM_COMMAND, IDC_CODERTHEME_CHECK, 0);
  }
  else if (uMsg == AKDLL_UPDATEADDMODIFY)
  {
    BOOL bEnable;

    if (scDlgChar.nOldChar == SC_ERR || (pscCurChar && !xmemcmp(scDlgChar.wszName, pscCurChar->wszName, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName))))
      bEnable=FALSE;
    else
      bEnable=TRUE;

    if (bEnable)
    {
      if (pscCurChar && scDlgChar.nOldChar == pscCurChar->nOldChar)
        SetWindowTextWide(hWndAddModifyButton, GetLangStringW(wLangModule, STRID_EDIT));
      else
        SetWindowTextWide(hWndAddModifyButton, GetLangStringW(wLangModule, STRID_ADD));
    }
    else SetWindowTextWide(hWndAddModifyButton, GetLangStringW(wLangModule, STRID_EDIT));

    EnableWindow(hWndAddModifyButton, bEnable);
    EnableWindow(hWndDeleteButton, (BOOL)(INT_PTR)pscCurChar);

    //Update indent line dependencies
    if (scDlgChar.nOldChar == SC_INDENTLINE)
    {
      ShowWindow(hWndSolidIndentLineCheck, SW_SHOW);
      ShowWindow(hWndIndentLineSizeLabel, SW_SHOW);
      ShowWindow(hWndIndentLineSizeEdit, SW_SHOW);
      ShowWindow(hWndIndentLineSizeSpin, SW_SHOW);
      EnableWindow(hWndNewCharHexEdit, FALSE);
    }
    else
    {
      ShowWindow(hWndSolidIndentLineCheck, SW_HIDE);
      ShowWindow(hWndIndentLineSizeLabel, SW_HIDE);
      ShowWindow(hWndIndentLineSizeEdit, SW_HIDE);
      ShowWindow(hWndIndentLineSizeSpin, SW_HIDE);
      EnableWindow(hWndNewCharHexEdit, TRUE);
    }

    if (!GetFocus()) SetFocus(hWndList);
  }
  else if (uMsg == AKDLL_UPDATENEWCHARGROUP)
  {
    BOOL bEnable;

    if ((scDlgChar.dwFlags & SCF_BASICENABLE) || (scDlgChar.dwFlags & SCF_SELENABLE))
      bEnable=TRUE;
    else
      bEnable=FALSE;

    EnableWindow(hWndNewCharHexEdit, bEnable);
    EnableWindow(hWndNewCharFontStyleLabel, bEnable);
    EnableWindow(hWndNewCharFontStyleCombo, bEnable);
  }
  else if (uMsg == WM_DRAWITEM)
  {
    DRAWITEMSTRUCT *dis=(DRAWITEMSTRUCT *)lParam;
    HBRUSH hBrush;
    DWORD dwColor=GetSysColor(COLOR_BTNFACE);

    if (wParam == IDC_NEWCHARBASICTEXTCUSTOM_BUTTON)
    {
      if (scDlgChar.dwFlags & SCF_BASICTEXTCOLOR)
        dwColor=scDlgChar.dwBasicTextColor;
    }
    else if (wParam == IDC_NEWCHARSELTEXTCUSTOM_BUTTON)
    {
      if (scDlgChar.dwFlags & SCF_SELTEXTCOLOR)
        dwColor=scDlgChar.dwSelTextColor;
    }

    if (hBrush=CreateSolidBrush(dwColor))
    {
      FillRect(dis->hDC, &dis->rcItem, hBrush);
      if (dis->hwndItem == GetFocus())
        DrawFocusRect(dis->hDC, &dis->rcItem);
      DeleteObject(hBrush);
    }
    return TRUE;
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_LIST)
    {
      if (HIWORD(wParam) == LBN_SELCHANGE)
      {
        nCurItem=(int)SendMessage(hWndList, LB_GETCURSEL, 0, 0);
        if (nCurItem == LB_ERR)
        {
          pscCurChar=NULL;
          xmemset(scDlgChar.wszName, 0, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName));
        }
        else
        {
          pscCurChar=(SPECIALCHAR *)SendMessage(hWndList, LB_GETITEMDATA, (WPARAM)nCurItem, 0);
          xmemcpy(scDlgChar.wszName, pscCurChar->wszName, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName));
        }

        //Update controls
        SetWindowTextWide(hWndNameEdit, scDlgChar.wszName);

        if (scDlgChar.nOldChar >= 0)
        {
          SendMessage(hWndOldCharHexRadio, BM_SETCHECK, BST_CHECKED, 0);
          SendMessage(hWndOldCharSpecialRadio, BM_SETCHECK, BST_UNCHECKED, 0);
          SendMessage(hWndOldCharSpecialCombo, CB_SETCURSEL, (WPARAM)-1, 0);

          ValueToHexChar(scDlgChar.nOldChar, wszBuffer);
          SetWindowTextWide(hWndOldCharHexEdit, wszBuffer + 2);
        }
        else
        {
          SendMessage(hWndOldCharHexRadio, BM_SETCHECK, BST_UNCHECKED, 0);
          SendMessage(hWndOldCharSpecialRadio, BM_SETCHECK, BST_CHECKED, 0);
          if (scDlgChar.nOldChar == SC_ERR)
            SendMessage(hWndOldCharSpecialCombo, CB_SETCURSEL, (WPARAM)-1, 0);
          else
            SendMessage(hWndOldCharSpecialCombo, CB_SETCURSEL, (WPARAM)(-2 - scDlgChar.nOldChar), 0);
          SetWindowTextWide(hWndOldCharHexEdit, L"");
          SetWindowTextWide(hWndOldCharHexPreview, L"");
        }

        ValueToHexChar(scDlgChar.nNewChar, wszBuffer);
        SetWindowTextWide(hWndNewCharHexEdit, wszBuffer + 2);

        if (scDlgChar.dwFlags & SCF_BASICENABLE)
          SendMessage(hWndNewCharBasicTextCheck, BM_SETCHECK, BST_CHECKED, 0);
        else
          SendMessage(hWndNewCharBasicTextCheck, BM_SETCHECK, BST_UNCHECKED, 0);
        if (scDlgChar.dwFlags & SCF_BASICTEXTCOLOR)
        {
          SendMessage(hWndNewCharBasicTextCustomRadio, BM_SETCHECK, BST_CHECKED, 0);
          SendMessage(hWndNewCharBasicTextDefaultRadio, BM_SETCHECK, BST_UNCHECKED, 0);
        }
        else
        {
          SendMessage(hWndNewCharBasicTextCustomRadio, BM_SETCHECK, BST_UNCHECKED, 0);
          SendMessage(hWndNewCharBasicTextDefaultRadio, BM_SETCHECK, BST_CHECKED, 0);
        }

        SendMessage(hDlg, WM_COMMAND, IDC_NEWCHARBASICTEXT_CHECK, 0);

        if (scDlgChar.dwFlags & SCF_SELENABLE)
          SendMessage(hWndNewCharSelTextCheck, BM_SETCHECK, BST_CHECKED, 0);
        else
          SendMessage(hWndNewCharSelTextCheck, BM_SETCHECK, BST_UNCHECKED, 0);
        if (scDlgChar.dwFlags & SCF_SELTEXTCOLOR)
        {
          SendMessage(hWndNewCharSelTextCustomRadio, BM_SETCHECK, BST_CHECKED, 0);
          SendMessage(hWndNewCharSelTextDefaultRadio, BM_SETCHECK, BST_UNCHECKED, 0);
        }
        else
        {
          SendMessage(hWndNewCharSelTextCustomRadio, BM_SETCHECK, BST_UNCHECKED, 0);
          SendMessage(hWndNewCharSelTextDefaultRadio, BM_SETCHECK, BST_CHECKED, 0);
        }

        if (scDlgChar.dwBasicFontStyle == scDlgChar.dwSelFontStyle)
          SendMessage(hWndNewCharFontStyleCombo, CB_SETCURSEL, (WPARAM)scDlgChar.dwBasicFontStyle, 0);
        else
          SendMessage(hWndNewCharFontStyleCombo, CB_SETCURSEL, (WPARAM)-1, 0);

        SendMessage(hDlg, WM_COMMAND, IDC_NEWCHARSELTEXT_CHECK, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_ADDMODIFY_BUTTON)
    {
      if (pscCurChar && scDlgChar.nOldChar == pscCurChar->nOldChar)
      {
        if (xstrcmpW(pscCurChar->wszName, scDlgChar.wszName))
        {
          SendMessage(hWndList, LB_DELETESTRING, (WPARAM)nCurItem, 0);
          ListBox_InsertStringWide(hWndList, nCurItem, scDlgChar.wszName);
          SendMessage(hWndList, LB_SETITEMDATA, (WPARAM)nCurItem, (LPARAM)pscCurChar);
          SendMessage(hWndList, LB_SETCURSEL, (WPARAM)nCurItem, 0);
        }
        xmemcpy(pscCurChar->wszName, scDlgChar.wszName, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName));
        bListChanged=TRUE;
      }
      else
      {
        for (pscChar=hSpecialCharDlgStack.first; pscChar; pscChar=pscChar->next)
        {
          if (scDlgChar.nOldChar != SC_ERR && scDlgChar.nOldChar == pscChar->nOldChar)
            break;
        }
        if (pscChar)
        {
          xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_SPECIALCHAR_EXISTS), pscChar->wszName);
          MessageBoxW(hDlg, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
        }
        else
        {
          if (!StackInsertIndex((stack **)&hSpecialCharDlgStack.first, (stack **)&hSpecialCharDlgStack.last, (stack **)&pscChar, -1, sizeof(SPECIALCHAR)))
          {
            if (!*scDlgChar.wszName)
              ValueToHexChar(scDlgChar.nOldChar, scDlgChar.wszName);

            xmemcpy(pscChar->wszName, scDlgChar.wszName, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName));
            nIndex=ListBox_AddStringWide(hWndList, pscChar->wszName);
            SendMessage(hWndList, LB_SETITEMDATA, (WPARAM)nIndex, (LPARAM)pscChar);
            SendMessage(hWndList, LB_SETCURSEL, (WPARAM)nIndex, 0);
            SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_LIST, LBN_SELCHANGE), (LPARAM)hWndList);

            bListChanged=TRUE;
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_DELETE_BUTTON)
    {
      if (pscCurChar)
      {
        StackDelete((stack **)&hSpecialCharDlgStack.first, (stack **)&hSpecialCharDlgStack.last, (stack *)pscCurChar);
        pscCurChar=NULL;

        SendMessage(hWndList, LB_DELETESTRING, (WPARAM)nCurItem, 0);
        nCurItem=-1;

        SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_LIST, LBN_SELCHANGE), (LPARAM)hWndList);
        bListChanged=TRUE;
      }
    }
    else if (LOWORD(wParam) == IDC_NAME_EDIT)
    {
      GetWindowTextWide(hWndNameEdit, scDlgChar.wszName, MAX_PATH);
    }
    else if (LOWORD(wParam) == IDC_OLDCHARSPECIAL_COMBO)
    {
      if (HIWORD(wParam) == CBN_SELENDOK)
      {
        nIndex=(int)SendMessage(hWndOldCharSpecialCombo, CB_GETCURSEL, 0, 0);

        if (nIndex != LB_ERR)
        {
          scDlgChar.nOldChar=-2 - nIndex;
        }
        if (scDlgChar.nOldChar == SC_INDENTLINE)
        {
          ShowWindow(hWndSolidIndentLineCheck, SW_SHOW);
          ShowWindow(hWndIndentLineSizeLabel, SW_SHOW);
          ShowWindow(hWndIndentLineSizeEdit, SW_SHOW);
          ShowWindow(hWndIndentLineSizeSpin, SW_SHOW);
        }
        else
        {
          ShowWindow(hWndSolidIndentLineCheck, SW_HIDE);
          ShowWindow(hWndIndentLineSizeLabel, SW_HIDE);
          ShowWindow(hWndIndentLineSizeEdit, SW_HIDE);
          ShowWindow(hWndIndentLineSizeSpin, SW_HIDE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_OLDCHARHEX_EDIT ||
             LOWORD(wParam) == IDC_OLDCHARHEX_RADIO ||
             LOWORD(wParam) == IDC_OLDCHARSPECIAL_RADIO)
    {
      if (SendMessage(hWndOldCharHexRadio, BM_GETCHECK, 0, 0) == BST_CHECKED)
      {
        GetWindowTextWide(hWndOldCharHexEdit, wszBuffer, BUFFER_SIZE);
        scDlgChar.nOldChar=(int)hex2decW(wszBuffer, -1, NULL);
        ValueToNormalChar(scDlgChar.nOldChar, wszBuffer);
        SetWindowTextWide(hWndOldCharHexPreview, wszBuffer);
        EnableWindow(hWndOldCharHexEdit, TRUE);
        EnableWindow(hWndOldCharSpecialCombo, FALSE);
      }
      else
      {
        SendMessage(hDlg, WM_COMMAND, MAKELONG(IDC_OLDCHARSPECIAL_COMBO, CBN_SELENDOK), (LPARAM)hWndOldCharSpecialCombo);
        EnableWindow(hWndOldCharHexEdit, FALSE);
        EnableWindow(hWndOldCharSpecialCombo, TRUE);
      }
    }
    else if (LOWORD(wParam) == IDC_NEWCHARHEX_EDIT)
    {
      GetWindowTextWide(hWndNewCharHexEdit, wszBuffer, BUFFER_SIZE);
      scDlgChar.nNewChar=(int)hex2decW(wszBuffer, -1, NULL);
      ValueToNormalChar(scDlgChar.nNewChar, wszBuffer);
      SetWindowTextWide(hWndNewCharHexPreview, wszBuffer);
    }
    else if (LOWORD(wParam) == IDC_NEWCHARBASICTEXT_CHECK)
    {
      bEnable=(BOOL)SendMessage(hWndNewCharBasicTextCheck, BM_GETCHECK, 0, 0);
      if (bEnable)
        scDlgChar.dwFlags|=SCF_BASICENABLE;
      else
        scDlgChar.dwFlags&=~SCF_BASICENABLE;

      EnableWindow(hWndNewCharBasicTextCustomRadio, bEnable);
      EnableWindow(hWndNewCharBasicTextCustomButton, bEnable && (scDlgChar.dwFlags & SCF_BASICTEXTCOLOR));
      EnableWindow(hWndNewCharBasicTextDefaultRadio, bEnable);
      InvalidateRect(hWndNewCharBasicTextCustomButton, NULL, FALSE);

      SendMessage(hDlg, AKDLL_UPDATENEWCHARGROUP, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_NEWCHARBASICTEXTCUSTOM_RADIO ||
             LOWORD(wParam) == IDC_NEWCHARBASICTEXTDEFAULT_RADIO)
    {
      if (SendMessage(hWndNewCharBasicTextCustomRadio, BM_GETCHECK, 0, 0) == BST_CHECKED)
        scDlgChar.dwFlags|=SCF_BASICTEXTCOLOR;
      else
        scDlgChar.dwFlags&=~SCF_BASICTEXTCOLOR;

      EnableWindow(hWndNewCharBasicTextCustomButton, (scDlgChar.dwFlags & SCF_BASICTEXTCOLOR));
      InvalidateRect(hWndNewCharBasicTextCustomButton, NULL, FALSE);
    }
    else if (LOWORD(wParam) == IDC_NEWCHARSELTEXT_CHECK)
    {
      bEnable=(BOOL)SendMessage(hWndNewCharSelTextCheck, BM_GETCHECK, 0, 0);
      if (bEnable)
        scDlgChar.dwFlags|=SCF_SELENABLE;
      else
        scDlgChar.dwFlags&=~SCF_SELENABLE;

      EnableWindow(hWndNewCharSelTextCustomRadio, bEnable);
      EnableWindow(hWndNewCharSelTextCustomButton, bEnable && (scDlgChar.dwFlags & SCF_SELTEXTCOLOR));
      EnableWindow(hWndNewCharSelTextDefaultRadio, bEnable);
      InvalidateRect(hWndNewCharSelTextCustomButton, NULL, FALSE);

      SendMessage(hDlg, AKDLL_UPDATENEWCHARGROUP, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_NEWCHARSELTEXTCUSTOM_RADIO ||
             LOWORD(wParam) == IDC_NEWCHARSELTEXTDEFAULT_RADIO)
    {
      if (SendMessage(hWndNewCharSelTextCustomRadio, BM_GETCHECK, 0, 0) == BST_CHECKED)
        scDlgChar.dwFlags|=SCF_SELTEXTCOLOR;
      else
        scDlgChar.dwFlags&=~SCF_SELTEXTCOLOR;

      EnableWindow(hWndNewCharSelTextCustomButton, (scDlgChar.dwFlags & SCF_SELTEXTCOLOR));
      InvalidateRect(hWndNewCharSelTextCustomButton, NULL, FALSE);
    }
    else if (LOWORD(wParam) == IDC_NEWCHARBASICTEXTCUSTOM_BUTTON ||
             LOWORD(wParam) == IDC_NEWCHARSELTEXTCUSTOM_BUTTON)
    {
      static COLORREF crCustColors[16];
      COLORREF *lpcrColor=NULL;
      HWND hWndButton=NULL;

      if (LOWORD(wParam) == IDC_NEWCHARBASICTEXTCUSTOM_BUTTON)
      {
        lpcrColor=&scDlgChar.dwBasicTextColor;
        hWndButton=hWndNewCharBasicTextCustomButton;
      }
      else if (LOWORD(wParam) == IDC_NEWCHARSELTEXTCUSTOM_BUTTON)
      {
        lpcrColor=&scDlgChar.dwSelTextColor;
        hWndButton=hWndNewCharSelTextCustomButton;
      }

      if (bOldWindows)
      {
        CHOOSECOLORA ccA;

        xmemset(&ccA, 0, sizeof(CHOOSECOLORA));
        ccA.lStructSize  =sizeof(CHOOSECOLORA);
        ccA.hwndOwner    =hDlg;
        ccA.lpCustColors =crCustColors;
        ccA.Flags        =CC_FULLOPEN|CC_RGBINIT;
        ccA.rgbResult    =*lpcrColor;

        if (ChooseColorA(&ccA))
        {
          *lpcrColor=ccA.rgbResult;
          InvalidateRect(hWndButton, NULL, FALSE);
        }
      }
      else
      {
        CHOOSECOLORW ccW;

        xmemset(&ccW, 0, sizeof(CHOOSECOLORW));
        ccW.lStructSize  =sizeof(CHOOSECOLORW);
        ccW.hwndOwner    =hDlg;
        ccW.lpCustColors =crCustColors;
        ccW.Flags        =CC_FULLOPEN|CC_RGBINIT;
        ccW.rgbResult    =*lpcrColor;

        if (ChooseColorW(&ccW))
        {
          *lpcrColor=ccW.rgbResult;
          InvalidateRect(hWndButton, NULL, FALSE);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_NEWCHARFONTSTYLE_COMBO)
    {
      if (HIWORD(wParam) == CBN_SELENDOK)
      {
        nIndex=(int)SendMessage(hWndNewCharFontStyleCombo, CB_GETCURSEL, 0, 0);
        if (nIndex != LB_ERR)
        {
          scDlgChar.dwBasicFontStyle=nIndex;
          scDlgChar.dwSelFontStyle=nIndex;
        }
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      int i;

      if (pscCurChar && scDlgChar.nOldChar == pscCurChar->nOldChar && xmemcmp(scDlgChar.wszName, pscCurChar->wszName, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName)))
      {
        //We accept changes for current special char
        xmemcpy(pscCurChar->wszName, scDlgChar.wszName, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName));
        bListChanged=TRUE;
      }

      if (bListChanged)
      {
        FreeSpecialCharStack(&hSpecialCharStack);
        hSpecialCharStack.first=hSpecialCharDlgStack.first;
        hSpecialCharStack.last=hSpecialCharDlgStack.last;
        hSpecialCharDlgStack.first=NULL;
        hSpecialCharDlgStack.last=NULL;
        pscIndentLine=NULL;

        for (pscChar=hSpecialCharStack.first; pscChar; pscChar=pscChar->next)
        {
          if (pscChar->nOldChar == SC_INDENTLINE)
          {
            pscIndentLine=pscChar;
            break;
          }
        }
        dwSaveFlags|=OF_LISTTEXT;
      }
      else FreeSpecialCharStack(&hSpecialCharDlgStack);

      i=GetDlgItemInt(hDlg, IDC_INDENTLINESIZE_EDIT, NULL, FALSE);
      if (nIndentLineSize != i)
      {
        nIndentLineSize=i;
        dwSaveFlags|=OF_SETTINGS;
      }
      i=(BOOL)SendMessage(hWndSolidIndentLineCheck, BM_GETCHECK, 0, 0);
      if (bIndentLineSolid != i)
      {
        bIndentLineSolid=i;
        dwSaveFlags|=OF_SETTINGS;
      }
      i=(BOOL)SendMessage(hWndCoderThemeCheck, BM_GETCHECK, 0, 0);
      if (bCoderTheme != i)
      {
        bCoderTheme=i;
        dwSaveFlags|=OF_SETTINGS;
      }

      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }
      EndDialog(hDlg, 0);
      UpdateEditAll(UE_DRAWRECT);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      FreeSpecialCharStack(&hSpecialCharDlgStack);
      EndDialog(hDlg, 0);
      return TRUE;
    }
    PostMessage(hDlg, AKDLL_UPDATEADDMODIFY, 0, 0);
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
  if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

    UninitMain();
    return FALSE;
  }

  //Special messages
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
  //Special messages
  {
    LRESULT lResult;

    if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
      return lResult;
  }

  //Call next procedure
  return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_PAINT)
  {
    DWORD dwEventMask;

    //Enable AEN_PAINT
    dwEventMask=(DWORD)SendMessage(hWnd, AEM_GETEVENTMASK, 0, 0);
    if (!(dwEventMask & AENM_PAINT))
      SendMessage(hWnd, AEM_SETEVENTMASK, 0, dwEventMask|AENM_PAINT);
  }

  //Call next procedure
  return NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT)
    {
      if (((NMHDR *)lParam)->code == AEN_PAINT)
      {
        AENPAINT *pnt=(AENPAINT *)lParam;
        RECT rcDraw;
        static AECOLORS aec;
        static AESELECTION aes;
        static HBRUSH hbrSelBk;
        static INT_PTR nPrevLineSpaces;
        static INT_PTR nNextLineSpaces;
        static int nInitTopStartLine;
        static int nInitTopEndLine;
        static int nPrevLine;
        static int nNextLine;
        static int nLinesInPage;
        static int nCharHeight;
        static int nCharHeightNoGap;
        static int nAveCharWidth;
        static int nSpaceWidth;
        static int nTabStopSize;
        static int nIndentSpaces;
        static BOOL bIntCall;
        static BOOL bClearType;

        if (pnt->dwType == AEPNT_BEGIN)
        {
          aec.dwFlags=AECLR_ALL;
          SendMessage(pnt->hdr.hwndFrom, AEM_GETCOLORS, 0, (LPARAM)&aec);
          hbrSelBk=CreateSolidBrush(aec.crSelBk);
          bClearType=GetClearType();
          GetCoderColors(pnt->hdr.hwndFrom);

          SendMessage(pnt->hdr.hwndFrom, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&aes);
          nCharHeight=(int)SendMessage(pnt->hdr.hwndFrom, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
          nCharHeightNoGap=nCharHeight - (int)SendMessage(pnt->hdr.hwndFrom, AEM_GETLINEGAP, 0, 0);
          nAveCharWidth=(int)SendMessage(pnt->hdr.hwndFrom, AEM_GETCHARSIZE, AECS_AVEWIDTH, 0);
          nSpaceWidth=(int)SendMessage(pnt->hdr.hwndFrom, AEM_GETCHARSIZE, AECS_SPACEWIDTH, 0);
          nTabStopSize=(int)SendMessage(pnt->hdr.hwndFrom, AEM_GETTABSTOP, 0, 0);
          if (!nIndentLineSize)
            nIndentSpaces=nTabStopSize;
          else
            nIndentSpaces=nIndentLineSize;
          if (pscIndentLine)
          {
            SendMessage(pnt->hdr.hwndFrom, AEM_GETRECT, 0, (LPARAM)&rcDraw);
            nLinesInPage=(rcDraw.bottom - rcDraw.top) / nCharHeight;
          }
          nPrevLineSpaces=-1;
          nNextLineSpaces=-1;
          nInitTopStartLine=-1;
          nInitTopEndLine=-1;
          nPrevLine=-1;
          nNextLine=-1;
        }
        else if (pnt->dwType == AEPNT_DRAWLINE)
        {
          AECHARINDEX ciCount=pnt->ciMinDraw;
          SPECIALCHAR *pscChar;
          SPECIALCHAR *pscIndentDraw;
          DWORD dwFontStyle;
          COLORREF crTextColor;
          COLORREF crTextColorPrev=0;
          COLORREF crBkColor;
          COLORREF crIndentColor;
          COLORREF crIndentColorPrev;
          POINT pt;
          INT_PTR nOffset=pnt->nMinDrawOffset;
          INT_PTR nLineSpaces=0;
          INT_PTR nMaxLineSpaces=0;
          int nChar;
          int nBkModePrev;
          BOOL bIndentFound=FALSE;
          BOOL bCharInSel;

          nBkModePrev=SetBkMode(pnt->hDC, TRANSPARENT);

          if (pscIndentLine && ((pscIndentLine->dwFlags & SCF_BASICENABLE) || (pscIndentLine->dwFlags & SCF_SELENABLE)))
          {
            if (!pnt->ciMinDraw.lpLine->prev || pnt->ciMinDraw.lpLine->prev->nLineBreak != AELB_WRAP)
            {
              bIndentFound=GetLineSpaces(&pnt->ciMinDraw, nTabStopSize, &nLineSpaces);
            }

            if (bIndentFound || (!bIntCall && nInitTopStartLine == -1))
            {
              if (nPrevLineSpaces == -1)
              {
                ciCount=pnt->ciMinDraw;
                nMaxLineSpaces=0;

                while (AEC_PrevLine(&ciCount) && pnt->ciMinDraw.nLine - ciCount.nLine <= nLinesInPage)
                {
                  ciCount.nCharInLine=ciCount.lpLine->nLineLen;
                  if (!GetLineSpaces(&ciCount, nTabStopSize, &nPrevLineSpaces))
                    break;
                  nMaxLineSpaces=max(nPrevLineSpaces, nMaxLineSpaces);
                }
                nPrevLineSpaces=max(nPrevLineSpaces, nMaxLineSpaces);
                nPrevLine=ciCount.nLine;
              }
              if (nNextLineSpaces == -1)
              {
                ciCount=pnt->ciMinDraw;
                nMaxLineSpaces=0;

                while (AEC_NextLine(&ciCount) && ciCount.nLine - pnt->ciMinDraw.nLine <= nLinesInPage)
                {
                  ciCount.nCharInLine=ciCount.lpLine->nLineLen;
                  if (!GetLineSpaces(&ciCount, nTabStopSize, &nNextLineSpaces))
                    break;
                  nMaxLineSpaces=max(nNextLineSpaces, nMaxLineSpaces);
                }
                nNextLineSpaces=max(nNextLineSpaces, nMaxLineSpaces);
                nNextLine=ciCount.nLine;
              }
              if (nInitTopStartLine == -1)
              {
                nInitTopStartLine=nPrevLine + 1;
                nInitTopEndLine=pnt->ciMinDraw.nLine - 1;
              }
              nMaxLineSpaces=max(nPrevLineSpaces, nNextLineSpaces);
            }
          }

          ciCount=pnt->ciMinDraw;

          do
          {
            pscIndentDraw=NULL;

            if (ciCount.lpLine->nLineBreak == AELB_WRAP &&
                ciCount.nCharInLine >= ciCount.lpLine->nLineLen)
            {
              nChar=-AELB_WRAP;
            }
            else nChar=AEC_CharAtIndex(&ciCount);

            for (pscChar=hSpecialCharStack.first; pscChar; pscChar=pscChar->next)
            {
              if ((nChar == -AELB_EOF && pscChar->nOldChar == SC_EOF) ||
                  (nChar == -AELB_WRAP && pscChar->nOldChar == SC_WRAP) ||
                  (nChar >= -AELB_RRN && nChar <= -AELB_R && pscChar->nOldChar == SC_NEWLINE) ||
                  (nChar >= 0 && nChar == pscChar->nOldChar))
              {
                break;
              }
            }
            if (bIndentFound)
            {
              if (nChar >= 0)
              {
                if (nChar == L' ')
                  ++nLineSpaces;
                else if (nChar != L'\t')
                  bIndentFound=FALSE;

                if (bIndentFound && ciCount.nCharInLine &&
                    (nTabStopSize == 1 ||
                     (nChar == L' ' && (nLineSpaces % nIndentSpaces) == 1) ||
                     (nChar == L'\t' && (nLineSpaces % nIndentSpaces) == 0)))
                  pscIndentDraw=pscIndentLine;

                if (nChar == L'\t')
                  nLineSpaces+=nTabStopSize - nLineSpaces % nTabStopSize;
              }
              else pscIndentDraw=pscIndentLine;
            }

            if (pscChar || pscIndentDraw)
            {
              //Get draw char color
              dwFontStyle=AEHLS_NONE;
              crTextColor=(DWORD)-1;
              crBkColor=(DWORD)-1;
              crIndentColor=(DWORD)-1;

              if ((aes.dwFlags & AESELT_COLUMNON) ?
                    AEC_IsCharInSelection(&ciCount) :
                    (AEC_IndexCompare(&ciCount, &aes.crSel.ciMin) >= 0 &&
                     AEC_IndexCompare(&ciCount, &aes.crSel.ciMax) < 0))
              {
                if (pscChar && (pscChar->dwFlags & SCF_SELENABLE))
                {
                  if (scCoder.dwSelFontStyle != (DWORD)-1)
                    dwFontStyle=scCoder.dwSelFontStyle;
                  else
                    dwFontStyle=pscChar->dwSelFontStyle;

                  if (scCoder.dwSelTextColor != (DWORD)-1)
                    crTextColor=scCoder.dwSelTextColor;
                  else
                  {
                    if (pscChar->dwFlags & SCF_SELTEXTCOLOR)
                      crTextColor=pscChar->dwSelTextColor;
                    else
                      crTextColor=aec.crSelText;
                  }

                  if (scCoder.dwSelBkColor != (DWORD)-1)
                    crBkColor=scCoder.dwSelBkColor;
                  else
                  {
                    if (pscChar->dwFlags & SCF_SELBKCOLOR)
                      crBkColor=pscChar->dwSelBkColor;
                  }
                }
                bCharInSel=TRUE;
              }
              else
              {
                if (pscChar && (pscChar->dwFlags & SCF_BASICENABLE))
                {
                  if (scCoder.dwBasicFontStyle != (DWORD)-1)
                    dwFontStyle=scCoder.dwBasicFontStyle;
                  else
                    dwFontStyle=pscChar->dwBasicFontStyle;

                  if (scCoder.dwBasicTextColor != (DWORD)-1)
                    crTextColor=scCoder.dwBasicTextColor;
                  else
                  {
                    if (pscChar->dwFlags & SCF_BASICTEXTCOLOR)
                      crTextColor=pscChar->dwBasicTextColor;
                    else
                      crTextColor=aec.crBasicText;
                  }

                  if (scCoder.dwBasicBkColor != (DWORD)-1)
                    crBkColor=scCoder.dwBasicBkColor;
                  else
                  {
                    if (pscChar->dwFlags & SCF_BASICBKCOLOR)
                      crBkColor=pscChar->dwBasicBkColor;
                  }
                }
                bCharInSel=FALSE;
              }
              crIndentColor=GetIndentColor(bCharInSel, pscIndentDraw, &aec);

              if ((pscChar && (crTextColor != (DWORD)-1 || crBkColor != (DWORD)-1)) || pscIndentDraw)
              {
                SendMessage(pnt->hdr.hwndFrom, AEM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)&ciCount);

                if (pscChar && (crTextColor != (DWORD)-1 || crBkColor != (DWORD)-1))
                {
                  if (crTextColor != (DWORD)-1 && pscChar->nOldChar == L'\t' && pscChar->nNewChar == L'\0')
                  {
                    //Draw tabulation arrow
                    HPEN hPen;
                    HPEN hPenOld;
                    int nTabWidth;
                    int nMargin=1;

                    hPen=CreatePen(PS_SOLID, 0, crTextColor);
                    hPenOld=(HPEN)SelectObject(pnt->hDC, hPen);
                    if (nTabWidth=(int)SendMessage(pnt->hdr.hwndFrom, AEM_GETCHARSIZE, AECS_INDEXWIDTH, (LPARAM)&ciCount))
                      nTabWidth-=nMargin * 2;

                    MoveToEx(pnt->hDC, pt.x + nMargin, pt.y + nCharHeightNoGap / 2, NULL);
                    LineTo(pnt->hDC, pt.x + nTabWidth, pt.y + nCharHeightNoGap / 2);
                    LineTo(pnt->hDC, pt.x + nTabWidth - nCharHeightNoGap / 4, (pt.y + nCharHeightNoGap / 2) - nCharHeightNoGap / 4);
                    MoveToEx(pnt->hDC, pt.x + nTabWidth, pt.y + nCharHeightNoGap / 2, NULL);
                    LineTo(pnt->hDC, pt.x + nTabWidth - nCharHeightNoGap / 4, (pt.y + nCharHeightNoGap / 2) + nCharHeightNoGap / 4);

                    if (hPenOld) SelectObject(pnt->hDC, hPenOld);
                    if (hPen) DeleteObject(hPen);
                  }
                  else if (crTextColor != (DWORD)-1 &&
                            ((pscChar->nOldChar == SC_NEWLINE && pscChar->nNewChar == L'\0') ||
                             (pscChar->nOldChar == SC_EOF && pscChar->nNewChar == L'\0')))
                  {
                    //Draw new line identificator
#if 0
                    HBRUSH hBrush;
                    HBRUSH hBrushOld;
                    HPEN hPen;
                    HPEN hPenOld;
                    const wchar_t *wpNewLine;
                    int nNewLineLen;
                    RECT rcBrush;
                    SIZE sizeNewLine;
                    int nPosX=pt.x;
                    int nStrMargin=1;

                    if (pscChar->nOldChar == SC_NEWLINE)
                    {
                      //New line string: "r", "n", "rn", "rrn".
                      nNewLineLen=GetNewLineString(ciCount.lpLine->nLineBreak, &wpNewLine);
                    }
                    else
                    {
                      //End of file string: "eof".
                      wpNewLine=L"eof";
                      nNewLineLen=3;
                    }
                    GetTextExtentPoint32W(pnt->hDC, wpNewLine, nNewLineLen, &sizeNewLine);
                    nPosX+=nStrMargin;
                    sizeNewLine.cx+=nStrMargin * 2;

                    if (!(dwPaintOptions & PAINT_NONEWLINEDRAW) && bCharInSel)
                    {
                      //Selection rectangle.
                      rcBrush.left=nPosX;
                      rcBrush.top=pt.y;
                      rcBrush.right=nPosX + sizeNewLine.cx + nStrMargin;
                      rcBrush.bottom=pt.y + nCharHeight;
                      FillRect(pnt->hDC, &rcBrush, hbrSelBk);
                    }

                    //Draw rounded rectangle.
                    hPen=CreatePen(PS_SOLID, 0, crTextColor);
                    hPenOld=(HPEN)SelectObject(pnt->hDC, hPen);
                    hBrush=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
                    hBrushOld=(HBRUSH)SelectObject(pnt->hDC, hBrush);
                    RoundRect(pnt->hDC, nPosX, pt.y, nPosX + sizeNewLine.cx, pt.y + nCharHeightNoGap, sizeNewLine.cx / 3, nCharHeightNoGap / 3);
                    if (hBrushOld) SelectObject(pnt->hDC, hBrushOld);
                    if (hPenOld) SelectObject(pnt->hDC, hPenOld);
                    if (hPen) DeleteObject(hPen);

                    //Draw new line string.
                    crTextColorPrev=SetTextColor(pnt->hDC, crTextColor);
                    TextOutW(pnt->hDC, nPosX + nStrMargin, pt.y, wpNewLine, nNewLineLen);
                    SetTextColor(pnt->hDC, crTextColorPrev);
#else
                    SIZE sizeNewLine;
                    int nStrMargin=1;
                    GetTextExtentPoint32W(pnt->hDC, L"  ", 2, &sizeNewLine);
                    pt.x+=nStrMargin;
                    sizeNewLine.cx+=nStrMargin * 2;
                    {
                        RECTL rc = {pt.x, pt.y, pt.x + sizeNewLine.cx, pt.y + nCharHeightNoGap};
                        DrawVisualEndOfLine(pnt->hDC, &rc, crTextColor, ciCount.lpLine->nLineBreak);
                    }
#endif
                  }
                  else if (crTextColor != (DWORD)-1 && pscChar->nOldChar == SC_WRAP && pscChar->nNewChar == L'\0')
                  {
                    //Draw wrap line arrow
                    HPEN hPen;
                    HPEN hPenOld;
                    int nMarginX=1;
                    int nMarginY=3;

                    hPen=CreatePen(PS_SOLID, 0, crTextColor);
                    hPenOld=(HPEN)SelectObject(pnt->hDC, hPen);

                    if (!ciCount.lpLine->prev || ciCount.lpLine->prev->nLineBreak != AELB_WRAP)
                    {
                      //First wrap line
                      MoveToEx(pnt->hDC, (pt.x + nMarginX + 1) + nAveCharWidth / 2 - nCharHeightNoGap / 4, pt.y + nCharHeightNoGap / 4, NULL);
                      LineTo(pnt->hDC, (pt.x + nMarginX) + nAveCharWidth / 2, pt.y + nCharHeightNoGap / 4);
                      LineTo(pnt->hDC, (pt.x + nMarginX) + nAveCharWidth / 2, pt.y + nCharHeightNoGap - nMarginY);
                    }
                    else
                    {
                      MoveToEx(pnt->hDC, (pt.x + nMarginX) + nAveCharWidth / 2, pt.y + nMarginY, NULL);
                      LineTo(pnt->hDC, (pt.x + nMarginX) + nAveCharWidth / 2, pt.y + nCharHeightNoGap - nMarginY);
                    }
                    LineTo(pnt->hDC, (pt.x + nMarginX) + nAveCharWidth / 2 - nCharHeightNoGap / 4, (pt.y + nCharHeightNoGap - nMarginY) - nCharHeightNoGap / 4);
                    MoveToEx(pnt->hDC, (pt.x + nMarginX) + nAveCharWidth / 2, pt.y + nCharHeightNoGap - nMarginY, NULL);
                    LineTo(pnt->hDC, (pt.x + nMarginX) + nAveCharWidth / 2 + nCharHeightNoGap / 4, (pt.y + nCharHeightNoGap - nMarginY) - nCharHeightNoGap / 4);

                    if (hPenOld) SelectObject(pnt->hDC, hPenOld);
                    if (hPen) DeleteObject(hPen);
                  }
                  else
                  {
                    AECHARCOLORS aecc;
                    COLORREF crOldBk=0;
                    HFONT hCharFont;
                    HFONT hOldFont=NULL;
                    wchar_t wszChar[10];
                    int nCharLen;

                    //Erase old
                    if (nChar >= 0 &&
                        nChar != L' ' &&
                        nChar != L'\t')
                    {
                      aecc.dwFlags=0;
                      GetCharColor(pnt->hdr.hwndFrom, nOffset, &aecc);

                      if (bClearType)
                      {
                        SetBkMode(pnt->hDC, nBkModePrev);
                        crOldBk=SetBkColor(pnt->hDC, aecc.crBk);
                      }
                      crTextColorPrev=SetTextColor(pnt->hDC, aecc.crBk);
                      hCharFont=(HFONT)SendMessage(pnt->hdr.hwndFrom, AEM_GETFONT, aecc.dwFontStyle, 0);
                      hOldFont=(HFONT)SelectObject(pnt->hDC, hCharFont);

                      nCharLen=ValueToNormalChar(pscChar->nOldChar, wszChar);
                      TextOutW(pnt->hDC, pt.x, pt.y, wszChar, nCharLen);

                      SelectObject(pnt->hDC, hOldFont);
                      SetTextColor(pnt->hDC, crTextColorPrev);
                      if (bClearType)
                      {
                        SetBkColor(pnt->hDC, crOldBk);
                        SetBkMode(pnt->hDC, TRANSPARENT);
                      }
                    }

                    //Draw new
                    if (crTextColor != (DWORD)-1)
                    {
                      crTextColorPrev=SetTextColor(pnt->hDC, crTextColor);
                      if (dwFontStyle != AEHLS_NONE)
                      {
                        hCharFont=(HFONT)SendMessage(pnt->hdr.hwndFrom, AEM_GETFONT, dwFontStyle, 0);
                        hOldFont=(HFONT)SelectObject(pnt->hDC, hCharFont);
                      }
                    }
                    if (crBkColor != (DWORD)-1)
                    {
                      crOldBk=SetBkColor(pnt->hDC, crBkColor);
                      SetBkMode(pnt->hDC, nBkModePrev);
                    }

                    nCharLen=ValueToNormalChar(pscChar->nNewChar, wszChar);
                    TextOutW(pnt->hDC, pt.x, pt.y, wszChar, nCharLen);

                    if (crBkColor != (DWORD)-1)
                    {
                      SetBkColor(pnt->hDC, crOldBk);
                      SetBkMode(pnt->hDC, TRANSPARENT);
                    }
                    if (crTextColor != (DWORD)-1)
                    {
                      SetTextColor(pnt->hDC, crTextColorPrev);
                      if (dwFontStyle != AEHLS_NONE)
                        SelectObject(pnt->hDC, hOldFont);
                    }
                  }
                }
                if (pscIndentDraw)
                {
                  //Draw indent line
                  HPEN hPen=NULL;
                  HPEN hPenOld=NULL;
                  POINT64 ptGlobal;
                  int nTop;
                  int nBottom;
                  int nLeft;
                  int i;

                  if (crIndentColor != (DWORD)-1)
                  {
                    hPen=CreatePen(PS_SOLID, 0, crIndentColor);
                    hPenOld=(HPEN)SelectObject(pnt->hDC, hPen);
                  }

                  if (!bIndentLineSolid)
                  {
                    ptGlobal.x=pt.x;
                    ptGlobal.y=pt.y;
                    SendMessage(pnt->hdr.hwndFrom, AEM_CONVERTPOINT, AECPT_CLIENTTOGLOBAL, (LPARAM)&ptGlobal);

                    nTop=pt.y + !(ptGlobal.y % 2);
                  }
                  else
                    nTop=pt.y;
                  nBottom=pt.y + nCharHeight;

                  if (nChar >= 0)
                  {
                    //Inside line
                    if (crIndentColor != (DWORD)-1)
                    {
                      nLeft=pt.x;
  
                      if (!bIndentLineSolid)
                      {
                        for (i=nTop; i < nBottom; i+=2)
                        {
                          //Draw dot
                          MoveToEx(pnt->hDC, nLeft, i, NULL);
                          LineTo(pnt->hDC, nLeft + 1, i + 1);
                        }
                      }
                      else
                      {
                        MoveToEx(pnt->hDC, nLeft, nTop, NULL);
                        LineTo(pnt->hDC, nLeft, nBottom);
                      }
                    }
                  }
                  else
                  {
                    //Outside line
                    if (nLineSpaces % nIndentSpaces)
                      nLineSpaces+=nIndentSpaces - nLineSpaces % nIndentSpaces;

                    while (nLineSpaces < nMaxLineSpaces)
                    {
                      if (crIndentColor != (DWORD)-1 && nLineSpaces)
                      {
                        nLeft=(int)((pt.x - ciCount.lpLine->nLineWidth) + nLineSpaces * nSpaceWidth);

                        if (!bIndentLineSolid)
                        {
                          for (i=nTop; i < nBottom; i+=2)
                          {
                            //Draw dot
                            MoveToEx(pnt->hDC, nLeft, i, NULL);
                            LineTo(pnt->hDC, nLeft + 1, i + 1);
                          }
                        }
                        else
                        {
                          MoveToEx(pnt->hDC, nLeft, nTop, NULL);
                          LineTo(pnt->hDC, nLeft, nBottom);
                        }
                      }
                      nLineSpaces+=nIndentSpaces;

                      //Next indent line in selection?
                      if (ciCount.lpLine->nSelStart <= ciCount.nCharInLine + nLineSpaces && ciCount.nCharInLine + nLineSpaces < ciCount.lpLine->nSelEnd)
                        bCharInSel=TRUE;
                      else
                        bCharInSel=FALSE;
                      crIndentColorPrev=crIndentColor;
                      crIndentColor=GetIndentColor(bCharInSel, pscIndentDraw, &aec);
                      if (crIndentColorPrev != crIndentColor)
                      {
                        if (hPenOld)
                        {
                          SelectObject(pnt->hDC, hPenOld);
                          hPenOld=NULL;
                        }
                        if (hPen)
                        {
                          DeleteObject(hPen);
                          hPen=NULL;
                        }
                        if (crIndentColor != (DWORD)-1)
                        {
                          hPen=CreatePen(PS_SOLID, 0, crIndentColor);
                          hPenOld=(HPEN)SelectObject(pnt->hDC, hPen);
                        }
                      }
                    }
                  }
                  if (hPenOld) SelectObject(pnt->hDC, hPenOld);
                  if (hPen) DeleteObject(hPen);
                }
              }
            }

            //Next char
            AEC_IndexInc(&ciCount);
            if (ciCount.nCharInLine > ciCount.lpLine->nLineLen)
              if (!AEC_NextLine(&ciCount))
                break;

            ++nOffset;
          }
          while (AEC_IndexCompare(&ciCount, &pnt->ciMaxDraw) <= 0);

          if (nNextLine <= pnt->ciMaxDraw.nLine)
            nNextLineSpaces=-1;
          nPrevLineSpaces=nLineSpaces;
          SetBkMode(pnt->hDC, nBkModePrev);
        }
        else if (pnt->dwType == AEPNT_END)
        {
          int nInitBottomStartLine=pnt->ciMaxDraw.nLine + 1;
          int nInitBottomEndLine=nNextLine - 1;

          if (hbrSelBk) DeleteObject(hbrSelBk);

          if (!bIntCall && ((nInitTopEndLine >= 0 && nInitTopStartLine <= nInitTopEndLine) ||
                            (nInitBottomEndLine >= 0 && nInitBottomStartLine <= nInitBottomEndLine)))
          {
            bIntCall=TRUE;

            if (nInitTopEndLine >= 0 && nInitTopStartLine <= nInitTopEndLine)
              SendMessage(pnt->hdr.hwndFrom, AEM_REDRAWLINERANGE, (WPARAM)nInitTopStartLine, (LPARAM)nInitTopEndLine);
            if (nInitBottomEndLine >= 0 && nInitBottomStartLine <= nInitBottomEndLine)
              SendMessage(pnt->hdr.hwndFrom, AEM_REDRAWLINERANGE, (WPARAM)nInitBottomStartLine, (LPARAM)nInitBottomEndLine);
            UpdateWindow(pnt->hdr.hwndFrom);
          }
          bIntCall=FALSE;
        }
      }
    }
  }
  return 0;
}

COLORREF GetIndentColor(BOOL bCharInSel, SPECIALCHAR *pscIndentDraw, AECOLORS *aec)
{
  COLORREF crIndentColor=(DWORD)-1;

  if (bCharInSel)
  {
    if (pscIndentDraw && (pscIndentDraw->dwFlags & SCF_SELENABLE))
    {
      if (scCoder.dwSelTextColor != (DWORD)-1)
        crIndentColor=scCoder.dwSelTextColor;
      else
      {
        if (pscIndentDraw->dwFlags & SCF_SELTEXTCOLOR)
          crIndentColor=pscIndentDraw->dwSelTextColor;
        else
          crIndentColor=aec->crSelText;
      }
    }
  }
  else
  {
    if (pscIndentDraw && (pscIndentDraw->dwFlags & SCF_BASICENABLE))
    {
      if (scCoder.dwBasicTextColor != (DWORD)-1)
        crIndentColor=scCoder.dwBasicTextColor;
      else
      {
        if (pscIndentDraw->dwFlags & SCF_BASICTEXTCOLOR)
          crIndentColor=pscIndentDraw->dwBasicTextColor;
        else
          crIndentColor=aec->crBasicText;
      }
    }
  }
  return crIndentColor;
}

void GetCoderColors(HWND hWnd)
{
  int i;

  //Default colors
  for (i=0; cti[i].wpVarName; ++i)
  {
    *(COLORREF *)cti[i].nVarValue=(COLORREF)-1;
  }

  if (bCoderTheme)
  {
    PLUGINFUNCTION *pfCoder;
    PLUGINCALLSENDW pcs;
    DLLEXTCODERFILLVARLIST decfvl;

    if ((pfCoder=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfCoder->bRunning)
    {
      decfvl.dwStructSize=sizeof(DLLEXTCODERFILLVARLIST);
      decfvl.nAction=DLLA_CODER_FILLVARLIST;
      decfvl.hWndEdit=hWnd;
      decfvl.hDocEdit=NULL;
      decfvl.cti=cti;

      pcs.pFunction=L"Coder::Settings";
      pcs.lParam=(LPARAM)&decfvl;
      pcs.dwSupport=PDS_STRWIDE;
      SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
    }
  }
}

void CreateSpecialCharStack(STACKSPECIALCHAR *hStack, const wchar_t *wpText)
{
  SPECIALCHAR sc;
  SPECIALCHAR *pscChar;

  xmemset(&sc, 0, sizeof(SPECIALCHAR));
  pscIndentLine=NULL;

  if (wpText)
  {
    for (; *wpText; NextLine(&wpText))
    {
      GetWord(wpText, sc.wszName, MAX_PATH, &wpText, NULL);

      SkipWhitespace(&wpText);
      sc.nOldChar=HexCharToValue(&wpText);

      SkipWhitespace(&wpText);
      sc.nNewChar=HexCharToValue(&wpText);

      SkipWhitespace(&wpText);
      sc.dwFlags=(DWORD)xatoiW(wpText, &wpText);

      SkipWhitespace(&wpText);
      sc.dwBasicFontStyle=(DWORD)xatoiW(wpText, &wpText);

      SkipWhitespace(&wpText);
      sc.dwBasicTextColor=(DWORD)xatoiW(wpText, &wpText);

      SkipWhitespace(&wpText);
      sc.dwBasicBkColor=(DWORD)xatoiW(wpText, &wpText);

      SkipWhitespace(&wpText);
      sc.dwSelFontStyle=(DWORD)xatoiW(wpText, &wpText);

      SkipWhitespace(&wpText);
      sc.dwSelTextColor=(DWORD)xatoiW(wpText, &wpText);

      SkipWhitespace(&wpText);
      sc.dwSelBkColor=(DWORD)xatoiW(wpText, &wpText);

      if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&pscChar, -1, sizeof(SPECIALCHAR)))
      {
        xmemcpy(pscChar->wszName, sc.wszName, sizeof(SPECIALCHAR) - offsetof(SPECIALCHAR, wszName));
        if (pscChar->nOldChar == SC_INDENTLINE)
          pscIndentLine=pscChar;
      }
    }
  }
}

void FreeSpecialCharStack(STACKSPECIALCHAR *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

int HexCharToValue(const wchar_t **wpText)
{
  const wchar_t *wpCount=*wpText;
  const wchar_t *wpCharStart;
  int nValue;

  if (*wpCount == L'\\' && *(wpCount + 1) == L'x')
  {
    wpCharStart=wpCount + 2;
    while (*wpCount != L',' && *wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\n' && *wpCount != L'\0')
      ++wpCount;
    nValue=(int)hex2decW(wpCharStart, wpCount - wpCharStart, NULL);
  }
  else nValue=(int)xatoiW(wpCount, &wpCount);

  *wpText=wpCount;
  return nValue;
}

int ValueToHexChar(int nValue, wchar_t *wszHexChar)
{
  if (nValue < 0)
    return (int)xprintfW(wszHexChar, L"%d", nValue);

  if (nValue >= 0x10000) //Unicode scalar value of surrogate pair
    return (int)xprintfW(wszHexChar, L"\\x%06X", nValue);
  else
    return (int)xprintfW(wszHexChar, L"\\x%04X", nValue);
}

int ValueToNormalChar(int nValue, wchar_t *wszNormalChar)
{
  if (nValue >= 0x10000) //Unicode surrogate pair
  {
    wszNormalChar[0]=AEC_HighSurrogateFromScalar(nValue);
    wszNormalChar[1]=AEC_LowSurrogateFromScalar(nValue);
    wszNormalChar[2]=L'\0';
    return 2;
  }
  else
  {
    wszNormalChar[0]=(wchar_t)nValue;
    wszNormalChar[1]=L'\0';
    return 1;
  }
}

void SkipWhitespace(const wchar_t **wpText)
{
  while (**wpText == L' ' || **wpText == L'\t') ++*wpText;
}

int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote)
{
  const wchar_t *wpCount;
  wchar_t wchStopChar;

  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  if (*wpText == L'\"' || *wpText == L'\'' || *wpText == L'`')
  {
    if (lpbQuote) *lpbQuote=TRUE;
    wchStopChar=*wpText;
    wpCount=++wpText;

    //Parse: "param" or 'param' or `param`
    while (*wpCount != wchStopChar && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
    {
      *wppNextWord=wpCount;
      if (*wpCount == wchStopChar)
        ++*wppNextWord;
    }
  }
  else
  {
    if (lpbQuote) *lpbQuote=FALSE;
    wpCount=wpText;

    //Parse: param1 param2 param3
    while (*wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
      *wppNextWord=wpCount;
  }
  return (int)xstrcpynW(wszWord, wpText, min(nWordMax, wpCount - wpText + 1));
}

BOOL NextLine(const wchar_t **wpText)
{
  while (**wpText != L'\r' && **wpText != L'\n' && **wpText != L'\0') ++*wpText;
  if (**wpText == L'\0') return FALSE;
  if (**wpText == L'\r') ++*wpText;
  if (**wpText == L'\n') ++*wpText;
  return TRUE;
}

BOOL GetLineSpaces(const AECHARINDEX *ciMinDraw, int nTabStopSize, INT_PTR *lpnLineSpaces)
{
  AECHARINDEX ciCount;
  INT_PTR nLineSpaces=0;

  ciCount.nLine=ciMinDraw->nLine;
  ciCount.lpLine=ciMinDraw->lpLine;
  ciCount.nCharInLine=0;

  while (ciCount.nCharInLine < ciMinDraw->nCharInLine)
  {
    if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L' ')
      ++nLineSpaces;
    else if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\t')
      nLineSpaces+=nTabStopSize - nLineSpaces % nTabStopSize;
    else
    {
      *lpnLineSpaces=nLineSpaces;
      return FALSE;
    }
    AEC_IndexInc(&ciCount);
  }
  *lpnLineSpaces=nLineSpaces;
  return TRUE;
}

BOOL GetCharColor(HWND hWndEdit, INT_PTR nCharOffset, AECHARCOLORS *aecc)
{
  AECHARINDEX ciCharIndex;

  SendMessage(hWndEdit, AEM_RICHOFFSETTOINDEX, (WPARAM)nCharOffset, (LPARAM)&ciCharIndex);
  return (BOOL)SendMessage(hWndEdit, AEM_GETCHARCOLORS, (WPARAM)&ciCharIndex, (LPARAM)aecc);
}

COLORREF GetColorFromStrAnsi(char *pColor)
{
  COLORREF crColor;

  if ((crColor=(COLORREF)hex2decA(pColor, 6, NULL)) != (COLORREF)-1)
    crColor=RGB(GetBValue(crColor), GetGValue(crColor), GetRValue(crColor));
  return crColor;
}

COLORREF GetColorFromStr(wchar_t *wpColor)
{
  COLORREF crColor;

  if ((crColor=(COLORREF)hex2decW(wpColor, 6, NULL)) != (COLORREF)-1)
    crColor=RGB(GetBValue(crColor), GetGValue(crColor), GetRValue(crColor));
  return crColor;
}

int GetNewLineString(int nNewLine, const wchar_t **wpNewLine)
{
  if (nNewLine == AELB_R)
  {
    if (wpNewLine) *wpNewLine=L"r";
    return 1;
  }
  else if (nNewLine == AELB_N)
  {
    if (wpNewLine) *wpNewLine=L"n";
    return 1;
  }
  else if (nNewLine == AELB_RN)
  {
    if (wpNewLine) *wpNewLine=L"rn";
    return 2;
  }
  else if (nNewLine == AELB_RRN)
  {
    if (wpNewLine) *wpNewLine=L"rrn";
    return 3;
  }
  if (wpNewLine) *wpNewLine=L"rn";
  return 2;
}

BOOL GetClearType()
{
  DWORD dwParam;

  if (SystemParametersInfoA(SPI_GETFONTSMOOTHING, 0, &dwParam, 0) && dwParam == TRUE)
    if (SystemParametersInfoA(SPI_GETFONTSMOOTHINGTYPE, 0, &dwParam, 0) && dwParam == FE_FONTSMOOTHINGCLEARTYPE)
      return TRUE;
  return FALSE;
}

void UpdateEdit(HWND hWnd, DWORD dwFlags)
{
  RECT rc;
  BOOL bErase=FALSE;

  if (dwFlags & UE_ERASE)
    bErase=TRUE;

  if (dwFlags & UE_ALLRECT)
  {
    InvalidateRect(hWnd, NULL, bErase);
  }
  else if (dwFlags & UE_DRAWRECT)
  {
    SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)&rc);
    InvalidateRect(hWnd, &rc, bErase);
  }
  else if (dwFlags & UE_FIRSTPIXEL)
  {
    rc.left=0;
    rc.top=0;
    rc.right=1;
    rc.bottom=1;
    InvalidateRect(hWnd, &rc, bErase);
  }
  if (dwFlags & UE_UPDATEWINDOW)
    UpdateWindow(hWnd);
}

void UpdateEditAll(DWORD dwFlags)
{
  EDITINFO ei;
  BOOL bMaximized=TRUE;

  if (nMDI == WMD_MDI)
    SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);

  if (bMaximized)
  {
    if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
    {
      if (ei.hWndMaster)
      {
        UpdateEdit(ei.hWndMaster, dwFlags);
        if (ei.hWndClone1)
          UpdateEdit(ei.hWndClone1, dwFlags);
        if (ei.hWndClone2)
          UpdateEdit(ei.hWndClone2, dwFlags);
        if (ei.hWndClone3)
          UpdateEdit(ei.hWndClone3, dwFlags);
      }
      else UpdateEdit(ei.hWndEdit, dwFlags);
    }
  }
  else
  {
    //nMDI == WMD_MDI in non-maximized state
    FRAMEDATA *lpFrameCount=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0);
    FRAMEDATA *lpFrameInit=lpFrameCount;

    while (lpFrameCount)
    {
      if (lpFrameCount->ei.hWndMaster)
      {
        UpdateEdit(lpFrameCount->ei.hWndMaster, dwFlags);
        if (lpFrameCount->ei.hWndClone1)
          UpdateEdit(lpFrameCount->ei.hWndClone1, dwFlags);
        if (lpFrameCount->ei.hWndClone2)
          UpdateEdit(lpFrameCount->ei.hWndClone2, dwFlags);
        if (lpFrameCount->ei.hWndClone3)
          UpdateEdit(lpFrameCount->ei.hWndClone3, dwFlags);
      }
      else UpdateEdit(lpFrameCount->ei.hWndEdit, dwFlags);

      if (lpFrameInit == (lpFrameCount=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_NEXT, (LPARAM)lpFrameCount)))
        break;
    }
  }
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
  int nSize;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    //OF_LISTTEXT
    if ((nSize=(int)WideOption(hOptions, L"SpecialCharText", PO_BINARY, NULL, 0)) > 0)
    {
      if (wszSpecialCharText=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize))
      {
        WideOption(hOptions, L"SpecialCharText", PO_BINARY, (LPBYTE)wszSpecialCharText, nSize);
      }
    }

    //OF_SETTINGS
    WideOption(hOptions, L"IndentLineSolid", PO_DWORD, (LPBYTE)&bIndentLineSolid, sizeof(DWORD));
    WideOption(hOptions, L"IndentLineSize", PO_DWORD, (LPBYTE)&nIndentLineSize, sizeof(DWORD));
    WideOption(hOptions, L"CoderTheme", PO_DWORD, (LPBYTE)&bCoderTheme, sizeof(DWORD));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }

  if (!wszSpecialCharText)
  {
    nSize=(int)xprintfW(NULL, L"%s", GetLangStringW(wLangModule, STRID_DEFAULTSPECIALCHARS));

    if (wszSpecialCharText=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize * sizeof(wchar_t)))
    {
      xprintfW(wszSpecialCharText, L"%s", GetLangStringW(wLangModule, STRID_DEFAULTSPECIALCHARS));
    }
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_LISTTEXT)
    {
      SPECIALCHAR *pscChar;
      wchar_t wszOldChar[32];
      wchar_t wszNewChar[32];
      const wchar_t *wpFormat=L"\"%s\" %s %s %d %d %d %d %d %d %d\r";
      DWORD dwSize;

      for (dwSize=0, pscChar=(SPECIALCHAR *)hSpecialCharStack.first; pscChar; pscChar=pscChar->next)
      {
        ValueToHexChar(pscChar->nOldChar, wszOldChar);
        ValueToHexChar(pscChar->nNewChar, wszNewChar);
        dwSize+=(DWORD)xprintfW(NULL, wpFormat, pscChar->wszName, wszOldChar, wszNewChar, pscChar->dwFlags, pscChar->dwBasicFontStyle, pscChar->dwBasicTextColor, pscChar->dwBasicBkColor, pscChar->dwSelFontStyle, pscChar->dwSelTextColor, pscChar->dwSelBkColor) - 1;
      }
      if (wszSpecialCharText=(wchar_t *)GlobalAlloc(GPTR, (dwSize + 1) * sizeof(wchar_t)))
      {
        for (dwSize=0, pscChar=(SPECIALCHAR *)hSpecialCharStack.first; pscChar; pscChar=pscChar->next)
        {
          ValueToHexChar(pscChar->nOldChar, wszOldChar);
          ValueToHexChar(pscChar->nNewChar, wszNewChar);
          dwSize+=(DWORD)xprintfW(wszSpecialCharText + dwSize, wpFormat, pscChar->wszName, wszOldChar, wszNewChar, pscChar->dwFlags, pscChar->dwBasicFontStyle, pscChar->dwBasicTextColor, pscChar->dwBasicBkColor, pscChar->dwSelFontStyle, pscChar->dwSelTextColor, pscChar->dwSelBkColor);
        }
      }
      WideOption(hOptions, L"SpecialCharText", PO_BINARY, (LPBYTE)wszSpecialCharText, ((int)xstrlenW(wszSpecialCharText) + 1) * sizeof(wchar_t));
      GlobalFree((HGLOBAL)wszSpecialCharText);
      wszSpecialCharText=NULL;
    }
    if (dwFlags & OF_SETTINGS)
    {
      WideOption(hOptions, L"IndentLineSolid", PO_DWORD, (LPBYTE)&bIndentLineSolid, sizeof(DWORD));
      WideOption(hOptions, L"IndentLineSize", PO_DWORD, (LPBYTE)&nIndentLineSize, sizeof(DWORD));
      WideOption(hOptions, L"CoderTheme", PO_DWORD, (LPBYTE)&bCoderTheme, sizeof(DWORD));
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
    if (nStringID == STRID_DEFAULTSPECIALCHARS)
      return L"\
\"\x041B\x0438\x043D\x0438\x044F\x0020\x043E\x0442\x0441\x0442\x0443\x043F\x0430\" -4 \\x00 257 0 0 0 0 0 0\r\
\"\x041F\x0440\x043E\x0431\x0435\x043B\" \\x20 \\xB7 257 0 0 0 0 0 0\r\
\"\x0422\x0430\x0431\x0443\x043B\x044F\x0446\x0438\x044F\" \\x09 \\x00 257 0 0 0 0 0 0\r\
\"\x041D\x043E\x0432\x0430\x044F\x0020\x0441\x0442\x0440\x043E\x043A\x0430\" -2 \\x00 257 0 0 0 0 0 0\r\
\"\x041A\x043E\x043D\x0435\x0446\x0020\x0444\x0430\x0439\x043B\x0430\" -5 \\x00 257 0 0 0 0 0 0\r\
\"\x041F\x0435\x0440\x0435\x043D\x043E\x0441\" -3 \\x00 257 0 0 0 0 0 0\r\
\"\x0412\x0435\x0440\x0442\x0438\x043A\x0430\x043B\x044C\x043D\x0430\x044F\x0020\x0442\x0430\x0431\x0443\x043B\x044F\x0446\x0438\x044F\" \\x0b \\xA6 257 0 0 0 0 0 0\r\
\"\x041F\x0440\x043E\x0433\x043E\x043D\x0020\x043B\x0438\x0441\x0442\x0430\" \\x0c \\xA7 257 0 0 0 0 0 0\r\
\"\x041D\x0443\x043B\x0435\x0432\x043E\x0439\x0020\x0441\x0438\x043C\x0432\x043E\x043B\" \\x00 \\xA4 257 0 0 0 0 0 0\r";

    if (nStringID == STRID_SPECIALCHAR_EXISTS)
      return L"\x0421\x043F\x0435\x0446\x0438\x0430\x043B\x044C\x043D\x044B\x0439\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x0020\x0443\x0436\x0435\x0020\x0441\x0443\x0449\x0435\x0441\x0442\x0432\x0443\x0435\x0442: \"%s\"";
    if (nStringID == STRID_ADD)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_EDIT)
      return L"\x0418\x0437\x043C\x0435\x043D\x0438\x0442\x044C";
    if (nStringID == STRID_DELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_NAME)
      return L"\x041D\x0430\x0438\x043C\x0435\x043D\x043E\x0432\x0430\x043D\x0438\x0435:";
    if (nStringID == STRID_OLDCHAR)
      return L"\x0421\x0442\x0430\x0440\x044B\x0439\x0020\x0441\x0438\x043C\x0432\x043E\x043B";
    if (nStringID == STRID_HEX)
      return L"\x0428\x0435\x0441\x0442\x043D\x0430\x0434\x0446\x0430\x0442\x0435\x0440\x0438\x0447\x043D\x044B\x0439";
    if (nStringID == STRID_SPECIAL)
      return L"\x0421\x043F\x0435\x0446\x0438\x0430\x043B\x044C\x043D\x044B\x0439";
    if (nStringID == STRID_SOLIDINDENTLINE)
      return L"\x0421\x043F\x043B\x043E\x0448\x043D\x0430\x044F\x0020\x043B\x0438\x043D\x0438\x044F\x0020\x043E\x0442\x0441\x0442\x0443\x043F\x0430";
    if (nStringID == STRID_INDENTLINESIZE)
      return L"\x0420\x0430\x0437\x043C\x0435\x0440:";
    if (nStringID == STRID_NEWCHAR)
      return L"\x041D\x043E\x0432\x044B\x0439\x0020\x0441\x0438\x043C\x0432\x043E\x043B";
    if (nStringID == STRID_DEFAULT)
      return L"\x041F\x043E\x0020\x0443\x043C\x043E\x043B\x0447\x0430\x043D\x0438\x044E";
    if (nStringID == STRID_BASICTEXT)
      return L"\x0426\x0432\x0435\x0442\x0020\x0442\x0435\x043A\x0441\x0442\x0430\x0020\x043E\x0441\x043D\x043E\x0432\x043D\x043E\x0439";
    if (nStringID == STRID_SELTEXT)
      return L"\x0426\x0432\x0435\x0442\x0020\x0442\x0435\x043A\x0441\x0442\x0430\x0020\x0432\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x0438\x0438";
    if (nStringID == STRID_FONTSTYLE)
      return L"\x0421\x0442\x0438\x043B\x044C\x0020\x0448\x0440\x0438\x0444\x0442\x0430";
    if (nStringID == STRID_CODERTHEME)
      return L"\x0418\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x044C\x0020\x0442\x0435\x043C\x044B Coder";
    if (nStringID == STRID_NEWLINE)
      return L"\x041D\x043E\x0432\x0430\x044F\x0020\x0441\x0442\x0440\x043E\x043A\x0430";
    if (nStringID == STRID_WRAP)
      return L"\x041F\x0435\x0440\x0435\x043D\x043E\x0441";
    if (nStringID == STRID_INDENTLINE)
      return L"\x041B\x0438\x043D\x0438\x044F\x0020\x043E\x0442\x0441\x0442\x0443\x043F\x0430";
    if (nStringID == STRID_EOF)
      return L"\x041A\x043E\x043D\x0435\x0446\x0020\x0444\x0430\x0439\x043B\x0430";
    if (nStringID == STRID_IGNORE)
      return L"\x0418\x0433\x043D\x043E\x0440\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_NORMAL)
      return L"\x041E\x0431\x044B\x0447\x043D\x044B\x0439";
    if (nStringID == STRID_BOLD)
      return L"\x0416\x0438\x0440\x043D\x044B\x0439";
    if (nStringID == STRID_ITALIC)
      return L"\x041A\x0443\x0440\x0441\x0438\x0432";
    if (nStringID == STRID_BOLDITALIC)
      return L"\x0416\x0438\x0440\x043D\x044B\x0439\x0020\x043A\x0443\x0440\x0441\x0438\x0432";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_DEFAULTSPECIALCHARS)
      return L"\
\"Indent line\" -4 \\x00 257 0 0 0 0 0 0\r\
\"Space\" \\x20 \\xB7 257 0 0 0 0 0 0\r\
\"Tabulation\" \\x09 \\x00 257 0 0 0 0 0 0\r\
\"New line\" -2 \\x00 257 0 0 0 0 0 0\r\
\"End of file\" -5 \\x00 257 0 0 0 0 0 0\r\
\"Wrap\" -3 \\x00 257 0 0 0 0 0 0\r\
\"Vertical tabulation\" \\x0b \\xA6 257 0 0 0 0 0 0\r\
\"Form-feed\" \\x0c \\xA7 257 0 0 0 0 0 0\r\
\"Null\" \\x00 \\xA4 257 0 0 0 0 0 0\r";

    if (nStringID == STRID_SPECIALCHAR_EXISTS)
      return L"Special char already exist: \"%s\"";
    if (nStringID == STRID_ADD)
      return L"Add";
    if (nStringID == STRID_EDIT)
      return L"Edit";
    if (nStringID == STRID_DELETE)
      return L"Delete";
    if (nStringID == STRID_NAME)
      return L"Name:";
    if (nStringID == STRID_OLDCHAR)
      return L"Old symbol";
    if (nStringID == STRID_HEX)
      return L"Hexadecimal";
    if (nStringID == STRID_SPECIAL)
      return L"Special";
    if (nStringID == STRID_SOLIDINDENTLINE)
      return L"Solid indent line";
    if (nStringID == STRID_INDENTLINESIZE)
      return L"Size:";
    if (nStringID == STRID_NEWCHAR)
      return L"New char";
    if (nStringID == STRID_DEFAULT)
      return L"Default";
    if (nStringID == STRID_BASICTEXT)
      return L"Basic text color";
    if (nStringID == STRID_SELTEXT)
      return L"Text color in selection";
    if (nStringID == STRID_FONTSTYLE)
      return L"Font style";
    if (nStringID == STRID_CODERTHEME)
      return L"Use Coder themes";
    if (nStringID == STRID_NEWLINE)
      return L"New line";
    if (nStringID == STRID_WRAP)
      return L"Wrap";
    if (nStringID == STRID_INDENTLINE)
      return L"Indent line";
    if (nStringID == STRID_EOF)
      return L"End of file";
    if (nStringID == STRID_IGNORE)
      return L"Ignore";
    if (nStringID == STRID_NORMAL)
      return L"Normal";
    if (nStringID == STRID_BOLD)
      return L"Bold";
    if (nStringID == STRID_ITALIC)
      return L"Italic";
    if (nStringID == STRID_BOLDITALIC)
      return L"Bold italic";
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
  hMainWnd=pd->hMainWnd;
  hMdiClient=pd->hMdiClient;
  bOldWindows=pd->bOldWindows;
  bOldRichEdit=pd->bOldRichEdit;
  bAkelEdit=pd->bAkelEdit;
  nMDI=pd->nMDI;
  wLangModule=PRIMARYLANGID(pd->wLangModule);

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

  dwPaintOptions=(DWORD)SendMessage(hMainWnd, AKD_GETMAININFO, MI_PAINTOPTIONS, 0);
  ReadOptions(0);
  CreateSpecialCharStack(&hSpecialCharStack, wszSpecialCharText);
  GlobalFree((HGLOBAL)wszSpecialCharText);
  wszSpecialCharText=NULL;
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

  NewEditProcData=NULL;
  SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NewEditProc, (LPARAM)&NewEditProcData);
}

void UninitMain()
{
  bInitMain=FALSE;

  //Save options
  if (dwSaveFlags)
  {
    SaveOptions(dwSaveFlags);
    dwSaveFlags=0;
  }
  FreeSpecialCharStack(&hSpecialCharStack);
  pscIndentLine=NULL;

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
  if (NewEditProcData)
  {
    SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&NewEditProcData);
    NewEditProcData=NULL;
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

#include <assert.h>
#define ENSURE_NOT_ZERO(x) do { if ((x)<=0) { (x) = 1; } } while(FALSE)

/////////////////////////////////////////////////////////////////////////////
void DrawVisualEndOfLine(HDC hDC, const RECTL *rc, COLORREF color, int lbType)
{
    const LONG x=rc->left, y=rc->top, cx=rc->right-rc->left, cy=rc->bottom-rc->top;

    POINT pt[17] = { 0 };
    int nPtCount = _countof(pt);
    if (lbType == AELB_RN)
    {
        // Windows style line break. "\r\n" ("carriage return" and "line feed")
        // 
        //              P5+----+P4             ----+  y1
        //       P8       |    |                   |  y2
        //       /|P7   P6|    |                   |    
        // P9  /  +-------+    |                   |  y3
        // P0<              0  | (xCenter,yCenter) |  y4
        //     \  +------------+P3                 |  y5
        //       \|P2       +--+                   |    
        //   |   P1     nDelta |               ----+  y6
        //   |                 |                        
        //   +-----------------+                        
        //   x1   x2      x3   x4                       

        double xDelta = (double) cx * 1.0 / 34.0;
        double yDelta = (double) cy * 1.0 / 34.0;

        LONG x1, x2, x3, x4;
        LONG y1, y2, y3, y4, y5, y6;

        x1 = x + (LONG) (xDelta *  6.0 + 0.5); ENSURE_NOT_ZERO(x1);
        x2 = x + (LONG) (xDelta * 14.0 + 0.5); ENSURE_NOT_ZERO(x2);
        x3 = x + (LONG) (xDelta * 18.0 + 0.5); ENSURE_NOT_ZERO(x3);
        x4 = x + (LONG) (xDelta * 26.0 + 0.5); ENSURE_NOT_ZERO(x4);

        y1 = y + (LONG) (yDelta *  8.0 + 0.5); ENSURE_NOT_ZERO(y1);
        y2 = y + (LONG) (yDelta * 10.0 + 0.5); ENSURE_NOT_ZERO(y2);
        y3 = y + (LONG) (yDelta * 14.0 + 0.5); ENSURE_NOT_ZERO(y3);
        y4 = y + (LONG) (yDelta * 18.0 + 0.5); ENSURE_NOT_ZERO(y4);
        y5 = y + (LONG) (yDelta * 22.0 + 0.5); ENSURE_NOT_ZERO(y5);
        y6 = y + (LONG) (yDelta * 26.0 + 0.5); ENSURE_NOT_ZERO(y6);

        pt[0].x = pt[9].x = x1;
        pt[0].y = pt[9].y = y4;
        pt[1].x = pt[2].x = pt[7].x = pt[8].x = x2;
        pt[1].y = y6;
        pt[2].y = pt[3].y = y5;
        pt[3].x = pt[4].x = x4;
        pt[4].y = pt[5].y = y1;
        pt[5].x = pt[6].x = x3;
        pt[6].y = pt[7].y = y3;
        pt[8].y = y2;

        nPtCount = 10;
    } else if (lbType == AELB_N) {
        // unix style line break. "\n" (line feed). 
        //                    |
        //                   |=|----------------|---
        //         (x2, y1)p2|||p3((x4, y1)     |
        //                   |||                |  
        //                   |||                |2dx
        //                   |||                |  
        //---------(x2, y2)p1|+|p4(x4, y2)------+-------------- 
        //     (x1, y2)p0|\==|||==/|p5(x5, y2)--|---
        //     (x1, y2)p7| \  |  / |            |
        //               |  \ | /  |            |dx
        //               |    v----+------------|---
        //          (x3, y3)p6|    |
        //               |    |    |
        //             --|---2dx---|--

        LONG dx = (min(cx, cy) + 3) / 4;
        LONG xc = x + 2 * dx;

        LONG x1 = xc - dx;
        LONG x2 = xc - dx / 2;
        LONG x3 = xc;
        LONG x4 = xc + dx / 2;
        LONG x5 = xc + dx;

        LONG y1 = y + 1 * dx;
        LONG y2 = y + 3 * dx;
        LONG y3 = y + 4 * dx;


        pt[0].x = pt[7].x = x1;
        pt[0].y = pt[7].y = pt[1].y = pt[4].y = pt[5].y = y2;
        pt[1].x = pt[2].x = x2;
        pt[2].y = pt[3].y = y1;
        pt[3].x = pt[4].x = x4;
        pt[5].x = x5;
        pt[6].x = x3;
        pt[6].y = y3;

        nPtCount = 8;
    } else if (lbType == AELB_R) {
        // MAC OS type line break. "\r" (carriage return).
        //                           |Y
        //                           |
        //       (x2, y1) p1|--------|----------------------|---
        //                 /|(x2, y2)|            (x3, y2)  | 
        // p0(x1, y3)    /  +p2======|===========|p3        |1dy
        // -----------|<-------------+-----------|----------|------X
        // p7(x1, y3) |  \  +p5======|=|=========|p4        |1dy
        //            |    \|(x2, y4)| |         |(x3, y4)  |
        //       (x2, y5) p6|--------+-+---------+----------|--- 
        //            |     |        | |         |
        //------------|-1dy-|---1dy--|-|---1dy---|----
        //                           |
        //                           | 
        //

        LONG dx = (min(cx, cy) + 3) / 4;
        LONG yCenter = y + cy / 2;

        LONG x1 = x + dx;
        LONG x2 = x + 2 * dx;
        LONG x3 = x + 4 * dx;

        LONG y1 = yCenter - dx;
        LONG y2 = yCenter - dx / 2;
        LONG y3 = yCenter;
        LONG y4 = yCenter + dx / 2;
        LONG y5 = yCenter + dx;

        pt[0].x = pt[7].x = x1;
        pt[0].y = pt[7].y = y3;
        pt[1].x = pt[2].x = pt[5].x = pt[6].x = x2;
        pt[1].y = y1;
        pt[2].y = pt[3].y = y2;
        pt[3].x = pt[4].x = x3;
        pt[5].y = pt[4].y = y4;
        pt[6].y = y5;

        nPtCount = 8;
    } else if (lbType == AELB_EOF) {
        //    \/\/      |
        //    /  \      |
        //    \  /      |
        //    /\/\      |

        LONG xCenter = x + cx / 2;
        LONG yCenter = y + cy / 2;
        LONG dx = cx * 5 / 8 / 4;
        LONG dy = cy * 5 / 8 / 4;

        LONG x0 = xCenter - 2 * dx;
        LONG x1 = xCenter - 1 * dx;
        LONG x2 = xCenter;
        LONG x3 = xCenter + 1 * dx;
        LONG x4 = xCenter + 2 * dx;

        LONG y0 = yCenter - 2 * dy;
        LONG y1 = yCenter - 1 * dy;
        LONG y2 = yCenter;
        LONG y3 = yCenter + 1 * dy;
        LONG y4 = yCenter + 2 * dy;

        pt[0].x = pt[16].x = pt[2].x = pt[4].x = x0;
        pt[1].x = pt[15].x = pt[3].x = pt[5].x = x1;
        pt[14].x = pt[6].x = x2;
        pt[13].x = pt[11].x = pt[7].x = pt[9].x = x3;
        pt[12].x = pt[10].x = pt[8].x = x4;

        pt[0].y = pt[16].y = pt[14].y = pt[12].y = y0;
        pt[1].y = pt[15].y = pt[13].y = pt[11].y = y1;
        pt[2].y = pt[10].y = y2;
        pt[3].y = pt[5].y = pt[7].y = pt[9].y = y3;
        pt[4].y = pt[6].y = pt[8].y = y4;

        nPtCount = 17;
    } else {
        assert(FALSE);
    }

    {
        HGDIOBJ oldPen = SelectObject(hDC,GetStockObject(DC_PEN));
        COLORREF oldColor = SetDCPenColor(hDC, color);
        Polyline(hDC, pt, nPtCount);
        SetDCPenColor(hDC, oldColor);
        SelectObject(hDC, oldPen);
    }
}
