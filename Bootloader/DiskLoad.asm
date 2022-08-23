DiskLoad:
    pusha
    push dx

    mov ah, 0x02
    mov al, dh
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    int 0x13
    jc DiskError

    pop dx
    cmp al, dh
    jne SectorError
    popa
    ret


DiskError:
    jmp DiskLoop

SectorError:
    jmp DiskLoop

DiskLoop:
    jmp $

