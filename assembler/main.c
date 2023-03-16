#include "pre_assembler.h"
#include "encoder.h"

int main(int argc, char *argv[]) {
    FILE **readFiles, **writeFiles;
    hashTable **tables;
    int numOfFiles = argc - 1;
    /* no files specified */
    if (argc == 1) return 0;

    /* files received from program arguments */
    readFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);
    /* files to write and deploy the macros read from readFiles */
    writeFiles = (FILE**) malloc(sizeof(FILE*) * numOfFiles);
    tables = (hashTable**) malloc(sizeof(hashTable*) * numOfFiles);

    if (!deploy_macros(readFiles, writeFiles, tables, argc - 1, argv)) return 0;

    return 1;
}
