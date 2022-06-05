// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.2
// Author: Shengalts Aleksander aka Instructor
//
// Description(1033): Filter lines using regular expressions.
// Description(1049): ќтфильтровать строки, использу€ регул€рные выражени€.

//Actions
var ACTL_INCLUDE=1;
var ACTL_EXCLUDE=2;

//Options
var bRegExp=true;
var bSensitive=false;
var nAction=ACTL_INCLUDE;
var nSearchStrings=10;

//Control IDs
var IDC_FIND           =1001;
var IDC_REGEXP         =1002;
var IDC_MATCHCASE      =1003;
var IDC_INCLUDE        =1004;
var IDC_EXCLUDE        =1005;
var IDC_ALLFILES       =1006;
var IDC_OK             =1007;
var IDC_CANCEL         =1008;
var IDC_STATIC         =-1;

//Variables
var hMainWnd=AkelPad.GetMainWnd();
var hWndEdit=AkelPad.GetEditWnd();
var oSys=AkelPad.SystemFunction();
var oSet=AkelPad.ScriptSettings();
var pScriptName=WScript.ScriptName;
var hInstanceDLL=AkelPad.GetInstanceDll();
var pClassName="AkelPad::Scripts::" + pScriptName + "::" + oSys.Call("kernel32::GetCurrentProcessId");
var bAkelEdit=AkelPad.IsAkelEdit();
var hWndDialog;
var hWndStatic;
var hWndFind;
var hWndRegExp;
var hWndCase;
var hWndGroup;
var hWndInclude;
var hWndExclude;
var hWndAll;
var hWndOK;
var hWndCancel;
var ptScale=[];
var hGuiFont;
var lpBuffer;
var lpSearchBuffer;
var lpFindStrings=[];
var pFindIt="";
var nFindItLength;

if (hWndEdit)
{
  if (ScriptEngineMajorVersion() <= 5 && ScriptEngineMinorVersion() < 5)
  {
    AkelPad.MessageBox(hMainWnd, GetLangString(0), pScriptName, 48 /*MB_ICONEXCLAMATION*/);
    WScript.Quit();
  }

  if (AkelPad.WindowRegisterClass(pClassName))
  {
    if (lpBuffer=AkelPad.MemAlloc(256 * _TSIZE))
    {
      var sizeNonClient=[];

      sizeNonClient.cx=oSys.Call("user32::GetSystemMetrics", 7 /*SM_CXFIXEDFRAME*/) * 2;
      sizeNonClient.cy=oSys.Call("user32::GetSystemMetrics", 8 /*SM_CYFIXEDFRAME*/) * 2 + oSys.Call("user32::GetSystemMetrics", 4 /*SM_CYCAPTION*/);

      //Get scale factor for ScaleX and ScaleY
      ScaleInit(0, hMainWnd);

      //Create dialog
      hWndDialog=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                           0,                              //dwExStyle
                           pClassName,                     //lpClassName
                           0,                              //lpWindowName
                           0x90CA0000,                     //WS_VISIBLE|WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX
                           ScaleX(0),                      //x
                           ScaleY(0),                      //y
                           ScaleX(350) + sizeNonClient.cx, //nWidth
                           ScaleY(165) + sizeNonClient.cy, //nHeight
                           hMainWnd,                       //hWndParent
                           0,                              //ID
                           hInstanceDLL,                   //hInstance
                           DialogCallback);                //Script function callback. To use it class must be registered by WindowRegisterClass.
      if (hWndDialog)
      {
        //Allow other scripts running
        AkelPad.ScriptNoMutex();

        //Message loop
        AkelPad.WindowGetMessage(0x4 /*WGM_KEYDOWNUP*/);
      }
      AkelPad.MemFree(lpBuffer);
    }
    AkelPad.WindowUnregisterClass(pClassName);
  }
  else if (hWndDialog=oSys.Call("user32::FindWindowEx" + _TCHAR, 0, 0, pClassName, 0))
  {
    if (oSys.Call("user32::IsIconic", hWndDialog))
      oSys.Call("user32::ShowWindow", hWndDialog, 9 /*SW_RESTORE*/);
    AkelPad.SendMessage(hWndDialog, 7 /*WM_SETFOCUS*/, 0, 0);
  }
}

