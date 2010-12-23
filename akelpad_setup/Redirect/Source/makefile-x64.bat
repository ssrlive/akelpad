@ECHO OFF
Set MINGW=C:\MinGW-w64
Set BIT=64

::#######################::
Set PATH=%MINGW%\bin;%PATH%

mingw32-make.exe BIT=%BIT% all clean

if not "%1" == "/S" PAUSE
CLS
