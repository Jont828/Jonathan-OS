#include "system.h"

/* keyboard_us means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */

char current = 0;
int left_shift_pressed = 0;
int right_shift_pressed = 0;
int caps_lock_on = 0;

int cmd_history_scroll_up = 0;
int cmd_history_scroll_down = 0;

extern last_writable;
extern furthest_writable;
extern csr_x;
extern csr_y;

unsigned char keyboard_us[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
	0,	/* Alt */
  ' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
  '-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
  '+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};		

unsigned char keyboard_us_shifted[128] =
{
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
  '(', ')', '-', '=', '\b', /* Backspace */
  '\t',         /* Tab */
  'Q', 'W', 'E', 'R',   /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
	0,          /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
 '\"', '~',   0,        /* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
  'M', '<', '>', '?',   0,              /* Right shift */
  '*',
	0,  /* Alt */
  ' ',  /* Space bar */
	0,  /* Caps lock */
	0,  /* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,  /* < ... F10 */
	0,  /* 69 - Num lock*/
	0,  /* Scroll Lock */
	0,  /* Home key */
	0,  /* Up Arrow */
	0,  /* Page Up */
  '-',
	0,  /* Left Arrow */
	0,
	0,  /* Right Arrow */
  '+',
	0,  /* 79 - End key*/
	0,  /* Down Arrow */
	0,  /* Page Down */
	0,  /* Insert Key */
	0,  /* Delete Key */
	0,   0,   0,
	0,  /* F11 Key */
	0,  /* F12 Key */
	0,  /* All other keys are undefined */
};  

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
// void keyboard_handler()
{
	unsigned char scancode;

	/* Read from the keyboard's data buffer */
	scancode = inportb(0x60);

	/* If the top bit of the byte we read from the keyboard is
	*  set, that means that a key has just been released */
	if (scancode & 0x80)
	{
		/* You can use this one to see if the user released the
		*  shift, alt, or control keys... */

		
		/* SCANCODE FOR RELEASING LEFT SHIFT */
		if(scancode == 170) {
			left_shift_pressed = 0;
		}
		/* SCANCODE FOR RELEASING RIGHT SHIFT */
		if(scancode == 182) {
			right_shift_pressed = 0;
		}


	} 
	else
	{
		/* Here, a key was just pressed. Please note that if you
		*  hold a key down, you will get repeated key press
		*  interrupts. */

		/* Just to show you how this works, we simply translate
		*  the keyboard scancode into an ASCII value, and then
		*  display it to the screen. You can get creative and
		*  use some flags to see if a shift is pressed and use a
		*  different layout, or you can add another 128 entries
		*  to the above layout to correspond to 'shift' being
		*  held. If shift is held using the larger lookup table,
		*  you would add 128 to the scancode when you look for it */

		// putint(scancode);
		// putch('\n');

		/* SCANCODE FOR PRESSING LEFT SHIFT */
		if(scancode == 42) {
			left_shift_pressed = 1;
		}

		/* SCANCODE FOR PRESSING RIGHT SHIFT */
		if(scancode == 54) {
			right_shift_pressed = 1;
		}

		/* SCANCODE FOR PRESSING LEFT ARROW */
		if((csr_y * 80 + csr_x) > last_writable)
			if(scancode == 75)
				csr_x--;

		/* SCANCODE FOR PRESSING RIGHT ARROW */
		if((csr_y * 80 + csr_x) < furthest_writable)
			if(scancode == 77)
				csr_x++;


		/* SCANCODE FOR PRESSING UP ARROW */
		if(scancode == 72) {
			cmd_history_scroll_up = 1;
		}

		/* SCANCODE FOR PRESSING DOWN ARROW */
		if(scancode == 80) {
			cmd_history_scroll_down = 1;
		}



		/* SCANCODES FOR PRESSING CAPS LOCK */
		if(scancode == 58) {
			// puts("User pressed caps lock!\n");
			if(caps_lock_on) {
				caps_lock_on = 0;
				// puts("caps lock off\n");
			}
			else {
				caps_lock_on = 1;
				// puts("caps lock on\n");
			}
		}

		// puts("A key was pressed");

		if(caps_lock_on) {
			if(left_shift_pressed || right_shift_pressed)
				if(isalpha(keyboard_us[scancode]))
					current = keyboard_us[scancode];
				else
					current = keyboard_us_shifted[scancode];
			else
				if(isalpha(keyboard_us[scancode]))
					current = keyboard_us_shifted[scancode];
				else
					current = keyboard_us[scancode];
		} else {
			if(left_shift_pressed || right_shift_pressed)
				current = keyboard_us_shifted[scancode];
			else
				current = keyboard_us[scancode];
		}

		keyboard_putch(current);
	}
	// putch(keyboard_us[scancode]); 
	// puts("\nRead a character!");
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install()
{
	irq_install_handler(1, keyboard_handler);
}

int getchar() {
	char temp;

	/* current is the last character entered by the keyboard and processed by keyboard_handler() */
	while(current == 0)
		;

	temp = current;
	current = 0;

	return temp;
}

/* Read characters into buffer until user presses '\n' and return length. */
int getline(char *buffer, int lim)
{
	int i=0;
	while(i < (lim-1) && (buffer[i] = getchar()) != '\n') {
		if(buffer[i] == '\b') {
			if(i != 0) {
				buffer[i-1] = '\0';
				buffer[i] = '\0';
				i = i-1;
			}
		} else {
			i++;
		}
	}

	buffer[i] = '\0';

	return i;
}