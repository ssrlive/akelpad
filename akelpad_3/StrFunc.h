/*****************************************************************
 *              String functions header v3.1                     *
 *                                                               *
 * 2008 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Functions:                                                     *
 * WordFindA, WordFindW, StrReplaceA, StrReplaceW,               *
 * GetOptionsA, GetOptionsW, xstrstrA, xstrstrW,                 *
 * xstrcmpA, xstrcmpW, xstrcmpnA, xstrcmpnW,                     *
 * FindWordStringA, FindWordStringW                              *
 *                                                               *
 *****************************************************************/

#ifndef _STRFUNC_H_
#define _STRFUNC_H_

int WordFindA(char *pText, char *pDelim, int nNumber, char *pOption, BOOL bSensitive, char *szResult, int *nMaxResult, char **ppResult);
int WordFindW(wchar_t *wpText, wchar_t *wpDelim, int nNumber, wchar_t *wpOption, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult, wchar_t **wppResult);
int StrReplaceA(char *pText, char *pIt, char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult);
int StrReplaceW(wchar_t *wpText, wchar_t *wpIt, wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult);
int GetOptionsA(char *pLine, char *pOption, BOOL bSensitive, char *szResult, int nMaxResult);
int GetOptionsW(wchar_t *wpLine, wchar_t *wpOption, BOOL bSensitive, wchar_t *wszResult, int nMaxResult);
BOOL xstrstrA(char *pText, char *pStr, BOOL bSensitive, char **pStrBegin, char **pStrEnd);
BOOL xstrstrW(wchar_t *wpText, wchar_t *wpStr, BOOL bSensitive, wchar_t **wpStrBegin, wchar_t **wpStrEnd);
int xstrcmpA(const char *pString, const char *pString2, BOOL bSensitive);
int xstrcmpW(const wchar_t *wpString, const wchar_t *wpString2, BOOL bSensitive);
int xstrcmpnA(const char *pString, const char *pString2, DWORD dwMaxLength, BOOL bSensitive);
int xstrcmpnW(const wchar_t *wpString, const wchar_t *wpString2, DWORD dwMaxLength, BOOL bSensitive);
int FindWordStringA(char *pText, char *pDelim, BOOL bSensitive, char *pWord, int *nMaxWord, char **ppWord);
int FindWordStringW(wchar_t *wpText, wchar_t *wpDelim, BOOL bSensitive, wchar_t *wpWord, int *nMaxWord, wchar_t **wppWord);

#endif

/********************************************************************
 *
 *  WordFindA
 *
 *String manipulation function.
 *
 * [in] char *pText      Text
 * [in] char *pDelim     Delimiter
 * [in] int nNumber      Number of the delimiter/word if positive
 *                        search from beginning if negative from end,
 *                        if (nNumber == 0) then returns sum of delimiters/words
 * [in] char *pOption    ">"     all text before founded delimiter
 *                       "<"     all text after founded delimiter
 *                       "<>"    deletes delimiter
 *                       "*"     text between delimiters (word)
 *                       "*>"    all text after founded word
 *                       ">*"    word and all text after founded word
 *                       "<*"    all text before founded word
 *                       "*<"    word and all text before founded word
 *                       "<*>"   deletes word and neighbouring delimiter
 * [in] BOOL bSensitive  TRUE   case sensitive
 *                       FALSE  case insensitive
 *[out] char *szResult   Output for result string, can be NULL
 *[out] int *nMaxResult  Contains the length of the result string,
 *                        not including the terminating null character,
 *                        can be NULL
 *[out] char **ppResult  Pointer to the first character of result string in pText,
 *                        can be NULL
 *
 *Returns (nRes):  sum of delimiters/words, if (nNumber == 0)
 *                 -1 syntax error or pDelim is empty
 *                 -2 no delimiters found
 *                 -3 no such delimiter/word number
 *
 *Defines:
 * #define WordFindA_UNMINUS  //nNumber only positive (uses for minimize program size)
 * #define WordFindA_UNPLUS   //nNumber only negative (uses for minimize program size)
 ********************************************************************/
