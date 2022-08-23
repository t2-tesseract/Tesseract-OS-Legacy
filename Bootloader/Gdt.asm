GdtStart:
    dd 0x0
    dd 0x0

GdtCode: 
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

GdtData:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

GdtEnd:

GdtDescriptor:
    dw GdtEnd - GdtStart - 1
    dd GdtStart

CodeSegement equ GdtCode - GdtStart
DataSegement equ GdtData - GdtStart