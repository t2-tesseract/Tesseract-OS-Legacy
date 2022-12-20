[org 0x7c00]
KernelLocation equ 0x1000

    mov [BootDrive], dl
    mov bp, 0x9000
    mov sp, bp

    call LoadKernel
    call SwitchProtectedMode
    jmp $

%include "Bootloader/DiskLoad.asm"
%include "Bootloader/Gdt.asm"
%include "Bootloader/SwitchProtectedMode.asm"

[bits 16]
LoadKernel:
    mov ax, 0x4f01
    mov cx, 0x0118 ; Mode we want
    mov bx, 0x0800 ; Offset for the vbe structure
    mov es, bx
    mov di, 0x00
    int 0x10

    ; Make the switch
    mov ax, 0x4f02
    mov bx, 0x117
    int 0x10

    mov bx, KernelLocation
    mov dh, 54
    mov dl, [BootDrive]
    call DiskLoad
    
    ret

[bits 32]
BeginProtectedMode:
    call KernelLocation
    jmp $

BootDrive db 0

times 510 - ($-$$) db 0
dw 0xaa55