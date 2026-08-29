#ifndef __LIST_H__
#define __LIST_H__

typedef struct _Node {
    struct _Node **prev;
    struct _Node *next;
    void         *value;
} Node, *List;

int listPrepend(List *list, void *element);
void *listRemove(List *list, void *element, int (*cmp)(void *, void *));

#endif
