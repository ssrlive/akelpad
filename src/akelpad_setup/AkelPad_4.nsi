!ifndef PRODUCT_NAME
  !define PRODUCT_NAME "AkelPad"
!endif
!ifndef PRODUCT_VERSION
  !define PRODUCT_VERSION "4.9.8"
!endif
!ifndef PRODUCT_BIT
  !define PRODUCT_BIT "32"
!endif
!ifndef PRODUCT_DIR
  !define PRODUCT_DIR "Files"
!endif
!ifndef PRODUCT_NOTEPAD
  !define PRODUCT_NOTEPAD "notepad"
!endif

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
!include "LogicLib.nsh"
!include "x64.nsh"

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
!define LANG_UKRAINIAN            1058
!define LANG_BELORUSSIAN          1059
!define LANG_GERMAN               1043
!define LANG_SPANISH              1034
!define LANG_FRENCH               1036
!define LANG_JAPANESE             1041
!define LANG_BRAZILIAN            1046
!define LANG_ITALIAN              1040
!define LANG_KOREAN               1042
!define LANG_CHINESE_TRADITIONAL  1028
!define LANG_CHINESE_SIMPLIFIED   2052
!define LANG_TATAR                1092
!define LANG_SLOVAK               1051
!define LANG_ROMANIAN             1048
!define LANG_DUTCH                1043
!define LANG_FINNISH              1035
!define LANG_POLISH               1045
!define LANG_CZECH                1029
!define LANG_HUNGARIAN            1038

!define INSTTYPE_STANDARD   1
!define INSTTYPE_TOTALCMD   2
!define INSTTYPE_NOTEPAD    3
!define INSTTYPE_NOTEPADALT 4

!define SHORTCUT_QUICKLAUNCH  0x1
!define SHORTCUT_DESKTOP      0x2
!define SHORTCUT_STARTMENU    0x4

############  Variables  ############
Var PARAMETERS
Var INI
Var HWND
Var REDCTL
Var ALTCTL
Var INSTTYPE
Var SHORTCUT
Var COMSPEC
Var USERNAME
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
LangString No64bit ${LANG_ENGLISH} '64-bit OS required'
LangString No64bit ${LANG_RUSSIAN} 'Требуется 64-битная ОС'
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
LangString TypeNotepadAlt ${LANG_ENGLISH} 'Alternative - via registry'
LangString TypeNotepadAlt ${LANG_RUSSIAN} 'Альтернативная - через реестр'
LangString TypeStandardText ${LANG_ENGLISH} 'Program will be installed to the specified directory.'
LangString TypeStandardText ${LANG_RUSSIAN} 'Программа будет установлена в указанную директорию.'
LangString TypeTotalcmdText ${LANG_ENGLISH} 'Program will be installed as external editor for Total Commander file manager.'
LangString TypeTotalcmdText ${LANG_RUSSIAN} 'Программа будет установлена как внешний редактор для файлового менеджера Total Commander.'
LangString TypeNotepadText ${LANG_ENGLISH} 'Windows notepad will be replaced with program. Copy of the notepad will be restored after program uninstall.'
LangString TypeNotepadText ${LANG_RUSSIAN} 'Блокнот Windows будет заменен программой. Копия блокнота будет восстановлена после удаления программы.'
LangString TypeNotepadTextAlt ${LANG_ENGLISH} 'Windows notepad calls will be redirected to AkelPad.'
LangString TypeNotepadTextAlt ${LANG_RUSSIAN} `Вызовы блокнота Windows будут перенаправлены на AkelPad.`
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
  #GetUserDefaultLangID() for $LANGUAGE
  ;System::Call "kernel32::GetUserDefaultLangID() i .a"

  !if ${PRODUCT_BIT} == "64"
    ${IfNot} ${RunningX64}
      MessageBox MB_OK|MB_ICONEXCLAMATION "$(No64bit)"
      quit
    ${EndIf}
    SetRegView 64
  !endif

  #Help message
  ${GetParameters} $PARAMETERS
  ExpandEnvStrings $PARAMETERS $PARAMETERS
  ${If} $PARAMETERS == '/?'
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
  ${EndIf}

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
  IfErrors +2
  StrCpy $INSTTYPE $0

  ${If} $INSTDIR == ''
    ${GetOptions} $PARAMETERS "/DIR=" $0
    IfErrors +2
    StrCpy $INSTDIR $0
  ${EndIf}

  ${GetOptions} $PARAMETERS "/SHORTCUT=" $0
  IfErrors +2
  StrCpy $SHORTCUT $0

  #Environment variables
  ReadEnvStr $COMSPEC COMSPEC
  ReadEnvStr $USERNAME USERNAME

  #Silent install
  IfSilent 0 custom
  ${If} $INSTDIR == ''
    Call GetInstallDirectory
  ${EndIf}
  Call SetInstallDirectory
  Call CreateShortcuts
  Return

  #Custom page
  custom:
  InitPluginsDir

  StrCpy $REDCTL 0
  StrCpy $ALTCTL 1

  GetTempFileName $INI $PLUGINSDIR
  File /oname=$INI "Pages\InstallType.ini"

  GetTempFileName $0 $PLUGINSDIR
  File /oname=$0 "Graphics\IconTypeStandard.ico"
  WriteINIStr "$INI" "Field 8" "Text" "$0"

  GetTempFileName $0 $PLUGINSDIR
  File /oname=$0 "Graphics\IconTypeTotalcmd.ico"
  WriteINIStr "$INI" "Field 9" "Text" "$0"

  GetTempFileName $0 $PLUGINSDIR
  File /oname=$0 "Graphics\IconTypeNotepad.ico"
  WriteINIStr "$INI" "Field 10" "Text" "$0"

  GetTempFileName $0 $PLUGINSDIR
  File /oname=$0 "Graphics\WizardWelcome.bmp"
  WriteINIStr "$INI" "Field 11" "Text" "$0"
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
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepadAlt)'
  ${If} $INSTTYPE == ${INSTTYPE_STANDARD}
  ${OrIf} $INSTTYPE == ${INSTTYPE_TOTALCMD}
    EnableWindow $0 0
  ${EndIf}
  ${If} $ALTCTL == 1
    SendMessage $0 ${BM_SETCHECK} 1 0
  ${EndIf}

  ReadINIStr $0 "$INI" "Field 5" "HWND"
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeStandardText)'

  ReadINIStr $0 "$INI" "Field 6" "HWND"
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeTotalcmdText)'

  ReadINIStr $0 "$INI" "Field 7" "HWND"
  ${If} $ALTCTL == 1
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepadTextAlt)'
  ${Else}
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepadText)'
  ${EndIf}
  ${If} $REDCTL == 1
    SetCtlColors $0 /BRANDING 0xFF0000
  ${EndIf}


  InstallOptions::show
  Pop $0
