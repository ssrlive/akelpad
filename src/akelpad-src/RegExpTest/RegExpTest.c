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
  int nStartTime;
  DWORD dwOptions=RESE_MATCHCASE|RESE_GLOBAL|RESE_MULTILINE;

  //http://akelpad.sourceforge.net/forum/viewtopic.php?p=33712#33712
  nLine=__LINE__;
  TextReplaceRE(L"abcd abcd", L"a((?!a).){3,}?", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  //http://akelpad.sourceforge.net/forum/viewtopic.php?p=33584#33584
  nLine=__LINE__;
  TextReplaceRE(L"ab", L"(?!z?a).+b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ab")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ababc", L"(a+b)+c", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  //Test compilation
  nLine=__LINE__;
  TextReplaceRE(L"abc", L"[a][b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

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
  if (xstrcmpW(wpResult, L"abc")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\Za", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"\\za", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"abc")) goto Error;

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

  nLine=__LINE__;
  TextReplaceRE(L"-1+3", L"-|+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"[\\K]", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"a(?(b)bc)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"")) goto Error;

  //Test execution
  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(a*)+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x][x]b[x]c[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"A", L"((\")?)*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]A[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"yAy", L"((y)?)*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x][x]A[x][x]")) goto Error;

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
  if (xstrcmpW(wpResult, L"[x]m[x]a[x][x],[x]p[x]ap[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"<>", L"<(a)?(?(1)=b)>", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"<a=b>", L"<(a)?(?(1)=b)>", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

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
  TextReplaceRE(L"A", L"(?^A)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"A")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"B", L"(?^A)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC", L"(?^AB)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"A[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC123 DEF789", L"(?^ABC)(\\d\\d\\d)", L"[\\1]<\\2>", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ABC123 [DEF]<789>")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ABC123 DEF789", L"(?^ABC){2}(\\d\\d\\d)", L"[\\1]<\\2>", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"ABC1[23 DEF]<789>")) goto Error;

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
  TextReplaceRE(L"A", L"(A*+B?)*+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x][x]")) goto Error;

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

  nLine=__LINE__;
  TextReplaceRE(L"a123a456a789", L"(a\\d+)++a", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a123a456a789")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a123a456a789", L"(a\\d+)+?a", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]456a789")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a123a456a789", L"(a\\d+)+a", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]789")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aaabbbccc", L"a+b+\\Kc+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"aaabbb[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aaabbbccc", L"a+(b+)\\Kc+", L"\\1", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"aaabbbbbb")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"1.2", L"(-?\\d+)\\.\\d+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(?<=a)|(?=a)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]a[x]bc")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"1,2,3", L"([\\d,]+),[^\\n]+", L"(\\1)", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"(1,2)")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"aa", L"a?a", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ab", L"(a?.)b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"ab", L"(a?.)?b", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a>1", L"(a*[^b])?>", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]1")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a>1", L"([^>]*>)?>", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"a[x]1")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"abc", L"(a)b(c)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a", L"a|(?=a)", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"a", L"(?=a)|a", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]a")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"1\r\n2", L"(?<=1\\n)2", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1\r\n[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"1\r\n2", L"(?<=1$\\n)2", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"1\r\n[x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"b, qb, qbq", L"(q?)b\\1", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x], q[x], [x]")) goto Error;

  nLine=__LINE__;
  TextReplaceRE(L"111", L"1\\Z", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"11[x]")) goto Error;

  //POSIX result "[x]"
  //PCRE result "[x]sufficient"
  nLine=__LINE__;
  TextReplaceRE(L"oneselfsufficient", L"one(self)?(selfsufficient)?", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]sufficient")) goto Error;

  //Test performance
  nStartTime=GetTickCount();
  nLine=__LINE__;
  TextReplaceRE(L"Notepad is a wonderful editor, being used to edit files in plain text format, thus it is irreplaceable for programming, designing of Web documents, and so on. However, the one comprised in Windows, is very inconvenient due to several limitations. This program claims to be able to fill up these drawbacks.", L"([^#]*@?)*", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x][x]")) goto Error;
  if (GetTickCount() - nStartTime > 100) goto BadTime;

  nStartTime=GetTickCount();
  nLine=__LINE__;
  TextReplaceRE(L"Notepad is a wonderful editor", L"(\\s*+\\S+)+", L"[x]", dwOptions, &wpResult);
  if (xstrcmpW(wpResult, L"[x]")) goto Error;
  if (GetTickCount() - nStartTime > 100) goto BadTime;

  //Success
  MessageBoxA(NULL, "All tests passed", "RegExpTest", MB_OK|MB_ICONINFORMATION);
  goto Quit;

  Error:
  wsprintfA(szMsg, "Test failed on line %d", nLine + 1);
  MessageBoxA(NULL, szMsg, "RegExpTest", MB_OK|MB_ICONERROR);
  goto Quit;

  BadTime:
  wsprintfA(szMsg, "Performance test failed on line %d", nLine + 1);
  MessageBoxA(NULL, szMsg, "RegExpTest", MB_OK|MB_ICONERROR);
  goto Quit;

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
  pr.wpPat=wpIt;
  pr.wpMaxPat=pr.wpPat + nItLen;
  pr.wpRep=wpWith;
  pr.wpMaxRep=pr.wpRep + nWithLen;
  pr.dwOptions=dwOptions;
  pr.wpDelim=NULL;
  pr.wpText=pr.wpStr;
  pr.wpMaxText=pr.wpMaxStr;
  pr.wpRange=pr.wpStr;
  pr.wpMaxRange=pr.wpMaxStr;
  pr.wpNewLine=NULL;
  pr.nPointCount=0;
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
