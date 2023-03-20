#include "pre_assembler.h"
#include "encoder.h"

int main(int argc, char *argv[]) {
    FILE **readFiles, **writeFiles, **postDeployReadFiles, **preScanFiles, **finalFiles;
    hashTable **macrosTables;
    hashTableInt **labelsTables, **entriesTables, **externsTables;
    int i, j, lastFileIndex = 0;
    int IC = 100, DC = 0;
    int longestMacroBodyLength = -1;

    char **preDeployFileNames, **postDeployFileNames, **preScanFileNames, **finalFileNames;
    int currentFileNameLength;

    const char filePostfix[] = ".as";
    const char fileWritePostfix[] = ".am";
    const char preScanFilePostfix[] = ".temp_ob";
    const char finalFilePostfix[] = ".ob";

    int numOfFiles = argc - 1;

    /* no files specified */
    if (argc == 1) return 0;

    /* files received from program arguments */
    readFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    /* files to write and deploy the macros read from readFiles */
    writeFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    postDeployReadFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    preScanFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);
    finalFiles = (FILE **) malloc(sizeof(FILE *) * numOfFiles);

    /* tables for each file name */
    macrosTables = (hashTable **) malloc(sizeof(hashTable *) * numOfFiles);
    labelsTables = (hashTableInt**) malloc(sizeof(hashTable *) * numOfFiles);

    /* file names for each step */
    preDeployFileNames = (char **) calloc(numOfFiles, sizeof(char));
    postDeployFileNames = (char **) calloc(numOfFiles, sizeof(char));
    preScanFileNames = (char **) calloc(numOfFiles, sizeof(char));
    finalFileNames = (char **) calloc(numOfFiles, sizeof(char));

    /* storing all files in readFiles array */
    for (i = 1; i < numOfFiles+1; i++) {
        currentFileNameLength = strlen(argv[i]);
        preDeployFileNames[i-1] = (char *) calloc(currentFileNameLength + strlen(filePostfix), sizeof(char)); /* adding 4 for .as postfix */
        postDeployFileNames[i-1] = (char *) calloc(currentFileNameLength + strlen(fileWritePostfix), sizeof(char)); /* adding 4 as adding .am postfix */
        preScanFileNames[i-1] = (char *) calloc(currentFileNameLength + strlen(preScanFilePostfix), sizeof(char)); /* adding 8 as adding .temp_ob postfix */
        finalFileNames[i-1] = (char *) calloc(currentFileNameLength + strlen(finalFilePostfix), sizeof(char)); /* adding 4 as adding .ob postfix */

        if (preDeployFileNames[i-1] == NULL || postDeployFileNames[i-1] == NULL
            || preScanFileNames[i-1] == NULL || finalFileNames[i-1] == NULL) {
            fprintf(stderr, MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE);
            return MEMORY_NOT_ALLOCATED_ERROR_CODE;
        }

        /* setting every file name */
        strcpy(preDeployFileNames[i-1], argv[i]);
        strcpy(postDeployFileNames[i-1], argv[i]);
        strcpy(preScanFileNames[i-1], argv[i]);
        strcpy(finalFileNames[i-1], argv[i]);

        /* adding ".as" postfix */
        strcat(preDeployFileNames[i-1], filePostfix);
        /* adding ".am" postfix */
        strcat(postDeployFileNames[i-1], fileWritePostfix);
        /* adding ".temp_ob" postfix */
        strcat(preScanFileNames[i-1], preScanFilePostfix);
        /* adding ".ob" postfix */
        strcat(finalFileNames[i-1], finalFilePostfix);

        /* trying to open file to read macros from */
        if ((readFiles[i-1] = fopen(preDeployFileNames[i-1], READ_MODE)) == NULL) {
            fprintf(stderr, "Error trying to open file %s\n", preDeployFileNames[i-1]);
            free(preDeployFileNames[i-1]);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(readFiles[j]);
            free(readFiles);
            return MAIN_ERROR_CODE;
        }
        /* trying to open file to write to with all macros */
        if ((writeFiles[i-1] = fopen(postDeployFileNames[i-1], WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", postDeployFileNames[i-1]);
            free(postDeployFileNames[i-1]);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(writeFiles[j]);
            free(writeFiles);
            return MAIN_ERROR_CODE;
        }

        /* initializing table in memory for current file */
        macrosTables[i-1] = (hashTable*) malloc(sizeof(hashTable));

        if (!init_hash_table(macrosTables[i-1], HASH_TABLE_SIZE)) {
            fprintf(stderr, "Unsuccessful initialization of hash table.\n");
            return MAIN_ERROR_CODE;
        }
    }

    entriesTables = (hashTableInt **) malloc(sizeof(hashTableInt*));
    externsTables = (hashTableInt **) malloc(sizeof(hashTableInt*));

    for (i = 0; i < numOfFiles; i++) {
        if (!deploy_macros(readFiles[i], writeFiles[i],
                           macrosTables[i], &longestMacroBodyLength)) return 0;
        entriesTables[i] = (hashTableInt *) malloc(sizeof(hashTableInt));
        externsTables[i] = (hashTableInt *) malloc(sizeof(hashTableInt));
        labelsTables[i] = (hashTableInt *) malloc(sizeof(hashTableInt));

        if (!init_hash_table_int(entriesTables[i], HASH_TABLE_SIZE) ||
            !init_hash_table_int(externsTables[i], HASH_TABLE_SIZE) ||
            !init_hash_table_int(labelsTables[i], HASH_TABLE_SIZE)) {
            fprintf(stderr, "Unsuccessful initialization of hash table.\n");
            break;
        }

        if ((preScanFiles[i] = fopen(preScanFileNames[i], WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", preScanFileNames[i]);
            break;
        }

        if ((finalFiles[i] = fopen(finalFileNames[i], WRITE_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", finalFileNames[i-1]);
            break;
        }

        if ((postDeployReadFiles[i] = fopen(postDeployFileNames[i], READ_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", postDeployFileNames[i]);
            free(postDeployFileNames[i]);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(writeFiles[j]);
            free(writeFiles);
            break;
        }

        if (!first_scan(postDeployReadFiles[i], preScanFiles[i], labelsTables[i],
                        &IC, &DC, entriesTables[i], externsTables[i])) {
            /* TODO: free all pointers */
            break;
        }

        fclose(preScanFiles[i]);

        if ((preScanFiles[i] = fopen(preScanFileNames[i], READ_MODE)) == NULL) {
            fprintf(stderr, "Unable to write to file %s\n", preScanFileNames[i]);
            free(preScanFileNames[i]);
            /* free all opened files in case one doesn't exist */
            for (j = 0; j < i; j++) fclose(preScanFiles[j]);
            free(preScanFiles);
            break;
        }

        if (!second_scan(preScanFileNames[i], preScanFiles[i], finalFiles[i], labelsTables[i],
                         entriesTables[i], externsTables[i], &IC)) {
            remove(finalFileNames[i]);
            remove(postDeployFileNames[i]);
            return 0;
        }

        /* remove .am file */
        remove(postDeployFileNames[i]);
        lastFileIndex++;
    }

    return 1;
}
