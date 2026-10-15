#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <assert.h>
#include <errno.h>

#include "./stack_global.h"
#include "./common.h"
#include "./dump.h"

bool Verificator(Stack_t *stack);

enum ERRORS_STACK StackInit(Stack_t *stack);
enum ERRORS_STACK StackDestroy(Stack_t *stack);
enum ERRORS_STACK StackPush(Stack_t *stk, StackElem_t elem);
enum ERRORS_STACK StackPop(Stack_t *stk, StackElem_t *popValue);

enum ERRORS_STACK ResizeUp(Stack_t *stack);
enum ERRORS_STACK ResizeDown(Stack_t *stack);

enum ERRORS_STACK CheckStack(Stack_t stk);    

bool IsValuePoison(Stack_t stack, StackElem_t value);
bool IsZero(StackElem_t a, StackElem_t b);

void EditHash(unsigned long long *hash, unsigned long long value);

CANARY_PROTECTION(void MakeCanaryEnd(Stack_t *stk);)

HASH_PROTECTION(unsigned long long CalculateHashData(Stack_t stk);
                unsigned long long CalculateHashStack(Stack_t stk);)


#endif
//end #ifndef STACK_H