#ifdef WordFindA
#define WordFindA_INCLUDED
#undef WordFindA
int WordFindA(char *pText, char *pDelim, int nNumber, char *pOption, BOOL bSensitive, char *szResult, int *nMaxResult, char **ppResult)
{
  char *pTextStart;
  char *pTextEnd;
  char *pTextMinus=NULL;
  char *pDelimCount;
  char *pDelimMinus=NULL;
  char *pWord;
  char *pResult=NULL;
  BOOL bFound=FALSE;
  BOOL bRow=TRUE;
  int nOption=0;
  int nCurStrNumber=0;
  int nCurWordNumber=0;
  int nDelimLen=lstrlenA(pDelim);
  int nResultSize=0;

  if (!lstrcmpA(pOption, ">")) nOption=1;
  else if (!lstrcmpA(pOption, "<")) nOption=2;
  else if (!lstrcmpA(pOption, "<>")) nOption=3;
  else if (!lstrcmpA(pOption, "*")) nOption=4;
  else if (!lstrcmpA(pOption, "*>")) nOption=5;
  else if (!lstrcmpA(pOption, ">*")) nOption=6;
  else if (!lstrcmpA(pOption, "<*")) nOption=7;
  else if (!lstrcmpA(pOption, "*<")) nOption=8;
  else if (!lstrcmpA(pOption, "<*>")) nOption=9;

  if (nDelimLen == 0 || nOption == 0)
    return -1;

  #ifndef WordFindA_UNPLUS
  if (nNumber >= 0)
  {
    for (pWord=pTextStart=pTextEnd=pText; *pTextStart && *pTextEnd; ++pTextStart)
    {
      for (pTextEnd=pTextStart, pDelimCount=pDelim;
           (*pTextEnd) &&
           ((bSensitive == TRUE && *pTextEnd == *pDelimCount) ||
            (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pTextEnd) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pDelimCount)));
           ++pTextEnd)
      {
        if (!*++pDelimCount)
        {
          if (nOption > 3)
          {
            if (bRow == FALSE && ++nCurWordNumber == nNumber)
            {
              if (nOption == 4)
              {
                pResult=pWord;
                nResultSize=pTextEnd - pResult - nDelimLen + 1;
              }
              else if (nOption == 5)
              {
                pResult=pTextEnd - nDelimLen + 1;
                nResultSize=lstrlenA(pResult);
              }
              else if (nOption == 6)
              {
                pResult=pWord;
                nResultSize=lstrlenA(pResult);
              }
              else if (nOption == 7)
              {
                pResult=pText;
                nResultSize=pWord - pResult;
              }
              else if (nOption == 8)
              {
                pResult=pText;
                nResultSize=pTextEnd - pResult - nDelimLen + 1;
              }
              else if (nOption == 9)
              {
                pResult=pTextEnd + 1;
                nResultSize=(pWord - pResult) + lstrlenA(pResult);
              }
              if (ppResult) *ppResult=pResult;
              if (nMaxResult) *nMaxResult=nResultSize;
              if (szResult)
              {
                if (nOption == 9)
                {
                  lstrcpynA(szResult, pText, pWord - pText + 1);
                  lstrcatA(szResult, pResult);
                }
                else lstrcpynA(szResult, pResult, nResultSize + 1);
              }
              return 0;
            }
            pWord=pTextEnd + 1;
            bRow=TRUE;
          }
          else if (++nCurStrNumber == nNumber)
          {
            if (nOption == 1)
            {
              pResult=pTextEnd + 1;
              nResultSize=lstrlenA(pResult);
            }
            else if (nOption == 2)
            {
              pResult=pText;
              nResultSize=pTextEnd - pResult - nDelimLen + 1;
            }
            else if (nOption == 3)
            {
              pResult=pTextEnd + 1;
              nResultSize=(pResult - pText - nDelimLen) + lstrlenA(pResult);
            }
            if (ppResult) *ppResult=pResult;
            if (nMaxResult) *nMaxResult=nResultSize;
            if (szResult)
            {
              if (nOption == 3)
              {
                lstrcpynA(szResult, pText, pTextEnd - pText - nDelimLen + 2);
                lstrcatA(szResult, pResult);
              }
              else lstrcpynA(szResult, pResult, nResultSize + 1);
            }
            return 0;
          }
          pTextStart=pTextEnd + 1;
          pDelimCount=pDelim;
          bFound=TRUE;
        }
      }
      if (*pTextStart && *pTextEnd) bRow=FALSE;
    }
    if (*pTextEnd) pTextEnd=pTextStart;

    if (nOption > 3 && bFound == TRUE && bRow == FALSE && ++nCurWordNumber == nNumber)
    {
      if (nOption == 4 || nOption == 6)
      {
        pResult=pWord;
        nResultSize=pTextEnd - pResult;
      }
      else if (nOption == 5)
      {
        pResult=pTextEnd;
        nResultSize=0;
      }
      else if (nOption == 7)
      {
        pResult=pText;
        nResultSize=pWord - pResult;
      }
      else if (nOption == 8)
      {
        pResult=pText;
        nResultSize=pTextEnd - pResult;
      }
      else if (nOption == 9)
      {
        pResult=pText;
        nResultSize=(pWord - nDelimLen) - pResult;
      }
      if (ppResult) *ppResult=pResult;
      if (nMaxResult) *nMaxResult=nResultSize;
      if (szResult)
      {
        lstrcpynA(szResult, pResult, nResultSize + 1);
      }
      return 0;
    }
    if (nNumber == 0)
    {
      if (nOption <= 3)
        return nCurStrNumber;
      return nCurWordNumber;
    }
  }
  #endif

  #ifndef WordFindA_UNMINUS
  else if (nNumber < 0)
  {
    pTextMinus=pTextStart=pTextEnd=pText + lstrlenA(pText) - 1;
    pDelimMinus=pDelim + nDelimLen - 1;
    pWord=pTextMinus + 1;

    for (; pTextEnd >= pText && pTextStart >= pText; --pTextEnd)
    {
      for (pTextStart=pTextEnd, pDelimCount=pDelimMinus;
           (pTextStart >= pText) &&
           ((bSensitive == TRUE && *pTextStart == *pDelimCount) ||
            (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pTextStart) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pDelimCount)));
           --pTextStart)
      {
        if (--pDelimCount < pDelim)
        {
          if (nOption > 3)
          {
            if (bRow == FALSE && --nCurWordNumber == nNumber)
            {
              if (nOption == 4)
              {
                pResult=pTextStart + nDelimLen;
                nResultSize=pWord - pResult;
              }
              else if (nOption == 5)
              {
                pResult=pWord;
                nResultSize=pTextMinus - pResult + 1;
              }
              else if (nOption == 6)
              {
                pResult=pTextStart + nDelimLen;
                nResultSize=pTextMinus - pResult + 1;
              }
              else if (nOption == 7)
              {
                pResult=pText;
                nResultSize=(pTextStart + nDelimLen) - pResult;
              }
              else if (nOption == 8)
              {
                pResult=pText;
                nResultSize=pWord - pResult;
              }
              else if (nOption == 9)
              {
                pResult=pTextStart;
                nResultSize=(pResult - pText) + (pTextMinus - pWord) + 1;
              }
              if (ppResult) *ppResult=pResult;
              if (nMaxResult) *nMaxResult=nResultSize;
              if (szResult)
              {
                if (nOption == 9)
                {
                  lstrcpynA(szResult, pText, pTextStart - pText + 1);
                  lstrcatA(szResult, pWord);
                }
                else lstrcpynA(szResult, pResult, nResultSize + 1);
              }
              return 0;
            }
            pWord=pTextStart;
            bRow=TRUE;
          }
          else if (--nCurStrNumber == nNumber)
          {
            if (nOption == 1)
            {
              pResult=pTextStart + nDelimLen;
              nResultSize=pTextMinus - pResult + 1;
            }
            else if (nOption == 2)
            {
              pResult=pText;
              nResultSize=pTextStart - pResult;
            }
            else if (nOption == 3)
            {
              pResult=pTextStart;
              nResultSize=pTextMinus - pText - nDelimLen + 1;
            }
            if (ppResult) *ppResult=pResult;
            if (nMaxResult) *nMaxResult=nResultSize;
            if (szResult)
            {
              if (nOption == 3)
              {
                lstrcpynA(szResult, pText, pTextStart - pText + 1);
                lstrcatA(szResult, pTextStart + nDelimLen);
              }
              else lstrcpynA(szResult, pResult, nResultSize + 1);
            }
            return 0;
          }
          pTextEnd=pTextStart;
          pDelimCount=pDelimMinus;
          bFound=TRUE;
        }
      }
      if (pTextEnd >= pText && pTextStart >= pText) bRow=FALSE;
    }
    if (nOption > 3 && bFound == TRUE && bRow == FALSE && --nCurWordNumber == nNumber)
    {
      if (nOption == 4 || nOption == 8)
      {
        pResult=pText;
        nResultSize=pWord - pResult;
      }
      else if (nOption == 5)
      {
        pResult=pWord;
        nResultSize=pTextMinus - pResult + 1;
      }
      else if (nOption == 6)
      {
        pResult=pText;
        nResultSize=pTextMinus - pResult + 1;
      }
      else if (nOption == 7)
      {
        pResult=pText;
        nResultSize=0;
      }
      else if (nOption == 9)
      {
        pResult=pWord + nDelimLen;
        nResultSize=pTextMinus - pResult + 1;
      }
      if (ppResult) *ppResult=pResult;
      if (nMaxResult) *nMaxResult=nResultSize;
      if (szResult)
      {
        lstrcpynA(szResult, pResult, nResultSize + 1);
      }
      return 0;
    }
  }
  #endif

  if (bFound == TRUE)
    return -3;
  else
    return -2;
}
#endif


