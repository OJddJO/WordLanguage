#include "keywords.h"

dict *keywords;

/**
 * \brief Add a keyword in the keywords dictionary
 * \param name The keyword to add
 * \param func The function to execute when the keyword is called
 * \param argc The number of arguments the keyword takes
 * \param priority The priority of the keyword
 */
void add_keyword(char *name, W_Word *(func)(Scope*, list*, int, list_element*), int argc, int priority) {
    keyword *kw = w_malloc(sizeof(keyword));
    kw->func = func;
    kw->argc = argc;
    kw->priority = priority;
    dict_set(keywords, name, kw);
}
