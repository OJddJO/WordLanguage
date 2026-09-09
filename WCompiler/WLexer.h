#ifndef __WLEXER_H__
#define __WLEXER_H__

#include <stdio.h>
#include <stdint.h>

typedef enum _WTokType {
    WTOK_KEYWORD,
    WTOK_OPERATOR,
    WTOK_PUNCTUATOR,
    WTOK_IDENTIFIER,
    WTOK_LITERAL,
} WTokType;

typedef enum _WTokOp {
    WTOKOP_MEMBER_ACCESS,
    WTOKOP_NOT, WTOKOP_BNOT,
    WTOKOP_POS, WTOKOP_NEG,
    WTOKOP_TIME, WTOKOP_DIV, WTOKOP_MOD,
    WTOKOP_PLUS, WTOKOP_MINUS,
    WTOKOP_LSH, WTOKOP_RSH,
    WTOKOP_LT, WTOKOP_LE, WTOKOP_GT, WTOKOP_GE,
    WTOKOP_EQ, WTOKOP_NEQ,
    WTOKOP_BAND, WTOKOP_BXOR, WTOKOP_BOR,
    WTOKOP_AND, WTOKOP_OR,
    WTOKOP_IS,

    OP_NB
} WTokOp;

typedef enum _WTokKw {
    WTOKKW_IF, WTOKKW_ELSE,
    WTOKKW_WHILE, WTOKKW_CONTINUE, WTOKKW_BREAK,
    WTOKKW_CLASS, WTOKKW_NAMESPACE,
    WTOKKW_RETURN,
    WTOKKW_VOID, WTOKKW_INT, WTOKKW_FLOAT, WTOKKW_BOOL, WTOKKW_STRING,
    WTOKKW_TRUE, WTOKKW_FALSE,

    KW_NB
} WTokKw;

typedef enum _WLitType {
    NOT_LIT = 0,
    LIT_INT = 1,
    LIT_FLOAT,
    LIT_STR,
} WLitType;

typedef enum _WTokPunc {
    WTOKPUNC_COMMA,
    WTOKPUNC_OPEN,
    WTOKPUNC_CLOSE,
} WTokPunc;

typedef struct _WToken {
    WTokType    type;
    union {
        const char  *id;
        WTokOp      op;
        WTokKw      kw;
        WTokPunc    punc;
        struct {
            WLitType type;
            union {
                const char  *s;
                int64_t     i;
                double      f;
            };
        } lit;
    } as;
} WToken;

typedef struct _WLexer {
    FILE    *src;
    long    cur;
} WLexer;

int lexerInit(const char *filepath, WLexer *lexer);
void lexerDestroy(WLexer *lexer);
WToken *lexerNext(WLexer *lexer);

#endif
