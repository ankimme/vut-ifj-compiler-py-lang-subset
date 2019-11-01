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


void st_init_context_table(St_context_table* context_table)
{
    St_table* main_sym_table;
    st_init_table(&main_sym_table);
    context_table->context_frames[0] = main_sym_table;
    context_table->context_count = 1;
    context_table->capacity = MAX_CONTEXT_FRAMES;
}

void st_create_context_frame(St_context_table* context_table)
{
    if (context_table->capacity > context_table->context_count)
    {
        St_table* new_sym_table;
        st_init_table(&new_sym_table);
        context_table->context_frames[context_table->context_count] = new_sym_table;
        context_table->context_count++;
    }
    else
    {
        error_handle(99);
        return;
    }
}

void pop_context_frame(St_context_table* context_table)
{
    if (context_table->context_count > 1)
    {
        context_table->context_count--;
        st_delete_table(&(context_table->context_frames[context_table->context_count]));
    }
    else
    {
        // pokus o smazani hlavni tabulky symbolu
        error_handle(99);
        return;
    }
}

void st_init_table(St_table** t_init)
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

void st_init_entry(St_entry** e_init)
{
    *e_init = malloc(sizeof(St_entry));
    if (e_init == NULL)
    {
        error_handle(99);
        return;
    }

    (*e_init)->key = 0;
    (*e_init)->value = NULL;
}

void st_delete_entry(St_entry** e_delete)
{
    free((*e_delete)->value); // TODO funkce na smazani obsahu value (polozky data uvnitr)
    free(*e_delete); 
}

void st_delete_table(St_table** t_delete)
{
    for (int i = 0; i < (*t_delete)->size; i++)
    {
        St_entry* entry = (*t_delete)->entries[i];
        if (entry != NULL)
        {
            st_delete_entry(&entry); // TODO zamyslet se jestli se opravdu predava dobry parametr
        }
    }

    free((*t_delete)->entries);
    free(*t_delete);
}

void st_insert_item_in_current_context(St_context_table* context_table, char* identificator, St_item* value)
{
    // nalezeni tabulky symbolu s aktualnim kontextem
    St_table* current_sym_table = context_table->context_frames[context_table->context_count - 1];

    // vytvoreni noveho zaznamu s pozadovanymi daty
    St_entry* ins_entry;
    st_init_entry(&ins_entry);
    ins_entry->key = st_generate_hash(identificator);
    ins_entry->value = value;
    
    St_item* existing_item = st_search_item(context_table, identificator); // hleda jestli v tabulce jiz existuje zaznam se stejnym hash klicem
    if (existing_item == NULL) // pokud nebyl nalezen prvek se stejnym klicem, pak pokracuje ve vkladani
    {
        if (current_sym_table->size != current_sym_table->item_count)
        {
            current_sym_table->entries[current_sym_table->item_count] = ins_entry;
            current_sym_table->item_count++;
        }
        else // v tabulce neni dostatek mista
        {
            // TODO realloc and then insert
        }
    }
    else // pokud byl nalezen prvek se stejnym klicem, zaznam se prepise
    {
        free(existing_item);
        existing_item = ins_entry->value;
    }
}

St_item* st_search_item(St_context_table* context_table, char* identificator)
{
    unsigned long hash = st_generate_hash(identificator);
    St_table* table;
    for (int i = context_table->context_count - 1; i >= 0; i--)
    {
        table = context_table->context_frames[i];
        for (int j = 0; j < table->item_count; j++)
        {
            if (table->entries[j]->key == hash)
            {
                return table->entries[j]->value;
            }
        }
    }
    return NULL;
}

unsigned long st_generate_hash(char *s)
{
    unsigned long hash = 1, pow;
    const int len_s = strlen(s);
    int loop_stop = (len_s * len_s) % 500;

    for (int i = 0; i < loop_stop; i++)
    {
        pow = 2;
        for (int j = 0; j < len_s; j++)
        {
            pow *= s[j] * (i + 1);
        }
        hash += pow;
    }
    return hash;
}



void error_handle(int error_number)
{
    /* TODO error handling */
    // st_delete_table(&table);
    if (error_number) // DELETE
    {

    }
    return;
}