// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.2
// Author: Shengalts Aleksander aka Instructor
//
//
// Description(1033): Evaluates an expression passed through command line.
//
// Flags - first number in expression (sum of the following):
// 0x1  replace \ with \\.
// 0x2  expand AkelPad variables:
//        %f active file.
//        %d directory of active file.
//        %a AkelPad's directory.
// 0x4  expand escaped symbols:
//        %xx escape symbol xx is equivalent to the hexadecimal number representing the character.
//        For example, "%5Cn" equal to new line character "\n".
//
// Usage without flags:
// Call("Scripts::Main", 1, "EvalCmd.js", `AkelPad.ReplaceSel("Line 1\nLine 2");`)
//
// Usage with flags:
// Call("Scripts::Main", 1, "EvalCmd.js", `0x1; AkelPad.OpenFile("C:\File.txt");`)
//
//
// Description(1049): Обработать выражение, переданное через командную строку.
//
// Флаги - первое число в выражении (сумма следующих):
// 0x1  заменить \ на \\.
// 0x2  развернуть переменные AkelPad:
//        %f активный файл.
//        %d директория активного файла.
//        %a директория AkelPad'а.
// 0x4  развернуть экранированные символы:
//        %xx символ экранирования xx эквивалентно шестнадцатеричному коду символа.
//        Например, "%5Cn" эквивалентно символу новой строки "\n".
//
// Применение без флагов:
// Call("Scripts::Main", 1, "EvalCmd.js", `AkelPad.ReplaceSel("Line 1\nLine 2");`)
//
// Применение с флагами:
// Call("Scripts::Main", 1, "EvalCmd.js", `0x1; AkelPad.OpenFile("C:\File.txt");`)

var pArgLine;
var dwFlags;

if (pArgLine=AkelPad.GetArgLine())
{
  AkelPad.ScriptNoMutex();

  //Get flags
  dwFlags=parseInt(pArgLine);
  if (!isNaN(dwFlags))
  {
    var bHex=pArgLine.substr(0, 2) == "0x";
    var nSemicolonOffset=dwFlags.toString(bHex?16:10).length + (bHex?2:0);

    if (pArgLine.substr(nSemicolonOffset, 1) == ";")
    {
      pArgLine=pArgLine.substr(nSemicolonOffset + 1);

      if (dwFlags & 0x1)
      {
        pArgLine=pArgLine.replace(/\\/g, "\\\\");
      }
      if (dwFlags & 0x2)
      {
        if (/%f|%d|%a/.test(pArgLine))
        {
          var pFile=AkelPad.GetEditFile(0).replace(/\\/g, "\\\\");
          var pFileDir=pFile.substr(0, pFile.lastIndexOf("\\\\"));
          var pAkelDir=AkelPad.GetAkelDir().replace(/\\/g, "\\\\");

          pArgLine=pArgLine.replace(/%f/g, pFile);
          pArgLine=pArgLine.replace(/%d/g, pFileDir);
          pArgLine=pArgLine.replace(/%a/g, pAkelDir);
        }
      }
      if (dwFlags & 0x4)
      {
        pArgLine=unescape(pArgLine);
      }
    }
  }
  eval(pArgLine);
}