function DialogCallback(hWnd, uMsg, wParam, lParam)
{
  if (uMsg == 1)  //WM_CREATE
  {
    //Read settings
    if (oSet.Begin("", 0x1 /*POB_READ*/))
    {
      bRegExp=oSet.Read("RegExp", 1 /*PO_DWORD*/);
      bSensitive=oSet.Read("Sensitive", 1 /*PO_DWORD*/);
      nAction=oSet.Read("Action", 1 /*PO_DWORD*/);

      //Find
      for (i=0; i < nSearchStrings; ++i)
      {
        lpFindStrings[i]=oSet.Read("Find" + i, 3 /*PO_STRING*/);
        if (typeof lpFindStrings[i] == "undefined")
          break;
      }
      if (typeof lpFindStrings[0] != "undefined")
        pFindIt=lpFindStrings[0];

      oSet.End();
    }

    hGuiFont=oSys.Call("gdi32::GetStockObject", 17 /*DEFAULT_GUI_FONT*/);

    //Dialog caption
    oSys.Call("user32::SetWindowText" + _TCHAR, hWnd, pScriptName);


    ////Static window

    //Create window
    hWndStatic=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                         0,            //dwExStyle
                         "STATIC",     //lpClassName
                         0,            //lpWindowName
                         0x50000000,   //WS_VISIBLE|WS_CHILD
                         ScaleX(14),   //x
                         ScaleY(13),   //y
                         ScaleX(305),  //nWidth
                         ScaleY(13),   //nHeight
                         hWnd,         //hWndParent
                         IDC_STATIC,   //ID
                         hInstanceDLL, //hInstance
                         0);           //lpParam
    //Set font and text
    SetWindowFontAndText(hWndStatic, hGuiFont, GetLangString(3));


    ////Edit window

    //Create window
    hWndFind=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                       0,            //dwExStyle
                       "COMBOBOX",   //lpClassName
                       0,            //lpWindowName
                       0x50210042,   //WS_VISIBLE|WS_CHILD|WS_TABSTOP|WS_VSCROLL|CBS_DROPDOWN|CBS_AUTOHSCROLL
                       ScaleX(11),   //x
                       ScaleY(31),   //y
                       ScaleX(323),  //nWidth
                       ScaleY(160),  //nHeight
                       hWnd,         //hWndParent
                       IDC_FIND,     //ID
                       hInstanceDLL, //hInstance
                       0);           //lpParam
    //Fill combobox
    for (i=0; i < nSearchStrings && typeof lpFindStrings[i] != "undefined"; ++i)
    {
      oSys.Call("user32::SendMessage" + _TCHAR, hWndFind, 0x143 /*CB_ADDSTRING*/, 0, lpFindStrings[i]);
    }

    //Set font and text
    SetWindowFontAndText(hWndFind, hGuiFont, "");
    AkelPad.SendMessage(hWndFind, 0x14E /*CB_SETCURSEL*/, 0, 0);


    ////Checkbox RegExp

    //Create window
    hWndRegExp=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                         0,            //dwExStyle
                         "BUTTON",     //lpClassName
                         0,            //lpWindowName
                         0x50010003,   //WS_VISIBLE|WS_CHILD|WS_TABSTOP|BS_AUTOCHECKBOX
                         ScaleX(14),   //x
                         ScaleY(69),   //y
                         ScaleX(155),  //nWidth
                         ScaleY(16),   //nHeight
                         hWnd,         //hWndParent
                         IDC_REGEXP,   //ID
                         hInstanceDLL, //hInstance
                         0);           //lpParam
    //Set font and text
    SetWindowFontAndText(hWndRegExp, hGuiFont, GetLangString(4));


    ////Checkbox Sensitive

    //Create window
    hWndCase=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                       0,             //dwExStyle
                       "BUTTON",      //lpClassName
                       0,             //lpWindowName
                       0x50010003,    //WS_VISIBLE|WS_CHILD|WS_TABSTOP|BS_AUTOCHECKBOX
                       ScaleX(14),    //x
                       ScaleY(90),    //y
                       ScaleX(155),   //nWidth
                       ScaleY(16),    //nHeight
                       hWnd,          //hWndParent
                       IDC_MATCHCASE, //ID
                       hInstanceDLL,  //hInstance
                       0);            //lpParam
    //Set font and text
    SetWindowFontAndText(hWndCase, hGuiFont, GetLangString(5));


    ////GroupBox 1

    //Create window
    hWndGroup=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                        0,            //dwExStyle
                        "BUTTON",     //lpClassName
                        0,            //lpWindowName
                        0x50000007,   //WS_VISIBLE|WS_CHILD|BS_GROUPBOX
                        ScaleX(180),  //x
                        ScaleY(57),   //y
                        ScaleX(155),  //nWidth
                        ScaleY(56),   //nHeight
                        hWnd,         //hWndParent
                        IDC_STATIC,   //ID
                        hInstanceDLL, //hInstance
                        0);           //lpParam
    //Set font and text
    SetWindowFontAndText(hWndGroup, hGuiFont, "");


    ////Checkbox Include

    //Create window
    hWndInclude=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                          0,            //dwExStyle
                          "BUTTON",     //lpClassName
                          0,            //lpWindowName
                          0x50000009,   //WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON
                          ScaleX(186),  //x
                          ScaleY(71),   //y
                          ScaleX(143),  //nWidth
                          ScaleY(16),   //nHeight
                          hWnd,         //hWndParent
                          IDC_INCLUDE,  //ID
                          hInstanceDLL, //hInstance
                          0);           //lpParam
    //Set font and text
    SetWindowFontAndText(hWndInclude, hGuiFont, GetLangString(6));


    ////Checkbox Exclude

    //Create window
    hWndExclude=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                          0,            //dwExStyle
                          "BUTTON",     //lpClassName
                          0,            //lpWindowName
                          0x50000009,   //WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON
                          ScaleX(186),  //x
                          ScaleY(90),   //y
                          ScaleX(143),  //nWidth
                          ScaleY(16),   //nHeight
                          hWnd,         //hWndParent
                          IDC_EXCLUDE,  //ID
                          hInstanceDLL, //hInstance
                          0);           //lpParam
    //Set font and text
    SetWindowFontAndText(hWndExclude, hGuiFont, GetLangString(7));

    if (AkelPad.IsMDI())
    {
      ////All files button window

      //Create window
      hWndAll=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                        0,            //dwExStyle
                        "BUTTON",     //lpClassName
                        0,            //lpWindowName
                        0x50010000,   //WS_VISIBLE|WS_CHILD|WS_TABSTOP
                        ScaleX(98),   //x
                        ScaleY(130),  //y
                        ScaleX(75),   //nWidth
                        ScaleY(23),   //nHeight
                        hWnd,         //hWndParent
                        IDC_ALLFILES, //ID
                        hInstanceDLL, //hInstance
                        0);           //lpParam
      //Set font and text
      SetWindowFontAndText(hWndAll, hGuiFont, GetLangString(8));
    }

    ////OK button window

    //Create window
    hWndOK=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                     0,            //dwExStyle
                     "BUTTON",     //lpClassName
                     0,            //lpWindowName
                     0x50010001,   //WS_VISIBLE|WS_CHILD|WS_TABSTOP|BS_DEFPUSHBUTTON
                     ScaleX(179),  //x
                     ScaleY(130),  //y
                     ScaleX(75),   //nWidth
                     ScaleY(23),   //nHeight
                     hWnd,         //hWndParent
                     IDC_OK,       //ID
                     hInstanceDLL, //hInstance
                     0);           //lpParam
    //Set font and text
    SetWindowFontAndText(hWndOK, hGuiFont, GetLangString(1));


    ////Cancel button window

    //Create window
    hWndCancel=oSys.Call("user32::CreateWindowEx" + _TCHAR,
                         0,            //dwExStyle
                         "BUTTON",     //lpClassName
                         0,            //lpWindowName
                         0x50010000,   //WS_VISIBLE|WS_CHILD|WS_TABSTOP
                         ScaleX(260),  //x
                         ScaleY(130),  //y
                         ScaleX(75),   //nWidth
                         ScaleY(23),   //nHeight
                         hWnd,         //hWndParent
                         IDC_CANCEL,   //ID
                         hInstanceDLL, //hInstance
                         0);           //lpParam
    //Set font and text
    SetWindowFontAndText(hWndCancel, hGuiFont, GetLangString(2));


    //Checks
    if (bRegExp) AkelPad.SendMessage(hWndRegExp, 241 /*BM_SETCHECK*/, 1 /*BST_CHECKED*/, 0);
    if (bSensitive) AkelPad.SendMessage(hWndCase, 241 /*BM_SETCHECK*/, 1 /*BST_CHECKED*/, 0);
    if (nAction == ACTL_EXCLUDE)
      AkelPad.SendMessage(hWndExclude, 241 /*BM_SETCHECK*/, 1 /*BST_CHECKED*/, 0);
    else
      AkelPad.SendMessage(hWndInclude, 241 /*BM_SETCHECK*/, 1 /*BST_CHECKED*/, 0);

    //Center dialog
    CenterWindow(hMainWnd, hWnd);
  }
  else if (uMsg == 7)  //WM_SETFOCUS
  {
    oSys.Call("user32::SetFocus", hWndFind);
  }
  else if (uMsg == 256)  //WM_KEYDOWN
  {
    if (wParam == 27)  //VK_ESCAPE
    {
      //Escape key pushes Cancel button
      oSys.Call("user32::PostMessage" + _TCHAR, hWndDialog, 273 /*WM_COMMAND*/, IDC_CANCEL, 0);
    }
    else if (wParam == 13)  //VK_RETURN
    {
      if (oSys.Call("user32::IsWindowEnabled", hWndOK))
      {
        //Return key pushes OK button
        oSys.Call("user32::PostMessage" + _TCHAR, hWndDialog, 273 /*WM_COMMAND*/, IDC_OK, 0);
      }
    }
  }
  else if (uMsg == 273)  //WM_COMMAND
  {
    if (LOWORD(wParam) == IDC_FIND)
    {
      var nCurSel;
      var nTextLen;

      if (HIWORD(wParam) == 1 /*CBN_SELCHANGE*/)
      {
        nCurSel=AkelPad.SendMessage(hWndFind, 0x147 /*CB_GETCURSEL*/, 0, 0);
        nTextLen=AkelPad.SendMessage(hWndFind, 0x149 /*CB_GETLBTEXTLEN*/, nCurSel, 0);
      }
      else nTextLen=oSys.Call("user32::GetWindowTextLength" + _TCHAR, hWndFind);

      if (nTextLen)
      {
        oSys.Call("user32::EnableWindow", hWndOK, true);
        if (hWndAll) oSys.Call("user32::EnableWindow", hWndAll, true);
      }
      else
      {
        oSys.Call("user32::EnableWindow", hWndOK, false);
        if (hWndAll) oSys.Call("user32::EnableWindow", hWndAll, false);
      }
    }
    else if (LOWORD(wParam) == IDC_REGEXP ||
             LOWORD(wParam) == IDC_MATCHCASE ||
             LOWORD(wParam) == IDC_INCLUDE ||
             LOWORD(wParam) == IDC_EXCLUDE)
    {
      if (LOWORD(wParam) == IDC_REGEXP)
        bRegExp=AkelPad.SendMessage(hWndRegExp, 240 /*BM_GETCHECK*/, 0, 0);
      else if (LOWORD(wParam) == IDC_MATCHCASE)
        bSensitive=AkelPad.SendMessage(hWndCase, 240 /*BM_GETCHECK*/, 0, 0);
      else if (LOWORD(wParam) == IDC_INCLUDE)
        nAction=ACTL_INCLUDE;
      else if (LOWORD(wParam) == IDC_EXCLUDE)
        nAction=ACTL_EXCLUDE;
    }
    else if (LOWORD(wParam) == IDC_OK ||
             LOWORD(wParam) == IDC_ALLFILES)
    {
      //Find
      nFindItLength=oSys.Call("user32::GetWindowTextLength" + _TCHAR, hWndFind);

      if (lpSearchBuffer=AkelPad.MemAlloc((nFindItLength + 1) * _TSIZE))
      {
        oSys.Call("user32::GetWindowText" + _TCHAR, hWndFind, lpSearchBuffer, nFindItLength + 1);
        pFindIt=AkelPad.MemRead(lpSearchBuffer, _TSTR);

        if (nSearchStrings)
        {
          for (i=0; i < nSearchStrings && typeof lpFindStrings[i] != "undefined"; ++i)
          {
            if (lpFindStrings[i] == pFindIt)
            {
              AkelPad.SendMessage(hWndFind, 0x144 /*CB_DELETESTRING*/, i, 0);
              DeleteFromArray(lpFindStrings, i, 1);
            }
          }
          InsertInArray(lpFindStrings, pFindIt, 0);
          if (lpFindStrings.length > nSearchStrings)
            DeleteFromArray(lpFindStrings, -1, 1);

          //AkelPad.MemCopy(lpSearchBuffer, pFindIt, _TSTR);
          AkelPad.SendMessage(hWndFind, 0x14A /*CB_INSERTSTRING*/, 0, lpSearchBuffer);
          AkelPad.SendMessage(hWndFind, 0x14E /*CB_SETCURSEL*/, 0, 0);
        }
        AkelPad.MemFree(lpSearchBuffer);
      }

      oSys.Call("user32::EnableWindow", hWndOK, false);
      if (hWndAll) oSys.Call("user32::EnableWindow", hWndAll, false);

      if (LOWORD(wParam) == IDC_OK)
        LinesFilter(false);
      else
        LinesFilter(true);

      oSys.Call("user32::PostMessage" + _TCHAR, hWndDialog, 16 /*WM_CLOSE*/, 0, 0);
    }
    else if (LOWORD(wParam) == IDC_CANCEL)
    {
      oSys.Call("user32::PostMessage" + _TCHAR, hWndDialog, 16 /*WM_CLOSE*/, 0, 0);
    }
  }
  else if (uMsg == 16)  //WM_CLOSE
  {
    if (oSet.Begin("", 0x2 /*POB_SAVE*/))
    {
      //Save settings
      oSet.Write("RegExp", 1 /*PO_DWORD*/, bRegExp);
      oSet.Write("Sensitive", 1 /*PO_DWORD*/, bSensitive);
      oSet.Write("Action", 1 /*PO_DWORD*/, nAction);

      //Save find history
      for (i=0; i < nSearchStrings && typeof lpFindStrings[i] != "undefined"; ++i)
        oSet.Write("Find" + i, 3 /*PO_STRING*/, lpFindStrings[i]);

      oSet.End();
    }

    //Destroy dialog
    oSys.Call("user32::DestroyWindow", hWnd);
  }
  else if (uMsg == 2)  //WM_DESTROY
  {
    //Exit message loop
    oSys.Call("user32::PostQuitMessage", 0);
  }
  return 0;
}

