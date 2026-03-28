#include "shell.h"
#include "terminal.h"

#define CMD_BUFFER_SIZE 256

static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;

static void shell_prompt(void) {
    terminal_writestring_colored("ZenithOS", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    terminal_writestring_colored("> ", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
}

static void shell_execute(void) {
    // compare command
    auto strcmp = [](const char* a, const char* b) {
        while (*a && *b && *a == *b) { a++; b++; }
        return *a - *b;
    };

    if (strcmp(cmd_buffer, "help") == 0) {
        terminal_writestring_colored("Commands:\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
        terminal_writestring("  help    - show this message\n");
        terminal_writestring("  clear   - clear the screen\n");
        terminal_writestring("  echo    - echo back your input\n");
        terminal_writestring("  about   - about ZenithOS\n");
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

void shell_init(void) {
    shell_prompt();
}
