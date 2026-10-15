#ifndef STACK_GLOBAL_H
#define STACK_GLOBAL_H

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

typedef double StackElem_t;
#define POISON_V NAN
#define PRINTF_T "lg"
#define TEXT_T_VALUE "double"


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



enum ERRORS_STACK
{
    stackNoErr,
    stackNoInit,
    stackOverflow,
    stackUnderflow,
    stackNoResize,
    stackDataPointerNull
    CANARY_PROTECTION(,
                      leftCanaryDataKilled,
                      rightCanaryDataKilled,
                      leftCanaryStackKilled,
                      rightCanaryStackKilled)
    HASH_PROTECTION(,
                    hashDataIncorrect,
                    hashStackIncorrect)
};

#endif