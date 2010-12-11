@ECHO OFF
Set MSSDK=D:\Program Files\Microsoft Platform SDK
Set LANGNAME=English
Set LANGID=0x409

::###################################::
Set PATH=%MSSDK%\bin\win64\x86\AMD64;%MSSDK%\bin;%PATH%
Set INCLUDE=%MSSDK%\include;%MSSDK%\include\crt;%INCLUDE%
Set LIB=%MSSDK%\lib\AMD64;%LIB%

rc /R /DAKELEDIT_STATICBUILD /DEXE_VERSION /DRC_VERSIONLANGID=%LANGID% /I "AkelEdit\Resources" /Fo"AkelPad.res" "AkelFiles\Langs\Resources\%LANGNAME%.rc"
cl /O1 /GS- /DAKELEDIT_STATICBUILD /DRC_VERSIONLANGID=%LANGID% AkelPad.c Edit.c AkelEdit\AkelEdit.c AkelPad.res /link kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib comctl32.lib comdlg32.lib ole32.lib uuid.lib imm32.lib version.lib /SUBSYSTEM:WINDOWS /OPT:NOWIN98 /MACHINE:AMD64 /NODEFAULTLIB /ENTRY:_WinMain /OUT:AkelPad.exe

if exist AkelPad.obj del AkelPad.obj
if exist Edit.obj del Edit.obj
if exist AkelEdit.obj del AkelEdit.obj
if exist AkelPad.res del AkelPad.res
@PAUSE
