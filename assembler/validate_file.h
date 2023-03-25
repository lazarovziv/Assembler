#include "input_validation.h"
#include "constants.h"

#define MAX_LINE_SIZE 80

/* function to check if an entire assembly file has valid syntax */
int validFile(char *fileToOpen);

/* checks if a specific line has valid syntax, if so it returns the number of words to be encoded into memory */
int validLine(char *line);

/* determines to which function we need to send the current line according to the number of operation */
int sendToOp(char *line,int operation,int copyFrom);

/* determines to which function we need to send the current line according to the number of instruction */
int sendToInstruction(char *line, int instruction, int copyFrom);
