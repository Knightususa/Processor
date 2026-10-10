#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>

#include "./global.h"
#include "./stack.h"
#include "../Headers/common.h"



enum ERRORS ProcessorAdd(Stack_t *stk);
enum ERRORS ProcessorSub(Stack_t *stk);
enum ERRORS ProcessorDiv(Stack_t *stk);
enum ERRORS ProcessorMul(Stack_t *stk); 

extern const command push_;
extern const command add_;
extern const command sub_;
extern const command div_;
extern const command mul_;
extern const command notcomm_;

extern const command commands_[]; 
extern const size_t number_of_commands_;

#endif