FunctionEnd

Function CustomLeave
  ReadINIStr $0 "$INI" "Settings" "State"
  StrCmp $0 0 next
  StrCmp $0 1 standard
  StrCmp $0 2 totalcmd
  StrCmp $0 3 notepad
  StrCmp $0 4 notepadalt
  abort

  standard:
  totalcmd:
  ReadINIStr $0 "$INI" "Field 4" "HWND"
  EnableWindow $0 0
  ${If} $REDCTL == 0
    abort
  ${EndIf}
  ReadINIStr $0 "$INI" "Field 7" "HWND"
  SetCtlColors $0 /BRANDING 0x000000
  ShowWindow $0 ${SW_HIDE}
  ShowWindow $0 ${SW_SHOW}
  StrCpy $REDCTL 0
  abort

  notepad:
  notepadalt:
  ReadINIStr $ALTCTL "$INI" "Field 4" "State"
  ${If} $ALTCTL == 1
    ReadINIStr $0 "$INI" "Field 7" "HWND"
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepadTextAlt)'
    StrCpy $REDCTL 0
    SetCtlColors $0 /BRANDING 0x000000
    ShowWindow $0 ${SW_HIDE}
    ShowWindow $0 ${SW_SHOW}
  ${Else}
    ReadINIStr $0 "$INI" "Field 7" "HWND"
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepadText)'
    StrCpy $REDCTL 1
    SetCtlColors $0 /BRANDING 0xFF0000
    ShowWindow $0 ${SW_HIDE}
    ShowWindow $0 ${SW_SHOW}
  ${EndIf}
  ReadINIStr $0 "$INI" "Field 4" "HWND"
  EnableWindow $0 1
  abort

  next:
  ReadINIStr $0 "$INI" "Field 1" "State"
  ${If} $0 == 1
    StrCpy $INSTTYPE ${INSTTYPE_STANDARD}
    goto getdir
  ${EndIf}
  ReadINIStr $0 "$INI" "Field 2" "State"
  ${If} $0 == 1
    StrCpy $INSTTYPE ${INSTTYPE_TOTALCMD}
    goto getdir
  ${EndIf}
  ReadINIStr $0 "$INI" "Field 3" "State"
  ${If} $0 == 1
    ${If} $ALTCTL == 1
      StrCpy $INSTTYPE ${INSTTYPE_NOTEPADALT}
    ${Else}
      StrCpy $INSTTYPE ${INSTTYPE_NOTEPAD}
    ${EndIf}
  ${EndIf}

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
  ${If} $INSTTYPE == ${INSTTYPE_STANDARD}
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextStandard)'
  ${ElseIf} $INSTTYPE == ${INSTTYPE_TOTALCMD}
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextTotalcmd)'
  ${ElseIf} $INSTTYPE == ${INSTTYPE_NOTEPAD}
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextNotepad)'
  ${ElseIf} $INSTTYPE == ${INSTTYPE_NOTEPADALT}
    SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextStandard)'
  ${EndIf}

  GetDlgItem $0 $R0 1050
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutGroup)'

  GetDlgItem $0 $R0 1051
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutQuickLaunch)'

  GetDlgItem $0 $R0 1052
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutDesktop)'

  GetDlgItem $0 $R0 1053
  SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutStartMenu)'

  ${If} $INSTTYPE == ${INSTTYPE_STANDARD}
    SendMessage $0 ${BM_SETCHECK} 1 0
  ${EndIf}