/********************************************************************
 *
 *  WordFindW
 *
 *Unicode string manipulation function.
 *
 * [in] wchar_t *wpText      Text
 * [in] wchar_t *wpDelim     Delimiter
 * [in] int nNumber          Number of the delimiter/word if positive
 *                            search from beginning if negative from end,
 *                            if (nNumber == 0) then returns sum of delimiters/words
 * [in] wchar_t *wpOption    L">"     all text before founded delimiter
 *                           L"<"     all text after founded delimiter
 *                           L"<>"    deletes delimiter
 *                           L"*"     text between delimiters (word)
 *                           L"*>"    all text after founded word
 *                           L">*"    word and all text after founded word
 *                           L"<*"    all text before founded word
 *                           L"*<"    word and all text before founded word
 *                           L"<*>"   deletes word and neighbouring delimiter
 * [in] BOOL bSensitive      TRUE   case sensitive
 *                           FALSE  case insensitive
 *[out] wchar_t *wszResult   Output for result string, can be NULL
 *[out] int *nMaxResult      Contains the length of the result string,
 *                            not including the terminating null character,
 *                            can be NULL
 *[out] wchar_t **wppResult  Pointer to the first character of result string in wpText,
 *                            can be NULL
 *
 *Returns (nRes):  sum of delimiters/words, if (nNumber == 0)
 *                 -1 syntax error or wpDelim is empty
 *                 -2 no delimiters found
 *                 -3 no such delimiter/word number
 *
 *Defines:
 * #define WordFindW_UNMINUS  //nNumber only positive (uses for minimize program size)
 * #define WordFindW_UNPLUS   //nNumber only negative (uses for minimize program size)
 ********************************************************************/
