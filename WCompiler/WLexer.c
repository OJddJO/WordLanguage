#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

static const char *punctuator = ",()";

int lexerInit(const char *filepath, WLexer *lexer) {
    FILE *file = fopen(filepath, "r");
    if (!file) return 0;

    lexer->src = file;
    size_t cur = 0;

    return 1;
}

void lexerDestroy(WLexer *lexer) {
    free(lexer->src);
}

static void consumeWhitespaces(WLexer *lexer) {
    long n = lexer->cur;
    fseek(lexer->src, n, SEEK_SET);
    int c;
    while ((c = fgetc(lexer->src)) != EOF && isspace(c) && ++n);
    lexer->cur = n;
}

static long getTokenLen(WLexer *lexer) {
    fseek(lexer->src, lexer->cur, SEEK_SET);
    long len = 0;
    int c;
    while ((c = fgetc(lexer->src)) != EOF && !isspace(c) && ++len);
    return len;
}

static bool isInteger(const char *str, int64_t *out) {
    char *end;
    *out = strtoll(str, &end, 0);
    return end != str && *end == '\0';
}

static bool isFloat(const char *str, double *out) {
    char *end;
    *out = strtod(str, &end);
    return end != str && *end == '\0';
}

static WLitType isLiteral(const char *str, int64_t *iout, double *fout) {
    if (strchr("\"'", str[0])) return LIT_STR;
    if (isInteger(str, iout)) return LIT_INT;
    if (isFloat(str, fout)) return LIT_FLOAT;
    return NOT_LIT;
}

static int buildToken(WLexer *lexer, WToken *out) {
    long tokLen = getTokenLen(lexer);

    char *token = malloc(tokLen);
    if (!token) {
        PRINT_ERR("Failed to alloc memory for token\n");
        return 0;
    }
    fread(token, 1, (size_t)tokLen, lexer->src);
    lexer->cur += tokLen;

    for (WTokOp i = 0; i < OP_NB; i++) {
        if (strcmp(token, operators[i]) == 0) {
            free(token);
            out->type = WTOK_OPERATOR;
            out->as.op = i;
            return 1;
        }
    }

    for (WTokKw i = 0; i < KW_NB; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            free(token);
            out->type = WTOK_OPERATOR;
            out->as.kw = i;
            return 1;
        }
    }

    if (strchr(punctuator, *token)) {
        out->type = WTOK_PUNCTUATOR;
        switch (*token) {
            case '(': {
                out->as.punc = WTOKPUNC_OPEN;
                break;
            }
            case ')': {
                out->as.punc = WTOKPUNC_CLOSE;
                break;
            }
            case ',': {
                out->as.punc = WTOKPUNC_COMMA;
            }
        }
        return 1;
    }

    WLitType litType = isLiteral(token, &out->as.lit.i, &out->as.lit.f);
    if (litType) {
        if (litType != LIT_STR) free(token);
        out->type = WTOK_LITERAL;
        out->as.lit.type = litType;
        return 1;
    }

    out->type = WTOK_IDENTIFIER;
    out->as.id = token;
    return 1;
}

inline WToken *lexerNext(WLexer *lexer) {
    WToken *ret = malloc(sizeof(WToken));
    if (!ret) {
        PRINT_ERR("Failed to allocate memory for token\n");
        return NULL;
    }

    consumeWhitespaces(lexer);
    if (!buildToken(lexer, ret)) {
        free(ret);
        return NULL;
    }

    return ret;
}
