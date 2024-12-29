.586
.model flat,stdcall
.code
getmin PROC uses ecx esi ebx, parr: dword, elem: dword
	mov ecx, elem
	mov esi, parr
	mov ebx, [esi]
CYCLE:
	lodsd
	cmp eax, ebx
	jge FALSE		; переход, если больше или равно, т.е. левый операнд >= правого операнда
	xchg ebx, eax	; Команда XCHG обменивает содержимое двух операндов
FALSE:
	loop CYCLE
	xchg eax, ebx
		ret
getmin ENDP

getmax PROC uses ecx esi ebx, parr: dword, elem: dword
	mov ecx, elem
	mov esi, parr
	mov ebx, [esi]
CYCLE:
	lodsd
	cmp eax, ebx
	jle FALSE		; переход, если меньше или равно, т.е. левый операнд <= правого операнда
	xchg ebx, eax	; Команда XCHG обменивает содержимое двух операндов
FALSE:
	loop CYCLE
	xchg eax, ebx
		ret
getmax ENDP

end