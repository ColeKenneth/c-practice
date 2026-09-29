#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 16

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **buckets;
    size_t capacity;
    size_t size;
} HashMap;

static size_t hash(const char *key, const size_t capacity) {
    size_t hash = 5381;
    int c;

    while ((c = (unsigned char)*key++)) {
        hash = (hash << 5) + hash + c;
    }

    return hash % capacity;
}

static HashMap *hashmap_create() {
    const auto map = (HashMap *)malloc(sizeof(HashMap));
    if (map == nullptr) return nullptr;

    map->capacity = INITIAL_CAPACITY;
    map->size = 0;

    map->buckets = (Entry **)calloc(map->capacity, sizeof(Entry *));
    if (map->buckets == nullptr) {
        free(map);
        return nullptr;
    }

    return map;
}

static bool hashmap_put(HashMap *map, const char *key, int value) {
    if (map == nullptr || key == nullptr) return false;

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

static bool hashmap_get(const HashMap *map, const char *key, int *out_value) {
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

static void hashmap_free(HashMap *map) {
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

int main() {
    const auto map = hashmap_create();

    hashmap_put(map, "Daniel", 90);
    hashmap_put(map, "Graham", 95);
    hashmap_put(map, "Darlene", 97);

    int score = 0;
    if (hashmap_get(map, "Graham", &score)) {
        printf("Graham's score: %d\n", score);
    } else {
        printf("Key not found.\n");
    }

    hashmap_put(map, "Graham", 87);
    if (hashmap_get(map, "Graham", &score)) {
        printf("Graham's updated score: %d\n", score);
    }

    hashmap_free(map);
    return 0;
}