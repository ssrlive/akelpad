!define PRODUCT_NAME "AkelPad"
!define PRODUCT_VERSION "4.10.0-x64"
!define PRODUCT_BIT "64"
!define PRODUCT_DIR "Files-x64"
!define PRODUCT_NOTEPAD "notepad-x64"

!searchparse ${NSIS_VERSION} v VER_MAJOR .
!if ${VER_MAJOR} > 2
  !include /CHARSET=ACP "AkelPad_4.nsi"
!else
  !include "AkelPad_4.nsi"
!endif
