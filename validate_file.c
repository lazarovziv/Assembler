#include <stdio.h>
#include <string.h>
#include "input_validation.h"
#include "constants.h"
#include <stdlib.h>
#include "functions.h"
#include "validate_file.h"
#include "errors.h"
#define MAX_LINE_SIZE 80

int validFile(char *fileToOpen);
int validLine(char *line);


char *operationss[] = {"mov", "cmp", "add", "sub", "lea",
                       "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                       "rts", "stop"};

char *instruction_sentencee[] = {".data", ".string", ".entry", ".extern"};

int validFile(char *fileToOpen){
    FILE *file;
    char currentLine[MAX_LINE_SIZE];
    file = fopen(fileToOpen,"r");
    int lineNumber = 1;
    if(file == NULL){
        printf("Error opening my file\n");
        return 0;
    }
    while(fgets(currentLine,MAX_LINE_SIZE,file)){
        if(!validLine(currentLine)) {
            fprintf(stderr,"Error found on line: %d\n\n",lineNumber);
            fclose(file);
            return 0;
        }
        lineNumber++;
    }
    fclose(file);
    return 1;
}

int validLine(char *line){
    char *token;
    char *delimiter = " \n";
    int index = 0;
    int i;
    char *copyLine;
    int firstWordInLine = 1;
    copyLine = (char*)malloc(sizeof(char*) * strlen(line));
    copyWord(line,copyLine,strlen(line));
    token = strtok(line, delimiter);


    while (token != NULL) {
        for(i = 0;i < sizeof(operationss) / sizeof(char*); i++) {
            if (strcmp(token, operationss[i]) == 0) {
                return sendToOp(copyLine,i,index);
            }
        }

        for(i = 0;i < sizeof(instruction_sentencee) / sizeof(char*); i++){
            if(strcmp(token, instruction_sentencee[i]) == 0)
                return sendToInstruction(copyLine, i, index);
        }

        /* if its a definition of a label check it */
        if(firstWordInLine){
            if(!isLabel(token,firstWordInLine))
                return 0;
            firstWordInLine = 0;
        }
        index += strlen(token) + strlen(delimiter);
        token = strtok(NULL, delimiter);
    }

    free(copyLine);
    return 1;
}

int sendToOp(char *line,int operation,int copyFrom){
    int index = copyFrom;
    /* first group operationss */
    if((operation <= SUB_CODE && operation >= MOV_CODE) || operation == LEA_CODE)
        return firstGroupOps(operation, &line[index + strlen(operationss[operation])]);
        /* second group operationss */
    else if((operation <= JSR_CODE && operation >= CLR_CODE) || operation == NOT_CODE)
        return secondGroupOps(&line[index + strlen(operationss[operation])], operation);
        /* third group operationss */
    else return thirdGroupOps(&line[index + strlen(operationss[operation])]);
}

int sendToInstruction(char *line, int instruction, int copyFrom){
    int index = copyFrom;
    char *lineToSend = &line[index + strlen(instruction_sentencee[instruction])];
    if(instruction == 0)
        return validData(lineToSend);
    else if(instruction == 1)
        return validString(lineToSend);
    else
        return validEntryOrExtern(lineToSend);
}