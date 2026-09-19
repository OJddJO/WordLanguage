#include <werror.h>
#include "WParser.h"
#include "WLexer.h"

int parserInit(const char *src, WParser *parser) {
    WParser ret = {0};

    if (!lexerInit(src, &ret.lexer)) {
        return 0;
    }

    if (!stackInit(&ret.tokens, sizeof(WToken))) {
        lexerDestroy(&ret.lexer);
        return 0;
    }

    if (!uArrayInit(&ret.program, sizeof(WP_ASTNode))) {
        lexerDestroy(&ret.lexer);
        stackDestroy(&ret.tokens);
        return 0;
    }

    *parser = ret;
    return 1;
}

static WP_ASTNode *consumeBlock();
static WP_ASTNode *consumeIf();
static WP_ASTNode *consumeElse();
static WP_ASTNode *consumeWhile();
static WP_ASTNode *consumeContinue();
static WP_ASTNode *consumeBreak();
static WP_ASTNode *consumeVar();
static WP_ASTNode *consumeDef();
static WP_ASTNode *consumeClass();
static WP_ASTNode *consumeReturn();
static WP_ASTNode *consumeTrue();
static WP_ASTNode *consumeFalse();
static WP_ASTNode *consumeMemory();
static WP_ASTNode *consumeSizeof();

static WP_ASTNode *consumeKeyword(WToken *token) {
    switch (token->as.kw) {
        case (WTOKKW_IF): return consumeIf();
        case (WTOKKW_ELSE): return consumeElse();
        case (WTOKKW_WHILE): return consumeWhile();
        case (WTOKKW_CONTINUE): return consumeContinue();
        case (WTOKKW_BREAK): return consumeBreak();
        case (WTOKKW_VAR): return consumeVar();
        case (WTOKKW_DEF): return consumeDef();
        case (WTOKKW_MEMORY): return consumeMemory();
        case (WTOKKW_SIZEOF): return consumeSizeof();
        case (WTOKKW_TRUE): return consumeTrue();
        case (WTOKKW_FALSE): return consumeFalse();
        default: return NULL;
    }
}

static WP_ASTNode *consumeIf() {
    WP_ASTNode *ret = malloc(sizeof(WP_ASTNode));
    if (!ret) {
        PRINT_ERR("Failed to alloc if stmt\n");
        return NULL;
    }
    ret->as.ifStmt.condition = consumeBlock();
    if (!ret->as.ifStmt.condition) {
        PRINT_ERR("If syntax error\n");
        return NULL;
    }
    ret->as.ifStmt.thenBlock = consumeBlock();
    return ret;
}


WP_ASTNode *parserConsume(WToken *token) {
    switch (token->type) {
        case (WTOK_KEYWORD): {
            switch (token->as.kw) {
                case (WTOKKW_IF): {
                    return consumeIf();
                }
            }
        }
    }
}

// Parser tries to consume a block, if can't find a block NULL is returned
WP_ASTNode *parserConsumeBlock(WParser *parser) {
    WToken *tok = lexerNext(&parser->lexer);
    if (!tok) return NULL;
    if (tok->type != WTOK_PUNCTUATOR && tok->as.punc != WTOKPUNC_OPEN) {
        return NULL;
    }


}


int parserBuildAST(WParser *parser) {

}
