extern CDefaultInt
global DefaultInt

DefaultInt:
	call CDefaultInt
	mov al, 0x20
	out 0x20, al
	iret
