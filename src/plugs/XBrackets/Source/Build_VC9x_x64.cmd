REM Visual Studio 9.0 (2008) Express

@ECHO OFF
if "%ProgramFiles(x86)%" == "" goto programfiles_ok
Set ProgramFiles=%ProgramFiles(x86)%
:programfiles_ok
Set VCDIR=%ProgramFiles%\Microsoft Visual Studio 9.0\VC
Set VSCOMMON=%ProgramFiles%\Microsoft Visual Studio 9.0\Common7\IDE
Set MSSDK=%ProgramFiles%\Microsoft SDKs\Windows\v7.0

if exist "%MSSDK%" goto sdk_ok
Set MSSDK=%VCDIR%\PlatformSDK
:sdk_ok

set CLFLAGS=/Wall /wd4100 /wd4201 /wd4204 /wd4255 /wd4310 /wd4619 /wd4668 /wd4701 /wd4706 /wd4711 /wd4820 /wd4826
set BIT=64

::###################################::
set "PATH=%MSSDK%\bin\win64\x86\amd64;%MSSDK%\bin;%VCDIR%\bin\x86_amd64;%VSCOMMON%;%VCDIR%\bin;%PATH%"
set "INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%VCDIR%\include;%INCLUDE%"
set "LIB=%MSSDK%\lib\amd64;%MSSDK%\lib\x64;%VCDIR%\lib\amd64;%LIB%"
set MACHINE=AMD64

cd .\XBrackets

rc /R /DRC_VERSIONBIT=%BIT% /Fo"XBrackets.res" "XBrackets.rc"
cl /O1 %CLFLAGS% /D "AKELPAD_X64" AutoBrackets.c Plugin.c AnyRichEdit.c AnyWindow.c SettingsDlg.c XBracketsLng.c XBrackets.res /LD /link kernel32.lib user32.lib gdi32.lib Comdlg32.lib /OPT:NOWIN98 /MACHINE:%MACHINE% /ENTRY:DllMain /OUT:..\..\Plugs64\XBrackets.dll

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
