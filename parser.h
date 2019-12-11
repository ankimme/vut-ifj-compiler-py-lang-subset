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
#include "gen_code.h"
#include <stdbool.h>

/** @def Funkční data potřebná pro syntaktickou analýzu */
typedef struct
{
    St_token *current_token;
    St_token *backup_token;
    tStack *scanner_stack;
    tSymtable *symtable;
    tHash_Table_Item *table_l_value;
    tHash_Table_Item *table_r_value;
    dynamic_string *label;
    int error_code;
    int fun_call_param_count; // pocet parametru u volane funkce
    int if_statement_index;
    int max_if_statement_index;
    int while_statement_index;
    bool unget_token; // flag který vynutí načíst stejný token znova
    bool load_backup;
    bool function_definition_scope; // flag určuje zda se právě nacházíme v definici funkce
} tParser_data;

/**
 * Zajistí spuštění syntaktické analýzy a překladu vstupního kódu
 *
 * @return 0 v případě úspěchu, jiné číslo v případě chyby (podle zadání)
 */
int start_analysis();

/**
 * Inicializace strukturury pro uchování dat pro parser
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @pre Paměť pro ukazatel je již alokována
 * @return true v případě úspěchu, false v případě neúspěchu
 */
bool init_parser_data(tParser_data *parser_data);

/**
 * Uvolnění veškterých dat parseru
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 */
void free_parser_data(tParser_data *parser_data);

/**
 * V případě že je unget_token true, nastaví unget_token na false a nenačíta další token
 * V případě že je unget_token false, načte následující token a zkontroluje zda-li nedošlo k lexikální analýze. Pokud ano, nastaví error_code na 1 a vrací false
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @return true v případě úspěchu, false v případě chyby vzniklé při lexikální analýze
 */
bool get_token_and_set_error_code(tParser_data *parser_data);

/**
 * Načte následující token (záleží na nastavení flagu unget_token) a porovná typ načteného tokenu s typem předaným parametrem
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param type Enum hodnota pro porovnání typu tokenu
 * @post V případě neočekávaného typu tokenu, nastaví error_code na 2 (syntaktická chyba)
 * @return true v případě že úspěchu, false v případě chyby
 */
bool get_compare_check(tParser_data *parser_data, Token_type type);

/**
 * Načte následující token (záleží na nastavení flagu unget_token) a porovná typ načteného tokenu s typy předaným parametrem. Stačí aby souhlasil jeden.
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param type Enum hodnota pro 1. porovnání typu tokenu
 * @param type Enum hodnota pro 2. porovnání typu tokenu
 * @post V případě neočekávaného typu tokenu, nastaví error_code na 2 (syntaktická chyba)
 * @return true v případě že úspěchu, false v případě chyby
 */
bool get_compare_check_double(tParser_data *parser_data, Token_type type1, Token_type type2);

/**
 * Nastaví nový error code pouze v případě, že již nebyl dříve nastaven.
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param new_code Nový error code
 */
void set_error_code(tParser_data *parser_data, int new_code);

/**
 * Udělá hlubokou kopii tokenu 1 do tokenu 2
 * 
 * @param parser_data Ukazatel na strukturu tParser_data
 * @param t1 Zdrojový token
 * @param t2 Cílový token
 * @pre Paměť pro oba tokeny je již alokována
 * @return true v případě úspěšné kopie, false v případě neúspěchu
 */
bool copy_token(tParser_data* parser_data, St_token* t1, St_token* t2);

/**
 * Rozgenerovává neterminál PROG
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool prog(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál PARAMS
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool params(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál PARAMS_N
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool params_n(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál STATEMENTS
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool statements(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál SEQUENCE
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool sequence(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál SEQUENCE_N
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool sequence_n(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál FUNC_RETURN
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool func_return(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál RETURN_VALUE
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool return_value(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál INSTRUCT
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool instruct(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál INSTRUCT_CONTINUE
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool instruct_continue(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TERM
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool term(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TERM_N
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool term_n(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TERM_N_VALUE
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool term_n_value(tParser_data *parser_data);

/**
 * Rozgenerovává neterminál TYPE
 *
 * @param parser_data Ukazatel na strukturu tParser_data
 * @post V případě chyby, nastaví příslušný error code
 * @return true v případě úspěšného rozgenerování, false v případě chyby
 */
bool type(tParser_data *parser_data);

#endif