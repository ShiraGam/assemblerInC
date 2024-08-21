#ifndef MY_MACRO_H
#define MY_MACRO_H

#include <stdio.h>
#include <stdlib.h>
/* Struct for the macro_node */
struct macro_node {
    char *name;
    char *content;
    struct macro_node *next;
};

/* Struct for the linked list */
struct macro_linked_list {
    struct macro_node *head;
    struct macro_node *tail;
};

/* Function declarations */
/**
 * @brief Creates a new linked list.
 *
 * @return Pointer to the newly created linked list.
 */
struct macro_linked_list* create_macro_linked_list();
/**
 * @brief Adds a new macro_node to the end of the linked list.
 *
 * @param list Pointer to the linked list.
 * @param name Pointer to the macro name string.
 * @param content Pointer to the macro content string.
 * 
 * @return void
 */
int insert_new_macro(struct macro_linked_list *list, char *name, char *content);
/**
 * @brief Creates a new macro_node with the given macro name and content.
 *
 * @param name Pointer to the macro name string.
 * @param content Pointer to the macro content string.
 * 
 * @return Pointer to the newly created macro_node.
 */
 struct macro_node* create_macro_node(char *name, char *content);

/**
 * @brief Checks whether a macro name has appeared in the linked list.
 *
 * @param list Pointer to the linked list.
 * @param name Pointer to the macro name string to check.
 * 
 * @return 1 if the macro name appeared , otherwise 0.
 */

int macro_name_appeared(struct macro_linked_list *list, char *name);



/**
 * @brief Writes the macro content to a file if the macro name matches a given word.
 *
 * This function searches for a macro_node in the linked list whose macro name matches
 * the given word. If a match is found, it writes the macro content of that macro_node
 * to the specified file pointer.
 *
 * @param first_word The word to be compared with the macro names in the linked list.
 * @param file_am The file pointer where the macro content will be written.
 * @param list Pointer to the linked list containing macro definitions.
 * 
 * @return void
 */
void call_to_macro(char *first_word, FILE *file_am, struct macro_linked_list *list);


/**
 * @brief Frees the memory allocated for the linked list and its macro_nodes.
 *
 * @param list Pointer to the linked list to be freed.
 * 
 * @return void
 */
void free_macro_list(struct macro_linked_list *list);

#endif /* MY_MACRO_H */

