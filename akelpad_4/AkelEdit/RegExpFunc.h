/******************************************************************
 *                  RegExp functions header v1.1                  *
 *                                                                *
 * 2012 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)   *
 *                                                                *
 *                                                                *
 * RegExpFunc.h header uses functions:                            *
 * 1. From StrFunc.h header                                       *
 *   WideCharUpper, WideCharLower, xmemcpy, xstrlenW, xstrcpyW,   *
 *   xstrcmpnW, xstrcmpinW, xatoiW, hex2decW                      *
 * 2. From StackFunc.h header                                     *
 *   StackInsertBefore, StackDelete, StackJoin                    *
 *****************************************************************/

#ifndef _REGEXPFUNC_H_
#define _REGEXPFUNC_H_

//STACKREGROUP options
#define REO_MATCHCASE        0x001 //Case-sensitive search.
#define REO_MULTILINE        0x002 //Multiline search. Symbols ^ and $ specifies the line edge.
#define REO_NOFIRSTLINEBEGIN 0x004 //String starts not from line beginning. Used with REO_MULTILINE flag. AE_PatExec ignore this flag.
#define REO_NOLASTLINEEND    0x008 //String ends not on line ending. Used with REO_MULTILINE flag. AE_PatExec ignore this flag.
#define REO_REFEXIST         0x100 //Internal.

//REGROUP flags
#define REGF_ROOTITEM        0x001
#define REGF_ROOTANY         0x002
#define REGF_ROOTMULTILINE   0x004
#define REGF_ANY             0x008
#define REGF_AUTOGROUP       0x010
#define REGF_OR              0x020
#define REGF_POSITIVE        0x040
#define REGF_NEGATIVE        0x100
#define REGF_CHILDNOMAXMATCH 0x200

//PatCharCmp return value
#define RECC_EQUAL    0x01
#define RECC_DIF      0x02
#define RECC_MIX      0x04
#define RECC_BOUNDARY 0x08
#define RECC_REF      0x10

//PatEscChar return value
#define REEC_WRONG    70001
#define REEC_NEWLINE  70002
#define REEC_BOUNDARY 70003
#define REEC_REF      70004

//PatStructExec options
#define REPE_MATCHCASE        0x001 //Case-sensitive search.
#define REPE_MULTILINE        0x002 //Multiline search. Symbols ^ and $ specifies the line edge.
#define REPE_NOFIRSTLINEBEGIN 0x004 //PATEXEC.wpStr starts not from line beginning. Used with REPE_MULTILINE flag.
#define REPE_NOLASTLINEEND    0x008 //PATEXEC.wpMaxStr ends not on line ending. Used with REPE_MULTILINE flag.
#define REPE_GLOBAL           0x100 //Search all possible occurrences. If not specified then find only first occurrence.
#define REPE_ISMATCH          0x200 //Find first occurrence that should located at the beginning of the string. Cannot be combined with REPE_GLOBAL.

//PatStructExec callback return value
#define REPEC_CONTINUE  0  //Find next match.
#define REPEC_STOP      -1 //Stop search.

#ifndef __AKELEDIT_H__
  #define AELB_R        5  //"\r" new line.
  #define AELB_N        6  //"\n" new line.
  #define AELB_RN       7  //"\r\n" new line.
  #define AELB_RRN      8  //"\r\r\n" new line.
#endif

typedef struct _REGROUP {
  struct _REGROUP *next;
  struct _REGROUP *prev;
  struct _REGROUP *parent;
  struct _REGROUP *firstChild;
  struct _REGROUP *lastChild;
  const wchar_t *wpPatStart;
  const wchar_t *wpPatEnd;
  const wchar_t *wpPatLeft;
  const wchar_t *wpPatRight;
  const wchar_t *wpStrStart;    //PatExec: Begin of matched string.
  const wchar_t *wpStrEnd;      //PatExec: End of matched string.
  #ifdef __AKELEDIT_H__
    AECHARINDEX ciStrStart;       //AE_PatExec: Begin of matched string.
    AECHARINDEX ciStrEnd;         //AE_PatExec: End of matched string.
  #else
    int nAlignA1;
    INT_PTR nAlignA2;
    int nAlignA3;
    int nAlignB1;
    INT_PTR nAlignB2;
    int nAlignB3;
  #endif
  INT_PTR nStrLen;              //Matched string length.
  int nMinMatch;                //Minimum group match.
  int nMaxMatch;                //Maximum group match, -1 if unlimited.
  DWORD dwFlags;                //See REGF_* defines.
  int nIndex;                   //Group index, -1 if not captured.
  UINT_PTR dwUserData;          //User data.
} REGROUP;

typedef struct {
  REGROUP *first;
  REGROUP *last;
  DWORD dwOptions;              //See REO_* defines.
  const wchar_t *wpDelim;       //List of delimiters. If NULL, default list will be used " \t\n".
  const wchar_t *wpMaxDelim;    //Pointer to the last character. If wpDelim is null-terminated, then wpMaxDelim is pointer to the NULL character.
  int nLastIndex;               //Last captured index.
} STACKREGROUP;


typedef int (CALLBACK *PATEXECCALLBACK)(void *pe, REGROUP *lpREGroupRoot, BOOL bMatched);
//pe              Pointer to a PATEXEC structure. The application specifies this value when it sends the AKD_PATEXEC message.
//lpREGroupRoot   Pointer to a first REGROUP structure in stack (root group).
//bMatched        TRUE  - lpREGroupRoot->wpStrStart and lpREGroupRoot->wpStrEnd are valid.
//                FALSE - pe->wpStr is valid.
//Return Value
// See REPEC_* defines.

typedef struct {
  STACKREGROUP *lpREGroupStack; //Groups stack. Must be zero if AKD_PATEXEC called for the first time.
  const wchar_t *wpPat;         //Pattern for process.
  const wchar_t *wpMaxPat;      //Pointer to the last character. If wpPat is null-terminated, then wpMaxPat is pointer to the NULL character.
  const wchar_t *wpStr;         //PatExec: String for process. If NULL, ciStr and ciMaxStr will be used.
  const wchar_t *wpMaxStr;      //PatExec: Pointer to the last character. If wpStr is null-terminated, then wpMaxStr is pointer to the NULL character.
  #ifdef __AKELEDIT_H__
    AECHARINDEX ciStr;            //AE_PatExec: First character for process. Used if wpStr is NULL.
    AECHARINDEX ciMaxStr;         //AE_PatExec: Last character at which processing is stopped.
  #else
    int nAlignA1;
    INT_PTR nAlignA2;
    int nAlignA3;
    int nAlignB1;
    INT_PTR nAlignB2;
    int nAlignB3;
  #endif
  DWORD dwOptions;              //See REPE_* defines.
  const wchar_t *wpDelim;       //List of delimiters. If NULL, default list will be used " \t\n".
  const wchar_t *wpMaxDelim;    //Pointer to the last character. If wpDelim is null-terminated, then wpMaxDelim is pointer to the NULL character.
  INT_PTR nErrorOffset;         //Contain wpPat offset, if error occurred during compile pattern.

  //Callback
  PATEXECCALLBACK lpCallback;   //Pointer to an PATEXECCALLBACK function.
  LPARAM lParam;                //Specifies an application-defined value.
} PATEXEC;

typedef struct {
  const wchar_t *wpPat;      //Pattern for process.
  const wchar_t *wpMaxPat;   //Pointer to the last character. If wpPat is null-terminated, then wpMaxPat is pointer to the NULL character.
  const wchar_t *wpStr;      //PatExec: String for process. If NULL, ciStr and ciMaxStr will be used.
  const wchar_t *wpMaxStr;   //PatExec: Pointer to the last character. If wpStr is null-terminated, then wpMaxStr is pointer to the NULL character.
  #ifdef __AKELEDIT_H__
    AECHARINDEX ciStr;         //AE_PatExec: First character for process. Used if wpStr is NULL.
    AECHARINDEX ciMaxStr;      //AE_PatExec: Last character at which processing is stopped.
  #else
    int nAlignA1;
    INT_PTR nAlignA2;
    int nAlignA3;
    int nAlignB1;
    INT_PTR nAlignB2;
    int nAlignB3;
  #endif
  const wchar_t *wpRep;      //String to replace with. Can be used "\n" or "\nn" - the n'th captured submatch.
  const wchar_t *wpMaxRep;   //Pointer to the last character. If wpRep is null-terminated, then wpMaxRep is pointer to the NULL character.
  DWORD dwOptions;           //See REPE_* defines.
  const wchar_t *wpDelim;    //List of delimiters. If NULL, default list will be used " \t\n".
  const wchar_t *wpMaxDelim; //Pointer to the last character. If wpDelim is null-terminated, then wpMaxDelim is pointer to the NULL character.
  const wchar_t *wpNewLine;  //New line string. If NULL, default will be used "\r\n".
  INT_PTR nErrorOffset;      //Contain wpPat offset, if error occurred during compile pattern.
  int nReplaceCount;         //Receives replace count number.
  const wchar_t *wpLeftStr;  //PatExec: First replace occurrence in string.
  const wchar_t *wpRightStr; //PatExec: Unmatched right part of string.
  #ifdef __AKELEDIT_H__
    AECHARINDEX ciLeftStr;     //AE_PatExec: First replace occurrence in string.
    AECHARINDEX ciRightStr;    //AE_PatExec: Unmatched right part of string.
  #else
    int nAlignC1;
    INT_PTR nAlignC2;
    int nAlignC3;
    int nAlignD1;
    INT_PTR nAlignD2;
    int nAlignD3;
  #endif
  wchar_t *wszResult;        //Buffer that received replace result. If NULL, AKD_PATREPLACE returns required buffer size in characters.
} PATREPLACE;

typedef struct {
  const wchar_t *wpRep;
  const wchar_t *wpMaxRep;
  const wchar_t *wpNewLine;
  const wchar_t *wpRightStr;
  #ifdef __AKELEDIT_H__
    AECHARINDEX ciRightStr;
  #else
    int nAlign1;
    INT_PTR nAlign2;
    int nAlign3;
  #endif
  wchar_t *wszBuf;
  wchar_t *wpBufCount;
} PATEXECPARAM;

