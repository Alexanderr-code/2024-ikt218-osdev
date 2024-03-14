#include "stdint.h"
#pragma once

//Defineing the structure of the gdt
struct gdt_entry_structure
{
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granurality; //flags
    uint8_t base_high;
}__attribute__((packed));// the memory is defined the way we defined it in the struct, it will not allocate more memmory

//A pointer structure
struct gdt_ptr_struct
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

//Defineing the functions
void initGdt(); // initializing the gdt
void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran); //define the entries in the gdt
