#include "system.h"

void kmain(void)
{
    init_video();
    settextcolor(LIGHT_GREY, BLACK);
    gdt_install();
    idt_install();

    char text[] = "Hello, kernel world!";
    char serial_text[] = "Hello, serial ports!\n";

    // int i = 10;
    // putch(i);
    // char c = 'a';
    puts(text);
    serial_puts(SERIAL_COM1_BASE, serial_text);
}
