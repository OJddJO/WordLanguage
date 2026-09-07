#ifndef __WLEXER_H__
#define __WLEXER_H__

#include <stdio.h>

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
} WTokOp;

typedef enum _WTokKw {
    WTOKKW_IF, WTOKKW_ELSE,
    WTOKKW_WHILE, WTOKKW_CONTINUE, WTOKKW_BREAK,
    WTOKKW_CLASS,
    WTOKKW_RETURN,
} WTokKw;

typedef struct _WToken {
    WTokType    type;
    union {
        char    *tok;
        WTokOp  op;
        WTokKw  kw;
    } as;
} WToken;

typedef struct _WLexer {
    const FILE  *file;
    fpos_t      cur;
} WLexer;

#endif
