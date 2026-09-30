//
// Created by ckbsi on 9/30/2026.
//

#ifndef CPRACTICE_DICTIONARY_H
#define CPRACTICE_DICTIONARY_H

#include <stddef.h>

typedef struct HashMap HashMap;

HashMap *hashmap_create(size_t initial_capacity);
bool hashmap_put(HashMap *map, const char *key, int value);
bool hashmap_get(const HashMap *map, const char *key, int *out_value);
bool hashmap_remove(HashMap *map, const char *key);
size_t hashmap_size(const HashMap *map);
size_t hashmap_capacity(const HashMap *map);
void hashmap_free(HashMap *map);

#endif //CPRACTICE_DICTIONARY_H
