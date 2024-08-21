#include "Second_pass.h"
#include "Global_def.h"
#include "Global_fun.h"
#include "Symbols.h"
#include "First_pass.h"
#include "Opcodes_table.h"
#include "Arrays.h"
#include "Errors.h"
#include "Reserved_words.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "Macro.h"
void second_pass_main(date_structures * structures, errors_status * errors,DC_IC * locations,struct file_status * file,FILE * file_am)
{
	int prev_ic = locations->IC,found_EOF=FALSE;
	char  *buffer = NULL,*file_name_with_ext=NULL,*copy=NULL;
	char ** ptp;
	FILE *file_ex;
/*check that the amount of words is lligal*/
if((locations->IC + locations->DC) > SIZE_MEMORY){
errors->external_error_exist =TRUE;
print_external_error(too_many_words,file);


}
	/*initialize */
	locations->IC =-1;
	file->line = 0;
	fseek(file_am, 0, SEEK_SET);

	

	
	file_name_with_ext  = (char*)malloc((MAX_NAME_LENGTH)*sizeof(char));  
	if (file_name_with_ext == NULL ) {
		print_internal_error(memory_failed);
		errors->internal_error_exist = TRUE;
		return;}


	strcpy(file_name_with_ext,"../project/");
	strcat(file_name_with_ext,file->name);
	file_name_with_ext[strlen(file_name_with_ext) - REMOVE_ENDING] = '\0';
	file_ex = fopen((strcat(file_name_with_ext, ".ext")), "w+");




	if ( file_ex == NULL ) {
		print_internal_error(write_fail);
		errors->internal_error_exist = TRUE;
		free(file_name_with_ext);
		return;
	}


	while (!found_EOF)
	{
		file->line++;
		/*read line until eof*/
		buffer = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
		if (buffer == NULL){
			errors->internal_error_exist = TRUE;
			print_internal_error(memory_failed);
			close_ex_file(file_ex,errors,file_name_with_ext);
			free(file_name_with_ext);
			return ;}

		found_EOF=fill_and_check(buffer,file_am);
 if (feof(file_am)){
            found_EOF = TRUE;
        } 
		copy = buffer;
		ptp = &copy;
		if(**ptp == '\n' || **ptp == '\0' || **ptp == EOF)
			break;
		/*coding the line to the ic struct*/
		send_to_function_pass_two(ptp,file, errors, locations,structures,file_ex,prev_ic);
		if (errors->internal_error_exist == TRUE){
			close_ex_file(file_ex,errors,file_name_with_ext);
			free_strings(2,buffer,file_name_with_ext);
			return;}


		}


		locations->IC =prev_ic;

		/*No output files are found if there is an error*/
		if(errors->internal_error_exist || errors->external_error_exist){
			close_ex_file(file_ex,errors,file_name_with_ext);
			free_strings(2,buffer,file_name_with_ext);
			return;
			}

		print_ob_file(file,errors,structures,locations,prev_ic);	
		if( locations->IC  > 0 || locations->DC  > 0 ){	
			print_entry(file, errors,structures);
						}
			close_ex_file(file_ex,errors,file_name_with_ext);
			free_strings(2,file_name_with_ext,buffer);
			return;	
		
}




