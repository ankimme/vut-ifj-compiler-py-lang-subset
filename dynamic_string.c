/*
 * @file dynamic_string.c
 * @brief Pomocná knihovna pro tvorbu a práci s řetězci
 * @author Matej Alexej Helc (xhelcm00@stud.fit.vutbr.cz)
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 27.10.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "dynamic_string.h"


void string_clear(dynamic_string *s)
{
	s->length = 0;
	for (int i = 0; i < s->allocated_size; i++)
	{
		s->str[i] = '\0';
	}
}

int string_init(dynamic_string *s)
{
	if (s == NULL)
	{
		return 0;
	}

	s->str = malloc(sizeof(char) * CHUNK_SIZE);
	if (s->str == NULL)
	{
		return 0;
	}
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		s->str[i] = '\0';
	}


	s->allocated_size = CHUNK_SIZE;
	s->length = 0;

	return 1;
}

void string_free(dynamic_string *s)
{
	free(s->str);
	s->str = NULL;
	s->allocated_size = 0;
	s->length = 0;
}

int string_add_char(dynamic_string *s, char c)
{
	if (s->length + 1 >= s->allocated_size)
	{
		int new_string_length = s->length + CHUNK_SIZE;
		char* new_string_ptr = malloc(sizeof(char) * new_string_length);
		if (new_string_ptr == NULL)
		{
			return 0;
		}
		for (int i = 0; i < new_string_length; i++)
		{
			new_string_ptr[i] = '\0';
		}
		strcpy(new_string_ptr, s->str);
		free(s->str);
		s->str = new_string_ptr;
		s->allocated_size = new_string_length;
	}

	s->str[s->length] = c;
	s->length++;

	return 1;
}

int string_append(dynamic_string *s, char *string)
{
	int new_string_length = s->length + strlen(string) + 1; // delka vcetne znaku '\0'

	if (new_string_length >= s->allocated_size)
	{
		char* new_string_ptr = malloc(sizeof(char) * new_string_length);
		if (new_string_ptr == NULL)
		{
			return 0;
		}
		for (int i = 0; i < new_string_length; i++)
		{
			new_string_ptr[i] = '\0';
		}
		strcpy(new_string_ptr, s->str);
		free(s->str);
		s->str = new_string_ptr;
		s->allocated_size = new_string_length;
	}

	strcat(s->str, string);
	s->length += strlen(string); // nova delka bez znaku '\0'

	return 1;
}

