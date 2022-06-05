/*****************************************************************
 *                 Method functions header v1.4                  *
 *                                                               *
 * 2015 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 * MethodParseParameters, MethodStructParameters,                *
 * MethodGetParameter, MethodFreeParameters,                     *
 * MethodComment, MethodGetName, MethodGetIcon                   *
 *****************************************************************/


#ifndef _METHODFUNC_H_
#define _METHODFUNC_H_

#define ALLMETHODFUNC

//External parameters
#define EXTPARAM_CHAR     1
#define EXTPARAM_INT      2
#define EXTPARAM_VAR      3

typedef struct _EXTPARAM {
  struct _EXTPARAM *next;
  struct _EXTPARAM *prev;
  DWORD dwType;            //See EXTPARAM_* defines.
  INT_PTR nNumber;         //External parameter number.
  char *pString;           //External parameter string (Ansi).
  wchar_t *wpString;       //External parameter string (Unicode).
  char *pExpanded;         //External parameter expanded string - without %variables% (Ansi).
  int nExpandedAnsiLen;    //External parameter expanded ansi string length.
  wchar_t *wpExpanded;     //External parameter expanded string - without %variables% (Unicode).
  int nExpandedUnicodeLen; //External parameter expanded unicode string length.
} EXTPARAM;

typedef struct {
  EXTPARAM *first;
  EXTPARAM *last;
  int nElements;
} STACKEXTPARAM;

void MethodComment(const wchar_t *wpText, const wchar_t **wppText);
int MethodParseParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpText, const wchar_t **wppText);
int MethodStructParameters(STACKEXTPARAM *hParamStack, unsigned char *lpStruct);
EXTPARAM* MethodGetParameter(STACKEXTPARAM *hParamStack, int nIndex);
void MethodFreeParameters(STACKEXTPARAM *hParamStack);
int MethodGetName(const wchar_t *wpText, wchar_t *wszMethod, int nMethodMax, const wchar_t **wppText);
int MethodGetScript(const wchar_t *wpText, wchar_t *wszMethod, int nMethodMax, const wchar_t **wppText);
void MethodGetIcon(const wchar_t *wpText, wchar_t *wszIconFile, int nMaxIconFile, int *nIconIndex, const wchar_t **wppText);


/********************************************************************
 *
 *  MethodComment
 *
 *Skip comment if any.
 *
 * [in] const wchar_t *wpText    -Text.
 *[out] const wchar_t **wppText  -Pointer to the text after comment if any.
 *
 *Returns: no return value.
 ********************************************************************/
#if defined MethodComment || defined ALLMETHODFUNC
#define MethodComment_INCLUDED
#undef MethodComment
__inline void MethodComment(const wchar_t *wpText, const wchar_t **wppText)
{
  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  if (*wpText == L'/' && *(wpText + 1) == L'*')
  {
    for (wpText+=2; *wpText; ++wpText)
    {
      if (*wpText == L'*' && *(wpText + 1) == L'/')
      {
        wpText+=2;
        break;
      }
    }
    while (*wpText == L' ' || *wpText == L'\t') ++wpText;
  }
  *wppText=wpText;
}
#endif

/********************************************************************
 *
 *  MethodParseParameters
 *
 *Parse parameters and put them to the stack.
 *
 * [in,out] STACKEXTPARAM *hParamStack  -Stack pointer.
 * [in] const wchar_t *wpText           -Text. Usually points to a first symbol after "(", for example: "Scripts::Main", 1, "SearchReplace.js")
 *[out] const wchar_t **wppText         -Pointer to the text after method. Can be NULL.
 *
 *Returns: number of elements in stack.
 ********************************************************************/