FunctionEnd

Function .onVerifyInstDir
  ${If} $INSTTYPE == ${INSTTYPE_TOTALCMD}
    StrCpy $TCDIR $INSTDIR
    ${IfNot} ${FileExists} "$TCDIR\TotalCmd.exe"
    ${AndIfNot} ${FileExists} "$TCDIR\TotalCmd64.exe"
      ${GetParent} "$INSTDIR" $TCDIR
      ${IfNot} ${FileExists} "$TCDIR\TotalCmd.exe"
      ${AndIfNot} ${FileExists} "$TCDIR\TotalCmd64.exe"
        Abort
      ${EndIf}
    ${EndIf}
  ${EndIf}
FunctionEnd

Function DirectoryLeave
  Call SetInstallDirectory

  GetDlgItem $0 $R0 1051
  SendMessage $0 ${BM_GETCHECK} 0 0 $1
  ${If} $1 == 1
    IntOp $SHORTCUT $SHORTCUT | ${SHORTCUT_QUICKLAUNCH}
  ${EndIf}

  GetDlgItem $0 $R0 1052
  SendMessage $0 ${BM_GETCHECK} 0 0 $1
  ${If} $1 == 1
    IntOp $SHORTCUT $SHORTCUT | ${SHORTCUT_DESKTOP}
  ${EndIf}

  GetDlgItem $0 $R0 1053
  SendMessage $0 ${BM_GETCHECK} 0 0 $1
  ${If} $1 == 1
    IntOp $SHORTCUT $SHORTCUT | ${SHORTCUT_STARTMENU}
  ${EndIf}

  Call CreateShortcuts
FunctionEnd

Function GetInstallDirectory
  ${If} $INSTTYPE == ${INSTTYPE_STANDARD}
  ${OrIf} $INSTTYPE == ${INSTTYPE_NOTEPADALT}
    !if ${PRODUCT_BIT} == "64"
      StrCpy $INSTDIR "$PROGRAMFILES64\${PRODUCT_NAME}"
    !else
      StrCpy $INSTDIR "$PROGRAMFILES\${PRODUCT_NAME}"
    !endif
    ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
    ${GetParent} "$0" $0
    ${GetParent} "$0" $0
    ${If} $0 != ''
      StrCpy $INSTDIR $0
    ${EndIf}
  ${ElseIf} $INSTTYPE == ${INSTTYPE_TOTALCMD}
    ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
    ${If} $0 != ''
      ${GetParent} "$0" $0
      ${GetParent} "$0" $0
      ${GetParent} "$0" $0
      ${If} ${FileExists} "$0\TotalCmd.exe"
      ${OrIf} ${FileExists} "$0\TotalCmd64.exe"
        StrCpy $INSTDIR "$0\${PRODUCT_NAME}"
        goto end
      ${EndIf}
    ${EndIf}

    ReadRegStr $0 HKCU "SOFTWARE\Ghisler\Total Commander" "InstallDir"
    ${If} $0 != ''
      ${If} ${FileExists} "$0\TotalCmd.exe"
      ${OrIf} ${FileExists} "$0\TotalCmd64.exe"
        StrCpy $INSTDIR "$0\${PRODUCT_NAME}"
        goto end
      ${EndIf}
    ${EndIf}
    ${If} ${FileExists} "C:\TotalCmd\TotalCmd.exe"
    ${OrIf} ${FileExists} "C:\TotalCmd\TotalCmd64.exe"
      StrCpy $INSTDIR "C:\TotalCmd\${PRODUCT_NAME}"
      goto end
    ${EndIf}
    ${If} ${FileExists} "C:\TC\TotalCmd.exe"
    ${OrIf} ${FileExists} "C:\TC\TotalCmd64.exe"
      StrCpy $INSTDIR "C:\TC\${PRODUCT_NAME}"
      goto end
    ${EndIf}
    StrCpy $INSTDIR ""
  ${ElseIf} $INSTTYPE == ${INSTTYPE_NOTEPAD}
    ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion" "VersionNumber"
    ${If} $0 == ''
      StrCpy $INSTDIR "$SYSDIR"
    ${Else}
      StrCpy $INSTDIR "$WINDIR"
    ${EndIf}
  ${EndIf}

  end:
