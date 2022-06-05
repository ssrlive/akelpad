/******************************************************************
 *                  RegExp functions header v2.5                  *
 *                                                                *
 * 2018 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)   *
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

//Global variables
#ifdef __AKELEDIT_H__
  extern AECHARINDEX RegExpGlobal_ciMaxStr;
#endif

//STACKREGROUP options
#define REO_MATCHCASE         0x0001 //Case-sensitive search.
#define REO_MULTILINE         0x0002 //Multiline search. Symbols ^ and $ specifies the line edge.
#define REO_WHOLEWORD         0x0004 //Whole word match.
#define REO_NONEWLINEDOT      0x0008 //Symbol . specifies any character except new line.
#define REO_REFEXIST          0x1000 //Internal.

//REGROUP flags
#define REGF_MATCHCASE        0x00000001 //Case-sensitive search.
#define REGF_MULTILINE        0x00000002 //Multiline search. Symbols ^ and $ specifies the line edge.
#define REGF_NONEWLINEDOT     0x00000008 //Symbol . specifies any character except new line.
#define REGF_ROOTITEM         0x00000100
#define REGF_ROOTANY          0x00000200
#define REGF_ROOTMULTILINE    0x00000400
#define REGF_AUTOGROUP        0x00001000
#define REGF_OR               0x00002000
#define REGF_ORPARENT         0x00004000
#define REGF_ONLYOPTIONS      0x00008000
#define REGF_POSITIVEFORWARD  0x00010000
#define REGF_NEGATIVEFORWARD  0x00020000
#define REGF_POSITIVEBACKWARD 0x00040000
#define REGF_NEGATIVEBACKWARD 0x00080000
#define REGF_NEGATIVEFIXED    0x00100000
#define REGF_ATOMIC           0x00200000
#define REGF_IFPARENT         0x00400000
#define REGF_IFCONDITION      0x00800000
#define REGF_IFTRUE           0x01000000
#define REGF_IFFALSE          0x02000000
#define REGF_REFEXIST         0x04000000
#define REGF_NOTFIXEDPARENT   0x08000000
#define REGF_GREEDY           0x10000000
#define REGF_POSSESSIVE       0x20000000
#define REGF_INVERTGREEDY     0x40000000

//PatExec return value
#define REE_INT_LASTGROUP     -2
#define REE_INT_DEFAULT       -1
#define REE_FALSE             0x0
#define REE_TRUE              0x1
#define REE_NEXTMATCH         0x2
#define REE_LASTGROUP         0x4
#define REE_ENDSTRING         0x8

//PatCharCmp flags
#define RECCF_MATCHCASE     0x01 //Case-sensitive search.
#define RECCF_FULLSURROGATE 0x02 //Check full surrogate character.

//PatCharCmp return value
#define RECCE_EQUAL         0x001
#define RECCE_DIF           0x002
#define RECCE_MIX           0x004
#define RECCE_BOUNDARY      0x008
#define RECCE_WORD          0x010
#define RECCE_STRBEGIN      0x020
#define RECCE_STREND        0x040
#define RECCE_RANGEBEGIN    0x080
#define RECCE_RANGEEND      0x100
#define RECCE_ROOTBEGIN     0x200
#define RECCE_REF           0x400

//PatEscChar return value
#define REEC_FIRST      (MAXLONG - 20)
#define REEC_WRONG      (REEC_FIRST + 0)
#define REEC_NEWLINE    (REEC_FIRST + 1)
#define REEC_WORD       (REEC_FIRST + 2)
#define REEC_BOUNDARY   (REEC_FIRST + 3)
#define REEC_STRBEGIN   (REEC_FIRST + 4)
#define REEC_STREND     (REEC_FIRST + 5)
#define REEC_RANGEBEGIN (REEC_FIRST + 6)
#define REEC_RANGEEND   (REEC_FIRST + 7)
#define REEC_ROOTBEGIN  (REEC_FIRST + 8)
#define REEC_REF        (REEC_FIRST + 9)
#define REEC_DIGIT      (REEC_FIRST + 10)
#define REEC_NONDIGIT   (REEC_FIRST + 11)
#define REEC_SPACE      (REEC_FIRST + 12)
#define REEC_NONSPACE   (REEC_FIRST + 13)

//PatStructExec options
#define RESE_MATCHCASE        0x0001 //Case-sensitive search.
#define RESE_MULTILINE        0x0002 //Multiline search. Symbols ^ and $ specifies the line edge.
#define RESE_WHOLEWORD        0x0004 //Whole word match.
#define RESE_NONEWLINEDOT     0x0008 //Symbol . specifies any character except new line.
#define RESE_GLOBAL           0x1000 //Search all possible occurrences. If not specified then find only first occurrence.
#define RESE_ISMATCH          0x2000 //Find first occurrence that should located at the beginning of the string. Cannot be combined with RESE_GLOBAL.

//PatStructExec callback return value
#define RESEC_CONTINUE  0  //Find next match.
#define RESEC_STOP      -1 //Stop search.

#ifndef __AKELEDIT_H__
  #define AELB_EOF      4  //End-of-file, last line in document.
  #define AELB_R        5  //"\r" new line.
  #define AELB_N        6  //"\n" new line.
  #define AELB_RN       7  //"\r\n" new line.
  #define AELB_RRN      8  //"\r\r\n" new line.

  //For alignment
  typedef struct {
    int nLine;
    void *lpLine;
    int nCharInLine;
  } AECHARINDEX;
#endif

#ifndef AEC_IsSurrogate
  #define AEC_IsSurrogate(c)  ((wchar_t)(c) >= 0xD800 && (wchar_t)(c) <= 0xDFFF)
#endif
#ifndef AEC_IsHighSurrogate
  #define AEC_IsHighSurrogate(c)  ((wchar_t)(c) >= 0xD800 && (wchar_t)(c) <= 0xDBFF)
#endif
#ifndef AEC_IsLowSurrogate
  #define AEC_IsLowSurrogate(c)  ((wchar_t)(c) >= 0xDC00 && (wchar_t)(c) <= 0xDFFF)
#endif
#ifndef AEC_ScalarFromSurrogate
  #define AEC_ScalarFromSurrogate(high, low)  ((((high) - 0xD800) * 0x400) + ((low) - 0xDC00) + 0x10000)
#endif
#ifndef AEC_HighSurrogateFromScalar
  #define AEC_HighSurrogateFromScalar(s)  ((wchar_t)((((s) - 0x10000) / 0x400) + 0xD800))
#endif
#ifndef AEC_LowSurrogateFromScalar
  #define AEC_LowSurrogateFromScalar(s)  ((wchar_t)((((s) - 0x10000) % 0x400) + 0xDC00))
#endif

typedef struct _REGROUP {
  struct _REGROUP *next;
  struct _REGROUP *prev;
  struct _REGROUP *parent;
  struct _REGROUP *firstChild;
  struct _REGROUP *lastChild;
  const wchar_t *wpPatStart;
  const wchar_t *wpPatEnd;
  INT_PTR nGroupLen;             //Number of characters in fixed length pattern or -1 if no fixed.
  const wchar_t *wpPatLeft;
  const wchar_t *wpPatRight;
  const wchar_t *wpStrStart;     //PatExec: Begin of matched string.
  const wchar_t *wpStrEnd;       //PatExec: End of matched string.
  AECHARINDEX ciStrStart;        //AE_PatExec: Begin of matched string.
  AECHARINDEX ciStrEnd;          //AE_PatExec: End of matched string.
  INT_PTR nStrLen;               //Matched string length.
  int nMinMatch;                 //Minimum group match.
  int nMaxMatch;                 //Maximum group match, -1 if unlimited.
  int nSelfMatch;                //Self parent: current recursive group match.
  int nSelfExec;                 //Self parent: count of recursive execution.
  DWORD dwFlags;                 //See REGF_* defines.
  int nIndex;                    //Group index, -1 if not captured.
  struct _REGROUP *conditionRef; //REGF_IFCONDITION group.
  UINT_PTR dwUserData;           //User data.
} REGROUP;

typedef struct _STACKREGROUP {
  REGROUP *first;                //Filled by PatCompile. Must be zero before passing to PatCompile.
  REGROUP *last;                 //Filled by PatCompile. Must be zero before passing to PatCompile.
  DWORD dwOptions;               //See REO_* defines.
  const wchar_t *wpDelim;        //List of delimiters. If NULL, default list will be used " \t\n".
  const wchar_t *wpMaxDelim;     //Pointer to the last character. If wpDelim is null-terminated, then wpMaxDelim is pointer to the NULL character.
  const wchar_t *wpText;         //PatExec: Text begin.
  const wchar_t *wpMaxText;      //PatExec: Text end.
  const wchar_t *wpRange;        //PatExec: Range begin (for \a).
  const wchar_t *wpMaxRange;     //PatExec: Range end (for \z).
  AECHARINDEX ciRange;           //AE_PatExec: Range begin (for \a).
  AECHARINDEX ciMaxRange;        //AE_PatExec: Range end (for \z).

  struct _STACKREGROUP *ref100;  //Pointer to a STACKREGROUP structure for backreferences >= 100. If not NULL, backreference \101 matches \1 in ref100, \102 matches \2 and so on. If NULL, not used.
  const wchar_t *wpRootStart;    //Internal (for \K).
  AECHARINDEX ciRootStart;       //Internal (for \K).
  INT_PTR nMaxBackward;          //Maximum backward group length. Filled by PatCompile.
  int nLastIndex;                //Last captured index. Filled by PatCompile.
  int nDeepness;                 //For PatExec/AE_PatExec debugging.
} STACKREGROUP;


typedef int (CALLBACK *PATEXECCALLBACK)(void *pe, REGROUP *lpREGroupRoot, BOOL bMatched);
//pe              Pointer to a PATEXEC structure.
//lpREGroupRoot   Pointer to a first REGROUP structure in stack (root group).
//bMatched        TRUE  - lpREGroupRoot->wpStrStart and lpREGroupRoot->wpStrEnd are valid.
//                FALSE - pe->wpStr is valid.
//Return Value
// See RESEC_* defines.

typedef struct {
  DWORD dwOptions;              //See RESE_* defines.
  const wchar_t *wpDelim;       //List of delimiters. If NULL, default list will be used " \t\n".
  const wchar_t *wpMaxDelim;    //Pointer to the last character. If wpDelim is null-terminated, then wpMaxDelim is pointer to the NULL character.
  const wchar_t *wpText;        //PatExec: Text begin. Valid if wpStr is not NULL.
  const wchar_t *wpMaxText;     //PatExec: Text end. Valid if wpStr is not NULL.
  const wchar_t *wpRange;       //PatExec: Range begin (for \a). Valid if wpStr is not NULL.
  const wchar_t *wpMaxRange;    //PatExec: Range end (for \z). Valid if wpStr is not NULL.
  AECHARINDEX ciRange;          //AE_PatExec: Range begin (for \a). Valid if wpStr is NULL.
  AECHARINDEX ciMaxRange;       //AE_PatExec: Range end (for \z). Valid if wpStr is NULL.

  STACKREGROUP *lpREGroupStack; //Groups stack. Must be zero if PatStructExec called for the first time.
  const wchar_t *wpPat;         //Pattern for process.
  const wchar_t *wpMaxPat;      //Pointer to the last character. If wpPat is null-terminated, then wpMaxPat is pointer to the NULL character.
  const wchar_t *wpStr;         //PatExec: String for process. If NULL, ciStr and ciMaxStr will be used.
  const wchar_t *wpMaxStr;      //PatExec: Pointer to the last character. If wpStr is null-terminated, then wpMaxStr is pointer to the NULL character.
  AECHARINDEX ciStr;            //AE_PatExec: First character for process. Used if wpStr is NULL.
  AECHARINDEX ciMaxStr;         //AE_PatExec: Last character at which processing is stopped. Used if wpStr is NULL.
  INT_PTR nErrorOffset;         //Contain wpPat offset, if error occurred during compile pattern.

  //Callback
  PATEXECCALLBACK lpCallback;   //Pointer to an PATEXECCALLBACK function.
  LPARAM lParam;                //Specifies an application-defined value.
} PATEXEC;

typedef struct {
  const wchar_t *wpStr;          //Points to any place of PATREPLACE.wpStr.
  AECHARINDEX ciStr;             //AE_PatExec: Points to any place of AkelEdit document.
  INT_PTR nShift;                //Shift point according to replace operation.
} PATREPLACEPOINT;

typedef struct {
  DWORD dwOptions;               //See RESE_* defines.
  const wchar_t *wpDelim;        //List of delimiters. If NULL, default list will be used " \t\n".
  const wchar_t *wpMaxDelim;     //Pointer to the last character. If wpDelim is null-terminated, then wpMaxDelim is pointer to the NULL character.
  const wchar_t *wpText;         //PatExec: Text begin. Valid if wpStr is not NULL.
  const wchar_t *wpMaxText;      //PatExec: Text end. Valid if wpStr is not NULL.
  const wchar_t *wpRange;        //PatExec: Range begin (for \a). Valid if wpStr is not NULL.
  const wchar_t *wpMaxRange;     //PatExec: Range end (for \z). Valid if wpStr is not NULL.
  AECHARINDEX ciRange;           //AE_PatExec: Range begin (for \a). Valid if wpStr is NULL.
  AECHARINDEX ciMaxRange;        //AE_PatExec: Range end (for \z). Valid if wpStr is NULL.

  const wchar_t *wpPat;          //Pattern for process.
  const wchar_t *wpMaxPat;       //Pointer to the last character. If wpPat is null-terminated, then wpMaxPat is pointer to the NULL character.
  const wchar_t *wpStr;          //PatExec: String for process. If NULL, ciStr and ciMaxStr will be used.
  const wchar_t *wpMaxStr;       //PatExec: Pointer to the last character. If wpStr is null-terminated, then wpMaxStr is pointer to the NULL character.
  AECHARINDEX ciStr;             //AE_PatExec: First character for process. Used if wpStr is NULL.
  AECHARINDEX ciMaxStr;          //AE_PatExec: Last character at which processing is stopped. Used if wpStr is NULL.
  const wchar_t *wpRep;          //String to replace with. Can be used "\n" or "\nn" - the n'th captured submatch.
  const wchar_t *wpMaxRep;       //Pointer to the last character. If wpRep is null-terminated, then wpMaxRep is pointer to the NULL character.
  const wchar_t *wpNewLine;      //New line string. If NULL, default will be used "\r\n".
  PATREPLACEPOINT *lpPointArray; //Pointer to an array of PATREPLACEPOINT.
  int nPointCount;               //Number of elements in lpPointArray. If zero, lpPointArray is ignored.
  INT_PTR nErrorOffset;          //Contain wpPat offset, if error occurred during compile pattern.
  int nReplaceCount;             //Receives replace count number.
  const wchar_t *wpLeftStr;      //PatExec: First replace occurrence in string.
  const wchar_t *wpRightStr;     //PatExec: Unmatched right part of string.
  AECHARINDEX ciLeftStr;         //AE_PatExec: First replace occurrence in string.
  AECHARINDEX ciRightStr;        //AE_PatExec: Unmatched right part of string.
  wchar_t *wszResult;            //Buffer that received replace result. If NULL, PatReplace returns required buffer size in characters.
} PATREPLACE;

typedef struct {
  const wchar_t *wpRep;
  const wchar_t *wpMaxRep;
  const wchar_t *wpNewLine;
  PATREPLACEPOINT *lpPointArray;
  int nPointCount;
  const wchar_t *wpRightStr;
  AECHARINDEX ciRightStr;
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
int PatEscChar(const wchar_t **wppPat);
DWORD PatCharCmp(const wchar_t **wppPat, int nStrChar, DWORD dwFlags, int *lpnPatChar);
BOOL PatIsCharDelim(int nChar, const wchar_t *wpDelim, const wchar_t *wpMaxDelim);
BOOL PatIsNextCharDelim(const wchar_t *wpChar, STACKREGROUP *hStack);
BOOL PatIsPrevCharDelim(const wchar_t *wpChar, STACKREGROUP *hStack);
BOOL PatIsCharBoundary(const wchar_t *wpChar, STACKREGROUP *hStack);
int PatRefIndex(const wchar_t **wppPat);
INT_PTR PatStrCmp(const wchar_t *wpStrStart1, const wchar_t *wpStrEnd1, DWORD dwFlags, const wchar_t *wpStrStart2, const wchar_t *wpMaxStr);
REGROUP* PatCloseGroups(REGROUP *lpREGroupItem, const wchar_t *wpPatEnd, const wchar_t *wpPatRight, REGROUP **lppREGroupNextAuto);
BOOL PatIsInNonCapture(REGROUP *lpREGroupItem);
REGROUP* PatGetGroup(STACKREGROUP *hStack, int nIndex);
REGROUP* PatGetMatchedGroup(STACKREGROUP *hStack, int nIndex);
REGROUP* PatNextGroup(REGROUP *lpREGroupItem);
REGROUP* PatNextGroupNoChild(REGROUP *lpREGroupItem);
REGROUP* PatNextGroupForExec(REGROUP *lpREGroupItem);
REGROUP* PatPrevGroup(REGROUP *lpREGroupItem);
INT_PTR PatGroupStr(PATGROUPSTR *pgs);
void PatReset(STACKREGROUP *hStack);
void PatFree(STACKREGROUP *hStack);

#ifdef __AKELEDIT_H__
  BOOL AE_PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, AECHARINDEX *ciInput, const AECHARINDEX *ciMaxInput);
  int AE_PatStrChar(const AECHARINDEX *ciChar);
  BOOL AE_PatIsNextCharDelim(const AECHARINDEX *ciChar, STACKREGROUP *hStack);
  BOOL AE_PatIsPrevCharDelim(const AECHARINDEX *ciChar, STACKREGROUP *hStack);
  BOOL AE_PatIsCharBoundary(const AECHARINDEX *ciChar, STACKREGROUP *hStack);
  AELINEDATA* AE_PatNextChar(AECHARINDEX *ciChar);
  INT_PTR AE_PatStrCmp(const AECHARINDEX *ciStrStart1, const AECHARINDEX *ciStrEnd1, DWORD dwFlags, const AECHARINDEX *ciStrStart2, AECHARINDEX *ciStrEnd2, const AECHARINDEX *ciMaxStr);
  INT_PTR AE_PatStrSub(const AECHARINDEX *ciStart, const AECHARINDEX *ciEnd);
  REGROUP* AE_PatNextGroupForExec(REGROUP *lpREGroupItem);
  void AE_PatReset(STACKREGROUP *hStack);
#endif

int PatStructExec(PATEXEC *pe);
void PatStructFree(PATEXEC *pe);
INT_PTR PatReplace(PATREPLACE *pr);
int CALLBACK PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched);
#ifdef __AKELEDIT_H__
  int CALLBACK AE_PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched);
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
  REGROUP *lpREGroupRef;
  REGROUP *lpREGroupNextAuto=NULL;
  const wchar_t *wpMinPat=wpPat;
  const wchar_t *wpClassStart=NULL;
  const wchar_t *wpClassEnd=NULL;
  const wchar_t *wpCharStart=NULL;
  const wchar_t *wpStrTmp;
  int nIndex=0;
  int nPatChar;
  int nPatRefIndex;
  BOOL bClassOpen=FALSE;
  BOOL bUnset;
  BOOL bOptions;

  if (wpPat == wpMaxPat)
    return 0;
  if (!hStack->wpDelim)
  {
    //Default delimiters list
    hStack->wpDelim=L" \t\n";
    hStack->wpMaxDelim=hStack->wpDelim + 3;
  }
  hStack->nMaxBackward=0;

  //Zero group is the all pattern
  if (!StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)NULL, (stack **)&lpREGroupItem, sizeof(REGROUP)))
  {
    lpREGroupItem->dwFlags=hStack->dwOptions & (REO_MATCHCASE|REO_MULTILINE|REO_NONEWLINEDOT);
    lpREGroupItem->dwFlags|=REGF_ROOTANY;

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
    lpREGroupItem->nGroupLen=0;
    lpREGroupItem->wpPatLeft=wpPat;
    lpREGroupItem->wpPatRight=wpMaxPat;
    lpREGroupItem->nIndex=0;
    lpREGroupItem->nMinMatch=1;
    lpREGroupItem->nMaxMatch=1;
  }

  while (wpPat < wpMaxPat)
  {
    if (lpREGroupNextAuto)
    {
      if (*wpPat != L')' &&
          *wpPat != L'*' &&
          *wpPat != L'+' &&
          *wpPat != L'?' &&
          *wpPat != L'{')
      {
        //"|" stops grouping
        if (*wpPat != L'|')
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
            lpREGroupNew->dwFlags=lpREGroupNextAuto->dwFlags & (REGF_MATCHCASE|REGF_MULTILINE|REGF_NONEWLINEDOT|REGF_INVERTGREEDY);
            lpREGroupNew->dwFlags|=REGF_AUTOGROUP;

            lpREGroupItem=lpREGroupNew;
          }
        }
        lpREGroupNextAuto=NULL;
      }
    }

    if (*wpPat == L'\\')
    {
      wpCharStart=wpPat;
      if (++wpPat >= wpMaxPat) goto Error;

      if (*wpPat == L'x')
      {
        if (++wpPat >= wpMaxPat) goto Error;

        if (*wpPat == L'{')
        {
          wpStrTmp=++wpPat;
          for (;;)
          {
            if (wpPat >= wpMaxPat) goto Error;
            if (*wpPat++ == L'}') break;
          }
          if (lpREGroupItem->nGroupLen != -1 && !bClassOpen)
          {
            nPatChar=(int)hex2decW(wpStrTmp, (wpPat - 1) - wpStrTmp, NULL);
            if (nPatChar == -1)
            {
              wpPat=wpStrTmp;
              goto Error;
            }
            if (nPatChar <= MAXWORD)
              lpREGroupItem->nGroupLen+=1;
            else
              lpREGroupItem->nGroupLen+=2;
          }
        }
        else
        {
          if (wpPat + 2 > wpMaxPat)
            goto Error;
          wpPat+=2;
          if (lpREGroupItem->nGroupLen != -1 && !bClassOpen)
            ++lpREGroupItem->nGroupLen;
        }
      }
      else if (*wpPat == L'u')
      {
        if (wpPat + 5 > wpMaxPat)
          goto Error;
        wpPat+=5;
        if (lpREGroupItem->nGroupLen != -1 && !bClassOpen)
          ++lpREGroupItem->nGroupLen;
      }
      else if (*wpPat >= L'0' && *wpPat <= L'9')
      {
        nPatRefIndex=PatRefIndex(&wpPat);
        hStack->nLastIndex=nIndex;

        if (nPatRefIndex > 0 && (lpREGroupRef=PatGetGroup(hStack, nPatRefIndex)))
        {
          lpREGroupRef->dwFlags|=REGF_REFEXIST;
          hStack->dwOptions|=REO_REFEXIST;
          if (lpREGroupItem->nGroupLen != -1)
          {
            if (lpREGroupRef->nGroupLen != -1)
              lpREGroupItem->nGroupLen+=lpREGroupRef->nGroupLen;
            else
            {
              lpREGroupItem->nGroupLen=-1;
              lpREGroupItem->dwFlags|=REGF_NOTFIXEDPARENT;
            }
          }
        }
        else
        {
          wpPat=wpCharStart;
          goto Error;
        }
        if (lpREGroupItem->dwFlags & (REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD))
          goto Error;
      }
      else if (*wpPat == L'A')
      {
        if (lpREGroupItem->wpPatStart != wpPat - 1)
          goto Error;
        ++wpPat;
      }
      else if (*wpPat == L'Z')
      {
        if (wpPat + 1 < wpMaxPat && *(wpPat + 1) != L')')
          goto Error;
        ++wpPat;
      }
      else if (*wpPat == L'a' || *wpPat == L'z' ||
               *wpPat == L'B' || *wpPat == L'b')
      {
        ++wpPat;
      }
      else if (*wpPat == L'K')
      {
        ++wpPat;
        if (bClassOpen)
          goto Error;
        if (hStack->first->nGroupLen != -1)
          hStack->first->nGroupLen=0;
      }
      else
      {
        ++wpPat;
        if (lpREGroupItem->nGroupLen != -1 && !bClassOpen)
          ++lpREGroupItem->nGroupLen;
      }
      continue;
    }
    if (*wpPat == L'^' ||
        *wpPat == L'$')
    {
      ++wpPat;
      continue;
    }
    if (*wpPat == L']')
    {
      if (!bClassOpen)
        goto Error;
      if (!wpCharStart || *wpCharStart == L'-' ||
          (*wpCharStart == L'^' && wpCharStart + 1 == wpPat))
        goto Error;
      bClassOpen=FALSE;
      wpClassEnd=++wpPat;
      if (lpREGroupItem->nGroupLen != -1)
        ++lpREGroupItem->nGroupLen;
      continue;
    }
    if (bClassOpen)
    {
      if (*wpPat == L'[')
        goto Error;
      wpCharStart=wpPat++;
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
      if (!(lpREGroupItem->dwFlags & REGF_OR))
      {
        //Set REGF_ORPARENT because PatCloseGroups change wpPatEnd:
        //str - "123", find "(\d{6}|\d{3})", replace - "x"
        lpREGroupItem->dwFlags|=REGF_ORPARENT;
      }
      lpREGroupItem=PatCloseGroups(lpREGroupItem, wpPat, wpPat + 1, &lpREGroupNextAuto);

      //100|ABC(200|300|400)
      if (lpREGroupItem->dwFlags & REGF_OR)
      {
        //300
        if (!lpREGroupItem->wpPatEnd)
          lpREGroupItem->wpPatEnd=wpPat;
        lpREGroupItem->wpPatRight=wpPat + 1;
        lpREGroupItem=lpREGroupItem->parent;
      }
      else if (!lpREGroupItem->firstChild || !(lpREGroupItem->firstChild->dwFlags & (REGF_OR|REGF_IFCONDITION)))
      {
        //100 or 200
        if (lpREGroupOr=(REGROUP *)GlobalAlloc(GPTR, sizeof(REGROUP)))
        {
          lpREGroupOr->parent=lpREGroupItem;
          lpREGroupOr->wpPatLeft=lpREGroupItem->wpPatStart;
          lpREGroupOr->wpPatStart=lpREGroupItem->wpPatStart;
          lpREGroupOr->wpPatEnd=wpPat;
          lpREGroupOr->nGroupLen=lpREGroupItem->nGroupLen;
          lpREGroupOr->wpPatRight=wpPat + 1;
          lpREGroupOr->nMinMatch=1;
          lpREGroupOr->nMaxMatch=1;
          lpREGroupOr->nIndex=-1;
          lpREGroupOr->dwFlags=lpREGroupItem->dwFlags & (REGF_MATCHCASE|REGF_MULTILINE|REGF_NONEWLINEDOT|REGF_INVERTGREEDY|REGF_ATOMIC);
          lpREGroupOr->dwFlags|=REGF_OR;
          //Always positive in alternations
          if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD))
            lpREGroupOr->dwFlags|=REGF_POSITIVEFORWARD;

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

        PatCloseGroups(lpREGroupOr, wpPat, wpPat + 1, &lpREGroupNextAuto);
        lpREGroupItem->nGroupLen=-1;
        lpREGroupItem->dwFlags|=REGF_NOTFIXEDPARENT;
        if (lpREGroupItem->dwFlags & (REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD|REGF_NEGATIVEFIXED))
          goto Error;
      }

      if (!StackInsertBefore((stack **)&lpREGroupItem->firstChild, (stack **)&lpREGroupItem->lastChild, (stack *)NULL, (stack **)&lpREGroupNew, sizeof(REGROUP)))
      {
        lpREGroupNew->parent=lpREGroupItem;
        lpREGroupNew->wpPatLeft=++wpPat;
        lpREGroupNew->wpPatStart=wpPat;
        lpREGroupNew->nMinMatch=1;
        lpREGroupNew->nMaxMatch=1;
        lpREGroupNew->nIndex=-1;
        lpREGroupNew->dwFlags=lpREGroupItem->dwFlags & (REGF_MATCHCASE|REGF_MULTILINE|REGF_NONEWLINEDOT|REGF_INVERTGREEDY|REGF_ATOMIC);
        lpREGroupNew->dwFlags|=REGF_OR;
        //Always positive in alternations
        if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD))
          lpREGroupNew->dwFlags|=REGF_POSITIVEFORWARD;

        lpREGroupItem=lpREGroupNew;
      }
      wpCharStart=NULL;
      continue;
    }
    else if (*wpPat == L')')
    {
      lpREGroupItem=PatCloseGroups(lpREGroupItem, wpPat, wpPat + 1, &lpREGroupNextAuto);
      if (lpREGroupItem->dwFlags & REGF_OR)
      {
        if (!lpREGroupItem->wpPatEnd)
          lpREGroupItem->wpPatEnd=wpPat;
        lpREGroupItem->wpPatRight=wpPat;
        lpREGroupItem=lpREGroupItem->parent;
        if (!(lpREGroupItem->dwFlags & REGF_ATOMIC))
        {
          //str - "abc", find "a(bc|b)c"
          lpREGroupNextAuto=lpREGroupItem;
        }

        //Close groups for parent
        lpREGroupItem=PatCloseGroups(lpREGroupItem, wpPat, wpPat + 1, &lpREGroupNextAuto);
      }
      if (!lpREGroupItem->parent)
        goto Error;
      if (lpREGroupItem->dwFlags & (REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD|REGF_NEGATIVEFIXED))
      {
        if (lpREGroupItem->nGroupLen <= 0)
        {
          if (lpREGroupItem->wpPatEnd)
            wpPat=lpREGroupItem->wpPatEnd;
          goto Error;
        }
        if (lpREGroupItem->dwFlags & (REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD))
        {
          if (lpREGroupItem->nGroupLen > hStack->nMaxBackward)
            hStack->nMaxBackward=lpREGroupItem->nGroupLen;
        }
      }
      if (!lpREGroupItem->wpPatEnd)
        lpREGroupItem->wpPatEnd=wpPat;
      lpREGroupItem->wpPatRight=wpPat + 1;
      if (!(lpREGroupItem->dwFlags & REGF_ONLYOPTIONS) && lpREGroupItem->wpPatStart == lpREGroupItem->wpPatEnd)
        goto Error;

      if (lpREGroupItem->nGroupLen == -1)
      {
        //str - "1.2", find "(-?\d+)\.\d+"
        lpREGroupNextAuto=lpREGroupItem;
      }
      if (lpREGroupItem->parent->nGroupLen != -1)
      {
        if (lpREGroupItem->nGroupLen != -1)
        {
          if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD|REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD))
          {
            //Don't count non-capture group.
          }
          else lpREGroupItem->parent->nGroupLen+=lpREGroupItem->nGroupLen;
        }
        else lpREGroupItem->parent->nGroupLen=-1;
      }
      if (lpREGroupItem->parent->nGroupLen == -1)
        lpREGroupItem->dwFlags|=REGF_NOTFIXEDPARENT;

      if ((lpREGroupItem->parent->dwFlags & REGF_IFPARENT) && lpREGroupItem->parent->firstChild == lpREGroupItem->parent->lastChild)
      {
        nPatRefIndex=(int)xatoiW(lpREGroupItem->wpPatStart, &wpStrTmp);
        hStack->nLastIndex=nIndex;

        if (nPatRefIndex > 0 && wpStrTmp == lpREGroupItem->wpPatEnd)
        {
          if (!(lpREGroupItem->conditionRef=PatGetGroup(hStack, nPatRefIndex)))
            goto Error;
        }
        else
        {
          if (!(lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD|REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD)))
            goto Error;
        }
        lpREGroupItem->dwFlags|=REGF_IFCONDITION;
        lpREGroupNextAuto=lpREGroupItem;
      }
      else if (lpREGroupItem->dwFlags & REGF_IFPARENT)
      {
        if (!lpREGroupItem->firstChild || !lpREGroupItem->firstChild->next)
          goto Error;
        if (lpREGroupItem->firstChild->next->dwFlags & REGF_OR)
        {
          //(?(condition)|pattern-false)
          lpREGroupItem->firstChild->next->dwFlags|=REGF_IFFALSE;
          if (lpREGroupItem->firstChild->next->next)
            goto Error;
        }
        else
        {
          //(?(condition)pattern-true)
          lpREGroupItem->firstChild->next->dwFlags|=REGF_IFTRUE;
          if (lpREGroupItem->firstChild->next->next)
          {
            //(?(condition)pattern-true|pattern-false)
            lpREGroupItem->firstChild->next->next->dwFlags|=REGF_IFFALSE;
            if (lpREGroupItem->firstChild->next->next->next)
              goto Error;
          }
        }
      }
      lpREGroupItem=lpREGroupItem->parent;

      ++wpPat;
      continue;
    }
    //Multipliers
    else if (*wpPat == L'*' ||
             *wpPat == L'+' ||
             *wpPat == L'?' ||
             *wpPat == L'{')
    {
      if (lpREGroupItem->lastChild && wpPat == lpREGroupItem->lastChild->wpPatRight && *(lpREGroupItem->lastChild->wpPatRight - 1) == L')')
      {
        //(...)* or (...)+ or (...){1,1}
        lpREGroupNew=lpREGroupItem->lastChild;

        if (lpREGroupNew->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD|REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD))
          goto Error;
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
          lpREGroupNew->nMinMatch=1;
          lpREGroupNew->nMaxMatch=1;
          lpREGroupNew->nGroupLen=1;
          lpREGroupNew->nIndex=-1;
          lpREGroupNew->dwFlags=lpREGroupItem->dwFlags & (REGF_MATCHCASE|REGF_MULTILINE|REGF_NONEWLINEDOT|REGF_INVERTGREEDY);
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
          lpREGroupNew->nMinMatch=1;
          lpREGroupNew->nMaxMatch=1;
          lpREGroupNew->nGroupLen=1;
          lpREGroupNew->nIndex=-1;
          lpREGroupNew->dwFlags=lpREGroupItem->dwFlags & (REGF_MATCHCASE|REGF_MULTILINE|REGF_NONEWLINEDOT|REGF_INVERTGREEDY);
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
        lpREGroupNew->nMinMatch*=1;
        lpREGroupNew->nMaxMatch=-1;
      }
      else if (*wpPat == L'?')
      {
        lpREGroupNew->nMinMatch=0;
        lpREGroupNew->nMaxMatch*=1;
      }
      else if (*wpPat == L'{')
      {
        lpREGroupNew->nMinMatch*=(int)xatoiW(++wpPat, &wpPat);
        if (*wpPat == L'}')
          lpREGroupNew->nMaxMatch=lpREGroupNew->nMinMatch;
        else if (*wpPat == L',')
        {
          if (*++wpPat == L'}')
            lpREGroupNew->nMaxMatch=-1;
          else
            lpREGroupNew->nMaxMatch*=(int)xatoiW(wpPat, &wpPat);
        }
        else goto Error;
      }
      if ((DWORD)lpREGroupNew->nMinMatch > (DWORD)lpREGroupNew->nMaxMatch)
        goto Error;
      if (!lpREGroupNew->nMinMatch && !lpREGroupNew->nMaxMatch)
        goto Error;
      if (lpREGroupNew->nMinMatch != 1 || lpREGroupNew->nMaxMatch != 1)
        lpREGroupNextAuto=lpREGroupNew;
      wpCharStart=NULL;

      //Greedy
      if (lpREGroupNew->nMinMatch != lpREGroupNew->nMaxMatch)
        lpREGroupNew->dwFlags|=REGF_GREEDY;
      if (*++wpPat == L'?')
      {
        //Lazy
        lpREGroupNew->dwFlags&=~REGF_GREEDY;
        ++wpPat;
      }
      else if (*wpPat == L'+')
      {
        //Possessive
        lpREGroupNew->dwFlags|=REGF_POSSESSIVE;
        ++wpPat;
      }
      if (lpREGroupNew->dwFlags & REGF_INVERTGREEDY)
        lpREGroupNew->dwFlags^=REGF_GREEDY;

      lpREGroupNew->wpPatRight=wpPat;
      if (lpREGroupNew->nMinMatch == lpREGroupNew->nMaxMatch && lpREGroupItem->nGroupLen != -1 && lpREGroupNew->nGroupLen != -1)
      {
        lpREGroupItem->nGroupLen+=lpREGroupNew->nGroupLen * (lpREGroupNew->nMinMatch - 1);
        lpREGroupNew->nGroupLen=lpREGroupNew->nGroupLen * lpREGroupNew->nMinMatch;
      }
      else
      {
        lpREGroupItem->nGroupLen=-1;
        lpREGroupItem->dwFlags|=REGF_NOTFIXEDPARENT;
        lpREGroupNew->nGroupLen=-1;
        lpREGroupNew->dwFlags|=REGF_NOTFIXEDPARENT;
      }
      if (lpREGroupItem->dwFlags & (REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD|REGF_NEGATIVEFIXED))
      {
        wpPat=lpREGroupNew->wpPatEnd;
        goto Error;
      }
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
        lpREGroupNew->nMinMatch=1;
        lpREGroupNew->nMaxMatch=1;
        if (lpREGroupItem->dwFlags & REGF_IFPARENT)
          lpREGroupNew->nIndex=-1;
        else
          lpREGroupNew->nIndex=++nIndex;
        lpREGroupNew->dwFlags=lpREGroupItem->dwFlags & (REGF_MATCHCASE|REGF_MULTILINE|REGF_NONEWLINEDOT|REGF_INVERTGREEDY);

        if (*wpPat == L'?')
        {
          --nIndex;
          lpREGroupNew->nIndex=-1;
          ++wpPat;

          //Options
          bOptions=FALSE;
          bUnset=FALSE;

          for (;;)
          {
            if (*wpPat == L'-')
            {
              ++wpPat;
              bUnset=TRUE;
            }
            if (*wpPat == L'i')
            {
              if (bUnset)
                lpREGroupNew->dwFlags|=REGF_MATCHCASE;
              else
                lpREGroupNew->dwFlags&=~REGF_MATCHCASE;
              bOptions=TRUE;
            }
            else if (*wpPat == L'm')
            {
              if (bUnset)
                lpREGroupNew->dwFlags&=~REGF_MULTILINE;
              else
                lpREGroupNew->dwFlags|=REGF_MULTILINE;
              bOptions=TRUE;
            }
            else if (*wpPat == L's')
            {
              if (bUnset)
                lpREGroupNew->dwFlags|=REGF_NONEWLINEDOT;
              else
                lpREGroupNew->dwFlags&=~REGF_NONEWLINEDOT;
              bOptions=TRUE;
            }
            else if (*wpPat == L'U')
            {
              if (bUnset)
                lpREGroupNew->dwFlags&=~REGF_INVERTGREEDY;
              else
                lpREGroupNew->dwFlags|=REGF_INVERTGREEDY;
              bOptions=TRUE;
            }
            else break;

            if (++wpPat >= wpMaxPat) goto Error;
          }
          if (bOptions && *wpPat == L')')
          {
            lpREGroupNew->dwFlags|=REGF_ONLYOPTIONS;
            lpREGroupNew->wpPatStart=wpPat;
            lpREGroupItem=lpREGroupNew;
            lpREGroupNextAuto=lpREGroupNew;
            continue;
          }

          if (*wpPat == L':')
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
          else if (*wpPat == L'>')
          {
            //Atomic group
            lpREGroupNew->dwFlags|=REGF_ATOMIC;
          }
          else if (*wpPat == L'^')
          {
            lpREGroupNew->nIndex=++nIndex;

            //Capture negative group
            lpREGroupNew->dwFlags|=REGF_NEGATIVEFIXED;
          }
          else if (*wpPat == L'(')
          {
            --wpPat;

            //Conditional group
            lpREGroupNew->dwFlags|=REGF_IFPARENT;
          }
          else goto Error;

          lpREGroupNew->wpPatStart=++wpPat;
        }
        lpREGroupItem=lpREGroupNew;
        continue;
      }
    }
    else
    {
      wpCharStart=wpPat;
      if (lpREGroupItem->nGroupLen != -1)
        ++lpREGroupItem->nGroupLen;
    }
    ++wpPat;
  }
  if (wpClassStart > wpClassEnd)
  {
    wpPat=wpClassStart;
    goto Error;
  }
  //".*123)"
  if (!lpREGroupItem)
    goto Error;

  lpREGroupItem=PatCloseGroups(lpREGroupItem, wpPat, wpPat, NULL);
  if (lpREGroupItem->dwFlags & REGF_OR)
  {
    if (!lpREGroupItem->wpPatEnd)
      lpREGroupItem->wpPatEnd=wpPat;
    lpREGroupItem->wpPatRight=wpPat;
    lpREGroupItem=lpREGroupItem->parent;
  }
  if (lpREGroupItem->parent)
    goto Error;
  hStack->nLastIndex=nIndex;
  #ifdef _DEBUG
    hStack->nDeepness=0;
  #endif
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
  REGROUP *lpREGroupOrMatch;
  const wchar_t *wpPat;
  const wchar_t *wpMaxPat=lpREGroupItem->wpPatEnd;
  const wchar_t *wpPatChar=NULL;
  const wchar_t *wpMinStr=wpStr;
  const wchar_t *wpStrStart=wpStr;
  const wchar_t *wpNextGroup;
  const wchar_t *wpGreedyStrEnd=NULL;
  const wchar_t *wpBackupStrStart;
  const wchar_t *wpBackupStrEnd;
  INT_PTR nBackupStrLen;
  INT_PTR nPrevStrLen;
  INT_PTR nRefLen;
  INT_PTR nGreedyStrLen=0;
  int nStrChar;
  int nCharSize=0;
  int nPatChar;
  int nPatNextChar;
  wchar_t wchCaseChar;
  int nCurMatch;
  int nRefIndex;
  DWORD dwCmpResult=0;
  int nNextMatched=REE_INT_DEFAULT;
  BOOL bMatched;
  BOOL bLastMatched;
  BOOL bExclude;

  #ifdef _DEBUG
    ++hStack->nDeepness;
  #endif

  if (lpREGroupItem->dwFlags & REGF_ROOTITEM)
  {
    nCurMatch=0;
    lpREGroupItem->wpStrStart=wpStr;
    lpREGroupItem->wpStrEnd=wpStr;
    lpREGroupItem->nStrLen=0;
    lpREGroupItem->nSelfMatch=0;
    hStack->wpRootStart=NULL;

    if (lpREGroupItem->dwFlags & REGF_ROOTANY)
    {
      BOOL bResult=REE_FALSE;

      //Turn off REGF_ROOTANY and execute itself
      lpREGroupItem->dwFlags&=~REGF_ROOTITEM & ~REGF_ROOTANY;

      while (wpStr <= wpMaxStr)
      {
        if (PatExec(hStack, lpREGroupItem, wpStr, wpMaxStr))
        {
          if (hStack->dwOptions & REO_WHOLEWORD)
          {
            if (!PatIsPrevCharDelim(lpREGroupItem->wpStrStart, hStack) ||
                !PatIsNextCharDelim(lpREGroupItem->wpStrEnd, hStack))
              goto RootReset;
          }
          bResult=REE_TRUE;
          break;

          RootReset:
          lpREGroupItem->wpStrStart=wpStr;
          lpREGroupItem->wpStrEnd=wpStr;
          lpREGroupItem->nSelfMatch=0;
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
  else
  {
    if (lpREGroupItem->nSelfExec)
    {
      nCurMatch=lpREGroupItem->nSelfMatch;
      //str - "abc", find "($a*)+"
      if (wpStr <= lpREGroupItem->wpStrStart)
        goto EndLoopAfterNegativeFixed;
    }
    else nCurMatch=0;

    if (lpREGroupItem->dwFlags & (REGF_NEGATIVEBACKWARD|REGF_POSITIVEBACKWARD))
    {
      //Find start position
      for (nPrevStrLen=lpREGroupItem->nGroupLen; nPrevStrLen > 0 && --wpStr >= hStack->wpText; --nPrevStrLen)
      {
        if (*wpStr == L'\n')
        {
          if (wpStr - 1 >= hStack->wpText && *(wpStr - 1) == L'\r')
          {
            if (wpStr - 2 >= hStack->wpText && *(wpStr - 2) == L'\r')
            {
              //\r\r\n
              wpStr-=2;
            }
            else
            {
              //\r\n
              wpStr-=1;
            }
          }
        }
      }
      if (nPrevStrLen) goto EndLoopAfterNegativeFixed;
    }
    if (!lpREGroupItem->nMinMatch &&
        //str - "123", find "(?>\d+?)3"
        !(lpREGroupItem->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE)))
    {
      nPrevStrLen=-1;
      lpREGroupItem->wpStrStart=wpStr;
      lpREGroupItem->wpStrEnd=wpStr;
      lpREGroupItem->nStrLen=0;
      goto MatchNext;
    }
    lpREGroupItem->nStrLen=0;
  }

  BeginLoop:
  if ((DWORD)nCurMatch < (DWORD)lpREGroupItem->nMaxMatch)
  {
    wpPat=lpREGroupItem->wpPatStart;
    if (lpREGroupNext=lpREGroupItem->firstChild)
      wpNextGroup=lpREGroupNext->wpPatLeft;
    else
      wpNextGroup=NULL;

    for (;;)
    {
      wpPatChar=wpPat;

      if (wpPat >= wpMaxPat)
      {
        if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD|REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD|REGF_NEGATIVEFIXED))
        {
          if (lpREGroupItem->dwFlags & REGF_NEGATIVEFIXED)
            goto MismatchFixed;
          ++nCurMatch;
          goto EndLoop;
        }
        MatchFixed:
        ++nCurMatch;
        nPrevStrLen=lpREGroupItem->nStrLen;
        lpREGroupItem->wpStrStart=wpStrStart;
        lpREGroupItem->wpStrEnd=wpStr;
        lpREGroupItem->nStrLen=wpStr - wpStrStart;

        if (((nCurMatch >= lpREGroupItem->nMinMatch &&
             (DWORD)nCurMatch <= (DWORD)lpREGroupItem->nMaxMatch &&
             (lpREGroupItem->dwFlags & REGF_NOTFIXEDPARENT)) ||
            //str - "abc", find "a(bc|b)c"
            (lpREGroupItem->dwFlags & REGF_OR)) &&
            //str - "123", find "(?>\d+?)3"
            !(lpREGroupItem->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE)))
        {
          MatchNext:
          if (lpREGroupNextNext=PatNextGroupForExec(lpREGroupItem))
          {
            if (lpREGroupNextNext->nSelfExec)
            {
              //Save data before PatExec
              wpBackupStrStart=lpREGroupNextNext->wpStrStart;
              wpBackupStrEnd=lpREGroupNextNext->wpStrEnd;
              nBackupStrLen=lpREGroupNextNext->nStrLen;

              nNextMatched=PatExec(hStack, lpREGroupNextNext, wpStr, wpMaxStr);

              //Restore data after PatExec
              lpREGroupItem->wpStrStart=wpStrStart;
              lpREGroupItem->wpStrEnd=wpStr;
              lpREGroupItem->nStrLen=wpStr - wpStrStart;

              if (wpBackupStrStart < lpREGroupNextNext->wpStrStart)
                lpREGroupNextNext->wpStrStart=wpBackupStrStart;
              if (wpBackupStrEnd > lpREGroupNextNext->wpStrEnd)
                lpREGroupNextNext->wpStrEnd=wpBackupStrEnd;
              lpREGroupNextNext->nStrLen=lpREGroupNextNext->wpStrEnd - lpREGroupNextNext->wpStrStart;

              --lpREGroupNextNext->nSelfMatch;
              --lpREGroupNextNext->nSelfExec;
            }
            else
            {
              nNextMatched=PatExec(hStack, lpREGroupNextNext, wpStr, wpMaxStr);
            }

            //Check nStrLen for \d+Z? in 123Z
            if (nNextMatched && (lpREGroupNextNext->nStrLen || lpREGroupNextNext->nMinMatch))
            {
              if (!(lpREGroupItem->dwFlags & REGF_GREEDY))
                goto EndLoop;
              wpGreedyStrEnd=wpStr;
              nGreedyStrLen=lpREGroupItem->nStrLen;
            }
            ////str - "long string", find - "([^#]*@?)*"
            //if (nNextMatched & REE_ENDSTRING)
            //  goto EndLoop;
          }
          else nNextMatched=REE_INT_LASTGROUP;
        }
        else nNextMatched=REE_INT_DEFAULT;

        //str - "abc", find ".*$"
        if (wpStr >= wpMaxStr)
          goto EndLoop;
        //Prevent infinite loop: str - "yAy", find "((y)?)*"
        if (lpREGroupItem->nStrLen == nPrevStrLen)
          goto EndLoop;
        goto BeginLoop;
      }
      if (wpPat == wpNextGroup)
      {
        if (lpREGroupNext->dwFlags & REGF_ONLYOPTIONS)
          goto NextGroup;
        bMatched=FALSE;
        lpREGroupOrMatch=NULL;
        goto FirstCheck;

        for (;;)
        {
          if (!bMatched || (!(bMatched & (REE_NEXTMATCH|REE_LASTGROUP)) && !(lpREGroupNext->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE))))
          {
            FirstCheck:
            if (lpREGroupItem->dwFlags & REGF_REFEXIST)
            {
              //Set wpStrStart for possible backreferences in child PatExec.
              lpREGroupItem->wpStrStart=wpStrStart;
            }
            //str - "a1a2a3a4", find "(a.*?){2}"
            //str - "a1a2a3a4", find "(a.*?){2}\w"
            lpREGroupItem->nSelfMatch=nCurMatch;
            lpREGroupNext->wpStrStart=wpStr;
            lpREGroupNext->nSelfMatch=0;

            if ((lpREGroupItem->dwFlags & REGF_IFPARENT) || (lpREGroupNext->dwFlags & REGF_IFPARENT))
            {
              //str - "abc", find - "(a)(?(1)bc)"
              if (lpREGroupItem->dwFlags & REGF_IFPARENT)
                lpREGroupNext=lpREGroupItem;

              if (lpREGroupNext->firstChild->conditionRef ?
                    lpREGroupNext->firstChild->conditionRef->nStrLen :
                    PatExec(hStack, lpREGroupNext->firstChild, wpStr, wpMaxStr))
              {
                if (lpREGroupNext->firstChild->next->dwFlags & REGF_IFFALSE)
                  goto NextGroup;
                lpREGroupNextNext=lpREGroupNext->firstChild->next;
              }
              else
              {
                if (lpREGroupNext->firstChild->next->next)
                  lpREGroupNextNext=lpREGroupNext->firstChild->next->next;
                else if (lpREGroupNext->firstChild->next->dwFlags & REGF_IFFALSE)
                  lpREGroupNextNext=lpREGroupNext->firstChild->next;
                else
                  goto NextGroup;
              }
              if (!PatExec(hStack, lpREGroupNextNext, wpStr, wpMaxStr))
              {
                if (lpREGroupNext->dwFlags & REGF_OR)
                  goto NextOR;
                goto EndLoop;
              }
              lpREGroupNext->wpStrStart=lpREGroupNextNext->wpStrStart;
              lpREGroupNext->wpStrEnd=lpREGroupNextNext->wpStrEnd;
              lpREGroupNext->nStrLen=lpREGroupNextNext->nStrLen;
              lpREGroupNext->nSelfMatch=lpREGroupNextNext->nSelfMatch;
              wpStr=lpREGroupNextNext->wpStrEnd;
              if (lpREGroupNext->dwFlags & REGF_OR)
              {
                bMatched=TRUE;
                goto NextOR;
              }
              goto NextGroup;
            }

            if (!(bLastMatched=PatExec(hStack, lpREGroupNext, wpStr, wpMaxStr)))
            {
              if (lpREGroupNext->dwFlags & REGF_OR)
                goto NextOR;
              goto EndLoop;
            }
            if (lpREGroupNext->dwFlags & REGF_OR)
            {
              if (!lpREGroupOrMatch || lpREGroupNext->nStrLen < lpREGroupOrMatch->nStrLen || (bLastMatched & (REE_NEXTMATCH|REE_LASTGROUP)))
                lpREGroupOrMatch=lpREGroupNext;
              bMatched=bLastMatched;
              goto NextOR;
            }
            wpStr=lpREGroupNext->wpStrEnd;
            goto NextGroup;
          }
          NextOR:
          if (lpREGroupNext->next && (lpREGroupNext->next->dwFlags & REGF_OR))
            lpREGroupNext=lpREGroupNext->next;
          else
            break;
        }
        if (bMatched)
        {
          if (lpREGroupItem->dwFlags & REGF_NEGATIVEFORWARD)
            goto ReturnFalse;
          if (lpREGroupOrMatch)
            wpStr=lpREGroupOrMatch->wpStrEnd;
        }
        else goto EndLoop;

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
        if (wpStr == hStack->wpMaxText || ((lpREGroupItem->dwFlags & REGF_MULTILINE) && (*wpStr == L'\n' || *wpStr == L'\r')))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }
      if (*wpPat == L'^')
      {
        if (wpStr == hStack->wpText || ((lpREGroupItem->dwFlags & REGF_MULTILINE) && (*(wpStr - 1) == L'\n' || *(wpStr - 1) == L'\r')))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }

      //Compare char
      nPatChar=0;

      if (!(lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD)))
      {
        if (wpStr > wpMaxStr)
          goto EndLoop;
        if (wpStr == wpMaxStr)
        {
          nStrChar=-AELB_EOF;
          nCharSize=0;
          goto PatChar;
        }
      }
      if (wpStr == hStack->wpMaxText)
      {
        nStrChar=-AELB_EOF;
        nCharSize=0;
      }
      else nCharSize=PatStrChar(wpStr, wpMaxStr, &nStrChar);

      PatChar:
      if (*wpPat == L'.')
      {
        if (lpREGroupItem->dwFlags & REGF_NONEWLINEDOT)
        {
          //Any character except new line
          if (nStrChar < 0) goto EndLoop;
        }
        else if (nStrChar == -AELB_EOF)
          goto EndLoop;
      }
      else if (*wpPat == L'[')
      {
        if (nStrChar == -AELB_EOF)
          goto EndLoop;

        if (*++wpPat == L'^')
        {
          bExclude=TRUE;
          ++wpPat;
        }
        else bExclude=FALSE;

        while (*wpPat != L']')
        {
          if (*wpPat == L'-')
          {
            if (!(dwCmpResult & RECCE_MIX))
            {
              ++wpPat;
              dwCmpResult=PatCharCmp(&wpPat, nStrChar, (lpREGroupItem->dwFlags & REGF_MATCHCASE)|RECCF_FULLSURROGATE, &nPatNextChar);
              if (dwCmpResult & RECCE_EQUAL)
                goto ClassMatch;

              //Check range
              if (!(dwCmpResult & RECCE_MIX))
              {
                if ((nStrChar >= nPatChar && nStrChar <= nPatNextChar) ||
                    (!(lpREGroupItem->dwFlags & REGF_MATCHCASE) && nStrChar >= 0 && nStrChar <= MAXWORD &&
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
            dwCmpResult=PatCharCmp(&wpPat, nStrChar, (lpREGroupItem->dwFlags & REGF_MATCHCASE)|RECCF_FULLSURROGATE, &nPatChar);
            if (dwCmpResult & RECCE_EQUAL)
              goto ClassMatch;
            if (dwCmpResult & RECCE_WORD)
            {
              if (PatIsCharDelim(nStrChar, hStack->wpDelim, hStack->wpMaxDelim) == (*wpPat == L'W'))
                goto ClassMatch;
            }
            else if (dwCmpResult & RECCE_REF)
            {
              nRefIndex=PatRefIndex(&wpPat);
              if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
              {
                nRefLen=PatStrCmp(lpREGroupRef->wpStrStart, lpREGroupRef->wpStrEnd, lpREGroupItem->dwFlags, wpStr, wpMaxStr);
                if (nRefLen == 1)
                {
                  --wpPat;
                  goto ClassMatch;
                }
              }
              continue;
            }
          }
          ++wpPat;
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
        dwCmpResult=PatCharCmp(&wpPat, nStrChar, (lpREGroupItem->dwFlags & REGF_MATCHCASE), &nPatChar);

        if (!(dwCmpResult & RECCE_EQUAL))
        {
          if (dwCmpResult & RECCE_DIF)
            goto EndLoop;

          if (dwCmpResult & RECCE_WORD)
          {
            if (PatIsCharDelim(nStrChar, hStack->wpDelim, hStack->wpMaxDelim) == (*wpPat == L'W'))
              goto NextChar;
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_REF)
          {
            nRefIndex=PatRefIndex(&wpPat);
            if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
            {
              nRefLen=PatStrCmp(lpREGroupRef->wpStrStart, lpREGroupRef->wpStrEnd, lpREGroupItem->dwFlags, wpStr, wpMaxStr);
              if (!nRefLen)
              {
                if (lpREGroupRef->nMinMatch || lpREGroupRef->nStrLen)
                  goto EndLoop;
              }
            }
            else goto EndLoop;

            wpStr+=nRefLen;
            continue;
          }
          else if (dwCmpResult & RECCE_BOUNDARY)
          {
            if (PatIsCharBoundary(wpStr, hStack))
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
            if (wpStr == hStack->wpText)
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_STREND)
          {
            if (wpStr == hStack->wpMaxText)
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_RANGEBEGIN)
          {
            if (wpStr == hStack->wpRange)
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_RANGEEND)
          {
            if (wpStr == hStack->wpMaxRange)
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_ROOTBEGIN)
          {
            hStack->wpRootStart=wpStr;
            ++wpPat;
            continue;
          }
        }
      }
      NextChar:
      if (nStrChar > MAXWORD && nPatChar <= MAXWORD)
        wpStr+=1;
      else
        wpStr+=1 + nCharSize;
      ++wpPat;
    }
    EndLoop:
    if (lpREGroupItem->dwFlags & REGF_NEGATIVEFIXED)
    {
      if (wpPat < wpMaxPat)
      {
        wpPat=wpPatChar;

        //Count all fixed length pattern
        do
        {
          if (wpStr >= wpMaxStr)
            goto ReturnFalse;

          nPatChar=PatEscChar(&wpPat);

          if (*wpPat == L'[')
          {
            while (*++wpPat != L']')
            {
              if (*wpPat == L'\\')
                ++wpPat;
            }
          }
          else if (nPatChar == REEC_REF)
          {
            nRefIndex=PatRefIndex(&wpPat);
            if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
            {
              nRefLen=lpREGroupRef->wpStrEnd - lpREGroupRef->wpStrStart;
              if (wpStr + nRefLen < wpMaxStr)
                wpStr+=nRefLen;
              else
                goto ReturnFalse;
            }
            continue;
          }
          nCharSize=PatStrChar(wpStr, wpMaxStr, &nStrChar);

          if (nStrChar > MAXWORD && nPatChar <= MAXWORD)
            wpStr+=1;
          else
            wpStr+=1 + nCharSize;
          ++wpPat;
        }
        while (wpPat < wpMaxPat);

        goto MatchFixed;
      }
      MismatchFixed:
      if (nCurMatch < lpREGroupItem->nMinMatch)
        goto ReturnFalse;
      if (!nCurMatch)
      {
        lpREGroupItem->wpStrStart=wpStrStart;
        lpREGroupItem->wpStrEnd=wpStrStart;
        lpREGroupItem->nStrLen=0;
      }
      goto ReturnTrue;
    }
    EndLoopAfterNegativeFixed:
    if (nCurMatch < lpREGroupItem->nMinMatch)
    {
      if (lpREGroupItem->dwFlags & (REGF_NEGATIVEFORWARD|REGF_NEGATIVEBACKWARD))
      {
        lpREGroupItem->wpStrStart=wpMinStr;
        lpREGroupItem->wpStrEnd=wpMinStr;
        lpREGroupItem->nStrLen=0;
        goto ReturnTrue;
      }
      goto ReturnFalse;
    }
    if (!nCurMatch)
    {
      lpREGroupItem->wpStrStart=wpStrStart;
      lpREGroupItem->wpStrEnd=wpStrStart;
      lpREGroupItem->nStrLen=0;
    }
  }
  if (lpREGroupItem->dwFlags & (REGF_NEGATIVEFORWARD|REGF_NEGATIVEBACKWARD))
  {
    goto ReturnFalse;
  }
  if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_POSITIVEBACKWARD))
  {
    lpREGroupItem->wpStrStart=wpMinStr;
    lpREGroupItem->wpStrEnd=wpMinStr;
    lpREGroupItem->nStrLen=0;
  }
  if (wpGreedyStrEnd)
  {
    lpREGroupItem->wpStrEnd=wpGreedyStrEnd;
    lpREGroupItem->nStrLen=nGreedyStrLen;
    if (nNextMatched == REE_FALSE)
      nNextMatched=REE_TRUE;
    goto ReturnTrue;
  }
  if (nNextMatched == REE_FALSE && !(lpREGroupItem->dwFlags & REGF_OR))
    goto ReturnFalse;
  ReturnTrue:
  #ifdef _DEBUG
    --hStack->nDeepness;
  #endif
  if (nNextMatched <= 0)
  {
    if (nNextMatched == REE_INT_LASTGROUP || !PatNextGroupForExec(lpREGroupItem))
      nNextMatched=REE_TRUE|REE_LASTGROUP;
    else
      nNextMatched=REE_TRUE;
  }
  else
    nNextMatched|=REE_NEXTMATCH;
  //if (wpStr >= wpMaxStr)
  //  nNextMatched|=REE_ENDSTRING;
  if (!lpREGroupItem->parent && hStack->wpRootStart)
    hStack->first->wpStrStart=hStack->wpRootStart;
  return nNextMatched;

  ReturnFalse:
  #ifdef _DEBUG
    --hStack->nDeepness;
  #endif
  return REE_FALSE;
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

int PatEscChar(const wchar_t **wppPat)
{
  int nPatChar=**wppPat;
  const wchar_t *wpCodeEnd;

  if (nPatChar == L'\\')
  {
    ++(*wppPat);
    nPatChar=**wppPat;

    if (nPatChar == L'u')
    {
      nPatChar=(int)hex2decW(++(*wppPat), 4, NULL);
      (*wppPat)+=3;
    }
    else if (nPatChar == L'x')
    {
      ++(*wppPat);
      if (**wppPat == L'{')
      {
        for (wpCodeEnd=++(*wppPat); *wpCodeEnd && *wpCodeEnd != L'}'; ++wpCodeEnd);
        nPatChar=(int)hex2decW(*wppPat, wpCodeEnd - *wppPat, NULL);
        if (*wpCodeEnd)
          *wppPat=wpCodeEnd;
        else
          *wppPat=wpCodeEnd - 1;
      }
      else
      {
        nPatChar=(int)hex2decW(*wppPat, 2, NULL);
        (*wppPat)+=1;
      }
    }
    else
    {
      switch (nPatChar)
      {
        case L'r':
        case L'n':
          return REEC_NEWLINE;
        case L't':
          return L'\t';
        case L'f':
          return L'\f';
        case L'v':
          return L'\v';
        case L'd':
          return REEC_DIGIT;
        case L'D':
          return REEC_NONDIGIT;
        case L's':
          return REEC_SPACE;
        case L'S':
          return REEC_NONSPACE;
        case L'w':
        case L'W':
          return REEC_WORD;
        case L'b':
        case L'B':
          return REEC_BOUNDARY;
        case L'A':
          return REEC_STRBEGIN;
        case L'Z':
          return REEC_STREND;
        case L'a':
          return REEC_RANGEBEGIN;
        case L'z':
          return REEC_RANGEEND;
        case L'K':
          return REEC_ROOTBEGIN;
      }
      if (nPatChar >= L'0' && nPatChar <= L'9')
        return REEC_REF;
    }
  }
  return nPatChar;
}

DWORD PatCharCmp(const wchar_t **wppPat, int nStrChar, DWORD dwFlags, int *lpnPatChar)
{
  int nPatChar=PatEscChar(wppPat);

  //AELB_EOF, AELB_R, AELB_N, AELB_RN, AELB_RRN
  if (nStrChar < 0)
  {
    if (nPatChar < REEC_WRONG)
    {
      if (nStrChar == -AELB_EOF)
        return RECCE_DIF;
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
      *lpnPatChar=L'\0';

      if (nStrChar == -AELB_EOF)
        goto SpecialChars;
      if (nPatChar == REEC_NEWLINE)
      {
        *lpnPatChar=L'\n';
        return RECCE_EQUAL|RECCE_MIX;
      }
      if (nPatChar == REEC_SPACE ||
          nPatChar == REEC_NONDIGIT)
      {
        return RECCE_EQUAL|RECCE_MIX;
      }
      goto SpecialChars;
    }
    return RECCE_DIF;
  }

  if (nPatChar > REEC_WRONG)
  {
    if (nPatChar == REEC_NEWLINE)
    {
      *lpnPatChar=L'\n';
      return RECCE_DIF|RECCE_MIX;
    }
    *lpnPatChar=L'\0';

    if (nPatChar == REEC_DIGIT)
    {
      if (nStrChar < L'0' || nStrChar > L'9')
        return RECCE_DIF|RECCE_MIX;
      return RECCE_EQUAL|RECCE_MIX;
    }
    else if (nPatChar == REEC_NONDIGIT)
    {
      if (nStrChar >= L'0' && nStrChar <= L'9')
        return RECCE_DIF|RECCE_MIX;
      return RECCE_EQUAL|RECCE_MIX;
    }
    else if (nPatChar == REEC_SPACE)
    {
      if (nStrChar != L' ' && nStrChar != L'\f' && nStrChar != L'\n' && nStrChar != L'\r' && nStrChar != L'\t' && nStrChar != L'\v')
        return RECCE_DIF|RECCE_MIX;
      return RECCE_EQUAL|RECCE_MIX;
    }
    else if (nPatChar == REEC_NONSPACE)
    {
      if (nStrChar == L' ' || nStrChar == L'\f' || nStrChar == L'\n' || nStrChar == L'\r' || nStrChar == L'\t' || nStrChar == L'\v')
        return RECCE_DIF|RECCE_MIX;
      return RECCE_EQUAL|RECCE_MIX;
    }
    goto SpecialChars;
  }

  //Compare
  *lpnPatChar=nPatChar;
  if (!(dwFlags & RECCF_FULLSURROGATE) && nStrChar > MAXWORD && nPatChar <= MAXWORD)
    nStrChar=AEC_HighSurrogateFromScalar(nStrChar);
  if (nStrChar != nPatChar && ((dwFlags & RECCF_MATCHCASE) || nStrChar > MAXWORD || nPatChar > MAXWORD || WideCharLower((wchar_t)nStrChar) != WideCharLower((wchar_t)nPatChar)))
    return RECCE_DIF;
  return RECCE_EQUAL;

  SpecialChars:
  switch (nPatChar)
  {
    case REEC_WORD:
      return RECCE_WORD|RECCE_MIX;
    case REEC_BOUNDARY:
      return RECCE_BOUNDARY|RECCE_MIX;
    case REEC_STRBEGIN:
      return RECCE_STRBEGIN|RECCE_MIX;
    case REEC_STREND:
      return RECCE_STREND|RECCE_MIX;
    case REEC_RANGEBEGIN:
      return RECCE_RANGEBEGIN|RECCE_MIX;
    case REEC_RANGEEND:
      return RECCE_RANGEEND|RECCE_MIX;
    case REEC_ROOTBEGIN:
      return RECCE_ROOTBEGIN|RECCE_MIX;
    case REEC_REF:
      return RECCE_REF|RECCE_MIX;
  }
  return RECCE_DIF|RECCE_MIX;
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

BOOL PatIsNextCharDelim(const wchar_t *wpChar, STACKREGROUP *hStack)
{
  int nNextChar;

  if (wpChar < hStack->wpMaxText)
  {
    nNextChar=*wpChar;
    if (nNextChar == L'\r') nNextChar=L'\n';
    return PatIsCharDelim(nNextChar, hStack->wpDelim, hStack->wpMaxDelim);
  }
  return TRUE;
}

BOOL PatIsPrevCharDelim(const wchar_t *wpChar, STACKREGROUP *hStack)
{
  int nPrevChar;

  if (wpChar > hStack->wpText)
  {
    nPrevChar=*(wpChar - 1);
    if (nPrevChar == L'\r') nPrevChar=L'\n';
    return PatIsCharDelim(nPrevChar, hStack->wpDelim, hStack->wpMaxDelim);
  }
  return TRUE;
}

BOOL PatIsCharBoundary(const wchar_t *wpChar, STACKREGROUP *hStack)
{
  if (PatIsNextCharDelim(wpChar, hStack) != PatIsPrevCharDelim(wpChar, hStack))
    return TRUE;
  return FALSE;
}

int PatRefIndex(const wchar_t **wppPat)
{
  return (int)xatoiW(*wppPat, wppPat);

  //const wchar_t *wpPat=*wppPat;
  //wchar_t wszIndex[3];
  //int nIndex=-1;
  //
  //if (*wpPat >= L'0' && *wpPat <= L'9')
  //{
  //  wszIndex[0]=*wpPat++;
  //  wszIndex[1]=L'\0';
  //  if (*wpPat >= L'0' && *wpPat <= L'9')
  //  {
  //    wszIndex[1]=*wpPat++;
  //    wszIndex[2]=L'\0';
  //  }
  //  nIndex=(int)xatoiW(wszIndex, NULL);
  //  *wppPat=wpPat;
  //}
  //return nIndex;
}

INT_PTR PatStrCmp(const wchar_t *wpStrStart1, const wchar_t *wpStrEnd1, DWORD dwFlags, const wchar_t *wpStrStart2, const wchar_t *wpMaxStr)
{
  INT_PTR nStrLen1=wpStrEnd1 - wpStrStart1;

  if (nStrLen1 >= 0 && nStrLen1 <= wpMaxStr - wpStrStart2 &&
      ((dwFlags & REGF_MATCHCASE) ?
       !xstrcmpnW(wpStrStart1, wpStrStart2, nStrLen1) :
       !xstrcmpinW(wpStrStart1, wpStrStart2, nStrLen1)))
  {
    //Matched
    return nStrLen1;
  }
  return 0;
}

REGROUP* PatCloseGroups(REGROUP *lpREGroupItem, const wchar_t *wpPatEnd, const wchar_t *wpPatRight, REGROUP **lppREGroupNextAuto)
{
  REGROUP *lpREGroupChild;
  REGROUP *lpREGroupParent;
  const wchar_t *wpPatChild;

  do
  {
    if (!lpREGroupItem->parent)
      return lpREGroupItem;

    if (lpREGroupItem->dwFlags & REGF_AUTOGROUP)
    {
      if (lppREGroupNextAuto) *lppREGroupNextAuto=NULL;
      lpREGroupItem->wpPatEnd=wpPatEnd;
      lpREGroupItem->wpPatRight=wpPatEnd;
    }
    if (lpREGroupItem->firstChild)
    {
      //If only one children and pattern the same, then remove redundant grouping
      if (lpREGroupItem->firstChild == lpREGroupItem->lastChild && !(lpREGroupItem->dwFlags & REGF_IFPARENT) && !(lpREGroupItem->dwFlags & REGF_ORPARENT))
      {
        lpREGroupChild=lpREGroupItem->firstChild;

        if (lpREGroupChild->wpPatLeft == lpREGroupItem->wpPatStart &&
            lpREGroupChild->wpPatRight == wpPatEnd &&
            (lpREGroupChild->nIndex == -1 || lpREGroupItem->nIndex == -1))
        {
          lpREGroupParent=lpREGroupItem->parent;

          //Replace lpREGroupItem with lpREGroupChild
          if (lpREGroupItem->wpPatLeft < lpREGroupChild->wpPatLeft)
            lpREGroupChild->wpPatLeft=lpREGroupItem->wpPatLeft;
          if (wpPatRight > lpREGroupChild->wpPatLeft)
            lpREGroupChild->wpPatRight=wpPatRight;
          lpREGroupChild->dwFlags|=lpREGroupItem->dwFlags;
          if (lpREGroupChild->nIndex == -1)
            lpREGroupChild->nIndex=lpREGroupItem->nIndex;
          StackJoin((stack **)&lpREGroupParent->firstChild, (stack **)&lpREGroupParent->lastChild, (stack *)lpREGroupItem, (stack *)lpREGroupChild, (stack *)lpREGroupChild);
          StackDelete((stack **)&lpREGroupParent->firstChild, (stack **)&lpREGroupParent->lastChild, (stack *)lpREGroupItem);
          lpREGroupChild->parent=lpREGroupParent;
          lpREGroupItem=lpREGroupChild;
        }
      }

      //If nMinMatch of all children is zero, then set nMinMatch to zero
      if (lpREGroupItem->nMinMatch)
      {
        wpPatChild=lpREGroupItem->wpPatStart;

        for (lpREGroupChild=lpREGroupItem->firstChild; lpREGroupChild; lpREGroupChild=lpREGroupChild->next)
        {
          if (lpREGroupChild->nMinMatch) break;
          if (lpREGroupChild->wpPatLeft != wpPatChild) break;
          wpPatChild=lpREGroupChild->wpPatRight;
        }
        if (!lpREGroupChild && wpPatEnd == wpPatChild)
          lpREGroupItem->nMinMatch=0;
      }
    }
    if (!(lpREGroupItem->dwFlags & REGF_AUTOGROUP)) break;

    if (lpREGroupItem->parent->nGroupLen != -1)
    {
      if (lpREGroupItem->nGroupLen != -1)
      {
        if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD|REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD))
        {
          //Don't count non-capture group.
        }
        else lpREGroupItem->parent->nGroupLen+=lpREGroupItem->nGroupLen;
      }
      else lpREGroupItem->parent->nGroupLen=-1;
    }
    if (lpREGroupItem->parent->nGroupLen == -1)
      lpREGroupItem->dwFlags|=REGF_NOTFIXEDPARENT;
    lpREGroupItem=lpREGroupItem->parent;
  }
  while (lpREGroupItem);

  return lpREGroupItem;
}

BOOL PatIsInNonCapture(REGROUP *lpREGroupItem)
{
  do
  {
    if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD))
      return TRUE;
  }
  while (lpREGroupItem=lpREGroupItem->parent);

  return FALSE;
}

REGROUP* PatGetGroup(STACKREGROUP *hStack, int nIndex)
{
  REGROUP *lpREGroupItem;

  if (nIndex >= 100)
  {
    hStack=hStack->ref100;
    nIndex-=100;
  }
  if (nIndex > hStack->nLastIndex)
    return NULL;

  for (lpREGroupItem=hStack->first; lpREGroupItem; lpREGroupItem=PatNextGroup(lpREGroupItem))
  {
    if (lpREGroupItem->nIndex == nIndex)
      break;
  }
  return lpREGroupItem;
}

REGROUP* PatGetMatchedGroup(STACKREGROUP *hStack, int nIndex)
{
  //str - "A", find - "((A)\s+)?A", replace - "[\2]"
  REGROUP *lpREGroupItem;

  if (nIndex > hStack->nLastIndex)
    return NULL;

  for (lpREGroupItem=hStack->first; lpREGroupItem;)
  {
    if (lpREGroupItem->nStrLen)
    {
      if (lpREGroupItem->nIndex > nIndex)
        return NULL;
      if (lpREGroupItem->nIndex == nIndex)
        break;
      lpREGroupItem=PatNextGroup(lpREGroupItem);
    }
    else lpREGroupItem=PatNextGroupNoChild(lpREGroupItem);
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

REGROUP* PatNextGroupForExec(REGROUP *lpREGroupItem)
{
  //No childrens, no REGF_OR
  REGROUP *lpREGroupNext=lpREGroupItem;

  for (;;)
  {
    Loop:
    if (lpREGroupNext->next)
    {
      if (lpREGroupNext->next->dwFlags & REGF_OR)
      {
        lpREGroupNext=lpREGroupNext->next;
        goto Loop;
      }
      return lpREGroupNext->next;
    }
    if (!(lpREGroupNext=lpREGroupNext->parent))
      break;

    //nSelfExec
    if (lpREGroupNext->parent)
    {
      if (lpREGroupNext->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE))
        return NULL;

      //str - "dac", find "(a|d)+"
      if (!(lpREGroupNext->dwFlags & REGF_ORPARENT) &&
        (DWORD)lpREGroupNext->nSelfMatch + 1 < (DWORD)lpREGroupNext->nMinMatch)
      {
        //str - "A", find "(A*+B?)*+"
        if (lpREGroupNext->nSelfExec == 0)
        {
          //Deny execute parent if children not match anything
          if (lpREGroupItem->wpStrEnd <= lpREGroupNext->wpStrStart)
            return NULL;
        }
        else
        {
          //Deny execute parent if no progress since last exec
          if (lpREGroupItem->wpStrEnd <= lpREGroupNext->wpStrEnd)
            return NULL;
        }
        lpREGroupNext->wpStrEnd=lpREGroupItem->wpStrEnd;
        lpREGroupNext->nStrLen=lpREGroupNext->wpStrEnd - lpREGroupNext->wpStrStart;

        ++lpREGroupNext->nSelfMatch;
        ++lpREGroupNext->nSelfExec;
        break;
      }
    }
  }
  return lpREGroupNext;
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
    if (*wpStr == L'\\')
    {
      if (lpREGroupRef=PatGetMatchedGroup(pgs->lpREGroupStack, (int)xatoiW(++wpStr, &wpStr)))
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
  REGROUP *lpREGroupItem=hStack->first;

  //Check root element
  if (lpREGroupItem && lpREGroupItem->wpStrStart)
  {
    do
    {
      lpREGroupItem->wpStrStart=NULL;
      lpREGroupItem->wpStrEnd=NULL;
      lpREGroupItem->nStrLen=0;
    }
    while (lpREGroupItem=PatNextGroup(lpREGroupItem));
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
BOOL AE_PatExec(STACKREGROUP *hStack, REGROUP *lpREGroupItem, AECHARINDEX *ciInput, const AECHARINDEX *ciMaxInput)
{
  REGROUP *lpREGroupNext;
  REGROUP *lpREGroupNextNext;
  REGROUP *lpREGroupRef;
  REGROUP *lpREGroupOrMatch;
  AECHARINDEX ciStr=*ciInput;
  AECHARINDEX ciMinStr=*ciInput;
  AECHARINDEX ciMaxStr=*ciMaxInput;
  AECHARINDEX ciStrStart=ciStr;
  AECHARINDEX ciGreedyStrEnd;
  AECHARINDEX ciBackupStrStart;
  AECHARINDEX ciBackupStrEnd;
  const wchar_t *wpPat;
  const wchar_t *wpMaxPat=lpREGroupItem->wpPatEnd;
  const wchar_t *wpPatChar=NULL;
  const wchar_t *wpNextGroup;
  INT_PTR nStrLen=0;
  INT_PTR nBackupStrLen;
  INT_PTR nPrevStrLen;
  INT_PTR nRefLen;
  INT_PTR nGreedyStrLen=0;
  int nStrChar;
  int nPatChar;
  int nPatNextChar;
  wchar_t wchCaseChar;
  int nCurMatch;
  int nRefIndex;
  int nCompare;
  DWORD dwCmpResult=0;
  int nNextMatched=REE_INT_DEFAULT;
  BOOL bMatched;
  BOOL bLastMatched;
  BOOL bExclude;
  BOOL bNewLoop;

  #ifdef _DEBUG
    ++hStack->nDeepness;
  #endif
  ciGreedyStrEnd.lpLine=NULL;

  if (lpREGroupItem->dwFlags & REGF_ROOTITEM)
  {
    nCurMatch=0;
    lpREGroupItem->ciStrStart=ciStr;
    lpREGroupItem->ciStrEnd=ciStr;
    lpREGroupItem->nStrLen=0;
    lpREGroupItem->nSelfMatch=0;
    hStack->ciRootStart.lpLine=NULL;

    if (lpREGroupItem->dwFlags & REGF_ROOTANY)
    {
      BOOL bResult=REE_FALSE;

      //Turn off REGF_ROOTANY and execute itself
      lpREGroupItem->dwFlags&=~REGF_ROOTITEM & ~REGF_ROOTANY;

      while (AEC_IndexCompare(&ciStr, &ciMaxStr) <= 0)
      {
        if (AE_PatExec(hStack, lpREGroupItem, &ciStr, &ciMaxStr))
        {
          if (hStack->dwOptions & REO_WHOLEWORD)
          {
            if (!AE_PatIsPrevCharDelim(&lpREGroupItem->ciStrStart, hStack) ||
                !AE_PatIsNextCharDelim(&lpREGroupItem->ciStrEnd, hStack))
              goto RootReset;
          }
          bResult=REE_TRUE;
          break;

          RootReset:
          lpREGroupItem->ciStrStart=ciStr;
          lpREGroupItem->ciStrEnd=ciStr;
          lpREGroupItem->nSelfMatch=0;
        }
        if (lpREGroupItem->dwFlags & REGF_ROOTMULTILINE)
        {
          if (!AEC_NextLine(&ciStr))
            break;
        }
        else
        {
          if (!AEC_NextChar(&ciStr))
            break;
        }
      }
      lpREGroupItem->dwFlags|=REGF_ROOTITEM|REGF_ROOTANY;
      return bResult;
    }
  }
  else
  {
    if (lpREGroupItem->nSelfExec)
    {
      nCurMatch=lpREGroupItem->nSelfMatch;
      //str - "abc", find "($a*)+"
      if (AEC_IndexCompare(&ciStr, &lpREGroupItem->ciStrStart) <= 0)
        goto EndLoopAfterNegativeFixed;
    }
    else nCurMatch=0;

    if (lpREGroupItem->dwFlags & (REGF_NEGATIVEBACKWARD|REGF_POSITIVEBACKWARD))
    {
      //Find start position
      for (nPrevStrLen=lpREGroupItem->nGroupLen; nPrevStrLen > 0 && AEC_PrevChar(&ciStr); --nPrevStrLen);
      if (nPrevStrLen) goto EndLoopAfterNegativeFixed;
    }
    if (!lpREGroupItem->nMinMatch &&
        //str - "123", find "(?>\d+?)3"
        !(lpREGroupItem->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE)))
    {
      nPrevStrLen=-1;
      lpREGroupItem->ciStrStart=ciStr;
      lpREGroupItem->ciStrEnd=ciStr;
      lpREGroupItem->nStrLen=0;
      goto MatchNext;
    }
    lpREGroupItem->nStrLen=0;
  }

  BeginLoop:
  bNewLoop=TRUE;

  if ((DWORD)nCurMatch < (DWORD)lpREGroupItem->nMaxMatch)
  {
    wpPat=lpREGroupItem->wpPatStart;
    if (lpREGroupNext=lpREGroupItem->firstChild)
      wpNextGroup=lpREGroupNext->wpPatLeft;
    else
      wpNextGroup=NULL;

    for (;;)
    {
      wpPatChar=wpPat;

      if (wpPat >= wpMaxPat)
      {
        if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD|REGF_POSITIVEBACKWARD|REGF_NEGATIVEBACKWARD|REGF_NEGATIVEFIXED))
        {
          if (lpREGroupItem->dwFlags & REGF_NEGATIVEFIXED)
            goto MismatchFixed;
          ++nCurMatch;
          goto EndLoop;
        }
        MatchFixed:
        ++nCurMatch;
        nPrevStrLen=lpREGroupItem->nStrLen;
        lpREGroupItem->ciStrStart=ciStrStart;
        lpREGroupItem->ciStrEnd=ciStr;
        lpREGroupItem->nStrLen=nStrLen;

        if (((nCurMatch >= lpREGroupItem->nMinMatch &&
             (DWORD)nCurMatch <= (DWORD)lpREGroupItem->nMaxMatch &&
             (lpREGroupItem->dwFlags & REGF_NOTFIXEDPARENT)) ||
            //str - "abc", find "a(bc|b)c"
            (lpREGroupItem->dwFlags & REGF_OR)) &&
            //str - "123", find "(?>\d+?)3"
            !(lpREGroupItem->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE)))
        {
          MatchNext:
          if (lpREGroupNextNext=AE_PatNextGroupForExec(lpREGroupItem))
          {
            if (lpREGroupNextNext->nSelfExec)
            {
              //Save data before PatExec
              ciBackupStrStart=lpREGroupNextNext->ciStrStart;
              ciBackupStrEnd=lpREGroupNextNext->ciStrEnd;
              nBackupStrLen=lpREGroupNextNext->nStrLen;

              nNextMatched=AE_PatExec(hStack, lpREGroupNextNext, &ciStr, &ciMaxStr);

              //Restore data after PatExec
              lpREGroupItem->ciStrStart=ciStrStart;
              lpREGroupItem->ciStrEnd=ciStr;
              lpREGroupItem->nStrLen=nStrLen;

              lpREGroupNextNext->nStrLen=nBackupStrLen;
              if (AEC_IndexCompare(&ciBackupStrStart, &lpREGroupNextNext->ciStrStart) < 0)
                lpREGroupNextNext->ciStrStart=ciBackupStrStart;
              if (AEC_IndexCompare(&ciBackupStrEnd, &lpREGroupNextNext->ciStrEnd) > 0)
                lpREGroupNextNext->ciStrEnd=ciBackupStrEnd;
              else
                lpREGroupNextNext->nStrLen+=AE_PatStrSub(&lpREGroupNextNext->ciStrEnd, &ciBackupStrEnd);

              --lpREGroupNextNext->nSelfMatch;
              --lpREGroupNextNext->nSelfExec;
            }
            else
            {
              nNextMatched=AE_PatExec(hStack, lpREGroupNextNext, &ciStr, &ciMaxStr);
            }

            //Check nStrLen for \d+Z? in 123Z
            if (nNextMatched && (lpREGroupNextNext->nStrLen || lpREGroupNextNext->nMinMatch))
            {
              if (!(lpREGroupItem->dwFlags & REGF_GREEDY))
                goto EndLoop;
              ciGreedyStrEnd=ciStr;
              nGreedyStrLen=lpREGroupItem->nStrLen;
            }
            ////str - "long string", find - "([^#]*@?)*"
            //if (nNextMatched & REE_ENDSTRING)
            //  goto EndLoop;
          }
          else nNextMatched=REE_INT_LASTGROUP;
        }
        else nNextMatched=REE_INT_DEFAULT;

        //str - "abc", find ".*$"
        if (AEC_IndexCompare(&ciStr, &ciMaxStr) >= 0)
          goto EndLoop;
        //Prevent infinite loop: str - "yAy", find "((y)?)*"
        if (lpREGroupItem->nStrLen == nPrevStrLen)
          goto EndLoop;
        goto BeginLoop;
      }
      if (wpPat == wpNextGroup)
      {
        if (lpREGroupNext->dwFlags & REGF_ONLYOPTIONS)
          goto NextGroup;
        bMatched=FALSE;
        lpREGroupOrMatch=NULL;
        goto FirstCheck;

        for (;;)
        {
          if (!bMatched || (!(bMatched & (REE_NEXTMATCH|REE_LASTGROUP)) && !(lpREGroupNext->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE))))
          {
            FirstCheck:
            if (lpREGroupItem->dwFlags & REGF_REFEXIST)
            {
              //Set ciStrStart for possible backreferences in child AE_PatExec.
              lpREGroupItem->ciStrStart=ciStrStart;
            }
            //str - "a1a2a3a4", find "(a.*?){2}"
            //str - "a1a2a3a4", find "(a.*?){2}\w"
            lpREGroupItem->nSelfMatch=nCurMatch;
            lpREGroupNext->ciStrStart=ciStr;
            lpREGroupNext->nSelfMatch=0;

            if ((lpREGroupItem->dwFlags & REGF_IFPARENT) || (lpREGroupNext->dwFlags & REGF_IFPARENT))
            {
              //str - "abc", find - "(a)(?(1)bc)"
              if (lpREGroupItem->dwFlags & REGF_IFPARENT)
                lpREGroupNext=lpREGroupItem;

              if (lpREGroupNext->firstChild->conditionRef ?
                    lpREGroupNext->firstChild->conditionRef->nStrLen :
                    AE_PatExec(hStack, lpREGroupNext->firstChild, &ciStr, &ciMaxStr))
              {
                if (lpREGroupNext->firstChild->next->dwFlags & REGF_IFFALSE)
                  goto NextGroup;
                lpREGroupNextNext=lpREGroupNext->firstChild->next;
              }
              else
              {
                if (lpREGroupNext->firstChild->next->next)
                  lpREGroupNextNext=lpREGroupNext->firstChild->next->next;
                else if (lpREGroupNext->firstChild->next->dwFlags & REGF_IFFALSE)
                  lpREGroupNextNext=lpREGroupNext->firstChild->next;
                else
                  goto NextGroup;
              }
              if (!AE_PatExec(hStack, lpREGroupNextNext, &ciStr, &ciMaxStr))
              {
                if (lpREGroupNext->dwFlags & REGF_OR)
                  goto NextOR;
                goto EndLoop;
              }
              lpREGroupNext->ciStrStart=lpREGroupNextNext->ciStrStart;
              lpREGroupNext->ciStrEnd=lpREGroupNextNext->ciStrEnd;
              lpREGroupNext->nStrLen=lpREGroupNextNext->nStrLen;
              lpREGroupNext->nSelfMatch=lpREGroupNextNext->nSelfMatch;
              ciStr=lpREGroupNextNext->ciStrEnd;
              nStrLen+=lpREGroupNextNext->nStrLen;
              if (lpREGroupNext->dwFlags & REGF_OR)
              {
                bMatched=TRUE;
                goto NextOR;
              }
              goto NextGroup;
            }

            if (!(bLastMatched=AE_PatExec(hStack, lpREGroupNext, &ciStr, &ciMaxStr)))
            {
              if (lpREGroupNext->dwFlags & REGF_OR)
                goto NextOR;
              goto EndLoop;
            }
            if (lpREGroupNext->dwFlags & REGF_OR)
            {
              if (!lpREGroupOrMatch || lpREGroupNext->nStrLen < lpREGroupOrMatch->nStrLen || (bLastMatched & (REE_NEXTMATCH|REE_LASTGROUP)))
                lpREGroupOrMatch=lpREGroupNext;
              bMatched=bLastMatched;
              goto NextOR;
            }
            ciStr=lpREGroupNext->ciStrEnd;
            nStrLen+=lpREGroupNext->nStrLen;
            goto NextGroup;
          }
          NextOR:
          if (lpREGroupNext->next && (lpREGroupNext->next->dwFlags & REGF_OR))
            lpREGroupNext=lpREGroupNext->next;
          else
            break;
        }
        if (bMatched)
        {
          if (lpREGroupItem->dwFlags & REGF_NEGATIVEFORWARD)
            goto ReturnFalse;
          if (lpREGroupOrMatch)
          {
            ciStr=lpREGroupOrMatch->ciStrEnd;
            nStrLen+=lpREGroupOrMatch->nStrLen;
          }
        }
        else goto EndLoop;

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
        if ((lpREGroupItem->dwFlags & REGF_MULTILINE) ? AEC_IsLastCharInLine(&ciStr) : AEC_IsLastCharInFile(&ciStr))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }
      if (*wpPat == L'^')
      {
        if ((lpREGroupItem->dwFlags & REGF_MULTILINE) ? AEC_IsFirstCharInLine(&ciStr) : AEC_IsFirstCharInFile(&ciStr))
        {
          ++wpPat;
          continue;
        }
        goto EndLoop;
      }

      //Compare char
      nPatChar=0;

      if (!(lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_NEGATIVEFORWARD)))
      {
        if ((nCompare=AEC_IndexCompare(&ciStr, &ciMaxStr)) > 0)
          goto EndLoop;
        if (!nCompare)
        {
          nStrChar=-AELB_EOF;
          goto PatChar;
        }
      }
      nStrChar=AE_PatStrChar(&ciStr);

      PatChar:
      if (*wpPat == L'.')
      {
        if (lpREGroupItem->dwFlags & REGF_NONEWLINEDOT)
        {
          //Any character except new line
          if (nStrChar < 0) goto EndLoop;
        }
        else if (nStrChar == -AELB_EOF)
          goto EndLoop;
      }
      else if (*wpPat == L'[')
      {
        if (nStrChar == -AELB_EOF)
          goto EndLoop;

        if (*++wpPat == L'^')
        {
          bExclude=TRUE;
          ++wpPat;
        }
        else bExclude=FALSE;

        while (*wpPat != L']')
        {
          if (*wpPat == L'-')
          {
            if (!(dwCmpResult & RECCE_MIX))
            {
              ++wpPat;
              dwCmpResult=PatCharCmp(&wpPat, nStrChar, (lpREGroupItem->dwFlags & REGF_MATCHCASE)|RECCF_FULLSURROGATE, &nPatNextChar);
              if (dwCmpResult & RECCE_EQUAL)
                goto ClassMatch;

              //Check range
              if (!(dwCmpResult & RECCE_MIX))
              {
                if ((nStrChar >= nPatChar && nStrChar <= nPatNextChar) ||
                    (!(lpREGroupItem->dwFlags & REGF_MATCHCASE) && nStrChar >= 0 && nStrChar <= MAXWORD &&
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
            dwCmpResult=PatCharCmp(&wpPat, nStrChar, (lpREGroupItem->dwFlags & REGF_MATCHCASE)|RECCF_FULLSURROGATE, &nPatChar);
            if (dwCmpResult & RECCE_EQUAL)
              goto ClassMatch;
            if (dwCmpResult & RECCE_WORD)
            {
              if (PatIsCharDelim(nStrChar, hStack->wpDelim, hStack->wpMaxDelim) == (*wpPat == L'W'))
                goto ClassMatch;
            }
            else if (dwCmpResult & RECCE_REF)
            {
              nRefIndex=PatRefIndex(&wpPat);
              if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
              {
                nRefLen=AE_PatStrCmp(&lpREGroupRef->ciStrStart, &lpREGroupRef->ciStrEnd, lpREGroupItem->dwFlags, &ciStr, NULL, &ciMaxStr);
                if (nRefLen == 1)
                {
                  --wpPat;
                  goto ClassMatch;
                }
              }
              continue;
            }
          }
          ++wpPat;
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
        dwCmpResult=PatCharCmp(&wpPat, nStrChar, (lpREGroupItem->dwFlags & REGF_MATCHCASE), &nPatChar);

        if (!(dwCmpResult & RECCE_EQUAL))
        {
          if (dwCmpResult & RECCE_DIF)
            goto EndLoop;

          if (dwCmpResult & RECCE_WORD)
          {
            if (PatIsCharDelim(nStrChar, hStack->wpDelim, hStack->wpMaxDelim) == (*wpPat == L'W'))
              goto NextChar;
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_REF)
          {
            nRefIndex=PatRefIndex(&wpPat);
            if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
            {
              nRefLen=AE_PatStrCmp(&lpREGroupRef->ciStrStart, &lpREGroupRef->ciStrEnd, lpREGroupItem->dwFlags, &ciStr, &ciStr, &ciMaxStr);
              if (!nRefLen)
              {
                if (lpREGroupRef->nMinMatch || lpREGroupRef->nStrLen)
                  goto EndLoop;
              }
            }
            else goto EndLoop;

            nStrLen+=nRefLen;
            continue;
          }
          else if (dwCmpResult & RECCE_BOUNDARY)
          {
            if (AE_PatIsCharBoundary(&ciStr, hStack))
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
            if (AEC_IsFirstCharInFile(&ciStr))
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_STREND)
          {
            if (AEC_IsLastCharInFile(&ciStr))
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_RANGEBEGIN)
          {
            if (!AEC_IndexCompare(&ciStr, &hStack->ciRange))
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_RANGEEND)
          {
            if (!AEC_IndexCompare(&ciStr, &hStack->ciMaxRange))
            {
              ++wpPat;
              continue;
            }
            goto EndLoop;
          }
          else if (dwCmpResult & RECCE_ROOTBEGIN)
          {
            hStack->ciRootStart=ciStr;
            ++wpPat;
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
    if (lpREGroupItem->dwFlags & REGF_NEGATIVEFIXED)
    {
      if (wpPat < wpMaxPat)
      {
        wpPat=wpPatChar;

        //Count all fixed length pattern
        do
        {
          if (AEC_IndexCompare(&ciStr, &ciMaxStr) >= 0)
            goto ReturnFalse;

          nPatChar=PatEscChar(&wpPat);

          if (*wpPat == L'[')
          {
            while (*++wpPat != L']')
            {
              if (*wpPat == L'\\')
                ++wpPat;
            }
          }
          else if (nPatChar == REEC_REF)
          {
            nRefIndex=PatRefIndex(&wpPat);
            if (lpREGroupRef=PatGetGroup(hStack, nRefIndex))
            {
              for (nPrevStrLen=lpREGroupRef->nStrLen; nPrevStrLen > 0; --nPrevStrLen)
              {
                if (AEC_IndexCompare(&ciStr, &ciMaxStr) >= 0)
                  goto ReturnFalse;
                AE_PatNextChar(&ciStr);
                ++nStrLen;
              }
            }
            continue;
          }

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
        while (wpPat < wpMaxPat);

        goto MatchFixed;
      }
      MismatchFixed:
      if (nCurMatch < lpREGroupItem->nMinMatch)
        goto ReturnFalse;
      if (!nCurMatch)
      {
        lpREGroupItem->ciStrStart=ciStrStart;
        lpREGroupItem->ciStrEnd=ciStrStart;
        lpREGroupItem->nStrLen=0;
      }
      goto ReturnTrue;
    }
    EndLoopAfterNegativeFixed:
    if (nCurMatch < lpREGroupItem->nMinMatch)
    {
      if (lpREGroupItem->dwFlags & (REGF_NEGATIVEFORWARD|REGF_NEGATIVEBACKWARD))
      {
        lpREGroupItem->ciStrStart=ciMinStr;
        lpREGroupItem->ciStrEnd=ciMinStr;
        lpREGroupItem->nStrLen=0;
        goto ReturnTrue;
      }
      goto ReturnFalse;
    }
    if (!nCurMatch)
    {
      lpREGroupItem->ciStrStart=ciStrStart;
      lpREGroupItem->ciStrEnd=ciStrStart;
      lpREGroupItem->nStrLen=0;
    }
  }
  if (lpREGroupItem->dwFlags & (REGF_NEGATIVEFORWARD|REGF_NEGATIVEBACKWARD))
  {
    goto ReturnFalse;
  }
  if (lpREGroupItem->dwFlags & (REGF_POSITIVEFORWARD|REGF_POSITIVEBACKWARD))
  {
    lpREGroupItem->ciStrStart=ciMinStr;
    lpREGroupItem->ciStrEnd=ciMinStr;
    lpREGroupItem->nStrLen=0;
  }
  if (ciGreedyStrEnd.lpLine)
  {
    lpREGroupItem->ciStrEnd=ciGreedyStrEnd;
    lpREGroupItem->nStrLen=nGreedyStrLen;
    if (nNextMatched == REE_FALSE)
      nNextMatched=REE_TRUE;
    goto ReturnTrue;
  }
  if (nNextMatched == REE_FALSE && !(lpREGroupItem->dwFlags & REGF_OR))
    goto ReturnFalse;
  ReturnTrue:
  #ifdef _DEBUG
    --hStack->nDeepness;
  #endif
  if (nNextMatched <= 0)
  {
    if (nNextMatched == REE_INT_LASTGROUP || !PatNextGroupForExec(lpREGroupItem))
      nNextMatched=REE_TRUE|REE_LASTGROUP;
    else
      nNextMatched=REE_TRUE;
  }
  else
    nNextMatched|=REE_NEXTMATCH;
  //if (AEC_IndexCompare(&ciStr, &ciMaxStr) >= 0)
  //  nNextMatched|=REE_ENDSTRING;
  if (!lpREGroupItem->parent && hStack->ciRootStart.lpLine)
    hStack->first->ciStrStart=hStack->ciRootStart;
  return nNextMatched;

  ReturnFalse:
  #ifdef _DEBUG
    --hStack->nDeepness;
  #endif
  return REE_FALSE;
}

int AE_PatStrChar(const AECHARINDEX *ciChar)
{
  return AEC_CharAtIndex(ciChar);
}

BOOL AE_PatIsNextCharDelim(const AECHARINDEX *ciChar, STACKREGROUP *hStack)
{
  if (!AEC_IsLastCharInFile(ciChar))
    return PatIsCharDelim(AE_PatStrChar(ciChar), hStack->wpDelim, hStack->wpMaxDelim);
  return TRUE;
}

BOOL AE_PatIsPrevCharDelim(const AECHARINDEX *ciChar, STACKREGROUP *hStack)
{
  AECHARINDEX ciPrevChar=*ciChar;

  AEC_PrevChar(&ciPrevChar);
  if (ciPrevChar.lpLine)
    return PatIsCharDelim(AE_PatStrChar(&ciPrevChar), hStack->wpDelim, hStack->wpMaxDelim);
  return TRUE;
}

BOOL AE_PatIsCharBoundary(const AECHARINDEX *ciChar, STACKREGROUP *hStack)
{
  if (AE_PatIsNextCharDelim(ciChar, hStack) != AE_PatIsPrevCharDelim(ciChar, hStack))
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

INT_PTR AE_PatStrCmp(const AECHARINDEX *ciStrStart1, const AECHARINDEX *ciStrEnd1, DWORD dwFlags, const AECHARINDEX *ciStrStart2, AECHARINDEX *ciStrEnd2, const AECHARINDEX *ciMaxStr)
{
  AECHARINDEX ciStrCount1=*ciStrStart1;
  AECHARINDEX ciStrCount2=*ciStrStart2;
  INT_PTR nStrLen2=0;
  int nStrChar1;
  int nStrChar2;

  while (AEC_IndexCompare(&ciStrCount1, ciStrEnd1) < 0 && AEC_IndexCompare(&ciStrCount2, ciMaxStr) < 0)
  {
    nStrChar1=AE_PatStrChar(&ciStrCount1);
    nStrChar2=AE_PatStrChar(&ciStrCount2);
    if (nStrChar1 < 0) nStrChar1=L'\n';
    if (nStrChar2 < 0) nStrChar2=L'\n';

    if (((dwFlags & REGF_MATCHCASE) || nStrChar2 > MAXWORD || nStrChar1 > MAXWORD) ?
         nStrChar2 != nStrChar1 :
         WideCharLower((wchar_t)nStrChar2) != WideCharLower((wchar_t)nStrChar1))
    {
      nStrLen2=0;
      goto End;
    }

    AEC_NextChar(&ciStrCount1);
    AEC_NextChar(&ciStrCount2);
    if (nStrChar2 <= MAXWORD)
      nStrLen2+=1;
    else
      nStrLen2+=2;
  }
  if (AEC_IndexCompare(&ciStrCount1, ciStrEnd1) < 0)
    nStrLen2=0;

  End:
  if (nStrLen2 && ciStrEnd2) *ciStrEnd2=ciStrCount2;
  return nStrLen2;
}

INT_PTR AE_PatStrSub(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2)
{
  const AECHARINDEX *ciTmp;
  AELINEDATA *lpLine;
  INT_PTR nCount;
  int nCompare=AEC_IndexCompare(ciChar1, ciChar2);

  if (!nCompare)
    return 0;
  if (nCompare < 0)
  {
    //ciChar1 should be greater
    ciTmp=ciChar1;
    ciChar1=ciChar2;
    ciChar2=ciTmp;
  }

  if (ciChar1->nLine == ciChar2->nLine)
  {
    nCount=ciChar1->nCharInLine - ciChar2->nCharInLine;
  }
  else
  {
    nCount=ciChar2->lpLine->nLineLen - ciChar2->nCharInLine;
    if (ciChar2->lpLine->nLineBreak != AELB_WRAP)
      ++nCount;

    for (lpLine=ciChar2->lpLine->next; ; lpLine=lpLine->next)
    {
      if (lpLine == ciChar1->lpLine)
      {
        nCount+=ciChar1->nCharInLine;
        break;
      }
      if (lpLine->nLineBreak != AELB_WRAP)
        nCount+=lpLine->nLineLen + 1;
      else
        nCount+=lpLine->nLineLen;
    }
  }
  return (nCompare < 0)?-nCount:nCount;
}

REGROUP* AE_PatNextGroupForExec(REGROUP *lpREGroupItem)
{
  //No childrens, no REGF_OR
  REGROUP *lpREGroupNext=lpREGroupItem;

  for (;;)
  {
    Loop:
    if (lpREGroupNext->next)
    {
      if (lpREGroupNext->next->dwFlags & REGF_OR)
      {
        lpREGroupNext=lpREGroupNext->next;
        goto Loop;
      }
      return lpREGroupNext->next;
    }
    if (!(lpREGroupNext=lpREGroupNext->parent))
      break;

    //nSelfExec
    if (lpREGroupNext->parent)
    {
      if (lpREGroupNext->dwFlags & (REGF_ATOMIC|REGF_POSSESSIVE))
        return NULL;

      //str - "dac", find "(a|d)+"
      if (!(lpREGroupNext->dwFlags & REGF_ORPARENT) &&
          (DWORD)lpREGroupNext->nSelfMatch + 1 < (DWORD)lpREGroupNext->nMinMatch)
      {
        //str - "A", find "(A*+B?)*+"
        if (lpREGroupNext->nSelfExec == 0)
        {
          //Deny execute parent if children not match anything
          if (AEC_IndexCompare(&lpREGroupItem->ciStrEnd, &lpREGroupNext->ciStrStart) <= 0)
            return NULL;
          lpREGroupNext->nStrLen=AE_PatStrSub(&lpREGroupItem->ciStrEnd, &lpREGroupNext->ciStrStart);
        }
        else
        {
          //Deny execute parent if no progress since last exec
          if (AEC_IndexCompare(&lpREGroupItem->ciStrEnd, &lpREGroupNext->ciStrEnd) <= 0)
            return NULL;
          lpREGroupNext->nStrLen+=AE_PatStrSub(&lpREGroupItem->ciStrEnd, &lpREGroupNext->ciStrEnd);
        }
        lpREGroupNext->ciStrEnd=lpREGroupItem->ciStrEnd;

        ++lpREGroupNext->nSelfMatch;
        ++lpREGroupNext->nSelfExec;
        break;
      }
    }
  }
  return lpREGroupNext;
}

void AE_PatReset(STACKREGROUP *hStack)
{
  REGROUP *lpREGroupItem=hStack->first;

  //Check root element
  if (lpREGroupItem && lpREGroupItem->ciStrStart.lpLine && lpREGroupItem->nStrLen)
  {
    do
    {
      xmemset(&lpREGroupItem->ciStrStart, 0, sizeof(AECHARRANGE));
      lpREGroupItem->nStrLen=0;
    }
    while (lpREGroupItem=PatNextGroup(lpREGroupItem));
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
      pe->lpREGroupStack->dwOptions=pe->dwOptions & (RESE_MATCHCASE|RESE_MULTILINE|RESE_WHOLEWORD|RESE_NONEWLINEDOT);
      pe->lpREGroupStack->wpDelim=pe->wpDelim;
      pe->lpREGroupStack->wpMaxDelim=pe->wpDelim?pe->wpMaxDelim:NULL;
      pe->lpREGroupStack->wpText=pe->wpText;
      pe->lpREGroupStack->wpMaxText=pe->wpMaxText;
      pe->lpREGroupStack->wpRange=pe->wpRange;
      pe->lpREGroupStack->wpMaxRange=pe->wpMaxRange;
      #ifdef __AKELEDIT_H__
      if (!pe->wpStr)
      {
        pe->lpREGroupStack->ciRange=pe->ciRange;
        pe->lpREGroupStack->ciMaxRange=pe->ciMaxRange;
      }
      #endif

      if (pe->nErrorOffset=PatCompile(pe->lpREGroupStack, pe->wpPat, pe->wpMaxPat))
        return 0;
    }
  }
  if (!(lpREGroupRoot=pe->lpREGroupStack->first))
    return 0;
  if (pe->dwOptions & RESE_ISMATCH)
  {
    lpREGroupRoot->dwFlags&=~REGF_ROOTANY;
    nMaxMatchCount=1;
  }
  else if (!(pe->dwOptions & RESE_GLOBAL))
    nMaxMatchCount=1;

  if (pe->wpStr)
  {
    const wchar_t *wpStrNext=pe->wpStr;
    int nStrChar;

    //Patterns for check with <= or == sign:
    //str - "any", find - ".*", replace - "A\0Z"
    //str - "", find - "$", replace - "ZZZ"
    while (wpStrNext <= pe->wpMaxStr)
    {
      if (bMatched=PatExec(pe->lpREGroupStack, lpREGroupRoot, wpStrNext, pe->wpMaxStr))
        ++nMatchCount;

      if (pe->lpCallback)
      {
        if (pe->lpCallback(pe, lpREGroupRoot, bMatched) == RESEC_STOP)
          return nMatchCount;
      }

      //Find next match
      if (!bMatched || (DWORD)nMatchCount >= (DWORD)nMaxMatchCount)
        break;
      pe->wpStr=lpREGroupRoot->wpStrEnd;
      wpStrNext=lpREGroupRoot->wpStrEnd;
      if (!lpREGroupRoot->nStrLen) //*(pe->wpMaxPat - 1) == L'^' or L'$'
        wpStrNext+=PatStrChar(wpStrNext, pe->wpMaxStr, &nStrChar) + 1;

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
        if (pe->lpCallback(pe, lpREGroupRoot, bMatched) == RESEC_STOP)
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

  //Fill PATEXECPARAM
  pep.wpRep=pr->wpRep;
  pep.wpMaxRep=pr->wpMaxRep;
  pep.wpNewLine=pr->wpNewLine?pr->wpNewLine:L"\r\n";
  if (pr->nPointCount)
  {
    pep.lpPointArray=pr->lpPointArray;
    pep.nPointCount=pr->nPointCount;
  }
  else
  {
    pep.lpPointArray=NULL;
    pep.nPointCount=0;
  }
  pep.wszBuf=pr->wszResult;
  pep.wpBufCount=pep.wszBuf;

  //Fill PATEXEC
  pe.dwOptions=pr->dwOptions;
  pe.wpDelim=pr->wpDelim;
  pe.wpMaxDelim=pr->wpDelim?pr->wpMaxDelim:NULL;
  pe.wpText=pr->wpText;
  pe.wpMaxText=pr->wpMaxText;
  pe.lpREGroupStack=0;
  pe.wpPat=pr->wpPat;
  pe.wpMaxPat=pr->wpMaxPat;
  pe.wpStr=pr->wpStr;
  pe.wpMaxStr=pr->wpMaxStr;

  if (pr->wpStr)
  {
    pe.wpRange=pr->wpRange;
    pe.wpMaxRange=pr->wpMaxRange;
    pe.lpCallback=(PATEXECCALLBACK)PatReplaceCallback;
  }
  else
  {
    #ifdef __AKELEDIT_H__
      pe.ciRange=pr->ciRange;
      pe.ciMaxRange=pr->ciMaxRange;
      pe.ciStr=pr->ciStr;
      pe.ciMaxStr=pr->ciMaxStr;
      pe.lpCallback=(PATEXECCALLBACK)AE_PatReplaceCallback;
    #endif
  }
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
  }
  pr->nErrorOffset=pe.nErrorOffset;

  if (!pe.nErrorOffset)
  {
    if (pep.wszBuf)
      *pep.wpBufCount=L'\0';
    else
      ++pep.wpBufCount;
  }
  PatStructFree(&pe);

  return (INT_PTR)(pep.wpBufCount - pep.wszBuf);
}

int CALLBACK PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched)
{
  PATEXECPARAM *pep=(PATEXECPARAM *)pe->lParam;
  REGROUP *lpREGroupRef;
  const wchar_t *wpRep=pep->wpRep;
  const wchar_t *wpStartRep;
  int nPatChar;
  int nIndex;

  if (bMatched)
  {
    //Copy unmatched left part of string
    if (pep->wszBuf)
      xmemcpy(pep->wpBufCount, pe->wpStr, (lpREGroupRoot->wpStrStart - pe->wpStr) * sizeof(wchar_t));
    pep->wpBufCount+=lpREGroupRoot->wpStrStart - pe->wpStr;

    //Replace matched part of string
    wpStartRep=pep->wpBufCount;

    while (wpRep < pep->wpMaxRep)
    {
      nPatChar=PatEscChar(&wpRep);

      if (nPatChar == REEC_REF)
      {
        nIndex=PatRefIndex(&wpRep);

        if (lpREGroupRef=PatGetMatchedGroup(pe->lpREGroupStack, nIndex))
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

    //Shift points
    if (pep->nPointCount)
    {
      PATREPLACEPOINT *lpPointCount;
      PATREPLACEPOINT *lpPointMax=pep->lpPointArray + pep->nPointCount;
      INT_PTR nDiff=lpREGroupRoot->nStrLen - (pep->wpBufCount - wpStartRep);

      for (lpPointCount=pep->lpPointArray; lpPointCount < lpPointMax; ++lpPointCount)
      {
        if (lpPointCount->wpStr >= lpREGroupRoot->wpStrEnd)
          lpPointCount->nShift-=nDiff;
        else if (lpPointCount->wpStr > lpREGroupRoot->wpStrStart)
          lpPointCount->nShift-=(lpPointCount->wpStr - lpREGroupRoot->wpStrStart);
      }
    }
  }
  else
  {
    //Copy unmatched right part of string.
    if (pep->wszBuf)
      xmemcpy(pep->wpBufCount, pe->wpStr, (pe->wpMaxStr - pe->wpStr) * sizeof(wchar_t));
    pep->wpBufCount+=pe->wpMaxStr - pe->wpStr;
    pep->wpRightStr=pe->wpMaxStr;
  }
  return RESEC_CONTINUE;
}

#ifdef __AKELEDIT_H__
int CALLBACK AE_PatReplaceCallback(PATEXEC *pe, REGROUP *lpREGroupRoot, BOOL bMatched)
{
  PATEXECPARAM *pep=(PATEXECPARAM *)pe->lParam;
  REGROUP *lpREGroupRef;
  const wchar_t *wpRep=pep->wpRep;
  const wchar_t *wpStartRep;
  int nPatChar;
  int nIndex;

  if (bMatched)
  {
    //Copy unmatched left part of string
    pep->wpBufCount+=AE_PatStrCopy(&pe->ciStr, &lpREGroupRoot->ciStrStart, pep->wszBuf?pep->wpBufCount:NULL, NULL);

    //Replace matched part of string
    wpStartRep=pep->wpBufCount;

    while (wpRep < pep->wpMaxRep)
    {
      nPatChar=PatEscChar(&wpRep);

      if (nPatChar == REEC_REF)
      {
        nIndex=PatRefIndex(&wpRep);

        if (lpREGroupRef=PatGetMatchedGroup(pe->lpREGroupStack, nIndex))
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

    //Shift points
    if (pep->nPointCount)
    {
      PATREPLACEPOINT *lpPointCount;
      PATREPLACEPOINT *lpPointMax=pep->lpPointArray + pep->nPointCount;
      INT_PTR nDiff=lpREGroupRoot->nStrLen - (pep->wpBufCount - wpStartRep);

      for (lpPointCount=pep->lpPointArray; lpPointCount < lpPointMax; ++lpPointCount)
      {
        if (AEC_IndexCompare(&lpPointCount->ciStr, &lpREGroupRoot->ciStrEnd) >= 0)
          lpPointCount->nShift-=nDiff;
        else if (AEC_IndexCompare(&lpPointCount->ciStr, &lpREGroupRoot->ciStrStart) > 0)
          lpPointCount->nShift-=AE_PatStrCopy(&lpREGroupRoot->ciStrStart, &lpPointCount->ciStr, NULL, NULL);
      }
    }
  }
  else
  {
    //Copy unmatched right part of string.
    pep->wpBufCount+=AE_PatStrCopy(&pe->ciStr, &pe->ciMaxStr, pep->wszBuf?pep->wpBufCount:NULL, NULL);
    pep->ciRightStr=pe->ciMaxStr;
  }
  return RESEC_CONTINUE;
}

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

//// Global variables
#ifdef __AKELEDIT_H__
  AECHARINDEX RegExpGlobal_ciMaxStr={0x7FFFFFFF, NULL, 0};
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
  //Search example (PatCompile + PatExec)
  {
    STACKREGROUP sreg;
    const wchar_t *wpPat=L"(23)(.*)(89)";
    const wchar_t *wpMaxPat=wpPat + lstrlenW(wpPat);
    const wchar_t *wpStr=L"1234567890 11223344556677889900";
    const wchar_t *wpMaxStr=wpStr + lstrlenW(wpStr);
    wchar_t wszResult[MAX_PATH];
    INT_PTR nErrorOffset;

    sreg.first=0;
    sreg.last=0;
    sreg.dwOptions=REO_MULTILINE;
    sreg.wpDelim=NULL;
    sreg.wpMaxDelim=NULL;

    if (!(nErrorOffset=PatCompile(&sreg, wpPat, wpMaxPat)))
    {
      sreg.wpText=wpStr;
      sreg.wpMaxText=wpMaxStr;
      sreg.wpRange=wpStr;
      sreg.wpMaxRange=wpMaxStr;

      if (PatExec(&sreg, sreg.first, wpStr, wpMaxStr))
      {
        lstrcpynW(wszResult, sreg.first->wpStrStart, (int)sreg.first->nStrLen + 1);
        MessageBoxW(NULL, wszResult, L"Matched", MB_OK);
      }
      else MessageBoxW(NULL, L"", L"Not matched", MB_OK);

      PatFree(&sreg);
    }
  }

  //Search example (PatStructExec)
  {
    PATEXEC pe;
    REGROUP *lpREGroupRoot;
    REGROUP *lpREGroupNext;
    wchar_t wszResult[MAX_PATH];
    wchar_t *wpResult;

    //Fill structure for PatStructExec
    pe.wpStr=L"1234567890 11223344556677889900";
    pe.wpMaxStr=pe.wpStr + lstrlenW(pe.wpStr);
    pe.wpPat=L"(23)(.*)(89)";
    pe.wpMaxPat=pe.wpPat + lstrlenW(pe.wpPat);
    pe.lpREGroupStack=0;
    pe.dwOptions=RESE_MATCHCASE;
    pe.wpDelim=NULL;
    pe.wpText=pe.wpStr;
    pe.wpMaxText=pe.wpMaxStr;
    pe.wpRange=pe.wpStr;
    pe.wpMaxRange=pe.wpMaxStr;
    pe.lpCallback=NULL;

    while (PatStructExec(&pe))
    {
      lpREGroupRoot=pe.lpREGroupStack->first;
      lpREGroupNext=lpREGroupRoot;
      wpResult=wszResult;

      do
      {
        if (lpREGroupNext->nStrLen && lpREGroupNext->nIndex != -1)
        {
          //wpResult+=xprintfW(wpResult, L"%d [%.%ds]\n", lpREGroupNext->nIndex, lpREGroupNext->nStrLen, lpREGroupNext->wpStrStart);
          wpResult+=wsprintfW(wpResult, L"%d [", lpREGroupNext->nIndex);
          lstrcpynW(wpResult, lpREGroupNext->wpStrStart, (int)lpREGroupNext->nStrLen + 1);
          wpResult+=lpREGroupNext->nStrLen;
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

  //Replace example
  {
    PATREPLACE pr;
    INT_PTR nResultTextLen;

    pr.wpStr=L"1234567890 1234567890";
    pr.wpMaxStr=pr.wpStr + lstrlenW(pr.wpStr);
    pr.wpPat=L"(23)(.*)(89)";
    pr.wpMaxPat=pr.wpPat + lstrlenW(pr.wpPat);
    pr.wpRep=L"\\1abc\\3";
    pr.wpMaxRep=pr.wpRep + lstrlenW(pr.wpRep);
    pr.dwOptions=RESE_GLOBAL|RESE_MULTILINE;
    pr.wpDelim=NULL;
    pr.wpText=pr.wpStr;
    pr.wpMaxText=pr.wpMaxStr;
    pr.wpRange=pr.wpStr;
    pr.wpMaxRange=pr.wpMaxStr;
    pr.wpNewLine=NULL;
    pr.nPointCount=0;
    pr.wszResult=NULL;
    nResultTextLen=PatReplace(&pr);

    if (pr.nReplaceCount)
    {
      if (pr.wszResult=(wchar_t *)GlobalAlloc(GMEM_FIXED, nResultTextLen * sizeof(wchar_t)))
        PatReplace(&pr);
    }
    if (pr.wszResult)
    {
      MessageBoxW(NULL, pr.wszResult, NULL, 0);
      GlobalFree((HGLOBAL)pr.wszResult);
    }
  }
}
*/
