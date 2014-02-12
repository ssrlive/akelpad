!define PRODUCT_NAME "AkelUpdater"
!define PRODUCT_VERSION "4.0"

Name "AkelUpdater"
OutFile "AkelUpdater.exe"
SetCompressor /SOLID lzma
CRCCheck off
RequestExecutionLevel user

############  File info  ############
VIAddVersionKey FileDescription "AkelPad text editor updater"
VIAddVersionKey LegalCopyright "© 2014 Shengalts Aleksander aka Instructor"
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
LangString open_file_error ${LANG_ENGLISH} 'File open error.'
LangString open_file_error ${LANG_RUSSIAN} 'Ошибка открытия файла.'
LangString nothing_selected ${LANG_ENGLISH} 'Nothing is selected.'
LangString nothing_selected ${LANG_RUSSIAN} 'Ничего не выбрано.'
LangString download_finished ${LANG_ENGLISH} 'Updates downloading finished.'
LangString download_finished ${LANG_RUSSIAN} 'Скачивание обновлений закончено.'
LangString done ${LANG_ENGLISH} 'Done'
LangString done ${LANG_RUSSIAN} 'Готово'
LangString language ${LANG_RUSSIAN} 'языковой модуль'
LangString language ${LANG_ENGLISH} 'language module'
LangString plugin ${LANG_RUSSIAN} 'плагин'
LangString plugin ${LANG_ENGLISH} 'plugin'
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
Var AKELPLUGSDIR
Var AKELPLUGIN
Var EXEVERSIONFULL
Var EXEVERSIONMAJOR
Var EXEBIT
Var BITSUFFIXMINUS
Var BITSUFFIXSLASH
Var PLUGINCOUNT
Var PLUGINCOPY
Var PLUGINCOPIES
Var NOSUPPORT64BIT
Var UPDATENAME
Var UPDATENEWVER
Var UPDATECURVER
Var UPDATECOMPARE
Var LANGEXIST
Var ZIPMIRROR
Var ZIPLANG
Var DLONLY
Var AUTO
Var NORUN
Var NOCOPIES
Var ZIPXLANG
Var UNZIP
Var NOTEPAD
Var FILEHANDLE
Var FILELINE
Var LASTEXTRACTERROR

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
  StrCpy $AKELPLUGSDIR "$AKELFILESDIR\Plugs"
  ${GetParent} $EXEDIR $AKELPADDIR
  ${GetFileName} $EXEDIR $0
  ${If} $0 != "AkelFiles"
    MessageBox MB_OK|MB_ICONEXCLAMATION '$(not_in_folder)'
    Quit
  ${EndIf}

  InitPluginsDir
  StrCpy $SAVEDIR $PLUGINSDIR
  StrCpy $EXEBIT 0
  StrCpy $ZIPLANG $(lng)
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
  ;File "/oname=$PLUGINSDIR\versions.lst" "versions.lst"
  inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
             $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
             /TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
             "http://akelpad.sourceforge.net/img/versions.lst" "$PLUGINSDIR\versions.lst" /END
  Pop $0
  ${If} $0 != "OK"
    MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
    Quit
  ${EndIf}

  ;Initialize AkelUpdater.dll variables
  AkelUpdater::Init /NOUNLOAD
  Pop $0
  ${If} $EXEBIT == "0"
    StrCpy $EXEBIT $0
  ${EndIf}
  ${If} $EXEBIT == "64"
    StrCpy $BITSUFFIXMINUS "-x64"
    StrCpy $BITSUFFIXSLASH "/x64"
  ${Else}
    StrCpy $BITSUFFIXMINUS ""
    StrCpy $BITSUFFIXSLASH ""
  ${EndIf}

  ;Extract helper
  ${If} $EXEBIT == "64"
  ${AndIf} $NOCOPIES == 0
    File "/oname=$PLUGINSDIR\AkelUpdaterHelp.exe" "AkelUpdaterHelp.exe"
  ${EndIf}

  ;Fill stack with versions
  Call FillStack
  ${If} ${Errors}
    MessageBox MB_OK|MB_ICONEXCLAMATION '$(open_file_error)'
    Quit
  ${EndIf}

  ;Show dialog (Result: $0="ExeVersion|DllCount", $1="Download mirror", $2="Language")
  AkelUpdater::List ${PRODUCT_VERSION} $ZIPLANG $EXEBIT $AUTO $NOCOPIES "$PLUGINSDIR\AkelUpdaterHelp.exe"
  StrCpy $ZIPMIRROR $1
  StrCpy $ZIPLANG $2

  ${If} $0 == "0|0"
    MessageBox MB_OK|MB_ICONEXCLAMATION '$(nothing_selected)'
    Quit
  ${EndIf}

  ${WordFind} "$0" "|" "E+1" $EXEVERSIONFULL
  ${If} ${Errors}
    Quit
  ${EndIf}
  StrCpy $EXEVERSIONMAJOR $EXEVERSIONFULL 1

  ${WordFind} "$0" "|" "E+2" $PLUGINCOUNT
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

  ;Download "AkelPad-x.x.x-bin-lng.zip"
  ${If} $EXEVERSIONFULL != 0
    ;File "/oname=$SAVEDIR\AkelPad-4.2.1-bin-eng.zip" "AkelPad-4.2.1-bin-eng.zip"
    inetc::get /CAPTION "${PRODUCT_NAME}" /POPUP "" \
               $PROXYPARAM "$PROXYVALUE" $LOGINPARAM "$LOGINVALUE" $PASSWORDPARAM "$PASSWORDVALUE" \
               /TRANSLATE "$(url)" "$(downloading)" "$(connecting)" "$(file_name)" "$(received)" "$(file_size)" "$(remaining_time)" "$(total_time)" \
               "http://$ZIPMIRROR.dl.sourceforge.net/project/akelpad/AkelPad%20$EXEVERSIONMAJOR/$EXEVERSIONFULL$BITSUFFIXSLASH/AkelPad-$EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip" "$SAVEDIR\AkelPad-$EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip" /end
    Pop $0
    ${If} $0 != "OK"
      MessageBox MB_OK|MB_ICONEXCLAMATION '$(download_error): $0'
      Quit
    ${EndIf}

    ;Download "LangsPack.zip"
    ${If} ${FileExists} "$AKELLANGSDIR\*.dll"
      StrCpy $LANGEXIST "true"
      ;File "/oname=$SAVEDIR\LangsPack.zip" "LangsPack.zip"
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

  ;Download "PlugsPack.zip"
  ${If} $PLUGINCOUNT != 0
    ;File "/oname=$SAVEDIR\PlugsPack.zip" "PlugsPack.zip"
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

  ;Exit if update not required
  ${If} $DLONLY == 1
    MessageBox MB_OK|MB_ICONINFORMATION '$(download_finished)$\n"$SAVEDIR"'
    Quit
  ${EndIf}
