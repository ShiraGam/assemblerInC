#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "Errors.h"
#include "Global_fun.h"
#include "Reserved_words.h"
#include "Macro.h"
#include "Global_def.h"
#include "Symbols.h"



int fill_and_check(char * buffer,FILE * file){
    char c;
    int index = 0;
    c=fgetc(file);
	while(isspace(c) != 0){
		c=fgetc(file);
	}
    for(;index<MAX_LINE_LENGTH &&c!='\n'&&c!=EOF ;index++){
	  buffer[index]=c;
	  c=fgetc(file);
     }
     buffer[index++]='\n';
     buffer[index]='\0';
     if (c==EOF){
	return TRUE;
     }
     return FALSE;
}



char *my_strdup( char *src) {
    int length = strlen(src) + 1; /* +1 for null terminator*/
    char *dst = (char *)malloc(length); /* Allocate memory*/
    if (dst == NULL) {
	print_internal_error(memory_failed);
	return NULL;
    }
     /* Copy the string*/
    strcpy(dst, src);
    return dst; /* Return the duplicated string*/
}



int valid_end(char *line, int index,struct file_status * file, int *error_exists) {
    char *ptr = line + index; /* Move pointer to the given index*/

    /* Iterate through characters from the given index*/
    while (*ptr != '\0') {
        if (!isspace(*ptr)) { /* Check if the character is not whitespace*/
            *error_exists = TRUE; /* Set error to true if non-whitespace character found */
            print_external_error(extra_text, file);
            return FALSE; /* Not only whitespace*/
        }
        ptr++; /* Move to the next character*/
    }

    return TRUE; /* Only whitespace from the given index*/
}



int legal_word(char *str,int is_define, struct macro_linked_list *list,struct file_status * file) {

    if (!isalpha(*str)){ /* Check if the first character is alphabetic*/
        print_external_error(illegal_name, file);
        return FALSE;
    }
    if ( reserved_words(str)){ /* Check if the word is a reserved word*/
        print_external_error(reserved_word, file);
        return FALSE;
    }	
    if ( !is_define){
    while (*str) {
        if (!isdigit(*str) && !isalpha(*str)){ /* Check if the character is printable and not whitespace */
            print_external_error(illegal_name, file);
            return FALSE;
	}
        str++;
    }}
    if (macro_name_appeared(list, str) ){ /* Check if the macro name appeared*/
        print_external_error(defined_macro,file);
        return FALSE;
    }

    return TRUE;
}




void free_strings(int num_strings, ...) {
int i = 0;
    va_list args;
    va_start(args, num_strings);

    for (; i < num_strings; ++i) {
        char *str = va_arg(args, char*);
        free(str); /* Free the memory allocated for the string*/
    }

    va_end(args);
}



int skip_spaces(char ** ptp){
	while (**ptp != '\0') {
        	if (!isspace(**ptp)) { /* Check if the character is not whitespace*/
            		return TRUE; /* Not only whitespace*/
        	}
        	(*ptp)++; /* Move to the next character*/
        }
	return FALSE;/*if we got here we came to '\0' so there is nothing in the line*/	
}



/*assume that we dont have spaces in beginins. stops in space*/
char * next_word(char ** ptp){
	int i = 0;
    char * first_word = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    if (first_word == NULL){
	        print_internal_error(memory_failed);
        return NULL; 
    }
    while(i<MAX_LINE_LENGTH && !isspace(**ptp) && isprint(**ptp) && (**ptp) != EOF){
      first_word[i] = **ptp;
      (*ptp)++;
      i++; 
    }
    first_word[i] = '\0';
    return first_word;
    /*notice- i moved to be the end of first_word ,ptp moved and cut of the first_word!*/
}



/**
*stops when it meets ',' or end of line or space.... assume that we dont have just white spaces until the end of line
*assume that we dont have spaces in beginins
**/
char * next_param(char ** ptp){
	int i =0;
    char * param = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    if (param == NULL){
	        print_internal_error(memory_failed);
        return NULL; 
    }
    while(i<MAX_LINE_LENGTH && isprint(**ptp) &&!isspace(**ptp)&& (**ptp) != ','  && (**ptp) != EOF){
      param[i] = **ptp;
      (*ptp)++;
      i++; 
    }
    param[i] = '\0';
    return param;
    /*notice- ptp moved and cut of the next_param*/
}




int figure_addressing_methods(char * opernad,struct symbols_linked_list * list){
    /*Index indicate the addressing methods (respectively) 
    * IMMEDIATE 0 
    * DIRECT 1 
    * BY_INDEX 2
    * REGISTER 3 
    * NO_OPPERAND 4 */
    struct symbols_node * node;
    if (opernad==NULL){
        return NO_OPPERAND;
    }
    /* the folow steps are correct because label,define or register will never begin with #,+,- or digit */
    if(*opernad == '#'){
        /*skip the #*/
        opernad++;
    }
    if (isdigit(*opernad) || *opernad == '+' || *opernad=='-'){
        /*so its number*/
        return IMMEDIATE;
    }
/*assume that label or define will not end with ]*/
    if(opernad[  strlen(opernad)-1  ] == ']'){
        return BY_INDEX;
    }
    if (is_register(opernad)){
        return REGISTER;
    }
    node = get_symbol(list,opernad);
    if (node != NULL && node->location == MDEFINE){
        /*so the opernad is define*/
        return IMMEDIATE;
    }
    /*its word that not register, define, number, not ends with ] so its label(that aleady declare or that will declare in the future)*/
    return DIRECT;
}


