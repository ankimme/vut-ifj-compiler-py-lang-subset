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

int new_line = 1;

void Get_next_token(St_token *Token)
{
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
            //generovat indent/dedent
            //zanoření/vynoření
        }
    }

    //vytvoření stringu pro aktuální stav automatu
    Dynamic_string *string;
    if (!string_init(string))
    {
        printf("CHYBIČKA STRING INIT\n");
        //vnitřní chyba alokace, error 99
    }
    State = start;

    while(c != EOF)
    {
        switch(State)
        {
            case start:
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')) //identifikátor
                {
                    State = IDENTIFIER;
                    strings_cat(string, &c);
                }
                else if (c >= '1' && c <= '9') //celé číslo
                {
                    State = INTEGER;
                }
                else if (c == '#') //komentář
                {
                    State = commentary;
                }
                else if (c == '!') //vykřičník
                {
                    State = exclamation_mark;
                }
                else if (c == '<' || c == '>' || c == '=') //relační operátor
                {
                    State = RELATION_OPERATOR_1;
                }
                else if (c == '/') //lomítko
                {
                    State = BINARY_OPERATOR_1;
                }
                else if (c == '+' || c == '-' || c == '*') //binární operátor
                {
                    State = BINARY_OPERATOR_3;
                }
                else if (c == '"') //uvozovka
                {
                    State = quotation_mark_1;
                }
                else if (c == '\'') //apostrof
                {
                    State = character;
                }
                else if (c == ' ' || c == '\t')
                {
                    State = start;
                }
                else
                {
                    printf("error\n");
                    //error
                }
            //case ...
        }
    }




    //TODO
    //Přidat dedenty a vynoření z tabulek symbolů
    //Návratový kod
}