#ifdef WordFindW
#define WordFindW_INCLUDED
#undef WordFindW
int WordFindW(wchar_t *wpText, wchar_t *wpDelim, int nNumber, wchar_t *wpOption, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult, wchar_t **wppResult)
{
  wchar_t *wpTextStart;
  wchar_t *wpTextEnd;
  wchar_t *wpTextMinus=NULL;
  wchar_t *wpDelimCount;
  wchar_t *wpDelimMinus=NULL;
  wchar_t *wpWord;
  wchar_t *wpResult=NULL;
  BOOL bFound=FALSE;
  BOOL bRow=TRUE;
  int nOption=0;
  int nCurStrNumber=0;
  int nCurWordNumber=0;
  int nDelimLen=lstrlenW(wpDelim);
  int nResultSize=0;

  if (!lstrcmpW(wpOption, L">")) nOption=1;
  else if (!lstrcmpW(wpOption, L"<")) nOption=2;
  else if (!lstrcmpW(wpOption, L"<>")) nOption=3;
  else if (!lstrcmpW(wpOption, L"*")) nOption=4;
  else if (!lstrcmpW(wpOption, L"*>")) nOption=5;
  else if (!lstrcmpW(wpOption, L">*")) nOption=6;
  else if (!lstrcmpW(wpOption, L"<*")) nOption=7;
  else if (!lstrcmpW(wpOption, L"*<")) nOption=8;
  else if (!lstrcmpW(wpOption, L"<*>")) nOption=9;

  if (nDelimLen == 0 || nOption == 0)
    return -1;

  #ifndef WordFindW_UNPLUS
  if (nNumber >= 0)
  {
    for (wpWord=wpTextStart=wpTextEnd=wpText; *wpTextStart && *wpTextEnd; ++wpTextStart)
    {
      for (wpTextEnd=wpTextStart, wpDelimCount=wpDelim;
           (*wpTextEnd) &&
           ((bSensitive == TRUE && *wpTextEnd == *wpDelimCount) ||
            (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpTextEnd) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpDelimCount)));
           ++wpTextEnd)
      {
        if (!*++wpDelimCount)
        {
          if (nOption > 3)
          {
            if (bRow == FALSE && ++nCurWordNumber == nNumber)
            {
              if (nOption == 4)
              {
                wpResult=wpWord;
                nResultSize=wpTextEnd - wpResult - nDelimLen + 1;
              }
              else if (nOption == 5)
              {
                wpResult=wpTextEnd - nDelimLen + 1;
                nResultSize=lstrlenW(wpResult);
              }
              else if (nOption == 6)
              {
                wpResult=wpWord;
                nResultSize=lstrlenW(wpResult);
              }
              else if (nOption == 7)
              {
                wpResult=wpText;
                nResultSize=wpWord - wpResult;
              }
              else if (nOption == 8)
              {
                wpResult=wpText;
                nResultSize=wpTextEnd - wpResult - nDelimLen + 1;
              }
              else if (nOption == 9)
              {
                wpResult=wpTextEnd + 1;
                nResultSize=(wpWord - wpResult) + lstrlenW(wpResult);
              }
              if (wppResult) *wppResult=wpResult;
              if (nMaxResult) *nMaxResult=nResultSize;
              if (wszResult)
              {
                if (nOption == 9)
                {
                  lstrcpynW(wszResult, wpText, wpWord - wpText + 1);
                  lstrcatW(wszResult, wpResult);
                }
                else lstrcpynW(wszResult, wpResult, nResultSize + 1);
              }
              return 0;
            }
            wpWord=wpTextEnd + 1;
            bRow=TRUE;
          }
          else if (++nCurStrNumber == nNumber)
          {
            if (nOption == 1)
            {
              wpResult=wpTextEnd + 1;
              nResultSize=lstrlenW(wpResult);
            }
            else if (nOption == 2)
            {
              wpResult=wpText;
              nResultSize=wpTextEnd - wpResult - nDelimLen + 1;
            }
            else if (nOption == 3)
            {
              wpResult=wpTextEnd + 1;
              nResultSize=(wpResult - wpText - nDelimLen) + lstrlenW(wpResult);
            }
            if (wppResult) *wppResult=wpResult;
            if (nMaxResult) *nMaxResult=nResultSize;
            if (wszResult)
            {
              if (nOption == 3)
              {
                lstrcpynW(wszResult, wpText, wpTextEnd - wpText - nDelimLen + 2);
                lstrcatW(wszResult, wpResult);
              }
              else lstrcpynW(wszResult, wpResult, nResultSize + 1);
            }
            return 0;
          }
          wpTextStart=wpTextEnd + 1;
          wpDelimCount=wpDelim;
          bFound=TRUE;
        }
      }
      if (*wpTextStart && *wpTextEnd) bRow=FALSE;
    }
    if (*wpTextEnd) wpTextEnd=wpTextStart;

    if (nOption > 3 && bFound == TRUE && bRow == FALSE && ++nCurWordNumber == nNumber)
    {
      if (nOption == 4 || nOption == 6)
      {
        wpResult=wpWord;
        nResultSize=wpTextEnd - wpResult;
      }
      else if (nOption == 5)
      {
        wpResult=wpTextEnd;
        nResultSize=0;
      }
      else if (nOption == 7)
      {
        wpResult=wpText;
        nResultSize=wpWord - wpResult;
      }
      else if (nOption == 8)
      {
        wpResult=wpText;
        nResultSize=wpTextEnd - wpResult;
      }
      else if (nOption == 9)
      {
        wpResult=wpText;
        nResultSize=(wpWord - nDelimLen) - wpResult;
      }
      if (wppResult) *wppResult=wpResult;
      if (nMaxResult) *nMaxResult=nResultSize;
      if (wszResult)
      {
        lstrcpynW(wszResult, wpResult, nResultSize + 1);
      }
      return 0;
    }
    if (nNumber == 0)
    {
      if (nOption <= 3)
        return nCurStrNumber;
      return nCurWordNumber;
    }
  }
  #endif

  #ifndef WordFindW_UNMINUS
  else if (nNumber < 0)
  {
    wpTextMinus=wpTextStart=wpTextEnd=wpText + lstrlenW(wpText) - 1;
    wpDelimMinus=wpDelim + nDelimLen - 1;
    wpWord=wpTextMinus + 1;

    for (; wpTextEnd >= wpText && wpTextStart >= wpText; --wpTextEnd)
    {
      for (wpTextStart=wpTextEnd, wpDelimCount=wpDelimMinus;
           (wpTextStart >= wpText) &&
           ((bSensitive == TRUE && *wpTextStart == *wpDelimCount) ||
            (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpTextStart) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpDelimCount)));
           --wpTextStart)
      {
        if (--wpDelimCount < wpDelim)
        {
          if (nOption > 3)
          {
            if (bRow == FALSE && --nCurWordNumber == nNumber)
            {
              if (nOption == 4)
              {
                wpResult=wpTextStart + nDelimLen;
                nResultSize=wpWord - wpResult;
              }
              else if (nOption == 5)
              {
                wpResult=wpWord;
                nResultSize=wpTextMinus - wpResult + 1;
              }
              else if (nOption == 6)
              {
                wpResult=wpTextStart + nDelimLen;
                nResultSize=wpTextMinus - wpResult + 1;
              }
              else if (nOption == 7)
              {
                wpResult=wpText;
                nResultSize=(wpTextStart + nDelimLen) - wpResult;
              }
              else if (nOption == 8)
              {
                wpResult=wpText;
                nResultSize=wpWord - wpResult;
              }
              else if (nOption == 9)
              {
                wpResult=wpTextStart;
                nResultSize=(wpResult - wpText) + (wpTextMinus - wpWord) + 1;
              }
              if (wppResult) *wppResult=wpResult;
              if (nMaxResult) *nMaxResult=nResultSize;
              if (wszResult)
              {
                if (nOption == 9)
                {
                  lstrcpynW(wszResult, wpText, wpTextStart - wpText + 1);
                  lstrcatW(wszResult, wpWord);
                }
                else lstrcpynW(wszResult, wpResult, nResultSize + 1);
              }
              return 0;
            }
            wpWord=wpTextStart;
            bRow=TRUE;
          }
          else if (--nCurStrNumber == nNumber)
          {
            if (nOption == 1)
            {
              wpResult=wpTextStart + nDelimLen;
              nResultSize=wpTextMinus - wpResult + 1;
            }
            else if (nOption == 2)
            {
              wpResult=wpText;
              nResultSize=wpTextStart - wpResult;
            }
            else if (nOption == 3)
            {
              wpResult=wpTextStart;
              nResultSize=wpTextMinus - wpText - nDelimLen + 1;
            }
            if (wppResult) *wppResult=wpResult;
            if (nMaxResult) *nMaxResult=nResultSize;
            if (wszResult)
            {
              if (nOption == 3)
              {
                lstrcpynW(wszResult, wpText, wpTextStart - wpText + 1);
                lstrcatW(wszResult, wpTextStart + nDelimLen);
              }
              else lstrcpynW(wszResult, wpResult, nResultSize + 1);
            }
            return 0;
          }
          wpTextEnd=wpTextStart;
          wpDelimCount=wpDelimMinus;
          bFound=TRUE;
        }
      }
      if (wpTextEnd >= wpText && wpTextStart >= wpText) bRow=FALSE;
    }
    if (nOption > 3 && bFound == TRUE && bRow == FALSE && --nCurWordNumber == nNumber)
    {
      if (nOption == 4 || nOption == 8)
      {
        wpResult=wpText;
        nResultSize=wpWord - wpResult;
      }
      else if (nOption == 5)
      {
        wpResult=wpWord;
        nResultSize=wpTextMinus - wpResult + 1;
      }
      else if (nOption == 6)
      {
        wpResult=wpText;
        nResultSize=wpTextMinus - wpResult + 1;
      }
      else if (nOption == 7)
      {
        wpResult=wpText;
        nResultSize=0;
      }
      else if (nOption == 9)
      {
        wpResult=wpWord + nDelimLen;
        nResultSize=wpTextMinus - wpResult + 1;
      }
      if (wppResult) *wppResult=wpResult;
      if (nMaxResult) *nMaxResult=nResultSize;
      if (wszResult)
      {
        lstrcpynW(wszResult, wpResult, nResultSize + 1);
      }
      return 0;
    }
  }
  #endif

  if (bFound == TRUE)
    return -3;
  else
    return -2;
}
#endif

