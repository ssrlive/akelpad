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
  DWORD dwOptions=RESE_MATCHCASE|RESE_GLOBAL|RESE_MULTILINE;

  //Test compilation
  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\x", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\x6", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\x62", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a[x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\x{", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\x{}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\x{62}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a[x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\x{0062}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a[x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\u", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\u0", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\u00", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\u006", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\u0062", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a[x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\1", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"b\\A", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"b\\a", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\Za", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\za", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"]", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"[]", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"[a-]", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"[-a]", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"[[]", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?<=a|b)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?<!a|b)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?^a|b)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L")", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"()", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(a)(?(1)bc)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a(?(1)bc)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(a)(?(1))", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(a)(?(1)b|c|d)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a(?=b)*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a(?!b)*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a(?<=b)*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a(?<!b)*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"?", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"{", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a{a}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a{1,a}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a{2,1}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a{0}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?<=a+)b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?<!a+)b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?^a+)b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?a)b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?<a)b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L".*123)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  //Test execution
  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(a*)+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x][x]b[x]c[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123489 7890", L"(23)(.*?)(89)", L"[\\1]<\\3>", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1[23]<89> 7890")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123489 7890", L"(23)(.*)(89)", L"[\\1]<\\3>", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1[23]<89>0")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"1234567890 1234567890", L"(23)(45)", L"[\\1]<\\2>", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1[23]<45>67890 1[23]<45>67890")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"1234567890 1234567890", L"(23)(.*?)(89)", L"[\\1]<\\3>", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1[23]<89>0 1[23]<89>0")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"100 200 ABC400 ABC300 ABC200 ABC500", L"100|ABC(200|300|400)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] 200 [x] [x] [x] ABC500")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"100 200 ABC400 ABC300 ABC200 ABC500", L"100|ABC(200|300|400)", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[] 200 [400] [300] [200] ABC500")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"BBAAABB", L"(A+)|(B+)", L"[\\2]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[BB][][BB]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"bc de", L"([bc]|x)*+\\s", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]de")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abbc abcc", L"a(bc|b)+c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abbc abcc", L"a(b|bc)+c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc abcc", L"a(bc|b)c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc abcc", L"a(?>bc|b)c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"abc [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"mam,pap", L"(?(?<=a)m|p)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ma[x],[x]ap")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"mam,pap", L"(?(?<=a)m)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ma[x],pap")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"129", L"\\d{1,3}?9", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc aBc aBC ABC", L"a(?i)bc", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x] [x] ABC")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc aBc aBC ABC", L"a(?i:b)c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x] aBC ABC")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc aBc aBC ABC", L"a(?i)b(?-i)c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x] aBC ABC")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abcABC", L"(abc)(?i)\\1", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc aBc\naBC ABC", L"a(?-s).*?", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]\n[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc aBc\naBC ABC", L"(?im-s)^A.*?$", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]\n[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"1299", L"(?U)\\d+9", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]9")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc aBc\naBC ABC", L"(?-m)^a.*$", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc aBc\naBC ABC", L"^a.*?$", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]\n[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC", L"^.*$", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"3513b 3512b", L"\\d+(?<=12)b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"3513b [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123", L"(?>\\d+?)3", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"123")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123 789", L"(?:\\d)(\\d)\\d", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[2] [8]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123 789", L"\\d(\\d)(?=[^\\d])", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1[3] 789")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123 789", L"\\d(\\d)(?=[^\\d]|$)", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1[3] 7[9]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123ABC 789DEF", L"\\d(\\d)(?=ABC)", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1[3]ABC 789DEF")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123ABC 789DEF", L"\\d(\\d)(?!ABC)", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[2]3ABC [8]9DEF")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC123 DEF789", L"(?<=EF)(\\d+)", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ABC123 DEF[789]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC123 DEF789", L"(?<!EF)(\\d+)", L"[\\1]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ABC[123] DEF7[89]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC123 DEF789", L"(?^ABC)(\\d\\d\\d)", L"[\\1]<\\2>", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ABC123 [DEF]<789>")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC", L"(?^AB)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"A[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"($a*)+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"abc[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc ABC", L"[\\w]+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"123Z 123", L"\\d+Z?", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"AAB123", L"(A+?|.*?)123", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"AAB123123", L"(A+?|.*?)123", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x][x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"AAB123123", L"(A+?|.*)123", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"", L"$", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"[a]c[/a] [b]c[/b]", L"\\[(/?)b\\]", L"[\\1a]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[a]c[/a] [a]c[/a]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aa bb cc", L"([ab])[\\1]", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x] cc")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L" 123 123 123x", L"( 123\\b)+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] 123x")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a123a456a789", L"(a(.*?)){3}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a123a456a789a0", L"(a(.*?)){3}", L"(\\1)(\\2)", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"(a123a456a789a0)(789a0)")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a", L"(a.*?){2}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aa", L"(a.*?){2}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aaabbb", L"(a.*?){2}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aaabbb", L"(a(.*?)){2}", L"(\\1)(\\2)", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"(aaabbb)(abbb)")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aaabbb", L"(a((.*?))){2}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aaabbb", L"(a((.*?))){3}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aabbb", L"(a((.*?))){3}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"aabbb")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aaabcc", L"(a(.*?))(c|d|e)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a1 ac ab", L"\\w(?=\\d|c)\\w", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x] [x] ab")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a1 ac ab", L"\\w(?!\\d|c)\\w", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a1 ac [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a123 123", L"1(2)3", L"[x]", dwOptions|RESE_WHOLEWORD, &wpResult);
  if (xstrcmpW(wpResult, L"a123 [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"dar", L"(a|d)+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]r")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"dar", L"(a|d)+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]r")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a1a2a3a4", L"(a.*?){2}\\w", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x][x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a1a2a3a4", L"(a.*?){2}", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"ab?b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"ab?+b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"abc")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abbbc", L"ab?+b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]bc")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abbbc", L"ab??b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]bbc")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"abb?+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]c")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a1a2a3a", L"(a.+)\\d", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]a")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a1a2a3a", L"(a.+?)\\d", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]a3a")) goto Error;

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

  if (nResultTextLen)
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
