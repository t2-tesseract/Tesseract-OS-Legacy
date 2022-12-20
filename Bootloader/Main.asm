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