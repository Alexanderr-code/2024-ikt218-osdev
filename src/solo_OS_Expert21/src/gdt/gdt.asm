[global gdt_flush]

section .text
gdt_flush:
    mov eax, [esp + 4] 
    lgdt [eax]  ; Load the new GDT pointer
    mov ax, 0x10  ; Load the new data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush ; Far jump to the new code segment selector
.flush:
    ret