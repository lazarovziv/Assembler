#include "encoder.h"

const char *REGISTERS_NAMES[] = {
        "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8" };

/* TODO: instead of encoding ? for labels, perhaps encode num of label occurrence in each line
 * to identify in second_scan?? */
int first_scan(FILE *file, FILE *writeFile, hashTableInt *table, int *IC, int *DC) {
    int i, j, startParamsIdx;
    /* flag whether to encode currentLine of not */
    int encodeLine = 1;
    /* counting starts from 1 */
    int lineNum = 0;
    /* reading line from file */
    char currentLine[MAX_WORD_LENGTH];
    char copyCurrentLine[MAX_WORD_LENGTH];
    char operationString[MAX_WORD_LENGTH];
    char labelBeforeParams[MAX_WORD_LENGTH];
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
    int isEncodedFirst = 0, isEncodedSecond = 0, isEncodedThird = 0;

    short encodedWord = 0;
    short operationEncode, firstParamEncode, secondParamEncode, thirdEncode, srcEncode, destEncode, externEntryEncode;
    operationEncode = firstParamEncode = secondParamEncode = thirdEncode = srcEncode = destEncode
            = externEntryEncode = 0;

    while (fgets(currentLine, MAX_WORD_LENGTH, file) != NULL) {
        L = 0;
        currentLine[strlen(currentLine)-1] = '\0';
        strcpy(copyCurrentLine, currentLine);
        L = validLine(copyCurrentLine);
        strcpy(copyCurrentLine, currentLine);
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
            for (i = 0; !isspace(currentLine[i]); i++) {
                operationString[i] = currentLine[i];
            }
            operationString[i] = '\0';
            operationEncode = (short) command_code(operationString);

            strcpy(copyCurrentLine, currentLine);
            copyCurrentLine[strlen(copyCurrentLine)] = '\0';

            write_to_ob_file((operationEncode << SHIFTS_FOR_OPCODE), encodedString, lineNum, writeFile);

            i++; /* skip space */

            switch (operationEncode) {
                case MOV_CODE:
                case CMP_CODE:
                case ADD_CODE:
                case SUB_CODE: {
                    token = strtok(&copyCurrentLine[i], " \t\n,#");
                    while (token != NULL) {
                        if (!isEncodedFirst) {
                            if ((firstRegisterIdx = is_register(token))) {
                                /* in first_scan setting labels as ? in .ob file (encode them in 2nd scan) */
                            } else if (is_label(token)) fprintf(writeFile, "%s\n", "?");
                            /* it's a number */
                            else {
                                firstParamEncode = (short) atoi(token);
                                firstParamEncode = firstParamEncode << SHIFTS_FOR_SRC;
                                write_to_ob_file(firstParamEncode, encodedString, lineNum, writeFile);
                                firstParamEncode = 0;
                            }
                            isEncodedFirst = 1;
                        } else if (!isEncodedSecond) {
                            if ((secondRegisterIdx = is_register(token))) {
                                /* need to encode both registers in one word */
                                if (firstRegisterIdx) {
                                    write_to_ob_file((firstRegisterIdx << SHIFTS_FOR_FIRST_REGISTER) |
                                                             (secondRegisterIdx << SHIFTS_FOR_SECOND_REGISTER),
                                                             encodedString, lineNum, writeFile);
                                    firstRegisterIdx = 0;
                                } else {
                                    /* encode 2 separate words */
                                    write_to_ob_file(firstRegisterIdx << SHIFTS_FOR_SRC, encodedString,
                                                     lineNum, writeFile);
                                    write_to_ob_file(secondRegisterIdx << SHIFTS_FOR_SRC, encodedString,
                                                     lineNum, writeFile);
                                }
                                /* in first_scan setting labels as ? in .ob file (encode them in 2nd scan) */
                            } else if (is_label(token)) fprintf(writeFile, "%s\n", "?");
                            /* it's a number */
                            else {
                                secondParamEncode = atoi(token);
                                secondParamEncode = secondParamEncode << SHIFTS_FOR_SRC;
                                write_to_ob_file(secondParamEncode, encodedString, lineNum, writeFile);
                                secondParamEncode = 0;
                            }
                            isEncodedSecond = 1;
                        }
                        firstRegisterIdx = secondRegisterIdx = 0;
                        token = strtok(NULL, " \t\n,#");
                    }
                    break;
                }

                /* same thing but no immediate addressing */
                case LEA_CODE: {
                    token = strtok(&copyCurrentLine[i], " \t\n,");
                    while (token != NULL) {
                        if (!isEncodedFirst) {
                            if ((firstRegisterIdx = is_register(token))) {
                                /* in first_scan setting labels as ? in .ob file (encode them in 2nd scan) */
                            } else if (is_label(token)) fprintf(writeFile, "%s\n", "?");
                            isEncodedFirst = 1;
                        } else if (!isEncodedSecond) {
                            if ((secondRegisterIdx = is_register(token))) {
                                if (firstRegisterIdx) {
                                    write_to_ob_file((firstRegisterIdx << SHIFTS_FOR_FIRST_REGISTER) |
                                                     (secondRegisterIdx << SHIFTS_FOR_SECOND_REGISTER),
                                                     encodedString, lineNum, writeFile);
                                    firstRegisterIdx = 0;
                                } else {
                                    write_to_ob_file(firstRegisterIdx << SHIFTS_FOR_SRC, encodedString,
                                                     lineNum, writeFile);
                                    write_to_ob_file(secondRegisterIdx << SHIFTS_FOR_SRC, encodedString,
                                                     lineNum, writeFile);
                                }
                                /* in first_scan setting labels as ? in .ob file (encode them in 2nd scan) */
                            } else if (is_label(token)) fprintf(writeFile, "%s\n", "?");
                            isEncodedSecond = 1;
                        }
                        firstRegisterIdx = secondRegisterIdx = 0;
                        token = strtok(NULL, " \t\n,");
                    }
                    isEncodedFirst = isEncodedSecond = isEncodedThird = 0;
                    break;
                }

                /* only destination operand instructions (no jmp, bne, jsr) can be registers or label only */
                case NOT_CODE:
                case CLR_CODE:
                case INC_CODE:
                case DEC_CODE:
                case RED_CODE:
                case PRN_CODE: {
                    token = strtok(&copyCurrentLine[i], " \t\n#");
                    while (token != NULL) {
                        if (!isEncodedFirst) {
                            if ((secondRegisterIdx = is_register(token))) {
                                secondRegisterIdx = secondRegisterIdx << SHIFTS_FOR_DEST;
                                write_to_ob_file(secondRegisterIdx, encodedString, lineNum, writeFile);
                                secondRegisterIdx = 0;
                                /* in first_scan setting labels as ? in .ob file (encode them in 2nd scan) */
                            } else if (is_label(token)) fprintf(writeFile, "%s\n", "?");
                            else {
                                if (operationEncode == PRN_CODE) {
                                    /* check immediate addressing */
                                    secondParamEncode = atoi(token);
                                    secondParamEncode = secondParamEncode << SHIFTS_FOR_SRC;
                                    write_to_ob_file(secondParamEncode, encodedString, lineNum, writeFile);
                                    secondParamEncode = 0;
                                }
                            }
                            isEncodedFirst = 1;
                        }
                        token = strtok(NULL, " \t\n#");
                    }
                    isEncodedFirst = isEncodedSecond = isEncodedThird = 0;
                    break;
                }

                /* still no source operand */
                case JMP_CODE:
                case BNE_CODE:
                case JSR_CODE: {
                    /* handle one operand cases */
                    if (!strstr(&copyCurrentLine[i], "(")) {
                        token = strtok(&copyCurrentLine[i], " \t\n");
                        while (token != NULL) {
                            if (!isEncodedFirst) {
                                if ((secondRegisterIdx = is_register(token))) {
                                    secondRegisterIdx = secondRegisterIdx << SHIFTS_FOR_DEST;
                                    write_to_ob_file(secondRegisterIdx, encodedString, lineNum, writeFile);
                                    secondRegisterIdx = 0;
                                    /* in first_scan setting labels as ? in .ob file (encode them in 2nd scan) */
                                } else if (is_label(token)) fprintf(writeFile, "%s\n", "?");
                                isEncodedFirst = 1;
                            }
                            token = strtok(NULL, " \t\n");
                        }
                        isEncodedFirst = isEncodedSecond = isEncodedThird = 0;
                        break;
                    }
                    /* handle two parameters and destination operand */

                    /* finding label/register as destination operand */
                    for (j = i; copyCurrentLine[j] != '('; j++) {
                        labelBeforeParams[j-i] = copyCurrentLine[j];
                    }
                    labelBeforeParams[j-i] = '\0';
                    if ((secondRegisterIdx = is_register(labelBeforeParams))) {
                        secondRegisterIdx = secondRegisterIdx << SHIFTS_FOR_DEST;
                        write_to_ob_file(secondRegisterIdx, encodedString, lineNum, writeFile);
                        secondRegisterIdx = 0;
                        /* it's a label */
                    } else fprintf(writeFile, "%s\n", "?");

                    /* handle parameters */
                    token = strtok(&copyCurrentLine[j], " \t,()");
                    while (token != NULL) {
                        if (!isEncodedFirst) {
                            if ((firstRegisterIdx = is_register(token))) {
                                firstParamEncode = firstRegisterIdx;
                            } else if (is_label(token)) fprintf(writeFile, "%s\n", "?");
                            isEncodedFirst = 1;
                        } else if (!isEncodedSecond) {
                            if ((secondRegisterIdx = is_register(token))) {
                                secondParamEncode = secondRegisterIdx;
                                /* encode 2 registers in 1 line */
                                if (firstRegisterIdx) {
                                    thirdEncode = (firstParamEncode << SHIFTS_FOR_FIRST_REGISTER) |
                                            (secondParamEncode << SHIFTS_FOR_SECOND_REGISTER);
                                    write_to_ob_file(thirdEncode, encodedString, lineNum, writeFile);
                                    thirdEncode = 0;
                                } else {
                                    firstParamEncode = firstParamEncode << SHIFTS_FOR_SRC;
                                    secondParamEncode = secondParamEncode << SHIFTS_FOR_SRC;
                                    write_to_ob_file(firstParamEncode, encodedString, lineNum, writeFile);
                                    write_to_ob_file(secondParamEncode, encodedString, lineNum, writeFile);
                                }
                            } else if (is_label(token)) {
                                /* if first was a register, need to encode it */
                                if (firstRegisterIdx) {
                                    firstParamEncode = firstParamEncode << SHIFTS_FOR_SRC;
                                    convert_to_special_binary(firstParamEncode, encodedString);
                                }
                                /* encode the label */
                                fprintf(writeFile, "%s\n", "?");
                            }
                            isEncodedSecond = 1;
                        }
                        token = strtok(NULL, " \t,()");
                    }
                    break;
                }

                case RTS_CODE:
                case STOP_CODE: {
                    printf("STOP!\n");
                    break;
                }

                default: break;
            }
            isEncodedFirst = isEncodedSecond = isEncodedThird = 0;

            if (maxLabelLength != - 1) memset(labelName, 0, maxLabelLength);
            memset(currentLine, 0, MAX_WORD_LENGTH);
            memset(copyCurrentLine, 0, MAX_WORD_LENGTH);

            (*IC) += L;
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
            for (startParamsIdx = i; currentLine[startParamsIdx] != ' '; startParamsIdx++) {
                storeDataString[startParamsIdx-i] = currentLine[startParamsIdx];
            }
            storeDataString[startParamsIdx-i] = '\0';

            /* insert label into hash table with DC value as value */
            insertLabelReturnCode = insert_int(table, labelName, *DC);

            if (insertLabelReturnCode == HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE) {
                fprintf(stderr, "Label %s already exists!\n", labelName);
                free(labelName);
                return -1; /* TODO: handle errors if label already exists */
            }

            startParamsIdx++; /* skip space */
            /* .data declaration */
            if (strcmp(storeDataString, DATA_STORE_COMMAND) == 0) {
                token = strtok(&copyCurrentLine[startParamsIdx], ", \n");
                while (token != NULL) {
                    encodedWord = (short) atoi(token);
                    encodedWord = encodedWord << 2;
                    write_to_ob_file(encodedWord, encodedString, lineNum, writeFile);

                    (*DC)++;

                    token = strtok(NULL, ", \n");
                }
                /* .string declaration */
            } else if (strcmp(storeDataString, STRING_STORE_COMMAND) == 0) {
                startParamsIdx++;
                /* skipping space between .string and '"' and counting characters until '"' */
                for (j = startParamsIdx; currentLine[j] != '"'; j++) {
                    encodedWord = (short) currentLine[j];
                    encodedWord = encodedWord << 2;
                    write_to_ob_file(encodedWord, encodedString, lineNum, writeFile);
                    (*DC)++;
                }
                convert_to_special_binary(0, encodedString);
                /* writing \0 value at the end */
                fprintf(writeFile, "%s\n", encodedString);
            }

            if (maxLabelLength != -1) memset(labelName, 0, maxLabelLength);
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
                encodedWord = (firstRegisterIdx << SHIFTS_FOR_FIRST_REGISTER) |
                        (secondRegisterIdx << SHIFTS_FOR_SECOND_REGISTER);
                write_to_ob_file((operationEncode << SHIFTS_FOR_OPCODE), encodedString,
                                 lineNum, writeFile);
                write_to_ob_file(encodedWord, encodedString, lineNum, writeFile);
            }

            firstRegisterIdx = secondRegisterIdx = 0;

            (*IC) += L;
            continue;
        }

        /* not a declaration of a .data or .string (incrementing IC) can be a regular command (i.e: LOOP: mov #2, r4) */

        /* TODO: handle extern/entry commands */

        if (maxLabelLength != -1) memset(labelName, 0, maxLabelLength);
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

void write_to_ob_file(int num, char finalString[], int lineNum, FILE *file) {
    convert_to_special_binary(num, finalString);
    fprintf(file, "%d\t%s\n", lineNum, finalString);
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

int is_label(char *string) {
    int i;

    if (strlen(string) == 0) return 0;
    if (!isalpha(string[0])) return 0;

    for (i = 0; string[i] != '\0'; i++) {
        if (!isalpha(string[i]) && !isdigit(string[i])) return 0;
    }
    return 1;
}

int is_register(char *string) {
    int i;
    for (i = 0; i < NUM_REGISTERS; i++) {
        /* returning the register index */
        if (strcmp(REGISTERS_NAMES[i], string) == 0) return i+1;
    }
    return 0;
}
