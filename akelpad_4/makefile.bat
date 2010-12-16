@ECHO OFF
Set MINGW=C:\MinGW
Set LANGNAME=English
Set LANGID=0x409
Set BIT=32

::#######################::
Set PATH=%MINGW%\bin;%PATH%

mingw32-make.exe LANGNAME=%LANGNAME% LANGID=%LANGID% BIT=%BIT% all clean

if not "%1" == "/S" PAUSE
CLS
