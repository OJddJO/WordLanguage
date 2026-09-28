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

void ASTDestroy(ASTNode *ast, bool freeNode) {
    if (!ast) return;
    switch (ast->type) {
        case (WPNODE_LITERAL): {
            if (ast->as.lit.type == LIT_STR) {
                free(ast->as.lit.s);
            }
            break;
        }
        case (WPNODE_IDENTIFIER): {
            free(ast->as.id);
            break;
        }
        case (WPNODE_BLOCK): {
            for (size_t i = 0; i < ast->as.block.count; i++) {
                ASTDestroy(ast->as.block.stmt[i], true);
            }
            free(ast->as.block.stmt);
            break;
        }
        case (WPNODE_UNARY_OP): {
            ASTDestroy(ast->as.unaryOp.operand, true);
            break;
        }
        case (WPNODE_BINARY_OP): {
            ASTDestroy(ast->as.binaryOp.left, true);
            ASTDestroy(ast->as.binaryOp.right, true);
            break;
        }
        case (WPNODE_CLASS_DEF): {
            for (size_t i = 0; i < ast->as.classDef.fieldsCount; i++) {
                ASTDestroy(ast->as.classDef.fields[i], true);
            }
            for (size_t i = 0; i < ast->as.classDef.methodsCount; i++) {
                ASTDestroy(ast->as.classDef.methods[i], true);
            }
            break;
        }
        case (WPNODE_MEMBER_ACCESS): {
            free(ast->as.member.member);
            ASTDestroy(ast->as.member.object, true);
            break;
        }
        case (WPNODE_FUNC_DEF): {
            free(ast->as.funcDef.retType);
            free(ast->as.funcDef.name);
            ASTDestroy(ast->as.funcDef.params, true);
            ASTDestroy(ast->as.funcDef.body, true);
            break;
        }
        case (WPNODE_VAR_DECL): {
            free(ast->as.varDecl.typeName);
            free(ast->as.varDecl.varName);
            ASTDestroy(ast->as.varDecl.init, true);
            break;
        }
        case (WPNODE_IF): {
            ASTDestroy(ast->as.ifStmt.condition, true);
            ASTDestroy(ast->as.ifStmt.thenBlock, true);
            if (ast->as.ifStmt.elseBlock) ASTDestroy(ast->as.ifStmt.elseBlock, true);
            break;
        }
        case (WPNODE_WHILE): {
            ASTDestroy(ast->as.whileStmt.condition, true);
            ASTDestroy(ast->as.whileStmt.block, true);
            break;
        }
        case (WPNODE_CALL): {
            ASTDestroy(ast->as.call.callee, true);
            for (size_t i = 0; i < ast->as.call.argc; i++) {
                ASTDestroy(ast->as.call.args[i], true);
            }
            break;
        }
        case (WPNODE_RETURN): {
            ASTDestroy(ast->as.retStmt.value, true);
            break;
        }
    }

    if (freeNode) free(ast);
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
    int status = 1;

    ASTNode node = {0};
    node.type = WPNODE_IF;
    node.as.ifStmt.condition = malloc(sizeof(ASTNode));
    if (!node.as.ifStmt.condition) {
        PRINT_ERR("if cond block malloc error\n");
        status = ERR_PARSE_IF_COND;
        goto ret;
    }
    node.as.ifStmt.thenBlock = malloc(sizeof(ASTNode));
    if (!node.as.ifStmt.thenBlock) {
        PRINT_ERR("if then block malloc error\n");
        free(node.as.ifStmt.condition);
        status = ERR_PARSE_IF_THEN;
        goto ret;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("if condition syntax error\n");
        status = ERR_PARSE_IF_COND;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.ifStmt.condition)) {
        PRINT_ERR("failed to retrieve if cond block\n");
        status = ERR_PARSE_IF_COND;
        goto ret;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("if then syntax error\n");
        status = ERR_PARSE_IF_THEN;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.ifStmt.thenBlock)) {
        PRINT_ERR("failed to retrieve if then block\n");
        status = ERR_PARSE_IF_THEN;
        goto ret;
    }

    node.as.ifStmt.elseBlock = NULL;

    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push while statement to stack\n");
        status = ERR_GENERIC;
        goto ret;
    }

ret:
    if (IS_ERR(status)) {
        if (node.as.ifStmt.condition) ;
        if (node.as.ifStmt.elseBlock) ;
        if (node.as.ifStmt.thenBlock) ;
    }
    return status;
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

static consumeVar(WParser *parser) {
    ASTNode node;
    node.type = WPNODE_VAR_DECL;
    WToken *type = lexerNext(&parser->lexer);
    if (!type || type->type != WTOK_IDENTIFIER) {
        PRINT_ERR("var syntax error\n");
        return ERR_PARSE_VAR_NOTYPE;
    }
    node.as.varDecl.typeName = type->as.id;

    WToken *id = lexerNext(&parser->lexer);
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
