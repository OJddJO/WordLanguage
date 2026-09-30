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
                ASTDestroy(ast->as.block.stmt[i], true);
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
                ASTDestroy(ast->as.classDef.fields[i], true);
            }
            for (size_t i = 0; i < ast->as.classDef.methodsCount; i++) {
                ASTDestroy(ast->as.classDef.methods[i], true);
            }
            break;
        }
        case (ASTNODE_MEMBER_ACCESS): {
            ASTDestroy(ast->as.member.memName, true);
            ASTDestroy(ast->as.member.object, true);
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

static int parserConsume(WParser *parser);
static int consumeIdentifier(WParser *parser, WToken *tok);

/**
 * @brief Consume an identifier
 * @param parser The parser which should consume the identifier
 * @param tok The token corresponding to the supposed identifier
 * @return 1 if success, non-positive else
 */
static int consumeIdentifier(WParser *parser, WToken *tok) {
    ASTNode node = {0};
    if (tok->type != WTOK_IDENTIFIER) return 0;

    node.type = ASTNODE_IDENTIFIER;
    node.as.id = tok->as.id;
    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push identifier to stack\n");
        return ERR_INTERNAL;
    }

    return 1;
}

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
static int consumeBlock(WParser *parser);

/**
 * @brief Consume a keyword
 * @param parser The parser which should consume the keyword
 * @param token The token corresponding to the supposed keyword
 * @return 1 if success, non-positive else
 */
static int consumeKeyword(WParser *parser, WToken *token) {
    if (token->type != WTOK_KEYWORD) return 0;
    switch (token->as.kw) {
        case (WTOKKW_IF): return consumeIf(parser);
        case (WTOKKW_ELSE): return consumeElse(parser);
        case (WTOKKW_WHILE): return consumeWhile(parser);
        case (WTOKKW_CONTINUE): return consumeContinue(parser);
        case (WTOKKW_BREAK): return consumeBreak(parser);
        case (WTOKKW_VAR): return consumeVar(parser);
        case (WTOKKW_DEF): return consumeDef(parser);
        case (WTOKKW_CLASS): return consumeClass(parser);
        case (WTOKKW_TRUE): return consumeTrue(parser);
        case (WTOKKW_FALSE): return consumeFalse(parser);
        default: return 0;
    }
}

static int consumeIf(WParser *parser) {
    int32_t status = 1;

    ASTNode node = {0};
    node.type = ASTNODE_IF;

    node.as.ifStmt.condition = calloc(1, sizeof(ASTNode));
    if (!node.as.ifStmt.condition) {
        PRINT_ERR("if cond block alloc error\n");
        status = ERR_INTERNAL;
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

    node.as.ifStmt.thenBlock = calloc(1, sizeof(ASTNode));
    if (!node.as.ifStmt.thenBlock) {
        PRINT_ERR("if then block alloc error\n");
        status = ERR_INTERNAL;
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
    if (IS_ERR(status)) ASTDestroy(&node, false);
    return status;
}

static int consumeElse(WParser *parser) {
    ASTNode *node = stackTop(&parser->nodes);
    if (!node || node->type != ASTNODE_IF) {
        PRINT_ERR("else statement should match a if statement\n");
        return ERR_PARSE_ELSE_NO_IF;
    }
    node->as.ifStmt.elseBlock = calloc(1, sizeof(ASTNode));
    if (!node->as.ifStmt.elseBlock) {
        PRINT_ERR("else then block alloc error\n");
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
    int32_t status = 1;

    ASTNode node = {0};
    node.type = ASTNODE_WHILE;

    node.as.whileStmt.condition = calloc(1, sizeof(ASTNode));
    if (!node.as.whileStmt.condition) {
        PRINT_ERR("while cond block alloc failed\n");
        status = ERR_INTERNAL;
        goto ret;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("while syntax error\n");
        status = ERR_PARSE_WHILE_COND;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.whileStmt.condition)) {
        PRINT_ERR("failed to retrieve while cond block\n");
        status = ERR_PARSE_WHILE_COND;
        goto ret;
    }

    node.as.whileStmt.body = calloc(1, sizeof(ASTNode));
    if (!node.as.whileStmt.body) {
        PRINT_ERR("while body block alloc failed\n");
        status = ERR_INTERNAL;
        goto ret;
    }

    if (IS_ERR(consumeBlock(parser))) {
        PRINT_ERR("while syntax error\n");
        status = ERR_PARSE_WHILE_THEN;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.whileStmt.body)) {
        PRINT_ERR("failed to retrieve while body block\n");
        status = ERR_PARSE_WHILE_THEN;
        goto ret;
    }

    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push while statement to stack\n");
        status = ERR_INTERNAL;
        goto ret;
    }

ret:
    if (IS_ERR(status)) ASTDestroy(&node, false);
    return status;
}

static int consumeContinue(WParser *parser) {
    ASTNode node = {0};
    node.type = ASTNODE_CONTINUE;
    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push continue node to stack\n");
        return ERR_INTERNAL;
    }

    return 1;
}

static int consumeBreak(WParser *parser) {
    ASTNode node = {0};
    node.type = ASTNODE_BREAK;
    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push break node to stack\n");
        return ERR_INTERNAL;
    }

    return 1;
}

static consumeVar(WParser *parser) {
    int32_t status = 1;

    ASTNode node = {0};
    node.type = ASTNODE_VAR_DECL;
    WToken token;
    if (lexerNext(&parser->lexer, &token) || !consumeIdentifier(parser, &token)) {
        PRINT_ERR("var syntax error\n");
        status = ERR_PARSE_VAR_TYPE;
        goto ret;
    }
    ASTNode *typeName = calloc(sizeof(ASTNode), 1);
    if (!typeName) {
        PRINT_ERR("var typename alloc failed\n");
        status = ERR_INTERNAL;
        goto ret;
    }
    if (!stackPop(&parser->nodes, typeName)) {
        PRINT_ERR("failed to retrieve var type\n");
        status = ERR_INTERNAL;
        goto ret;
    }
    if (typeName->type != ASTNODE_IDENTIFIER) {
        PRINT_ERR("expected identifier\n");
        status = ERR_PARSE_VAR_TYPE;
        goto ret;
    }

    ASTNode *varName = calloc(sizeof(ASTNode), 1);
    if (!varName) {
        PRINT_ERR("var varname alloc failed\n");
        status = ERR_INTERNAL;
        goto ret;
    }

ret:
    if (IS_ERR(status)) ASTDestroy(&node, false);
    return status;
}

// Parser tries to consume a block, if can't find a block NULL is returned
static int consumeBlock(WParser *parser) {

}

int parserConsume(WParser *parser) {
}

int parserBuildAST(WParser *parser) {

}
