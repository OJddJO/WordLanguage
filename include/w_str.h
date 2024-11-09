#ifndef __W_STR_H__
#define __W_STR_H__

#include "w_alloc.h"
#include "w_type.h"

typedef struct _w_str {
    W_Type type;
    void (*destroy)(struct _w_str *);
    char *(*stringify)(struct _w_str *);
    struct _w_str *(*copy)(struct _w_str *);
    char *value;
} W_Str;

W_Str *w_str_init();
void w_str_assign(W_Str *s, char *value);
void w_str_set(W_Str *s, char *value);
char *w_str_value(W_Str *s);
char *w_str_concat(W_Str *s1, W_Str *s2);
char *w_str_slice(W_Str *s, int start, int end);
int w_str_length(W_Str *s);
void w_str_destroy(W_Str *s);
W_Str *w_str_copy(W_Str *s);
char *w_str_stringify(W_Str *s);

#endif