/********************************************************************
 *
 *  StrReplaceA
 *
 *Replace substring in string.
 *
 * [in] char *pText      Text
 * [in] char *pIt        Replace it
 * [in] char *pWith      Replace with
 * [in] BOOL bSensitive  TRUE   case sensitive
 *                       FALSE  case insensitive
 *[out] char *szResult   Output, can be NULL
 *[out] int *nMaxResult  Contains the length of the result string,
 *                        including the terminating null character,
 *                        can be NULL
 *
 *Returns:  Number of changes
 ********************************************************************/
#ifdef StrReplaceA
#define StrReplaceA_INCLUDED
#undef StrReplaceA
int StrReplaceA(char *pText, char *pIt, char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult)
{
  char *pTextCount;
  char *pItCount;
  char *pResult=szResult;
  int nWithLen=lstrlenA(pWith);
  int nChanges=0;
  int i;

  for (pTextCount=pText; *pText; ++pText)
  {
    for (pTextCount=pText, pItCount=pIt;
         (bSensitive == TRUE && *pTextCount == *pItCount) ||
         (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pTextCount) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pItCount));
         ++pTextCount)
    {
      if (!*++pItCount)
      {
        if (szResult)
        {
          for (i=0; i < nWithLen; ++i)
            *pResult++=pWith[i];
        }
        else pResult+=nWithLen;

        pText=pTextCount + 1;
        pItCount=pIt;
        ++nChanges;
        if (!*pText) goto End;
      }
    }
    if (szResult) *pResult=*pText;
    ++pResult;
  }

  End:
  if (szResult) *pResult=*pText;
  if (nMaxResult) *nMaxResult=(pResult - szResult) + 1;
  return nChanges;
}
#endif

/********************************************************************
 *
 *  StrReplaceW
 *
 *Replace substring in unicode string.
 *
 * [in] wchar_t *wpText      Text
 * [in] wchar_t *wpIt        Replace it
 * [in] wchar_t *wpWith      Replace with
 * [in] BOOL bSensitive      TRUE   case sensitive
 *                           FALSE  case insensitive
 *[out] wchar_t *wszResult   Output, can be NULL
 *[out] int *nMaxResult      Contains the length of the result string,
 *                            including the terminating null character,
 *                            can be NULL
 *
 *Returns:  Number of changes
 ********************************************************************/
#ifdef StrReplaceW
#define StrReplaceW_INCLUDED
#undef StrReplaceW
int StrReplaceW(wchar_t *wpText, wchar_t *wpIt, wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult)
{
  wchar_t *wpTextCount;
  wchar_t *wpItCount;
  wchar_t *wpResult=wszResult;
  int nWithLen=lstrlenW(wpWith);
  int nChanges=0;
  int i;

  for (wpTextCount=wpText; *wpText; ++wpText)
  {
    for (wpTextCount=wpText, wpItCount=wpIt;
         (bSensitive == TRUE && *wpTextCount == *wpItCount) ||
         (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpTextCount) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpItCount));
         ++wpTextCount)
    {
      if (!*++wpItCount)
      {
        if (wszResult)
        {
          for (i=0; i < nWithLen; ++i)
            *wpResult++=wpWith[i];
        }
        else wpResult+=nWithLen;

        wpText=wpTextCount + 1;
        wpItCount=wpIt;
        ++nChanges;
        if (!*wpText) goto End;
      }
    }
    if (wszResult) *wpResult=*wpText;
    ++wpResult;
  }

  End:
  if (wszResult) *wpResult=*wpText;
  if (nMaxResult) *nMaxResult=(wpResult - wszResult) + 1;
  return nChanges;
}
#endif

