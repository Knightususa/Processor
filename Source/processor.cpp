#include "../Headers/processor.h"

// include-what-you-use -- linux
main(int argc, char **argv)
{
    for(size_t i = 0; i < number_of_commands_; i++)
        commands_processor[i].cmd = commands_global_[i];

    OptionsProc Options;
    Options.filename = NULL;
    GetOptionsProc(argc, argv, &Options);   
    DelNameExt(&Options);
    char *filenamebin = CreateName(Options.filename, "bin");


    ByteCodeStruct byteCodeStruct = ReadFileBytecode(filenamebin);
    ProcessorStruct *proc = ProcessorStructInit(byteCodeStruct);
    while(proc->ip < byteCodeStruct.size)
    {
        COMMAND_INDEX index = (COMMAND_INDEX)proc->byteCodeStruct.byteCode[proc->ip];
        View indexP;
        indexP.value = index;
        // StackDump(proc->stk);
        
        ON_PROC_DEBUG
        (
        printf(CYAN "%s\n\n" RESET, commands_processor[indexP.bits.b0].cmd.name);
        PrintProcessor(proc);
        )

        commands_processor[indexP.bits.b0].function(proc);
    }
}

void GetOptionsProc(int argc, char **argv, OptionsProc *pOptions)
{
    yaissert(argv != NULL);

    int opt = 0;
    while ((opt = getopt(argc, argv, "i:")) != -1)
    {
        switch (opt)
        {       
            case 'i':
                pOptions -> filename = optarg;
                break;
            
            default:
                warning(0, "Wrong flag");
                break;
        }
    }
    if(pOptions->filename == NULL)
        printf(RED "Enter flag -i and filename\n" RESET);
}

void DelNameExt(OptionsProc *pOptions)
{   
    yaissert(pOptions != NULL);
    yaissert(pOptions->filename != NULL);

    char *f = pOptions->filename;
    int lenf = (int)strlen(f);
    int i = lenf - 1;

    while(i > 0 && f[i] != '.')
        i--;

    if(i <= 0)
        return;

    for(int j = 0; i + j < lenf; j++)
        (pOptions->filename)[i + j] = '\0';
}

char *CreateName(char *filename, const char *ext)
{
    yaissert(filename != NULL);

    char *nameOut = (char *)safe_calloc(strlen(filename) + strlen("./Code_/.") + 2 * strlen(ext), sizeof(char));
    strcpy(nameOut, "./Code_");
    strcat(nameOut, ext);
    strcat(nameOut, "/");
    strcat(nameOut, filename);
    strcat(nameOut, ".");
    strcat(nameOut, ext);
    return nameOut;
}

ByteCodeStruct ReadFileBytecode(char *fileName)
{
    yaissert(fileName != NULL);
    
    ByteCodeStruct byteCodeStruct;
    char mode[] = "rb";
    FILE *pFile = safe_fopen(fileName, mode);

    struct stat fileStat = {};
    fstat(fileno(pFile), &fileStat);
    size_t fileSize = fileStat.st_size;
    
    // printf("Size of file = " RED "%lli bytes\n\n\n" RESET, fileSize);
    byteCodeStruct.size = fileSize;
    byteCodeStruct.byteCode = (unsigned char *)safe_calloc((size_t)fileSize + 1, sizeof(unsigned char));
    
    size_t nReadBytes = safe_fread((void *) byteCodeStruct.byteCode, sizeof(unsigned char), fileSize, pFile);
    warning (nReadBytes == fileSize, "nReadBytes not equals fileSize");
    yaissert(nReadBytes == fileSize);

    fclose(pFile);
    return byteCodeStruct;
}


void PrintProcessor(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    PrintBytecode(proc);
    PrintRegisters(proc);
    PrintReturns(proc);
    PrintRAM(proc);
}

void PrintBytecode(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    size_t size = proc->byteCodeStruct.size;
    size_t ip = proc->ip;
    size_t n = number_of_elem_on_line;
    for(size_t i = 0; i < size; i++)
    {
        if (i == 0)
            printf(MAGENTA " %02X\n" RESET, (unsigned int)i);
        if((i + 1) % n == 1 || (i + 1) == 0)
            printf("[");

        if(i == ip)
            printf(RED);
        printf("%02X" RESET, proc->byteCodeStruct.byteCode[i]);

        if ((i + 1) % n == 0 || i == size - 1)
        {
            printf("]\n");
            size_t delta = ip - (n * (i / n));

            if(delta < n)
            {
                for(size_t j = 0; j < delta * 3 + 1 + (delta / 4) * 2; j++)
                    printf(" ");
                printf(RED BOLD "^" RESET);
            }
            printf("\n");
            if(i != size - 1)
                printf(MAGENTA " %02X\n" RESET, (unsigned int)i);
        }

        else if (i < size - 1)
            printf(" ");
        if((i + 1) % 4 == 0 && !((i + 1) % 16 == 0))
            printf(MAGENTA "| " RESET);
    }
}

