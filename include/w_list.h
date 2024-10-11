#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include "w_type.h"

typedef struct _w_list_element {
    W_Type type;
    void *value;
    struct _w_list_element *prev;
    struct _w_list_element *next;
} W_ListElement;

typedef struct _w_list {
    W_ListElement *head;
    W_ListElement *tail;
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
void list_destroy(W_List *l);

#endif