#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "StrFunc.h"
#include "AkelEdit.h"
#include "AkelDLL.h"


/*
//Include string functions
#define xstrlenW
#define xstrcpynW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"
//*/

//Defines
#define STRID_INFO   1
#define STRID_PLUGIN 2

//Functions prototypes
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
void InitCommon(PLUGINDATA *pd);

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HWND hMainWnd;
HWND hWndEdit;
BOOL bOldWindows;
BOOL bAkelEdit;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
DWORD dwStatusPosType=0;


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Stats";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  //Function doesn't support autoload
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  InitCommon(pd);

  //Calculate
  {
    AECHARRANGE crInit;
    AECHARRANGE crCount;
    AECHARINDEX ciWordStart;
    AECHARINDEX ciWordEnd;
    AECHARINDEX ciCount;
    AEINDEXSUBTRACT is;
    INT_PTR nCharCount=0;
    INT_PTR nCharLatinSpaces=0;
    INT_PTR nCharLatinLetters=0;
    INT_PTR nCharLatinDigits=0;
    INT_PTR nCharLatinOther=0;
    INT_PTR nCharNonLatin=0;
    INT_PTR nCharSurrogate=0;
    int nWordCount=0;
    int nLineCount=0;
    BOOL bFirst=TRUE;

    if (hWndEdit)
    {
      SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTSELCHAR, (LPARAM)&crInit.ciMin);
      SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTSELCHAR, (LPARAM)&crInit.ciMax);

      if (!AEC_IndexCompare(&crInit.ciMin, &crInit.ciMax))
      {
        SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crInit.ciMin);
        SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crInit.ciMax);

        //Line count
        nLineCount=(crInit.ciMax.nLine - crInit.ciMin.nLine) + 1;
        if (!(dwStatusPosType & SPT_LINEWRAP))
          nLineCount=(int)SendMessage(hWndEdit, AEM_GETUNWRAPLINE, nLineCount - 1, 0) + 1;

        //Char count
        is.ciChar1=&crInit.ciMax;
        is.ciChar2=&crInit.ciMin;
        is.bColumnSel=FALSE;
        is.nNewLine=AELB_ASOUTPUT;
        nCharCount=SendMessage(hWndEdit, AEM_INDEXSUBTRACT, 0, (LPARAM)&is);

        //Word count
        ciCount=crInit.ciMin;

        for (;;)
        {
          if (!SendMessage(hWndEdit, AEM_GETNEXTBREAK, AEWB_RIGHTWORDEND, (LPARAM)&ciCount))
            break;

          ++nWordCount;
        }

        //Symbols detalization
        for (ciCount=crInit.ciMin; AEC_IndexCompare(&ciCount, &crInit.ciMax) < 0; AEC_NextChar(&ciCount))
        {
          if (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
          {
            if (AEC_IndexLen(&ciCount) == 1)
            {
              if (ciCount.lpLine->wpLine[ciCount.nCharInLine] <= 0x80)
              {
                if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L' ' ||
                    ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\t')
                {
                  ++nCharLatinSpaces;
                }
                else if ((ciCount.lpLine->wpLine[ciCount.nCharInLine] >= L'A' &&
                          ciCount.lpLine->wpLine[ciCount.nCharInLine] <= L'Z') ||
                         (ciCount.lpLine->wpLine[ciCount.nCharInLine] >= L'a' &&
                          ciCount.lpLine->wpLine[ciCount.nCharInLine] <= L'z'))
                {
                  ++nCharLatinLetters;
                }
                else if (ciCount.lpLine->wpLine[ciCount.nCharInLine] >= L'0' &&
                         ciCount.lpLine->wpLine[ciCount.nCharInLine] <= L'9')
                {
                  ++nCharLatinDigits;
                }
                else ++nCharLatinOther;
              }
              else ++nCharNonLatin;
            }
            else ++nCharSurrogate;
          }
          else
          {
            if (ciCount.lpLine->nLineBreak == AELB_R || ciCount.lpLine->nLineBreak == AELB_N)
              ++nCharLatinOther;
            else if (ciCount.lpLine->nLineBreak == AELB_RN)
              nCharLatinOther+=2;
            else if (ciCount.lpLine->nLineBreak == AELB_RRN)
              nCharLatinOther+=3;
          }
        }
      }
      else
      {
        //Line count
        if (!(dwStatusPosType & SPT_LINEWRAP))
          nLineCount=(int)SendMessage(hWndEdit, AEM_GETUNWRAPLINE, crInit.ciMax.nLine, 0) - (int)SendMessage(hWndEdit, AEM_GETUNWRAPLINE, crInit.ciMin.nLine, 0) + 1;
        else
          nLineCount=(crInit.ciMax.nLine - crInit.ciMin.nLine) + 1;

        //Char count
        is.ciChar1=&crInit.ciMax;
        is.ciChar2=&crInit.ciMin;
        is.bColumnSel=(BOOL)SendMessage(hWndEdit, AEM_GETCOLUMNSEL, 0, 0);
        is.nNewLine=AELB_ASOUTPUT;
        nCharCount=SendMessage(hWndEdit, AEM_INDEXSUBTRACT, 0, (LPARAM)&is);

        //Word count
        if (is.bColumnSel)
        {
          crCount.ciMin=crInit.ciMin;
          crCount.ciMax=crInit.ciMax;

          while (AEC_IndexCompare(&crCount.ciMin, &crCount.ciMax) < 0)
          {
            ciWordEnd=crCount.ciMin;

            for (;;)
            {
              if (!SendMessage(hWndEdit, AEM_GETNEXTBREAK, AEWB_RIGHTWORDEND, (LPARAM)&ciWordEnd))
                goto Symbols;

              if (ciWordEnd.nLine == crCount.ciMin.nLine && ciWordEnd.nCharInLine <= crCount.ciMin.lpLine->nSelEnd)
              {
                if (bFirst)
                {
                  bFirst=FALSE;
                  ciWordStart=ciWordEnd;

                  if (SendMessage(hWndEdit, AEM_GETPREVBREAK, AEWB_LEFTWORDSTART, (LPARAM)&ciWordStart))
                    if (AEC_IndexCompare(&crCount.ciMin, &ciWordStart) <= 0)
                      ++nWordCount;
                }
                else ++nWordCount;

                if (ciWordEnd.nCharInLine == crCount.ciMin.lpLine->nSelEnd)
                  break;
              }
              else break;
            }

            //Next line
            bFirst=TRUE;
            if (AEC_NextLine(&crCount.ciMin))
              crCount.ciMin.nCharInLine=crCount.ciMin.lpLine->nSelStart;
          }
        }
        else
        {
          crCount.ciMin=crInit.ciMin;
          crCount.ciMax=crInit.ciMax;

          for (;;)
          {
            ciWordEnd=crCount.ciMin;

            if (!SendMessage(hWndEdit, AEM_GETNEXTBREAK, AEWB_RIGHTWORDEND, (LPARAM)&ciWordEnd))
              goto Symbols;

            if (AEC_IndexCompare(&ciWordEnd, &crCount.ciMax) <= 0)
            {
              if (bFirst)
              {
                bFirst=FALSE;
                ciWordStart=ciWordEnd;

                if (SendMessage(hWndEdit, AEM_GETPREVBREAK, AEWB_LEFTWORDSTART, (LPARAM)&ciWordStart))
                  if (AEC_IndexCompare(&crCount.ciMin, &ciWordStart) <= 0)
                    ++nWordCount;
              }
              else ++nWordCount;

              if (AEC_IndexCompare(&ciWordEnd, &crCount.ciMax) == 0)
                break;
            }
            else break;

            //Next word
            crCount.ciMin=ciWordEnd;
          }
        }

        //Symbols detalization
        Symbols:

        for (ciCount=crInit.ciMin; AEC_IndexCompare(&ciCount, &crInit.ciMax) < 0; AEC_NextChar(&ciCount))
        {
          if (ciCount.nCharInLine < ciCount.lpLine->nSelStart)
            ciCount.nCharInLine=ciCount.lpLine->nSelStart;

          if (ciCount.nCharInLine < ciCount.lpLine->nLineLen &&
              ciCount.nCharInLine < ciCount.lpLine->nSelEnd)
          {
            if (AEC_IndexLen(&ciCount) == 1)
            {
              if (ciCount.lpLine->wpLine[ciCount.nCharInLine] <= 0x80)
              {
                if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L' ' ||
                    ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\t')
                {
                  ++nCharLatinSpaces;
                }
                else if ((ciCount.lpLine->wpLine[ciCount.nCharInLine] >= L'A' &&
                          ciCount.lpLine->wpLine[ciCount.nCharInLine] <= L'Z') ||
                         (ciCount.lpLine->wpLine[ciCount.nCharInLine] >= L'a' &&
                          ciCount.lpLine->wpLine[ciCount.nCharInLine] <= L'z'))
                {
                  ++nCharLatinLetters;
                }
                else if (ciCount.lpLine->wpLine[ciCount.nCharInLine] >= L'0' &&
                         ciCount.lpLine->wpLine[ciCount.nCharInLine] <= L'9')
                {
                  ++nCharLatinDigits;
                }
                else ++nCharLatinOther;
              }
              else ++nCharNonLatin;
            }
            else ++nCharSurrogate;
          }
          else
          {
            if (ciCount.lpLine->nLineBreak == AELB_R || ciCount.lpLine->nLineBreak == AELB_N)
              ++nCharLatinOther;
            else if (ciCount.lpLine->nLineBreak == AELB_RN)
              nCharLatinOther+=2;
            else if (ciCount.lpLine->nLineBreak == AELB_RRN)
              nCharLatinOther+=3;
            AEC_NextLine(&ciCount);
          }
        }
      }
    }

    //Show result
    xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_INFO), nCharCount, nCharLatinSpaces + nCharLatinLetters + nCharLatinDigits + nCharLatinOther, nCharLatinLetters, nCharLatinDigits, nCharLatinSpaces, nCharLatinOther, nCharNonLatin, nCharSurrogate, nWordCount, nLineCount);
    MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONINFORMATION);
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
    if (nStringID == STRID_INFO)
      return L"\x0421\x0438\x043C\x0432\x043E\x043B\x044B=%Id\n    \x041B\x0430\x0442\x0438\x043D\x0441\x043A\x0438\x0435=%Id\n        \x0411\x0443\x043A\x0432\x044B=%Id\n        \x0426\x0438\x0444\x0440\x044B=%Id\n        \x041F\x0440\x043E\x0431\x0435\x043B\x044B=%Id\n        \x0414\x0440\x0443\x0433\x0438\x0435=%Id\n    \x041D\x0435\x043B\x0430\x0442\x0438\x043D\x0441\x043A\x0438\x0435=%Id\n    \x0421\x0443\x0440\x0440\x043E\x0433\x0430\x0442\x044B=%Id\n\x0421\x043B\x043E\x0432\x0430=%d\n\x0421\x0442\x0440\x043E\x043A\x0438=%d\n";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
  }
  else
  {
    if (nStringID == STRID_INFO)
      return L"Chars=%Id\n    Latin=%Id\n        Letters=%Id\n        Digits=%Id\n        Spaces=%Id\n        Other=%Id\n    NonLatin=%Id\n    Surrogates=%Id\nWords=%d\nLines=%d\n";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
  }
  return L"";
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hMainWnd=pd->hMainWnd;
  hWndEdit=pd->hWndEdit;
  bOldWindows=pd->bOldWindows;
  bAkelEdit=pd->bAkelEdit;
  wLangModule=PRIMARYLANGID(pd->wLangModule);

  //Plugin name
  {
    int i;

    for (i=0; pd->wszFunction[i] != L':'; ++i)
      wszPluginName[i]=pd->wszFunction[i];
    wszPluginName[i]=L'\0';
  }
  xprintfW(wszPluginTitle, GetLangStringW(wLangModule, STRID_PLUGIN), wszPluginName);

  dwStatusPosType=(DWORD)SendMessage(hMainWnd, AKD_GETMAININFO, MI_STATUSPOSTYPE, 0);
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