FunctionEnd

Function SetInstallDirectory
  StrCpy $SETUPDIR "$INSTDIR"
  StrCpy $SETUPEXE "$SETUPDIR\AkelPad.exe"
  ${If} $INSTTYPE == ${INSTTYPE_NOTEPAD}
    ${If} $SETUPDIR == $WINDIR
    ${OrIf} $SETUPDIR == $SYSDIR
    ${OrIf} $SETUPDIR == $WINDIR\SysWOW64
      ${If} ${RunningX64}
      ${AndIf} $SETUPDIR == $SYSDIR
        ;Force install to SysWOW64, because AkelUpdater and uninstaller are x86 bit.
        StrCpy $SETUPDIR "$WINDIR\SysWOW64"
      ${EndIf}
      StrCpy $SETUPEXE "$SETUPDIR\notepad.exe"
    ${EndIf}
  ${EndIf}
FunctionEnd

Function CreateShortcuts
  IntOp $0 $SHORTCUT & ${SHORTCUT_QUICKLAUNCH}
  ${If} $0 != 0
    SetOutPath "$SETUPDIR"
    CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$SETUPEXE"
  ${EndIf}

  IntOp $0 $SHORTCUT & ${SHORTCUT_DESKTOP}
  ${If} $0 != 0
    SetOutPath "$SETUPDIR"
    CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$SETUPEXE"
  ${EndIf}

  IntOp $0 $SHORTCUT & ${SHORTCUT_STARTMENU}
  ${If} $0 != 0
    SetOutPath "$SETUPDIR"
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$SETUPEXE"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk" "$SETUPDIR\AkelFiles\Uninstall.exe"
    ${If} $SYSLANGUAGE == ${LANG_RUSSIAN}
      CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$SETUPDIR\AkelFiles\Docs\AkelHelp-Rus.htm"
    ${Else}
      CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$SETUPDIR\AkelFiles\Docs\AkelHelp-Eng.htm"
    ${EndIf}
  ${EndIf}
FunctionEnd

