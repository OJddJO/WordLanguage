#ifndef __WPARSER_H__
#define __WPARSER_H__

#include <stdint.h>

#include <stack.h>
#include <unorderedArray.h>
#include "WLexer.h"

typedef enum _ASTNodeType {
    ASTNODE_NOT_INIT,

    ASTNODE_LITERAL,
    ASTNODE_IDENTIFIER,

    ASTNODE_BLOCK,

    ASTNODE_UNARY_OP,
    ASTNODE_BINARY_OP,

    ASTNODE_CLASS_DEF,
    ASTNODE_MEMBER_ACCESS,

    ASTNODE_FUNC_DEF,
    ASTNODE_VAR_DECL,

    ASTNODE_IF,
    ASTNODE_WHILE,
    ASTNODE_CONTINUE,
    ASTNODE_BREAK,

    ASTNODE_CALL,
    ASTNODE_RETURN,
} ASTNodeType;

typedef struct _ASTNode ASTNode;
struct _ASTNode {
    ASTNodeType type;
    union {
        WLiteral    lit;
        char        *id;

        struct {
            ASTNode **stmt;     // array of ASTNode
            size_t  count;
        } block;
        struct {
            WTokOp  unary;
            ASTNode *operand;   // single operand ASTNode
        } unaryOp;
        struct {
            WTokOp  op;
            ASTNode *left;      // single operand ASTNode
            ASTNode *right;     // single operand ASTNode
        } binaryOp;
        struct {
            ASTNode **fields;   // array of varDecl ASTNode
            size_t  fieldsCount;
            ASTNode **methods;  // array of funcDef ASTNode
            size_t  methodsCount;
        } classDef;
        struct {
            ASTNode *object;    // single left operand (object) ASTNode
            ASTNode *memName;   // single right operand (member name) ASTNode
        } member;
        struct {
            ASTNode *retType;   // single identifier (type name) ASTNode
            ASTNode *name;      // single identifier ASTNode
            ASTNode *params;    // single block ASTNode of varDecl ASTNode
            ASTNode *body;      // single block ASTNode
        } funcDef;
        struct {
            ASTNode *typeName;  // single identifier (type name) ASTNode
            ASTNode *varName;   // single identifier ASTNode
        } varDecl;
        struct {
            ASTNode *condition; // single block ASTNode
            ASTNode *thenBlock; // single block ASTNode
            ASTNode *elseBlock; // NULL by default, single block ASTNode
        } ifStmt;
        struct {
            ASTNode *condition; // single block ASTNode
            ASTNode *body;      // single block ASTNode
        } whileStmt;
        struct {
            ASTNode *args;      // single block ASTNode of identifier ASTNode
            size_t  argc;
        } call;
        struct {
            ASTNode *value;     // single identifier ASTNode
        } retStmt;
    } as;
};

typedef struct _WParser {
    WLexer      lexer;
    Stack       nodes;
    UArray      program;
} WParser;

#endif
