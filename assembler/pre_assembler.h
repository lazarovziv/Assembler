#include <string.h>
#include "hash_table.h"

int read_macros_from_file(FILE *file, hashTable *table, int *longestMacroBody);
int write_macros_to_file(FILE *readFile, FILE *writeFile, hashTable *table);
int deploy_macros(FILE **readFiles, FILE **writeFile, hashTable **tables, int numOfFiles, char *argv[]);

void calculate_longest_macro_body(FILE *file, int *longest);
