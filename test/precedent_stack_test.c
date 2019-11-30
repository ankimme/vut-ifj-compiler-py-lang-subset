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
        printf("EMPTY_STACK_TOP: NULL\n");    
    }
}

int main()
{
    printf("\033[0;36m");
    tPrec_stack prec_stack;

    tPrec_stack_init(&prec_stack);
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, "$");

    print_top(&prec_stack);

    tPrec_stack_clean(&prec_stack);

    print_top(&prec_stack);

    printf("\033[0m"); 
    return 0;
}