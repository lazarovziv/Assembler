#include "input_validation.h"

int firstGroupOps(int operation, char *line) {
    int i = 0;
    int wordSize = 0;
    char *firstParameter;
    char *secondParameter;
    int copyFromMem = 0;
    int numOfWords = 3; /* number of words to encode */

    /* skip whitespaces */
    while (isspace(line[i]))
        i++;

    /* get first argument */
    copyFromMem = i;
    while (line[i] != ',' && line[i] != '\0') i++;
    wordSize = i - copyFromMem;
    firstParameter = (char *) malloc(sizeof(char *) * wordSize);

    if(firstParameter == NULL){
        errors(23);
        return 0;
    }
    copyWord(&line[copyFromMem], firstParameter, wordSize);
    if(line[i] == '\0'){
        errors(20);
        free(firstParameter);
        return 0;
    }
    if(line[i] != ','){
        /* TODO: error missing comma */
        errors(7);
        free(firstParameter);
        return 0;
    }

    /* get second argument */
    i++; /* skip first comma */
    while (isspace(line[i]) && line[i] != '\0') i++;
    copyFromMem = i; /* start copying the second word */

    if(line[i] == '\0'){
        errors(20);
        free(firstParameter);
        return 0;
    }
    while (isalpha(line[i]) || isdigit(line[i])) {
        i++;
    }
    wordSize = i - copyFromMem;
    secondParameter = (char *) malloc(sizeof(char *) * wordSize);

    if(secondParameter == NULL){
        errors(23);
        return 0;
    }
    copyWord(&line[copyFromMem], secondParameter, wordSize);


    if (groupOneFirstArg(firstParameter, operation) == 0) {
        /* TODO: error */
        free(firstParameter);
        free(secondParameter);
        return 0;
    }


    if (!groupOneSecondArg(secondParameter, operation)) {
        /* TODO: error */
        free(firstParameter);
        free(secondParameter);
        return 0;
    }

    if(!terminatedCorrectly(line, i)){
        errors(4);
        free(firstParameter);
        free(secondParameter);
        return 0;
    }

    if(isRegister(firstParameter) && isRegister(secondParameter))
        numOfWords = 2;

    free(firstParameter);
    free(secondParameter);
    return numOfWords;
}

int secondGroupOps(char *line, int operation) {
    int i = 0;
    int wordSize = 0;
    int copyFromMem = 0;
    int numOfWords = 2;
    char *argument;
    char *firstParam;
    char *secondParam;

    /* skip whitespaces */
    while (isspace(line[i]) && line[i] != '\0') i++;


    /* get first argument */
    copyFromMem = i;
    while ((isalpha(line[i]) || isdigit(line[i])) || line[i] == '#' || line[i] == '+' || line[i] == '-') i++;
    wordSize = i - copyFromMem;
    argument = (char *) malloc(sizeof(char *) * wordSize);

    if(argument == NULL){
        errors(23);
        return 0;
    }
    copyWord(&line[copyFromMem], argument, wordSize);

    switch (operation) {
        case NOT_CODE:
        case DEC_CODE:
        case INC_CODE:
        case CLR_CODE:
        case RED_CODE:
            if (!validRegisterOrLabel(argument)) {
                free(argument);
                return 0;
            }
            break;
        case PRN_CODE:
            if (!immediateAddressing(argument) && !validRegisterOrLabel(argument)  ) {
                free(argument);
                return 0;
            }
            break;
        case JMP_CODE:
        case BNE_CODE:
        case JSR_CODE:
/* if any its without parenthesis like bne END */
            if(isLabel(argument,0) && terminatedCorrectly(line,i))
                return 2;

            if(line[i] != '('){
                errors(19);
                return 0;
            }
            /* must jump into a label */
            if (!isLabel(argument,0)) {
                free(argument);
                return 0;
            }
            i++; /* skip the '(' */
            /* get first parameter */
            copyFromMem = i;
            if(line[i] == '#')
                i++;
            while(isalpha(line[i]) || isdigit(line[i]) || line[i] == '-' || line[i] == '+') i++;
            wordSize = i - copyFromMem;
            firstParam = (char *) malloc(sizeof(char *) * wordSize);

            if(firstParam == NULL){
                errors(23);
                return 0;
            }
            copyWord(&line[copyFromMem], firstParam, wordSize);

            i++;
            copyFromMem = i; /* start copying the second word */
            while (isalpha(line[i]) || isdigit(line[i]) || line[i] == '#') {
                i++;
            }
            wordSize = i - copyFromMem;
            secondParam = (char *) malloc(sizeof(char *) * wordSize);

            if(secondParam == NULL){
                errors(23);
                return 0;
            }
            copyWord(&line[copyFromMem], secondParam, wordSize);

            if(firstParam[0] == '#') {
                if (!immediateAddressing(firstParam)) {
                    free(firstParam);
                    free(secondParam);
                    return 0;
                }
            }
            else if(!isRegister(firstParam) && !isLabel(firstParam,0)) {
                free(firstParam);
                free(secondParam);
                return 0;
            }
            if(secondParam[0] == '#') {
                if (!immediateAddressing(secondParam)) {
                    free(firstParam);
                    free(secondParam);
                    return 0;
                }
            }
            else if(!isRegister(secondParam) && !isLabel(secondParam,0)) {
                free(firstParam);
                free(secondParam);
                return 0;
            }

            if(line[i] != ')') {
                free(firstParam);
                free(secondParam);
                errors(18);
                return 0;
            }



            if(isRegister(firstParam) && isRegister(secondParam))
                numOfWords = 3;
            else numOfWords = 4;
            free(firstParam);
            free(secondParam);
            break;
    }
    /* skip the ) */
    if(line[i] == ')')
        i++;
    while(isspace(line[i]) && line[i] != '\0') i++;
    if(!terminatedCorrectly(line, i)) {
        /* TODO: error */
        errors(4);
        free(firstParam);
        free(secondParam);
        return 0;
    }
    free(argument);

    return numOfWords ;
}