Section
  ${If} ${RunningX64}
    ${DisableX64FSRedirection}
  ${EndIf}

  SetOutPath "$SETUPDIR"
  !if ${PRODUCT_BIT} == "64"
    File /r /x RichTune*.* /x SpellCheck*.* /x GNUASpell.Copying "${PRODUCT_DIR}\*.*"
  !else
    File /r /x RichTune*.* "${PRODUCT_DIR}\*.*"
  !endif

  #IfFileExists "$SETUPDIR\AkelFiles\Plugs\Scripts.dll" 0 +2
  #RegDLL "$SETUPDIR\AkelFiles\Plugs\Scripts.dll"

  ${If} $INSTTYPE == ${INSTTYPE_STANDARD}
    ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
  ${ElseIf} $INSTTYPE == ${INSTTYPE_TOTALCMD}
    ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'

    Call GetTCINI
    ${If} $TCINI == ''
      goto RegInfo
    ${EndIf}

    ReadINIStr $0 "$TCINI" "Configuration" "Editor"
    ${If} $0 != ''
      ${GetFileName} "$0" $1
      ${If} $1 != "Akelpad.exe"
        WriteINIStr "$TCINI" "Configuration" "Editor_AkelUndo" "$0"
      ${EndIf}
    ${EndIf}
    ReadINIStr $0 "$TCINI" "Configuration" "MultiRenameEdit"
    ${If} $0 != ''
      ${GetFileName} "$0" $1
      ${If} $1 != "Akelpad.exe"
        WriteINIStr "$TCINI" "Configuration" "MultiRenameEdit_AkelUndo" "$0"
      ${EndIf}
    ${EndIf}

    ${WordReplace} "$SETUPDIR\AkelPad.exe" "$TCDIR" "%COMMANDER_PATH%" "+" $0
    WriteINIStr "$TCINI" "Configuration" "Editor" "$0"
    WriteINIStr "$TCINI" "Configuration" "MultiRenameEdit" "$0"
  ${ElseIf} $INSTTYPE == ${INSTTYPE_NOTEPAD}
    ;Take notepad.exe ownership to replace it
    ${If} ${FileExists} "$SYSDIR\takeown.exe"
      ${If} ${FileExists} "$WINDIR\notepad.exe"
        nsExec::Exec '"$SYSDIR\takeown.exe" /F "$WINDIR\notepad.exe"'
        Pop $0
      ${EndIf}
      ${If} ${FileExists} "$SYSDIR\notepad.exe"
        nsExec::Exec '"$SYSDIR\takeown.exe" /F "$SYSDIR\notepad.exe"'
        Pop $0
      ${EndIf}
      ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
        nsExec::Exec '"$SYSDIR\takeown.exe" /F "$WINDIR\SysWOW64\notepad.exe"'
        Pop $0
      ${EndIf}

      ;Use icacls.exe instead of cacls.exe. Because "echo y" will be useless on French OS (prompt between "O/N").
      ${If} ${FileExists} "$SYSDIR\icacls.exe"
        ${If} ${FileExists} "$WINDIR\notepad.exe"
          nsExec::Exec '"$SYSDIR\icacls.exe" "$WINDIR\notepad.exe" /grant "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$SYSDIR\notepad.exe"
          nsExec::Exec '"$SYSDIR\icacls.exe" "$SYSDIR\notepad.exe" /grant "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
          nsExec::Exec '"$SYSDIR\icacls.exe" "$WINDIR\SysWOW64\notepad.exe" /grant "$USERNAME":F'
          Pop $0
        ${EndIf}
      ${ElseIf} ${FileExists} "$SYSDIR\cacls.exe"
        ${If} ${FileExists} "$WINDIR\notepad.exe"
          nsExec::Exec '$COMSPEC /c echo y|"$SYSDIR\cacls.exe" "$WINDIR\notepad.exe" /G "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$SYSDIR\notepad.exe"
          nsExec::Exec '$COMSPEC /c echo y|"$SYSDIR\cacls.exe" "$SYSDIR\notepad.exe" /G "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
          nsExec::Exec '$COMSPEC /c echo y|"$SYSDIR\cacls.exe" "$WINDIR\SysWOW64\notepad.exe" /G "$USERNAME":F'
          Pop $0
        ${EndIf}
      ${EndIf}
    ${EndIf}

    #Create backup
    ${If} ${FileExists} "$WINDIR\notepad.exe"
    ${AndIfNot} ${FileExists} "$WINDIR\notepad_AkelUndo.exe"
      CopyFiles /SILENT "$WINDIR\notepad.exe" "$WINDIR\notepad_AkelUndo.exe"
    ${EndIf}
    ${If} ${FileExists} "$SYSDIR\notepad.exe"
    ${AndIfNot} ${FileExists} "$SYSDIR\notepad_AkelUndo.exe"
      CopyFiles /SILENT "$SYSDIR\notepad.exe" "$SYSDIR\notepad_AkelUndo.exe"
    ${EndIf}
    ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
    ${AndIfNot} ${FileExists} "$WINDIR\SysWOW64\notepad_AkelUndo.exe"
      CopyFiles /SILENT "$WINDIR\SysWOW64\notepad.exe" "$WINDIR\SysWOW64\notepad_AkelUndo.exe"
    ${EndIf}
    ${If} ${FileExists} "$SYSDIR\DLLCACHE\notepad.exe"
      Delete "$SYSDIR\DLLCACHE\notepad.exe"
    ${EndIf}
    ${If} ${FileExists} "$WINDIR\SysWOW64\DLLCACHE\notepad.exe"
      Delete "$WINDIR\SysWOW64\DLLCACHE\notepad.exe"
    ${EndIf}

    ${If} $SETUPDIR == $WINDIR
    ${OrIf} $SETUPDIR == $SYSDIR
    ${OrIf} $SETUPDIR == $WINDIR\SysWOW64
      ${If} $SETUPDIR == $WINDIR
        ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
          File /oname=$WINDIR\SysWOW64\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
        ${EndIf}
        File /oname=$SYSDIR\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
      ${ElseIf} $SETUPDIR == $SYSDIR
        ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
          File /oname=$WINDIR\SysWOW64\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
        ${EndIf}
        File /oname=$WINDIR\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
      ${ElseIf} $SETUPDIR == $WINDIR\SysWOW64
        File /oname=$WINDIR\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
        File /oname=$SYSDIR\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
      ${EndIf}

      ${If} ${FileExists} "$SETUPDIR\notepad.exe"
        Delete "$SETUPDIR\notepad.exe"
      ${EndIf}
      Rename "$SETUPDIR\AkelPad.exe" "$SETUPDIR\notepad.exe"
      ExecWait '"$SETUPDIR\notepad.exe" /reassoc /quit'
      WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\notepad.exe"
    ${Else}
      ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
        File /oname=$WINDIR\SysWOW64\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
      ${EndIf}
      File /oname=$WINDIR\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"
      File /oname=$SYSDIR\notepad.exe "Redirect\${PRODUCT_NOTEPAD}.exe"

      ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
      WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\AkelPad.exe"
    ${EndIf}
  ${ElseIf} $INSTTYPE == ${INSTTYPE_NOTEPADALT}
    ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'

    WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\notepad.exe" "Debugger" `"$SETUPDIR\AkelPad.exe" /z`
  ${EndIf}

  RegInfo:
  WriteUninstaller "$SETUPDIR\AkelFiles\Uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayName" "$(^Name)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayIcon" "$SETUPEXE"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString" "$SETUPDIR\AkelFiles\Uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "InstallType" "$INSTTYPE"

  ClearErrors
  ReadRegStr $0 HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule"
  IfErrors 0 end
  ${If} $SYSLANGUAGE == ${LANG_RUSSIAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Russian.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_UKRAINIAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Ukrainian.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_BELORUSSIAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Belorussian.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_GERMAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "German.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_SPANISH}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Spanish.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_FRENCH}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "French.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_JAPANESE}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Japanese.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_BRAZILIAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Brazilian.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_ITALIAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Italian.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_KOREAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Korean.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_CHINESE_TRADITIONAL}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Chinese (Traditional).dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_CHINESE_SIMPLIFIED}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Chinese (Simplified).dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_SLOVAK}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Slovak.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_ROMANIAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Romanian.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_DUTCH}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Dutch.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_FINNISH}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Finnish.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_POLISH}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Polish.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_CZECH}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Czech.dll"
  ${ElseIf} $SYSLANGUAGE == ${LANG_HUNGARIAN}
    WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Hungarian.dll"
  ${EndIf}

  end:
