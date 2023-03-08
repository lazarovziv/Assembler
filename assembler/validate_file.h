#include <stdio.h>
#include <string.h>
#include "input_validation.h"
#include "../constants.h"
#define MAX_LINE_SIZE 80
int validFile(char *fileToOpen);
int validLine(char *line);
int sendToOp(char *line,int operation,int copyFrom);
int sendToInstruction(char *line, int instruction, int copyFrom);