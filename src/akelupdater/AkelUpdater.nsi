!define PRODUCT_NAME "AkelUpdater"
!define PRODUCT_VERSION "6.8"

Name "AkelUpdater"
OutFile "AkelUpdater.exe"
SetCompressor /SOLID lzma
CRCCheck off
RequestExecutionLevel user

############  File info  ############
VIAddVersionKey FileDescription "AkelPad text editor updater"
VIAddVersionKey LegalCopyright "© 2018 Shengalts Aleksander aka Instructor"
VIAddVersionKey ProductName "${PRODUCT_NAME}"
VIAddVersionKey FileVersion "${PRODUCT_VERSION}"
VIAddVersionKey Comments ""
VIAddVersionKey LegalTrademarks ""
VIProductVersion ${PRODUCT_VERSION}.0.0

############  Functions  ############
!addplugindir "."
!include "WordFunc.nsh"
!insertmacro WordFind
!insertmacro WordFind2X
!insertmacro WordReplace
!include "FileFunc.nsh"
!insertmacro GetParameters
!insertmacro GetOptions
!insertmacro GetParent
!insertmacro GetFileName
!insertmacro GetBaseName
!insertmacro GetFileVersion
!insertmacro VersionCompare
!include "LogicLib.nsh"
!include "WinMessages.nsh"

############  MUI  ############
icon "AkelUpdater.ico"
ChangeUI all "Pages\default.exe"
Caption "${PRODUCT_NAME}"
SubCaption 4 " "
BrandingText "${PRODUCT_NAME} ${PRODUCT_VERSION}"
MiscButtonText " " "" "" "$(close)"
CompletedText "$(completed)"
ShowInstDetails show
XPStyle on

############  Defines  ############
!define LANG_ENGLISH  1033
!define LANG_RUSSIAN  1049

############  Strings  ############
LangString InstallAlreadyRun ${LANG_ENGLISH} 'To install updates close AkelPad and select Retry.$\n$\nIf you want abort installation, select Cancel.'
LangString InstallAlreadyRun ${LANG_RUSSIAN} 'Для установки обновлений закройте AkelPad и выберите Повтор.$\n$\nЕсли Вы хотите прервать установку, выберите Отмена.'
LangString lng ${LANG_ENGLISH} 'eng'
LangString lng ${LANG_RUSSIAN} 'rus'
LangString url ${LANG_ENGLISH} 'URL'
LangString url ${LANG_RUSSIAN} 'Ссылка'
LangString downloading ${LANG_ENGLISH} 'Downloading'
LangString downloading ${LANG_RUSSIAN} 'Скачивание'
LangString connecting ${LANG_ENGLISH} 'Connecting'
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
LangString download_finished ${LANG_ENGLISH} 'Updates downloading finished.'
LangString download_finished ${LANG_RUSSIAN} 'Скачивание обновлений закончено.'
LangString done ${LANG_ENGLISH} 'Done'
LangString done ${LANG_RUSSIAN} 'Готово'
LangString language ${LANG_RUSSIAN} 'языковой модуль'
LangString language ${LANG_ENGLISH} 'language module'
LangString plugin ${LANG_RUSSIAN} 'плагин'
LangString plugin ${LANG_ENGLISH} 'plugin'
LangString script ${LANG_RUSSIAN} 'скрипт'
LangString script ${LANG_ENGLISH} 'script'
LangString for_script ${LANG_RUSSIAN} 'для скрипта'
LangString for_script ${LANG_ENGLISH} 'for script'
LangString error ${LANG_ENGLISH} 'Error'
LangString error ${LANG_RUSSIAN} 'Ошибка'
LangString close ${LANG_ENGLISH} '&Close'
LangString close ${LANG_RUSSIAN} '&Закрыть'
LangString run ${LANG_ENGLISH} 'Run AkelPad'
LangString run ${LANG_RUSSIAN} 'Запустить AkelPad'
LangString completed ${LANG_ENGLISH} 'Completed'
LangString completed ${LANG_RUSSIAN} 'Завершено'

