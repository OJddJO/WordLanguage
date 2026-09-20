#ifndef __WERROR_H__
#define __WERROR_H__

#include <stdio.h>

#define PRINT_ERR(format, ...)  fprintf(stderr, "[ ERROR ][%s:%u] " format, __func__, __LINE__, ##__VA_ARGS__)
#define PRINT_WARN(format, ...) fprintf(stderr, "[WARNING][%s:%u] " format, __func__, __LINE__, ##__VA_ARGS__)

#define IS_ERR(X) (X <= 0)

#define ERR_GENERIC             0

#define ERR_PARSE_IF_COND       -1
#define ERR_PARSE_IF_THEN       -2

#define ERR_PARSE_ELSE_NO_IF    -1
#define ERR_PARSE_ELSE_THEN     -2

#define ERR_PARSE_WHILE_COND    -1
#define ERR_PARSE_WHILE_THEN    -2

#endif
