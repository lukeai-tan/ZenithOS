TARGET = i686-elf
CC = $(TARGET)-gcc
AS = $(TARGET)-as

CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti \
         -I./kernel -I./drivers -I./cpu -I./terminal

OBJS = build/boot.o \
       build/kernel.o \
       build/terminal.o \
	   build/gdt.o \
       build/idt.o \
       build/keyboard.o \
	   build/shell.o

.PHONY: all clean run

all: myos.iso

# Boot assembly
build/boot.o: boot.s
	mkdir -p build
	$(AS) boot.s -o build/boot.o

# Kernel
build/kernel.o: kernel/kernel.cpp
	mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS)

# Terminal
build/terminal.o: terminal/terminal.cpp
	mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS)

# CPU / GDT

build/gdt.o: cpu/gdt.cpp
	mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS)

# CPU / IDT
build/idt.o: cpu/idt.cpp
	mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS)

# Drivers
build/keyboard.o: drivers/keyboard.cpp
	mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS)

build/shell.o: kernel/shell.cpp
	mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS)

# Link
myos: $(OBJS) linker.ld
	$(CC) -T linker.ld -o myos $(OBJS) -ffreestanding -O2 -nostdlib -lgcc

# ISO
myos.iso: myos
	mkdir -p isodir/boot/grub
	cp myos isodir/boot/myos
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run: myos.iso
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -f myos myos.iso
	rm -rf build isodir
