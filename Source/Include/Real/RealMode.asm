global WriteCr0, ReadCr0

ReadCr0:
        mov     eax, cr0        ; read CR0 to eax
        mov     edx, eax
        shr     edx, 16         ; dx:ax = CR0 (return value)
        retn

WriteCr0:
        push    bp
        mov     bp, sp
        mov     eax, [ss:bp+4]  ; eax = 32-bit parameter
        mov     cr0, eax
        pop     bp
        retn