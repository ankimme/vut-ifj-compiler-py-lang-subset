#include <string.h>
#include <stdlib.h>

#define INCREMENT_SIZE 100

typedef struct{

    unsigned long key;
    St_item* value;

} St_entry;

typedef struct {

    int size;
    int item_count;
    St_entry** entries;

} St_table;

typedef enum {

    VARIABLE_OR_CONSTANT,
    FUNCTION

} Identificator_type;

typedef enum {

    INT,
    DOUBLE,
    STRING,
    //BOOL, CHAR

} Variable_type;

typedef struct {

    Identificator_type identificator_type;
    Variable_type variable_type;
    Data_value * data;

} St_item;

typedef union {

    int int_value;
    double double_value;
    char* string_value;

} Data_value;

static void st_init_table(St_table** t_init);

static void st_init_entry(St_entry** e_init);

static void st_delete_entry(St_entry** e_delete);

static void st_delete_table(St_table** t_delete);

void st_insert_item(St_table* table, char* identificator, St_item* value);

static void st_search_item(St_table* table, char* identificator);

static unsigned long st_generate_hash(char *s);