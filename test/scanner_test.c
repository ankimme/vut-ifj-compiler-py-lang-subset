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

const char* get_token_type(int type) 
{
   switch (type) 
   {
      case 0: return "Indent";
      case 1: return "Dedent";
      case 2: return "EOL";
      case 3: return "EOF";
      case 4: return "Comma";
      case 5: return "Colon";
      case 6: return "Left bracket";
      case 7: return "Right bracket";
      case 8: return "Plus";
      case 9: return "Minus";
      case 10: return "Multiply";
      case 11: return "Keyword";
      case 12: return "Identifier";
      case 13: return "Integer";
      case 14: return "Double";
      case 15: return "Not equal";
      case 16: return "Equal";
      case 17: return "Less or equal";
      case 18: return "Greater or equal";
      case 19: return "Less than";
      case 20: return "Greater than";
      case 21: return "Assignment";
      case 22: return "Divide integer";
      case 23: return "Divide float";
      case 24: return "String literal";
      case 25: return "Undefined";
      default: return "NO TYPE! PROBABLY AN ERROR";
      
   }
}

void print_token(St_token *token)
{
    printf("TYPE: %s\n", get_token_type(token->type));

    if (token->attribute)
    {
        printf("ATTRIBUTE STRING: %s\n", token->attribute->str);
    }

    printf("ERROR VALUE: %i\n", token->error_value);
    printf("\n");
}

int main()
{
    printf("\033[0;36m");

    St_token token;
    token.type = UNDEFINED;
    token.attribute = malloc(sizeof(dynamic_string));
    string_init(token.attribute);
    token.error_value = NO_ERROR;
    tStack stack;
    tStack_init(&stack);
    tStack_push(&stack, 0);

    while (token.type != 3)
    {
        string_clear(token.attribute);
        get_next_token(&token, &stack);
        if (token.error_value != 0)
        {
            printf("\033[0;31m");
        }
        print_token(&token);
    }

    string_free(token.attribute);
    free(token.attribute);

    printf("\033[0m"); 
    return 0;
}