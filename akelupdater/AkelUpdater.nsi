!define PRODUCT_NAME "AkelUpdater"
!define PRODUCT_VERSION "1.7"

Name "AkelUpdater"
OutFile "AkelUpdater.exe"
SetCompressor /SOLID lzma

!define LANG_ENGLISH  1033
!define LANG_RUSSIAN  1049

LangString InstallAlreadyRun ${LANG_ENGLISH} 'AkelPad running.$\n$\nAfter closing AkelPad, select Retry.$\n$\nIf you want abort installation, select Cancel.'
LangString InstallAlreadyRun ${LANG_RUSSIAN} 'AkelPad запущен.$\n$\nПосле того, как Вы закроете AkelPad, выберите Повтор.$\n$\nЕсли Вы хотите прервать установку, выберите Отмена.'
LangString lng ${LANG_ENGLISH} 'eng'
LangString lng ${LANG_RUSSIAN} 'rus'
LangString url ${LANG_ENGLISH} 'URL'
LangString url ${LANG_RUSSIAN} 'Ссылка'
LangString downloading ${LANG_ENGLISH} 'Downloading'
LangString downloading ${LANG_RUSSIAN} 'Скачивание'
LangString connecting ${LANG_ENGLISH} 'Сonnecting'
LangString connecting ${LANG_RUSSIAN} 'Соединение'
LangString file_name ${LANG_ENGLISH} 'File Name'
LangString file_name ${LANG_RUSSIAN} 'Имя файла'
LangString received ${LANG_ENGLISH} 'Received'
LangString received ${LANG_RUSSIAN} 'Получено'
LangString file_size ${LANG_ENGLISH} 'File Size'
LangString file_size ${LANG_RUSSIAN} 'Размер'
LangString remaining_time ${LANG_ENGLISH} 'Remaining Time'
LangString remaining_time ${LANG_RUSSIAN} 'Осталось'
LangString total_time ${LANG_ENGLISH} 'Total Time'
LangString total_time ${LANG_RUSSIAN} 'Прошло'
LangString dir_not_exist ${LANG_ENGLISH} 'Directory does not exist'
LangString dir_not_exist ${LANG_RUSSIAN} 'Директория не существует'
LangString not_in_folder ${LANG_ENGLISH} 'AkelUpdater should be in AkelFiles folder.'
LangString not_in_folder ${LANG_RUSSIAN} 'AkelUpdater должен находиться в папке AkelFiles.'
LangString download_error ${LANG_ENGLISH} 'Download error'
LangString download_error ${LANG_RUSSIAN} 'Ошибка скачивания'
LangString open_file_error ${LANG_ENGLISH} 'File open error.'
LangString open_file_error ${LANG_RUSSIAN} 'Ошибка открытия файла.'
LangString nothing_selected ${LANG_ENGLISH} 'Nothing is selected.'
LangString nothing_selected ${LANG_RUSSIAN} 'Ничего не выбрано.'
LangString done ${LANG_ENGLISH} 'Done'
LangString done ${LANG_RUSSIAN} 'Готово'
LangString error ${LANG_ENGLISH} 'Error'
LangString error ${LANG_RUSSIAN} 'Ошибка'
LangString close ${LANG_ENGLISH} '&Close'
LangString close ${LANG_RUSSIAN} '&Закрыть'
LangString completed ${LANG_ENGLISH} 'Completed'
LangString completed ${LANG_RUSSIAN} 'Завершено'

icon "Source\Resources\AkelUpdater.ico"
Caption "${PRODUCT_NAME}"
SubCaption 4 " "
BrandingText "${PRODUCT_NAME} ${PRODUCT_VERSION}"
MiscButtonText " " "" "" "$(close)"
CompletedText "$(completed)"
ShowInstDetails show
XPStyle on

VIAddVersionKey FileDescription "AkelPad text editor updater"
VIAddVersionKey LegalCopyright "© 2009 Shengalts Aleksander aka Instructor"
VIAddVersionKey ProductName "${PRODUCT_NAME}"
VIAddVersionKey FileVersion "${PRODUCT_VERSION}"
VIAddVersionKey Comments ""
VIAddVersionKey LegalTrademarks ""
VIProductVersion ${PRODUCT_VERSION}.0.0

!addplugindir "."
!include "WordFunc.nsh"
!insertmacro WordFind
!insertmacro WordFind2X
!insertmacro WordReplace
!include "TextFunc.nsh"
!include "FileFunc.nsh"
!insertmacro GetParameters
!insertmacro GetOptions
!insertmacro GetParent
!insertmacro GetFileVersion
!insertmacro VersionCompare

Var PARAMETERS
Var PROXYPARAM
Var PROXYVALUE
Var LOGINPARAM
Var LOGINVALUE
Var PASSWORDPARAM
Var PASSWORDVALUE
Var SAVEDIR
Var AKELPADDIR
Var AKELFILESDIR
Var AKELPLUGSDIR
Var AKELPLUGIN
Var EXEVERSION
Var DLLCOUNT
Var ZIPMIRROR
Var ZIPLANG
Var ZIPXLANG
Var UNZIP
Var NOTEPAD