function LinesFilter(bAllDocuments)
{
  var lpFrameInit=AkelPad.SendMessage(hMainWnd, 1288 /*AKD_FRAMEFIND*/, 1 /*FWF_CURRENT*/, 0);
  var lpFrameCur;
  var hWndEditCur;
  var oPattern;
  var pSelText;
  var pArray;
  var pResult;
  var nFirstLine;
  var nSelStart;
  var nSelEnd;
  var bAtLineStart=false;
  var bAtLineEnd=false;
  var i;

  if (bRegExp)
  {
    if (pFindIt.charAt(0) == "^")
      bAtLineStart=true;
    else if (pFindIt.charAt(pFindIt.length - 1) == "$")
      bAtLineEnd=true;
  }
  oPattern=new RegExp((bAtLineStart?"":"^.*") + "(" + (bRegExp?pFindIt:EscRegExp(pFindIt)) + ")" + (bAtLineEnd?"":".*$") + "\n?", "gm" + (bSensitive?"":"i"));

  for (;;)
  {
    hWndEditCur=AkelPad.GetEditWnd();
    nFirstLine=SaveLineScroll(hWndEditCur);
    nSelStart=AkelPad.GetSelStart();
    nSelEnd=AkelPad.GetSelEnd();
    if (nSelStart == nSelEnd)
      AkelPad.SetSel(0, -1);
    else
      SelCompliteLine(hWndEditCur, nSelStart, nSelEnd);
    pSelText=AkelPad.GetSelText(2 /*\n*/);
    if (!bAkelEdit)
      pSelText=pSelText.replace(/\r/g, "\n");
    pResult="";

    if (nAction == ACTL_INCLUDE)
    {
      if (pArray=pSelText.match(oPattern))
        pResult=pArray.join("");
    }
    else
    {
      pResult=pSelText.replace(oPattern, "");
    }

    AkelPad.ReplaceSel(pResult);
    RestoreLineScroll(hWndEditCur, nFirstLine);

    if (bAllDocuments)
    {
      //Next MDI frame
      lpFrameCur=AkelPad.Command(4316 /*IDM_WINDOW_FRAMENEXT*/);
      if (!lpFrameCur || lpFrameCur == lpFrameInit)
        break;
    }
    else break;
  }
}


