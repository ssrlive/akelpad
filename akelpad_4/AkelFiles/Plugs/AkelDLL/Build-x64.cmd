@ECHO OFF
Set MSSDK=C:\Program Files\Microsoft Platform SDK
Set BIT=64

::###################################::
Set PATH=%MSSDK%\bin\win64\x86\AMD64;%MSSDK%\bin;%PATH%
Set INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%INCLUDE%
Set LIB=%MSSDK%\lib\AMD64;%LIB%

cl /O1 /GS- AkelDLL.c /LD /link kernel32.lib user32.lib /OPT:NOWIN98 /MACHINE:AMD64 /NODEFAULTLIB /ENTRY:DllMain

if exist AkelDLL.obj del AkelDLL.obj
if exist AkelDLL.lib del AkelDLL.lib
if exist AkelDLL.exp del AkelDLL.exp
if not "%1" == "/S" @PAUSE
