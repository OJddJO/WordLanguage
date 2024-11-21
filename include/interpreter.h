#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_alloc.h"
#include "keywords.h"
#include "w_word.h"
#include "lexer.h"
#include "parser.h"
#include "scope.h"
#include "w_stdlib.h"

extern dict *keywords;
extern dict *reserved_words;

#endif