#include "system.h"

extern writable_index;

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

    __asm__ __volatile__ ("sti");

    // puts("Booting OS from Bochs:\n");
        

    int i, j = 0;

    puts("Type something here: ");
    char c;
    while((c = getchar()) != '0') {
        if(c == '\n') {
            puts("Type something here: ");

            // serial_putch(SERIAL_COM1_BASE, writable_index + '0');
            // serial_putch(SERIAL_COM1_BASE, '\n');

            // for(i=0; i<80; i++) 
            //     serial_putch(SERIAL_COM1_BASE, writable[i + j*80] + '0');

            // serial_putch(SERIAL_COM1_BASE, '\n');
            j++;
        }
    }

    // puts("Done!");

    // serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
