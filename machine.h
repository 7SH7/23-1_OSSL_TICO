#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

// if you are on Windows, you need to define strndup.
#if defined(_WIN32) || defined(_WIN64)
#include <math.h>
// strndup is not available on Windows
char *strndup(const char *s1, size_t n);
#endif

//  instruction type
enum instruction_type { 
    READ,
    WRITE,
    ASSIGN,
    MOVE,
    LOAD,
    STORE,
    ADD,
    MINUS,
    MULT,
    MOD,
    DIV,
    EQ,
    LESS,
    JUMP,
    JUMPIF,
    TERM
};

typedef struct instruction
{
    enum instruction_type type;
    unsigned char mem[3];
}Instruction;

typedef union value
{
    unsigned char num;
    Instruction instruction;
}Value;

// Struct Initializaion
Value * init_value(int size);

// read a text file line by line
char * read_a_line(FILE * fp);

// store the instruction in the value
void store_instruction(char * line, Value * value);

// error handling function
void error_handling(char *message);

// convert string to int without quotes
int int_without_quotes(char *str);

// store the value map
void store_instruction_map(char * line, Value * value, int idx);

// execute the instruction and return the next instruction index
int excute_instruction(Value * value, int idx);