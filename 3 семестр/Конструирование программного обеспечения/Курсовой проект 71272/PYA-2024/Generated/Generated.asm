.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib C:\PYA-2024\Debug\GenLib.lib
ExitProcess		PROTO:DWORD

Remainder		PROTO : DWORD, :DWORD
Rand			PROTO : DWORD
Abs			PROTO : DWORD
Sqrt			PROTO : DWORD
Input			PROTO : DWORD
Writestroke		PROTO : DWORD
WriteNumb		PROTO : DWORD
Writelinestroke		PROTO : DWORD
WritelineNumb		PROTO : DWORD
Strlen			PROTO : DWORD
.stack 4096
.const
		L0 SDWORD 123
		L1 SDWORD 45
		L2 SDWORD 15
		L3 SDWORD 2
		L4 byte "'some text'", 0
		L5 SDWORD 13
		L6 byte "'Length of str1 is '", 0
		L7 SDWORD 1
		L8 SDWORD 10
		L9 SDWORD 12
		L10 byte '"a"', 0
		TRUE equ 1
		FALSE equ 0
		L11 word FALSE
		L12 byte "'Result of function max:'", 0
		L13 byte "'ff:'", 0
		L14 byte "'Result of function loo :'", 0
		L15 SDWORD 1000
		L16 byte "'Rand number :'", 0
		L17 SDWORD 25
.data
		emptyString byte 0
		maxresult sdword 0
		maxk sdword 0
		maxstr1 dword offset emptyString
		maxlen sdword 0
		circuita sdword 0
		circuititer sdword 0
		maina sdword 0
		mainb sdword 0
		mainc sdword 0
		mainS dword offset emptyString
		mainb1 word FALSE
		mainresult sdword 0
		mainstart sdword 0
		mainend sdword 0
		maink sdword 0
		mainran sdword 0
		mainaresult sdword 0
		mainbresult sdword 0
.code

;----------- max ------------

max PROC,
	maxq : sdword, maxv : sdword, maxz : sdword  
push ebx
push edx

pop eax
mov maxk, eax

mov edx, maxq
cmp edx, maxv

jl right1
jg wrong1
	right1:
mov edx, maxv
cmp edx, maxz

jl right2
jg wrong2
	right2:
pop eax
mov maxresult, eax

mov maxstr1, offset L4

jmp next2
	wrong2:
pop eax
mov maxresult, eax

next2:
jmp next1
	wrong1:
pop eax
mov maxresult, eax

next1:

push maxstr1
call WriteStroke


pop eax
mov maxlen, eax


push offset L6
call WriteStroke



push maxlen
call WritelineNumb



pop ebx
pop edx
mov eax, maxlen
ret
max ENDP
;------------------------------


;----------- circuit ------------

circuit PROC,
	circuitstart : sdword, circuitend : sdword  
push ebx
push edx

pop eax
mov circuita, eax

pop eax
mov circuititer, eax

mov edx, circuitstart
cmp edx, circuitend

jl repeat3
jmp repeatnext3
repeat3:


push circuitstart
call WritelineNumb


pop eax
mov circuitstart, eax

pop eax
mov circuititer, eax

mov edx, circuitstart
cmp edx, circuitend

jl repeat3
repeatnext3:

pop ebx
pop edx
mov eax, circuititer
ret
circuit ENDP
;------------------------------


;----------- MAIN ------------

main PROC
pop eax
mov maina, eax

pop eax
mov mainb, eax

pop eax
mov mainc, eax

mov mainS, offset L10

mov cx, mainb1

pop eax
mov mainresult, eax


push offset L12
call WriteStroke



push mainresult
call WritelineNumb



push offset L13
call WriteStroke


pop eax
mov mainstart, eax

pop eax
mov mainend, eax

pop eax
mov maink, eax


push offset L14
call WriteStroke



push maink
call WritelineNumb


pop eax
mov mainran, eax


push offset L16
call WriteStroke



push mainran
call WritelineNumb


pop eax
mov mainaresult, eax


push mainaresult
call WritelineNumb


pop eax
mov mainbresult, eax


push mainbresult
call WritelineNumb



;------------------------------

push 0
call ExitProcess
main ENDP
end main

