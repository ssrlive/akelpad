!define MUI_UI "Pages\Modern.exe"
!define PRODUCT_NAME "AkelPad"
!define PRODUCT_VERSION "3.1.8"

;_____________________________________________________________________________________________
;
;                                     Install
;_____________________________________________________________________________________________

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}-setup.exe"
SetCompressor /SOLID lzma
SubCaption 3 ' '
BrandingText "${PRODUCT_NAME} ${PRODUCT_VERSION}"

############  Functions  ############
!include "WordFunc.nsh"
!insertmacro WordReplace

!include "FileFunc.nsh"
!insertmacro GetFileName
!insertmacro un.GetFileName
!insertmacro GetParent
!insertmacro un.GetParent

############  MUI  ############
!include "MUI.nsh"
!define MUI_ICON "Graphics\IconInstall.ico"
!define MUI_UNICON "Graphics\IconUninstall.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "Graphics\HeaderInstall.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "Graphics\HeaderUninstall.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "Graphics\WizardFinish.bmp"

############  Defines  ############
!define LANG_ENGLISH    1033
!define LANG_RUSSIAN    1049
!define LANG_GERMAN     1043
!define LANG_SPANISH    1034
!define LANG_UKRAINIAN  1058
!define LANG_FRENCH     1036

!define INSTTYPE_STANDART 1
!define INSTTYPE_TOTALCMD 2
!define INSTTYPE_NOTEPAD  3

############  Variables  ############
Var INI
Var HWND
Var REDCTL
Var INSTTYPE
Var INSTEXE
Var SYSLANGUAGE

############  Pages  ############
Page Custom CustomShow CustomLeave

PageEx directory
	Caption ' '
	PageCallbacks '' DirectoryShow DirectoryLeave
PageExEnd

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

PageEx un.uninstConfirm
	Caption ' '
	PageCallbacks '' un.uninstConfirmShow un.uninstConfirmLeave
PageExEnd

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

ChangeUI IDD_INST "${MUI_UI}"

