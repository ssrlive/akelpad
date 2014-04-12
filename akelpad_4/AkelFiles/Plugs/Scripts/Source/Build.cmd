@ECHO OFF
set VCDIR=c:\Program Files\Microsoft Visual C++ Toolkit 2003
set MSSDK=c:\Program Files\Microsoft Platform SDK
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
midl /win32 /mktyplib203 Scripts.idl
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%
rc /R /DRC_VERSIONBIT=%BIT% /Fo"Scripts.res" "Resources\Scripts.rc"
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%
cl /O1 %CLFLAGS% Scripts.c IServer.c IWScript.c IWArguments.c IDocument.c IScriptSettings.c ISystemFunction.c IConstants.c IActiveScriptSite.c Scripts.res Scripts.def /LD /link kernel32.lib user32.lib gdi32.lib advapi32.lib oleaut32.lib ole32.lib uuid.lib /OPT:NOWIN98 /MACHINE:%MACHINE% /NODEFAULTLIB /ENTRY:DllMain /OUT:"Scripts.dll"
if not %ERRORLEVEL% == 0 set EXITCODE=%ERRORLEVEL%

::### Clean up ###::
if exist Scripts.tlb del Scripts.tlb
if exist Scripts.res del Scripts.res
if exist Scripts.obj del Scripts.obj
if exist Scripts.lib del Scripts.lib
if exist Scripts.exp del Scripts.exp
if exist IServer.obj del IServer.obj
if exist IWScript.obj del IWScript.obj
if exist IWArguments.obj del IWArguments.obj
if exist IDocument.obj del IDocument.obj
if exist IScriptSettings.obj del IScriptSettings.obj
if exist ISystemFunction.obj del ISystemFunction.obj
if exist IConstants.obj del IConstants.obj
if exist IActiveScriptSite.obj del IActiveScriptSite.obj

::### End ###::
if not "%1" == "/S" @PAUSE
if defined EXITCODE exit %2 %EXITCODE%