############  Variables  ############
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
Var AKELSCRIPTSDIR
Var LISTITEM
Var ITEMTYPE
Var ITEMPACK
Var ITEM64
Var ITEMNAME
Var EXEBIT
Var EXEVERSIONFULL
Var EXEVERSIONMAJOR
Var EXEVERSIONFULL64
Var EXEVERSIONMAJOR64
Var DL_EXEBIT
Var DL_PATH64
Var DL_EXEVERSIONFULL
Var DL_EXEVERSIONMAJOR
Var DL_PLUGINCOUNT
Var BITSUFFIXMINUS
Var BITSUFFIXSLASH
Var PLUGINCOUNT
Var PLUGINCOUNT64
Var SCRIPTCOUNT
Var PLUGINCOPY
Var PLUGINCOPIES
Var SCRIPTSPACK
Var EXTENSION
Var WORDCOUNT
Var LANGEXIST
Var ZIPMIRROR
Var ZIPLANG
Var ONTOP
Var DLONLY
Var AUTO
Var NORUN
Var NOCOPIES
Var DLSCRIPTSPROC
Var ZIPXLANG
Var UNZIP
Var NOTEPAD
Var LASTEXTRACTERROR
Var DEBUG

Function .onInit
  #Help message
  ${GetParameters} $PARAMETERS
  ExpandEnvStrings $PARAMETERS $PARAMETERS
  ${If} $PARAMETERS == '/?'
    MessageBox MB_OK \
    `|   Command line options:$\n\
     |   $\n\
     |   /PROXY=IP:PORT$\n\
     |     Current proxy settings. IE settings will be used by default.$\n\
     |$\n\
     |   /AUTH=LOGIN:PASSWORD$\n\
     |     Proxy login and password (http only).$\n\
     |$\n\
     |   /NOPROXY$\n\
     |     Disables proxy settings for this connection (if any).$\n\
     |$\n\
     |   /LANG=[eng|rus]$\n\
     |     Select language.$\n\
     |$\n\
     |   /ONTOP$\n\
     |     Set dialog always on top.$\n\
     |$\n\
     |   /BIT=[32|64]$\n\
     |     Update to 32-bit or to 64-bit version. If not specified it will be autodetected.$\n\
     |$\n\
     |   /SAVEDIR=[path]$\n\
     |     Save downloads to directory.$\n\
     |$\n\
     |   /DLONLY$\n\
     |     Don't update, download only.$\n\
     |$\n\
     |   /AUTO$\n\
     |     Automatically choose Update if possible or Cancel/Close otherwise.$\n\
     |$\n\
     |   /NORUN$\n\
     |     Turn off "Run AkelPad" checkbox.$\n\
     |$\n\
     |   /NOCOPIES$\n\
     |     Don't load DLLs to find original plugin name.$\n\
     |$\n\
     |   /UNZIP=[options]$\n\
     |     nsUnzip options.$\n\
     $\n\
     Example:$\n\
     AkelUpdater.exe /PROXY=192.168.0.1:3128 /SAVEDIR="%a\AkelFiles\Updates"`
    quit
  ${EndIf}

  StrCpy $AKELFILESDIR $EXEDIR
  StrCpy $AKELLANGSDIR "$AKELFILESDIR\Langs"
  StrCpy $AKELSCRIPTSDIR "$AKELFILESDIR\Plugs\Scripts"
  ${GetParent} $EXEDIR $AKELPADDIR
  ${GetFileName} $EXEDIR $0
  ${If} $0 != "AkelFiles"
    MessageBox MB_OK|MB_ICONEXCLAMATION '$(not_in_folder)'
    Quit
  ${EndIf}

  InitPluginsDir
  ;CopyFiles /SILENT "c:\a\VC\akelpad_4\AkelUpdater\Source\Debug\AkelUpdater.dll" "$PLUGINSDIR"
  StrCpy $DEBUG 0
  ${If} $DEBUG == 1
    StrCpy $SAVEDIR $AKELPADDIR
  ${Else}
    StrCpy $SAVEDIR $PLUGINSDIR
  ${EndIf}
  StrCpy $EXEBIT 0
  StrCpy $ZIPLANG $(lng)
  StrCpy $ONTOP 0
  StrCpy $DLONLY 0
  StrCpy $AUTO 0
  StrCpy $NORUN 0
  StrCpy $NOCOPIES 0
  StrCpy $PROXYPARAM /NUL
  StrCpy $PROXYVALUE /NUL
  StrCpy $LOGINPARAM /NUL
  StrCpy $LOGINVALUE /NUL
  StrCpy $PASSWORDPARAM /NUL
  StrCpy $PASSWORDVALUE /NUL

  ${GetOptions} $PARAMETERS "/PROXY=" $0
  ${IfNot} ${Errors}
    StrCpy $PROXYPARAM /PROXY
    StrCpy $PROXYVALUE $0
  ${EndIf}

  ${GetOptions} $PARAMETERS "/AUTH=" $0
  ${IfNot} ${Errors}
    ${WordFind} "$0" ":" "E+1{" $1
    ${IfNot} ${Errors}
      ${WordFind} "$0" ":" "E+1}" $2
      ${IfNot} ${Errors}
        StrCpy $LOGINPARAM /USERNAME
        StrCpy $LOGINVALUE $1
        StrCpy $PASSWORDPARAM /PASSWORD
        StrCpy $PASSWORDVALUE $2
      ${EndIf}
    ${EndIf}
  ${EndIf}

  ${GetOptions} $PARAMETERS "/NOPROXY" $0
  ${IfNot} ${Errors}
    StrCpy $PROXYPARAM /NOPROXY
    StrCpy $PROXYVALUE /NUL
  ${EndIf}

  ${GetOptions} $PARAMETERS "/BIT=" $0
  ${IfNot} ${Errors}
    ${If} $0 == 32
      StrCpy $EXEBIT 32
    ${ElseIf} $0 == 64
      StrCpy $EXEBIT 64
    ${EndIf}
  ${EndIf}

  ${GetOptions} $PARAMETERS "/LANG=" $0
  ${IfNot} ${Errors}
    ${If} $0 == eng
      StrCpy $ZIPLANG eng
    ${ElseIf} $0 == rus
      StrCpy $ZIPLANG rus
    ${EndIf}
  ${EndIf}

  ${GetOptions} $PARAMETERS "/SAVEDIR=" $0
  ${IfNot} ${Errors}
    ${WordReplace} "$0" "%a" "$AKELPADDIR" "+" $0
    StrCpy $SAVEDIR $0
  ${EndIf}

  ${GetOptions} $PARAMETERS "/ONTOP" $0
  ${IfNot} ${Errors}
    StrCpy $ONTOP 1
  ${EndIf}

  ${GetOptions} $PARAMETERS "/DLONLY" $0
  ${IfNot} ${Errors}
    StrCpy $DLONLY 1
  ${EndIf}

  ${GetOptions} $PARAMETERS "/AUTO" $0
  ${IfNot} ${Errors}
    StrCpy $AUTO 1
  ${EndIf}

  ${GetOptions} $PARAMETERS "/NORUN" $0
  ${IfNot} ${Errors}
    StrCpy $NORUN 1
  ${EndIf}

  ${GetOptions} $PARAMETERS "/NOCOPIES" $0
  ${IfNot} ${Errors}
    StrCpy $NOCOPIES 1
  ${EndIf}

  ${GetOptions} $PARAMETERS "/UNZIP=" $0
  ${IfNot} ${Errors}
    StrCpy $UNZIP $0
  ${EndIf}

  ;Download "versions.lst"
  ${If} $DEBUG == 1
    CopyFiles /SILENT "$AKELPADDIR\versions.lst" "$PLUGINSDIR"
    CopyFiles /SILENT "$AKELPADDIR\*.ini" "$PLUGINSDIR"
  ${Else}
    inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
               $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
               /TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
               "http://akelpad.sourceforge.net/img/versions.lst" "$PLUGINSDIR\versions.lst" /END
    Pop $0
    ${If} $0 != "OK"
      MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
      Quit
    ${EndIf}
  ${EndIf}

  ;Extract helper
  File "/oname=$PLUGINSDIR\AkelUpdaterHelp.exe" "AkelUpdaterHelp.exe"

  ;Initialize AkelUpdater.dll variables
  AkelUpdater::Init /NOUNLOAD
  Pop $0
  ${If} $EXEBIT == "0"
    StrCpy $EXEBIT $0
  ${EndIf}

  ;When user press "Scripts" call this address
  GetFunctionAddress $DLSCRIPTSPROC DownloadScriptsProc

  ;Show dialog (Result: $0="ExeVersion|ExeVersion64|DllCount|DllCount64|ScriptCount", $1="Download mirror", $2="Language")
  AkelUpdater::List ${PRODUCT_VERSION} $ZIPLANG $ONTOP $EXEBIT $AUTO $NOCOPIES $PLUGINSDIR $DLSCRIPTSPROC "AkelUpdaterHelp.exe"
  Pop $R0
  ${If} $R0 == 0
    Quit
  ${EndIf}
  StrCpy $ZIPMIRROR $1
  StrCpy $ZIPLANG $2

  ${WordFind} "$0" "|" "E+1" $EXEVERSIONFULL
  ${If} ${Errors}
    Quit
  ${EndIf}
  StrCpy $EXEVERSIONMAJOR $EXEVERSIONFULL 1

  ${WordFind} "$0" "|" "E+2" $EXEVERSIONFULL64
  ${If} ${Errors}
    Quit
  ${EndIf}
  StrCpy $EXEVERSIONMAJOR64 $EXEVERSIONFULL64 1

  ${WordFind} "$0" "|" "E+3" $PLUGINCOUNT
  ${If} ${Errors}
    Quit
  ${EndIf}

  ${WordFind} "$0" "|" "E+4" $PLUGINCOUNT64
  ${If} ${Errors}
    Quit
  ${EndIf}

  ${WordFind} "$0" "|" "E+5" $SCRIPTCOUNT
  ${If} ${Errors}
    Quit
  ${EndIf}

  ;Is AkelPad running?
  CheckWindow:
  ${If} $DLONLY != 1
    FindWindow $0 "AkelPad4"
    IsWindow $0 +5
    FindWindow $0 "AkelPad3"
    IsWindow $0 +3
    FindWindow $0 "AkelPad2"
    IsWindow $0 0 +3
    MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(InstallAlreadyRun)" IDRETRY CheckWindow
    quit
  ${EndIf}

  ;Create save to directory
  CreateDirectory $SAVEDIR
  ${IfNot} ${FileExists} "$SAVEDIR\*.*"
    MessageBox MB_OK|MB_ICONEXCLAMATION '$(dir_not_exist): "$SAVEDIR"'
    Quit
  ${EndIf}

  ;Download
  StrCpy $DL_EXEBIT $EXEBIT
  StrCpy $DL_PATH64 ""
  StrCpy $DL_EXEVERSIONFULL $EXEVERSIONFULL
  StrCpy $DL_EXEVERSIONMAJOR $EXEVERSIONMAJOR
  StrCpy $DL_PLUGINCOUNT $PLUGINCOUNT
  Call DownloadZip

  StrCpy $DL_EXEBIT "64"
  StrCpy $DL_PATH64 "64"
  StrCpy $DL_EXEVERSIONFULL $EXEVERSIONFULL64
  StrCpy $DL_EXEVERSIONMAJOR $EXEVERSIONMAJOR64
  StrCpy $DL_PLUGINCOUNT $PLUGINCOUNT64
  Call DownloadZip

  ;Exit if update not required
  ${If} $DLONLY == 1
    MessageBox MB_OK|MB_ICONINFORMATION '$(download_finished)$\n"$SAVEDIR"'
    Quit
  ${EndIf}
