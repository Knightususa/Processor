#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdbool.h>
#include "./global.h"
#include "./common.h"
#include "../Stack_lib/Headers/dump.h"
#include "../Stack_lib/Headers/stack.h"


#ifdef PROC_DEBUG
    #define ON_PROC_DEBUG(...) __VA_ARGS__
#else
    #define ON_PROC_DEBUG(...)
#endif

enum ERRORS_PROCESSOR
{
    procCantAdd,
    procCantSub,
    procCantDiv,
    procCantMul,
    procCantSqrt,
    procCantJmp,
    procCantIn,
    procNoErr
};


typedef struct 
{
    ByteCodeStruct byteCodeStruct;
    StackElem_t *registers;
    StackElem_t *RAM;
    Stack_t *stk;
    Stack_t *stkRet;
    size_t ip;//instruction pointer
}ProcessorStruct;

typedef struct
{
    char *filename;
}OptionsProc;

void GetOptionsProc(int argc, char **argv, OptionsProc *pOptions);
void DelNameExt(OptionsProc *pOptions);
char *CreateName(char *filename, const char *ext);

ByteCodeStruct ReadFileBytecode(char *fileName);
ProcessorStruct *ProcessorStructInit(ByteCodeStruct byteCodeStruct);
void ProcessorRegistersInit(ProcessorStruct *proc);
void ProcessorRAMInit(ProcessorStruct *proc);

void PrintProcessor(ProcessorStruct *proc);
void PrintBytecode(ProcessorStruct *proc);
void PrintRegisters(ProcessorStruct *proc);
void PrintReturns(ProcessorStruct *proc);
void PrintRAM(ProcessorStruct *proc);

void *MakeElemFromByteCode(ProcessorStruct *proc, size_t size);


enum ERRORS_PROCESSOR ProcessorHlt(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorPush(ProcessorStruct *proc);
enum ERRORS_PROCESSOR ProcessorOut(ProcessorStruct *proc);

enum ERRORS_PROCESSOR ProcessorPop(ProcessorStruct *proc);

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



typedef struct
{
    CommandGlobal cmd;
    ERRORS_PROCESSOR (*function)(ProcessorStruct *);
}CommandProcessor;


CommandProcessor commands_processor[] = 
{
    {.function = *ProcessorHlt},
    
    {.function = *ProcessorPush},
    {.function = *ProcessorOut},

    {.function = *ProcessorPop},

    {.function = *ProcessorAdd},
    {.function = *ProcessorSub},
    {.function = *ProcessorDiv},
    {.function = *ProcessorMul},
    {.function = *ProcessorSqrt},

    {.function = *ProcessorJmp},
    {.function = *ProcessorJe},
    {.function = *ProcessorJne},
    {.function = *ProcessorJa},
    {.function = *ProcessorJae},
    {.function = *ProcessorJb},
    {.function = *ProcessorJbe},

    {.function = *ProcessorIn},
    {.function = *ProcessorCall},
    {.function = *ProcessorRet},

    {.function = *ProcessorNotComm},
};

const size_t number_of_elem_on_line    = 16;//How much elem of bytecode show PrintProcessor
const size_t number_of_elem_on_display = 20;//How much elem of bytecode show PrintProcessor
//TODO: rename to VM

#endif