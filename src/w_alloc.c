#include "w_alloc.h"

#if MONITOR_MEMORY

int w_alloc_count = 0;
long long int w_alloc_used = 0;
long long int w_alloc_total = 0;

/**
 * \brief Allocates memory for the given size.
 * \param size The size of the memory to allocate.
 * \return The allocated memory.
 */
void *w_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "[MEMORY] Error: Could not allocate memory\n");
        exit(1);
    }
    printf("[MEMORY] Allocated %zu bytes at %p\n", size, ptr);
    w_alloc_count++;
    w_alloc_used += size;
    w_alloc_total += size;
    return ptr;
}

/**
 * \brief Frees the given memory.
 * \param ptr The memory to free.
 */
void w_free(void *ptr) {
    printf("[MEMORY] Freeing memory at %p... ", ptr);
    //test if ptr is not already freed
    free(ptr);
    printf("Freed\n");
    w_alloc_count--;
    w_alloc_used -= sizeof(ptr);
}

/**
 * \brief Prints the memory usage.
 */
void w_alloc_print() {
    printf("[MEMORY] Memory usage: %d allocations, %lld bytes used, %lld bytes total\n", w_alloc_count, w_alloc_used, w_alloc_total);
}

#endif