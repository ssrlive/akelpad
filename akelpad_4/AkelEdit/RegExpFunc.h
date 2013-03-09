/******************************************************************
 *                  RegExp functions header v1.3                  *
 *                                                                *
 * 2013 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)   *
 *                                                                *
 *                                                                *
 * RegExpFunc.h header uses functions:                            *
 * 1. From StrFunc.h header                                       *
 *   WideCharUpper, WideCharLower, xmemcpy, xmemset,              *
 *   xstrlenW, xstrcpyW, xstrcmpnW, xstrcmpinW, xatoiW, hex2decW  *
 * 2. From StackFunc.h header                                     *
 *   StackInsertBefore, StackDelete, StackJoin                    *
 *****************************************************************/

#ifndef _REGEXPFUNC_H_
#define _REGEXPFUNC_H_

//STACKREGROUP options
#define REO_MATCHCASE        0x001 //Case-sensitive search.
#define REO_MULTILINE        0x002 //Multiline search. Symbols ^ and $ specifies the line edge.
#define REO_STARTLINEBEGIN   0x004 //Internal.
#define REO_ENDLINEFINISH    0x008 //Internal.
#define REO_STARTBOUNDARY    0x010 //Internal.
#define REO_ENDBOUNDARY      0x020 //Internal.
#define REO_REFEXIST         0x100 //Internal.

//REGROUP flags
#define REGF_ROOTITEM         0x001
#define REGF_ROOTANY          0x002
#define REGF_ROOTMULTILINE    0x004
#define REGF_ANY              0x008
#define REGF_AUTOGROUP        0x010
#define REGF_OR               0x020
#define REGF_POSITIVEFORWARD  0x040
#define REGF_NEGATIVEFORWARD  0x100
#define REGF_POSITIVEBACKWARD 0x200
#define REGF_NEGATIVEBACKWARD 0x400

//PatCharCmp flags
#define RECCF_MATCHCASE     0x01 //Case-sensitive search.
#define RECCF_FULLSURROGATE 0x02 //Check full surrogate character.

//PatCharCmp return value
#define RECCE_EQUAL         0x01
#define RECCE_DIF           0x02
#define RECCE_MIX           0x04
#define RECCE_BOUNDARY      0x08
#define RECCE_STRBEGIN      0x10
#define RECCE_STREND        0x20
#define RECCE_REF           0x40

//PatEscChar return value
#define REEC_FIRST      (MAXLONG - 20)
#define REEC_WRONG      (REEC_FIRST + 0)
#define REEC_NEWLINE    (REEC_FIRST + 1)
#define REEC_BOUNDARY   (REEC_FIRST + 2)
#define REEC_STRBEGIN   (REEC_FIRST + 3)
#define REEC_STREND     (REEC_FIRST + 4)
#define REEC_REF        (REEC_FIRST + 5)
#define REEC_DIGIT      (REEC_FIRST + 6)
#define REEC_NONDIGIT   (REEC_FIRST + 7)
#define REEC_SPACE      (REEC_FIRST + 8)
#define REEC_NONSPACE   (REEC_FIRST + 9)
#define REEC_LATIN      (REEC_FIRST + 10)
#define REEC_NONLATIN   (REEC_FIRST + 11)

//PatStructExec options
#define REPE_MATCHCASE        0x001 //Case-sensitive search.
#define REPE_MULTILINE        0x002 //Multiline search. Symbols ^ and $ specifies the line edge.
#define REPE_STARTLINEBEGIN   0x004 //PATEXEC.wpStr starts from line beginning. Used with REPE_MULTILINE flag.
#define REPE_ENDLINEFINISH    0x008 //PATEXEC.wpMaxStr ends on line ending. Used with REPE_MULTILINE flag.
#define REPE_STARTBOUNDARY    0x010 //PATEXEC.wpStr is word boundary. Valid if metacharacter \b or \B used in pattern.
#define REPE_ENDBOUNDARY      0x020 //PATEXEC.wpMaxStr is word boundary. Valid if metacharacter \b or \B used in pattern.
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
//pe              Pointer to a PATEXEC structure.
//lpREGroupRoot   Pointer to a first REGROUP structure in stack (root group).
//bMatched        TRUE  - lpREGroupRoot->wpStrStart and lpREGroupRoot->wpStrEnd are valid.
//                FALSE - pe->wpStr is valid.
//Return Value
// See REPEC_* defines.

typedef struct {
  STACKREGROUP *lpREGroupStack; //Groups stack. Must be zero if PatStructExec called for the first time.
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
  wchar_t *wszResult;        //Buffer that received replace result. If NULL, PatReplace returns required buffer size in characters.
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
  STACKREGROUP *lpREGroupStack; //Groups stack. Filled by PatExec message.
  const wchar_t *wpStr;         //String for process. Can be used "$n" - the n'th captured submatch.
  const wchar_t *wpMaxStr;      //Pointer to the last character. If wpStr is null-terminated, then wpMaxStr is pointer to the NULL character.
  wchar_t *wszResult;           //Buffer that received convert result. If NULL, PatGroupStr returns required buffer size in characters.
} PATGROUPSTR;

