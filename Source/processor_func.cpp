#include "../Headers/processor_func.h"

// include-what-you-use -- linux
// cmd arr
//  {name "push", enum PUSH, args=1, pointerToFunc}
//  {name "pop",  enum POP,  args=0}
// TODO

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
    {.index = ja_index,      .name = "JA",   .args = 1, .function = *ProcessorJa},
    {.index = jae_index,     .name = "JAE",  .args = 1, .function = *ProcessorJae},
    {.index = jb_index,      .name = "JB",   .args = 1, .function = *ProcessorJb},
    {.index = jbe_index,     .name = "JBE",  .args = 1, .function = *ProcessorJbe},

    {.index = notcomm_index, .name = "NOTC",  .args = 0, .function = *ProcessorNotComm},
};
const size_t number_of_commands_ = sizeof(commands_) / sizeof(commands_[0]);

const int number_of_registers = 10;

void RunProcessor(ByteCodeStruct byteCodeStruct, Stack_t *stk)
{
    ProcessorStruct *proc = ProcessorStructInit(byteCodeStruct, stk);
    while(proc->ip < byteCodeStruct.size)
    {
        COMMAND_INDEX index = (COMMAND_INDEX)proc->byteCodeStruct.byteCode[proc->ip];
        commands_[index].function(proc);
        printf("%s\n", commands_[index].name);
        yaissert(StackDump(proc->stk));
    }
}

ProcessorStruct *ProcessorStructInit(ByteCodeStruct byteCodeStruct, Stack_t *stk)
{
    ProcessorStruct *proc = (ProcessorStruct *)safe_calloc(sizeof(ProcessorStruct), 1);
    proc->byteCodeStruct = byteCodeStruct;
    proc->stk = stk;
    proc->ip = 0;
    ProcessorRegistersInit(proc);
    return proc;
}

void ProcessorRegistersInit(ProcessorStruct *proc)
{
    proc->registers = (StackElem_t *)safe_calloc(sizeof(StackElem_t), number_of_registers);
    for(int i = 0; i < number_of_registers; i++)
        proc->registers[i] = proc->stk->poison_v;
}

enum ERRORS ProcessorHlt(ProcessorStruct *proc)
{
    proc->ip = proc->byteCodeStruct.size;
    return noErr;
}

enum ERRORS ProcessorNotComm(ProcessorStruct *proc)
{
    proc->ip++;
    return noErr;
}

enum ERRORS ProcessorPush(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemToPush = *(StackElem_t *)MakeElemFromByteCode(proc, sizeof(StackElem_t));
    StackPush(proc->stk, elemToPush);
    return noErr;
}

void *MakeElemFromByteCode(ProcessorStruct *proc, size_t size)
{
    void *elem = safe_calloc(size, 1);
    for(size_t i = 0; i < size; i++)
        *((unsigned char *)elem + i) = proc->byteCodeStruct.byteCode[proc->ip++];
    return elem;
}


enum ERRORS ProcessorOut(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    printf("Outed from stack <%" PRINTF_T ">\n", elemThatPoped);
    return noErr;
}


enum ERRORS ProcessorPusr(ProcessorStruct *proc)
{
    proc->ip++;
    int indexInRegisterArr = proc->byteCodeStruct.byteCode[proc->ip++];
    StackElem_t elemToPush = proc->registers[indexInRegisterArr];
    StackPush(proc->stk, elemToPush);
    return noErr;
}


enum ERRORS ProcessorPopr(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);

    int indexInRegisterArr = proc->byteCodeStruct.byteCode[proc->ip++];
    proc->registers[indexInRegisterArr] = elemThatPoped;
    return noErr;
}

enum ERRORS ProcessorAdd(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for adding\n\n" RESET);
        return cantAdd;
    }

    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);
    StackElem_t sum = elem1 + elem2;
    StackPush(proc->stk, sum);
    return noErr;
}



enum ERRORS ProcessorSub(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for subtraction\n\n" RESET);
        return cantSub;
    }

    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);
    StackElem_t sub = elem1 - elem2;

    StackPush(proc->stk, sub);
    return noErr;
}


enum ERRORS ProcessorDiv(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for division\n\n" RESET);
        return cantDiv;
    }

    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);
    StackElem_t div = elem1 / elem2;

    StackPush(proc->stk, div);
    return noErr;
}



enum ERRORS ProcessorMul(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for multiply\n\n" RESET);
        return cantMul;
    }

    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);
    StackElem_t mul = elem1 * elem2;

    StackPush(proc->stk, mul);
    return noErr;
}

enum ERRORS ProcessorSqrt(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 1)
    {
        printf("Too few elements for sqrt\n\n" RESET);
        return cantMul;
    }
    StackElem_t elem1;
    StackPop(proc->stk, &elem1);

    StackElem_t sqrt_value = sqrt(elem1);
    StackPush(proc->stk, sqrt_value);
    return noErr;
}

enum ERRORS ProcessorJmp(ProcessorStruct *proc)
{
    proc->ip++;
    size_t indexToGo = *(size_t *)MakeElemFromByteCode(proc, sizeof(size_t));
    proc->ip = indexToGo;
    return noErr;
}

enum ERRORS ProcessorJe(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for make compare\n\n" RESET);
        proc->ip++;
        return cantJump;
    }
    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = IsZero(elem1, elem2);
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip++;
    return noErr;
}

enum ERRORS ProcessorJa(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for make compare\n\n" RESET);
        proc->ip++;
        return cantJump;
    }
    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 > elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip++;
    return noErr;
}

enum ERRORS ProcessorJae(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for make compare\n\n" RESET);
        proc->ip++;
        return cantJump;
    }
    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 >= elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip++;
    return noErr;
}

enum ERRORS ProcessorJb(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for make compare\n\n" RESET);
        proc->ip++;
        return cantJump;
    }
    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 < elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip++;
    return noErr;
}

enum ERRORS ProcessorJbe(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for make compare\n\n" RESET);
        proc->ip++;
        return cantJump;
    }
    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 <= elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip++;
    return noErr;
}