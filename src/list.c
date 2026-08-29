#include <stddef.h>

#include "list.h"

int listPrepend(List *list, void *element) {
    Node *new = malloc(sizeof(Node));
    if (!new) return 0;
    *new = (Node) {
        .prev = list,
        .next = *list,
        .value = element,
    };

    if (*list) (*list)->prev = &new->next;
    *list = new;

    return 1;
}

void *listRemove(List *list, void *element, int (*cmp)(void *, void *)) {
    while (*list) {
        if (cmp((*list)->value, element)) {
            Node *node = *list;

            *(node->prev) = node->next;
            if (node->next)
                node->next->prev = node->prev;

            void *ret = node->value;
            free(node);
            return ret;
        }

        list = &(*list)->next;
    }
    return NULL;
}
