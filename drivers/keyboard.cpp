#include "keyboard.h"
#include "ports.h"
#include "terminal.h"
#include "idt.h"
#include "shell.h"

extern "C" void keyboard_isr(void);

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC_EOI      0x20
#define KB_DATA_PORT 0x60

static char key_buffer[256];
static uint8_t buf_head = 0;
static uint8_t buf_tail = 0;

static const char scancode_map[] = {
    0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',0,
    '*',0,' '
};

void pic_init(void) {
    // ICW1 - initialize
    outb(0x20, 0x11);   // PIC1 command
    outb(0xA0, 0x11);   // PIC2 command

    // ICW2 - vector offsets
    outb(0x21, 0x20);   // PIC1 offset: IRQ0 = int 0x20
    outb(0xA1, 0x28);   // PIC2 offset: IRQ8 = int 0x28

    // ICW3 - cascade
    outb(0x21, 0x04);   // PIC1: IRQ2 has slave
    outb(0xA1, 0x02);   // PIC2: cascade identity

    // ICW4
    outb(0x21, 0x01);   // 8086 mode
    outb(0xA1, 0x01);

    // 11111100
    // IRQ0 (timer)
    // IRQ1 (keyboard)
    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);   // all PIC2 masked
}

extern "C" void keyboard_handler(void) {
    uint8_t scancode = inb(KB_DATA_PORT);

    // ignore key release events (break codes)
    if (scancode & 0x80) {
        outb(PIC1_COMMAND, PIC_EOI);
        return;
    }

    if (scancode < sizeof(scancode_map)) {
        char c = scancode_map[scancode];
        if (c) {
            shell_handle_char(c);
        }
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

void keyboard_init(void) {
    pic_init();
    // IRQ1 = interrupt 0x21
    idt_set_gate(0x21, (uint32_t)keyboard_isr, 0x08, 0x8E);
}

char keyboard_getchar(void) {
    while (buf_head == buf_tail);  // spin wait
    return key_buffer[buf_tail++];
}
