#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H

struct file_status {
    char *name;
    int line;  
};
#define FALSE 0  
#define TRUE 1
#define MAX_LINE_LENGTH 82/*Maximum line length*/
#define NO_ERROR 0 /* no error found*/
#define INTERNAL_ERROR 1 /* internal error accured*/
#define EXTERNAL_ERROR 2  /* external error found*/
#define LENGTH_CHECKING 3
/*we assume that*/
#define MAX_NAME_LENGTH 80


/*for symbols attribute*/
/*location....it will be fictive in extern*/
#define CODE 1
#define DATA 2
#define MDEFINE 3
/*updeate_arrtibute*/
#define RELOCATEABLE 1
#define EXTERNAL 2

/*for searching in the symbol in defferent cases- type*/
#define DEFINE 1
#define EXTERN 2
#define REGULAR 3


#define FICTIVE -1

/*this is the \0 char*/
#define END_OF_STRING 0

/*for systems*/
#define IMMEDIATE 0 
#define DIRECT 1 
#define BY_INDEX 2
#define REGISTER 3 
#define NO_OPPERAND 4 


/*for code the operands method*/
/*this are the first bit (from left) that the methos should code in*/
#define SOURCE 5
#define DESTINATION 3

/*coz we have 12 bits and one bit is reserved for the sign (0 for positive, 1 for negative).*/
#define  MAXIMUM_VALUE 2047
#define  MINIMUM_VALUE -2048
/*for set_bit_range method - coding the bits according to the instructions*/
#define START_ARE_BIT  0
#define END_ARE_BIT  1
#define START_VALUE_BIT 2
#define END_VALUE_BIT 13
#define START_BIT_SOURCE_REGISTER  5 
#define  START_BIT_DESTINATION_REGISTER 2
#define END_BIT_SOURCE_REGISTER 7   
#define  END_BIT_DESTINATION_REGISTER 4
#define  CODING_EXTERNAL 1
#define  CODING_RELOCATEABLE 2

/*14 bits in each word*/
#define BITS_PER_ENCODED_WORD 14
/*For the change to base 4*/
#define BITS_PER_ITERATION 2
/*Length of an encrypted word in base 4*/
#define ENCODED_WORD_LENGTH 7
/*mask*/
#define BASE_4 3
#define START_MEMORY 100
#define  SIZE_MEMORY 4096
#define REMOVE_ENDING 3

#endif  /* GLOBAL_DEF_H */
