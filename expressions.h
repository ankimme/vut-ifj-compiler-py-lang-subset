/*
 * @file expressions.h
 * @brief Zpracování výrazů
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 28.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "parser.h"
#include "precedent_stack.h"

#include <stdbool.h>
#include <stdlib.h>


/**
 *
 * @enum Prec_rule.
 * @brief Pravidla pro precedenční analýzu.
 */
typedef enum
{
    /*  Výraz s binárním/aritmetickým/řetězcovým operátorem  */
    RULE_SUM,       //E -> E + E            //0
    RULE_SUB,       //E -> E - E            //1
    RULE_MUL,       //E -> E * E            //2
    RULE_DIV_INT,   //E -> E // E           //3
    RULE_DIV_FLOAT, //E -> E / E            //4

    /*  Výraz v závorkách  */
    RULE_BRACKETS,  //E -> (E)              //5
    
    /*  Výraz s relačním operátorem  */
    RULE_L,         //E -> E < E            //6
    RULE_G,         //E -> E > E            //7
    RULE_LEQ,       //E -> E <= E           //8
    RULE_GEQ,       //E -> E >= E           //9
    RULE_EQ,        //E -> E == E           //10
    RULE_NEQ,       //E -> E != E           //11

    /*  Symbol s hodnotou  */
    RULE_VAL,       //E -> i                //12

}Prec_rule;

/**
 * Prochází vygenerovaný abstraktní syntaktický strom metodou post order a volá zpracování jednotlivých uzlů
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param node Odkaz na data kořenového uzlu
 */
void post_order_gen_code(tParser_data* parser_data, tNodeData *node);

/**
 * Zpracuje uzel abstraktního derivačního stromu a vygeneruje příslušné instrukce v jazyce ifj19code
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param node Odkaz na data zpracovávaný
 */
void process_node(tParser_data* parser_data, tNodeData *item);

/**
 * Vyčistí pomocný precedenční zásobník, nastaví flag unget_token, odstraní jeden kontext z tabulky symbolů, vypne flag zpracování if/while
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 */
void clean_resources(tParser_data* parser_data, tPrec_stack* prec_stack);

/**
 * Zkontroluje jestli je identifikátor proměnná a jestli je definovaná
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @return true v případě že je identifikátor platný, jinak false
 */

bool search_variable_in_symtable(tParser_data* parser_data);

/**
 * Provede zderivování pravidla bez nutnosti přetypování
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 * @param symb_rule Neterminál, který se vloží na zásobník a nahradí tím zderivované pravidlo
 * @param l_ptr Ukazatel na levý synovský uzel v abstraktním syntaktickém stromě
 * @param r_ptr Ukazatel na pravý synovský uzel v abstraktním syntaktickém stromě
 * @param next Ukazatel na symbol následující za derivovaným pravidlem
 * @param retype Značí zda je třeba přetypovat některý z operandů pravidla
 * @param type Typ symbolu na zásobníku
 * @param value_type Typ možné hodnoty indexu precedenční tabulky
 * @param str Konkrétní obsah symbolu na zásobníku
 * @return true v případě, že se zderivování pravidla zdaří, jinak false
 */

bool process_types_no_retype(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol symb_rule, tNodeData* l_ptr, tNodeData* r_ptr, tSymbol next, int retype, Prec_stack_symbol type, Token_type value_type, char* str);

/**
 * Provede zderivování pravidla s nutností přetypování jednoho z operandů
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 * @param symb_rule Neterminál, který se vloží na zásobník a nahradí tím zderivované pravidlo
 * @param l_ptr Ukazatel na levý synovský uzel v abstraktním syntaktickém stromě
 * @param r_ptr Ukazatel na pravý synovský uzel v abstraktním syntaktickém stromě
 * @param l_value Typ levého operandu, slouží k určení jaký ze dvou operandů je třeba pčetypovat
 * @param next Ukazatel na symbol následující za derivovaným pravidlem
 * @param retype Značí zda je třeba přetypovat některý z operandů pravidla
 * @param type Typ symbolu na zásobníku
 * @param value_type Typ možné hodnoty indexu precedenční tabulky
 * @param str Konkrétní obsah symbolu na zásobníku
 * @return true v případě, že se zderivování pravidla zdaří, jinak false
 */

bool process_types_one_retype(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol symb_rule, tNodeData* l_ptr, tNodeData* r_ptr, Token_type l_value, tSymbol next, int retype, Prec_stack_symbol type, Token_type value_type, char* str);

