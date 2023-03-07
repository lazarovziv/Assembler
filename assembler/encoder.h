#include <string.h>
#include "hash_table.h"

enum addressingType {
    IMMEDIATE, DIRECT, JUMP, REGISTER
};

int first_scan(FILE *file, hashTable *table, int *IC, int *DC);
int command_code(char *command);
int encode_mov_command(char *command, enum addressingType *first, enum addressingType *second);
/*
int encode_cmp_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_add_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_sub_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_not_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_clr_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_lea_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_inc_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_dec_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_jmp_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_bne_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_red_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_prn_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_jsr_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_rts_command(char *command, enum addressingType *first, enum addressingType *second);
int encode_stop_command(char *command, enum addressingType *first, enum addressingType *second);
 */

void encode_command(char *command, int commandCode);

int is_register(char *string);
