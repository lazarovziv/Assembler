#include "validate_file.h"
#define MAX_LINE_SIZE 80

char *operations[] = {"mov", "cmp", "add", "sub", "lea",
                      "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                      "rts", "stop"};

char *instruction_sentence[] = {".data", ".string", ".entry", ".extern"};

int validFile(char *fileToOpen){
    FILE *file;
    char currentLine[MAX_LINE_SIZE];
    file = fopen(fileToOpen,"r");
    if(file == NULL){
        printf("Error opening my file\n");
        return 0;
    }
    while(fgets(currentLine,MAX_LINE_SIZE,file)){
        if(!validLine(currentLine)) {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int validLine(char *line){
    char *token;
    char *delimiter = " ";
    int index = 0;
    int i;
    char *copyLine;
    copyLine = (char*)malloc(sizeof(char*) * strlen(line));
    copyWord(line,copyLine,strlen(line));
    token = strtok(line, delimiter);

    while (token != NULL) {
        for(i = 0;i < sizeof(operations) / sizeof(char*);i++) {
            if (strcmp(token, operations[i]) == 0) {
                return sendToOp(copyLine,i,index);
            }
        }

        for(i = 0;i < sizeof(instruction_sentence) / sizeof(char*);i++){
            if(strcmp(token,instruction_sentence[i]) == 0)
                return sendToInstruction(copyLine, i, index);
        }
        index += strlen(token) + strlen(delimiter);
        token = strtok(NULL, delimiter);
    }
    return 0;
}

int sendToOp(char *line,int operation,int copyFrom){
    int index = copyFrom;
    /* first group operations */
    if((operation <= SUB_CODE && operation >= MOV_CODE) || operation == LEA_CODE)
        return firstGroupOps(operation, &line[index + strlen(operations[operation])]);
        /* second group operations */
    else if((operation <= JSR_CODE && operation >= CLR_CODE) || operation == NOT_CODE)
        return secondGroupOps(&line[index + strlen(operations[operation])],operation);
        /* third group operations */
    else return thirdGroupOps(&line[index + strlen(operations[operation])]);
}

int sendToInstruction(char *line, int instruction, int copyFrom){
    int index = copyFrom;
    char *lineToSend = &line[index + strlen(instruction_sentence[instruction])];
    if(instruction == 0)
        return validData(lineToSend);
    else if(instruction == 1)
        return validString(lineToSend);
    else
        return validEntryOrExtern(lineToSend);
}
