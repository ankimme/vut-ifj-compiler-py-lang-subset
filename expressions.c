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

#define FREE_S \
    if(symb_rule) \
    { \
        if(symb_rule->attribute) \
        { \
            string_free(symb_rule->attribute); \
            free(symb_rule->attribute); \
        } \
        free(symb_rule); \
    } \
    symb_rule = NULL; \

void clean_resources(tParser_data* parser_data, tPrec_stack* prec_stack)
{
    tPrec_stack_clean(prec_stack);
    parser_data->unget_token = true;
    st_dedent(parser_data->symtable);
    parser_data->table_r_value = NULL;
}

bool search_variable_in_symtable(tParser_data* parser_data)
{
    parser_data->table_r_value = st_search_entry(parser_data->symtable, parser_data->current_token->attribute);

    if (parser_data->table_r_value == NULL)
    {
        set_error_code(parser_data, DEFINITION_SEMANTIC_ERROR);
        return false;
    }
    
    if (parser_data->table_r_value->type != HT_TYPE_VARIABLE)
    {
        set_error_code(parser_data, DEFINITION_SEMANTIC_ERROR);
        return false;
    }

    if (parser_data->table_r_value->var_data->type == UNDEFINED)
    {
        set_error_code(parser_data, DEFINITION_SEMANTIC_ERROR);
        return false;
    }

    return true;
}


bool process_types_no_retype(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol symb_rule, tNodeData* l_ptr, tNodeData* r_ptr, tSymbol next, int retype, Prec_stack_symbol type, Token_type value_type, char* str)
{
    //vytvoří neterminál simulující vložení zderivovaného pravidla na zásobník
    if(!deep_copy_rule(symb_rule, next, retype, type, value_type, str))
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        FREE_S
        return false;
    }

    //derivace pravidla
    if (!derive_rule(parser_data, prec_stack, symb_rule, l_ptr, r_ptr))
    {
        FREE_S
        return false;
    }

    FREE_S
    return true;
}

bool process_types_one_retype(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol symb_rule, tNodeData* l_ptr, tNodeData* r_ptr, Token_type l_value, tSymbol next, int retype, Prec_stack_symbol type, Token_type value_type, char* str)
{
    //vytvoří neterminál simulující vložení zderivovaného pravidla na zásobník
    if(!deep_copy_rule(symb_rule, next, retype, type, value_type, str))
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        FREE_S
        return false;
    }

    //derivace pravidla
    if (!derive_rule(parser_data, prec_stack, symb_rule, l_ptr, r_ptr))
    {
        FREE_S
        return false;
    }

    //nutno jeden operand přetypovat
    if (l_value == TOKEN_INTEGER)
    {
        prec_stack->top->item->lptr->retype = 1;
    }
    else
    {
        prec_stack->top->item->rptr->retype = 1;
    }

    FREE_S
    return true;
}

bool process_types_both_retype(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol symb_rule, tNodeData* l_ptr, tNodeData* r_ptr, tSymbol next, int retype, Prec_stack_symbol type, Token_type value_type, char* str)
{
    //vytvoří neterminál simulující vložení zderivovaného pravidla na zásobník
    if(!deep_copy_rule(symb_rule, next, retype, type, value_type, str))
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        FREE_S
        return false;
    }

    //derivace pravidla
    if (!derive_rule(parser_data, prec_stack, symb_rule, l_ptr, r_ptr))
    {
        FREE_S
        return false;
    }

    //nutno přetypovat oba operandy na float
    prec_stack->top->item->lptr->retype = 1;
    prec_stack->top->item->rptr->retype = 1;

    FREE_S
    return true;
}

bool deep_copy_rule(tSymbol destination, tSymbol next, int retype, Prec_stack_symbol type, Prec_token value_type, char* str)
{
    
    destination->next_ptr = next;
    destination->retype = retype;
    destination->type = type;
    destination->value_type = value_type;
    destination->item = NULL;
    // hluboka kopie dynamic stringu
    destination->attribute = malloc(sizeof(dynamic_string));
    if (destination->attribute == NULL)
    {
        return false;
    }
    string_init(destination->attribute);
    if(!string_append(destination->attribute, str))
    {
        return false;
    }
    return true;
}


bool deep_copy_value(tSymbol source, tSymbol destination)
{
    destination->next_ptr = source->next_ptr->next_ptr;
    destination->retype = source->retype;
    destination->type = source->type;
    destination->value_type = source->value_type;
    destination->item = source->item;
    // hluboka kopie dynamic stringu
    destination->attribute = malloc(sizeof(dynamic_string));
    if (destination->attribute == NULL)
    {
        return false;
    }
    string_init(destination->attribute);
    if(!string_append(destination->attribute, source->attribute->str))
    {
        return false;
    }
    return true;
}

