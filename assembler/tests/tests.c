#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hash_table.h"

int test_insert(hashTable *table) {
    char i, k;
    char key[6] = "key";
    char *value = "value1";
    for (i = 0; i < table->size; i++) {
        for (k = 0; k < 10; k++) {
            if (k <= 9) {
                key[3] = k + 48;
                key[4] = '\0';
                key[5] = '\0';
            }
            else {
                key[3] = k/10 + 48;
                key[4] = k%10 + 48;
                key[5] = '\0';
            }
            printf("for key: %s\n", key);
            insert(table, key, value);
        }
        key[4] = '\0';
        key[5] = '\0';
    }

    return 0;
}

int main() {
    int i, step;
    hashTableItem *current;
    hashTable *table = (hashTable*) malloc(sizeof(hashTable*));
    init_hash_table(table, 5);
    test_insert(table);

    for (i = 0; i < table->size; i++) {
        if (table->items[i]) {
            current = table->items[i];
            step = 0;
            printf("i: %d\n", i);
            while (current) {
                printf("step: %d\n", step);
                printf("key: %s\nvalue: %s\n", current->key, current->value);
                current = current->next;
                step++;
            }
        }
    }
    return 0;
}