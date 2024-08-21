#include "Symbols.h"
#include "Errors.h"
#include "Global_def.h"
#include "Global_fun.h"
#include "First_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int appear_in_symbols(struct symbols_linked_list *list, char * symbol_name,int type,int define_value,struct file_status * file, errors_status * errors ){
    struct symbols_node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, symbol_name) == 0){
            switch (type) {
                case REGULAR:
		        errors->external_error_exist=TRUE;
			print_external_error(double_label,file);
                    return TRUE; /* Same name */
                case EXTERN:
                    if (current->update_attribute != EXTERNAL) {
		       errors->external_error_exist=TRUE;
			print_external_error(double_label,file);
                        return TRUE; /* Appears but not as extern, so it's a problem */
                    }
		    /*if we are here it means that we had this extern aleady so i dont want to enter it again*/
			print_warning(w_extern_double,file);
			return TRUE;
                    break;
                case DEFINE:
                    if (current->location != MDEFINE) {
		        errors->external_error_exist=TRUE;
			print_external_error(double_label,file);
                         return TRUE; /* There is another label in the same name */
                    }
		    else if (current->value != define_value) {
			/*its define, with another value*/
			errors->external_error_exist=TRUE;
			print_external_error(double_label_define_value,file);
                         return TRUE; /* There is a define not in the same value */
			}
			else{
			/*its define, with the same value*/
			print_warning(w_define_decleration_double,file);
			return TRUE;
			}
                    break;
            }
        }
        current = current->next;
    }
    return FALSE; /* there is no problem*/
}



/* Function to initialize the linked list */
struct symbols_linked_list* create_symbols_linked_list(){
    /*dynamically allocates memory for a new linked list*/
    struct symbols_linked_list * newList = (struct symbols_linked_list *)malloc(sizeof(struct symbols_linked_list));
    if (newList == NULL) {
        print_internal_error(memory_failed);
        return NULL;
    }
    /*initializes its head and tail pointers to NULL*/
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}




/* Function to add a node to the end of the linked list. this function sets next and is_entry itself */
void insert_new_symbol(struct symbols_linked_list *list, struct symbols_node * new_symbols_node){
    new_symbols_node->next=NULL;
    new_symbols_node->is_entry=FALSE;
    if (list->head == NULL) {
        list->head = new_symbols_node;
        list->tail = new_symbols_node;
    } else {
        list->tail->next = new_symbols_node;
        list->tail = new_symbols_node;
    }
}




/* Function to search for a name in the linked list and return the corresponding node.its return the adress to the node so be carefull not to change the node, coz it will change in the list too. it means you dont 
*have to aloocate memory yourself */
struct symbols_node * get_symbol(struct symbols_linked_list *list, char *name) {
    struct symbols_node * current = list->head;

    /* Traverse the linked list*/
    while (current != NULL) {
        /* Check if the name matches*/
        if (!strcmp(current->name, name)) {
            return current; /*Return the node if the name is found*/
        }
        current = current->next; /* Move to the next node*/
    }

    return NULL; /* Return NULL if the name is not found*/
}




int set_entry(struct symbols_linked_list * list,char * name,struct file_status * file){
    struct symbols_node * current = list->head;

    /* Traverse the linked list*/
    while (current != NULL) {
        /* Check if the name matches*/
        if (!strcmp(current->name, name)) {
            if (current->location == MDEFINE){
                print_external_error(double_define_entry,file);
                return FALSE;
            }
            else if (current->update_attribute == EXTERNAL){
                print_external_error(double_external_entry,file);
                return FALSE;
            }
            else{
                current->is_entry= TRUE;
		return TRUE;
            }

        }
        current = current->next; /* Move to the next node*/
    }
    print_external_error(entry_un_realized,file);
    return FALSE; /* this entry never realized*/
}




/* Function to create a new node. she puts next and is_entry itself*/
 int create_insert_symbol(struct symbols_linked_list * list,char *name, int value,int location,int update_attribute,long size,int line){
    /*dynamically allocates memory for a new node*/
    struct symbols_node * new_symbols_node = (struct symbols_node*)malloc(sizeof(struct symbols_node));
    if (new_symbols_node == NULL) {
        print_internal_error(memory_failed);
        return INTERNAL_ERROR; 
    }
    /*copies the symbol name and content strings*/
    new_symbols_node->name = my_strdup(name);
    if (name == NULL){
	free(new_symbols_node);
	return INTERNAL_ERROR; 
    }
    new_symbols_node->value = value;
    new_symbols_node->location = location;
    new_symbols_node->update_attribute=update_attribute;
    new_symbols_node->size=size;
    new_symbols_node->line=line;
    new_symbols_node->is_entry=FALSE;
    new_symbols_node->next=NULL;
    insert_new_symbol(list,new_symbols_node);
	return NO_ERROR;
}




/* Function to free memory allocated for the linked list */
void free_symbols_list(struct symbols_linked_list *list) {
 /* This function iterates through each node in the linked list,
 * frees the memory allocated for the symbol name,
 * and finally frees the memory allocated for each node and the linked list itself.*/
    struct symbols_node *current = list->head;
    while (current != NULL) {
        struct symbols_node *temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
    free(list);
}
