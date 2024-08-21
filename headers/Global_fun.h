#ifndef GLOBAL_FUN_H
#define GLOBAL_FUN_H
#include "Macro.h"
#include "Global_def.h"
#include "Symbols.h"
#include "First_pass.h"


/**
 * @brief Duplicates a string.
 *
 * This function dynamically allocates memory for a new string and copies the content
 * of the source string into it.
 *
 * @param src Pointer to the source string to be duplicated.
 * 
 * @return Pointer to the newly allocated duplicated string. NULL if memory allocation fails.
 */
char *my_strdup(char *src);

/**
 * @brief Finds the index of the first occurrence of a character in a string.
 *
 * This function searches for the first occurrence of a specified character within
 * the provided string and returns the index of that character if found.
 *
 * @param str Pointer to the null-terminated string to search within.
 * @param target The character to search for within the string.
 * @return The index of the first occurrence of the target character within the string,
 * or -1 if the character is not found.
 */
int indexOf(char *str, char target);

/**
 * @brief Checks if the remaining part of a line contains only whitespace characters.
 *
 * @param line The input string.
 * @param index The index from which to start checking.
 * @param file_status the name of the file and the line.
 * @param error_exists A pointer to a variable representing if an error exists in the file.
 * 
 * @return 1 if the remaining part of the line contains only whitespace, 0 otherwise.
 */
int valid_end(char *line, int index, struct file_status* file, int *error_exists);

/**
 * @brief Checks if a given word is legal i.e., it starts with an alphabetic
 * character, contains only printable characters, and does not match any reserved words
 * in the array of reserved words.
 *
 * @param str The word to check.
 * @param is_define if we check symbol in type define or not.
 * @param list pointer to the list of macros.
 * @param file_status the name of the file and the line.
 * 
 * 
 * @return 1 if the word is legal, 0 otherwise.
 */
int legal_word( char *str,int is_define, struct macro_linked_list *list, struct file_status* file);

/**
 * @brief Frees memory allocated for a variable number of strings.
 *
 * @param num_strings The number of strings to be freed.
 * @param ...         The strings to be freed.
 */
void free_strings(int num_strings, ...);
/**
 * Fill a buffer and check for end of file.
 *
 * @param buffer A pointer to the buffer where the line will be stored.
 * @param file A pointer to the FILE object representing the file to read from.
 *
 * @return Returns an integer indicating whether there is EOF or not.
 *       
 */
int fill_and_check(char * buffer,FILE * file);
/**
 * Skip leading spaces in a string.
 *
 * @param ptp A pointer to a pointer to the string where spaces will be skipped.
 *
 * @return Returns an integer indicating if there is spaces till the end of line or not.
 */
int skip_spaces(char ** ptp);
/**
 * Get the next word from a string.
 * This function assume that there is no spaces in the begining of the line
 * This function extracts the next word from a string and advances the pointer to the next word.
 *
 * @param ptp A pointer to a pointer to the string.
 *
 * @return Returns a pointer to the next word in the string.
 */
char * next_word(char ** ptp);
/**
 * Get the next parameter from a string (stops in comma).
 * This function assume that there is no spaces in the begining of the line
 * This function extracts the next word from a string and advances the pointer to the next parameter.
 *
 * @param ptp A pointer to a pointer to the string.
 *
 * @return Returns a pointer to the next parameter in the string.
 */
char * next_param(char ** ptp);
/**
 * Determine the addressing methods for an operand.
 *
 * This function analyzes an operand and determines the addressing methods used.
 *
 * @param operand A pointer to the operand to be analyzed.
 * @param list A pointer to the symbols linked list used for reference.
 *
 * @return Returns an integer representing the addressing methods according the definers in Global_def.h
 */
int figure_addressing_methods(char * opernad,struct symbols_linked_list * list);
void printing(date_structures * structures, errors_status * errors,DC_IC * locations);   

#endif  /* GLOBAL_FUN_H */