Function .onInit
	#Help message
	${GetParameters} $PARAMETERS
	ExpandEnvStrings $PARAMETERS $PARAMETERS
	StrCmp $PARAMETERS '/?' 0 CheckLocation
	MessageBox MB_OK \
	`|   Command line options:$\n\
	 |   $\n\
	 |   /PROXY=IP:PORT$\n\
	 |     Current proxy settings. IE settings will be used by default.$\n\
	 |$\n\
	 |   /AUTH=LOGIN:PASSWORD$\n\
	 |     Proxy login and password (http only).$\n\
	 |$\n\
	 |   /SAVEDIR=[path]$\n\
	 |     Save downloads to directory.$\n\
	 |$\n\
	 |   /LANG=[eng|rus]$\n\
	 |     Select language.$\n\
	 |$\n\
	 |   /UNZIP=[options]$\n\
	 |     nsUnzip options.$\n\
	 $\n\
	 Example:$\n\
	 AkelUpdater.exe /PROXY=192.168.0.1:3128 /SAVEDIR="%a\AkelFiles\Updates"`
	quit

	CheckLocation:
	StrCpy $AKELFILESDIR $EXEDIR
	StrCpy $AKELPLUGSDIR "$AKELFILESDIR\Plugs"
	${GetParent} $EXEDIR $AKELPADDIR
	IfFileExists "$AKELPLUGSDIR\*.*" 0 NotAkelFiles

	InitPluginsDir
	StrCpy $SAVEDIR $PLUGINSDIR
	StrCpy $ZIPLANG $(lng)
        StrCpy $PROXYPARAM /NUL
        StrCpy $PROXYVALUE /NUL
        StrCpy $LOGINPARAM /NUL
        StrCpy $LOGINVALUE /NUL
        StrCpy $PASSWORDPARAM /NUL
        StrCpy $PASSWORDVALUE /NUL

	${GetOptions} $PARAMETERS "/PROXY=" $0
	IfErrors auth
        StrCpy $PROXYPARAM /PROXY
	StrCpy $PROXYVALUE $0
	auth:
	${GetOptions} $PARAMETERS "/AUTH=" $0
	IfErrors savedir
	${WordFind} "$0" ":" "E+1{" $1
	IfErrors savedir
	${WordFind} "$0" ":" "E+1}" $2
	IfErrors savedir
        StrCpy $LOGINPARAM /USERNAME
	StrCpy $LOGINVALUE $1
        StrCpy $PASSWORDPARAM /PASSWORD
	StrCpy $PASSWORDVALUE $2
	savedir:
	${GetOptions} $PARAMETERS "/SAVEDIR=" $0
	IfErrors langopt
	${WordReplace} "$0" "%a" "$AKELPADDIR" "+" $0
	StrCpy $SAVEDIR $0
	langopt:
	${GetOptions} $PARAMETERS "/LANG=" $0
	IfErrors unzipopt
	StrCmp $0 eng 0 +2
	StrCpy $ZIPLANG eng
	StrCmp $0 rus 0 +2
	StrCpy $ZIPLANG rus
	unzipopt:
	${GetOptions} $PARAMETERS "/UNZIP=" $0
	IfErrors DownloadVersions
	StrCpy $UNZIP $0

	;Download "versions.lst"
	DownloadVersions:
;	File "/oname=$PLUGINSDIR\versions.lst" "versions.lst"
	inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
        $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
	/TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
	"http://akelpad.sourceforge.net/img/versions.lst" "$PLUGINSDIR\versions.lst" /END
	Pop $0
	StrCmp $0 "OK" 0 DownloadError

	;Fill stack with versions
	Call FillStack
	IfErrors FileOpenError

	;Show dialog (Result: $0="ExeVersion|DllCount", $1="Download mirror", $2="Language")
	AkelUpdater::List $ZIPLANG
	StrCpy $ZIPMIRROR $1
	StrCpy $ZIPLANG $2
	StrCmp $0 "0|0" NothingSelected
	${WordFind} "$0" "|" "E+1{" $EXEVERSION
	IfErrors Exit
	${WordFind} "$0" "|" "E+1}" $DLLCOUNT
	IfErrors Exit

	;Is AkelPad running?
	CheckWindow:
	FindWindow $0 "AkelPad4"
	IsWindow $0 +5
	FindWindow $0 "AkelPad3"
	IsWindow $0 +3
	FindWindow $0 "AkelPad2"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(InstallAlreadyRun)" IDRETRY CheckWindow
	quit

	;Download "AkelPad-x.x.x-bin-lng.zip"
	CreateDirectory $SAVEDIR
	IfFileExists "$SAVEDIR\*.*" 0 DirNotExist
	StrCmp $EXEVERSION 0 PlugsPack
;	File "/oname=$SAVEDIR\AkelPad-4.2.1-bin-eng.zip" "AkelPad-4.2.1-bin-eng.zip"
	inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
        $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
	/TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
	"http://$ZIPMIRROR.dl.sourceforge.net/sourceforge/akelpad/AkelPad-$EXEVERSION-bin-$ZIPLANG.zip" "$SAVEDIR\AkelPad-$EXEVERSION-bin-$ZIPLANG.zip" /end
	Pop $0
	StrCmp $0 "OK" 0 DownloadError

	;Download "PlugsPack.zip"
	PlugsPack:
	StrCmp $DLLCOUNT 0 End
