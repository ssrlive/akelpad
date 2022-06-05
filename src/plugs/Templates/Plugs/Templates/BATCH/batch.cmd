::::CARET
@SETLOCAL
@ECHO OFF 
:: < Script definition > 

:: check parameters 
IF "%1"=="" GOTO :NoParam 

:: main stuff here 
::::CARET

ENDLOCAL
@EXIT /B %ERRORLEVEL% 

:NoParam 
ECHO %~n0. Command line error: "%*" 
ENDLOCAL
@EXIT /B 1
