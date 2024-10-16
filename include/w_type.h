#ifndef __TYPE_H__
#define __TYPE_H__

typedef enum _w_type {
    NULL_TYPE,
    INT,
    FLOAT, //double is the same as float
    STRING, //char * is the same as string
    BOOL,
    ARRAY,
    LIST,
    DICT, //dict will not be implemented in the first version due to the memory management
    FUNCTION,
} W_Type;

typedef struct _w_var {
    W_Type type;
    void *value;
} W_Var;

#endif