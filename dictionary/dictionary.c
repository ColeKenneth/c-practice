#include "dictionary.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_FACTOR 0.75f
#define DEFAULT_CAPACITY 16

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

struct HashMap {
    Entry **buckets;
    size_t capacity;
    size_t size;
};

static size_t hash(const char *key, const size_t capacity) {
   size_t hash = 5381;
    int c;

    while ((c = (unsigned char)*key++)) {
        hash = (hash << 5) + hash + c;
    }

    return hash % capacity;
}

static bool hashmap_resize(HashMap *map, const size_t new_capacity) {
    Entry **new_buckets = calloc(new_capacity, sizeof(Entry *));

    if (new_buckets == nullptr) return false;

    for (size_t i = 0; i < map->capacity; ++i) {
        Entry *current = map->buckets[i];

        while (current != nullptr) {
            Entry *next = current->next;

            const size_t new_index = hash(current->key, new_capacity);

            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;
            current = next;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;

    return true;
}

HashMap *hashmap_create(const size_t initial_capacity) {
    const auto map = (HashMap *)malloc(sizeof(HashMap));
    if (map == nullptr) return nullptr;

    map->capacity = (initial_capacity > 0) ? initial_capacity : DEFAULT_CAPACITY;
    map->size = 0;

    map->buckets = (Entry **)calloc(map->capacity, sizeof(Entry *));
    if (map->buckets == nullptr) {
        free(map);
        return nullptr;
    }

    return map;
}

bool hashmap_put(HashMap *map, const char *key, int value) {
    if (map == nullptr || key == nullptr) return false;

    const float current_load_factor = (float)(map->size + 1) / (float)map->capacity;
    if (current_load_factor > MAX_LOAD_FACTOR) {
        hashmap_resize(map, map->capacity * 2);
    }

    const size_t index = hash(key, map->capacity);
    Entry *current = map->buckets[index];

    while (current != nullptr) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return true;
        }

        current = current->next;
    }

    const auto new_entry = (Entry *)malloc(sizeof(Entry));
    if (new_entry == nullptr) return false;

    new_entry->key = strdup(key);
    if (new_entry->key == nullptr) {
        free(new_entry);
        return false;
    }

    new_entry->value = value;
    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;
    map->size++;

    return true;
}

bool hashmap_get(const HashMap *map, const char *key, int *out_value) {
    if (map == nullptr || key == nullptr || out_value == nullptr) return false;

    const size_t index = hash(key, map->capacity);
    const Entry *current = map->buckets[index];

    while (current != nullptr) {
        if (strcmp(current->key, key) == 0) {
            *out_value = current->value;
            return true;
        }
        current = current->next;
    }

    return false;
}

bool hashmap_remove(HashMap *map, const char *key) {
    if (map == nullptr || key == nullptr) return false;

    const size_t index = hash(key, map->capacity);
    Entry *current = map->buckets[index];
    Entry *prev = nullptr;

    while (current != nullptr) {
        if (strcmp(current->key, key) == 0) {
            if (prev == nullptr) {
                map->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free(current->key);
            free(current);
            map->size--;
            return true;
        }
        prev = current;
        current = current->next;
    }

    return false;
}

size_t hashmap_size(const HashMap *map) {
    return map ? map->size : 0;
}

size_t hashmap_capacity(const HashMap *map) {
    return map ? map->capacity : 0;
}

void hashmap_free(HashMap *map) {
    if (map == nullptr) return;

    for (size_t i = 0; i < map->capacity; ++i) {
        Entry *current = map->buckets[i];
        while (current != nullptr) {
            Entry *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }

    free(map->buckets);
    free(map);
}