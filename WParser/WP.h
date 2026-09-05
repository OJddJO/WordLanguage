#ifndef __WP_H__
#define __WP_H__

#include <stdint.h>

typedef enum _WP_ASTNodeType {
    NODE_PROGRAM,
    NODE_CLASS_DEF,
    NODE_FUNC_DEF,
    NODE_VAR_DECL,
    NODE_IF,
    NODE_WHILE,
    NODE_RETURN,
    NODE_BINARY_OP,
    NODE_MEMBER_ACCESS,
    NODE_CALL,
    NODE_IDENTIFIER,
    NODE_LITERAL,
} WP_ASTNodeType;

typedef struct _WP_ASTNode WP_ASTNode;
struct _WP_ASTNode {
    WP_ASTNodeType type;
    union {
        struct {
            WP_ASTNode **items;
            size_t  count;
        } block;
        struct {
            char        *op;
            WP_ASTNode  *left;
            WP_ASTNode  *right;
        } binOp;
        struct {
            WP_ASTNode  *object;
            char        *member;
        } member;
        struct {
            WP_ASTNode  *callee;
            WP_ASTNode  *args;
            uint64_t    argc;
        } call;
        struct {
            char        *typeName;
            char        *varName;
            WP_ASTNode  *init;
        } varDecl;
        struct {
            char        *retType;
            char        *name;
            WP_ASTNode  *params;
            WP_ASTNode  *body;
        } funcDef;

        char    *stringVal;
        int64_t *intVal;
    } as;
};

#endif