tNodeData* st_create_node(tSymtable *symtable, tSymbol symbol, tNodeData *lptr, tNodeData *rptr)
{
    tNodeData *data = malloc(sizeof(tNodeData));
    data->attribute = malloc(sizeof(dynamic_string));
    string_init(data->attribute);
    if(!string_append(data->attribute, symbol->attribute->str))
    {
        return NULL;
    }
    data->retype = symbol->retype;
    data->value_type = symbol->value_type;
    data->lptr = lptr;
    data->rptr = rptr;
    return st_insert_entry_in_current_context_random_key (symtable, data, HT_TYPE_NODE)->node_data;
}

bool derive_rule(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol val, tNodeData* l_ptr, tNodeData* r_ptr)
{
    
    tNodeData *data = st_create_node(parser_data->symtable, val, l_ptr, r_ptr);
    if (data == NULL)
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }
    val->item = data;

    //syntaktické zderivování pravidla
    tPrec_stack_pop_handle(prec_stack, DERIVATION_RULE);

    if(!tPrec_stack_push_nonterminal(prec_stack, val)) //internal error, pokud se symbol nepodaří vložit na zásobník
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }

    return true;
}

void post_order_gen_code(tParser_data* parser_data, tNodeData *node)
{
    if (node == NULL)
    {
        return;
    }
    post_order_gen_code(parser_data, node->lptr);
    post_order_gen_code(parser_data, node->rptr);

    process_node(node);
}

void process_node(tNodeData *item)
{
    // tHash_Table_Item *ht_item;
    switch (item->value_type)
    {
        case TOKEN_INTEGER:
        case TOKEN_DOUBLE:
        case TOKEN_STRING_LITERAL:
        case TOKEN_KEYWORD:
        case TOKEN_IDENTIFIER:
            // dosel uzel scitani
            if (strcmp(item->attribute->str, "+") == 0)
            {
                generate_adds();
            }
            // dosel uzel odcitani
            else if (strcmp(item->attribute->str, "-") == 0)
            {
                generate_subs();
            }
            // dosel uzel nasobeni
            else if (strcmp(item->attribute->str, "*") == 0)
            {
                generate_muls();
            }
            // dosel uzel deleni celych cisel
            else if (strcmp(item->attribute->str, "//") == 0)
            {
                generate_idivs();
            }
            // dosel uzel deleni desetinnych cisel
            else if (strcmp(item->attribute->str, "/") == 0)
            {
                generate_divs();
            }
            // dosel uzel "=="
            else if (strcmp(item->attribute->str, "==") == 0)
            {
                generate_EQS();
            }
            // dosel uzel "<="
            else if (strcmp(item->attribute->str, "<=") == 0)
            {
                // generate_LTEQ();
            }
            // dosel uzel ">="
            else if (strcmp(item->attribute->str, ">=") == 0)
            {
                // generate_GTEQ();
            }
            // dosel uzel "<"
            else if (strcmp(item->attribute->str, "<") == 0)
            {
                generate_LTS();
            }
            // dosel uzel ">"
            else if (strcmp(item->attribute->str, ">") == 0)
            {
                generate_GTS();
            }
            // doslo cislo nebo promenna
            else
            {
                if (item->value_type == TOKEN_KEYWORD)
                {
                    generate_pushs(item->attribute->str, TOKEN_KEYWORD);
                }
                // prvni znak je a-zA-Z_ -> jedna se o identifikator
                else if ((item->attribute->str[0] >= 65 && item->attribute->str[0] <= 90) || (item->attribute->str[0] >= 97 && item->attribute->str[0] <= 122) || (item->attribute->str[0] == '_'))
                {
                    generate_pushs(item->attribute->str, TOKEN_IDENTIFIER);
                }
                else
                {
                    generate_pushs(item->attribute->str, item->value_type);
                }

            }
            if (item->retype)
            {
                conv_int_to_float_stack();
            }
            break;
            
        default:
            break;
    }
}

