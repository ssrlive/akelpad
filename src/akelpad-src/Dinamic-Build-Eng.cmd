@ECHO OFF
set VCDIR=c:\Program Files\Microsoft Visual C++ Toolkit 2003
set MSSDK=c:\Program Files\Microsoft Platform SDK
set CLFLAGS=/Wall /WX /wd4100 /wd4201 /wd4204 /wd4255 /wd4310 /wd4619 /wd4668 /wd4701 /wd4706 /wd4711 /wd4820 /wd4826
set LANGNAME=English
set LANGID=0x409
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
rc /R /DRC_EXEVERSION /DRC_VERSIONLANGID=%LANGID% /DRC_VERSIONBIT=%BIT% /Fo"AkelPad.res" "AkelFiles\Langs\Resources\%LANGNAME%.rc"
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%
cl /O1 %CLFLAGS% /DRC_VERSIONLANGID=%LANGID% AkelPad.c Edit.c AkelPad.res /link kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib comctl32.lib comdlg32.lib oleaut32.lib uuid.lib version.lib /SUBSYSTEM:WINDOWS /OPT:NOWIN98 /MACHINE:%MACHINE% /NODEFAULTLIB /ENTRY:_WinMain /OUT:AkelPad.exe
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%

::### Clean up ###::
if exist AkelPad.obj del AkelPad.obj
if exist Edit.obj del Edit.obj
if exist AkelPad.res del AkelPad.res

::### End ###::
if not "%1" == "/S" @PAUSE
if defined EXITCODE exit %2 %EXITCODE%
