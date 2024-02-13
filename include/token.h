#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef enum TokenType
{
    token_identifier,
    token_number,
    token_newline,
    token_set,
    token_inc,
    token_dec,
    token_chk,
    token_jmp,
    token_hlt,
    token_colon,
    token_undefined
} TokenType;

typedef struct Token Token;

Token* createToken(TokenType type, size_t line, size_t column, Token *past, void *data, bool spaceAfter);
void destroyToken(Token *token);
void printToken(char buffer[], size_t size, Token *token);

struct Token
{
    TokenType type;
    size_t line, column;
    Token *past, *next;
    void *data;
    bool spaceAfter;
};