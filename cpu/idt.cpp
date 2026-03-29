#include "idt.h"
#include "ports.h"
#include <stdint.h>

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

extern "C" void idt_load(uint32_t);  // defined in asm
extern "C" void default_isr(void);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

void idt_init(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;
    
    // zero the IDT
    for (int i = 0; i < IDT_ENTRIES; i++)
        idt_set_gate(i, 0, 0, 0);
    
    idt_load((uint32_t)&idtp);
}

extern "C" void default_handler(void) {
    // do nothing, just acknowledge
    outb(0x20, 0x20);  // send EOI to PIC
}
