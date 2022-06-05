// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.1
// Author: Shengalts Aleksander aka Instructor
//
//
// Description(1033): Convert keyboard layout or transliterate text.
//
// Arguments:
// -Type=Layout       -Convert keyboard layout.
// -Type=Translit     -Transliteration.
// -Direction=En->Ru  -From English to Russian.
// -Direction=Ru->En  -From Russian to English.
//
// Usage:
// "Convert layout" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Layout -Direction=En->Ru`)
// "Transliteration" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Translit -Direction=Ru->En`)
//
//
// Description(1049): Исправляет раскладку клавиатуры или транслитерирует текст.
//
// Аргументы:
// -Type=Layout       -Исправление раскладки клавиатуры.
// -Type=Translit     -Транслитерация.
// -Direction=En->Ru  -Из английского в русский.
// -Direction=Ru->En  -Из русского в английский.
//
// Применение:
// "Исправить набор" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Layout -Direction=En->Ru`)
// "Транслитерация" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Translit -Direction=Ru->En`)

//Arguments
var pType=AkelPad.GetArgValue("Type", "").toLowerCase();
var pDirection=AkelPad.GetArgValue("Direction", "").toLowerCase();

//Include
if (!AkelPad.Include("ShowMenu.js")) WScript.Quit();

//Variables
var hMainWnd=AkelPad.GetMainWnd();
var hWndEdit=AkelPad.GetEditWnd();
var oSys=AkelPad.SystemFunction();
var hInstanceDLL=AkelPad.GetInstanceDll();
var nSelStart;
var nSelEnd;
var nFirstLine;
var pSelText;

