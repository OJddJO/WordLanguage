/**
 * \attention This file is used for the list implementation, used for the implementation of the lexer, parser and interpreter.
 * \attention To refer to the WordLanguage list representation, use w_list instead.
 */

#include "list.h"

/**
 * \brief Initialize a list (malloc)
 * \return A pointer to the new list
 **/
list *list_init() {
    list *l = (list *)malloc(sizeof(list));
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    l->middle = 0;
    return l;
}

/**
 * \brief Add an element to the list (malloc)
 * \param l The list to add the element to
 * \param value The value of the element to add
 **/
void list_append(list *l, void *value) {
    list_element *e = (list_element *)malloc(sizeof(list_element));
    e->value = value;
    e->prev = l->tail;
    e->next = NULL;
    if (l->head == NULL) {
        l->head = e;
    } else {
        l->tail->next = e;
    }
    l->tail = e;
    l->size++;
    if (l->size % 2 == 0) {
        l->middle++;
    }
}

/**
 * \brief Remove an element from the list
 * \param l The list to remove the element from
 * \param index The index of the element to remove
 **/
void list_remove(list *l, int index) {
    list_element *e = l->head;
    for (int i = 0; i < index; i++) {
        e = e->next;
    }
    if (e->prev != NULL) {
        e->prev->next = e->next;
    } else {
        l->head = e->next;
    }
    if (e->next != NULL) {
        e->next->prev = e->prev;
    } else {
        l->tail = e->prev;
    }
    if (l->size % 2 == 0) {
        l->middle--;
    }
    l->size--;
    free(e);
}

/**
 * \brief Get the size of the list
 * \param l The list to get the size of
 * \return The size of the list
 **/
int list_size(list *l) {
    return l->size;
}

/**
 * \brief Remove and return the last element in the list (malloc)
 * \param l The list to pop the element from
 * \return The value of the last element in the list, or NULL if the list is empty
 **/
void *list_pop(list *l) {
    if (l->size == 0) {
        return NULL;
    }
    void *value = l->tail->value;
    list_element *e = l->tail;
    l->tail = e->prev;
    if (l->tail != NULL) {
        l->tail->next = NULL;
    } else {
        l->head = NULL;
    }
    if (l->size % 2 == 0) {
        l->middle--;
    }
    l->size--;
    free(e);
    return value;
}

/**
 * \brief Destroy a list
 * \param l The list to destroy
 **/
void list_destroy(list *l) {
    list_element *e = l->head;
    while (e != NULL) {
        list_element *next = e->next;
        if (e->value != NULL) free(e->value);
        free(e);
        e = next;
    }
    free(l);
}