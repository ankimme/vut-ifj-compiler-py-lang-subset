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

#include <stdbool.h>

/**
 *
 * @enum Prec_rule.
 * @brief Pravidla pro precedenční analýzu.
 *
 */

typedef enum
{
    /*  Výraz s binárním/aritmetickým/řetězcovým operátorem  */
    RULE_SUM,       //E -> E + E
    RULE_SUB,       //E -> E - E
    RULE_MUL,       //E -> E * E
    RULE_DIV_INT,   //E -> E // E
    RULE_DIV_FLOAT, //E -> E / E

    /*  Výraz v závorkách  */
    RULE_BRACKETS,  //E -> (E)
    
    /*  Výraz s relačním operátorem  */
    RULE_L,         //E -> E < E
    RULE_G,         //E -> E > E
    RULE_LEQ,       //E -> E <= E
    RULE_GEQ,       //E -> E >= E
    RULE_EQ,        //E -> E == E
    RULE_NEQ,       //E -> E != E

    /*  Hodnota výrazu  */
    RULE_VALUE,     //E -> value
}Prec_rule;

/**
 *
 * @enum Prec_stack_symbol.
 * @brief Typy symbolů na zásobníku precedenční analýzy.
 *
 */

typedef enum
{
    SYMBOL_NONTERMINAL,
    SYMBOL_TERMINAL,
    SYMBOL_HANDLE,
    SYMBOL_UNDEFINED,
}Prec_stack_symbol;


typedef enum
{
   PREC_PLUS_MINUS,     // +-
   PREC_MUL_DIV_FLOAT,  // */
   PREC_DIV_INT,        // //
   PREC_L_BRACKET,      // (
   PREC_R_BRACKET,      // )
   PREC_RELATION_OP,    // RELATION_OP
   PREC_VALUE,          // VALUE

   PREC_OTHER,          // $
}Prec_token;

/**
 *
 * @enum Prec_derive.
 * @brief Počet symbolů potřebných k derivování aktuálního pravidla.
 *
 */

typedef enum
{
    DERIVATION_VALUE = 1,   //E -> value
    DERIVATION_RULE = 3,    //E -> E OP E (OP značí operátor) || (E)
}Prec_derivation;



//void clean_resources(tParser_data* parser_data, Errors err, tPrec_stack* prec_stack);
Prec_token process_token(tParser_data* parser_data);

bool process_expression(tParser_data* parser_data);

#endif