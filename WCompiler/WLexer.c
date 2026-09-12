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
    FILE *file = fopen(filepath, "rb");
    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    lexer->buf = malloc(size + 1);
    if (!lexer->buf) {
        fclose(file);
        return 0;
    }

    fread(lexer->buf, 1, size, file);
    lexer->buf[size] = '\0';
    lexer->cur = 0;
    lexer->line = 0;
    fclose(file);
    return 1;
}

void lexerDestroy(WLexer *lexer) {
    if (lexer->buf) free(lexer->buf);
    lexer->buf = NULL;
    lexer->cur = 0;
    lexer->line = 0;
}

static void consumeWhitespaces(WLexer *lexer) {
    while (lexer->buf[lexer->cur] != '\0' && isspace(lexer->buf[lexer->cur])) {
        if (lexer->buf[lexer->cur] == '\n') {
            lexer->line++;
        }
        lexer->cur++;
    }
}

static bool isPunctuator(char c) {
    return c == ',' || c == '(' || c == ')';
}

static bool isSymbolOperator(char c) {
    return c == '.' || c == '+' || c == '-';
}

static long getNumberLen(const char *src) {
    long len = 0;

    if (*src == '0' && src[1] != '\0') {
        if (src[1] == 'x' || src[1] == 'o' || src[1] == 'b') {
            len += 2;
        }
    }

    while (isdigit(src[len])) len++;

    if (src[len] == '.' && isdigit(src[len - 1])) {
        len++;
        while (isdigit(src[len])) len++;
    }

    if (src[len] == 'e' || src[len] == 'E') {
        long expStart = len;
        long expLen = 1;

        if (src[expStart + expLen] == '+' || src[expStart + expLen] == '-') {
            expLen++;
        }

        if (isdigit(src[expStart + expLen])) {
            len += expLen;
            while (isdigit(src[len])) len++;
        }
    }
    return len;
}

static long getStringLen(const char *src) {
    char quote = *src;
    long len = 1;
    while (src[len] != '\0' && src[len] != quote) {
        if (src[len] == '\\' && src[len + 1] != '\0') len++;
        len++;
    }
    if (src[len] == quote) len++;
    return len;
}

static long getTokenLen(WLexer *lexer) {
    const char *src = &lexer->buf[lexer->cur];
    if (*src == '\0') return 0;

    if (*src == '"' || *src == '\'') return getStringLen(src);

    if (isPunctuator(*src) || isSymbolOperator(*src)) return 1;

    if (isdigit(*src)) return getNumberLen(src);

    long len = 0;
    while (src[len] != '\0' && !isspace(src[len]) && !isPunctuator(src[len])
            && !isSymbolOperator(src[len])) {
        len++;
    }

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
    if (*str == '"' || *str == '\'') return LIT_STR;
    if (isInteger(str, iout)) return LIT_INT;
    if (isFloat(str, fout)) return LIT_FLOAT;
    return NOT_LIT;
}

static int buildToken(WLexer *lexer, WToken *out) {
    long tokLen = getTokenLen(lexer);
    if (!tokLen) return 0;

    char *token = malloc(tokLen + 1);
    if (!token) return 0;

    memcpy(token, &lexer->buf[lexer->cur], tokLen);
    token[tokLen] = '\0';
    lexer->cur += tokLen;


    for (int i = 0; i < OP_NB; i++) {
        if (strcmp(token, operators[i]) == 0) {
            free(token);
            out->type = WTOK_OPERATOR;
            out->as.op = i;
            return 1;
        }
    }

    for (int i = 0; i < KW_NB; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            free(token);
            out->type = WTOK_KEYWORD;
            out->as.kw = i;
            return 1;
        }
    }

    if (isPunctuator(*token)) {
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
                break;
            }
        }
        free(token);
        return 1;
    }

    WLitType litType = isLiteral(token, &out->as.lit.i, &out->as.lit.f);
    if (litType) {
        if (litType != LIT_STR) free(token);
        else out->as.lit.s = token;
        out->type = WTOK_LITERAL;
        out->as.lit.type = litType;
        return 1;
    }

    out->type = WTOK_IDENTIFIER;
    out->as.id = token;
    return 1;
}

WToken *lexerNext(WLexer *lexer) {
    consumeWhitespaces(lexer);
    if (lexer->buf[lexer->cur] == '\0') return NULL;

    WToken *ret = malloc(sizeof(WToken));
    if (!ret) {
        PRINT_ERR("Failed to allocate memory for token\n");
        return NULL;
    }

    if (!buildToken(lexer, ret)) {
        free(ret);
        return NULL;
    }

    return ret;
}

void tokenFree(WToken *token) {
    if (token->type == WTOK_IDENTIFIER)
        free(token->as.id);

    if (token->type == WTOK_LITERAL && token->as.lit.type == LIT_STR)
        free(token->as.lit.s);

    free(token);
}
