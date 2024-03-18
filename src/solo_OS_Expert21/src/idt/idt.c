#include <libc/idt.h>

extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_lo = base & 0xFFFF;           // Set lower 16 bits of the base address
    idt[num].base_hi = (base >> 16) & 0xFFFF;   // Set higher 16 bits of the base address
    
    idt[num].sel = sel;                         // Set segment selector
    idt[num].always0 = 0;                       // Always 0
    
    // Set flags
    idt[num].flags = flags;
}

/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    idt_load();
}