/**
 * \attention This file is used for the dict implementation, used for the implementation of the lexer, parser and interpreter.
 * \attention To refer to the WordLanguage dict representation, use w_dict instead.
 */

#include "dict.h"

/**
 * \brief Initializes a new dictionary. (malloc)
 * \return A new dictionary.
 */
dict *dict_init() {
    dict *d = (dict *)malloc(sizeof(dict));
    d->keys = list_init(); //list of keys, is a normal list
    d->values = list_init(); //list of values, is a list
    return d;
}

/**
 * \brief Sets the given key and value in the given dictionary. (malloc)
 * \param d The dictionary to set the key and value in. 
 * \param key The key to set. Must be a string.
 * \param value The value to set.
 */
void dict_set(dict *d, char *key, void *value) {
    if (dict_contains(d, key)) {
        list_element *current_key = d->keys->head;
        list_element *current_value = d->values->head;
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
int dict_contains(dict *d, char *key) {
    list_element *current_key = d->keys->head;
    for (int i = 0; i < d->keys->size; i++) {
        if (strcmp((char *)current_key->value, key) == 0) {
            return 1;
        }
        current_key = current_key->next;
    }
    return 0;
}

/**
 * \brief Gets the value of the given key from the given dictionary.
 * \param d The dictionary to get the value from.
 * \param key The key to get the value of.
 * \return The value of the key.
 */
void *dict_get(dict *d, char *key) {
    list_element *current_key = d->keys->head;
    list_element *current_value = d->values->head;
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
list *dict_keys(dict *d) {
    return d->keys;
}

/**
 * \brief Gets the size of the given dictionary.
 * \param d The dictionary to get the size of.
 * \return The size of the dictionary.
 */
int dict_size(dict *d) {
    return list_size(d->keys);
}

/**
 * \brief Removes the given key and value from the given dictionary.
 * \param d The dictionary to remove the key and value from.
 * \param key The key to remove.
 */
void dict_remove(dict *d, char *key) {
    list_element *current_key = d->keys->head;
    for (int i = 0; i < d->keys->size; i++) {
        if (strcmp(current_key->value, key) == 0) {
            list_remove(d->keys, i);
            list_remove(d->values, i);
            return;
        }
        current_key = current_key->next;
    }
}

/**
 * \brief Destroys the given dictionary.
 * \param d The dictionary to destroy.
 */
void dict_destroy(dict *d) {
    list_destroy(d->keys);
    list_destroy(d->values);
    free(d);
}
