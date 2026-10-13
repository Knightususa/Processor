#ifndef STACK_H
#define STACK_H

#include "./dump.h"
#include "./common.h"

bool Verificator(Stack_t *stack);

enum ERRORS StackInit(Stack_t *stack);
enum ERRORS StackDestroy(Stack_t *stack);
enum ERRORS StackPush(Stack_t *stk, StackElem_t elem);
enum ERRORS StackPop(Stack_t *stk, StackElem_t *popValue);

enum ERRORS ResizeUp(Stack_t *stack);
enum ERRORS ResizeDown(Stack_t *stack);

enum ERRORS CheckStack(Stack_t stk);    

void EditHash(unsigned long long *hash, unsigned long long value);

CANARY_PROTECTION(void MakeCanaryEnd(Stack_t *stk);)

HASH_PROTECTION(unsigned long long CalculateHashData(Stack_t stk);
                unsigned long long CalculateHashStack(Stack_t stk);)


#endif
//end #ifndef STACK_H