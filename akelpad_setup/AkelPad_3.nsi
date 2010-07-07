!define PRODUCT_NAME "AkelPad"
!define PRODUCT_VERSION "3.7.4"

;_____________________________________________________________________________________________
;
;                                     Install
;_____________________________________________________________________________________________

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}-setup.exe"
SetCompressor /SOLID lzma
SubCaption 3 ' '
BrandingText "${PRODUCT_NAME} ${PRODUCT_VERSION}"
RequestExecutionLevel admin


############  Functions  ############
!include "FileFunc.nsh"
!insertmacro GetParameters
!insertmacro GetOptions
!insertmacro GetFileName
!insertmacro un.GetFileName
!insertmacro GetParent
!insertmacro un.GetParent

!include "WordFunc.nsh"
!insertmacro WordReplace

############  MUI  ############
!include "MUI.nsh"
!define MUI_UI "Pages\Modern.exe"
!define MUI_UI_HEADERIMAGE_RIGHT "Pages\Modern_headerbmpr.exe"
!define MUI_ICON "Graphics\IconInstall.ico"
!define MUI_UNICON "Graphics\IconUninstall.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "Graphics\HeaderInstall.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "Graphics\HeaderUninstall.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "Graphics\WizardFinish.bmp"
!define MUI_FINISHPAGE_RUN "$SETUPEXE"

############  Defines  ############
!define LANG_ENGLISH              1033
!define LANG_RUSSIAN              1049
!define LANG_GERMAN               1043
!define LANG_SPANISH              1034
!define LANG_UKRAINIAN            1058
!define LANG_BELORUSSIAN          1059
!define LANG_FRENCH               1036
!define LANG_JAPANESE             1041
!define LANG_BRAZILIAN            1046
!define LANG_ITALIAN              1040
!define LANG_KOREAN               1042
!define LANG_CHINESE_TRADITIONAL  1028
!define LANG_CHINESE_SIMPLIFIED   2052
!define LANG_POLISH               1045
!define LANG_TURKISH              1055
!define LANG_KYRGYZ               1088

!define INSTTYPE_STANDARD  1
!define INSTTYPE_TOTALCMD  2
!define INSTTYPE_NOTEPAD   3

!define SHORTCUT_QUICKLAUNCH  0x1
!define SHORTCUT_DESKTOP      0x2
!define SHORTCUT_STARTMENU    0x4

############  Variables  ############
Var PARAMETERS
Var INI
Var HWND
Var REDCTL
Var INSTTYPE
Var SHORTCUT
Var SETUPDIR
Var SETUPEXE
Var TCDIR
Var TCINI
Var SYSLANGUAGE
Var UNSETTINGS
Var UNFILES
Var UNRESULT

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

