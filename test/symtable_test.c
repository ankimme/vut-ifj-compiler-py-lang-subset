/*
 * @file symtable_test.c
 * @brief Test tabulky symbolu
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 27.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 */

#include "../symtable.h"

int main()
{
    tSymtable symtable;

    st_init(&symtable);

    tData data;

    data.number = 1;
    st_insert_entry_in_current_context(&symtable, "adda", data);
    data.number = 2;
    st_insert_entry_in_current_context(&symtable, "daad", data);
    data.number = 3;
    st_insert_entry_in_current_context(&symtable, "ide3", data);
    data.number = 4;
    st_insert_entry_in_current_context(&symtable, "ide4", data);
    data.number = 5;
    st_insert_entry_in_current_context(&symtable, "ide5", data);
    data.number = 6;
    st_insert_entry_in_current_context(&symtable, "ide6", data);
    data.number = 7;
    st_insert_entry_in_current_context(&symtable, "ide7", data);


    tData* new_data;
    /*
    new_data = st_search_entry(&symtable, "ide1");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide2");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide3");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide4");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide5");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "adda");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");
    
    new_data = st_search_entry(&symtable, "daad");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");
    */

    st_indent(&symtable);

    data.number = 8;
    st_insert_entry_in_current_context(&symtable, "ide8", data);

    st_indent(&symtable);

    data.number = 9;
    st_insert_entry_in_current_context(&symtable, "ide9", data);

    new_data = st_search_entry(&symtable, "ide7");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide8");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide9");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    st_dedent(&symtable);

    new_data = st_search_entry(&symtable, "ide7");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide8");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide9");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    st_dedent(&symtable);

    new_data = st_search_entry(&symtable, "ide7");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide8");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    new_data = st_search_entry(&symtable, "ide9");
    if (new_data)
        printf("%i\n", new_data->number);
    else
        printf("NULL\n");

    st_clean_all(&symtable);

    return 0;
}