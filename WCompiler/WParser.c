#include <stdlib.h>
#include <string.h>
#include <werror.h>
#include <queue.h>
#include "WParser.h"
#include "WLexer.h"

static Wstatus consumeStatement(WParser *parser, ASTNode *out, bool consumeComma);
static Wstatus consumeBlock(WParser *parser, ASTNode *out);
static void ASTDestroy(ASTNode *ast, bool freeRoot);

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

    if (!arrayInit(&ret.program, sizeof(ASTNode))) {
        lexerDestroy(&ret.lexer);
        return 0;
    }

    *parser = ret;
    return 1;
}

void parserDestroy(WParser *parser) {
    lexerDestroy(&parser->lexer);
    for (size_t i = 0; i < parser->program.count; i++) {
        ASTNode *tmp = arrayGet(&parser->program, i);
        if (!tmp) continue;
        ASTDestroy(tmp, false);
    }
    arrayDestroy(&parser->program);
}

/**
 * @brief Destroy an AST
 * @param ast The AST to destroy
 * @param freeRoot If the root should be freed or not
 */
static void ASTDestroy(ASTNode *ast, bool freeRoot) {
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
            for (size_t i = 0; i < ast->as.block.stmts.count; i++) {
                ASTNode *node = arrayGet(&ast->as.block.stmts, i);
                if (!node) continue;
                ASTDestroy(node, false);
            }
            arrayDestroy(&ast->as.block.stmts);
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
            free(ast->as.classDef.name);
            ASTDestroy(ast->as.classDef.fields, true);
            ASTDestroy(ast->as.classDef.methods, true);
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
            ASTDestroy(ast->as.call.callee, true);
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
static bool ASTNodeInit(ASTNodeType type, ASTNode *out) {
    ASTNode ret = {0};
    ret.type = type;
    switch (type) {
        case (ASTNODE_BLOCK): {
            if (!arrayInit(&ret.as.block.stmts, sizeof(ASTNode))) {
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
            };
            if (!(ret.as.binaryOp.left && ret.as.binaryOp.right)) {
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
                .callee = calloc(1, sizeof(ASTNode)),
                .args = calloc(1, sizeof(ASTNode)),
            };
            if (!(ret.as.call.callee && ret.as.call.args)) {
                ASTDestroy(&ret, false);
                return 0;
            }
            break;
        }
    }

    *out = ret;
    return 1;
}

static bool tokIsStatementEnd(const WToken *token) {
    return token->type == WTOK_PUNCTUATOR
            && (token->as.punc == WTOKPUNC_COMMA
                || token->as.punc == WTOKPUNC_CALLCLOSE
                || token->as.punc == WTOKPUNC_CLOSE);
}

static Wstatus consumePostfixCall(WParser *parser, ASTNode *node) {
    const WToken *tok;
    if (!(tok = lexerPeekToken(&parser->lexer))) {
        return ERR_GENERIC;
    }

    if (tok->type != WTOK_PUNCTUATOR || tok->as.punc != WTOKPUNC_CALLOPEN) {
        return ERR_GENERIC;
    }

    ASTNode ret = {0};
    if (!ASTNodeInit(ASTNODE_CALL, &ret)) {
        return ERR_INTERNAL;
    }
    if (IS_ERR(consumeBlock(parser, ret.as.call.args))) {
        ASTDestroy(&ret, false);
        PRINT_ERR("failed to parse call args\n");
        return ERR_PARSE_CALL_BLOCK;
    }

    *node = ret;
    return 1;
}

/**
 * @brief Consume an identifier
 * @param parser The parser which should consume the identifier
 * @param node Where to store the output `ASTNode`
 * @return 1 if success, non-positive else
 */
static Wstatus consumeIdentifier(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming id\n");
    const WToken *tok;
    Wstatus status = 1;
    ASTNode ret = {0};
    if (!(tok = lexerPeekToken(&parser->lexer))) {
        return ERR_GENERIC;
    }
    if (tok->type != WTOK_IDENTIFIER) return ERR_GENERIC;

    ret.type = ASTNODE_IDENTIFIER;
    ret.as.id = malloc(parser->lexer.tokLen + 1);
    if (!ret.as.id) {
        PRINT_ERR("failed to alloc buffer for id\n");
        return ERR_INTERNAL;
    }
    strcpy(ret.as.id, tok->as.id);

    if (!lexerForward(&parser->lexer)) {
        THROW_ERR(ERR_INTERNAL, ret, "lexer error\n");
    }

    ASTNode call = {0};
    Wstatus postfix = consumePostfixCall(parser, &call);
    switch (postfix) {
        case (1): {
            memcpy(call.as.call.callee, &ret, sizeof(ASTNode));
            *node = call;
            break;
        }
        case (0): {
            *node = ret;
            break;
        }
        default: {
            status = ERR_GENERIC;
            goto ret;
        }
    }
ret:
    if (IS_ERR(status)) ASTDestroy(&ret, false);
    return status;
}

static Wstatus consumeLiteral(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming lit\n");
    const WToken *tok;
    ASTNode ret = {0};
    if (!(tok = lexerPeekToken(&parser->lexer))) {
        return ERR_GENERIC;
    }
    if (tok->type != WTOK_LITERAL) return ERR_GENERIC;

    ret.type = ASTNODE_LITERAL;
    if (tok->as.lit.type == LIT_STR) {
        ret.as.lit.type = LIT_STR;
        ret.as.lit.s = malloc(parser->lexer.tokLen - 1); // removing the quotes
        if (!ret.as.lit.s) {
            PRINT_ERR("failed to alloc buffer for string\n");
            return ERR_INTERNAL;
        }
        strncpy(ret.as.lit.s, tok->as.lit.s + 1, parser->lexer.tokLen - 2); // removing the quotes
        ret.as.lit.s[parser->lexer.tokLen - 2] = '\0';
    } else ret.as.lit = tok->as.lit;

    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        free(ret.as.lit.s);
        return ERR_INTERNAL;
    }

    *node = ret;

    return 1;
}

