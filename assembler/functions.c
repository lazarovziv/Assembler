#include "functions.h"
char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
char *operations[] = {"mov", "cmp", "add", "sub", "lea",
                      "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                      "rts", "stop"};

char *instruction_sentence[] = {".data", ".string", ".entry", ".extern"};

void copyWord(char *source, char *target, int size) {
    memcpy(target, source, size);
    target[size] = '\0';
}

int immediateAddressing(char *word) {
    int index = 1;
    if(word[0] != '#')
        return 0;
    if (!isdigit(word[index]) && word[index] != '+' && word[index] != '-' ) {
        /* TODO: error */
        errors(10);
        return 0;
    }
    index++; /* skip ahead to the number */
    while (isdigit(word[index])) index++; /* read the entire number */
    if(word[index] != '\0' && !isspace(word[index])) {
        /* TODO: error invalid number */
        return 0;
    }
    while (isspace(word[index])) index++;



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
    int i = index;
    while(isspace(line[i]) && line[i] != '\0')
        i++;
    return line[i] == '\0';
}

int isLabel(char *line,int firstWordInLine) {
    int i = 0;
    int labelDefinition = 0;
    char firstChar = line[0];
    char *firstWord;

    firstWord = (char*)malloc(sizeof (char*) * strlen(line));

    if(strlen(line) > 30){
        errors(2);
        return 0;
    }
    /* if-else in order to check if its a definition of a label or a parameter */
    if(firstWordInLine) {
        while(isdigit(line[i]) || isalpha(line[i])) i++;
        if(line[i] == ':') {
            copyWord(line, firstWord, i);
            labelDefinition = 1;
        }
    }
    else
        copyWord(line,firstWord,strlen(line));



    for(i = 0;i < sizeof(operations) / sizeof(char*); i++){
        if(strcmp(firstWord, operations[i]) == 0 && labelDefinition){
            /* TODO: error label is named as an operation */
            errors(14);
            return 0;
        }
    }


    for(i = 0;i < sizeof(instruction_sentence) / sizeof(char*); i++){
        if(strcmp(firstWord, instruction_sentence[i]) == 0 && labelDefinition){
            /* TODO: error label is named as an instruction */
            errors(13);
            return 0;
        }
    }

    for (i = 0; i < sizeof(registers) / sizeof(char *); i++) {
        if (strcmp(firstWord, registers[i]) == 0) {
            if(firstWordInLine) {
                /* TODO: error label defined as a register */
                errors(15);
                return 0;
            }
            else
                return 1;
        }
    }

    if(!isalpha(firstChar) && firstChar != '.') {
        /* TODO: error first character isnt alphabet */
        errors(0);
        return 0;
    }

    for (i = 0; i < strlen(firstWord) && !isspace(firstWord[i]); i++) {
        if (!isalpha(firstWord[i]) && !isdigit(firstWord[i]) && labelDefinition) {
            errors(17);
            return 0;
        }
    }

    if(firstChar == 'r'){
        for(i = 1;i < strlen(firstWord);i++){
            if(firstWordInLine && i == strlen(firstWord) - 1){
                /* label is defind as the first word in line so it need to end with : */
                return firstWord[i] == ':';
            }
                /* if we found out that its not a register */
            else if(!isdigit(firstWord[i]))
                break;
        }

        /* we made  to the end of the line and it was all number i.e r1234 */
        if(i == strlen(firstWord)) {
            /* TODO: undefind resgister name */
            errors(16);
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
    for(i = 0;i < sizeof(registers) / sizeof(char *);i++){
        if(strcmp(registers[i],reg) == 0)
            return 1;
    }
    free(reg);
    return 0;

}

int validNumber(char *num){
    int i = 0;
    if(!isdigit(num[0]) && num[0] != '+' && num[0] != '-')
        return 0;
    i++;
    while(num[i] != '\0'){
        if(!isdigit(num[i])) {
            errors(10);
            return 0;
        }
        i++;
    }
    return 1;
}

