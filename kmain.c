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
        

    int i = 0;
    char raw_buffer[1024], buffer[1024];
    memset(raw_buffer, '\0', 1024);
    char c;

    puts("Type something here: ");
    while((raw_buffer[i] = getchar()) != '0') {
        if(raw_buffer[i] == '\n') {
            //puts(raw_buffer);
            puts("Raw:\t\t");
            put_buffer(raw_buffer);
            process_buffer(buffer, raw_buffer);
            puts("Processed:\t");
            put_buffer(buffer);

            memset(raw_buffer, '\0', 1024);

            puts("Type something here: ");
            i=0;
        } else {
            i++;
        }
    }

    // puts("Done!");

    // serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
