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
    printf("\033[0;36m");

    tSymtable symtable;
    st_init(&symtable);
    tNodeData *data;
    tHash_Table_Item *item;


    data = malloc(sizeof(tNodeData));
    data->a = 1;
    data->lptr = NULL;
    data->rptr = NULL;

    st_insert_entry_in_current_context_random_key(&symtable, data, HT_TYPE_NODE);

    data = malloc(sizeof(tNodeData));
    data->a = 2;
    data->lptr = NULL;
    data->rptr = NULL;

    dynamic_string *str1 = malloc(sizeof(dynamic_string));
    string_init(str1);
    string_append(str1, "shell");

    st_insert_entry_in_current_context(&symtable, str1, data, HT_TYPE_NODE);


    st_indent(&symtable);

    data = malloc(sizeof(tNodeData));
    data->a = 3;
    data->lptr = NULL;
    data->rptr = NULL;

    dynamic_string *str2 = malloc(sizeof(dynamic_string));
    string_init(str2);
    string_append(str2, "sun");

    st_insert_entry_in_current_context(&symtable, str2, data, HT_TYPE_NODE);


    item = st_search_entry(&symtable, str1);
    printf("%i\n", item->node_data->a);

    item = st_search_entry(&symtable, str2);
    printf("%i\n", item->node_data->a);

    st_dedent(&symtable);

    item = st_search_entry(&symtable, str1);
    if (item->node_data)
        printf("%i\n", item->node_data->a);
    else
        printf("NULL\n");

    dynamic_string *str3 = malloc(sizeof(dynamic_string));
    string_init(str3);
    string_append(str3, "sun");

    item = st_search_entry(&symtable, str3);
    if (item)
        printf("%i\n", item->node_data->a);
    else
        printf("NULL\n");
    

    string_free(str3);
    free(str3);
    str3 = NULL;

    st_clean_all(&symtable);

    printf("\033[0m"); 

    return 0;
}