INT_PTR PatCompile(STACKREGROUP *hStack, const wchar_t *wpPat, const wchar_t *wpMaxPat);
BOOL PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, const wchar_t *wpStr, const wchar_t *wpMaxStr);
int PatStrChar(const wchar_t *wpStr, const wchar_t *wpMaxStr, int *nChar);
BOOL PatIsCharBoundary(const wchar_t *wpChar, int nChar, STACKREGROUP *hStack);
int PatEscChar(const wchar_t **wppPat);
DWORD PatCharCmp(const wchar_t **wppPat, int nStrChar, DWORD dwFlags, int *lpnPatChar);
BOOL PatIsCharDelim(int nChar, const wchar_t *wpDelim, const wchar_t *wpMaxDelim);
int PatRefIndex(const wchar_t **wppPat);
REGROUP* PatCloseAutoGroups(REGROUP *lpREGroupItem, const wchar_t *wpPat, BOOL *lpbGroupNextChars);
BOOL PatIsInNonCapture(REGROUP *lpREGroupItem);
REGROUP* PatGetGroup(STACKREGROUP *hStack, int nIndex);
REGROUP* PatNextGroup(REGROUP *lpREGroupItem);
REGROUP* PatNextGroupNoChild(REGROUP *lpREGroupItem);
REGROUP* PatPrevGroup(REGROUP *lpREGroupItem);
INT_PTR PatGroupStr(PATGROUPSTR *pgs);
void PatReset(STACKREGROUP *hStack);
void PatFree(STACKREGROUP *hStack);

