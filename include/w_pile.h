#ifndef __W_PILE_H__
#define __W_PILE_H__

#include <stdlib.h>

typedef struct _w_pile_element {
    void *value;
    W_PileElement *next;
} W_PileElement;

typedef struct _pile {
    W_PileElement *top;
    int size;
} W_Pile;

W_Pile *pile_init();
void pile_push(W_Pile *p, void *value);
void *pile_pop(W_Pile *p);
int pile_is_empty(W_Pile *p);
int pile_size(W_Pile *p);
void pile_destroy(W_Pile *p);

#endif