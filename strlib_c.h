#pragma once
#include <stdint.h>

typedef struct string {
    uint32_t len;
    char* data;

    uint32_t _m_capacity;
    uint8_t _in_use;

    void (*print)(struct string* self);
    void (*del)(struct string* self);
    int (*startswith)(struct string* self, struct string* str);
} string;

string* str_init(char* inits);

void strcpy_s(string* a, string* b);
void strprint_s(string* self);
void strdel_s(string* self);
int startswith_s(string* self, string* str);
string* itoa_s(int n);