void send_to_function_pass_two(char ** ptp,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures,FILE * file_ex,int prev_ic){
	
			int index_of_action,word_length;
			char * first_word = NULL;
			first_word =(char*)malloc((MAX_NAME_LENGTH)*sizeof(char));
			if (first_word== NULL){
				errors->internal_error_exist=TRUE;
				return;
			}
			first_word = next_word(ptp);

			word_length=strlen(first_word);

			/*coz if its label we got label into label*/
			if(word_length >= 2 && first_word[word_length - 1] == ':'){

				/*assuming that a label should not be a set on empty line*/
				if(skip_spaces(ptp) == FALSE){
					free(first_word);
					errors->external_error_exist=TRUE;
					print_external_error(empty_label,file);
					return;}

				/*the first word after tha label*/      
				first_word = next_word(ptp);
				}

			/*Handled on the first pass*/
			if (!strcmp(first_word,".data") || !strcmp(first_word,".string")||!strcmp(first_word,".define")||!strcmp(first_word,".extern")){
				return;}

			else if (!strcmp(first_word,".entry")){
				/*.entry without label definition*/
				if(skip_spaces(ptp) == FALSE){
					free_strings(1,first_word);
					errors->external_error_exist=TRUE;
					print_external_error(empty_entry,file);
					return;}
				first_word = next_word(ptp);
				/*more than one parameter*/
				if(skip_spaces(ptp)){
				free_strings(1,first_word);
					errors->external_error_exist=TRUE;
					print_external_error(empty_entry,file);
					return;
				
				
				}
				set_entry(structures->symbols_list, first_word, file);
				return;}

			else if ((index_of_action=index_action(first_word)) != FICTIVE){
				/* if its not fictive its action ,we send the index action*/
				locations->IC++;/*move to the next word*/
				locations->IC++;/*skip coz the first word existsted in the arry*/
				action_coding(index_of_action,ptp , file,errors,locations,structures,file_ex,prev_ic);
				
				
				if (errors->internal_error_exist == TRUE){
					free_strings(1,first_word);
					return ;
				}}
			else{
				errors->external_error_exist= TRUE;
				free_strings(1,first_word);}
			return;
		}

		void action_coding(int index_of_action,char ** ptp ,struct file_status * file, errors_status * errors,DC_IC * 			locations,date_structures * structures,FILE * file_ex,int prev_ic){
			/*this is num of words the this line will take in the instructuon array*/
			int  L=FICTIVE, destination_operand_method = NO_OPPERAND, source_operand_method = NO_OPPERAND;
			char * destination_operand = NULL, * source_operand = NULL;
			/*threr is no oprands so we can countinue*/
			if(!skip_spaces(ptp)){
				destination_operand=NULL;
				source_operand=NULL;
				L= calculate_L_and_check(index_of_action,NO_OPPERAND,NO_OPPERAND);

				/*this action gots wrong parameters here*/
				if (L==FICTIVE){
					print_external_error(operand_not_correspond,file);
					free_strings(2,source_operand,destination_operand);
					errors->external_error_exist=TRUE;
					}
				locations ->IC --;
				return ;}

			source_operand = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
			if(source_operand == NULL){
			errors->internal_error_exist=TRUE;
				return;}

			destination_operand=(char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
			if( destination_operand== NULL){
				free(source_operand);
				return;}

			/*there is one oprand*/
			if(!contains_comma(ptp)){
			skip_spaces(ptp);
			source_operand= next_word(ptp);
			if(skip_spaces(ptp) == FALSE){
				destination_operand=source_operand;
				source_operand=NULL;
				destination_operand_method= figure_addressing_methods(destination_operand,structures->symbols_list);
				L= calculate_L_and_check(index_of_action,destination_operand_method,source_operand_method);
				/*this action gots wrong parameters here*/
				if (L==FICTIVE){
					print_external_error(operand_not_correspond,file);
					free_strings(2,source_operand,destination_operand);
					errors->external_error_exist=TRUE;
					return ;}
				/*coding the destination and value*/
		coding_operand(file,errors,locations,structures,destination_operand,file_ex,FALSE,destination_operand_method,prev_ic);
				
					return;
			}}

			else{ 
				skip_spaces(ptp);
				source_operand= next_param(ptp);
				skip_spaces(ptp);
				if (**ptp == ','){
                		/*skip the comma to check the parameter that come after this*/
                		(*ptp)++;
            				}
				
				if(!skip_spaces(ptp)){
					print_external_error(operand_not_correspond,file);
					errors->external_error_exist=TRUE;
					free_strings(2,source_operand,destination_operand);
					return ;
				}
				/*so we have more parameter*/
				destination_operand= next_word(ptp);
				/*source_operand[strlen(source_operand) - 1 ] = '\0';remove the comma*/
				if(skip_spaces(ptp)){
					errors->external_error_exist=TRUE;
					free_strings(2,source_operand,destination_operand);
					print_external_error(operand_not_correspond,file);
					return ;
				}

				/*get method of destination*/
				destination_operand_method= figure_addressing_methods(destination_operand,structures->symbols_list);
				source_operand_method=figure_addressing_methods(source_operand,structures->symbols_list);
				L= calculate_L_and_check(index_of_action,destination_operand_method,source_operand_method);
				if (L==FICTIVE){
					print_external_error(operand_not_correspond,file);
					free_strings(2,source_operand,destination_operand);
					errors->external_error_exist=TRUE;
					return ;}
				/*coding the sorce and value source opreand*/
				coding_operand(file,errors,locations,structures,source_operand,file_ex,TRUE,source_operand_method,prev_ic);
				/*coding the destination and value target operand*/
				if(!(destination_operand_method ==REGISTER && source_operand_method ==REGISTER)){
				locations->IC++;/*registers share one  word */
				}
		coding_operand(file,errors,locations,structures,destination_operand,file_ex,FALSE,destination_operand_method,prev_ic);
			}  return;
		}

 

		void coding_operand(struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures,char* operand,FILE * file_ex,int source_operand,int destination_operand,int prev_ic){
			
			/*coding according to addresing method*/
			if(destination_operand == IMMEDIATE){
			coding_immediate(file,errors,locations,structures,operand, prev_ic);}

			else if(destination_operand == REGISTER ){
				coding_register(file,errors,locations,structures,operand,source_operand,prev_ic);}

			else if(destination_operand == DIRECT){
				coding_direct(file,errors,operand,locations,structures,file_ex,prev_ic);}

			else if(destination_operand == BY_INDEX ){
				coding_by_index(file,operand,locations,structures,errors,file_ex,prev_ic);
			}


		}


		void coding_immediate(struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures,char *operand,int prev_ic){
			
			int is_number = TRUE,i=0,operand_value;
			struct symbols_node* node;
			if(*operand == '#'){
				/*skip the #*/
				operand++;}
			if(*operand == '+'){
				/*skip the #*/
				operand++;}
			
			if(operand[i] == '-'){
			i++;
			}

			for ( ; operand[i] != '\0'; i++) {
				if (operand[i] < '0' || operand[i] > '9'  ) {
					is_number = FALSE;
					break;}}
			if(is_number)
				operand_value = atoi(operand);
			/*check if its mdefine*/
			else{
			node=(struct symbols_node*)malloc(sizeof(struct symbols_node*));
			if (node==NULL){
				errors->internal_error_exist=TRUE;
				print_external_error(not_label,file);
				return;
			}
			node = get_symbol(structures->symbols_list,operand);
			if(!is_number && node != NULL && node->location ==MDEFINE && (node->line) < (file->line)){
				is_number = TRUE;
				operand_value=node->value;
			} }     
			if(is_number){
				/*coding ARE and value*/
set_bit_range( file,errors,&(structures->instructions_array)[locations->IC] , START_VALUE_BIT,END_VALUE_BIT,operand_value,prev_ic,locations->IC);
				/*earse?coz all its 0*/
set_bit_range(file,errors, &(structures->instructions_array)[locations->IC] , START_ARE_BIT,END_ARE_BIT,IMMEDIATE,prev_ic,locations->IC );
			}
			else{
				errors->external_error_exist= TRUE;
				print_external_error(invalid_immediate,file);}
			return;
		}

		/*get register assume that its correct*/
		void coding_register(struct file_status * file,errors_status * errors,DC_IC * locations,date_structures * structures,char* register_value,int source_operand,int prev_ic){
			int start_bit,end_bit;
			register_value++;
			if(source_operand){
				start_bit = START_BIT_SOURCE_REGISTER;
				end_bit = END_BIT_SOURCE_REGISTER;}
			else{
				start_bit = START_BIT_DESTINATION_REGISTER;
				end_bit = END_BIT_DESTINATION_REGISTER;
			}

set_bit_range( file,errors,&(structures->instructions_array)[locations->IC] ,start_bit ,end_bit,(atoi(register_value)),prev_ic,locations->IC);
		}




		void coding_by_index(struct file_status * file,char *operand,DC_IC * locations,date_structures * structures,errors_status * errors,FILE * file_ex,int prev_ic) {
			char label_name[MAX_LINE_LENGTH];
			char index[MAX_LINE_LENGTH];      
			int i = 0, j = 0,k=0,is_number=TRUE,index_value;
			int index_length = 0;
			struct symbols_node* node_label;
			struct symbols_node* node_define;
			/* Find the label name */
			while (operand[i] != '[' && operand[i] != '\0') {
				if (!isspace(operand[i])) {
					label_name[j++] = operand[i];
				} else {
					errors->external_error_exist= TRUE;
					print_external_error(space_before_bracket,file);
					return;
				}
				i++;
			}
			label_name[j] = '\0';
			
			/* Check for opening bracket */
			if (operand[i] != '[') {
				errors->external_error_exist= TRUE;
				print_external_error(missing_bracket,file);
				return;
			}
			i++;
			/*skip space*/
			while(isspace(operand[i])){
				i++;}


			/* Find the index */
			while (operand[i] != ']' && operand[i] != '\0'&& !isspace(operand[i])) {
				index[index_length++] = operand[i];
				i++;
			}

			index[index_length] = '\0';
			
			/*get details about the label*/
			 node_label=(struct symbols_node*)malloc(sizeof(struct symbols_node*));
			if (node_label==NULL){
				errors->internal_error_exist=TRUE;
				print_internal_error(memory_failed);
				return;
			}
			node_label=get_symbol(structures->symbols_list,label_name);
			/*if label never been defined*/
			if (node_label==NULL){
				errors->external_error_exist=TRUE;
				print_external_error(not_label,file);
				return;
			}
			
			/*check that the index exsist in the label*/ 
			/*check if the index is number*/
			for (k=0 ; index[k] != '\0'; k++) {
				if (index[k] < '0' || index[k] > '9') {
					is_number = FALSE;
					break;}}
			if(is_number)
				index_value = atoi(index);
			/*check if the index define as mdefine*/
				node_define=(struct symbols_node*)malloc(sizeof(struct symbols_node*));
				if (node_define==NULL){
					errors->internal_error_exist=TRUE;
					print_internal_error(memory_failed);
					return;
					}
					/*check if the index is mdefine*/
					node_define=get_symbol(structures->symbols_list,index);
					/*if itsnt define and not number - error */
					if (node_define==NULL&& !is_number){
						errors->external_error_exist=TRUE;
						print_external_error(invalid_index,file);
						return;
						}
			if(node_define != NULL){	
			
			if(node_define -> location == MDEFINE){
				is_number = TRUE;
				index_value = node_define->value;
			}}
			if((is_number && node_label->update_attribute == EXTERNAL )|| (is_number && index_value <= node_label->size &&node_label->location != MDEFINE) ){
				coding_direct(file,errors,label_name,locations,structures,file_ex,prev_ic);
				locations->IC++;
				coding_immediate(file,errors,locations,structures,index,prev_ic);}
			else{
				errors->external_error_exist=TRUE;
				print_external_error(index_not_exists,file);
				return;
			}
		}


		void coding_direct(struct file_status * file,errors_status * errors,char* operand,DC_IC * locations,date_structures * structures,FILE * file_ex,int prev_ic){
			struct symbols_node* node=(struct symbols_node*)malloc(sizeof(struct symbols_node*));
			int type;
			if (node==NULL){
				errors->internal_error_exist=TRUE;
				print_internal_error(memory_failed);
				return;
			}
			
			node=get_symbol(structures->symbols_list,operand);
			/*if label never been defined*/
			if (node==NULL){
				errors->external_error_exist=TRUE;
				print_external_error(not_label,file);
				return;
			}
			/*print to extern file*/
			if(node->update_attribute == EXTERNAL){
			fprintf(file_ex,"%s\t%d\n",node->name,locations->IC+START_MEMORY);}
type = 	node->update_attribute;	
if(type == EXTERNAL)
{
type = CODING_EXTERNAL;
}
else{
type = CODING_RELOCATEABLE;
}	
set_bit_range(file,errors,&(structures->instructions_array)[locations->IC] ,START_ARE_BIT,END_ARE_BIT,type,prev_ic,locations->IC);
set_bit_range( file,errors,&(structures->instructions_array)[locations->IC] ,START_VALUE_BIT,END_VALUE_BIT,node->value,prev_ic,locations->IC);
		}


				
		
		
int contains_comma(char **ptp) {
    char *word = *ptp;
    while (*word != '\0') {
        if (*word == ',') {
            return TRUE;
        }
        word++;
    }
    return FALSE;
}




		







void set_bit_range(struct file_status * file,errors_status * errors,bit_field *bitfield, int start, int end, int value,int prev_ic,int current_ic) {
   int range;
    unsigned int mask, shifted_value;
    range = end - start + 1;
   if (range < 0 || range > BITS_PER_ENCODED_WORD   || start> end || prev_ic   <= current_ic) {
        errors->external_error_exist=TRUE;
	return;
    }

    else if (value < MINIMUM_VALUE || value > MAXIMUM_VALUE) {
        			errors->external_error_exist=TRUE;
				print_external_error(large_value,file);
				return;
    }
     
    else{

    mask = ((1 << range) - 1) << start; /* create a mask with the bits in range set to 1 */
    shifted_value = (unsigned int)value << start; /* Cast value to unsigned int for proper sign extension*/

    /* Clear the bits in the range */
    bitfield->data &= ~mask;

    /* Set the bits to the new value */
    bitfield->data |= shifted_value;}


		}
		
		
		





void print_entry(struct file_status * file, errors_status * errors,date_structures * structures) {
			FILE * file_ent;
			long size;
			struct symbols_node *current;
			char * file_name_with_ent = NULL;
			/*node to the head in the list*/
			current=(struct symbols_node*)malloc(sizeof(struct symbols_node*));
			current = structures->symbols_list->head;
			/*open the file*/
			file_name_with_ent  = (char*)malloc((MAX_NAME_LENGTH)*sizeof(char));
			if(file_name_with_ent == NULL){
				errors->internal_error_exist = TRUE;
				print_internal_error(memory_failed);
				return;
			}
			strcpy(strcpy(file_name_with_ent,"../project/"),file->name);
			file_name_with_ent[strlen(file_name_with_ent) - REMOVE_ENDING] = '\0';
			strcat(file_name_with_ent, ".ent");
			file_ent = fopen(file_name_with_ent, "w+");
			if ( file_ent == NULL ) {
				print_internal_error(write_fail);
				errors->internal_error_exist = TRUE;
				free_strings(1,file_name_with_ent);
				return;}


			while (current != NULL) {
				if (current->is_entry) {
					fprintf(file_ent," %s \t %d\n", current->name, current->value);
				}
				current = current->next;
			}
			fseek(file_ent, 0, SEEK_END);
			size = ftell(file_ent);
			if(!size ){

				if(remove(file_name_with_ent) != FALSE){
					print_internal_error(remove_fail);}
					errors->internal_error_exist = TRUE;
			}
			fclose(file_ent);
			return;
		}






		void print_ob_file(struct file_status * file, errors_status * errors,date_structures * structures,DC_IC * locations,int prev_ic) {
			FILE * file_ob;
			long size=locations->IC;
			char * file_name_with_ob = NULL;	
			/*open the file*/
			file_name_with_ob  = (char*)malloc((MAX_NAME_LENGTH)*sizeof(char));
			if(file_name_with_ob == NULL){
				errors->internal_error_exist = TRUE;
				print_internal_error(memory_failed);
				return;
			}
			strcpy(strcpy(file_name_with_ob,"../project/"),file->name);
			file_name_with_ob[strlen(file_name_with_ob) - REMOVE_ENDING] = '\0';
			strcat(file_name_with_ob, ".ob");
			file_ob = fopen(file_name_with_ob, "w+");
			if ( file_ob == NULL ) {
				print_internal_error(write_fail);
				errors->internal_error_exist = TRUE;
				free_strings(1,file_name_with_ob);
				return;}


			/* Print each element of the instructions_array to the file */
			if(locations->IC < 0 )
			locations->IC++;
			fprintf(file_ob,"  %d %d  \n",locations->IC,locations->DC);
			print_bits_encoded(file_ob,structures->instructions_array,prev_ic,START_MEMORY);
			print_bits_encoded(file_ob,structures->data_array,(locations->DC),START_MEMORY+prev_ic);

			fseek(file_ob, 0, SEEK_END);
			size = ftell(file_ob);
			if(size == FALSE ){
				if(remove(file_name_with_ob) == FALSE){
					print_internal_error(remove_fail);}
					errors->internal_error_exist = TRUE;
			}
			fclose(file_ob);	
			return;
		}


void close_ex_file(FILE*file_ex,errors_status * errors,char * file_name_with_ext)
{
long size;
if(fseek(file_ex, 0, SEEK_END) == 0){
	size = ftell(file_ex); /* Get the current position, which is the size of the file*/
		if(size == 0 || errors->internal_error_exist || errors->external_error_exist){
			if(remove(file_name_with_ext) != FALSE){
				print_internal_error(remove_fail);
							fclose(file_ex);
							return;}
						}	
			}
fclose(file_ex);
	
}




void print_bits_encoded(FILE *file_ob, bit_field *instructions_array, int array_length,int begin) {
    int i, j;
    for (i = 0; i < array_length; i++) {
        unsigned int word = instructions_array[i].data;
        char encoded_word[ENCODED_WORD_LENGTH + 1]; /* Maximum length of encoded word is 7 characters + null terminator*/
        for (j = 0; j < BITS_PER_ENCODED_WORD; j += BITS_PER_ITERATION) {
            /* Extract BITS_PER_ITERATION bits from the word and encode them into base 4 encrypted format*/
            unsigned int bits = (word >> (BITS_PER_ENCODED_WORD - BITS_PER_ITERATION - j)) & BASE_4;
            switch (bits) {
                case 0: encoded_word[j / BITS_PER_ITERATION] = '*'; break;
                case 1: encoded_word[j / BITS_PER_ITERATION] = '#'; break;
                case 2: encoded_word[j / BITS_PER_ITERATION] = '%'; break;
                case 3: encoded_word[j / BITS_PER_ITERATION] = '!'; break;
            }
        }
        encoded_word[ENCODED_WORD_LENGTH] = '\0'; /* Null terminator*/
        /* Print the line number (100 + i) followed by the encoded word*/
        fprintf(file_ob, "%d %s\n", begin + i, encoded_word);
    }
}
		
		
