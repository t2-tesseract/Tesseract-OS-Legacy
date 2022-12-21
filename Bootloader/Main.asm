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

; VBE info block
VBE_INFO_BLOCK db 256 dup (0)

; VBE controller info block
VBE_CONTROLLER_INFO db 256 dup (0)

; VBE mode info block
VBE_MODE_INFO db 256 dup (0)

; Function prototypes
GetVBEInfoBlock    equ 0x4F00
GetVBEControllerInfo equ 0x4F01
GetVBEModeInfo     equ 0x4F01
SetVBEMode         equ 0x4F02


; Initialize the VBE
InitializeVBE:
    ; Set up the VBE info block
    mov ax, GetVBEInfoBlock
    mov di, VBE_INFO_BLOCK
    int 0x10
    cmp ax, 0x4F
    jne .error

    ; Set up the VBE controller info block
    mov ax, GetVBEControllerInfo
    mov di, VBE_CONTROLLER_INFO
    int 0x10
    cmp ax, 0x4F
    jne .error

    ; Set up the VBE mode info block
    mov ax, GetVBEModeInfo
    mov di, VBE_MODE_INFO
    int 0x10
    cmp ax, 0x4F
    jne .error

    ret
.error:
    ; Handle error
    ...

times 510 - ($-$$) db 0
dw 0xaa55