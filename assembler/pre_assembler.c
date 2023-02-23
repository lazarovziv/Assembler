#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "constants.h"

#define MAX_WORD_LENGTH 81
#define HASH_TABLE_SIZE 10000

#define READ_MODE "r"
#define WRITE_MODE "w"
#define MACRO_KEYWORD "mcr"
#define END_MACRO_KEYWORD "endmcr"

enum macroState {
    NOT_IN_MACRO, IN_MACRO_BODY, IN_MACRO_NAME, END_MACRO
};

int read_macros_from_file(FILE *file, hashTable *table);
int write_macros_to_file(FILE *readFile, FILE *writeFile, hashTable *table);

int main(int argc, char *argv[]) {
    int numOfFiles = argc - 1;
    int i, j;
    /* files received from program arguments */
    FILE** readFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);
    /* files to write and deploy the macros read from readFiles */
    FILE** writeFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);
    hashTable **tables = (hashTable**) malloc(sizeof(hashTable*) * numOfFiles);

    char *currentFileNameWrite;
    char *currentFileName;
    int currentFileNameLength;

    const char filePostfix[] = ".as";
    const char fileWritePostfix[] = ".am";

    /* no files specified */
    if (argc == 1) return 1;

    /* storing all files in readFiles array */
    for (i = 1; i < argc; i++) {
        currentFileNameLength = strlen(argv[i]);
        currentFileName = (char *) malloc(sizeof(char) * (currentFileNameLength + 4)); /* adding 4 for .as postfix */
        currentFileNameWrite = (char*) malloc(sizeof(char) * (currentFileNameLength + 4)); /* adding 6 as adding .am postfix */

        strcpy(currentFileName, argv[i]);
        /* adding ".as" postfix */
        strcat(currentFileName, filePostfix);

        strcpy(currentFileNameWrite, argv[i]);
        /* adding "_w.as" postfix */
        strcat(currentFileNameWrite, fileWritePostfix);

        /* trying to open file to read macros from */
        if ((readFiles[i-1] = fopen(currentFileName, READ_MODE)) == NULL) {
            fprintf(stderr, "Error trying to open file %s\n", currentFileName);
            free(currentFileName);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(readFiles[j]);
            free(readFiles);
            return 1;
        }
        /* trying to open file to write to with all macros */
        if ((writeFiles[i-1] = fopen(currentFileNameWrite, WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", currentFileNameWrite);
            free(currentFileNameWrite);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(writeFiles[j]);
            free(writeFiles);
            return 1;
        }
        /* initializing table in memory for current file */
        tables[i-1] = (hashTable*) malloc(sizeof(hashTable));

        if (!init_hash_table(tables[i-1], HASH_TABLE_SIZE)) {
            fprintf(stderr, "unsuccessful initialization of hash table.\n");
            return 1;
        }

        /* finished deploying macros on this file */
        free(currentFileName);
        free(currentFileNameWrite);
    }
    for (i = 0; i < numOfFiles; i++) {
        printf("READ %d\n", i);
        if (!read_macros_from_file(readFiles[i], tables[i])) return MAIN_ERROR_CODE;
        /* going back to start of readFile to reiterate it for writing */
        rewind(readFiles[i]);
        if (!write_macros_to_file(
                readFiles[i], writeFiles[i], tables[i])) return MAIN_ERROR_CODE;
    }

    /* freeing all file and hash tables separately */
    for (i = 0; i < numOfFiles; i++) {
        fclose(readFiles[i]);
        fclose(writeFiles[i]);

        free(tables[i]);
    }

    /* freeing arrays */
    free(readFiles);
    free(writeFiles);
    free(tables);
    return 0;
}

int read_macros_from_file(FILE *file, hashTable *table) {
    int j, currentMacroBodyLength = 0, insertReturnCode, wordLength;
    int atFirstMacro = 1, atFirstMacroBody = 1;
    int macroLength = 0, tokenLength;
    char *token;
    char *macroName, *macroBody;
    char *word = (char*) malloc(sizeof(char) * MAX_WORD_LENGTH);

    enum macroState macroStatus = NOT_IN_MACRO;
    /* traversing file for finding macros */
    while (fgets(word, MAX_WORD_LENGTH, file) != NULL) {
        j = 0;
        while (isspace(word[j]))
            j++;
        if (word[j] == ';') continue;
        wordLength = strlen(&word[j]);
        /* ignoring \n character at the end */
        word[wordLength+j-1] = '\0';

        switch (macroStatus) {
            case NOT_IN_MACRO: {
                /* traversing words in word */
                token = strtok(word, " \t");
                while (token != NULL) {
                    tokenLength = strlen(token);
                    if (macroStatus == IN_MACRO_NAME) {
                        /* allocating memory for the first time */
                        if (atFirstMacro) {
                            macroName = (char*) malloc(sizeof(char) * tokenLength);
                            macroLength = tokenLength;
                            atFirstMacro = 0;
                            /* reallocate memory if length of current macro name is longer */
                        } else if (tokenLength > macroLength) {
                            macroName = (char*) realloc(macroName, sizeof(char) * tokenLength);
                            macroLength = tokenLength;
                        }
                        /* reset macroName */
                        memset(macroName, 0, sizeof(char) * macroLength);
                        /* set new value to macroName */
                        strcpy(macroName, token);
                        /* found "mcr" keyword */
                    } else if (strcmp(token, MACRO_KEYWORD) == 0) {
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
                    fprintf(stderr, "Macro %s already exists!\n%s\n",
                            macroName, MACRO_ALREADY_EXISTS_ERROR_MESSAGE);
                    return MACRO_ALREADY_DEFINED_ERROR_CODE;
                }
                break;
            }
                /* reading the macro body and adding it to the item with the "macroName" key in the hash table */
            case IN_MACRO_BODY: {
                if (currentMacroBodyLength == 0) {
                    if (atFirstMacroBody) {
                        macroBody = (char *) malloc(sizeof(char) * (wordLength + 1));
                        atFirstMacroBody = 0;
                    }
                    currentMacroBodyLength += wordLength;
                    /* adding \n character as macro body should consist of several (or just 1) lines */
                    word[wordLength+j-1] = '\n';
                    strcpy(macroBody, &word[j]);
                } else {
                    if (strcmp(&word[j], END_MACRO_KEYWORD) == 0) {
                        macroStatus = END_MACRO;
                        break;
                    }
                    currentMacroBodyLength += wordLength;
                    macroBody = (char*) realloc(macroBody, sizeof(char) * (currentMacroBodyLength));
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
        memset(word, 0, MAX_WORD_LENGTH);
    }
    /* deallocating memory */
    free(word);
    free(macroName);
    free(macroBody);

    for (j = 0; j < table->size; j++) {
        if (table->items[j]) {
            printf("key: %s\nvalue:\n%s\n", table->items[j]->key, table->items[j]->value);
        }
    }

    return 1;
}

int write_macros_to_file(FILE *readFile, FILE *writeFile, hashTable *table) {
    int j, k, cutIdx;
    char *cutWord = (char*) malloc(sizeof(char) * MAX_WORD_LENGTH);
    char *shortenedCutWord = (char*) malloc(sizeof(char) * MAX_WORD_LENGTH);
    char *word = (char*) malloc(sizeof(char) * MAX_WORD_LENGTH);

    enum macroState macroStatus = NOT_IN_MACRO;

    while (fgets(word, MAX_WORD_LENGTH, readFile) != NULL) {
        j = 0;
        k = strlen(word);
        while (isspace(word[j])) j++;
        if (j == k) continue;
        if (word[j] == ';') continue;

        while (isspace(word[k])) k--;

        for (cutIdx = j; cutIdx < k; cutIdx++) cutWord[cutIdx-j] = word[cutIdx];

        if (strstr(cutWord, ";")) {
            /* reset word and cutWord */
            memset(cutWord, 0, MAX_WORD_LENGTH);
            memset(word, 0, MAX_WORD_LENGTH);
            memset(shortenedCutWord, 0, MAX_WORD_LENGTH);
            continue;
        }

        /* if finished with macro deployment OR haven't found one yet */
        if (macroStatus == NOT_IN_MACRO) {
            /* check if macro is declared here */
            if (strstr(cutWord, MACRO_KEYWORD) != NULL
            && strstr(cutWord, " ") != NULL) {
                macroStatus = IN_MACRO_BODY;
                /* reset word and cutWord */
                memset(cutWord, 0, MAX_WORD_LENGTH);
                memset(word, 0, MAX_WORD_LENGTH);
                memset(shortenedCutWord, 0, MAX_WORD_LENGTH);
                continue;
            }

            cutWord[strlen(cutWord)-1] = '\0';

            /* if macro should be deployed here */
            if (contains_key(table, cutWord)) {
                /* setting value of key macroName */
                fprintf(writeFile, "%s", get_value(table, cutWord));
                /* no macro declared, a normal command */
            } else {
                cutWord[strlen(cutWord)] = '\n';
                fprintf(writeFile, "%s", cutWord);
            }

            /* reset word and cutWord */
            memset(cutWord, 0, MAX_WORD_LENGTH);
            memset(word, 0, MAX_WORD_LENGTH);
            memset(shortenedCutWord, 0, MAX_WORD_LENGTH);
            continue;
        }

        /* found a macro declaration previously, should ignore it's body */
        if (macroStatus == IN_MACRO_BODY) {
            /* if reached end of macro declaration, call it */
            if (strstr(cutWord, END_MACRO_KEYWORD) != NULL) {
                memcpy(shortenedCutWord, cutWord, strlen(cutWord)-1);
                if (strcmp(shortenedCutWord, END_MACRO_KEYWORD) == 0) {
                    /* "got out" of macro body */
                    macroStatus = NOT_IN_MACRO;
                }
            }
        }

        /* reset word and cutWord */
        memset(cutWord, 0, MAX_WORD_LENGTH);
        memset(word, 0, MAX_WORD_LENGTH);
        memset(shortenedCutWord, 0, MAX_WORD_LENGTH);
    }

    free(word);
    free(cutWord);

    return 1;
}


