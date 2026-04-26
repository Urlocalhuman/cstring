#include "strlib_c.h"
#include <stdio.h>
#include <stdlib.h>

// Source - https://stackoverflow.com/q/14476627
// Posted by Hommer Smith, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-22, License - CC BY-SA 3.0

static void _strcpy(char dest[], const char source[]) {
    int i = 0;
    while (1) {
        dest[i] = source[i];

        if (dest[i] == '\0') {
            break;
        }

        i++;
    } 
}

// Source - https://stackoverflow.com/a/1068937
// Posted by paxdiablo, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-26, License - CC BY-SA 4.0

int _numPlaces(int n) {
    if (n < 0) return _numPlaces((n == INT_MIN) ? INT_MAX : -n);
    if (n < 10) return 1;
    return 1 + _numPlaces(n / 10);
}


static int _strlen(const char *s) {
    int count = 0;
    while(*s!='\0'){
        count++;
        s++;
    }
    return count;
}

string* pool;
uint32_t pool_size = 0;
uint32_t pool_used = 0;

unsigned char init_called;

static void init() {
    if (!init_called) {
        pool = malloc(sizeof(string));
        pool_size = 1;
    }
    init_called = 1;
}

static void pool_ensure_capacity(int needed) {
    if (pool_size >= needed) return;
    
    void* ptr = 0;

    while (pool_size < needed) {
        pool_size *= 2;
    }
    ptr = realloc(pool, pool_size * sizeof(string));
    if (ptr == NULL) {
        perror("Realloc returned NULL");
        exit(1);
    }
    pool = ptr;
}

static void string_ensure_capacity(string* s, uint32_t needed) {
    if (s->_m_capacity >= needed) return;
    if (s->_m_capacity == 0) s->_m_capacity = 1;

    void* ptr = 0;

    while (s->_m_capacity < needed) {
        s->_m_capacity *= 2;
    }
    ptr = realloc(s->data, s->_m_capacity);
    if (ptr == NULL) {
        perror("Realloc returned NULL");
        exit(1);
    }
    s->data = ptr;
}

static void _strcpy_s(string* a, char* b) {
    int len = _strlen(b);
    string_ensure_capacity(a, len);
    _strcpy(a->data, b);
    a->len = len;
}

void strcpy_s(string* a, string* b) {
    string_ensure_capacity(a, b->len);
    _strcpy(a->data, b->data);
    return;
}

void strprint_s(string* self) {
    printf("%s", self->data);
}

void strdel_s(string* self) {
    if (!self->_in_use) return;
    self->_in_use = 0;
    free(self->data);
    //self->data = realloc(self->data, 1);
    self->_m_capacity = 0;
    self->len = 0;
}

int startswith_s(string* self, string* str) {
    if (self->len < str->len) return 0;
    for (int i = 0; i < str->len; i++) {
        if (self->data[i] != str->data[i]) return 0;
    }
    return 1;
}

/**
 * Creates a new string and returns a pointer to it.
 * @param[in] char array to initialise the string to
 * @param[out] string* pointer to the new string
 */
string* str_init(char* inits) {
    init();

    pool_ensure_capacity(pool_used+1);

    string* str = NULL;
    for (int i = 0; i < pool_used; i++) {
        if (pool[i]._in_use == 0) {
            str = &pool[i];
            break;
        }
    }
    if (str == NULL) {
        str = &pool[pool_used++];
        str->print = strprint_s;
        str->del = strdel_s;
        str->startswith = startswith_s;
    }
    
    str->len = 0;
    str->_m_capacity = 1;
    str->data = malloc(1);
    str->data[0] = '\0';
    str->_in_use = 1;

    _strcpy_s(str, inits);


    return str;
}

string* itoa_s(int n) {
    char* c[10];
    _itoa_s(n, c, 10, 10);
    string* s = str_init(c);
    return s;
}