;	File "/oname=$SAVEDIR\PlugsPack.zip" "PlugsPack.zip"
	inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
        $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
	/TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
	"http://akelpad.sourceforge.net/files/plugs/PlugsPack.zip" "$SAVEDIR\PlugsPack.zip" /end
	Pop $0
	StrCmp $0 "OK" 0 DownloadError
	goto End

	DirNotExist:
	MessageBox MB_OK|MB_ICONEXCLAMATION '$(dir_not_exist): "$SAVEDIR"'
	goto Exit

	NotAkelFiles:
	MessageBox MB_OK|MB_ICONEXCLAMATION '$(not_in_folder)'
	goto Exit

	DownloadError:
	MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
	goto Exit

	FileOpenError:
	MessageBox MB_OK|MB_ICONEXCLAMATION '$(open_file_error)'
	goto Exit

	NothingSelected:
	MessageBox MB_OK|MB_ICONEXCLAMATION '$(nothing_selected)'
	goto Exit

	Exit:
	Quit

	End:
FunctionEnd

Function FillStack
	FileOpen $0 "$PLUGINSDIR\versions.lst" r
	IfErrors 0 ReadLine
	return

	ReadLine:
	FileRead $0 $1
	IfErrors Close

	${WordFind2X} "$1" "$$" "Ver=" "E+1" $R0
	IfErrors ReadLine
	${WordFind2X} "$1" "'" "'" "E+1" $R1
	IfErrors ReadLine

	StrCpy $2 $R0 -1
	StrCmp $2 "AkelPad" 0 PluginVersion
	StrCpy $NOTEPAD 0
	${GetFileVersion} "$AKELPADDIR\AkelPad.exe" $R2
	IfErrors 0 IsActual
	StrCpy $NOTEPAD 1
	${GetFileVersion} "$AKELPADDIR\notepad.exe" $R2
	IfErrors Empty
	IsActual:
	StrCpy $2 $R0 '' -1
	StrCpy $3 $R2 1
	StrCmp $2 $3 0 ReadLine
	StrCpy $R0 $R0 -1
	StrCpy $R2 $R2 -2
	goto Compare

	PluginVersion:
	${GetFileVersion} "$AKELPLUGSDIR\$R0.dll" $R2
	StrCpy $R2 $R2 -4
	IfErrors Empty

	Compare:
	${VersionCompare} "$R1" "$R2" $R3
	StrCmp $R3 2 0 +2
	StrCpy $R3 0
	goto PushString

	Empty:
	StrCpy $R2 ""
	StrCpy $R3 0

	PushString:
	Push $R3
	Push $R2
	Push $R1
	Push $R0
	Goto ReadLine

	Close:
	FileClose $0
FunctionEnd

Section
	;Hide buttons
	GetDlgItem $0 $HWNDPARENT 2
	ShowWindow $0 0

	GetDlgItem $0 $HWNDPARENT 3
	AkelUpdater::Collapse $0

	;Extract "AkelPad-x.x.x-bin-lng.zip"
	StrCmp $EXEVERSION 0 GetExcludeLanguage
	nsUnzip::Extract "$SAVEDIR\AkelPad-$EXEVERSION-bin-$ZIPLANG.zip" "/d=$AKELPADDIR" /END
	Pop $0
	StrCmp $0 0 +3
	DetailPrint "$(error) ($0): AkelPad-$EXEVERSION-bin-$ZIPLANG.zip"
	goto GetExcludeLanguage
	DetailPrint "$(done): AkelPad-$EXEVERSION-bin-$ZIPLANG.zip"
	StrCmp $NOTEPAD 1 0 GetExcludeLanguage
	SetDetailsPrint listonly
	CopyFiles /SILENT "$AKELPADDIR\AkelPad.exe" "$AKELPADDIR\notepad.exe"
	Delete "$AKELPADDIR\AkelPad.exe"
	SetDetailsPrint both

	GetExcludeLanguage:
	StrCmp $ZIPLANG eng 0 +2
	StrCpy $ZIPXLANG rus
	StrCmp $ZIPLANG rus 0 +2
	StrCpy $ZIPXLANG eng

	;Get AkelUpdater::List items
	NextPlugin:
	Pop $AKELPLUGIN
	IfErrors End

	Push /END
	AkelUpdater::ParseAndPush "$UNZIP"
	nsUnzip::Extract "$SAVEDIR\PlugsPack.zip" "/d=$AKELFILESDIR" "/x=Docs\$AKELPLUGIN-$ZIPXLANG.txt" /C "Docs\$AKELPLUGIN*" "Plugs\$AKELPLUGIN*"
	Pop $0
	StrCmp $0 0 +3
	DetailPrint "$(error) ($0): $AKELPLUGIN"
	goto End
	DetailPrint "$(done): $AKELPLUGIN"
	goto NextPlugin

	End:
SectionEnd
