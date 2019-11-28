/*
 * @file dynamic_string.h
 * @brief Deklarace struktur a funkcí pro dynamic_string knihovnu
 * @author Matej Alexej Helc (xhelcm00@stud.fit.vutbr.cz)
 * @date 27.10.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 32

typedef struct
{
    char *str;
    int length;
    int allocated_size;
} dynamic_string;

/**
 * Vyprázdni reťazec.
 *
 * @param s Ukazeteľ na reťazec.
 */
void string_clear(dynamic_string *s);

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
int strings_append(dynamic_string *s, char *string);

#endif