@ECHO OFF
ECHO /**
ECHO  *		Templates for AkelPad
ECHO  *		Panych Y.W. aka FeyFre (c) 2010-2012 (panych.y@gmail.com)
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
SET RCSOURCE=%SRCDIR%\Templates.rc
SET LINKSOURCE=%INTDIR%\*.res %INTDIR%\*.obj

SET PSDK=C:\PSDK
SET INCLUDE=%PSDK%\INCLUDE;%INCLUDE%
SET LIB=%PSDK%\LIB;%LIB%

SET LIBS=kernel32.lib user32.lib shell32.lib
SET SMALL_ICON_OPT=/D "NO_SMALL_ICON"
SET CFLAGS=/c /GR- /GX- /Gi- /Gm- /W3 /O1 /Os /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "UNICODE" %SMALL_ICON_OPT% /Fo%INTDIR%\ /Fd%INTDIR%
SET RCFLAGS=/l 0x422 /fo%INTDIR%\Templates.res /d "NDEBUG" 
SET LFLAGS=/DLL /ENTRY:DllMain /RELEASE /MAP /MAPINFO:LINES /NODEFAULTLIB /OPT:NOWIN98 /OPT:REF /OPT:ICF /INCREMENTAL:NO /MACHINE:I386 /PDB:%OUTDIR%\Templates.pdb /OUT:%OUTDIR%\Templates.dll
SET SMALL_ICON_OPT=

MKDIR %OUTDIR%
MKDIR %INTDIR%
DEL /F /Q %INTDIR%\*.*
DEL /F /Q %OUTDIR%\Templates.dll

cl /nologo %CFLAGS% %CSOURCE%
rc %RCFLAGS% %RCSOURCE%
link /nologo %LFLAGS% %LIBS% %LINKSOURCE%

SET SRCDIR=
SET OUTDIR=
SET INTDIR=
SET PSDK=
SET CSOURCE=
SET RCSOURCE=
SET LINKSOURCE=
