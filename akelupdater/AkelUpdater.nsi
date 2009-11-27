!define PRODUCT_NAME "AkelUpdater"
!define PRODUCT_VERSION "2.5"

Name "AkelUpdater"
OutFile "AkelUpdater.exe"
SetCompressor /SOLID lzma
CRCCheck off

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
LangString download_finished ${LANG_ENGLISH} 'Updates downloading finished.'
LangString download_finished ${LANG_RUSSIAN} 'Скачивание обновлений закончено.'
LangString done ${LANG_ENGLISH} 'Done'
LangString done ${LANG_RUSSIAN} 'Готово'
LangString error ${LANG_ENGLISH} 'Error'
LangString error ${LANG_RUSSIAN} 'Ошибка'
LangString close ${LANG_ENGLISH} '&Close'
LangString close ${LANG_RUSSIAN} '&Закрыть'
LangString completed ${LANG_ENGLISH} 'Completed'
LangString completed ${LANG_RUSSIAN} 'Завершено'

icon "AkelUpdater.ico"
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
!insertmacro GetFileName
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
Var AKELLANGSDIR
Var AKELPLUGSDIR
Var AKELPLUGIN
Var EXEVERSIONFULL
Var EXEVERSIONMAJOR
Var PLUGINCOUNT
Var LANGEXIST
Var ZIPMIRROR
Var ZIPLANG
Var DLONLY
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
	 |   /LANG=[eng|rus]$\n\
	 |     Select language.$\n\
	 |$\n\
	 |   /SAVEDIR=[path]$\n\
	 |     Save downloads to directory.$\n\
	 |$\n\
	 |   /DLONLY$\n\
	 |     Don't update, download only.$\n\
	 |$\n\
	 |   /UNZIP=[options]$\n\
	 |     nsUnzip options.$\n\
	 $\n\
	 Example:$\n\
	 AkelUpdater.exe /PROXY=192.168.0.1:3128 /SAVEDIR="%a\AkelFiles\Updates"`
	quit

	CheckLocation:
	StrCpy $AKELFILESDIR $EXEDIR
	StrCpy $AKELLANGSDIR "$AKELFILESDIR\Langs"
	StrCpy $AKELPLUGSDIR "$AKELFILESDIR\Plugs"
	${GetParent} $EXEDIR $AKELPADDIR
	${GetFileName} $EXEDIR $0
	StrCmp "$0" "AkelFiles" 0 NotAkelFiles

	InitPluginsDir
	StrCpy $SAVEDIR $PLUGINSDIR
	StrCpy $ZIPLANG $(lng)
	StrCpy $DLONLY 0
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
	IfErrors langopt
	${WordFind} "$0" ":" "E+1{" $1
	IfErrors langopt
	${WordFind} "$0" ":" "E+1}" $2
	IfErrors langopt
	StrCpy $LOGINPARAM /USERNAME
	StrCpy $LOGINVALUE $1
	StrCpy $PASSWORDPARAM /PASSWORD
	StrCpy $PASSWORDVALUE $2
	langopt:
	${GetOptions} $PARAMETERS "/LANG=" $0
	IfErrors savedir
	StrCmp $0 eng 0 +2
	StrCpy $ZIPLANG eng
	StrCmp $0 rus 0 +2
	StrCpy $ZIPLANG rus
	savedir:
	${GetOptions} $PARAMETERS "/SAVEDIR=" $0
	IfErrors dlonly
	${WordReplace} "$0" "%a" "$AKELPADDIR" "+" $0
	StrCpy $SAVEDIR $0
	dlonly:
	${GetOptions} $PARAMETERS "/DLONLY" $0
	IfErrors unzipopt
	StrCpy $DLONLY 1
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
	${WordFind} "$0" "|" "E+1{" $EXEVERSIONFULL
	IfErrors Exit
	${WordFind} "$0" "|" "E+1}" $PLUGINCOUNT
	IfErrors Exit
	StrCpy $EXEVERSIONMAJOR $EXEVERSIONFULL 1

	;Is AkelPad running?
	CheckWindow:
	StrCmp $DLONLY 1 DownloadProgram
	FindWindow $0 "AkelPad4"
	IsWindow $0 +5
	FindWindow $0 "AkelPad3"
	IsWindow $0 +3
	FindWindow $0 "AkelPad2"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(InstallAlreadyRun)" IDRETRY CheckWindow
	quit

	;Download "AkelPad-x.x.x-bin-lng.zip"
	DownloadProgram:
	CreateDirectory $SAVEDIR
	IfFileExists "$SAVEDIR\*.*" 0 DirNotExist
	StrCmp $EXEVERSIONFULL 0 DownloadPlugsPack