############  Strings  ############
LangString Help ${LANG_ENGLISH} 'Help'
LangString Help ${LANG_RUSSIAN} 'Справка'
LangString Delete ${LANG_ENGLISH} 'Uninstall'
LangString Delete ${LANG_RUSSIAN} 'Удалить'
LangString WelcomeInfoTitle ${LANG_ENGLISH} 'Welcome to the $(^Name) Setup Wizard'
LangString WelcomeInfoTitle ${LANG_RUSSIAN} 'Вас приветствует мастер установки $(^Name)'
LangString WelcomeInfoText ${LANG_ENGLISH} 'Choose install type.'
LangString WelcomeInfoText ${LANG_RUSSIAN} 'Выберите тип установки.'
LangString TypeStandard ${LANG_ENGLISH} 'Standard install'
LangString TypeStandard ${LANG_RUSSIAN} 'Стандартная установка'
LangString TypeTotalcmd ${LANG_ENGLISH} 'Editor for Total Commander'
LangString TypeTotalcmd ${LANG_RUSSIAN} 'Редактор для Total Commander'
LangString TypeNotepad ${LANG_ENGLISH} 'Windows notepad replacement'
LangString TypeNotepad ${LANG_RUSSIAN} 'Замена блокнота Windows'
LangString TypeStandardText ${LANG_ENGLISH} 'Program will be installed to the specified directory.'
LangString TypeStandardText ${LANG_RUSSIAN} 'Программа будет установлена в указанную директорию.'
LangString TypeTotalcmdText ${LANG_ENGLISH} 'Program will be installed as external editor for Total Commander file manager.'
LangString TypeTotalcmdText ${LANG_RUSSIAN} 'Программа будет установлена как внешний редактор для файлового менеджера Total Commander.'
LangString TypeNotepadText ${LANG_ENGLISH} 'Windows notepad will be replaced with program. Copy of the notepad will be restored after program uninstall.'
LangString TypeNotepadText ${LANG_RUSSIAN} 'Блокнот Windows будет заменен программой. Копия блокнота будет восстановлена после удаления программы.'
LangString DirectoryInfoTitle ${LANG_ENGLISH} 'Choose Install Location'
LangString DirectoryInfoTitle ${LANG_RUSSIAN} 'Выбор папки установки'
LangString DirectoryInfoText ${LANG_ENGLISH} 'Choose the folder in which to install $(^Name).'
LangString DirectoryInfoText ${LANG_RUSSIAN} 'Выберите папку для установки $(^Name).'
LangString DirectoryTextStandard ${LANG_ENGLISH} 'Setup will install $(^Name) in the following folder. To install in a different folder, click Browse and select another folder.'
LangString DirectoryTextStandard ${LANG_RUSSIAN} 'Программа установит $(^Name) в указанный каталог. Чтобы установить программу в другой каталог, нажмите на кнопку "Обзор".'
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
LangString ConfirmDeleteSettings ${LANG_ENGLISH} 'Delete settings'
LangString ConfirmDeleteSettings ${LANG_RUSSIAN} 'Удалить настройки'
LangString ConfirmDeleteFiles ${LANG_ENGLISH} 'Delete files'
LangString ConfirmDeleteFiles ${LANG_RUSSIAN} 'Удалить файлы'
LangString InstallAlreadyRun ${LANG_ENGLISH} '${PRODUCT_NAME} running.$\n$\nAfter closing ${PRODUCT_NAME}, select Retry.$\n$\nIf you want abort installation, select Cancel.'
LangString InstallAlreadyRun ${LANG_RUSSIAN} '${PRODUCT_NAME} запущен.$\n$\nПосле того, как Вы закроете ${PRODUCT_NAME}, выберите Повтор.$\n$\nЕсли Вы хотите прервать установку, выберите Отмена.'
LangString UninstallAlreadyRun ${LANG_ENGLISH} '${PRODUCT_NAME} running.$\n$\nAfter closing ${PRODUCT_NAME}, select Retry.$\n$\nIf you want abort uninstallation, select Cancel.'
LangString UninstallAlreadyRun ${LANG_RUSSIAN} '${PRODUCT_NAME} запущен.$\n$\nПосле того, как Вы закроете ${PRODUCT_NAME}, выберите Повтор.$\n$\nЕсли Вы хотите отменить удаление, выберите Отмена.'
LangString UninstallSuccess ${LANG_ENGLISH} 'Uninstall was completed successfully.'
LangString UninstallSuccess ${LANG_RUSSIAN} 'Удаление программы успешно завершено.'