SectionEnd


;_____________________________________________________________________________________________
;
;                                     Uninstall
;_____________________________________________________________________________________________

Function un.onInit
  !if ${PRODUCT_BIT} == "64"
    ${IfNot} ${RunningX64}
      MessageBox MB_OK|MB_ICONEXCLAMATION "$(No64bit)"
      quit
    ${EndIf}
    SetRegView 64
  !endif

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
  ${If} ${RunningX64}
    ${DisableX64FSRedirection}
  ${EndIf}

  ${un.GetParent} "$INSTDIR" $SETUPDIR

  ReadRegStr $INSTTYPE HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "InstallType"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
  Delete "$SETUPDIR\AkelFiles\Uninstall.exe"

  SetOutPath "$TEMP"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk"
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

  ${If} $UNSETTINGS != 0
    DeleteRegKey HKCU "SOFTWARE\Akelsoft\${PRODUCT_NAME}"
    ${If} ${FileExists} "$SETUPDIR\AkelPad.ini"
      Delete "$SETUPDIR\AkelPad.ini"
    ${EndIf}
  ${EndIf}

  StrCmp $UNFILES 0 End
  #IfFileExists "$SETUPDIR\AkelFiles\Plugs\Scripts.dll" 0 +2
  #UnRegDLL "$SETUPDIR\AkelFiles\Plugs\Scripts.dll"

  #_notepad:
  ${If} $INSTTYPE == ${INSTTYPE_NOTEPAD}
    ;Take notepad.exe ownership to replace it
    ${If} ${FileExists} "$SYSDIR\takeown.exe"
      ${If} ${FileExists} "$WINDIR\notepad.exe"
        nsExec::Exec '"$SYSDIR\takeown.exe" /F "$WINDIR\notepad.exe"'
        Pop $0
      ${EndIf}
      ${If} ${FileExists} "$SYSDIR\notepad.exe"
        nsExec::Exec '"$SYSDIR\takeown.exe" /F "$SYSDIR\notepad.exe"'
        Pop $0
      ${EndIf}
      ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
        nsExec::Exec '"$SYSDIR\takeown.exe" /F "$WINDIR\SysWOW64\notepad.exe"'
        Pop $0
      ${EndIf}

      ;Use icacls.exe instead of cacls.exe. Because "echo y" will be useless on French OS (prompt between "O/N").
      ${If} ${FileExists} "$SYSDIR\icacls.exe"
        ${If} ${FileExists} "$WINDIR\notepad.exe"
          nsExec::Exec '"$SYSDIR\icacls.exe" "$WINDIR\notepad.exe" /grant "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$SYSDIR\notepad.exe"
          nsExec::Exec '"$SYSDIR\icacls.exe" "$SYSDIR\notepad.exe" /grant "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
          nsExec::Exec '"$SYSDIR\icacls.exe" "$WINDIR\SysWOW64\notepad.exe" /grant "$USERNAME":F'
          Pop $0
        ${EndIf}
      ${ElseIf} ${FileExists} "$SYSDIR\cacls.exe"
        ${If} ${FileExists} "$WINDIR\notepad.exe"
          nsExec::Exec '$COMSPEC /c echo y|"$SYSDIR\cacls.exe" "$WINDIR\notepad.exe" /G "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$SYSDIR\notepad.exe"
          nsExec::Exec '$COMSPEC /c echo y|"$SYSDIR\cacls.exe" "$SYSDIR\notepad.exe" /G "$USERNAME":F'
          Pop $0
        ${EndIf}
        ${If} ${FileExists} "$WINDIR\SysWOW64\notepad.exe"
          nsExec::Exec '$COMSPEC /c echo y|"$SYSDIR\cacls.exe" "$WINDIR\SysWOW64\notepad.exe" /G "$USERNAME":F'
          Pop $0
        ${EndIf}
      ${EndIf}
    ${EndIf}

    ${If} ${FileExists} "$WINDIR\notepad_AkelUndo.exe"
      ExecWait '"$WINDIR\notepad.exe" /deassoc /quit'
    ${ElseIf} ${FileExists} "$SYSDIR\notepad_AkelUndo.exe"
      ExecWait '"$SYSDIR\notepad.exe" /deassoc /quit'
    ${ElseIf} ${FileExists} "$WINDIR\SysWOW64\notepad_AkelUndo.exe"
      ExecWait '"$WINDIR\SysWOW64\notepad.exe" /deassoc /quit'
    ${Else}
      goto _totalcmd
    ${EndIf}

    #Restore backup
    ${If} ${FileExists} "$WINDIR\notepad_AkelUndo.exe"
      Delete "$WINDIR\notepad.exe"
      Rename "$WINDIR\notepad_AkelUndo.exe" "$WINDIR\notepad.exe"
    ${EndIf}
    ${If} ${FileExists} "$SYSDIR\notepad_AkelUndo.exe"
      Delete "$SYSDIR\notepad.exe"
      Rename "$SYSDIR\notepad_AkelUndo.exe" "$SYSDIR\notepad.exe"
    ${EndIf}
    ${If} ${FileExists} "$WINDIR\SysWOW64\notepad_AkelUndo.exe"
      Delete "$WINDIR\SysWOW64\notepad.exe"
      Rename "$WINDIR\SysWOW64\notepad_AkelUndo.exe" "$WINDIR\SysWOW64\notepad.exe"
    ${EndIf}
  ${EndIf}

  DeleteRegValue HKLM "Software\Akelsoft\AkelPad" "Path"

  _totalcmd:
  StrCpy $TCDIR $SETUPDIR
  ${IfNot} ${FileExists} "$TCDIR\TotalCmd.exe"
  ${AndIfNot} ${FileExists} "$TCDIR\TotalCmd64.exe"
    ${un.GetParent} "$SETUPDIR" $TCDIR
    ${IfNot} ${FileExists} "$TCDIR\TotalCmd.exe"
    ${AndIfNot} ${FileExists} "$TCDIR\TotalCmd64.exe"
      goto _standard
    ${EndIf}
  ${EndIf}
  ExecWait '"$SETUPDIR\AkelPad.exe" /deassoc /quit'

  Call un.GetTCINI
  ${If} $TCINI == ''
    goto DeleteFiles
  ${EndIf}

  ReadINIStr $0 "$TCINI" "Configuration" "Editor_AkelUndo"
  ${If} $0 != ''
    WriteINIStr "$TCINI" "Configuration" "Editor" "$0"
    DeleteINIStr "$TCINI" "Configuration" "Editor_AkelUndo"
  ${EndIf}
  ReadINIStr $0 "$TCINI" "Configuration" "MultiRenameEdit_AkelUndo"
  ${If} $0 != ''
    WriteINIStr "$TCINI" "Configuration" "MultiRenameEdit" "$0"
    DeleteINIStr "$TCINI" "Configuration" "MultiRenameEdit_AkelUndo"
  ${EndIf}

  ReadINIStr $0 "$TCINI" "Configuration" "Editor"
  ${If} $0 != ''
    ${un.GetFileName} "$0" $1
    ${If} $1 == "Akelpad.exe"
      DeleteINIStr "$TCINI" "Configuration" "Editor"
    ${EndIf}
  ${EndIf}
  ReadINIStr $0 "$TCINI" "Configuration" "MultiRenameEdit"
  ${If} $0 != ''
    ${un.GetFileName} "$0" $1
    ${If} $1 == "Akelpad.exe"
      DeleteINIStr "$TCINI" "Configuration" "MultiRenameEdit"
    ${EndIf}
  ${EndIf}
  goto DeleteFiles

  _standard:
  ReadRegStr $0 HKLM "Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\notepad.exe" "Debugger"
  ${If} $0 == `"$SETUPDIR\AkelPad.exe" /z`
    DeleteRegValue HKLM "Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\notepad.exe" "Debugger"
  ${EndIf}
  ExecWait '"$SETUPDIR\AkelPad.exe" /deassoc /quit'

  DeleteFiles:
  Delete "$SETUPDIR\AkelFiles\Plugs\Coder\cache"

  ;Generate file list and include it in script at compile-time
  !execute 'unList\unList.exe /DATE=0 /INSTDIR="${PRODUCT_DIR}" /LOG=unList.txt /UNDIR_VAR=$SETUPDIR /MB=0'
  !include 'unList\unList.txt'
  !delfile 'unList\unList.txt'
  RMDir "$SETUPDIR"

  End:
  StrCpy $UNRESULT "SuccessUninstall"
  quit
