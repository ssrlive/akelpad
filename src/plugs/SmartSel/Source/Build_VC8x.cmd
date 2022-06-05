REM Visual Studio 8.0 (2005) Express

@ECHO OFF
Set VCDIR=C:\Program Files\Microsoft Visual Studio 8\VC
Set VSCOMMON=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE
Set MSSDK=C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%VSCOMMON%;%PATH%
Set INCLUDE=%MSSDK%\include;%VCDIR%\include;%INCLUDE%
Set LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%

cd .\SmartSel

rc /r /Fo"SmartSel.res" "SmartSel.rc"
cl /O1 SmartSel.c SmartSel.res /LD /link kernel32.lib user32.lib /OPT:NOWIN98 /OUT:..\..\Plugs\SmartSel.dll

if exist SmartSel.res del SmartSel.res
if exist SmartSel.lib del SmartSel.lib
if exist SmartSel.exp del SmartSel.exp
if exist SmartSel.obj del SmartSel.obj
@PAUSE
