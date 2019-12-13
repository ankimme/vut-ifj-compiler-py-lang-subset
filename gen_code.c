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

void start_generating()
{
	generate_header();
	generate_build_in_functions();
	generate_main_header();
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
			break;
	}
	printf("%s", "\n");
}

void generate_false_on_stack()
{
	printf("%s", "PUSHS bool@false\n");
}

void conv_int_to_float(char* value_in_register)
{
	printf("%s", "INT2FLOAT TF@");
	printf("%s", value_in_register);
	printf("%s", " TF@");
	printf("%s", value_in_register);
	printf("%s", "\n");
}

void conv_int_to_float_stack()
{
	printf("%s", "INT2FLOATS\n");
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
			break;
	}
	printf("%s", "\n");
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
	char arr[10] = "";
	st_generate_random_label_name(&(arr[0]));
	printf("%s", "JUMPIFNEQ _");
	printf("%s", arr);
	printf("%s", " GF@$variable1 float@0x0p+0\n");
	printf("%s", "EXIT int@9\n");
	printf("%s", "LABEL _");
	printf("%s", arr);
	printf("%s", "\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "DIVS\n");
}

void generate_idivs()
{
	printf("%s", "POPS GF@$variable1\n");
	char arr[10] = "";
	st_generate_random_label_name(&(arr[0]));
	printf("%s", "JUMPIFNEQ _");
	printf("%s", arr);
	printf("%s", " GF@$variable1 int@0\n");
	printf("%s", "EXIT int@9\n");
	printf("%s", "LABEL _");
	printf("%s", arr);
	printf("%s", "\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "IDIVS\n");
}

void generate_EQS()
{
	printf("%s", "POPS GF@$variable1\n");
	printf("%s", "POPS GF@$variable2\n");

	printf("%s", "DEFVAR LF@type_1\n");
	printf("%s", "DEFVAR LF@type_2\n");

	printf("%s","TYPE LF@type_1 GF@$variable1\n");
	printf("%s","TYPE LF@type_2 GF@$variable2\n");

	printf("%s","PUSHS LF@type_1\n");
	printf("%s","PUSHS LF@type_2\n");

	printf("%s", "EQS\n");
}

void generate_compare()
{

	
	printf("%s", "POPS GF@$variable1\n");

	printf("%s", "DEFVAR LF@exp_type_1\n");
	printf("%s", "DEFVAR LF@exp_type_2\n");
	printf("%s", "DEFVAR LF@exp_type_3\n");
	printf("%s", "DEFVAR LF@exp_type_4\n");
	printf("%s", "DEFVAR LF@type\n");
	

	printf("%s", "TYPE LF@type GF@$variable1\n");
	printf("%s", "MOVE LF@exp_type_1 int@0\n");
	printf("%s", "MOVE LF@exp_type_2 string@\n");
	printf("%s", "MOVE LF@exp_type_3 nil@nil\n");
	printf("%s", "MOVE LF@exp_type_4 float@0x0p+0\n");

	char arr[10] = "";
	st_generate_random_label_name(&(arr[0]));

	printf("%s", "JUMPIFNEQ _");
	printf("%s", arr);
	printf("%s", " LF@type string@int\n");

	printf("%s","CLEARS\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "PUSHS LF@exp_type_1\n");

	printf("%s", "EQS\n");
	printf("%s", "NOTS\n");

	printf("%s", "LABEL _");
	printf("%s", arr);
	printf("%s", "\n");

	st_generate_random_label_name(&(arr[0]));

	printf("%s", "JUMPIFNEQ _");
	printf("%s", arr);
	printf("%s", " LF@type string@string\n");

	printf("%s","CLEARS\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "PUSHS LF@exp_type_2\n");

	printf("%s", "EQS\n");
	printf("%s", "NOTS\n");

	printf("%s", "LABEL _");
	printf("%s", arr);
	printf("%s", "\n");

	st_generate_random_label_name(&(arr[0]));

	printf("%s", "JUMPIFNEQ _");
	printf("%s", arr);
	printf("%s", " LF@type string@nil\n");

	printf("%s","CLEARS\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "PUSHS LF@exp_type_3\n");
	printf("%s", "EQS\n");
	printf("%s", "NOTS\n");

	printf("%s", "LABEL _");
	printf("%s", arr);
	printf("%s", "\n");

	st_generate_random_label_name(&(arr[0]));

	printf("%s", "JUMPIFNEQ _");
	printf("%s", arr);
	printf("%s", " LF@type string@float\n");

	printf("%s","CLEARS\n");
	printf("%s", "PUSHS GF@$variable1\n");
	printf("%s", "PUSHS LF@exp_type_4\n");
	printf("%s", "EQS\n");
	printf("%s", "NOTS\n");

	printf("%s", "LABEL _");
	printf("%s", arr);
	printf("%s", "\n");
}

void generate_NEQS()
{
	printf("%s", "POPS GF@$variable1\n");
	printf("%s", "POPS GF@$variable2\n");

	printf("%s", "DEFVAR LF@type_1\n");
	printf("%s", "DEFVAR LF@type_2\n");

	printf("%s","TYPE LF@type_1 GF@$variable1\n");
	printf("%s","TYPE LF@type_2 GF@$variable2\n");

	printf("%s","PUSHS LF@type_1\n");
	printf("%s","PUSHS LF@type_2\n");

	printf("%s", "EQS\n");
	printf("%s", "NOTS\n");
}

void generate_same_type_EQS()
{
	printf("%s", "EQS\n");
}

void generate_same_type_NEQS()
{
	printf("%s", "EQS\n");
	printf("%s", "NOTS\n");
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
	printf("GTS\n");
	printf("NOTS\n");
}

void generate_GTEQ()
{
	printf("LTS\n");
	printf("NOTS\n");
}

void generate_label(char* label)
{
	printf("%s", "LABEL _");
	printf("%s", label);
	printf("%s","\n");
}

void generate_jump(char* label)
{
	printf("%s", "JUMP _");
	printf("%s", label);
	printf("%s", "\n");
}

void generate_jump_if_equals_stack(char* label)
{
	printf("%s", "JUMPIFEQS _");
	printf("%s", label);
	printf("%s", "\n");
}

void st_generate_random_label_name(char *label)
{
	int label_len = 10;
    const char symbols[] = "abcdefghijklmnopqrstuvwxyz0123456789$";
    int symbol_count = strlen(symbols);

    for (int i = 0; i < label_len - 1; i++)
    {
        label[i] = symbols[rand() % symbol_count];
    }
	label[label_len - 1] = '\0';
}