############  Strings  ############
LangString Help ${LANG_ENGLISH} 'Help'
LangString Help ${LANG_RUSSIAN} 'Справка'
LangString Delete ${LANG_ENGLISH} 'Uninstall'
LangString Delete ${LANG_RUSSIAN} 'Удалить'
LangString WelcomeInfoTitle ${LANG_ENGLISH} 'Welcome to the $(^Name) Setup Wizard'
LangString WelcomeInfoTitle ${LANG_RUSSIAN} 'Вас приветствует мастер установки $(^Name)'
LangString WelcomeInfoText ${LANG_ENGLISH} 'Choose install type.'
LangString WelcomeInfoText ${LANG_RUSSIAN} 'Выберите тип установки.'
LangString TypeStandart ${LANG_ENGLISH} 'Standart install'
LangString TypeStandart ${LANG_RUSSIAN} 'Стандартная установка'
LangString TypeTotalcmd ${LANG_ENGLISH} 'Editor for Total Commander'
LangString TypeTotalcmd ${LANG_RUSSIAN} 'Редактор для Total Commander'
LangString TypeNotepad ${LANG_ENGLISH} 'Windows notepad replacement'
LangString TypeNotepad ${LANG_RUSSIAN} 'Замена блокнота Windows'
LangString TypeStandartText ${LANG_ENGLISH} 'Program will be installed to the specified directory.'
LangString TypeStandartText ${LANG_RUSSIAN} 'Программа будет установлена в указанную директорию.'
LangString TypeTotalcmdText ${LANG_ENGLISH} 'Program will be installed as external editor for Total Commander file manager.'
LangString TypeTotalcmdText ${LANG_RUSSIAN} 'Программа будет установлена как внешний редактор для файлового менеджера Total Commander.'
LangString TypeNotepadText ${LANG_ENGLISH} 'Windows notepad will be replaced with program. Copy of the notepad will be restored after program uninstall.'
LangString TypeNotepadText ${LANG_RUSSIAN} 'Блокнот Windows будет заменен программой. Копия блокнота будет восстановлена после удаления программы.'
LangString DirectoryInfoTitle ${LANG_ENGLISH} 'Choose Install Location'
LangString DirectoryInfoTitle ${LANG_RUSSIAN} 'Выбор папки установки'
LangString DirectoryInfoText ${LANG_ENGLISH} 'Choose the folder in which to install $(^Name).'
LangString DirectoryInfoText ${LANG_RUSSIAN} 'Выберите папку для установки $(^Name).'
LangString DirectoryTextStandart ${LANG_ENGLISH} 'Setup will install $(^Name) in the following folder. To install in a different folder, click Browse and select another folder.'
LangString DirectoryTextStandart ${LANG_RUSSIAN} 'Программа установит $(^Name) в указанный каталог. Чтобы установить программу в другой каталог, нажмите на кнопку "Обзор".'
LangString DirectoryTextTotalcmd ${LANG_ENGLISH} 'Select Total Commander folder.'
LangString DirectoryTextTotalcmd ${LANG_RUSSIAN} 'Выберите каталог Total Commander.'
LangString DirectoryTextNotepad ${LANG_ENGLISH} 'System folder.'
LangString DirectoryTextNotepad ${LANG_RUSSIAN} 'Системный каталог.'
LangString ShortcutGroup ${LANG_ENGLISH} 'Shortcuts'
LangString ShortcutGroup ${LANG_RUSSIAN} 'Ярлыки'
LangString ShortcutQuickLaunch ${LANG_ENGLISH} 'Quick launch'
LangString ShortcutQuickLaunch ${LANG_RUSSIAN} 'Быстрый запуск'
LangString ShortcutDesktop ${LANG_ENGLISH} 'Desktop'
LangString ShortcutDesktop ${LANG_RUSSIAN} 'Рабочий стол'
LangString ShortcutStartMenu ${LANG_ENGLISH} 'Start menu'
LangString ShortcutStartMenu ${LANG_RUSSIAN} 'Меню пуск'
LangString ConfirmInfoTitle ${LANG_ENGLISH} 'Uninstall $(^Name)'
LangString ConfirmInfoTitle ${LANG_RUSSIAN} 'Удаление $(^Name)'
LangString ConfirmInfoText ${LANG_ENGLISH} 'Remove $(^Name) from your computer.'
LangString ConfirmInfoText ${LANG_RUSSIAN} 'Удаление $(^Name) из компьютера.'
LangString ConfirmDeleteSettings ${LANG_ENGLISH} 'Delete program settings'
LangString ConfirmDeleteSettings ${LANG_RUSSIAN} 'Удалить настройки программы'
LangString InstallAlreadyRun ${LANG_ENGLISH} '${PRODUCT_NAME} running.$\n$\nAfter closing ${PRODUCT_NAME}, select Retry.$\n$\nIf you want abort installation, select Cancel.'
LangString InstallAlreadyRun ${LANG_RUSSIAN} '${PRODUCT_NAME} запущен.$\n$\nПосле того, как Вы закроете ${PRODUCT_NAME}, выберите Повтор.$\n$\nЕсли Вы хотите прервать установку, выберите Отмена.'
LangString UninstallAlreadyRun ${LANG_ENGLISH} '${PRODUCT_NAME} running.$\n$\nAfter closing ${PRODUCT_NAME}, select Retry.$\n$\nIf you want abort uninstallation, select Cancel.'
LangString UninstallAlreadyRun ${LANG_RUSSIAN} '${PRODUCT_NAME} запущен.$\n$\nПосле того, как Вы закроете ${PRODUCT_NAME}, выберите Повтор.$\n$\nЕсли Вы хотите отменить удаление, выберите Отмена.'
LangString UninstallSuccess ${LANG_ENGLISH} 'Uninstall was completed successfully.'
LangString UninstallSuccess ${LANG_RUSSIAN} 'Удаление программы успешно завершено.'

Function .onInit
	FindWindow $0 "AkelPad3"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(InstallAlreadyRun)" IDRETRY  -2
	quit

	# Custom page #
	InitPluginsDir

	GetTempFileName $INI $PLUGINSDIR
	File /oname=$INI "Pages\InstallType.ini"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\IconTypeStandart.ico"
	WriteINIStr "$INI" "Field 7" "Text" "$0"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\IconTypeTotalcmd.ico"
	WriteINIStr "$INI" "Field 8" "Text" "$0"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\IconTypeNotepad.ico"
	WriteINIStr "$INI" "Field 9" "Text" "$0"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\WizardWelcome.bmp"
	WriteINIStr "$INI" "Field 10" "Text" "$0"

	StrCpy $REDCTL 0
	StrCpy $SYSLANGUAGE $LANGUAGE
FunctionEnd

