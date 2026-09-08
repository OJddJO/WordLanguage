#ifndef __WERROR_H__
#define __WERROR_H__

#include <stdio.h>

#define PRINT_ERR(format, ...)  fprintf(stderr, "[ ERROR ][%s:%u] " format, __func__, __LINE__, ##__VA_ARGS__)
#define PRINT_WARN(format, ...) fprintf(stderr, "[WARNING][%s:%u] " format, __func__, __LINE__, ##__VA_ARGS__)

#endif
