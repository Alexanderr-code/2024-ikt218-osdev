#include "libc/keyboard.h"
#include "libc/interupts.h"
#include "libc/common.h"
#include "libc/screen.h"
#include "libc/isrs.h"


char scan_code_chart[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int get_scancode()
{
    //Initializes the variable scancode to 0.
    int scancode = 0;

    //Enters an infinite loop.
    while(1)
    {
        //Keeps checking if the keyboard has sent a scancode by checking the first bit of the keyboard status port.
        if((inb(KEYBOARD_STATUS_PORT) & 1) != 0)
        {
            //When a scancode has been recieved it stores it in the scancode variable and exits the function.
            scancode = inb(KEYBOARD_DATA_PORT);
            break;
        }
    }

    //Returns the scancode.
    return scancode;
}

uint8_t input_buffer_occupancy = 0;
char input_buffer[INPUT_BUFFER_SIZE+1];


void input_buffer_backspace() {
  if (input_buffer_occupancy > 0) {
    input_buffer_occupancy -= 1;
    input_buffer[input_buffer_occupancy] = 0;
  }
}

void keyboard_handler(registers_t regs)
{
    int scancode;
    char ch = 0;

    scancode = get_scancode();
    if (scancode & 0x80)
    {
        // Key release handling (optional)
        // Ignore key releases
        return;
    }
    else
    {
        // Handle regular keypress
        ch = scan_code_chart[scancode];
    }

    // Handle backspace
    if (scancode == BACKSPACE) {
        input_buffer_backspace();
        handle_backspace();
        return;
    }

    // Print the character if it's not a special key
    printf_put(ch);
}

void init_keyboard(){
        register_interrupt_handler(IRQ1, keyboard_handler);
}