typedef struct {
  STACKREGROUP *lpREGroupStack; //Groups stack. Filled by AKD_PATEXEC message.
  const wchar_t *wpStr;         //String for process. Can be used "$n" - the n'th captured submatch.
  const wchar_t *wpMaxStr;      //Pointer to the last character. If wpStr is null-terminated, then wpMaxStr is pointer to the NULL character.
  wchar_t *wszResult;           //Buffer that received convert result. If NULL, AKD_PATGROUPSTR returns required buffer size in characters.
} PATGROUPSTR;

INT_PTR PatCompile(STACKREGROUP *hStack, const wchar_t *wpPat, const wchar_t *wpMaxPat);
BOOL PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, const wchar_t *wpStr, const wchar_t *wpMaxStr);
int PatStrChar(const wchar_t *wpStr, const wchar_t *wpMaxStr, int *nChar);
int PatEscChar(const wchar_t **wppPat);
DWORD PatCharCmp(const wchar_t **wppPat, int nStrChar, BOOL bSensitive, wchar_t *wchPatChar);
BOOL PatIsCharDelim(int nChar, const wchar_t *wpDelim, const wchar_t *wpMaxDelim);
int PatRefIndex(const wchar_t **wppPat);
REGROUP* PatCloseAutoGroups(REGROUP *lpREGroupItem, const wchar_t *wpPat, BOOL *lpbGroupNextChars);
REGROUP* PatGetGroup(STACKREGROUP *hStack, int nIndex);
REGROUP* PatNextGroup(REGROUP *lpREGroupItem);
REGROUP* PatNextGroupNoChild(REGROUP *lpREGroupItem);
REGROUP* PatPrevGroup(REGROUP *lpREGroupItem);
INT_PTR PatGroupStr(PATGROUPSTR *pgs);
void PatFree(STACKREGROUP *hStack);

#ifdef __AKELEDIT_H__
  BOOL AE_PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, AECHARINDEX *ciInput, AECHARINDEX *ciMaxInput);
  int AE_PatStrChar(const AECHARINDEX *ciChar);
  AELINEDATA* AE_PatNextChar(AECHARINDEX *ciChar);
  REGROUP* AE_PatCharInGroup(STACKREGROUP *hStack, const AECHARINDEX *ciChar);
#endif

int PatStructExec(PATEXEC *pe);
void PatStructFree(PATEXEC *pe);
INT_PTR PatReplace(PATREPLACE *pr);
int CALLBACK PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched);
int CALLBACK AE_PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched);
#ifdef __AKELEDIT_H__
  INT_PTR AE_PatStrCopy(AECHARINDEX *ciStart, AECHARINDEX *ciEnd, wchar_t *wszTarget, const wchar_t *wpTargetMax);
#endif
#endif //_REGEXPFUNC_H_


#ifdef RE_FUNCTIONS
#define RE_FUNCTIONS_INCLUDED
INT_PTR PatCompile(STACKREGROUP *hStack, const wchar_t *wpPat, const wchar_t *wpMaxPat)
{
  REGROUP *lpREGroupItem;
  REGROUP *lpREGroupNew;
  REGROUP *lpREGroupOr;
  REGROUP *lpREGroupParent;
  const wchar_t *wpMinPat=wpPat;
  const wchar_t *wpClassStart=NULL;
  const wchar_t *wpClassEnd=NULL;
  const wchar_t *wpCharStart=NULL;
  const wchar_t *wpPatLeft;
  int nIndex=0;
  BOOL bGroupNextChars=FALSE;
  BOOL bClassOpen=FALSE;

  if (!hStack->wpDelim)
  {
    //Default delimiters list
    hStack->wpDelim=L" \t\n";
    hStack->wpMaxDelim=hStack->wpDelim + 3;
  }

  //Zero group is the all pattern
  if (!StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)NULL, (stack **)&lpREGroupItem, sizeof(REGROUP)))
  {
    if (*wpPat == L'^')
    {
      if (hStack->dwOptions & REO_MULTILINE)
        lpREGroupItem->dwFlags|=REGF_ROOTANY|REGF_ROOTMULTILINE;
      else
        ++wpPat;
    }
    else
      lpREGroupItem->dwFlags|=REGF_ROOTANY;
    lpREGroupItem->dwFlags|=REGF_ROOTITEM;

    lpREGroupItem->parent=NULL;
    lpREGroupItem->wpPatStart=wpPat;
    lpREGroupItem->wpPatEnd=wpMaxPat;
    lpREGroupItem->wpPatLeft=wpPat;
    lpREGroupItem->wpPatRight=wpMaxPat;
    lpREGroupItem->nIndex=0;
    lpREGroupItem->nMinMatch=1;
    lpREGroupItem->nMaxMatch=1;
  }

  while (wpPat < wpMaxPat)
  {
    if (bGroupNextChars)
    {
      if (*wpPat != L'|' &&
          *wpPat != L')' &&
          *wpPat != L'*' &&
          *wpPat != L'+' &&
          *wpPat != L'?' &&
          *wpPat != L'{')
      {
        //"(.*)456(789)0" -> group to "(.*)(456(789)0)".
        if (!StackInsertBefore((stack **)&lpREGroupItem->firstChild, (stack **)&lpREGroupItem->lastChild, (stack *)NULL, (stack **)&lpREGroupNew, sizeof(REGROUP)))
        {
          lpREGroupNew->parent=lpREGroupItem;
          lpREGroupNew->wpPatLeft=wpPat;
          lpREGroupNew->wpPatStart=wpPat;
          lpREGroupNew->nMinMatch=1;
          lpREGroupNew->nMaxMatch=1;
          lpREGroupNew->nIndex=-1;
          lpREGroupNew->dwFlags|=REGF_AUTOGROUP;

          lpREGroupItem=lpREGroupNew;
        }
        bGroupNextChars=FALSE;
      }
    }

    if (*wpPat == L'\\')
    {
      wpCharStart=wpPat;

      if (*++wpPat == L'x')
        wpPat+=2;
      else if (*wpPat == L'u')
        wpPat+=4;
      else if (*wpPat >= L'0' && *wpPat <= L'9')
      {
        if (PatRefIndex(&wpPat) > nIndex)
        {
          wpPat=wpCharStart;
          goto Error;
        }
        hStack->dwOptions|=REO_REFEXIST;
      }
      else ++wpPat;

      continue;
    }
    if (*wpPat == L']')
    {
      if (!bClassOpen)
        goto Error;
      bClassOpen=FALSE;
      wpClassEnd=++wpPat;
      continue;
    }
    if (bClassOpen)
    {
      if (*wpPat == L'[')
        goto Error;
      ++wpPat;
      continue;
    }
    //Class open
    if (*wpPat == L'[')
    {
      bClassOpen=TRUE;
      wpClassStart=wpPat++;
      if (*wpPat == L'-')
        goto Error;
      continue;
    }

    if (*wpPat == L'|')
    {
      //Close all REGF_AUTOGROUP
      lpREGroupItem=PatCloseAutoGroups(lpREGroupItem, wpPat, &bGroupNextChars);

      //100|ABC(200|300|400)
      if (lpREGroupItem->dwFlags & REGF_OR)
      {
        //300
        lpREGroupItem->wpPatEnd=wpPat;
        lpREGroupItem->wpPatRight=wpPat + 1;

        lpREGroupItem=lpREGroupItem->parent;
      }
      else if (!lpREGroupItem->firstChild || !(lpREGroupItem->firstChild->dwFlags & REGF_OR))
      {
        //100 or 200
        if (lpREGroupOr=(REGROUP *)GlobalAlloc(GPTR, sizeof(REGROUP)))
        {
          lpREGroupOr->parent=lpREGroupItem;
          lpREGroupOr->wpPatLeft=lpREGroupItem->wpPatStart;
          lpREGroupOr->wpPatStart=lpREGroupItem->wpPatStart;
          lpREGroupOr->wpPatEnd=wpPat;
          lpREGroupOr->wpPatRight=wpPat + 1;
          lpREGroupOr->nMinMatch=1;
          lpREGroupOr->nMaxMatch=1;
          lpREGroupOr->nIndex=-1;
          lpREGroupOr->dwFlags|=REGF_OR;

          //Change parent for childrens
          for (lpREGroupNew=lpREGroupItem->firstChild; lpREGroupNew; lpREGroupNew=lpREGroupNew->next)
          {
            lpREGroupNew->parent=lpREGroupOr;
          }

          //Change childrens for parent
          lpREGroupOr->firstChild=lpREGroupItem->firstChild;
          lpREGroupOr->lastChild=lpREGroupItem->lastChild;
          lpREGroupItem->firstChild=lpREGroupOr;
          lpREGroupItem->lastChild=lpREGroupOr;
        }
      }

      if (!StackInsertBefore((stack **)&lpREGroupItem->firstChild, (stack **)&lpREGroupItem->lastChild, (stack *)NULL, (stack **)&lpREGroupNew, sizeof(REGROUP)))
      {
        lpREGroupNew->parent=lpREGroupItem;
        lpREGroupNew->wpPatLeft=++wpPat;
        lpREGroupNew->wpPatStart=wpPat;
        lpREGroupNew->nMinMatch=1;
        lpREGroupNew->nMaxMatch=1;
        lpREGroupNew->nIndex=-1;
        lpREGroupNew->dwFlags|=REGF_OR;

        lpREGroupItem=lpREGroupNew;
      }
      continue;
    }
    else if (*wpPat == L')')
    {
      if (!lpREGroupItem->parent || lpREGroupItem->wpPatEnd)
        goto Error;

      //Close all REGF_AUTOGROUP
      lpREGroupItem=PatCloseAutoGroups(lpREGroupItem, wpPat, &bGroupNextChars);

      if (lpREGroupItem->dwFlags & REGF_OR)
      {
        lpREGroupItem->wpPatEnd=wpPat;
        lpREGroupItem->wpPatRight=wpPat;

        lpREGroupItem=lpREGroupItem->parent;
      }

      lpREGroupItem->wpPatEnd=wpPat++;
      lpREGroupItem->wpPatRight=wpPat;
      lpREGroupItem->nMinMatch=1;
      lpREGroupItem->nMaxMatch=1;

      lpREGroupItem=lpREGroupItem->parent;
      continue;
    }
    //Multipliers
    else if (*wpPat == L'*' ||
             *wpPat == L'+' ||
             *wpPat == L'?' ||
             *wpPat == L'{')
    {
      if (lpREGroupItem->lastChild && wpPat == lpREGroupItem->lastChild->wpPatEnd + 1 && *lpREGroupItem->lastChild->wpPatEnd == L')')
      {
        //(...)* or (...)+ or (...){1,1}
        lpREGroupNew=lpREGroupItem->lastChild;
      }
      else if (wpPat == wpClassEnd)
      {
        //[...]* or [...]+ or [...]{1,1}
        if (!StackInsertBefore((stack **)&lpREGroupItem->firstChild, (stack **)&lpREGroupItem->lastChild, (stack *)NULL, (stack **)&lpREGroupNew, sizeof(REGROUP)))
        {
          lpREGroupNew->parent=lpREGroupItem;
          lpREGroupNew->wpPatLeft=wpClassStart;
          lpREGroupNew->wpPatStart=wpClassStart;
          lpREGroupNew->wpPatEnd=wpClassEnd;
          lpREGroupNew->nIndex=-1;
        }
      }
      else if (wpCharStart)
      {
        //\d* or \d+ or \d{1,1}
        if (!StackInsertBefore((stack **)&lpREGroupItem->firstChild, (stack **)&lpREGroupItem->lastChild, (stack *)NULL, (stack **)&lpREGroupNew, sizeof(REGROUP)))
        {
          lpREGroupNew->parent=lpREGroupItem;
          lpREGroupNew->wpPatLeft=wpCharStart;
          lpREGroupNew->wpPatStart=wpCharStart;
          lpREGroupNew->wpPatEnd=wpPat;
          lpREGroupNew->nIndex=-1;

          if (lpREGroupNew->wpPatEnd - lpREGroupNew->wpPatStart == 1 && *lpREGroupNew->wpPatStart == L'.')
            lpREGroupNew->dwFlags|=REGF_ANY;
        }
      }
      else goto Error;

      if (*wpPat == L'*')
      {
        lpREGroupNew->nMinMatch=0;
        lpREGroupNew->nMaxMatch=-1;
      }
      else if (*wpPat == L'+')
      {
        lpREGroupNew->nMinMatch=1;
        lpREGroupNew->nMaxMatch=-1;
      }
      else if (*wpPat == L'?')
      {
        lpREGroupNew->nMinMatch=0;
        lpREGroupNew->nMaxMatch=1;
      }
      else if (*wpPat == L'{')
      {
        lpREGroupNew->nMinMatch=(int)xatoiW(++wpPat, &wpPat);
        if (*wpPat == L'}')
          lpREGroupNew->nMaxMatch=lpREGroupNew->nMinMatch;
        else if (*wpPat == L',')
        {
          if (*++wpPat == L'}')
            lpREGroupNew->nMaxMatch=-1;
          else
            lpREGroupNew->nMaxMatch=(int)xatoiW(wpPat, &wpPat);
        }
        else goto Error;
      }
      if ((DWORD)lpREGroupNew->nMaxMatch > 1 &&
          ((lpREGroupNew->wpPatEnd - 1 >= lpREGroupNew->wpPatStart && (*(lpREGroupNew->wpPatEnd - 1) == L'$' || *(lpREGroupNew->wpPatEnd - 1) == L'^')) ||
           (lpREGroupNew->wpPatEnd - 2 >= lpREGroupNew->wpPatStart && *(lpREGroupNew->wpPatEnd - 2) == L'\\' && *(lpREGroupNew->wpPatEnd - 1) == L'b')))
        goto Error;
      if (lpREGroupNew->nMaxMatch == -1)
      {
        bGroupNextChars=TRUE;
        //Mark all parents if lpREGroupNew is the first item in group
        wpPatLeft=lpREGroupNew->wpPatLeft;
        lpREGroupNew->dwFlags|=REGF_CHILDNOMAXMATCH;

        for (lpREGroupParent=lpREGroupNew->parent; lpREGroupParent; lpREGroupParent=lpREGroupParent->parent)
        {
          if (wpPatLeft == lpREGroupParent->wpPatStart)
          {
            lpREGroupParent->dwFlags|=REGF_CHILDNOMAXMATCH;
            wpPatLeft=lpREGroupParent->wpPatLeft;
          }
          else break;
        }
      }
      wpCharStart=NULL;

      //We already non-greedy, so ignore it.
      if (*++wpPat == L'?') ++wpPat;
      lpREGroupNew->wpPatRight=wpPat;
      continue;
    }
    //Group open
    else if (*wpPat == L'(')
    {
      if (!StackInsertBefore((stack **)&lpREGroupItem->firstChild, (stack **)&lpREGroupItem->lastChild, (stack *)NULL, (stack **)&lpREGroupNew, sizeof(REGROUP)))
      {
        lpREGroupNew->parent=lpREGroupItem;
        lpREGroupNew->wpPatLeft=wpPat;
        lpREGroupNew->wpPatStart=++wpPat;
        lpREGroupNew->nIndex=++nIndex;

        if (*wpPat == L'?')
        {
          if (*++wpPat == L':')
          {
            //Don't assign index
          }
          else if (*wpPat == L'=')
          {
            //Non-capture positive group
            lpREGroupNew->dwFlags|=REGF_POSITIVE;
          }
          else if (*wpPat == L'!')
          {
            //Non-capture negative group
            lpREGroupNew->dwFlags|=REGF_NEGATIVE;
          }
          else goto Error;

          --nIndex;
          lpREGroupNew->nIndex=-1;
          lpREGroupNew->wpPatStart=++wpPat;
        }
        lpREGroupItem=lpREGroupNew;
        continue;
      }
    }
    else wpCharStart=wpPat;

    ++wpPat;
  }
  if (wpClassStart > wpClassEnd)
  {
    wpPat=wpClassStart;
    goto Error;
  }

  //Close all REGF_AUTOGROUP
  lpREGroupItem=PatCloseAutoGroups(lpREGroupItem, wpPat, NULL);

  if (lpREGroupItem->dwFlags & REGF_OR)
  {
    lpREGroupItem->wpPatEnd=wpPat;
    lpREGroupItem->wpPatRight=wpPat;

    lpREGroupItem=lpREGroupItem->parent;
  }
  if (!lpREGroupItem->wpPatEnd)
    goto Error;
  hStack->nLastIndex=nIndex;
  return 0;

  Error:
  PatFree(hStack);
  return (wpPat - wpMinPat) + 1;
}