//Functions
function InsertInArray(lpArray, lpItem, nPos)
{
  //For MS JScript backward compatibility
  var i;

  if (nPos < 0) nPos=lpArray.length + nPos + 1;
  if (nPos < 0) nPos=0;
  if (nPos > lpArray.length) nPos=lpArray.length;

  for (i=lpArray.length; i >= 0; --i)
  {
    if (i == nPos)
    {
      lpArray[i]=lpItem;
      break;
    }
    lpArray[i]=lpArray[i - 1];
  }
}

function DeleteFromArray(lpArray, nPos, nCount)
{
  //For MS JScript backward compatibility
  var i;

  if (nPos < 0) nPos=lpArray.length + nPos;
  if (nPos < 0 || nPos >= lpArray.length) return;
  if (nPos + nCount >= lpArray.length) nCount=lpArray.length - nPos;

  for (i=nPos; i + nCount < lpArray.length; ++i)
  {
    lpArray[i]=lpArray[i + nCount];
  }
  lpArray.length-=nCount;
}

function SetWindowFontAndText(hWnd, hFont, pText)
{
  AkelPad.SendMessage(hWnd, 48 /*WM_SETFONT*/, hFont, 1);
  oSys.Call("user32::SetWindowText" + _TCHAR, hWnd, pText);
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
      AkelPad.MemCopy(_PtrAdd(lpScrollPos, 0) /*offsetof(POINT64, x)*/, -1, 2 /*DT_QWORD*/);
      AkelPad.MemCopy(_PtrAdd(lpScrollPos, _X64?8:4) /*offsetof(POINT64, y)*/, nPosY, 2 /*DT_QWORD*/);
      AkelPad.SendMessage(hWnd, 3180 /*AEM_SETSCROLLPOS*/, 0, lpScrollPos);
      AkelPad.MemFree(lpScrollPos);
    }
  }
  AkelPad.SendMessage(hWnd, 3377 /*AEM_UPDATECARET*/, 0, 0);
  AkelPad.SendMessage(hWnd, 11 /*WM_SETREDRAW*/, true, 0);
  oSys.Call("user32::InvalidateRect", hWnd, 0, true);
}

