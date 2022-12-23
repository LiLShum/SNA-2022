.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
	includelib ../Debug/Lib.lib
	ExitProcess PROTO :DWORD

	readw PROTO: DWORD
	readr PROTO: DWORD
	strln PROTO: DWORD
	strtonum PROTO: BYTE

.stack 4096
.const
	num_err byte 'Error: Переполнение в результате арифметической операции!', 0
	L0 BYTE "Default value", 0
	L1 DWORD 12
	L2 DWORD 8
	L3 DWORD 15
	L4 DWORD 70
	L5 DWORD 71
	L6 DWORD 0
	L7 DWORD 17
	L8 BYTE "negavtive element", 0
	L9 BYTE "Result of logic and", 0
	L10 BYTE "Result of logic or", 0
	L11 BYTE "Result of logic not", 0
	L12 BYTE "Checkstrln", 0
	L13 BYTE "Result of STRLN(checkstrln)", 0
	L14 BYTE "5", 0
	L15 BYTE "Result of STRTONUM", 0
	L16 DWORD 2
	L17 BYTE "Result strton plus 2", 0
	L18 BYTE "Result of cycle", 0
	L19 DWORD 10
	L20 DWORD 6
	L21 BYTE "RESULT FUNC SECOND", 0
	L22 DWORD 74
	L23 BYTE "Octal number", 0
.data
	firstfx DWORD ?
	secondres DWORD ?
	startsta DWORD ?
	startsafa DWORD ?
	startonea DWORD ?
	starttwoa DWORD ?
	startoneo DWORD ?
	starttwoo DWORD ?
	startinvrt DWORD ?
	startresa DWORD ?
	startreso DWORD ?
	startresn DWORD ?
	startresng DWORD ?
	startonestr DWORD ?
	starttwostr DWORD ?
	startresln DWORD ?
	startresstn DWORD ?
	startoneapp DWORD ?
	starttwoapp DWORD ?
	startthapp DWORD ?
	startfrapp DWORD ?
	startsys DWORD ?
	startresf DWORD ?

.code

first PROC firstfb : DWORD
	push firstfb
	pop edx
	push firstfb
	call strln
	push eax
	pop firstfx
	push firstfx
	ret
first ENDP

second PROC secondsa : SDWORD, secondsb : SDWORD
	push secondsa
	push secondsb
	pop eax
	pop ebx
	add eax, ebx
	cmp eax, 127
	jg num_error
	cmp eax, -128
	jl num_error
	push eax
	pop secondres
	push secondres
	ret
second ENDP

main PROC
	push offset L0
		call readw
	push startsta
		call readr
	push L1
	pop startonea
	push startonea
		call readr
	push L2
	pop starttwoa
	push L3
	pop startoneo
	push L2
	pop starttwoo
	push L3
	pop startinvrt
	push L4
	pop startoneapp
	push L5
	pop starttwoapp
	push L1
	pop startsafa
	push L6
	push L7
	pop ebx
	pop eax
	sub eax, ebx
cmp eax, 127
jg num_error
cmp eax, -128
jl num_error
	push eax
	pop startresng
	push offset L8
		call readw
	push startresng
		call readr
	push offset L9
		call readw
	push startonea
	push starttwoa
	pop eax
	pop ebx
	and eax, ebx
	cmp eax, 127
	jg num_error
	cmp eax, -128
	jl num_error
	push eax
	push ebx
	pop startresa
	push startresa
		call readr
	push offset L10
		call readw
	push startoneo
	push starttwoo
	pop eax
	pop ebx
	or eax, ebx
	cmp eax, 127
	jg num_error
	cmp ebx, -128
	jl num_error
	push eax
	push ebx
	pop startreso
	push startreso
		call readr
	push offset L11
		call readw
	push startinvrt
	pop eax
	not eax
	cmp eax, 127
	jg num_error
	cmp eax, -128
	jl num_error
	push eax
	pop startresn
	push startresn
		call readr
	push offset L12
	pop startonestr
	push offset L13
		call readw
	push startonestr
	pop edx
	push startonestr
	call first
	push eax
	pop startresln
	push startresln
		call readr
	push offset L14
	pop starttwostr
	push offset L15
		call readw
	push starttwostr
	pop edx
	push starttwostr
	call strtonum
	push eax
	pop startresstn
	push startresstn
		call readr
	push startresstn
	push L16
	pop eax
	pop ebx
	add eax, ebx
	cmp eax, 127
	jg num_error
	cmp eax, -128
	jl num_error
	push eax
	pop startthapp
	push offset L17
		call readw
	push startthapp
		call readr
	push offset L18
		call readw
	mov ecx, L19
Mark:
	push startfrapp
	push L20
	pop eax
	pop ebx
	add eax, ebx
	cmp eax, 127
	jg num_error
	cmp eax, -128
	jl num_error
	push eax
	pop startfrapp
	push startfrapp
loop Mark
	push startfrapp
		call readr
	push offset L21
		call readw
	push startoneapp
	push starttwoapp
	pop edx
	pop edx
	push starttwoapp
	push startoneapp
	call second
	push eax
	pop startresf
	push startresf
		call readr
	push L22
	pop startsys
	push offset L23
		call readw
	push startsys
		call readr
push 0
call ExitProcess
num_error::
push offset num_err
call readw
call ExitProcess
main ENDP
end main