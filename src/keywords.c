#include "keywords.h"

dict *keywords;
dict *reserved_words;

/**
 * \brief Add a keyword in the keywords dictionary (w_malloc)
 * \param name The keyword to add
 * \param func The function to execute when the keyword is called
 * \param argc The number of arguments the keyword takes
 * \param priority The priority of the keyword
 */
void add_keyword(char *name, W_Word *(func)(Scope*, list*, int, list_element**, W_Type, void *), int argc, int priority) {
    keyword *kw = w_malloc(sizeof(keyword));
    kw->func = func;
    kw->argc = argc;
    kw->priority = priority;
    dict_set(keywords, name, kw);
}

/**
 * \brief Add a reserved word to the reserved words list (w_malloc)
 * \param name The reserved word to add
 * \param priority The priority of the reserved word
 */
void add_reserved_word(char *name, int priority) {
    int *p = (int *)w_malloc(sizeof(int));
    *p = priority;
    dict_set(reserved_words, name, p);
}