#ifndef __W_FLOAT_H__
#define __W_FLOAT_H__

#include <stdlib.h>
#include "w_type.h"

typedef struct _w_float {
    W_Type type;
    double *value;
} W_Float;

W_Float *float_init();
void float_set(W_Float *f, double value);
double *float_value(W_Float *f);
void float_destroy(W_Float *f);

#endif