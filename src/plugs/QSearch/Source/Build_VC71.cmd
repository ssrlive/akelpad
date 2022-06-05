REM Visual Studio 7.1 (2003)

@ECHO OFF
Set VCDIR=%ProgramFiles%\Microsoft Visual Studio .NET 2003\Vc7
Set MSSDK=%VCDIR%\PlatformSDK
Set ATLMFC=%VCDIR%\atlmfc

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%MSSDK%\include;%VCDIR%\include;%ATLMFC%\include;%INCLUDE%
Set LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%

cd .\QSearch

rc /r /Fo"QSearch.res" "QSearch.rc"
cl /O1 QSearch.c QSearchDlg.c QSearchLng.c QSearchFindEx.c DialogSwitcher.c QSearch.res /LD /link kernel32.lib user32.lib comctl32.lib gdi32.lib Advapi32.lib /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:DllMain /OUT:..\..\Plugs\QSearch.dll

if exist QSearch.res del QSearch.res
if exist QSearch.lib del QSearch.lib
if exist QSearch.exp del QSearch.exp
if exist QSearch.obj del QSearch.obj
if exist QSearchDlg.obj del QSearchDlg.obj
if exist QSearchLng.obj del QSearchLng.obj
if exist QSearchFindEx.obj del QSearchFindEx.obj
if exist DialogSwitcher.obj del DialogSwitcher.obj
@PAUSE
