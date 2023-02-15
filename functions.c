#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void copyWord(char *source,char *target,int size){
    memcpy(target,source,size);
    target[size] = '\0';
}
int immediateAddressing(char *word){
    int index = 1;
    if (!isdigit(word[index]) && word[index] != '+' && word[index] != '-'){
        /* TODO: error */
        return 0;
    }
    index++; /* skip ahead to the number */
    while(isdigit(word[index])) index++; /* read the entire number */
    while(isspace(word[index])) index++;

    if (word[index] != ',') {
        /* TODO: error */
        return 0;
    }
    return index;
}

void getArgument(char *line,char *target,int index){
    int wordSize = 0;
    int copyFromMem = index;

    while (line[index] != ',' && line[index] != '\0') index++;
    wordSize = index - copyFromMem + 1;
    target = (char*) malloc(sizeof(char *) * wordSize);
    copyWord(&line[copyFromMem],target,wordSize);
}

int terminatedCorrectly(char *line,int index){
    int i = index;
    while(isspace(line[i]) && line[i] != '\0')i++;
    return line[i] == '\0';
}