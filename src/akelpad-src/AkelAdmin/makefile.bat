@ECHO OFF
Set MINGW=C:\MinGW
Set BIT=32

::### Set paths ###::
Set PATH=%MINGW%\bin;%PATH%

::### Compile ###::
mingw32-make.exe BIT=%BIT% all clean

::### End ###::
if not "%1" == "/S" @PAUSE
if not %ERRORLEVEL% == 0 exit %2 %ERRORLEVEL%