/********************************************************************
 *
 *  GetOptionsA
 *
 *Gets option string from parameters line.
 *
 * [in] char *pLine      Parameters line
 * [in] char *pOption    Option
 * [in] BOOL bSensitive  TRUE   case sensitive
 *                       FALSE  case insensitive
 *[out] char *szResult   Output, can be NULL
 * [in] int nMaxResult   Output buffer size
 *
 *Returns:  length of the string copied to szResult,
 *          including the terminating null character
 ********************************************************************/
#ifdef GetOptionsA
#define GetOptionsA_INCLUDED
#undef GetOptionsA
int GetOptionsA(char *pLine, char *pOption, BOOL bSensitive, char *szResult, int nMaxResult)
{
  char *pLineStart;
  char *pLineEnd;
  char *pOptionCount;
  char *pOptionString=NULL;
  char chQuote='\0';
  char chDelimiter=*pOption++;
  int nBytes=0;

  for (pLineStart=pLineEnd=pLine; *pLineStart && *pLineEnd; ++pLineStart)
  {
    if (chQuote == '\0' && *pLineStart != '\"' && *pLineStart != '\'' && *pLineStart != '`')
    {
      if (*pLineStart == chDelimiter)
      {
        if (pOptionString) break;

        for (pLineEnd=pLineStart + 1, pOptionCount=pOption;
             (*pLineEnd) &&
             ((bSensitive == TRUE && *pLineEnd == *pOptionCount) ||
              (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pLineEnd) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pOptionCount)));
             ++pLineEnd)
        {
          if (!*++pOptionCount)
          {
            pLineStart=pLineEnd;
            pOptionString=pLineEnd + 1;
            break;
          }
        }
      }
    }
    else if (chQuote == '\0')
      chQuote=*pLineStart;
    else if (*pLineStart == chQuote)
      chQuote='\0';
  }
  if (pOptionString)
  {
    while (*--pLineStart == ' ' || *pLineStart == '\t');

    if (*pOptionString == *pLineStart && (*pOptionString == '\"' || *pOptionString == '\'' || *pOptionString == '`'))
    {
      ++pOptionString, --pLineStart;
    }
    nBytes=pLineStart - pOptionString + 2;
    if (szResult)
    {
      if (nMaxResult < nBytes) nBytes=nMaxResult;
      lstrcpynA(szResult, pOptionString, nBytes);
    }
  }
  return nBytes;
}
#endif

/********************************************************************
 *
 *  GetOptionsW
 *
 *Gets option string from unicode parameters line.
 *
 * [in] wchar_t *wpLine      Parameters line
 * [in] wchar_t *wpOption    Option
 * [in] BOOL bSensitive      TRUE   case sensitive
 *                           FALSE  case insensitive
 *[out] wchar_t *wszResult   Output, can be NULL
 * [in] int nMaxResult       Output buffer size
 *
 *Returns:  length of the string copied to wszResult,
 *          including the terminating null character
 ********************************************************************/
#ifdef GetOptionsW
#define GetOptionsW_INCLUDED
#undef GetOptionsW
int GetOptionsW(wchar_t *wpLine, wchar_t *wpOption, BOOL bSensitive, wchar_t *wszResult, int nMaxResult)
{
  wchar_t *wpLineStart;
  wchar_t *wpLineEnd;
  wchar_t *wpOptionCount;
  wchar_t *wpOptionString=NULL;
  wchar_t wchQuote='\0';
  wchar_t wchDelimiter=*wpOption++;
  int nBytes=0;

  for (wpLineStart=wpLineEnd=wpLine; *wpLineStart && *wpLineEnd; ++wpLineStart)
  {
    if (wchQuote == '\0' && *wpLineStart != '\"' && *wpLineStart != '\'' && *wpLineStart != '`')
    {
      if (*wpLineStart == wchDelimiter)
      {
        if (wpOptionString) break;

        for (wpLineEnd=wpLineStart + 1, wpOptionCount=wpOption;
             (*wpLineEnd) &&
             ((bSensitive == TRUE && *wpLineEnd == *wpOptionCount) ||
              (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpLineEnd) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpOptionCount)));
             ++wpLineEnd)
        {
          if (!*++wpOptionCount)
          {
            wpLineStart=wpLineEnd;
            wpOptionString=wpLineEnd + 1;
            break;
          }
        }
      }
    }
    else if (wchQuote == '\0')
      wchQuote=*wpLineStart;
    else if (*wpLineStart == wchQuote)
      wchQuote='\0';
  }
  if (wpOptionString)
  {
    while (*--wpLineStart == ' ' || *wpLineStart == '\t');

    if (*wpOptionString == *wpLineStart && (*wpOptionString == '\"' || *wpOptionString == '\'' || *wpOptionString == '`'))
    {
      ++wpOptionString, --wpLineStart;
    }
    nBytes=wpLineStart - wpOptionString + 2;
    if (wszResult)
    {
      if (nMaxResult < nBytes) nBytes=nMaxResult;
      lstrcpynW(wszResult, wpOptionString, nBytes);
    }
  }
  return nBytes;
}
#endif

/********************************************************************
 *
 *  xstrstrA
 *
 *Find substring in string.
 *
 * [in] char *pText      Text
 * [in] char *pStr       Find it
 * [in] BOOL bSensitive  TRUE   case sensitive
 *                       FALSE  case insensitive
 *[out] char *pStrBegin  Pointer to the first char of pStr
 *[out] char *pStrEnd    Pointer to the first char after pStr
 *
 *Returns:  TRUE  pStr is founded
 *          FALSE pStr isn't founded
 ********************************************************************/
#ifdef xstrstrA
#define xstrstrA_INCLUDED
#undef xstrstrA
BOOL xstrstrA(char *pText, char *pStr, BOOL bSensitive, char **pStrBegin, char **pStrEnd)
{
  char *pTextCount;
  char *pStrCount;

  for (; *pText; ++pText)
  {
    for (pTextCount=pText, pStrCount=pStr;
         (bSensitive == TRUE && *pTextCount == *pStrCount) ||
         (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pTextCount) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pStrCount));
         ++pTextCount)
    {
      if (!*++pStrCount)
      {
        if (pStrBegin) *pStrBegin=pText;
        if (pStrEnd) *pStrEnd=pTextCount + 1;
        return TRUE;
      }
    }
  }
  return FALSE;
}
#endif

