# Src

The source code of the project.

## Structure

- `src/`: The source code of the project.
- `src/interpreter.c`: The interpreter of the language. Main file.

## Uhmm... The code is a mess

The code is unreadable and not well structured. It will be changed soon, as the structure of the interpreter will be changed. All keywords/functions call structures are currently hard coded in the `interpreter.c` file. Keywords/functions call will be modularized and moved to the `w_stdlib.c` file. And so, the interpreter will only contain the main loop, and the program will be more efficient, readable, maintainable and extensible. It will be also easier to implement new features and add a module system (extension of the language coded in `c`, not Python-like modules).