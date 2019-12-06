/*
 * @file gen_code.h
 * @brief Deklarace funkc� pro generov�n� v�stupn�ho k�du
 * @author Martin �er� (xserym01@stud.fit.vutbr.cz)
 * @date 04.12.2019
 *
 * Projekt: Implementace p�eklada�e imperativn�ho jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef GEN_CODE_H
#define GEN_CODE_H

#include <stdio.h>

#define HEADER \
	".IFJcode19\n"\
	"JUMP _main\n" \
	"LABEL _main\n"\

#define INPUTS \
	"LABEL _inputs\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line\n" \
	"READ LF@line string\n" \
	"POPFRAME\n" \
	"RETURN\n" \

#define INPUTI \
	"LABEL _inputs\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line\n" \
	"READ LF@line int\n" \
	"POPFRAME\n" \
	"RETURN\n" \

#define INPUTF \
	"LABEL _inputs\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@line\n" \
	"READ LF@line float\n" \
	"POPFRAME\n" \
	"RETURN\n" \

#define CHAR \
	"LABEL _chr\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@type\n"\
	"DEFVAR LF@return\n"\
	"TYPE LF@type LF@input\n"\
	"JUMPIFEQ chr_continue LF@type int\n"\
	"EXIT int@4\n"\
	"LABEL _chr_continue\n"\
	"INT2CHAR LF@return LF@input\n"\
	"POPFRAME\n"\
	"RETURN\n"\

#define LENGTH \
	"LABEL _length\n" \
	"PUSHFRAME\n" \
	"STRLEN GF@return LF@input\n" \
	"POPFRAME\n" \
	"RETURN\n"\

#define ORD \
	"LABEL _ord\n"\
	"PUSHFRAME\n"\
	"DEFVAR LF@return\n"\
	"TYPE GF@type LF@variable2\n"\
	"JUMPIFEQ ord_continue LF@type int\n"\
	"EXIT int@4\n"\
	"LABEL _ord_continue\n"\
	"DEFVAR LF@length\n"\
	"CREATEFRAME\n"\
	"DEFVAR TF@variable1\n"\
	"MOVE TF@variable1 LF@variable1\n"\
	"CALL _length\n"\
	"MOVE LF@length TF@return\n"\
	"CLEARS\n"\
	"PUSHS LF@variable2\n"\
	"PUSHS LF@input\n"\
	"LTS\n"\
	"PUSHS LF@variable2\n"\
	"PUSHS LF@length\n"\
	"LTS\n"\
	"NOTS\n"\
	"ORS\n"\
	"POPS TF@return\n"\
	"JUMPIFEQ _ord_end TF@return bool@true\n"\
	"STRI2INT LF@$return LF@variable1 LF@variable2\n"\
	"LABEL _ord_end\n"\
	"POPFRAME\n"\
	"RETURN\n"\

#define SUBSTRING \
	"LABEL _substring\n"\

#define PRINT \
	"LABEL _print\n" \
	"PUSHFRAME\n" \
	"WRITE LF@input\n" \
	"POPFRAME\n" \
	"RETURN\n" \

void generate_build_in_functions();

void generate_header();

void start_generating();

void generate_main_header();

void generate_main_footer();

void generate_function_start(char* function_name);

void generate_function_end(char* function_name);

void generate_call_for_function(char* function_name);

void generate_return_for_function(char* value);

void generate_variable_declaration(char* variable);

void generate_value_type(int type);

void generate_func_variables(char* name, char* value);

void generate_label(char* label);

void conv_int_to_float(char* value_in_register);

void conv_float_to_int(char* value_in_register);

void conv_int_to_float_stack();

void conv_float_to_int_stack();

void conv_int_to_float_stack_2();

void conv_float_to_int_stack_2();

void generate_concat();

void generate_print();

void generate_pushs(char* variable);

void generate_pops();

void generate_adds();

void generate_subs();

void generate_muls();

void generate_divs();

void generate_idivs();

void generate_EQS();

void generate_GTS();

void generate_LTS();

void if_statement(char* function);

void if_else_statement(char* function);

void while_statement(char* function);

void while_end_statement(char* function);

#endif