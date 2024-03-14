#include "libc/gdt.h"

struct gdt_entry_structure gdt_enteries[5];
struct gdt_ptr_struct gp;

extern void gdt_flush(uint32_t gp);

void initGdt(){
    gp.limit = (sizeof(struct gdt_entry_structure) *5)-1;
    gp.base = (uint32_t)&gdt_enteries;

    setGdtGate(0,0,0,0,0); //Null segment
    setGdtGate(1,0,0xFFFFFFFF,0x9A,0xCF); //kernel code segment
    setGdtGate(2,0,0xFFFFFFFF,0x92,0xCF); //kernel data segment
    setGdtGate(3,0,0xFFFFFFFF,0xFA,0xCF); //User code segment
    setGdtGate(4,0,0xFFFFFFFF,0xF2,0xCF); //Kernel code segment

    gdt_flush((uint32_t)&gp);
}


void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
    // Setup for the descriptor base adress
    gdt_enteries[num].base_low = (base & 0xFFFF);
    gdt_enteries[num].base_middle = (base >> 16 )& 0xFF;
    gdt_enteries[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt_enteries[num].limit = (limit & 0xFFFF);
    gdt_enteries[num].granurality = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt_enteries[num].granurality |= (gran & 0xF0);
    gdt_enteries[num].access = access;
}