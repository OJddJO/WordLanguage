#include "w_str.h"

/**
 * \brief Initializes a new string. (w_malloc)
 * \return A pointer to the new string.
 */
W_Str *w_str_init() {
    W_Str *s = (W_Str *)w_malloc(sizeof(W_Str));
    s->type = STRING;
    s->destroy = &w_str_destroy;
    s->stringify = &w_str_stringify; //this is so fuc*ing dumb but it is needed
    s->copy = &w_str_copy;
    s->value = NULL;
    return s;
}

/**
 * \brief Assigns the value of a string. (w_malloc)
 * \param s The string.
 * \param value The new value.
 */
void w_str_assign(W_Str *s, char *value) {
    char *v;
    if (s->value == NULL) {
        v = (char *)w_malloc(strlen(value) + 1);
    } else v = s->value;
    v = (char *)realloc(v, strlen(value) + 1);
    strcpy(v, value);
    s->value = v;
}

/**
 * \brief Sets the value of a string. (w_malloc)
 * \param s The string.
 * \param value The new value.
 */
void w_str_set(W_Str *s, char *value) {
    w_str_assign(s, value);
}

/**
 * \brief Gets the value of a string.
 * \param s The string.
 * \return The value of the string.
 */
char *w_str_value(W_Str *s) {
    return s->value;
}

/**
 * \brief Concatenates two strings. (w_malloc)
 * \param s1 The first string.
 * \param s2 The second string.
 * \return The concatenation of the two strings.
 */
char *w_str_concat(W_Str *s1, W_Str *s2) {
    char *result = (char *)w_malloc(strlen(s1->value) + strlen(s2->value) + 1);
    strcpy(result, s1->value);
    strcat(result, s2->value);
    return result;
}

/**
 * \brief Slices a string. (w_malloc)
 * \param s The string.
 * \param start The start index.
 * \param end The end index.
 * \return The sliced string.
 */
char *w_str_slice(W_Str *s, int start, int end) { 
    char *result = (char *)w_malloc(end - start + 1);
    strncpy(result, s->value + start, end - start);
    result[end - start] = '\0';
    return result;
}

/**
 * \brief Gets the length of a string.
 * \param s The string.
 * \return The length of the string.
 */
int w_str_length(W_Str *s) {
    return strlen(s->value);
}

/**
 * \brief Stringifies a string. (w_malloc)
 * \param s The string.
 * \return The string representation of the string.
 */
char *w_str_stringify(W_Str *s) {
    char *str = (char *)w_malloc(strlen(s->value)+1);
    strcpy(str, s->value);
    return str; //bruh just to return the same string but w_malloc...
}

/**
 * \brief Destroys a string.
 * \param s The string.
 */
void w_str_destroy(W_Str *s) {
    w_free(s->value);
    w_free(s);
}

/**
 * \brief Copies a string. (w_malloc)
 * \param s The string.
 * \return A pointer to the copied string.
 */
W_Str *w_str_copy(W_Str *s) {
    W_Str *copy = w_str_init();
    w_str_set(copy, s->value);
    return copy;
}