FunctionEnd

Function FillStack
  FileOpen $FILEHANDLE "$PLUGINSDIR\versions.lst" r
  ${If} ${Errors}
    return
  ${EndIf}

  ${Do}
    FileRead $FILEHANDLE $FILELINE
    ${If} ${Errors}
      ${Break}
    ${EndIf}

    ${If} $EXEBIT == 64
      ${If} $NOSUPPORT64BIT == ''
        ${WordFind} "$FILELINE" "$$NoSupport64Bit=" "E+1}" $NOSUPPORT64BIT
        ${If} ${Errors}
          StrCpy $NOSUPPORT64BIT ''
        ${Else}
          ${Continue}
        ${EndIf}
      ${EndIf}
    ${EndIf}

    ${WordFind2X} "$FILELINE" "$$" "Ver=" "E+1" $UPDATENAME
    ${If} ${Errors}
      ${Continue}
    ${EndIf}

    ${WordFind2X} "$FILELINE" "'" "'" "E+1" $UPDATENEWVER
    ${If} ${Errors}
      ${Continue}
    ${EndIf}

    ${If} $EXEBIT == 64
      ${WordFind} "$NOSUPPORT64BIT" "$UPDATENAME" "E+1{" $0
      ${IfNot} ${Errors}
        ${Continue}
      ${EndIf}
    ${EndIf}

    StrCpy $0 $UPDATENAME -1
    ${If} $0 == "AkelPad"
      StrCpy $NOTEPAD 0
      ${GetFileVersion} "$AKELPADDIR\AkelPad.exe" $UPDATECURVER
      ${If} ${Errors}
        ${GetFileVersion} "$AKELPADDIR\notepad.exe" $UPDATECURVER
        IfErrors Empty
        StrCpy $NOTEPAD 1
      ${EndIf}

      StrCpy $0 $UPDATENAME '' -1
      StrCpy $1 $UPDATECURVER 1
      ${If} $0 == $1
        StrCpy $UPDATENAME $UPDATENAME -1
        StrCpy $UPDATECURVER $UPDATECURVER -2
      ${Else}
        ${Continue}
      ${EndIf}
    ${Else}
      ${GetFileVersion} "$AKELPLUGSDIR\$UPDATENAME.dll" $UPDATECURVER
      StrCpy $UPDATECURVER $UPDATECURVER -4
      IfErrors Empty
    ${EndIf}

    ${VersionCompare} "$UPDATENEWVER" "$UPDATECURVER" $UPDATECOMPARE
    ;0  Versions are equal
    ;1  Version1 is newer
    ;2  Version2 is newer
    goto PushString

    Empty:
    StrCpy $UPDATECURVER ""
    StrCpy $UPDATECOMPARE 3

    PushString:
    Push $UPDATECOMPARE
    Push $UPDATECURVER
    Push $UPDATENEWVER
    Push $UPDATENAME
  ${Loop}

  FileClose $FILEHANDLE
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

  ;Extract "AkelPad-x.x.x-bin-lng.zip"
  ${While} $EXEVERSIONFULL != 0
    Push /END
    AkelUpdater::ParseAndPush "$UNZIP"
    nsUnzip::Extract "$SAVEDIR\AkelPad-$EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip" "/d=$AKELPADDIR"
    Pop $0
    ${If} $0 != 0
      DetailPrint "$(error) ($0): AkelPad-$EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip"
      StrCpy $LASTEXTRACTERROR $0
      ${Break}
    ${EndIf}
    DetailPrint "$(done): AkelPad-$EXEVERSIONFULL$BITSUFFIXMINUS-bin-$ZIPLANG.zip"

    ${If} $NOTEPAD == 1
      SetDetailsPrint textonly
      CopyFiles /SILENT "$AKELPADDIR\AkelPad.exe" "$AKELPADDIR\notepad.exe"
      Delete "$AKELPADDIR\AkelPad.exe"
      SetDetailsPrint both
    ${EndIf}

    ${Break}
  ${EndWhile}

  ${If} $ZIPLANG == eng
    StrCpy $ZIPXLANG rus
  ${ElseIf} $ZIPLANG == rus
    StrCpy $ZIPXLANG eng
  ${EndIf}

  ;Extract "LangsPack.zip"
  ${If} $LANGEXIST == "true"
    GetFunctionAddress $0 ServiceCallback
    Push /END
    AkelUpdater::ParseAndPush "$UNZIP"
    nsUnzip::Extract "$SAVEDIR\LangsPack$BITSUFFIXMINUS.zip" "/d=$AKELFILESDIR" /callS=$0 /e "Langs\*"
    Pop $0
    ${If} $0 != 0
      DetailPrint "$(error) ($0): LangsPack$BITSUFFIXMINUS.zip"
      StrCpy $LASTEXTRACTERROR $0
    ${EndIf}
  ${EndIf}

  ;Get AkelUpdater::List items
  ${Do}
    Pop $AKELPLUGIN
    ${If} ${Errors}
      ${Break}
    ${EndIf}

    ${WordFind} "$AKELPLUGIN" "|" "E+1}" $PLUGINCOPIES
    ${If} ${Errors}
      StrCpy $PLUGINCOPIES ''
    ${EndIf}
    ${WordFind} "$AKELPLUGIN" "|" "+1{" $AKELPLUGIN

    ;Update plugin
    Push /END
    AkelUpdater::ParseAndPush "$UNZIP"
    IfFileExists "$AKELFILESDIR\Docs\$AKELPLUGIN-$ZIPXLANG.txt" +2
    Push "/x=Docs\$AKELPLUGIN-$ZIPXLANG.txt"
    nsUnzip::Extract "$SAVEDIR\PlugsPack$BITSUFFIXMINUS.zip" "/d=$AKELFILESDIR" /C "Docs\$AKELPLUGIN*" "Plugs\$AKELPLUGIN*"
    Pop $0
    ${If} $0 != 0
      DetailPrint "$(error) ($0): $AKELPLUGIN $(plugin)"
      StrCpy $LASTEXTRACTERROR $0
      ${Continue}
    ${Else}
      DetailPrint "$(done): $AKELPLUGIN $(plugin)"
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
      CopyFiles /SILENT "$AKELPLUGSDIR\$AKELPLUGIN.dll" "$AKELPLUGSDIR\$PLUGINCOPY.dll"
      SetDetailsPrint both
      DetailPrint "$(done): $PLUGINCOPY ($AKELPLUGIN $(plugin))"
    ${Loop}
  ${Loop}

  ${If} $AUTO == 1
    Call .onInstSuccess
    Quit
  ${EndIf}
SectionEnd

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
  DetailPrint "$(done): $R0 $(language)"

  #Use "SetErrors" for stop extracting
FunctionEnd
