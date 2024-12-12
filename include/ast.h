#ifndef __AST_H__
#define __AST_H__

#include <stdlib.h>
#include <stdio.h>

typedef struct _node {
    void *value;
    struct _node *parent;
    struct _node *l_child, *r_child;
} node;

typedef struct _op {
    char *op;
    int precedence;
} op;

node *node_init(void *value);
void node_append(node *n, node *child);
void node_destroy(node *n);

#endif // _AST_H_