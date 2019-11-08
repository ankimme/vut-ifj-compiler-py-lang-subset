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


int is_keyword(dynamic_string *str)
{
    if ((!strings_cmp(str, "None")) || (!strings_cmp(str, "def")) || (!strings_cmp(str, "else")) || (!strings_cmp(str, "if")) || (!strings_cmp(str, "pass")) || (!strings_cmp(str, "return")) || (!strings_cmp(str, "while")))
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


void get_next_token(St_token *token)
{
    static int new_line = 1; //začátek řádku

    char c = getchar();

    //pokud se znak nachází na začátku řádku, možnost INDENTU
    if (new_line == 1)
    {
        int counter = 0;

        while (c == ' ')
        {
            counter++;

            c = getchar();
        }
        if (counter > 0)
        {
            printf("INDENT POZDĚJI\n");
            //TODO generovat indent/dedent
            //zanoření/vynoření V PARSERU ASI
        }
        new_line = 0;
    }

    //vytvoření stringu pro aktuální stav automatu
    
    dynamic_string *string = NULL;
    if (!string_init(string))
    {
        token->error_value = clean_all(INTERNAL_ERROR, string);
        return;
    }


    /**************************/
    /*        AUTOMAT         */
    /**************************/
    
    State state = start;

    while(c != EOF)
    {
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
                    if (!string_add_char(string, c)) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    state = INTEGER;
                }
                else if (c == '#') //komentář
                {
                    state = commentary;
                }
                else if (c == '\n') //EOL
                {
                    if (!strings_cat(token->type, "EOL")) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string); //předána hodnota o chybě v tokenu
                        return;
                    }
                    token->error_value = clean_all(NO_ERROR, string);
                    new_line = 1;
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
                    if (!strings_cat(token->type, "PLUS"))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '-') //binární operátor -
                {
                    if (!strings_cat(token->type, "MINUS"))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '*') //binární operátor *
                {
                    if (!strings_cat(token->type, "MULTIPLY"))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else if (c == '"') //uvozovka
                {
                    state = quotation_mark_1;
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
                        if (!strings_cat(token->type, "KEYWORD")) //klíčové slovo
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
                    }
                    else
                    {
                        if (!strings_cat(token->type, "IDENTIFIER")) //identifikátor
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
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

                    if (!strings_cat(token->type, "INTEGER")) //celé číslo
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    convert_to_integer(string, token); //převede celé nezáporné číslo ze stringu do integeru, v případě přetečení/podtečení nastaví INTERNAL_ERROR    
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

                    if (!strings_cat(token->type, "DOUBLE")) //desetinný literál
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
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

                    if (!strings_cat(token->type, "DOUBLE")) //desetinný literál
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
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
                    if (!strings_cat(token->type, "EOL")) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string); //předána hodnota o chybě v tokenu
                        return;
                    }
                    token->error_value = clean_all(NO_ERROR, string);
                    new_line = 1;
                    return; 
                }

                break;

            case exclamation_mark:
                if (c == '=')
                {
                    if (!strings_cat(token->type, "NOT_EQUAL"))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
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
                    if (!strings_cmp(string, "<"))
                    {
                        if (!strings_cat(token->type, "LESS_OR_EQUAL"))
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
                    }
                    else if (!strings_cmp(string, ">"))
                    {
                        if (!strings_cat(token->type, "GREATER_OR_EQUAL"))
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
                    }
                    else // jedná se o rovnítko
                    {
                        if (!strings_cat(token->type, "EQUAL"))
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
                    }
                    
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else
                {
                    ungetc(c,stdin);

                    if (!strings_cmp(string, "<"))
                    {
                        if (!strings_cat(token->type, "LESS_THAN"))
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
                    }
                    else if (!strings_cmp(string, ">"))
                    {
                        if (!strings_cat(token->type, "GREATER_THAN"))
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
                    }
                    else // jedná se o rovnítko
                    {
                        if (!strings_cat(token->type, "ASSIGNMENT"))
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }
                    }

                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                
                break;

            case BINARY_OPERATOR:
                if (c == '/') //BINARY_OPERATION_2 (//)
                {
                    if (!strings_cat(token->type, "DIVIDE_INTEGER"))
                        {
                            token->error_value = clean_all(INTERNAL_ERROR, string);
                            return;
                        }

                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }
                else //BINARY_OPERATION (/)
                {
                    ungetc(c,stdin);

                    if (!strings_cat(token->type, "DIVIDE_FLOAT"))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }

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
                    state = start;
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

            case character:
                if (c == '\\')
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
                    if (!strings_cat(token->type, "STRING_LITERAL")) //token typu ŘETĚZCOVÝ LITERÁL
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
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