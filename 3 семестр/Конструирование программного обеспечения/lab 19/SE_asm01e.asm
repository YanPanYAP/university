.586
.model flat,stdcall
includelib kernel32.lib
includelib libucrt.lib
includelib ..\Debug\SE_asm01d.lib


ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD
EXTRN getmax: proc
EXTRN getmin: proc

.stack 4096
.const
Arr sdword 5, 12, -9, 37, 46, 0, -14, 52, 101, -2

.data
MB_OK	EQU 0  

result1	BYTE "getmax + getmin = ", 0
result	sdword 0
result2	BYTE 40 dup(0)	; буфер для хранения результата

STR1	DB "Орловская Полина Валерьевна 2 курс 9 группа", 0 
HW		DD ?  

.code

main PROC
START:	
	push lengthof Arr	; длина(количество элементов массива)
	push offset Arr		; смещение
	call getmin

	mov result, eax

	push lengthof Arr	; длина(количество элементов массива)
	push offset Arr		; смещение
	call getmax

	add result, eax
	xor eax, eax

	; Конвертация числа в строку
	push result
	push offset result2
	call tochar
	
	invoke MessageBoxA, 0, offset result1, offset STR1, MB_OK
	invoke MessageBoxA, 0, offset result2, offset STR1, MB_OK

 	push 0  
	CALL ExitProcess 

main ENDP 

; Преобразование числа
tochar PROC uses eax ebx ecx edi esi, pstr : dword, intfield : sdword
	mov edi, pstr		; edi: Указывает на буфер.
	mov esi, 0
	mov eax, intfield	; eax: Хранит число для обработки
	cdq					; cdq: Расширяет знак из eax в edx
	mov ebx, 10
	idiv ebx			; idiv ebx: Делит eax на 10. Остаток (edx) — это следующая цифра

	; Цикл обработки цифр
cycle:
	push dx
	inc esi
	test eax, eax
	jz fin
	cdq
	idiv ebx
	jmp cycle

	; Обратная запись цифр
fin:
	mov ecx, esi
write: 
	pop bx
	add bl, '0'		; преобразует их в ASCII
	mov[edi], bl
	inc edi
	loop write
	ret
tochar ENDP


end main  