#if defined MethodParseParameters || defined ALLMETHODFUNC
#define MethodParseParameters_INCLUDED
#undef MethodParseParameters
__inline int MethodParseParameters(STACKEXTPARAM *hParamStack, const wchar_t *wpText, const wchar_t **wppText)
{
  EXTPARAM *lpParameter;
  const wchar_t *wpParamBegin=wpText;
  const wchar_t *wpParamEnd;
  wchar_t wchStopChar;
  INT_PTR nStringLen;

  MethodParameter:
  MethodComment(wpParamBegin, &wpParamBegin);

  if (*wpParamBegin == L'\"' || *wpParamBegin == L'\'' || *wpParamBegin == L'`')
  {
    //String
    wchStopChar=*wpParamBegin++;
    for (wpParamEnd=wpParamBegin; *wpParamEnd != wchStopChar && *wpParamEnd != L'\0'; ++wpParamEnd);

    if (!StackInsertBefore((stack **)&hParamStack->first, (stack **)&hParamStack->last, NULL, (stack **)&lpParameter, sizeof(EXTPARAM)))
    {
      ++hParamStack->nElements;
      lpParameter->dwType=EXTPARAM_CHAR;
      nStringLen=wpParamEnd - wpParamBegin;
      if (lpParameter->wpString=(wchar_t *)GlobalAlloc(GPTR, (nStringLen + 1) * sizeof(wchar_t)))
        xstrcpynW(lpParameter->wpString, wpParamBegin, nStringLen + 1);
    }
    if (*wpParamEnd) ++wpParamEnd;
  }
  else
  {
    //Number
    for (wpParamEnd=wpParamBegin; *wpParamEnd != L' ' && *wpParamEnd != L'\t' && *wpParamEnd != L',' && *wpParamEnd != L')' && *wpParamEnd != L'\r' && *wpParamEnd != L'\n' && *wpParamEnd != L'\0'; ++wpParamEnd);
    if (wpParamEnd == wpParamBegin) goto End;

    if (!StackInsertBefore((stack **)&hParamStack->first, (stack **)&hParamStack->last, NULL, (stack **)&lpParameter, sizeof(EXTPARAM)))
    {
      ++hParamStack->nElements;

      if ((*wpParamBegin >= L'0' && *wpParamBegin <= L'9') ||
          (*wpParamBegin == L'-' && *(wpParamBegin + 1) >= L'0' && *(wpParamBegin + 1) <= L'9'))
      {
        lpParameter->dwType=EXTPARAM_INT;
        if (*wpParamBegin == L'0' && *(wpParamBegin + 1) == L'x')
          lpParameter->nNumber=hex2decW(wpParamBegin + 2, -2, NULL);
        else
          lpParameter->nNumber=xatoiW(wpParamBegin, NULL);
      }
      else
      {
        lpParameter->dwType=EXTPARAM_VAR;
        nStringLen=wpParamEnd - wpParamBegin;
        if (lpParameter->wpString=(wchar_t *)GlobalAlloc(GPTR, (nStringLen + 1) * sizeof(wchar_t)))
          xstrcpynW(lpParameter->wpString, wpParamBegin, nStringLen + 1);
      }
    }
  }
  if (WideGlobal_bOldWindows && lpParameter->wpString)
  {
    nStringLen=WideCharToMultiByte(CP_ACP, 0, lpParameter->wpString, -1, NULL, 0, NULL, NULL);
    if (lpParameter->pString=(char *)GlobalAlloc(GPTR, nStringLen))
      WideCharToMultiByte(CP_ACP, 0, lpParameter->wpString, -1, lpParameter->pString, (int)nStringLen, NULL, NULL);
  }
  MethodComment(wpParamEnd, &wpParamEnd);

  if (*wpParamEnd == L',')
  {
    wpParamBegin=wpParamEnd + 1;
    goto MethodParameter;
  }

  End:
  if (*wpParamEnd == L')')
    ++wpParamEnd;
  //else
  //  MethodFreeParameters(hParamStack);
  if (wppText) *wppText=wpParamEnd;
  return hParamStack->nElements;
}
#endif

/********************************************************************
 *
 *  MethodStructParameters
 *
 *Create structure from stack.
 *
 * [in] STACKEXTPARAM *hParamStack  -Stack pointer.
 *[out] unsigned char *lpStruct     -Structure pointer.
 *                                    First element is the size of the structure.
 *                                    Every element has size of INT_PTR.
 *                                    If NULL required structure size returned in bytes.
 *
 *Returns: size of the structure in bytes.
 ********************************************************************/
