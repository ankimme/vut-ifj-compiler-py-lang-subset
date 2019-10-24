#include <string.h>
#include <stdlib.h>
 
#define INCREMENT_SIZE 100

typedef struct{

	char* key;
	char* value;

} St_entry;

typedef struct {

	int size;
	int item_count;
	st_entry** entries;

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
	Data_values * data;

} St_item;

typedef union {

	int int_value;
	double double_value;
	char* string_value;

} Data_values;

static void init_table(St_table** t_init) {

	*t_init = malloc(sizeof(St_table);

	if (*t_init == NULL) {

		error_handle(99);
		return;
	}

	*t_init->size = INCREMENT_SIZE;
	*t_init->item_count = 0;
	*t_init->entries = malloc(sizeof(St_entry) * INCREMENT_SIZE);

	if (*t_init->entries == NULL) {
		free(*t_init);
		error_handle(99);
		return;
	}

} 

static void init_entry(St_entry** e_init, enum identificator_type) {

	*e_init = malloc(sizeof(St_entry); 

	if (*e_init == NULL) {

		error_handle(99);
		return;
	}

}

static void delete_entry(St_entry** e_delete) {

	free(*e_delete->key);
	free(*e_delete->value); // TODO funkce na smazání obsahu value
	free(*e_delete); 

}

static void delete_table(St_table** t_delete) {

	for (int i = 0; i < *t_delete->size; i++) {
	
		St_entry* entry = *t_delete->entries[i];
		if (entry != NULL) {
		
			delete_entry(entry);
		
		}
	}

	free(*t_delete->entries);
	free(*t_delete);

}

static void insert_item(St_table* st_insert, char* key, char* value) {

	St_entry* ins_entry = init_entry(key, value);
	
	int index = st_generate_hash(ins_entry->key, st_insert->size);
	
	St_entry* cur_entry = st_insert->items[index];
	
	int i = 1;

	while (cur_entry != NULL) {
		
		index = st_generate_hash(ins_entry->key, st_insert->size);
		cur_entry = st_insert->items[index];
		i++;
	}

	st_insert->items[index] = ins_entry;
	st_insert->count++;
}

static int st_generate_hash(int key, char *s) {

	long hash = 0;
	int len_s = strlen(s);
	
	for (int i = 0; i < len_s; i++) { //TODO 
	
		hash += (long)s[i] * (len_s + key * (len_s - key * pow(2,key)));
	
	}

	return hash;

}

//TODO VYØEŠIT KOLIZE


/*
e_init->identificator_type = identificator_type;
e_init->variable_type =
*/
