#include "pre_assembler.h"

int deploy_macros(int argc, char *argv[]) {
    int numOfFiles = argc - 1;
    int i, j;
    int longestMacroBodyLength = -1;
    /* files received from program arguments */
    FILE **readFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);
    /* files to write and deploy the macros read from readFiles */
    FILE **writeFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);
    hashTable **tables = (hashTable**) malloc(sizeof(hashTable*) * numOfFiles);

    char *currentFileNameWrite;
    char *currentFileName;
    int currentFileNameLength;

    const char filePostfix[] = ".as";
    const char fileWritePostfix[] = ".am";

    /* no files specified */
    if (argc == 1) return 0;

    /* storing all files in readFiles array */
    for (i = 1; i < argc; i++) {
        currentFileNameLength = strlen(argv[i]);
        currentFileName = (char *) malloc(currentFileNameLength + 4); /* adding 4 for .as postfix */
        currentFileNameWrite = (char*) malloc(currentFileNameLength + 4); /* adding 6 as adding .am postfix */

        if (currentFileName == NULL || currentFileNameWrite == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }

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
            return MAIN_ERROR_CODE;
        }
        /* trying to open file to write to with all macros */
        if ((writeFiles[i-1] = fopen(currentFileNameWrite, WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", currentFileNameWrite);
            free(currentFileNameWrite);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(writeFiles[j]);
            free(writeFiles);
            return MAIN_ERROR_CODE;
        }

        /* initializing table in memory for current file */
        tables[i-1] = (hashTable*) malloc(sizeof(hashTable));

        if (!init_hash_table(tables[i-1], HASH_TABLE_SIZE)) {
            fprintf(stderr, "unsuccessful initialization of hash table.\n");
            return MAIN_ERROR_CODE;
        }

        /* finished deploying macros on this file */
        free(currentFileName);
        free(currentFileNameWrite);
    }

    for (i = 0; i < numOfFiles; i++) {
        calculate_longest_macro_body(readFiles[i], &longestMacroBodyLength);
        /* going back to start of readFile to reiterate it for writing */
        rewind(readFiles[i]);
        if (!read_macros_from_file(readFiles[i], tables[i], &longestMacroBodyLength)) return MAIN_ERROR_CODE;
        /* going back to start of readFile to reiterate it for writing */
        rewind(readFiles[i]);
        write_macros_to_file(readFiles[i], writeFiles[i], tables[i]);
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
    return 1;
}

int read_macros_from_file(FILE* file, hashTable *table, int *longestMacroBody) {
    int i, j, start, end;
    int maxMacroBodyLength = *longestMacroBody, endMacro = 1;
    int macroNameMaxLength = -1;
    int tokenLength, currentLineLength = -1, insertResultCode;
    char *token;
    char *macroName = NULL;
    char macroBody[maxMacroBodyLength];
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
                    free(macroName);
                    return MACRO_ALREADY_DEFINED_ERROR_CODE;
                } /* TODO: add more errors */

                memset(macroName, 0, macroNameMaxLength);
                memset(macroBody, 0, maxMacroBodyLength);
                memset(currentLine, 0, MAX_WORD_LENGTH);
                memset(tempLine, 0, MAX_WORD_LENGTH);
                memset(cutCurrentLine, 0, MAX_WORD_LENGTH);
                endMacro = 1;
                continue;
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

    return 1;
}

