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
    // memset(raw_buffer, '\0', 1024);
    char c;

    while(1) {
        puts("Enter a command: ");
        getline(buffer, 1024);

        if(!strcmp(buffer, "exit")) {
            puts("OS shutting down.");
            break;
        } else if(!strcmp(buffer, "hello")) {
            puts("Hi! How are you?\n");
        } else if(!strcmp(buffer, "")) {
            // puts("Please enter a command\n");

            /* Do nothing if no command is entered */
        } 
        else {
            putch('\'');
            puts(buffer);
            puts("\' is an unrecognized command\n");
        }
    }

    // do {
    //     puts("Type something here: ");
    //     getline(buffer, 1024);
    //     puts(buffer);
    //     putch('\n');
    // } while(strcmp(buffer, "exit"));

    puts("\nDone!");

    // serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
