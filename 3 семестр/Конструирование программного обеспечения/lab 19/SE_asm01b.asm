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
result2	BYTE 40 dup(0)	; ����� ��� �������� ����������

STR1	DB "��������� ������ ���������� 2 ���� 9 ������", 0 
HW		DD ?  

.code

main PROC
START:	
	invoke getmin, offset Arr, lengthof Arr

	mov result, eax

	invoke getmax, offset Arr, lengthof Arr

	add result, eax
	xor eax, eax

	; ����������� ����� � ������
	push result
	push offset result2
	call tochar


	invoke MessageBoxA, 0, offset result1, offset STR1, MB_OK
	invoke MessageBoxA, 0, offset result2, offset STR1, MB_OK

	push 0  
	CALL ExitProcess 

main ENDP 

; �������������� �����
tochar PROC uses eax ebx ecx edi esi, pstr : dword, intfield : sdword
	mov edi, pstr		; edi: ��������� �� �����.
	mov esi, 0			; ������� ���������� ���� 
	mov eax, intfield	; eax: ������ ����� ��� ���������
	cdq					; cdq: ��������� ���� �� eax � edx
	mov ebx, 10
	idiv ebx			; idiv ebx: ����� eax �� 10. ������� (edx) � ��� ��������� �����

	; ���� ��������� ����
cycle:
	push dx				; ��������� ������� ����� (������� �� ������� � EDX) � ����.
	inc esi				; ����������� ������� ���� (ESI).
	test eax, eax		; ���������, ����� �� ����� � EAX ����.	
	jz fin				; ���� EAX = 0, ��������� � ���������� ��������� ����.
	cdq
	idiv ebx
	jmp cycle

	; �������� ������ ����
fin:
	mov ecx, esi
write: 
	pop bx				; ��������� ����������� ����� �� ����� � ������� BX.
	add bl, '0'			; ����������� �� � ASCII		
	mov[edi], bl		; ���������� ASCII-������ � ����� (��������� EDI).
	inc edi				; �������� ��������� ������ �� ��������� �������.
	loop write			; ��������� ECX � ���������, ���� ��� �� ������ ����� 0.
	ret
tochar ENDP

end main  