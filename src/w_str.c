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
    s->type = STRING;
    s->value = value;
}

/**
 * \brief Gets the value of a string.
 * \param s The string.
 * \return The value of the string.
 */
char *str_value(W_Str *s) {
    return s->value;
}

/**
 * \brief Concatenates two strings.
 * \param s1 The first string.
 * \param s2 The second string.
 * \return The concatenated string.
 */
char *str_concat(W_Str *s1, W_Str *s2) {
    char *result = (char *)malloc(strlen(s1->value) + strlen(s2->value) + 1);
    strcpy(result, s1->value);
    strcat(result, s2->value);
    return result;
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