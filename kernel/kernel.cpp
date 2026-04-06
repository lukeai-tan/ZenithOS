#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include "multiboot.h"

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
    terminal_writestring("Loading GDT...\n");

    terminal_writestring_colored("[  OK  ] ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Loading IDT...\n");

    terminal_writestring_colored("[  OK  ] ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Initializing keyboard...\n");
}

extern "C" void kernel_main(uint32_t multiboot_info) {
    terminal_initialize();
    print_banner();
    print_boot_messages();
    // terminal_writestring("Before GDT\n");
    gdt_init();
    // terminal_writestring("Before IDT\n");
    idt_init();
    // terminal_writestring("Before keyboard\n");
    keyboard_init();
    // terminal_writestring("Before timer\n");
    timer_init();
    // terminal_writestring("Before shell\n");
    shell_init((struct multiboot_info*) multiboot_info);
    
    // enable interrupts only after everything is setup
    asm volatile("sti");

    while (1) asm volatile("hlt");
}
