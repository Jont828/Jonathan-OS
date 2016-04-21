#ifndef __SYSTEM_H
#define __SYSTEM_H

/* MAIN.C */
void *memcpy(void *dest, const void *src, int /*size_t*/ count);
void *memset(void *dest, char val, int /*size_t*/ count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int /*size_t*/ count);
int strlen(char *text);

/* MAIN.C */

/* SCRN.C */

void scroll(void);
void move_csr(void);
void cls();
void putch(unsigned char c);
void puts(char *text);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
void init_video(void);

/* SCRN.C */

/* GDT.C */

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install(); // To be called by main()

/* GDT.C */

/* IDT.C */

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

/* IDT.C */

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void isrs_install(); // To be called in main()

#endif