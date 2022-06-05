// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.1
// Author: Shengalts Aleksander aka Instructor
//
// Description(1033): Rename current editing file.
// Description(1049): Переименовать текущий файл.

//Variables
var hMainWnd=AkelPad.GetMainWnd();
var hWndEdit=AkelPad.GetEditWnd();
var oSys=AkelPad.SystemFunction();
var pFileFullName=AkelPad.GetEditFile(0);
var pFileName=AkelPad.GetFilePath(pFileFullName, 2 /*CPF_FILENAME*/);
var pFileDir=AkelPad.GetFilePath(pFileFullName, 1 /*CPF_DIR*/);
var pNewFileFullName="";
var lpPoint64;
var lpSel;
var lpCaret;
var nCodePage;
var nBOM;
var dwFlags;

if (hWndEdit)
{
  if (!pFileFullName)
  {
    AkelPad.MessageBox(hMainWnd, GetLangString(1), WScript.ScriptName, 48 /*MB_ICONEXCLAMATION*/);
    WScript.Quit();
  }

  for (;;)
  {
    pFileName=AkelPad.InputBox(hMainWnd, WScript.ScriptName, GetLangString(0), pFileName);
    if (!pFileName)
      WScript.Quit();

    pNewFileFullName=pFileDir + "\\" + pFileName;
    if (pNewFileFullName == pFileFullName)
      WScript.Quit();

    if (IsFileExist(pNewFileFullName))
      AkelPad.MessageBox(hMainWnd, GetLangString(2).replace(/%s/, pNewFileFullName), WScript.ScriptName, 48 /*MB_ICONEXCLAMATION*/);
    else
      break;
  }

  if (lpPoint64=AkelPad.MemAlloc(_X64?16:8 /*sizeof(POINT64)*/))
  {
    if (lpSel=AkelPad.MemAlloc(_X64?56:32 /*sizeof(AESELECTION)*/))
    {
      if (lpCaret=AkelPad.MemAlloc(_X64?24:12 /*sizeof(AECHARINDEX)*/))
      {
        //Get document state
        AkelPad.SendMessage(hWndEdit, 3179 /*AEM_GETSCROLLPOS*/, 0, lpPoint64);
        AkelPad.SendMessage(hWndEdit, 3125 /*AEM_GETSEL*/, lpCaret, lpSel);
        nCodePage=AkelPad.GetEditCodePage(hWndEdit);
        nBOM=AkelPad.GetEditBOM(hWndEdit);

        //Close editing file
        if (AkelPad.SendMessage(hMainWnd, 273 /*WM_COMMAND*/, 4324 /*IDM_WINDOW_FILECLOSE*/, 0))
        {
          //Rename file
          if (!oSys.Call("kernel32::MoveFile" + _TCHAR, pFileFullName, pNewFileFullName))
          {
            AkelPad.MessageBox(hMainWnd, GetLangString(3).replace(/%d/, "" + oSys.GetLastError()), WScript.ScriptName, 48 /*MB_ICONEXCLAMATION*/);
            pNewFileFullName=pFileFullName;
          }

          //Open file
          AkelPad.OpenFile(pNewFileFullName, 0, nCodePage, nBOM);

          //Restore document position
          dwFlags=AkelPad.MemRead(_PtrAdd(lpSel, _X64?48:24) /*offsetof(AESELECTION, dwFlags)*/, 3 /*DT_DWORD*/);
          AkelPad.MemCopy(_PtrAdd(lpSel, _X64?48:24) /*offsetof(AESELECTION, dwFlags)*/, dwFlags | 0x808 /*AESELT_LOCKSCROLL|AESELT_INDEXUPDATE*/, 3 /*DT_DWORD*/);
          AkelPad.SendMessage(hWndEdit, 3126 /*AEM_SETSEL*/, lpCaret, lpSel);
          AkelPad.SendMessage(hWndEdit, 3180 /*AEM_SETSCROLLPOS*/, 0, lpPoint64);
        }
        AkelPad.MemFree(lpCaret);
      }
      AkelPad.MemFree(lpSel);
    }
    AkelPad.MemFree(lpPoint64);
  }
}


//Functions
function IsFileExist(pFile)
{
  if (oSys.Call("kernel32::GetFileAttributes" + _TCHAR, pFile) == -1)
    return false;
  return true;
}

function GetLangString(nStringID)
{
  var nLangID=AkelPad.GetLangId(1 /*LANGID_PRIMARY*/);

  if (nLangID == 0x19) //LANG_RUSSIAN
  {
    if (nStringID == 0)
      return "\u041D\u043E\u0432\u043E\u0435\u0020\u0438\u043C\u044F\u003A";
    if (nStringID == 1)
      return "\u0421\u043F\u0435\u0440\u0432\u0430\u0020\u0441\u043E\u0445\u0440\u0430\u043D\u0438\u0442\u0435\u0020\u0444\u0430\u0439\u043B\u002E";
    if (nStringID == 2)
      return "\u0424\u0430\u0439\u043B \"%s\" \u0443\u0436\u0435\u0020\u0441\u0443\u0449\u0435\u0441\u0442\u0432\u0443\u0435\u0442\u002E";
    if (nStringID == 3)
      return "\u041E\u0448\u0438\u0431\u043A\u0430 MoveFile: \"%d\".";
  }
  else
  {
    if (nStringID == 0)
      return "New name:";
    if (nStringID == 1)
      return "Save file first.";
    if (nStringID == 2)
      return "File \"%s\" already exists.";
    if (nStringID == 3)
      return "MoveFile error: \"%d\".";
  }
  return "";
}
