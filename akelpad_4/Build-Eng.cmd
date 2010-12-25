@ECHO OFF
set LANGNAME=English
set LANGID=0x409
set BIT=32
set VCTYPE=/TOOLKIT
set VCROOT=%ProgramFiles%

::### Find compliler ###::
if not "%1" == "" set VCTYPE=%1
if "%VCTYPE%" == "/TOOLKIT" (
  if "%BIT%" == "32" (
    set VCDIR=\Microsoft Visual C++ Toolkit 2003
    set MSSDK=\Microsoft Platform SDK
  ) else (
    set VCDIR=\Microsoft Platform SDK
    set MSSDK=\Microsoft Platform SDK
    set CLFLAGS=/GS-
  )
) else if "%VCTYPE%" == "/VS10" (
  set VCDIR=\Microsoft Visual Studio 10.0\VC
  set MSSDK=\Microsoft Visual Studio 10.0\VC\PlatformSDK
  set CLFLAGS=/GS-
) else if "%VCTYPE%" == "/VS9" (
  set VCDIR=\Microsoft Visual Studio 9.0\VC
  set MSSDK=\Microsoft Visual Studio 9.0\VC\PlatformSDK
  set CLFLAGS=/GS-
) else if "%VCTYPE%" == "/VS8" (
  set VCDIR=\Microsoft Visual Studio 8\VC
  set MSSDK=\Microsoft Visual Studio 8\VC\PlatformSDK
  set CLFLAGS=/GS-
) else if "%VCTYPE%" == "/VS7" (
  set VCDIR=\Microsoft Visual Studio.Net 2003\VC7
  set MSSDK=\Microsoft Visual Studio.Net 2003\VC7\PlatformSDK
) else if "%VCTYPE%" == "/VS6" (
  set VCDIR=\Microsoft Visual Studio\VC98
  set MSSDK=\Microsoft Visual Studio\VC98
) else (
  echo "Valid types: /TOOLKIT /VS6 /VS7 /VS8 /VS9 /VS10"
  goto END
)
if exist "%VCROOT%%VCDIR%\*.*" (
  set "VCDIR=%VCROOT%%VCDIR%"
  set "MSSDK=%VCROOT%%MSSDK%"
) else if exist "c:\Program Files%VCDIR%\*.*" (
  set "VCDIR=c:\Program Files%VCDIR%"
  set "MSSDK=c:\Program Files%MSSDK%"
) else if exist "c:\Program Files (x86)%VCDIR%\*.*" (
  set "VCDIR=c:\Program Files (x86)%VCDIR%"
  set "MSSDK=c:\Program Files (x86)%MSSDK%"
) else (
  echo Error: compiler not found.
  goto END
)

::### Set paths ###::
set OLDPATH=%PATH%
set OLDINCLUDE=%INCLUDE%
set OLDLIB=%LIB%
if "%BIT%" == "32" (
  set "PATH=%MSSDK%\bin;%VCDIR%\bin;%VCDIR%\..\Common7\IDE;%VCDIR%\..\Common\MSDev98\Bin;%PATH%"
  set "INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%VCDIR%\include;%INCLUDE%"
  set "LIB=%MSSDK%\lib;%VCDIR%\lib;%LIB%"
  set MACHINE=I386
) else (
  set "PATH=%MSSDK%\bin\win64\x86\amd64;%MSSDK%\bin;%VCDIR%\bin\x86_amd64;%VCDIR%\..\Common7\IDE;%PATH%"
  set "INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%VCDIR%\include;%INCLUDE%"
  set "LIB=%MSSDK%\lib\amd64;%VCDIR%\lib\amd64;%LIB%"
  set MACHINE=AMD64
)

::### Compile ###::
rc /R /DAKELEDIT_STATICBUILD /DEXE_VERSION /DRC_VERSIONLANGID=%LANGID% /DRC_VERSIONBIT=%BIT% /I "AkelEdit\Resources" /Fo"AkelPad.res" "AkelFiles\Langs\Resources\%LANGNAME%.rc"
cl /O1 %CLFLAGS% /DAKELEDIT_STATICBUILD /DRC_VERSIONLANGID=%LANGID% AkelPad.c Edit.c AkelEdit\AkelEdit.c AkelPad.res /link kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib comctl32.lib comdlg32.lib ole32.lib uuid.lib imm32.lib version.lib /SUBSYSTEM:WINDOWS /OPT:NOWIN98 /MACHINE:%MACHINE% /NODEFAULTLIB /ENTRY:_WinMain /OUT:AkelPad.exe

::### Clean up ###::
if exist AkelPad.obj del AkelPad.obj
if exist Edit.obj del Edit.obj
if exist AkelEdit.obj del AkelEdit.obj
if exist AkelPad.res del AkelPad.res
set CLFLAGS=
set PATH=%OLDPATH%
set INCLUDE=%OLDINCLUDE%
set LIB=%OLDLIB%
@PAUSE

:END
