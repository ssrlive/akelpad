REM Visual Studio 6 (1998)

@ECHO OFF
Set ProgramFiles=C:\Program Files
Set VCDIR=%ProgramFiles%\Microsoft Visual Studio\VC98
Set VSCOMMON=%ProgramFiles%\Microsoft Visual Studio\Common\MSDev98

::###################################::
Set PATH=%VCDIR%\Bin;%VSCOMMON%\Bin;%PATH%
Set INCLUDE=%MSSDK%\include;%VCDIR%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%LIB%

cd .\XBrackets

rc /r /Fo"XBrackets.res" "XBrackets.rc"
cl /O1 AutoBrackets.c Plugin.c AnyRichEdit.c AnyWindow.c SettingsDlg.c XBracketsLng.c XBrackets.res /LD /link kernel32.lib user32.lib gdi32.lib Comdlg32.lib /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:DllMain /OUT:..\..\Plugs\XBrackets.dll

if exist XBrackets.res del XBrackets.res
if exist AutoBrackets.lib del AutoBrackets.lib
if exist AutoBrackets.exp del AutoBrackets.exp
if exist AutoBrackets.obj del AutoBrackets.obj
if exist AnyRichEdit.obj del AnyRichEdit.obj
if exist AnyWindow.obj del AnyWindow.obj
if exist HexStr.obj del HexStr.obj
if exist Plugin.obj del Plugin.obj
if exist SettingsDlg.obj del SettingsDlg.obj
if exist XBracketsLng.obj del XBracketsLng.obj
@PAUSE
