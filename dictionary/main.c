//
// Created by ckbsi on 9/30/2026.
//
#include "dictionary.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFFER 256

static bool parse_int(const char *str, int *out_value) {
    if (str == nullptr || *str == '\0') return false;

    char *endptr = nullptr;
    errno = 0;

    const long val = strtol(str, &endptr, 10);

    if (endptr == str) return false;

    if (*endptr != '\0') return false;

    if (errno == ERANGE || val < INT_MIN || val > INT_MAX) return false;

    *out_value = (int)val;
    return true;
}

static bool read_input(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buffer, (int)size, stdin) == nullptr) {
        return false;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    return true;
}
int main() {
    const auto map = hashmap_create(8);
    if (map == nullptr) {
        fprintf(stderr, "Failed to initialize hash map memory.\n");
        return 1;
    }

    char key_buffer[MAX_BUFFER];
    char input_buffer[MAX_BUFFER];
    int choice = 0;

    printf("===================================\n");
    printf("     HASHMAP INTERACTIVE MENU       \n");
    printf("====================================\n");

    while (true) {
        printf("Options\n");
        printf("1. Add/Update Entity\n");
        printf("2. Lookup Key\n");
        printf("3. Remove Entry\n");
        printf("4. View Map Metadata\n");
        printf("5. Exit");

        if (!read_input("\nSelect an option (1-5):", input_buffer, sizeof(input_buffer))) break;

        if (!parse_int(input_buffer, &choice)) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            continue;
        }

        switch (choice) {
            case 1: {
                if (!read_input("Enter key:", key_buffer, sizeof(key_buffer)) || strlen(key_buffer) == 0) {
                    printf("Error: Key cannot be empty.\n");
                    break;
                }

                if (!read_input("Enter integer value:", input_buffer, sizeof(input_buffer))) break;

                int val = 0;
                if (!parse_int(input_buffer, &val)) {
                    printf("Error: value must be a valid integer.\n");
                    break;
                }

                if (hashmap_put(map, key_buffer, val)) {
                    printf("Success: Stored (\"%s\" -> %d)\n", key_buffer, val);
                } else {
                    printf("Failed to store entry.\n");
                }

                break;
            }
            case 2: {
                if (!read_input("Enter key to search:", key_buffer, sizeof(key_buffer))) break;

                int found_val = 0;
                if (hashmap_get(map, key_buffer, &found_val)) {
                    printf("Found: (\"%s\" -> %d)\n", key_buffer, found_val);
                } else {
                    printf("Key \"%s\" not found.\n", key_buffer);
                }

                break;
            }
            case 3: {
                if (!read_input("Enter key to remove: ", key_buffer, sizeof(key_buffer))) break;

                if (hashmap_remove(map, key_buffer)) {
                    printf("Success: Removed key \"%s\"\n", key_buffer);
                } else {
                    printf("Key \"%s\" not found.\n", key_buffer);
                }

                break;
            }
            case 4: {
                printf("Map Metadata\n");
                printf("Size (Entries): %zu\n", hashmap_size(map));
                printf("Capacity: %zu\n", hashmap_capacity(map));

                if (hashmap_capacity(map) > 0) {
                    const float load = (float)hashmap_size(map) / (float)hashmap_capacity(map);
                    printf("Load factor: %.2f\n", load);
                }

                break;
            }
            case 5:
                printf("Exiting and releasing memory...");
                hashmap_free(map);
                return 0;

            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                break;

        }
    }

    hashmap_free(map);
    return 0;
}