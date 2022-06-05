@ECHO OFF
Set MSSDK=D:\Program Files\Microsoft Platform SDK
Set VCDIR=D:\Program Files\Microsoft Visual C++ Toolkit 2003

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%VCDIR%\include;%MSSDK%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%MSSDK%\lib;%LIB%

cl /O1 AkelDLL.cpp /LD /link kernel32.lib user32.lib /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:DllMain

if exist AkelDLL.obj del AkelDLL.obj
if exist AkelDLL.lib del AkelDLL.lib
if exist AkelDLL.exp del AkelDLL.exp
if not "%1" == "/S" @PAUSE
