#ifndef MY_PRE_PROCCESOR_H
#define MY_PRE_PROCCESOR_H
#include <stdio.h>
#include "Macro.h"
#include "Global_def.h"
/**
*@brief Skips characters in a file until a newline character or EOF is encountered.
*@param file_as A pointer to the FILE structure representing the file to read from.
*@return Returns TRUE if EOF is encountered, indicating the end of the file; otherwise, returns FALSE.
*/
int goToNewline(FILE *file_as);

/**
 * @brief  Checks whether it is an empty line or a comment line
 * @param buffer The character array representing the line to be checked.
 *               It is assumed that the buffer is null-terminated.
 * @return       Returns TRUE (1) if the line is an omitted line, 
 *               otherwise returns FALSE (0).
 */

int omittedLine(char ** ptp);


/**
 * @brief Main function for the pre-processor.
 *
 * This function processes the input file line by line, handling macro definitions
 * and calls, and writing the processed output to the specified files.
 *
 * @param error_exist Pointer to an integer flag indicating if errors were encountered during processing.
 * @param file The name of the input file being processed (line =0 meanwhile).
 * @param file_as The file pointer for the input file.
 * @param file_am The file pointer for the output file where macro content will be written.
 * @param list Pointer to the linked list containing macro definitions.
 * 
 * @return void
 */
int pre_proccesor_main(int *error_exist, struct file_status* file, FILE *file_as, FILE *file_am, struct macro_linked_list * list);

/**
 * @brief Finds a macro name in a given text.
 *
 * This function searches for a macro name in a given text.
 *
 * @param ptp Pointer to a pointer to the text.
 * @param macroName Pointer to store the found macro name.
 * @param pti Pointer to the current index in the text.
 * @file The name of the input file being processed and the current line.
 * @return Returns NO_ERROR if successful, INTERNAL_ERROR if memory allocation fails,
 * or EXTERNAL_ERROR if there's an error in the external file.
 */
int find_name(char ** ptp , char ** macro_name,int * pti,struct file_status* file);


/**
 * @brief Validates a macro name for legality.
 *
 *
 * @param str A pointer to the string representing the macro name to be validated.
 * @param list A pointer to the head of the macro linked list where existing macro names are stored.
 * @param file A pointer to the structure containing file status information.
 *
 * @return Returns TRUE if the macro name is legal ; otherwise, returns FALSE and prints an appropriate error message.
 */
int legal_macro(char *str, struct macro_linked_list *list, struct file_status *file);


#endif /*MY_PRE_PROCCESOR_H*/
