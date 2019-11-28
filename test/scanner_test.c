/*
 * @file scanner_test.c
 * @brief Test scanneru
 * @author Andrea Chimenti (xchime00@stud.fit.vutbr.cz)
 * @date 28.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 * 
 */

#include "../scanner.h"
// #include <stdio.h>


void print_token(St_token *token)
{
    printf("TYPE: %i\n", token->type);

    if (token->attribute.string)
    {
        printf("ATTRIBUTE STRING: %s\n", token->attribute.string->str);
    }
    if (token->attribute.number)
    {
        printf("ATTRIBUTE NUMBER: %i\n", token->attribute.number);
    }
    if (token->attribute.decimal)
    {
        printf("ATTRIBUTE DECIMAL: %f\n", token->attribute.decimal);
    }

    printf("ERROR VALUE: %i\n", token->error_value);
    printf("\n");
}

int main()
{

    St_token token;
    token.type = UNDEFINED;
    token.attribute.string = NULL;
    token.error_value = NO_ERROR;
    tStack stack;
    tStack_init(&stack);
    tStack_push(&stack, 0);

    // for (int i = 0; i < 8; i++)
    // {
    //     get_next_token(&token, &stack);
    //     print_token(&token);
    // }

    get_next_token(&token, &stack);
    print_token(&token);

    get_next_token(&token, &stack);
    print_token(&token);

    return 0;
}