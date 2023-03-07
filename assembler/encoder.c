#include "encoder.h"

const char *REGISTERS_NAMES[] = {
        "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" };

int first_scan(FILE *file, hashTable *table, int *IC, int *DC) {
    int i, j;
    /* counting starts from 1 */
    int lineNum = 0, lineNumTemp;
    /* for counting num of digits */
    int lineNumDigitCounter, maxLineNumDigitCounter = -1;
    /* reading line from file */
    char currentLine[MAX_WORD_LENGTH];
    /* label to store in symbol's table (as key) */
    char *labelName = NULL;
    char storeDataString[strlen(STRING_STORE_COMMAND)]; /* strlen(".string") = 7 */
    /* label's row in file to store in symbol's table (as value)  */
    char *labelRow = NULL;
    int currentLabelLength, maxLabelLength = -1;
    int currentLabelCommandCode;
    int insertLabelReturnCode;

    /* addressing type for each parameter */
    enum addressingType firstAddressingType, secondAddressingType;

    /* current line number of words */
    int L;

    while (fgets(currentLine, MAX_WORD_LENGTH, file) != NULL) {
        L = 0;
        lineNum++;
        lineNumDigitCounter = 0;
        currentLabelLength = 0;

        /* no label declared */
        if (!strstr(currentLine, ":")) continue;

        for (i = 0; isspace(currentLine[i]); i++);
        /* if a label is declared */
        for (; currentLine[i] != ':'; i++) {
            currentLabelLength++;
        }
        /* allocating memory */
        if (labelName == NULL) {
            maxLabelLength = currentLabelLength;
            labelName = (char *) malloc(maxLabelLength);
        } else if (maxLabelLength < currentLabelLength) {
            maxLabelLength = currentLabelLength;
            labelName = (char *) realloc(labelName, maxLabelLength);
        }
        /* memory allocation was unsuccessful */
        if (labelName == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }
        /* initializing labelName */
        strncpy(labelName, &currentLine[i-currentLabelLength], currentLabelLength);

        /* skipping whitespaces after colons */
        for (i = i+1; isspace(currentLine[i]); i++);

        /* .data or .string */
        if (currentLine[i] == '.') {
            storeDataString[0] = '.';
            /* setting storeDataString */
            for (j = i+1; isalpha(currentLine[j]); j++) {
                storeDataString[j - i] = currentLine[j];
            }

            /* insert label into hash table with DC value as value */
            /* counting digits in DC */
            lineNumTemp = *DC;
            while (lineNumTemp != 0) {
                lineNumDigitCounter++;
                lineNumTemp /= 10;
            }

            if (labelRow == NULL) {
                labelRow = (char *) malloc(lineNumDigitCounter);
                maxLineNumDigitCounter = lineNumDigitCounter;
            } else if (maxLineNumDigitCounter < lineNumDigitCounter) {
                maxLineNumDigitCounter = lineNumDigitCounter;
                labelRow = (char *) realloc(labelRow, maxLineNumDigitCounter);
            }

            if (labelRow == NULL) {
                fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
                return MEMORY_NOT_ALLOCATED_ERROR_CODE;
            }

            /* converting line number to string for storing in hash table */
            lineNumTemp = *DC;
            for (i = lineNumDigitCounter - 1; i >= 0; i--) {
                labelRow[i] = ((char) (lineNumTemp % 10)) + '0';
                lineNumTemp /= 10;
            }

            /* inserting labels into symbols table */
            insertLabelReturnCode = insert(table, labelName, labelRow);

            if (insertLabelReturnCode == HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE) {
                fprintf(stderr, "Label %s already exists!\n", labelName);
                free(labelName);
                free(labelRow);
                return -1; /* TODO: handle errors if label already exists */
            }

            /* .data declaration */
            if (strcmp(storeDataString, DATA_STORE_COMMAND) == 0) {
                /* skipping space between .data and first argument and counting commas */
                for (j = j + 1; currentLine[j] != '\0'; j++) {
                    /* TODO: encode numbers */
                    if (currentLine[j] == ',') (*DC)++;
                }
                /* number of arguments = number of commas + 1 */
                (*DC)++;
                /* .string declaration */
            } else if (strcmp(storeDataString, STRING_STORE_COMMAND) == 0) {
                /* skipping space between .string and '\"' and counting characters until '\"' */
                for (j = j + 2; currentLine[j] != '\"'; j++) {
                    /* TODO: encode characters */
                    (*DC)++;
                }
            }

            memset(labelName, 0, maxLabelLength);
            memset(labelRow, 0, lineNumDigitCounter);
            memset(currentLine, 0, MAX_WORD_LENGTH);
            memset(storeDataString, 0, 7);
            continue;
        }

        /* not a declaration of a .data or .string (incrementing IC) can be a regular command (i.e: LOOP: mov #2, r4) */
        currentLabelCommandCode = command_code(&currentLine[i]);

        encode_command(&currentLine[i], currentLabelCommandCode, &firstAddressingType, &secondAddressingType);

        /* TODO: handle extern/entry commands */

        memset(labelName, 0, maxLabelLength);
        memset(labelRow, 0, lineNumDigitCounter);
        memset(currentLine, 0, MAX_WORD_LENGTH);
    }

    free(labelName);
    free(labelRow);
    return 1;
}

