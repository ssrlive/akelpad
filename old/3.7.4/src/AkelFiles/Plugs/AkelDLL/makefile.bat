@ECHO OFF
Set MINGW=C:\MinGW

::#######################::
Set PATH=%MINGW%\bin;%PATH%

mingw32-make.exe all clean

if not "%1" == "/S" PAUSE
CLS
