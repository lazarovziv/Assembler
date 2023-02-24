#include <stdio.h>
#include <stdlib.h>
#include "../hash_table.h"

int test_insert(hashTable *table) {
    char k;
    char key[6] = "key";
    char *value = "value1";
    for (k = 0; k < 15; k++) {
        if (k <= 9) {
            key[3] = k + 48;
            key[4] = '\0';
            key[5] = '\0';
        } else {
            key[3] = k/10 + 48;
            key[4] = k%10 + 48;
            key[5] = '\0';
        }
        insert(table, key, value);
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
                printf("key: %s\n", current->key);
                current = current->next;
                step++;
            }
            printf("\n");
        }
    }
    return 0;
}