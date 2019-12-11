/*
 * @file gen_code.h
 * @brief Deklarace funkcí pro generování výstupního kódu
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 04.12.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef GEN_CODE_H
#define GEN_CODE_H

#include "parser.h"
#include <stdio.h>

#define HEADER \
	".IFJcode19\n"\
	"DEFVAR GF@return\n"\
	"DEFVAR GF@$concat_result\n"\
	"DEFVAR GF@$str1\n"\
	"DEFVAR GF@$str2\n"\
	"DEFVAR GF@$variable1\n"\
	"DEFVAR GF@$variable2\n"\
	"JUMP _main\n" \


#define INPUTS \
	"LABEL _inputs\n" \
	"PUSHFRAME\n"\
	"DEFVAR LF@%return\n"\
    "READ LF@%return string\n" \
	"POPFRAME\n"\
    "RETURN\n" \

#define INPUTI \
	"LABEL _inputi\n" \
	"PUSHFRAME\n"\
	"DEFVAR LF@%return\n"\
	"READ LF@%return int\n" \
	"POPFRAME\n"\
	"RETURN\n" \

#define INPUTF \
	"LABEL _inputf\n" \
	"PUSHFRAME\n"\
	"DEFVAR LF@%return\n"\
	"READ LF@%return float\n" \
	"POPFRAME\n"\
	"RETURN\n" \

#define CHR \
	"LABEL _chr\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@type\n"\
	"DEFVAR LF@param1\n"\
	"DEFVAR LF@%return\n"\
	"DEFVAR LF@exp_type\n"\
	"DEFVAR LF@isHigher\n"\
	"DEFVAR LF@isLower\n"\
	"DEFVAR LF@top\n"\
	"DEFVAR LF@bottom\n"\
	"MOVE LF@param1 LF@%1\n"\
	"MOVE LF@top int@256\n"\
	"MOVE LF@bottom int@-1\n"\
	"MOVE LF@exp_type string@int\n"\
	"TYPE LF@type LF@param1\n"\
	"JUMPIFEQ _chr_cont_correct_type LF@type LF@exp_type\n"\
	"EXIT int@4\n"\
	"LABEL _chr_cont_correct_type\n"\
	"LT LF@isHigher LF@param1 LF@top\n"\
	"JUMPIFEQ _chr_cont_if_lower LF@isHigher bool@true\n"\
	"EXIT int@4\n"\
	"LABEL _chr_cont_if_lower\n"\
	"GT LF@isLower LF@param1 LF@bottom\n"\
	"JUMPIFEQ _chr_cont_if_higher LF@isLower bool@true\n"\
	"EXIT int@4\n"\
	"LABEL _chr_cont_if_higher\n"\
	"INT2CHAR LF@%return LF@param1\n"\
	"POPFRAME\n"\
	"RETURN\n"\

#define LEN \
	"LABEL _len\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@%return\n"\
	"DEFVAR LF@param1\n"\
	"DEFVAR LF@type\n"\
	"DEFVAR LF@exp_type\n"\
	"MOVE LF@param1 LF@%1\n"\
	"MOVE LF@exp_type string@string\n"\
	"TYPE LF@type LF@param1\n"\
	"JUMPIFEQ _len_continue LF@type LF@exp_type\n"\
	"EXIT int@4\n"\
	"LABEL _len_continue\n"\
	"STRLEN LF@%return LF@param1\n" \
	"POPFRAME\n" \
	"RETURN\n"\

#define ORD \
	"LABEL _ord\n"\
	"PUSHFRAME\n"\
	"DEFVAR LF@%return\n"\
	"DEFVAR LF@param1\n"\
	"DEFVAR LF@param2\n"\
	"DEFVAR LF@type\n"\
	"DEFVAR LF@length\n"\
	"DEFVAR LF@isHigher\n"\
	"DEFVAR LF@exp_type\n"\
	"DEFVAR LF@isLower\n"\
	"DEFVAR LF@top\n"\
	"DEFVAR LF@bottom\n"\
	"MOVE LF@param1 LF@%1\n"\
	"MOVE LF@param2 LF@%2\n"\
	"MOVE LF@bottom int@-1\n"\
	"MOVE LF@exp_type string@int\n"\
	"TYPE LF@type LF@param2\n"\
	"JUMPIFEQ _ord_cont LF@type LF@exp_type\n"\
	"EXIT int@4\n"\
	"LABEL _ord_cont\n"\
	"CREATEFRAME\n"\
	"DEFVAR TF@%1\n"\
	"MOVE TF@%1 LF@param1\n"\
	"CALL _len\n"\
	"MOVE LF@length TF@%return\n"\
	"MOVE LF@top LF@length\n"\
	"GT LF@isLower LF@param2 LF@bottom\n"\
	"JUMPIFNEQ _ord_end LF@isLower bool@true\n"\
	"LT LF@isHigher LF@param2 LF@top\n"\
	"JUMPIFNEQ _ord_end LF@isHigher bool@true\n"\
	"STRI2INT LF@%return LF@param1 LF@param2\n"\
	"POPFRAME\n"\
	"RETURN\n"\
	"LABEL _ord_end\n"\
	"MOVE LF@%return nil@nil\n"\
	"POPFRAME\n"\
	"RETURN\n"\

#define SUBSTRING \
	"LABEL _substring\n"\

#define PRINT \
	"LABEL _print\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@param1\n"\
	"MOVE LF@param1 LF@%1\n"\
	"WRITE LF@param1\n" \
	"WRITE string@\\010\n" \
	"POPFRAME\n" \
	"RETURN\n" \

void generate_build_in_functions();

void generate_header();

void start_generating();

void generate_main_header();

void generate_main_footer();

void generate_store_return(char * variable);

void generate_function_start(char* function_name);

void generate_function_end(char* function_name);

void generate_call_for_function(char* function_name);

void generate_return_for_function(char* value);

void generate_function_pass_arg_stack(int tempName);

void generate_variable_declaration(char* variable);

void generate_function_createframe();

void generate_pops_variable(char* variable);

void generate_pass_arg_to_func(char* variable, Token_type type, int index);

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

void generate_pushs(char* variable, Token_type type);

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