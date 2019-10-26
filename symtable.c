/*
 * @file symtable.c
 * @brief Implementace tabulky symbolů pomocí hashovací tabulky
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 26.10.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 * Implementace inspirována z následujícího zdroje:
 * https://github.com/jamesroutley/write-a-hash-table/tree/master/02-hash-table
 */

#include "symtable.h"

static void st_init_table(St_table** t_init)
{
    *t_init = malloc(sizeof(St_table));

    if (t_init == NULL)
    {
        error_handle(99);
        return;
    }

    (*t_init)->size = INCREMENT_SIZE;
    (*t_init)->item_count = 0;
    (*t_init)->entries = malloc(sizeof(St_entry) * INCREMENT_SIZE);

    if ((*t_init)->entries == NULL)
    {
        free(*t_init);
        error_handle(99);
        return;
    }
} 

static void st_init_entry(St_entry** e_init)
{
    *e_init = malloc(sizeof(St_entry));
    (*e_init)->key = NULL;
    (*e_init)->value = NULL;

    if (e_init == NULL)
    {

        error_handle(99);
        return;
    }
}

static void st_delete_entry(St_entry** e_delete)
{
    free((*e_delete)->key);
    free((*e_delete)->value); // TODO funkce na smazani obsahu value
    free(*e_delete); 
}

static void st_delete_table(St_table** t_delete)
{
    for (int i = 0; i < (*t_delete)->size; i++)
    {
        St_entry* entry = (*t_delete)->entries[i];
        if (entry != NULL)
        {
            st_delete_entry(entry);
        }
    }

    free((*t_delete)->entries);
    free(*t_delete);
}

void st_insert_item(St_table* table, char* identificator, St_item* value)
{
    St_entry* ins_entry;
    st_init_entry(&ins_entry);
    ins_entry->key = st_generate_hash(identificator);
    ins_entry->value = value;
    
    int index = st_search_item(table, identificator); // hleda jestli v tabulce jiz existuje zaznam se stejnym hash klicem
    if (index == -1) // pokud nebyl nalezen prvek se stejnym klicem, pak pokracuje ve vkladani
    {
        if (table->size != table->item_count)
        {
            table->entries[table->item_count] = ins_entry;
            table->item_count++;
        }
        else // v tabulce neni dostatek mista
        {
            // TODO realloc and then insert
        }
    }
    else // pokud byl nalezen prvek se stejnym klicem, zaznam se prepise
    {
        table->entries[index] = ins_entry;
    }
}

static int st_search_item(St_table* table, char* identificator)
{
    unsigned long hash = st_generate_hash(identificator);
    for (int i = 0; i < table->size; i++)
    {
        if (table->entries[i]->key == hash)
        {
            return i;
        }
    }
    return -1;
}

static unsigned long st_generate_hash(char *s)
{
    unsigned long hash;

    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++)
    {
        hash += (long)pow(2, len_s - (i + 1)) * s[i];
    }
    return hash;
}