#if defined MethodStructParameters || defined ALLMETHODFUNC
#define MethodStructParameters_INCLUDED
#undef MethodStructParameters
__inline int MethodStructParameters(STACKEXTPARAM *hParamStack, unsigned char *lpStruct)
{
  EXTPARAM *lpParameter;
  int nElementOffset;
  int nStringOffset=0;

  if (hParamStack->nElements > 1)
  {
    //nStringOffset is pointer to memory where first string will be copied
    nElementOffset=0;
    nStringOffset=hParamStack->nElements * sizeof(INT_PTR);

    //First element in structure is the size of the structure
    if (lpStruct) *((INT_PTR *)(lpStruct + nElementOffset))=nStringOffset;
    nElementOffset+=sizeof(INT_PTR);

    //Skip hParamStack->first equal to "Plugin::Function".
    for (lpParameter=hParamStack->first->next; lpParameter; lpParameter=lpParameter->next)
    {
      if (lpParameter->dwType == EXTPARAM_CHAR)
      {
        //Strings located after call parameters structure
        if (lpStruct) *((INT_PTR *)(lpStruct + nElementOffset))=(INT_PTR)(lpStruct + nStringOffset);

        if (WideGlobal_bOldWindows)
        {
          if (lpStruct) xmemcpy(lpStruct + nStringOffset, lpParameter->pExpanded, lpParameter->nExpandedAnsiLen + 1);
          nStringOffset+=(lpParameter->nExpandedAnsiLen + 1);
        }
        else
        {
          if (lpStruct) xmemcpy(lpStruct + nStringOffset, lpParameter->wpExpanded, (lpParameter->nExpandedUnicodeLen + 1) * sizeof(wchar_t));
          nStringOffset+=(lpParameter->nExpandedUnicodeLen + 1) * sizeof(wchar_t);
        }
      }
      else
      {
        if (lpStruct) *((INT_PTR *)(lpStruct + nElementOffset))=lpParameter->nNumber;
      }
      nElementOffset+=sizeof(INT_PTR);
    }
  }
  return nStringOffset;
}
#endif

/********************************************************************
 *
 *  MethodGetParameter
 *
 *Get parameter in stack by index.
 *
 * [in] STACKEXTPARAM *hParamStack  -Stack pointer.
 * [in] int nIndex                  -Index of the parameter in stack. Can be negative.
 *
 *Returns: pointer to a EXTPARAM structure or NULL if not found.
 ********************************************************************/
#if defined MethodGetParameter || defined ALLMETHODFUNC
#define MethodGetParameter_INCLUDED
#undef MethodGetParameter
__inline EXTPARAM* MethodGetParameter(STACKEXTPARAM *hParamStack, int nIndex)
{
  EXTPARAM *lpParameter;

  if (!StackGetElement((stack *)hParamStack->first, (stack *)hParamStack->last, (stack **)&lpParameter, nIndex))
    return lpParameter;
  return NULL;
}
#endif

/********************************************************************
 *
 *  MethodFreeParameters
 *
 *Free parameters stack.
 *
 *[in,out] STACKEXTPARAM *hParamStack  -Stack pointer.
 *
 *Returns: no return value.
 ********************************************************************/
#if defined MethodFreeParameters || defined ALLMETHODFUNC
#define MethodFreeParameters_INCLUDED
#undef MethodFreeParameters
__inline void MethodFreeParameters(STACKEXTPARAM *hParamStack)
{
  EXTPARAM *lpParameter;

  for (lpParameter=hParamStack->first; lpParameter; lpParameter=lpParameter->next)
  {
    if (lpParameter->dwType == EXTPARAM_CHAR || lpParameter->dwType == EXTPARAM_VAR)
    {
      if (lpParameter->pString) GlobalFree((HGLOBAL)lpParameter->pString);
      if (lpParameter->wpString) GlobalFree((HGLOBAL)lpParameter->wpString);
      if (lpParameter->pExpanded) GlobalFree((HGLOBAL)lpParameter->pExpanded);
      if (lpParameter->wpExpanded) GlobalFree((HGLOBAL)lpParameter->wpExpanded);
    }
  }
  StackClear((stack **)&hParamStack->first, (stack **)&hParamStack->last);
  hParamStack->nElements=0;
}
#endif

