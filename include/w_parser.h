#ifndef __W_PARSER_H__
#define __W_PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_lexer.h"
#include "w_list.h"

W_List *parse(W_List *tokenized_code);
W_List *parse_line(W_List *line);
int get_priority(char *operator);
W_List *shunting_yard(W_List_Element *current_word);
void print_parsed_code(W_List *parsed_code);
void parser_destroy(W_List *parsed_code);

#endif