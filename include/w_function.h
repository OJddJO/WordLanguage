#ifndef __W_FUNCTION_H__
#define __W_FUNCTION_H__

#include "w_type.h"
#include "list.h"
#include "dict.h"
#include "parser.h"

typedef struct _w_func {
    W_Type type;
    void *value; //not used "DUMMY"
    void (*destroy)(struct _w_func *);
    char *(*stringify)(struct _w_func *);
    struct _w_func *(*copy)(struct _w_func *);
    W_Type return_type;
    dict *args; //arguments: dict: key: arg name, value: arg type
    list *parsed_code;
} W_Func;

W_Func *w_func_init();
void w_func_destroy(W_Func *f);
char *w_func_stringify(W_Func *f);
W_Func *w_func_copy(W_Func *f);

#endif