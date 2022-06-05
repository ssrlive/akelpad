// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.3
// Author: Shengalts Aleksander aka Instructor
//
// Description(1033): Spell check using Microsoft Word.
// Description(1049): Проверка орфографии, используя Microsoft Word.

//Variables
var Word=new ActiveXObject("Word.application");
var hMainWnd=AkelPad.GetMainWnd();
var hWndEdit=AkelPad.GetEditWnd();
var oSys=AkelPad.SystemFunction();
var pSelText=AkelPad.GetSelText();

if (hWndEdit)
{
  if (Word)
  {
    if (pSelText)
    {
      var oSpellDoc;
      var pMessage;
      var pTextIn;
      var pTextOut;

      Word.Visible=false;
      Word.Options.SuggestSpellingCorrections=true;

      if (oSpellDoc=Word.Documents.Add())
      {
        oSpellDoc.Content.Text=pSelText;
        Word.ActiveWindow.WindowState=2;  //wdWindowStateMinimize
        if (parseInt(Word.Version) >= 15)
        {
          //MS Word 2013 and higher
          Word.Visible=false;
          oSys.Call("user32::SetForegroundWindow", oSpellDoc.ActiveWindow.Hwnd);
        }

        pTextIn=oSpellDoc.Content.Text;
        if (Word.Options.CheckGrammarWithSpelling == true)
          oSpellDoc.CheckGrammar();
        else
          oSpellDoc.CheckSpelling();
        pTextOut=oSpellDoc.Content.Text;

        oSpellDoc.Close(false);
      }
      Word.Quit(true);
      Word=0;

      if (pTextIn == pTextOut)
      {
        AkelPad.MessageBox(hMainWnd, GetLangString(2), WScript.ScriptName, 64 /*MB_ICONINFORMATION*/);
      }
      else
      {
        if (AkelPad.MessageBox(hMainWnd, GetLangString(3), WScript.ScriptName, 36 /*MB_ICONQUESTION|MB_YESNO*/) == 6 /*IDYES*/)
        {
          AkelPad.ReplaceSel(pTextOut.substr(0, pTextOut.length - 1), -2);
        }
      }
    }
    else AkelPad.MessageBox(hMainWnd, GetLangString(1), WScript.ScriptName, 48 /*MB_ICONEXCLAMATION*/);
  }
  else AkelPad.MessageBox(hMainWnd, GetLangString(0), WScript.ScriptName, 48 /*MB_ICONEXCLAMATION*/);
}
if (Word) Word.Quit(true);


//Functions
function GetLangString(nStringID)
{
  var nLangID=AkelPad.GetLangId(1 /*LANGID_PRIMARY*/);

  if (nLangID == 0x19) //LANG_RUSSIAN
  {
    if (nStringID == 0)
      return "\u041D\u0435\u043E\u0431\u0445\u043E\u0434\u0438\u043C\u043E\u0020\u0441\u043D\u0430\u0447\u0430\u043B\u0430\u0020\u0443\u0441\u0442\u0430\u043D\u043E\u0432\u0438\u0442\u044C\u0020\u004D\u0069\u0063\u0072\u006F\u0073\u006F\u0066\u0074\u0020\u0057\u006F\u0072\u0064\u0020\u0039\u0037\u0020\u0438\u043B\u0438\u0020\u0432\u044B\u0448\u0435\u002E";
    if (nStringID == 1)
      return "\u041E\u0442\u0441\u0443\u0442\u0441\u0442\u0432\u0443\u0435\u0442\u0020\u0432\u044B\u0434\u0435\u043B\u0435\u043D\u043D\u044B\u0439\u0020\u0442\u0435\u043A\u0441\u0442\u002E";
    if (nStringID == 2)
      return "\u0418\u0441\u043F\u0440\u0430\u0432\u043B\u0435\u043D\u0438\u044F\u0020\u043E\u0442\u0441\u0443\u0442\u0441\u0442\u0432\u0443\u044E\u0442\u002E";
    if (nStringID == 3)
      return "\u0412\u0441\u0442\u0430\u0432\u0438\u0442\u044C\u0020\u0438\u0441\u043F\u0440\u0430\u0432\u043B\u0435\u043D\u043D\u044B\u0439\u0020\u0442\u0435\u043A\u0441\u0442\u003F";
  }
  else
  {
    if (nStringID == 0)
      return "You must first install Microsoft Word 97 or higher.";
    if (nStringID == 1)
      return "No text selected.";
    if (nStringID == 2)
      return "No spelling corrections made.";
    if (nStringID == 3)
      return "Insert corrected text?";
  }
  return "";
}
