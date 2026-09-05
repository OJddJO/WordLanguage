#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdint.h>

typedef struct _HashmapItem {
    struct _HashmapItem **prev;
    struct _HashmapItem *next;
    const char          *key;
    void                *value;
} HashmapItem, *HashmapBucket;

typedef struct _Hashmap {
    uint64_t        key[2];
    uint64_t        bucketCount; // used bucket count
    uint64_t        itemCount; // item count
    HashmapBucket   *buckets;
} Hashmap;

int hashmapInit(Hashmap *map);
int hashmapAdd(Hashmap *map, const char *key, void *element);
void *hashmapRemove(Hashmap *map, const char *key);
void *hashmapGet(Hashmap *map, const char *key);

#endif
