/*
 * @file stack.h
 * @brief Implementace pomocného zásobníku
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 26.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>

#define STACK_SIZE 50

typedef struct 
{
	int arr[STACK_SIZE];                            
	int top;                                
} tStack;

/* ZASOBNIK CELYCH CISEL */

/**
 * Inicializuje zásobník
 * 
 * @param s Ukazatel na strukturu zásobníku
 * @pre Paměť pro zásobník už je alokovaná
 */
void tStack_init ( tStack* s );

/**
 * Určí zda-li je zásobník prázný
 * 
 * @param s Ukazatel na strukturu zásobníku
 * @return true v případě že je zásobník prázdný, jinak false
 */
int tStack_empty ( const tStack* s );

/**
 * Určí zda-li je zásobník plný
 * 
 * @param s Ukazatel na strukturu zásobníku
 * @return true v případě že je zásobník plný, jinak false
 */
int tStack_full ( const tStack* s );

/**
 * Odstraní položku na vrcholu zásobníku
 * 
 * @param s Ukazatel na strukturu zásobníku
 */
void tStack_pop ( tStack* s );

/**
 * Vrátí položku na vrcholu zásobníku
 * 
 * @param s Ukazatel na strukturu zásobníku
 * @param i Ukazatel na integer
 * @post V proměnné i je hodnota z vrcholu zásobníku
 */
void tStack_top ( const tStack* s, int* i );

/**
 * Vloží položku na vrchol zásobníku
 * 
 * @param s Ukazatel na strukturu zásobníku
 * @param i Položka která se bude na zásobník vkládat
 */
void tStack_push ( tStack* s, int i );

#endif
