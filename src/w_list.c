#include "w_list.h"

/**
 * \brief Initialize a list (malloc)
 * \return A pointer to the new list
 **/
W_List *list_init() {
    W_List *l = (W_List *)malloc(sizeof(W_List));
    l->type = LIST;
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    l->middle = 0;
    l->destroy = &list_destroy;
    l->stringify = &list_stringify;
    l->copy = &list_copy;
    return l;
}

/**
 * \brief Add an element to the list (malloc)
 * \param l The list to add the element to
 * \param value The value of the element to add
 **/
void list_append(W_List *l, void *value) {
    W_List_Element *e = (W_List_Element *)malloc(sizeof(W_List_Element));
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
 * \param value The value to set (W_Var)
 * \return 0 if successful, -1 if the index is out of bounds
 **/
int list_replace(W_List *l, int index, void *value) {
    if (index < 0 || index >= l->size) {
        return -1;
    }
    W_List_Element *e;
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
    ((W_Var*) e->value)->destroy(e->value);
    e->value = value;
    return 0;
}

/**
 * \brief Remove an element from the list (works with any list) (do not use for evaluation of wordlang)
 * \param l The list to remove the element from
 * \param index The index of the element to remove
 * \note This function is not safe to use with the evaluation of wordlang, as it does not free the memory of the value
 * \note Please use list_remove instead.
 **/
void list_remove_any(W_List *l, int index) {
    W_List_Element *e = l->head;
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
    if (e->value != NULL) free(e->value);
    free(e);
}

/**
 * \brief Remove an element from the list
 * \param l The list to remove the element from
 * \param index The index of the element to remove
 **/
void list_remove(W_List *l, int index) {
    W_List_Element *e = l->head;
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
    ((W_Var*) e->value)->destroy(e->value);
    free(e);
}

/**
 * \brief Remove and return the last element in the list (malloc)
 * \param l The list to pop the element from
 * \return The value of the last element in the list, or NULL if the list is empty
 **/
void *list_pop(W_List *l) {
    if (l->size == 0) {
        return NULL;
    }
    void *value = l->tail->value;
    W_List_Element *e = l->tail;
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
 * \brief Get the value of an element in the list
 * \param l The list to get the element from
 * \param index The index of the element to get
 * \return The value of the element at the given index, or NULL if the index is out of bounds
 **/
void *list_get(W_List *l, int index) {
    if (index < 0 || index >= l->size) {
        return NULL;
    }
    W_List_Element *e;
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
    W_List_Element *e = l->head;
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
 * \brief Concatenate two lists and destroy the second list
 * \param l1 The first list
 * \param l2 The second list
 **/
void list_concat(W_List *l1, W_List *l2) {
    if (l1->head == NULL) {
        l1->head = l2->head;
        l1->tail = l2->tail;
    } else {
        l1->tail->next = l2->head;
        l2->head->prev = l1->tail;
        l1->tail = l2->tail;
    }
    l1->size += l2->size;
    l1->middle += l2->middle;
}

/**
 * \brief Stringify a list
 * \param l The list to stringify
 * \return The string representation of the list
 **/
char *list_stringify(W_List *l) {
    int size = 0;
    W_List_Element *e = l->head;

    // Calculate the required size for the resulting string
    while (e != NULL) {
        if (((W_Var *)e->value)->type == STRING) size += 2; // for quotes
        char *str = ((W_Var *)e->value)->stringify(e->value);
        size += strlen(str) + 1; // +1 for comma or null terminator
        free(str);
        e = e->next;
    }

    // Allocate memory for the resulting string
    char *str = (char *)malloc(size + 3 + 2 * l->size); // +3 for brackets and null terminator, +2*l->size for commas and spaces

    str[0] = '[';
    str[1] = '\0';

    e = l->head;

    // Construct the string by iterating over the list elements
    for (int i = 0; i < l->size; i++) {
        if (((W_Var *)e->value)->type == STRING) strcat(str, "\"");
        char *value = ((W_Var *) e->value)->stringify(e->value);
        strcat(str, value);
        free(value);
        if (((W_Var *)e->value)->type == STRING) strcat(str, "\"");
        if (i < l->size - 1) {
            strcat(str, ", ");
        }
        e = e->next;
    }

    strcat(str, "]");
    return str;
}

/**
 * \brief Copy a list (malloc)
 * \param l The list to copy
 * \return A copy of the list
 **/
W_List *list_copy(W_List *l) {
    W_List *copy = list_init();
    W_List_Element *e = l->head;
    for (int i = 0; i < l->size; i++) {
        W_Var *value = e->value;
        list_append(copy, value->copy(value));
    }
    return copy;
}

/**
 * \brief Destroy a list (works with any list) (do not use for evaluation of wordlang)
 * \param l The list to destroy
 * \note This function is not safe to use with the evaluation of wordlang, as it does not free the memory of the value
 * \note Please use list_destroy instead.
 **/
void list_destroy_any(W_List *l) {
    W_List_Element *e = l->head;
    while (e != NULL) {
        W_List_Element *next = e->next;
        if (e->value != NULL) free(e->value);
        free(e);
        e = next;
    }
    free(l);
}

/**
 * \brief Destroy a list
 * \param l The list to destroy
 **/
void list_destroy(W_List *l) {
    W_List_Element *e = l->head;
    while (e != NULL) {
        W_List_Element *next = e->next;
        if (e->value != NULL) ((W_Var*)e->value)->destroy(e->value);
        free(e);
        e = next;
    }
    free(l);
}