#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>
#include <math.h>

#include "./common.h"
#include "./stack.h"
#include "./assembler.h"


typedef struct 
{
    ByteCodeStruct byteCodeStruct;
    StackElem_t *registers;
    Stack_t *stk;
    size_t ip;//instruction pointer
}ProcessorStruct;


typedef struct
{
    COMMAND_INDEX index;
    const char *name;
    int args;
    enum ERRORS (*function)(ProcessorStruct *);
}Command;

extern const Command commands_[];  
extern const size_t number_of_commands_;

extern const int number_of_registers;

void RunProcessor(ByteCodeStruct byteCodeStruct, Stack_t *stk);
ProcessorStruct *ProcessorStructInit(ByteCodeStruct byteCodeStruct, Stack_t *stk);
void ProcessorRegistersInit(ProcessorStruct *proc);

void *MakeElemFromByteCode(ProcessorStruct *proc, size_t size);

enum ERRORS ProcessorHlt(ProcessorStruct *proc);

enum ERRORS ProcessorPush(ProcessorStruct *proc);
enum ERRORS ProcessorOut(ProcessorStruct *proc);

enum ERRORS ProcessorPusr(ProcessorStruct *proc);
enum ERRORS ProcessorPopr(ProcessorStruct *proc);

enum ERRORS ProcessorAdd(ProcessorStruct *proc);
enum ERRORS ProcessorSub(ProcessorStruct *proc);
enum ERRORS ProcessorDiv(ProcessorStruct *proc);
enum ERRORS ProcessorMul(ProcessorStruct *proc);
enum ERRORS ProcessorSqrt(ProcessorStruct *proc);

enum ERRORS ProcessorJmp(ProcessorStruct *proc);
enum ERRORS ProcessorJe(ProcessorStruct *proc);
enum ERRORS ProcessorJa(ProcessorStruct *proc);
enum ERRORS ProcessorJae(ProcessorStruct *proc);
enum ERRORS ProcessorJb(ProcessorStruct *proc);
enum ERRORS ProcessorJbe(ProcessorStruct *proc);

enum ERRORS ProcessorNotComm(ProcessorStruct *proc);
#endif