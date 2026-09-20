#include <werror.h>
#include "WParser.h"
#include "WLexer.h"

int parserInit(const char *src, WParser *parser) {
    WParser ret = {0};

    if (!lexerInit(src, &ret.lexer)) {
        return 0;
    }

    if (!stackInit(&ret.nodes, sizeof(WToken))) {
        lexerDestroy(&ret.lexer);
        return 0;
    }

    if (!uArrayInit(&ret.program, sizeof(WP_ASTNode))) {
        lexerDestroy(&ret.lexer);
        stackDestroy(&ret.nodes);
        return 0;
    }

    *parser = ret;
    return 1;
}

static int parserConsume(WParser *parser);

static int consumeBlock(WParser *parser);
static int consumeIf(WParser *parser);
static int consumeElse(WParser *parser);
static int consumeWhile(WParser *parser);
static int consumeContinue(WParser *parser);
static int consumeBreak(WParser *parser);
static int consumeVar(WParser *parser);
static int consumeDef(WParser *parser);
static int consumeClass(WParser *parser);
static int consumeReturn(WParser *parser);
static int consumeTrue(WParser *parser);
static int consumeFalse(WParser *parser);
static int consumeMemory(WParser *parser);
static int consumeSizeof(WParser *parser);

static int consumeKeyword(WParser *parser, WToken *token) {
    switch (token->as.kw) {
        case (WTOKKW_IF): return consumeIf(parser);
        case (WTOKKW_ELSE): return consumeElse(parser);
        case (WTOKKW_WHILE): return consumeWhile(parser);
        case (WTOKKW_CONTINUE): return consumeContinue(parser);
        case (WTOKKW_BREAK): return consumeBreak(parser);
        case (WTOKKW_VAR): return consumeVar(parser);
        case (WTOKKW_DEF): return consumeDef(parser);
        case (WTOKKW_MEMORY): return consumeMemory(parser);
        case (WTOKKW_SIZEOF): return consumeSizeof(parser);
        case (WTOKKW_TRUE): return consumeTrue(parser);
        case (WTOKKW_FALSE): return consumeFalse(parser);
        default: return NULL;
    }
}

static int consumeIf(WParser *parser) {
    WP_ASTNode node;

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("if condition syntax error\n");
        return ERR_PARSE_IF_COND;
    }
    node.as.ifStmt.condition = malloc(sizeof(WP_ASTNode));
    if (!node.as.ifStmt.condition) {
        PRINT_ERR("if cond block malloc error\n");
        return ERR_PARSE_IF_COND;
    }
    if (!stackPop(&parser->nodes, node.as.ifStmt.condition)) {
        PRINT_ERR("failed to retrieve if cond block\n");
        return ERR_PARSE_IF_COND;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("if then syntax error\n");
        return ERR_PARSE_IF_THEN;
    }
    node.as.ifStmt.thenBlock = malloc(sizeof(WP_ASTNode));
    if (!node.as.ifStmt.thenBlock) {
        PRINT_ERR("if then block malloc error\n");
        return ERR_PARSE_IF_THEN;
    }
    if (!stackPop(&parser->nodes, node.as.ifStmt.thenBlock)) {
        PRINT_ERR("failed to retrieve if then block\n");
        return ERR_PARSE_IF_THEN;
    }

    node.as.ifStmt.elseBlock = NULL;
    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push if statement to stack\n");
        return ERR_IF;
    }

    return 1;
}

static int consumeElse(WParser *parser) {

}

// Parser tries to consume a block, if can't find a block NULL is returned
static int consumeBlock(WParser *parser) {
    WToken *tok = lexerNext(&parser->lexer);
    if (!tok) return NULL;
    if (tok->type != WTOK_PUNCTUATOR && tok->as.punc != WTOKPUNC_OPEN) {
        return NULL;
    }


}

int parserConsume(WParser *parser) {
    WToken *token = lexerNext(&parser->lexer);
    switch (token->type) {
        case (WTOK_KEYWORD): return consumeKeyword(parser, token);
    }
}

int parserBuildAST(WParser *parser) {

}
