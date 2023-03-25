#include <string.h>
#include "hash_table.h"

/* inserting macros' names and bodies values into hash table (table param) */
int read_macros_from_file(FILE *file, hashTable *table, int *longestMacroBody);
/* creating .am file and deploying all macros' bodies based on their names from hash table */
int write_macros_to_file(FILE *readFile, FILE *writeFile, hashTable *table);
/* process of reading from .as file and writing to .am file */
int deploy_macros(FILE *readFile, FILE *writeFile, hashTable *labelTable, int *longestMacroBodyLength);

/* checking longest macro body for memory allocation in read_macros_from_file */
void calculate_longest_macro_body(FILE *file, int *longest);

/* used to check whether a macro name is a command name */
int is_command(char *line);
