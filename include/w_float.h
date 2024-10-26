#ifndef __W_FLOAT_H__
#define __W_FLOAT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "w_type.h"

typedef struct _w_float {
    W_Type type;
    double *value;
    void (*destroy)(struct _w_float *);
    void (*print)(struct _w_float *);
    struct _w_float *(*copy)(struct _w_float *);
    void (*assign)(struct _w_float *, char *);
    void (*set)(struct _w_float *, double);
    double *(*get)(struct _w_float *);
} W_Float;

W_Float *float_init();
void float_assign(W_Float *f, char *value);
void float_set(W_Float *f, double value);
double *float_value(W_Float *f);
void float_print(W_Float *f);
void float_destroy(W_Float *f);
W_Float *float_copy(W_Float *f);

#endif