int groupOneFirstArg(char *word, int operation) {
    char firstCharacter = word[0];
    int index = 0; /* get second character */


    if (operation >= MOV_CODE && operation <= SUB_CODE) {
        if (firstCharacter == '#') {
            if( immediateAddressing(word) == 0) {
                /* TODO: error invalid number */
                return 0;
            }
        } else if (!isLabel(word,0) && !isRegister(word)) {
            /* not a label and not a register */
            return 0;
        }
    } else if (operation == LEA_CODE && immediateAddressing(word)) {
        /* TODO: error */
        return 0;
    }
    while (!isspace(word[index]) && word[index] != '\0' && word[index] != ',') {
        index++;
    }
    while (isspace(word[index]) && word[index] != '\0')
        index++;

    return 1;
}

/* send the word into this function with one comma already checked in order to check multiple commas */
int groupOneSecondArg(char *word, int operation) {
    char firstCharacter = word[0];

    if (firstCharacter == ',') {
        /* TODO: error multiple commas */
        return 0;
    } else if (strlen(word) == 0) {
        /* TODO: missing parameter */
        return 0;
    } else if (operation == CMP_CODE && word[(int) firstCharacter] == '#' && immediateAddressing(word)) {

        /* TODO: error invalid number */
        return 0;
    } else if (!isLabel(word,0) && !isRegister(word)) {
        /* TODO: error */
        return 0;
    }


    return 1;
}

int validRegisterOrLabel(char *line) {
    if (strlen(line) == 0) {
        /* missing parameter */
        errors(20);
        return 0;
    } else if (!isLabel(line,0) && !isRegister(line)) {
        return 0;
    }
    return 1;
}

int thirdGroupOps(char *line){
    int i = 0;
    while(isalpha(line[i])) i++;
    return terminatedCorrectly(line,i);
}

int validData(char *line){
    int i = 0;
    enum status state;
    char *currentNum;
    int copyFromHere;
    int numOfDigits;
    int numOfWords = 0;
    state = NUMBER;
    /* skip whitespaces */
    while(isspace(line[i])) i++;

    while(line[i] != '\0'){
        switch(state){
            case NUMBER:
                numOfWords += 1;
                copyFromHere = i;
                while(!isspace(line[i]) && line[i] != ',' && line[i] != '\0') i++;
                numOfDigits = i - copyFromHere;
                currentNum = (char*)malloc(sizeof(char*) * numOfDigits);
                if(currentNum == NULL){
                    errors(23);
                    return 0;
                }
                copyWord(&line[copyFromHere],currentNum,numOfDigits);
                /* invalid number input */
                if(validNumber(currentNum) == 0){
                    free(currentNum);
                    return 0;
                }

                /* continue running until ',' or '\0' */
                while(isspace(line[i])) i++;
                if(line[i] != ',' && line[i] != '\0') {
                    free(currentNum);
                    return 0;
                }

                if(line[i] == '\0')
                    state = DONE;
                else
                    state = COMMA;
                break;

            case COMMA:
                i++;
                while(isspace(line[i])) i++;
                if(line[i] != '+' && line[i] != '-' && !isdigit(line[i])) {
                    free(currentNum);
                    return 0;
                }
                state = NUMBER;
                break;

            case DONE:
                free(currentNum);
                return 1;
        }
    }
    free(currentNum);
    return numOfWords;
}

int validString(char *line){
    int i = 0;
    int finalChar = strlen(line) - 1;
    /*skip whitespaces */
    while(isspace(line[i])) i++;

    if(line[i] != '"' || line[finalChar] != '"')
        return 0;
    return strlen(&line[i]) - 2;
}

int validEntryOrExtern(char *line){
    int i = 0;
    int copyFrom = 0;

    /*skip whitespaces */
    while(isspace(line[i])) i++;
    copyFrom = i;

    if(line[i] == '\0'){
        errors(20);
        return 0;
    }

    if(isRegister(&line[i])){
        errors(22);
        return 0;
    }

    while(isalpha(line[i]) || isdigit(line[i])) i++;
    while(isspace(line[i])) i++;

    if(line[i] != '\0'){
        errors(4);
        return 0;
    }
    return isLabel(&line[copyFrom],0);
}

