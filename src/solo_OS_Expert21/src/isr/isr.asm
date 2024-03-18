global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dd isr_stub_%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep

%macro isr_err_stub 1
isr_stub_%+%1:
    call exception_handler
    iret 
%endmacro
; if writing for 64-bit, use iretq instead
%macro isr_no_err_stub 1
isr_stub_%+%1:
    call exception_handler
    iret
%endmacro

extern exception_handler
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31



/*global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
; _isr8 is typically the Double Fault Exception handler, already mentioned
global _isr9
global _isr10 ; Invalid TSS Exception
global _isr11 ; Segment Not Present
global _isr12 ; Stack-Segment Fault
global _isr13 ; General Protection Fault
global _isr14 ; Page Fault
; Note: ISR 15 is reserved and typically not used
global _isr16 ; x87 Floating-Point Exception
global _isr17 ; Alignment Check Exception
global _isr18 ; Machine Check Exception
global _isr19 ; SIMD Floating-Point Exception
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
; _isr30 and _isr31 are already mentioned


;  2: Non-maskable Interrupt
_isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

;  3: Breakpoint Exception
_isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

;  4: Overflow Exception
_isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

;  5: Bound Range Exceeded Exception
_isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

;  6: Invalid Opcode Exception
_isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

;  7: Device Not Available Exception (No Math Coprocessor)
_isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun (obsolete in modern CPUs, but included for completeness)
_isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Invalid TSS Exception
_isr10:
    cli
    push byte 0
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present
_isr11:
    cli
    push byte 0
    push byte 11
    jmp isr_common_stub

; 12: Stack-Segment Fault
_isr12:
    cli
    push byte 0
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault
_isr13:
    cli
    push byte 0
    push byte 13
    jmp isr_common_stub

; 14: Page Fault
_isr14:
    cli
    ; Page fault pushes an error code automatically
    push byte 14
    jmp isr_common_stub

; ISR stubs for 15 through 31, which include various exceptions like
; x87 Floating-Point Exception, Alignment Check, Machine Check,
; SIMD Floating-Point Exception, etc.
; Note: ISR 15 is reserved and typically not used, but we define it for completeness

_isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; Continue this pattern for ISR 16 through 31, adjusting the pushed ISR number
; appropriately. Remember, exceptions that automatically push an error code
; (like the Page Fault) do not need a dummy error code pushed

extern _fault_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, _fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
*/