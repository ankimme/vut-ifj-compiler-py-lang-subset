/*
 * @file scanner.h
 * @brief Hlavičkový soubor pro lexikální analyzátor
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 7.11.2019
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
#include <errno.h>
#include <limits.h>

/**
 *
 * @enum State.
 * @brief Stavy DKA.
 * Stavy psané velkými písmeny značí koncový stav, jiné mezistav
 *
 */

typedef enum 
{
    start, //počáteční stav automatu

    /*  IDENTIFIKÁTOR  */
    IDENTIFIER, //koncový stav, typ identifikátor

    /*  CELÉ ČÍSLO  */
    INTEGER, //koncový stav, typ celé číslo (nenulové)
    ZERO, //koncový stav, typ celé číslo (nula)

    excess, //mezistav, slouží k načítání přebytečných nul (v případě, že značí zápis v jiné číselné soustavě, chyba)

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

    /*  KOMENTÁŘ  */
    commentary, //mezistav, komentář

    /*  RELAČNÍ OPERÁTOR  */
    exclamation_mark, //mezistav, vykřičník
    RELATION_OPERATOR,  //koncové stavy, typ konkrétního relačního operátoru (<,>,=)
                        //případně dalších při načtení následného znaku (<=,>=,==,!=)
    

    /*  BINÁRNÍ OPERÁTOR  */
    BINARY_OPERATOR, //koncový stavy, zpracování různých typů binárních operátorů

    /*  DOKUMENTAČNÍ ŘETĚZEC  */
    quotation_mark_1, //mezistavy, sekvence 3 po sobě jdoucích znaků uvozovek (""")
    quotation_mark_2,
    quotation_mark_3, //mezistav, slouží k načítání dalších znaků
    backslash, //mezistav, slouží k ošetření escape sekvence pro uvozovky (\")

    ending_quotation_1, //mezistavy, posloupnost 3 ukončujících uvotovek (""")
    ending_quotation_2,
    doc_string, //mezistav, načtený víceřádkový dokumentační řetězec, očekává se EOL
}State;


/**
 *
 * @enum Token_type.
 * @brief Typy tokenů.
 * Jednotlivé typy tokenů z lexikální analýzy.
 *
 */

typedef enum 
{
    TOKEN_INDENT,
    TOKEN_DEDENT,

    TOKEN_EOL,
    TOKEN_EOF,

    TOKEN_COMMA,
    TOKEN_COLON,

    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,

    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,

    TOKEN_INTEGER,
    TOKEN_DOUBLE,

    TOKEN_NOT_EQUAL,
    TOKEN_EQUAL,

    TOKEN_LESS_OR_EQUAL,
    TOKEN_GREATER_OR_EQUAL,

    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_ASSIGNMENT,

    TOKEN_DIVIDE_INTEGER,
    TOKEN_DIVIDE_FLOAT,

    TOKEN_STRING_LITERAL,
}Token_type;

/**
 *
 * @union Token_value.
 * @brief Druhy atributu tokenu.
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
 * @struct St_token.
 * @brief Definování tokenu.
 *
 */

typedef struct Token
{
    Token_type type;
    Token_value attribute;
    int error_value;
}St_token;

/**
 * Zpracování vstupního lexému na odpovídající token. 
 *
 * @param token Token pro uchování informace o zpracovaném lexému.
 * @post Token je naplněn hodnotami pro další vyhodnocení a zpracování.
 *
 */

void get_next_token(St_token* token);

/**
 * Uvolnění naalokované paměti a předání návratové hodnoty lexikální analýzy.
 *
 * @param err Návratová hodnota identifikující výsledek zpracování tokenu.
 * @param str Řetězec, kterému se uvolní pamět.
 * @return Návratová hodnota lexikální analýzy.
 *
 */

int clean_all(Errors err, dynamic_string *str);

/**
 * Rozpoznání klíčového slova od identifikátoru.
 *
 * @param str Řetězec pro kontrolu, zda se jedná o klíčové slovo.
 * @return Zda se jedná o klíčové slovo
 *
 */

int is_keyword(dynamic_string *string);

/**
 * Převod řetězce na celočíselný literál.
 *
 * @param str Řetězec pro převod
 * @param token Token pro uložení hodnoty převedeného celočíselného literálu
 * @post Token je naplněn hodnotami pro další vyhodnocení a zpracování.
 *
 */

void convert_to_integer(dynamic_string *str, St_token *token);

/**
 * Převod řetězce na desetinný literál.
 *
 * @param str Řetězec pro převod
 * @param token Token pro uložení hodnoty převedeného desetinného literálu
 * @post Token je naplněn hodnotami pro další vyhodnocení a zpracování.
 *
 */

void convert_to_double(dynamic_string *str, St_token *token);

#endif