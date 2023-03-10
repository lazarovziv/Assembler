#include "errors.h"

void errors(int error) {
    switch (error) {

        case 0:
            fprintf(stderr, "First character isnt alphabet\n");
            break;
        case 1:
            fprintf(stderr, "Size of label is more than 30\n");
            break;
        case 2:
            fprintf(stderr, "Label contains spaces\n");
            break;
        case 3:
            fprintf(stderr, "Label already defined\n");
            break;
        case 4:
            fprintf(stderr, "Extraneous text after command\n");
            break;
        case 5:
            fprintf(stderr, "Multiple consecutive commas\n");
            break;
        case 6:
            fprintf(stderr, "Comma after operation\n");
            break;
        case 7:
            fprintf(stderr, "Missing comma\n");
            break;
        case 8:
            fprintf(stderr, "Line length exceeded maximum\n");
            break;
        case 9:
            fprintf(stderr, "Missing input for .data\n");
            break;
        case 10:
            fprintf(stderr, "Not a number\n");
            break;

        case 12:
            fprintf(stderr,"Missing input for .string instruction\n");
            break;
        case 13:
            fprintf(stderr,"Missing \" in .string instruction\n");
            break;
    }
}