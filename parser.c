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
#include "expressions.h"

int start_analysis()
{
    tParser_data *parser_data = malloc(sizeof(tParser_data));
    if(!init_parser_data(parser_data))
    {
        return 99;
    }

    // nacteni prvniho tokenu
    // get_next_token(parser_data->current_token, parser_data->scanner_stack); DELETE
    get_token_and_set_error_code(parser_data);
    bool unknown_error = !prog(parser_data); // TODO remove unknown error handling
    int error_code = unknown_error && !parser_data->error_code ? 98 : parser_data->error_code;
    free_parser_data(parser_data);
    free(parser_data);
    parser_data = NULL;

    return error_code;
}

bool init_parser_data(tParser_data *parser_data)
{
    // inicializace current_tokenu
    parser_data->current_token = malloc(sizeof(St_token));
    if (parser_data->current_token == NULL)
    {
        // todo set error everywhere
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

    // inicializace backup_tokenu
    parser_data->backup_token = malloc(sizeof(St_token));
    if (parser_data->backup_token == NULL)
    {
        return false;
    }
    parser_data->backup_token->type = UNDEFINED;
    parser_data->backup_token->error_value = 0;
    parser_data->backup_token->attribute = malloc(sizeof(dynamic_string));
    if(parser_data->backup_token->attribute == NULL)
    {
        return false;
    }
    if(!string_init(parser_data->backup_token->attribute))
    {
        return false;
    }

    // inicializace zasobniku pro scanner
    parser_data->scanner_stack = malloc(sizeof(tStack));
    if(parser_data->scanner_stack == NULL)
    {
        return false;
    }
    tStack_init(parser_data->scanner_stack);
    tStack_push(parser_data->scanner_stack, 0);

    // inicializace tabulky symbolu
    parser_data->symtable = malloc(sizeof(tSymtable));
    if(parser_data->symtable == NULL)
    {
        return false;
    }
    st_init(parser_data->symtable);

    // inicializace error kodu
    parser_data->error_code = 0;

    // inicializace flagu unget_token
    parser_data->unget_token = false;

    // inicializace flagu load_backup
    parser_data->load_backup = false;

    // inicializace flagu function_definition_scope
    parser_data->function_definition_scope = false;

    return true;
}

void free_parser_data(tParser_data *parser_data)
{
    // uvolneni current_tokenu
    string_free(parser_data->current_token->attribute);
    free(parser_data->current_token->attribute);
    parser_data->current_token->attribute = NULL;
 
    free(parser_data->current_token);
    parser_data->current_token = NULL;

    // uvolneni backup_tokenu
    string_free(parser_data->backup_token->attribute);
    free(parser_data->backup_token->attribute);
    parser_data->backup_token->attribute = NULL;
 
    free(parser_data->backup_token);
    parser_data->backup_token = NULL;

    // uvolneni zasobniku
    free(parser_data->scanner_stack);
    parser_data->scanner_stack = NULL;

    // uvolneni tabulky symbolu
    st_clean_all(parser_data->symtable);
    free(parser_data->symtable);
    parser_data->symtable = NULL;
}

bool get_token_and_set_error_code(tParser_data *parser_data)
{
    //TODO A
    if (parser_data->load_backup && parser_data->unget_token)
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }

    if (parser_data->load_backup)
    {
        copy_token(parser_data, parser_data->backup_token, parser_data->current_token);
        parser_data->load_backup = false;
        return true;
    }

    if (parser_data->unget_token)
    {
        parser_data->unget_token = false;
        return true;
    }
    get_next_token(parser_data->current_token, parser_data->scanner_stack);
    // zachyceni lexikalni chyby
    if (parser_data->current_token->error_value)
    {
        set_error_code(parser_data, parser_data->current_token->error_value);
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
            set_error_code(parser_data, 2);
        }
    }
    return false;
}

bool get_compare_check_double(tParser_data *parser_data, Token_type type1, Token_type type2)
{
    if(get_token_and_set_error_code(parser_data))
    {
        if (parser_data->current_token->type == type1 || parser_data->current_token->type == type2)
        {
            return true;
        }
        else
        {
            set_error_code(parser_data, 2);
        }
    }
    return false;
}

