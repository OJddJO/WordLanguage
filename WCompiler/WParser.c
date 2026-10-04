#include <stdlib.h>
#include <werror.h>
#include "WParser.h"
#include "WLexer.h"

/**
 * @brief Init a parser struct
 * @param src The path to the file to parse
 * @param parser The struct to initialize
 * @return 1 if success, 0 else
 */
int parserInit(const char *src, WParser *parser) {
    WParser ret = {0};

    if (!lexerInit(src, &ret.lexer)) {
        return 0;
    }

    if (!stackInit(&ret.nodes, sizeof(WToken))) {
        lexerDestroy(&ret.lexer);
        return 0;
    }

    if (!uArrayInit(&ret.program, sizeof(ASTNode))) {
        lexerDestroy(&ret.lexer);
        stackDestroy(&ret.nodes);
        return 0;
    }

    *parser = ret;
    return 1;
}

/**
 * @brief Destroy an AST
 * @param ast The AST to destroy
 * @param freeRoot If the root should be freed or not
 */
void ASTDestroy(ASTNode *ast, bool freeRoot) {
    if (!ast) return;
    switch (ast->type) {
        case (ASTNODE_LITERAL): {
            if (ast->as.lit.type == LIT_STR) {
                free(ast->as.lit.s);
            }
            break;
        }
        case (ASTNODE_IDENTIFIER): {
            free(ast->as.id);
            break;
        }
        case (ASTNODE_BLOCK): {
            for (size_t i = 0; i < ast->as.block.count; i++) {
                ASTDestroy(&ast->as.block.stmt[i], false);
            }
            free(ast->as.block.stmt);
            break;
        }
        case (ASTNODE_UNARY_OP): {
            ASTDestroy(ast->as.unaryOp.operand, true);
            break;
        }
        case (ASTNODE_BINARY_OP): {
            ASTDestroy(ast->as.binaryOp.left, true);
            ASTDestroy(ast->as.binaryOp.right, true);
            break;
        }
        case (ASTNODE_CLASS_DEF): {
            for (size_t i = 0; i < ast->as.classDef.fieldsCount; i++) {
                ASTDestroy(&ast->as.classDef.fields[i], false);
            }
            for (size_t i = 0; i < ast->as.classDef.methodsCount; i++) {
                ASTDestroy(&ast->as.classDef.methods[i], false);
            }
            break;
        }
        case (ASTNODE_FUNC_DEF): {
            ASTDestroy(ast->as.funcDef.retType, true);
            ASTDestroy(ast->as.funcDef.name, true);
            ASTDestroy(ast->as.funcDef.params, true);
            ASTDestroy(ast->as.funcDef.body, true);
            break;
        }
        case (ASTNODE_VAR_DECL): {
            ASTDestroy(ast->as.varDecl.typeName, true);
            ASTDestroy(ast->as.varDecl.varName, true);
            break;
        }
        case (ASTNODE_IF): {
            ASTDestroy(ast->as.ifStmt.condition, true);
            ASTDestroy(ast->as.ifStmt.thenBlock, true);
            if (ast->as.ifStmt.elseBlock) ASTDestroy(ast->as.ifStmt.elseBlock, true);
            break;
        }
        case (ASTNODE_WHILE): {
            ASTDestroy(ast->as.whileStmt.condition, true);
            ASTDestroy(ast->as.whileStmt.body, true);
            break;
        }
        case (ASTNODE_CALL): {
            ASTDestroy(ast->as.call.args, true);
            break;
        }
        case (ASTNODE_RETURN): {
            ASTDestroy(ast->as.retStmt.value, true);
            break;
        }
    }

    if (freeRoot) free(ast);
}

/**
 * @brief Init a node
 * @param type The type of node to init
 * @param out The node to init
 * @return 1 if success, 0 else
 */
