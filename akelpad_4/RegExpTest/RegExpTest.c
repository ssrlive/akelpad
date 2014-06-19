#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "..\AkelEdit\StrFunc.h"
#include "..\AkelEdit\StackFunc.h"
#include "..\AkelEdit\RegExpFunc.h"

//Include string functions
#define WideCharUpper
#define WideCharLower
#define xmemcpy
#define xstrlenW
#define xstrcpyW
#define xstrcmpW
#define xstrcmpnW
#define xstrcmpinW
#define xatoiW
#define hex2decW
#include "..\AkelEdit\StrFunc.h"

//Include stack functions
#define StackInsertBefore
#define StackDelete
#define StackJoin
#include "..\AkelEdit\StackFunc.h"

//Include RegExp functions
#define RE_FUNCTIONS
#include "..\AkelEdit\RegExpFunc.h"

//Functions prototypes
int TextReplaceRE(const wchar_t *wpStr, const wchar_t *wpIt, const wchar_t *wpWith, DWORD dwOptions, wchar_t **wppResult);

//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

void _WinMain()
{
  char szMsg[MAX_PATH];
  wchar_t *wpResult=NULL;
  int nLine=0;
  DWORD dwOptions=REPE_MATCHCASE|REPE_GLOBAL|REPE_MULTILINE;

  nLine=__LINE__;
  TextReplaceRE(L"BBAAABB", L"(A+)|(B+)", L"[\\2]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[BB][][BB]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc abcc", L"a(bc|b)c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc abcc", L"a(?>bc|b)c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"abc [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"mam,pap", L"(?(?<=a)m|p)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ma[x],[x]ap")) goto Error;

  //Success
  MessageBoxA(NULL, "All tests passed", "RegExpTest", MB_OK|MB_ICONINFORMATION);
  goto Quit;

  Error:
  wsprintfA(szMsg, "Test failed on line %d", nLine + 1);
  MessageBoxA(NULL, szMsg, "RegExpTest", MB_OK|MB_ICONERROR);

  Quit:
  if (wpResult) GlobalFree((HGLOBAL)wpResult);
  ExitProcess(0);
}

int TextReplaceRE(const wchar_t *wpStr, const wchar_t *wpIt, const wchar_t *wpWith, DWORD dwOptions, wchar_t **wppResult)
{
  PATREPLACE pr;
  INT_PTR nStrLen=xstrlenW(wpStr);
  INT_PTR nItLen=xstrlenW(wpIt);
  INT_PTR nWithLen=xstrlenW(wpWith);
  INT_PTR nResultTextLen;

  if (*wppResult) GlobalFree((HGLOBAL)*wppResult);
  *wppResult=(wchar_t *)GlobalAlloc(GPTR, sizeof(wchar_t));

  pr.wpStr=wpStr;
  pr.wpMaxStr=pr.wpStr + nStrLen;
  pr.wpText=pr.wpStr;
  pr.wpMaxText=pr.wpMaxStr;
  pr.wpPat=wpIt;
  pr.wpMaxPat=pr.wpPat + nItLen;
  pr.wpRep=wpWith;
  pr.wpMaxRep=pr.wpRep + nWithLen;
  pr.dwOptions=dwOptions;
  pr.wpDelim=NULL;
  pr.wpNewLine=NULL;
  pr.wszResult=NULL;
  nResultTextLen=PatReplace(&pr);

  if (pr.nReplaceCount)
  {
    if (pr.wszResult=(wchar_t *)GlobalAlloc(GMEM_FIXED, nResultTextLen * sizeof(wchar_t)))
    {
      nResultTextLen=PatReplace(&pr);
      if (*wppResult) GlobalFree((HGLOBAL)*wppResult);
      *wppResult=pr.wszResult;
    }
  }
  return pr.nReplaceCount;
}
