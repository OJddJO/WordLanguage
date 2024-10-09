#ifndef __W_INT_H__
#define __W_INT_H__

#include "w_type.h"

typedef struct _w_int {
    W_Type type;
    int value;
} W_Int;

W_Int *int_init();
void int_set(W_Int *i, int value);
int int_value(W_Int *i);
void int_destroy(W_Int *i);

#endif