int encode_mov_command(char *command, enum addressingType *first, enum addressingType *second) {
    int i;
    int tempI;
    float tempF;
    /* index in command where each parameter starts */
    int firstParamIdx, secondParamIdx;
    /* registers' number */
    int firstRegisterIdx, secondRegisterIdx;
    /* for storing parameters' values */
    void *firstValue, *secondValue;
    /* identifying addressing type with first symbol (./#/etc.) */
    char firstParamSymbol, secondParamSymbol;
    /* each parameter's substring */
    char *firstParamString, *secondParamString;
    int isFirstFloat = 0;

    firstParamIdx = strlen("mov ");
    firstParamSymbol = command[firstParamIdx];
    /* immediate addressing */
    if (firstParamSymbol == '#') {
        *first = IMMEDIATE;

        /* traversing all characters before comma */
        for (i = firstParamIdx+1; command[i] != ','; i++);
        /* creating first parameter string */
        firstParamString = (char *) malloc(i - firstParamIdx - 1);

        if (firstParamString == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }

        for (i = firstParamIdx+1; command[i] != ','; i++) {
            if (command[i] == '.') isFirstFloat = 1;
            firstParamString[i-firstParamIdx-1] = command[i];
        }

        if (isFirstFloat) {
            tempF = (float) atof(firstParamString);
            firstValue = &tempF;
        } else {
            tempI = atoi(firstParamString);
            firstValue = &tempI;
        }
        /* register addressing */
    } else if (firstParamSymbol == 'r' && strlen(firstParamString) == 2) {
        /* traversing all characters before comma */
        for (i = firstParamIdx; command[i] != ','; i++);
        /* creating first parameter string */
        firstParamString = (char *) malloc(i - firstParamIdx - 1);

        if (firstParamString == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }
        /* first param is a register (register addressing) */
        for (i = firstParamIdx; command[i] != ','; i++) {
            firstParamString[i-firstParamIdx] = command[i];
        }
        if ((firstRegisterIdx = is_register(firstParamString))) {
            *first = REGISTER;
        }
    } else {

    }

    /* checking second parameter */
    i++;
    for (; isspace(command[i]); i++);
    secondParamIdx = i;
    secondParamSymbol = command[secondParamIdx];

    if (secondParamSymbol == '.') {
        *second = DIRECT;

        /* traversing all characters before comma */
        for (i = secondParamIdx+1; command[i] != ','; i++);
        /* creating first parameter string */
        secondParamString = (char *) malloc(i - secondParamIdx - 1);

        if (secondParamString == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }
        /* first param is a label (direct addressing) */
        for (i = secondParamIdx+1; command[i] != ','; i++) {
            secondParamString[i-secondParamIdx-1] = command[i];
        }
    } else if (secondParamSymbol == 'r' && strlen(secondParamString) == 2) {
        /* checking for register addressing */
        if ((secondRegisterIdx = is_register(secondParamString))) {
            *second = REGISTER;
        }
    }

    return 0;
}

