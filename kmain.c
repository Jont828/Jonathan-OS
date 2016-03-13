#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15


#define FB_GREEN     2
#define FB_DARK_GREY 8

int sum_of_three(int arg1, int arg2, int arg3);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_move_cursor(unsigned short pos);
int write(char *buf);

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

int write(char *buf)
{
    if(!buf)
        return 1;

    int start = 20;

    // fb_move_cursor(start);
    // fb_write_cell(start * 2, 'A', 7, 0);
    int i = 0;
    while(*(buf + i) != '\0') {
        fb_write_cell( (start + i) * 2, buf[i], 7, 0); // changed to ( start + i ) * 2
        fb_move_cursor(start + i);
        i++;
    }

    return 0;
}


int main(void)
{
	// fb_write_cell(0, 'A', 7, 0);
    // fb_write_cell(20, 'B', 7, 0);
	// fb_move_cursor(10);
    // fb_move_cursor(20);

    char text[] = "Hello, kernel world!";
    write(text);

	return 0;
}