FunctionEnd

Function DownloadScriptsProc
  Pop $SCRIPTSPACK
  Pop $EXTENSION

  ${If} $DEBUG == 1
    Return
  ${EndIf}

  ${If} $EXTENSION == "zip"
    ;Create save to directory
    CreateDirectory $SAVEDIR
    ${IfNot} ${FileExists} "$SAVEDIR\*.*"
      MessageBox MB_OK|MB_ICONEXCLAMATION '$(dir_not_exist): "$SAVEDIR"'
      Quit
    ${EndIf}
  ${EndIf}

  ;Download "KDJ.ini" (or "KDJ.zip"), "Infocatcher.ini" (or "Infocatcher.zip") and so on
  ${If} $SCRIPTSPACK != ""
    ;Add urls for inetc::get
    Push "/END"
    StrCpy $WORDCOUNT 1
    ${Do}
      ;KDJ|Infocatcher|Instructor|VladSh
      ${WordFind} "$SCRIPTSPACK" "|" "E-$WORDCOUNT" $1
      ${If} ${Errors}
        ${Break}
      ${EndIf}
      ${If} $EXTENSION == "zip"
        Push "$SAVEDIR\$1.$EXTENSION"
      ${Else}
        Push "$PLUGINSDIR\$1.$EXTENSION"
      ${EndIf}
      Push "http://akelpad.sourceforge.net/files/plugs/Scripts/$1.$EXTENSION"
      IntOp $WORDCOUNT $WORDCOUNT + 1
    ${Loop}
    inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
                       $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
                       /TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)"
    Pop $0
    ${If} $0 != "OK"
      MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
      Quit
    ${EndIf}
  ${EndIf}
