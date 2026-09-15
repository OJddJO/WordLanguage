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
static WP_ASTNode *consumeVoid();
static WP_ASTNode *consumeInt();
static WP_ASTNode *consumeFloat();
static WP_ASTNode *consumeBool();
static WP_ASTNode *consumeString();
static WP_ASTNode *consumeTrue();
static WP_ASTNode *consumeFalse();

WP_ASTNode *parserConsume(WToken *token) {
    switch (token->type) {
        case (WTOK_KEYWORD): {

        }
    }
}

static WP_ASTNode *parserConsumeIf() {

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
