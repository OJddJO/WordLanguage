/**
 * \attention This file is used for the dict implementation, used for the implementation of the lexer, parser and interpreter.
 * \attention To refer to the WordLanguage dict representation, use w_dict instead.
 */

#ifndef __DICT_H__
#define __DICT_H__

#include <string.h>
#include "w_alloc.h"
#include "list.h"

typedef struct _dict {
    list *keys;
    list *values;
} dict;

dict *dict_init();
void dict_set(dict *d, char *key, void *value);
int dict_contains(dict *d, char *key);
void *dict_get(dict *d, char *key);
list *dict_keys(dict *d);
int dict_size(dict *d);
void dict_remove(dict *d, char *key);
void dict_destroy(dict *d); 

#endif