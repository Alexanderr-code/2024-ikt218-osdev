#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"
#include <libc/gdt.h>
#include <multiboot2.h>
#include "libc/common.h"
#include <libc/screen.h>




struct multiboot_info {
    uint32_t size;
    uint32_t reserved;
    struct multiboot_tag *first;
};

int kernel_main();



int main(uint32_t magic, struct multiboot_info* mb_info_addr) {
    
    initGdt();
    //idt_install();
    //idt_set_gate();
    printf_string("Hello World!\n");

    int num = 15;
    char* string1 = "Hello world!";
    char c = 'A';
    float flt = 2.33333;
    printf("This is a number: %d, %s, Im gonna get the grade %c, here is a percentage sign: %%, this is a float: %f uhuhu", num, string1, c, flt);

    // Call cpp kernel_main (defined in kernel.cpp) 
    
    return kernel_main();
}