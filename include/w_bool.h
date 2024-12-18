#ifndef __W_BOOL_H__
#define __W_BOOL_H__

#include <stdbool.h>
#include "w_alloc.h"
#include "w_type.h"

typedef struct _w_bool {
    W_Type type;
    bool *value;
    void (*destroy)(struct _w_bool *);
    char *(*stringify)(struct _w_bool *);
    struct _w_bool *(*copy)(struct _w_bool *);
    void (*assign)(struct _w_bool *, char *);
    void (*set)(struct _w_bool *, bool);
    bool *(*get)(struct _w_bool *);
} W_Bool;

W_Bool *w_bool_init();
void w_bool_assign(W_Bool *b, char *value);
void w_bool_set(W_Bool *b, bool value);
bool *w_bool_value(W_Bool *b);
char *w_bool_stringify(W_Bool *b);
void w_bool_destroy(W_Bool *b);
W_Bool *w_bool_copy(W_Bool *b);

#endif