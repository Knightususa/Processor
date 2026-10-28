#include "../Headers/processor_func.h"


CommandProcessor commands_processor[] = 
{
    MakeCommandProcessor(Hlt),

    MakeCommandProcessor(Push),
    MakeCommandProcessor(Out),

    MakeCommandProcessor(Pop),

    MakeCommandProcessor(Add),
    MakeCommandProcessor(Sub),
    MakeCommandProcessor(Div),
    MakeCommandProcessor(Fmod),
    MakeCommandProcessor(Mul),
    MakeCommandProcessor(Sqrt),
    MakeCommandProcessor(Pow),

    MakeCommandProcessor(Jmp),
    MakeCommandProcessor(Je),
    MakeCommandProcessor(Jne),
    MakeCommandProcessor(Ja),
    MakeCommandProcessor(Jae),
    MakeCommandProcessor(Jb),
    MakeCommandProcessor(Jbe),

    MakeCommandProcessor(In),
    MakeCommandProcessor(Call),
    MakeCommandProcessor(Ret),

    MakeCommandProcessor(Outc),
    MakeCommandProcessor(Draw),
    MakeCommandProcessor(Delay),
    MakeCommandProcessor(InitW),
    MakeCommandProcessor(CloseW),
    MakeCommandProcessor(Rand),

    MakeCommandProcessor(NotCmd),
};
std::mt19937 gen(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
std::uniform_int_distribution<int> dist(0, INT32_MAX);

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
        printf(_RED "Enter flag -i and filename\n" _RESET);
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
    
    // printf("Size of file = " _RED "%lli bytes\n\n\n" _RESET, fileSize);
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
    CheckProc(proc);

    // PrintBytecode(proc);
    // PrintRegisters(proc);
    // PrintReturns(proc);
    // PrintRAM(proc);
    PrintVRAM(proc);
}

void PrintBytecode(ProcessorStruct *proc)
{
    CheckProc(proc);

    size_t size = proc->byteCodeStruct.size;
    size_t ip = proc->ip;
    size_t n = number_of_elem_on_line;
    for(size_t i = 0; i < size; i++)
    {
        if (i == 0)
            printf(_MAGENTA " %02X\n" _RESET, (unsigned int)i);
        if((i + 1) % n == 1 || (i + 1) == 0)
            printf("[");

        if(i == ip)
            printf(_RED);
        printf("%02X" _RESET, proc->byteCodeStruct.byteCode[i]);

        if ((i + 1) % n == 0 || i == size - 1)
        {
            printf("]\n");
            size_t delta = ip - (n * (i / n));

            if(delta < n)
            {
                for(size_t j = 0; j < delta * 3 + 1 + (delta / 4) * 2; j++)
                    printf(" ");
                printf(_RED _BOLD "^" _RESET);
            }
            printf("\n");
            if(i != size - 1)
                printf(_MAGENTA " %02X\n" _RESET, (unsigned int)i);
        }

        else if (i < size - 1)
            printf(" ");
        if((i + 1) % 4 == 0 && !((i + 1) % 16 == 0))
            printf(_MAGENTA "| " _RESET);
    }
}

void PrintRegisters(ProcessorStruct *proc)
{
    CheckProc(proc);

    printf("\n\n--------------------REGISTERS--------------------\n[");
    for(size_t i = 0; i < size_registers; i++)
    {
        printf(_MAGENTA "%c%c: " _RESET, (char)('a' + i), 'x');
        printf("%" PRINTF_T, proc->registers[i]);
        if(i < size_registers - 1)
            printf(", ");
    }
}

void PrintReturns(ProcessorStruct *proc)
{
    CheckProc(proc);

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
    CheckProc(proc);

    size_t n = number_of_elem_on_display;
    size_t size1 = delta_size_RAM;
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

    printf("\n\n\n\n");
}

