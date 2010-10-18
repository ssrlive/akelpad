@ECHO OFF
Set MSSDK=D:\Program Files\Microsoft Platform SDK
Set VCDIR=D:\Program Files\Microsoft Visual C++ Toolkit 2003

::###################################::
Set PATH=%VCDIR%\bin;%MSSDK%\bin;%PATH%
Set INCLUDE=%VCDIR%\include;%MSSDK%\include;%INCLUDE%
Set LIB=%VCDIR%\lib;%MSSDK%\lib;%LIB%

rc /r /Fo"AkelUpdater.res" "Resources\AkelUpdater.rc"
cl /O1 AkelUpdater.c AkelUpdater.res  /LD /link kernel32.lib user32.lib /OPT:NOWIN98 /NODEFAULTLIB /ENTRY:DllMain

if exist AkelUpdater.res del AkelUpdater.res
if exist AkelUpdater.obj del AkelUpdater.obj
if exist AkelUpdater.lib del AkelUpdater.lib
if exist AkelUpdater.exp del AkelUpdater.exp
@PAUSE