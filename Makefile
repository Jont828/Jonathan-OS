OBJECTS = loader.o system.o utilities.o kmain.o scrn.o serial.o gdt.o idt.o isrs.o irq.o timer.o kb.o edit.o terminal.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c -fno-pie #-Werror -c
LDFLAGS = -T link.ld -melf_i386 --ignore-unresolved-symbol _GLOBAL_OFFSET_TABLE_
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

fresh: clean kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                      \
		-b boot/grub/stage2_eltorito    \
		-no-emul-boot                   \
		-boot-load-size 4               \
		-A os                           \
		-input-charset utf8             \
		-quiet                          \
		-boot-info-table                \
		-o os.iso                       \
		iso

run: os.iso
	-bochs -f .bochsrc -q
	$(MAKE) clean

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso
