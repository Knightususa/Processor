#include "../Headers/processor_func.h"

// include-what-you-use -- linux

const COMMAND_INDEX commands_[] = 
{
    hlt_index,
    push_index, out_index, 
    pusr_index, popr_index,
    add_index, sub_index, div_index, mul_index,
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

void RunProcessor(ByteCodeStruct byteCodeStruct, Stack_t *stk)
{
    ProcessorStruct procdata = {.byteCodeStruct = byteCodeStruct, .stk = stk, .ip = 0};
    ProcessorStruct *proc = &procdata;
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
                


            // case(pusr_index):
            //     ProcessorPusr(proc);
            //     break;

            // case(popr_index):
            //     ProcessorPopr(proc);
            //     break;



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



            case(jmp_index):
            case(je_index):
            case(ja_index):
            case(jae_index):
            case(jb_index):
            case(jbe_index):
                ProcessorJump(proc, index);
                break;

            case(notcomm_index):
            default:
                proc->ip++;
                break;
        }
        yaissert(StackDump(proc->stk));
    }
}

enum ERRORS ProcessorPush(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemToPush = MakeElemFromByteCode(proc);
    yaissert(StackDump(proc->stk));
    StackPush(proc->stk, elemToPush);
    return noErr;
}

StackElem_t MakeElemFromByteCode(ProcessorStruct *proc)
{
    StackElem_t elemToPush = (StackElem_t)(proc->byteCodeStruct.byteCode[proc->ip]);
    proc->ip += sizeof(StackElem_t);
    return elemToPush;
}


enum ERRORS ProcessorOut(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    printf("\n\n\nOuted from stack <%" PRINTF_T ">\n\n\n", elemThatPoped);
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

enum ERRORS ProcessorJump(ProcessorStruct *proc, COMMAND_INDEX index)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf("Too few elements for make compare\n\n" RESET);
        return cantMul;
    }

    StackElem_t elem1, elem2;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = false;
    switch (index)
    {
        case(jmp_index):
            equ = true;
            break;

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
    {
        StackElem_t indexToGo = MakeElemFromByteCode(proc);
        proc->ip = (size_t) indexToGo;
    }

    return noErr;
}