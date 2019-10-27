/*
 * @file dynamic_string.h
 * @brief Deklarace struktur a funkcí pro dynamic_string knihovnu
 * @author Matej Alexej Helc (xhelcm00@stud.fit.vutbr.cz)
 * @date XX.XX.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *str;
    unsigned int length;
    unsigned int alloc_size;
} dynamic_string;

/**
 * Vyprázdni reťazec.
 *
 * @param s Ukazeteľ na reťazec.
 */
void dynamic_string_clear(dynamic_string *s);

/**
 * Inicializácia dynamického reťazca.
 *
 * @param s Ukazateľ na reťazec.
 * @return 1 ak sa inicializácia podarila, 0 ak nie.
 */
int string_init(dynamic_string *s);

/**
 * Uvoľňuje alokovanú pamäť pre dynamický reťazec.
 *
 * @param s Ukazateľ na reťazec.
 */
void string_free(dynamic_string *s);

/**
 * Porovná reťazce.
 *
 * @param dynamic_string Dynamický reťazec.
 * @param const_string Reťazec, konštanta.
 * @return Vracia 1, 0 alebo -1 v závislosti či je dyn. reťazec väčší, rovný alebo menší ako druhý reťazec.
 */
int string_cmp(dynamic_string *dynamic_string, const char *string);

/**
 * Pridá znak na koniec dynamického reťazca.
 *
 * @param s Dynamický reťazec.
 * @param c Znak, ktorý chceme pridať.
 * @return 1 ak sa podarilo pridať znak, 0 ak nie.
 */
int string_add_char(dynamic_string *s, char c);

/**
 * Zreťazí reťazce.
 *
 * @param s Dynamický reťazec.
 * @param const_string Reťazec, konštanta.
 * @return 1 ak sa zreťazenie podarilo, 0 ak nie.
 */
int string_cat(dynamic_string *s, const char *string);

#endif