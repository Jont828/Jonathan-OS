#include "system.h"

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
    outportb(SERIAL_LINE_COMMAND_PORT(com),
         SERIAL_LINE_ENABLE_DLAB);
    outportb(SERIAL_DATA_PORT(com),
         (divisor >> 8) & 0x00FF);
    outportb(SERIAL_DATA_PORT(com),
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
     
    outportb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
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
    return inportb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}


void serial_putch(unsigned int com, char c)
{
    serial_configure_baud_rate(com, 3);
    serial_configure_line(com);

    while(!serial_is_transmit_fifo_empty(com))
        ;

    outportb(SERIAL_DATA_PORT(com), c);
}

void serial_puts(unsigned int com, char *buffer)
{
    serial_configure_baud_rate(com, 3);
    serial_configure_line(com);

    while(!serial_is_transmit_fifo_empty(com))
        ;

    int len = strlen(buffer);

    int i;
    for(i=0; i<len; i++) {
        serial_putch(com, buffer[i]);
    }
}