/********************************************************************
 *
 *  xstrstrW
 *
 *Find substring in unicode string.
 *
 * [in] wchar_t *wpText      Text
 * [in] wchar_t *wpStr       Find it
 * [in] BOOL bSensitive      TRUE   case sensitive
 *                           FALSE  case insensitive
 *[out] wchar_t *wpStrBegin  Pointer to the first char of wpStr
 *[out] wchar_t *wpStrEnd    Pointer to the first char after wpStr
 *
 *Returns:  TRUE  wpStr is founded
 *          FALSE wpStr isn't founded
 ********************************************************************/
#ifdef xstrstrW
#define xstrstrW_INCLUDED
#undef xstrstrW
BOOL xstrstrW(wchar_t *wpText, wchar_t *wpStr, BOOL bSensitive, wchar_t **wpStrBegin, wchar_t **wpStrEnd)
{
  wchar_t *wpTextCount;
  wchar_t *wpStrCount;

  for (; *wpText; ++wpText)
  {
    for (wpTextCount=wpText, wpStrCount=wpStr;
         (bSensitive == TRUE && *wpTextCount == *wpStrCount) ||
         (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpTextCount) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpStrCount));
         ++wpTextCount)
    {
      if (!*++wpStrCount)
      {
        if (wpStrBegin) *wpStrBegin=wpText;
        if (wpStrEnd) *wpStrEnd=wpTextCount + 1;
        return TRUE;
      }
    }
  }
  return FALSE;
}
#endif

/********************************************************************
 *
 *  xstrcmpA
 *
 *Compare characters of two strings.
 *
 *[in] char *pString    First string to compare
 *[in] char *pString2   Second string to compare
 *[in] BOOL bSensitive  TRUE   case sensitive
 *                      FALSE  case insensitive
 *
 *Returns:  -1 string1 less than string2
 *           0 string1 identical to string2
 *           1 string1 greater than string2
 ********************************************************************/
#ifdef xstrcmpA
#define xstrcmpA_INCLUDED
#undef xstrcmpA
int xstrcmpA(const char *pString, const char *pString2, BOOL bSensitive)
{
  while (*pString &&
         ((bSensitive == TRUE && *pString == *pString2) ||
          (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString2))))
  {
    ++pString;
    ++pString2;
  }

  if (*pString == *pString2) return 0;
  if (bSensitive == TRUE)
  {
    if ((DWORD)*pString < (DWORD)*pString2)
      return -1;
  }
  else
  {
    if ((WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString) < (WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString2))
      return -1;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  xstrcmpW
 *
 *Compare characters of two unicode strings.
 *
 *[in] wchar_t *wpString    First string to compare
 *[in] wchar_t *wpString2   Second string to compare
 *[in] BOOL bSensitive      TRUE   case sensitive
 *                          FALSE  case insensitive
 *
 *Returns:  -1 string1 less than string2
 *           0 string1 identical to string2
 *           1 string1 greater than string2
 ********************************************************************/
#ifdef xstrcmpW
#define xstrcmpW_INCLUDED
#undef xstrcmpW
int xstrcmpW(const wchar_t *wpString, const wchar_t *wpString2, BOOL bSensitive)
{
  while (*wpString &&
         ((bSensitive == TRUE && *wpString == *wpString2) ||
          (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString2))))
  {
    ++wpString;
    ++wpString2;
  }

  if (*wpString == *wpString2) return 0;
  if (bSensitive == TRUE)
  {
    if ((DWORD)*wpString < (DWORD)*wpString2)
      return -1;
  }
  else
  {
    if ((WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString) < (WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString2))
      return -1;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  xstrcmpnA
 *
 *Compare characters of two strings.
 *
 *[in] char *pString      First string to compare
 *[in] char *pString2     Second string to compare
 *[in] DWORD dwMaxLength  Number of characters to compare,
 *                         -1 compare until NULL character in pString
 *[in] BOOL bSensitive    TRUE   case sensitive
 *                        FALSE  case insensitive
 *
 *Returns:  -1 string1 less than string2
 *           0 string1 identical to string2
 *           1 string1 greater than string2
 ********************************************************************/
#ifdef xstrcmpnA
#define xstrcmpnA_INCLUDED
#undef xstrcmpnA
int xstrcmpnA(const char *pString, const char *pString2, DWORD dwMaxLength, BOOL bSensitive)
{
  DWORD i;

  for (i=0;
       i < dwMaxLength && pString[i] &&
       ((bSensitive == TRUE && pString[i] == pString2[i]) ||
        (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)pString[i]) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)pString2[i])));
       ++i);

  if (dwMaxLength == (DWORD)-1 && !pString[i]) return 0;
  if (i >= dwMaxLength) return 0;
  if (pString[i] == pString2[i]) return 0;
  if (bSensitive == TRUE)
  {
    if ((DWORD)pString[i] < (DWORD)pString2[i])
      return -1;
  }
  else
  {
    if ((WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)pString[i]) < (WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)pString2[i]))
      return -1;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  xstrcmpnW
 *
 *Compare characters of two unicode strings.
 *
 *[in] wchar_t *wpString    First string to compare
 *[in] wchar_t *wpString2   Second string to compare
 *[in] DWORD dwMaxLength    Number of characters to compare,
 *                           -1 compare until NULL character in wpString
 *[in] BOOL bSensitive      TRUE   case sensitive
 *                          FALSE  case insensitive
 *
 *Returns:  -1 string1 less than string2
 *           0 string1 identical to string2
 *           1 string1 greater than string2
 ********************************************************************/
