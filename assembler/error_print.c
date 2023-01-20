#include "error_print.h"

void print_error(int errorCode) {
    switch (errorCode) {
        case HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE: {
            printf(HASH_TABLE_KEY_ALREADY_EXIST_ERROR_MESSAGE);
            break;
        }
        case HASH_TABLE_KEY_DOESNT_EXIST_ERROR_CODE: {
            printf(HASH_TABLE_KEY_DOESNT_EXIST_ERROR_MESSAGE);
            break;
        }
    }
}