Function CustomShow
	InstallOptions::initDialog /NOUNLOAD "$INI"
	Pop $HWND

	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(WelcomeInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(WelcomeInfoText)'

	ReadINIStr $0 "$INI" "Field 1" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeStandart)'

	ReadINIStr $0 "$INI" "Field 2" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeTotalcmd)'

	ReadINIStr $0 "$INI" "Field 3" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepad)'

	ReadINIStr $0 "$INI" "Field 4" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeStandartText)'

	ReadINIStr $0 "$INI" "Field 5" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeTotalcmdText)'

	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepadText)'

	StrCmp $REDCTL 1 0 +2
	SetCtlColors $0 /BRANDING 0xFF0000

	InstallOptions::show
	Pop $0
FunctionEnd

Function CustomLeave
	ReadINIStr $0 "$INI" "Settings" "State"
	StrCmp $0 0 next
	StrCmp $0 1 standart
	StrCmp $0 2 totalcmd
	StrCmp $0 3 notepad
	abort

	standart:
	totalcmd:
	StrCmp $REDCTL 0 0 +2
	abort

	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SetCtlColors $0 /BRANDING 0x000000
	ShowWindow $0 ${SW_HIDE}
	ShowWindow $0 ${SW_SHOW}
	StrCpy $REDCTL 0
	abort

	notepad:
	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SetCtlColors $0 /BRANDING 0xFF0000
	ShowWindow $0 ${SW_HIDE}
	ShowWindow $0 ${SW_SHOW}
	StrCpy $REDCTL 1
	abort

	next:
	ReadINIStr $0 "$INI" "Field 1" "State"
	StrCmp $0 1 0 _totalcmd
	StrCpy $INSTTYPE ${INSTTYPE_STANDART}

	ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
	StrCmp $0 '' 0 +3
	StrCpy $INSTDIR "$PROGRAMFILES\${PRODUCT_NAME}"
	goto end
	${GetParent} "$0" $0
	${GetParent} "$0" $INSTDIR
	goto end

	_totalcmd:
	ReadINIStr $0 "$INI" "Field 2" "State"
	StrCmp $0 1 0 _notepad
	StrCpy $INSTTYPE ${INSTTYPE_TOTALCMD}

	ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
	StrCmp $0 '' Ghisler
	${GetParent} "$0" $0
	${GetParent} "$0" $0
	${GetParent} "$0" $0
	IfFileExists "$0\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "$0"
	goto end

	Ghisler:
	ReadRegStr $0 HKCU "SOFTWARE\Ghisler\Total Commander" "InstallDir"
	StrCmp $0 '' +4
	IfFileExists "$0\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "$0"
	goto end
	IfFileExists "C:\TotalCmd\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "C:\TotalCmd"
	goto end
	IfFileExists "C:\TC\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "C:\TC"
	goto end
	StrCpy $INSTDIR ""
	goto end

	_notepad:
	ReadINIStr $0 "$INI" "Field 3" "State"
	StrCmp $0 1 0 end
	StrCpy $INSTTYPE ${INSTTYPE_NOTEPAD}

	ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion" "VersionNumber"
	StrCmp $0 '' 0 +3
	StrCpy $INSTDIR "$SYSDIR"
	goto end
	StrCpy $INSTDIR "$WINDIR"

	end:
FunctionEnd

Function DirectoryShow
	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryInfoText)'

	FindWindow $R0 '#32770' '' $HWNDPARENT

	GetDlgItem $0 $R0 1006
	StrCmp $INSTTYPE ${INSTTYPE_STANDART} 0 +2
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextStandart)'
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 +2
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextTotalcmd)'
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 +2
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextNotepad)'

	GetDlgItem $0 $R0 1050
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutGroup)'

	GetDlgItem $0 $R0 1051
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutQuickLaunch)'

	GetDlgItem $0 $R0 1052
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutDesktop)'

	GetDlgItem $0 $R0 1053
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutStartMenu)'

	StrCmp $INSTTYPE ${INSTTYPE_STANDART} 0 +2
	SendMessage $0 ${BM_SETCHECK} 1 0

	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 +5
	GetDlgItem $0 $R0 1019
	EnableWindow $0 0
	GetDlgItem $0 $R0 1001
	EnableWindow $0 0
FunctionEnd