BOOL PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, const wchar_t *wpStr, const wchar_t *wpMaxStr)
{
  REGROUP *lpREGroupNext;
  REGROUP *lpREGroupNextNext;
  REGROUP *lpREGroupRef;
  const wchar_t *wpPat;
  const wchar_t *wpMaxPat=lpREGroupItem->wpPatEnd;
  const wchar_t *wpMinStr=wpStr;
  const wchar_t *wpStrStart;
  const wchar_t *wpNextGroup;
  int nStrChar;
  wchar_t wchPatChar;
  wchar_t wchPatNextChar;
  wchar_t wchCaseChar;
  int nCurMatch=0;
  int nRefIndex;
  INT_PTR nRefLen;
  DWORD dwCmpResult=0;
  BOOL bExclude;

  if (lpREGroupItem->dwFlags & REGF_ROOTITEM)
  {
    lpREGroupItem->wpStrStart=wpStr;
    lpREGroupItem->wpStrEnd=wpStr;
    lpREGroupItem->nStrLen=0;

    if (lpREGroupItem->dwFlags & REGF_ROOTANY)
    {
      BOOL bResult=FALSE;

      //Turn off REGF_ROOTANY and execute itself
      lpREGroupItem->dwFlags&=~REGF_ROOTITEM & ~REGF_ROOTANY;

      while (wpStr <= wpMaxStr)
      {
        if (PatExec(hStack, lpREGroupItem, wpStr, wpMaxStr))
        {
          bResult=TRUE;
          break;
        }
        if (lpREGroupItem->dwFlags & REGF_ROOTMULTILINE)
        {
          while (*wpStr != L'\r' && *wpStr != L'\n' && wpStr > wpMaxStr) ++wpStr;
          if (wpStr >= wpMaxStr) break;
        }
        wpStr+=PatStrChar(wpStr, wpMaxStr, &nStrChar) + 1;
      }
      lpREGroupItem->dwFlags|=REGF_ROOTITEM|REGF_ROOTANY;
      return bResult;
    }
  }

  BeginLoop:
  if ((DWORD)nCurMatch < (DWORD)lpREGroupItem->nMaxMatch)
  {
    wpStrStart=wpStr;
    wpPat=lpREGroupItem->wpPatStart;
    if (lpREGroupNext=lpREGroupItem->firstChild)
      wpNextGroup=lpREGroupNext->wpPatLeft;
    else
      wpNextGroup=NULL;

    for (;;)
    {
      if (wpPat >= wpMaxPat)
      {
        Match:
        lpREGroupItem->wpStrStart=wpStrStart;
        lpREGroupItem->wpStrEnd=wpStr;
        lpREGroupItem->nStrLen=wpStr - wpStrStart;
        ++nCurMatch;

        if (wpStr >= wpMaxStr)
          goto EndLoop;

        if (lpREGroupItem->nMaxMatch == -1 && nCurMatch >= lpREGroupItem->nMinMatch)
        {
          if ((lpREGroupNextNext=PatNextGroupNoChild(lpREGroupItem)) && !(lpREGroupNextNext->dwFlags & REGF_CHILDNOMAXMATCH))
          {
            if (hStack->dwOptions & REO_REFEXIST)
            {
              //Set wpStrEnd to all parent groups for possible backreferences in child PatExec.
              for (lpREGroupRef=lpREGroupItem->parent; lpREGroupRef; lpREGroupRef=lpREGroupRef->parent)
                lpREGroupRef->wpStrEnd=wpStr;
            }
            //Check nStrLen for \d+Z? in 123Z
            if (PatExec(hStack, lpREGroupNextNext, wpStr, wpMaxStr) && lpREGroupNextNext->nStrLen)
              goto EndLoop;
          }
        }
        goto BeginLoop;
      }
      if (wpPat == wpNextGroup)
      {
        if (lpREGroupNext->dwFlags & REGF_OR)
        {
          bExclude=FALSE;

          for (;;)
          {
            if (!bExclude)
            {
              if (PatExec(hStack, lpREGroupNext, wpStr, wpMaxStr))
              {
                wpStr=lpREGroupNext->wpStrEnd;
                bExclude=TRUE;
              }
            }
            if (lpREGroupNext->next && (lpREGroupNext->next->dwFlags & REGF_OR))
              lpREGroupNext=lpREGroupNext->next;
            else
              break;
          }
          if (!bExclude) goto EndLoop;
        }
        else
        {
          if (hStack->dwOptions & REO_REFEXIST)
          {
            //Set wpStrStart for possible backreferences in child PatExec.
            lpREGroupItem->wpStrStart=wpStrStart;
          }

          if (!lpREGroupNext->nMinMatch)
          {
            if ((lpREGroupNextNext=PatNextGroupNoChild(lpREGroupNext)) && !(lpREGroupNextNext->dwFlags & REGF_CHILDNOMAXMATCH))
            {
              if (PatExec(hStack, lpREGroupNextNext, wpStr, wpMaxStr))
              {
                if (lpREGroupNext->parent == lpREGroupNextNext->parent)
                {
                  lpREGroupNext=lpREGroupNextNext;
                  wpStr=lpREGroupNext->wpStrEnd;
                }
                goto NextGroup;
              }
            }
          }
          if (!PatExec(hStack, lpREGroupNext, wpStr, wpMaxStr))
            goto EndLoop;
          wpStr=lpREGroupNext->wpStrEnd;
        }

        NextGroup:
        wpPat=lpREGroupNext->wpPatRight;

        if (lpREGroupNext=lpREGroupNext->next)
          wpNextGroup=lpREGroupNext->wpPatLeft;
        else
          wpNextGroup=NULL;
        continue;
      }
      if (*wpPat == L'$')
      {
        if ((!(hStack->dwOptions & REO_NOLASTLINEEND) && wpStr == wpMaxStr) ||
            ((hStack->dwOptions & REO_MULTILINE) && (*wpStr == L'\n' || *wpStr == L'\r')))
        {
          ++wpPat;
          continue;
        }
        if (wpPat + 1 == wpMaxPat)
          goto EndLoop;
      }
      if (wpStr >= wpMaxStr)
      {
        if (wpPat + 2 == wpMaxPat && *wpPat == L'\\' && *(wpPat + 1) == L'b')
          goto Match;
        goto EndLoop;
      }

      if (*wpPat == L'.')
      {
        ////Any character except new line
        //wpStr+=PatStrChar(wpStr, wpMaxStr, &nStrChar);
        //if (nStrChar < 0) goto EndLoop;
      }
      else if (*wpPat == L'^')
      {
        //REO_MULTILINE
        if ((!(hStack->dwOptions & REO_NOFIRSTLINEBEGIN) && wpStr == hStack->first->wpStrStart) ||
            (wpStr > hStack->first->wpStrStart && (*(wpStr - 1) == L'\n' || *(wpStr - 1) == L'\r')))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }
      else if (*wpPat == L'[')
      {
        wpStr+=PatStrChar(wpStr, wpMaxStr, &nStrChar);

        if (*++wpPat == L'^')
        {
          bExclude=TRUE;
          ++wpPat;
        }
        else bExclude=FALSE;

        for (; *wpPat != L']'; ++wpPat)
        {
          if (*wpPat == L'-')
          {
            if (!(dwCmpResult & RECC_MIX))
            {
              ++wpPat;
              dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &wchPatNextChar);
              if (dwCmpResult & RECC_EQUAL)
              {
                if (bExclude) goto EndLoop;
                break;
              }
              if (nStrChar < 0) nStrChar=L'\n';

              //Check range
              if (!(dwCmpResult & RECC_MIX))
              {
                if ((nStrChar >= wchPatChar && nStrChar <= wchPatNextChar) ||
                    (!(hStack->dwOptions & REO_MATCHCASE) &&
                      (((wchCaseChar=WideCharLower((wchar_t)nStrChar)) >= wchPatChar && wchCaseChar <= wchPatNextChar) ||
                       (wchCaseChar == nStrChar && (wchCaseChar=WideCharUpper((wchar_t)nStrChar)) >= wchPatChar && wchCaseChar <= wchPatNextChar))))
                {
                  if (bExclude) goto EndLoop;
                  break;
                }
              }
            }
          }
          else
          {
            dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &wchPatChar);
            if (dwCmpResult & RECC_EQUAL)
            {
              if (bExclude) goto EndLoop;
              break;
            }
          }
        }

        if (*wpPat == L']')
        {
          if (!bExclude) goto EndLoop;
        }
        else while (*++wpPat != L']');
      }
      else
      {
        wpStr+=PatStrChar(wpStr, wpMaxStr, &nStrChar);
        dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &wchPatChar);

        if (!(dwCmpResult & RECC_EQUAL))
        {
          if (dwCmpResult & RECC_DIF)
            goto EndLoop;

          if (dwCmpResult & RECC_BOUNDARY)
          {
            if (PatIsCharDelim(nStrChar, hStack->wpDelim, hStack->wpMaxDelim))
            {
              if (wpStr > hStack->first->wpStrStart)
              {
                if (*wpPat == L'b')
                {
                  ++wpPat;
                  continue;
                }
              }
              else if (*wpPat == L'B')
              {
                ++wpPat;
                continue;
              }
              goto EndLoop;
            }
            else
            {
              if (wpStr - 1 > hStack->first->wpStrStart >= 0)
              {
                nStrChar=*(wpStr - 1);
                if (nStrChar == L'\r') nStrChar=L'\n';
              }
              if (wpStr - 1 < hStack->first->wpStrStart || PatIsCharDelim(nStrChar, hStack->wpDelim, hStack->wpMaxDelim))
              {
                if (*wpPat == L'b')
                {
                  ++wpPat;
                  continue;
                }
              }
              else if (*wpPat == L'B')
              {
                ++wpPat;
                continue;
              }
              goto EndLoop;
            }
          }
          else if (dwCmpResult & RECC_REF)
          {
            nRefIndex=(int)xatoiW(wpPat, &wpPat);

            if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
            {
              nRefLen=lpREGroupRef->wpStrEnd - lpREGroupRef->wpStrStart;
              if (nRefLen && ((hStack->dwOptions & REO_MATCHCASE)?
                               (!xstrcmpnW(lpREGroupRef->wpStrStart, wpStr, nRefLen)) :
                               (!xstrcmpinW(lpREGroupRef->wpStrStart, wpStr, nRefLen))))
                wpStr+=nRefLen;
              else
                goto EndLoop;
            }
            else goto EndLoop;

            continue;
          }
        }
      }
      ++wpStr;
      ++wpPat;
    }
    EndLoop:
    if (nCurMatch < lpREGroupItem->nMinMatch)
    {
      if (lpREGroupItem->dwFlags & REGF_NEGATIVE)
      {
        lpREGroupItem->wpStrStart=wpMinStr;
        lpREGroupItem->wpStrEnd=wpMinStr;
        lpREGroupItem->nStrLen=0;
        return TRUE;
      }
      return FALSE;
    }
    if (!nCurMatch)
    {
      lpREGroupItem->wpStrStart=wpStrStart;
      lpREGroupItem->wpStrEnd=wpStrStart;
      lpREGroupItem->nStrLen=0;
    }
  }
  if (lpREGroupItem->dwFlags & REGF_NEGATIVE)
  {
    //lpREGroupItem->wpStrStart=wpMinStr;
    //lpREGroupItem->wpStrEnd=wpMinStr;
    //lpREGroupItem->nStrLen=0;
    return FALSE;
  }
  if (lpREGroupItem->dwFlags & REGF_POSITIVE)
  {
    lpREGroupItem->wpStrStart=wpMinStr;
    lpREGroupItem->wpStrEnd=wpMinStr;
    lpREGroupItem->nStrLen=0;
  }
  return TRUE;
}

