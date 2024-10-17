#ifndef __W_BOOL_H__
#define __W_BOOL_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "w_type.h"

typedef struct _w_bool {
    W_Type type;
    bool *value;
    void (*destroy)(struct _w_bool *);
    void (*print)(struct _w_bool *);
    void (*assign)(struct _w_bool *, char *);
    void (*set)(struct _w_bool *, bool);
    bool *(*get)(struct _w_bool *);
} W_Bool;

W_Bool *bool_init();
void bool_assign(W_Bool *b, char *value);
void bool_set(W_Bool *b, bool value);
bool *bool_value(W_Bool *b);
void bool_print(W_Bool *b);
void bool_destroy(W_Bool *b);

#endif