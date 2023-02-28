#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#define REGISTER_WORD_LEN 2
char *registerss[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

void copyWord(char *source, char *target, int size) {
    memcpy(target, source, size);
    target[size] = '\0';
}

int immediateAddressing(char *word) {
    int index = 1;
    if (!isdigit(word[index]) && word[index] != '+' && word[index] != '-') {
        /* TODO: error */
        return 0;
    }
    index++; /* skip ahead to the number */
    while (isdigit(word[index])) index++; /* read the entire number */
    while (isspace(word[index])) index++;

    if (word[index] != ',') {
        /* TODO: error */
        return 0;
    }
    return index;
}

void getArgument(char *line, char *target, int index) {
    int wordSize = 0;
    int copyFromMem = index;

    while (line[index] != ',' && line[index] != '\0') index++;
    wordSize = index - copyFromMem + 1;
    target = (char *) malloc(sizeof(char *) * wordSize);
    copyWord(&line[copyFromMem], target, wordSize);
}

int terminatedCorrectly(char *line, int index) {
    int i;
    for(i = index;i < strlen(line);i++){
        if(line[i] != '\0' && !isspace(line[i]))
            return 0;
    }
    return 1;
}

int isLabel(char *line) {
    int i;
    char firstChar = line[0];
    char secondChar;

    for (i = 0; i < strlen(line); i++) {
        if (!isalpha(line[i]) && !isdigit(line[i]) && line[i] != ',') {
            return 0;
        }
    }

    if(firstChar == 'r'){
        for(i = 1;i < strlen(line);i++){
            /* if we found out that its not a register */
            if(!isdigit(line[i]))
                break;
        }

        /* we made  to the end of the line and it was all number i.e r1234 */
        if(i == strlen(line)) {
            /* TODO: undefind resgister name */
            return 0;
        }
    }
    return 1;
}

int isRegister(char *line) {
    int i;
    char *reg;
    int wordSize = strlen(line);
    reg = (char*)malloc(sizeof(char*) * wordSize);
    copyWord(line,reg,wordSize);
    for(i = 0;i < sizeof(registerss) / sizeof(char *);i++){
        if(strcmp(registerss[i],reg) == 0)
            return 1;
    }
    return 0;

}