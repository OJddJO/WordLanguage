#ifndef __WLEXER_H__
#define __WLEXER_H__

#include <stdio.h>
#include <stdint.h>

typedef enum _WTokType : uint8_t {
    WTOK_KEYWORD,
    WTOK_OPERATOR,
    WTOK_PUNCTUATOR,
    WTOK_IDENTIFIER,
    WTOK_LITERAL,
} WTokType;

extern const char *operators[];
typedef enum _WTokOp : uint8_t {
    WTOKOP_MEMBER_ACCESS,
    WTOKOP_POS, WTOKOP_NEG,
    WTOKOP_NOT, WTOKOP_BNOT,
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

extern const char *keywords[];
typedef enum _WTokKw : uint8_t {
    WTOKKW_IF, WTOKKW_ELSE,
    WTOKKW_WHILE, WTOKKW_CONTINUE, WTOKKW_BREAK,
    WTOKKW_VAR, WTOKKW_FUNC, WTOKKW_CLASS,
    WTOKKW_RETURN,

    KW_NB
} WTokKw;

typedef enum _WLitType : uint8_t {
    NOT_LIT = 0,
    LIT_INT = 1,
    LIT_FLOAT,
    LIT_STR,
} WLitType;

extern const char *punctuator;
typedef enum _WTokPunc : uint8_t {
    WTOKPUNC_COMMA,
    WTOKPUNC_OPEN,
    WTOKPUNC_CLOSE,
    WTOKPUNC_CALLOPEN,
    WTOKPUNC_CALLCLOSE,
} WTokPunc;

typedef struct _WLit {
    union {
        char    *s;
        int64_t i;
        double  f;
    };
    WLitType    type;
} WLiteral;

typedef struct _WToken {
    union {
        const char  *id;
        WTokOp      op;
        WTokKw      kw;
        WTokPunc    punc;
        WLiteral    lit;
    } as;
    WTokType    type;
} WToken;

typedef struct _WLexer {
    char    *buf;       // Whole code
    size_t  size;       // .buf size

    int64_t cur;        // cursor on the buffer
    size_t  line;       // current line in the code

    size_t  tokLen;     // cached tokLen
    char    *tokStr;    // string of current token
    size_t  tokStrSize; // tokStr buffer size (NOT STRING LENGTH)
    WToken  token;      // current token
} WLexer;

bool lexerInit(const char *filepath, WLexer *lexer);
void lexerDestroy(WLexer *lexer);
const WToken *lexerPeekToken(WLexer *lexer);
bool lexerForward(WLexer *lexer);
bool lexerHasToken(WLexer *lexer);
void tokenDestroy(WToken *token);

#endif
