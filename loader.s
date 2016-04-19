KERNEL_STACK_SIZE equ 4096                  ; size of stack in bytes

section .bss
align 4                                     ; align at 4 bytes
kernel_stack:                               ; label points to beginning of memory
	resb KERNEL_STACK_SIZE                  ; reserve stack for the kernel

global loader                   ; the entry symbol for ELF

MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                            	; (magic number + checksum + flags should equal 0)

section .text:                  ; start of the text (code) section
align 4                         ; the code must be 4 byte aligned
	dd MAGIC_NUMBER             ; write the magic number to the machine code,
	dd FLAGS                    ; the flags,
	dd CHECKSUM                 ; and the checksum

; GDT code

; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
global gdt_flush     ; Allows the C code to link to this
extern gp            ; Says that 'gp' is in another file
gdt_flush:
    lgdt [gp]        ; Load the GDT with our 'gp' which is a special pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret               ; Returns back to the C code!

; GDT code

; IDT code

; Loads the IDT defined in 'idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
  global idt_load
  extern idtp
  idt_load:
      lidt [idtp]
      ret

; IDT code


loader:                         ; the loader label (defined as entry point in linker script)
	mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
                                                ; stack (end of memory area)
	extern kmain

	call kmain

.loop:
	jmp .loop                   ; loop forever
