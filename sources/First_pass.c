#include "Global_def.h"
#include "Global_fun.h"
#include "Symbols.h"
#include "Opcodes_table.h"
#include "Second_pass.h"
#include "Arrays.h"
#include "Errors.h"
#include "First_pass.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int first_pass_main(struct macro_linked_list * list, int * error_exist,struct file_status * file,FILE * file_am){
    int found_EOF=FALSE;
    char ** ptp;
    char * buffer,*copy;
    DC_IC * locations;
    date_structures * structures;
    errors_status * errors;
    /*coz send to function returns struct for symbols to the symbols*/
    struct symbols_node* fictive = NULL;
    locations= (DC_IC *)malloc(sizeof(DC_IC));
    if (locations==NULL){
        print_internal_error(memory_failed);
        return INTERNAL_ERROR;
    }

    locations->DC=0;
    locations->IC=0;
    structures= (date_structures *)malloc(sizeof(date_structures));
    if (structures==NULL){
        print_internal_error(memory_failed);
        free(locations);
        return INTERNAL_ERROR;
    }

    structures->macro_list= list;
    structures->symbols_list= create_symbols_linked_list();
    if (structures->symbols_list==NULL){
        print_internal_error(memory_failed);
        free(locations);
	free(structures);
        return INTERNAL_ERROR;
    }

    /*we need to initialize the arrays*/
    structures->data_array=NULL;
    structures->instructions_array=NULL;
    errors= (errors_status *)malloc(sizeof( errors_status));
    if (errors==NULL){
        print_internal_error(memory_failed);
        free(locations);
	free_symbols_list(structures->symbols_list);
	free(structures);
        return INTERNAL_ERROR;
    }

    structures->data_array=NULL;
    structures->instructions_array=NULL;
    errors->external_error_exist=*error_exist;
    errors->internal_error_exist=FALSE;
    buffer = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
     if (buffer == NULL){
            free(locations);
	    free_symbols_list(structures->symbols_list);
	    free(structures);
	    free(errors);
            print_internal_error(memory_failed);
            return INTERNAL_ERROR;
      }
    
    

   while (!found_EOF)
   {
        file->line++;
	buffer = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
     	if (buffer == NULL){
            free(locations);
	    free_symbols_list(structures->symbols_list);
	    free(structures);
	    free(errors);
            print_internal_error(memory_failed);
            return INTERNAL_ERROR;
         }
        found_EOF=fill_and_check(buffer,file_am);
        if (feof(file_am)){
            found_EOF = TRUE;
        }
        copy = buffer;
	ptp = &copy;
        fictive = send_to_function(FALSE,ptp,file,errors,locations,structures);
        free(fictive);
        if (errors->internal_error_exist == TRUE){
            free(locations);
	    free_symbols_list(structures->symbols_list);
	    free(structures->data_array);
    	    free(structures->instructions_array);
	    free(structures);
	    free(errors);
            return INTERNAL_ERROR;
        }	
    }



    end_of_pass_1(locations,structures);
    second_pass_main(structures,errors,locations,file,file_am);
    free(buffer);
    free(locations);
    free_symbols_list(structures->symbols_list);
    free(structures->data_array);
    free(structures->instructions_array);
    free(structures);
    if (errors->internal_error_exist == TRUE){
        return INTERNAL_ERROR;
    }
    free(errors);
    return NO_ERROR;
}


struct symbols_node* send_to_function(int is_label,char ** ptp,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){
    struct symbols_node* node = NULL;
    int index_of_action;
	int word_length;
    char * first_word = next_word(ptp);
    /*notice-  ptp moved*/
    if (first_word== NULL){
        errors->internal_error_exist=TRUE;
        return NULL;
    }
    word_length=strlen(first_word);
    /*coz maybe first word is just the EOF*/
    if (word_length < 1){
	  return NULL;
     }
    /*coz if its label we got label into label*/
    if(!is_label && first_word[ word_length -1 ]==':'){
        /*send the name of label*/
        /*remove the last :,by override it */
        first_word[word_length-1] ='\0';
        node = label(first_word,ptp,file,errors,locations,structures);
    }
    else if (!strcmp(first_word,".entry")){
        free(first_word);
        /*we want skip entries*/
        return NULL;
    }
    else if (!strcmp(first_word,".extern")){
        node = extern_line(ptp,file,errors,locations,structures);
    }
    else if (!strcmp(first_word,".data")){
        node = data(ptp,file,errors,locations,structures);
    }
    else if (!strcmp(first_word,".string")){
        node = string(ptp,file,errors,locations,structures);
    }
    else if (!strcmp(first_word,".define")){
        node = define(ptp,file,errors,locations,structures);
    }
    else if ((index_of_action=index_action(first_word)) != FICTIVE){
        /* if its not fictive its action ,we send the index action*/
        node = action(index_of_action,ptp,file,errors,locations,structures);
    }
    else{
        /*to print that there is problem with the line. it can be lot of thing so print something general*/
        print_external_error(unrocognize_line,file);
        errors->external_error_exist= TRUE;
    }
    free(first_word);
    return node;
}




