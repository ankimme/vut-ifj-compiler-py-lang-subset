/*
 * @file gen_code.c
 * @brief Generování výstupního kódu
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 04.12.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "gen_code.h"

void generate_build_in_functions() 
{
	printf("%s", INPUTS);
	printf("%s", INPUTI);
	printf("%s", INPUTF);
	printf("%s", CHR);
	printf("%s", LEN);
	printf("%s", PRINT);
	printf("%s", ORD);
//	printf("%s", SUBSTRING);
}

void generate_header() 
{
	printf("%s", HEADER);
}

void generate_main_header()
{
	printf("%s","LABEL _main\n");
	printf("%s","CREATEFRAME\n");
	printf("%s","PUSHFRAME\n");
}

void generate_main_footer() 
{
	printf("%s", "POPFRAME\n");
	printf("%s", "CLEARS\n");
}

void generate_function_createframe() 
{
	printf("%s", "CREATEFRAME\n");
}

void generate_function_pass_arg_stack(int tempName)
{
	printf("%s", "DEFVAR TF@");
	printf("%d", tempName);
	printf("%s", "\n");

	printf("%s", "POPS TF@");
	printf("%d", tempName);
	printf("%s", "\n");

	/*printf("%s", "MOVE TF@");
	printf("%d", tempName);
	printf("%s", " ");
	printf("%s", param);
	printf("%s", "\n");*/
}

void start_generating()
{
	generate_header();
	generate_build_in_functions();
	generate_main_header();
}

void generate_function_start(char* function_name)
{
	printf("%s", "LABEL _");
	printf("%s", function_name);
	printf("%s", "\n");
	printf("%s", "PUSHFRAME\n");
}

void generate_function_end(char* function_name)
{
	printf("%s", "LABEL _");
	printf("%s", function_name);
	printf("%s", "return");
	printf("%s", "\n");
	printf("%s", "RETURN\n");
}

void generate_call_for_function(char* function_name)
{
	printf("%s", "CALL _");
	printf("%s", function_name);
	printf("%s", "\n");
}

void generate_store_return(char * variable)
{
	printf("%s","MOVE ");
	printf("%s","GF@");
	printf("%s",variable);
	printf("%s"," TF@%return");
	printf("%s", "\n");
}

void generate_variable_declaration(char* variable)
{
	printf("%s", "DEFVAR GF@");
	printf("%s", variable);
	printf("%s", "\n");
}

void generate_pops_variable(char* variable)
{
	printf("%s", "POPS GF@");
	printf("%s", variable);
	printf("%s", "\n");

	// printf("%s", "WRITE GF@"); // todo delete
	// printf("%s", variable); // todo delete
	// printf("%s", "\n"); // todo delete
}

void generate_pass_arg_to_func(char* variable, Token_type type, int index)
{
	float getval;
	
	printf("%s", "DEFVAR TF@%");
	printf("%d", index);
	printf("%s", "\n");


	printf("%s", "MOVE TF@%");
	printf("%d", index);
	printf("%s", " ");

	switch (type)
	{
		case TOKEN_STRING_LITERAL:
			printf("%s", "string@");
			printf("%s", variable);
			break;
		case TOKEN_INTEGER:
			printf("%s", "int@");
			printf("%s", variable);
			break;
		case TOKEN_DOUBLE:
			getval = atof(variable);
			sprintf(variable,"%a", getval);
			printf("%s", "float@");
			printf("%s", variable);
			break;
		case TOKEN_KEYWORD:
			printf("%s", "nil@nil");
			break;
		case TOKEN_IDENTIFIER:
			printf("%s", "GF@");
			printf("%s", variable);
			break;
		default:
			// raise error
			break;
	}
	printf("%s", "\n");
}

void generate_func_variables(char* name, char*value)
{
	generate_variable_declaration(name);
	printf("%s", "MOVE LF@");
	printf("%s", name);
	printf("%s", "LF@");
	printf("%s", value);
	printf("%s", "\n");
}

void generate_label(char* label)
{
	printf("%s", "LABEL _");
	printf("%s", label);
	printf("%s","\n");
}

void conv_int_to_float(char* value_in_register)
{
	printf("%s", "INT2FLOAT TF@");
	printf("%s", value_in_register);
	printf("%s", " TF@");
	printf("%s", value_in_register);
	printf("%s", "\n");
}

