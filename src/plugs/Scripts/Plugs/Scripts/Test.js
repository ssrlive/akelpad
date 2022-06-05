// http://akelpad.sourceforge.net/en/plugins.php#Scripts
// Version: 1.0
// Author: Shengalts Aleksander aka Instructor
//
// Description(1033): JScript test file.
// Description(1049): Тестовый файл JScript.

//Defines
var WM_COMMAND=273
var WM_USER=1024
var OD_ADT_BINARY_ERROR=0x1
var OD_ADT_REG_CODEPAGE=0x2
var OD_ADT_DETECT_CODEPAGE=0x4
var OD_ADT_DETECT_BOM=0x8

//See AkelDLL.h
var IDM_FILE_NEW=4101
var AKD_PASTE=(WM_USER + 170)

//Get main window handle
var hMainWnd=AkelPad.GetMainWnd();
WScript.Echo("Handle=" + hMainWnd);

//Get current file
var pFile=AkelPad.GetEditFile(0);
WScript.Echo(pFile);

//Send message
AkelPad.SendMessage(hMainWnd, WM_COMMAND, IDM_FILE_NEW, 0);
WScript.Echo("New window has been created");

//Get edit window handle
var hEditWnd=AkelPad.GetEditWnd();
WScript.Echo("EditHandle=" + hEditWnd);

//Read contents of a file
//Syntax: ReadFile("File", flags, code page, BOM)
var pText=AkelPad.ReadFile(pFile, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, 0);
var pTextPart=pText.substr(0, 100);
WScript.Echo(pTextPart);

//Set clipboard text
AkelPad.SetClipboardText(pTextPart);
WScript.Echo("Clipboard updated");

//Send message
var nResult=AkelPad.SendMessage(hMainWnd, AKD_PASTE, hEditWnd, false);
WScript.Echo("Text has been pasted from clipboard");

//Set selection
AkelPad.SetSel(0, -1);
WScript.Echo("All text selected");

//Get current selection
var nSelStart=AkelPad.GetSelStart();
var nSelEnd=AkelPad.GetSelEnd();
WScript.Echo("nSelStart=" + nSelStart + "\r\n" + "nSelEnd=" + nSelEnd);

//Get selected text
var pText=AkelPad.GetSelText();
WScript.Echo(pText);

//Replace selection
AkelPad.ReplaceSel("[" + pText + "]");
WScript.Echo("Brackets added");

//Call plugin
var nResult=AkelPad.Call("Format::LineSortStrAsc");
WScript.Echo("Plugin has been called (" + nResult + ")");
