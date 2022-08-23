[org 0x7c00]                        
KernelLocation equ 0x1000
                                    

mov [BootDrive], dl                 
             
xor ax, ax
mov es, ax
mov ds, ax
mov bp, 0x8000
mov sp, bp

mov bx, KernelLocation
mov dh, 2

mov ah, 0x02
mov al, dh 
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BootDrive]
int 0x13
      
mov ah, 0x0
mov al, 0x3
int 0x10

CodeSegement equ GdtCode - GdtStart
DataSegement equ GdtData - GdtStart

cli
lgdt [GdtDescriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CodeSegement:MainProtectedMode

jmp $
                                    
BootDrive: db 0

GdtStart:
    GdtNull:
        dd 0x0
        dd 0x0

    GdtCode:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GdtData:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GdtEnd:

GdtDescriptor:
    dw GdtEnd - GdtStart - 1
    dd GdtStart


[bits 32]
MainProtectedMode:
    mov ax, DataSegement
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000
	mov esp, ebp

    jmp KernelLocation

times 510-($-$$) db 0              
dw 0xaa55