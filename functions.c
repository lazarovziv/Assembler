#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "functions.h"
#define REGISTER_WORD_LEN 2

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
        case 0:
        case 2:
        case 3:
            /* get first argument */
            copyFromMem = i;
            while (line[i] != '\n' && line[i] != ',' && line[i] != '\0') i++;
            wordSize = i - copyFromMem + 1;
            word = (char *) malloc(sizeof(char *) * wordSize);
            memcpy(word, &line[copyFromMem], wordSize);
            if (groupOneFirstArg(word) == 0) {
                /* TODO: error */
                return 0;
            }

            /* get second argument */
            i++; /* skip first comma */
            while(isspace(line[i])) i++;
            copyFromMem = i; /* start copying the second word */
            while (line[i] != '\0') i++;
            wordSize = i - copyFromMem;
            word = (char *) malloc(sizeof(char *) * wordSize);
            memcpy(word, &line[copyFromMem], wordSize);
            if (!groupTwoSecondArg(word)) {
                /* TODO: error */
                return 0;
            }
            break;


            /* lea */
        case 4:
            /* TODO: implement lea case */
            break;


    }
    return 1;
}

int groupOneFirstArg(char *word) {
    char firstCharacter = word[0];
    int index = 1; /* get second character */
    int i;
    char reg[REGISTER_WORD_LEN];
    int foundReg = 0;

    switch (firstCharacter) {
        case '#': {
            if (!isdigit(word[index]) && word[index] != '+' && word[index] != '-')
                /* TODO: error */
                return 0;
        }
            index++; /* skip ahead to the number */
            while(isdigit(word[index])) index++; /* read the entire number */
            while(isspace(word[index])) index++;

            if (word[index] != ',') {
                /* TODO: error */
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
    }
    /* if we didn't enter the switch statement and first parameter is a label */
    if (isalpha(firstCharacter) && firstCharacter != 'r') {
        while (isalpha(word[index]) || isdigit(word[index]))
            index++;
        while(isspace(word[index])) index++;
        if (word[index] != ',') {
            /* TODO: error */
            return 0;
        }
    }
    return 1;
}

/* send the word into this function with one comma already checked in order to check multiple commas */
int groupTwoSecondArg(char *word) {
    char firstCharacter = word[0];
    int i;
    char reg[REGISTER_WORD_LEN];
    int foundReg = 0;

    if(firstCharacter == ','){
        /* TODO: error multiple commas */
        return 0;
    }

        /* check if it's a register */
    else if (firstCharacter == 'r') {
        memcpy(reg, &word[firstCharacter], 2);
        for (i = 0; i < strlen((const char *) registers); i++) {
            if (strcmp(reg, registers[i]) == 0) {
                foundReg = 1;
                break;
            }
        }
        /* check if line is not terminated correctly */
        if (foundReg == 1) {
            for (i = 2; word[i] != '\0'; i++) {
                if (!isspace(word[i])) {
                    /* TODO: error line not terminated correctly */
                    return 0;
                }
            }
        }
    } else if (isalpha(firstCharacter)) {
        for (i = 0; i < strlen(word); i++) {
            if (!isalpha(word[i]) && !isdigit(word[i])) {
                /* TODO: error, invalid label name */
                return 0;
            }
        }
    }
    return 1;
}