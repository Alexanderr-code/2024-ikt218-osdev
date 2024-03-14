#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write a single character out to the screen.
void printf_put(char c);

// Clear the screen to all black.
void printf_clear();

//declearing the move_cursor
void move_cursor();

// Output a null-terminated ASCII string to the monitor.
void printf_string(char *c);

void printf(char* c, ...);
void int_to_string(char* str, int num);
void float_to_string(char* str, float f, int precision);




#endif // MONITOR_H