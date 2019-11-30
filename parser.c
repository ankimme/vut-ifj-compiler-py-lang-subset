/*
 * @file parser.c
 * @brief Implementace syntaktického analyzátoru
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 29.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "parser.h"

int start_analysis()
{
    tParser_data *parser_data = malloc(sizeof(tParser_data));
    if(!init_parser_data(parser_data))
    {
        return 99;
    }

    // nacteni prvniho tokenu
    get_next_token(parser_data->current_token, parser_data->scanner_stack);
    prog(parser_data);

    int error_code = parser_data ? parser_data->error_code : 99;
    free_parser_data(parser_data);
    free(parser_data);
    parser_data = NULL;

    return error_code;
}

bool init_parser_data(tParser_data *parser_data)
{
    // inicializace tokenu
    parser_data->current_token = malloc(sizeof(St_token));
    if (parser_data == NULL)
    {
        return false;
    }
    parser_data->current_token->type = UNDEFINED;
    parser_data->current_token->error_value = 0;
    parser_data->current_token->attribute = malloc(sizeof(dynamic_string));
    if(parser_data->current_token->attribute == NULL)
    {
        return false;
    }
    if(!string_init(parser_data->current_token->attribute))
    {
        return false;
    }

    // inicializace zasobniku pro scanner
    parser_data->scanner_stack = malloc(sizeof(tStack));
    tStack_init(parser_data->scanner_stack);
    tStack_push(parser_data->scanner_stack, 0);

    // inicializace error kodu
    parser_data->error_code = 0;

    // inicializace flagu unget_token
    parser_data->unget_token = false;

    return true;
}

void free_parser_data(tParser_data *parser_data)
{
    // uvolneni tokenu
    string_free(parser_data->current_token->attribute);
    free(parser_data->current_token->attribute);
    parser_data->current_token->attribute = NULL;
 
    free(parser_data->current_token);
    parser_data->current_token = NULL;

    // uvolneni zasobniku
    free(parser_data->scanner_stack);
    parser_data->scanner_stack = NULL;
}

bool get_token_and_set_error_code(tParser_data *parser_data)
{
    if (parser_data->unget_token)
    {
        parser_data->unget_token = false;
        return true;
    }
    get_next_token(parser_data->current_token, parser_data->scanner_stack);
    // zachyceni lexikalni chyby
    if (parser_data->current_token->error_value)
    {
        parser_data->error_code = parser_data->current_token->error_value;
        return false;
    }
    return true;
}

bool get_compare_check(tParser_data *parser_data, Token_type type)
{
    if(get_token_and_set_error_code(parser_data))
    {
        if (parser_data->current_token->type == type)
        {
            return true;
        }
        else
        {
            parser_data->error_code = 2;
        }
    }
    return false;
}

bool prog(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je klicove slovo
    if (parser_data->current_token->type == TOKEN_KEYWORD)
    {
        // token je "def" -> pravidlo 2
        if (strcmp(parser_data->current_token->attribute->str, "def") == 0)
        {
            // musi nasledovat identifikator
            if (!get_compare_check(parser_data, TOKEN_IDENTIFIER))
            {
                return false;
            }

            // musi nasledovat "("
            if (!get_compare_check(parser_data, TOKEN_LEFT_BRACKET))
            {
                return false;
            }

            // musi nasledovat neterminal PARAMS
            get_token_and_set_error_code(parser_data);
            if (!params(parser_data))
            {
                return false;
            }

            // musi nasledovat ")"
            if (!get_compare_check(parser_data, TOKEN_RIGHT_BRACKET))
            {
                return false;
            }

            // musi nasledovat ":"
            if (!get_compare_check(parser_data, TOKEN_COLON))
            {
                return false;
            }

            // musi nasledovat EOL
            if (!get_compare_check(parser_data, TOKEN_EOL))
            {
                return false;
            }

            // musi nasledovat INDENT
            if (!get_compare_check(parser_data, TOKEN_INDENT))
            {
                return false;
            }

            // musi nasledovat neterminal STATEMETS
            get_token_and_set_error_code(parser_data);
            if (!statements(parser_data))
            {
                return false;
            }

            // musi nasledovat DEDENT
            if (!get_compare_check(parser_data, TOKEN_DEDENT))
            {
                return false;
            }

            // musi nasledovat neterminal PROG
            get_token_and_set_error_code(parser_data);
            if (!prog(parser_data))
            {
                return false;
            }
        }
        //TODO
    }
    // token je EOL
    else if (parser_data->current_token->type == TOKEN_EOL)
    {
        // musi nasledovat neterminal PROG
        get_token_and_set_error_code(parser_data);
        if (!prog(parser_data))
        {
            return false;
        }
    }
    // token je EOF
    else if (parser_data->current_token->type == TOKEN_EOF)
    {
        return true;
    }
    // TODO
    else
    {
        parser_data->error_code = 2;
        return false;
    }
    
    return true;
}

bool params(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je ")" -> pravidlo 6
    if (parser_data->current_token->type == TOKEN_RIGHT_BRACKET)
    {
        parser_data->unget_token = true;
        return true;
    }
    // TODO
    else
    {
        parser_data->error_code = 2;
        return false;
    }
    
    return true;
}

bool statements(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    get_token_and_set_error_code(parser_data); // DELETE - only for testing purposes
    return true;
    // TODO
}
