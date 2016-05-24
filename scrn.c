#include "system.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */

unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

// int writable[VGA_HEIGHT*VGA_WIDTH];
int last_writable = 0;
int furthest_writable = 0;

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}


/* Scrolls the screen */
void scroll(void)
{
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);

    /* Row VGA_HEIGHT is the end, this means we need to scroll up */
    if(csr_y >= VGA_HEIGHT)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = csr_y - VGA_HEIGHT + 1;
        memcpy (textmemptr, textmemptr + temp * VGA_WIDTH, (VGA_HEIGHT - temp) * VGA_WIDTH * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (VGA_HEIGHT - temp) * VGA_WIDTH, blank, VGA_WIDTH);
        csr_y = VGA_HEIGHT - 1;
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * VGA_WIDTH + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
    unsigned blank;
    int i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = 0x20 | (attrib << 8);

    /* Sets the entire screen to spaces in our current
    *  color */
    for(i = 0; i < VGA_HEIGHT; i++)
        memsetw (textmemptr + i * VGA_WIDTH, blank, VGA_WIDTH);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_csr();

    last_writable = furthest_writable = 0;
}

/* Puts a single character typed by the keyboard on the screen */
void keyboard_putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08) {   
        /* Testing to see if the cell before is writable */
        if( (csr_y * VGA_WIDTH + csr_x) > last_writable ) {
            if(csr_x > 0) {
                csr_x--;
            } else {
                if(csr_y) { /* csr_y goes from 0-VGA_HEIGHT inclusive */
                    csr_x = VGA_WIDTH - 1; /* csr_x position goes from 0-79 inclusive */
                    csr_y--;
                }
                /* else: do nothing */
            }

            unsigned blank = 0x20 | (attrib << 8);
            memsetw (textmemptr + csr_x + csr_y * VGA_WIDTH, blank, 1);
        }
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ') /* For all normal/printable characters */
    {
        where = textmemptr + (csr_y * VGA_WIDTH + csr_x);
        *where = c | att;   /* Character AND attributes: color */
        csr_x++;

        // if( (csr_y * VGA_WIDTH + csr_x) > furthest_writable )
        //     furthest_writable++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= VGA_WIDTH)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Puts a single character on the screen */
void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08) {   
        /* Testing to see if the cell before is writable */
        if( last_writable < (csr_y * VGA_WIDTH + csr_x) ) {
            if(csr_x > 0) {
                csr_x--;
            } else {
                if(csr_y) { /* csr_y goes from 0-VGA_HEIGHT inclusive */
                    csr_x = VGA_WIDTH - 1; /* csr_x position goes from 0-79 inclusive */
                    csr_y--;
                }
                /* else: do nothing */
            }

            unsigned blank = 0x20 | (attrib << 8);
            memsetw (textmemptr + csr_x + csr_y * VGA_WIDTH, blank, 1);
        }
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ') /* For all normal/printable characters */
    {
        where = textmemptr + (csr_y * VGA_WIDTH + csr_x);
        *where = c | att;   /* Character AND attributes: color */
        csr_x++;
    }
    
    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= VGA_WIDTH)
    {
        csr_x = 0;
        csr_y++;
    }


    /* Move writable index to current cursor location */
    last_writable = csr_y * VGA_WIDTH + csr_x;

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/* Uses the above routine to output a string... */
void puts(char *text)
{
    int i;
    for(i=0; i<strlen(text); i++) {
        putch(text[i]);
    }
}

/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}