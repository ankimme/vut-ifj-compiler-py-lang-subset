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

#define INCREMENT_SIZE 100

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
    Data_value * data;
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
 * Inicializace tabulky symbolů.
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
* Vložení položky do tabulky symbolů.
*
* @param table Ukazatel na tabulku.
* @param identificator Identifikátor
* @param value Ukazatel na položku.
*/

void st_insert_item(St_table* table, char* identificator, St_item* value);

/**
 * Vyhledání položky v tabulce symbolů.
 *
 * @param table Ukazatel na tabulku.
 * @param identificator Identifikátor
 */

int st_search_item(St_table* table, char* identificator);

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