inline static int ASTNodeInit(ASTNodeType type, ASTNode *out) {
    ASTNode ret = {0};
    ret.type = type;
    switch (type) {
        case (ASTNODE_BLOCK): {
            ret.as.block = (struct _ASTBlock){
                .stmt = calloc(1, sizeof(ASTNode))
            };
            if (!ret.as.block.stmt) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_UNARY_OP): {
            ret.as.unaryOp = (struct _ASTUnary){
                .operand = calloc(1, sizeof(ASTNode)),
            };
            if (!ret.as.unaryOp.operand) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_BINARY_OP): {
            ret.as.binaryOp = (struct _ASTBinary){
                .left = calloc(1, sizeof(ASTNode)),
                .right = calloc(1, sizeof(ASTNode)),
                .op = calloc(1, sizeof(ASTNode)),
            };
            if (!(ret.as.binaryOp.left && ret.as.binaryOp.right && ret.as.binaryOp.op)) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_CLASS_DEF): {
            ret.as.classDef = (struct _ASTClass){
                .name = calloc(1, sizeof(ASTNode)),
                .fields = calloc(1, sizeof(ASTNode)),
                .methods = calloc(1, sizeof(ASTNode)),
            };
            if (!(ret.as.classDef.fields && ret.as.classDef.methods)) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_FUNC_DEF): {
            ret.as.funcDef = (struct _ASTFunc){
                .retType = calloc(1, sizeof(ASTNode)),
                .name = calloc(1, sizeof(ASTNode)),
                .params = calloc(1, sizeof(ASTNode)),
                .body = calloc(1, sizeof(ASTNode)),
            };
            if (!(ret.as.funcDef.retType && ret.as.funcDef.name && ret.as.funcDef.params && ret.as.funcDef.body)) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_VAR_DECL): {
            ret.as.varDecl = (struct _ASTVar){
                .typeName = calloc(1, sizeof(ASTNode)),
                .varName = calloc(1, sizeof(ASTNode)),
            };
            if (!(ret.as.varDecl.typeName && ret.as.varDecl.varName)) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_IF): {
            ret.as.ifStmt = (struct _ASTIf){
                .condition = calloc(1, sizeof(ASTNode)),
                .thenBlock = calloc(1, sizeof(ASTNode)),
            };
            if (!(ret.as.ifStmt.condition && ret.as.ifStmt.thenBlock)) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_WHILE): {
            ret.as.whileStmt = (struct _ASTWhile){
                .condition = calloc(1, sizeof(ASTNode)),
                .body = calloc(1, sizeof(ASTNode)),
            };
            if (!(ret.as.whileStmt.condition && ret.as.whileStmt.body)) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
        case (ASTNODE_CALL): {
            ret.as.call = (struct _ASTCall){
                .args = calloc(1, sizeof(ASTNode)),
            };
            if (!ret.as.call.args) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
    }

    *out = ret;
    return 1;
}

static int parserConsume(WParser *parser);
/**
 * @brief Consume an identifier
 * @param parser The parser which should consume the identifier
 * @param tok The token corresponding to the supposed identifier
 * @param node Where to store the output `ASTNode`
 * @return 1 if success, non-positive else
 */
static int consumeIdentifier(WParser *parser, WToken *tok, ASTNode *node) {
    if (tok->type != WTOK_IDENTIFIER) return ERR_GENERIC;

    node->type = ASTNODE_IDENTIFIER;
    node->as.id = tok->as.id;

    return 1;
}

static int consumeLitteral(WParser *parser, WToken *tok, ASTNode *node) {
    if (tok->type != WTOK_LITERAL) return ERR_GENERIC;

    node->type = ASTNODE_LITERAL;
    node->as.lit = tok->as.lit;

    return 1;
}

static int consumeIf(WParser *parser, ASTNode *node);
static int consumeElse(WParser *parser, ASTNode *node);
static int consumeWhile(WParser *parser, ASTNode *node);
static int consumeContinue(WParser *parser, ASTNode *node);
static int consumeBreak(WParser *parser, ASTNode *node);
static int consumeVar(WParser *parser, ASTNode *node);
static int consumeFunc(WParser *parser, ASTNode *node);
static int consumeClass(WParser *parser, ASTNode *node);
static int consumeReturn(WParser *parser, ASTNode *node);
static int consumeTrue(WParser *parser, ASTNode *node);
static int consumeFalse(WParser *parser, ASTNode *node);
static int consumeBlock(WParser *parser, WToken *token, ASTNode *node);

/**
 * @brief Consume a keyword
 * @param parser The parser which should consume the keyword
 * @param token The token corresponding to the supposed keyword
 * @param out Where to store the output `ASTNode`
 * @return 1 if success, non-positive else
 */
static int consumeKeyword(WParser *parser, WToken *token, ASTNode *node) {
    if (token->type != WTOK_KEYWORD) return 0;
    switch (token->as.kw) {
        case (WTOKKW_IF): return consumeIf(parser, node);
        case (WTOKKW_ELSE): return consumeElse(parser, node);
        case (WTOKKW_WHILE): return consumeWhile(parser, node);
        case (WTOKKW_CONTINUE): return consumeContinue(parser, node);
        case (WTOKKW_BREAK): return consumeBreak(parser, node);
        case (WTOKKW_VAR): return consumeVar(parser, node);
        case (WTOKKW_FUNC): return consumeFunc(parser, node);
        case (WTOKKW_CLASS): return consumeClass(parser, node);
        case (WTOKKW_TRUE): return consumeTrue(parser, node);
        case (WTOKKW_FALSE): return consumeFalse(parser, node);
        default: return 0;
    }
}

static int consumeIf(WParser *parser, ASTNode *node) {
    if (!ASTNodeInit(ASTNODE_IF, node)) {
        PRINT_ERR("if node initialization failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;

    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.ifStmt.condition))) {
        PRINT_ERR("if condition syntax error\n");
        status = ERR_PARSE_IF_COND;
        goto ret;
    }

    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.ifStmt.thenBlock))) {
        PRINT_ERR("if then syntax error\n");
        status = ERR_PARSE_IF_THEN;
        goto ret;
    }

