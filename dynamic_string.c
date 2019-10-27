/*
 * @file dynamic_string.c
 * @brief Pomocná knihovna pro tvorbu a práci s řetězci
 * @author Matej Alexej Helc (xhelcm00@stud.fit.vutbr.cz)
 * @date XX.XX.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "dynamic_string.h"

#define DYNAMIC_STRING_LEN 16

int string_init(Dynamic_string *s){

	if (!(s->str = (char *) malloc(DYNAMIC_STRING_LEN))){
		return 0;
	}

	s->alloc_size = DYNAMIC_STRING_LEN;

	return 1;
}

void string_free(Dynamic_string *s){

	free(s->str);
}

int strings_cmp(Dynamic_string *dynamic_string, const char *string){

	return strcmp(dynamic_string->str, const_string);
}

int strings_cat(Dynamic_string *s, const char *string){

	unsigned int string_length = (unsigned int) strlen(string);

	if (s->length + string_length + 1 >= s->alloc_size){
		unsigned int new_size = s->length + string_length + 1;

		if (!(s->str = (char *) realloc(s->str, new_size))){
			return 0;
		}
		s->alloc_size = new_size;
	}

	s->length += string_length;
	strcat(s->str, string);
	s->str[s->length] = '\0';

	return 1;
}