if (hWndEdit)
{
  if (pType == "")
  {
    var aItems=[];
    var nItem;

    //Show menu
    aItems[0]=["Cnhjrf->Строка", MF_NORMAL, 0];
    aItems[1]=["Ыекштп->String", MF_NORMAL, 1];
    aItems[2]=["Stroka->Строка", MF_NORMAL, 2];
    aItems[3]=["Строка->Stroka", MF_NORMAL, 3];

    if ((nItem=ShowMenu(aItems, POS_CARET, POS_CARET)) != -1)
    {
      if (nItem == 0)
      {
        pType="layout";
        pDirection="en->ru";
      }
      else if (nItem == 1)
      {
        pType="layout";
        pDirection="ru->en";
      }
      else if (nItem == 2)
      {
        pType="translit";
        pDirection="en->ru";
      }
      else if (nItem == 3)
      {
        pType="translit";
        pDirection="ru->en";
      }
    }
  }
  if (!pType) WScript.Quit();

  //Get selection
  nSelStart=AkelPad.GetSelStart();
  nSelEnd=AkelPad.GetSelEnd();
  if (nSelStart == nSelEnd)
    AkelPad.SetSel(0, -1);
  pSelText=AkelPad.GetSelText();

  if (pType == "layout")
  {
    if (pDirection == "en->ru")
    {
      // Convert keyboard layout En->Ru
      var pArraySource=new Array("'", ",", ".", ":", ";", "<", ">", "\"", "[", "]", "`", "{", "}", "~", "#", "@",  "^", "$", "?", "&", "/", "|", "A", "a", "B", "b", "C", "c", "D", "d", "E", "e", "F", "f", "G", "g", "H", "h", "I", "i", "J", "j", "K", "k", "L", "l", "M", "m", "N", "n", "O", "o", "P", "p", "Q", "q", "R", "r", "S", "s", "T", "t", "U", "u", "V", "v", "W", "w", "X", "x", "Y", "y", "Z", "z");
      var pArrayTarget=new Array("э", "б", "ю", "Ж", "ж", "Б", "Ю", "Э",  "х", "ъ", "ё", "Х", "Ъ", "Ё", "№", "\"", ":", ";", ",", "?", ".", "/", "Ф", "ф", "И", "и", "С", "с", "В", "в", "У", "у", "А", "а", "П", "п", "Р", "р", "Ш", "ш", "О", "о", "Л", "л", "Д", "д", "Ь", "ь", "Т", "т", "Щ", "щ", "З", "з", "Й", "й", "К", "к", "Ы", "ы", "Е", "е", "Г", "г", "М", "м", "Ц", "ц", "Ч", "ч", "Н", "н", "Я", "я");

      pSelText=ConvertLayout(pSelText, pArraySource, pArrayTarget);
    }
    else if (pDirection == "ru->en")
    {
      // Convert keyboard layout Ru->En
      var pArraySource=new Array("\"", ":", ";", "?", ",", "/", ".", "э", "б", "ю", "Ж", "ж", "Б", "Ю", "Э",  "х", "ъ", "ё", "Х", "Ъ", "Ё", "№", "Ф", "ф", "И", "и", "С", "с", "В", "в", "У", "у", "А", "а", "П", "п", "Р", "р", "Ш", "ш", "О", "о", "Л", "л", "Д", "д", "Ь", "ь", "Т", "т", "Щ", "щ", "З", "з", "Й", "й", "К", "к", "Ы", "ы", "Е", "е", "Г", "г", "М", "м", "Ц", "ц", "Ч", "ч", "Н", "н", "Я", "я");
      var pArrayTarget=new Array("@",  "^", "$", "&", "?", "|", "/", "'", ",", ".", ":", ";", "<", ">", "\"", "[", "]", "`", "{", "}", "~", "#", "A", "a", "B", "b", "C", "c", "D", "d", "E", "e", "F", "f", "G", "g", "H", "h", "I", "i", "J", "j", "K", "k", "L", "l", "M", "m", "N", "n", "O", "o", "P", "p", "Q", "q", "R", "r", "S", "s", "T", "t", "U", "u", "V", "v", "W", "w", "X", "x", "Y", "y", "Z", "z");

      pSelText=ConvertLayout(pSelText, pArraySource, pArrayTarget);
    }
  }
  else if (pType == "translit")
  {
    if (pDirection == "en->ru")
    {
      // Transliteration Latin->Cyrillic
      var pArraySourceMulti=new Array("jo", "yo", "\xF6", "ch", "w", "shh", "sh", "je", "\xE4", "ju", "yu", "\xFC", "ja", "ya", "zh", "ts");
      var pArrayTargetMulti=new Array("ё",  "ё",  "ё",    "ч",  "щ", "щ",   "ш",  "э",  "э",    "ю",  "ю",  "ю",    "я",  "я",  "ж",  "ц");
      var pArraySourceSingle=new Array("c", "h", "x", "j", "'", "y", "#", "`", "a", "b", "v", "g", "d", "e", "z", "i", "k", "l", "m", "n", "o", "p", "r", "s", "t", "u", "f");
      var pArrayTargetSingle=new Array("ц", "х", "х", "й", "ь", "ы", "ъ", "ъ", "а", "б", "в", "г", "д", "е", "з", "и", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у", "ф");

      pSelText=Transliterate(pSelText, pArraySourceMulti, pArrayTargetMulti);
      pSelText=Transliterate(pSelText, pArraySourceSingle, pArrayTargetSingle);
    }
    else if (pDirection == "ru->en")
    {
      // Transliteration Cyrillic->Latin
      var pArraySource=new Array("а", "б", "в", "г", "д", "е", "ё",  "ж",  "з", "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у", "ф", "х", "ц",  "ч",  "ш",  "щ",   "ъ", "ы", "ь", "э",  "ю",  "я");
      var pArrayTarget=new Array("a", "b", "v", "g", "d", "e", "jo", "zh", "z", "i", "j", "k", "l", "m", "n", "o", "p", "r", "s", "t", "u", "f", "h", "ts", "ch", "sh", "shh", "`", "y", "'", "je", "ju", "ja");

      pSelText=Transliterate(pSelText, pArraySource, pArrayTarget);
    }
  }
  AkelPad.ReplaceSel(pSelText, -2);
}


//Functions
function ConvertLayout(pText, pArraySource, pArrayTarget)
{
  var oPattern;
  var i;

  for (i=0; i < pArraySource.length; ++i)
  {
    oPattern=new RegExp(EscRegExp(pArraySource[i]), "g");
    pText=pText.replace(oPattern, pArrayTarget[i]);
  }
  return pText;
}

function Transliterate(pText, pArraySource, pArrayTarget)
{
  var oPattern;
  var i;

  for (i=0; i < pArraySource.length; ++i)
  {
    oPattern=new RegExp(EscRegExp(pArraySource[i]), "g");
    pText=pText.replace(oPattern, pArrayTarget[i]);
  }
  for (i=0; i < pArraySource.length; ++i)
  {
    oPattern=new RegExp(EscRegExp(pArraySource[i]), "gi");
    pText=pText.replace(oPattern, pArrayTarget[i].substr(0, 1).toUpperCase() + pArrayTarget[i].substr(1));
  }
  return pText;
}

function EscRegExp(pString)
{
  return pString.replace(/[.?*+^$[\]\\(){}|-]/g, "\\$&");
}
