#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <wincrypt.h>
#include <richedit.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"


/*
//Include stack functions
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackDelete
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xstrcpynW
#define xstrlenA
#define xstrlenW
#define xstrcmpW
#define xstrcmpiW
#define xstrcmpnW
#define xstrcmpinW
#define xstrstrW
#define xstrrepW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define hex2decW
#define bin2hexW
#define hex2binW
#define xprintfW
#include "StrFunc.h"

//Include wide functions
#define DialogBoxWide
#define SetDlgItemTextWide
#define SetWindowTextWide
#include "WideFunc.h"

//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"
//*/

//Defines
#define STRID_WORDWRAPOFF    1
#define STRID_ENCRYPTION     2
#define STRID_DECRYPTION     3
#define STRID_PASS1          4
#define STRID_PASS2          5
#define STRID_PASSDONTMATCH  6
#define STRID_NOSELECTION    7
#define STRID_PLUGIN         8
#define STRID_OK             9
#define STRID_CANCEL         10

#define DLLA_FORMAT_MATCHCASE     1
#define DLLA_FORMAT_MATCHLOCALE   2
#define DLLA_FORMAT_CASEANDLOCALE 3

//Format flags
#define FMTF_MATCHCASE       0x1
#define FMTF_LOCALE          0x2

//LinesOrder action
#define LO_SORTSTRASC        1
#define LO_SORTSTRDESC       2
#define LO_SORTINTASC        3
#define LO_SORTINTDESC       4
#define LO_GETUNIQUE         5
#define LO_GETDUPLICATES     6
#define LO_REMOVEDUPLICATES  7
#define LO_REVERSE           8
#define LO_FIXWRAP           9

//Duplicate item action
#define DIA_NONE             0
#define DIA_EXCLUDE          1
#define DIA_INCLUDE          2

typedef struct _LINEITEMARRAY {
  wchar_t *wpLine;
  int nLineLen;
  wchar_t *wpLineStr;
  int nLineStrLen;
  BOOL nDuplAction;
  INT_PTR nIndex;
} LINEITEMARRAY;

typedef struct _LINEITEMSTACK {
  struct _LINEITEMSTACK *next;
  struct _LINEITEMSTACK *prev;
  wchar_t *wpLine;
  int nLineLen;
} LINEITEMSTACK;

//Encrypt algorithm
#define ENCRYPT_ALGORITHM  CALG_RC4

//Functions prototypes
void LinesOrder(HWND hWnd, HSTACK *hStack, int nOrder);
wchar_t* StackToTextBuffer(HSTACK *hStack);
LINEITEMSTACK* StackInsertLine(HSTACK *hStack, wchar_t *wpLine, int nLineLen, int nIndex);
void StackLinesFree(HSTACK *hStack);
void StackFree(HSTACK *hStack);
int PluginStrCmp(const wchar_t *wpString1, const wchar_t *wpString2);
int CompareLineStr(const void *elem1, const void *elem2);
int CompareLineInt(const void *elem1, const void *elem2);
int CompareLineDupl(const void *elem1, const void *elem2);
void swap_item(register unsigned char *src, register unsigned char *dest, register unsigned int size);
void quicksort(void *base, unsigned int count, unsigned int width, int (*compare)(const void *, const void *));

BOOL CALLBACK CryptDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL SetupCryptoClient();
BOOL EncryptString(unsigned char *lpData, DWORD *dwDataLen, unsigned char *lpKey, DWORD dwKeyLen);
BOOL DecryptString(unsigned char *lpData, DWORD *dwDataLen, unsigned char *lpKey, DWORD dwKeyLen);

BOOL IsReadOnly();
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);

//Global variables
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceDLL;
HWND hMainWnd;
HWND hWndEdit;
HICON hMainIcon;
BOOL bOldWindows;
BOOL bAkelEdit;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
HSTACK hLinesStack={0};
DWORD dwGlobalFlags;
BOOL bGlobalEncrypt;
int nGlobalOrder;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Format";
}

//Plugin extern function
void __declspec(dllexport) LineSortStrAsc(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction >= DLLA_FORMAT_MATCHCASE && nAction <= DLLA_FORMAT_CASEANDLOCALE)
    {
      dwGlobalFlags=(DWORD)nAction;
    }
  }
  LinesOrder(hWndEdit, &hLinesStack, LO_SORTSTRASC);
}

void __declspec(dllexport) LineSortStrDesc(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction >= DLLA_FORMAT_MATCHCASE && nAction <= DLLA_FORMAT_CASEANDLOCALE)
    {
      dwGlobalFlags=(DWORD)nAction;
    }
  }
  LinesOrder(hWndEdit, &hLinesStack, LO_SORTSTRDESC);
}

void __declspec(dllexport) LineSortIntAsc(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction >= DLLA_FORMAT_MATCHCASE && nAction <= DLLA_FORMAT_CASEANDLOCALE)
    {
      dwGlobalFlags=(DWORD)nAction;
    }
  }
  LinesOrder(hWndEdit, &hLinesStack, LO_SORTINTASC);
}

void __declspec(dllexport) LineSortIntDesc(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction >= DLLA_FORMAT_MATCHCASE && nAction <= DLLA_FORMAT_CASEANDLOCALE)
    {
      dwGlobalFlags=(DWORD)nAction;
    }
  }
  LinesOrder(hWndEdit, &hLinesStack, LO_SORTINTDESC);
}

