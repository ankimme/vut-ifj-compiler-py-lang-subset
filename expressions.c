/*
 * @file expressions.c
 * @brief Zpracování výrazů
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 28.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "expressions.h"

char prec_table[7][7] =
{
    {']', '[', '[', ']', ']', '[', ']'},   // +-
    {']', ']', '[', ']', ']', '[', ']'},   // */ //
    {'[', '[', '[', '=', '[', '[', 'U'},   // (
    {']', ']', 'U', ']', ']', 'U', ']'},   // )
    {'[', '[', '[', ']', ']', '[', ']'},   // RELATION_OP
    {']', ']', 'U', ']', ']', 'U', ']'},   // VALUE
    {'[', '[', '[', 'U', '[', '[', 'U'},   // $
};


void clean_resources(tParser_data* parser_data, tPrec_stack* prec_stack)
{
    tPrec_stack_clean(prec_stack);
    parser_data->unget_token = true;
}


//TODO zpracování terminálu ze zásobníku a získání hodnotu z enumu pro určení indexu v tabulce

Prec_token process_terminal(tPrec_stack* prec_stack) //získáme index v řádku pro určení pravidla
{
    tSymbol top_term = tPrec_stack_top_term(prec_stack);
    Token_type symbol_type = top_term->value_type;

    switch(symbol_type)
    {
        //podovně jak u process token PROMYSLET!!
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return PREC_PLUS_MINUS;

        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE_FLOAT:
        case TOKEN_DIVIDE_INTEGER:
            return PREC_MUL_DIVS;

        case TOKEN_LEFT_BRACKET:
            return PREC_L_BRACKET;

        case TOKEN_RIGHT_BRACKET:
            return PREC_R_BRACKET;            

        case TOKEN_LESS_THAN:
        case TOKEN_GREATER_THAN:
        case TOKEN_LESS_OR_EQUAL:
        case TOKEN_GREATER_OR_EQUAL:
        case TOKEN_NOT_EQUAL:
        case TOKEN_EQUAL:
            return PREC_RELATION_OP;

        case TOKEN_INTEGER:
        case TOKEN_DOUBLE:
        case TOKEN_STRING_LITERAL:
        case TOKEN_IDENTIFIER:
            return PREC_VALUE;

        case TOKEN_KEYWORD:
            if(!strcmp(top_term->attribute->str, "None"))
            {
                return PREC_VALUE;
            }
            else
            {
                return PREC_OTHER;
            }


        default:
            return PREC_OTHER;
    }
}


Prec_token process_token(tParser_data* parser_data) //získáme index v sloupci pro určení pravidla
{
    Token_type type = parser_data->current_token->type;
    switch(type)
    {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return PREC_PLUS_MINUS;

        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE_FLOAT:
        case TOKEN_DIVIDE_INTEGER:
            return PREC_MUL_DIVS;

        case TOKEN_LEFT_BRACKET:
            return PREC_L_BRACKET;

        case TOKEN_RIGHT_BRACKET:
            return PREC_R_BRACKET;            

        case TOKEN_LESS_THAN:
        case TOKEN_GREATER_THAN:
        case TOKEN_LESS_OR_EQUAL:
        case TOKEN_GREATER_OR_EQUAL:
        case TOKEN_NOT_EQUAL:
        case TOKEN_EQUAL:
            return PREC_RELATION_OP;

        case TOKEN_INTEGER:
        case TOKEN_DOUBLE:
        case TOKEN_STRING_LITERAL:
        case TOKEN_IDENTIFIER:
            return PREC_VALUE;

        case TOKEN_KEYWORD:
            if(!strcmp(parser_data->current_token->attribute->str, "None"))
            {
                return PREC_VALUE;
            }
            else
            {
                return PREC_OTHER;
            }


        default:
            return PREC_OTHER;
    }
}

//TODO zpracování terminálu ze zásobníku spolu se znakem na vstupu podle pravidla z tabulky

