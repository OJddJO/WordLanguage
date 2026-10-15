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
bool parserInit(const char *src, WParser *parser) {
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
inline static bool ASTNodeInit(ASTNodeType type, ASTNode *out) {
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

static Wstatus parserConsume(WParser *parser);

#define TOK_IS_COMMA(tok) ((tok).type == WTOK_PUNCTUATOR && (tok).as.punc == WTOKPUNC_COMMA)

/**
 * @brief Consume an identifier
 * @param parser The parser which should consume the identifier
 * @param tok The token corresponding to the supposed identifier
 * @param node Where to store the output `ASTNode`
 * @return 1 if success, non-positive else
 */
static Wstatus consumeIdentifier(WParser *parser, WToken *tok, ASTNode *node) {
    if (tok->type != WTOK_IDENTIFIER) return ERR_GENERIC;
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }

    node->type = ASTNODE_IDENTIFIER;
    node->as.id = tok->as.id;

    return 1;
}

static Wstatus consumeLitteral(WParser *parser, WToken *tok, ASTNode *node) {
    if (tok->type != WTOK_LITERAL) return ERR_GENERIC;
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }

    node->type = ASTNODE_LITERAL;
    node->as.lit = tok->as.lit;

    return 1;
}

static Wstatus consumeTrue(WParser *parser, WToken *tok, ASTNode *node) {
    if (tok->type != WTOK_KEYWORD && tok->as.kw != WTOKKW_TRUE) return ERR_GENERIC;
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    node->type = ASTNODE_LITERAL;
    node->as.lit.i = 1;

    return 1;
}

static Wstatus consumeFalse(WParser *parser, WToken *tok, ASTNode *node) {
    if (tok->type != WTOK_KEYWORD && tok->as.kw != WTOKKW_FALSE) return ERR_GENERIC;
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    node->type = ASTNODE_LITERAL;
    node->as.lit.i = 0;

    return 1;
}

static Wstatus consumeValue(WParser *parser, WToken *tok, ASTNode *node) {
    if (!IS_ERR(consumeIdentifier(parser, tok, node))) {
        return 1;
    }
    if (!IS_ERR(consumeLitteral(parser, tok, node))) {
        return 1;
    }
    if (!IS_ERR(consumeTrue(parser, tok, node))) {
        return 1;
    }
    if (!IS_ERR(consumeFalse(parser, tok, node))) {
        return 1;
    }
    return ERR_GENERIC;
}

static Wstatus consumeIf(WParser *parser, ASTNode *node);
static Wstatus consumeElse(WParser *parser, ASTNode *node);
static Wstatus consumeWhile(WParser *parser, ASTNode *node);
static Wstatus consumeContinue(WParser *parser, ASTNode *node);
static Wstatus consumeBreak(WParser *parser, ASTNode *node);
static Wstatus consumeVar(WParser *parser, ASTNode *node);
static Wstatus consumeFunc(WParser *parser, ASTNode *node);
static Wstatus consumeClass(WParser *parser, ASTNode *node);
static Wstatus consumeReturn(WParser *parser, ASTNode *node);
static Wstatus consumeBlock(WParser *parser, WToken *token, ASTNode *node);

/**
 * @brief Consume a keyword
 * @param parser The parser which should consume the keyword
 * @param token The token corresponding to the supposed keyword
 * @param out Where to store the output `ASTNode`
 * @return 1 if success, non-positive else
 */
static Wstatus consumeKeyword(WParser *parser, WToken *token, ASTNode *node) {
    if (token->type != WTOK_KEYWORD) return 0;
    switch (token->as.kw) {
        case (WTOKKW_IF): return consumeIf(parser, node);
        // case (WTOKKW_ELSE): return 0; // Handled by if
        case (WTOKKW_WHILE): return consumeWhile(parser, node);
        case (WTOKKW_CONTINUE): return consumeContinue(parser, node);
        case (WTOKKW_BREAK): return consumeBreak(parser, node);
        case (WTOKKW_VAR): return consumeVar(parser, node);
        case (WTOKKW_FUNC): return consumeFunc(parser, node);
        case (WTOKKW_CLASS): return consumeClass(parser, node);
        case (WTOKKW_TRUE): return consumeTrue(parser, token, node);
        case (WTOKKW_FALSE): return consumeFalse(parser, token, node);
        default: return 0;
    }
}

static Wstatus consumeIf(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }

    if (!ASTNodeInit(ASTNODE_IF, node)) {
        PRINT_ERR("if node initialization failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    WToken tok;
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.ifStmt.condition))) {
        PRINT_ERR("if condition syntax error\n");
        status = ERR_PARSE_IF_COND;
        goto ret;
    }

    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.ifStmt.thenBlock))) {
        PRINT_ERR("if then syntax error\n");
        status = ERR_PARSE_IF_THEN;
        goto ret;
    }

    if (lexerToken(&parser->lexer, &tok) && tok.type == WTOK_KEYWORD && tok.as.kw == WTOKKW_ELSE) {
        if (IS_ERR(status = consumeElse(parser, node))) {
            goto ret;
        }
    }

ret:
    if (IS_ERR(status)) ASTDestroy(node, false);
    return status;
}

static Wstatus consumeElse(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }

    node = calloc(1, sizeof(ASTNode));
    if (!node) {
        PRINT_ERR("else then block alloc error\n");
        return ERR_PARSE_ELSE_THEN;
    }
    WToken tok;
    if (lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node))) {
        PRINT_ERR("else syntax error\n");
        return ERR_PARSE_ELSE_THEN;
    }

    return 1;
}

