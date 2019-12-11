/*
 * @file dynamic_string.c
 * @brief Pomocná knihovna pro tvorbu a práci s řetězci
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

void string_trim(dynamic_string **s)
{
	if ((*s)->str[0] != '\'')
	{
		return;
	}

	// odstraneni prvniho a posledniho znaku
	(*s)->length--;
	(*s)->str[(*s)->length] = '\0';
	memmove((*s)->str, (*s)->str + 1, strlen((*s)->str));

	dynamic_string *new_string = malloc(sizeof(dynamic_string));
	string_init(new_string);
	char c = (*s)->str[0];
	char escape[5];

	for (int i = 0; c != '\0'; i++)
	{
		// tabulator
		if (c == '\\' && (*s)->str[i + 1] == 't')
		{
			string_append(new_string, "\\009");
			i++;
		}
		// newline
		else if (c == '\\' && (*s)->str[i + 1] == 'n')
		{
			string_append(new_string, "\\010");
			i++;
		}
		// dvojta uvozovka
		else if (c == '\\' && (*s)->str[i + 1] == '"')
		{
			string_append(new_string, "\\034");
			i++;
		}
		// jednotna uvozovka
		else if (c == '\\' && (*s)->str[i + 1] == '\'')
		{
			string_append(new_string, "\\039");
			i++;
		}
		// lomitko zpetne
		else if (c == '\\' && (*s)->str[i + 1] == '\\')
		{
			string_append(new_string, "\\092");
			i++;
		}
		// hexa hodnota
		else if (c == '\\' && (*s)->str[i + 1] == 'x' && isxdigit((*s)->str[i + 2]) && isxdigit((*s)->str[i + 3]))
		{
			c = 0;
			char hex_value[] = "00";
			hex_value[0] = (*s)->str[i + 2];
			hex_value[1] = (*s)->str[i + 3];
			long b = strtol(hex_value, NULL, 16);
			if (b > 127)
			{
				c = 32;
			}
			else
			{
				c = b;
			}
			
			i += 2;
			continue;
		}
		// ostatni znaky ktere vyzaduji escape sekvenci
		else if (c <= 32 || c == 35 || c == 92)
		{
			sprintf(escape, "\\0%d", c);
			string_append(new_string, escape);
		}
		else
		{
			string_add_char(new_string, c);
		}
		c = (*s)->str[i + 1];
	}

	string_free(*s);
	free(*s);
	*s = new_string;
}


void string_append_int(dynamic_string *s, int a)
{
	while(a)
	{
		string_add_char(s, (a % 10) + 48);
		a /= 10;
	}
}
