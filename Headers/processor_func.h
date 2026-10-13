#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>

#include "./common.h"
#include "./stack.h"
#include "./assembler.h"

typedef struct 
{
    ByteCodeStruct byteCodeStruct;
    Stack_t *stk;
    size_t ip;//instruction pointer
}ProcessorStruct;

extern const COMMAND_INDEX commands_[]; 
extern const size_t number_of_commands_;

extern const COMMAND_INDEX arg_commands_[]; 
extern const size_t number_of_arg_commands_;

void RunProcessor(ByteCodeStruct byteCodeStruct, Stack_t *stk);
StackElem_t MakeElemFromByteCode(ProcessorStruct *proc);

enum ERRORS ProcessorPush(ProcessorStruct *proc);
enum ERRORS ProcessorOut(ProcessorStruct *proc);

enum ERRORS ProcessorAdd(ProcessorStruct *proc);
enum ERRORS ProcessorSub(ProcessorStruct *proc);
enum ERRORS ProcessorDiv(ProcessorStruct *proc);
enum ERRORS ProcessorMul(ProcessorStruct *proc);

enum ERRORS ProcessorJump(ProcessorStruct *proc, COMMAND_INDEX index);


#endif