#ifdef __AKELEDIT_H__
  BOOL AE_PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, AECHARINDEX *ciInput, AECHARINDEX *ciMaxInput);
  int AE_PatStrChar(const AECHARINDEX *ciChar);
  BOOL AE_PatIsCharBoundary(AECHARINDEX *ciChar, const wchar_t *wpDelim, const wchar_t *wpMaxDelim);
  AELINEDATA* AE_PatNextChar(AECHARINDEX *ciChar);
  REGROUP* AE_PatCharInGroup(STACKREGROUP *hStack, const AECHARINDEX *ciChar);
  void AE_PatReset(STACKREGROUP *hStack);
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
  const wchar_t *wpMinPat=wpPat;
  const wchar_t *wpClassStart=NULL;
  const wchar_t *wpClassEnd=NULL;
  const wchar_t *wpCharStart=NULL;
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
    lpREGroupItem->dwFlags|=REGF_ROOTANY;

    if (*wpPat == L'\\' && wpPat + 1 < wpMaxPat && *(wpPat + 1) == L'A')
    {
      wpPat+=2;
      lpREGroupItem->dwFlags&=~REGF_ROOTANY;
    }
    if (*wpPat == L'^')
    {
      if (hStack->dwOptions & REO_MULTILINE)
        lpREGroupItem->dwFlags|=REGF_ROOTMULTILINE;
      else
      {
        ++wpPat;
        lpREGroupItem->dwFlags&=~REGF_ROOTANY;
      }
    }
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
      {
        if (*++wpPat == L'{')
        {
          const wchar_t *wpCodeEnd;

          for (wpCodeEnd=++wpPat; *wpCodeEnd && *wpCodeEnd != L'}'; ++wpCodeEnd);
          if (*wpCodeEnd)
            wpPat=wpCodeEnd + 1;
          else
            wpPat=wpCodeEnd;
        }
        else wpPat+=2;
      }
      else if (*wpPat == L'u')
        wpPat+=5;
      else if (*wpPat >= L'0' && *wpPat <= L'9')
      {
        int nPatRefIndex=PatRefIndex(&wpPat);

        if (nPatRefIndex > nIndex || nPatRefIndex == 0)
        {
          wpPat=wpCharStart;
          goto Error;
        }
        hStack->dwOptions|=REO_REFEXIST;
      }
      else if (*wpPat == L'A' && lpREGroupItem->wpPatStart != wpPat - 1)
        goto Error;
      else if ((*wpPat == L'Z' || *wpPat == L'z') &&
               (wpPat + 1 < wpMaxPat && *(wpPat + 1) != L')'))
      {
        goto Error;
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
      //Close all REGF_AUTOGROUP
      lpREGroupItem=PatCloseAutoGroups(lpREGroupItem, wpPat, &bGroupNextChars);

      if (lpREGroupItem->dwFlags & REGF_OR)
      {
        lpREGroupItem->wpPatEnd=wpPat;
        lpREGroupItem->wpPatRight=wpPat;

        lpREGroupItem=lpREGroupItem->parent;
      }
      if (lpREGroupItem->wpPatEnd)
        goto Error;

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
      if ((DWORD)lpREGroupNew->nMinMatch > (DWORD)lpREGroupNew->nMaxMatch)
        goto Error;
      if ((DWORD)lpREGroupNew->nMaxMatch > 1 &&
          ((lpREGroupNew->wpPatEnd - 1 >= lpREGroupNew->wpPatStart && (*(lpREGroupNew->wpPatEnd - 1) == L'$' || *(lpREGroupNew->wpPatEnd - 1) == L'^')) ||
           (lpREGroupNew->wpPatEnd - 2 >= lpREGroupNew->wpPatStart && *(lpREGroupNew->wpPatEnd - 2) == L'\\' && (*(lpREGroupNew->wpPatEnd - 1) == L'b' || *(lpREGroupNew->wpPatEnd - 1) == L'B'))))
        goto Error;
      if (lpREGroupNew->nMinMatch != 1 || lpREGroupNew->nMaxMatch != 1)
        bGroupNextChars=TRUE;
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
          else if (*wpPat == L'<')
          {
            if (*++wpPat == L'=')
            {
              //Non-capture positive backward group
              lpREGroupNew->dwFlags|=REGF_POSITIVEBACKWARD;
            }
            else if (*wpPat == L'!')
            {
              //Non-capture negative backward group
              lpREGroupNew->dwFlags|=REGF_NEGATIVEBACKWARD;
            }
            else goto Error;
          }
          else if (*wpPat == L'=')
          {
            //Non-capture positive foreward group
            lpREGroupNew->dwFlags|=REGF_POSITIVEFORWARD;
          }
          else if (*wpPat == L'!')
          {
            //Non-capture negative foreward group
            lpREGroupNew->dwFlags|=REGF_NEGATIVEFORWARD;
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
  int nCharSize;
  int nPatChar;
  int nPatNextChar;
  wchar_t wchCaseChar;
  int nCurMatch=0;
  int nRefIndex;
  INT_PTR nRefLen;
  DWORD dwCmpResult=0;
  BOOL bExclude;
  int nNegativeBackward=0;

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

        if ((DWORD)nCurMatch < (DWORD)lpREGroupItem->nMaxMatch && nCurMatch >= lpREGroupItem->nMinMatch)
        {
          if (lpREGroupNextNext=PatNextGroupNoChild(lpREGroupItem))
          {
            if (hStack->dwOptions & REO_REFEXIST)
            {
              //Set wpStrEnd to all parent groups for possible backreferences in child PatExec.
              for (lpREGroupRef=lpREGroupItem->parent; lpREGroupRef; lpREGroupRef=lpREGroupRef->parent)
                lpREGroupRef->wpStrEnd=wpStr;
            }
            //Check nStrLen for \d+Z? in 123Z
            if (PatExec(hStack, lpREGroupNextNext, wpStr, wpMaxStr) && (lpREGroupNextNext->nStrLen || lpREGroupNextNext->nMinMatch))
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
            if (lpREGroupNextNext=PatNextGroupNoChild(lpREGroupNext))
            {
              if (PatExec(hStack, lpREGroupNextNext, wpStr, wpMaxStr) && (lpREGroupNextNext->nStrLen || lpREGroupNextNext->nMinMatch))
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
          if (wpPat == lpREGroupItem->wpPatStart && !lpREGroupItem->parent &&
              ((lpREGroupNext->dwFlags & REGF_POSITIVEBACKWARD) || (lpREGroupNext->dwFlags & REGF_NEGATIVEBACKWARD)))
            wpStrStart=wpStr;
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
        if (((hStack->dwOptions & REO_ENDLINEFINISH) && wpStr == wpMaxStr) ||
            ((hStack->dwOptions & REO_MULTILINE) && (*wpStr == L'\n' || *wpStr == L'\r')))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }
      if (*wpPat == L'^')
      {
        //REO_MULTILINE
        if (((hStack->dwOptions & REO_STARTLINEBEGIN) && wpStr == hStack->first->wpStrStart) ||
            (wpStr > hStack->first->wpStrStart && (*(wpStr - 1) == L'\n' || *(wpStr - 1) == L'\r')))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }
      if (wpStr >= wpMaxStr)
      {
        if (wpPat + 2 == wpMaxPat && *wpPat == L'\\')
        {
          if ((hStack->dwOptions & REO_ENDBOUNDARY) && *(wpPat + 1) == L'b')
            goto Match;
          else if (*(wpPat + 1) == L'Z' || *(wpPat + 1) == L'z')
            goto Match;
        }
        nNegativeBackward=-1;
        goto EndLoop;
      }

      if (*wpPat == L'.')
      {
        ////Any character except new line
        //wpStr+=PatStrChar(wpStr, wpMaxStr, &nStrChar);
        //if (nStrChar < 0) goto EndLoop;
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
            if (!(dwCmpResult & RECCE_MIX))
            {
              ++wpPat;
              dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE)|RECCF_FULLSURROGATE, &nPatNextChar);
              if (dwCmpResult & RECCE_EQUAL)
                goto ClassMatch;
              if (nStrChar < 0) nStrChar=L'\n';

              //Check range
              if (!(dwCmpResult & RECCE_MIX))
              {
                if ((nStrChar >= nPatChar && nStrChar <= nPatNextChar) ||
                    (!(hStack->dwOptions & REO_MATCHCASE) && nStrChar <= MAXWORD &&
                      (((wchCaseChar=WideCharLower((wchar_t)nStrChar)) >= nPatChar && wchCaseChar <= nPatNextChar) ||
                       (wchCaseChar == nStrChar && (wchCaseChar=WideCharUpper((wchar_t)nStrChar)) >= nPatChar && wchCaseChar <= nPatNextChar))))
                {
                  goto ClassMatch;
                }
              }
            }
          }
          else
          {
            dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE)|RECCF_FULLSURROGATE, &nPatChar);
            if (dwCmpResult & RECCE_EQUAL)
              goto ClassMatch;
          }
        }
        if (!bExclude) goto EndLoop;
        goto NextChar;

        ClassMatch:
        if (bExclude) goto EndLoop;
        while (*++wpPat != L']')
        {
          if (*wpPat == L'\\')
            ++wpPat;
        }
      }
      else
      {
        nCharSize=PatStrChar(wpStr, wpMaxStr, &nStrChar);
        dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &nPatChar);

        if (!(dwCmpResult & RECCE_EQUAL))
        {
          if (dwCmpResult & RECCE_DIF)
            goto EndLoop;

          if (dwCmpResult & RECCE_BOUNDARY)
          {
            if (PatIsCharBoundary(wpStr, nStrChar, hStack))
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
          else if (dwCmpResult & RECCE_STRBEGIN)
          {
            if (wpStr == hStack->first->wpStrStart)
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_STREND)
          {
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_REF)
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
        if (nCharSize)
          wpStr+=nCharSize;
        if (nPatChar <= MAXWORD && nStrChar > MAXWORD)
        {
          ++wpPat;
          continue;
        }
      }
      NextChar:
      ++wpStr;
      ++wpPat;
    }
    EndLoop:
    if (nCurMatch < lpREGroupItem->nMinMatch)
    {
      if (lpREGroupItem->dwFlags & REGF_NEGATIVEFORWARD)
      {
        lpREGroupItem->wpStrStart=wpMinStr;
        lpREGroupItem->wpStrEnd=wpMinStr;
        lpREGroupItem->nStrLen=0;
        return TRUE;
      }
      if (lpREGroupItem->dwFlags & REGF_NEGATIVEBACKWARD)
      {
        if (wpPat < lpREGroupItem->wpPatEnd && nNegativeBackward != -1)
        {
          ++nNegativeBackward;
          goto NextChar;
        }
        lpREGroupItem->wpStrStart=wpStr;
        lpREGroupItem->wpStrEnd=wpStr;
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
  if (lpREGroupItem->dwFlags & REGF_NEGATIVEFORWARD)
  {
    return FALSE;
  }
  if (lpREGroupItem->dwFlags & REGF_NEGATIVEBACKWARD)
  {
    if (nNegativeBackward)
    {
      lpREGroupItem->wpStrStart=wpStr;
      lpREGroupItem->wpStrEnd=wpStr;
      lpREGroupItem->nStrLen=0;
      return TRUE;
    }
    return FALSE;
  }
  if (lpREGroupItem->dwFlags & REGF_POSITIVEFORWARD)
  {
    lpREGroupItem->wpStrStart=wpMinStr;
    lpREGroupItem->wpStrEnd=wpMinStr;
    lpREGroupItem->nStrLen=0;
  }
  if (lpREGroupItem->dwFlags & REGF_POSITIVEBACKWARD)
  {
    lpREGroupItem->wpStrStart=wpStr;
    lpREGroupItem->wpStrEnd=wpStr;
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
  if (wpStr + 1 < wpMaxStr && AEC_IsHighSurrogate(*wpStr))
  {
    *nChar=AEC_ScalarFromSurrogate(*wpStr, *(wpStr + 1));
    return 1;
  }
  *nChar=*wpStr;
  return 0;
}

BOOL PatIsCharBoundary(const wchar_t *wpChar, int nChar, STACKREGROUP *hStack)
{
  int nPrevChar;
  BOOL bPrevCharDelim;

  if (wpChar > hStack->first->wpStrStart)
  {
    nPrevChar=*(wpChar - 1);
    if (nPrevChar == L'\r') nPrevChar=L'\n';
    bPrevCharDelim=PatIsCharDelim(nPrevChar, hStack->wpDelim, hStack->wpMaxDelim);
  }
  else bPrevCharDelim=(hStack->dwOptions & REO_STARTBOUNDARY)?TRUE:FALSE;

  if (PatIsCharDelim(nChar, hStack->wpDelim, hStack->wpMaxDelim) != bPrevCharDelim)
    return TRUE;
  return FALSE;
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
      ++(*wppPat);
      if (**wppPat == L'{')
      {
        const wchar_t *wpCodeEnd;

        for (wpCodeEnd=++(*wppPat); *wpCodeEnd && *wpCodeEnd != L'}'; ++wpCodeEnd);
        nPatChar=(int)hex2decW(*wppPat, wpCodeEnd - *wppPat);
        if (*wpCodeEnd)
          *wppPat=wpCodeEnd;
        else
          *wppPat=wpCodeEnd - 1;
      }
      else
      {
        nPatChar=(int)hex2decW(*wppPat, 2);
        (*wppPat)+=1;
      }
    }
    else
    {
      if (nPatChar == L'r' || nPatChar == L'n')
        return REEC_NEWLINE;
      if (nPatChar == L't')
        return L'\t';
      if (nPatChar == L'f')
        return L'\f';
      if (nPatChar == L'v')
        return L'\v';
      if (nPatChar == L'd')
        return REEC_DIGIT;
      if (nPatChar == L'D')
        return REEC_NONDIGIT;
      if (nPatChar == L's')
        return REEC_SPACE;
      if (nPatChar == L'S')
        return REEC_NONSPACE;
      if (nPatChar == L'w')
        return REEC_LATIN;
      if (nPatChar == L'W')
        return REEC_NONLATIN;
      if (nPatChar == L'b' || nPatChar == L'B')
        return REEC_BOUNDARY;
      if (nPatChar == L'A')
        return REEC_STRBEGIN;
      if (nPatChar == L'Z' || nPatChar == L'z')
        return REEC_STREND;
      if (nPatChar >= L'0' && nPatChar <= L'9')
        return REEC_REF;
    }
  }
  return nPatChar;
}