void PrintRegisters(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    printf("\n\n--------------------REGISTERS--------------------\n[");
    for(size_t i = 0; i < size_registers; i++)
    {
        printf(MAGENTA "%c%c: " RESET, (char)('a' + i), 'x');
        printf("%" PRINTF_T, proc->registers[i]);
        if(i < size_registers - 1)
            printf(", ");
    }
}

void PrintReturns(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

    size_t n = number_of_elem_on_display;
    size_t size1 = size_VRAM;
    size_t size2 = size_RAM;
    printf("\n\n-----------------------RAM-----------------------\n");
    for(size_t i = 0; i < size1; i++)
    {
        if((i + 1) % n == 1 || (i + 1) == 0)
            printf("[");

        printf("%02X", (unsigned int)proc->RAM[i]);

        if (((i + 1) % n == 0 && (i + 1) > 0) || i == size1 - 1)
            printf("]\n\n");

        else if (i < size1 - 1)
            printf(", ");
    }

    printf("\n\n\n\n-----------------------VRAM-----------------------\n");
    for(size_t i = size1; i < size2; i++)
    {
        if((i + 1) % n == 1 || (i + 1) == 0)
            printf("[");

        if(isnan(proc->RAM[i]))
            printf("*");

        if(proc->RAM[i] >= 1)
            printf("0");

        if (((i + 1) % n == 0 && (i + 1) > 0) || i == size2 - 1)
            printf("]\n\n");

        else if (i < size2 - 1)
            printf("  ");
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
    yaissert(proc != NULL);

    proc->registers = (StackElem_t *)safe_calloc(sizeof(StackElem_t), size_registers);
    for(size_t i = 0; i < size_registers; i++)
        proc->registers[i] = POISON_V;
}

void ProcessorRAMInit(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    proc->RAM = (StackElem_t *)safe_calloc(sizeof(StackElem_t), size_RAM);
    for(size_t i = 0; i < size_RAM; i++)
            proc->RAM[i] = POISON_V;
}

enum ERRORS_PROCESSOR ProcessorHlt(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    proc->ip = proc->byteCodeStruct.size;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorNotComm(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    proc->ip++;
    printf("Its not a command\n");
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorPush(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    View indexP;
    indexP.value = proc->byteCodeStruct.byteCode[proc->ip++];
    StackElem_t res = 0;

    if(indexP.bits.b2)
        res += proc->registers[proc->byteCodeStruct.byteCode[proc->ip++]];

    if(indexP.bits.b1)
    {
        StackElem_t elemToPush = *(StackElem_t *)MakeElemFromByteCode(proc, sizeof(StackElem_t));
        res += elemToPush;
    }

    if(indexP.bits.b3)
        res = proc->RAM[(size_t)res];

    StackPush(proc->stk, res);
    return procNoErr;
}

void *MakeElemFromByteCode(ProcessorStruct *proc, size_t size)
{
    yaissert(proc != NULL);

    void *elem = safe_calloc(size, 1);
    for(size_t i = 0; i < size; i++)
        *((unsigned char *)elem + i) = proc->byteCodeStruct.byteCode[proc->ip++];
    return elem;
}


enum ERRORS_PROCESSOR ProcessorOut(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    printf(MAGENTA "Outed from stack <%" PRINTF_T ">\n" RESET, elemThatPoped);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorPop(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

    View indexP;
    indexP.value = proc->byteCodeStruct.byteCode[proc->ip++];
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    
    StackElem_t res = 0;
    size_t indexReg = 0;
    if(indexP.bits.b2)
    {
        indexReg = proc->byteCodeStruct.byteCode[proc->ip++];
        res += proc->registers[indexReg];
    }

    if(indexP.bits.b1)
    {
        StackElem_t elemToPush = *(StackElem_t *)MakeElemFromByteCode(proc, sizeof(StackElem_t));
        res += elemToPush;
    }

    if(indexP.bits.b3)
        proc->RAM[(size_t)res] = elemThatPoped;
    else
        proc->registers[indexReg] = elemThatPoped;

    // printf("res = %lg\n",res);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorAdd(ProcessorStruct *proc)
{
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

    proc->ip++;
    size_t indexToGo = *(size_t *)MakeElemFromByteCode(proc, sizeof(size_t));
    proc->ip = indexToGo;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJe(ProcessorStruct *proc)
{
    yaissert(proc != NULL);
    
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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);

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
    yaissert(proc != NULL);
    
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
    yaissert(proc != NULL);
    
    proc->ip++;
    size_t indexToGo = *(size_t *)MakeElemFromByteCode(proc, sizeof(size_t));
    size_t indexToRet = proc->ip;
    StackPush(proc->stkRet, (StackElem_t)indexToRet);
    proc->ip = indexToGo;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorRet(ProcessorStruct *proc)
{
    yaissert(proc != NULL);
    
    StackElem_t elemThatPoped = 0;
    StackPop(proc->stkRet, &elemThatPoped);
    proc->ip = (size_t)elemThatPoped;
    return procNoErr;
}