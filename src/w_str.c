#include "w_str.h"

/**
 * \brief Initializes a new string.
 * \return A pointer to the new string.
 */
W_Str *str_init() {
    W_Str *s = (W_Str *)malloc(sizeof(W_Str));
    s->type = NULL_TYPE;
    s->value = NULL;
    return s;
}

/**
 * \brief Sets the value of a string.
 * \param s The string.
 * \param value The new value.
 */
void str_set(W_Str *s, char *value) {
    if (s->type != NULL_TYPE) {
        free(s->value);
        s->type = STRING;
    }
    char *v = (char *)malloc(strlen(value) + 1);
    strcpy(v, value);
    s->value = v;
}

/**
 * \brief Gets the value of a string.
 * \param s The string.
 * \return The value of the string.
 */
char *str_value(W_Str *s) {
    return s->value;
}

char *str_slice(W_Str *s, int start, int end) {
    char *result = (char *)malloc(end - start + 1);
    strncpy(result, s->value + start, end - start);
    result[end - start] = '\0';
    return result;
}

int str_length(W_Str *s) {
    return strlen(s->value);
}

void str_destroy(W_Str *s) {
    free(s->value);
    free(s);
}