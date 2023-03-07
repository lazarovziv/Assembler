#define MEMORY_NOT_ALLOCATED_ERROR_CODE -1
#define HASH_TABLE_INSERT_CONTAINS_KEY_ERROR_CODE -2
#define HASH_TABLE_KEY_DOESNT_EXIST_ERROR_CODE -3
#define MACRO_ALREADY_DEFINED_ERROR_CODE -4
#define MAIN_ERROR_CODE -5
#define HASH_TABLE_INSERT_EMPTY_KEY_ERROR_CODE -6
#define HASH_TABLE_INSERT_EMPTY_VALUE_ERROR_CODE -7


#define MACRO_ALREADY_EXISTS_ERROR_MESSAGE "This macro already exists. Exiting...\n"
#define HASH_TABLE_KEY_ALREADY_EXIST_ERROR_MESSAGE "This key already exists in the hash table.\n"
#define HASH_TABLE_KEY_DOESNT_EXIST_ERROR_MESSAGE "This key doesn't exist in the hash table.\n"
#define FILE_NOT_EXIST_ERROR_MESSAGE "The file you specified doesn't exist..\n"
#define MEMORY_NOT_ALLOCATED_SUCCESSFULLY_ERROR_MESSAGE "Memory wasn't allocated successfully.\n"

#define UNKNOWN_COMMAND_CODE -1
#define MOV_COMMAND_CODE 0
#define CMP_COMMAND_CODE 1
#define ADD_COMMAND_CODE 2
#define SUB_COMMAND_CODE 3
#define NOT_COMMAND_CODE 4
#define CLR_COMMAND_CODE 5
#define LEA_COMMAND_CODE 6
#define INC_COMMAND_CODE 7
#define DEC_COMMAND_CODE 8
#define JMP_COMMAND_CODE 9
#define BNE_COMMAND_CODE 10
#define RED_COMMAND_CODE 11
#define PRN_COMMAND_CODE 12
#define JSR_COMMAND_CODE 13
#define RTS_COMMAND_CODE 14
#define STOP_COMMAND_CODE 15

#define NUM_REGISTERS 8
#define MAX_WORD_LENGTH 81
#define HASH_TABLE_SIZE 100

#define READ_MODE "r"
#define WRITE_MODE "w"
#define MACRO_KEYWORD "mcr"
#define END_MACRO_KEYWORD "endmcr"

#define DATA_STORE_COMMAND ".data"
#define STRING_STORE_COMMAND ".string"

enum macroState {
    NOT_IN_MACRO, IN_MACRO_BODY, IN_MACRO_NAME, END_MACRO
};

extern const char *REGISTERS_NAMES[8];
