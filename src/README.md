# Src

The source code of the project.

## Structure

- `src/`: The source code of the project.
- `src/interpreter.c`: The interpreter of the language. Main file.

## WordLang Interpreter

The interpreter of the language is coded in `c`. It is a simple interpreter that reads the source code from a file, tokenizes it, parses it, and executes it.

### How it works

The **interpreter** reads the **source code** from a file and gives it to the **lexer**. The lexer **tokenizes** the source code and gives the tokens to the **parser**. Until now, it seems to be a normal language interpreter. \
**BUT** the parser **does not parse the tokens to an AST**. It creates a list of list of list of tokens *(... yes I know)*. The big list represents the **source code**, the lists inside represents the **lines**, and the lists inside the lines represents **blocks to evaluate** (if the list contains multiple tokens, it is an operation, if it contains only one token, it is a single token to evaluate). The interpreter then evaluates the list of list of list of tokens and execute the code.

#### Examples:

WordLang source code:
```
print "Hello, World!"
print "Second line"
```
Result:
```
Lexer output:
[
    [{"print", KEYWORD, 1}, {"Hello, World!", STR, 1}],
    [{"print", KEYWORD, 2}, {"Second line", STR, 2}]
]

Parser output:
[ <--- Source code
    [ <--- Line 1
        [ <--- Block 1
            {"print", KEYWORD, 1}
        ],
        [ <--- Block 2
            {"Hello, World!", STR, 1}
        ]
    ],
    [ <--- Line 2
        [ <--- Block 1
            {"print", KEYWORD, 2}
        ],
        [ <--- Block 2
            {"Second line", STR, 2}
        ]
    ]
]
```

WordLang source code:
```
int var assign 5 plus 3
```
Result:
```
Lexer output:
[
    [{"int", KEYWORD, 1}, {"var", VARIABLE, 1}, {"assign", KEYWORD, 1}, {"5", NUMBER, 1}, {"plus", OPERATOR, 1}, {"3", NUMBER, 1}]
]

Parser output:
[ <--- Source code
    [ <--- Line 1
        [ <--- Block 1
            {"int", KEYWORD, 1}
        ],
        [ <--- Block 2
            {"var", VARIABLE, 1}
        ],
        [ <--- Block 3
            {"assign", KEYWORD, 1}
        ],
        [ <--- Block 4, size of 3 = operation
            {"5", NUMBER, 1},
            {"plus", OPERATOR, 1},
            {"3", NUMBER, 1}
        ]
    ]
]
```

## Uhmm... The code is a mess

The code is actually unreadable. It will be changed soon, as the structure of the interpreter will be changed. All keywords/functions call structures are currently hard coded in the `interpreter.c` file. Keywords/functions call will be modularized and moved to the `w_stdlib.c` file. And so, the interpreter will only contain the main loop, and the program will be more efficient, readable, maintainable and extensible. It will be also easier to implement new features and add a module system (extension of the language coded in `c`, not Python-like modules).