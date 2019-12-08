/*
 * @file gen_code.c
 * @brief Generování výstupního kódu
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 04.12.2019
 *
 * Projekt: Implementace pøekladaèe imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "gen_code.h"

void generate_build_in_functions() 
{
//	printf("%s", INPUTS);
//	printf("%s", INPUTI);
//	printf("%s", INPUTF);
	//printf("%s", CHAR);
//	printf("%s", LENGTH);
	printf("%s", PRINT);
	//printf("%s", ORD);
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

void generate_function_arg() 
{
	printf("%s", "CREATEFRAME\n");
}

void generate_function_pass_arg(char* param, int functionID)
{
	printf("%s", "DEFVAR TF@");
	printf("%d", functionID);
	printf("%s", "\n");
	printf("%s", "MOVE TF@");
	printf("%d", functionID);
	printf("%s", " ");
	printf("%s", param);
	printf("%s", "\n");
}

void start_generating()
{
	generate_header();
	generate_main_header();

	generate_function_arg();
	generate_function_pass_arg("AAA", 1);
	generate_build_in_functions();
	generate_main_footer();
}


void generate_function_start(char* function_name)
{
	printf("%s", "LABEL _");
	printf("%s", function_name);
	printf("%s", "\n");
	printf("%s", "PUSHFRAME");
}

void generate_function_end(char* function_name)
{
	printf("%s", "LABEL _");
	printf("%s", function_name);
	printf("%s", "return");
	printf("%s", "\n");
	printf("%s", "POPFRAME");
	printf("%s", "RETURN");
}

void generate_call_for_function(char* function_name)
{
	printf("%s", "CALL _");
	printf("%s", function_name);
	printf("%s", "\n");
}

void generate_return_for_function(char* value)
{
	printf("%s", "DEFVAR LF@return");
}

void generate_variable_declaration(char* variable)
{
	printf("%s", "DEFVAR LF@");
	printf("%s", variable);
	printf("%s", "\n");
}

void generate_value_type(int type)
{
	switch (type)
	{
	case 1:
		printf("%s", "int@");
		printf("%s", "\n");
		return;
	case 2:
		printf("%s", "float@");
		printf("%s", "\n");
		return;
	case 3:
		printf("%s", "string@");
		printf("%s", "\n");
		return;
	}
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
	printf("%s", "INT2FLOATS");
}

void conv_float_to_int_stack()
{
	printf("%s", "FLOAT2INTS");
}

void conv_int_to_float_stack_2()
{
	printf("%s", "POPS GF@variable1");
	printf("%s", "INT2FLOATS");
	printf("%s", "PUSHS GF@variable1");
}

void conv_float_to_int_stack_2()
{
	printf("%s", "POPS GF@variable1");
	printf("%s", "FLOAT2INTS");
	printf("%s", "PUSHS GF@variable1");
}

void generate_concat()
{
	printf("%s", "POPS GF@variable1");
	printf("%s", "POPS GF@variable2");
	printf("%s", "CONCAT GF@result GF@variable2 GF@variable1");
	printf("%s", "PUSHS GF@result");
}

void generate_print()
{
	printf("%s", "WRITE GF@variable");
}

void generate_pushs(char* variable)
{
	printf("%s", "PUSHS ");
	printf("%s", variable);
	printf("%s", "\n");
}

void generate_pops()
{
	printf("%s", "POPS  GF@return");
}

void generate_adds()
{
	printf("%s", "ADDS");
}

void generate_subs()
{
	printf("%s", "SUBS");
}

void generate_muls()
{
	printf("%s", "MULS");
}

void generate_divs()
{
	printf("%s", "DIVS");
}

void generate_idivs()
{
	printf("%s", "POPS GF@variable1");
	printf("%s", "INT2FLOATS");
	printf("%s", "PUSHS GF@variable2");
	printf("%s", "INT2FLOATS");
	printf("%s", "DIVS");
	printf("%s", "FLOAT2INTS");
}

void generate_EQS()
{
	printf("%s", "EQS");
}

void generate_GTS()
{
	printf("%s", "GTS");
}

void generate_LTS()
{
	printf("%s", "LTS");
}

void if_statement(char* function)
{
	printf("%s", "JUMPIFEQ _");
	printf("%s", function);
	printf("%s", "GF@result GF@comp_variable");
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
	printf("%s", "GF@result GF@comp_variable");
	printf("%s", "\n");
}

void while_end_statement(char* function)
{
	printf("%s", "JUMP _");
	printf("%s", function);
	printf("%s", "\n");
}