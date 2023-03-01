#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pre_assembler.h"

void print_table(hashTable *table) {
    int i, step;
    hashTableItem *current;
    for (i = 0; i < table->size; i++) {
        if (table->items[i]) {
            current = table->items[i];
            step = 0;
            printf("i: %d\n", i);
            while (current) {
                printf("step: %d\n", step);
                printf("key (%lu): %s\n", strlen(current->key), current->key);
                printf("value:\n%s\n", current->value);
                current = current->next;
                step++;
            }
            printf("\n");
        }
    }
    printf("\n");
}

int test_insert(hashTable *table) {
    char k;
    char key[6] = "key";
    char *value = "value1";
    for (k = 0; k < 15; k++) {
        if (k <= 9) {
            key[3] = k + 48;
            key[4] = '\0';
            key[5] = '\0';
        } else {
            key[3] = k/10 + 48;
            key[4] = k%10 + 48;
            key[5] = '\0';
        }
        insert(table, key, value);
    }

    print_table(table);

    return 0;
}

int test_change_value(hashTable *table) {
    char key[] = "key14";
    char value[] = "changed";
    if (change_value(table, key, value)) print_table(table);

    return 0;
}

int read_from_file(FILE* file, hashTable *table) {
    int i, j, start, end;
    int currentMacroBodyLength = 0, endMacro = 1;
    int macroBodyMaxLength = -1, macroNameMaxLength = -1;
    int tokenLength, currentLineLength = -1, insertResultCode;
    char *token;
    char *macroName = NULL, *macroBody = NULL;
    enum macroState macroStatus;
    char currentLine[MAX_WORD_LENGTH];
    char tempLine[MAX_WORD_LENGTH];
    char cutCurrentLine[MAX_WORD_LENGTH];

    macroStatus = NOT_IN_MACRO;

    while (fgets(currentLine, MAX_WORD_LENGTH, file) != NULL) {
        currentLineLength = strlen(currentLine);
        start = 0, end = currentLineLength-1;
        for (; isspace(currentLine[start]); start++);
        for (; isspace(currentLine[end]); end--);
        if (start >= end) continue;
        for (i = start; i <= end; i++) cutCurrentLine[i-start] = currentLine[i];

        if (strcmp(currentLine, "\n") == 0) continue;

        if (macroStatus == NOT_IN_MACRO) {
            strcpy(tempLine, currentLine);
            token = strtok(tempLine, " \t");

            while (token != NULL) {
                tokenLength = strlen(token);
                if (strcmp(token, MACRO_KEYWORD) == 0) {
                    macroStatus = IN_MACRO_NAME;
                    token = strtok(NULL, " \t");
                    continue;
                }
                /* reading macro name and changing to macro body state */
                if (macroStatus == IN_MACRO_NAME) {
                    if (macroNameMaxLength < tokenLength) {
                        macroNameMaxLength = tokenLength;
                        if (macroName == NULL) macroName = (char *) malloc(macroNameMaxLength);
                        else macroName = (char *) realloc(macroName, macroNameMaxLength);
                    }
                    /* setting macroName */
                    strcpy(macroName, token);
                    macroStatus = IN_MACRO_BODY;
                }
                token = strtok(NULL, " \t");
            }

            memset(currentLine, 0, MAX_WORD_LENGTH);
            memset(tempLine, 0, MAX_WORD_LENGTH);
            memset(cutCurrentLine, 0, MAX_WORD_LENGTH);
            continue;
        }

        /* reading macro body */
        if (macroStatus == IN_MACRO_BODY) {
            if (strlen(cutCurrentLine) != strlen(END_MACRO_KEYWORD)) endMacro = 0;
            else {
                for (j = 0; j < end-start+1; j++) {
                    if (cutCurrentLine[j] != END_MACRO_KEYWORD[j]) endMacro = 0;
                }
            }
            if (endMacro) {
                macroStatus = NOT_IN_MACRO;
                macroName[strlen(macroName)-1] = '\0';
                insertResultCode = insert(table, macroName, macroBody);
                /* error handling */
                if (insertResultCode == HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE) {
                    fprintf(stderr, "Macro %s:\n%s",
                            macroName, MACRO_ALREADY_EXISTS_ERROR_MESSAGE);
                    free(macroBody);
                    free(macroName);
                    return MACRO_ALREADY_DEFINED_ERROR_CODE;
                } /* TODO: add more errors */

                memset(macroName, 0, macroNameMaxLength);
                memset(macroBody, 0, macroBodyMaxLength);
                memset(currentLine, 0, MAX_WORD_LENGTH);
                memset(tempLine, 0, MAX_WORD_LENGTH);
                memset(cutCurrentLine, 0, MAX_WORD_LENGTH);
                currentMacroBodyLength = 0;
                endMacro = 1;
                continue;
            }

            currentMacroBodyLength += currentLineLength;

            if (macroBodyMaxLength < currentMacroBodyLength) {
                macroBodyMaxLength = currentMacroBodyLength;
                /* if uninitialized */
                if (macroBody == NULL) macroBody = (char *) malloc(macroBodyMaxLength);
                /* needs to reallocate */
                else macroBody = (char *) realloc(macroBody, macroBodyMaxLength);
            }

            cutCurrentLine[i-1] = '\n';
            strcat(macroBody, cutCurrentLine);
        }

        memset(currentLine, 0, MAX_WORD_LENGTH);
        memset(tempLine, 0, MAX_WORD_LENGTH);
        memset(cutCurrentLine, 0, MAX_WORD_LENGTH);
        endMacro = 1;
    }

    free(macroName);
    free(macroBody);

    return 1;
}

void test_read_from_file() {
    FILE *file = fopen("../x.as", "r");
    hashTable *table = (hashTable *) malloc(sizeof(hashTable));
    init_hash_table(table, HASH_TABLE_SIZE/10);

    read_from_file(file, table);

    print_table(table);
}

int main() {
    hashTable *table = (hashTable*) malloc(sizeof(hashTable));
    init_hash_table(table, 10);

    test_insert(table);

    test_change_value(table);

    printf("--------------------------------\n");
    test_read_from_file();

    return 0;
}