static Wstatus consumeValue(WParser *parser, ASTNode *node) {
    if (!IS_ERR(consumeIdentifier(parser, node))) {
        return 1;
    }
    if (!IS_ERR(consumeLiteral(parser, node))) {
        return 1;
    }
    if (!IS_ERR(consumeBlock(parser, node))) {
        return 1;
    }
    return ERR_GENERIC;
}

static inline Wstatus consumeElse(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming else\n");
    node->as.ifStmt.elseBlock = calloc(1, sizeof(ASTNode));
    if (!node->as.ifStmt.elseBlock) {
        PRINT_ERR("else then block alloc error\n");
        return ERR_PARSE_ELSE_THEN;
    }
    if (IS_ERR(consumeBlock(parser, node->as.ifStmt.elseBlock))) {
        PRINT_ERR("else parser error\n");
        return ERR_PARSE_ELSE_THEN;
    }
    return 1;
}

static inline Wstatus consumeIf(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming if\n");
    ASTNode ret = {0};
    if (!ASTNodeInit(ASTNODE_IF, &ret)) {
        PRINT_ERR("if node initialization failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    if (IS_ERR(consumeBlock(parser, ret.as.ifStmt.condition))) {
        THROW_ERR(ERR_PARSE_IF_COND, ret, "if condition parser error\n");
    }

    if (IS_ERR(consumeBlock(parser, ret.as.ifStmt.thenBlock))) {
        THROW_ERR(ERR_PARSE_IF_THEN, ret, "if then parser error\n");
    }

    const WToken *tok;
    if (tok = lexerPeekToken(&parser->lexer)) {
        if (tok->type == WTOK_KEYWORD && tok->as.kw == WTOKKW_ELSE) {
            if (!lexerForward(&parser->lexer)) {
                THROW_ERR(ERR_INTERNAL, ret, "lexer error\n");
            }
            if (IS_ERR(status = consumeElse(parser, &ret))) {
                goto ret;
            }
        }
    }

    *node = ret;
ret:
    if (IS_ERR(status)) ASTDestroy(&ret, false);
    return status;
}

static inline Wstatus consumeWhile(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming while\n");
    ASTNode ret = {0};
    if (!ASTNodeInit(ASTNODE_WHILE, &ret)) {
        PRINT_ERR("while node initialization failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    if (IS_ERR(consumeBlock(parser,ret.as.whileStmt.condition))) {
        THROW_ERR(ERR_PARSE_WHILE_COND, ret, "while condition parser error\n");
    }

    if (IS_ERR(consumeBlock(parser,ret.as.whileStmt.body))) {
        THROW_ERR(ERR_PARSE_WHILE_THEN, ret, "while then parser error\n");
    }

    *node = ret;
ret:
    if (IS_ERR(status)) ASTDestroy(&ret, false);
    return status;
}

static inline Wstatus consumeContinue(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming continue\n");
    node->type = ASTNODE_CONTINUE;
    return 1;
}

static inline Wstatus consumeBreak(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming break\n");
    node->type = ASTNODE_BREAK;
    return 1;
}

static inline Wstatus consumeVar(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming var\n");
    ASTNode ret = {0};
    if (!ASTNodeInit(ASTNODE_VAR_DECL, &ret)) {
        PRINT_ERR("var node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    if (IS_ERR(consumeIdentifier(parser, ret.as.varDecl.typeName))) {
        THROW_ERR(ERR_PARSE_VAR_TYPE, ret, "var type parser error\n");
    }

    if (IS_ERR(consumeIdentifier(parser, ret.as.varDecl.varName))) {
        THROW_ERR(ERR_PARSE_VAR_ID, ret, "var id parser error\n");
    }

    *node = ret;
ret:
    if (IS_ERR(status)) ASTDestroy(&ret, false);
    return status;
}

static Wstatus consumeFuncName(WParser *parser, ASTNode *node) {
    const WToken *tok;
    if (!(tok = lexerPeekToken(&parser->lexer))) {
        return ERR_GENERIC;
    }

    if (tok->type == WTOK_IDENTIFIER) {
        return consumeIdentifier(parser, node);
    }

    if (tok->type == WTOK_OPERATOR) {
        ASTNode ret = {0};
        ret.type = ASTNODE_IDENTIFIER;

        const char *opName = operators[tok->as.op];
        size_t len = strlen(opName);

        ret.as.id = malloc(len + 1);
        if (!ret.as.id) {
            PRINT_ERR("failed to alloc buffer for operator function name\n");
            return ERR_INTERNAL;
        }
        memcpy(ret.as.id, opName, len + 1);

        if (!lexerForward(&parser->lexer)) {
            free(ret.as.id);
            PRINT_ERR("lexer error\n");
            return ERR_INTERNAL;
        }

        *node = ret;
        return 1;
    }

    return ERR_GENERIC;
}

static inline Wstatus consumeFunc(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming func\n");
    ASTNode ret = {0};
    if (!ASTNodeInit(ASTNODE_FUNC_DEF, &ret)) {
        PRINT_ERR("func node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    if (IS_ERR(consumeIdentifier(parser, ret.as.funcDef.retType))) {
        THROW_ERR(ERR_PARSE_FUNC_TYPE, ret, "func type parser error\n");
    }
    if (IS_ERR(consumeFuncName(parser, ret.as.funcDef.name))) {
        THROW_ERR(ERR_PARSE_FUNC_ID, ret, "func id parser error\n");
    }

    if (IS_ERR(consumeBlock(parser, ret.as.funcDef.params))) {
        THROW_ERR(ERR_PARSE_FUNC_ARGS, ret, "func args parser error\n");
    }

    if (IS_ERR(consumeBlock(parser, ret.as.funcDef.body))) {
        THROW_ERR(ERR_PARSE_FUNC_BODY, ret, "func body parser error\n");
    }

    *node = ret;
ret:
    if (IS_ERR(status)) ASTDestroy(&ret, false);
    return status;
}

static inline Wstatus consumeClass(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming class\n");
    ASTNode ret = {0};
    if (!ASTNodeInit(ASTNODE_CLASS_DEF, &ret)) {
        PRINT_ERR("class node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    if (IS_ERR(consumeIdentifier(parser, ret.as.classDef.name))) {
        THROW_ERR(ERR_PARSE_CLASS_ID, ret, "class id parser error\n");
    }
    if (IS_ERR(consumeBlock(parser, ret.as.classDef.fields))) {
        THROW_ERR(ERR_PARSE_CLASS_FIELDS, ret, "class fields parser error\n");
    }
    if (IS_ERR(consumeBlock(parser, ret.as.classDef.methods))) {
        THROW_ERR(ERR_PARSE_CLASS_METHODS, ret, "class methods parser error\n");
    }

    *node = ret;
ret:
    if (IS_ERR(status)) ASTDestroy(&ret, false);
    return status;
}

static inline Wstatus consumeReturn(WParser *parser, ASTNode *node) {
    PRINT_DEBUG("consuming return\n");
    ASTNode ret = {0};
    if (!ASTNodeInit(ASTNODE_RETURN, &ret)) {
        PRINT_ERR("return node init failed\n");
        return ERR_INTERNAL;
    }

    Wstatus status = 1;
    const WToken *tok;
    if (!(tok = lexerPeekToken(&parser->lexer))) {
        THROW_ERR(ERR_GENERIC, ret, "failed to get token after return\n");
    }
    if (!tokIsStatementEnd(tok) && (ret.as.retStmt.value = calloc(1, sizeof(ASTNode)))
        && IS_ERR(consumeStatement(parser, ret.as.retStmt.value, false))) {
        THROW_ERR(ERR_PARSE_RETURN_VALUE, ret, "return value parser error\n");
    }

    *node = ret;
ret:
    if (IS_ERR(status)) ASTDestroy(&ret, false);
    return status;
}

/**
 * @brief Consume a keyword
 * @param parser The parser which should consume the keyword
 * @param out Where to store the output `ASTNode`
 * @return 1 if success, non-positive else
 */
static Wstatus consumeKeyword(WParser *parser, ASTNode *node) {
    const WToken *token;
    if (!(token = lexerPeekToken(&parser->lexer))) return 0;
    if (token->type != WTOK_KEYWORD) return 0;

    WTokKw kw = token->as.kw;
    if (!lexerForward(&parser->lexer)) {
        PRINT_ERR("lexer error\n");
        return ERR_INTERNAL;
    }
    switch (kw) {
        case (WTOKKW_IF): return consumeIf(parser, node);
        // case (WTOKKW_ELSE): return 0; // Handled by if
        case (WTOKKW_WHILE): return consumeWhile(parser, node);
        case (WTOKKW_CONTINUE): return consumeContinue(parser, node);
        case (WTOKKW_BREAK): return consumeBreak(parser, node);
        case (WTOKKW_VAR): return consumeVar(parser, node);
        case (WTOKKW_FUNC): return consumeFunc(parser, node);
        case (WTOKKW_CLASS): return consumeClass(parser, node);
        case (WTOKKW_RETURN): return consumeReturn(parser, node);
        default: return ERR_GENERIC;
    }
    return 1;
}

static bool opIsUnary(WTokOp op) {
    return (
        op == WTOKOP_POS
        || op == WTOKOP_NEG
        || op == WTOKOP_NOT
        || op == WTOKOP_BNOT
    );
}

// Check if stack op has higher precedence
static inline bool higherPrecedence(WTokOp stackOp, WTokOp curOp) {
    if (opIsUnary(curOp)) {
        return stackOp < curOp;
    }
    return stackOp <= curOp;
}

static Wstatus reduceStackOp(Stack *operators, Stack *operands) {
    ASTNode opNode;
    if (!stackPop(operators, &opNode)) {
        return ERR_INTERNAL;
    }

    if (opNode.type == ASTNODE_BINARY_OP) {
        if (!stackPop(operands, opNode.as.binaryOp.right)) {
            return ERR_GENERIC;
        }
        if (!stackPop(operands, opNode.as.binaryOp.left)) {
            return ERR_GENERIC;
        }
    } else if (opNode.type == ASTNODE_UNARY_OP) {
        if (!stackPop(operands, opNode.as.unaryOp.operand)) {
            return ERR_GENERIC;
        }
    } else {
        return ERR_GENERIC;
    }

    if (!stackPush(operands, &opNode)) {
        ASTDestroy(&opNode, false);
        return ERR_INTERNAL;
    }

    return 1;
}

static Wstatus shuntingYard(Queue *tokQ, ASTNode *out) {
    Wstatus status = 1;

    Stack operands = {0};
    if (!stackInit(&operands, sizeof(ASTNode))) {
        THROW_ERR(ERR_INTERNAL, ret, "failed to init operands stack\n");
    }
    Stack operators = {0};
    if (!stackInit(&operators, sizeof(ASTNode))) {
        THROW_ERR(ERR_INTERNAL, ret, "failed to init operators stack\n");
    }

    #define ASTNODE_IS_OP(node) ((node).type == ASTNODE_BINARY_OP || (node).type == ASTNODE_UNARY_OP)
    ASTNode node;
    while (queuePop(tokQ, &node)) {
        if (!ASTNODE_IS_OP(node)) {
            if (!stackPush(&operands, &node)) {
                THROW_ERR(ERR_INTERNAL, ret, "failed to push node to operands stack\n");
            }
            continue;
        }

        const ASTNode *top;
        while ((top = stackTop(&operators)) && ASTNODE_IS_OP(*top) && higherPrecedence(top->as.op.op, node.as.op.op)) {
            if (IS_ERR(status = reduceStackOp(&operators, &operands))) {
                THROW_ERR(status, ret, "failed to reduce operator\n");
            }
        }

        if (!stackPush(&operators, &node)) {
            THROW_ERR(ERR_INTERNAL, ret, "failed to push node to operators stack\n");
        }
    }

    while (stackTop(&operators)) {
        if (IS_ERR(status = reduceStackOp(&operators, &operands))) {
            THROW_ERR(status, ret, "failed to reduce leftover operator\n");
        }
    }
    ASTNode root = {0};
    if (!stackPop(&operands, &root)) {
        PRINT_DEBUG("empty statement");
        // THROW_ERR(ERR_GENERIC, ret, "empty statement\n");
    }

    // {
    //     PRINT_DEBUG("printing partial tree\n");
    //     ASTPrint(&root);
    // }

    {
        ASTNode tmp = {0};
        stackPop(&operands, &tmp);
        PRINT_DEBUG("%i\n", tmp.type);
    }

    if (stackTop(&operands)) {
        ASTDestroy(&root, false);
        THROW_ERR(ERR_GENERIC, ret, "invalid statement: extra operands\n");
    }

    *out = root;

ret:
    if (IS_ERR(status)) {
        ASTNode node;
        while (stackPop(&operands, &node)) {
            ASTDestroy(&node, false);
        }
        while (stackPop(&operators, &node)) {
            ASTDestroy(&node, false);
        }
        ASTDestroy(out, false);
    }
    stackDestroy(&operands);
    stackDestroy(&operators);
    return status;
}

static Wstatus consumeStatement(WParser *parser, ASTNode *out, bool consumeComma) {
    PRINT_DEBUG("starting consumeStatement\n");
    Wstatus status = 1;
    Queue tokQ;
    queueInit(&tokQ, sizeof(ASTNode));

    while (lexerHasToken(&parser->lexer)) {
        const WToken *token;
        if (!(token = lexerPeekToken(&parser->lexer))) {
            return ERR_GENERIC;
        }
        if (tokIsStatementEnd(token)) {
            if (consumeComma && token->as.punc == WTOKPUNC_COMMA) {
                if (!lexerForward(&parser->lexer)) {
                    THROW_ERR(ERR_INTERNAL, ret, "lexer failed to forward\n");
                }
            }
            if (IS_ERR(status = shuntingYard(&tokQ, out))) {
                THROW_ERR(status, ret, "expression reduction failed\n");
            }
            goto ret;
        }

        ASTNode *buf = queueBump(&tokQ);
        if (!buf) {
            THROW_ERR(ERR_INTERNAL, ret, "failed to bump stack\n");
        }
        switch (token->type) {
            case (WTOK_KEYWORD): {
                if (IS_ERR(consumeKeyword(parser, buf))) {
                    THROW_ERR(ERR_GENERIC, ret, "failed to parse keyword\n");
                }
                break;
            }
            case (WTOK_PUNCTUATOR): {
                if (IS_ERR(consumeBlock(parser, buf))) {
                    THROW_ERR(ERR_GENERIC, ret, "unexpected punc token\n");
                }
                break;
            }
            case (WTOK_IDENTIFIER): {
                if (IS_ERR(consumeIdentifier(parser, buf))) {
                    THROW_ERR(ERR_GENERIC, ret, "failed to parse identifier\n");
                }
                break;
            }
            case (WTOK_LITERAL): {
                if (IS_ERR(consumeLiteral(parser, buf))) {
                    THROW_ERR(ERR_GENERIC, ret, "failed to parse literal\n");
                }
                break;
            }
            case (WTOK_OPERATOR): {
                if (opIsUnary(token->as.op)) {
                    if (!ASTNodeInit(ASTNODE_UNARY_OP, buf)) {
                        THROW_ERR(ERR_INTERNAL, ret, "failed to init node\n");
                    }
                    buf->as.unaryOp.unary = token->as.op;
                } else {
                    if (!ASTNodeInit(ASTNODE_BINARY_OP, buf)) {
                        THROW_ERR(ERR_INTERNAL, ret, "failed to init node\n");
                    }
                    buf->as.binaryOp.op = token->as.op;
                }
                if (!lexerForward(&parser->lexer)) {
                    THROW_ERR(ERR_INTERNAL, ret, "lexer failed to forward\n");
                }
                break;
            }
            default: {
                THROW_ERR(ERR_INTERNAL, ret, "unidentified token\n");
                break;
            }
        }
    }
    PRINT_ERR("unexpected end of file\n");
    status = ERR_GENERIC;

ret:
    queueDestroy(&tokQ);
    return status;
}

static WTokPunc matchedCloser(WTokPunc open) {
    switch (open) {
        case WTOKPUNC_OPEN: return WTOKPUNC_CLOSE;
        case WTOKPUNC_CALLOPEN: return WTOKPUNC_CALLCLOSE;
        default: return (WTokPunc)-1;
    }
}

static Wstatus consumeBlock(WParser *parser, ASTNode *out) {
    PRINT_DEBUG("consuming block\n");
    const WToken *tok;
    if (!(tok = lexerPeekToken(&parser->lexer))) {
        return ERR_GENERIC;
    }
    Wstatus status = 1;
    if (tok->type != WTOK_PUNCTUATOR) return ERR_GENERIC;
    WTokPunc closing = matchedCloser(tok->as.punc);
    if (closing == (WTokPunc)-1) {
        THROW_ERR(ERR_INTERNAL, ret, "unknown bracket\n");
    }

    if (!lexerForward(&parser->lexer)) {
        THROW_ERR(ERR_INTERNAL, ret, "lexer failed to forward\n");
    }

    if (!ASTNodeInit(ASTNODE_BLOCK, out)) {
        THROW_ERR(ERR_INTERNAL, ret, "failed to init block\n");
    }
    while (lexerHasToken(&parser->lexer)) {
        if (!(tok = lexerPeekToken(&parser->lexer))) {
            THROW_ERR(ERR_GENERIC, ret, "lexer error, no token\n");
        }
        if (tok->type == WTOK_PUNCTUATOR && tok->as.punc == closing) {
            if (!lexerForward(&parser->lexer)) {
                THROW_ERR(ERR_INTERNAL, ret, "lexer failed to forward\n");
            }
            return 1;
        }
        ASTNode node;
        if (IS_ERR(consumeStatement(parser, &node, true))) {
            THROW_ERR(ERR_GENERIC, ret, "block parser error\n");
        }
        if (!arrayAdd(&out->as.block.stmts, &node)) {
            THROW_ERR(ERR_INTERNAL, ret, "failed to add statement in block\n");
        }
    }
    THROW_ERR(ERR_GENERIC, ret, "block never closed\n");

ret:
    // if (IS_ERR(status)) ASTDestroy(out, false); // DO NOT FREE, NOT OWNER
    return status;
}

Wstatus parserBuildAST(WParser *parser) {
    Wstatus status = 1;
    ASTNode node;
    while (lexerHasToken(&parser->lexer)) {
        if (IS_ERR(status = consumeStatement(parser, &node, true))) {
            THROW_ERR(status, ret, "failed to build AST for program\n");
        }

        if (node.type != ASTNODE_FUNC_DEF && node.type != ASTNODE_CLASS_DEF) {
            THROW_ERR(status, ret, "failed to build AST for program: unexpected statement\n");
        }

        // const WToken *tok;
        // if (!(tok = lexerPeekToken(&parser->lexer))) {
        //     break;
        // }

        // if (tok->type != WTOK_KEYWORD) {
        //     THROW_ERR(status, ret, "failed to build AST for program: unexpected statement\n");
        // }

        // if (tok->as.kw != WTOKKW_CLASS && tok->as.kw != WTOKKW_FUNC) {
        //     THROW_ERR(status, ret, "failed to build AST for program: unexpected statement\n");
        // }

        // if (tok->as.kw == WTOKKW_CLASS) {
        //     if (IS_ERR(consumeClass(parser, &node))) {
        //         THROW_ERR(status, ret, "failed to build AST for program\n");
        //     }
        // } else if (tok->as.kw == WTOKKW_FUNC) {
        //     if (IS_ERR(consumeFunc(parser, &node))) {
        //         THROW_ERR(status, ret, "failed to build AST for program\n");
        //     }
        // } else THROW_ERR(status, ret, "failed to build AST for program: unexpected statement\n");

        arrayAdd(&parser->program, &node);
    }

ret:
    if (IS_ERR(status)) {
        for (size_t i = 0; i < parser->program.count; i++) {
            ASTNode *tmp = arrayGet(&parser->program, i);
            if (!tmp) continue;
            ASTDestroy(tmp, false);
        }
    }
    return status;
}


static void _ASTPrint(ASTNode *ast, int indent) {
    if (!ast) return;
    for (int i = 0; i < indent; i++) {
        printf("| ");
    }
    printf("+-");
    switch (ast->type) {
        case (ASTNODE_LITERAL): {
            switch (ast->as.lit.type) {
                case (LIT_STR): {
                    printf("LIT {STR, %s}\n", ast->as.lit.s);
                    break;
                }
                case (LIT_INT): {
                    printf("LIT {INT, %zd}\n", ast->as.lit.i);
                    break;
                }
                case (LIT_FLOAT): {
                    printf("LIT {FLT, %lf}\n", ast->as.lit.f);
                    break;
                }
            }
            break;
        }
        case (ASTNODE_IDENTIFIER): {
            printf("ID {%s}\n", ast->as.id);
            break;
        }
        case (ASTNODE_BLOCK): {
            printf("BLOCK {}\n");
            for (size_t i = 0; i < ast->as.block.stmts.count; i++) {
                ASTNode *node = arrayGet(&ast->as.block.stmts, i);
                _ASTPrint(node, indent + 1);
            }
            break;
        }
        case (ASTNODE_UNARY_OP): {
            printf("UNARY {%s}\n", operators[ast->as.unaryOp.unary]);
            _ASTPrint(ast->as.unaryOp.operand, indent + 1);
            break;
        }
        case (ASTNODE_BINARY_OP): {
            printf("BINARY {%s}\n", operators[ast->as.binaryOp.op]);
            _ASTPrint(ast->as.binaryOp.left, indent + 1);
            _ASTPrint(ast->as.binaryOp.right, indent + 1);
            break;
        }
        case (ASTNODE_CLASS_DEF): {
            printf("CLASS {%s}\n", ast->as.classDef.name->as.id);
            _ASTPrint(ast->as.classDef.fields, indent + 1);
            _ASTPrint(ast->as.classDef.methods, indent + 1);
            break;
        }
        case (ASTNODE_FUNC_DEF): {
            printf("FUNC {%s, %s}\n", ast->as.funcDef.retType->as.id, ast->as.funcDef.name->as.id);
            _ASTPrint(ast->as.funcDef.params, indent + 1);
            _ASTPrint(ast->as.funcDef.body, indent + 1);
            break;
        }
        case (ASTNODE_VAR_DECL): {
            printf("VAR {%s, %s}\n", ast->as.varDecl.typeName->as.id, ast->as.varDecl.varName->as.id);
            break;
        }
        case (ASTNODE_IF): {
            printf("IF {}\n");
            _ASTPrint(ast->as.ifStmt.condition, indent + 1);
            _ASTPrint(ast->as.ifStmt.thenBlock, indent + 1);
            _ASTPrint(ast->as.ifStmt.elseBlock, indent + 1);
            break;
        }
        case (ASTNODE_WHILE): {
            printf("WHILE {}\n");
            _ASTPrint(ast->as.whileStmt.condition, indent + 1);
            _ASTPrint(ast->as.whileStmt.body, indent + 1);
            break;
        }
        case (ASTNODE_CALL): {
            printf("CALL {}\n");
            _ASTPrint(ast->as.call.callee, indent + 1);
            _ASTPrint(ast->as.call.args, indent + 1);
            break;
        }
        case (ASTNODE_RETURN): {
            printf("RETURN {}\n");
            _ASTPrint(ast->as.retStmt.value, indent + 1);
            break;
        }
        case (ASTNODE_BREAK): {
            printf("BREAK {}\n");
            break;
        }
        case (ASTNODE_CONTINUE): {
            printf("CONTINUE {}\n");
            break;
        }
        default: printf("unknown ???\n");
    }
}

void ASTPrint(ASTNode *ast) {
    _ASTPrint(ast, 0);
}

void parserPrintProgram(WParser *parser) {
    for (size_t i = 0; i < parser->program.count; i++) {
        ASTNode *node = arrayGet(&parser->program, i);
        ASTPrint(node);
    }
}
