#include "w_piles.h"

/**
 * \brief Initialize a new pile
 * \return A pointer to the new pile
 */
W_Pile *pile_init() {
    W_Pile *p = (W_Pile *)malloc(sizeof(W_Pile));
    p->top = NULL;
    p->size = 0;
    return p;
}

/**
 * \brief Push a new element on top of the pile
 * \param p The pile
 * \param value The value to push
 */
void pile_push(W_Pile *p, void *value) {
    W_PileElement *e = (W_PileElement *)malloc(sizeof(W_PileElement));
    e->value = value;
    e->next = p->top;
    p->top = e;
    p->size++;
}

/**
 * \brief Pop the top element of the pile
 * \param p The pile
 * \return The value of the top element
 */
void *pile_pop(W_Pile *p) {
    W_PileElement *e = p->top;
    void *value = e->value;
    p->top = e->next;
    p->size--;
    free(e);
    return value;
}

/**
 * \brief Check if the pile is empty
 * \param p The pile
 * \return 1 if the pile is empty, 0 otherwise
 */
int pîle_is_empty(W_Pile *p) {
    return p->size == 0;
}

/**
 * \brief Get the size of the pile
 * \param p The pile
 * \return The size of the pile
 */
int pile_size(W_Pile *p) {
    return p->size;
}

/**
 * \brief Delete the pile
 * \param p The pile
 */
void pile_destroy(W_Pile *p) {
    W_PileElement *e = p->top;
    while (e != NULL) {
        W_PileElement *next = e->next;
        free(e);
        e = next;
    }
    free(p);
}