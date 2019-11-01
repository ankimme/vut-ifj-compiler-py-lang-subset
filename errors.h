/*
 * @file errors.h
 * @brief Soubor s jednotlivými chybovými výstupy
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 1.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#ifndef ERRORS_H
#define ERRORS_H

/**
 *
 * @enum Errors
 * @brief Druhy errorů a jejich návratových kódů
 *
 */

typedef enum
{
    NO_ERROR = 0,
    LEXICAL_ERROR = 1,
    SYNTAX_ERROR = 2,
    DEFINITION_SEMANTIC_ERROR = 3,
    RUNTIME_SEMANTIC_ERROR = 4,
    PARAMS_SEMANTIC_ERROR = 5,
    OTHER_SEMANTIC_ERROR = 6, 
    DIVISION_BY_ZERO_ERROR = 9;
    INTERNAL_ERROR = 99,
}Errors


#endif