function RectToArray(lpRect, rcRect)
{
  rcRect.left=AkelPad.MemRead(_PtrAdd(lpRect, 0) /*offsetof(RECT, left)*/, 3 /*DT_DWORD*/);
  rcRect.top=AkelPad.MemRead(_PtrAdd(lpRect, 4) /*offsetof(RECT, top)*/, 3 /*DT_DWORD*/);
  rcRect.right=AkelPad.MemRead(_PtrAdd(lpRect, 8) /*offsetof(RECT, right)*/, 3 /*DT_DWORD*/);
  rcRect.bottom=AkelPad.MemRead(_PtrAdd(lpRect, 12) /*offsetof(RECT, bottom)*/, 3 /*DT_DWORD*/);
  return rcRect;
}

function ArrayToRect(rcRect, lpRect)
{
  if (!lpRect)
    lpRect=AkelPad.MemAlloc(16 /*sizeof(RECT)*/);

  if (lpRect)
  {
    AkelPad.MemCopy(_PtrAdd(lpRect, 0) /*offsetof(RECT, left)*/, rcRect.left, 3 /*DT_DWORD*/);
    AkelPad.MemCopy(_PtrAdd(lpRect, 4) /*offsetof(RECT, top)*/, rcRect.top, 3 /*DT_DWORD*/);
    AkelPad.MemCopy(_PtrAdd(lpRect, 8) /*offsetof(RECT, right)*/, rcRect.right, 3 /*DT_DWORD*/);
    AkelPad.MemCopy(_PtrAdd(lpRect, 12) /*offsetof(RECT, bottom)*/, rcRect.bottom, 3 /*DT_DWORD*/);
  }
  return lpRect;
}

