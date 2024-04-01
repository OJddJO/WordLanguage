#ifndef __PILES_H__
#define __PILES_H__


struct pile_element {
    int value;
    struct pile_element *next;
};

struct pile {
    struct pile_element *top;
    int size;
    int capacity;
};

void init_pile(struct pile *p);
void push(struct pile *p, int value);
int pop(struct pile *p);
int top(struct pile *p);
int is_empty(struct pile *p);
int pile_size(struct pile *p);
void delete_pile(struct pile *p);
void print_pile(struct pile *p);

#endif