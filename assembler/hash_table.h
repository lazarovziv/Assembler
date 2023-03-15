#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"

typedef struct hashTableItem {
    char* key;
    char* value;
    struct hashTableItem *next;
} hashTableItem;

typedef struct hashTable {
    hashTableItem **items;
    int size;
} hashTable;

typedef struct hashTableIntItem {
    char *key;
    int value;
    struct hashTableIntItem *next;
} hashTableIntItem;

typedef struct hashTableInt {
    hashTableIntItem **items;
    int size;
} hashTableInt;

unsigned int calculate_hash(char *input, int size);
int init_hash_table(hashTable* table, int size);
char* get_value(hashTable* table, char* key);
int insert(hashTable* table, char* key, char* value);
int contains_key(hashTable* table, char* key);
int change_value(hashTable* table, char* key, char* value);

/* for hashTableInt */
int init_hash_table_int(hashTableInt *table, int size);
int get_value_int(hashTableInt *table, char *key);
int insert_int(hashTableInt *table, char *key, int value);
int contains_key_int(hashTableInt *table, char *key);
int change_value_int(hashTableInt *table, char *key, int value);
