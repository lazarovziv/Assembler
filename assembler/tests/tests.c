#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

int test_insert(hashTable *table) {
    int i, k;
    char *key = (char*) malloc(sizeof(char) * 5), *value = (char*) malloc(sizeof(char) * 5);
    if (table == NULL) return -1;
    for (i = 0; i < table->size; i++) {
        for (k = 0; k < 5; k++) {
            key[k] = (i*k) % 128;
            value[k] = (i*k) % 128;
        }
        insert(table, key, value);
    }
    return 0;
}

int main() {
    int i;
    hashTableItem *current;
    hashTable *table = (hashTable*) malloc(sizeof(hashTable*));
    init_hash_table(table, 10000);
    test_insert(table);

    for (i = 0; i < table->size; i++) {
        if (table->items[i]) {
            current = table->items[i];
            printf("i: %d\n", i);
            while (current) {
                printf("key: %s\nvalue: %s\n", current->key, current->value);
                current = current->next;
            }
        }
    }
    return 0;
}