FunctionEnd

Function DownloadZip
  ${If} $DL_EXEBIT == "64"
    StrCpy $BITSUFFIXMINUS "-x64"
    StrCpy $BITSUFFIXSLASH "/x64"
  ${Else}
    StrCpy $BITSUFFIXMINUS ""
    StrCpy $BITSUFFIXSLASH ""
  ${EndIf}

  ;Download "AkelPad-x.x.x-bin-lng.zip"
  ${If} $DL_EXEVERSIONFULL != 0
    ${If} $DEBUG != 1
      inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
                 $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
                 /TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
                 "http://$ZIPMIRROR.dl.sourceforge.net/project/akelpad/AkelPad%20$DL_EXEVERSIONMAJOR/$DL_EXEVERSIONFULL$BITSUFFIXSLASH/AkelPad-$DL_EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip" "$SAVEDIR\AkelPad-$DL_EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip" /end
      Pop $0
      ${If} $0 != "OK"
        MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
        Quit
      ${EndIf}

      ;Download "LangsPack.zip"
      ${If} ${FileExists} "$AKELLANGSDIR$DL_PATH64\*.dll"
        StrCpy $LANGEXIST "true"
        inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
                   $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
                   /TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
                   "http://akelpad.sourceforge.net/files/langs/LangsPack$BITSUFFIXMINUS.zip" "$SAVEDIR\LangsPack$BITSUFFIXMINUS.zip" /end
        Pop $0
        ${If} $0 != "OK"
          MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
          Quit
        ${EndIf}
      ${EndIf}
    ${EndIf}
  ${EndIf}

  ;Download "PlugsPack.zip"
  ${If} $DL_PLUGINCOUNT != 0
    ${If} $DEBUG != 1
      inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
                 $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
                 /TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
                 "http://akelpad.sourceforge.net/files/plugs/PlugsPack$BITSUFFIXMINUS.zip" "$SAVEDIR\PlugsPack$BITSUFFIXMINUS.zip" /end
      Pop $0
      ${If} $0 != "OK"
        MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
        Quit
      ${EndIf}
    ${EndIf}
  ${EndIf}