void __declspec(dllexport) LineGetUnique(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_FORMAT_MATCHCASE)
    {
      dwGlobalFlags=FMTF_MATCHCASE;
    }
  }
  LinesOrder(hWndEdit, &hLinesStack, LO_GETUNIQUE);
}

void __declspec(dllexport) LineGetDuplicates(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_FORMAT_MATCHCASE)
    {
      dwGlobalFlags=FMTF_MATCHCASE;
    }
  }
  LinesOrder(hWndEdit, &hLinesStack, LO_GETDUPLICATES);
}

void __declspec(dllexport) LineRemoveDuplicates(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_FORMAT_MATCHCASE)
    {
      dwGlobalFlags=FMTF_MATCHCASE;
    }
  }
  LinesOrder(hWndEdit, &hLinesStack, LO_REMOVEDUPLICATES);
}

void __declspec(dllexport) LineReverse(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  LinesOrder(hWndEdit, &hLinesStack, LO_REVERSE);
}

void __declspec(dllexport) LineFixWrap(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD|PDS_NORICHEDIT;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  LinesOrder(hWndEdit, &hLinesStack, LO_FIXWRAP);
}

void __declspec(dllexport) Encrypt(PLUGINDATA *pd)
{
  CHARRANGE64 cr;

  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  bGlobalEncrypt=TRUE;
  SendMessage(hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);

  if (cr.cpMin != cr.cpMax)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_ENCRYPT), hMainWnd, (DLGPROC)CryptDlgProc);
  }
  else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_NOSELECTION), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
}

void __declspec(dllexport) Decrypt(PLUGINDATA *pd)
{
  CHARRANGE64 cr;

  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  bGlobalEncrypt=FALSE;
  SendMessage(hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);

  if (cr.cpMin != cr.cpMax)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_DECRYPT), hMainWnd, (DLGPROC)CryptDlgProc);
  }
  else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_NOSELECTION), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
}

void __declspec(dllexport) LinkExtract(PLUGINDATA *pd)
{
  CHARRANGE64 cr;
  wchar_t *wszSelText;
  wchar_t *wpTextCount;
  const wchar_t *wpStrBegin;
  const wchar_t *wpStrEnd;
  wchar_t wchQuote;
  BOOL bEgual=FALSE;

  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);
  if (IsReadOnly()) return;

  SendMessage(hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
  if (cr.cpMin == cr.cpMax)
  {
    cr.cpMin=0;
    cr.cpMax=-1;
    SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
    SendMessage(hWndEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
  }

  if (wszSelText=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)hWndEdit, (LPARAM)NULL))
  {
    wpTextCount=wszSelText;
    wpStrEnd=(const wchar_t *)wszSelText;

    while (xstrstrW(wpStrEnd, -1, L" href", -1, FALSE, &wpStrBegin, &wpStrEnd))
    {
      while (*wpStrEnd == ' ' || *wpStrEnd == '\t' || *wpStrEnd == '=')
      {
        if (*wpStrEnd == '=') bEgual=TRUE;
        ++wpStrEnd;
      }
      if (!bEgual) continue;
      bEgual=FALSE;

      if (*wpStrEnd == '\"' || *wpStrEnd == '\'')
      {
        wchQuote=*wpStrEnd++;

        while (*wpStrEnd != wchQuote && *wpStrEnd != '\0')
        {
          if (*wpStrEnd == '\r')
            ++wpStrEnd;
          else
            *wpTextCount++=*wpStrEnd++;
        }
      }
      else
      {
        while (*wpStrEnd != ' ' && *wpStrEnd != '\t' && *wpStrEnd != '\r' && *wpStrEnd != '>' && *wpStrEnd != '\0')
          *wpTextCount++=*wpStrEnd++;
      }
      *wpTextCount++='\r';
    }
    *wpTextCount='\0';

    SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndEdit, (LPARAM)wszSelText);
    cr.cpMax=cr.cpMin + (wpTextCount - wszSelText);
    SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);

    SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wszSelText);
  }
}

