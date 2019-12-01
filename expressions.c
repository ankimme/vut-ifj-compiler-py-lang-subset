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
#include "scanner.h"
#include "symtable.h"
#include "precedent_stack.h"



char prec_table[8][8] =
{
    {']', '[', '[', '[', ']', ']', '[', ']'},   // +-
    {']', ']', ']', '[', ']', ']', '[', ']'},   // */
    {']', '[', ']', '[', ']', ']', '[', ']'},   // //
    {'[', '[', '[', '[', '=', '[', '[', 'U'},   // (
    {']', ']', ']', 'U', ']', ']', 'U', ']'},   // )
    {'[', '[', '[', '[', ']', ']', '[', ']'},   // RELATION_OP
    {']', ']', ']', 'U', ']', ']', 'U', ']'},   // VALUE
    {'[', '[', '[', '[', 'U', '[', '[', 'U'},   // $
};

void clean_resources(tParser_data* parser_data, Errors err, tPrec_stack* prec_stack);
void process_prec_table(tParser_data* parser_data, tPrec_stack* prec_stack, Prec_token prec_term_type, Prec_token prec_token_type);

void clean_resources(tParser_data* parser_data, Errors err, tPrec_stack* prec_stack)
{
    tPrec_stack_clean(prec_stack);
    parser_data->error_code = err;
}


//TODO zpracování terminálu ze zásobníku a získání hodnotu z enumu pro určení indexu v tabulce
/*
Prec_token process_terminal(tParser_data* parser_data)
{



}
*/

Prec_token process_token(tParser_data* parser_data)
{
    Token_type type = parser_data->current_token->type;
    switch(type)
    {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return PREC_PLUS_MINUS;

        case TOKEN_MULTIPLY:
        case TOKEN_DIVIDE_FLOAT:
            return PREC_MUL_DIV_FLOAT;

        case TOKEN_DIVIDE_INTEGER:
            return PREC_DIV_INT;

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
        case TOKEN_KEYWORD:
            if(!strcmp(parser_data->current_token->attribute->str, "None"))
            {
                return PREC_VALUE;
            }
            else
            {
                return PREC_OTHER;
            }


            return PREC_VALUE;

        default:
            return PREC_OTHER;
    }
}

//TODO zpracování terminálu ze zásobníku spolu se znakem na vstupu podle pravidla z tabulky
/*
void process_prec_table(tParser_data* parser_data, tPrec_stack* prec_stack, Prec_token prec_term_type, Prec_token prec_token_type)
{
    char table_symbol = prec_table[prec_term_type][prec_token_type];

    switch(table_symbol)    
    {
        case '[':
        case ']':
        case '=':
        case 'U':

        default:
    }
    
}

*/


bool process_expression(tParser_data* parser_data)
{

    if(&parser_data->error_code != NO_ERROR)
    {
        return false;  
    }

    tPrec_stack prec_stack;
    tPrec_stack_init(&prec_stack);

    if(!tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, "$")) //internal error, pokud se nepodaří vložit $ na zásobník
    {
        clean_resources(parser_data, INTERNAL_ERROR, &prec_stack);
        return false;
    }

    
    tSymbol term = tPrec_stack_top_term(&prec_stack); //top terminál //TODO opravit s užitím fce process terminal k získání indexu
    Prec_token prec_token_type = process_token(parser_data); //znak na vstupu 

    while ((strcmp(term->str, "$") != 0) && (prec_token_type != PREC_OTHER))
    {
        term = tPrec_stack_top_term(&prec_stack); //top terminál //TODO, ZMĚNIT
        prec_token_type = process_token(parser_data); //znak na vstupu

        //process_table(); TODO
    }



    //TODO

    
    return true;
}