#ifdef xstrcmpnW
#define xstrcmpnW_INCLUDED
#undef xstrcmpnW
int xstrcmpnW(const wchar_t *wpString, const wchar_t *wpString2, DWORD dwMaxLength, BOOL bSensitive)
{
  DWORD i;

  for (i=0;
       i < dwMaxLength && wpString[i] &&
       ((bSensitive == TRUE && wpString[i] == wpString2[i]) ||
        (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)wpString[i]) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)wpString2[i])));
       ++i);

  if (dwMaxLength == (DWORD)-1 && !wpString[i]) return 0;
  if (i >= dwMaxLength) return 0;
  if (wpString[i] == wpString2[i]) return 0;
  if (bSensitive == TRUE)
  {
    if ((DWORD)wpString[i] < (DWORD)wpString2[i])
      return -1;
  }
  else
  {
    if ((WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)wpString[i]) < (WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)wpString2[i]))
      return -1;
  }
  return 1;
}
#endif

/********************************************************************
 *
 *  FindWordStringA
 *
 *Find word substring in string.
 *
 * [in] char *pText      Text
 * [in] char *pDelim     Delimiter
 * [in] BOOL bSensitive  TRUE   case sensitive
 *                       FALSE  case insensitive
 * [in] char *pWord      Word to find
 *[out] int *nMaxWord    Contains the length of the result word,
 *                        not including the terminating null character,
 *                        can be NULL
 *[out] char **ppWord    Pointer to the first character of result word in pText,
 *                        can be NULL
 *
 *Returns:  Number of the result word in pText
 *
 *Note:
 *  FindWordStringA uses WordFindA, xstrcmpnA
 ********************************************************************/
#ifdef FindWordStringA
#define FindWordStringA_INCLUDED
#undef FindWordStringA
int FindWordStringA(char *pText, char *pDelim, BOOL bSensitive, char *pWord, int *nMaxWord, char **ppWord)
{
  char *pStart=pText;
  int nWordLen=lstrlenA(pWord);
  int nLen;
  int nNumber;

  for (nNumber=1; !WordFindA(pStart, pDelim, 1, "*", bSensitive, NULL, &nLen, &pStart); ++nNumber, pStart+=nLen)
  {
    if (nWordLen == nLen && !xstrcmpnA(pStart, pWord, nWordLen, bSensitive))
    {
      if (nMaxWord) *nMaxWord=nLen;
      if (ppWord) *ppWord=pStart;
      return nNumber;
    }
  }
  return 0;
}
#endif

/********************************************************************
 *
 *  FindWordStringW
 *
 *Find word substring in unicode string.
 *
 * [in] wchar_t *wpText      Text
 * [in] wchar_t *wpDelim     Delimiter
 * [in] BOOL bSensitive      TRUE   case sensitive
 *                           FALSE  case insensitive
 * [in] wchar_t *wpWord      Word to find
 *[out] int *nMaxWord        Contains the length of the result word,
 *                            not including the terminating null character,
 *                            can be NULL
 *[out] wchar_t **wppWord    Pointer to the first character of result word in wpText,
 *                            can be NULL
 *
 *Returns:  Number of the result word in wpText
 *
 *Note:
 *  FindWordStringW uses WordFindW, xstrcmpnW
 ********************************************************************/
#ifdef FindWordStringW
#define FindWordStringW_INCLUDED
#undef FindWordStringW
int FindWordStringW(wchar_t *wpText, wchar_t *wpDelim, BOOL bSensitive, wchar_t *wpWord, int *nMaxWord, wchar_t **wppWord)
{
  wchar_t *wpStart=wpText;
  int nWordLen=lstrlenW(wpWord);
  int nLen;
  int nNumber;

  for (nNumber=1; !WordFindW(wpStart, wpDelim, 1, L"*", bSensitive, NULL, &nLen, &wpStart); ++nNumber, wpStart+=nLen)
  {
    if (nWordLen == nLen && !xstrcmpnW(wpStart, wpWord, nWordLen, bSensitive))
    {
      if (nMaxWord) *nMaxWord=nLen;
      if (wppWord) *wppWord=wpStart;
      return nNumber;
    }
  }
  return 0;
}
#endif


/********************************************************************
 *                                                                  *
 *                           Example                                *
 *                                                                  *
 ********************************************************************

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "StrFunc.h"

//insert functions
#define WordFindA
#define StrReplaceA
#define GetOptionsA
#define xstrstrA
#define xstrcmpA
#define xstrcmpnA
#define FindWordStringA
#include "StrFunc.h"

void main()
{
  char szResult[MAX_PATH];
  char *pStringBegin=NULL;
  char *pStringEnd=NULL;
  int nStringLen;
  int nError;

  nError=WordFindA("ABC||dfg||HJK", "||", 2, "*", TRUE, szResult, &nStringLen, &pStringBegin);
  printf("szResult={%s}, nStringLen={%d}, pStringBegin={%s}, nError={%d}\n", szResult, nStringLen, pStringBegin, nError);

  nError=StrReplaceA("ABC||dfg||HJK", "||", "##", TRUE, szResult, &nStringLen);
  printf("szResult={%s}, nStringLen={%d}, nError={%d}\n", szResult, nStringLen, nError);

  nError=GetOptionsA("/A=123 /B=\"456\" /C=`789`", "/b=", FALSE, szResult, MAX_PATH);
  printf("szResult={%s}, nError={%d}\n", szResult, nError);

  nError=xstrstrA("ABC||dfg||HJK", "Dfg", FALSE, &pStringBegin, &pStringEnd);
  printf("pStringBegin={%s}, pStringEnd={%s}, nError={%d}\n", pStringBegin, pStringEnd, nError);

  nError=xstrcmpA("ABC", "abc", FALSE);
  printf("nError={%d}\n", nError);

  nError=xstrcmpnA("ABCdfg", "abcxyz", 3, FALSE);
  printf("nError={%d}\n", nError);

  nError=FindWordStringA("ABC||dfg||HJK", "||", TRUE, "dfg", &nStringLen, &pStringBegin);
  printf("nStringLen={%d}, pStringBegin={%s}, nError={%d}\n", nStringLen, pStringBegin, nError);
}

*/