int PatStrChar(const wchar_t *wpStr, const wchar_t *wpMaxStr, int *nChar)
{
  if (*wpStr == L'\n')
  {
    *nChar=-AELB_N;
    return 0;
  }
  else if (*wpStr == L'\r')
  {
    if (wpStr + 1 < wpMaxStr && *(wpStr + 1) == L'\n')
    {
      *nChar=-AELB_RN;
      return 1;
    }
    else if (wpStr + 2 < wpMaxStr && *(wpStr + 1) == L'\r' && *(wpStr + 2) == L'\n')
    {
      *nChar=-AELB_RRN;
      return 2;
    }
    else
    {
      *nChar=-AELB_R;
      return 0;
    }
  }
  *nChar=*wpStr;
  return 0;
}

int PatEscChar(const wchar_t **wppPat)
{
  int nPatChar=**wppPat;

  if (nPatChar == L'\\')
  {
    ++(*wppPat);
    nPatChar=**wppPat;

    if (nPatChar == L'u')
    {
      nPatChar=(int)hex2decW(++(*wppPat), 4);
      (*wppPat)+=3;
    }
    else if (nPatChar == L'x')
    {
      nPatChar=(int)hex2decW(++(*wppPat), 2);
      (*wppPat)+=1;
    }
    else if (nPatChar == L'r' || nPatChar == L'n')
      return REEC_NEWLINE;
    else if (nPatChar == L't')
      return L'\t';
    else if (nPatChar == L'f')
      return L'\f';
    else if (nPatChar == L'v')
      return L'\v';
    else if (nPatChar == L'b' || nPatChar == L'B')
      return REEC_BOUNDARY;
    else if (nPatChar >= L'0' && nPatChar <= L'9')
      return REEC_REF;
    else if (nPatChar == L'd' ||
             nPatChar == L'D' ||
             nPatChar == L's' ||
             nPatChar == L'S' ||
             nPatChar == L'w' ||
             nPatChar == L'W')
    {
      return REEC_WRONG;
    }
  }
  return nPatChar;
}

