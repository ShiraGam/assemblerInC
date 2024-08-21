#include "Opcodes_table.h"
#include "Global_def.h"
#include <string.h>
#define NUM_ACTIONS 16

instruction instructions[NUM_ACTIONS] = 
   { {"mov", {0,0,0,0},{1, 1, 1, 1,0}, {0, 1, 1, 1,0}},
       {"cmp", {0,0,0,1},{1, 1, 1, 1,0}, {1, 1, 1, 1,0}},
        {"add", {0,0,1,0},{1, 1, 1, 1,0}, {0, 1, 1, 1,0}},
        {"sub",{0,0,1,1} ,{1, 1, 1, 1,0}, {0, 1, 1, 1,0}},
        {"not",{0,1,0,0}, {0, 0, 0, 0,1}, {0, 1, 1, 1,0}},
        {"clr",{0,1,0,1}, {0, 0, 0, 0,1}, {0, 1, 1, 1,0}},
        {"lea",{0,1,1,0}, {0, 1, 1, 0,0}, {0, 1, 1, 1,0}},
        {"inc",{0,1,1,1}, {0, 0, 0, 0,1}, {0, 1, 1, 1,0}},
        {"dec",{1,0,0,0}, {0, 0, 0, 0,1}, {0, 1, 1, 1,0}},
        {"jmp",{1,0,0,1}, {0, 0, 0, 0,1}, {0, 1, 0, 1,0}},
        {"bne",{1,0,1,0}, {0, 0, 0, 0,1}, {0, 1, 0, 1,0}},
        {"red", {1,0,1,1},{0, 0, 0, 0,1}, {0, 1, 1, 1,0}},
        {"prn",{1,1,0,0}, {0, 0, 0, 0,1}, {1, 1, 1, 1,0}},
        {"jsr", {1,1,0,1},{0, 0, 0, 0,1}, {0, 1, 0, 1,0}},
        {"rts",{1,1,1,0}, {0, 0, 0, 0,1}, {0, 0, 0, 0,1}},
        {"hlt",{1,1,1,1},{0, 0, 0, 0,1}, {0, 0, 0, 0,1}} };



void code_opcode(int index_action,DC_IC * locations,date_structures * structures){
	int index_data=9,index_opcode=0;
	/*we want to code 4 bits from the 9th bits till the 6th bit ...*/
	/* it should cope like a string so the last index in the opcode is in 6th bit*/
	/*and the first index in the opcode (the index 0) should be in the 9th bit*/
	for(;index_opcode <4;index_opcode++){
		set_bit(&(structures->instructions_array)[locations->IC],index_data,instructions[index_action].opcode[index_opcode]);
		index_data--;
	}
}



/*check that the num of params and their methods sutable to the action. return fictive if not. return L if everything fine*/
int calculate_L_and_check(int index_action,int destination_operand_method,int source_operand_method){
	int L=1;/* coz we always have the first word*/
	if (instructions[index_action].source_operand[source_operand_method] != 1 || instructions[index_action].destination_operand[destination_operand_method] != 1 ){
		return FICTIVE;
	}
	if(destination_operand_method == REGISTER && source_operand_method== REGISTER){
		/*so they are both in the same word*/
		/* so we got the first word + word for the registers*/
		return L+1;
	}
	/*if not 2 registers..*/
	L += ( calculate(destination_operand_method) + calculate(source_operand_method) );
	return L;
}



/*calculate how mant words the method adds*/
int calculate(int operand_method){
	int num=0;
	if (operand_method == REGISTER || operand_method == IMMEDIATE || operand_method == DIRECT ){
		/*those adds 1 word*/
		num++;
	}
	else if (operand_method == BY_INDEX){
		/*this add 2 words*/
		num+=2;
	}
	return num;
}




/*return the index of the actiin. fictive if its not action*/
int index_action(char * word){
   int i =0;
   for (;i<NUM_ACTIONS;i++){
	if (!strcmp(word,instructions[i].name)){
		return i;
	}
   }
   return FICTIVE;
}


