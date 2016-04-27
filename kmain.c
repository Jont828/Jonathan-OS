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

    __asm__ __volatile__ ("sti");

    puts("Booting OS from Bochs:\n\n");
    puts("Hello, kernel world!\n");

    char c;

    while((c = getchar()) != '\n')
        putch(c);

    // timer_wait(18);

    /* ISRC (ERROR) TEST CODE */
    // int a[3];
    // int i;
    // for(i=0; i<10; i++)
    //     a[i] = i;
    //     i = a[i];

    serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
