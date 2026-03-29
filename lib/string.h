#pragma once
#include <stddef.h>

int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t n);
size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
void itoa(int num, char* buf, int base);