Function .onInit
	#Help message
	${GetParameters} $PARAMETERS
	ExpandEnvStrings $PARAMETERS $PARAMETERS
	StrCmp $PARAMETERS '/?' 0 CheckWindow
	MessageBox MB_OK \
	`|   Command line options:$\n\
	 |   $\n\
	 |   /S$\n\
	 |     Silent mode$\n\
	 |   /TYPE=[1|2|3]$\n\
	 |     Installation type:$\n\
	 |     1 - Standard install$\n\
	 |     2 - Editor for Total Commander$\n\
	 |     3 - Windows notepad replacement$\n\
	 |     default: 1$\n\
	 |   /DIR=[path]$\n\
	 |     Installation directory$\n\
	 |     default: %Program Files%\AkelPad$\n\
	 |   /SHORTCUT=[0x0]$\n\
	 |     Create shortcuts:$\n\
	 |     0x1 - quick launch$\n\
	 |     0x2 - desktop$\n\
	 |     0x4 - start menu$\n\
	 |     default: 0x0$\n\
	 $\n\
	 Example:$\n\
	 setup.exe /S /TYPE=1 /DIR="%SystemDrive%\AkelPad" /SHORTCUT=0x7`
	quit

	#Is AkelPad running?
	CheckWindow:
	FindWindow $0 "AkelPad4"
	IsWindow $0 +5
	FindWindow $0 "AkelPad3"
	IsWindow $0 +3
	FindWindow $0 "AkelPad2"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(InstallAlreadyRun)" IDRETRY CheckWindow
	quit

	#Variables
	StrCpy $SYSLANGUAGE $LANGUAGE
	StrCpy $INSTTYPE ${INSTTYPE_STANDARD}
	StrCpy $SHORTCUT 0

	${GetOptions} $PARAMETERS "/TYPE=" $0
	IfErrors dir
	StrCpy $INSTTYPE $0
	dir:
	StrCmp $INSTDIR '' 0 shortcut
	${GetOptions} $PARAMETERS "/DIR=" $0
	IfErrors shortcut
	StrCpy $INSTDIR $0
	shortcut:
	${GetOptions} $PARAMETERS "/SHORTCUT=" $0
	IfErrors silent
	StrCpy $SHORTCUT $0

	#Silent install
	silent:
	IfSilent 0 custom
	StrCmp $INSTDIR '' 0 +2
	Call GetInstallDirectory
	Call SetInstallDirectory
	Call CreateShortcuts
	Return

	#Custom page
	custom:
	InitPluginsDir

	StrCpy $REDCTL 0

	GetTempFileName $INI $PLUGINSDIR
	File /oname=$INI "Pages\InstallType.ini"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\IconTypeStandard.ico"
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
FunctionEnd

Function CustomShow
	InstallOptions::initDialog /NOUNLOAD "$INI"
	Pop $HWND

	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(WelcomeInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(WelcomeInfoText)'

	ReadINIStr $0 "$INI" "Field 1" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeStandard)'

	ReadINIStr $0 "$INI" "Field 2" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeTotalcmd)'

	ReadINIStr $0 "$INI" "Field 3" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepad)'

	ReadINIStr $0 "$INI" "Field 4" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeStandardText)'

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
	StrCmp $0 1 color_standard
	StrCmp $0 2 color_totalcmd
	StrCmp $0 3 color_notepad
	abort

	color_standard:
	color_totalcmd:
	StrCmp $REDCTL 0 0 +2
	abort

	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SetCtlColors $0 /BRANDING 0x000000
	ShowWindow $0 ${SW_HIDE}
	ShowWindow $0 ${SW_SHOW}
	StrCpy $REDCTL 0
	abort

	color_notepad:
	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SetCtlColors $0 /BRANDING 0xFF0000
	ShowWindow $0 ${SW_HIDE}
	ShowWindow $0 ${SW_SHOW}
	StrCpy $REDCTL 1
	abort

	next:
	ReadINIStr $0 "$INI" "Field 1" "State"
	StrCmp $0 1 0 +3
	StrCpy $INSTTYPE ${INSTTYPE_STANDARD}
	goto getdir
	ReadINIStr $0 "$INI" "Field 2" "State"
	StrCmp $0 1 0 +3
	StrCpy $INSTTYPE ${INSTTYPE_TOTALCMD}
	goto getdir
	ReadINIStr $0 "$INI" "Field 3" "State"
	StrCmp $0 1 0 getdir
	StrCpy $INSTTYPE ${INSTTYPE_NOTEPAD}

	getdir:
	Call GetInstallDirectory
FunctionEnd

