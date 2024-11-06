#ifndef __W_LIST_H__
#define __W_LIST_H__

#include "w_alloc.h"
#include "w_type.h"

typedef struct _w_list_element {
    W_Type type;
    void *value;
    struct _w_list_element *prev;
    struct _w_list_element *next;
} W_List_Element;

typedef struct _w_list {
    W_Type type;
    W_List_Element *head;
    void (*destroy)(struct _w_list *);
    char *(*stringify)(struct _w_list *);
    struct _w_list *(*copy)(struct _w_list *);
    W_List_Element *tail;
    int size;
    int middle;
} W_List;

W_List *w_list_init();
void w_list_append(W_List *l, void *value);
int w_list_replace(W_List *l, int index, void *value);
void w_list_remove(W_List *l, int index);
void *w_list_pop(W_List *l);
void *w_list_get(W_List *l, int index);
int w_list_index(W_List *l, void *value);
int w_list_size(W_List *l);
void w_list_concat(W_List *l1, W_List *l2);
char *w_list_stringify(W_List *l);
W_List *w_list_copy(W_List *l);
void w_list_destroy(W_List *l);

#endif