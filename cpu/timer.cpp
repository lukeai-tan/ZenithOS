#include "timer.h"
#include "idt.h"
#include "ports.h"

extern "C" void timer_isr(void);

#define PIT_FREQUENCY  100      // 100 ticks per second
#define PIT_COMMAND    0x43
#define PIT_CHANNEL0   0x40

static uint32_t ticks = 0;

extern "C" void timer_handler(void) {
    ticks++;
    outb(0x20, 0x20);   // EOI
}

uint32_t timer_get_ticks(void) {
    return ticks;
}

uint32_t timer_get_seconds(void) {
    return ticks / PIT_FREQUENCY;
}

void timer_init(void) {
    // configure PIT to fire at 100hz
    uint32_t divisor = 1193180 / PIT_FREQUENCY;
    outb(PIT_COMMAND,  0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);

    // register IRQ0 handler (int 0x20)
    idt_set_gate(0x20, (uint32_t)timer_isr, 0x08, 0x8E);
}
