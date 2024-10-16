#include "w_dict.h"

/**
 * \brief Initializes a new dictionary. (malloc)
 * \return A new dictionary.
 */
W_Dict *dict_init() {
    W_Dict *d = (W_Dict *)malloc(sizeof(W_Dict));
    d->type = DICT;
    d->keys = list_init();
    d->values = list_init();
    return d;
}

/**
 * \brief Sets the given key and value in the given dictionary. (malloc)
 * \param d The dictionary to set the key and value in. 
 * \param key The key to set. Must be a string.
 * \param value The value to set.
 */
void dict_set(W_Dict *d, char *key, void *value) {
    if (dict_contains(d, key)) {
        W_List_Element *current_key = d->keys->head;
        W_List_Element *current_value = d->values->head;
        for (int i = 0; i < d->keys->size; i++) {
            if (strcmp((char *)current_key->value, key) == 0) {
                if (current_value->value != NULL) {
                    free(current_value->value);
                }
                current_value->value = value;
                return;
            }
            current_key = current_key->next;
            current_value = current_value->next;
        }
    } else {
        char *key_copy = (char *)malloc(strlen(key) + 1);
        strcpy(key_copy, key);
        list_append(d->keys, key_copy);
        list_append(d->values, value);
    }
}

/**
 * \brief Checks if the given dictionary contains the given key.
 * \param d The dictionary to check.
 * \param key The key to check. Must be a string.
 * \return 1 if the dictionary contains the key, 0 otherwise.
 */
int dict_contains(W_Dict *d, char *key) {
    W_List_Element *current_key = d->keys->head;
    for (int i = 0; i < d->keys->size; i++) {
        if (strcmp((char *)current_key->value, key) == 0) {
            return 1;
        }
        current_key = current_key->next;
    }
    return 0;
}

/**
 * \brief Gets the value associated with the given key in the given dictionary.
 * \param d The dictionary to get the value from.
 * \param key The key to get the value for.
 * \return The value associated with the key. NULL if the key does not exist.
 */
void *dict_get(W_Dict *d, char *key) {
    W_List_Element *current_key = d->keys->head;
    W_List_Element *current_value = d->values->head;
    for (int i = 0; i < d->keys->size; i++) {
        if (strcmp((char *)current_key->value, key) == 0) {
            return current_value->value;
        }
        current_key = current_key->next;
        current_value = current_value->next;
    }
    return NULL;
}

/**
 * \brief Gets the keys of the given dictionary.
 * \param d The dictionary to get the keys from.
 * \return The keys of the dictionary.
 */
W_List *dict_keys(W_Dict *d) {
    return d->keys;
}

/**
 * \brief Gets the size of the given dictionary.
 * \param d The dictionary to get the size of.
 * \return The size of the dictionary.
 */
int dict_size(W_Dict *d) {
    return list_size(d->keys);
}

/**
 * \brief Removes the given key and value from the given dictionary.
 * \param d The dictionary to remove the key and value from.
 * \param key The key to remove.
 */
void dict_remove(W_Dict *d, char *key) {
    for (int i = 0; i < d->keys->size; i++) {
        if (strcmp(d->keys->head->value, key) == 0) {
            list_remove(d->keys, i);
            list_remove(d->values, i);
            return;
        }
    }
}

/**
 * \brief Destroys the given dictionary.
 * \param d The dictionary to destroy.
 */
void dict_destroy(W_Dict *d) {
    list_destroy(d->keys);
    list_destroy(d->values);
    free(d);
}