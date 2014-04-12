//// Show custom popup menu. Script implemented as the library for using in other scripts.
//// Отображение пользовательского меню. Скрипт реализован как библиотека для использования другими скриптами.
//
//
//// Menu example at caret position (CaretMenu.js):
//// Пример меню на позиции каретки (CaretMenu.js):
//
//  //Include
//  if (!AkelPad.Include("ShowMenu.js")) WScript.Quit();
//
//  //Variables
//  var lpItems;
//  var nItem;
//
//  lpItems=[["ItemA", MF_NORMAL, 101],
//           ["ItemB", MF_SUBMENU],
//             ["ItemB-1", MF_NORMAL, 102],
//             ["ItemB-2", MF_NORMAL, 103],
//             ["ItemB-3", MF_NORMAL|MF_LAST, 104],
//           ["ItemC", MF_NORMAL, 105],
//           ["", MF_SEPARATOR],
//           ["ItemD", MF_NORMAL|MF_GRAYED, 106],
//           ["ItemF", MF_SUBMENU],
//             ["ItemF-1", MF_NORMAL|MF_CHECKED, 107],
//             ["ItemF-2", MF_NORMAL, 108],
//             ["ItemF-3", MF_SUBMENU|MF_LAST],
//               ["ItemF-3a", MF_NORMAL, 109],
//               ["ItemF-3b", MF_NORMAL|MF_LAST, 110]];
//
//  nItem=ShowMenu(lpItems, POS_CARET, POS_CARET);
//  if (nItem == -1)
//    WScript.Echo("Nothing selected");
//  else
//    WScript.Echo("Item index: " + nItem + "\nItem name: " + lpItems[nItem][0] + "\nItem ID: " + lpItems[nItem][2]);
//
//
//// Toolbar button menu example (ToolbarItemMenu.js):
//// Пример меню для кнопки панели инструментов (ToolbarItemMenu.js):
//
//  //Arguments
//  if (WScript.Arguments.length < 2) WScript.Quit();
//  var hToolbarHandle=parseInt(WScript.Arguments(0));
//  var nToolbarItemID=parseInt(WScript.Arguments(1));
//
//  //Include
//  if (!AkelPad.Include("ShowMenu.js")) WScript.Quit();
//
//  //Variables
//  var lpItems;
//  var nItem;
//  var ptPoint=GetToolbarBottonPos(hToolbarHandle, nToolbarItemID)
//
//  lpItems=[["ItemA", MF_NORMAL, "Value1"],
//           ["", MF_SEPARATOR],
//           ["ItemB", MF_NORMAL|MF_CHECKED|MF_USECHECKBITMAPS, "Value2"]];
//
//  nItem=ShowMenu(lpItems, ptPoint.x, ptPoint.y);
//  if (nItem != -1)
//    WScript.Echo("Item index: " + nItem + "\nItem value: " + lpItems[nItem][2]);
//
//
//// Toolbar plugin item:
//// Пункт Toolbar плагина:
//  -"Button menu" Call("Scripts::Main", 1, "ToolbarItemMenu.js", `"%m" "%i"`) Icon(0)

//Menu item flags
var MF_NORMAL          =0x00000; //Normal item.
var MF_SEPARATOR       =0x00800; //Separator item.
var MF_SUBMENU         =0x00010; //SubMenu item.
var MF_LAST            =0x10000; //Last item in submenu.
var MF_DEFAULT         =0x20000; //Sets the default menu item for the specified menu.
var MF_GRAYED          =0x00001; //Grayed state.
var MF_DISABLED        =0x00002; //Disabled state.
var MF_CHECKED         =0x00008; //Checked state.
var MF_USECHECKBITMAPS =0x00200; //Show radiobutton, if combined with MF_CHECKED.
var MF_MENUBREAK       =0x00040; //Places the item in a new column.
var MF_MENUBARBREAK    =0x00020; //Same as the MF_MENUBREAK, but new column is separated from the old column by a vertical line.
var MF_HILITE          =0x00080; //Item is highlighted.

//Special X or Y positions of ShowMenu function
var POS_CARET   =-1; //Under caret position.
var POS_CURSOR  =-2; //Cursor position.


