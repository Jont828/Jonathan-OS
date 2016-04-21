#include "system.h"

struct gdt {
    unsigned int address;
    unsigned short size;
} __attribute__((packed));

void kmain(void)
{
    init_video();
    settextcolor(7, 0);

    char text[] = "Hello, kernel world!";
    char serial_text[] = "Hello, serial ports!\n";

    // gdt_install();
    // idt_install();

    // int i = 10;
    // putch(i);
    // char c = 'a';
    puts(text);
    serial_puts(SERIAL_COM1_BASE, serial_text);
}
