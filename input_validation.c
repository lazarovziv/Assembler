#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "input_validation.h"
#include "constants.h"
#include "functions.h"
#include <stdio.h>

char *instruction_sentence[] = {".data", ".string", ".entry", ".extern"};
char *firstGroup[] = {"mov", "cmp", "add", "sub", "lea"};
char *secondGroup[] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
char *thirdGroup[] = {"rts", "stop"};
char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

int firstGroupOps(int operation, int index, char *line) {
    int i = 0;
    int wordSize = 0;
    char *word;
    char *firstParameter;
    char *secondParameter;
    int copyFromMem = 0;
    /* skip whitespaces */
    while (isspace(line[i]))
        i++;

    /* get first argument */
    copyFromMem = i;
    while (line[i] != ',' && line[i] != '\0') i++;
    wordSize = i - copyFromMem + 1;
    firstParameter = (char *) malloc(sizeof(char *) * wordSize);
    copyWord(&line[copyFromMem], firstParameter, wordSize);

    /* get second argument */
    i++; /* skip first comma */
    while (isspace(line[i]) && line[i] != '\0') i++;
    copyFromMem = i; /* start copying the second word */
    while (!isspace(line[i]) && line[i] != '\0') {
        i++;
    }
    wordSize = i - copyFromMem;
    secondParameter = (char *) malloc(sizeof(char *) * wordSize);
    copyWord(&line[copyFromMem], secondParameter, wordSize);


    if (groupOneFirstArg(firstParameter, operation) == 0) {
        /* TODO: error */
        return 0;
    }


    if (!groupOneSecondArg(secondParameter, operation) || !terminatedCorrectly(line, i)) {
        /* TODO: error */
        return 0;
    }


    return 1;
}

int secondGroupOps(char *line){
    int i = 0;
    int wordSize = 0;
    char *param;
    int copyFromMem = 0;

    /* skip whitespaces */
    while (isspace(line[i]))
        i++;

    /* get first argument */
    copyFromMem = i;
    while (line[i] != ',' && line[i] != '\0') i++;
    wordSize = i - copyFromMem + 1;
    param = (char *) malloc(sizeof(char *) * wordSize);
    copyWord(&line[copyFromMem], param, wordSize);

    if(!groupTwoArg(line) || !terminatedCorrectly(line,i - 1))
        return 0;

    return 1;
}

int groupOneFirstArg(char *word, int operation) {
    char firstCharacter = word[0];
    int index = 0; /* get second character */
    int i;
    char *reg;
    int foundReg = 0;

    if (operation >= MOV_CODE && operation <= SUB_CODE) {

        if (firstCharacter == '#' && immediateAddressing(word) == 0) {

            /* TODO: error invalid number */
            return 0;
        } else if (isalpha(firstCharacter) && !isLabel(word) && !isRegister(word)) {
            /* not a label and not a register */
            return 0;
        }
    } else if (operation == LEA_CODE && immediateAddressing(word)) {

        /* TODO: error */
        return 0;
    }
    while (!isspace(word[index]) && word[index] != '\0' && word[index] != ',') {
        printf("current: %c\n", word[index]);
        index++;
    }
    while (isspace(word[index]) && word[index] != '\0')
        index++;
    if (word[index] != ',') {
        /* TODO: error */
        return 0;
    }
    return 1;
}

/* send the word into this function with one comma already checked in order to check multiple commas */
int groupOneSecondArg(char *word, int operation) {
    char firstCharacter = word[0];

    if (firstCharacter == ',' ) {
        /* TODO: error multiple commas */
        return 0;
    }
    else if(strlen(word) == 0){
        /* TODO: missing parameter */
        return 0;
    }
    else if (operation == CMP_CODE && word[firstCharacter] == '#' && immediateAddressing(word)) {

        /* TODO: error invalid number */
        return 0;
    }
    else if (!isLabel(word) && !isRegister(word)) {
        /* TODO: error */
        return 0;
    }



    return 1;
}

int groupTwoArg(char *line){
    if(strlen(line) == 0){
        /* missing parameter */
    }
    else if(!isLabel(line) && !isRegister(line)){
        return 0;
    }
    return 1;
}