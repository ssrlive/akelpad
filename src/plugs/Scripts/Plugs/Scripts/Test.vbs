' http://akelpad.sourceforge.net/en/plugins.php#Scripts
' Version: 1.0
' Author: Shengalts Aleksander aka Instructor
'
' Description(1033): VBScript test file.
' Description(1049): Тестовый файл VBScript.

'Defines
Const WM_COMMAND=273
Const WM_USER=1024
Const OD_ADT_BINARY_ERROR=&H1
Const OD_ADT_REG_CODEPAGE=&H2
Const OD_ADT_DETECT_CODEPAGE=&H4
Const OD_ADT_DETECT_BOM=&H8

'See AkelDLL.h
IDM_FILE_NEW=4101
AKD_PASTE=(WM_USER + 170)

'Get main window handle
hMainWnd=AkelPad.GetMainWnd
WScript.Echo "MainHandle=" & hMainWnd

'Get current file
pFile=AkelPad.GetEditFile(0)
WScript.Echo pFile

'Send message
AkelPad.SendMessage hMainWnd, WM_COMMAND, IDM_FILE_NEW, 0
WScript.Echo "New window has been created"

'Get edit window handle
hEditWnd=AkelPad.GetEditWnd
WScript.Echo "EditHandle=" & hEditWnd

'Read contents of a file
'Syntax: ReadFile("File", flags, code page, BOM)
pText=AkelPad.ReadFile(pFile, OD_ADT_BINARY_ERROR Or OD_ADT_REG_CODEPAGE, 0, 0)
pTextPart=Left(pText, 100)
WScript.Echo pTextPart

'Set clipboard text
AkelPad.SetClipboardText pTextPart
WScript.Echo "Clipboard updated"

'Send message
nResult=AkelPad.SendMessage(hMainWnd, AKD_PASTE, hEditWnd, false)
WScript.Echo "Text has been pasted from clipboard"

'Set selection
AkelPad.SetSel 0, -1
WScript.Echo "All text selected"

'Get current selection
nSelStart=AkelPad.GetSelStart
nSelEnd=AkelPad.GetSelEnd
WScript.Echo "nSelStart=" & nSelStart & vbCRLF & "nSelEnd=" & nSelEnd

'Get selected text
pText=AkelPad.GetSelText
WScript.Echo pText

'Replace selection
AkelPad.ReplaceSel "[" & pText & "]"
WScript.Echo "Brackets added"

'Call plugin
nResult=AkelPad.Call("Format::LineSortStrAsc")
WScript.Echo "Plugin has been called (" & nResult & ")"
