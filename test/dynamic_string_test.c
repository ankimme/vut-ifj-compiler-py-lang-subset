/*
 * @file dynamic_string_test.c
 * @brief Test knihovny pro praci s retezci
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 28.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */

#include "../dynamic_string.h"
#include <stdio.h>

int main()
{
    printf("\033[0;36m");
    dynamic_string *string = malloc(sizeof(dynamic_string));
    string->str = NULL;

    char one[] = "5555555555555555555555555555555555";
    char two[] = "two";
    char three[] = "three";

    string_init(string);

    strings_append(string, two);
    strings_append(string, two);
    strings_append(string, one);
    strings_append(string, three);
    // strings_append(string, three);
    // strings_append(string, three);
    // strings_append(string, three);
    // strings_append(string, three);
    // strings_append(string, three);
    // strings_append(string, three);
    // strings_append(string, three);
    // strings_append(string, three);

    // string_clear(string);

    for (int i = 0; i < 2; i++)
    {
        string_add_char(string, 'a');
    }

    printf("%s\n", string->str);

    string_free(string);

    free(string);
    string = NULL;

    printf("\033[0m"); 

    return 0;
}