function CenterWindow(hWndParent, hWnd)
{
  var lpRect;
  var rcWndParent=[];
  var rcWnd=[];
  var X;
  var Y;

  if (lpRect=AkelPad.MemAlloc(16))  //sizeof(RECT)
  {
    if (!hWndParent)
      hWndParent=oSys.Call("user32::GetDesktopWindow");

    oSys.Call("user32::GetWindowRect", hWndParent, lpRect);
    RectToArray(lpRect, rcWndParent);

    oSys.Call("user32::GetWindowRect", hWnd, lpRect);
    RectToArray(lpRect, rcWnd);

    //Center window
    X=rcWndParent.left + ((rcWndParent.right - rcWndParent.left) / 2 - (rcWnd.right - rcWnd.left) / 2);
    Y=rcWndParent.top + ((rcWndParent.bottom - rcWndParent.top) / 2 - (rcWnd.bottom - rcWnd.top) / 2);

    oSys.Call("user32::SetWindowPos", hWnd, 0, X, Y, 0, 0, 0x15 /*SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE*/);

    AkelPad.MemFree(lpRect);
  }
}

function SelCompliteLine(hWnd, nMinSel, nMaxSel)
{
  var nMinLine;
  var nMaxLine;
  var nMinLineIndex;
  var nMaxLineIndex;
  var nMaxLineLength;

  if (nMinSel < nMaxSel)
  {
    nMinLine=AkelPad.SendMessage(hWnd, 1078 /*EM_EXLINEFROMCHAR*/, 0, nMinSel);
    nMaxLine=AkelPad.SendMessage(hWnd, 1078 /*EM_EXLINEFROMCHAR*/, 0, nMaxSel);
    nMinLineIndex=AkelPad.SendMessage(hWnd, 187 /*EM_LINEINDEX*/, nMinLine, 0);
    nMaxLineIndex=AkelPad.SendMessage(hWnd, 187 /*EM_LINEINDEX*/, nMaxLine, 0);
    nMaxLineLength=AkelPad.SendMessage(hWnd, 193 /*EM_LINELENGTH*/, nMaxSel, 0);

    if (nMaxLineIndex == nMaxSel) --nMaxLine;
    else if (nMaxLineLength) nMaxSel=nMaxLineIndex + nMaxLineLength + 1;
    nMinSel=nMinLineIndex;

    AkelPad.SetSel(nMinSel, nMaxSel);
    return nMaxLine - nMinLine + 1;
  }
  return 0;
}

