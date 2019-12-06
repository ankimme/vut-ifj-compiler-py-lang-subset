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

/*
 * MISSING DOCUMENTATION!
 */
void tStack_init ( tStack* s );

/*
 * MISSING DOCUMENTATION!
 */
int tStack_empty ( const tStack* s );

/*
 * MISSING DOCUMENTATION!
 */
int tStack_full ( const tStack* s );

/*
 * MISSING DOCUMENTATION!
 */
void tStack_pop ( tStack* s );

/*
 * MISSING DOCUMENTATION!
 */
void tStack_top ( const tStack* s, int* i );

/*
 * MISSING DOCUMENTATION!
 */
void tStack_push ( tStack* s, int i );

#endif
