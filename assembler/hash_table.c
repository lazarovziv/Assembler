#include "hash_table.h"
#include <string.h>

const int PRIME_FOR_HASH = 37;
const long HASH_MOD = 1e9+7;

/* idea for a hash function */
int calculate_hash(char *input, int size) {
    int sum = 0;
    long powerPrime = 1;
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        /* TODO: multiply by (input[i]-97+1) or keep it this way? */
        /* TODO: define a numeric value for each "legal" possible character to use in a label?
         * i.e: '0'=1, '1'=2,..., '9'=10, 'a'=11, 'b'=12,...,'z'=36, 'A'=37, 'B'=38,...,'Z'=62 */
        if (islower(input[i])) sum = (sum + powerPrime * (input[i]-86)) % HASH_MOD;
        else if (isupper(input[i])) sum = (sum + powerPrime * (input[i]-28)) % HASH_MOD;
        else if (isdigit(input[i])) sum = (sum + powerPrime * (input[i]-47)) % HASH_MOD;
        powerPrime = (powerPrime * PRIME_FOR_HASH) % HASH_MOD;
    }
    return (sum % HASH_MOD) % size;
}

int init_hash_table(hashTable* table, int size) {
    int i;
    if (!table) return 0;
    table->items = (hashTableItem**) malloc(sizeof(hashTableItem*) * size);
    table->size = size;
    for (i = 0; i < size; i++) table->items[i] = 0;
    /* if memory allocated successfully */
    if (table->items != NULL) return 1;
    return 0;
}

char* get_value(hashTable* table, char* key) {
    int idx = calculate_hash(key, table->size);
    if (table->items[idx] != 0) return table->items[idx]->value;
    return 0; /* retrieval of value was unsuccessful */
}

void insert(hashTable* table, char* key, char* value) {
    int idx, keyLength, valueLength;
    idx = calculate_hash(key, table->size);
    table->items[idx] = (hashTableItem*) malloc(sizeof(hashTableItem*));
    /* TODO: if memory allocation was successful */

    /* setting key and value for hash table item */
    keyLength = strlen(key);
    valueLength = strlen(value);
    table->items[idx]->key = (char*) malloc(sizeof(char) * keyLength);
    table->items[idx]->value = (char*) malloc(sizeof(char) * valueLength);

    memcpy(table->items[idx]->key, key, keyLength);
    memcpy(table->items[idx]->value, value, valueLength);
}

int contains_key(hashTable* table, char* key) {
    int idx = calculate_hash(key, table->size);
    return table->items[idx] != 0;
}