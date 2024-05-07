#include <libc/common.h>
#include <libc/isrs.h>
#include <libc/timer.h>
#include <libc/screen.h>


u32int tick = 0;

static void callback_timer(registers_t regs)
{
    tick++;
    //printf("Tick: %d", tick);
    //printf("\n");
}
    

void init_timer()
{
    //register our callbackk_timer
    register_interrupt_handler(IRQ0, callback_timer);

    //divide the PIT value so that it fits in a 16-bits 
    u32int divisor = DIVIDER;

    //sends the command byte
    outb(0x43, 0x36);

    //divisor is split into lower and higher bytes and sent byte wise
    u8int lower = (u8int)(divisor & 0xFF);
    u8int higher = (u8int)((divisor>>8) & 0xFF);

    //send out the frequnecy divisor
    outb(0x40, lower);
    outb(0x40, higher);
}

//Sleep using interrupts for the specified number of milliseconds.
void sleep_interrupt(u32int milliseconds)
{
    u32int current_tick = tick;
    u32int ticks_to_wait = milliseconds * TICKS_PER_MS;
    u32int end_ticks = current_tick + ticks_to_wait;

    //Wait for the desired number of ticks to pass
    while (current_tick < end_ticks)
    {
        //Enable interrupts and hald the cpu until the next interrupt occurs.
        asm volatile(
            "sti\n\t" 
            "hlt\n\t"
        );
        //Update the current tick count.
        current_tick = tick;
    }
    
}

//Sleep using busy waiting for the specified number of milliseconds.
void sleep_busy(u32int milliseconds)
{
    u32int start_tick = tick;
    u32int ticks_to_wait = milliseconds * TICKS_PER_MS;
    u32int elapsed_ticks = 0;


    //Wait for the desired number of ticks to pass.
    while (elapsed_ticks < ticks_to_wait)
    {
        //Busy wait for the next tick to occur.
        while(tick == start_tick + elapsed_ticks)
        {
            //Do nothing (busy wait)
        }
        //Increment the elapsed tick count.
        elapsed_ticks++;
    }
    
}