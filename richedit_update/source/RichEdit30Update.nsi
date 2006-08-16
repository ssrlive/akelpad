;-----------------------------------------------------------------------------------------------
; NSIS script for RichEdit update to 3.0 version
;
; 2006 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)
;-----------------------------------------------------------------------------------------------

Name "RichEdit v3.0 Update"
OutFile "RichEdit30Update.exe"
Caption "$(^Name)"
SetCompressor /SOLID lzma

!include "FileFunc.nsh"
!insertmacro GetFileVersion

!include "WordFunc.nsh"
!insertmacro VersionCompare

Function .onInit
	${GetFileVersion} "$SYSDIR\riched20.dll" $0
	IfErrors Update

	${VersionCompare} "5.30.23.1221" "$0" $1
	StrCmp $1 1 0 AlreadyInstalled

	Update:
	SetOutPath $SYSDIR
	File "riched20.dll"
	MessageBox MB_OK|MB_ICONINFORMATION "RichEdit v3.0 successfully installed"
	goto End

	AlreadyInstalled:
	MessageBox MB_OK|MB_ICONINFORMATION "RichEdit v3.0 or later already installed"

	End:
	Quit
FunctionEnd

Section
SectionEnd
