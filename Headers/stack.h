#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <assert.h>
#include <errno.h>

#include "./global.h"
#include "./dump.h"
#include "../Headers/common.h"



bool Verificator(Stack_t *stack);

enum ERRORS StackInit(Stack_t *stack);
enum ERRORS StackDestroy(Stack_t *stack);
enum ERRORS StackPush(Stack_t *stack, StackElem_t valueToPush);
enum ERRORS StackPop(Stack_t *stack);

enum ERRORS ResizeUp(Stack_t *stack);
enum ERRORS ResizeDown(Stack_t *stack);

enum ERRORS CheckStack(Stack_t stk);

void EditHash(unsigned long long *hash, unsigned long long value);

CANARY_PROTECTION(void MakeCanaryEnd(Stack_t *stk);)

HASH_PROTECTION(unsigned long long CalculateHashData(Stack_t stk);
                unsigned long long CalculateHashStack(Stack_t stk);)


#endif
//end #ifndef STACK_H