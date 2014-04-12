extrn StackSaveSysParam: proc
extrn AsmCallbackHelper: proc

SYSPARAMSTACK struct
  first       QWORD  ?
  last        QWORD  ?
  nElements   QWORD  ?
SYSPARAMSTACK ends

SYSPARAMITEM struct
  next        QWORD  ?
  prev        QWORD  ?
  dwType      QWORD  ?
  dwValue     QWORD  ?
SYSPARAMITEM ends

.data

.data?
;AsmCallSysFunc variables
hCurStack QWORD ?
hSaveStack QWORD ?
lpProcedure QWORD ?

;AsmCallbackCommonProc variables
nCallbackIndex QWORD ?

;AsmCallbackHelper variables
lpnFirstArg QWORD ?

.code

AsmCallSysFunc proc
  mov hCurStack, rcx   ; SYSPARAMSTACK *hCurStack
  mov hSaveStack, rdx  ; SYSPARAMSTACK *hSaveStack
  mov lpProcedure, r8  ; FARPROC lpProcedure

  ;Prolog (LocalSize=hCurStack->nElements * sizeof QWORD * 2 + 32)
  push rbp
  mov rbp, rsp
  mov r10, hCurStack
  mov r10, (SYSPARAMSTACK ptr [r10]).nElements
  ;rsp must be aligned to 16 (sizeof QWORD * 2)
  imul r10, r10, 16
  ;nElements can be 0, so reserve space for at least four parameters (sizeof QWORD * 4).
  add r10, 32
  sub rsp, r10
  ;Here rsp is aligned to 16, because:
  ;rsp+0            will be first function parameter
  ;rsp+LocalSize+0  saved rbp in prolog
  ;rsp+LocalSize+8  AsmCallSysFunc return address

  ;Push parameters. Items in SYSPARAMSTACK arranged in reverse order.
  mov r10, hCurStack
  mov r10, (SYSPARAMSTACK ptr [r10]).last
  mov r11, rsp

  @loop:
  cmp r10, 0
  je @endloop
  mov rax, (SYSPARAMITEM ptr [r10]).dwValue
  mov [r11], rax

  ;Next parameter
  mov r10, (SYSPARAMITEM ptr [r10]).prev
  add r11, sizeof QWORD
  jmp @loop
  @endloop:

  ;Move arguments from hCurStack to hSaveStack stack to support recursive calls
  sub rsp, 32
  mov  rcx, hCurStack
  mov  rdx, hSaveStack
  call StackSaveSysParam
  add rsp, 32

  ;Move first four parameter to registers
  mov  rcx, [rsp + 0]
  mov  rdx, [rsp + 8]
  mov  r8, [rsp + 16]
  mov  r9, [rsp + 24]

  ;Call system function
  call lpProcedure

  ;Epilog
  mov  rsp, rbp
  pop  rbp

  ret
AsmCallSysFunc endp

AsmCallbackCommonProc proc
  ;Set nCallbackIndex:
  ;rsp+0   AsmCallbackCommonProc return address
  ;rsp+8   nCallbackIndex argument
  ;rsp+16  AsmCallbackXProc return address
  mov  r10, [rsp+8]
  mov  nCallbackIndex, r10

  ;Prolog
  push rbp
  mov  rbp, rsp
  ;Here rsp is aligned to 16, because:
  ;rsp+0   saved rbp in prolog
  ;rsp+8   AsmCallbackCommonProc return address
  ;rsp+16  nCallbackIndex argument
  ;rsp+24  AsmCallbackXProc return address

  ;Get pointer to first element in stack
  mov  lpnFirstArg, rbp

  ;To get first argument pointer we skip first four elements in stack:
  ;rsp+0   saved rbp in prolog
  ;rsp+8   AsmCallbackCommonProc return address
  ;rsp+16  nCallbackIndex argument
  ;rsp+24  AsmCallbackXProc return address
  add  lpnFirstArg, 32

  ;Move registers to memory for use in AsmCallbackHelper
  mov  r10, lpnFirstArg
  mov  [r10 + 0], rcx
  mov  [r10 + 8], rdx
  mov  [r10 + 16], r8
  mov  [r10 + 24], r9

  ;Call script function
  sub rsp, 32
  mov  rcx, lpnFirstArg
  mov  rdx, nCallbackIndex
  mov  r8, 0
  call AsmCallbackHelper
  add rsp, 32

  ;Epilog
  mov  rsp, rbp
  pop  rbp

  ;We will use AsmCallbackXProc return address:
  ;rsp+0   AsmCallbackCommonProc return address
  ;rsp+8   nCallbackIndex argument
  ;rsp+16  AsmCallbackXProc return address
  add  rsp, 16
  ret
