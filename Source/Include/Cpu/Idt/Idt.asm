global LoadIdt
extern KernelIdtPointer
LoadIdt:
    lidt [KernelIdtPointer]
    ret