void LinesOrder(HWND hWnd, HSTACK *hStack, int nOrder)
{
  AESELECTION aesInitial;
  AESELECTION aesFullLines;
  AECHARINDEX ciInitialCaret;
  AECHARINDEX ciCount;
  EXGETTEXTRANGE tr;
  LINEITEMARRAY *lpLinesArray=NULL;
  EDITINFO ei;
  CHARRANGE64 cr;
  wchar_t *wszResult;
  wchar_t *wpText;
  wchar_t *wpLine;
  wchar_t *wpLineStr;
  int nSelMinLine;
  int nSelMaxLine;
  int nLineLen;
  int nLineStrLen;
  int nLinesCount=0;
  INT_PTR nTextLen;
  INT_PTR nTextCount;
  int i;
  BOOL bColumnSel;

  nGlobalOrder=nOrder;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
  {
    if (bAkelEdit)
    {
      if (SendMessage(hWnd, AEM_GETSEL, (WPARAM)&ciInitialCaret, (LPARAM)&aesInitial))
      {
        bColumnSel=(aesInitial.dwFlags & AESELT_COLUMNON);
        xmemcpy(&aesFullLines, &aesInitial, sizeof(AESELECTION));

        if (nOrder == LO_FIXWRAP)
        {
          //if (aesFullLines.crSel.ciMin.lpLine->nLineBreak == AELB_WRAP)
          //  AEC_PrevLineEx(&aesFullLines.crSel.ciMin, &aesFullLines.crSel.ciMin);
          aesFullLines.crSel.ciMin.nCharInLine=0;

          if (aesFullLines.crSel.ciMax.nCharInLine != 0)
            AEC_NextLineEx(&aesFullLines.crSel.ciMax, &aesFullLines.crSel.ciMax);
        }
        else
        {
          SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEBEGIN, (LPARAM)&aesFullLines.crSel.ciMin);
          if (!AEC_IsFirstCharInLine(&aesFullLines.crSel.ciMax))
          {
            SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEEND, (LPARAM)&aesFullLines.crSel.ciMax);
            AEC_NextLineEx(&aesFullLines.crSel.ciMax, &aesFullLines.crSel.ciMax);
          }
        }
      }
      else
      {
        bColumnSel=FALSE;

        SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&aesFullLines.crSel.ciMin);
        SendMessage(hWnd, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&aesFullLines.crSel.ciMax);
      }
      aesFullLines.dwFlags=0;
      aesFullLines.dwType=0;

      if (nOrder != LO_FIXWRAP)
      {
        nSelMinLine=(int)SendMessage(hWnd, AEM_GETUNWRAPLINE, (WPARAM)aesFullLines.crSel.ciMin.nLine, 0);
        nSelMaxLine=(int)SendMessage(hWnd, AEM_GETUNWRAPLINE, (WPARAM)aesFullLines.crSel.ciMax.nLine, 0);
        if (AEC_IsFirstCharInLine(&aesFullLines.crSel.ciMax))
          --nSelMaxLine;
      }
    }
    else
    {
      //RichEdit support dropped.
      return;
    }

    if (nOrder == LO_FIXWRAP)
    {
      AECHARRANGE crRange;

      crRange.ciMin=aesFullLines.crSel.ciMin;
      crRange.ciMax=aesFullLines.crSel.ciMax;
      if (crRange.ciMax.nLine <= crRange.ciMin.nLine)
        return;

      if (!ei.bWordWrap)
      {
        MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_WORDWRAPOFF), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
        return;
      }

      if (crRange.ciMax.nCharInLine == 0)
        AEC_PrevLineEx(&crRange.ciMax, &crRange.ciMax);

      do
      {
        if (crRange.ciMin.nLine <= crRange.ciMax.nLine)
        {
          if (wpLine=(wchar_t *)GlobalAlloc(GPTR, (crRange.ciMin.lpLine->nLineLen + 1) * sizeof(wchar_t)))
          {
            xmemcpy(wpLine, crRange.ciMin.lpLine->wpLine, (crRange.ciMin.lpLine->nLineLen + 1) * sizeof(wchar_t));
            StackInsertLine(hStack, wpLine, crRange.ciMin.lpLine->nLineLen, -1);
            continue;
          }
        }
        break;
      }
      while (AEC_NextLine(&crRange.ciMin));
    }
    else
    {
      if (nSelMaxLine <= nSelMinLine)
        return;

      tr.cr.ciMin=aesFullLines.crSel.ciMin;
      tr.cr.ciMax=aesFullLines.crSel.ciMax;
      tr.bColumnSel=FALSE;
      tr.pText=NULL;
      tr.nNewLine=AELB_R;

      if (nTextLen=SendMessage(hMainWnd, AKD_EXGETTEXTRANGEW, (WPARAM)hWnd, (LPARAM)&tr))
      {
        //Create empty array for quicksort
        if (nOrder >= LO_SORTSTRASC && nOrder <= LO_REMOVEDUPLICATES)
        {
          wpText=(wchar_t *)tr.pText;
          nLinesCount=(nSelMaxLine - nSelMinLine) + 1;
          lpLinesArray=(LINEITEMARRAY *)GlobalAlloc(GPTR, nLinesCount * sizeof(LINEITEMARRAY));
        }

        if (bColumnSel) SendMessage(hWnd, AEM_GETINDEX, AEGI_FIRSTSELCHAR, (LPARAM)&ciCount);
        wpLine=(wchar_t *)tr.pText;
        nLineLen=0;
        nLinesCount=0;

        for (nTextCount=0; nTextCount <= nTextLen; ++nTextCount)
        {
          if (wpLine[nLineLen] == '\r' || nTextCount == nTextLen)
          {
            if (nTextCount == nTextLen && nLineLen == 0)
              break;
            wpLine[nLineLen]='\0';

            if (nOrder >= LO_SORTSTRASC && nOrder <= LO_REMOVEDUPLICATES)
            {
              if (bColumnSel)
              {
                wpLineStr=wpLine + min(ciCount.lpLine->nSelStart, nLineLen);

                if (nOrder >= LO_GETUNIQUE && nOrder <= LO_REMOVEDUPLICATES)
                {
                  if (!(nLineStrLen=min(ciCount.lpLine->nSelEnd, nLineLen) - min(ciCount.lpLine->nSelStart, nLineLen)))
                  {
                    wpLineStr=wpLine;
                    nLineStrLen=nLineLen;
                  }
                }
                else nLineStrLen=(int)((wpLine + nLineLen) - wpLineStr);

                SendMessage(hWnd, AEM_GETINDEX, AEGI_WRAPLINEEND, (LPARAM)&ciCount);
                SendMessage(hWnd, AEM_GETINDEX, AEGI_NEXTLINE, (LPARAM)&ciCount);
              }
              else
              {
                wpLineStr=wpLine;
                nLineStrLen=nLineLen;
              }

              if (nOrder == LO_SORTINTASC || nOrder == LO_SORTINTDESC)
              {
                while (*wpLineStr && (*wpLineStr < '0' || *wpLineStr > '9'))
                  ++wpLineStr;
                if (*wpLineStr == '0')
                {
                  ++wpLineStr;
                  while (*wpLineStr == '0')
                    ++wpLineStr;
                  if (*wpLineStr < '0' || *wpLineStr > '9')
                    --wpLineStr;
                }
                nLineStrLen=(int)((wpLine + nLineLen) - wpLineStr);
              }

              lpLinesArray[nLinesCount].wpLine=wpLine;
              lpLinesArray[nLinesCount].nLineLen=nLineLen;
              lpLinesArray[nLinesCount].wpLineStr=wpLineStr;
              lpLinesArray[nLinesCount].nLineStrLen=nLineStrLen;
              lpLinesArray[nLinesCount].nIndex=nLinesCount;

              nLinesCount++;
            }
            else if (nOrder == LO_REVERSE)
            {
              StackInsertLine(hStack, wpLine, nLineLen, 1);
            }
            wpLine+=nLineLen + 1;
            nLineLen=0;
          }
          else ++nLineLen;
        }

        //Sort and fill the stack
        if (nOrder >= LO_SORTSTRASC && nOrder <= LO_REMOVEDUPLICATES)
        {
          if (lpLinesArray)
          {
            int nUpDown;

            if (nOrder == LO_SORTSTRDESC || nOrder == LO_SORTINTDESC)
              nUpDown=1;
            else
              nUpDown=-1;

            if (nOrder >= LO_SORTSTRASC && nOrder <= LO_SORTINTDESC)
            {
              if (nOrder == LO_SORTSTRASC || nOrder == LO_SORTSTRDESC)
                quicksort(lpLinesArray, nLinesCount, sizeof(LINEITEMARRAY), CompareLineStr);
              else if (nOrder == LO_SORTINTASC || nOrder == LO_SORTINTDESC)
                quicksort(lpLinesArray, nLinesCount, sizeof(LINEITEMARRAY), CompareLineInt);

              for (i=0; i < nLinesCount; ++i)
              {
                StackInsertLine(hStack, lpLinesArray[i].wpLine, lpLinesArray[i].nLineLen, nUpDown);
              }
            }
            else if (nOrder >= LO_GETUNIQUE && nOrder <= LO_REMOVEDUPLICATES)
            {
              quicksort(lpLinesArray, nLinesCount, sizeof(LINEITEMARRAY), CompareLineDupl);

              //Restore lines order
              for (i=0;;)
              {
                swap_item((unsigned char *)&lpLinesArray[i], (unsigned char *)&lpLinesArray[lpLinesArray[i].nIndex], sizeof(LINEITEMARRAY));
                if (lpLinesArray[i].nIndex == i)
                  if (++i >= nLinesCount) break;
              }

              if (nGlobalOrder == LO_GETUNIQUE)
              {
                for (i=0; i < nLinesCount; ++i)
                {
                  if (lpLinesArray[i].nDuplAction == DIA_EXCLUDE) continue;
                  StackInsertLine(hStack, lpLinesArray[i].wpLine, lpLinesArray[i].nLineLen, nUpDown);
                }
              }
              else if (nGlobalOrder == LO_GETDUPLICATES)
              {
                for (i=0; i < nLinesCount; ++i)
                {
                  if (lpLinesArray[i].nDuplAction != DIA_INCLUDE) continue;
                  StackInsertLine(hStack, lpLinesArray[i].wpLine, lpLinesArray[i].nLineLen, nUpDown);
                }
              }
              else if (nGlobalOrder == LO_REMOVEDUPLICATES)
              {
                for (i=0; i < nLinesCount; ++i)
                {
                  if (lpLinesArray[i].nDuplAction == DIA_EXCLUDE) continue;
                  StackInsertLine(hStack, lpLinesArray[i].wpLine, lpLinesArray[i].nLineLen, nUpDown);
                }
              }
            }
            GlobalFree((HGLOBAL)lpLinesArray);
          }
        }
      }
    }
    SendMessage(hWnd, AEM_SETSEL, (WPARAM)NULL, (LPARAM)&aesFullLines);
    SendMessage(hWnd, EM_GETSEL64, (WPARAM)&cr.cpMin, 0);

    //Move all stack elements to text buffer
    wszResult=StackToTextBuffer(hStack);

    //Free stack memory
    if (nOrder == LO_FIXWRAP)
      StackLinesFree(hStack);
    else
    {
      StackFree(hStack);
      if (tr.pText) SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
    }

    //Insert result text
    if (wszResult)
    {
      SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWnd, (LPARAM)wszResult);
      GlobalFree((HGLOBAL)wszResult);
    }
    SendMessage(hWnd, EM_GETSEL64, 0, (LPARAM)&cr.cpMax);

    if (bColumnSel)
    {
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&aesInitial.crSel.ciMin);
      SendMessage(hWnd, AEM_INDEXUPDATE, 0, (LPARAM)&aesInitial.crSel.ciMax);

      if (!AEC_IndexCompare(&aesInitial.crSel.ciMin, &ciInitialCaret))
        SendMessage(hWnd, AEM_SETSEL, (WPARAM)&aesInitial.crSel.ciMin, (LPARAM)&aesInitial);
      else
        SendMessage(hWnd, AEM_SETSEL, (WPARAM)&aesInitial.crSel.ciMax, (LPARAM)&aesInitial);
    }
    else SendMessage(hWnd, EM_EXSETSEL64, 0, (LPARAM)&cr);
  }
}

