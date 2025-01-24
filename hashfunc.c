/*
 Reference: http://www.cse.yorku.ca/~oz/hash.html
*/
#include <stdio.h>
#include <time.h>

// djb2
unsigned long djb(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}


// sdbm
unsigned long sdbm(char *str) {
    unsigned long hash = 0;
    int c;

    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned long loselose(char *str) {
    unsigned int hash = 0;
    int c;

    while ((c = *str++)) {
        hash += c;
    }

    return hash;
}

void benchmark() {

    clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        djb(key);
    }
    clock_t end = clock();
    printf("(DJB2) Hash Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < 100000; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        sdbm(key);
    }
    end = clock();
    printf("(SDBM) Hash Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < 100000; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        sdbm(key);
    }
    end = clock();
    printf("(LOSE LOSE) Hash Time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

}

int main() {
    benchmark();
    return 0;
}
