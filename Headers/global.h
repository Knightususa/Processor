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

#ifdef STACK_T_DOUBLE
typedef double StackElem_t;
#define POISON_V NAN
#define StackDump StackDumpCID
#define InputPushValue InputPushValueCID
#define IsValuePoison IsValuePoisonD
#define PRINTF_T "lg"
#define TEXT_T_VALUE "double"

#elif  defined(STACK_T_CHAR)
typedef char StackElem_t;
#define POISON_V '~'
#define StackDump StackDumpCID
#define InputPushValue InputPushValueCID
#define IsValuePoison IsValuePoisonCI
#define PRINTF_T "c"
#define TEXT_T_VALUE "char"

#elif  defined(STACK_T_STRUCT)
typedef Struction StackElem_t;
#define POISON_V {-1488526967, '~', NAN}
#define StackDump StackDumpS
#define InputPushValue InputPushValueS
#define IsValuePoison IsValuePoisonS

#else
typedef int StackElem_t;
#define POISON_V -1488526967
#define StackDump StackDumpCID
#define InputPushValue InputPushValueCID
#define IsValuePoison IsValuePoisonCI
#define PRINTF_T "i"
#define TEXT_T_VALUE "int"
#endif
//end STACK_T_DOUBLE or STACK_T_CHAR or STACK_T_STRUCT or else(INT)


#ifdef ON_HASH_PROTECTION
#define HASH_PROTECTION(...) __VA_ARGS__
#else
#define HASH_PROTECTION(...)
#endif


#ifdef ON_CANARY_PROTECTION
#define CANARY_PROTECTION(...) __VA_ARGS__
#else
#define CANARY_PROTECTION(...)
#endif



typedef struct
{
    CANARY_PROTECTION(unsigned long long arbusL; //LEFT CANARY
                      unsigned long long arbusDefault;)//DEFAULT VALUE OF CANARY
    int size;
    int capacity;
    StackElem_t *data;
    StackElem_t poison_v;
    CANARY_PROTECTION(unsigned long long *canaryL;
                      unsigned long long *canaryR;
                      unsigned long long canaryDefault;)
    HASH_PROTECTION(unsigned long long hashData;
        unsigned long long hashStack;)
    CANARY_PROTECTION(unsigned long long arbusR;) //RIGHT CANARY
}Stack_t;

enum ERRORS
{
    noErr,
    stackNoInit,
    stackOverflow,
    stackUnderflow,
    stackNoResize,
    dataPointerNull,
    cantAdd,//add error assembler
    candSub,
    cantDiv,
    cantMul
    CANARY_PROTECTION(,
                      leftCanaryDataKilled,
                      rightCanaryDataKilled,
                      leftCanaryStackKilled,
                      rightCanaryStackKilled)
    HASH_PROTECTION(,
                    hashDataIncorrect,
                    hashStackIncorrect)
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
    hlt_index  = 0,

    push_index = 1,
    pop_index  = 2,

    pusr_index = 3,
    popr_index = 4,

    add_index  = 5,
    sub_index  = 6,
    div_index  = 7,
    mul_index  = 8,

    jmp_index  = 9,
    je_index   = 10,
    ja_index   = 11,
    jae_index  = 12,
    jb_index   = 13,
    jbe_index  = 14,

    not_command = 15
};

typedef struct 
{
    const char *name;
    COMMAND_INDEX index;
    int argument;//TODO: command ardument 
}command;

#endif
//end #ifndef GLOBAL_H