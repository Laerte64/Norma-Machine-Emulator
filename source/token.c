#include "token.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

Token* createToken(TokenType type, size_t line, size_t column, Token *past, void *data, bool spaceAfter)
{
    Token *token = (Token*) malloc(sizeof(Token));
    if (token == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    token->type = type;
    token->line = line;
    token->column = column;

    if (past == NULL)
        token->past == NULL;
    else
    {
        token->past = past;
        past->next = token;
    }
    token->next = NULL;

    token->data = data;
    token->spaceAfter = spaceAfter;
    return token;
}

void destroyToken(Token *token)
{
    if (token->data != NULL)
        free(token->data);
    free(token);
}

void printToken(char buffer[], size_t size, Token *token)
{
    switch (token->type)
    {
    case token_identifier:
        snprintf(buffer, size, "Identifier at l%zu c%zu: <%s>", token->line, token->column, (char*) token->data);
        break;
    case token_number:
        snprintf(buffer, size, "Number at l%zu c%zu: <%zu>", token->line, token->column, *(size_t*) token->data);
        break;
    case token_newline:
        snprintf(buffer, size, "NewLine at l%zu c%zu", token->line, token->column);
        break;
    case token_set:
        snprintf(buffer, size, "Keyword at l%zu c%zu: <set>", token->line, token->column);
        break;
    case token_inc:
        snprintf(buffer, size, "Keyword at l%zu c%zu: <inc>", token->line, token->column);
        break;
    case token_dec:
        snprintf(buffer, size, "Keyword at l%zu c%zu: <dec>", token->line, token->column);
        break;
    case token_chk:
        snprintf(buffer, size, "Keyword at l%zu c%zu: <chk>", token->line, token->column);
        break;
    case token_jmp:
        snprintf(buffer, size, "Keyword at l%zu c%zu: <jmp>", token->line, token->column);
        break;
    case token_undefined:
        snprintf(buffer, size, "Undefined at l%zu c%zu", token->line, token->column);
        break;
    default:
        errno = EINVAL;
        break;
    }
    
}