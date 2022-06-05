REM Visual Studio 7.1 (2003)

@ECHO OFF
Set VCDIR=C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7
Set MSSDK=%VCDIR%\PlatformSDK
Set ATLMFC=%VCDIR%\atlmfc

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%MSSDK%\include;%VCDIR%\include;%INCLUDE%
Set LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%

cd .\SmartSel

rc /r /Fo"SmartSel.res" "SmartSel.rc"
cl /O1 SmartSel.c SmartSel.res /LD /link kernel32.lib user32.lib /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:DllMain /OUT:..\..\Plugs\SmartSel.dll

if exist SmartSel.res del SmartSel.res
if exist SmartSel.lib del SmartSel.lib
if exist SmartSel.exp del SmartSel.exp
if exist SmartSel.obj del SmartSel.obj
@PAUSE
