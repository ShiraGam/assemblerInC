#ifndef OPCODE_TABLE_H
#define OPCODE_TABLE_H
#include "First_pass.h"
#define OPERAND_OPTIONS 5/*For 4 addressing methods + possibility that no operand exists for this command*/
#define OPCODES_LENGTH 4
typedef struct {
    char* name;/*Action name*/
    int opcode[OPCODES_LENGTH];/*the binary representation*/
	/*Indexes 0 1 2 3 indicate the addressing methods (respectively) 
      Index 4 - indicates that there is no  operand*/
    int source_operand[OPERAND_OPTIONS];
    int destination_operand[OPERAND_OPTIONS];
} instruction;
/**
 * Code the operation code based on the index action.
 *
 * This function codes the operation code based on the provided index action,
 *
 * @param index_action An integer representing the index action to be coded(0-15).
 * @param locations A pointer to the data structure holding IC and DC.
 * @param structures A pointer to the data structures.
 */
void code_opcode(int index_action,DC_IC * locations,date_structures * structures);
/**
 * Calculate the L value after checking that te number of arguments and their type suit the action.
 *
 * @param index_action An integer representing the index action.
 * @param destination_operand_method An integer representing the addressing method of the destination operand.
 * @param source_operand_method An integer representing the addressing method of the source operand.
 *
 * @return L value if everything suit ,otherwise fictive 
 */
int calculate_L_and_check(int index_action,int destination_operand_method,int source_operand_method);
/**
 * Perform calculation based on operand method.
 *
 * This function calculate how many words this argument adds, based on the provided operand method.
 *
 * @param operand_method An integer representing the addressing method of the operand.
 *
 * @return Returns an integer indicating the result of the calculation.
 */
int calculate(int operand_method);
/**
 * Determine the index action based on the string that represent the action.
 *
 * @param word A pointer to the word to be analyzed.
 *
 * @return Returns an integer representing the index action determined(0-15)
 *        
 */
int index_action(char * word);

 #endif  /* OPCODE_TABLE_H */ 
  


