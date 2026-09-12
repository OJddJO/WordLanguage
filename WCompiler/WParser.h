#ifndef __WPARSER_H__
#define __WPARSER_H__

#include <stdint.h>

#include "WLexer.h"

typedef enum _WP_ASTNodeType {
    WPNODE_CLASS_DEF,
    WPNODE_FUNC_DEF,
    WPNODE_VAR_DECL,
    WPNODE_IF,
    WPNODE_WHILE,
    WPNODE_RETURN,
    WPNODE_BINARY_OP,
    WPNODE_MEMBER_ACCESS,
    WPNODE_CALL,
    WPNODE_IDENTIFIER,
    WPNODE_LITERAL,
} WP_ASTNodeType;

typedef struct _WP_ASTNode WP_ASTNode;
struct _WP_ASTNode {
    WP_ASTNodeType type;
    union {
        struct {
            WP_ASTNode  **items;
            size_t      count;
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

        int64_t *intVal;
        double  *floatVal;
    } as;
};

typedef struct _WParser {
    WLexer      lexer;
} WParser;

#endif
