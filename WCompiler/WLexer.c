#include <stdio.h>

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
    "class",
    "return",
};

static FILE *_file = NULL;

int lexer_init(const char *filepath, WLexer *lexer) {
    FILE *file = fopen(filepath, "r");
    if (!file) return 0;

    *lexer = (WLexer){
        .file = file,
        .cur = 0,
    };
    return 1;
}

inline WToken *lexer_next(WLexer *lexer) {
}
