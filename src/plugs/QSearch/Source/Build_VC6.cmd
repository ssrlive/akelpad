REM Visual Studio 6 (1998)

@ECHO OFF
Set ProgramFiles=C:\Program Files
Set VCDIR=%ProgramFiles%\Microsoft Visual Studio\VC98
Set VSCOMMON=%ProgramFiles%\Microsoft Visual Studio\Common\MSDev98

::###################################::
Set PATH=%VCDIR%\Bin;%VSCOMMON%\Bin;%PATH%
Set INCLUDE=%MSSDK%\include;%VCDIR%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%LIB%

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
