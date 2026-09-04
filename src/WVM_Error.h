#ifndef __WVM_ERROR_H__
#define __WVM_ERROR_H__

#include <stdio.h>

#define PRINT_ERR(format, ...)  fprintf(stderr, "[ ERROR ][%s:%u] " format, __func__, __LINE__, ##__VA_ARGS__)

#endif