Function DirectoryShow
	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryInfoText)'

	FindWindow $R0 '#32770' '' $HWNDPARENT

	GetDlgItem $0 $R0 1006
	StrCmp $INSTTYPE ${INSTTYPE_STANDARD} 0 +2
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextStandard)'
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

	StrCmp $INSTTYPE ${INSTTYPE_STANDARD} 0 +2
	SendMessage $0 ${BM_SETCHECK} 1 0
FunctionEnd

Function .onVerifyInstDir
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 end
	StrCpy $TCDIR $INSTDIR
	IfFileExists "$TCDIR\totalcmd.exe" end
	${GetParent} "$INSTDIR" $TCDIR
	IfFileExists "$TCDIR\totalcmd.exe" end
	Abort

	end:
FunctionEnd

Function DirectoryLeave
	Call SetInstallDirectory

	GetDlgItem $0 $R0 1051
	SendMessage $0 ${BM_GETCHECK} 0 0 $1
	StrCmp $1 1 0 +2
	IntOp $SHORTCUT $SHORTCUT | ${SHORTCUT_QUICKLAUNCH}

	GetDlgItem $0 $R0 1052
	SendMessage $0 ${BM_GETCHECK} 0 0 $1
	StrCmp $1 1 0 +2
	IntOp $SHORTCUT $SHORTCUT | ${SHORTCUT_DESKTOP}

	GetDlgItem $0 $R0 1053
	SendMessage $0 ${BM_GETCHECK} 0 0 $1
	StrCmp $1 1 0 +2
	IntOp $SHORTCUT $SHORTCUT | ${SHORTCUT_STARTMENU}

	Call CreateShortcuts
FunctionEnd

Function GetInstallDirectory
	#_standart:
	StrCmp $INSTTYPE ${INSTTYPE_STANDARD} 0 _totalcmd
	StrCpy $INSTDIR "$PROGRAMFILES\${PRODUCT_NAME}"
	ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
	${GetParent} "$0" $0
	${GetParent} "$0" $0
	StrCmp $0 '' end
	StrCpy $INSTDIR $0
	goto end

	_totalcmd:
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 _notepad
	ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
	StrCmp $0 '' Ghisler
	${GetParent} "$0" $0
	${GetParent} "$0" $0
	${GetParent} "$0" $0
	IfFileExists "$0\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "$0\${PRODUCT_NAME}"
	goto end

	Ghisler:
	ReadRegStr $0 HKCU "SOFTWARE\Ghisler\Total Commander" "InstallDir"
	StrCmp $0 '' +4
	IfFileExists "$0\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "$0\${PRODUCT_NAME}"
	goto end
	IfFileExists "C:\TotalCmd\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "C:\TotalCmd\${PRODUCT_NAME}"
	goto end
	IfFileExists "C:\TC\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "C:\TC\${PRODUCT_NAME}"
	goto end
	StrCpy $INSTDIR ""
	goto end

	_notepad:
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 end
	ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion" "VersionNumber"
	StrCmp $0 '' 0 +3
	StrCpy $INSTDIR "$SYSDIR"
	goto end
	StrCpy $INSTDIR "$WINDIR"

	end:
FunctionEnd

Function SetInstallDirectory
	StrCpy $SETUPDIR "$INSTDIR"
	StrCpy $SETUPEXE "$SETUPDIR\AkelPad.exe"
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 end
	StrCmp $SETUPDIR $WINDIR +2
	StrCmp $SETUPDIR $SYSDIR 0 +2
	StrCpy $SETUPEXE "$SETUPDIR\notepad.exe"

	end:
FunctionEnd

