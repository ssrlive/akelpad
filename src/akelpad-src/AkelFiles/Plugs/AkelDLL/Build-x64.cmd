::Build.cmd /S /X32 /D
:: /S   silent mode
:: /X32 32-bit or /X64 64-bit
:: /D   debug version

@ECHO OFF
set VCDIR=c:\Program Files\Microsoft Visual C++ Toolkit 2003
set MSSDK=c:\Program Files\Microsoft Platform SDK
set BIT=64
set DEBUG=0
set CLFLAGS=/Wall /WX /wd4100 /wd4201 /wd4204 /wd4255 /wd4310 /wd4619 /wd4668 /wd4701 /wd4706 /wd4711 /wd4820 /wd4826
set LINKFLAGS=
set RCFLAGS=

::### Set paths ###::
if "%2" == "/X32" set BIT=32
if "%2" == "/X64" set BIT=64
if "%BIT%" == "32" (
  set "PATH=%MSSDK%\bin;%VCDIR%\bin;%VCDIR%\..\Common7\IDE;%VCDIR%\..\Common\MSDev98\Bin;%PATH%"
  set "INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%VCDIR%\include;%INCLUDE%"
  set "LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%"
  set MACHINE=I386
) else (
  set VCDIR=%MSSDK%
  set "PATH=%MSSDK%\bin\win64\x86\amd64;%MSSDK%\bin;%VCDIR%\bin\x86_amd64;%VCDIR%\..\Common7\IDE;%PATH%"
  set "INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%VCDIR%\include;%INCLUDE%"
  set "LIB=%MSSDK%\lib\amd64;%MSSDK%\lib\x64;%VCDIR%\lib\amd64;%LIB%"
  set MACHINE=AMD64
)

::### Compile ###::
if "%3" == "/D" set DEBUG=1
if "%DEBUG%" == "1" (
  set CLFLAGS=%CLFLAGS% /Od /Zi /DDEBUG
  set LINKFLAGS=%LINKFLAGS% /DEBUG libcmt.lib
  set RCFLAGS=%RCFLAGS% /D_DEBUG
) else (
  set CLFLAGS=%CLFLAGS% /O1
)
if "%VCDIR%" == "%VCDIR:2003=%" (
  if "%VCDIR%" == "%VCDIR:VC98=%" set CLFLAGS=%CLFLAGS% /GS-
)
cl %CLFLAGS% AkelDLL.c /LD /link kernel32.lib user32.lib %LINKFLAGS% /OPT:NOWIN98 /MACHINE:%MACHINE% /NODEFAULTLIB /ENTRY:DllMain
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%

::### Clean up ###::
if exist AkelDLL.obj del AkelDLL.obj
if exist AkelDLL.lib del AkelDLL.lib
if exist AkelDLL.exp del AkelDLL.exp
if exist AkelDLL.ilk del AkelDLL.ilk
if exist vc?0.pdb del vc?0.pdb

::### End ###::
if not "%1" == "/S" @PAUSE
if defined EXITCODE exit %2 %EXITCODE%
