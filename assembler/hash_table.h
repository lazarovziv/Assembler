#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"

/* an item in macros' hash table */
typedef struct hashTableItem {
    char* key;
    char* value;
    struct hashTableItem *next;
} hashTableItem;

/* hash table to store macros' names and bodies */
typedef struct hashTable {
    hashTableItem **items;
    int size;
} hashTable;

/* hash table item for labels' names, IC/DC values and whether or not they were declared .data/.string */
typedef struct hashTableIntItem {
    char *key;
    int value;
    int isData;
    struct hashTableIntItem *next;
} hashTableIntItem;

/* hash table to store labels' names and values */
typedef struct hashTableInt {
    hashTableIntItem **items;
    int size;
} hashTableInt;

/* generating hash value withing [0, size) range to store in hash table array */
unsigned int calculate_hash(char *input, int size);
/* allocating memory for hash table array */
int init_hash_table(hashTable* table, int size);
/* getting key's value (macros' hashTableItem struct) */
char* get_value(hashTable* table, char* key);
/* inserting macro name and body strings */
int insert(hashTable* table, char* key, char* value);
/* checking whether a macro named "key" is already present in hash table */
int contains_key(hashTable* table, char* key);
/* changing "key" macro body string to "value" parameter */
int change_value(hashTable* table, char* key, char* value);

/* same but for hashTableInt (and used for labels' names as keys and IC/DC as value */
int init_hash_table_int(hashTableInt *table, int size);
int get_value_int(hashTableInt *table, char *key);
int insert_int(hashTableInt *table, char *key, int value, int isData);
int contains_key_int(hashTableInt *table, char *key);
int change_value_int(hashTableInt *table, char *key, int value, int isData);
