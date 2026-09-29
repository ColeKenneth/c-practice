#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 30

static int compare_strings(const void* a, const void* b) {
    const char* str_a = *(const char**)a;
    const char* str_b = *(const char **)b;
    return strcmp(str_a, str_b);
}

int main() {
    int capacity = 5;
    char** names = malloc(capacity * sizeof(char*));
    if (names == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }

    int count = 0;
    char buffer[MAX_LEN];

    while (1) {
        printf("Enter name %d (empty to stop): ", count + 1);

        if (fgets(buffer, MAX_LEN, stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '\0') break;

        if (count >= capacity) {
            capacity *= 2;
            char** temp = realloc(names, capacity * sizeof(char*));

            if (temp == NULL) {
                fprintf(stderr, "realloc failed\n");
                free(names);
                return 1;
            }
            names = temp;
        }

        names[count] = malloc(strlen(buffer) + 1);
        if (names[count] == NULL) {
            fprintf(stderr, "malloc failed\n");

            for (int i = 0; i < count; i++) {
                free(names[i]);
            }

            free(names);
            return 1;
        } 

        strncpy(names[count], buffer, strlen(buffer) + 1);
        count++;
    }

    qsort(names, count, sizeof(char*), compare_strings);

    printf("\nSorted Names (Ascending):\n");
    for (int i = 0; i < count; i++) {
        printf("%d . %s\n", i + 1, names[i]);
    }

    for (int i = 0; i < count; i++) {
        free(names[i]);
    }

    free(names);

    return 0;
}