.586
.model flat,stdcall
includelib kernel32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.stack 4096
.const
Arr sdword 5, 12, -9, 37, 46, 0, -14, 52, 101, -2 ; �������� ������

.data
MB_OK	EQU 0  

HW		DD ?  
min sdword ? ; ���������� ��� �������� ������������ ��������

.code

main PROC
START:	
    ; ���������� ������������ �������� �������
	push lengthof Arr	; ����� ������� (���������� ���������)
	push offset Arr		; �������� �������
	call getmin          ; ����� ������� getmin
	mov min, eax         ; ���������� ���������� � ���������� min

    ; ���������� ���������
	push 0  
	CALL ExitProcess 

main ENDP 

; ������� ��� ���������� ������������ �������� �������
getmin PROC uses ecx esi ebx, parr: dword, elem: dword
    mov ecx, elem       ; ���������� ��������� �������
    mov esi, parr       ; ��������� �� ������ �������
    mov ebx, [esi]      ; ������������, ��� ������ ������� � �����������

CYCLE:
    lodsd               ; ��������� ������� ������� ������� � EAX, ESI += 4
    cmp eax, ebx        ; �������� ������� ������� � �����������
    jge FALSE           ; ���� ������� ������� >= ������������, �������
    mov ebx, eax        ; �������� ����������� ��������

FALSE:
    loop CYCLE          ; ������� � ���������� ��������, ���� ECX != 0

    mov eax, ebx        ; ��������� ����������� �������� � EAX (�������)
    ret
getmin ENDP

end main  
