/*
 * @file scanner.c
 * @brief Implementace lexikálního analyzátoru
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 7.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "scanner.h"

int clean_all(Errors err, dynamic_string *str)
{
    string_free(str);

    return err;
}


int is_keyword(dynamic_string *string)
{
    if ((!strcmp(string->str, "None")) || (!strcmp(string->str, "def")) || (!strcmp(string->str, "else")) || (!strcmp(string->str, "if")) || (!strcmp(string->str, "pass")) || (!strcmp(string->str, "return")) || (!strcmp(string->str, "while")))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void convert_to_integer(dynamic_string *str, St_token *token)
{
    errno = 0;
    char *end_pointer;

    long integer_num = strtol(str->str, &end_pointer, 10); //konverze číselné hodnoty z řetězce do proměnné typu double

    if ((integer_num > INT_MAX) || (*end_pointer != '\0'))
    {
        errno = ERANGE;
        token->error_value = clean_all(INTERNAL_ERROR, str);
        return;
    }

    token->attribute.number = integer_num;
    token->error_value = clean_all(NO_ERROR, str);
    return;
}


void convert_to_double(dynamic_string *str, St_token *token)
{
    errno = 0;
    char *end_pointer;

    double decimal_num = strtod(str->str, &end_pointer); //konverze číselné hodnoty z řetězce do proměnné typu double
    
    if (errno == ERANGE)
    {
        token->error_value = clean_all(INTERNAL_ERROR, str);
        return;
    }

    token->attribute.decimal = decimal_num;
    token->error_value = clean_all(NO_ERROR, str);
    return;
}


void get_next_token(St_token *token, tStack* stack)
{
    static int new_line = 1; //nacházíme se na začátku řádku

    char c = getchar();

    //vytvoření stringu pro aktuální stav automatu
    
    dynamic_string *string = NULL;
    if (!string_init(string))
    {
        token->error_value = clean_all(INTERNAL_ERROR, string);
        return;
    }


    //pokud se znak nacházíme na začátku řádku, možnost INDENTU, v případě komentáře se negeneruje
    if (new_line == 1)
    {
        new_line = 0; //další token nebude první na řádku
        int counter = 0;

        if (c == '\"') //dokumentační řetězec je na začátku řádku, v pořádku
        {
            new_line = 1;
        }
            
        while (c == ' ') //počet mezer odsazení
        {
            counter++;

            c = getchar();
        }
    
        if ((counter > 0) && (c == '\"')) //nemůže přijít dokumentační řetězec, chyba
        {
            token->error_value = clean_all(LEXICAL_ERROR, string);
            return;
        }

        if (counter >= 0) //generuj token INDENT či DEDENT
        {
            int indentation;
            tStack_top (stack, &indentation);

            if (c == '#') //komentář, indent se negeneruje
            {
                ungetc(c,stdin);
            }
            else if (c == '\n') //EOL, indent se negeneruje
            {
                ungetc(c,stdin);   
            }
            else if ((c == EOF) && (indentation != 0)) //dogenerování dedentů
            {
                new_line = 1;

                ungetc(c,stdin);
        
                tStack_pop(stack);
                tStack_top (stack, &indentation);
                
                token->type = TOKEN_DEDENT;
                token->error_value = clean_all(NO_ERROR, string);

                return;
            }
            else if (counter < indentation) //DEDENT
            {
                new_line = 1;

                ungetc(c,stdin);
        
                tStack_pop(stack);
                tStack_top (stack, &indentation);

                if (counter <= indentation) //generuj dedent
                {
                    token->type = TOKEN_DEDENT;
                    token->error_value = clean_all(NO_ERROR, string);

                    return;
                }
                else if (counter > indentation) //chyba
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }
            }
            else if (counter > indentation) //INDENT
            {
                tStack_push(stack, counter);

                token->type = TOKEN_INDENT;
                token->error_value = clean_all(NO_ERROR, string);

                return;
            }
        }
        else
        {
            ungetc(c,stdin);
        }
    }

    


    /**************************/
    /*        AUTOMAT         */
    /**************************/
    
    State state = start;

    while(1)
    {
        c = getchar();
        switch(state)
        {
            case start:
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')) //identifikátor
                {
                    if (!string_add_char(string, c)) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string); //předána hodnota o chybě v tokenu
                        return;
                    }
                    state = IDENTIFIER;
                }
                else if (c >= '1' && c <= '9') //celé číslo
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = INTEGER;
                }
                else if (c == '0') //nula
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = ZERO;
                }
                else if (c == '#') //komentář
                {
                    state = commentary;
                }
                else if (c == '\n') //EOL
                {
                    new_line = 1;
                    token->type = TOKEN_EOL;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '!') //vykřičník
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = exclamation_mark;
                }
                else if (c == '<' || c == '>' || c == '=') //relační operátor
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = RELATION_OPERATOR;
                }
                else if (c == ':') //dvojtečka
                {
                    token->type = TOKEN_COLON;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == ',') //čárka
                {
                    token->type = TOKEN_COMMA;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '/') //lomítko
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = BINARY_OPERATOR;
                }
                else if (c == '+') //binární operátor +
                {
                    token->type = TOKEN_PLUS;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '-') //binární operátor -
                {
                    token->type = TOKEN_MINUS;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '*') //binární operátor *
                {
                    token->type = TOKEN_MULTIPLY;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '"') //uvozovka
                {
                    if (new_line == 1)
                    {
                        state = quotation_mark_1;    
                    }
                    else
                    {
                        token->error_value = clean_all(LEXICAL_ERROR, string);
                        return;
                    }
                }
                else if (c == EOF) //EOF
                {
                    token->type = TOKEN_EOF;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '(') //levá závorka
                {
                    token->type = TOKEN_LEFT_BRACKET;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == ')') //pravá závorka
                {
                    token->type = TOKEN_RIGHT_BRACKET;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '\'') //apostrof
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = character;
                }
                else if (c == ' ' || c == '\t')
                {
                    state = start;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case IDENTIFIER:
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_')) //identifikátor
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = IDENTIFIER;
                }
                else
                {
                    ungetc(c,stdin); //navrácení následného znaku na stdin

                    if(is_keyword(string))
                    {
                        token->type = TOKEN_KEYWORD; //klíčové slovo
                    }
                    else
                    {
                        token->type = TOKEN_IDENTIFIER; //identifikátor
                    }
                    token->attribute.string = string;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }

                break;

            case INTEGER:
                if (c >= '0' && c <= '9') //celé číslo
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = INTEGER;
                }
                else if(c == '.') //desetinná tečka
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = decimal_point;
                }
                else if(c == 'e' || c == 'E') //exponent
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = exponent;
                }
                else
                {
                    ungetc(c,stdin);

                    token->type = TOKEN_INTEGER; //celé číslo
                    
                    convert_to_integer(string, token); //převede celé nezáporné číslo ze stringu do integeru, v případě přetečení/podtečení nastaví INTERNAL_ERROR    
                    return;
                }

                break;

            case ZERO:
                if (c == '0') //více nul
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = ZERO;
                }
                else if (c == '.') //desetinná tečka
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = decimal_point;
                }
                else if (c >= '1' && c <= '9') //číslo
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = excess;
                }
                else //jedna nula
                {
                    ungetc(c,stdin);

                    token->type = TOKEN_INTEGER; //integer
                    convert_to_integer(string, token); //převede celé nezáporné číslo ze stringu do integeru, v případě přetečení/podtečení nastaví INTERNAL_ERROR    
                    return;
                }

                break;

            case excess:
                if (c == '.') //desetinná tečka
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = decimal_point;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case decimal_point:
                if (c >= '0' && c <= '9') //desetinná část
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = DOUBLE_1;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case DOUBLE_1:
                if (c >= '0' && c <= '9') //desetinná část
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = DOUBLE_1;
                }
                else if (c == 'e' || c == 'E') //exponent
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = exponent;
                }
                else
                {
                    ungetc(c,stdin);

                    token->type = TOKEN_DOUBLE; //desetinný literál
                    
                    convert_to_double(string, token); //převede desetinný literál ze stringu do doublu, v případě přetečení/podtečení nastaví INTERNAL_ERROR
                    return;
                }

                break;

            case exponent:
                if (c >= '0' && c <= '9')
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = DOUBLE_2;
                }
                else if (c == '+' || c == '-')
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = plus_minus;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case plus_minus:
                if (c >= '0' && c <= '9')
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = DOUBLE_2;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case DOUBLE_2:
                if (c >= '0' && c <= '9')
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = DOUBLE_2;
                }
                else
                {
                    ungetc(c,stdin);

                    token->type = TOKEN_DOUBLE; //desetinný literál
                    
                    convert_to_double(string, token); //převede desetinný literál ze stringu do doublu, v případě přetečení/podtečení nastaví INTERNAL_ERROR
                    return;
                }

                break;

            case commentary:
                if (c != '\n')
                {
                    state = commentary;
                }
                else
                {
                    new_line = 1;
                    token->type = TOKEN_EOL;  //EOL         
                    token->error_value = clean_all(NO_ERROR, string);
                    return; 
                }

                break;

            case exclamation_mark:
                if (c == '=')
                {
                    token->type = TOKEN_NOT_EQUAL;  //NEROVNÁ SE
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case RELATION_OPERATOR:
                if (c == '=')
                {
                    if (!strcmp(string->str, "<"))
                    {
                        token->type = TOKEN_LESS_OR_EQUAL; //<=
                    }
                    else if (!strcmp(string->str, ">"))
                    {
                        token->type = TOKEN_GREATER_OR_EQUAL;  //>=
                    }
                    else // jedná se o rovnítko
                    {
                        token->type = TOKEN_EQUAL;  //==
                    }
                    
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else
                {
                    ungetc(c,stdin);

                    if (!strcmp(string->str, "<"))
                    {
                        token->type = TOKEN_LESS_THAN;  //<
                    }
                    else if (!strcmp(string->str, ">"))
                    {
                        token->type = TOKEN_GREATER_THAN;  //>
                    }
                    else // jedná se o rovnítko
                    {
                        token->type = TOKEN_ASSIGNMENT;  //=
                    }

                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                
                break;

            case BINARY_OPERATOR:
                if (c == '/') //BINARY_OPERATION_2 (//)
                {
                    token->type = TOKEN_DIVIDE_INTEGER;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else //BINARY_OPERATION (/)
                {
                    ungetc(c,stdin);

                    token->type = TOKEN_DIVIDE_FLOAT; 
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }

                break;

            case quotation_mark_1:
                if (c == '"')
                {
                    state = quotation_mark_2;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case quotation_mark_2:
                if (c == '"')
                {
                    state = quotation_mark_3;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case quotation_mark_3:
                if (c == '"')
                {
                    state = ending_quotation_1;
                }
                else if (c == '\\')
                {
                    state = backslash;
                }
                else
                {
                    state = quotation_mark_3;
                }

                break;
                
            case ending_quotation_1:
                if (c == '"')
                {
                    state = ending_quotation_2;
                }
                else if (c == '\\')
                {
                    state = backslash;
                }
                else
                {
                    state = quotation_mark_3;
                }

                break;

            case ending_quotation_2:
                if (c == '"')
                {
                    state = doc_string;
                }
                else if (c == '\\')
                {
                    state = backslash;
                }
                else
                {
                    state = quotation_mark_3;
                }

                break;

            case backslash:
                if (c)
                {
                    state = quotation_mark_3;
                }

                break;

            case doc_string:
                if (c == '\n')
                {
                    new_line = 1;
                    token->type = TOKEN_EOL;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == ' ' || c == '\t')
                {
                    state = doc_string;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case character:
                if (c == '\\') //zpracování řetězcového literálu
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = escape;
                }
                else if (c == '\'')
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    token->type = TOKEN_STRING_LITERAL; //token typu ŘETĚZCOVÝ LITERÁL
                    token->attribute.string = string;
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = character;
                }

                break;

            case escape:
                if (c == 'x' || c == 'X')
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = hexadecimal_1;
                }
                else
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = character;
                }

                break;

            case hexadecimal_1:
                if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = hexadecimal_2;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;

            case hexadecimal_2:
                if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = character;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
                    return;
                }

                break;
        }
    }

    //TODO
    //Přidat tokeny dedentů
}