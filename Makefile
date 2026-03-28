TARGET = i686-elf
CC = $(TARGET)-gcc
AS = $(TARGET)-as
LD = $(TARGET)-ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

OBJS = boot.o kernel.o

.PHONY: all clean run

all: myos.iso

boot.o: boot.s
	$(AS) boot.s -o boot.o

kernel.o: kernel.cpp
	$(CC) -c kernel.cpp -o kernel.o $(CFLAGS)

myos: $(OBJS) linker.ld
	$(CC) -T linker.ld -o myos $(OBJS) -ffreestanding -O2 -nostdlib -lgcc

myos.iso: myos
	mkdir -p isodir/boot/grub
	cp myos isodir/boot/myos
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run: myos.iso
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -f *.o myos myos.iso
	rm -rf isodir
