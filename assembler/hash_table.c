#include "hash_table.h"
#include <string.h>

#define FIRST_PRIME 54059 /* a prime */
#define SECOND_PRIME 76963 /* another prime */
#define THIRD_PRIME 86969 /* yet another prime */
#define START_NUM 37 /* also prime */

const int PRIME_FOR_HASH = 37;
const int HASH_MOD = 1e9+7;

/* idea for a hash function */
unsigned int calculate_hash(char *input, int size) {
    unsigned int sum = START_NUM;
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        /* setting each possible character a code for hashing function:
         * '0'=1, '1'=2,..., '9'=10, 'a'=11, 'b'=12,...,'z'=36, 'A'=37, 'B'=38,...,'Z'=62 */
        if (islower(input[i])) sum = (sum * FIRST_PRIME) ^ ((input[i]-86) * SECOND_PRIME);  /* sum = (sum + powerPrime * (input[i]-86)) % HASH_MOD; */
        else if (isupper(input[i])) sum = (sum * FIRST_PRIME) ^ ((input[i]-28) * SECOND_PRIME); /* sum = (sum + powerPrime * (input[i]-28)) % HASH_MOD; */
        else if (isdigit(input[i])) sum = (sum * FIRST_PRIME) ^ ((input[i]-47) * SECOND_PRIME); /* sum = (sum + powerPrime * (input[i]-47)) % HASH_MOD; */
    }
    return sum % size;
}

int init_hash_table(hashTable* table, int size) {
    int i;
    if (!table) return 0;
    table->items = (hashTableItem**) malloc(sizeof(hashTableItem*) * size);

    if (table->items == NULL) return 0;
    printf("INIT\n");
    table->size = size;
    for (i = 0; i < size; i++) {
        table->items[i] = 0;

    }
    /* if memory allocated successfully */
    return 1;
}

char* get_value(hashTable* table, char* key) {
    int idx;
    hashTableItem *current = 0;
    if (!contains_key(table, key)) {
        fprintf(stderr, HASH_TABLE_KEY_DOESNT_EXIST_ERROR_MESSAGE);
        return NULL; /* retrieval of value was unsuccessful */
    }
    idx = calculate_hash(key, table->size);
    current = table->items[idx];
    if (strcmp(key, table->items[idx]->key) == 0) return table->items[idx]->value;
    while (current) {
        if (strcmp(key, current->key) == 0) return current->value;
        current = current->next;
    }
    return 0;
}

int insert(hashTable* table, char* key, char* value) {
    int idx, keyLength, valueLength;
    hashTableItem *current = 0, *next = 0;
    /* before calculating hash, checking if key exists */
    if (contains_key(table, key)) return HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE;

    printf("INSERT\n");

    idx = calculate_hash(key, table->size);
    table->items[idx] = (hashTableItem*) malloc(sizeof(hashTableItem*));
    current = table->items[idx];

    /* if memory allocation was unsuccessful */
    if (current == NULL) {
        fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
        return MEMORY_NOT_ALLOCATED_ERROR_CODE;
    }

    /* setting key and value for hash table item */
    keyLength = strlen(key);
    valueLength = strlen(value);
    current->key = (char*) malloc(sizeof(char) * keyLength);
    current->value = (char*) malloc(sizeof(char) * valueLength);

    strcpy(current->key, key);
    strcpy(current->value, value);

    /* everything went fine */
    return 1;
}

int contains_key(hashTable* table, char* key) {
    hashTableItem *current  = 0;
    int idx = calculate_hash(key, table->size);
    if (table->items[idx] == NULL) return 0;
    current = table->items[idx];

    printf("Checking key: %s (%lu)\n", key, strlen(key));
    if (strcmp(table->items[idx]->key, key) == 0) return 1;
    printf("checking list...\n");
    while (current) {
        printf("current: %p\n", current);
        if (strcmp(current->key, key) == 0) return 1;
        current = current->next;
    }
    return 0;
}

int change_value(hashTable* table, char* key, char* value) {
    int idx, valueLength, itemValueLength;
    /* can't change value which isn't in table */
    if (!contains_key(table, key)) {
        fprintf(stderr, HASH_TABLE_KEY_DOESNT_EXIST_ERROR_MESSAGE);
        return HASH_TABLE_KEY_DOESNT_EXIST_ERROR_CODE;
    }

    idx = calculate_hash(key, table->size);
    itemValueLength = strlen(table->items[idx]->value);
    valueLength = strlen(value);

    /* if value's length is longer than item's length */
    if (valueLength > itemValueLength) {
        table->items[idx]->value = (char*) realloc(table->items[idx]->value,
                                                   sizeof(char) * valueLength);
        /* filling new memory with NULL */
        memset(table->items[idx]->value, 0, sizeof(char) * valueLength);
    } else {
        /* filling previous value with NULL */
        memset(table->items[idx]->value, 0, sizeof(char) * itemValueLength);
    }
    /* copying value to item */
    memcpy(table->items[idx]->value, value, sizeof(char) * valueLength);
    return 1;
}
