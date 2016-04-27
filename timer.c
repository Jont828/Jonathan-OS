#include "system.h"

/* This will keep track of how many ticks that the system
*  has been running for */
int timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */


/* HAD TO REMOVE THE ARGUMENT FROM TIMER_HANDLER BECAUSE OF UNUSED VARIABLE */
void timer_handler(struct regs *r)
// void timer_handler()
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0)
    {;
        puts("\nOne second has passed\n");
    }
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
}

/* OS DEV'S VERSION OF TIMER_WAIT 
 * BRAN'S VERSION DOESN'T WORK */ 

void timer_wait(int ticks)
{
    // unsigned int eticks;
    int eticks;
 
    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks) 
    { 
        // __asm__ __volatile__ ("sti//hlt//cli");
        __asm__ __volatile__ ("sti");
        __asm__ __volatile__ ("hlt");
        __asm__ __volatile__ ("cli");
    }
}


/* BRAN'S VERSION OF TIMER_WAIT */

//This will continuously loop until the given time has
// been reached 
// void timer_wait(int ticks)
// {
//     unsigned long eticks;

//     eticks = timer_ticks + ticks;
//     while(timer_ticks < eticks);
// }