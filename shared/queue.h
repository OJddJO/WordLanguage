#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h>

typedef struct _QNode {
    struct _QNode   *next;
    void            *value;
} QNode;

typedef struct _Queue {
    QNode   *head;
    QNode   *tail;
    size_t  eltSize;
} Queue;

void queueInit(Queue *queue, size_t eltSize);
void *queueBump(Queue *queue);
bool queueAdd(Queue *queue, void *elt);
bool queuePop(Queue *queue, void *out);
void queueDestroy(Queue *queue);

#endif