FunctionEnd

Section
  StrCpy $LASTEXTRACTERROR 0

  ;Hide button
  GetDlgItem $0 $HWNDPARENT 3
  AkelUpdater::Collapse $0

  ;Checkbox
  GetDlgItem $0 $HWNDPARENT 2001
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(run)'
  ${If} $NORUN == 0
    SendMessage $0 ${BM_SETCHECK} 1 0
  ${EndIf}

  ;Detect notepad replacement
  ${If} ${FileExists} "$AKELPADDIR\notepad.exe"
  ${AndIfNot} ${FileExists} "$AKELPADDIR\AkelPad.exe"
    StrCpy $NOTEPAD "1"
  ${EndIf}

  ;Extract
  ${If} $EXEVERSIONFULL != 0
  ${OrIf} $PLUGINCOUNT != 0
  ${OrIf} $SCRIPTCOUNT != 0
    StrCpy $DL_EXEBIT $EXEBIT
    StrCpy $DL_EXEVERSIONFULL $EXEVERSIONFULL
    StrCpy $DL_PATH64 ""
    Call ExtractZip
  ${EndIf}

  ${If} $EXEVERSIONFULL64 != 0
  ${OrIf} $PLUGINCOUNT64 != 0
    StrCpy $DL_EXEBIT "64"
    StrCpy $DL_EXEVERSIONFULL $EXEVERSIONFULL64
    StrCpy $DL_PATH64 "64"
    Call ExtractZip
  ${EndIf}

  ${If} $AUTO == 1
    Call .onInstSuccess
    Quit
  ${EndIf}