//Functions
function ShowMenu(lpItemsArray, X, Y)
{
  //Variables
  var hMainWnd=AkelPad.GetMainWnd();
  var oSys=AkelPad.SystemFunction();
  var lpMenuArray=[];
  var nMenuCount;
  var nItemCount;
  var ptPoint;
  var hWndHidden;
  var nResult=0;
  var i;

  //Menus array
  var MENU=0;
  var ITEM=1;

  //Items array
  var NAME=0;
  var FLAGS=1;

  //Menu flags
  var MF_NORMAL   =0x00000;
  var MF_SEPARATOR=0x00800;
  var MF_SUBMENU  =0x00010;
  var MF_LAST     =0x10000;
  var MF_DEFAULT  =0x20000;

  //Menu coordinates
  var POS_CARET   =-1;
  var POS_CURSOR  =-2;

  if (lpItemsArray.length > 0)
  {
    //Create menu
    nMenuCount=0;
    lpMenuArray[nMenuCount]=[oSys.Call("user32::CreatePopupMenu"), 0];

    //Fill menu
    for (nItemCount=0; nItemCount < lpItemsArray.length; ++nItemCount)
    {
      if (lpItemsArray[nItemCount][FLAGS] & MF_SEPARATOR)
      {
        oSys.Call("user32::AppendMenu" + _TCHAR, lpMenuArray[nMenuCount][MENU], 0x800 /*MF_SEPARATOR*/, nItemCount + 1, lpItemsArray[nItemCount][NAME]);
      }
      else if (lpItemsArray[nItemCount][FLAGS] & MF_SUBMENU)
      {
        lpMenuArray[nMenuCount + 1]=[oSys.Call("user32::CreatePopupMenu"), 0];
        oSys.Call("user32::AppendMenu" + _TCHAR, lpMenuArray[nMenuCount][MENU], 0x10 /*MF_POPUP*/, lpMenuArray[nMenuCount + 1][MENU], lpItemsArray[nItemCount][NAME]);
      }
      else
      {
        oSys.Call("user32::AppendMenu" + _TCHAR, lpMenuArray[nMenuCount][MENU], lpItemsArray[nItemCount][FLAGS], nItemCount + 1, lpItemsArray[nItemCount][NAME]);
        if (lpItemsArray[nItemCount][FLAGS] & MF_DEFAULT)
          oSys.Call("user32::SetMenuDefaultItem", lpMenuArray[nMenuCount][MENU], lpMenuArray[nMenuCount][ITEM], true);
      }

      if (lpItemsArray[nItemCount][FLAGS] & MF_LAST)
        lpMenuArray[nMenuCount][ITEM]=-1;
      else
        lpMenuArray[nMenuCount][ITEM]+=1;

      if (!(lpItemsArray[nItemCount][FLAGS] & MF_SUBMENU))
      {
        if (lpItemsArray[nItemCount][FLAGS] & MF_LAST)
        {
          while (--nMenuCount >= 0)
          {
            if (lpMenuArray[nMenuCount][ITEM] != -1)
              break;
          }
          if (nMenuCount < 0) break;
        }
      }
      else ++nMenuCount;
    }

    if (lpMenuArray[0][MENU])
    {
      //Create window for menu
      hWndHidden=oSys.Call("user32::CreateWindowEx" + _TCHAR, 0, "Static", 0, 0x50000000 /*WS_VISIBLE|WS_CHILD*/, 0, 0, 0, 0, hMainWnd, 0, AkelPad.GetInstanceDll(), 0);
      oSys.Call("user32::SetFocus", hWndHidden);

      //Show menu
      if (X < 0 || Y < 0)
      {
        if (X == POS_CARET || Y == POS_CARET)
          ptPoint=GetCaretPos(AkelPad.GetEditWnd());
        else if (X == POS_CURSOR || Y == POS_CURSOR)
          ptPoint=GetCursorPos();

        if (X < 0) X=ptPoint.x;
        if (Y < 0) Y=ptPoint.y;
      }
      nResult=oSys.Call("user32::TrackPopupMenu", lpMenuArray[0][MENU], 0x100|0x80|0x2 /*TPM_RETURNCMD|TPM_NONOTIFY|TPM_RIGHTBUTTON*/, X, Y, 0, hWndHidden, 0);

      //Clean up
      oSys.Call("user32::DestroyMenu", lpMenuArray[0][MENU]);
      oSys.Call("user32::DestroyWindow", hWndHidden);
    }
  }
  return nResult - 1;
}

function GetCaretPos(hWndEdit)
{
  var ptPoint=[];
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
  return ptPoint;
}

function GetCursorPos()
{
  var oSys=AkelPad.SystemFunction();
  var ptPoint=[];
  var lpPoint;

  ptPoint.x=0;
  ptPoint.y=0;

  if (lpPoint=AkelPad.MemAlloc(8 /*sizeof(POINT)*/))
  {
    //Caret position
    oSys.Call("user32::GetCursorPos", lpPoint);
    ptPoint.x=AkelPad.MemRead(lpPoint, 3 /*DT_DWORD*/);
    ptPoint.y=AkelPad.MemRead(lpPoint + 4, 3 /*DT_DWORD*/);
    AkelPad.MemFree(lpPoint);
  }
  return ptPoint;
}

function GetToolbarBottonPos(hToolbarHandle, nToolbarItemID)
{
  var ptPoint=[];
  var lpRect;

  ptPoint.x=0;
  ptPoint.y=0;

  if (hToolbarHandle && nToolbarItemID)
  {
    if (lpRect=AkelPad.MemAlloc(16 /*sizeof(RECT)*/))
    {
      //Get Toolbar button position
      AkelPad.SendMessage(hToolbarHandle, 1075 /*TB_GETRECT*/, nToolbarItemID, lpRect);
      ptPoint.x=AkelPad.MemRead(lpRect, 3 /*DT_DWORD*/);
      ptPoint.y=AkelPad.MemRead(lpRect + 12, 3 /*DT_DWORD*/);
      AkelPad.MemFree(lpRect);

      //In screen coordinates
      ClientToScreen(hToolbarHandle, ptPoint);
    }
  }
  return ptPoint;
}

function ClientToScreen(hWnd, ptPoint)
{
  var oSys=AkelPad.SystemFunction();
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
