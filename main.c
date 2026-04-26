#include "strlib_c.h"
#include <stdio.h>

int main() {
    string* str = str_init("Hello, world!\n");
    str->print(str);
    //str = str_init("Hello, world1!\n"); // !! causes memory leak
    str->del(str); // correct way to do it
    str = str_init("Hello, world1!\n");
    str->print(str);
    string* compare = str_init("Hello");
    int result = str->startswith(str, compare);
    printf("str '%s' startswith '%s': %d", str->data, compare->data, result);
    return 0;
}