;	File "/oname=$SAVEDIR\AkelPad-4.2.1-bin-eng.zip" "AkelPad-4.2.1-bin-eng.zip"
	inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
	$PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
	/TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
	"http://$ZIPMIRROR.dl.sourceforge.net/project/akelpad/AkelPad%20$EXEVERSIONMAJOR/$EXEVERSIONFULL/AkelPad-$EXEVERSIONFULL-bin-$ZIPLANG.zip" "$SAVEDIR\AkelPad-$EXEVERSIONFULL-bin-$ZIPLANG.zip" /end
	Pop $0
	StrCmp $0 "OK" 0 DownloadError

	;Download "LangsPack.zip"
	IfFileExists "$AKELLANGSDIR\*.dll" 0 DownloadPlugsPack
	StrCpy $LANGEXIST "true"
;	File "/oname=$SAVEDIR\LangsPack.zip" "LangsPack.zip"
	inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
	$PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
	/TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
	"http://akelpad.sourceforge.net/files/langs/LangsPack.zip" "$SAVEDIR\LangsPack.zip" /end
	Pop $0
	StrCmp $0 "OK" 0 DownloadError

	;Download "PlugsPack.zip"
	DownloadPlugsPack:
	StrCmp $PLUGINCOUNT 0 DowloadOnlyCheck
;	File "/oname=$SAVEDIR\PlugsPack.zip" "PlugsPack.zip"
	inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
	$PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
	/TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
	"http://akelpad.sourceforge.net/files/plugs/PlugsPack.zip" "$SAVEDIR\PlugsPack.zip" /end
	Pop $0
	StrCmp $0 "OK" 0 DownloadError

	;Exit if update not required
	DowloadOnlyCheck:
	StrCmp $DLONLY 1 0 End
	MessageBox MB_OK|MB_ICONINFORMATION '$(download_finished)$\n"$SAVEDIR"'
	goto Exit

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
	${GetFileVersion} "$AKELPADDIR\notepad.exe" $R2
	IfErrors Empty
	StrCpy $NOTEPAD 1
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
	goto PushString

	Empty:
	StrCpy $R2 ""
	StrCpy $R3 3

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
	StrCmp $EXEVERSIONFULL 0 GetExcludeLanguage
	Push /END
	AkelUpdater::ParseAndPush "$UNZIP"
	nsUnzip::Extract "$SAVEDIR\AkelPad-$EXEVERSIONFULL-bin-$ZIPLANG.zip" "/d=$AKELPADDIR"
	Pop $0
	StrCmp $0 0 +3
	DetailPrint "$(error) ($0): AkelPad-$EXEVERSIONFULL-bin-$ZIPLANG.zip"
	goto GetExcludeLanguage
	DetailPrint "$(done): AkelPad-$EXEVERSIONFULL-bin-$ZIPLANG.zip"
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

	;Extract "LangsPack.zip"
	StrCmp $LANGEXIST "true" 0 NextPlugin
	Push /END
	AkelUpdater::ParseAndPush "$UNZIP"
	nsUnzip::Extract "$SAVEDIR\LangsPack.zip" "/d=$AKELFILESDIR" /e "Langs\*"
	Pop $0
	StrCmp $0 0 +3
	DetailPrint "$(error) ($0): LangsPack.zip"
	goto NextPlugin
	DetailPrint "$(done): LangsPack.zip"

	;Get AkelUpdater::List items
	NextPlugin:
	Pop $AKELPLUGIN
	IfErrors End

	Push /END
	AkelUpdater::ParseAndPush "$UNZIP"
	IfFileExists "$AKELFILESDIR\Docs\$AKELPLUGIN-$ZIPXLANG.txt" +2
	Push "/x=Docs\$AKELPLUGIN-$ZIPXLANG.txt"
	nsUnzip::Extract "$SAVEDIR\PlugsPack.zip" "/d=$AKELFILESDIR" /C "Docs\$AKELPLUGIN*" "Plugs\$AKELPLUGIN*"
	Pop $0
	StrCmp $0 0 +3
	DetailPrint "$(error) ($0): $AKELPLUGIN"
	goto NextPlugin
	DetailPrint "$(done): $AKELPLUGIN"
	goto NextPlugin

	End:
SectionEnd
