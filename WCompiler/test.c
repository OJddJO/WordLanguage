#include "WLexer.h"

#include <stdio.h>
#include <werror.h>

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
    "true", "false",
};

static const char *punctuator = ",()[]";

int main(int argc, char *argv[]) {
    printf("argc: %d\n", argc);
    if (argc != 2) return 1;
    const char *file = argv[1];

    WLexer lex;
    if (!lexerInit(file, &lex)) {
        PRINT_ERR("failed to init lexer\n");
        return 0;
    }
    printf("%s", lex.buf);

    WToken tok;
    while (lexerNext(&lex, &tok)) {
        switch (tok.type) {
            case (WTOK_IDENTIFIER): {
                printf("{ID, %s} | ", tok.as.id);
                break;
            }
            case (WTOK_KEYWORD): {
                printf("{KEY, %s} | ", keywords[tok.as.kw]);
                break;
            }
            case (WTOK_PUNCTUATOR): {
                printf("{PUNC, '%c'} | ", punctuator[tok.as.punc]);
                break;
            }
            case (WTOK_OPERATOR): {
                printf("{OP, %s} | ", operators[tok.as.op]);
                break;
            }
            case (WTOK_LITERAL): {
                switch (tok.as.lit.type) {
                    case (LIT_FLOAT): {
                        printf("{LIT, %lf} | ", tok.as.lit.f);
                        break;
                    }
                    case (LIT_INT): {
                        printf("{LIT, %zd} | ", tok.as.lit.i);
                        break;
                    }
                    case (LIT_STR): {
                        printf("{LIT, \"%s\"} | ", tok.as.lit.s);
                        break;
                    }
                    default:
                        return 1;
                        break;
                }
                break;
            }
        }
        // lexerForward(&lex);
    }

    return 0;
}
