#include "encoder.h"

const char *REGISTERS_NAMES[] = {
        "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" };

int first_scan(FILE *file, FILE *writeFile, hashTableInt *table, int *IC, int *DC) {
    int i, j, startParamsIdx;
    /* flag whether to encode currentLine of not */
    int encodeLine = 1;
    /* counting starts from 1 */
    int lineNum = 0;
    /* reading line from file */
    char currentLine[MAX_WORD_LENGTH];
    char copyCurrentLine[MAX_WORD_LENGTH];
    char *token;
    /* label to store in symbol's table (as key) */
    char *labelName = NULL;
    char storeDataString[STRING_STORE_COMMAND_LENGTH]; /* strlen(".string") = 7 */
    int currentLabelLength, maxLabelLength = -1;
    int insertLabelReturnCode;
    int numRegistersInCurrentLine = 0, firstRegisterIdx, secondRegisterIdx;
    /* for iterating through linked list in hash table incrementing each node by IC */
    hashTableIntItem *current;
    /* current line number of words */
    int L;
    /* encoded string */
    char encodedString[NUM_BITS];

    short encodedWord = 0;
    short operationEncode, firstParamEncode, secondParamEncode, srcEncode, destEncode, externEntryEncode;
    operationEncode = firstParamEncode = secondParamEncode = srcEncode = destEncode = externEntryEncode = 0;

    while (fgets(currentLine, MAX_WORD_LENGTH, file) != NULL) {
        L = 0;
        strcpy(copyCurrentLine, currentLine);
        copyCurrentLine[strlen(copyCurrentLine)] = '\0';
        L = validLine(copyCurrentLine);
        lineNum++;
        currentLabelLength = 0;

        if (!L) {
            /* TODO: print error */
            printf("Line %d invalid!\n", lineNum);
            encodeLine = 0;
            continue;
        }

        if (!encodeLine) {
            encodeLine = 1;
            continue;
        }

        /* no label declared */
        if (!strstr(currentLine, ":")) {
            /* TODO: handle "regular" commands */
            continue;
        }

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
        labelName[currentLabelLength] = '\0';

        /* skipping whitespaces after colons */
        /*for (i = i+1; isspace(currentLine[i]); i++);*/

        /* skipping colon and space */
        i += 2;

        /* .data or .string */
        if (currentLine[i] == '.') {
            /* setting storeDataString */
            for (j = i; currentLine[j] != ' '; j++) {
                storeDataString[j-i] = currentLine[j];
            }
            storeDataString[j-i] = '\0';

            /* insert label into hash table with DC value as value */
            insertLabelReturnCode = insert_int(table, labelName, *DC);

            if (insertLabelReturnCode == HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE) {
                fprintf(stderr, "Label %s already exists!\n", labelName);
                free(labelName);
                return -1; /* TODO: handle errors if label already exists */
            }

            /* .data declaration */
            if (strcmp(storeDataString, DATA_STORE_COMMAND) == 0) {
                startParamsIdx = j+1;
                token = strtok(&copyCurrentLine[startParamsIdx], ", \n");
                while (token != NULL) {
                    encodedWord = (short) atoi(token);
                    encodedWord = encodedWord << 2;
                    convert_to_special_binary(encodedWord, encodedString);
                    fprintf(writeFile, "%s\n", encodedString);
                    (*DC)++;
                    token = strtok(NULL, ", \n");
                }

                /* .string declaration */
            } else if (strcmp(storeDataString, STRING_STORE_COMMAND) == 0) {
                startParamsIdx = j + 2;
                /* skipping space between .string and '"' and counting characters until '"' */
                for (j = startParamsIdx; currentLine[j] != '"'; j++) {
                    encodedWord = (short) currentLine[j];
                    encodedWord = encodedWord << 2;
                    convert_to_special_binary(encodedWord, encodedString);
                    fprintf(writeFile, "%s\n", encodedString);
                    (*DC)++;
                }
                convert_to_special_binary(0, encodedString);
                /* writing \0 value at the end */
                fprintf(writeFile, "%s\n", encodedString);
            }

            memset(labelName, 0, maxLabelLength);
            memset(currentLine, 0, MAX_WORD_LENGTH);
            memset(copyCurrentLine, 0, MAX_WORD_LENGTH);
            memset(storeDataString, 0, STRING_STORE_COMMAND_LENGTH);
            continue;
            /* not storing data */
        } else {
            insertLabelReturnCode = insert_int(table, labelName, *IC);

            if (insertLabelReturnCode == HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE) {
                fprintf(stderr, "Label %s already exists!\n", labelName);
                free(labelName);
                return -1; /* TODO: handle errors if label already exists */
            }

            for (startParamsIdx = 0; currentLine[startParamsIdx] != ':'; startParamsIdx++);
            startParamsIdx++;
            strcpy(copyCurrentLine, currentLine);
            token = strtok(&copyCurrentLine[startParamsIdx], " \t\n,");
            operationEncode = command_code(token);

            while (token != NULL) {
                if (command_code(token) == -1) {
                    if (numRegistersInCurrentLine == 0) {
                        if ((firstRegisterIdx = is_register(token))) {
                            numRegistersInCurrentLine++;
                            srcEncode = firstRegisterIdx;
                        }
                    } else if (numRegistersInCurrentLine == 1) {
                        if ((secondRegisterIdx = is_register(token))) {
                            numRegistersInCurrentLine++;
                            destEncode = secondRegisterIdx;
                        }
                    }
                }
                token = strtok(NULL, " \t\n,");
            }

            /* can be mov, cmp, add, sub */
            if (numRegistersInCurrentLine == 2) {
                /* writing operation code in first line and registers in one line */
                encodedWord = (firstRegisterIdx << SHIFTS_FOR_FIRST_REGISTER) | (secondRegisterIdx << SHIFTS_FOR_SECOND_REGISTER);
                convert_to_special_binary((operationEncode << SHIFTS_FOR_OPCODE), encodedString);
                fprintf(writeFile, "%s\n", encodedString);
                convert_to_special_binary(encodedWord, encodedString);
                fprintf(writeFile, "%s\n", encodedString);
            }

            firstRegisterIdx = secondRegisterIdx = 0;

            (*IC) += L;
            continue;
        }

        /* not a declaration of a .data or .string (incrementing IC) can be a regular command (i.e: LOOP: mov #2, r4) */

        /* TODO: handle extern/entry commands */

        memset(labelName, 0, maxLabelLength);
        memset(currentLine, 0, MAX_WORD_LENGTH);
        memset(copyCurrentLine, 0, MAX_WORD_LENGTH);

        (*IC) += L;
    }

    (*DC) += *IC;

    /* incrementing all symbol's table values by IC */
    for (i = 0; i < table->size; i++) {
        if (table->items[i]) {
            current = table->items[i];
            while (current) {
                current->value += *IC;
                current = current->next;
            }
        }
    }

    free(labelName);

    return 1;
}

