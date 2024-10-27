#ifndef __TYPE_H__
#define __TYPE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum _w_type {
    NULL_TYPE,
    INT,
    FLOAT, //double is the same as float
    STRING, //char * is the same as string
    BOOL,
    LIST,
    DICT, //dict will not be implemented in the first version due to the memory management
    FUNCTION,
} W_Type;

typedef struct _w_var { // WARNING: All types must have the same order of struct members
    W_Type type;
    void *value;
    void (*destroy)(void *);
    char *(*stringify)(void *);
    void *(*copy)(void *);
    void (*assign)(void *, void *);
    void (*set)(void *, void *);
    void *(*get)(void *);
} W_Var;

#endif