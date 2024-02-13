#pragma once

#include <stddef.h>
#include <stdio.h>
#include "token.h"

typedef struct Lexer Lexer;

Lexer* createLexer(FILE *file);
void destroyLexer(Lexer *lexer);
void tokenize(Lexer *lexer);
void printTokens(Lexer *lexer);

struct Lexer
{
    FILE *file;
    size_t line, column;
    char currChar;
    Token *currToken;
    Token *tokens;
};