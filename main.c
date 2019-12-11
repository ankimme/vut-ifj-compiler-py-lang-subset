/*
 * @file main.c
 * @brief Hlavní soubor pro spuštění programu
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 27.10.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "parser.h"

int main()
{
    int error_code = start_analysis();
    return error_code;
}