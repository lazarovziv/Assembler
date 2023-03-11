#include <string.h>
#include "hash_table.h"
#include "validate_file.h"

enum addressingType {
    IMMEDIATE, DIRECT, JUMP, REGISTER
};

int first_scan(FILE *file, FILE *writeFile, hashTableInt *table, int *IC, int *DC);
int command_code(char *command);
int is_register(char *string);

void convert_to_special_binary(int num, char finalString[]);