int write_macros_to_file(FILE *readFile, FILE *writeFile, hashTable *table) {
    int j, k, cutIdx;
    char cutWord[MAX_WORD_LENGTH];
    char shortenedCutWord[MAX_WORD_LENGTH];
    char word[MAX_WORD_LENGTH];
    char tempWord[MAX_WORD_LENGTH];
    char *token;
    int firstInLine = 1, deployedMacro = 0, enteredMacro = 0;

    enum macroState macroStatus;

    macroStatus = NOT_IN_MACRO;

    while (fgets(word, MAX_WORD_LENGTH, readFile) != NULL) {
        j = 0;
        k = strlen(word);
        while (isspace(word[j])) j++;
        if (j == k) continue;
        /* assuming comments are at beginning of line */
        if (word[j] == ';') continue;

        while (isspace(word[k])) k--;

        for (cutIdx = j; cutIdx < k; cutIdx++) cutWord[cutIdx-j] = word[cutIdx];

        /* encountered a comment */
        if (strstr(cutWord, ";")) {
            /* reset word and cutWord */
            memset(cutWord, 0, MAX_WORD_LENGTH);
            memset(word, 0, MAX_WORD_LENGTH);
            memset(shortenedCutWord, 0, MAX_WORD_LENGTH);
            memset(tempWord, 0, MAX_WORD_LENGTH);
            continue;
        }

        /* if finished with macro deployment OR haven't found one yet */
        if (macroStatus == NOT_IN_MACRO) {
            /* getting rid of \n character */
            cutWord[strlen(cutWord)-1] = '\0';
            strcpy(tempWord, cutWord);

            token = strtok(tempWord, " \t");
            /* traversing every word in sentence to check for macro deployments */
            while (token != NULL) {
                /* if reached a macro declaration */
                if (strcmp(token, MACRO_KEYWORD) == 0) {
                    macroStatus = IN_MACRO_BODY;
                    enteredMacro = 1;
                    break;
                }
                /* checking if macro is supposed to be deployed here */
                if (contains_key(table, token)) {
                    if (!firstInLine) fprintf(writeFile, "%s", " ");
                    fprintf(writeFile, "%s", get_value(table, token));
                    deployedMacro = 1;
                } else {
                    if (firstInLine) {
                        fprintf(writeFile, "%s", token);
                        firstInLine = 0;
                    } else {
                        fprintf(writeFile, " %s", token);
                    }
                }

                token = strtok(NULL, " \t");
            }

            if (!deployedMacro && !enteredMacro) {
                fprintf(writeFile, "%s", "\n");
                enteredMacro = 0;
            } else deployedMacro = 0;

            firstInLine = 1;

            /* reset word and cutWord */
            memset(cutWord, 0, MAX_WORD_LENGTH);
            memset(word, 0, MAX_WORD_LENGTH);
            memset(shortenedCutWord, 0, MAX_WORD_LENGTH);
            memset(tempWord, 0, MAX_WORD_LENGTH);
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
                    enteredMacro = 0;
                }
            }
        }

        /* reset word and cutWord */
        memset(cutWord, 0, MAX_WORD_LENGTH);
        memset(word, 0, MAX_WORD_LENGTH);
        memset(shortenedCutWord, 0, MAX_WORD_LENGTH);
        memset(tempWord, 0, MAX_WORD_LENGTH);
    }

    return 1;
}

void calculate_longest_macro_body(FILE *file, int *longest) {
    int i, j, start, end;
    int currentMacroBodyLength = 0, endMacro = 1;
    int currentLineLength;
    char *token;
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
                if (strcmp(token, MACRO_KEYWORD) == 0) {
                    macroStatus = IN_MACRO_NAME;
                    token = strtok(NULL, " \t");
                    continue;
                }
                /* reading macro name and changing to macro body state */
                if (macroStatus == IN_MACRO_NAME) {
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
                memset(currentLine, 0, MAX_WORD_LENGTH);
                memset(tempLine, 0, MAX_WORD_LENGTH);
                memset(cutCurrentLine, 0, MAX_WORD_LENGTH);
                currentMacroBodyLength = 0;
                endMacro = 1;
                continue;
            }

            currentMacroBodyLength += currentLineLength;

            if (*longest < currentMacroBodyLength) {
                *longest = currentMacroBodyLength;
            }
        }

        memset(currentLine, 0, MAX_WORD_LENGTH);
        memset(tempLine, 0, MAX_WORD_LENGTH);
        memset(cutCurrentLine, 0, MAX_WORD_LENGTH);
        endMacro = 1;
    }
}