Function DirectoryLeave
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 +3
	StrCpy $INSTEXE "$INSTDIR\notepad.exe"
	goto +2
	StrCpy $INSTEXE "$INSTDIR\AkelPad.exe"

	GetDlgItem $0 $R0 1051
	SendMessage $0 ${BM_GETSTATE} 0 0 $1
	StrCmp $1 1 0 desktop
	SetOutPath "$INSTDIR"
	CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$INSTEXE"

	desktop:
	GetDlgItem $0 $R0 1052
	SendMessage $0 ${BM_GETSTATE} 0 0 $1
	StrCmp $1 1 0 startmenu
	SetOutPath "$INSTDIR"
	CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTEXE"

	startmenu:
	GetDlgItem $0 $R0 1053
	SendMessage $0 ${BM_GETSTATE} 0 0 $1
	StrCmp $1 1 0 end
	SetOutPath "$INSTDIR"
	CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTEXE"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk" "$INSTDIR\AkelFiles\Uninstall.exe"
	StrCmp $SYSLANGUAGE ${LANG_RUSSIAN} 0 +3
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$INSTDIR\AkelFiles\AkelPad-Rus.htm"
	goto +2
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$INSTDIR\AkelFiles\AkelPad-Eng.htm"

	end:
FunctionEnd

Function .onVerifyInstDir
	StrCmp $INSTTYPE ${INSTTYPE_STANDART} end

	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 end
	IfFileExists "$INSTDIR\totalcmd.exe" end
	Abort

	end:
FunctionEnd

