#include "../Headers/processor_func.h"

// include-what-you-use -- linux

void RunProcessor(ByteCodeStruct byteCodeStruct)
{
    ProcessorStruct *proc = ProcessorStructInit(byteCodeStruct);
    while(proc->ip < byteCodeStruct.size)
    {
        COMMAND_INDEX index = (COMMAND_INDEX)proc->byteCodeStruct.byteCode[proc->ip];
        View indexP;
        indexP.value = index;
        printf(CYAN "%s\n\n" RESET, commands_[indexP.bits.b0].name);
        PrintProcessor(proc);
        commands_[indexP.bits.b0].function(proc);
    }
}

void PrintProcessor(ProcessorStruct *proc)
{
    PrintBytecode(proc);
    PrintRegisters(proc);
    PrintReturns(proc);
    PrintRAM(proc);
}

void PrintBytecode(ProcessorStruct *proc)
{
    size_t size = proc->byteCodeStruct.size;
    size_t ip = proc->ip;
    size_t n = number_of_elem_on_line;
    for(size_t i = 0; i < size; i++)
    {

        if((i + 1) % n == 1 || (i + 1) == 0)
            printf("[");

        if(i == ip)
            printf(RED);
        printf("%02X" RESET, proc->byteCodeStruct.byteCode[i]);

        if (((i + 1) % n == 0 && (i + 1) > 0) || i == size - 1)
        {
            printf("]\n");
            size_t delta = ip - (n * (i / n));

            if(delta < n)
            {
                for(size_t j = 0; j < delta * 4 + 1; j++)
                    printf(" ");
                printf(RED BOLD "^" RESET);
            }
            printf("\n");
        }

        else if (i < size - 1)
            printf(", ");
    }
}

void PrintRegisters(ProcessorStruct *proc)
{
    printf("\n\n--------------------REGISTERS--------------------\n[");
    for(size_t i = 0; i < number_of_registers; i++)
    {
        printf(MAGENTA "%c%c: " RESET, 'A' + (char) i, 'X');
        printf("%" PRINTF_T, proc->registers[i]);
        if(i < number_of_registers - 1)
            printf(", ");
    }
}

void PrintReturns(ProcessorStruct *proc)
{
    printf("]\n\n---------------------RETURNS---------------------\n[");
    for(int i = 0; i < proc->stkRet->size; i++)
    {
        printf("%" PRINTF_T, proc->stkRet->data[i]);
        if(i < proc->stkRet->size - 1)
            printf(", ");
    }
    printf("]\n\n");
}

void PrintRAM(ProcessorStruct *proc)
{
    printf("\n\n-----------------------RAM-----------------------\n");
    size_t n = number_of_elem_on_line;
    size_t size = number_of_RAM;
    for(size_t i = 0; i < size; i++)
    {
        if((i + 1) % n == 1 || (i + 1) == 0)
            printf("[");
        printf("%02X", (unsigned int)proc->RAM[i]);

        if(i == size - 1)
            printf("]");

        else if((i + 1) % n == 0 && (i + 1) > 0)
            printf("]\n\n");
    }
    printf("\n\n");
}

ProcessorStruct *ProcessorStructInit(ByteCodeStruct byteCodeStruct)
{
    ProcessorStruct *proc = (ProcessorStruct *)safe_calloc(sizeof(ProcessorStruct), 1);
    proc->byteCodeStruct = byteCodeStruct;
    ProcessorRegistersInit(proc);
    ProcessorRAMInit(proc);

    proc->stk = (Stack_t *)safe_calloc(sizeof(Stack_t), 1);
    StackInit(proc->stk); //TODO: error handle \ add to struct proc

    proc->stkRet = (Stack_t *)safe_calloc(sizeof(Stack_t), 1);
    StackInit(proc->stkRet);

    proc->ip = 0;
    return proc;
}

void ProcessorRegistersInit(ProcessorStruct *proc)
{
    proc->registers = (StackElem_t *)safe_calloc(sizeof(StackElem_t), number_of_registers);
    for(size_t i = 0; i < number_of_registers; i++)
        proc->registers[i] = POISON_V;
}

void ProcessorRAMInit(ProcessorStruct *proc)
{
    proc->RAM = (StackElem_t *)safe_calloc(sizeof(StackElem_t), number_of_RAM);
    for(size_t i = 0; i < number_of_RAM; i++)
            proc->RAM[i] = POISON_V;
}

