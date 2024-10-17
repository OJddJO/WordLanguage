#ifndef __W_STR_H__
#define __W_STR_H__

#include <stdlib.h>
#include <string.h>
#include "w_type.h"

typedef struct _w_str {
    W_Type type;
    char *value;
    void (*destroy)(struct _w_str *);
} W_Str;

W_Str *str_init();
void str_set(W_Str *s, char *value);
char *str_value(W_Str *s);
char *str_concat(W_Str *s1, W_Str *s2);
char *str_slice(W_Str *s, int start, int end);
int str_length(W_Str *s);
void str_destroy(W_Str *s);

#endif