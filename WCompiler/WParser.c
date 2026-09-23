#include <stdlib.h>
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

    if (!uArrayInit(&ret.program, sizeof(ASTNode))) {
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
    ASTNode node;
    node.type = WPNODE_IF;
    node.as.ifStmt.condition = malloc(sizeof(ASTNode));
    if (!node.as.ifStmt.condition) {
        PRINT_ERR("if cond block malloc error\n");
        return ERR_PARSE_IF_COND;
    }
    node.as.ifStmt.thenBlock = malloc(sizeof(ASTNode));
    if (!node.as.ifStmt.thenBlock) {
        PRINT_ERR("if then block malloc error\n");
        return ERR_PARSE_IF_THEN;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("if condition syntax error\n");
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
    if (!stackPop(&parser->nodes, node.as.ifStmt.thenBlock)) {
        PRINT_ERR("failed to retrieve if then block\n");
        return ERR_PARSE_IF_THEN;
    }

    node.as.ifStmt.elseBlock = NULL;
    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push if statement to stack\n");
        return ERR_GENERIC;
    }

    return 1;
}

static int consumeElse(WParser *parser) {
    ASTNode *node = stackTop(&parser->nodes);
    if (!node || node->type != WPNODE_IF) {
        PRINT_ERR("else statement should match a if statement\n");
        return ERR_PARSE_ELSE_NO_IF;
    }
    node->as.ifStmt.elseBlock = malloc(sizeof(ASTNode));
    if (!node->as.ifStmt.elseBlock) {
        PRINT_ERR("else then block malloc error\n");
        return ERR_PARSE_ELSE_THEN;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("else syntax error\n");
        return ERR_PARSE_ELSE_THEN;
    }
    if (!stackPop(&parser->nodes, node->as.ifStmt.elseBlock)) {
        PRINT_ERR("failed to retrieve else then block\n");
        return ERR_PARSE_ELSE_THEN;
    }

    return 1;
}

static int consumeWhile(WParser *parser) {
    ASTNode node;
    node.type = WPNODE_WHILE;
    node.as.whileStmt.condition = malloc(sizeof(ASTNode));
    if (!node.as.whileStmt.condition) {
        PRINT_ERR("while cond block malloc failed\n");
        return ERR_PARSE_WHILE_COND;
    }
    node.as.whileStmt.block = malloc(sizeof(ASTNode));
    if (!node.as.whileStmt.block) {
        PRINT_ERR("while then block malloc failed\n");
        return ERR_PARSE_WHILE_THEN;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("while syntax error\n");
        return ERR_PARSE_WHILE_COND;
    }
    if (!stackPop(&parser->nodes, node.as.whileStmt.condition)) {
        PRINT_ERR("failed to retrieve while cond block\n");
        return ERR_PARSE_WHILE_COND;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("while syntax error\n");
        return ERR_PARSE_WHILE_THEN;
    }
    if (!stackPop(&parser->nodes, node.as.whileStmt.block)) {
        PRINT_ERR("failed to retrieve while then block\n");
        return ERR_PARSE_WHILE_THEN;
    }

    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push while statement to stack\n");
        return ERR_GENERIC;
    }

    return 1;
}

static int consumeContinue(WParser *parser) {
    ASTNode node;
    node.type = WPNODE_CONTINUE;
    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push continue node to stack\n");
        return ERR_GENERIC;
    }

    return 1;
}

static int consumeBreak(WParser *parser) {
    ASTNode node;
    node.type = WPNODE_BREAK;
    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push break node to stack\n");
        return ERR_GENERIC;
    }

    return 1;
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
