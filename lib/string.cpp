#include "string.h"

int strcmp(const char* a, const char* b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

int strncmp(const char* a, const char* b, size_t n) {
    while (n-- && *a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++));
    return dest;
}

void itoa(int num, char* buf, int base) {
    int i = 0;
    bool negative = false;

    if (num == 0) { buf[0] = '0'; buf[1] = '\0'; return; }

    if (num < 0 && base == 10) {
        negative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        buf[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num /= base;
    }

    if (negative) buf[i++] = '-';
    buf[i] = '\0';

    // reverse the string
    int start = 0, end = i - 1;
    while (start < end) {
        char tmp = buf[start];
        buf[start] = buf[end];
        buf[end] = tmp;
        start++; end--;
    }
}
