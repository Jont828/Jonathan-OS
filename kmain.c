#include "system.h"

// struct command {
//     char name[64];
//     void (*func)();
// };

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
    // char buffer[1024];
    // char editor_buffer[VGA_WIDTH * VGA_HEIGHT];

    // while(1) {
    //     puts("Enter a command: ");
    //     getline(buffer, 1024);

    //     if(!strcmp(buffer, "exit") || !strcmp(buffer, "logout")) {
    //         puts("OS shutting down.");
    //         break;
    //     } else if(!strcmp(buffer, "clear")) {
    //         cls();
    //     } else if(!strcmp(buffer, "edit")) {
    //         editor(editor_buffer, VGA_WIDTH * VGA_HEIGHT);
    //         puts("You typed:\n\n");
    //         puts(editor_buffer);
    //         putch('\n');
    //     } else if(!strlen(buffer)) { /* strlen(buffer) == 0 */
    //         // puts("Please enter a command\n");
    //         /* Do nothing if no command is entered */
    //     } 
    //     else {
    //         putch('\'');
    //         puts(buffer);
    //         puts("\' is an unrecognized command\n");
    //     }
    // }

    // __asm__ __volatile__ ("cli"); /* Clear interrupts */
    irq_uninstall_handler(1); /* Uninstalls keyboard */

    puts("\nGoodbye!");

    // serial_puts(SERIAL_COM1_BASE, "Hello, serial port world!\n");
}
