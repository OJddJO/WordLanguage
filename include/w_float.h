#ifndef __W_FLOAT_H__
#define __W_FLOAT_H__

#include "w_alloc.h"
#include "w_type.h"

typedef struct _w_float {
    W_Type type;
    float *value;
    void (*destroy)(struct _w_float *);
    char *(*stringify)(struct _w_float *);
    struct _w_float *(*copy)(struct _w_float *);
    void (*assign)(struct _w_float *, char *);
    void (*set)(struct _w_float *, float);
    float *(*get)(struct _w_float *);
} W_Float;

W_Float *w_float_init();
void w_float_assign(W_Float *f, char *value);
void w_float_set(W_Float *f, float value);
float *w_float_value(W_Float *f);
char *w_float_stringify(W_Float *f);
void w_float_destroy(W_Float *f);
W_Float *w_float_copy(W_Float *f);

#endif