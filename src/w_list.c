#include "w_list.h"

/**
 * \brief Initialize a list
 **/
W_List *list_init() {
    W_List *l = (W_List *)malloc(sizeof(W_List));
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    l->middle = 0;
    return l;
}

/**
 * \brief Add an element to the list
 * \param l The list to add the element to
 * \param value The value of the element to add
 **/
void list_append(W_List *l, void *value) {
    W_ListElement *e = (W_ListElement *)malloc(sizeof(W_ListElement));
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
 * \brief Replace the value of an element in the W_List
 * \param l The list to set the value in
 * \param index The index of the element to set
 * \param value The value to set
 * \return 0 if successful, -1 if the index is out of bounds
 **/
int list_replace(W_List *l, int index, void *value) {
    if (index < 0 || index >= l->size) {
        return -1;
    }
    W_ListElement *e;
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
    if (e == NULL) {
        return -1;
    }
    e->value = value;
    return 0;
}

/**
 * \brief Remove an element from the list
 * \param l The list to remove the element from
 * \param index The index of the element to remove
 **/
void list_remove(W_List *l, int index) {
    W_ListElement *e = l->head;
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
 * \brief Get the value of an element in the list
 * \param l The list to get the element from
 * \param index The index of the element to get
 * \return The value of the element at the given index, or NULL if the index is out of bounds
 **/
void *list_get(W_List *l, int index) {
    if (index < 0 || index >= l->size) {
        return NULL;
    }
    W_ListElement *e;
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
 * \brief Get the index of a value in the list
 * \param l The list to search
 * \param value The value to search for
 * \return The index of the value in the list, or -1 if the value is not found
 **/
int list_index(W_List *l, void *value) {
    W_ListElement *e = l->head;
    int position = 0;
    while (e != NULL) {
        if (e == value) {
            return position;
        }
        e = e->next;
        position++;
    }
    return -1;
}

/**
 * \brief Get the size of the list
 * \param l The list to get the size of
 * \return The size of the list
 **/
int list_size(W_List *l) {
    return l->size;
}

/**
 * \brief Destroy a list
 * \param l The list to destroy
 **/
void list_destroy(W_List *l) {
    W_ListElement *e = l->head;
    while (e != NULL) {
        W_ListElement *next = e->next;
        free(e);
        e = next;
    }
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    l->middle = 0;
    free(l);
}