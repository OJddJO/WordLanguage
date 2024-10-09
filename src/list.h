#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

typedef struct _element {
    void *value;
    struct _element *prev;
    struct _element *next;
} element;

typedef struct _list {
    element *head;
    element *tail;
    int size;
    int middle;
} list;

list *list_init();
void list_append(list *l, void *value);
int list_replace(list *l, int index, void *value);
void list_remove(list *l, int index);
void *list_get(list *l, int index);
int list_index(list *l, void *value);
int list_size(list *l);
void list_destroy(list *l);

#endif