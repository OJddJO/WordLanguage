#include <stdlib.h>
#include <string.h>
#include <queue.h>

void queueInit(Queue *queue, size_t eltSize) {
    queue->head = NULL;
    queue->tail = NULL;
    queue->eltSize = eltSize;
}

void *queueBump(Queue *queue) {
    QNode *new = malloc(sizeof(QNode));
    void *buf = malloc(queue->eltSize);
    if (!new || !buf) {
        free(new);
        free(buf);
        return NULL;
    }
    *new = (QNode) {
        .next = NULL,
        .value = buf
    };
    if (!queue->head) queue->head = new;
    else queue->tail->next = new;
    queue->tail = new;
    return buf;
}

// COPY ELT
bool queueAdd(Queue *queue, void *elt) {
    void *buf = queueBump(queue);
    if (!buf) return false;
    memcpy(buf, elt, queue->eltSize);
    return true;
}

bool queuePop(Queue *queue, void *out) {
    if (!queue->head) return false;
    QNode *head = queue->head;
    memcpy(out, head->value, queue->eltSize);

    queue->head = head->next;
    if (!queue->head) {
        queue->tail = NULL;
    }

    free(head->value);
    free(head);
    return true;
}

void queueDestroy(Queue *queue) {
    while (queue->head) {
        QNode *head = queue->head;
        queue->head = head->next;
        free(head->value);
        free(head);
    }
    queue->tail = NULL;
}
