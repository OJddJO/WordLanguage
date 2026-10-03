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
 * @warning Doesn't free allocated memory if failed, use `ASTDestroy`
 */
inline static int ASTNodeInit(ASTNodeType type, ASTNode *out) {
    ASTNode ret = {0};
    ret.type = type;
    switch (type) {
        case (ASTNODE_BLOCK): {
            ret.as.block = (struct _ASTBlock){
                .stmt = calloc(1, sizeof(ASTNode))
            };
            if (!ret.as.block.stmt) return 0;
            break;
        }
        case (ASTNODE_UNARY_OP): {
            ret.as.unaryOp = (struct _ASTUnary){
                .operand = calloc(1, sizeof(ASTNode)),
            };
            if (!ret.as.unaryOp.operand) return 0;
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
            if (!ret.as.call.args) return 0;
            break;
        }
    }

    *out = ret;
    return 1;
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
static int consumeFunc(WParser *parser);
static int consumeClass(WParser *parser);
static int consumeReturn(WParser *parser);
static int consumeTrue(WParser *parser);
static int consumeFalse(WParser *parser);
static int consumeBlock(WParser *parser, WToken *token);

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
        case (WTOKKW_FUNC): return consumeFunc(parser);
        case (WTOKKW_CLASS): return consumeClass(parser);
        case (WTOKKW_TRUE): return consumeTrue(parser);
        case (WTOKKW_FALSE): return consumeFalse(parser);
        default: return 0;
    }
}

static int consumeIf(WParser *parser) {
    ASTNode node;
    if (!ASTNodeInit(ASTNODE_IF, &node)) {
        PRINT_ERR("if node initialization failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;

    WToken tok;
    if (lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok))) {
        PRINT_ERR("if condition syntax error\n");
        status = ERR_PARSE_IF_COND;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.ifStmt.condition)) {
        PRINT_ERR("failed to retrieve if cond block\n");
        status = ERR_PARSE_IF_COND;
        goto ret;
    }

    if (lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok))) {
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
    WToken tok;
    if (lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok))) {
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
    ASTNode node = {0};
    if (!ASTNodeInit(ASTNODE_WHILE, &node)) {
        PRINT_ERR("while node initialization failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;

    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok))) {
        PRINT_ERR("while syntax error\n");
        status = ERR_PARSE_WHILE_COND;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.whileStmt.condition)) {
        PRINT_ERR("failed to retrieve while cond block\n");
        status = ERR_PARSE_WHILE_COND;
        goto ret;
    }

    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeBlock(parser, &tok))) {
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

// Helper function
inline static int _buildIdentifier(WParser *parser, ASTNode *out, int32_t errCode, const char *err, const char *purpose) {
    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || IS_ERR(consumeIdentifier(parser, &tok))) {
        PRINT_ERR("%s", errCode);
        return errCode;
    }
    if (!stackPop(&parser->nodes, out)) {
        PRINT_ERR("failed to retrieve %s\n", purpose);
        return ERR_INTERNAL;
    }

    return 1;
}

static int consumeVar(WParser *parser) {
    ASTNode node;
    if (!ASTNodeInit(ASTNODE_VAR_DECL, &node)) {
        PRINT_ERR("var node init failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;
    status = _buildIdentifier(parser, &node.as.varDecl.typeName, ERR_PARSE_VAR_TYPE, "var syntax error\n", "var type");
    if (IS_ERR(status)) {
        goto ret;
    }
    status = _buildIdentifier(parser, &node.as.varDecl.varName, ERR_PARSE_DEF_ID, "var syntax error\n", "var name");
    if (IS_ERR(status)) {
        goto ret;
    }

    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push var node to stack\n");
        status = ERR_INTERNAL;
        goto ret;
    }

ret:
    if (IS_ERR(status)) ASTDestroy(&node, false);
    return status;
}

static int consumeFunc(WParser *parser) {
    ASTNode node;
    if (!ASTNodeInit(ASTNODE_FUNC_DEF, &node)) {
        PRINT_ERR("func node init failed\n");
        return ERR_INTERNAL;
    }

    int32_t status = 1;

    status = _buildIdentifier(parser, node.as.funcDef.retType, ERR_PARSE_DEF_TYPE, "func syntax error\n", "func type");
    if (IS_ERR(status)) {
        goto ret;
    }

    status = _buildIdentifier(parser, node.as.funcDef.name, ERR_PARSE_DEF_ID, "func syntax error\n", "func name");
    if (IS_ERR(status)) {
        goto ret;
    }

    WToken tok;
    if (!lexerNext(&parser->lexer, &tok) || !consumeBlock(parser, &tok)) {
        PRINT_ERR("func syntax error\n");
        status = ERR_PARSE_DEF_ARGS;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.funcDef.params)) {
        PRINT_ERR("failed to retrieve func parameters\n");
        status = ERR_INTERNAL;
        goto ret;
    }

    if (!lexerNext(&parser->lexer, &tok) || !consumeBlock(parser, &tok)) {
        PRINT_ERR("func syntax error\n");
        status = ERR_PARSE_DEF_BODY;
        goto ret;
    }
    if (!stackPop(&parser->nodes, node.as.funcDef.body)) {
        PRINT_ERR("failed to retrieve func body\n");
        status = ERR_PARSE_DEF_BODY;
        goto ret;
    }

    if (!stackPush(&parser->nodes, &node)) {
        PRINT_ERR("failed to push func node to stack\n");
        status = ERR_INTERNAL;
        goto ret;
    }

ret:
    if (IS_ERR(status)) ASTDestroy(&node, false);
    return status;
}

// Parser tries to consume a block, if can't find a block NULL is returned
static int consumeBlock(WParser *parser, WToken *token) {
}

int parserConsume(WParser *parser) {
}

int parserBuildAST(WParser *parser) {

}
