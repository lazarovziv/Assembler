#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "errors.h"

#define REGISTER_WORD_LEN 2

void copyWord(char *source,char *target,int size);
/* validate the number by immediate addressing format and return its size */
int immediateAddressing(char *word);
void getArgument(char *source,char *target,int index);
/* function to check if a given line is terminated correctly */
int terminatedCorrectly(char *line,int index);
int isLabel(char *line, int firstWordInLine);
int isRegister(char *line);
int validNumber(char *num);
