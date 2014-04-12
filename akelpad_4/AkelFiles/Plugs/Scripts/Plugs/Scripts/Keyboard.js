//// Convert keyboard layout or transliterate text.
//
// Arguments:
// -Type=Layout       -Convert keyboard layout.
// -Type=Translit     -Transliteration.
// -Direction=En->Ru  -From English to Russian.
// -Direction=Ru->En  -From Russian to English.
//
// Examples:
// -"Convert layout" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Layout -Direction=En->Ru`)
// -"Transliteration" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Translit -Direction=Ru->En`)
//
//
//// Исправляет раскладку клавиатуры или транслитерирует текст.
//
// Аргументы:
// -Type=Layout       -Исправление раскладки клавиатуры.
// -Type=Translit     -Транслитерация.
// -Direction=En->Ru  -Из английского в русский.
// -Direction=Ru->En  -Из русского в английский.
//
// Примеры:
// -"Исправить набор" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Layout -Direction=En->Ru`)
// -"Транслитерация" Call("Scripts::Main", 1, "Keyboard.js", `-Type=Translit -Direction=Ru->En`)

//Arguments
var pType=AkelPad.GetArgValue("Type", "").toLowerCase();
var pDirection=AkelPad.GetArgValue("Direction", "").toLowerCase();

//Variables
var hMainWnd=AkelPad.GetMainWnd();
var hWndEdit=AkelPad.GetEditWnd();
var oSys=AkelPad.SystemFunction();
var hInstanceDLL=AkelPad.GetInstanceDll();
var nSelStart;
var nSelEnd;
var nFirstLine;
var pSelText;

