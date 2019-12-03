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
	string_init(gen_code_string);
	generate_header();
	generate_build_in_functions();

}

void start_clearing_generator()
{
	string_free(gen_code_string);
}

void print_code()
{
	printf("%s", gen_code_string->str);
}

void generate_main_header()
{
	PRINT_INSTRUCTION("LABEL _main");
	PRINT_INSTRUCTION("CREATEFRAME");
	PRINT_INSTRUCTION("PUSHFRAME");
}

void generate_main_footer() 
{
	PRINT_INSTRUCTION("POPFRAME");
	PRINT_INSTRUCTION("CLEARS");

}


void generate_function_start(char * function_name)
{
	PRINT_CODE("LABEL ");
	PRINT_CODE(function_name);
	PRINT_CODE("\n");
	PRINT_INSTRUCTION("PUSHFRAME");


}

void generate_function_end(char* function_name)
{
	PRINT_CODE("LABEL ");
	PRINT_CODE(function_name);
	PRINT_CODE("return");
	PRINT_CODE("\n");
	PRINT_CODE("POPFRAME");
	PRINT_CODE("RETURN");


}

void generate_call_for_function(char* function_name)
{

	PRINT_CODE("CALL ");
	PRINT_CODE(function_name);
	PRINT_CODE("\n");


}

void generate_return_for_function(char* type)
{
	PRINT_INSTRUCTION("DEFVAR LF@return");
	//neco tu bude jeste
	PRINT_CODE("\n");

}

void generate_variable_declaration(char* variable)
{
	PRINT_INSTRUCTION("DEFVAR LF@");
	//
	PRINT_CODE("\n");

}

void generate_label(char* label)
{
	PRINT_CODE("LABEL ");
	PRINT_CODE(label);
}

void generate_expr_if() 
{
	PRINT_CODE("JUMPIF");

}

void generate_value(int type)
{

	switch (type)
	{
		case 1:
			PRINT_CODE("int@");
			PRINT_CODE("\n");
			return;

		case 2:
			PRINT_CODE("float@");
			PRINT_CODE("\n");
			return;

		case 3:
			PRINT_CODE("string@");
			PRINT_CODE("\n");
			return;

	}

}

void generate_concat() 
{
	PRINT_INSTRUCTION("POPS GF@tmp2");
	PRINT_INSTRUCTION("POPS GF@tmp1");
	PRINT_INSTRUCTION("CONCAT GF@result GF@temp1 GF@temp2");
	PRINT_INSTRUCTION("PUSHS GF@result");
}

void generate_adds() 
{
	PRINT_INSTRUCTION("ADDS");
}

void generate_subs() 
{
	PRINT_INSTRUCTION("SUBS");
}

void generate_muls() 
{
	PRINT_INSTRUCTION("MULS");
}

void generate_divs() 
{
	PRINT_INSTRUCTION("DIVS");
}

void generate_idivs() 
{
	PRINT_INSTRUCTION("POPS GF@temp1");
	PRINT_INSTRUCTION("INT2FLOATS");
	PRINT_INSTRUCTION("PUSHS GF@temp2");
	PRINT_INSTRUCTION("INT2FLOATS");
	PRINT_INSTRUCTION("DIVS");
	PRINT_INSTRUCTION("FLOAT2INTS");


}

void generate_EQS() 
{
	PRINT_INSTRUCTION("EQS");
}

void generate_GTS()
{
	PRINT_INSTRUCTION("GTS");
}

void generate_LTS()
{
	PRINT_INSTRUCTION("LTS");
}

void generate_convert_to_double()
{
	PRINT_INSTRUCTION("INT2FLOATS");

}

void generate_convert_to_int()
{
	PRINT_INSTRUCTION("FLOAT2INT");
}

void generate_pushs(char* variable)
{
	PRINT_CODE("PUSHS ");
	PRINT_CODE(variable);
	PRINT_CODE("\n");
}

void generate_pops()
{
	PRINT_INSTRUCTION("POPS  GF@return");
}

void generate_if_statement(char *label)
{
	PRINT_CODE("JUMPIFEQ ");
	PRINT_CODE(label);
	PRINT_CODE("_else GF@%result bool@false\n");

}