void set_error_code(tParser_data *parser_data, int new_code)
{
    if (parser_data->error_code == 0)
    {
        parser_data->error_code = new_code;
    }
}

bool copy_token(tParser_data *parser_data, St_token* t1, St_token* t2)
{
    t2->type = t1->type;
    t2->error_value = t1->error_value;

    string_clear(t2->attribute);
    // t2->attribute->allocated_size = t1->attribute->allocated_size;
    t2->attribute->length = t1->attribute->length;
    if(!string_append(t2->attribute, t1->attribute->str))
    {
        set_error_code(parser_data, INTERNAL_ERROR);
        return false;
    }
    return true;
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

            // ignoruje EOLy
            do
            {
                get_token_and_set_error_code(parser_data);
            } while (parser_data->current_token->type == TOKEN_EOL);
            parser_data->unget_token = true;

            // musi nasledovat INDENT
            if (!get_compare_check(parser_data, TOKEN_INDENT))
            {
                return false;
            }

            parser_data->function_definition_scope = true;

            // musi nasledovat neterminal STATEMETS
            get_token_and_set_error_code(parser_data);
            if (!statements(parser_data))
            {
                return false;
            }

            parser_data->function_definition_scope = false;

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
        // token je "if", "while", "pass", "None" -> pravidlo 1
        else if (strcmp(parser_data->current_token->attribute->str, "if") == 0 || strcmp(parser_data->current_token->attribute->str, "while") == 0 || strcmp(parser_data->current_token->attribute->str, "pass") == 0 || strcmp(parser_data->current_token->attribute->str, "None") == 0)
        {
            // musi nasledovat neterminal SEQUENCE
            // get_token_and_set_error_code(parser_data); DELETE
            if (!sequence(parser_data))
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
        else
        {
            set_error_code(parser_data, 2);
            return false;
        }
    }
    // token je identifikator -> pravidlo 1
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat neterminal SEQUENCE
        // get_token_and_set_error_code(parser_data);
        if (!sequence(parser_data))
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
    // token je EOL -> pravidlo 4
    else if (parser_data->current_token->type == TOKEN_EOL)
    {
        // musi nasledovat neterminal PROG
        get_token_and_set_error_code(parser_data);
        if (!prog(parser_data))
        {
            return false;
        }
    }
    // token je EOF -> pravidlo 3
    else if (parser_data->current_token->type == TOKEN_EOF)
    {
        return true;
    }
    // token je pravdepodobne expression -> pravidlo 1
    else // TODO expr
    {
        // musi nasledovat neterminal SEQUENCE
        // get_token_and_set_error_code(parser_data); DELETE
        if (!sequence(parser_data))
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
    // else DELETE
    // {
    //     set_error_code(parser_data, 2);
    //     return false;
    // }
    
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
    // token je identifikator -> pravidlo 5
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat neterminal PARAMS_N
        get_token_and_set_error_code(parser_data);
        if (!params_n(parser_data))
        {
            return false;
        }
    }
    else
    {
        set_error_code(parser_data, 2);
        return false;
    }
    
    return true;
}

