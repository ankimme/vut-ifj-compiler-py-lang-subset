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

/**
 * @def HEADER
 * Vytiskne na stdout hlavičku IFJcode19
 */
#define HEADER \
	".IFJcode19\n"\
	"DEFVAR GF@return\n"\
	"DEFVAR GF@$concat_result\n"\
	"DEFVAR GF@$str1\n"\
	"DEFVAR GF@$str2\n"\
	"DEFVAR GF@$variable1\n"\
	"DEFVAR GF@$variable2\n"\
	"JUMP _main\n" \

/**
 * @def INPUTS
 * Vytiskne na stdout vestavěnou funkci INPUTS 
 */
#define INPUTS \
	"LABEL _inputs\n" \
	"PUSHFRAME\n"\
	"DEFVAR LF@%return\n"\
    "READ LF@%return string\n" \
	"POPFRAME\n"\
    "RETURN\n" \

/**
 * @def INPUTI
 * Vytiskne na stdout vestavěnou funkci INPUTI
 */
#define INPUTI \
	"LABEL _inputi\n" \
	"PUSHFRAME\n"\
	"DEFVAR LF@%return\n"\
	"READ LF@%return int\n" \
	"POPFRAME\n"\
	"RETURN\n" \

/**
 * @def INPUTF	 
 * Vytiskne na stdout vestavěnou funkci INPUTF
 */
#define INPUTF \
	"LABEL _inputf\n" \
	"PUSHFRAME\n"\
	"DEFVAR LF@%return\n"\
	"READ LF@%return float\n" \
	"POPFRAME\n"\
	"RETURN\n" \

/**
 * @def CHR	 
 * Vytiskne na stdout vestavěnou funkci CHR
 */
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
	"EXIT int@6\n"\
	"LABEL _chr_cont_if_lower\n"\
	"GT LF@isLower LF@param1 LF@bottom\n"\
	"JUMPIFEQ _chr_cont_if_higher LF@isLower bool@true\n"\
	"EXIT int@6\n"\
	"LABEL _chr_cont_if_higher\n"\
	"INT2CHAR LF@%return LF@param1\n"\
	"POPFRAME\n"\
	"RETURN\n"\

/**
 * @def LEN	 
 * Vytiskne na stdout vestavěnou funkci LEN
 */
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

/**
 * @def ORD	 
 * Vytiskne na stdout vestavěnou funkci ORD
 */
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

/**
 * @def PRINT
 * Vytiskne na stdout vestavěnou funkci PRINT
 */
#define PRINT \
	"LABEL _print\n" \
	"PUSHFRAME\n" \
	"DEFVAR LF@param1\n"\
	"MOVE LF@param1 LF@%1\n"\
	"WRITE LF@param1\n" \
	"WRITE string@\\010\n" \
	"POPFRAME\n" \
	"RETURN\n" \

/**
 * Vytiskne na stdout všechny věstavené funkce
 */
void generate_build_in_functions();

/**
 * Vytiskne na stdout hlavičku IFJcode19
 */
void generate_header();

/**
 * Vytiskne na stdout začátek funkce main
 */
void generate_main_header();

/**
 * Vytiskne na stdout konec funkce main
 */
void generate_main_footer();

/**
 * Vytiskne na stdout vytvoření dočasného rámce
 */
void generate_function_createframe(); 

/**
 * Provede základní inicalizaci generátor
 * Zavolá funkce pro výtisk věstavěných funkcí, hlavičky IFJcode19 a začátku funkce main
 */
void start_generating();

/**
 * Vytiskne na stdout volání funkce 
 * @param function_name název funkce
 */
void generate_call_for_function(char* function_name);

/**
 * Vytiskne na stdout získání návratové hodnoty z funkce
 * @param variable název proměnné, do které se ukládá hodnota vrácená z funkce
 */
void generate_store_return(char * variable);

/**
 * Vytiskne na stdout deklaraci globální proměnné 
 * @param variable název deklarované proměnné
 */
void generate_variable_declaration(char* variable);

/**
 * Vytiskne na stdout vyjmutí hodnoty ze zásobníku
 * @param variable název proměnné do které se hodnota uloží
 */
void generate_pops_variable(char* variable);

/**
 * Vytiskne na stdout předávání parametrů funkci
 * @param variable argument, který se předává funkci
 * @param type datový typ argumentu předávaný tokenem
 * @param index specifické označení pro dočasný rámec
 */
void generate_pass_arg_to_func(char* variable, Token_type type, int index);

/**
 * Vytiskne na stdout vložení bool s hodnotou false na zásobník
 */
void generate_false_on_stack();

/**
 * Vytiskne na stdout konvertování int na float
 * @param value_in_register specifické označení pro dočasný rámec
 */
void conv_int_to_float(char* value_in_register);

/**
 * Vytiskne na stdout zásobníkovou verzi konvertování int na float
 */
void conv_int_to_float_stack();

/**
 * Vytiskne na stdout konkatenaci dvou řetězců
 */
void generate_concat();

/**
 * Vytiskne na stdout vložení hodnoty na zásobník
 * @param variable název proměnné, jejíž hodnota se vloží na zásobník
 * @param type datový typ argumentu předávaný tokenem
 */
void generate_pushs(char* variable, Token_type type);

/**
 * Vytiskne na stdout sčítání dvou hodnot ze zásobníku
 */
void generate_adds();

/**
 * Vytiskne na stdout odečítání dvou hodnot ze zásobníku
 */
void generate_subs();

/**
 * Vytiskne na stdout násobení dvou hodnot ze zásobníku
 */
void generate_muls();

/**
 * Vytiskne na stdout dělení dvou hodnot ze zásobníku
 */
void generate_divs();

/**
 * Vytiskne na stdout celočíselné dělení dvou hodnot ze zásobníku
 */
void generate_idivs();

/**
 * Vytiskne na stdout porovnání pokud jsou dvě hodnoty různého datového typu shodné
 */
void generate_EQS();

/**
 * Vytiskne na stdout porovnání datového typu hodnoty ze zásobníku
 */
void generate_compare();

/**
 * Vytiskne na stdout porovnání pokud jsou dvě hodnoty různého datového typu odlišné
 */
void generate_NEQS();

/**
 * Vytiskne na stdout porovnání pokud jsou dvě hodnoty stejného datového typu shodné
 */
void generate_same_type_EQS();

/**
 * Vytiskne na stdout porovnání pokud jsou dvě hodnoty stejného datového typu odlišné
 */
void generate_same_type_NEQS();

/**
 * Vytiskne na stdout relační operátor větší než
 */
void generate_GTS();

/**
 * Vytiskne na stdout relační operátor menší než
 */
void generate_LTS();

/**
 * Vytiskne na stdout relační operátor menší nebo rovno
 */
void generate_LTEQ();

/**
 * Vytiskne na stdout relační operátor větší nebo rovno
 */
void generate_GTEQ();

/**
 * Vytiskne na stdout návěští
 * @param label název návěští
 */
void generate_label(char* label);

/**
 * Vytiskne na stdout skok na návěští
 * @param label název návěští
 */
void generate_jump(char* label);

/**
 * Vytiskne na stdout podmíněný skok na návěští
 * @param label název návěští
 */
void generate_jump_if_equals_stack(char* label);

/**
 * Vytiskne na stdout generace unikátního návěští
 * @param label pole znaků, které musí být délky 10
 */
void st_generate_random_label_name(char *label);

#endif