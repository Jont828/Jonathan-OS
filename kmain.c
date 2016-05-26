#include "system.h"

void kmain(void)
{
    init_video();
    settextcolor(LIGHT_GREY, BLACK);
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    timer_install();
    keyboard_install();

    /* Initializes keyboard */
    __asm__ __volatile__ ("sti"); /* Set interrupts */

    puts("Booting Jonathan's OS from Bochs:\n\n");

    start_terminal();
 
        
    // __asm__ __volatile__ ("cli"); /* Clear interrupts */
    irq_uninstall_handler(1); /* Uninstalls keyboard */

    puts("\nGoodbye!");

    serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
