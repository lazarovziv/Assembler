#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "constants.h"

typedef struct hashTableItem {
    char* key;
    char* value;
} hashTableItem;

typedef struct hashTable {
    hashTableItem** items;
    int size;
} hashTable;

int calculate_hash(char *input, int size);
int init_hash_table(hashTable* table, int size);
char* get_value(hashTable* table, char* key);
void insert(hashTable* table, char* key, char* value);
int contains_key(hashTable* table, char* key);