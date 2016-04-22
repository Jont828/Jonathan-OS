#ifndef __SYSTEM_H
#define __SYSTEM_H

/* SYSTEM.C */

	void *memcpy(void *dest, const void *src, int /*size_t*/ count);
	void *memset(void *dest, char val, int /*size_t*/ count);
	unsigned short *memsetw(unsigned short *dest, unsigned short val, int /*size_t*/ count);
	int strlen(char *text);

	char *itoa(int num, char *str, int base);
	void reverse(char str[]);

/* SYSTEM.C */

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

	unsigned char inportb (unsigned short _port);
	void outportb (unsigned short _port, unsigned char _data);

	void scroll(void);
	void move_csr(void);
	void cls();
	void putch(unsigned char c);
	void puts(char *text);
	void settextcolor(unsigned char forecolor, unsigned char backcolor);
	void init_video(void);

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

#endif