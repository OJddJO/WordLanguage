#ifndef __WORD_H__
#define __WORD_H__

#include <stdlib.h>
#include <stdio.h>
#include "w_alloc.h"
#include "keywords.h"
#include "w_word.h"
#include "lexer.h"
#include "parser.h"
#include "scope.h"
#include "w_stdlib.h"

#ifndef DEBUG
#define DEBUG true 
#endif

extern dict *keywords;

#endif