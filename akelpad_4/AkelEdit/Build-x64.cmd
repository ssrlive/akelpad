@ECHO OFF
Set MSSDK=C:\Program Files\Microsoft Platform SDK
Set BIT=64

::###################################::
Set PATH=%MSSDK%\bin\win64\x86\AMD64;%MSSDK%\bin;%PATH%
Set INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%INCLUDE%
Set LIB=%MSSDK%\lib\AMD64;%LIB%

rc /R /DRC_AKELEDITBIT=%BIT% /Fo"AkelEdit.res" "Resources\AkelEdit.rc"
cl /O1 /GS- AkelEdit.c AkelEdit.res /LD /link kernel32.lib user32.lib gdi32.lib ole32.lib uuid.lib imm32.lib /OPT:NOWIN98 /MACHINE:AMD64 /NODEFAULTLIB /ENTRY:DllMain /OUT:AkelEdit.dll

if exist AkelEdit.res del AkelEdit.res
if exist AkelEdit.obj del AkelEdit.obj
if exist AkelEdit.lib del AkelEdit.lib
if exist AkelEdit.exp del AkelEdit.exp
if not "%1" == "/S" @PAUSE
