@ECHO OFF
ECHO /**
ECHO  *		Speech for AkelPad
ECHO  *		Panych Y.W. aka FeyFre (c) 2010 (panych.y@gmail.com)
ECHO  *
ECHO  *		This piece of software is distributed under BSD license.
ECHO  *		AkelPad is property of its copyright holders.
ECHO  */

REM Specify real location of build tools
CALL "C:\Program Files\Microsoft Visual Studio\VC98\bin\vcvars32.bat"
SET SRCDIR=.
SET OUTDIR=.\output
SET INTDIR=.\output\obj

SET CSOURCE=%SRCDIR%\*.c
SET RCSOURCE=%SRCDIR%\Speech.rc
SET LINKSOURCE=%INTDIR%\*.res %INTDIR%\*.obj

SET PSDK=C:\PSDK
REM In new versions of WindowsSDK SAPI already included(at least in 6.0a) so you probably should not install this path
REM Get SAPI 5.1 http://www.microsoft.com/downloads/details.aspx?FamilyID=5e86ec97-40a7-453f-b0ee-6583171b4530&displaylang=en
REM Item: SpeechSDK51.exe
SET SAPI=C:\Program Files\Microsoft Speech SDK 5.1
SET INCLUDE=%SAPI%\INCLUDE;%PSDK%\INCLUDE;%INCLUDE%
SET LIB=%SAPI%\lib\i386;%PSDK%\LIB;%LIB%

SET LIBS=kernel32.lib user32.lib ole32.lib sapi.lib comdlg32.lib uuid.lib
SET CFLAGS=/c /GR- /GX- /Gi- /Gm- /W3 /O1 /Os /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "UNICODE" /Fo%INTDIR%\ /Fd%INTDIR%
SET RCFLAGS=/l 0x422 /fo%INTDIR%\Speech.res /d "NDEBUG" 
SET LFLAGS=/DLL /ENTRY:DllMain /RELEASE /MAP /MAPINFO:LINES /NODEFAULTLIB /OPT:NOWIN98 /OPT:REF /OPT:ICF /INCREMENTAL:NO /MACHINE:I386 /PDB:%OUTDIR%\Speech.pdb /OUT:%OUTDIR%\Speech.dll


MKDIR %OUTDIR%
MKDIR %INTDIR%
DEL /F /Q %INTDIR%\*.*
DEL /F /Q %OUTDIR%\Speech.dll

cl /nologo %CFLAGS% %CSOURCE%
rc %RCFLAGS% %RCSOURCE%
link /nologo %LFLAGS% %LIBS% %LINKSOURCE%

SET SRCDIR=
SET OUTDIR=
SET INTDIR=
SET PSDK=
SET SAPI=
SET CSOURCE=
SET RCSOURCE=
SET LINKSOURCE=
