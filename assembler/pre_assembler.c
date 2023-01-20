#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

#define MAX_WORD_LENGTH 81
#define HASH_TABLE_SIZE 10000
#define READ_MODE "r"

enum macroState {
    NOT_IN_MACRO, IN_MACRO_BODY, IN_MACRO_NAME, END_MACRO
};

int main(int argc, char *argv[]) {
    int numOfFiles = argc - 1;
    FILE** files = (FILE**) malloc(sizeof(FILE*) * (numOfFiles));
    hashTable *table;
    char *currentFileName;
    int currentFileNameLength;
    int i, j;
    const char filePostfix[] = ".as";
    char *word;
    int wordLength = 0;
    const char macroKeyword[] = "mcr";
    const char endMacroKeyword[] = "endmcr";
    char *macroName;
    char *macroBody;
    char *token;
    int currentMacroBodyLength = 0;
    int insertReturnCode = 0;

    if (argc == 1) return 1;

    for (i = 1; i < argc; i++) {
        currentFileNameLength = strlen(argv[i]);
        currentFileName = (char *) malloc(sizeof(char) * (currentFileNameLength + 4)); /* adding 4 for .as postfix */

        strcpy(currentFileName, argv[i]);
        /* adding ".as" postfix */
        strcat(currentFileName, filePostfix);

        printf("File name: %s\n\n", currentFileName);
        if ((files[i-1] = fopen(currentFileName, READ_MODE)) == NULL) {
            fprintf(stderr, "Compilation error: bluh bluh bluh\n");
            free(currentFileName);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(files[j]);
            free(files);
            return 1;
        }
        /* TODO: create new file with macros deployed */

        /* finished deploying macros on this file */
        free(currentFileName);
    }

    /* initializing current word to read to from files */
    word = (char *) malloc(sizeof(char) * MAX_WORD_LENGTH);
    /* initializing table in memory */
    table = (hashTable *) malloc(sizeof(hashTable));

    if (!init_hash_table(table, HASH_TABLE_SIZE)) {
        printf("unsuccessful initialization of hash table.\n");
        return 1;
    }
    for (i = 0; i < numOfFiles; i++) {
        enum macroState macroStatus = NOT_IN_MACRO;
        while (fgets(word, MAX_WORD_LENGTH, files[i]) != NULL) {
            j = 0;
            while (isspace(word[j]))
                j++;
            wordLength = strlen(&word[j]);
            /* ignoring \n character at the end */
            word[wordLength+j-1] = '\0';

            switch (macroStatus) {
                case NOT_IN_MACRO: {
                    /* traversing words in word */
                    token = strtok(word, " \t");
                    while (token != NULL) {
                        if (macroStatus == IN_MACRO_NAME) {
                            macroName = (char*) malloc(sizeof(char) * strlen(token));
                            strcpy(macroName, token);
                            /* found "mcr" keyword */
                        } else if (strcmp(token, macroKeyword) == 0) {
                            macroStatus = IN_MACRO_NAME;
                        }
                        token = strtok(NULL, " \t");
                    }
                    macroStatus = IN_MACRO_BODY;
                    break;
                }

                case END_MACRO: {
                    macroStatus = NOT_IN_MACRO;
                    currentMacroBodyLength = 0;
                    /* trying to insert macroBody into macroName key */
                    insertReturnCode = insert(table, macroName, macroBody);

                    if (insertReturnCode == HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE) {
                        /* TODO: raise error of multiple macros with same name */
                        printf(MACRO_ALREADY_EXISTS_ERROR_MESSAGE);
                        printf("Macro %s already exists!!\nChanging value...\n", macroName);
                        break;
                    }

                    printf("table item key (%s) hash:\n%d\ntable item value:\n%s\n\n", macroName,
                           calculate_hash(macroName, table->size), get_value(table, macroName));

                    /* deallocating memory for other macro names and bodies */
                    free(macroName);
                    free(macroBody);
                    break;
                }
                /* reading the macro body and adding it to the item with the "macroName" key in the hash table */
                case IN_MACRO_BODY: {
                    if (currentMacroBodyLength == 0) {
                        macroBody = (char *) malloc(sizeof(char) * (wordLength + 1));
                        currentMacroBodyLength += wordLength + 1;
                        /* adding \n character as macro body should consist of several (or just 1) lines */
                        word[wordLength+j-1] = '\n';
                        strcpy(macroBody, &word[j]);
                    } else {
                        if (strcmp(&word[j], endMacroKeyword) == 0) {
                            macroStatus = END_MACRO;
                            break;
                        }
                        currentMacroBodyLength += wordLength;
                        macroBody = (char*) realloc(macroBody, sizeof(char) * (currentMacroBodyLength + 1));
                        /* adding \n character as "deleted" it after skipped whitespaces of sentence */
                        word[wordLength+j-1] = '\n';
                        strcat(macroBody, &word[j]);
                    }
                    break;
                }

                /* default case should be IN_MACRO_NAME which is handled in NOT_IN_MACRO case while loop */
                default: {
                    break;
                }
            }
        }
    }
    return 0;
}


