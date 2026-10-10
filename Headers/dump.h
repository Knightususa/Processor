#ifndef DUMP_H
#define DUMP_H

#include "./global.h"
#include "./stack.h"

int StackDumpCID(Stack_t *stack);
int StackDumpS(Stack_t *stack);

bool IsValuePoisonCI(Stack_t stack, StackElem_t value);
bool IsValuePoisonD(Stack_t stack, StackElem_t value);
bool IsValuePoisonS(Stack_t stack, StackElem_t value);

#endif
//end #ifndef DUMP_H