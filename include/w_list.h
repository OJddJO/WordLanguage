#ifndef __LIST_H__
#define __LIST_H__

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

W_List *list_init();
void list_append(W_List *l, void *value);
int list_replace(W_List *l, int index, void *value);
void list_remove(W_List *l, int index);
void *list_pop(W_List *l);
void *list_get(W_List *l, int index);
int list_index(W_List *l, void *value);
int list_size(W_List *l);
void list_concat(W_List *l1, W_List *l2);
char *list_stringify(W_List *l);
W_List *list_copy(W_List *l);
void list_destroy_any(W_List *l);
void list_destroy(W_List *l);

#endif