#include "ast.h"

/**
 * \brief Initializes a new node. (w_malloc)
 * \param value The value of the node.
 * \return A pointer to the newly created node.
 */
node *node_init(void *value) {
    node *n = (node *)w_malloc(sizeof(node));
    n->value = value;
    n->parent = NULL;
    n->r_child = NULL;
    n->l_child = NULL;
    return n;
}

/**
 * \brief Appends a child to the given node. (w_malloc)
 * \param n The node to append the child to.
 * \param child The child to append.
 */
void node_append(node *n, node *child) {
    if (n->l_child == NULL) {
        n->l_child = child;
    } else if (n->r_child == NULL) {
        n->r_child = child;
    } else {
        fprintf(stderr, "Error: Node already has two children\n");
        exit(1);
    }
    child->parent = n;
}

/**
 * \brief Destroys the given node. (w_free)
 * \param n The node to destroy.
 */
void node_destroy(node *n) {
    if (n->l_child != NULL) node_destroy(n->l_child);
    if (n->r_child != NULL) node_destroy(n->r_child);
    w_free(n);
}
