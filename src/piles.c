#include "piles.h"

/**
 * \brief Initialize a new pile
 * \return A pointer to the new pile
 */
Pile *pile_init() {
    Pile *p = (Pile *)malloc(sizeof(Pile));
    p->top = NULL;
    p->size = 0;
    return p;
}

/**
 * \brief Push a new element on top of the pile
 * \param p The pile
 * \param value The value to push
 */
void pile_push(Pile *p, void *value) {
    PileElement *e = (PileElement *)malloc(sizeof(PileElement));
    e->value = value;
    e->next = p->top;
    p->top = e;
    p->size++;
}

/**
 * \brief Pop the top element of the pile
 * \param p The pile
 */
void pile_pop(Pile *p) {
    PileElement *e = p->top;
    p->top = e->next;
    p->size--;
    free(e);
}

/**
 * \brief Get the top element of the pile
 * \param p The pile
 * \return The top element of the pile
 */
void *pile_top(Pile *p) {
    if (p->top == NULL) {
        return -1;
    }
    return p->top->value;
}

/**
 * \brief Check if the pile is empty
 * \param p The pile
 * \return 1 if the pile is empty, 0 otherwise
 */
int pîle_is_empty(Pile *p) {
    return p->size == 0;
}

/**
 * \brief Get the size of the pile
 * \param p The pile
 * \return The size of the pile
 */
int pile_size(Pile *p) {
    return p->size;
}

/**
 * \brief Delete the pile
 * \param p The pile
 */
void pile_destroy(Pile *p) {
    PileElement *e = p->top;
    while (e != NULL) {
        PileElement *next = e->next;
        free(e);
        e = next;
    }
    free(p);
}