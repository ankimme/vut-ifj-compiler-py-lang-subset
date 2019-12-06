/*
 * @file symtable.c
 * @brief Implementace tabulky symbolů pomocí hashovací tabulky
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @author Martin Šerý (xserym01@stud.fit.vutbr.cz)
 * @date 18.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 */

#include "symtable.h"

/* FUNKCE PRO TABULKU SYMBOLU */

void st_init (tSymtable *symtable)
{
    if (symtable == NULL)
    {
        return;
    }

    // nastavi ukazatele na hash tabulky na NULL
    for (int i = 0; i < HT_STACK_SIZE; i++)
    {
        symtable->HT_array[i] = NULL;
    }
    // vlozi tabulku hlavniho kontextu do symtablu
    tHash_Table *new_hash_table = malloc(sizeof(tHash_Table));
    if (new_hash_table == NULL)
    {
        // TODO RAISE MALLOC ERROR
        return;
    }
    ht_init(new_hash_table);
    symtable->HT_array[0] = new_hash_table;
    // printf("%lu", sizeof(tHash_Table)); // DELETE
    symtable->top = 0;
}

void st_indent (tSymtable *symtable)
{
    if (symtable->top >= HT_STACK_SIZE - 1)
    {
        // TODO RAISE TOO MANY INDENTS ERROR
        return;
    }

    // vlozi novou hash tabulku do symtablu
    tHash_Table *new_hash_table = malloc(sizeof(tHash_Table));
    if (new_hash_table == NULL)
    {
        // TODO RAISE MALLOC ERROR
        return;
    }
    ht_init(new_hash_table);
    symtable->top++;
    symtable->HT_array[symtable->top] = new_hash_table;
}

void st_dedent (tSymtable *symtable)
{
    if (symtable->top <= 0)
    {
        // TODO RAISE CANNOT DEDENT MAIN CONTEXT
        return;
    }

    ht_clean(symtable->HT_array[symtable->top]);
    free(symtable->HT_array[symtable->top]);
    symtable->HT_array[symtable->top] = NULL;
    symtable->top--;
}

void st_insert_entry_in_current_context (tSymtable *symtable, tKey key, void *data, tHTType type)
{
    tHash_Table *hash_table = symtable->HT_array[symtable->top];

    tHash_Table_Item* item = ht_search(hash_table, key);
    if (item != NULL) // pokud existuje polozka se stejnym klicem, jednoducse prepise jeji data
    {
        switch (type)
        {
            case HT_TYPE_VARIABLE:
                if (!item->var_data)
                {
                    // raise error
                    return;
                }
                item->var_data = (tVariableData*) data;
                break;
            case HT_TYPE_FUNCTION:
                if (!item->fun_data)
                {
                    // raise error
                    return;
                }
                item->fun_data = (tFunctionData*) data;
                break;
            case HT_TYPE_NODE:
                if (!item->node_data)
                {
                    // raise error
                    return;
                }
                item->node_data = (tNodeData *) data;
                break;
            default:
                // todo raise error 99
                break;
        }
        item->type = type;
        return;
    }
    
    // polozka neexistuje => pokracuje se ve vkladani na zacatek seznamu
    int hash = st_generate_hash(key);
    tHash_Table_Item* new_item = malloc(sizeof(tHash_Table_Item));
    if (new_item == NULL)
    {
        // TODO RAISE MALLOC ERROR
        return;
    }
    // new_item->key todo
    new_item->key = key; //DELETE
    switch (type)
    {
        case HT_TYPE_VARIABLE:
                new_item->var_data = (tVariableData*) data;
                new_item->fun_data = NULL;
                new_item->node_data = NULL;
            break;
        case HT_TYPE_FUNCTION:
                new_item->fun_data = (tFunctionData*) data;
                new_item->var_data = NULL;
                new_item->node_data = NULL;
            break;
        case HT_TYPE_NODE:
                new_item->node_data = (tNodeData *) data;
                new_item->var_data = NULL;
                new_item->fun_data = NULL;
            break;
        default:
            // todo raise error 99
            break;
    }
    new_item->type = type;
    new_item->next_item = (*hash_table)[hash];
    (*hash_table)[hash] = new_item;

    return;
}

tHash_Table_Item* st_insert_entry_in_current_context_random_key (tSymtable *symtable, void *data, tHTType type)
{
    tKey key = st_generate_random_key();
    st_insert_entry_in_current_context (symtable, key, data, type);
    tHash_Table_Item *item = st_search_entry(symtable, key);
    return item;
}


tHash_Table_Item* st_search_entry (tSymtable *symtable, tKey key)
{

    tHash_Table_Item* item;
    int hash = st_generate_hash(key);

    for (int i = symtable->top; i >= 0; i--)
    {
        tHash_Table *hash_table = symtable->HT_array[i];
        // int hash = st_generate_hash(key); // delete
        item = (*hash_table)[hash]; // prvni polozka v zretezenem seznamu

        // prohledani zretezeneho seznamu
        while (item != NULL)
        {
            if (!strcmp(item->key->str, key->str))
            {
                return item;
            }
            item = item->next_item;
        }
    }
    return NULL;
}

void st_clean_all (tSymtable *symtable)
{
    // projizdi v zasobniku tabulky jednu po jedne
    for (int i = symtable->top; i >= 0; i--)
    {
        ht_clean(symtable->HT_array[i]);
        free(symtable->HT_array[i]);
        symtable->HT_array[i] = NULL;
    }
}

