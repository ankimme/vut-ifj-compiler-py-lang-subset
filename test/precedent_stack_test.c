/*
 * @file precedent_stack_test.c
 * @brief Test zásobníku symbolů pro precedenční analýzu
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 30.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */


#include "../precedent_stack.h"

const char* get_symbol_type(int type) 
{
   switch (type) 
   {
      case 0: return "NONTERMINAL";
      case 1: return "TERMINAL";
      case 2: return "HANDLE";
      case 3: return "UNDEFINED";
      default: return "ERROR!";
   }
}

void print_top(tPrec_stack* prec_stack)
{

    printf("TOP SYMBOL\n");
    printf("----------\n");

    if (!tPrec_stack_empty(prec_stack))
    {
        printf("Type: %s\n", get_symbol_type(prec_stack->top->type));
        printf("Symbol: %s\n", prec_stack->top->str);

        if (prec_stack->top->next_ptr == NULL)
        {
            printf("Next symbol: NULL\n");
        }
        else
        {
            printf("Next symbol: %s\n", prec_stack->top->next_ptr->str);
        }

        printf("\n");
    }
    else if(prec_stack->top == NULL)
    {
        printf("EMPTY STACK TOP: NULL\n");
        printf("\n"); 
    }
    else
    {
        printf("ERROR WITH EMPTY STACK!!!\n");
    }
}

void print_top_terminal(tPrec_stack* prec_stack)
{
    printf("TOP TERMINAL\n");
    printf("----------\n");

    if (!tPrec_stack_empty(prec_stack))
    {
        tSymbol term = tPrec_stack_top_term(prec_stack);

        if (term == NULL)
        {
            printf("NO TERM ON STACK!\n");
        }
        else
        {
            printf("Type: %s\n", get_symbol_type(term->type));
            printf("Symbol: %s\n", term->str);

            if (term->next_ptr == NULL)
            {
                printf("Next symbol: NULL\n");
            }
            else
            {
                printf("Next symbol: %s\n", term->next_ptr->str);
            }
        }
    }
    else if(prec_stack->top == NULL)
    {
        printf("EMPTY STACK TOP: NULL\n");
    }
    printf("\n");
}

void pop_symbol(tPrec_stack* prec_stack)
{
    printf("\033[0;32m");
    printf("POPPING SYMBOL\n");
    printf("\n");
    tPrec_stack_pop(prec_stack);
    printf("\033[0;36m");
}

void pop_handle(tPrec_stack* prec_stack, Prec_derivation derivation)
{
    printf("\033[0;32m");
    printf("POPPING HANDLE RULE\n");
    printf("\n");
    tPrec_stack_pop_handle(prec_stack, derivation);
    printf("\033[0;36m");
}

void push_handle(tPrec_stack* prec_stack, Prec_stack_symbol type, char* string)
{
    if (tPrec_stack_push_handle(prec_stack, type, string))
    {
        printf("\033[0;32m");
        printf("HANDLE INSERTED\n");
    }
    else
    {
        printf("\033[1;31m");
        printf("HANDLE NOT INSERTED!\n");
    }
    printf("\n");
    printf("\033[0;36m");
}

int main()
{
    printf("\033[0;36m");
    tPrec_stack prec_stack;

    tPrec_stack_init(&prec_stack);

    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, "$");
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, "+");
    tPrec_stack_push(&prec_stack, SYMBOL_NONTERMINAL, "55");
    tPrec_stack_push(&prec_stack, SYMBOL_NONTERMINAL, "E");
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, "i1");

    print_top(&prec_stack);
    print_top_terminal(&prec_stack);

    push_handle(&prec_stack, SYMBOL_HANDLE, "[");
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, "i2");
    //pop_symbol(&prec_stack);

    print_top(&prec_stack);
    print_top_terminal(&prec_stack);

    pop_handle(&prec_stack, DERIVATION_VALUE);

    print_top(&prec_stack);
    print_top_terminal(&prec_stack);


    //  STACK CLEANING  //

    printf("\033[0;32m");     
    printf("CLEANING STACK\n");
    printf("\n");

    tPrec_stack_clean(&prec_stack);
    print_top(&prec_stack);

    

    printf("\033[0m"); 
    return 0;
}