struct symbols_node* data(char ** ptp ,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){

    struct symbols_node* node=(struct symbols_node*)malloc(sizeof(struct symbols_node));
    struct symbols_node * define_node;
    char * param,* param2,*check_param; /* Pointer to store the position of the first invalid character*/
    long num=0;
    int end = FALSE;
    if (node==NULL){
	print_internal_error(memory_failed);
        errors->internal_error_exist=TRUE;
        return NULL;
    }
    node->value=locations->DC;
    node->location= DATA;
    node->update_attribute=RELOCATEABLE;
    node->line=file->line;
    while(!end){
        if(skip_spaces(ptp) == FALSE){
            /*in the first time it means we have no data at all and after that it means we had comma but dont param after this*/
            print_external_error(missing_parameter,file);
            errors->external_error_exist=TRUE;
            free(node);
            return NULL;
        }
        /*skip spaces not false so sure we hava something till the end*/
        param = next_param(ptp);
        if (param==NULL){
	   free(node);
	   print_internal_error(memory_failed);
            errors->internal_error_exist=TRUE;
            return NULL;
        }
        param2 = param;
        if(param2[0] == '#'){
            /*skip this char*/
            param2++;
        }
        /*The third argument is the base of the number system to convert from (typically 10 for decimal).*/
         num = strtol(param2, &check_param, 10); /*Convert string to long*/ 
        if (param2 == check_param || *check_param != '\0') {
            /* Conversion failed, un-digit character found , which it means we got difine*/
            define_node = get_symbol(structures->symbols_list,param2);
            if (define_node == NULL || define_node->location != MDEFINE){
                print_external_error(no_such_define,file);
                errors->external_error_exist=TRUE;
                free(node);
		free(param);
                return NULL;
            }
            else{
                /*we find the define that you refer to*/
		if (define_node->value > MAXIMUM_VALUE || define_node->value < MINIMUM_VALUE){
        		print_external_error(too_big_number,file);
        		errors->external_error_exist=TRUE;
			free(node);
	    		free(param);
            		return NULL;
    		}
                insert_data(define_node->value,file,errors,locations,structures);
            }
        }
        else {
		if (num > MAXIMUM_VALUE || num < MINIMUM_VALUE){
        		print_external_error(too_big_number,file);
        		errors->external_error_exist=TRUE;
			free(node);
	   		 free(param);
           		 return NULL;
    		}
            /*we got number...*/
            insert_data(num,file,errors,locations,structures);
        }
        /*checking that the insert went well*/
        if ( errors->internal_error_exist== TRUE){
            /* error happend*/
            free(node);
	    free(param);
            return NULL;
        }
        if (skip_spaces(ptp)==FALSE){
            /*we reach end of input or*/
            end = TRUE;
        }
        else{
            /*we didnt reach end of input*/
            if (**ptp == ','){
                /*skip the comma to check the parameter that come after this*/
                (*ptp)++;
            }
            else{
                /*we had parameter and then space and now something that its not comma so we are missing comma*/
                print_external_error(missing_comma,file);
                errors->external_error_exist=TRUE;
                free(node);
		free(param);
                return NULL;
            }
        }
    }
    /*it represent how much DC grew here*/
    node->size= (locations->DC)-(node->value);
    free(param);
    return node;   
}




struct symbols_node* string(char ** ptp ,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){
    struct symbols_node* node=(struct symbols_node*)malloc(sizeof(struct symbols_node));
    if (node==NULL){
        print_internal_error(memory_failed);
        errors->internal_error_exist=TRUE;
        return NULL;
    }
    node->value=locations->DC;
    node->location= DATA;
    node->update_attribute=RELOCATEABLE;
    node->line=file->line;

