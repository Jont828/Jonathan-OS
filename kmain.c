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

    // puts("Booting OS from Bochs:\n");
        

    int i;

    char *c1 = "Hello";
    char *c2 = "Hello world!";
    char c3[] = "Helloings";
    char c4[64];

    puts("Type something here: ");
    char c;
    while((c = getchar()) != '0') {
        if(c == '\n') {
            puts("Type something here: ");
        }
    }

    // puts("Done!");

    // serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
