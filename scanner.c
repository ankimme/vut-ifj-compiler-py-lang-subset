/*
 * @file scanner.c
 * @brief Implementace lexikálního analyzátoru
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date 1.11.2019
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


void get_next_token(St_token *token)
{
    static int new_line = 1; //začátek řádku

    char c = getchar();


    //pokud se znak nachází na začátku řádku, možnost INDENTU
    if (new_line == 1)
    {
        int counter = 0;

        while ((new_line == 1) && (c == ' '))
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
    }

    //vytvoření stringu pro aktuální stav automatu
    //dynamic_string str;
    dynamic_string *string = NULL;
    if (!string_init(string))
    {
        printf("CHYBIČKA STRING INIT\n");
        //TODO vnitřní chyba alokace, error 99
    }


    /**************************/
    /*        AUTOMAT         */
    /**************************/
    
    State = start;

    while(c != EOF)
    {
        switch(State)
        {
            case start:
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')) //identifikátor
                {
                    if (!string_add_char(string, c)) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string); //předána hodnota o chybě v tokenu
                        return;
                    }
                    State = IDENTIFIER;
                }
                else if (c >= '1' && c <= '9') //celé číslo
                {
                    if (!string_add_char(string, c)) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = INTEGER;
                }
                else if (c == '#') //komentář
                {
                    State = commentary;
                }
                else if (c == '\n') //EOL
                {
                    if (!strings_cat(token->type, "EOL")) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string); //předána hodnota o chybě v tokenu
                        return;
                    }
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
                    State = exclamation_mark;
                }
                else if (c == '<' || c == '>' || c == '=') //relační operátor
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = RELATION_OPERATOR_1;
                }
                else if (c == '/') //lomítko
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = BINARY_OPERATOR_1;
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
                    State = quotation_mark_1;
                }
                else if (c == '\'') //apostrof
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = character;
                }
                else if (c == ' ' || c == '\t')
                {
                    State = start;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
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
                    State = IDENTIFIER;
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
                    State = INTEGER;
                }
                else if(c == '.') //desetinná tečka
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = decimal_point;
                }
                else if(c == 'e' || c == 'E') //exponent
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = exponent;
                }
                else
                {
                    ungetc(c,stdin);

                    if (!strings_cat(token->type, "INTEGER")) //identifikátor
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    token->attribute.number = string; //TODO překonvertovat na číslo, asi funkce
                    token->error_value = clean_all(NO_ERROR, string);
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
                    State = DOUBLE_1;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
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
                    State = DOUBLE_1;
                }
                else if (c == 'e' || c == 'E') //exponent
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = exponent;
                }
                else
                {
                    ungetc(c,stdin);

                    if (!strings_cat(token->type, "DOUBLE")) //desetinný literál
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    token->attribute.decimal = string; //TODO překonvertovat na číslo, asi funkce
                    token->error_value = clean_all(NO_ERROR, string);
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
                    State = DOUBLE_2;
                }
                else if (c == '+' || c == '-')
                {
                    if (!string_add_char(string, c))
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    State = plus_minus;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
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
                    State = DOUBLE_2;
                }
                else
                {
                    token->error_value = clean_all(LEXICAL_ERROR, string);
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
                    State = DOUBLE_2;
                }
                else
                {
                    ungetc(c,stdin);

                    if (!strings_cat(token->type, "DOUBLE")) //desetinný literál
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string);
                        return;
                    }
                    token->attribute.decimal = string; //TODO překonvertovat na číslo, asi funkce
                    token->error_value = clean_all(NO_ERROR, string);
                    return;
                }

                break;

            case commentary:
                if (c != '\n')
                {
                    State = commentary;
                }
                else
                {
                    if (!strings_cat(token->type, "EOL")) //v případě chyby v alokaci
                    {
                        token->error_value = clean_all(INTERNAL_ERROR, string); //předána hodnota o chybě v tokenu
                        return;
                    }
                    token->error_value = clean_all(NO_ERROR, string);
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
                }

                break;

            case RELATION_OPERATOR_1:
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

            case BINARY_OPERATOR_1:
            
                //TODO
        }
    }

    //TODO
    //Přidat dedenty a vynoření z tabulek symbolů
    //Návratový kod
}