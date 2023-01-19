#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

#define MAX_WORD_LENGTH 81
#define IN_MACRO_BODY 2
#define IN_MACRO_NAME 1
#define HASH_TABLE_SIZE 10000
#define READ_MODE "r"

int main(int argc, char *argv[]) {
    int numOfFiles = argc - 1;
    FILE* files[numOfFiles];
    FILE *file;
    hashTable* table;
    char *currentFileName;
    int currentFileNameLength;
    int i, j, macroIdx, tempIdx;
    char *tempString;
    const char filePostfix[] = ".as";
    char *word;
    int wordLength = 0;
    const char macroKeyword[] = "mcr";
    const char endMacroKeyword[] = "endmcr\n";
    char *macroName;
    char *macroBody;
    int currentMacroBodyIdx = 0;
    int inMacro = 0;
    int macroNameLen = 0;
    if (argc == 1) return 1;

    for (i = 1; i < argc; i++) {
        currentFileNameLength = strlen(argv[i]);
        currentFileName = (char *) malloc(sizeof(char) * (currentFileNameLength + 4));

        strcpy(currentFileName, argv[i]);
        /* adding ".as" postfix */
        strcpy(&currentFileName[currentFileNameLength], filePostfix);

        printf("File name: %s\n\n", currentFileName);
        if ((file = fopen(currentFileName, READ_MODE)) == NULL) {
            fprintf(stderr, "Compilation error: bluh bluh bluh\n");
            free(currentFileName);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(files[j]);
            return 1;
        }

        free(currentFileName);
    }

    /* initializing current word to read to from files */
    word = (char*) malloc(sizeof(char) * MAX_WORD_LENGTH);
    /* initializing table in memory */
    table = (hashTable*) malloc(sizeof(hashTable));

    if (!init_hash_table(table, HASH_TABLE_SIZE)) {
        printf("unsuccessful initialization of hash table.\n");
        return 1;
    }

    for (i = 0; i < numOfFiles; i++) {
        while (fgets(word, MAX_WORD_LENGTH, file) != NULL) {
            j = 0;
            while(isspace(word[j]))
                j++;
            wordLength = strlen(&word[j]);
            if (!inMacro) {
                /* finding next space in word after first space */
                for (macroIdx = j; !isspace(word[macroIdx]); macroIdx++);
                /* defining tempString to store substring from first to second space */
                tempString = (char*) malloc(sizeof(char) * (macroIdx-j+1));
                /* setting characters in it */
                for (tempIdx = j; tempIdx < macroIdx; tempIdx++) {
                    tempString[tempIdx-j] = word[tempIdx];
                }
                macroIdx++;
                /* if it's mcr, starting a macro definition */
                if (strcmp(tempString, macroKeyword) == 0) {
                    word[strlen(word)-1] = '\0';
                    inMacro = IN_MACRO_BODY;
                    free(tempString);
                    /* skip whitespaces between 'mcr' and the macro name */
                    while(isspace(word[macroIdx]))
                        macroIdx++;
                    /* count the length of the macro name */
                    for(j = macroIdx; j < wordLength; j++){
                        macroNameLen++;
                    }
                    /* set macroName */
                    macroName = (char*) malloc(sizeof(char) * macroNameLen);
                    memcpy(macroName,&word[macroIdx],macroNameLen);
                    continue;
                }
                free(tempString);
            }


            /* finished reading macro body, adding it to hash table key "macroName". exiting macro reading modes */
            if (strcmp(&word[j], endMacroKeyword) == 0) {
                inMacro = 0;
                currentMacroBodyIdx = 0;
                /* insert macroBody into macroName key */
                if (!insert(table, macroName, macroBody)) printf("Macro %s already exists!!\n\n", macroName); /* TODO: handle error of redefinition of macro / collisions */
                printf("table item key (%s) hash:\n%d\ntable item value:\n%s\n\n", macroName, calculate_hash(macroName, table->size), get_value(table, macroName));
                free(macroName);
                free(macroBody);
                continue;
            }

            /* reading the macro body and adding it to the item with the "macroName" key in the hash table */
            if (inMacro == IN_MACRO_BODY) {
                if(currentMacroBodyIdx == 0){
                    macroBody = (char*) malloc(sizeof(char) * (wordLength+1));
                    currentMacroBodyIdx += wordLength;
                    strcpy(macroBody,&word[j]);
                } else {
                    currentMacroBodyIdx += wordLength;
                    macroBody = (char*) realloc(macroBody,sizeof(char) * (currentMacroBodyIdx + 1));
                    strcat(macroBody,&word[j]);
                }
            }
             /*
            if (word.isIn(dataStructure)) {
                write to new file macro data
            }*/

        }
    }
    return 0;
}


