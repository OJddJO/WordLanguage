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
    WPNODE_CONTINUE,
    WPNODE_BREAK,

    WPNODE_CALL,
    WPNODE_RETURN,
} WP_ASTNodeType;

typedef struct _WP_ASTNode ASTNode;
struct _WP_ASTNode {
    WP_ASTNodeType type;
    union {
        WLiteral    lit;
        char        *id;

        struct {
            ASTNode **stmt;
            size_t  count;
        } block;
        struct {
            WTokOp  unary;
            ASTNode *operand;
        } unaryOp;
        struct {
            WTokOp  op;
            ASTNode *left;
            ASTNode *right;
        } binaryOp;
        struct {
            ASTNode **fields;
            ASTNode **methods;
        } classDef;
        struct {
            ASTNode *object;
            char    *member;
        } member;
        struct {
            char    *retType;
            char    *name;
            ASTNode *params;
            ASTNode *body;
        } funcDef;
        struct {
            char    *typeName;
            char    *varName;
            ASTNode *init;
        } varDecl;
        struct {
            ASTNode *condition;
            ASTNode *thenBlock;
            ASTNode *elseBlock;
        } ifStmt;
        struct {
            ASTNode *condition;
            ASTNode *block;
        } whileStmt;
        struct {
            ASTNode *callee;
            ASTNode *args;
            uint64_t argc;
        } call;
        struct {
            ASTNode *value;
        } retStmt;
    } as;
};

typedef struct _WParser {
    WLexer      lexer;
    Stack       nodes;
    UArray      program;
} WParser;

#endif
