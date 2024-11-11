/**
 * \attention This file is used for the list implementation, used for the implementation of the lexer, parser and interpreter.
 * \attention To refer to the WordLanguage list representation, use w_list instead.
 */

#include "list.h"

/**
 * \brief Initialize a list (w_malloc)
 * \return A pointer to the new list
 **/
list *list_init() {
    list *l = (list *)w_malloc(sizeof(list));
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    l->middle = 0;
    return l;
}

/**
 * \brief Add an element to the list (w_malloc)
 * \param l The list to add the element to
 * \param value The value of the element to add
 **/
void list_append(list *l, void *value) {
    list_element *e = (list_element *)w_malloc(sizeof(list_element));
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
    w_free(e);
}

/**
 * \brief Get the value of an element in the list
 * \param l The list to get the element from
 * \param index The index of the element to get
 * \return The value of the element at the given index, or NULL if the index is out of bounds
 **/
void *list_get(list *l, int index) {
    if (index < 0 || index >= l->size) {
        return NULL;
    }
    list_element *e;
    if (index > l->middle) {
        e = l->tail;
        for (int i = l->size - 1; i > index; i--) {
            e = e->prev;
        }
    } else {
        e = l->head;
        for (int i = 0; i < index; i++) {
            e = e->next;
        }
    }
    return e->value;
}

/**
 * \brief Insert an element into the list (w_malloc)
 * \param l The list to insert the element into
 * \param index The index to insert the element at
 * \param value The value of the element to insert
 **/
void list_insert(list *l, int index, void *value) {
    if (index < 0 || index > l->size) {
        return;
    }
    list_element *e = (list_element *)w_malloc(sizeof(list_element));
    e->value = value;
    if (index == 0) {
        e->prev = NULL;
        e->next = l->head;
        if (l->head != NULL) {
            l->head->prev = e;
        } else {
            l->tail = e;
        }
        l->head = e;
    } else if (index == l->size) {
        e->prev = l->tail;
        e->next = NULL;
        l->tail->next = e;
        l->tail = e;
    } else {
        list_element *current = l->head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        e->prev = current->prev;
        e->next = current;
        current->prev->next = e;
        current->prev = e;
    }
    l->size++;
    if (l->size % 2 == 0) {
        l->middle++;
    }
}

/**
 * \brief Reverse the list
 * \param l The list to reverse
 **/
void list_reverse(list *l) {
    list_element *current = l->head;
    while (current != NULL) {
        list_element *next = current->next;
        current->next = current->prev;
        current->prev = next;
        current = next;
    }
    list_element *temp = l->head;
    l->head = l->tail;
    l->tail = temp;
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
 * \brief Remove and return the last element in the list (w_malloc)
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
    w_free(e);
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
        if (e->value != NULL) w_free(e->value);
        w_free(e);
        e = next;
    }
    w_free(l);
}

/**
 * \brief Destroy a list without freeing the values
 * \param l The list to destroy
 **/
void list_destroy_no_free(list *l) {
    list_element *e = l->head;
    while (e != NULL) {
        list_element *next = e->next;
        w_free(e);
        e = next;
    }
    w_free(l);
}