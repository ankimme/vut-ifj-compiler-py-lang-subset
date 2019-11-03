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

    for (int i = 1; i < context_table->capacity; i++) // nastaveni pointeru na NULL
    {
        context_table->context_frames[i] = NULL;
    }
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
        // prekrocen maximalni pocet kontextu
        error_handle(99);
        return;
    }
}

void st_pop_context_frame(St_context_table* context_table)
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
        *t_init = NULL;
        error_handle(99);
        return;
    }

    for(int i = 0; i < (*t_init)->size; i++) // nastaveni pointeru na NULL jako defaultni hodnota
    {
        (*t_init)->entries[i] = NULL;
    }

} 

void st_delete_table(St_table** t_delete)
{
    for (int i = 0; i < (*t_delete)->size; i++)
    {
        St_entry* entry = (*t_delete)->entries[i];
        if (entry != NULL)
        {
            st_delete_entry(&entry);
        }
    }

    free((*t_delete)->entries);
    (*t_delete)->entries = NULL;
    free(*t_delete);
    (*t_delete) = NULL;
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
    free((*e_delete)->value);
    (*e_delete)->value = NULL;
    free(*e_delete); 
    *e_delete = NULL;
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
    
    St_item* existing_item = st_search_item_in_current_context(context_table, identificator); // hleda jestli v tabulce jiz existuje zaznam se stejnym hash klicem
    if (existing_item == NULL) // pokud nebyl nalezen prvek se stejnym klicem, pak pokracuje ve vkladani
    {
        if (current_sym_table->size == current_sym_table->item_count) // v pripade ze v tabulce neni dostatek mista, pokusi se alokovat vice
        {
            current_sym_table->entries = realloc(current_sym_table->entries, sizeof(St_entry) * current_sym_table->size + sizeof(St_entry) * INCREMENT_SIZE); // prideleni vetsi pameti poli entries
            if (current_sym_table->entries == NULL)
            {
                error_handle(99);
                return;
            }

            for(int i = current_sym_table->size; i < current_sym_table->size + INCREMENT_SIZE; i++) // nastaveni pointeru na NULL jako defaultni hodnota
            {
                current_sym_table->entries[i] = NULL;
            }
        
            current_sym_table->size += INCREMENT_SIZE;
        }
        current_sym_table->entries[current_sym_table->item_count] = ins_entry;
        current_sym_table->item_count++;
    }
    else // pokud byl nalezen prvek se stejnym klicem, zaznam se prepise
    {
        free(existing_item);
        existing_item = ins_entry->value;
    }
}

St_item* st_search_item_in_all_contexts(St_context_table* context_table, char* identificator)
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

St_item* st_search_item_in_current_context(St_context_table* context_table, char* identificator)
{
    unsigned long hash = st_generate_hash(identificator);
    St_table* table = context_table->context_frames[context_table->context_count - 1];
    for (int i = 0; i < table->item_count; i++)
    {
        if (table->entries[i]->key == hash)
        {
            return table->entries[i]->value;
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

void st_free_all(St_context_table* context_table)
{
    St_table* table;
    for (int i = context_table->context_count - 1; i >= 0; i--)
    {
        table = context_table->context_frames[i];
        for (int j = 0; j < table->item_count; j++)
        {
            free(table->entries[j]->value);
            table->entries[j]->value = NULL;
            free(table->entries[j]);
            table->entries[j] = NULL;
        }
        free(table->entries);
        table->entries = NULL;
        free(table);
        table = NULL;
    }
    context_table->context_count = 0;
}

void error_handle(int error_number)
{
    // TODO
    // st_free_all(&context_table);
    if (error_number) {};
    return;
}