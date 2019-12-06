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
    {'[', '[', '[', ']', 'U', '[', ']'},   // RELATION_OP
    {']', ']', 'U', ']', ']', 'U', ']'},   // VALUE
    {'[', '[', '[', 'U', '[', '[', 'U'},   // $
};



void clean_resources(tParser_data* parser_data, tPrec_stack* prec_stack)
{
    tPrec_stack_clean(prec_stack);
    parser_data->unget_token = true;
}

void deep_copy_symbol(tSymbol source, tSymbol destination)
{
    destination->next_ptr = source->next_ptr->next_ptr;
    destination->retype = source->retype;
    destination->type = source->type;
    destination->value_type = source->value_type;

    // hluboka kopie dynamic stringu
    destination->attribute = malloc(sizeof(dynamic_string));
    string_init(destination->attribute);
    string_append(destination->attribute, source->attribute->str);
}

bool derive_rule(tParser_data* parser_data, tPrec_stack* prec_stack, Token_type value_type, char* string)
{
    //syntaktické zderivování pravidla
    tPrec_stack_pop_handle(prec_stack, DERIVATION_RULE);

    if(!tPrec_stack_push(prec_stack, SYMBOL_NONTERMINAL, value_type, string)) //internal error, pokud se symbol nepodaří vložit na zásobník
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }

    return true;
}


bool check_operands_type(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol left_operand, tSymbol operator, tSymbol right_operand)
{
    Token_type l_value = left_operand->value_type;
    Token_type r_value = right_operand->value_type;
    Token_type op_value = operator->value_type;

    switch(op_value)
    {
        case TOKEN_PLUS:
            if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_STRING_LITERAL, ""))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_INTEGER, ""))
                {
                    return false;
                }
                return true;  
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                //PŘETYPOVAT
                if (l_value == TOKEN_INTEGER)
                {
                    left_operand->retype = 1;
                }
                else
                {
                    right_operand->retype = 1;
                }
                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze sčítat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }

        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_INTEGER, ""))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                //PŘETYPOVAT
                if (l_value == TOKEN_INTEGER)
                {
                    left_operand->retype = 1;
                }
                else
                {
                    right_operand->retype = 1;
                }

                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;

            }
            else
            {
                //nelze odečítat, násobit, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }

        case TOKEN_DIVIDE_INTEGER:
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_INTEGER, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //lze dělit pouze celočíselné operandy
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }

        case TOKEN_DIVIDE_FLOAT:
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                //PŘETYPOVAT
                if (l_value == TOKEN_INTEGER)
                {
                    left_operand->retype = 1;
                }
                else
                {
                    right_operand->retype = 1;
                }

                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze dělit, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }
        case TOKEN_LESS_THAN:
        case TOKEN_GREATER_THAN:
        case TOKEN_LESS_OR_EQUAL:
        case TOKEN_GREATER_OR_EQUAL:
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                //PŘETYPOVAT
                if (l_value == TOKEN_INTEGER)
                {
                    left_operand->retype = 1;
                }
                else
                {
                    right_operand->retype = 1;
                }

                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }
        case TOKEN_EQUAL:
        case TOKEN_NOT_EQUAL:
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                //PŘETYPOVAT
                if (l_value == TOKEN_INTEGER)
                {
                    left_operand->retype = 1;
                }
                else
                {
                    right_operand->retype = 1;
                }

                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //lze porovnávat asi vše
                if (!derive_rule(parser_data, prec_stack, UNDEFINED, ""))
                {
                    return false;
                }
                return true;
            }
        case TOKEN_IDENTIFIER:
            if (l_value == TOKEN_LEFT_BRACKET && r_value == TOKEN_RIGHT_BRACKET)
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_IDENTIFIER, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }
        case TOKEN_KEYWORD:
            if (l_value == TOKEN_LEFT_BRACKET && r_value == TOKEN_RIGHT_BRACKET)
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_KEYWORD, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }
        case TOKEN_INTEGER:
            if (l_value == TOKEN_LEFT_BRACKET && r_value == TOKEN_RIGHT_BRACKET)
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_INTEGER, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }
        case TOKEN_DOUBLE:
            if (l_value == TOKEN_LEFT_BRACKET && r_value == TOKEN_RIGHT_BRACKET)
            {
                if (!derive_rule(parser_data, prec_stack, TOKEN_DOUBLE, ""))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                return false;
            }
            


        default:
            set_error_code(parser_data, SYNTAX_ERROR);
            return false;
    }
}

//TODO
bool semantic_node_value(tSymbol value)
{
    if (value->type == SYMBOL_TERMINAL)
    {
        return true;
    }
    return false;
}


