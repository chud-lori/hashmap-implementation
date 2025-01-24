#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct KeyValue {
    char *key;
    int value;
    struct KeyValue *next;
} KeyValue;

typedef struct HashMap {
    KeyValue **buckets;
    int capacity;
    int size;
} HashMap;

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        //printf("C is %c", c);
        hash = ((hash << 5) + hash) + c; // hash * 33 + c, shifting 5 same as * 32 and + hash meaning * 32
    }

    return hash;
}

HashMap *init_hashmap(int capacity) {
    HashMap *map = malloc(sizeof(HashMap));
    map->capacity = capacity;
    map->size = 0;
    map->buckets = calloc(capacity, sizeof(KeyValue*));
    return map;
}

void insert(HashMap *map, const char *key, int value) {
    unsigned long index = hash(key) % map->capacity;
    printf("HASH: %lu\t", hash(key));
    printf("INDEX: %lu\n", index);
    KeyValue *newNode = malloc(sizeof(KeyValue));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
    map->size++;
}

int search(HashMap *map, const char *key, int *found) {
    unsigned long index = hash(key) % map->capacity;
    KeyValue *current = map->buckets[index];
    while (current) {
        if(strcmp(current->key, key) == 0) {
            *found = 1;
            return current->value;
        }
        current = current->next;
    }
    *found = 0;
    return 0;
}

void delete(HashMap *map, const char *key) {
    unsigned long index = hash(key) % map->capacity;
    KeyValue *current = map->buckets[index];
    KeyValue *prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                map->buckets[index] = current->next;
            }
            free(current->key);
            free(current);
            map->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_hashmap(HashMap *map) {
    for (int i = 0; i < map->capacity; i++) {
        KeyValue *current = map->buckets[i];
        while (current) {
            KeyValue *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }

    free(map->buckets);
    free(map);
}

void benchmark() {
    HashMap *map = init_hashmap(16);

    clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        insert(map, key, i);
    }
    clock_t end = clock();
    printf("Insert Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < 100000; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        int found;
        search(map, key, &found);
    }
    end = clock();
    printf("Search Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    free_hashmap(map);
}

int main() {
    //benchmark();
    HashMap *map = init_hashmap(3);
    insert(map, "apple", 12);
    insert(map, "jaja", 44);
    insert(map, "orange", 333);

    int found;
    int value = search(map, "orange", &found);
    if (found) {
        printf("Value of 'apple': %d\n", value);
    }

    delete(map, "banana");

    free_hashmap(map);

    return 0;
}