function EscRegExp(pString)
{
  return pString.replace(/[.?*+^$[\]\\(){}|-]/g, "\\$&");
}

function LOWORD(dwNumber)
{
  return (dwNumber & 0xffff);
}

function HIWORD(dwNumber)
{
  return (dwNumber >> 16);
}

function MAKELONG(a, b)
{
  return (a & 0xffff) | ((b & 0xffff) << 16);
}

function ScaleInit(hDC, hWnd)
{
  if (!ptScale.x && !ptScale.y)
  {
    var hNewDC=hDC;

    if (!hDC) hNewDC=oSys.Call("user32::GetDC", hWnd);

    if (hNewDC)
    {
      ptScale.x=oSys.Call("gdi32::GetDeviceCaps", hNewDC, 88 /*LOGPIXELSX*/);
      ptScale.y=oSys.Call("gdi32::GetDeviceCaps", hNewDC, 90 /*LOGPIXELSY*/);

      //Align to 16 pixel
      if (ptScale.x % 16) ptScale.x+=16 - ptScale.x % 16;
      if (ptScale.y % 16) ptScale.y+=16 - ptScale.y % 16;
    }
    else return false;

    if (!hDC) oSys.Call("user32::ReleaseDC", hWnd, hNewDC);
  }
  return true;
}