void conv_float_to_int(char* value_in_register)
{
	printf("%s", "FLOAT2INT TF@");
	printf("%s", value_in_register);
	printf("%s", " TF@");
	printf("%s", value_in_register);
	printf("%s", "\n");
}

void conv_int_to_float_stack()
{
	printf("%s", "INT2FLOATS\n");
}

void conv_float_to_int_stack()
{
	printf("%s", "FLOAT2INTS\n");
}

void generate_concat()
{
	printf("%s", "POPS GF@$str1\n");
	printf("%s", "POPS GF@$str2\n");
	printf("%s", "CONCAT GF@$concat_result GF@$str2 GF@$str1\n");
	printf("%s", "PUSHS GF@$concat_result\n");
}

void generate_pushs(char* variable, Token_type type)
{
	float getval;

	switch (type)
	{
		case TOKEN_STRING_LITERAL:
			printf("%s", "PUSHS string@");
			printf("%s", variable);
			break;
		case TOKEN_INTEGER:
			printf("%s", "PUSHS int@");
			printf("%s", variable);
			break;
		case TOKEN_DOUBLE:
			getval = atof(variable);
			sprintf(variable,"%a", getval);
			printf("%s", "PUSHS float@");
			printf("%s", variable);
			break;
		case TOKEN_KEYWORD:
			printf("%s", "PUSHS nil@nil");
			break;
		case TOKEN_IDENTIFIER:
			printf("%s", "PUSHS GF@");
			printf("%s", variable);
			break;
		default:
			// raise error
			break;
	}
	printf("%s", "\n");
}

void generate_pops()
{
	printf("%s", "POPS  GF@return\n");
}

void generate_adds()
{
	printf("%s", "ADDS\n");
}

void generate_subs()
{
	printf("%s", "SUBS\n");
}

void generate_muls()
{
	printf("%s", "MULS\n");
}

void generate_divs()
{
	
	printf("%s", "POPS GF@$variable1\n");
	printf("%s", "JUMPIFNEQ _not_zero GF@$variable1 float@0x0p+0\n");
	printf("%s", "EXIT int@9\n");
	printf("%s", "LABEL _not_zero\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "DIVS\n");
}

void generate_idivs()
{
	printf("%s", "POPS GF@$variable1\n");
	printf("%s", "JUMPIFNEQ _not_zero GF@$variable1 int@0\n");
	printf("%s", "EXIT int@9\n");
	printf("%s", "LABEL _not_zero\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "IDIVS\n");
}

void generate_EQS()
{
	printf("%s", "EQS\n");
}

void generate_GTS()
{
	printf("%s", "GTS\n");
}

void generate_LTS()
{
	printf("%s", "LTS\n");
}

void generate_LTEQ()
{
	printf("%s","POPS GF@$variable1\n");
	printf("%s","POPS GF@$variable2\n");
	printf("%s","PUSHS GF@$variable2\n");
	printf("%s","PUSHS GF@$variable1\n");
	printf("LTS\n");
	printf("%s","PUSHS GF@$variable1\n");
	printf("%s","PUSHS GF@$variable2\n");
	printf("EQS\n");
	printf("ORS\n");
}

void generate_GTEQ()
{
	printf("%s","POPS GF@$variable1\n");
	printf("%s","POPS GF@$variable2\n");
	printf("%s","PUSHS GF@$variable2\n");
	printf("%s","PUSHS GF@$variable1\n");
	printf("GTS\n");
	printf("%s","PUSHS GF@$variable2\n");
	printf("%s","PUSHS GF@$variable1\n");
	printf("EQS\n");
	printf("ORS\n");
}

void if_statement(char* function)
{
	printf("%s", "JUMPIFEQ _");
	printf("%s", function);
	printf("%s", "GF@$result GF@$comp_variable");
	printf("%s", "\n");
}

void if_else_statement(char* function)
{
	printf("%s", "JUMP _");
	printf("%s", function);
	printf("%s", "\n");
}

void while_statement(char* function)
{
	printf("%s", "JUMPIFEQ _");
	printf("%s", function);
	printf("%s", "GF@$result GF@$comp_variable");
	printf("%s", "\n");
}

void while_end_statement(char* function)
{
	printf("%s", "JUMP _");
	printf("%s", function);
	printf("%s", "\n");
}