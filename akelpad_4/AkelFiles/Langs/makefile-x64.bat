@ECHO OFF
Set MINGW=C:\MinGW-w64
Set LANGNAME=English
Set LANGID=0x409
Set BIT=64

::#######################::
Set PATH=%MINGW%\bin;%PATH%

mingw32-make.exe LANGNAME=%LANGNAME% LANGID=%LANGID% BIT=%BIT% all clean

if not "%1" == "/S" PAUSE
CLS
