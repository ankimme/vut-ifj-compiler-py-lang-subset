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

/**
 * @struct dynamic_string
 * 
 * Struktura reprezentující dynamický řetězec
 */
typedef struct
{
    char *str;
    int length;
    int allocated_size;
} dynamic_string;

/**
 * Vyprázdni řetězec.
 *
 * @param s Ukazatel na strukturu dynamického řetězce.
 * @post Nastaví všechny znaky v alokované paměti pro řetězec na '\0'.
 */
void string_clear(dynamic_string *s);

/**
 * Inicializace dynamického retězce.
 *
 * @param s Ukazatel na strukturu dynamického řetězce.
 * @pre Paměť pro pointer s je již alokována
 * @return 1 když se inicializace podařila, jinak 0.
 */
int string_init(dynamic_string *s);

/**
 * Uvolňuje alokovanou pamět pro dynamický řetězec
 *
 * @param s Ukazateľ na strukturu dynamického řetězce.
 */
void string_free(dynamic_string *s);

/**
 * Přidá znak na konec dynamického řetězce
 *
 * @param s Ukazatel na strukturu dynamického řetězce.
 * @param c Znak, který se přidá na konec řetězce.
 * @return 1 když se podařilo znak přida, jinak 0.
 */
int string_add_char(dynamic_string *s, char c);

/**
 * Přidá řetězec na konec dynamického řetězce.
 *
 * @param s Ukazatel na strukturu dynamického řetězce.
 * @param string řetězec který se přidá na konec řetězce s.
 * @return 1 když se podařilo řetězec přidate, jinak 0.
 */
int string_append(dynamic_string *s, char *string);

/**
 * Převede vstupní řetězec do formátu který je kompatibilní s jazykem IFJcode19.
 * 
 * @param s Ukazatel na ukazatel na strukturu dynamického řetězce.
 * @post Ukazatel s nově ukazuje na převedený řetězec. První a poslední znak se ořeže, escape sekvence se převedout na příslušné escape sekvence kompatibilní s kódem v jazyce IFJcode19.
 */
void string_trim(dynamic_string **s);

/**
 * Převede číslo na řetězec a přidá jej na konec jiného řetězce
 * 
 * @param s Ukazatel na strukturu dynamického řetězce.
 * @param a Číslo, které se přidá na konec dynamického řetězce
 */
void string_append_int(dynamic_string *s, int a);

#endif