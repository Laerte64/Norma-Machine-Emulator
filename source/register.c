#include "register.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

Register *createRegister(char *label)
{
    Register *reg = (Register*) malloc(sizeof(Register));
    if (reg == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    reg->label = (char*) malloc(strlen(label) * sizeof(char));
    if (reg->label == NULL)
    {
        free(reg);
        errno = ENOMEM;
        return NULL;
    }
    strcpy(reg->label, label);

    reg->value = 0;
    reg->next = NULL;
    return reg;
}

void destroyRegister(Register *reg)
{
    free(reg->label);
    free(reg);
}

void pushRegister(Register *list, Register *reg)
{
    Register *end = list;
    while (end->next != NULL)
        end = end->next;
    end->next = reg;
}

Register* findRegister(Register *list, char *label)
{
    Register *reg = list;
    while(reg != NULL)
    {
        if (strcmp(label, reg->label) == 0)
            return reg;
        reg = reg->next;
    }
    return NULL;
}

void destroyRegisters(Register *list)
{
    Register *curr = list, *next;
    while (curr != NULL)
    {
        next = curr->next;
        destroyRegister(curr);
        curr = next;
    }
}

void printRegister(char *buffer, size_t size, Register *reg)
{
    unsigned int addr = (size_t)reg % 0x1000000;
    unsigned int next = (size_t)reg->next % 0x1000000;
    snprintf(buffer, size, "addr: <0x%06X> label: <%s> value: <%u> next: <0x%06X>", addr, reg->label, reg->value, next);
}

void printRegisters(Register *list)
{
    Register *reg = list;
    while (reg != NULL)
    {
        char buffer[100];
        printRegister(buffer, 100, reg);
        printf("%s\n", buffer);
        reg = reg->next;
    }
}