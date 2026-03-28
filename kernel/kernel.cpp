#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"

#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

static void print_banner(void) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    terminal_writestring("========================================\n");

    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    terminal_writestring("           Welcome to ZenithOS\n");

    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    terminal_writestring("========================================\n");

    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

static void print_boot_messages(void) {
    terminal_writestring_colored("[  OK  ] ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Initializing terminal...\n");

    terminal_writestring_colored("[  OK  ] ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Loading IDT...\n");

    terminal_writestring_colored("[  OK  ] ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Initializing keyboard...\n");
}

extern "C" void kernel_main(void) {
    terminal_initialize();
    print_banner();
    print_boot_messages();

    gdt_init();
    idt_init();
    keyboard_init();
    
    terminal_writestring_colored("\nReady.\n", VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    while (1) {
        asm volatile("hlt");
    }
}
