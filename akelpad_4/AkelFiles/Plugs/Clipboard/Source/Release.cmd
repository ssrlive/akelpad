@ECHO OFF
set PLUGINNAME=Clipboard

::### x86 ###::
call Build.cmd /S /B
if not %ERRORLEVEL% == 0 goto ERROR
move "%PLUGINNAME%.dll" "..\Plugs"
if not %ERRORLEVEL% == 0 goto ERROR

::### x64 ###::
call Build-x64.cmd /S /B
if not %ERRORLEVEL% == 0 goto ERROR
move "%PLUGINNAME%.dll" "..\Plugs64"
if not %ERRORLEVEL% == 0 goto ERROR

::### Create archive ###::
for %%X in (7za.exe) do (set 7ZPATH=%%~$PATH:X)
if not defined 7ZPATH echo ERROR: 7za.exe not found
if not defined 7ZPATH set EXITCODE=1
if not defined 7ZPATH goto END
cd "%~dp0.."
if exist "%PLUGINNAME%.zip" del "%PLUGINNAME%.zip"
"7za.exe" a -tzip -r "%PLUGINNAME%.zip" "Docs\*" "Plugs\*" "Plugs64\*" "Source\*"
cd "%~dp0"
goto END

:ERROR
set EXITCODE=%ERRORLEVEL%

:END
if not "%1" == "/S" @PAUSE
if defined EXITCODE exit %2 %EXITCODE%
