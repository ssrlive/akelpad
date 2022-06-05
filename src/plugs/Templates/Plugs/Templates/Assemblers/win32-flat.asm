;;;CARET
format PE GUI 4.0
entry start
stack 20000h
include 'Encoding\Utf8.inc'
include '%win32w.inc'
section '.data' data readable writeable				;Data
title	TCHAR	"Template", 0
message	TCHAR	"Template works perfectly", 0
section '.text' code readable executable			;Code
start:
	;;;CARET
	invoke MessageBox, HWND_DESKTOP, message, title, MB_OK
	invoke ExitProcess, 0
section '.idata' import data readable writeable		;Imports
library kernel, 'KERNEL32.DLL',\
	user, 'USER32.DLL'
import kernel,\
	ExitProcess, 'ExitProcess'
import user,\
	MessageBox, 'MessageBoxW'
;section '.rsrc' resource data readable				;Resources