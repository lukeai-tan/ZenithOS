#pragma once
#include <stdint.h>

void timer_init(void);
uint32_t timer_get_ticks(void);
uint32_t timer_get_seconds(void);
