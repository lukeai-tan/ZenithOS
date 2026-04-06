#pragma once
#include "multiboot.h"

void shell_init(struct multiboot_info* mb);
void shell_handle_char(char c);
