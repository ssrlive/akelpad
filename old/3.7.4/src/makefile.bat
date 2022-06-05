@ECHO OFF
Set MINGW=C:\MinGW
Set LANG=English

::#######################::
Set PATH=%MINGW%\bin;%PATH%

mingw32-make.exe LANG=%LANG% all clean

if not "%1" == "/S" PAUSE
CLS