Section
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 Install
	StrCpy $0 "$INSTDIR\Wincmd.ini"
	IfFileExists "$0" +3
	SearchPath $0 "Wincmd.ini"
	StrCmp $0 '' ChangeOutPath
	ReadINIStr $1 "$0" "Configuration" "Editor"
	StrCmp $1 '' WriteINIEditor
	${GetFileName} "$1" $2
	StrCmp $2 "Akelpad.exe" WriteINIEditor
	${WordReplace} "$1" "%COMMANDER_PATH%" "$INSTDIR" "+" $2
	IfFileExists $2 0 WriteINIEditor
	WriteINIStr "$0" "Configuration" "Editor_AkelUndo" "$1"
	WriteINIEditor:
	WriteINIStr "$0" "Configuration" "Editor" "%COMMANDER_PATH%\AkelPad\Akelpad.exe"
	ChangeOutPath:
	StrCpy $INSTDIR "$INSTDIR\AkelPad"

	Install:
	SetOutPath "$INSTDIR"
	File "Files\AkelPad.exe"

	SetOutPath "$INSTDIR\AkelFiles"
	File "Files\AkelFiles\AkelPad-Eng.htm"
	File "Files\AkelFiles\AkelPad-Rus.htm"
	File "Files\AkelFiles\History-Rus.txt"

	SetOutPath "$INSTDIR\AkelFiles\Langs"
	File "Files\AkelFiles\Langs\English.dll"
	File "Files\AkelFiles\Langs\Russian.dll"
	File "Files\AkelFiles\Langs\German.dll"
	File "Files\AkelFiles\Langs\Spanish.dll"
	File "Files\AkelFiles\Langs\Ukrainian.dll"
	File "Files\AkelFiles\Langs\French.dll"
	SetOutPath "$INSTDIR\AkelFiles\Plugs"

	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 RegInfo
	IfFileExists "$INSTDIR\notepad_AkelUndo.exe" +2
	CopyFiles /SILENT "$INSTDIR\notepad.exe" "$INSTDIR\notepad_AkelUndo.exe"
	IfFileExists "$INSTDIR\DLLCACHE\notepad.exe" 0 +3
	Delete "$INSTDIR\DLLCACHE\notepad.exe"
	CopyFiles /SILENT "$INSTDIR\AkelPad.exe" "$INSTDIR\DLLCACHE\notepad.exe"
	IfFileExists "$INSTDIR\notepad.exe" 0 +2
	Delete "$INSTDIR\notepad.exe"
	Rename "$INSTDIR\AkelPad.exe" "$INSTDIR\notepad.exe"

	StrCmp $INSTDIR $SYSDIR 0 RegInfo
	SetOutPath "$WINDIR"
	File "Redirect\notepad.exe"

	RegInfo:
	WriteUninstaller "$INSTDIR\AkelFiles\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayName" "$(^Name)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayIcon" "$INSTEXE"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayVersion" "${PRODUCT_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString" "$INSTDIR\AkelFiles\Uninstall.exe"

	ClearErrors
	ReadRegStr $0 HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule"
	IfErrors 0 end
	StrCmp $SYSLANGUAGE ${LANG_RUSSIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Russian.dll"
	StrCmp $SYSLANGUAGE ${LANG_GERMAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "German.dll"
	StrCmp $SYSLANGUAGE ${LANG_SPANISH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Spanish.dll"
	StrCmp $SYSLANGUAGE ${LANG_UKRAINIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Ukrainian.dll"
	StrCmp $SYSLANGUAGE ${LANG_FRENCH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "French.dll"

	end:
SectionEnd


;_____________________________________________________________________________________________
;
;                                     Uninstall
;_____________________________________________________________________________________________

Function un.onInit
	FindWindow $0 "AkelPad3"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(UninstallAlreadyRun)" IDRETRY  -2
	quit
FunctionEnd

Function un.uninstConfirmShow
	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ConfirmInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ConfirmInfoText)'

	FindWindow $R0 '#32770' '' $HWNDPARENT

	GetDlgItem $0 $R0 1051
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ConfirmDeleteSettings)'
	SendMessage $0 ${BM_SETCHECK} 1 0
FunctionEnd

Function un.uninstConfirmLeave
	GetDlgItem $0 $R0 1051
	SendMessage $0 ${BM_GETSTATE} 0 0 $1

	StrCmp $1 0 end
	DeleteRegKey HKCU "SOFTWARE\Akelsoft\${PRODUCT_NAME}"

	${un.GetParent} "$INSTDIR" $0
	IfFileExists "$0\AkelPad.ini" 0 +2
	Delete "$0\AkelPad.ini"

	end:
FunctionEnd

Section un.install
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

	SetOutPath "$TEMP"
	Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
	Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk"
	RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

	Delete "$INSTDIR\Langs\English.dll"
	Delete "$INSTDIR\Langs\Russian.dll"
	Delete "$INSTDIR\Langs\German.dll"
	Delete "$INSTDIR\Langs\Spanish.dll"
	Delete "$INSTDIR\Langs\Ukrainian.dll"
	Delete "$INSTDIR\Langs\French.dll"
	Delete "$INSTDIR\History-Rus.txt"
	Delete "$INSTDIR\AkelPad-Eng.htm"
	Delete "$INSTDIR\AkelPad-Rus.htm"
	Delete "$INSTDIR\Uninstall.exe"
	RMDir "$INSTDIR\Langs"
	RMDir "$INSTDIR\Plugs"
	RMDir "$INSTDIR"

	${un.GetParent} "$INSTDIR" $0
	Delete "$0\AkelPad.exe"
	RMDir "$0"

	StrCmp $0 $SYSDIR +2
	StrCmp $0 $WINDIR 0 UnTotalcmd
	IfFileExists "$0\notepad_AkelUndo.exe" 0 Success
	IfFileExists "$0\DLLCACHE\notepad.exe" 0 +3
	Delete "$0\DLLCACHE\notepad.exe"
	CopyFiles /SILENT "$0\notepad_AkelUndo.exe" "$0\DLLCACHE\notepad.exe"
	Delete "$0\notepad.exe"
	Rename "$0\notepad_AkelUndo.exe" "$0\notepad.exe"

	StrCmp $0 $SYSDIR 0 UnTotalcmd
	Delete "$WINDIR\notepad.exe"
	CopyFiles /SILENT "$SYSDIR\notepad.exe" "$WINDIR\notepad.exe"

	UnTotalcmd:
	${un.GetParent} "$0" $0
	IfFileExists "$0\Totalcmd.exe" 0 Success
	StrCpy $0 "$0\Wincmd.ini"
	IfFileExists "$0" +3
	SearchPath $0 "Wincmd.ini"
	StrCmp $0 '' Success
	ReadINIStr $1 "$0" "Configuration" "Editor"
	${un.GetFileName} "$1" $2
	StrCmp $2 "Akelpad.exe" 0 DeleteINIEditorUndo
	DeleteINIStr "$0" "Configuration" "Editor"
	ReadINIStr $1 "$0" "Configuration" "Editor_AkelUndo"
	StrCmp $1 '' Success
	WriteINIStr "$0" "Configuration" "Editor" "$1"
	DeleteINIEditorUndo:
	DeleteINIStr "$0" "Configuration" "Editor_AkelUndo"

	Success:
	StrCpy $0 SuccessUninstall
	quit
SectionEnd

Function un.onGUIEnd
	StrCmp $0 SuccessUninstall 0 +2
	MessageBox MB_ICONINFORMATION|MB_OK "$(UninstallSuccess)"
FunctionEnd