bool params_n(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je "," -> pravidlo 7
    if (parser_data->current_token->type == TOKEN_COMMA)
    {
        // musi nasledovat identifikator
        if (!get_compare_check(parser_data, TOKEN_IDENTIFIER))
        {
            return false;
        }

        // musi nasledovat neterminal PARAMS_N
        get_token_and_set_error_code(parser_data);
        if (!params_n(parser_data))
        {
            return false;
        }
    }
    // token je ")" -> pravidlo 8
    else if (parser_data->current_token->type == TOKEN_RIGHT_BRACKET)
    {
        parser_data->unget_token = true;
        return true;
    }
    else
    {
        set_error_code(parser_data, 2);
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

    // token je klicove slovo
    if (parser_data->current_token->type == TOKEN_KEYWORD)
    {
        // token je "if", "while", "pass" -> pravidlo 14
        if (strcmp(parser_data->current_token->attribute->str, "if") == 0 || strcmp(parser_data->current_token->attribute->str, "while") == 0 || strcmp(parser_data->current_token->attribute->str, "pass") == 0 || strcmp(parser_data->current_token->attribute->str, "None") == 0)
        {
            // musi nasledovat neterminal SEQUENCE
            // get_token_and_set_error_code(parser_data); DELETE
            if (!sequence(parser_data))
            {
                return false;
            }

            // musi nasledovat neterminal SEQUENCE_N
            get_token_and_set_error_code(parser_data);
            if (!sequence_n(parser_data))
            {
                return false;
            }
        }
        // token je "return" -> pravidlo 15
        else if (strcmp(parser_data->current_token->attribute->str, "return") == 0 && parser_data->function_definition_scope)
        {
            // musi nasledovat neterminal FUNC_RETURN
            //get_token_and_set_error_code(parser_data); DELETE
            if (!func_return(parser_data))
            {
                return false;
            }

            // musi nasledovat neterminal SEQUENCE_N
            get_token_and_set_error_code(parser_data);
            if (!sequence_n(parser_data))
            {
                return false;
            }
        }
        else
        {
            set_error_code(parser_data, 2);
            return false;
        }
    }
    // token je identifikator -> pravidlo 14
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat neterminal SEQUENCE
        // get_token_and_set_error_code(parser_data); DELETE
        if (!sequence(parser_data))
        {
            return false;
        }

        // musi nasledovat neterminal SEQUENCE_N
        get_token_and_set_error_code(parser_data);
        if (!sequence_n(parser_data))
        {
            return false;
        }

        return true;
    }
    // token je pravdepodobne expression -> pravidlo 14
    else
    {
        // musi nasledovat neterminal SEQUENCE
        // get_token_and_set_error_code(parser_data); DELETE
        if (!sequence(parser_data))
        {
            return false;
        }

        // musi nasledovat neterminal SEQUENCE_N
        get_token_and_set_error_code(parser_data);
        if (!sequence_n(parser_data))
        {
            return false;
        }
    }
    // else DELETE
    // {
    //     set_error_code(parser_data, 2);
    //     return false;
    // }

    return true;
}

