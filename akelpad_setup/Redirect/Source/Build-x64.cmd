@ECHO OFF
Set MSSDK=C:\Program Files\Microsoft Platform SDK
Set BIT=64

::###################################::
Set PATH=%MSSDK%\bin\win64\x86\AMD64;%MSSDK%\bin;%PATH%
Set INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%INCLUDE%
Set LIB=%MSSDK%\lib\AMD64;%LIB%

rc /R /DRC_VERSIONBIT=%BIT% /Fo"Redirect.res" "Resources\Redirect.rc"
cl /O1 /GS- Redirect.c Redirect.res /link kernel32.lib user32.lib advapi32.lib comdlg32.lib /SUBSYSTEM:WINDOWS /MACHINE:AMD64 /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:_WinMain /OUT:"notepad-x64.exe"

if exist Redirect.obj del Redirect.obj
if exist Redirect.res del Redirect.res
@PAUSE