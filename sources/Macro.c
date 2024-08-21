#include "Macro.h"
#include "Errors.h"
#include "Global_def.h"
#include "Global_fun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* Function to create a new macro_node*/
 struct macro_node* create_macro_node(char *name, char *content) {
    /*dynamically allocates memory for a new macro_node*/
    struct macro_node* new_macro_node = (struct macro_node*)malloc(sizeof(struct macro_node));
    if (new_macro_node == NULL) {
        print_internal_error(memory_failed);
        return NULL;
    }
    /*copies the macro name and content strings*/
    new_macro_node->name = my_strdup(name);
    new_macro_node->content = my_strdup(content);
    if (new_macro_node->name == NULL || new_macro_node->content == NULL){
	print_internal_error(memory_failed);
        return NULL;
    }
    new_macro_node->next = NULL;
    return new_macro_node;
}




/* Function to initialize the linked list */
struct macro_linked_list* create_macro_linked_list() {
    /*dynamically allocates memory for a new linked list*/
    struct macro_linked_list* newList = (struct macro_linked_list*)malloc(sizeof(struct macro_linked_list));
    if (newList == NULL) {
        print_internal_error(memory_failed);
        return NULL;
    }
    /*initializes its head and tail pointers to NULL*/
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}



/* Function to add a macro_node to the end of the linked list */
int insert_new_macro(struct macro_linked_list *list, char *name, char *content) {
    /*creates a new macro_node with the given macro name and content*/
    struct macro_node *new_macro_node = create_macro_node(name, content);
    if (new_macro_node == NULL){
	return INTERNAL_ERROR;
    }	
    if (list->head == NULL) {
        list->head = new_macro_node;
        list->tail = new_macro_node;
    } 
    else {
        list->tail->next = new_macro_node;
        list->tail = new_macro_node;
    }
    return NO_ERROR;
}




/*writes the macro content*/
void call_to_macro(char *first_word, FILE *file_am, struct macro_linked_list *list) {
    struct macro_node *current = list->head; /* Start from the head of the linked list */
    while (current != NULL) { /* Loop through the linked list */
        if (strcmp(current->name, first_word) == 0) { /* Check if macro name matches */
            fprintf(file_am, "%s", current->content); /* Write macro content to file */
            break; /* Stop searching after finding the first match */
        }
        current = current->next; /* Move to the next macro_node */
    }
}




/* Function to check whether a macro name appeared in the linked list */
int macro_name_appeared(struct macro_linked_list *list, char *name) {
    struct macro_node *current = list->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return TRUE; /* Macro name found*/
        current = current->next;
    }
    return FALSE; /* Macro name not found*/
}




/* Function to free memory allocated for the linked list */
void free_macro_list(struct macro_linked_list *list) {
 /* This function iterates through each macro_node in the linked list,
 * frees the memory allocated for the macro name and content strings,
 * and finally frees the memory allocated for each macro_node and the linked list itself.*/
    struct macro_node *current = list->head;
    while (current != NULL) {
        struct macro_node *temp = current;
        current = current->next;
        free(temp->name);
        free(temp->content);
        free(temp);
    }
    free(list);
}