    if(skip_spaces(ptp) == FALSE){
        /* it means we have no string at all*/
        print_external_error(missing_parameter,file);
        errors->external_error_exist=TRUE;
        free(node);
        return NULL;
    }
    if ((**ptp) != '"'){
        /*there is no "  at first*/
        print_external_error(string_with_invalid_begin,file);
        errors->external_error_exist=TRUE;
        free(node);
        return NULL;
    }
    /*skip the "*/
    (*ptp)++;
    while ( **ptp != '\n' && ( **ptp != '"' || !isspace((*ptp)[1]) ) ){
        if(!isprint(**ptp)){
            print_external_error(invalid_char_in_string,file);
            errors->external_error_exist=TRUE;
            free(node);
            return NULL;
        }
        insert_data((int)(**ptp), file,errors,locations,structures);
        (*ptp)++;
    }
    /*we are out of the loop while because we reach \n or " with space after it*/
    if (**ptp != '"'){
        /*we out of the while not because we reaches " ,but because we reached end of line */
        print_external_error(missing_ending_char,file);
        errors->external_error_exist=TRUE;
        free(node);
        return NULL;
    }
    /*skip the "*/
    (*ptp)++;
    if (!valid_end(*ptp,0,file,&(errors->external_error_exist))){
        /*we out of the while because we reaches " ,but there is extra text after that*/
        print_external_error(missing_ending_char,file);
        free(node);
        return NULL;
    }
    insert_data(END_OF_STRING, file,errors,locations,structures);
    node->size= (locations->DC)-(node->value);
    return node;
}



/*its also increace the DC itself*/
void insert_data(int value,  struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){
    /*for size I want DC + 1 for the 0 in the end of string*/
    structures->data_array= (bit_field *)realloc(structures->data_array,(locations->DC +1)*sizeof(bit_field));
    if (structures->data_array == NULL){
        print_internal_error(memory_failed);
        errors->internal_error_exist=TRUE;
    }
	else{
    /*for index I want DC- coz indexes begin in 0*/
    (structures->data_array)[locations->DC].data= value;
    locations->DC=locations->DC+1;
	}

}



struct symbols_node* label(char * label,char ** ptp ,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){
    struct symbols_node * node;
    /*this values maybe will change*/
    int type=REGULAR,define_value= FICTIVE;
    if (!legal_word(label,FALSE,structures->macro_list,file)){
        errors->external_error_exist=TRUE;
        return NULL;
    }
    /*we need to skip spaces coz send_to_fun get only without spaces. if skip return false it means we have nothing after*/
    if(skip_spaces(ptp) == FALSE){
        print_external_error(empty_label,file);
        errors->external_error_exist=TRUE;
        return NULL;
    }
    node = send_to_function(TRUE,ptp,file,errors,locations,structures);
    if (node==NULL){
        /*it means that it was define or entry or extern or there was error*/
        return NULL;
    }
    if (node->location == MDEFINE){
        type = DEFINE;
        define_value=node->value;
    }
    if (node->update_attribute==EXTERNAL){
        type = EXTERN;
    }
    if (appear_in_symbols(structures->symbols_list,label,type,define_value,file,errors)){
	free(node);
        return NULL;
    }
    node->name= my_strdup(label);
    if (node->name == NULL){
	free(node);
        errors->internal_error_exist=TRUE;
        return NULL;
    }
    insert_new_symbol(structures->symbols_list,node);
	return NULL;
}




struct symbols_node* extern_line(char ** ptp ,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){
    char * label;
    /*skip coz send to fun get only without spaces. is skip return false it means we have nothing after*/
    if(skip_spaces(ptp) == FALSE){
        print_external_error(empty_extern,file);
        errors->external_error_exist=TRUE;
        return NULL;
    } 
    label = next_word(ptp);
    if (label == NULL){
        errors->internal_error_exist=TRUE;
        return NULL;
    }
    /*0 so that will check from begining*/
    if (!valid_end(*ptp,0,file, &(errors->external_error_exist)) || !legal_word(label,FALSE,structures->macro_list,file) ){
	free(label);
        errors->external_error_exist=TRUE;
        return NULL;
    }
	if (appear_in_symbols(structures->symbols_list,label,EXTERN,FICTIVE,file,errors)){
		free(label);
		return NULL;
        }
    /*the value is 0 coz its extern. the location and size are fictive. we will know not to add 100 coz its extern*/
    if (create_insert_symbol(structures->symbols_list,label,0,FICTIVE,EXTERNAL,FICTIVE,file->line) == INTERNAL_ERROR){
	free(label);
        errors->internal_error_exist=TRUE;
    }
    return NULL;/*coz extern never in a label*/
}