/********************************************************************
 *
 *  MethodGetName
 *
 *Get method name.
 *
 * [in] const wchar_t *wpText    -Text.
 *[out] wchar_t *wszMethod       -Pointer to buffer that receives the method name. If this value is NULL, the function returns the required buffer size in characters.
 * [in] int nMethodMax           -Specifies the maximum number of characters to copy to the buffer, including the NULL character.
 *[out] const wchar_t **wppText  -Pointer to the text after name. Can be NULL.
 *
 *Returns: number of characters copied, not including the terminating NULL character.
 *
 *Example:
 *  const wchar_t *wpText=L"Call(\"Scripts::Main\", 1, \"SearchReplace.js\")";
 *  wchar_t wszMethod[MAX_PATH];
 *
 *  MethodGetName(wpText, wszMethod, MAX_PATH, &wpText);
 *  //wszMethod == "Call"
 *  //wpText == L"\"Scripts::Main\", 1, \"SearchReplace.js\")"
 ********************************************************************/
#if defined MethodGetName || defined ALLMETHODFUNC
#define MethodGetName_INCLUDED
#undef MethodGetName
__inline int MethodGetName(const wchar_t *wpText, wchar_t *wszMethod, int nMethodMax, const wchar_t **wppText)
{
  const wchar_t *wpCount;

  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  for (wpCount=wpText; *wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\0'; ++wpCount)
  {
    if (*wpCount == L'(')
    {
      if (wppText)
        *wppText=wpCount + 1;
      return (int)xstrcpynW(wszMethod, wpText, min(nMethodMax, wpCount - wpText + 1));
    }
  }
  if (wszMethod) wszMethod[0]=L'\0';
  return 0;
}
#endif

/********************************************************************
 *
 *  MethodGetScript
 *
 *Get script method name.
 *
 * [in] const wchar_t *wpText    -Text.
 *[out] wchar_t *wszMethod       -Pointer to buffer that receives the method name. If this value is NULL, the function returns the required buffer size in characters.
 * [in] int nMethodMax           -Specifies the maximum number of characters to copy to the buffer, including the NULL character.
 *[out] const wchar_t **wppText  -Pointer to the text after name. Can be NULL.
 *
 *Returns: number of characters copied, not including the terminating NULL character.
 *
 *Example:
 *  const wchar_t *wpText=L"AkelPad.SystemFunction().Call(\"kernel32::GetFileAttributesW\", \"c:\\1.TXT\")";
 *  wchar_t wszMethod[MAX_PATH];
 *
 *  MethodGetScript(wpText, wszMethod, MAX_PATH, &wpText);
 *  //wszMethod == "AkelPad.SystemFunction().Call"
 *  //wpText == L"\"kernel32::GetFileAttributesW\", \"c:\\1.TXT\")"
 ********************************************************************/
#if defined MethodGetScript || defined ALLMETHODFUNC
#define MethodGetScript_INCLUDED
#undef MethodGetScript
__inline int MethodGetScript(const wchar_t *wpText, wchar_t *wszMethod, int nMethodMax, const wchar_t **wppText)
{
  const wchar_t *wpCount;

  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  for (wpCount=wpText; *wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\0'; ++wpCount)
  {
    if (*wpCount == L'(' && !(*(wpCount + 1) == L')' && *(wpCount + 2) == L'.'))
      break;
  }
  if (wppText)
  {
    if (*wpCount == L'(')
      *wppText=wpCount + 1;
    else
      *wppText=wpCount;
  }
  return (int)xstrcpynW(wszMethod, wpText, min(nMethodMax, wpCount - wpText + 1));
}
#endif

/********************************************************************
 *
 *  MethodGetIcon
 *
 *Get icon method parameters.
 *
 * [in] const wchar_t *wpText    -Text. Usually points to a first symbol after "Icon(", for example: "C:\MyIcons\IconPack.dll", 2)
 *[out] wchar_t *wszIconFile     -Pointer to buffer that receives the icon file. If this value is NULL, the function returns the required buffer size in characters.
 * [in] int nMaxIconFile         -Specifies the maximum number of characters to copy to the buffer, including the NULL character.
 *[out] int *nIconIndex          -Icon index in icon file.
 *[out] const wchar_t **wppText  -Pointer to the text after icon method. Can be NULL.
 *
 *Returns: no return value.
 ********************************************************************/
#if defined MethodGetIcon || defined ALLMETHODFUNC
#define MethodGetIcon_INCLUDED
#undef MethodGetIcon
__inline void MethodGetIcon(const wchar_t *wpText, wchar_t *wszIconFile, int nMaxIconFile, int *nIconIndex, const wchar_t **wppText)
{
  const wchar_t *wpFileEnd;
  wchar_t wchStopChar;

  wszIconFile[0]=L'\0';
  *nIconIndex=0;

  //File
  MethodComment(wpText, &wpText);

  if (*wpText == L'\"' || *wpText == L'\'' || *wpText == L'`')
  {
    wchStopChar=*wpText++;
    for (wpFileEnd=wpText; *wpFileEnd != wchStopChar && *wpFileEnd != L'\0'; ++wpFileEnd);
    xstrcpynW(wszIconFile, wpText, min(nMaxIconFile, wpFileEnd - wpText + 1));
    if (*wpFileEnd)
      wpText=wpFileEnd + 1;
    else
      wpText=wpFileEnd;
    MethodComment(wpText, &wpText);
    if (*wpText == L',')
      ++wpText;
    else if (*wpText != L')')
      goto End;
  }

  //Index
  MethodComment(wpText, &wpText);
  *nIconIndex=(int)xatoiW(wpText, &wpText);
  MethodComment(wpText, &wpText);

  End:
  if (*wpText == L')')
    ++wpText;
  if (wppText) *wppText=wpText;
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
#include "StackFunc.h"
#include "StrFunc.h"
#include "WideFunc.h"
#include "MethodFunc.h"

//Include stack functions
#define StackInsertBefore
#define StackGetElement
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xstrlenW
#define xstrcpynW
#define xstrcmpiW
#define xatoiW
#define hex2decW
#include "StrFunc.h"

//Include wide functions
#define ANYWIDEFUNC_INCLUDED
#include "WideFunc.h"

//Include method functions
#define ALLMETHODFUNC
#include "MethodFunc.h"

void main()
{
  STACKEXTPARAM hParamStack={0};
  wchar_t wszMethod[MAX_PATH];
  wchar_t wszIconFile[MAX_PATH];
  EXTPARAM *lpParameter;
  const wchar_t *wpText=L"Call(\"Scripts::Main\", 1, \"SearchReplace.js\") Icon(\"%a\\AkelFiles\\Plugs\\Scripts.dll\", 0)";
  int nIconIndex;

  //Initialize WideFunc.h header
  WideInitialize();

  while (MethodGetName(wpText, wszMethod, MAX_PATH, &wpText))
  {
    if (!xstrcmpiW(wszMethod, L"Call"))
    {
      if (MethodParseParameters(&hParamStack, wpText, &wpText))
      {
        for (lpParameter=hParamStack.first; lpParameter; lpParameter=lpParameter->next)
        {
          if (lpParameter->dwType == EXTPARAM_CHAR)
            printf("%ls: lpParameter->wpString={%ls}\n", wszMethod, lpParameter->wpString);
          else
            printf("%ls: lpParameter->nNumber={%d}\n", wszMethod, lpParameter->nNumber);
        }
      }
    }
    else if (!xstrcmpiW(wszMethod, L"Icon"))
    {
      MethodGetIcon(wpText, wszIconFile, MAX_PATH, &nIconIndex, &wpText);
      printf("%ls: wszIconFile={%ls}, nIconIndex={%d}\n", wszMethod, wszIconFile, nIconIndex);
    }
  }
}

*/

#endif /* _METHODFUNC_H_ */