bool process_prec_table(tParser_data* parser_data, tPrec_stack* prec_stack, Prec_token prec_term_type, Prec_token prec_token_type)
{
    char table_symbol = prec_table[prec_term_type][prec_token_type];

    switch(table_symbol)    
    {
        case '=':
            if(!tPrec_stack_push(prec_stack, SYMBOL_TERMINAL, parser_data->current_token->type, parser_data->current_token->attribute->str)) //internal error, pokud se symbol nepodaří vložit na zásobník
            {
                set_error_code(parser_data, INTERNAL_ERROR);
                return false;
            }
            get_token_and_set_error_code(parser_data);
            if (parser_data->error_code != NO_ERROR)
            {
                return false;
            }

            return true;

        case '[':
            if (!tPrec_stack_push_handle(prec_stack))
            {
                set_error_code(parser_data, INTERNAL_ERROR);
                return false;
            }

            if(!tPrec_stack_push(prec_stack, SYMBOL_TERMINAL, parser_data->current_token->type, parser_data->current_token->attribute->str)) //internal error, pokud se symbol nepodaří vložit na zásobník
            {
                set_error_code(parser_data, INTERNAL_ERROR);
                return false;
            }

            get_token_and_set_error_code(parser_data);
            if (parser_data->error_code != NO_ERROR)
            {
                return false;
            }

            return true;

        case ']':

            if (prec_stack->top->next_ptr->type == SYMBOL_HANDLE)
            {
                tPrec_stack_pop_handle(prec_stack, DERIVATION_VALUE);
                if(!tPrec_stack_push(prec_stack, SYMBOL_NONTERMINAL, UNDEFINED, "E")) //internal error, pokud se symbol nepodaří vložit na zásobník
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }

                return true;
            }
            else if(prec_stack->top->next_ptr->next_ptr->next_ptr->type == SYMBOL_HANDLE)
            {
                tPrec_stack_pop_handle(prec_stack, DERIVATION_RULE);

                if(!tPrec_stack_push(prec_stack, SYMBOL_NONTERMINAL, UNDEFINED, "E")) //internal error, pokud se symbol nepodaří vložit na zásobník
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }

                return true;
            }
            else
            {
                set_error_code(parser_data, SYNTAX_ERROR);
                return false;
            }

        case 'U':
            set_error_code(parser_data, SYNTAX_ERROR);
            return false;

        default:
            set_error_code(parser_data, INTERNAL_ERROR);
            return false;
    }
}


bool process_expression(tParser_data* parser_data)
{

    if(parser_data->error_code != NO_ERROR)
    {
        return false;  
    }

    parser_data->unget_token = false;

    /***********************/
    /*  ZPRACOVÁNÍ VÝRAZU  */
    /***********************/

    tPrec_stack prec_stack;
    tPrec_stack_init(&prec_stack);

    if(!tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, UNDEFINED, "$")) //internal error, pokud se nepodaří vložit $ na zásobník
    {
        clean_resources(parser_data, &prec_stack);
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }

    //  ZÍSKÁNÍ INDEXU NA ZÁSOBNÍKU  //
    Prec_token prec_term_type = process_terminal(&prec_stack);

    //  ZÍSKÁNÍ INDEXU INPUTU  //
    Prec_token prec_token_type = process_token(parser_data);

    //  TOP TERMINÁL  //    
    tSymbol term = tPrec_stack_top_term(&prec_stack);

    if ((!strcmp(term->attribute->str, "$")) && (prec_token_type == PREC_OTHER))
    {
        clean_resources(parser_data, &prec_stack);
        set_error_code(parser_data, SYNTAX_ERROR);
        return false;
    }

    do
    {
        if (process_prec_table(parser_data, &prec_stack, prec_term_type, prec_token_type))
        {
            term = tPrec_stack_top_term(&prec_stack); //top terminál
            prec_term_type = process_terminal(&prec_stack); //znak na zásobnníku
            prec_token_type = process_token(parser_data); //znak na vstupu
        }
        else
        {
            clean_resources(parser_data, &prec_stack);
            return false;
        }
    } while ((strcmp(term->attribute->str, "$") != 0) || (prec_token_type != PREC_OTHER));

    clean_resources(parser_data, &prec_stack);
    return true;
}