static Wstatus consumeWhile(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    if (!ASTNodeInit(ASTNODE_WHILE, node)) {
        PRINT_ERR("while node initialization failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    WToken tok;
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.whileStmt.condition))) {
        status = ERR_PARSE_WHILE_COND;
        goto ret;
    }

    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.whileStmt.body))) {
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

static Wstatus consumeContinue(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    node->type = ASTNODE_CONTINUE;
    return 1;
}

static Wstatus consumeBreak(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    node->type = ASTNODE_BREAK;
    return 1;
}

static Wstatus consumeVar(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    if (!ASTNodeInit(ASTNODE_VAR_DECL, node)) {
        PRINT_ERR("var node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    WToken tok;
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.varDecl.typeName))) {
        status = ERR_PARSE_VAR_TYPE;
        goto ret;
    }

    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.varDecl.varName))) {
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

static Wstatus consumeFunc(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    if (!ASTNodeInit(ASTNODE_FUNC_DEF, node)) {
        PRINT_ERR("func node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    WToken tok;
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.funcDef.retType))) {
        status = ERR_PARSE_FUNC_TYPE;
        goto ret;
    }
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.funcDef.name))) {
        status = ERR_PARSE_FUNC_ID;
        goto ret;
    }

    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.funcDef.params))) {
        status = ERR_PARSE_FUNC_ARGS;
        goto ret;
    }

    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.funcDef.body))) {
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

static Wstatus consumeClass(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    if (!ASTNodeInit(ASTNODE_CLASS_DEF, node)) {
        PRINT_ERR("class node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    WToken tok;
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok, node->as.classDef.name))) {
        status = ERR_PARSE_CLASS_ID;
        goto ret;
    }
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.classDef.fields))) {
        status = ERR_PARSE_CLASS_FIELDS;
        goto ret;
    }
    if (!lexerToken(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok, node->as.classDef.methods))) {
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

static Wstatus consumeReturn(WParser *parser, ASTNode *node) {
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    if (!ASTNodeInit(ASTNODE_RETURN, node)) {
        PRINT_ERR("return node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    WToken tok;
    if (!lexerToken(&parser->lexer, &tok)) {
        status = ERR_GENERIC;
        goto ret;
    }
    if (!TOK_IS_COMMA(tok) && IS_ERR(consumeValue(parser, &tok, node))) {
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

static bool opIsUnary(WTokOp op) {
    return (
        op == WTOKOP_POS
        || op == WTOKOP_NEG
        || op == WTOKOP_NOT
        || op == WTOKOP_BNOT
    );
}

static Wstatus consumeStatement(WParser *parser) {
    Wstatus status = 1;

    Stack operands = {0};
    if (!stackInit(&operands, sizeof(ASTNode))) {
        THROW_ERR(ERR_INTERNAL, ret, "failed to init operands stack\n");
    }
    Stack operators = {0};
    if (!stackInit(&operands, sizeof(ASTNode))) {
        THROW_ERR(ERR_INTERNAL, ret, "failed to init operators stack\n");
    }

    while (lexerHasNext(&parser->lexer)) {
        WToken token;
        if (!lexerToken(&parser->lexer, &token)) {
            return ERR_GENERIC;
        }
        if (token.type == WTOK_PUNCTUATOR
            && ( token.as.punc == WTOKPUNC_COMMA
            || token.as.punc == WTOKPUNC_CLOSE
            || token.as.punc == WTOKPUNC_CALLCLOSE
        )) {
            if (token.as.punc == WTOKPUNC_COMMA) {
                if (!lexerForward(&parser->lexer)) {
                    THROW_ERR(ERR_INTERNAL, ret, "lexer failed to forward\n");
                }
            }
            goto ret;
        }

        if (token.type == WTOK_OPERATOR) {
            ASTNode *buf = stackBump(&operators);
            if (!buf) {
                THROW_ERR(ERR_INTERNAL, ret, "failed to bump operator stack\n");
            }
            if (opIsUnary(token.as.op)) {
                buf->type = ASTNODE_UNARY_OP;
                buf->as.unaryOp.unary = token.as.op;
            } else {
                buf->type = ASTNODE_BINARY_OP;
                buf->as.binaryOp.op = token.as.op;
            }
            break;
        } else {
            ASTNode *buf = stackBump(&operands);
            if (!buf) {
                THROW_ERR(ERR_INTERNAL, ret, "failed to bump stack\n");
            }
            switch (token.type) {
                case (WTOK_KEYWORD): {
                    status = consumeKeyword(parser, &token, buf);
                    break;
                }
                case (WTOK_PUNCTUATOR): {
                    break;
                }
                case (WTOK_IDENTIFIER): {
                    buf->type = ASTNODE_IDENTIFIER;
                    buf->as.id = token.as.id;
                    break;
                }
                case (WTOK_LITERAL): {
                    buf->type = ASTNODE_LITERAL;
                    switch (buf->as.lit.type) {
                        case (LIT_INT): {
                            buf->as.lit.i = token.as.lit.i;
                            break;
                        }
                        case (LIT_FLOAT): {
                            buf->as.lit.f = token.as.lit.f;
                            break;
                        }
                        case (LIT_STR): {
                            buf->as.lit.s = token.as.lit.s;
                            break;
                        }
                    }
                    break;
                }
                default: {
                    THROW_ERR(ERR_INTERNAL, ret, "unidentified token\n");
                    break;
                }
            }
        }
        if (IS_ERR(status)) goto ret;
    }

ret:
    if (IS_ERR(status)) {
        stackDestroy(&operands);
        stackDestroy(&operators);
    }
    return status;
}

// Parser tries to consume a block, if can't find a block NULL is returned
static Wstatus consumeBlock(WParser *parser, WToken *token, ASTNode *out) {

}


int parserConsume(WParser *parser) {

}

int parserBuildAST(WParser *parser) {

}
