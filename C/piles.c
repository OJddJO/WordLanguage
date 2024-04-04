#include <stdlib.h>
#include <stdio.h>
#include "piles.h"

/*Initializes pile structure p*/
void init_pile (struct pile *p) {
    p->top = NULL;
    p->size = 0;
}

/*Adds element x to the pile p*/
void push(struct pile *p, int value) {
    struct pile_element *e = (struct pile_element *)malloc(sizeof(struct pile_element));
    e->value = value;
    e->next = p->top;
    p->top = e;
    p->size++;
}

/*Removes the top element from the pile p*/
void pop(struct pile *p) {
    struct pile_element *e = p->top;
    p->top = e->next;
    p->size--;
    free(e);
}

/*Returns the top element of the pile p*/
int top(struct pile *p) {
    if (p->top == NULL) {
        return -1;
    }
    return p->top->value;
}

int is_empty(struct pile *p) {
    return p->size == 0;
}

/*Returns the size of the pile p*/
int pile_size(struct pile *p) {
    return p->size;
}

/*Deletes the pile p*/
void delete_pile(struct pile *p) {
    struct pile_element *e = p->top;
    while (e != NULL) {
        struct pile_element *next = e->next;
        free(e);
        e = next;
    }
}

/*Prints the pile p*/
void print_pile(struct pile *p) {
    struct pile_element *e = p->top;
    while (e != NULL) {
        printf("%d ", e->value);
        e = e->next;
    }
    printf("\n");
}