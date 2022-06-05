// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.1
// Author: Shengalts Aleksander aka Instructor
//
//
// Description(1033): Column selection counter.
//
// How to use:
// Select column with numbers for processing.
// First selected line number is a initial number.
// Each next line will be increased at defined step.
//  3    3     -9    -9     08    08     0x08     0x08
//  9    4     9     -8     56    09     0x1      0x09
//  2 -> 5     -2 -> -7     50 -> 10     0x100 -> 0x0a
//  0    6     0     -6     08    11     0x4      0x0b
//  5    7     5     -5     78    12     0x2      0x0c
// If line not contain number, then it is not processed.
// If selected only single line, then selected number will be increased at defined step.
// If step equal to zero, then it will be automatically calculated as the difference between second and first number.
//
//
// Description(1049): ”величение чисел (счетчик) вертикального выделени€.
//
//  ак использовать:
// ¬ыделите вертикальный блок с числами, которые необходимо преобразовать.
// „исло в первой выделенной строке €вл€етс€ начальным числом.
//  ажда€ последующа€ строка будет увеличена на величину назначенного шага.
//  3    3     -9    -9     08    08     0x08     0x08
//  9    4     9     -8     56    09     0x1      0x09
//  2 -> 5     -2 -> -7     50 -> 10     0x100 -> 0x0a
//  0    6     0     -6     08    11     0x4      0x0b
//  5    7     5     -5     78    12     0x2      0x0c
// ≈сли строка не содержит числа, то она не обрабатываетс€.
// ≈сли выделена только одна строка, то выделенное число будет увеличено на величину назначенного шага.
// ≈сли шаг равен нулю, то он будет вычислен автоматически, как разница между вторым и первым числом.

//Arguments
var nStep=AkelPad.GetArgValue("Step", 1);
var nMinDigits=AkelPad.GetArgValue("MinDigits", 0);
var bHexCombine=AkelPad.GetArgValue("HexCombine", false);

//Variables
var hMainWnd=AkelPad.GetMainWnd();
var hWndEdit=AkelPad.GetEditWnd();
var pSelText;
var pLinesArray;
var pFirstNumber="";
var nFirstNumber;
var pCurNumber;
var nCurNumber;
var bSingle=false;
var bMinus=false;
var bHex=false;
var nIndex=0;
var i;

if (hWndEdit)
{
  if (!AkelPad.SendMessage(hWndEdit, 3129 /*AEM_GETLINENUMBER*/, 12 /*AEGL_UNWRAPSELMULTILINE*/, 0))
  {
    bSingle=true;
    if (!nStep) nStep=1;
  }
  if (AkelPad.SendMessage(hWndEdit, 3127 /*AEM_GETCOLUMNSEL*/, 0, 0) || bSingle)
  {
    pSelText=AkelPad.GetSelText();

    if (pLinesArray=pSelText.split("\r"))
    {
      do
      {
        if (pLinesArray[nIndex].match(/^\s*(-{0,1}(0x[\da-fA-F]+|\d+))/) && !isNaN(parseInt(RegExp.$1)))
        {
          if (!pFirstNumber || (!nStep && typeof nFirstNumber == "undefined"))
          {
            if (pFirstNumber) nFirstNumber=nCurNumber;
            pFirstNumber=RegExp.$1;

            if (pFirstNumber.substr(0, 1) == "-")
            {
              pFirstNumber=pFirstNumber.substr(1);
              bMinus=true;
            }
            if (pFirstNumber.substr(0, 2) == "0x")
            {
              if (!nMinDigits) nMinDigits=pFirstNumber.length - 2;
              nCurNumber=parseInt(pFirstNumber);
              bHex=true;
            }
            else if (pFirstNumber.substr(0, 1) == "0" && pFirstNumber.length > 1)
            {
              if (!nMinDigits) nMinDigits=pFirstNumber.length;
              nCurNumber=parseInt(pFirstNumber.replace(/^0+/, "") || 0);
            }
            else nCurNumber=parseInt(pFirstNumber);

            if (bMinus) nCurNumber=-nCurNumber;
            if (!nStep && typeof nFirstNumber != "undefined")
              nStep=nCurNumber - nFirstNumber;
          }
          if (bSingle)
          {
            if (bHex && bHexCombine)
            {
              if (nCurNumber < 0)
                nCurNumber=parseInt(nCurNumber / (nStep * 2));
              else
                nCurNumber*=nStep * 2;
            }
            else nCurNumber=nCurNumber + nStep;
          }

          if (bHex)
            pCurNumber="" + mod(nCurNumber).toString(16);
          else
            pCurNumber="" + mod(nCurNumber);
          for (i=pCurNumber.length; i < nMinDigits; ++i)
            pCurNumber="0" + pCurNumber;
          if (bHex) pCurNumber="0x" + pCurNumber;
          if (nCurNumber < 0) pCurNumber="-" + pCurNumber;
          pLinesArray[nIndex]=pLinesArray[nIndex].replace(/^\s*(-{0,1}(0x[\da-fA-F]+|\d+))/, pCurNumber);
          if (bSingle) break;

          if (bHex && bHexCombine)
          {
            if (nCurNumber < 0)
              nCurNumber=parseInt(nCurNumber / (nStep * 2));
            else
              nCurNumber*=nStep * 2;
          }
          else nCurNumber=nCurNumber + nStep;
        }
      }
      while (++nIndex < pLinesArray.length)

      pSelText=pLinesArray.join("\r");
      AkelPad.ReplaceSel(pSelText, -2);
    }
  }
  else AkelPad.MessageBox(hMainWnd, GetLangString(0), WScript.ScriptName, 48 /*MB_ICONEXCLAMATION*/);
}

//Functions
function mod(nNumber)
{
  if (nNumber < 0)
    return -nNumber;
  return nNumber;
}

function GetLangString(nStringID)
{
  var nLangID=AkelPad.GetLangId(1 /*LANGID_PRIMARY*/);

  if (nLangID == 0x19) //LANG_RUSSIAN
  {
    if (nStringID == 0)
      return "\u0422\u0440\u0435\u0431\u0443\u0435\u0442\u0441\u044F\u0020\u0432\u0435\u0440\u0442\u0438\u043A\u0430\u043B\u044C\u043D\u043E\u0435\u0020\u0432\u044B\u0434\u0435\u043B\u0435\u043D\u0438\u0435\u002E";
  }
  else
  {
    if (nStringID == 0)
      return "Required column selection.";
  }
  return "";
}
