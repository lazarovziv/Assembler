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

    if (table->items == NULL) return MEMORY_NOT_ALLOCATED_ERROR_CODE;
    table->size = size;

    for (i = 0; i < table->size; i++) {
        table->items[i] = NULL;
    }
    /* if memory allocated successfully */
    return 1;
}

char* get_value(hashTable* table, char* key) {
    int idx;
    hashTableItem *current;

    if (!contains_key(table, key)) {
        fprintf(stderr, HASH_TABLE_KEY_DOESNT_EXIST_ERROR_MESSAGE);
        return NULL; /* retrieval of value was unsuccessful */
    }

    idx = calculate_hash(key, table->size);
    current = table->items[idx];

    if (strcmp(key, table->items[idx]->key) == 0) return table->items[idx]->value;
    /* continue through the linked list */
    while (current) {
        if (strcmp(key, current->key) == 0) return current->value;
        current = current->next;
    }

    return 0;
}

int insert(hashTable* table, char* key, char* value) {
    int idx, keyLength, valueLength;
    hashTableItem *current, *last;
    if (strlen(key) == 0) return HASH_TABLE_INSERT_EMPTY_KEY_ERROR_CODE;
    if (strlen(value) == 0) return HASH_TABLE_INSERT_EMPTY_VALUE_ERROR_CODE;
    /* before calculating hash, checking if key exists */
    if (contains_key(table, key)) return HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE;

    idx = calculate_hash(key, table->size);
    last = (hashTableItem*) malloc(sizeof(hashTableItem*));

    /* if memory allocation was unsuccessful */
    if (last == NULL) {
        fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
        return MEMORY_NOT_ALLOCATED_ERROR_CODE;
    }

    /* if idx is not taken */
    if (table->items[idx] == NULL) table->items[idx] = last;
    else {
        current = table->items[idx];
        while (current->next != NULL) current = current->next;
        current->next = last;
    }

    /* setting key and value for hash table item */
    keyLength = strlen(key);
    valueLength = strlen(value);
    last->key = (char*) malloc(keyLength);
    last->value = (char*) malloc(valueLength);

    if (last->key == NULL || last->value == NULL) {
        fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
        return MEMORY_NOT_ALLOCATED_ERROR_CODE;
    }

    strcpy(last->key, key);
    strcpy(last->value, value);

    /* everything went fine */
    return 1;
}

int contains_key(hashTable* table, char* key) {
    hashTableItem *current;
    int idx = calculate_hash(key, table->size);
    if (table->items[idx] == NULL) return 0;
    current = table->items[idx];

    if (strcmp(table->items[idx]->key, key) == 0) return 1;

    while (current) {
        if (strcmp(current->key, key) == 0) return 1;
        current = current->next;
    }
    return 0;
}

int change_value(hashTable* table, char* key, char* value) {
    int idx, valueLength, itemValueLength;
    hashTableItem *current;
    /* can't change value which isn't in table */
    if (!contains_key(table, key)) {
        fprintf(stderr, HASH_TABLE_KEY_DOESNT_EXIST_ERROR_MESSAGE);
        return HASH_TABLE_KEY_DOESNT_EXIST_ERROR_CODE;
    }

    idx = calculate_hash(key, table->size);
    current = table->items[idx];
    while (current) {
        if (strcmp(current->key, key) == 0) break;
        current = current->next;
    }

    itemValueLength = strlen(current->value);
    valueLength = strlen(value);

    /* if value's length is longer than item's length */
    if (valueLength > itemValueLength) {
        current->value = (char*) realloc(current->value,
                                                   valueLength);
        if (current->value == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }
        /* filling new memory with NULL */
        memset(current->value, 0, valueLength);
    } else if (valueLength == itemValueLength) {
        /* filling previous value with NULL */
        memset(current->value, 0, itemValueLength);
    } else if (valueLength > 0) {
        current->value = (char*) realloc(current->value,
                                         valueLength);
        if (current->value == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }

        memset(current->value, 0, valueLength);
    }
    /* copying value to item */
    strcpy(current->value, value);
    return 1;
}
