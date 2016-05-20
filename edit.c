#include "system.h"

void editor(char *editor_buffer, int lim) {
	cls();
	puts("Type some stuff here (press ESC to exit):\n");

	int len = edit_get_text(editor_buffer, lim);
	cls();

	// return len;
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