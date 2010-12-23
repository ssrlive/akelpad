@ECHO OFF
Set MSSDK=C:\Program Files\Microsoft Platform SDK
Set VCDIR=C:\Program Files\Microsoft Visual C++ Toolkit 2003
Set BIT=32

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%VCDIR%\include;%MSSDK%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%MSSDK%\lib;%LIB%

rc /R /DRC_VERSIONBIT=%BIT% /Fo"Redirect.res" "Resources\Redirect.rc"
cl /O1 Redirect.c Redirect.res /link kernel32.lib user32.lib advapi32.lib comdlg32.lib /SUBSYSTEM:WINDOWS /MACHINE:I386 /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:_WinMain /OUT:"notepad.exe"

if exist Redirect.obj del Redirect.obj
if exist Redirect.res del Redirect.res
@PAUSE