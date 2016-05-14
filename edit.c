#include "system.h"

void editor(void) {
	char editor_buffer[VGA_WIDTH * VGA_HEIGHT];
	cls();
	edit_get_text(editor_buffer, VGA_WIDTH * VGA_HEIGHT);
	cls();
}

int edit_get_text(char *buffer, int lim)
{
    int i=0;
    while(i < (lim-1) && (buffer[i] = getchar()) != 27) {
        if(buffer[i] == '\b') {
            buffer[i-1] = '\0';
            buffer[i] = '\0';
            i = i-1;
        } else {
            i++;
        }
    }

    buffer[i] = '\0';

    return i;
}