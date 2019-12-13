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

#include "symtable.h"
#include "scanner.h"


/**
 *
 * @enum Prec_stack_symbol.
 * @brief Typy symbolů na zásobníku precedenční analýzy.
 *
 */

typedef enum
{
    SYMBOL_NONTERMINAL, // DERIVATED RULE
    SYMBOL_TERMINAL,    // ONE OF TOKEN VALUES PUSHED ON STACK
    SYMBOL_HANDLE,      // [
}Prec_stack_symbol;

/**
 *
 * @enum Prec_token.
 * @brief Typy tokenů přijímané jako indexy precedenční tabulkou.
 *
 */

typedef enum
{
   PREC_PLUS_MINUS,     // +-
   PREC_MUL_DIVS,       // */ //
   PREC_L_BRACKET,      // (
   PREC_R_BRACKET,      // )
   PREC_RELATION_OP,    // RELATION_OP
   PREC_VALUE,          // VALUE

   PREC_OTHER,          // $ a jiné tokeny
}Prec_token;

/**
 *
 * @enum Prec_derive.
 * @brief Počet symbolů potřebných k derivování aktuálního pravidla.
 *
 */

typedef enum
{
    DERIVATION_VALUE = 1,   //E -> value
    DERIVATION_RULE = 3,    //E -> E OP E (OP značí operátor) || (E)
}Prec_derivation;

/**
 *
 * @struct tSymbol.
 * @brief Definice symbolu na zásobníku.
 *
 */

typedef struct symbol
{
    Prec_stack_symbol type;
    Token_type value_type;
    int retype;
    dynamic_string* attribute;
    tNodeData* item;
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
 * Vložení terminálního symbolu na vrchol zásobníku. 
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @param type Určuje, zda že se vkládá terminál či neterminál na vrchol zásobníku.
 * @param value_type Typ symbolu načteného ze vstupní pásky (typ tokenu).
 * @param string Konkrétní symbol načtený ze vstupní pásky a vkládaný na zásobník.
 * @return Vrací hodnotu 1 v případě úspěšného vložení symbolu na zásobník, jinak vrací hodnotu 0.
 *
 */

int tPrec_stack_push(tPrec_stack* prec_s, Prec_stack_symbol type, Token_type value_type, char* string);

/**
 * Vložení neterminálního symbolu na vrchol zásobníku. 
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @param val Terminální symbol, který bude vložen na zásobník ve formě neterminálu.
 * @return Vrací hodnotu 1 v případě úspěšného vložení symbolu na zásobník, jinak vrací hodnotu 0.
 *
 */

int tPrec_stack_push_nonterminal(tPrec_stack* prec_s, tSymbol val);

/**
 * Získání terminálu vyskytujícího se nejvýše u vrcholu zásobníku. 
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @return Vrací ukazatel na terminál, který se nachází nejvýše u vrcholu, případně NULL, zda tam žádný není.
 *
 */

tSymbol tPrec_stack_top_term(tPrec_stack* prec_s);

/**
 * Vložení symbolu pro začátek handle za první nalezený terminál nejblíže vrcholu.
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @return V případě úspěšného vložení symbolu pro začátek handle vrací 1, jinak vrací 0.
 *
 */

int tPrec_stack_push_handle(tPrec_stack* prec_s);

/**
 * Uvolnění symbolu ze zásobníku a uvolnění jím naalokované paměti.
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @post V případě neprázdného zásobníku bude snížen počet symbolů na zásobníku o 1, jinak zásobník zůstane prázdný.
 *
 */

void tPrec_stack_pop(tPrec_stack* prec_s);

/**
 * Uvolnění symbolu nacházejícího se za vrcholem zásobníku a uvolnění jím naalokované paměti.
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @post V případě, že se za vrcholem vyskytuje nějaký symbol, bude odstraněn ze zásobníku a dojde k provázání ukazatele vrcholu na následný symbol.
 *
 */

void tPrec_stack_pop_after_top(tPrec_stack* prec_s);

/**
 * Uvolnění paměti všech symbolů zásobníku.
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @param derivation Určuje o jakou obsluhu handle se bude jednat, buď pouhé přiřazení hodnoty či derivování podle složitějšího pravidla. 
 * @post Odstraní ze zásobníku handle sekvenci a umožní tak její budoucí nahrazení podle odpovídajícího pravidla.
 *
 */

void tPrec_stack_pop_handle(tPrec_stack* prec_s, Prec_derivation derivation);

/**
 * Uvolnění paměti všech symbolů zásobníku.
 *
 * @param prec_s Ukazatel na zásobník symbolů pro precedenční analýzu.
 * @post Zanechá prázdný zásobník a uvolní pamět naalokovanou pro symboly zásobníku.
 *
 */

void tPrec_stack_clean(tPrec_stack* prec_s);

#endif