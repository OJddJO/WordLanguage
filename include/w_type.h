#ifndef __TYPE_H__
#define __TYPE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "w_alloc.h"

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
    // mandatory members
    W_Type type; //type of the variable
    void *value; //value of the variable
    void (*destroy)(void *); //function to free the memory of the variable
    char *(*stringify)(void *); //function to convert the variable to a string
    void *(*copy)(void *); //function to copy the variable
    // following functions are optionnal
    void (*assign)(void *, char *); //function to assign a value to the variable (word -> variable)
    void (*set)(void *, void *); //function to set the value of the variable (value -> variable)
    void *(*get)(void *); //function to get the value of the variable (variable -> value)
} W_Var;

#endif