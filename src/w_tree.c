#include "w_tree.h"

// Tree is not part of the WordLanguage, but it is used in the parser.

/**
 * \brief Initializes a new tree.
 * \return A new tree.
 */
W_Tree *tree_init() {
    W_Tree *t = (W_Tree *)malloc(sizeof(W_Tree));
    t->value = NULL;
    t->left = NULL;
    t->right = NULL;
    return t;
}

/**
 * \brief Sets the value of a tree.
 * \param t The tree to set the value of.
 * \param value The value to set.
 */
void tree_set(W_Tree *t, W_Word *value) {
    t->value = value;
}

/**
 * \brief Destroys a tree.
 * \param t The tree to destroy.
 */
void tree_destroy(W_Tree *t) {
    if (t->left != NULL) {
        tree_destroy(t->left);
    }
    if (t->right != NULL) {
        tree_destroy(t->right);
    }
    free(t);
}