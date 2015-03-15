// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.1
// Author: Shengalts Aleksander aka Instructor
//
//
// Description(1033): Evaluates an escaped expression passed through command line.
//
// Variables:
// %%f  active file.
// %%d  directory of active file.
// %%a  AkelPad's directory.
// %%xx escape symbol xx is equivalent to the hexadecimal number representing the character.
//      For example, "%%5Cn" equal to new line character "\n".
//
// Usage:
// Call("Scripts::Main", 1, "EvalCmdEsc.js", `AkelPad.OpenFile("C:\File.txt")`)
//
//
// Description(1049): Обработать экранированное выражение, переданное через командную строку.
//
// Переменные:
// %%f  активный файл.
// %%d  директория активного файла.
// %%a  директория AkelPad'а.
// %%xx символ экранирования xx эквивалентно шестнадцатеричному коду символа.
//      Например, "%%5Cn" эквивалентно символу новой строки "\n".
//
// Применение:
// Call("Scripts::Main", 1, "EvalCmdEsc.js", `AkelPad.OpenFile("C:\File.txt")`)

var pArgLine;

if (pArgLine=AkelPad.GetArgLine())
{
  AkelPad.ScriptNoMutex();
  pArgLine=pArgLine.replace(/\\/g, "\\\\");

  if (/%f|%d|%a/.test(pArgLine))
  {
    var pFile=AkelPad.GetEditFile(0).replace(/\\/g, "\\\\");
    var pFileDir=pFile.substr(0, pFile.lastIndexOf("\\\\"));
    var pAkelDir=AkelPad.GetAkelDir().replace(/\\/g, "\\\\");

    pArgLine=pArgLine.replace(/%f/g, pFile);
    pArgLine=pArgLine.replace(/%d/g, pFileDir);
    pArgLine=pArgLine.replace(/%a/g, pAkelDir);
  }
  pArgLine=unescape(pArgLine);
  eval(pArgLine);
}
