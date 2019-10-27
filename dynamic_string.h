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

int string_init(Dynamic_string *s); //inicializácia dynamického reťazca

void string_free(Dynamic_string *s); //uvoľnenie alokovanej pamäte

int string_cmp(Dynamic_string *dynamic_string, const char *string); //porovnanie reťazcov

int string_cat(Dynamic_string *s, const char *string); //zreťazenie reťazcov

#endif