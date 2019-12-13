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

    // char one[] = "one";
    // char two[] = "two";
    // char three[] = "three";

    string_init(string);

    // string_append(string, one);
    // string_append(string, two);
    // string_append(string, three);
    // string_append(string, three);
    // string_append(string, three);
    // string_append(string, three);
    // string_append(string, three);
    // string_append(string, three);
    // string_append(string, three);
    // string_append(string, three);
    // string_append(string, three);

    // string_clear(string);

    // for (int i = 0; i < 2; i++)
    // {
    //     string_add_char(string, 'a');
    // }

    // string_add_char(string, 'a');
    string_append_int(string, 102390);

    printf("%s\n", string->str);

    string_free(string);

    free(string);
    string = NULL;

    printf("\033[0m"); 

    return 0;
}