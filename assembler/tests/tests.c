#include <stdio.h>
#include <stdlib.h>
#include "../hash_table.h"

void print_table(hashTable *table) {
    int i, step;
    hashTableItem *current;
    for (i = 0; i < table->size; i++) {
        if (table->items[i]) {
            current = table->items[i];
            step = 0;
            printf("i: %d\n", i);
            while (current) {
                printf("step: %d\n", step);
                printf("key: %s\n", current->key);
                printf("value: %s\n", current->value);
                current = current->next;
                step++;
            }
            printf("\n");
        }
    }
    printf("\n");
}

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

    print_table(table);

    return 0;
}

int test_change_value(hashTable *table) {
    char key[] = "key14";
    char value[] = "changed";
    if (change_value(table, key, value)) print_table(table);

    return 0;
}

int main() {
    hashTable *table = (hashTable*) malloc(sizeof(hashTable*));
    init_hash_table(table, 5);

    test_insert(table);

    test_change_value(table);

    return 0;
}
