#include "system.h"

#include "io.h" /* io.h is implement in the section "Moving the cursor" */
#include "gdt.h" // from Bran's tutorial
#include "idt.h" // from Bran's tutorial

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

//DEFINING THE 16 GLORIOUS COLORS I HAVE TO WORK WITH

#define BLACK            0
#define BLUE             1
#define GREEN            2
#define CYAN             3
#define RED              4
#define MAGENTA          5
#define BROWN            6
#define LIGHT_GREY       7
#define DARK_GREY        8
#define LIGHT_BLUE       9
#define LIGHT_GREEN     10
#define LIGHT_CYAN      11
#define LIGHT_RED       12
#define LIGHT_MAGENTA   13
#define LIGHT_BROWN     14
#define WHITE           15


/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80



struct gdt {
    unsigned int address;
    unsigned short size;
} __attribute__((packed));



void kmain(void); // early version of main(), doesn't do argc and argv[] yet, also need to call manually in loader.s since there's no OS to call it

int sum_of_three(int arg1, int arg2, int arg3);

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_move_cursor(unsigned short pos);
int fb_write(char *buf, int pos);
int fb_write_num(int num, int pos, int base);
char *itoa(int num, char *str, int base);
void reverse(char str[]);

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void serial_write_char(unsigned int com, char c);
void serial_write(unsigned int com, char *buffer);


int sum_of_three(int arg1, int arg2, int arg3)
{
	return arg1 + arg2 + arg3;
}

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
	char *fb = (char *) 0x000B8000;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

int fb_write(char *buf, int pos)
{
    if(!buf)
        return 1;

    // fb_move_cursor(pos);
    // fb_write_cell(pos * 2, 'A', 7, 0);
    // int i = 0;
    // while(*(buf + i) != '\0') {
    //     fb_write_cell( (pos + i) * 2, buf[i], BLACK, LIGHT_GREY); // changed to ( pos + i ) * 2
    //     fb_move_cursor(pos + i + 1); // cursor is at the space for the next character
    //     i++;
    // }

    int i = 0;
    int len = strlen(buf);
    for(i=0; i<len; i++) {
        fb_write_cell( (pos + i) * 2, buf[i], BLACK, LIGHT_GREY); // changed to ( pos + i ) * 2
        fb_move_cursor(pos + i + 1); // cursor is at the space for the next character
    }

    return 0;
}

int fb_write_num(int num, int pos, int base)
{
    char buf[64];
    itoa(num, buf, base);
    return fb_write(buf, pos);
}

void reverse(char str[]) {
    int len = strlen(str);
    char c;
    int i=0;
    while(i < strlen(str) / 2) {
        c = str[i];
        str[i] = str[len-1];
        str[len-1] = c;

        i++;
        len--;
    }
}



char *itoa(int num, char *str, int base)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str);
 
    return str;
}


/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
         SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
         (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
         divisor & 0x00FF);
}

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}


void serial_write_char(unsigned int com, char c)
{
    serial_configure_baud_rate(com, 3);
    serial_configure_line(com);

    while(!serial_is_transmit_fifo_empty(com))
        ;

    outb(SERIAL_DATA_PORT(com), c);
}

void serial_write(unsigned int com, char *buffer)
{
    serial_configure_baud_rate(com, 3);
    serial_configure_line(com);

    while(!serial_is_transmit_fifo_empty(com))
        ;

    int len = strlen(buffer);

    int i;
    for(i=0; i<len; i++) {
        serial_write_char(com, buffer[i]);
    }
}

void kmain(void)
{
    char text[] = "Hello, kernel world!";
    char serial[] = "Hello, serial port world!\n";

    gdt_install();
    idt_install();

    fb_write(text, 0);
    serial_write(SERIAL_COM1_BASE, serial);
}
