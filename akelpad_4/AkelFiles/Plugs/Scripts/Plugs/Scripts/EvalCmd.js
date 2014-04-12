//// Evaluates an expression passed through command line.
//
// Variables:
// %%f active file.
// %%d directory of active file.
// %%a AkelPad's directory.
//
// Example:
//  Call("Scripts::Main", 1, "EvalCmd.js", `AkelPad.ReplaceSel("Line 1\nLine 2");`)
//
//
//// Обработать выражение, переданное через командную строку.
//
// Переменные:
// %%f  активный файл.
// %%d  директория активного файла.
// %%a  директория AkelPad'а.
//
// Пример:
//  Call("Scripts::Main", 1, "EvalCmd.js", `AkelPad.ReplaceSel("Line 1\nLine 2");`)

var pArgLine;

if (pArgLine=AkelPad.GetArgLine())
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
  eval(pArgLine);
}