DWORD PatCharCmp(const wchar_t **wppPat, int nStrChar, BOOL bSensitive, wchar_t *wchPatChar)
{
  int nPatChar;

  //AELB_R, AELB_N, AELB_RN, AELB_RRN
  if (nStrChar < 0)
  {
    if ((nPatChar=PatEscChar(wppPat)) < REEC_WRONG)
    {
      if (nPatChar == L'\n')
      {
        *wchPatChar=L'\n';

        if (nStrChar == -AELB_N)
          return RECC_EQUAL;
      }
      else if (nPatChar == L'\r')
      {
        *wchPatChar=L'\n';

        if (nStrChar == -AELB_R)
        {
          return RECC_EQUAL;
        }
        else if (nStrChar == -AELB_RN)
        {
          ++(*wppPat);
          if (PatEscChar(wppPat) == L'\n')
            return RECC_EQUAL;
        }
        else if (nStrChar == -AELB_RRN)
        {
          ++(*wppPat);
          if (PatEscChar(wppPat) == L'\r')
          {
            ++(*wppPat);
            if (PatEscChar(wppPat) == L'\n')
              return RECC_EQUAL;
          }
        }
      }
      else *wchPatChar=(wchar_t)nPatChar;
    }
    else if (nPatChar == REEC_NEWLINE)
    {
      *wchPatChar=L'\n';
      return RECC_EQUAL;
    }
    else
    {
      *wchPatChar=L'\0';
      if (nPatChar == REEC_BOUNDARY)
        return RECC_BOUNDARY|RECC_MIX;
      if (nPatChar == REEC_REF)
        return RECC_REF|RECC_MIX;
    }
    return RECC_DIF;
  }

  if ((nPatChar=PatEscChar(wppPat)) == REEC_WRONG)
  {
    nPatChar=**wppPat;

    if (nPatChar == L'd')
    {
      if (nStrChar < L'0' || nStrChar > L'9')
        return RECC_DIF|RECC_MIX;
    }
    else if (nPatChar == L'D')
    {
      if (nStrChar >= L'0' && nStrChar <= L'9')
        return RECC_DIF|RECC_MIX;
    }
    else if (nPatChar == L's')
    {
      if (nStrChar != L' ' && nStrChar != L'\f' && nStrChar != L'\n' && nStrChar != L'\r' && nStrChar != L'\t' && nStrChar != L'\v')
        return RECC_DIF|RECC_MIX;
    }
    else if (nPatChar == L'S')
    {
      if (nStrChar == L' ' || nStrChar == L'\f' || nStrChar == L'\n' || nStrChar == L'\r' || nStrChar == L'\t' || nStrChar == L'\v')
        return RECC_DIF|RECC_MIX;
    }
    else if (nPatChar == L'w')
    {
      if ((nStrChar < L'A' || nStrChar > L'Z') && (nStrChar < L'a' || nStrChar > L'z') && (nStrChar < L'0' || nStrChar > L'9') && nStrChar != L'_')
        return RECC_DIF|RECC_MIX;
    }
    else if (nPatChar == L'W')
    {
      if ((nStrChar >= L'A' && nStrChar <= L'Z') || (nStrChar >= L'a' && nStrChar <= L'z') || (nStrChar >= L'0' && nStrChar <= L'9') || nStrChar == L'_')
        return RECC_DIF|RECC_MIX;
    }
    return RECC_EQUAL|RECC_MIX;
  }
  else if (nPatChar == REEC_NEWLINE)
  {
    *wchPatChar=L'\n';
    return RECC_DIF;
  }
  else if (nPatChar == REEC_BOUNDARY)
  {
    return RECC_BOUNDARY|RECC_MIX;
  }
  else if (nPatChar == REEC_REF)
  {
    return RECC_REF|RECC_MIX;
  }

  //Compare
  *wchPatChar=(wchar_t)nPatChar;
  if (nStrChar != nPatChar && (bSensitive || WideCharLower((wchar_t)nStrChar) != WideCharLower((wchar_t)nPatChar)))
    return RECC_DIF;
  return RECC_EQUAL;
}

BOOL PatIsCharDelim(int nChar, const wchar_t *wpDelim, const wchar_t *wpMaxDelim)
{
  if (nChar < 0) nChar=L'\n';

  for (; wpDelim < wpMaxDelim; ++wpDelim)
  {
    if (*wpDelim == nChar)
      return TRUE;
  }
  return FALSE;
}

int PatRefIndex(const wchar_t **wppPat)
{
  const wchar_t *wpPat=*wppPat;
  wchar_t wszIndex[3];
  int nIndex=-1;

  if (*wpPat >= L'0' && *wpPat <= L'9')
  {
    wszIndex[0]=*wpPat++;
    wszIndex[1]=L'\0';
    if (*wpPat >= L'0' && *wpPat <= L'9')
    {
      wszIndex[1]=*wpPat++;
      wszIndex[2]=L'\0';
    }
    nIndex=(int)xatoiW(wszIndex, NULL);
    *wppPat=wpPat;
  }
  return nIndex;
}

REGROUP* PatCloseAutoGroups(REGROUP *lpREGroupItem, const wchar_t *wpPat, BOOL *lpbGroupNextChars)
{
  REGROUP *lpREGroupChild;
  REGROUP *lpREGroupParent;
  const wchar_t *wpPatChild;

  do
  {
    if (lpREGroupItem->dwFlags & REGF_AUTOGROUP)
    {
      if (lpbGroupNextChars) *lpbGroupNextChars=FALSE;

      if (lpREGroupItem->firstChild)
      {
        //If only one children and pattern the same, then remove redundant grouping
        if (lpREGroupItem->firstChild == lpREGroupItem->lastChild)
        {
          lpREGroupChild=lpREGroupItem->firstChild;

          if (lpREGroupChild->wpPatLeft == lpREGroupItem->wpPatLeft &&
              lpREGroupChild->wpPatRight == wpPat)
          {
            lpREGroupParent=lpREGroupItem->parent;

            //Replace lpREGroupItem with lpREGroupChild
            StackJoin((stack **)&lpREGroupParent->firstChild, (stack **)&lpREGroupParent->lastChild, (stack *)lpREGroupItem, (stack *)lpREGroupChild, (stack *)lpREGroupChild);
            StackDelete((stack **)&lpREGroupParent->firstChild, (stack **)&lpREGroupParent->lastChild, (stack *)lpREGroupItem);
            lpREGroupChild->parent=lpREGroupParent;
            lpREGroupItem=lpREGroupChild;
            continue;
          }
        }

        //If nMinMatch of all children is zero, then set nMinMatch to zero
        wpPatChild=lpREGroupItem->wpPatLeft;

        for (lpREGroupChild=lpREGroupItem->firstChild; lpREGroupChild; lpREGroupChild=lpREGroupChild->next)
        {
          if (lpREGroupChild->nMinMatch) break;
          if (lpREGroupChild->wpPatLeft != wpPatChild) break;
          wpPatChild=lpREGroupChild->wpPatRight;
        }
        if (!lpREGroupChild && wpPat == wpPatChild)
          lpREGroupItem->nMinMatch=0;
      }
      lpREGroupItem->wpPatEnd=wpPat;
      lpREGroupItem->wpPatRight=wpPat;
    }
    else break;
  }
  while (lpREGroupItem=lpREGroupItem->parent);

  return lpREGroupItem;
}

REGROUP* PatGetGroup(STACKREGROUP *hStack, int nIndex)
{
  REGROUP *lpREGroupItem;

  if (nIndex > hStack->nLastIndex)
    return NULL;

  for (lpREGroupItem=hStack->first; lpREGroupItem; lpREGroupItem=PatNextGroup(lpREGroupItem))
  {
    if (lpREGroupItem->nIndex == nIndex)
      break;
  }
  return lpREGroupItem;
}

REGROUP* PatNextGroup(REGROUP *lpREGroupItem)
{
  if (lpREGroupItem->firstChild)
    return lpREGroupItem->firstChild;

  do
  {
    if (lpREGroupItem->next)
      return lpREGroupItem->next;
  }
  while (lpREGroupItem=lpREGroupItem->parent);

  return lpREGroupItem;
}

REGROUP* PatNextGroupNoChild(REGROUP *lpREGroupItem)
{
  do
  {
    if (lpREGroupItem->next)
      return lpREGroupItem->next;
  }
  while (lpREGroupItem=lpREGroupItem->parent);

  return lpREGroupItem;
}

REGROUP* PatPrevGroup(REGROUP *lpREGroupItem)
{
  if (lpREGroupItem->lastChild)
    return lpREGroupItem->lastChild;

  do
  {
    if (lpREGroupItem->prev)
      return lpREGroupItem->prev;
  }
  while (lpREGroupItem=lpREGroupItem->parent);

  return lpREGroupItem;
}

INT_PTR PatGroupStr(PATGROUPSTR *pgs)
{
  REGROUP *lpREGroupRef;
  const wchar_t *wpStr=pgs->wpStr;
  wchar_t *wpBufCount=pgs->wszResult;

  while (wpStr < pgs->wpMaxStr)
  {
    if (*wpStr == L'$')
    {
      if (lpREGroupRef=PatGetGroup(pgs->lpREGroupStack, (int)xatoiW(++wpStr, &wpStr)))
      {
        if (pgs->wszResult)
          xmemcpy(wpBufCount, lpREGroupRef->wpStrStart, (lpREGroupRef->wpStrEnd - lpREGroupRef->wpStrStart) * sizeof(wchar_t));
        wpBufCount+=lpREGroupRef->wpStrEnd - lpREGroupRef->wpStrStart;
      }
    }
    else
    {
      if (pgs->wszResult)
        *wpBufCount=*wpStr;
      ++wpBufCount;
      ++wpStr;
    }
  }
  if (pgs->wszResult)
    *wpBufCount=L'\0';
  else
    ++wpBufCount;
  return (wpBufCount - pgs->wszResult);
}

void PatFree(STACKREGROUP *hStack)
{
  REGROUP *lpParent=NULL;
  REGROUP *lpSubling=hStack->first;
  REGROUP *lpNextSubling;

  while (lpSubling)
  {
    NextParent:
    if (lpSubling->firstChild)
    {
      lpSubling=lpSubling->firstChild;
      continue;
    }

    //Group doesn't have childrens
    lpParent=lpSubling->parent;
    lpNextSubling=lpSubling->next;

    //Delete group
    if (!lpParent)
      StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpSubling);
    else
      StackDelete((stack **)&lpParent->firstChild, (stack **)&lpParent->lastChild, (stack *)lpSubling);

    lpSubling=lpNextSubling;
  }
  if (lpParent)
  {
    lpSubling=lpParent;
    goto NextParent;
  }
}

