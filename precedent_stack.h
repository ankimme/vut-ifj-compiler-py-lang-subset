/*
 * @file precedent_stack.h
 * @brief Zásobník pro precedenční analýzu
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 29.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef PRECEDENT_STACK_H
#define PRECEDENT_STACK_H

#include "expressions.h"
#include "dynamic_string.h"

/**
 *
 * @struct tSymbol.
 * @brief Definice symbolu na zásobníku.
 *
 */

typedef struct symbol
{
    Prec_stack_symbols type;
    char* str;
    struct symbol *next_ptr;
}*tSymbol;

/**
 *
 * @struct tPrec_stack.
 * @brief Zásobník symbolů precedenční analýzy.
 *
 */

typedef struct
{
    tSymbol top;
}tPrec_stack;

/**
 * Inicializace zásobníku symbolů precedenční analýzy. 
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @post Zásobník má inicializovanou hodnotu vrcholu na NULL.
 *
 */

void tPrec_stack_init(tPrec_stack* prec_s);


/**
 * Kontrola prázdnosti zásobníku. 
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @return Vrací hodnotu 1 v případě prázdného zásobníku, jinak vrací hodnotu 0.
 *
 */

int tPrec_stack_empty (const tPrec_stack* prec_s);

/**
 * Vložení symbolu na vrchol zásobníku. 
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @param type Platný typ symbolu, jejž chceme vložit na vrchol zásobníku.
 * @param string Konkrétní znak symbolu načtený ze vstupní pásky vkládaný na zásobník.
 * @return Vrací hodnotu 1 v případě úspěšného vložení symbolu na zásobník, jinak vrací hodnotu 0.
 *
 */

int tPrec_stack_push(tPrec_stack* prec_s, Prec_stack_symbols type, char* string);

/**
 * Získání terminálu vyskytujícího se nejvýše u vrcholu zásobníku. 
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @return Vrací ukazatel na terminál, který se nachází nejvýše u vrcholu, případně NULL, zda tam žádný není.
 *
 */

tSymbol tPrec_stack_top_term(tPrec_stack* prec_s);

/**
 * Vložení symbolu pro začátek handle za první nalezený terminál
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @param type Typ symbolu (očekáván handle), jejž chceme vložit na zásobník.
 * @param string Znak uvozující začátek handle načtený ze vstupní pásky.
 * @return V případě úspěšného vložení symbolu pro začátek handle vrací 1, jinak vrací 0.
 *
 */

int tPrec_stack_push_handle(tPrec_stack* prec_s, Prec_stack_symbols type, char* string);

/**
 * Uvolnění paměti všech symbolů zásobníku
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @post Zanechá prázdný zásobník a uvolní pamět naalokovanou pro symboly zásobníku.
 *
 */

void tPrec_stack_clean(tPrec_stack* prec_s);

#endif