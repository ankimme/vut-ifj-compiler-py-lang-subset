/*
 * @file dynamic_string.h
 * @brief Deklarace struktur a funkcí pro dynamic_string knihovnu
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 27.10.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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
 * @param s Ukazeteľ na strukturu dynamického reťazeca.
 * @post Nastaví všechny znaky v alokované paměti pro řetězec na '\0'.
 */
void string_clear(dynamic_string *s);

/**
 * Inicializácia dynamického reťazca.
 *
 * @param s Ukazateľ na strukturu dynamického reťazeca.
 * @pre Paměť pro pointer s je již alokována
 * @return 1 ak sa inicializácia podarila, 0 ak nie.
 */
int string_init(dynamic_string *s);

/**
 * Uvoľňuje alokovanú pamäť pre dynamický reťazec.
 *
 * @param s Ukazateľ na strukturu dynamického reťazeca.
 */
void string_free(dynamic_string *s);

/**
 * Pridá znak na koniec dynamického reťazca.
 *
 * @param s Ukazateľ na strukturu dynamického reťazeca.
 * @param c Znak, ktorý chceme pridať.
 * @return 1 ak sa podarilo pridať znak, 0 ak nie.
 */
int string_add_char(dynamic_string *s, char c);

/**
 * Pridá reťazec na koniec dynamického reťazca.
 *
 * @param s Ukazateľ na strukturu dynamického reťazeca.
 * @param string Reťazec ktorý se prilepí k reťazci s.
 * @return 1 ak sa zreťazenie podarilo, 0 ak nie.
 */
int string_append(dynamic_string *s, char *string);

void string_trim(dynamic_string **s);


#endif