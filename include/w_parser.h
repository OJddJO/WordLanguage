#ifndef __W_PARSER_H__
#define __W_PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_lexer.h"
#include "w_tree.h"
#include "w_list.h"

W_List *parse(W_List *tokenized_code);
W_List *parse_line(W_List *line);
int parser_highest_priority(W_List_Element *current_word);
int get_priority(char *operator);
W_Tree *parse_operation(W_List_Element *current_word);

#endif