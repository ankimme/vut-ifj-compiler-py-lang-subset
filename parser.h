/*
 * @file parser.h
 * @brief Implementace syntaktického analyzátoru
 * @author 
 * @date 28.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "symtable.h"



int new_token(St_token* token, tStack* stack);





int prog();


#endif