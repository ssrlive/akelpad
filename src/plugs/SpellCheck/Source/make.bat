rem @ECHO OFF
SETLOCAL
REM /**
REM  *		SpellChecker for AkelPad
REM  *		Panych Y.W. aka FeyFre (c) 2010 (panych.y@gmail.com)
REM  *
REM  *		This piece of software is distributed under BSD license.
REM  *		AkelPad is property of its copyright holders.
REM  *		The New Aspell Copyright (C) 2001-2002 by Kevin Atkinson under the GNU LGPL
REM  *		license version 2.0 or 2.1.
REM  */

SET INCLUDE_OLD=%INCLUDE%
SET LIB_OLD=%LIB%
SET PATH_OLD=%PATH%

IF /I .static==.%1 (
	REM compile in libc[for MSVC 6 and 7 only]
	SET CRT_LIB=libc.lib
	SET CRT_FLAG=/ML
) ELSE (
	REM libc in shared dll
	SET CRT_LIB=msvcrt.lib
	SET CRT_FLAG=/MD
)

CALL "C:\Program Files\Microsoft Visual Studio\VC98\bin\vcvars32.bat"
SET SRCDIR=%~dp0
SET OUTDIR=output
SET INTDIR=output\obj

SET CSOURCE=%SRCDIR%*.cpp
SET RCSOURCE=%SRCDIR%version.rc
SET LINKSOURCE=%INTDIR%\*.res %INTDIR%\*.obj

SET PSDK=d:\.SYSTEM\Microsoft SDKs\Windows\v5.0
SET INCLUDE=%PSDK%\INCLUDE;%INCLUDE%
SET LIB=%PSDK%\LIB;%LIB%

SET LIBS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib %CRT_LIB%

SET CDEFINES=/D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_WARNINGS" /D "_CRT_NON_CONFORMING_SWPRINTFS"
SET CFLAGS=/c /TP %CRT_FLAG% /GR- /GX- /Gi- /Gm- /GD /W3 /O1 /Os /Oy %CDEFINES% /Fo%INTDIR%\ /Fd%INTDIR%
SET RCFLAGS=/l 0x422 /fo%INTDIR%\Version.res /d "NDEBUG" 
SET LFLAGS=/DLL /RELEASE /MAP /MAPINFO:LINES /OPT:NOWIN98 /OPT:REF /OPT:ICF /INCREMENTAL:NO /MACHINE:I386 /PDB:%OUTDIR%\SpellCheck.pdb /OUT:%OUTDIR%\SpellCheck.dll
SET DEFINES=

MKDIR %OUTDIR%
MKDIR %INTDIR%
DEL /F /Q %INTDIR%
DEL /F /Q %OUTDIR%

cl /nologo %CFLAGS% %CSOURCE%
rc %RCFLAGS% %RCSOURCE%
link /nologo %LFLAGS% %LIBS% %LINKSOURCE%

SET PSDK=
SET SRCDIR=
SET OUTDIR=
SET INTDIR=
SET CSOURCE=
SET RCSOURCE=
SET LINKSOURCE=

SET INCLUDE=%INCLUDE_OLD%
SET LIB=%LIB_OLD%
SET PATH=%PATH_OLD%
SET INCLUDE_OLD=
SET LIB_OLD=
SET PATH_OLD=
ENDLOCAL