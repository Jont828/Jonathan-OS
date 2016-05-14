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
    __asm__ __volatile__ ("sti");

    puts("Booting Jonathan's OS from Bochs:\n\n");
    
    char buffer[1024];

    while(1) {
        puts("Enter a command: ");
        getline(buffer, 1024);

        // puts("You typed \'");
        // puts(buffer);
        // puts("\'\n");

        if(!strcmp(buffer, "exit") || !strcmp(buffer, "logout")) {
            puts("OS shutting down.");
            break;
        } else if(!strcmp(buffer, "clear")) {
            cls();
        } else if(!strcmp(buffer, "edit")) {
            editor();
        } else if(!strcmp(buffer, "hello")) {
            puts("Hi! How are you?\n");
        } else if(!strcmp(buffer, "whoami")) {
            puts("I dunno, you tell me\n");
        } else if(!strlen(buffer)) { /* strlen(buffer) == 0 */
            puts("Please enter a command\n");
            /* Do nothing if no command is entered */
        } 
        else {
            putch('\'');
            puts(buffer);
            puts("\' is an unrecognized command\n");
        }
    }

    keyboard_uninstall();

    // do {
    //     puts("Type something here: ");
    //     getline(buffer, 1024);
    //     puts(buffer);
    //     putch('\n');
    // } while(strcmp(buffer, "exit"));

    puts("\nGoodbye!");

    // serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