void PrintVRAM(ProcessorStruct *proc)
{
    size_t n = number_of_elem_on_display;
    size_t size1 = delta_size_RAM;
    size_t size2 = delta_size_RAM + 100;
    printf("-----------------------VRAM-----------------------\n");
    for(size_t i = size1; i < size2; i++)
    {
        if((i + 1) % n == 1 || (i + 1) == 0)
            printf("[");

        // if(isnan(proc->RAM[i]))
        //     printf("*");

        // if(proc->RAM[i] >= 1)
        //     printf("0");

        printf("%02X", (unsigned int)proc->RAM[i]);

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
    StackInit(proc->stk);

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
    for(size_t i = 0; i < delta_size_RAM; i++)
            proc->RAM[i] = POISON_V;
    for(size_t i = delta_size_RAM; i < size_RAM; i++)
            proc->RAM[i] = 0; 

}

enum ERRORS_PROCESSOR ProcessorHlt(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip = proc->byteCodeStruct.size;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorNotCmd(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip++;
    printf("Its not a command\n");
    PrintBytecode(proc);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorPush(ProcessorStruct *proc)
{
    CheckProc(proc);

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
    CheckProc(proc);

    void *elem = safe_calloc(size, 1);
    for(size_t i = 0; i < size; i++)
        *((unsigned char *)elem + i) = proc->byteCodeStruct.byteCode[proc->ip++];
    return elem;
}


enum ERRORS_PROCESSOR ProcessorOut(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    printf(_MAGENTA "Outed from stack <%" PRINTF_T ">\n" _RESET, elemThatPoped);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorOutc(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip++;
    StackElem_t elemThatPoped;
    StackPop(proc->stk, &elemThatPoped);
    printf(_MAGENTA "%c" _RESET, (char)elemThatPoped);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorDraw(ProcessorStruct *proc)
{
    CheckProc(proc);
    proc->ip++;
    Color *picture = (Color *)safe_calloc(size_VRAM, sizeof(Color));
    for(size_t y = 0; y < size_VRAM_y; y++)
    {
        for(size_t x = 0; x < size_VRAM_x; x++)
        {
            size_t index = y * size_VRAM_x + x + delta_size_RAM;
            unsigned char value;
            value = (unsigned char)proc->RAM[index];
            Color pixel;
            // pixel = {.r = value.bits.r, .g = value.bits.g, .b = value.bits.b, .a = value.bits.a};
            pixel = {.r = 0, .g = 0, .b = 0, .a = value};
            picture[y * size_VRAM_x + x] = pixel;
        }
    }

    drawPicture(picture);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorDelay(ProcessorStruct *proc)
{
    CheckProc(proc);
    
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

    WaitTime(res);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorInitW(ProcessorStruct *proc)
{
    CheckProc(proc);
    proc->ip++;
    InitWindow(1200, 1200, "Picture");
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorCloseW(ProcessorStruct *proc)
{
    CheckProc(proc);
    proc->ip++;
    CloseWindow();
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorRand(ProcessorStruct *proc)
{
    CheckProc(proc);
    proc->ip++;

    StackElem_t maxRandom = *(StackElem_t *)MakeElemFromByteCode(proc, sizeof(StackElem_t));
    int randomValue = dist(gen);
    StackElem_t random = randomValue % (int)maxRandom;
    StackPush(proc->stk, random);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorPop(ProcessorStruct *proc)
{
    CheckProc(proc);

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
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorAdd(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for adding\n\n" _RESET);
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
    CheckProc(proc);

    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for subtraction\n\n" _RESET);
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
    CheckProc(proc);

    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for division\n\n" _RESET);
        return procCantDiv;
    }

    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem2);
    StackPop(proc->stk, &elem1);
    if(IsEqual(elem2, 0))
    {
        printf(_RED "The denominator is 0\n" _RESET);
        return procCantDiv;
    }

    StackElem_t div = elem1 / elem2;

    StackPush(proc->stk, div);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorFmod(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for fmod\n\n" _RESET);
        return procCantDiv;
    }

    StackElem_t elem1 = 0, elem2 = 0;
    StackPop(proc->stk, &elem2);
    StackPop(proc->stk, &elem1);
    StackElem_t fmod = (int)elem1 % (int)elem2;

    StackPush(proc->stk, fmod);
    return procNoErr;
}



enum ERRORS_PROCESSOR ProcessorMul(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip++;
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for multiply\n\n" _RESET);
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
    CheckProc(proc);

    proc->ip++;
    int size = proc->stk->size;
    if(size < 1)
    {
        printf(_RED "Too few elements for sqrt\n\n" _RESET);
        return procCantMul;
    }
    StackElem_t elem1 = 0;
    StackPop(proc->stk, &elem1);
    if(elem1 < 0)
    {
        printf(_RED "The number is negative(for sqrt)\n" _RESET);
        return procCantSqrt;
    }

    StackElem_t sqrt_value = sqrt(elem1);
    StackPush(proc->stk, sqrt_value);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorPow(ProcessorStruct *proc)
{
    CheckProc(proc);
    
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

    int size = proc->stk->size;
    if(size < 1)
    {
        printf(_RED "Too few elements for pow\n\n" _RESET);
        return procCantMul;
    }
    StackElem_t elem1 = 0;
    StackPop(proc->stk, &elem1);

    StackElem_t pow_value = pow(elem1, res);
    StackPush(proc->stk, pow_value);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJmp(ProcessorStruct *proc)
{
    CheckProc(proc);

    proc->ip++;
    size_t indexToGo = *(size_t *)MakeElemFromByteCode(proc, sizeof(size_t));
    proc->ip = indexToGo;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorJe(ProcessorStruct *proc)
{
    CheckProc(proc);
    
    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for make compare\n\n" _RESET);
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
    CheckProc(proc);

    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for make compare\n\n" _RESET);
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
    CheckProc(proc);

    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for make compare\n\n" _RESET);
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
    CheckProc(proc);

    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for make compare\n\n" _RESET);
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
    CheckProc(proc);

    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for make compare\n\n" _RESET);
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
    CheckProc(proc);

    int size = proc->stk->size;
    if(size < 2)
    {
        printf(_RED "Too few elements for make compare\n\n" _RESET);
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
    CheckProc(proc);
    
    proc->ip++;

    printf("Enter number that push in stack: ");
    StackElem_t inArg = 0;
    int nInput = scanf("%" PRINTF_T, &inArg);
    printf("\n");
    if(nInput <= 0)
    {
        printf("Entered not a number\n\n" _RESET);
        proc->ip += 1 + sizeof(size_t);
        return procCantIn;
    }
    StackPush(proc->stk, inArg);
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorCall(ProcessorStruct *proc)
{
    CheckProc(proc);
    
    proc->ip++;
    size_t indexToGo = *(size_t *)MakeElemFromByteCode(proc, sizeof(size_t));
    size_t indexToRet = proc->ip;
    StackPush(proc->stkRet, (StackElem_t)indexToRet);
    proc->ip = indexToGo;
    return procNoErr;
}

enum ERRORS_PROCESSOR ProcessorRet(ProcessorStruct *proc)
{
    CheckProc(proc);
    
    StackElem_t elemThatPoped = 0;
    StackPop(proc->stkRet, &elemThatPoped);
    proc->ip = (size_t)elemThatPoped;
    return procNoErr;
}

void CheckProc(ProcessorStruct *proc)
{
    yaissert(proc != NULL);
    yaissert(proc->byteCodeStruct.byteCode != NULL);
    yaissert(proc->RAM != NULL);
    yaissert(proc->registers != NULL);
    yaissert(proc->stk != NULL);
    yaissert(proc->stkRet != NULL);
}