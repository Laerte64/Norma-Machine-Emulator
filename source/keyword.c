#include "keyword.h"
#include "token.h"
#include <string.h>

TokenType analyseWord(char word[])
{
    if (strlen(word) < 3)
        return token_identifier;
    if (strcmp(word, "set") == 0)
        return token_set;
    if (strcmp(word, "inc") == 0)
        return token_inc;
    if (strcmp(word, "dec") == 0)
        return token_dec;
    if (strcmp(word, "chk") == 0)
        return token_chk;
    if (strcmp(word, "jmp") == 0)
        return token_jmp;
    if (strcmp(word, "hlt") == 0)
        return token_hlt;
    return token_identifier;
}