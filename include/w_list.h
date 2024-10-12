#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include "w_type.h"

typedef struct _w_list_element W_List_Element;
typedef struct _w_list W_List;

typedef struct _w_list_element {
    W_Type type;
    void *value;
    W_List_Element *prev;
    W_List_Element *next;
} W_List_Element;

typedef struct _w_list {
    W_Type type;
    W_List_Element *head;
    W_List_Element *tail;
    int size;
    int middle;
} W_List;

W_List *list_init();
void list_append(W_List *l, void *value);
int list_replace(W_List *l, int index, void *value);
void list_remove(W_List *l, int index);
void *list_get(W_List *l, int index);
int list_index(W_List *l, void *value);
int list_size(W_List *l);
void list_concat(W_List *l1, W_List *l2);
void list_destroy(W_List *l);

#endif