#ifndef DUMP_H
#define DUMP_H

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <assert.h>
#include <errno.h>
#include "./global.h"

typedef struct
{
    CANARY_PROTECTION(unsigned long long arbusL; //LEFT CANARY
                      unsigned long long arbusDefault;)//DEFAULT VALUE OF CANARY
    int size;
    int capacity;
    StackElem_t *data;
    StackElem_t poison_v;
    CANARY_PROTECTION(unsigned long long *canaryL;
                      unsigned long long *canaryR;
                      unsigned long long canaryDefault;)
    HASH_PROTECTION(unsigned long long hashData;
        unsigned long long hashStack;)
    CANARY_PROTECTION(unsigned long long arbusR;) //RIGHT CANARY
}Stack_t;

int StackDumpCID(Stack_t *stack);
int StackDumpS(Stack_t *stack);

bool IsValuePoisonCI(Stack_t stack, StackElem_t value);
bool IsValuePoisonD(Stack_t stack, StackElem_t value);
bool IsValuePoisonS(Stack_t stack, StackElem_t value);

#endif
//end #ifndef DUMP_H