bool semantic_node_rule(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol left_sym, tSymbol middle_sym, tSymbol right_sym)
{
    Token_type rule_type = middle_sym->value_type;

    switch(rule_type)
    {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_MULTIPLY:
            if(check_operands_type(parser_data, prec_stack, left_sym, middle_sym, right_sym))
            {
                return true;
            }
            else
            {
                return false;
            }

        case TOKEN_DIVIDE_INTEGER:
        case TOKEN_DIVIDE_FLOAT:
            if(!strcmp(right_sym->attribute->str, "0"))
            {
                set_error_code(parser_data, DIVISION_BY_ZERO_ERROR);
                return false;
            }
            else if (check_operands_type(parser_data, prec_stack, left_sym, middle_sym, right_sym))
            {
                return true;
            }
            else
            {
                return false;
            }


        case TOKEN_LESS_THAN:
        case TOKEN_GREATER_THAN:
        case TOKEN_LESS_OR_EQUAL:
        case TOKEN_GREATER_OR_EQUAL:
        case TOKEN_EQUAL:
        case TOKEN_NOT_EQUAL:
            if(check_operands_type(parser_data, prec_stack, left_sym, middle_sym, right_sym))
            {
                return true;
            }
            else
            {
                return false;
            }

        case TOKEN_IDENTIFIER:
        case TOKEN_KEYWORD:
        case TOKEN_INTEGER:
        case TOKEN_DOUBLE:
            if(check_operands_type(parser_data, prec_stack, left_sym, middle_sym, right_sym))
            {
                return true;
            }
            else
            {
                return false;
            }

        default:
            set_error_code(parser_data, SYNTAX_ERROR);
            return false;
    }

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

            //kontrola zda se nesnažíme o vložení samostatného operátoru mezi závorky
        
            if ((prec_stack->top->value_type == TOKEN_LEFT_BRACKET) && (prec_token_type != PREC_VALUE))
            {
                set_error_code(parser_data, SYNTAX_ERROR);
                return false;
            }

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

            //pravidlo E -> i
            if ((prec_stack->top->next_ptr->type == SYMBOL_HANDLE) && (prec_term_type == PREC_VALUE))
            {

                //sémantická kontrola pravidla, přetypování

                if (!semantic_node_value(prec_stack->top))
                {
                    return false;
                }

                //zpracování pravidla
                tSymbol val = malloc(sizeof(struct symbol)); // TODO
                deep_copy_symbol(prec_stack->top, val);
                tPrec_stack_pop_handle(prec_stack, DERIVATION_VALUE);
                //internal error, pokud se symbol nepodaří vložit na zásobník
                if(!tPrec_stack_push(prec_stack, SYMBOL_NONTERMINAL, val->value_type, val->attribute->str))
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }
                string_free(val->attribute);
                free(val->attribute);
                free(val);
                val = NULL;

                

                return true;
            }
            // jiné pravidlo stylu E -> E op E nebo E -> (E)
            else if(prec_stack->top->next_ptr->next_ptr->next_ptr->type == SYMBOL_HANDLE)
            {
                tSymbol leftmost_sym = prec_stack->top->next_ptr->next_ptr; //nejlevější hodnota pravidla pro zderivování
                tSymbol middle_sym = prec_stack->top->next_ptr; //prostřední hodnota pravidla
                tSymbol rightmost_sym = prec_stack->top; //nejpravější hodnota pravidla pro zderivování

                // if ((leftmost_sym->value_type != UNDEFINED) && (leftmost_sym->value_type != TOKEN_LEFT_BRACKET) && (rightmost_sym->value_type != UNDEFINED) && (rightmost_sym->value_type != TOKEN_RIGHT_BRACKET))
                // {
                //     set_error_code(parser_data, SYNTAX_ERROR);
                //     return false;
                // } DELETE
                
                //sémantická kontrola pravidla, přetypování

                if (!semantic_node_rule(parser_data, prec_stack, leftmost_sym, middle_sym, rightmost_sym))
                {
                    return false;
                }
                /*
                //syntaktické zderivování pravidla

                tPrec_stack_pop_handle(prec_stack, DERIVATION_RULE);

                if(!tPrec_stack_push(prec_stack, SYMBOL_NONTERMINAL, UNDEFINED, "E")) //internal error, pokud se symbol nepodaří vložit na zásobník
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }
                */

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

    //Na vstupu je symbol, jímž výraz nemůže začínat, chyba
    if (((!strcmp(term->attribute->str, "$")) && (prec_token_type == PREC_OTHER)) || ((prec_token_type != PREC_VALUE) && (prec_token_type != PREC_L_BRACKET)))
    {
        clean_resources(parser_data, &prec_stack);
        set_error_code(parser_data, SYNTAX_ERROR);
        return false;
    }

    //načítání symbolů ze vstupu a zpracování výrazu
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

