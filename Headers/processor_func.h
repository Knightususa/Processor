#ifndef PROCESSOR_FUNC_H
#define PROCESSOR_FUNC_H

#include <stdio.h>
#include <math.h>

#include "./common.h"
#include "../Stack_lib/Headers/dump.h"
#include "./assembler.h"

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

enum COMMAND_INDEX
{
    hlt_index     = 0,
   
    push_index    = 1,
    out_index     = 2,
   
    pusr_index    = 3,
    popr_index    = 4,
   
    add_index     = 5,
    sub_index     = 6,
    div_index     = 7,
    mul_index     = 8,
    sqrt_index    = 9,
   
    jmp_index     = 10,
    je_index      = 11,
    jne_index     = 12,
    ja_index      = 13,
    jae_index     = 14,
    jb_index      = 15,
    jbe_index     = 16,

    in_index      = 17,
    call_index    = 18,
    ret_index     = 19,

    notcmd_index = 20
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
    COMMAND_INDEX index;
    const char *name;
    int args;
    ERRORS_PROCESSOR (*function)(ProcessorStruct *);
}Command;


void RunProcessor(ByteCodeStruct byteCodeStruct);


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

const Command commands_[] = 
{
    {.index = hlt_index,     .name = "HLT",  .args = 0, .function = *ProcessorHlt},
    
    {.index = push_index,    .name = "PUSH", .args = 1, .function = *ProcessorPush},
    {.index = out_index,     .name = "OUT",  .args = 0, .function = *ProcessorOut},

    {.index = pusr_index,    .name = "PUSR", .args = 1, .function = *ProcessorPusr},
    {.index = popr_index,    .name = "POPR", .args = 1, .function = *ProcessorPopr},

    {.index = add_index,     .name = "ADD",  .args = 0, .function = *ProcessorAdd},
    {.index = sub_index,     .name = "SUB",  .args = 0, .function = *ProcessorSub},
    {.index = div_index,     .name = "DIV",  .args = 0, .function = *ProcessorDiv},
    {.index = mul_index,     .name = "MUL",  .args = 0, .function = *ProcessorMul},
    {.index = sqrt_index,    .name = "SQRT", .args = 0, .function = *ProcessorSqrt},

    {.index = jmp_index,     .name = "JMP",  .args = 1, .function = *ProcessorJmp},
    {.index = je_index,      .name = "JE",   .args = 1, .function = *ProcessorJe},
    {.index = jne_index,     .name = "JNE",  .args = 1, .function = *ProcessorJne},
    {.index = ja_index,      .name = "JA",   .args = 1, .function = *ProcessorJa},
    {.index = jae_index,     .name = "JAE",  .args = 1, .function = *ProcessorJae},
    {.index = jb_index,      .name = "JB",   .args = 1, .function = *ProcessorJb},
    {.index = jbe_index,     .name = "JBE",  .args = 1, .function = *ProcessorJbe},

    {.index = in_index,      .name = "IN",   .args = 0, .function = *ProcessorIn},
    {.index = call_index,    .name = "CALL", .args = 1, .function = *ProcessorCall},
    {.index = ret_index,     .name = "RET",  .args = 0, .function = *ProcessorRet},

    {.index = notcmd_index, .name = "NOTC",  .args = 0, .function = *ProcessorNotComm},
};
const size_t number_of_commands_ = sizeof(commands_) / sizeof(commands_[0]);

const size_t number_of_registers = 10;
const size_t number_of_RAM = 200;

const size_t number_of_elem_on_line = 16;//How much elem of bytecode show PrintProcessor


#endif