#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 7

typedef struct HashEntry {
    char *key;
    int value;
    int is_occupied; // flag indicate the slot occupied
    int is_deleted; // flag indicate the slot has been deleted
} HashEntry;

typedef struct HashMap {
    HashEntry table[TABLE_SIZE];
} HashMap;

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void init_hashmap(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->table[i].key = NULL;
        map->table[i].is_occupied = 0;
        map->table[i].is_deleted = 0;
    }
}

void insert(HashMap *map, const char *key, int value) {
    unsigned long index = hash(key) % TABLE_SIZE;

    for (int i = 0; i < TABLE_SIZE; i++) {
        int try = (index + i) % TABLE_SIZE; // linear probing
        if (!map->table[try].is_occupied || map->table[try].is_deleted) {
            map->table[try].key = strdup(key);
            map->table[try].value = value;
            map->table[try].is_occupied = 1;
            map->table[try].is_deleted = 0;
            printf("Inserted (%s, %d) at index %d\n", key, value, try);
            return;
        }
    }

    printf("Hash table is full. Cannot insert (%s, %d)\n", key, value);

}

int search(HashMap *map, const char *key, int *found) {
    unsigned long index = hash(key) % TABLE_SIZE;

    for (int i = 0; i < TABLE_SIZE; i++) {
        int try = (index + i) % TABLE_SIZE;
        if (map->table[try].is_occupied) {
            if (strcmp(map->table[try].key, key) == 0 && !map->table[try].is_deleted) {
                *found = 1;
                return map->table[try].value;
            }
        } else if (!map->table[try].is_occupied && !map->table[try].is_deleted) {
            // There’s no point in continuing the search further because, in linear probing, keys are always inserted into the first available slot.
            break;
        }
    }

    *found = 0;
    return 0;
}

void delete(HashMap *map, const char *key) {
    unsigned long index = hash(key) % TABLE_SIZE;

    for (int i = 0; i < TABLE_SIZE; i++) {
        int try = (index + i) % TABLE_SIZE;

        if (map->table[try].is_occupied) {
            if (strcmp(map->table[try].key, key) == 0 && !map->table[try].is_deleted) {
                // Mark as deleted
                map->table[try].is_deleted = 1;
                map->table[try].is_occupied = 0;
                printf("deleted key %s at index %d", key, try);
                return;
            }
        } else if (!map->table[try].is_occupied && !map->table[try].is_deleted) {
            printf("Key not found");
            return;
        }
    }

    printf("Key not found");
}

//void benchmark() {
//    HashMap *map = init_hashmap(16);
//
//    clock_t start = clock();
//    for (int i = 0; i < 100000; i++) {
//        char key[20];
//        sprintf(key, "key%d", i);
//        insert(map, key, i);
//    }
//    clock_t end = clock();
//    printf("Insert Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
//
//    start = clock();
//    for (int i = 0; i < 100000; i++) {
//        char key[20];
//        sprintf(key, "key%d", i);
//        int found;
//        search(map, key, &found);
//    }
//    end = clock();
//    printf("Search Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
//
//    free_hashmap(map);
//}

int main() {
    HashMap map;
    init_hashmap(&map);

    insert(&map, "apple", 123);
    insert(&map, "orange", 111);
    insert(&map, "banana", 555);

    int found;
    int value = search(&map, "apple", &found);
    if (found)
        printf("Found key 'apple' with value: %d\n", value);
    else
        printf("Not found\n");

    value = search(&map, "grape", &found);
    if (found)
        printf("Found key 'grape' with value: %d\n", value);
    else
        printf("Not found\n");


    return 0;
}

