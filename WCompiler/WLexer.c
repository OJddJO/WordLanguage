#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <werror.h>
#include "WLexer.h"

// operators precedence is defined by the order of each operators in this array
static const char *operators[] = {
    ".", // member access
    "+", "-", // unary sign
    "not", "bitnot", // logical/bitwise not
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
    "var", "func", "class",
    "return",
};

static const char *punctuator = ",()[]";

bool lexerInit(const char *filepath, WLexer *lexer) {
    FILE *file = fopen(filepath, "rb");
    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    lexer->size = ftell(file);
    if (lexer->size <= 0) return false;
    fseek(file, 0, SEEK_SET);

    lexer->buf = malloc(lexer->size + 1);
    if (!lexer->buf) {
        fclose(file);
        return 0;
    }

    if (fread(lexer->buf, 1, lexer->size, file) != lexer->size) {
        return false;
    }
    lexer->buf[lexer->size] = '\0';
    fclose(file);

    lexer->cur = 0;
    lexer->line = 0;
    lexer->tokLen = 0;
    lexer->tokStr = malloc(100);
    lexer->tokStrSize = 100;
    lexer->token = (WToken){0};
    return 1;
}

void lexerDestroy(WLexer *lexer) {
    if (lexer->buf) free(lexer->buf);
    if (lexer->tokStr) free(lexer->tokStr);
    lexer->buf = NULL;
    lexer->cur = 0;
    lexer->line = 0;
    lexer->tokLen = 0;
    lexer->tokStr = NULL;
    lexer->tokStrSize = 0;
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
    return strchr(punctuator, c);
}

static bool isSymbolOperator(char c) {
    return c == '.' || c == '+' || c == '-';
}

static size_t getNumberLen(const char *src) {
    size_t len = 0;

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
        size_t expStart = len;
        size_t expLen = 1;

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

static size_t getStringLen(const char *src) {
    char quote = *src;
    size_t len = 1;
    while (src[len] != '\0' && src[len] != quote) {
        if (src[len] == '\\' && src[len + 1] != '\0') len++;
        len++;
    }
    if (src[len] == quote) len++;
    return len;
}

static size_t getTokenLen(WLexer *lexer) {
    const char *src = &lexer->buf[lexer->cur];
    if (*src == '\0') return 0;

    if (*src == '"' || *src == '\'') return getStringLen(src);

    if (isPunctuator(*src) || isSymbolOperator(*src)) return 1;

    if (isdigit(*src)) return getNumberLen(src);

    size_t len = 0;
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

static bool buildToken(WLexer *lexer, WToken *out) {
    lexer->tokLen = getTokenLen(lexer);
    if (!lexer->tokLen) return 0;

    if (lexer->tokLen + 1 > lexer->tokStrSize) {
        char *new = realloc(lexer->tokStr, lexer->tokLen + 1);
        if (!new) return 0;
        lexer->tokStr = new;
    }

    memcpy(lexer->tokStr, &lexer->buf[lexer->cur], lexer->tokLen);
    lexer->tokStr[lexer->tokLen] = '\0';
    // lexer->cur += lexer->tokLen;


    for (int i = 0; i < OP_NB; i++) {
        if (strcmp(lexer->tokStr, operators[i]) == 0) {
            out->type = WTOK_OPERATOR;
            out->as.op = i;
            return 1;
        }
    }

    for (int i = 0; i < KW_NB; i++) {
        if (strcmp(lexer->tokStr, keywords[i]) == 0) {
            out->type = WTOK_KEYWORD;
            out->as.kw = i;
            return 1;
        }
    }

    if (isPunctuator(*lexer->tokStr)) {
        out->type = WTOK_PUNCTUATOR;
        switch (*lexer->tokStr) {
            case '(': {
                out->as.punc = WTOKPUNC_OPEN;
                break;
            }
            case ')': {
                out->as.punc = WTOKPUNC_CLOSE;
                break;
            }
            case '[': {
                out->as.punc = WTOKPUNC_CALLOPEN;
                break;
            }
            case ']': {
                out->as.punc = WTOKPUNC_CALLCLOSE;
                break;
            }
            case ',': {
                out->as.punc = WTOKPUNC_COMMA;
                break;
            }
        }
        return 1;
    }

    WLitType litType = isLiteral(lexer->tokStr, &out->as.lit.i, &out->as.lit.f);
    if (litType) {
        out->type = WTOK_LITERAL;
        out->as.lit.type = litType;
        if (litType == LIT_STR) out->as.lit.s = lexer->tokStr;
        return 1;
    }

    out->type = WTOK_IDENTIFIER;
    out->as.id = lexer->tokStr;
    return 1;
}

//readonly
bool lexerPeekToken(WLexer *lexer, WToken **out) {
    if (!lexer->tokLen) { // if the current cached token is wrong, rebuild
        if (!buildToken(lexer, &lexer->token))
            return 0;
    }
    *out = &lexer->token;
    return 1;
}

bool lexerForward(WLexer *lexer) {
    if (!lexer->tokLen) return 0;
    if (lexer->cur + lexer->tokLen >= lexer->size) return 0;
    lexer->cur += lexer->tokLen;
    lexer->tokLen = 0;
    return 1;
}

bool lexerHasToken(WLexer *lexer) {
    consumeWhitespaces(lexer);
    return lexer->cur != lexer->size;
}

// DO NOT USE ON RESULT OF `lexerPeekToken`
void tokenDestroy(WToken *token) {
    if (token->type == WTOK_IDENTIFIER)
        free((char *)token->as.id);

    if (token->type == WTOK_LITERAL && token->as.lit.type == LIT_STR)
        free((char *)token->as.lit.s);
}