/*
int encode_cmp_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_add_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_sub_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_not_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_clr_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_lea_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_inc_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_dec_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_jmp_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_bne_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_red_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_prn_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_jsr_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_rts_command(char *command, enum addressingType *first, enum addressingType *second) {}
int encode_stop_command(char *command, enum addressingType *first, enum addressingType *second) {}*/

int command_code(char *command) {
    int i, spaceIdx;
    char *commandSubstring;
    int commandCode = UNKNOWN_COMMAND_CODE;

    if (strcmp(command, "stop") == 0) return STOP_COMMAND_CODE;

    for (spaceIdx = 0; command[spaceIdx] != ' ' && command[spaceIdx] != '\t'; spaceIdx++);
    commandSubstring = (char *) malloc(spaceIdx);

    if (commandSubstring == NULL) {
        fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
        return MEMORY_NOT_ALLOCATED_ERROR_CODE;
    }

    for (i = 0; i < spaceIdx; i++) commandSubstring[i] = command[i];

    if (strcmp(commandSubstring, "mov") == 0) commandCode = MOV_COMMAND_CODE;
    if (strcmp(commandSubstring, "cmp") == 0) commandCode = CMP_COMMAND_CODE;
    if (strcmp(commandSubstring, "add") == 0) commandCode = ADD_COMMAND_CODE;
    if (strcmp(commandSubstring, "sub") == 0) commandCode = SUB_COMMAND_CODE;
    if (strcmp(commandSubstring, "not") == 0) commandCode = NOT_COMMAND_CODE;
    if (strcmp(commandSubstring, "clr") == 0) commandCode = CLR_COMMAND_CODE;
    if (strcmp(commandSubstring, "lea") == 0) commandCode = LEA_COMMAND_CODE;
    if (strcmp(commandSubstring, "inc") == 0) commandCode = INC_COMMAND_CODE;
    if (strcmp(commandSubstring, "dec") == 0) commandCode = DEC_COMMAND_CODE;
    if (strcmp(commandSubstring, "jmp") == 0) commandCode = JMP_COMMAND_CODE;
    if (strcmp(commandSubstring, "bne") == 0) commandCode = BNE_COMMAND_CODE;
    if (strcmp(commandSubstring, "red") == 0) commandCode = RED_COMMAND_CODE;
    if (strcmp(commandSubstring, "prn") == 0) commandCode = PRN_COMMAND_CODE;
    if (strcmp(commandSubstring, "jsr") == 0) commandCode = JSR_COMMAND_CODE;
    if (strcmp(commandSubstring, "rts") == 0) commandCode = RTS_COMMAND_CODE;

    free(commandSubstring);
    return commandCode;
}

int is_register(char *string) {
    int i;
    for (i = 0; i < NUM_REGISTERS; i++) {
        /* returning the register index */
        if (strcmp(REGISTERS_NAMES[i], string) == 0) return i+1;
    }
    return 0;
}

/* mov #-1, r2 */
void encode_command(char *command, int commandCode) {
    int i;
    /* index in command where each parameter starts */
    int firstParamIdx, secondParamIdx;
    /* number of registers */
    int firstRegisterIdx, secondRegisterIdx;
    /* identifying addressing type with first symbol (./#/etc.) */
    char firstParamSymbol, secondParamSymbol;
    /* each parameter's substring */
    char *firstParamString, *secondParamString;
    /* addressing type for each parameter */
    enum addressingType firstAddressingType, secondAddressingType;

    switch (commandCode) {
        case MOV_COMMAND_CODE: {

            break;
        }
    }

    /* TODO: handle all possible addressing pairs */
}
