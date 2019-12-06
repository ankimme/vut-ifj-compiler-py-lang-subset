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

/**
 *
 * @enum Prec_rule.
 * @brief Pravidla pro precedenční analýzu.
 *
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

    /* */
    RULE_VAL,       //E -> i                //12

}Prec_rule;


void clean_resources(tParser_data* parser_data, tPrec_stack* prec_stack);

bool derive_rule(tParser_data* parser_data, tPrec_stack* prec_stack, Token_type value_type, char* string);

bool check_operands_type(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol left_operand, tSymbol operator, tSymbol right_operand);

bool semantic_node_value(tSymbol value);

bool semantic_node_rule(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol left_sym, tSymbol middle_sym, tSymbol right_sym);

Prec_token process_terminal(tPrec_stack* prec_stack);

Prec_token process_token(tParser_data* parser_data);

bool process_prec_table(tParser_data* parser_data, tPrec_stack* prec_stack, Prec_token prec_term_type, Prec_token prec_token_type);

bool process_expression(tParser_data* parser_data);

#endif