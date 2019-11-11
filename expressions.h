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

#include "scanner.h"
#include "symtable.h"

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
 * @enum Prec_stack_symbols.
 * @brief Typy symbolů na zásobníku precedenční analýzy
 *
 */

typedef enum
{
    EXPR_NONTERMINAL,
    EXPR_TERMINAL,
    EXPR_HANDLE,
    EXPR_UNDEFINED,
}Prec_stack_symbols;


void process_expression();


#endif