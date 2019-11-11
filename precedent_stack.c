/*
 * @file precedent_stack.c
 * @brief Zásobník pro precedenční analýzu
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 29.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "precedent_stack.h"

void tPrec_stack_init(tPrec_stack* prec_s)
{
    if (prec_s == NULL)
    {
        return;
    }
    prec_s->top = NULL;
}

int tPrec_stack_empty (const tPrec_stack* prec_s)
{
    return ((prec_s->top == NULL) ? 1 : 0); //1 je prázdný, 0 není
}

int tPrec_stack_push(tPrec_stack* prec_s, Prec_stack_symbols type, char* string)
{
    tSymbol new_symbol = malloc(sizeof(struct symbol));

    if (new_symbol == NULL) //chyba v alokaci
    {
        return 0;
    }
    else
    {
        new_symbol->type = type;
        new_symbol->str = string;

        new_symbol->next_ptr = prec_s->top; //ukazuje na předcházející top
        prec_s->top = new_symbol; //nový symbol novým topem

        return 1;
    }
}

tSymbol tPrec_stack_top_term(tPrec_stack* prec_s)
{

    if (!tPrec_stack_empty(prec_s))
    {
        tSymbol top_symbol = prec_s->top;

        while(top_symbol != NULL) //dokud nejsme na konci zásobníku hledej terminál
        {
            if (top_symbol->type == EXPR_TERMINAL) //našel se terminál
            {
                return top_symbol;
            }
            top_symbol = top_symbol->next_ptr;
        }

        return NULL; //nenašel se žádný terminál
    }
    else
    {
        return NULL; //prázdný zásobník
    }
}

int tPrec_stack_push_handle(tPrec_stack* prec_s, Prec_stack_symbols type, char* string)
{
    if (type != EXPR_HANDLE) //chceme vkládat pouze handle, jinak chyba
    {
        return 0;
    }

    tSymbol top_symbol = prec_s->top;

    while (top_symbol != NULL) //hledej první terminál a vlož za ním začátek handle (znak [ )
    {
        if (top_symbol->type == EXPR_TERMINAL) //první na topu je terminál, vlož handle za něj
        {
            if(tPrec_stack_push(prec_s, type, string))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else if (top_symbol->next_ptr->type == EXPR_TERMINAL) //terminál se nachází za aktuálním neterminálem, nutno provázat ukazatele
        {
            tSymbol new_symbol = malloc(sizeof(struct symbol));

            if (new_symbol == NULL) //chyba v alokaci
            {
                return 0;
            }
            else
            {
                new_symbol->type = type;
                new_symbol->str = string;


                new_symbol->next_ptr = top_symbol->next_ptr;
                top_symbol->next_ptr = new_symbol;

                return 1;
            }
        }
        else //další symbol je neterminál, hledej dál
        {
            top_symbol = top_symbol->next_ptr;
        }        
    }

    return 0; //není terminál
}

void tPrec_stack_clean(tPrec_stack* prec_s)
{
    while (!tPrec_stack_empty(prec_s)) //dokud zásobník není prázdný, uvolňuje paměť
    {
        tSymbol pop_symbol = prec_s->top;
        prec_s->top = pop_symbol->next_ptr;
        free(pop_symbol);
    }
}