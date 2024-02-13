#pragma once

#include <stddef.h>

typedef struct Register Register;

Register *createRegister(char *label);
void destroyRegister(Register *reg);
void pushRegister(Register *list, Register *reg);
Register* findRegister(Register *list, char *label);
void destroyRegisters(Register *list);
void printRegister(char *buffer, size_t size, Register *reg);
void printRegisters(Register *list);

struct Register
{
    char *label;
    unsigned int value;
    Register *next;
};