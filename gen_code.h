/*
 * @file gen_code.h
 * @brief Deklarace funkcí pro generování výstupního kódu
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 04.12.2019
 *
 * Projekt: Implementace pøekladaèe imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "dynamic_string.h"
#include <stdio.h>


dynamic_string * gen_code_string;

#define PRINT_INSTRUCTION(inst) \
	string_append(gen_code_string, (inst "\n"));

#define PRINT_CODE(code) \
	string_append(gen_code_string, (code));


#define HEADER \
	".IFJcode19\n"\
	"JUMP _main\n" \

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
	"POPFRAME\n"\
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
	"MOVE LF@line_length  GF@return\n"\
	"STRI2INT GF@return LF@input LF@0\n"\
	"POPFRAME\n" \
	"RETURN\n" \


//#define SUBSTRING


#define PRINT \
	"LABEL print\n" \
	"PUSHFRAME\n" \
	"WRITE LF@input\n" \
	"POPFRAME\n" \
	"RETURN\n" \

void generate_build_in_functions();

void generate_header();


void start_generating();

void start_clearing_generator();

void print_code();

void generate_main_header();

void generate_main_footer();

void generate_function_start(char* function_name);

void generate_function_end(char* function_name);

void generate_call_for_function(char* function_name);

void generate_return_for_function(char* type);

void generate_variable_declaration(char* variable);

void generate_label(char* label);

void generate_expr_if();

void generate_value(int type);

void generate_concat();

void generate_adds();

void generate_subs();

void generate_muls();

void generate_divs();

void generate_idivs();

void generate_EQS();

void generate_GTS();

void generate_LTS();

void generate_convert_to_double();

void generate_convert_to_int();

void generate_pushs(char* variable);

void generate_pops();

void generate_if_statement(char* label);
