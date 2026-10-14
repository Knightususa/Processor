#ifndef GLOBAL_H
#define GLOBAL_H

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

const unsigned long long ArbusDefault = 0xDEDC0CA1;
const unsigned long long CanaryDefault = 0xDEDC0CA1DEDC0CA1;

const int CommandMaxLen = 4; 
const int ArgumentMaxLen = 32; 


typedef struct
{
    int int_v;
    char char_v;
    double double_v;
}Struction;

enum ERRORS_PROCESSOR
{
     //TODO: add error assembler
    procCantAdd,
    procCantSub,
    procCantDiv,
    procCantMul,
    procCantJump,
    procNoErr
};


enum OPTIONS
{
    initStack,
    destroyStack,
    pushStack,
    popStack
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

    notcomm_index = 17
};

#endif
//end #ifndef GLOBAL_H