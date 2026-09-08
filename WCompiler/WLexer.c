#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <werror.h>
#include "WLexer.h"

// operators precedence is defined by the order of each operators in this array
static const char *operators[] = {
    ".", // member access
    "not", "bitnot", // logical/bitwise not
    "+", "-", // unary sign
    "time", "div", "mod", // multiplicative operators
    "plus", "minus", // additive operators
    "lsh", "rsh", // bitshift
    "lt", "le", "gt", "ge", // relational operators
    "eq", "neq", // equality operators
    "bitand", "bitxor", "bitor", // bitwise operators (excluding not)
    "and", "or", // logical operators (excluding not)
    "is", // assignment
};

static const char *keywords[] = {
    "if", "else",
    "while", "continue", "break",
    "class", "namespace",
    "return",
    "Void", "Int", "Float", "Bool", "String",
    "true", "false",
};

static const char *whitespaces = " \n\t\r";
static const char *punctuator = ",()";

int lexerInit(const char *filepath, WLexer *lexer) {
    FILE *file = fopen(filepath, "r");
    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buf = (char *)malloc(size + 1);
    if (!buf) {
        fclose(file);
        return 0;
    }

    if (fread(buf, 1, size, file) != (size_t)size) {
        PRINT_WARN("File content size differ from probed size\n");
    }
    buf[size] = '\0';
    fclose(file);

    lexer->src = buf;
    size_t cur = 0;

    return 1;
}

void lexerDestroy(WLexer *lexer) {
    free(lexer->src);
}

static size_t consumeWhitespaces(WLexer *lexer) {
    size_t n = lexer->cur;
    while (lexer->src[n] != '\0' && strchr(whitespaces, lexer->src[n]) && ++n);
    return n;
}

static int buildToken(const char *token, WToken *out) {
    for (WTokOp i = 0; i < OP_NB; i++) {
        if (strcmp(token, operators[i]) == 0) {
            out->type = WTOK_OPERATOR;
            out->as.op = i;
            return 1;
        }
    }

    for (WTokKw i = 0; i < KW_NB; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            out->type = WTOK_OPERATOR;
            out->as.kw = i;
            return 1;
        }
    }

    if (strchr(punctuator, *token)) {
        out->type = WTOK_PUNCTUATOR;
        char *tok = malloc(2);
        if (!tok) return 0;

        strncpy(out->as.tok, token, 1);
        out->as.tok = tok;
        return 1;
    }

}

inline WToken *lexerNext(WLexer *lexer) {
    WToken *ret = malloc(sizeof(WToken));
    if (!ret) {
        PRINT_ERR("Failed to allocate memory for token\n");
        return NULL;
    }

    size_t n = consumeWhitespaces(lexer);
    while (strchr(whitespaces, lexer->src[n++]));


}
