#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "constants.h"

#define MAX_WORD_LENGTH 81
#define HASH_TABLE_SIZE 100

#define READ_MODE "r"
#define WRITE_MODE "w"
#define MACRO_KEYWORD "mcr"
#define END_MACRO_KEYWORD "endmcr"

enum macroState {
    NOT_IN_MACRO, IN_MACRO_BODY, IN_MACRO_NAME, END_MACRO
};

int read_macros_from_file(FILE *file, hashTable *table);
int write_macros_to_file(FILE *readFile, FILE *writeFile, hashTable *table);
int deploy_macros(int argc, char *argv[]);
