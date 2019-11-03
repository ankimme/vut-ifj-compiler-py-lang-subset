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
 * @enum Identificator_type
 * @brief Typy identifikátorů.
 */

typedef enum
{
    VARIABLE_OR_CONSTANT,
    FUNCTION
} Identificator_type;

/**
 * @enum Variable_type
 * @brief Typy proměnných.
 */

typedef enum
{
    INT,
    DOUBLE,
    STRING,
} Variable_type;

/**
  * @union Data_value
  * @brief Hodnota dat v tabulce symbolů.
  */

typedef union
{
    int int_value;
    double double_value;
    char* string_value;
} Data_value;

/**
 * @struct St_item
 * @brief Položka v záznamu.
 */

typedef struct
{
    Identificator_type identificator_type;
    Variable_type variable_type;
    Data_value data;
} St_item;

 /**
  * @struct St_entry
  * @brief Záznam v tabulce některé z tabulek symbolů (v případě že je více kontextů).
  */

typedef struct
{
    unsigned long key;
    St_item* value;
} St_entry;

/**
 * @struct St_table
 * @brief Tabulka symbolů.
 */

typedef struct
{
    int size;
    int item_count;
    St_entry** entries;
} St_table;

/**
 * @struct St_context_table
 * @brief Kontextová tabulka. Obsahuje ukazatele na tabulky symbolů. Plní se jako zásobník.
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
 * Vloží do tabulky kontextů novou (prázdnou) tabulku symbolů, která reprezentuje nový kontext.
 *
 * @param context_table Ukazatel na tabulku kontextů.
 * @post Poslední prvek context_table.context_frames je odkaz na nově vloženou tabulku symbolů.
 */

void st_create_context_frame(St_context_table* context_table);

/**
 * Odstraní z tabulky kontextů poslední vloženou tabulku symbolů. Data v ní uložená se odstraní. Simuluje odstranění lokálních proměnných funkce.
 *
 * @param context_table Ukazatel na tabulku kontextů.
 * @post Poslední prvek context_table.context_frames je odstraněn, včetně jeho obsahu.
 */

void st_pop_context_frame(St_context_table* context_table);

/**
 * Inicializace prázdné tabulky symbolů.
 *
 * @param t_init Ukazatel na ukayatel na tabulku.
 * @post Pole entries[] obsahuje ukazatele inicializované na hodnotu NULL.
 */

void st_init_table(St_table** t_init);

/**
 * Inicializace záznamu v tabulce symbolů.
 *
 * @param e_init Ukazatel na záznam.
 * @post Key = 0, Value = NULL
 */

void st_init_entry(St_entry** e_init);

/**
 * Uvolnění záznamu z paměti.
 *
 * @param e_delete Ukazatel na záznam.
 */

void st_delete_entry(St_entry** e_delete);

/**
 * Uvolnění tabulky symbolů z paměti.
 *
 * @param t_delete Ukazatel na tabulku.
 */

void st_delete_table(St_table** t_delete);

/**
 * Vložení položky do nejaktuálnější tabulky symbolů. Tedy do nejnovějšího kontextu který vznikl.
 *
 * @param context_table Ukazatel na tabulku kontextů.
 * @param identificator Identifikátor podle kterého bude vygenerován hash klíč.
 * @param value Ukazatel na položku která bude vložena do tabulky.
 * @post Záznam je uložen v tabulce symbolů pod hashem vygenerovaným z identifikátoru.
 */

void st_insert_item_in_current_context(St_context_table* context_table, char* identificator, St_item* value);

/**
 * Vyhledání položky ve všech tabulkách symbolů. Prohledají se všechny kontext.
 *
 * @param context_table Ukazatel na tabulku kontextů.
 * @param identificator Identifikátor pro vygenerování hash klíče.
 * @return Ukazatel na nalezenou položku. NULL v případě že položka nebyla nalezena.
 */

St_item* st_search_item_in_all_contexts(St_context_table* context_table, char* identificator);

/**
 * Vyhledání položky v aktuální tabulce symbolů. Prohledá se jen nejaktuálnější kontext.
 *
 * @param context_table Ukazatel na tabulku kontextů.
 * @param identificator Identifikátor pro vygenerování hash klíče.
 * @return Ukazatel na nalezenou položku. NULL v případě že položka nebyla nalezena.
 */

St_item* st_search_item_in_current_context(St_context_table* context_table, char* identificator);

/**
 * Vygenerování hash klíče z identifikátoru.
 *
 * @param s Identifikator pro generovaní hash klíče.
 * @return Hash klíč.
 *
 */

unsigned long st_generate_hash(char *s);

/**
 * Uvolnění všech symbolických tabulek a jejich obsahu z paměti.
 *
 * @param context_table Ukazatel na tabulku kontextů.
 *
 */

void st_free_all(St_context_table* context_table);

/**
 * Funkce zatím není navržena ani implementována.
 * 
 * @param error_number Číslo chyby.
 */

void error_handle(int error_number);

#endif