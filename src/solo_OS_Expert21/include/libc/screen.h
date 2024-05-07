#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"
#include "stdint.h"


// Write a single character out to the screen.
void printf_put(char c);

// Clear the screen to all black.
void printf_clear();

//declearing the move_cursor
void move_cursor();

// Output a null-terminated ASCII string to the monitor.
void printf_string(char *c);

void printf(char *c, ...);
void int_to_string(char* str, int num);
void float_to_string(char* str, float f, int precision);
void print_hex_digit(uint32_t digit);
void print_write_hex(uint32_t n);
void print_write_dec(uint32_t n);
void print_dec(uint32_t n);
void handle_backspace();




#endif // MONITOR_H