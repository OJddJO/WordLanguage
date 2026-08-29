#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

#include "hashmap.h"
#include "siphash24.h"

#define DEFAULT_SIZE (1 << 6)
#define CAPACITY_THRESHOLD 0.75
#define GROWTH_FACTOR 1.5

#define RAND64() (rand() << 32 | rand())

static int bucketAdd(HashmapBucket *bucket, const char *key, void *value) {
    HashmapItem *new = malloc(sizeof(HashmapItem));
    if (!new) return 0;
    *new = (HashmapItem){
        .prev = bucket,
        .next = *bucket,
        .key = key,
        .value = value,
    };

    if (*bucket) (*bucket)->prev = &new->next;
    *bucket = new;

    return 1;
}

// Pop the first element
static HashmapItem *bucketPop(HashmapBucket *bucket) {
    if (!*bucket) return NULL;
    HashmapItem *ret = *bucket;
    *bucket = ret->next;
    if (ret->next) ret->next->prev = ret->prev;
    return ret;
}

// Only for growing buffers, reducing buffer is UB
static void *recalloc(void *ptr, size_t prevSize, size_t newSize) {
    void *new = realloc(ptr, newSize);
    if (!new) return NULL;
    memset(((uint8_t *)ptr) + prevSize, 0, newSize - prevSize);
    return new;
}

int hashmapInit(Hashmap *map) {
    Hashmap ret = {
        .key = {RAND64(), RAND64()},
        .bucketCount = DEFAULT_SIZE,
        .itemCount = 0,
        .buckets = calloc(DEFAULT_SIZE, sizeof(HashmapBucket)),
    };

    if (!ret.buckets) return 0;

    *map = ret;
    return 1;
}

int hashmapGrow(Hashmap *map) {
    HashmapBucket *new = recalloc(
                            map->buckets, sizeof(HashmapBucket) * map->bucketCount,
                            sizeof(HashmapBucket) * map->bucketCount * GROWTH_FACTOR);
    if (!new) return 0;
    map->buckets = new;


}

int hashmapAdd(Hashmap *map, const char *key, void *element) {
    if (map->itemCount >= map->bucketCount * CAPACITY_THRESHOLD) {
        if (hashmapGrow(map)) {
            return 0;
        }
    }

    HashmapItem *mapItem = malloc(sizeof(HashmapItem));
    if (!mapItem) return 0;
    *mapItem = (HashmapItem){
        .key = key,
        .value = element,
    };

    uint64_t hash = siphash24(key, strlen(key), map->key);
    uint64_t idx = hash % map->bucketCount;


    return 1;
}
