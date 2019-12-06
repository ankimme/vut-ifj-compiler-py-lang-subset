/*
 * @file symtable.c
 * @brief Implementace tabulky symbolů pomocí hashovací tabulky
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 18.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dynamic_string.h"

// @def Velikost hash tabulky
#define HT_SIZE 1171
// @def Velikost zasobniku hash tabulek
#define HT_STACK_SIZE 500

/* klíč záznamu v tabulce symbolů */
typedef dynamic_string* tKey;

typedef struct
{
	int a;
} tFunctionData;

typedef struct
{
	int a;
} tVariableData;

typedef struct tNodeData
{
	int a;
	struct tNodeData *lptr;
	struct tNodeData *rptr;
} tNodeData;

/* typ obsahu */
typedef enum
{
	HT_TYPE_VARIABLE,
	HT_TYPE_FUNCTION,
	HT_TYPE_NODE,
} tHTType;

/* Datová položka HT s explicitně řetězenými synonymy*/
 typedef struct tHash_Table_Item
 {
	tKey key; // klíč
	tHTType type; // typ
	// data
	tFunctionData *fun_data;
	tVariableData *var_data;
	tNodeData *node_data;
	struct tHash_Table_Item* next_item;	/* ukazatel na další synonymum */
} tHash_Table_Item;

/* HT s explicitně zřetězenými synonymy. */
typedef tHash_Table_Item* tHash_Table[HT_SIZE];

typedef struct
{
	tHash_Table* HT_array[HT_STACK_SIZE];
	int top;
} tSymtable;


/* Hlavičky funkcí. */

/* FUNKCE PRO TABULKU SYMBOLU */

/**
 * Inicializace symtablu (aka zasobniku hash tabulek)
 * a vlozeni hash tabulky reprezentujici hlavni kontext
 * 
 * @param symtable Vstupní tabulka symbolů 
 * @post Inicializovany symtable, na vrcholu zasobniku je hash tabulka reprezentujici hlavni kontext
 */
void st_init (tSymtable *symtable);

/**
 * Push nově alokované tabulky do zásobníku hash tabulek
 * 
 * @param symtable Vstupní tabulka symbolů 
 * @post Do tabulky symbolu je vložena nová hash tabulka, která reprezentuje aktuální kontext
 */
void st_indent (tSymtable *symtable);

// TODO
void st_dedent (tSymtable *symtable);

/**
 * Přidá záznam dat do aktuální hash tabulky
 * V případě že záznam se stejným klíčem již existuje, data se přepíší
 * 
 * @param symtable Vstupní tabulka symbolů 
 * @post 
 */
void st_insert_entry_in_current_context (tSymtable *symtable, tKey key, void *data, tHTType type);

tHash_Table_Item* st_insert_entry_in_current_context_random_key (tSymtable *symtable, void *data, tHTType type);

// TODO
tHash_Table_Item* st_search_entry (tSymtable *symtable, tKey key);

/**
 * Uvolneni tabulky symbolu z pameti
 * 
 * @param symtable Vstupní tabulka symbolů 
 * @post Všechny ukazatelé (včetně zanořených) jsou uvolněny a nastaveny na NULL
 */
void st_clean_all (tSymtable *symtable);


/* FUNKCE PRO HASH TABULKU */

/**
 * Inicializace hash tabulky
 * 
 * @param hash_table Vstupní hash tabulka
 * @pre hash_table má již alokovanou paměť
 * @post Všechny ukazatelé v hash_table jsou nastaveny na NULL
 */
void ht_init (tHash_Table* hash_table);

/**
 * Uvolnění hash tabulky z paměti
 * 
 * @param hash_table Vstupní hash tabulka
 * @post Všechny ukazatelé v hash_table jsou uvolněny nastaveny na NULL
 */
void ht_clean (tHash_Table* hash_table);

void ht_item_clean ( tHash_Table_Item* item );

/**
 * Vyhledání položky v hash tabulce na základě klíče
 * 
 * @param hash_table Vstupní hash tabulka
 * @param key Hashovací klíč
 * @return Ukazatel na nalezenou položku, v ostatních případech NULL
 */
tHash_Table_Item* ht_search (tHash_Table* hash_table, tKey key);

// /**
//  * 
//  * @param hash_table Vstupní hash tabulka
//  * @param key Hashovací klíč
//  * @return 
//  */
// void htInsert ( tHash_Table* ptrht, tKey key, tData data )

// void htInit ( tHash_Table* ptrht );

// tHash_Table_Item* htSearch ( tHash_Table* ptrht, tKey key );

// void htInsert ( tHash_Table* ptrht, tKey key, tData data );

// tData* htRead ( tHash_Table* ptrht, tKey key );

// void htDelete ( tHash_Table* ptrht, tKey key );

// void htClearAll ( tHash_Table* ptrht );


tKey st_generate_random_key();

int st_generate_hash(tKey key);

#endif
