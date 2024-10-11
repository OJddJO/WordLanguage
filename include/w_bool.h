#ifndef __W_BOOL_H__
#define __W_BOOL_H__

#include <stdlib.h>
#include <stdbool.h>
#include "w_type.h"

typedef struct _w_bool {
    W_Type type;
    bool *value;
} W_Bool;

W_Bool *bool_init();
void bool_set(W_Bool *b, bool value);
bool bool_value(W_Bool *b);
void bool_destroy(W_Bool *b);

#endif