#ifndef __W_PARSER_H__
#define __W_PARSER_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_lexer.h"
#include "w_tree.h"
#include "w_list.h"

W_List *parse(W_List *tokenized_code);
W_List *parse_line(W_List *line);

#endif