#if defined __AKELEDIT_H__
BOOL AE_PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, AECHARINDEX *ciInput, AECHARINDEX *ciMaxInput)
{
  REGROUP *lpREGroupNext;
  REGROUP *lpREGroupNextNext;
  REGROUP *lpREGroupRef;
  AECHARINDEX ciStr=*ciInput;
  AECHARINDEX ciMinStr=*ciInput;
  AECHARINDEX ciMaxStr=*ciMaxInput;
  AECHARINDEX ciStrStart;
  AECHARINDEX ciStrCount;
  AECHARINDEX ciGroupCount;
  const wchar_t *wpPat;
  const wchar_t *wpMaxPat=lpREGroupItem->wpPatEnd;
  const wchar_t *wpNextGroup;
  INT_PTR nStrLen=0;
  INT_PTR nCount;
  int nStrChar;
  wchar_t wchPatChar;
  wchar_t wchPatNextChar;
  int nGroupChar;
  wchar_t wchCaseChar;
  int nCurMatch=0;
  int nRefIndex;
  DWORD dwCmpResult=0;
  BOOL bExclude;

  if (lpREGroupItem->dwFlags & REGF_ROOTITEM)
  {
    lpREGroupItem->ciStrStart=ciStr;
    lpREGroupItem->ciStrEnd=ciStr;
    lpREGroupItem->nStrLen=0;

    if (lpREGroupItem->dwFlags & REGF_ROOTANY)
    {
      BOOL bResult=FALSE;

      //Turn off REGF_ROOTANY and execute itself
      lpREGroupItem->dwFlags&=~REGF_ROOTITEM & ~REGF_ROOTANY;

      while (AEC_IndexCompare(&ciStr, &ciMaxStr) <= 0)
      {
        if (AE_PatExec(hStack, lpREGroupItem, &ciStr, &ciMaxStr))
        {
          bResult=TRUE;
          break;
        }
        if (lpREGroupItem->dwFlags & REGF_ROOTMULTILINE)
          AEC_NextLine(&ciStr);
        else
          AE_PatNextChar(&ciStr);
      }
      lpREGroupItem->dwFlags|=REGF_ROOTITEM|REGF_ROOTANY;
      return bResult;
    }
  }

  BeginLoop:
  if ((DWORD)nCurMatch < (DWORD)lpREGroupItem->nMaxMatch)
  {
    ciStrStart=ciStr;
    wpPat=lpREGroupItem->wpPatStart;
    if (lpREGroupNext=lpREGroupItem->firstChild)
      wpNextGroup=lpREGroupNext->wpPatLeft;
    else
      wpNextGroup=NULL;

    for (;;)
    {
      if (wpPat >= wpMaxPat)
      {
        Match:
        lpREGroupItem->ciStrStart=ciStrStart;
        lpREGroupItem->ciStrEnd=ciStr;
        lpREGroupItem->nStrLen=nStrLen;
        ++nCurMatch;

        if (AEC_IndexCompare(&ciStr, &ciMaxStr) >= 0)
          goto EndLoop;

        if (lpREGroupItem->nMaxMatch == -1 && nCurMatch >= lpREGroupItem->nMinMatch)
        {
          if ((lpREGroupNextNext=PatNextGroupNoChild(lpREGroupItem)) && !(lpREGroupNextNext->dwFlags & REGF_CHILDNOMAXMATCH))
          {
            if (hStack->dwOptions & REO_REFEXIST)
            {
              //Set ciStrEnd to all parent groups for possible backreferences in child AE_PatExec.
              for (lpREGroupRef=lpREGroupItem->parent; lpREGroupRef; lpREGroupRef=lpREGroupRef->parent)
                lpREGroupRef->ciStrEnd=ciStr;
            }
            //Check nStrLen for \d+Z? in 123Z
            if (AE_PatExec(hStack, lpREGroupNextNext, &ciStr, &ciMaxStr) && lpREGroupNextNext->nStrLen)
              goto EndLoop;
          }
        }
        goto BeginLoop;
      }
      if (wpPat == wpNextGroup)
      {
        if (lpREGroupNext->dwFlags & REGF_OR)
        {
          bExclude=FALSE;

          for (;;)
          {
            if (!bExclude)
            {
              if (AE_PatExec(hStack, lpREGroupNext, &ciStr, &ciMaxStr))
              {
                ciStr=lpREGroupNext->ciStrEnd;
                nStrLen+=lpREGroupNext->nStrLen;
                bExclude=TRUE;
              }
            }
            if (lpREGroupNext->next && (lpREGroupNext->next->dwFlags & REGF_OR))
              lpREGroupNext=lpREGroupNext->next;
            else
              break;
          }
          if (!bExclude) goto EndLoop;
        }
        else
        {
          if (hStack->dwOptions & REO_REFEXIST)
          {
            //Set ciStrStart for possible backreferences in child AE_PatExec.
            lpREGroupItem->ciStrStart=ciStrStart;
          }

          if (!lpREGroupNext->nMinMatch)
          {
            if ((lpREGroupNextNext=PatNextGroupNoChild(lpREGroupNext)) && !(lpREGroupNextNext->dwFlags & REGF_CHILDNOMAXMATCH))
            {
              if (AE_PatExec(hStack, lpREGroupNextNext, &ciStr, &ciMaxStr))
              {
                if (lpREGroupNext->parent == lpREGroupNextNext->parent)
                {
                  lpREGroupNext=lpREGroupNextNext;
                  ciStr=lpREGroupNext->ciStrEnd;
                  nStrLen+=lpREGroupNext->nStrLen;
                }
                goto NextGroup;
              }
            }
          }
          if (!AE_PatExec(hStack, lpREGroupNext, &ciStr, &ciMaxStr))
            goto EndLoop;
          ciStr=lpREGroupNext->ciStrEnd;
          nStrLen+=lpREGroupNext->nStrLen;
        }

        NextGroup:
        wpPat=lpREGroupNext->wpPatRight;

        if (lpREGroupNext=lpREGroupNext->next)
          wpNextGroup=lpREGroupNext->wpPatLeft;
        else
          wpNextGroup=NULL;
        continue;
      }
      if (*wpPat == L'$')
      {
        if (hStack->dwOptions & REO_MULTILINE ? AEC_IsLastCharInLine(&ciStr) : !AEC_IndexCompare(&ciStr, &ciMaxStr))
        {
          ++wpPat;
          continue;
        }
        if (wpPat + 1 == wpMaxPat)
          goto EndLoop;
      }
      if (AEC_IndexCompare(&ciStr, &ciMaxStr) >= 0)
      {
        if (wpPat + 2 == wpMaxPat && *wpPat == L'\\' && (*(wpPat + 1) == L'b' || *(wpPat + 1) == L'B'))
        {
          if (PatIsCharDelim(AE_PatStrChar(&ciStr), hStack->wpDelim, hStack->wpMaxDelim))
          {
            if (*(wpPat + 1) == L'b')
              goto Match;
          }
          else if (*(wpPat + 1) == L'B')
            goto Match;
        }
        goto EndLoop;
      }

      if (*wpPat == L'.')
      {
        ////Any character except new line
        //nStrChar=AE_PatStrChar(&ciStr);
        //if (nStrChar < 0) goto EndLoop;
      }
      else if (*wpPat == L'^')
      {
        //REO_MULTILINE
        if (!AEC_IsFirstCharInLine(&ciStr))
          goto EndLoop;
        ++wpPat;
        continue;
      }
      else if (*wpPat == L'[')
      {
        nStrChar=AE_PatStrChar(&ciStr);

        if (*++wpPat == L'^')
        {
          bExclude=TRUE;
          ++wpPat;
        }
        else bExclude=FALSE;

        for (; *wpPat != L']'; ++wpPat)
        {
          if (*wpPat == L'-')
          {
            if (!(dwCmpResult & RECC_MIX))
            {
              ++wpPat;
              dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &wchPatNextChar);
              if (dwCmpResult & RECC_EQUAL)
              {
                if (bExclude) goto EndLoop;
                break;
              }
              if (nStrChar < 0) nStrChar=L'\n';

              //Check range
              if (!(dwCmpResult & RECC_MIX))
              {
                if ((nStrChar >= wchPatChar && nStrChar <= wchPatNextChar) ||
                    (!(hStack->dwOptions & REO_MATCHCASE) &&
                      (((wchCaseChar=WideCharLower((wchar_t)nStrChar)) >= wchPatChar && wchCaseChar <= wchPatNextChar) ||
                       (wchCaseChar == nStrChar && (wchCaseChar=WideCharUpper((wchar_t)nStrChar)) >= wchPatChar && wchCaseChar <= wchPatNextChar))))
                {
                  if (bExclude) goto EndLoop;
                  break;
                }
              }
            }
          }
          else
          {
            dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &wchPatChar);
            if (dwCmpResult & RECC_EQUAL)
            {
              if (bExclude) goto EndLoop;
              break;
            }
          }
        }

        if (*wpPat == L']')
        {
          if (!bExclude) goto EndLoop;
        }
        else while (*++wpPat != L']');
      }
      else
      {
        nStrChar=AE_PatStrChar(&ciStr);
        dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &wchPatChar);

        if (!(dwCmpResult & RECC_EQUAL))
        {
          if (dwCmpResult & RECC_DIF)
            goto EndLoop;

          if (dwCmpResult & RECC_BOUNDARY)
          {
            AECHARINDEX ciPrevChar=ciStr;
            BOOL bCharDelim;
            BOOL bPrevCharDelim;

            bCharDelim=PatIsCharDelim(nStrChar, hStack->wpDelim, hStack->wpMaxDelim);
            AEC_PrevChar(&ciPrevChar);
            if (ciPrevChar.lpLine)
              bPrevCharDelim=PatIsCharDelim(AE_PatStrChar(&ciPrevChar), hStack->wpDelim, hStack->wpMaxDelim);
            else
              bPrevCharDelim=TRUE;

            if (bCharDelim != bPrevCharDelim)
            {
              if (*wpPat == L'b')
              {
                ++wpPat;
                continue;
              }
              goto EndLoop;
            }
            else
            {
              if (*wpPat == L'B')
              {
                ++wpPat;
                continue;
              }
              goto EndLoop;
            }
          }
          else if (dwCmpResult & RECC_REF)
          {
            nRefIndex=(int)xatoiW(wpPat, &wpPat);

            if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
            {
              ciStrCount=ciStr;
              ciGroupCount=lpREGroupRef->ciStrStart;
              nCount=0;

              while (AEC_IndexCompare(&ciGroupCount, &lpREGroupRef->ciStrEnd) < 0 && AEC_IndexCompare(&ciStrCount, &ciMaxStr) < 0)
              {
                nStrChar=AE_PatStrChar(&ciStrCount);
                nGroupChar=AE_PatStrChar(&ciGroupCount);
                if (nStrChar < 0) nStrChar=L'\n';
                if (nGroupChar < 0) nGroupChar=L'\n';

                if ((hStack->dwOptions & REO_MATCHCASE)?
                     nStrChar != nGroupChar :
                     WideCharLower((wchar_t)nStrChar) != WideCharLower((wchar_t)nGroupChar))
                  goto EndLoop;

                AE_PatNextChar(&ciStrCount);
                AE_PatNextChar(&ciGroupCount);
                ++nCount;
              }
              if (AEC_IndexCompare(&ciGroupCount, &lpREGroupRef->ciStrEnd) < 0)
                goto EndLoop;
              ciStr=ciStrCount;
              nStrLen+=nCount;
            }
            else goto EndLoop;

            continue;
          }
        }
      }
      AE_PatNextChar(&ciStr);
      ++nStrLen;
      ++wpPat;
    }
    EndLoop:
    if (nCurMatch < lpREGroupItem->nMinMatch)
    {
      if (lpREGroupItem->dwFlags & REGF_NEGATIVE)
      {
        lpREGroupItem->ciStrStart=ciMinStr;
        lpREGroupItem->ciStrEnd=ciMinStr;
        lpREGroupItem->nStrLen=0;
        return TRUE;
      }
      return FALSE;
    }
    if (!nCurMatch)
    {
      lpREGroupItem->ciStrStart=ciStrStart;
      lpREGroupItem->ciStrEnd=ciStrStart;
      lpREGroupItem->nStrLen=0;
    }
  }
  if (lpREGroupItem->dwFlags & REGF_NEGATIVE)
  {
    //lpREGroupItem->ciStrStart=ciMinStr;
    //lpREGroupItem->ciStrEnd=ciMinStr;
    //lpREGroupItem->nStrLen=0;
    return FALSE;
  }
  if (lpREGroupItem->dwFlags & REGF_POSITIVE)
  {
    lpREGroupItem->ciStrStart=ciMinStr;
    lpREGroupItem->ciStrEnd=ciMinStr;
    lpREGroupItem->nStrLen=0;
  }
  return TRUE;
}

