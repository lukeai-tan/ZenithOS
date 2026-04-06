#include "shell.h"
#include "terminal.h"
#include "ports.h"
#include "timer.h"
#include "string.h"
#include "multiboot.h"

#define CMD_BUFFER_SIZE 256

static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;
static struct multiboot_info* mboot;

static void shell_prompt(void) {
    terminal_writestring_colored("ZenithOS", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    terminal_writestring_colored("> ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
}

static void shell_execute(void) {
    if (strcmp(cmd_buffer, "help") == 0) {
        terminal_writestring_colored("Commands:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
        terminal_writestring("  help    - show this message\n");
        terminal_writestring("  clear   - clear the screen\n");
        terminal_writestring("  echo    - echo back your input\n");
        terminal_writestring("  about   - about ZenithOS\n");
        terminal_writestring("  uptime  - show time since boot\n");
        terminal_writestring("  reboot  - reboot the system\n");
        terminal_writestring("  halt    - halt the system\n");
        terminal_writestring("  mem     - show memory information\n");
    } else if (strcmp(cmd_buffer, "clear") == 0) {
        terminal_initialize();
    } else if (strcmp(cmd_buffer, "about") == 0) {
        terminal_writestring_colored("ZenithOS\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        terminal_writestring("A hobby OS built from scratch.\n");
    } else if (cmd_buffer[0] == 'e' && cmd_buffer[1] == 'c' &&
               cmd_buffer[2] == 'h' && cmd_buffer[3] == 'o' &&
               cmd_buffer[4] == ' ') {
        terminal_writestring(cmd_buffer + 5);
        terminal_putchar('\n');
    } else if (cmd_buffer[0] == '\0') {
        // empty command, just show prompt again
    } else if (strcmp(cmd_buffer, "reboot") == 0) {
        terminal_writestring_colored("Rebooting...\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        // pulse the keyboard controller reset line
        outb(0x64, 0xFE);
    } else if (strcmp(cmd_buffer, "halt") == 0) {
        terminal_writestring_colored("Halting system. Arrividerci!\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        asm volatile("cli");   // disable interrupts
        asm volatile("hlt");   // halt the CPU
    } else if (strcmp(cmd_buffer, "uptime") == 0) {
        char buf[32];
        uint32_t seconds = timer_get_seconds();
        uint32_t minutes = seconds / 60;
        uint32_t hours   = minutes / 60;
        seconds %= 60;
        minutes %= 60;

        terminal_writestring("Uptime: ");
        itoa(hours, buf, 10);   terminal_writestring(buf);
        terminal_writestring("h ");
        itoa(minutes, buf, 10); terminal_writestring(buf);
        terminal_writestring("m ");
        itoa(seconds, buf, 10); terminal_writestring(buf);
        terminal_writestring("s\n");
    } else if (strcmp(cmd_buffer, "mem") == 0) {
       char buf[32];
        uint32_t lower = mboot->mem_lower;
        uint32_t upper = mboot->mem_upper / 1024;  // convert to MB

        terminal_writestring_colored("Memory Info:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
        terminal_writestring("  Lower memory: ");
        itoa(lower, buf, 10);
        terminal_writestring(buf);
        terminal_writestring(" KB\n");

        terminal_writestring("  Upper memory: ");
        itoa(upper, buf, 10);
        terminal_writestring(buf);
        terminal_writestring(" MB\n");

        uint32_t total = (lower + mboot->mem_upper) / 1024;
        terminal_writestring("  Total:        ");
        itoa(total, buf, 10);
        terminal_writestring(buf);
        terminal_writestring(" MB\n");
    } else {
        terminal_writestring_colored("Unknown command: ", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
        terminal_writestring(cmd_buffer);
        terminal_putchar('\n');
    }
}

void shell_handle_char(char c) {
    if (c == '\n') {
        terminal_putchar('\n');
        cmd_buffer[cmd_index] = '\0';
        shell_execute();
        cmd_index = 0;
        cmd_buffer[0] = '\0';
        shell_prompt();
    } else if (c == '\b') {
        if (cmd_index > 0) {
            cmd_index--;
            cmd_buffer[cmd_index] = '\0';
            terminal_putchar('\b');
        }
    } else if (cmd_index < CMD_BUFFER_SIZE - 1) {
        cmd_buffer[cmd_index++] = c;
        terminal_putchar(c);
    }
}

void shell_init(struct multiboot_info* mb) {
    mboot = mb;
    shell_prompt();
}