SectionEnd

Function ExtractZip
  ${If} $DL_EXEBIT == "64"
    StrCpy $BITSUFFIXMINUS "-x64"
    StrCpy $BITSUFFIXSLASH "/x64"
  ${Else}
    StrCpy $BITSUFFIXMINUS ""
    StrCpy $BITSUFFIXSLASH ""
  ${EndIf}

  ;Extract "AkelPad-x.x.x-bin-lng.zip"
  ${While} $DL_EXEVERSIONFULL != 0
    Push /END
    AkelUpdater::ParseAndPush "$UNZIP"
    ${If} $DL_PATH64 == "64"
      nsUnzip::Extract "$SAVEDIR\AkelPad-$DL_EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip" /C "AkelPad.exe" "/d=$PLUGINSDIR\64"
      Pop $0

      SetDetailsPrint textonly
      CopyFiles /SILENT "$PLUGINSDIR\64\AkelPad.exe" "$AKELPADDIR\AkelPad64.exe"
      SetDetailsPrint both
    ${Else}
      nsUnzip::Extract "$SAVEDIR\AkelPad-$DL_EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip" "/d=$AKELPADDIR"
      Pop $0
    ${EndIf}
    ${If} $0 != 0
      DetailPrint "$(error) ($0): AkelPad-$DL_EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip"
      StrCpy $LASTEXTRACTERROR $0
      ${Break}
    ${EndIf}
    DetailPrint "$(done): AkelPad-$DL_EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip"

    ${If} $NOTEPAD == 1
    ${AndIf} $DL_PATH64 == ""
      SetDetailsPrint textonly
      CopyFiles /SILENT "$AKELPADDIR\AkelPad.exe" "$AKELPADDIR\notepad.exe"
      Delete "$AKELPADDIR\AkelPad.exe"
      SetDetailsPrint both
    ${EndIf}

    ;Extract "LangsPack.zip"
    ${If} $LANGEXIST == "true"
      GetFunctionAddress $0 ServiceCallback
      Push /END
      AkelUpdater::ParseAndPush "$UNZIP"
      nsUnzip::Extract "$SAVEDIR\LangsPack$BITSUFFIXMINUS.zip" "/d=$AKELFILESDIR\Langs$DL_PATH64" /callS=$0 /e /j "Langs\*"
      Pop $0
      ${If} $0 != 0
        DetailPrint "$(error) ($0): LangsPack$BITSUFFIXMINUS.zip"
        StrCpy $LASTEXTRACTERROR $0
      ${EndIf}
    ${EndIf}

    ${Break}
  ${EndWhile}

  ;Get AkelUpdater::List items
  ${If} $ZIPLANG == eng
    StrCpy $ZIPXLANG rus
  ${ElseIf} $ZIPLANG == rus
    StrCpy $ZIPXLANG eng
  ${EndIf}

  ${Do}
    Pop $LISTITEM
    ${If} ${Errors}
      ${Break}
    ${EndIf}

    ${WordFind} "$LISTITEM" "|" "+1" $ITEMTYPE
    ${WordFind} "$LISTITEM" "|" "+2" $ITEMPACK
    ${WordFind} "$LISTITEM" "|" "+3" $ITEM64
    ${WordFind} "$LISTITEM" "|" "+4" $ITEMNAME
    ${WordFind} "$LISTITEM" "|" "E+4}" $PLUGINCOPIES
    ${If} ${Errors}
      StrCpy $PLUGINCOPIES ''
    ${EndIf}

    ;We pop all items from stack in first ExtractZip call
    ${If} $ITEMPACK == "PlugsPack64"
      ${If} $ITEM64 == "1"
        StrCpy $DL_PATH64 "64"
      ${Else}
        StrCpy $DL_PATH64 ""
      ${EndIf}
      StrCpy $BITSUFFIXMINUS "-x64"
      StrCpy $BITSUFFIXSLASH "/x64"
    ${Else}
      StrCpy $DL_PATH64 ""
      StrCpy $BITSUFFIXMINUS ""
      StrCpy $BITSUFFIXSLASH ""
    ${EndIf}

    ${If} $ITEMTYPE == 3
    ${OrIf} $ITEMTYPE == 4
      ;Update script
      ${If} $ITEMTYPE == 3
        StrCpy $R0 $(script)
      ${Elseif} $ITEMTYPE == 4
        StrCpy $R0 $(for_script)
      ${EndIf}
      Push /END
      AkelUpdater::ParseAndPush "$UNZIP"
      nsUnzip::Extract "$SAVEDIR\$ITEMPACK.zip" "/d=$AKELSCRIPTSDIR" /C "$ITEMNAME"
      Pop $0
      ${If} $0 != 0
        DetailPrint "$(error) ($0): $ITEMNAME $R0"
        StrCpy $LASTEXTRACTERROR $0
        ${Continue}
      ${Else}
        DetailPrint "$(done): $ITEMNAME $R0"
      ${EndIf}
    ${ElseIf} $ITEMTYPE == 2
      ;Update plugin
      Push /END
      AkelUpdater::ParseAndPush "$UNZIP"
      IfFileExists "$AKELFILESDIR\Docs\$ITEMNAME-$ZIPXLANG.txt" +2
      Push "/x=Docs\$ITEMNAME-$ZIPXLANG.txt"

      ${If} $DL_PATH64 == "64"
        nsUnzip::Extract "$SAVEDIR\PlugsPack$BITSUFFIXMINUS.zip" "/d=$AKELFILESDIR\Plugs$DL_PATH64" /C /j "Plugs\$ITEMNAME.dll"
        Pop $0
      ${Else}
        nsUnzip::Extract "$SAVEDIR\PlugsPack$BITSUFFIXMINUS.zip" "/d=$AKELFILESDIR" /C "Docs\$ITEMNAME*" "Plugs\$ITEMNAME*"
        Pop $0
      ${EndIf}

      ${If} $0 != 0
        DetailPrint "$(error) ($0): $ITEMNAME$DL_PATH64 $(plugin)"
        StrCpy $LASTEXTRACTERROR $0
        ${Continue}
      ${Else}
        DetailPrint "$(done): $ITEMNAME$DL_PATH64 $(plugin)"
      ${EndIf}

      ;Update plugin copies
      ${Do}
        ${If} $PLUGINCOPIES == ''
          ${Break}
        ${EndIf}
        ${WordFind} "$PLUGINCOPIES" "|" "+1{" $PLUGINCOPY
        ${WordFind} "$PLUGINCOPIES" "|" "E+1}" $PLUGINCOPIES
        ${If} ${Errors}
          StrCpy $PLUGINCOPIES ''
        ${EndIf}
        SetDetailsPrint textonly
        CopyFiles /SILENT "$AKELFILESDIR\Plugs$DL_PATH64\$ITEMNAME.dll" "$AKELFILESDIR\Plugs$DL_PATH64\$PLUGINCOPY.dll"
        SetDetailsPrint both
        DetailPrint "$(done): $PLUGINCOPY ($ITEMNAME$DL_PATH64 $(plugin))"
      ${Loop}
    ${EndIf}
  ${Loop}
FunctionEnd

Function .onInstSuccess
  GetDlgItem $0 $HWNDPARENT 2001
  SendMessage $0 ${BM_GETCHECK} 0 0 $1
  ${If} $1 == 1
    ${If} $NOTEPAD == 1
      Exec "$AKELPADDIR\notepad.exe"
    ${Else}
      Exec "$AKELPADDIR\AkelPad.exe"
    ${EndIf}
  ${EndIf}
  SetErrorLevel $LASTEXTRACTERROR
FunctionEnd

Function ServiceCallback
  Pop $0  ;Archive name
  Pop $1  ;Archive size
  Pop $2  ;Entry file name
  Pop $3  ;Entry uncompressed size
  Pop $4  ;Entry compressed size
  Pop $5  ;Entry CRC: ff00ff00

  ${GetBaseName} "$2" $R0
  DetailPrint "$(done): $R0$DL_PATH64 $(language)"

  #Use "SetErrors" for stop extracting
FunctionEnd