DWORD PatCharCmp(const wchar_t **wppPat, int nStrChar, DWORD dwFlags, int *lpnPatChar)
{
  int nPatChar=PatEscChar(wppPat);

  //AELB_R, AELB_N, AELB_RN, AELB_RRN
  if (nStrChar < 0)
  {
    if (nPatChar < REEC_WRONG)
    {
      if (nPatChar == L'\n')
      {
        *lpnPatChar=L'\n';

        if (nStrChar == -AELB_N)
          return RECCE_EQUAL;
      }
      else if (nPatChar == L'\r')
      {
        *lpnPatChar=L'\n';

        if (nStrChar == -AELB_R)
        {
          return RECCE_EQUAL;
        }
        else if (nStrChar == -AELB_RN)
        {
          ++(*wppPat);
          if (PatEscChar(wppPat) == L'\n')
            return RECCE_EQUAL;
        }
        else if (nStrChar == -AELB_RRN)
        {
          ++(*wppPat);
          if (PatEscChar(wppPat) == L'\r')
          {
            ++(*wppPat);
            if (PatEscChar(wppPat) == L'\n')
              return RECCE_EQUAL;
          }
        }
      }
      else *lpnPatChar=nPatChar;
    }
    else
    {
      if (nPatChar == REEC_NEWLINE)
      {
        *lpnPatChar=L'\n';
        return RECCE_EQUAL|RECCE_MIX;
      }
      *lpnPatChar=L'\0';
      if (nPatChar == REEC_BOUNDARY)
        return RECCE_BOUNDARY|RECCE_MIX;
      if (nPatChar == REEC_STRBEGIN)
        return RECCE_STRBEGIN|RECCE_MIX;
      if (nPatChar == REEC_STREND)
        return RECCE_STREND|RECCE_MIX;
      if (nPatChar == REEC_SPACE ||
          nPatChar == REEC_NONDIGIT ||
          nPatChar == REEC_NONLATIN)
      {
        return RECCE_EQUAL|RECCE_MIX;
      }
      if (nPatChar == REEC_REF)
        return RECCE_REF|RECCE_MIX;
      return RECCE_DIF|RECCE_MIX;
    }
    return RECCE_DIF;
  }

  if (nPatChar > REEC_WRONG)
  {
    if (nPatChar >= REEC_DIGIT)
    {
      if (nPatChar == REEC_DIGIT)
      {
        if (nStrChar < L'0' || nStrChar > L'9')
          return RECCE_DIF|RECCE_MIX;
      }
      else if (nPatChar == REEC_NONDIGIT)
      {
        if (nStrChar >= L'0' && nStrChar <= L'9')
          return RECCE_DIF|RECCE_MIX;
      }
      else if (nPatChar == REEC_SPACE)
      {
        if (nStrChar != L' ' && nStrChar != L'\f' && nStrChar != L'\n' && nStrChar != L'\r' && nStrChar != L'\t' && nStrChar != L'\v')
          return RECCE_DIF|RECCE_MIX;
      }
      else if (nPatChar == REEC_NONSPACE)
      {
        if (nStrChar == L' ' || nStrChar == L'\f' || nStrChar == L'\n' || nStrChar == L'\r' || nStrChar == L'\t' || nStrChar == L'\v')
          return RECCE_DIF|RECCE_MIX;
      }
      else if (nPatChar == REEC_LATIN)
      {
        if ((nStrChar < L'A' || nStrChar > L'Z') && (nStrChar < L'a' || nStrChar > L'z') && (nStrChar < L'0' || nStrChar > L'9') && nStrChar != L'_')
          return RECCE_DIF|RECCE_MIX;
      }
      else if (nPatChar == REEC_NONLATIN)
      {
        if ((nStrChar >= L'A' && nStrChar <= L'Z') || (nStrChar >= L'a' && nStrChar <= L'z') || (nStrChar >= L'0' && nStrChar <= L'9') || nStrChar == L'_')
          return RECCE_DIF|RECCE_MIX;
      }
      return RECCE_EQUAL|RECCE_MIX;
    }
    else if (nPatChar == REEC_NEWLINE)
    {
      *lpnPatChar=L'\n';
      return RECCE_DIF|RECCE_MIX;
    }
    else if (nPatChar == REEC_BOUNDARY)
    {
      return RECCE_BOUNDARY|RECCE_MIX;
    }
    else if (nPatChar == REEC_STRBEGIN)
    {
      return RECCE_STRBEGIN|RECCE_MIX;
    }
    else if (nPatChar == REEC_STREND)
    {
      return RECCE_STREND|RECCE_MIX;
    }
    else if (nPatChar == REEC_REF)
    {
      return RECCE_REF|RECCE_MIX;
    }
    return RECCE_DIF|RECCE_MIX;
  }

  //Compare
  *lpnPatChar=nPatChar;
  if (!(dwFlags & RECCF_FULLSURROGATE) && nStrChar > MAXWORD && nPatChar <= MAXWORD)
    nStrChar=AEC_HighSurrogateFromScalar(nStrChar);
  if (nStrChar != nPatChar && ((dwFlags & RECCF_MATCHCASE) || nStrChar > MAXWORD || nPatChar > MAXWORD || WideCharLower((wchar_t)nStrChar) != WideCharLower((wchar_t)nPatChar)))
    return RECCE_DIF;
  return RECCE_EQUAL;
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

BOOL PatIsInNonCapture(REGROUP *lpREGroupItem)
{
  do
  {
    if ((lpREGroupItem->dwFlags & REGF_POSITIVEFORWARD) || (lpREGroupItem->dwFlags & REGF_NEGATIVEFORWARD))
      return TRUE;
  }
  while (lpREGroupItem=lpREGroupItem->parent);

  return FALSE;
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

void PatReset(STACKREGROUP *hStack)
{
  REGROUP *lpREGroupItem;

  for (lpREGroupItem=hStack->first; lpREGroupItem; lpREGroupItem=PatNextGroup(lpREGroupItem))
  {
    lpREGroupItem->wpStrStart=NULL;
    lpREGroupItem->wpStrEnd=NULL;
  }
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

#ifdef __AKELEDIT_H__
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
  int nPatChar;
  int nPatNextChar;
  int nGroupChar;
  wchar_t wchCaseChar;
  int nCurMatch=0;
  int nRefIndex;
  DWORD dwCmpResult=0;
  BOOL bExclude;
  int nNegativeBackward=0;

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
          AEC_NextChar(&ciStr);
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

        if ((DWORD)nCurMatch < (DWORD)lpREGroupItem->nMaxMatch && nCurMatch >= lpREGroupItem->nMinMatch)
        {
          if (lpREGroupNextNext=PatNextGroupNoChild(lpREGroupItem))
          {
            if (hStack->dwOptions & REO_REFEXIST)
            {
              //Set ciStrEnd to all parent groups for possible backreferences in child AE_PatExec.
              for (lpREGroupRef=lpREGroupItem->parent; lpREGroupRef; lpREGroupRef=lpREGroupRef->parent)
                lpREGroupRef->ciStrEnd=ciStr;
            }
            //Check nStrLen for \d+Z? in 123Z
            if (AE_PatExec(hStack, lpREGroupNextNext, &ciStr, &ciMaxStr) && (lpREGroupNextNext->nStrLen || lpREGroupNextNext->nMinMatch))
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
            if (lpREGroupNextNext=PatNextGroupNoChild(lpREGroupNext))
            {
              if (AE_PatExec(hStack, lpREGroupNextNext, &ciStr, &ciMaxStr) && (lpREGroupNextNext->nStrLen || lpREGroupNextNext->nMinMatch))
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
          if (wpPat == lpREGroupItem->wpPatStart && !lpREGroupItem->parent &&
              ((lpREGroupNext->dwFlags & REGF_POSITIVEBACKWARD) || (lpREGroupNext->dwFlags & REGF_NEGATIVEBACKWARD)))
            ciStrStart=ciStr;
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
        if (hStack->dwOptions & REO_MULTILINE ? AEC_IsLastCharInLine(&ciStr) : !AEC_IsLastCharInFile(&ciStr))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }
      if (*wpPat == L'^')
      {
        //REO_MULTILINE
        if (!AEC_IsFirstCharInLine(&ciStr))
          goto EndLoop;
        ++wpPat;
        continue;
      }
      if (AEC_IndexCompare(&ciStr, &ciMaxStr) >= 0 && !PatIsInNonCapture(lpREGroupItem))
      {
        if (wpPat + 2 == wpMaxPat && *wpPat == L'\\')
        {
          if (*(wpPat + 1) == L'b' || *(wpPat + 1) == L'B')
          {
            if (AE_PatIsCharBoundary(&ciStr, hStack->wpDelim, hStack->wpMaxDelim))
            {
              if (*(wpPat + 1) == L'b')
                goto Match;
            }
            else if (*(wpPat + 1) == L'B')
              goto Match;
          }
          else if (*(wpPat + 1) == L'Z' || *(wpPat + 1) == L'z')
            goto Match;
        }
        nNegativeBackward=-1;
        goto EndLoop;
      }

      //Compare char
      nPatChar=0;

      if (*wpPat == L'.')
      {
        ////Any character except new line
        //nStrChar=AE_PatStrChar(&ciStr);
        //if (nStrChar < 0) goto EndLoop;
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
            if (!(dwCmpResult & RECCE_MIX))
            {
              ++wpPat;
              dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE)|RECCF_FULLSURROGATE, &nPatNextChar);
              if (dwCmpResult & RECCE_EQUAL)
                goto ClassMatch;
              if (nStrChar < 0) nStrChar=L'\n';

              //Check range
              if (!(dwCmpResult & RECCE_MIX))
              {
                if ((nStrChar >= nPatChar && nStrChar <= nPatNextChar) ||
                    (!(hStack->dwOptions & REO_MATCHCASE) && nStrChar <= MAXWORD &&
                      (((wchCaseChar=WideCharLower((wchar_t)nStrChar)) >= nPatChar && wchCaseChar <= nPatNextChar) ||
                       (wchCaseChar == nStrChar && (wchCaseChar=WideCharUpper((wchar_t)nStrChar)) >= nPatChar && wchCaseChar <= nPatNextChar))))
                {
                  goto ClassMatch;
                }
              }
            }
          }
          else
          {
            dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE)|RECCF_FULLSURROGATE, &nPatChar);
            if (dwCmpResult & RECCE_EQUAL)
              goto ClassMatch;
          }
        }
        if (!bExclude) goto EndLoop;
        goto NextChar;

        ClassMatch:
        if (bExclude) goto EndLoop;
        while (*++wpPat != L']')
        {
          if (*wpPat == L'\\')
            ++wpPat;
        }
      }
      else
      {
        nStrChar=AE_PatStrChar(&ciStr);
        dwCmpResult=PatCharCmp(&wpPat, nStrChar, (hStack->dwOptions & REO_MATCHCASE), &nPatChar);

        if (!(dwCmpResult & RECCE_EQUAL))
        {
          if (dwCmpResult & RECCE_DIF)
            goto EndLoop;

          if (dwCmpResult & RECCE_BOUNDARY)
          {
            if (AE_PatIsCharBoundary(&ciStr, hStack->wpDelim, hStack->wpMaxDelim))
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
          else if (dwCmpResult & RECCE_STRBEGIN)
          {
            if (!AEC_IndexCompare(&ciStr, &hStack->first->ciStrStart))
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_STREND)
          {
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_REF)
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

                if (((hStack->dwOptions & REO_MATCHCASE) || nStrChar > MAXWORD || nGroupChar > MAXWORD) ?
                     nStrChar != nGroupChar :
                     WideCharLower((wchar_t)nStrChar) != WideCharLower((wchar_t)nGroupChar))
                  goto EndLoop;

                AEC_NextChar(&ciStrCount);
                AEC_NextChar(&ciGroupCount);
                if (nStrChar <= MAXWORD)
                  nCount+=1;
                else
                  nCount+=2;
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
      NextChar:
      if (nPatChar <= MAXWORD)
      {
        nStrLen+=1;
        AE_PatNextChar(&ciStr);
      }
      else
      {
        nStrLen+=AEC_IndexLen(&ciStr);
        AEC_NextChar(&ciStr);
      }
      ++wpPat;
    }
    EndLoop:
    if (nCurMatch < lpREGroupItem->nMinMatch)
    {
      if (lpREGroupItem->dwFlags & REGF_NEGATIVEFORWARD)
      {
        lpREGroupItem->ciStrStart=ciMinStr;
        lpREGroupItem->ciStrEnd=ciMinStr;
        lpREGroupItem->nStrLen=0;
        return TRUE;
      }
      if (lpREGroupItem->dwFlags & REGF_NEGATIVEBACKWARD)
      {
        if (wpPat < lpREGroupItem->wpPatEnd && nNegativeBackward != -1)
        {
          ++nNegativeBackward;
          goto NextChar;
        }
        lpREGroupItem->ciStrStart=ciStr;
        lpREGroupItem->ciStrEnd=ciStr;
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
  if (lpREGroupItem->dwFlags & REGF_NEGATIVEFORWARD)
  {
    return FALSE;
  }
  if (lpREGroupItem->dwFlags & REGF_NEGATIVEBACKWARD)
  {
    if (nNegativeBackward)
    {
      lpREGroupItem->ciStrStart=ciStr;
      lpREGroupItem->ciStrEnd=ciStr;
      lpREGroupItem->nStrLen=0;
      return TRUE;
    }
    return FALSE;
  }
  if (lpREGroupItem->dwFlags & REGF_POSITIVEFORWARD)
  {
    lpREGroupItem->ciStrStart=ciMinStr;
    lpREGroupItem->ciStrEnd=ciMinStr;
    lpREGroupItem->nStrLen=0;
  }
  if (lpREGroupItem->dwFlags & REGF_POSITIVEBACKWARD)
  {
    lpREGroupItem->ciStrStart=ciStr;
    lpREGroupItem->ciStrEnd=ciStr;
    lpREGroupItem->nStrLen=0;
  }
  return TRUE;
}

int AE_PatStrChar(const AECHARINDEX *ciChar)
{
  return AEC_CharAtIndex(ciChar);
}

BOOL AE_PatIsCharBoundary(AECHARINDEX *ciChar, const wchar_t *wpDelim, const wchar_t *wpMaxDelim)
{
  AECHARINDEX ciPrevChar=*ciChar;
  BOOL bPrevCharDelim;

  AEC_PrevChar(&ciPrevChar);
  if (ciPrevChar.lpLine)
    bPrevCharDelim=PatIsCharDelim(AE_PatStrChar(&ciPrevChar), wpDelim, wpMaxDelim);
  else
    bPrevCharDelim=TRUE;

  if (PatIsCharDelim(AE_PatStrChar(ciChar), wpDelim, wpMaxDelim) != bPrevCharDelim)
    return TRUE;
  return FALSE;
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

void AE_PatReset(STACKREGROUP *hStack)
{
  REGROUP *lpREGroupItem;

  for (lpREGroupItem=hStack->first; lpREGroupItem; lpREGroupItem=PatNextGroup(lpREGroupItem))
  {
    xmemset(&lpREGroupItem->ciStrStart, 0, sizeof(AECHARRANGE));
  }
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
      if (pe->dwOptions & REPE_STARTLINEBEGIN)
        pe->lpREGroupStack->dwOptions|=REO_STARTLINEBEGIN;
      if (pe->dwOptions & REPE_ENDLINEFINISH)
        pe->lpREGroupStack->dwOptions|=REO_ENDLINEFINISH;
      if (pe->dwOptions & REPE_STARTBOUNDARY)
        pe->lpREGroupStack->dwOptions|=REO_STARTBOUNDARY;
      if (pe->dwOptions & REPE_ENDBOUNDARY)
        pe->lpREGroupStack->dwOptions|=REO_ENDBOUNDARY;
      pe->lpREGroupStack->wpDelim=pe->wpDelim;
      pe->lpREGroupStack->wpMaxDelim=pe->wpDelim?pe->wpMaxDelim:NULL;
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
    const wchar_t *wpStrNext=pe->wpStr;
    int nStrChar;

    while (wpStrNext <= pe->wpMaxStr)
    {
      if (bMatched=PatExec(pe->lpREGroupStack, lpREGroupRoot, wpStrNext, pe->wpMaxStr))
        ++nMatchCount;

      if (pe->lpCallback)
      {
        if (pe->lpCallback(pe, lpREGroupRoot, bMatched) == REPEC_STOP)
          return nMatchCount;
      }

      //Find next match
      if (!bMatched || (DWORD)nMatchCount >= (DWORD)nMaxMatchCount)
        break;
      pe->wpStr=lpREGroupRoot->wpStrEnd;
      wpStrNext=lpREGroupRoot->wpStrEnd;
      if (!lpREGroupRoot->nStrLen) //*(pe->wpMaxPat - 1) == L'^' or L'$'
        wpStrNext+=PatStrChar(wpStrNext, pe->wpMaxStr, &nStrChar) + 1;
      if (pe->dwOptions & REPE_MULTILINE)
      {
        if (*(wpStrNext - 1) == L'\n' || *(wpStrNext - 1) == L'\r')
          pe->lpREGroupStack->dwOptions|=REO_STARTLINEBEGIN;
        else
          pe->lpREGroupStack->dwOptions&=~REO_STARTLINEBEGIN;
      }

      PatReset(pe->lpREGroupStack);
    }
  }
  else
  {
    #ifdef __AKELEDIT_H__
    AECHARINDEX ciStrNext=pe->ciStr;

    while (AEC_IndexCompare(&ciStrNext, &pe->ciMaxStr) <= 0)
    {
      if (bMatched=AE_PatExec(pe->lpREGroupStack, lpREGroupRoot, &ciStrNext, &pe->ciMaxStr))
        ++nMatchCount;

      if (pe->lpCallback)
      {
        if (pe->lpCallback(pe, lpREGroupRoot, bMatched) == REPEC_STOP)
          return nMatchCount;
      }

      //Find next match
      if (!bMatched || (DWORD)nMatchCount >= (DWORD)nMaxMatchCount)
        break;
      pe->ciStr=lpREGroupRoot->ciStrEnd;
      ciStrNext=lpREGroupRoot->ciStrEnd;
      if (!lpREGroupRoot->nStrLen) //*(pe->wpMaxPat - 1) == L'^' or L'$'
        AEC_NextChar(&ciStrNext);

      AE_PatReset(pe->lpREGroupStack);
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
  if (!pr->wpStr)
  {
    #ifdef __AKELEDIT_H__
      pe.ciStr=pr->ciStr;
      pe.ciMaxStr=pr->ciMaxStr;
    #endif
  }
  pe.dwOptions=pr->dwOptions;
  pe.wpDelim=pr->wpDelim;
  pe.wpMaxDelim=pr->wpDelim?pr->wpMaxDelim:NULL;
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
      else if (nPatChar < REEC_WRONG)
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
      else if (nPatChar < REEC_WRONG)
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
