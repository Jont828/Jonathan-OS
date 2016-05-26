#ifndef __SYSTEM_H
	#define __SYSTEM_H
#endif	

/* SYSTEM.C */

	void *memcpy(void *dest, const void *src, int /*size_t*/ count);
	void *memset(void *dest, char val, int /*size_t*/ count);
	unsigned short *memsetw(unsigned short *dest, unsigned short val, int /*size_t*/ count);

/* SYSTEM.C */

/* UTILITIES.C -- general purpose functions I wrote that are useful for this project */

	int strlen(char *text);
	int strcmp(const char *s1, const char *s2);
	char *strcpy(char *to, const char *from);
	char *strcat(char *dest, const char *src);
	char *itoa(int num, char *str, int base);
	void reverse(char str[]);
	void putint();
	int power(int num, int power);
	int isalpha(char c); 
	void put_buffer(char *text);
	void process_buffer(char *dest, char *raw);
	char *strncpy(char *dest, const char *src, int n);
 
/* UTILITIES.C */

/* SCRN.C */

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

	#define VGA_WIDTH   80
	#define VGA_HEIGHT  25

	unsigned char inportb (unsigned short _port);
	void outportb (unsigned short _port, unsigned char _data);

	void scroll(void);
	void move_csr(void);
	void cls();
	void putch(unsigned char c);
	void puts(char *text);
	void settextcolor(unsigned char forecolor, unsigned char backcolor);
	void init_video(void);

	void save_screen_data(void);
	void restore_screen_data(void);

/* SCRN.C */

/* SERIAL.C */

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


	void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
	void serial_configure_line(unsigned short com);
	int serial_is_transmit_fifo_empty(unsigned int com);
	void serial_putch(unsigned int com, char c);
	void serial_puts(unsigned int com, char *buffer);
	void serial_putint(unsigned int com, int num);

/* SERIAL.C */

/* GDT.C */

	void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
	void gdt_install(); // To be called by main()

/* GDT.C */

/* IDT.C */

	void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
	void idt_install();

/* IDT.C */

/* ISRS.C */

	/* This defines what the stack looks like after an ISR was running */
	struct regs
	{
	    unsigned int gs, fs, es, ds;      /* pushed the segs last */
	    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
	    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
	    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
	};

	void isrs_install(); // To be called in main()
	void fault_handler(struct regs *r);

/* ISRS.C */

/* IRQ.C */

	void irq_install_handler(int irq, void (*handler)(struct regs *r));
	void irq_uninstall_handler(int irq);
	void irq_remap(void);
	void irq_install();
	void irq_handler(struct regs *r);

/* IRQ.C */

/* TIMER.C */
	void timer_handler(struct regs *r);
	// void timer_handler();
	void timer_install();
	void timer_wait(int ticks);

/* TIMER.C */

/* KB.C */

	void keyboard_handler(struct regs *r);
	void keyboard_putch(unsigned char c);
	void keyboard_install();
	int getchar();
	int getline(char *buffer, int lim);

/* KB.C */

/* EDIT.C */

	void editor(char *editor_buffer, int lim);

/* EDIT.C */

/* TERMINAL.C */

	#define MAX_CMD_LENGTH  256
	#define MAX_CMD_DESC_LENGTH 256
	#define CMD_HISTORY_LENGTH  4

	void start_terminal();

/* TERMINAL.C */