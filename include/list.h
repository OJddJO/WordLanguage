/**
 * \attention This file is used for the list implementation, used for the implementation of the lexer, parser and interpreter.
 * \attention To refer to the WordLanguage list representation, use w_list instead.
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include "w_alloc.h"

typedef struct _list_element {
    void *value;
    struct _list_element *prev;
    struct _list_element *next;
} list_element;

typedef struct _list {
    list_element *head;
    list_element *tail;
    int size;
    int middle;
} list;

list *list_init();
void list_append(list *l, void *value);
void list_remove(list *l, int index);
void *list_get(list *l, int index);
void list_insert(list *l, int index, void *value);
void list_reverse(list *l);
int list_size(list *l);
void *list_pop(list *l);
void list_destroy(list *l);
void list_destroy_no_free(list *l);

#endif