bool sequence(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je klicove slovo
    if (parser_data->current_token->type == TOKEN_KEYWORD)
    {
        // token je "if" -> pravidlo 10
        if (strcmp(parser_data->current_token->attribute->str, "if") == 0)
        {
            // musi nasledovat expression
            get_token_and_set_error_code(parser_data);
            if (!process_expression(parser_data))
            {
                return false;
            }

            // musi nasledovat ":"
            if (!get_compare_check(parser_data, TOKEN_COLON))
            {
                return false;
            }

            // musi nasledovat "EOL"
            if (!get_compare_check(parser_data, TOKEN_EOL))
            {
                return false;
            }

            // ignoruje EOLy
            do
            {
                get_token_and_set_error_code(parser_data);
            } while (parser_data->current_token->type == TOKEN_EOL);
            parser_data->unget_token = true;

            // musi nasledovat INDENT
            if (!get_compare_check(parser_data, TOKEN_INDENT))
            {
                return false;
            }

            // musi nasledovat neterminal STATEMENTS
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

            // musi nasledovat "else"
            if (!get_compare_check(parser_data, TOKEN_KEYWORD))
            {
                return false;
            }
            if (strcmp(parser_data->current_token->attribute->str, "else") != 0)
            {
                set_error_code(parser_data, SYNTAX_ERROR);
                return false;
            }

            // musi nasledovat ":"
            if (!get_compare_check(parser_data, TOKEN_COLON))
            {
                return false;
            }
            
            // musi nasledovat "EOL"
            if (!get_compare_check(parser_data, TOKEN_EOL))
            {
                return false;
            }

            // ignoruje EOLy
            do
            {
                get_token_and_set_error_code(parser_data);
            } while (parser_data->current_token->type == TOKEN_EOL);
            parser_data->unget_token = true;

            // musi nasledovat INDENT
            if (!get_compare_check(parser_data, TOKEN_INDENT))
            {
                return false;
            }

            // musi nasledovat neterminal STATEMENTS
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
        }
        // token je "while" -> pravidlo 11
        else if (strcmp(parser_data->current_token->attribute->str, "while") == 0)
        {
            // musi nasledovat expression
            get_token_and_set_error_code(parser_data);
            if (!process_expression(parser_data))
            {
                return false;
            }

            // musi nasledovat ":"
            if (!get_compare_check(parser_data, TOKEN_COLON))
            {
                return false;
            }

            // musi nasledovat "EOL"
            if (!get_compare_check(parser_data, TOKEN_EOL))
            {
                return false;
            }

            // ignoruje EOLy
            do
            {
                get_token_and_set_error_code(parser_data);
            } while (parser_data->current_token->type == TOKEN_EOL);
            parser_data->unget_token = true;

            // musi nasledovat INDENT
            if (!get_compare_check(parser_data, TOKEN_INDENT))
            {
                return false;
            }

            // musi nasledovat neterminal STATEMENTS
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
        }
        // token je "pass" -> pravidlo 13
        else if (strcmp(parser_data->current_token->attribute->str, "pass") == 0)
        {
            get_token_and_set_error_code(parser_data);
            parser_data->unget_token = true;

            if (parser_data->current_token->type == TOKEN_DEDENT)
            {
                return true;
            }

            // musi nasledovat EOL nebo EOF 
            if (!get_compare_check_double(parser_data, TOKEN_EOL, TOKEN_EOF))
            {
                return false;
            }
        }
        // token je None -> vede na expression
        else if (strcmp(parser_data->current_token->attribute->str, "None") == 0)
        {
            // musi nasledovat expression
            // get_token_and_set_error_code(parser_data); DELETE
            if (!process_expression(parser_data))
            {
                return false;
            }

            get_token_and_set_error_code(parser_data);
            if (parser_data->current_token->type == TOKEN_DEDENT)
            {
                parser_data->unget_token = true;
                return true;
            }
            // musi nasledovat "EOL" nebo "EOF"
            else if (parser_data->current_token->type != TOKEN_EOL && parser_data->current_token->type != TOKEN_EOF)
            {
                set_error_code(parser_data, SYNTAX_ERROR);
                return false;
            } 
        }
        else
        {
            set_error_code(parser_data, 2);
            return false;
        }
    }
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // TODO symtable
        if (!copy_token(parser_data, parser_data->current_token, parser_data->backup_token)) // todo
        {
            return false;
        }

        get_token_and_set_error_code(parser_data);

        // token je "(" nebo "=" -> pravidlo 12
        if (parser_data->current_token->type == TOKEN_LEFT_BRACKET || parser_data->current_token->type == TOKEN_ASSIGNMENT)
        {
            // musi nasledovat neterminal INSTRUCT
            // get_token_and_set_error_code(parser_data); DELETE
            if (!instruct(parser_data))
            {
                return false;
            }

            get_token_and_set_error_code(parser_data);
            parser_data->unget_token = true;
            // EOF HANDLING
            if (parser_data->current_token->type == TOKEN_DEDENT)
            {
                return true;
            }

            // musi nasledovat EOL
            if (!get_compare_check_double(parser_data, TOKEN_EOL, TOKEN_EOF))
            {
                return false;
            }
        }
        // token je expression -> pravidlo 9
        else
        {
            St_token* pom = parser_data->current_token;
            parser_data->current_token = parser_data->backup_token;
            parser_data->backup_token = pom;
            pom = NULL;
            parser_data->load_backup = true;



            // musi nasledovat expression
            // get_token_and_set_error_code(parser_data); DELETE
            if (!process_expression(parser_data))
            {
                return false;
            }


            get_token_and_set_error_code(parser_data);

            if (parser_data->current_token->type == TOKEN_DEDENT)
            {
                parser_data->unget_token = true;
                return true;
            }
            // musi nasledovat "EOL" nebo "EOF"
            else if (parser_data->current_token->type != TOKEN_EOL && parser_data->current_token->type != TOKEN_EOF)
            {
                set_error_code(parser_data, SYNTAX_ERROR);
                return false;
            }            
        }
    }
    else
    {
        // musi nasledovat expression
        // get_token_and_set_error_code(parser_data); DELETE
        if (!process_expression(parser_data))
        {
            return false;
        }

        get_token_and_set_error_code(parser_data);

        if (parser_data->current_token->type == TOKEN_DEDENT)
        {
            parser_data->unget_token = true;
            return true;
        }
        // musi nasledovat "EOL" nebo "EOF"
        else if (parser_data->current_token->type != TOKEN_EOL && parser_data->current_token->type != TOKEN_EOF)
        {
            set_error_code(parser_data, SYNTAX_ERROR);
            return false;
        }
    }
    /*
    // token je identifikator -> pravidlo 12
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat neterminal INSTRUCT
        get_token_and_set_error_code(parser_data);
        if (!instruct(parser_data))
        {
            return false;
        }

        // musi nasledovat EOL
        if (!get_compare_check_double(parser_data, TOKEN_EOL, TOKEN_EOF))
        {
            return false;
        }
    }
    // TODO expr
    // token je pravdepodobne expression -> pravidlo 9
    else
    {
        // musi nasledovat expression
        // get_token_and_set_error_code(parser_data); DELETE
        if (!process_expression(parser_data))
        {
            return false;
        }

        // musi nasledovat "EOL" nebo "EOF"
        get_token_and_set_error_code(parser_data);
        // if (!get_compare_check(parser_data, TOKEN_EOL)) DELETE
        if (parser_data->current_token->type != TOKEN_EOL && parser_data->current_token->type != TOKEN_EOF)
        {
            return false;
        }
    }
    */
    // else // DELETE ???
    // {
    //     set_error_code(parser_data, 2);
    //     return false;
    // }

    return true;
}