ret:
    if (IS_ERR(status)) ASTDestroy(node, false);
    return status;
}

static int consumeElse(WParser *parser, ASTNode *node) {
    if (node->type != ASTNODE_IF) {
        PRINT_ERR("else statement should match a if statement\n");
        return ERR_PARSE_ELSE_NO_IF;
    }

    node->as.ifStmt.elseBlock = calloc(1, sizeof(ASTNode));
    if (!node->as.ifStmt.elseBlock) {
        PRINT_ERR("else then block alloc error\n");
        return ERR_PARSE_ELSE_THEN;
    }
    WToken tok;
    if (lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node))) {
        PRINT_ERR("else syntax error\n");
        return ERR_PARSE_ELSE_THEN;
    }

    return 1;
}

static int consumeWhile(WParser *parser, ASTNode *node) {
    if (!ASTNodeInit(ASTNODE_WHILE, node)) {
        PRINT_ERR("while node initialization failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;

    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.whileStmt.condition))) {
        status = ERR_PARSE_WHILE_COND;
        goto ret;
    }

    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.whileStmt.body))) {
        status = ERR_PARSE_WHILE_THEN;
        goto ret;
    }

ret:
    if (IS_ERR(status)) {
        PRINT_ERR("while syntax error\n");
        ASTDestroy(node, false);
    }
    return status;
}

static int consumeContinue(WParser *parser, ASTNode *node) {
    node->type = ASTNODE_CONTINUE;
    return 1;
}

static int consumeBreak(WParser *parser, ASTNode *node) {
    node->type = ASTNODE_BREAK;
    return 1;
}

static int consumeVar(WParser *parser, ASTNode *node) {
    if (!ASTNodeInit(ASTNODE_VAR_DECL, node)) {
        PRINT_ERR("var node init failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;
    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.varDecl.typeName))) {
        status = ERR_PARSE_VAR_TYPE;
        goto ret;
    }

    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.varDecl.varName))) {
        status = ERR_PARSE_VAR_ID;
        goto ret;
    }

ret:
    if (IS_ERR(status)) {
        PRINT_ERR("var syntax error\n");
        ASTDestroy(node, false);
    }
    return status;
}

static int consumeFunc(WParser *parser, ASTNode *node) {
    if (!ASTNodeInit(ASTNODE_FUNC_DEF, node)) {
        PRINT_ERR("func node init failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;
    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.funcDef.retType))) {
        status = ERR_PARSE_FUNC_TYPE;
        goto ret;
    }
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.funcDef.name))) {
        status = ERR_PARSE_FUNC_ID;
        goto ret;
    }

    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.funcDef.params))) {
        status = ERR_PARSE_FUNC_ARGS;
        goto ret;
    }

    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.funcDef.body))) {
        status = ERR_PARSE_FUNC_BODY;
        goto ret;
    }

ret:
    if (IS_ERR(status)) {
        PRINT_ERR("func syntax error\n");
        ASTDestroy(node, false);
    }
    return status;
}

static int consumeClass(WParser *parser, ASTNode *node) {
    if (!ASTNodeInit(ASTNODE_CLASS_DEF, node)) {
        PRINT_ERR("class node init failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;
    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.classDef.name))) {
        status = ERR_PARSE_CLASS_ID;
        goto ret;
    }
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.classDef.fields))) {
        status = ERR_PARSE_CLASS_FIELDS;
        goto ret;
    }
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.classDef.methods))) {
        status = ERR_PARSE_CLASS_METHODS;
        goto ret;
    }

ret:
    if (IS_ERR(status)) {
        PRINT_ERR("class syntax error\n");
        ASTDestroy(node, false);
    }
    return status;
}

static int consumeReturn(WParser *parser, ASTNode *node) {
    if (!ASTNodeInit(ASTNODE_RETURN, node)) {
        PRINT_ERR("return node init failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;
    WToken tok;
    if (!lexerNext(&parser->lexer, &tok)) {
        status = ERR_GENERIC;
        goto ret;
    }
    if (!(tok.type == WTOK_PUNCTUATOR && tok.as.punc == WTOKPUNC_COMMA) && IS_ERR(consumeIdentifier(parser, &tok, node))) {
        status = ERR_PARSE_RETURN_VALUE;
        goto ret;
    }

ret:
    if (IS_ERR(status)) {
        PRINT_ERR("return syntax error\n");
        ASTDestroy(node, false);
    }
    return status;
}

// Parser tries to consume a block, if can't find a block NULL is returned
static int consumeBlock(WParser *parser, WToken *token, ASTNode *out) {

}

int parserConsume(WParser *parser) {
}

int parserBuildAST(WParser *parser) {

}
