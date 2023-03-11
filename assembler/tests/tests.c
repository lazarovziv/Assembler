#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../encoder.h"
#include "../pre_assembler.h"

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
                printf("key (length %lu): %s\n", strlen(current->key), current->key);
                printf("value:\n%s", current->value);
                current = current->next;
                step++;
            }
            printf("\n");
        }
    }
    printf("\n");
}

void print_table_int(hashTableInt *table) {
    int i, step;
    hashTableIntItem *current;
    for (i = 0; i < table->size; i++) {
        if (table->items[i]) {
            current = table->items[i];
            step = 0;
            printf("i: %d\n", i);
            while (current) {
                printf("step: %d\n", step);
                printf("key (length %lu): %s\n", strlen(current->key), current->key);
                printf("value:\n%d\n", current->value);
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

int test_change_value_int(hashTableInt *table) {
    char key[] = "key14";
    int value = 18;
    if (change_value_int(table, key, value)) print_table_int(table);
}

int test_insert_int(hashTableInt *table) {
    char k;
    char key[6] = "key";
    int x = 16;
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
        insert_int(table, key, x);
    }

    print_table_int(table);

    return 0;
}

void test_read_from_file() {
    int longest = -1;
    FILE *file = fopen("../x.as", "r");
    hashTable *table = (hashTable *) malloc(sizeof(hashTable));
    init_hash_table(table, HASH_TABLE_SIZE/10);

    calculate_longest_macro_body(file, &longest);
    printf("longest: %d\n", longest);
    rewind(file);
    read_macros_from_file(file, table, &longest);

    print_table(table);
}

void test_first_scan() {
    int IC = 0, DC = 0;
    FILE *file = fopen("y.as", READ_MODE);
    FILE *writeFile = fopen("y.ob", WRITE_MODE);

    hashTableInt *table = (hashTableInt *) malloc(sizeof(hashTable));
    init_hash_table_int(table, 10);

    first_scan(file, writeFile, table, &IC, &DC);

    printf("DC: %d\nIC: %d\n", DC, IC);

    print_table_int(table);

}

int main() {
    /*hashTableInt *table = (hashTableInt *) malloc(sizeof(hashTableInt));
    init_hash_table_int(table, 10);
    test_insert_int(table);
    printf("--------------------------\n");
    test_change_value_int(table);*/

    /*test_read_from_file();*/

    test_first_scan();


    return 0;
}