Function CreateShortcuts
	#quicklaunch:
	IntOp $0 $SHORTCUT & ${SHORTCUT_QUICKLAUNCH}
	IntCmp $0 0 desktop
	SetOutPath "$SETUPDIR"
	CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$SETUPEXE"

	desktop:
	IntOp $0 $SHORTCUT & ${SHORTCUT_DESKTOP}
	IntCmp $0 0 startmenu
	SetOutPath "$SETUPDIR"
	CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$SETUPEXE"

	startmenu:
	IntOp $0 $SHORTCUT & ${SHORTCUT_STARTMENU}
	IntCmp $0 0 end
	SetOutPath "$SETUPDIR"
	CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$SETUPEXE"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk" "$SETUPDIR\AkelFiles\Uninstall.exe"
	StrCmp $SYSLANGUAGE ${LANG_RUSSIAN} 0 +3
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$SETUPDIR\AkelFiles\Docs\AkelHelp-Rus.htm"
	goto +2
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$SETUPDIR\AkelFiles\Docs\AkelHelp-Eng.htm"

	end:
FunctionEnd

Section
	SetOutPath "$SETUPDIR"
	File /r /x Stats*.* "Files\*.*"

	#IfFileExists "$SETUPDIR\AkelFiles\Plugs\Scripts.dll" 0 +2
	#RegDLL "$SETUPDIR\AkelFiles\Plugs\Scripts.dll"

#	_standard:
	StrCmp $INSTTYPE ${INSTTYPE_STANDARD} 0 _totalcmd
	ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
	goto RegInfo

	_totalcmd:
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 _notepad
	ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
	StrCpy $TCINI "$TCDIR\Wincmd.ini"
	IfFileExists "$TCINI" +3
	SearchPath $TCINI "Wincmd.ini"
	StrCmp $TCINI '' RegInfo
	ReadINIStr $1 "$TCINI" "Configuration" "Editor"
	StrCmp $1 '' WriteINIEditor
	${GetFileName} "$1" $2
	StrCmp $2 "Akelpad.exe" WriteINIEditor
	WriteINIStr "$TCINI" "Configuration" "Editor_AkelUndo" "$1"
	WriteINIEditor:
	${WordReplace} "$SETUPDIR\AkelPad.exe" "$TCDIR" "%COMMANDER_PATH%" "+" $1
	WriteINIStr "$TCINI" "Configuration" "Editor" "$1"
	goto RegInfo

	_notepad:
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 RegInfo
	SearchPath $0 takeown.exe
	StrCmp $0 "" backup
	SearchPath $0 cacls.exe
	StrCmp $0 "" backup
	nsExec::Exec 'takeown.exe /F $WINDIR\notepad.exe'
	Pop $0
	nsExec::Exec 'takeown.exe /F $SYSDIR\notepad.exe'
	Pop $0
	nsExec::Exec '%comspec% /c "echo y|cacls.exe $WINDIR\notepad.exe /G %USERNAME%:F"'
	Pop $0
	nsExec::Exec '%comspec% /c "echo y|cacls.exe $SYSDIR\notepad.exe /G %USERNAME%:F"'
	Pop $0

	backup:
	IfFileExists "$WINDIR\notepad_AkelUndo.exe" +2
	CopyFiles /SILENT "$WINDIR\notepad.exe" "$WINDIR\notepad_AkelUndo.exe"
	IfFileExists "$SYSDIR\notepad_AkelUndo.exe" +2
	CopyFiles /SILENT "$SYSDIR\notepad.exe" "$SYSDIR\notepad_AkelUndo.exe"
	IfFileExists "$SYSDIR\DLLCACHE\notepad.exe" 0 +3
	Delete "$SYSDIR\DLLCACHE\notepad.exe"
	CopyFiles /SILENT "$SETUPDIR\AkelPad.exe" "$SYSDIR\DLLCACHE\notepad.exe"

