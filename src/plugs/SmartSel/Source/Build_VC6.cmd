REM Visual Studio 6 (1998)

@ECHO OFF
Set VCDIR=C:\Program Files\Microsoft Visual Studio\VC98
Set VSCOMMON=C:\Program Files\Microsoft Visual Studio\Common\MSDev98

::###################################::
Set PATH=%VCDIR%\bin;%VSCOMMON%\Bin;%PATH%
Set INCLUDE=%VCDIR%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%LIB%

cd .\SmartSel

rc /r /Fo"SmartSel.res" "SmartSel.rc"
cl /O1 SmartSel.c SmartSel.res /LD /link kernel32.lib user32.lib /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:DllMain /OUT:..\..\Plugs\SmartSel.dll

if exist SmartSel.res del SmartSel.res
if exist SmartSel.lib del SmartSel.lib
if exist SmartSel.exp del SmartSel.exp
if exist SmartSel.obj del SmartSel.obj
@PAUSE
