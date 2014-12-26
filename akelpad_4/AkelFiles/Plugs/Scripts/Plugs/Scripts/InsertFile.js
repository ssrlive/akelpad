// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.1
// Author: Shengalts Aleksander aka Instructor
//
// Description(1033): Insert contents of a file.
// Description(1049): Вставить содержимое файла.

//Options
var pFilter=GetLangString(0);
var nFilterIndex=2;

//Variables
var hMainWnd=AkelPad.GetMainWnd();
var hWndEdit=AkelPad.GetEditWnd();
var pInitialFile=AkelPad.GetEditFile(0);
var pFile;
var pText;

if (hWndEdit)
{
  if (pFile=FileDialog(true, hMainWnd, pInitialFile, pFilter, nFilterIndex))
  {
    if (pText=AkelPad.ReadFile(pFile))
    {
      AkelPad.ReplaceSel(pText, -2);
    }
  }
}


//Functions
function FileDialog(bOpenTrueSaveFalse, hWnd, pInitialFile, pFilter, nFilterIndex)
{
  var nFlags=0x880804; //OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_EXPLORER|OFN_ENABLESIZING
  var pDefaultExt="txt";
  var lpStructure;
  var lpFilterBuffer;
  var lpFileBuffer;
  var lpExtBuffer;
  var oSys;
  var pResultFile="";
  var nCallResult;

  if (lpFilterBuffer=AkelPad.MemAlloc(256 * _TSIZE))
  {
    AkelPad.MemCopy(lpFilterBuffer, pFilter.substr(0, 255), _TSTR);

    if (lpFileBuffer=AkelPad.MemAlloc(256 * _TSIZE))
    {
      AkelPad.MemCopy(lpFileBuffer, pInitialFile.substr(0, 255), _TSTR);

      if (lpExtBuffer=AkelPad.MemAlloc(256 * _TSIZE))
      {
        AkelPad.MemCopy(lpExtBuffer, pDefaultExt.substr(0, 255), _TSTR);

        if (lpStructure=AkelPad.MemAlloc(_X64?136:76 /*sizeof(OPENFILENAME)*/))
        {
          //Fill structure
          AkelPad.MemCopy(_PtrAdd(lpStructure, 0) /*offsetof(OPENFILENAME, lStructSize)*/, _X64?136:76, 3 /*DT_DWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpStructure, _X64?8:4) /*offsetof(OPENFILENAME, hwndOwner)*/, hWnd, 2 /*DT_QWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpStructure, _X64?24:12) /*offsetof(OPENFILENAME, lpstrFilter)*/, lpFilterBuffer, 2 /*DT_QWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpStructure, _X64?44:24) /*offsetof(OPENFILENAME, nFilterIndex)*/, nFilterIndex, 3 /*DT_DWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpStructure, _X64?48:28) /*offsetof(OPENFILENAME, lpstrFile)*/, lpFileBuffer, 2 /*DT_QWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpStructure, _X64?56:32) /*offsetof(OPENFILENAME, nMaxFile)*/, 256, 3 /*DT_DWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpStructure, _X64?96:52) /*offsetof(OPENFILENAME, Flags)*/, nFlags, 3 /*DT_DWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpStructure, _X64?104:60) /*offsetof(OPENFILENAME, lpstrDefExt)*/, lpExtBuffer, 2 /*DT_QWORD*/);

          if (oSys=AkelPad.SystemFunction())
          {
            //Call dialog
            if (bOpenTrueSaveFalse == true)
              nCallResult=oSys.Call("comdlg32::GetOpenFileName" + _TCHAR, lpStructure);
            else
              nCallResult=oSys.Call("comdlg32::GetSaveFileName" + _TCHAR, lpStructure);

            //Result file
            if (nCallResult) pResultFile=AkelPad.MemRead(lpFileBuffer, _TSTR);
          }
          AkelPad.MemFree(lpStructure);
        }
        AkelPad.MemFree(lpExtBuffer);
      }
      AkelPad.MemFree(lpFileBuffer);
    }
    AkelPad.MemFree(lpFilterBuffer);
  }
  return pResultFile;
}

function GetLangString(nStringID)
{
  var nLangID=AkelPad.GetLangId(1 /*LANGID_PRIMARY*/);

  if (nLangID == 0x19) //LANG_RUSSIAN
  {
    if (nStringID == 0)
      return "\u0422\u0435\u043A\u0441\u0442\u043E\u0432\u044B\u0435\u0020\u0444\u0430\u0439\u043B\u044B\u0020(*.txt)\x00*.txt\x00\u0412\u0441\u0435\u0020\u0444\u0430\u0439\u043B\u044B (*.*)\x00*.*\x00\x00";
  }
  else
  {
    if (nStringID == 0)
      return "Text files (*.txt)\x00*.txt\x00All Files (*.*)\x00*.*\x00\x00";
  }
  return "";
}
