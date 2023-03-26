#include <string.h>
#include "hash_table.h"
#include "validate_file.h"

enum addressingType {
    IMMEDIATE, DIRECT, JUMP, REGISTER
};

/* checking .am file for labels, data/string declarations and encoding all statements that don't include a label
 * setting every to-be-encoded-label's line with the label's name */
int first_scan(FILE *file, FILE *writeFile, hashTableInt *table, int *IC, int *DC,
               hashTableInt *entriesTable, hashTableInt *externsTable);
/* checking .temp_ob file created from first_scan function and checking every line marked with a label name to encode it with it's value inserted in first_scan */
int second_scan(char *fileName, FILE *readFile, FILE *writeFile, hashTableInt *table, hashTableInt *entriesTable, hashTableInt *externsTable, int *IC, int *DC);

/* dividing to each and every command and its' use cases. using it in write_to_ob_file function */
int encode_regular_command(FILE *writeFile, short operationEncode, char *line, int lineNum, char encodedString[]);
/* encoding .data/.string statements (label or not) */
void encode_data_command(FILE *writeFile, char *line, int isData, int lineNum, char encodedString[], int *DC);

/* getting command code from specific substring */
int command_code(char *command);
/* returning index of register if substring is a register, -1 otherwise */
int is_register(char *string);
/* following label names guidelines and rules, and returning 1 whether it is, 0 otherwise */
int is_label(char *string);

/* converting a number to the special binary base defined and setting the string in finalString parameter */
void convert_to_special_binary(int num, char finalString[]);
/* writing the special binary representation of parameter num and writing the line number to its' left */
void write_to_ob_file(int num, char finalString[], int lineNum, FILE *file);
