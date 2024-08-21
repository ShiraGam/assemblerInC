#include "Pre_proccesor.h"
#include "Global_def.h"
#include "Global_fun.h"
#include "Errors.h"
#include "Macro.h"
#include "Reserved_words.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>




int pre_proccesor_main(int *error_exist, struct file_status * file, FILE *file_as, FILE *file_am, struct macro_linked_list * list)
{
	int i = 0, flag_macro = 0, found_EOF = 0,result,old_size = 0,new_size,size_macro;
	char * buffer = NULL,*copy=NULL, *first_word=NULL;
	char  *macro_name=NULL;
	char **ptp;
	char* macro_content=NULL;



	while (!found_EOF)
	{
		i = 0;
		file->line++;
		/*contains the line*/
		buffer = (char *)malloc((MAX_LINE_LENGTH + LENGTH_CHECKING) * sizeof(char));
		if (buffer == NULL){
			print_internal_error(memory_failed);
			return INTERNAL_ERROR;
		}
		/*for the first word in line*/
		first_word = (char *)malloc((MAX_LINE_LENGTH + LENGTH_CHECKING) * sizeof(char));
		if (first_word == NULL){
			print_internal_error(memory_failed);
			free(buffer);
			return INTERNAL_ERROR;
		}
		/*read line*/
		found_EOF=fill_and_check(buffer,file_as);

		/*-1 coz: the 80 is without \0 and \n,and max include them(82),and strlen consider \n as char */
		if(strlen(buffer) > MAX_LINE_LENGTH - 1){
			print_external_error(too_long_line, file);
			*error_exist = TRUE;
			found_EOF = goToNewline(file_as);
			free_strings(4, macro_name, first_word, buffer, macro_content);
			continue;
		}

		copy = buffer;/*save for free after*/
		ptp=&copy;

		/*here ptp supposed to change- to skip all the spaces that in the begining of the line*/
		if (omittedLine(ptp)){

			continue;
		}

		/*find the first word in the line*/
		while (i<MAX_LINE_LENGTH && !isspace((*ptp)[i])  && ((*ptp)[i]) != EOF){
			first_word[i] = (*ptp)[i];
			i++;
		}
		first_word[i] = '\0';
		if(!strcmp(first_word,"endmcr")) {
			if (!valid_end(*ptp, i, file, error_exist)){
				free_strings(4, macro_name, first_word, buffer, macro_content);
				return EXTERNAL_ERROR;
			}
			insert_new_macro(list, macro_name, macro_content);
			flag_macro = FALSE;
			free(macro_name);
			free(macro_content);
			macro_content=NULL;
		}
		/*read macro content*/
		else if(flag_macro){
		/*If we still haven't read the content of memory allocation by malloc*/
			if (macro_content == NULL) {
				old_size = 0;
				macro_content = (char *)malloc( strlen(*ptp)* sizeof(char)+1);
				if(macro_content == NULL){
					free_strings(3, macro_name, first_word, buffer);
					print_internal_error(memory_failed);
					return INTERNAL_ERROR;
				}
				strcpy(macro_content, *ptp);
			}
			/*in middle to read macro content*/
			else {
				/*Calculate the new size*/
				old_size = strlen(macro_content);
				size_macro = strlen(*ptp);
				new_size = old_size + size_macro + 2;/*for \n*/
				macro_content = (char *)realloc(macro_content, new_size);
				if (macro_content == NULL) {

					free_strings(4, macro_name, first_word, buffer, macro_content);
					print_internal_error(memory_failed);
					return INTERNAL_ERROR;
				}
				
        		macro_content[old_size + 1] = '\0';
				strncat(macro_content, *ptp, new_size - old_size-1);
				
			}
		}
		else if(!strcmp(first_word,"mcr")){
			result = find_name(ptp, &macro_name, &i, file);
			if(result == INTERNAL_ERROR){
				free_strings(4, macro_name, first_word, buffer, macro_content);
				return INTERNAL_ERROR;
			}
			else if (result == EXTERNAL_ERROR){
				free_strings(4, macro_name, first_word, buffer, macro_content);
				return EXTERNAL_ERROR;
			}
			if (!legal_macro(macro_name,list, file)){
				free_strings(4, macro_name, first_word, buffer, macro_content);
				return EXTERNAL_ERROR;
			}
			if (!valid_end(*ptp, i, file, error_exist)){
				free_strings(4, macro_name, first_word, buffer, macro_content);
				return INTERNAL_ERROR;
			}
			flag_macro = TRUE;
		}	
		else if(macro_name_appeared(list,first_word)){
			if(valid_end(*ptp,i,file,error_exist)){
				/*retirement*/
				call_to_macro(first_word,file_am,list);  
			}
		}
		else{
			/*copy the line (ptp) to the am as is*/
			fprintf(file_am, "%s", *ptp);
		}

		/*after writing to am*/
		free(first_word);
		free(buffer);
	}
	return NO_ERROR;
}




int omittedLine(char **ptp){
	char c = **ptp;
	while (isspace(c) != FALSE && c != '\n' && c != EOF){
		(*ptp)++;
		c = **ptp;
	}
	if (c == ';' || c == '\n' || c == EOF){
		return TRUE;
	}
	return FALSE;
}




int goToNewline(FILE *file_as){
	char c = fgetc(file_as);
	while (c != EOF && c != '\n'){
		c = fgetc(file_as);
	}
	if (c == EOF){
		return TRUE;
	}
	return FALSE;
}

int find_name(char **ptp, char **macro_name, int *pti, struct file_status * file){
	char c ;  
	int macro_index=0;
	*macro_name = (char *)malloc((MAX_LINE_LENGTH + LENGTH_CHECKING) * sizeof(char));
	if (*macro_name == NULL){
		print_internal_error(memory_failed);
		return INTERNAL_ERROR;
	}
	c = (*ptp)[*pti];
	while (isspace(c) != FALSE && c != '\n' && c != EOF){
		(*pti)++;
		c = (*ptp)[*pti];
	}
	if (c == '\n' || c == EOF){
		print_external_error(macro_name_missing, file);
		return EXTERNAL_ERROR;
	}
	while (*pti<MAX_LINE_LENGTH && !isspace((*ptp)[*pti])  && ((*ptp)[*pti]) != EOF){
		(*macro_name)[macro_index] = (*ptp)[*pti];
		macro_index++;
		(*pti)++;
	}
	(*macro_name)[macro_index] = '\0';
	return NO_ERROR;
}





int legal_macro(char *str, struct macro_linked_list *list,struct file_status * file) {

    if (!isalpha(*str)){ /* Check if the first character is alphabetic*/
        print_external_error(illegal_name, file);
        return FALSE;
    }
    


    if ( reserved_words(str)){ /* Check if the word is a reserved word*/
        print_external_error(reserved_word, file);
        return FALSE;
    }    

    while (*str) {
        if (!isprint(*str)){ /* Check if the character is printable and not whitespace */
            print_external_error(illegal_char, file);
            return FALSE;
    }
        str++;
    }

    if (macro_name_appeared(list, str) ){ /* Check if the macro name appeared*/
        print_external_error(defined_macro,file);
        return FALSE;
    }

    return TRUE;
}

