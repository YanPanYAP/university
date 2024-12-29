.586
.model flat,stdcall
includelib kernel32.lib
includelib libucrt.lib
includelib ..\Debug\SE_asm01a.lib


ExitProcess PROTO : DWORD
getmin PROTO: DWORD, : DWORD
getmax PROTO: DWORD, : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

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
	invoke getmin, offset Arr, lengthof Arr

	mov result, eax

	invoke getmax, offset Arr, lengthof Arr

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
	mov esi, 0			; счетчик количества цифр 
	mov eax, intfield	; eax: Хранит число для обработки
	cdq					; cdq: Расширяет знак из eax в edx
	mov ebx, 10
	idiv ebx			; idiv ebx: Делит eax на 10. Остаток (edx) — это следующая цифра

	; Цикл обработки цифр
cycle:
	push dx				; Сохраняем текущую цифру (остаток от деления в EDX) в стек.
	inc esi				; Увеличиваем счетчик цифр (ESI).
	test eax, eax		; Проверяем, равно ли число в EAX нулю.	
	jz fin				; Если EAX = 0, переходим к завершению обработки цифр.
	cdq
	idiv ebx
	jmp cycle

	; Обратная запись цифр
fin:
	mov ecx, esi
write: 
	pop bx				; Извлекаем сохраненную цифру из стека в регистр BX.
	add bl, '0'			; преобразует ее в ASCII		
	mov[edi], bl		; Записываем ASCII-символ в буфер (указатель EDI).
	inc edi				; Сдвигаем указатель буфера на следующую позицию.
	loop write			; Уменьшаем ECX и повторяем, пока оно не станет равно 0.
	ret
tochar ENDP

end main  