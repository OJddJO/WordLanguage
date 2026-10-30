#ifndef __WPARSER_H__
#define __WPARSER_H__

#include <stdint.h>

#include <array.h>
#include <stack.h>
#include <array.h>
#include "WLexer.h"
#include "werror.h"

typedef enum _ASTNodeType : uint8_t {
    ASTNODE_NOT_INIT,

    ASTNODE_LITERAL,
    ASTNODE_IDENTIFIER,

    ASTNODE_BLOCK,

    ASTNODE_UNARY_OP,
    ASTNODE_BINARY_OP,

    ASTNODE_CLASS_DEF,
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
    union {
        WLiteral    lit;
        char        *id;

        struct _ASTBlock {
            Array   stmts;     // array of ASTNode
        } block;
        struct _ASTOp {         // USED ONLY FOR CHECKING OP PRECEDENCE
            WTokOp  op;
        } op;
        struct _ASTUnary {
            WTokOp  unary;
            ASTNode *operand;   // single operand ASTNode
        } unaryOp;
        struct _ASTBinary {
            WTokOp  op;
            ASTNode *left;      // single operand ASTNode
            ASTNode *right;     // single operand ASTNode
        } binaryOp;
        struct _ASTClass {
            ASTNode *name;
            ASTNode *fields;    // single block of varDecl ASTNode
            ASTNode *methods;   // single block of funcDef ASTNode
        } classDef;
        struct _ASTFunc {
            ASTNode *retType;   // single identifier (type name) ASTNode
            ASTNode *name;      // single identifier ASTNode
            ASTNode *params;    // single block ASTNode of varDecl ASTNode
            ASTNode *body;      // single block ASTNode
        } funcDef;
        struct _ASTVar {
            ASTNode *typeName;  // single identifier (type name) ASTNode
            ASTNode *varName;   // single identifier ASTNode
        } varDecl;
        struct _ASTIf {
            ASTNode *condition; // single block ASTNode
            ASTNode *thenBlock; // single block ASTNode
            ASTNode *elseBlock; // NULL by default, single block ASTNode
        } ifStmt;
        struct _ASTWhile {
            ASTNode *condition; // single block ASTNode
            ASTNode *body;      // single block ASTNode
        } whileStmt;
        struct _ASTCall {
            ASTNode *args;      // single block ASTNode of value ASTNode
        } call;
        struct _ASTRet {
            ASTNode *value;     // NULL by default, single value ASTNode
        } retStmt;
    } as;
    ASTNodeType type;
};

typedef struct _WParser {
    WLexer      lexer;
    Array       program;    // Array of ASTs
} WParser;

bool parserInit(const char *src, WParser *parser);
void parserDestroy(WParser *parser);
Wstatus parserBuildAST(WParser *parser);
void ASTPrint(ASTNode *ast);
void parserPrintProgram(WParser *parser);

#endif
