#ifndef __W_OBJECT_H__
#define __W_OBJECT_H__

#include <stdlib.h>
#include <stdio.h>

typedef enum _w_type {
    W_INT,
    W_FLOAT,
    W_STR,
    W_BOOL,
    W_LIST,
    W_DICT,
    W_FUNCTION,
    W_NULL
} W_Type;

typedef struct _w_object {
    W_Type type;
    void *class;
    void *value
} W_Object

#endif // __W_OBJECT_H__