SectionEnd

Function un.onGUIEnd
  ${If} $UNRESULT == "SuccessUninstall"
    MessageBox MB_ICONINFORMATION|MB_OK "$(UninstallSuccess)"
  ${EndIf}
FunctionEnd

;Shared functions
!macro GetTCINI un
  Function ${un}GetTCINI
    ;Check in directory of the program
    StrCpy $TCINI "$TCDIR\Wincmd.ini"
    ${If} ${FileExists} "$TCINI"
      ReadINIStr $0 "$TCINI" "Configuration" "UseIniInProgramDir"
      IntOp $1 $0 & 0x1
      ${If} $1 != 0
        goto IniFound
      ${EndIf}
    ${EndIf}

    ;Check in registry
    ReadRegStr $TCINI HKCU "SOFTWARE\Ghisler\Total Commander" "IniFileName"
    ${If} $TCINI != ''
      goto IniFound
    ${EndIf}
    ReadRegStr $TCINI HKLM "SOFTWARE\Ghisler\Total Commander" "IniFileName"
    ${If} $TCINI != ''
      goto IniFound
    ${EndIf}

    ;Check in Application Data
    StrCpy $TCINI "$APPDATA\GHISLER\Wincmd.ini"
    ${If} ${FileExists} "$TCINI"
      goto IniFound
    ${EndIf}

    ;Check in environment paths
    SearchPath $TCINI "Wincmd.ini"
    ${If} $TCINI == ''
      Return
    ${EndIf}

    IniFound:
    ;Check section redirection
    ReadINIStr $0 "$TCINI" "Configuration" "RedirectSection"
    ${If} $0 == '1'
      ReadINIStr $TCINI "$TCINI" "Configuration" "AlternateUserIni"
    ${EndIf}
  FunctionEnd
!macroend

!insertmacro GetTCINI ""
!insertmacro GetTCINI "un."
