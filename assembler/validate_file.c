#include "validate_file.h"

#define MAX_LINE_SIZE 80

char *operationss[] = {"mov", "cmp", "add", "sub", "lea",
                       "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                       "rts", "stop"};

char *instruction_sentencee[] = {".data", ".string", ".entry", ".extern"};

int validFile(char *fileToOpen){
    FILE *file;
    int lineNumber = 1;
    char currentLine[MAX_LINE_SIZE];
    file = fopen(fileToOpen,"r");
    if(file == NULL){
        printf("Error opening my file\n");
        return 0;
    }
    while(fgets(currentLine,MAX_LINE_SIZE,file)){
        currentLine[strlen(currentLine) - 1] = '\0';
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
    char *delimiter = " \n\r\t\b";
    int index = 0;
    int i;
    char *copyLine;
    int firstWordInLine = 1;
    copyLine = (char*)malloc(sizeof(char*) * strlen(line));
    copyWord(line,copyLine,strlen(line));
    token = strtok(copyLine, delimiter);

    while(isspace(line[index]) && line[index] != '\0') index++;

    if(isLabel(token,firstWordInLine))
        index += strlen(token);

    while (token != NULL) {
        for(i = 0;i < sizeof(operationss) / sizeof(char*); i++) {
            if (strcmp(token, operationss[i]) == 0) {
                return sendToOp(line,i,index);
            }
        }

        for(i = 0;i < sizeof(instruction_sentencee) / sizeof(char*); i++){
            if(strcmp(token, instruction_sentencee[i]) == 0)
                return sendToInstruction(line, i, index);
        }

        token = strtok(NULL, delimiter);
    }

    free(copyLine);
    return 0;
}

int sendToOp(char *line,int operation,int copyFrom){
    int index = copyFrom;
    /* skip the operation word */
    while(isspace(line[index]) && line[index] != '\0') index++;
    while(isalpha(line[index]) && line[index] != '\0') index++;
    /* first group operationss */
    if((operation <= SUB_CODE && operation >= MOV_CODE) || operation == LEA_CODE)
        return firstGroupOps(operation, &line[index]);
        /* second group operationss */
    else if((operation <= JSR_CODE && operation >= CLR_CODE) || operation == NOT_CODE)
        return secondGroupOps(&line[index], operation);
        /* third group operationss */
    else return thirdGroupOps(&line[index]);
}

int sendToInstruction(char *line, int instruction, int copyFrom){
    int index = copyFrom;
    /*char *lineToSend;*/
    if(line[index] == '.')
        index++;
    /* skip the operation word */
    while(isspace(line[index]) && line[index] != '\0') index++;
    while((isalpha(line[index]) && line[index] != '\0') || line[index] == '.') index++;

    /*lineToSend = &line[index];*/
    if(instruction == 0)
        return validData(&line[index]);
    else if(instruction == 1)
        return validString(&line[index]);
    else
        return validEntryOrExtern(&line[index]);
}
