/*This file contains an array containing the reserved words in the language*/
#include "Reserved_words.h"
#include "Global_def.h"
#include <string.h>
#define RESERVED_WORD_NUM 31

/*An array of all reserved words in the language*/
char strings[RESERVED_WORD_NUM][10] = {
    /*directive names*/
    ".data",
    ".string",
    ".entry",
    ".extern",
    "define",
    /*Used to define a macro*/
    "mcr",
    "endmcr",
    /*16 instructions in our assembly language*/
    "mov",
    "cmp",
    "add",
    "sub",
    "not",
    "clr",
    "lea",
    "inc",
    "dec",
    "jmp",
    "bne",
    "red", 
    "prn",
    "jsr", 
    "rts",
    "hlt",
    /*These are the 8 registers of the processor*/
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7"
};
int reserved_words( char *str) {
    int i;
    for (i = 0; i <RESERVED_WORD_NUM; i++) {
        if (strcmp(strings[i], str) == 0)
            return TRUE; /* String exists in the array*/
    }
    return FALSE; /* String does not exist in the array*/
}

int is_register(char * str){
	/*where the registers begins*/
	int register_index=23;
	int i;
    	for (i = register_index; i < RESERVED_WORD_NUM; i++) {
        	if (strcmp(strings[i], str) == 0)
            	return TRUE; /* String is register name*/
    	}
    	return FALSE; /* String does not register name*/
}
