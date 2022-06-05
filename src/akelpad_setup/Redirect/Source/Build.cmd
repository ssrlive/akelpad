@ECHO OFF
set VCDIR=C:\Program Files\Microsoft Visual C++ Toolkit 2003
set MSSDK=C:\Program Files\Microsoft Platform SDK
set CLFLAGS=/Wall /WX /wd4100 /wd4201 /wd4204 /wd4255 /wd4310 /wd4619 /wd4668 /wd4701 /wd4706 /wd4711 /wd4820 /wd4826
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
if "%VCDIR%" == "%VCDIR:2003=%" (
  if "%VCDIR%" == "%VCDIR:VC98=%" set CLFLAGS=%CLFLAGS% /GS-
)
rc /R /DRC_VERSIONBIT=%BIT% /Fo"Redirect.res" "Resources\Redirect.rc"
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%
cl /O1 %CLFLAGS% Redirect.c Redirect.res /link kernel32.lib user32.lib advapi32.lib comdlg32.lib /SUBSYSTEM:WINDOWS /OPT:NOWIN98 /MACHINE:%MACHINE% /NODEFAULTLIB /ENTRY:_WinMain /OUT:"notepad.exe"
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%

::### Clean up ###::
if exist Redirect.res del Redirect.res
if exist Redirect.obj del Redirect.obj

::### End ###::
if not "%1" == "/S" @PAUSE
if defined EXITCODE exit %2 %EXITCODE%
