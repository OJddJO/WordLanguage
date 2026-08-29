#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "siphash24.h"

#define DEFAULT_SIZE (1 << 6)
#define CAPACITY_THRESHOLD 0.75
#define GROWTH_FACTOR 1.5

#define RAND64() (rand() << 32 | rand())

inline static void bucketAddStruct(HashmapBucket *bucket, HashmapItem *item) {
    if (*bucket) (*bucket)->prev = &item->next;
    *bucket = item;

    return 1;
}

inline static int bucketAddKeyValue(HashmapBucket *bucket, const char *key, void *value) {
    HashmapItem *new = malloc(sizeof(HashmapItem));
    if (!new) return 0;
    *new = (HashmapItem){
        .prev = bucket,
        .next = *bucket,
        .key = key,
        .value = value,
    };

    bucketAddStruct(bucket, new);

    return 1;
}

// Pop the first element
inline static HashmapItem *bucketPop(HashmapBucket *bucket) {
    if (!*bucket) return NULL;
    HashmapItem *ret = *bucket;
    *bucket = ret->next;
    if (ret->next) ret->next->prev = ret->prev;
    return ret;
}

inline static HashmapItem *bucketRemove(HashmapBucket *bucket, const char *key) {
    while (*bucket) {
        if (strcmp((*bucket)->key, key) == 0) {
            HashmapItem *ret = *bucket;
            *bucket = ret->next;
            if (ret->next) ret->next->prev = ret->prev;
            return ret;
        }
    }
    return NULL;
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

inline static int hashmapGrow(Hashmap *map) {
    uint64_t targetSize = map->bucketCount * GROWTH_FACTOR;
    HashmapBucket *new = calloc(map->buckets, sizeof(HashmapBucket) * targetSize);

    if (!new) return 0;

    for (uint64_t i = 0; i < map->bucketCount; i++) {
        HashmapItem *item;
        while (item = bucketPop(&map->buckets[i])) {
            uint64_t hash = siphash24(item->key, strlen(item->key), map->key);
            bucketAddStruct(&new[hash % targetSize], item);
        }
    }

    free(map->buckets);
    map->buckets = new;
    map->bucketCount = targetSize;

    return 1;
}

int hashmapAdd(Hashmap *map, const char *key, void *element) {
    if (map->itemCount >= map->bucketCount * CAPACITY_THRESHOLD) {
        if (!hashmapGrow(map)) {
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

    if (bucketAddKeyValue(&map->buckets[idx], key, element)) return 1;
    return 0;
}