enum ERRORS_PROCESSOR ProcessorHlt(ProcessorStruct *proc)
{
    proc->ip = proc->byteCodeStruct.size;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorNotComm(ProcessorStruct *proc)
{
    proc->ip++;
    printf("Its not a command\n");
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorPush(ProcessorStruct *proc)
{
    View indexP;
    indexP.value = proc->byteCodeStruct.byteCode[proc->ip++];
    StackElem_t res = 0;

    if(indexP.bits.b2)
    {
        res += proc->registers[proc->byteCodeStruct.byteCode[proc->ip++]];
    }

    if(indexP.bits.b1)
    {
        StackElem_t elemToPush = *(StackElem_t *)MakeElemFromByteCode(proc, sizeof(StackElem_t));
        res += elemToPush;
    }

    if(indexP.bits.b3)
    {
        res = proc->RAM[(size_t)res];
    }

    StackPush(proc->stk, res);
    return procNoErr;
}

void *MakeElemFromByteCode(ProcessorStruct *proc, size_t size)
{
    void *elem = safe_calloc(size, 1);
    for(size_t i = 0; i < size; i++)
        *((unsigned char *)elem + i) = proc->byteCodeStruct.byteCode[proc->ip++];
    return elem;
}


enum ERRORS_PROCESSOR ProcessorOut(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    printf(MAGENTA "Outed from stack <%" PRINTF_T ">\n" RESET, elemThatPoped);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorPopr(ProcessorStruct *proc)
{
    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);

    int indexInRegisterArr = proc->byteCodeStruct.byteCode[proc->ip++];
    proc->registers[indexInRegisterArr] = elemThatPoped;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorAdd(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for adding\n\n" RESET);
        return procCantAdd;
    }

    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem2);
    StackPop(proc->stk, &elem1);
    StackElem_t sum = elem1 + elem2;
    StackPush(proc->stk, sum);
    return procNoErr;
}



enum ERRORS_PROCESSOR ProcessorSub(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for subtraction\n\n" RESET);
        return procCantSub;
    }

    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem2);
    StackPop(proc->stk, &elem1);
    StackElem_t sub = elem1 - elem2;

    StackPush(proc->stk, sub);
    return procNoErr;
}


enum ERRORS_PROCESSOR ProcessorDiv(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for division\n\n" RESET);
        return procCantDiv;
    }

    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem2);
    StackPop(proc->stk, &elem1);
    if(IsEqual(elem2, 0))
    {
        printf(RED "The denominator is 0\n" RESET);
        return procCantDiv;
    }

    StackElem_t div = elem1 / elem2;

    StackPush(proc->stk, div);
    return procNoErr;
}



enum ERRORS_PROCESSOR ProcessorMul(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for multiply\n\n" RESET);
        return procCantMul;
    }

    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem2);
    StackPop(proc->stk, &elem1);
    StackElem_t mul = elem1 * elem2;

    StackPush(proc->stk, mul);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorSqrt(ProcessorStruct *proc)
{
    proc->ip++;
    int size = proc->stk->size;
    if(size < 1)
    {
        printf(RED "Too few elements for sqrt\n\n" RESET);
        return procCantMul;
    }
    StackElem_t elem1 = 0;
    StackPop(proc->stk, &elem1);
    if(elem1 < 0)
    {
        printf(RED "The number is negative(for sqrt)\n" RESET);
        return procCantSqrt;
    }

    StackElem_t sqrt_value = sqrt(elem1);
    StackPush(proc->stk, sqrt_value);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJmp(ProcessorStruct *proc)
{
    proc->ip++;
    size_t indexToGo = *(size_t *)MakeElemFromByteCode(proc, sizeof(size_t));
    proc->ip = indexToGo;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJe(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for make compare\n\n" RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantJmp;
    }
    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);
    
    bool equ = IsEqual(elem1, elem2);
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip += 1 + sizeof(size_t);
    return procNoErr;
}


enum ERRORS_PROCESSOR ProcessorJne(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for make compare\n\n" RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantJmp;
    }
    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = !IsEqual(elem1, elem2);
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip += 1 + sizeof(size_t);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJa(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for make compare\n\n" RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantJmp;
    }
    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 > elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip += 1 + sizeof(size_t);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJae(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for make compare\n\n" RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantJmp;
    }
    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 >= elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip += 1 + sizeof(size_t);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJb(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for make compare\n\n" RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantJmp;
    }
    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 < elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip += 1 + sizeof(size_t);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJbe(ProcessorStruct *proc)
{
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(RED "Too few elements for make compare\n\n" RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantJmp;
    }
    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem1);
    StackPop(proc->stk, &elem2);

    bool equ = elem2 <= elem1;
    if(equ)
        ProcessorJmp(proc);
    else
        proc->ip += 1 + sizeof(size_t);
    return procNoErr;
}


enum ERRORS_PROCESSOR ProcessorIn(ProcessorStruct *proc)
{
    proc->ip++;

    printf("Enter number that push in stack: ");
    StackElem_t inArg = 0;
    int nInput = scanf("%" PRINTF_T, &inArg);
    printf("\n");
    if(nInput <= 0)
    {
        printf("Entered not a number\n\n" RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantIn;
    }
    StackPush(proc->stk, inArg);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorCall(ProcessorStruct *proc)
{
    proc->ip++;
    size_t indexToGo = *(size_t *)MakeElemFromByteCode(proc, sizeof(size_t));
    size_t indexToRet = proc->ip;
    StackPush(proc->stkRet, (StackElem_t)indexToRet);
    proc->ip = indexToGo;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorRet(ProcessorStruct *proc)
{
    StackElem_t elemThatPoped = 0;
    StackPop(proc->stkRet, &elemThatPoped);
    proc->ip = (size_t)elemThatPoped;
    return procNoErr;
}