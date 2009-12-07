@ECHO OFF
Set MSSDK=D:\Program Files\Microsoft Platform SDK
Set VCDIR=D:\Program Files\Microsoft Visual C++ Toolkit 2003
Set LANG=Italian

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%VCDIR%\include;%MSSDK%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%MSSDK%\lib;%LIB%

rc /R /DDLL_VERSION /Fo"%LANG%.res" "Resources\%LANG%.rc"
cl /O1 Module.cpp /LD /link "%LANG%.res" /OPT:NOWIN98 /MACHINE:I386 /NODEFAULTLIB /ENTRY:DllMain /OUT:"%LANG%.dll"

if exist Module.obj del Module.obj
if exist "%LANG%.res" del "%LANG%.res"
@PAUSE
