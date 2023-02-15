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
char *registers[] = {"r1", "r2", "r3", "r4", "r5", "r6", "r7"};

int firstGroupOps(int operation, int index, char *line) {
    int i = 0;
    int wordSize = 0;
    char *word;
    int copyFromMem = 0;
    /* skip whitespaces */
    while (isspace(line[i]))
        i++;

    switch (operation) {
        /* move add sub */
        case MOV_CODE:
        case ADD_CODE:
        case SUB_CODE:
        case CMP_CODE:
        case LEA_CODE:
            /* get first argument */
            copyFromMem = i;
            while (line[i] != ',' && line[i] != '\0') i++;
            wordSize = i - copyFromMem + 1;
            word = (char *) malloc(sizeof(char *) * wordSize);
            copyWord(&line[copyFromMem], word, wordSize);
            if (groupOneFirstArg(word, operation) == 0) {
                /* TODO: error */
                return 0;
            }

            /* get second argument */
            i++; /* skip first comma */
            while (isspace(line[i]) && line[i] != '\0') i++;
            copyFromMem = i; /* start copying the second word */
            while (!isspace(line[i]) && line[i] != '\0') {
                i++;
            }
            wordSize = i - copyFromMem;
            word = (char *) malloc(sizeof(char *) * wordSize);
            copyWord(&line[copyFromMem], word, wordSize);
            if (!groupTwoSecondArg(word, operation) || !terminatedCorrectly(line,i)) {
                /* TODO: error */
                return 0;
            }
            break;


    }
    return 1;
}

int groupOneFirstArg(char *word, int operation) {
    char firstCharacter = word[0];
    int index = 1; /* get second character */
    int i;
    char reg[REGISTER_WORD_LEN];
    int foundReg = 0;

    if (operation >= MOV_CODE && operation <= SUB_CODE) {
        switch (firstCharacter) {
            case '#':
                if (immediateAddressing(word) == 0) {
                    /* TODO: error invalid number */
                    return 0;
                }
                break;

            case 'r':
                memcpy(reg, word, REGISTER_WORD_LEN);
                for (i = 0; i < strlen((const char *) registers); i++)
                    if (strcmp(registers[i], reg) == 0) {
                        foundReg = 1;
                    }

                if (foundReg == 0) /* haven't found a valid name for the register */{
                    /* TODO: error */
                    return 0;
                }

                index++;
                while (isspace(word[index]))
                    index++;
                /* missing comma */
                if (word[index] != ',') {
                    /* TODO: error */
                    return 0;
                }
                break;

            default:
                /* TODO: missing parameter error */
                return 0;
        }
        /* if we didn't enter the switch statement and first parameter is a label */
        if (isalpha(firstCharacter) && foundReg == 0) {
            while (isalpha(word[index]) || isdigit(word[index]))
                index++;
            while (isspace(word[index])) index++;
            if (word[index] != ',') {
                /* TODO: error */
                return 0;
            }
        }
    }
    if (operation == LEA_CODE) {
        if (immediateAddressing(word) == 0) {
            /* TODO: error */
            return 0;
        }
    }
    return 1;
}

/* send the word into this function with one comma already checked in order to check multiple commas */
int groupTwoSecondArg(char *word, int operation) {
    char firstCharacter = word[0];
    int i;
    char *reg;
    int foundReg = 0;

    if (firstCharacter == ',') {
        /* TODO: error multiple commas */
        return 0;
    }

        /* check if it's a register */
    else if (firstCharacter == 'r') {
        reg = (char *) malloc(sizeof(char *) * REGISTER_WORD_LEN);
        copyWord(word, reg, REGISTER_WORD_LEN);
        for (i = 0; i < strlen((const char *) registers); i++) {
            if (strcmp(reg, registers[i]) == 0) {
                foundReg = 1;
                break;
            }
        }
    } else if (isalpha(firstCharacter)) {
        for (i = 0; i < strlen(word); i++) {
            if (!isalpha(word[i]) && !isdigit(word[i])) {
                /* TODO: error, invalid label name */
                return 0;
            }
        }
    } else if (operation == CMP_CODE && word[firstCharacter] == '#') {
        if (immediateAddressing(word)) {
            /* TODO: error invalid number */
            return 0;
        }
    } else {
        /* TODO: error invalid parameter */
        return 0;
    }

    return 1;
}