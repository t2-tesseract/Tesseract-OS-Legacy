[bits 16]
SwitchProtectedMode:
    cli
    lgdt [GdtDescriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CodeSegement:InitProtectedMode

[bits 32]
InitProtectedMode:
    mov ax, DataSegement
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call BeginProtectedMode