if (hMainWnd)
{
  //Show menu
  if (pType == "")
  {
    var hWndHidden;
    var hMenu;
    var nItem;
    var ptPoint=[];

    if (hWndHidden=oSys.Call("user32::CreateWindowEx" + _TCHAR, 0, "Static", 0, 0x50000000 /*WS_VISIBLE|WS_CHILD*/, 0, 0, 0, 0, hMainWnd, 0, hInstanceDLL, 0))
    {
      oSys.Call("user32::SetFocus", hWndHidden);
      GetCaretPos(hWndEdit, ptPoint);

      if (hMenu=oSys.Call("user32::CreatePopupMenu"))
      {
        oSys.Call("user32::AppendMenu" + _TCHAR, hMenu, 0x0 /*MF_STRING*/, 1, "Cnhjrf->Строка");
        oSys.Call("user32::AppendMenu" + _TCHAR, hMenu, 0x0 /*MF_STRING*/, 2, "Ыекштп->String");
        oSys.Call("user32::AppendMenu" + _TCHAR, hMenu, 0x0 /*MF_STRING*/, 3, "Stroka->Строка");
        oSys.Call("user32::AppendMenu" + _TCHAR, hMenu, 0x0 /*MF_STRING*/, 4, "Строка->Stroka");
        nItem=oSys.Call("user32::TrackPopupMenu", hMenu, 0x182 /*TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON*/, ptPoint.x, ptPoint.y, 0, hWndHidden, 0);

        if (nItem == 1)
        {
          pType="layout";
          pDirection="en->ru";
        }
        else if (nItem == 2)
        {
          pType="layout";
          pDirection="ru->en";
        }
        else if (nItem == 3)
        {
          pType="translit";
          pDirection="en->ru";
        }
        else if (nItem == 4)
        {
          pType="translit";
          pDirection="ru->en";
        }
        oSys.Call("user32::DestroyMenu", hMenu);
      }
      oSys.Call("user32::DestroyWindow", hWndHidden);
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
  nFirstLine=SaveLineScroll(hWndEdit);
  AkelPad.ReplaceSel(pSelText, true);
  RestoreLineScroll(hWndEdit, nFirstLine);
}


//Functions
function ConvertLayout(pText, pArraySource, pArrayTarget)
{
  var oPattern;
  var i;

  for (i=0; i < pArraySource.length; ++i)
  {
    oPattern=new RegExp(PatternToString(pArraySource[i]), "g");
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
    oPattern=new RegExp(PatternToString(pArraySource[i]), "g");
    pText=pText.replace(oPattern, pArrayTarget[i]);
  }
  for (i=0; i < pArraySource.length; ++i)
  {
    oPattern=new RegExp(PatternToString(pArraySource[i]), "gi");
    pText=pText.replace(oPattern, pArrayTarget[i].substr(0, 1).toUpperCase() + pArrayTarget[i].substr(1));
  }
  return pText;
}

function SaveLineScroll(hWnd)
{
  AkelPad.SendMessage(hWnd, 11 /*WM_SETREDRAW*/, false, 0);
  return AkelPad.SendMessage(hWnd, 3129 /*AEM_GETLINENUMBER*/, 4 /*AEGL_FIRSTVISIBLELINE*/, 0);
}

function RestoreLineScroll(hWnd, nBeforeLine)
{
  if (AkelPad.SendMessage(hWnd, 3129 /*AEM_GETLINENUMBER*/, 4 /*AEGL_FIRSTVISIBLELINE*/, 0) != nBeforeLine)
  {
    var lpScrollPos;
    var nPosY=AkelPad.SendMessage(hWnd, 3198 /*AEM_VPOSFROMLINE*/, 0 /*AECT_GLOBAL*/, nBeforeLine);

    if (lpScrollPos=AkelPad.MemAlloc(_X64?16:8 /*sizeof(POINT64)*/))
    {
      AkelPad.MemCopy(lpScrollPos, -1, 2 /*DT_QWORD*/);
      AkelPad.MemCopy(lpScrollPos + (_X64?8:4), nPosY, 2 /*DT_QWORD*/);
      AkelPad.SendMessage(hWnd, 3180 /*AEM_SETSCROLLPOS*/, 0, lpScrollPos);
      AkelPad.MemFree(lpScrollPos);
    }
  }
  AkelPad.SendMessage(hWnd, 3377 /*AEM_UPDATECARET*/, 0, 0);
  AkelPad.SendMessage(hWnd, 11 /*WM_SETREDRAW*/, true, 0);
  oSys.Call("user32::InvalidateRect", hWnd, 0, true);
}

function PatternToString(pPattern)
{
  var pString="";
  var pCharCode;
  var i;

  for (i=0; i < pPattern.length; ++i)
  {
    pCharCode=pPattern.charCodeAt(i).toString(16);
    while (pCharCode.length < 4) pCharCode="0" + pCharCode;
    pString=pString + "\\u" + pCharCode;
  }
  return pString;
}

function GetCaretPos(hWndEdit, ptPoint)
{
  var lpPoint;

  ptPoint.x=0;
  ptPoint.y=0;

  if (lpPoint=AkelPad.MemAlloc(8 /*sizeof(POINT)*/))
  {
    //Caret position
    AkelPad.SendMessage(hWndEdit, 3190 /*AEM_GETCARETPOS*/, lpPoint, 0);
    ptPoint.x=AkelPad.MemRead(lpPoint, 3 /*DT_DWORD*/);
    ptPoint.y=AkelPad.MemRead(lpPoint + 4, 3 /*DT_DWORD*/);
    AkelPad.MemFree(lpPoint);

    //Caret bottom
    ptPoint.y+=AkelPad.SendMessage(hWndEdit, 3188 /*AEM_GETCHARSIZE*/, 0 /*AECS_HEIGHT*/, 0);

    //In screen coordinates
    ClientToScreen(hWndEdit, ptPoint);
  }
}

function ClientToScreen(hWnd, ptPoint)
{
  var lpPoint;

  if (lpPoint=AkelPad.MemAlloc(8 /*sizeof(POINT)*/))
  {
    AkelPad.MemCopy(lpPoint, ptPoint.x, 3 /*DT_DWORD*/);
    AkelPad.MemCopy(lpPoint + 4, ptPoint.y, 3 /*DT_DWORD*/);
    oSys.Call("user32::ClientToScreen", hWnd, lpPoint);
    ptPoint.x=AkelPad.MemRead(lpPoint, 3 /*DT_DWORD*/);
    ptPoint.y=AkelPad.MemRead(lpPoint + 4, 3 /*DT_DWORD*/);
    AkelPad.MemFree(lpPoint);
  }
}
