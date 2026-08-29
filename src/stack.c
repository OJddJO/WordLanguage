#include <stdlib.h>
#include <stddef.h>

#include "stack.h"

#define DEFAULT_SIZE  (1 << 5)
#define GROWTH_FACTOR 1.5

int stackInit(Stack *stack) {
    Stack ret = {
        .size = DEFAULT_SIZE,
        .count = 0,
        .buf = malloc(sizeof(void *) * DEFAULT_SIZE),
    };
    if (!ret.buf) return 0;

    *stack = ret;
    return 1;
}

int stackPush(Stack *stack, void *element) {
    if (stack->size == stack->count) {
        size_t targetSize = stack->size * GROWTH_FACTOR;
        void **new = realloc(stack->buf, targetSize);
        if (!new) return 0;

        stack->buf = new;
        stack->size = targetSize;
    }

    stack->buf[stack->count++] = element;
    return 1;
}

void *stackPop(Stack *stack) {
    if (stack->count == 0) return NULL;

    void *ret = stack->buf[--stack->count];
    if (stack->size > DEFAULT_SIZE && stack->count == stack->size / GROWTH_FACTOR / GROWTH_FACTOR) {
        size_t targetSize = stack->size / GROWTH_FACTOR;
        void **new = realloc(stack->buf, targetSize);
        if (new) {
            stack->buf = new;
            stack->size = targetSize;
        }
    }
    return ret;
}
