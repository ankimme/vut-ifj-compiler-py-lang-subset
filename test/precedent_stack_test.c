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

const char* get_token_type(int type) 
{
   switch (type) 
   {
      case 0: return "Indent";
      case 1: return "Dedent";
      case 2: return "EOL";
      case 3: return "EOF";
      case 4: return "Comma";
      case 5: return "Colon";
      case 6: return "Left bracket";
      case 7: return "Right bracket";
      case 8: return "Plus";
      case 9: return "Minus";
      case 10: return "Multiply";
      case 11: return "Keyword";
      case 12: return "Identifier";
      case 13: return "Integer";
      case 14: return "Double";
      case 15: return "Not equal";
      case 16: return "Equal";
      case 17: return "Less or equal";
      case 18: return "Greater or equal";
      case 19: return "Less than";
      case 20: return "Greater than";
      case 21: return "Assignment";
      case 22: return "Divide integer";
      case 23: return "Divide float";
      case 24: return "String literal";
      case 25: return "Undefined";
      default: return "ERROR";
   }
}

const char* get_symbol_type(int type) 
{
   switch (type) 
   {
      case 0: return "NONTERMINAL";
      case 1: return "TERMINAL";
      case 2: return "HANDLE";
      default: return "ERROR!";
   }
}


void print_top(tPrec_stack* prec_stack)
{

    printf("TOP SYMBOL\n");
    printf("----------\n");

    if (!tPrec_stack_empty(prec_stack))
    {
        printf("Symbol type: %s\n", get_symbol_type(prec_stack->top->type));
        printf("Value type: %s\n", get_token_type(prec_stack->top->value_type));
        printf("Symbol: %s\n", prec_stack->top->attribute->str);

        if (prec_stack->top->next_ptr == NULL)
        {
            printf("Next symbol: NULL\n");
        }
        else
        {
            printf("Next symbol: %s\n", prec_stack->top->next_ptr->attribute->str);
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
            printf("Symbol type: %s\n", get_symbol_type(term->type));
            printf("Value type: %s\n", get_token_type(prec_stack->top->value_type));
            printf("Symbol: %s\n", term->attribute->str);

            if (term->next_ptr == NULL)
            {
                printf("Next symbol: NULL\n");
            }
            else
            {
                printf("Next symbol: %s\n", term->next_ptr->attribute->str);
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

void push_handle(tPrec_stack* prec_stack)
{
    if (tPrec_stack_push_handle(prec_stack))
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


    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, UNDEFINED,"$");
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, TOKEN_PLUS, "+");
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, TOKEN_INTEGER, "55");
    tPrec_stack_push(&prec_stack, SYMBOL_NONTERMINAL, UNDEFINED, "E");
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, TOKEN_IDENTIFIER, "i1");
    

    print_top(&prec_stack);
    print_top_terminal(&prec_stack);

    push_handle(&prec_stack);
    //tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, TOKEN_IDENTIFIER, "i2");
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


    // SMALL TEST //
    /*
    tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, "e");
    print_top_terminal(&prec_stack);
    tSymbol top_term = tPrec_stack_top_term(&prec_stack);
    char * test = top_term->str;

    printf("%s\n", test);
    */

    return 0;
}