bool sequence_n(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je klicove slovo
    if (parser_data->current_token->type == TOKEN_KEYWORD)
    {
        // token je "if", "while", "pass", "return" -> pravidlo 16
        if (strcmp(parser_data->current_token->attribute->str, "if") == 0 || strcmp(parser_data->current_token->attribute->str, "while") == 0 || strcmp(parser_data->current_token->attribute->str, "pass") == 0 || strcmp(parser_data->current_token->attribute->str, "None") == 0 || (strcmp(parser_data->current_token->attribute->str, "return") == 0 && parser_data->function_definition_scope))
        {
            // musi nasledovat neterminal STATEMENTS
            // get_token_and_set_error_code(parser_data); DELETE
            if (!statements(parser_data))
            {
                return false;
            }
        }
        else
        {
            set_error_code(parser_data, 2);
            return false;
        }
    }
    // token je identifikator -> pravidlo 16
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat neterminal STATEMENTS
        // get_token_and_set_error_code(parser_data); DELETE
        if (!statements(parser_data))
        {
            return false;
        }
    }
    // token je DEDENT -> pravidlo 17
    else if (parser_data->current_token->type == TOKEN_DEDENT)
    {
        parser_data->unget_token = true;
        return true;
    }
    // token je EOL -> pravidlo 18
    else if (parser_data->current_token->type == TOKEN_EOL)
    {
        // musi nasledovat neterminal SEQUENCE_N
        get_token_and_set_error_code(parser_data);
        if (!sequence_n(parser_data))
        {
            return false;
        }
        // parser_data->unget_token = true;
        // return true;
    }
    // else if (parser_data->current_token->type == TOKEN_EOF) // TODO test feature
    // {
    //     return true;
    // }
    // TODO expr
    // token je pravdepodobne expression -> pravidlo 16
    else
    {
        // musi nasledovat neterminal STATEMENTS
        // get_token_and_set_error_code(parser_data); DELETE
        if (!statements(parser_data))
        {
            return false;
        }
    }
    // else DELETE
    // {
    //     set_error_code(parser_data, 2);
    //     return false;
    // }

    return true;
}

bool func_return(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je klicove slovo
    if (parser_data->current_token->type == TOKEN_KEYWORD)
    {
        // token je "return" -> pravidlo 35
        if (strcmp(parser_data->current_token->attribute->str, "return") == 0 && parser_data->function_definition_scope)
        {
            // musi nasledovat neterminal RETURN_VALUE
            get_token_and_set_error_code(parser_data);
            if (!return_value(parser_data))
            {
                return false;
            }
        }
        else
        {
            set_error_code(parser_data, 2);
            return false;
        }
    }
    else
    {
        set_error_code(parser_data, 2);
        return false;
    }

    return true;
}

