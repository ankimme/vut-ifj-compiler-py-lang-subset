/*
 * @file symtable.h
 * @brief Deklarace struktur pro tabulku symbolů
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 26.10.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 * Implementace inspirována z následujícího zdroje:
 * https://github.com/jamesroutley/write-a-hash-table/tree/master/02-hash-table
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string.h>
#include <stdlib.h>
#include <math.h>

// @def Velikost o kterou se zvetsuje tabulka symbolu v pripade ze dojde misto na polozky
#define INCREMENT_SIZE 100
// @def Maximalni pocet moznych context framu, v praxi kolik max krat lze volat funkci rekurzivne
#define MAX_CONTEXT_FRAMES 1500


/**
 *
 * @enum Identificator_type
 * @brief Typy identifikátorů
 *
 */

typedef enum
{
    VARIABLE_OR_CONSTANT,
    FUNCTION
} Identificator_type;

/**
 *
 * @enum Variable_type
 * @brief Typy proměnných
 *
 */

typedef enum
{
    INT,
    DOUBLE,
    STRING,
    //BOOL, CHAR TODO
} Variable_type;

/**
  * @union Data_value
  * @brief Hodnoty dat
  */

typedef union
{
    int int_value;
    double double_value;
    char* string_value;
} Data_value;

/**
 *
 * @struct St_item
 * @brief Definování položek
 *
 */

typedef struct
{
    Identificator_type identificator_type;
    Variable_type variable_type;
    Data_value data;
} St_item;

 /**
  *
  * @struct St_entry
  * @brief Definování záznamů
  *	
  */

typedef struct
{
    unsigned long key;
    St_item* value;
} St_entry;

/**
 *
 * @struct St_table
 * @brief Definování tabulky
 *
 */

typedef struct
{
    int size;
    int item_count;
    St_entry** entries;
} St_table;

/**
 * 
 * TODO
 */

typedef struct
{
    int capacity;
    int context_count;
    St_table *context_frames[MAX_CONTEXT_FRAMES];
} St_context_table;

/**
 * Inicializace tabulky kontextů.
 *
 * @param context_table Ukazatel na tabulku kontextů.
 * @post context_table.context_frames[0] obsahuje odkaz na hlavní tabulku symbolů.
 */

void st_init_context_table(St_context_table* context_table);

/**
 * Vloží do tabulky kontextů novou (prázdnout) tabulku symbolu (která reprezentuje nový kontext).
 *
 * @param context_table Ukazatel na tabulku kontextů.
 * @post Poslední prvek context_table.context_frames je odkaz na nově vloženou tabulku symbolů.
 */

void st_create_context_frame(St_context_table* context_table);

/**
 * Inicializace prázdné tabulky symbolů.
 *
 * @param t_init Ukazatel na tabulku.
 */

void st_init_table(St_table** t_init);

/**
 * Inicializace záznamu v tabulce symbolů.
 *
 * @param e_init Ukazatel na položku v tabulce.
 */

void st_init_entry(St_entry** e_init);

/**
 * Odstranění záznamu z tabulky symbolů.
 *
 * @param e_delete Ukazatel na položku v tabulce.
 */

void st_delete_entry(St_entry** e_delete);

/**
 * Odstranění tabulky symbolů.
 *
 * @param t_delete Ukazatel na tabulku.
 */

void st_delete_table(St_table** t_delete);

/**
* Vložení položky do tabulky symbolů. TODO
*
* @param table Ukazatel na tabulku.
* @param identificator Identifikátor
* @param value Ukazatel na položku která bude vložena do tabulky TODO.
*/

void st_insert_item_in_current_context(St_context_table* context_table, char* identificator, St_item* value);

/**
 * Vyhledání položky v tabulce symbolů. TODO
 *
 * @param table Ukazatel na tabulku.
 * @param identificator Identifikátor
 */

St_item* st_search_item(St_context_table* context_table, char* identificator);

/**
 * Vygenerování hashe
 *
 * @param s je identifikator
 * @return funkce vrací hash
 *
 */

unsigned long st_generate_hash(char *s);

/** TODO dokuentace */

void error_handle(int error_number);

#endif