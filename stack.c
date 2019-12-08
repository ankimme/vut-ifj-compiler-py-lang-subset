/*
 * @file stack.c
 * @brief Implementace pomocného zásobníku
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 26.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 */

#include "stack.h"

/* ZASOBNIK CELYCH CISEL */

void tStack_init( tStack* s )
{
    if (s == NULL)
    {
        return;
    }

    s->top = -1;
}

int tStack_empty ( const tStack* s )
{
    return s->top == -1;
}

int tStack_full ( const tStack* s )
{
    return s->top == STACK_SIZE - 1;   
}

void tStack_pop ( tStack* s )
{
    if (!tStack_empty(s))
    {
        s->top--;
    }
}

void tStack_top ( const tStack* s, int* c )
{
    if (tStack_empty(s))
    {
        return;
    }

    *c = s->arr[s->top];
}

void tStack_push ( tStack* s, int c )
{
    if (tStack_full(s))
    {
        return;
    }

    s->top++;  
    s->arr[s->top] = c;
}
