REM Visual Studio 9.0 (2008) Express

@ECHO OFF
Set ProgramFiles0=%ProgramFiles%
if "%ProgramFiles(x86)%" == "" goto programfiles_ok
Set ProgramFiles=%ProgramFiles(x86)%
:programfiles_ok
Set VCDIR=%ProgramFiles%\Microsoft Visual Studio 9.0\VC
Set VSCOMMON=%ProgramFiles%\Microsoft Visual Studio 9.0\Common7\IDE
Set MSSDK=%ProgramFiles0%\Microsoft SDKs\Windows\v6.0A

REM if exist "%MSSDK%" goto sdk_ok
REM Set MSSDK=%VCDIR%\PlatformSDK
REM :sdk_ok

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%VSCOMMON%;%PATH%
Set INCLUDE=%MSSDK%\include;%VCDIR%\include;%INCLUDE%
Set LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%

cd .\QSearch

rc /r /Fo"QSearch.res" "QSearch.rc"
cl /O1 QSearch.c QSearchDlg.c QSearchLng.c QSearchFindEx.c DialogSwitcher.c QSearch.res /LD /link kernel32.lib user32.lib comctl32.lib gdi32.lib Advapi32.lib /OPT:NOWIN98 /OUT:..\..\Plugs\QSearch.dll

if exist QSearch.res del QSearch.res
if exist QSearch.lib del QSearch.lib
if exist QSearch.exp del QSearch.exp
if exist QSearch.obj del QSearch.obj
if exist QSearchDlg.obj del QSearchDlg.obj
if exist QSearchLng.obj del QSearchLng.obj
if exist QSearchFindEx.obj del QSearchFindEx.obj
if exist DialogSwitcher.obj del DialogSwitcher.obj
@PAUSE