AsmCallbackCommonProc endp

AsmCallback1Proc proc
  push 1
  call AsmCallbackCommonProc
AsmCallback1Proc endp

AsmCallback2Proc proc
  push 2
  call AsmCallbackCommonProc
AsmCallback2Proc endp

AsmCallback3Proc proc
  push 3
  call AsmCallbackCommonProc
AsmCallback3Proc endp

AsmCallback4Proc proc
  push 4
  call AsmCallbackCommonProc
AsmCallback4Proc endp

AsmCallback5Proc proc
  push 5
  call AsmCallbackCommonProc
AsmCallback5Proc endp

AsmCallback6Proc proc
  push 6
  call AsmCallbackCommonProc
AsmCallback6Proc endp

AsmCallback7Proc proc
  push 7
  call AsmCallbackCommonProc
AsmCallback7Proc endp

AsmCallback8Proc proc
  push 8
  call AsmCallbackCommonProc
AsmCallback8Proc endp

AsmCallback9Proc proc
  push 9
  call AsmCallbackCommonProc
AsmCallback9Proc endp

AsmCallback10Proc proc
  push 10
  call AsmCallbackCommonProc
AsmCallback10Proc endp

AsmCallback11Proc proc
  push 11
  call AsmCallbackCommonProc
AsmCallback11Proc endp

AsmCallback12Proc proc
  push 12
  call AsmCallbackCommonProc
AsmCallback12Proc endp

AsmCallback13Proc proc
  push 13
  call AsmCallbackCommonProc
AsmCallback13Proc endp

AsmCallback14Proc proc
  push 14
  call AsmCallbackCommonProc
AsmCallback14Proc endp

AsmCallback15Proc proc
  push 15
  call AsmCallbackCommonProc
AsmCallback15Proc endp

AsmCallback16Proc proc
  push 16
  call AsmCallbackCommonProc
AsmCallback16Proc endp

AsmCallback17Proc proc
  push 17
  call AsmCallbackCommonProc
AsmCallback17Proc endp

AsmCallback18Proc proc
  push 18
  call AsmCallbackCommonProc
AsmCallback18Proc endp

AsmCallback19Proc proc
  push 19
  call AsmCallbackCommonProc
AsmCallback19Proc endp

AsmCallback20Proc proc
  push 20
  call AsmCallbackCommonProc
AsmCallback20Proc endp

AsmCallback21Proc proc
  push 21
  call AsmCallbackCommonProc
AsmCallback21Proc endp

AsmCallback22Proc proc
  push 22
  call AsmCallbackCommonProc
AsmCallback22Proc endp

AsmCallback23Proc proc
  push 23
  call AsmCallbackCommonProc
AsmCallback23Proc endp

AsmCallback24Proc proc
  push 24
  call AsmCallbackCommonProc
AsmCallback24Proc endp

AsmCallback25Proc proc
  push 25
  call AsmCallbackCommonProc
AsmCallback25Proc endp

AsmCallback26Proc proc
  push 26
  call AsmCallbackCommonProc
AsmCallback26Proc endp

AsmCallback27Proc proc
  push 27
  call AsmCallbackCommonProc
AsmCallback27Proc endp

AsmCallback28Proc proc
  push 28
  call AsmCallbackCommonProc
AsmCallback28Proc endp

AsmCallback29Proc proc
  push 29
  call AsmCallbackCommonProc
AsmCallback29Proc endp

AsmCallback30Proc proc
  push 30
  call AsmCallbackCommonProc
AsmCallback30Proc endp

END
