#include "validate_file.h"

int validFile(char *fileToOpen);
int validLine(char *line);


char *operationss[] = {"mov", "cmp", "add", "sub", "lea",
                       "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                       "rts", "stop"};

char *instruction_sentencee[] = {".data", ".string", ".entry", ".extern"};

int validFile(char *fileToOpen){
    int lineNumber = 1;
    FILE *file;
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
    int i = 0;
    int copyFromHere = 0;
    char *copyLine;
    char *temp;
    int firstWordInLine = 1;
    int foundLabel = 0;
    while(isspace(line[i])) i++;
    copyFromHere = i;
    while((isalpha(line[i]) || isdigit(line[i]))) i++;


    if(line[i] == ':'){
        temp = (char*)malloc(sizeof(char*) * i + 1);
        if(temp == NULL){
            errors(23);
            return 0;
        }
        copyWord(&line[copyFromHere],temp,i - copyFromHere + 1);
        if(isLabel(temp,firstWordInLine)){
            index += strlen(temp);
            i += 1;
            foundLabel = 1;
        }
        else {
            free(temp);
            return 0;
        } /* invalid label decleration */
    }
    if(!foundLabel)
        i = 0;
    copyLine = (char*)malloc(sizeof(char*) * strlen(line));

    if(copyLine == NULL){
        errors(23);
        return 0;
    }
    copyWord(&line[i],copyLine,strlen(line));
    token = strtok(copyLine, delimiter);

    index = i + 1;
    while (token != NULL) {
        for(i = 0;i < sizeof(operationss) / sizeof(char*); i++) {
            if (strcmp(token, operationss[i]) == 0) {
                free(copyLine);
                return sendToOp(line,i,index);
            }
        }

        for(i = 0;i < sizeof(instruction_sentencee) / sizeof(char*); i++){
            if(strcmp(token, instruction_sentencee[i]) == 0) {
                free(copyLine);
                return sendToInstruction(line, i, index);
            }
        }

        token = strtok(NULL, delimiter);
    }
    /* TODO: error if token was invalid */
    errors(21);
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
    if(line[index] == '.')
        index++;
    /* skip the operation word */
    while(isspace(line[index]) && line[index] != '\0') index++;
    while((isalpha(line[index]) && line[index] != '\0') || line[index] == '.') index++;

    while(isspace(line[index])) index++;

    if(line[index] == '\0'){
        errors(20);
        return 0;
    }

    if(instruction == 0)
        return validData(&line[index]);
    else if(instruction == 1)
        return validString(&line[index]);
    else
        return validEntryOrExtern(&line[index]);
}
