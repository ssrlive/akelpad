//// Text calculator (based on wisgest code).
//
// How to use:
// Select expression and call script.
//
// Constants:
// E       -Euler's constant, the base of natural logarithms. Approximately equal to 2.718.
// LN2     -The natural logarithm of 2. Approximately equal to 0.693.
// LN10    -The natural logarithm of 10. Approximately equal to 2.302.
// LOG2E   -The base-2 logarithm of e, Euler's constant. Approximately equal to 1.442.
// LOG10E  -The base-10 logarithm of e, Euler's constant. Approximately equal to 0.434.
// PI      -The ratio of the circumference of a circle to its diameter. Approximately equal to 3.141592653589793.
// SQRT1_2 -The square root of 0.5, or one divided by the square root of 2. Approximately equal to 0.707.
// SQRT2   -The square root of 2. Approximately equal to 1.414.
//
// Functions:
// abs     -The absolute value of a number.
// acos    -The arccosine of a number.
// asin    -The arcsine of a number.
// atan    -The arctangent of a number.
// atan2   -The angle (in radians) from the X axis to a point (y,x).
// ceil    -The smallest integer greater than or equal to its numeric argument.
// cos     -The cosine of a number.
// exp     -e (the base of natural logarithms) raised to a power.
// floor   -The greatest integer less than or equal to its numeric argument.
// log     -The natural logarithm of a number.
// max     -The greater of zero or more supplied numeric expressions.
// min     -The lesser of zero or more supplied numeric expressions.
// pow     -The value of a base expression taken to a specified power.
// random  -A pseudorandom number between 0 and 1.
// round   -A supplied numeric expression rounded to the nearest integer.
// sin     -The sine of a number.
// sqrt    -The square root of a number.
// tan     -The tangent of a number.
//
// Examples:
// 56 * 13
// Result: 728
//
// sin(PI/2)
// Result: 1
//
// res=0; for (i=1; i<=500; ++i) res=res+i*i;
// Result: 41791750
//
// x=34+56; Trace(x, "Intermediate result:"); x+26;
// Result: 116
//
// Trace(34+56)+26
// Result: 116
//
//
//// Текстовый калькулятор (основан на коде wisgest'a).
//
// Как использовать:
// Выделите выражение и вызовите скрипт.
//
// Константы:
// E       -Основание натуральных логарифмов, значение приблизительно равно 2.718. Иногда ее еще называют Эйлеровой константой.
// LN2     -Константа, определяющая натуральный логарифм числа 2, приблизительно равна 0.693.
// LN10    -Константа, определяющая натуральный логарифм числа десять, приблизительно равна 2.302.
// LOG2E   -Константа, представляющая логарифм с основанием 2 числа E, приблизительно равна 1.442.
// LOG10E  -Константа, представляющая логарифм с основанием 10 числа E, приблизительно равна 0.434.
// PI      -Значение pi - это отношение длины окружности к диаметру, приблизительно равное 3.14159.
// SQRT1_2 -Квадратный корень из 0.5, приблизительно равно 0.707.
// SQRT2   -Квадратный корень из числа два, приблизительно равно 1.414.
//
// Функции:
// abs     -Возвращает абсолютное значение указанного числа.
// acos    -Возвращает угол, косинус которого равен указанному числу.
// asin    -Возвращает угол, синус которого равен указанному числу.
// atan    -Возвращает угол, тангенс которого равен указанному числу.
// atan2   -Возвращает угол, тангенс которого равен отношению двух указанных чисел.
// ceil    -Возвращает наименьшее целое число, которое больше или равно указанному числу.
// cos     -Возвращает косинус указанного угла.
// exp     -Возвращает e, возведенное в указанную степень.
// floor   -Возвращает наибольшее целое число, которое меньше или равно указанному числу.
// log     -Возвращает логарифм указанного числа.
// max     -Возвращает наибольшее из двух чисел.
// min     -Возвращает наименьшее из двух чисел.
// pow     -Возвращает указанное число, возведенное в указанную степень.
// random  -Возвращает случайное число в интервале между 0 и 1.
// round   -Возвращает ближайшее к указанному значению число.
// sin     -Возвращает синус указанного угла.
// sqrt    -Возвращает квадратный корень из указанного числа.
// tan     -Возвращает тангенс указанного угла.
//
// Примеры:
// 56 * 13
// Результат: 728
//
// sin(PI/2)
// Результат: 1
//
// res=0; for (i=1; i<=500; ++i) res=res+i*i;
// Результат: 41791750
//
// x=34+56; Trace(x, "Промежуточный результат:"); x+26;
// Результат: 116
//
// Trace(34+56)+26
// Результат: 116

//Variables
var hMainWnd=AkelPad.GetMainWnd();
var pSelText=AkelPad.GetSelText();
var bHex=false;
var nResult=0;
var nError;
var WM_USER=1024;

if (pSelText)
{
  pSelText=pSelText.replace(/(\d),(\d)/g, "$1.$2");
  if (pSelText.substr(0, 2) == "0x")
    bHex=true;

  try
  {
    with (Math)
    {
      nResult=eval(pSelText);
    }
  }
  catch (nError)
  {
    AkelPad.MessageBox(hMainWnd, nError.description, WScript.ScriptName, 16 /*MB_ICONERROR*/);
    WScript.Quit();
  }
  Trace(nResult, GetLangString(1));
}
else
{
  AkelPad.MessageBox(hMainWnd, GetLangString(0), WScript.ScriptName, 48 /*MB_ICONEXCLAMATION*/);
}


//Functions
function Trace(nResult, pLabel)
{
  var nValue;

  if (bHex)
  {
    if (nResult < 0) nResult=(0xFFFFFFFF + 1) + nResult;
    nResult="0x" + nResult.toString(16).toUpperCase();
  }
  if (nValue=AkelPad.InputBox(hMainWnd, WScript.ScriptName, pLabel, nResult))
    return parseInt(nValue);
  return nResult;
}

function GetLangString(nStringID)
{
  var nLangID=AkelPad.GetLangId(1 /*LANGID_PRIMARY*/);

  if (nLangID == 0x19) //LANG_RUSSIAN
  {
    if (nStringID == 0)
      return "\u041E\u0442\u0441\u0443\u0442\u0441\u0442\u0432\u0443\u0435\u0442\u0020\u0432\u044B\u0434\u0435\u043B\u0435\u043D\u043D\u044B\u0439\u0020\u0442\u0435\u043A\u0441\u0442\u002E";
    if (nStringID == 1)
      return "\u0418\u0442\u043E\u0433\u043E\u0432\u044B\u0439\u0020\u0440\u0435\u0437\u0443\u043B\u044C\u0442\u0430\u0442\u003A";
  }
  else
  {
    if (nStringID == 0)
      return "No text selected.";
    if (nStringID == 1)
      return "Final result:";
  }
  return "";
}
