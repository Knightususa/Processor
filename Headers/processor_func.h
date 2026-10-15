#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>
#include <math.h>

#include "./common.h"
#include "../Stack_lib/Headers/dump.h"
#include "./assembler.h"


typedef struct 
{
    ByteCodeStruct byteCodeStruct;
    StackElem_t *registers;
    Stack_t *stk;
    Stack_t *stkRet;
    size_t ip;//instruction pointer
}ProcessorStruct;


typedef struct
{
    COMMAND_INDEX index;
    const char *name;
    int args;
    enum ERRORS_PROCESSOR (*function)(ProcessorStruct *);
}Command;

extern const Command commands_[];  
extern const size_t number_of_commands_;

extern const int number_of_registers;
extern const size_t number_of_elem_on_line;

void RunProcessor(ByteCodeStruct byteCodeStruct, Stack_t *stk);
ProcessorStruct *ProcessorStructInit(ByteCodeStruct byteCodeStruct, Stack_t *stk);
void ProcessorRegistersInit(ProcessorStruct *proc);
void PrintProcessor(ProcessorStruct *proc);

void *MakeElemFromByteCode(ProcessorStruct *proc, size_t size);

enum ERRORS_PROCESSOR ProcessorHlt(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorPush(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorOut(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorPusr(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorPopr(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorAdd(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorSub(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorDiv(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorMul(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorSqrt(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorJmp(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorJe(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorJne(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorJa(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorJae(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorJb(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorJbe(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorIn(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorCall(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorRet(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorNotComm(ProcessorStruct *proc);
#endif