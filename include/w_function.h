#ifndef __W_FUNCTION_H__
#define __W_FUNCTION_H__

#include <stdlib.h>
#include <string.h>
#include "w_type.h"
#include "w_list.h"
#include "w_dict.h"
#include "w_parser.h"

typedef struct _w_func {
    W_Type type;
    void *value; //not used "DUMMY"
    void (*destroy)(struct _w_func *);
    void (*print)(struct _w_func *);
    struct _w_func *(*copy)(struct _w_func *);
    W_Type return_type;
    W_Dict *args; //arguments: dict: key: arg name, value: arg type
    W_List *parsed_code;
} W_Func;

W_Func *func_init();
void func_destroy(W_Func *f);
void func_print(W_Func *f);
W_Func *func_copy(W_Func *f);

#endif