#include "../Headers/processor_func.h"

// include-what-you-use -- linux
// cmd arr
//  {name "push", enum PUSH, args=1, pointerToFunc}
//  {name "pop",  enum POP,  args=0}
// TODO
const COMMAND_INDEX commands_[] = 
{
    hlt_index,
    push_index, out_index, 
    pusr_index, popr_index,
    add_index, sub_index, div_index, mul_index, sqrt_index,
    jmp_index, je_index, ja_index, jae_index, jb_index, jbe_index,
    notcomm_index
};

const size_t number_of_commands_ = sizeof(commands_) / sizeof(commands_[0]);

const COMMAND_INDEX arg_commands_[] = 
{
    push_index, out_index, 
    jmp_index, je_index, ja_index, jae_index, jb_index, jbe_index
};

const size_t number_of_arg_commands_ = sizeof(arg_commands_) / sizeof(arg_commands_[0]);

const int number_of_registers = 10;

void RunProcessor(ByteCodeStruct byteCodeStruct, Stack_t *stk)
{
    ProcessorStruct *proc = ProcessorStructInit(byteCodeStruct, stk);
    while(proc->ip < byteCodeStruct.size)
    {
        COMMAND_INDEX index = (COMMAND_INDEX)proc->byteCodeStruct.byteCode[proc->ip];
        printf("%s\n", GetCommandName(index));
        switch(index)
        {
            case(hlt_index):
                proc->ip = byteCodeStruct.size;
                break;



            case(push_index):
                ProcessorPush(proc);
                break;

            case(out_index):
                ProcessorOut(proc);
                break;
                


            case(pusr_index):
                ProcessorPusr(proc);
                break;

            case(popr_index):
                ProcessorPopr(proc);
                break;



            case (add_index):
                ProcessorAdd(proc);
                break;
                
            case (sub_index):
                ProcessorSub(proc);
                break;
                
            case (div_index):
                ProcessorDiv(proc);
                break;
                
            case (mul_index):
                ProcessorMul(proc);
                break;

            case (sqrt_index):
                ProcessorSqrt(proc);
                break;



            case(jmp_index):
                ProcessorJump(proc);
                break;

            case(je_index):
            case(ja_index):
            case(jae_index):
            case(jb_index):
            case(jbe_index):
                ProcessorJumpIf(proc);
                break;

            case(notcomm_index):
            default:
                proc->ip++;
                break;
        }
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

enum ERRORS ProcessorPush(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemToPush = MakeElemFromByteCode(proc);
    StackPush(proc->stk, elemToPush);
    return noErr;
}

StackElem_t MakeElemFromByteCode(ProcessorStruct *proc)
{
    StackElem_t elem = 0;
    for(int i = 0; i < sizeof(StackElem_t); i++)
        *((unsigned char *)&elem + i) = proc->byteCodeStruct.byteCode[proc->ip++];
    return elem;
}


enum ERRORS ProcessorOut(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    printf("\n\n\nOuted from stack <%" PRINTF_T ">\n\n\n", elemThatPoped);
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

enum ERRORS ProcessorJump(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t indexToGo = MakeElemFromByteCode(proc);
    proc->ip = (size_t) indexToGo;
    return noErr;
}

enum ERRORS ProcessorJumpIf(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for make compare\n\n" RESET);
        proc->ip++;
        return cantJump;
    }
    COMMAND_INDEX index = (COMMAND_INDEX)proc->byteCodeStruct.byteCode[proc->ip];

    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = false;
    switch (index)
    {
        case(je_index):
            equ = elem2 == elem1;
            break;

        case(ja_index):
            equ = elem2 > elem1;
            break;

        case(jae_index):
            equ = elem2 >= elem1;
            break;

        case(jb_index):
            equ = elem2 < elem1;
            break;

        case(jbe_index):
            equ = elem2 <= elem1;
            break;

        default:
            break;
    }
    if(equ)
        ProcessorJump(proc);
    else
        proc->ip++;

    return noErr;
}