int AE_PatStrChar(const AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
  {
    if (ciChar->lpLine->nLineBreak == AELB_WRAP)
      return ciChar->lpLine->next->wpLine[0];
    return -ciChar->lpLine->nLineBreak;
  }
  return ciChar->lpLine->wpLine[ciChar->nCharInLine];
}

AELINEDATA* AE_PatNextChar(AECHARINDEX *ciChar)
{
  if (++ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
  {
    if (ciChar->nCharInLine > ciChar->lpLine->nLineLen ||
        ciChar->lpLine->nLineBreak == AELB_WRAP)
    {
      AEC_NextLine(ciChar);
    }
  }
  return ciChar->lpLine;
}

REGROUP* AE_PatCharInGroup(STACKREGROUP *hStack, const AECHARINDEX *ciChar)
{
  REGROUP *lpREGroupItem;

  for (lpREGroupItem=hStack->first; lpREGroupItem; lpREGroupItem=PatNextGroup(lpREGroupItem))
  {
    if (lpREGroupItem->dwUserData)
    {
      if (AEC_IndexCompare(ciChar, &lpREGroupItem->ciStrStart) >= 0 && AEC_IndexCompare(ciChar, &lpREGroupItem->ciStrEnd) < 0)
        break;
    }
  }
  return lpREGroupItem;
}
#endif //__AKELEDIT_H__

int PatStructExec(PATEXEC *pe)
{
  REGROUP *lpREGroupRoot;
  int nMatchCount=0;
  int nMaxMatchCount=-1;
  BOOL bMatched=FALSE;

  if (!pe->lpREGroupStack)
  {
    if (pe->lpREGroupStack=(STACKREGROUP *)GlobalAlloc(GMEM_FIXED, sizeof(STACKREGROUP)))
    {
      pe->lpREGroupStack->first=0;
      pe->lpREGroupStack->last=0;
      pe->lpREGroupStack->dwOptions=0;
      if (pe->dwOptions & REPE_MATCHCASE)
        pe->lpREGroupStack->dwOptions|=REO_MATCHCASE;
      if (pe->dwOptions & REPE_MULTILINE)
        pe->lpREGroupStack->dwOptions|=REO_MULTILINE;
      if (pe->dwOptions & REPE_NOFIRSTLINEBEGIN)
        pe->lpREGroupStack->dwOptions|=REO_NOFIRSTLINEBEGIN;
      if (pe->dwOptions & REPE_NOLASTLINEEND)
        pe->lpREGroupStack->dwOptions|=REO_NOLASTLINEEND;
      pe->lpREGroupStack->wpDelim=pe->wpDelim;
      pe->lpREGroupStack->wpMaxDelim=pe->wpMaxDelim;
      if (pe->nErrorOffset=PatCompile(pe->lpREGroupStack, pe->wpPat, pe->wpMaxPat))
        return 0;
    }
  }
  if (!(lpREGroupRoot=pe->lpREGroupStack->first))
    return 0;
  if (pe->dwOptions & REPE_ISMATCH)
  {
    lpREGroupRoot->dwFlags&=~REGF_ROOTANY;
    nMaxMatchCount=1;
  }
  else if (!(pe->dwOptions & REPE_GLOBAL))
    nMaxMatchCount=1;

  if (pe->wpStr)
  {
    const wchar_t *wpLineStart;

    while (pe->wpStr < pe->wpMaxStr)
    {
      if (bMatched=PatExec(pe->lpREGroupStack, lpREGroupRoot, pe->wpStr, pe->wpMaxStr))
      {
        //*(pe->wpMaxPat - 1) == L'$'
        if ((pe->dwOptions & REPE_MULTILINE) && pe->wpStr == lpREGroupRoot->wpStrEnd)
        {
          if (*pe->wpStr == L'\r' || *pe->wpStr == L'\n')
          {
            wpLineStart=pe->wpStr;

            if (*wpLineStart == L'\r')
            {
              if (wpLineStart + 1 < pe->wpMaxStr && *(wpLineStart + 1) == L'\n')
                wpLineStart+=2;
              else if (wpLineStart + 2 < pe->wpMaxStr && *(wpLineStart + 1) == L'\r' && *(wpLineStart + 2) == L'\n')
                wpLineStart+=3;
              else
                wpLineStart+=1;
            }
            else wpLineStart+=1;

            if (wpLineStart < pe->wpMaxStr)
            {
              //Execute without new line
              if (bMatched=PatExec(pe->lpREGroupStack, lpREGroupRoot, wpLineStart, pe->wpMaxStr))
                ++nMatchCount;
            }
            else bMatched=FALSE;
          }
          else bMatched=FALSE;
        }
        else ++nMatchCount;
      }

      if (pe->lpCallback)
      {
        if (pe->lpCallback(pe, lpREGroupRoot, bMatched) == REPEC_STOP)
          return nMatchCount;
      }

      //Find next match
      if (!bMatched || (DWORD)nMatchCount >= (DWORD)nMaxMatchCount)
        break;
      pe->wpStr=lpREGroupRoot->wpStrEnd;
    }
  }
  else
  {
    #ifdef __AKELEDIT_H__
    AECHARINDEX ciLineStart;

    while (AEC_IndexCompare(&pe->ciStr, &pe->ciMaxStr) < 0)
    {
      if (bMatched=AE_PatExec(pe->lpREGroupStack, lpREGroupRoot, &pe->ciStr, &pe->ciMaxStr))
      {
        //*(pe->wpMaxPat - 1) == L'$'
        if ((pe->dwOptions & REPE_MULTILINE) && !AEC_IndexCompare(&pe->ciStr, &lpREGroupRoot->ciStrEnd))
        {
          //Is new line?
          if (AE_PatStrChar(&pe->ciStr) < 0)
          {
            ciLineStart=pe->ciStr;
            AE_PatNextChar(&ciLineStart);

            if (AEC_IndexCompare(&ciLineStart, &pe->ciMaxStr) < 0)
            {
              //Execute without new line
              if (bMatched=AE_PatExec(pe->lpREGroupStack, lpREGroupRoot, &ciLineStart, &pe->ciMaxStr))
                ++nMatchCount;
            }
            else bMatched=FALSE;
          }
          else bMatched=FALSE;
        }
        else ++nMatchCount;
      }

      if (pe->lpCallback)
      {
        if (pe->lpCallback(pe, lpREGroupRoot, bMatched) == REPEC_STOP)
          return nMatchCount;
      }

      //Find next match
      if (!bMatched || (DWORD)nMatchCount >= (DWORD)nMaxMatchCount)
        break;
      pe->ciStr=lpREGroupRoot->ciStrEnd;
    }
    #endif
  }
  return nMatchCount;
}

void PatStructFree(PATEXEC *pe)
{
  if (pe->lpREGroupStack)
  {
    PatFree(pe->lpREGroupStack);
    GlobalFree((HGLOBAL)pe->lpREGroupStack);
    pe->lpREGroupStack=NULL;
  }
}

INT_PTR PatReplace(PATREPLACE *pr)
{
  PATEXEC pe;
  PATEXECPARAM pep;

  //Replace using RegExp
  pep.wpRep=pr->wpRep;
  pep.wpMaxRep=pr->wpMaxRep;
  pep.wpNewLine=pr->wpNewLine?pr->wpNewLine:L"\r\n";
  pep.wszBuf=pr->wszResult;
  pep.wpBufCount=pep.wszBuf;

  pe.lpREGroupStack=0;
  pe.wpPat=pr->wpPat;
  pe.wpMaxPat=pr->wpMaxPat;
  pe.wpStr=pr->wpStr;
  pe.wpMaxStr=pr->wpMaxStr;
  #ifdef __AKELEDIT_H__
    pe.ciStr=pr->ciStr;
    pe.ciMaxStr=pr->ciMaxStr;
  #endif
  pe.dwOptions=pr->dwOptions;
  pe.wpDelim=pr->wpDelim;
  pe.wpMaxDelim=pr->wpMaxDelim;
  if (pr->wpStr)
    pe.lpCallback=(PATEXECCALLBACK)PatReplaceCallback;
  else
    pe.lpCallback=(PATEXECCALLBACK)AE_PatReplaceCallback;
  pe.lParam=(LPARAM)&pep;

  if (pr->nReplaceCount=PatStructExec(&pe))
  {
    if (pr->wpStr)
    {
      pr->wpLeftStr=pe.lpREGroupStack->first->wpStrStart;
      pr->wpRightStr=pep.wpRightStr;
    }
    else
    {
      #ifdef __AKELEDIT_H__
        pr->ciLeftStr=pe.lpREGroupStack->first->ciStrStart;
        pr->ciRightStr=pep.ciRightStr;
      #endif
    }
    if (pep.wszBuf)
      *pep.wpBufCount=L'\0';
    else
      ++pep.wpBufCount;
  }
  else pr->nErrorOffset=pe.nErrorOffset;

  PatStructFree(&pe);

  return (INT_PTR)(pep.wpBufCount - pep.wszBuf);
}

int CALLBACK PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched)
{
  PATEXECPARAM *pep=(PATEXECPARAM *)pe->lParam;
  REGROUP *lpREGroupRef;
  const wchar_t *wpRep=pep->wpRep;
  int nPatChar;
  int nIndex;

  if (bMatched)
  {
    //Copy unmatched left part of string
    if (pep->wszBuf)
      xmemcpy(pep->wpBufCount, pe->wpStr, (lpREGroupRoot->wpStrStart - pe->wpStr) * sizeof(wchar_t));
    pep->wpBufCount+=lpREGroupRoot->wpStrStart - pe->wpStr;

    //Replace matched part of string
    while (wpRep < pep->wpMaxRep)
    {
      nPatChar=PatEscChar(&wpRep);

      if (nPatChar == REEC_REF)
      {
        nIndex=PatRefIndex(&wpRep);

        if (lpREGroupRef=PatGetGroup(pe->lpREGroupStack, nIndex))
        {
          if (pep->wszBuf)
            xmemcpy(pep->wpBufCount, lpREGroupRef->wpStrStart, (lpREGroupRef->wpStrEnd - lpREGroupRef->wpStrStart) * sizeof(wchar_t));
          pep->wpBufCount+=lpREGroupRef->wpStrEnd - lpREGroupRef->wpStrStart;
        }
        continue;
      }
      if (nPatChar == REEC_NEWLINE)
      {
        if (pep->wszBuf)
          pep->wpBufCount+=xstrcpyW(pep->wpBufCount, pep->wpNewLine);
        else
          pep->wpBufCount+=xstrlenW(pep->wpNewLine);
      }
      else if (nPatChar != REEC_WRONG)
      {
        if (pep->wszBuf)
          *pep->wpBufCount=(wchar_t)nPatChar;
        ++pep->wpBufCount;
      }
      ++wpRep;
    }
    pep->wpRightStr=lpREGroupRoot->wpStrEnd;
  }
  else
  {
    //Copy unmatched right part of string.
    if (pep->wszBuf)
      xmemcpy(pep->wpBufCount, pe->wpStr, (pe->wpMaxStr - pe->wpStr) * sizeof(wchar_t));
    pep->wpBufCount+=pe->wpMaxStr - pe->wpStr;
    pep->wpRightStr=pe->wpMaxStr;
  }
  return REPEC_CONTINUE;
}

