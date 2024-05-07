#include <libc/screen.h>
#include <libc/stdarg.h>
#include <libc/stdint.h>

// In screen.h or another relevant header file
int cursor_x;
int cursor_y;

u16int *video_memory = (u16int *)0xB8000;

void move_cursor()
{
   // The screen is 80 characters wide...
   u16int cursorLocation = cursor_y * 80 + cursor_x;
   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{

   // Get a space character with the default colour attributes.
   u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16int blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(cursor_y >= 25)
   {
       // Move the current text chunk that makes up the screen
       // back in the buffer by a line
       int i;
       for (i = 0*80; i < 24*80; i++)
       {
           video_memory[i] = video_memory[i+80];
       }

       // The last line should now be blank. Do this by writing
       // 80 spaces to it.
       for (i = 24*80; i < 25*80; i++)
       {
           video_memory[i] = blank;
       }
       // The cursor should now be on the last line.
       cursor_y = 24;
   }
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
{
   // Make an attribute byte for the default colours
   u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16int blank = 0x20 /* space */ | (attributeByte << 8);

   int i;
   for (i = 0; i < 80*25; i++)
   {
       video_memory[i] = blank;
   }

   // Move the hardware cursor back to the start.
   cursor_x = 0;
   cursor_y = 0;
   move_cursor();
}

void printf_put(char c)
{
   // The background colour is black (0), the foreground is white (15).
   u8int backColour = 0;
   u8int foreColour = 15;

   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   u16int attribute = attributeByte << 8;
   u16int *location;

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && cursor_x)
   {
       cursor_x--;
   }

   // Handle a tab by increasing the cursor's X, but only to a point
   // where it is divisible by 8.
   else if (c == 0x09)
   {
       cursor_x = (cursor_x+8) & ~(8-1);
   }

   // Handle carriage return
   else if (c == '\r')
   {
       cursor_x = 0;
   }

   // Handle newline by moving cursor back to left and increasing the row
   else if (c == '\n')
   {
       cursor_x = 0;
       cursor_y++;
   }
   // Handle any other printable character.
   else if(c >= ' ')
   {
       location = video_memory + (cursor_y*80 + cursor_x);
       *location = c | attribute;
       cursor_x++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (cursor_x >= 80)
   {
       cursor_x = 0;
       cursor_y ++;
   }
   scroll();
   move_cursor();
}

void handle_backspace() {
    cursor_x--;  // Move cursor back one position
    // Clear the character at the new cursor position
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);
    u16int *location = video_memory + (cursor_y * 80 + cursor_x);
    *location = blank;  // Set the character to space with default attributes
    move_cursor();  // Move cursor to the new position
}


void printf_clear()
{
   // Make an attribute byte for the default colours
   u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   u16int blank = 0x20 /* space */ | (attributeByte << 8);

   int i;
   for (i = 0; i < 80*25; i++)
   {
       video_memory[i] = blank;
   }

   // Move the hardware cursor back to the start.
   cursor_x = 0;
   cursor_y = 0;
   move_cursor();
}

void printf_string(char *c)
{
   int i = 0;
   while (c[i])
   {
       printf_put(c[i++]);
   }
}

void printf(char* str, ...){
    va_list args;
    va_start(args, str);

    while (*str != '\0')
    {
        if (*str != '%')
        {
            printf_put(*str);
            str++;

            continue;
        }
        
        str++; 

        switch (*str)
        {
            case '%':
            {
                printf_put(*str);
                str++; 
                break;
            }

            case 'c':
            {
                char c = (char)va_arg(args, int);
                printf_put(c);
                str++;
                break;
            }

            case 's':
            {
                char* s = va_arg(args, const char*);
                while (*s != '\0')
                {
                    printf_put(*s);
                    *s++;
                }
                str++;
                break;
            }

            case 'd': 
            {
                int num = va_arg(args, int);
                char nstr[50];

                int_to_string(nstr, num);
                char *ptr = nstr;
            
                while (*ptr != '\0')
                {
                    printf_put(*ptr);
                    *ptr++;
                }

                str++;
                break;
            }

            /*case 'k':
            {
                int n = va_arg(args, int);
                char buffer[100]; 

                int_to_ascii(n,buffer);

                char *ptr = buffer;

                while (*ptr != '\0')
                {
                    printf_put(*ptr);
                    *ptr++;
                }

                str++;
                break;
                
            }*/
        

            case 'f': 
            {
                float f = va_arg(args, double);
                char nstr[50];

                float_to_string(nstr, f, 5);

                char *ptr = nstr;

                while (*ptr != '\0')
                {
                    printf_put(*ptr);
                    *ptr++;
                }

                str++;
                break;
            }

            case 'x':
            {
                // Get the next argument from the va_list as an unsigned integer.
                int num = va_arg(args, int);
                // Calculate the number of digits in the hexadecimal representation.
                int num_digits = 0;
                int temp = num;
                do 
                {
                    num_digits++;
                    temp /= 16;
                } 
                while (temp != 0);
                // Create a buffer to store the hexadecimal representation of the number.
                char hex_buffer[9] = {0};
                // Convert each digit of the number to a hexadecimal character.
                for (int i = num_digits - 1; i >= 0; i--) 
                {
                    int digit = (num >> (4 * i)) & 0xF;
                    if (digit < 10) 
                    {
                        hex_buffer[num_digits - i - 1] = '0' + digit;
                    } 
                    else 
                    {
                        hex_buffer[num_digits - i - 1] = 'a' + (digit - 10);
                    }
                }
                // Print the hexadecimal string to the terminal.
                for (int i = 0; i < num_digits; i++) 
                {
                    printf_put(hex_buffer[i]);
                }
                // Move on to the next character in the format string.
                str++;
                break;
            }
        }                    
    }
    va_end(args);
}

//Function that turns an integer into a string.
void int_to_string(char* str, int num)
{
    //Sets up the variable length that keeps track of the number of digits, and a variable temp that is equal to the num parameter.
    int end_index = 0;
    int temp = num;

    //If the number is 0 turn the string to 0 and return.
    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    //While loop that gets the number of digits in the integer and stores it in the digits variable.
    while (temp != 0) 
    {
        end_index++;
        temp /= 10;
    }

    //If the number is negative add a minus at the start of the string and turn the number positive. Also increment the end_index variable.
    if (num < 0) 
    {
        str[0] = '-';
        num = -num;
        end_index++;
    }

    //Set the variable length as the number of digits -1, a.k.a as the length of the number.
    int length = end_index - 1;

    //While the num varieble is not 0
    while (num != 0) 
    {
        //Calculates the remainder of num divided by 10, which gives the rightmost digit of num.
        //Then adds the ASCII value of the character '0' to the remainder to convert it to the corresponding ASCII character.
        str[length--] = num % 10 + '0';
        
        //Updates num by dividing it by 10, which effectively removes the rightmost digit of num.
            num /= 10;
    }

    //Set the character at the end of the string as \0.
    str[end_index] = '\0';
}




void float_to_string(char* str, float f, int precision) 
{

    int integer_part = (int) f;

    float fractional_part = f - integer_part;  

    int power = 1;  

    if (f < 0) 
    {
        str[0] = '-';
        str++;
    }

    for (int p = 0; p < precision; p++) 
    {
        power *= 10;
    }

    int i = 0;


    if (integer_part == 0) 
    {
        str[i++] = '0';
    } 

    else 
    {

        while (integer_part != 0) 
        {

            int rem = integer_part % 10;

            str[i++] = rem + '0';

            integer_part /= 10;
        }
   }  

    for (int j = 0, k = i - 1; j < k; j++, k--) 
    {
        char temp = str[j];

        str[j] = str[k];

        str[k] = temp;
    }

    if (fractional_part > 0 && precision > 0) 
    {
        str[i++] = '.';

        int fractional_part_integer = (int) (fractional_part * power + 0.5);

        while (precision > 0) 
        {
            power /= 10;
            int digit = fractional_part_integer / power;

            str[i++] = digit + '0';

            fractional_part_integer -= digit * power;

            precision--;
        }
    }

    str[i] = '\0';
}

/*// Helper function to print a hexadecimal digit
void print_hex_digit(uint32_t digit) {
    if (digit < 10) {
        printf_put('0' + digit);
    } else {
        printf_put('A' + digit - 10);
    }
}

// Function to print a 32-bit number in hexadecimal
void print_write_hex(uint32_t n) {
    printf_put('0');
    printf_put('x');
    for (int i = 28; i >= 0; i -= 4) {
        uint32_t digit = (n >> i) & 0xF; // Extract the digit
        if (digit != 0 || i == 0) { // Avoid leading zeros, except for the number 0
            print_hex_digit(digit);
        }
    }
}

void print_dec(uint32_t n) {
    if (n >= 10) {
        print_dec(n / 10);
    }
    printf_put('0' + n % 10);
}

// Function to print a 32-bit number in decimal
void print_write_dec(uint32_t n) {
    if (n == 0) {
        // Handle 0 explicitly
        printf_put('0');
        return;
    }
    print_dec(n);
}*/