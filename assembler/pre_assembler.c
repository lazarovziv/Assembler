#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

#define MAX_WORD_LENGTH 81
#define HASH_TABLE_SIZE 10000
#define READ_MODE "r"
#define WRITE_MODE "w"

enum macroState {
    NOT_IN_MACRO, IN_MACRO_BODY, IN_MACRO_NAME, END_MACRO
};

int main(int argc, char *argv[]) {
    int numOfFiles = argc - 1;
    /* files received from program arguments */
    FILE** readFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);
    /* files to write and deploy the macros read from readFiles */
    FILE** writeFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);

    hashTable *table;

    char *currentFileNameWrite;
    char *currentFileName;
    int currentFileNameLength;

    int i, j, k, cutIdx;
    const char filePostfix[] = ".as";
    const char fileWritePostfix[] = "_w.as";
    /* using for reading from files */
    char *word;
    char *cutWord;
    int wordLength = 0;
    const char macroKeyword[] = "mcr";
    const char endMacroKeyword[] = "endmcr";
    /* storing it for key, value pair in hash table */
    char *macroName;
    char *macroBody;
    /* token used for strtok call in reading from file loop */
    char *token;

    int currentMacroBodyLength = 0;
    /* indication whether inserting to hash table was successful or not */
    int insertReturnCode = 0;

    /* no files specified */
    if (argc == 1) return 1;

    /* storing all files in readFiles array */
    for (i = 1; i < argc; i++) {
        currentFileNameLength = strlen(argv[i]);
        currentFileName = (char *) malloc(sizeof(char) * (currentFileNameLength + 4)); /* adding 4 for .as postfix */
        currentFileNameWrite = (char*) malloc(sizeof(char) * (currentFileNameLength + 6)); /* adding 6 as adding _w.as as postfix */

        strcpy(currentFileName, argv[i]);
        /* adding ".as" postfix */
        strcat(currentFileName, filePostfix);

        strcpy(currentFileNameWrite, argv[i]);
        /* adding "_w.as" postfix */
        strcat(currentFileNameWrite, fileWritePostfix);

        printf("File name: %s\n\n", currentFileName);
        if ((readFiles[i-1] = fopen(currentFileName, READ_MODE)) == NULL) {
            fprintf(stderr, "Error trying to open file %s\n", currentFileName);
            free(currentFileName);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(readFiles[j]);
            free(readFiles);
            return 1;
        }
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
        /* traversing file for finding macros */
        while (fgets(word, MAX_WORD_LENGTH, readFiles[i]) != NULL) {
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

        /* trying to open file to write to with all macros */
        if ((writeFiles[i] = fopen(currentFileNameWrite, WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", currentFileNameWrite);
            free(currentFileNameWrite);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(writeFiles[j]);
            free(writeFiles);
            return 1;
        }

        /* going back to start of readFile to reiterate it for writing */
        rewind(readFiles[i]);

        /* traversing file again for deploying macros */
        while (fgets(word, MAX_WORD_LENGTH, readFiles[i]) != NULL) {
            j = 0;
            k = strlen(word);
            while (isspace(word[j])) j++;

            /* if entire sentence is whitespaces, user must've spaced his written code */
            if (j == k) {
                continue;
            }
            /* cutting whitespaces from the end */
            while (isspace(word[k])) k--;
            cutWord = (char*) malloc(sizeof(char) * (k-j+1));
            for (cutIdx = j; cutIdx < k; cutIdx++) cutWord[cutIdx-j] = word[cutIdx];

            /* declaration of a macro */
            if (contains_key(table, cutWord)) {
                /* setting value of key macroName */
                fprintf(writeFiles[i], "%s", get_value(table, cutWord));
                /* setting rest of file "as is" */
            } else {
                /* assuming macro declaration is a saved keyword so declaring it is a sentence of its own */
                if (strstr(cutWord, macroKeyword) || strstr(cutWord, endMacroKeyword)) {
                    continue;
                } else fprintf(writeFiles[i], "%s", cutWord);
            }
        }
    }
    return 0;
}