/**
 * Provede zderivování pravidla s nutností přetypování obou operandů
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 * @param symb_rule Neterminál, který se vloží na zásobník a nahradí tím zderivované pravidlo
 * @param l_ptr Ukazatel na levý synovský uzel v abstraktním syntaktickém stromě
 * @param r_ptr Ukazatel na pravý synovský uzel v abstraktním syntaktickém stromě
 * @param next Ukazatel na symbol následující za derivovaným pravidlem
 * @param retype Značí, že je třeba přetypovat oba operandy
 * @param type Typ symbolu na zásobníku
 * @param value_type Typ možné hodnoty indexu precedenční tabulky
 * @param str Konkrétní obsah symbolu na zásobníku
 * @return true v případě, že se zderivování pravidla zdaří, jinak false
 */

bool process_types_both_retype(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol symb_rule, tNodeData* l_ptr, tNodeData* r_ptr, tSymbol next, int retype, Prec_stack_symbol type, Token_type value_type, char* str);

/**
 * Provede hlubokou kopii zderivováného pravidla
 * 
 * @param destination Cílový symbol
 * @param next Ukazatel na následující symbol
 * @param retype Informace, zda má dojít k přetypování
 * @param type Typ symbolu na zásobníku
 * @param value_type Typ možné hodnoty indexu precedenční tabulky
 * @param str Konkrétní obsah symbolu na zásobníku
 * @return True pokud se kopie podaří, v případě neúspěchu false
 */

bool deep_copy_rule(tSymbol destination, tSymbol next, int retype, Prec_stack_symbol type, Prec_token value_type, char* str);

/**
 * Provede hlubokou kopii symbolu s hodnotou
 * 
 * @param source Zdrojový symbol
 * @param destination Cílový symbol
 * @return True pokud se kopie podaří, v případě neúspěchu false
 */


bool deep_copy_value(tSymbol source, tSymbol destination);

/**
 * Vytvoření uzlu do dílčí tabulky symbolů
 * 
 * @param symtable Uzazatel do tabulky symbolů
 * @param symbol Symbol, jehož obsah má být vložem do tabulky
 * @param lptr Ukazatel na levý synovský uzel k provázání
 * @param rptr Ukazatel na pravý synovský uzel k provázání
 * @return V případě úspěchu vrací ukazatel na záznam v tabulce, jinak NULL
 */

tNodeData* st_create_node(tSymtable *symtable, tSymbol symbol, tNodeData *lptr, tNodeData *rptr);

/**
 * Zderivování pravidla na zásobníku symbolů
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 * @param val Zderivované pravidlo uložené v neterminálu
 * @param lptr Ukazatel na levý synovský uzel k provázání
 * @param rptr Ukazatel na pravý synovský uzel k provázání
 * @return V případě úspěšného zderivování vrací true, jinak false
 */

bool derive_rule(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol val, tNodeData* l_ptr, tNodeData* r_ptr);

/**
 * Kontrola typů operandů u příchozího pravidla
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 * @param left_operand Hodnota levého operandu pravidla
 * @param operator Operátor značící typ pravidla
 * @param right_operand Hodnota pravého operandu pravidla
 * @return Pokud je typová kontrola v pořádku, vrací true, jinak false
 */

bool check_operands_type(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol left_operand, tSymbol operator, tSymbol right_operand);


/**
 * Vytvoření uzlu do dílčí tabulky symbolů
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param value Hodnota symbolu pro vložení záznamu do tabulky symbolů
 * @param backup Symbol sloužící k uložení ukazatele na nově vložený záznam
 * @return V případě zdárného vytvoření záznamu vrací true, jinak false
 */

bool semantic_node_value(tParser_data* parser_data, tSymbol value, tSymbol backup);

/**
 * Zjistí typ symbolu na vrcholu zásobníku a určí jeho index do precedenční tabulky
 * 
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 * @return Navrací index sloupce v tabulce
 */

Prec_token process_terminal(tPrec_stack* prec_stack);

/**
 * Zjistí typ tokenu na vstupu a určí jeho index do precedenční tabulky
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @return Navrací index řádku v tabulce
 */

Prec_token process_token(tParser_data* parser_data);

/**
 * Provede zpracování precedence tokenu na řádku a symbolu na zásobníku
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param prec_stack Ukazatel na strukturu tPrec_Stack
 * @param prec_term_type Index sloupce tabulky
 * @param prec_token_type Index řádku tabulky
 * @return Pokud existuje precedenční pravidlo, zpracuje a vrací true, jinak false
 */

bool process_prec_table(tParser_data* parser_data, tPrec_stack* prec_stack, Prec_token prec_term_type, Prec_token prec_token_type);

/**
 * Provede zpracování výrazu
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @return Pokud se jedná o platný výraz a nedošlo k chybě, vrací true, jinak false. 
 */

bool process_expression(tParser_data* parser_data);

#endif