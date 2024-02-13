#include "lexer.h"
#include "token.h"
#include "keyword.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>

static void parseWord(Lexer *lexer);
static void parseNumber(Lexer *lexer);

Lexer* createLexer(FILE *file)
{
    Lexer *lexer = (Lexer*) malloc(sizeof(Lexer));
    if (lexer == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    fseek(file, 0, SEEK_SET);
    lexer->file = file;
    lexer->line = 1;
    lexer->column = 1;
    lexer->currChar = 0;
    lexer->currToken = NULL;
    lexer->tokens = NULL;

    return lexer;
}

void destroyLexer(Lexer *lexer)
{
    Token *curr = lexer->tokens, *next;
    while (curr != NULL)
    {
        next = curr->next;
        destroyToken(curr);
        curr = next;
    }
    free(lexer);
}

void tokenize(Lexer *lexer)
{
    Token *falseToken = createToken(token_undefined, 0, 0, NULL, NULL, false);
    if (falseToken == NULL)
    {
        errno = ENOMEM;
        return;
    }
    lexer->tokens = falseToken;
    lexer->currToken = falseToken;

    lexer->currChar = fgetc(lexer->file);
    while(lexer->currChar != EOF && errno == 0)
    {
        if (lexer->currChar == '\n')
        {
            lexer->currChar = fgetc(lexer->file);
            lexer->currToken = createToken(token_newline, lexer->line, lexer->column, lexer->currToken, NULL, isspace(lexer->currChar));
            lexer->line++;
            lexer->column = 0;
        }
        else if (lexer->currChar == ' ')
        {
            lexer->currChar = fgetc(lexer->file);
            lexer->column++;
        }
            
        else if (lexer->currChar == '\t')
        {
            lexer->currChar = fgetc(lexer->file);
            lexer->column += 4 - lexer->column % 4;
        }
        else if (isalpha(lexer->currChar))
            parseWord(lexer);
        else if (isdigit(lexer->currChar))
            parseNumber(lexer);
        else if (lexer->currChar == ':')
        {
            lexer->currChar = fgetc(lexer->file);
            lexer->currToken = createToken(token_colon, lexer->line, lexer->column, lexer->currToken, NULL, isspace(lexer->currChar));
            lexer->column++;
        }
        else
        {
            errno = EINVAL;
            fprintf(stderr, "Invalid Symbol at l%zu c%zu: %c\n", lexer->line, lexer->column, lexer->currChar);
        }
    }

    lexer->tokens = falseToken->next;
    if (lexer->tokens != NULL)
        lexer->tokens->past = NULL;
    if (lexer->currToken == falseToken)
        lexer->currToken = NULL;
    destroyToken(falseToken);
}

static void parseWord(Lexer *lexer)
{
    size_t size = 1;
    char *data = malloc((size+1) * sizeof(char));
    if (data == NULL)
    {
        errno = ENOMEM;
        return;
    }
    data[0] = lexer->currChar;
    lexer->currChar = fgetc(lexer->file);

    while (isalpha(lexer->currChar) || isdigit(lexer->currChar) || lexer->currChar == '_')
    {
        size++;
        char *new_data = realloc(data, (size+1) * sizeof(char));
        if (new_data == NULL)
        {
            free(data);
            errno = ENOMEM;
            return;
        }
        data = new_data;
        data[size-1] = lexer->currChar;
        lexer->currChar = fgetc(lexer->file);
    }
    data[size] = '\0';

    TokenType type = analyseWord(data);
    Token *token;
    if (type == token_identifier)
        token = createToken(token_identifier, lexer->line, lexer->column, lexer->currToken, data, isspace(lexer->currChar));
    else
    {
        free(data);
        token = createToken(type, lexer->line, lexer->column, lexer->currToken, NULL, isspace(lexer->currChar));
    }
        
    lexer->currToken = token;
    lexer->column += size;
}

static void parseNumber(Lexer *lexer)
{
    size_t size = 1;
    char *data = malloc((size+1) * sizeof(char));
    if (data == NULL)
    {
        errno = ENOMEM;
        return;
    }
    data[0] = lexer->currChar;
    lexer->currChar = fgetc(lexer->file);

    while (isdigit(lexer->currChar))
    {
        size++;
        char *new_data = realloc(data, (size+1) * sizeof(char));
        if (new_data == NULL)
        {
            free(data);
            errno = ENOMEM;
            return;
        }
        data = new_data;
        data[size-1] = lexer->currChar;
        lexer->currChar = fgetc(lexer->file);
    }
    data[size] = '\0';

    if (isalpha(lexer->currChar) || lexer->currChar == '.')
    {
        free(data);
        errno = EINVAL;
        fprintf(stderr, "Invalid Number at l%zu c%zu\n", lexer->line, lexer->column);
        return;
    }

    unsigned int *value = malloc(sizeof(unsigned int));
    if (value == NULL)
    {
        free(data);
        errno = ENOMEM;
        return;
    }

    sscanf(data, "%u", value);
    free(data);

    Token *token = createToken(token_number, lexer->line, lexer->column, lexer->currToken, value, isspace(lexer->currChar));
    lexer->currToken = token;
    lexer->column += size;
}

void printTokens(Lexer *lexer)
{
    char buffer[100];
    Token *token = lexer->tokens;
    while (token != NULL)
    {
        printToken(buffer, 100, token);
        token = token->next;
        printf("%s\n", buffer);
    }
}

