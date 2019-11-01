/*
 * @file scanner.c
 * @brief Implementace lexikálního analyzátoru
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date XX.XX.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */


#include "scanner.h"

//int error_handling()


void get_next_token(St_token *token)
{
    static int new_line = 1;

    char c = getchar();

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
    dynamic_string str;
    dynamic_string *string = &str;
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
                    State = IDENTIFIER;
                    string_add_char(string, c);
                }
                else if (c >= '1' && c <= '9') //celé číslo
                {
                    State = INTEGER;
                    string_add_char(string, c);
                }
                else if (c == '#') //komentář
                {
                    State = commentary;
                }
                else if (c == '\n') //EOL
                {

                    return;
                }
                else if (c == '!') //vykřičník
                {
                    State = exclamation_mark;
                    string_add_char(string, c);
                }
                else if (c == '<' || c == '>' || c == '=') //relační operátor
                {
                    State = RELATION_OPERATOR_1;
                    string_add_char(string, c);
                }
                else if (c == '/') //lomítko
                {
                    State = BINARY_OPERATOR_1;
                    string_add_char(string, c);
                }
                else if (c == '+' || c == '-' || c == '*') //binární operátor //??
                {
                    State = BINARY_OPERATOR_3;
                    string_add_char(string, c);
                }
                else if (c == '"') //uvozovka
                {
                    State = quotation_mark_1;
                }
                else if (c == '\'') //apostrof
                {
                    State = character;
                    string_add_char(string, c);
                }
                else if (c == ' ' || c == '\t')
                {
                    State = start;
                }
                else
                {
                    printf("error\n");
                    //TODO error
                }
                break;

            case IDENTIFIER:
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_')) //identifikátor
                {
                    State = IDENTIFIER;
                    string_add_char(string, c);
                }
                else
                {
                    //token->type = "KEYWORD";
                    //token->attribute.keyword = string;

                    //nebo
                    token->type = IDENTIFIER;
                    token->attribute.string = string;


                    ungetc(c,stdin);
                    return;
                }
                
        }
    }




    //TODO
    //Přidat dedenty a vynoření z tabulek symbolů
    //Návratový kod
}