void convert_to_special_binary(int num, char finalString[]) {
    int i;
    /* mask = 0010000000000000 */
    short mask = 1 << (NUM_BITS-1);

    for (i = 0; i < NUM_BITS; i++) {
        if (num & mask) finalString[i] = '/';
        else finalString[i] = '.';
        mask = mask >> 1;
    }
    finalString[NUM_BITS] = '\0';
}

int command_code(char *command) {
    int i, spaceIdx;
    char *commandSubstring;
    int commandCode = UNKNOWN_CODE;

    if (strcmp(command, "stop") == 0) return STOP_CODE;

    for (spaceIdx = 0; command[spaceIdx] != ' ' && command[spaceIdx] != '\t'; spaceIdx++);
    commandSubstring = (char *) malloc(spaceIdx);

    if (commandSubstring == NULL) {
        fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
        return MEMORY_NOT_ALLOCATED_ERROR_CODE;
    }

    for (i = 0; i < spaceIdx; i++) commandSubstring[i] = command[i];

    if (strcmp(commandSubstring, "mov") == 0) commandCode = MOV_CODE;
    if (strcmp(commandSubstring, "cmp") == 0) commandCode = CMP_CODE;
    if (strcmp(commandSubstring, "add") == 0) commandCode = ADD_CODE;
    if (strcmp(commandSubstring, "sub") == 0) commandCode = SUB_CODE;
    if (strcmp(commandSubstring, "not") == 0) commandCode = NOT_CODE;
    if (strcmp(commandSubstring, "clr") == 0) commandCode = CLR_CODE;
    if (strcmp(commandSubstring, "lea") == 0) commandCode = LEA_CODE;
    if (strcmp(commandSubstring, "inc") == 0) commandCode = INC_CODE;
    if (strcmp(commandSubstring, "dec") == 0) commandCode = DEC_CODE;
    if (strcmp(commandSubstring, "jmp") == 0) commandCode = JMP_CODE;
    if (strcmp(commandSubstring, "bne") == 0) commandCode = BNE_CODE;
    if (strcmp(commandSubstring, "red") == 0) commandCode = RED_CODE;
    if (strcmp(commandSubstring, "prn") == 0) commandCode = PRN_CODE;
    if (strcmp(commandSubstring, "jsr") == 0) commandCode = JSR_CODE;
    if (strcmp(commandSubstring, "rts") == 0) commandCode = RTS_CODE;

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
