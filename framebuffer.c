/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
// void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
// {
// 	char *fb = (char *) 0x000B8000;
//     fb[i] = c;
//     fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
// }

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
// void fb_move_cursor(unsigned short pos)
// {
//     outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
//     outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
//     outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
//     outb(FB_DATA_PORT,    pos & 0x00FF);
// }

// int fb_write(char *buf, int pos)
// {
//     if(!buf)
//         return 1;

//     // fb_move_cursor(pos);
//     // fb_write_cell(pos * 2, 'A', 7, 0);
//     // int i = 0;
//     // while(*(buf + i) != '\0') {
//     //     fb_write_cell( (pos + i) * 2, buf[i], BLACK, LIGHT_GREY); // changed to ( pos + i ) * 2
//     //     fb_move_cursor(pos + i + 1); // cursor is at the space for the next character
//     //     i++;
//     // }

//     int i = 0;
//     int len = strlen(buf);
//     for(i=0; i<len; i++) {
//         fb_write_cell( (pos + i) * 2, buf[i], BLACK, LIGHT_GREY); // changed to ( pos + i ) * 2
//         fb_move_cursor(pos + i + 1); // cursor is at the space for the next character
//     }

//     return 0;
// }

// int fb_write_num(int num, int pos, int base)
// {
//     char buf[64];
//     itoa(num, buf, base);
//     return fb_write(buf, pos);
// }