#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "Global_def.h"
#include "Global_fun.h"
#include "Symbols.h"
#include "First_pass.h"
#include "Opcodes_table.h"
#include "Arrays.h"
#include "Errors.h"
#include "Reserved_words.h"

/**
 * @brief Performs the second pass of the assembler.
 *
 * @param structures Data structures used during the second pass.
 * @param errors Structure to capture error statuses.
 * @param locations IC and DC counters for instruction and data segments.
 * @param file Information about the source file being processed.
 * @param file_am The intermediate file from the first pass.
 */
void second_pass_main(date_structures* structures, errors_status* errors, DC_IC* locations, struct file_status* file, FILE* file_am);

/**
 * @brief Processes a line of code during the second pass.
 *
 * @param ptp Pointer to the current position in the line.
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param locations IC and DC counters for instruction and data segments.
 * @param structures Data structures used during the second pass.
 * @param file_ex The output file for external symbols.
 * @param prev_ic Previous instruction counter value.
 */
void send_to_function_pass_two(char** ptp, struct file_status* file, errors_status* errors, DC_IC* locations, date_structures* structures, FILE* file_ex, int prev_ic);

/**
 * @brief Encodes the action part of the instruction.
 *
 * @param index_of_action The index of the action in the opcode table.
 * @param ptp Pointer to the current position in the line.
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param locations IC and DC counters for instruction and data segments.
 * @param structures Data structures used during the second pass.
 * @param file_ex The output file for external symbols.
 * @param prev_ic Previous instruction counter value.
 */
void action_coding(int index_of_action, char** ptp, struct file_status* file, errors_status* errors, DC_IC* locations, date_structures* structures, FILE* file_ex, int prev_ic);

/**
 * @brief Encodes the operand of an instruction.
 *
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param locations IC and DC counters for instruction and data segments.
 * @param structures Data structures used during the second pass.
 * @param operand The operand to encode.
 * @param file_ex The output file for external symbols.
 * @param source_operand Flag indicating if the operand is a source operand.
 * @param destination_operand Flag indicating if the operand is a destination operand.
 * @param prev_ic Previous instruction counter value.
 */
void coding_operand(struct file_status* file, errors_status* errors, DC_IC* locations, date_structures* structures, char* operand, FILE* file_ex, int source_operand, int destination_operand, int prev_ic);

/**
 * @brief Encodes an immediate operand.
 *
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param locations IC and DC counters for instruction and data segments.
 * @param structures Data structures used during the second pass.
 * @param operand The immediate operand to encode.
 * @param prev_ic Previous instruction counter value.
 */
void coding_immediate(struct file_status* file, errors_status* errors, DC_IC* locations, date_structures* structures, char* operand, int prev_ic);

/**
 * @brief Encodes a register operand.
 *
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param locations IC and DC counters for instruction and data segments.
 * @param structures Data structures used during the second pass.
 * @param register_value The register operand to encode.
 * @param source_operand Flag indicating if the operand is a source operand.
 * @param prev_ic Previous instruction counter value.
 */
void coding_register(struct file_status* file, errors_status* errors, DC_IC* locations, date_structures* structures, char* register_value, int source_operand, int prev_ic);

/**
 * @brief Encodes an operand using index addressing.
 *
 * @param file Information about the source file being processed.
 * @param operand The operand to encode.
 * @param locations IC and DC counters for instruction and data segments.
 * @param structures Data structures used during the second pass.
 * @param errors Structure to capture error statuses.
 * @param file_ex The output file for external symbols.
 * @param prev_ic Previous instruction counter value.
 */
void coding_by_index(struct file_status* file, char* operand, DC_IC* locations, date_structures* structures, errors_status* errors, FILE* file_ex, int prev_ic);

/**
 * @brief Encodes a direct operand.
 *
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param operand The operand to encode.
 * @param locations IC and DC counters for instruction and data segments.
 * @param structures Data structures used during the second pass.
 * @param file_ex The output file for external symbols.
 * @param prev_ic Previous instruction counter value.
 */
void coding_direct(struct file_status* file, errors_status* errors, char* operand, DC_IC* locations, date_structures* structures, FILE* file_ex, int prev_ic);

/**
 * @brief Checks if the operand contains a comma.
 *
 * @param ptp Pointer to the current position in the line.
 * @return int Returns 1 if the operand contains a comma, 0 otherwise.
 */
int contains_comma(char** ptp);

/**
 * @brief Sets a range of bits in a bit field.
 *
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param bitfield The bit field to modify.
 * @param start The starting bit position.
 * @param end The ending bit position.
 * @param value The value to set within the bit range.
 * @param prev_ic Previous instruction counter value.
 * @param current_ic Current instruction counter value.
 */
void set_bit_range(struct file_status* file, errors_status* errors, bit_field* bitfield, int start, int end, int value, int prev_ic, int current_ic);

/**
 * @brief Prints the encoded bits to the object file.
 *
 * @param file_ob The object file to write to.
 * @param instructions_array The array of instructions to encode.
 * @param array_length The length of the instructions array.
 * @param begin The starting position to print.
 */
void print_bits_encoded(FILE* file_ob, bit_field* instructions_array, int array_length, int begin);

/**
 * @brief Closes the external symbols file.
 *
 * @param file_ex The external symbols file to close.
 * @param errors Structure to capture error statuses.
 * @param file_name_with_ex The name of the external symbols file.
 */
void close_ex_file(FILE* file_ex, errors_status* errors, char* file_name_with_ex);

/**
 * @brief Prints the entry symbols to the corresponding file.
 *
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param structures Data structures used during the second pass.
 */
void print_entry(struct file_status* file, errors_status* errors, date_structures* structures);

/**
 * @brief Prints the object file after the second pass.
 *
 * @param file Information about the source file being processed.
 * @param errors Structure to capture error statuses.
 * @param structures Data structures used during the second pass.
 * @param locations IC and DC counters for instruction and data segments.
 * @param prev_ic Previous instruction counter value.
 */
void print_ob_file(struct file_status* file, errors_status* errors, date_structures* structures, DC_IC* locations, int prev_ic);

#endif
