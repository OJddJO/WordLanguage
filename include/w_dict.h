#ifndef __W_DICT_H__
#define __W_DICT_H__

#include "w_type.h"
#include "w_list.h"
#include "list.h"

typedef struct _w_dict {
    W_Type type;
    list *keys;
    void (*destroy)(struct _w_dict *);
    char *(*stringify)(struct _w_dict *);
    struct _w_dict *(*copy)(struct _w_dict *);
    W_List *values;
} W_Dict;

W_Dict *w_dict_init();
void w_dict_set(W_Dict *d, char *key, void *value);
int w_dict_contains(W_Dict *d, char *key);
void *w_dict_get(W_Dict *d, char *key);
list *w_dict_keys(W_Dict *d);
int w_dict_size(W_Dict *d);
void w_dict_remove(W_Dict *d, char *key);
char *w_dict_stringify(W_Dict *d);
void w_dict_destroy(W_Dict *d);
W_Dict *w_dict_copy(W_Dict *d);

#endif