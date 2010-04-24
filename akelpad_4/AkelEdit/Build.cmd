@ECHO OFF
Set MSSDK=D:\Program Files\Microsoft Platform SDK
Set VCDIR=D:\Program Files\Microsoft Visual C++ Toolkit 2003

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%VCDIR%\include;%MSSDK%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%MSSDK%\lib;%LIB%

rc /r /Fo"AkelEdit.res" "Resources\AkelEdit.rc"
cl /O1 AkelEdit.cpp AkelEdit.res /LD /link kernel32.lib user32.lib gdi32.lib ole32.lib uuid.lib imm32.lib /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:DllMain /OUT:AkelEdit.dll

if exist AkelEdit.res del AkelEdit.res
if exist AkelEdit.obj del AkelEdit.obj
if exist AkelEdit.lib del AkelEdit.lib
if exist AkelEdit.exp del AkelEdit.exp
if not "%1" == "/S" @PAUSE