function ScaleX(x)
{
  if (ptScale.x)
    return oSys.Call("kernel32::MulDiv", x, ptScale.x, 96);
  return x;
}

function ScaleY(y)
{
  if (ptScale.y)
    return oSys.Call("kernel32::MulDiv", y, ptScale.y, 96);
  return y;
}

function GetLangString(nStringID)
{
  var nLangID=AkelPad.GetLangId(1 /*LANGID_PRIMARY*/);

  if (nLangID == 0x19) //LANG_RUSSIAN
  {
    if (nStringID == 0)
      return "\u0412\u0435\u0440\u0441\u0438\u044F\u0020\u004A\u0053\u0063\u0072\u0069\u0070\u0074\u0020\u043D\u0438\u0436\u0435\u002C\u0020\u0447\u0435\u043C\u0020\u0035\u002E\u0035\u002E";
    if (nStringID == 1)
      return "\u004F\u004B";
    if (nStringID == 2)
      return "\u041E\u0442\u043C\u0435\u043D\u0430";
    if (nStringID == 3)
      return "\u0421\u0442\u0440\u043E\u043A\u0430\u0020\u0441\u043E\u0434\u0435\u0440\u0436\u0438\u0442\u003A";
    if (nStringID == 4)
      return "\u0420\u0435\u0433\u0443\u043B\u044F\u0440\u043D\u044B\u0435\u0020\u0432\u044B\u0440\u0430\u0436\u0435\u043D\u0438\u044F";
    if (nStringID == 5)
      return "\u0423\u0447\u0438\u0442\u044B\u0432\u0430\u0442\u044C\u0020\u0440\u0435\u0433\u0438\u0441\u0442\u0440";
    if (nStringID == 6)
      return "\u041E\u0441\u0442\u0430\u0432\u0438\u0442\u044C\u0020\u0441\u0442\u0440\u043E\u043A\u0438";
    if (nStringID == 7)
      return "\u0423\u0434\u0430\u043B\u0438\u0442\u044C\u0020\u0441\u0442\u0440\u043E\u043A\u0438";
    if (nStringID == 8)
      return "&\u0412\u0441\u0435\u0020\u0444\u0430\u0439\u043B\u044B";
  }
  else
  {
    if (nStringID == 0)
      return "JScript version is less than 5.5.";
    if (nStringID == 1)
      return "OK";
    if (nStringID == 2)
      return "Cancel";
    if (nStringID == 3)
      return "String contain:";
    if (nStringID == 4)
      return "Regular expressions";
    if (nStringID == 5)
      return "Case sensitive";
    if (nStringID == 6)
      return "Include lines";
    if (nStringID == 7)
      return "Exclude lines";
    if (nStringID == 8)
      return "&All files";
  }
  return "";
}
