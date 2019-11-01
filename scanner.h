/*
 * @file scanner.h
 * @brief Hlavičkový soubor pro lexikální analyzátor
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 1.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef SCANNER_H
#define SCANNER_H

#include "dynamic_string.h"
#include "errors.h"

#include <stdio.h>
#include <stdlib.h>

/**
 *
 * @enum State
 * @brief Stavy DKA
 *
 */

enum 
{
    start, //počáteční stav automatu

    /*  IDENTIFIKÁTOR  */
    IDENTIFIER, //koncový stav, typ indentifikátor

    /*  CELÉ ČÍSLO  */
    INTEGER, //koncový stav, typ celé číslo

    /*  DESETINNÝ LITERÁL  */
    decimal_point, //mezistav, desetinná čárka
    DOUBLE_1, //koncový stav, typ desetinný literál

    exponent, //mezistav, literál s exponentem
    plus_minus, //mezistav, literál s exponentem
    DOUBLE_2, //koncový stav, typ desetinný literál

    /*  ŘETĚZCOVÝ LITERÁL  */
    character, //mezistav, načítání znaků řetězce
    
    escape, //mezistav, znak pro možnou escape sekvenci

    hexadecimal_1, //mezistavy, escape sekvence pro platné hexadecimální číslo
    hexadecimal_2, 

    STRING_LITERAL, //koncový stav, typ řetězcový literál

    /*  KOMENTÁŘ  */
    commentary, //mezistav, komentář
    END_OF_LINE, //koncový stav, typ znaku konce řádku

    /*  RELAČNÍ OPERÁTOR  */
    exclamation_mark, //mezistav, vykřičník
    RELATION_OPERATOR_1, //koncové stavy, typ konkrétního relačního operátoru (<,>,=)
    RELATION_OPERATOR_2, //(<=,>=,==,!=)

    /*  BINÁRNÍ OPERÁTOR  */
    BINARY_OPERATOR_1, //koncové stavy, typ různých binárních operátorů (/)
    BINARY_OPERATOR_2, // (//)
    BINARY_OPERATOR_3, // (+,-,*)

    /*  DOKUMENTAČNÍ ŘETĚZEC  */
    quotation_mark_1, //mezistavy, sekvence 3 po sobě jdoucích znaků uvozovek (""")
    quotation_mark_2,
    quotation_mark_3, //mezistav, slouží k načítání dalších znaků
    backslash, //mezistav, slouží k ošetření escape sekvence pro uvozovky (\")

    ending_quotation_1, //mezistavy, posloupnost 3 ukončujících uvotovek (""")
    ending_quotation_2,
    DOCUMENTARY_LITERAL, //koncový stav, nemá přiřazen žádný typ
}State;


/**
 *
 * @union Token_value
 * @brief Druhy atributu tokenu
 *
 */

typedef union 
{
    dynamic_string *string;
    int number;
    double decimal;
}Token_value;

/**
 *
 * @struct St_token
 * @brief Definování tokenu
 *
 */

typedef struct Token
{
    dynamic_string *type; //Zkontrolovat
    Token_value attribute;
    int error_value;
}St_token;



void get_next_token(St_token* Token);

int clean_all(Errors err, dynamic_string *str);

int is_keyword(dynamic_string *str);

#endif