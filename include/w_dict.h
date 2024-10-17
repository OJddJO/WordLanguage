#ifndef __W_DICT_H__
#define __W_DICT_H__

#include <stdlib.h>
#include <string.h>
#include "w_type.h"
#include "w_list.h"

typedef struct _w_dict {
    W_Type type;
    W_List *keys;
    W_List *values;
    void (*destroy)(struct _w_dict *);
} W_Dict;

W_Dict *dict_init();
void dict_set(W_Dict *d, char *key, void *value);
int dict_contains(W_Dict *d, char *key);
void *dict_get(W_Dict *d, char *key);
W_List *dict_keys(W_Dict *d);
int dict_size(W_Dict *d);
void dict_remove(W_Dict *d, char *key);
void dict_destroy(W_Dict *d);

#endif
