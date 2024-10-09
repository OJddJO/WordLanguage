#ifndef __W_STR_H__
#define __W_STR_H__

#include <stdlib.h>

typedef struct _w_str {
    char *value;
} W_Str;

W_Str *str_init();
void str_set(W_Str *s, char *value);
char *str_value(W_Str *s);
char *str_concat(W_Str *s1, W_Str *s2);

void str_destroy(W_Str *s);

#endif