bool return_value(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je EOL -> pravidlo 36
    if (parser_data->current_token->type == TOKEN_EOL)
    {
        // parser_data->unget_token = true; DELETE
        return true;
    }
    // EOF handling
    else if (parser_data->current_token->type == TOKEN_DEDENT)
    {
        parser_data->unget_token = true;
        return true;
    }
    // token je pravdepodobne expression -> pravidlo 37
    else
    {
        // musi nasledovat expression
        // get_token_and_set_error_code(parser_data); DELETE
        if (!process_expression(parser_data))
        {
            return false;
        }


        get_token_and_set_error_code(parser_data);

        // EOF handling
        if (parser_data->current_token->type == TOKEN_DEDENT)
        {
            parser_data->unget_token = true;
            return true;
        }
        // musi nasledovat "EOL"
        else if (parser_data->current_token->type != TOKEN_EOL)
        {
            return false;
        }
    }
    // else TODO / overit funkcnost, jestli expressions opravdu prekouse jakoukoliv blbost / DELETE
    // {
    //     set_error_code(parser_data, 2);
    //     return false;
    // }

    return true;
}

bool instruct(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je "(" -> pravidlo 20
    if (parser_data->current_token->type == TOKEN_LEFT_BRACKET)
    {
        // musi nasledovat neterminal TERM
        get_token_and_set_error_code(parser_data);
        if (!term(parser_data))
        {
            return false;
        }

        // musi nasledovat ")"
        if (!get_compare_check(parser_data, TOKEN_RIGHT_BRACKET))
        {
            return false;
        }
    }
    // token je "=" -> pravidlo 19
    else if (parser_data->current_token->type == TOKEN_ASSIGNMENT)
    {
        // musi nasledovat neterminal INSTRUCT_CONTINUE
        get_token_and_set_error_code(parser_data);
        if (!instruct_continue(parser_data))
        {
            return false;
        }
    }
    else
    {
        set_error_code(parser_data, 2);
        return false;
    }

    return true;
}

bool instruct_continue(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // TODO zde se bude rozhodovat zda vstoupit do analyzy vyrazu nebo do vetve pro identifikator, tohle rozhodnuti probehne na zaklade nahlednuti do tabulky symbolu

    // TODO symtable
    // token je identifikator
    if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        if (!copy_token(parser_data, parser_data->current_token, parser_data->backup_token)) // todo
        {
            return false;
        }

        get_token_and_set_error_code(parser_data);

        // token je "(" -> pravidlo 22
        if (parser_data->current_token->type == TOKEN_LEFT_BRACKET)
        {
            // // musi nasledovat "("
            // if (!get_compare_check(parser_data, TOKEN_LEFT_BRACKET))
            // {
            //     return false;
            // }

            // musi nasledovat neterminal TERM
            get_token_and_set_error_code(parser_data);
            if (!term(parser_data))
            {
                return false;
            }

            // musi nasledovat ")"
            if (!get_compare_check(parser_data, TOKEN_RIGHT_BRACKET))
            {
                return false;
            }
        }
        // token je expression -> pravidlo 21
        else
        {
            St_token* pom = parser_data->current_token;
            parser_data->current_token = parser_data->backup_token;
            parser_data->backup_token = pom;
            pom = NULL;
            parser_data->load_backup = true;

            // musi nasledovat expression
            // get_token_and_set_error_code(parser_data); DELETE
            if (!process_expression(parser_data))
            {
                return false;
            }          
        }
    }
    else
    {
        // musi nasledovat expression
        // get_token_and_set_error_code(parser_data); DELETE
        if (!process_expression(parser_data))
        {
            return false;
        }   
    }
    
    /*
    // token je identifikator -> pravidlo 22
    if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat "("
        if (!get_compare_check(parser_data, TOKEN_LEFT_BRACKET))
        {
            return false;
        }

        // musi nasledovat neterminal TERM
        get_token_and_set_error_code(parser_data);
        if (!term(parser_data))
        {
            return false;
        }

        // musi nasledovat ")"
        if (!get_compare_check(parser_data, TOKEN_RIGHT_BRACKET))
        {
            return false;
        }
    }
    // TODO expr
    // token je pravdepodobne expression -> pravidlo 21
    else
    {
        // musi nasledovat neterminal EXPRESSION
        // get_token_and_set_error_code(parser_data);
        if (!process_expression(parser_data))
        {
            return false;
        }
    }
    */
    // else // DELETE ???
    // {
    //     set_error_code(parser_data, 2);
    //     return false;
    // }
    
    return true;
}