struct symbols_node* define(char ** ptp ,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){
    char * name = (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
    char* number= (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
    char * check_param, *number2;
    long num= 0;
    int i=0;
    if (name == NULL || number == NULL){
        free_strings(2,name,number);
        print_internal_error(memory_failed);
        errors->internal_error_exist=TRUE;
        return NULL;
    }
    /*skip coz send to fun get only without spaces. is skip return false it means we have nothing after*/
    if(skip_spaces(ptp) == FALSE){
        print_external_error(empty_define,file);
        errors->external_error_exist=TRUE;
        free_strings(2,name,number);
        return NULL;
    } 
    while(i<MAX_NAME_LENGTH && isprint(**ptp) &&!isspace(**ptp) && (**ptp) != '='  && (**ptp) != EOF){
      name[i] = **ptp;
      (*ptp)++;
      i++; 
    }
    name[i] = '\0';
    /*we out of while coz we got space or = or end of line*/
    /*skip coz send to fun get only without spaces. is skip return false it means we have nothing after the name of define*/
    if(skip_spaces(ptp) == FALSE){
        print_external_error(missing_define_value,file);
        errors->external_error_exist=TRUE;
        free_strings(2,name,number);
        return NULL;
    } 
    if (**ptp != '='){
        print_external_error(missing_assignment_sign,file);
        errors->external_error_exist=TRUE;
        free_strings(2,name,number);
        return NULL;
    }
    /*if we are here it mean we are on =. skip that*/
    (*ptp)++;
    if(skip_spaces(ptp) == FALSE){
        print_external_error(missing_define_value,file);
        errors->external_error_exist=TRUE;
        free_strings(2,name,number);
        return NULL;
    } 
    i = 0;
    while(i<MAX_NAME_LENGTH && !isspace(**ptp) && (**ptp) != EOF){
      number[i] = **ptp;
      (*ptp)++;
      i++; 
    }
    number[i]='\0';
    /*maybe we should skip #*/
    number2=number;
    if (*number2 == '#'){
        number2++;
    }
    /*The third argument is the base of the number system to convert from (typically 10 for decimal).*/
    num = strtol(number2, &check_param, 10); /*Convert string to long*/ 
    if (number2 == check_param || *check_param != '\0') {
        /* Conversion failed, un-digit character found*/
        print_external_error(invalid_num,file);
        errors->external_error_exist=TRUE;
        free_strings(2,name,number);
        return NULL;
        
    }
    /*we got number...*/
    if (!valid_end(*ptp,0,file,&(errors->external_error_exist))){
        free_strings(2,name,number);
        return NULL;
        
    }
    if (!legal_word(name,TRUE,structures->macro_list,file)){
        errors->external_error_exist=TRUE;
        free_strings(2,name,number);
        return NULL;
    }
    if(appear_in_symbols(structures->symbols_list,name,DEFINE,num,file,errors)){
        /*there is label in this name or define in this name but with another value*/
        free_strings(2,name,number);
        return NULL;
    }
    if(create_insert_symbol(structures->symbols_list,name,num,MDEFINE,RELOCATEABLE,FICTIVE,file->line) == INTERNAL_ERROR){
        print_internal_error(memory_failed);
        errors->internal_error_exist=TRUE;
        free_strings(2,name,number);
        return NULL;
    }
    free_strings(2,name,number); 
    /*anyway we return null coz definition of define should not be in label*/ 
    return NULL;
} 



struct symbols_node* action(int index_of_action,char ** ptp ,struct file_status * file, errors_status * errors,DC_IC * locations,date_structures * structures ){
    /*this is num of words the this line will take in the instructuon array*/
    int L ,i, destination_operand_method, source_operand_method;
    char * destination_operand, * source_operand;
    struct symbols_node* node=(struct symbols_node*)malloc(sizeof(struct symbols_node));
    if (node==NULL){
	print_internal_error(memory_failed);
        errors->internal_error_exist=TRUE;
        return NULL;
    }
    node->value=locations->IC;
    node->location= CODE;
    node->update_attribute=RELOCATEABLE;
    node->line=file->line;
    if(skip_spaces(ptp) == FALSE){
        /*so we dont hava params*/
        destination_operand=NULL;
        source_operand=NULL;
    }
    else{
        /*so we have parameters ahead*/
        source_operand= next_param(ptp);
	if (source_operand==NULL){
		print_internal_error(memory_failed);
             errors->internal_error_exist=TRUE;
		free(node);
             return NULL;
	}
        if(skip_spaces(ptp) == FALSE){
            /*so we dont hava 2 param*/
            /* if we hava just 1 opernad it consider destination_operand*/
            destination_operand=source_operand;
            source_operand=NULL;
        }
        else{
            /*so we have more parameter*/
		if (**ptp != ','){
			print_external_error(missing_comma,file);
        		errors->external_error_exist=TRUE;
			free(node);
			free(source_operand);
			return NULL;
		}
		/*so we got comma between*/
		(*ptp)++;	
		if(skip_spaces(ptp) == FALSE){
            		/*so we dont hava param after the comma*/
            		print_external_error(missing_param_after_comma,file);
        		errors->external_error_exist=TRUE;
			free(node);
			free(source_operand);
			return NULL;
        	}	
            	destination_operand= next_param(ptp);
	     	if (destination_operand==NULL){
			print_internal_error(memory_failed);
             		errors->internal_error_exist=TRUE;
			free(node);
			free(source_operand);
             		return NULL;
	     }
        }
    }
    if (!valid_end(*ptp,0,file, &(errors->external_error_exist))){
        free(node);
	free_strings(2,destination_operand,source_operand);
        return NULL;
    }
    destination_operand_method= figure_addressing_methods(destination_operand,structures->symbols_list);
    source_operand_method=figure_addressing_methods(source_operand,structures->symbols_list);
    L= calculate_L_and_check(index_of_action,destination_operand_method,source_operand_method);
    if (L==FICTIVE){
        /*this action gots wrong parameters here*/
        print_external_error(wrong_param_fun,file);
        errors->external_error_exist=TRUE;
        free(node);
	free_strings(2,destination_operand,source_operand);
        return NULL;
    }
    /* save place in the array according to L*/
	structures->instructions_array= (bit_field *)realloc(structures->instructions_array,(locations->IC + L)*sizeof(bit_field));
    if (structures->instructions_array == NULL){
        print_internal_error(memory_failed);
        errors->internal_error_exist=TRUE;
	free(node);
	free_strings(2,destination_operand,source_operand);
        return NULL;
    }
    /*initialize to zero*/
    for (i=0;i<L;i++){
        (structures->instructions_array)[locations->IC + i].data= 0;
    }
    /*code the first word in the array*/
    code_opcode(index_of_action,locations,structures);
    code_operand_method(DESTINATION,destination_operand_method,locations,structures);
    code_operand_method(SOURCE,source_operand_method,locations,structures);
    locations->IC = locations->IC+L;
    node->size = L;
    free_strings(2,destination_operand,source_operand);
    return node;
}




void code_operand_method(int first_bit,int opernad_method,DC_IC * locations,date_structures * structures){
    if (opernad_method== IMMEDIATE || opernad_method== NO_OPPERAND){
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit,0);
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit-1,0);
    }
    else if (opernad_method== DIRECT){
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit,0);
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit-1,1);
    }
    else if (opernad_method== BY_INDEX){
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit,1);
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit-1,0);
    }
    else if (opernad_method== REGISTER){
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit,1);
        set_bit( &(structures->instructions_array)[locations->IC] , first_bit-1,1);
    }
}



/*Add the IC +100 to the values in the symbol table that are both relocatable and data. and add 100 to the values that are both relocatable and code. (In other words, we will not change the value of the defines and externals at all...)*/
void end_of_pass_1(DC_IC * locations,date_structures * structures){
	struct symbols_node * current = structures->symbols_list->head;
         /*100 is for the Operating System*/
	/* Traverse the linked list*/
    	while (current != NULL) {
        	if (current->location == DATA && current->update_attribute== RELOCATEABLE) {
			current->value=current->value +100 +locations->IC;
        	}
		else if (current->location == CODE && current->update_attribute== RELOCATEABLE){
			current->value=current->value +100;
		}
        	current = current->next; /* Move to the next node*/
        }
}


