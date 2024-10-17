#include "w_str.h"

/**
 * \brief Initializes a new string. (malloc)
 * \return A pointer to the new string.
 */
W_Str *str_init() {
    W_Str *s = (W_Str *)malloc(sizeof(W_Str));
    s->type = NULL_TYPE;
    s->value = NULL;
    s->destroy = &str_destroy;
    s->print = &str_print;
    s->assign = &str_assign;
    s->set = &str_set;
    s->get = &str_value;
    return s;
}

/**
 * \brief Assigns the value of a string. (malloc)
 * \param s The string.
 * \param value The new value.
 */
void str_assign(W_Str *s, char *value) {
    //strip quotes
    int len = strlen(value);
    char var_value[len - 1];
    for (int i = 1; i < len - 1; i++) {
        var_value[i - 1] = value[i];
    }
    var_value[len - 1] = '\0';
    if (s->type != NULL_TYPE) {
        free(s->value);
    } else s->type = STRING;

    char *v = (char *)malloc(strlen(var_value) + 1);
    strcpy(v, var_value);
    s->value = v;
}

/**
 * \brief Sets the value of a string. (malloc)
 * \param s The string.
 * \param value The new value.
 */
void str_set(W_Str *s, char *value) {
    if (s->type == NULL_TYPE) {
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

/**
 * \brief Concatenates two strings. (malloc)
 * \param s1 The first string.
 * \param s2 The second string.
 * \return The concatenation of the two strings.
 */
char *str_concat(W_Str *s1, W_Str *s2) {
    char *result = (char *)malloc(strlen(s1->value) + strlen(s2->value) + 1);
    strcpy(result, s1->value);
    strcat(result, s2->value);
    return result;
}

/**
 * \brief Slices a string. (malloc)
 * \param s The string.
 * \param start The start index.
 * \param end The end index.
 * \return The sliced string.
 */
char *str_slice(W_Str *s, int start, int end) { 
    char *result = (char *)malloc(end - start + 1);
    strncpy(result, s->value + start, end - start);
    result[end - start] = '\0';
    return result;
}

/**
 * \brief Gets the length of a string.
 * \param s The string.
 * \return The length of the string.
 */
int str_length(W_Str *s) {
    return strlen(s->value);
}

/**
 * \brief Prints a string.
 * \param s The string.
 */
void str_print(W_Str *s) {
    printf("'%s'", s->value);
}

/**
 * \brief Destroys a string.
 * \param s The string.
 */
void str_destroy(W_Str *s) {
    free(s->value);
    free(s);
}