bool check_operands_type(tParser_data* parser_data, tPrec_stack* prec_stack, tSymbol left_operand, tSymbol operator, tSymbol right_operand)
{
    Token_type l_value = left_operand->value_type;
    Token_type r_value = right_operand->value_type;
    Token_type op_value = operator->value_type;

    //alokuje paměť pro neterminál vkládaný na zásobník
    tSymbol symb_rule = malloc(sizeof(struct symbol));
    if (symb_rule == NULL)
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }
    symb_rule->attribute = NULL;
    symb_rule->item = NULL;
    symb_rule->next_ptr = NULL;

    tSymbol next = right_operand->next_ptr->next_ptr;
    
    switch(op_value)
    {
        case TOKEN_PLUS: //E -> E + E
            if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                //kontrola přetypování a zderivování pravidla bez nutnosti přetypování jednoho z operandů
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_STRING_LITERAL, "+"))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_INTEGER, "+"))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "+"))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                //kontrola přetypování a zderivování pravidla s nutností jednoho přetypování
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "+"))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze sčítat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }

        case TOKEN_MINUS: //E -> E - E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_INTEGER, "-"))
                {
                    return false;
                }
                return true; 
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "-"))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "-"))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze odečítat, násobit, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }
            
        case TOKEN_MULTIPLY: //E -> E * E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_INTEGER, "*"))
                {
                    return false;
                }
                return true; 
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "*"))
                {
                    return false;
                }
                return true; 
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "*"))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze odečítat, násobit, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }

        case TOKEN_DIVIDE_INTEGER: //E -> E // E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_INTEGER, "//"))
                {
                    return false;
                }
                return true; 
            }
            else
            {
                //lze dělit pouze celočíselné operandy
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }

        case TOKEN_DIVIDE_FLOAT: // E -> E / E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                //kontrola přetypování a zderivování pravidla s nutností dvou přetypování
                if(!process_types_both_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "/"))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "/"))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, TOKEN_DOUBLE, "/"))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze dělit, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }
        case TOKEN_LESS_THAN: // E -> E < E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<"))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<"))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<"))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<"))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }
            
        case TOKEN_GREATER_THAN: // E -> E > E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">"))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">"))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">"))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">"))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }
        case TOKEN_LESS_OR_EQUAL: // E -> E <= E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<="))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<="))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<="))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "<="))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }
        case TOKEN_GREATER_OR_EQUAL: // E -> E >= E
            if ((l_value == TOKEN_INTEGER) && (r_value == TOKEN_INTEGER))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">="))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_DOUBLE))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">="))
                {
                    return false;
                }
                return true;
            }
            else if ((l_value == TOKEN_STRING_LITERAL) && (r_value == TOKEN_STRING_LITERAL))
            {
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">="))
                {
                    return false;
                }
                return true;
            }
            else if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, ">="))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }
        case TOKEN_EQUAL: // E -> E == E
            if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "=="))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //lze porovnávat asi vše
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "=="))
                {
                    return false;
                }
                return true;
            }
        case TOKEN_NOT_EQUAL: // E -> E != E
            if (((l_value == TOKEN_INTEGER) && (r_value == TOKEN_DOUBLE)) || ((l_value == TOKEN_DOUBLE) && (r_value == TOKEN_INTEGER)))
            {
                if(!process_types_one_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, l_value, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "!="))
                {
                    return false;
                }
                return true;
            }
            else
            {
                //lze porovnávat asi vše
                if(!process_types_no_retype(parser_data, prec_stack, symb_rule, left_operand->item, right_operand->item, next, 0, SYMBOL_NONTERMINAL, UNDEFINED, "!="))
                {
                    return false;
                }
                return true;
            }
        case TOKEN_IDENTIFIER:
        case TOKEN_KEYWORD:
        case TOKEN_INTEGER:
        case TOKEN_DOUBLE:
        case TOKEN_STRING_LITERAL:
            if (l_value == TOKEN_LEFT_BRACKET && r_value == TOKEN_RIGHT_BRACKET)
            {
                //vyhodí pravou závorku
                tPrec_stack_pop(prec_stack);
                
                //uvolní ze zásobníku symboly za symbolem mezi závorkami a prováže ukazatel symbolu mezi závorkami na symbol za handle závorek na zásobníku
                tPrec_stack_pop_after_top(prec_stack);
                tPrec_stack_pop_after_top(prec_stack);
                
                FREE_S
                return true;
            }
            else
            {
                //nelze porovnávat dané operandy, chyba
                set_error_code(parser_data, RUNTIME_SEMANTIC_ERROR);
                FREE_S
                return false;
            }

        default:
            set_error_code(parser_data, SYNTAX_ERROR);
            return false;
    }
}


