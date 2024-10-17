#ifndef __W_INT_H__
#define __W_INT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "w_type.h"

typedef struct _w_int {
    W_Type type;
    int *value;
    void (*destroy)(struct _w_int *);
    void (*print)(struct _w_int *);
    void (*assign)(struct _w_int *, char *);
    void (*set)(struct _w_int *, int);
    int *(*get)(struct _w_int *);
} W_Int;

W_Int *int_init();
void int_assign(W_Int *i, char *value);
void int_set(W_Int *i, int value);
int *int_value(W_Int *i);
void int_print(W_Int *i);
void int_destroy(W_Int *i);

#endif