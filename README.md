# assembler
---

# Assembler Software Readme

## Introduction

This repository contains the source code for an assembler software written in C language. The purpose of this project is to create a tool that translates assembly language code into machine language code, allowing programs to run on a computer. The assembler follows the principles outlined in this readme.

## Background

In the realm of computer programming, all instructions and data are ultimately represented in binary code. This binary code resides in the computer's memory and is processed by the central processing unit (CPU). The CPU executes machine instructions, which are sequences of bits representing various operations.

## Assembly Language

Assembly language provides a symbolic representation of machine instructions, making it easier for programmers to write and understand code. An assembler is a tool used to translate assembly language code into machine language code that the computer can execute.

## Project Overview

The goal of this project is to develop an assembler software that performs the translation from assembly language to machine language. The software operates on input files containing assembly code and produces output files containing machine code and other relevant information.

## Assumptions and Constraints

In the development of the assembler software, certain assumptions and constraints were taken into account to ensure proper functioning and prevent errors. These include:

### Symbol Naming

- Symbols, including labels, macros, and constants, are labeled with letters and numbers only.
- Each entry or external symbol can only be defined once.
- Constants can be defined twice but must have the same value.
- A symbol name cannot serve as both a macro and a constant.
- A symbol name cannot be used simultaneously as both a label and a constant.

### Array Access

- Accessing a cell that does not exist in the array is not allowed.
- Accessing a code-type constant or label in the form of an index in the array is not permitted.

### Error Handling

- Labels with errors in their definition are not accepted.
- In cases where a symbol is flagged for external entry, it is ignored without issuing an error or warning.

### Data Limits

- The project assumes a maximum size for the name of a file.
- A possible minimum and maximum integer value are considered, ensuring consistency throughout the project.
- While there may be integers used, a large number of integers throughout the project is not expected.

### Summary

In summary, the assembler software follows strict guidelines to ensure proper symbol naming, array access, and error handling. By adhering to these assumptions and constraints, the assembler is designed to effectively translate assembly language code into machine language, facilitating the execution of programs on a computer system.

## Usage

To use the assembler, input and output files must be provided via command line arguments. The assembler operates on each source file separately.

```
assembler <source_file1> <source_file2> ... <source_fileN>
```

If no errors are detected during the assembly process, the assembler generates the output files for each source file.

## Note

This assembler software is designed specifically for the assembly language defined in this project. While general explanations about the functioning of assemblers may occasionally reference this specific assembler, the focus remains on the implementation of the assembler for the defined assembly language.

## Next Steps

After the assembly process, the generated output files can be further processed through linking and loading steps to prepare the code for execution on hardware. However, these steps are not covered in this project.

---
*This readme provides an overview of the assembler software project, its purpose, usage, and assumptions. For detailed implementation instructions, refer to the project documentation and source code.*
