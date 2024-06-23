::Build.cmd /S /X32 /D
:: /S   silent mode
:: /X32 32-bit or /X64 64-bit
:: /D   debug version

@ECHO OFF
set FLAG_S=0
set FLAG_X=/X64
set FLAG_D=0
if "%1" == "/S" set FLAG_S=/S
if "%2" == "/X32" set FLAG_X=32
if "%2" == "/X64" set FLAG_X=64
if "%3" == "/D" set FLAG_D=/D

call Build-All.cmd %FLAG_S% %FLAG_X% %FLAG_D%