#	__windir:
	StrCmp $SETUPDIR $WINDIR 0 __sysdir
	IfFileExists "$SETUPDIR\notepad.exe" 0 +2
	Delete "$SETUPDIR\notepad.exe"
	Rename "$SETUPDIR\AkelPad.exe" "$SETUPDIR\notepad.exe"
	ExecWait '"$SETUPDIR\notepad.exe" /reassoc /quit'
	SetOutPath "$SYSDIR"
	File "Redirect\notepad.exe"
	WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\notepad.exe"
	goto RegInfo

	__sysdir:
	StrCmp $SETUPDIR $SYSDIR 0 __nonsystem
	IfFileExists "$SETUPDIR\notepad.exe" 0 +2
	Delete "$SETUPDIR\notepad.exe"
	Rename "$SETUPDIR\AkelPad.exe" "$SETUPDIR\notepad.exe"
	ExecWait '"$SETUPDIR\notepad.exe" /reassoc /quit'
	SetOutPath "$WINDIR"
	File "Redirect\notepad.exe"
	WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\notepad.exe"
	goto RegInfo

	__nonsystem:
	ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
	SetOutPath "$WINDIR"
	File "Redirect\notepad.exe"
	SetOutPath "$SYSDIR"
	File "Redirect\notepad.exe"
	WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\AkelPad.exe"

	RegInfo:
	WriteUninstaller "$SETUPDIR\AkelFiles\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayName" "$(^Name)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayIcon" "$SETUPEXE"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayVersion" "${PRODUCT_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString" "$SETUPDIR\AkelFiles\Uninstall.exe"

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
	StrCmp $SYSLANGUAGE ${LANG_BELORUSSIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Belorussian.dll"
	StrCmp $SYSLANGUAGE ${LANG_FRENCH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "French.dll"
	StrCmp $SYSLANGUAGE ${LANG_JAPANESE} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Japanese.dll"
	StrCmp $SYSLANGUAGE ${LANG_BRAZILIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Brazilian.dll"
	StrCmp $SYSLANGUAGE ${LANG_ITALIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Italian.dll"
	StrCmp $SYSLANGUAGE ${LANG_KOREAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Korean.dll"
	StrCmp $SYSLANGUAGE ${LANG_CHINESE_TRADITIONAL} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Chinese (Traditional).dll"
	StrCmp $SYSLANGUAGE ${LANG_CHINESE_SIMPLIFIED} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Chinese (Simplified).dll"
	StrCmp $SYSLANGUAGE ${LANG_POLISH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Polish.dll"
	StrCmp $SYSLANGUAGE ${LANG_TURKISH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Turkish.dll"
	StrCmp $SYSLANGUAGE ${LANG_KYRGYZ} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Kyrgyz.dll"

	end:
SectionEnd


;_____________________________________________________________________________________________
;
;                                     Uninstall
;_____________________________________________________________________________________________

Function un.onInit
	CheckWindow:
	FindWindow $0 "AkelPad4"
	IsWindow $0 +5
	FindWindow $0 "AkelPad3"
	IsWindow $0 +3
	FindWindow $0 "AkelPad2"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(UninstallAlreadyRun)" IDRETRY CheckWindow
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

	GetDlgItem $0 $R0 1052
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ConfirmDeleteFiles)'
	SendMessage $0 ${BM_SETCHECK} 1 0
FunctionEnd

Function un.uninstConfirmLeave
	GetDlgItem $0 $R0 1051
	SendMessage $0 ${BM_GETCHECK} 0 0 $UNSETTINGS

	GetDlgItem $0 $R0 1052
	SendMessage $0 ${BM_GETCHECK} 0 0 $UNFILES
FunctionEnd

Section un.install
	${un.GetParent} "$INSTDIR" $SETUPDIR

	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
	Delete "$SETUPDIR\AkelFiles\Uninstall.exe"

	SetOutPath "$TEMP"
	Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
	Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk"
	RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

	StrCmp $UNSETTINGS 0 +4
	DeleteRegKey HKCU "SOFTWARE\Akelsoft\${PRODUCT_NAME}"
	IfFileExists "$SETUPDIR\AkelPad.ini" 0 +2
	Delete "$SETUPDIR\AkelPad.ini"

	StrCmp $UNFILES 0 End
	#IfFileExists "$SETUPDIR\AkelFiles\Plugs\Scripts.dll" 0 +2
	#UnRegDLL "$SETUPDIR\AkelFiles\Plugs\Scripts.dll"

#	_notepad:
	SearchPath $0 takeown.exe
	StrCmp $0 "" deassoc
	SearchPath $0 cacls.exe
	StrCmp $0 "" deassoc
	nsExec::Exec 'takeown.exe /F $WINDIR\notepad.exe'
	Pop $0
	nsExec::Exec 'takeown.exe /F $SYSDIR\notepad.exe'
	Pop $0
	nsExec::Exec '%comspec% /c "echo y|cacls.exe $WINDIR\notepad.exe /G %USERNAME%:F"'
	Pop $0
	nsExec::Exec '%comspec% /c "echo y|cacls.exe $SYSDIR\notepad.exe /G %USERNAME%:F"'
	Pop $0

	deassoc:
	IfFileExists "$WINDIR\notepad_AkelUndo.exe" 0 +3
	ExecWait '"$WINDIR\notepad.exe" /deassoc /quit'
	goto +3
	IfFileExists "$SYSDIR\notepad_AkelUndo.exe" 0 _totalcmd
	ExecWait '"$SYSDIR\notepad.exe" /deassoc /quit'

	IfFileExists "$WINDIR\notepad_AkelUndo.exe" 0 +3
	Delete "$WINDIR\notepad.exe"
	Rename "$WINDIR\notepad_AkelUndo.exe" "$WINDIR\notepad.exe"
	IfFileExists "$SYSDIR\notepad_AkelUndo.exe" 0 +3
	Delete "$SYSDIR\notepad.exe"
	Rename "$SYSDIR\notepad_AkelUndo.exe" "$SYSDIR\notepad.exe"

	IfFileExists "$SYSDIR\DLLCACHE\notepad.exe" 0 +3
	Delete "$SYSDIR\DLLCACHE\notepad.exe"
	CopyFiles /SILENT "$SYSDIR\notepad.exe" "$SYSDIR\DLLCACHE\notepad.exe"

	DeleteRegValue HKLM "Software\Akelsoft\AkelPad" "Path"

	_totalcmd:
	StrCpy $TCDIR $SETUPDIR
	IfFileExists "$TCDIR\totalcmd.exe" +3
	${un.GetParent} "$SETUPDIR" $TCDIR
	IfFileExists "$TCDIR\totalcmd.exe" 0 _standard
	ExecWait '"$SETUPDIR\AkelPad.exe" /deassoc /quit'
	StrCpy $TCINI "$TCDIR\Wincmd.ini"
	IfFileExists "$TCINI" +3
	SearchPath $TCINI "Wincmd.ini"
	StrCmp $TCINI '' DeleteFiles
	ReadINIStr $1 "$TCINI" "Configuration" "Editor"
	StrCmp $1 '' RestoreAkelUndo
	${un.GetFileName} "$1" $2
	StrCmp $2 "Akelpad.exe" 0 DeleteFiles
	DeleteINIStr "$TCINI" "Configuration" "Editor"
	RestoreAkelUndo:
	ReadINIStr $1 "$TCINI" "Configuration" "Editor_AkelUndo"
	StrCmp $1 '' DeleteFiles
	WriteINIStr "$TCINI" "Configuration" "Editor" "$1"
	DeleteINIStr "$TCINI" "Configuration" "Editor_AkelUndo"
	goto DeleteFiles

	_standard:
	ExecWait '"$SETUPDIR\AkelPad.exe" /deassoc /quit'

	DeleteFiles:
	;Generate list and include it in script at compile-time
	!execute 'unList\unList.exe /DATE=0 /INSTDIR="Files" /LOG=unList.txt /UNDIR_VAR=$SETUPDIR /MB=0'
	!include 'unList\unList.txt'
	!delfile 'unList\unList.txt'
	RMDir "$SETUPDIR"

	End:
	StrCpy $UNRESULT SuccessUninstall
	quit
SectionEnd

Function un.onGUIEnd
	StrCmp $UNRESULT SuccessUninstall 0 +2
	MessageBox MB_ICONINFORMATION|MB_OK "$(UninstallSuccess)"
FunctionEnd
