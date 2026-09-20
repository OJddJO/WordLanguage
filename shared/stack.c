#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "stack.h"

#define DEFAULT_SIZE  (1 << 5)
#define GROWTH_FACTOR 1.5

int stackInit(Stack *stack, size_t elemSize) {
    Stack ret = {
        .size = DEFAULT_SIZE,
        .count = 0,
        .elemSize = elemSize,
        .buf = malloc(elemSize * DEFAULT_SIZE),
    };
    if (!ret.buf) return 0;

    *stack = ret;
    return 1;
}

int stackPush(Stack *stack, void *value) {
    if (stack->size == stack->count) {
        size_t targetSize = stack->size * GROWTH_FACTOR;
        void *new = realloc(stack->buf, stack->elemSize * targetSize);
        if (!new) return 0;

        stack->buf = new;
        stack->size = targetSize;
    }

    memcpy(stack->buf + stack->count++ * stack->elemSize, value, stack->elemSize);
    return 1;
}

int stackPop(Stack *stack, void *ret) {
    if (stack->count == 0) return 0;

    memcpy(ret, stack->buf + --stack->count * stack->elemSize, stack->elemSize);
    if (stack->size > DEFAULT_SIZE && stack->count == stack->size / GROWTH_FACTOR / GROWTH_FACTOR) {
        size_t targetSize = stack->size / GROWTH_FACTOR;
        uint8_t *new = realloc(stack->buf, stack->elemSize * targetSize);
        if (new) {
            stack->buf = new;
            stack->size = targetSize;
        }
    }
    return 1;
}

void *stackTop(Stack *stack) {
    if (stack->count == 0) return NULL;
    return stack->buf + (stack->elemSize * stack->count);
}

void stackDestroy(Stack *stack) {
    free(stack->buf);
}
