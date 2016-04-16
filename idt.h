void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

/* Installs the IDT */
void idt_install();