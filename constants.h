#define MEMORY_NOT_ALLOCATED_ERROR_CODE -1
#define HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE -2
#define HASH_TABLE_KEY_DOESNT_EXIST_ERROR_CODE -3

#define REGISTER_WORD_LEN 2
#define MOV_CODE 0
#define CMP_CODE 1
#define ADD_CODE 2
#define SUB_CODE 3
#define LEA_CODE 6
#define NOT_CODE 4
#define CLR_CODE 5
#define INC_CODE 7
#define DEC_CODE 8
#define JMP_CODE 9
#define BNE_CODE 10
#define RED_CODE 11
#define PRN_CODE 12
#define JSR_CODE 13
#define RTS_CODE 14
#define STOP_CODE 15

#define MACRO_ALREADY_EXISTS_ERROR_MESSAGE "This macro already exists. Exiting...\n"
#define HASH_TABLE_KEY_ALREADY_EXIST_ERROR_MESSAGE "This key already exists in the hash table.\n"
#define HASH_TABLE_KEY_DOESNT_EXIST_ERROR_MESSAGE "This key doesn't exist in the hash table.\n"
#define FILE_NOT_EXIST_ERROR_MESSAGE "The file you specified doesn't exist..\n"