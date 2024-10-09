#ifndef __PILES_H__
#define __PILES_H__

#include <stdlib.h>

typedef struct pile_element {
    void *value;
    struct pile_element *next;
} PileElement;

typedef struct _pile {
    struct pile_element *top;
    int size;
} Pile;

Pile *pile_init();
void pile_push(Pile *p, void *value);
void pile_pop(Pile *p);
void *pile_top(Pile *p);
int pile_is_empty(Pile *p);
int pile_size(Pile *p);
void pile_destroy(Pile *p);

#endif