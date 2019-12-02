#include "dynamic_string.h"
#include "gen_code.h"

Dynamic_string code

#define PRINT_INSTRUCTION(inst) \
	string_add_char(gen_code, (inst "\n"));

#define PRINT_CODE(code) \
	string_add_char(gen_code, (code));


#define HEADER \
	".IFJcode19\n"\
	"JUMP $$main\n" \

#define INPUTS \
	"LABEL inputs\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line\n" \
	"READ LF@line string\n" \
	"POPFRAME\n" \
	"RETURN\n" \

#define INPUTI \
	"LABEL inputs\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line\n" \
	"READ LF@line int\n" \
	"POPFRAME\n" \
	"RETURN\n" \

#define INPUTF \
	"LABEL inputs\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line\n" \
	"READ LF@line float\n" \
	"POPFRAME\n" \
	"RETURN\n" \

#define CHAR \
	"LABEL chr\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@type\n"\
	"DEFVAR LF@return\n"\
	"TYPE LF@type LF@input\n"\
	"JUMPIFEQ chr_continue LF@type int\n"\
	"EXIT int@4\n"\
	"LABEL chr_continue\n"\
	"INT2CHAR LF@return LF@input\n"\
	"POPFRAME\n" \	
	"RETURN\n"\

#define LENGTH \
	"LABEL length\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line_length\n" \
	"STRLEN LF@line_length LF@input\n" \
	"POPFRAME\n" \
	"RETURN\n" \

#define ORD \
	"LABEL ord\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line_length\n" \
	"CREATEFRAME\n" \
	"DEFVAR TF@temp\n"\
	"MOVE TF@temp LF@input\n"\
	"CALL length\n" \
	"MOVE LF@line_length  GF@return"
	"STRI2INT GF@return LF@input LF@0\n"\
	"POPFRAME\n" \
	"RETURN\n" \


#define SUBSTR


#define PRINT \
	"LABEL print\n" \
	"PUSHFRAME\n" \
	"WRITE LF@input\n" \
	"POPFRAME\n" \
	"RETURN\n" \

void generate_build_in_functions() 
{

	PRINT_INSTRUCTION(INPUTS)
	PRINT_INSTRUCTION(INPUTI)
	PRINT_INSTRUCTION(INPUTF)
	PRINT_INSTRUCTION(CHAR)
	PRINT_INSTRUCTION(LENGTH)
	PRINT_INSTRUCTION(PRINT)

}

void generate_header() 
{
	PRINT_INSTRUCTION(HEADER)
}


void start_generating()
{
	string_init(code);
	generate_header();
	generate_build_in_functions();

}

void start_clearing_generator()
{
	string_free(code);
}

void print_code()
{
	printf("%s", code->str)
}

void p	
