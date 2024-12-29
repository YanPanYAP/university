.586
.model flat,stdcall
includelib kernel32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.stack 4096
.const
Arr sdword 5, 12, -9, 37, 46, 0, -14, 52, 101, -2 ; Исходный массив

.data
MB_OK	EQU 0  

HW		DD ?  
min sdword ? ; Переменная для хранения минимального значения

.code

main PROC
START:	
    ; Вычисление минимального элемента массива
	push lengthof Arr	; Длина массива (количество элементов)
	push offset Arr		; Смещение массива
	call getmin          ; Вызов функции getmin
	mov min, eax         ; Сохранение результата в переменную min

    ; Завершение программы
	push 0  
	CALL ExitProcess 

main ENDP 

; Функция для нахождения минимального элемента массива
getmin PROC uses ecx esi ebx, parr: dword, elem: dword
    mov ecx, elem       ; Количество элементов массива
    mov esi, parr       ; Указатель на начало массива
    mov ebx, [esi]      ; Предполагаем, что первый элемент — минимальный

CYCLE:
    lodsd               ; Загрузить текущий элемент массива в EAX, ESI += 4
    cmp eax, ebx        ; Сравнить текущий элемент с минимальным
    jge FALSE           ; Если текущий элемент >= минимального, перейти
    mov ebx, eax        ; Обновить минимальное значение

FALSE:
    loop CYCLE          ; Переход к следующему элементу, если ECX != 0

    mov eax, ebx        ; Поместить минимальное значение в EAX (возврат)
    ret
getmin ENDP

end main  
