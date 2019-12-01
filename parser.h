/*
 * @file parser.h
 * @brief Implementace syntaktického analyzátoru
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 29.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "symtable.h"

#include <stdbool.h>

typedef struct
{
    St_token *current_token;
    tStack *scanner_stack;
    int error_code;
    bool unget_token;
} tParser_data;



/**
 * Zajistí spuštění syntaktické analýzy a překladu vstupního kódu
 *
 * @return 0 v případě úspěchu, jiné číslo v případě chyby (podle zadání)
 */
int start_analysis();

/**
 * Inicializace struktury pro uchování dat pro parser
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @pre Paměť pro ukazatel je již alokována
 * @return true v případě úspěchu, false v případě neúspěchu
 */
bool init_parser_data(tParser_data *parser_data);

/**
 * Uvolnění veškterých dat parseru
 *
 * @param parser_data Ukazel na struktu tParser_data
 */
void free_parser_data(tParser_data *parser_data);

/**
 * V případě že je unget_token true, nastaví unget_token na false a nenačíta další token
 * V případě že je unget_token false, načte následující token a zkontroluje zda-li nedošlo k lexikální analýze. Pokud ano, nastaví error_code na 1 a vrací false
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @return true v případě úspěchu, false v případě chyby vzniklé při lexikální analýze
 */
bool get_token_and_set_error_code(tParser_data *parser_data);

/**
 * Načte následující token (záleží na nastavení flagu unget_token) a porovná typ načteného tokenu s typem předaným parametrem
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @param type Enum hodnota pro porovnání typu tokenu
 * @post V případě neočekávaného typu tokenu, nastaví error_code na 2 (syntaktická chyba)
 * @return true v případě že úspěchu, false v případě chyby
 */
bool get_compare_check(tParser_data *parser_data, Token_type type);

/**
 * Nastaví nový error code pouze v případě, že již nebyl dříve nastaven.
 * 
 * @param parser_data Ukazel na struktu tParser_data
 * @param new_code Nový error code
 */
void set_error_code(tParser_data *parser_data, int new_code);

/**
 * Rozgenerovává neterminál PROG
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool prog(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál PARAMS
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool params(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál PARAMS_N
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool params_n(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál STATEMENTS
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool statements(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál SEQUENCE
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool sequence(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál SEQUENCE_N
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool sequence_n(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál FUNC_RETURN
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool func_return(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál RETURN_VALUE
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool return_value(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál INSTRUCT
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool instruct(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál INSTRUCT_CONTINUE
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool instruct_continue(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TERM
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool term(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TERM_N
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool term_n(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TERM_N_VALUE
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool term_n_value(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TYPE
 *
 * @param parser_data Ukazel na struktu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool type(tParser_data *parser_data);

#endif