/* FUNKCE PRO HASH TABULKU */

void ht_init (tHash_Table* hash_table)
{
    // nastaveni pointeru v tabulce na NULL
    for (int i = 0; i < HT_SIZE; i++)
    {
        (*hash_table)[i] = NULL;
    }
}

void ht_clean ( tHash_Table* hash_table )
{
    tHash_Table_Item* item;
    tHash_Table_Item* next_item;

    // prochazeni celeho pole
    for (int i = 0; i < HT_SIZE; i++)
    {
        item = (*hash_table)[i]; // prvni polozka v zretezenem seznamu
        // prochazeni zretezenych seznamu (polozek pole)
        while(item != NULL)
        {
            next_item = item->next_item;
            ht_item_clean(item);
            free(item);
            item = next_item;
        }
        (*hash_table)[i] = NULL;
    }
}

void ht_item_clean ( tHash_Table_Item* item )
{
    //  uvolneni klice
    string_free(item->key);
    free(item->key);
    item->key = NULL;

    // uvolneni fun_data
    free(item->fun_data);
    item->fun_data = NULL;

    // uvolneni var_data
    free(item->var_data);
    item->var_data = NULL;

    // uvolneni node_data
    free(item->node_data);
    item->node_data = NULL;
}


tHash_Table_Item* ht_search ( tHash_Table* hash_table, tKey key )
{
    tHash_Table_Item* item;

    int hash = st_generate_hash(key);
    item = (*hash_table)[hash]; // prvni polozka v zretezenem seznamu

    // prohledani zretezeneho seznamu
    while (item != NULL)
    {
        if (!strcmp(item->key->str, key->str))
        {
            return item;
        }
        item = item->next_item;
    }

    return NULL;
}

// void ht_insert ( tHash_Table* hash_table, tKey key, tData* data )
// {
//     tHash_Table_Item* item = htSearch(hash_table, key);
//     if (item != NULL) // pokud existuje polozka se stejnym klicem, jednoducse prepise jeji data
//     {
//         item->data = data;
//         return;
//     }
    
//     // polozka neexistuje => pokracuje se ve vkladani na zacatek seznamu
//     int hash = hashCode(key);
//     tHash_Table_Item* new_item = malloc(sizeof(tHash_Table_Item));
//     if (new_item != NULL)
//     {
//         new_item->key = key;
//         new_item->data = data;
//         new_item->next_item = (*hash_table)[hash];
//     }
//     (*hash_table)[hash] = new_item;

//     return;
// }

// /*
// ** TRP s explicitně zřetězenými synonymy.
// ** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
// ** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
// ** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
// ** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
// **
// ** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
// ** prvku do seznamu synonym použijte co nejefektivnější způsob,
// ** tedy proveďte.vložení prvku na začátek seznamu.
// **/



// /*
// ** TRP s explicitně zřetězenými synonymy.
// ** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
// ** Pokud je položka nalezena, vrací funkce ukazatel na položku
// ** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
// **
// ** Využijte dříve vytvořenou funkci HTSearch.
// */

// tData* htRead ( tHTable* ptrht, tKey key )
// {
//     tHTItem* item = htSearch(ptrht, key);
//     return item != NULL ? &(item->data) : NULL;
// }

// /*
// ** TRP s explicitně zřetězenými synonymy.
// ** Tato procedura vyjme položku s klíčem key z tabulky
// ** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
// ** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
// ** nic).
// **
// ** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
// */

// void htDelete ( tHTable* ptrht, tKey key )
// {
//     tHTItem* item;
//     int hash = hashCode(key);

//     item = (*ptrht)[hash]; // prvni polozka v poli na indexu danem klicem
    
//     if (item == NULL) // zadna polozka indexu danem klicem v poli neexistuje
//     {
//         return;
//     }

//     if (!strcmp(item->key, key)) // pokud je mazana polozka prvni v zretezenem seznamu
//     {
//         tHTItem* temp = item->ptrnext;
//         free((*ptrht)[hash]);
//         (*ptrht)[hash] = temp;
//         return;
//     }

//     // hledani prvku v zretezenem seznamu
//     while (item->ptrnext != NULL)
//     {
//         if (!strcmp(item->ptrnext->key, key))
//         {
//             if (item->ptrnext->ptrnext != NULL)// mazany prvek neni posledni
//             {
//                 tHTItem* temp = item->ptrnext->ptrnext;
//                 free(item->ptrnext);
//                 item->ptrnext = temp;
//                 return;
//             }
//             else // mazany prvek je posledni
//             {
//                 free(item->ptrnext);
//                 item->ptrnext = NULL;
//                 return;
//             }
//         }
//         item = item->ptrnext;
//     }
// }

// /* TRP s explicitně zřetězenými synonymy.
// ** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
// ** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
// */


tKey st_generate_random_key()
{
    tKey key = malloc(sizeof(dynamic_string));
    string_init(key);
    const char symbols[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    int symbol_count = strlen(symbols);
    for (int i = 0; i < 127; i++)
    {
        string_add_char(key, symbols[rand() % symbol_count]);
    }
    return key;
}

int st_generate_hash(tKey key)
{
    unsigned long hash = 1, pow;
    const int len_s = strlen(key->str);
    int loop_stop = ((len_s * len_s) % 500) + 5;

    for (int i = 0; i < loop_stop; i++)
    {
        pow = 2;
        for (int j = 0; j < len_s; j++)
        {
            pow *= key->str[j] * (i + 1);
        }
        hash += pow;
    }
    return (hash % HT_SIZE);
}