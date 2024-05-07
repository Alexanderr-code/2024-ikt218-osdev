#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/gdt.h>
#include <multiboot2.h>
#include "libc/common.h"
#include <libc/screen.h>
#include "libc/idt.h"
#include <libc/timer.h>
#include "libc/keyboard.h"
#include <libc/memory.h>

extern uint32_t end; // This is defined in arch/i386/linker.ld


struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();



int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    initGdt();
    init_idt();
    init_timer();
    //init_keyboard();

    // Initialize the kernel's memory manager using the end address of the kernel.
    init_kernel_memory(&end);

    print_memory_layout();

    // Initialize paging for memory management.
    paging_init();

    int num = 15;
    char* string1 = "Hello world!";
    char c = 'A';
    float flt = 2.33333;
    //printf("This is a number: %d, %s, Im gonna get the grade %c, here is a percentage sign: %%, this is a float: %f uhuhu", num, string1, c, flt);

    //asm volatile ("int $0x2");

    // Call cpp kernel_main (defined in kernel.cpp) 
    
    return kernel_main();
}