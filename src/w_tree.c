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
 * \brief Returns a list of words in post-order.
 * \param t The tree to traverse.
 * \return A list of words in post-order.
 */
W_List *tree_post_order(W_Tree *t) {
    W_List *l = list_init();
    if (t->left != NULL) {
        W_List *left = tree_post_order(t->left);
        list_concat(l, left);
    }
    if (t->right != NULL) {
        W_List *right = tree_post_order(t->right);
        list_concat(l, right);
    }
    list_append(l, t->value);
    return l;
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