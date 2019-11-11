/*
 * @file parser.c
 * @brief Implementace syntaktického analyzátoru
 * @author 
 * @date 28.11.2019
 *
 * Projekt: Implementace překladače imperativního jazyka IFJ19 (varianta II)
 * VUT FIT
 */





int new_token(St_token* token, tStack* stack)
{
    get_next_token(token, stack)

    return token->error_value;
}