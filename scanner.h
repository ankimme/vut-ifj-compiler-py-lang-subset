/*
 * @file scanner.h
 * @brief Hlavičkový soubor pro lexikální analyzátor
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date XX.XX.2019
 *
 * Projekt: Implementace prekladace imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
//#include "dynamic_string.h"

enum state
{
    start; //počáteční stav automatu

    /*  IDENTIFIKÁTOR  */
    IDENTIFIER; //koncový stav, typ indentifikátor

    /*  CELÉ ČÍSLO  */
    INTEGER; //koncový stav, typ celé číslo

    /*  DESETINNÝ LITERÁL  */
    decimal_point; //mezistav, desetinná čárka
    DOUBLE_1; //koncový stav, typ desetinný literál

    exponent; //mezistav, literál s exponentem
    plus_minus; //mezistav, literál s exponentem
    DOUBLE_2; //koncový stav, typ desetinný literál

    /*  ŘETĚZCOVÝ LITERÁL  */
    character; //mezistav, načítání znaků řetězce

    escape; //mezistav, znak pro možnou escape sekvenci

    hexadecimal_1; //mezistavy, escape sekvence pro platné hexadecimální číslo
    hexadecimal_2; 

    STRING_LITERAL; //koncový stav, typ řetězcový literál

    /*  KOMENTÁŘ  */
    commentary; //mezistav, komentář
    END_OF_LINE; //koncový stav, typ znaku konce řádku

    /*  RELAČNÍ OPERÁTOR  */
    exclamation_mark; //mezistav, vykřičník
    RELATION_OPERATOR_1; //koncové stavy, typ konkrétního relačního operátoru (<,>,=)
    RELATION_OPERATOR_2; //(<=,>=,==,!=)

    /*  BINÁRNÍ OPERÁTOR  */
    BINARY_OPERATOR_1; //koncové stavy, typ různých binárních operátorů (/)
    BINARY_OPERATOR_2; // (//)
    BINARY_OPERATOR_3; // (+,-,*)

    /*  DOKUMENTAČNÍ ŘETĚZEC  */
    quotation_mark_1; //mezistavy, sekvence 3 po sobě jdoucích znaků uvozovek (""")
    quotation_mark_2;
    quotation_mark_3; //mezistav, slouží k načítání dalších znaků
    slash; //mezistav, slouží k ošetření escape sekvence pro uvozovky (\")

    ending_quotation_1; //mezistavy, posloupnost 3 ukončujících uvotovek (""")
    ending_quotation_2;
    DOCUMENTARY_LITERAL; //koncový stav, nemá přiřazen žádný typ
}


typedef union
{
    Dynamic_string *string; //Zkontrolvat
    Keyword keyword; //Zkontrolvat
    int number;
    double decimal;

}Token_value

typedef struct
{
    Dynamic_string *type; //Zkontrolvat
    Token_value attribute;
}St_token;

void Get_next_token(St_token* Token);


#endif