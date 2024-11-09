#include "keywords.h"

dict *keywords;

void add_keyword(char *name, W_Word *(func)(Scope*, list*, int, list_element*), int argc) {
    keyword *kw = w_alloc(sizeof(keyword));
    kw->func = func;
    kw->argc = argc;
    w_dict_set(keywords, name, kw);
}