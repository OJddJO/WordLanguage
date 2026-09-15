#ifndef __WPARSER_H__
#define __WPARSER_H__

#include <stdint.h>

#include <stack.h>
#include <unorderedArray.h>
#include "WLexer.h"

typedef enum _WP_ASTNodeType {
    WPNODE_LITERAL,
    WPNODE_IDENTIFIER,

    WPNODE_BLOCK,

    WPNODE_UNARY_OP,
    WPNODE_BINARY_OP,

    WPNODE_CLASS_DEF,
    WPNODE_MEMBER_ACCESS,

    WPNODE_FUNC_DEF,
    WPNODE_VAR_DECL,

    WPNODE_IF,
    WPNODE_WHILE,

    WPNODE_CALL,
    WPNODE_RETURN,
} WP_ASTNodeType;

typedef struct _WP_ASTNode WP_ASTNode;
struct _WP_ASTNode {
    WP_ASTNodeType type;
    union {
        WLiteral    lit;
        char        *id;

        struct {
            WP_ASTNode  **items;
            size_t      count;
        } block;
        struct {
            WTokOp      unary;
            WP_ASTNode  *operand;
        } unaryOp;
        struct {
            WTokOp      op;
            WP_ASTNode  *left;
            WP_ASTNode  *right;
        } binaryOp;
        struct {
            WP_ASTNode  **fields;
            WP_ASTNode  **methods;
        } classDef;
        struct {
            WP_ASTNode  *object;
            char        *member;
        } member;
        struct {
            char        *retType;
            char        *name;
            WP_ASTNode  *params;
            WP_ASTNode  *body;
        } funcDef;
        struct {
            char        *typeName;
            char        *varName;
            WP_ASTNode  *init;
        } varDecl;
        struct {
            WP_ASTNode  *condition;
            WP_ASTNode  *thenBlock;
            WP_ASTNode  *elseBlock;
        } ifStmt;
        struct {
            WP_ASTNode  *condition;
            WP_ASTNode  *block;
        } whileStmt;
        struct {
            WP_ASTNode  *callee;
            WP_ASTNode  *args;
            uint64_t    argc;
        } call;
        struct {
            WP_ASTNode  *value;
        } retStmt;
    } as;
};

typedef struct _WParser {
    WLexer      lexer;
    Stack       tokens;
    UArray      program;
} WParser;

#endif
