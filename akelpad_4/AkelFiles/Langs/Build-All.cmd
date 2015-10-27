@ECHO OFF
set VCDIR=c:\Program Files\Microsoft Visual C++ Toolkit 2003
set MSSDK=c:\Program Files\Microsoft Platform SDK
set BIT=32

::### Set paths ###::
if "%BIT%" == "32" (
  set "PATH=%MSSDK%\bin;%VCDIR%\bin;%VCDIR%\..\Common7\IDE;%VCDIR%\..\Common\MSDev98\Bin;%PATH%"
  set "INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%VCDIR%\include;%INCLUDE%"
  set "LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%"
  set MACHINE=I386
) else (
  set "PATH=%MSSDK%\bin\win64\x86\amd64;%MSSDK%\bin;%VCDIR%\bin\x86_amd64;%VCDIR%\..\Common7\IDE;%PATH%"
  set "INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%VCDIR%\include;%INCLUDE%"
  set "LIB=%MSSDK%\lib\amd64;%MSSDK%\lib\x64;%VCDIR%\lib\amd64;%LIB%"
  set MACHINE=AMD64
)

::### Compile ###::
call :COMPILE "Belorussian" 0x423
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Brazilian" 0x416
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Catalan" 0x403
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Chinese (Simplified)" 0x804
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Chinese (Traditional)" 0x404
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Czech" 0x405
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Dutch" 0x413
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "English" 0x409
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Finnish" 0x40B
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "French" 0x40C
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "German" 0x407
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Greek" 0x408
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Hungarian" 0x40E
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Italian" 0x410
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Japanese" 0x411
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Korean" 0x412
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Polish" 0x415
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Romanian" 0x418
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Russian" 0x419
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Slovak" 0x41B
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Spanish" 0x40A
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Ukrainian" 0x422
if not %ERRORLEVEL% == 0 goto END

call :COMPILE "Uzbek" 0x443
if not %ERRORLEVEL% == 0 goto END

:END
if not "%1" == "/S" @PAUSE
goto :EOF

:COMPILE
set LANGNAME=%~1
set LANGID=%2
rc /R /DDLL_VERSION /DRC_VERSIONLANGID=%LANGID% /DRC_VERSIONBIT=%BIT% /I "..\..\AkelEdit\Resources" /Fo"%LANGNAME%.res" "Resources\%LANGNAME%.rc"
if not %ERRORLEVEL% == 0 echo %LANGNAME%.rc FAILED!
if not %ERRORLEVEL% == 0 goto :EOF
cl /O1 Module.c /LD /link "%LANGNAME%.res" /OPT:NOWIN98 /MACHINE:%MACHINE% /NODEFAULTLIB /ENTRY:DllMain /OUT:"%LANGNAME%.dll"

::### Clean up ###::
if exist Module.obj del Module.obj
if exist "%LANGNAME%.res" del "%LANGNAME%.res"
