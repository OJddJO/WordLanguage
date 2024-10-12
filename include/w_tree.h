#ifndef __W_TREE_H__
#define __W_TREE_H__

#include <stdlib.h>
#include "w_lexer.h"

typedef struct _w_tree W_Tree;
typedef struct _w_tree {
    W_Word *value;
    W_Tree *left;
    W_Tree *right;
} W_Tree;

W_Tree *tree_init();
void tree_set(W_Tree *t, W_Word *value);
W_List *tree_post_order(W_Tree *t);
void tree_destroy(W_Tree *t);

#endif