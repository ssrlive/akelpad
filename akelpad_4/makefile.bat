@ECHO OFF
Set MINGW=C:\MinGW
Set LANGNAME=English
Set LANGID=0x409

::#######################::
Set PATH=%MINGW%\bin;%PATH%

mingw32-make.exe LANGNAME=%LANGNAME% LANGID=%LANGID% all clean

if not "%1" == "/S" PAUSE
CLS