wchar_t* StackToTextBuffer(HSTACK *hStack)
{
  LINEITEMSTACK *lpElement;
  wchar_t *wszData;
  wchar_t *wpCount;
  INT_PTR nSize=0;

  for (lpElement=(LINEITEMSTACK *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    nSize+=lpElement->nLineLen + 1;
  }

  if (wszData=(wchar_t *)GlobalAlloc(GPTR, (nSize + 1) * sizeof(wchar_t)))
  {
    wpCount=wszData;

    for (lpElement=(LINEITEMSTACK *)hStack->first; lpElement; lpElement=lpElement->next)
    {
      wpCount+=xstrcpynW(wpCount, lpElement->wpLine, lpElement->nLineLen + 1);
      *wpCount++=L'\r';
    }
    *wpCount=L'\0';
  }
  return wszData;
}

LINEITEMSTACK* StackInsertLine(HSTACK *hStack, wchar_t *wpLine, int nLineLen, int nIndex)
{
  LINEITEMSTACK *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, nIndex, sizeof(LINEITEMSTACK)))
  {
    lpElement->wpLine=wpLine;
    lpElement->nLineLen=nLineLen;
  }
  return lpElement;
}

void StackLinesFree(HSTACK *hStack)
{
  LINEITEMSTACK *lpElement=(LINEITEMSTACK *)hStack->first;

  while (lpElement)
  {
    GlobalFree((HGLOBAL)lpElement->wpLine);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void StackFree(HSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

int PluginStrCmp(const wchar_t *wpString1, const wchar_t *wpString2)
{
  if (dwGlobalFlags & FMTF_LOCALE)
  {
    if (bOldWindows)
    {
      char *pString1=AllocAnsi(wpString1);
      char *pString2=AllocAnsi(wpString2);
      int nResult;

      if (dwGlobalFlags & FMTF_MATCHCASE)
        nResult=lstrcmpA(pString1, pString2);
      nResult=lstrcmpiA(pString1, pString2);
      FreeAnsi(pString1);
      FreeAnsi(pString2);
      return nResult;
    }
    if (dwGlobalFlags & FMTF_MATCHCASE)
      return lstrcmpW(wpString1, wpString2);
    return lstrcmpiW(wpString1, wpString2);
  }
  else
  {
    if (dwGlobalFlags & FMTF_MATCHCASE)
      return xstrcmpW(wpString1, wpString2);
    return xstrcmpiW(wpString1, wpString2);
  }
}

int CompareLineStr(const void *elem1, const void *elem2)
{
  LINEITEMARRAY *lpItem1=(LINEITEMARRAY *)elem1;
  LINEITEMARRAY *lpItem2=(LINEITEMARRAY *)elem2;
  int nResult;

  nResult=PluginStrCmp(lpItem1->wpLineStr, lpItem2->wpLineStr);
  if (!nResult && (lpItem1->wpLine != lpItem1->wpLineStr || lpItem2->wpLine != lpItem2->wpLineStr))
    nResult=PluginStrCmp(lpItem1->wpLine, lpItem2->wpLine);
  return nResult;
}

int CompareLineInt(const void *elem1, const void *elem2)
{
  LINEITEMARRAY *lpItem1=(LINEITEMARRAY *)elem1;
  LINEITEMARRAY *lpItem2=(LINEITEMARRAY *)elem2;
  wchar_t *wpLine1=lpItem1->wpLineStr;
  wchar_t *wpLine2=lpItem2->wpLineStr;
  int nNumberWidth1=0;
  int nNumberWidth2=0;
  int nResult=0;

  for (;; ++wpLine1, ++wpLine2)
  {
    if (*wpLine1 && *wpLine1 >= '0' && *wpLine1 <= '9')
      ++nNumberWidth1;

    if (*wpLine2 && *wpLine2 >= '0' && *wpLine2 <= '9')
      ++nNumberWidth2;
    else if (nNumberWidth1 == nNumberWidth2)
      break;

    if (nNumberWidth1 == nNumberWidth2)
    {
      if (!nResult) nResult=*wpLine1 - *wpLine2;
    }
    //Count of digits in numbers are different.
    else return nNumberWidth1 - nNumberWidth2;
  }
  if (nResult) return nResult;

  //Numbers are equal. Compare all string.
  return PluginStrCmp(lpItem1->wpLine, lpItem2->wpLine);
}

int CompareLineDupl(const void *elem1, const void *elem2)
{
  LINEITEMARRAY *lpItem1=(LINEITEMARRAY *)elem1;
  LINEITEMARRAY *lpItem2=(LINEITEMARRAY *)elem2;
  int nResult;

  if (lpItem1->nLineStrLen == lpItem2->nLineStrLen)
  {
    if (dwGlobalFlags & FMTF_MATCHCASE)
      nResult=xstrcmpnW(lpItem1->wpLineStr, lpItem2->wpLineStr, lpItem1->nLineStrLen);
    else
      nResult=xstrcmpinW(lpItem1->wpLineStr, lpItem2->wpLineStr, lpItem1->nLineStrLen);

    if (!nResult)
    {
      if (nGlobalOrder == LO_GETUNIQUE)
      {
        lpItem1->nDuplAction=DIA_EXCLUDE;
        lpItem2->nDuplAction=DIA_EXCLUDE;
      }
      else if (nGlobalOrder == LO_GETDUPLICATES)
      {
        lpItem1->nDuplAction=DIA_INCLUDE;
        lpItem2->nDuplAction=DIA_INCLUDE;
      }
      else if (nGlobalOrder == LO_REMOVEDUPLICATES)
      {
        if (lpItem1->nIndex < lpItem2->nIndex)
          lpItem2->nDuplAction=DIA_EXCLUDE;
        else
          lpItem1->nDuplAction=DIA_EXCLUDE;
        return (int)(lpItem1->nIndex - lpItem2->nIndex);
      }
    }
    return nResult;
  }
  return (lpItem1->nLineStrLen - lpItem2->nLineStrLen);
}

void swap_item(register unsigned char *src, register unsigned char *dest, register unsigned int size)
{
  register int temp;

  if (src != dest)
  {
    while (size--)
    {
      temp=*src;
      *src++=*dest;
      *dest++=(unsigned char)temp;
    }
  }
}

void quicksort(void *base, unsigned int count, unsigned int width, int (*compare)(const void *, const void *))
{
  unsigned char *low;
  unsigned char *high;
  unsigned char *middle;
  unsigned char *lowguy;
  unsigned char *highguy;
  unsigned char *lowstk[30];
  unsigned char *highstk[30];
  int size;
  int stkptr;

  if (count < 2 || width == 0)
    return;

  stkptr=0;
  low=(unsigned char *)base;
  high=(unsigned char *)base + width * (count - 1);

  recurse:

  size=(int)((high - low) / width + 1);
  middle=low + (size / 2) * width;
  swap_item(middle, low, width);
  lowguy=low;
  highguy=high + width;

  for (;;)
  {
    do
    {
      lowguy+=width;
    }
    while (lowguy <= high && compare(lowguy, low) < 0);

    do
    {
      highguy-=width;
    }
    while (highguy > low && compare(highguy, low) > 0);

    if (highguy < lowguy)
      break;

    swap_item(lowguy, highguy, width);
  }

  swap_item(low, highguy, width);

  if (highguy - 1 - low >= high - lowguy)
  {
    if (low + width < highguy)
    {
      lowstk[stkptr]=low;
      highstk[stkptr]=highguy - width;
      ++stkptr;
    }
    if (lowguy < high)
    {
      low=lowguy;
      goto recurse;
    }
  }
  else
  {
    if (lowguy < high)
    {
      lowstk[stkptr]=lowguy;
      highstk[stkptr]=high;
      ++stkptr;
    }
    if (low + width < highguy)
    {
      high=highguy - width;
      goto recurse;
    }
  }

  if (--stkptr >= 0)
  {
    low=lowstk[stkptr];
    high=highstk[stkptr];
    goto recurse;
  }
  else return;
}

BOOL CALLBACK CryptDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndPass1;
  static HWND hWndPass2;
  static HWND hWndOK;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndPass1=GetDlgItem(hDlg, IDC_PASS1);
    if (bGlobalEncrypt) hWndPass2=GetDlgItem(hDlg, IDC_PASS2);
    hWndOK=GetDlgItem(hDlg, IDOK);

    SetWindowTextWide(hDlg, wszPluginTitle);
    if (bGlobalEncrypt)
      SetDlgItemTextWide(hDlg, IDC_GROUP_LABEL, GetLangStringW(wLangModule, STRID_ENCRYPTION));
    else
      SetDlgItemTextWide(hDlg, IDC_GROUP_LABEL, GetLangStringW(wLangModule, STRID_DECRYPTION));
    SetDlgItemTextWide(hDlg, IDC_PASS1_LABEL, GetLangStringW(wLangModule, STRID_PASS1));
    if (bGlobalEncrypt) SetDlgItemTextWide(hDlg, IDC_PASS2_LABEL, GetLangStringW(wLangModule, STRID_PASS2));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndPass1, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    if (bGlobalEncrypt) SendMessage(hWndPass2, EM_LIMITTEXT, (WPARAM)MAX_PATH, 0);
    EnableWindow(hWndOK, FALSE);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_PASS1)
    {
      if (GetWindowTextLengthA(hWndPass1))
        EnableWindow(hWndOK, TRUE);
      else
        EnableWindow(hWndOK, FALSE);
    }
    else if (LOWORD(wParam) == IDOK)
    {
      CHARRANGE64 cr;
      char szPass1[MAX_PATH];
      char szPass2[MAX_PATH];
      wchar_t *wszSelection;
      wchar_t *wszString;
      unsigned char *lpData;
      INT_PTR nSelectionLen=0;
      INT_PTR nDataLen=0;
      INT_PTR nStrLen=0;
      INT_PTR a;
      INT_PTR b;
      int nMaxLineLen=80;
      int nCopyLen;

      GetWindowTextA(hWndPass1, szPass1, MAX_PATH);
      if (bGlobalEncrypt) GetWindowTextA(hWndPass2, szPass2, MAX_PATH);

      if (bGlobalEncrypt)
      {
        if (lstrcmpA(szPass1, szPass2))
        {
          SetWindowTextA(hWndPass1, "");
          SetWindowTextA(hWndPass2, "");
          MessageBoxW(hDlg, GetLangStringW(wLangModule, STRID_PASSDONTMATCH), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
          return FALSE;
        }
      }

      if (wszSelection=(wchar_t *)SendMessage(hMainWnd, AKD_GETSELTEXTW, (WPARAM)hWndEdit, (LPARAM)&nSelectionLen))
      {
        if (SetupCryptoClient())
        {
          if (bGlobalEncrypt)
          {
            lpData=(unsigned char *)wszSelection;
            nDataLen=nSelectionLen * sizeof(wchar_t);

            if (EncryptString(lpData, (DWORD *)&nDataLen, (unsigned char *)szPass1, lstrlenA(szPass1)))
            {
              nStrLen=nDataLen * 2;

              if (wszString=(wchar_t *)GlobalAlloc(GPTR, (nStrLen + nStrLen / nMaxLineLen + 1) * sizeof(wchar_t)))
              {
                for (a=0, b=0; a < nStrLen; a+=nMaxLineLen, b+=nMaxLineLen + 1)
                {
                  nCopyLen=min(nMaxLineLen, (int)(nStrLen - a));
                  bin2hexW(lpData + a / 2, nCopyLen / 2, wszString + b, nCopyLen + 1, TRUE);
                  if (nCopyLen == nMaxLineLen)
                    wszString[b + nCopyLen]=L'\r';
                }
                SendMessage(hWndEdit, EM_GETSEL64, (WPARAM)&cr.cpMin, 0);
                SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndEdit, (LPARAM)wszString);
                SendMessage(hWndEdit, EM_GETSEL64, 0, (LPARAM)&cr.cpMax);
                SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
                GlobalFree((HGLOBAL)wszString);
              }
            }
          }
          else
          {
            if (lpData=(unsigned char *)GlobalAlloc(GPTR, nSelectionLen + 2))
            {
              if (xstrrepW(wszSelection, nSelectionLen + 1, L"\r", 1, L"", 0, TRUE, wszSelection, &a))
                nSelectionLen=a - 1;
              nDataLen=hex2binW(wszSelection, lpData, nSelectionLen);

              if (DecryptString(lpData, (DWORD *)&nDataLen, (unsigned char *)szPass1, lstrlenA(szPass1)))
              {
                wszString=(wchar_t *)lpData;
                wszString[nDataLen]='\0';

                SendMessage(hWndEdit, EM_GETSEL64, (WPARAM)&cr.cpMin, 0);
                SendMessage(hMainWnd, AKD_REPLACESELW, (WPARAM)hWndEdit, (LPARAM)wszString);
                SendMessage(hWndEdit, EM_GETSEL64, 0, (LPARAM)&cr.cpMax);
                SendMessage(hWndEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
              }
              GlobalFree((HGLOBAL)lpData);
            }
          }
        }
        SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wszSelection);
      }
      EndDialog(hDlg, 0);
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
  return FALSE;
}

BOOL SetupCryptoClient()
{
  // Ensure that the default cryptographic client is set up.
  HCRYPTPROV hProv=0;
  HCRYPTKEY hKey=0;

  // Attempt to acquire a handle to the default key container.
  if (!CryptAcquireContextA(&hProv, GetLangStringA(wLangModule, STRID_PLUGIN), MS_DEF_PROV, PROV_RSA_FULL, 0))
  {
    // Some sort of error occured, create default key container.
    if (!CryptAcquireContextA(&hProv, GetLangStringA(wLangModule, STRID_PLUGIN), MS_DEF_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
    {
      // Error creating key container!
      return FALSE;
    }
  }

  // Attempt to get handle to signature key.
  if (!CryptGetUserKey(hProv, AT_SIGNATURE, &hKey))
  {
    if ((int)GetLastError() == NTE_NO_KEY)
    {
      // Create signature key pair.
      if (!CryptGenKey(hProv, AT_SIGNATURE, 0, &hKey))
      {
        // Error during CryptGenKey!
        CryptReleaseContext(hProv, 0);
        return FALSE;
      }
      else
      {
        CryptDestroyKey(hKey);
      }
    }
    else
    {
      // Error during CryptGetUserKey!
      CryptReleaseContext(hProv, 0);
      return FALSE;
    }
  }

  // Attempt to get handle to exchange key.
  if (!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hKey))
  {
    if ((int)GetLastError() == NTE_NO_KEY)
    {
      // Create key exchange key pair.
      if (!CryptGenKey(hProv, AT_KEYEXCHANGE, 0, &hKey))
      {
        // Error during CryptGenKey!
        CryptReleaseContext(hProv, 0);
        return FALSE;
      }
      else
      {
        CryptDestroyKey(hKey);
      }
    }
    else
    {
      // Error during CryptGetUserKey!
      CryptReleaseContext(hProv, 0);
      return FALSE;
    }
  }

  CryptReleaseContext(hProv, 0);
  return TRUE;
}

BOOL EncryptString(unsigned char *lpData, DWORD *dwDataLen, unsigned char *lpKey, DWORD dwKeyLen)
{
  HCRYPTPROV hProv=0;
  HCRYPTKEY hKey=0;
  HCRYPTHASH hHash=0;
  BOOL bResult=FALSE;

  // Get handle to default provider.
  if (CryptAcquireContextA(&hProv, GetLangStringA(wLangModule, STRID_PLUGIN), MS_DEF_PROV, PROV_RSA_FULL, 0))
  {
    // Create hash object.
    if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
      // Hash password string.
      if (CryptHashData(hHash, lpKey, dwKeyLen, 0))
      {
        // Create block cipher session key based on hash of the password.
        if (CryptDeriveKey(hProv, ENCRYPT_ALGORITHM, hHash, CRYPT_EXPORTABLE, &hKey))
        {
          // Encrypt data
          if (CryptEncrypt(hKey, 0, TRUE, 0, lpData, dwDataLen, *dwDataLen))
          {
            bResult=TRUE;
          }
          CryptDestroyKey(hKey);
        }
      }
      CryptDestroyHash(hHash);
    }
    CryptReleaseContext(hProv, 0);
  }
  return bResult;
}

BOOL DecryptString(unsigned char *lpData, DWORD *dwDataLen, unsigned char *lpKey, DWORD dwKeyLen)
{
  HCRYPTPROV hProv=0;
  HCRYPTKEY hKey=0;
  HCRYPTHASH hHash=0;
  BOOL bResult=FALSE;

  // Get handle to default provider.
  if (CryptAcquireContextA(&hProv, GetLangStringA(wLangModule, STRID_PLUGIN), MS_DEF_PROV, PROV_RSA_FULL, 0))
  {
    // Create hash object.
    if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
      // Hash password string.
      if (CryptHashData(hHash, lpKey, dwKeyLen, 0))
      {
        // Create block cipher session key based on hash of the password.
        if (CryptDeriveKey(hProv, ENCRYPT_ALGORITHM, hHash, CRYPT_EXPORTABLE, &hKey))
        {
          if (CryptDecrypt(hKey, 0, TRUE, 0, lpData, dwDataLen))
          {
            bResult=TRUE;
          }
          CryptDestroyKey(hKey);
        }
      }
      CryptDestroyHash(hHash);
    }
    CryptReleaseContext(hProv, 0);
  }
  return bResult;
}

BOOL IsReadOnly()
{
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
  {
    if (!ei.bReadOnly)
      return FALSE;
    else
      MessageBeep(MB_OK);
  }
  return TRUE;
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
    if (nStringID == STRID_WORDWRAPOFF)
      return L"\x041F\x0435\x0440\x0435\x043D\x043E\x0441\x0020\x043F\x043E\x0020\x0441\x043B\x043E\x0432\x0430\x043C\x0020\x0434\x043E\x043B\x0436\x0435\x043D\x0020\x0431\x044B\x0442\x044C\x0020\x0432\x043A\x043B\x044E\x0447\x0435\x043D\x002E";
    if (nStringID == STRID_ENCRYPTION)
      return L"\x0428\x0438\x0444\x0440\x043E\x0432\x0430\x043D\x0438\x0435";
    if (nStringID == STRID_DECRYPTION)
      return L"\x0420\x0430\x0441\x0448\x0438\x0444\x0440\x043E\x0432\x044B\x0432\x0430\x043D\x0438\x0435";
    if (nStringID == STRID_PASS1)
      return L"\x041F\x0430\x0440\x043E\x043B\x044C\x003A";
    if (nStringID == STRID_PASS2)
      return L"\x041F\x0430\x0440\x043E\x043B\x044C\x0020\x0435\x0449\x0435\x0020\x0440\x0430\x0437\x003A";
    if (nStringID == STRID_PASSDONTMATCH)
      return L"\x041F\x0430\x0440\x043E\x043B\x0438\x0020\x043D\x0435\x0020\x0441\x043E\x0432\x043F\x0430\x0434\x0430\x044E\x0442\x002E";
    if (nStringID == STRID_NOSELECTION)
      return L"\x041E\x0442\x0441\x0443\x0442\x0441\x0442\x0432\x0443\x0435\x0442\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x0438\x0435\x0020\x0442\x0435\x043A\x0441\x0442\x0430\x002E";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_WORDWRAPOFF)
      return L"Word wrap must be turned on.";
    if (nStringID == STRID_ENCRYPTION)
      return L"Encryption";
    if (nStringID == STRID_DECRYPTION)
      return L"Decryption";
    if (nStringID == STRID_PASS1)
      return L"Password:";
    if (nStringID == STRID_PASS2)
      return L"Retype password:";
    if (nStringID == STRID_PASSDONTMATCH)
      return L"Passwords do not match.";
    if (nStringID == STRID_NOSELECTION)
      return L"No text selected.";
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
  hWndEdit=pd->hWndEdit;
  hMainIcon=pd->hMainIcon;
  bOldWindows=pd->bOldWindows;
  bAkelEdit=pd->bAkelEdit;
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

  dwGlobalFlags=0;
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
