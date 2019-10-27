/*
 * @file scanner.c
 * @brief Implementace lexikálního analyzátoru
 * @author Jan Klhůfek (xklhuf01@stud.fit.vutbr.cz)
 * @date XX.XX.2019
 *
 * Projekt: Implementace prekladace imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

//#include "error.h"
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
            //generovat indent/dedent
            //zanoření/vynoření
        }
    }

    //vytvoření stringu pro aktuální stav automatu
    Dynamic_string *string;
    if (!string_init(string))
    {
        //vnitřní chyba alokace, error 99
    }
    state = start;

    while(c != EOF)
    {
        switch(state)
        {
            case start:
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')) //identifikátor
                {
                    state = INDENTIFIER;
                }
                else if (c >= '1' && c <= '9') //celé číslo
                {
                    state = INTEGER;
                }
                else if (c == '#') //komentář
                {
                    state = commentary;
                }
                else if (c == '!') //vykřičník
                {
                    state = exlamation_mark;
                }
                else if (c == '<' || c == '>' || c == '=') //relační operátor
                {
                    state = RELATION_OPERATION_1;
                }
                else if (c == '/') //lomítko
                {
                    state = BINARY_OPERATION_1;
                }
                else if (c == '+' || c == '-' || c == '*') //binární operátor
                {
                    state = BINARY_OPERATION_3;
                }
                else if (c == '"') //uvozovka
                {
                    state = quotation_mark_1;
                }
                else if (c == '\'') //apostrof
                {
                    state = character;
                }
                else if (c == ' ' || c == '\t')
                {
                    state = start;
                }

        }





    }




    //TODO
    //Přidat dedenty a vynoření z tabulek symbolů
    //Návratový kod
}


