@ECHO OFF
Set MSSDK=D:\Program Files\Microsoft Platform SDK
Set VCDIR=D:\Program Files\Microsoft Visual C++ Toolkit 2003


::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%MSSDK%\Include;%VCDIR%\include;%INCLUDE%
Set LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%

rc /r /Fo"Redirect.res" "Resources\Redirect.rc"
cl /O1 Redirect.cpp /link kernel32.lib user32.lib advapi32.lib comdlg32.lib Redirect.res /SUBSYSTEM:WINDOWS /MACHINE:I386 /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:_WinMain /OUT:"notepad.exe"

if exist Redirect.obj del Redirect.obj
if exist Redirect.res del Redirect.res
@PAUSE