int CALLBACK AE_PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched)
{
  #ifdef __AKELEDIT_H__
  PATEXECPARAM *pep=(PATEXECPARAM *)pe->lParam;
  REGROUP *lpREGroupRef;
  const wchar_t *wpRep=pep->wpRep;
  int nPatChar;
  int nIndex;

  if (bMatched)
  {
    //Copy unmatched left part of string
    pep->wpBufCount+=AE_PatStrCopy(&pe->ciStr, &lpREGroupRoot->ciStrStart, pep->wszBuf?pep->wpBufCount:NULL, NULL);

    //Replace matched part of string
    while (wpRep < pep->wpMaxRep)
    {
      nPatChar=PatEscChar(&wpRep);

      if (nPatChar == REEC_REF)
      {
        nIndex=PatRefIndex(&wpRep);

        if (lpREGroupRef=PatGetGroup(pe->lpREGroupStack, nIndex))
        {
          pep->wpBufCount+=AE_PatStrCopy(&lpREGroupRef->ciStrStart, &lpREGroupRef->ciStrEnd, pep->wszBuf?pep->wpBufCount:NULL, NULL);
        }
        continue;
      }
      if (nPatChar == REEC_NEWLINE)
      {
        if (pep->wszBuf)
          pep->wpBufCount+=xstrcpyW(pep->wpBufCount, pep->wpNewLine);
        else
          pep->wpBufCount+=xstrlenW(pep->wpNewLine);
      }
      else if (nPatChar != REEC_WRONG)
      {
        if (pep->wszBuf)
          *pep->wpBufCount=(wchar_t)nPatChar;
        ++pep->wpBufCount;
      }
      ++wpRep;
    }
    pep->ciRightStr=lpREGroupRoot->ciStrEnd;
  }
  else
  {
    //Copy unmatched right part of string.
    pep->wpBufCount+=AE_PatStrCopy(&pe->ciStr, &pe->ciMaxStr, pep->wszBuf?pep->wpBufCount:NULL, NULL);
    pep->ciRightStr=pe->ciMaxStr;
  }
  #endif
  return REPEC_CONTINUE;
}

#ifdef __AKELEDIT_H__
INT_PTR AE_PatStrCopy(AECHARINDEX *ciStart, AECHARINDEX *ciEnd, wchar_t *wszTarget, const wchar_t *wpTargetMax)
{
  AECHARINDEX ciCount;
  wchar_t *wpTarget=wszTarget;

  if (!wpTargetMax) wpTargetMax+=0x3FFFFFFF;

  for (ciCount=*ciStart; AEC_IndexCompare(&ciCount, ciEnd) < 0 && wpTarget < wpTargetMax; AE_PatNextChar(&ciCount))
  {
    if (ciCount.nCharInLine >= ciCount.lpLine->nLineLen)
    {
      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
      {
        if (wszTarget)
          *wpTarget++=ciCount.lpLine->next->wpLine[0];
        else
          wpTarget+=1;
      }
      else if (ciCount.lpLine->nLineBreak == AELB_RN)
      {
        if (wpTarget + 1 >= wpTargetMax) break;

        if (wszTarget)
        {
          *wpTarget++=L'\r';
          *wpTarget++=L'\n';
        }
        else wpTarget+=2;
      }
      else if (ciCount.lpLine->nLineBreak == AELB_N)
      {
        if (wszTarget)
          *wpTarget++=L'\n';
        else
          wpTarget+=1;
      }
      else if (ciCount.lpLine->nLineBreak == AELB_R)
      {
        if (wszTarget)
          *wpTarget++=L'\r';
        else
          wpTarget+=1;
      }
      else if (ciCount.lpLine->nLineBreak == AELB_RRN)
      {
        if (wpTarget + 2 >= wpTargetMax) break;

        if (wszTarget)
        {
          *wpTarget++=L'\r';
          *wpTarget++=L'\r';
          *wpTarget++=L'\n';
        }
        else wpTarget+=3;
      }
    }
    else
    {
      if (wszTarget)
        *wpTarget++=ciCount.lpLine->wpLine[ciCount.nCharInLine];
      else
        wpTarget+=1;
    }
  }
  return (wpTarget - wszTarget);
}
#endif
#endif //RE_FUNCTIONS


/********************************************************************
 *                                                                  *
 *                           Example                                *
 *                                                                  *
 ********************************************************************

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "StrFunc.h"
#include "StackFunc.h"
#include "RegExpFunc.h"

//Include string functions
#define WideCharUpper
#define WideCharLower
#define xmemcpy
#define xstrlenW
#define xstrcpyW
#define xstrcmpnW
#define xstrcmpinW
#define xatoiW
#define hex2decW
#include "StrFunc.h"

//Include stack functions
#define StackInsertBefore
#define StackDelete
#define StackJoin
#include "StackFunc.h"

//Include RegExp functions
#define RE_FUNCTIONS
#include "RegExpFunc.h"

void main()
{
  PATEXEC pe;
  REGROUP *lpREGroupRoot;
  REGROUP *lpREGroupNext;
  wchar_t wszResult[MAX_PATH];
  wchar_t *wpResult;

  //Fill structure for PatStructExec
  pe.lpREGroupStack=0;
  pe.wpStr=L"1234567890 11223344556677889900";
  pe.wpMaxStr=pe.wpStr + lstrlenW(pe.wpStr);
  pe.wpPat=L"(23)(.*)(89)";
  pe.wpMaxPat=pe.wpPat + lstrlenW(pe.wpPat);
  pe.dwOptions=REPE_MATCHCASE;
  pe.wpDelim=NULL;
  pe.lpCallback=NULL;

  while (PatStructExec(&pe))
  {
    lpREGroupRoot=pe.lpREGroupStack->first;
    lpREGroupNext=lpREGroupRoot;
    wpResult=wszResult;

    do
    {
      if (lpREGroupNext->wpStrStart != lpREGroupNext->wpStrEnd && lpREGroupNext->nIndex != -1)
      {
        //wpResult+=xprintfW(wpResult, L"%d [%.%ds]\n", lpREGroupNext->nIndex, lpREGroupNext->wpStrEnd - lpREGroupNext->wpStrStart, lpREGroupNext->wpStrStart);
        wpResult+=wsprintfW(wpResult, L"%d [", lpREGroupNext->nIndex);
        lstrcpynW(wpResult, lpREGroupNext->wpStrStart, (lpREGroupNext->wpStrEnd - lpREGroupNext->wpStrStart) + 1);
        wpResult+=lpREGroupNext->wpStrEnd - lpREGroupNext->wpStrStart;
        wpResult+=wsprintfW(wpResult, L"]\n");
      }
    }
    while (lpREGroupNext=PatNextGroup(lpREGroupNext));

    if (MessageBoxW(NULL, wszResult, L"Find next?", MB_YESNO) == IDNO)
      break;
    pe.wpStr=lpREGroupRoot->wpStrEnd;
  }
  PatStructFree(&pe);
}

*/