bool semantic_node_value(tParser_data* parser_data, tSymbol value, tSymbol backup)
{
    tNodeData *data = st_create_node(parser_data->symtable, value, NULL, NULL);
    if (data == NULL)
    {
        return false;
    }
    backup->item = data;
    return true;
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
        case TOKEN_STRING_LITERAL:
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


Prec_token process_terminal(tPrec_stack* prec_stack) //získáme index v řádku pro určení pravidla
{
    tSymbol top_term = tPrec_stack_top_term(prec_stack);
    Token_type symbol_type = top_term->value_type;

    switch(symbol_type)
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


bool process_prec_table(tParser_data* parser_data, tPrec_stack* prec_stack, Prec_token prec_term_type, Prec_token prec_token_type)
{
    char table_symbol = prec_table[prec_term_type][prec_token_type];

    switch(table_symbol)    
    {
        case '=':
            
            //vkládání symbolu typu proměnné na zásobník
            if (parser_data->current_token->type == TOKEN_IDENTIFIER)
            {
                if(!tPrec_stack_push(prec_stack, SYMBOL_TERMINAL, parser_data->table_r_value->var_data->type, parser_data->table_r_value->key->str)) //internal error, pokud se symbol nepodaří vložit na zásobník
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }        
            }
            //vkládání jiného symbolu na zásobník
            else
            {
                if(!tPrec_stack_push(prec_stack, SYMBOL_TERMINAL, parser_data->current_token->type, parser_data->current_token->attribute->str)) //internal error, pokud se symbol nepodaří vložit na zásobník
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }    
            }
            
            get_token_and_set_error_code(parser_data);
            if (parser_data->error_code != NO_ERROR)
            {
                return false;
            }

            if (parser_data->current_token->type == TOKEN_IDENTIFIER)
            {
                if (!search_variable_in_symtable(parser_data))
                {
                    return false;
                }
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

            //vkládání symbolu typu proměnné na zásobník
            if (parser_data->current_token->type == TOKEN_IDENTIFIER)
            {
                if(!tPrec_stack_push(prec_stack, SYMBOL_TERMINAL, parser_data->table_r_value->var_data->type, parser_data->table_r_value->key->str)) //internal error, pokud se symbol nepodaří vložit na zásobník
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }        
            }
            //vkládání jiného symbolu na zásobník
            else
            {
                if(!tPrec_stack_push(prec_stack, SYMBOL_TERMINAL, parser_data->current_token->type, parser_data->current_token->attribute->str)) //internal error, pokud se symbol nepodaří vložit na zásobník
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }    
            }
            

            get_token_and_set_error_code(parser_data);
            if (parser_data->error_code != NO_ERROR)
            {
                return false;
            }

            if (parser_data->current_token->type == TOKEN_IDENTIFIER)
            {
                if (!search_variable_in_symtable(parser_data))
                {
                    return false;
                }
            }

            return true;

        case ']':

            //pravidlo E -> i
            if ((prec_stack->top->next_ptr->type == SYMBOL_HANDLE) && (prec_term_type == PREC_VALUE))
            {

                //sémantická kontrola pravidla, přetypování
                tSymbol val = malloc(sizeof(struct symbol));
                if (!deep_copy_value(prec_stack->top, val))
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }

                if (!semantic_node_value(parser_data, prec_stack->top, val))
                {
                    set_error_code(parser_data, INTERNAL_ERROR);
                    return false;
                }

                //zpracování pravidla
                tPrec_stack_pop_handle(prec_stack, DERIVATION_VALUE);

                //internal error, pokud se symbol nepodaří vložit na zásobník
                if(!tPrec_stack_push_nonterminal(prec_stack, val))
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

                
                //sémantická kontrola pravidla, přetypování
                if (!semantic_node_rule(parser_data, prec_stack, leftmost_sym, middle_sym, rightmost_sym))
                {
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

    /***********************/
    /*  ZPRACOVÁNÍ VÝRAZU  */
    /***********************/

    if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        if (!search_variable_in_symtable(parser_data))
        {
            return false;
        }
    }
    

    st_indent(parser_data->symtable);

    tPrec_stack prec_stack;
    tPrec_stack_init(&prec_stack);

    if(!tPrec_stack_push(&prec_stack, SYMBOL_TERMINAL, UNDEFINED, "$")) //internal error, pokud se nepodaří vložit $ na zásobník
    {
        clean_resources(parser_data, &prec_stack);
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }

    //todo ověřit první token proměnná

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

    //přiřazení výsledku výrazu do proměnné (nebo nikam), v případě pokusu o přířazení bool hodnoty, sémantická chyba
    if (parser_data->table_l_value != NULL)
    {
        parser_data->table_l_value->var_data->type = (&prec_stack)->top->item->value_type;

        // přiřadila by se bool hodnota, tu my ale nepodporujeme
        if (parser_data->table_l_value->var_data->type == UNDEFINED)
        {
            set_error_code(parser_data, OTHER_SEMANTIC_ERROR);
            clean_resources(parser_data, &prec_stack);
            return false;
        }
    }
    post_order_gen_code(parser_data, (&prec_stack)->top->item);
    clean_resources(parser_data, &prec_stack);
    return true;
}