bool term(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je klicove slovo
    if (parser_data->current_token->type == TOKEN_KEYWORD)
    {
        // token je "None" -> pravidlo 25
        if (strcmp(parser_data->current_token->attribute->str, "None") == 0)
        {
            // musi nasledovat neterminal TERM_N
            get_token_and_set_error_code(parser_data);
            if (!term_n(parser_data))
            {
                return false;
            }
        }
        else
        {
            set_error_code(parser_data, 2);
            return false;
        }
    }
    // token je identifikator -> pravidlo 23
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat neterminal TERM_N
        get_token_and_set_error_code(parser_data);
        if (!term_n(parser_data))
        {
            return false;
        }
    }
    // token je ")" -> pravidlo 26
    else if (parser_data->current_token->type == TOKEN_RIGHT_BRACKET)
    {
        parser_data->unget_token = true;
        return true;
    }
    // token je string, int nebo float -> pravidlo 24
    else if (parser_data->current_token->type == TOKEN_STRING_LITERAL || parser_data->current_token->type == TOKEN_INTEGER || parser_data->current_token->type == TOKEN_DOUBLE)
    {
        // musi nasledovat neterminal TYPE
        //get_token_and_set_error_code(parser_data); // DELETE TODO smazat tento radek ? nejlevejsi derivace
        if (!type(parser_data))
        {
            return false;
        }

        // musi nasledovat neterminal TERM_N
        get_token_and_set_error_code(parser_data);
        if (!term_n(parser_data))
        {
            return false;
        }
    }
    else
    {
        set_error_code(parser_data, 2);
        return false;
    }

    return true;
}

bool term_n(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je ")" -> pravidlo 28
    if (parser_data->current_token->type == TOKEN_RIGHT_BRACKET)
    {
        parser_data->unget_token = true;
        return true;
    }
    // token je "," -> pravidlo 27
    else if (parser_data->current_token->type == TOKEN_COMMA)
    {
        // musi nasledovat neterminal TERM_N_VALUE
        get_token_and_set_error_code(parser_data);
        if (!term_n_value(parser_data))
        {
            return false;
        }
    }
    else
    {
        set_error_code(parser_data, 2);
        return false;
    }

    return true;
}

bool term_n_value(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je klicove slovo
    if (parser_data->current_token->type == TOKEN_KEYWORD)
    {
        // token je "None" -> pravidlo 31
        if (strcmp(parser_data->current_token->attribute->str, "None") == 0)
        {
            // musi nasledovat neterminal TERM_N
            get_token_and_set_error_code(parser_data);
            if (!term_n(parser_data))
            {
                return false;
            }
        }
        else
        {
            set_error_code(parser_data, 2);
            return false;
        }
    }
    // token je identifikator -> pravidlo 29
    else if (parser_data->current_token->type == TOKEN_IDENTIFIER)
    {
        // musi nasledovat neterminal TERM_N
        get_token_and_set_error_code(parser_data);
        if (!term_n(parser_data))
        {
            return false;
        }
    }
    // token je string, int nebo float -> pravidlo 30
    else if (parser_data->current_token->type == TOKEN_STRING_LITERAL || parser_data->current_token->type == TOKEN_INTEGER || parser_data->current_token->type == TOKEN_DOUBLE)
    {
        // musi nasledovat neterminal TYPE
        //get_token_and_set_error_code(parser_data); // DELETE
        if (!type(parser_data))
        {
            return false;
        }

        // musi nasledovat neterminal TERM_N
        get_token_and_set_error_code(parser_data);
        if (!term_n(parser_data))
        {
            return false;
        }
    }
    else
    {
        set_error_code(parser_data, 2);
        return false;
    }

    return true;
}

bool type(tParser_data *parser_data)
{
    // zachyceni jiz nastale chyby
    if (parser_data->error_code)
    {
        return false;
    }

    // token je string, int nebo float -> pravidla 31, 32, 33
    else if (parser_data->current_token->type == TOKEN_STRING_LITERAL || parser_data->current_token->type == TOKEN_INTEGER || parser_data->current_token->type == TOKEN_DOUBLE)
    {
        return true;
    }
    else
    {
        set_error_code(parser_data, 2);
        return false;
    }    

    return true;
}