#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "constants.h"
#include "functions.h"

enum status {COMMA,NUMBER,DONE};


/* maybe change the return value of groupOneFirstArg to the number of addressing code */
int groupOneFirstArg(char *word,int operation);
int groupOneSecondArg(char *word, int operation);
int firstGroupOps(int operation, char *line);

/* covers cases of registers and labels */
int validRegisterOrLabel(char *line);

/* validate not,clr,inc,dec */
int secondGroupOps(char *line,int operation);

/* cover case of jmp,bne,jsr */
int jumpAddressing(char *line);

int thirdGroupOps(char *line);

/* function to validate the numbers recieved by .data instruction */
int validData(char *line);

/*funtion to check the string recieved by .string instruction */
int validString(char *string);

/* function to validate that the input recieved by .entry is a label */
int validEntryOrExtern(char *line);
