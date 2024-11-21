#ifndef __W_INT_H__
#define __W_INT_H__

#include "w_type.h"
#include <math.h>

typedef struct _w_int {
    W_Type type;
    void (*destroy)(struct _w_int *);
    char *(*stringify)(struct _w_int *);
    struct _w_int *(*copy)(struct _w_int *);
    float *value;
} W_Int;

W_Int *w_int_init();
void w_int_assign(W_Int *i, char *value);
void w_int_set(W_Int *i, float value);
float *w_int_value(W_Int *i);
char *w_int_stringify(W_Int *i);
void w_int_destroy(W_Int *i);
W_Int *w_int_copy(W_Int *i);

#endif